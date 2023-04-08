#include "config.h"

// data
char mTable[] = {'L','H'};
uint16_t pa1F[] = {250,125};

// 计时 0-给模式切换  1-LED2闪烁  2-给切换模式后的计时
uint16_t sysCount[3] = {5000,100,0};

// 记录按键按下的时间
__IO uint32_t uwKeyTick;

// LCD 模式
char mod;

// 定义显示数据
char M = 0,oldM = 0;
float V = 0;
char RK[] = {1,1},RKOld[] = {1,1},rkCount = 0;
uint16_t N = 0;
float MH = 0,ML = 0;

// PA7测得的频率
extern uint16_t f;
// 上一次频率的值
uint16_t fOld = 1;

// adc的值
double adcV = 0;

// PA1的占空比  0-表示以前的  1-表示现在的
char pa1Zhan[] = {32,32};

// 锁定标志 占空比锁
char lock = 0;

// LED2是否闪烁 1-是表示切换时间未到 0-表示至少到切换时间
char LED2Flag = 0;

/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    lcdDisplay
  * @brief   LCD显示数据
  * @param   char mod：显示模式 可以切换显示数据
  * @retval  None
  * @author  黑心萝卜三条杠
  
 **/
/* -------------------------------- end -------------------------------- */
static void lcdDisplay()
{
	char temp[20];
	extern uint32_t  cclValue ;
	// 数据显示界面
	if(mod == 0)
	{
	  LCD_DisplayStringLine(Line1,(u8*)"        DATA  ");
		sprintf(temp,"     M=%c     ",mTable[M]);
		LCD_DisplayStringLine(Line3,(u8*)temp);
		sprintf(temp,"     P=%d%%     ",pa1Zhan[1]);
		LCD_DisplayStringLine(Line4,(u8*)temp);
		sprintf(temp,"     V=%.1f     ",V);
		LCD_DisplayStringLine(Line5,(u8*)temp);
		
	}
	// 参数显示界面
	else if(mod == 1)
	{
		LCD_DisplayStringLine(Line1,(u8*)"        PARA  ");
		sprintf(temp,"     R=%d     ",RK[0]);
		LCD_DisplayStringLine(Line3,(u8*)temp);
		sprintf(temp,"     K=%d     ",RK[1]);
		LCD_DisplayStringLine(Line4,(u8*)temp);
		LCD_ClearLine(Line5);
	}
	// 统计界面
	else if(mod == 2)
	{
		LCD_DisplayStringLine(Line1,(u8*)"        RECD   ");
		sprintf(temp,"     N=%d    ",N);
		LCD_DisplayStringLine(Line3,(u8*)temp);
		sprintf(temp,"     MH=%.1f     ",MH);
		LCD_DisplayStringLine(Line4,(u8*)temp);
		sprintf(temp,"     ML=%.1f     ",ML);
		LCD_DisplayStringLine(Line5,(u8*)temp);
	}
}

/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    dataHandle
  * @brief   数据处理逻辑函数
  * @param   None
  * @retval  None
  * @author  黑心萝卜三条杠
  
 **/
/* -------------------------------- end -------------------------------- */
static void dataHandle(void)
{
	// adc的值
	//double adcV = 0;
	
	// PA7频率 与 速度V  相关的转换
	if(f != fOld)
	{
		V = (f*2*3.14*RK[0]*1.0)/(100*RK[1]);
		fOld = f;
		sysCount[2] = 0;
	}
	/** 统计最大速度 **/ 
	else if(f == fOld && sysCount[2] >= 2000)
	{
		// 低频
		if(M == 0) 
		{
			if(ML < V)
				ML = V;
		}
		// 高频
		else if(M == 1)
		{
			
			if(MH < V)
				MH = V;
		}
	}
	
	// ADC获取R37 与  PA1输出的转换
	adcV = getADC(&hadc2);
	// 处于解锁模式
	if(lock == 0)
	{
		if(0<= adcV && adcV < 1)
			pa1Zhan[1] = 10;
		else if(3<= adcV)
			pa1Zhan[1] = 85;
		else 
			pa1Zhan[1] = (char)((adcV*75- 55)/2+0.5);		
		// 占空比发生改变应该调整
		if(pa1Zhan[0] != pa1Zhan[1] && LED2Flag == 0)
		{
			__HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_2,pa1F[M]*pa1Zhan[1]/100);
			pa1Zhan[0] = pa1Zhan[1];
		}
	}
}

