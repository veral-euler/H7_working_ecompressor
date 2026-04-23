#include "Safety_and_Cutoffs.h"

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim17;

void Hardware_OV(float V_dc)
{
	if (V_dc >= 100.0f) {
		FOC_F_T.HW_OV_Fault = 1;
		L_g.error_code_2 |= (FOC_F_T.HW_OV_Fault << 2);
		FOC_F_T.Fault_Occured = 1;
	}
}

void LV_Fault(float Lv_12v)
{
	if (Lv_12v <= (9.5f)) {
		FOC_F_T.Lv_Fault = 1;
		L_g.error_code_2 |= (FOC_F_T.Lv_Fault << 1);
		FOC_F_T.Fault_Occured = 1;
	}
}

void Motor_abnormal_run()
{
	if (curr_thr_state == DRIVE && (TIM2->CR1 & TIM_CR1_DIR)) {
		FOC_F_T.ABn_Fault = 1;
		L_g.error_code_1 |= (FOC_F_T.ABn_Fault << 7);
	} else if (curr_thr_state == REVERSE && !(TIM2->CR1 & TIM_CR1_DIR)) {
		FOC_F_T.ABn_Fault = 1;
		L_g.error_code_1 |= (FOC_F_T.ABn_Fault << 7);
	}
}

void Over_temp_fault(float motor_temp)
{
	if (motor_temp >= MT_OT_LIMIT) {
		FOC_F_T.mt_OT_Fault = 1;
		L_g.error_code |= (FOC_F_T.mt_OT_Fault << 3);
		FOC_F_T.Fault_Occured = 1;
	}
}

void Over_temp_MC_fault(float mc_temp)
{
	if (mc_temp >= MCU_OT_LIMIT) {
		FOC_F_T.mct_OT_Fault |= 1;
		FOC_F_T.Fault_Occured = 1;
	}
}

void Over_speed_fault(float rpm)
{
	if (rpm >= 6500.0) {
		FOC_F_T.OS_Fault = 1;
		L_g.error_code |= (FOC_F_T.OS_Fault << 1);
		FOC_F_T.Fault_Occured = 1;
	}
}

void Speed_limit_warning(float rpm)
{
	if (rpm < 6000.0f) {
		FOC_F_T.OS_Warn = 0;
		L_g.error_code &= ~(1 << 7);
	} else if (rpm >= 6000.0f && rpm < 6500.0f) {
		FOC_F_T.OS_Warn = 1;
		L_g.error_code |= (FOC_F_T.OS_Warn << 7);
	}
}

void Over_Current_Cutoff(float Ia, float Ib, float Ic)
{
	volatile float min_curr = fminf(Ia, fminf(Ib, Ic));
	volatile float max_curr = fmaxf(Ia, fmaxf(Ib, Ic));

	volatile uint32_t curr_time = HAL_GetTick();
	static uint32_t prev_time = 0;
	static uint8_t counter = 0;

	if (curr_time - prev_time >= 10)
	{
		if ((max_curr >= OVER_CURRENT_LIMIT) || (min_curr <= -OVER_CURRENT_LIMIT)) counter++;

		if (counter >= 5)
		{
			FOC_F_T.OC_Fault = 1;
			L_g.error_code_1 |= (FOC_F_T.OC_Fault << 3);
			FOC_F_T.Fault_Occured = 1;
			counter = 0;
		}

		prev_time = curr_time;
	}
}

void Stall_Protection(float Irms, float Speed_RPM)
{
	volatile uint32_t curr_time = HAL_GetTick();
	static uint32_t prev_time = 0;
	static uint8_t counter = 0;

	if (curr_time - prev_time >= 10)
	{
		if (Speed_RPM <= 200.0f && (Irms >= 150.0f || Irms <= -150.0f))
		{
			counter++;
		}

		if (counter >= 260)
		{
			FOC_F_T.Stall_Fault = 1;
			L_g.error_code_1 |= (FOC_F_T.Stall_Fault << 2);
			FOC_F_T.Fault_Occured = 1;
			counter = 0;
		}

		prev_time = curr_time;
	}
}

