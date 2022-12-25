#include "config.h"

/************************该.c文件用于写系统初始化以及系统初始化后的主逻辑***********************************/

//* 存储LCD显示数据的变量
unsigned char lcdDisplayData[20];

//* 记录syswork函数执行的次数
u8 sysWorkCount = 0;

//* 记录上一次串口接收到的数据
uint8_t oldRxBuff[USARTMAXLENTH];

/**********************************************
* 函数功能：系统初始化函数
* 函数参数：无
* 函数返回值：无
**********************************************/
void sysInit(void)
{
	//LED的初始化
	ledInit();

	//LCD初始化
	//lcdInit();

	//按键初始化
	keyInit();

	//串口初始化
	MX_USART1_UART_Init();
}

/**********************************************
* 函数功能：系统主逻辑函数(由于主函数中含while(1) 此处不在含有while(1))
* 函数参数：无
* 函数返回值：无
***********************************************/
void sysWork(void)
{
#if 0
	//注意每次LCD写入数据后 操作LED时需要重置所有LED的转态 不然所有LED都会点亮
	changeAllLedByStateNumber(0);
	
	//开启部分LED
	turnOnLedByLocation(GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_12|GPIO_PIN_11,0); 
	//毫秒级延时 下面这部分延时是延时1s
	HAL_Delay(1000);
	turnOnLedByLocation(GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_12|GPIO_PIN_11,1); 
	HAL_Delay(1000);

	//LCD显示"hello word!"
	LCD_DisplayStringLine(Line1,"hello word!");
	//LCD记录sysWorkCount的数值
	sprintf((char*)lcdDisplayData,"WorkCount is %d",(int)sysWorkCount);
	LCD_DisplayStringLine(Line2,lcdDisplayData);
	
#endif
	sysWorkCount++;
#if 1
	/*******按键测试程序*************/
	//按键B1
	if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0) == RESET){
		rollbackLedByLocation(GPIO_PIN_8);
	}
	//按键B2
	if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1) == RESET){
		rollbackLedByLocation(GPIO_PIN_9);
	}
	//按键B3
	if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2) == RESET){
		rollbackLedByLocation(GPIO_PIN_10);
	}
	//按键B4
	if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0) == RESET){
		rollbackLedByLocation(GPIO_PIN_11);
	}
#endif

#if 0
	//按键与LED的联合测试
	if(scanKey() ==1){
		rollbackLedByLocation(GPIO_PIN_8|GPIO_PIN_10|GPIO_PIN_12);
	}
#endif

#if 0
	/********串口1的测试程序***************/
	//串口1的发送测试
	rollbackLedByLocation(GPIO_PIN_8);
	sprintf((char*)Txbuff,"count:%d",(int)sysWorkCount);
	HAL_UART_Transmit(&huart1,(u8*)Txbuff,strlen((char*)Txbuff),50);
	printf("%s\r\n",Txbuff);

	//串口1的中断接收测试  跟上一次串口接收到的数据比较 如果不同就发送到PC
	/*使用说明：
		1.该串口接收是不定长的 
		2.Rxbuff存储的是历史接收到的数据
		3.如果不想保存历史数据 可以到中断接收函数中修改每次接收数据后保存的起始位置
		4.如果想要将串口接收到的数据改成定长的 可以修改串口初始化与串口中断中HAL_UART_Receive_IT函数
		  的第三个参数
	*/
	if(strcmp((char*)oldRxBuff,(char*)Rxbuff) != 0){
		HAL_UART_Transmit(&huart1,(uint8_t*)Rxbuff,strlen((char*)Rxbuff),50);
		sprintf((char*)oldRxBuff,"%s",(char*)Rxbuff);
		rollbackLedByLocation(GPIO_PIN_9);
	}
#endif

	// sprintf((char*)Txbuff,"count:%d",uTick_ms);
	// HAL_UART_Transmit(&huart1,(u8*)Txbuff,strlen((char*)Txbuff),50);
	//HAL_Delay(1000);
}



