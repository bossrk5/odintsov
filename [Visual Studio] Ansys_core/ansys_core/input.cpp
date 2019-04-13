#include "common.h"

#include <string.h>
//ввод параметров материала
static int input_mat(const char*buf, struct INPUT_INFO*ii)
{
	int n;//число прочитанных параметров
	int ind;//индекс в таблице материалов
	double E;//модуль Юнга
			 //стандартная подрограмма разбора строки, возвращает кол-во параметров которые удалось разобрать
	n = sscanf(buf, "%d %lg", &ind, &E);//&-амперсант (указателл на переменную)
	if (n != 2) {
		fprintf(stderr, "bad material: %s\n", buf); // s - строка
		return -2; //признак ошибки
	}
	if (ind < 0 || ind >= MAX_MAT) {
		fprintf(stderr, "bad material index: %d\n", ind); //к примеру модуль Юнга
		return -3;
	}
	//запись значения в струкруту по указателю
	ii->mat[ind].E = E;//получили доступ к инпут инфо, потом к мату, а потом к элементу с индексом ind и элементу Е // храним по значению
	return 0;
}

//ввод параметров сечения
static int input_sec(const char*buf, struct INPUT_INFO*ii)
{
	int n;//число прочитанных параметров
	int ind;//индекс в таблице сечений
	double Ix, Wx; //осевой момент инерции и момент сопротивления изгибу

	printf("%s\n", buf);
	n = sscanf(buf, "%d %lg %lg", &ind, &Ix, &Wx);//&-амперсант (указателл на переменную)
	printf("%d, %6.3f, %6.3f\n", ind, Ix, Wx);
	if (n != 3) {
		fprintf(stderr, "bad section: %s\n", buf);
		return -4;
	}
	if (ind < 0 || ind >= MAX_SEC) {
		fprintf(stderr, "bad section index: %d\n", ind);
		return -5;
	}
	ii->sec[ind].Ix = Ix;//записываем
	ii->sec[ind].Wx = Wx;
	return 0;
}

//ввод параметров участка балки
static int input_beam(const char*buf, struct INPUT_INFO*ii)
{
	int n;//число прочитанных параметров
		   //указатель на параметры текущего участка
	struct BEAM_INFO*beam = ii->beam + ii->n_beam;//ii->beam имя массива = указатель 1ого элемента + увеличение на n_beam - текушеее количество элементов балки
												   //может оказать что участков много и превысили размеры maxbeam
	if (ii->n_beam == MAX_BEAM) {
		fprintf(stderr, "too many beams\n");
		return -6;
	}

	n = sscanf(buf, "%lg %d %d", &beam->l, &beam->mat, &beam->sec);//&-амперсант (указателл на переменную)
	if (n != 3) {
		fprintf(stderr, "bad beam parametres: %s\n", buf);
		return -7;
	}
	beam->p1 = beam->p2 = 0;//распределенная нагрузка отсутствует
	beam->ndiv = ii->ndiv_cur;//количество разбиений

	++ii->n_beam;// инкремент числа участков на 1
	return 0;
}

//ввод заданных перемещений
static int input_displ(const char*buf, struct INPUT_INFO*ii)
{
	int n;//число прочитанных параметров
	struct DISPL_INFO*displ = ii->displ + ii->n_displ;//n_beam-екущеий номер элемента балков (указатель на параметры текущего участка)
	if (ii->n_displ == MAX_DISPL) { //число заданных перемещений  превышает макс
		fprintf(stderr, "too many displacements\n");
		return -8;
	}

	n = sscanf(buf, "%d %lg", &displ->df, &displ->d);//ввод параметров перемещ.
	switch (n) {
	case 2://присутствует 2 параметра
		break;
	case 1://присутствует 1 параметр
		displ->d = 0;//значение перемещения по умолчанию
		break;
	default://остальные ситуации - ошибка
		fprintf(stderr, "bad displacement: %s\n", buf); //вывод сообщ. об ошибке
		return -9; //возврат признака ошибки
	}

	displ->nd = ii->n_beam;//номер узла совпадает с нломером участка
	++ii->n_displ;
	return 0;

}


