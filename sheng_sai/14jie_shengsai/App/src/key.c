#include "key.h"
/*********************************
 * 函数功能：按键的初始化函数
 * 函数参数：无
 * 函数返回值：无
*********************************/
void keyInit(void)
{
    GPIO_InitTypeDef GPIO_initStructure;

/*********按键B4(PA0)初始化*******************/
#if KEYPA
    __HAL_RCC_GPIOA_CLK_ENABLE();
    //浮空输入模式
    GPIO_initStructure.Mode = GPIO_MODE_IT_FALLING;
    //设置成低频
    GPIO_initStructure.Speed = GPIO_SPEED_FREQ_LOW;
    //GPIO引脚设置
    GPIO_initStructure.Pin = GPIO_PIN_0;
    //引脚的模式设置
    GPIO_initStructure.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA,&GPIO_initStructure);
#endif

/*********按键B4初始化*******************/
#if KEYPB
    __HAL_RCC_GPIOB_CLK_ENABLE();
        //浮空输入模式
    GPIO_initStructure.Mode = GPIO_MODE_IT_FALLING;
    //设置成低频
    GPIO_initStructure.Speed = GPIO_SPEED_FREQ_LOW;
    //GPIO引脚设置
    GPIO_initStructure.Pin = KEYGPIOB;
    //引脚的模式设置
    GPIO_initStructure.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB,&GPIO_initStructure);
#endif
}

/*********************************************
 * 函数功能：按键扫描 含按键消抖 无长按短按设计
 * 函数参数：无
 * 函数返回值：按键的位置
 *            返回值说明：B1-1 B2-2 B3-3 B4-4
*********************************************/
unsigned char scanKey(void)
{
	//按键锁
	static unsigned char keyLock = 1;
    //记录按键消抖时间
    // static uint16_t keyCount = 0;

	//按键按下
    if((HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0) == RESET || HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1) == RESET
      || HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2) == RESET || HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0) == RESET) 
      && keyLock == 1){
        //给按键上锁 避免多次触发按键
        keyLock = 0;
        
        //按键消抖 这里最好不要使用延时函数进行消抖 会影响系统的实时性
        // if(++keyCount % 10 < 5) return 0;
        // if(keyTime()%15 < 10) return 0;
        HAL_Delay(10);

        //按键B1
        if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0) == RESET){
            return 1;
        }
        //按键B2
        if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1) == RESET){
            return 2;
        }
        //按键B3
        if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2) == RESET){
            return 3;
        }
        //按键B4
        if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0) == RESET){
            return 4;
        }
    }
    //按键松开
    if((HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0) == SET && HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1) == SET
      && HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2) == SET && HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0) == SET) 
      && keyLock == 0){
        //开锁
        keyLock = 1;
    }
    return 0;
}

/*********************************************
 * 函数功能：按键扫描 用于按键的{长按-短按} {按下-松开}设计
 * 函数参数：无
 * 函数返回值：无
*********************************************/
unsigned char scanKeyNoTime(void)
{
    //按键B1
    if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0) == RESET){
        return 1;
    }
    //按键B2
    if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1) == RESET){
        return 2;
    }
    //按键B3
    if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2) == RESET){
        return 3;
    }
    //按键B4
    if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0) == RESET){
        return 4;
    }
    return 0;
}

/******************************************************
 * 函数功能：按键扫描 用于{单击-双击}按键 该函数只是一个示例 而且还存在问题
 * 函数参数：无
 * 函数返回值：按键的键值
******************************************************/
unsigned char doubleScankey(void)
{
	//保存第一次按键按下的键值
	uint16_t keyValue[3] ;
	keyValue[0] = scanKey(); // scanKey
	HAL_Delay(700);
	keyValue[1] = scanKey(); // scanKeyNoTime
	if(keyValue[0] == keyValue[1])
		return keyValue[1];
    return 0;
}

//定义一个按键结构体数组变量 其中包含按键是否按下  是否长按  是否双击
struct keys key[4] = {0,0,0,0};

/****************************************************************************************************
 * 函数功能：按键扫描函数 注意此函数放在定时器中断（10ms）中的使用效果最佳 否则双击与长按会出现问题
 * 函数参数：无
 * 函数返回值：无 
*****************************************************************************************************/
void scanKeyUseStructAndTime(void)
{
	static struct keyState _key[4];
	
	//获取按键的最新状态
	_key[0].keyState = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0);
	_key[1].keyState = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1);
	_key[2].keyState = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2);
	_key[3].keyState = HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0);
	
	//处理按键的最新状态
	for(int i=0;i<4;i++){
		switch (_key[i].judgeSate){
			//按键第一次按下
			case 0:
				if(_key[i].keyState == 0){
					//跳转按键的状态
					_key[i].judgeSate=1;
					//清空按键时间
					_key[i].keyTime=0;
				}
				break;
			//按键第二次按下 两次相隔10ms可以起到消抖作用	
			case 1:
				//按键再次按下 跳转按键状态
				if(_key[i].keyState == 0)
					_key[i].judgeSate=2;
				//上一次按键按下是抖动按下 属于无效状态 应该退回最开始的状态
				else 
					_key[i].judgeSate=0;
				break;
			//确定按键按下后的处理过程
			case 2:
				//等待松开过程,且非长按键
				if((_key[i].keyState==1) && _key[i].keyTime<50){	
					//可能双击按键的第一次，进入计时
					if(_key[i].doubleClickTimerFlag == 0) {
						_key[i].doubleClickTimerFlag = 1;
						_key[i].doubleClickTime = 0;
					}
					//在计时范围内又按了一次
					else{
						key[i].doubleFlag=1;//双击情况
						_key[i].doubleClickTimerFlag = 0;
					}
					_key[i].judgeSate = 0;
				}
				//松开且是长按键
				else if(_key[i].keyState==1 && _key[i].keyTime>=70)
					_key[i].judgeSate = 0;
				//按下 且为长按键
				else {
					//长按键
					if (_key[i].keyTime >=50)
						key[i].longFlag = 1;
					//长按键计时 还没松开
					_key[i].keyTime++;
				}
				break;
		}
		//按键单次按下
		if(_key[i].doubleClickTimerFlag == 1 && _key[i].doubleClickTime >= 25) {
			key[i].flag = 1;
			_key[i].doubleClickTimerFlag = 0;
		}
		//按键双击 双击计时
		else if(_key[i].doubleClickTimerFlag == 1){
			_key[i].doubleClickTime++;
		}
	}
}
		
