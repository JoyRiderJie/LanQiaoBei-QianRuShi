#include "config.h"


// 保存RTC的日期时间
RTC_TimeTypeDef myTime;
RTC_DateTypeDef myDate;

extern RTC_HandleTypeDef hrtc;

// 是否发生RTC中断
uint16_t RTCFlag = 0;
u8 sEnterCnt = 0;
//设置闹钟,记得写中断回调函数
static void setalarm(int hours,int minutes,int seconds)
{
	RTC_AlarmTypeDef sAlarm ;
	
	/********设置下一次响应时间*******/  
//	sAlarm.Alarm = alarmName;
	sAlarm.AlarmTime.Hours = hours;
	sAlarm.AlarmTime.Minutes = minutes;
	sAlarm.AlarmTime.Seconds = seconds;	
	sAlarm.AlarmTime.SubSeconds = 0x0;
	sAlarm.AlarmMask = RTC_ALARMMASK_NONE;
	sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
	sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_WEEKDAY;
	sAlarm.AlarmDateWeekDay = RTC_WEEKDAY_TUESDAY;
	sAlarm.Alarm = RTC_ALARM_A;
	
	//重置中断
	HAL_RTC_SetAlarm_IT(&hrtc,&sAlarm, RTC_FORMAT_BIN);
//    __HAL_RTC_ALARM_EXTI_CLEAR_IT();
	rollbackLedByLocation(LED2);
}

/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    sysInit
  * @brief   自定义系统外设初始化函数
  * @param   None
  * @retval
  * @author  黑心萝卜三条杠
  * @Data    2023-03-19
  * 1. ...
  * <modify staff>:
  * <data>        :
  * <description> :
  * 2. ...
 **/
/* -------------------------------- end -------------------------------- */
void sysInit(void)
{
	// LCD初始化函数
	lcdInit();
	
	// 关闭所有的LED
	changeAllLedByStateNumber(0);
//	
//	__HAL_RTC_ALARMA_ENABLE(&hrtc);
	
}

/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    sysWork
  * @brief   自定义系统外设工作函数
  * @param   None
  * @retval
  * @author  黑心萝卜三条杠
  * @Data    2023-03-19
  * 1. ...
  * <modify staff>:
  * <data>        :
  * <description> :
  * 2. ...
 **/
/* -------------------------------- end -------------------------------- */
void sysWork(void)
{
	char temp[50];
	
	HAL_RTC_GetTime(&hrtc,&myTime,RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc,&myDate,RTC_FORMAT_BIN);
	sprintf((char *)temp, "date 20%02d-%02d-%02d-%d",myDate.Year,myDate.Month,myDate.Date,(myDate.WeekDay));
	LCD_DisplayStringLine(Line2,(uint8_t*)temp);
	sprintf((char *)temp, "time %02d-%02d-%02d",myTime.Hours,myTime.Minutes,myTime.Seconds);
	LCD_DisplayStringLine(Line4,(uint8_t*)temp);

	if(RTCFlag == 1)
	{
		RTCFlag = 0;
		sprintf((char *)temp, "count:%d",++sEnterCnt);
		LCD_DisplayStringLine(Line6,(uint8_t*)temp);
//		setalarm(myTime.Hours,myTime.Minutes,myTime.Seconds+3); // 20,20,30); //
		setalarm(0,0,5); // 20,20,30); //
	}
	
	if(myTime.Seconds % 7 == 100)
	{
		myDate.Year = 22;
		myDate.Month = 12;
		myDate.Date = 25;
		myDate.WeekDay = 1;
		HAL_RTC_SetDate(&hrtc,&myDate,RTC_FORMAT_BIN);
	}
	if(sEnterCnt == 3)
	{
		myTime.Hours = 1;
		myTime.Minutes = 4;
		myTime.Seconds = 0;
		HAL_RTC_SetTime(&hrtc,&myTime,RTC_FORMAT_BIN);
	}
	
	
	rollbackLedByLocation(LED1);
	HAL_Delay(1000);
}


/**
  * @brief  Alarm callback 闹钟中断回调函数
  * @param  hrtc : RTC handle
  * @retval None
  */
void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc1) 
{
	RTCFlag = 1;
//	__HAL_RTC_ALARM_CLEAR_FLAG(&hrtc,RTC_FLAG_ALRAF);
	
}
