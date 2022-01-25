/************赫夫曼编码************
* 哈夫曼编码( Huffman Coding )是一种编码方式,
* 哈夫曼树——即最优二叉树,带权路径长度最小的二叉树,
* 经常应用于数据压缩。
************************************/
#define _CRT_SECURE_NO_WARNINGS 1
#include<stdio.h>
#include<stdlib.h>
#include<string.h> 
#include<iostream> 
int n = 27; //字符集大小
char* c = (char*)malloc((n + 1) * sizeof(char)); //存储字符集
int* w = (int*)malloc((n + 1) * sizeof(int)); //存储字符权值

typedef struct {
	unsigned int Weight;
	unsigned int Parent, Lchild, Rchild;
} HTNode, * HuffmanTree;  //动态分配数组存储赫夫曼树

typedef char** Huffmancode;  //动态分配数组存储赫夫曼编码表

void SetChar(char* c, int* w, int n) {
	//设置待编码的字符和其对应权值
	int i;
	c[1] = ' ';
	for (i = 2; i <= n; i++) c[i] = 'A' + i - 2;
	w[1] = 186; w[2] = 64;  w[3] = 13;  w[4] = 22;  w[5] = 32;
	w[6] = 103; w[7] = 21;  w[8] = 15;  w[9] = 47;  w[10] = 57;
	w[11] = 1;  w[12] = 5;  w[13] = 32; w[14] = 20; w[15] = 57;
	w[16] = 63; w[17] = 15; w[18] = 1;  w[19] = 48; w[20] = 51;
	w[21] = 80; w[22] = 23; w[23] = 8;  w[24] = 18; w[25] = 1;
	w[26] = 16; w[27] = 1;
	//c[1] = 'a'; c[2] = 'b'; c[3] = 'c'; c[4] = 'd'; c[5] = 'e'; c[6] = 'f'; c[7] = 'g'; c[8] = 'h';
	//w[1] = 5;   w[2] = 29;  w[3] = 7;   w[4] = 8;   w[5] = 14;  w[6] = 23;  w[7] = 3;   w[8] = 11;
}//SetChar

void HuffmanCoding(HuffmanTree& HT, Huffmancode& HC, int* w, int n) {
	//创建一棵叶子结点数为n的Huffman树，求出n个字符的编码
	char* cd;
	int m, c, i, k, start, s1, s2, s3, f;
	if (n <= 1)  return;
	m = 2 * n - 1;  //总结点个数 
	HT = (HTNode*)malloc((m + 1) * sizeof(HTNode));  //0号单元未用 

	//初始化向量HT
	for (i = 1; i <= n; i++) {
		HT[i].Weight = w[i]; HT[i].Parent = 0; HT[i].Lchild = 0; HT[i].Rchild = 0;
	}
	for (i = n + 1; i <= m; i++) {
		HT[i].Weight = 0; HT[i].Parent = 0; HT[i].Lchild = 0; HT[i].Rchild = 0;
	}

	//建赫夫曼树 
	for (i = n + 1; i <= m; i++) {
		//在HT[1...i-1] 选择parent为0且weight最小的两个结点，序号为s1和s2 
		s1 = 0; s2 = 0;
		for (k = 1; k < i; k++) {
			if (HT[k].Parent == 0) {
				if (s1 == 0 || HT[s1].Weight > HT[k].Weight) s1 = k;
				if (s2 == 0 || HT[s2].Weight > HT[s1].Weight) {
					s3 = s1; s1 = s2; s2 = s3;
				}
			}
		}//Select(HT,i-1,s2,s2)
		HT[s1].Parent = i; HT[s2].Parent = i;
		HT[i].Lchild = s1; HT[i].Rchild = s2;
		HT[i].Weight = HT[s1].Weight + HT[s2].Weight;
	}

	HC = (Huffmancode)malloc((n + 1) * sizeof(char*));
	cd = (char*)malloc(n * sizeof(char)); //动态分配求编码的工作空间
	cd[n - 1] = '\0';    //编码的结束标志

	// 逐个求字符的编码,从叶子结点到根逆向求编码
	for (i = 1; i <= n; i++) {
		start = n - 1; c = i; f = HT[i].Parent;
		for (; f != 0; c = f, f = HT[f].Parent) {
			if (HT[f].Lchild == c)  cd[--start] = '0';
			else  cd[--start] = '1';
		}
		HC[i] = (char*)malloc((n - start) * sizeof(char));
		// 为第k个字符分配保存编码的空间
		strcpy(HC[i], &cd[start]);
	}
	free(cd);
}//HuffmanCoding

