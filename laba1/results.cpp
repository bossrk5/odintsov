#include "common.h"

//Известны узловые силы, нужно найти какая часть относится к конкретному элементу. 
//Вычислить силы

//подпрограмма вычисления узловых сил, приходящихся на конкретный элемент Fe
int getfe(const struct INPUT_INFO*ii, const struct SOLVE_INFO*si, int ie, double Fe[4]) //результаты решения как константы
{
	double Ke[4][4];
	const double *Qe = si->Q + ie * 2; //указатель=массив, Qe - вычисленный вектор узловых степеней элемента
	formke(ii, ie, Ke, Fe);//Fe - аргумент
	//нужно из сил вычисть те силы, которые получены в результате умножения матрицы K на Q и поменять знак
	//Произведение матриц Ke и Qe

	//for (int i=0;i<4;i++)
	// {F[i]= -Fe[i]+Ke[i][0] * Qe[0] + Ke[i][1] * Qe[1] + Ke[i][2] * Qe[2] + Ke[i][3] * Qe[3];}

	Fe[0] = -Fe[0] + Ke[0][0] * Qe[0] + Ke[0][1] * Qe[1] + Ke[0][2] * Qe[2] + Ke[0][3] * Qe[3];
	Fe[1] = -Fe[1] + Ke[1][0] * Qe[0] + Ke[1][1] * Qe[1] + Ke[1][2] * Qe[2] + Ke[1][3] * Qe[3];
	Fe[2] = -Fe[2] + Ke[2][0] * Qe[0] + Ke[2][1] * Qe[1] + Ke[2][2] * Qe[2] + Ke[2][3] * Qe[3];
	Fe[3] = -Fe[3] + Ke[3][0] * Qe[0] + Ke[3][1] * Qe[1] + Ke[3][2] * Qe[2] + Ke[3][3] * Qe[3];
	return 0;
}

//Вычисление результатов для произвольного сечения
int getri(const struct INPUT_INFO*ii, const double Qe[4], const double Fe[4],
	int ie, double z, struct RESULT_INFO*ri)
{
	const struct BEAM_INFO*beam = ii->beam + ie;//параметры текущего участка
	double p1 = beam->p1, dp = beam->p2 - beam->p1;
	double EIx = ii->mat[beam->mat].E*ii->sec[beam->sec].Ix;

}

//подпрограмма вычисления результатов
int results(const struct INPUT_INFO**ii, const struct SOLVE_INFO*si, FILE*out)
{

}
