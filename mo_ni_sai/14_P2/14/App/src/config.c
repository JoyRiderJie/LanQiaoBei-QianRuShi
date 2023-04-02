#include "config.h"
#include "main.h"

// LCD��ʾ����
struct data displayData[2] = {{1,1000,10},{2,1000,10}},oldData[2] = {{1,1000,10},{2,1000,10}};
// ��ʾLCD��˳��
char displayCount = 0;
// ģʽ�����л�
char contrlMod = 0,flag = 0;

// ����ֵ
unsigned short count[2] = {100,0};


/***************************************
* �������ܣ������߼�����
* ������������
* ��������ֵ����
***************************************/
static void lcdDisplay(void)
{
//	static u16 myc  = 0;
	char temp[20];
	
	// ����ˢ��Ƶ��
	if(count[1] < 100)
		return ;
	count[1] = 0;
	
	// ��ʾ����
	sprintf(temp,"       PA%d",displayData[displayCount].name);  //"       PA%d"
	LCD_DisplayStringLine(Line1,(u8*)temp);
	// ��ʾƵ��
	sprintf(temp,"    F:%dHz  ",displayData[displayCount].f);   // "    F:%dHz  "
	LCD_DisplayStringLine(Line3,(u8*)temp);
	// ��ʾռ�ձ�
	sprintf(temp,"    D:%d%% ",displayData[displayCount].d);    //"    D:%.1f%% "
	LCD_DisplayStringLine(Line4,(u8*)temp);
	
	// ��ʾ��������
//	sprintf(temp,"u:%s us:%d m:%d f:%d ",_ucRxbuff,strlen((char*)_ucRxbuff),contrlMod,flag);
//	LCD_DisplayStringLine(Line6,(u8*)temp);
//	sprintf(temp,"0:%3d 1:%3d myc:%d",count[0],count[1],myc++);
//	LCD_DisplayStringLine(Line7,(u8*)temp);
//	sprintf(temp,"td:%d ",tempData);
//	LCD_DisplayStringLine(Line6,(u8*)temp);
}

/***************************************
* �������ܣ������߼�����
* ������������
* ��������ֵ����
***************************************/
static void changePWM(void)
{
	/***������װ��ֵ �� ���ñȽ�ֵ ���ڵõ�ռ�ձ�***/

	// ����PA1 TIM2
	if(oldData[0].f != displayData[0].f || oldData[0].d != displayData[0].d)
	{
		__HAL_TIM_SetAutoreload(&htim2,1000000/displayData[0].f-1);
		__HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_2,(1000000/displayData[0].f+1)*displayData[0].d/100);
		// ���¼Ĵ������ֵ
		HAL_TIM_GenerateEvent(&htim2, TIM_EVENTSOURCE_UPDATE);
		oldData[0].d = displayData[0].d;
		oldData[0].f = displayData[0].f;	
	}
	
	// ����PA7 TIM3
	if(oldData[1].f != displayData[1].f || oldData[1].d != displayData[1].d)
	{
		__HAL_TIM_SetAutoreload(&htim3,1000000/displayData[1].f-1);
		__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_2,(1000000/displayData[1].f+1)*displayData[1].d/100);
		// ���¼Ĵ������ֵ
		HAL_TIM_GenerateEvent(&htim3, TIM_EVENTSOURCE_UPDATE);
		oldData[1].f = displayData[1].f;
		oldData[1].d = displayData[1].d;
	}
}

/***************************************
* �������ܣ������߼�����
* ������������
* ��������ֵ����
***************************************/
static void keyPro(void)
{
	// ����ɨ��
	unsigned char keyValue = scanKey();
	switch(keyValue)
	{
		// B1 Ƶ������
		case 1:
			if(contrlMod) return ;
			displayData[displayCount].f += 1000;
			if( displayData[displayCount].f == 11000 )
					displayData[displayCount].f = 1000;
			break;
		// B2 ռ�ձ�����
		case 2:
			if(contrlMod) return ;
			displayData[displayCount].d += 10;
			if( displayData[displayCount].d == 100 )
				displayData[displayCount].d = 10;
			break;
		// B3 �л�PA1 PA7����
		case 3:
			if(contrlMod) return ;
			displayCount ^= 1;
			break;
		// B4 �л�ģʽ
		case 4:
			contrlMod ^= 1;
			break;
		default :break;
	}
}

