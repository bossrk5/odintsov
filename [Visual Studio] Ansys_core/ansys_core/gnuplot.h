// gnuplot.h
#ifndef _GNUPLOT_H_
#define _GNUPLOT_H_

#include <cstdio>
#include <string>
#include <iostream>

#ifdef WIN32
#define GNUPLOT_NAME "plot.plt -persist"
#else
#define GNUPLOT_NAME "plot.plt  -persist"
#endif

using std::string;
using std::cerr;

class Gnuplot
{
public:
	Gnuplot();
	~Gnuplot();
	void operator ()(const string& command); // отправить команду gnuplot

protected:
	FILE* gnuplotpipe;
};

Gnuplot::Gnuplot()
{
#ifdef WIN32
	gnuplotpipe = _popen(GNUPLOT_NAME, "w");
#else
	gnuplotpipe = popen(GNUPLOT_NAME, "w");
#endif

	if (!gnuplotpipe)
	{
		cerr << ("Gnuplot not found !");
	}
}
Gnuplot::~Gnuplot()
{
	fprintf(gnuplotpipe, "exit\n");

#ifdef WIN32
	_pclose(gnuplotpipe);
#else
	pclose(gnuplotpipe);
#endif
}
void Gnuplot::operator()(const string& command)
{
	fprintf(gnuplotpipe, "%s\n", command.c_str());
	fflush(gnuplotpipe); //без fflush ничего рисоваться не будет
};

#endif // #ifndef _GNUPLOT_H_