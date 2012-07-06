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

/*
// monitor2.c 	%I%	%Y%	%E%	%U%
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef TURBO_C

#include <alloc.h>

#else  /* SunOS, BSD4.2 e RISC 6000 */

#include <malloc.h>

#endif

#include "util.h"
#include "hparserg.h"
#include "headerli.h"
#include "hrotsem.h"
#include "hmonitor.h"
#include "header.h"
#include "cte.h"

/* Declaracao de Variaveis Externas */

extern FILE * mod_arq;
extern NODEINFO info_no;
extern STACK stack_break;
extern STACK stack_break_int;
extern STACK stack_loop;
extern int tab_counter;
extern int num_no;
extern int no_nos;
extern struct grafo * graph;
extern int break_counter;
extern int type_monitor;

void limpa_stack_break_int(void);
void monitor_node_1(int);

/*******************************************************************/
/* void in_while_monitor(int)                                      */
/* Autor: Marcos L. Chaim                                          */
/* Data: 28/11/89                                                  */
/* Versao: 1.0                                                     */
/*                                                                 */
/* FUNCAO: iniciar a monitoracao de comandos do tipo $WHILE.       */
/*                                                                 */
/* ENTRADAS: numero do no' a ser monitorado.                       */
/*                                                                 */
/* SAIDA: nenhuma.                                                 */
/*                        				           */
/* VARIAVEIS GLOBAIS UTILIZDAS: info_no, tab_counter.              */
/*                                                                 */
/*******************************************************************/

void in_while_monitor(num_while)
int num_while;
{
 if(info_no[num_while].to_monitor != ALREADY)
   {
    ajusta_ponta_de_prova(tab_counter);
    fprintf(mod_arq,"ponta_de_prova(%d);\n",num_while);
   }
 return;
}

/*******************************************************************/
/* void fim_while_monitor(int)                                     */
/* Autor: Marcos L. Chaim                                          */
/* Data: 28/11/89                                                  */
/* Versao: 1.0                                                     */
/*                                                                 */
/* FUNCAO: terminar a monitoracao de comandos do tipo $WHILE.      */
/*                                                                 */
/* ENTRADAS: numero do no' a ser monitorado.                       */
/*                                                                 */
/* SAIDA: nenhuma.                                                 */
/*                        				           */
/* VARIAVEIS GLOBAIS UTILIZDAS: info_no, tab_counter.              */
/*                                                                 */
/*******************************************************************/

void fim_while_monitor(num_while)
int num_while;
{
 if(info_no[num_while].to_monitor != ALREADY)
    {
     ajusta_ponta_de_prova(tab_counter);
     fprintf(mod_arq,"ponta_de_prova(%d);\n",num_while);
     info_no[num_while].to_monitor = ALREADY;
    }
 /*
 if(!empty_int(&stack_break_int))
    {
      ajusta_ponta_de_prova(tab_counter);
      fprintf(mod_arq,"out%d_break:;\n",top_int(&stack_break));
      break_counter++;
      limpa_stack_break_int();
    }
    */
 if(top_int(&stack_break_int))
    {
      ajusta_ponta_de_prova(tab_counter);
      fprintf(mod_arq,"out%d_break:;\n",top_int(&stack_break));
    }
   
 return;
}

/*******************************************************************/
/* void in_for_monitor(int)                                        */
/* Autor: Marcos L. Chaim                                          */
/* Data: 28/11/89                                                  */
/* Versao: 1.0                                                     */
/*                                                                 */
/* FUNCAO: iniciar a monitoracao de comandos do tipo $FOR.         */
/*                                                                 */
/* ENTRADAS: numero do no' a ser monitorado.                       */
/*                                                                 */
/* SAIDA: nenhuma.                                                 */
/*                        				           */
/* VARIAVEIS GLOBAIS UTILIZDAS: info_no, tab_counter.              */
/*                                                                 */
/*******************************************************************/


