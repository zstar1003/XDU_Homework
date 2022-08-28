#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define  N 256
#define Maxsize 80
#define  SOME 1
#define Empty 0
#define FULL -1

typedef unsigned long int WeightType;
typedef unsigned char datatype;
typedef struct			//哈夫曼树
{
	datatype ch;		//存字符
	WeightType weight;	// 用来存放各个结点的权值 //
	int parent, LChild, RChild;	//指向双亲、孩子结点的指针 //
} hufmtree;

typedef struct			//队列
{
	int tag;
	int front;
	int rear;
	datatype length;
	char elem[Maxsize];
} SeqQueue;

typedef struct		
{
	char bits[N];
	int start;
	datatype data;
}codetype;

void writeFile();
void printHFM(hufmtree * ht, int n);
void code(char **hc, int n, unsigned char *ch);

//初始化队列 
int InitQueue(SeqQueue * Q)
{
	if (!Q)
		return 1;
	Q->tag = Empty;
	Q->front = Q->rear = 0;
	Q->length = 0;
	return 0;
}

//入队 
int In_seqQueue(SeqQueue * Q, char x)
{
	if (Q->front == Q->rear && Q->tag == SOME)
		return FULL;	

	Q->elem[Q->rear] = x;	
	Q->rear = (Q->rear + 1) % Maxsize;
	Q->length++;
	Q->tag = SOME;
	return SOME;
}

//出队 
int Out_Queue(SeqQueue * Q, char *x)
{
	if (Q->tag == Empty)
		return Empty;

	*x = Q->elem[Q->front];
	Q->length--;
	Q->front = (Q->front + 1) % Maxsize;

	if (Q->front == Q->rear)
		Q->tag = Empty;

	return SOME;
}


void SelectMinTree(hufmtree * ht, int n, int *k)
{
	int i, temp;
	WeightType min;
	for (i = 0; i <= n; i++) {
		if (ht[i].parent==0) {
			min = ht[i].weight;	
			temp = i;
			break;
		}
	}
	for (i++; i <= n; i++) {
		if (0 == ht[i].parent && ht[i].weight < min) {
			min = ht[i].weight;
			temp = i;
		}
	}
	*k = temp;
}

// 对哈夫曼树排序，并统计叶子数量
int SortTree(hufmtree * ht)
{
	short i, j;
	hufmtree tmp;

	for (i = N - 1; i >= 0; i--) {
		for (j = 0; j < i; j++)
			if (ht[j].weight < ht[j + 1].weight) {
				tmp = ht[j];
				ht[j] = ht[j + 1];
				ht[j + 1] = tmp;
			}
	}
	for (i = 0; i < N; i++)
		if (0 == ht[i].weight)
			return i;
	return i;		//返回叶子个数
}

//求哈夫曼0-1字符编码表
char **CrtHuffmanCode(hufmtree * ht, short LeafNum)
/*从叶子结点到根，逆向求每个叶子结点对应的哈夫曼编码*/
{
	char *cd, **hc;		//容器
	int i, start, p, last;

	hc = (char **)malloc((LeafNum) * sizeof(char *));	/*分配n个编码的头指针 */

	if (1 == LeafNum)	//只有一个叶子节点时 
	{
		hc[0] = (char *)malloc((LeafNum + 1) * sizeof(char));
		strcpy(hc[0], "0");
		return hc;
	}

	cd = (char *)malloc((LeafNum + 1) * sizeof(char));	/*分配求当前编码的工作空间 */
	cd[LeafNum] = '\0';	/*从右向左逐位存放编码，首先存放编码结束符 */
	for (i = 0; i < LeafNum; i++) {	/*求n个叶子结点对应的哈夫曼编码 */
		start = LeafNum;	/*初始化编码起始指针 */
		last = i;
		for (p = ht[i].parent; p != 0; p = ht[p].parent) {	/*从叶子到根结点求编码 */
			if (ht[p].LChild == last)
				cd[--start] = '0';	/*左分支标0 */
			else
				cd[--start] = '1';	/*右分支标1 */
			last = p;
		}
		hc[i] = (char *)malloc((LeafNum - start) * sizeof(char));	/*为第i个编码分配空间 */
		strcpy(hc[i], &cd[start]);
		printf("%3d号 %3c 码长:%2d;编码:%s\n", ht[i].ch, ht[i].ch,
		       LeafNum - start, &cd[start]);
	}			
	free(cd);	
	return hc;
}

