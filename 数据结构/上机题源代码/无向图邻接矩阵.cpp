//����ͼ�ڽӾ������������ĳ������
#include<stdio.h>
//ͼ���ڽӾ������Ͷ���
const int n=8;
const int e=10;
typedef char vextype;
typedef int adjtype;
typedef struct
{
	vextype vexs[n];
	adjtype arcs[n][n];
}graph;
graph*g=new graph;
void creatgraph();
void dfsa(int);
int visited[n];

int main()
{
	creatgraph();
	int i;
	while(1)
	{
		for(i=0;i<n;i++)
			visited[i]=0;
		printf("�����������ţ�0-7��������-1������");
		scanf("%d",&i);
		if(i==-1) break;
	    dfsa(i);
	}
}

//��������ͼ�ڽӾ���
void creatgraph()
{
	int i,j,k;
	char ch;
	printf("����8��������ַ�������Ϣ:\n");
	for(i=0;i<n;i++)
		if((ch=getchar())!='\n') g->vexs[i]=ch;
	for(i=0;i<n;i++)
		for(j=0;j<n;j++)
			g->arcs[i][j]=0;
	printf("����10���ߵ����յ�i,j:\n"); 
	for(k=0;k<e;k++)
	{
		scanf("%d,%d",&i,&j); //������Ŵ�0��ʼ
		g->arcs[i][j]=g->arcs[j][i]=1;
	}
}

//�������������������㷨
void dfsa(int i)
{
	int j;
	printf("node:%c\n",g->vexs[i]);
	visited[i]=1;
	for(j=0;j<n;j++)
	{
		if((g->arcs[i][j]==1)&&(visited[j]==0))
		{
			dfsa(j);
		}
	}
}


