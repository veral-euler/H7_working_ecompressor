#include "SVM_Implementation.h"
#include "main.h"

void SVM(float Va, float Vb, float Vc)
{
	volatile float max_v = fmaxf(Va, fmaxf(Vb, Vc));
	volatile float min_v = fminf(Va, fminf(Vb, Vc));

	volatile float v_off = (max_v + min_v) / 2.0f;

	FOC_Out.Va_SVM_FOC = Va - v_off;
	FOC_Out.Va_SVM_FOC *= 1.15;

	FOC_Out.Vb_SVM_FOC = Vb - v_off;
	FOC_Out.Vb_SVM_FOC *= 1.15;

	FOC_Out.Vc_SVM_FOC = Vc - v_off;
	FOC_Out.Vc_SVM_FOC *= 1.15;
}
