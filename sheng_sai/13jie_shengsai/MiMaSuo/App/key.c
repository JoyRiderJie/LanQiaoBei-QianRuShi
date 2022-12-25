#include "key.h"
#include "usart.h"

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
        // if(HAL_GetTick()%15 < 10) return 0;
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
    unsigned char count = 3;
    
    while(count){
        //保存第一次按键按下的键值
        uint16_t keyValue = scanKeyNoTime();
        if(count == 1){
            return keyValue;
        }
        //处理第一次按键按下
        if(0 < keyValue && keyValue < 4){
            HAL_GPIO_WritePin(GPIOB,(GPIO_PIN_0 << (keyValue-1)),GPIO_PIN_SET);
        }
        else if(keyValue == 4){
            HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_SET);
        }
        HAL_Delay(10);
        count--;
    }

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


