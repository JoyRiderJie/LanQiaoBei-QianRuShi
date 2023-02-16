#include "myadc.h"

//��ȡadc��ֵ
double adcValue = 0;

/*******************************************************************
* ��������:��ȡADC��ֵ
* ����������
*			ADC_HandleTypeDef *pin��ADC��ͨ��ֵ
* ��������ֵ��
*			double:ת�����ADCֵ
*******************************************************************/
double getADC(ADC_HandleTypeDef *hadc)
{
	unsigned int value = 0;
	
	//����ת��ADC���һ�ȡֵ
	HAL_ADC_Start(hadc);
	value = HAL_ADC_GetValue(hadc);
	
	//ADCֵ��ת�� 3.3V�ǵ�ѹ  4096��ADC�ľ���Ϊ12λҲ����2^12=4096
	return value*3.3/4096;
}

/*******************************************************************
* ��������:��ȡADC���ͨ����ֵ
* ����������
*			ADC_HandleTypeDef *hadc��ADC
*			double*data������ADC��ֵ
*			int n��ADCͨ���ĸ���	
* ��������ֵ����
*******************************************************************/
void getManyADC(ADC_HandleTypeDef *hadc,double*data,int n)
{
	int i=0;
	for(i=0;i<n;i++)
	{
		HAL_ADC_Start(hadc);
		//�ȴ�ת����ɣ��ڶ���������ʾ��ʱʱ�䣬��λms
		HAL_ADC_PollForConversion (hadc,50);
		data[i] = ((double)HAL_ADC_GetValue(hadc)/4096)*3.3;
	}
	HAL_ADC_Stop(hadc);
}