void in_for_monitor(num_for)
int num_for;
{
 if(info_no[num_for].to_monitor != ALREADY)
   {
    monitor_node_1(num_for);
    ajusta_ponta_de_prova(tab_counter);
    fprintf(mod_arq,"ponta_de_prova(%d);\n",num_for);
    info_no[num_for].to_monitor = ALREADY;
   }
 return;
}

/*******************************************************************/
/* void meio_for_monitor(int)                                      */
/* Autor: Marcos L. Chaim                                          */
/* Data: 28/11/89                                                  */
/* Versao: 1.0                                                     */
/*                                                                 */
/* FUNCAO: monitora o interior de comandos do tipo $FOR.           */
/*                                                                 */
/* ENTRADAS: numero do no' a ser monitorado.                       */
/*                                                                 */
/* SAIDA: nenhuma.                                                 */
/*                        				           */
/* VARIAVEIS GLOBAIS UTILIZDAS: info_no, tab_counter.              */
/*                                                                 */
/*******************************************************************/

void meio_for_monitor(num_for)
int num_for;
{
 if(info_no[num_for+1].to_monitor != ALREADY)
    {
     ajusta_ponta_de_prova(tab_counter);
     fprintf(mod_arq,"ponta_de_prova(%d);\n", num_for+1);
    }
 return;
}

/*
void meio2_for_monitor(num_for)
int num_for;
{
 if(info_no[num_for+2].to_monitor != ALREADY)
   {
    ajusta_ponta_de_prova(tab_counter);
    fprintf(mod_arq,"ponta_de_prova(%d);\n", num_for+2);
    info_no[num_for+2].to_monitor = ALREADY;
   }
return;
} */

/*******************************************************************/
/* void fim_for_monitor(int)                                       */
/* Autor: Marcos L. Chaim                                          */
/* Data: 28/11/89                                                  */
/* Versao: 1.0                                                     */
/*                                                                 */
/* FUNCAO: termina de monitorar comandos do tipo $FOR.             */
/*                                                                 */
/* ENTRADAS: numero do no' a ser monitorado.                       */
/*                                                                 */
/* SAIDA: nenhuma.                                                 */
/*                        				           */
/* VARIAVEIS GLOBAIS UTILIZDAS: info_no, tab_counter.              */
/*                                                                 */
/*******************************************************************/

void fim_for_monitor(num_for)
int num_for;
{
 if(info_no[num_for+1].to_monitor != ALREADY)
   {
    ajusta_ponta_de_prova(tab_counter);
    fprintf(mod_arq,"ponta_de_prova(%d);\n",num_for+1);
    info_no[num_for+1].to_monitor = ALREADY;
   }
 /* if(!empty_int(&stack_break_int))
   {
    ajusta_ponta_de_prova(tab_counter);
    fprintf(mod_arq,"out%d_break:;\n",top_int(&stack_break));
    break_counter++;
    limpa_stack_break_int();
   }
 */
 
 if(top_int(&stack_break_int))
    {
      ajusta_ponta_de_prova(tab_counter);
      fprintf(mod_arq,"out%d_break:;\n",top_int(&stack_break));
    }

 return;
}

/*******************************************************************/
/* void in_until_monitor()                                         */
/* Autor: Marcos L. Chaim                                          */
/* Data: 28/11/89                                                  */
/* Versao: 1.0                                                     */
/*                                                                 */
/* FUNCAO: inicia a monitoracao de comandos do tipo $UNTIL.        */
/*                                                                 */
/* ENTRADAS: nenhuma.                                              */
/*                                                                 */
/* SAIDA: nenhuma.                                                 */
/*                        				           */
/* VARIAVEIS GLOBAIS UTILIZDAS: info_no, tab_counter.              */
/*                                                                 */
/*******************************************************************/

void in_until_monitor(num_until)
int num_until;
{
 if(info_no[num_until].to_monitor != ALREADY)
    {
     ajusta_ponta_de_prova(tab_counter);
     fprintf(mod_arq,"ponta_de_prova(%d);\n",num_until);
    }
 return;
}

