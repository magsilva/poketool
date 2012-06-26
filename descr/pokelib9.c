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
#include "hparserg.h"
#include "hrotsem.h"

/* Declaracao de Variaveis Globais */

extern DESPOINTER names;
extern STACK stack_nest;
extern BITVECTOR undef;
extern TABDEF tab_var_def[];

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
/* Saida: apontador para tabela de variaveis definidas.            */
/*                                                                 */
/*******************************************************************/

void make_null()
{
 int i;
 for(i=0; i < SIZEHASH; ++i)
     tab_var_def[i] = (TABDEF) NULL;
 return;
}

/*******************************************************************/
/* PAIRINT retr_id(char *, int);                                   */
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

  while( !empty_int(&stack_nest) && var.abs == -10 )
      {
       current = tab_var_def[bucket];
       blk_aux = pop_int(&stack_nest);
       push_int(blk_aux,&stack_aux);
       while( current != (TABDEF) NULL )
	   {
	    if(blk_aux == current->blk &&
	       !strcmp(description,current->description))
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

void insert_var(description,type)
char * description;
int type;
{

 /* Declaracao de Variaveis Locais */

 TABDEF oldheader;
 TABDEF current;
 int bucket, blk_cur;
 static int var_counter;  /* contador de variaveis */

 if( var_counter > NMAXVAR-1)
       error("* * Erro Fatal: Numero Maximo de Variaveis Excedido * *\n");

 bucket = hash(description);
 current = tab_var_def[bucket];
 oldheader = current;

 blk_cur = top_int(&stack_nest);     /* # do bloco corrent */

 while(current != (TABDEF)NULL)
   {
    if(!strcmp(description,current->description)) /* mesma descricao ? */
	  if(blk_cur == current->blk && blk_cur != 0)   /* mesmo bloco ?     */
		 {
		  error("* * Erro Fatal: Variavel redeclarada no mesmo escopo * *\n");
		  return;
		 }
	else   /* variavel com mesma descricao mas em escopo diferente */
	   set_bit(current->id, &undef);
    current = current->next;
   }

 tab_var_def[bucket]  = (TABDEF) malloc(sizeof(TABDEFCELL));
 if(tab_var_def[bucket] == (TABDEF) NULL)
     error("* * Erro Fatal: Nao consegui alocar espaco para a tabela de variaveis definidas * *\n");
 strcpy(tab_var_def[bucket]->description,description);
 tab_var_def[bucket]->id = var_counter;
 tab_var_def[bucket]->blk = blk_cur;
 tab_var_def[bucket]->type = type;
 tab_var_def[bucket]->next = oldheader;
 insert_name(description,var_counter);
 ++var_counter;  /* incrementa contador de variaveis */
 return;
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
