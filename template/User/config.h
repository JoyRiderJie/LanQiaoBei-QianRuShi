#ifndef __CONFIG_H
#define __CONFIG_H

#include "main.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "led.h"
#include "lcd.h"
#include "key.h"
#include "usart.h"

//记录sysTick定时器（毫秒级）执行的次数
extern uint16_t uTick_ms;

/**********************************************
* 函数功能：系统初始化函数
* 函数参数：无
* 函数返回值：无
**********************************************/
void sysInit(void);

/**********************************************
* 函数功能：系统主逻辑函数  (由于主函数中含while(1) 此处不在含有while(1))
* 函数参数：无
* 函数返回值：无
***********************************************/
void sysWork(void);

#endif
