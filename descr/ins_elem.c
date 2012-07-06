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

/*******************************************************************/
/* funcao int ins_elem(struct no **conj, int elem)                 */
/* Autor: Marcos L. Chaim                                          */
/* Versao: 1.0                                                     */
/* Data: 7/06/89                                                   */
/*                                                                 */
/* FUNCAO: esta funcao insere um elemento em uma lista ordenada de */
/*         inteiros. Esta lista ordenada representa um conjunto de */
/*         numeros inteiros, logo o elemento so' e' inserido se    */
/*         estiver presente  no conjunto.                          */
/*                                                                 */
/* ENTRADA: ponteiro para o ponteiro da lista ordenada, este       */
/*	    ponteiro e' do tipo struct no **, e o elemento a ser   */
/*          inserido.                                              */
/*                                                                 */
/* SAIDA:   retorna o inteiro 10 se o elemento foi inserido na     */
/*          lista e -10 no caso contrario.                         */
/*******************************************************************/

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
#include "header.h"
#include "hparserg.h"

int ins_elem(conj, elem)
struct no **conj;
int elem;

{

/* declaracao de variaveis locais */

struct no *anaux, *noaux, *oldaux;

noaux= *conj;
oldaux= *conj;

if(noaux==(struct no *)NULL)
   {       /* caso a lista esteja vazia */
    noaux=(struct no *) malloc(sizeof(struct no));
    if(noaux==(struct no *)NULL)
       error("* * Erro Fatal: Nao consegui alocar memoria para este no' * *\n");
    noaux->num=elem;
    noaux->proximo = NULL;
    *conj=noaux;
    return(10);
   }

do /* procurando onde inserir novo elemento */
   {
    if(noaux->num==elem)
	         return(-10);      /* o elemento ja' esta na lista */
     else
       {
	if(noaux->num > elem) break;
	oldaux=noaux;
	noaux=noaux->proximo;
       }
   }
while(noaux!=(struct no *)NULL);

/* caso a lista ja' contenha algum elemento */

anaux = (struct no *) malloc(sizeof(struct no));
if(anaux==(struct no *)NULL)
   error("* * Erro Fatal: Nao consegui alocar memoria para este no' * *\n");
anaux->num=elem;
anaux->proximo=noaux;
if(noaux== *conj)
    *conj=anaux;   /* caso o elemento seja o menor da lista */
else
    oldaux->proximo=anaux;
return(10);
}

/*   Programa Teste
void main()

{
struct grafo *g1;
struct no *aux;

char * arq1;

arq1="grafo1.tes";

inigrf(arq1, fg1);
system("cls");
printf("\n");

ins_elem(f(g1[12].list_suc),2);
rins_elem(f(g1[12].list_suc),2);
ins_elem(f(g1[12].list_suc),30);
printf("elementos da lista de sucessores do no' 12: \n");
for(aux=g1[12].list_suc;aux!=(struct no *)NULL;aux=aux->proximo)
   {
     printf("%d ",aux->num);
   }
printf("\n");
printf(" no de ocorrencia de 2 = %d\n",no_ocorren(g1[12].list_suc,2));
rdel_elem(f(g1[12].list_suc),2);
del_elem(f(g1[12].list_suc),30);
del_elem(f(g1[12].list_suc),19);
printf("elementos da lista de sucessores do no' 12: \n");
for(aux=g1[12].list_suc;aux!=(struct no *)NULL;aux=aux->proximo)
   {
     printf("%d ",aux->num);
   }
printf("\n");

printf("elementos da lista de sucessores do no' 13: \n");


for(aux=g1[13].list_suc;aux!=(struct no *)NULL;aux=aux->proximo)
   {
     printf("%d ",aux->num);
   }

printf("\nComparando as duas listas anteriores\n");

if(e_igual(g1[24].list_suc,g1[13].list_suc))
   printf("As listas sao iguais!!!\n");
else
   printf("As listas sao diferentes!!!\n");

intersecao(fg1[24].list_suc,fg1[24].list_suc,fg1[4].list_suc);

printf("\n");
printf("elementos da lista de sucessores do no' 4: \n");
while(g1[4].list_suc !=(struct no *)NULL)
   {
    printf("Retirei o numero %d\n", ret_elem(fg1[4].list_suc));
   }
printf(" %d", ret_elem(fg1[4].list_suc));

}                  */

