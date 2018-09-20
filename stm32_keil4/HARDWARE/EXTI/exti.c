#include "exti.h"

//外部中断初始化程序

void EXTIX_Init(void){
	Ex_NVIC_Config(GPIO_A,0,FTIR); 	//falling edge trigger
	MY_NVIC_Init(2,3,EXTI0_IRQn,2);	//抢占2，子优先级3，组2	
}












