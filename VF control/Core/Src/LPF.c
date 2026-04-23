#include "LPF.h"

void LPF_Init(LPF* lpf, float initial_value, float alpha) {
    lpf->filtered_value = initial_value;
    lpf->alpha = alpha;
}

float LPF_Update(LPF* lpf, float new_sample) {
    lpf->filtered_value = lpf->alpha * new_sample +
                          (1.0f - lpf->alpha) * lpf->filtered_value;
    return lpf->filtered_value;
}
