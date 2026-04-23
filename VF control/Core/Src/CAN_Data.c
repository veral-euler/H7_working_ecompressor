#include "CAN_Data.h"
#include "fdcan.h"
#include "main.h"

extern uint8_t can_rx_Flag;

/* Vehicle CAN Matrix */
/*****************************************************************************/
void Send_on_CAN_705(float Mtr_Temp, float Cntrl_Temp)
{
	uint8_t data[8] = {0};

	float mt_t = Mtr_Temp + 50.0f;
	float ct_t = Cntrl_Temp + 50.0f;
	float can_iq = FOC_Y.Iq + 512.0f;
	float can_id = FOC_Y.Id + 512.0f;
	float offset_angle = Angle_calibration_Y.OffsetAngle * 100.0f;

	data[0] = (uint8_t)((uint16_t)mt_t);
	data[1] = (uint8_t)((uint16_t)ct_t);

	data[2] = (uint8_t)((uint16_t)can_iq & 0x00FF);
	data[3] = (uint8_t)(((uint16_t)can_iq & 0xFF00) >> 8);

	data[4] = (uint8_t)((uint16_t)can_id & 0x00FF);
	data[5] = (uint8_t)(((uint16_t)can_id & 0xFF00) >> 8);

	data[6] = (uint8_t)((uint16_t)offset_angle & 0x00FF);
	data[7] = (uint8_t)(((uint16_t)offset_angle & 0xFF00) >> 8);

	_fdcan_transmit_on_can(0x705, 0, data, 0x08);
}

void Send_on_CAN_706(uint8_t forward, uint8_t reverse, float bus_voltage, float Speed_KMPH, uint8_t error_c)
{
	uint8_t data[8] = {0};

	if (reverse == 1 && forward == 0)
		data[0] = 0x04;
	else if (forward == 1 && reverse == 1)
		data[0] = 0x01;

	if (ModeManager_GetMode() == MODE_SPORTS && curr_thr_state == DRIVE) {
		data[0] = 0x08;
	} else if (ModeManager_GetMode() == MODE_ECO && curr_thr_state == DRIVE) {
		data[0] = 0x02;
	}

	if (FOC_Out.regen_flag) {
		data[1] |= (1 << 2);
	} else {
		data[1] &= ~(1 << 2);
	}

	if (bus_voltage <= 40)
		data[1] |= (1 << 3);
	else
		data[1] &= ~(1 << 3);
	
	data[1] |= (1 << 5);

	if (FOC_F_T.mct_OT_Fault) {
		data[1] |= (1 << 4);
	}

	data[3] = error_c;

	Speed_KMPH *= 10.0f;
	data[4] = (uint8_t)((uint16_t)(Speed_KMPH) & 0x00FF);
	data[5] = (uint8_t)(((uint16_t)(Speed_KMPH) & 0xFF00) >> 8);

	_fdcan_transmit_on_can(0x706, 0, data, 0x08);
}

void Send_on_CAN_708(uint8_t error_c_1, uint8_t error_c_2)
{
	uint8_t data[8] = {0};

	float theta_elec = L_g.model_angle_elec_ref * 100.0f;
	float theta_elec_sensor = Angle_calibration_U.Theta_e_sensor * 100.0f;
	uint16_t encoder_count = (uint16_t)(TIM2->CNT);

	data[0] = error_c_1;
	data[1] = error_c_2;

	if (ctlr_derate_flag) {
		data[1] |= (1 << 3);
	} else {
		data[1] &= ~(1 << 3);
	}

	if (motor_derate_flag) {
		data[1] |= (1 << 4);
	} else {
		data[1] &= ~(1 << 4);
	}

	data[2] = (uint8_t)((uint16_t)theta_elec & 0x00FF);
	data[3] = (uint8_t)(((uint16_t)theta_elec & 0xFF00) >> 8);

	data[4] = (uint8_t)(encoder_count & 0x00FF);
	data[5] = (uint8_t)((encoder_count & 0xFF00) >> 8);

	data[6] = (uint8_t)((uint16_t)theta_elec_sensor & 0x00FF);
	data[7] = (uint8_t)(((uint16_t)theta_elec_sensor & 0xFF00) >> 8);

	_fdcan_transmit_on_can(0x708, 0, data, 0x08);
}

