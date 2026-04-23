#ifndef __CAN_DATA_H_
#define __CAN_DATA_H_

#include <stdint.h>

void Send_on_CAN_705(float Mtr_Temp, float Cntrl_Temp);
void Send_on_CAN_706(uint8_t forward, uint8_t reverse, float bus_voltage, float Speed_KMPH, uint8_t error_c);
void Send_on_CAN_708(uint8_t error_c_1, uint8_t error_c_2);
void Send_on_CAN_709(uint8_t historic_errors);
void Send_on_CAN_710(float throttle_percent, float throttle_voltage);
void Send_on_CAN_715(float rpm, float bus_voltage, float lv_12v_voltage);
void Send_on_CAN_716(float vrms, float irms);
void Send_on_CAN_717(float rpm);
void Send_on_CAN_724(float torque_ref);
void Send_on_CAN_726(void);

void Send_on_CAN_300(void);
void Send_on_CAN_301(void);
void Send_on_CAN_302(void);
void Send_on_CAN_303(void);
void Send_on_CAN_304(void);
void Send_on_CAN_305(void);
void Send_on_CAN_306(void);
void Send_on_CAN_307(void);
void Send_on_CAN_308(void);
void Send_on_CAN_309(void);
void Send_on_CAN_Debug(void);
// void Send_all_data_CAN(FOC foc_data, Log_Out log_data);  
#endif
