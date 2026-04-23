/*
 * File: Encoder_to_angle0.h
 *
 * Code generated for Simulink model 'Encoder_to_angle0'.
 *
 * Model version                  : 1.10
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * C/C++ source code generated on : Fri May 30 13:07:49 2025
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives:
 *    1. Execution efficiency
 *    2. RAM efficiency
 * Validation result: Not run
 */

#ifndef Encoder_to_angle0_h_
#define Encoder_to_angle0_h_
#ifndef Encoder_to_angle0_COMMON_INCLUDES_
#define Encoder_to_angle0_COMMON_INCLUDES_
#include <stdbool.h>
#include <stdint.h>
#include "rtw_continuous.h"
#include "rtw_solver.h"
#include "math.h"
#endif                                 /* Encoder_to_angle0_COMMON_INCLUDES_ */

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

/* Forward declaration for rtModel */
typedef struct Encoder_to_angle0_tag_RTM Encoder_to_angle0_RT_MODEL;

/* Block signals and states (default storage) for system '<Root>' */
typedef struct {
  float Probe[2];                      /* '<S6>/Probe' */
  float Integrator_DSTATE;             /* '<S12>/Integrator' */
  uint32_t CircBufIdx;                 /* '<S5>/Delay' */
  uint16_t Delay_DSTATE[45];           /* '<S5>/Delay' */
  int8_t Integrator_PrevResetState;    /* '<S12>/Integrator' */
  uint8_t Integrator_IC_LOADING;       /* '<S12>/Integrator' */
} Encoder_to_angle0_DW;

/* External inputs (root inport signals with default storage) */
typedef struct {
  double Count;                        /* '<Root>/Count' */
  double IndexCount;                   /* '<Root>/IndexCount' */
  double Offset;                       /* '<Root>/Offset' */
} Encoder_to_angle0_ExtU;

/* External outputs (root outports fed by signals with default storage) */
typedef struct {
  float Theta_e;                       /* '<Root>/Theta_e' */
  float Theta_m;                       /* '<Root>/Theta_m' */
  float Speed_mech;                    /* '<Root>/Speed_mech' */
  float Speed_mech_filtered;           /* '<Root>/Speed_mech_filtered' */
} Encoder_to_angle0_ExtY;

/* Real-time Model Data Structure */
struct Encoder_to_angle0_tag_RTM {
  const char * volatile errorStatus;
};

/* Block signals and states (default storage) */
extern Encoder_to_angle0_DW rtEncoder_to_angle0_DW;

/* External inputs (root inport signals with default storage) */
extern Encoder_to_angle0_ExtU rtEncoder_to_angle0_U;

/* External outputs (root outports fed by signals with default storage) */
extern Encoder_to_angle0_ExtY rtEncoder_to_angle0_Y;

/* Model entry point functions */
extern void Encoder_to_angle0_initialize(void);
extern void Encoder_to_angle0_step(void);

