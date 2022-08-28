//ͳ�ƽ��������Ҷ�ӽ�������ĳ������
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
int countnode(bitree*); 
int countleaf(bitree*); 

int main()
{
	bitree*root;
	int leafnum,nodenum;
	root=creattree();
	printf("ɾ������֮ǰ�Ķ�������");
	preorder(root);
	printf("\n");
    nodenum=countnode(root);
    printf("��������ǣ�%d\n",nodenum);
	leafnum=countleaf(root); 
    printf("Ҷ�ӽ�������ǣ�%d\n",leafnum);
}

//����������
bitree*creattree()
{
	datatype ch;
	bitree*Q[maxsize];
	int front,rear;
	bitree*root,*s;
	root=NULL;
	front=1;rear=0;
	printf("�����������ֵ����������'@'���Ի��з�������");
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

//����������������
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

//���ͳ�ƽ������㷨
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
//���ͳ��Ҷ�ӽ������㷨
int countleaf(bitree*p)
{
	int leafnum=0;
	if(p==NULL)return 0;
	if(p->lchild==NULL&&p->rchild==NULL)return 1;
	leafnum+=countleaf(p->lchild);
	leafnum+=countleaf(p->rchild);
	return leafnum;
}
