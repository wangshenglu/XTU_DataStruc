/***************
* 程序以栈模拟停车场，自栈底至顶按进入停车场时间排序；
* 以队列模拟车场外的便道，自队首至队尾按进入便道时间排序。
* 程序从终端读入数据指令进行模拟管理。
***************/
#include<stdio.h>
#include<stdlib.h>
#define ERROR 0
#define OK 1
#define OVERFLOW -1
char T[3][10] = { "小汽车","客车","十轮卡车" };
typedef int Status;

typedef struct {
	//表示一辆车的结构体
	int number;	//车牌号码
	int intime; //车辆上次移动的时刻
	int waitime; //车辆在便道上等待的时间
	enum VeEnum { Car = 1, Bus, Truck }type; //车辆类型
}Vehicle;

typedef struct {
	//模拟停车场的栈
	Vehicle* base;
	int top;
	int stacksize;
}SqStack;

typedef struct {
	//模拟便道的队列
	Vehicle* base;
	int front;
	int rear;
	int queuesize;
}SqQueue;

Status InitStack(SqStack& S, int n) {
	//构造一个大小为n的空栈S
	S.base = (Vehicle*)malloc(sizeof(Vehicle) * n);
	if (!S.base)exit(OVERFLOW);
	S.top = 0; S.stacksize = n;
	return OK;
}//InitStack

Status DestoryStack(SqStack& S) {
	//销毁栈S
	free(S.base); S.base = NULL;
	return OK;
}//DestoryStack

Status Push(SqStack& S, Vehicle e) {
	//若栈不满，将e压入栈
	if (S.top >= S.stacksize)return OVERFLOW;
	S.base[S.top++] = e;
	return OK;
}//Push

Status Pop(SqStack& S, Vehicle& e) {
	//若栈不空，栈顶元素出栈，用e返回
	if (S.top == 0)return ERROR;
	e = S.base[--S.top];
	return OK;
}//Pop

Status InitQueue(SqQueue& Q, int n) {
	//构造一个大小为n的空队列Q
	Q.base = (Vehicle*)malloc(sizeof(Vehicle) * n);
	if (!Q.base)exit(OVERFLOW);
	Q.front = Q.rear = 0;
	Q.queuesize = n;
	return OK;
}//InitQueue

Status DestoryQueue(SqQueue& Q) {
	//销毁队列Q
	free(Q.base); Q.base = NULL;
	return OK;
}//DestoryQueue

int QueueLength(SqQueue Q) {
	//返回队列Q元素个数，即长度
	return(Q.rear - Q.front + Q.queuesize) % Q.queuesize;
}//QueueLength

Status InsertQueue(SqQueue& Q, Vehicle e) {
	//若队列不满，插入e为Q新的队尾元素
	if ((Q.rear + 1) % Q.queuesize == Q.front)return OVERFLOW;
	Q.base[Q.rear] = e;
	Q.rear = (Q.rear + 1) % Q.queuesize;
	return OK;
}//InsertQueue

Status DeleteQueue(SqQueue& Q, Vehicle& e) {
	//若队列不空，队首元素出队，用e返回
	if (Q.rear == Q.front)return ERROR;
	e = Q.base[Q.front];
	Q.front = (Q.front + 1) % Q.queuesize;
	return OK;
}//DeleteQueue

Status EnterVehicle(SqQueue& Q, SqStack& S, Vehicle e) {
	//处理进车命令
	int i;
	if (S.stacksize - S.top >= e.type) {
		//停车场空位足够，e入栈
		for (i = 0; i < e.type; i++)Push(S, e);
		printf("%s%d", T[e.type - 1], e.number);
		printf("停放在停车场第%d格\n", S.top - e.type + 1);
	}//if
	else if (Q.queuesize - QueueLength(Q) - 1 >= e.type) {
		//e不入栈情况下便道空位足够，e入队列
		for (i = 0; i < e.type; i++)InsertQueue(Q, e);
		printf("停车场剩余空间%d格，空间不足！\n", S.stacksize - S.top);
		printf("%s%d", T[e.type - 1], e.number);
		printf("停放在便道第%d格\n", QueueLength(Q) - e.type + 1);
	}//else if
	else printf("便道空间不足！入车失败。\n");
	return OK;
}//EnterVehicle

Status OutVehicle(SqQueue& Q, SqStack& Stokein, SqStack& S, Vehicle e) {
	//处理出车命令
	int i, t;
	Vehicle tk;
	while (1) {
		//将栈S内e之上的车移至Stokein中
		if (S.base[S.top - 1].number == e.number)break;
		t = S.base[S.top - 1].type;
		for (i = 0; i < t; i++) {
			Pop(S, tk); Push(Stokein, tk);
			if (S.top == 0) {
				//若停车场中没有所要移出的车辆
				printf("请确保车辆在停车场中！\n");
				while (Pop(Stokein, tk))Push(S, tk); return ERROR;
			}
		}
	}//while
	if (S.base[S.top - 1].type != e.type) {
		//若停车场内车辆与所要移出车辆类型不一致 
		printf("请确保同编号车辆类型一致！\n");
		while (Pop(Stokein, tk))Push(S, tk); return ERROR;
	}
	//将e移出栈，输出收费信息
	t = S.base[S.top - 1].type;
	for (i = 0; i < t; i++)Pop(S, tk);
	printf("%s%d", T[e.type - 1], e.number);
	printf("在停车场停放时间为%d，", e.intime - tk.intime);
	printf("收费%d元\n", (e.intime - tk.intime) * e.type);
	printf("在便道停放时间为%d，收费%g元\n", tk.waitime, e.type*tk.waitime/2.0);
	//将栈Stokein中车返回S
	while (Pop(Stokein, tk))Push(S, tk);
	if (S.stacksize - S.top >= Q.base[Q.front].type && QueueLength(Q) > 0) {
		//若栈空间足够将队列第一辆车入栈
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
	printf("请输入停车场大小："); scanf("%d", &n); getchar();
	InitStack(Park, n); InitStack(ParkTokein, n); InitQueue(Road, 10 * n);
	while (1) {
		printf("请输入指令：");
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
