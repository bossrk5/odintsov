#include "common.h"
#include <string.h>

int formke(const struct INPUT_INFO*ii,int ie,double K[4][4],double F[4])//подпрограммма формирования матрицы жесткостей элемента
{
    const struct BEAM_INFO*beam=ii->beam+ie; //текущий элемент
    double l=beam->l;
    double EIx=ii->mat[beam->mat].E*ii->sec[beam->sec].Ix;
    double dp=beam->p2-beam->p1;
    K[0][0]=K[2][2]=12.0*EIx/l/l/l;
		// ОШИБКА Я 0 НАПИСАЛ
	K[0][1] = K[1][0] = K[0][3] = K[3][0] = 6.0*EIx / l / l;
//У МАТРОСОВОЙ ТАК:
    //K[0][1]=K[1][2]=K[0][3]=K[3][0]=6.0*EIx/l/l;
    K[0][2]=K[2][0]=-K[0][0];
    K[1][1]=K[3][3]=4.0*EIx/l;
    K[1][2]=K[2][1]=K[2][3]=K[3][2]=-K[0][1];
    K[1][3]=K[3][1]=K[1][1]/2.0;
    F[0]=F[2]=beam->p1*l/2.0;
    F[1]=beam->p1*l*l/12.0;
    F[3]=-F[1];
    F[0]+=dp*3.0/20.0*l;
    F[1]+=dp*1.0/30.0*l*l;
    F[2]+=dp*7.0/20.0*l;
    F[3]+=-dp*1.0/20.0*l*l;
    return 0;
}
//формирование матрицы всей конструкции (Ансамблирование)
int formk(const struct INPUT_INFO*ii,struct SOLVE_INFO*si)
{
    int ie, df, iv;
    const double L=1e20; //чтобы исп метод айрнса пейна
    memset(si,0,sizeof(*si));//очистка параметров решателя
    si->ndf=(ii->n_beam+1)*2;//число степеней свободы задачи
    for (ie=0,df=0; ie<ii->n_beam; ++ie, df+=2) {
        double Ke[4][4], Fe[4];
        int i,j;
        formke(ii,ie,Ke,Fe);//вычисление МЖ отдельного элемента
        for (i=0; i<4;++i) {//цикл по строкам МЖ эл-та
            si->F[df+i]+=Fe[i];
            for(j=i;j<4;++j){//цикл по столбцам выше диагонали
                si->K[df+i][j-i]+=Ke[i][j];
            }
        }
    }
    for (iv=0; iv<ii->n_force; ++iv) {//цикл по заданным силам/моментам
        int ndf=ii->force[iv].nd*2+ii->force[iv].df;
        si->F[ndf]+=ii->force[iv].f; //добавляемое значение силы
    }
    for (iv=0; iv<ii->n_displ;++iv){//цикл по заданным перемещениям
        int ndf=ii->displ[iv].nd*2+ii->displ[iv].df;
        si->K[ndf][0]=L;//метод Айронса-Пейна
            si->F[ndf]=L*ii->displ[iv].d;
    }
    return 0;
}

