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
extern int n_des_pdu;

/*******************************************************************/
/* void des_pdu_gen(INFODESCRITORES *)                             */
/* Autor: Marcos L. Chaim                                          */
/* Data: 10/01/90                                                  */
/*                                                                 */
/* Funcao: gerar os descritores para o criterio todos potenciais   */
/*         du-caminhos.                                            */
/*                                                                 */
/* Entradas: apontador para as informacoes de descritores.         */
/*                                                                 */
/* Saida: nenhuma.                                                 */
/*                                                                 */
/*******************************************************************/
void des_pdu_gen(des_info,g_suc, descritores,pdu_paths)
INFODESCRITORES * des_info;
struct grafo * g_suc;
FILE * descritores;
FILE * pdu_paths;
{

 /* Declaracao de Variaveis Locais */
 NOGRAFOI * no_grfi;
 SUCGRAFOI * suc_aux;
 STACKARC des_stack;
 ARCINFO aux;
 int i;

 mk_nl_arc(&des_stack); /* inicia a pilha de arcos */

 no_grfi = des_info->grafo_i;


 if(no_grfi->sucgfi == (SUCGRAFOI *) NULL) /* grafo(i) de um unico no'. */
	{
	 /* imprime descritor e caminho */

	 ++n_des_pdu; /* atualiza contador de descritores */
	 i = (no_grfi->infosuc).num_no_G; /* no' i */

	 fprintf(descritores,"%2d) ",n_des_pdu); /* escreve o numero do descritor */
	 fprintf(descritores,"%s %d \n","N*",i);

	 fprintf(pdu_paths,"%2d) ",n_des_pdu); /* escreve o numero do caminho */
	 fprintf(pdu_paths,"%d \n",i);

	 return;
	}

 for(suc_aux = no_grfi->sucgfi ; suc_aux != (SUCGRAFOI *) NULL;
     suc_aux = suc_aux->next)
      {
       /* coloca arco  no registro de informacoes do arco */

       aux.arco.abs = (no_grfi->infosuc).num_no_G;
       aux.arco.coor = (suc_aux->no_address)->infosuc.num_no_G;

       if(suc_aux->tipo == PFI)
         aux.tipo = PFI; /* e' um arco primitivo pfi */
       else
         if(e_primitivo(aux.arco.abs,aux.arco.coor,g_suc))
               aux.tipo = PRIMITIVO;
            else
               aux.tipo = HERDEIRO;

       /* anula o conteudo de des_meio */

       aux.des_meio[0] = '\0';

       /* empilha o conteudo de informacao do arco */

       push_arc(&aux,&des_stack);

       /* des_pu_gen(...); determina o descritor desse arco */

       /* continua a pesquisa no grafo(i) */

       pes_grfi(suc_aux->no_address, des_info, &des_stack,g_suc,descritores,pdu_paths);
   }
}

/*******************************************************************/
/* void pes_grfi(NOGRAFOI *,INFODESCRITORES *, STACKARC *)         */
/* Autor: Marcos L. Chaim                                          */
/* Data: 10/01/90                                                  */
/*                                                                 */
/* Funcao: faz a pesquisa em profundidade para gerar os descritores*/
/*         para o criterio todos potenciais du-caminhos.           */
/*                                                                 */
/* Entradas: apontador para o no' do grafo(i) onde continua a pes- */
/*           quisa apontador para as informacoes de descritores e  */
/*           apontador para pilha de arcos.                        */
/* Saida: nenhuma.                                                 */
/*                                                                 */
/*******************************************************************/

void pes_grfi(no_pes,des_info, des_stack, g_suc, descritores,pdu_paths)
NOGRAFOI * no_pes;
INFODESCRITORES * des_info;
STACKARC * des_stack;
struct grafo * g_suc;
FILE * descritores;
FILE * pdu_paths;
{

 /* Declaracao de Variaveis Locais */

 SUCGRAFOI * suc_aux;
 ARCINFO aux;

 for(suc_aux = no_pes->sucgfi; suc_aux != (SUCGRAFOI *) NULL;
     suc_aux = suc_aux->next)
      {
       /* coloca arco  no registro de informacoes do arco */

       aux.arco.abs = (no_pes->infosuc).num_no_G;
       aux.arco.coor = (suc_aux->no_address)->infosuc.num_no_G;

       if(suc_aux->tipo == PFI)
         aux.tipo = PFI; /* e' um arco primitivo pfi */
       else
         if(e_primitivo(aux.arco.abs,aux.arco.coor,g_suc))
               aux.tipo = PRIMITIVO;
         else
               aux.tipo = HERDEIRO;

       /* ajusta o conteudo de des_meio */


       if(aux.tipo == HERDEIRO)
         strcpy(aux.des_meio,"Nlf*"); /* copia "Nlf" em aux.des_meio */
       else /* arco e' primitivo */
         aux.des_meio[0] = '\0';

       /* empilha os conteudos de informacao dos arcos */

       push_arc(&aux, des_stack);

       /* des_pu_gen(...); determina o descritor desse arco */

       /* continua a pesquisa no grafo(i) */

       pes_grfi(suc_aux->no_address, des_info, des_stack, g_suc, descritores, pdu_paths);
   }
if(no_pes->sucgfi == (SUCGRAFOI *) NULL) /* fim da pesquisa */
 { /* imprime descritor e caminho */
  imprime_descritor(des_stack,descritores,des_info);
  imprime_caminho(des_stack,pdu_paths, n_des_pdu);
 }
 pop_arc(des_stack);
}

