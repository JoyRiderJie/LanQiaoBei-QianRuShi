#ifndef _CONFIG_H
#define _CONFIG_H

#include "main.h"

#include "stdio.h"
#include "string.h"

#include "lcd.h"
#include "led.h"
#include "key.h"
#include "R47.h"

// �ⲿ����
extern ADC_HandleTypeDef hadc2;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim16;
extern uint8_t ucRxbuff[4];
extern uint8_t lenBuff,_ucRxbuff[1];
extern UART_HandleTypeDef huart1;
extern uint32_t  RP[4];

// ���ݽ�����Ҫ��ʾ������
struct data{
	// �Ƕ�����a
	float a;
	// �Ƕ�����b
	float b;
	// Ƶ������
	int f;
	// �Ƕȱ仯����ax
	int ax;
	// �Ƕȱ仯����bx
	int bx;
	// ����ģʽ
	char mod;
};

// �����������ʾ����
struct para{
	// �Ƕȱ仯����Pax
	int Pax;
	// �Ƕȱ仯����Pbx
	int Pbx;
	// Ƶ�ʲ���
	int Pf;
};
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
