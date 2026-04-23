/*
 * File: Throttle_torque_mode.h
 *
 * Code generated for Simulink model 'Throttle_torque_mode'.
 *
 * Model version                  : 1.2
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * C/C++ source code generated on : Sat Aug  2 17:46:08 2025
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef Throttle_torque_mode_h_
#define Throttle_torque_mode_h_
#ifndef Throttle_torque_mode_COMMON_INCLUDES_
#define Throttle_torque_mode_COMMON_INCLUDES_
#include <stdbool.h>
#include <stdint.h>
#include "rtwtypes.h"
#include "main.h"
#include "math.h"
#endif                               /* Throttle_torque_mode_COMMON_INCLUDES_ */

#include "Throttle_torque_mode_types.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

/* Block states (default storage) for system '<Root>' */
typedef struct {
  double PrevY;                        /* '<S4>/Rate Limiter' */
} DW_Throttle_torque_mode_T;

/* Invariant block signals (default storage) */
typedef struct {
  const double Reciprocal;             /* '<S9>/Reciprocal' */
} ConstB_Throttle_torque_mode_T;

/* External inputs (root inport signals with default storage) */
typedef struct {
  double Iq_torque_ratio;              /* '<Root>/Iq_torque_ratio' */
  double Const_ref;                    /* '<Root>/Const_ref' */
  double Throttle_voltage;             /* '<Root>/Throttle_voltage' */
  double I_Max_Value;                  /* '<Root>/I_Max_Value' */
  double Max_Throttle_pos;             /* '<Root>/Max_Throttle_pos' */
  double Min_Throttle_pos;             /* '<Root>/Min_Throttle_pos' */
  double Cons_1_throttle_0;            /* '<Root>/Cons_1_throttle_0' */
  double Max_Power_Watts;              /* '<Root>/Max_Power_Watts' */
  double Speed;                        /* '<Root>/Speed' */
} ExtU_Throttle_torque_mode_T;

/* External outputs (root outports fed by signals with default storage) */
typedef struct {
  double T_ref;                        /* '<Root>/T_ref' */
  double Actual_Torque_demand;         /* '<Root>/Actual_Torque_demand' */
  bool flag;                           /* '<Root>/flag' */
} ExtY_Throttle_torque_mode_T;

/* Real-time Model Data Structure */
struct tag_RTM_Throttle_torque_mode_T {
  const char * volatile errorStatus;
};

/* Block states (default storage) */
extern DW_Throttle_torque_mode_T Throttle_torque_mode_DW;

/* External inputs (root inport signals with default storage) */
extern ExtU_Throttle_torque_mode_T Throttle_torque_mode_U;

/* External outputs (root outports fed by signals with default storage) */
extern ExtY_Throttle_torque_mode_T Throttle_torque_mode_Y;
extern const ConstB_Throttle_torque_mode_T Throttle_torque_mode_ConstB;/* constant block i/o */

/* Model entry point functions */
extern void Throttle_torque_mode_initialize(void);
extern void Throttle_torque_mode_step(void);
extern void Throttle_torque_mode_terminate(void);

/* Real-time Model object */
extern RT_MODEL_Throttle_torque_mode_T *const Throttle_torque_mode_M;

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<S1>/Display' : Unused code path elimination
 * Block '<S4>/Display' : Unused code path elimination
 * Block '<S4>/Display1' : Unused code path elimination
 * Block '<S4>/Display2' : Unused code path elimination
 * Block '<S4>/Scope' : Unused code path elimination
 * Block '<S1>/Scope' : Unused code path elimination
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
 * hilite_system('Throttle_ref_torque/Throttle_torque_mode')    - opens subsystem Throttle_ref_torque/Throttle_torque_mode
 * hilite_system('Throttle_ref_torque/Throttle_torque_mode/Kp') - opens and selects block Kp
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'Throttle_ref_torque'
 * '<S1>'   : 'Throttle_ref_torque/Throttle_torque_mode'
 * '<S2>'   : 'Throttle_ref_torque/Throttle_torque_mode/Ref_Input'
 * '<S3>'   : 'Throttle_ref_torque/Throttle_torque_mode/Subsystem'
 * '<S4>'   : 'Throttle_ref_torque/Throttle_torque_mode/Ref_Input/Throttle'
 * '<S5>'   : 'Throttle_ref_torque/Throttle_torque_mode/Ref_Input/Throttle/If Action Subsystem'
 * '<S6>'   : 'Throttle_ref_torque/Throttle_torque_mode/Ref_Input/Throttle/If Action Subsystem1'
 * '<S7>'   : 'Throttle_ref_torque/Throttle_torque_mode/Ref_Input/Throttle/Subsystem'
 * '<S8>'   : 'Throttle_ref_torque/Throttle_torque_mode/Subsystem/If Action Subsystem'
 * '<S9>'   : 'Throttle_ref_torque/Throttle_torque_mode/Subsystem/If Action Subsystem1'
 */
#endif                                 /* Throttle_torque_mode_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
