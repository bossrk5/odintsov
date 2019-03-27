#include "common.h"
int getfe(const struct INPUT_INFO*ii, const struct SOLVE_INFO*si, int ie, double Fe[4]) //результаты решения как константы
{
	double Ke[4][4];
	const double *Qe = si->Q + ie * 2; //указатель=массив, Qe - вычисленный вектор узловых степеней элемента
	formke(ii, ie, Ke, Fe);//Fe - аргумент
	Fe[0] = -Fe[0] + Ke[0][0] * Qe[0] + Ke[0][1] * Qe[1] + Ke[0][2] * Qe[2] + Ke[0][3] * Qe[3];
	Fe[1] = -Fe[1] + Ke[1][0] * Qe[0] + Ke[1][1] * Qe[1] + Ke[1][2] * Qe[2] + Ke[1][3] * Qe[3];
	Fe[2] = -Fe[2] + Ke[2][0] * Qe[0] + Ke[2][1] * Qe[1] + Ke[2][2] * Qe[2] + Ke[2][3] * Qe[3];
	Fe[3] = -Fe[3] + Ke[3][0] * Qe[0] + Ke[3][1] * Qe[1] + Ke[3][2] * Qe[2] + Ke[3][3] * Qe[3];
	return 0;
}
int getri(const struct INPUT_INFO*ii, const double Qe[4], const double Fe[4],int ie, double z, struct RESULT_INFO*ri)
{
	const struct BEAM_INFO*beam = ii->beam + ie;//параметры текущего участка
	double p1 = beam->p1, dp = beam->p2 - beam->p1;
	double EIx = ii->mat[beam->mat].E*ii->sec[beam->sec].Ix;
	ri->p = p1 + dp * z / beam->l; //z/l =безразмерный от 0 до 1, то p = p1;p2
	ri->Qy = Fe[0] + p1 * z + dp * z*z / 2.0 / beam->l;
	ri->Mx = -Fe[1] + Fe[0] * z +
		p1 * z * z / 2.0 +
		dp * z*z*z / 6.0 / beam->l;
	ri->theta = Qe[1] + (-Fe[1] * z +
		Fe[0] * z*z / 2.0 +
		p1 * z*z*z / 6.0 +
		dp * z*z*z*z / 24.0 / beam->l) / EIx;
	ri->v = Qe[0] + Qe[1] * z + (-Fe[1] * z*z / 2.0 +
		Fe[0] * z*z*z / 6.0 +
		p1 * z*z*z*z / 24.0 +
		dp * z*z*z*z*z / 120.0 / beam->l) / EIx;
	ri->sigma = ri->Mx / ii->sec[beam->sec].Wx;
	if (ri->sigma < 0)
	{
		ri->sigma = -ri->sigma;
	}
	return 0;
}
int results(const struct INPUT_INFO*ii, const struct SOLVE_INFO*si, FILE*out)
{
	int ie;
	double l0;
	for (ie = 0, l0 = 0; ie < ii->n_beam; ++ie)
	{
		const struct BEAM_INFO*beam = ii->beam + ie;
		double Fe[4];
		int i, nd;
		nd = beam->ndiv + 1;
		getfe(ii, si, ie, Fe);
		for (i = 0; i < nd; ++i)
		{
			double z = beam->l*i / beam->ndiv;
			struct RESULT_INFO ri;
			ri.z = l0 + z; //по значению; -> по указателю
			ri.ie = ie;
			getri(ii, si->Q + ie * 2, Fe, ie, z, &ri);
			fprintf(out, "%g %f %g %g %g %g %g %g\n",
				ri.z, ri.ie, ri.p, ri.Qy, ri.Mx, ri.theta, ri.v, ri.sigma);
		}
		l0 += beam->l;
	}
	return 0;
}
