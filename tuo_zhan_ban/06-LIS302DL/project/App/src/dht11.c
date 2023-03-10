#include "dht11.h"

#define Delay_us(X)  delay((X)*80/5)

void delay(unsigned int n)
{
    while(n--);
}



void DHT11_PIN_OUT(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /**/
    GPIO_InitStruct.Pin = DHT11_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(DHT11_PIN_PORT, &GPIO_InitStruct);
}
void DHT11_PIN_INPUT(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    DHT11_PIN_CLOCK;
    /**/
    GPIO_InitStruct.Pin = DHT11_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(DHT11_PIN_PORT, &GPIO_InitStruct);
}
//��λDHT11
void DHT11_Rst(void)
{
    DHT11_PIN_OUT(); 	//����Ϊ���
    DHT11_PIN_OUT_L; 	//�͵�ƽ
    HAL_Delay(20);	//��������18ms
    DHT11_PIN_OUT_H; 	//�ߵ�ƽ
    Delay_us(60);     	//��������20~40us
}

//�ȴ�DHT11�Ļ�Ӧ
//����1:δ��⵽DHT11�Ĵ���
//����0:����
uint8_t DHT11_Check(void)
{
    uint8_t re = 0;
    DHT11_PIN_INPUT();      //����Ϊ���
    while (DHT11_PIN_IN && re < 100) //DHT11������40~80us
    {
        re++;
        Delay_us(1);
    };
    if(re >= 100)return 1;
    else re = 0;
    while (!DHT11_PIN_IN && re < 100) //DHT11���ͺ���ٴ�����40~80us
    {
        re++;
        Delay_us(1);
    };
    if(re >= 100)return 1;
    return 0;
}

//��DHT11��ȡһ��λ
uint8_t DHT11_Read_Bit(void)
{
    uint8_t re = 0;
    while(DHT11_PIN_IN && re < 110) //�ȴ���Ϊ�͵�ƽ
    {
        re++;
        Delay_us(1);
    }
    re = 0;
    while(!DHT11_PIN_IN && re < 110) //�ȴ���ߵ�ƽ
    {
        re++;
        Delay_us(1);
    }
    Delay_us(80);//�ȴ�40us
    if(DHT11_PIN_IN)return 1;
    else return 0;
}

//��DHT11��ȡһ���ֽ�
uint8_t DHT11_Read_Byte(void)
{
    uint8_t i, dat;
    dat = 0;
    for (i = 0; i < 8; i++)
    {
        dat <<= 1;
        dat |= DHT11_Read_Bit();
    }
    return dat;
}

//��ʼ��DHT11��IO�ڣ�ͬʱ���DHT11�Ĵ���
uint8_t DHT11_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    DHT11_PIN_CLOCK;;
    GPIO_InitStruct.Pin = DHT11_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(DHT11_PIN_PORT, &GPIO_InitStruct);

    DHT11_Rst();
    return DHT11_Check();
}

//��DHT11��ȡһ������  data[2]��ʾ�¶����� data[3]��ʾ�¶�С���� data[0]��ʾʪ������ data[1]��ʾʪ��С��
uint8_t DHT11_Read_Data(uint8_t *data)
{
    uint8_t buf[5];
    uint8_t i;
    DHT11_Rst();
    if(DHT11_Check() == 0)
    {
        // ��ȡһ������
        for(i = 0; i < 5; i++)
        {
            buf[i] = DHT11_Read_Byte();
        }
        // �ж������Ƿ����
        if((buf[0] + buf[1] + buf[2] + buf[3]) == buf[4])
        {
            for(i=0; i<4; i++)
                data[i] = buf[i];
        }
    }
    else
        return 1;
    return 0;
}



