#include "config.h"

/**********************************************串口相关************************************/

//定义一个串口信息的结构
uint8_t ucRxbuff[4];
uint8_t _ucRxbuff[1],lenBuff = 0;

/***使用HAL_UART_Receive_IT中断接收数据 每次接收完成数据后就会执行该函数***/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{	
	if(huart->Instance == USART1){
		ucRxbuff[lenBuff++%3] = _ucRxbuff[0];
		// 重新使能中断		
		HAL_UART_Receive_IT(huart,(uint8_t *)&_ucRxbuff,sizeof(_ucRxbuff)); 
	}
}

/**********************************************定时器输入捕获相关************************************/

//记录定时器上升沿 下降沿计数值
struct date
{
	//记录计数值  偶数为上升沿 计数的下降沿
	uint32_t count[3];
	//用于记录当前应该是上升沿触发还是下将沿触发
	char edge_flag;
	//记录数据采集数量
	int number;
};

// 保存定时器输入捕获时需要的相关变量以及最终结果
struct date time16Data,time3Data;
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
		//本次为上升沿触发
		if(time16Data.edge_flag%2==0)
		{
			//获取本次上升沿计数值
			time16Data.count[time16Data.number++] = HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_1);
			//采集一次下降沿、两次上升沿数据完成
			if(time16Data.number == 3)
			{
				RP[0] = (((time16Data.count[1]-time16Data.count[0])*1.0)/(time16Data.count[2]-time16Data.count[0]))*100;
				//将定时器的计数值设置成0
				__HAL_TIM_SetCounter(htim,0);
				__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING); 
				//重新开启定时器
				HAL_TIM_IC_Start_IT(htim,TIM_CHANNEL_1);
				time16Data.edge_flag = 0;
				time16Data.number = 0;
			}
			else
			{				
				__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_FALLING); 
				//将下次触发方式设置为下降沿触发
				time16Data.edge_flag += 1;
			}
		}
		//下降沿触发
		else
		{
			//读取本轮的计数值
			time16Data.count[time16Data.number++] = HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_1);
			//修改触发方式为上升沿触发
			__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING); 
			time16Data.edge_flag += 1;				
		}	
		
		HAL_TIM_IC_Start_IT(&htim16, TIM_CHANNEL_1);
	}
	// 定时器3时执行该段
	if(htim->Instance == TIM3)
	{		
		//本次为上升沿触发
		if(time3Data.edge_flag%2==0)
		{
			//获取本次上升沿计数值
			time3Data.count[time3Data.number++] = HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_2);
			//采集一次下降沿、两次上升沿数据完成
			if(time3Data.number == 3)
			{
				RP[1] = (((time3Data.count[1]-time3Data.count[0])*1.0)/(time3Data.count[2]-time3Data.count[0]))*100;
				//将定时器的计数值设置成0
				__HAL_TIM_SetCounter(htim,0);
				__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_2, TIM_INPUTCHANNELPOLARITY_RISING); 
				//重新开启定时器
				HAL_TIM_IC_Start_IT(htim,TIM_CHANNEL_2);
				time3Data.edge_flag = 0;
				time3Data.number = 0;
			}
			else
			{				
				__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_2, TIM_INPUTCHANNELPOLARITY_FALLING); 
				//将下次触发方式设置为下降沿触发
				time3Data.edge_flag += 1;
			}
		}
		//下降沿触发
		else
		{
			//读取本轮的计数值
			time3Data.count[time3Data.number++] = HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_2);
			//修改触发方式为上升沿触发
			__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_2, TIM_INPUTCHANNELPOLARITY_RISING); 
			time3Data.edge_flag += 1;				
		}	
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
	
}

