#ifndef _CONFIG_H
#define _CONFIG_H

#include "main.h"

#include "stdio.h"
#include "string.h"

#include "lcd.h"
#include "led.h"
#include "key.h"
#include "myadc.h"

// �ⲿ����
extern UART_HandleTypeDef huart1;
extern ADC_HandleTypeDef hadc2;
extern uint8_t ucRxbuff[];
extern uint8_t _ucRxbuff[1],lenBuff;
extern TIM_HandleTypeDef htim3;

// data

/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    sysInit
  * @brief   �Զ����ϵͳ��ʼ������
  * @param   None
  * @retval  None
  * @author  �����ܲ�������
  * @Data    2023-03-24
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
  * @Data    2023-03-24
 **/
/* -------------------------------- end -------------------------------- */
void sysWork(void);


#endif
