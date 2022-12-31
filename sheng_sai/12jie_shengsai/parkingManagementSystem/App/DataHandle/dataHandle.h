#ifndef _DATAHANDLE_H
#define _DATAHANDLE_H

#include "string.h"

/****************************************
* 函数功能：获取指定位置字符串
* 函数参数：
* 			char*s:传入的字符串
*			int iStart:起始位置
*			int iEnd:终止位置
* 函数返回值：
*			res:返回的整型数字
****************************************/
void getStringRxBuffDataByLocation(char*s,char*res,int iStart,int iEnd);

/****************************************
* 函数功能：将指定位置字符串转换成整型数字
* 函数参数：
* 			char*s:传入的字符串数字
*			int iStart:起始位置
*			int iEnd:终止位置
* 函数返回值：
*			res:返回的整型数字
****************************************/
int getIntRxBuffDataByLocation(char*s,int iStart,int iEnd);

/****************************************
* 函数功能：将一个年月日时分秒的时间转换成秒
* 函数参数：
* 			const unsigned int year0：转换时间的年
*			const unsigned int mon0：转换时间的月
*			const unsigned int day：转换时间的天
*			const unsigned int hour：转换时间的小时
*			const unsigned int min：转换时间的分钟
* 函数返回值：
*			res:返回转换成秒后的时间
****************************************/
unsigned long myMktime(const unsigned int year0, const unsigned int mon0,
        const unsigned int day, const unsigned int hour,const unsigned int min,
		const unsigned int sec);
		
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
	const int hour,const  int min,const  int sec);

#endif
