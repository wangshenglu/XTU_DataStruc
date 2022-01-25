/******************************** 
* ��������������ݣ��������򲢼�ʱ
********************************/
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <stack>
clock_t Start, End; //��ʱ����

void InsertSort(int arr[], int size) {
	//ֱ�Ӳ�������
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
	//ϣ������
	int i, j, d;
	int tmp;
	d = n / 2; //��������
	while (d > 0) { //����
		for (i = d; i < n; i++) { //�����������ֱ�Ӳ�������
			//�����d��Ϊֹһ�����ֱ�Ӳ�������
			tmp = R[i];
			j = i - d;
			while (j >= 0 && tmp < R[j]) {
				R[j + d] = R[j];
				j = j - d;
			}
			R[j + d] = tmp;
		}
		d = d / 2; //��С����
	}
} //ShellSort

void BubbleSort(int* arr, int n) {
	//�������� 
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
	//һ�˿�������
	int i = low, j = high;
	L[0] = L[i];       /*   R[0]��Ϊ��ʱ��Ԫ���ڱ�  */
	do {
		while (L[0] <= L[j] && j > i)
			j--;
		if (j > i) { L[i] = L[j]; i++; }
		while (L[i] <= L[0] && j > i)
			i++;
		if (j > i) { L[j] = L[i]; j--; }
	} while (i != j);    /*   i=jʱ�˳�ɨ��  */
	L[i] = L[0];
	return(i);
} //quick_one_pass

void quick_Sort(int L[], int low, int high) {
	//��������
	int k, top = 0, size, * stack;
	size = high - low < 50000 ? 1000 : 100004;
	stack = (int*)malloc(sizeof(int)*size);
	if (!stack)exit(2);
	do {
		while (low < high)
		{
			k = quick_one_pass(L, low, high);
			stack[++top] = high;  stack[++top] = k + 1;
			/*  �ڶ��������е���,�½�ֱ���ջ  */
			high = k - 1;
		}
		if (top != 0) {
			low = stack[top--]; high = stack[top--];
		}
	} while (top != 0 || low < high);
	free(stack);
} //quick_Sort

void merge(int arr[], int l, int mid, int r) {
	//�鲢����
	int* help = (int*)malloc(sizeof(int) * (r - l + 1));//��������
	if (!help)exit(2);
	int i = 0;
	int lIndex = l;
	int rIndex = mid + 1;
	while (lIndex <= mid && rIndex <= r) {
		help[i++] = arr[lIndex] < arr[rIndex] ? arr[lIndex++] : arr[rIndex++];
	}
	//��ߺ��ұ߿϶���һ�ߵ�ͷ�ˣ�������ͬʱ����Ϊÿ��ֻ�ƶ�һ��
	while (lIndex <= mid) {
		help[i++] = arr[lIndex++];
	}
	while (rIndex <= r) {
		help[i++] = arr[rIndex++];
	}
	//���ź���ĸ������鸳ֵ��ԭʼ���飬����Ҫ����ֵ
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
	//��벿�ֹ鲢����
	mergeSort(arr, l, mid);
	//�Ұ벿�ֹ鲢����
	mergeSort(arr, mid + 1, r);
	//���Ҳ��ֹ鲢
	merge(arr, l, mid, r);
} //mergeSort

void mergeSort(int arr[], int n) {
	//�鲢������������
	//�������Ϊ�ջ�ֻ��һ��Ԫ�أ�����Ҫ����
	if (arr == NULL || n < 2) {
		return;
	}
	mergeSort(arr, 0, n - 1);
} //mergeSort

void adjust(int arr[], int len, int index) {
	// �ݹ鷽ʽ���������(len��arr�ĳ��ȣ�index�ǵ�һ����Ҷ�ӽڵ���±�)
	int left = 2 * index + 1; // index�����ӽڵ�
	int right = 2 * index + 2;// index�����ӽڵ�
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
	//������
	int temp;
	// ��������ѣ������һ����Ҷ�ӽڵ����ϣ�
	for (int i = size / 2 - 1; i >= 0; i--){
		adjust(arr, size, i);
	}
	// ���������
	for (int i = size - 1; i >= 1; i--){
		temp = arr[0]; arr[0] = arr[i]; arr[i] = temp; // ����ǰ���ķ��õ�����ĩβ
		adjust(arr, i, 0);              // ��δ�������Ĳ��ּ������ж�����
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
	int* data; //��������
	int* back_up; //�洢ԭʼ����
	int NUM[8] = { 100,1000,(int)1e4,(int)1e5,(int)1e6,(int)1e8,(int)1e5,(int)1e5 }; //�����Сȡֵ
	int i, j, n, flag; //nΪ�����С
	while (1) {
		//����������
		printf("\n\t\t\t����������������\n\n");
		printf("0���˳�    1��һ��    2��һǧ    3��һ��    4��ʮ��");
		printf("    5������    6��һ��    7��ʮ��(˳��)    8��ʮ��(����)\n\n");
		std::cin >> flag;
		if (!flag) return 0;
		n = NUM[flag - 1];
		data = (int*)malloc((n + 1) * sizeof(int));
		back_up = (int*)malloc(n * sizeof(int));
		if (!data)exit(2); if (!back_up)exit(2);

		//���������
		if (flag <= 6) {
			srand((unsigned)time(0)); //����ʱ�������ʵ�����ݵ������ 
			for (j = 0; j < n; j++)  back_up[j] = rand();  //����������� 
			if (n == 100) {
				printf("����ǰ����Ϊ��\t");
				for (j = 0; j < n; j++) printf("%d ", back_up[j]);
				printf("\n\n");
			}
		}
		else if (flag == 7) { //ʮ��(˳��)
			for (j = 0; j < n; j++)  back_up[j] = j;
		}
		else if (flag == 8) { //ʮ��(����)
			for (j = 0; j < n; j++)  back_up[j] = n - j;
		}
		else continue;

		//����&��ʱ
		for (i = 0; i < 6; i++) {
			if (n == 1e8 && (i == 0 || i == 2))continue; //һ�����ݲ�������
			if (i != 3)
				for (j = 0; j < n; j++)  data[j] = back_up[j];  //ת������
			else for (j = 0; j < n; j++)  data[j + 1] = back_up[j];
			Start = clock();  //��ʱ��ʼ
			switch (i) {
			case 0: //ֱ�Ӳ�������
				InsertSort(data, n); End = clock(); //��ʱ���� 
				show(data, i, flag); 
				printf("    ֱ�Ӳ�������  %lf\n", (End - Start) / (double)CLK_TCK);
				break;
			case 1: //ϣ������
				ShellSort(data, n); End = clock(); show(data, i, flag);
				printf("      ϣ������    %lf\n", (End - Start) / (double)CLK_TCK); 
				break;
			case 2: //����
				BubbleSort(data, n); End = clock(); show(data, i, flag);
				printf("      ��������    %lf\n", (End - Start) / (double)CLK_TCK); 
				break;
			case 3: //����
				quick_Sort(data, 1, n); End = clock(); show(data, i, flag);
				printf("      ��������    %lf\n", (End - Start) / (double)CLK_TCK); 
				break;
			case 4: //������
				heapSort(data, n); End = clock(); show(data, i, flag);
				printf("       ������     %lf\n", (End - Start) / (double)CLK_TCK); 
				break;
			case 5: //�鲢����
				mergeSort(data, n); End = clock(); show(data, i, flag);
				printf("      �鲢����    %lf\n", (End - Start) / (double)CLK_TCK);
				break;
			default:
				break;
			}
		}
		free(data); free(back_up);
	}
	return 0;
}
