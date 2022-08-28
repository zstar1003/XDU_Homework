//交换左右子树的程序代码
#include<stdio.h>
#include<malloc.h>
//二叉链表的结构类型定义
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

//二叉树的建立
bitree*creattree()
{
	char ch;
	bitree*Q[maxsize];
	int front,rear;
	bitree*root,*s;
	root=NULL;
	front=1;rear=0;
	printf("按层次输入二叉树，虚结点输入'@'，以'#'结束输入：\n");
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

//先序遍历按层次输出二叉树
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

//添加交换左右子树算法
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


