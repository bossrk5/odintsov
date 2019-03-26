// laba1.cpp: определяет точку входа для консольного приложения.
//
#include "common.h"

#include <stdio.h>
#include <stdlib.h>

struct INPUT_INFO ii;
struct SOLV
E_INFO si;

int main()
{
	//результат выполнения текущего шага
	int r;
	FILE*in, *out;
	//режим чтения входного файла
	in = fopen("input.txt", "rt");
	if (!in) //ошибка открытия файла (in=NULL)
	{
		fprintf(stderr, "err file\n");
		return 1;
	}
	r = input(in,&ii); //&-указатель на структуру ввод исходных данных
	if (r) //если r!=0  ошибка ввода исходных данных
	{
		fprintf(stderr, "err input: %d\n",r); //строка сообщ. об ошибке
		return 2;
	}
	//закрывается файл
	fclose(in);
	//формирование матрицы жесткосте

	r = formk(&ii, &si);
	if (r) //!=0
	{
		fprintf(stderr, "formk ошибка: %d\n", r); //ошибка
		return 3;
	}
	//решение системы уравнений задачи
	r = solve(&si);
	if (r)
	{
		fprintf(stderr, "sol err: %d\n", r);
			return 4;
	}
	//открыть выходной файл на запись
	out = fopen("output.txt", "wt");
	if (!out)
	{
		fprintf(stderr, "unable to create out file\n");
		return 5;
	}

	//запись в файл
	r = results(&ii, &si, out);
	if (r)
	{
		fprintf(stderr, "results error: %d\n",r);
		return 6;

	}
	//закрываем прогу
	fclose(out);
	printf("Solution finished\n");
	return 0;
}

