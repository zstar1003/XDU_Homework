//找马鞍点程序代码
#include<stdio.h>
#include<malloc.h>
//数组的结构类型定义
const int m=3;
const int n=3;
typedef struct{
	int A[m+1][n+1];
	int max[m+1],min[n+1];
}array;
void minmax(array*);

int main()
{
	array*pa=(array*)malloc(sizeof(array));
    int i, j;
    for (i=1;i<=m;i++)
      for (j=1;j<=n;j++)
        scanf("%d",&pa->A[i][j]);
    minmax(pa);
}

//添加找马鞍点算法
void minmax(array*pa)
{
	int i,j;
	for(i=1;i<=m;i++)//找出每一行的最小值 
	{
		pa->min[i]=pa->A[i][1];
		for(j=1;j<=n;j++)
		{
			if(pa->A[i][j]<pa->min[i])
			{
				pa->min[i]=pa->A[i][j];
			}
		} 
	}
	for(j=1;j<=n;j++)//找出每一列的最大值 
	{
		pa->max[j]=pa->A[1][j];
		for(i=1;i<=m;i++)
		{
			if(pa->A[i][j]>pa->max[j])
			{
				pa->max[j]=pa->A[i][j];
			}
		} 
	}
	for(i=1;i<=m;i++)
	{
		for(j=1;j<=n;j++)
		{
			if(pa->min[i]==pa->max[j])printf("%d ",pa->min[i]);
		}
	}	
}

