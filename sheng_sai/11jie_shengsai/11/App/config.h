#ifndef _CONFIG_H
#define _CONFIG_H

#include "main.h"
#include "key.h"
#include "lcd.h"
#include "led.h"
#include "myadc.h"

#include "stdlib.h"
#include "stdio.h"
#include "string.h"

//定义一个显示数据的结构体
struct displayData{
	//定义显示的界面
	int page;	
	//记录模式
	int mode;
	//记录电压值
	double vValue;
	//记录PA6占空比
	int frdP6;
	//记录PA7占空比
	int frdP7;
};


//声明外部定义的变量
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim17;
extern ADC_HandleTypeDef hadc2;;
/***********************************************
* 函数功能：自定义的系统初始化函数
* 函数参数：无
* 函数返回值：无
***********************************************/
void sysInit(void);

/***********************************************
* 函数功能：自定义的系统工作函数
* 函数参数：无
* 函数返回值：无
***********************************************/
void sysWork(void);

/***********************************************
* 函数功能：显示函数
* 函数参数：无
* 函数返回值：无
***********************************************/
void display(void);

/***********************************************
* 函数功能：更改PWM占空比函数
* 函数参数：无
* 函数返回值：无
***********************************************/
void changePwmFrd(void);

/***********************************************
* 函数功能：LED工作函数
* 函数参数：无
* 函数返回值：无
***********************************************/
void ledWork(void);


#endif
