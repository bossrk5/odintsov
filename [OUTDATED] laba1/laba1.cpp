// laba1.cpp: определяет точку входа для консольного приложения.
//
#include <iostream>
#include "common.h"

#include <stdio.h>
#include <stdlib.h>
//#pragma warning(suppress : 4996) //fopen

struct INPUT_INFO ii;
struct SOLVE_INFO si;
//FILE *stream, *stream2;

int main()
{
	setlocale(0, "");  //русский
	//результат выполнения текущего шага
	int r=0;
	FILE*in, *out;
	//errno_t in, out;
	//режим чтения входного файла
	in = fopen("input.txt", "rt");
	//in = fopen_s(&stream, "input.txt", "r");
	if (!in) //ошибка открытия файла (in=NULL)
	{
		fprintf(stderr, "Ошибка открытия входного файла\n");
		return 1;
	}
	r = input(in, &ii); //&-указатель на структуру ввод исходных данных
	if (r) //если r!=0  ошибка ввода исходных данных
	{
		fprintf(stderr, "Ошибка ввода данных: %d\n", r); //строка сообщ. об ошибке
		return 2;
	}
	//закрывается файл
	fclose(in);
	//формирование матрицы жесткосте

	r = formk(&ii, &si);
	if (r) //!=0
	{
		fprintf(stderr, "Ошибка формирования матрицы жесткостей: %d\n", r); //ошибка
		return 3;
	}
	//решение системы уравнений задачи
	r = solve(&si);
	if (r)
	{
		fprintf(stderr, "Ошибка решения системы уравнений: %d\n", r);
		return 4;
	}
	//открыть выходной файл на запись
	out = fopen("output.txt", "wt");
	if (!out)
	{
		fprintf(stderr, "Невозможно создать выходной файл\n");
		return 5;
	}

	//запись в файл
	r = results(&ii, &si, out);
	if (r)
	{
		fprintf(stderr, "Ошибка запис результатов/расчета: %d\n", r);
		return 6;

	}
	//закрываем прогу
	fclose(out);
	printf("Solution finished\n");
	return 0;
}