void write(Huffmancode& HC, int* w, int n) { 
	int j;
	int WPL = 0;
	for (j = 1; j <= n; j++) {
		printf("%c的编码：", c[j]);
		puts(HC[j]);
		WPL += strlen(HC[j]) * w[j];
	}
	printf("WPL=%d\n", WPL);
}//write

void PrintRecessedTable(HuffmanTree HT, int deep, int p) {
	//凹入表示法
	int i;
	for (i = 0; i < deep; i++)printf("   ");
	for (i = 0; i < n/2 - deep; i++)printf("***");
	printf("%4d  ", HT[p].Weight);
	if (HT[p].Lchild == 0 && HT[p].Rchild == 0)printf("%c", c[p]);
	printf("\n");
	deep++;
	if (HT[p].Lchild != 0) PrintRecessedTable(HT, deep, HT[p].Lchild);
	if (HT[p].Rchild != 0) PrintRecessedTable(HT, deep, HT[p].Rchild);
}//PrintRecessedTable

void PrintTree(HuffmanTree HT, int p, int h) {
	//直接打印树
	if (p != 0)
	{
		PrintTree(HT, HT[p].Rchild, h + 1);
		for (int i = 0; i < h; i++) printf("     ");
		printf("%d  ", HT[p].Weight);
		if (HT[p].Lchild == 0 && HT[p].Rchild == 0)printf("%c", c[p]);
		printf("\n");
		PrintTree(HT, HT[p].Lchild, h + 1);
	}
}//PrintTree

void Coding(char* original, Huffmancode HC, char* code) {
	//对原文编码
	int loc = 1, i, len;
	len = strlen(original);
	for (i = 0; i < len; i++) {
		loc = 1;
		while (c[loc] != original[i])loc++; //在HC中找到字符对应位置
		strcat(code, HC[loc]);
	}
}//Coding

void Decoding(char* original, HuffmanTree HT, char* code) {
	//解码
	int i, len, j = 2 * n - 1, oloc = 0; //j为HT中指针，oloc为明码指针
	len = strlen(code);
	for (i = 0; i < len; i++) {
		if (code[i] == '0')j = HT[j].Lchild;
		else if (code[i] == '1')j = HT[j].Rchild;
		else { printf("代码有误!\n"); return; }
		if (HT[j].Lchild == 0 && HT[j].Rchild == 0) {
			original[oloc] = c[j];
			j = 2 * n - 1; oloc++;
		}
	}
	original[oloc] = '\0';
}//Decoding

int main() {
	int j, flag;
	char original[100], code[500];
	HTNode* HT; Huffmancode HC;
	SetChar(c, w, n);
	HuffmanCoding(HT, HC, w, n);
	printf("哈夫曼树初始化完成，请选择功能：\n");
	while (1) {
		printf("\n0、退出     1、编码     2、译码     3、打印编码表     4、打印树     5、打印凹入表示的树\n\n");
		std::cin >> flag; getchar();
		switch (flag) {
		case 0:
			exit(0);
		case 1:
			printf("请输入字符："); gets(original);
			code[0] = '\0';
			Coding(original, HC, code);
			printf("字符为："); puts(original); printf("编码为："); puts(code);
			break;
		case 2:
			printf("请输入代码："); gets(code);
			Decoding(original, HT, code);
			printf("代码为：");  puts(code); printf("译码得："); puts(original);
			break;
		case 3:
			write(HC, w, n);
			break;
		case 4:
			PrintTree(HT, 2 * n - 1, 0);
			break;
		case 5:
			PrintRecessedTable(HT, 0, 2 * n - 1);
			break;
		default:
			break;
		}
	}
	return 0;
}
