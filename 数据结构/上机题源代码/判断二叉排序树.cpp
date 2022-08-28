//判断二叉排序树的程序代码
#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
//二叉链表的结构类型定义
const int maxsize=1024;
typedef int keytype;
typedef struct node
{
	keytype key;
	struct node *lchild,*rchild;
}bitree;

bitree*creattree();
void preorder(bitree*);
int inorder(bitree*);

int main()
{
	bitree*pb;
	pb=creattree();
	preorder(pb);
	printf("\n");
	if(inorder(pb)==1)printf("是二叉排序树！\n");
	else printf("不是二叉排序树！\n");
}

//二叉树的建立
bitree*creattree()
{
	keytype x;
	bitree*Q[maxsize];
	int front,rear;
	bitree*root,*s;
	root=NULL;
	front=1;rear=0;
	printf("按层次输入二叉排序树的整型结点数据，0表示虚结点，-1表示结束：\n");
	scanf("%d",&x);//输入0表示虚结点，-1表示结束
	while(x!=-1)
	{
		s=NULL;
		if(x!=0)
		{
			s=(bitree*)malloc(sizeof(bitree));
			s->key=x;
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
		scanf("%d",&x);;
	}
	return root;
}

//二叉树的输出
void preorder(bitree*p)
{
	if(p!=NULL)
	{
		printf("%d",p->key);
		if(p->lchild!=NULL||p->rchild!=NULL)
		{
			printf("(");
			preorder(p->lchild);
			if(p->rchild!=NULL) printf(",");
			preorder(p->rchild);
			printf(")");
		}
	}
}

//添加判断二叉排序树算法

/*判断左子树的结点值是否都小于val*/
int isSubTreeLessThan(bitree*p, int val)
{
 if (!p) return 1;
 return (p->key < val &&
     isSubTreeLessThan(p->lchild, val) &&
     isSubTreeLessThan(p->rchild, val));
}
/*判断右子树的结点值是否都大于val*/
int isSubTreeGreaterThan(bitree*p, int val)
{
 if (!p) return 1;
 return (p->key > val &&
     isSubTreeGreaterThan(p->lchild, val) &&
     isSubTreeGreaterThan(p->rchild, val));
}
/*判定二叉树是否是二叉搜索树*/
int inorder(bitree*p)
{
 if (!p) return 1;
 return isSubTreeLessThan(p->lchild, p->key) &&
     isSubTreeGreaterThan(p->rchild, p->key) &&
     inorder(p->lchild) &&
     inorder(p->rchild);
}



