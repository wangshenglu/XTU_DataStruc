/***************
* 为实现任意长的整数进行加减乘除运算，采用动态顺序表存储长整数
* 每个结点含一个整型变量，即每个节点不超过2^31-1。
* 为方便输入输出，采取每个节点存储一个0-999,999,999的整数，即以10^10为进制。
* 负数存储其绝对值与负数标志。
* 通过相对位数的整数加减实现加减法，通过双重循环实现乘法，通过移位相减实现除法。
***************/
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define OK 1
#define ERROR 0
#define LIST_SIZE 100
#define INPUT_SIZE 140
#define OUTPUT_SIZE 280
typedef int Status;
typedef struct {
	//用于封装长整数的顺序表
	int* elem;
	int length;
	int listsize;
	int sign; //负数为0，正数或0为1
}IntList;

Status InitIntList(IntList& Ia) {
	//生成空长整数
	Ia.elem = (int*)malloc(sizeof(int) * LIST_SIZE);
	Ia.listsize = LIST_SIZE;
	Ia.length = 0;
	return OK;
}//InitIntList

Status InPutIntList(IntList& Ia) {
	//由用户的输入生成长整数
	char token[INPUT_SIZE];
	int i, j = 0, k = 0, u = 1; 
	gets(token);
	InitIntList(Ia);
	Ia.elem[0] = 0;
	for (i = strlen(token) - 1; i >= 0; i--) {
		if (j == 9) {
			j = 0; k++; Ia.elem[k] = 0; u = 1;
		}//if
		if (token[i] == ',')continue;
		if (token[i] == '-')break;
		Ia.elem[k] += (token[i] - '0') * u;
		j++; u *= 10;
	}//for
	if (token[0] == '-')Ia.sign = 0;
	else Ia.sign = 1;
	Ia.length = k + 1;
	return OK;
}//InPutIntList

Status ShowIntList(IntList Is) {
	//输出长整数
	int i = Is.length - 1;
	if (Is.sign == 0)printf("-");
	//对最高位输出特殊处理
	if (Is.elem[i] > 999999) {
		printf("%d", Is.elem[i] / 1000000);
		printf(",%03d", (Is.elem[i] / 1000) % 1000);
		printf(",%03d", Is.elem[i] % 1000);
	}
	else if (Is.elem[i] > 999) {
		printf("%d", (Is.elem[i] / 1000) % 1000);
		printf(",%03d", Is.elem[i] % 1000);
	}
	else printf("%d", Is.elem[i] % 1000);
	//输出除最高位外的其余数
	for (i = Is.length - 2; i >= 0; i--) {
		printf(",%03d", Is.elem[i] / 1000000);
		printf(",%03d", (Is.elem[i] / 1000) % 1000);
		printf(",%03d", Is.elem[i] % 1000);
	}//for
	return OK;
}//ShowIntList

Status DestoryIntList(IntList& Id) {
	//销毁长整数
	if (Id.elem != NULL)free(Id.elem);
	return OK;
}//DestoryIntList

Status SmoothIntList(IntList& Ia) {
	//整理长整数，通过进位消除为负数或者超过999,999,999的结点
	int i;
	Ia.elem[Ia.length] = 0; Ia.length++;
	for (i = 0; i < Ia.length; i++) {
		if (Ia.elem[i] < 0) {
			Ia.elem[i] += 1000000000;
			Ia.elem[i + 1]--;
		}
		if (Ia.elem[i] > 999999999) {
			Ia.elem[i] -= 1000000000;
			Ia.elem[i + 1]++;
		}
	}//for
	if (Ia.elem[Ia.length - 1] == 0)Ia.length--;
	return OK;
}//SmoothIntList

int CompareIntList(IntList Ia, IntList Ib) {
	//Ia > Ib 返回1，Ia < Ib 返回-1，Ia == Ib 返回0，为绝对值比较
	int i;
	if (Ia.length > Ib.length)return 1;
	if (Ia.length < Ib.length)return -1;
	for (i = Ia.length-1; i >= 0; i--) {
		if (Ia.elem[i] > Ib.elem[i])return 1;
		if (Ia.elem[i] < Ib.elem[i])return -1;
	}
	return 0;
}//CompareIntList

