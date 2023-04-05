#include "myadc.h"


/*******************************************************************
* ��������:��ȡADC��ֵ
* ����������
* 			ADC_HandleTypeDef *hadc��ADC��ͨ��ֵ
* ��������ֵ��
* 			double:ת�����ADCֵ
*******************************************************************/
double getADC(ADC_HandleTypeDef *hadc)
{
	unsigned int value = 0;
	//����ת��ADC���һ�ȡֵ
	HAL_ADC_Start(hadc);
	while(LL_ADC_IsActiveFlag_EOC(ADC2) == 0) 
		;
	value = HAL_ADC_GetValue(hadc);
	
	//ADCֵ��ת�� 3.3V�ǵ�ѹ 4096��ADC�ľ���Ϊ12λҲ����2^12=4096
	return value*3.3/4096;
}

#if 0
double getADC(ADC_HandleTypeDef *hadc)
{
	unsigned int value = 0,i = 0;
	//����ת��ADC���һ�ȡֵ
	HAL_ADC_Start(hadc);
	while(LL_ADC_IsActiveFlag_EOC(ADC2) == 0) ;
	// HAL_ADC_PollForConversion(hadc,50);
//	for(i=0;i<10;++i)
		value += HAL_ADC_GetValue(hadc);
	
	//ADCֵ��ת�� 3.3V�ǵ�ѹ 4096��ADC�ľ���Ϊ12λҲ����2^12=4096
	return value*3.3/4096;
//	return value/10*3.3/4096;
}
#endif


