#include "config.h"
 
// 数据界面显示数据
struct data data = {0.0,0.0,0,0,0,'A'};
// 参数界面显示数据
struct para para = {20,20,1000};
// 按时间排序的历史数据
float oldData[2][5] = {{0}};
// 按时间排序的历史数据的计数值
int oldCount[2] = {0,0};
// 保存按键触发模式下 触发一次数据采集的标志  由于有中断与延时为了使得按键效果更加准确这里最好使用全局变量或者是static修饰的局部变量
char falg = 0;
// 按大小排序的历史数据
float sortData[5] = {0,0,0,0,0};
// 记录本次光照值以及上次光照值
float lightData[2] = {0,0};

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
static void lcdDisplay(char mod)
{
	char temp[20];
	// 数据显示界面
	if(mod == 0)
	{
		LCD_DisplayStringLine(Line1,(u8*)"        DATA      ");
		sprintf(temp,"    a:%.1f        ",data.a);
		LCD_DisplayStringLine(Line2,(u8*)temp);
		sprintf(temp,"    b:%.1f         ",data.b);
		LCD_DisplayStringLine(Line3,(u8*)temp);
		sprintf(temp,"    f:%dHz          ", data.f);
		LCD_DisplayStringLine(Line4,(u8*)temp);
		
		sprintf(temp,"    ax:%d         ",data.ax);
		LCD_DisplayStringLine(Line6,(u8*)temp);
		sprintf(temp,"    bx:%d         ",data.bx);
		LCD_DisplayStringLine(Line7,(u8*)temp);
		sprintf(temp,"    mode:%c       ",data.mod);
		LCD_DisplayStringLine(Line8,(u8*)temp);
	}
	// 参数显示界面
	else if(mod == 1)
	{
		LCD_DisplayStringLine(Line1,(u8*)"        PARA      ");
		sprintf(temp,"    Pax:%d        ",para.Pax);
		LCD_DisplayStringLine(Line2,(u8*)temp);
		sprintf(temp,"    Pbx:%d        ",para.Pbx);
		LCD_DisplayStringLine(Line3,(u8*)temp);
		sprintf(temp,"    Pf:%d         ", para.Pf);
		LCD_DisplayStringLine(Line4,(u8*)temp);
	}
	// 多余的测试
//	sprintf(temp,"%s : %d : %d %.4f       ",ucRxbuff,RP[0],RP[1],lightData[0]);
//	LCD_DisplayStringLine(Line0,(u8*)temp);
}

/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    myAbs
  * @brief   自己写的求绝对值函数
  * @param   float a：参与求绝对值的数1 float b：参与求绝对值的数2
  * @retval  None
  * @author  黑心萝卜三条杠
  * @Data    2023-03-24
 **/
/* -------------------------------- end -------------------------------- */
static float myAbs(float a,float b)    
{                              
	float temp = a-b;  
	if(temp >0) return temp;
	else return -temp; 
}

/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    sort
  * @brief   对长度为len的数组data进行不改变原数组顺序的排序 将结果保存在resData
  * @param   float*data:源数据 int len：源数据长度 float*resData：排序后结果
  * @retval  None
  * @author  黑心萝卜三条杠
  * @Data    2023-03-24
 **/
/* -------------------------------- end -------------------------------- */
static void sort(float*data,int len,float*resData)
{
	// 复制原数组到新数组中
    memcpy(resData, data, len * sizeof(float)); 
    // 冒泡排序 
    for (int i = 0; i < len - 1; ++i) {
        for (int j = i + 1; j < len; ++j) {
            if (resData[i] > resData[j]) {
                int temp = resData[i];
                resData[i] = resData[j];
                resData[j] = temp;
            }
        }
    }
}

/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    getData
  * @brief   数据更新函数
  * @param   None
  * @retval  None
  * @author  黑心萝卜三条杠
  * @Data    2023-03-24
 **/
/* -------------------------------- end -------------------------------- */
static void getData(void)
{
	int flagPro = 0;
	float temp = 0;
	// 更新频率
	data.f = RP[2];
	// 触发模式
	if(data.mod == 'A' && falg == 1)
	{
		falg = 0;
		flagPro = 1;
	}
	// 环境光照值触发
	else if(data.mod == 'B' )
	{
		lightData[0] = getR47ADC();
		if(lightData[0]-lightData[1] > 10)
			flagPro = 1;
		// 保存本次光照值
		lightData[1] = lightData[0];
	}
	// 采集了数据 需要更新角度值
	if(flagPro == 1)
	{
		// 根据折线图转换角度a
		if(0 < RP[0] && RP[0] <= 10)
			temp = 0;
		else if(10 < RP[0] && RP[0] < 90)
			temp = (RP[0]-10)*9*1.0/4;
		else
			temp = 180;
		data.ax = myAbs(temp,data.a);
		data.a = temp;
		oldData[0][oldCount[0]] = temp;
		if(++oldCount[0] == 5) oldCount[0] = 0;
		// 根据折线图转换角度b
		if(0 < RP[1] && RP[1] <= 10)
			temp = 0;
		else if(10 < RP[1] && RP[1] < 90)
			temp = (RP[1]-10)*9*1.0/8;
		else
			temp = 90;
		data.bx = myAbs(temp,data.b);
		data.b = temp;
		oldData[1][oldCount[1]] = temp;
		if(++oldCount[1] == 5) oldCount[1] = 0;
	}
}

