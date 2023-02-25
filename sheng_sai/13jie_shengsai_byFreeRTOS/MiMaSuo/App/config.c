#include "config.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"

//外部定义变量
extern TIM_HandleTypeDef htim2;
extern UART_HandleTypeDef huart1;

//记录目前LCD处于的界面 0-密码输入界面  1-显示输出界面
int lcd_view_mod = 0;
//密码输入界面显示的数据
char psd_txt[20];
//输出状态界面显示数据
char sta_txt[20];
//用于循环
int i;
//* 存储串口1接收的数据
uint8_t Rxbuff[7];
//密码锁密码 注意 这里一定一定要加上字符串结束符'\0' 否则会出现字符串拼接的情况
char passwd[4] = {'1','2','3','\0'};
//输入的密码
char passwd_test[4] = {'0','0','0','\0'};
//记录密码输错次数
unsigned int passwd_wrong_count = 0;
//记录是否重头开始输入密码 0-是重头开始输入且没开始输入的  1-不是重头开始输入密码
char passwd_flag = 0;

/***********************************************
* 函数功能：自定义的系统初始化
* 函数参数：无
* 函数返回值：无
***********************************************/
void sysInit(void)
{
	//LCD初始化
	lcdInit();
	//关闭所有的LED
	changeAllLedByStateNumber(0);
	//打开串口的中断接收功能
	HAL_UART_Receive_IT(&huart1,(uint8_t *)&Rxbuff,sizeof(Rxbuff)); 
	//打开定时器2通道2的PWM输出功能
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);
}

/***********************************************
* 函数功能：显示密码输入界面
* 函数参数：无
* 函数返回值：无
***********************************************/
void PSDViewDisplay(void)
{
	LCD_DisplayStringLine(Line2,(uint8_t*)"       PSD");
	//在第一次输入密码前显示
	if(passwd_flag == 0)
	{
		sprintf(psd_txt,"    B1 :%c       ",'@');
		LCD_DisplayStringLine(Line3,(uint8_t*)psd_txt);
		sprintf(psd_txt,"    B2 :%c       ",'@');
		LCD_DisplayStringLine(Line4,(uint8_t*)psd_txt);
		sprintf(psd_txt,"    B3 :%c       ",'@');
		LCD_DisplayStringLine(Line5,(uint8_t*)psd_txt);
	}
	else
	{
		sprintf(psd_txt,"    B1 :%c       ",passwd_test[0]);
		LCD_DisplayStringLine(Line3,(uint8_t*)psd_txt);
		sprintf(psd_txt,"    B2 :%c       ",passwd_test[1]);
		LCD_DisplayStringLine(Line4,(uint8_t*)psd_txt);
		sprintf(psd_txt,"    B3 :%c       ",passwd_test[2]);
		LCD_DisplayStringLine(Line5,(uint8_t*)psd_txt);
	}
}

/***********************************************
* 函数功能：显示输出界面
* 函数参数：无
* 函数返回值：无
***********************************************/
void STAViewDisplay(void)
{
	LCD_DisplayStringLine(Line2,(uint8_t*)"       STA");
	LCD_DisplayStringLine(Line3,(uint8_t*)"    F :2000Hz");
	LCD_DisplayStringLine(Line4,(uint8_t*)"    D :10%");
	LCD_ClearLine(Line5);
}

/********************************************
* 函数功能：分析串口接收数据并且合理设置密码
* 函数参数:无
* 函数返回值:无
********************************************/
void getPasswdByUsart(void)
{
	//分析串口接收到数据  需：旧密码匹配才能够设置新密码
	for(i=0;i<7;i++)
	{
		//判断输入是否合理
		if(!((Rxbuff[i]>='0'&&Rxbuff[i]<='9') || Rxbuff[i]=='-'))
			return ;
		//判断旧密码是否正确
		if(i < 3)
		{
			if(Rxbuff[i]!=passwd[i])
				return ;
		}
		//设置新密码
		if(i>3)
		{
			passwd[i%4] = Rxbuff[i];
		}
	}
	//重置密码后也需要重置旧密码输入错误的次数
	passwd_wrong_count = 0;
}

/********************************************
* 函数功能：
* 函数参数:无
* 函数返回值:无
********************************************/
void keyProc(void)
{
	if(lcd_view_mod ) return ;
	
	scanKeyUseStructAndTime();
	//遍历4个按键按键
	for(i=0;i<4;i++)
	{
		//按键按下
		if(key[i].flag == 1)
		{
			//按键B0-B2
			if(i!= 3)
			{
				if(++passwd_test[i]>'9') passwd_test[i] = '0';
				passwd_flag = 1;
			}
			//按键B3
			else
			{
				//输入密码正确
				if(!strcmp(passwd,passwd_test)) 
				{
					lcd_view_mod = 1;
					passwd_wrong_count = 0;
				}
				//输入密码错误
				else
				{
					passwd_flag = 0;
					passwd_wrong_count++;
				}
				memset(passwd_test,'0',sizeof(char)*3);
			}
			key[i].flag = 0;
		}
	}
}

/****************************************
* 函数功能：修改PWM的频率工作
* 函数参数：
*			unsigned int autoreloadDate:重装载值
*			unsigned int compareDate:PWM的比较值
* 函数返回值：无
****************************************/
void pwmWorkByFre(unsigned int autoreloadDate,unsigned int compareDate)
{
	//设置重装载值
	__HAL_TIM_SetAutoreload(&htim2,autoreloadDate-1);
	__HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_2,compareDate);
}

/***使用HAL_UART_Receive_IT中断接收数据 每次接收完成数据后就会执行该函数***/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART1){
		// 重新使能中断
		HAL_UART_Receive_IT(huart,(uint8_t *)&Rxbuff,sizeof(Rxbuff)); 
	}
}

