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
// pokelib9.c 	%I%	%Y%	%E%	%U%
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
#include "hrotsem.h"

/* Declaracao de Variaveis Globais */

extern STACK stack_nest;
extern b_vector undef;
extern char oldlabel[];
static TABDEF * tab_var_def[MAXHASH];
static int var_counter=0;       /* contador de variaveis */


/*******************************************************************/
/* make_null()                                                     */
/* Autor: Marcos L. Chaim                                          */
/* Data: 24/10/89                                                  */
/* Versao: 1.0                                                     */
/*                                                                 */
/* Funcao: inicia as entradas da tabela de variaveis definidas com */
/*         o valor vazio.                                          */
/*                                                                 */
/* Entradas: nenhuma.                                              */
/*                                                                 */
/* Saida: nenhuma.                                                 */
/*                                                                 */
/*******************************************************************/

void make_null()
{
 int i;
 for(i=0; i < MAXHASH; ++i)
     tab_var_def[i] = (TABDEF *) NULL;
 return;
}

/*******************************************************************/
/* inicia_tab_var_def(int, int)                                    */
/* Autor: Marcos L. Chaim                                          */
/* Data: 26/12/94                                                  */
/* Versao: 1.0                                                     */
/*                                                                 */
/* Funcao: inicia a tabela de variaveis definidas de um dado bloco.*/
/*                                                                 */
/* Entradas: bloco e tamanho da tabela.                            */
/*                                                                 */
/* Saida: nenhuma.                                                 */ 
/*                                                                 */
/*******************************************************************/

void inicia_tab_var_def(blk,tam)
int blk;
int tam;
{

int i;

if(blk < MAXHASH)
	{
	if(tam < MAXHASH-1) /* Deixa um espaco para marcar o fim das tabelas */
		{

		/* E' alocado um espaco a mais para marcar o final da 
		** tabela de variaveis definidas do nivel
		*/

		tab_var_def[blk] = (TABDEF *) malloc((tam+1)*sizeof(TABDEF));
		if (tab_var_def[blk] == (TABDEF *) NULL)
			error("Erro Fatal: memoria insuficiente para alocacao\
 da tabela de simbolos");

		/* Inicia com NULL os ponteiros da tab_var_def do nivel */
 		for(i=0; i < tam; ++i)
    			 tab_var_def[blk][i] = (TABDEF) NULL;

		/* Marca o final da tab_var_def do nivel. Util p/ liberar 
		** memoria.
		 */
    		tab_var_def[blk][i] = (TABDEF) -1;
		}
	else
		error("Erro Fatal: tamanho para alocacao da tabela de simbolos \
excedeu o permitido");
	}
else
	error("Erro Fatal: Nivel de aninhamento excedeu o permitido");
}

/*******************************************************************/
/* libera_tabs_from(int )                                          */
/* Autor: Marcos L. Chaim                                          */
/* Data: 26/12/94                                                  */
/* Versao: 1.0                                                     */
/*                                                                 */
/* Funcao: desaloca todas as tabelas de variaveis definidas a par- */
/* de um dado bloco.                                               */
/*                                                                 */
/* Entradas: bloco.                                                */
/*                                                                 */
/* Saida: nenhuma.                                                 */
/*                                                                 */
/*******************************************************************/

void libera_tabs_from(blk)
{
int i;

for(i=blk; tab_var_def[i] != (TABDEF *) NULL; ++i)
	libera_tab_var_def(i);
}

/*******************************************************************/
/* libera_tab_var_def(int)                                         */
/* Autor: Marcos L. Chaim                                          */
/* Data: 26/12/94                                                  */
/* Versao: 1.0                                                     */
/*                                                                 */
/* Funcao: desaloca a tabela de variaveis definidas de um dado     */
/* bloco.                                                          */
/*                                                                 */
/* Entradas: bloco e tamanho da tabela.                            */
/*                                                                 */
/* Saida: nenhuma.                                                 */ 
/*                                                                 */
/*******************************************************************/

void libera_tab_var_def(blk)
int blk;
{
int i;

for(i=0; tab_var_def[blk][i] != (TABDEF) -1; ++i)
	libera_bucket(tab_var_def[blk][i]);

free (tab_var_def[blk]);

tab_var_def[blk]=(TABDEF *) NULL;

}

