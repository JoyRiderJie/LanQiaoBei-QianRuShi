#include "config.h"

/**********************************************�������************************************/

//����һ��������Ϣ�Ľṹ
uint8_t ucRxbuff[5];
uint8_t _ucRxbuff[1],lenBuff = 0;

/***ʹ��HAL_UART_Receive_IT�жϽ������� ÿ�ν���������ݺ�ͻ�ִ�иú���***/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{	
	if(huart->Instance == USART1){
		// ����ʹ���ж�
		HAL_UART_Receive_IT(huart,(uint8_t *)&_ucRxbuff,sizeof(_ucRxbuff)); 
	}
}



/*******************************************��ʱ�����***********************************/
// ��¼��ʱ�������Ĵ���
uint16_t tim7Count = 0;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM7)
	{
		tim7Count++;
	}

}	