//ввод заданных сосредоточенных сил,моментов
static int input_force(const char*buf, struct INPUT_INFO*ii)
{
	int n;//число прочитанных параметров
	struct FORCE_INFO*force = ii->force + ii->n_force;//n_beam-екущеий номер элемента балков (указатель на параметры текущего участка)
	if (ii->n_force == MAX_FORCE) {//превышено число сосредоточ сил
		fprintf(stderr, "too many forces\n");
		return -10;
	}

	n = sscanf(buf, "%d %lg", &force->df, &force->f);
	if (n != 2) {
		fprintf(stderr, "bad force/moment: %s\n", buf);//вывод ошибки
		return -11;
	}


	force->nd = ii->n_beam;//номер узла совпадает с нломером участка
	++ii->n_force;//увеличение индекса в табл. сил
	return 0;

}


//ввод распределенной нагрузки
static int input_q(const char*buf, struct INPUT_INFO*ii)
{
	int n;//число прочитанных параметров
	struct BEAM_INFO*beam = ii->beam + ii->n_beam - 1;//n_beam-екущеий номер элемента балков (указатель на параметры текущего участка)
	if (ii->n_beam == 0) {//если ещё участок не задан
		fprintf(stderr, "need beam parameters\n");
		return -12;
	}
	//считали макс 2 значения
	n = sscanf(buf, "%lg %lg", &beam->p1, &beam->p2);//по указателю beam получаем размещение структуры, затем определяем размещение поля, а амперсанд вычисляет указатель на начало поля, в сканф передается указатель на начало поля в структуре
	switch (n) {
	case 2:
		break;
	case 1:
		beam->p2 = beam->p1;
		break;
	default:
		fprintf(stderr, "bad beam disributed load parameters: %s\n", buf);
		return -13;
	}
	return 0;

}

//ввод числа разбиений
int input_ndiv(const char*buf, struct INPUT_INFO*ii)
{
	int n;
	n = sscanf(buf, "%d", &ii->ndiv_cur);
	if (n != 1) {
		fprintf(stderr, "bad ndiv parameter: %s", buf);
		return -14;
	}
	if (ii->ndiv_cur <= 0) {
		fprintf(stderr, "bad ndiv value: %d\n", ii->ndiv_cur);
		return -15;
	}
	return 0;
}


//---реализация подпрограммы
int input(FILE*in, struct INPUT_INFO*ii)
{
	//	setlocale(LC_ALL, "Russian");
//ввод построчно, требуется буфер для записи текущей строки

	char buf[1024];//буфер текущей строки
	int r = 0;//результат ввода
	ii->ndiv_cur = DEF_NDIV;//начальное число разбиение
	//цикл ввода строк
	while (fgets(buf, sizeof(buf), in)) {
		int n;//число нензначащих символов (в начале строки) пропускаем пробелы и \t - символ табуляции
		//int n = strspn(buf, " \n");
		n = strspn(buf, " \t");//количество незначащих символов в начале строки
		switch (buf[n]) { //выбор, buf элемент массива
		case '#':
		case '\n'://переход на строку
		case '\r'://конец строки отсекаем
		case 0:
			break;
		case 'M':
			r = input_mat(buf + n + 1, ii);//ввод параметров материала
			break;
		case 'S'://если видим в строке символ S - то выполняем программу для сечения
			r = input_sec(buf + n + 1, ii);//ввод параметров сечения
			break;
		case 'B':
			r = input_beam(buf + n + 1, ii);//ввод параметров участка балки
			break;
		case 'D':
			r = input_displ(buf + n + 1, ii);//ввод заданного перемещения
			break;
		case 'F':
			r = input_force(buf + n + 1, ii);//ввод заданной силы
			break;
		case 'Q':
			r = input_q(buf + n + 1, ii);//ввод распределенной нагрузки
			break;
		case 'N':
			r = input_ndiv(buf + n + 1, ii);//ввод числа разбиений
			break;
		default://если текст не совпал ни один из вышеперечисленных вариантов
			fprintf(stderr, "bad parameter %c\n", buf[n]);
			r = -1;
		}
		if (r) break;//прерывание программы
	}
	return r;//результат ввода данныъх
}
