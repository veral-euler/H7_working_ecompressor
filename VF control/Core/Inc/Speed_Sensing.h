#ifndef __SPEED_SENSING_H_
#define __SPEED_SENSING_H_

#define T_S 0.0001f
#define DERIVATIVE_CONSTANT (1.0f / 0.0001f) // Derivative constant
#define T_F_ROTOR_SPEED 0.03f // Constant to filter rotor speed
#define SPEED_FILTER_CONSTANT_1 ((2.0f * T_F_ROTOR_SPEED - T_S) / (2.0f * T_F_ROTOR_SPEED + T_S)) // Constant to filter speed
#define SPEED_FILTER_CONSTANT_2 (T_S / (2.0f * T_F_ROTOR_SPEED + T_S)) // Constant to filter speed

void Speed_Filter(float Speed);
void Speed_Sense(float);

#endif
