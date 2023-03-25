#ifndef _R47_H
#define _R47_H

#include "main.h"

#define DO_GPIO_Port GPIOA
#define DO_Pin       GPIO_PIN_3

// 获取光敏电阻的阻值
float getR47ADC(void);

#endif
