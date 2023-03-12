#include "myAdc.h"

/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    getADC_RPx
  * @brief   ��ȡ��Ӧͨ����ADC��ģ���ѹֵ
  * @param   hadc: [����]  ָ��ADC����
**			 channel: [����] ָ������ADC��ͨ�� 
  * @retval  float: ����ָ��ͨ����ADCֵ
  * @author  �����ܲ�������
  * @Data    2023-03-12
  * 1. ...
  * <modify staff>:
  * <data>        :
  * <description> :
  * 2. ...
 **/
/* -------------------------------- end -------------------------------- */
float getADC_RPx(ADC_HandleTypeDef *hadc,uint32_t channel)
{
    ADC_ChannelConfTypeDef sConfig = {0};
    uint16_t adc = 0;
    sConfig.Channel = channel; 
    sConfig.Rank = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_640CYCLES_5;
    sConfig.SingleDiff = ADC_SINGLE_ENDED;
    sConfig.OffsetNumber = ADC_OFFSET_NONE;
    sConfig.Offset = 0;
    if (HAL_ADC_ConfigChannel(hadc, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }
    HAL_ADC_Start(hadc);
    adc = HAL_ADC_GetValue(hadc);
	HAL_Delay(10);

    return adc* 3.3 / 4096;
}

