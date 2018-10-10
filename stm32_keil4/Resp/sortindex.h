#include "parametre_define.h"

// same as sort() in matlab
int sortIndex(float a[], int p_index[], int n){
	// a is input and p_index is output
	// n = MAP_LENGTH
	int i, j;
	int temp1;
	float temp2;

	

	for (i = 0; i<n; i++){
		p_index[i] = i;
	}

	for (i = 0; i<n; i++)//n个数的数列总共扫描n-1次  
	{
		for (j = 0; j<n - i - 1; j++)//每一趟扫描到a[n-i-2]与a[n-i-1]比较为止结束  
		{
			if (a[j]<a[j + 1])//后一位数比前一位数大的话，就交换两个数的位置(降序)
			{
				temp2 = a[j + 1];
				a[j + 1] = a[j];
				a[j] = temp2;

				temp1 = p_index[j];
				p_index[j] = p_index[j + 1];
				p_index[j + 1] = temp1;
			}
		}
	}

	return 1;
	//free(p_index);
}

// 测试成功，可输出从大到小对应的下标
/*void main(){
	int i;
	float a[5] = { 1.8, 7.2, 10.31, 2.8, 9.6 };
	int *p_index;
	for (i = 0; i < 5; i++)
		printf("%f ", a[i]);
	printf("\n");
	p_index = sortindex(a, 5);
	for (i = 0; i < 5; i++)
		printf("%d ", p_index[i]);
	printf("\n");
}*/
