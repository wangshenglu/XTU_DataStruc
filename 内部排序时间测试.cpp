/******************************** 
* 程序生成随机数据，对其排序并计时
********************************/
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <stack>
clock_t Start, End; //计时工具

void InsertSort(int arr[], int size) {
	//直接插入排序
	int i = 0, j = 0;
	int tmp = 0;
	for (i = 1; i < size; i++) {
		tmp = arr[i];
		j = i;
		while (j > 0 && arr[j - 1] > tmp) {
			arr[j] = arr[j - 1];
			j--;
		}
		arr[j] = tmp;
	}
} //InsertSort

void ShellSort(int R[], int n) {
	//希尔排序
	int i, j, d;
	int tmp;
	d = n / 2; //设置增量
	while (d > 0) { //出口
		for (i = d; i < n; i++) { //对所有组采用直接插入排序
			//对相隔d个为止一组采用直接插入排序
			tmp = R[i];
			j = i - d;
			while (j >= 0 && tmp < R[j]) {
				R[j + d] = R[j];
				j = j - d;
			}
			R[j + d] = tmp;
		}
		d = d / 2; //减小增量
	}
} //ShellSort

void BubbleSort(int* arr, int n) {
	//起泡排序 
	int m, i, j;
	for (i = 0; i < n - 1; i++)
		for (j = 0; j < n - 1 - i; j++)
			if (arr[j] > arr[j + 1]) {
				m = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = m;
			}
} //BubbleSort

int quick_one_pass(int L[], int low, int high) {
	//一趟快速排序
	int i = low, j = high;
	L[0] = L[i];       /*   R[0]作为临时单元和哨兵  */
	do {
		while (L[0] <= L[j] && j > i)
			j--;
		if (j > i) { L[i] = L[j]; i++; }
		while (L[i] <= L[0] && j > i)
			i++;
		if (j > i) { L[j] = L[i]; j--; }
	} while (i != j);    /*   i=j时退出扫描  */
	L[i] = L[0];
	return(i);
} //quick_one_pass

void quick_Sort(int L[], int low, int high) {
	//快速排序
	int k, top = 0, size, * stack;
	size = high - low < 50000 ? 1000 : 100004;
	stack = (int*)malloc(sizeof(int)*size);
	if (!stack)exit(2);
	do {
		while (low < high)
		{
			k = quick_one_pass(L, low, high);
			stack[++top] = high;  stack[++top] = k + 1;
			/*  第二个子序列的上,下界分别入栈  */
			high = k - 1;
		}
		if (top != 0) {
			low = stack[top--]; high = stack[top--];
		}
	} while (top != 0 || low < high);
	free(stack);
} //quick_Sort

void merge(int arr[], int l, int mid, int r) {
	//归并过程
	int* help = (int*)malloc(sizeof(int) * (r - l + 1));//辅助数组
	if (!help)exit(2);
	int i = 0;
	int lIndex = l;
	int rIndex = mid + 1;
	while (lIndex <= mid && rIndex <= r) {
		help[i++] = arr[lIndex] < arr[rIndex] ? arr[lIndex++] : arr[rIndex++];
	}
	//左边和右边肯定有一边到头了，不可能同时，因为每次只移动一边
	while (lIndex <= mid) {
		help[i++] = arr[lIndex++];
	}
	while (rIndex <= r) {
		help[i++] = arr[rIndex++];
	}
	//将排好序的辅助数组赋值给原始数组，不需要返回值
	for (i = 0; i < r - l + 1; i++) {
		arr[l + i] = help[i];
	}
	free(help);
} //merge

static void mergeSort(int arr[], int l, int r) {
	if (l == r) {
		return;
	}
	int mid = (l + r) / 2;
	//左半部分归并排序
	mergeSort(arr, l, mid);
	//右半部分归并排序
	mergeSort(arr, mid + 1, r);
	//左右部分归并
	merge(arr, l, mid, r);
} //mergeSort

void mergeSort(int arr[], int n) {
	//归并排序整个数组
	//如果数组为空或只有一个元素，不需要排序
	if (arr == NULL || n < 2) {
		return;
	}
	mergeSort(arr, 0, n - 1);
} //mergeSort

