/* ----------------------------------------------------------------------
 * Copyright (C) 2010-2012 ARM Limited. All rights reserved.
 *
* $Date:         17. January 2013
* $Revision:     V1.4.0
*
* Project:       CMSIS DSP Library
 * Title:        arm_fir_example_f32.c
 *
 * Description:  Example code demonstrating how an FIR filter can be used
 *               as a low pass filter.
 *
 * Target Processor: Cortex-M4/Cortex-M3
 *
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
*   - Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer.
*   - Redistributions in binary form must reproduce the above copyright
*     notice, this list of conditions and the following disclaimer in
*     the documentation and/or other materials provided with the
*     distribution.
*   - Neither the name of ARM LIMITED nor the names of its contributors
*     may be used to endorse or promote products derived from this
*     software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
* COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
* LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
* ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
 * -------------------------------------------------------------------- */

/**
 * @ingroup groupExamples
 */

/**
 * @defgroup FIRLPF FIR Lowpass Filter Example
 *
 * \par Description:
 * \par
 * Removes high frequency signal components from the input using an FIR lowpass filter.
 * The example demonstrates how to configure an FIR filter and then pass data through
 * it in a block-by-block fashion.
 * \image html FIRLPF_signalflow.gif
 *
 * \par Algorithm:
 * \par
 * The input signal is a sum of two sine waves:  1 kHz and 15 kHz.
 * This is processed by an FIR lowpass filter with cutoff frequency 6 kHz.
 * The lowpass filter eliminates the 15 kHz signal leaving only the 1 kHz sine wave at the output.
 * \par
 * The lowpass filter was designed using MATLAB with a sample rate of 48 kHz and
 * a length of 29 points.
 * The MATLAB code to generate the filter coefficients is shown below:
 * <pre>
 *     h = fir1(28, 6/24);
 * </pre>
 * The first argument is the "order" of the filter and is always one less than the desired length.
 * The second argument is the normalized cutoff frequency.  This is in the range 0 (DC) to 1.0 (Nyquist).
 * A 6 kHz cutoff with a Nyquist frequency of 24 kHz lies at a normalized frequency of 6/24 = 0.25.
 * The CMSIS FIR filter function requires the coefficients to be in time reversed order.
 * <pre>
 *     fliplr(h)
 * </pre>
 * The resulting filter coefficients and are shown below.
 * Note that the filter is symmetric (a property of linear phase FIR filters)
 * and the point of symmetry is sample 14.  Thus the filter will have a delay of
 * 14 samples for all frequencies.
 * \par
 * \image html FIRLPF_coeffs.gif
 * \par
 * The frequency response of the filter is shown next.
 * The passband gain of the filter is 1.0 and it reaches 0.5 at the cutoff frequency 6 kHz.
 * \par
 * \image html FIRLPF_response.gif
 * \par
 * The input signal is shown below.
 * The left hand side shows the signal in the time domain while the right hand side is a frequency domain representation.
 * The two sine wave components can be clearly seen.
 * \par
 * \image html FIRLPF_input.gif
 * \par
 * The output of the filter is shown below.  The 15 kHz component has been eliminated.
 * \par
 * \image html FIRLPF_output.gif
 *
 * \par Variables Description:
 * \par
 * \li \c testInput points to the input data
 * \li \c refOutput points to the reference output data
 * \li \c testOutput points to the test output data
 * \li \c firStateF32 points to state buffer
 * \li \c firCoeffs32 points to coefficient buffer
 * \li \c blockSize number of samples processed at a time
 * \li \c numBlocks number of frames
 *
 * \par CMSIS DSP Software Library Functions Used:
 * \par
 * - arm_fir_init_f32()
 * - arm_fir_f32()
 *
 * <b> Refer  </b>
 * \link arm_fir_example_f32.c \endlink
 *
 */


/** \example arm_fir_example_f32.c
 */

