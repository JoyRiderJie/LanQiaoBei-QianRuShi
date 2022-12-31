#include "dataHandle.h"

//存储每月的天数
int monthTable[12] = {31,28,31,30,31,30,31,31,30,31,30,31}; 


/****************************************
* 函数功能：将指定位置字符串转换成整型数字
* 函数参数：
* 			char*s:传入的字符串数字
*			int iStart:起始位置
*			int iEnd:终止位置
* 函数返回值：
*			res:返回的整型数字
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
* 函数功能：获取指定位置字符串
* 函数参数：
* 			char*s:传入的字符串
*			int iStart:起始位置
*			int iEnd:终止位置
* 函数返回值：
*			res:返回的字符串
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
* 函数功能：将一个年月日时分秒的时间转换成秒
* 函数参数：
* 			const unsigned int year0：转换时间的年
*			const unsigned int mon0：转换时间的月
*			const unsigned int day：转换时间的天
*			const unsigned int hour：转换时间的小时
*			const unsigned int min：转换时间的分钟
*			const unsigned int sec：转换时间的秒
* 函数返回值：
*			res:返回转换成秒后的时间
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
* 函数功能：判断串口发送过来的数据是否符合要求
* 函数参数：
* 			const unsigned int year0：转换时间的年
*			const unsigned int mon0：转换时间的月
*			const unsigned int day：转换时间的天
*			const unsigned int hour：转换时间的小时
*			const unsigned int min：转换时间的分钟
*			const unsigned int sec：转换时间的秒
* 函数返回值：
*			1：符合要求
*			0:不符合要求
****************************************/
int checkData(const char*type,const int year, const int mon,const  int day, 
	const int hour,const  int min,const  int sec)
{
	//判断是否存在负数  存在直接返回
	if(year<0 || mon<0 || day<0 || hour<0 || min<0 || sec<0)
		return 0;
	
	//如果车辆的停车类型不对直接返回
	if(strcmp(type,"CNBR")!=0 && strcmp(type,"VNBR")!=0)
		return 0;
	
	//年
	if(year > 99)
		return 0;
	//月
	if( mon > 12)
		return 0;
	//日  
	//闰年  二月份
	if(((year%4==0 && year%100!=0)||year%400==0) && day>monthTable[1]+1)
		return 0;
	//平年所有月份 闰年除二月份外
	else if(day>monthTable[mon])
		return 0;
	//时
	if(hour >=24)
		return 0;
	//分
	if(min >=60)
		return 0;
	//秒
	if(sec >= 60)
		return 0;
	
	return 1;
}

