#ifndef _EEPROM_H
#define _EEPROM_H

#include "i2c - hal.h"

/***************************************
* 函数功能：读取eeprom相应位置的值
* 函数参数：
*			unsigned char ucAddr:读取的地址
* 函数返回值：
*			ucRes：读取到的值
***************************************/
unsigned char readEepromByBit(unsigned char ucAddr);


/***************************************
* 函数功能：向eeprom对应地址写入数据
* 函数参数：
*			unsigned char ucAddr：写入的地址
*			unsigned char ucData：写入的数据
* 函数返回值：无
***************************************/
void writeEepromByBit(unsigned char ucAddr,unsigned char ucData);



void readEepromByPage(unsigned char ucAddr,unsigned char*pucRes);
void writeEepromByPage(unsigned char ucAddr,unsigned char*ucData,unsigned char ucDataSize);


#endif
