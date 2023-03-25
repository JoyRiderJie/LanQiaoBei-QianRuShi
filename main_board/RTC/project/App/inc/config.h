#ifndef _CONFIG_H
#define _CONFIG_H

#include "stm32g4xx_hal.h"

#include "lcd.h"
#include "led.h"

#include "stdio.h"

/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    sysInit
  * @brief   自定义系统外设初始化函数
  * @param   None
  * @retval
  * @author  黑心萝卜三条杠
  * @Data    2023-03-19
  * 1. ...
  * <modify staff>:
  * <data>        :
  * <description> :
  * 2. ...
 **/
/* -------------------------------- end -------------------------------- */
void sysInit(void);

/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    sysWork
  * @brief   自定义系统外设工作函数
  * @param   None
  * @retval
  * @author  黑心萝卜三条杠
  * @Data    2023-03-19
  * 1. ...
  * <modify staff>:
  * <data>        :
  * <description> :
  * 2. ...
 **/
/* -------------------------------- end -------------------------------- */
void sysWork(void);


#endif 

