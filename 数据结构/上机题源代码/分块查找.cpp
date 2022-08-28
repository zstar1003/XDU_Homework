//分块查找的程序代码
#include<stdio.h>
//类型定义
typedef int keytype;
typedef struct
{
	keytype key;
	int low,high;
}index;
typedef struct
{
	keytype key;
}record;
const int recN=18;
const int idxN=3;

int blksearch(record[],index[],keytype,int);
int main()
{
	record r[recN]={22,12,13,8,9,20,33,42,44,38,24,48,60,58,74,49,86,53};
	index idx[idxN]={{22,0,5},{48,6,11},{86,12,17}};
	keytype key;
	int loc,i;
	printf("待查找的记录关键字表：\n");
	for(i=0;i<recN;i++)
		printf("%5d",r[i]);
	printf("\n");
	printf("输入所要查找记录的关键字：");
	scanf("%d",&key);
	loc=blksearch(r,idx,key,idxN);
	if(loc!=-1) printf("查找到，是第%d个记录。\n",loc+1);
	else printf("记录查找不到！\n");
}

//添加折半查找索引表，块内顺序查找算法
int blksearch(record r[],index idx[],keytype key,int idxN)
{
	int low,high=idxN-1,mid,i,j;
	{
		while(low<=high)
		{
			mid=(low+high)/2;
			if(idx[mid].key>=key) high=mid-1;
			else low=mid+1;
		}
		if(low<idxN)
		{
			i=idx[low].high;
			j=idx[low].low;
			while(r[i].key!=key&&i>=j)i--;
			if(i>=j)return i;
		}
		return -1;
	}
}


