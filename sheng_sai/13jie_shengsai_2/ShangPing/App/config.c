#include "config.h"

//定义变量存储商品信息
struct goods*goods[2];
//记录EEPROM的值
struct eepromMsg*eepromMsg[2];
//记录按键的值
unsigned char keyValue = 0;
//记录定时器7的中断次数 数值说明：0-用于记录LED1的时间 1-用于记录LED2的时间  2-用于通用
unsigned int uiTime7Count[3] = {0,0,0};
//开始记录定时器7中断次数的标志 数值说明：0-暂停 1-开始  位置说明：0-记录LED1的时间标志 1-记录LED2的时间标志  2-通用标志
unsigned int uiTime7CountFlag[3] = {0,0,1};
//显示模式
unsigned char displayMode = 0;
//记录确定标志
unsigned char checkFlag = 0;
//记录串口接收到的值
unsigned char cRxBuff[1] = {0};
//用于判断上次接收到串口数据是否处理完成
unsigned int iRxFlag = 0;
//用于辅助写入或读取EEPROM的位置
unsigned char ucEepromLocation[2] = {0,2}; 

/*************************************
* 函数功能：自定义的系统初始化函数
* 函数参数：无
* 函数返回值：无
*************************************/
void sysInit(void)
{
	//记录EEPROM的0xa4 0xa5位置的值
	unsigned char ucEdata[10] = {0,0};
	//for循环遍历使用
	int i;
	
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

	for(i=0;i<2;i++)
	{
		//初始化商品信息
		goods[i] = goodsInit(10,1.0,0);
		//EEPROM结构体初始化
		eepromMsg[i] = eepromInit(0,0);
	}
	
	//读取EEPROM特定位置查看该设备是否启动过
	ucEdata[0] = readEepromByBit(0xa4);
	HAL_Delay(10);
	ucEdata[1] = readEepromByBit(0xa5);
	
	//判断固定位置是否存储过信息  其表示的是该设备是否第一次烧入代码
	if(ucEdata[0]==55 && ucEdata[1]==66) 
	{
		//加载EEPROM中商品的数量、价格信息
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
		//向EEPROM写入数据表明该设备已经启动过
		for(i=0;i<2;i++)
		{
			writeEepromByBit(0xa4+i,55+11*i);
			HAL_Delay(10);
		}
	}
}

/*************************************
* 函数功能：系统的逻辑函数
* 函数参数：无
* 函数返回值：无
*************************************/
void sysWork(void)
{
	//for循环遍历使用
	int i=0;
	
	//按键工作
	keyPro();
	//遍历判断数值是否符合要求
	for(i=0;i<2;i++)
		if(goods[i]->dGoodsPrice >=2.1)
			goods[i]->dGoodsPrice = 1.0;
		else if(goods[i]->dGoodsPrice < 1.0)
			goods[i]->dGoodsPrice = 2.0;
	//界面显示
	display();
	//串口数据处理
	usartProcess();
	//LED工作
	LedDisplay();
	//PWM工作
	pwmWorkState();
		
	//商品确认购买
	if(checkFlag)
	{
		//商品确认购买后库存减少  清空刚才的交易信息
		for(int i=0;i<2;i++)
		{
			goods[i]->iGoodsCount -= goods[i]->iGoodsBuyCount;
			goods[i]->iGoodsBuyCount = 0;
		}
		//消除确认的标志位
		checkFlag = 0;
	}
			
	//判断库存数量是否跟EEPROM存储是否相同 不同就写入
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
				if(++goods[0]->iGoodsBuyCount == goods[0]->iGoodsCount+1)
					goods[0]->iGoodsBuyCount = 0;
			//商品价格界面  单价加0.1
			if(displayMode == 1)
				goods[0]->dGoodsPrice += 0.1;	
			//库存界面  库存加1
			if(displayMode == 2)
				goods[0]->iGoodsCount++;
			break;
		//按键B3
		case 3:
			//购买界面  购买数量加1
			if(displayMode == 0)
				if(++goods[1]->iGoodsBuyCount == goods[1]->iGoodsCount+1)
					goods[1]->iGoodsBuyCount = 0;
			//商品价格界面  单价加0.1
			if(displayMode == 1)
				goods[1]->dGoodsPrice += 0.1;	
			//库存界面  库存加1
			if(displayMode == 2)
				goods[1]->iGoodsCount++;
			break;
		//按键B4
		case 4:
			if(displayMode == 0)
				checkFlag = 1;
			break;
		//其他
		default : break;
	}	
	keyValue = 0;
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
void LedDisplay(void)
{
	//关闭所有LED
	changeAllLedByStateNumber(0);
	//确认购买商品
	if(checkFlag)
	{
		uiTime7CountFlag[0] = 1;
	}
	//判断LED时长是否符合条件
	if(uiTime7CountFlag[0] &&  uiTime7Count[0]%50<50)
		changeLedStateByLocation(LED1,1);
	if(uiTime7Count[0]%55>51)
	{
		changeLedStateByLocation(LED1,0);
		uiTime7CountFlag[0] = 0;
		uiTime7Count[0] = 0;
	}
	
	//商品数量都为0
	if(!goods[0]->iGoodsCount && !goods[1]->iGoodsCount)
		uiTime7CountFlag[1] = 1;
	else
	{
		uiTime7CountFlag[1] = 0;
		uiTime7Count[1] = 0;
	}
	//判断时间是否间隔0.1秒 
	if(uiTime7Count[1]%2 == 1)
		rollbackLedByLocation(LED2);
}

