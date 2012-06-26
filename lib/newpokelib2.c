/*
// newpokelib2.c 	%I%	%Y%	%E%	%U%
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
#include "hparserg.h"
#include "hrotsem.h"

/*******************************************************************/
/*                                                                  */
/* Procedimento e Funcoes Associadas as Pilhas de Inteiros, de Pa-  */
/* res de Inteiros e de Vetores de Bits                             */
/*                                                                  */
/* Autor: Marcos L. Chaim                                           */
/* Data: 30/10/89                                                   */
/* Versao: 1.0                                                      */
/*                                                                  */
/********************************************************************/

/********************************************************************/
/*                                                                  */
/*           FUNCOES ASSOCIADAS A PILHA DE INTEIROS                 */
/*                                                                  */
/********************************************************************/


void mk_nl_int(s)
STACK  * s;
{
 s->top = MAXLENGTH;
 return;
}

int empty_int(s)
STACK * s;
{
 if(s->top > MAXLENGTH - 1)
	    return(TRUE);
 else
	    return(FALSE);
}

int top_int(s)
STACK * s;
{
 if(empty_int(s))
      error("* * Erro Fatal: Pilha de inteiros vazia * *\n");
 return(s->elements[s->top]);
}

int pop_int(s)
STACK * s;
{

 /* Declaracao de Variaveis Locais */

 int topo;

 if(empty_int(s))
     error("* * Erro Fatal: Pilha de inteiros vazia * * \n");
 topo = s->elements[s->top];
 s->top = s->top+1;
 return(topo);
 }

 void push_int(x,s)
 int x;
 STACK * s;
 {
  if(s->top == 0)
      error("* * Erro Fatal: Pilha de inteiros cheia * *\n");
  s->top = s->top - 1;
  s->elements[s->top] = x;
  return;
 }

/********************************************************************/
/*                                                                  */
/*     FUNCOES ASSOCIADAS A PILHA DE PARES DE INTEIROS              */
/*                                                                  */
/********************************************************************/


void mk_nl_pair(s)
STACKPAIR  * s;
{
 s->top = MAXLENGTH;
 return;
}

int empty_pair(s)
STACKPAIR * s;
{
 if(s->top > MAXLENGTH - 1)
	    return(TRUE);
 else
	    return(FALSE);
}

PAIRINT top_pair(s)
STACKPAIR * s;
{
 if(empty_pair(s))
      error("* * Erro Fatal: Pilha vazia * *\n");
 return(s->elements[s->top]);
}

PAIRINT pop_pair(s)
STACKPAIR * s;
{

 /* Declaracao de Variaveis Locais */

 PAIRINT topo;

 if(empty_pair(s))
     error("* * Erro Fatal: Pilha vazia * *\n");
 topo = s->elements[s->top];
 s->top = s->top+1;
 return(topo);
 }

 void push_pair(x,s)
 PAIRINT * x;
 STACKPAIR * s;
 {
  if(s->top == 0)
      error("* * Erro Fatal: Pilha cheia * *\n");
  s->top = s->top - 1;
  s->elements[s->top] = *x;
  return;
 }

/********************************************************************/
/*                                                                  */
/*     FUNCOES ASSOCIADAS A PILHA DE PARES DE REAIS                 */
/*                                                                  */
/********************************************************************/


void mk_nl_mix(s)
STACKMIX  * s;
{
 s->top = MAXLENGTH;
 return;
}

int empty_mix(s)
STACKMIX * s;
{
 if(s->top > MAXLENGTH - 1)
	    return(TRUE);
 else
	    return(FALSE);
}

MIX top_mix(s)
STACKMIX * s;
{
 if(empty_mix(s))
      error("* * Erro Fatal: Pilha vazia * *\n");
 return(s->elements[s->top]);
}

MIX pop_mix(s)
STACKMIX * s;
{

 /* Declaracao de Variaveis Locais */

 MIX topo;

 if(empty_mix(s))
     error("* * Erro Fatal: Pilha vazia * *\n");
 topo = s->elements[s->top];
 s->top = s->top+1;
 return(topo);
 }

 void push_mix(x,s)
 MIX * x;
 STACKMIX * s;
 {
  if(s->top == 0)
      error("* * Erro Fatal: Pilha cheia * *\n");
  s->top = s->top - 1;
  s->elements[s->top] = *x;
  return;
 }

/********************************************************************/
/*                                                                  */
/*     FUNCOES ASSOCIADAS A PILHA DE VETORES DE BITS                */
/*                                                                  */
/********************************************************************/

