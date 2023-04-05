#include "config.h"

/* 存储串口1接收的数据 
** 数据样例：CNBR: A392: 2002021 20000  停车类型:车辆编号:进入/退出时间(YYMMDDHmmSS)
** 数据样例解释：表示停车类型CNBR,编号为A392的车辆,进入停车场时间为2020年2月2日12时整。
**/
//频率测量
u32 crrl_t,frd;
u32 oldFrd = 1;

//记录定时器7触发次数
uint16_t uiTime7Count = 0;
//记录按键的值
uint8_t ucKeyNumber = 0;
//记录车辆信息的头指针
struct head*pcarMessage;


//VNBR类型停车单价
double dVnbrPrice = 2.0;
//CNBR类型停车单价
double dCnbrPrice = 3.5;
//记录显示界面
int iDisplayMod = 1;
//记录PWM输出的模式
int iPwmMode = 0;

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
	HAL_UART_Receive_IT(&huart1,(uint8_t *)&_ucRxbuff,sizeof(_ucRxbuff)); 
	//打开定时器17通道1的PWM输出功能
	HAL_TIM_PWM_Start(&htim17,TIM_CHANNEL_1);
	_pwmWorkByFre(500);
	
	//为汽车存储申请空间
	pcarMessage = (struct head*)malloc(sizeof(struct head));
	pcarMessage->uiIdleCount = 8;
	pcarMessage->uiCnbrCount = 0;
	pcarMessage->uiVnbrCount = 0;
}


/***********************************************
* 函数功能：系统工作逻辑函数
* 函数参数：无
* 函数返回值：无
***********************************************/
void sysWork(void)
{
	//按键处理
	_keyPro();
	
	//串口数据处理
	_usartMsgProcess();
	
	//显示停车场车辆信息
	if(iDisplayMod)
		_dataMessageDisplay();
	//显示单价信息
	else
		_paraMessageDisplay();
	//LED工作函数
	_LEDDisplay();
}

/***********************************************
* 函数功能：按键工作逻辑函数
* 函数参数：无
* 函数返回值：无
***********************************************/
void _keyPro(void)
{
	ucKeyNumber = scanKey();
	switch(ucKeyNumber)
	{
		//按键B1 切换显示界面
		case 1: 
			iDisplayMod ^= 1;
			break;
		//按键B2 增加费率
		case 2: 
			if(!iDisplayMod)
			{
				dVnbrPrice += 0.5;
				dCnbrPrice += 0.5;
			}
			break;
		//按键B3 减少费率
		case 3: 
			if(!iDisplayMod)
			{
				dVnbrPrice -= 0.5;
				dCnbrPrice -= 0.5;
			}
			break;
		//按键B4 切换PA7的PWM输出
		case 4: 
			iPwmMode ^= 1;
			//输出占空比为20的PWM
			if(iPwmMode)
				_pwmWorkByFre(200);
			//持续输出低电平
			else
				_pwmWorkByFre(500);
			break;
		default:return ;
	}
	ucKeyNumber = 0;
}

/****************************************
* 函数功能：汽车信息显示界面
* 函数参数：无
* 函数返回值：无
****************************************/
void _dataMessageDisplay(void)
{
	char temp[20];
	LCD_DisplayStringLine(Line1,(uint8_t*)"       Data");
	sprintf(temp,"   CNBR:%3d  ",pcarMessage->uiCnbrCount);
	LCD_DisplayStringLine(Line3,(uint8_t*)temp);
	sprintf(temp,"   VNBR:%3d  ",pcarMessage->uiVnbrCount);
	LCD_DisplayStringLine(Line5,(uint8_t*)temp);
	sprintf(temp,"   IDLE:%3d",pcarMessage->uiIdleCount);
	LCD_DisplayStringLine(Line7,(uint8_t*)temp);
}

/****************************************
* 函数功能：汽车信息显示界面
* 函数参数：无
* 函数返回值：无
****************************************/
void _paraMessageDisplay(void)
{
	char temp[20];
	LCD_DisplayStringLine(Line1,(uint8_t*)"       Para");
	sprintf(temp,"   CNBR:%.2f",dCnbrPrice);
	LCD_DisplayStringLine(Line3,(uint8_t*)temp);
	sprintf(temp,"   VNBR:%.2f",dVnbrPrice);
	LCD_DisplayStringLine(Line5,(uint8_t*)temp);
	LCD_ClearLine(Line7);
}

