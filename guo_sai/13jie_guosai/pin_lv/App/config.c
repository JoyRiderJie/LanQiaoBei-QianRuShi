#include "config.h"

//����һ��������Ϣ�Ľṹ
uint8_t ucRxbuff[4];
uint8_t _ucRxbuff;

//����һ��������¼��ʱ��7�жϴ���
unsigned int uiTime7Count = 0;
// ���ݽ�����ʾ���� 0-PA4 1-PA5 2-PA1 3-PA7
double data[4] = {0,0,0,0};
int P1 = 0;
// ����������ʾ X/0-Ƶ�ʲ���  Y/1-��ѹ����
uint8_t para[2] = {0,0};
// �ṹ��0��ʾPA4  �ṹ��1��ʾPA5 
struct paContent paText[2] = {{4,0,0,0,0},{5,0,0,0,0}};
// ��¼�ṹ��λ��
int paLocation;
// ��¼ϵͳģʽ  ʹ�ð��������л�
int systemMode;
// ��¼����B4
int keyB4[2] = {0,0};
// ��¼��ʾģʽ
int LCDMod = 0;
uint8_t LED3TimeFlag = 0;

extern void EnableDebugIRQ(void);

/*************************************
* �������ܣ��Զ����ϵͳ��ʼ������
* ������������
* ��������ֵ����
*************************************/
void sysInit(void)
{
	int i=0;
	
	//LCD��ʾ����ʼ��
	lcdInit();
	//IIC��ʼ��
	I2CInit();
	// ��ȡEEPROM�е�ֵ ���ҽ������
	for(i=0;i<2;i++)
	{
		para[i] = readEepromByBit(0xa1-i);
			if(para[i] > 5)
				para[i] = 1;
			else if(para[i] < 1)
				para[i] = 1;
		HAL_Delay(10);
	}
	//�򿪴��ڵ��жϽ��չ���
	HAL_UART_Receive_IT(&huart1,(uint8_t *)&_ucRxbuff,sizeof(_ucRxbuff)); 
	//�򿪶�ʱ��17ͨ��1��PWM�������
	HAL_TIM_PWM_Start(&htim17,TIM_CHANNEL_1);
	//�򿪶�ʱ��2ͨ��2�����벶����
    HAL_TIM_IC_Start_IT(&htim2,TIM_CHANNEL_2);
	//�򿪶�ʱ��7�ж�
	HAL_TIM_Base_Start_IT(&htim7);
	// ��ADC���Զ�У׼
	HAL_ADCEx_Calibration_Start(&hadc2,ADC_SINGLE_ENDED);	
	// �ر�����LED
	changeAllLedByStateNumber(0);
}

/*************************************
* �������ܣ�ϵͳ���߼�����
* ������������
* ��������ֵ����
*************************************/
void sysWork(void)
{	
	usartPro();
	keyPro();
	displayText(systemMode%3);
	ledPro((data[0]>data[1]*para[1]) && LED3TimeFlag,LCDMod);
}

/***************************************************
* �������ܣ������Ĵ�����
* ������������
* ��������ֵ����
***************************************************/
void keyPro(void)
{
	int i=0;
	
	// ����ɨ��
	scanKeyUseStructAndTime();
	// key[0]=B1 
	if(key[0].flag)
		systemMode++;
	// key[1]=B2 
	if(key[1].flag && systemMode%3==1)
	{
		if(++para[0] >= 5) 
			para[0] = 1;
		writeEepromByBit(0xa1,para[0]);
	}
	// key[2]=B3 
	if(key[2].flag && systemMode%3==1)
	{
		if(++para[1] >= 5) 
			para[1] = 1;
		writeEepromByBit(0xa0,para[1]);
	}
	// key[3]=B4
	// B4�����ݽ��水��
	if(key[3].flag && systemMode%3==0)
		elePro();
	// B4�ڲ������水�� ��Ƶ/��Ƶ
	if(key[3].flag && systemMode%3==1)
		FrePro(++keyB4[0]%2);
	// B4�ڼ�¼���水�� ��
	if(systemMode%3==2 && key[3].flag && key[3].longFlag==0)
		keyB4[1] = (keyB4[1]+1)%2;
	// B4�ڼ�¼���水�� ��
	if(systemMode%3==2 && key[3].flag==0 && key[3].longFlag)
	{
		paText[keyB4[1]].a = 0;
		paText[keyB4[1]].t = 0;
		paText[keyB4[1]].h = 0;
		paText[keyB4[1]].n = 0;
		memset(paText[keyB4[1]].data,0,sizeof(paText[keyB4[1]].data));
	}
	// ������������Чֵ
	for(i=0;i<4;i++)
	{
		key[i].flag = 0;
		key[i].longFlag = 0;
	}
}