/* ----------------------------------------------------------------------
** Include Files
** ------------------------------------------------------------------- */

#include "arm_math.h"
#include "math_helper.h"

/* ----------------------------------------------------------------------
** Macro Defines
** ------------------------------------------------------------------- */

#define TEST_LENGTH_SAMPLES  1024
#define SNR_THRESHOLD_F32    140.0f
#define BLOCK_SIZE            1
#define NUM_TAPS              44

/* -------------------------------------------------------------------
 * The input signal and reference output (computed with MATLAB)
 * are defined externally in arm_fir_lpf_data.c.
 * ------------------------------------------------------------------- */

extern float32_t testInput[TEST_LENGTH_SAMPLES];
//extern float32_t refOutput[TEST_LENGTH_SAMPLES];

/* -------------------------------------------------------------------
 * Declare Test output buffer
 * ------------------------------------------------------------------- */

static float32_t testOutput[TEST_LENGTH_SAMPLES];

/* -------------------------------------------------------------------
 * Declare State buffer of size (numTaps + blockSize - 1)
 * ------------------------------------------------------------------- */

static float32_t firStateF32[BLOCK_SIZE + NUM_TAPS - 1];

/* ----------------------------------------------------------------------
** FIR Coefficients buffer generated using fir1() MATLAB function.
** fir1(28, 6/24)
** ------------------------------------------------------------------- */

const float32_t firCoeffs32[NUM_TAPS] = { -0.000432968108717059, -0.00140765600882177,
    -0.00307208470538723, -0.00507583399824286, -0.00651386126630115,
    -0.00608279210663509, -0.00262294123528309, 0.0040996502053513,
    0.0127339186161362, 0.0202636565151811, 0.0228171801858489,
    0.0172944759319146, 0.00326102151472989, -0.0157704037158572,
    -0.0324992203964246, -0.0377776039614552, -0.0238906006086383,
    0.011993001087039, 0.0656674635287373, 0.126161646152355, 0.178653612860713,
    0.20913670626946, 0.20913670626946, 0.178653612860713, 0.126161646152355,
    0.0656674635287373, 0.011993001087039, -0.0238906006086383,
    -0.0377776039614552, -0.0324992203964246, -0.0157704037158572,
    0.00326102151472989, 0.0172944759319146, 0.0228171801858489,
    0.0202636565151811, 0.0127339186161362, 0.0040996502053513,
    -0.00262294123528309, -0.00608279210663509, -0.00651386126630115,
    -0.00507583399824286, -0.00307208470538723, -0.00140765600882177,
    -0.000432968108717059 };

/* ------------------------------------------------------------------
 * Global variables for FIR LPF Example
 * ------------------------------------------------------------------- */

uint32_t blockSize = BLOCK_SIZE;
uint32_t numBlocks = TEST_LENGTH_SAMPLES/BLOCK_SIZE;

//float32_t  snr;

/* ----------------------------------------------------------------------
 * FIR LPF Example
 * ------------------------------------------------------------------- */

int32_t fun_lpf(void)
	// input stores in testInput and output stores in testOutput 
{
  uint32_t i;
  arm_fir_instance_f32 S;
 // arm_status status;
  float32_t  *inputF32, *outputF32;

  /* Initialize input and output buffer pointers */
  inputF32 = &testInput[0];
  outputF32 = &testOutput[0];

  /* Call FIR init function to initialize the instance structure. */
  arm_fir_init_f32(&S, NUM_TAPS, (float32_t *)&firCoeffs32[0], &firStateF32[0], blockSize);

  /* ----------------------------------------------------------------------
  ** Call the FIR process function for every blockSize samples
  ** ------------------------------------------------------------------- */

  for(i=0; i < numBlocks; i++)
  {
    arm_fir_f32(&S, inputF32 + (i * blockSize), outputF32 + (i * blockSize), blockSize);
  }

  
}

/** \endlink */
