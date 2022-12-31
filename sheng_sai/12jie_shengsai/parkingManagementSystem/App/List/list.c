#include "list.h"

/*******************************************
* ��������:��������ӽڵ�
* ����:
*		struct head*head:�����ͷ���
*		LISTNODETYPE*newNode:���������Ľڵ�
* ����ֵ����
********************************************/
void addListNode(struct head*head,struct node*newNode)
{
	struct node*list = head->pNext;
	//�ж��Ƿ�û���κνڵ�
	if(list)
	{
		//�ƶ��������λ�õ�ǰһ��λ��
		while(list->pNext){
			list = list->pNext;
		}
		//��ӽڵ㲢�Ҹ������ȼ�1
		list->pNext = newNode;
	}
	else
	{
		head->pNext = newNode; 
	}
	//�жϽڵ������
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
* ��������:������ɾ���ڵ�
* ����:
*		struct head*head:�����ͷ���
*		unsigned char*target:Ŀ��ֵ
* ����ֵ����
********************************************/
void deleteListNode(struct head*head,char*target)
{
	struct node*p,*q;
	if(!head->pNext) return ;
	p = head->pNext;
	//�ж�ͷ����Ƿ���Ŀ��ֵ
	if(strcmp((char*)p->ucCode,(char*)target))
	{
		//������ͷ�ڵ�������нڵ�
		while(p->pNext && strcmp((char*)p->pNext->ucCode,(char*)target))
		{
			p = p->pNext;
		}
		q = p->pNext;
		p->pNext = p->pNext->pNext;
	}
	else
	{
		//ɾ��ͷ���
		q = head->pNext;
		head->pNext = head->pNext->pNext;
	}
	//�ж�Ŀ��ֵ������
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
* ��������:�ж������Ƿ�Ϊ��
* ����:
*		struct head*head:�����ͷ���
* ����ֵ��
*		����Ϊ�շ���0 ���򷵻�������
********************************************/
unsigned int isEmptyListNode(struct head*head)
{
	return head->uiCnbrCount+head->uiVnbrCount;
}

/*******************************************
* ��������:��������
* ����:
*		struct head*head:�����ͷ���
*		
* ����ֵ��
*		������ֵ����0 ���򷵻�1
********************************************/
struct node* searchListNode(struct head*head,char*target)
{
	struct node*p = head->pNext;
	//��������
	while(p)
	{
		//�ж��Ƿ�Ӧ��ɾ��
		if(!strcmp(p->ucCode,target))
		{
			return p;
		}
		p = p->pNext;
	}
	return NULL;
}




