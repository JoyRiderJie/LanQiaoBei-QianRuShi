#include "config.h"

/**********************************************�������************************************/

//����һ��������Ϣ�Ľṹ
uint8_t ucRxbuff[4];
uint8_t _ucRxbuff[1],lenBuff = 0;

/***ʹ��HAL_UART_Receive_IT�жϽ������� ÿ�ν���������ݺ�ͻ�ִ�иú���***/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{	
	if(huart->Instance == USART1){
		ucRxbuff[lenBuff++%3] = _ucRxbuff[0];
		// ����ʹ���ж�		
		HAL_UART_Receive_IT(huart,(uint8_t *)&_ucRxbuff,sizeof(_ucRxbuff)); 
	}
}

/**********************************************��ʱ�����벶�����************************************/

//��¼��ʱ�������� �½��ؼ���ֵ
struct date
{
	//��¼����ֵ  ż��Ϊ������ �������½���
	uint32_t count[3];
	//���ڼ�¼��ǰӦ���������ش��������½��ش���
	char edge_flag;
	//��¼���ݲɼ�����
	int number;
};

// ���涨ʱ�����벶��ʱ��Ҫ����ر����Լ����ս��
struct date time16Data,time3Data;
// ����PWM������ֵ RP[0]-PWM1��ֵ  RP[1]-PWM2��ֵ
uint32_t  RP[4] = {0,0,0,0};

// ��ʱ���Ļص�����
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	// ����TIMx_CCR��ֵ
	uint32_t  cclValue = 0;
	// ��ʱ��16ʱִ�иö�
	if(htim->Instance == TIM16)
	{		
		//����Ϊ�����ش���
		if(time16Data.edge_flag%2==0)
		{
			//��ȡ���������ؼ���ֵ
			time16Data.count[time16Data.number++] = HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_1);
			//�ɼ�һ���½��ء������������������
			if(time16Data.number == 3)
			{
				RP[0] = (((time16Data.count[1]-time16Data.count[0])*1.0)/(time16Data.count[2]-time16Data.count[0]))*100;
				//����ʱ���ļ���ֵ���ó�0
				__HAL_TIM_SetCounter(htim,0);
				__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING); 
				//���¿�����ʱ��
				HAL_TIM_IC_Start_IT(htim,TIM_CHANNEL_1);
				time16Data.edge_flag = 0;
				time16Data.number = 0;
			}
			else
			{				
				__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_FALLING); 
				//���´δ�����ʽ����Ϊ�½��ش���
				time16Data.edge_flag += 1;
			}
		}
		//�½��ش���
		else
		{
			//��ȡ���ֵļ���ֵ
			time16Data.count[time16Data.number++] = HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_1);
			//�޸Ĵ�����ʽΪ�����ش���
			__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING); 
			time16Data.edge_flag += 1;				
		}	
		
		HAL_TIM_IC_Start_IT(&htim16, TIM_CHANNEL_1);
	}
	// ��ʱ��3ʱִ�иö�
	if(htim->Instance == TIM3)
	{		
		//����Ϊ�����ش���
		if(time3Data.edge_flag%2==0)
		{
			//��ȡ���������ؼ���ֵ
			time3Data.count[time3Data.number++] = HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_2);
			//�ɼ�һ���½��ء������������������
			if(time3Data.number == 3)
			{
				RP[1] = (((time3Data.count[1]-time3Data.count[0])*1.0)/(time3Data.count[2]-time3Data.count[0]))*100;
				//����ʱ���ļ���ֵ���ó�0
				__HAL_TIM_SetCounter(htim,0);
				__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_2, TIM_INPUTCHANNELPOLARITY_RISING); 
				//���¿�����ʱ��
				HAL_TIM_IC_Start_IT(htim,TIM_CHANNEL_2);
				time3Data.edge_flag = 0;
				time3Data.number = 0;
			}
			else
			{				
				__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_2, TIM_INPUTCHANNELPOLARITY_FALLING); 
				//���´δ�����ʽ����Ϊ�½��ش���
				time3Data.edge_flag += 1;
			}
		}
		//�½��ش���
		else
		{
			//��ȡ���ֵļ���ֵ
			time3Data.count[time3Data.number++] = HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_2);
			//�޸Ĵ�����ʽΪ�����ش���
			__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_2, TIM_INPUTCHANNELPOLARITY_RISING); 
			time3Data.edge_flag += 1;				
		}	
		HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_2);
	}
	// ��ʱ��2ʱִ�иö�
	if(htim->Instance == TIM2)
	{
		cclValue = __HAL_TIM_GET_COUNTER(&htim2);
		__HAL_TIM_SetCounter(&htim2, 0);
		RP[2] = 1000000 / cclValue;
		HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_2);
	}
	
}

