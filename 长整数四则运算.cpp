/***************
* Ϊʵ�����ⳤ���������мӼ��˳����㣬���ö�̬˳���洢������
* ÿ����㺬һ�����ͱ�������ÿ���ڵ㲻����2^31-1��
* Ϊ���������������ȡÿ���ڵ�洢һ��0-999,999,999������������10^10Ϊ���ơ�
* �����洢�����ֵ�븺����־��
* ͨ�����λ���������Ӽ�ʵ�ּӼ�����ͨ��˫��ѭ��ʵ�ֳ˷���ͨ����λ���ʵ�ֳ�����
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
	//���ڷ�װ��������˳���
	int* elem;
	int length;
	int listsize;
	int sign; //����Ϊ0��������0Ϊ1
}IntList;

Status InitIntList(IntList& Ia) {
	//���ɿճ�����
	Ia.elem = (int*)malloc(sizeof(int) * LIST_SIZE);
	Ia.listsize = LIST_SIZE;
	Ia.length = 0;
	return OK;
}//InitIntList

Status InPutIntList(IntList& Ia) {
	//���û����������ɳ�����
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
	//���������
	int i = Is.length - 1;
	if (Is.sign == 0)printf("-");
	//�����λ������⴦��
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
	//��������λ���������
	for (i = Is.length - 2; i >= 0; i--) {
		printf(",%03d", Is.elem[i] / 1000000);
		printf(",%03d", (Is.elem[i] / 1000) % 1000);
		printf(",%03d", Is.elem[i] % 1000);
	}//for
	return OK;
}//ShowIntList

Status DestoryIntList(IntList& Id) {
	//���ٳ�����
	if (Id.elem != NULL)free(Id.elem);
	return OK;
}//DestoryIntList

Status SmoothIntList(IntList& Ia) {
	//����������ͨ����λ����Ϊ�������߳���999,999,999�Ľ��
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
	//Ia > Ib ����1��Ia < Ib ����-1��Ia == Ib ����0��Ϊ����ֵ�Ƚ�
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
	//Ia = Ia + Ib��Ҫ��Ia����Ib����Ϊ����
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
	//Ia = Ia - Ib��Ҫ��Ia�ϸ���ڵ���Ib����Ϊ����
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
	//ͨ���������򡢸ı���Ž����ּӼ�����Ϊ��������
	//��������(-1)-(-1)�ļ��㣬����������
	int t, flag, i = 0;
	IntList* p, * q;
	//��pָ���ϸ�������qָ����һ��
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
	if (i == 1 && operat == '-') //��������������Ҫ�����
		p->sign = (p->sign + 1) % 2;
	if (p->length == 1 && p->elem[0] == 0) //��0�ķ������⴦��
		p->sign = 1;
	ShowIntList(*p);
	return OK;
}//AddIntList

Status MultiIntList(IntList IA, IntList IB, IntList& IC) {
	//IC = IA * IB
	InitIntList(IC);
	int i, j, rq = 0, rp = 0;
	long long r, ra, rb;
	//��ʼ��IC
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
	}//���������λ��
	IC.sign = IA.sign == IB.sign ? 1 : 0;
	ShowIntList(IC);
	return OK;
}//MultiIntList

Status ConIntList(IntList Ia, IntList& Rs) {
	//Ϊ����ʹ����λ��������������Ia��10^9����ת��Ϊʮ����
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
	//ʮ���Ƽ���
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
	//��Ia��ʮ����ת��Ϊ10^9����
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
	//IC = IA / IB Ϊ����
	InitIntList(IC);
	if (CompareIntList(IA, IB) == -1) {
		printf("0"); return OK;
	}//��IA < IB��IC = 0
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
	}//Tb����λ����Ta����
	while (1) {
		while (1) {
			if (CompareIntList(Ta, Tb) != -1)break;
			r--; if (r < 0)break;
			for (i = 0; i < q - r + 1; i++)
				Tb.elem[r + i] = Tb.elem[r + i + 1];
			q--; Tb.length--;
		}//Tb����λֱ��Ta>Tb
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
	printf("�������һ��������");
	InPutIntList(A);
	printf("�����������[+,-,*,/]");
	opchar = getchar(); getchar();
	printf("������ڶ���������");
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
