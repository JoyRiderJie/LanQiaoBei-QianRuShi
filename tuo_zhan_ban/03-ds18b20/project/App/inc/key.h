#ifndef _KEY_H
#define _KEY_H

#include "main.h"
#include "adc.h"
#include "gpio.h"

#define u8 unsigned char
#define u16 unsigned int
	
#define BTN_BUFF_LEN 50

/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    getADC
  * @brief   �ж��Ǹ���������
  * @param   None
  * @retval	 ������ֵ
  * @author  �����ܲ�������
  * @Data    2023-03-06
  * 1. ...
  * <modify staff>:
  * <data>        :
  * <description> :
  * 2. ...
 **/
/* -------------------------------- end -------------------------------- */
u8 scanBtn(void);


#endif
