/*
 * File: _coder_cellForC_api.h
 *
 * MATLAB Coder version            : 4.1
 * C/C++ source code generated on  : 31-Oct-2018 11:13:13
 */

#ifndef _CODER_CELLFORC_API_H
#define _CODER_CELLFORC_API_H

/* Include Files */
#include "tmwtypes.h"
#include "mex.h"
#include "emlrt.h"
#include <stddef.h>
#include <stdlib.h>
#include "_coder_cellForC_api.h"

/* Variable Declarations */
extern emlrtCTX emlrtRootTLSGlobal;
extern emlrtContext emlrtContextGlobal;

/* Function Declarations */
extern void cellForC(real_T y[35]);
extern void cellForC_api(int32_T nlhs, const mxArray *plhs[1]);
extern void cellForC_atexit(void);
extern void cellForC_initialize(void);
extern void cellForC_terminate(void);
extern void cellForC_xil_terminate(void);

#endif

/*
 * File trailer for _coder_cellForC_api.h
 *
 * [EOF]
 */
