#ifndef OL_SWEEP_H
#define OL_SWEEP_H

#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ===== User I/O (implement these for your platform) ===== */

/* Return latest phase currents (A). If you prefer Id/Iq, see config in .c */
void get_phase_currents(float* ia, float* ib, float* ic);

/* Return measured DC bus voltage (V) */
float get_vbus_V(void);

/* Apply normalized (0..1) center-aligned SVPWM duties to TIM1 CH1/2/3.
   You can replace body to write TIM1->CCR1/2/3 directly. */
void apply_duty_abc(float da, float db, float dc);

/* ===== API ===== */

typedef struct {
    /* Configuration */
    float vtest_pct_of_vbus;   /* e.g., 0.05 = 5% of Vbus */
    float step_deg;            /* angle step, e.g., 5.0 */
    float dwell_s;             /* dwell per step, e.g., 0.025 s */
    int   pole_pairs;          /* motor pole pairs */
    int   counts_per_rev;     /* encoder counts per mechanical revolution */

    /* Runtime (set by caller) */
    float dt_s;                /* calling period (FOC/PWM interrupt period) */

    /* Results */
    float theta_min_elec;      /* electrical angle where Is is minimal (0..2π) */
    float _theta_emul_at_min;
    float _enc_count_at_min;
    float theta_offset;        /* recommended Park offset = -theta_min_elec (wrapped 0..2π) */
    int   done;                /* becomes 1 when sweep finishes */

    /* Internal state (private) */
    float _theta_e;            /* current electrical angle */
    float _vtest_V;            /* test voltage magnitude (V) */
    float _Is_min;             /* best Is found so far */
    float _theta_at_min;       /* angle at min Is */
    float _t_in_step;          /* dwell timer */
    float _step_rad;           /* step in radians */
} OLSweep;

/* Initialize with defaults */
void OLSweep_Init(OLSweep* s);

/* Start a full sweep 0..2π */
void OLSweep_Start(OLSweep* s);

/* Call every control tick (dt = s->dt_s).
   Returns 0 while sweeping, 1 when finished. */
int OLSweep_Update(OLSweep* s, uint16_t enc_count);

extern OLSweep sw;

#ifdef __cplusplus
}
#endif

#endif /* OL_SWEEP_H */
