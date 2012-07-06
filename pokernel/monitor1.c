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
// monitor1.c 	%I%	%Y%	%E%	%U%
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

#include "headerli.h"
#include "hmonitor.h"
#include "cte.h"

/* Declaracao de Variaveis Globais */

extern FILE * mod_arq;
extern int num_no;
extern int tab_counter;
extern int num_no;
extern long int position_for;
extern int type_monitor;

/*******************************************************************/
/* void write_no(int)                                              */
/* Autor: Marcos L. Chaim                                          */
/* Versao: 1.0                                                     */
/* Data: 25/11/89                                                  */
/*                                                                 */
/* FUNCAO: escrever o numero do no' no arquivo fonte modificado na */
/*         forma de comentario.                                    */
/*                                                                 */
/* ENTRADA: numero do no' a ser impresso.                          */
/*                                                                 */
/* SAIDA: nenhuma.                                                 */
/*                                                                 */
/*******************************************************************/

void write_no(no_number)
int no_number;
{

 /* escreve o numero do no' no arquivo fonte modificado */

 fprintf(mod_arq,"/* %2d */        ",no_number);

}

/*******************************************************************/
/* void for_write_no(int)                                          */
/* Autor: Marcos L. Chaim                                          */
/* Versao: 1.0                                                     */
/* Data: 25/11/89                                                  */
/*                                                                 */
/* FUNCAO: escrever os numeros dos nos da inicializacao, teste e   */
/*	   incremento do "for" no arquivo fonte modificado na      */
/*         forma de comentario.                                    */
/*                                                                 */
/* ENTRADA: numero do no' de inicializacao do "for".               */
/*                                                                 */
/* SAIDA: nenhuma.                                                 */
/*                                                                 */
/*******************************************************************/

void for_write_no(no_number)
int no_number;
{
 position_for = ftell(mod_arq); /* salva a posicao em que vai ser numerado o "for" */
 fprintf(mod_arq,"/* %2d %2d %2d */  ",no_number, no_number+1,
				       no_number+2);
}

/*******************************************************************/
/* void atualiza_num_for(int, int)                                 */
/* Autor: Marcos L. Chaim                                          */
/* Versao: 1.0                                                     */
/* Data: 27/7/90                                                   */
/*                                                                 */
/* FUNCAO: atualizar o numero do incremento na numeracao do "for". */
/*                                                                 */
/* ENTRADA: numero do no' de inicializacao e do incremento do      */
/*          "for".                                                 */
/*                                                                 */
/* SAIDA: nenhuma.                                                 */
/*                                                                 */
/* VARIAVEIS GLOBAIS: position_for, mod_arq.                       */
/*                                                                 */
/*******************************************************************/

void atualiza_num_for(no_for, no_incr)
int no_for;
int no_incr;
{
 long int pos_atual;
 pos_atual = ftell(mod_arq); /* salva a posicao atual do arquivo modificado */

 fseek(mod_arq,position_for,SEEK_SET);

 fprintf(mod_arq,"/* %2d %2d %2d */  ",no_for, no_for+1,
				       no_incr);
 fseek(mod_arq,pos_atual,SEEK_SET); /* retorna a posicao correta */
}

/*******************************************************************/
/* void ajusta_formato(int)                                        */
/* Autor: Marcos L. Chaim                                          */
/* Versao: 1.0                                                     */
/* Data: 25/11/89                                                  */
/*                                                                 */
/* FUNCAO: tabular o arquivo fonte modificado com um numero intei- */
/*	   ro (passado como parametro) de vezes.                   */
/*                                                                 */
/* ENTRADA: numero inteiro de tabulacoes.                          */
/*                                                                 */
/* SAIDA: nenhuma.                                                 */
/*                                                                 */
/*******************************************************************/

void ajusta_formato(n)
int n;
{
 int i;
 for(i=0; i>=0 && i<=n-1; ++i)
     fprintf(mod_arq,"   ");
}

/*******************************************************************/
/* void ajusta_ponta_de_prova(int)                                 */
/* Autor: Marcos L. Chaim                                          */
/* Versao: 1.0                                                     */
/* Data: 28/11/89                                                  */
/*                                                                 */
/* FUNCAO: tabular o arquivo fonte modificado com um numero intei- */
/*	   ro (passado como parametro) de vezes sem insecao de co- */
/*         comentario com o numero do no'.                         */
/*                                                                 */
/* ENTRADA: numero inteiro de tabulacoes.                          */
/*                                                                 */
/* SAIDA: nenhuma.                                                 */
/*                                                                 */
/*******************************************************************/

