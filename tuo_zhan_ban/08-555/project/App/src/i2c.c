#include "i2c.h"

#define DELAY_TIME	500

/** I2C 总线接口 */
#define I2C_PORT GPIOA
#define SDA_Pin	GPIO_PIN_5
#define SCL_Pin GPIO_PIN_4

#define FAILURE 0
#define SUCCESS 1

//配置SDA信号线为输入模式
void SDA_Input_Mode(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_InitStruct.Pin =  SDA_Pin;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;

    HAL_GPIO_Init(I2C_PORT, &GPIO_InitStruct);
}

//配置SDA信号线为输出模式
void SDA_Output_Mode(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_InitStruct.Pin = SDA_Pin;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD ;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(I2C_PORT, &GPIO_InitStruct);
}

//
void SDA_Output( uint16_t val )
{
    if ( val )
    {

        HAL_GPIO_WritePin(I2C_PORT, SDA_Pin, GPIO_PIN_SET);
    }
    else
    {

        HAL_GPIO_WritePin(I2C_PORT, SDA_Pin, GPIO_PIN_RESET);
    }
}

//
void SCL_Output( uint16_t val )
{
    if ( val )
    {

        HAL_GPIO_WritePin(I2C_PORT, SCL_Pin, GPIO_PIN_SET);
    }
    else
    {

        HAL_GPIO_WritePin(I2C_PORT, SCL_Pin, GPIO_PIN_RESET);
    }
}

//
uint8_t SDA_Input()
{
    return HAL_GPIO_ReadPin( I2C_PORT, SDA_Pin);
}

//延时程序
void delay1(unsigned int n)
{
    unsigned int i;
    for ( i = 0; i < n; ++i);
}

//I2C总线启动
void I2CStart(void)
{
    SDA_Output(1);
    delay1(DELAY_TIME);
    SCL_Output(1);
    delay1(DELAY_TIME);
    SDA_Output(0);
    delay1(DELAY_TIME);
    SCL_Output(0);
    delay1(DELAY_TIME);
}

//I2C总线停止
void I2CStop(void)
{
    SCL_Output(0);
    delay1(DELAY_TIME);
    SDA_Output(0);
    delay1(DELAY_TIME);
    SCL_Output(1);
    delay1(DELAY_TIME);
    SDA_Output(1);
    delay1(DELAY_TIME);

}

//等待应答
unsigned char I2CWaitAck(void)
{
    unsigned short cErrTime = 5;
    SDA_Input_Mode();
    delay1(DELAY_TIME);
    SCL_Output(1);
    delay1(DELAY_TIME);
    while(SDA_Input())
    {
        cErrTime--;
        delay1(DELAY_TIME);
        if (0 == cErrTime)
        {
            SDA_Output_Mode();
            I2CStop();
            return FAILURE;
        }
    }
    SDA_Output_Mode();
    SCL_Output(0);
    delay1(DELAY_TIME);
    return SUCCESS;
}

//发送应答位
void I2CSendAck(void)
{
    SDA_Output(0);
    delay1(DELAY_TIME);
    delay1(DELAY_TIME);
    SCL_Output(1);
    delay1(DELAY_TIME);
    SCL_Output(0);
    delay1(DELAY_TIME);

}

//
void I2CSendNotAck(void)
{
    SDA_Output(1);
    delay1(DELAY_TIME);
    SCL_Output(1);
    delay1(DELAY_TIME);
    SCL_Output(0);
    delay1(DELAY_TIME);

}

//通过I2C总线发送一个字节数据
void I2CSendByte(unsigned char cSendByte)
{
    unsigned char  i = 8;
    while (i--)
    {
        SCL_Output(0);
        delay1(DELAY_TIME);
        SDA_Output(cSendByte & 0x80);
        delay1(DELAY_TIME);
        cSendByte += cSendByte;
        delay1(DELAY_TIME);
        SCL_Output(1);
        delay1(DELAY_TIME);
    }
    SCL_Output(0);
    delay1(DELAY_TIME);
}

//从I2C总线接收一个字节数据
unsigned char I2CReceiveByte(void)
{
    unsigned char i = 8;
    unsigned char cR_Byte = 0;
    SDA_Input_Mode();
    while (i--)
    {
        cR_Byte += cR_Byte;
        SCL_Output(0);
        delay1(DELAY_TIME);
        delay1(DELAY_TIME);
        SCL_Output(1);
        delay1(DELAY_TIME);
        cR_Byte |=  SDA_Input();
    }
    SCL_Output(0);
    delay1(DELAY_TIME);
    SDA_Output_Mode();
    return cR_Byte;
}

//I2C总线初始化
void i2c_init()
{
    GPIO_InitTypeDef GPIO_InitStruct;

    __HAL_RCC_GPIOA_CLK_ENABLE();
    GPIO_InitStruct.Pin = SDA_Pin | SCL_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

    HAL_GPIO_Init(I2C_PORT, &GPIO_InitStruct);
}

