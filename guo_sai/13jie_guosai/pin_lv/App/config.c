#include "config.h"

//定义一个串口信息的结构
uint8_t ucRxbuff[4];
uint8_t _ucRxbuff;

//定义一个变量记录定时器7中断次数
unsigned int uiTime7Count = 0;
// 数据界面显示数据 0-PA4 1-PA5 2-PA1 3-PA7
double data[4] = {0,0,0,0};
int P1 = 0;
// 参数界面显示 X/0-频率参数  Y/1-电压参数
uint8_t para[2] = {0,0};
// 结构体0表示PA4  结构体1表示PA5 
struct paContent paText[2] = {{4,0,0,0,0},{5,0,0,0,0}};
// 记录结构体位置
int paLocation;
// 记录系统模式  使用按键可以切换
int systemMode;
// 记录按键B4
int keyB4[2] = {0,0};
// 记录显示模式
int LCDMod = 0;
uint8_t LED3TimeFlag = 0;

extern void EnableDebugIRQ(void);

/*************************************
* 函数功能：自定义的系统初始化函数
* 函数参数：无
* 函数返回值：无
*************************************/
void sysInit(void)
{
	int i=0;
	
	//LCD显示屏初始化
	lcdInit();
	//IIC初始化
	I2CInit();
	// 读取EEPROM中的值 并且将其合理化
	for(i=0;i<2;i++)
	{
		para[i] = readEepromByBit(0xa1-i);
			if(para[i] > 5)
				para[i] = 1;
			else if(para[i] < 1)
				para[i] = 1;
		HAL_Delay(10);
	}
	//打开串口的中断接收功能
	HAL_UART_Receive_IT(&huart1,(uint8_t *)&_ucRxbuff,sizeof(_ucRxbuff)); 
	//打开定时器17通道1的PWM输出功能
	HAL_TIM_PWM_Start(&htim17,TIM_CHANNEL_1);
	//打开定时器2通道2的输入捕获功能
    HAL_TIM_IC_Start_IT(&htim2,TIM_CHANNEL_2);
	//打开定时器7中断
	HAL_TIM_Base_Start_IT(&htim7);
	// 打开ADC的自动校准
	HAL_ADCEx_Calibration_Start(&hadc2,ADC_SINGLE_ENDED);	
	// 关闭所有LED
	changeAllLedByStateNumber(0);
}

/*************************************
* 函数功能：系统的逻辑函数
* 函数参数：无
* 函数返回值：无
*************************************/
void sysWork(void)
{	
	usartPro();
	keyPro();
	displayText(systemMode%3);
	ledPro((data[0]>data[1]*para[1]) && LED3TimeFlag,LCDMod);
}

/***************************************************
* 函数功能：按键的处理函数
* 函数参数：无
* 函数返回值：无
***************************************************/
void keyPro(void)
{
	int i=0;
	
	// 按键扫描
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
	// B4在数据界面按下
	if(key[3].flag && systemMode%3==0)
		elePro();
	// B4在参数界面按下 倍频/分频
	if(key[3].flag && systemMode%3==1)
		FrePro(++keyB4[0]%2);
	// B4在记录界面按下 短
	if(systemMode%3==2 && key[3].flag && key[3].longFlag==0)
		keyB4[1] = (keyB4[1]+1)%2;
	// B4在记录界面按下 长
	if(systemMode%3==2 && key[3].flag==0 && key[3].longFlag)
	{
		paText[keyB4[1]].a = 0;
		paText[keyB4[1]].t = 0;
		paText[keyB4[1]].h = 0;
		paText[keyB4[1]].n = 0;
		memset(paText[keyB4[1]].data,0,sizeof(paText[keyB4[1]].data));
	}
	// 消除按键的有效值
	for(i=0;i<4;i++)
	{
		key[i].flag = 0;
		key[i].longFlag = 0;
	}
}

/***************************************************
* 函数功能：设置LCD显示模式
* 函数参数：
*			int mod:显示模式 0-正向显示 1-翻转显示
* 函数返回值：无
***************************************************/
void setDisplayMod(int mod)
{
	// 反向显示
	if(mod == 1)
	{
		LCD_WriteReg( R1 ,0x0100 );
		LCD_WriteReg( R96,0xA700 );
		LCD_Clear(Black);
	}
	// 反向显示
	else
	{
		LCD_WriteReg( R1 ,0x0000 );
		LCD_WriteReg( R96,0x2700 );
		LCD_Clear(Black);
	}
}

