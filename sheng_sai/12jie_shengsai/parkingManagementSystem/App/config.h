#ifndef _CONFIG_H
#define _CONFIG_H

#include "main.h"
#include "key.h"
#include "led.h"
#include "lcd.h"
#include "list.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include <math.h>
#include "dataHandle.h"


//外部定义变量
extern TIM_HandleTypeDef htim17;
extern UART_HandleTypeDef huart1;
extern uint8_t ucKeyNumber;
extern uint16_t uiTime7Count;
extern uint8_t ucRxbuff[22],_ucRxbuff[1],lenBuff;

/***********************************************
* 函数功能：自定义的系统初始化
* 函数参数：无
* 函数返回值：无
***********************************************/
void sysInit(void);

/***********************************************
* 函数功能：系统工作逻辑函数
* 函数参数：无
* 函数返回值：无
***********************************************/
void sysWork(void);

/****************************************
* 函数功能：修改PWM的占空比
* 函数参数：
*			unsigned int compareDate:PWM的比较值
* 函数返回值：无
****************************************/
void _pwmWorkByFre(unsigned int compareDate);

/****************************************
* 函数功能：汽车信息显示界面
* 函数参数：无
* 函数返回值：无
****************************************/
void _dataMessageDisplay(void);

/****************************************
* 函数功能：汽车信息显示界面
* 函数参数：无
* 函数返回值：无
****************************************/
void _paraMessageDisplay(void);

/***********************************************
* 函数功能：按键工作逻辑函数
* 函数参数：无
* 函数返回值：无
***********************************************/
void _keyPro(void);

/****************************************
* 函数功能：处理接收串口信息
* 函数参数：无
* 函数返回值：无
****************************************/
void _usartMsgProcess(void);

/****************************************
* 函数功能：LED工作函数
* 函数参数：无
* 函数返回值：无
****************************************/
void _LEDDisplay(void);




#endif

