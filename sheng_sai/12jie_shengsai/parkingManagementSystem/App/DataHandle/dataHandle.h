#ifndef _DATAHANDLE_H
#define _DATAHANDLE_H

#include "string.h"

/****************************************
* �������ܣ���ȡָ��λ���ַ���
* ����������
* 			char*s:������ַ���
*			int iStart:��ʼλ��
*			int iEnd:��ֹλ��
* ��������ֵ��
*			res:���ص���������
****************************************/
void getStringRxBuffDataByLocation(char*s,char*res,int iStart,int iEnd);

/****************************************
* �������ܣ���ָ��λ���ַ���ת������������
* ����������
* 			char*s:������ַ�������
*			int iStart:��ʼλ��
*			int iEnd:��ֹλ��
* ��������ֵ��
*			res:���ص���������
****************************************/
int getIntRxBuffDataByLocation(char*s,int iStart,int iEnd);

/****************************************
* �������ܣ���һ��������ʱ�����ʱ��ת������
* ����������
* 			const unsigned int year0��ת��ʱ�����
*			const unsigned int mon0��ת��ʱ�����
*			const unsigned int day��ת��ʱ�����
*			const unsigned int hour��ת��ʱ���Сʱ
*			const unsigned int min��ת��ʱ��ķ���
* ��������ֵ��
*			res:����ת��������ʱ��
****************************************/
unsigned long myMktime(const unsigned int year0, const unsigned int mon0,
        const unsigned int day, const unsigned int hour,const unsigned int min,
		const unsigned int sec);
		
/****************************************
* �������ܣ��жϴ��ڷ��͹����������Ƿ����Ҫ��
* ����������
* 			const unsigned int year0��ת��ʱ�����
*			const unsigned int mon0��ת��ʱ�����
*			const unsigned int day��ת��ʱ�����
*			const unsigned int hour��ת��ʱ���Сʱ
*			const unsigned int min��ת��ʱ��ķ���
*			const unsigned int sec��ת��ʱ�����
* ��������ֵ��
*			1������Ҫ��
*			0:������Ҫ��
****************************************/
int checkData(const char*type,const int year, const int mon,const  int day, 
	const int hour,const  int min,const  int sec);

#endif
