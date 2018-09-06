/*************
ŷ���Ƿ��������鰴����Ӵ�С����±�, �����СΪ3����n = 3
��������Ϊ��˳�����е�ŷ���ǵķ���{Pitch_var, Yaw_var, Roll_var}
�±�0��Pitch���1��Yaw���2��Roll����
�����loc = {max, mid, min}
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
	
	index_mid = 3 - index_min - index_max; //�����±�֮��Ϊ3 = 0 + 1 + 2
	return index_max;
}
// ���Գɹ������Ϊ����loc

/*void main(){
	int i, n = 3;
	float all[3] = { -22.8, 2, 98.65 };
	int *loc;
	for (i = 0; i<n; i++)
		printf("%f  ", all[i]);
	printf("\n");
	loc = AngLocDes(all, n);//���
	for (i = 0; i < n; i++)
		printf("%3d", loc[i]);
	printf("\n");
}*/
