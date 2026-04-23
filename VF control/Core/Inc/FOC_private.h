/*
 * File: FOC_private.h
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

#ifndef FOC_private_h_
#define FOC_private_h_
#include <stdbool.h>
#include <stdint.h>
//#include "complex_types.h"
#include "FOC_types.h"
#include "FOC.h"

extern double rt_hypotd_snf(double u0, double u1);
extern double look2_pbinlx(double u0, double u1, const double bp0[], const
  double bp1[], const double table[], uint32_t prevIndex[], const uint32_t
  maxIndex[], uint32_t stride);

#endif                                 /* FOC_private_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
