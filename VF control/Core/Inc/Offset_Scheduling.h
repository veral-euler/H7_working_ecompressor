#ifndef OFFSET_SCHEDULING_H
#define OFFSET_SCHEDULING_H

#include "main.h"
/*
    offset   low   high
    PT10     1.28  1.38
    R1       1.28  1.33
    P4       1.28  1.33
    NP01     1.28  1.38
    NP02     1.28  1.30
    please change accordingly
*/

// ===== Configurable thresholds =====
#define OFFSET_LOW_TORQUE   1.28f   // rad (for high speed / low current)
#define OFFSET_HIGH_TORQUE  1.33f   // rad (for low speed / high current)
#define REVERSE_OFFSET      1.38f   // rad (fixed offset in reverse)

#define IRMS_MIN   150.0f    // A (below this, consider "low load")
#define IRMS_MAX   200.0f   // A (above this, consider "high load")
#define VRMS_MIN   25.0f    // V (low bus utilization, low speed)
#define VRMS_MAX   30.0f    // V (high bus utilization, high speed)

float OffsetScheduler_Update(float Irms, float Vrms);

#endif