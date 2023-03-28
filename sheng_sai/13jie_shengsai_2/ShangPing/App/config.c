#include "config.h"

//��������洢��Ʒ��Ϣ
struct goods*goods[2];
//��¼EEPROM��ֵ
struct eepromMsg*eepromMsg[2];
//��¼������ֵ
unsigned char keyValue = 0;
//��¼��ʱ��7���жϴ��� ��ֵ˵����0-���ڼ�¼LED1��ʱ�� 1-���ڼ�¼LED2��ʱ��  2-����ͨ��
unsigned int uiTime7Count[3] = {0,0,0};
//��ʼ��¼��ʱ��7�жϴ����ı�־ ��ֵ˵����0-��ͣ 1-��ʼ  λ��˵����0-��¼LED1��ʱ���־ 1-��¼LED2��ʱ���־  2-ͨ�ñ�־
unsigned int uiTime7CountFlag[3] = {0,0,1};
//��ʾģʽ
unsigned char displayMode = 0;
//��¼ȷ����־
unsigned char checkFlag = 0;
//��¼���ڽ��յ���ֵ
unsigned char cRxBuff[1] = {0};
//�����ж��ϴν��յ����������Ƿ������
unsigned int iRxFlag = 0;
//���ڸ���д����ȡEEPROM��λ��
unsigned char ucEepromLocation[2] = {0,2}; 

/*************************************
* �������ܣ��Զ����ϵͳ��ʼ������
* ������������
* ��������ֵ����
*************************************/
void sysInit(void)
{
	//��¼EEPROM��0xa4 0xa5λ�õ�ֵ
	unsigned char ucEdata[10] = {0,0};
	//forѭ������ʹ��
	int i;
	
	//LCD��ʾ����ʼ��
	lcdInit();
	//IIC��ʼ��
	I2CInit();
	//�򿪶�ʱ��7�ж�
	HAL_TIM_Base_Start_IT(&htim7);
	//�򿪴��ڵ��жϽ��չ���
	HAL_UART_Receive_IT(&huart1,(uint8_t *)&cRxBuff,sizeof(cRxBuff)); 
	//�򿪶�ʱ��2ͨ��2��PWM�������
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);

	for(i=0;i<2;i++)
	{
		//��ʼ����Ʒ��Ϣ
		goods[i] = goodsInit(10,1.0,0);
		//EEPROM�ṹ���ʼ��
		eepromMsg[i] = eepromInit(0,0);
	}
	
	//��ȡEEPROM�ض�λ�ò鿴���豸�Ƿ�������
	ucEdata[0] = readEepromByBit(0xa4);
	HAL_Delay(10);
	ucEdata[1] = readEepromByBit(0xa5);
	
	//�жϹ̶�λ���Ƿ�洢����Ϣ  ���ʾ���Ǹ��豸�Ƿ��һ���������
	if(ucEdata[0]==55 && ucEdata[1]==66) 
	{
		//����EEPROM����Ʒ���������۸���Ϣ
		for(i=0;i<2;i++)
		{
			eepromMsg[i]->count = readEepromByBit(0xa0+ucEepromLocation[i]);
			goods[i]->iGoodsCount = eepromMsg[i]->count;
			HAL_Delay(10);
			
			eepromMsg[i]->price = readEepromByBit(0xa1+ucEepromLocation[i])*1.0/10;
			goods[i]->dGoodsPrice = eepromMsg[i]->price;
			HAL_Delay(10);	
		}	
	}
	else
	{
		//��EEPROMд�����ݱ������豸�Ѿ�������
		for(i=0;i<2;i++)
		{
			writeEepromByBit(0xa4+i,55+11*i);
			HAL_Delay(10);
		}
	}
}

