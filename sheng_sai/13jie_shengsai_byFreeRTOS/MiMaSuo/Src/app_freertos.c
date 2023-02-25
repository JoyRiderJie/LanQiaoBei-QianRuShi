/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : app_freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "config.h"
#include "string.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
int myTime[2] = {0,0};

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
char temp[20];
int j=0;
unsigned int count[3] = {0,0,0};
int timeFlag = 0;
int stopTimeFlag = 0;
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};
/* Definitions for keyTask */
osThreadId_t keyTaskHandle;
const osThreadAttr_t keyTask_attributes = {
  .name = "keyTask",
  .priority = (osPriority_t) osPriorityLow,
  .stack_size = 128 * 4
};
/* Definitions for ledTask */
osThreadId_t ledTaskHandle;
const osThreadAttr_t ledTask_attributes = {
  .name = "ledTask",
  .priority = (osPriority_t) osPriorityLow,
  .stack_size = 128 * 4
};
/* Definitions for usartTask */
osThreadId_t usartTaskHandle;
const osThreadAttr_t usartTask_attributes = {
  .name = "usartTask",
  .priority = (osPriority_t) osPriorityLow,
  .stack_size = 128 * 4
};
/* Definitions for myTask05 */
osThreadId_t myTask05Handle;
const osThreadAttr_t myTask05_attributes = {
  .name = "myTask05",
  .priority = (osPriority_t) osPriorityLow,
  .stack_size = 128 * 4
};
/* Definitions for passwdQueue01 */
osMessageQueueId_t passwdQueue01Handle;
const osMessageQueueAttr_t passwdQueue01_attributes = {
  .name = "passwdQueue01"
};
/* Definitions for myTimer01 */
osTimerId_t myTimer01Handle;
const osTimerAttr_t myTimer01_attributes = {
  .name = "myTimer01"
};
/* Definitions for ledEvent01 */
osEventFlagsId_t ledEvent01Handle;
const osEventFlagsAttr_t ledEvent01_attributes = {
  .name = "ledEvent01"
};
/* Definitions for flagEvent02 */
osEventFlagsId_t flagEvent02Handle;
const osEventFlagsAttr_t flagEvent02_attributes = {
  .name = "flagEvent02"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartTask02(void *argument);
void StartTask03(void *argument);
void StartTask04(void *argument);
void StartTask05(void *argument);
void Callback01(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* Create the timer(s) */
  /* creation of myTimer01 */
  myTimer01Handle = osTimerNew(Callback01, osTimerPeriodic, NULL, &myTimer01_attributes);

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of passwdQueue01 */
  passwdQueue01Handle = osMessageQueueNew (16, sizeof(struct passwdMsg), &passwdQueue01_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of keyTask */
  keyTaskHandle = osThreadNew(StartTask02, NULL, &keyTask_attributes);

  /* creation of ledTask */
  ledTaskHandle = osThreadNew(StartTask03, NULL, &ledTask_attributes);

  /* creation of usartTask */
  usartTaskHandle = osThreadNew(StartTask04, NULL, &usartTask_attributes);

  /* creation of myTask05 */
  myTask05Handle = osThreadNew(StartTask05, NULL, &myTask05_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* Create the event(s) */
  /* creation of ledEvent01 */
  ledEvent01Handle = osEventFlagsNew(&ledEvent01_attributes);

  /* creation of flagEvent02 */
  flagEvent02Handle = osEventFlagsNew(&flagEvent02_attributes);

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
	sprintf(temp,"p:%s pt:%s r:%d",passwd,passwd_test,strcmp(passwd,passwd_test));
	LCD_DisplayStringLine(Line6,(uint8_t*)temp);
	  
    sprintf(temp,"tF:%d c:%d l:%d p:%d",timeFlag,count[0],lcd_view_mod,passwd_wrong_count);
	LCD_DisplayStringLine(Line8,(uint8_t*)temp);  
	  
	sprintf(temp,"Rxbuff:%s",Rxbuff);
	LCD_DisplayStringLine(Line7,(uint8_t*)temp);
    osDelay(10);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartTask02 */
/**
* @brief Function implementing the keyTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask02 */
void StartTask02(void *argument)
{
  /* USER CODE BEGIN StartTask02 */
  /* Infinite loop */
  for(;;)
  {	   	
	if(stopTimeFlag)
	{
		osTimerStop(myTimer01Handle);
		timeFlag = 0;
	}
	
	//密码输入错误三次  LED2以0.1秒间隔闪烁 （5秒后熄灭）
	if(passwd_wrong_count>=3 && timeFlag == 0)
	{
		timeFlag = 1;
		passwd_wrong_count = 0;
		stopTimeFlag = 0;
		osTimerStart(myTimer01Handle,100);
	}
	//密码验证成功 LED1点亮5秒 （5秒后熄灭）
	if(lcd_view_mod && timeFlag == 0)
	{
		timeFlag = 2;
		stopTimeFlag = 0;
		osTimerStart(myTimer01Handle,100);
	}
		
	osDelay(10);
  }
  /* USER CODE END StartTask02 */
}

/* USER CODE BEGIN Header_StartTask03 */
/**
* @brief Function implementing the ledTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask03 */
void StartTask03(void *argument)
{
  /* USER CODE BEGIN StartTask03 */
  /* Infinite loop */
  for(;;)
  {
	//按键任务函数
	keyProc();
    osDelay(8);
  }
  /* USER CODE END StartTask03 */
}

/* USER CODE BEGIN Header_StartTask04 */
/**
* @brief Function implementing the usartTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask04 */
void StartTask04(void *argument)
{
  /* USER CODE BEGIN StartTask04 */
  /* Infinite loop */
  for(;;)
  {
	//更新密码
	getPasswdByUsart();
    osDelay(5);
  }
  /* USER CODE END StartTask04 */
}

/* USER CODE BEGIN Header_StartTask05 */
/**
* @brief Function implementing the myTask05 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask05 */
void StartTask05(void *argument)
{
  /* USER CODE BEGIN StartTask05 */
  /* Infinite loop */
  for(;;)
  {
	//密码的相关操作以及显示密码界面
	if(lcd_view_mod == 0)
	{
		PSDViewDisplay();
		pwmWorkByFre(1000,400);
	}
	//显示输出界面
	else if(lcd_view_mod == 1)
	{
		STAViewDisplay();
		pwmWorkByFre(500,50);
		passwd_flag = 0;
	}
    osDelay(2);
  }
  /* USER CODE END StartTask05 */
}

/* Callback01 function */
void Callback01(void *argument)
{
  /* USER CODE BEGIN Callback01 */
	changeAllLedByStateNumber(0);
	if(timeFlag && ++count[0]%50==0)
	{
		timeFlag = 0;
		lcd_view_mod = 0;
		stopTimeFlag = 1;
	}
	if(timeFlag == 1)
		changeLedStateByLocation(LED2,count[0]%2);
	if(timeFlag == 2)
		changeLedStateByLocation(LED1,1);
  /* USER CODE END Callback01 */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

