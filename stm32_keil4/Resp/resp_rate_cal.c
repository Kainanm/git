#include "arm_const_structs.h"
#include "arm_math.h"
#include "math_helper.h"
#include "parametre_define.h"

/*------------------------------------------------
 * state the global variables
-------------------------------------------------*/
// for resp_rate_cal
static float32_t cell_double_length[TEST_LENGTH_SAMPLES_FFT];
static float32_t cell_length[TEST_LENGTH_SAMPLES_FFT/2];
int max_index = 0;
int sort_out[MAP_LENGTH]={0};

// for FIR
static float32_t firStateF32[BLOCK_SIZE + NUM_TAPS - 1]; // Must be global
// 4hz sampling
const float firCoeffs32[NUM_TAPS] = 
{ 
	0.0122954799504127, 0.0425068233315651,
    0.0718910972165521, 0.0718910972165521, 0.0425068233315651,
    0.0122954799504127
};

/*
const float firCoeffs32[NUM_TAPS] = 
{ 
	0.0019906816571561, 0.00663508546985115,
    0.0154808677407167, 0.0288579339714581, 0.0456736982408063,
    0.0632783341418269, 0.0780497282265503, 0.086501957872566, 0.086501957872566,
    0.0780497282265503, 0.0632783341418269, 0.0456736982408063,
    0.0288579339714581, 0.0154808677407167, 0.00663508546985115,
    0.0019906816571561
};
*/
/* 17 Hz
const float firCoeffs32[NUM_TAPS] = 
{ 
	0.000638089232304094, 0.00113749927915644,
    0.002061834022221, 0.00337958711571103, 0.00514989906986831,
    0.00741121218618356, 0.0101737495736841, 0.013412325689036,
    0.0170631666803178, 0.0210233473510598, 0.0251537632837594,
    0.0292857059356585, 0.0332315280935397, 0.036797890201564,
    0.0397987503841056, 0.0420717046137831, 0.0434893276251056,
    0.0439712986903181, 0.0434893276251056, 0.0420717046137831,
    0.0397987503841056, 0.036797890201564, 0.0332315280935397,
    0.0292857059356585, 0.0251537632837594, 0.0210233473510598,
    0.0170631666803178, 0.013412325689036, 0.0101737495736841,
    0.00741121218618356, 0.00514989906986831, 0.00337958711571103,
    0.002061834022221, 0.00113749927915644, 0.000638089232304094
};
*/
// for FFT
uint32_t fftSize = LENGTH;
uint32_t ifftFlag = 0;
uint32_t doBitReverse = 1;


double GetVariane(float x[], int n){
	//求数组x（具有n个元素）的方差: Var=E(X^2)-[E(X)]^2
	int i;
	double xaver = 0.0; // xaver为x的平均值
	double x_var = 0.0;
	
	for (i = 0; i<n; i++){
		xaver += x[i]; 
	}
	xaver /= (n-1); 
	
	for(i = 0; i<n; i++){
		x_var += (x[i] - xaver) * (x[i] - xaver);
	}
	x_var /= (n+1); //求x的平均、x^2的平均

	return x_var;
}

int AngLocDes(double angle_var[], int n){
	// n is the length of angle_var, AKA the number of variances
	int i;
	double max; 
	int index_max;
  max = angle_var[0]; 
	for (i = 1; i<n; i++){
		if (angle_var[i] > max){
			max = angle_var[i];
			index_max = i;
		}
	}
	if(i>2){
		i = 2;
	} else if(i<0){
		i = 0;
	}	// to make sure index is in {0,1,2}
	
	return index_max;
}

int32_t fun_lpf(float32_t *testInput, float32_t *testOutput){
	uint32_t i;
	arm_fir_instance_f32 S;
	float32_t  *inputF32, *outputF32;
	
	uint32_t blockSize = BLOCK_SIZE;
	uint32_t numBlocks = TEST_LENGTH_SAMPLES/BLOCK_SIZE;

	/* Initialize input and output buffer pointers */
    inputF32 = testInput;
	outputF32 = &testOutput[0];

	/* Call FIR init function to initialize the instance structure. */
	arm_fir_init_f32(&S, NUM_TAPS, (float32_t *)&firCoeffs32[0], &firStateF32[0], blockSize);

	for(i=0; i < numBlocks; i++){
		arm_fir_f32(&S, inputF32 + (i * blockSize), outputF32 + (i * blockSize), blockSize);
	}
	return 1;
}

void detrend(float y[], int n){
	/***********************************
	To remove the linear trend of the input y[].
	n is the length of the input, n is 1024 (LENGTH).
	detrend_out[] is output.
	****************************************/
	int x=0;
	double sum_x = 0,sum_y = 0, sum_xy = 0, sum_xx = 0;
	double avr_x = 0, avr_y = 0;
	double a = 0, b = 0;

	for (x = 0; x<n; x++){
		sum_x += x;
		sum_y += y[x];
		sum_xy += x*y[x];
		sum_xx += x*x;
	}

	a = (n * sum_xy - sum_x * sum_y) / (n * sum_xx - sum_x * sum_x);
	avr_x = sum_x / n;
	avr_y = sum_y / n;
	b = avr_y - a * avr_x;

	for (x = 0; x<n; x++){
		y[x] = y[x] - (float)(a * x + b); 
	}
}

