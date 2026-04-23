#include "Park_and_Clark_Transform.h"
#include "main.h"

void PARK_TRANSFORM(float I_Alpha, float I_Beta, float ElecPos)
{
	FOC_Out.Iq = -I_Alpha * sinf(ElecPos) + I_Beta * cosf(ElecPos);
	FOC_Out.Id = I_Alpha * cosf(ElecPos) + I_Beta * sinf(ElecPos);
}

void CLARK_TRANSFORM(float Ia, float Ib, float Ic)
{
	FOC_Out.I_Alpha = Ia;
	FOC_Out.I_Beta = (Ia + 2.0f * Ib) * 0.577350f;
}

void INVERSE_PARK(float Vd, float Vq, float ElecPos)
{
	FOC_Out.V_AlphaAfterInv = Vd * cosf(ElecPos) - Vq * sinf(ElecPos);
	FOC_Out.V_BetaAfterInv = Vd * sinf(ElecPos) + Vq * cosf(ElecPos);
}

void INVERSE_CLARK(float V_Alpha, float V_Beta)
{
	FOC_Out.Va_afterInv = V_Alpha;
	FOC_Out.Vb_afterInv = (-V_Alpha + 1.732f * V_Beta) * 0.5f;
	FOC_Out.Vc_afterInv = (-V_Alpha - 1.732f * V_Beta) * 0.5f;
}