void Send_on_CAN_709(uint8_t historic_errors)
{
	uint8_t data[8] = {0};

	float vdc_sat_val = Angle_calibration_Y.Vdc_sat * 100.0f;

	data[6] = (uint8_t)((uint16_t)vdc_sat_val & 0x00FF);
	data[7] = (uint8_t)(((uint16_t)vdc_sat_val & 0xFF00) >> 8);

	_fdcan_transmit_on_can(0x709, 0, data, 0x08);
}

void Send_on_CAN_710(float throttle_percent, float throttle_voltage) {
	uint8_t data[8] = {0};

	data[0] = (uint8_t)((uint16_t)throttle_percent);
	data[3] = (uint8_t)((uint16_t)(throttle_voltage * 10.0f));
	data[4] = 0x00;

	_fdcan_transmit_on_can(0x710, 0, data, 0x08);
}

void Send_on_CAN_715(float RPM, float bus_voltage, float lv_12v_voltage)
{
	uint8_t data[8] = {0};

	data[3] = (uint8_t)((uint16_t)FOC_Out.pack_voltage);
	data[4] = (uint8_t)((uint16_t)bus_voltage);
	data[5] = (uint8_t)((uint16_t)lv_12v_voltage);
	data[6] = (uint8_t)((uint16_t)RPM & 0x00FF);
	data[7] = (uint8_t)(((uint16_t)RPM & 0xFF00) >> 8);

	_fdcan_transmit_on_can(0x715, 0, data, 0x08);
}

void Send_on_CAN_716(float vrms, float irms)
{
	uint8_t data[8] = {0};

	float pack_dc = FOC_Out.pack_current + 500.0f;

	data[2] = (uint8_t)((uint16_t)pack_dc & 0x00FF);
	data[3] = (uint8_t)(((uint16_t)pack_dc & 0xFF00) >> 8);
	data[4] = (uint8_t)((uint16_t)((vrms/FOC_Out.Vbus_Voltage) * 100.0f));
	data[5] = (uint8_t)((uint16_t)vrms);
	data[6] = (uint8_t)((uint16_t)irms & 0x00FF);
	data[7] = (uint8_t)(((uint16_t)irms & 0xFF00) >> 8);

	_fdcan_transmit_on_can(0x716, 0, data, 0x08);
}

void Send_on_CAN_717(float rpm)
{
	uint8_t data[8] = {0};

	volatile float Hz = (rpm * POLEPAIRS * 2.0f) / 120.0f;

	data[0] = (uint8_t)((uint16_t)Hz & 0x00FF);
	data[1] = (uint8_t)(((uint16_t)Hz & 0xFF00) >> 8);

	_fdcan_transmit_on_can(0x717, 0, data, 0x08);
}

void Send_on_CAN_724(float torque_ref)
{
	uint8_t data[8] = {0};

	float can_vq = FOC_Y.Vq + 62.0f;
	float can_vd = FOC_Y.Vd + 62.0f;
	torque_ref *= 10.0f;

	data[0] = (uint8_t)((uint16_t)torque_ref & 0x00FF);
	data[1] = (uint8_t)(((uint16_t)torque_ref & 0xFF00) >> 8);

	data[2] = (uint8_t)((uint16_t)can_vq & 0x00FF);
	data[3] = (uint8_t)(((uint16_t)can_vq & 0xFF00) >> 8);

	data[4] = (uint8_t)((uint16_t)can_vd & 0x00FF);
	data[5] = (uint8_t)(((uint16_t)can_vd & 0xFF00) >> 8);

	_fdcan_transmit_on_can(0x724, 0, data, 0x08);
}

