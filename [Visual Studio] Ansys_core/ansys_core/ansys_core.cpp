#include <iostream>
#include "common.h"

struct INPUT_INFO ii;
struct SOLVE_INFO si;


int main()
{
	//setlocale(0, "");  //русский
	int r;//результат выполнения текущего шага
	FILE*in, *out;
	in = fopen("balka.txt" , "rt");//открывается входной файл
	if (!in) { //ошибка открытия файла (in=NULL)
		fprintf(stderr, "error open file\n");
		return 1;
	}
	r = input(in, &ii);//&-указатель на структуру ввод исходных данных
	if (r) {//если r!=0  ошибка ввода исходных данных
		fprintf(stderr, "input error: %d\n", r);
		return 2;
	}
	fclose(in);	//закрывается файл
	r = formk(&ii, &si);	//формирование матрицы жесткостей
	if (r) {
		fprintf(stderr, "formk error: %d\n", r);
		return 3;
	}
	r = solve(&si);	//решение системы уравнений задачи
	if (r) {
		fprintf(stderr, "error solving system: %d\n", r);
		return 4;
	}
	out = fopen("output.txt", "wt");//открыть выходной файл на запись
	if (!out) {
		fprintf(stderr, "unable to create output file");
		return 5;
	}
	r = result(&ii, &si, out);	//запись в файл
	if (r) {
		fprintf(stderr, "Error writing file: %d\n", r);
		return 6;
	}

	fclose(out);	//закрываем прогу
	printf("Solution success!\n");
	return 0;
}