/*******************************************************************/
/* funcao int ret_elem(struct no **conj)                           */
/* Autor: Marcos L. Chaim                                          */
/* Versao: 1.0                                                     */
/* Data: 13/06/89                                                  */
/*                                                                 */
/* FUNCAO: esta funcao retira o primeiro elemento de uma lista     */
/*         ordenada de inteiros positivos retornado este elemento. */
/*         Se a lista estiver vazia e' retornado o valor -10.      */
/*    						                   */
/* ENTRADA: ponteiro para o ponteiro da lista ordenada, este       */
/*	    ponteiro e' do tipo struct no **.                      */
/*                                                                 */
/* SAIDA:   retorna o primeiro elemento da lista e o valor -10;    */
/*          se a lista for vazia.                                  */
/*******************************************************************/


int ret_elem(conj)
struct no **conj;
{
 /* declaracao de variaveis locais */

struct no *noaux;
int i;

noaux= *conj;

if(noaux!=(struct no *)NULL)
   {
    i=noaux->num;
    *conj=noaux->proximo;
    free(noaux);
    return(i);
   }
return(-10);  /* lista vazia */
}


/*******************************************************************/
/* int del_elem(struct no **, int z)                               */
/* Autor: Marcos L. Chaim                                          */
/* Versao: 1.0  					           */
/* Data: 26/06/89                                                  */
/*                                                                 */
/* FUNCAO: elimina um elemento de uma lista caso exista.           */
/*                                                                 */
/* ENTRADA:o primeiro argumento e' o elemento a ser eliminado e o  */
/*         segundo argumento o apontador para o apontador da lista */
/*	   onde o elemento pode estar presente.                    */
/*                                                                 */
/*******************************************************************/

void del_elem(list_prof,z)
struct no **list_prof;
int z;
{
 /* declaracao de variaveis locais */
 struct no * lista, * noaux;

 lista= *list_prof;
 if(lista!=(struct no *)NULL && lista->num==z)  /* caso seja o primeiro elemento da lista */
     {
      *list_prof=lista->proximo;
      free(lista);
      return;
     }

				   /* caso nao seja o primeiro da lista */
 for(noaux= *list_prof;noaux!=(struct no *)NULL && noaux->num <=z;noaux=noaux->proximo)
    {
      if(noaux->num==z)
	 {
	  lista->proximo=noaux->proximo;
	  free(noaux);
	  noaux=lista->proximo;
	  break;
	 }
      else lista=noaux;
    }
}

/*******************************************************************/
/* int tem_marca(int, int, struct grafo *)                         */
/* Autor: Marcos L. Chaim                                          */
/* Versao: 1.0                                                     */
/* Data: 26/06/88                                                  */
/*                                                                 */
/* FUNCAO: verificar se o arco representado pelos nos indicados    */
/*         pelos parametros de entrada possuem uma marca de her-   */
/*         no grafo de fluxo de controle representado pelo tercei- */
/*         ro parametro.                                           */
/*                                                                 */
/* ENTRADA:nos do arco a ser verificado, sendo que o primeiro pa-  */
/*         rametro e' o no' fonte e o segundo parametro e' no'     */
/*         alvo, e o apontador para o grafo de fluxo de controle.  */
/*                                                                 */
/* SAIDA:  inteiro indicando se e' membro ou nao.                  */
/*                                                                 */
/*******************************************************************/

int tem_marca(x,y,g)
int x;
int y;
struct grafo *g;
{

 /* declaracao de variaveis locais */

 int retorno,i= -10;
 struct no *noaux;

 retorno=FALSE;

 for(noaux=g[x].list_suc;noaux!=(struct no *)NULL && noaux->num <=y;noaux=noaux->proximo)
   {
      if(noaux->num==y)
	{
	  i=0;
	  if(noaux->marca=='t' || noaux->marca=='h')
		 retorno=TRUE;
	}
    }

 if(i==-10)
      error("* * Erro Fatal: No' nao e' sucessor * *");
 return(retorno);
}

/*******************************************************************/
/* int membro(int, struct no *)                                    */
/* Autor: Marcos L. Chaim                                          */
/* Versao: 1.0              					   */
/* Data: 26/06/89                                                  */
/*                                                                 */
/* FUNCAO: verificar se um dado inteiro positivo e' componente de  */
/*         uma dada lista.                                         */
/*                                                                 */
/* ENTRADA:o primeiro parametro e' o inteiro positivo a ser veri-  */
/*	   ficado como componente da lista representada pelo se    */
/*	   gundo parametro.                                        */
/*                                                                 */
/* SAIDA:  inteiro indicando se o inteiro e' membro ou nao.        */
/*                                                                 */
/*******************************************************************/

int membro(z,lista)
int z;
struct no *lista;
{
 /* declaracao de variaveis locais */
 int member_ship;
 struct no *noaux;

