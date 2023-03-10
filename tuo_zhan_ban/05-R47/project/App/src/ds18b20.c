#include "ds18b20.h"

#define delay_us(X)  delay((X)*80/5)

// 非准确延时
static void delay(unsigned int n)
{
    while(n--);
}

// ds18b20的初始化
void ds18b20_init_x(void)
{
    GPIO_InitTypeDef	GPIO_InitStruct = {0};

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOA_CLK_ENABLE();


    GPIO_InitStruct.Pin = OW_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH ;
    GPIO_InitStruct.Pull = GPIO_PULLUP ;
    HAL_GPIO_Init(OW_PIN_PORT, &GPIO_InitStruct);
}

// 这也是初始化 用于输入模式
static void mode_input1(void )
{

    GPIO_InitTypeDef	GPIO_InitStruct = {0};

    /* GPIO Ports Clock Enable */

    GPIO_InitStruct.Pin = OW_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(OW_PIN_PORT, &GPIO_InitStruct);
}

// 这也是初始化 用于输出模式
static void mode_output1(void )
{

    GPIO_InitTypeDef	GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = OW_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Pull = GPIO_NOPULL ;
    HAL_GPIO_Init(OW_PIN_PORT, &GPIO_InitStruct);
}

// ds18b20初始化时序（或者说是复位时序）
static uint8_t ow_reset(void)
{
    uint8_t err;

    OW_DIR_OUT(); // pull OW-Pin low for 480us
    OW_OUT_LOW(); // disable internal pull-up (maybe on from parasite)

    delay_us(400);	  //about 480us

    // set Pin as input - wait for clients to pull low
    OW_DIR_IN(); // input

    delay_us(66);
    err = OW_GET_IN();		// no presence detect
    // nobody pulled to low, still high


    // after a delay the clients should release the line
    // and input-pin gets back to high due to pull-up-resistor
    delay_us(480 - 66);
    if( OW_GET_IN() == 0 )		// short circuit
        err = 1;

    return err;
}

// 位处理函数
static uint8_t ow_bit_io( uint8_t b )
{
    OW_DIR_OUT(); // drive bus low
    OW_OUT_LOW();
    delay_us(1); // Recovery-Time wuffwuff was 1

    if ( b ) OW_DIR_IN(); // if bit is 1 set bus high (by ext. pull-up)

#define  OW_CONF_DELAYOFFSET  5
    delay_us(15 - 1 - OW_CONF_DELAYOFFSET);

    if( OW_GET_IN() == 0 ) b = 0;  // sample at end of read-timeslot

    delay_us(60 - 15);
    OW_DIR_IN();

    return b;
}

// 写入字节处理函数
static uint8_t ow_byte_wr( uint8_t b )
{
    uint8_t i = 8, j;
    do
    {
        j = ow_bit_io( b & 1 );
        b >>= 1;
        if( j ) b |= 0x80;
    }
    while( --i );
    return b;
}

// 读取字节处理函数
static uint8_t ow_byte_rd( void )
{
    return ow_byte_wr( 0xFF );
}

//
//float ds18b20_read(void)
//{
//    _u8 val[2];
//    _u8 i = 0;

//    unsigned int x = 0;

//    ow_reset();
//    ow_byte_wr(OW_SKIP_ROM);
//    ow_byte_wr(DS18B20_CONVERT);
//    delay_us(750000);

//    ow_reset();
//    ow_byte_wr( OW_SKIP_ROM );
//    ow_byte_wr ( DS18B20_READ );

//    for ( i = 0 ; i < 2; i++)
//    {
//        val[i] = ow_byte_rd();
//    }

//    x = val[1];
//    x <<= 8;
//    x |= val[0];

//    return (x & 0x07FF)*1.0/16;
//}

// 读取温度的值 返回一个浮点小数
float ds18b20Read(void)
{
  	_u8 val[2],s=0;
  	_u8 i = 0;
	unsigned int x = 0;	
	float Temp = 0;
	
	__disable_irq(); 	//关闭总中断

	// 时序初始化 相当于判断总线是否可用并且经过这个操作总结上的设备已经准备好了
  	ow_reset();
	// 主设备发出跳过 ROM 命令
  	ow_byte_wr(OW_SKIP_ROM); // 0xCC
	// 主设备发出转换命令
  	ow_byte_wr(DS18B20_CONVERT); // 0x44
  	delay_us(750000);
	
	// 时序初始化
  	ow_reset();
	// 主设备发出跳过 ROM 命令
  	ow_byte_wr( OW_SKIP_ROM );  // 0xCC
	// 主设备读取暂存器命令。
  	ow_byte_wr ( DS18B20_READ );  // 0xbe

	// 读取两个字节
  	for ( i=0 ;i<2; i++) {
    	val[i] = ow_byte_rd();
  	}
	__enable_irq();		//开启总中断
	
	// 判断温度的正负
	if(val[1] >7)
	{
		val[0] = ~val[0];
		val[1] = ~val[1];
		s = 0;
	}
	else 
		s = 1;

	x = val[1];
	x <<= 8;   
	x |= val[0];
	
	Temp = (float)(x*1.0*0.0625);	
	
	// 判断温度的正负
	if(s)	return Temp;
	else return -Temp;
}

