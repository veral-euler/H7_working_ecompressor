/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
ADC_HandleTypeDef hadc2;
DMA_HandleTypeDef hdma_adc1;
DMA_HandleTypeDef hdma_adc2;

FDCAN_HandleTypeDef hfdcan2;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim5;
TIM_HandleTypeDef htim17;

/* USER CODE BEGIN PV */
FDCAN_TxHeaderTypeDef	TxMsg;
FDCAN_RxHeaderTypeDef RxMessageBuf;

FOC FOC_Out = {0};
Log_Out L_g = {0.0};
LPF Vrms_LPF = {0.0f};
LPF Irms_LPF = {0.0f};
LPF LV_12v = {0.0f};

Current_Sensor_Offset curr_sen_off = {.Offset_a = 0.0f, .Offset_b = 0.0f};
uint16_t analog_buffer_data_adc_1[4] = {0};
uint16_t analog_buffer_data_adc_2[5] = {0};
uint8_t data[8] = {0x04, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01, 0x02};
uint8_t rxMsg[8] = {0};
uint8_t can_rx_Flag = 0;
uint8_t is_thr_valid = 0;
uint8_t align_flag = 1;
uint8_t alignment_done = 0;
uint8_t align_counter = 0;
uint8_t curr_thr_state = 0;
uint8_t prev_thr_state = 0;
uint8_t thr_disabled = 0;
uint8_t start_flag = 0;
uint8_t motor_derate_flag = 0, ctlr_derate_flag = 0;
float align_id_ref = -30.0f;
float align_iq_ref = 0.0f;
float ref_speed_can = 0.0f;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void PeriphCommonClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_TIM17_Init(void);
static void MX_TIM2_Init(void);
static void MX_FDCAN2_Init(void);
static void MX_ADC1_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM5_Init(void);
static void MX_ADC2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* Configure the peripherals common clocks */
  PeriphCommonClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_TIM17_Init();
  MX_TIM2_Init();
  MX_FDCAN2_Init();
  MX_ADC1_Init();
  MX_TIM1_Init();
  MX_TIM5_Init();
  MX_ADC2_Init();
  volatile uint32_t lastTick = 0;
  /* USER CODE BEGIN 2 */
  (void)memset(&FOC_F_T, 0, sizeof(FOC_Flag_T));
  (void)memset(&FOC_U, 0, sizeof(ExtU_FOC_T));
  (void)memset(&FOC_Y, 0, sizeof(ExtY_FOC_T));

  HAL_Delay(100);

  /**************************************************************/
  /* Setting indicator LED, TIM1 channels and OptoCoupler IC */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12, GPIO_PIN_SET);

  if(HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_3) != HAL_OK) Error_Handler();
  if(HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2) != HAL_OK) Error_Handler();
  if(HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1) != HAL_OK) Error_Handler();

  if(HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3) != HAL_OK) Error_Handler();
  if(HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2) != HAL_OK) Error_Handler();
  if(HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1) != HAL_OK) Error_Handler();

  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,1250);
  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,1250);
  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,1250);
  /**************************************************************/

  /**************************************************************/
  /* Start the FDCAN module, send message on ID 0x401 */
  FDCAN_SETUP();

  _fdcan_transmit_on_can(0x401, 0, data, 0x08);
  /**************************************************************/

  /**************************************************************/
  /* Starting ADC DMA */
  HAL_ADC_Start_DMA(&hadc1, (uint32_t *)analog_buffer_data_adc_1, 4);
  HAL_ADC_Start_DMA(&hadc2, (uint32_t *)analog_buffer_data_adc_2, 5);
  /**************************************************************/

  HAL_Delay(100);

  /**************************************************************/
  /* Calculating Current sensors 0 current offset */
  for (uint8_t i = 0; i < 10; i ++)
  {
	  curr_sen_off.Offset_a += (float)analog_buffer_data_adc_1[PHASE_W];
	  curr_sen_off.Offset_b += (float)analog_buffer_data_adc_1[PHASE_V];
  }

  curr_sen_off.Offset_a = (float)(curr_sen_off.Offset_a / 10.0f);
  curr_sen_off.Offset_b = (float)(curr_sen_off.Offset_b / 10.0f);
  /**************************************************************/

  LPF_Init(&Vrms_LPF, 0.0f , 0.0591f); // Initializing Vrms LPF with alpha = 0.0591, how is it calulate? answer: alpha = 1 - exp(-1/(f_sampling * tau)), where tau = 0.01s, f_sampling = 1000Hz (??)
  LPF_Init(&Irms_LPF, 0.0f , 0.0591f); // Initializing Irms LPF 
  LPF_Init(&LV_12v, 0.0f , 0.2f); // Initializing 12V LPF

  HAL_Delay(100);

  /**************************************************************/
  /* Initializing FOC and Encoder Model */
  HAL_TIM_Encoder_Start_IT(&htim2, TIM_CHANNEL_ALL);//Encoder pulses

  /**************************************************************/
  /* Setting the initial angle */
  // if (FOC_Out.First_Boot == 0)
  // {
	//   TIM2->CNT = (uint32_t)(((100.0f-FOC_Out.duty) / 100.0f) * 4096.0f) + 24;
	//   FOC_Out.First_Boot = 1;
  // }
  /**************************************************************/

  // Encoder_to_angle0_initialize();

  // Initial_Fault_Check();

  // FOC_initialize();
  Angle_calibration_initialize();

  // Regen_Init();

  // ModeManager_Init(GPIOD, GPIO_PIN_2, 50, 3, 0);
  /**************************************************************/

  /**************************************************************/
  /* Starting Motor Controller main timer interrupt */
  HAL_Delay(100);
  HAL_TIM_Base_Start_IT(&htim17);//10kHz
  /**************************************************************/
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

    // ModeManager_Tick();

    // if (start_flag && curr_thr_state == DRIVE) {
    //   if (ModeManager_GetMode() == MODE_ECO) {
    //     g_current_mode_params.max_speed_rpm = 4500.0f;
    //     g_current_mode_params.torque_scale = 1.0f;
    //     FOC_Out.speed_max_lim = 4200.0f;
    //     FOC_U.Id_Iq_MTPA_limit.Id_low_limit = -282.0f;
    //     FOC_U.Id_Iq_MTPA_limit.Iq_up_limit = 282.0f;
    //   } else if (ModeManager_GetMode() == MODE_SPORTS) {
    //     g_current_mode_params.max_speed_rpm = 6100.0f;
    //     g_current_mode_params.torque_scale = 1.0f;
    //     FOC_Out.speed_max_lim = 5000.0f;
    //     FOC_U.Id_Iq_MTPA_limit.Id_low_limit = -512.0f;
    //     FOC_U.Id_Iq_MTPA_limit.Iq_up_limit = 512.0f;
    //   }
    // } else if (start_flag && curr_thr_state == REVERSE) {
    //   if (ModeManager_GetMode() == MODE_ECO) {
    //     g_current_mode_params.max_speed_rpm = 1500.0f;
    //     g_current_mode_params.torque_scale = 1.0f;
    //     FOC_Out.speed_max_lim = 1500.0f;
    //     FOC_U.Id_Iq_MTPA_limit.Id_low_limit = -282.0f;
    //     FOC_U.Id_Iq_MTPA_limit.Iq_up_limit = 282.0f;
    //   }
    // }
	/**************************************************************/
    // if (!FOC_F_T.Drive_off) {
    //   /* Reading FNR Status */
    //   FOC_Out.forward_pin = HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_4);
    //   FOC_Out.reverse_pin = HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_3);
    // } else {
    //   FOC_Out.forward_pin = 1;
    //   FOC_Out.reverse_pin = 1;
    // }
	/**************************************************************/

	/**************************************************************/
	/* Getting Ref Speed from throttle */
  #if !THROTTLE_CONTROL
    if(FOC_Out.forward_pin == 1 && FOC_Out.reverse_pin == 0){
      FOC_U.Ref_Speed_mech_rpm = update_speed_ref(ref_speed_can, FOC_U.Ref_Speed_mech_rpm, 50.0f, 100.0f, 0.001f);
      L_g.Ref_Speed = FOC_U.Ref_Speed_mech_rpm;
    }
  #endif
  
  // static float prev_thr_volt = 0.0f;
  // L_g.Throttle_Voltage = analog_buffer_data_adc_1[THROTTLE] * (3.297f / 65536.0f) * 2.0f;
  // L_g.Throttle_Voltage = L_g.Throttle_Voltage * (1.0f - 0.5f) + prev_thr_volt * 0.5f;
  // prev_thr_volt = L_g.Throttle_Voltage;

  // if (alignment_done && !is_thr_valid) {
  //   if (start_flag == 0) {
  //     FOC_U.Id_Iq_MTPA_limit.Id_low_limit = -512.0f;
  //     FOC_U.Id_Iq_MTPA_limit.Id_up_limit = 282.0f;
  //     FOC_U.Id_Iq_MTPA_limit.Iq_low_limit = -282.0f;
  //     FOC_U.Id_Iq_MTPA_limit.Iq_up_limit = 512.0f;

  //     start_flag = 1;
  //   }
  // }

  // if (L_g.Throttle_Voltage >= 1.2f && start_flag && (curr_thr_state == DRIVE || curr_thr_state == REVERSE)) {
  //   is_thr_valid = 1;
  // }

  /**************************************************************/
  /* Throttle State Machine */
  // if (FOC_Out.forward_pin && !FOC_Out.reverse_pin && (fabsf(FOC_Out.Filtered_Speed) * 9.55f) <= 10.0f && L_g.Throttle_Voltage <= 1.1f && start_flag) {
  //   curr_thr_state = DRIVE;
  //   prev_thr_state = curr_thr_state;
  //   thr_disabled = 0;

  //   FOC_U.MTPA_PID.Up_Limit_torque_PID = (61.0f);
  //   FOC_U.MTPA_PID.Low_Limit_torque_PID = (0.0f);
  //   FOC_U.MTPA_PID.Up_Limit_flux_PID = (61.0f);
  //   FOC_U.MTPA_PID.Low_Limit_flux_PID = -(61.0f);
  // } else if (!FOC_Out.forward_pin && FOC_Out.reverse_pin && (fabsf(FOC_Out.Filtered_Speed) * 9.55f) <= 10.0f && L_g.Throttle_Voltage <= 1.1f && start_flag) {
  //   curr_thr_state = REVERSE;
  //   prev_thr_state = curr_thr_state;
  //   thr_disabled = 0;

  //   FOC_U.MTPA_PID.Up_Limit_torque_PID = (61.0f);
  //   FOC_U.MTPA_PID.Low_Limit_torque_PID = -(61.0f);
  //   FOC_U.MTPA_PID.Up_Limit_flux_PID = (61.0f);
  //   FOC_U.MTPA_PID.Low_Limit_flux_PID = -(61.0f);
  // } else if (FOC_Out.forward_pin && FOC_Out.reverse_pin) {
  //   curr_thr_state = NEUTRAL;
  //   thr_disabled = 1;
  // }
  /**************************************************************/

	/**************************************************************/
	/* Calculating the NTC Temperatures and VBUS Voltage */
	FOC_Out.Phase_U_Temp = FOC_U.MCTemperature_C    = L_g.Controller_Temp = NTC_Read(analog_buffer_data_adc_2[TEMP_U]);
	FOC_Out.Motor_Temp   = FOC_U.MotorTemperature_C = L_g.Motor_Temp = NTC_Read(analog_buffer_data_adc_2[TEMP_M]);
	FOC_Out.Phase_V_Temp = FOC_U.MCTemperature_C    = NTC_Read(analog_buffer_data_adc_2[TEMP_V]);
	FOC_Out.Phase_W_Temp = FOC_U.MCTemperature_C    = NTC_Read(analog_buffer_data_adc_2[TEMP_W]);

	FOC_Out.Vbus_Voltage = (float)(analog_buffer_data_adc_2[VBUS_V] * 0.00206f - 2.4f);
  FOC_Out.lv_12v_voltage = (float)(analog_buffer_data_adc_1[LV_12_V] * 0.00005031f);
  FOC_Out.lv_12v_voltage *= 3.75f;
	/**************************************************************/

	/**************************************************************/
	/* Calculating I rms, V rms and DC A */
	L_g.I_rms = sqrtf(powf(FOC_Y.Iq, 2.0f) + powf(FOC_Y.Id, 2.0f)) / 1.414f;
	L_g.V_rms = sqrtf(powf(FOC_Y.Vq, 2.0f) + powf(FOC_Y.Vd, 2.0f)) / 1.414f;
	L_g.I_dc = 1.5f * L_g.I_rms * (L_g.V_rms / FOC_U.BusVoltage_V);

	L_g.Irms_Filtered = LPF_Update(&Irms_LPF, L_g.I_rms);
	L_g.Vrms_Filtered = LPF_Update(&Vrms_LPF, L_g.V_rms);
  L_g.lv_12v_avg = LPF_Update(&LV_12v, FOC_Out.lv_12v_voltage);
	/**************************************************************/

	/**************************************************************/
	/* Reading faults and cutting off the drive */
	// Fault_Check();
	/**************************************************************/
	/* Sending DATA on CAN at 500ms */
	uint32_t curr_time = HAL_GetTick();
  static uint32_t prev_time_500ms = 0;

  if (curr_time - prev_time_500ms >= 500)
  {
    Send_all_data_CAN(FOC_Out, L_g);
    prev_time_500ms = curr_time;

    // align_counter += 1;

    // if (align_counter >= 2 && align_flag) {
    //   align_id_ref = 0.0f;

    //   if (align_counter >= 3) {
    //     align_iq_ref = 15.0f;
    //     align_counter = 0;
    //     align_flag = 0;
    //   }
    // }

    // if (align_flag == 0 && align_counter >= 2) {
    //   align_iq_ref = 0.0f;
    //   align_counter = 0;
    //   alignment_done = 1;
    // }
  }

  #if PID_TUNING
  static uint32_t prev_time_PID = 0;

  if (curr_time - prev_time_PID >= 1000)
  {
    Send_on_CAN_308();
    Send_on_CAN_309();
    prev_time_PID = curr_time;
  }
  #endif
	/**************************************************************/
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 50;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 8;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_3;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief Peripherals Common Clock Configuration
  * @retval None
  */
