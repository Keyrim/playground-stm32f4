/*
 * Primary and Secondary School License - for use in teaching and meeting
 * course requirements at primary and secondary schools only.  Not for
 * government, commercial, university, or other organizational use.
 *
 * File: kalman_linearised_continuous.c
 *
 * Code generated for Simulink model 'kalman_linearised_continuous'.
 *
 * Model version                  : 1.1
 * Simulink Coder version         : 9.4 (R2020b) 29-Jul-2020
 * C/C++ source code generated on : Fri Apr 23 03:23:20 2021
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives:
 *    1. Execution efficiency
 *    2. RAM efficiency
 * Validation result: Not run
 */

#include "kalman_linearised_continuous.h"

/* Private macros used by the generated code to access rtModel */
#ifndef rtmIsMajorTimeStep
#define rtmIsMajorTimeStep(rtm)        (((rtm)->Timing.simTimeStep) == MAJOR_TIME_STEP)
#endif

#ifndef rtmIsMinorTimeStep
#define rtmIsMinorTimeStep(rtm)        (((rtm)->Timing.simTimeStep) == MINOR_TIME_STEP)
#endif

#ifndef rtmSetTPtr
#define rtmSetTPtr(rtm, val)           ((rtm)->Timing.t = (val))
#endif

/* Continuous states */
X rtX;

/* Block signals and states (default storage) */
DW rtDW;

/* Real-time model */
static RT_MODEL rtM_;
RT_MODEL *const rtM = &rtM_;
extern real_T rt_urand_Upu32_Yd_f_pw_snf(uint32_T *u);
extern real_T rt_nrand_Upu32_Yd_f_pw_snf(uint32_T *u);

/* private model entry point functions */
extern void kalman_linearised_continuous_derivatives(void);

/*
 * This function updates continuous states using the ODE3 fixed-step
 * solver algorithm
 */
static void rt_ertODEUpdateContinuousStates(RTWSolverInfo *si )
{
  /* Solver Matrices */
  static const real_T rt_ODE3_A[3] = {
    1.0/2.0, 3.0/4.0, 1.0
  };

  static const real_T rt_ODE3_B[3][3] = {
    { 1.0/2.0, 0.0, 0.0 },

    { 0.0, 3.0/4.0, 0.0 },

    { 2.0/9.0, 1.0/3.0, 4.0/9.0 }
  };

  time_T t = rtsiGetT(si);
  time_T tnew = rtsiGetSolverStopTime(si);
  time_T h = rtsiGetStepSize(si);
  real_T *x = rtsiGetContStates(si);
  ODE3_IntgData *id = (ODE3_IntgData *)rtsiGetSolverData(si);
  real_T *y = id->y;
  real_T *f0 = id->f[0];
  real_T *f1 = id->f[1];
  real_T *f2 = id->f[2];
  real_T hB[3];
  int_T i;
  int_T nXc = 4;
  rtsiSetSimTimeStep(si,MINOR_TIME_STEP);

  /* Save the state values at time t in y, we'll use x as ynew. */
  (void) memcpy(y, x,
                (uint_T)nXc*sizeof(real_T));

  /* Assumes that rtsiSetT and ModelOutputs are up-to-date */
  /* f0 = f(t,y) */
  rtsiSetdX(si, f0);
  kalman_linearised_continuous_derivatives();

  /* f(:,2) = feval(odefile, t + hA(1), y + f*hB(:,1), args(:)(*)); */
  hB[0] = h * rt_ODE3_B[0][0];
  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (f0[i]*hB[0]);
  }

  rtsiSetT(si, t + h*rt_ODE3_A[0]);
  rtsiSetdX(si, f1);
  kalman_linearised_continuous_step();
  kalman_linearised_continuous_derivatives();

  /* f(:,3) = feval(odefile, t + hA(2), y + f*hB(:,2), args(:)(*)); */
  for (i = 0; i <= 1; i++) {
    hB[i] = h * rt_ODE3_B[1][i];
  }

  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (f0[i]*hB[0] + f1[i]*hB[1]);
  }

  rtsiSetT(si, t + h*rt_ODE3_A[1]);
  rtsiSetdX(si, f2);
  kalman_linearised_continuous_step();
  kalman_linearised_continuous_derivatives();

  /* tnew = t + hA(3);
     ynew = y + f*hB(:,3); */
  for (i = 0; i <= 2; i++) {
    hB[i] = h * rt_ODE3_B[2][i];
  }

  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (f0[i]*hB[0] + f1[i]*hB[1] + f2[i]*hB[2]);
  }

  rtsiSetT(si, tnew);
  rtsiSetSimTimeStep(si,MAJOR_TIME_STEP);
}