 member_ship=FALSE;
 for(noaux=lista;noaux!=(struct no *)NULL && noaux->num <=z;noaux=noaux->proximo)
     if(noaux->num==z) member_ship=TRUE;
 return(member_ship);
}

/*******************************************************************/
/* funcao int rins_elem(struct no **conj, int elem)                */
/* Autor: Marcos L. Chaim                                          */
/* Versao: 1.0                                                     */
/* Data: 26/06/89                                                  */
/*                                                                 */
/* FUNCAO: esta funcao insere um elemento em uma lista ordenada de */
/*         inteiros,  podendo haver repeticao de elementos.        */
/*                                                                 */
/* ENTRADA: ponteiro para o ponteiro da lista ordenada, este       */
/*	    ponteiro e' do tipo struct no **, e o elemento a ser   */
/*          inserido.                                              */
/*                                                                 */
/* SAIDA:   retorna o inteiro 10 se o elemento foi inserido na     */
/*          lista.                                                 */
/*******************************************************************/


int rins_elem(conj, elem)
struct no **conj;
int elem;

{

/* declaracao de variaveis locais */

struct no *anaux, *noaux, *oldaux;

noaux= *conj;
oldaux= *conj;

if(noaux==(struct no *)NULL)
   {       /* caso a lista esteja vazia */
    noaux=(struct no *) malloc(sizeof(struct no));
    if(noaux==(struct no *)NULL)
       error("* * Erro Fatal: Nao consegui alocar memoria para este no' * *\n");
    noaux->num=elem;
    noaux->proximo = NULL;
    *conj=noaux;
    return(10);
   }

do /* procurando onde inserir novo elemento */
   {
	if(noaux->num > elem) break;
	oldaux=noaux;
	noaux=noaux->proximo;
   }
while(noaux!=(struct no *)NULL);

/* caso a lista ja' contenha algum elemento */

anaux = (struct no *) malloc(sizeof(struct no));
if(anaux==(struct no *)NULL)
    error("* * Erro Fatal: Nao consegui alocar memoria para este no' * *\n");
anaux->num=elem;
anaux->proximo=noaux;
if(noaux== *conj)
    *conj=anaux;   /* caso o elemento seja o menor da lista */
else
    oldaux->proximo=anaux;
return(10);
}

/*******************************************************************/
/* int rdel_elem(struct no **, int z)                               */
/* Autor: Marcos L. Chaim                                          */
/* Versao: 1.0  					           */
/* Data: 26/06/89                                                  */
/*                                                                 */
/* FUNCAO: elimina uma de um elemento de uma lista com repeticao,  */
/*	   caso exista.                                            */
/*                                                                 */
/* ENTRADA:o primeiro argumento e' o elemento a ser eliminado e o  */
/*         segundo argumento o apontador para o apontador da lista */
/*	   onde o elemento pode estar presente.                    */
/*                                                                 */
/*******************************************************************/

void rdel_elem(list_prof,z)
struct no **list_prof;
int z;
{
 /* declaracao de variaveis locais */
 struct no * lista, * noaux;

 lista= *list_prof;

 if(lista!=(struct no *)NULL && lista->num==z)  /* caso seja o primeiro elemento da lista */
     {
      *list_prof=lista->proximo;
      free(lista);
      return;
     }

				   /* caso nao seja o primeiro da lista */
 for(noaux= *list_prof;noaux!=(struct no *)NULL && noaux->num <=z;noaux=noaux->proximo)
     {
      if(noaux->num==z)
	 {
	  lista->proximo=noaux->proximo;
	  free(noaux);
	  noaux=lista->proximo;
	  break;
	 }
      else lista=noaux;
     }
}

/*******************************************************************/
/* int no_ocorren(struct no *, int)                                */
/* Autor: Marcos L. Chaim                                          */
/* Versao: 1.0                                                     */
/* Data: 26/06/89                                                  */
/*                                                                 */
/* FUNCAO: calcula o numero de ocorrencias de um elemento em uma   */
/*         lista com repeticao.                                    */
/*                                                                 */
/* ENTRADA:apontador para a lista e o elemento.                    */
/*                                                                 */
/* SAIDA:  numero de ocorrencias.                                  */
/*                                                                 */
/*******************************************************************/

int no_ocorren(lista,elem)
struct no *lista;
int elem;
{
 /* declaracao de variaveis locais */
 struct no *noaux=(struct no *)NULL;
 int n_ocorren=0;

 for(noaux=lista;noaux!=(struct no *)NULL;noaux=noaux->proximo)
     if(noaux->num==elem) ++n_ocorren;

 return(n_ocorren);
}

