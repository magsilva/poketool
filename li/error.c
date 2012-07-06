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
// error.c			1.2		94/12/11 11:15:41
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef TURBO_C
#include <malloc.h>
#else
#include <alloc.h>
#endif


#include "util.h"
#include "tabelasli.h"
#include "hparserg.h"
#include "hli.h" 

/* Declaracao de Variaveis Globais */


extern struct table * tab_trans[];
extern struct chave * tab_chave[];
extern int n_chaves;
extern int ntablex;

extern struct cabeca * list,
					 * h,
					 * hdeclaration,
					 * hstatement,
					 * hexpression,
					 * sentinel;
extern struct nodo * p;
extern struct no_tipdef *raiz; /* raiz da arvore binaria de tipos definidos */
extern GOTOLABEL * lista_resul;
extern char * includes[];

void error(msg)
char msg[200];
{

 /* declaracao de variaveis locais */

 int i;
 struct cabeca *aux;
 FILE * logerror;

 /* libera espaco ocupado pelas tabelas de transicao e tabela de palavras
    reservas */

 for(i=0; i>=0 && i<= ntablex; ++i)
	 free(tab_trans[i]);

 for(i=0; i>=0 && i<= n_chaves; ++i)
	 free(tab_chave[i]);

 /* libera espaco ocupado pelo grafo sintatico do parser especifico */

 for(aux = list; aux != sentinel; aux = aux->suc)
	lib_parser(aux->entrada);

 /* libera espaco ocupado pelos apontadores dos grafos relativos aos
	nao-terminais */

 lib_cabeca(list);


 if(sentinel != (struct cabeca *) NULL)
	  free(sentinel);

 printf("%s",msg);

 logerror = (FILE *) fopen("logerror.tes","w");
 if(logerror == (FILE *) NULL)
   {
    printf("%s",msg);
    msg_print("* * Erro Fatal: Nao existe espaco para arquivo logerror.tes * *");
    exit(1);
   }

 lib_tipo(raiz);
 clear_list(lista_resul);

/* Libera area ocupada pelos nomes dos diretorios de Include */

 for(i=0; (i>=0 && i < MAXINC)&& (includes[i] != (char *) NULL); ++i)
    	free(includes[i]); 

 fprintf(logerror,"%d",-1);
 fprintf(logerror,"%s",msg);
 fclose(logerror);

 exit(1);
}

void libera(msg)
char msg[200];
{

 /* declaracao de variaveis locais */

 int i;
 struct cabeca *aux;

 /* libera espaco ocupado pelas tabelas de transicao e tabela de palavras
    reservas */

 for(i=0; i>=0 && i<= ntablex; ++i)
	 free(tab_trans[i]);

 for(i=0; i>=0 && i<= n_chaves; ++i)
	 free(tab_chave[i]);

 /* libera espaco ocupado pelo grafo sintatico do parser especifico */

 for(aux = list; aux != sentinel; aux = aux->suc)
	lib_parser(aux->entrada);

 /* libera espaco ocupado pelos apontadores dos grafos relativos aos
	nao-terminais */

 lib_cabeca(list);


 if(sentinel != (struct cabeca *) NULL)
	  free(sentinel);

 lib_tipo(raiz);
 clear_list(lista_resul);

/* Libera area ocupada pelos nomes dos diretorios de Include */

 for(i=0; (i>=0 && i < MAXINC)&& (includes[i] != (char *) NULL); ++i)
    	free(includes[i]); 


 printf("%s",msg);

}
/********************************************************************/
/* void lib_cabeca(struct cabeca *)                                 */
/* Autor: Marcos L. Chaim                                           */
/* Versao: 1.0                                                      */
/* Data: 16/11/89                                                   */
/*                                                                  */
/* FUNCAO: desaloca espaco ocupado pelos apontadores dos nao-termi- */
/*         nais.                                                    */
/*                                                                  */
/* ENTRADA: apontador para a posicao de memoria onde esta o aponta- */
/*          dor do nao-terminal.                                    */
/*                                                                  */
/* SAIDA: nenhuma.                                                  */
/*                                                                  */
/********************************************************************/

void lib_cabeca(head)
struct cabeca * head;
{
 if( head != sentinel )
	{
	 lib_cabeca(head->suc);
	 free(head);
	}
 return;
}

/********************************************************************/
/* void lib_parser(struct nodo *)                                   */
/* Autor: Marcos L. Chaim                                           */
/* Versao: 1.0                                                      */
/* Data: 16/11/89                                                   */
/*                                                                  */
/* FUNCAO: desaloca espaco ocupado pelos nos do grafo sintatico do  */
/*         parser especifico.                                       */
/*                                                                  */
/* ENTRADA: apontador para a posicao de memoria onde esta o aponta- */
/*          dor do nao-terminal.                                    */
/*                                                                  */
/* SAIDA: nenhuma.                                                  */
/*                                                                  */
/********************************************************************/


void lib_parser(no_parser)
struct nodo * no_parser;
{

 /* Declaracao de Variaveis Locais */

 struct nodo * aux;

 if(no_parser != (struct nodo *) NULL)
   {
    no_parser->sem = MARCADO;
	aux = no_parser->suc;
	if( aux != (struct nodo *) NULL )
	   {
		if(aux->sem != MARCADO)
		   lib_parser(no_parser->suc);
	   }
	aux = no_parser->alt;
	if( aux != (struct nodo *) NULL )
	   {
		if(aux->sem != MARCADO)
		   lib_parser(no_parser->alt);
	   }
	free(no_parser);
   }
 return;
}
