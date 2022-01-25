/******************
* 用结构体将复数的实部与虚部分开储存
* 实现可以进行复数加减乘除运算的计算器 
*******************/ 
#include<stdio.h>
#define OK 1
#define ERROR 0
typedef int Status;

typedef struct
{
	double real;
	double image;
}Complex;

Status InitComplex(Complex &c)
{	//生成复数 
	int f = scanf("%lf%lf",&c.real,&c.image);
	getchar();
	if( f == 2)
		return OK;
	else
		return ERROR;
}//InitComplex

Status AddComplex(Complex ca,Complex cb,Complex &res)
{	//复数相加
	res.real = ca.real + cb.real;
	res.image = ca.image + cb.image;
	return OK;
}//AddComplex

Status SubComplex(Complex ca,Complex cb,Complex &res)
{	//复数相减
	res.real = ca.real - cb.real;
	res.image = ca.image - cb.image;
	return OK;
}//SubComplex

Status MultiComplex(Complex ca,Complex cb,Complex &res)
{	//复数相乘 
	res.real = ca.real*cb.real - ca.image*cb.image;
	res.image = ca.real*cb.image + cb.real*ca.image;
	return OK;
}//MultiComplex

Status ShowReal(Complex ca,double &r)
{	//分离出实部 
	r = ca.real;
	return OK;
}//ShowReal

Status ShowImage(Complex ca,double &r)
{	//分离出虚部 
	r = ca.image;
	return OK;
}//ShowImage

Status ConComplex(Complex ca,Complex &res)
{	//求复数的共轭 
	res.real = ca.real;
	res.image = -ca.image;
	return OK;
}//ConComplex

Status DiComplex(Complex ca,Complex cb,Complex &res)
{	//复数除法 
	if(cb.real==0 && cb.image==0)	return ERROR;
	double tem = cb.real*cb.real + cb.image*cb.image;
	res.real = (ca.real*cb.real + ca.image*cb.image) / tem;
	res.image = (-ca.real*cb.image + cb.real*ca.image) / tem;
	return OK;
}//DiComplex

int main()
{
	Complex c1,c2,result;
	char operation,con;
	int f=OK;
	while(1)
	{
		//控制程序是否继续运行 
		printf("是否继续?[y/n]");
		scanf("%c",&con);getchar();
		if (con == 'n' || con == 'N')	break;
		//输入第一个复数 
		printf("请输入第一个复数：");
		while( InitComplex(c1) == ERROR )
			printf("输入错误，请重新输入");
		//输入运算符
		while(1)
		{	
			printf("请输入运算符[+,-,*,/]");
			scanf("%c",&operation);getchar();
			if(operation=='+' || operation=='-' || operation=='*' || operation =='/')
				break;
		}
		//输入第二个复数 
		printf("请输入第二个复数：");
		while( InitComplex(c2) == ERROR )
			printf("输入错误，请重新输入");
		//进行计算
		switch(operation)
		{
			case '+':
				AddComplex(c1,c2,result);
				break;
			case '-':
				SubComplex(c1,c2,result);
				break;
			case '*':
				MultiComplex(c1,c2,result);
				break;
			case '/':
				f = DiComplex(c1,c2,result);
				break;
		}
		//输出答案
		if(f==ERROR)	{printf("ERROR\n");continue;}
		if(result.real==0 && result.image==0)	{printf("0\n");continue;}
		if(result.real != 0)	printf("%g",result.real);
		if(result.image != 0)	printf("%+gi",result.image);
		printf("\n");
	}
	return 0;
}