/*******************************************************************/
/* void fim_until_monitor(int)                                     */
/* Autor: Marcos L. Chaim                                          */
/* Data: 28/11/89                                                  */
/* Versao: 1.0                                                     */
/*                                                                 */
/* FUNCAO: termina de monitorar comandos do tipo $UNTIL.           */
/*                                                                 */
/* ENTRADAS: numero do no' a ser monitorado.                       */
/*                                                                 */
/* SAIDA: nenhuma.                                                 */
/*                        				           */
/* VARIAVEIS GLOBAIS UTILIZDAS: info_no, tab_counter.              */
/*                                                                 */
/*******************************************************************/

void fim_until_monitor(num_until)
int num_until;
{
 if(info_no[num_until].to_monitor != ALREADY)
   {
    ajusta_ponta_de_prova(tab_counter);
    fprintf(mod_arq,"ponta_de_prova(%d);\n",num_until);
    info_no[num_until].to_monitor = ALREADY;
   }
 return;
}
/*******************************************************************/
/* void in_case_monitor(int)                                       */
/* Autor: Marcos L. Chaim                                          */
/* Data: 28/11/89                                                  */
/* Versao: 1.0                                                     */
/*                                                                 */
/* FUNCAO: inicia a monitoracao de comandos do tipo $CASE.         */
/*                                                                 */
/* ENTRADAS: numero do no' a ser monitorado.                       */
/*                                                                 */
/* SAIDA: nenhuma.                                                 */
/*                        				           */
/* VARIAVEIS GLOBAIS UTILIZDAS: info_no, tab_counter.              */
/*                                                                 */
/*******************************************************************/

void in_case_monitor(num_case)
int num_case;
{
 if(info_no[num_case].to_monitor != ALREADY)
   {
    ++tab_counter;
    ajusta_ponta_de_prova(tab_counter);
    fprintf(mod_arq,"{\n");
    ajusta_ponta_de_prova(tab_counter);
    fprintf(mod_arq,"ponta_de_prova(%d);\n",num_case);
    info_no[num_case].to_monitor = ALREADY;
   }
 return;
}

/*******************************************************************/
/* void fim_case_monitor()                                         */
/* Autor: Marcos L. Chaim                                          */
/* Data: 28/11/89                                                  */
/* Versao: 1.0                                                     */
/*                                                                 */
/* FUNCAO: termina a monitoracao de comandos do tipo $CASE.        */
/*                                                                 */
/* ENTRADAS: nenhuma.                                              */
/*                                                                 */
/* SAIDA: nenhuma.                                                 */
/*                        				           */
/* VARIAVEIS GLOBAIS UTILIZDAS: info_no, tab_counter.              */
/*                                                                 */
/*******************************************************************/

void fim_case_monitor()
{
 --tab_counter;
 ajusta_ponta_de_prova(tab_counter);
 fprintf(mod_arq,"}\n");
}

/*******************************************************************/
/* void in_std_monitor(int)                                        */
/* Autor: Marcos L. Chaim                                          */
/* Data: 28/11/89                                                  */
/* Versao: 1.0                                                     */
/*                                                                 */
/* FUNCAO: iniciar a monitoracao de comandos do tipo $S que apare- */
/*         cem "sozinhos" apos os comandos IF, WHILE, REPEAT e     */
/*         ELSE.                                                   */
/*                                                                 */
/* ENTRADAS: numero do no' a ser monitorado.                       */
/*                                                                 */
/* SAIDA: nenhuma.                                                 */
/*                        				           */
/* VARIAVEIS GLOBAIS UTILIZDAS: info_no, tab_counter.              */
/*                                                                 */
/*******************************************************************/

