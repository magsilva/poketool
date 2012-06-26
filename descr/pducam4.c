#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <alloc.h>

#include "headerli.h"
#include "hparserg.h"
#include "hrotsem.h"
#include "header.h"
#include "hpducam.h"

/*******************************************************************/
/* void faca_pred_pfi(INFODFNO *, NOGRAFOI *)                      */
/* Autor: Marcos L. Chaim                                          */
/* Data: 8/1/90                                                    */
/* Versao: 1.0                                                     */
/*                                                                 */
/* Funcao: tornar um dos arcos contstituido por um predecessor e o */
/*         o no' passado como parametro um arco pfi.               */
/*                                                                 */
/* Entradas: apontador para o registro INFODFNO contendo informa-  */
/*           coes de fluxo de dados do no' em questao e apontador  */
/*           para o grafo(i).                                      */
/*                                                                 */
/* Saida: nenhuma.                                                 */
/*                                                                 */
/* Funcoes chamadas: is_equal_bitvector().                         */
/*                                                                 */
/*******************************************************************/

void faca_pred_pfi(suc_no, grafo_i)
INFODFNO * suc_no;
NOGRAFOI * grafo_i;
{

 /* Declaracao de Variaveis Locais */

 NOGRAFOI * head;
 SUCGRAFOI * an_head;


 /* Encontra suc_no no grafo(i) */

 for(head = grafo_i; head != (NOGRAFOI *) NULL; head = head->next)
   if((head->infosuc).num_no_G == suc_no->num_no_G &&
       head->repetido == NAO_REPETIDO &&
       is_equal_bitvector(&(head->infosuc).deff,&(suc_no->deff)))
         break;

 if(head == (NOGRAFOI *) NULL)
     error("* * Erro Fatal: Nao encontrei o no' no grafo(i) * *\n");

 if(head->list_pred == (LISTPRED *) NULL)
     error("* * Erro Fatal: Este no' nao tem predecessores, como posso alterar condicao!!! * *\n");

 /* altera condicao do arco */

 for(an_head = ((head->list_pred)->address)->sucgfi; an_head != (SUCGRAFOI *) NULL;
     an_head = an_head->next)
      if(((an_head->no_address)->infosuc).num_no_G == suc_no->num_no_G &&
         (an_head->no_address)->repetido == NAO_REPETIDO &&
         is_equal_bitvector(&((an_head->no_address)->infosuc.deff),
                            &(suc_no->deff))) {
             an_head->tipo = PFI;
             return;
            }

 error("* * Erro Fatal: Nao consegui alterar o tipo de um no' * *\n");

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
/* Funcoes chamadas: is_equal_bitvector().                         */
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
       head->repetido == NAO_REPETIDO &&
       is_equal_b_vector(&(head->infosuc).deff,&(fonte->deff)))
         break;

 if(head == (NOGRAFOI *) NULL)
     error("PROBLEMA! Nao encontrei o no' no grafo(i)\n");

 /* altera condicao do arco */

 for(an_head = head->sucgfi; an_head != (SUCGRAFOI *) NULL;
     an_head = an_head->next)
      if(((an_head->no_address)->infosuc).num_no_G == sucessor->num_no_G &&
         (an_head->no_address)->repetido == NAO_REPETIDO &&
         is_equal_b_vector(&(((an_head->no_address)->infosuc).deff),
                            &(sucessor->deff)))
            {
             an_head->tipo = PFI;
             return;
            }

 error("* * Erro Fatal: Nao consegui alterar o tipo de um no' * *\n");

 }


/*******************************************************************/
/* NOGRAFOI * find_address_grafo_i(INFODFNO *, NOGRAFOI *)         */
/* Autor: Marcos L. Chaim                                          */
/* Data: 8/1/90                                                    */
/* Versao: 1.0                                                     */
/*                                                                 */
/* Funcao: encontra o endereco de um no' no grafo(i), a partir das */
/*         informacoes de fluxo de dados do no'.                   */
/*                                                                 */
/* Entradas: apontador para o registro INFODFNO contendo informa-  */
/*           coes de fluxo de dados do no' em questao e apontador  */
/*           para o grafo(i).                                      */
/*                                                                 */
/* Saida: apontador para o no' no grafo(i).                        */
/*                                                                 */
/* Funcoes chamadas: is_equal_bitvector().                         */
/*                                                                 */
/*******************************************************************/

