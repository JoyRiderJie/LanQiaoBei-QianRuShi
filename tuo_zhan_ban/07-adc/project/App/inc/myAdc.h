#ifndef _MYADC_H
#define _MYADC_H

#include "main.h"

/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    getADC_RPx
  * @brief   获取对应通道的ADC的模拟电压值
  * @param   hadc: [输入]  指定ADC分组
**			 channel: [输入] 指定分组ADC的通道 
  * @retval  float: 返回指定通道的ADC值
  * @author  黑心萝卜三条杠
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