/***************************************************
* �������ܣ�����LCD��ʾģʽ
* ����������
*			int mod:��ʾģʽ 0-������ʾ 1-��ת��ʾ
* ��������ֵ����
***************************************************/
void setDisplayMod(int mod)
{
	// ������ʾ
	if(mod == 1)
	{
		LCD_WriteReg( R1 ,0x0100 );
		LCD_WriteReg( R96,0xA700 );
		LCD_Clear(Black);
	}
	// ������ʾ
	else
	{
		LCD_WriteReg( R1 ,0x0000 );
		LCD_WriteReg( R96,0x2700 );
		LCD_Clear(Black);
	}
}

/***************************************************
* �������ܣ�LCD��ʾ����
* ����������
*			int mod:��ʾ����ģʽ 0-���ݽ��� 1-��ת��ʾ 2-��¼����
* ��������ֵ����
***************************************************/
void displayText(int mod)
{
	// ��¼��ʾ����
	static char displayTemp[5][30];
	//0-���ݽ���
	if(mod == 0)
	{	
		sprintf(displayTemp[0],"         DATA        ");
		sprintf(displayTemp[1],"      PA4=%.2f        ",data[0]);
		sprintf(displayTemp[2],"      PA5=%.2f        ",data[1]);
		sprintf(displayTemp[3],"      PA1=%d          ",P1);
		sprintf(displayTemp[4],"                ");
	}		
	//1-������ʾ 
	else if(mod == 1)
	{
		sprintf(displayTemp[0],"         PARA    ");
		sprintf(displayTemp[1],"      X=%d      ",para[0]);
		sprintf(displayTemp[2],"      Y=%d      ",para[1]);
		sprintf(displayTemp[3],"                   ");
		sprintf(displayTemp[4],"                   ");
	}
	//2-��¼����
	else if(mod == 2)
	{
		sprintf(displayTemp[0],"         REC-PA%d",paText[keyB4[1]].nameID);
		sprintf(displayTemp[1],"      N=%d          ",paText[keyB4[1]].n);
		sprintf(displayTemp[2],"      A=%.2f        ",paText[keyB4[1]].a);
		sprintf(displayTemp[3],"      T=%.2f        ",paText[keyB4[1]].t);
		sprintf(displayTemp[4],"      H=%.2f        ",paText[keyB4[1]].h);
	}
	
	LCD_DisplayStringLine(Line1,(uint8_t*)displayTemp[0]);
	LCD_DisplayStringLine(Line3,(uint8_t*)displayTemp[1]);
	LCD_DisplayStringLine(Line4,(uint8_t*)displayTemp[2]);
	LCD_DisplayStringLine(Line5,(uint8_t*)displayTemp[3]);
	LCD_DisplayStringLine(Line6,(uint8_t*)displayTemp[4]);
}

