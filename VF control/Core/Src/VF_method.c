/*
 * File: VF_method.c
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

#include "VF_method.h"
#include <math.h>
#include <stdbool.h>

/* Block states (default storage) */
DW_VF_method_T VF_method_DW;

/* External inputs (root inport signals with default storage) */
ExtU_VF_method_T VF_method_U;

/* External outputs (root outports fed by signals with default storage) */
ExtY_VF_method_T VF_method_Y;

/* Real-time model */
static RT_MODEL_VF_method_T VF_method_M_;
RT_MODEL_VF_method_T *const VF_method_M = &VF_method_M_;

/* Model step function */
void VF_method_step(void)
{
  float q;
  float rtb_F_refHz;
  float rtb_Product;
  float rtb_Product1;
  float rtb_RateLimiter;
  float yTemp;
  bool rEQ0;

  /* RateLimiter: '<S1>/Rate Limiter' incorporates:
   *  Inport: '<Root>/Speed_ref_rpm'
   */
  rtb_RateLimiter = VF_method_U.Speed_ref_rpm - VF_method_DW.PrevY;
  if (rtb_RateLimiter > 0.005F) {
    rtb_RateLimiter = VF_method_DW.PrevY + 0.005F;
  } else if (rtb_RateLimiter < -0.005F) {
    rtb_RateLimiter = VF_method_DW.PrevY - 0.005F;
  } else {
    rtb_RateLimiter = VF_method_U.Speed_ref_rpm;
  }

  VF_method_DW.PrevY = rtb_RateLimiter;

  /* End of RateLimiter: '<S1>/Rate Limiter' */

  /* Product: '<S1>/Product' incorporates:
   *  Inport: '<Root>/Pole_pairs'
   */
  rtb_Product = rtb_RateLimiter * VF_method_U.Pole_pairs;

  /* Gain: '<S1>/Gain' */
  rtb_F_refHz = 0.0166666675F * rtb_Product;

  /* Product: '<S1>/Product1' incorporates:
   *  Inport: '<Root>/V//F ratio'
   */
  rtb_Product1 = VF_method_U.VFratio * rtb_F_refHz;

  /* Switch: '<S2>/Switch2' incorporates:
   *  Inport: '<Root>/V_low_limit'
   *  Inport: '<Root>/V_up_limit'
   *  RelationalOperator: '<S2>/LowerRelop1'
   *  RelationalOperator: '<S2>/UpperRelop'
   *  Switch: '<S2>/Switch'
   */
  if (rtb_Product1 > VF_method_U.V_up_limit) {
    rtb_Product1 = VF_method_U.V_up_limit;
  } else if (rtb_Product1 < VF_method_U.V_low_limit) {
    /* Switch: '<S2>/Switch' incorporates:
     *  Inport: '<Root>/V_low_limit'
     */
    rtb_Product1 = VF_method_U.V_low_limit;
  }

  /* End of Switch: '<S2>/Switch2' */

  /* Outport: '<Root>/Vdc_sat' */
  VF_method_Y.Vdc_sat = rtb_Product1;

  /* Outport: '<Root>/Theta_integrated' incorporates:
   *  DiscreteIntegrator: '<S1>/Discrete-Time Integrator'
   */
  VF_method_Y.Theta_integrated = VF_method_DW.DiscreteTimeIntegrator_DSTATE;

  /* MATLABSystem: '<S1>/Modulo by Constant' incorporates:
   *  Outport: '<Root>/Theta_integrated'
   */
  if (VF_method_Y.Theta_integrated == 0.0F) {
    yTemp = 0.0F;
  } else {
    yTemp = fmodf(VF_method_Y.Theta_integrated, 6.28318548F);
    rEQ0 = (yTemp == 0.0F);
    if (!rEQ0) {
      q = fabsf(VF_method_Y.Theta_integrated / 6.28318548F);
      rEQ0 = (fabsf(q - floorf(q + 0.5F)) <= 1.1920929E-7F * q);
    }

    if (rEQ0) {
      yTemp = 0.0F;
    } else if (yTemp < 0.0F) {
      yTemp += 6.28318548F;
    }
  }

  /* Outport: '<Root>/Theta_e_ref' incorporates:
   *  MATLABSystem: '<S1>/Modulo by Constant'
   */
  VF_method_Y.Theta_e_ref = yTemp;

  /* Outport: '<Root>/V_abc' incorporates:
   *  Constant: '<S1>/-2pi//3'
   *  Constant: '<S1>/2pi//3'
   *  Outport: '<Root>/Theta_e_ref'
   *  Product: '<S1>/Va'
   *  Product: '<S1>/Vb'
   *  Product: '<S1>/Vc'
   *  Sum: '<S1>/Add'
   *  Sum: '<S1>/Add1'
   *  Trigonometry: '<S1>/sin_wt'
   *  Trigonometry: '<S1>/sin_wt+2pi//3'
   *  Trigonometry: '<S1>/sin_wt-2pi//3'
   */
  VF_method_Y.V_abc[0] = rtb_Product1 * sinf(VF_method_Y.Theta_e_ref);
  VF_method_Y.V_abc[1] = sinf((float)(VF_method_Y.Theta_e_ref -
    2.0943951023931953)) * rtb_Product1;
  VF_method_Y.V_abc[2] = sinf((float)(VF_method_Y.Theta_e_ref +
    2.0943951023931953)) * rtb_Product1;

  /* Outport: '<Root>/F_ref' */
  VF_method_Y.F_ref = rtb_F_refHz;

  /* Gain: '<S1>/rpm_to_rad//s' */
  rtb_Product *= 0.104719758F;

  /* Outport: '<Root>/Speed_rad_s' */
  VF_method_Y.Speed_rad_s = rtb_Product;

  /* Outport: '<Root>/Speed_ref_Ramp' */
  VF_method_Y.Speed_ref_Ramp = rtb_RateLimiter;

  /* Update for DiscreteIntegrator: '<S1>/Discrete-Time Integrator' */
  VF_method_DW.DiscreteTimeIntegrator_DSTATE += 0.0001F * rtb_Product;
}

/* Model initialize function */
void VF_method_initialize(void)
{
  /* (no initialization code required) */
	  VF_method_U.Pole_pairs = 3.0f;
	  VF_method_U.Speed_ref_rpm = 500.0f;
	  VF_method_U.V_up_limit = 300.0f;
	  VF_method_U.V_low_limit = 0.0f;
	  VF_method_U.VFratio = 0.3778f;
}

/* Model terminate function */
void VF_method_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
