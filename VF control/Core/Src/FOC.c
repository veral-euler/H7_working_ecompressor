/*
 * File: FOC.c
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

#include "FOC.h"
#include <stdint.h>
#include <math.h>
#include "FOC_private.h"
#include "FOC_types.h"
#include "rt_nonfinite.h"

/* Block signals (default storage) */
B_FOC_T FOC_B;

/* Block states (default storage) */
DW_FOC_T FOC_DW;

/* External inputs (root inport signals with default storage) */
ExtU_FOC_T FOC_U;

/* External outputs (root outports fed by signals with default storage) */
ExtY_FOC_T FOC_Y;

FOC_Flag_T FOC_F_T;

/* Real-time model */
static RT_MODEL_FOC_T FOC_M_;
RT_MODEL_FOC_T *const FOC_M = &FOC_M_;

/* Forward declaration for local functions */
static void FOC_VoltageProtection(void);
double look2_pbinlx(double u0, double u1, const double bp0[], const double bp1[],
                    const double table[], uint32_t prevIndex[], const uint32_t
                    maxIndex[], uint32_t stride)
{
  double fractions[2];
  double frac;
  double yL_0d0;
  double yL_0d1;
  uint32_t bpIndices[2];
  uint32_t bpIdx;
  uint32_t found;
  uint32_t iLeft;
  uint32_t iRght;

  /* Column-major Lookup 2-D
     Search method: 'binary'
     Use previous index: 'on'
     Interpolation method: 'Linear point-slope'
     Extrapolation method: 'Linear'
     Use last breakpoint for index at or above upper limit: 'off'
     Remove protection against out-of-range input in generated code: 'off'
   */
  /* Prelookup - Index and Fraction
     Index Search method: 'binary'
     Extrapolation method: 'Linear'
     Use previous index: 'on'
     Use last breakpoint for index at or above upper limit: 'off'
     Remove protection against out-of-range input in generated code: 'off'
   */
  if (u0 <= bp0[0U]) {
    bpIdx = 0U;
    frac = (u0 - bp0[0U]) / (bp0[1U] - bp0[0U]);
  } else if (u0 < bp0[maxIndex[0U]]) {
    /* Binary Search using Previous Index */
    bpIdx = prevIndex[0U];
    iLeft = 0U;
    iRght = maxIndex[0U];
    found = 0U;
    while (found == 0U) {
      if (u0 < bp0[bpIdx]) {
        iRght = bpIdx - 1U;
        bpIdx = ((bpIdx + iLeft) - 1U) >> 1U;
      } else if (u0 < bp0[bpIdx + 1U]) {
        found = 1U;
      } else {
        iLeft = bpIdx + 1U;
        bpIdx = ((bpIdx + iRght) + 1U) >> 1U;
      }
    }

    frac = (u0 - bp0[bpIdx]) / (bp0[bpIdx + 1U] - bp0[bpIdx]);
  } else {
    bpIdx = maxIndex[0U] - 1U;
    frac = (u0 - bp0[maxIndex[0U] - 1U]) / (bp0[maxIndex[0U]] - bp0[maxIndex[0U]
      - 1U]);
  }

  prevIndex[0U] = bpIdx;
  fractions[0U] = frac;
  bpIndices[0U] = bpIdx;

  /* Prelookup - Index and Fraction
     Index Search method: 'binary'
     Extrapolation method: 'Linear'
     Use previous index: 'on'
     Use last breakpoint for index at or above upper limit: 'off'
     Remove protection against out-of-range input in generated code: 'off'
   */
  if (u1 <= bp1[0U]) {
    bpIdx = 0U;
    frac = (u1 - bp1[0U]) / (bp1[1U] - bp1[0U]);
  } else if (u1 < bp1[maxIndex[1U]]) {
    /* Binary Search using Previous Index */
    bpIdx = prevIndex[1U];
    iLeft = 0U;
    iRght = maxIndex[1U];
    found = 0U;
    while (found == 0U) {
      if (u1 < bp1[bpIdx]) {
        iRght = bpIdx - 1U;
        bpIdx = ((bpIdx + iLeft) - 1U) >> 1U;
      } else if (u1 < bp1[bpIdx + 1U]) {
        found = 1U;
      } else {
        iLeft = bpIdx + 1U;
        bpIdx = ((bpIdx + iRght) + 1U) >> 1U;
      }
    }

    frac = (u1 - bp1[bpIdx]) / (bp1[bpIdx + 1U] - bp1[bpIdx]);
  } else {
    bpIdx = maxIndex[1U] - 1U;
    frac = (u1 - bp1[maxIndex[1U] - 1U]) / (bp1[maxIndex[1U]] - bp1[maxIndex[1U]
      - 1U]);
  }

  prevIndex[1U] = bpIdx;

  /* Column-major Interpolation 2-D
     Interpolation method: 'Linear point-slope'
     Use last breakpoint for index at or above upper limit: 'off'
     Overflow mode: 'wrapping'
   */
  iLeft = bpIdx * stride + bpIndices[0U];
  yL_0d0 = table[iLeft];
  yL_0d0 += (table[iLeft + 1U] - yL_0d0) * fractions[0U];
  iLeft += stride;
  yL_0d1 = table[iLeft];
  return (((table[iLeft + 1U] - yL_0d1) * fractions[0U] + yL_0d1) - yL_0d0) *
    frac + yL_0d0;
}

double rt_hypotd_snf(double u0, double u1)
{
  double a;
  double b;
  double y;
  a = fabs(u0);
  b = fabs(u1);
  if (a < b) {
    a /= b;
    y = sqrt(a * a + 1.0) * b;
  } else if (a > b) {
    b /= a;
    y = sqrt(b * b + 1.0) * a;
  } else if (rtIsNaN(b)) {
    y = (rtNaN);
  } else {
    y = a * 1.4142135623730951;
  }

  return y;
}

/* Function for Chart: '<S3>/Protection_States' */
static void FOC_VoltageProtection(void)
{
  switch (FOC_DW.is_VoltageProtection) {
   case FOC_IN_OV_Error:
    /* Outport: '<Root>/VoltageFlag' */
    FOC_Y.VoltageFlag = OV_Error;
    if (FOC_DW.durationCounter_1_m > 1000U) {
      FOC_DW.durationCounter_2_c0 = 0U;
      FOC_DW.durationCounter_1_h = 0U;
      FOC_DW.is_VoltageProtection = FOC_IN_VoltageSafe;

      /* Outport: '<Root>/VoltageFlag' */
      FOC_Y.VoltageFlag = SafeVoltage;
    } else if (FOC_DW.durationCounter_2_d > 1000U) {
      FOC_DW.durationCounter_2_bq = 0U;
      FOC_DW.durationCounter_1_f = 0U;
      FOC_DW.is_VoltageProtection = FOC_IN_OV_Warning;

      /* Outport: '<Root>/VoltageFlag' */
      FOC_Y.VoltageFlag = OV_Warning;
    }
    break;

   case FOC_IN_OV_Warning:
    /* Outport: '<Root>/VoltageFlag' */
    FOC_Y.VoltageFlag = OV_Warning;
    FOC_DW.durationCounter_2_bq = 0U;
    if (FOC_DW.durationCounter_1_f > 1000U) {
      FOC_DW.durationCounter_2_c0 = 0U;
      FOC_DW.durationCounter_1_h = 0U;
      FOC_DW.is_VoltageProtection = FOC_IN_VoltageSafe;

      /* Outport: '<Root>/VoltageFlag' */
      FOC_Y.VoltageFlag = SafeVoltage;
    }
    break;

   case FOC_IN_UV_Error:
    /* Outport: '<Root>/VoltageFlag' */
    FOC_Y.VoltageFlag = UV_Error;

    /* Inport: '<Root>/BusVoltage_V' */
    /* Inport: '<Root>/BusVoltage_V' */
    if (!(FOC_U.BusVoltage_V > -80.0F)) {
      FOC_DW.durationCounter_1_e3 = 0U;
    }

    if (FOC_DW.durationCounter_1_e3 > 1000U) {
      FOC_DW.durationCounter_2_k = 0U;
      FOC_DW.durationCounter_1_o = 0U;
      FOC_DW.is_VoltageProtection = FOC_IN_UV_Warning;

      /* Outport: '<Root>/VoltageFlag' */
      FOC_Y.VoltageFlag = UV_Warning;
    } else {
      if (!(FOC_U.BusVoltage_V > -60.0F)) {
        FOC_DW.durationCounter_2_n = 0U;
      }

      if (FOC_DW.durationCounter_2_n > 1000U) {
        FOC_DW.durationCounter_2_c0 = 0U;
        FOC_DW.durationCounter_1_h = 0U;
        FOC_DW.is_VoltageProtection = FOC_IN_VoltageSafe;

        /* Outport: '<Root>/VoltageFlag' */
        FOC_Y.VoltageFlag = SafeVoltage;
      }
    }
    break;

   case FOC_IN_UV_Warning:
    /* Outport: '<Root>/VoltageFlag' */
    FOC_Y.VoltageFlag = UV_Warning;

    /* Inport: '<Root>/BusVoltage_V' */
    /* Inport: '<Root>/BusVoltage_V' */
    if (!(FOC_U.BusVoltage_V < -80.0F)) {
      FOC_DW.durationCounter_1_o = 0U;
    }

    if (FOC_DW.durationCounter_1_o > 1000U) {
      FOC_DW.durationCounter_2_n = 0U;
      FOC_DW.durationCounter_1_e3 = 0U;
      FOC_DW.is_VoltageProtection = FOC_IN_UV_Error;

      /* Outport: '<Root>/VoltageFlag' */
      FOC_Y.VoltageFlag = UV_Error;
    } else {
      if (!(FOC_U.BusVoltage_V > -60.0F)) {
        FOC_DW.durationCounter_2_k = 0U;
      }

      if (FOC_DW.durationCounter_2_k > 1000U) {
        FOC_DW.durationCounter_2_c0 = 0U;
        FOC_DW.durationCounter_1_h = 0U;
        FOC_DW.is_VoltageProtection = FOC_IN_VoltageSafe;

        /* Outport: '<Root>/VoltageFlag' */
        FOC_Y.VoltageFlag = SafeVoltage;
      }
    }
    break;

   default:
    /* Outport: '<Root>/VoltageFlag' */
    /* case IN_VoltageSafe: */
    FOC_Y.VoltageFlag = SafeVoltage;
    FOC_DW.durationCounter_2_c0 = 0U;

    /* Inport: '<Root>/BusVoltage_V' */
    if (!(FOC_U.BusVoltage_V < -60.0F)) {
      FOC_DW.durationCounter_1_h = 0U;
    }

    if (FOC_DW.durationCounter_1_h > 1000U) {
      FOC_DW.durationCounter_2_k = 0U;
      FOC_DW.durationCounter_1_o = 0U;
      FOC_DW.is_VoltageProtection = FOC_IN_UV_Warning;

      /* Outport: '<Root>/VoltageFlag' */
      FOC_Y.VoltageFlag = UV_Warning;
    }
    break;
  }
}

