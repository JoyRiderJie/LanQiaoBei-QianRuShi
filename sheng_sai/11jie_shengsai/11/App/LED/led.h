#ifndef __LED_H
#define __LED_H

#include "main.h"
#include "string.h"

//LED的引脚选择
#define LED_GPIO_Pin GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15

enum LEDLOCATION {LED1=GPIO_PIN_8, LED2=GPIO_PIN_9, LED3=GPIO_PIN_10, LED4=GPIO_PIN_11,
				  LED5=GPIO_PIN_12, LED6=GPIO_PIN_13, LED7=GPIO_PIN_14, LED8=GPIO_PIN_15,};
enum LEDSTATE{OFF,ON};
	
/****************************************************
* 函数功能：LED的初始化
* 函数参数：无
* 函数返回值：无
*****************************************************/
void ledInit(void);

/*****************************************************
* 函数功能：改变所有LED的状态
* 函数参数：
*			char LEDSTATE: 0-表示关闭 1-表示打开
* 函数返回值：无
******************************************************/
void changeAllLedByStateNumber(char LEDSTATE);

/*****************************************************
* 函数功能：根据LED的位置打开或者是关闭LED
* 函数参数：
*			char LEDLOCATION:需要操作LED的位置
*			char LEDSTATE:LED改变后的状态 0-表示关闭 1-表示打开
* 函数返回值：无
******************************************************/
void changeLedStateByLocation(uint16_t LEDLOCATION,char LEDSTATE);

/*****************************************************
* 函数功能：根据LED的位置反转LED的状态
* 函数参数：
*			uint16_t LEDLOCATION:需要操作LED的位置
* 函数返回值：无
******************************************************/
void rollbackLedByLocation(uint16_t LEDLOCATION);

#endif
