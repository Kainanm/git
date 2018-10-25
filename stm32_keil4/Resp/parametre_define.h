#include <math.h>  
#include "sys.h"

#define LENGTH 256 // length of input signal
#define ROW 3
#define COLUMN LENGTH
#define SAMP_RATE 4.0
#define BUFF_LEN 128
//#define MPU_RATE (u16)SAMP_RATE // the sample rate of MPU6050, 4-1000 hz
#define MPU_DVDR (u8)(100/SAMP_RATE) // The sample rate of MPU is always 100 Hz
																			// Do not know why. So, it needs a freqz devider 
																			// to get the freqz you need.

//#define SRAM 0x2000e000 // SRAM starts from 0x2000e000 in STM32F103ZET6

// define the range of respiratory frequencies
#define HZ_LOWER 0.2
#define HZ_UPPER 1.25

// Calculate the length of output of fun_map
#define MMM (float)(LENGTH - 1) / (float)SAMP_RATE // The gradient of map line
#define MAP_LENGTH ((int)(MMM * HZ_UPPER) + 1) - ((int)(MMM * HZ_LOWER)) + 1
// #define MAP_LENGTH 69

// Assume the resipatory rate is [12.5, 40] per minute. 
// Results out of the range are considered as odd values.
#define ODD_LOWER 12.5
#define ODD_UPPER 40

// Define parametres for FIR filter
#define TEST_LENGTH_SAMPLES  LENGTH
#define SNR_THRESHOLD_F32    140.0f
#define BLOCK_SIZE           1
#define NUM_TAPS             6


// Define for FFT
#define TEST_LENGTH_SAMPLES_FFT LENGTH*2


