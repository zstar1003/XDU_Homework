//判字符串中心对称
#include<stdio.h>
#include<malloc.h>
#include<string.h>
//定义单链表结构类型
typedef char datatype;
typedef struct node
{  datatype data;
   struct node *next;
}linklist;
//定义顺序栈结构类型
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
int symmetry(linklist*,stack*);//判字符串是否中心对称的函数声明

int main()
{
	linklist *head;stack *s;
	datatype str[80];
	gets(str);
	create(head,str);
	printlink(head);
	setnull(s);
	if(symmetry(head,s)) printf("字符串\"%s\"中心对称\n",str);
	else printf("字符串\"%s\"不是中心对称\n",str);
}

//栈初始化
void setnull(stack *&s)
{
	s=(stack*)malloc(sizeof(stack));
	s->top=-1;
}

//求单链表长度
int length(linklist*head)
{  linklist *p=head->next;
   int n=0;
   while(p!=NULL){  n++;  p=p->next; }
   return n;
}

//输出单链表
void printlink(linklist*head)
{  linklist *p=head->next;
   while(p!=NULL)
   {  printf("%c",p->data);
      p=p->next;
   }
   printf("\n");
}

//建立具有头结点的单链表
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

//顺序栈入栈
void push(stack*s,datatype e)
{
	s->top++;
	s->elements[s->top]=e;
}

//顺序栈出栈
datatype pop(stack*s)
{
	datatype temp;
	s->top--;
	temp=s->elements[s->top+1];
	return temp;
}
//添加判字符串是否中心对称算法
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

