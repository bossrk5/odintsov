#include "common.h"

#include <string.h>
//���� ���������� ���������
static int input_mat(const char*buf, struct INPUT_INFO*ii)
{
	int n;//����� ����������� ����������
	int ind;//������ � ������� ����������
	double E;//������ ����
			 //����������� ����������� ������� ������, ���������� ���-�� ���������� ������� ������� ���������
	n = sscanf(buf, "%d %lg", &ind, &E);//&-��������� (��������� �� ����������)
	if (n != 2) {
		fprintf(stderr, "bad material: %s\n", buf); // s - ������
		return -2; //������� ������
	}
	if (ind < 0 || ind >= MAX_MAT) {
		fprintf(stderr, "bad material index: %d\n", ind); //� ������� ������ ����
		return -3;
	}
	//������ �������� � ��������� �� ���������
	ii->mat[ind].E = E;//�������� ������ � ����� ����, ����� � ����, � ����� � �������� � �������� ind � �������� � // ������ �� ��������
	return 0;
}

//���� ���������� �������
static int input_sec(const char*buf, struct INPUT_INFO*ii)
{
	int n;//����� ����������� ����������
	int ind;//������ � ������� �������
	double Ix, Wx; //������ ������ ������� � ������ ������������� ������

	printf("%s\n", buf);
	n = sscanf(buf, "%d %lg %lg", &ind, &Ix, &Wx);//&-��������� (��������� �� ����������)
	printf("%d, %6.3f, %6.3f\n", ind, Ix, Wx);
	if (n != 3) {
		fprintf(stderr, "bad section: %s\n", buf);
		return -4;
	}
	if (ind < 0 || ind >= MAX_SEC) {
		fprintf(stderr, "bad section index: %d\n", ind);
		return -5;
	}
	ii->sec[ind].Ix = Ix;//����������
	ii->sec[ind].Wx = Wx;
	return 0;
}

//���� ���������� ������� �����
static int input_beam(const char*buf, struct INPUT_INFO*ii)
{
	int n;//����� ����������� ����������
		   //��������� �� ��������� �������� �������
	struct BEAM_INFO*beam = ii->beam + ii->n_beam;//ii->beam ��� ������� = ��������� 1��� �������� + ���������� �� n_beam - �������� ���������� ��������� �����
												   //����� ������� ��� �������� ����� � ��������� ������� maxbeam
	if (ii->n_beam == MAX_BEAM) {
		fprintf(stderr, "too many beams\n");
		return -6;
	}

	n = sscanf(buf, "%lg %d %d", &beam->l, &beam->mat, &beam->sec);//&-��������� (��������� �� ����������)
	if (n != 3) {
		fprintf(stderr, "bad beam parametres: %s\n", buf);
		return -7;
	}
	beam->p1 = beam->p2 = 0;//�������������� �������� �����������
	beam->ndiv = ii->ndiv_cur;//���������� ���������

	++ii->n_beam;// ��������� ����� �������� �� 1
	return 0;
}

//���� �������� �����������
static int input_displ(const char*buf, struct INPUT_INFO*ii)
{
	int n;//����� ����������� ����������
	struct DISPL_INFO*displ = ii->displ + ii->n_displ;//n_beam-������� ����� �������� ������ (��������� �� ��������� �������� �������)
	if (ii->n_displ == MAX_DISPL) { //����� �������� �����������  ��������� ����
		fprintf(stderr, "too many displacements\n");
		return -8;
	}

	n = sscanf(buf, "%d %lg", &displ->df, &displ->d);//���� ���������� �������.
	switch (n) {
	case 2://������������ 2 ���������
		break;
	case 1://������������ 1 ��������
		displ->d = 0;//�������� ����������� �� ���������
		break;
	default://��������� �������� - ������
		fprintf(stderr, "bad displacement: %s\n", buf); //����� �����. �� ������
		return -9; //������� �������� ������
	}

	displ->nd = ii->n_beam;//����� ���� ��������� � �������� �������
	++ii->n_displ;
	return 0;

}


//���� �������� ��������������� ���,��������
static int input_force(const char*buf, struct INPUT_INFO*ii)
{
	int n;//����� ����������� ����������
	struct FORCE_INFO*force = ii->force + ii->n_force;//n_beam-������� ����� �������� ������ (��������� �� ��������� �������� �������)
	if (ii->n_force == MAX_FORCE) {//��������� ����� ���������� ���
		fprintf(stderr, "too many forces\n");
		return -10;
	}

	n = sscanf(buf, "%d %lg", &force->df, &force->f);
	if (n != 2) {
		fprintf(stderr, "bad force/moment: %s\n", buf);//����� ������
		return -11;
	}


	force->nd = ii->n_beam;//����� ���� ��������� � �������� �������
	++ii->n_force;//���������� ������� � ����. ���
	return 0;

}


//���� �������������� ��������
static int input_q(const char*buf, struct INPUT_INFO*ii)
{
	int n;//����� ����������� ����������
	struct BEAM_INFO*beam = ii->beam + ii->n_beam - 1;//n_beam-������� ����� �������� ������ (��������� �� ��������� �������� �������)
	if (ii->n_beam == 0) {//���� ��� ������� �� �����
		fprintf(stderr, "need beam parameters\n");
		return -12;
	}
	//������� ���� 2 ��������
	n = sscanf(buf, "%lg %lg", &beam->p1, &beam->p2);//�� ��������� beam �������� ���������� ���������, ����� ���������� ���������� ����, � ��������� ��������� ��������� �� ������ ����, � ����� ���������� ��������� �� ������ ���� � ���������
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

//���� ����� ���������
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


//---���������� ������������
int input(FILE*in, struct INPUT_INFO*ii)
{
	//	setlocale(LC_ALL, "Russian");
//���� ���������, ��������� ����� ��� ������ ������� ������

	char buf[1024];//����� ������� ������
	int r = 0;//��������� �����
	ii->ndiv_cur = DEF_NDIV;//��������� ����� ���������
	//���� ����� �����
	while (fgets(buf, sizeof(buf), in)) {
		int n;//����� ����������� �������� (� ������ ������) ���������� ������� � \t - ������ ���������
		//int n = strspn(buf, " \n");
		n = strspn(buf, " \t");//���������� ���������� �������� � ������ ������
		switch (buf[n]) { //�����, buf ������� �������
		case '#':
		case '\n'://������� �� ������
		case '\r'://����� ������ ��������
		case 0:
			break;
		case 'M':
			r = input_mat(buf + n + 1, ii);//���� ���������� ���������
			break;
		case 'S'://���� ����� � ������ ������ S - �� ��������� ��������� ��� �������
			r = input_sec(buf + n + 1, ii);//���� ���������� �������
			break;
		case 'B':
			r = input_beam(buf + n + 1, ii);//���� ���������� ������� �����
			break;
		case 'D':
			r = input_displ(buf + n + 1, ii);//���� ��������� �����������
			break;
		case 'F':
			r = input_force(buf + n + 1, ii);//���� �������� ����
			break;
		case 'Q':
			r = input_q(buf + n + 1, ii);//���� �������������� ��������
			break;
		case 'N':
			r = input_ndiv(buf + n + 1, ii);//���� ����� ���������
			break;
		default://���� ����� �� ������ �� ���� �� ����������������� ���������
			fprintf(stderr, "bad parameter %c\n", buf[n]);
			r = -1;
		}
		if (r) break;//���������� ���������
	}
	return r;//��������� ����� �������
}
