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

#include <alloc.h>

#else  /* SunOS, BSD4.2 e RISC 6000 */

#include <malloc.h>

#endif

#include "util.h"


#include "hparserg.h"
#include "hrotsem.h"
#include "header.h"
#include "hpducam.h"



/********************************************************************/
/* int belongs_pair_stack(PAIRINT *, STACKPAIR *)                   */
/* Autor: Marcos L. Chaim                                           */
/* Data: 26/12/89                                                   */
/* Versao: 1.0                                                      */
/*                                                                  */
/* Funcao: esta funcao retorna verdadeiro caso o par de nos passado */
/*         como parametro esteja presenta na pilha do caminho e     */
/*         falso caso contrario.                                    */
/*                                                                  */
/* Entradas: registro do tipo PAIRINT passado por referencia con-   */
/*          tendo o par de nos e apontador para a pilha de PAIRINT. */
/*                                                                  */
/* Saida: inteiro indicando verdadeiro ou falso.                    */
/*                                                                  */
/********************************************************************/

int belongs_pair_stack(par, pilha)
PAIRINT * par;
STACKPAIR * pilha;
{

 /* Declaracao de Variaveis Locais */

 STACKPAIR stack_aux;

 PAIRINT aux;
 int retorno = FALSE;

 mk_nl_pair(&stack_aux);  /* inicia pilha auxiliar */

 while(!empty_pair(pilha))
   {
    aux = pop_pair(pilha);
    push_pair(&aux,&stack_aux);
    if(aux.abs == par->abs && aux.coor == par->coor)
       retorno = TRUE;
   }

 while(!empty_pair(&stack_aux))
    {
     aux = pop_pair(&stack_aux);
     push_pair(&aux,pilha);
    }

 return(retorno);
}

/*******************************************************************/
/* void reogarniza_grafo(NOGRAFOI *, NOGRAFOI **)                  */
/* Autor: Marcos L. Chaim                                          */
/* Data: 4/1/90                                                    */
/* Versao: 1.0                                                     */
/*                                                                 */
/* Funcao: eliminar  do grafo(i) os nos e arcos possiveis de ser   */
/*         atingidos pelo no' do grafo(i) passado por parametro.   */
/*                                                                 */
/* Entrada: apontador para o no' do grafo(i) a partir do qual deve-*/
/*          se eliminar os nos e arcos possiveis de serem atingidos*/
/*          e apontador para o apontador do inicio do grafo(i).    */
/*                                                                 */
/* Saida: nenhuma.                                                 */
/*                                                                 */
/* Funcoes chamadas: suc_mark() e suc_elimina().                   */
/*                                                                 */
/*******************************************************************/