real_T rt_urand_Upu32_Yd_f_pw_snf(uint32_T *u)
{
  uint32_T hi;
  uint32_T lo;

  /* Uniform random number generator (random number between 0 and 1)

     #define IA      16807                      magic multiplier = 7^5
     #define IM      2147483647                 modulus = 2^31-1
     #define IQ      127773                     IM div IA
     #define IR      2836                       IM modulo IA
     #define S       4.656612875245797e-10      reciprocal of 2^31-1
     test = IA * (seed % IQ) - IR * (seed/IQ)
     seed = test < 0 ? (test + IM) : test
     return (seed*S)
   */
  lo = *u % 127773U * 16807U;
  hi = *u / 127773U * 2836U;
  if (lo < hi) {
    *u = 2147483647U - (hi - lo);
  } else {
    *u = lo - hi;
  }

  return (real_T)*u * 4.6566128752457969E-10;
}

real_T rt_nrand_Upu32_Yd_f_pw_snf(uint32_T *u)
{
  real_T si;
  real_T sr;
  real_T y;

  /* Normal (Gaussian) random number generator */
  do {
    sr = 2.0 * rt_urand_Upu32_Yd_f_pw_snf(u) - 1.0;
    si = 2.0 * rt_urand_Upu32_Yd_f_pw_snf(u) - 1.0;
    si = sr * sr + si * si;
  } while (si > 1.0);

  y = sqrt(-2.0 * log(si) / si) * sr;
  return y;
}

