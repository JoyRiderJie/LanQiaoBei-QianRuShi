#include "seg.h"

/************   共阴数码管段码  *****************/
//               0     1     2     3      4		5	  6     7     8     9    -    熄灭
u8 segTab[] = { 0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f, 0x40, 0x00,
//               0.    1.    2.    3.    4.    5.	 6.    7.    8.    9.    -    熄灭	
				0xbf, 0x86, 0xdb, 0xcf, 0xe6, 0xed, 0xfd, 0x87, 0xff, 0xef, 0x40, 0x00,
//				A      B     C     D      E      F  熄灭
				0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71, 0x00 };
// 储存数码管需要显示的值
u8 segBuff[3] = {10,10,10};

/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    segDisplay
  * @brief   数码管显示函数
  * @param   None
  * @retval  None
  * @author  黑心萝卜三条杠
  * @Data    2023-03-02
  * 1. ...
  * <modify staff>:
  * <data>        :
  * <description> :
  * 2. ...
 **/
/* -------------------------------- end -------------------------------- */
#if 0
void segDisplay(void)
{
    u8 code_tmp = 0,j = 0;
	static u8 i = 3;
	
	code_tmp = segTab[segBuff[i-1]];
	
	// 拉低复位端口 低电平有效 表示需要写入数据
	RCLK_L;
	for(j = 0; j < 8; ++j)
	{
		// 拉低控制信号
		SCK_L;
		// 判断最高位是否为1
		if(code_tmp & 0x80)
			// 写入1
			SER_H;
		else
			// 写入0
			SER_L;
		code_tmp = code_tmp << 1;
		// 拉低控制信号
		SCK_L;
		// 拉高控制信号
		SCK_H;
	}
	// 拉高复位端口 一般默认接VCC  表示写入数据完成
    RCLK_H;
	
	// 移动下一次显示数码管的位置
	if(--i > 0 ) i = 3;
}
#endif

#if 1
void segDisplay(void)
{
    u8 code_tmp = 0,j = 0,i = 3;
	
	for(i=3;i>0;--i)
	{
		code_tmp = segTab[segBuff[i-1]];
		
		// 拉低复位端口 低电平有效 表示需要写入数据
		RCLK_L;
		for(j = 0; j < 8; ++j)
		{
			// 拉低控制信号
			SCK_L;
			// 判断最高位是否为1
			if(code_tmp & 0x80)
				// 写入1
				SER_H;
			else
				// 写入0
				SER_L;
			code_tmp = code_tmp << 1;
			// 拉低控制信号
			SCK_L;
			// 拉高控制信号
			SCK_H;
		}
		// 拉高复位端口 一般默认接VCC  表示写入数据完成
		RCLK_H;
	}
}
#endif


