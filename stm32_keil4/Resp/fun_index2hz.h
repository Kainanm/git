#include "parametre_define.h"

int fun_index2hz(int index[], float hz[], int len, float lower_hz, float upper_hz){
	/* To map the index to a frequency based on given upper and lower hz (in hz),
	where len is the length of the indices.
	The output represents times of breath per second, stored in hz[]. */
	float m;
	int i;

	
	m = (upper_hz - lower_hz) / (float)(len - 1);

	for (i = 0; i<len; i++){
		hz[i] = m * (float)index[i] + lower_hz;
		hz[i] = hz[i] * 60.0; // To convert to times per minute.
	}


	return 1;
	//free(hz);
}
// 测试成功

/* int  main( )
{
int   indata[LENGTH],outdata[LENGTH];
int		i;

// input data
errno_t err;
FILE* fpRead;
if ((err = fopen_s(&fpRead, "input.txt", "r")) != 0){
printf("Cannot open this filefpRead\n");
exit(0);
}
int res = 0;
for (i = 0; i < LENGTH; i++){
res = fscanf_s(fpRead, "%d", &indata[i]);
//printf("%d\n", indata[i]);
if (res == EOF)	break;
}
fclose(fpRead);

// --------code start here---------

float lower = 0.2;
float upper = 1.25;
float *freq;
freq = fun_index2hz(indata, LENGTH, lower, upper);



// ----------code end here---------

//output data
FILE *fpWrite=fopen("output.txt","w");
if(fpWrite==NULL){
return 0;
}
for(int i=0;i<LENGTH;i++){
fprintf(fpWrite,"%f ",freq[i]);
}
fclose(fpWrite);

getchar();
return 1;

}
// 测试成功 */


/*******************************************
#include <stdio.h>
#include <math.h>

float index2hz(int index, int len, float lower_hz, float upper_hz){
// To map the index to a frequency based on given upper and lower hz (in hz),
// where len is the length of the indices.
// The output represents times of breath per second.
float m, hz;
m = (upper_hz - lower_hz) / (float)(len - 1);
hz = m * (float)index + lower_hz;
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
