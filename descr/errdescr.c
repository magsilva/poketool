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

#ifdef TURBO_C

#include <conio.h>
#include <alloc.h>

#else  /* SunOS, BSD4.2 e RISC 6000 */

#include <malloc.h>

#endif

#include "util.h"
#include "hrotsem.h"
#include "tabelasli.h"
#include "hparserg.h"
#include "headerli.h"
#include "header.h"

/* Declaracao de Variaveis Globais */

extern struct grafo * graph, * graphaux;


extern NODEINFO info_no;
extern b_vector undef;
extern b_vector flags;
extern DESPOINTER names;
extern int num_no;
extern int no_nos;
extern int no_vars;
extern int n_des_pdu;
extern int n_des_pu;

/*******************************************************************/
/* void inicia_glb()                                               */
/* Autor: Marcos L. Chaim                                          */
/* Data: 6/11/89                                                   */
/* Versao: 1.0                                                     */
/*                                                                 */
/* Funcao: inicia as estruturas de dados globais. Tabelas de tran- */
/*         sicao e de palavras chaves, os vetores de bits das es-  */
/*         truturas undef, info_no, as pilhas stack_var, stack_un- */
/*         def e stack_nest e a tab_var_def sao iniciadas.         */
/*                                                                 */
/* Entrada: nenhuma.                                               */
/*                                                                 */
/* Saida: nenhuma.                                                 */
/*                                                                 */
/* Variaveis Globais Utilizadas: undef, info_no, stack_var, stack_ */
/*                               nest, stack_scope e tab_var_def,  */
/*                               flags.                            */
/*                                                                 */
/*******************************************************************/

void inicia_glb()
{

 /* Declaracao de Variaveis Locais */

 int i;


 /* Inicia valores dos vetores de bits de info_no */

 for(i = 0; i >=0 && i <= no_nos; ++i)
    {
      /* cria vetores de bits */
      
      b_vector_cons(&(info_no[i].defg_i),no_vars/BYTE+1,0);
      b_vector_cons(&(info_no[i].undef),no_vars/BYTE+1,1);
      b_vector_cons(&(info_no[i].def_ref),no_vars/BYTE+1,0);
      b_vector_cons(&(info_no[i].c_use),no_vars/BYTE+1,0);
      b_vector_cons(&(info_no[i].p_use),no_vars/BYTE+1,0);

      /* inicia vetores de bits */
      
      reset_all(&(info_no[i].defg_i));
      set_all(&(info_no[i].undef));
      reset_all(&(info_no[i].def_ref));
      reset_all(&(info_no[i].p_use));
      reset_all(&(info_no[i].c_use));
      info_no[i].to_monitor = NOTYET;
    }

 /* Inicia vetores de bits flags e undef */
 
 b_vector_cons(&flags,no_nos/BYTE+1,0);
 reset_all(&flags);

 b_vector_cons(&undef,no_nos/BYTE+1,1);
 set_all(&undef);

 /* Inicia armazenador de nomes das variaveis (provisorio) */

 names = (DESPOINTER) NULL;

 /* Inicia os contadores de descritores para os criterios potenciais usos */

 n_des_pdu = 0;
 n_des_pu = 0;

 return;
}


/********************************************************************/
/* void error(char *)                                               */
/* Autor: Marcos L. Chaim                                           */
/* Versao: 1.0                                                      */
/* Data: 4/07/89                                                    */
/*                                                                  */
/* FUNCAO: desaloca espaco ocupado e imprime mensagem de erro.      */
/*                                                                  */
/* ENTRADA: "string" de caracteres que descreve o erro ocorrido.    */
/*                                                                  */
/* SAIDA: nenhuma.                                                  */
/*                                                                  */
/********************************************************************/

void error(msg)
char * msg;
{

 /* declaracao de variaveis locais */

 int i;
 struct cabeca *aux;
 DESPOINTER aux_names;
 FILE * logerror;

 /* libera espaco ocupado pelos grafos de antecessores e sucessores */

 lib_grf(graph,no_nos);
 lib_grf(graphaux,no_nos);

 /* libera espaco ocupado pela estrutura de dados info_no */

 if(info_no != (NODEINFO) NULL)
 	{
        for(i = 0; i >=0 && i <= no_nos; ++i)
           {
           /* destroi vetores de bits */
           b_vector_destroy(&(info_no[i].defg_i));
           b_vector_destroy(&(info_no[i].undef));
           b_vector_destroy(&(info_no[i].def_ref));
           b_vector_destroy(&(info_no[i].c_use));
           b_vector_destroy(&(info_no[i].p_use));
           }
        free(info_no);
        }
 
 /* libera espaco ocupado pela lista dos nomes das variaveis (provisorio) 

 for(aux_names = names;
	 aux_names != (DESPOINTER) NULL; aux_names = aux_names->next)
	  free(aux_names);
*/
 printf("%s",msg);

 logerror = (FILE *) fopen("logerror.tes","w");
 if(logerror == (FILE *) NULL)
   {
    printf("%s\n",msg);
    msg_print("* * Erro Fatal: Nao existe espaco para arquivo logerror.tes * *");
    exit(1);
   }
 fprintf(logerror,"%d",-1);
 fprintf(logerror,"%s",msg);
 fclose(logerror);

 exit(1);
}

/********************************************************************/
/* void lib_mem(char *)                                             */
/* Autor: Marcos L. Chaim                                           */
/* Versao: 1.0                                                      */
/* Data: 4/07/89                                                    */
/*                                                                  */
/* FUNCAO: desaloca espaco ocupado e imprime mensagem.              */
/*                                                                  */
/* ENTRADA: "string" de caracteres que descreve o erro ocorrido.    */
/*                                                                  */
/* SAIDA: nenhuma.                                                  */
/*                                                                  */
/********************************************************************/

void lib_mem(msg)
char msg[200];
{

 /* declaracao de variaveis locais */

 int i;
 struct cabeca *aux;
 DESPOINTER aux_names;
 FILE * logerror;

 /* libera espaco ocupado pelos grafos de antecessores e sucessores */

 lib_grf(graph,no_nos);
 lib_grf(graphaux,no_nos);

 /* libera espaco ocupado pela estrutura de dados info_no */

 if(info_no != (NODEINFO) NULL)
 	{
        for(i = 0; i >=0 && i <= no_nos; ++i)
           {
           /* destroi vetores de bits */
           b_vector_destroy(&(info_no[i].defg_i));
           b_vector_destroy(&(info_no[i].undef));
           b_vector_destroy(&(info_no[i].def_ref));
           b_vector_destroy(&(info_no[i].c_use));
           b_vector_destroy(&(info_no[i].p_use));
           }
        free(info_no);
        }

 /* libera espaco ocupado pela lista dos nomes das variaveis (provisorio) */

 aux_names = names;
 while(aux_names != (DESPOINTER) NULL)
	  {
	    DESPOINTER aux1=aux_names;
            aux_names=aux_names->next;
            free(aux1);
          }


 msg_print(msg);

 logerror = (FILE *) fopen("logerror.tes","w");
 if(logerror == (FILE *) NULL)
   {
    msg_print("* * Erro Fatal: Nao existe espaco para arquivo logerror.tes * *");
    exit(1);
   }
 fprintf(logerror,"%d",0);
 fclose(logerror);
}

/*
**			Fim de errdesc.c
*/