/**************************************************
* �������ܣ�LED��������
* ����������
*		   int mod1��LED3�Ĺ���ģʽ
*		   int mod2��LED4�Ĺ���ģʽ
* ��������ֵ����
***************************************************/
void ledPro(int mod1,int mod2)
{
	// ��Ƶ
	if(keyB4[0]%2 == 0)
		changeLedStateByLocation(LED1,1);
	else
		changeLedStateByLocation(LED1,0);
	
	// ��Ƶ
	if(keyB4[0]%2 == 1)
		changeLedStateByLocation(LED2,1);
	else
		changeLedStateByLocation(LED2,0);
	
	// ��ѹ
	if(mod1%2 == 1)
	{
		rollbackLedByLocation(LED3);
		LED3TimeFlag = 0;
	}
	else
		changeLedStateByLocation(LED3,0);
	
	// LCD����
	if(mod2%2 == 0)
		changeLedStateByLocation(LED4,1);
	else
		changeLedStateByLocation(LED4,0);
}

/**************************************************
* �������ܣ��������ݴ�����
* ������������
* ��������ֵ����
***************************************************/
void usartPro(void)
{
	char usartTemp[10];
	
	// ��֤�Ƿ��յ�������Ϣ�Լ�������Ϣ����
	// ����
	if(ucRxbuff[0] == 'X')
		sprintf(usartTemp,"X:%d\r\n",para[0]);
	else if(ucRxbuff[0] == 'Y')
		sprintf(usartTemp,"Y:%d\r\n",para[1]);
	// ��ʾ�л�
	else if(ucRxbuff[0] == '#')
		setDisplayMod(++LCDMod%2);
	
	// ����
	if(ucRxbuff[0] == 'P')
	{
		if(strcmp((char*)ucRxbuff,"PA1") == 0)
			sprintf(usartTemp,"PA1:%d\r\n",P1);
		else if(strcmp((char*)ucRxbuff,"PA4") == 0)
			sprintf(usartTemp,"PA4:%.2f\r\n",data[0]);
		else if(strcmp((char*)ucRxbuff,"PA5") == 0)
			sprintf(usartTemp,"PA5:%.2f\r\n",data[1]);	
	}
	
	// �Ƿ�������
	if(usartTemp[0]=='P' || usartTemp[0]=='X' || usartTemp[0]=='Y')
		HAL_UART_Transmit(&huart1,(uint8_t*)usartTemp,sizeof(char)*strlen(usartTemp),10);
	
	memset(ucRxbuff,0,sizeof(ucRxbuff));
}

/**************************************************
* �������ܣ���ѹ��������
* ������������
* ��������ֵ����
***************************************************/

void elePro(void)
{	
	int i=0;
	
	// ��ȡADC����
	getManyADC(&hadc2,data,2);
	// ����ȡ����ADC���ݴ��浽������
	while(i < 2)
	{	
		if(data[i] > paText[i].a)
			paText[i].a = data[i];
		if(data[i] < paText[i].t || paText[i].n==0)
			paText[i].t = data[i];
		paText[i].data[paText[i].n++%100] = data[i];
		paText[i].h = (paText[i].h*(paText[i].n-1)+data[i])*1.0/paText[i].n;
		i++;
	}
}

/**************************************************
* �������ܣ�Ƶ�ʹ�������
* ����������
*			int mod: mod=0Ϊ��Ƶ  mod=1Ϊ��Ƶ
* ��������ֵ����
***************************************************/
void FrePro(int mod)
{
	int frd = 0;

	// ��Ƶ  keyB4[0]%2
	if(mod == 0)
		frd = (int)(P1/para[0]-1);
	// ��Ƶ
	else
		frd = (int)(P1*para[0]-1);
	//����Ƶ��
	__HAL_TIM_SetAutoreload(&htim17,frd);
	HAL_TIM_GenerateEvent(&htim17, TIM_EVENTSOURCE_UPDATE);
}



