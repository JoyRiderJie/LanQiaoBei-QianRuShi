#include "eeprom.h"

/***************************************
* 函数功能：读取eeprom相应位置的值
* 函数参数：
*			unsigned char ucAddr:读取的地址
* 函数返回值：
*			ucRes：读取到的值
***************************************/
unsigned char readEepromByBit(unsigned char ucAddr)
{
	unsigned char ucRes = 0;
	
	//发送起始信号
	I2CStart();
	//发送设备地址
	I2CSendByte(0xa0);
	//等待应答
	I2CWaitAck();
	//发送读取地址
	I2CSendByte(ucAddr);
	//等待应答
	I2CWaitAck();
	//发送停止信号
	I2CStop();
	
	//发送起始信号
	I2CStart();
	//发送读取数据命令
	I2CSendByte(0xa1);
	//等待应答
	I2CWaitAck();
	//接收数据
	ucRes = I2CReceiveByte();
	//发送应答
	I2CSendNotAck();
	//发送停止信号
	I2CStop();
	
	return ucRes;
}

/***************************************
* 函数功能：向eeprom对应地址写入数据
* 函数参数：
*			unsigned char ucAddr：写入的地址
*			unsigned char ucData：写入的数据
* 函数返回值：无
***************************************/
void writeEepromByBit(unsigned char ucAddr,unsigned char ucData)
{
	//发送起始信号
	I2CStart();
	//发送设备地址
	I2CSendByte(0xa0);
	//等待应答
	I2CWaitAck();
	
	//发送写入地址
	I2CSendByte(ucAddr);
	//发送应答
	I2CSendAck();
	//发送写入数据
	I2CSendByte(ucData);
	//等待应答
	I2CWaitAck();
	
	//发送停止信号
	I2CStop();
}


void readEepromByPage(unsigned char ucAddr,unsigned char*pucRes)
{
	int i=0;
	
	//发送起始信号
	I2CStart();
	//发送设备地址
	I2CSendByte(0xa0);
	//等待应答
	I2CWaitAck();
	//发送读取地址
	I2CSendByte(ucAddr);
	//等待应答
	I2CWaitAck();
	//发送停止信号
	I2CStop();
	
	//发送起始信号
	I2CStart();
	//发送读取数据命令
	I2CSendByte(0xa1);
	//等待应答
	I2CWaitAck();
	for(i=0;i<8;i++)
	{
	//接收数据
	*pucRes = I2CReceiveByte();
	//发送应答
	I2CSendAck();
	}
	//发送停止信号
	I2CStop();
}


void writeEepromByPage(unsigned char ucAddr,unsigned char*ucData,unsigned char ucDataSize)
{
	int i=0;
	
	//发送起始信号
	I2CStart();
	//发送设备地址
	I2CSendByte(0xa0);
	//等待应答
	I2CWaitAck();
	
	//发送写入地址
	I2CSendByte(ucAddr);
	//发送应答
	I2CSendAck();
	for(i=0;i<ucDataSize;i++)
	{
		//发送写入数据
		I2CSendByte(ucData[i]);
		//等待应答
		I2CWaitAck();
	}
	//发送停止信号
	I2CStop();
}




