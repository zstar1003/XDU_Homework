//�ж϶����������ĳ������
#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
//��������Ľṹ���Ͷ���
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
	if(inorder(pb)==1)printf("�Ƕ�����������\n");
	else printf("���Ƕ�����������\n");
}

//�������Ľ���
bitree*creattree()
{
	keytype x;
	bitree*Q[maxsize];
	int front,rear;
	bitree*root,*s;
	root=NULL;
	front=1;rear=0;
	printf("�����������������������ͽ�����ݣ�0��ʾ���㣬-1��ʾ������\n");
	scanf("%d",&x);//����0��ʾ���㣬-1��ʾ����
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

//�����������
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

//����ж϶����������㷨

/*�ж��������Ľ��ֵ�Ƿ�С��val*/
int isSubTreeLessThan(bitree*p, int val)
{
 if (!p) return 1;
 return (p->key < val &&
     isSubTreeLessThan(p->lchild, val) &&
     isSubTreeLessThan(p->rchild, val));
}
/*�ж��������Ľ��ֵ�Ƿ񶼴���val*/
int isSubTreeGreaterThan(bitree*p, int val)
{
 if (!p) return 1;
 return (p->key > val &&
     isSubTreeGreaterThan(p->lchild, val) &&
     isSubTreeGreaterThan(p->rchild, val));
}
/*�ж��������Ƿ��Ƕ���������*/
int inorder(bitree*p)
{
 if (!p) return 1;
 return isSubTreeLessThan(p->lchild, p->key) &&
     isSubTreeGreaterThan(p->rchild, p->key) &&
     inorder(p->lchild) &&
     inorder(p->rchild);
}



