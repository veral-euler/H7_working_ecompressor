#include "Regen_Manager.h"

/* ===== One-time init (e.g., in main or system init) ===== */
DecelDetector_t gDecel;
IqRegenMgr_t    gIqMgr;
IdRegenMgr_t    gIdMgr;

/* ===== Decel Detector (shared by both managers) ===== */
void DecelDetector_Init(DecelDetector_t* d,
                                      float throttle_deadband,
                                      float throttle_drop_thresh,
                                      float speed_decel_thresh_rpm,
                                      float torque_neg_thresh_nm,
                                      float speed_min_enable_rpm,
                                      float clear_speed_rpm,
                                      float accel_hysteresis)
{
    d->throttle_deadband       = throttle_deadband;
    d->throttle_drop_thresh    = throttle_drop_thresh;
    d->speed_decel_thresh_rpm  = speed_decel_thresh_rpm;
    d->torque_neg_thresh_nm    = torque_neg_thresh_nm;
    d->speed_min_enable_rpm    = speed_min_enable_rpm;
    d->clear_speed_rpm         = clear_speed_rpm;
    d->accel_hysteresis        = accel_hysteresis;

    d->prev_throttle  = 0.0f;
    d->prev_speed_rpm = 0.0f;
    d->decel_active   = 0u;
}

/* Call every control tick; returns 1 when in decel/regen mode */
uint8_t DecelDetector_Update(DecelDetector_t* d,
                                           float throttle_0to1,
                                           float torque_ref_nm,
                                           float speed_rpm,
                                           uint8_t accel_request_hint)
{
    float dThrottle = d->prev_throttle - throttle_0to1;     /* + when dropping */
    float dSpeed    = d->prev_speed_rpm - speed_rpm;        /* + when slowing  */

    uint8_t enter = 0, exit = 0;

    if ( (dThrottle > d->throttle_drop_thresh) ||
         (throttle_0to1 < d->throttle_deadband && speed_rpm > d->speed_min_enable_rpm) ||
         (torque_ref_nm < d->torque_neg_thresh_nm) ||
         (dSpeed > d->speed_decel_thresh_rpm && throttle_0to1 < 0.2f) )
    {
        enter = 1;
    }

    if (accel_request_hint ||
        throttle_0to1 > (d->throttle_deadband + d->accel_hysteresis) ||
        speed_rpm < d->clear_speed_rpm)
    {
        exit = 1;
    }

    if (exit) d->decel_active = 0u;
    else if (enter && speed_rpm > d->speed_min_enable_rpm) d->decel_active = 1u;

    d->prev_throttle  = throttle_0to1;
    d->prev_speed_rpm = speed_rpm;
    return d->decel_active;
}

void IqRegenMgr_Init(IqRegenMgr_t* m,
                                   float neg_iq_limit_A,
                                   float to_zero_step_A,
                                   float to_negative_step_A,
                                   float iq_slew_A,
                                   float low_speed_release_rpm,
                                   uint8_t forward_only)
{
    m->neg_iq_limit_A      = neg_iq_limit_A;
    m->to_zero_step_A      = to_zero_step_A;
    m->to_negative_step_A  = to_negative_step_A;
    m->iq_slew_A           = iq_slew_A;
    m->low_speed_release_rpm = low_speed_release_rpm;
    m->allow_regen_forward_only = forward_only;

    m->iq_out_prev = 0.0f;
    m->stage = 0u;
}

