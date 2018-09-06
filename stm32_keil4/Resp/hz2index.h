#include <stdio.h>
#include <math.h>

int hz2index(float hz, int len, float lower_hz, float upper_hz){
	/*len is the length of the indices*/
	float m;
	int index;

	m = (float)(len - 1) / (upper_hz - lower_hz);
	index = round(m * (hz - lower_hz));

	return index;
}
// ≤‚ ‘≥…π¶
/*
void main(){
	// int ind = 14;
	int length = 128;
	float lower = 0;
	float upper = 8.5;
	float hz = 0.937;

	int ind = hz2index(hz, length, lower, upper);

	// float freq = index2hz(ind, length, lower, upper);

	printf("%d ", ind);
}
*/
// ind = 14