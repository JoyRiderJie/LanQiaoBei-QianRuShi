#include "config.h"




/*****************************************************************
* @sysWork����Ŀ����Ҫ���ܳ�ʼ������
* @arg:
*		��
* @return:
*		��
*****************************************************************/
void sysInit(void)
{
    // LCD��ʼ��
    lcdInit();
#if 0
    // DHT11��ʼ��
    while(DHT11_Init())
    {
        LCD_DisplayStringLine(Line0, (uint8_t *)"  DHT11 Error       ");
        HAL_Delay(200);
    }
    LCD_DisplayStringLine(Line0, (uint8_t *)"  DHT11 OK          ");
#endif
#if 0
    // LIS392DL��ʼ�� ����˵�� IIC��ʼ��
    lis302dlInit();
#endif
}

/*****************************************************************
* @sysWork����Ŀ����Ҫ�����߼�����
* @arg:
*		��
* @return:
*		��
*****************************************************************/
void sysWork(void)
{
    u8 data[4] = {0,0,0,0};
    float alz[3] = {0,0,0};
    float temp = 0;
    float adcValue = 0;
    char txt[50];

// ds18b20�Ĳ���
#if 0
    int temp = (int)(ds18b20Read()*10);
    segBuff[0] = temp/100;//scanBtn();
    segBuff[1] = temp/10%10+12;
    segBuff[2] = temp%10;
    segDisplay();
#endif

// ds18b20����ʪ�ȴ�����DHT11�Ĳ���
#if 0
    temp = ds18b20Read();
    DHT11_Read_Data(data);
    sprintf(txt," temp:%.3f ",temp);
    LCD_DisplayStringLine(Line1,(uint8_t*)txt);
    if( data[0]!=0 || data[2]!=0 )
    {
        sprintf(txt," hD:%d.%d tD:%d.%d",data[0],data[1],data[2],data[3]);
        LCD_DisplayStringLine(Line2,(uint8_t*)txt);
    }
#endif

// ���մ������Ĳ���
#if 0
    // ��ȡģ������
    adcValue = getR47ADC();
    snprintf((char *)txt, sizeof(txt), " R-P:%.2fK  ", adcValue);
    LCD_DisplayStringLine(Line4,(uint8_t *)txt);

    // ��ȡ���������  ����������е��ѿ��� ��Ҫʹ���ֻ�������������
    if(HAL_GPIO_ReadPin(DO_GPIO_Port,DO_Pin) == 1)
        LCD_DisplayStringLine(Line5, (uint8_t *)"  HIGN    ");
    else
        LCD_DisplayStringLine(Line5, (uint8_t *)"  LOW     ");

#endif

// ������ٶȴ�����LIS302DL�Ĳ���
#if 0
    lis302dlOutput(alz);
    snprintf((char *)txt, sizeof(txt), "  X acclr:%.2fg   ", alz[0]);
    LCD_DisplayStringLine(Line6, (uint8_t*)txt);
    snprintf((char *)txt, sizeof(txt), "  Y acclr:%.2fg   ", alz[1]);
    LCD_DisplayStringLine(Line7, (uint8_t*)txt);
    snprintf((char *)txt, sizeof(txt), "  Z acclr:%.2fg   ", alz[2]);
    LCD_DisplayStringLine(Line8, (uint8_t*)txt);
#endif

// ADC����
#if 1
	float adcData[2] ;
    LCD_DisplayStringLine(Line7,(uint8_t*)"  ADC test");
	adcData[0] = getADC_RPx(&hadc2,ADC_CHANNEL_13);
	adcData[1] = getADC_RPx(&hadc2,ADC_CHANNEL_17);
	sprintf(txt," PB5:%.2f PB6:%.2f ",adcData[0],adcData[1]);
    LCD_DisplayStringLine(Line8,(uint8_t*)txt);
#endif

    HAL_Delay(500);
}

