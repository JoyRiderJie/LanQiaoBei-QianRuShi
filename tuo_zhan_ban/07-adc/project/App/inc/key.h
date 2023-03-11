#ifndef _KEY_H
#define _KEY_H

#include "main.h"
#include "adc.h"
#include "gpio.h"
	
#define BTN_BUFF_LEN 50

/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    getADC
  * @brief   判断那个按键按下
  * @param   None
  * @retval	 按键的值
  * @author  黑心萝卜三条杠
  * @Data    2023-03-06
  * 1. ...
  * <modify staff>:
  * <data>        :
  * <description> :
  * 2. ...
 **/
/* -------------------------------- end -------------------------------- */
unsigned char scanBtn(void);


#endif
