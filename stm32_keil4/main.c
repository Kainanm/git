// For mpu6050
#include "sys.h"
#include "delay.h"
#include "led.h" 		 	 
#include "usmart.h"   
#include "mpu_read.h"
// For respiratory rate calculation
#include "parametre_define.h"

// For Extern interrupt
#include "exti.h" 	

float breath_rate;
float indata[ROW][COLUMN]; 
// float pitch,roll,yaw; 		// Eulerian angles	
//u8 t = 0; 
//u8 k = 0;			// for periodic counting of resp_rate_cal

float resp_rate_cal(float indata[ROW][COLUMN]);

void EXTI0_IRQHandler(void){
	// falling edge triggering	
	breath_rate = resp_rate_cal(indata);
	EXTI->PR=1<<0;  //���LINE0�ϵ��жϱ�־λ  
}

int main(void){	
 // u8 mpu_status;
	LED0 = 0;
	LED1 = 0;	
	Stm32_Clock_Init(9);		//ϵͳʱ������
	delay_init(72);	   	 		//��ʱ��ʼ�� 
	usmart_dev.init(72);		//��ʼ��USMART
	LED_Init();		  			//��ʼ����LED���ӵ�Ӳ���ӿ�
	MPU_Init();					//��ʼ��MPU6050
		while(mpu_dmp_init()); // wait until mpu initialisation finished
	EXTIX_Init();        // initiate extern interrupt
		
	while(1){	
		mpu_read(indata); 
		// k is a counter, defined in mpu_read.h
		if(k == BUFF_LEN/2){
			LED0 = !LED0;
			k = 0;
		}
	}
}