#if 0
/***************************************************
* �������ܣ������Ĵ�����
* ������������
* ��������ֵ����
***************************************************/
void keyPro(void)
{
	int i=0;
	
	// ����ɨ��
	scanKeyUseStructAndTime();
	// key[0]=B1 
	if(key[0].flag)
		systemMode++;
	// key[1]=B2 
	if(key[1].flag && systemMode%3==1)
	{
		if(++para[0] >= 5) 
			para[0] = 1;
		writeEepromByBit(0xa1,para[0]);
	}
	// key[2]=B3 
	if(key[2].flag && systemMode%3==1)
	{
		if(++para[1] >= 5) 
			para[1] = 1;
		writeEepromByBit(0xa0,para[1]);
	}
	// key[3]=B4
	// B4�����ݽ��水��
	if(key[3].flag && systemMode%3==0)
		elePro();
	// B4�ڲ������水�� ��Ƶ/��Ƶ
	if(key[3].flag && systemMode%3==1)
		FrePro(++keyB4[0]%2);
	// B4�ڼ�¼���水�� ��
	if(systemMode%3==2 && key[3].flag && key[3].longFlag==0)
		keyB4[1] = (keyB4[1]+1)%2;
	// B4�ڼ�¼���水�� ��
	if(systemMode%3==2 && key[3].flag==0 && key[3].longFlag)
	{
		paText[keyB4[1]].a = 0;
		paText[keyB4[1]].t = 0;
		paText[keyB4[1]].h = 0;
		paText[keyB4[1]].n = 0;
		memset(paText[keyB4[1]].data,0,sizeof(paText[keyB4[1]].data));
	}
	// ������������Чֵ
	for(i=0;i<4;i++)
	{
		key[i].flag = 0;
		key[i].longFlag = 0;
	}
}

/***************************************************
* �������ܣ�����LCD��ʾģʽ
* ����������
*			int mod:��ʾģʽ 0-������ʾ 1-��ת��ʾ
* ��������ֵ����
***************************************************/
void setDisplayMod(int mod)
{
	// ������ʾ
	if(mod == 1)
	{
		LCD_WriteReg( R1 ,0x0100 );
		LCD_WriteReg( R96,0xA700 );
		LCD_Clear(Black);
	}
	// ������ʾ
	else
	{
		LCD_WriteReg( R1 ,0x0000 );
		LCD_WriteReg( R96,0x2700 );
		LCD_Clear(Black);
	}
}

/***************************************************
* �������ܣ�LCD��ʾ����
* ����������
*			int mod:��ʾ����ģʽ 0-���ݽ��� 1-��ת��ʾ 2-��¼����
* ��������ֵ����
***************************************************/
void displayText(int mod)
{
	// ��¼��ʾ����
	static char displayTemp[5][30];
	//0-���ݽ���
	if(mod == 0)
	{	
		sprintf(displayTemp[0],"         DATA        ");
		sprintf(displayTemp[1],"      PA4=%.2f        ",data[0]);
		sprintf(displayTemp[2],"      PA5=%.2f        ",data[1]);
		sprintf(displayTemp[3],"      PA1=%d          ",(int)(1000000/P1));
		sprintf(displayTemp[4],"                ");
	}		
	//1-������ʾ 
	else if(mod == 1)
	{
		sprintf(displayTemp[0],"         PARA    ");
		sprintf(displayTemp[1],"      X=%d      ",para[0]);
		sprintf(displayTemp[2],"      Y=%d      ",para[1]);
		sprintf(displayTemp[3],"                   ");
		sprintf(displayTemp[4],"                   ");
	}
	//2-��¼����
	else if(mod == 2)
	{
		sprintf(displayTemp[0],"         REC-PA%d",paText[keyB4[1]].nameID);
		sprintf(displayTemp[1],"      N=%d          ",paText[keyB4[1]].n);
		sprintf(displayTemp[2],"      A=%.2f        ",paText[keyB4[1]].a);
		sprintf(displayTemp[3],"      T=%.2f        ",paText[keyB4[1]].t);
		sprintf(displayTemp[4],"      H=%.2f        ",paText[keyB4[1]].h);
	}
	
	LCD_DisplayStringLine(Line1,(uint8_t*)displayTemp[0]);
	LCD_DisplayStringLine(Line3,(uint8_t*)displayTemp[1]);
	LCD_DisplayStringLine(Line4,(uint8_t*)displayTemp[2]);
	LCD_DisplayStringLine(Line5,(uint8_t*)displayTemp[3]);
	LCD_DisplayStringLine(Line6,(uint8_t*)displayTemp[4]);
}

