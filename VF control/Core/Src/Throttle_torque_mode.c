/*
 * File: Throttle_torque_mode.c
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

#include "Throttle_torque_mode.h"

/* Block states (default storage) */
DW_Throttle_torque_mode_T Throttle_torque_mode_DW;

/* External inputs (root inport signals with default storage) */
ExtU_Throttle_torque_mode_T Throttle_torque_mode_U;

/* External outputs (root outports fed by signals with default storage) */
ExtY_Throttle_torque_mode_T Throttle_torque_mode_Y;

/* Real-time model */
static RT_MODEL_Throttle_torque_mode_T Throttle_torque_mode_M_;
RT_MODEL_Throttle_torque_mode_T *const Throttle_torque_mode_M =
  &Throttle_torque_mode_M_;

/* Model step function */
void Throttle_torque_mode_step(void)
{
  double rtb_SwitchforConstantPedal;

  /* RateLimiter: '<S4>/Rate Limiter' incorporates:
   *  Inport: '<Root>/Throttle_voltage'
   */
  rtb_SwitchforConstantPedal = Throttle_torque_mode_U.Throttle_voltage -
    Throttle_torque_mode_DW.PrevY;
  if (rtb_SwitchforConstantPedal > 2.0E-4) {
    /* RateLimiter: '<S4>/Rate Limiter' */
    Throttle_torque_mode_DW.PrevY += 2.0E-4;
  } else if (rtb_SwitchforConstantPedal < -0.1) {
    /* RateLimiter: '<S4>/Rate Limiter' */
    Throttle_torque_mode_DW.PrevY -= 0.1;
  } else {
    /* RateLimiter: '<S4>/Rate Limiter' */
    Throttle_torque_mode_DW.PrevY = Throttle_torque_mode_U.Throttle_voltage;
  }

  /* End of RateLimiter: '<S4>/Rate Limiter' */

  /* If: '<S4>/If' incorporates:
   *  Inport: '<Root>/Max_Throttle_pos'
   *  Inport: '<Root>/Min_Throttle_pos'
   */
  if (Throttle_torque_mode_DW.PrevY <= Throttle_torque_mode_U.Min_Throttle_pos)
  {
    /* Outputs for IfAction SubSystem: '<S4>/If Action Subsystem' incorporates:
     *  ActionPort: '<S5>/Action Port'
     */
    /* SignalConversion generated from: '<S5>/Ref_value' incorporates:
     *  Constant: '<S5>/Constant'
     */
    rtb_SwitchforConstantPedal = 0.0;

    /* End of Outputs for SubSystem: '<S4>/If Action Subsystem' */
  } else if ((Throttle_torque_mode_DW.PrevY >
              Throttle_torque_mode_U.Min_Throttle_pos) &&
             (Throttle_torque_mode_DW.PrevY <=
              Throttle_torque_mode_U.Max_Throttle_pos)) {
    /* Outputs for IfAction SubSystem: '<S4>/Subsystem' incorporates:
     *  ActionPort: '<S7>/Action Port'
     */
    /* Product: '<S7>/Product' incorporates:
     *  Inport: '<Root>/I_Max_Value'
     *  Inport: '<Root>/Max_Throttle_pos'
     *  Product: '<S7>/Divide1'
     *  Sum: '<S7>/Subtract3'
     *  Sum: '<S7>/Subtract4'
     */
    rtb_SwitchforConstantPedal = 1.0 / (Throttle_torque_mode_U.Max_Throttle_pos
      - Throttle_torque_mode_U.Min_Throttle_pos) *
      Throttle_torque_mode_U.I_Max_Value * (Throttle_torque_mode_DW.PrevY -
      Throttle_torque_mode_U.Min_Throttle_pos);

    /* End of Outputs for SubSystem: '<S4>/Subsystem' */
  } else {
    /* Outputs for IfAction SubSystem: '<S4>/If Action Subsystem1' incorporates:
     *  ActionPort: '<S6>/Action Port'
     */
    /* SignalConversion generated from: '<S6>/Max_i_q' incorporates:
     *  Inport: '<Root>/I_Max_Value'
     */
    rtb_SwitchforConstantPedal = Throttle_torque_mode_U.I_Max_Value;

    /* End of Outputs for SubSystem: '<S4>/If Action Subsystem1' */
  }

  /* End of If: '<S4>/If' */

  /* Switch: '<S2>/Switch for Constant//Pedal' incorporates:
   *  Inport: '<Root>/Cons_1_throttle_0'
   *  Inport: '<Root>/Const_ref'
   */
  if (Throttle_torque_mode_U.Cons_1_throttle_0 > 0.0) {
    rtb_SwitchforConstantPedal = Throttle_torque_mode_U.Const_ref;
  }

  /* Product: '<S1>/Product' incorporates:
   *  Inport: '<Root>/Iq_torque_ratio'
   *  Switch: '<S2>/Switch for Constant//Pedal'
   */
  Throttle_torque_mode_Y.Actual_Torque_demand =
    Throttle_torque_mode_U.Iq_torque_ratio * rtb_SwitchforConstantPedal;

  /* RelationalOperator: '<S3>/Relational Operator' incorporates:
   *  Constant: '<S3>/Constant'
   *  Inport: '<Root>/Max_Power_Watts'
   *  Inport: '<Root>/Speed'
   *  Product: '<S3>/Product'
   */
  Throttle_torque_mode_Y.flag = (Throttle_torque_mode_U.Max_Power_Watts <=
    0.10471975511965977 * Throttle_torque_mode_Y.Actual_Torque_demand *
    Throttle_torque_mode_U.Speed);

  /* If: '<S3>/If' */
  if (!Throttle_torque_mode_Y.flag) {
    /* Outputs for IfAction SubSystem: '<S3>/If Action Subsystem' incorporates:
     *  ActionPort: '<S8>/Action Port'
     */
    /* Outport: '<Root>/T_ref' incorporates:
     *  SignalConversion generated from: '<S8>/T_request'
     */
    Throttle_torque_mode_Y.T_ref = Throttle_torque_mode_Y.Actual_Torque_demand;

    /* End of Outputs for SubSystem: '<S3>/If Action Subsystem' */
  } else {
    /* Outputs for IfAction SubSystem: '<S3>/If Action Subsystem1' incorporates:
     *  ActionPort: '<S9>/Action Port'
     */
    /* Outport: '<Root>/T_ref' incorporates:
     *  Inport: '<Root>/Max_Power_Watts'
     *  Inport: '<Root>/Speed'
     *  Product: '<S9>/Product'
     */
    Throttle_torque_mode_Y.T_ref = Throttle_torque_mode_U.Max_Power_Watts *
      Throttle_torque_mode_ConstB.Reciprocal / Throttle_torque_mode_U.Speed;

    /* End of Outputs for SubSystem: '<S3>/If Action Subsystem1' */
  }

  /* End of If: '<S3>/If' */
}

/* Model initialize function */
void Throttle_torque_mode_initialize(void)
{
  (void *)memset(&Throttle_torque_mode_U, 0, sizeof(ExtU_Throttle_torque_mode_T));
  (void *)memset(&Throttle_torque_mode_Y, 0, sizeof(ExtY_Throttle_torque_mode_T));

  /* InitializeConditions for RateLimiter: '<S4>/Rate Limiter' */
  Throttle_torque_mode_DW.PrevY = 0.399;

  Throttle_torque_mode_U.Cons_1_throttle_0 = 0;
  Throttle_torque_mode_U.I_Max_Value = 450.0f;
  Throttle_torque_mode_U.Iq_torque_ratio = 0.17778f;
  Throttle_torque_mode_U.Max_Throttle_pos = 4.2f;
  Throttle_torque_mode_U.Min_Throttle_pos = 1.2f;
  Throttle_torque_mode_U.Max_Power_Watts = 10500.0f;
}

/* Model terminate function */
void Throttle_torque_mode_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
