#include "config.h"

// LCD显示模式
char LCDmod = 0;
// R37的值
float r37 = 0;
// PA7的频率 占空比
uint16_t Pa7Frd = 1000,Pa7Duty = 10;
// 以前的PA7的频率 占空比
uint16_t oldPa7Frd = 1000,oldPa7Duty = 10;
// 电压参数
float VTemp = 0.3f;

// 测试变量
uint16_t tempDuty = 10;


/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    lcdDisplay
  * @brief   LCD显示数据
  * @param   char mod：显示模式 可以切换显示数据
  * @retval  None
  * @author  黑心萝卜三条杠
  * @Data    2023-04-02
 **/
/* -------------------------------- end -------------------------------- */
static void lcdDisplay()
{
	char temp[20];
	// 数据显示界面
	if(LCDmod == 0)
	{
		LCD_DisplayStringLine(Line1,(u8*) "       DATA    ");
		sprintf(temp,"   VR37:%4.2fV    ",r37);
		LCD_DisplayStringLine(Line3,(u8*)temp);
		sprintf(temp,"   PA7:%dHz     ",Pa7Frd);
		LCD_DisplayStringLine(Line5,(u8*)temp);
	}
	// 参数显示界面
	else if(LCDmod == 1)
	{
		LCD_ClearLine(Line1);
		LCD_DisplayStringLine(Line3,(u8*)"       PARA     ");
		sprintf(temp,"     VP1:%3.1fV   ",VTemp);
		LCD_DisplayStringLine(Line5,(u8*)temp);
	}
	
	// 显示数据测试
#if 0
	sprintf(temp," %d  %d  ",(int)((Pa7Duty*1.0/100.0)*(1000000/Pa7Frd)+0.5),(int)((tempDuty*1.0/100.0)*(1000000/Pa7Frd)+0.5));
	LCD_DisplayStringLine(Line6,(u8*)temp);
#endif	
}

/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    sysInit
  * @brief   按键逻辑函数
  * @param   None
  * @retval  None
  * @author  黑心萝卜三条杠
  * @Data    2023-04-02
 **/
/* -------------------------------- end -------------------------------- */
static void keyPro(void)
{
	// 按键扫描
	keyRefresh();
	switch(keyRising)
	{
		// B1  切换显示界面
		case 1:
			LCDmod ^= 1;
			break;
		// B2 参数界面下增加电压参数
		case 2:
			if(LCDmod)
			{
				VTemp += 0.3f;
				if(VTemp > 3.3f) VTemp = 0.0f;
			}
			break;
		// B3 数据界面下调节频率
		case 4:
			if(!LCDmod)
			{
				Pa7Frd += 1000;
				if(Pa7Frd > 10000) Pa7Frd = 1000;
			}
			break;
// 按键控制占空比测试
#if 0
		case 8:
			tempDuty += 10;
			if(tempDuty == 100) tempDuty = 10;
			break;
#endif
		// 其他
		default:
			break;
	}
}

/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    sysInit
  * @brief   LED逻辑函数
  * @param   None
  * @retval  None
  * @author  黑心萝卜三条杠
  * @Data    2023-04-02
 **/
/* -------------------------------- end -------------------------------- */
static void ledWork(void)
{
	// 外部声明的计数值
	extern uint16_t count;
	// 数据界面
	if(LCDmod == 0 && HAL_GetTick()%90==0)
	{
		changeLedStateByLocation(LED1,1);
		changeLedStateByLocation(LED2,0);
	}
	// 参数界面
	else if(LCDmod == 1 && HAL_GetTick()%90==0)
	{
		changeLedStateByLocation(LED1,0);
		changeLedStateByLocation(LED2,1);
	}
	// r37 > vTemp
	if(r37 > VTemp)
	{	
		if(count >= 100)
		{
			rollbackLedByLocation(LED3);
			count = 0;
		}
	}
	else 
		changeLedStateByLocation(LED3,0);
}