void Send_on_CAN_726()
{
	uint8_t data[8] = {0};

	data[0] = (uint8_t)(CONFIG_VER_MAJOR);
	data[1] = (uint8_t)(CONFIG_VER_MIN);
	data[2] = (uint8_t)(CONFIG_VER_SUBMIN);
	data[3] = 0xE2;
	data[4] = 0x58;
	data[5] = (uint8_t)(FIRMWARE_VER_MAJOR);
	data[6] = (uint8_t)(FIRMWARE_VER_MIN);
	data[7] = (uint8_t)(FIRMWARE_VER_SUBMIN);

	_fdcan_transmit_on_can(0x726, 0, data, 0x08);
}
/*****************************************************************************/

/* Debug CAN IDs */
/*****************************************************************************/
void Send_on_CAN_300()
{
	uint8_t data[8] = {0};

	float Id_ref_scaled = FOC_Y.Id_ref + 350.0f;
	float Id_Scaled = FOC_Y.Id + 350.0f;
	float Iq_ref_scaled = FOC_Y.Iq_ref + 350.0f;
	float Iq_Scaled = FOC_Y.Iq + 350.0f;

	data[0] = (uint8_t)((uint16_t)Iq_ref_scaled & 0x00FF);
	data[1] = (uint8_t)(((uint16_t)Iq_ref_scaled & 0xFF00) >> 8);

	data[2] = (uint8_t)((uint16_t)Iq_Scaled & 0x00FF);
	data[3] = (uint8_t)(((uint16_t)Iq_Scaled & 0xFF00) >> 8);

	data[4] = (uint8_t)((uint16_t)Id_ref_scaled & 0x00FF);
	data[5] = (uint8_t)(((uint16_t)Id_ref_scaled & 0xFF00) >> 8);

	data[6] = (uint8_t)((uint16_t)Id_Scaled & 0x00FF);
	data[7] = (uint8_t)(((uint16_t)Id_Scaled & 0xFF00) >> 8);

	_fdcan_transmit_on_can(0x300, 0, data, 0x08);
}

void Send_on_CAN_301()
{
	uint8_t data[8] = {0};

	float Vq_scaled = FOC_Y.Vq + 62.0f;
	float Vd_scaled = FOC_Y.Vd + 62.0f;

	data[0] = (uint8_t)((uint16_t)Vq_scaled & 0x00FF);
	data[1] = (uint8_t)(((uint16_t)Vq_scaled & 0xFF00) >> 8);

	data[2] = (uint8_t)((uint16_t)Vd_scaled & 0x00FF);
	data[3] = (uint8_t)(((uint16_t)Vd_scaled & 0xFF00) >> 8);

	data[4] = (uint8_t)((uint16_t)L_g.Irms_Filtered & 0x00FF);
	data[5] = (uint8_t)(((uint16_t)L_g.Irms_Filtered & 0xFF00) >> 8);

	data[6] = (uint8_t)((uint16_t)L_g.Vrms_Filtered & 0x00FF);
	data[7] = (uint8_t)(((uint16_t)L_g.Vrms_Filtered & 0xFF00) >> 8);

	_fdcan_transmit_on_can(0x301, 0, data, 0x08);
}