hufmtree *CreatHFM(FILE * fp, short *n, WeightType * FileLength)
{
	hufmtree *ht = NULL;
	int i, m, s1, s2;
	datatype ch;

	ht = (hufmtree *) malloc(2 * N * sizeof(hufmtree));
	if (!ht)
		exit(1);

	for (i = 0; i < N; i++) {
		ht[i].weight = 0;
		ht[i].ch = (datatype)i;	/*1-n号ch 为字符，初始化 */
	}

	for (*FileLength = 0; !feof(fp); ++(*FileLength)) {
		ch = fgetc(fp);	    
		ht[ch].weight++;
	}
	--(*FileLength);	//去掉文件结束后的长度
	*n = SortTree(ht);
	m = *n * 2 - 1;		
	if (1 == *n) {
		ht[0].parent = 1;
		return ht;
	} else if (0 > *n)
		return NULL;

	for (i = m - 1; i >= 0; i--) {
		ht[i].LChild = 0;
		ht[i].parent = 0;
		ht[i].RChild = 0;
	}
	/*      ------------初始化完毕！*/
	for (i = *n; i < m; i++)	//创建非叶子结点,建哈夫曼树
	{			//在ht[0]~ht[i-1]的范围内选择两个parent为0且weight最小的结点，其序号分别赋值给s1、s2返回
		SelectMinTree(ht, i - 1, &s1);
		ht[s1].parent = i;
		ht[i].LChild = s1;

		SelectMinTree(ht, i - 1, &s2);
		ht[s2].parent = i;
		ht[i].RChild = s2;

		ht[i].weight = ht[s1].weight + ht[s2].weight;
	}	
	return ht;
}

//从队列里取8个字符（0、1），转换成一个字节
datatype GetBits(SeqQueue * Q)
{
	datatype i, bits = 0;
	char x;

	for (i = 0; i < 8; i++) {
		if (Out_Queue(Q, &x) != Empty) {
			if ('0' == x)
				bits = bits << 1;
			else
				bits = (bits << 1) | 1;
		} else
			break;
	}		
	return bits;
}

//求最长（最短）编码长度
void MaxMinLength(FILE * File, hufmtree * ht, char **hc, short NLeaf, datatype * Max,
	     datatype * Min)
{
	int i;
	datatype length;

	*Max = *Min = strlen(hc[0]);
	for (i = 0; i < NLeaf; i++) {
		length = strlen(hc[i]);
		fwrite(&ht[i].ch, sizeof(datatype), 1, File);	//字符和对应的
		fwrite(&length, sizeof(datatype), 1, File);	//编码长度写进文件
		if (length > *Max)
			*Max = length;
		if (length < *Min)
			*Min = length;
	}
}

//把出现过的字符编码表经过压缩写进文件
short CodeToFile(FILE * fp, char **hc, short n, SeqQueue * Q, datatype * length)
{
	int i;
	char *p;
	datatype j, bits;
	short count = 0;

	for (i = 0; i < n; i++)	// 将n个叶子压缩并写入文件
	{
		for (p = hc[i]; '\0' != *p; p++)
			In_seqQueue(Q, *p);

		while (Q->length > 8) {	// 
			bits = GetBits(Q);	//出队8个元素
			fputc(bits, fp);
			count++;
		}
	}			

	*length = Q->length;
	i = 8 - *length;
	bits = GetBits(Q);	//取8个如果队不空
	for (j = 0; j < i; j++)
		bits = bits << 1;
	fputc(bits, fp);
	count++;		

	InitQueue(Q);
	return count;
}