/****************************************
* 函数功能：修改PWM的占空比
* 函数参数：
*			unsigned int compareDate:PWM的比较值
* 函数返回值：无
****************************************/
void _pwmWorkByFre(unsigned int compareDate)
{
	__HAL_TIM_SetCompare(&htim17,TIM_CHANNEL_1,compareDate);
}

/****************************************
* 函数功能：LED工作函数
* 函数参数：无
* 函数返回值：无
****************************************/
void _LEDDisplay(void)
{
	//关闭所有LED灯
	changeAllLedByStateNumber(0);
	//有空余 LED1点亮
	if(pcarMessage->uiIdleCount > 0)
		changeLedStateByLocation(LED1,1);
	//PWM占空比为20 LED2点亮
	if(iPwmMode)
		changeLedStateByLocation(LED2,1);
}

/****************************************
* 函数功能：处理接收串口信息
* 函数参数：无
* 函数返回值：无
****************************************/
void _usartMsgProcess(void)
{
	char temp[19];
	//串口未收到数据该函数应该直接返回
	if(strlen((char*)ucRxbuff) == 0) return ;
	
	struct node*msg = NULL;
	//需要一个节点存储车辆信息
	struct node* node = (struct node*)malloc(sizeof(struct node));
	
	//串口发送的数据长度不对
	if(strlen((char*)ucRxbuff)!= 22)
		goto MYERROR;
	
	//解析串口信息
	sscanf((char*)ucRxbuff,"%4s:%4s:%2d%2d%2d%2d%2d%2d",node->ucType,node->ucCode,&node->year,&node->month,&node->day,&node->hour,&node->minute,&node->second);
	msg = searchListNode(pcarMessage,node->ucCode);	
	//车辆出库
	if(msg != NULL)
	{
		uint32_t longTime[3] = {0,0,0};
		//记录停车时间 单位为h
		double dStopTime = 0;
		//记录停车费用 单位为元
		double dStopMoney = 0;	
		//判断数据是否合理 不合理直接返回打印错误信息
		if(checkData(msg->ucType,node->year,node->month,node->day,node->hour,node->minute,node->second)==0 || strcmp(node->ucType,msg->ucType)!=0)
			goto MYERROR;
		else
		{
			//时间转换
			longTime[0] = myMktime(2000+msg->year,msg->month,msg->day,msg->hour,msg->minute,msg->second);
			longTime[1] = myMktime(2000+node->year,node->month,node->day,node->hour,node->minute,node->second);
			longTime[2] = longTime[1]-longTime[0];
			//计算停留时间
			dStopTime = ceil((double)(longTime[2]*1.0/3600));		
			//计算停车费
			if(msg->ucType[0] == 'C')
				dStopMoney = dStopTime*dCnbrPrice;
			else
				dStopMoney = dStopTime*dVnbrPrice;
			//发送信息到PC
			sprintf(temp,"%s:%s:%.0f,%.2f\r\n",msg->ucType,msg->ucCode,dStopTime,dStopMoney);
			HAL_UART_Transmit(&huart1,(uint8_t*)temp,sizeof(temp),150);
			
			//车辆出库
			deleteListNode(pcarMessage,msg->ucCode);
		}
	}
	// 新车入库
	else
	{
		node->pNext = NULL;
		//判断数据是否合理 不合理直接返回打印错误信息
		if(checkData(node->ucType,node->year,node->month,node->day,node->hour,node->minute,node->second)==0 || pcarMessage->uiIdleCount-1<0)
			goto MYERROR;
		//数据无误  添加车辆信息到存储链表中
		else
			addListNode(pcarMessage,node);	
	}	
	
	//清除本次串口接收到的数据 避免影响后续数据
	memset(ucRxbuff,0,sizeof(ucRxbuff));
	//处理完本次串口接收到的数据后清除标志位
	lenBuff = 0;
	return ;
	
	//接收数据出现问题时 发送错误信息到PC
MYERROR:	
		HAL_UART_Transmit(&huart1,(uint8_t*)"Error\r\n",sizeof("Error\r\n"),50);
		//清除本次串口接收到的数据 避免影响后续数据
		memset(ucRxbuff,0,sizeof(ucRxbuff));
		//处理完本次串口接收到的数据后清除标志位
		lenBuff = 0;
}
#if 0