void ajusta_ponta_de_prova(n)
int n;
{
 int i;
 fprintf(mod_arq,"                ");
 for(i=0; i>=0 && i<=n-1; ++i)
     fprintf(mod_arq,"   ");
}

/*******************************************************************/
/* void escreve_md(char *, int)                                    */
/* Autor: Marcos L. Chaim                                          */
/* Versao: 1.0                                                     */
/* Data: 25/11/89                                                  */
/*                                                                 */
/* FUNCAO: escreve no arquivo fonte modificado um "string" de ca-  */
/*	   racteres.                                               */
/*                                                                 */
/* ENTRADA: "string" de caracteres e numero do no.                 */
/*                                                                 */
/* SAIDA: nenhuma.                                                 */
/*                                                                 */
/* VARIAVEIS GLOBAIS UTILIZADAS: num_no.                           */
/*                                                                 */
/*******************************************************************/

void escreve_md(simb,num)
struct symbol * simb;
int num;
{

 /* Declaracao de Variaveis Locais */


 if(!strcmp(simb->simbolo,"{"))
    {
     write_no(num);
     ajusta_formato(tab_counter);
     tab_counter++;
     fprintf(mod_arq,"{\n");
     return;
    }
 if(!strcmp(simb->simbolo,"}"))
    {
     write_no(num);
     tab_counter--;
     ajusta_formato(tab_counter);
     fprintf(mod_arq,"}\n");
     return;
    }
 if(!strcmp(simb->simbolo,"$FOR"))
    {
     for_write_no(num);
     ajusta_formato(tab_counter); /* tabulacao e' menor porque sao impressos
				       o numero de tres nos; o que corresponde
				       a aproximadamente duas tabulacoes */
     fprintf(mod_arq,"for");
     return;
    }
 if(!strcmp(simb->simbolo,"$WHILE"))
    {
     write_no(num);
     ajusta_formato(tab_counter);
     fprintf(mod_arq,"while");
     return;
    }
 if(!strcmp(simb->simbolo,"$IF"))
    {
     write_no(num);
     ajusta_formato(tab_counter);
     fprintf(mod_arq,"if");
     return;
    }
 if(!strcmp(simb->simbolo,"$CASE"))
    {
     write_no(num);
     ajusta_formato(tab_counter);
     fprintf(mod_arq,"switch");
     return;
    }
 if(!strcmp(simb->simbolo,"$REPEAT"))
    {
     write_no(num);
     ajusta_formato(tab_counter);
     fprintf(mod_arq,"do\n");
     return;
    }
 if(!strcmp(simb->simbolo,"$UNTIL"))
    {
     write_no(num);
     ajusta_formato(tab_counter);
     fprintf(mod_arq,"while");
     return;
    }
 if(!strcmp(simb->simbolo,"$BREAK"))
    {
     write_no(num);
     ajusta_formato(tab_counter);
     fprintf(mod_arq,"break;\n");
     return;
    }
 if(!strcmp(simb->simbolo,"$CONTINUE"))
    {
     write_no(num);
     ajusta_formato(tab_counter);
     fprintf(mod_arq,"continue;\n");
     return;
    }
 if(!strcmp(simb->simbolo,"$ELSE"))
    {
     write_no(num);
     ajusta_formato(tab_counter);
     fprintf(mod_arq,"else");
     return;
    }
 if(!strcmp(simb->simbolo,"$GOTO"))
    {
     write_no(num);
     ajusta_formato(tab_counter);
     fprintf(mod_arq,"goto ");
     return;
    }
 write_no(num);
 ajusta_formato(tab_counter);
 copy_command(simb);
 return;
}

/*******************************************************************/
/* void inicia_monitoracao()                                       */
/* Autor: Marcos L. Chaim                                          */
/* Versao: 2.0 versao modificada em 11/12/89.                      */
/* Data: 7/12/89                                                   */
/*                                                                 */
/* FUNCAO: inicia a monitoracao do programa fonte criando o arqui- */
/*	   vo "paths.tes" e escrevendo no mesmo o numero 0 (zero); */
/*         alem disso e' incluido o "define" de "ponta_de_prova()" */
/*         e o "include" com o "stdio.h".                          */
/*                                                                 */
/* ENTRADA: nenhuma.                                               */
/*                                                                 */
/* SAIDA: nenhuma.                                                 */
/*                                                                 */
/* VARIAVEIS GLOBAIS UTILIZADAS: paths.                            */
/*                                                                 */
/*******************************************************************/

