#include "config.h"

//��������洢��Ʒ��Ϣ
struct goods*goodsX;
struct goods*goodsY;
//��¼EEPROM��ֵ
struct eepromMsg*eepromMsgX;
struct eepromMsg*eepromMsgY;
//��¼������ֵ
unsigned char keyValue = 0;
//��¼��ʱ��7���жϴ���
unsigned int uiTime7CountLED1 = 0;
//��¼��ʱ��7���жϴ����Ƿ�ʼ 0-ֹͣ 1-��ʼ
unsigned int uiTime7CountLED1Flag = 0;
//��¼��ʱ��7���жϴ���
unsigned int uiTime7Count= 0;
//��¼��ʱ��7���жϴ���
unsigned int uiTime7CountLED2 = 0;
//��¼��ʱ��7���жϴ����Ƿ�ʼ 0-ֹͣ 1-��ʼ
unsigned int uiTime7CountLED2Flag = 0;
//��ʾģʽ
unsigned char displayMode = 0;
//��¼ȷ����־
unsigned char checkFlag = 0;

//��¼���ڽ��յ���ֵ
unsigned char cRxBuff[1] = {49};
unsigned int iRxFlag = 0;



/*************************************
* �������ܣ��Զ����ϵͳ��ʼ������
* ������������
* ��������ֵ����
*************************************/
void sysInit(void)
{
	unsigned char ucEdata[10] = {0,0};

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

	//��ʼ����Ʒ��Ϣ
	goodsX = goodsInit(10,1.0,0);
	goodsY = goodsInit(10,1.0,0);
	//EEPROM�ṹ���ʼ��
	eepromMsgX = eepromInit(0,0);
	eepromMsgY = eepromInit(0,0);
	
	ucEdata[0] = readEepromByBit(0xa4);
	ucEdata[1] = readEepromByBit(0xa5);
	
	//�жϹ̶�λ���Ƿ�洢����Ϣ  ���ʾ���Ǹ��豸�Ƿ��һ���������
	if(ucEdata[0]==55 && ucEdata[1]==66) 
	{
		//����EEPROM���������۸���Ϣ
		eepromMsgX->count = readEepromByBit(0xa0);
		goodsX->iGoodsCount = eepromMsgX->count;
		HAL_Delay(10);
		
		eepromMsgX->price = readEepromByBit(0xa1)*1.0/10;
		goodsX->dGoodsPrice = eepromMsgX->price;
		HAL_Delay(10);	
		
		eepromMsgY->count = readEepromByBit(0xa2);
		goodsY->iGoodsCount = eepromMsgY->count;
		HAL_Delay(10);
		
		eepromMsgY->price = readEepromByBit(0xa3)*1.0/10;
		goodsY->dGoodsPrice = eepromMsgY->price;
		HAL_Delay(10);		
	}
	else
	{
		writeEepromByBit(0xa4,55);
		HAL_Delay(10);
		writeEepromByBit(0xa5,66);
	}
}

/*************************************
* �������ܣ�ϵͳ���߼�����
* ������������
* ��������ֵ����
*************************************/
void sysWork(void)
{
//	char temp[20] = {0};
	
	//��������
	keyPro();
	
	if(goodsX->dGoodsPrice >=2.1)
		goodsX->dGoodsPrice = 1.0;
	else if(goodsX->dGoodsPrice < 1.0)
		goodsX->dGoodsPrice = 2.0;
	
	if(goodsY->dGoodsPrice >=2.1)
		goodsY->dGoodsPrice = 1.0;	
	else if(goodsX->dGoodsPrice < 1.0)
		goodsX->dGoodsPrice = 2.0;
				
	//�������
	if(displayMode==0)
		shopDisplay();
	//��Ʒ�۸����
	else if(displayMode==1)
		priceDisplay();
	//�����Ϣ����
	else if(displayMode==2)
		repDisplay();
	//�������ݴ���
	usartProcess();
	//LED����
	LedDisplay();
	//PWM����
	pwmWorkState();
	
	//�жϿ�������Ƿ��EEPROM��ͬ ��ͬ��д��
	if(goodsX->iGoodsCount != eepromMsgX->count && uiTime7Count%50==10)
	{
		writeEepromByBit(0xa0,(unsigned char)goodsX->iGoodsCount);
		eepromMsgX->count = goodsX->iGoodsCount;
	}
	if(goodsX->dGoodsPrice != eepromMsgX->count && uiTime7Count%50==10)
	{
		writeEepromByBit(0xa1,(unsigned char)(goodsX->dGoodsPrice*10));
		eepromMsgX->price = goodsX->dGoodsPrice*1.0/10;
	}
	
	if(goodsY->iGoodsCount != eepromMsgY->count && uiTime7Count%50==30)
	{
		writeEepromByBit(0xa2,(unsigned char)goodsY->iGoodsCount);
		eepromMsgY->count = goodsY->iGoodsCount;
	}
	if(goodsY->dGoodsPrice != eepromMsgY->price && uiTime7Count%50==40)
	{
		writeEepromByBit(0xa3,(unsigned char)(goodsY->dGoodsPrice*10));
		eepromMsgY->price = goodsY->dGoodsPrice*1.0/10;
	}
	
	//��Ʒȷ�Ϲ���
	if(checkFlag)
	{
		//��Ʒȷ�Ϲ���������
		goodsX->iGoodsCount -= goodsX->iGoodsBuyCount;
		goodsY->iGoodsCount -= goodsY->iGoodsBuyCount;
		//��ողŵĽ�����Ϣ
		goodsX->iGoodsBuyCount = 0;
		goodsY->iGoodsBuyCount = 0;
		//����ȷ�ϵı�־λ
		checkFlag = 0;
	}
	
//	//�鿴EEPROM�е�����
//	sprintf(temp,"eXC:%d eXp%.2f",eepromMsgX->count,eepromMsgX->price);
//	LCD_DisplayStringLine(Line8,(uint8_t*)temp);
//	sprintf(temp,"eYC:%d eYp:%.2f",eepromMsgY->count,eepromMsgY->price);
//	LCD_DisplayStringLine(Line9,(uint8_t*)temp);
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
				if(++goodsX->iGoodsBuyCount == goodsX->iGoodsCount+1)
					goodsX->iGoodsBuyCount = 0;
			//��Ʒ�۸����  ���ۼ�0.1
			if(displayMode == 1)
				goodsX->dGoodsPrice += 0.1;	
			//������  ����1
			if(displayMode == 2)
				goodsX->iGoodsCount++;
			break;
		//����B3
		case 3:
			//�������  ����������1
			if(displayMode == 0)
				if(++goodsY->iGoodsBuyCount == goodsY->iGoodsCount+1)
					goodsY->iGoodsBuyCount = 0;
			//��Ʒ�۸����  ���ۼ�0.1
			if(displayMode == 1)
				goodsY->dGoodsPrice += 0.1;	
			//������  ����1
			if(displayMode == 2)
				goodsY->iGoodsCount++;
			break;
		//����B4
		case 4:
			if(displayMode == 0)
				checkFlag = 1;
			break;
		//����
		default :break;
	}	
	keyValue = 0;
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
		uiTime7CountLED1Flag = 1;
	}
	if(uiTime7CountLED1Flag &&  uiTime7CountLED1%50<50)
		changeLedStateByLocation(LED1,1);
	if(uiTime7CountLED1%55>51)
	{
		changeLedStateByLocation(LED1,0);
		uiTime7CountLED1Flag = 0;
		uiTime7CountLED1 = 0;
	}
	
	//��Ʒ������Ϊ0
	if(!goodsX->iGoodsCount && !goodsY->iGoodsCount)
		uiTime7CountLED2Flag = 1;
	else
	{
		uiTime7CountLED2Flag = 0;
		uiTime7CountLED2 = 0;
	}
	if(uiTime7CountLED2%2 == 1)
		rollbackLedByLocation(LED2);
}

