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

#include "headerli.h"
#include "hrotsem.h"
#include "header.h"
#include "hparserg.h"
#include "hpducam.h"


/*******************************************************************/
/* void grfi_inicializacao(NOGRAFOI **, INFODFNO *)                */
/* Autor: Marcos L. Chaim                                          */
/* Data: 22/12/89                                                  */
/* Versao: 1.0                                                     */
/*                                                                 */
/* Funcao: esta operacao aloca espaco para o no' i e suas informa- */
/*         coes (numero do no' no grafo G e o conjunto deff, no    */
/*         igual a 1; deff, no caso igual a defg_i uniao def_ref), */
/*         inicia os apontadores com o valor NULL e faz apt_grf a- */
/*         pontar para o espaco alocado.                           */
/*                                                                 */
/* Entradas: apontador para o grafo(i) (aptr_grf) passado por re-  */
/*           ferencia e um registro contendo as informacoes sobre  */
/*           o no' i tambem passado por referencia.                */
/*                                                                 */
/* Saida: nenhuma.                                                 */
/*                                                                 */
/*******************************************************************/

void grfi_inicializacao(aptr_grf, info_do_no)
NOGRAFOI ** aptr_grf;
INFODFNO * info_do_no;
{

 /* Declaracao de Variaveis Locais */

 NOGRAFOI * head;

 head = (NOGRAFOI *) malloc(sizeof(NOGRAFOI));
 if(head == (NOGRAFOI *) NULL)
   error("* * Erro Fatal: Nao consegui alocar espaco para um no' no grafo(i) * *\n");

 /* inicia o conteudo do primeiro no' do grafo(i) */

 head->num_grafo_i = 1;
 
 /* copia estrutura INFODFNO */
 /* head->infosuc = *info_do_no; */
 
 b_vector_cons(&((head->infosuc).deff),(*info_do_no).deff.nbytes,(*info_do_no).deff.type_in);
 b_vector_cpy(&((head->infosuc).deff),&((*info_do_no).deff));
 (head->infosuc).num_no_G=info_do_no->num_no_G;
 

 head->repetido = NAO_REPETIDO;
 head->list_pred = (LISTPRED *) NULL;
 head->next = (NOGRAFOI *) NULL;
 head->sucgfi = (SUCGRAFOI *) NULL;
 *aptr_grf = head;
}

/*******************************************************************/
/* void insere_grafo_i(INFODFNO *, INFODFNO *, NOGRAFOI *,         */
/*                     struct grafo *, char)                       */
/* Autor: Marcos L. Chaim                                          */
/* Versao: 1.0                                                     */
/* Data: 22/12/89                                                  */
/*                                                                 */
/* Funcao: esta operacao insere um novo no' no grafo(i) e coloca   */
/*         este novo no' inserido como sucessor do  no' fonte.     */
/*                                                                 */
/* Entradas: dois registros do tipo INFODFNO passados por referen- */
/*           cia, o apontador para o grafo(i),  o apontador para o */
/*           grafo G e tipo do no' a ser inserido.                 */
/*                                                                 */
/* Saida: endereco do no' inserido.                                */
/*                                                                 */
/*******************************************************************/