void inicia_monitoracao()
{

 /* inclui no arquivo fonte modificado a funcao ponta_de_prova() */

  switch(type_monitor)
    {
    case PATH:
      fprintf(mod_arq,"\n");
      fprintf(mod_arq,"#include \"poketool.h\"\n");
      fprintf(mod_arq,"\n");
      break;
    case PIPE:
      fprintf(mod_arq,"\n");
      fprintf(mod_arq,"#include \"poketool.h\"\n");
      fprintf(mod_arq,"FILE * pokepath;\n");
      fprintf(mod_arq,"\n");
      break;
    }
 }

/*******************************************************************/
/* void meio_monitoracao()                                         */
/* Autor: Marcos L. Chaim                                          */
/* Versao: 2.0  versao modificada em 11/12/89                      */
/* Data: 8/12/89                                                   */
/*                                                                 */
/* FUNCAO: declara o apontador para o arquivo "paths.tes" e abre   */
/*         esse arquivo.                                           */
/*                                                                 */
/* ENTRADA: nenhuma.                                               */
/*                                                                 */
/* SAIDA: nenhuma.                                                 */
/*                                                                 */
/* VARIAVEIS GLOBAIS UTILIZADAS: tab_counter.                      */
/*                                                                 */
/*******************************************************************/

void meio_monitoracao()
{

  switch(type_monitor)
    {
    case PATH:
      ajusta_ponta_de_prova(tab_counter);
#ifdef TURBO_C
      fprintf(mod_arq,"FILE * path = fopen(\"%s\\path.tes\",\"a\");\n",func_cur());
#else
      fprintf(mod_arq,"FILE * path = fopen(\"%s/path.tes\",\"a\");\n",func_cur());
#endif
      ajusta_ponta_de_prova(tab_counter);
      fprintf(mod_arq,"static int printed_nodes = 0;\n");
      break;

    case MEMORY:
    case PIPE:
      if(strcmp(func_cur(),"main"))
	{ 
	  ajusta_ponta_de_prova(tab_counter);
	  fprintf(mod_arq,"int previous_func  = Monitor_Get_Func();\n");  
	}
      break;

    }

 return;
}

/*******************************************************************/
/* void finaliza_monitoracao()                                     */
/* Autor: Marcos L. Chaim                                          */
/* Versao: 2.0 versao modificada em 11/12/89.                      */
/* Data: 7/12/89                                                   */
/*                                                                 */
/* FUNCAO: insere instrucao no programa modificado fechando o ar-- */
/*         quivo "paths.tes".                                      */
/*                                                                 */
/* ENTRADA: nenhuma.                                               */
/*                                                                 */
/* SAIDA: nenhuma.                                                 */
/*                                                                 */
/* VARIAVEIS GLOBAIS UTILIZADAS: tab_counter.                      */
/*                                                                 */
/*******************************************************************/

void finaliza_monitoracao()
{
  ajusta_ponta_de_prova(tab_counter);
  switch(type_monitor)
    {
    case PATH:
      fprintf(mod_arq,"fclose(path);\n");
      break; 

    case MEMORY:
      if(!strcmp(func_cur(),"main"))
	fprintf(mod_arq,"Aval_Exit();\n");
      else  
	fprintf(mod_arq,"Aval_Set_Func(previous_func);\n");
      break;

    case PIPE:
      if(!strcmp(func_cur(),"main"))
	fprintf(mod_arq,"fclose(pokepath);\n");
      else  
	fprintf(mod_arq,"Monitor_Init_Func(previous_func);\n");
      break;
    }
}

/*******************************************************************/
/* void cria_include()                                             */
/* Autor: Marcos L. Chaim                                          */
/* Versao: 1.0                                                     */
/* Data: 29/09/99                                                  */
/*                                                                 */
/* FUNCAO: cria arquivo de include poketool.h                      */
/*                                                                 */
/* ENTRADA: nenhuma.                                               */
/*                                                                 */
/* SAIDA: nenhuma.                                                 */
/*                                                                 */
/* VARIAVEIS GLOBAIS UTILIZADAS:                                   */
/*                                                                 */
/*******************************************************************/

