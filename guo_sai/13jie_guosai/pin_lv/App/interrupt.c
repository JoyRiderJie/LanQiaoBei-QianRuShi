#include "config.h"

/***使用HAL_UART_Receive_IT中断接收数据 每次接收完成数据后就会执行该函数***/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{	
	if(huart->Instance == USART1){
		// 重新使能中断
		if(ucRxbuff[0]=='P' && _ucRxbuff=='A') 
			ucRxbuff[1] = _ucRxbuff;
		else if((ucRxbuff[0]=='P' && ucRxbuff[1]=='A') && (_ucRxbuff=='1' || _ucRxbuff=='4' || _ucRxbuff=='5'))
			ucRxbuff[2] = _ucRxbuff;
		else
			ucRxbuff[0] = _ucRxbuff;
		HAL_UART_Receive_IT(huart,(uint8_t *)&_ucRxbuff,sizeof(_ucRxbuff));
	}
}

//输入捕获回调函数
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	uint32_t temp = 0;
	//发生中断的定时器为定时器2通道2
	if(htim->Instance == TIM2 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
	{
		//读取定时器的计数值
		temp = HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_2);
		//将定时器的计数值设置成0
		__HAL_TIM_SetCounter(htim,0);
		//频率<100时 钳制在100
		if(P1 < 100)
			P1 = 100;
		//频率>10k时 钳制在10k
		else if(P1 > 10000)
			P1 = 10000;
		//重新开启定时器
		HAL_TIM_IC_Start_IT(htim,TIM_CHANNEL_2);
	}
}

extern unsigned int uiTime7Count ;

/***非阻塞模式下定时器中断回调函数***/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	//定时器7的中断回调函数
	if(htim->Instance == TIM7)
	{
		//通用计时
		if(++uiTime7Count%100 == 0)
			LED3TimeFlag = 1;
	}
}
