#include "R47.h"

// 外部声明的ADC2
extern ADC_HandleTypeDef hadc2;
/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    getADC
  * @brief   获取光敏电阻的阻值
  * @param   None
  * @retval  float:转换后的值
  * @author  黑心萝卜三条杠
  * @Data    2023-03-10
  * 1. ...
  * <modify staff>:
  * <data>        :
  * <description> :
  * 2. ...
 **/
/* -------------------------------- end -------------------------------- */
float getR47ADC(void)
{
	uint16_t temp = 0;
	
    HAL_ADC_Start(&hadc2);
    temp = HAL_ADC_GetValue(&hadc2);
	
	return temp/(4096 - temp) * 10;
}