void PeriphCommonClock_Config(void)
{
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Initializes the peripherals clock
  */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInitStruct.PLL2.PLL2M = 4;
  PeriphClkInitStruct.PLL2.PLL2N = 9;
  PeriphClkInitStruct.PLL2.PLL2P = 4;
  PeriphClkInitStruct.PLL2.PLL2Q = 4;
  PeriphClkInitStruct.PLL2.PLL2R = 2;
  PeriphClkInitStruct.PLL2.PLL2RGE = RCC_PLL2VCIRANGE_3;
  PeriphClkInitStruct.PLL2.PLL2VCOSEL = RCC_PLL2VCOMEDIUM;
  PeriphClkInitStruct.PLL2.PLL2FRACN = 3072;
  PeriphClkInitStruct.AdcClockSelection = RCC_ADCCLKSOURCE_PLL2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_MultiModeTypeDef multimode = {0};
  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV2;
  hadc1.Init.Resolution = ADC_RESOLUTION_16B;
  hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc1.Init.LowPowerAutoWait = DISABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.NbrOfConversion = 4;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_EXTERNALTRIG_T1_TRGO;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
  hadc1.Init.ConversionDataManagement = ADC_CONVERSIONDATA_DMA_CIRCULAR;
  hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc1.Init.LeftBitShift = ADC_LEFTBITSHIFT_NONE;
  hadc1.Init.OversamplingMode = DISABLE;
  hadc1.Init.Oversampling.Ratio = 1;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure the ADC multi-mode
  */
  multimode.Mode = ADC_MODE_INDEPENDENT;
  if (HAL_ADCEx_MultiModeConfigChannel(&hadc1, &multimode) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_3;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_32CYCLES_5;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  sConfig.OffsetSignedSaturation = DISABLE;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_19;
  sConfig.Rank = ADC_REGULAR_RANK_2;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_6;
  sConfig.Rank = ADC_REGULAR_RANK_3;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_17;
  sConfig.Rank = ADC_REGULAR_RANK_4;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief ADC2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC2_Init(void)
{

  /* USER CODE BEGIN ADC2_Init 0 */

  /* USER CODE END ADC2_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC2_Init 1 */

  /* USER CODE END ADC2_Init 1 */

  /** Common config
  */
  hadc2.Instance = ADC2;
  hadc2.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV2;
  hadc2.Init.Resolution = ADC_RESOLUTION_16B;
  hadc2.Init.ScanConvMode = ADC_SCAN_ENABLE;
  hadc2.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc2.Init.LowPowerAutoWait = DISABLE;
  hadc2.Init.ContinuousConvMode = ENABLE;
  hadc2.Init.NbrOfConversion = 5;
  hadc2.Init.DiscontinuousConvMode = DISABLE;
  hadc2.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc2.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc2.Init.ConversionDataManagement = ADC_CONVERSIONDATA_DMA_CIRCULAR;
  hadc2.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc2.Init.LeftBitShift = ADC_LEFTBITSHIFT_NONE;
  hadc2.Init.OversamplingMode = DISABLE;
  hadc2.Init.Oversampling.Ratio = 1;
  if (HAL_ADC_Init(&hadc2) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_4;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_810CYCLES_5;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  sConfig.OffsetSignedSaturation = DISABLE;
  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_5;
  sConfig.Rank = ADC_REGULAR_RANK_2;
  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_8;
  sConfig.Rank = ADC_REGULAR_RANK_3;
  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_9;
  sConfig.Rank = ADC_REGULAR_RANK_4;
  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_15;
  sConfig.Rank = ADC_REGULAR_RANK_5;
  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC2_Init 2 */

  /* USER CODE END ADC2_Init 2 */

}

/**
  * @brief FDCAN2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_FDCAN2_Init(void)
{

  /* USER CODE BEGIN FDCAN2_Init 0 */

  /* USER CODE END FDCAN2_Init 0 */

  /* USER CODE BEGIN FDCAN2_Init 1 */

  /* USER CODE END FDCAN2_Init 1 */
  hfdcan2.Instance = FDCAN2;
  hfdcan2.Init.FrameFormat = FDCAN_FRAME_CLASSIC;
  hfdcan2.Init.Mode = FDCAN_MODE_NORMAL;
  hfdcan2.Init.AutoRetransmission = ENABLE;
  hfdcan2.Init.TransmitPause = DISABLE;
  hfdcan2.Init.ProtocolException = DISABLE;
  hfdcan2.Init.NominalPrescaler = 25;
  hfdcan2.Init.NominalSyncJumpWidth = 1;
  hfdcan2.Init.NominalTimeSeg1 = 13;
  hfdcan2.Init.NominalTimeSeg2 = 1;
  hfdcan2.Init.DataPrescaler = 25;
  hfdcan2.Init.DataSyncJumpWidth = 1;
  hfdcan2.Init.DataTimeSeg1 = 13;
  hfdcan2.Init.DataTimeSeg2 = 2;
  hfdcan2.Init.MessageRAMOffset = 0;
  hfdcan2.Init.StdFiltersNbr = 6;
  hfdcan2.Init.ExtFiltersNbr = 0;
  hfdcan2.Init.RxFifo0ElmtsNbr = 6;
  hfdcan2.Init.RxFifo0ElmtSize = FDCAN_DATA_BYTES_8;
  hfdcan2.Init.RxFifo1ElmtsNbr = 6;
  hfdcan2.Init.RxFifo1ElmtSize = FDCAN_DATA_BYTES_8;
  hfdcan2.Init.RxBuffersNbr = 6;
  hfdcan2.Init.RxBufferSize = FDCAN_DATA_BYTES_8;
  hfdcan2.Init.TxEventsNbr = 0;
  hfdcan2.Init.TxBuffersNbr = 22;
  hfdcan2.Init.TxFifoQueueElmtsNbr = 22;
  hfdcan2.Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION;
  hfdcan2.Init.TxElmtSize = FDCAN_DATA_BYTES_8;
  if (HAL_FDCAN_Init(&hfdcan2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN FDCAN2_Init 2 */

  /* USER CODE END FDCAN2_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 4;
  htim1.Init.CounterMode = TIM_COUNTERMODE_CENTERALIGNED1;
  htim1.Init.Period = 2499;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_ENABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_ENABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 185;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.BreakFilter = 0;
  sBreakDeadTimeConfig.Break2State = TIM_BREAK2_DISABLE;
  sBreakDeadTimeConfig.Break2Polarity = TIM_BREAK2POLARITY_HIGH;
  sBreakDeadTimeConfig.Break2Filter = 0;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_ENABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */
  HAL_TIM_MspPostInit(&htim1);

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_Encoder_InitTypeDef sConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 4096;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  sConfig.EncoderMode = TIM_ENCODERMODE_TI12;
  sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC1Filter = 0;
  sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC2Filter = 0;
  if (HAL_TIM_Encoder_Init(&htim2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief TIM5 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM5_Init(void)
{

  /* USER CODE BEGIN TIM5_Init 0 */

  /* USER CODE END TIM5_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_SlaveConfigTypeDef sSlaveConfig = {0};
  TIM_IC_InitTypeDef sConfigIC = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM5_Init 1 */

  /* USER CODE END TIM5_Init 1 */
  htim5.Instance = TIM5;
  htim5.Init.Prescaler = 0;
  htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim5.Init.Period = 4294967295;
  htim5.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim5.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim5) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim5, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_IC_Init(&htim5) != HAL_OK)
  {
    Error_Handler();
  }
  sSlaveConfig.SlaveMode = TIM_SLAVEMODE_RESET;
  sSlaveConfig.InputTrigger = TIM_TS_TI1FP1;
  sSlaveConfig.TriggerPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sSlaveConfig.TriggerPrescaler = TIM_ICPSC_DIV1;
  sSlaveConfig.TriggerFilter = 0;
  if (HAL_TIM_SlaveConfigSynchro(&htim5, &sSlaveConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(&htim5, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_FALLING;
  sConfigIC.ICSelection = TIM_ICSELECTION_INDIRECTTI;
  if (HAL_TIM_IC_ConfigChannel(&htim5, &sConfigIC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim5, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM5_Init 2 */
  HAL_TIM_IC_Start_IT(&htim5, TIM_CHANNEL_1);
  HAL_TIM_IC_Start_IT(&htim5, TIM_CHANNEL_2);
  /* USER CODE END TIM5_Init 2 */

}

/**
  * @brief TIM17 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM17_Init(void)
{

  /* USER CODE BEGIN TIM17_Init 0 */

  /* USER CODE END TIM17_Init 0 */

  /* USER CODE BEGIN TIM17_Init 1 */

  /* USER CODE END TIM17_Init 1 */
  htim17.Instance = TIM17;
  htim17.Init.Prescaler = 9;
  htim17.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim17.Init.Period = 1999;
  htim17.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim17.Init.RepetitionCounter = 0;
  htim17.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim17) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM17_Init 2 */

  /* USER CODE END TIM17_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream0_IRQn);
  /* DMA2_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_SET);

  /*Configure GPIO pin : PE7 */
  GPIO_InitStruct.Pin = GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : PD10 PD11 PD12 */
  GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : PD15 */
  GPIO_InitStruct.Pin = GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : PD2 */
  GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : PD3 PD4 */
  GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{
  HAL_FDCAN_GetRxMessage(&hfdcan2, FDCAN_RX_FIFO0, &RxMessageBuf, rxMsg);

  if (RxMessageBuf.Identifier == 0x110)
  {
    int32_t raw_current = (int32_t)((rxMsg[7] << 24) | (rxMsg[6] << 16) | (rxMsg[5] << 8) | rxMsg[4]);
    FOC_Out.pack_current = (float)(raw_current) * 1.0E-5;

    if (FOC_Out.pack_current >= 15.0f) {
      FOC_Out.regen_flag = 1;
    } else if (FOC_Out.pack_current <= -15.0f) {
      FOC_Out.regen_flag = 0;
    }
  }

  if (RxMessageBuf.Identifier == 0x109) {
    FOC_Out.pack_voltage = (float)(rxMsg[7] << 8 | rxMsg[6]) * 0.1f;
    FOC_Out.pack_SOC = (float)(rxMsg[1] << 8 | rxMsg[0]) * 0.0001f;
  }

  if (RxMessageBuf.Identifier == 0x12A) {
    FOC_Out.can_DCLO = (float)(rxMsg[3] << 8 | rxMsg[2]) * 0.1f;
    FOC_Out.can_DCLI = (float)(rxMsg[1] << 8 | rxMsg[0]) * 0.1f;
  }

  #if PID_TUNING
  if (RxMessageBuf.Identifier == 0x100)
  {
    /*              SPEED PI PARAMS                     */
    FOC_U.MTPA_PID.Speed_PID_MTPA.Kp_speed_PID_MTPA = rxMsg[0] / 10.0f;
    FOC_U.MTPA_PID.Speed_PID_MTPA.Ki_speed_PID_MTPA = rxMsg[1] / 10.0f;
    FOC_U.MTPA_PID.Speed_PID_MTPA.Kd_speed_PID_MTPA = rxMsg[2] / 10000.0f;
    FOC_U.MTPA_PID.Speed_PID_MTPA.Filter_speed_PID_MTPA = rxMsg[3] / 10.0f;

    /*              FLUX PI PARAMS                     */
    FOC_U.MTPA_PID.Flux_PID_MTPA.Kp_flux_PID_MTPA = rxMsg[4] / 10.0f;
    FOC_U.MTPA_PID.Flux_PID_MTPA.Ki_flux_PID_MTPA = rxMsg[5] / 10.0f;
    FOC_U.MTPA_PID.Flux_PID_MTPA.Kd_flux_PID_MTPA = rxMsg[6] / 10000.0f;
    FOC_U.MTPA_PID.Flux_PID_MTPA.Filter_flux_PID_MTPA = rxMsg[7] / 10.0f;

    Send_on_CAN_308();
  }

  if (RxMessageBuf.Identifier == 0x102)
  {
    /*              TORQUE PI PARAMS                     */
    FOC_U.MTPA_PID.Torque_PID_MTPA.Kp_torque_PID_MTPA = rxMsg[0] / 10.0f;
    FOC_U.MTPA_PID.Torque_PID_MTPA.Ki_torque_PID_MTPA = rxMsg[1] / 10.0f;
    FOC_U.MTPA_PID.Torque_PID_MTPA.Kd_torque_PID_MTPA = rxMsg[2] / 10000.0f;
    FOC_U.MTPA_PID.Torque_PID_MTPA.Filter_torque_PID_MTPA = rxMsg[3] / 10.0f;

    Send_on_CAN_309();
  }
  #endif

  #if SPEED_REF_ON_CAN
  if (RxMessageBuf.Identifier == 0x101)
  {
	  if (rxMsg[0] == 0x01)
	  {
		  can_rx_Flag = 1;
	  }
	  else
	  {
		  can_rx_Flag = 0;
	  }

    ref_speed_can = (float)((rxMsg[2] << 8) | rxMsg[1]);
  }
  #endif
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM17)
	{
    // rtEncoder_to_angle0_U.Count = __HAL_TIM_GET_COUNTER(&htim2);
    volatile uint16_t encoder_count_sensor = __HAL_TIM_GET_COUNTER(&htim2);
    volatile float angle_mech_sensor = ((float)encoder_count_sensor / 4096.0f) * 2.0f * PI;
    volatile float angle_elec_sensor = fmodf((angle_mech_sensor * POLEPAIRS), (2.0f * PI));
    Angle_calibration_U.Theta_e_sensor = angle_elec_sensor; // Update the sensor angle input for calibration
    Speed_Sense(angle_mech_sensor);
		/**************************************************************/
    // rtEncoder_to_angle0_U.Offset = OffsetScheduler_Update(L_g.Irms_Filtered, L_g.Vrms_Filtered);
		/* Calculating Elec Angle and Mech/Elec speed from model */
		// Encoder_to_angle0_step();
		/**************************************************************/

		/**************************************************************/
		/* Calculating the feedback current from the HALL Sensors */
		// volatile float filteredValue_a = 0.0f;
		// volatile float filteredValue_b = 0.0f;

		// for (uint8_t i = 0; i < 10; i++)
		// {
		//   filteredValue_a += (float)analog_buffer_data_adc_1[PHASE_W];
		//   filteredValue_b += (float)analog_buffer_data_adc_1[PHASE_V];
		// }

		// filteredValue_a = filteredValue_a / 10.0f - curr_sen_off.Offset_a;
		// filteredValue_b = filteredValue_b / 10.0f - curr_sen_off.Offset_b;

		// FOC_U.PhaseCurrent[2] = (filteredValue_a * 3.297f * RATED_CURRENT) / 65536.0f;
		// FOC_U.PhaseCurrent[1] = (filteredValue_b * 3.297f * RATED_CURRENT) / 65536.0f;
		// FOC_U.PhaseCurrent[0] = 0.0 - FOC_U.PhaseCurrent[2] - FOC_U.PhaseCurrent[1];
		/**************************************************************/

		/**************************************************************/
		/* Running FOC model */
    #if THROTTLE_CONTROL
    // if (thr_disabled == 0) {
    //   if (is_thr_valid)
    //   {
    //     FOC_U.Ref_Is_Current = get_Is_with_envelope_slew(analog_buffer_data_adc_1[THROTTLE], (fabsf(FOC_Out.Filtered_Speed) * 9.55f), FOC_U.Ref_Is_Current);
    //     L_g.Throttle_Percentage = (FOC_U.Ref_Is_Current / MAX_IS_CURRENT)*100.0f;
    //     L_g.Ref_Speed = FOC_U.Ref_Is_Current;
    //   }
    //   else if (!is_thr_valid)
    //   {
    //     FOC_U.Ref_Is_Current = get_Is_with_envelope_slew(0U, (fabsf(FOC_Out.Filtered_Speed) * 9.55f), FOC_U.Ref_Is_Current);
    //     L_g.Throttle_Percentage = (FOC_U.Ref_Is_Current / MAX_IS_CURRENT)*100.0f;
    //     L_g.Ref_Speed = FOC_U.Ref_Is_Current;
    //   }
    // } else if (thr_disabled == 1) {
    //   FOC_U.Ref_Is_Current = get_Is_with_envelope_slew(0U, (fabsf(FOC_Out.Filtered_Speed) * 9.55f), FOC_U.Ref_Is_Current);
    //   L_g.Throttle_Percentage = (FOC_U.Ref_Is_Current / MAX_IS_CURRENT)*100.0f;
    //   L_g.Ref_Speed = FOC_U.Ref_Is_Current;
    // }
    #endif
		
    // FOC_step0();
    Angle_calibration_step();
		/**************************************************************/

		/**************************************************************/
		/* Calculating the duty cycles and finally setting the registers in TIM1 */
		FOC_Out.pwm_a = (uint16_t)(1250 *(Angle_calibration_Y.V_abc[0] / (Angle_calibration_U.V_up_limit))  + 1250);
		FOC_Out.pwm_b = (uint16_t)(1250 *(Angle_calibration_Y.V_abc[1] / (Angle_calibration_U.V_up_limit))  + 1250);
		FOC_Out.pwm_c = (uint16_t)(1250 *(Angle_calibration_Y.V_abc[2] / (Angle_calibration_U.V_up_limit))  + 1250);

		if (FOC_Out.pwm_a <= 110)
			FOC_Out.pwm_a = 110;
		else if (FOC_Out.pwm_a >= 2349)
			FOC_Out.pwm_a = 2349;

		if (FOC_Out.pwm_b <= 110)
			FOC_Out.pwm_b = 110;
		else if (FOC_Out.pwm_b >= 2349)
			FOC_Out.pwm_b = 2349;

		if (FOC_Out.pwm_c <= 110)
			FOC_Out.pwm_c = 110;
		else if (FOC_Out.pwm_c >= 2349)
			FOC_Out.pwm_c = 2349;

		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,FOC_Out.pwm_c);
		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,FOC_Out.pwm_a);
		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,FOC_Out.pwm_b);
		/**************************************************************/

//		recorderStep();
	}
}

void rt_OneStep(void)
{
  static bool OverrunFlags[2] = { 0, 0 };

  static bool eventFlags[2] = { 0, 0 };/* Model has 2 rates */

  static int taskCounter[2] = { 0, 0 };

  /* Disable interrupts here */

  /* Check base rate for overrun */
  if (OverrunFlags[0]) {
    rtmSetErrorStatus(FOC_M, "Overrun");
    return;
  }

  OverrunFlags[0] = true;

  /* Save FPU context here (if necessary) */
  /* Re-enable timer or interrupt here */

  /*
   * For a bare-board target (i.e., no operating system), the
   * following code checks whether any subrate overruns,
   * and also sets the rates that need to run this time step.
   */
  if (taskCounter[1] == 0) {
    if (eventFlags[1]) {
      OverrunFlags[0] = false;
      OverrunFlags[1] = true;

      /* Sampling too fast */
      rtmSetErrorStatus(FOC_M, "Overrun");
      return;
    }

    eventFlags[1] = true;
  }

  taskCounter[1]++;
  if (taskCounter[1] == 10) {
    taskCounter[1]= 0;
  }

  /* Set model inputs associated with base rate here */


  /* Step the model for base rate */
  FOC_step0();

  /* Get model outputs here */

  /* Indicate task for base rate complete */
  OverrunFlags[0] = false;

  /* If task 1 is running, do not run any lower priority task */
  if (OverrunFlags[1]) {
    return;
  }

  /* Step the model for subrate */
  if (eventFlags[1]) {
    OverrunFlags[1] = true;

    /* Set model inputs associated with subrates here */
    //Baremetal Sensing newly ADDED
//     volatile float Mech_angle = ((TIM2->CNT)/4096.0f)*(6.2831853f);
//     Speed_Sense(Mech_angle);
//     FOC_U.MtrSpd = fabsf(FOC_Out.Filtered_Speed);
     //End of baremetal Speed sensing
    /* Step the model for subrate 1 */
    FOC_step1();

    /* Get model outputs here */

    /* Indicate task complete for subrate */
    OverrunFlags[1] = false;
    eventFlags[1] = false;
  }

  /* Disable interrupts here */
  /* Restore FPU context here (if necessary) */
  /* Enable interrupts here */
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	/**************************************************************/
	/* On GPIO_PIN_7 external interrupt (Z index) resetting TIM2->CNT, Encoder count,
	 * Encoder position to 0
	 * Toggling a variable to visualize z index
	 * incrementing a variable to keep count of indexes
	 */
	if (GPIO_Pin == GPIO_PIN_7)
	{
		// rtEncoder_to_angle0_U.IndexCount = TIM2->CNT;
		// TIM2->CNT = (uint32_t)0;
		// rtEncoder_to_angle0_U.Count = rtEncoder_to_angle0_Y.Theta_e = rtEncoder_to_angle0_Y.Theta_m = 0.0f;
		// FOC_U.MtrElcPos = 0.0f;
	}
	/**************************************************************/
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	/**************************************************************/
	/* Calculating Duty from PWM Signal */
	if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1 || htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
	{
		// FOC_Out.capture_value_0 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
		// if(FOC_Out.capture_value_0)
		// {
		// 	FOC_Out.freq = 200000000.0f / ((float)(FOC_Out.capture_value_0));
		// 	FOC_Out.duty = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2) / ((float)(FOC_Out.capture_value_0));
		// 	FOC_Out.duty *= 100.0f;
		// }
	}
	/**************************************************************/
}

void Initial_Fault_Check()
{
  if ((curr_sen_off.Offset_a >= 40000.0 || curr_sen_off.Offset_a <= 14000.0) || (curr_sen_off.Offset_b >= 40000.0 || curr_sen_off.Offset_b <= 14000.0)) {
    FOC_F_T.OC_Fault |= 1;
    L_g.error_code_1 |= (FOC_F_T.OC_Fault << 3);
    FOC_F_T.Fault_Occured = 1;
  }

  for (uint8_t i = 0; i < 5; i++) {
    FOC_Out.Vbus_Voltage = (float)(analog_buffer_data_adc_2[VBUS_V] * 0.00206f - 2.4f);

    FOC_Out.lv_12v_voltage = (float)(analog_buffer_data_adc_1[LV_12_V] * 0.00005031f);
    FOC_Out.lv_12v_voltage *= 3.75f;
    L_g.lv_12v_avg = LPF_Update(&LV_12v, FOC_Out.lv_12v_voltage);
  }

  if (FOC_Out.Vbus_Voltage <= 48.0f) {
    FOC_F_T.DC_UV_Fault |= 1;
    L_g.error_code |= (FOC_F_T.DC_UV_Fault << 4);
    FOC_F_T.Fault_Occured = 1;
  }

  if (FOC_Out.Vbus_Voltage >= 68.0f) {
    FOC_F_T.DC_OV_Fault |= 1;
    L_g.error_code |= (FOC_F_T.DC_OV_Fault << 5);
    FOC_F_T.Fault_Occured = 1;
  }

  // if (L_g.lv_12v_avg <= (10.5f)) {
	// 	FOC_F_T.Lv_Fault = 1;
	// 	L_g.error_code_2 |= (FOC_F_T.Lv_Fault << 1);
  //   FOC_F_T.Fault_Occured = 1;
	// }

  if (FOC_F_T.Fault_Occured) {
    FOC_F_T.Drive_off = 1;

    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_SET);

    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,1250);
    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,1250);
    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,1250);

    __HAL_TIM_MOE_DISABLE(&htim1);
  }
}

void Regen_Init(void)
{
  DecelDetector_Init(&gDecel,
      /* throttle_deadband         */ 0.03f,
      /* throttle_drop_thresh      */ 0.08f,
      /* speed_decel_thresh_rpm    */ 50.0f,
      /* torque_neg_thresh_nm      */ 5.0f,
      /* speed_min_enable_rpm      */ 200.0f,
      /* clear_speed_rpm           */ 100.0f,
      /* accel_hysteresis          */ 0.05f);

  IqRegenMgr_Init(&gIqMgr,
      /* neg_iq_limit_A            */ -30.0f,
      /* to_zero_step_A            */ 15.0f,
      /* to_negative_step_A        */ 0.01f,
      /* iq_slew_A                 */ 60.0f,
      /* low_speed_release_rpm     */ 200.0f,
      /* forward_only              */ 1u);

  IdRegenMgr_Init(&gIdMgr,
      /* pos_id_limit_A            */ 30.0f,
      /* to_pos_step_A             */ 0.01f,
      /* to_lut_step_A             */ 15.0f,
      /* id_slew_A                 */ 60.0f,
      /* low_speed_release_rpm     */ 200.0f,
      /* forward_only              */ 1u);
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