/* Model step function for TID0 */
void FOC_step0(void)                   /* Sample time: [0.0001s, 0.0s] */
{
  double rtb_Add1;
  double rtb_IProdOut_l;
  double rtb_IqTable;
  double rtb_Product;
  double rtb_Sum;
  double rtb_Sum_f;
  double rtb_Switch_f_idx_0;
  double rtb_TrigonometricFunction1_tmp;
  double rtb_add_c;
  float rtb_algDD_o1_d;
  float rtb_algDD_o2_o;

  /* Product: '<S159>/delta fall limit' incorporates:
   *  Abs: '<S7>/Abs'
   *  Gain: '<S7>/Gain'
   *  Inport: '<Root>/Actual Speed_mech_rad//sec'
   */
  FOC_Y.Iq_sat = fabs(9.5492965855137211 * FOC_U.MtrSpd);

  /* Product: '<S2>/Product' incorporates:
   *  Inport: '<Root>/Iq_Torque_ratio'
   */
  if (!is_thr_valid) {
    rtb_Product = 0.0f;
  } else {
    rtb_Product = FOC_U.Iq_Torque_ratio * FOC_U.Ref_Is_Current;
  }

  /* Delay: '<S9>/Delay' */
  if (FOC_DW.icLoad) {
    FOC_DW.Delay_DSTATE = rtb_Product;
  }

  /* Product: '<S158>/delta fall limit' incorporates:
   *  Inport: '<Root>/Rate_limiter'
   *  Product: '<S9>/delta rise limit'
   *  SampleTimeMath: '<S9>/sample time'
   *
   * About '<S9>/sample time':
   *  y = K where K = ( w * Ts )
   *   */
  FOC_Y.Id_sat = FOC_U.Rate_limiter.Torque_gen_ramp_up * 0.0001;

  /* Sum: '<S9>/Difference Inputs1' incorporates:
   *  Delay: '<S9>/Delay'
   *
   * Block description for '<S9>/Difference Inputs1':
   *
   *  Add in CPU
   */
  rtb_Product -= FOC_DW.Delay_DSTATE;

  /* Switch: '<S162>/Switch2' incorporates:
   *  RelationalOperator: '<S162>/LowerRelop1'
   */
  if (!(rtb_Product > FOC_Y.Id_sat)) {
    /* Product: '<S158>/delta fall limit' incorporates:
     *  Inport: '<Root>/Rate_limiter'
     *  Product: '<S9>/delta fall limit'
     *  SampleTimeMath: '<S9>/sample time'
     *
     * About '<S9>/sample time':
     *  y = K where K = ( w * Ts )
     *   */
    FOC_Y.Id_sat = 0.0001 * FOC_U.Rate_limiter.Torque_gen_ramp_down;

    /* Switch: '<S162>/Switch' incorporates:
     *  RelationalOperator: '<S162>/UpperRelop'
     */
    if (!(rtb_Product < FOC_Y.Id_sat)) {
      /* Product: '<S158>/delta fall limit' */
      FOC_Y.Id_sat = rtb_Product;
    }

    /* End of Switch: '<S162>/Switch' */
  }

  /* End of Switch: '<S162>/Switch2' */

  /* Sum: '<S9>/Difference Inputs2' incorporates:
   *  Delay: '<S9>/Delay'
   *
   * Block description for '<S9>/Difference Inputs2':
   *
   *  Add in CPU
   */
  FOC_Y.Torque_gen = FOC_Y.Id_sat + FOC_DW.Delay_DSTATE;

  if (FOC_Y.Torque_gen < 0.0f) {
    FOC_Y.Torque_gen = 0.0f;
  }

  volatile float throttle_0to1 = (L_g.Throttle_Voltage - 0.95f) / 3.25f;
  if (throttle_0to1 < 0.0f) {
    throttle_0to1 = 0.0f;
  } else if (throttle_0to1 > 1.0f) {
    throttle_0to1 = 1.0f;
  }

  /* 1) Update shared decel detector (single source of truth) */
  uint8_t accel_hint = (throttle_0to1 > 0.15f); /* simple hint; optional */
  uint8_t decel_active = DecelDetector_Update(&gDecel,
                                              throttle_0to1,
                                              FOC_Y.Torque_gen,
                                              (fabsf(FOC_Out.Filtered_Speed) * 9.55f),
                                              accel_hint);

  /* Lookup_n-D: '<S7>/Id Table' incorporates:
   *  Sum: '<S9>/Difference Inputs2'
   *  Switch: '<S11>/Switch2'
   *
   * Block description for '<S9>/Difference Inputs2':
   *
   *  Add in CPU
   */
  if (!is_thr_valid) {
    rtb_Product = align_id_ref;
  } else {
    volatile float Id_LUT = look2_pbinlx(FOC_Y.Iq_sat, FOC_Y.Torque_gen, FOC_ConstP.pooled2,
      FOC_ConstP.pooled3, FOC_ConstP.IdTable_tableData, FOC_DW.m_bpIndex,
      FOC_ConstP.pooled12, 79U);

    /* 3) Run Id manager (flux boost during regen, or follow LUT) */
    rtb_Product = IdRegenMgr_Step(&gIdMgr, decel_active, !curr_thr_state, (fabsf(FOC_Out.Filtered_Speed) * 9.55f), Id_LUT);
  }

  /* Delay: '<S158>/Delay' */
  if (FOC_DW.icLoad_a) {
    FOC_DW.Delay_DSTATE_a = rtb_Product;
  }

  /* Product: '<S158>/delta fall limit' incorporates:
   *  Inport: '<Root>/Rate_limiter'
   *  Product: '<S158>/delta rise limit'
   *  SampleTimeMath: '<S158>/sample time'
   *
   * About '<S158>/sample time':
   *  y = K where K = ( w * Ts )
   *   */
  FOC_Y.Id_sat = FOC_U.Rate_limiter.Id_ramp_up * 0.0001;

  /* Sum: '<S158>/Difference Inputs1' incorporates:
   *  Delay: '<S158>/Delay'
   *
   * Block description for '<S158>/Difference Inputs1':
   *
   *  Add in CPU
   */
  rtb_Product -= FOC_DW.Delay_DSTATE_a;

  /* Switch: '<S160>/Switch2' incorporates:
   *  RelationalOperator: '<S160>/LowerRelop1'
   */
  if (!(rtb_Product > FOC_Y.Id_sat)) {
    /* Product: '<S158>/delta fall limit' incorporates:
     *  Inport: '<Root>/Rate_limiter'
     *  SampleTimeMath: '<S158>/sample time'
     *
     * About '<S158>/sample time':
     *  y = K where K = ( w * Ts )
     *   */
    FOC_Y.Id_sat = 0.0001 * FOC_U.Rate_limiter.Id_ramp_down;

    /* Switch: '<S160>/Switch' incorporates:
     *  RelationalOperator: '<S160>/UpperRelop'
     */
    if (!(rtb_Product < FOC_Y.Id_sat)) {
      /* Product: '<S158>/delta fall limit' */
      FOC_Y.Id_sat = rtb_Product;
    }

    /* End of Switch: '<S160>/Switch' */
  }

  /* End of Switch: '<S160>/Switch2' */

  /* Sum: '<S158>/Difference Inputs2' incorporates:
   *  Delay: '<S158>/Delay'
   *
   * Block description for '<S158>/Difference Inputs2':
   *
   *  Add in CPU
   */
  FOC_Y.Id_ref = FOC_Y.Id_sat + FOC_DW.Delay_DSTATE_a;

  /* Switch: '<S10>/Switch2' incorporates:
   *  Inport: '<Root>/Id_Iq_MTPA_limit'
   *  RelationalOperator: '<S10>/LowerRelop1'
   *  RelationalOperator: '<S10>/UpperRelop'
   *  Switch: '<S10>/Switch'
   */
  if (FOC_Y.Id_ref > FOC_U.Id_Iq_MTPA_limit.Id_up_limit) {
    /* Product: '<S158>/delta fall limit' */
    FOC_Y.Id_sat = FOC_U.Id_Iq_MTPA_limit.Id_up_limit;
  } else if (FOC_Y.Id_ref < FOC_U.Id_Iq_MTPA_limit.Id_low_limit) {
    /* Product: '<S158>/delta fall limit' incorporates:
     *  Switch: '<S10>/Switch'
     */
    FOC_Y.Id_sat = FOC_U.Id_Iq_MTPA_limit.Id_low_limit;
  } else {
    /* Product: '<S158>/delta fall limit' incorporates:
     *  Switch: '<S10>/Switch'
     */
    FOC_Y.Id_sat = FOC_Y.Id_ref;
  }

  /* End of Switch: '<S10>/Switch2' */

  /* Outputs for Atomic SubSystem: '<S17>/Two phase CRL wrap' */
  /* Gain: '<S18>/one_by_sqrt3' incorporates:
   *  Inport: '<Root>/Phase Current'
   *  Sum: '<S18>/a_plus_2b'
   */
  rtb_IProdOut_l = ((FOC_U.PhaseCurrent[0] + FOC_U.PhaseCurrent[1]) +
                    FOC_U.PhaseCurrent[1]) * 0.57735026918962584;

  /* End of Outputs for SubSystem: '<S17>/Two phase CRL wrap' */

  /* Trigonometry: '<S4>/Trigonometric Function' incorporates:
   *  Inport: '<Root>/angle'
   *  Trigonometry: '<S6>/Trigonometric Function1'
   */
  rtb_Add1 = sin(FOC_U.MtrElcPos);

  /* Trigonometry: '<S4>/Trigonometric Function1' incorporates:
   *  Inport: '<Root>/angle'
   *  Trigonometry: '<S6>/Trigonometric Function'
   */
  rtb_TrigonometricFunction1_tmp = cos(FOC_U.MtrElcPos);

  /* Outputs for Atomic SubSystem: '<S16>/Two inputs CRL' */
  /* Outputs for Atomic SubSystem: '<S17>/Two phase CRL wrap' */
  /* AlgorithmDescriptorDelegate generated from: '<S25>/a16' incorporates:
   *  AlgorithmDescriptorDelegate generated from: '<S18>/a16'
   *  Inport: '<Root>/Phase Current'
   *  Product: '<S25>/acos'
   *  Product: '<S25>/asin'
   *  Product: '<S25>/bcos'
   *  Product: '<S25>/bsin'
   *  Sum: '<S25>/sum_Ds'
   *  Sum: '<S25>/sum_Qs'
   *  Switch: '<S26>/Switch'
   *  Trigonometry: '<S4>/Trigonometric Function'
   *  Trigonometry: '<S4>/Trigonometric Function1'
   */
  rtb_algDD_o1_d = (float)(FOC_U.PhaseCurrent[0] *
    rtb_TrigonometricFunction1_tmp + rtb_IProdOut_l * rtb_Add1);
  rtb_algDD_o2_o = (float)(rtb_IProdOut_l * rtb_TrigonometricFunction1_tmp -
    FOC_U.PhaseCurrent[0] * rtb_Add1);

  /* End of Outputs for SubSystem: '<S17>/Two phase CRL wrap' */
  /* End of Outputs for SubSystem: '<S16>/Two inputs CRL' */

  /* Sum: '<S21>/Add1' incorporates:
   *  Constant: '<S14>/One'
   *  Inport: '<Root>/IIR_Filter_Coefficient'
   *  Product: '<S21>/Product'
   *  Product: '<S21>/Product1'
   *  Sum: '<S14>/Sum'
   *  Switch: '<S14>/Switch1'
   *  UnitDelay: '<S21>/Unit Delay'
   */
  FOC_DW.UnitDelay_DSTATE = (1.0F -
    FOC_U.IIR_Filter_Coefficient.Id_Filter_coefficient) *
    FOC_DW.UnitDelay_DSTATE + rtb_algDD_o1_d *
    FOC_U.IIR_Filter_Coefficient.Id_Filter_coefficient;

  /* Switch: '<S4>/Switch' incorporates:
   *  Inport: '<Root>/IIR_Filter_Coefficient'
   *  UnitDelay: '<S21>/Unit Delay'
   */
  if (FOC_U.IIR_Filter_Coefficient.Id_Filter_switch > 0.0F) {
    /* DataTypeConversion: '<S4>/Data Type Conversion' incorporates:
     *  UnitDelay: '<S21>/Unit Delay'
     */
    FOC_Y.Id = FOC_DW.UnitDelay_DSTATE;
    rtb_algDD_o1_d = FOC_DW.UnitDelay_DSTATE;
  } else {
    /* DataTypeConversion: '<S4>/Data Type Conversion' */
    FOC_Y.Id = rtb_algDD_o1_d;
  }

  /* Sum: '<S33>/Sum3' incorporates:
   *  DataTypeConversion: '<S4>/Data Type Conversion'
   *  Switch: '<S4>/Switch'
   */
  FOC_Y.Id_error = FOC_Y.Id_sat - rtb_algDD_o1_d;

  /* Product: '<S131>/NProd Out' incorporates:
   *  DiscreteIntegrator: '<S123>/Filter'
   *  Inport: '<Root>/MTPA_PID'
   *  Product: '<S121>/DProd Out'
   *  Sum: '<S123>/SumD'
   */
  rtb_Product = (FOC_Y.Id_error * FOC_U.MTPA_PID.Flux_PID_MTPA.Kd_flux_PID_MTPA
                 - FOC_DW.Filter_DSTATE) *
    FOC_U.MTPA_PID.Flux_PID_MTPA.Filter_flux_PID_MTPA;

  if (rtb_Product >= 58.0f) {
    rtb_Product = 58.0f;
  } else if (rtb_Product <= -58.0f) {
    rtb_Product = -58.0f;
  }

  /* Sum: '<S138>/Sum' incorporates:
   *  DiscreteIntegrator: '<S128>/Integrator'
   *  Inport: '<Root>/MTPA_PID'
   *  Product: '<S133>/PProd Out'
   */

  volatile float Id_proportional = FOC_Y.Id_error * FOC_U.MTPA_PID.Flux_PID_MTPA.Kp_flux_PID_MTPA;

  if (Id_proportional >= 58.0f) {
    Id_proportional = 58.0f;
  } else if (Id_proportional <= -58.0f) {
    Id_proportional = -58.0f;
  }

  rtb_Sum = (Id_proportional + FOC_DW.Integrator_DSTATE) + rtb_Product;

  /* Switch: '<S136>/Switch2' incorporates:
   *  Inport: '<Root>/MTPA_PID'
   *  RelationalOperator: '<S136>/LowerRelop1'
   *  RelationalOperator: '<S136>/UpperRelop'
   *  Switch: '<S136>/Switch'
   */
  if (rtb_Sum > FOC_U.MTPA_PID.Up_Limit_flux_PID) {
    /* Switch: '<S136>/Switch2' */
    FOC_Y.Vd_PID = FOC_U.MTPA_PID.Up_Limit_flux_PID;
  } else if (rtb_Sum < FOC_U.MTPA_PID.Low_Limit_flux_PID) {
    /* Switch: '<S136>/Switch2' incorporates:
     *  Switch: '<S136>/Switch'
     */
    FOC_Y.Vd_PID = FOC_U.MTPA_PID.Low_Limit_flux_PID;
  } else {
    /* Switch: '<S136>/Switch2' incorporates:
     *  Switch: '<S136>/Switch'
     */
    FOC_Y.Vd_PID = rtb_Sum;
  }

  /* End of Switch: '<S136>/Switch2' */

  /* Product: '<S24>/Product1' incorporates:
   *  Constant: '<S15>/One'
   *  Inport: '<Root>/IIR_Filter_Coefficient'
   *  Sum: '<S15>/Sum'
   *  UnitDelay: '<S24>/Unit Delay'
   */
  FOC_DW.UnitDelay_DSTATE_f *= 1.0F -
    FOC_U.IIR_Filter_Coefficient.Iq_Filter_coefficient;

  /* Sum: '<S24>/Add1' incorporates:
   *  Inport: '<Root>/IIR_Filter_Coefficient'
   *  Product: '<S24>/Product'
   *  Switch: '<S15>/Switch1'
   *  UnitDelay: '<S24>/Unit Delay'
   */
  rtb_algDD_o1_d = rtb_algDD_o2_o *
    FOC_U.IIR_Filter_Coefficient.Iq_Filter_coefficient +
    FOC_DW.UnitDelay_DSTATE_f;

  /* Switch: '<S4>/Switch1' incorporates:
   *  Inport: '<Root>/IIR_Filter_Coefficient'
   *  UnitDelay: '<S24>/Unit Delay'
   */
  if (FOC_U.IIR_Filter_Coefficient.Iq_Filter_switch > 0.0F) {
    FOC_DW.UnitDelay_DSTATE_f = rtb_algDD_o1_d;
  } else {
    FOC_DW.UnitDelay_DSTATE_f = rtb_algDD_o2_o;
  }

  /* End of Switch: '<S4>/Switch1' */

  /* Lookup_n-D: '<S8>/2-D Lookup Table Lq' incorporates:
   *  DataTypeConversion: '<S4>/Data Type Conversion'
   *  DataTypeConversion: '<S4>/Data Type Conversion1'
   *  UnitDelay: '<S24>/Unit Delay'
   */
  FOC_Y.Lq = look2_pbinlx(FOC_Y.Id, FOC_DW.UnitDelay_DSTATE_f,
    FOC_ConstP.pooled7, FOC_ConstP.uDLookupTableLq_bp02Data,
    FOC_ConstP.uDLookupTableLq_tableData, FOC_DW.m_bpIndex_n,
    FOC_ConstP.uDLookupTableLq_maxIndex, 21U);

  /* Lookup_n-D: '<S7>/Iq Table' incorporates:
   *  Sum: '<S9>/Difference Inputs2'
   *  Switch: '<S11>/Switch2'
   *
   * Block description for '<S9>/Difference Inputs2':
   *
   *  Add in CPU
   */
  if (!is_thr_valid) {
    rtb_IqTable = align_iq_ref;
  } else if (curr_thr_state == DRIVE) {
    volatile float Iq_LUT = look2_pbinlx(FOC_Y.Iq_sat, FOC_Y.Torque_gen, FOC_ConstP.pooled2,
      FOC_ConstP.pooled3, FOC_ConstP.IqTable_tableData, FOC_DW.m_bpIndex_f,
      FOC_ConstP.pooled12, 79U);

    /* 2) Run Iq manager (regen torque shaping) */
    rtb_IqTable = IqRegenMgr_Step(&gIqMgr, decel_active, !curr_thr_state, (fabsf(FOC_Out.Filtered_Speed) * 9.55f), Iq_LUT);
  } else if (curr_thr_state == REVERSE) {
    rtb_IqTable = -1.0f * look2_pbinlx(FOC_Y.Iq_sat, FOC_Y.Torque_gen, FOC_ConstP.pooled2,
      FOC_ConstP.pooled3, FOC_ConstP.IqTable_tableData, FOC_DW.m_bpIndex_f,
      FOC_ConstP.pooled12, 79U);
  } else if (curr_thr_state == NEUTRAL) {
    if (prev_thr_state == DRIVE) {
      volatile float Iq_LUT = look2_pbinlx(FOC_Y.Iq_sat, FOC_Y.Torque_gen, FOC_ConstP.pooled2,
        FOC_ConstP.pooled3, FOC_ConstP.IqTable_tableData, FOC_DW.m_bpIndex_f,
        FOC_ConstP.pooled12, 79U);

      /* 2) Run Iq manager (regen torque shaping) */
      rtb_IqTable = IqRegenMgr_Step(&gIqMgr, decel_active, !curr_thr_state, (fabsf(FOC_Out.Filtered_Speed) * 9.55f), Iq_LUT);
    } else if (prev_thr_state == REVERSE) {
      rtb_IqTable = -1.0f * look2_pbinlx(FOC_Y.Iq_sat, FOC_Y.Torque_gen, FOC_ConstP.pooled2,
        FOC_ConstP.pooled3, FOC_ConstP.IqTable_tableData, FOC_DW.m_bpIndex_f,
        FOC_ConstP.pooled12, 79U);
    }
  }

  /* Delay: '<S159>/Delay' */
  if (FOC_DW.icLoad_h) {
    FOC_DW.Delay_DSTATE_g = rtb_IqTable;
  }

  /* Product: '<S159>/delta fall limit' incorporates:
   *  Inport: '<Root>/Rate_limiter'
   *  Product: '<S159>/delta rise limit'
   *  SampleTimeMath: '<S159>/sample time'
   *
   * About '<S159>/sample time':
   *  y = K where K = ( w * Ts )
   *   */
  FOC_Y.Iq_sat = FOC_U.Rate_limiter.Iq_ramp_up * 0.0001;

  /* Sum: '<S159>/Difference Inputs1' incorporates:
   *  Delay: '<S159>/Delay'
   *
   * Block description for '<S159>/Difference Inputs1':
   *
   *  Add in CPU
   */
  rtb_IqTable -= FOC_DW.Delay_DSTATE_g;

  /* Switch: '<S161>/Switch2' incorporates:
   *  RelationalOperator: '<S161>/LowerRelop1'
   */
  if (!(rtb_IqTable > FOC_Y.Iq_sat)) {
    /* Product: '<S159>/delta fall limit' incorporates:
     *  Inport: '<Root>/Rate_limiter'
     *  SampleTimeMath: '<S159>/sample time'
     *
     * About '<S159>/sample time':
     *  y = K where K = ( w * Ts )
     *   */
    FOC_Y.Iq_sat = 0.0001 * FOC_U.Rate_limiter.Iq_ramp_down;

    /* Switch: '<S161>/Switch' incorporates:
     *  RelationalOperator: '<S161>/UpperRelop'
     */
    if (!(rtb_IqTable < FOC_Y.Iq_sat)) {
      /* Product: '<S159>/delta fall limit' */
      FOC_Y.Iq_sat = rtb_IqTable;
    }

    /* End of Switch: '<S161>/Switch' */
  }

  /* End of Switch: '<S161>/Switch2' */

  /* Sum: '<S159>/Difference Inputs2' incorporates:
   *  Delay: '<S159>/Delay'
   *
   * Block description for '<S159>/Difference Inputs2':
   *
   *  Add in CPU
   */
  FOC_Y.Iq_ref = FOC_Y.Iq_sat + FOC_DW.Delay_DSTATE_g;
  // FOC_Y.Iq_ref *= -1; //REVERSE ATTEMPT

  /* Switch: '<S11>/Switch2' incorporates:
   *  Inport: '<Root>/Id_Iq_MTPA_limit'
   *  RelationalOperator: '<S11>/LowerRelop1'
   *  RelationalOperator: '<S11>/UpperRelop'
   *  Switch: '<S11>/Switch'
   */
  if (FOC_Y.Iq_ref > FOC_U.Id_Iq_MTPA_limit.Iq_up_limit) {
    /* Product: '<S159>/delta fall limit' */
    FOC_Y.Iq_sat = FOC_U.Id_Iq_MTPA_limit.Iq_up_limit;
  } else if (FOC_Y.Iq_ref < FOC_U.Id_Iq_MTPA_limit.Iq_low_limit) {
    /* Product: '<S159>/delta fall limit' incorporates:
     *  Switch: '<S11>/Switch'
     */
    FOC_Y.Iq_sat = FOC_U.Id_Iq_MTPA_limit.Iq_low_limit;
  } else {
    /* Product: '<S159>/delta fall limit' incorporates:
     *  Switch: '<S11>/Switch'
     */
    FOC_Y.Iq_sat = FOC_Y.Iq_ref;
  }

  /* End of Switch: '<S11>/Switch2' */

  /* Math: '<S5>/V_total' incorporates:
   *  Gain: '<S35>/wm_pu2si_mech2elec'
   *  Inport: '<Root>/Actual Speed_mech_rad//sec'
   */
  FOC_Y.V_total_calc = 3.0 * FOC_U.MtrSpd;

  /* Gain: '<S35>/NegSign' incorporates:
   *  Product: '<S35>/prod1'
   *  Switch: '<S36>/Switch1'
   */
  FOC_Y.Vd_FF = -(FOC_Y.Lq * FOC_Y.Iq_sat * FOC_Y.V_total_calc);

  /* Switch: '<S37>/Switch2' incorporates:
   *  Inport: '<Root>/BusVoltage_V'
   *  RelationalOperator: '<S37>/LowerRelop1'
   *  RelationalOperator: '<S37>/UpperRelop'
   *  Switch: '<S37>/Switch'
   *  UnaryMinus: '<S35>/Unary Minus'
   */
  if (FOC_Y.Vd_FF > FOC_U.BusVoltage_V) {
    /* Gain: '<S35>/NegSign' */
    FOC_Y.Vd_FF = FOC_U.BusVoltage_V;
  } else if (FOC_Y.Vd_FF < -FOC_U.BusVoltage_V) {
    /* Gain: '<S35>/NegSign' incorporates:
     *  Switch: '<S37>/Switch'
     *  UnaryMinus: '<S35>/Unary Minus'
     */
    FOC_Y.Vd_FF = -FOC_U.BusVoltage_V;
  }

  /* End of Switch: '<S37>/Switch2' */

  /* Sum: '<S5>/Sum' */
  if (align_flag == 0 && !is_thr_valid) {
    FOC_Y.Vd = 0.0f;
  } else {
    FOC_Y.Vd = FOC_Y.Vd_PID + FOC_Y.Vd_FF;
  }

  /* Switch: '<S30>/Switch2' incorporates:
   *  Inport: '<Root>/MTPA_PID'
   *  RelationalOperator: '<S30>/LowerRelop1'
   *  RelationalOperator: '<S30>/UpperRelop'
   *  Switch: '<S30>/Switch'
   */
  if (FOC_Y.Vd > FOC_U.MTPA_PID.Up_Limit_flux_PID) {
    /* Sum: '<S5>/Sum' */
    FOC_Y.Vd = FOC_U.MTPA_PID.Up_Limit_flux_PID;
  } else if (FOC_Y.Vd < FOC_U.MTPA_PID.Low_Limit_flux_PID) {
    /* Sum: '<S5>/Sum' incorporates:
     *  Switch: '<S30>/Switch'
     */
    FOC_Y.Vd = FOC_U.MTPA_PID.Low_Limit_flux_PID;
  }

  /* End of Switch: '<S30>/Switch2' */

  /* Lookup_n-D: '<S8>/2-D Lookup Table Psim' incorporates:
   *  DataTypeConversion: '<S4>/Data Type Conversion'
   *  DataTypeConversion: '<S4>/Data Type Conversion1'
   *  UnitDelay: '<S24>/Unit Delay'
   */
  FOC_Y.Lambda = look2_pbinlx(FOC_Y.Id, FOC_DW.UnitDelay_DSTATE_f,
    FOC_ConstP.pooled7, FOC_ConstP.pooled8,
    FOC_ConstP.uDLookupTablePsim_tableData, FOC_DW.m_bpIndex_o,
    FOC_ConstP.pooled13, 21U);

  /* Lookup_n-D: '<S8>/2-D Lookup Table Ld' incorporates:
   *  Abs: '<S8>/Iq_abs'
   *  DataTypeConversion: '<S4>/Data Type Conversion'
   *  DataTypeConversion: '<S4>/Data Type Conversion1'
   *  UnitDelay: '<S24>/Unit Delay'
   */
  FOC_Y.Ld = look2_pbinlx(FOC_Y.Id, fabs(FOC_DW.UnitDelay_DSTATE_f),
    FOC_ConstP.pooled7, FOC_ConstP.pooled8, FOC_ConstP.uDLookupTableLd_tableData,
    FOC_DW.m_bpIndex_h, FOC_ConstP.pooled13, 21U);

  /* Product: '<S35>/prod2' incorporates:
   *  Product: '<S35>/prod3'
   *  Sum: '<S35>/add1'
   *  Switch: '<S36>/Switch'
   *  Switch: '<S36>/Switch2'
   */
  FOC_Y.Vq_FF = (FOC_Y.Ld * FOC_Y.Id_sat + FOC_Y.Lambda) * FOC_Y.V_total_calc;

  /* Switch: '<S38>/Switch2' incorporates:
   *  Inport: '<Root>/BusVoltage_V'
   *  RelationalOperator: '<S38>/LowerRelop1'
   *  RelationalOperator: '<S38>/UpperRelop'
   *  Switch: '<S38>/Switch'
   *  UnaryMinus: '<S35>/Unary Minus'
   */
  if (FOC_Y.Vq_FF > FOC_U.BusVoltage_V) {
    /* Product: '<S35>/prod2' */
    FOC_Y.Vq_FF = FOC_U.BusVoltage_V;
  } else if (FOC_Y.Vq_FF < -FOC_U.BusVoltage_V) {
    /* Product: '<S35>/prod2' incorporates:
     *  Switch: '<S38>/Switch'
     *  UnaryMinus: '<S35>/Unary Minus'
     */
    FOC_Y.Vq_FF = -FOC_U.BusVoltage_V;
  }

  /* End of Switch: '<S38>/Switch2' */

  /* Sum: '<S32>/Sum2' incorporates:
   *  DataTypeConversion: '<S4>/Data Type Conversion1'
   *  UnitDelay: '<S24>/Unit Delay'
   */
  FOC_Y.Iq_error = FOC_Y.Iq_sat - FOC_DW.UnitDelay_DSTATE_f;

  /* Product: '<S77>/NProd Out' incorporates:
   *  DiscreteIntegrator: '<S69>/Filter'
   *  Inport: '<Root>/MTPA_PID'
   *  Product: '<S67>/DProd Out'
   *  Sum: '<S69>/SumD'
   */
  rtb_IqTable = (FOC_Y.Iq_error *
                 FOC_U.MTPA_PID.Torque_PID_MTPA.Kd_torque_PID_MTPA -
                 FOC_DW.Filter_DSTATE_d) *
    FOC_U.MTPA_PID.Torque_PID_MTPA.Filter_torque_PID_MTPA;

  if (rtb_IqTable >= 58.0f) {
    rtb_IqTable = 58.0f;
  } else if (rtb_IqTable <= -58.0f) {
    rtb_IqTable = -58.0f;
  }

  /* Sum: '<S84>/Sum' incorporates:
   *  DiscreteIntegrator: '<S74>/Integrator'
   *  Inport: '<Root>/MTPA_PID'
   *  Product: '<S79>/PProd Out'
   */
  
  volatile float Iq_proportional = FOC_Y.Iq_error * FOC_U.MTPA_PID.Torque_PID_MTPA.Kp_torque_PID_MTPA;

  if (Iq_proportional >= 58.0f) {
    Iq_proportional = 58.0f;
  } else if (Iq_proportional <= -58.0f) {
    Iq_proportional = -58.0f;
  }

  rtb_Sum_f = (Iq_proportional + FOC_DW.Integrator_DSTATE_h) + rtb_IqTable;

  /* Switch: '<S82>/Switch2' incorporates:
   *  Inport: '<Root>/MTPA_PID'
   *  RelationalOperator: '<S82>/LowerRelop1'
   *  RelationalOperator: '<S82>/UpperRelop'
   *  Switch: '<S82>/Switch'
   */
  if (rtb_Sum_f > FOC_U.MTPA_PID.Up_Limit_torque_PID) {
    /* Switch: '<S82>/Switch2' */
    FOC_Y.Vq_PID = FOC_U.MTPA_PID.Up_Limit_torque_PID;
  } else if (rtb_Sum_f < FOC_U.MTPA_PID.Low_Limit_torque_PID) {
    /* Switch: '<S82>/Switch2' incorporates:
     *  Switch: '<S82>/Switch'
     */
    FOC_Y.Vq_PID = FOC_U.MTPA_PID.Low_Limit_torque_PID;
  } else {
    /* Switch: '<S82>/Switch2' incorporates:
     *  Switch: '<S82>/Switch'
     */
    FOC_Y.Vq_PID = rtb_Sum_f;
  }

  /* End of Switch: '<S82>/Switch2' */

  /* Sum: '<S5>/Sum4' */
  if (align_flag && !is_thr_valid) {
    FOC_Y.Vq = 0.0f;
  } else {
    FOC_Y.Vq = FOC_Y.Vq_FF + FOC_Y.Vq_PID;
  }

  /* Switch: '<S31>/Switch2' incorporates:
   *  Inport: '<Root>/MTPA_PID'
   *  RelationalOperator: '<S31>/LowerRelop1'
   *  RelationalOperator: '<S31>/UpperRelop'
   *  Switch: '<S31>/Switch'
   */
  if (FOC_Y.Vq > FOC_U.MTPA_PID.Up_Limit_torque_PID) {
    /* Sum: '<S5>/Sum4' */
    FOC_Y.Vq = FOC_U.MTPA_PID.Up_Limit_torque_PID;
  } else if (FOC_Y.Vq < FOC_U.MTPA_PID.Low_Limit_torque_PID) {
    /* Sum: '<S5>/Sum4' incorporates:
     *  Switch: '<S31>/Switch'
     */
    FOC_Y.Vq = FOC_U.MTPA_PID.Low_Limit_torque_PID;
  }

  /* End of Switch: '<S31>/Switch2' */

  /* Math: '<S5>/V_total' */
  FOC_Y.V_total_calc = rt_hypotd_snf(FOC_Y.Vd, FOC_Y.Vq);

  /* Product: '<S5>/Scale' incorporates:
   *  Bias: '<S5>/Bias'
   *  Inport: '<Root>/V_limit'
   */
  FOC_Y.Vdq_Scale = FOC_U.V_limit / (FOC_Y.V_total_calc + 2.2204460492503131E-16);

  /* If: '<S5>/If' incorporates:
   *  Inport: '<Root>/V_limit'
   */
  if (FOC_Y.V_total_calc <= FOC_U.V_limit) {
    /* Outputs for IfAction SubSystem: '<S5>/If Action Subsystem' incorporates:
     *  ActionPort: '<S27>/Action Port'
     */
    /* SignalConversion generated from: '<S27>/Vd' incorporates:
     *  Merge: '<S5>/Merge'
     */
    FOC_B.Merge[0] = FOC_Y.Vd;

    /* SignalConversion generated from: '<S27>/Vq' incorporates:
     *  Merge: '<S5>/Merge'
     */
    FOC_B.Merge[1] = FOC_Y.Vq;

    L_g.V_limit_flag = 0;

    /* End of Outputs for SubSystem: '<S5>/If Action Subsystem' */
  } else if (FOC_Y.V_total_calc > FOC_U.V_limit) {
    /* Outputs for IfAction SubSystem: '<S5>/If Action Subsystem1' incorporates:
     *  ActionPort: '<S28>/Action Port'
     */
    /* Product: '<S28>/Vd_scaled' incorporates:
     *  Merge: '<S5>/Merge'
     */
    // FOC_B.Merge[0] = FOC_Y.Vd * FOC_Y.Vdq_Scale;
    FOC_B.Merge[0] = FOC_Y.Vd;
    /* Product: '<S28>/Vq_scaled' incorporates:
     *  Merge: '<S5>/Merge'
     */
    // FOC_B.Merge[1] = FOC_Y.Vdq_Scale * FOC_Y.Vq;
    FOC_B.Merge[1] = FOC_Y.Vq;
    L_g.V_limit_flag = 1;
    /* End of Outputs for SubSystem: '<S5>/If Action Subsystem1' */
  }

  /* End of If: '<S5>/If' */

  /* Outputs for Atomic SubSystem: '<S148>/Two inputs CRL' */
  /* Switch: '<S152>/Switch' incorporates:
   *  Product: '<S151>/dcos'
   *  Product: '<S151>/qsin'
   *  Sum: '<S151>/sum_alpha'
   */
  rtb_Switch_f_idx_0 = FOC_B.Merge[0] * rtb_TrigonometricFunction1_tmp -
    FOC_B.Merge[1] * rtb_Add1;

  /* Gain: '<S150>/one_by_two' incorporates:
   *  AlgorithmDescriptorDelegate generated from: '<S151>/a16'
   */
  rtb_add_c = 0.5 * rtb_Switch_f_idx_0;

  /* Gain: '<S150>/sqrt3_by_two' incorporates:
   *  Product: '<S151>/dsin'
   *  Product: '<S151>/qcos'
   *  Sum: '<S151>/sum_beta'
   */
  rtb_IProdOut_l = (FOC_B.Merge[1] * rtb_TrigonometricFunction1_tmp +
                    FOC_B.Merge[0] * rtb_Add1) * 0.8660254037844386;

  /* End of Outputs for SubSystem: '<S148>/Two inputs CRL' */

  /* Sum: '<S150>/add_b' */
  rtb_Add1 = rtb_IProdOut_l - rtb_add_c;

  /* Sum: '<S150>/add_c' */
  rtb_add_c = (0.0 - rtb_add_c) - rtb_IProdOut_l;

  /* Outputs for Atomic SubSystem: '<S148>/Two inputs CRL' */
  /* Gain: '<S156>/one_by_two' incorporates:
   *  AlgorithmDescriptorDelegate generated from: '<S151>/a16'
   *  MinMax: '<S156>/Max'
   *  MinMax: '<S156>/Min'
   *  Sum: '<S156>/Add'
   */
  rtb_IProdOut_l = (fmax(fmax(rtb_Switch_f_idx_0, rtb_Add1), rtb_add_c) + fmin
                    (fmin(rtb_Switch_f_idx_0, rtb_Add1), rtb_add_c)) * -0.5;

  /* MinMax: '<S3>/Max1' incorporates:
   *  AlgorithmDescriptorDelegate generated from: '<S151>/a16'
   *  Gain: '<S155>/Gain'
   *  Sum: '<S155>/Add1'
   *  Sum: '<S155>/Add2'
   *  Sum: '<S155>/Add3'
   */
  FOC_Y.Va = (rtb_Switch_f_idx_0 + rtb_IProdOut_l) * 1.1547005383792517;

  /* End of Outputs for SubSystem: '<S148>/Two inputs CRL' */
  FOC_Y.Vb = (rtb_Add1 + rtb_IProdOut_l) * 1.1547005383792517;
  FOC_Y.Vc = (rtb_IProdOut_l + rtb_add_c) * 1.1547005383792517;

  /* MinMax: '<S3>/Max2' incorporates:
   *  Inport: '<Root>/MCTemperature_C'
   *  Inport: '<Root>/MotorTemperature_C'
   */
  rtb_algDD_o2_o = fmaxf(FOC_U.MCTemperature_C, FOC_U.MotorTemperature_C);

  /* MinMax: '<S3>/Max' incorporates:
   *  Inport: '<Root>/Phase Current'
   */
  rtb_IProdOut_l = fmax(fmax(FOC_U.PhaseCurrent[0], FOC_U.PhaseCurrent[1]),
                        FOC_U.PhaseCurrent[2]);

  /* Chart: '<S3>/Protection_States' incorporates:
   *  Inport: '<Root>/BusVoltage_V'
   */
  if (FOC_DW.is_active_c7_FOC == 0) {
    FOC_DW.is_active_c7_FOC = 1U;
    FOC_DW.durationCounter_1 = 0U;
    FOC_DW.is_TemperatureProtection = FOC_IN_TempSafe;

    /* Outport: '<Root>/TempFlag' */
    FOC_Y.TempFlag = SafeTemperature;
    FOC_DW.durationCounter_1_h = 0U;
    FOC_DW.is_VoltageProtection = FOC_IN_VoltageSafe;

    /* Outport: '<Root>/VoltageFlag' */
    FOC_Y.VoltageFlag = SafeVoltage;
    FOC_DW.durationCounter_1_i = 0U;
    FOC_DW.is_CurrentProtection = FOC_IN_CurrentSafe;

    /* Outport: '<Root>/CurrentFlag' */
    FOC_Y.CurrentFlag = SafeCurrent;
  } else {
    switch (FOC_DW.is_TemperatureProtection) {
     case FOC_IN_HighTempError:
      /* Outport: '<Root>/TempFlag' */
      FOC_Y.TempFlag = OT_Error;
      if (!(rtb_algDD_o2_o < 80.0F)) {
        FOC_DW.durationCounter_1_c = 0U;
      }

      if (FOC_DW.durationCounter_1_c > 1000U) {
        FOC_DW.durationCounter_1 = 0U;
        FOC_DW.is_TemperatureProtection = FOC_IN_TempSafe;

        /* Outport: '<Root>/TempFlag' */
        FOC_Y.TempFlag = SafeTemperature;
      } else {
        if (!(rtb_algDD_o2_o < 100.0F)) {
          FOC_DW.durationCounter_2 = 0U;
        }

        if (FOC_DW.durationCounter_2 > 1000U) {
          FOC_DW.durationCounter_2_e = 0U;
          FOC_DW.durationCounter_1_e = 0U;
          FOC_DW.is_TemperatureProtection = FOC_IN_HighTempWarning;

          /* Outport: '<Root>/TempFlag' */
          FOC_Y.TempFlag = OT_Warning;
        }
      }
      break;

     case FOC_IN_HighTempWarning:
      /* Outport: '<Root>/TempFlag' */
      FOC_Y.TempFlag = OT_Warning;
      if (!(rtb_algDD_o2_o > 100.0F)) {
        FOC_DW.durationCounter_2_e = 0U;
      }

      if (FOC_DW.durationCounter_2_e > 1000U) {
        FOC_DW.durationCounter_2 = 0U;
        FOC_DW.durationCounter_1_c = 0U;
        FOC_DW.is_TemperatureProtection = FOC_IN_HighTempError;

        /* Outport: '<Root>/TempFlag' */
        FOC_Y.TempFlag = OT_Error;
      } else {
        if (!(rtb_algDD_o2_o < 80.0F)) {
          FOC_DW.durationCounter_1_e = 0U;
        }

        if (FOC_DW.durationCounter_1_e > 1000U) {
          FOC_DW.durationCounter_1 = 0U;
          FOC_DW.is_TemperatureProtection = FOC_IN_TempSafe;

          /* Outport: '<Root>/TempFlag' */
          FOC_Y.TempFlag = SafeTemperature;
        }
      }
      break;

     default:
      /* Outport: '<Root>/TempFlag' */
      /* case IN_TempSafe: */
      FOC_Y.TempFlag = SafeTemperature;
      if (!(rtb_algDD_o2_o > 80.0F)) {
        FOC_DW.durationCounter_1 = 0U;
      }

      if (FOC_DW.durationCounter_1 > 1000U) {
        FOC_DW.durationCounter_2_e = 0U;
        FOC_DW.durationCounter_1_e = 0U;
        FOC_DW.is_TemperatureProtection = FOC_IN_HighTempWarning;

        /* Outport: '<Root>/TempFlag' */
        FOC_Y.TempFlag = OT_Warning;
      }
      break;
    }

    FOC_VoltageProtection();
    switch (FOC_DW.is_CurrentProtection) {
     case FOC_IN_CurrentSafe:
      /* Outport: '<Root>/CurrentFlag' */
      FOC_Y.CurrentFlag = SafeCurrent;
      if (!(rtb_IProdOut_l > 400.0)) {
        FOC_DW.durationCounter_1_i = 0U;
      }

      if (FOC_DW.durationCounter_1_i > 1000U) {
        FOC_DW.durationCounter_2_c = 0U;
        FOC_DW.durationCounter_1_d = 0U;
        FOC_DW.is_CurrentProtection = FOC_IN_OC_Warning;

        /* Outport: '<Root>/CurrentFlag' */
        FOC_Y.CurrentFlag = OC_Warning;
      }
      break;

     case FOC_IN_OC_Error:
      /* Outport: '<Root>/CurrentFlag' */
      FOC_Y.CurrentFlag = OC_Error;
      if (!(rtb_IProdOut_l < 450.0)) {
        FOC_DW.durationCounter_1_a = 0U;
      }

      if (FOC_DW.durationCounter_1_a > 1000U) {
        FOC_DW.durationCounter_1_i = 0U;
        FOC_DW.is_CurrentProtection = FOC_IN_CurrentSafe;

        /* Outport: '<Root>/CurrentFlag' */
        FOC_Y.CurrentFlag = SafeCurrent;
      } else {
        if (!(rtb_IProdOut_l < 450.0)) {
          FOC_DW.durationCounter_2_b = 0U;
        }

        if (FOC_DW.durationCounter_2_b > 1000U) {
          FOC_DW.durationCounter_2_c = 0U;
          FOC_DW.durationCounter_1_d = 0U;
          FOC_DW.is_CurrentProtection = FOC_IN_OC_Warning;

          /* Outport: '<Root>/CurrentFlag' */
          FOC_Y.CurrentFlag = OC_Warning;
        }
      }
      break;

     default:
      /* Outport: '<Root>/CurrentFlag' */
      /* case IN_OC_Warning: */
      FOC_Y.CurrentFlag = OC_Warning;
      if (!(rtb_IProdOut_l > 450.0)) {
        FOC_DW.durationCounter_2_c = 0U;
      }

      if (FOC_DW.durationCounter_2_c > 1000U) {
        FOC_DW.durationCounter_2_b = 0U;
        FOC_DW.durationCounter_1_a = 0U;
        FOC_DW.is_CurrentProtection = FOC_IN_OC_Error;

        /* Outport: '<Root>/CurrentFlag' */
        FOC_Y.CurrentFlag = OC_Error;
      } else {
        if (!(rtb_IProdOut_l < 450.0)) {
          FOC_DW.durationCounter_1_d = 0U;
        }

        if (FOC_DW.durationCounter_1_d > 1000U) {
          FOC_DW.durationCounter_1_i = 0U;
          FOC_DW.is_CurrentProtection = FOC_IN_CurrentSafe;

          /* Outport: '<Root>/CurrentFlag' */
          FOC_Y.CurrentFlag = SafeCurrent;
        }
      }
      break;
    }
  }

  if (rtb_algDD_o2_o > 80.0F) {
    FOC_DW.durationCounter_1++;
  } else {
    FOC_DW.durationCounter_1 = 0U;
  }

  if (rtb_algDD_o2_o < 80.0F) {
    FOC_DW.durationCounter_1_c++;
    FOC_DW.durationCounter_1_e++;
  } else {
    FOC_DW.durationCounter_1_c = 0U;
    FOC_DW.durationCounter_1_e = 0U;
  }

  if (rtb_algDD_o2_o < 100.0F) {
    FOC_DW.durationCounter_2++;
  } else {
    FOC_DW.durationCounter_2 = 0U;
  }

  if (rtb_algDD_o2_o > 100.0F) {
    FOC_DW.durationCounter_2_e++;
  } else {
    FOC_DW.durationCounter_2_e = 0U;
  }

  if (rtb_IProdOut_l > 450.0) {
    FOC_DW.durationCounter_1_i++;
  } else {
    FOC_DW.durationCounter_1_i = 0U;
  }

  if (rtb_IProdOut_l < 450.0) {
    FOC_DW.durationCounter_1_a++;
    FOC_DW.durationCounter_1_d++;
  } else {
    FOC_DW.durationCounter_1_a = 0U;
    FOC_DW.durationCounter_1_d = 0U;
  }

  if (rtb_IProdOut_l < 450.0) {
    FOC_DW.durationCounter_2_b++;
  } else {
    FOC_DW.durationCounter_2_b = 0U;
  }

  if (rtb_IProdOut_l > 450.0) {
    FOC_DW.durationCounter_2_c++;
  } else {
    FOC_DW.durationCounter_2_c = 0U;
  }

  if (FOC_U.BusVoltage_V > -80.0F) {
    FOC_DW.durationCounter_1_e3++;
  } else {
    FOC_DW.durationCounter_1_e3 = 0U;
  }

  if (FOC_U.BusVoltage_V < -80.0F) {
    FOC_DW.durationCounter_1_o++;
  } else {
    FOC_DW.durationCounter_1_o = 0U;
  }

  if (FOC_U.BusVoltage_V < -60.0F) {
    FOC_DW.durationCounter_1_h++;
  } else {
    FOC_DW.durationCounter_1_h = 0U;
  }

  if (FOC_U.BusVoltage_V > -60.0F) {
    FOC_DW.durationCounter_2_k++;
    FOC_DW.durationCounter_2_n++;
  } else {
    FOC_DW.durationCounter_2_k = 0U;
    FOC_DW.durationCounter_2_n = 0U;
  }

  FOC_DW.durationCounter_2_c0 = 0U;
  FOC_DW.durationCounter_1_m++;
  FOC_DW.durationCounter_1_f++;
  FOC_DW.durationCounter_2_d++;
  FOC_DW.durationCounter_2_bq = 0U;

  /* End of Chart: '<S3>/Protection_States' */

  /* Outport: '<Root>/Iq' incorporates:
   *  DataTypeConversion: '<S4>/Data Type Conversion1'
   *  UnitDelay: '<S24>/Unit Delay'
   */
  FOC_Y.Iq = FOC_DW.UnitDelay_DSTATE_f;

  /* Outport: '<Root>/Vd_scaled' */
  FOC_Y.Vd_scaled = FOC_B.Merge[0];

  /* Outport: '<Root>/Vq_scaled' */
  FOC_Y.Vq_scaled = FOC_B.Merge[1];

  /* Outport: '<Root>/Speed_error' incorporates:
   *  Gain: '<S1>/rpm to Rad//sec'
   *  Inport: '<Root>/Actual Speed_mech_rad//sec'
   *  Inport: '<Root>/Ref_Speed_mech_rpm'
   *  Sum: '<S12>/Subtract'
   */
  FOC_Y.Speed_error = 0.10471975511965977 * FOC_U.Ref_Speed_mech_rpm -
    FOC_U.MtrSpd;

  /* Update for Delay: '<S9>/Delay' */
  FOC_DW.icLoad = false;
  FOC_DW.Delay_DSTATE = FOC_Y.Torque_gen;

  /* Update for Delay: '<S158>/Delay' */
  FOC_DW.icLoad_a = false;
  FOC_DW.Delay_DSTATE_a = FOC_Y.Id_ref;

  /* Update for DiscreteIntegrator: '<S128>/Integrator' incorporates:
   *  Inport: '<Root>/MTPA_PID'
   *  Product: '<S125>/IProd Out'
   *  Sum: '<S120>/SumI2'
   *  Sum: '<S120>/SumI4'
   *  Sum: '<S140>/SumI3'
   *  Sum: '<S141>/SumI1'
   */
  FOC_DW.Integrator_DSTATE += ((FOC_Y.Id_error *
    FOC_U.MTPA_PID.Flux_PID_MTPA.Ki_flux_PID_MTPA + (FOC_Y.Vd_PID - FOC_Y.Vd_PID))
    + (FOC_Y.Vd_PID - rtb_Sum) * FOC_U.Flux_Kb) * 0.0001;
  if (FOC_DW.Integrator_DSTATE > 58.0) {
    FOC_DW.Integrator_DSTATE = 58.0;
  } else if (FOC_DW.Integrator_DSTATE < -58.0) {
    FOC_DW.Integrator_DSTATE = -58.0;
  }
  FOC_Y.Integrator_Flux_PID = FOC_DW.Integrator_DSTATE; 
  /* End of Update for DiscreteIntegrator: '<S128>/Integrator' */

  /* Update for DiscreteIntegrator: '<S123>/Filter' */
  FOC_DW.Filter_DSTATE += 0.0001 * rtb_Product;

  /* Update for UnitDelay: '<S24>/Unit Delay' */
  FOC_DW.UnitDelay_DSTATE_f = rtb_algDD_o1_d;

  /* Update for Delay: '<S159>/Delay' */
  FOC_DW.icLoad_h = false;
  FOC_DW.Delay_DSTATE_g = FOC_Y.Iq_ref;

  /* Update for DiscreteIntegrator: '<S74>/Integrator' incorporates:
   *  Gain: '<S86>/Kt'
   *  Inport: '<Root>/MTPA_PID'
   *  Product: '<S71>/IProd Out'
   *  Sum: '<S66>/SumI2'
   *  Sum: '<S66>/SumI4'
   *  Sum: '<S86>/SumI3'
   *  Sum: '<S87>/SumI1'
   */
  FOC_DW.Integrator_DSTATE_h += (((FOC_Y.Vq_PID - FOC_Y.Vq_PID) * -10.0 +
    FOC_Y.Iq_error * FOC_U.MTPA_PID.Torque_PID_MTPA.Ki_torque_PID_MTPA) +
    (FOC_Y.Vq_PID - rtb_Sum_f) * FOC_U.Torque_Kb) * 0.0001;
  if (FOC_DW.Integrator_DSTATE_h > 58.0) {
    FOC_DW.Integrator_DSTATE_h = 58.0;
  } else if (FOC_DW.Integrator_DSTATE_h < -58.0) {
    FOC_DW.Integrator_DSTATE_h = -58.0;
  }
  FOC_Y.Integrator_Torque_PID = FOC_DW.Integrator_DSTATE_h; 
  /* End of Update for DiscreteIntegrator: '<S74>/Integrator' */

  /* Update for DiscreteIntegrator: '<S69>/Filter' */
  FOC_DW.Filter_DSTATE_d += 0.0001 * rtb_IqTable;
}