/*************************************
* 函数功能：PWM输出切换函数
* 函数参数：无
* 函数返回值：无
*************************************/
void pwmWorkState(void)
{	
	//输出占空比为30%  这里的判断条件是借用LED1的判断条件 因为他们的时长需求是一样的
	if(uiTime7CountFlag[0] &&  uiTime7Count[0]%50<50)
		__HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_2,150);
	//输出占空比为5%
	if(uiTime7Count[0]%55>51)
		__HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_2,25);
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
		sprintf(temp,"X:%d,Y:%d,Z:%.1f\r\n",goods[0]->iGoodsBuyCount,goods[1]->iGoodsBuyCount,
				(goods[0]->dGoodsPrice*goods[0]->iGoodsBuyCount+goods[1]->dGoodsPrice*goods[1]->iGoodsBuyCount));
		HAL_UART_Transmit(&huart1,(uint8_t*)temp,sizeof(temp),50);
	}
	
	//查询商品 发送目前的商品信息
	if(iRxFlag==1 && strcmp((char*)cRxBuff,(char*)"?") == 0)
	{
		sprintf(temp,"X:%.1f,Y:%.1f\r\n",goods[0]->dGoodsPrice,goods[1]->dGoodsPrice);
		HAL_UART_Transmit(&huart1,(uint8_t*)temp,sizeof(temp),50);
	}
	//清空串口接收到的数据以及打开串口锁
	memset(cRxBuff,'\0',sizeof(cRxBuff));
	iRxFlag = 0;	
}

/*************************************
* 函数功能：界面显示
* 函数参数：无
* 函数返回值：无
*************************************/
void display(void)
{
	char temp[20]  = {0};
	
	//购买界面显示
	if(displayMode == 0)
	{
		LCD_DisplayStringLine(Line1,(uint8_t*)"        SHOP ");
		sprintf(temp,"     X:%d       ",goods[0]->iGoodsBuyCount);
		LCD_DisplayStringLine(Line3,(uint8_t*)temp);
		sprintf(temp,"     Y:%d       ",goods[1]->iGoodsBuyCount);
		LCD_DisplayStringLine(Line4,(uint8_t*)temp);
	}
	//商品价格界面显示
	else if(displayMode == 1)	
	{
		LCD_DisplayStringLine(Line1,(uint8_t*)"        PRICE   ");
		sprintf(temp,"     X:%.1f    ",goods[0]->dGoodsPrice);
		LCD_DisplayStringLine(Line3,(uint8_t*)temp);
		sprintf(temp,"     Y:%.1f    ",goods[1]->dGoodsPrice);
		LCD_DisplayStringLine(Line4,(uint8_t*)temp);
	}
	//库存信息界面显示
	else if(displayMode == 2)
	{
		LCD_DisplayStringLine(Line1,(uint8_t*)"        REP   ");
		sprintf(temp,"     X:%d      ",goods[0]->iGoodsCount);
		LCD_DisplayStringLine(Line3,(uint8_t*)temp);
		sprintf(temp,"     Y:%d       ",goods[1]->iGoodsCount);
		LCD_DisplayStringLine(Line4,(uint8_t*)temp);
	}
}
