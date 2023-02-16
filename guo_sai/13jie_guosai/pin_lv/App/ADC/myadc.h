#ifndef _MYADC_H
#define _MYADC_H

#include "main.h"

//获取adc的值
extern double adcValue;


/*******************************************************************
* 函数功能:获取ADC的值
* 函数参数：
*			ADC_HandleTypeDef *pin：ADC的通道值
* 函数返回值：
*			double:转换后的ADC值
*******************************************************************/
double getADC(ADC_HandleTypeDef *pin);

/*******************************************************************
* 函数功能:获取ADC多个通道的值
* 函数参数：
*			ADC_HandleTypeDef *hadc：ADC
*			double*data：保存ADC的值
*			int n：ADC通道的个数	
* 函数返回值：无
*******************************************************************/
void getManyADC(ADC_HandleTypeDef *hadc,double*data,int n);

#endif