/**************************************************
* �������ܣ�LED��������
* ����������
*		   int mod1��LED3�Ĺ���ģʽ
*		   int mod2��LED4�Ĺ���ģʽ
* ��������ֵ����
***************************************************/
void ledPro(int mod1,int mod2)
{
	changeAllLedByStateNumber(0);
	
	// ��Ƶ
	if(keyB4[0]%2 == 0)
		changeLedStateByLocation(LED1,1);
	// ��Ƶ
	if(keyB4[0]%2 == 1)
		changeLedStateByLocation(LED2,1);
	// ��ѹ
	if(mod1%2==1 && uiTime7Count%50)
		changeLedStateByLocation(LED3,uiTime7Count/50%2);
	// LCD����
	if(mod2%2 == 0)
		changeLedStateByLocation(LED4,1);
}

/**************************************************
* �������ܣ����ڹ�������
* ������������
* ��������ֵ����
***************************************************/
void usartPro(void)
{
	char usartTemp[10];
	
	// ��֤�Ƿ��յ�������Ϣ�Լ�������Ϣ����
	// ����
	if(ucRxbuff[0] == 'X')
		sprintf(usartTemp,"X:%d\r\n",para[0]);
	else if(ucRxbuff[0] == 'Y')
		sprintf(usartTemp,"Y:%d\r\n",para[1]);
	// ��ʾ�л�
	else if(ucRxbuff[0] == '#')
		setDisplayMod(++LCDMod%2);
	
	// ����
	if(ucRxbuff[0] == 'P')
	{
		if(strcmp((char*)ucRxbuff,"PA1") == 0)
			sprintf(usartTemp,"PA1:%d\r\n",(int)(1000000/P1));
		else if(strcmp((char*)ucRxbuff,"PA4") == 0)
			sprintf(usartTemp,"PA4:%.2f\r\n",data[0]);
		else if(strcmp((char*)ucRxbuff,"PA5") == 0)
			sprintf(usartTemp,"PA5:%.2f\r\n",data[1]);	
	}
	
	// �Ƿ�������
	if(usartTemp[0]=='P' || usartTemp[0]=='X' || usartTemp[0]=='Y')
		HAL_UART_Transmit(&huart1,(uint8_t*)usartTemp,sizeof(char)*strlen(usartTemp),10);
	
	memset(ucRxbuff,0,sizeof(ucRxbuff));
}

/**************************************************
* �������ܣ���ѹ��������
* ������������
* ��������ֵ����
***************************************************/

void elePro(void)
{	
	int i=0;
	
	// ��ȡADC����
	getManyADC(&hadc2,data,2);
	// ����ȡ����ADC���ݴ��浽������
	while(i < 2)
	{	
		if(data[i] > paText[i].a)
			paText[i].a = data[i];
		if(data[i] < paText[i].t || paText[i].n==0)
			paText[i].t = data[i];
		paText[i].data[paText[i].n++%100] = data[i];
		paText[i].h = (paText[i].h*(paText[i].n-1)+data[i])*1.0/paText[i].n;
		i++;
	}
}

/**************************************************
* �������ܣ�Ƶ�ʹ�������
* ������������
* ��������ֵ����
***************************************************/
void FrePro(int mod)
{
	int frd = 0,cmpraeData = 0;

	// ��Ƶ  keyB4[0]%2
	if(mod == 0)
	{
		frd = (int)(P1/para[0]-1);
		cmpraeData = (int)(0.5f*P1/para[0]);
	}
	// ��Ƶ
	else
	{
		frd = (int)(P1*para[0]-1);
		cmpraeData = (int)(0.5f*P1*para[0]);
	}
	//����Ƶ��
	__HAL_TIM_SetAutoreload(&htim17,frd);
	//����ռ�ձ�
	__HAL_TIM_SetCompare(&htim17,TIM_CHANNEL_1,cmpraeData);
	HAL_TIM_GenerateEvent(&htim17, TIM_EVENTSOURCE_UPDATE);
}

#endif

