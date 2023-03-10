#ifndef __DHT11_H
#define __DHT11_H

#include "main.h"

#define	DHT11_PIN_PORT       GPIOA
#define	DHT11_PIN        GPIO_PIN_7
#define	DHT11_PIN_CLOCK  __HAL_RCC_GPIOA_CLK_ENABLE()
#define	DHT11_PIN_OUT_H   HAL_GPIO_WritePin(DHT11_PIN_PORT, DHT11_PIN, GPIO_PIN_SET)
#define	DHT11_PIN_OUT_L   HAL_GPIO_WritePin(DHT11_PIN_PORT, DHT11_PIN, GPIO_PIN_RESET)
#define	DHT11_PIN_IN      HAL_GPIO_ReadPin(DHT11_PIN_PORT, DHT11_PIN)


uint8_t DHT11_Read_Byte(void);//����һ���ֽ�
uint8_t DHT11_Read_Bit(void);//����һ��λ
uint8_t DHT11_Check(void);//����Ƿ����DHT11
void DHT11_Rst(void);//��λDHT11
uint8_t DHT11_Init(void);//��ʼ��DHT11
uint8_t DHT11_Read_Data(uint8_t *data); //��ȡ��ʪ��

#endif

