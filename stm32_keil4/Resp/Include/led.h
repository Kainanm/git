#ifndef __LED_H
#define __LED_H	 
#include "sys.h"

//LED�˿ڶ���
#define LED0 PBout(5)	// DS0
#define LED1 PEout(5)	// DS1

#define TRIG PEout(6) // To trig extern interrupt on PA0

void LED_Init(void);	//��ʼ��		 				    
#endif

















