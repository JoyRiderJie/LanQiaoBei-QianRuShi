#include "config.h"

//����һ��������Ϣ�Ľṹ
uint8_t ucRxbuff[22];
uint8_t _ucRxbuff[1],lenBuff = 0;

/***ʹ��HAL_UART_Receive_IT�жϽ������� ÿ�ν���������ݺ�ͻ�ִ�иú���***/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{	
	if(huart->Instance == USART1){
		ucRxbuff[lenBuff++%22] = _ucRxbuff[0];
		// ����ʹ���ж�		
		HAL_UART_Receive_IT(huart,(uint8_t *)&_ucRxbuff,sizeof(_ucRxbuff)); 
	}
}



