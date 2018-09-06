#include "parametre_define.h"



void update(float sig[ROW][LENGTH], float buff_update[ROW][BUFF_LEN]){
	int i, j, k=0;
	for(i=0;i<ROW;i++){
		for(j=0;j<LENGTH-BUFF_LEN;j++){
			sig[i][j] = sig[i][j+BUFF_LEN];
		}
		for(j=LENGTH-BUFF_LEN;j<LENGTH;j++){
			sig[i][j] = buff_update[i][k]; //k is 0-127
			k++;
		}
	}
}
/*
int main(){
	int i, j;
	float input[ROW][LENGTH];
	for(j=0;j<ROW;j++){
		for(i=0;i<LENGTH;i++){
			input[j][i] = (float)i;
		}
	}
	for(j=0;j<ROW;j++){
		for(i=0;i<BUFF_LEN;i++){
			Buff[j][i] = (float)(i + LENGTH);
		}
	}
	update(Buff, input);
	for(j=0;j<ROW;j++){
		for(i=0;i<LENGTH;i++){
			printf("%.0f ", input[j][i]);
		}
	}
	getchar();
	return 1;
}
*/
// ²âÊÔ³É¹¦