/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    usartPro
  * @brief   串口处理函数
  * @param   None
  * @retval  None
  * @author  黑心萝卜三条杠
  * @Data    2023-03-24
 **/
/* -------------------------------- end -------------------------------- */
static void usartPro(void)
{
	char temp [40];
	
	// 去除一些不非正常情况的结果
	if(strlen((char*)ucRxbuff) == 0)
		return ;
	
	// 查询当前数据
	if(strcmp((char*)ucRxbuff,"a?") == 0)
		sprintf(temp,"a:%1.f\r\n",data.a);
	else if(strcmp((char*)ucRxbuff,"b?") == 0)
		sprintf(temp,"b:%1.f\r\n",data.b);
	// 查询历史数据（按时间顺序）
	else if(ucRxbuff[0]=='a' && ucRxbuff[1]=='a' && ucRxbuff[2]=='?')
		sprintf(temp,"aa:%1.f-%1.f-%1.f-%1.f-%1.f\r\n",oldData[0][0],oldData[0][1],oldData[0][2],oldData[0][3],oldData[0][4]);
	else if(ucRxbuff[0]=='b' && ucRxbuff[1]=='b' && ucRxbuff[2]=='?')
		sprintf(temp,"bb:%1.f-%1.f-%1.f-%1.f-%1.f\r\n",oldData[1][0],oldData[1][1],oldData[1][2],oldData[1][3],oldData[1][4]);
	// 查询历史数据（按大小）
	else if(ucRxbuff[0]=='q' && ucRxbuff[1]=='a' && ucRxbuff[2]=='?')
	{
		sort(oldData[0],5,sortData);
		sprintf(temp,"aa:%1.f-%1.f-%1.f-%1.f-%1.f\r\n",sortData[0],sortData[1],sortData[2],sortData[3],sortData[4]);
	}
	else if(ucRxbuff[0]=='q' && ucRxbuff[1]=='b' && ucRxbuff[2]=='?')
	{
		sort(oldData[1],5,sortData);
		sprintf(temp,"bb:%1.f-%1.f-%1.f-%1.f-%1.f\r\n",sortData[0],sortData[1],sortData[2],sortData[3],sortData[4]);
	}
	// 接收到其他数据
	else
		sprintf(temp,"error\t\n");
	// 串口数据发送
	HAL_UART_Transmit(&huart1,(uint8_t*)temp,sizeof(char)*strlen(temp),20);
	memset(ucRxbuff,0,sizeof(ucRxbuff));
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
	static char mod[2] = {0,0};
	unsigned char keyValue = scanKey();
	switch(keyValue){
		case 1: 
			// 切换LCD显示界面
			mod[0] ^= 1;
		    break;
		case 2: 
			// 增加角度变化参数
			if(mod[0] == 1)
			{
				para.Pax += 10;
				para.Pbx += 10;
				if(para.Pax == 70)
					para.Pax = 10;
				if(para.Pbx == 70)
					para.Pbx = 10;
			}
			break;
		case 3: 
			// 增加频率参数
			if(mod[0] == 1)
			{
				para.Pf += 1000;
				if(para.Pf == 11000)
					para.Pf = 1000;
			}
			// 切换触发模式
			else if(mod[0] == 0)
			{
				mod[1] ^= 1;
				data.mod = 'A' + mod[1];
			}
			break;
		case 4: 
			// 数据刷新
			if(data.mod == 'A')
				falg = 1;
			break;
		default: break;
	}
	
	// LCD显示
	lcdDisplay(mod[0]);
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
	// a角度变化大于其参数
	if(data.ax > para.Pax)
		changeLedStateByLocation(LED1,1);
	else
		changeLedStateByLocation(LED1,0);
	// b角度变化大于其参数
	if(data.bx > para.Pbx)
		changeLedStateByLocation(LED2,1);
	else
		changeLedStateByLocation(LED2,0);
	// f频率大于其参数
	if(data.f > para.Pf)
		changeLedStateByLocation(LED3,1);
	else
		changeLedStateByLocation(LED3,0);
	// 处于模式A
	if(data.mod == 'A')
		changeLedStateByLocation(LED4,1);
	else
		changeLedStateByLocation(LED4,0);
	// 角度a-b>10
	if(data.a - data.b > 10)
		changeLedStateByLocation(LED5,1);
	else
		changeLedStateByLocation(LED5,0);
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
	// 可以说是初始化串口中断
	HAL_UART_Receive_IT(&huart1,(uint8_t *)&_ucRxbuff,sizeof(_ucRxbuff)); 
	//打开定时器2通道2的输入捕获功能
    HAL_TIM_IC_Start_IT(&htim2,TIM_CHANNEL_2);
	//打开定时器3通道2的输入捕获功能
    HAL_TIM_IC_Start_IT(&htim3,TIM_CHANNEL_2);
	//打开定时器16通道1的输入捕获功能
    HAL_TIM_IC_Start_IT(&htim16,TIM_CHANNEL_1);
	// 打开ADC的自动校准
//	HAL_ADCEx_Calibration_Start(&hadc2,ADC_SINGLE_ENDED);	
	// 关闭所有LED
	changeAllLedByStateNumber(0);
	
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
	// 更新数据
	getData();
	// 串口数据处理
	usartPro();
	// 按键处理函数
	keyPro();
	// LED显示函数
	ledWork();
}

