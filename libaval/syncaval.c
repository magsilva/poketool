#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "util.h"
#include "hrotsem.h"
#include "haval.h"

void main()
{

FILE * buff;
int num_no, func_no;
char bigstr[1000];
 
 buff = fopen("path.tes","r");
 if (buff==NULL)
   {
   printf("Erro nao abri o path.tes\n");
   exit(1);
   }

 Aval_Start(2,"main","find"); 
 
 while(!feof(buff))
   {
   fscanf(buff,"%s ", bigstr);
 
   switch(bigstr[0])
     {
      case '@':
         func_no=atoi(bigstr+1);
         Aval_Set_Func(func_no);
         /* printf("Entrei Funcao %d\n",func_no); */
         break;
      default:
	 num_no=atoi(bigstr);
         ponta_de_prova(num_no);
         /* printf("%d\n",num_no); */
         break;
      }
    }

 fclose(buff);  
 Aval_Exit();
 printf("Fim"); 
}



