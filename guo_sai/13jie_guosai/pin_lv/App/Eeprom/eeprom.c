#include "eeprom.h"

/***************************************
* �������ܣ���ȡeeprom��Ӧλ�õ�ֵ
* ����������
*			unsigned char ucAddr:��ȡ�ĵ�ַ
* ��������ֵ��
*			ucRes����ȡ����ֵ
***************************************/
unsigned char readEepromByBit(unsigned char ucAddr)
{
	unsigned char ucRes = 0;
	
	//������ʼ�ź�
	I2CStart();
	//�����豸��ַ
	I2CSendByte(0xa0);
	//�ȴ�Ӧ��
	I2CWaitAck();
	//���Ͷ�ȡ��ַ
	I2CSendByte(ucAddr);
	//�ȴ�Ӧ��
	I2CWaitAck();
	//����ֹͣ�ź�
	I2CStop();
	
	//������ʼ�ź�
	I2CStart();
	//���Ͷ�ȡ��������
	I2CSendByte(0xa1);
	//�ȴ�Ӧ��
	I2CWaitAck();
	//��������
	ucRes = I2CReceiveByte();
	//����Ӧ��
	I2CSendNotAck();
	//����ֹͣ�ź�
	I2CStop();
	
	return ucRes;
}

/***************************************
* �������ܣ���eeprom��Ӧ��ַд������
* ����������
*			unsigned char ucAddr��д��ĵ�ַ
*			unsigned char ucData��д�������
* ��������ֵ����
***************************************/
void writeEepromByBit(unsigned char ucAddr,unsigned char ucData)
{
	//������ʼ�ź�
	I2CStart();
	//�����豸��ַ
	I2CSendByte(0xa0);
	//�ȴ�Ӧ��
	I2CWaitAck();
	
	//����д���ַ
	I2CSendByte(ucAddr);
	//����Ӧ��
	I2CSendAck();
	//����д������
	I2CSendByte(ucData);
	//�ȴ�Ӧ��
	I2CWaitAck();
	
	//����ֹͣ�ź�
	I2CStop();
}


void readEepromByPage(unsigned char ucAddr,unsigned char*pucRes)
{
	int i=0;
	
	//������ʼ�ź�
	I2CStart();
	//�����豸��ַ
	I2CSendByte(0xa0);
	//�ȴ�Ӧ��
	I2CWaitAck();
	//���Ͷ�ȡ��ַ
	I2CSendByte(ucAddr);
	//�ȴ�Ӧ��
	I2CWaitAck();
	//����ֹͣ�ź�
	I2CStop();
	
	//������ʼ�ź�
	I2CStart();
	//���Ͷ�ȡ��������
	I2CSendByte(0xa1);
	//�ȴ�Ӧ��
	I2CWaitAck();
	for(i=0;i<8;i++)
	{
	//��������
	*pucRes = I2CReceiveByte();
	//����Ӧ��
	I2CSendAck();
	}
	//����ֹͣ�ź�
	I2CStop();
}


void writeEepromByPage(unsigned char ucAddr,unsigned char*ucData,unsigned char ucDataSize)
{
	int i=0;
	
	//������ʼ�ź�
	I2CStart();
	//�����豸��ַ
	I2CSendByte(0xa0);
	//�ȴ�Ӧ��
	I2CWaitAck();
	
	//����д���ַ
	I2CSendByte(ucAddr);
	//����Ӧ��
	I2CSendAck();
	for(i=0;i<ucDataSize;i++)
	{
		//����д������
		I2CSendByte(ucData[i]);
		//�ȴ�Ӧ��
		I2CWaitAck();
	}
	//����ֹͣ�ź�
	I2CStop();
}




