#ifndef __PID_CONTROL_H_
#define __PID_CONTROL_H_

#define LIM_MAX 61.0f
#define LIM_MIN -61.0f
#define LIM_MAX_SPEED_PID 150.0f
#define LIM_MIN_SPEED_PID 0.0f

void Q_AXIS_PID(float Iq_Ref, float Iq);
void D_AXIS_PID(float Id_Ref, float Id);

void SPEED_PID(float Speed_Ref, float Speed);

#endif