void Send_on_CAN_302()
{
	uint8_t data[8] = {0};

	float throttle_volt = L_g.Throttle_Voltage * 10.0f;
	float cont_temp = L_g.Controller_Temp + 50.0f;
	float motor_temp = L_g.Motor_Temp + 50.0f;
	float Speed_CAN_radps = rtEncoder_to_angle0_Y.Speed_mech_filtered * 0.10472f; //Convert to rad/s

	data[0] = (uint8_t)((uint16_t)throttle_volt);
	data[1] = (uint8_t)((uint16_t)L_g.Throttle_Percentage);

	data[2] = (uint8_t)((uint16_t)(fabsf(FOC_Out.Filtered_Speed) * 9.55f * RPM_TO_KMPH) & 0x00FF);
	data[3] = (uint8_t)(((uint16_t)(fabsf(FOC_Out.Filtered_Speed) * 9.55f * RPM_TO_KMPH) & 0xFF00) >> 8);

	data[4] = (uint8_t)((uint16_t)Speed_CAN_radps & 0x00FF); //not in use
	data[5] = (uint8_t)(((uint16_t)Speed_CAN_radps & 0xFF00) >> 8);

	data[6] = (uint8_t)((uint16_t)cont_temp);
	data[7] = (uint8_t)((uint16_t)motor_temp);

	_fdcan_transmit_on_can(0x302, 0, data, 0x08);
}

void Send_on_CAN_303(){
	uint8_t data[8] = {0};

	data[0] = (uint8_t)(L_g.error_code);
	data[1] = (uint8_t)((uint16_t)L_g.Is & 0x00FF); //add
	data[2] = (uint8_t)(((uint16_t)L_g.Is & 0xFF00) >> 8);
	data[3] = (uint8_t)((uint16_t)FOC_Out.Vbus_Voltage & 0x00FF);
	data[4] = (uint8_t)(can_rx_Flag);
	data[5] = (uint8_t)((uint16_t)FOC_U.Ref_Speed_mech_rpm & 0x00FF);
	data[6] = (uint8_t)(((uint16_t)FOC_U.Ref_Speed_mech_rpm & 0xFF00) >> 8);

	_fdcan_transmit_on_can(0x303, 0, data, 0x08);
}

void Send_on_CAN_304(){
	uint8_t data[8] = {0};

	uint16_t Torque_Gen_CAN = (uint16_t)(FOC_Y.Torque_gen * 100.0f);
	uint16_t Iq_Gen = (uint16_t)(FOC_Y.Iq_gen * 100.0f);
	float Integrator_Torque_PID_CAN = (FOC_Y.Integrator_Torque_PID + 100.0f) * 100.0f;

	data[0] = (uint8_t)((uint16_t)rtEncoder_to_angle0_Y.Speed_mech_filtered & 0x00FF);
	data[1] = (uint8_t)(((uint16_t)rtEncoder_to_angle0_Y.Speed_mech_filtered & 0xFF00) >> 8);

	data[2] = (uint8_t)((uint16_t)Torque_Gen_CAN & 0x00FF);
	data[3] = (uint8_t)(((uint16_t)Torque_Gen_CAN & 0xFF00) >> 8);

	data[4] = (uint8_t)(((uint16_t)Iq_Gen & 0x00FF));
	data[5] = (uint8_t)(((uint16_t)Iq_Gen & 0xFF00) >> 8);

	data[6] = (uint8_t)((uint16_t)Integrator_Torque_PID_CAN & 0x00FF);
	data[7] = (uint8_t)(((uint16_t)Integrator_Torque_PID_CAN & 0xFF00) >> 8);

	_fdcan_transmit_on_can(0x304, 0, data, 0x08);
}

void Send_on_CAN_305(){
	uint8_t data[8] = {0};

	uint16_t Vd_FF_CAN = (uint16_t)(FOC_Y.Vd_FF + 62.0f);
	uint16_t Vq_FF_CAN = (uint16_t)(FOC_Y.Vq_FF + 62.0f);

	uint16_t Id_sat_CAN = (uint16_t)(FOC_Y.Id_sat + 62.0f);
	uint16_t Iq_sat_CAN = (uint16_t)(FOC_Y.Iq_sat + 62.0f);

	data[0] = (uint8_t)((uint16_t)Vd_FF_CAN & 0x00FF);
	data[1] = (uint8_t)(((uint16_t)Vd_FF_CAN & 0xFF00) >> 8);

	data[2] = (uint8_t)((uint16_t)Vq_FF_CAN & 0x00FF);
	data[3] = (uint8_t)(((uint16_t)Vq_FF_CAN & 0xFF00) >> 8);

	data[4] = (uint8_t)((uint16_t)Id_sat_CAN & 0x00FF);
	data[5] = (uint8_t)(((uint16_t)Id_sat_CAN & 0xFF00) >> 8);

	data[6] = (uint8_t)((uint16_t)Iq_sat_CAN & 0x00FF);
	data[7] = (uint8_t)(((uint16_t)Iq_sat_CAN & 0xFF00) >> 8);

	_fdcan_transmit_on_can(0x305, 0, data, 0x08);
}

