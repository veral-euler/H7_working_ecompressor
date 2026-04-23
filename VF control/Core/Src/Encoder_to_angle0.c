/*
 * File: Encoder_to_angle0.c
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

#include "Encoder_to_angle0.h"
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include "main.h"
/* Block signals and states (default storage) */
Encoder_to_angle0_DW rtEncoder_to_angle0_DW;

/* External inputs (root inport signals with default storage) */
Encoder_to_angle0_ExtU rtEncoder_to_angle0_U;

/* External outputs (root outports fed by signals with default storage) */
Encoder_to_angle0_ExtY rtEncoder_to_angle0_Y;

/* Real-time model */
static Encoder_to_angle0_RT_MODEL rtEncoder_to_angle0_M_;
Encoder_to_angle0_RT_MODEL *const rtEncoder_to_angle0_M =
  &rtEncoder_to_angle0_M_;

/* Model step function */
void Encoder_to_angle0_step(void)
{
  float rtb_Product;
  float rtb_SpeedGain;
  uint16_t rtb_Merge;
  bool rtb_LogicalOperator;

  /* Logic: '<S6>/Logical Operator' incorporates:
   *  Constant: '<S10>/Constant'
   *  Constant: '<S6>/Time constant'
   *  RelationalOperator: '<S10>/Compare'
   *  Sum: '<S6>/Sum1'
   */
  rtb_LogicalOperator = ((float)(0.0015915494309189533 -
    rtEncoder_to_angle0_DW.Probe[0]) <= 0.0F);

  /* Outputs for IfAction SubSystem: '<S4>/PositionNoReset' incorporates:
   *  ActionPort: '<S22>/Action Port'
   */
  /* If: '<S4>/If1' incorporates:
   *  Constant: '<S27>/Constant'
   *  Constant: '<S28>/Constant'
   *  DataTypeConversion: '<S1>/Data Type Conversion'
   *  DataTypeConversion: '<S1>/Data Type Conversion1'
   *  DataTypeConversion: '<S25>/DTC'
   *  Inport: '<Root>/Count'
   *  Inport: '<Root>/IndexCount'
   *  MinMax: '<S22>/MinMax'
   *  Product: '<S4>/Product'
   *  Sum: '<S22>/Sum3'
   *  Sum: '<S22>/Sum7'
   */
  rtb_Merge = (uint16_t)((uint16_t)rtEncoder_to_angle0_U.Count - (uint16_t)
    rtEncoder_to_angle0_U.IndexCount);
  if (rtb_Merge > (uint16_t)(rtb_Merge + 4096)) {
    rtb_Merge = (uint16_t)(rtb_Merge + 4096);
  }

  rtb_Product = (float)rtb_Merge * 0.00153398095F;

  /* End of If: '<S4>/If1' */
  /* End of Outputs for SubSystem: '<S4>/PositionNoReset' */

  /* DataTypeConversion: '<S5>/AlgoDataType' incorporates:
   *  Constant: '<S5>/PositionToCount'
   *  Product: '<S5>/Product'
   */
  rtb_Merge = (uint16_t)(10430.3535F * rtb_Product);

  /* Gain: '<S5>/SpeedGain' incorporates:
   *  DataTypeConversion: '<S30>/DTC'
   *  Delay: '<S5>/Delay'
   *  Sum: '<S5>/SpeedCount'
   */
  rtb_SpeedGain = (float)(int16_t)((int16_t)rtb_Merge - (int16_t)
    rtEncoder_to_angle0_DW.Delay_DSTATE[rtEncoder_to_angle0_DW.CircBufIdx]) *
    0.203450516F;

  /* DiscreteIntegrator: '<S12>/Integrator' */
  if (rtEncoder_to_angle0_DW.Integrator_IC_LOADING != 0) {
    rtEncoder_to_angle0_DW.Integrator_DSTATE = rtb_SpeedGain;
    if (rtEncoder_to_angle0_DW.Integrator_DSTATE > FOC_Out.speed_max_lim) {
      rtEncoder_to_angle0_DW.Integrator_DSTATE = FOC_Out.speed_max_lim;
    } else if (rtEncoder_to_angle0_DW.Integrator_DSTATE < -FOC_Out.speed_max_lim) {
      rtEncoder_to_angle0_DW.Integrator_DSTATE = -FOC_Out.speed_max_lim;
    }
  }

  if (rtb_LogicalOperator || (rtEncoder_to_angle0_DW.Integrator_PrevResetState
       != 0)) {
    rtEncoder_to_angle0_DW.Integrator_DSTATE = rtb_SpeedGain;
    if (rtEncoder_to_angle0_DW.Integrator_DSTATE > FOC_Out.speed_max_lim) {
      rtEncoder_to_angle0_DW.Integrator_DSTATE = FOC_Out.speed_max_lim;
    } else if (rtEncoder_to_angle0_DW.Integrator_DSTATE < -FOC_Out.speed_max_lim) {
      rtEncoder_to_angle0_DW.Integrator_DSTATE = -FOC_Out.speed_max_lim;
    }
  }

  /* Outport: '<Root>/Speed_mech_filtered' incorporates:
   *  DiscreteIntegrator: '<S12>/Integrator'
   */
  rtEncoder_to_angle0_Y.Speed_mech_filtered =
    rtEncoder_to_angle0_DW.Integrator_DSTATE;

  L_g.Speed_In_RPM = fabsf(rtEncoder_to_angle0_Y.Speed_mech_filtered);

  L_g.Speed_In_KMPH = L_g.Speed_In_RPM * RPM_TO_KMPH;

  L_g.Mech_Speed = fabsf(rtEncoder_to_angle0_Y.Speed_mech_filtered);

  /* Outport: '<Root>/Speed_mech' */
  rtEncoder_to_angle0_Y.Speed_mech = rtb_SpeedGain;

  /* Outport: '<Root>/Theta_m' */
  rtEncoder_to_angle0_Y.Theta_m = rtb_Product;

  L_g.Mech_Angle = rtEncoder_to_angle0_Y.Theta_m;

  FOC_U.MtrSpd = rtEncoder_to_angle0_Y.Speed_mech_filtered * 0.1047f;

  /* If: '<S15>/If' incorporates:
   *  Constant: '<S17>/Constant'
   *  Gain: '<S20>/Number of pole pairs'
   *  Inport: '<Root>/Offset'
   *  Sum: '<S17>/Add'
   *  Sum: '<S18>/Add'
   *  Switch: '<S14>/Switch'
   */
  if (rtb_Product <= (float)rtEncoder_to_angle0_U.Offset) {
    /* Outputs for IfAction SubSystem: '<S15>/If Action Subsystem' incorporates:
     *  ActionPort: '<S17>/Action Port'
     */
    rtb_Product = (rtb_Product + 6.28318548F) - (float)
      rtEncoder_to_angle0_U.Offset;

    /* End of Outputs for SubSystem: '<S15>/If Action Subsystem' */
  } else {
    /* Outputs for IfAction SubSystem: '<S15>/If Action Subsystem1' incorporates:
     *  ActionPort: '<S18>/Action Port'
     */
    rtb_Product -= (float)rtEncoder_to_angle0_U.Offset;

    /* End of Outputs for SubSystem: '<S15>/If Action Subsystem1' */
  }

  rtb_Product *= 3.0F;

  /* End of If: '<S15>/If' */

  /* Outport: '<Root>/Theta_e' incorporates:
   *  Gain: '<S16>/Multiply'
   *  Gain: '<S16>/Multiply1'
   *  Rounding: '<S16>/Floor'
   *  Sum: '<S16>/Add'
   */
  rtEncoder_to_angle0_Y.Theta_e = rtb_Product - floorf(0.159154937F *
    rtb_Product) * 6.28318548F;

  FOC_U.MtrElcPos = rtEncoder_to_angle0_Y.Theta_e;

  L_g.Elec_Angle = rtEncoder_to_angle0_Y.Theta_e;

  /* Update for Delay: '<S5>/Delay' */
  rtEncoder_to_angle0_DW.Delay_DSTATE[rtEncoder_to_angle0_DW.CircBufIdx] =
    rtb_Merge;
  if (rtEncoder_to_angle0_DW.CircBufIdx < 44U) {
    rtEncoder_to_angle0_DW.CircBufIdx++;
  } else {
    rtEncoder_to_angle0_DW.CircBufIdx = 0U;
  }

  /* End of Update for Delay: '<S5>/Delay' */

  /* Update for DiscreteIntegrator: '<S12>/Integrator' incorporates:
   *  Constant: '<S6>/Time constant'
   *  MinMax: '<S6>/Max'
   *  Product: '<S2>/1//T'
   *  Sum: '<S2>/Sum1'
   */
  rtEncoder_to_angle0_DW.Integrator_IC_LOADING = 0U;
  rtEncoder_to_angle0_DW.Integrator_DSTATE += 1.0F / (float)fmax
    (rtEncoder_to_angle0_DW.Probe[0], 0.0015915494309189533) * (rtb_SpeedGain -
    rtEncoder_to_angle0_DW.Integrator_DSTATE) * 0.0001F;        //Changing to 0.001, Ts of Speed PI, reduces initial oscilaltion for some reason??
  if (rtEncoder_to_angle0_DW.Integrator_DSTATE > FOC_Out.speed_max_lim) {
    rtEncoder_to_angle0_DW.Integrator_DSTATE = FOC_Out.speed_max_lim;
  } else if (rtEncoder_to_angle0_DW.Integrator_DSTATE < -FOC_Out.speed_max_lim) {
    rtEncoder_to_angle0_DW.Integrator_DSTATE = -FOC_Out.speed_max_lim;
  }

  rtEncoder_to_angle0_DW.Integrator_PrevResetState = (int8_t)rtb_LogicalOperator;

  /* End of Update for DiscreteIntegrator: '<S12>/Integrator' */
}

/* Model initialize function */
void Encoder_to_angle0_initialize(void)
{
  /* Start for Probe: '<S6>/Probe' */
  rtEncoder_to_angle0_DW.Probe[0] = 0.0001F;
  rtEncoder_to_angle0_DW.Probe[1] = 0.0F;

  /* InitializeConditions for DiscreteIntegrator: '<S12>/Integrator' */
  rtEncoder_to_angle0_DW.Integrator_IC_LOADING = 1U;

  //  Encoder_to_angle0_U.Offset = 4.292f; // Reverse
/*
  offset   low   high
  PT10     1.28  1.38
  R1       1.28  1.33
  P4       1.28  1.33
  NP01     1.28  1.38
  NP02     1.28  1.30
  please change accordingly
*/
  rtEncoder_to_angle0_U.Offset = 1.33f; // Forward
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
