#include "common.h" //заголовок подключили
#include <string.h> //сист.библиотека
#include <iostream>
//#include "stdafx.h" //компилятор требует
using namespace std;

//подпрограмма не видна из других модулей
//char*buf - указатель на строку с именем буф

//ввод параметров материала
static int input_mat(const char*buf, struct INPUT_INFO*ii)
{
	int n;//число прочитанных параметров
	int ind;//индекс в таблице материалов
	double E;//модуль Юнга

	//стандартная подрограмма разбора строки, возвращает кол-во параметров которые удалось разобрать
	n = sscanf(buf, "%d %lg", &ind, &E); //%d - целочисленный int; %lg - double; &ind получает адрес(указатель на переменную ind) по имени переменной
	//при ошибке вернет кол-во верно прочитанных параметров ( к примеру 1)
	if (n != 2)
	{
		fprintf(stderr, "Плохой материал: %s\n", buf); //s-строка
		return -2;//признак ошибки 
	}
	if (ind < 0 || ind >= MAX_MAT)
	{
		fprintf(stderr, "Индекс плохого материала: %d\n", ind); //к примеру модуль Юнга
		return -3;
	}
	//запись значения в струкруту по указателю
	ii->mat[ind].E = E; // храним по значению
	return 0;
}

//ввод параметров сечения
static int input_sec(const char*buf, struct INPUT_INFO*ii)
{
	int n;//число прочитанных параметров
	int ind;//индекс в таблице сечений
	double Ix, Wx; //осевой момент инерции и момент сопротивления изгибу

	n = sscanf(buf, "%d %lg %lf", &ind, &Ix, &Wx);
	if (n != 3) //ошибка
	{
		fprintf(stderr, "Плохое сечение: %s\n", buf);
		return -4;
	}
	if (ind < 0 || ind >= MAX_SEC)
	{
		fprintf(stderr, "Индекс плохого сечения: %d\n", ind);
		return -5;
	}
	ii->sec[ind].Ix = Ix; //записываем
	ii->sec[ind].Wx = Wx;
	return 0;
}

//ввод параметров участка балки
static int input_beam(const char*buf, struct INPUT_INFO*ii)
{
	int n; //число прочитанных параметров
	//указатель на параметры текущего участка
	struct BEAM_INFO*beam = ii->beam + ii->n_beam; //ii->beam имя массива = указатель 1ого элемента + увеличение на n_beam - текушеее количество элементов балки
	//может оказать что участков много и превысили размеры maxbeam
	if (ii->n_beam == MAX_BEAM)
	{
		fprintf(stderr, "Слишком много балок, ты тупо ошибся!\n");
		return -6;
	}

	n = scanf(buf, "%lg %d %d", &beam->l, &beam->mat, &beam->sec);

	if (n != 3)
	{
		fprintf(stderr, "Плохие параметры балки, ля: %s\n", buf);
		return -7;
	}

	beam->p1 = beam->p2 = 0;//распределенной нагрузки нет
	beam->ndiv = ii->ndiv_cur;//количество разбиений
	++ii->n_beam;// инкремент числа участков на 1
	return 0;
}

//ввод заданного перемещения
static int input_displ(const char*buf, struct INPUT_INFO*ii)
{
	int n;//число прочитанных параметров
	struct DISPL_INFO*displ = ii->displ + ii->n_displ;
	if (ii->n_displ == MAX_DISPL) //число заданных перемещений  превышает макс
	{
		fprintf(stderr, "Слишком много перемещений, я не могу с тобой работать!\n");
		return -8;
	}
	n = sscanf(buf, "%d %lg", &displ->df, &displ->d); //ввод параметров перемещ.
	switch (n)
	{
	case 2: //присутствует 2 параметра
		break;
	case 1:  //присутствует 1 параметр
		displ->d = 0;//значение перемещения по умолчанию
		break;
	default: //остальные ситуации - ошибка
		fprintf(stderr, "Плохие перемещения, ты безнадежен: %s\n", buf); //вывод сообщ. об ошибке
		return -9; //возврат признака ошибки
	}
	displ->nd = ii->n_beam;// номер узла совпадает с номером участка
	++ii->n_displ;
	return 0;
}

