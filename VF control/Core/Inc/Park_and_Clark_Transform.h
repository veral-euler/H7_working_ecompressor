#ifndef __PARK_AND_CLARK_TRANSFORM_H
#define __PARK_AND_CLARK_TRANSFORM_H

#include <math.h>

void PARK_TRANSFORM(float I_Alpha, float I_Beta, float ElecPos);
void CLARK_TRANSFORM(float Ia, float Ib, float Ic);

void INVERSE_PARK(float Vd, float Vq, float ElecPos);
void INVERSE_CLARK(float V_Alpha, float V_Beta);

#endif
