#ifndef _CONFIG_H
#define _CONFIG_H

#include "main.h"
#include "key.h"
#include "lcd.h"
#include "led.h"
#include "eeprom.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"

//�����ⲿ����
extern unsigned int uiTime7Count[3];
extern unsigned int uiTime7CountFlag[3];
extern unsigned char cRxBuff[1];
extern UART_HandleTypeDef huart1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim7;
extern unsigned int iRxFlag;

//����һ���ṹ��洢��Ʒ��Ϣ
struct goods{
	//������Ʒ����
	int iGoodsCount;
	//������Ʒ�������
	int iGoodsBuyCount;
	//������Ʒ�۸�
	double dGoodsPrice;
};

//����һ���ṹ��洢EEPROM����Ϣ
struct eepromMsg{
	//��¼����
	unsigned char count;
	//��¼�۸�
	double price;
};

/*************************************
* �������ܣ��Զ����ϵͳ��ʼ������
* ������������
* ��������ֵ����
*************************************/
void sysInit(void);

/*************************************
* �������ܣ�ϵͳ���߼�����
* ������������
* ��������ֵ����
*************************************/
void sysWork(void);

/*************************************
* �������ܣ�������������
* ������������
* ��������ֵ����
*************************************/
void keyPro(void);

/*************************************
* �������ܣ�������������
* ������������
* ��������ֵ����
*************************************/
void LedDisplay(void);

/*************************************
* �������ܣ�PWM����л�����
* ������������
* ��������ֵ����
*************************************/
void pwmWorkState(void);

/*******************************************************
* �������ܣ���Ʒ��Ϣ��ʼ��
* ����������
*			int iGoodsCount����Ʒ����
*			double iGoodsBuyCount����Ʒ��������
*			double dGoodsPrice����Ʒ�۸�
* ��������ֵ��
*			struct goods*goods����ʼ����ɵ���Ʒ��Ϣ
*********************************************************/
struct goods*goodsInit(int iGoodsCount,double dGoodsPrice,int iGoodsBuyCount);

/*************************************
* �������ܣ��������ݴ�����
* ������������
* ��������ֵ����
*************************************/
void usartProcess(void);

/*******************************************************
* �������ܣ�EEPROM��Ϣ��ʼ��
* ����������
*			int count������
*			double price���۸�
* ��������ֵ��
*			struct eepromMsg*res����ʼ����ɵ�EEPROM��Ϣ�ṹ��
*********************************************************/
struct eepromMsg*eepromInit(int count,unsigned char price);

/*************************************
* �������ܣ�������ʾ
* ������������
* ��������ֵ����
*************************************/
void display(void);

#endif


