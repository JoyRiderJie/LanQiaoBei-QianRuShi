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
    //按键按下
    if((HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0) == RESET || HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1) == RESET
      || HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2) == RESET || HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0) == RESET) 
      && keyLock == 1){
        //给按键上锁 避免多次触发按键
        keyLock = 0;
        //while(uTick_ms%10 < 10) ;
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
    else if((HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0) == SET && HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1) == SET
      && HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2) == SET && HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0) == SET) 
      && keyLock == 0){
        //开锁
        keyLock = 1;
    }
    return 0;
}

/******************************************************************************
 * 函数功能：判断按键是长按还是短按
 * 函数参数：无
 * 函数返回值：按键的值以及长按（长按在原本的数值上加10） 还是短按（不做任何处理）
******************************************************************************/
char getKeyNumberByTime(void)
{

    return 0;
}