/*******************************************************************/
/* libera_bucket(TABDEFCELL *)                                     */
/* Autor: Marcos L. Chaim                                          */
/* Data: 26/12/94                                                  */
/* Versao: 1.0                                                     */
/*                                                                 */
/* Funcao: desaloca a lista de buckets da tabela de variaveis      */
/* definidas de um dado bloco.                                     */
/*                                                                 */
/* Entradas: ponteiro para o bucket.                               */
/*                                                                 */
/* Saida: nenhuma.                                                 */ 
/*                                                                 */
/*******************************************************************/

void libera_bucket(p_bucket)
TABDEFCELL * p_bucket;
{
if(p_bucket != (TABDEFCELL *) NULL)
	{
	libera_bucket(p_bucket->next);
	free(p_bucket);
	}
}

/*******************************************************************/
/* zera_local_counter()                                            */
/* Autor: Marcos L. Chaim                                          */
/* Data: 26/12/94                                                  */
/* Versao: 1.0                                                     */
/*                                                                 */
/* Funcao: atribui valor zero ao contador de variaveis locais.     */
/*                                                                 */
/* Entradas: nenhuma.                                              */
/*                                                                 */
/* Saida: nenhuma.                                                 */ 
/*                                                                 */
/*******************************************************************/

void zera_local_counter()
{

}

/*******************************************************************/
/* PAIRINT retr_id(char *;                                         */
/* Autor: Marcos L. Chaim                                          */
/* Data: 24/10/89                                                  */
/* Versao: 1.0                                                     */
/*                                                                 */
/* Funcao: recupera o numero de identificacao da variavel especi-  */
/*         ficada pelo parametro de entrada. Caso nao exista tal   */
/*         variavel retorna-se um numero de identificacao negati-  */
/*         vo.                                                     */
/*                                                                 */
/* Entradas: string de caracteres que descrevem a variavel.        */
/*                                                                 */
/* Saidas: numero de identificacao unico desta variavel.           */
/*                                                                 */
/* Variaveis Globais Utilizadas: stack_nest.                       */
/*                                                                 */
/* Alteracoes:                                                     */
/*                                                                 */
/* 27/12/94 - Marcos L. Chaim                                      */
/* Alteracao adequar a funcao aa nova forma da tab_var_def (com uma*/
/* tabela para cada nivel de aninhamento.                          */
/*                                                                 */
/*******************************************************************/

PAIRINT retr_id(description)
char * description;
{

  /* Declaracao de Variaveis Locais */

  TABDEF current;
  STACK stack_aux;
  int blk_aux, bucket;
  PAIRINT var;

  var.abs = -10;
  var.coor = '\0';

  mk_nl_int(&stack_aux);

  /* Procura description na Tabela de Variaveis Definidas */

  bucket =  hash(description);

  while(!empty_int(&stack_nest))
      {
       blk_aux = pop_int(&stack_nest);
       push_int(blk_aux,&stack_aux);
       current = tab_var_def[blk_aux][bucket];
       while( current != (TABDEF) NULL )
	   {
	    if(!strcmp(description,current->description))
	       {
		var.abs = current->id;
		var.coor = current->type;
		break;
	       }
	    current = current->next;
	   }
       }

   /* Restabelece pilha de aninhamento stack_nest */

   while(!empty_int(&stack_aux))
	push_int(pop_int(&stack_aux),&stack_nest);

   return(var);
}

/********************************************************************/
/* void insert_var(char *,char)                                     */
/* Autor: Marcos L. Chaim                                           */
/* Data: 24/10/89                                                   */
/* Versao: 1.0                                                      */
/*                                                                  */
/* Funcao: insere uma nova variavel declarada em tab_var_def.       */
/*                                                                  */
/* Entradas: string de caracteres que descrevem a variavel e tipo   */
/*           da variavel.                                           */
/*                                                                  */
/* Variaveis Globais Utilizadas: stack_nest, undef.                 */
/*                                                                  */
/********************************************************************/