/* direction_sign: +1 = forward, -1 = reverse mechanical direction */
float IqRegenMgr_Step(IqRegenMgr_t* m,
                                    uint8_t decel_active,
                                    int8_t direction_sign,
                                    float speed_rpm,
                                    float iq_lut_ref) /* positive when motoring forward */
{
    /* Optional: block regen in reverse (per your policy) */
    uint8_t regen_allowed = (m->allow_regen_forward_only) ? (direction_sign > 0) : 1;

    float cmd = iq_lut_ref;

    if (!decel_active || !regen_allowed) {
        /* When not decelling or regen not allowed: either hold if in unwind stage or pass-through */
        if (m->stage == 4u) {
            /* keep unwinding to zero */
        } else {
            m->stage = 0u;
            cmd = iq_lut_ref;
            float out = slew_step(cmd, m->iq_out_prev, m->iq_slew_A);
            m->iq_out_prev = out;
            FOC_U.MTPA_PID.Low_Limit_torque_PID = 0.0f;
            return out;
        }

        FOC_U.MTPA_PID.Low_Limit_torque_PID += 0.0001f;

        if (FOC_U.MTPA_PID.Low_Limit_torque_PID >= 0.0f) {
            FOC_U.MTPA_PID.Up_Limit_torque_PID = 0.0f;
        }
    }

    switch (m->stage)
    {
        case 0u: /* idle -> start toward zero */
            m->stage = 1u;
            /* fallthrough */
        case 1u: /* drive from current (likely +) toward 0 */
        {
            float toward0 = (m->iq_out_prev > 0.0f)
                            ? (m->iq_out_prev - m->to_zero_step_A)
                            : (m->iq_out_prev + m->to_zero_step_A); /* handles edge cases */
            if ((m->iq_out_prev >= 0.0f && toward0 <= 0.0f) ||
                (m->iq_out_prev <= 0.0f && toward0 >= 0.0f))
            {
                toward0 = 0.0f;
                m->stage = 2u; /* proceed to negative push */
            }

            FOC_U.MTPA_PID.Low_Limit_torque_PID -= 0.0001f;

            if (FOC_U.MTPA_PID.Low_Limit_torque_PID <= -61.0f) {
                FOC_U.MTPA_PID.Up_Limit_torque_PID = -61.0f;
            }

            cmd = toward0;
        } break;

        case 2u: /* push into negative */
        {
            float deeper = m->iq_out_prev - m->to_negative_step_A; /* more negative */
            cmd = clampf(deeper, m->neg_iq_limit_A, 0.0f);
            if (cmd <= m->neg_iq_limit_A + 1e-6f) {
                m->stage = 3u; /* reached target band; hold */
            }
        } break;

        case 3u: /* hold negative (until low speed) */
        {
            cmd = clampf(m->iq_out_prev, m->neg_iq_limit_A, 0.0f);
            if (speed_rpm < m->low_speed_release_rpm) {
                m->stage = 4u; /* begin unwind to 0A */
            }
        } break;

        case 4u: /* unwind back to 0A when slow */
        {
            float toward0 = (m->iq_out_prev < 0.0f)
                            ? (m->iq_out_prev + m->to_zero_step_A)
                            : (m->iq_out_prev - m->to_zero_step_A);
            if ((m->iq_out_prev <= 0.0f && toward0 >= 0.0f) ||
                (m->iq_out_prev >= 0.0f && toward0 <= 0.0f))
            {
                toward0 = 0.0f;
                m->stage = 0u; /* done */
            }

            FOC_U.MTPA_PID.Low_Limit_torque_PID += 0.0001f;

            if (FOC_U.MTPA_PID.Low_Limit_torque_PID >= 0.0f) {
                FOC_U.MTPA_PID.Up_Limit_torque_PID = 0.0f;
            }

            cmd = toward0;
        } break;

        default:
            m->stage = 0u;
            cmd = iq_lut_ref;
            break;
    }

    float out = slew_step(cmd, m->iq_out_prev, m->iq_slew_A);
    m->iq_out_prev = out;
    return out;
}

void IdRegenMgr_Init(IdRegenMgr_t* m,
                                   float pos_id_limit_A,
                                   float to_pos_step_A,
                                   float to_lut_step_A,
                                   float id_slew_A,
                                   float low_speed_release_rpm,
                                   uint8_t forward_only)
{
    m->pos_id_limit_A      = pos_id_limit_A;
    m->to_pos_step_A       = to_pos_step_A;
    m->to_lut_step_A       = to_lut_step_A;
    m->id_slew_A           = id_slew_A;
    m->low_speed_release_rpm = low_speed_release_rpm;
    m->allow_regen_forward_only = forward_only;

    m->id_out_prev = 0.0f;
    m->stage = 0u;
}

/* direction_sign: +1 = forward, -1 = reverse */
float IdRegenMgr_Step(IdRegenMgr_t* m,
                                    uint8_t decel_active,
                                    int8_t direction_sign,
                                    float speed_rpm,
                                    float id_lut_ref)
{
    uint8_t regen_allowed = (m->allow_regen_forward_only) ? (direction_sign > 0) : 1;
    float cmd = id_lut_ref;

    if (!decel_active || !regen_allowed) {
        /* Not in regen: walk back to LUT smoothly */
        m->stage = 3u;
    } else {
        if (m->stage == 0u || m->stage == 3u) m->stage = 1u; /* start push positive */
    }

    switch (m->stage)
    {
        case 0u: /* follow LUT */
        default:
        {
            cmd = id_lut_ref;
        } break;

        case 1u: /* push toward +Id */
        {
            float target = m->pos_id_limit_A;
            float toward = m->id_out_prev + m->to_pos_step_A;
            if (toward > target) { toward = target; m->stage = 2u; }
            cmd = toward;
        } break;

        case 2u: /* hold positive until we decide to release (low speed / exit decel) */
        {
            cmd = clampf(m->id_out_prev, 0.0f, m->pos_id_limit_A);
            if (speed_rpm < m->low_speed_release_rpm || !decel_active || !regen_allowed) {
                m->stage = 3u;
            }
        } break;

        case 3u: /* return to LUT value smoothly */
        {
            float toward = (id_lut_ref > m->id_out_prev)
                           ? (m->id_out_prev + m->to_lut_step_A)
                           : (m->id_out_prev - m->to_lut_step_A);
            /* Snap when crossed */
            if ((m->id_out_prev <= id_lut_ref && toward >= id_lut_ref) ||
                (m->id_out_prev >= id_lut_ref && toward <= id_lut_ref))
            {
                toward = id_lut_ref;
                m->stage = 0u; /* done */
            }
            cmd = toward;
        } break;
    }

    float out = slew_step(cmd, m->id_out_prev, m->id_slew_A);
    m->id_out_prev = out;
    return out;
}