void adjust(int arr[], int len, int index) {
	// 递归方式构建大根堆(len是arr的长度，index是第一个非叶子节点的下标)
	int left = 2 * index + 1; // index的左子节点
	int right = 2 * index + 2;// index的右子节点
	int temp;
	int maxIdx = index;
	if (left<len && arr[left] > arr[maxIdx])     maxIdx = left;
	if (right<len && arr[right] > arr[maxIdx])     maxIdx = right;
	if (maxIdx != index) {
		temp = arr[maxIdx]; arr[maxIdx] = arr[index]; arr[index] = temp;
		adjust(arr, len, maxIdx);
	}
} //adjust

void heapSort(int arr[], int size) {
	//堆排序
	int temp;
	// 构建大根堆（从最后一个非叶子节点向上）
	for (int i = size / 2 - 1; i >= 0; i--){
		adjust(arr, size, i);
	}
	// 调整大根堆
	for (int i = size - 1; i >= 1; i--){
		temp = arr[0]; arr[0] = arr[i]; arr[i] = temp; // 将当前最大的放置到数组末尾
		adjust(arr, i, 0);              // 将未完成排序的部分继续进行堆排序
	}
} //heapSort

void show(int* R,int i,int flag) {
	if (flag != 1)return;
	int j = i == 3 ? 1 : 0;
	for (int k = 0; k < 100; k++, j++) {
		printf("%d ", R[j]);
	}
	printf("\n\n");
}

int main() {
	int* data; //排序数组
	int* back_up; //存储原始数据
	int NUM[8] = { 100,1000,(int)1e4,(int)1e5,(int)1e6,(int)1e8,(int)1e5,(int)1e5 }; //数组大小取值
	int i, j, n, flag; //n为数组大小
	while (1) {
		//设置数据量
		printf("\n\t\t\t请输入排序数据量\n\n");
		printf("0、退出    1、一百    2、一千    3、一万    4、十万");
		printf("    5、百万    6、一亿    7、十万(顺序)    8、十万(逆序)\n\n");
		std::cin >> flag;
		if (!flag) return 0;
		n = NUM[flag - 1];
		data = (int*)malloc((n + 1) * sizeof(int));
		back_up = (int*)malloc(n * sizeof(int));
		if (!data)exit(2); if (!back_up)exit(2);

		//生成随机数
		if (flag <= 6) {
			srand((unsigned)time(0)); //利用时间变量来实现数据的随机性 
			for (j = 0; j < n; j++)  back_up[j] = rand();  //生成随机数据 
			if (n == 100) {
				printf("排序前数组为：\t");
				for (j = 0; j < n; j++) printf("%d ", back_up[j]);
				printf("\n\n");
			}
		}
		else if (flag == 7) { //十万(顺序)
			for (j = 0; j < n; j++)  back_up[j] = j;
		}
		else if (flag == 8) { //十万(逆序)
			for (j = 0; j < n; j++)  back_up[j] = n - j;
		}
		else continue;

		//排序&计时
		for (i = 0; i < 6; i++) {
			if (n == 1e8 && (i == 0 || i == 2))continue; //一亿数据不测慢排
			if (i != 3)
				for (j = 0; j < n; j++)  data[j] = back_up[j];  //转移数据
			else for (j = 0; j < n; j++)  data[j + 1] = back_up[j];
			Start = clock();  //计时开始
			switch (i) {
			case 0: //直接插入排序
				InsertSort(data, n); End = clock(); //计时结束 
				show(data, i, flag); 
				printf("    直接插入排序  %lf\n", (End - Start) / (double)CLK_TCK);
				break;
			case 1: //希尔排序
				ShellSort(data, n); End = clock(); show(data, i, flag);
				printf("      希尔排序    %lf\n", (End - Start) / (double)CLK_TCK); 
				break;
			case 2: //起泡
				BubbleSort(data, n); End = clock(); show(data, i, flag);
				printf("      起泡排序    %lf\n", (End - Start) / (double)CLK_TCK); 
				break;
			case 3: //快排
				quick_Sort(data, 1, n); End = clock(); show(data, i, flag);
				printf("      快速排序    %lf\n", (End - Start) / (double)CLK_TCK); 
				break;
			case 4: //堆排序
				heapSort(data, n); End = clock(); show(data, i, flag);
				printf("       堆排序     %lf\n", (End - Start) / (double)CLK_TCK); 
				break;
			case 5: //归并排序
				mergeSort(data, n); End = clock(); show(data, i, flag);
				printf("      归并排序    %lf\n", (End - Start) / (double)CLK_TCK);
				break;
			default:
				break;
			}
		}
		free(data); free(back_up);
	}
	return 0;
}
