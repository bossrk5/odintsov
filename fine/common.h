#ifndef COMMON_H // запрос не открывается ли этот фай повторно
#define COMMON_H


#include <stdio.h>
//Константы
#define MAX_BEAM 1000 //Максимальное число участков
#define MAX_MAT 100
#define MAX_SEC 100
#define MAX_DISPL 100
#define MAX_FORCE 100
#define DEF_NDIV 3


//Структуры данных
#define MAX_NDF ((MAX_BEAM + 1) * 2) //максимальное число степеней свободы
#define MAX_BAND 4 //полуширина ленты

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
    double l,p1,p2;
    int mat,sec,ndiv;
};
struct DISPL_INFO
{
    int nd,df;
    double d;
};
struct FORCE_INFO
{
    int nd,df;
    double f;
};
struct RESULT_INFO
{
 double z;
 int ie;
 double p,Qy,Mx,theta,v,sigma;
};
struct INPUT_INFO
{
 struct MAT_INFO mat[MAX_MAT];
 struct SEC_INFO sec[MAX_SEC];
 int n_beam;//число участков
 struct BEAM_INFO beam[MAX_BEAM];//таблица участокв
 int n_displ;
 struct DISPL_INFO displ[MAX_DISPL];
 int n_force;
 struct FORCE_INFO force[MAX_FORCE];
 int ndiv_cur;//число разбиений
};
//структура состояния решателя
struct SOLVE_INFO
{
    int ndf;//число степеней свободы
    double K[MAX_NDF][MAX_BAND];//Лента матрицы жесткости
    double F[MAX_NDF];//вектор узовых сил и моментов
    double Q[MAX_NDF];//вектор усзловых степеней свободы
};
//подгрограммы
int input(FILE*in,struct INPUT_INFO*ii);//подпрограммы вводы исходных данных
int formke(const struct INPUT_INFO*ii,int ie,double K[4][4],double F[4]);//подпрогммма формирования матрицы жесткостей элемента
//формирование матрицы всей коснтрукции (Ансамблирование)
int formk(const struct INPUT_INFO*ii,struct SOLVE_INFO*si);
//что такое указатель? число, которое описывет положение этого блока в памяти. Указатель *a
//подпрограмма решения СЛАУ
int solve(struct SOLVE_INFO*si);
//подпрогрммма вычисления узловых сил
int getfe(const struct INPUT_INFO*ii,const struct SOLVE_INFO*si,int ie,double Fe[4]);
//подпрограмма вычисления результатов
int results(const struct INPUT_INFO*ii,const struct SOLVE_INFO*si,FILE*out);





#endif // COMMON_H
