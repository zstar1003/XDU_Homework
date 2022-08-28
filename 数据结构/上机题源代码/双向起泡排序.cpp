//˫����������ĳ������
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
//˳���ṹ���Ͷ���
typedef int datatype;
typedef struct{
	int key;
	datatype data;
}sequenlist;

void create(sequenlist[],int);
void print(sequenlist[],int);
void dbubblesort(sequenlist[],int);

int main()
{
	const int n=10;
	sequenlist r[n+1];
	create(r,n);
	printf("����ǰ�����ݣ�");
	print(r,n);
	dbubblesort(r,n);
	printf("���������ݣ�");
	print(r,n);
}

//����˳���
void create(sequenlist r[],int n)
{
	srand(time(0));
	for(int i=1;i<=n;i++)
		r[i].key=rand()%90;
}

//���˳���
void print(sequenlist r[],int n)
{
	for(int i=1;i<=n;i++)
		printf("%5d",r[i].key);
	printf("\n");
}

//���˫�����������㷨
void dbubblesort(sequenlist r[],int n)
{
	int low=1,high=n,flag,i,temp;
	while (low<high)
	{
		flag=0;//flag����������ݽ��� 
		for(i=low;i<high;i++)
		{
			if(r[i].key>r[i+1].key)
			{
				temp=r[i].key;
				r[i].key=r[i+1].key;
				r[i+1].key=temp;
				flag=1;
			}	
		}
		if(flag==0)break;
		high--;
		for(i=high;i>low;i--)
		{
			if(r[i].key<r[i-1].key)
			{
				temp=r[i].key;
				r[i].key=r[i-1].key;
				r[i-1].key=temp;
			} 
		}
		low++; 
	}
}





