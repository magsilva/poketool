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
#include <alloc.h>

#include "headerli.h"
#include "hparserg.h"
#include "hrotsem.h"
#include "header.h"
#include "hpducam.h"
extern int n_des_pu;

/*******************************************************************/
/* void des_pu_gen(INFODESCRITORES *, FILE *, FILE *, FILE *)      */
/*                                                                 */
/* Autor: Marcos L. Chaim                                          */
/* Data: 30/08/90                                                  */
/*                                                                 */
/* Funcao: gerar os descritores para o criterio todos potenciais   */
/*         usos e potenciais usos/du e as associacoes de cada des- */
/*         critor.                                                 */
/*                                                                 */
/* Entradas: apontador para as informacoes de descritores e os ar- */
/*           quivos que conterao os descritores e as associacoes.  */
/*                                                                 */
/* Saida: nenhuma.                                                 */
/*                                                                 */
/*******************************************************************/

void des_pu_gen(des_info,g_suc, des_pu,des_pudu,pu_assoc)
INFODESCRITORES * des_info;
struct grafo * g_suc;
FILE * des_pu;
FILE * des_pudu;
FILE * pu_assoc;
{

 /* Declaracao de Variaveis Locais */

 NOGRAFOI * no_grfi;
 LISTPRED * pred_aux;
 STACKARC des_stack;
 ARCINFO aux;


 while(!empty_address(&(des_info->term_nodes)))
 {
 no_grfi = pop_address(&(des_info->term_nodes));

 mk_nl_arc(&des_stack); /* inicia a pilha de arcos */


 for(pred_aux = no_grfi->list_pred ; pred_aux != (LISTPRED *) NULL;
     pred_aux = pred_aux->next)
      {
       /* coloca arco  no registro de informacoes do arco */

       aux.arco.coor = (no_grfi->infosuc).num_no_G;
       aux.arco.abs = (pred_aux->address)->infosuc.num_no_G;

         if(e_pfi(pred_aux,no_grfi))
         aux.tipo = PFI; /* e' um arco primitivo pfi */
         else
         if(belongs_pair_stack(&(aux.arco),&(des_info->Er)))
           aux.tipo = PR;  /* e' um arco primitivo pr */
         else
            if(e_primitivo(aux.arco.abs,aux.arco.coor,g_suc))
              {
               if(!belongs_pair_stack(&(aux.arco),&(des_info->Erep)))
                  aux.tipo = PRIMITIVO;
               else
                   aux.tipo = PRIM_REP;
              }
            else
               aux.tipo = HERDEIRO;

       if(aux.tipo == HERDEIRO)
         strcpy(aux.des_meio,"Nh*"); /* copia "Nh" em aux.des_meio */
       else /* arco e' primitivo */
         aux.des_meio[0] = '\0';

       /* empilha o conteudo de informacao do arco */

       push_arc(&aux, &des_stack);

       /* continua a pesquisa no grafo(i) */

       pes_pu_grfi(pred_aux->address, des_info, &des_stack,g_suc,des_pu,des_pudu,pu_assoc);
   }
  }
}

/*******************************************************************/
/* void pes_pu_grfi(NOGRAFOI *,INFODESCRITORES *, STACKARC *,      */
/*                  FILE *, FILE *, FILE *)                        */
/* Autor: Marcos L. Chaim                                          */
/* Data: 30/08/90                                                  */
/*                                                                 */
/* Funcao: faz a pesquisa em profundidade para gerar os descritores*/
/*         e associacoes para o criterio todos potenciais usos e   */
/*         usos/du.                                                */
/*                                                                 */
/* Entradas: apontador para o no' do grafo(i) onde continua a pes- */
/*           quisa, apontador para as informacoes de descritores e */
/*           apontador para pilha de arcos e ponteiros de arquivos.*/
/*                                                                 */
/* Saida: nenhuma.                                                 */
/*                                                                 */
/*******************************************************************/