/* Model step function */
void kalman_linearised_continuous_step(void)
{
  if (rtmIsMajorTimeStep(rtM)) {
    /* set solver stop time */
    rtsiSetSolverStopTime(&rtM->solverInfo,((rtM->Timing.clockTick0+1)*
      rtM->Timing.stepSize0));
  }                                    /* end MajorTimeStep */

  /* Update absolute time of base rate at minor time step */
  if (rtmIsMinorTimeStep(rtM)) {
    rtM->Timing.t[0] = rtsiGetT(&rtM->solverInfo);
  }

  /* Step: '<Root>/Impulse' */
  if (rtM->Timing.t[0] < 1.0) {
    /* Step: '<Root>/Impulse' */
    rtDW.Impulse = 0.0;
  } else {
    /* Step: '<Root>/Impulse' */
    rtDW.Impulse = 20.0;
  }

  /* End of Step: '<Root>/Impulse' */

  /* SignalConversion generated from: '<Root>/Vector Concatenate' */
  rtDW.VectorConcatenate[0] = rtDW.Impulse;
  if (rtmIsMajorTimeStep(rtM)) {
    /* RandomNumber: '<Root>/Measurements noise' */
    rtDW.VectorConcatenate[3] = rtDW.NextOutput[0];

    /* RandomNumber: '<Root>/System Disturbances' */
    rtDW.VectorConcatenate[1] = rtDW.NextOutput_p[0];

    /* RandomNumber: '<Root>/Measurements noise' */
    rtDW.VectorConcatenate[4] = rtDW.NextOutput[1];

    /* RandomNumber: '<Root>/System Disturbances' */
    rtDW.VectorConcatenate[2] = rtDW.NextOutput_p[1];
  }

  if (rtmIsMajorTimeStep(rtM)) {
    if (rtmIsMajorTimeStep(rtM)) {
      /* Update for RandomNumber: '<Root>/Measurements noise' */
      rtDW.NextOutput[0] = rt_nrand_Upu32_Yd_f_pw_snf(&rtDW.RandSeed[0]);

      /* Update for RandomNumber: '<Root>/System Disturbances' */
      rtDW.NextOutput_p[0] = rt_nrand_Upu32_Yd_f_pw_snf(&rtDW.RandSeed_f[0]) *
        0.1;

      /* Update for RandomNumber: '<Root>/Measurements noise' */
      rtDW.NextOutput[1] = rt_nrand_Upu32_Yd_f_pw_snf(&rtDW.RandSeed[1]) * 0.1;

      /* Update for RandomNumber: '<Root>/System Disturbances' */
      rtDW.NextOutput_p[1] = rt_nrand_Upu32_Yd_f_pw_snf(&rtDW.RandSeed_f[1]) *
        0.1;
    }
  }                                    /* end MajorTimeStep */

  if (rtmIsMajorTimeStep(rtM)) {
    rt_ertODEUpdateContinuousStates(&rtM->solverInfo);

    /* Update absolute time for base rate */
    /* The "clockTick0" counts the number of times the code of this task has
     * been executed. The absolute time is the multiplication of "clockTick0"
     * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
     * overflow during the application lifespan selected.
     */
    ++rtM->Timing.clockTick0;
    rtM->Timing.t[0] = rtsiGetSolverStopTime(&rtM->solverInfo);

    {
      /* Update absolute timer for sample time: [0.01s, 0.0s] */
      /* The "clockTick1" counts the number of times the code of this task has
       * been executed. The resolution of this integer timer is 0.01, which is the step size
       * of the task. Size of "clockTick1" ensures timer will not overflow during the
       * application lifespan selected.
       */
      rtM->Timing.clockTick1++;
    }
  }                                    /* end MajorTimeStep */
}

/* Derivatives for root system: '<Root>' */
void kalman_linearised_continuous_derivatives(void)
{
  XDot *_rtXdot;
  _rtXdot = ((XDot *) rtM->derivs);

  /* Derivatives for StateSpace: '<Root>/Perfect Model' */
  _rtXdot->perfectModel[0] = 0.0;

  /* Derivatives for StateSpace: '<Root>/Real Model' */
  _rtXdot->RealModel_CSTATE[0] = 0.0;

  /* Derivatives for StateSpace: '<Root>/Perfect Model' */
  _rtXdot->perfectModel[1] = 0.0;

  /* Derivatives for StateSpace: '<Root>/Real Model' */
  _rtXdot->RealModel_CSTATE[1] = 0.0;

  /* Derivatives for StateSpace: '<Root>/Perfect Model' */
  _rtXdot->perfectModel[0] += rtX.perfectModel[1];
  _rtXdot->perfectModel[1] += -54.500000000000014 * rtX.perfectModel[0];
  _rtXdot->perfectModel[1] += -0.058333333333333341 * rtX.perfectModel[1];
  _rtXdot->perfectModel[1] += 3.3333333333333335 * rtDW.Impulse;

  /* Derivatives for StateSpace: '<Root>/Real Model' */
  _rtXdot->RealModel_CSTATE[0] += rtX.RealModel_CSTATE[1];
  _rtXdot->RealModel_CSTATE[1] += -54.500000000000014 * rtX.RealModel_CSTATE[0];
  _rtXdot->RealModel_CSTATE[1] += -0.058333333333333341 * rtX.RealModel_CSTATE[1];
  _rtXdot->RealModel_CSTATE[0] += 0.01 * rtDW.VectorConcatenate[1];
  _rtXdot->RealModel_CSTATE[1] += 3.3333333333333335 * rtDW.VectorConcatenate[0];
  _rtXdot->RealModel_CSTATE[1] += 0.01 * rtDW.VectorConcatenate[2];
}