NOGRAFOI * find_address_grafo_i(no_info, grafo_i)
INFODFNO * no_info;
NOGRAFOI * grafo_i;
{

 /* Declaracao de Variaveis Locais */

 NOGRAFOI * head;

 /* Encontra o no' no grafo(i) */

 for(head = grafo_i; head != (NOGRAFOI *) NULL; head = head->next)
   if((head->infosuc).num_no_G == no_info->num_no_G &&
       head->repetido == NAO_REPETIDO &&
       is_equal_bitvector(&(head->infosuc).deff,&(no_info->deff)))
         break;

 if(head == (NOGRAFOI *) NULL)
     error("PROBLEMA! Nao encontrei o no' no grafo(i)\n");

 return(head);
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
/* Funcoes chamadas: is_equal_bitvector() e esta_contido_bitvector.*/
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
       !is_equal_bitvector(&(head->infosuc).deff,&(no_info->deff)) &&
       !esta_contido_bitvector(&(head->infosuc).deff,&(no_info->deff)) &&
       !esta_contido_bitvector(&(no_info->deff),&(head->infosuc).deff))
          return(TRUE);

 return(FALSE);
}

/*******************************************************************/
/* INFODFNO find_disjunto_no_imagem(INFODFNO *, NOGRAFOI *)        */
/* Autor: Marcos L. Chaim                                          */
/* Data: 8/1/90                                                    */
/* Versao: 1.0                                                     */
/*                                                                 */
/* Funcao: encontrar no' imagem disjunto no grafo(i).              */
/*                                                                 */
/* Entradas: apontador para o registro INFODFNO contendo informa-  */
/*           coes de fluxo de dados do no' em questao e apontador  */
/*           para o grafo(i).                                      */
/*                                                                 */
/* Saida: registro contendo as informacoes de fluxo de dados do no.*/
/*                                                                 */
/* Funcoes chamadas: is_equal_bitvector() e esta_contido_bitvector.*/
/*                                                                 */
/*******************************************************************/

INFODFNO find_disjunto_no_imagem(no_info, grafo_i)
INFODFNO * no_info;
NOGRAFOI * grafo_i;
{

 /* Declaracao de Variaveis Locais */

 NOGRAFOI * head;
 INFODFNO aux;

 /* Encontra o no' no grafo(i) */

 for(head = grafo_i; head != (NOGRAFOI *) NULL; head = head->next)
   if((head->infosuc).num_no_G == no_info->num_no_G &&
       head->repetido == NAO_REPETIDO &&
       !is_equal_bitvector(&(head->infosuc).deff,&(no_info->deff)) &&
       !esta_contido_bitvector(&(head->infosuc).deff,&(no_info->deff)) &&
       !esta_contido_bitvector(&(no_info->deff),&(head->infosuc).deff))
          return(head->infosuc);

error("* * Erro Fatal: Nao encontrei no' imagem no grafo(i) * *\n");
aux.num_no_G = 0;
reset_all(&aux.deff);
return(aux);
}

/*******************************************************************/
/* int esta_deff_contido(INFODFNO *, NOGRAFOI *)                   */
/* Autor: Marcos L. Chaim                                          */
/* Data: 8/1/90                                                    */
/* Versao: 1.0                                                     */
/*                                                                 */
/* Funcao: verificar se existe algum no' do grafo(i) cujo conjunto */
/*         deff contem o conjunto deff do no' passado como parame- */
/*         tro.                                                    */
/*                                                                 */
/* Entradas: apontador para o registro INFODFNO contendo informa-  */
/*           coes de fluxo de dados do no' em questao e apontador  */
/*           para o grafo(i).                                      */
/*                                                                 */
/* Saida: TRUE se existe tal no' e FALSE caso contrario.           */
/*                                                                 */
/* Funcoes chamadas: esta_contido_bitvector().                     */
/*                                                                 */
/*******************************************************************/

int esta_deff_contido(no_info, grafo_i)
INFODFNO * no_info;
NOGRAFOI * grafo_i;
{

 /* Declaracao de Variaveis Locais */

 NOGRAFOI * head;

 /* Encontra o no' no grafo(i) */

 for(head = grafo_i; head != (NOGRAFOI *) NULL; head = head->next)
   if((head->infosuc).num_no_G == no_info->num_no_G &&
       head->repetido == NAO_REPETIDO &&
       esta_contido_bitvector(&(no_info->deff),&(head->infosuc).deff))
          return(TRUE);

 return(FALSE);
}

/*******************************************************************/
/* INFODFNO find_no_imagem(INFODEFNO *, NOGRAFOI *)                */
/* Autor: Marcos L. Chaim                                          */
/* Data: 8/1/90                                                    */
/* Versao: 1.0                                                     */
/*                                                                 */
/* Funcao: encontrar no' imagem no grafo(i) que contem o conjunto  */
/*         deff do no' passado como parametro.                     */
/*                                                                 */
/* Entradas: apontador para o registro INFODFNO contendo informa-  */
/*           coes de fluxo de dados do no' em questao e apontador  */
/*           para o grafo(i).                                      */
/*                                                                 */
/* Saida: registro contendo as informacoes de fluxo de dados do no.*/
/*                                                                 */
/* Funcoes chamadas: esta_contido_bitvector().                     */
/*                                                                 */
/*******************************************************************/

