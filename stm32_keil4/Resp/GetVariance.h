#include "parametre_define.h"

double GetVariane(float x[], int n){
	//求数组x（具有n个元素）的方差: Var=E(X^2)-[E(X)]^2
	int i;
	double xaver = 0.0; // xaver为x的平均值
	double x_var = 0.0;
	
	for (i = 0; i<n; i++){
		xaver += x[i]; 
	}
	xaver /= (n-1); 
	
	for(i = 0; i<n; i++){
		x_var += (x[i] - xaver) * (x[i] - xaver);
	}
	x_var /= (n+1); //求x的平均、x^2的平均

	return x_var;
}
// 测试成功 输出正确

/*
void main(){
	// 用于测试GetVariance是否正确
	float x[3] = { 1, 2, 3 };
	float s = GetVariane(x, 3);
	printf("%f \n", s);
	//getch();
	//return s;
}
*/
