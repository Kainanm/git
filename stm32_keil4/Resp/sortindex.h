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

	for (i = 0; i<n; i++)//n�����������ܹ�ɨ��n-1��  
	{
		for (j = 0; j<n - i - 1; j++)//ÿһ��ɨ�赽a[n-i-2]��a[n-i-1]�Ƚ�Ϊֹ����  
		{
			if (a[j]<a[j + 1])//��һλ����ǰһλ����Ļ����ͽ�����������λ��(����)
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

// ���Գɹ���������Ӵ�С��Ӧ���±�
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