void DC_OC_Cutoff(float I_dc)
{
	volatile uint32_t curr_time = HAL_GetTick();
	static uint32_t prev_time = 0;
	static uint8_t counter = 0;

	if (curr_time - prev_time >= 10)
	{
		if (I_dc <= -DC_OC_LIMIT) counter++;

		if (counter >= 3)
		{
			FOC_F_T.DC_OC_Fault = 1;
			FOC_F_T.Fault_Occured = 1;
			counter = 0;
		}

		prev_time = curr_time;
	}
}

void DC_Voltage_Cuttoff(float V_dc)
{
	volatile uint32_t curr_time = HAL_GetTick();
	static uint32_t prev_time = 0;
	static uint8_t counter_OV = 0;
	static uint8_t counter_UV = 0;

	if (curr_time - prev_time >= 10)
	{
		if (V_dc >= OV_LIMIT) counter_OV++;
		else if (V_dc <= UV_LIMIT) counter_UV++;

		if (counter_OV >= 3)
		{
			FOC_F_T.DC_OV_Fault = 1;
			L_g.error_code |= (FOC_F_T.DC_OV_Fault << 4);
			FOC_F_T.Fault_Occured = 1;
			counter_OV = 0;
		}
		else if (counter_UV >= 3)
		{
			FOC_F_T.DC_UV_Fault = 1;
			L_g.error_code |= (FOC_F_T.DC_UV_Fault << 5);
			FOC_F_T.Fault_Occured = 1;
			counter_UV = 1;
		}

		prev_time = curr_time;
	}
}

void Over_Current_Idq(float Iq, float Id)
{
	volatile uint32_t curr_time = HAL_GetTick();
	static uint32_t prev_time = 0;
	static uint8_t counter = 0;

	if (curr_time - prev_time >= 10)
	{
		if (Iq >= IQ_OC_LIMIT || Iq <= -IQ_OC_LIMIT || Id >= ID_OC_LIMIT || Id <= -ID_OC_LIMIT) counter++;

		if (counter >= 3)
		{
			FOC_F_T.Idq_OC_Fault = 1;
			L_g.error_code_1 |= (FOC_F_T.Idq_OC_Fault << 4);
			FOC_F_T.Fault_Occured = 1;
			counter = 0;
		}

		prev_time = curr_time;
	}
}

void Fault_Check(void)
{
	Over_Current_Cutoff(FOC_U.PhaseCurrent[0], FOC_U.PhaseCurrent[1], FOC_U.PhaseCurrent[2]);
	DC_Voltage_Cuttoff(FOC_Out.Vbus_Voltage);
	Over_Current_Idq(FOC_Y.Iq, FOC_Y.Id);
	Stall_Protection(L_g.Irms_Filtered, (fabsf(FOC_Out.Filtered_Speed) * 9.55f));
	Hardware_OV(FOC_Out.Vbus_Voltage);
	LV_Fault(FOC_Out.lv_12v_voltage);
	Motor_abnormal_run();
	Speed_limit_warning((fabsf(FOC_Out.Filtered_Speed) * 9.55f));
	Over_speed_fault((fabsf(FOC_Out.Filtered_Speed) * 9.55f));
	Over_temp_fault(L_g.Motor_Temp);
	Over_temp_MC_fault(L_g.Controller_Temp);
	// DC_OC_Cutoff(FOC_Out.pack_current);

	if (FOC_F_T.Fault_Occured)
	{
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12, GPIO_PIN_SET|GPIO_PIN_SET|GPIO_PIN_RESET);

		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_SET);

		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,1250);
		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,1250);
		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,1250);

		HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_3);
		HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_2);
		HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_1);

		HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_3);
		HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
		HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);

		HAL_TIM_Base_DeInit(&htim1);

		HAL_TIM_Base_Stop_IT(&htim17);

		HAL_TIM_Base_DeInit(&htim17);

		FOC_F_T.Drive_off = 1;
	}
}