/* Real-time Model object */
extern Encoder_to_angle0_RT_MODEL *const rtEncoder_to_angle0_M;

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<S14>/Data Type Duplicate' : Unused code path elimination
 * Block '<S3>/ReplaceInport_Npp' : Unused code path elimination
 * Block '<S4>/ReplaceInport_CountsPerRev' : Unused code path elimination
 * Block '<S4>/ReplaceInport_OneByCountsPerRev' : Unused code path elimination
 * Block '<S1>/Scope' : Unused code path elimination
 * Block '<S5>/DataTypeSet' : Unused code path elimination
 * Block '<S5>/fixdtDatatype' : Unused code path elimination
 * Block '<S12>/Saturation' : Eliminated Saturate block
 * Block '<S2>/K' : Eliminated nontunable gain of 1
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
 * hilite_system('Encoder_to_angle/Encoder_to_angle')    - opens subsystem Encoder_to_angle/Encoder_to_angle
 * hilite_system('Encoder_to_angle/Encoder_to_angle/Kp') - opens and selects block Kp
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'Encoder_to_angle'
 * '<S1>'   : 'Encoder_to_angle/Encoder_to_angle'
 * '<S2>'   : 'Encoder_to_angle/Encoder_to_angle/Low-Pass Filter (Discrete or Continuous)'
 * '<S3>'   : 'Encoder_to_angle/Encoder_to_angle/Mechanical to Electrical Position'
 * '<S4>'   : 'Encoder_to_angle/Encoder_to_angle/Quadrature Decoder'
 * '<S5>'   : 'Encoder_to_angle/Encoder_to_angle/Speed Measurement'
 * '<S6>'   : 'Encoder_to_angle/Encoder_to_angle/Low-Pass Filter (Discrete or Continuous)/Enable//disable time constant'
 * '<S7>'   : 'Encoder_to_angle/Encoder_to_angle/Low-Pass Filter (Discrete or Continuous)/Initialization'
 * '<S8>'   : 'Encoder_to_angle/Encoder_to_angle/Low-Pass Filter (Discrete or Continuous)/Integrator (Discrete or Continuous)'
 * '<S9>'   : 'Encoder_to_angle/Encoder_to_angle/Low-Pass Filter (Discrete or Continuous)/Enable//disable time constant/Compare To Constant'
 * '<S10>'  : 'Encoder_to_angle/Encoder_to_angle/Low-Pass Filter (Discrete or Continuous)/Enable//disable time constant/Compare To Zero'
 * '<S11>'  : 'Encoder_to_angle/Encoder_to_angle/Low-Pass Filter (Discrete or Continuous)/Initialization/Init_u'
 * '<S12>'  : 'Encoder_to_angle/Encoder_to_angle/Low-Pass Filter (Discrete or Continuous)/Integrator (Discrete or Continuous)/Discrete'
 * '<S13>'  : 'Encoder_to_angle/Encoder_to_angle/Mechanical to Electrical Position/MechToElec'
 * '<S14>'  : 'Encoder_to_angle/Encoder_to_angle/Mechanical to Electrical Position/MechToElec/floating-point'
 * '<S15>'  : 'Encoder_to_angle/Encoder_to_angle/Mechanical to Electrical Position/MechToElec/floating-point/Compensate Offset'
 * '<S16>'  : 'Encoder_to_angle/Encoder_to_angle/Mechanical to Electrical Position/MechToElec/floating-point/Mech To Elec'
 * '<S17>'  : 'Encoder_to_angle/Encoder_to_angle/Mechanical to Electrical Position/MechToElec/floating-point/Compensate Offset/If Action Subsystem'
 * '<S18>'  : 'Encoder_to_angle/Encoder_to_angle/Mechanical to Electrical Position/MechToElec/floating-point/Compensate Offset/If Action Subsystem1'
 * '<S19>'  : 'Encoder_to_angle/Encoder_to_angle/Mechanical to Electrical Position/MechToElec/floating-point/Mech To Elec/Variant Subsystem'
 * '<S20>'  : 'Encoder_to_angle/Encoder_to_angle/Mechanical to Electrical Position/MechToElec/floating-point/Mech To Elec/Variant Subsystem/Dialog'
 * '<S21>'  : 'Encoder_to_angle/Encoder_to_angle/Quadrature Decoder/DT_Handle'
 * '<S22>'  : 'Encoder_to_angle/Encoder_to_angle/Quadrature Decoder/PositionNoReset'
 * '<S23>'  : 'Encoder_to_angle/Encoder_to_angle/Quadrature Decoder/PositionResetAtIndex'
 * '<S24>'  : 'Encoder_to_angle/Encoder_to_angle/Quadrature Decoder/Variant Subsystem'
 * '<S25>'  : 'Encoder_to_angle/Encoder_to_angle/Quadrature Decoder/DT_Handle/floating-point'
 * '<S26>'  : 'Encoder_to_angle/Encoder_to_angle/Quadrature Decoder/PositionNoReset/Variant Subsystem'
 * '<S27>'  : 'Encoder_to_angle/Encoder_to_angle/Quadrature Decoder/PositionNoReset/Variant Subsystem/Dialog'
 * '<S28>'  : 'Encoder_to_angle/Encoder_to_angle/Quadrature Decoder/Variant Subsystem/Dialog'
 * '<S29>'  : 'Encoder_to_angle/Encoder_to_angle/Speed Measurement/DT_Handle'
 * '<S30>'  : 'Encoder_to_angle/Encoder_to_angle/Speed Measurement/DT_Handle/floating-point'
 */
#endif                                 /* Encoder_to_angle0_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
