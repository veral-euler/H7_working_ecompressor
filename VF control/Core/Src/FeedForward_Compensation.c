#include "FeedForward_Compensation.h"
#include "main.h"

void Feed_Forward(float Speed_Elec, float Lambda, float Ld, float Lq, float Iq, float Id)
{
	FOC_Out.PID_Q_Vq += Speed_Elec * (Lambda + Ld * Id);
	FOC_Out.PID_D_Vd -= Speed_Elec * Lq * Iq;
}