void _usartMsgProcess(void)
{
	char temp[19];
	//串口未收到数据该函数应该直接返回
	if(!iRxFlag) return ;
	
	struct node*msg = NULL;
	//串口发送的数据长度不对
	if(strlen((char*)ucRxbuff)!= 22)
	{
		goto MYERROR;
	}
	//获取本次车辆的车牌 用于查找该车是否已经入库
	getStringRxBuffDataByLocation((char*)ucRxbuff,temp,5,9);
	msg = searchListNode(pcarMessage,temp);
	
	//车辆出库
	if(msg != NULL)
	{
		uint32_t longTime[3] = {0,0,0};
		//记录停车时间 单位为h
		double dStopTime = 0;
		//记录停车费用 单位为元
		double dStopMoney = 0;
		//记录开始停车时间 结束停车时间
		struct node*eTime = (struct node*)malloc(sizeof(struct node));
#if 0
		getStringRxBuffDataByLocation((char*)ucRxbuff,eTime->ucType,0,4);
		eTime->year = getIntRxBuffDataByLocation((char*)ucRxbuff,10,12);
		eTime->month = getIntRxBuffDataByLocation((char*)ucRxbuff,12,14);
		eTime->day = getIntRxBuffDataByLocation((char*)ucRxbuff,14,16);
		eTime->hour = getIntRxBuffDataByLocation((char*)ucRxbuff,16,18);
		eTime->minute = getIntRxBuffDataByLocation((char*)ucRxbuff,18,20);
		eTime->second = getIntRxBuffDataByLocation((char*)ucRxbuff,20,22);
#else
		sscanf((char*)ucRxbuff,"%s:");
#endif		
		//判断数据是否合理 不合理直接返回打印错误信息
		if(checkData(msg->ucType,eTime->year,eTime->month,eTime->day,eTime->hour,eTime->minute,eTime->second)==0 || strcmp(eTime->ucType,msg->ucType)!=0)
		{
			goto MYERROR;
		}
		else
		{
			//时间转换
			longTime[0] = myMktime(2000+msg->year,msg->month,msg->day,msg->hour,msg->minute,msg->second);
			longTime[1] = myMktime(2000+eTime->year,eTime->month,eTime->day,eTime->hour,eTime->minute,eTime->second);
			longTime[2] = longTime[1]-longTime[0];
			//计算停留时间
			dStopTime = ceil((double)(longTime[2]*1.0/3600));		
			//计算停车费
			if(msg->ucType[0] == 'C')
				dStopMoney = dStopTime*dCnbrPrice;
			else
				dStopMoney = dStopTime*dVnbrPrice;
			//发送信息到PC
			sprintf(temp,"%s:%s:%.0f,%.2f\r\n",msg->ucType,msg->ucCode,dStopTime,dStopMoney);
			HAL_UART_Transmit(&huart1,(uint8_t*)temp,sizeof(temp),150);
			
			//车辆出库
			deleteListNode(pcarMessage,msg->ucCode);
		}
	}
	// 新车入库
	else
	{
		//新车入库 需要新增一个节点存储车辆信息
		struct node* node = (struct node*)malloc(sizeof(struct node));
		node->pNext = NULL;
		getStringRxBuffDataByLocation((char*)ucRxbuff,node->ucType,0,4);
		getStringRxBuffDataByLocation((char*)ucRxbuff,node->ucCode,5,9);
		node->year = getIntRxBuffDataByLocation((char*)ucRxbuff,10,12);
		node->month = getIntRxBuffDataByLocation((char*)ucRxbuff,12,14);
		node->day = getIntRxBuffDataByLocation((char*)ucRxbuff,14,16);
		node->hour = getIntRxBuffDataByLocation((char*)ucRxbuff,16,18);
		node->minute = getIntRxBuffDataByLocation((char*)ucRxbuff,18,20);
		node->second = getIntRxBuffDataByLocation((char*)ucRxbuff,20,22);
		//判断数据是否合理 不合理直接返回打印错误信息
		if(checkData(node->ucType,node->year,node->month,node->day,node->hour,node->minute,node->second)==0 || pcarMessage->uiIdleCount-1<0)
			goto MYERROR;
		//数据无误  添加车辆信息到存储链表中
		else
			addListNode(pcarMessage,node);	
	}	
	
	//清除本次串口接收到的数据 避免影响后续数据
	memset(ucRxbuff,0,sizeof(ucRxbuff));
	//处理完本次串口接收到的数据后清除标志位
	iRxFlag = 0;
	return ;
	
	//接收数据出现问题时 发送错误信息到PC
	MYERROR:	
		HAL_UART_Transmit(&huart1,(uint8_t*)"Error\r\n",sizeof("Error\r\n"),50);
		//清除本次串口接收到的数据 避免影响后续数据
		memset(ucRxbuff,0,sizeof(ucRxbuff));
		//处理完本次串口接收到的数据后清除标志位
		iRxFlag = 0;
}

#endif