//压缩
void Compress()
{
	char desFile[80], rename[80];
	datatype maxLen, minLen, ch, bits, n, finalLength;
	int i;
	short LeafNum, codeNum;
	WeightType count = 0, Length = 0, FileLength;
	FILE *fp, *compressFile;
	SeqQueue *Q;
	hufmtree *ht = NULL;
	char **hc = NULL, **Map = NULL, *p;

	printf("filename to be compressed:");
	scanf("%s", desFile);

	printf("filename after compressed:");
	scanf("%s", rename);
	if ('\0' == rename[0]) {
		printf("输入不能为空，请重新输入:");
		scanf("%s", rename);
	}

	compressFile = fopen(rename, "wb");
	fp = fopen(desFile, "rb");	//原文件

	if (!fp || !compressFile) {
		puts("Cannot open file.");
		return;
	}

	ht = CreatHFM(fp, &LeafNum, &FileLength);//创建哈夫曼树,统计叶子个数和原文件长度
	if (!FileLength) {	
		fclose(fp);
		fclose(compressFile);
		free(ht);
		return;
	}
	Q = (SeqQueue *) malloc(sizeof(SeqQueue));
	InitQueue(Q);		//SEEK_SET:文件开头 SEEK_CUR:当前位置 SEEK_END:文件结尾
	hc = CrtHuffmanCode(ht, LeafNum);	//取得哈夫曼0、1编码,hc的长度为LeafNum
	//Map为了取编码好定位，再建立全部(256个)//
	Map = (char **)malloc(N * sizeof(char *));	//字符编码表
	if (!Map) {
		puts("申请空间失败");
		return;
	}

	for (i = 0; i < N; i++)	//初始化
		Map[i] = NULL;

	for (i = 0; i < LeafNum; i++)// 定位，编码指针数组Map[256]
		Map[(int)(ht[i].ch)] = hc[i];
	fseek(compressFile, sizeof(WeightType) + sizeof(short) + 6 * sizeof(datatype), SEEK_SET);//先占个位置,等下填压缩叶子编码剩几个和最长编码长
	MaxMinLength(compressFile, ht, hc, LeafNum, &maxLen, &minLen);	//获得最长码串长度,顺便填写字符对应编码长
	free(ht);
	codeNum = CodeToFile(compressFile, hc, LeafNum, Q, &finalLength);	//把字符转成其二进制编码写入文件,返回压成多少个
	rewind(compressFile);	//使文件指针移到开始
	fseek(compressFile, sizeof(WeightType) + sizeof(datatype), SEEK_SET);
	fwrite(&LeafNum, sizeof(short), 1, compressFile);	//写入叶子个数
	fwrite(&maxLen, sizeof(datatype), 1, compressFile);	//最长码串长度
	fwrite(&minLen, sizeof(datatype), 1, compressFile);	//最短码串长度
	fwrite(&codeNum, sizeof(short), 1, compressFile);	//填写叶子编码压多少个
	fwrite(&finalLength, sizeof(datatype), 1, compressFile);	//最后剩下的 
	fseek(compressFile, 2 * LeafNum * sizeof(datatype) + codeNum, SEEK_CUR);
	fseek(fp, 0, SEEK_SET);
	printf("Please wait a minute,compressing...");

	while (count < FileLength) {
		ch = fgetc(fp);	      
		++count;
		for (p = Map[ch]; *p != '\0'; p++)
			In_seqQueue(Q, *p);

		while (Q->length > 8)
		{
			bits = GetBits(Q);	//出队8个元素,合成一个字节
			fputc(bits, compressFile);    
			Length++;	
		} 
	}		
	finalLength = Q->length;
	n = 8 - finalLength;
	bits = GetBits(Q);
	for (i = 0; i < n; i++)
		bits = bits << 1;	//以‘0’补
	fwrite(&bits, sizeof(datatype), 1, compressFile);
	Length++;
	rewind(compressFile);//原文件长fwrite(&FileLength,sizeof(WeightType),1,compressFile);
	fwrite(&Length, sizeof(WeightType), 1, compressFile);	//压缩后的长
	fwrite(&finalLength, sizeof(char), 1, compressFile);	//最后的串长

	Length = Length + 12 + codeNum;	
	if (Length >= FileLength)
		puts("\nCompression rate: 0.0%");
	else
		printf("\nCompression rate: %.2lf%c\n",
		       (double)((FileLength -
				 Length) / (double)FileLength) * 100.0, '%');
	fclose(fp);
	fclose(compressFile);
	free(Q);
	free(hc);
	free(Map);
}

//把读出的字符，转换成8个0、1字符串并入队
void ToQueue(SeqQueue * Q, datatype ch)
{
	int i;
	datatype temp;

	for (i = 0; i < 8; i++) {
		temp = ch << i;
		temp = temp >> 7;
		if (1 == temp)
			In_seqQueue(Q, '1');	
		else
			In_seqQueue(Q, '0');
	}	
}

