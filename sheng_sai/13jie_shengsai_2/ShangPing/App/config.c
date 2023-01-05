#include "config.h"

//定义变量存储商品信息
struct goods*goodsX;
struct goods*goodsY;
//记录EEPROM的值
struct eepromMsg*eepromMsgX;
struct eepromMsg*eepromMsgY;
//记录按键的值
unsigned char keyValue = 0;
//记录定时器7的中断次数
unsigned int uiTime7CountLED1 = 0;
//记录定时器7的中断次数是否开始 0-停止 1-开始
unsigned int uiTime7CountLED1Flag = 0;
//记录定时器7的中断次数
unsigned int uiTime7Count= 0;
//记录定时器7的中断次数
unsigned int uiTime7CountLED2 = 0;
//记录定时器7的中断次数是否开始 0-停止 1-开始
unsigned int uiTime7CountLED2Flag = 0;
//显示模式
unsigned char displayMode = 0;
//记录确定标志
unsigned char checkFlag = 0;

//记录串口接收到的值
unsigned char cRxBuff[1] = {49};
unsigned int iRxFlag = 0;



/*************************************
* 函数功能：自定义的系统初始化函数
* 函数参数：无
* 函数返回值：无
*************************************/
void sysInit(void)
{
	unsigned char ucEdata[10] = {0,0};

	//LCD显示屏初始化
	lcdInit();
	//IIC初始化
	I2CInit();
	//打开定时器7中断
	HAL_TIM_Base_Start_IT(&htim7);
	//打开串口的中断接收功能
	HAL_UART_Receive_IT(&huart1,(uint8_t *)&cRxBuff,sizeof(cRxBuff)); 
	//打开定时器2通道2的PWM输出功能
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);

	//初始化商品信息
	goodsX = goodsInit(10,1.0,0);
	goodsY = goodsInit(10,1.0,0);
	//EEPROM结构体初始化
	eepromMsgX = eepromInit(0,0);
	eepromMsgY = eepromInit(0,0);
	
	ucEdata[0] = readEepromByBit(0xa4);
	ucEdata[1] = readEepromByBit(0xa5);
	
	//判断固定位置是否存储过信息  其表示的是该设备是否第一次烧入代码
	if(ucEdata[0]==55 && ucEdata[1]==66) 
	{
		//加载EEPROM的数量、价格信息
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
* 函数功能：系统的逻辑函数
* 函数参数：无
* 函数返回值：无
*************************************/
void sysWork(void)
{
//	char temp[20] = {0};
	
	//按键工作
	keyPro();
	
	if(goodsX->dGoodsPrice >=2.1)
		goodsX->dGoodsPrice = 1.0;
	else if(goodsX->dGoodsPrice < 1.0)
		goodsX->dGoodsPrice = 2.0;
	
	if(goodsY->dGoodsPrice >=2.1)
		goodsY->dGoodsPrice = 1.0;	
	else if(goodsX->dGoodsPrice < 1.0)
		goodsX->dGoodsPrice = 2.0;
				
	//购买界面
	if(displayMode==0)
		shopDisplay();
	//商品价格界面
	else if(displayMode==1)
		priceDisplay();
	//库存信息界面
	else if(displayMode==2)
		repDisplay();
	//串口数据处理
	usartProcess();
	//LED工作
	LedDisplay();
	//PWM工作
	pwmWorkState();
	
	//判断库存数量是否跟EEPROM相同 不同就写入
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
	
	//商品确认购买
	if(checkFlag)
	{
		//商品确认购买后库存减少
		goodsX->iGoodsCount -= goodsX->iGoodsBuyCount;
		goodsY->iGoodsCount -= goodsY->iGoodsBuyCount;
		//清空刚才的交易信息
		goodsX->iGoodsBuyCount = 0;
		goodsY->iGoodsBuyCount = 0;
		//消除确认的标志位
		checkFlag = 0;
	}
	
//	//查看EEPROM中的数据
//	sprintf(temp,"eXC:%d eXp%.2f",eepromMsgX->count,eepromMsgX->price);
//	LCD_DisplayStringLine(Line8,(uint8_t*)temp);
//	sprintf(temp,"eYC:%d eYp:%.2f",eepromMsgY->count,eepromMsgY->price);
//	LCD_DisplayStringLine(Line9,(uint8_t*)temp);
}

/*******************************************************
* 函数功能：EEPROM信息初始化
* 函数参数：
*			int count：数量
*			double price：价格
* 函数返回值：
*			struct eepromMsg*res：初始化完成的EEPROM信息结构体
*********************************************************/
struct eepromMsg*eepromInit(int count,unsigned char price)
{
	struct eepromMsg*res = (struct eepromMsg*)malloc(sizeof(struct eepromMsg));
	
	//EEPROM结构体信息
	res->count = count;
	res->price = price;
	
	return res;
}

/*******************************************************
* 函数功能：商品信息初始化
* 函数参数：
*			int iGoodsCount：商品数量
*			double iGoodsBuyCount：商品购买数量
*			double dGoodsPrice：商品价格
* 函数返回值：
*			struct goods*goods：初始化完成的商品信息
*********************************************************/
struct goods*goodsInit(int iGoodsCount,double dGoodsPrice,int iGoodsBuyCount)
{
	struct goods*goods = (struct goods*)malloc(sizeof(struct goods));
	
	//初始化商品信息
	goods->dGoodsPrice = dGoodsPrice;
	goods->iGoodsCount = iGoodsCount;
	goods->iGoodsBuyCount = iGoodsBuyCount;
	
	return goods;
}


/*************************************
* 函数功能：按键工作函数
* 函数参数：无
* 函数返回值：无
*************************************/
void keyPro(void)
{
	//按键扫描
	keyValue = scanKey();
	switch(keyValue)
	{
		//按键B1
		case 1:
			//切换LCD显示界面
			if(++displayMode == 3) displayMode = 0;
			break;
		//按键B2
		case 2:
			//购买界面  购买数量加1
			if(displayMode == 0)
				if(++goodsX->iGoodsBuyCount == goodsX->iGoodsCount+1)
					goodsX->iGoodsBuyCount = 0;
			//商品价格界面  单价加0.1
			if(displayMode == 1)
				goodsX->dGoodsPrice += 0.1;	
			//库存界面  库存加1
			if(displayMode == 2)
				goodsX->iGoodsCount++;
			break;
		//按键B3
		case 3:
			//购买界面  购买数量加1
			if(displayMode == 0)
				if(++goodsY->iGoodsBuyCount == goodsY->iGoodsCount+1)
					goodsY->iGoodsBuyCount = 0;
			//商品价格界面  单价加0.1
			if(displayMode == 1)
				goodsY->dGoodsPrice += 0.1;	
			//库存界面  库存加1
			if(displayMode == 2)
				goodsY->iGoodsCount++;
			break;
		//按键B4
		case 4:
			if(displayMode == 0)
				checkFlag = 1;
			break;
		//其他
		default :break;
	}	
	keyValue = 0;
}

/*************************************
* 函数功能：按键工作函数
* 函数参数：无
* 函数返回值：无
*************************************/
void LedDisplay(void)
{
	//关闭所有LED
	changeAllLedByStateNumber(0);
	//确认购买商品
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
	
	//商品数量都为0
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
* 函数功能：PWM输出切换函数
* 函数参数：无
* 函数返回值：无
*************************************/
void pwmWorkState(void)
{
	//输出占空比为30%
	if(uiTime7CountLED1Flag && 0<uiTime7CountLED1%51 && uiTime7CountLED1%51<=51)
	{
		__HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_2,30);
	}
	//输出占空比为5%
	else
	{
		__HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_2,5);
	}
}

