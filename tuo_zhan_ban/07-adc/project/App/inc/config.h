#ifndef _CONFIG_H
#define _CONFIG_H

#include "seg.h"
//#include "key.h"
#include "ds18b20.h"
#include "dht11.h"
#include "lcd.h"
#include "stdio.h"
//#include "R47.h"
#include "lis302dl.h"
#include  "myAdc.h"

extern ADC_HandleTypeDef hadc2;

/*****************************************************************
* @sysWork����Ŀ����Ҫ���ܳ�ʼ������
* @arg:
*		��
* @return:
*		��
*****************************************************************/
void sysInit(void);

/*****************************************************************
* @sysWork����Ŀ����Ҫ�����߼�����
* @arg:
*		��
* @return:
*		��
*****************************************************************/
void sysWork(void);

#endif