void cria_include()
{
 /* Cria arquivo de include com a macro ponta_de_prova() */

 FILE * pokeincl;

 pokeincl = (FILE *) fopen("poketool.h", "w");
 if(pokeincl == (FILE *) NULL)
   error("* * Erro Fatal: Nao foi possivel criar arquivo \"poketool.h\" * *");
  switch(type_monitor)
    {
    case PATH: 
      if(!lookup_func("fprintf"))
	  fprintf(pokeincl,"#include <stdio.h>\n");

      fprintf(pokeincl,"\n");
      fprintf(pokeincl,"#define ponta_de_prova(num) if(++printed_nodes %% 10) fprintf(path,\" %%2d \",num);\\\n");
      fprintf(pokeincl,"else fprintf(path,\" %%2d\\n\",num);\n\n"); 
 
      break;

    case PIPE:
      fprintf(pokeincl,"\n");
      if(!get_stdio())
	{
	  fprintf(pokeincl,"#include <stdio.h>\n\n");
	  fprintf(pokeincl,"\n");
	}

      fprintf(pokeincl,"#include <sys/stat.h>\n");
      fprintf(pokeincl,"static int __curfunc;\n");
      fprintf(pokeincl,"#define ponta_de_prova(num)  fprintf(pokepath,\"%%d \",num);\n");
 
      fprintf(pokeincl,"#define Monitor_Create_Pipe() \\\n\tif(mkfifo(\"path.tes\",0755)) {printf(\"mkfifo falhou:\\n\");exit(1);} \\\n\tpokepath=fopen(\"path.tes\",\"w\");\n\n"); 
      fprintf(pokeincl,"#define Monitor_Set_Func(funcnum) (__curfunc=funcnum)\n");
      fprintf(pokeincl,"#define Monitor_Get_Func() (__curfunc)\n");

      fprintf(pokeincl,"#define Monitor_Init_Func(funcnum) (__curfunc=funcnum); \\\n\tfprintf(pokepath,\"@%%d \",funcnum);\n");
      fprintf(pokeincl,"\n"); 
      break;
    }
 
 fclose(pokeincl);
}

/*******************************************************************/
/* void writeIniciador(int)                                        */
/* Autor: Marcos L. Chaim                                          */
/* Versao: 1.0                                                     */
/* Data: 25/7/90                                                   */
/*                                                                 */
/* FUNCAO: escreve o comando inicializador de bloco para a lingua- */
/*         gem C.                                                  */
/*                                                                 */
/* ENTRADA: numero do no' associado ao iniciador.                  */
/*                                                                 */
/* SAIDA: nenhuma.                                                 */
/*                                                                 */
/* VARIAVEIS GLOBAIS UTILIZADAS: mod_arq, tab_counter.             */
/*                                                                 */
/* FUNCOES CHAMADAS: ajusta_ponta_de_prova().                      */
/*                                                                 */
/*******************************************************************/

void writeIniciador(num)
int num;
{
  if(num != 0)                     /* escreve o "abre chave" */
       {
        write_no(num);
        ajusta_formato(tab_counter);
        fprintf(mod_arq,"{\n");
        ++tab_counter;
       }
 else
      {
       ajusta_formato(tab_counter);
       fprintf(mod_arq,"{\n");
       ++tab_counter;
      }
}

/*******************************************************************/
/* void writeFinalizador(int)                                      */
/* Autor: Marcos L. Chaim                                          */
/* Versao: 1.0                                                     */
/* Data: 25/7/90                                                   */
/*                                                                 */
/* FUNCAO: escreve o comando finalizador de bloco para a lingua-   */
/*         gem C.                                                  */
/*                                                                 */
/* ENTRADA: numero do no' associado ao finalizador.                */
/*                                                                 */
/* SAIDA: nenhuma.                                                 */
/*                                                                 */
/* VARIAVEIS GLOBAIS UTILIZADAS: mod_arq, tab_counter.             */
/*                                                                 */
/* FUNCOES CHAMADAS: ajusta_ponta_de_prova().                      */
/*                                                                 */
/*******************************************************************/

void writeFinalizador(num)
int num;
{
  if(num != 0)
    {
     write_no(num);
     --tab_counter;
     ajusta_formato(tab_counter);
     fprintf(mod_arq,"}\n");
    }
 else
    {
     --tab_counter;
     ajusta_formato(tab_counter);
     fprintf(mod_arq,"}\n");
    }
}












