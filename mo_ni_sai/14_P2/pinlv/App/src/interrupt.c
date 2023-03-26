#include "config.h"

/**********************************************串口相关************************************/

//定义一个串口信息的结构
uint8_t ucRxbuff[5];
uint8_t _ucRxbuff[1],lenBuff = 0;

/***使用HAL_UART_Receive_IT中断接收数据 每次接收完成数据后就会执行该函数***/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{	
	if(huart->Instance == USART1){
		// 重新使能中断
		HAL_UART_Receive_IT(huart,(uint8_t *)&_ucRxbuff,sizeof(_ucRxbuff)); 
	}
}



/*******************************************定时器相关***********************************/
// 记录定时器触发的次数
uint16_t tim7Count = 0;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM7)
	{
		tim7Count++;
	}

}	



