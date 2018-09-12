// For mpu6050
#include "sys.h"
#include "delay.h"
#include "led.h" 		 	 
#include "mpu6050.h"
#include "usmart.h"   
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 
// For respiratory rate calculation
#include "parametre_define.h"
#include "resp_rate_cal.h"
// For Extern interrupt
#include "exti.h" 	


//发送加速度传感器数据和陀螺仪数据
//aacx,aacy,aacz:x,y,z三个方向上面的加速度值
//gyrox,gyroy,gyroz:x,y,z三个方向上面的陀螺仪值
void mpu6050_send_data(short aacx,short aacy,short aacz,short gyrox,short gyroy,short gyroz)
{
	u8 tbuf[12]; 
	tbuf[0]=(aacx>>8)&0XFF;
	tbuf[1]=aacx&0XFF;
	tbuf[2]=(aacy>>8)&0XFF;
	tbuf[3]=aacy&0XFF;
	tbuf[4]=(aacz>>8)&0XFF;
	tbuf[5]=aacz&0XFF; 
	tbuf[6]=(gyrox>>8)&0XFF;
	tbuf[7]=gyrox&0XFF;
	tbuf[8]=(gyroy>>8)&0XFF;
	tbuf[9]=gyroy&0XFF;
	tbuf[10]=(gyroz>>8)&0XFF;
	tbuf[11]=gyroz&0XFF;
}	


float breath_rate;
float indata[ROW][COLUMN]; 
void EXTI0_IRQHandler(void){
	//上升沿触发
	delay_ms(10);	
	if(LED1 == 1){
	//breath_rate = resp_rate_cal(indata);
		LED0 = !LED0;
	}
	EXTI->PR=1<<0;  //清除LINE0上的中断标志位  
}

int main(void){		
	unsigned int k = 0;			// for periodic counting of resp_rate_cal
	u8 t = 0;                 // for periodic reading of MPU6050
	int i = 0, j = 0;
	LED0 = 0;
	LED1 = 0;	
	float pitch,roll,yaw; 		//欧拉角	
	Stm32_Clock_Init(9);		//系统时钟设置
	delay_init(72);	   	 		//延时初始化 
	usmart_dev.init(72);		//初始化USMART
	LED_Init();		  			//初始化与LED连接的硬件接口
	MPU_Init();					//初始化MPU6050
		while(mpu_dmp_init()); // wait until mpu initialisation finished
	EXTIX_Init();        // initiate extern interrupt
		
	while(1){		
		if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0){ 
			// The actual sample rate of MPU6050 is actually 8.33Hz 
			// (= 100/((u8)100/SAMP_RATE)), in parametre_define.h
			t++;
			if(t == MPU_DVDR){
				t = 0;
				for(i = 0; i < ROW; i++){
					for(j = 1; j < LENGTH; j++){				
						indata[i][j-1] = indata[i][j];
					}
				}
				indata[0][LENGTH-1] = pitch;
				indata[1][LENGTH-1] = yaw;
				indata[2][LENGTH-1] = roll;
				k++;
			}
		}	
		
		if(k == BUFF_LEN/2){
			LED1 = !LED1;
			//LED1 = 1; // activate interrupt	by rising-edge trigger	
			k = 0;				
		}   		
	}	
}