int32_t arm_fft(float32_t *testInput_fft, float32_t *testOutput_fft){

  /* Process the data through the CFFT/CIFFT module */
  arm_cfft_f32(&arm_cfft_sR_f32_len256, testInput_fft, ifftFlag, doBitReverse);

  /* Process the data through the Complex Magnitude Module for
  calculating the magnitude at each bin */
	arm_cmplx_mag_f32(testInput_fft, testOutput_fft, fftSize);
	
	return 1;
}

// n is the length of the ang_fft
// The function is to get the abs(fft_signals) at frequencies we want.
// the output is stored in ind_mapped
int fun_map(float ang_fft[], float ind_mapped[], int n, float pickfreq_lower, float pickfreq_upper){
	
	int i;
	float m = (float)(n - 1) / (float)SAMP_RATE;
	int ind_lower = (int)(m * pickfreq_lower); // find the index corresponding to HZ_LOWER. 
	// if ind_lower +1, the outcome breath_rate is closer to matlab result, but it does not make sense on math.
		
	for (i = 0; i < MAP_LENGTH; i++){
		ind_mapped[i] = ang_fft[ind_lower + i];
	}

	return 1;
}

// a is input and p_index is output
// n = MAP_LENGTH
int sortIndex(float a[], int p_index[], int n){
	int i, j;
	int temp1;
	float temp2;	

	for (i = 0; i<n; i++){
		p_index[i] = i;
	}
	//n个数的数列总共扫描n-1次  
	for (i = 0; i<n; i++){
		//每一趟扫描到a[n-i-2]与a[n-i-1]比较为止结束 
		for (j = 0; j<n - i - 1; j++){
			//后一位数比前一位数大的话，就交换两个数的位置(降序)
			if (a[j]<a[j + 1]){
				temp2 = a[j + 1];
				a[j + 1] = a[j];
				a[j] = temp2;

				temp1 = p_index[j];
				p_index[j] = p_index[j + 1];
				p_index[j + 1] = temp1;
			}
		}
	}
	return 1;
}

/* To map the index to a frequency based on given upper and lower hz (in hz),
	where len is the length of the indices.
	The output represents times of breath per second, stored in hz[]. */
int fun_index2hz(int index[], float hz[], int len, float lower_hz, float upper_hz){
	float m;
	int i;
	m = (upper_hz - lower_hz) / (float)(len - 1);
	for (i = 0; i<len; i++){
		hz[i] = m * (float)index[i] + lower_hz;
		hz[i] = hz[i] * 60.0; // To convert to times per minute.
	}
	return 1;
}

/* To output the frequency of breath per minute. The input is in hz (per second).
If the frequency is out of range of (abnorm_lower, abnorm_upper),
then, we discard it.
abnorm_lower and abnorm_upper are in hz (per second)
len is the lenth of index_hz*/
float GetBreathFreq(float index_hz[], int len, float abnorm_lower, float abnorm_upper){
	int i = 0;
	float freq1, freq2;
	float breath_freq;

	while((index_hz[i] < abnorm_lower) || (index_hz[i] > abnorm_upper)){
		i = i + 1;
	}
	if (i <= len){
		freq1 = index_hz[i]; // Frequency correspond to the max peak
	}
	else{
		freq1 = -1.0; // Give it a very odd value, so we know something is wrong.
	}
	
	i= i+ 1;

	while((index_hz[i] < abnorm_lower) || (index_hz[i] > abnorm_upper)){
		i = i + 1;
	}
	if (i <= len){
		freq2 = index_hz[i]; // Frequency correspond to the second max peak
	}
	else{
		freq2 = -1.0;
	}

	breath_freq = (0.7 * freq1 + 0.3 * freq2); // output is in unit of per minute，公式有待改进
	return breath_freq;
}

float resp_rate_cal(float indata[ROW][COLUMN]){	      
/// state variables
	int i,j;	
	float breath_rate = 5.2;

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
	sortIndex(map_out, sort_out, MAP_LENGTH);
	
	// fun_index2hz
	float *freq_out;
	freq_out = &cell_length[0];
	fun_index2hz(sort_out, freq_out, MAP_LENGTH, HZ_LOWER, HZ_UPPER);
	
	// GetBreathFreq	
	breath_rate = GetBreathFreq(freq_out, MAP_LENGTH, ODD_LOWER, ODD_UPPER); 

	return breath_rate;

}
// breath_rate = 15.5437498 given the testData. 
// Close to m+atlab result(?)  15.2484
