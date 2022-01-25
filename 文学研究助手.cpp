/************��ѧ�о�����************
* ������ļ��ж����ַ�������������¹��ܣ�
* 1. ͳ��С˵��������
* 2. ���ñ���ģʽƥ���㷨�����ⶨ�Ĵ��
* 3. ��ѡС˵�е�һ���֣������doc�ĵ��С�
************************************/
#define _CRT_SECURE_NO_WARNINGS 1 //ȡ��VS��ȫ�������:) 
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<iostream>
#define OK 1
#define OVERFLOW 2
#define ERROR 3
#define PATH "�ź�.txt"
#define NEW_PATH "�ź�.doc"
char START[20] = "�塢����";
char END[20] = "�������Ҽ�";
#define MAX_LINE_SIZE 100000 //����ȡ�ļ��е��д��Ĵ�С����Ϊ100000
#define MAX_ROW_NUM 4000 //����ȡ�ļ���������Ϊ4000
typedef int Status;
char Token[MAX_LINE_SIZE];

typedef struct {
	char* head;
	int size; //������С 
	int LineNum; //����
	int row[MAX_ROW_NUM];
}Article;

Status InitArticle(Article& ToInit) {
	//����һƪ������
	ToInit.head = (char*)malloc(sizeof(char) * MAX_LINE_SIZE);
	if (!ToInit.head)exit(OVERFLOW);
	ToInit.size = MAX_LINE_SIZE; ToInit.head[0] = '\0';
	ToInit.LineNum = 0;
	return OK;
}//InitArticle

Status DestoryArticle(Article& ToDes) {
	//�������� 
	if (ToDes.head) free(ToDes.head);
	ToDes.LineNum = ToDes.size = 0;
	return OK;
}//DestoryArticle

Status ReadArticle(Article& ToRead) {
	//��ȡPATH�ļ�������ToRead
	FILE* fp; char* tmp;
	fp = fopen(PATH, "r");
	if (!fp) { printf("�ļ������ڣ�"); exit(ERROR); }
	while ((fgets(Token, MAX_LINE_SIZE, fp)) != NULL) {
		if (strlen(Token) + strlen(ToRead.head) > ToRead.size) {
			ToRead.size += MAX_LINE_SIZE;
			tmp = (char*)realloc(ToRead.head, sizeof(char) * ToRead.size);
			if (!tmp)exit(OVERFLOW);
			ToRead.head = tmp;
		}
		strcat(ToRead.head, Token); ToRead.LineNum++;
		ToRead.row[ToRead.LineNum - 1] = strlen(ToRead.head) - 1;
	}
	fclose(fp);
	return OK;
}//ReadArticle

int Index(Article field, char* goal, int pos) {
	//����ģʽƥ��
	int i = pos, j = 0, flag = 1, LenF, LenG;
	LenF = strlen(field.head); LenG = strlen(goal);
	flag = (strlen(goal) == 2 && goal[0] < 0) ? 2 : 1; //�ֱ���Ӣ���ַ� 
	while (i < LenF && j < LenG) {
		if (field.head[i] < 128 && field.head[i]>0 && flag == 2) {
			i++; continue;
		}
		if (field.head[i] == goal[j]) { ++i; ++j; }
		else { i = i - j + flag; j = 0; }
	}
	if (j == LenG)return i - j;
	else return -1;
}//Index

int Total(Article field, char* goal) {
	//ͳ��goal��field�г��ֵĴ�����������ֵ�λ�� 
	int ans = 0, pos = 0, tmp, r = 0, t;
	while ((tmp = Index(field, goal, pos)) != -1) {
		pos = tmp + strlen(goal); ans++;
		while (field.row[r] < tmp) r++;
		printf("%5d     *%8d    *", ans, r + 1);
		t = r == 0 ? 0 : field.row[r - 1];
		printf("%8d    *%8d\n", (tmp - t) / 2, tmp / 2);
		if (ans % 10 == 0)
			printf("\n ���ִ��� *    ����    *    ����    * �ַ�����  \n");
	}
	return ans;
}//Total

Status Write(Article ToW) {
	//�����´�START��END��һ��д��NEW_PATH�ļ� 
	FILE* fp; int op, ed; char tmp;
	fp = fopen(NEW_PATH, "w");
	op = Index(ToW, START, 0); ed = Index(ToW, END, op);
	tmp = ToW.head[ed]; ToW.head[ed] = '\0';
	fputs(ToW.head + op, fp); fclose(fp);
	ToW.head[ed] = tmp;
	return OK;
}//Write

int main() {
	Article Scream; //Scream���ź� 
	char Goal[30];
	InitArticle(Scream); ReadArticle(Scream);
	printf("���¶�ȡ��ϣ���%d�У�", Scream.LineNum);
	printf("Լ%d�������ַ���", (strlen(Scream.head) - Scream.LineNum) / 2);
	printf("��%d���ַ���\n", strlen(Scream.head) / 2);
	Write(Scream);
	printf("����Ƭ��д����ϡ�\n����������ҵĴ��");
	std::cin >> Goal;
	printf(" ���ִ��� *    ����    *    ����    * �ַ�����  \n");
	printf("������%d��\n", Total(Scream, Goal));
	DestoryArticle(Scream);
	return 0;
}//the end