/* Model initialize function */
void kalman_linearised_continuous_initialize(void)
{
  /* Registration code */
  {
    /* Setup solver object */
    rtsiSetSimTimeStepPtr(&rtM->solverInfo, &rtM->Timing.simTimeStep);
    rtsiSetTPtr(&rtM->solverInfo, &rtmGetTPtr(rtM));
    rtsiSetStepSizePtr(&rtM->solverInfo, &rtM->Timing.stepSize0);
    rtsiSetdXPtr(&rtM->solverInfo, &rtM->derivs);
    rtsiSetContStatesPtr(&rtM->solverInfo, (real_T **) &rtM->contStates);
    rtsiSetNumContStatesPtr(&rtM->solverInfo, &rtM->Sizes.numContStates);
    rtsiSetNumPeriodicContStatesPtr(&rtM->solverInfo,
      &rtM->Sizes.numPeriodicContStates);
    rtsiSetPeriodicContStateIndicesPtr(&rtM->solverInfo,
      &rtM->periodicContStateIndices);
    rtsiSetPeriodicContStateRangesPtr(&rtM->solverInfo,
      &rtM->periodicContStateRanges);
    rtsiSetErrorStatusPtr(&rtM->solverInfo, (&rtmGetErrorStatus(rtM)));
    rtsiSetRTModelPtr(&rtM->solverInfo, rtM);
  }

  rtsiSetSimTimeStep(&rtM->solverInfo, MAJOR_TIME_STEP);
  rtM->intgData.y = rtM->odeY;
  rtM->intgData.f[0] = rtM->odeF[0];
  rtM->intgData.f[1] = rtM->odeF[1];
  rtM->intgData.f[2] = rtM->odeF[2];
  rtM->contStates = ((X *) &rtX);
  rtsiSetSolverData(&rtM->solverInfo, (void *)&rtM->intgData);
  rtsiSetSolverName(&rtM->solverInfo,"ode3");
  rtmSetTPtr(rtM, &rtM->Timing.tArray[0]);
  rtM->Timing.stepSize0 = 0.01;

  /* InitializeConditions for StateSpace: '<Root>/Perfect Model' */
  rtX.perfectModel[0] = 0.0;

  /* InitializeConditions for RandomNumber: '<Root>/Measurements noise' */
  rtDW.RandSeed[0] = 1144108930U;
  rtDW.NextOutput[0] = rt_nrand_Upu32_Yd_f_pw_snf(&rtDW.RandSeed[0]);

  /* InitializeConditions for StateSpace: '<Root>/Perfect Model' */
  rtX.perfectModel[1] = -0.0;

  /* InitializeConditions for RandomNumber: '<Root>/Measurements noise' */
  rtDW.RandSeed[1] = 65536U;
  rtDW.NextOutput[1] = rt_nrand_Upu32_Yd_f_pw_snf(&rtDW.RandSeed[1]) * 0.1;

  /* InitializeConditions for RandomNumber: '<Root>/System Disturbances' */
  rtDW.RandSeed_f[0] = 1144108930U;
  rtDW.NextOutput_p[0] = rt_nrand_Upu32_Yd_f_pw_snf(&rtDW.RandSeed_f[0]) * 0.1;

  /* InitializeConditions for StateSpace: '<Root>/Real Model' */
  rtX.RealModel_CSTATE[0] = 0.0;

  /* InitializeConditions for RandomNumber: '<Root>/System Disturbances' */
  rtDW.RandSeed_f[1] = 65536U;
  rtDW.NextOutput_p[1] = rt_nrand_Upu32_Yd_f_pw_snf(&rtDW.RandSeed_f[1]) * 0.1;

  /* InitializeConditions for StateSpace: '<Root>/Real Model' */
  rtX.RealModel_CSTATE[1] = 0.0;
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