/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    sysInit
  * @brief   按键逻辑函数
  * @param   None
  * @retval  None
  * @author  黑心萝卜三条杠
  
 **/
/* -------------------------------- end -------------------------------- */
static void keyPro(void)
{
	signed char i = 0;
	// 按键扫描
	keyRefresh();
	
	// 按键按下 并且按键的值等于8也就是B4
	if(keyFalling == 8)
		uwKeyTick = HAL_GetTick();
	
	switch(keyRising)
	{
		// 按键B1
		case 1:
			mod++;
		  // 每次进去参数界面默认参数为R
		  if(mod == 1) rkCount = 0;
			// 退出参数界面 
			if(mod != 1 )
			{
				// 遍历参数 并且刷新
				for(i=0;i<2;++i)
					if(RKOld[i] != RK[i])
						RK[i] = RKOld[i];
			}
			if(mod == 3) mod = 0;
			break;
		// 按键B2
		case 2:
			// 数据界面 
			if(mod == 0 && LED2Flag == 0 && sysCount[0] >= 5000)
			{
				sysCount[0] = 0;
				LED2Flag = 1;
			}
			// 参数界面
			if(mod == 1)
						rkCount ^= 1;
			break;
		// 按键B3
		case 4:
			// 参数界面 加1
			if(mod == 1)
				if(++RKOld[rkCount] == 11) RKOld[rkCount] = 1;
			break;
		// 按键B4
		case 8:
			// 数据界面 
			if(mod == 0 )
				// 长按键锁住
				if(HAL_GetTick() - uwKeyTick > 2000)
					 lock = 1;
			  // 短按键解锁
				else
					lock = 0;
			// 参数界面 减1
			else if(mod == 1)
				if(--RKOld[rkCount] == 0) RKOld[rkCount] = 10;
			break;
		// 其他
		default:
			break;
	}
	
	// 延时5秒切换
	if(LED2Flag && sysCount[0]>=5000)
	{
		M ^= 1;
		// 切换次数增加 
		N++;
		LED2Flag = 0;
	}
}

/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    sysInit
  * @brief   LED逻辑函数
  * @param   None
  * @retval  None
  * @author  黑心萝卜三条杠
  
 **/
/* -------------------------------- end -------------------------------- */
static void ledWork(void)
{	
	// 数据界面LED1电量
	if(mod == 0)
	   changeLedStateByLocation(LED1,1);
	else
	   changeLedStateByLocation(LED1,0);
	
	// 切换期间 LED2闪烁
	if(LED2Flag && sysCount[1] >= 100)
	{
		rollbackLedByLocation(LED2);
		sysCount[1] = 0;
	}
	else if(!LED2Flag)		
	   changeLedStateByLocation(LED2,0);
	
	// 锁定模式下 LED3电量
	if(lock)
	   changeLedStateByLocation(LED3,1);
	else
	   changeLedStateByLocation(LED3,0);
		
}

/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    sysInit
  * @brief   自定义的系统初始化函数
  * @param   None
  * @retval  None
  * @author  黑心萝卜三条杠
  
 **/
/* -------------------------------- end -------------------------------- */
void sysInit(void)
{
	// LCD初始化
	lcdInit();

	// 关闭所有LED
	changeAllLedByStateNumber(0);	
	
	//打开定时器2通道2的PWM输出功能
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);
	
	//打开定时器3通道2	 
    HAL_TIM_IC_Start_IT(&htim3,TIM_CHANNEL_2);
}

/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    sysWork
  * @brief   自定义的系统工作逻辑函数
  * @param   None
  * @retval  None
  * @author  黑心萝卜三条杠
  
 **/
/* -------------------------------- end -------------------------------- */
void sysWork(void)
{	
	// 按键处理函数
	keyPro();
	
	// 数据处理逻辑函数
	dataHandle();
	
	// LED显示函数
	ledWork();
	
	// LCD显示
	lcdDisplay();
}
