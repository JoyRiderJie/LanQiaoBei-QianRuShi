#ifndef _CONFIG_H
#define _CONFIG_H

#include "main.h"

#include "stdio.h"
#include "string.h"

#include "lcd.h"
#include "led.h"
#include "key.h"
#include "R47.h"

// 外部声明
extern ADC_HandleTypeDef hadc2;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim16;
extern uint8_t ucRxbuff[4];
extern uint8_t lenBuff,_ucRxbuff[1];
extern UART_HandleTypeDef huart1;
extern uint32_t  RP[4];

// 数据界面需要显示的数据
struct data{
	// 角度数据a
	float a;
	// 角度数据b
	float b;
	// 频率数据
	int f;
	// 角度变化数据ax
	int ax;
	// 角度变化数据bx
	int bx;
	// 触发模式
	char mod;
};

// 参数界面的显示数据
struct para{
	// 角度变化参数Pax
	int Pax;
	// 角度变化参数Pbx
	int Pbx;
	// 频率参数
	int Pf;
};
/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    sysInit
  * @brief   自定义的系统初始化函数
  * @param   None
  * @retval  None
  * @author  黑心萝卜三条杠
  * @Data    2023-03-24
 **/
/* -------------------------------- end -------------------------------- */
void sysInit(void);

/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    sysWork
  * @brief   自定义的系统工作逻辑函数
  * @param   None
  * @retval  None
  * @author  黑心萝卜三条杠
  * @Data    2023-03-24
 **/
/* -------------------------------- end -------------------------------- */
void sysWork(void);


#endif
