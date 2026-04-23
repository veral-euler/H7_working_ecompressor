/*
 * File: FOC_types.h
 *
 * Code generated for Simulink model 'FOC'.
 *
 * Model version                  : 18.271
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * C/C++ source code generated on : Tue Jul  1 17:02:14 2025
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef FOC_types_h_
#define FOC_types_h_
#include <stdint.h>
#ifndef DEFINED_TYPEDEF_FOR_Flux_PID_MTPA_
#define DEFINED_TYPEDEF_FOR_Flux_PID_MTPA_

typedef struct {
  double Kp_flux_PID_MTPA;
  double Ki_flux_PID_MTPA;
  double Kd_flux_PID_MTPA;
  double Filter_flux_PID_MTPA;
} Flux_PID_MTPA;

#endif

#ifndef DEFINED_TYPEDEF_FOR_Speed_PID_MTPA_
#define DEFINED_TYPEDEF_FOR_Speed_PID_MTPA_

typedef struct {
  double Kp_speed_PID_MTPA;
  double Ki_speed_PID_MTPA;
  double Kd_speed_PID_MTPA;
  double Filter_speed_PID_MTPA;
} Speed_PID_MTPA;

#endif

#ifndef DEFINED_TYPEDEF_FOR_Torque_PID_MTPA_
#define DEFINED_TYPEDEF_FOR_Torque_PID_MTPA_

typedef struct {
  double Kp_torque_PID_MTPA;
  double Ki_torque_PID_MTPA;
  double Kd_torque_PID_MTPA;
  double Filter_torque_PID_MTPA;
} Torque_PID_MTPA;

#endif

#ifndef DEFINED_TYPEDEF_FOR_PID_MTPABus_
#define DEFINED_TYPEDEF_FOR_PID_MTPABus_

typedef struct {
  Flux_PID_MTPA Flux_PID_MTPA;
  Speed_PID_MTPA Speed_PID_MTPA;
  Torque_PID_MTPA Torque_PID_MTPA;
  double Up_Limit_speed_PID_MTPA;
  double Low_Limit_speed_PID_MTPA;
  double Up_Limit_flux_PID;
  double Low_Limit_flux_PID;
  double Up_Limit_torque_PID;
  double Low_Limit_torque_PID;
} PID_MTPABus;

#endif

#ifndef DEFINED_TYPEDEF_FOR_Id_Iq_MTPA_limitBus_
#define DEFINED_TYPEDEF_FOR_Id_Iq_MTPA_limitBus_

typedef struct {
  float Id_up_limit;
  float Id_low_limit;
  float Iq_up_limit;
  float Iq_low_limit;
} Id_Iq_MTPA_limitBus;

#endif

#ifndef DEFINED_TYPEDEF_FOR_Rate_limiterBus_
#define DEFINED_TYPEDEF_FOR_Rate_limiterBus_

typedef struct {
  float Id_ramp_up;
  float Id_ramp_down;
  float Iq_ramp_up;
  float Iq_ramp_down;
  float Torque_gen_ramp_up;
  float Torque_gen_ramp_down;
} Rate_limiterBus;

#endif

#ifndef DEFINED_TYPEDEF_FOR_IIR_FilterBus_
#define DEFINED_TYPEDEF_FOR_IIR_FilterBus_

typedef struct {
  float Id_Filter_coefficient;
  float Id_Filter_switch;
  float Iq_Filter_coefficient;
  float Iq_Filter_switch;
} IIR_FilterBus;

#endif

#ifndef DEFINED_TYPEDEF_FOR_ThresholdsBus_
#define DEFINED_TYPEDEF_FOR_ThresholdsBus_

typedef struct {
  float MaxCurrentLimit_A;
  float OTWarningLimit_C;
  float OTErrorLimit_C;
  float UVWarningLimit_V;
  float UVErrorLimit_V;
  float OVWarningLimit_V;
  float OVErrorLimit_V;
  float OCWarningLimit_A;
  float OCErrorLimit_A;
  float WarningSpeedLimit_rpm;
  float VoltageProtectionTimeout_msec;
  float TempProtectionTimeout_msec;
  float CurrentProtectionTimeout_msec;
} ThresholdsBus;

#endif

#ifndef DEFINED_TYPEDEF_FOR_TempState_
#define DEFINED_TYPEDEF_FOR_TempState_

typedef enum {
  SafeTemperature = 0,                 /* Default value */
  OT_Warning,
  OT_Error
} TempState;

#endif

#ifndef DEFINED_TYPEDEF_FOR_VoltageState_
#define DEFINED_TYPEDEF_FOR_VoltageState_

typedef enum {
  SafeVoltage = 0,                     /* Default value */
  OV_Warning,
  OV_Error,
  UV_Warning,
  UV_Error
} VoltageState;

#endif

#ifndef DEFINED_TYPEDEF_FOR_CurrentState_
#define DEFINED_TYPEDEF_FOR_CurrentState_

typedef enum {
  SafeCurrent = 0,                     /* Default value */
  OC_Warning,
  OC_Error
} CurrentState;

#endif

/* Forward declaration for rtModel */
typedef struct tag_RTM_FOC_T RT_MODEL_FOC_T;

#endif                                 /* FOC_types_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
