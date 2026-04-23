/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "NTC_Temp_Reading.h"
#include "Encoder_to_angle0.h"
#include "FOC.h"
#include "fdcan.h"
#include "Throttle_Mapping.h"
#include "Safety_and_Cutoffs.h"
#include "recorder.h"
#include "CAN_Data.h"
#include "LPF.h"
#include "Offset_Scheduling.h"
#include "Speed_Sensing.h"
#include "Regen_Manager.h"
#include "Angle_calibration.h"
#include <string.h>
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
// extern ExtU_FOC_T FOC_U;
// extern ExtY_FOC_T FOC_Y;
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
#define CONFIG_VER_MAJOR 0x02
#define CONFIG_VER_MIN 0x00
#define CONFIG_VER_SUBMIN 0x00
#define FIRMWARE_VER_MAJOR 0x00
#define FIRMWARE_VER_MIN 0x0A
#define FIRMWARE_VER_SUBMIN 0x06

#define PI 3.1415926f
#define POLEPAIRS 4.0f
#define RATED_CURRENT 800.0f
#define RPM_TO_KMPH 0.01f
#define PID_TUNING 0 // Set to 1 to enable PID tuning via CAN messages
#define SPEED_REF_ON_CAN 0 // Set to 1 to receive speed reference from CAN messages
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
extern uint8_t is_thr_valid;
extern uint8_t align_flag;
extern uint8_t thr_disabled;
extern uint8_t curr_thr_state;
extern uint8_t prev_thr_state;
extern uint8_t motor_derate_flag;
extern uint8_t ctlr_derate_flag;
extern float align_iq_ref;
extern float align_id_ref;
/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void Initial_Fault_Check(void);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/

/* USER CODE BEGIN Private defines */
typedef struct FOC {
	float Va;
	float Vb;
	float Vc;
	float Va_afterInv;
	float Vb_afterInv;
	float Vc_afterInv;
	float V_Alpha;
	float V_Beta;
	float V_AlphaAfterInv;
	float V_BetaAfterInv;
	float Vq;
	float Vd;
	float Va_SVM;
	float Vb_SVM;
	float Vc_SVM;
	float Va_SVM_FOC;
	float Vb_SVM_FOC;
	float Vc_SVM_FOC;
	float Ia;
	float Ib;
	float Ic;
	float I_Alpha;
	float I_Beta;
	float I_AlphaAfterInv;
	float I_BetaAfterInv;
	float Ia_afterInv;
	float Ib_afterInv;
	float Ic_afterInv;
	float Iq;
	float Id;
	float Iq_Ref;
	float Id_Ref;
	float Speed;
	float Speed_Ref;
	float Filtered_Speed;
	float I_rms;
	float Avg_I_rms;
	float Irms_afterFilter;
	float MtrElcPos;
	float Encoder_Position;
	float Encoder_Mech_Position;
	float Initial_Position;
	float Kp_q;
	float Ki_q;
	float Kp_d;
	float Ki_d;
	float Kp_speed;
	float Ki_speed;
	float PID_Q_Vq;
	float PID_D_Vd;
	float PID_Speed_IqRef;
	float duty;
	float freq;
	float Motor_Temp;
	float Phase_U_Temp;
	float Phase_V_Temp;
	float Phase_W_Temp;
	float Vbus_Voltage;
	float pack_current;
	float pack_voltage;
	float pack_SOC;
	float lv_12v_voltage;
	float can_DCLO;
	float can_DCLI;
	float speed_max_lim;
	uint8_t First_Boot;
	uint8_t Z_Pulse;
	uint8_t Z_Count;
	uint8_t Z_Count_prev;
	uint8_t forward_pin;
	uint8_t reverse_pin;
	uint32_t Time_Elapsed;
	uint32_t Initial_Encoder_Count;
	uint32_t capture_value_0;
	uint32_t capture_value_1;
	uint16_t Encoder_Count;
	uint16_t pwm_a;
	uint16_t pwm_b;
	uint16_t pwm_c;
	uint8_t regen_flag;
} FOC;

typedef struct Log_Out
{
	float Va;
	float Vb;
	float Vc;
	float Vd;
	float Vq;
	float Id;
	float Iq;
	float Id_ref;
	float Iq_ref;
	float V_rms;
	float I_rms;
	float I_dc;
	float Ref_Speed;
	float Elec_Angle;
	float Mech_Angle;
	float Mech_Speed;
	float Motor_Temp;
	float Controller_Temp;
	uint8_t DMA_int_flag;
	uint8_t error_code;
	uint8_t error_code_1;
	uint8_t error_code_2;
	uint8_t historic_error;
	float Irms_Filtered;
	float Vrms_Filtered;
	float Throttle_Voltage;
	float Throttle_Percentage;
	float Speed_In_KMPH;
	float Speed_In_RPM;
	float Is;
	float lv_12v_avg;
	float model_angle_elec_ref;
	uint8_t MTPAFWC;
	float Negative_speed;
	uint8_t V_limit_flag;
	// uint8_t 
} Log_Out;

typedef struct Current_Sensor_Offset {
	float Offset_a;
	float Offset_b;
} Current_Sensor_Offset;

typedef enum Throttle_States
{
	DRIVE = 0,
	REVERSE = 1,
	NEUTRAL = 2
} Throttle_States;

typedef enum ADC_1_Channels
{
	LV_12_V = 3,
	THROTTLE = 2,
	PHASE_V = 1,
	PHASE_W = 0,
} ADC_1_Channels;

typedef enum ADC_2_Channels
{
	TEMP_M = 2,
	TEMP_U = 0,
	TEMP_V = 1,
	TEMP_W = 3,
	VBUS_V = 4
} ADC_2_Channels;

extern Log_Out L_g;
extern FOC FOC_Out;
extern LPF Vrms_LPF;
extern LPF Irms_LPF;
extern Current_Sensor_Offset curr_sen_off;
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
