/*
 * File: VF_method.h
 *
 * Code generated for Simulink model 'VF_method'.
 *
 * Model version                  : 1.20
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * C/C++ source code generated on : Tue Apr 21 16:54:13 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef VF_method_h_
#define VF_method_h_
#ifndef VF_method_COMMON_INCLUDES_
#define VF_method_COMMON_INCLUDES_
#include <stdbool.h>
#include <stdint.h>
#endif                                 /* VF_method_COMMON_INCLUDES_ */

#include "VF_method_types.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

/* Block states (default storage) for system '<Root>' */
typedef struct {
  float DiscreteTimeIntegrator_DSTATE; /* '<S1>/Discrete-Time Integrator' */
  float PrevY;                         /* '<S1>/Rate Limiter' */
} DW_VF_method_T;

/* External inputs (root inport signals with default storage) */
typedef struct {
  float Speed_ref_rpm;                 /* '<Root>/Speed_ref_rpm' */
  float Pole_pairs;                    /* '<Root>/Pole_pairs' */
  float V_up_limit;                    /* '<Root>/V_up_limit' */
  float V_low_limit;                   /* '<Root>/V_low_limit' */
  float VFratio;                       /* '<Root>/V//F ratio' */
} ExtU_VF_method_T;

/* External outputs (root outports fed by signals with default storage) */
typedef struct {
  float Vdc_sat;                       /* '<Root>/Vdc_sat' */
  float V_abc[3];                      /* '<Root>/V_abc' */
  float Theta_e_ref;                   /* '<Root>/Theta_e_ref' */
  float F_ref;                         /* '<Root>/F_ref' */
  float Speed_ref_Ramp;                /* '<Root>/Speed_ref_Ramp' */
  float Theta_integrated;              /* '<Root>/Theta_integrated' */
  float Speed_rad_s;                   /* '<Root>/Speed_rad_s' */
} ExtY_VF_method_T;

/* Real-time Model Data Structure */
struct tag_RTM_VF_method_T {
  const char * volatile errorStatus;
};

/* Block states (default storage) */
extern DW_VF_method_T VF_method_DW;

/* External inputs (root inport signals with default storage) */
extern ExtU_VF_method_T VF_method_U;

/* External outputs (root outports fed by signals with default storage) */
extern ExtY_VF_method_T VF_method_Y;

/* Model entry point functions */
extern void VF_method_initialize(void);
extern void VF_method_step(void);
extern void VF_method_terminate(void);

/* Real-time Model object */
extern RT_MODEL_VF_method_T *const VF_method_M;

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<S2>/Data Type Duplicate' : Unused code path elimination
 * Block '<S2>/Data Type Propagation' : Unused code path elimination
 * Block '<S1>/Scope1' : Unused code path elimination
 * Block '<S1>/Scope2' : Unused code path elimination
 * Block '<S1>/Scope3' : Unused code path elimination
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
 * hilite_system('VF_model/VF_method')    - opens subsystem VF_model/VF_method
 * hilite_system('VF_model/VF_method/Kp') - opens and selects block Kp
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'VF_model'
 * '<S1>'   : 'VF_model/VF_method'
 * '<S2>'   : 'VF_model/VF_method/Saturation Dynamic'
 */
#endif                                 /* VF_method_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
