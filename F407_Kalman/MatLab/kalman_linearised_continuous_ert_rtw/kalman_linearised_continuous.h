/*
 * Primary and Secondary School License - for use in teaching and meeting
 * course requirements at primary and secondary schools only.  Not for
 * government, commercial, university, or other organizational use.
 *
 * File: kalman_linearised_continuous.h
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

#ifndef RTW_HEADER_kalman_linearised_continuous_h_
#define RTW_HEADER_kalman_linearised_continuous_h_
#include <math.h>
#include <string.h>
#ifndef kalman_linearised_continuous_COMMON_INCLUDES_
#define kalman_linearised_continuous_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#endif                       /* kalman_linearised_continuous_COMMON_INCLUDES_ */

/* Model Code Variants */

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

#ifndef rtmGetStopRequested
#define rtmGetStopRequested(rtm)       ((rtm)->Timing.stopRequestedFlag)
#endif

#ifndef rtmSetStopRequested
#define rtmSetStopRequested(rtm, val)  ((rtm)->Timing.stopRequestedFlag = (val))
#endif

#ifndef rtmGetStopRequestedPtr
#define rtmGetStopRequestedPtr(rtm)    (&((rtm)->Timing.stopRequestedFlag))
#endif

#ifndef rtmGetT
#define rtmGetT(rtm)                   (rtmGetTPtr((rtm))[0])
#endif

#ifndef rtmGetTPtr
#define rtmGetTPtr(rtm)                ((rtm)->Timing.t)
#endif

/* Forward declaration for rtModel */
typedef struct tag_RTM RT_MODEL;

/* Block signals and states (default storage) for system '<Root>' */
typedef struct {
  real_T VectorConcatenate[5];         /* '<Root>/Vector Concatenate' */
  real_T NextOutput[2];                /* '<Root>/Measurements noise' */
  real_T NextOutput_p[2];              /* '<Root>/System Disturbances' */
  real_T Impulse;                      /* '<Root>/Impulse' */
  uint32_T RandSeed[2];                /* '<Root>/Measurements noise' */
  uint32_T RandSeed_f[2];              /* '<Root>/System Disturbances' */
  boolean_T EnabledSubsystem_MODE;     /* '<S29>/Enabled Subsystem' */
  boolean_T MeasurementUpdate_MODE;    /* '<S22>/MeasurementUpdate' */
} DW;

/* Continuous states (default storage) */
typedef struct {
  real_T perfectModel[2];              /* '<Root>/Perfect Model' */
  real_T RealModel_CSTATE[2];          /* '<Root>/Real Model' */
} X;

/* State derivatives (default storage) */
typedef struct {
  real_T perfectModel[2];              /* '<Root>/Perfect Model' */
  real_T RealModel_CSTATE[2];          /* '<Root>/Real Model' */
} XDot;

/* State disabled  */
typedef struct {
  boolean_T perfectModel[2];           /* '<Root>/Perfect Model' */
  boolean_T RealModel_CSTATE[2];       /* '<Root>/Real Model' */
} XDis;

#ifndef ODE3_INTG
#define ODE3_INTG

/* ODE3 Integration Data */
typedef struct {
  real_T *y;                           /* output */
  real_T *f[3];                        /* derivatives */
} ODE3_IntgData;

#endif

/* Real-time Model Data Structure */
struct tag_RTM {
  const char_T *errorStatus;
  RTWSolverInfo solverInfo;
  X *contStates;
  int_T *periodicContStateIndices;
  real_T *periodicContStateRanges;
  real_T *derivs;
  boolean_T *contStateDisabled;
  boolean_T zCCacheNeedsReset;
  boolean_T derivCacheNeedsReset;
  boolean_T CTOutputIncnstWithState;
  real_T odeY[4];
  real_T odeF[3][4];
  ODE3_IntgData intgData;

  /*
   * Sizes:
   * The following substructure contains sizes information
   * for many of the model attributes such as inputs, outputs,
   * dwork, sample times, etc.
   */
  struct {
    int_T numContStates;
    int_T numPeriodicContStates;
    int_T numSampTimes;
  } Sizes;

  /*
   * Timing:
   * The following substructure contains information regarding
   * the timing information for the model.
   */
  struct {
    uint32_T clockTick0;
    time_T stepSize0;
    uint32_T clockTick1;
    SimTimeStep simTimeStep;
    boolean_T stopRequestedFlag;
    time_T *t;
    time_T tArray[2];
  } Timing;
};

/* Continuous states (default storage) */
extern X rtX;

/* Block signals and states (default storage) */
extern DW rtDW;

/* Model entry point functions */
extern void kalman_linearised_continuous_initialize(void);
extern void kalman_linearised_continuous_step(void);