/***************************************
* �������ܣ������߼�����
* ������������
* ��������ֵ����
***************************************/
static void usartPro(void)
{
	// �ж��Ƿ���յ���������
	if(strlen((char*)_ucRxbuff) == 0)
		return ;
	
	// ��������ģʽ�½��յ���������
	if(contrlMod == 0) 
		HAL_UART_Transmit(&huart1,(uint8_t*)"KEY CONTROL\r\n",sizeof("KEY CONTROL\r\n"),20); //  HAL_UART_Transmit_DMA
//		HAL_UART_Transmit_DMA(&huart1,(uint8_t*)"KEY CONTROL\r\n",sizeof("KEY CONTROL\r\n")); //  HAL_UART_Transmit_DMA
	
	// ����ģʽ�½��յ�����
	if(contrlMod == 1)
	{
		if(strcmp((char*)_ucRxbuff,"@") == 0)
			displayCount = 0;
		else if(strcmp((char*)_ucRxbuff,"#") == 0)
			displayCount = 1;
		else 
			HAL_UART_Transmit(&huart1,(uint8_t*)"ERROR\r\n",sizeof("ERROR\r\n"),10); //  HAL_UART_Transmit
	}
	memset(_ucRxbuff,0,sizeof((char*)_ucRxbuff));
}

/***************************************
* �������ܣ�LED��ʾ�߼�����
* ������������
* ��������ֵ����
***************************************/
static void ledPro(void)
{
	// PA1���ݽ���  LED1����
	if(displayCount == 0 && flag != 1)
		changeLedStateByLocation(LED1,1);
	else if(displayCount == 1 && flag != 1)
		changeLedStateByLocation(LED1,0);
	// PA7���ݽ���  LED2����
	if(displayCount == 1 && flag != 2)
		changeLedStateByLocation(LED2,1);
	else if(displayCount == 0 && flag != 2)
		changeLedStateByLocation(LED2,0);
	// ��������ģʽ��
	if(contrlMod == 0)
		changeLedStateByLocation(LED3,1);
	else
		changeLedStateByLocation(LED3,0);
	// PA1Ƶ�ʴ���PA7  LED1��0.1����˸
	if(displayData[0].f > displayData[1].f)
		flag = 1;
	// PA1Ƶ��С����PA7  LED2��0.1����˸
	else if(displayData[0].f < displayData[1].f)
		flag = 2;
	else
		flag = 0;
	// ��LED1 LED2������˸
	if(flag == 1 && count[0] >= 100)
	{
		rollbackLedByLocation(LED1);
		count[0] = 0;
	}
	if(flag == 2 && count[0] >= 100)
	{
		rollbackLedByLocation(LED2);
		count[0] = 0;
	}
}

/***************************************
* �������ܣ�ϵͳ�Զ����ʼ������
* ������������
* ��������ֵ����
***************************************/
void sysInit(void)
{
	// LCD��ʼ������
	lcdInit();
	// �ر�����LED
	changeAllLedByStateNumber(0);
	// �򿪶�ʱ����PWM
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_2);
	// ���������жϽ���
	HAL_UART_Receive_IT(&huart1,(uint8_t *)&_ucRxbuff,sizeof(_ucRxbuff)); 
}

/***************************************
* �������ܣ�ϵͳ�Զ����ʼ������
* ������������
* ��������ֵ����
***************************************/
void sysWork(void)
{
	// �����߼�
	keyPro();
	// �ı�PWM��״̬
	changePWM();
	// LCD��ʾ�߼�
	lcdDisplay();
	// �����߼�
	usartPro();
	// LED�߼�
	ledPro();
}


