#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 
#include "mpu6050.h"
#include "parametre_define.h"
#include "led.h"

u8 mpu_dmp_get_data(float *, float *, float *);
float pitch,roll,yaw; 		// Eulerian angles
u8 t = 0;
unsigned int k = 0;
// return 2: mpu data is read but not updated into indata[][]
// return 1: mpu data is read and updated into indata[][]
// return 0: mpu data is not read	
u8 mpu_read(float indata[ROW][COLUMN]){
	int i,j;
	if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0){ 
		// The actual sample rate of MPU6050 is actually 8.33Hz 
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
				LED1 = !LED1;	
				return 1;  
			} else return 2;
	} else return 0;	
}
