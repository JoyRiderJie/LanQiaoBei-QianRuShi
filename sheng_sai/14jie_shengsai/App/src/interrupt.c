#include "config.h"

/**********************************************��ʱ�����벶�����************************************/
uint16_t f = 0;
// ����TIMx_CCR��ֵ
uint32_t  cclValue = 0;

// ��ʱ���Ļص�����
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
//	// ����TIMx_CCR��ֵ
//	uint32_t  cclValue = 0;
	// ��ʱ��3ʱִ�иö�
	if(htim->Instance == TIM3)
	{
		cclValue = __HAL_TIM_GET_COUNTER(&htim3);
		__HAL_TIM_SetCounter(&htim3, 0);
		f = 1000000 / cclValue;
		HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_2);
	}
}


