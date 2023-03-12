#include "555.h"

extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim15;
extern TIM_HandleTypeDef htim16;

// 保存PWM的数据值 RP[0]-PWM1的值  RP[1]-PWM2的值
uint32_t  RP[4] = {0,0,0,0};

// 定时器的回调函数
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	// 保存TIMx_CCR的值
	uint32_t  cclValue = 0;
	
	// 定时器16时执行该段
	if(htim->Instance == TIM16)
	{
		cclValue = __HAL_TIM_GET_COUNTER(&htim16);
		__HAL_TIM_SetCounter(&htim16, 0);
		RP[0] = 1000000 / cclValue;
		HAL_TIM_IC_Start_IT(&htim16, TIM_CHANNEL_1);
	}
	// 定时器3时执行该段
	if(htim->Instance == TIM3)
	{
		cclValue = __HAL_TIM_GET_COUNTER(&htim3);
		__HAL_TIM_SetCounter(&htim3, 0);
		RP[1] = 1000000 / cclValue;
		HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_2);
	}
	// 定时器2时执行该段
	if(htim->Instance == TIM2)
	{
		cclValue = __HAL_TIM_GET_COUNTER(&htim2);
		__HAL_TIM_SetCounter(&htim2, 0);
		RP[2] = 1000000 / cclValue;
		HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_2);
	}
	// 定时器15时执行该段
	if(htim->Instance == TIM15)
	{
		cclValue = __HAL_TIM_GET_COUNTER(&htim15);
		__HAL_TIM_SetCounter(&htim15, 0);
		RP[3] = 1000000 / cclValue;
		HAL_TIM_IC_Start_IT(&htim15, TIM_CHANNEL_1);
	}
}

