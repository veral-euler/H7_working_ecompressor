#include "Throttle_mapping.h"
#include <float.h>

/* NOTE: this file expects the global L_g object (or similar) to contain
   Controller_Temp and Motor_Temp members (float).
   If your project uses a different name, replace L_g.Controller_Temp / L_g.Motor_Temp accordingly.
extern struct {
    float Controller_Temp;
    float Motor_Temp;
} L_g;
*/

/* ================= Helper Functions ================= */

/* Estimate modulation index (m) from commanded Vd/Vq and bus voltage. */
static inline float estimate_mod_index(float Vd_cmd, float Vq_cmd, float vbus_V)
{
    if (vbus_V <= FLT_EPSILON) return M_MIN_ESTIMATE;

    float Vph_peak = sqrtf(Vd_cmd * Vd_cmd + Vq_cmd * Vq_cmd);  /* peak voltage vector */
    float m = (1.73205f * Vph_peak) / vbus_V;                   /* normalized to DC bus (≈ √3 * Vphase_pk / Vdc) */

    if (m < M_MIN_ESTIMATE) m = M_MIN_ESTIMATE;
    if (m > M_MAX_SVPWM)   m = M_MAX_SVPWM;

    return m;
}

/* Convert DCLO (A) to allowable stator RMS current. */
static inline float dclo_to_Is_cap(float dc_limit_A,
                                   float vbus_V,
                                   float m_est,
                                   float eff,
                                   float pf)
{
    if (dc_limit_A <= 0.0f || vbus_V <= FLT_EPSILON) return 0.0f;

    if (eff <= 0.0f || eff > 1.0f) eff = DCLO_DEFAULT_EFF;
    if (pf  <= 0.0f || pf  > 1.0f) pf  = DCLO_DEFAULT_PF;

    const float two_sqrt2 = 2.8284271247461903f;  /* 2*sqrt(2) */
    float factor = (two_sqrt2 * eff) / (3.0f * m_est * pf);

    return dc_limit_A * factor;  /* allowable stator RMS current */
}

/* ------------------------------------------------------------------
   Small helpers (interpolator, clamp, Pmax calc)
   ------------------------------------------------------------------ */
static inline float clampf_local(float v, float lo, float hi) {
    return (v < lo) ? lo : (v > hi) ? hi : v;
}

/* linear interpolation for monotonic x[] */
static float interpolate(const float *x, const float *y, int n, float xq)
{
    if (n <= 0) return 0.0f;
    if (xq <= x[0]) return y[0];
    if (xq >= x[n-1]) return y[n-1];
    for (int i = 0; i < n-1; ++i) {
        if (xq >= x[i] && xq <= x[i+1]) {
            float t = (xq - x[i]) / (x[i+1] - x[i]);
            return y[i] + t * (y[i+1] - y[i]);
        }
    }
    return y[n-1];
}

/* linear blend from nominal to min power across a temperature interval */
static inline float calc_Pmax_from_temp(float temp, float t_start, float t_full)
{
    if (temp <= t_start) return NOMINAL_MAX_POWER_W;
    if (temp >= t_full)  return MIN_POWER_AT_FULL_DERATE_W;
    float r = (temp - t_start) / (t_full - t_start);
    return (1.0f - r) * NOMINAL_MAX_POWER_W + r * MIN_POWER_AT_FULL_DERATE_W;
}

/* Headroom multiplier vs SOC: 1.0 above SOC_BREAK, down to (1 - SOC_DERATE_MAX) at 0% */
static inline float soc_headroom(float soc_pu)
{
    float soc = clampf_local(soc_pu, 0.0f, 1.0f);
    if (soc >= SOC_BREAK) return 1.0f;
    float k = (SOC_BREAK - soc) / SOC_BREAK;   /* 0 at break, 1 at 0% */
    return 1.0f - (SOC_DERATE_MAX * k);
}

/* Absolute guard band (A) to subtract from DCLO before converting to Is_cap */
static inline float soc_guard_A(float soc_pu)
{
    float soc = clampf_local(soc_pu, 0.0f, 1.0f);
    if (soc >= SOC_BREAK) return 0.0f;
    float k = (SOC_BREAK - soc) / SOC_BREAK;   /* 0..1 */
    return SOC_GUARD_MAX_A * k;
}

