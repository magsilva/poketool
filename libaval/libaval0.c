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
#include <stdarg.h>

#include "util.h"
#include "hparserg.h"
#include "hrotsem.h"
#include "haval.h"

/* Declaracao de Variaveis Globais */

static AvalStr ** ppfunc;
static AvalStr * curfunc;
static int cur_no_func;
static int no_funcs;

void Aval_Start(int number_funcs, ...) {
 int i;
 char * argv[] = {"mainaval2", "N", "2", "3", "4", "5", "6", "7"};
 int argc = 8;
 /* char * argv[] = {"mainaval2", "N", "2", "3"};
 int argc = 4;
 */
 
 va_list  parameters;
 va_start(parameters, number_funcs);

 no_funcs=number_funcs;

 /* Create pointers to the automata of the functions */

 ppfunc = (AvalStr **) malloc(no_funcs*sizeof(AvalStr *));

 if(ppfunc == NULL)
   error_msg(mem_fault);

 for(i=0; i < no_funcs; ++i)
   {  
   ppfunc[i]=AvalStr_cons((char *) va_arg(parameters, char *));
   AvalStr_init(ppfunc[i],argc,argv);
   }
}

int Aval_Get_Func(void)
{
  return cur_no_func;
}

void Aval_Set_Func(int func_no)
{
  int i;
 
  cur_no_func = func_no;

  if(func_no < 0 || func_no >= no_funcs)
    error_msg(out_of_range);

  curfunc = ppfunc[func_no];
}

void ponta_de_prova(int node)
{
  AvalStr_avaliador(curfunc->pointer,curfunc->N,curfunc->tot_req_exec,&(curfunc->seq_exec), &(curfunc->no_func_invoc),node);
}

void Aval_Exit(void)
{
  int i;

 for(i=0; i < no_funcs; ++i)
   {  
   AvalStr_avaliador(ppfunc[i]->pointer,ppfunc[i]->N,ppfunc[i]->tot_req_exec,&(curfunc->seq_exec), &(curfunc->no_func_invoc),EOF);
   AvalStr_end(ppfunc[i]);
   AvalStr_destroy(ppfunc[i]);
   }
 free(ppfunc);
}



