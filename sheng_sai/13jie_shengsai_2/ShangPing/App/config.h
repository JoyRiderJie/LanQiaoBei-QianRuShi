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

//声明外部变量
extern unsigned int uiTime7Count[3];
extern unsigned int uiTime7CountFlag[3];
extern unsigned char cRxBuff[1];
extern UART_HandleTypeDef huart1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim7;
extern unsigned int iRxFlag;

//定义一个结构体存储商品信息
struct goods{
	//定义商品数量
	int iGoodsCount;
	//定义商品买的数量
	int iGoodsBuyCount;
	//定义商品价格
	double dGoodsPrice;
};

//定义一个结构体存储EEPROM的信息
struct eepromMsg{
	//记录数量
	unsigned char count;
	//记录价格
	double price;
};

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

/*************************************
* 函数功能：按键工作函数
* 函数参数：无
* 函数返回值：无
*************************************/
void keyPro(void);

/*************************************
* 函数功能：按键工作函数
* 函数参数：无
* 函数返回值：无
*************************************/
void LedDisplay(void);

/*************************************
* 函数功能：PWM输出切换函数
* 函数参数：无
* 函数返回值：无
*************************************/
void pwmWorkState(void);

/*******************************************************
* 函数功能：商品信息初始化
* 函数参数：
*			int iGoodsCount：商品数量
*			double iGoodsBuyCount：商品购买数量
*			double dGoodsPrice：商品价格
* 函数返回值：
*			struct goods*goods：初始化完成的商品信息
*********************************************************/
struct goods*goodsInit(int iGoodsCount,double dGoodsPrice,int iGoodsBuyCount);

/*************************************
* 函数功能：串口数据处理函数
* 函数参数：无
* 函数返回值：无
*************************************/
void usartProcess(void);

/*******************************************************
* 函数功能：EEPROM信息初始化
* 函数参数：
*			int count：数量
*			double price：价格
* 函数返回值：
*			struct eepromMsg*res：初始化完成的EEPROM信息结构体
*********************************************************/
struct eepromMsg*eepromInit(int count,unsigned char price);

/*************************************
* 函数功能：界面显示
* 函数参数：无
* 函数返回值：无
*************************************/
void display(void);

#endif