void pes_pu_grfi(no_pes,des_info, des_stack, g_suc,des_pu,des_pudu,archive)
NOGRAFOI * no_pes;
INFODESCRITORES * des_info;
STACKARC * des_stack;
struct grafo * g_suc;
FILE * des_pu;
FILE * des_pudu;
FILE * archive;
{

 /* Declaracao de Variaveis Locais */

 LISTPRED * pred_aux;
 ARCINFO aux;

 for(pred_aux = no_pes->list_pred; pred_aux != (LISTPRED *) NULL;
     pred_aux = pred_aux->next)
      {
       /* coloca arco  no registro de informacoes do arco */
       aux.arco.coor = (no_pes->infosuc).num_no_G;
       aux.arco.abs = (pred_aux->address)->infosuc.num_no_G;

         if(e_pfi(pred_aux,no_pes))
         aux.tipo = PFI; /* e' um arco primitivo pfi */
         else
         if(belongs_pair_stack(&(aux.arco),&(des_info->Er)))
           aux.tipo = PR;  /* e' um arco primitivo pr */
         else
            if(e_primitivo(aux.arco.abs,aux.arco.coor,g_suc))
              {
               if(!belongs_pair_stack(&(aux.arco),&(des_info->Erep)))
                  aux.tipo = PRIMITIVO;
               else
                   aux.tipo = PRIM_REP;
              }
            else
               aux.tipo = HERDEIRO;

       /* ajusta o conteudo de des_meio */

       if(((pred_aux->address)->list_pred) != (LISTPRED *) NULL)
          {
           if(aux.tipo == HERDEIRO)
               strcpy(aux.des_meio,"Nh*"); /* copia "Nh" em aux.des_meio */
           else /* arco e' primitivo */
               aux.des_meio[0] = '\0';
           }
       else
           aux.des_meio[0] = '\0';

       /* empilha os conteudos de informacao dos arcos */

       push_arc(&aux, des_stack);

       /* continua a pesquisa no grafo(i) */

       pes_pu_grfi(pred_aux->address, des_info, des_stack, g_suc, des_pu,des_pudu,archive);
   }
if(no_pes->list_pred == (LISTPRED *) NULL) /* fim da pesquisa */
 { /* imprime descritor e caminho */
  ++n_des_pu; /* atualiza contador de descritores */
  pu_imprime_caminho(des_info,des_stack,archive, n_des_pu);
  pu_imprime_descritor(des_stack,des_pu,des_pudu,des_info);
 }
}

/********************************************************************/
/* void pu_imprime_descritor(STACKARC *, int)                       */
/* Autor: Marcos L. Chaim                                           */
/* Data: 31/8/90                                                    */
/* Versao: 1.0                                                      */
/*                                                                  */
/* Funcao: esta funcao imprime o conteudo de uma pilha de arcos     */
/*         criando um descritor para os arcos contidos na pilha.    */
/*                                                                  */
/* Entrada: apontador para a pilha de arcos e numero do no' que ini-*/
/*          cia o descritor.                                        */
/*                                                                  */
/* Saida: nenhuma.                                                  */
/*                                                                  */
/********************************************************************/

void pu_imprime_descritor(stack,des_pu,des_pudu,des_info)
STACKARC * stack;
FILE * des_pu;
FILE * des_pudu;
INFODESCRITORES * des_info;
{

 /* Declaracao de Variaveis Locais */

 ARCINFO aux, an_aux;

 int i;

 i =(des_info->grafo_i)->infosuc.num_no_G; /* no' i */

 fprintf(des_pu,"%2d) ",n_des_pu); /* escreve o numero do descritor */
 fprintf(des_pudu,"%2d) ",n_des_pu); /* escreve o numero do descritor */

/* trata o primeiro arco da pilha de arcos */

aux = pop_arc(stack);

if(aux.tipo != HERDEIRO)
  { /* arco e' primitivo */
    if (aux.arco.abs == i)
       { /* no' i faz parte do primeiro arco */
        fprintf(des_pu,"%s ","N*");
        fprintf(des_pudu,"%s ","N*");
       }
    else
       {
        fprintf(des_pu,"%s %d ","N*",i);
        fprintf(des_pudu,"%s %d ","N*",i);
       }
    fprintf(des_pu,"%d %s %d ",aux.arco.abs,"Ni*", aux.arco.coor);
    fprintf(des_pudu,"%d %d ",aux.arco.abs, aux.arco.coor);
   }
else
   {
    fprintf(des_pu,"%s %d ","N*",i);
    fprintf(des_pudu,"%s %d ","N*",i);
   }

/* imprime o descritor */

while(!empty_arc(stack))
   {
    an_aux = aux;
    aux = pop_arc(stack);
    if(aux.tipo != HERDEIRO)
     { /* o arco e' primitivo */
      if(an_aux.arco.coor == aux.arco.abs && an_aux.tipo != HERDEIRO)
        {
         fprintf(des_pu,"%d ", aux.arco.coor); /* so' imprime no' alvo */
         fprintf(des_pudu,"%d ", aux.arco.coor); /* so' imprime no' alvo */
        }
      else
        {
         fprintf(des_pu,"%d %s %d ", aux.arco.abs, "Ni*", aux.arco.coor); /* imprime o arco */
         fprintf(des_pudu,"%d %d ", aux.arco.abs, aux.arco.coor); /* imprime o arco */
        }
     }
    else  /* arco herdeiro */
      if(strcmp(an_aux.des_meio,"Nh*") && !test_bit(aux.arco.coor,&(des_info->Nt)))
         {
          fprintf(des_pudu,"%s ",aux.des_meio);
          fprintf(des_pu,"%s ","Ni*");
         }

   }
 fprintf(des_pu,"\n");
 fprintf(des_pudu,"\n");
}