void Send_on_CAN_306(){
	uint8_t data[8] = {0};

	uint16_t Vd_PID_CAN = (uint16_t)(FOC_Y.Vd_PID + 62.0f);
	uint16_t Vq_PID_CAN = (uint16_t)(FOC_Y.Vq_PID + 62.0f);

	// float Vdq_Scale_CAN = FOC_Y.Vdq_Scale * 100.0f; // Scale to fit in uint16_t 
	float V_total_CAN = FOC_Y.V_total_calc + 62.0f;
//	float Integrator_Flux_PID_CAN = (FOC_Y.Integrator_Flux_PID + 100.0f) * 100.0f;

	//Data[0] and Data[1] are used for Id_Ramp_Delta
	data[2] = (uint8_t)((uint16_t)Vd_PID_CAN);
	data[3] = (uint8_t)((uint16_t)Vq_PID_CAN);

	// data[4] = (uint8_t)((uint16_t)Vdq_Scale_CAN & 0x00FF);
	// data[5] = (uint8_t)(((uint16_t)Vdq_Scale_CAN & 0xFF00) >> 8);

	data[4] = (uint8_t)(V_total_CAN);

	//data[5] = (uint8_t)((uint16_t)Integrator_Flux_PID_CAN & 0x00FF);
	//data[6] = (uint8_t)(((uint16_t)Integrator_Flux_PID_CAN & 0xFF00) >> 8);

	data[7] = (uint8_t)(L_g.V_limit_flag); // V_Limit_Flag to indicate if voltage limit is applied is greater than
	// data[7] = (uint8_t)(FOC_Y.back_EMF_constant);

	_fdcan_transmit_on_can(0x306, 0, data, 0x08);
}

void Send_on_CAN_307(){
	uint8_t data[8] = {0};
	float Id_error_CAN = FOC_Y.Id_error + 350.0f;
	float Iq_error_CAN = FOC_Y.Iq_error + 350.0f;
	float Speed_error_CAN = fabsf(FOC_Y.Speed_error) * 9.549297f; // Radps to RPM conversion
//	float Integrator_Speed_PID_CAN = (float)(FOC_Y.Integrator_Speed_PID + 400.0f) * 10.0;
	// uint16_t Back_EMF_CAN_0 = (FOC_Y.back_EMF[0] + 62.0f);
	// uint16_t Back_EMF_CAN_1 = (FOC_Y.back_EMF[1] + 62.0f);

	data[0] = (uint8_t)((uint16_t)Id_error_CAN & 0x00FF);
	data[1] = (uint8_t)(((uint16_t)Id_error_CAN & 0xFF00) >> 8);

	data[2] = (uint8_t)((uint16_t)Iq_error_CAN & 0x00FF);
	data[3] = (uint8_t)(((uint16_t)Iq_error_CAN & 0xFF00) >> 8);
	
	data[4] = (uint8_t)((uint16_t)Speed_error_CAN & 0x00FF);
	data[5] = (uint8_t)(((uint16_t)Speed_error_CAN & 0xFF00) >> 8);

//	data[6] = (uint8_t)((uint16_t)Integrator_Speed_PID_CAN & 0x00FF);
//	data[7] = (uint8_t)(((uint16_t)Integrator_Speed_PID_CAN & 0xFF00) >> 8);
	
	// data[6] = (uint8_t)(Back_EMF_CAN_0);
	// data[7] = (uint8_t)(Back_EMF_CAN_1);
	
	_fdcan_transmit_on_can(0x307, 0, data, 0x08);
}

