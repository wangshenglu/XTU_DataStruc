/***********稀疏矩阵运算器**********
* 稀疏矩阵是指那些多数元素为零的矩阵。
* 利用"稀疏"特点进行存储和计算， 
* 实现一个能进行稀疏矩阵基本运算的运算器。
************************************/
#define _CRT_SECURE_NO_WARNINGS 1 
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#define OK 1
#define ERROR 2
#define MAXSIZE 400     //非零元个数最大值
#define MAX_ROW_NUM 50        //行数最大值
typedef int Status;
typedef int ElemType;
typedef struct {
    int i, j;                             //该非零元的行下表和列下标
    ElemType e;
} Triple;
typedef struct {
    Triple data[MAXSIZE + 1];             //非零元三元组表
    int rpos[MAX_ROW_NUM + 1];                  //各行第一个非零元的位置表
    int mu, nu, tu;                       //矩阵的行数、列数和非零元个数
} RLSMatrix;

Status CreatRLSMatrix(RLSMatrix& M) {
    //创建稀疏矩阵M
    int t;
    //输入部分
    printf("请输入矩阵的行数、列数和非零元个数：");
    std::cin >> M.mu >> M.nu >> M.tu;
    for (t = 1; t <= M.tu; t++) {
        printf("请按三元组格式输入非零元：   ");
        std::cin >> M.data[t].i >> M.data[t].j >> M.data[t].e;
    }
    //生成rpos部分
    for (t = 1; t <= MAX_ROW_NUM; t++) M.rpos[t] = 0;
    for (t = 1; t <= M.tu; t++) M.rpos[M.data[t].i + 1]++;
    M.rpos[1] = 1;
    for (t = 2; t <= M.mu; t++) M.rpos[t] += M.rpos[t - 1];
    return OK;
}//CreatRLSMatrix

Status PrintRLSMatrix(RLSMatrix M) {
    //输出稀疏矩阵M
    int k, l, m = 1;
    printf("%7d行%7d列%7d个非零元\n\n", M.mu, M.nu, M.tu);
    for (k = 1; k <= M.mu; k++) {
        for (l = 1; l <= M.nu; l++) {
            if (M.data[m].i == k && M.data[m].j == l && m <= M.tu) {
                printf("%5d", M.data[m].e);
                m++;
            }
            else printf("%5d", 0);
        }
        printf("\n");
    }
    printf("\n");
    return OK;
}//PrintRLSMatrix

Status DestoryRLSMatrix(RLSMatrix& M) {
    //销毁稀疏矩阵M
    M.mu = M.nu = M.tu = 0;
    return OK;
}//DestoryRLSMatrix

Status TransposeRLSMatrix(RLSMatrix M, RLSMatrix& T) {
    //求 M 的转置的快速算法
    if (!M.tu) return OK;
    int col, t, p, q;
    int* num, * cpot;
    num = (int*)malloc(sizeof(int) * MAX_ROW_NUM);
    cpot = (int*)malloc(sizeof(int) * MAX_ROW_NUM);
    T.mu = M.nu;  T.nu = M.mu;  T.tu = M.tu;
    for (col = 1; col <= M.nu; ++col) num[col] = 0;
    for (t = 1; t <= M.tu; ++t) ++num[M.data[t].j];
    cpot[1] = 1;
    //求 M 中每一列的第一个非零元在 b.data 中的序号
    for (col = 2; col <= M.nu; ++col) cpot[col] = cpot[col - 1] + num[col - 1];
    for (p = 1; p <= M.tu; ++p) {
        col = M.data[p].j;   q = cpot[col];
        T.data[q].i = M.data[p].j;  T.data[q].j = M.data[p].i;
        T.data[q].e = M.data[p].e;  ++cpot[col];
    }
    free(num); free(cpot);
    return OK;
}//TransposeRLSMatrix

Status MultRLSMatrix(RLSMatrix A, RLSMatrix B, RLSMatrix& C) {
    //C = A * B
    int Arow, Brow, Ccol, a_end, p, b_end, q;
    ElemType* ctemp;
    if (A.nu != B.mu) return ERROR;
    if (A.tu * B.tu == 0) return  OK;
    C.mu = A.mu; C.nu = B.nu; C.tu = 0;
    ctemp = (ElemType*)malloc((B.nu + 1) * sizeof(ElemType));

    //处理A的每一行
    for (Arow = 1; Arow <= A.mu; Arow++) {
        for (Brow = 1; Brow <= B.nu; Brow++) ctemp[Brow] = 0;
        C.rpos[Arow] = C.tu + 1;   
        a_end = Arow < A.mu ? A.rpos[Arow + 1] : A.tu + 1;   //a_end = Arow行元素索引终点+1

        //对Arow行中每一个非零元，求得C中第C_row(=Arow)行的非零元
        for (p = A.rpos[Arow]; p < a_end; p++) {
            Brow = A.data[p].j;
            b_end = Brow < B.mu ? B.rpos[Brow + 1] : B.tu + 1;
            for (q = B.rpos[Brow]; q < b_end; q++) {
                Ccol = B.data[q].j;
                ctemp[Ccol] += A.data[p].e * B.data[q].e;
            }
        }

        //将ctmp存入C
        for (Ccol = 1; Ccol <= C.nu; Ccol++) {
            if (ctemp[Ccol] == 0) continue;
            if (++C.tu > MAXSIZE) return ERROR;
            C.data[C.tu].i = Arow; C.data[C.tu].j = Ccol;
            C.data[C.tu].e = ctemp[Ccol];
        }
    }
    free(ctemp);
    return  OK;
}