void mk_nl_bitvector(s)
STACKBITVECTOR  * s;
{
 s->top = MAXLENGTH;
 return;
}

int empty_bitvector(s)
STACKBITVECTOR * s;
{
 if(s->top > MAXLENGTH - 1)
	    return(TRUE);
 else
	    return(FALSE);
}

b_vector top_bitvector(s)
STACKBITVECTOR * s;
{
 if(empty_bitvector(s))
      error("* * Erro Fatal: Pilha vazia * *\n");
 return(s->elements[s->top]);
}

b_vector pop_bitvector(s)
STACKBITVECTOR * s;
{

 /* Declaracao de Variaveis Locais */

 int top_size;
 b_vector topo;
 
 if(empty_bitvector(s))
     error("* * Erro Fatal: Pilha vazia * *\n");
 
 topo=s->elements[s->top]; 
 
 s->top = s->top+1;
 return(topo);
}

void push_bitvector(x,s)
b_vector * x;
STACKBITVECTOR * s;
{
 b_vector topo;
 
 if(s->top == 0)
   error("* * Erro Fatal: Pilha cheia * *\n");

 b_vector_cons(&topo, VARTEST,x->type_in);
 b_vector_cpy(&topo,x);
 s->top = s->top - 1;
 s->elements[s->top] = topo;
 
 return;
}

/********************************************************************/
/*                                                                  */
/*              FUNCOES ASSOCIADAS A PILHA DE ELEMSUC               */
/*                                                                  */
/********************************************************************/

void mk_nl_elemsuc(s)
STACKELEMSUC  * s;
{
 s->top = MAXLENGTH;
 return;
}

int empty_elemsuc(s)
STACKELEMSUC * s;
{
 if(s->top > MAXLENGTH - 1)
	    return(TRUE);
 else
	    return(FALSE);
}

ELEMSUC top_elemsuc(s)
STACKELEMSUC * s;
{
 if(empty_elemsuc(s))
      error("* * Erro Fatal: Pilha vazia * *\n");
 return(s->elements[s->top]);
}

ELEMSUC pop_elemsuc(s)
STACKELEMSUC * s;
{

 /* Declaracao de Variaveis Locais */

 ELEMSUC topo;

 if(empty_elemsuc(s))
     error("* * Erro Fatal: Pilha vazia * *\n");
 topo = s->elements[s->top];
 s->top = s->top+1;
 return(topo);
}

void push_elemsuc(x,s)
ELEMSUC * x;
STACKELEMSUC * s;
 {
  ELEMSUC topo;
  
  if(s->top == 0)
      error("* * Erro Fatal: Pilha cheia * *\n");

  /* copia campos de ELEMSUC */
  
  topo.apont=x->apont;

  /* copia campos de INFODFNO */
  
  b_vector_cons(&(topo.infosuc.deff),(*x).infosuc.deff.nbytes,(*x).infosuc.deff.type_in);
  b_vector_cpy(&(topo.infosuc.deff),&((*x).infosuc.deff));  

  topo.infosuc.num_no_G=(*x).infosuc.num_no_G;

  s->top = s->top - 1;
  s->elements[s->top] = topo;
  return;
 }

/********************************************************************/
/*                                                                  */
/*       FUNCOES ASSOCIADAS A PILHA DE ENDERECOS NO GRAFO(I)        */
/*                                                                  */
/********************************************************************/

void mk_nl_address(s)
STACKADDRESS  * s;
{
 s->top = MAXLENGTH;
 return;
}

int empty_address(s)
STACKADDRESS * s;
{
 if(s->top > MAXLENGTH - 1)
	    return(TRUE);
 else
	    return(FALSE);
}

NOGRAFOI * top_address(s)
STACKADDRESS * s;
{
 if(empty_address(s))
      error("* * Erro Fatal: Pilha vazia * *\n");
 return(s->elements[s->top]);
}

NOGRAFOI * pop_address(s)
STACKADDRESS * s;
{

 /* Declaracao de Variaveis Locais */

 NOGRAFOI * topo;

 if(empty_address(s))
     error("* * Erro Fatal: Pilha vazia * *\n");
 topo = s->elements[s->top];
 s->top = s->top+1;
 return(topo);
}

void push_address(x,s)
NOGRAFOI * x;
STACKADDRESS * s;
 {
  if(s->top == 0)
      error("* * Erro Fatal: Pilha cheia * *\n");
  s->top = s->top - 1;
  s->elements[s->top] = x;
  return;
 }


/********************************************************************/
/*                                                                  */
/*              FUNCOES ASSOCIADAS A PILHA DE ARCINFO               */
/*                                                                  */
/********************************************************************/