void reorganiza_grafo(no_inicio, grafo_i)
NOGRAFOI * no_inicio;
NOGRAFOI ** grafo_i;
{

 /* Declaracao de Variaveis Locais */

 SUCGRAFOI * suc_aux, * an_suc_aux;
 NOGRAFOI * no_aux, * an_aux, * ini_grfi;
 LISTPRED * p_aux;

 /* TESTE */
#ifdef DEBUG

 printf("\nreorganizacao  %d\n",no_inicio->infosuc.num_no_G);

#endif

 ini_grfi = *grafo_i;
 no_inicio->num_grafo_i = MARK; /* marca o primeiro elemento a ser desalocado */

 /* marca os seu sucessores */

 for(suc_aux = no_inicio->sucgfi; suc_aux != (SUCGRAFOI *) NULL;
     suc_aux = suc_aux->next)
       suc_mark(suc_aux->no_address); /* marca os nos do grafo(i) */

 suc_elimina(no_inicio->sucgfi);  /* elimina os arcos desse no' */
 no_inicio->sucgfi = (SUCGRAFOI *) NULL;

 /* elimina o no' inicio da lista de sucessores dos seus predecessores */

 for(p_aux = no_inicio->list_pred; p_aux != (LISTPRED *) NULL; p_aux =p_aux->next)
   {
   an_suc_aux = (p_aux->address)->sucgfi;
   for(suc_aux = (p_aux->address)->sucgfi; suc_aux != (SUCGRAFOI *)NULL;
       suc_aux = suc_aux->next)
        {
         if(suc_aux->no_address == no_inicio)
           { /* os dois apontadores sao iguais entao e' inicio da lista */
            if(suc_aux == an_suc_aux)
              {
               (p_aux->address)->sucgfi = suc_aux->next;
               free(suc_aux);
               break;
              }
            else
              { /* os dois apontadores diferentes meio da lista */
               an_suc_aux->next = suc_aux->next;
               free(suc_aux);
               break;
              }
           }
          /* nao encontrei o no' ainda */
          an_suc_aux = suc_aux;
        }
     }

 /* elimina a lista de predecessores do no' */

 pred_elimina(no_inicio->list_pred);

 an_aux = no_aux = ini_grfi;

 /* elimina no' marcados do grafo(i) */

 while(no_aux != (NOGRAFOI *) NULL)
   {
    if(no_aux == ini_grfi && no_aux->num_grafo_i == MARK)
      {
       if (all_pred_marked(no_aux->list_pred))
	 { /* Primeiro no' do grafo(i) esta' marcado e todos os predecessores tambem */

	  an_aux = ini_grfi = no_aux->next;

#ifdef DEBUG

printf(" %d ", no_aux->infosuc.num_no_G);

#endif


	  suc_elimina(no_aux->sucgfi);  /* elimina os arcos desse no' */
	  no_aux->sucgfi = (SUCGRAFOI *) NULL;

	  pred_elimina(no_aux->list_pred); /* elimina lista de predecessores */
	  no_aux->list_pred = (LISTPRED *) NULL;

	  free(no_aux);
	  no_aux = ini_grfi;
	  continue;
	 }
       else
	  no_aux->num_grafo_i = UNMARK;
      }
    if(no_aux != ini_grfi && no_aux->num_grafo_i == MARK)
      {
       if (all_pred_marked(no_aux->list_pred))
	 { /* no' intermediario marcado e todos os seus predecessores tambem */

	  an_aux->next = no_aux->next;

#ifdef DEBUG

 printf(" %d ", no_aux->infosuc.num_no_G);

#endif

	  suc_elimina(no_aux->sucgfi);  /* elimina os arcos desse no' */
	  no_aux->sucgfi = (SUCGRAFOI *) NULL;

	  pred_elimina(no_aux->list_pred); /* elimina lista de predecessores */
	  no_aux->list_pred = (LISTPRED *) NULL;

	  free(no_aux);
	  no_aux = an_aux;
	  continue;
	 }
       else
	  no_aux->num_grafo_i = UNMARK;
      }
    if(no_aux->num_grafo_i != MARK)
     { /* no' nao marcado */
      an_aux = no_aux;
      no_aux = no_aux->next;
     }
   }
 *grafo_i = ini_grfi; /* recupera apontador para o grafo(i) */

#ifdef DEBUG

 printf("\nfim reorganizacao\n");

#endif
}

/*******************************************************************/
/* void suc_mark(NOGRAFOI *)                                       */
/* Autor: Marcos L. Chaim                                          */
/* Data: 4/1/90                                                    */
/* Versao: 1.0                                                     */
/*                                                                 */
/* Funcao: marca os nos do grafo(i) possiveis de ser atingidos pe- */
/*         lo no' do grafo(i) passado por parametro.               */
/*                                                                 */
/* Entrada: apontador para o no' do grafo(i).                      */
/*                                                                 */
/* Saida: nenhuma.                                                 */
/*                                                                 */
/* Funcoes chamadas: suc_mark() e suc_elimina().                   */
/*                                                                 */
/*******************************************************************/

void suc_mark(no_inicio)
NOGRAFOI * no_inicio;
{

 /* Declaracao de Variaveis Locais */

 SUCGRAFOI * suc_aux;

 /* if(no_inicio->infosuc.num_no_G >37) exit(1); */


 if(no_inicio->num_grafo_i == MARK) return;

 no_inicio->num_grafo_i = MARK;

 for(suc_aux = no_inicio->sucgfi; suc_aux != (SUCGRAFOI *) NULL;
     suc_aux = suc_aux->next)
       suc_mark(suc_aux->no_address); /* marca os nos do grafo(i) */

}

/*******************************************************************/
/* void suc_elimina(SUCGRAFOI *)                                   */
/* Autor: Marcos L. Chaim                                          */
/* Data: 4/1/90                                                    */
/* Versao: 1.0                                                     */
/*                                                                 */
/* Funcao: desaloca o espaco ocupado por uma lista de elementos do */
/*         tipo SUCGRAFOI.                                         */
/*                                                                 */
/* Entrada: apontador para o primeiro elemento da lista.           */
/*                                                                 */
/* Saida: nenhuma.                                                 */
/*                                                                 */
/* Funcoes chamadas: suc_elimina().                                */
/*                                                                 */
/*******************************************************************/

void suc_elimina(suc_list)
SUCGRAFOI * suc_list;
{

 if(suc_list == (SUCGRAFOI *)NULL)
   return; /* lista vazia */

 suc_elimina(suc_list->next);
 free(suc_list);
 return;
}

