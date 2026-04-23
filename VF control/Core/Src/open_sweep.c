#include "open_sweep.h"
#include <math.h>
#include <float.h>

/* ====== CONFIG: choose current magnitude source ====== */
/* If you already compute Id/Iq each tick, set USE_IDQ = 1 and implement get_id_iq(). */
#define USE_IDQ   0

#if USE_IDQ
static void get_id_iq(float* id, float* iq); /* user-provide if USE_IDQ=1 */
#endif

extern TIM_HandleTypeDef htim1;
OLSweep sw;

/* ====== Helpers ====== */

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#define TWO_PI (2.0f * (float)M_PI)

/* Extended OLSweep struct additions (private fields) */
    // add these members in OLSweep:
    // float _theta_emul_at_min;
    // uint16_t _enc_count_at_min;
    // int counts_per_rev; // e.g., 4096

/* Use OLSweep_Init to set counts_per_rev */
    // s->counts_per_rev = 4096; // example
static inline float mech_count_to_elec_rad(uint16_t enc_count, int counts_per_rev, int pole_pairs)
{
    float theta_m = (2.0f * M_PI) * ((float)enc_count / (float)counts_per_rev); // mech angle
    float theta_e = fmodf((float)pole_pairs * theta_m, 2.0f * M_PI);
    if (theta_e < 0.0f) theta_e += 2.0f * M_PI;
    return theta_e;
}

static inline float wrap_0_2pi(float x)
{
    float y = fmodf(x, TWO_PI);
    if (y < 0.0f) y += TWO_PI;
    return y;
}

/* Clarke αβ from abc (balanced, a+b+c=0 assumption not required for magnitude) */
static inline void clarke_alpha_beta(float ia, float ib, float ic, float* a, float* b)
{
    /* α = ia
       β = (ia + 2*ib)/√3  (one common orthogonal form) */
    *a = ia;
    *b = (ia + 2.0f * ib) * 0.57735026919f; /* 1/√3 */
}

/* SVPWM: convert Vαβ and Vdc to normalized (0..1) center-aligned duties for A,B,C */
static void svpwm_vab_to_duty(float v_alpha, float v_beta, float vdc, float* da, float* db, float* dc)
{
    /* Scale to phase voltages (space vector): v_a = v_alpha
       v_b = -0.5 v_alpha + (√3/2) v_beta
       v_c = -0.5 v_alpha - (√3/2) v_beta */
    const float SQRT3 = 1.73205080757f;
    float va = v_alpha;
    float vb = -0.5f * v_alpha + 0.5f * SQRT3 * v_beta;
    float vc = -0.5f * v_alpha - 0.5f * SQRT3 * v_beta;

    /* Convert to duty by centering (min subtraction, then normalize by Vdc) */
    float v_min = fminf(va, fminf(vb, vc));
    float v_max = fmaxf(va, fmaxf(vb, vc));
    /* center so middle of linear region; zero-sequence injection (simple) */
    float v0 = 0.5f * (v_max + v_min);

    va -= v0; vb -= v0; vc -= v0;

    /* Normalize to [0..1] using half of DC bus (since leg sees ±Vdc/2 around center) */
    float k = (vdc > 1e-6f) ? (0.5f / (vdc * 0.5f)) : 0.0f; /* simplifies to 1/vdc */
    *da = 0.5f + va * k;
    *db = 0.5f + vb * k;
    *dc = 0.5f + vc * k;

    /* clamp */
    if (*da < 0.0f) *da = 0.0f; else if (*da > 1.0f) *da = 1.0f;
    if (*db < 0.0f) *db = 0.0f; else if (*db > 1.0f) *db = 1.0f;
    if (*dc < 0.0f) *dc = 0.0f; else if (*dc > 1.0f) *dc = 1.0f;
}

/* ====== Public API ====== */
void apply_duty_abc(float da, float db, float dc) 
{
    /**************************************************************/
    /* Calculating the duty cycles and finally setting the registers in TIM1 */
    FOC_Out.pwm_a = (uint16_t)(2500.0 * da);
    FOC_Out.pwm_b = (uint16_t)(2500.0 * db);
    FOC_Out.pwm_c = (uint16_t)(2500.0 * dc);

    if (FOC_Out.pwm_a <= 110)
        FOC_Out.pwm_a = 110;
    else if (FOC_Out.pwm_a >= 2349)
        FOC_Out.pwm_a = 2349;

    if (FOC_Out.pwm_b <= 110)
        FOC_Out.pwm_b = 110;
    else if (FOC_Out.pwm_b >= 2349)
        FOC_Out.pwm_b = 2349;

    if (FOC_Out.pwm_c <= 110)
        FOC_Out.pwm_c = 110;
    else if (FOC_Out.pwm_c >= 2349)
        FOC_Out.pwm_c = 2349;

    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,FOC_Out.pwm_a);
    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,FOC_Out.pwm_b);
    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,FOC_Out.pwm_c);
    /**************************************************************/
}

