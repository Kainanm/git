#include "arm_const_structs.h"
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
const float firCoeffs32[NUM_TAPS] = 
{ 
	0.00158878927350977, 0.0048979218281252,
    0.00443964403229724, -0.00990584175404258, -0.0393638870264682,
    -0.0608687512419786, -0.0335153477433042, 0.0654496363209545,
    0.20492449540203, 0.307873715452276, 0.307873715452276, 0.20492449540203,
    0.0654496363209545, -0.0335153477433042, -0.0608687512419786,
    -0.0393638870264682, -0.00990584175404258, 0.00443964403229724,
    0.0048979218281252, 0.00158878927350977 
};

// for FFT
uint32_t fftSize = LENGTH;
uint32_t ifftFlag = 0;