void Send_on_CAN_308(){
	uint8_t data[8] = {0};

	data[0] = (uint8_t)(FOC_U.MTPA_PID.Speed_PID_MTPA.Kp_speed_PID_MTPA * 10.0f);
	data[1] = (uint8_t)(FOC_U.MTPA_PID.Speed_PID_MTPA.Ki_speed_PID_MTPA * 10.0f);
	data[2] = (uint8_t)(FOC_U.MTPA_PID.Speed_PID_MTPA.Kd_speed_PID_MTPA * 10000.0f);
	data[3] = (uint8_t)(FOC_U.MTPA_PID.Speed_PID_MTPA.Filter_speed_PID_MTPA * 10.0f);

	data[4] = (uint8_t)(FOC_U.MTPA_PID.Flux_PID_MTPA.Kp_flux_PID_MTPA * 10.0f);
	data[5] = (uint8_t)(FOC_U.MTPA_PID.Flux_PID_MTPA.Ki_flux_PID_MTPA * 10.0f);
	data[6] = (uint8_t)(FOC_U.MTPA_PID.Flux_PID_MTPA.Kd_flux_PID_MTPA * 10000.0f);
	data[7] = (uint8_t)(FOC_U.MTPA_PID.Flux_PID_MTPA.Filter_flux_PID_MTPA * 10.0f);

	_fdcan_transmit_on_can(0x308, 0, data, 0x08);
}

void Send_on_CAN_309(){
	uint8_t data[4] = {0};

	data[0] = (uint8_t)(FOC_U.MTPA_PID.Torque_PID_MTPA.Kp_torque_PID_MTPA * 10.0f);
	data[1] = (uint8_t)(FOC_U.MTPA_PID.Torque_PID_MTPA.Ki_torque_PID_MTPA * 10.0f);
	data[2] = (uint8_t)(FOC_U.MTPA_PID.Torque_PID_MTPA.Kd_torque_PID_MTPA * 10000.0f);
	data[3] = (uint8_t)(FOC_U.MTPA_PID.Torque_PID_MTPA.Filter_torque_PID_MTPA * 10.0f);
	
	_fdcan_transmit_on_can(0x309, 0, data, 0x04);
}
/*****************************************************************************/

void Send_all_data_CAN(FOC foc_data, Log_Out log_data)
{
	Send_on_CAN_705(foc_data.Motor_Temp, foc_data.Phase_U_Temp);
	Send_on_CAN_706(foc_data.forward_pin, foc_data.reverse_pin, foc_data.Vbus_Voltage, (fabsf(foc_data.Filtered_Speed) * RPM_TO_KMPH * 9.55f), log_data.error_code);
	Send_on_CAN_708(log_data.error_code_1, log_data.error_code_2);
	Send_on_CAN_709(log_data.historic_error);
	Send_on_CAN_710(log_data.Throttle_Percentage, log_data.Throttle_Voltage);
	Send_on_CAN_715(fabsf(foc_data.Filtered_Speed) * 9.55f, foc_data.Vbus_Voltage, log_data.lv_12v_avg);
	Send_on_CAN_716(log_data.Vrms_Filtered, log_data.Irms_Filtered);
	Send_on_CAN_717(fabsf(foc_data.Filtered_Speed) * 9.55f);
	Send_on_CAN_724(FOC_Y.Torque_gen);
	Send_on_CAN_726();
}

void Send_on_CAN_Debug()
{
	Send_on_CAN_300();
	Send_on_CAN_301();
	Send_on_CAN_302();
	Send_on_CAN_303();
	Send_on_CAN_304();
	Send_on_CAN_305();
	Send_on_CAN_306();
	Send_on_CAN_307();
}