void get_phase_currents(float* ia, float* ib, float* ic)
{
    *ia = FOC_U.PhaseCurrent[2];
    *ib = FOC_U.PhaseCurrent[1];
    *ic = FOC_U.PhaseCurrent[0];
}

void OLSweep_Init(OLSweep* s)
{
    s->vtest_pct_of_vbus = 0.05f;   /* 5% of Vdc */
    s->step_deg          = 5.0f;   /* 5° per step (72 steps) */
    s->dwell_s           = 0.025f;  /* 25 ms per step */
    s->pole_pairs        = POLEPAIRS;       /* <- set your motor PP */
    s->dt_s              = 0.0001f; /* e.g., 10 kHz control */
    s->counts_per_rev    = 4096;

    s->theta_min_elec    = 0.0f;
    s->theta_offset      = 0.0f;
    s->done              = 0;

    s->_theta_e          = 0.0f;
    s->_vtest_V          = 0.0f;
    s->_Is_min           = FLT_MAX;
    s->_theta_at_min     = 0.0f;
    s->_t_in_step        = 0.0f;
    s->_step_rad         = s->step_deg * (float)M_PI / 180.0f;
}

void OLSweep_Start(OLSweep* s)
{
    s->_theta_e = 0.0f;
    s->_Is_min  = FLT_MAX;
    s->_theta_at_min = 0.0f;
    s->_t_in_step = 0.0f;
    s->_step_rad  = s->step_deg * (float)M_PI / 180.0f;
    s->done = 0;

    /* pick Vtest absolute now */
    s->_vtest_V = fmaxf(0.0f, s->vtest_pct_of_vbus) * 61.0f;
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET);
}

/* Modified OLSweep_Update signature to accept encoder count */
int OLSweep_Update(OLSweep* s, uint16_t enc_count)
{
    static uint16_t sweep_count = 0;
    if (s->done) return 1;

    /* 1) Build emulated test vector at current emulated electrical angle */
    float vtest = s->_vtest_V;
    float v_alpha = vtest * cosf(s->_theta_e);
    float v_beta  = vtest * sinf(s->_theta_e);

    /* 2) SVPWM -> duties and apply */
    float da, db, dc;
    float vdc = 61.0f;
    svpwm_vab_to_duty(v_alpha, v_beta, vdc, &da, &db, &dc);
    apply_duty_abc(da, db, dc);

    /* 3) Measure stator current magnitude (same as before) */
    float ia, ib, ic;
    get_phase_currents(&ia, &ib, &ic);
    float alpha, beta;
    clarke_alpha_beta(ia, ib, ic, &alpha, &beta);
    float Is = sqrtf(alpha*alpha + beta*beta);

    /* 4) Track minimum: now record both emulated and encoder angles at min */
    if (Is < s->_Is_min) {
        s->_Is_min = Is;
        s->_theta_at_min = s->_theta_e;            /* emulated electrical angle */
        s->_enc_count_at_min = enc_count;          /* encoder (mechanical) count sample */
        s->_theta_emul_at_min = s->_theta_e;
    }

    /* 5) Dwell timing and step angle (same as before) */
    s->_t_in_step += s->dt_s;
    if (s->_t_in_step >= s->dwell_s) {
        s->_t_in_step = 0.0f;
        s->_theta_e -= s->_step_rad;

        if (s->_theta_e <= -TWO_PI) {
            /* finished full sweep */
            s->_theta_e = 0.0f;
            sweep_count++;
        
            if (sweep_count >= 4)
            {            
                /* compute encoder electrical angle at the recorded min */
                float theta_enc_at_min = mech_count_to_elec_rad(s->_enc_count_at_min, s->counts_per_rev, s->pole_pairs);

                /* compute offset: theta_offset = theta_emul_at_min - theta_enc_at_min  (wrap) */
                float raw_offset = s->_theta_emul_at_min - theta_enc_at_min;
                /* wrap into 0..2pi */
                float theta_offset = fmodf(raw_offset, TWO_PI);
                if (theta_offset < 0.0f) theta_offset += TWO_PI;

                s->theta_min_elec = wrap_0_2pi(s->_theta_at_min); /* emulated angle of min */
                s->theta_offset   = theta_offset;
                s->done = 1;

                /* neutral PWM */
                apply_duty_abc(0.5f, 0.5f, 0.5f);

                return 1;
            }
        }
    }

    return 0;
}