void in_std_monitor(num_std)
int num_std;
{
 if(info_no[num_std].to_monitor != ALREADY)
   {
    ajusta_ponta_de_prova(tab_counter);
    fprintf(mod_arq,"{\n");
    ++tab_counter;
   }
 return;
}

/*******************************************************************/
/* void fim_std_monitor(int)                                       */
/* Autor: Marcos L. Chaim                                          */
/* Data: 28/11/89                                                  */
/* Versao: 1.0                                                     */
/*                                                                 */
/* FUNCAO: termina a monitoracao de comandos do tipo $S que apare- */
/*         cem "sozinhos" apos os comandos IF, WHILE, REPEAT e     */
/*         ELSE.                                                   */
/*                                                                 */
/* ENTRADAS: numero do no' a ser monitorado.                       */
/*                                                                 */
/* SAIDA: nenhuma.                                                 */
/*                                                                 */
/* VARIAVEIS GLOBAIS UTILIZDAS: info_no, tab_counter.              */
/*                                                                 */
/*******************************************************************/

void std_monitor(num_std)
int num_std;
{
 if(info_no[num_std].to_monitor != ALREADY)
   {
    monitor_node_1(num_std);
    ajusta_ponta_de_prova(tab_counter);
    fprintf(mod_arq,"ponta_de_prova(%d);\n", num_std);
    info_no[num_std].to_monitor = ALREADY;
   }
 return;
}

/*******************************************************************/
/* void fecha_chave_monitor(int)                                   */
/* Autor: Marcos L. Chaim                                          */
/* Data: 26/11/89                                                  */
/* Versao: 1.0                                                     */
/*                                                                 */
/* FUNCAO: monitora nos terminados por "}" ou IF, WHILE, FOR e RE- */
/*         PEAT.                                                   */
/*                                                                 */
/* ENTRADAS: numero do no' a ser monitorado.                       */
/*                                                                 */
/* SAIDA: nenhuma.                                                 */
/*                                                                 */
/* VARIAVEIS GLOBAIS UTILIZDAS: info_no.                           */
/*                                                                 */
/*******************************************************************/

void fecha_chave_monitor(num_chave)
int num_chave;
{

if(info_no[num_chave].to_monitor != ALREADY)
   {
    ajusta_ponta_de_prova(tab_counter);
    fprintf(mod_arq,"ponta_de_prova(%d);\n",num_chave);
    info_no[num_chave].to_monitor = ALREADY;
   }
return;
}


/*******************************************************************/
/* void break_monitor(int)                                         */
/* Autor: Marcos L. Chaim                                          */
/* Data: 30/11/89                                                  */
/* Versao: 1.0                                                     */
/*                                                                 */
/* FUNCAO: monitora o comando "break" inserido um comando "goto"   */
/*         quando necessario.                                      */
/*                                                                 */
/* ENTRADAS: numero do no' a ser monitorado.                       */
/*                                                                 */
/* SAIDA: nenhuma.                                                 */
/*                                                                 */
/* VARIAVEIS GLOBAIS UTILIZDAS: info_no, tab_counter, stack_loop.  */
/*                                                                 */
/*******************************************************************/

void break_monitor(num_break)
int num_break;
{
 if(info_no[num_break].to_monitor != ALREADY )
   {
    ajusta_ponta_de_prova(tab_counter);
    fprintf(mod_arq,"ponta_de_prova(%d);\n",num_break);
    info_no[num_break].to_monitor = ALREADY;
   }
 if(top_int(&stack_loop) == IN_LOOP)
   {
    write_no(num_break);
    ajusta_formato(tab_counter);
    fprintf(mod_arq,"goto out%d_break;\n",top_int(&stack_break));
    /*    push_int(break_counter,&stack_break_int); */
    if(top_int(&stack_break_int) == 0)
      {
      pop_int(&stack_break_int);
      push_int(1,&stack_break_int);
      }
   }
 else
  {
   write_no(num_no);
   ajusta_formato(tab_counter);
   fprintf(mod_arq,"break;\n");
  }

}