/*******************************************************************/
/* void pred_elimina(LISTPRED *)                                   */
/* Autor: Marcos L. Chaim                                          */
/* Data: 8/1/90                                                    */
/* Versao: 1.0                                                     */
/*                                                                 */
/* Funcao: desaloca o espaco ocupado por uma lista de elementos do */
/*         tipo LISTPRED.                                          */
/*                                                                 */
/* Entrada: apontador para o primeiro elemento da lista.           */
/*                                                                 */
/* Saida: nenhuma.                                                 */
/*                                                                 */
/* Funcoes chamadas: pred_elimina().                               */
/*                                                                 */
/*******************************************************************/

void pred_elimina(pd_list)
LISTPRED * pd_list;
{

 if(pd_list == (LISTPRED *)NULL)
   return; /* lista vazia */

 pred_elimina(pd_list->next);
 free(pd_list);
 return;
}

/*******************************************************************/
/* void det_arcos_pr(int, int, struct grafo *, STACKPAIR *)        */
/* Autor: Marcos L. Chaim                                          */
/* Data: 5/1/90                                                    */
/* Versao: 1.0                                                     */
/*                                                                 */
/* Funcao: determina os arcos pr de um no' repetido.               */
/*                                                                 */
/* Entrada: numero do antecessor do no' repetido, no' repetido,    */
/*          apontador para o grafo de sucessores e apontador para a*/
/*          pilha de arcos pr.                                     */
/*                                                                 */
/* Saida: nenhuma.                                                 */
/*                                                                 */
/* Funcoes chamadas: belongs_pair_stack().                         */
/*                                                                 */
/*******************************************************************/

void det_arcos_pr(ant_nr, nr, g_pred, stack_of_pr)
int ant_nr;
int nr;
struct grafo * g_pred;
STACKPAIR * stack_of_pr;
{

 /* Declaracao de Variaveis Locais */

 struct no * naux;
 PAIRINT aux;

 /* insere o arco passado como pr (experimental) */
 aux.abs = ant_nr;
 aux.coor = nr;
 push_pair(&aux,stack_of_pr);

 for(naux = g_pred[nr].list_suc; naux != (struct no *)NULL; naux=naux->proximo)
   {
    if(naux->num != ant_nr)
      { /* sucessor e' diferente do antecessor, entao coloca na pilha */
       aux.abs = naux->num;
       aux.coor = nr;
       if(!belongs_pair_stack(&aux,stack_of_pr))
         push_pair(&aux,stack_of_pr);
      }
   }
}

/*******************************************************************/
/* NOGRAFOI * cal_nos_contidos(INFODFNO *, NOGRAFOI *)             */
/* Autor: Marcos L. Chaim                                          */
/* Data: 5/1/90                                                    */
/* Versao: 1.0                                                     */
/*                                                                 */
/* Funcao: calcula os nos do grafo(i) que possuem o mesmo no' do   */
/*         grafo G e o conjunto deff esta' contido no conjunto deff*/
/*         do no' i passado como parametro.                        */
/*                                                                 */
/* Entrada: apontador para o registro INFODFNO contendo as informa-*/
/*          coes de fluxo de dados para determinacao dos nos e a-  */
/*          pontador para o grafo(i).                              */
/*                                                                 */
/* Saida: lista contendo os nos.                                   */
/*                                                                 */
/*******************************************************************/

NOGRAFOI * cal_nos_contidos(nim, grafo_i)
INFODFNO * nim;
NOGRAFOI * grafo_i;
{

 /* Declaracao de Variaveis Locais */

 NOGRAFOI * cab, * current, * aux;

 cab = current = (NOGRAFOI *) malloc(sizeof(NOGRAFOI));
 current->next = (NOGRAFOI *) NULL;
 for(aux=grafo_i; aux != (NOGRAFOI *)NULL; aux = aux->next)
   {
    if(nim->num_no_G == (aux->infosuc).num_no_G &&
       aux->repetido == NAO_REPETIDO &&
       is_contained_in_b_vector(&((aux->infosuc).deff),&(nim->deff)))
       {
        current->next = (NOGRAFOI *) malloc(sizeof(NOGRAFOI));
        current = current->next;
        *current = *aux;
        current->next = (NOGRAFOI *)NULL;
       }
    }
 current = cab;
 cab=cab->next;
 free(current);
 return(cab);
 }

