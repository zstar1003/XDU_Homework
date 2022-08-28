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
typedef struct			//��������
{
	datatype ch;		//���ַ�
	WeightType weight;	// ������Ÿ�������Ȩֵ //
	int parent, LChild, RChild;	//ָ��˫�ס����ӽ���ָ�� //
} hufmtree;

typedef struct			//����
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

//��ʼ������ 
int InitQueue(SeqQueue * Q)
{
	if (!Q)
		return 1;
	Q->tag = Empty;
	Q->front = Q->rear = 0;
	Q->length = 0;
	return 0;
}

//��� 
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

//���� 
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

// �Թ����������򣬲�ͳ��Ҷ������
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
	return i;		//����Ҷ�Ӹ���
}

//�������0-1�ַ������
char **CrtHuffmanCode(hufmtree * ht, short LeafNum)
/*��Ҷ�ӽ�㵽����������ÿ��Ҷ�ӽ���Ӧ�Ĺ���������*/
{
	char *cd, **hc;		//����
	int i, start, p, last;

	hc = (char **)malloc((LeafNum) * sizeof(char *));	/*����n�������ͷָ�� */

	if (1 == LeafNum)	//ֻ��һ��Ҷ�ӽڵ�ʱ 
	{
		hc[0] = (char *)malloc((LeafNum + 1) * sizeof(char));
		strcpy(hc[0], "0");
		return hc;
	}

	cd = (char *)malloc((LeafNum + 1) * sizeof(char));	/*������ǰ����Ĺ����ռ� */
	cd[LeafNum] = '\0';	/*����������λ��ű��룬���ȴ�ű�������� */
	for (i = 0; i < LeafNum; i++) {	/*��n��Ҷ�ӽ���Ӧ�Ĺ��������� */
		start = LeafNum;	/*��ʼ��������ʼָ�� */
		last = i;
		for (p = ht[i].parent; p != 0; p = ht[p].parent) {	/*��Ҷ�ӵ����������� */
			if (ht[p].LChild == last)
				cd[--start] = '0';	/*���֧��0 */
			else
				cd[--start] = '1';	/*�ҷ�֧��1 */
			last = p;
		}
		hc[i] = (char *)malloc((LeafNum - start) * sizeof(char));	/*Ϊ��i���������ռ� */
		strcpy(hc[i], &cd[start]);
		printf("%3d�� %3c �볤:%2d;����:%s\n", ht[i].ch, ht[i].ch,
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
		ht[i].ch = (datatype)i;	/*1-n��ch Ϊ�ַ�����ʼ�� */
	}

	for (*FileLength = 0; !feof(fp); ++(*FileLength)) {
		ch = fgetc(fp);	    
		ht[ch].weight++;
	}
	--(*FileLength);	//ȥ���ļ�������ĳ���
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
	/*      ------------��ʼ����ϣ�*/
	for (i = *n; i < m; i++)	//������Ҷ�ӽ��,����������
	{			//��ht[0]~ht[i-1]�ķ�Χ��ѡ������parentΪ0��weight��С�Ľ�㣬����ŷֱ�ֵ��s1��s2����
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

//�Ӷ�����ȡ8���ַ���0��1����ת����һ���ֽ�
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

//�������̣����볤��
void MaxMinLength(FILE * File, hufmtree * ht, char **hc, short NLeaf, datatype * Max,
	     datatype * Min)
{
	int i;
	datatype length;

	*Max = *Min = strlen(hc[0]);
	for (i = 0; i < NLeaf; i++) {
		length = strlen(hc[i]);
		fwrite(&ht[i].ch, sizeof(datatype), 1, File);	//�ַ��Ͷ�Ӧ��
		fwrite(&length, sizeof(datatype), 1, File);	//���볤��д���ļ�
		if (length > *Max)
			*Max = length;
		if (length < *Min)
			*Min = length;
	}
}

//�ѳ��ֹ����ַ��������ѹ��д���ļ�
short CodeToFile(FILE * fp, char **hc, short n, SeqQueue * Q, datatype * length)
{
	int i;
	char *p;
	datatype j, bits;
	short count = 0;

	for (i = 0; i < n; i++)	// ��n��Ҷ��ѹ����д���ļ�
	{
		for (p = hc[i]; '\0' != *p; p++)
			In_seqQueue(Q, *p);

		while (Q->length > 8) {	// 
			bits = GetBits(Q);	//����8��Ԫ��
			fputc(bits, fp);
			count++;
		}
	}			

	*length = Q->length;
	i = 8 - *length;
	bits = GetBits(Q);	//ȡ8������Ӳ���
	for (j = 0; j < i; j++)
		bits = bits << 1;
	fputc(bits, fp);
	count++;		

	InitQueue(Q);
	return count;
}

//ѹ��
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
		printf("���벻��Ϊ�գ�����������:");
		scanf("%s", rename);
	}

	compressFile = fopen(rename, "wb");
	fp = fopen(desFile, "rb");	//ԭ�ļ�

	if (!fp || !compressFile) {
		puts("Cannot open file.");
		return;
	}

	ht = CreatHFM(fp, &LeafNum, &FileLength);//������������,ͳ��Ҷ�Ӹ�����ԭ�ļ�����
	if (!FileLength) {	
		fclose(fp);
		fclose(compressFile);
		free(ht);
		return;
	}
	Q = (SeqQueue *) malloc(sizeof(SeqQueue));
	InitQueue(Q);		//SEEK_SET:�ļ���ͷ SEEK_CUR:��ǰλ�� SEEK_END:�ļ���β
	hc = CrtHuffmanCode(ht, LeafNum);	//ȡ�ù�����0��1����,hc�ĳ���ΪLeafNum
	//MapΪ��ȡ����ö�λ���ٽ���ȫ��(256��)//
	Map = (char **)malloc(N * sizeof(char *));	//�ַ������
	if (!Map) {
		puts("����ռ�ʧ��");
		return;
	}

	for (i = 0; i < N; i++)	//��ʼ��
		Map[i] = NULL;

	for (i = 0; i < LeafNum; i++)// ��λ������ָ������Map[256]
		Map[(int)(ht[i].ch)] = hc[i];
	fseek(compressFile, sizeof(WeightType) + sizeof(short) + 6 * sizeof(datatype), SEEK_SET);//��ռ��λ��,������ѹ��Ҷ�ӱ���ʣ����������볤
	MaxMinLength(compressFile, ht, hc, LeafNum, &maxLen, &minLen);	//�����봮����,˳����д�ַ���Ӧ���볤
	free(ht);
	codeNum = CodeToFile(compressFile, hc, LeafNum, Q, &finalLength);	//���ַ�ת��������Ʊ���д���ļ�,����ѹ�ɶ��ٸ�
	rewind(compressFile);	//ʹ�ļ�ָ���Ƶ���ʼ
	fseek(compressFile, sizeof(WeightType) + sizeof(datatype), SEEK_SET);
	fwrite(&LeafNum, sizeof(short), 1, compressFile);	//д��Ҷ�Ӹ���
	fwrite(&maxLen, sizeof(datatype), 1, compressFile);	//��봮����
	fwrite(&minLen, sizeof(datatype), 1, compressFile);	//����봮����
	fwrite(&codeNum, sizeof(short), 1, compressFile);	//��дҶ�ӱ���ѹ���ٸ�
	fwrite(&finalLength, sizeof(datatype), 1, compressFile);	//���ʣ�µ� 
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
			bits = GetBits(Q);	//����8��Ԫ��,�ϳ�һ���ֽ�
			fputc(bits, compressFile);    
			Length++;	
		} 
	}		
	finalLength = Q->length;
	n = 8 - finalLength;
	bits = GetBits(Q);
	for (i = 0; i < n; i++)
		bits = bits << 1;	//�ԡ�0����
	fwrite(&bits, sizeof(datatype), 1, compressFile);
	Length++;
	rewind(compressFile);//ԭ�ļ���fwrite(&FileLength,sizeof(WeightType),1,compressFile);
	fwrite(&Length, sizeof(WeightType), 1, compressFile);	//ѹ����ĳ�
	fwrite(&finalLength, sizeof(char), 1, compressFile);	//���Ĵ���

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

