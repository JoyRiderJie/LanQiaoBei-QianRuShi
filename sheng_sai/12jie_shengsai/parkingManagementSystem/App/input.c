#include "config.h"

/***使用HAL_UART_Receive_IT中断接收数据 每次接收完成数据后就会执行该函数***/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART1){
		// 重新使能中断
		if(!iRxFlag)
		{
			HAL_UART_Receive_IT(huart,(uint8_t *)&ucRxbuff,sizeof(ucRxbuff)); 
			iRxFlag = 1;
		}
	}
}





