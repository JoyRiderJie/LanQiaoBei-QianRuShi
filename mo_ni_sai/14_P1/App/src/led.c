#include "led.h"

/****************************************************
* 函数功能：LED的初始化
* 函数参数：无
* 函数返回值：无
*****************************************************/
void ledInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
/********时钟初始化*************************/
	//GPIOC时钟初始化
	__HAL_RCC_GPIOC_CLK_ENABLE();
	//GPIOD时钟初始化
	__HAL_RCC_GPIOD_CLK_ENABLE();
	
/********GPIO初始化************************/
	/************GPIOC组************/
	HAL_GPIO_WritePin(GPIOC, LED_GPIO_Pin, GPIO_PIN_SET);
	//引脚初选择
	GPIO_InitStructure.Pin = LED_GPIO_Pin;
	//GPIO设置成推挽输出
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	//GPIO的时钟频率设置成 5 MHz 
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
	//因为引脚设置成推挽输出 因此此处不需要上拉输入也不需要下拉输入
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	//GPIO初始化
	HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);

	/*************GPIOD组初始化**********/
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
	//引脚选择
	GPIO_InitStructure.Pin = GPIO_PIN_2;
	//GPIO初始化
	HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	changeAllLedByStateNumber(0);
}

/*****************************************************
* 函数功能：改变所有LED的状态
* 函数参数：
*			char LEDSTATE: 0-表示关闭 1-表示打开
* 函数返回值：无
******************************************************/
void changeAllLedByStateNumber(char LEDSTATE)
{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15|GPIO_PIN_8
					|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12,(LEDSTATE==1?GPIO_PIN_RESET:GPIO_PIN_SET));
	//打开锁存器    准备写入数据
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
	//关闭锁存器 锁存器的作用为 使得锁存器输出端的电平一直维持在一个固定的状态
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
}

/*****************************************************
* 函数功能：根据LED的位置打开或者是关闭LED
* 函数参数：
*			uint16_t LEDLOCATION:需要操作LED的位置
*			char LEDSTATE: 0-表示关闭 1-表示打开
* 函数返回值：无
******************************************************/
void changeLedStateByLocation(enum LEDLOCATION LEDLOCATION,char LEDSTATE)
{
	HAL_GPIO_WritePin(GPIOC,LEDLOCATION,(LEDSTATE==1?GPIO_PIN_RESET:GPIO_PIN_SET));
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
	
}

/*****************************************************
* 函数功能：根据LED的位置反转LED的状态
* 函数参数：
*			uint16_t LEDLOCATION:需要操作LED的位置
* 函数返回值：无
******************************************************/
void rollbackLedByLocation(enum LEDLOCATION LEDLOCATION)
{
	HAL_GPIO_TogglePin(GPIOC,LEDLOCATION);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
}