/*************************************
* �������ܣ�ϵͳ���߼�����
* ������������
* ��������ֵ����
*************************************/
void sysWork(void)
{
	//forѭ������ʹ��
	int i=0;
	
	//��������
	keyPro();
	//�����ж���ֵ�Ƿ����Ҫ��
	for(i=0;i<2;i++)
		if(goods[i]->dGoodsPrice >=2.1)
			goods[i]->dGoodsPrice = 1.0;
		else if(goods[i]->dGoodsPrice < 1.0)
			goods[i]->dGoodsPrice = 2.0;
	//������ʾ
	display();
	//�������ݴ���
	usartProcess();
	//LED����
	LedDisplay();
	//PWM����
	pwmWorkState();
		
	//��Ʒȷ�Ϲ���
	if(checkFlag)
	{
		//��Ʒȷ�Ϲ���������  ��ողŵĽ�����Ϣ
		for(int i=0;i<2;i++)
		{
			goods[i]->iGoodsCount -= goods[i]->iGoodsBuyCount;
			goods[i]->iGoodsBuyCount = 0;
		}
		//����ȷ�ϵı�־λ
		checkFlag = 0;
	}
			
	//�жϿ�������Ƿ��EEPROM�洢�Ƿ���ͬ ��ͬ��д��
	for(i=0;i<2;i++)
	{
		if(goods[i]->iGoodsCount!=eepromMsg[i]->count && uiTime7Count[2]%5==(0+ucEepromLocation[i]))
		{
			writeEepromByBit(0xa0+ucEepromLocation[i],(unsigned char)goods[i]->iGoodsCount);
			eepromMsg[i]->count = goods[i]->iGoodsCount;
		}
		if(goods[i]->dGoodsPrice!=eepromMsg[i]->price && uiTime7Count[2]%5==(1+ucEepromLocation[i]))
		{
			writeEepromByBit(0xa1+ucEepromLocation[i],(unsigned char)(goods[i]->dGoodsPrice*10));
			eepromMsg[i]->price = goods[i]->dGoodsPrice;
		}
	}
}

/*************************************
* �������ܣ�������������
* ������������
* ��������ֵ����
*************************************/
void keyPro(void)
{
	//����ɨ��
	keyValue = scanKey();
	switch(keyValue)
	{
		//����B1
		case 1:
			//�л�LCD��ʾ����
			if(++displayMode == 3) displayMode = 0;
			break;
		//����B2
		case 2:
			//�������  ����������1
			if(displayMode == 0)
				if(++goods[0]->iGoodsBuyCount == goods[0]->iGoodsCount+1)
					goods[0]->iGoodsBuyCount = 0;
			//��Ʒ�۸����  ���ۼ�0.1
			if(displayMode == 1)
				goods[0]->dGoodsPrice += 0.1;	
			//������  ����1
			if(displayMode == 2)
				goods[0]->iGoodsCount++;
			break;
		//����B3
		case 3:
			//�������  ����������1
			if(displayMode == 0)
				if(++goods[1]->iGoodsBuyCount == goods[1]->iGoodsCount+1)
					goods[1]->iGoodsBuyCount = 0;
			//��Ʒ�۸����  ���ۼ�0.1
			if(displayMode == 1)
				goods[1]->dGoodsPrice += 0.1;	
			//������  ����1
			if(displayMode == 2)
				goods[1]->iGoodsCount++;
			break;
		//����B4
		case 4:
			if(displayMode == 0)
				checkFlag = 1;
			break;
		//����
		default : break;
	}	
	keyValue = 0;
}

/*******************************************************
* �������ܣ�EEPROM��Ϣ��ʼ��
* ����������
*			int count������
*			double price���۸�
* ��������ֵ��
*			struct eepromMsg*res����ʼ����ɵ�EEPROM��Ϣ�ṹ��
*********************************************************/
struct eepromMsg*eepromInit(int count,unsigned char price)
{
	struct eepromMsg*res = (struct eepromMsg*)malloc(sizeof(struct eepromMsg));
	
	//EEPROM�ṹ����Ϣ
	res->count = count;
	res->price = price;
	
	return res;
}

/*******************************************************
* �������ܣ���Ʒ��Ϣ��ʼ��
* ����������
*			int iGoodsCount����Ʒ����
*			double iGoodsBuyCount����Ʒ��������
*			double dGoodsPrice����Ʒ�۸�
* ��������ֵ��
*			struct goods*goods����ʼ����ɵ���Ʒ��Ϣ
*********************************************************/
struct goods*goodsInit(int iGoodsCount,double dGoodsPrice,int iGoodsBuyCount)
{
	struct goods*goods = (struct goods*)malloc(sizeof(struct goods));
	
	//��ʼ����Ʒ��Ϣ
	goods->dGoodsPrice = dGoodsPrice;
	goods->iGoodsCount = iGoodsCount;
	goods->iGoodsBuyCount = iGoodsBuyCount;
	
	return goods;
}

