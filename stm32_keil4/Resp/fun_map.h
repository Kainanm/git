#include "parametre_define.h"

static int Nums_of_map = 0; // the length of the mapped signals
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


/*
int  main( ){
float indata[LENGTH]; //改成相应的输入输出数组
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
res = fscanf_s(fpRead, "%f", &indata[i]); //indata[]改成输入的数组名和类型
if (res == EOF)	break;
}
fclose(fpRead);

// --------code start here---------

float lower = 0.2;
float upper = 1.25;
float *ang_picked;

ang_picked = fun_map(indata, LENGTH, lower, upper);



// ----------code end here---------

//output data
FILE *fpWrite=fopen("output.txt","w");
if(fpWrite==NULL){
return 0;
}
for(int i=0;i<Nums_of_map;i++){
fprintf(fpWrite,"%f\n",ang_picked[i]);  //outdata[]改成输出的数组名和类型
}
fclose(fpWrite);

//getchar();
return Nums_of_map;

}
*/