INFODFNO find_no_imagem(no_info, grafo_i)
INFODFNO * no_info;
NOGRAFOI * grafo_i;
{

 /* Declaracao de Variaveis Locais */

 NOGRAFOI * head;
 INFODFNO aux;

 /* Encontra o no' no grafo(i) */

 for(head = grafo_i; head != (NOGRAFOI *) NULL; head = head->next)
   if((head->infosuc).num_no_G == no_info->num_no_G &&
       head->repetido == NAO_REPETIDO &&
       esta_contido_bitvector(&(no_info->deff),&(head->infosuc).deff))
          return(head->infosuc);

error("* * Erro Fatal: Nao encontrei no' imagem no grafo(i) * *\n");
aux.num_no_G = 0;
reset_all(&aux.deff);
return(aux);
}

/*******************************************************************/
/* void info_pfi_elimination(INFODFNO *,NOGRAFOI *,struct grafo *) */
/* Autor: Marcos L. Chaim                                          */
/* Data: 8/1/90                                                    */
/* Versao: 1.0                                                     */
/*                                                                 */
/* Funcao: recupera o tipo do arco que foi ajustado como primitivo */
/*         pfi.                                                    */
/*                                                                 */
/* Entradas: apontador para o registro INFODFNO contendo informa-  */
/*           coes de fluxo de dados do no' em questao,  apontador  */
/*           para o grafo(i) e apontador para o grafo_G.           */
/*                                                                 */
/* Saida: nenhuma.                                                 */
/*                                                                 */
/* Funcoes chamadas: is_equal_bitvector().                         */
/*                                                                 */
/*******************************************************************/

void info_pfi_elimination(suc_no, grafo_i,g_suc)
INFODFNO * suc_no;
NOGRAFOI * grafo_i;
struct grafo * g_suc;
{

 /* Declaracao de Variaveis Locais */

 NOGRAFOI * head;
 SUCGRAFOI * an_head;
 LISTPRED * p_aux;

 /* Encontra suc_no no grafo(i) */

 for(head = grafo_i; head != (NOGRAFOI *) NULL; head = head->next)
   if((head->infosuc).num_no_G == suc_no->num_no_G &&
       head->repetido == NAO_REPETIDO &&
       is_equal_bitvector(&(head->infosuc).deff,&(suc_no->deff)))
         break;

 if(head == (NOGRAFOI *) NULL)
     error("PROBLEMA! Nao encontrei o no' no grafo(i)\n");

 /* altera condicao do arco */

 for(p_aux = head->list_pred; p_aux != (LISTPRED *) NULL; p_aux = p_aux->next)
   for(an_head = ((head->list_pred)->address)->sucgfi; an_head != (SUCGRAFOI *) NULL;
       an_head = an_head->next)
        if(((an_head->no_address)->infosuc).num_no_G == suc_no->num_no_G &&
           (an_head->no_address)->repetido == NAO_REPETIDO &&
            is_equal_bitvector(&((an_head->no_address)->infosuc.deff),
                               &(suc_no->deff)))
               {
                if(e_primitivo(((head->list_pred)->address)->infosuc.num_no_G,suc_no->num_no_G,g_suc))
                   an_head->tipo = PRIMITIVO;
                else
                   an_head->tipo = HERDEIRO;
               }

}
/********************************************************************/
/* void imprime_char_stack(STACKCHAR *, FILE *)                     */
/* Autor: Marcos L. Chaim                                           */
/* Data: 12/12/89                                                   */
/* Versao: 1.0                                                      */
/*                                                                  */
/* Funcao: esta funcao imprime o conteudo de uma pilha de caracteres*/
/*         num arquivo cujo descritor e' passado como parametro.    */
/*                                                                  */
/* Entrada: apontador para a pilha de caracteres e o descritor do   */
/*          arquivo.                                                */
/*                                                                  */
/* Saida: nenhuma.                                                  */
/*                                                                  */
/********************************************************************/

void imprime_char_stack(stack, archive)
STACKCHAR * stack;
FILE * archive;
{

 /* Declaracao de Variaveis Locais */

 STACKCHAR stack_aux;

 CHAR5 aux;

 mk_nl_char(&stack_aux);  /* inicia pilha auxiliar */

 while(!empty_char(stack))
   {
    aux = pop_char(stack);
    push_char(&aux,&stack_aux);
   }

while(!empty_char(&stack_aux))
   {
    aux = pop_char(&stack_aux);
    push_char(&aux,stack);
    fprintf(archive,"%s ",aux.array);
   }
 fprintf(archive,"\n");
}
