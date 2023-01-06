#include "config.h"

/***ʹ��HAL_UART_Receive_IT�жϽ������� ÿ�ν���������ݺ�ͻ�ִ�иú���***/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART1){
		// ����ʹ���ж�
		if(iRxFlag == 0)
		{
			HAL_UART_Receive_IT(huart,(uint8_t *)&cRxBuff,sizeof(cRxBuff)); 
			iRxFlag = 1;
		}
	}
}

/***������ģʽ�¶�ʱ���жϻص�����***/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	int i=0;
	//��ʱ��7���жϻص�����
	if(htim->Instance == TIM7)
	{
		for(i=0;i<3;i++)
			if(uiTime7CountFlag[i] == 1)
				uiTime7Count[i]++;
		
//		//LED1������ʱ
//		if(uiTime7CountLED1Flag)
//			uiTime7CountLED1++;
//		
//		//LED2������ʱ
//		if(uiTime7CountLED2Flag)
//			uiTime7CountLED2++;
//		
//		//ͨ�ü�ʱ
//		uiTime7Count++;
	}
}



