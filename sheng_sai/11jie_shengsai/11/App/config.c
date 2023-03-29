#include "config.h"

//����洢��ʾ���ݵĽṹ��
struct displayData*showData;

//���ڼ�ʱ
unsigned int count = 0;

/***********************************************
* �������ܣ��Զ����ϵͳ��ʼ������
* ������������
* ��������ֵ����
***********************************************/
void sysInit(void)
{
	//LCD��ʼ��
	lcdInit();
	//�ر�LED
	changeAllLedByStateNumber(OFF);
	
	//�򿪶�ʱ������PWM��ͨ��
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim17,TIM_CHANNEL_1);
	
	//��ʼ���ṹ��
	showData = (struct displayData*)malloc(sizeof(struct displayData));
	showData->frdP6 = 10;
	showData->frdP7 = 10;
	showData->mode = 0;
	showData->vValue = 0;
	showData->page = 0;
}

/***********************************************
* �������ܣ��Զ����ϵͳ��������
* ������������
* ��������ֵ����
***********************************************/
void sysWork(void)
{
	unsigned char keyNumber = scanKey();
	switch(keyNumber)
	{
		//�л���ʾ����
		case 1:
			showData->page++;
			break;
		//�ֶ�ģʽ  �������� �޸�PA6�Ĳ���
		case 2:
			if(showData->mode%2==1 && showData->page%2==1)
				showData->frdP6 += 10;
			if(showData->frdP6 == 100)
				showData->frdP6 = 10;
			break;
		//�ֶ�ģʽ  �������� �޸�PA7�Ĳ���
		case 3:
			if(showData->mode%2==1 && showData->page%2==1)
				showData->frdP7 += 10;
			if(showData->frdP7 == 100)
				showData->frdP7 = 10;
			break;
		//�л�ģʽ
		case 4:
			showData->mode++;
			break;
		default:break;
	}
	
	//��ȡADC��ֵ ���ͻ�ȡADCֵ��Ƶ��
	if(count++ %5 == 0)
		showData->vValue = getADC(&hadc2);
	//�л�PWM��Ƶ��
	changePwmFrd();
	//LCD��ʾ
	display();
	//LED��ʾ
	ledWork();
}

/***********************************************
* �������ܣ�����PWMռ�ձȺ���
* ������������
* ��������ֵ����
***********************************************/
void changePwmFrd(void)
{
	//�ֶ�ģʽ
	if(showData->mode%2 == 1)
	{
		__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_1,100*showData->frdP6);
		__HAL_TIM_SetCompare(&htim17,TIM_CHANNEL_1,50*showData->frdP7);  
	}
	//�Զ�ģʽ ��ѹֵΪ0V
	else if(showData->mode%2 == 0 && showData->vValue == 0)
	{
		__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_1,10000);
		__HAL_TIM_SetCompare(&htim17,TIM_CHANNEL_1,5000);
	}
	//�Զ�ģʽ  ��ѹֵΪ3.3V 
	else if(showData->mode%2 == 0 && showData->vValue == 3.3)
	{
		__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_1,0);
		__HAL_TIM_SetCompare(&htim17,TIM_CHANNEL_1,0);
	}
}

/***********************************************
* �������ܣ�LED��������
* ������������
* ��������ֵ����
***********************************************/
void ledWork(void)
{
	//�ر����е�LED��
	changeAllLedByStateNumber(OFF);
	
	//�Զ�ģʽ
	if(showData->mode%2 == 0)
		changeLedStateByLocation(LED1,ON);
	
	//���ݽ���
	if( showData->page%2 == 0 )
		changeLedStateByLocation(LED2,ON);
}

/***********************************************
* �������ܣ���ʾ����
* ������������
* ��������ֵ����
***********************************************/
void display(void)
{
	char temp[10];
	//������ʾ����
	if( showData->page%2 == 0 )
	{
		LCD_DisplayStringLine(Line0,(uint8_t*)"       Data");
		sprintf(temp,"     V:%.2fV",showData->vValue);
		LCD_DisplayStringLine(Line2,(uint8_t*)temp);
		//�Զ�ģʽ
		if(showData->mode%2 == 0)
			LCD_DisplayStringLine(Line4,(uint8_t*)"     Mode:AUTO     ");
		//�ֶ�ģʽ
		else
			LCD_DisplayStringLine(Line4,(uint8_t*)"     Mode:MANU     ");		
	}
	//��ʾ��������
	else if( showData->page%2 == 1)
	{
		LCD_DisplayStringLine(Line0,(uint8_t*)"       Pata");
		sprintf(temp,"    PA6:%d%%        ",showData->frdP6);
		LCD_DisplayStringLine(Line2,(uint8_t*)temp);
		sprintf(temp,"    PA7:%d%%        ",showData->frdP7);
		LCD_DisplayStringLine(Line4,(uint8_t*)temp);
	}
}



