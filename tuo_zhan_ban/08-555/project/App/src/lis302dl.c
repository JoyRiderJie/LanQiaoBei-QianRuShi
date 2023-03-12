#include "lis302dl.h"

/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    lis302dlWrite
  * @brief   д�����ݵ�LIS302DL��
  * @param   reg: [����] 
  *			 info: [����] 
  * @retval
  * @author  �����ܲ�������
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
  * @brief   ����LIS302DL
  * @param   None
  * @retval  None
  * @author  �����ܲ�������
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
  * @brief   ����LIS302DL�е�����
  * @param   address: [����] 
  * @retval  val�� ��ȡ��������
  * @author  �����ܲ�������
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
  * @brief   ���lis302dl�Ƿ��ܹ���������
  * @param   None
  * @retval
  * @author  �����ܲ�������
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
  * @brief   ��ʼ��lis392dl
  * @param   ��
  * @retval  ��ʼ���Ƿ�ɹ�
  * @author  �����ܲ�������
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
	// �����IIC�Ĵӳ�ʼ�� �˴�����ע��
//	i2c_init();
    lis302dlConfig();
    return lis302dlCheck();
}

/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    lis302dlOutput
  * @brief   ��ȡlis392dl�ļ��ٶ�
  * @param   int8_t*alz: [���]
  * @retval  None
  * @author  �����ܲ�������
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





