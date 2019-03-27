#ifndef COMMON_H // ������ �� ����������� �� ���� ��� ��������
#define COMMON_H


#include <stdio.h>
//���������
#define MAX_BEAM 1000 //������������ ����� ��������
#define MAX_MAT 100
#define MAX_SEC 100
#define MAX_DISPL 100
#define MAX_FORCE 100
#define DEF_NDIV 3


//��������� ������
#define MAX_NDF ((MAX_BEAM + 1) * 2) //������������ ����� �������� �������
#define MAX_BAND 4 //���������� �����

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
 int n_beam;//����� ��������
 struct BEAM_INFO beam[MAX_BEAM];//������� ��������
 int n_displ;
 struct DISPL_INFO displ[MAX_DISPL];
 int n_force;
 struct FORCE_INFO force[MAX_FORCE];
 int ndiv_cur;//����� ���������
};
//��������� ��������� ��������
struct SOLVE_INFO
{
    int ndf;//����� �������� �������
    double K[MAX_NDF][MAX_BAND];//����� ������� ���������
    double F[MAX_NDF];//������ ������ ��� � ��������
    double Q[MAX_NDF];//������ �������� �������� �������
};
//������������
int input(FILE*in,struct INPUT_INFO*ii);//������������ ����� �������� ������
int formke(const struct INPUT_INFO*ii,int ie,double K[4][4],double F[4]);//����������� ������������ ������� ���������� ��������
//������������ ������� ���� ����������� (���������������)
int formk(const struct INPUT_INFO*ii,struct SOLVE_INFO*si);
//��� ����� ���������? �����, ������� �������� ��������� ����� ����� � ������. ��������� *a
//������������ ������� ����
int solve(struct SOLVE_INFO*si);
//������������ ���������� ������� ���
int getfe(const struct INPUT_INFO*ii,const struct SOLVE_INFO*si,int ie,double Fe[4]);
//������������ ���������� �����������
int results(const struct INPUT_INFO*ii,const struct SOLVE_INFO*si,FILE*out);





#endif // COMMON_H
