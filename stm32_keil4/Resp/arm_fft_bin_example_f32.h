/**
 * @ingroup groupExamples
 */

/**
 * @defgroup FrequencyBin Frequency Bin Example
 *
 * \par Description
 * \par
 * Demonstrates the calculation of the maximum energy bin in the frequency
 * domain of the input signal with the use of Complex FFT, Complex
 * Magnitude, and Maximum functions.
 *
 * \par Algorithm:
 * \par
 * The input test signal contains a 10 kHz signal with uniformly distributed white noise.
 * Calculating the FFT of the input signal will give us the maximum energy of the
 * bin corresponding to the input frequency of 10 kHz.
 *
 * \par Block Diagram:
 * \image html FFTBin.gif "Block Diagram"
 * \par
 * The figure below shows the time domain signal of 10 kHz signal with
 * uniformly distributed white noise, and the next figure shows the input
 * in the frequency domain. The bin with maximum energy corresponds to 10 kHz signal.
 * \par
 * \image html FFTBinInput.gif "Input signal in Time domain"
 * \image html FFTBinOutput.gif "Input signal in Frequency domain"
 *
 * \par Variables Description:
 * \par
 * \li \c testInput_fft points to the input data
 * \li \c testOutput_fft points to the output data
 * \li \c fftSize length of FFT
 * \li \c ifftFlag flag for the selection of CFFT/CIFFT
 * \li \c doBitReverse Flag for selection of normal order or bit reversed order
 * \li \c refIndex reference index value at which maximum energy of bin ocuurs
 * \li \c testIndex calculated index value at which maximum energy of bin ocuurs
 *
 * \par CMSIS DSP Software Library Functions Used:
 * \par
 * - arm_cfft_f32()
 * - arm_cmplx_mag_f32()
 * - arm_max_f32()
 *
 * <b> Refer  </b>
 * \link arm_fft_bin_example_f32.c \endlink
 *
 */


/** \example arm_fft_bin_example_f32.c
  */


#include "arm_math.h"
#include "arm_const_structs.h"
#include "parametre_define.h"


/* -------------------------------------------------------------------
* External Input and Output buffer Declarations for FFT Bin Example
* ------------------------------------------------------------------- */
// static float32_t cell_2048[TEST_LENGTH_SAMPLES_FFT] = {0};
// static float32_t cell_1024[TEST_LENGTH_SAMPLES_FFT/2] = {0};

// static float32_t *testInput_fft = &cell_2048[0];
// static float32_t *testOutput_fft = &cell_1024[0];


/* ------------------------------------------------------------------
* Global variables for FFT Bin Example
* ------------------------------------------------------------------- */
uint32_t fftSize = LENGTH;
uint32_t ifftFlag = 0;
uint32_t doBitReverse = 1;

// Input has 2048 elements (1024 pairs, real & imaginary) 
// and output has 1024 elements
int32_t arm_fft(float32_t *testInput_fft, float32_t *testOutput_fft){

  /* Process the data through the CFFT/CIFFT module */
 // arm_cfft_f32(&arm_cfft_sR_f32_len1024, testInput_fft, ifftFlag, doBitReverse);

  /* Process the data through the Complex Magnitude Module for
  calculating the magnitude at each bin */
	arm_cmplx_mag_f32(testInput_fft, testOutput_fft, fftSize);
	
	return 1;
}
