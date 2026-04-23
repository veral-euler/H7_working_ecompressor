#ifndef __LPF_H_
#define __LPF_H_

typedef struct {
    float filtered_value;
    float alpha;
} LPF;

void LPF_Init(LPF* lpf, float initial_value, float alpha);

float LPF_Update(LPF* lpf, float new_sample);

extern LPF lpf;

#endif
