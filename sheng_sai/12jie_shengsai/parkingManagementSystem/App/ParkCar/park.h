#ifndef _PARK_H
#define _PARK_H

#include "main.h"


//外部定义变量
extern TIM_HandleTypeDef htim7,htim17;
extern UART_HandleTypeDef huart1;
extern TIM_HandleTypeDef htim3;
extern uint8_t ucKeyNumber;
extern uint8_t ucRxbuff[22];
extern uint16_t uiTime7Count;
extern int iRxFlag;


extern struct park*parkMsg; 
	
//车辆信息结构体
struct car{
	//记录停车类型
	char ucType[5];
	//记录车牌号
	char ucCode[5];
	//记录时间
	int year;
	int month;
	int day;
	int hour;
	int minute;
};

//停车场信息结构体
struct park{
	struct car*carMsg[9];
	//记录空余车位数量
	unsigned int uiIdleCount;
	//CNBR类型车辆数量
	unsigned int uiCnbrCount;
	//VNBR类型车辆的数量
	unsigned int uiVnbrCount;
};


#endif