/*************************************
* �������ܣ�������������
* ������������
* ��������ֵ����
*************************************/
void LedDisplay(void)
{
	//�ر�����LED
	changeAllLedByStateNumber(0);
	//ȷ�Ϲ�����Ʒ
	if(checkFlag)
	{
		uiTime7CountFlag[0] = 1;
	}
	//�ж�LEDʱ���Ƿ��������
	if(uiTime7CountFlag[0] &&  uiTime7Count[0]%50<50)
		changeLedStateByLocation(LED1,1);
	if(uiTime7Count[0]%55>51)
	{
		changeLedStateByLocation(LED1,0);
		uiTime7CountFlag[0] = 0;
		uiTime7Count[0] = 0;
	}
	
	//��Ʒ������Ϊ0
	if(!goods[0]->iGoodsCount && !goods[1]->iGoodsCount)
		uiTime7CountFlag[1] = 1;
	else
	{
		uiTime7CountFlag[1] = 0;
		uiTime7Count[1] = 0;
	}
	//�ж�ʱ���Ƿ���0.1�� 
	if(uiTime7Count[1]%2 == 1)
		rollbackLedByLocation(LED2);
}

/*************************************
* �������ܣ�PWM����л�����
* ������������
* ��������ֵ����
*************************************/
void pwmWorkState(void)
{	
	//���ռ�ձ�Ϊ30%  ������ж������ǽ���LED1���ж����� ��Ϊ���ǵ�ʱ��������һ����
	if(uiTime7CountFlag[0] &&  uiTime7Count[0]%50<50)
		__HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_2,150);
	//���ռ�ձ�Ϊ5%
	if(uiTime7Count[0]%55>51)
		__HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_2,25);
}

/*************************************
* �������ܣ��������ݴ�����
* ������������
* ��������ֵ����
*************************************/
void usartProcess(void)
{
	char temp[20] = {0};
	
	//ȷ�Ϲ��������Ʒ��Ϣ
	if(checkFlag)
	{
		sprintf(temp,"X:%d,Y:%d,Z:%.1f\r\n",goods[0]->iGoodsBuyCount,goods[1]->iGoodsBuyCount,
				(goods[0]->dGoodsPrice*goods[0]->iGoodsBuyCount+goods[1]->dGoodsPrice*goods[1]->iGoodsBuyCount));
		HAL_UART_Transmit(&huart1,(uint8_t*)temp,sizeof(temp),50);
	}
	
	//��ѯ��Ʒ ����Ŀǰ����Ʒ��Ϣ
	if(iRxFlag==1 && strcmp((char*)cRxBuff,(char*)"?") == 0)
	{
		sprintf(temp,"X:%.1f,Y:%.1f\r\n",goods[0]->dGoodsPrice,goods[1]->dGoodsPrice);
		HAL_UART_Transmit(&huart1,(uint8_t*)temp,sizeof(temp),50);
	}
	//��մ��ڽ��յ��������Լ��򿪴�����
	memset(cRxBuff,'\0',sizeof(cRxBuff));
	iRxFlag = 0;	
}

/*************************************
* �������ܣ�������ʾ
* ������������
* ��������ֵ����
*************************************/
void display(void)
{
	char temp[20]  = {0};
	
	//���������ʾ
	if(displayMode == 0)
	{
		LCD_DisplayStringLine(Line1,(uint8_t*)"        SHOP ");
		sprintf(temp,"     X:%d       ",goods[0]->iGoodsBuyCount);
		LCD_DisplayStringLine(Line3,(uint8_t*)temp);
		sprintf(temp,"     Y:%d       ",goods[1]->iGoodsBuyCount);
		LCD_DisplayStringLine(Line4,(uint8_t*)temp);
	}
	//��Ʒ�۸������ʾ
	else if(displayMode == 1)	
	{
		LCD_DisplayStringLine(Line1,(uint8_t*)"        PRICE   ");
		sprintf(temp,"     X:%.1f    ",goods[0]->dGoodsPrice);
		LCD_DisplayStringLine(Line3,(uint8_t*)temp);
		sprintf(temp,"     Y:%.1f    ",goods[1]->dGoodsPrice);
		LCD_DisplayStringLine(Line4,(uint8_t*)temp);
	}
	//�����Ϣ������ʾ
	else if(displayMode == 2)
	{
		LCD_DisplayStringLine(Line1,(uint8_t*)"        REP   ");
		sprintf(temp,"     X:%d      ",goods[0]->iGoodsCount);
		LCD_DisplayStringLine(Line3,(uint8_t*)temp);
		sprintf(temp,"     Y:%d       ",goods[1]->iGoodsCount);
		LCD_DisplayStringLine(Line4,(uint8_t*)temp);
	}
}
