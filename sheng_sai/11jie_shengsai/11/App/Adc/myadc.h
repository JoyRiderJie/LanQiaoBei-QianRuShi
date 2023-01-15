#ifndef _MYADC_H
#define _MYADC_H

#include "main.h"

/*******************************************************************
* 函数功能:获取ADC的值
* 函数参数：
* 			ADC_HandleTypeDef *hadc：ADC的通道值
* 函数返回值：
* 			double:转换后的ADC值
*******************************************************************/
double getADC(ADC_HandleTypeDef *hadc);


#endif



