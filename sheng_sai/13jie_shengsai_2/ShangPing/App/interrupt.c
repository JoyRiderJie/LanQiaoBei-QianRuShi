#include "config.h"

/***使用HAL_UART_Receive_IT中断接收数据 每次接收完成数据后就会执行该函数***/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART1){
		// 重新使能中断
		if(iRxFlag == 0)
		{
			HAL_UART_Receive_IT(huart,(uint8_t *)&cRxBuff,sizeof(cRxBuff)); 
			iRxFlag = 1;
		}
	}
}

/***非阻塞模式下定时器中断回调函数***/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	int i=0;
	//定时器7的中断回调函数
	if(htim->Instance == TIM7)
	{
		for(i=0;i<3;i++)
			if(uiTime7CountFlag[i] == 1)
				uiTime7Count[i]++;
		
//		//LED1工作计时
//		if(uiTime7CountLED1Flag)
//			uiTime7CountLED1++;
//		
//		//LED2工作计时
//		if(uiTime7CountLED2Flag)
//			uiTime7CountLED2++;
//		
//		//通用计时
//		uiTime7Count++;
	}
}