/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    usartPro
  * @brief   串口处理逻辑函数
  * @param   None
  * @retval  None
  * @author  黑心萝卜三条杠
  * @Data    2023-04-02
 **/
/* -------------------------------- end -------------------------------- */
static void usartPro(void)
{
#if 0
	/************* 串口接收一位数据*********/
	// 判断是否接收到串口数据
	if(_ucRxbuff[0] == '0' || strlen((char*)_ucRxbuff) == 0) return ;
	// 判断是否接收到串口数据
	if('0'<_ucRxbuff[0] && _ucRxbuff[0]<='9')
		Pa7Duty = (_ucRxbuff[0] - '0')*10;
	// 其他情况发送错误信息
	else
		HAL_UART_Transmit_IT(&huart1,(u8*)"error\r\n",sizeof((char*)"error"));
	// 清空有效值
	_ucRxbuff[0] = '0';
#else
	/************* 串口接收多位数据*********/
	// 判断是否接收到串口数据
	if(strlen((char*)ucRxbuff) == 0) return ;
	// 判断是否接收到串口数据长度是否合理 串口数据格式是否正确
	else if(strlen((char*)ucRxbuff) == 1 && '0'<ucRxbuff[0] && ucRxbuff[0]<='9')
		Pa7Duty = (ucRxbuff[0] - '0')*10;
	else
		HAL_UART_Transmit_IT(&huart1,(u8*)"error\r\n",sizeof((char*)"error\r\n"));
	memset(ucRxbuff,0,sizeof((char*)ucRxbuff));
	lenBuff = 0;
#endif
}

/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    changePWMData
  * @brief   定时器占空比与频率处理逻辑函数
  * @param   None
  * @retval  None
  * @author  黑心萝卜三条杠
  * @Data    2023-04-02
 **/
/* -------------------------------- end -------------------------------- */
static void changePWMData(void)
{
	// 频率发生了更新 应该更新定时器频率
	if(Pa7Frd != oldPa7Frd)
	{
		__HAL_TIM_SetAutoreload(&htim3,1000000/Pa7Frd-1);
		HAL_TIM_GenerateEvent(&htim3,TIM_EVENTSOURCE_UPDATE);
		oldPa7Frd = Pa7Frd;
	}
	// 占空比发生了更新 应该更新定时器占空比
	if(Pa7Duty != oldPa7Duty || Pa7Frd != oldPa7Frd)
	{
		__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_2,(int)((Pa7Duty*1.0/100.0)*(1000000/Pa7Frd)+0.5));
		oldPa7Duty = Pa7Duty;
	}
}

/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    sysInit
  * @brief   自定义的系统初始化函数
  * @param   None
  * @retval  None
  * @author  黑心萝卜三条杠
  * @Data    2023-04-02
 **/
/* -------------------------------- end -------------------------------- */
void sysInit(void)
{
	// LCD初始化
	lcdInit();
	// 使能串口中断
	HAL_UART_Receive_IT(&huart1,(uint8_t *)&_ucRxbuff,sizeof(_ucRxbuff)); 
	// 开启定时器的PWM
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_2);
	// 关闭所有LED
	changeAllLedByStateNumber(0);	
}

/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    sysWork
  * @brief   自定义的系统工作逻辑函数
  * @param   None
  * @retval  None
  * @author  黑心萝卜三条杠
  * @Data    2023-04-02
 **/
/* -------------------------------- end -------------------------------- */
void sysWork(void)
{	
	// 按键处理函数
	keyPro();
	// 串口数据处理函数
	usartPro();
	// 间隔获取r37的值
	if(HAL_GetTick() % 20 == 0 ) r37 = getADC(&hadc2);
	// 定时器的频率与占空比更新逻辑函数
	changePWMData();
	// LED显示函数
	ledWork();
	// LCD显示
	if(HAL_GetTick() % 105 == 0 )
		lcdDisplay();
}




