/*
 * File: Angle_calibration.h
 *
 * Code generated for Simulink model 'Angle_calibration'.
 *
 * Model version                  : 1.15
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * C/C++ source code generated on : Fri Oct 17 15:12:06 2025
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef Angle_calibration_h_
#define Angle_calibration_h_
#ifndef Angle_calibration_COMMON_INCLUDES_
#define Angle_calibration_COMMON_INCLUDES_
#include <stdbool.h>
#include <stdint.h>
#include "rtwtypes.h"
#include "rt_nonfinite.h"
#include "math.h"
#include "main.h"
#endif                                 /* Angle_calibration_COMMON_INCLUDES_ */

#include "Angle_calibration_types.h"
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
  double Input;                        /* '<S7>/Input' */
  bool RelationalOperator1;            /* '<S11>/Relational Operator1' */
  bool RelationalOperator1_f;          /* '<S10>/Relational Operator1' */
} B_Angle_calibration_T;

/* Block states (default storage) for system '<Root>' */
typedef struct {
  double UnitDelay_DSTATE;             /* '<S2>/Unit Delay' */
  double DiscreteTimeIntegrator_DSTATE;/* '<S1>/Discrete-Time Integrator' */
  double DelayOneStep_DSTATE;          /* '<S5>/Delay One Step' */
  double PrevY;                        /* '<S1>/Rate Limiter' */
  bool UnitDelay_DSTATE_l;             /* '<S9>/Unit Delay' */
  bool Delay_DSTATE;                   /* '<S6>/Delay' */
} DW_Angle_calibration_T;

/* Zero-crossing (trigger) state */
typedef struct {
  ZCSigState TriggeredSubsystem_Trig_ZCE;/* '<S1>/Triggered Subsystem' */
} PrevZCX_Angle_calibration_T;

/* Invariant block signals (default storage) */
typedef struct {
  const double MultiportSwitch[2];     /* '<S9>/Multiport Switch' */
} ConstB_Angle_calibration_T;

/* External inputs (root inport signals with default storage) */
typedef struct {
  double Speed_ref_rpm;                /* '<Root>/Speed_ref_rpm' */
  double Pole_pairs;                   /* '<Root>/Pole_pairs' */
  double V_up_limit;                   /* '<Root>/V_up_limit' */
  double V_low_limit;                  /* '<Root>/V_low_limit' */
  double VFratio;                      /* '<Root>/V//F ratio' */
  double Theta_e_sensor;               /* '<Root>/Theta_e_sensor' */
  double Angle_selection;              /* '<Root>/Angle_selection' */
} ExtU_Angle_calibration_T;

/* External outputs (root outports fed by signals with default storage) */
typedef struct {
  double Vdc_sat;                      /* '<Root>/Vdc_sat' */
  double V_abc[3];                     /* '<Root>/V_abc' */
  double OffsetAngle;                  /* '<Root>/Offset Angle' */
  double Theta_e_ref;                  /* '<Root>/Theta_e_ref' */
  bool ZCD;                            /* '<Root>/ZCD' */
  double F_ref;                        /* '<Root>/F_ref' */
  double Speed_ref_Ramp;               /* '<Root>/Speed_ref_Ramp' */
} ExtY_Angle_calibration_T;

/* Real-time Model Data Structure */
struct tag_RTM_Angle_calibration_T {
  const char * volatile errorStatus;
};

/* Block signals (default storage) */
extern B_Angle_calibration_T Angle_calibration_B;

/* Block states (default storage) */
extern DW_Angle_calibration_T Angle_calibration_DW;

/* Zero-crossing (trigger) state */
extern PrevZCX_Angle_calibration_T Angle_calibration_PrevZCX;

/* External inputs (root inport signals with default storage) */
extern ExtU_Angle_calibration_T Angle_calibration_U;

/* External outputs (root outports fed by signals with default storage) */
extern ExtY_Angle_calibration_T Angle_calibration_Y;
extern const ConstB_Angle_calibration_T Angle_calibration_ConstB;/* constant block i/o */

/* Model entry point functions */
extern void Angle_calibration_initialize(void);
extern void Angle_calibration_step(void);
extern void Angle_calibration_terminate(void);

/* Real-time Model object */
extern RT_MODEL_Angle_calibration_T *const Angle_calibration_M;

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<S2>/Data Type Duplicate2' : Unused code path elimination
 * Block '<S3>/Data Type Duplicate' : Unused code path elimination
 * Block '<S3>/Data Type Propagation' : Unused code path elimination
 * Block '<S1>/Scope' : Unused code path elimination
 * Block '<S1>/Scope1' : Unused code path elimination
 * Block '<S1>/Scope2' : Unused code path elimination
 * Block '<S9>/Data Type Conversion2' : Eliminate redundant data type conversion
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
 * hilite_system('Open_Loop_new/Angle_calibration')    - opens subsystem Open_Loop_new/Angle_calibration
 * hilite_system('Open_Loop_new/Angle_calibration/Kp') - opens and selects block Kp
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'Open_Loop_new'
 * '<S1>'   : 'Open_Loop_new/Angle_calibration'
 * '<S2>'   : 'Open_Loop_new/Angle_calibration/Position Generator'
 * '<S3>'   : 'Open_Loop_new/Angle_calibration/Saturation Dynamic'
 * '<S4>'   : 'Open_Loop_new/Angle_calibration/Triggered Subsystem'
 * '<S5>'   : 'Open_Loop_new/Angle_calibration/Zero_Crossing_Detection'
 * '<S6>'   : 'Open_Loop_new/Angle_calibration/Position Generator/Accumulate'
 * '<S7>'   : 'Open_Loop_new/Angle_calibration/Position Generator/Accumulate/Subsystem'
 * '<S8>'   : 'Open_Loop_new/Angle_calibration/Zero_Crossing_Detection/Edge Detector'
 * '<S9>'   : 'Open_Loop_new/Angle_calibration/Zero_Crossing_Detection/Edge Detector/Model'
 * '<S10>'  : 'Open_Loop_new/Angle_calibration/Zero_Crossing_Detection/Edge Detector/Model/NEGATIVE Edge'
 * '<S11>'  : 'Open_Loop_new/Angle_calibration/Zero_Crossing_Detection/Edge Detector/Model/POSITIVE Edge'
 */
#endif                                 /* Angle_calibration_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
