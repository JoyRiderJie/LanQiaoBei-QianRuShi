#ifndef _MYADC_H
#define _MYADC_H

#include "main.h"

/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    getADC_RPx
  * @brief   ��ȡ��Ӧͨ����ADC��ģ���ѹֵ
  * @param   hadc: [����]  ָ��ADC����
**			 channel: [����] ָ������ADC��ͨ�� 
  * @retval  float: ����ָ��ͨ����ADCֵ
  * @author  �����ܲ�������
  * @Data    2023-03-12
  * 1. ...
  * <modify staff>:
  * <data>        :
  * <description> :
  * 2. ...
 **/
/* -------------------------------- end -------------------------------- */
float getADC_RPx(ADC_HandleTypeDef *hadc,uint32_t channel);

#endif
