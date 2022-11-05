#include "config.h"
#include "main.h"
#include "stdlib.h"
#include "stdio.h"

/************************该.c文件用于写系统初始化以及系统初始化后的主逻辑***********************************/

//*存储LCD显示数据的变量
unsigned char lcdDisplayData[20];

//记录syswork函数执行的次数
u8 sysWorkCount = 0;

/**********************************************
* 函数功能：系统初始化函数
* 函数参数：无
* 函数返回值：无
**********************************************/
void sysInit(void)
{
	//LCD初始化
	lcdInit();

	//LED的初始化
	ledInit();
}

/**********************************************
* 函数功能：系统主逻辑函数(由于主函数中含while(1) 此处不在含有while(1))
* 函数参数：无
* 函数返回值：无
***********************************************/
void sysWork(void)
{
	//注意每次LCD写入数据后 操作LED时需要重置所有LED的转态 不然所有LED都会点亮
	changeAllLedByStateNumber(0);
	//开启部分LED
	turnOnLedByLocation(GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_12|GPIO_PIN_11,0); 
	//毫秒级延时 下面这部分延时是延时1s
	HAL_Delay(1000);
	turnOnLedByLocation(GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_12|GPIO_PIN_11,1); 
	HAL_Delay(1000);

	//LCD显示"hello word!"
	LCD_DisplayStringLine(Line1,"hello word!");
	//LCD记录sysWorkCount的数值
	sprintf((char*)lcdDisplayData,"WorkCount is %d",(int)sysWorkCount);
	LCD_DisplayStringLine(Line2,lcdDisplayData);
	sysWorkCount++;
}





