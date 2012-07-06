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
#include "hparserg.h"
#include "hrotsem.h"
#include "header.h"
#include "hpducam.h"

extern int n_des_pu;
extern int no_nos;
extern int no_vars;
extern NODEINFO info_no;
extern struct grafo * graph;
extern struct grafo * graphaux;

/*
** void delete_elem_list_deff() -- deleta um elemnto da lista.
*/
void delete_elem_list_deff(listdeff, deff)
LISTBITVECTOR ** listdeff;
LISTBITVECTOR * deff;
{
 LISTBITVECTOR * bit_aux, *bit_aux2;

 for(bit_aux = bit_aux2 = *listdeff; bit_aux != NULL; bit_aux=bit_aux->next)
   {
     if(bit_aux == deff)
       {
        b_vector_destroy(&(bit_aux->conj));
        if(bit_aux == *listdeff)
           *listdeff = bit_aux->next;
        else
           bit_aux2->next = bit_aux->next;  
        free(bit_aux);
        break;
       }
     bit_aux2 = bit_aux;
   }
}

/*
** void lib_list_deff() -- libera lista de conjunto deff.
*/

void lib_list_deff(listdeff)
LISTBITVECTOR * listdeff;
{
  if(listdeff != NULL)
    {
      if(listdeff->next != NULL)
        lib_list_deff(listdeff->next);
      b_vector_destroy(&(listdeff->conj));
      free(listdeff);
    }
}

/*
** insere_list_deff () -- cria elemento na lista de deff.
*/

void insere_list_deff(listdeff, bitvector)
LISTBITVECTOR ** listdeff;
b_vector * bitvector;     
{
 LISTBITVECTOR * bit_aux, *bit_aux2, *bit_aux3;
 int cardnew, cardcur;
   
 bit_aux = (LISTBITVECTOR *) malloc(sizeof(LISTBITVECTOR));
 if(bit_aux != (LISTBITVECTOR *) NULL)
   {
     b_vector_cons(&(bit_aux->conj),no_nos/BYTE+1,0);
     b_vector_cpy(&(bit_aux->conj),bitvector);
     bit_aux->next = NULL;
    }
   else
     error("* * Erro Fatal: Nao consegui alocar espaco para a lista de conjuntos deff * *\n");


 /* Coloca em ordem crescente de tamanho do conjunto deff */

 /* cadinalidade do novo conjunto */
   
 cardnew = count_b_vector(&(bit_aux->conj));
   
 for(bit_aux2 = bit_aux3 = *listdeff;
     bit_aux2 != NULL; bit_aux2=bit_aux2->next)
    {
    cardcur = count_b_vector(&(bit_aux2->conj));
    if(cardnew <= cardcur)
      break;
    bit_aux3 = bit_aux2;
    }

 if(*listdeff == bit_aux2)
   {
    bit_aux->next = *listdeff;
   *listdeff = bit_aux;
   }
  else
   {
    bit_aux3->next=bit_aux;
    bit_aux->next = bit_aux2;
   }
}

/*
** ajust_deff_set() -- ajusta a lista de deff dos arcos primitivos
**                     para ajustar bug identificado pela Silvia.
*/

