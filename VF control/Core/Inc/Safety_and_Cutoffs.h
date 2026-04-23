#ifndef __SAFETY_AND_CUTOFFS_H_
#define __SAFETY_AND_CUTOFFS_H_

#include "main.h"

#define OVER_CURRENT_LIMIT 512.0f
#define DC_OC_LIMIT 260.0f
#define OV_LIMIT 72.0f
#define UV_LIMIT 48.0f
#define MCU_OT_LIMIT 85.0f
#define MT_OT_LIMIT 115.0f
#define IQ_OC_LIMIT 512.0f //Peak Value
#define ID_OC_LIMIT 512.0f

void Hardware_OV(float V_dc);
void LV_Fault(float lv_12v);
void Motor_abnormal_run(void);
void Speed_limit_warning(float rpm);
void Over_speed_fault(float rpm);
void Over_temp_fault(float motor_temp);
void Over_temp_MC_fault(float mc_temp);
void Over_Current_Cutoff(float Ia, float Ib, float Ic);
void DC_Voltage_Cuttoff(float V_dc);
void DC_OC_Cutoff(float I_dc);
void Over_Current_Idq(float Iq, float Id);
void Stall_Protection(float Iq, float Speed_RPM);
void Fault_Check(void);

#endif
