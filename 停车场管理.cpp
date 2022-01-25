/***************
* ������ջģ��ͣ��������ջ������������ͣ����ʱ������
* �Զ���ģ�⳵����ı�����Զ�������β��������ʱ������
* ������ն˶�������ָ�����ģ�����
***************/
#include<stdio.h>
#include<stdlib.h>
#define ERROR 0
#define OK 1
#define OVERFLOW -1
char T[3][10] = { "С����","�ͳ�","ʮ�ֿ���" };
typedef int Status;

typedef struct {
	//��ʾһ�����Ľṹ��
	int number;	//���ƺ���
	int intime; //�����ϴ��ƶ���ʱ��
	int waitime; //�����ڱ���ϵȴ���ʱ��
	enum VeEnum { Car = 1, Bus, Truck }type; //��������
}Vehicle;

typedef struct {
	//ģ��ͣ������ջ
	Vehicle* base;
	int top;
	int stacksize;
}SqStack;

typedef struct {
	//ģ�����Ķ���
	Vehicle* base;
	int front;
	int rear;
	int queuesize;
}SqQueue;

Status InitStack(SqStack& S, int n) {
	//����һ����СΪn�Ŀ�ջS
	S.base = (Vehicle*)malloc(sizeof(Vehicle) * n);
	if (!S.base)exit(OVERFLOW);
	S.top = 0; S.stacksize = n;
	return OK;
}//InitStack

Status DestoryStack(SqStack& S) {
	//����ջS
	free(S.base); S.base = NULL;
	return OK;
}//DestoryStack

Status Push(SqStack& S, Vehicle e) {
	//��ջ��������eѹ��ջ
	if (S.top >= S.stacksize)return OVERFLOW;
	S.base[S.top++] = e;
	return OK;
}//Push

Status Pop(SqStack& S, Vehicle& e) {
	//��ջ���գ�ջ��Ԫ�س�ջ����e����
	if (S.top == 0)return ERROR;
	e = S.base[--S.top];
	return OK;
}//Pop

Status InitQueue(SqQueue& Q, int n) {
	//����һ����СΪn�Ŀն���Q
	Q.base = (Vehicle*)malloc(sizeof(Vehicle) * n);
	if (!Q.base)exit(OVERFLOW);
	Q.front = Q.rear = 0;
	Q.queuesize = n;
	return OK;
}//InitQueue

Status DestoryQueue(SqQueue& Q) {
	//���ٶ���Q
	free(Q.base); Q.base = NULL;
	return OK;
}//DestoryQueue

int QueueLength(SqQueue Q) {
	//���ض���QԪ�ظ�����������
	return(Q.rear - Q.front + Q.queuesize) % Q.queuesize;
}//QueueLength

Status InsertQueue(SqQueue& Q, Vehicle e) {
	//�����в���������eΪQ�µĶ�βԪ��
	if ((Q.rear + 1) % Q.queuesize == Q.front)return OVERFLOW;
	Q.base[Q.rear] = e;
	Q.rear = (Q.rear + 1) % Q.queuesize;
	return OK;
}//InsertQueue

Status DeleteQueue(SqQueue& Q, Vehicle& e) {
	//�����в��գ�����Ԫ�س��ӣ���e����
	if (Q.rear == Q.front)return ERROR;
	e = Q.base[Q.front];
	Q.front = (Q.front + 1) % Q.queuesize;
	return OK;
}//DeleteQueue

Status EnterVehicle(SqQueue& Q, SqStack& S, Vehicle e) {
	//�����������
	int i;
	if (S.stacksize - S.top >= e.type) {
		//ͣ������λ�㹻��e��ջ
		for (i = 0; i < e.type; i++)Push(S, e);
		printf("%s%d", T[e.type - 1], e.number);
		printf("ͣ����ͣ������%d��\n", S.top - e.type + 1);
	}//if
	else if (Q.queuesize - QueueLength(Q) - 1 >= e.type) {
		//e����ջ����±����λ�㹻��e�����
		for (i = 0; i < e.type; i++)InsertQueue(Q, e);
		printf("ͣ����ʣ��ռ�%d�񣬿ռ䲻�㣡\n", S.stacksize - S.top);
		printf("%s%d", T[e.type - 1], e.number);
		printf("ͣ���ڱ����%d��\n", QueueLength(Q) - e.type + 1);
	}//else if
	else printf("����ռ䲻�㣡�복ʧ�ܡ�\n");
	return OK;
}//EnterVehicle

Status OutVehicle(SqQueue& Q, SqStack& Stokein, SqStack& S, Vehicle e) {
	//�����������
	int i, t;
	Vehicle tk;
	while (1) {
		//��ջS��e֮�ϵĳ�����Stokein��
		if (S.base[S.top - 1].number == e.number)break;
		t = S.base[S.top - 1].type;
		for (i = 0; i < t; i++) {
			Pop(S, tk); Push(Stokein, tk);
			if (S.top == 0) {
				//��ͣ������û����Ҫ�Ƴ��ĳ���
				printf("��ȷ��������ͣ�����У�\n");
				while (Pop(Stokein, tk))Push(S, tk); return ERROR;
			}
		}
	}//while
	if (S.base[S.top - 1].type != e.type) {
		//��ͣ�����ڳ�������Ҫ�Ƴ��������Ͳ�һ�� 
		printf("��ȷ��ͬ��ų�������һ�£�\n");
		while (Pop(Stokein, tk))Push(S, tk); return ERROR;
	}
	//��e�Ƴ�ջ������շ���Ϣ
	t = S.base[S.top - 1].type;
	for (i = 0; i < t; i++)Pop(S, tk);
	printf("%s%d", T[e.type - 1], e.number);
	printf("��ͣ����ͣ��ʱ��Ϊ%d��", e.intime - tk.intime);
	printf("�շ�%dԪ\n", (e.intime - tk.intime) * e.type);
	printf("�ڱ��ͣ��ʱ��Ϊ%d���շ�%gԪ\n", tk.waitime, e.type*tk.waitime/2.0);
	//��ջStokein�г�����S
	while (Pop(Stokein, tk))Push(S, tk);
	if (S.stacksize - S.top >= Q.base[Q.front].type && QueueLength(Q) > 0) {
		//��ջ�ռ��㹻�����е�һ������ջ
		t = Q.base[Q.front].type;
		for (i = 0; i < t; i++) {
			DeleteQueue(Q, tk); Push(S, tk);
			S.base[S.top - 1].waitime = e.intime - tk.intime;
			S.base[S.top - 1].intime = e.intime;
		}
	}//if
	return OK;
}//OutVehicle

int main() {
	SqStack Park, ParkTokein;
	SqQueue Road;
	int n, t; char c; Vehicle ve;
	printf("������ͣ������С��"); scanf("%d", &n); getchar();
	InitStack(Park, n); InitStack(ParkTokein, n); InitQueue(Road, 10 * n);
	while (1) {
		printf("������ָ�");
		c = getchar();
		if (c == 'E') {
			DestoryQueue(Road); DestoryStack(Park); DestoryStack(ParkTokein);
			return 0;
		}
		scanf("%d %d %d", &ve.number, &ve.intime, &t); getchar();
		ve.type = (Vehicle::VeEnum)t; ve.waitime = 0;
		if (c == 'A')EnterVehicle(Road, Park, ve);
		else OutVehicle(Road, ParkTokein, Park, ve);
	}//while
}
