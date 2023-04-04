#ifndef _EEPROM_H
#define _EEPROM_H

#include "i2c - hal.h"

/***************************************
* �������ܣ���ȡeeprom��Ӧλ�õ�ֵ
* ����������
*			unsigned char ucAddr:��ȡ�ĵ�ַ
* ��������ֵ��
*			ucRes����ȡ����ֵ
***************************************/
unsigned char readEepromByBit(unsigned char ucAddr);


/***************************************
* �������ܣ���eeprom��Ӧ��ַд������
* ����������
*			unsigned char ucAddr��д��ĵ�ַ
*			unsigned char ucData��д�������
* ��������ֵ����
***************************************/
void writeEepromByBit(unsigned char ucAddr,unsigned char ucData);



void readEepromByPage(unsigned char ucAddr,unsigned char*pucRes);
void writeEepromByPage(unsigned char ucAddr,unsigned char*ucData,unsigned char ucDataSize);


#endif
