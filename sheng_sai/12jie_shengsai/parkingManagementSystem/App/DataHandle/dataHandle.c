#include "dataHandle.h"

//�洢ÿ�µ�����
int monthTable[12] = {31,28,31,30,31,30,31,31,30,31,30,31}; 


/****************************************
* �������ܣ���ָ��λ���ַ���ת������������
* ����������
* 			char*s:������ַ�������
*			int iStart:��ʼλ��
*			int iEnd:��ֹλ��
* ��������ֵ��
*			res:���ص���������
****************************************/
int getIntRxBuffDataByLocation(char*s,int iStart,int iEnd)
{
	int res = 0;
	while(iStart < iEnd)
	{
		res = res*10 + s[iStart] - '0';
		iStart++;
	}
	
	return res;
}

/****************************************
* �������ܣ���ȡָ��λ���ַ���
* ����������
* 			char*s:������ַ���
*			int iStart:��ʼλ��
*			int iEnd:��ֹλ��
* ��������ֵ��
*			res:���ص��ַ���
****************************************/
void getStringRxBuffDataByLocation(char*s,char*res,int iStart,int iEnd)
{
	int j=0;
	while(iStart < iEnd)
	{
		res[j++] = s[iStart++];
	}
	res[j++] = '\0';
}

/****************************************
* �������ܣ���һ��������ʱ�����ʱ��ת������
* ����������
* 			const unsigned int year0��ת��ʱ�����
*			const unsigned int mon0��ת��ʱ�����
*			const unsigned int day��ת��ʱ�����
*			const unsigned int hour��ת��ʱ���Сʱ
*			const unsigned int min��ת��ʱ��ķ���
*			const unsigned int sec��ת��ʱ�����
* ��������ֵ��
*			res:����ת��������ʱ��
****************************************/
unsigned long myMktime(const unsigned int year0, const unsigned int mon0,const unsigned int day,
	const unsigned int hour,const unsigned int min,const unsigned int sec)
{
    unsigned int mon = mon0, year = year0;

    /* 1..12 -> 11,12,1..10 */
    if (0 >= (int) (mon -= 2)) {
        mon += 12;    /* Puts Feb last since it has leap day */
        year -= 1;
    }

    return ((((unsigned long)
          (year/4 - year/100 + year/400 + 367*mon/12 + day) +
          year*365 - 719499
        )*24 + hour /* now have hours */
      )*60 + min /* now have minutes */
    )*60+sec; /* finally seconds */
}

/****************************************
* �������ܣ��жϴ��ڷ��͹����������Ƿ����Ҫ��
* ����������
* 			const unsigned int year0��ת��ʱ�����
*			const unsigned int mon0��ת��ʱ�����
*			const unsigned int day��ת��ʱ�����
*			const unsigned int hour��ת��ʱ���Сʱ
*			const unsigned int min��ת��ʱ��ķ���
*			const unsigned int sec��ת��ʱ�����
* ��������ֵ��
*			1������Ҫ��
*			0:������Ҫ��
****************************************/
int checkData(const char*type,const int year, const int mon,const  int day, 
	const int hour,const  int min,const  int sec)
{
	//�ж��Ƿ���ڸ���  ����ֱ�ӷ���
	if(year<0 || mon<0 || day<0 || hour<0 || min<0 || sec<0)
		return 0;
	
	//���������ͣ�����Ͳ���ֱ�ӷ���
	if(strcmp(type,"CNBR")!=0 && strcmp(type,"VNBR")!=0)
		return 0;
	
	//��
	if(year > 99)
		return 0;
	//��
	if( mon > 12)
		return 0;
	//��  
	//����  ���·�
	if(((year%4==0 && year%100!=0)||year%400==0) && day>monthTable[1]+1)
		return 0;
	//ƽ�������·� ��������·���
	else if(day>monthTable[mon])
		return 0;
	//ʱ
	if(hour >=24)
		return 0;
	//��
	if(min >=60)
		return 0;
	//��
	if(sec >= 60)
		return 0;
	
	return 1;
}

