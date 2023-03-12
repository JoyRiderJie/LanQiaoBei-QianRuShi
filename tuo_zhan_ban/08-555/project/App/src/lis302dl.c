#include "lis302dl.h"

/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    lis302dlWrite
  * @brief   写入数据到LIS302DL中
  * @param   reg: [输入] 
  *			 info: [输入] 
  * @retval
  * @author  黑心萝卜三条杠
  * @Data    2023-03-10
  * 1. ...
  * <modify staff>:
  * <data>        :
  * <description> :
  * 2. ...
 **/
/* -------------------------------- end -------------------------------- */

static void lis302dlWrite(unsigned char reg, unsigned char info)
{
    I2CStart();
    I2CSendByte(0x38);
    I2CWaitAck();
    I2CSendByte(reg);
    I2CWaitAck();
    I2CSendByte(info);
    I2CWaitAck();
    I2CStop();
}

/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    lis302dlConfig
  * @brief   配置LIS302DL
  * @param   None
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
static void lis302dlConfig(void)
{
    //Power up (100Hz data rate)
    lis302dlWrite(CTRL_REG1, 0x47);
    //HP filter bypassed
    lis302dlWrite(CTRL_REG2, 0x00);
    //FF_WU 1 interrupt send to INT Pad1(open drain) active low
    lis302dlWrite(CTRL_REG3, 0xC1);
    //threshold :0.5 g
    lis302dlWrite(FF_WU_THS_1, 0x28);
    //filter :200ms @100Hz
    lis302dlWrite(FF_WU_DURATION_1, 40);
    //ZLIE event
    lis302dlWrite(FF_WU_CFG_1, 0x10);
}

/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    lis302dlRead
  * @brief   读出LIS302DL中的数据
  * @param   address: [输入] 
  * @retval  val： 读取出的数据
  * @author  黑心萝卜三条杠
  * @Data    2023-03-10
  * 1. ...
  * <modify staff>:
  * <data>        :
  * <description> :
  * 2. ...
 **/
/* -------------------------------- end -------------------------------- */
static uint8_t lis302dlRead(uint8_t address)
{
    unsigned char val;
    I2CStart();
    I2CSendByte(0x38);
    I2CWaitAck();

    I2CSendByte(address);
    I2CWaitAck();

    I2CStart();
    I2CSendByte(0x39);
    I2CWaitAck();
    val = I2CReceiveByte();
    I2CSendNotAck();
    I2CStop();

    return(val);
}

/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    lis302dlCheck
  * @brief   检查lis302dl是否能够正常工作
  * @param   None
  * @retval
  * @author  黑心萝卜三条杠
  * @Data    2023-03-10
  * 1. ...
  * <modify staff>:
  * <data>        :
  * <description> :
  * 2. ...
 **/
/* -------------------------------- end -------------------------------- */
static uint8_t lis302dlCheck(void)
{
    if(lis302dlRead(0x0f))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

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
uint8_t lis302dlInit(void)
{
	// 如果有IIC的从初始化 此处可以注释
//	i2c_init();
    lis302dlConfig();
    return lis302dlCheck();
}

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
void lis302dlOutput(float*alz)
{
    if((lis302dlRead(0x27) & 0x08) != 0)
    {
        alz[0] = (lis302dlRead(0x29)) * 18.0 / 1000;  //x
        alz[1] = (lis302dlRead(0x2B)) * 18.0 / 1000;  //y
        alz[2] = (lis302dlRead(0x2D)) * 18.0 / 1000;  //z
    }
}





