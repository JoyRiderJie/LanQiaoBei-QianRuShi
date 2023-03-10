/**
  ************************************* Copyright ******************************   
  *                 (C) Copyright 2023,黑心萝卜三条杠,China, GCU.
  *                            All Rights Reserved
  *                              
  *      
  * FileName   : seg.h   
  * Version    : v1.0		
  * Author     : 黑心萝卜三条杠			
  * Date       : 2023-03-01         
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

 #ifndef __SEG_H_
#define __SEG_H_

#include "main.h"

// 声明GPIO分组及引脚
#define RCLK_PIN    GPIO_PIN_2
#define RCLK_PORT   GPIOA
#define SER_PIN   	GPIO_PIN_1
#define SER_PORT    GPIOA
#define SCK_PIN   	GPIO_PIN_3
#define SCK_PORT    GPIOA

// 声明函数别名
#define RCLK_H				HAL_GPIO_WritePin(RCLK_PORT, RCLK_PIN, GPIO_PIN_SET)
#define RCLK_L				HAL_GPIO_WritePin(RCLK_PORT, RCLK_PIN, GPIO_PIN_RESET)

#define SER_H				HAL_GPIO_WritePin(SER_PORT, SER_PIN, GPIO_PIN_SET)
#define SER_L				HAL_GPIO_WritePin(SER_PORT, SER_PIN, GPIO_PIN_RESET)

#define SCK_H				HAL_GPIO_WritePin(SCK_PORT, SCK_PIN, GPIO_PIN_SET)
#define SCK_L				HAL_GPIO_WritePin(SCK_PORT, SCK_PIN, GPIO_PIN_RESET)

// 给其他函数提供数码管显示值的数组  segbuff[0]是seg1 segbuff[1]是seg2 segbuff[2]是seg3
extern unsigned char segBuff[3];

// 数码管刷新函数
void segDisplay(void);

#endif


 

