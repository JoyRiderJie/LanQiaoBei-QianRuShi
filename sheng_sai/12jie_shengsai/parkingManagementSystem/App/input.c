#include "config.h"

/***ʹ��HAL_UART_Receive_IT�жϽ������� ÿ�ν���������ݺ�ͻ�ִ�иú���***/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART1){
		// ����ʹ���ж�
		if(!iRxFlag)
		{
			HAL_UART_Receive_IT(huart,(uint8_t *)&ucRxbuff,sizeof(ucRxbuff)); 
			iRxFlag = 1;
		}
	}
}





