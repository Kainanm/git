// For mpu6050
#include "sys.h"
#include "delay.h"
#include "led.h" 		 	 
#include "usmart.h" 
#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 

// For respiratory rate calculation
#include "parametre_define.h"
// For Extern interrupt
#include "exti.h" 	

float breath_rate;
float indata[ROW][COLUMN]; 
float resp_rate_cal(float indata[ROW][COLUMN]);
u8 mpu_read(float indata[ROW][COLUMN]);
extern unsigned int k;
// for test
int count_resp_cal = 0;
float data_record[1024];

void EXTI0_IRQHandler(void){
	// falling edge triggering	
	breath_rate = resp_rate_cal(indata);
	// for test
	data_record[count_resp_cal%1024] = breath_rate;
	count_resp_cal++; 
	// test end
	EXTI->PR=1<<0;  //清除LINE0上的中断标志位  
}

// ---- test-----
//串口1发送1个字符 
//c:要发送的字符
//void usart1_send_char(u8 c)
//{
//	while((USART1->SR&0X40)==0);//等待上一次发送完毕   
//	USART1->DR=c;   	
//} 
//---- test end -------

int main(void){	
	//short temp; 
	LED0 = 0;
	LED1 = 0;	
	Stm32_Clock_Init(9);		//系统时钟设置
	delay_init(72);	   	 		//延时初始化 
	usmart_dev.init(72);		//初始化USMART
	LED_Init();		  			//初始化与LED连接的硬件接口
	// -----test begin--------
//	LCD_Init();			   		//初始化LCD
//	POINT_COLOR=BLUE;//设置字体为蓝色 
//	LCD_ShowString(30,220,200,16,16,"Breath:    . per sec");
	// -----test end----------
	MPU_Init();					//初始化MPU6050
		while(mpu_dmp_init()); // wait until mpu initialisation finished
	EXTIX_Init();        // initiate extern interrupt
		
	while(1){	
		mpu_read(indata); 
		// k is a counter, defined in mpu_read.h
		if(k == BUFF_LEN/2){
			LED0 = !LED0;
			delay_ms(10);
			k = 0;
		}
// -----test begin--------
		// display the result on the LCD
//		temp=breath_rate*10;
//		if(temp<0){
//			LCD_ShowChar(30+48,220,'-',16,0);		//显示负号
//			temp=-temp;		//转为正数
//		}else LCD_ShowChar(30+48,220,' ',16,0);		//去掉负号 
//		LCD_ShowNum(30+48+8,220,temp/10,3,16);		//显示整数部分	    
//		LCD_ShowNum(30+48+40,220,temp%10,1,16);		//显示小数部分 
// -----test end--------
	}
}
