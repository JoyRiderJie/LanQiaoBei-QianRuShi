#include "myadc.h"

//获取adc的值
double adcValue = 0;

/*******************************************************************
* 函数功能:获取ADC的值
* 函数参数：
*			ADC_HandleTypeDef *pin：ADC的通道值
* 函数返回值：
*			double:转换后的ADC值
*******************************************************************/
double getADC(ADC_HandleTypeDef *hadc)
{
	unsigned int value = 0;
	
	//开启转换ADC并且获取值
	HAL_ADC_Start(hadc);
	value = HAL_ADC_GetValue(hadc);
	
	//ADC值的转换 3.3V是电压  4096是ADC的精度为12位也就是2^12=4096
	return value*3.3/4096;
}

/*******************************************************************
* 函数功能:获取ADC多个通道的值
* 函数参数：
*			ADC_HandleTypeDef *hadc：ADC
*			double*data：保存ADC的值
*			int n：ADC通道的个数	
* 函数返回值：无
*******************************************************************/
void getManyADC(ADC_HandleTypeDef *hadc,double*data,int n)
{
	int i=0;
	for(i=0;i<n;i++)
	{
		HAL_ADC_Start(hadc);
		//等待转换完成，第二个参数表示超时时间，单位ms
		HAL_ADC_PollForConversion (hadc,50);
		data[i] = ((double)HAL_ADC_GetValue(hadc)/4096)*3.3;
	}
	HAL_ADC_Stop(hadc);
}





