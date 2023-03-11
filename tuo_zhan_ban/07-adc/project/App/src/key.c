/**
  ************************************* Copyright ******************************
  *
  *                 (C) Copyright 2023,�����ܲ�������,China, GCU.
  *                            All Rights Reserved
  *
  *
  * FileName   : key.c
  * Version    : v1.0
  * Author     : �����ܲ�������
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
  * @brief   ��ȡADC��ֵ
  * @param   None
  * @retval
  * @author  �����ܲ�������
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
    // ����ADC2
    HAL_ADC_Start(&hadc2);
    // ����ADC��ֵ
    return HAL_ADC_GetValue(&hadc2);
}

/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    getADC
  * @brief   Ϊ��ADC���þ�׼������ö�ζ�ȡADC��ֵ
  * @param   None
  * @retval  ����ADC����ֵ
  * @author  �����ܲ�������
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

    // Ϊ�˸��Ӿ�׼�ػ��ADC��ֵ ������ö�ζ�ȡ��ƽ��ֵ�ķ�ʽ
    for(i = 0; i < BTN_BUFF_LEN; ++i)
        tmp += getADC();
    // ������50��ADC���õ�ƽ��ֵ
    return tmp/BTN_BUFF_LEN;

#elif N == 1
    // �������50�β�����ֵ
    u16 btn_buff[BTN_BUFF_LEN];
    u16 tmp = 0;
    u8 i = 0, j = 0;
    // �������50��ADC��ֵ
    for(i = 0; i < BTN_BUFF_LEN; ++i)
        btn_buff[i] = getADC();

    // ð������  ��Ϊֻ��Ҫȡ����λ�� �������ֻ��Ҫ����BTN_BUFF_LEN / 2��
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

    // ������λ��
    if(BTN_BUFF_LEN % 2 == 0)
        return(btn_buff[BTN_BUFF_LEN / 2 - 1] + btn_buff[BTN_BUFF_LEN / 2]) / 2;
    else
        return(btn_buff[BTN_BUFF_LEN / 2]);
#endif
}

/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    getADC
  * @brief   �ж��Ǹ���������
  * @param   None
  * @retval	 ������ֵ
  * @author  �����ܲ�������
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

    // ��ȡ���50��ADC���õ�ƽ��ֵ��������λ��
    btn_tmp = readBtn();

    /* �ж����ֵ�����Ǹ���������� Ҳ������Щ������ͨ����ܻ�������ֵ  */
    // ����1
    if(btn_tmp < 0x0FFF / 14 * 1)
        return 1;
    // ����2
    else if((btn_tmp > 0x0FFF / 14 * 1) && (btn_tmp < 0x0FFF / 14 * 3))
        return 2;
    // ����2
    else if((btn_tmp > 0x0FFF / 14 * 3) && (btn_tmp < 0x0FFF / 14 * 5))
        return 3;
    // ����4
    else if((btn_tmp > 0x0FFF / 14 * 5) && (btn_tmp < 0x0FFF / 14 * 7))
        return 4;
    // ����5
    else if((btn_tmp > 0x0FFF / 14 * 7) && (btn_tmp < 0x0FFF / 14 * 9))
        return 5;
    // ����6
    else if((btn_tmp > 0x0FFF / 14 * 9) && (btn_tmp < 0x0FFF / 14 * 11))
        return 6;
    // ����7
    else if((btn_tmp > 0x0FFF / 14 * 11) && (btn_tmp < 0x0FFF / 14 * 13))
        return 7;
    // ����8
    else if((btn_tmp > 0x0FFF / 14 * 13) && (btn_tmp < 0x0F6F))
        return 8;
    // �Ų�����ֵ�ĸ��� ���� �ǰ���û�а���
    else
        return 0;
}
