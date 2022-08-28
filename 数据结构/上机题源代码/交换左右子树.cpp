//�������������ĳ������
#include<stdio.h>
#include<malloc.h>
//��������Ľṹ���Ͷ���
const int maxsize=1024;
typedef char datatype;
typedef struct node
{
	datatype data;
	struct node *lchild,*rchild;
}bitree;

bitree*creattree();
void preorder(bitree*);
bitree*swap(bitree*);

int main()
{
	bitree*pb;
	pb=creattree();
	preorder(pb);
	printf("\n");
	swap(pb);
	preorder(pb);
	printf("\n");
}

//�������Ľ���
bitree*creattree()
{
	char ch;
	bitree*Q[maxsize];
	int front,rear;
	bitree*root,*s;
	root=NULL;
	front=1;rear=0;
	printf("������������������������'@'����'#'�������룺\n");
	while((ch=getchar())!='#')
	{
		s=NULL;
		if(ch!='@')
		{
			s=(bitree*)malloc(sizeof(bitree));
			s->data=ch;
			s->lchild=NULL;
			s->rchild=NULL;
		}
		rear++;
		Q[rear]=s;
		if(rear==1)root=s;
		else
		{
			if(s&&Q[front])
				if(rear%2==0)Q[front]->lchild=s;
				else Q[front]->rchild=s;
			if(rear%2==1)front++;
		}
	}
	return root;
}

//���������������������
void preorder(bitree*p)
{
	if(p!=NULL)
	{
		printf("%c",p->data);
		if(p->lchild!=NULL||p->rchild!=NULL)
		{
			printf("(");
			preorder(p->lchild);
			if(p->rchild!=NULL)printf(",");
			preorder(p->rchild);
			printf(")");
		}
	}
}

//��ӽ������������㷨
bitree*swap(bitree*p)
{
	bitree *temp;
	temp=p->lchild;
	p->lchild=p->rchild;
	p->rchild=temp;
	if(p->lchild)swap(p->lchild);
	if(p->rchild)swap(p->rchild);
	return p;
}