/********************************************************************/
/* void imprime_descritor(STACKARC *, int)                          */
/* Autor: Marcos L. Chaim                                           */
/* Data: 10/1/90                                                    */
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

void imprime_descritor(stack,descritores,des_info)
STACKARC * stack;
FILE * descritores;
INFODESCRITORES * des_info;
{

 /* Declaracao de Variaveis Locais */

 STACKARC stack_aux;

 ARCINFO aux, an_aux;

 int i, no_her;

 i =(des_info->grafo_i)->infosuc.num_no_G; /* no' i */
 ++n_des_pdu; /* atualiza contador de descritores */
 fprintf(descritores,"%2d) ",n_des_pdu); /* escreve o numero do descritor */

 mk_nl_arc(&stack_aux);  /* inicia pilha auxiliar */

 /* empilha numa pilha auxiliar */

 while(!empty_arc(stack))
   {
    aux = pop_arc(stack);
    push_arc(&aux,&stack_aux);
   }

/* trata o primeiro arco da pilha de arcos */

aux = pop_arc(&stack_aux);

push_arc(&aux,stack);

no_her = 0; /* inicia o contador de arcos herdeiros */

if(aux.tipo != HERDEIRO)
  { /* arco e' primitivo */
    if(aux.arco.abs == i)
       /* no' i faz parte do primeiro arco */
       fprintf(descritores,"%s ","N*");
    else
       fprintf(descritores,"%s %d ","N*",i);

    fprintf(descritores,"%d %d ",aux.arco.abs, aux.arco.coor);
   }
else
   {
    fprintf(descritores,"%s %d ","N*",i);
    no_her++; /* atualiza contador de arcos herdeiros */
   }

/* imprime o descritor */

while(!empty_arc(&stack_aux))
   {
    aux = pop_arc(&stack_aux);
    an_aux = top_arc(stack);
    if(aux.tipo != HERDEIRO)
     { /* o arco e' primitivo */
      if(an_aux.tipo != HERDEIRO)  /* arco anterior e' primitivo */
        {
         if(an_aux.arco.coor == aux.arco.abs)
           fprintf(descritores,"%d ", aux.arco.coor); /* so' imprime no' alvo */
         else
           fprintf(descritores,"%d %d ", aux.arco.abs, aux.arco.coor); /* imprime o arco */
        }
      else /* arco anterior e' herdeiro */
        {
         if(no_her>1)
           fprintf(descritores,"Nlf* %d %d ", aux.arco.abs, aux.arco.coor);
         else
           fprintf(descritores,"%d %d ", aux.arco.abs, aux.arco.coor); /* imprime o arco */
         no_her = 0; /* atualiza contador de arcos herdeiros */
        }
      }
     else
         no_her++; /* atualiza contador de arcos herdeiros */

    push_arc(&aux,stack);
   }
 fprintf(descritores,"\n");
}



/********************************************************************/
/* void imprime_caminho(STACKARC *, FILE *)                         */
/* Autor: Marcos L. Chaim                                           */
/* Data: 12/01/90                                                   */
/* Versao: 1.0                                                      */
/*                                                                  */
/* Funcao: esta funcao imprime o caminho dos arcos de uma pilha de  */
/*         arcos num arquivo cujo descritor e' passado como         */
/*         parametro.                                               */
/*                                                                  */
/* Entradas: apontador para a pilha de arcos e apontador para o     */
/*           arquivo.                                               */
/*                                                                  */
/* Saida: nenhuma.                                                  */
/*                                                                  */
/********************************************************************/

void imprime_caminho(stack, archive, n_path)
STACKARC * stack;
FILE * archive;
int n_path;
{

 /* Declaracao de Variaveis Locais */

 STACKARC stack_aux;

 ARCINFO aux;

 fprintf(archive,"%2d) ",n_path); /* escreve o numero do caminho */

 mk_nl_arc(&stack_aux);  /* inicia pilha auxiliar */

 while(!empty_arc(stack))
  { aux = pop_arc(stack);
    push_arc(&aux,&stack_aux);
  }

while(!empty_arc(&stack_aux))
   {
    aux = pop_arc(&stack_aux);
    push_arc(&aux,stack);
    fprintf(archive,"%d ",aux.arco.abs);
   }
 fprintf(archive,"%d ",aux.arco.coor);
 fprintf(archive,"\n");
}
