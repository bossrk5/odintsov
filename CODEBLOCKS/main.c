// laba1.cpp: ���������� ����� ����� ��� ����������� ����������.
//
#include "common.h"

#include <stdio.h>
#include <stdlib.h>

struct INPUT_INFO ii;
struct SOLV
E_INFO si;

int main()
{
	//��������� ���������� �������� ����
	int r;
	FILE*in, *out;
	//����� ������ �������� �����
	in = fopen("input.txt", "rt");
	if (!in) //������ �������� ����� (in=NULL)
	{
		fprintf(stderr, "err file\n");
		return 1;
	}
	r = input(in,&ii); //&-��������� �� ��������� ���� �������� ������
	if (r) //���� r!=0  ������ ����� �������� ������
	{
		fprintf(stderr, "err input: %d\n",r); //������ �����. �� ������
		return 2;
	}
	//����������� ����
	fclose(in);
	//������������ ������� ���������

	r = formk(&ii, &si);
	if (r) //!=0
	{
		fprintf(stderr, "formk ������: %d\n", r); //������
		return 3;
	}
	//������� ������� ��������� ������
	r = solve(&si);
	if (r)
	{
		fprintf(stderr, "sol err: %d\n", r);
			return 4;
	}
	//������� �������� ���� �� ������
	out = fopen("output.txt", "wt");
	if (!out)
	{
		fprintf(stderr, "unable to create out file\n");
		return 5;
	}

	//������ � ����
	r = results(&ii, &si, out);
	if (r)
	{
		fprintf(stderr, "results error: %d\n",r);
		return 6;

	}
	//��������� �����
	fclose(out);
	printf("Solution finished\n");
	return 0;
}

