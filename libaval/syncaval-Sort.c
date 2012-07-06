/*
* Copyright 2012 Marcos Lordello Chaim, José Carlos Maldonado, Mario Jino, 
* CCSL-ICMC <ccsl.icmc.usp.br>
* 
* This file is part of POKE-TOOL.
* 
* POKE-TOOL is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

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
 Aval_Start(19,"copyproto","safeoutfil","field","number","disorder","oldfile","newfile","cant","term","diag","setfil","eol","cmpa","main","sort","merge","qsort","skip","cmp");
 
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
   /*   printf("Func: %d No: %d\r",func_no, num_no); */
    }

 fclose(buff);  
 Aval_Exit();
 printf("Fim Avaliador Sort"); 
}