/* Model step function for TID1 */
void FOC_step1(void)                   /* Sample time: [0.001s, 0.0s] */
{
  double rtb_NProdOut_j;
  double rtb_Sum;

  /* Product: '<S201>/NProd Out' incorporates:
   *  DiscreteIntegrator: '<S193>/Filter'
   *  Inport: '<Root>/MTPA_PID'
   *  Outport: '<Root>/Speed_error'
   *  Product: '<S191>/DProd Out'
   *  Sum: '<S193>/SumD'
   */
  rtb_NProdOut_j = (FOC_Y.Speed_error *
                    FOC_U.MTPA_PID.Speed_PID_MTPA.Kd_speed_PID_MTPA -
                    FOC_DW.Filter_DSTATE_i) *
    FOC_U.MTPA_PID.Speed_PID_MTPA.Filter_speed_PID_MTPA;

  /* Sum: '<S208>/Sum' incorporates:
   *  DiscreteIntegrator: '<S198>/Integrator'
   *  Inport: '<Root>/MTPA_PID'
   *  Outport: '<Root>/Speed_error'
   *  Product: '<S203>/PProd Out'
   */
  //PID Output: Speed Error * Kp + Integrator O/p + Derivative Output 
  rtb_Sum = (FOC_Y.Speed_error * FOC_U.MTPA_PID.Speed_PID_MTPA.Kp_speed_PID_MTPA
             + FOC_DW.Integrator_DSTATE_b) + rtb_NProdOut_j; 

  /* Switch: '<S206>/Switch2' incorporates:
   *  Inport: '<Root>/MTPA_PID'
   *  RelationalOperator: '<S206>/LowerRelop1'
   *  RelationalOperator: '<S206>/UpperRelop'
   *  Switch: '<S206>/Switch'
   */
  if (rtb_Sum > FOC_U.MTPA_PID.Up_Limit_speed_PID_MTPA) {
    /* Switch: '<S206>/Switch2' */
    FOC_B.Switch2 = FOC_U.MTPA_PID.Up_Limit_speed_PID_MTPA;
  } else if (rtb_Sum < FOC_U.MTPA_PID.Low_Limit_speed_PID_MTPA) {
    /* Switch: '<S206>/Switch' incorporates:
     *  Switch: '<S206>/Switch2'
     */
    FOC_B.Switch2 = FOC_U.MTPA_PID.Low_Limit_speed_PID_MTPA;
  } else {
    /* Switch: '<S206>/Switch2' incorporates:
     *  Switch: '<S206>/Switch'
     */
    FOC_B.Switch2 = rtb_Sum;
  }

  /* End of Switch: '<S206>/Switch2' */

  /* Outport: '<Root>/Iq_gen' */
  FOC_Y.Iq_gen = FOC_B.Switch2;

  /* Update for DiscreteIntegrator: '<S198>/Integrator' incorporates:
   *  Inport: '<Root>/MTPA_PID'
   *  Outport: '<Root>/Speed_error'
   *  Product: '<S195>/IProd Out'
   *  Sum: '<S190>/SumI2'
   *  Sum: '<S190>/SumI4'
   *  Sum: '<S210>/SumI3'
   *  Sum: '<S211>/SumI1'
   */
  FOC_DW.Integrator_DSTATE_b += ((FOC_Y.Speed_error *
    FOC_U.MTPA_PID.Speed_PID_MTPA.Ki_speed_PID_MTPA + (FOC_B.Switch2 -
    FOC_B.Switch2)) + (FOC_B.Switch2 - rtb_Sum) * FOC_U.Speed_Kb) * 0.001;
  if (FOC_DW.Integrator_DSTATE_b > 500.0) {
    FOC_DW.Integrator_DSTATE_b = 500.0;
  } else if (FOC_DW.Integrator_DSTATE_b < -500.0) {
    FOC_DW.Integrator_DSTATE_b = -500.0;
  }
  FOC_Y.Integrator_Speed_PID = FOC_DW.Integrator_DSTATE_b; 

  /* End of Update for DiscreteIntegrator: '<S198>/Integrator' */

  /* Update for DiscreteIntegrator: '<S193>/Filter' */
  FOC_DW.Filter_DSTATE_i += 0.001 * rtb_NProdOut_j;
}

