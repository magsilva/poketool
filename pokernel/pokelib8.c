/*
// pokelib8.c 	%I%	%Y%	%E%	%U%
*/

/********************************************************************/
/*                                                                  */
/* Procedimento e Funcoes Associadas as Pilhas de Inteiros, de Pa-  */
/* res de Inteiros e de Vetores de Bits                             */
/*                                                                  */
/* Autor: Marcos L. Chaim                                           */
/* Data: 30/10/89                                                   */
/* Versao: 1.0                                                      */
/*                                                                  */
/********************************************************************/


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

BITVECTOR top_bitvector(s)
STACKBITVECTOR * s;
{
 if(empty_bitvector(s))
      error("* * Erro Fatal: Pilha vazia * *\n");
 return(s->elements[s->top]);
}

BITVECTOR pop_bitvector(s)
STACKBITVECTOR * s;
{

 /* Declaracao de Variaveis Locais */

 BITVECTOR topo;

 if(empty_bitvector(s))
     error("* * Erro Fatal: Pilha vazia * *\n");
 topo = s->elements[s->top];
 s->top = s->top+1;
 return(topo);
}

void push_bitvector(x,s)
BITVECTOR * x;
STACKBITVECTOR * s;
 {
  if(s->top == 0)
      error("* * Erro Fatal: Pilha cheia * *\n");
  s->top = s->top - 1;
  s->elements[s->top] = *x;
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
  if(s->top == 0)
      error("* * Erro Fatal: Pilha cheia * *\n");
  s->top = s->top - 1;
  s->elements[s->top] = *x;
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
/* void set_bit(int, BITVECTOR *)                                   */
/* Autor: Marcos L. Chaim                                           */
/* Data: 30/10/89                                                   */
/* Versao: 1.0                                                      */
/*                                                                  */
/* Funcao: "setar" o "bit" relativo a variavel indicada pelo numero */
/*         identificacao.                                           */
/*                                                                  */
/* Entradas: numero de identificacao da variavel e apontador para o */
/*           vetor de bits.                                         */
/*                                                                  */
/* Saida: nenhuma.                                                  */
/*                                                                  */
/********************************************************************/

void set_bit(no_id,array)
int no_id;
BITVECTOR * array;
{

 /* Declaracao de Variaveis Locais */

 int q,r;

 q = no_id/BYTE;
 r = no_id % BYTE;

 if(q >= LENGTHBYTES)
     error("* * Erro Fatal: Numero de Identifcacao excede a faixa permitida * *\n");

 switch(r)
   {
    case 0 : array->vector_bits[q] = array->vector_bits[q] | 0x80;break;
    case 1 : array->vector_bits[q] = array->vector_bits[q] | 0x40;break;
    case 2 : array->vector_bits[q] = array->vector_bits[q] | 0x20;break;
    case 3 : array->vector_bits[q] = array->vector_bits[q] | 0x10;break;
    case 4 : array->vector_bits[q] = array->vector_bits[q] | 0x08;break;
    case 5 : array->vector_bits[q] = array->vector_bits[q] | 0x04;break;
    case 6 : array->vector_bits[q] = array->vector_bits[q] | 0x02;break;
    case 7 : array->vector_bits[q] = array->vector_bits[q] | 0x01;break;
   }
 return;
}

/********************************************************************/
/* void set_all(BITVECTOR *)                                        */
/* Autor: Marcos L. Chaim                                           */
/* Data: 30/10/89                                                   */
/* Versao: 1.0                                                      */
/*                                                                  */
/* Funcao: "setar" todos os "bits" de um vetor de bits.             */
/*                                                                  */
/* Entradas: apontador para o vetor de bits.                        */
/*                                                                  */
/* Saida: nenhuma.                                                  */
/*                                                                  */
/********************************************************************/

void set_all(array)
BITVECTOR * array;
{
 int i;

 for(i=0; i < LENGTHBYTES; ++i)
       array->vector_bits[i] = array->vector_bits[i] | 0xFF;
 return;
}

/********************************************************************/
/* void reset_bit(int, BITVECTOR *)                                 */
/* Autor: Marcos L. Chaim                                           */
/* Data: 30/10/89                                                   */
/* Versao: 1.0                                                      */
/*                                                                  */
/* Funcao: "resetar" o "bit" relativo a variavel indicada pelo      */
/*         numero identificacao.                                    */
/*                                                                  */
/* Entradas: numero de identificacao da variavel e apontador para o */
/*           vetor de bits.                                         */
/*                                                                  */
/* Saida: nenhuma.                                                  */
/*                                                                  */
/********************************************************************/

void reset_bit(no_id,array)
int no_id;
BITVECTOR * array;
{

 /* Declaracao de Variaveis Locais */

 int q,r;

 q = no_id/BYTE;
 r = no_id % BYTE;

 if(q >= LENGTHBYTES)
     error("* * Erro Fatal: Numero de Identifcacao excede a faixa permitida * *\n");

 switch(r)
   {
    case 0 : array->vector_bits[q] = array->vector_bits[q] & 0x7F;break;
    case 1 : array->vector_bits[q] = array->vector_bits[q] & 0xBF;break;
    case 2 : array->vector_bits[q] = array->vector_bits[q] & 0xDF;break;
    case 3 : array->vector_bits[q] = array->vector_bits[q] & 0xEF;break;
    case 4 : array->vector_bits[q] = array->vector_bits[q] & 0xF7;break;
    case 5 : array->vector_bits[q] = array->vector_bits[q] & 0xFB;break;
    case 6 : array->vector_bits[q] = array->vector_bits[q] & 0xFD;break;
    case 7 : array->vector_bits[q] = array->vector_bits[q] & 0xFE;break;
   }
 return;
}

/********************************************************************/
/* void reset_all(BITVECTOR *)                                      */
/* Autor: Marcos L. Chaim                                           */
/* Data: 30/10/89                                                   */
/* Versao: 1.0                                                      */
/*                                                                  */
/* Funcao: "resetar" todos os "bits" de um vetor de bits.           */
/*                                                                  */
/* Entradas: apontador para o vetor de bits.                        */
/*                                                                  */
/* Saida: nenhuma.                                                  */
/*                                                                  */
/********************************************************************/

void reset_all(array)
BITVECTOR * array;
{
 int i;

 for(i=0; i < LENGTHBYTES; ++i)
       array->vector_bits[i] = array->vector_bits[i] & 0x00;
 return;
}

/********************************************************************/
/* int test_bit(int, BITVECTOR *)                                   */
/* Autor: Marcos L. Chaim                                           */
/* Data: 1/11/89                                                    */
/* Versao: 1.0                                                      */
/*                                                                  */
/* Funcao: testa se o "bit" relativo a variavel indicada            */
/*         pela posicao no vetor de "bits" e' verdadeiro ou nao.    */
/*                                                                  */
/* Entradas: numero de identificacao da variavel e apontador para o */
/*           vetor de bits.                                         */
/*                                                                  */
/* Saida: retorna verdadeiro caso o "bit" esteja "setado" e falso   */
/*        caso contrario.                                           */
/*                                                                  */
/********************************************************************/

int test_bit(position,vb)
int position;
BITVECTOR * vb;
{

 /* Declaracao de Variaveis Locais */

 int q,r;
 unsigned char resul;
 q = position/BYTE;
 r = position % BYTE;

 if(q >= LENGTHBYTES)
     error("* * Erro Fatal: Numero de Identifcacao excede a faixa permitida * *\n");

 switch(r)
   {
    case 0 : resul = vb->vector_bits[q] & 0x80;break;
    case 1 : resul = vb->vector_bits[q] & 0x40;break;
    case 2 : resul = vb->vector_bits[q] & 0x20;break;
    case 3 : resul = vb->vector_bits[q] & 0x10;break;
    case 4 : resul = vb->vector_bits[q] & 0x08;break;
    case 5 : resul = vb->vector_bits[q] & 0x04;break;
    case 6 : resul = vb->vector_bits[q] & 0x02;break;
    case 7 : resul = vb->vector_bits[q] & 0x01;break;
   }
  if(resul)
    return(TRUE);
  else
    return(FALSE);
}


/********************************************************************/
/* int vazio_bit(BITVECTOR *)                                       */
/* Autor: Marcos L. Chaim                                           */
/* Data: 13/12/89                                                   */
/* Versao: 1.0                                                      */
/*                                                                  */
/* Funcao: testa se o vetor de "bits" possui algum "bit" verdadeiro.*/
/*                                                                  */
/* Entradas: apontador para o vetor de bits.                        */
/*                                                                  */
/* Saida: retorna verdadeiro caso exista algum "bit" "setado" e     */
/*        falso caso contrario.                                     */
/*                                                                  */
/********************************************************************/

int vazio_bit(vb)
BITVECTOR * vb;
{

 /* Declaracao de Variaveis Locais */

 int i;
 unsigned char resul;

 for(i=0; i>=0 && i < LENGTHBYTES; ++i)
   {
     resul = vb->vector_bits[i] & 0xff;
     if(resul)
        return(FALSE);
   }

 return(TRUE);
}


/********************************************************************/
/* BITVECTOR neg_bitvector(BITVECTOR *)                             */
/* Autor: Marcos L. Chaim                                           */
/* Data: 13/12/89                                                   */
/* Versao: 1.0                                                      */
/*                                                                  */
/* Funcao: faz a negacao do vetor de "bits".                        */
/*                                                                  */
/* Entradas: apontador para o vetor de "bits".                      */
/*                                                                  */
/* Saida: vetor de "bits" negado.                                   */
/*                                                                  */
/********************************************************************/

BITVECTOR neg_bitvector(vb)
BITVECTOR * vb;
{

 /* Declaracao de Variaveis Locais */

 int i;
 BITVECTOR bitaux;

 for(i=0; i>=0 && i < LENGTHBYTES; ++i)
        bitaux.vector_bits[i] = ~vb->vector_bits[i];

 return(bitaux);
}


/********************************************************************/
/* BITVECTOR uniao_bitvector(BITVECTOR *, BITVECTOR *)              */
/* Autor: Marcos L. Chaim                                           */
/* Data: 13/12/89                                                   */
/* Versao: 1.0                                                      */
/*                                                                  */
/* Funcao: faz a uniao de dois vetores de "bits".                   */
/*                                                                  */
/* Entradas: apontadores para os vetores de "bits".                 */
/*                                                                  */
/* Saida: vetor de "bits" resultada da uniao.                       */
/*                                                                  */
/********************************************************************/

BITVECTOR uniao_bitvector(vb1,vb2)
BITVECTOR * vb1;
BITVECTOR * vb2;
{

 /* Declaracao de Variaveis Locais */

 int i;
 BITVECTOR bitaux;

 for(i=0; i>=0 && i < LENGTHBYTES; ++i)
     bitaux.vector_bits[i] = vb1->vector_bits[i] | vb2->vector_bits[i];


 return(bitaux);
}

/********************************************************************/
/* BITVECTOR inter_bitvector(BITVECTOR *, BITVECTOR *)              */
/* Autor: Marcos L. Chaim                                           */
/* Data: 13/12/89                                                   */
/* Versao: 1.0                                                      */
/*                                                                  */
/* Funcao: faz a intersecao de dois vetores de "bits".              */
/*                                                                  */
/* Entradas: apontadores para os vetores de "bits".                 */
/*                                                                  */
/* Saida: vetor de "bits" resultada da uniao.                       */
/*                                                                  */
/********************************************************************/

BITVECTOR inter_bitvector(vb1,vb2)
BITVECTOR * vb1;
BITVECTOR * vb2;
{

 /* Declaracao de Variaveis Locais */

 int i;
 BITVECTOR bitaux;

 for(i=0; i>=0 && i < LENGTHBYTES; ++i)
     bitaux.vector_bits[i] = vb1->vector_bits[i] & vb2->vector_bits[i];


 return(bitaux);
}


/********************************************************************/
/* BITVECTOR sub_bitvector(BITVECTOR *, BITVECTOR *)                */
/* Autor: Marcos L. Chaim                                           */
/* Data: 13/12/89                                                   */
/* Versao: 1.0                                                      */
/*                                                                  */
/* Funcao: retira do primeiro vetor de "bits" os elementos do       */
/*         segundo vetor de "bits".                                 */
/*                                                                  */
/* Entradas: apontadores para os vetores de "bits".                 */
/*                                                                  */
/* Saida: vetor de "bits" resultado da subtracao.                   */
/*                                                                  */
/********************************************************************/

BITVECTOR sub_bitvector(vb1,vb2)
BITVECTOR * vb1;
BITVECTOR * vb2;
{

 /* Declaracao de Variaveis Locais */

 int i;
 BITVECTOR bitaux;

 for(i=0; i>=0 && i < LENGTHBYTES; ++i)
     bitaux.vector_bits[i] = vb1->vector_bits[i] & (~vb2->vector_bits[i]);


 return(bitaux);
}

/********************************************************************/
/* int is_equal_bitvector(BITVECTOR *, BITVECTOR *)                 */
/* Autor: Marcos L. Chaim                                           */
/* Data: 27/12/89                                                   */
/* Versao: 1.0                                                      */
/*                                                                  */
/* Funcao: verifica se dois vetores de bits sao iguais.             */
/*                                                                  */
/* Entradas: apontadores para os vetores de "bits".                 */
/*                                                                  */
/* Saida: retorna o valor TRUE se forem iguais e FALSE caso contra- */
/*        rio.                                                      */
/*                                                                  */
/********************************************************************/

int is_equal_bitvector(vb1,vb2)
BITVECTOR * vb1;
BITVECTOR * vb2;
{

 /* Declaracao de Variaveis Locais */
 int retorno = TRUE, i;

 for(i=0; i>=0 && i < LENGTHBYTES; ++i)
   {
     if(vb1->vector_bits[i] != vb2->vector_bits[i])
       {
        retorno = FALSE;
        break;
       }
   }
 return(retorno);
}

/********************************************************************/
/* int esta_contido_bitvector(BITVECTOR *, BITVECTOR *)             */
/* Autor: Marcos L. Chaim                                           */
/* Data: 5/1/90                                                     */
/* Versao: 1.0                                                      */
/*                                                                  */
/* Funcao: verifica se o primeiro vetor de "bits" esta' contido no  */
/*         no segundo.                                              */
/*                                                                  */
/* Entradas: apontadores para os vetores de "bits".                 */
/*                                                                  */
/* Saida: retorna o valor TRUE se forem iguais e FALSE caso contra- */
/*        rio.                                                      */
/*                                                                  */
/********************************************************************/

int esta_contido_bitvector(conj1, conj2)
BITVECTOR * conj1;
BITVECTOR * conj2;
{

 /* Declaracao de Variaveis Locais */

 BITVECTOR aux;
 aux = inter_bitvector(conj1,conj2);
 return(is_equal_bitvector(conj1,&aux));

}






