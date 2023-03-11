/**
  ************************************* Copyright ******************************
  *
  *                 (C) Copyright 2023,黑心萝卜三条杠,China, GCU.
  *                            All Rights Reserved
  *
  *
  * FileName   : key.c
  * Version    : v1.0
  * Author     : 黑心萝卜三条杠
  * Date       : 2023-03-06
  * Description:
  * Function List:
  	1. ....
  	   <version>:
  <modify staff>:
  		  <data>:
   <description>:
  	2. ...
  ******************************************************************************
 */
#include "key.h"

#define u8 unsigned char
#define u16 unsigned int

/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    getADC
  * @brief   获取ADC的值
  * @param   None
  * @retval
  * @author  黑心萝卜三条杠
  * @Data    2023-03-06
  * 1. ...
  * <modify staff>:
  * <data>        :
  * <description> :
  * 2. ...
 **/
/* -------------------------------- end -------------------------------- */
static uint16_t getADC(void)
{
    // 开启ADC2
    HAL_ADC_Start(&hadc2);
    // 返回ADC的值
    return HAL_ADC_GetValue(&hadc2);
}

/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    getADC
  * @brief   为了ADC采用精准这里采用多次读取ADC的值
  * @param   None
  * @retval  这轮ADC采样值
  * @author  黑心萝卜三条杠
  * @Data    2023-03-06
  * 1. ...
  * <modify staff>:
  * <data>        :
  * <description> :
  * 2. ...
 **/
/* -------------------------------- end -------------------------------- */
static u16 readBtn(void)
{
#define N 1

#if N == 0
    u16 tmp = 0, i = 0;

    // 为了更加精准地获得ADC的值 这里采用多次读取求平均值的方式
    for(i = 0; i < BTN_BUFF_LEN; ++i)
        tmp += getADC();
    // 返回这50次ADC采用的平均值
    return tmp/BTN_BUFF_LEN;

#elif N == 1
    // 保存最近50次采样的值
    u16 btn_buff[BTN_BUFF_LEN];
    u16 tmp = 0;
    u8 i = 0, j = 0;
    // 保存最近50次ADC的值
    for(i = 0; i < BTN_BUFF_LEN; ++i)
        btn_buff[i] = getADC();

    // 冒泡排序  因为只需要取出中位数 因此这里只需要排列BTN_BUFF_LEN / 2次
    for(i = 0; i <= BTN_BUFF_LEN / 2; ++i)
    {
        for(j = 0; j <	BTN_BUFF_LEN - i - 1; ++j)
        {
            if(btn_buff[j + 1] < btn_buff[j])
            {
                tmp = btn_buff[j + 1];
                btn_buff[j + 1] = btn_buff[j];
                btn_buff[j] = tmp;
            }
        }
    }

    // 返回中位数
    if(BTN_BUFF_LEN % 2 == 0)
        return(btn_buff[BTN_BUFF_LEN / 2 - 1] + btn_buff[BTN_BUFF_LEN / 2]) / 2;
    else
        return(btn_buff[BTN_BUFF_LEN / 2]);
#endif
}

/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    getADC
  * @brief   判断那个按键按下
  * @param   None
  * @retval	 按键的值
  * @author  黑心萝卜三条杠
  * @Data    2023-03-06
  * 1. ...
  * <modify staff>:
  * <data>        :
  * <description> :
  * 2. ...
 **/
/* -------------------------------- end -------------------------------- */
u8 scanBtn(void)
{
    u16 btn_tmp = 0;

    // 获取最近50次ADC采用的平均值或者是中位数
    btn_tmp = readBtn();

    /* 判断这个值是由那个电阻引起的 也就是哪些电阻连通后可能会产生这个值  */
    // 按键1
    if(btn_tmp < 0x0FFF / 14 * 1)
        return 1;
    // 按键2
    else if((btn_tmp > 0x0FFF / 14 * 1) && (btn_tmp < 0x0FFF / 14 * 3))
        return 2;
    // 按键2
    else if((btn_tmp > 0x0FFF / 14 * 3) && (btn_tmp < 0x0FFF / 14 * 5))
        return 3;
    // 按键4
    else if((btn_tmp > 0x0FFF / 14 * 5) && (btn_tmp < 0x0FFF / 14 * 7))
        return 4;
    // 按键5
    else if((btn_tmp > 0x0FFF / 14 * 7) && (btn_tmp < 0x0FFF / 14 * 9))
        return 5;
    // 按键6
    else if((btn_tmp > 0x0FFF / 14 * 9) && (btn_tmp < 0x0FFF / 14 * 11))
        return 6;
    // 按键7
    else if((btn_tmp > 0x0FFF / 14 * 11) && (btn_tmp < 0x0FFF / 14 * 13))
        return 7;
    // 按键8
    else if((btn_tmp > 0x0FFF / 14 * 13) && (btn_tmp < 0x0F6F))
        return 8;
    // 排查其他值的干扰 或者 是按键没有按下
    else
        return 0;
}