NOGRAFOI * insere_grafo_i(fonte, sucessor, grafo_i,g_suc,tipo_no)
INFODFNO * fonte;
INFODFNO * sucessor;
NOGRAFOI * grafo_i;
struct grafo * g_suc;
char tipo_no;
{

 /* Declaracao de Variaveis Locais */

 NOGRAFOI * head, * aux;
 static int num_grafo_i = 2;
 LISTPRED * current1;
 SUCGRAFOI * current2;


 if(grafo_i == (NOGRAFOI *) NULL)
    error("* * Erro Fatal: Estou tentando inserir em um grafo(i) nao iniciado * *\n");


 /* Procurando o fim da lista de nos do grafo(i) */

 for(head = grafo_i; head->next != (NOGRAFOI *) NULL; head=head->next);

 /* Insere novo no' na lista de nos do grafo(i) */

 if(head->next != (NOGRAFOI *) NULL)
    error("* * Erro Fatal: Nao encontrei o fim da lista de nos do grafo(i) * *\n");

 head->next = (NOGRAFOI *) malloc(sizeof(NOGRAFOI));
 if(head->next == (NOGRAFOI *) NULL)
   error("* * Erro Fatal: Nao consegui alocar espaco para um no' no grafo(i) * *\n");

 (head->next)->num_grafo_i = num_grafo_i++;

 
 /* (head->next)->infosuc = *sucessor; */

 /* copia campos de INFODFNO */
  
 b_vector_cons(&((head->next->infosuc).deff),(sucessor->deff).nbytes,(sucessor->deff).type_in);
 b_vector_cpy(&((head->next->infosuc).deff),&(sucessor->deff));  

 (head->next->infosuc).num_no_G=sucessor->num_no_G;


 (head->next)->repetido = tipo_no;
 (head->next)->next = (NOGRAFOI *) NULL;
 (head->next)->sucgfi = (SUCGRAFOI *) NULL;
 (head->next)->list_pred = (LISTPRED *) NULL;

 /* Salva o endereco do no' inserido na lista de nos do grafo(i) */

 aux = head->next;

 /* Procurando onde inserir o sucessor */

 for(head = grafo_i; head != (NOGRAFOI *) NULL;head = head->next)
     if(is_equal_b_vector(&fonte->deff,&((head->infosuc).deff)) &&
        fonte->num_no_G == (head->infosuc).num_no_G &&
        head->repetido == NAO_REPETIDO) break;


 if(head == (NOGRAFOI *) NULL)
    error("* * Erro Fatal: Nao encontrei o no' fonte no grafo(i) * *\n");

 /* insere endereco do predecessor na lista de predecessores */

 current1 = aux->list_pred;

 aux->list_pred = (LISTPRED *) malloc(sizeof(LISTPRED));
 if(aux->list_pred == (LISTPRED *) NULL)
   error("* * Erro Fatal: Nao consegui alocar espaco para a lista de predecessores * *\n");
 (aux->list_pred)->address =  head;
 (aux->list_pred)->next = current1;

 /* insere o novo no' do grafo(i) na lista de sucessores de fonte */

 current2 = head->sucgfi;

 head->sucgfi = (SUCGRAFOI *) malloc(sizeof(SUCGRAFOI));
 if(head->sucgfi == (SUCGRAFOI *) NULL)
   error("* * Erro Fatal: Nao consegui alocar espaco para um no' no grafo(i) * *\n");

 (head->sucgfi)->num_grafo_i = num_grafo_i-1;
 if(e_primitivo(fonte->num_no_G,sucessor->num_no_G, g_suc))
       (head->sucgfi)->tipo = PRIMITIVO;
 else
       (head->sucgfi)->tipo = HERDEIRO;
 (head->sucgfi)->no_address = aux;
 (head->sucgfi)->next = current2;

#ifdef DEBUG

 fprintf(stderr,"ins (%d,%d) ",fonte->num_no_G,sucessor->num_no_G);

#endif

 return(aux);
}


/*******************************************************************/
/* void liga_grafo_i(INFODFNO *, INFODFNO *, NOGRAFOI *,           */
/*                   struct grafo *))                              */
/* Autor: Marcos L. Chaim                                          */
/* Versao: 1.0                                                     */
/* Data: 22/12/89                                                  */
/*                                                                 */
/* Funcao: esta operacao liga um no' ja' presente no grafo(i) a um */
/*         outro no' tambem ja' presente no grafo(i).              */
/*                                                                 */
/* Entradas: dois registros do tipo INFODFNO passados por referen- */
/*           cia, o apontador para o grafo(i) e o apontador para o */
/*           grafo G.                                              */
/*                                                                 */
/* Saida: nenhuma.                                                 */
/*                                                                 */
/*******************************************************************/

