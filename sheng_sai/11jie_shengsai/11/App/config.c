#include "config.h"

//定义存储显示数据的结构体
struct displayData*showData;

//用于计时
unsigned int count = 0;

/***********************************************
* 函数功能：自定义的系统初始化函数
* 函数参数：无
* 函数返回值：无
***********************************************/
void sysInit(void)
{
	//LCD初始化
	lcdInit();
	//关闭LED
	changeAllLedByStateNumber(OFF);
	
	//打开定时器产生PWM的通道
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim17,TIM_CHANNEL_1);
	
	//初始化结构体
	showData = (struct displayData*)malloc(sizeof(struct displayData));
	showData->frdP6 = 10;
	showData->frdP7 = 10;
	showData->mode = 0;
	showData->vValue = 0;
	showData->page = 0;
}

/***********************************************
* 函数功能：自定义的系统工作函数
* 函数参数：无
* 函数返回值：无
***********************************************/
void sysWork(void)
{
	unsigned char keyNumber = scanKey();
	switch(keyNumber)
	{
		//切换显示界面
		case 1:
			showData->page++;
			break;
		//手动模式  参数界面 修改PA6的参数
		case 2:
			if(showData->mode%2==1 && showData->page%2==1)
				showData->frdP6 += 10;
			if(showData->frdP6 == 100)
				showData->frdP6 = 10;
			break;
		//手动模式  参数界面 修改PA7的参数
		case 3:
			if(showData->mode%2==1 && showData->page%2==1)
				showData->frdP7 += 10;
			if(showData->frdP7 == 100)
				showData->frdP7 = 10;
			break;
		//切换模式
		case 4:
			showData->mode++;
			break;
		default:break;
	}
	
	//获取ADC的值 降低获取ADC值的频率
	if(count++ %5 == 0)
		showData->vValue = getADC(&hadc2);
	//切换PWM的频率
	changePwmFrd();
	//LCD显示
	display();
	//LED显示
	ledWork();
}

/***********************************************
* 函数功能：更改PWM占空比函数
* 函数参数：无
* 函数返回值：无
***********************************************/
void changePwmFrd(void)
{
	//手动模式
	if(showData->mode%2 == 1)
	{
		__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_1,100*showData->frdP6);
		__HAL_TIM_SetCompare(&htim17,TIM_CHANNEL_1,50*showData->frdP7);  
	}
	//自动模式 电压值为0V
	else if(showData->mode%2 == 0 && showData->vValue == 0)
	{
		__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_1,10000);
		__HAL_TIM_SetCompare(&htim17,TIM_CHANNEL_1,5000);
	}
	//自动模式  电压值为3.3V 
	else if(showData->mode%2 == 0 && showData->vValue == 3.3)
	{
		__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_1,0);
		__HAL_TIM_SetCompare(&htim17,TIM_CHANNEL_1,0);
	}
}

/***********************************************
* 函数功能：LED工作函数
* 函数参数：无
* 函数返回值：无
***********************************************/
void ledWork(void)
{
	//关闭所有的LED灯
	changeAllLedByStateNumber(OFF);
	
	//自动模式
	if(showData->mode%2 == 0)
		changeLedStateByLocation(LED1,ON);
	
	//数据界面
	if( showData->page%2 == 0 )
		changeLedStateByLocation(LED2,ON);
}

/***********************************************
* 函数功能：显示函数
* 函数参数：无
* 函数返回值：无
***********************************************/
void display(void)
{
	char temp[10];
	//数据显示界面
	if( showData->page%2 == 0 )
	{
		LCD_DisplayStringLine(Line0,(uint8_t*)"       Data");
		sprintf(temp,"     V:%.2fV",showData->vValue);
		LCD_DisplayStringLine(Line2,(uint8_t*)temp);
		//自动模式
		if(showData->mode%2 == 0)
			LCD_DisplayStringLine(Line4,(uint8_t*)"     Mode:AUTO     ");
		//手动模式
		else
			LCD_DisplayStringLine(Line4,(uint8_t*)"     Mode:MANU     ");		
	}
	//显示参数界面
	else if( showData->page%2 == 1)
	{
		LCD_DisplayStringLine(Line0,(uint8_t*)"       Pata");
		sprintf(temp,"    PA6:%d%%        ",showData->frdP6);
		LCD_DisplayStringLine(Line2,(uint8_t*)temp);
		sprintf(temp,"    PA7:%d%%        ",showData->frdP7);
		LCD_DisplayStringLine(Line4,(uint8_t*)temp);
	}
}



