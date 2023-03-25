#ifndef _CONFIG_H
#define _CONFIG_H

#include "main.h"
#include "key.h"
#include "lcd.h"
#include "led.h"
#include "eeprom.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "myadc.h"

// 记录PA4 PA5的结构体
struct paContent{
	// 名字序号
	int nameID;
	// 次数
	int n;
	// 最大值
	double a;
	// 最小值
	double t;
	// 平均值
	double h;
	// 记录历史数据
	double data[100];
};

extern uint8_t ucRxbuff[4];
extern uint8_t _ucRxbuff;
extern UART_HandleTypeDef huart1;
extern TIM_HandleTypeDef htim2,htim7,htim17;
extern ADC_HandleTypeDef hadc2;
extern struct uart*usart1;
extern unsigned int uiTime7Count;
extern int P1;
extern uint8_t LED3TimeFlag;

/*************************************
* 函数功能：自定义的系统初始化函数
* 函数参数：无
* 函数返回值：无
*************************************/
void sysInit(void);

/*************************************
* 函数功能：系统的逻辑函数
* 函数参数：无
* 函数返回值：无
*************************************/
void sysWork(void);

/***************************************************
* 函数功能：设置LCD显示模式
* 函数参数：
*			int mod:显示模式 0-正向显示 1-翻转显示
* 函数返回值：无
***************************************************/
void setDisplayMod(int mod);

/***************************************************
* 函数功能：LCD显示数据
* 函数参数：
*			int mod:显示数据模式 0-数据界面 1-翻转显示 2-记录界面
* 函数返回值：无
***************************************************/
void displayText(int mod);

/**************************************************
* 函数功能：LED工作函数
* 函数参数：
*		   int mod1：LED3的工作模式
*		   int mod2：LED4的工作模式
* 函数返回值：无
***************************************************/
void ledPro(int mod1,int mod2);

/**************************************************
* 函数功能：串口工作函数
* 函数参数：无
* 函数返回值：无
***************************************************/
void usartPro(void);

/**************************************************
* 函数功能：电压工作函数
* 函数参数：无
* 函数返回值：无
***************************************************/
void elePro(void);

/**************************************************
* 函数功能：频率工作函数
* 函数参数：无
* 函数返回值：无
***************************************************/
void FrePro(int mod);

/***************************************************
* 函数功能：按键的处理函数
* 函数参数：无
* 函数返回值：无
***************************************************/
void keyPro(void);

#endif


