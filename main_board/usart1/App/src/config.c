#include "config.h"

/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    lcdDisplay
  * @brief   LCD显示数据
  * @param   char mod：显示模式 可以切换显示数据
  * @retval  None
  * @author  黑心萝卜三条杠
  * @Data    2023-03-24
 **/
/* -------------------------------- end -------------------------------- */
static void lcdDisplay()
{

}

/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    usartPro
  * @brief   串口逻辑函数
  * @param   None
  * @retval  None
  * @author  黑心萝卜三条杠
  * @Data    2023-03-24
 **/
/* -------------------------------- end -------------------------------- */
static void usartPro(void)
{
	
}

/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    sysInit
  * @brief   按键逻辑函数
  * @param   None
  * @retval  None
  * @author  黑心萝卜三条杠
  * @Data    2023-03-24
 **/
/* -------------------------------- end -------------------------------- */
static void keyPro(void)
{	
	// 按键扫描
	keyRefresh();
	switch(keyRising)
	{
		// 按键B1
		case 1:
			
			break;
		// 按键B2
		case 2:
			HAL_UART_Transmit_DMA(&huart1, (u8*)"234567\r\n", 10);
			break;
		// 按键B3
		case 4:
			HAL_UART_Receive_DMA(&huart1,ucRxbuff,10);
			rollbackLedByLocation(LED1);
			break;
		// 按键B4
		case 8:
			break;
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
  * @Data    2023-03-24
 **/
/* -------------------------------- end -------------------------------- */
static void ledWork(void)
{
	
}
	
/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    sysInit
  * @brief   自定义的系统初始化函数
  * @param   None
  * @retval  None
  * @author  黑心萝卜三条杠
  * @Data    2023-03-24
 **/
/* -------------------------------- end -------------------------------- */
void sysInit(void)
{
	// LCD初始化
	lcdInit();

	// 关闭所有LED
	changeAllLedByStateNumber(0);
	
	HAL_UART_Receive_DMA(&huart1,_ucRxbuff,1);
//	
//	// 使能串口中断
//	HAL_UART_Receive_IT(&huart1,(uint8_t *)&_ucRxbuff,sizeof(_ucRxbuff)); 
}

/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    sysWork
  * @brief   自定义的系统工作逻辑函数
  * @param   None
  * @retval  None
  * @author  黑心萝卜三条杠
  * @Data    2023-03-24
 **/
/* -------------------------------- end -------------------------------- */
void sysWork(void)
{	
	static uint16_t count = 0;
	char temp[20];
	// 按键处理函数
	keyPro();
	
	// 串口逻辑函数
	usartPro();
	
	// LED显示函数
	if(HAL_GetTick() % 15 == 0)
		ledWork();
	
	// LCD显示
	if(HAL_GetTick() % 10 == 0)
		lcdDisplay();
	
	sprintf(temp,"count:%d",count++);
	memcpy((char*)temp,(char*)temp,sizeof(temp) );
	LCD_DisplayStringLine(Line1,(u8*)temp);
	if(strlen((char*)ucRxbuff) != 0 )
	{
		LCD_ClearLine(Line2);
		LCD_DisplayStringLine(Line2,(u8*)ucRxbuff);
		memset(ucRxbuff,0,10);
		lenBuff = 0;
	}
	rollbackLedByLocation(LED8);
	HAL_Delay(1000);
}