/* ------------------------------------------------------------------
   Temperature filter state (persistent)
   ------------------------------------------------------------------ */
static float s_ctrl_temp_filt = 25.0f;
static float s_motor_temp_filt = 25.0f;

/* ------------------------------------------------------------------
   Mode manager implementation & mode parameters
   ------------------------------------------------------------------ */

/* Forward declarations from header */
ModeParams_t g_current_mode_params;

/* Mode manager internals */
static GPIO_TypeDef *s_mode_gpio_port = GPIOD;
static uint16_t      s_mode_gpio_pin  = GPIO_PIN_2;
static uint32_t      s_mode_sample_ms = 50;
static uint8_t       s_mode_count_thresh = 3;
static uint8_t       s_mode_active_high = 0;
static uint8_t       s_mode_sample_count = 0;
static uint8_t       s_mode_last_sample = 0;
static uint32_t      s_mode_last_tick = 0;
static DriveMode_e   s_active_mode = MODE_ECO;

/* Default per-mode parameters — tune these values */
static const ModeParams_t s_mode_params[2] = {
    /* MODE_ECO */
    {
        .max_torque_nm = 44.0f,
        .nominal_max_power_w = 4500.0f,
        .min_power_at_full_derate_w = 2000.0f,
        .max_speed_rpm = 4500.0f, /* change according to your rpm->kmph */
        .torque_scale = 1.0f
    },
    /* MODE_SPORTS */
    {
        .max_torque_nm = 80.0f,
        .nominal_max_power_w = 11000.0f,
        .min_power_at_full_derate_w = 2000.0f,
        .max_speed_rpm = 6100.0f,
        .torque_scale = 1.0f
    }
};

void ModeManager_Init(GPIO_TypeDef *gpio_port, uint16_t gpio_pin, uint32_t sample_ms, uint8_t count_thresh, uint8_t active_high)
{
    s_mode_gpio_port = gpio_port;
    s_mode_gpio_pin = gpio_pin;
    s_mode_sample_ms = sample_ms;
    s_mode_count_thresh = count_thresh;
    s_mode_active_high = active_high ? 1 : 0;
    s_mode_sample_count = 0;
    s_mode_last_sample = 0;
    s_mode_last_tick = HAL_GetTick();
    /* read initial pin */
    if (s_mode_gpio_port) {
        uint8_t v = (uint8_t)(HAL_GPIO_ReadPin(s_mode_gpio_port, s_mode_gpio_pin) ? 1 : 0);
        s_mode_last_sample = v;
        if ((s_mode_active_high && v) || (!s_mode_active_high && !v)) s_active_mode = MODE_SPORTS;
        else s_active_mode = MODE_ECO;
    }
    /* apply initial mode params */
    g_current_mode_params = s_mode_params[s_active_mode];
}

void ModeManager_Tick(void)
{
    if (!s_mode_gpio_port) return;
    uint32_t now = HAL_GetTick();
    if ((now - s_mode_last_tick) < s_mode_sample_ms) return;
    s_mode_last_tick = now;
    uint8_t v = (uint8_t)(HAL_GPIO_ReadPin(s_mode_gpio_port, s_mode_gpio_pin) ? 1 : 0);
    if (v == s_mode_last_sample) {
        if (s_mode_sample_count < 255) s_mode_sample_count++;
    } else {
        s_mode_last_sample = v;
        s_mode_sample_count = 1;
    }
    if (s_mode_sample_count >= s_mode_count_thresh) {
        DriveMode_e new_mode = (((s_mode_active_high && v) || (!s_mode_active_high && !v)) && (curr_thr_state == DRIVE)) ? MODE_SPORTS : MODE_ECO;
        if (new_mode != s_active_mode) {
            s_active_mode = new_mode;
            g_current_mode_params = s_mode_params[s_active_mode];
            /* optional: add logging / status update here */
        }
        s_mode_sample_count = s_mode_count_thresh;
    }
}

DriveMode_e ModeManager_GetMode(void) { return s_active_mode; }
void ModeManager_ForceMode(DriveMode_e m) { if (m <= MODE_SPORTS) { s_active_mode = m; g_current_mode_params = s_mode_params[s_active_mode]; } }
void ApplyModeSettings(DriveMode_e mode) { if (mode <= MODE_SPORTS) g_current_mode_params = s_mode_params[mode]; }