/*******************************************************************/
/* void cont_monitor(int)                                          */
/* Autor: Marcos L. Chaim                                          */
/* Data: 30/11/89                                                  */
/* Versao: 1.0                                                     */
/*                                                                 */
/* FUNCAO: monitora o comando "continue".                          */
/*                                                                 */
/* ENTRADAS: numero do no' a ser monitorado.                       */
/*                                                                 */
/* SAIDA: nenhuma.                                                 */
/*                                                                 */
/* VARIAVEIS GLOBAIS UTILIZDAS: info_no, tab_counter, stack_loop.  */
/*                                                                 */
/*******************************************************************/

void cont_monitor(num_cont)
int num_cont;
{
 /* Declaracao de Variaveis locais */

 int num_next_cont;

 if(info_no[num_cont].to_monitor != ALREADY)
   {
    ajusta_ponta_de_prova(tab_counter);
    fprintf(mod_arq,"ponta_de_prova(%d);\n",num_cont);
    info_no[num_cont].to_monitor = ALREADY;
   }

 /* obtem o numero do no seguinte ao no' que contem o "continue" */
 num_next_cont = (graph[num_cont].list_suc)->num;

 ajusta_ponta_de_prova(tab_counter);
 fprintf(mod_arq,"ponta_de_prova(%d);\n",num_next_cont);

 return;
}

/*******************************************************************/
/* void last_node_write(int)                                       */
/* Autor: Marcos L. Chaim                                          */
/* Data: 26/7/90                                                   */
/* Versao: 1.0                                                     */
/*                                                                 */
/* FUNCAO: escreve o numero do ultimo no'.                         */
/*                                                                 */
/* ENTRADAS: numero do no' a ser monitorado.                       */
/*                                                                 */
/* SAIDA: nenhuma.                                                 */
/*                                                                 */
/* VARIAVEIS GLOBAIS UTILIZDAS: tab_counter.                       */
/*                                                                 */
/*******************************************************************/

void last_node_write(num)
int num;
{
int num_next_cont;

 if(info_no[num].to_monitor != ALREADY)
	{
  	ajusta_ponta_de_prova(tab_counter);
  	fprintf(mod_arq,"ponta_de_prova(%d);\n",num);
	}

 /* obtem o numero do no seguinte ao no' que contem o "continue" */

 num_next_cont = (graph[num].list_suc)->num;

 ajusta_ponta_de_prova(tab_counter);
 fprintf(mod_arq,"ponta_de_prova(%d);\n",num_next_cont);

}

void limpa_stack_break_int()
{
int break_aux;

while(!empty_int(&stack_break_int))
	{
	break_aux=pop_int(&stack_break_int);
	if(!empty_int(&stack_break_int))
		if(break_aux!=top_int(&stack_break_int))
			break;
	}

return ;
}

void monitor_node_1(node)
int node;
{
  switch(type_monitor)
    {
    case MEMORY:
      if(node == 1)
	{
	  if(!strcmp(func_cur(),"main"))
	    {
	      ajusta_ponta_de_prova(tab_counter);
	      fprintf(mod_arq,"Aval_Start(%d,%s);\n",get_number_funcs(),get_str_funcs());
	    }
	  ajusta_ponta_de_prova(tab_counter);
	  fprintf(mod_arq,"Aval_Set_Func(%d);\n",get_number_func_by_name(func_cur()));
	}
      break;
    case PIPE:
      if(node == 1)
	{
	  if(!strcmp(func_cur(),"main"))
	    {
	      ajusta_ponta_de_prova(tab_counter);
	      fprintf(mod_arq,"Monitor_Create_Pipe();\n");
	    }	  
	  ajusta_ponta_de_prova(tab_counter);
	  fprintf(mod_arq,"Monitor_Init_Func(%d);\n",get_number_func_by_name(func_cur()));
	}
      break;
    }
}
/*
**				Fim de monitor2.c
*/







