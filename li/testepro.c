
#define ponta_de_prova(num) if(++printed_nodes % 10) fprintf(path," %2d ",num);\
else fprintf(path," %2d\n",num);

#include <stdio.h>


int Get1Real(struct charac *p1, struct charac **pp2, double *ureal_ptr)

/*  1 */        {
                   FILE * path = fopen("Get1Real/path.tes","a");
                   static int printed_nodes = 0;
