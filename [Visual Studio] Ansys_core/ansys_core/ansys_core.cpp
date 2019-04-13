#include <iostream>
#include "common.h"
#include "gnuplot.h"
#include "windows.h"

struct INPUT_INFO ii;
struct SOLVE_INFO si;
//enum { MAXL = 128, MAXC = 256 };

int main()
{
	//char fnbase[MAXC] = "", fnplt[MAXC] = ""; //��� �����
	//setlocale(0, "");  //�������
	int r;//��������� ���������� �������� ����
	FILE*in, *out;
	in = fopen("balka.txt" , "rt");//����������� ������� ����
	if (!in) { //������ �������� ����� (in=NULL)
		fprintf(stderr, "error open file\n");
		return 1;
	}
	r = input(in, &ii);//&-��������� �� ��������� ���� �������� ������
	if (r) {//���� r!=0  ������ ����� �������� ������
		fprintf(stderr, "input error: %d\n", r);
		return 2;
	}
	fclose(in);	//����������� ����
	r = formk(&ii, &si);	//������������ ������� ����������
	if (r) {
		fprintf(stderr, "formk error: %d\n", r);
		return 3;
	}
	r = solve(&si);	//������� ������� ��������� ������
	if (r) {
		fprintf(stderr, "error solving system: %d\n", r);
		return 4;
	}
	out = fopen("output.txt", "wt");//������� �������� ���� �� ������
	if (!out) {
		fprintf(stderr, "unable to create output file");
		return 5;
	}
	r = result(&ii, &si, out);	//������ � ����
	if (r) {
		fprintf(stderr, "Error writing file: %d\n", r);
		return 6;
	}
	//Create PLTot
	Gnuplot plot;
	plot("");
	// ������� ��������
	//ShellExecute('', 'c:\windows\������.bmp', '', '', '');
	//ShellExecute(GetDesktopWindow(), "open", "plots.pdf", NULL, NULL, SW_SHOWNORMAL);
	fclose(out);	//��������� �����
	printf("Solution success!\n");
	return 0;
}