/* ================= Public API ================= */

/**
 * @brief Enforce DCLO limit on stator current reference with SOC-aware headroom.
 *
 * @param Is_ref_in     Requested stator RMS current (A)
 * @param dc_limit_A    DCLO (BMS DC current limit in A)
 * @param vbus_V        Measured DC bus voltage (V)
 * @param Vd_cmd        Commanded D-axis voltage (V)
 * @param Vq_cmd        Commanded Q-axis voltage (V)
 * @param soc_pu        State-of-charge in per-unit (0.0 .. 1.0)
 * @return              Limited stator RMS current reference (A)
 */
float Apply_DCLO_IsLimit_SOC(float Is_ref_in,
                             float dc_limit_A,
                             float vbus_V,
                             float Vd_cmd,
                             float Vq_cmd,
                             float soc_pu)
{
    /* 0) SOC-aware DC headroom: subtract absolute guard A */
    float dc_guard = soc_guard_A(soc_pu);
    float dc_limit_eff_A = dc_limit_A - dc_guard;
    if (dc_limit_eff_A < 0.0f) dc_limit_eff_A = 0.0f;

    /* 1) Estimate modulation index from Vdq */
    float m_est = estimate_mod_index(Vd_cmd, Vq_cmd, vbus_V);

    float eff_eff = DCLO_DEFAULT_EFF * soc_headroom(soc_pu);

    /* 2) Convert DCLO (effective) to stator RMS current cap */
    float Is_cap = dclo_to_Is_cap(dc_limit_eff_A, vbus_V, m_est,
                                  eff_eff, DCLO_DEFAULT_PF);

    /* 3) Apply SOC multiplicative headroom (more conservative at low SOC) */
    Is_cap *= soc_headroom(soc_pu);

    /* 4) Limit Is_ref */
    return (Is_ref_in > Is_cap) ? Is_cap : Is_ref_in;
}

/* Apply soft speed limit based on active mode.
 * requested_torque_nm: torque that would be used normally (from envelope / driver)
 * speed_rpm: measured speed (mechanical)
 * mode_params: pointer to current mode params (g_current_mode_params)
 *
 * Returns adjusted torque in Nm.
 *
 * Behavior:
 *  - If speed < (vmax - soft_band): pass through.
 *  - If speed between (vmax - soft_band) and vmax: scale torque down smoothly to 0 at vmax.
 *  - If speed slightly above vmax and SPEED_REGEN_ABOVE_RPM > 0: apply small negative torque (regen) to bring speed down.
 *  - Hysteresis prevents chatter when speed sits near the boundary.
 */
static float apply_speed_limit(float requested_torque_nm, float speed_rpm, const ModeParams_t *mode_params)
{
    static float last_vmax_used = 0.0f;
    static float exit_thresh = 0.0f;

    float vmax = mode_params->max_speed_rpm;
    float soft_band = SPEED_SOFT_BAND_RPM;
    float hyster = SPEED_HYSTERESIS_RPM;

    /* If vmax changed (mode switch), reset exit threshold */
    if (vmax != last_vmax_used) {
        last_vmax_used = vmax;
        exit_thresh = vmax - soft_band - hyster;
    }

    /* If speed below (vmax - soft_band - hyster) -> normal */
    if (speed_rpm <= (vmax - soft_band - hyster)) {
        return requested_torque_nm;
    }

    /* If speed above (vmax + hyster) -> strictly enforce */
    if (speed_rpm > (vmax + hyster)) {
        /* optional regen when significantly above vmax */
        if (SPEED_REGEN_ABOVE_RPM > 0 && (speed_rpm > (vmax + SPEED_REGEN_ABOVE_RPM))) {
            return SPEED_REGEN_TORQUE_NM; /* negative torque for regen (tune carefully) */
        } else {
            /* clamp to zero torque (no positive torque) */
            return fminf(requested_torque_nm, 0.0f);
        }
    }

    /* In soft band (with hysteresis around edges) -> scale smoothly */
    float start = vmax - soft_band;   /* where scaling begins */
    float end   = vmax;               /* where positive torque goes to zero */

    /* Add hysteresis so we don't bounce around start-end */
    float s = start;
    float e = end + hyster;

    if (speed_rpm <= s) {
        return requested_torque_nm;
    } else if (speed_rpm >= e) {
        /* above end with hysteresis still: clamp */
        return fminf(requested_torque_nm, 0.0f);
    } else {
        /* scale factor t = 0 at s -> 1 at e (we want torque * (1 - t)) */
        float t = (speed_rpm - s) / (e - s);
        t = clampf_local(t, 0.0f, 1.0f);
        float scale = 1.0f - t; /* scalar applied to requested torque */
        float adjusted = requested_torque_nm * scale;
        /* never allow adjusted > requested (shouldn't), ensure we don't produce tiny positive torque if we want zero */
        if (adjusted < 1e-6f && requested_torque_nm > 0.0f) adjusted = 0.0f;
        return adjusted;
    }
}