void liga_grafo_i(fonte,sucessor,grafo_i,g_suc)
INFODFNO * fonte;
INFODFNO * sucessor;
NOGRAFOI * grafo_i;
struct grafo * g_suc;
{

/* Declaracao de Variaveis Locais */

 NOGRAFOI * head, * aux;
 SUCGRAFOI * current;
 LISTPRED * p_aux1, * p_aux2;
 int i;


 /* Encontra o numero e o endereco de sucessor no grafo(i) */

 for(head = grafo_i; head != (NOGRAFOI *) NULL;head = head->next)
     if(is_equal_b_vector(&sucessor->deff,&((head->infosuc).deff)) &&
        sucessor->num_no_G == (head->infosuc).num_no_G &&
        head->repetido == NAO_REPETIDO) break;

 if(head == (NOGRAFOI *) NULL)
    error("* * Erro Fatal: Nao consegui encontrar o no' sucessor no grafo(i) * *\n");

 i = head->num_grafo_i;
 aux = head;

 /* Encontra o numero e o endereco de fonte no grafo(i) */

 for(head = grafo_i; head != (NOGRAFOI *) NULL;head = head->next)
     if(is_equal_b_vector(&fonte->deff,&((head->infosuc).deff)) &&
        fonte->num_no_G == (head->infosuc).num_no_G &&
        head->repetido == NAO_REPETIDO) break;

 if(head == (NOGRAFOI *) NULL)
    error("* * Erro Fatal: Nao consegui encontrar o no' fonte no grafo(i) * *\n");

/* insere o predecessor na lista de predecessores */

p_aux2 = aux->list_pred;
for(p_aux1 = aux->list_pred; p_aux1 != (LISTPRED *) NULL; p_aux1 = p_aux1->next)
   if(p_aux1->address == head)
        break;
   else
      p_aux2 = p_aux1;

/* se p_aux1 e' igual a NULL, e' porque este no' nao e' predecessor */

if(p_aux1 == (LISTPRED *)NULL)
   {
    if(p_aux2 == p_aux1)
      error("* * Erro Fatal: Estou ligando um elemento sem predecessor * *\n");
    p_aux1 = (LISTPRED *) malloc(sizeof(LISTPRED));
    if(p_aux1 == (LISTPRED *) NULL)
      error("* * Erro Fatal: Nao consegui alocar espaco para a lista de predecessores do grafo(i) * *\n");
    p_aux1->address = head;
    p_aux1->next = (LISTPRED *) NULL;
    p_aux2->next = p_aux1;
   }

/* liga os nos */

current = head->sucgfi;
head->sucgfi = (SUCGRAFOI *) malloc(sizeof(SUCGRAFOI));
if(head->sucgfi == (SUCGRAFOI *) NULL)
   error("* * Erro Fatal: Nao consegui alocar espaco para um no' no grafo(i) * *\n");
(head->sucgfi)->num_grafo_i = i;
(head->sucgfi)->no_address = aux;
if(e_primitivo(fonte->num_no_G,sucessor->num_no_G,g_suc))
   (head->sucgfi)->tipo = PRIMITIVO;
else
   (head->sucgfi)->tipo = HERDEIRO;
(head->sucgfi)->next = current;

#ifdef DEBUG

fprintf(stderr,"lig (%d,%d) ",fonte->num_no_G,sucessor->num_no_G);

#endif
}





