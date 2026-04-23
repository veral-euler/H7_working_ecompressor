#ifndef __THROTTLE_MAPPING_H_
#define __THROTTLE_MAPPING_H_

#include "main.h"           /* for L_g global used in C file (remove if not needed) */

/* ========================= Config / Tunables ========================= */

/* Control loop timing (used for slew calculations) */
#ifndef DT_CTRL
#define DT_CTRL 0.0001f    /* default 10 kHz */
#endif

/* ADC throttle mapping - adapt to your ADC values */
#ifndef ADC_DEADBAND_MIN
#define ADC_DEADBAND_MIN  11926u
#endif
#ifndef ADC_MAX_EFFECTIVE
#define ADC_MAX_EFFECTIVE 42000u
#endif

/* Map to maximum Is (A) from throttle mapping */
#ifndef MAX_IS_CURRENT
#define MAX_IS_CURRENT 512.0f
#endif

/* Torque-to-current ratio (Nm / A) */
#ifndef TORQUE_RATIO
#define TORQUE_RATIO 0.15625f   /* your value; change if different */
#endif

/* Slew rates for Is (A per second) */
#ifndef IS_SLEW_RATE_ACCEL
#define IS_SLEW_RATE_ACCEL  220.0f
#endif
#ifndef IS_SLEW_RATE_DECEL
#define IS_SLEW_RATE_DECEL  5000.0f
#endif

/* Temperature derate thresholds */
#define CTRL_DERATE_START_C    75.0f
#define CTRL_FULL_DERATE_C     85.0f

#define MOTOR_DERATE_START_C   100.0f
#define MOTOR_FULL_DERATE_C    115.0f

/* Nominal/derated power (watts) */
#define NOMINAL_MAX_POWER_W         11000.0f
#define MIN_POWER_AT_FULL_DERATE_W  2000.0f

/* Temperature smoothing & hysteresis */
#define TEMP_IIR_ALPHA  0.01f
#define TEMP_HYSTERESIS_C 2.0f

/* Small omega floor to avoid division by zero */
#define OMEGA_M_FLOOR 1.0f
#define THROTTLE_CONTROL 1

#define SPEED_LIMIT_ECO_RPM        4500.0f   // eco limit
#define SPEED_LIMIT_SPORTS_RPM     6100.0f   // sports limit (if any)
#define SPEED_SOFT_BAND_RPM        350.0f    // start reducing torque this many RPM before vmax
#define SPEED_HYSTERESIS_RPM       50.0f     // hysteresis to avoid chattering
#define SPEED_REGEN_ABOVE_RPM      0.0f     // if above vmax by this, allow small regen
#define SPEED_REGEN_TORQUE_NM      0.0f    // small negative torque to bring speed down (Nm)

/* ===== Configurable parameters ===== */
#define DCLO_DEFAULT_EFF        0.90f    /* inverter+MOSFET efficiency estimate */
#define DCLO_DEFAULT_PF         0.90f    /* power factor (Iq-dominant ~1.0, use conservative <1) */
#define M_MIN_ESTIMATE          0.368f     /* avoid division by tiny modulation index */
#define M_MAX_SVPWM             0.866f   /* max modulation index for SVPWM */

/* ===== SOC derating knobs (tune these) ===== */
#define SOC_BREAK               0.50f    /* start derating below 40% SOC */
#define SOC_DERATE_MAX          0.25f    /* max multiplicative derate at 0% SOC (15%) */
#define SOC_GUARD_MAX_A         40.0f    /* additional absolute A of DC headroom to reserve at 0% SOC */

/* torque tables / points (example values; replace with your full arrays if needed) */
static const float speed_rpm_points[] = {
    0, 500, 1000, 1500, 2000, 2500,
    3000, 3500, 4000, 4500, 5000, 5500, 6000
};

/* nominal torque envelope (Nm) - example */
static const float torque_points[] = {
    80, 80, 78, 70, 50, 40,
    20, 18, 15, 13, 12, 10, 8
};

/* derated torque envelope (Nm) - example */
static const float torque_points_derating[] = {
    50, 45, 40, 35, 30, 20,
    15, 14, 10, 8, 5, 3, 2
};

static const int torque_table_len = sizeof(speed_rpm_points) / sizeof(speed_rpm_points[0]);

/* ========================= Mode Manager API ========================= */

typedef enum {
    MODE_ECO = 0,
    MODE_SPORTS = 1
} DriveMode_e;

/* Initialize mode manager (call once after GPIO configured).
   gpio_port / gpio_pin: pin used to select mode.
   sample_ms: sampling interval used for debounce (e.g., 50 ms).
   count_thresh: consecutive identical samples required to accept change (e.g., 3).
   active_high: set 1 if GPIO HIGH corresponds to SPORTS mode; set 0 if reversed.
*/
void ModeManager_Init(GPIO_TypeDef *gpio_port, uint16_t gpio_pin, uint32_t sample_ms, uint8_t count_thresh, uint8_t active_high);

/* Call periodically (every ~sample_ms) from main loop or slow timer (NOT from 10kHz ISR) */
void ModeManager_Tick(void);

/* Query active mode */
DriveMode_e ModeManager_GetMode(void);

/* Force mode from code (optional) */
void ModeManager_ForceMode(DriveMode_e m);

/* Mode parameters that throttle mapping will use */
typedef struct {
    float max_torque_nm;
    float nominal_max_power_w;
    float min_power_at_full_derate_w;
    float max_speed_rpm;
    float torque_scale;
} ModeParams_t;

/* Active mode parameters (exported from C file) */
extern ModeParams_t g_current_mode_params;

/* Apply mode settings programmatically */
void ApplyModeSettings(DriveMode_e mode);

float Apply_DCLO_IsLimit(float Is_ref_in,
                         float dc_limit_A,
                         float vbus_V,
                         float Vd_cmd,
                         float Vq_cmd);

/* ========================= Throttle Mapping API ========================= */

/**
 * @brief Compute Is (A) from throttle ADC + speed with envelope, temperature derating and slew limiting.
 * @param throttle_adc  Raw ADC throttle (units from ADC)
 * @param speed_rpm     Mechanical speed in RPM
 * @param prev_Is       previous Is output (for slew limiting)
 * @return              New Is value (A)
 */
float get_Is_with_envelope_slew(uint16_t throttle_adc, float speed_rpm, float prev_Is);

/* Optional speed ref helper (not required) */
float update_speed_ref(float speed_target_rpm, float current_speed_rpm, float accel_limit_rpm_per_sec, float decel_limit_rpm_per_sec, float dt_sec);

#endif /* __THROTTLE_MAPPING_H_ */
