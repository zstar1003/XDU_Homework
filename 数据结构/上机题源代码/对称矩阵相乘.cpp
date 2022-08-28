//�Գƾ�����˵ĳ������
#include<stdio.h>
#include<malloc.h>
//����ṹ���͵Ķ���.h
const int n=3;
const int size=n*(n+1)/2;
typedef int datatype;
typedef struct{
	datatype A[size],B[size],C[n][n];
}array;

void input(datatype[]);
void output(datatype[][n]);
void mult(array*);

int main()
{
	array*pa;
	pa=(array*)malloc(sizeof(array));
	printf("����Ϊ�����������A��������:\n");
    input(pa->A);//����Ϊ�����������A��������
	printf("����Ϊ�����������B��������:\n");
	input(pa->B);//����Ϊ�����������B��������
	mult(pa);
	output(pa->C);//�������C
}

//�Գƾ��������
void input(datatype x[])
{
	for(int i=0;i<size;i++)
		scanf("%d",&x[i]);
}

//��������
void output(datatype x[][n])
{
	for(int i=0;i<n;i++){
		for(int j=0;j<n;j++)
			printf("%5d",x[i][j]); 
		printf("\n");
	}
}

datatype value(int a[], int i, int j)
{
    if(i>=j)
        return a[(i*(i+1))/2+j];
    else
        return a[(j*(j+1))/2+i];
}
//��ӶԳƾ�������㷨
void mult(array*pa)
{
	int i,j,k,sum;
	for (i=0;i<n;i++)
	{
		for(j=0;j<n;j++)
		{
			sum=0;
			for(k=0;k<n;k++)
			{
				sum=sum+value(pa->A,i,k)*value(pa->B,k,j);
			}
			pa->C[i][j]=sum;
		}
	}
}



