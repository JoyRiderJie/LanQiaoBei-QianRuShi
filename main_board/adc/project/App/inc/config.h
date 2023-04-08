#ifndef _CONFIG_H
#define _CONFIG_H

#include "main.h"

#include "stdio.h"
#include "string.h"

#include "..\inc\lcd.h"
#include "..\inc\led.h"
#include "..\inc\key.h"
#include "..\inc\eeprom.h"
#include "..\inc\myadc.h"
#include "..\inc\i2c - hal.h"

// 外部声明
extern UART_HandleTypeDef huart1;
extern uint8_t ucRxbuff[];
extern uint8_t _ucRxbuff[1],lenBuff;
extern ADC_HandleTypeDef hadc2;

// data

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