void insert_var(description,type,nature)
char * description;
int type;
int nature;
{

 /* Declaracao de Variaveis Locais */

 TABDEF oldheader;
 TABDEF current;
 int bucket, blk_cur, scope;

 blk_cur = top_int(&stack_nest);     /* # do bloco corrent */

 /* Verifica se este bloco ja' tem uma tab_var_def inicidada */

 if(tab_var_def[blk_cur] == (TABDEF *) NULL)
	inicia_tab_var_def(blk_cur,SIZEHASH);
	

 bucket = hash(description);
 current = tab_var_def[blk_cur][bucket];
 oldheader = current;


 /* Verifica se esta' ocorrendo redefinicao de variaveis */

 while(current != (TABDEF)NULL)
   {
    if(!strcmp(description,current->description)) /* mesma descricao ? */
		  error("* * Erro Fatal: Variavel redeclarada no mesmo escopo * *\n");
    current = current->next;
   }

 /* Aloca memoria para o bucket */

 tab_var_def[blk_cur][bucket]  = (TABDEF) malloc(sizeof(TABDEFCELL));
 if(tab_var_def[blk_cur][bucket] == (TABDEF) NULL)
     error("* * Erro Fatal: Nao consegui alocar espaco para a tabela de variaveis definidas * *\n");

 /* Salva informacao na tabela de Simbolos */

 (tab_var_def[blk_cur][bucket])->id = (type == VAR)? var_counter++:-1;
 if(type == VAR)
   insert_name(description,var_counter);
 
 strcpy((tab_var_def[blk_cur][bucket])->description,description);
 (tab_var_def[blk_cur][bucket])->blk = blk_cur;
 (tab_var_def[blk_cur][bucket])->type = type;
 (tab_var_def[blk_cur][bucket])->nature = nature;
 (tab_var_def[blk_cur][bucket])->next = oldheader;
 (tab_var_def[blk_cur][bucket])->tok_descr = NULL;
 return;
}

/*******************************************************************/
/* PAIRINT change_type(char *, int);                               */
/* Autor: Marcos L. Chaim                                          */
/* Data: 08/07/95                                                  */
/* Versao: 1.0                                                     */
/*                                                                 */
/* Funcao: altera o tipo de um simbolo na tabela de variaveis      */
/*         definidas.                                              */
/*                                                                 */
/* Entradas: string de caracteres que descrevem a variavel         */
/*            e o novo tipo do simbolo.                            */
/*                                                                 */
/* Saidas: numero de identificacao unico desta variavel e um valor */
/*         negativo caso nao exista a variavel.                    */
/*                                                                 */
/* Variaveis Globais Utilizadas: stack_nest.                       */
/*                                                                 */
/*                                                                 */
/*******************************************************************/

PAIRINT change_type(description,type)
char * description;
int type;
{

  /* Declaracao de Variaveis Locais */

  TABDEF current;
  STACK stack_aux;
  int blk_aux, bucket;
  PAIRINT var;

  var.abs = -10;
  var.coor = '\0';

  mk_nl_int(&stack_aux);

  /* Procura description na Tabela de Variaveis Definidas */

  bucket =  hash(oldlabel);

  while(!empty_int(&stack_nest))
      {
       blk_aux = pop_int(&stack_nest);
       push_int(blk_aux,&stack_aux);
       current = tab_var_def[blk_aux][bucket];
       while( current != (TABDEF) NULL )
	   {
	    if(!strcmp(oldlabel,current->description))
	       {
		var.abs = current->id;
		var.coor = current->type=type;
		break;
	       }
	    current = current->next;
	   }
       }

   /* Restabelece pilha de aninhamento stack_nest */

   while(!empty_int(&stack_aux))
	push_int(pop_int(&stack_aux),&stack_nest);

   return(var);
}

/********************************************************************/
/* int hash(char *)                                                 */
/* Autor: Marcos L. Chaim                                           */
/* Data: 30/10/89                                                   */
/* Versao: 1.0                                                      */
/*                                                                  */
/* Funcao: calcula o indice de uma tabela "hash" a partir de um     */
/*	   "string" de caracteres.                                  */
/*                                                                  */
/* Entradas: "string" de caracteres que descreve a variavel.        */
/*                                                                  */
/********************************************************************/

int hash(description)
char * description;
{

 /* Declaracao de Variaveis Locais */

 int sum, i ;

 sum = 0;
 for(i=0; description[i] != '\0' ; ++i)
    sum = sum + description[i];
 return(sum % SIZEHASH);

}

/*
**				Fim de pokelib9.c
*/
