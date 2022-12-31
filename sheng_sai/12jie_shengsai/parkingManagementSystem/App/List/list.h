#ifndef _LIST_H
#define _LIST_H

#include "string.h"


//����ͷ���
struct head{
	//ָ��洢��������Ϣ
	struct node*pNext;
	//��¼���೵λ����
	int uiIdleCount;
	//CNBR���ͳ�������
	int uiCnbrCount;
	//VNBR���ͳ���������
	int uiVnbrCount;
};

//������Ϣ�ṹ��
struct node{
	//��¼ͣ������
	char ucType[5];
	//��¼���ƺ�
	char ucCode[5];
	//��¼����ʱ��
	int year;
	int month;
	int day;
	int hour;
	int minute;
	int second;
	//ָ����һ������Ϣ
	struct node*pNext;
};

/*******************************************
* ��������:��������ӽڵ�
* ����:
*		struct head*head:�����ͷ���
*		LISTNODETYPE*newNode:���������Ľڵ�
* ����ֵ����
********************************************/
void addListNode(struct head*head,struct node*newNode);

/*******************************************
* ��������:������ɾ���ڵ�
* ����:
*		struct head*head:�����ͷ���
*		unsigned char*target:Ŀ��ֵ
* ����ֵ����
********************************************/
void deleteListNode(struct head*head,char*target);

/*******************************************
* ��������:�ж������Ƿ�Ϊ��
* ����:
*		struct head*head:�����ͷ���
* ����ֵ��
*		����Ϊ�շ���0 ���򷵻�������
********************************************/
unsigned int isEmptyListNode(struct head*head);

/*******************************************
* ��������:��������
* ����:
*		struct head*head:�����ͷ���
*		
* ����ֵ��
*		������ֵ����0 ���򷵻�1
********************************************/
struct node* searchListNode(struct head*head,char*target);


#endif