/*******************************************************************/
/* NOGRAFOI ret_grafo_i_elem(NOGRAFOI **)                          */
/* Autor: Marcos L. Chaim                                          */
/* Versao: 1.0                                                     */
/* Data: 6/01/90                                                   */
/*                                                                 */
/* FUNCAO: esta funcao retira o primeiro elemento de uma lista     */
/*         de elementos do tipo NOGRAFOI retornado este elemento.  */
/*         Se a lista estiver vazia e' retornado o valor NULL.     */
/*    						                   */
/* ENTRADA: ponteiro para o ponteiro da lista ordenada, este       */
/*	    ponteiro e' do tipo NOGRAFOI **.                       */
/*                                                                 */
/* SAIDA:   retorna o primeiro elemento da lista e retorna um ele- */
/*          mento com todos os campos zeros (com excecao do campo  */
/*          "repetido") se a lista for vazia.                      */
/*******************************************************************/


NOGRAFOI ret_grafo_i_elem(conj)
NOGRAFOI **conj;
{
 /* Declaracao de Variaveis Locais */

NOGRAFOI * noaux;
NOGRAFOI aux;

noaux= *conj;

if(noaux!=(NOGRAFOI *)NULL)
   {
    aux = *noaux;
    *conj=noaux->next;
    free(noaux);
    return(aux);
   }
aux.num_grafo_i = 0;
aux.infosuc.num_no_G = 0;
reset_all(&(aux.infosuc.deff));
aux.list_pred =(LISTPRED *)NULL;
aux.next = (NOGRAFOI *) NULL;
aux.sucgfi = (SUCGRAFOI *) NULL;
aux.repetido = 'z';
return(aux);  /* lista vazia */
}

/*******************************************************************/
/* int all_pred_marked(NOGRAFOI *)                                 */
/* Autor: Marcos L. Chaim                                          */
/* Data: 19/8/90                                                   */
/* Versao: 1.0                                                     */
/*                                                                 */
/* Funcao: verifica se todos os predecessores de um no' marcado    */
/*         do grafo(i) tambem estao marcados.                      */
/*                                                                 */
/* Entrada: apontador para o no' do grafo(i).                      */
/*                                                                 */
/* Saida: TRUE, se todos estiverem marcados e FALSE, caso contra-  */
/*        rio.                                                     */
/*                                                                 */
/*******************************************************************/

int all_pred_marked(pred)
LISTPRED * pred;
{

 /* Declaracao de Variaveis Locais */

 int retorno = TRUE;
 LISTPRED * aux_pred;

 for(aux_pred = pred; aux_pred != (LISTPRED *) NULL; aux_pred = aux_pred->next)
    if((aux_pred->address)->num_grafo_i != MARK)
	{
	 retorno = FALSE;
	 break;
	}

return(retorno);
}

/*******************************************************************/
/* void faca_arco_pfi(INFODFNO *, INFODFNO *, NOGRAFOI *)          */
/* Autor: Marcos L. Chaim                                          */
/* Data: 8/1/90                                                    */
/* Versao: 1.0                                                     */
/*                                                                 */
/* Funcao: tornar primitivo pfi um arco passado como parametro.    */
/*                                                                 */
/* Entradas: apontadores para os registros INFODFNO contendo infor-*/
/*           macoes de fluxo de dados dos nos dos arcos e apontador*/
/*           para o grafo(i).                                      */
/*                                                                 */
/* Saida: nenhuma.                                                 */
/*                                                                 */
/* Funcoes chamadas: is_equal_b_vector().                         */
/*                                                                 */
/*******************************************************************/

void faca_arco_pfi(fonte,sucessor, grafo_i)
INFODFNO * fonte;
INFODFNO * sucessor;
NOGRAFOI * grafo_i;
{

 /* Declaracao de Variaveis Locais */

 NOGRAFOI * head;
 SUCGRAFOI * an_head;

 /* Encontra no fonte no grafo(i) */

 for(head = grafo_i; head != (NOGRAFOI *) NULL; head = head->next)
   if((head->infosuc).num_no_G == fonte->num_no_G &&
/*       head->repetido == NAO_REPETIDO && */
       is_equal_b_vector(&(head->infosuc).deff,&(fonte->deff)))
         break;

 if(head == (NOGRAFOI *) NULL)
     error("PROBLEMA! Nao encontrei o no' no grafo(i)\n");

 /* altera condicao do arco */

 for(an_head = head->sucgfi; an_head != (SUCGRAFOI *) NULL;
     an_head = an_head->next)
      if(((an_head->no_address)->infosuc).num_no_G == sucessor->num_no_G
     /* &&  (an_head->no_address)->repetido == NAO_REPETIDO &&
         is_equal_b_vector(&(((an_head->no_address)->infosuc).deff),
			    &(sucessor->deff)) */)
            {
             an_head->tipo = PFI;
             return;
            }

 error("* * Erro Fatal: Nao consegui alterar o tipo de um no' * *\n");

 }