/********************************************************************/
/* void pu_imprime_caminho(INFODESCRITORES *, STACKARC *, FILE *)   */
/* Autor: Marcos L. Chaim                                           */
/* Data: 31/08/90                                                   */
/* Versao: 1.0                                                      */
/*                                                                  */
/* Funcao: esta funcao imprime a associacao dos arcos de uma pilha  */
/*         de arcos num arquivo cujo descritor e' passado como      */
/*         parametro.                                               */
/*                                                                  */
/* Entradas: apontador para as informacoes do grafo(i), apontador   */
/*           para a pilha de arcos e apontador para o arquivo.      */
/*                                                                  */
/* Saida: nenhuma.                                                  */
/*                                                                  */
/********************************************************************/

void pu_imprime_caminho(des_info, stack, archive, n_assoc)
INFODESCRITORES * des_info;
STACKARC * stack;
FILE * archive;
int n_assoc;
{

/* Declaracao de Variaveis Locais */

int i;

STACKARC stack_aux;

ARCINFO aux;

i =(des_info->grafo_i)->infosuc.num_no_G; /* no' i */

fprintf(archive,"%2d) ",n_assoc); /* escreve o numero da associacao */

mk_nl_arc(&stack_aux);  /* inicia pilha auxiliar */

/* trata primeiro arco */

aux = pop_arc(stack);
push_arc(&aux,&stack_aux);

fprintf(archive,"<");

if(aux.tipo != HERDEIRO)
   {
    if(aux.arco.abs == i)
      fprintf(archive,"(%d,%d)",aux.arco.abs,aux.arco.coor);
    else
      fprintf(archive,"%d,(%d,%d)",i,aux.arco.abs,aux.arco.coor);
   }

while(!empty_arc(stack))
   {
    aux = pop_arc(stack);
    push_arc(&aux,&stack_aux);

    if(aux.tipo != HERDEIRO)
       fprintf(archive,",(%d,%d)",aux.arco.abs,aux.arco.coor);
   }
fprintf(archive,">\n");

/* recupera a pilha */

while(!empty_arc(&stack_aux))
  { aux = pop_arc(&stack_aux);
    push_arc(&aux,stack);
  }

}

/*******************************************************************/
/* int e_pfi(LISTPRED *, NOGRAFOI *)                               */
/* Autor: Marcos L. Chaim                                          */
/* Data: 3/9/90                                                    */
/* Versao: 1.0                                                     */
/*                                                                 */
/* Funcao: determinar se um arco e' primitivo de fluxo de dados ou */
/*         nao.                                                    */
/*                                                                 */
/* Entrada: apontador para o predecessor e para o sucessor no arco.*/
/*                                                                 */
/* Saida: TRUE se for primitivo de fluxo de dados, FALSE caso con- */
/*        trario.                                                  */
/*                                                                 */
/*******************************************************************/

int e_pfi(pred, suc)
LISTPRED * pred;
NOGRAFOI * suc;
{

/* Declaracao de Variaveis Locais */

int retorno = FALSE;
SUCGRAFOI * aux;

for(aux = (pred->address)->sucgfi; aux != (SUCGRAFOI *) NULL; aux=aux->next)
  if(suc == aux->no_address)
     {
      if(aux->tipo == PFI)
        {
         retorno = TRUE;
         break;
        }
      else
         break;
      }

return(retorno);
