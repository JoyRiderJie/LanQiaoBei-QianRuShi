#include "config.h"

// 控制LCD显示
char mod = 0;
// 电压及电压参数相关
double V = 0 , Vmax = 3.0,Vmin = 1.0,VmaxTemp = 3.0,VminTemp = 1.0 ,VoldData = 0;
// 计时相关
uint16_t T = 0;
// LED相关
char LEDFlag[3] = {0,0,0};
// 计时相关
uint32_t count = 0;

// 测试
//uint16_t tCount  = 0;

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
	char temp[20];
	// 数据显示界面
	if(mod == 0)
	{
		sprintf(temp,"      Data  ");
		LCD_DisplayStringLine(Line0,(u8*)temp);
		sprintf(temp," V:%.2fV      ",V);
		LCD_DisplayStringLine(Line2,(u8*)temp);
		sprintf(temp," T:%ds        ",T);
		LCD_DisplayStringLine(Line3,(u8*)temp);
	}
	// 参数显示界面
	else if(mod == 1)
	{
		sprintf(temp,"      Para  ");
		LCD_DisplayStringLine(Line0,(u8*)temp);
		sprintf(temp," Vmax:%.1fV      ",Vmax);
		LCD_DisplayStringLine(Line2,(u8*)temp);
		sprintf(temp," Vmin:%.1fV        ",Vmin);
		LCD_DisplayStringLine(Line3,(u8*)temp);
	}
	
//	// 测试数据
//	sprintf(temp,"maxT:%.1f minT:%.1f  ",VmaxTemp,VminTemp);
//	LCD_DisplayStringLine(Line6,(u8*)temp);
//	LCD_DisplayStringLine(Line8,(u8*)ucRxbuff);
//	sprintf(temp,"tC:%d    ",tCount++);		
//	LCD_DisplayStringLine(Line8,(u8*)temp);
//	sprintf(temp,"V:%.2fVt:%.2fcs:%d",V,VoldData,LEDFlag[0]);		
//	LCD_DisplayStringLine(Line9,(u8*)temp);
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
	// 未收到数据
	if(strlen((char*)ucRxbuff) == 0) return ;
	// 收到数据并且判断其格式是否正确
	if(strlen((char*)ucRxbuff) == 9 && ucRxbuff[0] == '"' && '0' <= ucRxbuff[1] && ucRxbuff[1] <= '9' && ucRxbuff[2] == '.' && '0' <= ucRxbuff[3] && ucRxbuff[3] <= '9' \
		&& ucRxbuff[4] == ',' && '0' <= ucRxbuff[5] && ucRxbuff[5] <= '9' && ucRxbuff[6] == '.' && '0' <= ucRxbuff[7] && ucRxbuff[7] <= '9' && ucRxbuff[8] == '"')
	{
		VmaxTemp = ((ucRxbuff[1]-'0')*10 + ucRxbuff[3]-'0')*1.0/10;
		VminTemp = ((ucRxbuff[5]-'0')*10 + ucRxbuff[7]-'0')*1.0/10;
		if(VmaxTemp > 3.3 || VminTemp>3.3)
			LEDFlag[2] = 1;
		else
			LEDFlag[2] = 0;
	}
	else
		LEDFlag[2] = 1;
	
	memset(ucRxbuff,0,sizeof((char*)ucRxbuff));
	lenBuff = 0;
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
			mod ^= 1;
		  //即将进入数据界面 判断参数是否合理
			if(mod == 0  ) 
			{
				// 判断数据是否合理
				if(VmaxTemp >= VminTemp+1)
				{
					Vmax = VmaxTemp;
					Vmin = VminTemp;
					LEDFlag[1] = 0;
				}
				else
					LEDFlag[1] = 1;
			}
			break;
		// 按键B2
		case 2:
			if(mod == 1)
			{
				VmaxTemp += 0.1;
				if(VmaxTemp >= 3.4) VmaxTemp = 0;
			}
			break;
		// 按键B3
		case 4:
			if(mod == 1)
			{
				VminTemp += 0.1;
				if(VminTemp >= 3.4) VminTemp = 0;
			}
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
	// 触发计时功能 且计时未结束 LED点亮 
	changeLedStateByLocation(LED1,LEDFlag[0]);
	
	// 参数设置不正确就点亮LED2 直到其正确
	changeLedStateByLocation(LED2,LEDFlag[1]);
	
	// 串口一旦收到不正确或者是不满足要求的指令就点亮LED3
	changeLedStateByLocation(LED3,LEDFlag[2]);	
}
/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    ADCHandle
  * @brief   ADC数据获取到后的处理函数
  * @param   None
  * @retval  None
  * @author  黑心萝卜三条杠
  * @Data    2023-03-24
 **/
/* -------------------------------- end -------------------------------- */
void ADCHandle(void)
{	
	// ADC获取
	V = getADC(&hadc2);
	
	// 触发计时
	if(V - VoldData > 0.02 && V -  Vmin >= 0 && V -  Vmin <= 0.03 )
	{
		LEDFlag[0] = 1;
		T = 0;
	}
	// 结束计时
	else if(V - VoldData > 0.02 &&  Vmax -  V >= 0 && Vmax -  V <= 0.03)
		LEDFlag[0] = 0;
	// 保存上一次的历史数据
	VoldData = V;
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
	
	// 使能串口中断
	HAL_UART_Receive_IT(&huart1,(uint8_t *)&_ucRxbuff,sizeof(_ucRxbuff)); 
	
	// 初始化ADC相关的值
	VoldData = V = getADC(&hadc2);
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
	// 按键处理函数
	keyPro();
	
	// ADC数据获取以及处理
	ADCHandle();
	
	// 串口逻辑函数
	usartPro();
	
	// LED显示函数
	if(HAL_GetTick() % 15 == 0)
		ledWork();
	
	// LCD显示
	if(HAL_GetTick() % 10 == 0)
		lcdDisplay();
}




