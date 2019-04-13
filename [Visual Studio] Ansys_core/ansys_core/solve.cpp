#include "common.h"
//Прямой и обратный ход метода

//Локальная функция band = ленточная, 1 - прямой ход
//Прямой ход метода Гаусса для ленточной симметричной матрицы
static void solve_band_1(double A[MAX_NDF][MAX_BAND], double F[MAX_NDF], int n, int b)//n-реальн число степ, b=реальн ширина ленты
{
	int i, j, k;
	for (i = 0; i < n; ++i) { // цикл по строкам для исключения
		int p = b; // количество исключающихся строк
		double v; //диагональный элемент
		if (i + p > n) p = n - i; //корректировка значения p
		v = A[i][0];
		for (j = 1; j < p; ++j) { //цикл по исключаемым строкам
			double m = -A[i][j] / v;
			F[i + j] += F[i] * m; //обработка правой части
			for (k = 0; k < b - j; ++k) { //цикл по элементам строки
				A[i + j][k] += A[i][j + k] * m;
			}
		}
	}
}

//Обратный ход метода Гаусса для ленточной симметричной матрицы
//нужно подставить в вектор решения произведение правой части факторизованной на треугольную факторизованную матрицу
//снизу вверх идет const - не изменяются
static void solve_band_2(const double A[MAX_NDF][MAX_BAND], const double F[MAX_NDF], double Q[MAX_NDF], int n, int b)
{
	int i, j;
	for (i = n - 1; i >= 0; --i) {//цикл по строкам снизу вверх
		int p = b;
		double s = F[i];
		//Берем Fi и вычитаем сумму известных элемнтов и делим на диаг. элемент - получаем
		if (i + p > n) p = n - i;
		for (j = 1; j < p; ++j) {
			s -= A[i][j] * Q[i + j];
		}
		Q[i] = s / A[i][0];
	}
}


//подпрограмма решения СЛАУ
int solve(struct SOLVE_INFO*si)
{
	solve_band_1(si->K, si->F, si->ndf, MAX_BAND);
	solve_band_2(si->K, si->F, si->Q, si->ndf, MAX_BAND);
	return 0;
}
