/******************
* �ýṹ�彫������ʵ�����鲿�ֿ�����
* ʵ�ֿ��Խ��и����Ӽ��˳�����ļ����� 
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
{	//���ɸ��� 
	int f = scanf("%lf%lf",&c.real,&c.image);
	getchar();
	if( f == 2)
		return OK;
	else
		return ERROR;
}//InitComplex

Status AddComplex(Complex ca,Complex cb,Complex &res)
{	//�������
	res.real = ca.real + cb.real;
	res.image = ca.image + cb.image;
	return OK;
}//AddComplex

Status SubComplex(Complex ca,Complex cb,Complex &res)
{	//�������
	res.real = ca.real - cb.real;
	res.image = ca.image - cb.image;
	return OK;
}//SubComplex

Status MultiComplex(Complex ca,Complex cb,Complex &res)
{	//������� 
	res.real = ca.real*cb.real - ca.image*cb.image;
	res.image = ca.real*cb.image + cb.real*ca.image;
	return OK;
}//MultiComplex

Status ShowReal(Complex ca,double &r)
{	//�����ʵ�� 
	r = ca.real;
	return OK;
}//ShowReal

Status ShowImage(Complex ca,double &r)
{	//������鲿 
	r = ca.image;
	return OK;
}//ShowImage

Status ConComplex(Complex ca,Complex &res)
{	//�����Ĺ��� 
	res.real = ca.real;
	res.image = -ca.image;
	return OK;
}//ConComplex

Status DiComplex(Complex ca,Complex cb,Complex &res)
{	//�������� 
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
		//���Ƴ����Ƿ�������� 
		printf("�Ƿ����?[y/n]");
		scanf("%c",&con);getchar();
		if (con == 'n' || con == 'N')	break;
		//�����һ������ 
		printf("�������һ��������");
		while( InitComplex(c1) == ERROR )
			printf("�����������������");
		//���������
		while(1)
		{	
			printf("�����������[+,-,*,/]");
			scanf("%c",&operation);getchar();
			if(operation=='+' || operation=='-' || operation=='*' || operation =='/')
				break;
		}
		//����ڶ������� 
		printf("������ڶ���������");
		while( InitComplex(c2) == ERROR )
			printf("�����������������");
		//���м���
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
		//�����
		if(f==ERROR)	{printf("ERROR\n");continue;}
		if(result.real==0 && result.image==0)	{printf("0\n");continue;}
		if(result.real != 0)	printf("%g",result.real);
		if(result.image != 0)	printf("%+gi",result.image);
		printf("\n");
	}
	return 0;
}
