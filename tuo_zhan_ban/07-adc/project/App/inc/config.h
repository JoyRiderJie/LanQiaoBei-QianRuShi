#ifndef _CONFIG_H
#define _CONFIG_H

#include "seg.h"
//#include "key.h"
#include "ds18b20.h"
#include "dht11.h"
#include "lcd.h"
#include "stdio.h"
//#include "R47.h"
#include "lis302dl.h"
#include  "myAdc.h"

extern ADC_HandleTypeDef hadc2;

/*****************************************************************
* @sysWork：项目的主要功能初始化函数
* @arg:
*		无
* @return:
*		无
*****************************************************************/
void sysInit(void);

/*****************************************************************
* @sysWork：项目的主要功能逻辑函数
* @arg:
*		无
* @return:
*		无
*****************************************************************/
void sysWork(void);

#endif