//�Ѷ������ַ���ת����8��0��1�ַ��������
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

//��ѹ��
void UnCompress()
{
	datatype *str, MaxLength, MinLength, ch, *num, finalLength = 0, final = 0;
	FILE *cf, *uf;
	short NLeaf, Ncom;
	char CName[N], UName[N], **hc, *p, x, *buf;
	SeqQueue *Q = NULL;
	int i, j;
	WeightType srcLen = 0, thisFile = 0;

	fflush(stdin);		//������뻺������
	printf("filename to be uncompressed:");
	scanf("%s", CName);
	if (CName[0] == '\0') {
		printf("try again:");
		scanf("%s", CName);
		fflush(stdin);
	}
	fflush(stdin);		//������뻺������
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
	//ѹ��Ҷ������x�����ʣ��
	if (!Q || !str || !num || !buf) {
		puts("Memery error.");
		exit(1);
	}
	InitQueue(Q);		//��ʼ��

	fread(&Ncom, sizeof(short), 1, cf);
	fread(&final, sizeof(datatype), 1, cf);

	for (i = 0; i < NLeaf; i++)	//��ȡҶ�Ӽ����볤
	{
		fread(&str[i], sizeof(datatype), 1, cf);
		fread(&num[i], sizeof(datatype), 1, cf);
	}

	hc = (char **)malloc((NLeaf) * sizeof(char *));	//hcΪ������ָ������
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
		//����г����봮�����ֵ       
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
	system("cls");//���� 
	system("color f5");
	while (1) {
		puts("");
		puts("         ��ӭʹ�ù�����ѹ����");
		puts("         -------------------------------------------------");
		puts("                                                        ");
		printf("         1.ѹ��  \n");
		puts("                                                        ");
		printf("         2.��ѹ�� \n");
		puts("                                                       ");
		printf("         3.�˳�   \n");
		puts("                                                       ");
		puts("         -------------------------------------------------");
		printf("         ��ѡ�� (1 to 3):");
		choice = getchar();

		puts("");
		getchar();
		fflush(stdin);	//������뻺�����򣬷������Ļس�������Ϊ�����β����scanf���룬�˺�����stdio.h��
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
		default:printf("         ��ѡ����Чѡ��\n");
		}
	}

	return 0;
}
