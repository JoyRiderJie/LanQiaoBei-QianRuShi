#include "config.h"

uint32_t  cc1_value_T2 = 0;  									// TIMx_CCR1 ��ֵ 
uint32_t  f40 = 0;

uint32_t  cc1_value_T22 = 0;  									// TIMx_CCR1 ��ֵ 
double  f4 = 0;

uint32_t  cc1_value_T3 = 0;  									// TIMx_CCR1 ��ֵ 
uint32_t  f39 = 0;

uint32_t d1[3] = {0,0};

//��¼��ʱ�������� �½��ؼ���ֵ
struct date
{
	//��¼����ֵ  ż��Ϊ������ �������½���
	uint32_t count[3];
	//���ڼ�¼��ǰӦ���������ش��������½��ش���
	char edge_flag;
	//��¼���ݲɼ�����
	int number;
	uint32_t dd[3][100];
};
struct date date;
double dutyCicle = 0;

extern TIM_HandleTypeDef htim2,htim3;
char txt[50];

void sysInit(void)
{
	int i=0;
	lcdInit();
	for(;i<3;i++)
		date.count[i] = 0;
	date.edge_flag = 0;
	date.number = 0;
}

void sysWork(void)
{
	LCD_DisplayStringLine(Line0, (uint8_t *)"     DATA      ");
	sprintf(txt,"  time3 %d",f39);
	LCD_DisplayStringLine(Line2, (uint8_t *)txt);
	sprintf(txt,"  time2 %d",f40);
	LCD_DisplayStringLine(Line3, (uint8_t *)txt);	
	
	sprintf(txt,"  dutyCicle %.3f",dutyCicle );
	LCD_DisplayStringLine(Line4, (uint8_t *)txt);	
	sprintf(txt," %5d %5d %5d",date.count[0],date.count[1],date.count[2]);
	LCD_DisplayStringLine(Line5, (uint8_t *)txt); 
	sprintf(txt,"%.3f %d",((date.count[1]-date.count[0])*1.0),(date.count[2]-date.count[0]));
	LCD_DisplayStringLine(Line6, (uint8_t *)txt); 
	

	sprintf(txt,"%d %d %d",date.dd[0][0],date.dd[1][0],date.dd[2][0]);
	LCD_DisplayStringLine(Line7, (uint8_t *)txt); 
	
	sprintf(txt,"%d %d %d %5d",d1[0],d1[1],d1[2],d1[1]-d1[0]);
	LCD_DisplayStringLine(Line8, (uint8_t *)txt); 
	
//	sprintf(txt,"cc1_value_T22:%d",cc1_value_T22);
//	LCD_DisplayStringLine(Line5, (uint8_t *)txt); 
//	sprintf(txt,"f4:%.3f",f4);
//	LCD_DisplayStringLine(Line6, (uint8_t *)txt); 

	HAL_Delay(1000);
}
int i=0;

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM2)
	{
#if 0
		//��ȡ��ʱ������Ԫ�ļ���ֵ
		cc1_value_T2 = HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_1);
		cc1_value_T22 = HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_2);
		//����ʱ���ļ���ֵ���ó�0
		__HAL_TIM_SetCounter(htim,0);
		//����Ƶ��
		f40 = 1000000/cc1_value_T2; // 1000000 = (80000000/80)
		f4 = cc1_value_T22*1.0/cc1_value_T2;
		//���¿�����ʱ��
		HAL_TIM_IC_Start_IT(htim,TIM_CHANNEL_1);
		HAL_TIM_IC_Start_IT(htim,TIM_CHANNEL_2);		
	}
#endif
		
#if 01
		//����Ϊ�����ش���
		if(date.edge_flag%2==0)
		{
			date.dd[date.number][0]++;
			//��ȡ���������ؼ���ֵ
			date.count[date.number++] = HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_1);
			//�ɼ�һ���½��ء������������������
			if(date.number == 3)
			{
				dutyCicle = (((date.count[1]-date.count[0])*1.0)/(date.count[2]-date.count[0]))*100;
				//����ʱ���ļ���ֵ���ó�0
				__HAL_TIM_SetCounter(htim,0);
				__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING); 
				//���¿�����ʱ��
				HAL_TIM_IC_Start_IT(htim,TIM_CHANNEL_1);
				date.edge_flag = 0;
				date.number = 0;
			}
			else
			{				
				__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_FALLING); 
				//���´δ�����ʽ����Ϊ�½��ش���
				date.edge_flag += 1;
			}
		}
		//�½��ش���
		else
		{
			date.dd[date.number][0]++;
			//��ȡ���ֵļ���ֵ
			date.count[date.number++] = HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_1);
			//�޸Ĵ�����ʽΪ�����ش���
			__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING); 
			date.edge_flag += 1;				
		}	
		d1[i%3] = HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_1);
		i++;
	}
#endif
	if(htim->Instance == TIM3)
	{
		//��ȡ��ʱ���ļ���ֵ
		cc1_value_T3 = HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_1);
		//����ʱ���ļ���ֵ���ó�0
		__HAL_TIM_SetCounter(htim,0);
		//����Ƶ��
		f39 = 1000000/cc1_value_T3; // 1000000 = (80000000/80)
		//���¿�����ʱ��
		HAL_TIM_IC_Start_IT(htim,TIM_CHANNEL_1);
	}
}

