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

/********************************************************************/
/* void ins_arc(ARCPRIM **, PAIR, b_vector *)                       */
/* Autor: Marcos L. Chaim                                           */
/* Data: 13/09/90                                                   */
/* Versao: 1.0                                                      */
/*                                                                  */
/* Funcao: insere um arco primitivo na lista de arcos primitivos e  */
/*         seus deff's, se necessario.                              */
/*                                                                  */
/* Entradas: apontador para a lista de arcos primitivos, apontador  */
/*           para o arco primitivo e apontador para o deff.         */
/*                                                                  */
/* Saida: nenhuma.                                                  */
/*                                                                  */
/********************************************************************/

void ins_arc(list_prim, prim, deff)
ARCPRIM * * list_prim;
PAIRINT * prim;
b_vector * deff;
{

/* Declaracao de Variaveis Locais */

ARCPRIM * list_aux;
int flag = FALSE;

for(list_aux = *list_prim;list_aux != (ARCPRIM *) NULL; list_aux = list_aux->next)
   {
    if((list_aux->arco).abs == prim->abs && (list_aux->arco).coor == prim->coor)
      {
       flag = TRUE;
       if(!deff_e_igual(list_aux->deff_ptr, deff))
           ins_deff(&list_aux->deff_ptr,deff);
      }
    }

if(!flag)
  {
   list_aux = (ARCPRIM *) malloc(sizeof(ARCPRIM));
   if(list_aux == (ARCPRIM *) NULL)
       error("* * Erro Fatal: Nao consegui alocar espaco para a lista de arcos primitivos * *");
   list_aux->arco = *prim;
   list_aux->deff_ptr = (LISTBITVECTOR *) NULL;
   ins_deff(&list_aux->deff_ptr,deff);
   list_aux->next = *list_prim;
   *list_prim = list_aux;
 }
}
/********************************************************************/
/* int deff_e_igual(LISTBITVECTOR *, b_vector *)                    */
/* Autor: Marcos L. Chaim                                           */
/* Data: 13/09/90                                                   */
/* Versao: 1.0                                                      */
/*                                                                  */
/* Funcao: verifica na lista de deff's de um arco se existe algum   */
/*         deff igual `aquele que foi passado como parametro.       */
/*                                                                  */
/* Entradas: apontador para a lista de conjuntos de deff e apontador*/
/*           o deff.                                                */
/*                                                                  */
/* Saida: TRUE caso exista algum igual e FALSE caso contrario.      */
/*                                                                  */
/********************************************************************/

int deff_e_igual(list_deff, deff)
LISTBITVECTOR * list_deff;
b_vector * deff;
{

 /* Declaracao de Variaveis Locais */

 LISTBITVECTOR * list_aux;
 int retorno = FALSE;

 for(list_aux = list_deff; list_aux != (LISTBITVECTOR *) NULL;
     list_aux = list_aux->next)
     {
      if(is_equal_b_vector(&(list_aux->conj),deff))
        {
         retorno = TRUE;
         break;
        }
     }
 return(retorno);
}

/********************************************************************/
/* void ins_deff(LISTBITVECTOR * *, b_vector *)                    */
/* Autor: Marcos L. Chaim                                           */
/* Data: 13/09/90                                                   */
/* Versao: 1.0                                                      */
/*                                                                  */
/* Funcao: insere um deff na lista de deff's.                       */
/*                                                                  */
/* Entradas: apontador para o apontador da lista de conjuntos de    */
/*           deff e apontador para o deff.                          */
/*                                                                  */
/* Saida: nenhuma.                                                  */
/*                                                                  */
/********************************************************************/

void ins_deff(list_deff, deff)
LISTBITVECTOR * * list_deff;
b_vector * deff;
{

 /* Declaracao de Variaveis Locais */

 LISTBITVECTOR * list_aux;

 list_aux = (LISTBITVECTOR *) malloc(sizeof(LISTBITVECTOR));
 if(list_aux == (LISTBITVECTOR *) NULL)
   error("* * Erro Fatal: Nao consegui alocar espaco para a lista de deff's * *");

 /* list_aux->conj = *deff; */
 
 b_vector_cons(&(list_aux->conj),deff->nbytes,deff->type_in);
 b_vector_cpy(&(list_aux->conj),deff);  
 
 
 list_aux->next = *list_deff;
 *list_deff = list_aux;
}
