/**** 留给按键外部调用的值  ****/
// 用于判断按键是否处于按下状态 非0-表示按下 0-表示松开 （keyOldState取值有：1-表示B1 2-表示B2 4-表示B3 8-表示B4）
uint8_t keyOldState = 0;
// 用于判断按键是否按下过 非0-表示按下 0-表示松开
uint8_t keyFalling = 0;
// 用于判断按键是否松开 非0-表示按下 0-表示松开
uint8_t keyRising = 0;

/**************************************************************************
* 函数功能：通过逻辑运算获取按键的值  （这玩意可以设计多按键控制某个功能）
* 函数参数：无
* 函数返回值：无
***************************************************************************/
void keyRefresh(void)
{
//	if(keyTime() % 5 ) return ;
	// 获取按键状态
	uint16_t state = getKeysState();
	// 对按键的值进行异或处理 一次判断
	uint8_t key_temp = 0xFF ^ (0xF0 | state);
	
	/*** 通过逻辑运算处理消抖 **/
	// 按下状态
	keyFalling = key_temp & (key_temp ^ keyOldState);
	// 松开状态
	keyRising = ~key_temp & (key_temp ^ keyOldState);
	// 保存本次按键的值
	keyOldState = key_temp;
}

/****************************************************************************
* 函数功能：判断按键是否长按  注意这里是一个通用的判断
* 函数参数：无
* 函数返回值：
*            -1-表示按键短按或无效
*		     其他-表示按键长按时的按键值
****************************************************************************/
signed char scanKeyForTime(void)
{
	// 保存按键按下时的时间
	static uint32_t myTime = 0;

#if 0
	// 按键扫描 如果已经存在了按键扫描这里的调用可以注释
	keyRefresh();
#endif	
	/*** 长按操作 *****/
	// 按键按下
	if(keyFalling )
		myTime = keyTime();
	
	// 使用延时来计算按键按下的时间
//	while(!keyRising && keyOldState) keyRefresh();
	
	// 按键长按
	if(keyRising &&  keyTime() - myTime > LONGTIME)
	{
		// 清楚有效的松开按键值
		return keyRising;	
	}
	return -1;
}

/****************************************************************************
* 函数功能：判断按键是否双击  注意这里是一个通用的判断  (这里还有点问题，双击第一次会干扰到单击)
* 函数参数：无
* 函数返回值：
*            -1-表示按键单击或无效
*		     其他-表示按键双击时的按键值
****************************************************************************/
signed char scanKeyForDouble(void)
{
	static uint32_t myTime = 0;	
	int32_t t1 = 0;
	// 保存第一次按键按下的值
	static uint8_t _keytemp = 0,keyCount = 0;
	
#if 0	
	// 按键扫描 如果已经存在了按键扫描这里的调用可以注释
	keyRefresh();
#endif	
	
	// 按键按下的逻辑
	if(keyFalling)
	{	
		// 第一次按下 应该记录时间
		if(keyCount == 0)
		{
			myTime = keyTime();
			_keytemp = keyFalling;
		}
		keyCount++;
	}
	t1 = keyTime() - myTime - DOUBLETIME;
	
	// 判断按键时间是否超时
	if( _keytemp && t1 < 0)
	{
		// 按键松开的逻辑
		if(keyOldState == _keytemp &&  keyCount >= 2)
		{
				keyCount = 0;
				return keyOldState;
		}
	}
	else
	{
		keyCount = 0;
		return -2;
	}
	return -1;
}

// 使用状态机实现按键的单击 双击 长按
unsigned char scanKeyByState(void)
{
	static char KeyState = 0;
	
	// 获取按键状态
	uint16_t state = getKeysState();
	// 对按键的值进行异或处理 一次判断
	uint8_t key_temp = 0xFF ^ (0xF0 | state);

	/*** 通过逻辑运算处理消抖 **/
	// 按下状态
	keyFalling = key_temp & (key_temp ^ keyOldState);
	// 松开状态
	keyRising = ~key_temp & (key_temp ^ keyOldState);
	// 保存本次按键的值
	keyOldState = key_temp;
	
	switch(KeyState)
	{
		case 1:
			if(keyOldState)
				KeyState = 2;
			break;
		case 2:
			if(!keyOldState) 
			{
				KeyState  = 1;
				return 0;
			}
			else
			{
				KeyState = 3 ;
			}
			break;
		case 3:
			if(keyRising && !keyOldState) 
				return keyRising;
			else if(!keyRising && keyOldState)
				KeyState = 2;
			else
				KeyState = 0;
			break;
		default:
			break;
	}
	return 0;
}