//ввод сосредоточенной силы/момента
static int input_force(const char*buf, struct INPUT_INFO*ii)
{
	int n;//число прочитанных параметров
	struct FORCE_INFO*force = ii->force + ii->n_force;// текущий элемент таблицы сил
	if (ii->n_force >= MAX_FORCE) //превышено число сосредоточ сил
	{
		fprintf(stderr, "слишком много сил\n");
		return -10;
	}
	n = sscanf(buf, "%d $lg", &force->df, &force->f);
	if (n != 2)
	{
		fprintf(stderr, "Плохие сила или момент: %s\n", buf);//вывод ошибки
		return -11;
	}
	force->nd = ii->n_beam;//номер узла
	++ii->n_force;//увеличение индекса в табл. сил
	return 0;
}

//ввод распределенной нагрузки
static int input_force(const char*buf, struct INPUT_INFO*ii)
{
	int n;//число прочитанных параметров
	struct BEAM_INFO*beam = ii->beam + ii->n_beam - 1;
	if (ii->n_beam == 0) //если ещё участок не задан
	{
		fprintf(stderr, "Нужно задать параметры участка\n", buf);//вывод ошибки
		return -12;
	}
	//считали макс 2 значения
	n = scanf(buf, "%lg %lg", &beam->p1, &beam->p2);//по указателю beam получаем размещение структуры, затем определяем размещение поля, а амперсанд вычисляет указатель на начало поля, в сканф передается указатель на начало поля в структуре
	switch (n)
	{
	case 2:
		break;
	case 1:
		beam->p2 = beam->p1;
		break;
	default:
		fprintf(stderr, "Плохие параметры распред. нагрузки: %s\n", buf);//вывод ошибки
		return -13;
	}
	return 0;
}

//---реализация подпрограммы
int input(FILE*in, struct INPUT_INFO*ii)
{
	setlocale(LC_ALL, "Russian");
	//ввод построчно, требуется буфер для записи текущей строки
	char buf[1024];//бyфер текущей строки 1024 символа
	int r = 0;//результат ввода

	ii->ndiv_cur = DEF_NDIV; //начальное число разбиений; разбиение по умолчанию

	//цикл ввода строк
	while (fgets(buf, sizeof(buf), in))//сист функция ввода строки in - дескриптор файла
	{
		int n = strspn(buf, " \t");//число нензначащих символов (в начале строки) пропускаем пробелы и \t - символ табуляции
		switch (buf[n]) //выбор, buf элемент массива
		{
		case '#':
		case'\n': //переход на строку
		case'\r': //конец строки отсекаем
		case 0:
			break;
		case 'M':
			r = input_mat(buf + n + 1, ii); //ввод параметров материала
			break;
		case 'S': //если видим в строке символ S - то выполняем программу для сечения
			r = input_sec(buf + n + 1, ii); //ввод параметров сечения
		case 'B':
			r = input_beam(buf + n + 1, ii); //ввод параметров участка балки
		case 'D':
			r = input_displ(buf + n + 1, ii); //ввод заданного перемещения
		case 'F':
			r = input_force(buf + n + 1, ii); //ввод заданной силы
		case 'Q':
			r = input_q(buf + n + 1, ii); //ввод распределенной нагрузки
		case 'N':
			r = input_ndiv(buf + n + 1, ii); //ввод числа разбиений
			break;
		default: //если текст не совпал ни один из вышеперечисленных вариантов
			fprintf(stderr, "Плохие параметры, йопта: %c\n", buf[n]); //вывод ошибок stderr %c -char
			r = -1; //признак ошибки
		}
		if (r) break;//прерывание ввода данных
	}
	return r; //результат ввода данных
}
