#include "config.h"

//定义一个串口信息的结构
uint8_t ucRxbuff[22];
uint8_t _ucRxbuff[1],lenBuff = 0;

/***使用HAL_UART_Receive_IT中断接收数据 每次接收完成数据后就会执行该函数***/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{	
	if(huart->Instance == USART1){
		ucRxbuff[lenBuff++%22] = _ucRxbuff[0];
		// 重新使能中断		
		HAL_UART_Receive_IT(huart,(uint8_t *)&_ucRxbuff,sizeof(_ucRxbuff)); 
	}
}



