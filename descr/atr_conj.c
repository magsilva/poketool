/*******************************************************************/
/* funcao void lib_conj(struct no *conj)                           */
/* Autor: Marcos L. Chaim                                          */
/* Versao: 1.0  						   */
/* Data: 13/06/89                                                  */
/*                                                                 */
/* TAREFA: esta funcao libera o espaco ocupado por uma lista liga- */
/*         da ordenada de inteiros positivos.                      */
/*                                                                 */
/* ENTRADA:apontador para a lista a ser desalocada.                */
/*                                                                 */
/* SAIDA:  nenhuma                                                 */
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

void lib_conj(conj)
struct no *conj;
{
 if(conj==NULL)
    return;       /* a lista esta vazia */
 lib_conj(conj->proximo);
 free(conj);
 return;
}

/*******************************************************************/
/* funcao void atr_conj(struct no **, struct no **)                */
/* Autor: Marcos L. Chaim                                          */
/* Versao: 1.0                                                     */
/* Data: 13/06/89                                                  */
/*                                                                 */
/* TAREFA: esta funcao atribui os elementos de um conjunto para    */
/*         outro conjunto, de tal maneira que o segundo conjunto   */
/*         passa a possuir os mesmos elementos do primeiro. Os     */
/*         conjuntos sao representados por listas ordenadas de in- */
/*         inteiros positivos.                                     */
/*                                                                 */
/* ENTRADA:apontadores para os apontadores dos conjuntos.          */
/*                                                                 */
/* SAIDA:  nenhuma                                                 */
/*                                                                 */
/* FUNCOES CHAMADAS: void lib_conj()                               */
/*******************************************************************/

void atr_conj(conj_a, conj_b)
struct no **conj_a;
struct no **conj_b;
{

/* declaracao de variaveis locais */

struct no *a, *b;

/* iniciando */

a= *conj_a;
b= *conj_b;

if(b==NULL)
    {
     lib_conj(a);
     *conj_a=NULL;
     return;
    }
if(a==NULL)
    {
     a=(struct no *) malloc (sizeof(struct no));
     if(a==NULL)
	 {
	   fprintf(stderr,"PROBLEMA! Nao consegui alocar espaco para o no'\n");
	   exit(1);
	 }
      a->proximo=NULL;
      *conj_a=a;
     }
a->num=b->num;
while(b->proximo!=NULL)
     {
      if(a->proximo!=NULL)
	  (a->proximo)->num=(b->proximo)->num;
      else
	 {
	  a->proximo=(struct no *) malloc (sizeof(struct no));
	  if((a->proximo)==NULL)
	      {
	       fprintf(stderr,"PROBLEMA! Nao consegui alocar espaco para o no'\n");
	       exit(1);
	      }
	  (a->proximo)->num=(b->proximo)->num;
	  (a->proximo)->proximo=NULL;
	 }
      a=a->proximo;
      b=b->proximo;
     }
lib_conj(a->proximo);
a->proximo=NULL;
}

/*******************************************************************/
/* int e_primitivo(int, int, struct grafo *)                       */
/* Autor: Marcos L. Chaim                                          */
/* Data: 28/12/89                                                  */
/* Versao: 1.0                                                     */
/*                                                                 */
/* Funcao: determina se o arco em questao e' primitivo ou nao.     */
/*                                                                 */
/* Entradas: numero do no' fonte, numero do no' alvo e apontador   */
/*           para o grafo.                                         */
/*                                                                 */
/* Saida: retorna o valor TRUE se for um arco primitivo e FALSE    */
/*        caso contrario.                                          */
/*                                                                 */
/*******************************************************************/

int e_primitivo(fonte, alvo, g_suc)
int fonte;
int alvo;
struct grafo * g_suc;
{

 /* Declaracao de Variaveis Locais */

 struct no * aux;
 int retorno = FALSE;

 for(aux = g_suc[fonte].list_suc; aux!=(struct no *) NULL;aux =aux->proximo)
    if(aux->num == alvo)
      {
       if(aux->marca == PRIMITIVO)
          retorno = TRUE;
       break;
      }
 return(retorno);
}












