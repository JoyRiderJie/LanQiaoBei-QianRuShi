#ifndef CONFIG_H
#define CONFIG_H

#include "main.h"

#include "stdio.h"
#include "string.h"

#include "led.h"
#include "lcd.h"
#include "key.h"

// �ⲿ����
extern TIM_HandleTypeDef htim2,htim3,htim7;
extern UART_HandleTypeDef huart1;
extern uint8_t ucRxbuff[],_ucRxbuff[1],lenBuff;

// LCD��ʾ���ݵĽ����
struct data{
	char name;
	int f;
	int d;
};

/***************************************
* �������ܣ�ϵͳ�Զ����ʼ������
* ������������
* ��������ֵ����
***************************************/
void sysInit(void);

/***************************************
* �������ܣ�ϵͳ�Զ����ʼ������
* ������������
* ��������ֵ����
***************************************/
void sysWork(void);


#endif
