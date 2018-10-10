#include "parametre_define.h"
#include "GetVariance.h"
#include "AngLocDes.h"
#include "arm_fir_example_f32.h"
#include "detrend.h"
#include "arm_fft_bin_example_f32.h"
#include "fun_map.h"
#include "sortindex.h"
#include "fun_index2hz.h"
#include "GetBreathFreq.h"
#include "mpu_read.h"

/*------------------------------------------------
* state the global variables
-------------------------------------------------*/
static float32_t cell_double_length[TEST_LENGTH_SAMPLES_FFT];
static float32_t cell_length[TEST_LENGTH_SAMPLES_FFT/2];
int max_index = 0;

float resp_rate_cal(float indata[ROW][COLUMN]){	      
/// state variables
	int i,j;	
	float breath_rate = 5.2;

/// --------code start here---------	
	
	// Get variance	
	// Alert!!!!: The variance may go infinite if there are too many or too large inputs.
	float64_t var[ROW] = {0}; // to store the variances of input Eulerian angles in order of Pitch Yaw Roll
	for(i=0;i<ROW;i++){
		var[i] = GetVariane(indata[i], LENGTH);
	}
	
	// Find the max angle variance
	max_index = AngLocDes(var, ROW);
	// to make sure max_index is in {0,1,2}
	if (max_index >2) 
		max_index = 2;
	else if(max_index<0) 
		max_index = 0;
	else 
		max_index = 1;
	
	// LPF	
	float *lpf_input;
	float *lpf_output;
	lpf_input = &indata[max_index][0]; // testInput is the pointer to the input of fun_lpf
	lpf_output = &cell_length[0];
	fun_lpf(lpf_input, lpf_output); //the output is in testOutput, a global variable in arm_fir_example_f32.h

	// detrend
	// The input and outputo of detrend() are both detrend_out
	float *detrend_out;
	detrend_out = &cell_length[0];
	detrend(detrend_out, LENGTH); 

	// fft
	// * testIntput_fft has 2048 numbers. Even positions are for real parts 
	// * and odd positions are for imaginary parts, which are 0 in this case.
	j = 0;
	float *fft_input; 
	fft_input = &cell_double_length[0];
	for(i=0;i<TEST_LENGTH_SAMPLES;i=i+2){
		// fft_input[i] = detrend_out[j]; 
		fft_input[i] = cell_length[j]; 
		fft_input[i+1] = 0;
		j++;
	}
	float *fft_output;
	fft_output = &cell_length[0];
	arm_fft(fft_input, fft_output); // The output is in testOutput_fft in arm_fft_bin_example_f32.h

	
	// fun_map. 
	// Pick up freuqency components of the signal between 0.2-1.25 Hz.
	// The length of output is MAP_LENGTH
	float *map_out;
	map_out = &cell_double_length[0];
	fun_map(fft_output, map_out, LENGTH, HZ_LOWER, HZ_UPPER);

	// sort
	int sort_out[MAP_LENGTH];
	// sortindex(map_out, sort_out, MAP_LENGTH);
	
	// fun_index2hz
	float *freq_out;
	freq_out = &cell_length[0];
	fun_index2hz(map_out, freq_out, MAP_LENGTH, HZ_LOWER, HZ_UPPER);
	
	// GetBreathFreq	
	breath_rate = GetBreathFreq(freq_out, MAP_LENGTH, ODD_LOWER, ODD_UPPER); 

	return breath_rate;

}
// breath_rate = 15.5437498 given the testData. 
// Close to m+atlab result(?)  15.2484
