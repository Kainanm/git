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

//����1����1���ַ� 
//c:Ҫ���͵��ַ�
void usart1_send_char(u8 c)
{
	while((USART1->SR&0X40)==0);//�ȴ���һ�η������   
	USART1->DR=c;   	
} 
//�������ݸ�����������λ�����(V2.6�汾)
//fun:������. 0XA0~0XAF
//data:���ݻ�����,���28�ֽ�!!
//len:data����Ч���ݸ���
void usart1_niming_report(u8 fun,u8*data,u8 len)
{
	u8 send_buf[32];
	u8 i;
	if(len>28)return;	//���28�ֽ����� 
	send_buf[len+3]=0;	//У��������
	send_buf[0]=0X88;	//֡ͷ
	send_buf[1]=fun;	//������
	send_buf[2]=len;	//���ݳ���
	for(i=0;i<len;i++)send_buf[3+i]=data[i];			//��������
	for(i=0;i<len+3;i++)send_buf[len+3]+=send_buf[i];	//����У���	
	for(i=0;i<len+4;i++)usart1_send_char(send_buf[i]);	//�������ݵ�����1 
}
//���ͼ��ٶȴ��������ݺ�����������
//aacx,aacy,aacz:x,y,z������������ļ��ٶ�ֵ
//gyrox,gyroy,gyroz:x,y,z�������������������ֵ
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
	usart1_niming_report(0XA1,tbuf,12);//�Զ���֡,0XA1
}	
//ͨ������1�ϱ���������̬���ݸ�����
//aacx,aacy,aacz:x,y,z������������ļ��ٶ�ֵ
//gyrox,gyroy,gyroz:x,y,z�������������������ֵ
//roll:�����.��λ0.01�ȡ� -18000 -> 18000 ��Ӧ -180.00  ->  180.00��
//pitch:������.��λ 0.01�ȡ�-9000 - 9000 ��Ӧ -90.00 -> 90.00 ��
//yaw:�����.��λΪ0.1�� 0 -> 3600  ��Ӧ 0 -> 360.0��
void usart1_report_imu(short aacx,short aacy,short aacz,short gyrox,short gyroy,short gyroz,short roll,short pitch,short yaw)
{
	u8 tbuf[28]; 
	u8 i;
	for(i=0;i<28;i++)tbuf[i]=0;//��0
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
	usart1_niming_report(0XAF,tbuf,28);//�ɿ���ʾ֡,0XAF
}  



extern float indata[ROW][COLUMN]; 
extern float breath_rate;
u8 k=0;			// for periodic counting
int main(void)
{		
	
	int i=0, j=0;
	short temp;		
	LED1 = 0;	
	float pitch,roll,yaw; 		//ŷ����	
 	float breath_rate = 0;
	// initiate others
	Stm32_Clock_Init(9);		//ϵͳʱ������
	uart_init(72,500000);		//���ڳ�ʼ��Ϊ500000
	delay_init(72);	   	 		//��ʱ��ʼ�� 
	usmart_dev.init(72);		//��ʼ��USMART
	LED_Init();		  			//��ʼ����LED���ӵ�Ӳ���ӿ�
	KEY_Init();					//��ʼ������
	LCD_Init();			   		//��ʼ��LCD  
	// initiate MPU6050
	MPU_Init();					//��ʼ��MPU6050
 	POINT_COLOR=RED;			//��������Ϊ��ɫ 
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
	POINT_COLOR=BLUE;//��������Ϊ��ɫ 
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
			LCD_ShowChar(30+48,200,'-',16,0);		//��ʾ����
			temp=-temp;		//תΪ����
		}else LCD_ShowChar(30+48,200,' ',16,0);		//ȥ������ 
		LCD_ShowNum(30+48+8,200,temp/100,3,16);		//��ʾ��������	    
		LCD_ShowNum(30+48+40,200,(temp/10)%10,1,16);//��ʾС������ 				
		// Display pitch
		temp=pitch*10;
		if(temp<0){
			LCD_ShowChar(30+48,220,'-',16,0);		//��ʾ����
			temp=-temp;		//תΪ����
		}else LCD_ShowChar(30+48,220,' ',16,0);		//ȥ������ 
		LCD_ShowNum(30+48+8,220,temp/10,3,16);		//��ʾ��������	    
		LCD_ShowNum(30+48+40,220,temp%10,1,16);		//��ʾС������ 				
		// Display roll
		temp=roll*10;
		if(temp<0){
			LCD_ShowChar(30+48,240,'-',16,0);		//��ʾ����
			temp=-temp;		//תΪ����
		}else LCD_ShowChar(30+48,240,' ',16,0);		//ȥ������ 
		LCD_ShowNum(30+48+8,240,temp/10,3,16);		//��ʾ��������	    
		LCD_ShowNum(30+48+40,240,temp%10,1,16);		//��ʾС������ 				
		// Display yaw
		temp=yaw*10;
		if(temp<0){
			LCD_ShowChar(30+48,260,'-',16,0);		//��ʾ����
			temp=-temp;		//תΪ����
		}else LCD_ShowChar(30+48,260,' ',16,0);		//ȥ������ 
		LCD_ShowNum(30+48+8,260,temp/10,3,16);		//��ʾ��������	    
		LCD_ShowNum(30+48+40,260,temp%10,1,16);		//��ʾС������  
	}	
}
