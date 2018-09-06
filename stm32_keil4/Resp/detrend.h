#include "parametre_define.h"



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
//���Գɹ�������matlab��detrend�ڿ�ʼ������ʱ�в��죬���Լ�д��fun_detrend.m�����ͬ
//���ǵ������յĺ���������Ӱ�첻�󣬲���ɺ���

