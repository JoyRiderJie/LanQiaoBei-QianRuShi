#ifndef _PARK_H
#define _PARK_H

#include "main.h"


//�ⲿ�������
extern TIM_HandleTypeDef htim7,htim17;
extern UART_HandleTypeDef huart1;
extern TIM_HandleTypeDef htim3;
extern uint8_t ucKeyNumber;
extern uint8_t ucRxbuff[22];
extern uint16_t uiTime7Count;
extern int iRxFlag;


extern struct park*parkMsg; 
	
//������Ϣ�ṹ��
struct car{
	//��¼ͣ������
	char ucType[5];
	//��¼���ƺ�
	char ucCode[5];
	//��¼ʱ��
	int year;
	int month;
	int day;
	int hour;
	int minute;
};

//ͣ������Ϣ�ṹ��
struct park{
	struct car*carMsg[9];
	//��¼���೵λ����
	unsigned int uiIdleCount;
	//CNBR���ͳ�������
	unsigned int uiCnbrCount;
	//VNBR���ͳ���������
	unsigned int uiVnbrCount;
};


#endif