/* Model initialize function */
void FOC_initialize(void)
{
  /* InitializeConditions for Delay: '<S9>/Delay' */
  FOC_DW.icLoad = true;

  /* InitializeConditions for Delay: '<S158>/Delay' */
  FOC_DW.icLoad_a = true;

  /* InitializeConditions for Delay: '<S159>/Delay' */
  FOC_DW.icLoad_h = true;

  // rtFOC_U.Lq = 0.000387;
  // rtFOC_U.Ld = 0.000147;
  // rtFOC_U.Lamda = 0.0263;
  // FOC_U.Rs = 0.0107;
  // rtFOC_U.p = POLEPAIRS;
  // FOC_U.Max_Throttle_pos = 4.2f;
  // FOC_U.Min_Throttle_pos = 1.2f;
  // FOC_U.Max_Speed_RPM = 6500.0f;
  // FOC_U.Cons_1_throttle_0 = 1;
  // FOC_U.I_max = 350.0f;

  /*              */
  FOC_U.MCTemperature_C = 38.0f;
  FOC_U.MotorTemperature_C = 40.0f;

  FOC_U.BusVoltage_V = 61.0f;
  FOC_U.V_limit = 61.0f;
  FOC_U.Iq_Torque_ratio = 0.15625f;

  FOC_U.Id_Iq_MTPA_limit.Id_low_limit = -75.0f;
  FOC_U.Id_Iq_MTPA_limit.Id_up_limit = 0.0f;
  FOC_U.Id_Iq_MTPA_limit.Iq_low_limit = 0.0f;
  FOC_U.Id_Iq_MTPA_limit.Iq_up_limit = 75.0f;

  /*              IIR Filter PARAMS                  */
  FOC_U.IIR_Filter_Coefficient.Id_Filter_coefficient = 0.88;
  FOC_U.IIR_Filter_Coefficient.Iq_Filter_coefficient = 0.88;
  FOC_U.IIR_Filter_Coefficient.Id_Filter_switch = 1.0;
  FOC_U.IIR_Filter_Coefficient.Iq_Filter_switch = 1.0;

  /*              Idq RATE LIMITER                    */
  FOC_U.Rate_limiter.Id_ramp_up = 50.0 * 10000.0f;
  FOC_U.Rate_limiter.Id_ramp_down = -50.0 * 10000.0f;
  FOC_U.Rate_limiter.Iq_ramp_up = 50.0 * 10000.0f;
  FOC_U.Rate_limiter.Iq_ramp_down = -50.0 * 10000.0f;
  FOC_U.Rate_limiter.Torque_gen_ramp_up = 50.0 * 10000.0f;
  FOC_U.Rate_limiter.Torque_gen_ramp_down = -50.0 * 10000.0f;

  /*              SPEED PI PARAMS                     */
  FOC_U.MTPA_PID.Speed_PID_MTPA.Kp_speed_PID_MTPA = 8.0;
  FOC_U.MTPA_PID.Speed_PID_MTPA.Ki_speed_PID_MTPA = 9.0;
  FOC_U.MTPA_PID.Speed_PID_MTPA.Kd_speed_PID_MTPA = 0.0001;
  FOC_U.MTPA_PID.Speed_PID_MTPA.Filter_speed_PID_MTPA = 5.0;

  FOC_U.MTPA_PID.Up_Limit_speed_PID_MTPA = 450.0f;
  FOC_U.MTPA_PID.Low_Limit_speed_PID_MTPA = 0.0f;

  /*              FLUX PI PARAMS                     */
  FOC_U.MTPA_PID.Flux_PID_MTPA.Kp_flux_PID_MTPA = 0.2;
  FOC_U.MTPA_PID.Flux_PID_MTPA.Ki_flux_PID_MTPA = 4.0;
  FOC_U.MTPA_PID.Flux_PID_MTPA.Kd_flux_PID_MTPA = 0.003;
  FOC_U.MTPA_PID.Flux_PID_MTPA.Filter_flux_PID_MTPA = 5.0;

  FOC_U.MTPA_PID.Up_Limit_flux_PID = (0.0f);
  FOC_U.MTPA_PID.Low_Limit_flux_PID = -(61.0f);

  /*              TORQUE PI PARAMS                     */
  FOC_U.MTPA_PID.Torque_PID_MTPA.Kp_torque_PID_MTPA = 0.2;
  FOC_U.MTPA_PID.Torque_PID_MTPA.Ki_torque_PID_MTPA = 4.5;
  FOC_U.MTPA_PID.Torque_PID_MTPA.Kd_torque_PID_MTPA = 0.003;
  FOC_U.MTPA_PID.Torque_PID_MTPA.Filter_torque_PID_MTPA = 5.0;

  FOC_U.MTPA_PID.Up_Limit_torque_PID = (61.0f);
  FOC_U.MTPA_PID.Low_Limit_torque_PID = (0.0f);

  FOC_U.Speed_Kb = 100.0f;
  FOC_U.Flux_Kb = 1000.0f;
  FOC_U.Torque_Kb = 1000.0f;

  curr_thr_state = NEUTRAL;
  prev_thr_state = DRIVE;
  thr_disabled = 1;

  if (FOC_F_T.Drive_off) {
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_SET);
  } else { 
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET);
  }

  // Send_on_CAN_308();
  // Send_on_CAN_309();
}

/* Model terminate function */
void FOC_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
