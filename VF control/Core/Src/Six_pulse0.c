/*
 * File: Six_pulse0.c
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

#include "Six_pulse0.h"
#include <stdint.h>
#include <math.h>
#include "Six_pulse0_private.h"
#include <stdbool.h>
#include "zero_crossing_types.h"
#include "rt_nonfinite.h"

/* Named constants for Chart: '<S1>/Chart' */
#define Six_pulse0_IN_Init_            ((uint8_t)1U)
#define Six_pulse0_IN_NO_ACTIVE_CHILD  ((uint8_t)0U)
#define Six_pulse0_IN_calc             ((uint8_t)2U)
#define Six_pulse0_IN_stop             ((uint8_t)3U)
#define Six_pulse0_IN_trig             ((uint8_t)1U)
#define Six_pulse0_IN_trig_1           ((uint8_t)2U)

extern TIM_HandleTypeDef htim2; // Encoder Timer

/* Block signals (default storage) */
B_Six_pulse0_T Six_pulse0_B;

/* Block states (default storage) */
DW_Six_pulse0_T Six_pulse0_DW;

/* Previous zero-crossings (trigger) states */
PrevZCX_Six_pulse0_T Six_pulse0_PrevZCX;

/* External inputs (root inport signals with default storage) */
ExtU_Six_pulse0_T Six_pulse0_U;

/* External outputs (root outports fed by signals with default storage) */
ExtY_Six_pulse0_T Six_pulse0_Y;

/* Real-time model */
static RT_MODEL_Six_pulse0_T Six_pulse0_M_;
RT_MODEL_Six_pulse0_T *const Six_pulse0_M = &Six_pulse0_M_;
double rt_hypotd_snf_six_step(double u0, double u1)
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

