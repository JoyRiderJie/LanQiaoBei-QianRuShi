#ifndef _MYADC_H
#define _MYADC_H

#include "main.h"

//��ȡadc��ֵ
extern double adcValue;


/*******************************************************************
* ��������:��ȡADC��ֵ
* ����������
*			ADC_HandleTypeDef *pin��ADC��ͨ��ֵ
* ��������ֵ��
*			double:ת�����ADCֵ
*******************************************************************/
double getADC(ADC_HandleTypeDef *pin);

/*******************************************************************
* ��������:��ȡADC���ͨ����ֵ
* ����������
*			ADC_HandleTypeDef *hadc��ADC
*			double*data������ADC��ֵ
*			int n��ADCͨ���ĸ���	
* ��������ֵ����
*******************************************************************/
void getManyADC(ADC_HandleTypeDef *hadc,double*data,int n);

#endif

