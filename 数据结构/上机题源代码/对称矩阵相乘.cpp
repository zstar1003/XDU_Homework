//对称矩阵相乘的程序代码
#include<stdio.h>
#include<malloc.h>
//数组结构类型的定义.h
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
	printf("以行为主序输入矩阵A的下三角:\n");
    input(pa->A);//以行为主序输入矩阵A的下三角
	printf("以行为主序输入矩阵B的下三角:\n");
	input(pa->B);//以行为主序输入矩阵B的下三角
	mult(pa);
	output(pa->C);//输出矩阵C
}

//对称矩阵的输入
void input(datatype x[])
{
	for(int i=0;i<size;i++)
		scanf("%d",&x[i]);
}

//矩阵的输出
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
//添加对称矩阵相乘算法
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



