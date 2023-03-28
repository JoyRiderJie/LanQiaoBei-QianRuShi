#include "config.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"


//外部定义变量
extern TIM_HandleTypeDef htim7,htim2;
extern UART_HandleTypeDef huart1;
extern TIM_HandleTypeDef htim3;

//记录目前LCD处于的界面 0-密码输入界面  1-显示输出界面
int lcd_view_mod = 0;
//定时器7计数  计数值加1表示定时器触发一次也就是过了10ms
int time7_count = 0;
//保存上一次定时器7计数值
int time7_count_start_flag = 0;
//密码输入界面显示的数据
char psd_txt[20];
//输出状态界面显示数据
char sta_txt[20];

char temp[20];
unsigned int crrl_t = 0;
uint32_t frd = 0;

//用于循环
int i,j;

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
	//打开定时器7中断
	HAL_TIM_Base_Start_IT(&htim7);
	//打开串口的中断接收功能
	HAL_UART_Receive_IT(&huart1,(uint8_t *)&Rxbuff,sizeof(Rxbuff)); 
	//打开定时器2通道2的PWM输出功能
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);
}


/***********************************************
* 函数功能：系统工作逻辑函数
* 函数参数：无
* 函数返回值：无
***********************************************/
void sysWork(void)
{
	//更新密码
	getPasswdByUsart();
	
	//密码的相关操作以及显示密码界面
	if(lcd_view_mod == 0)
	{
		keyProc();
		PSDViewDisplay();
		pwmWorkByFre(1000,400);
	}
	//显示输出界面
	else if(lcd_view_mod == 1)
	{
		STAViewDisplay();
		pwmWorkByFre(500,50);
		passwd_flag = 0;
		time7_count_start_flag = 1;
	}
	//LED灯显示
	ledDisplay();
	
	//5秒时间到 定时器值以及标志位归零 界面显示密码界面 密码输错次数归零
	if(time7_count>500)
	{
		time7_count = 0;
		time7_count_start_flag = 0;
		lcd_view_mod = 0;
		passwd_wrong_count = 0;
	}
	
	// 显示测试数据
//	sprintf(temp,"c:%d f:%6d",crrl_t,frd);
//	LCD_DisplayStringLine(Line0,(uint8_t*)temp);
//	
//	
//	sprintf(temp,"passwd:%s",Rxbuff);
//	LCD_DisplayStringLine(Line6,(uint8_t*)temp);
//	
//	sprintf(temp,"flag:%d count:%d",time7_count_start_flag,time7_count);
//	LCD_DisplayStringLine(Line7,(uint8_t*)temp);
//	
//	sprintf(temp,"data:%s len:%d",passwd,strlen(passwd));
//	LCD_DisplayStringLine(Line8,(uint8_t*)temp);
}

/***非阻塞模式下定时器中断回调函数***/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	//定时器7的中断回调函数
	if(htim->Instance == TIM7)
	{
		scanKeyUseStructAndTime();
		if(time7_count_start_flag)
			time7_count++;
	}
}

/***使用HAL_UART_Receive_IT中断接收数据 每次接收完成数据后就会执行该函数***/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART1){
		// 重新使能中断
		HAL_UART_Receive_IT(huart,(uint8_t *)&Rxbuff,sizeof(Rxbuff)); 
	}
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{

	if(htim->Instance == TIM3)
	{
		//读取定时器的计数值
		crrl_t = HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_1);
		//将定时器的计数值设置成0
		__HAL_TIM_SetCounter(htim,0);
		//计算频率
		frd = (80000000/80)/crrl_t;
		//重新开启定时器
		HAL_TIM_IC_Start_IT(htim,TIM_CHANNEL_1);
	}
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

/***********************************************
* 函数功能：LED的相关显示
* 函数参数：无
* 函数返回值：无
***********************************************/
void ledDisplay(void)
{
	static int LED2_state = 1;
	//为了避免LED显示受到LCD影响 应该先关闭所有LED
	changeAllLedByStateNumber(0);
	
	//密码输入错误三次  LED2以0.1秒间隔闪烁 （5秒后熄灭）
	if(passwd_wrong_count>=3)
	{
		time7_count_start_flag = 1;
		if(time7_count <= 500)
		{
			//定时器计数值刷新  时间到了 LED2状态需要反转
			changeLedStateByLocation(LED2,LED2_state);
			LED2_state ^= 1;
		}
	}
	
	//密码验证成功 LED1点亮5秒 （5秒后熄灭）
	if(lcd_view_mod && time7_count <= 500)
	{
		changeLedStateByLocation(LED1,1);
	}
}

/********************************************
* 函数功能：
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
	HAL_TIM_GenerateEvent(&htim2, TIM_EVENTSOURCE_UPDATE);
}




