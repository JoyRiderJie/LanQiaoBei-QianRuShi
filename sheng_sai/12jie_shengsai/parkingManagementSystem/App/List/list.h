#ifndef _LIST_H
#define _LIST_H

#include "string.h"


//链表头结点
struct head{
	//指向存储辆车的信息
	struct node*pNext;
	//记录空余车位数量
	int uiIdleCount;
	//CNBR类型车辆数量
	int uiCnbrCount;
	//VNBR类型车辆的数量
	int uiVnbrCount;
};

//车辆信息结构体
struct node{
	//记录停车类型
	char ucType[5];
	//记录车牌号
	char ucCode[5];
	//记录进入时间
	int year;
	int month;
	int day;
	int hour;
	int minute;
	int second;
	//指向下一辆车信息
	struct node*pNext;
};

/*******************************************
* 函数功能:给链表添加节点
* 参数:
*		struct head*head:链表的头结点
*		LISTNODETYPE*newNode:链表新增的节点
* 返回值：无
********************************************/
void addListNode(struct head*head,struct node*newNode);

/*******************************************
* 函数功能:给链表删除节点
* 参数:
*		struct head*head:链表的头结点
*		unsigned char*target:目标值
* 返回值：无
********************************************/
void deleteListNode(struct head*head,char*target);

/*******************************************
* 函数功能:判断链表是否为空
* 参数:
*		struct head*head:链表的头结点
* 返回值：
*		链表为空返回0 否则返回链表长度
********************************************/
unsigned int isEmptyListNode(struct head*head);

/*******************************************
* 函数功能:查找链表
* 参数:
*		struct head*head:链表的头结点
*		
* 返回值：
*		链表无值返回0 否则返回1
********************************************/
struct node* searchListNode(struct head*head,char*target);


#endif

