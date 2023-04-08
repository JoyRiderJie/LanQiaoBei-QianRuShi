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
  * @brief   �Զ����ϵͳ��ʼ������
  * @param   None
  * @retval  None
  * @author  �����ܲ�������
  
 **/
/* -------------------------------- end -------------------------------- */
void sysInit(void);

/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    sysWork
  * @brief   �Զ����ϵͳ�����߼�����
  * @param   None
  * @retval  None
  * @author  �����ܲ�������
  
 **/
/* -------------------------------- end -------------------------------- */
void sysWork(void);


#endif