void mk_nl_arc(s)
STACKARC  * s;
{
 s->top = MAXLENGTH;
 return;
}

int empty_arc(s)
STACKARC * s;
{
 if(s->top > MAXLENGTH - 1)
	    return(TRUE);
 else
	    return(FALSE);
}

ARCINFO top_arc(s)
STACKARC * s;
{
 if(empty_arc(s))
      error("* * Erro Fatal: Pilha vazia * *\n");
 return(s->elements[s->top]);
}

ARCINFO pop_arc(s)
STACKARC * s;
{

 /* Declaracao de Variaveis Locais */

 ARCINFO topo;

 if(empty_arc(s))
     error("* * Erro Fatal: Pilha vazia * *\n");
 topo = s->elements[s->top];
 s->top = s->top+1;
 return(topo);
}

void push_arc(x,s)
ARCINFO * x;
STACKARC * s;
 {
  if(s->top == 0)
      error("* * Erro Fatal: Pilha cheia * * \n");
  s->top = s->top - 1;
  s->elements[s->top] = *x;
  return;
 }

/********************************************************************/
/*                                                                  */
/*              FUNCOES ASSOCIADAS A PILHA DE REPINFO               */
/*                                                                  */
/********************************************************************/

void mk_nl_rep(s)
STACKREP  * s;
{
 s->top = MAXLENGTH;
 return;
}

int empty_rep(s)
STACKREP * s;
{
 if(s->top > MAXLENGTH - 1)
	    return(TRUE);
 else
	    return(FALSE);
}

REPINFO top_rep(s)
STACKREP * s;
{
 if(empty_rep(s))
      error("* * Erro Fatal: Pilha vazia * *\n");
 return(s->elements[s->top]);
}

REPINFO pop_rep(s)
STACKREP * s;
{

 /* Declaracao de Variaveis Locais */

 REPINFO topo;

 if(empty_rep(s))
     error("* * Erro Fatal: Pilha vazia * *\n");
 topo = s->elements[s->top];
 s->top = s->top+1;
 return(topo);
}

void push_rep(x,s)
REPINFO * x;
STACKREP * s;
 {
  if(s->top == 0)
      error("* * Erro Fatal: Pilha cheia * * \n");
  s->top = s->top - 1;
  s->elements[s->top] = *x;
  return;
 }


/********************************************************************/
/*                                                                  */
/*              FUNCOES ASSOCIADAS A PILHA DE TABLEOUT              */
/*                                                                  */
/********************************************************************/

void mk_nl_tableout(s)
STACKTABLEOUT  * s;
{
 s->top = MAXLENGTH;
 return;
}

int empty_tableout(s)
STACKTABLEOUT * s;
{
 if(s->top > MAXLENGTH - 1)
	    return(TRUE);
 else
	    return(FALSE);
}

TABLEOUT top_tableout(s)
STACKTABLEOUT * s;
{
 if(empty_tableout(s))
      error("* * Erro Fatal: Pilha vazia * *\n");
 return(s->elements[s->top]);
}

TABLEOUT pop_tableout(s)
STACKTABLEOUT * s;
{

 /* Declaracao de Variaveis Locais */

 TABLEOUT topo;

 if(empty_tableout(s))
     error("* * Erro Fatal: Pilha vazia * *\n");
 topo = s->elements[s->top];
 s->top = s->top+1;
 return(topo);
}

void push_tableout(x,s)
TABLEOUT * x;
STACKTABLEOUT * s;
 {
  if(s->top == 0)
      error("* * Erro Fatal: Pilha cheia * * \n");
  s->top = s->top - 1;
  s->elements[s->top] = *x;
  return;
 }

/*
** Special stack function to deal with pointers to heap allocated data
** structures
*/

/*
** push_char - pushes a pointer to a string allocate in the heap.
*/

void push_char(p, s)
char *p;
STACK * s;
{
  char *ptr;

  ptr=strdup(p);
  if(ptr==(char *) NULL)
    error("* * Erro Fatal: Falta de Memoria Disponivel * *\n");
  
  push_int((int) ptr, s);
}

/*
** pop_char - pops a pointer to a string allocate in the heap and frees
**             the space.
*/

void pop_char(s)
STACK * s;
{
  char * ptr;
  ptr = (char *) pop_int(s);
  if(ptr != NULL)
    free(ptr);
}

/*
** top_char - picks up the pointer in the top of the stack.
*/

char * top_char(s)
STACK * s;
{
  return ((char *) top_int(s));
}

/*
** End of newpokelib2.c
*/
