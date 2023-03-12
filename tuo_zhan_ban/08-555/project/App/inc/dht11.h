#ifndef __DHT11_H
#define __DHT11_H

#include "main.h"

#define	DHT11_PIN_PORT       GPIOA
#define	DHT11_PIN        GPIO_PIN_7
#define	DHT11_PIN_CLOCK  __HAL_RCC_GPIOA_CLK_ENABLE()
#define	DHT11_PIN_OUT_H   HAL_GPIO_WritePin(DHT11_PIN_PORT, DHT11_PIN, GPIO_PIN_SET)
#define	DHT11_PIN_OUT_L   HAL_GPIO_WritePin(DHT11_PIN_PORT, DHT11_PIN, GPIO_PIN_RESET)
#define	DHT11_PIN_IN      HAL_GPIO_ReadPin(DHT11_PIN_PORT, DHT11_PIN)


uint8_t DHT11_Read_Byte(void);//读出一个字节
uint8_t DHT11_Read_Bit(void);//读出一个位
uint8_t DHT11_Check(void);//检测是否存在DHT11
void DHT11_Rst(void);//复位DHT11
uint8_t DHT11_Init(void);//初始化DHT11
uint8_t DHT11_Read_Data(uint8_t *data); //读取温湿度

#endif

