//���ַ������ĶԳ�
#include<stdio.h>
#include<malloc.h>
#include<string.h>
//���嵥����ṹ����
typedef char datatype;
typedef struct node
{  datatype data;
   struct node *next;
}linklist;
//����˳��ջ�ṹ����
const int maxsize=40;
typedef struct
{  datatype elements[maxsize];
   int top;
}stack;

void setnull(stack *&);
int length(linklist*);
void printlink(linklist*);
void create(linklist *&,datatype*);
void push(stack*,datatype);
datatype pop(stack*);
int symmetry(linklist*,stack*);//���ַ����Ƿ����ĶԳƵĺ�������

int main()
{
	linklist *head;stack *s;
	datatype str[80];
	gets(str);
	create(head,str);
	printlink(head);
	setnull(s);
	if(symmetry(head,s)) printf("�ַ���\"%s\"���ĶԳ�\n",str);
	else printf("�ַ���\"%s\"�������ĶԳ�\n",str);
}

//ջ��ʼ��
void setnull(stack *&s)
{
	s=(stack*)malloc(sizeof(stack));
	s->top=-1;
}

//��������
int length(linklist*head)
{  linklist *p=head->next;
   int n=0;
   while(p!=NULL){  n++;  p=p->next; }
   return n;
}

//���������
void printlink(linklist*head)
{  linklist *p=head->next;
   while(p!=NULL)
   {  printf("%c",p->data);
      p=p->next;
   }
   printf("\n");
}

//��������ͷ���ĵ�����
void create(linklist *&head,datatype*str)
{  datatype *p=str;
   linklist *s,*r;
   head=(linklist*)malloc(sizeof(linklist));
   r=head;
   while(*p!='\0')
   {  
	  s=(linklist*)malloc(sizeof(linklist));
      s->data=*p;
	  r->next=s;
	  r=s;
	  p++;
   }
   r->next=NULL;
}

//˳��ջ��ջ
void push(stack*s,datatype e)
{
	s->top++;
	s->elements[s->top]=e;
}

//˳��ջ��ջ
datatype pop(stack*s)
{
	datatype temp;
	s->top--;
	temp=s->elements[s->top+1];
	return temp;
}
//������ַ����Ƿ����ĶԳ��㷨
int symmetry(linklist*head,stack*s)
{	
	int l=length(head);
	linklist *p=head->next;
	for(int i=1;i<=l/2;i++)
	{
		push(s,p->data);
		p=p->next;
	}
	if(l%2==1)p=p->next;
	while(p->data==spop(s))
	{
		p=p->next;
	}
	p==NULL?return 1:return 0;
} 