/***************************************************
* 函数功能：LCD显示数据
* 函数参数：
*			int mod:显示数据模式 0-数据界面 1-翻转显示 2-记录界面
* 函数返回值：无
***************************************************/
void displayText(int mod)
{
	// 记录显示数据
	static char displayTemp[5][30];
	//0-数据界面
	if(mod == 0)
	{	
		sprintf(displayTemp[0],"         DATA        ");
		sprintf(displayTemp[1],"      PA4=%.2f        ",data[0]);
		sprintf(displayTemp[2],"      PA5=%.2f        ",data[1]);
		sprintf(displayTemp[3],"      PA1=%d          ",P1);
		sprintf(displayTemp[4],"                ");
	}		
	//1-参数显示 
	else if(mod == 1)
	{
		sprintf(displayTemp[0],"         PARA    ");
		sprintf(displayTemp[1],"      X=%d      ",para[0]);
		sprintf(displayTemp[2],"      Y=%d      ",para[1]);
		sprintf(displayTemp[3],"                   ");
		sprintf(displayTemp[4],"                   ");
	}
	//2-记录界面
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
* 函数功能：LED工作函数
* 函数参数：
*		   int mod1：LED3的工作模式
*		   int mod2：LED4的工作模式
* 函数返回值：无
***************************************************/
void ledPro(int mod1,int mod2)
{
	// 倍频
	if(keyB4[0]%2 == 0)
		changeLedStateByLocation(LED1,1);
	else
		changeLedStateByLocation(LED1,0);
	
	// 分频
	if(keyB4[0]%2 == 1)
		changeLedStateByLocation(LED2,1);
	else
		changeLedStateByLocation(LED2,0);
	
	// 电压
	if(mod1%2 == 1)
	{
		rollbackLedByLocation(LED3);
		LED3TimeFlag = 0;
	}
	else
		changeLedStateByLocation(LED3,0);
	
	// LCD正反
	if(mod2%2 == 0)
		changeLedStateByLocation(LED4,1);
	else
		changeLedStateByLocation(LED4,0);
}

/**************************************************
* 函数功能：串口数据处理函数
* 函数参数：无
* 函数返回值：无
***************************************************/
void usartPro(void)
{
	char usartTemp[10];
	
	// 验证是否收到串口信息以及串口信息长度
	// 参数
	if(ucRxbuff[0] == 'X')
		sprintf(usartTemp,"X:%d\r\n",para[0]);
	else if(ucRxbuff[0] == 'Y')
		sprintf(usartTemp,"Y:%d\r\n",para[1]);
	// 显示切换
	else if(ucRxbuff[0] == '#')
		setDisplayMod(++LCDMod%2);
	
	// 数据
	if(ucRxbuff[0] == 'P')
	{
		if(strcmp((char*)ucRxbuff,"PA1") == 0)
			sprintf(usartTemp,"PA1:%d\r\n",P1);
		else if(strcmp((char*)ucRxbuff,"PA4") == 0)
			sprintf(usartTemp,"PA4:%.2f\r\n",data[0]);
		else if(strcmp((char*)ucRxbuff,"PA5") == 0)
			sprintf(usartTemp,"PA5:%.2f\r\n",data[1]);	
	}
	
	// 是否发送数据
	if(usartTemp[0]=='P' || usartTemp[0]=='X' || usartTemp[0]=='Y')
		HAL_UART_Transmit(&huart1,(uint8_t*)usartTemp,sizeof(char)*strlen(usartTemp),10);
	
	memset(ucRxbuff,0,sizeof(ucRxbuff));
}

/**************************************************
* 函数功能：电压工作函数
* 函数参数：无
* 函数返回值：无
***************************************************/

void elePro(void)
{	
	int i=0;
	
	// 获取ADC数据
	getManyADC(&hadc2,data,2);
	// 将获取到的ADC数据储存到数组中
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
* 函数功能：频率工作函数
* 函数参数：
*			int mod: mod=0为倍频  mod=1为分频
* 函数返回值：无
***************************************************/
void FrePro(int mod)
{
	int frd = 0;

	// 倍频  keyB4[0]%2
	if(mod == 0)
		frd = (int)(P1/para[0]-1);
	// 分频
	else
		frd = (int)(P1*para[0]-1);
	//设置频率
	__HAL_TIM_SetAutoreload(&htim17,frd);
	HAL_TIM_GenerateEvent(&htim17, TIM_EVENTSOURCE_UPDATE);
}



#if 0
/***************************************************
* 函数功能：按键的处理函数
* 函数参数：无
* 函数返回值：无
***************************************************/
void keyPro(void)
{
	int i=0;
	
	// 按键扫描
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
	// B4在数据界面按下
	if(key[3].flag && systemMode%3==0)
		elePro();
	// B4在参数界面按下 倍频/分频
	if(key[3].flag && systemMode%3==1)
		FrePro(++keyB4[0]%2);
	// B4在记录界面按下 短
	if(systemMode%3==2 && key[3].flag && key[3].longFlag==0)
		keyB4[1] = (keyB4[1]+1)%2;
	// B4在记录界面按下 长
	if(systemMode%3==2 && key[3].flag==0 && key[3].longFlag)
	{
		paText[keyB4[1]].a = 0;
		paText[keyB4[1]].t = 0;
		paText[keyB4[1]].h = 0;
		paText[keyB4[1]].n = 0;
		memset(paText[keyB4[1]].data,0,sizeof(paText[keyB4[1]].data));
	}
	// 消除按键的有效值
	for(i=0;i<4;i++)
	{
		key[i].flag = 0;
		key[i].longFlag = 0;
	}
}

/***************************************************
* 函数功能：设置LCD显示模式
* 函数参数：
*			int mod:显示模式 0-正向显示 1-翻转显示
* 函数返回值：无
***************************************************/
void setDisplayMod(int mod)
{
	// 反向显示
	if(mod == 1)
	{
		LCD_WriteReg( R1 ,0x0100 );
		LCD_WriteReg( R96,0xA700 );
		LCD_Clear(Black);
	}
	// 反向显示
	else
	{
		LCD_WriteReg( R1 ,0x0000 );
		LCD_WriteReg( R96,0x2700 );
		LCD_Clear(Black);
	}
}

/***************************************************
* 函数功能：LCD显示数据
* 函数参数：
*			int mod:显示数据模式 0-数据界面 1-翻转显示 2-记录界面
* 函数返回值：无
***************************************************/
void displayText(int mod)
{
	// 记录显示数据
	static char displayTemp[5][30];
	//0-数据界面
	if(mod == 0)
	{	
		sprintf(displayTemp[0],"         DATA        ");
		sprintf(displayTemp[1],"      PA4=%.2f        ",data[0]);
		sprintf(displayTemp[2],"      PA5=%.2f        ",data[1]);
		sprintf(displayTemp[3],"      PA1=%d          ",(int)(1000000/P1));
		sprintf(displayTemp[4],"                ");
	}		
	//1-参数显示 
	else if(mod == 1)
	{
		sprintf(displayTemp[0],"         PARA    ");
		sprintf(displayTemp[1],"      X=%d      ",para[0]);
		sprintf(displayTemp[2],"      Y=%d      ",para[1]);
		sprintf(displayTemp[3],"                   ");
		sprintf(displayTemp[4],"                   ");
	}
	//2-记录界面
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
* 函数功能：LED工作函数
* 函数参数：
*		   int mod1：LED3的工作模式
*		   int mod2：LED4的工作模式
* 函数返回值：无
***************************************************/
void ledPro(int mod1,int mod2)
{
	changeAllLedByStateNumber(0);
	
	// 倍频
	if(keyB4[0]%2 == 0)
		changeLedStateByLocation(LED1,1);
	// 分频
	if(keyB4[0]%2 == 1)
		changeLedStateByLocation(LED2,1);
	// 电压
	if(mod1%2==1 && uiTime7Count%50)
		changeLedStateByLocation(LED3,uiTime7Count/50%2);
	// LCD正反
	if(mod2%2 == 0)
		changeLedStateByLocation(LED4,1);
}

/**************************************************
* 函数功能：串口工作函数
* 函数参数：无
* 函数返回值：无
***************************************************/
void usartPro(void)
{
	char usartTemp[10];
	
	// 验证是否收到串口信息以及串口信息长度
	// 参数
	if(ucRxbuff[0] == 'X')
		sprintf(usartTemp,"X:%d\r\n",para[0]);
	else if(ucRxbuff[0] == 'Y')
		sprintf(usartTemp,"Y:%d\r\n",para[1]);
	// 显示切换
	else if(ucRxbuff[0] == '#')
		setDisplayMod(++LCDMod%2);
	
	// 数据
	if(ucRxbuff[0] == 'P')
	{
		if(strcmp((char*)ucRxbuff,"PA1") == 0)
			sprintf(usartTemp,"PA1:%d\r\n",(int)(1000000/P1));
		else if(strcmp((char*)ucRxbuff,"PA4") == 0)
			sprintf(usartTemp,"PA4:%.2f\r\n",data[0]);
		else if(strcmp((char*)ucRxbuff,"PA5") == 0)
			sprintf(usartTemp,"PA5:%.2f\r\n",data[1]);	
	}
	
	// 是否发送数据
	if(usartTemp[0]=='P' || usartTemp[0]=='X' || usartTemp[0]=='Y')
		HAL_UART_Transmit(&huart1,(uint8_t*)usartTemp,sizeof(char)*strlen(usartTemp),10);
	
	memset(ucRxbuff,0,sizeof(ucRxbuff));
}

/**************************************************
* 函数功能：电压工作函数
* 函数参数：无
* 函数返回值：无
***************************************************/

void elePro(void)
{	
	int i=0;
	
	// 获取ADC数据
	getManyADC(&hadc2,data,2);
	// 将获取到的ADC数据储存到数组中
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
* 函数功能：频率工作函数
* 函数参数：无
* 函数返回值：无
***************************************************/
void FrePro(int mod)
{
	int frd = 0,cmpraeData = 0;

	// 倍频  keyB4[0]%2
	if(mod == 0)
	{
		frd = (int)(P1/para[0]-1);
		cmpraeData = (int)(0.5f*P1/para[0]);
	}
	// 分频
	else
	{
		frd = (int)(P1*para[0]-1);
		cmpraeData = (int)(0.5f*P1*para[0]);
	}
	//设置频率
	__HAL_TIM_SetAutoreload(&htim17,frd);
	//设置占空比
	__HAL_TIM_SetCompare(&htim17,TIM_CHANNEL_1,cmpraeData);
	HAL_TIM_GenerateEvent(&htim17, TIM_EVENTSOURCE_UPDATE);
}

#endif

