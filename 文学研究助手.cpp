/************文学研究助手************
* 程序从文件中读入字符串，并完成如下功能：
* 1. 统计小说的字数；
* 2. 采用暴力模式匹配算法检索拟定的词语；
* 3. 截选小说中的一部分，输出至doc文档中。
************************************/
#define _CRT_SECURE_NO_WARNINGS 1 //取消VS安全函数检查:) 
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<iostream>
#define OK 1
#define OVERFLOW 2
#define ERROR 3
#define PATH "呐喊.txt"
#define NEW_PATH "呐喊.doc"
char START[20] = "五、故乡";
char END[20] = "六、孔乙己";
#define MAX_LINE_SIZE 100000 //所读取文件中单行串的大小限制为100000
#define MAX_ROW_NUM 4000 //所读取文件行数限制为4000
typedef int Status;
char Token[MAX_LINE_SIZE];

typedef struct {
	char* head;
	int size; //容量大小 
	int LineNum; //行数
	int row[MAX_ROW_NUM];
}Article;

Status InitArticle(Article& ToInit) {
	//生成一篇空文章
	ToInit.head = (char*)malloc(sizeof(char) * MAX_LINE_SIZE);
	if (!ToInit.head)exit(OVERFLOW);
	ToInit.size = MAX_LINE_SIZE; ToInit.head[0] = '\0';
	ToInit.LineNum = 0;
	return OK;
}//InitArticle

Status DestoryArticle(Article& ToDes) {
	//销毁文章 
	if (ToDes.head) free(ToDes.head);
	ToDes.LineNum = ToDes.size = 0;
	return OK;
}//DestoryArticle

Status ReadArticle(Article& ToRead) {
	//读取PATH文件到文章ToRead
	FILE* fp; char* tmp;
	fp = fopen(PATH, "r");
	if (!fp) { printf("文件不存在！"); exit(ERROR); }
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
	//暴力模式匹配
	int i = pos, j = 0, flag = 1, LenF, LenG;
	LenF = strlen(field.head); LenG = strlen(goal);
	flag = (strlen(goal) == 2 && goal[0] < 0) ? 2 : 1; //分辨中英文字符 
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
	//统计goal在field中出现的次数，输出出现的位置 
	int ans = 0, pos = 0, tmp, r = 0, t;
	while ((tmp = Index(field, goal, pos)) != -1) {
		pos = tmp + strlen(goal); ans++;
		while (field.row[r] < tmp) r++;
		printf("%5d     *%8d    *", ans, r + 1);
		t = r == 0 ? 0 : field.row[r - 1];
		printf("%8d    *%8d\n", (tmp - t) / 2, tmp / 2);
		if (ans % 10 == 0)
			printf("\n 出现次数 *    行数    *    列数    * 字符索引  \n");
	}
	return ans;
}//Total

Status Write(Article ToW) {
	//将文章从START到END的一段写入NEW_PATH文件 
	FILE* fp; int op, ed; char tmp;
	fp = fopen(NEW_PATH, "w");
	op = Index(ToW, START, 0); ed = Index(ToW, END, op);
	tmp = ToW.head[ed]; ToW.head[ed] = '\0';
	fputs(ToW.head + op, fp); fclose(fp);
	ToW.head[ed] = tmp;
	return OK;
}//Write

int main() {
	Article Scream; //Scream即呐喊 
	char Goal[30];
	InitArticle(Scream); ReadArticle(Scream);
	printf("文章读取完毕，共%d行，", Scream.LineNum);
	printf("约%d个中文字符，", (strlen(Scream.head) - Scream.LineNum) / 2);
	printf("共%d个字符。\n", strlen(Scream.head) / 2);
	Write(Scream);
	printf("文章片段写入完毕。\n请输入待查找的词语：");
	std::cin >> Goal;
	printf(" 出现次数 *    行数    *    列数    * 字符索引  \n");
	printf("共出现%d次\n", Total(Scream, Goal));
	DestoryArticle(Scream);
	return 0;
}//the end
