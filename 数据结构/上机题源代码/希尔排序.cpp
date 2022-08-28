//ϣ������ĳ������
#include<stdio.h>
//˳���ṹ���Ͷ���
typedef int datatype;
typedef struct{
	int key;
	datatype data;
}rectype;
const int N=10;
const int D1=5;

void create(rectype[],int);
void print(rectype[],int);
void shellsort(rectype[],int[]);

int main()
{
    rectype r[N+D1];//D1��Ԫ�ش�ż����ڣ�N��Ԫ�ش�ż�¼
    int d[3]={5,3,1};//����3�˵�����	
	create(r,N);//������ż�¼��˳���
	printf("����ǰ�����ݣ�");
	print(r,N);//�������ǰ�ļ�¼��
	shellsort(r,d);//ϣ������
	printf("���������ݣ�");
	print(r,N);//��������ļ�¼��
}

//����˳���
void create(rectype r[],int n)
{
	printf("����10����������");
	for(int i=0;i<n;i++)
		scanf("%d",&r[D1+i].key);
}

//���˳���
void print(rectype r[],int n)
{
	for(int i=0;i<n;i++)
		printf("%5d",r[D1+i].key);
	printf("\n");
}

//���ϣ�������㷨
void shellsort(rectype r[],int d[])
{
	
	int i,j,k;
	//����λ��0
	for(i=0;i<D1;i++)
	{
		r[i].key=-10000;
	} 
	for(k=0;k<3;k++)
	{
		for(i=d[k]+1;i<=N;i++)
		{
			if(r[i+D1-1].key<r[i-d[k]+D1-1].key)
			{
				r[0].key=r[i+D1-1].key;
				j=i-d[k];
				while(r[0].key<r[j+D1-1].key)
				{
					r[j+d[k]+D1-1].key=r[j+D1-1].key;
					j=j-d[k];
				}
				r[j+d[k]+D1-1].key=r[0].key;
			}
		}
	}
}

