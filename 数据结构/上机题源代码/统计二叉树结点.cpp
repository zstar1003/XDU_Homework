//统计结点总数及叶子结点总数的程序代码
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
int countnode(bitree*); 
int countleaf(bitree*); 

int main()
{
	bitree*root;
	int leafnum,nodenum;
	root=creattree();
	printf("删除子树之前的二叉树：");
	preorder(root);
	printf("\n");
    nodenum=countnode(root);
    printf("结点总数是：%d\n",nodenum);
	leafnum=countleaf(root); 
    printf("叶子结点总数是：%d\n",leafnum);
}

//建立二叉树
bitree*creattree()
{
	datatype ch;
	bitree*Q[maxsize];
	int front,rear;
	bitree*root,*s;
	root=NULL;
	front=1;rear=0;
	printf("按层次输入结点值，虚结点输入'@'，以换行符结束：");
	while((ch=getchar())!='\n')
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

//先序遍历输出二叉树
void preorder(bitree*p)
{
	if(p!=NULL)
	{
		printf("%c",p->data);
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

//添加统计结点个数算法
int countnode(bitree*p)
{
	int nodenum=0;
	if(p==NULL)return 0;
	if(p->lchild!=NULL||p->rchild!=NULL)nodenum++;
	if(p->lchild==NULL&&p->rchild==NULL)nodenum++;
	nodenum+=countnode(p->lchild);
	nodenum+=countnode(p->rchild);
	return nodenum;
}
//添加统计叶子结点个数算法
int countleaf(bitree*p)
{
	int leafnum=0;
	if(p==NULL)return 0;
	if(p->lchild==NULL&&p->rchild==NULL)return 1;
	leafnum+=countleaf(p->lchild);
	leafnum+=countleaf(p->rchild);
	return leafnum;
}