/* ------------------------------------------------------------------
   Main function: get_Is_with_envelope_slew
   - mode-aware: uses g_current_mode_params
   - temperature derate with "first-trigger locks" state machine
   - slew limiting on final Is
   ------------------------------------------------------------------ */
float get_Is_with_envelope_slew(uint16_t throttle_adc, float speed_rpm, float prev_Is)
{
    /* 1) Map throttle ADC -> requested Is */
    float Is_req = 0.0f;
    if (throttle_adc > ADC_DEADBAND_MIN) {
        uint32_t adc = throttle_adc;
        if (adc > ADC_MAX_EFFECTIVE) adc = ADC_MAX_EFFECTIVE;
        Is_req = ((float)(adc - ADC_DEADBAND_MIN) / (float)(ADC_MAX_EFFECTIVE - ADC_DEADBAND_MIN)) * MAX_IS_CURRENT;
    }

    /* 2) Lookup torque envelope (nominal and derated) */
    int n = torque_table_len;
    float torque_env_nom = interpolate(speed_rpm_points, torque_points, n, speed_rpm);
    float torque_env_der = interpolate(speed_rpm_points, torque_points_derating, n, speed_rpm);

    /* 3) Filter temperatures */
    float a = TEMP_IIR_ALPHA;
    if (a <= 0.0f) a = 0.01f;
    if (a > 1.0f) a = 1.0f;
    /* NOTE: ensure L_g.Controller_Temp and L_g.Motor_Temp exist in your code */
    s_ctrl_temp_filt  += a * (L_g.Controller_Temp - s_ctrl_temp_filt);
    s_motor_temp_filt += a * (L_g.Motor_Temp - s_motor_temp_filt);

    /* ----------------------------------------------------------------
       Derate state machine (locks to whichever sensor triggers first)
       0 = none, 1 = ctrl, 2 = motor
       ---------------------------------------------------------------- */
    static int s_local_derate_state = 0;
    if (s_local_derate_state == 0) {
        if (s_ctrl_temp_filt >= CTRL_DERATE_START_C) s_local_derate_state = 1;
        else if (s_motor_temp_filt >= MOTOR_DERATE_START_C) s_local_derate_state = 2;
    } else if (s_local_derate_state == 1) {
        ctlr_derate_flag = 1;

        if ((s_ctrl_temp_filt < (CTRL_DERATE_START_C - TEMP_HYSTERESIS_C)) && (s_motor_temp_filt < MOTOR_DERATE_START_C)) {
            s_local_derate_state = 0;
            ctlr_derate_flag = 0;
        } else if (s_motor_temp_filt >= MOTOR_DERATE_START_C) {
            /* optional: allow switching to motor derate if motor crosses start */
            s_local_derate_state = 2;
            ctlr_derate_flag = 0;
        }
    } else if (s_local_derate_state == 2) {
        motor_derate_flag = 1;

        if ((s_motor_temp_filt < (MOTOR_DERATE_START_C - TEMP_HYSTERESIS_C)) && (s_ctrl_temp_filt < CTRL_DERATE_START_C)) {
            s_local_derate_state = 0;
            motor_derate_flag = 0;
        } else if (s_ctrl_temp_filt >= CTRL_DERATE_START_C) {

        }
    }

    /* 4) Choose envelope & Pmax based on derate state (use g_current_mode_params) */
    float torque_from_envelope = torque_env_nom;
    float Pmax_effective = g_current_mode_params.nominal_max_power_w;

    if (s_local_derate_state == 1) {
        float t_ctrl = clampf_local((s_ctrl_temp_filt - CTRL_DERATE_START_C) / (CTRL_FULL_DERATE_C - CTRL_DERATE_START_C), 0.0f, 1.0f);
        torque_from_envelope = (1.0f - t_ctrl) * torque_env_nom + t_ctrl * torque_env_der;
        Pmax_effective = calc_Pmax_from_temp(s_ctrl_temp_filt, CTRL_DERATE_START_C, CTRL_FULL_DERATE_C);
    } else if (s_local_derate_state == 2) {
        float t_motor = clampf_local((s_motor_temp_filt - MOTOR_DERATE_START_C) / (MOTOR_FULL_DERATE_C - MOTOR_DERATE_START_C), 0.0f, 1.0f);
        torque_from_envelope = (1.0f - t_motor) * torque_env_nom + t_motor * torque_env_der;
        Pmax_effective = calc_Pmax_from_temp(s_motor_temp_filt, MOTOR_DERATE_START_C, MOTOR_FULL_DERATE_C);
    } else {
        torque_from_envelope = torque_env_nom;
        Pmax_effective = g_current_mode_params.nominal_max_power_w;
    }

    /* 5) Apply mode-specific scale and clamp */
    torque_from_envelope *= g_current_mode_params.torque_scale;
    if (torque_from_envelope > g_current_mode_params.max_torque_nm) torque_from_envelope = g_current_mode_params.max_torque_nm;

    /* 6) Power cap -> torque */
    float omega_m = fabsf(speed_rpm) * (2.0f * 3.14159265358979323846f / 60.0f);
    if (omega_m < OMEGA_M_FLOOR) omega_m = OMEGA_M_FLOOR;
    float torque_from_power = Pmax_effective / omega_m;

    /* 7) Final torque limit */
    float torque_lim = torque_from_envelope;
    if (torque_from_power < torque_lim) torque_lim = torque_from_power;

    /* apply per-mode speed soft limiter to torque_lim (prevents coasting above vmax) */
    float adjusted_torque_lim = apply_speed_limit(torque_lim, speed_rpm, &g_current_mode_params);
    /* now use adjusted_torque_lim as the final torque cap */
    torque_lim = adjusted_torque_lim;

    /* 8) Convert torque limit to Is limit */
    float Is_lim = torque_lim / TORQUE_RATIO;

    Is_lim = Apply_DCLO_IsLimit_SOC(Is_lim, (FOC_Out.can_DCLO * 1.1f), 62.0f, FOC_Y.Vd_FF, FOC_Y.Vq_FF, FOC_Out.pack_SOC);

    if (Is_lim < 0.0f) Is_lim = 0.0f;

    /* 9) Apply to Is_req */
    if (Is_req > Is_lim) Is_req = Is_lim;

    /* 10) Slew limiting */
    float delta = Is_req - prev_Is;
    float max_accel_step = IS_SLEW_RATE_ACCEL * DT_CTRL;
    float max_decel_step = IS_SLEW_RATE_DECEL * DT_CTRL;
    if (delta > max_accel_step) delta = max_accel_step;
    else if (delta < -max_decel_step) delta = -max_decel_step;

    float Is_out = prev_Is + delta;

    if (Is_out >= 512.0f) {
        Is_out = 512.0f;
    }
    
    return Is_out;
}

/* Optional helper: simple speed reference slew (unchanged) */
float update_speed_ref(float speed_target_rpm, float current_speed_rpm, float accel_limit_rpm_per_sec, float decel_limit_rpm_per_sec, float dt_sec)
{
    float delta = speed_target_rpm - current_speed_rpm;
    if (delta > 0) {
        float max_inc = accel_limit_rpm_per_sec * dt_sec;
        if (delta > max_inc) delta = max_inc;
    } else {
        float max_dec = decel_limit_rpm_per_sec * dt_sec;
        if (delta < -max_dec) delta = -max_dec;
    }
    return current_speed_rpm + delta;
}
