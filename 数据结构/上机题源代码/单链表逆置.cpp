#include<malloc.h>
#include<stdio.h>
//������ṹ���Ͷ���
typedef char datatype;
typedef struct node
{
	datatype data;
	struct node *next;
}linklist;
void create(linklist*&);
void print(linklist *);
void invert(linklist*);
int main()
{
	linklist*head;
	create(head);
	print(head);
	invert(head);//���õ��������õĺ���
	print(head);
}

//����β�巨��������ͷ���ĵ�����
void create(linklist*&head)
{
	char ch;
	linklist *s,*r;
	head=(linklist*)malloc(sizeof(linklist));
	r=head;
	while((ch=getchar())!='*')
	{
		s=(linklist*)malloc(sizeof(linklist));
		s->data=ch;
		r->next=s;
		r=s;
	}
	r->next=NULL;
}

//���������
void print(linklist *head)
{
	linklist*p=head->next;
	while(p!=NULL)
	{
		printf("%2c",p->data);
		p=p->next;
	}
	printf("\n");
}

//��ӵ����������㷨
void invert(linklist *head)
{
	linklist *p=head->next,*q,*r;
	q=head->next->next;
	head->next->next=NULL;
	while(q!=NULL)
	{
		r = q->next;
		q->next=p;
		p=q;
		q=r;
	}
	head->next=p;
}
