#ifndef _LIS302DL_
#define _LIS302DL_

#include "main.h"
#include "i2c.h"

#define CTRL_REG1 			0x20
#define CTRL_REG2 			0x21
#define CTRL_REG3 			0x22
#define FF_WU_THS_1 		0x32
#define FF_WU_DURATION_1 	0x33
#define FF_WU_CFG_1 		0x30
#define STATUS_REG 			0x27

/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    lis302dlInit
  * @brief   初始化lis392dl
  * @param   无
  * @retval  初始化是否成功
  * @author  黑心萝卜三条杠
  * @Data    2023-03-10
  * 1. ...
  * <modify staff>:
  * <data>        :
  * <description> :
  * 2. ...
 **/
/* -------------------------------- end -------------------------------- */
uint8_t lis302dlInit(void);

/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    lis302dlOutput
  * @brief   读取lis392dl的加速度
  * @param   int8_t*alz: [输出]
  * @retval  None
  * @author  黑心萝卜三条杠
  * @Data    2023-03-10
  * 1. ...
  * <modify staff>:
  * <data>        :
  * <description> :
  * 2. ...
 **/
/* -------------------------------- end -------------------------------- */
void lis302dlOutput(float*alz);

#endif
