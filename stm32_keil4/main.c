// For mpu6050
#include "sys.h"
#include "delay.h"
#include "usart.h" 
#include "led.h" 		 	 
#include "lcd.h"  
#include "key.h"  
#include "mpu6050.h"
#include "usmart.h"   
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 
// For respiratory rate calculation
#include "parametre_define.h"
#include "resp_rate_cal.h"
// For Extern interrupt
//#include "exti.h" 	
// For watchdog
//#include "wdg.h" 	  

//串口1发送1个字符 
//c:要发送的字符
void usart1_send_char(u8 c)
{
	while((USART1->SR&0X40)==0);//等待上一次发送完毕   
	USART1->DR=c;   	
} 
//传送数据给匿名四轴上位机软件(V2.6版本)
//fun:功能字. 0XA0~0XAF
//data:数据缓存区,最多28字节!!
//len:data区有效数据个数
void usart1_niming_report(u8 fun,u8*data,u8 len)
{
	u8 send_buf[32];
	u8 i;
	if(len>28)return;	//最多28字节数据 
	send_buf[len+3]=0;	//校验数置零
	send_buf[0]=0X88;	//帧头
	send_buf[1]=fun;	//功能字
	send_buf[2]=len;	//数据长度
	for(i=0;i<len;i++)send_buf[3+i]=data[i];			//复制数据
	for(i=0;i<len+3;i++)send_buf[len+3]+=send_buf[i];	//计算校验和	
	for(i=0;i<len+4;i++)usart1_send_char(send_buf[i]);	//发送数据到串口1 
}
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
	usart1_niming_report(0XA1,tbuf,12);//自定义帧,0XA1
}	
//通过串口1上报结算后的姿态数据给电脑
//aacx,aacy,aacz:x,y,z三个方向上面的加速度值
//gyrox,gyroy,gyroz:x,y,z三个方向上面的陀螺仪值
//roll:横滚角.单位0.01度。 -18000 -> 18000 对应 -180.00  ->  180.00度
//pitch:俯仰角.单位 0.01度。-9000 - 9000 对应 -90.00 -> 90.00 度
//yaw:航向角.单位为0.1度 0 -> 3600  对应 0 -> 360.0度
void usart1_report_imu(short aacx,short aacy,short aacz,short gyrox,short gyroy,short gyroz,short roll,short pitch,short yaw)
{
	u8 tbuf[28]; 
	u8 i;
	for(i=0;i<28;i++)tbuf[i]=0;//清0
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
	tbuf[18]=(roll>>8)&0XFF;
	tbuf[19]=roll&0XFF;
	tbuf[20]=(pitch>>8)&0XFF;
	tbuf[21]=pitch&0XFF;
	tbuf[22]=(yaw>>8)&0XFF;
	tbuf[23]=yaw&0XFF;
	usart1_niming_report(0XAF,tbuf,28);//飞控显示帧,0XAF
}  



extern float indata[ROW][COLUMN]; 
extern float breath_rate;
u8 k=0;			// for periodic counting
int main(void)
{		
	
	int i=0, j=0;
	short temp;		
	LED1 = 0;	
	float pitch,roll,yaw; 		//欧拉角	
 	float breath_rate = 0;
	// initiate others
	Stm32_Clock_Init(9);		//系统时钟设置
	uart_init(72,500000);		//串口初始化为500000
	delay_init(72);	   	 		//延时初始化 
	usmart_dev.init(72);		//初始化USMART
	LED_Init();		  			//初始化与LED连接的硬件接口
	KEY_Init();					//初始化按键
	LCD_Init();			   		//初始化LCD  
	// initiate MPU6050
	MPU_Init();					//初始化MPU6050
 	POINT_COLOR=RED;			//设置字体为红色 
	LCD_ShowString(30,50,200,16,16,"STM32");	
	LCD_ShowString(30,70,200,16,16,"MPU6050 TEST");	
	// wait for mpu completing initiation
	while(mpu_dmp_init())
 	{
		LCD_ShowString(30,130,200,16,16,"MPU6050 Initialising");
		delay_ms(200);
		LCD_Fill(30,130,239,130+16,WHITE);
 		delay_ms(200);
	}  
	LCD_ShowString(30,130,200,16,16,"MPU6050 OK");
	POINT_COLOR=BLUE;//设置字体为蓝色 
 	LCD_ShowString(30,200,200,16,16," Resp:    . C");	
 	LCD_ShowString(30,220,200,16,16,"Pitch:    . C");	
 	LCD_ShowString(30,240,200,16,16," Roll:    . C");	 
 	LCD_ShowString(30,260,200,16,16," Yaw :    . C");	 
	
	//EXTIX_Init();        // initiate extern interrupt
	//IWDG_Init(6,4096);    	// initiate the watch dog, TTL = 60000 ms	
	
	while(1){		
		LED1 = 1; // trigger rising-edge interrupt
		if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0){ // fucntion returns 0 when get eularian angles
			k++;
			for(i = 0; i < ROW; i++){\
				for(j = 1; j < LENGTH; j++){				
					indata[i][j-1] = indata[i][j];
				}
			}
			indata[0][LENGTH-1] = pitch;
			indata[1][LENGTH-1] = yaw;
			indata[2][LENGTH-1] = roll;
			LED0 = !LED0;
		}		 
		if(k % (BUFF_LEN) == 0){
			k = 0;
			breath_rate = resp_rate_cal(indata);
			//LED1 = 1; // activate interrupt									
		}   
		
		// Display respiratory rate
		temp = breath_rate*100;
		if(temp<0){
			LCD_ShowChar(30+48,200,'-',16,0);		//显示负号
			temp=-temp;		//转为正数
		}else LCD_ShowChar(30+48,200,' ',16,0);		//去掉负号 
		LCD_ShowNum(30+48+8,200,temp/100,3,16);		//显示整数部分	    
		LCD_ShowNum(30+48+40,200,(temp/10)%10,1,16);//显示小数部分 				
		// Display pitch
		temp=pitch*10;
		if(temp<0){
			LCD_ShowChar(30+48,220,'-',16,0);		//显示负号
			temp=-temp;		//转为正数
		}else LCD_ShowChar(30+48,220,' ',16,0);		//去掉负号 
		LCD_ShowNum(30+48+8,220,temp/10,3,16);		//显示整数部分	    
		LCD_ShowNum(30+48+40,220,temp%10,1,16);		//显示小数部分 				
		// Display roll
		temp=roll*10;
		if(temp<0){
			LCD_ShowChar(30+48,240,'-',16,0);		//显示负号
			temp=-temp;		//转为正数
		}else LCD_ShowChar(30+48,240,' ',16,0);		//去掉负号 
		LCD_ShowNum(30+48+8,240,temp/10,3,16);		//显示整数部分	    
		LCD_ShowNum(30+48+40,240,temp%10,1,16);		//显示小数部分 				
		// Display yaw
		temp=yaw*10;
		if(temp<0){
			LCD_ShowChar(30+48,260,'-',16,0);		//显示负号
			temp=-temp;		//转为正数
		}else LCD_ShowChar(30+48,260,' ',16,0);		//去掉负号 
		LCD_ShowNum(30+48+8,260,temp/10,3,16);		//显示整数部分	    
		LCD_ShowNum(30+48+40,260,temp%10,1,16);		//显示小数部分  
	}	
}
