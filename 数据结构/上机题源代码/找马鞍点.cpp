//������������
#include<stdio.h>
#include<malloc.h>
//����Ľṹ���Ͷ���
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

//����������㷨
void minmax(array*pa)
{
	int i,j;
	for(i=1;i<=m;i++)//�ҳ�ÿһ�е���Сֵ 
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
	for(j=1;j<=n;j++)//�ҳ�ÿһ�е����ֵ 
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

