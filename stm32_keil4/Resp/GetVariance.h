#include "parametre_define.h"

double GetVariane(float x[], int n){
	//������x������n��Ԫ�أ��ķ���: Var=E(X^2)-[E(X)]^2
	int i;
	double xaver = 0.0; // xaverΪx��ƽ��ֵ
	double x_var = 0.0;
	
	for (i = 0; i<n; i++){
		xaver += x[i]; 
	}
	xaver /= (n-1); 
	
	for(i = 0; i<n; i++){
		x_var += (x[i] - xaver) * (x[i] - xaver);
	}
	x_var /= (n+1); //��x��ƽ����x^2��ƽ��

	return x_var;
}
// ���Գɹ� �����ȷ

/*
void main(){
	// ���ڲ���GetVariance�Ƿ���ȷ
	float x[3] = { 1, 2, 3 };
	float s = GetVariane(x, 3);
	printf("%f \n", s);
	//getch();
	//return s;
}
*/