//解压缩
void UnCompress()
{
	datatype *str, MaxLength, MinLength, ch, *num, finalLength = 0, final = 0;
	FILE *cf, *uf;
	short NLeaf, Ncom;
	char CName[N], UName[N], **hc, *p, x, *buf;
	SeqQueue *Q = NULL;
	int i, j;
	WeightType srcLen = 0, thisFile = 0;

	fflush(stdin);		//清空输入缓冲区域
	printf("filename to be uncompressed:");
	scanf("%s", CName);
	if (CName[0] == '\0') {
		printf("try again:");
		scanf("%s", CName);
		fflush(stdin);
	}
	fflush(stdin);		//清空输入缓冲区域
	printf("filename after uncompressed:");
	scanf("%s", UName);
	if (UName[0] == '\0') {
		printf("try again:");
		scanf("%s", UName);
	}

	cf = fopen(CName, "rb");
	uf = fopen(UName, "wb");
	if (!cf || !uf) {
		puts("Cannot open files.");
		return;
	}

	fread(&srcLen, sizeof(WeightType), 1, cf);
	fread(&finalLength, 1, 1, cf);	
	fread(&NLeaf, sizeof(short), 1, cf);
	fread(&MaxLength, sizeof(datatype), 1, cf);
	fread(&MinLength, sizeof(datatype), 1, cf);
	Q = (SeqQueue *) malloc(sizeof(SeqQueue));

	buf = (char *)malloc((2 + MaxLength * sizeof(char)));
	str = (datatype *) malloc(NLeaf * sizeof(datatype));
	num = (datatype *) malloc(NLeaf * sizeof(datatype));
	//压缩叶子数量x和最后剩长
	if (!Q || !str || !num || !buf) {
		puts("Memery error.");
		exit(1);
	}
	InitQueue(Q);		//初始化

	fread(&Ncom, sizeof(short), 1, cf);
	fread(&final, sizeof(datatype), 1, cf);

	for (i = 0; i < NLeaf; i++)	//读取叶子及其码长
	{
		fread(&str[i], sizeof(datatype), 1, cf);
		fread(&num[i], sizeof(datatype), 1, cf);
	}

	hc = (char **)malloc((NLeaf) * sizeof(char *));	//hc为编码表的指针数组
	if (!hc)
		exit(1);

	--Ncom;	
	for (j = i = 0; i < Ncom; i++) {
		ch = fgetc(cf);	
		ToQueue(Q, ch);	
		while (Q->length > MaxLength) {
			hc[j] = p = (char *)malloc(1 + num[j]);
			for (ch = 0; ch < num[j]; ch++) {
				Out_Queue(Q, &x);
				*p++ = x;
			}
			*p = '\0';	
			j++;
		}
	}
	ch = fgetc(cf);	
	ToQueue(Q, ch);
	final = 8 - final;
	while (Q->length > final) {
		p = hc[j] = (char *)malloc(1 + num[j]);
		for (ch = 0; ch < num[j]; ch++) {
			Out_Queue(Q, &x);
			*p++ = x;
		}
		*p = '\0';	
		j++;
	}

	InitQueue(Q);
	--srcLen;
	--MinLength;
	printf("Please wait a minute,uncompressing...");
	while (thisFile < srcLen) {
		ch = fgetc(cf);
		ToQueue(Q, ch);
		thisFile++;	
		//后队列长于码串的最大值       
		while (Q->length > MaxLength) {
			for (i = 0; i < MinLength; i++) {
				Out_Queue(Q, &x);
				buf[i] = x;
			}
			for (; i < MaxLength; i++) {
				Out_Queue(Q, &x);
				buf[i] = x;
				buf[i + 1] = '\0';
				for (j = 0; j < NLeaf; j++) {
					if (i + 1 == num[j]
					    && 0 == strcmp(hc[j], buf)) {
						ch = str[j];	
						fputc(ch, uf);	
						break;
					}
				}
				if (j < NLeaf)
					break;
			}
		}		
	}

	ch = fgetc(cf);		
	ToQueue(Q, ch);

	finalLength = 8 - finalLength;
	while (Q->length > finalLength) {
		for (i = 0; i < MinLength; i++) {
			Out_Queue(Q, &x);
			buf[i] = x;
		}
		for (; i < MaxLength; i++) {
			Out_Queue(Q, &x);
			buf[i] = x;
			buf[i + 1] = '\0';
			for (j = 0; j < NLeaf; j++) {
				if (i + 1 == num[j] && 0 == strcmp(hc[j], buf)) {
					ch = str[j];	
					fputc(ch, uf);	
					break;
				}
			}
			if (j < NLeaf)
				break;
		}
	}
	free(Q);
	free(str);
	free(num);
	free(hc);
	fclose(uf);
	fclose(cf);
}

int main(int argc, char *argv[])
{
	char choice;
	system("cls");//清屏 
	system("color f5");
	while (1) {
		puts("");
		puts("         欢迎使用哈夫曼压缩器");
		puts("         -------------------------------------------------");
		puts("                                                        ");
		printf("         1.压缩  \n");
		puts("                                                        ");
		printf("         2.解压缩 \n");
		puts("                                                       ");
		printf("         3.退出   \n");
		puts("                                                       ");
		puts("         -------------------------------------------------");
		printf("         请选择 (1 to 3):");
		choice = getchar();

		puts("");
		getchar();
		fflush(stdin);	//清空输入缓冲区域，否则键入的回车符将作为程序结尾处的scanf输入，此函数在stdio.h中
		switch (choice) {
		case '1':
			Compress();
			printf("\nPress Enter to continue...");
			getchar();
			break;
		case '2':
			UnCompress();
			printf("\nPress Enter to continue...");
			getchar();
			break;
		case '3':
			return 0;
			break;
		default:printf("         请选择有效选项\n");
		}
	}

	return 0;
}
