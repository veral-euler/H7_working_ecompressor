#ifndef REGEN_MANAGERS_H
#define REGEN_MANAGERS_H

#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ===== Helpers ===== */
static inline float clampf(float x, float lo, float hi){
    return (x < lo) ? lo : (x > hi) ? hi : x;
}
static inline float slew_step(float target, float prev, float step){
    float d = target - prev;
    if (d >  step) return prev + step;
    if (d < -step) return prev - step;
    return target;
}

typedef struct {
    /* Tuning */
    float throttle_deadband;      /* 0..1 */
    float throttle_drop_thresh;   /* sudden drop */
    float speed_decel_thresh_rpm; /* |dSpeed| threshold */
    float torque_neg_thresh_nm;   /* T_ref < this => regen intent */
    float speed_min_enable_rpm;   /* don’t enable scheme below this */
    float clear_speed_rpm;        /* exit regen when slower than this */
    float accel_hysteresis;       /* extra margin to exit (throttle) */

    /* State */
    float prev_throttle;
    float prev_speed_rpm;
    uint8_t decel_active;         /* latched */
} DecelDetector_t;


/* ===== Iq Regen Manager ===== */
typedef struct {
    /* Tuning */
    float neg_iq_limit_A;      /* most negative during regen (e.g., -100 A) */
    float to_zero_step_A;      /* step per tick toward 0 from +Iq */
    float to_negative_step_A;  /* step per tick deeper negative */
    float iq_slew_A;           /* final output slew per tick */
    float low_speed_release_rpm;/* when below this, unwind back to 0A */
    uint8_t allow_regen_forward_only; /* 1 => disable in reverse */

    /* State */
    float iq_out_prev;
    uint8_t stage;             /* 0: idle, 1: to zero, 2: to negative, 3: hold, 4: unwind */
} IqRegenMgr_t;

/* ===== Id Regen Manager (positive flux boost during regen) ===== */
typedef struct {
    /* Tuning */
    float pos_id_limit_A;      /* cap during regen (+10..+40 A typical) */
    float to_pos_step_A;       /* step per tick toward +Id target */
    float to_lut_step_A;       /* step per tick back toward LUT when not in regen */
    float id_slew_A;           /* final output slew per tick */
    float low_speed_release_rpm;/* mirror of Iq manager to release */
    uint8_t allow_regen_forward_only;

    /* State */
    float id_out_prev;
    uint8_t stage;             /* 0: follow LUT, 1: push positive, 2: hold, 3: return to LUT */
} IdRegenMgr_t;

void DecelDetector_Init(DecelDetector_t* d,
                                      float throttle_deadband,
                                      float throttle_drop_thresh,
                                      float speed_decel_thresh_rpm,
                                      float torque_neg_thresh_nm,
                                      float speed_min_enable_rpm,
                                      float clear_speed_rpm,
                                      float accel_hysteresis);

uint8_t DecelDetector_Update(DecelDetector_t* d,
                                           float throttle_0to1,
                                           float torque_ref_nm,
                                           float speed_rpm,
                                           uint8_t accel_request_hint);

void IqRegenMgr_Init(IqRegenMgr_t* m,
                                   float neg_iq_limit_A,
                                   float to_zero_step_A,
                                   float to_negative_step_A,
                                   float iq_slew_A,
                                   float low_speed_release_rpm,
                                   uint8_t forward_only);

float IqRegenMgr_Step(IqRegenMgr_t* m,
                                    uint8_t decel_active,
                                    int8_t direction_sign,
                                    float speed_rpm,
                                    float iq_lut_ref); /* positive when motoring forward */

void IdRegenMgr_Init(IdRegenMgr_t* m,
                                   float pos_id_limit_A,
                                   float to_pos_step_A,
                                   float to_lut_step_A,
                                   float id_slew_A,
                                   float low_speed_release_rpm,
                                   uint8_t forward_only);

float IdRegenMgr_Step(IdRegenMgr_t* m,
                                    uint8_t decel_active,
                                    int8_t direction_sign,
                                    float speed_rpm,
                                    float id_lut_ref);

/* ===== One-time init (e.g., in main or system init) ===== */
extern DecelDetector_t gDecel;
extern IqRegenMgr_t    gIqMgr;
extern IdRegenMgr_t    gIdMgr;

#ifdef __cplusplus
}
#endif
#endif /* REGEN_MANAGERS_H */
