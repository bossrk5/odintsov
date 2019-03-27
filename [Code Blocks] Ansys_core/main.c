#include "common.h"
struct INPUT_INFO ii;
struct SOLVE_INFO si;





int main()
{
int r;
FILE*in,*out;
in=fopen("input.txt","rt");//открывается входной файл
if(!in){ //ошибка открытия файла (in=NULL)
    fprintf(stderr, "unable to open input file\n");
    return 1;
    }
r=input(in,&ii);
if (r) {
    fprintf(stderr, "input failed: %d\n",r);
    return 2;
}
fclose(in);
r=formk(&ii,&si);
if (r) {
    fprintf(stderr, "formk failed: %d\n",r);
    return 3;
}
r=solve(&si);
if (r) {
    fprintf(stderr, "solve failed: %d\n",r);
    return 4;
}
out = fopen("output.txt", "wt");
if (!out) {
    fprintf(stderr, "unable to create output file");
    return 5;
}
r=results(&ii,&si, out);
if (r) {fprintf(stderr, "result failed: %d\n",r);
    return 6;
    }
fclose(out);
printf("solution success\n");
return 0;
}