LISTBITVECTOR * ajust_deff_set(list_deff, arco)
LISTBITVECTOR * list_deff;
PAIRINT arco;
{
 int card = 0, no_id;
 LISTBITVECTOR* listcpy=NULL,  * bit_aux = NULL, *bit_aux2;
 b_vector deff_core, aux;
 
 b_vector_cons(&deff_core,no_vars/BYTE+1,1);
 b_vector_cons(&aux,no_vars/BYTE+1,0);

  for(bit_aux = list_deff; bit_aux != (LISTBITVECTOR *) NULL;
     bit_aux = bit_aux->next)
     {
       (void) inter_b_vector(&deff_core,&(bit_aux->conj));
       card++;
     }

  if(card == 1 && cardinal(graphaux[arco.abs].list_suc) > 1)
    {
      /* trata de maneira especial o arco primitivo que possui
         mais de um predecessor e um unico deff.
         Caso tipico: arco de saida de um loop.
         Solucao: separa as variaveis para evitar que todas as variaveis
         precisem estar vivas.
         Cria lista de variaveis isoladas */
      for(no_id=0; no_id < no_vars; no_id++)
        if(test_bit(no_id,&(list_deff->conj)))
           {
            reset_all(&aux);
            set_bit(no_id,&aux);
            insere_list_deff(&listcpy, &aux); 
           }
     }   
  else  /* Caso Normal */
    {
  /* Cria lista repetida */  
      for(bit_aux = list_deff; bit_aux != (LISTBITVECTOR *) NULL;
          bit_aux = bit_aux->next)
        {
          b_vector_cpy(&aux,&(bit_aux->conj));
          (void) sub_b_vector(&aux, &deff_core);
          insere_list_deff(&listcpy, &aux);     
        }

  /* Calcula os conjuntos deff linearmente independentes */
  /* Conjuntos foram inseridos em ordem crescente */
  
      for(bit_aux=listcpy; bit_aux != (LISTBITVECTOR *) NULL;
          bit_aux = bit_aux->next)
        for(bit_aux2=listcpy; bit_aux2 != (LISTBITVECTOR *) NULL;
            bit_aux2 = bit_aux2->next)
          if(bit_aux != bit_aux2)
            (void) sub_b_vector(&(bit_aux2->conj), &(bit_aux->conj));

  /* Insere conjunto de variaveis comuns */
  
      insere_list_deff(&listcpy, &deff_core);     

  /* Elimina conjuntos deff vazios */
  
      for(bit_aux=listcpy; bit_aux != (LISTBITVECTOR *) NULL;
          bit_aux = bit_aux->next)
        if(empty_bit(&(bit_aux->conj)))
          delete_elem_list_deff(&listcpy,bit_aux);
    }
  
 b_vector_destroy(&deff_core);
 b_vector_destroy(&aux);
 return listcpy;
}

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