/* Real-time Model object */
extern RT_MODEL *const rtM;

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<Root>/Estimation' : Unused code path elimination
 * Block '<S1>/A' : Unused code path elimination
 * Block '<S1>/B' : Unused code path elimination
 * Block '<S1>/C' : Unused code path elimination
 * Block '<S2>/CovarianceZ' : Unused code path elimination
 * Block '<S45>/Conversion' : Unused code path elimination
 * Block '<S45>/Data Type Duplicate' : Unused code path elimination
 * Block '<S46>/Conversion' : Unused code path elimination
 * Block '<S46>/Data Type Duplicate' : Unused code path elimination
 * Block '<S47>/Conversion' : Unused code path elimination
 * Block '<S47>/Data Type Duplicate' : Unused code path elimination
 * Block '<S48>/Conversion' : Unused code path elimination
 * Block '<S48>/Data Type Duplicate' : Unused code path elimination
 * Block '<S2>/KalmanGainL' : Unused code path elimination
 * Block '<S2>/KalmanGainM' : Unused code path elimination
 * Block '<S1>/D' : Unused code path elimination
 * Block '<S5>/Data Type Duplicate' : Unused code path elimination
 * Block '<S6>/Data Type Duplicate' : Unused code path elimination
 * Block '<S7>/Data Type Duplicate' : Unused code path elimination
 * Block '<S8>/Data Type Duplicate' : Unused code path elimination
 * Block '<S10>/Data Type Duplicate' : Unused code path elimination
 * Block '<S11>/Data Type Duplicate' : Unused code path elimination
 * Block '<S12>/Data Type Duplicate' : Unused code path elimination
 * Block '<S13>/Conversion' : Unused code path elimination
 * Block '<S13>/Data Type Duplicate' : Unused code path elimination
 * Block '<S14>/Data Type Duplicate' : Unused code path elimination
 * Block '<S15>/Data Type Duplicate' : Unused code path elimination
 * Block '<S16>/Data Type Duplicate' : Unused code path elimination
 * Block '<S18>/Conversion' : Unused code path elimination
 * Block '<S18>/Data Type Duplicate' : Unused code path elimination
 * Block '<S19>/Data Type Duplicate' : Unused code path elimination
 * Block '<S1>/G' : Unused code path elimination
 * Block '<S1>/H' : Unused code path elimination
 * Block '<S1>/MemoryX' : Unused code path elimination
 * Block '<S1>/N' : Unused code path elimination
 * Block '<S22>/A[k]*xhat[k|k-1]' : Unused code path elimination
 * Block '<S22>/Add' : Unused code path elimination
 * Block '<S22>/B[k]*u[k]' : Unused code path elimination
 * Block '<S51>/Add1' : Unused code path elimination
 * Block '<S51>/C[k]*xhat[k|k-1]' : Unused code path elimination
 * Block '<S51>/D[k]*u[k]' : Unused code path elimination
 * Block '<S51>/Product3' : Unused code path elimination
 * Block '<S51>/Sum' : Unused code path elimination
 * Block '<S22>/Reshape' : Unused code path elimination
 * Block '<S1>/P0' : Unused code path elimination
 * Block '<S1>/Q' : Unused code path elimination
 * Block '<S1>/R' : Unused code path elimination
 * Block '<S1>/ReshapeX0' : Unused code path elimination
 * Block '<S1>/Reshapeu' : Unused code path elimination
 * Block '<S1>/Reshapexhat' : Unused code path elimination
 * Block '<S1>/Reshapey' : Unused code path elimination
 * Block '<S29>/Add' : Unused code path elimination
 * Block '<S53>/Add1' : Unused code path elimination
 * Block '<S53>/Product' : Unused code path elimination
 * Block '<S53>/Product2' : Unused code path elimination
 * Block '<S1>/X0' : Unused code path elimination
 * Block '<S43>/CheckSignalProperties' : Unused code path elimination
 * Block '<S44>/CheckSignalProperties' : Unused code path elimination
 * Block '<Root>/Measured output' : Unused code path elimination
 * Block '<Root>/Perfect output' : Unused code path elimination
 * Block '<Root>/Quantizer' : Unused code path elimination
 * Block '<Root>/Quantizer1' : Unused code path elimination
 * Block '<Root>/Zero-Order Hold' : Unused code path elimination
 * Block '<Root>/Zero-Order Hold1' : Unused code path elimination
 */

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Use the MATLAB hilite_system command to trace the generated code back
 * to the model.  For example,
 *
 * hilite_system('<S3>')    - opens system 3
 * hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'kalman_linearised_continuous'
 * '<S1>'   : 'kalman_linearised_continuous/Kalman Filter'
 * '<S2>'   : 'kalman_linearised_continuous/Kalman Filter/CalculatePL'
 * '<S3>'   : 'kalman_linearised_continuous/Kalman Filter/CalculateYhat'
 * '<S4>'   : 'kalman_linearised_continuous/Kalman Filter/CovarianceOutputConfigurator'
 * '<S5>'   : 'kalman_linearised_continuous/Kalman Filter/DataTypeConversionA'
 * '<S6>'   : 'kalman_linearised_continuous/Kalman Filter/DataTypeConversionB'
 * '<S7>'   : 'kalman_linearised_continuous/Kalman Filter/DataTypeConversionC'
 * '<S8>'   : 'kalman_linearised_continuous/Kalman Filter/DataTypeConversionD'
 * '<S9>'   : 'kalman_linearised_continuous/Kalman Filter/DataTypeConversionEnable'
 * '<S10>'  : 'kalman_linearised_continuous/Kalman Filter/DataTypeConversionG'
 * '<S11>'  : 'kalman_linearised_continuous/Kalman Filter/DataTypeConversionH'
 * '<S12>'  : 'kalman_linearised_continuous/Kalman Filter/DataTypeConversionN'
 * '<S13>'  : 'kalman_linearised_continuous/Kalman Filter/DataTypeConversionP'
 * '<S14>'  : 'kalman_linearised_continuous/Kalman Filter/DataTypeConversionP0'
 * '<S15>'  : 'kalman_linearised_continuous/Kalman Filter/DataTypeConversionQ'
 * '<S16>'  : 'kalman_linearised_continuous/Kalman Filter/DataTypeConversionR'
 * '<S17>'  : 'kalman_linearised_continuous/Kalman Filter/DataTypeConversionReset'
 * '<S18>'  : 'kalman_linearised_continuous/Kalman Filter/DataTypeConversionX'
 * '<S19>'  : 'kalman_linearised_continuous/Kalman Filter/DataTypeConversionX0'
 * '<S20>'  : 'kalman_linearised_continuous/Kalman Filter/DataTypeConversionu'
 * '<S21>'  : 'kalman_linearised_continuous/Kalman Filter/MemoryP'
 * '<S22>'  : 'kalman_linearised_continuous/Kalman Filter/Observer'
 * '<S23>'  : 'kalman_linearised_continuous/Kalman Filter/ReducedQRN'
 * '<S24>'  : 'kalman_linearised_continuous/Kalman Filter/Reset'
 * '<S25>'  : 'kalman_linearised_continuous/Kalman Filter/Reshapeyhat'
 * '<S26>'  : 'kalman_linearised_continuous/Kalman Filter/ScalarExpansionP0'
 * '<S27>'  : 'kalman_linearised_continuous/Kalman Filter/ScalarExpansionQ'
 * '<S28>'  : 'kalman_linearised_continuous/Kalman Filter/ScalarExpansionR'
 * '<S29>'  : 'kalman_linearised_continuous/Kalman Filter/UseCurrentEstimator'
 * '<S30>'  : 'kalman_linearised_continuous/Kalman Filter/checkA'
 * '<S31>'  : 'kalman_linearised_continuous/Kalman Filter/checkB'
 * '<S32>'  : 'kalman_linearised_continuous/Kalman Filter/checkC'
 * '<S33>'  : 'kalman_linearised_continuous/Kalman Filter/checkD'
 * '<S34>'  : 'kalman_linearised_continuous/Kalman Filter/checkEnable'
 * '<S35>'  : 'kalman_linearised_continuous/Kalman Filter/checkG'
 * '<S36>'  : 'kalman_linearised_continuous/Kalman Filter/checkH'
 * '<S37>'  : 'kalman_linearised_continuous/Kalman Filter/checkN'
 * '<S38>'  : 'kalman_linearised_continuous/Kalman Filter/checkP0'
 * '<S39>'  : 'kalman_linearised_continuous/Kalman Filter/checkQ'
 * '<S40>'  : 'kalman_linearised_continuous/Kalman Filter/checkR'
 * '<S41>'  : 'kalman_linearised_continuous/Kalman Filter/checkReset'
 * '<S42>'  : 'kalman_linearised_continuous/Kalman Filter/checkX0'
 * '<S43>'  : 'kalman_linearised_continuous/Kalman Filter/checku'
 * '<S44>'  : 'kalman_linearised_continuous/Kalman Filter/checky'
 * '<S45>'  : 'kalman_linearised_continuous/Kalman Filter/CalculatePL/DataTypeConversionL'
 * '<S46>'  : 'kalman_linearised_continuous/Kalman Filter/CalculatePL/DataTypeConversionM'
 * '<S47>'  : 'kalman_linearised_continuous/Kalman Filter/CalculatePL/DataTypeConversionP'
 * '<S48>'  : 'kalman_linearised_continuous/Kalman Filter/CalculatePL/DataTypeConversionZ'
 * '<S49>'  : 'kalman_linearised_continuous/Kalman Filter/CalculatePL/Ground'
 * '<S50>'  : 'kalman_linearised_continuous/Kalman Filter/CalculateYhat/Ground'
 * '<S51>'  : 'kalman_linearised_continuous/Kalman Filter/Observer/MeasurementUpdate'
 * '<S52>'  : 'kalman_linearised_continuous/Kalman Filter/ReducedQRN/Ground'
 * '<S53>'  : 'kalman_linearised_continuous/Kalman Filter/UseCurrentEstimator/Enabled Subsystem'
 */
#endif                          /* RTW_HEADER_kalman_linearised_continuous_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
