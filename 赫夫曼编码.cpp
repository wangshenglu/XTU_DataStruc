/************�շ�������************
* ����������( Huffman Coding )��һ�ֱ��뷽ʽ,
* �����������������Ŷ�����,��Ȩ·��������С�Ķ�����,
* ����Ӧ��������ѹ����
************************************/
#define _CRT_SECURE_NO_WARNINGS 1
#include<stdio.h>
#include<stdlib.h>
#include<string.h> 
#include<iostream> 
int n = 27; //�ַ�����С
char* c = (char*)malloc((n + 1) * sizeof(char)); //�洢�ַ���
int* w = (int*)malloc((n + 1) * sizeof(int)); //�洢�ַ�Ȩֵ

typedef struct {
	unsigned int Weight;
	unsigned int Parent, Lchild, Rchild;
} HTNode, * HuffmanTree;  //��̬��������洢�շ�����

typedef char** Huffmancode;  //��̬��������洢�շ��������

void SetChar(char* c, int* w, int n) {
	//���ô�������ַ������ӦȨֵ
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
	//����һ��Ҷ�ӽ����Ϊn��Huffman�������n���ַ��ı���
	char* cd;
	int m, c, i, k, start, s1, s2, s3, f;
	if (n <= 1)  return;
	m = 2 * n - 1;  //�ܽ����� 
	HT = (HTNode*)malloc((m + 1) * sizeof(HTNode));  //0�ŵ�Ԫδ�� 

	//��ʼ������HT
	for (i = 1; i <= n; i++) {
		HT[i].Weight = w[i]; HT[i].Parent = 0; HT[i].Lchild = 0; HT[i].Rchild = 0;
	}
	for (i = n + 1; i <= m; i++) {
		HT[i].Weight = 0; HT[i].Parent = 0; HT[i].Lchild = 0; HT[i].Rchild = 0;
	}

	//���շ����� 
	for (i = n + 1; i <= m; i++) {
		//��HT[1...i-1] ѡ��parentΪ0��weight��С��������㣬���Ϊs1��s2 
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
	cd = (char*)malloc(n * sizeof(char)); //��̬���������Ĺ����ռ�
	cd[n - 1] = '\0';    //����Ľ�����־

	// ������ַ��ı���,��Ҷ�ӽ�㵽�����������
	for (i = 1; i <= n; i++) {
		start = n - 1; c = i; f = HT[i].Parent;
		for (; f != 0; c = f, f = HT[f].Parent) {
			if (HT[f].Lchild == c)  cd[--start] = '0';
			else  cd[--start] = '1';
		}
		HC[i] = (char*)malloc((n - start) * sizeof(char));
		// Ϊ��k���ַ����䱣�����Ŀռ�
		strcpy(HC[i], &cd[start]);
	}
	free(cd);
}//HuffmanCoding

void write(Huffmancode& HC, int* w, int n) { 
	int j;
	int WPL = 0;
	for (j = 1; j <= n; j++) {
		printf("%c�ı��룺", c[j]);
		puts(HC[j]);
		WPL += strlen(HC[j]) * w[j];
	}
	printf("WPL=%d\n", WPL);
}//write

void PrintRecessedTable(HuffmanTree HT, int deep, int p) {
	//�����ʾ��
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
	//ֱ�Ӵ�ӡ��
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
	//��ԭ�ı���
	int loc = 1, i, len;
	len = strlen(original);
	for (i = 0; i < len; i++) {
		loc = 1;
		while (c[loc] != original[i])loc++; //��HC���ҵ��ַ���Ӧλ��
		strcat(code, HC[loc]);
	}
}//Coding

void Decoding(char* original, HuffmanTree HT, char* code) {
	//����
	int i, len, j = 2 * n - 1, oloc = 0; //jΪHT��ָ�룬olocΪ����ָ��
	len = strlen(code);
	for (i = 0; i < len; i++) {
		if (code[i] == '0')j = HT[j].Lchild;
		else if (code[i] == '1')j = HT[j].Rchild;
		else { printf("��������!\n"); return; }
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
	printf("����������ʼ����ɣ���ѡ���ܣ�\n");
	while (1) {
		printf("\n0���˳�     1������     2������     3����ӡ�����     4����ӡ��     5����ӡ�����ʾ����\n\n");
		std::cin >> flag; getchar();
		switch (flag) {
		case 0:
			exit(0);
		case 1:
			printf("�������ַ���"); gets(original);
			code[0] = '\0';
			Coding(original, HC, code);
			printf("�ַ�Ϊ��"); puts(original); printf("����Ϊ��"); puts(code);
			break;
		case 2:
			printf("��������룺"); gets(code);
			Decoding(original, HT, code);
			printf("����Ϊ��");  puts(code); printf("����ã�"); puts(original);
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