Status AtomAddIntList(IntList& Ia, IntList Ib) {
	//Ia = Ia + Ib，要求Ia长于Ib，都为正数
	long long r, ra, rb;
	int i;
	Ia.elem[Ia.length] = 0; Ia.length++;
	for (i = 0; i < Ib.length; i++) {
		ra = Ia.elem[i]; rb = Ib.elem[i];
		r = ra + rb;
		if (r > 999999999) {
			r -= 1000000000;
			Ia.elem[i + 1]++;
		}
		Ia.elem[i] = r;
	}//for
	if (Ia.elem[Ia.length - 1] == 0)Ia.length--;
	SmoothIntList(Ia);
	return OK;
}//AtomAddIntList

Status AtomSubIntList(IntList& Ia, IntList Ib) {
	//Ia = Ia - Ib，要求Ia严格大于等于Ib，都为正数
	int i;
	for (i = Ib.length - 1; i >= 0; i--)
		Ia.elem[i] -= Ib.elem[i];
	SmoothIntList(Ia);
	for (i = Ia.length - 1; i > 0; i--) {
		if (Ia.elem[i] == 0)Ia.length--;
		else break;
	}
	return OK;
}//AtomSubIntList

Status AddIntList(IntList IA, IntList IB, char operat) {
	//通过调换次序、改变符号将各种加减法归为两种类型
	//包含诸如(-1)-(-1)的计算，共八种情形
	int t, flag, i = 0;
	IntList* p, * q;
	//将p指向严格大的数，q指向另一个
	if (CompareIntList(IA,IB)==1) { p = &IA; q = &IB; }
	else { p = &IB; q = &IA; i++; }
	t = operat == '+' ? 1 : 0;
	flag = t * 100 + p->sign * 10 + q->sign;
	switch (flag) {
	case(0):case(101):
		AtomSubIntList(*p, *q);
		p->sign = 0;
		break;
	case(1):case(100):
		AtomAddIntList(*p, *q);
		p->sign = 0;
		break;
	case(10):case(111):
		AtomAddIntList(*p, *q);
		p->sign = 1;
		break;
	case(11):case(110):
		AtomSubIntList(*p, *q);
		p->sign = 1;
		break;
	}//switch
	if (i == 1 && operat == '-') //减法交换次序需要变符号
		p->sign = (p->sign + 1) % 2;
	if (p->length == 1 && p->elem[0] == 0) //对0的符号特殊处理
		p->sign = 1;
	ShowIntList(*p);
	return OK;
}//AddIntList

Status MultiIntList(IntList IA, IntList IB, IntList& IC) {
	//IC = IA * IB
	InitIntList(IC);
	int i, j, rq = 0, rp = 0;
	long long r, ra, rb;
	//初始化IC
	for (i = 0; i < IA.length + IB.length; i++)
		IC.elem[i] = 0;
	IC.length = IA.length + IB.length;
	for (i = 0; i < IA.length; i++) {
		for (j = 0; j < IB.length; j++) {
			ra = IA.elem[i]; rb = IB.elem[j];
			r = ra * rb;
			if (r > 999999999) {
				rq = r / 1000000000;
				rp = r % 1000000000;
				IC.elem[i + j] += rp;
				IC.elem[i + j + 1] += rq;
			}
			else IC.elem[i + j] += r;
			SmoothIntList(IC);
		}
	}
	SmoothIntList(IC);
	for (i = IC.length - 1; i > 0; i--) {
		if (IC.elem[i] == 0)IC.length--;
		else break;
	}//消除多余的位数
	IC.sign = IA.sign == IB.sign ? 1 : 0;
	ShowIntList(IC);
	return OK;
}//MultiIntList

