#include "exti.h"

//�ⲿ�жϳ�ʼ������

void EXTIX_Init(void){
	Ex_NVIC_Config(GPIO_A,0,RTIR); 	//�����ش���
	MY_NVIC_Init(2,3,EXTI0_IRQn,2);	//��ռ2�������ȼ�3����2	
}












