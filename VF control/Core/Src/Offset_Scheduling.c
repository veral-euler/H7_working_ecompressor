#include "Offset_Scheduling.h"

// ===== Helper for linear interpolation =====
static inline float interpolatef(float x, float in_min, float in_max, float out_min, float out_max)
{
    if (x <= in_min) return out_min;
    if (x >= in_max) return out_max;
    float t = (x - in_min) / (in_max - in_min);
    return out_min + t * (out_max - out_min);
}

/**
 * @brief Adaptive offset scheduler based on Irms and Vrms
 * @param Irms  measured RMS current (A)
 * @param Vrms  measured RMS phase voltage (V)
 * @return      adaptive offset angle (rad)
 */
float OffsetScheduler_Update(float Irms, float Vrms)
{
    // 1. Compute "torque load factor" based on Irms
    float load_factor = interpolatef(Irms, IRMS_MIN, IRMS_MAX, 0.0f, 1.0f);

    // 2. Compute "speed factor" based on Vrms
    float speed_factor = interpolatef(Vrms, VRMS_MIN, VRMS_MAX, 1.0f, 0.0f);

    // 3. Blend both (you can weight differently if needed)
    float combined = 0.4f * load_factor + 0.6f * speed_factor;

    // 4. Interpolate between offsets
    float offset = interpolatef(combined, 0.0f, 1.0f, OFFSET_LOW_TORQUE, OFFSET_HIGH_TORQUE);

    if (curr_thr_state == REVERSE) {
        offset = REVERSE_OFFSET; // Fixed offset in reverse
    }

    return offset;
}