Status ConIntList(IntList Ia, IntList& Rs) {
	//为方便使用移位相减计算除法，将Ia由10^9进制转换为十进制
	InitIntList(Rs);
	int i, j, t = 1;
	for (i = 0; i < Ia.length; i++) {
		for (j = 0; j < 9; j++) {
			Rs.elem[9 * i + j] = (Ia.elem[i] % (10 * t) / t);
			t *= 10;
		}
		t = 1;
	}
		
	Rs.length = 9 * Ia.length;
	for (i = Rs.length - 1; i > 0; i--) {
		if (Rs.elem[i] == 0)Rs.length--;
		else break;
	}
	return OK;
}//ConIntList

Status DSubIntList(IntList& Ia, IntList Ib) {
	//十进制减法
	int j;
	for (j = Ib.length - 1; j >= 0; j--)
		Ia.elem[j] -= Ib.elem[j];
	for (j = 0; j < Ia.length; j++) {
		if (Ia.elem[j] < 0) {
			Ia.elem[j] += 10;
			Ia.elem[j + 1]--;
		}
	}
	for (j = Ia.length - 1; j > 0; j--) {
		if (Ia.elem[j] == 0)Ia.length--;
		else break;
	}//Ta = Ta - Tb
	return OK;
}//DSubIntList

Status DisConIntList(IntList Ia, IntList& Rs) {
	//将Ia由十进制转换为10^9进制
	int t, i, u, j;
	t = Ia.length / 9;
	t = Ia.length%9 == 0 ? t : t + 1;
	Rs.length = t;
	for (i = 0; i < Rs.length; i++)Rs.elem[i] = 0;
	for (i = 0; i < Ia.length; i++) {
		t = i / 9; u = 1;
		for(j = 0; j < i - 9 * t; j++)
			u *= 10;
		Rs.elem[t] += Ia.elem[i] * u;
	}
	return OK;
}
Status DivIntList(IntList IA, IntList IB, IntList& IC) {
	//IC = IA / IB 为整除
	InitIntList(IC);
	if (CompareIntList(IA, IB) == -1) {
		printf("0"); return OK;
	}//若IA < IB，IC = 0
	IntList Ta, Tb, Rs;
	InitIntList(Rs);
	ConIntList(IA, Ta); ConIntList(IB, Tb);
	IC.length = Ta.length - Tb.length + 1;
	int i, p = Ta.length - 1, q = Tb.length - 1, r = 0;
	for (i = 0; i < IC.length; i++)
		IC.elem[i] = 0;
	while (1) {
		if (p == q)break;
		for (i = 0; i < Tb.length; i++)
			Tb.elem[q - i + 1] = Tb.elem[q - i];
		Tb.elem[q - i + 1] = 0;
		q++; r++; Tb.length++;
		if (p == q)break;
	}//Tb左移位至与Ta对齐
	while (1) {
		while (1) {
			if (CompareIntList(Ta, Tb) != -1)break;
			r--; if (r < 0)break;
			for (i = 0; i < q - r + 1; i++)
				Tb.elem[r + i] = Tb.elem[r + i + 1];
			q--; Tb.length--;
		}//Tb右移位直至Ta>Tb
		if (r < 0)break;
		DSubIntList(Ta, Tb);
		IC.elem[r]++;
	}
	if (IC.elem[IC.length - 1] == 0)IC.length--;
	DisConIntList(IC, Rs);
	Rs.sign = IA.sign == IB.sign ? 1 : 0;
	ShowIntList(Rs);
	DestoryIntList(Ta); DestoryIntList(Tb); DestoryIntList(Rs);
	return OK;
}//DivIntList

int main() {
	IntList A, B, C;
	char opchar;
	printf("请输入第一个整数：");
	InPutIntList(A);
	printf("请输入运算符[+,-,*,/]");
	opchar = getchar(); getchar();
	printf("请输入第二个整数：");
	InPutIntList(B);
	switch (opchar)
	{
	case('+'): case('-'):
		AddIntList(A, B, opchar);
		break;
	case('*'):
		MultiIntList(A, B, C);
		DestoryIntList(C);
		break;
	case('/'):
		DivIntList(A, B, C);
		DestoryIntList(C);
		break;
	}
	DestoryIntList(A); DestoryIntList(B);
	return 0;
}//main
