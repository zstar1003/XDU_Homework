//ѭ��������ӳ���
#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include<string.h> 
//ѭ�����еĽṹ���Ͷ���
const int m=5;
typedef char datatype;
typedef struct
{   datatype *sequ[m];
    int  rear, quelen;
}qu;
void setnull(qu*);
void enqueue(qu*, char x[]);
datatype *dequeue(qu*);

int main()
{  qu *sq;
   datatype *p;
   char x[20]="";
   int key;
   sq=(qu*)malloc(sizeof(qu));
   setnull(sq);
   do
   {  printf("1.Enter Queue   2.Delete Queue   -1.Quit:");
      scanf("%d",&key);
      switch(key)
      {  case 1:  printf("Enter the Data:"); 
      			  fflush(stdin);
	  			  gets(x);
		          enqueue(sq,x);  break;
	     case 2:  p=dequeue(sq);
		          if(p!=NULL) printf("%s\n",p);
		          break;
	     case -1: exit(0);
      }
   }while(1);
}

//�ÿն�
void setnull(qu *sq)
{  sq->rear=m-1;
   sq->quelen=0;
}

//�������㷨
void enqueue(qu *sq,char x[20])
{
	if (sq->quelen ==m)
	{
		printf("��������");
	}
	else
	{
		sq->rear=(sq->rear+1)%m;
		sq->sequ[sq->rear]=(char*)malloc(100*sizeof(char)); 
		strcpy(sq->sequ[sq->rear],x);
		sq->quelen++;
	}
}
//��ӳ����㷨
datatype *dequeue(qu *sq)
{
	datatype *temp;
	if (sq->quelen ==0)
	{
		printf("�����ѿ�");
		return NULL;
	}
	else{
		temp=(datatype*)malloc(sizeof(datatype)*20);
		temp=sq->sequ[(sq->rear-sq->quelen+m+1)%m];
		sq->quelen--;
		return(temp);
	} 
}






