#ifndef _CONFIG_H
#define _CONFIG_H

#include "main.h"

#include "stdio.h"
#include "string.h"

#include "lcd.h"
#include "led.h"
#include "key.h"
#include "myadc.h"

extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern ADC_HandleTypeDef hadc2;

/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    sysInit
  * @brief   自定义的系统初始化函数
  * @param   None
  * @retval  None
  * @author  黑心萝卜三条杠
  
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
  
 **/
/* -------------------------------- end -------------------------------- */
void sysWork(void);


#endif
