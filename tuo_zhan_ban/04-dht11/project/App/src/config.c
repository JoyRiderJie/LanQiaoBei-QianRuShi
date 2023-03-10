#include "config.h"

/*****************************************************************
* @sysWork：项目的主要功能初始化函数
* @arg:
*		无
* @return:
*		无
*****************************************************************/
void sysInit(void)
{
	// LCD初始化
	lcdInit();
	// DHT11初始化
	while(DHT11_Init())
	{
        LCD_DisplayStringLine(Line0, (uint8_t *)"  DHT11 Error       ");
        HAL_Delay(200);
    }
	LCD_DisplayStringLine(Line0, (uint8_t *)"  DHT11 OK          ");
}

/*****************************************************************
* @sysWork：项目的主要功能逻辑函数
* @arg:
*		无
* @return:
*		无
*****************************************************************/
void sysWork(void)
{
	u8 data[4] = {0,0,0,0};
	float temp = 0; 
	char txt[50];
	
//	int temp = (int)(ds18b20Read()*10);
//	segBuff[0] = temp/100;//scanBtn();
//	segBuff[1] = temp/10%10+12;
//	segBuff[2] = temp%10;
//	segDisplay();
	
	temp = ds18b20Read();
	DHT11_Read_Data(data);
	sprintf(txt," temp:%.3f ",temp);
	LCD_DisplayStringLine(Line1,(uint8_t*)txt);
	if( data[0]!=0 || data[2]!=0 )
	{
		sprintf(txt,"hD:%d.%d tD:%d.%d",data[0],data[1],data[2],data[3]);
		LCD_DisplayStringLine(Line2,(uint8_t*)txt);
	}
	HAL_Delay(500);
}