/* Model step function */
void Six_pulse0_step(void)
{
  double rtb_TrigonometricFunction;
  double rtb_TrigonometricFunction1;
  double rtb_sum_Ds;
  int32_t rtb_trig;
  bool rtb_RelationalOperator;
  ZCEventType zcEvent;

  /* Chart: '<S1>/Chart' incorporates:
   *  Inport: '<Root>/angle_diff'
   *  Inport: '<Root>/volt_in'
   *  Outport: '<Root>/angle'
   */
  if (Six_pulse0_DW.temporalCounter_i1 < 511) {
    Six_pulse0_DW.temporalCounter_i1++;
  }

  if (Six_pulse0_DW.temporalCounter_i2 < 511) {
    Six_pulse0_DW.temporalCounter_i2++;
  }

  if (Six_pulse0_DW.is_active_c3_Six_pulse0 == 0) {
    Six_pulse0_DW.is_active_c3_Six_pulse0 = 1U;
    Six_pulse0_DW.temporalCounter_i2 = 0U;
    Six_pulse0_DW.is_c3_Six_pulse0 = Six_pulse0_IN_Init_;
    Six_pulse0_B.Volt = Six_pulse0_U.volt_in;

    /* Outport: '<Root>/angle' incorporates:
     *  Inport: '<Root>/volt_in'
     */
    Six_pulse0_Y.angle = 0.0;

    /* Outport: '<Root>/last' */
    Six_pulse0_Y.last = 0.0;
    Six_pulse0_DW.temporalCounter_i1 = 0U;
    Six_pulse0_DW.is_Init_ = Six_pulse0_IN_trig;
    rtb_trig = 0;
  } else {
    switch (Six_pulse0_DW.is_c3_Six_pulse0) {
     case Six_pulse0_IN_Init_:
      if (Six_pulse0_DW.temporalCounter_i2 >= 500) {
        Six_pulse0_DW.is_Init_ = Six_pulse0_IN_NO_ACTIVE_CHILD;
        Six_pulse0_DW.temporalCounter_i2 = 0U;
        Six_pulse0_DW.is_c3_Six_pulse0 = Six_pulse0_IN_calc;

        /* Outport: '<Root>/angle' incorporates:
         *  Inport: '<Root>/angle_diff'
         */
        Six_pulse0_Y.angle -= Six_pulse0_U.angle_diff;
        Six_pulse0_DW.temporalCounter_i1 = 0U;
        Six_pulse0_DW.is_calc = Six_pulse0_IN_trig;
        rtb_trig = 0;
      } else {
        Six_pulse0_B.Volt = Six_pulse0_U.volt_in;

        /* Outport: '<Root>/angle' incorporates:
         *  Inport: '<Root>/volt_in'
         */
        Six_pulse0_Y.angle = 0.0;

        /* Outport: '<Root>/last' */
        Six_pulse0_Y.last = 0.0;
        if (Six_pulse0_DW.is_Init_ == Six_pulse0_IN_trig) {
          if (Six_pulse0_DW.temporalCounter_i1 >= 400) {
            Six_pulse0_DW.is_Init_ = Six_pulse0_IN_trig_1;
            rtb_trig = 1;
          } else {
            rtb_trig = 0;
          }
        } else {
          /* case IN_trig_1: */
          rtb_trig = 1;
        }
      }
      break;

     case Six_pulse0_IN_calc:
      if (Six_pulse0_Y.angle < -(360.0 - Six_pulse0_U.angle_diff)) {
        Six_pulse0_DW.is_calc = Six_pulse0_IN_NO_ACTIVE_CHILD;
        Six_pulse0_DW.is_c3_Six_pulse0 = Six_pulse0_IN_stop;
        Six_pulse0_B.Volt = 0.0;
        Six_pulse0_Y.angle = 0.0;
        rtb_trig = 0;

        /* Outport: '<Root>/last' */
        Six_pulse0_Y.last = 1.0;
      } else if (Six_pulse0_DW.temporalCounter_i2 >= 500) {
        Six_pulse0_DW.temporalCounter_i2 = 0U;
        Six_pulse0_DW.is_c3_Six_pulse0 = Six_pulse0_IN_calc;
        Six_pulse0_Y.angle -= Six_pulse0_U.angle_diff;
        Six_pulse0_DW.temporalCounter_i1 = 0U;
        Six_pulse0_DW.is_calc = Six_pulse0_IN_trig;
        rtb_trig = 0;
      } else if (Six_pulse0_DW.is_calc == Six_pulse0_IN_trig) {
        if (Six_pulse0_DW.temporalCounter_i1 >= 400) {
          Six_pulse0_DW.is_calc = Six_pulse0_IN_trig_1;
          rtb_trig = 1;
        } else {
          rtb_trig = 0;
        }
      } else {
        /* case IN_trig_1: */
        rtb_trig = 1;
      }
      break;

     default:
      /* case IN_stop: */
      Six_pulse0_B.Volt = 0.0;

      /* Outport: '<Root>/angle' */
      Six_pulse0_Y.angle = 0.0;
      rtb_trig = 0;

      /* Outport: '<Root>/last' */
      Six_pulse0_Y.last = 1.0;
      break;
    }
  }

  /* End of Chart: '<S1>/Chart' */

  /* Sum: '<S9>/Add1' incorporates:
   *  Constant: '<S3>/One'
   *  Inport: '<Root>/Ia'
   *  Inport: '<Root>/Ia_Filter_coefficient'
   *  Product: '<S9>/Product'
   *  Product: '<S9>/Product1'
   *  Sum: '<S3>/Sum'
   *  UnitDelay: '<S9>/Unit Delay'
   */
  Six_pulse0_DW.UnitDelay_DSTATE = (1.0 - Six_pulse0_U.Ia_Filter_coefficient) *
    Six_pulse0_DW.UnitDelay_DSTATE + Six_pulse0_U.Ia *
    Six_pulse0_U.Ia_Filter_coefficient;

  /* Sum: '<S12>/Add1' incorporates:
   *  Constant: '<S4>/One'
   *  Inport: '<Root>/Ib'
   *  Inport: '<Root>/Ib_Filter_coefficient'
   *  Product: '<S12>/Product'
   *  Product: '<S12>/Product1'
   *  Sum: '<S4>/Sum'
   *  UnitDelay: '<S12>/Unit Delay'
   */
  Six_pulse0_DW.UnitDelay_DSTATE_c = (1.0 - Six_pulse0_U.Ib_Filter_coefficient) *
    Six_pulse0_DW.UnitDelay_DSTATE_c + Six_pulse0_U.Ib *
    Six_pulse0_U.Ib_Filter_coefficient;

  /* Outputs for Triggered SubSystem: '<S1>/Triggered Subsystem' incorporates:
   *  TriggerPort: '<S5>/Trigger'
   */
  zcEvent = rt_ZCFcn(RISING_ZERO_CROSSING,
                     &Six_pulse0_PrevZCX.TriggeredSubsystem_Trig_ZCE,
                     ((double)rtb_trig));
  if (zcEvent != NO_ZCEVENT) {
    /* Delay: '<S5>/Delay' */
    Six_pulse0_B.Delay = Six_pulse0_DW.Delay_DSTATE_e;

    /* Delay: '<S5>/Delay1' */
    Six_pulse0_B.Delay1 = Six_pulse0_DW.Delay1_DSTATE[0];

    /* Delay: '<S5>/Delay2' */
    Six_pulse0_B.Delay2 = Six_pulse0_DW.Delay2_DSTATE[0];

    /* Delay: '<S5>/Delay3' */
    Six_pulse0_B.Delay3 = Six_pulse0_DW.Delay3_DSTATE[0];

    /* Delay: '<S5>/Delay4' */
    Six_pulse0_B.Delay4 = Six_pulse0_DW.Delay4_DSTATE[0];

    /* Delay: '<S5>/Delay5' */
    Six_pulse0_B.Delay5 = Six_pulse0_DW.Delay5_DSTATE;

    /* Delay: '<S5>/Delay6' */
    Six_pulse0_B.Delay6 = Six_pulse0_DW.Delay6_DSTATE[0];

    /* Delay: '<S5>/Delay7' */
    Six_pulse0_B.Delay7 = Six_pulse0_DW.Delay7_DSTATE[0];

    /* Delay: '<S5>/Delay8' */
    Six_pulse0_B.Delay8 = Six_pulse0_DW.Delay8_DSTATE[0];

    /* Delay: '<S5>/Delay9' */
    Six_pulse0_B.Delay9 = Six_pulse0_DW.Delay9_DSTATE[0];

    /* Outputs for Atomic SubSystem: '<S15>/Two phase CRL wrap' */
    /* Gain: '<S16>/one_by_sqrt3' incorporates:
     *  Sum: '<S16>/a_plus_2b'
     *  UnitDelay: '<S12>/Unit Delay'
     *  UnitDelay: '<S9>/Unit Delay'
     */
    rtb_sum_Ds = ((Six_pulse0_DW.UnitDelay_DSTATE +
                   Six_pulse0_DW.UnitDelay_DSTATE_c) +
                  Six_pulse0_DW.UnitDelay_DSTATE_c) * 0.57735026918962584;

    /* End of Outputs for SubSystem: '<S15>/Two phase CRL wrap' */

    /* SignalConversion generated from: '<S5>/angle' incorporates:
     *  Outport: '<Root>/angle'
     */
    Six_pulse0_B.angle_o = Six_pulse0_Y.angle;

    /* Trigonometry: '<S5>/Trigonometric Function' */
    rtb_TrigonometricFunction = sin(Six_pulse0_B.angle_o);

    /* Trigonometry: '<S5>/Trigonometric Function1' */
    rtb_TrigonometricFunction1 = cos(Six_pulse0_B.angle_o);

    /* Switch: '<S5>/Switch' incorporates:
     *  AlgorithmDescriptorDelegate generated from: '<S16>/a16'
     *  Delay: '<S5>/Delay5'
     *  Inport: '<Root>/I_sw_1_normal_0_park'
     *  Math: '<S5>/Hypot'
     *  Math: '<S5>/Hypot1'
     *  Product: '<S17>/acos'
     *  Product: '<S17>/asin'
     *  Product: '<S17>/bcos'
     *  Product: '<S17>/bsin'
     *  Sum: '<S17>/sum_Ds'
     *  Sum: '<S17>/sum_Qs'
     *  UnitDelay: '<S12>/Unit Delay'
     *  UnitDelay: '<S9>/Unit Delay'
     */
    if (Six_pulse0_U.I_sw_1_normal_0_park > 0.0) {
      Six_pulse0_DW.Delay5_DSTATE = rt_hypotd_snf_six_step(Six_pulse0_DW.UnitDelay_DSTATE,
        Six_pulse0_DW.UnitDelay_DSTATE_c);
    } else {
      /* Outputs for Atomic SubSystem: '<S14>/Two inputs CRL' */
      /* Outputs for Atomic SubSystem: '<S15>/Two phase CRL wrap' */
      Six_pulse0_DW.Delay5_DSTATE = rt_hypotd_snf_six_step(Six_pulse0_DW.UnitDelay_DSTATE
        * rtb_TrigonometricFunction1 + rtb_sum_Ds * rtb_TrigonometricFunction,
        rtb_sum_Ds * rtb_TrigonometricFunction1 - Six_pulse0_DW.UnitDelay_DSTATE
        * rtb_TrigonometricFunction);

      /* End of Outputs for SubSystem: '<S15>/Two phase CRL wrap' */
      /* End of Outputs for SubSystem: '<S14>/Two inputs CRL' */
    }

    /* End of Switch: '<S5>/Switch' */

    /* SignalConversion generated from: '<S5>/current' incorporates:
     *  Delay: '<S5>/Delay5'
     */
    Six_pulse0_B.OutportBufferForcurrent = Six_pulse0_DW.Delay5_DSTATE;

    /* SignalConversion generated from: '<S5>/angle_1' */
    Six_pulse0_B.OutportBufferForangle_1 = Six_pulse0_B.angle_o;

    /* Update for Delay: '<S5>/Delay' */
    Six_pulse0_DW.Delay_DSTATE_e = Six_pulse0_B.angle_o;

    /* Update for Delay: '<S5>/Delay1' */
    Six_pulse0_DW.Delay1_DSTATE[0] = Six_pulse0_DW.Delay1_DSTATE[1];
    Six_pulse0_DW.Delay1_DSTATE[1] = Six_pulse0_B.angle_o;

    /* Update for Delay: '<S5>/Delay2' */
    Six_pulse0_DW.Delay2_DSTATE[0] = Six_pulse0_DW.Delay2_DSTATE[1];
    Six_pulse0_DW.Delay2_DSTATE[1] = Six_pulse0_DW.Delay2_DSTATE[2];
    Six_pulse0_DW.Delay2_DSTATE[2] = Six_pulse0_B.angle_o;

    /* Update for Delay: '<S5>/Delay3' */
    Six_pulse0_DW.Delay3_DSTATE[0] = Six_pulse0_DW.Delay3_DSTATE[1];
    Six_pulse0_DW.Delay3_DSTATE[1] = Six_pulse0_DW.Delay3_DSTATE[2];
    Six_pulse0_DW.Delay3_DSTATE[2] = Six_pulse0_DW.Delay3_DSTATE[3];
    Six_pulse0_DW.Delay3_DSTATE[3] = Six_pulse0_B.angle_o;

    /* Update for Delay: '<S5>/Delay4' */
    Six_pulse0_DW.Delay4_DSTATE[0] = Six_pulse0_DW.Delay4_DSTATE[1];
    Six_pulse0_DW.Delay4_DSTATE[1] = Six_pulse0_DW.Delay4_DSTATE[2];
    Six_pulse0_DW.Delay4_DSTATE[2] = Six_pulse0_DW.Delay4_DSTATE[3];
    Six_pulse0_DW.Delay4_DSTATE[3] = Six_pulse0_DW.Delay4_DSTATE[4];
    Six_pulse0_DW.Delay4_DSTATE[4] = Six_pulse0_B.angle_o;

    /* Update for Delay: '<S5>/Delay6' incorporates:
     *  Delay: '<S5>/Delay5'
     */
    Six_pulse0_DW.Delay6_DSTATE[0] = Six_pulse0_DW.Delay6_DSTATE[1];
    Six_pulse0_DW.Delay6_DSTATE[1] = Six_pulse0_DW.Delay5_DSTATE;

    /* Update for Delay: '<S5>/Delay7' incorporates:
     *  Delay: '<S5>/Delay5'
     */
    Six_pulse0_DW.Delay7_DSTATE[0] = Six_pulse0_DW.Delay7_DSTATE[1];
    Six_pulse0_DW.Delay7_DSTATE[1] = Six_pulse0_DW.Delay7_DSTATE[2];
    Six_pulse0_DW.Delay7_DSTATE[2] = Six_pulse0_DW.Delay5_DSTATE;

    /* Update for Delay: '<S5>/Delay8' incorporates:
     *  Delay: '<S5>/Delay5'
     */
    Six_pulse0_DW.Delay8_DSTATE[0] = Six_pulse0_DW.Delay8_DSTATE[1];
    Six_pulse0_DW.Delay8_DSTATE[1] = Six_pulse0_DW.Delay8_DSTATE[2];
    Six_pulse0_DW.Delay8_DSTATE[2] = Six_pulse0_DW.Delay8_DSTATE[3];
    Six_pulse0_DW.Delay8_DSTATE[3] = Six_pulse0_DW.Delay5_DSTATE;

    /* Update for Delay: '<S5>/Delay9' incorporates:
     *  Delay: '<S5>/Delay5'
     */
    Six_pulse0_DW.Delay9_DSTATE[0] = Six_pulse0_DW.Delay9_DSTATE[1];
    Six_pulse0_DW.Delay9_DSTATE[1] = Six_pulse0_DW.Delay9_DSTATE[2];
    Six_pulse0_DW.Delay9_DSTATE[2] = Six_pulse0_DW.Delay9_DSTATE[3];
    Six_pulse0_DW.Delay9_DSTATE[3] = Six_pulse0_DW.Delay9_DSTATE[4];
    Six_pulse0_DW.Delay9_DSTATE[4] = Six_pulse0_DW.Delay5_DSTATE;
  }

  /* End of Outputs for SubSystem: '<S1>/Triggered Subsystem' */

  /* Outport: '<Root>/angle_array' */
  Six_pulse0_Y.angle_array[0] = Six_pulse0_B.angle_o;
  Six_pulse0_Y.angle_array[1] = Six_pulse0_B.Delay;
  Six_pulse0_Y.angle_array[2] = Six_pulse0_B.Delay1;
  Six_pulse0_Y.angle_array[3] = Six_pulse0_B.Delay2;
  Six_pulse0_Y.angle_array[4] = Six_pulse0_B.Delay3;
  Six_pulse0_Y.angle_array[5] = Six_pulse0_B.Delay4;

  /* Outport: '<Root>/current_array' incorporates:
   *  Delay: '<S5>/Delay5'
   */
  Six_pulse0_Y.current_array[0] = Six_pulse0_DW.Delay5_DSTATE;
  Six_pulse0_Y.current_array[1] = Six_pulse0_B.Delay5;
  Six_pulse0_Y.current_array[2] = Six_pulse0_B.Delay6;
  Six_pulse0_Y.current_array[3] = Six_pulse0_B.Delay7;
  Six_pulse0_Y.current_array[4] = Six_pulse0_B.Delay8;
  Six_pulse0_Y.current_array[5] = Six_pulse0_B.Delay9;

  /* Outport: '<Root>/Va' incorporates:
   *  Outport: '<Root>/angle'
   *  Product: '<S1>/Product'
   *  SignalConversion generated from: '<S1>/Trigonometric Function2'
   *  Trigonometry: '<S1>/Trigonometric Function2'
   */
  Six_pulse0_Y.Va = Six_pulse0_B.Volt * sin(Six_pulse0_Y.angle);

  /* Outport: '<Root>/Vb' incorporates:
   *  Bias: '<S1>/Bias'
   *  Outport: '<Root>/angle'
   *  Product: '<S1>/Product'
   *  Trigonometry: '<S1>/Trigonometric Function2'
   */
  Six_pulse0_Y.Vb = sin(Six_pulse0_Y.angle - 120.0) * Six_pulse0_B.Volt;

  /* Outport: '<Root>/Vc' incorporates:
   *  Bias: '<S1>/Bias1'
   *  Outport: '<Root>/angle'
   *  Product: '<S1>/Product'
   *  Trigonometry: '<S1>/Trigonometric Function2'
   */
  Six_pulse0_Y.Vc = sin(Six_pulse0_Y.angle + 120.0) * Six_pulse0_B.Volt;

  /* Delay: '<S1>/Delay' */
  if (Six_pulse0_DW.icLoad) {
    Six_pulse0_DW.Delay_DSTATE = Six_pulse0_B.OutportBufferForcurrent;
  }

  /* RelationalOperator: '<S1>/Relational Operator' incorporates:
   *  Delay: '<S1>/Delay'
   */
  rtb_RelationalOperator = (Six_pulse0_B.OutportBufferForcurrent <=
    Six_pulse0_DW.Delay_DSTATE);

  /* Outputs for Triggered SubSystem: '<S1>/Triggered Subsystem1' incorporates:
   *  TriggerPort: '<S6>/Trigger'
   */
  if (rtb_RelationalOperator && (Six_pulse0_PrevZCX.TriggeredSubsystem1_Trig_ZCE
       != POS_ZCSIG)) {
    /* Outport: '<Root>/Current_min' incorporates:
     *  SignalConversion generated from: '<S6>/current'
     */
    Six_pulse0_Y.Current_min = Six_pulse0_B.OutportBufferForcurrent;

    /* Outport: '<Root>/angle_offset' incorporates:
     *  SignalConversion generated from: '<S6>/angle'
     */
    volatile float _enc_sensor_angle_elec = (((float)(__HAL_TIM_GET_COUNTER(&htim2)) / 4096.0f) * 360.0f) * POLEPAIRS; // Encoder mech angle in degrees
    _enc_sensor_angle_elec = fmodf(_enc_sensor_angle_elec, 360.0f);
    if (_enc_sensor_angle_elec < 0.0f) {
      _enc_sensor_angle_elec += 360.0f;
    }

    Six_pulse0_Y._enc_sensor_angle_elec = _enc_sensor_angle_elec;
    Six_pulse0_Y.angle_offset = Six_pulse0_B.OutportBufferForangle_1;
  }

  Six_pulse0_PrevZCX.TriggeredSubsystem1_Trig_ZCE = rtb_RelationalOperator;

  /* End of Outputs for SubSystem: '<S1>/Triggered Subsystem1' */

  /* Outport: '<Root>/trig' */
  Six_pulse0_Y.trig = rtb_trig;

  /* Update for Delay: '<S1>/Delay' */
  Six_pulse0_DW.icLoad = false;
  Six_pulse0_DW.Delay_DSTATE = Six_pulse0_B.OutportBufferForcurrent;
}

/* Model initialize function */
void Six_pulse0_initialize(void)
{
  Six_pulse0_PrevZCX.TriggeredSubsystem_Trig_ZCE = UNINITIALIZED_ZCSIG;
  Six_pulse0_PrevZCX.TriggeredSubsystem1_Trig_ZCE = POS_ZCSIG;

  /* InitializeConditions for Delay: '<S1>/Delay' */
  Six_pulse0_DW.icLoad = true;

  Six_pulse0_U.I_sw_1_normal_0_park = 0;
  Six_pulse0_U.Ia_Filter_coefficient = 0.2f;
  Six_pulse0_U.Ib_Filter_coefficient = 0.2f;
  Six_pulse0_U.volt_in = 5.0f;
  Six_pulse0_U.angle_diff = 1.0f;

  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET);
}

/* Model terminate function */
void Six_pulse0_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
