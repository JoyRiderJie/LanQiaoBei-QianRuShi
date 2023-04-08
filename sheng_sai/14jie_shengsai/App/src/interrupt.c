#include "config.h"

/**********************************************定时器输入捕获相关************************************/
uint16_t f = 0;
// 保存TIMx_CCR的值
uint32_t  cclValue = 0;

// 定时器的回调函数
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
//	// 保存TIMx_CCR的值
//	uint32_t  cclValue = 0;
	// 定时器3时执行该段
	if(htim->Instance == TIM3)
	{
		cclValue = __HAL_TIM_GET_COUNTER(&htim3);
		__HAL_TIM_SetCounter(&htim3, 0);
		f = 1000000 / cclValue;
		HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_2);
	}
}


