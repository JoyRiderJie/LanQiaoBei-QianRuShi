#include "list.h"

/*******************************************
* 函数功能:给链表添加节点
* 参数:
*		struct head*head:链表的头结点
*		LISTNODETYPE*newNode:链表新增的节点
* 返回值：无
********************************************/
void addListNode(struct head*head,struct node*newNode)
{
	struct node*list = head->pNext;
	//判断是否没有任何节点
	if(list)
	{
		//移动到待添加位置的前一个位置
		while(list->pNext){
			list = list->pNext;
		}
		//添加节点并且给链表长度加1
		list->pNext = newNode;
	}
	else
	{
		head->pNext = newNode; 
	}
	//判断节点的类型
	if(newNode->ucType[0] == 'C')
	{
		head->uiCnbrCount++;
		head->uiIdleCount--;
	}
	else if(newNode->ucType[0] == 'V')
	{
		head->uiVnbrCount++;
		head->uiIdleCount--;
	}
}


/*******************************************
* 函数功能:给链表删除节点
* 参数:
*		struct head*head:链表的头结点
*		unsigned char*target:目标值
* 返回值：无
********************************************/
void deleteListNode(struct head*head,char*target)
{
	struct node*p,*q;
	if(!head->pNext) return ;
	p = head->pNext;
	//判断头结点是否是目标值
	if(strcmp((char*)p->ucCode,(char*)target))
	{
		//遍历出头节点外的所有节点
		while(p->pNext && strcmp((char*)p->pNext->ucCode,(char*)target))
		{
			p = p->pNext;
		}
		q = p->pNext;
		p->pNext = p->pNext->pNext;
	}
	else
	{
		//删除头结点
		q = head->pNext;
		head->pNext = head->pNext->pNext;
	}
	//判断目标值的类型
	if(q->ucType[0] == 'C')
	{
		head->uiCnbrCount--;
		head->uiIdleCount++;
	}
	else if(q->ucType[0] == 'V')
	{
		head->uiVnbrCount--;
		head->uiIdleCount++;
	}
}

/*******************************************
* 函数功能:判断链表是否为空
* 参数:
*		struct head*head:链表的头结点
* 返回值：
*		链表为空返回0 否则返回链表长度
********************************************/
unsigned int isEmptyListNode(struct head*head)
{
	return head->uiCnbrCount+head->uiVnbrCount;
}

/*******************************************
* 函数功能:查找链表
* 参数:
*		struct head*head:链表的头结点
*		
* 返回值：
*		链表无值返回0 否则返回1
********************************************/
struct node* searchListNode(struct head*head,char*target)
{
	struct node*p = head->pNext;
	//遍历链表
	while(p)
	{
		//判断是否应该删除
		if(!strcmp(p->ucCode,target))
		{
			return p;
		}
		p = p->pNext;
	}
	return NULL;
}




