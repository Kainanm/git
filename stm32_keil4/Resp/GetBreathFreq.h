#include "parametre_define.h"

float GetBreathFreq(float index_hz[], int len, float abnorm_lower, float abnorm_upper){
	/* To output the frequency of breath per minute. The input is in hz (per second).
	If the frequency is out of range of (abnorm_lower, abnorm_upper),
	then, we discard it.
	abnorm_lower and abnorm_upper are in hz (per second)
	len is the lenth of index_hz*/
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

/*
void main(){

}
*/
