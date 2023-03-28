#ifndef CONFIG_H
#define CONFIG_H

#include "main.h"

#include "stdio.h"
#include "string.h"

#include "led.h"
#include "lcd.h"
#include "key.h"

// 外部声明
extern TIM_HandleTypeDef htim2,htim3,htim7;
extern UART_HandleTypeDef huart1;
extern uint8_t ucRxbuff[],_ucRxbuff[1],lenBuff;

// LCD显示数据的结果提
struct data{
	char name;
	int f;
	int d;
};

/***************************************
* 函数功能：系统自定义初始化函数
* 函数参数：无
* 函数返回值：无
***************************************/
void sysInit(void);

/***************************************
* 函数功能：系统自定义初始化函数
* 函数参数：无
* 函数返回值：无
***************************************/
void sysWork(void);


#endif
