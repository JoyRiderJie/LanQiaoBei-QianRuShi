#ifndef _CONFIG_H
#define _CONFIG_H

#include "main.h"
#include "stdio.h"
#include "key.h"
#include "led.h"
#include "lcd.h"
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

#define EVENT1 (0x01 << 0)
#define EVENT2 (0x01 << 1)
#define EVENT3 (0x01 << 2)
#define EVENT4 (0x01 << 3)
#define EVENT5 (0x01 << 4)
#define EVENT6 (0x01 << 5)
#define EVENT7 (0x01 << 6)
#define EVENT8 (0x01 << 7)

// 消息队列发送与接收结构体
struct passwdMsg{
	int id;
	char passwdTxt[4];
};

//记录目前LCD处于的界面 0-密码输入界面  1-显示输出界面
extern int lcd_view_mod  ;
//密码输入界面显示的数据
extern char psd_txt[20];
//输出状态界面显示数据
extern char sta_txt[20];

//* 存储串口1接收的数据
extern uint8_t Rxbuff[7];

//密码锁密码 注意 这里一定一定要加上字符串结束符'\0' 否则会出现字符串拼接的情况
extern  char passwd[4] ;
//输入的密码
extern  char passwd_test[4];
//记录密码输错次数
extern unsigned int passwd_wrong_count  ;
//记录是否重头开始输入密码 0-是重头开始输入且没开始输入的  1-不是重头开始输入密码
extern char passwd_flag  ;

extern osEventFlagsId_t flagEvent02Handle;
extern osEventFlagsId_t ledEvent01Handle;

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

/***********************************************
* 函数功能：显示输出界面
* 函数参数：无
* 函数返回值：无
***********************************************/
void STAViewDisplay(void);

/***********************************************
* 函数功能：显示密码输入界面
* 函数参数：无
* 函数返回值：无
***********************************************/
void PSDViewDisplay(void);

/****************************************
* 函数功能：修改PWM的频率工作
* 函数参数：
*			unsigned int autoreloadDate:重装载值
*			unsigned int compareDate:PWM的比较值
* 函数返回值：无
****************************************/
void pwmWorkByFre(unsigned int autoreloadDate,unsigned int compareDate);

/********************************************
* 函数功能：
* 函数参数:无
* 函数返回值:无
********************************************/
void keyProc(void);

/********************************************
* 函数功能：分析串口接收数据并且合理设置密码
* 函数参数:无
* 函数返回值:无
********************************************/
void getPasswdByUsart(void);
/***********************************************
* 函数功能：LED的相关显示
* 函数参数：无
* 函数返回值：无
***********************************************/
void ledDisplay(void);



#endif

