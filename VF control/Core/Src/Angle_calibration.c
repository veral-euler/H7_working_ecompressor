/*
 * File: Angle_calibration.c
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

#include "Angle_calibration.h"
#include <math.h>
#include "rt_nonfinite.h"
#include <stdbool.h>
#include <stdint.h>
#include "zero_crossing_types.h"

/* Block signals (default storage) */
B_Angle_calibration_T Angle_calibration_B;

/* Block states (default storage) */
DW_Angle_calibration_T Angle_calibration_DW;

/* Previous zero-crossings (trigger) states */
PrevZCX_Angle_calibration_T Angle_calibration_PrevZCX;

/* External inputs (root inport signals with default storage) */
ExtU_Angle_calibration_T Angle_calibration_U;

/* External outputs (root outports fed by signals with default storage) */
ExtY_Angle_calibration_T Angle_calibration_Y;

/* Real-time model */
static RT_MODEL_Angle_calibration_T Angle_calibration_M_;
RT_MODEL_Angle_calibration_T *const Angle_calibration_M = &Angle_calibration_M_;

/* Model step function */
void Angle_calibration_step(void)
{
  double rtb_Vc;
  double tmp;
  bool rEQ0;

  /* RateLimiter: '<S1>/Rate Limiter' incorporates:
   *  Inport: '<Root>/Speed_ref_rpm'
   */
  rtb_Vc = Angle_calibration_U.Speed_ref_rpm - Angle_calibration_DW.PrevY;
  if (rtb_Vc > 0.005) {
    /* RateLimiter: '<S1>/Rate Limiter' */
    Angle_calibration_DW.PrevY += 0.005;
  } else if (rtb_Vc < -0.005) {
    /* RateLimiter: '<S1>/Rate Limiter' */
    Angle_calibration_DW.PrevY -= 0.005;
  } else {
    /* RateLimiter: '<S1>/Rate Limiter' */
    Angle_calibration_DW.PrevY = Angle_calibration_U.Speed_ref_rpm;
  }

  /* End of RateLimiter: '<S1>/Rate Limiter' */

  /* Gain: '<S1>/Gain' incorporates:
   *  Inport: '<Root>/Pole_pairs'
   *  Product: '<S1>/Product'
   */
  Angle_calibration_Y.F_ref = Angle_calibration_DW.PrevY *
    Angle_calibration_U.Pole_pairs * 0.016666666666666666;

  /* Outputs for Enabled SubSystem: '<S2>/Accumulate' incorporates:
   *  EnablePort: '<S6>/Enable'
   */
  /* Outputs for Enabled SubSystem: '<S6>/Subsystem' incorporates:
   *  EnablePort: '<S7>/Enable'
   */
  /* Delay: '<S6>/Delay' */
  if (Angle_calibration_DW.Delay_DSTATE) {
    /* SignalConversion generated from: '<S7>/Input' incorporates:
     *  Gain: '<S1>/Gain1'
     *  Gain: '<S1>/Gain2'
     *  Gain: '<S2>/scaleIn'
     */
    Angle_calibration_B.Input = 6.2831853071795862 * Angle_calibration_Y.F_ref *
      0.0001 * 0.15915494309189535;
  }

  /* End of Delay: '<S6>/Delay' */
  /* End of Outputs for SubSystem: '<S6>/Subsystem' */

  /* Sum: '<S6>/Add' incorporates:
   *  UnitDelay: '<S2>/Unit Delay'
   */
  rtb_Vc = Angle_calibration_B.Input + Angle_calibration_DW.UnitDelay_DSTATE;

  /* DataTypeConversion: '<S6>/Data Type Conversion' */
  tmp = floor(rtb_Vc);
  if (rtIsNaN(tmp) || rtIsInf(tmp)) {
    tmp = 0.0;
  } else {
    tmp = fmod(tmp, 65536.0);
  }

  /* Sum: '<S6>/Add1' incorporates:
   *  DataTypeConversion: '<S6>/Data Type Conversion'
   *  DataTypeConversion: '<S6>/Data Type Conversion1'
   *  UnitDelay: '<S2>/Unit Delay'
   */
  Angle_calibration_DW.UnitDelay_DSTATE = rtb_Vc - (double)(tmp < 0.0 ? (int32_t)
    (int16_t)-(int16_t)(uint16_t)-tmp : (int32_t)(int16_t)(uint16_t)tmp);

  /* Update for Delay: '<S6>/Delay' incorporates:
   *  Constant: '<S6>/Constant'
   */
  Angle_calibration_DW.Delay_DSTATE = true;

  /* End of Outputs for SubSystem: '<S2>/Accumulate' */

  /* MATLABSystem: '<S1>/Modulo by Constant' incorporates:
   *  DiscreteIntegrator: '<S1>/Discrete-Time Integrator'
   */
  if (rtIsNaN(Angle_calibration_DW.DiscreteTimeIntegrator_DSTATE) || rtIsInf
      (Angle_calibration_DW.DiscreteTimeIntegrator_DSTATE)) {
    /* Switch: '<S1>/Switch' */
    Angle_calibration_Y.Theta_e_ref = (rtNaN);
  } else if (Angle_calibration_DW.DiscreteTimeIntegrator_DSTATE == 0.0) {
    /* Switch: '<S1>/Switch' */
    Angle_calibration_Y.Theta_e_ref = 0.0;
  } else {
    /* Switch: '<S1>/Switch' */
    Angle_calibration_Y.Theta_e_ref = fmod
      (Angle_calibration_DW.DiscreteTimeIntegrator_DSTATE, 6.2831853071795862);
    rEQ0 = (Angle_calibration_Y.Theta_e_ref == 0.0);
    if (!rEQ0) {
      rtb_Vc = fabs(Angle_calibration_DW.DiscreteTimeIntegrator_DSTATE /
                    6.2831853071795862);
      rEQ0 = !(fabs(rtb_Vc - floor(rtb_Vc + 0.5)) > 2.2204460492503131E-16 *
               rtb_Vc);
    }

    if (rEQ0) {
      /* Switch: '<S1>/Switch' */
      Angle_calibration_Y.Theta_e_ref = 0.0;
    } else if (Angle_calibration_Y.Theta_e_ref < 0.0) {
      /* Switch: '<S1>/Switch' */
      Angle_calibration_Y.Theta_e_ref += 6.2831853071795862;
    }
  }

  /* End of MATLABSystem: '<S1>/Modulo by Constant' */

  /* Switch: '<S1>/Switch' incorporates:
   *  Inport: '<Root>/Angle_selection'
   */
  if (Angle_calibration_U.Angle_selection > 0.0) {
    /* Switch: '<S1>/Switch' incorporates:
     *  Gain: '<S2>/scaleOut'
     *  UnitDelay: '<S2>/Unit Delay'
     */
    Angle_calibration_Y.Theta_e_ref = 6.2831853071795862 *
      Angle_calibration_DW.UnitDelay_DSTATE;
  }

  /* End of Switch: '<S1>/Switch' */

  /* Logic: '<S5>/Logical Operator' incorporates:
   *  Constant: '<S5>/Constant2'
   *  Delay: '<S5>/Delay One Step'
   *  Logic: '<S5>/Logical Operator1'
   *  Logic: '<S5>/Logical Operator2'
   *  RelationalOperator: '<S5>/Relational Operator2'
   *  RelationalOperator: '<S5>/Relational Operator3'
   *  RelationalOperator: '<S5>/Relational Operator4'
   *  RelationalOperator: '<S5>/Relational Operator5'
   */
  rEQ0 = (((!(Angle_calibration_Y.Theta_e_ref >=
              Angle_calibration_DW.DelayOneStep_DSTATE)) ||
           (!(Angle_calibration_DW.DelayOneStep_DSTATE < 0.0))) &&
          ((!(Angle_calibration_Y.Theta_e_ref <=
              Angle_calibration_DW.DelayOneStep_DSTATE)) ||
           (!(Angle_calibration_DW.DelayOneStep_DSTATE > 0.0))));

  /* Outputs for Enabled SubSystem: '<S9>/POSITIVE Edge' incorporates:
   *  EnablePort: '<S11>/Enable'
   */
  if (Angle_calibration_ConstB.MultiportSwitch[0] > 0.0) {
    /* RelationalOperator: '<S11>/Relational Operator1' incorporates:
     *  UnitDelay: '<S9>/Unit Delay'
     */
    Angle_calibration_B.RelationalOperator1 = ((int32_t)rEQ0 > (int32_t)
      Angle_calibration_DW.UnitDelay_DSTATE_l);
  }

  /* End of Outputs for SubSystem: '<S9>/POSITIVE Edge' */

  /* Outputs for Enabled SubSystem: '<S9>/NEGATIVE Edge' incorporates:
   *  EnablePort: '<S10>/Enable'
   */
  if (Angle_calibration_ConstB.MultiportSwitch[1] > 0.0) {
    /* RelationalOperator: '<S10>/Relational Operator1' incorporates:
     *  UnitDelay: '<S9>/Unit Delay'
     */
    Angle_calibration_B.RelationalOperator1_f = ((int32_t)
      Angle_calibration_DW.UnitDelay_DSTATE_l > (int32_t)rEQ0);
  }

  /* End of Outputs for SubSystem: '<S9>/NEGATIVE Edge' */

  /* Logic: '<S9>/Logical Operator1' */
  Angle_calibration_Y.ZCD = (Angle_calibration_B.RelationalOperator1 ||
    Angle_calibration_B.RelationalOperator1_f);

  /* Outputs for Triggered SubSystem: '<S1>/Triggered Subsystem' incorporates:
   *  TriggerPort: '<S4>/Trigger'
   */
  if (Angle_calibration_Y.ZCD &&
      (Angle_calibration_PrevZCX.TriggeredSubsystem_Trig_ZCE != POS_ZCSIG)) {
    /* Outport: '<Root>/Offset Angle' incorporates:
     *  Gain: '<S4>/Gain'
     *  Inport: '<Root>/Theta_e_sensor'
     *  Sum: '<S4>/Subtract'
     */
    Angle_calibration_Y.OffsetAngle = -(Angle_calibration_Y.Theta_e_ref -
      Angle_calibration_U.Theta_e_sensor);
  }

  Angle_calibration_PrevZCX.TriggeredSubsystem_Trig_ZCE =
    Angle_calibration_Y.ZCD;

  /* End of Outputs for SubSystem: '<S1>/Triggered Subsystem' */

  /* Product: '<S1>/Product1' incorporates:
   *  Inport: '<Root>/V//F ratio'
   */
  Angle_calibration_Y.Vdc_sat = Angle_calibration_U.VFratio *
    Angle_calibration_Y.F_ref;

  /* Switch: '<S3>/Switch2' incorporates:
   *  Inport: '<Root>/V_low_limit'
   *  Inport: '<Root>/V_up_limit'
   *  RelationalOperator: '<S3>/LowerRelop1'
   *  RelationalOperator: '<S3>/UpperRelop'
   *  Switch: '<S3>/Switch'
   */
  if (Angle_calibration_Y.Vdc_sat > Angle_calibration_U.V_up_limit) {
    /* Product: '<S1>/Product1' */
    Angle_calibration_Y.Vdc_sat = Angle_calibration_U.V_up_limit;
  } else if (Angle_calibration_Y.Vdc_sat < Angle_calibration_U.V_low_limit) {
    /* Product: '<S1>/Product1' incorporates:
     *  Inport: '<Root>/V_low_limit'
     *  Switch: '<S3>/Switch'
     */
    Angle_calibration_Y.Vdc_sat = Angle_calibration_U.V_low_limit;
  }

  /* End of Switch: '<S3>/Switch2' */

  /* Outport: '<Root>/V_abc' incorporates:
   *  Constant: '<S1>/-2pi//3'
   *  Constant: '<S1>/2pi//3'
   *  Product: '<S1>/Va'
   *  Product: '<S1>/Vb'
   *  Product: '<S1>/Vc'
   *  Sum: '<S1>/Add'
   *  Sum: '<S1>/Add1'
   *  Trigonometry: '<S1>/sin_wt'
   *  Trigonometry: '<S1>/sin_wt+2pi//3'
   *  Trigonometry: '<S1>/sin_wt-2pi//3'
   */
  Angle_calibration_Y.V_abc[0] = Angle_calibration_Y.Vdc_sat * sin
    (Angle_calibration_Y.Theta_e_ref);
  Angle_calibration_Y.V_abc[1] = sin(Angle_calibration_Y.Theta_e_ref -
    2.0943951023931953) * Angle_calibration_Y.Vdc_sat;
  Angle_calibration_Y.V_abc[2] = sin(Angle_calibration_Y.Theta_e_ref +
    2.0943951023931953) * Angle_calibration_Y.Vdc_sat;

  /* Outport: '<Root>/Speed_ref_Ramp' */
  Angle_calibration_Y.Speed_ref_Ramp = Angle_calibration_DW.PrevY;

  /* Update for DiscreteIntegrator: '<S1>/Discrete-Time Integrator' incorporates:
   *  Gain: '<S1>/rpm_to_rad//s'
   */
  Angle_calibration_DW.DiscreteTimeIntegrator_DSTATE += 0.10471975511965977 *
    Angle_calibration_DW.PrevY * 0.0001;

  /* Update for Delay: '<S5>/Delay One Step' */
  Angle_calibration_DW.DelayOneStep_DSTATE = Angle_calibration_Y.Theta_e_ref;

  /* Update for UnitDelay: '<S9>/Unit Delay' */
  Angle_calibration_DW.UnitDelay_DSTATE_l = rEQ0;
}

/* Model initialize function */
void Angle_calibration_initialize(void)
{
  Angle_calibration_PrevZCX.TriggeredSubsystem_Trig_ZCE = POS_ZCSIG;

  Angle_calibration_U.Angle_selection = 0;
  Angle_calibration_U.Pole_pairs = POLEPAIRS;
  Angle_calibration_U.Speed_ref_rpm = 2000.0f;
  Angle_calibration_U.V_up_limit = 300.0f;
  Angle_calibration_U.V_low_limit = 0.0f;
  Angle_calibration_U.VFratio = 0.654f;

  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET);
}

/* Model terminate function */
void Angle_calibration_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