/*************************************
* �������ܣ�PWM����л�����
* ������������
* ��������ֵ����
*************************************/
void pwmWorkState(void)
{
	//���ռ�ձ�Ϊ30%
	if(uiTime7CountLED1Flag && 0<uiTime7CountLED1%51 && uiTime7CountLED1%51<=51)
	{
		__HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_2,30);
	}
	//���ռ�ձ�Ϊ5%
	else
	{
		__HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_2,5);
	}
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
		sprintf(temp,"X:%d,Y:%d,Z:%.1f\r\n",goodsX->iGoodsBuyCount,goodsY->iGoodsBuyCount,
				(goodsX->dGoodsPrice*goodsX->iGoodsBuyCount+goodsY->dGoodsPrice*goodsY->iGoodsBuyCount));
		HAL_UART_Transmit(&huart1,(uint8_t*)temp,sizeof(temp),50);
	}
	
	//��ѯ��Ʒ ����Ŀǰ����Ʒ��Ϣ
	if(iRxFlag==1 && strcmp((char*)cRxBuff,(char*)"?") == 0)
	{
		sprintf(temp,"X:%.1f,Y:%.1f\r\n",goodsX->dGoodsPrice,goodsY->dGoodsPrice);
		HAL_UART_Transmit(&huart1,(uint8_t*)temp,sizeof(temp),50);
	}
	//��մ��ڽ��յ��������Լ��򿪴�����
	memset(cRxBuff,'\0',sizeof(cRxBuff));
	iRxFlag = 0;	
}

/*************************************
* �������ܣ���Ʒ���������ʾ
* ������������
* ��������ֵ����
*************************************/
void shopDisplay(void)
{
	char temp[20]  = {0};
	LCD_DisplayStringLine(Line1,(uint8_t*)"        SHOP ");
	sprintf(temp,"     X:%d       ",goodsX->iGoodsBuyCount);
	LCD_DisplayStringLine(Line3,(uint8_t*)temp);
	sprintf(temp,"     Y:%d       ",goodsY->iGoodsBuyCount);
	LCD_DisplayStringLine(Line4,(uint8_t*)temp);
}

/*************************************
* �������ܣ���Ʒ�۸������ʾ
* ������������
* ��������ֵ����
*************************************/
void priceDisplay(void)
{
	char temp[20]  = {0};
	LCD_DisplayStringLine(Line1,(uint8_t*)"        PRICE   ");
	sprintf(temp,"     X:%.1f    ",goodsX->dGoodsPrice);
	LCD_DisplayStringLine(Line3,(uint8_t*)temp);
	sprintf(temp,"     Y:%.1f    ",goodsY->dGoodsPrice);
	LCD_DisplayStringLine(Line4,(uint8_t*)temp);
}

/*************************************
* �������ܣ���Ʒ��������ʾ
* ������������
* ��������ֵ����
*************************************/
void repDisplay(void)
{
	char temp[20]  = {0};
	LCD_DisplayStringLine(Line1,(uint8_t*)"        REP   ");
	sprintf(temp,"     X:%d      ",goodsX->iGoodsCount);
	LCD_DisplayStringLine(Line3,(uint8_t*)temp);
	sprintf(temp,"     Y:%d       ",goodsY->iGoodsCount);
	LCD_DisplayStringLine(Line4,(uint8_t*)temp);
}

