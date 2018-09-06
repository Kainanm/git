/*************
欧拉角方差数数组按方差从大到小输出下标, 数组大小为3，即n = 3
输入数组为按顺序排列的欧拉角的方差{Pitch_var, Yaw_var, Roll_var}
下标0：Pitch方差；1：Yaw方差；2：Roll方差
输出：loc = {max, mid, min}
*************/


#include "parametre_define.h"

int AngLocDes(double angle_var[], int n){
	// n is the length of angle_var, AKA the number of variances
	int i;
	//int	*loc;
	double max, min; 
	int index_max, index_mid, index_min;
	
	//loc = (int*)malloc(sizeof(int)*n);
	
    max = angle_var[0]; 
    min = angle_var[0];

	for (i = 1; i<n; i++){
		if (angle_var[i] >= max){
			max = angle_var[i];
			index_max = i;
		}
		if (angle_var[i] <= min){
			min = angle_var[i];
			index_min = i;
		}
	}
	
	index_mid = 3 - index_min - index_max; //三个下标之和为3 = 0 + 1 + 2
	return index_max;
}
// 测试成功，输出为数组loc

/*void main(){
	int i, n = 3;
	float all[3] = { -22.8, 2, 98.65 };
	int *loc;
	for (i = 0; i<n; i++)
		printf("%f  ", all[i]);
	printf("\n");
	loc = AngLocDes(all, n);//输出
	for (i = 0; i < n; i++)
		printf("%3d", loc[i]);
	printf("\n");
}*/