/*************************************
* 函数功能：串口数据处理函数
* 函数参数：无
* 函数返回值：无
*************************************/
void usartProcess(void)
{
	char temp[20] = {0};
	
	//确认购买后发送商品信息
	if(checkFlag)
	{
		sprintf(temp,"X:%d,Y:%d,Z:%.1f\r\n",goodsX->iGoodsBuyCount,goodsY->iGoodsBuyCount,
				(goodsX->dGoodsPrice*goodsX->iGoodsBuyCount+goodsY->dGoodsPrice*goodsY->iGoodsBuyCount));
		HAL_UART_Transmit(&huart1,(uint8_t*)temp,sizeof(temp),50);
	}
	
	//查询商品 发送目前的商品信息
	if(iRxFlag==1 && strcmp((char*)cRxBuff,(char*)"?") == 0)
	{
		sprintf(temp,"X:%.1f,Y:%.1f\r\n",goodsX->dGoodsPrice,goodsY->dGoodsPrice);
		HAL_UART_Transmit(&huart1,(uint8_t*)temp,sizeof(temp),50);
	}
	//清空串口接收到的数据以及打开串口锁
	memset(cRxBuff,'\0',sizeof(cRxBuff));
	iRxFlag = 0;	
}

/*************************************
* 函数功能：商品购买界面显示
* 函数参数：无
* 函数返回值：无
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
* 函数功能：商品价格界面显示
* 函数参数：无
* 函数返回值：无
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
* 函数功能：商品库存界面显示
* 函数参数：无
* 函数返回值：无
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