/*******************************************************************/
/* void existe_no_igual(INFODFNO *, NOGRAFOI *)                    */
/* Autor: Marcos L. Chaim                                          */
/* Versao: 1.0                                                     */
/* Data: 22/12/89                                                  */
/*                                                                 */
/* Funcao :  retorna um valor indicando a existencia ou nao de um  */
/*           no grafo(i) que possui as mesmas informacoes do regis-*/
/*           to INFODFNO no_grafo_i.                               */
/*                                                                 */
/* Entrada: registro do tipo INFODFNO passado por referencia e um  */
/*          apontador para o grafo(i).                             */
/*                                                                 */
/*                                                                 */
/* Saida: valor indicando verdadeiro ou falso.                     */
/*                                                                 */
/*******************************************************************/

int existe_no_igual(no_grafo_i,grafo_i)
INFODFNO * no_grafo_i;
NOGRAFOI * grafo_i;
{

/* Declaracao de Variaveis Locais */

 NOGRAFOI * head;

 /* Procurando o no' no grafo(i) */

 for(head = grafo_i; head != (NOGRAFOI *) NULL;head = head->next)
     if(is_equal_b_vector(&no_grafo_i->deff,&((head->infosuc).deff)) &&
        no_grafo_i->num_no_G == (head->infosuc).num_no_G &&
        head->repetido == NAO_REPETIDO) break;

 if(head == (NOGRAFOI *) NULL)
     return(FALSE); /* nao existe o no' */
 else
     return(TRUE); /* encontrei o no'  */
}

/*******************************************************************/
/* void ini_info_descritores(INFODESCRITORES *)                    */
/* Autor: Marcos L. Chaim                                          */
/* Versao: 1.0                                                     */
/* Data: 22/12/89                                                  */
/*                                                                 */
/* Funcao :  inicia as diversas estruturas de dados contidas no re-*/
/*           gistro INFODESCRITORES passado por referencia.        */
/*                                                                 */
/* Entrada: registro do tipo INFODESCRITORES.                      */
/*                                                                 */
/* Saida: nenhuma.                                                 */
/*                                                                 */
/*******************************************************************/
void ini_info_descritores(descriptors_info)
INFODESCRITORES * descriptors_info;
{

 /* Inicia as diversas estruturas */

 reset_all(&(descriptors_info->Ni));
 reset_all(&(descriptors_info->Nt));

 descriptors_info->grafo_i = (NOGRAFOI *) NULL;
 descriptors_info->prim_arcs = (ARCPRIM *) NULL;
}


/*******************************************************************/
/* int no_im_existe_disjunto(INFODFNO *, NOGRAFOI *)               */
/* Autor: Marcos L. Chaim                                          */
/* Data: 8/1/90                                                    */
/* Versao: 1.0                                                     */
/*                                                                 */
/* Funcao: verificar se existe algum no' do grafo(i) que possui o  */
/*         mesmo numero no grafo(i), porem o conjunto deff e' dis- */
/*         junto do no' em questao.                                */
/*                                                                 */
/* Entradas: apontador para o registro INFODFNO contendo informa-  */
/*           coes de fluxo de dados do no' em questao e apontador  */
/*           para o grafo(i).                                      */
/*                                                                 */
/* Saida: TRUE se existe tal no' e FALSE caso contrario.           */
/*                                                                 */
/* Funcoes chamadas: is_equal_b_vector() e esta_contido_b_vector.*/
/*                                                                 */
/*******************************************************************/

int no_im_existe_disjunto(no_info, grafo_i)
INFODFNO * no_info;
NOGRAFOI * grafo_i;
{

 /* Declaracao de Variaveis Locais */

 NOGRAFOI * head;

 /* Encontra o no' no grafo(i) */

 for(head = grafo_i; head != (NOGRAFOI *) NULL; head = head->next)
   if((head->infosuc).num_no_G == no_info->num_no_G &&
       head->repetido == NAO_REPETIDO &&
       !is_equal_b_vector(&(head->infosuc).deff,&(no_info->deff)) &&
       !is_contained_in_b_vector(&(head->infosuc).deff,&(no_info->deff)) &&
       !is_contained_in_b_vector(&(no_info->deff),&(head->infosuc).deff))
          return(TRUE);

 return(FALSE);
}
