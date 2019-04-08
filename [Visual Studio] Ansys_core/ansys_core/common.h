#pragma once
#ifndef COMMON_H // def
#define COMMON_H


#include <stdio.h>
//Êîíñòàíòû
#define MAX_BEAM 1000 //Ìàêñèìàëüíîå ÷èñëî ó÷àñòêîâ
#define MAX_MAT 100
#define MAX_SEC 100
#define MAX_DISPL 100
#define MAX_FORCE 100
#define DEF_NDIV 3


//Ñòðóêòóðû äàííûõ
#define MAX_NDF ((MAX_BEAM + 1) * 2) //ìàêñèìàëüíîå ÷èñëî ñòåïåíåé ñâîáîäû
#define MAX_BAND 4 //ïîëóøèðèíà ëåíòû

struct MAT_INFO
{
	double E;
};
struct SEC_INFO
{
	double Ix;
	double Wx;
};
struct BEAM_INFO
{
	double l, p1, p2;
	int mat, sec, ndiv;
};
struct DISPL_INFO
{
	int nd, df;
	double d;
};
struct FORCE_INFO
{
	int nd, df;
	double f;
};
struct RESULT_INFO
{
	double z;
	int ie;
	double p, Qy, Mx, theta, v, sigma;
};
struct INPUT_INFO
{
	struct MAT_INFO mat[MAX_MAT];
	struct SEC_INFO sec[MAX_SEC];
	int n_beam;//÷èñëî ó÷àñòêîâ
	struct BEAM_INFO beam[MAX_BEAM];//òàáëèöà ó÷àñòîêâ
	int n_displ;
	struct DISPL_INFO displ[MAX_DISPL];
	int n_force;
	struct FORCE_INFO force[MAX_FORCE];
	int ndiv_cur;//÷èñëî ðàçáèåíèé
};
//ñòðóêòóðà ñîñòîÿíèÿ ðåøàòåëÿ
struct SOLVE_INFO
{
	int ndf;//÷èñëî ñòåïåíåé ñâîáîäû
	double K[MAX_NDF][MAX_BAND];//Ëåíòà ìàòðèöû æåñòêîñòè
	double F[MAX_NDF];//âåêòîð óçîâûõ ñèë è ìîìåíòîâ
	double Q[MAX_NDF];//âåêòîð óñçëîâûõ ñòåïåíåé ñâîáîäû
};
//ïîäãðîãðàììû
int input(FILE*in, struct INPUT_INFO*ii);//ïîäïðîãðàììû ââîäû èñõîäíûõ äàííûõ
int formke(const struct INPUT_INFO*ii, int ie, double K[4][4], double F[4]);//ïîäïðîãìììà ôîðìèðîâàíèÿ ìàòðèöû æåñòêîñòåé ýëåìåíòà
//ôîðìèðîâàíèå ìàòðèöû âñåé êîñíòðóêöèè (Àíñàìáëèðîâàíèå)
int formk(const struct INPUT_INFO*ii, struct SOLVE_INFO*si);
//÷òî òàêîå óêàçàòåëü? ÷èñëî, êîòîðîå îïèñûâåò ïîëîæåíèå ýòîãî áëîêà â ïàìÿòè. Óêàçàòåëü *a
//ïîäïðîãðàììà ðåøåíèÿ ÑËÀÓ
int solve(struct SOLVE_INFO*si);
//ïîäïðîãðìììà âû÷èñëåíèÿ óçëîâûõ ñèë
int getfe(const struct INPUT_INFO*ii, const struct SOLVE_INFO*si, int ie, double Fe[4]);
//ïîäïðîãðàììà âû÷èñëåíèÿ ðåçóëüòàòîâ
int result(const struct INPUT_INFO*ii, const struct SOLVE_INFO*si, FILE*out);





#endif // COMMON_H
