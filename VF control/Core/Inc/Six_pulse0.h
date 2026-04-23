/*
 * File: Six_pulse0.h
 *
 * Code generated for Simulink model 'Six_pulse0'.
 *
 * Model version                  : 1.11
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * C/C++ source code generated on : Sat Nov  1 13:01:16 2025
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef Six_pulse0_h_
#define Six_pulse0_h_
#ifndef Six_pulse0_COMMON_INCLUDES_
#define Six_pulse0_COMMON_INCLUDES_
#include <stdbool.h>
#include <stdint.h>
#include "rtwtypes.h"
#include "rt_nonfinite.h"
#include "math.h"
#include "main.h"
#endif                                 /* Six_pulse0_COMMON_INCLUDES_ */

#include "Six_pulse0_types.h"
#include "rt_zcfcn.h"
#include "rtGetNaN.h"
#include "zero_crossing_types.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

/* Block signals (default storage) */
typedef struct {
  double Delay;                        /* '<S5>/Delay' */
  double Delay1;                       /* '<S5>/Delay1' */
  double Delay2;                       /* '<S5>/Delay2' */
  double Delay3;                       /* '<S5>/Delay3' */
  double Delay4;                       /* '<S5>/Delay4' */
  double Delay5;                       /* '<S5>/Delay5' */
  double Delay6;                       /* '<S5>/Delay6' */
  double Delay7;                       /* '<S5>/Delay7' */
  double Delay8;                       /* '<S5>/Delay8' */
  double Delay9;                       /* '<S5>/Delay9' */
  double angle_o;                      /* '<S5>/angle' */
  double OutportBufferForcurrent;      /* '<S5>/Switch' */
  double OutportBufferForangle_1;
  double Volt;                         /* '<S1>/Chart' */
} B_Six_pulse0_T;

/* Block states (default storage) for system '<Root>' */
typedef struct {
  double UnitDelay_DSTATE;             /* '<S9>/Unit Delay' */
  double UnitDelay_DSTATE_c;           /* '<S12>/Unit Delay' */
  double Delay_DSTATE;                 /* '<S1>/Delay' */
  double Delay_DSTATE_e;               /* '<S5>/Delay' */
  double Delay1_DSTATE[2];             /* '<S5>/Delay1' */
  double Delay2_DSTATE[3];             /* '<S5>/Delay2' */
  double Delay3_DSTATE[4];             /* '<S5>/Delay3' */
  double Delay4_DSTATE[5];             /* '<S5>/Delay4' */
  double Delay5_DSTATE;                /* '<S5>/Delay5' */
  double Delay6_DSTATE[2];             /* '<S5>/Delay6' */
  double Delay7_DSTATE[3];             /* '<S5>/Delay7' */
  double Delay8_DSTATE[4];             /* '<S5>/Delay8' */
  double Delay9_DSTATE[5];             /* '<S5>/Delay9' */
  uint16_t temporalCounter_i1;         /* '<S1>/Chart' */
  uint16_t temporalCounter_i2;         /* '<S1>/Chart' */
  uint8_t is_active_c3_Six_pulse0;     /* '<S1>/Chart' */
  uint8_t is_c3_Six_pulse0;            /* '<S1>/Chart' */
  uint8_t is_Init_;                    /* '<S1>/Chart' */
  uint8_t is_calc;                     /* '<S1>/Chart' */
  bool icLoad;                         /* '<S1>/Delay' */
} DW_Six_pulse0_T;

/* Zero-crossing (trigger) state */
typedef struct {
  ZCSigState TriggeredSubsystem1_Trig_ZCE;/* '<S1>/Triggered Subsystem1' */
  ZCSigState TriggeredSubsystem_Trig_ZCE;/* '<S1>/Triggered Subsystem' */
} PrevZCX_Six_pulse0_T;

/* External inputs (root inport signals with default storage) */
typedef struct {
  double volt_in;                      /* '<Root>/volt_in' */
  double Ia;                           /* '<Root>/Ia' */
  double Ib;                           /* '<Root>/Ib' */
  double I_sw_1_normal_0_park;         /* '<Root>/I_sw_1_normal_0_park' */
  double angle_diff;                   /* '<Root>/angle_diff' */
  double Ia_Filter_coefficient;        /* '<Root>/Ia_Filter_coefficient' */
  double Ib_Filter_coefficient;        /* '<Root>/Ib_Filter_coefficient' */
} ExtU_Six_pulse0_T;

/* External outputs (root outports fed by signals with default storage) */
typedef struct {
  double Va;                           /* '<Root>/Va' */
  double Vb;                           /* '<Root>/Vb' */
  double Vc;                           /* '<Root>/Vc' */
  double trig;                         /* '<Root>/trig' */
  double angle;                        /* '<Root>/angle' */
  double last;                         /* '<Root>/last' */
  double angle_array[6];               /* '<Root>/angle_array' */
  double current_array[6];             /* '<Root>/current_array' */
  double Current_min;                  /* '<Root>/Current_min' */
  double angle_offset;                 /* '<Root>/angle_offset' */
  double _enc_sensor_angle_elec;        /* '<Root>/_enc_sensor_angle_mech' */
} ExtY_Six_pulse0_T;

