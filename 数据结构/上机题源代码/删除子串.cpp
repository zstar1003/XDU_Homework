//ɾ���Ӵ��ĳ������
#include<stdio.h>
#include<string.h>
#include<malloc.h>
//˳�򴮵Ľṹ���Ͷ���
#define maxsize 100
typedef struct
{  
	char str[maxsize];
    int len;
}seqstring;

void strPut(seqstring*);
void strDelete(seqstring*,int,int);
int main()
{
	seqstring*S;
	int i,m;
	S=(seqstring*)malloc(sizeof(seqstring));
	printf("���봮��"); gets(S->str);
	S->len=strlen(S->str);
	strPut(S);
	printf("ɾ���Ŀ�ʼλ��:");scanf("%d",&i);
	printf("ɾ�����ַ�����:");scanf("%d",&m);
	strDelete(S,i,m);
	strPut(S);
}

//�����
void strPut(seqstring*S)
{
	int i;
	for(i=0;i<S->len;i++)
		printf("%c",S->str[i]);
	printf("\n");
}

//���ɾ���Ӵ��㷨// m<=S->len-m+1
void strDelete(seqstring*S,int i,int m)
{
	char temp[maxsize]="";
	char temp2[maxsize]="";
	if (i>=1 && i<=S->len)
	{
		if(i+m<=S->len)
		{
			strncpy(temp,S->str,i-1);
			strncpy(temp2,S->str+i+m-1,S->len-m-i+1);
			strcat(temp,temp2);
			strcpy(S->str,temp);
			S->len=S->len-m+1;
		}
		else 
		{
			strncpy(temp,S->str,i-1);
			strcpy(S->str,temp);
			S->len=i;
		}
	}
	else
	{
		printf("ɾ��λ�ó��������ȣ�û���ַ���ɾ��,SΪ");
	}
}