Status AddRLSMatrix(RLSMatrix A, RLSMatrix B, RLSMatrix& C) {
    //C = A + B
    if (A.mu != B.mu || A.nu != B.nu)return ERROR;
    int p, q, r, flag;
    p = q = r = 1;
    C.mu = A.mu; C.nu = A.nu;
    for (; p <= A.tu && q <= B.tu;) {
        //判断A[p]和B[q]在C中前后顺序
        if (A.data[p].i < B.data[q].i) flag = -1;
        else if (A.data[p].i > B.data[q].i) flag = 1;
        else {
            if (A.data[p].j < B.data[q].j) flag = -1;
            else if (A.data[p].j > B.data[q].j) flag = 1;
            else flag = 0;
        }
        //相加
        if (flag == -1) { C.data[r] = A.data[p]; p++; r++; }
        else if (flag == 1) { C.data[r] = B.data[q]; q++; r++; }
        else {
            if (A.data[p].e + B.data[q].e == 0) {
                p++; q++; continue;
            }
            C.data[r] = A.data[p];
            C.data[r].e += B.data[q].e;
            p++; q++; r++;
        }
    }//for
    //处理尾部数据
    if (p != A.tu + 1)
        for (; p <= A.tu; p++, r++) C.data[r] = A.data[p];
    if (q != B.tu + 1)
        for (; q <= B.tu; q++, r++) C.data[r] = B.data[q];
    C.tu = r - 1;
    //生成C.rpos
    for (r = 1; r <= MAX_ROW_NUM; r++) C.rpos[r] = 0;
    for (r = 1; r <= C.tu; r++) C.rpos[C.data[r].i + 1]++;
    C.rpos[1] = 1;
    for (r = 2; r <= C.mu; r++) C.rpos[r] += C.rpos[r - 1];
    return OK;
}//AddRLSMatrix

Status SubtRLSMatrix(RLSMatrix A, RLSMatrix& B, RLSMatrix& C) {
    //C = A - B
    if (A.mu != B.mu || A.nu != B.nu) return ERROR;
    int i;
    for (i = 1; i <= B.tu; i++) B.data[i].e = -B.data[i].e;
    AddRLSMatrix(A, B, C);
    return OK;
}//SubtSMatrix

int main() {
    RLSMatrix A, B, C;
    int flag;
    printf("                    稀疏矩阵计算器                   \n");
    printf("*   *   *   *   *   *   *   *   *   *   *   *   *   *\n");
    while (1) {
        printf("\n1、转置     2、加法     3、减法     4、乘法     5、退出\n\n");
        printf("*   *   *   *   *   *   *   *   *   *   *   *   *   *\n");
        printf("输入要进行的运算功能的编号:");
        std::cin >> flag;
        switch (flag) {
        case 5:
            printf("程序退出！\n");
            exit(0);
        case 1:
            //转置
            CreatRLSMatrix(A); printf("矩阵A:\n"); PrintRLSMatrix(A);
            TransposeRLSMatrix(A, C);
            printf("A的转置:\n"); PrintRLSMatrix(C);
            break;
        case 2:
            //加法
            CreatRLSMatrix(A); printf("矩阵A:\n"); PrintRLSMatrix(A);
            CreatRLSMatrix(B); printf("矩阵B:\n"); PrintRLSMatrix(B);
            if (AddRLSMatrix(A, B, C) != ERROR) {
                printf("A+B:\n"); PrintRLSMatrix(C);
            }
            else printf("错误！矩阵A、B行列数不一致！\n");
            break;
        case 3:
            //减法
            CreatRLSMatrix(A); printf("矩阵A:\n"); PrintRLSMatrix(A);
            CreatRLSMatrix(B); printf("矩阵B:\n"); PrintRLSMatrix(B);
            if (SubtRLSMatrix(A, B, C) != ERROR) {
                printf("A-B:\n"); PrintRLSMatrix(C);
            }
            else printf("错误！矩阵A、B行列数不一致！\n");
            break;
        case 4:
            //乘法
            CreatRLSMatrix(A); printf("矩阵A:\n"); PrintRLSMatrix(A);
            CreatRLSMatrix(B); printf("矩阵B:\n"); PrintRLSMatrix(B);
            if (MultRLSMatrix(A, B, C) != ERROR) {
                printf("A*B:\n"); PrintRLSMatrix(C);
            }
            else printf("错误！矩阵A列数!=矩阵B行数！\n");
            break;
        default:
            printf("请输入合法编号！\n"); getchar(); 
            break;
        }
        DestoryRLSMatrix(A); DestoryRLSMatrix(B); DestoryRLSMatrix(C);
    }
    return 0;
}
