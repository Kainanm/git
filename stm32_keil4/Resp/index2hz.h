#include <stdio.h>
#include <math.h>
#include<malloc.h>

float *index2hz(int index[], int len, float lower_range, float upper_range){
	/* To map the index to a frequency based on given upper and lower range (in hz),
	where len is the length of the indices.
	The output represents times of breath per second. */
	float m;
	float *hz;
	int i;

	hz = (float*)malloc(sizeof(float)*len);

	m = (upper_range - lower_range) / (float)(len - 1);

	for (i = 0; i<len; i++){
		hz[i] = m * (float)index[i] + lower_range;
		//hz[i] = hz[i] * 60.0; // To convert to times per minute.
	}


	return hz;
	free(hz);
}
// 未测试
/*******************************
void main(){
	int ind = 14;
	int length = 128;
	float lower = 0;
	float upper = 8.5;

	//float freq = index2hz(ind, length, lower, upper);

	//printf("%f ", freq);
}

// 输出 freq = 0.937
********************************************/


/*******************************************
#include <stdio.h>
#include <math.h>

float index2hz(int index, int len, float lower_range, float upper_range){
	// To map the index to a frequency based on given upper and lower range (in hz),
	// where len is the length of the indices.
	// The output represents times of breath per second. 
	float m, hz;
	m = (upper_range - lower_range) / (float)(len - 1);
	hz = m * (float)index + lower_range;
	// hz = hz * 60; // To convert to times per minute.

	return hz;
}
// 测试成功，可输出下标对应的频率

void main(){
	int ind = 14;
	int length = 128;
	float lower = 0;
	float upper = 8.5;

	float freq = index2hz(ind, length, lower, upper);

	printf("%f ", freq);
}

// 输出 freq = 0.937
******************************************/