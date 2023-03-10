#ifndef _DS18B20_H
#define _DS18B20_H

#include "main.h"


#define OW_PIN_PORT		GPIOA
#define OW_PIN				GPIO_PIN_6
#define u8 unsigned char
#define u16 unsigned int	


#define OW_DIR_OUT() 	mode_output1()
#define OW_DIR_IN() 	mode_input1()
#define OW_OUT_LOW() 	(HAL_GPIO_WritePin(OW_PIN_PORT, OW_PIN, GPIO_PIN_RESET))
#define OW_GET_IN()  	(HAL_GPIO_ReadPin(OW_PIN_PORT, OW_PIN))
#define OW_SKIP_ROM 		0xCC
#define DS18B20_CONVERT 	0x44
#define DS18B20_READ 		0xBE

//u8 DS18B20_Init(void);			//初始化DS18B20
//short DS18B20_Get_Temp(void);	//获取温度
void ds18b20_init_x(void);
float ds18b20Read(void);



#endif
