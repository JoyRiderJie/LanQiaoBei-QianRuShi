#include "config.h"

uint32_t  cc1_value_T2 = 0;  									// TIMx_CCR1 的值 
uint32_t  f40 = 0;

uint32_t  cc1_value_T22 = 0;  									// TIMx_CCR1 的值 
double  f4 = 0;

uint32_t  cc1_value_T3 = 0;  									// TIMx_CCR1 的值 
uint32_t  f39 = 0;

uint32_t d1[3] = {0,0};

//记录定时器上升沿 下降沿计数值
struct date
{
	//记录计数值  偶数为上升沿 计数的下降沿
	uint32_t count[3];
	//用于记录当前应该是上升沿触发还是下将沿触发
	char edge_flag;
	//记录数据采集数量
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
		//读取定时器捕获单元的计数值
		cc1_value_T2 = HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_1);
		cc1_value_T22 = HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_2);
		//将定时器的计数值设置成0
		__HAL_TIM_SetCounter(htim,0);
		//计算频率
		f40 = 1000000/cc1_value_T2; // 1000000 = (80000000/80)
		f4 = cc1_value_T22*1.0/cc1_value_T2;
		//重新开启定时器
		HAL_TIM_IC_Start_IT(htim,TIM_CHANNEL_1);
		HAL_TIM_IC_Start_IT(htim,TIM_CHANNEL_2);		
	}
#endif
		
#if 01
		//本次为上升沿触发
		if(date.edge_flag%2==0)
		{
			date.dd[date.number][0]++;
			//获取本次上升沿计数值
			date.count[date.number++] = HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_1);
			//采集一次下降沿、两次上升沿数据完成
			if(date.number == 3)
			{
				dutyCicle = (((date.count[1]-date.count[0])*1.0)/(date.count[2]-date.count[0]))*100;
				//将定时器的计数值设置成0
				__HAL_TIM_SetCounter(htim,0);
				__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING); 
				//重新开启定时器
				HAL_TIM_IC_Start_IT(htim,TIM_CHANNEL_1);
				date.edge_flag = 0;
				date.number = 0;
			}
			else
			{				
				__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_FALLING); 
				//将下次触发方式设置为下降沿触发
				date.edge_flag += 1;
			}
		}
		//下降沿触发
		else
		{
			date.dd[date.number][0]++;
			//读取本轮的计数值
			date.count[date.number++] = HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_1);
			//修改触发方式为上升沿触发
			__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING); 
			date.edge_flag += 1;				
		}	
		d1[i%3] = HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_1);
		i++;
	}
#endif
	if(htim->Instance == TIM3)
	{
		//读取定时器的计数值
		cc1_value_T3 = HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_1);
		//将定时器的计数值设置成0
		__HAL_TIM_SetCounter(htim,0);
		//计算频率
		f39 = 1000000/cc1_value_T3; // 1000000 = (80000000/80)
		//重新开启定时器
		HAL_TIM_IC_Start_IT(htim,TIM_CHANNEL_1);
	}
}

