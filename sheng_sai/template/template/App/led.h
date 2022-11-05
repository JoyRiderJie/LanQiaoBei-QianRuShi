#ifndef __LED_H
#define __LED_H

#include "main.h"

//LED的引脚选择
#define LED_GPIO_Pin GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15

/****************************************************
* 函数功能：LED的初始化
* 函数参数：无
* 函数返回值：无
*****************************************************/
void ledInit(void);


#endif
