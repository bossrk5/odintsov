#include "common.h"
//������ � �������� ��� ������

//��������� ������� band = ���������, 1 - ������ ���
//������ ��� ������ ������ ��� ��������� ������������ �������
static void solve_band_1(double A[MAX_NDF][MAX_BAND], double F[MAX_NDF], int n, int b)//n-������ ����� ����, b=������ ������ �����
{
	int i, j, k;
	for (i = 0; i < n; ++i) { // ���� �� ������� ��� ����������
		int p = b; // ���������� ������������� �����
		double v; //������������ �������
		if (i + p > n) p = n - i; //������������� �������� p
		v = A[i][0];
		for (j = 1; j < p; ++j) { //���� �� ����������� �������
			double m = -A[i][j] / v;
			F[i + j] += F[i] * m; //��������� ������ �����
			for (k = 0; k < b - j; ++k) { //���� �� ��������� ������
				A[i + j][k] += A[i][j + k] * m;
			}
		}
	}
}

//�������� ��� ������ ������ ��� ��������� ������������ �������
//����� ���������� � ������ ������� ������������ ������ ����� ��������������� �� ����������� ��������������� �������
//����� ����� ���� const - �� ����������
static void solve_band_2(const double A[MAX_NDF][MAX_BAND], const double F[MAX_NDF], double Q[MAX_NDF], int n, int b)
{
	int i, j;
	for (i = n - 1; i >= 0; --i) {//���� �� ������� ����� �����
		int p = b;
		double s = F[i];
		//����� Fi � �������� ����� ��������� �������� � ����� �� ����. ������� - ��������
		if (i + p > n) p = n - i;
		for (j = 1; j < p; ++j) {
			s -= A[i][j] * Q[i + j];
		}
		Q[i] = s / A[i][0];
	}
}


//������������ ������� ����
int solve(struct SOLVE_INFO*si)
{
	solve_band_1(si->K, si->F, si->ndf, MAX_BAND);
	solve_band_2(si->K, si->F, si->Q, si->ndf, MAX_BAND);
	return 0;
}
