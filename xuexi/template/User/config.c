#include "config.h"

/************************该.c文件用于写系统初始化以及系统初始化后的主逻辑***********************************/

//* 存储LCD显示数据的变量
unsigned char lcdDisplayData[20];

//* 记录syswork函数执行的次数
u8 sysWorkCount = 0;

#if 0
//* 记录上一次串口接收到的数据
uint8_t oldRxBuff[USARTMAXLENTH];

#endif

//* 记录应该点亮的LED灯

uint16_t ledLocation = 0x0000;

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
	lcdInit();

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
	changeLedStateByLocation(GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_12|GPIO_PIN_11,0); 
	//毫秒级延时 下面这部分延时是延时1s
	HAL_Delay(1000);
	changeLedStateByLocation(GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_12|GPIO_PIN_11,1); 
	HAL_Delay(1000);

	//LCD显示"hello word!"
	LCD_DisplayStringLine(Line1,"hello word!");
	//LCD记录sysWorkCount的数值
	sprintf((char*)lcdDisplayData,"WorkCount is %d",(int)sysWorkCount);
	LCD_DisplayStringLine(Line2,lcdDisplayData);
	
#endif
	sysWorkCount++;
#if 0
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
	if(scanKey() == 2){
		rollbackLedByLocation(GPIO_PIN_9|GPIO_PIN_11|GPIO_PIN_13);
	}
#endif

#if 0
	//按键的单击与双击测试
	if(doubleScankey() == 1){
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

#endif

#if  0
	//串口1的中断接收测试  跟上一次串口接收到的数据比较 如果不同就发送到PC
	/*使用说明：
		1.如果想要将串口接收到的数据改成定长的 可以修改串口初始化与串口中断中HAL_UART_Receive_IT函数
		  的第三个参数
	*/
	if(strcmp((char*)oldRxBuff,(char*)Rxbuff) != 0){
		HAL_UART_Transmit(&huart1,(uint8_t*)Rxbuff,strlen((char*)Rxbuff),50);
		//为了避免Rxbuff中能够存储历史数据 下面特意将其清空
		memset(Rxbuff,0,100);
		RxCount = 0;
		sprintf((char*)oldRxBuff,"%s",(char*)Rxbuff);
		rollbackLedByLocation(GPIO_PIN_9);
	}
	if(RxCount){
		changeLedStateByLocation(LED7,1);
	}
		changeLedStateByLocation(LED8,0);
#endif

#if 0
	//按键的长按-短按 按下-松开设计
	keyproc();
#endif

#if 0
	// if(uTick_ms%1010<1000) return;
	HAL_Delay(1000);
	rollbackLedByLocation(GPIO_PIN_8|GPIO_PIN_10|GPIO_PIN_12);
	sprintf((char*)Rxbuff,"%d\r\n\n",uTick_ms);
	HAL_UART_Transmit(&huart1,(uint8_t*)Rxbuff,strlen((char*)Rxbuff),50);
#endif


#if 0
// 按键测试 长按 短按 单击双击 理论上应该使用定时器 但是为了测试方便此处直接在while循环中测试 
	scanKeyUseStructAndTime();
	
	sprintf((char*)lcdDisplayData,"%3d,%3d,%3d",key[0].flag,key[0].longFlag,key[0].doubleFlag);
	LCD_DisplayStringLine(Line2,lcdDisplayData);
		
	sprintf((char*)lcdDisplayData,"%4X",ledLocation);
	LCD_DisplayStringLine(Line3,lcdDisplayData);
	
	
	changeAllLedByStateNumber(0);
	
	//1被单次按下
	if(key[0].flag==1)
	{
		ledLocation |= LED1;
		rollbackLedByLocation(LED4);
		//清楚本次有效值
		key[0].flag=0;
	}
	//700ms长按键
	if(key[1].longFlag==1)
	{
		ledLocation |= LED2;
		//清楚本次有效值
		key[1].longFlag=0;
	}
	//双击判断
	if(key[2].doubleFlag==1)
	{
		ledLocation |= LED3;
		key[2].doubleFlag=0;
	}
	changeLedStateByLocation(ledLocation,1);

	HAL_Delay(10);
#endif

}

/***********************************************
 * 函数功能：按键{按下-松开}{长按-短按}设计函数
 * 函数参数：无
 * 函数返回值：无
************************************************/
void keyproc(void)
{
    //本次按键的值 按键是否按下  按键是否松开
    char keyValue = 0,keyDown = 0,keyUp = 0;
	//上次按键的值
	static char keyOld = 0;
	//记录按键开始按下的时间
	static uint32_t keyTime = 0; 
	//用于判断是长按还是短按的标志  0-短按 1-长按 默认值为短按
	int keyFlag = 0;

    keyValue = scanKeyNoTime();
    keyDown = keyValue & (keyOld ^ keyValue);
    keyUp = ~keyValue & (keyOld ^ keyValue);

#if 0	
	/*************按键按下松开使用的示例*********/
	//按键按下设计
	if(keyDown && keyValue == 1){
		//保存按键按下的时间
		rollbackLedByLocation(GPIO_PIN_8);
	}
	//按键松开设计
	if(keyUp){
		changeLedStateByLocation(GPIO_PIN_9,0);
	}
#endif

#if 0
	/**************按键长按短按的使用示例********/
	if(keyDown){
		keyTime = uTick_ms;
	}
	//在次设计为按键按下时间小于800ms都统称之为短按
	if(uTick_ms - keyTime > 800 && keyOld){
		keyFlag = 1;
	}
	//按键松开 进行键值判断
	if(keyUp){
		switch(keyOld){
			// 按键B1 
			case 1: 
				//长按
				if(keyFlag == 1){
					rollbackLedByLocation(GPIO_PIN_11);
				}
				//短按
				else{
					rollbackLedByLocation(GPIO_PIN_12);
				}
				break;
			// 按键B2 
			case 2: 
				//长按
				if(keyFlag){
					
				}
				//短按
				else{

				}
				break;
			// 按键B3 
			case 3: 
				//长按
				if(keyFlag){
					
				}
				//短按
				else{

				}
				break;
			// 按键B4 
			case 4: 
				//长按
				if(keyFlag){
					
				}
				//短按
				else{

				}
				break;
			//屏蔽其他状态
			default:break;
		}
	}
#endif

	/************单击双击按键的使用示例********/
	if(keyValue){
		rollbackLedByLocation(GPIO_PIN_15);
	}
    keyOld = keyValue;
}