/* Real-time Model Data Structure */
struct tag_RTM_Six_pulse0_T {
  const char * volatile errorStatus;
};

/* Block signals (default storage) */
extern B_Six_pulse0_T Six_pulse0_B;

/* Block states (default storage) */
extern DW_Six_pulse0_T Six_pulse0_DW;

/* Zero-crossing (trigger) state */
extern PrevZCX_Six_pulse0_T Six_pulse0_PrevZCX;

/* External inputs (root inport signals with default storage) */
extern ExtU_Six_pulse0_T Six_pulse0_U;

/* External outputs (root outports fed by signals with default storage) */
extern ExtY_Six_pulse0_T Six_pulse0_Y;

/* Model entry point functions */
extern void Six_pulse0_initialize(void);
extern void Six_pulse0_step(void);
extern void Six_pulse0_terminate(void);

/* Real-time Model object */
extern RT_MODEL_Six_pulse0_T *const Six_pulse0_M;

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<S1>/Display' : Unused code path elimination
 * Block '<S1>/Display1' : Unused code path elimination
 * Block '<S3>/Data Type Duplicate' : Unused code path elimination
 * Block '<S4>/Data Type Duplicate' : Unused code path elimination
 * Block '<S1>/Scope' : Unused code path elimination
 * Block '<S1>/Scope1' : Unused code path elimination
 * Block '<S16>/Data Type Duplicate' : Unused code path elimination
 * Block '<S17>/Data Type Duplicate' : Unused code path elimination
 * Block '<S17>/Data Type Duplicate1' : Unused code path elimination
 * Block '<S1>/Data Type Conversion' : Eliminate redundant data type conversion
 * Block '<S1>/Data Type Conversion1' : Eliminate redundant data type conversion
 * Block '<S15>/Kalpha' : Eliminated nontunable gain of 1
 * Block '<S15>/Kbeta' : Eliminated nontunable gain of 1
 * Block '<S3>/FilterConstant' : Unused code path elimination
 * Block '<S3>/OneMinusFilterConstant' : Unused code path elimination
 * Block '<S3>/UseInputPort' : Unused code path elimination
 * Block '<S4>/FilterConstant' : Unused code path elimination
 * Block '<S4>/OneMinusFilterConstant' : Unused code path elimination
 * Block '<S4>/UseInputPort' : Unused code path elimination
 * Block '<S18>/Offset' : Unused code path elimination
 * Block '<S18>/Unary_Minus' : Unused code path elimination
 */

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Note that this particular code originates from a subsystem build,
 * and has its own system numbers different from the parent model.
 * Refer to the system hierarchy for this subsystem below, and use the
 * MATLAB hilite_system command to trace the generated code back
 * to the parent model.  For example,
 *
 * hilite_system('Six_Pulse_1/Six_pulse')    - opens subsystem Six_Pulse_1/Six_pulse
 * hilite_system('Six_Pulse_1/Six_pulse/Kp') - opens and selects block Kp
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'Six_Pulse_1'
 * '<S1>'   : 'Six_Pulse_1/Six_pulse'
 * '<S2>'   : 'Six_Pulse_1/Six_pulse/Chart'
 * '<S3>'   : 'Six_Pulse_1/Six_pulse/IIR Filter'
 * '<S4>'   : 'Six_Pulse_1/Six_pulse/IIR Filter1'
 * '<S5>'   : 'Six_Pulse_1/Six_pulse/Triggered Subsystem'
 * '<S6>'   : 'Six_Pulse_1/Six_pulse/Triggered Subsystem1'
 * '<S7>'   : 'Six_Pulse_1/Six_pulse/IIR Filter/IIR Filter'
 * '<S8>'   : 'Six_Pulse_1/Six_pulse/IIR Filter/IIR Filter/Low-pass'
 * '<S9>'   : 'Six_Pulse_1/Six_pulse/IIR Filter/IIR Filter/Low-pass/IIR Low Pass Filter'
 * '<S10>'  : 'Six_Pulse_1/Six_pulse/IIR Filter1/IIR Filter'
 * '<S11>'  : 'Six_Pulse_1/Six_pulse/IIR Filter1/IIR Filter/Low-pass'
 * '<S12>'  : 'Six_Pulse_1/Six_pulse/IIR Filter1/IIR Filter/Low-pass/IIR Low Pass Filter'
 * '<S13>'  : 'Six_Pulse_1/Six_pulse/Triggered Subsystem/Clarke Transform'
 * '<S14>'  : 'Six_Pulse_1/Six_pulse/Triggered Subsystem/Park Transform'
 * '<S15>'  : 'Six_Pulse_1/Six_pulse/Triggered Subsystem/Clarke Transform/Two phase input'
 * '<S16>'  : 'Six_Pulse_1/Six_pulse/Triggered Subsystem/Clarke Transform/Two phase input/Two phase CRL wrap'
 * '<S17>'  : 'Six_Pulse_1/Six_pulse/Triggered Subsystem/Park Transform/Two inputs CRL'
 * '<S18>'  : 'Six_Pulse_1/Six_pulse/Triggered Subsystem/Park Transform/Two inputs CRL/Switch_Axis'
 */
#endif                                 /* Six_pulse0_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
