//循环队列入队出队
#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include<string.h> 
//循环队列的结构类型定义
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

//置空队
void setnull(qu *sq)
{  sq->rear=m-1;
   sq->quelen=0;
}

//添加入队算法
void enqueue(qu *sq,char x[20])
{
	if (sq->quelen ==m)
	{
		printf("队列已满");
	}
	else
	{
		sq->rear=(sq->rear+1)%m;
		sq->sequ[sq->rear]=(char*)malloc(100*sizeof(char)); 
		strcpy(sq->sequ[sq->rear],x);
		sq->quelen++;
	}
}
//添加出队算法
datatype *dequeue(qu *sq)
{
	datatype *temp;
	if (sq->quelen ==0)
	{
		printf("队列已空");
		return NULL;
	}
	else{
		temp=(datatype*)malloc(sizeof(datatype)*20);
		temp=sq->sequ[(sq->rear-sq->quelen+m+1)%m];
		sq->quelen--;
		return(temp);
	} 
}