void des_pu_gen(des_info, des_pu,des_pudu,pu_assoc)
INFODESCRITORES * des_info;
FILE * des_pu;
FILE * des_pudu;
FILE * pu_assoc;
{

 /* Declaracao de Variaveis Locais */

 ARCPRIM * prim_aux;
 b_vector Nnv, deff_int;
 LISTBITVECTOR * bit_aux;
 PAIRINT arco;
 struct no * conodef = (struct no *) NULL, * no_def;
 int i,j,no;
 int vir = FALSE; /* flag para controlar a impressao das associacoes */

 b_vector_cons(&Nnv,no_nos/BYTE+1,0);
 b_vector_cons(&deff_int,no_vars/BYTE+1,0);
 
 conodef = det_conodef(conodef); /* determina lista de nos que possuem defi-
                                    nicao de variaveis */

 i  = (des_info->grafo_i)->infosuc.num_no_G; /* no' i */

 /* Nnv = des_info->Ni; 

 b_vector_cpy(&Nv,&des_info->Ni);*/

 for(prim_aux = des_info->prim_arcs; prim_aux != (ARCPRIM *) NULL;
     prim_aux = prim_aux->next)
   {
     
     for(bit_aux = ajust_deff_set(prim_aux->deff_ptr, prim_aux->arco);
         bit_aux != (LISTBITVECTOR *) NULL;
         bit_aux = bit_aux->next)
         {
          ++n_des_pu;            /* incrementa contador de descritores */
          /* Nnv = des_info->Ni;    inicia Nnv com o conjunto Ni */
          b_vector_cpy(&Nnv,&des_info->Ni);
          
          arco = prim_aux->arco; /* faz arco contem o arco primitivo */

/* escreve os descritores para os criterios todos-pot-usos e pot-usos/du */

          if(arco.abs == i)
            { /* e' igual a i */

             fprintf(des_pu,"%d) N* %d [ Nnv* %d ]* %d\n",
              n_des_pu, arco.abs,arco.abs, arco.coor); /* imprime o arco pu */

             fprintf(des_pudu,"%d) N* %d %d\n",
             n_des_pu, arco.abs, arco.coor); /* imprime o arco pu/du */
            }
          else
            {
             fprintf(des_pu,"%d) N* %d Nnv* %d [ Nnv* %d ]* %d\n",
             n_des_pu, i, arco.abs, arco.abs, arco.coor);

             fprintf(des_pudu,"%d) N* %d Nnvlf* %d %d\n",
             n_des_pu, i,arco.abs,arco.coor);
            }

         /* calcula o conjunto de nos livre de definicao para a associacao */

           for(no_def = conodef; no_def != (struct no *) NULL;
               no_def = no_def->proximo)
              {

               reset_bit(i,&Nnv); /* reseta o no' i */

               /* calcula a intersecao */

               b_vector_cpy(&deff_int,&(info_no[no_def->num].defg_i));
               (void) inter_b_vector(&deff_int,&(bit_aux->conj));

               if(!empty_bit(&deff_int))
                 reset_bit(no_def->num,&Nnv); /* reseta no' com def de variavel
                                                 pertencente ao deff*/
              }

         /* escreve Nnv nos arquivos de descritores */

         fprintf(des_pu,"Nnv = ");
         fprintf(des_pudu,"Nnvlf = ");

         for(no = 1; no <= no_nos; ++no)
           if(test_bit(no,&Nnv))
             {
              fprintf(des_pu,"%d ",no);
              fprintf(des_pudu,"%d ",no);
             }
         fprintf(des_pu,"\n\n");
         fprintf(des_pudu,"\n\n");

/* escreve as associacoes no arquivo que as descreve */

         fprintf(pu_assoc,"%d) <%d,(%d,%d),{",n_des_pu,i,arco.abs,arco.coor);

         vir = FALSE;

         for(j=0; j>=0 && j < no_vars; ++j)
           {
             if(test_bit(j,&(bit_aux->conj)))
                  {
                   if(vir)
                     fprintf(pu_assoc,",");
		   show_description(j,pu_assoc);
                   vir = TRUE;
                  }
           }

         fprintf(pu_assoc," }>\n");
         }
    /* libera espaco de lista de conjuntos deff ajustado */ 
    (void) lib_list_deff(bit_aux);
   }

/* testa se lista e' vazia */

if(des_info->prim_arcs ==  (ARCPRIM *) NULL)
   {
    ++n_des_pu;            /* incrementa contador de descritores */
    Nnv = des_info->Ni;    /* inicia Nnv com o conjunto Ni */

    /* calcula o deff do no' i */

    b_vector_cpy(&deff_int,&(info_no[i].defg_i));
    (void) union_b_vector(&deff_int,&(info_no[i].def_ref));

    fprintf(des_pu,"%d) N* %d\n",n_des_pu, i); /* imprime descritor ate' o no' i */

    fprintf(des_pudu,"%d) N* %d\n",n_des_pu, i); /* imprime descritor ate' o no' i */

    /* escreve Nnv nos arquivos de descritores */

    fprintf(des_pu,"Nnv = ");
    fprintf(des_pudu,"Nnvlf = ");

    for(no = 1; no <= no_nos; ++no)
       if(test_bit(no,&Nnv))
         {
          fprintf(des_pu,"%d ",no);
          fprintf(des_pudu,"%d ",no);
         }
    fprintf(des_pu,"\n\n");
    fprintf(des_pudu,"\n\n");

/* escreve as associacoes no arquivo que as descreve */

   fprintf(pu_assoc,"%d) <%d,( , ),{",n_des_pu,i);

   vir = FALSE;

   for(j=0; j>=0 && j < no_vars; ++j)
      {
       if(test_bit(j,&(deff_int)))
         {
          if(vir)
             fprintf(pu_assoc,",");
	  show_description(j,pu_assoc);
          vir = TRUE;
         }
      }

    fprintf(pu_assoc," }>\n");
   }

/* libera o espaco ocupado por conodef */

while(ret_elem(&conodef) > 0);

b_vector_destroy(&Nnv);
b_vector_destroy(&deff_int);

/* libera o espaco ocupado pela lista de arcos primitivos */

lib_prim_list(des_info->prim_arcs);
}

/*******************************************************************/
/* void lib_prim_list(ARCPRIM *)                                   */
/*                                                                 */
/* Autor: Marcos L. Chaim                                          */
/* Data: 14/09/90                                                  */
/*                                                                 */
/* Funcao: liberar espaco ocupa pela lista de arcos primitivos.    */
/*                                                                 */
/* Entradas: apontador para a lista de  arcos                      */
/*                                                                 */
/* Saida: nenhuma.                                                 */
/*                                                                 */
/*******************************************************************/

void lib_prim_list(prim_list)
ARCPRIM * prim_list;
{

/* Declaracao de Variaveis Locais */

ARCPRIM * prim1_aux, * prim2_aux;
LISTBITVECTOR * bit1_aux, * bit2_aux;

for(prim1_aux = prim_list; prim1_aux != (ARCPRIM *) NULL;
     prim1_aux = prim2_aux)
     {
      for(bit1_aux = prim1_aux->deff_ptr; bit1_aux != (LISTBITVECTOR *) NULL;
         bit1_aux = bit2_aux)
         {
          bit2_aux = bit1_aux->next;
          b_vector_destroy(&bit1_aux->conj);
          free(bit1_aux);
         }
      prim2_aux = prim1_aux->next;
      free(prim1_aux);
     }
}









