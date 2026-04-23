#include "PID_Control.h"
#include "main.h"

void Q_AXIS_PID(float Iq_Ref, float Iq)
{
	float error_iq = 0.0f;
	float proportional = 0.0f;
	float integral = 0.0f;
	float Output = 0.0f;
	static float prev_error_iq = 0.0f;
	static float prev_integral = 0.0f;

	error_iq = Iq_Ref - Iq;

	proportional = FOC_Out.Kp_q * error_iq;

	integral = prev_integral + 0.5f * 0.0001f * FOC_Out.Ki_q * (error_iq + prev_error_iq);

	if (integral >= 61.0f) integral = 61.0f;
	else if (integral <= -61.0f) integral = -61.0f;

	Output = proportional + integral;

	prev_error_iq = error_iq;

	prev_integral = integral;

	if (Output >= 61.0f) Output = 61.0f;
	else if (Output <= -61.0f) Output = -61.0f;

	FOC_Out.PID_Q_Vq = Output;
}

void D_AXIS_PID(float Id_Ref, float Id)
{
	float error_id = 0.0f;
	float proportional = 0.0f;
	float integral = 0.0f;
	float Output = 0.0f;
	static float prev_error_id = 0.0f;
	static float prev_integral = 0.0f;

	error_id = Id_Ref - Id;

	proportional = FOC_Out.Kp_d * error_id;

	integral = prev_integral + 0.5f * 0.0001f * FOC_Out.Ki_d * (error_id + prev_error_id);

	if (integral >= 61.0f) integral = 61.0f;
	else if (integral <= -61.0f) integral = -61.0f;

	Output = proportional + integral;

	prev_error_id = error_id;

	prev_integral = integral;

	if (Output >= 61.0f) Output = 61.0f;
	else if (Output <= -61.0f) Output = -61.0f;

	FOC_Out.PID_D_Vd = Output;
}

void SPEED_PID(float Speed_Ref, float Speed)
{
	float error_Speed = 0.0f;
	float proportional = 0.0f;
	float integral = 0.0f;
	float Output = 0.0f;
	static float prev_error_Speed = 0.0f;
	static float prev_integral = 0.0f;

	error_Speed = Speed_Ref - Speed;

	proportional = FOC_Out.Kp_speed * error_Speed;

	integral = prev_integral + 0.5f * 0.0001f * FOC_Out.Ki_speed * (error_Speed + prev_error_Speed);

	if (integral >= 200.0f) integral = 200.0f;
	else if (integral <= 0.0f) integral = 0.0f;

	Output = proportional + integral;

	prev_error_Speed = error_Speed;

	prev_integral = integral;

	if (Output >= 200.0f) Output = 200.0f;
	else if (Output <= 0.0f) Output = 0.0f;

	FOC_Out.PID_Speed_IqRef = Output;
}
