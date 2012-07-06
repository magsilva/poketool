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

/*
// funcoes.c 	1.1		94/12/29	10:34:03
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"
#include "stack.h"
#include "chanomat.h"

/* Declaracao de Variaveis Globais */

extern char mat[MATRIXSIZE][MATRIXSIZE];
extern int no_vazio;
extern int num_no;

/*
   car_int- calcula a cardinalidade de uma pilha
*/

int car_int(p_stack)
STACK * p_stack;
{

/* Declaracao de Variaveis Locais */

STACK aux;
int aux_var;
int card;

mk_nl_int(&aux);
card = 0;

while (!empty_int(p_stack))
 {
  aux_var = pop_int(p_stack);
  push_int(aux_var,&aux);
  ++card;
 }

while (!empty_int(&aux))
 {
  aux_var = pop_int(&aux);
  push_int(aux_var,p_stack);
 }

return(card);
}

/*
  liga_pair - liga os elementos de uma pilha de pares de inteiros com um no e
              esvazia a pilha
*/

void liga_pair(p_stack, no)
STACKPAIR * p_stack;
int no;
{
PAIRINT aux_var;


while (!empty_pair(p_stack))
 {
  aux_var = pop_pair(p_stack);
  if(aux_var.abs > MATRIXSIZE)
     error("Erro Fatal: numero excessivo de nos!\n");
  mat[aux_var.abs][no] = '1';
 }

}

void liga_int(p_stack, no)
STACK * p_stack;
int no;
{
int aux_var;


while (!empty_int(p_stack))
 {
  aux_var = pop_int(p_stack);
  if(aux_var > MATRIXSIZE)
     error("Erro Fatal: numero excessivo de nos!\n");
  mat[aux_var][no] = '1';
 }

}

void liga_goto(l_goto,l_label)
GOTOLABEL * l_goto;
GOTOLABEL * l_label;
{

 GOTOLABEL * aux_goto, * aux_label;

 for(aux_goto = l_goto; aux_goto != (GOTOLABEL *) NULL; aux_goto = aux_goto->next)
   for(aux_label = l_label; aux_label != (GOTOLABEL *) NULL; aux_label = aux_label->next)
    if(!strcmp(aux_goto->name,aux_label->name))
       {
        mat[aux_goto->id][aux_label->id] = '1';
        break;
       }

}

/*
 cat_pair -  concatena duas pilhas de do tipo PAIRINT
*/

void cat_pair(p_stack, s_stack)
STACKPAIR * p_stack;
STACKPAIR * s_stack;
{
PAIRINT aux_var;

while (!empty_pair(s_stack))
 {
  aux_var = pop_pair(s_stack);
  push_pair(&aux_var,p_stack);
 }

}

void cat_mix(p_stack, s_stack)
STACKPAIR * p_stack;
STACK * s_stack;
{
PAIRINT aux_var;

while (!empty_int(s_stack))
 {
  aux_var.abs = pop_int(s_stack);
  aux_var.coor = TRUE;

  push_pair(&aux_var,p_stack);
 }

}

void cat_int(p_stack, s_stack)
STACK * p_stack;
STACK * s_stack;
{
 int aux_var;

while (!empty_int(s_stack))
 {
  aux_var = pop_int(s_stack);
  push_int(aux_var,p_stack);
 }

}

void atualiza_chave(arq, pos_atual, pos_marcada, no)
FILE * arq;
long int pos_atual;
long int pos_marcada;
int no;
{
 struct symbol newsymbol;
 struct symbol * pnewsymbol;
 pnewsymbol = &newsymbol;

 fseek (arq,pos_marcada,SEEK_SET);
 fscanf(arq,"%s %d %ld %ld %ld",
	pnewsymbol->simbolo,&pnewsymbol->no,
                            &pnewsymbol->inicio,
                            &pnewsymbol->comprimento,
                            &pnewsymbol->linha);

/*
 strcpy(pnewsymbol->simbolo,"Burro");
 pnewsymbol->comprimento = pnewsymbol->linha = pnewsymbol->inicio = 1000-pos_marcada;
 */
 pnewsymbol->no = no;

 fseek (arq,pos_marcada,SEEK_SET);

 fprintf(arq,"%-20s %6d %6ld %6ld %6ld\n",
	pnewsymbol->simbolo,pnewsymbol->no,
                            pnewsymbol->inicio,
                            pnewsymbol->comprimento,
                            pnewsymbol->linha);

 fseek (arq,pos_atual,SEEK_SET);
}

void atualiza_comprimento(arq, pos_atual, pos_marcada, comprimento)
FILE * arq;
long int pos_atual;
long int pos_marcada;
int comprimento;
{
 struct symbol newsymbol;
 struct symbol * pnewsymbol;
 pnewsymbol = &newsymbol;

 fseek (arq,pos_marcada,SEEK_SET);
 fscanf(arq,"%s %ld %ld %ld %ld",
	pnewsymbol->simbolo,&pnewsymbol->no,
                            &pnewsymbol->inicio,
                            &pnewsymbol->comprimento,
                            &pnewsymbol->linha);

 pnewsymbol->comprimento = comprimento;

 fseek (arq,pos_marcada,SEEK_SET);

 fprintf(arq,"%-20s %6ld %6ld %6ld %6ld\n",
	pnewsymbol->simbolo,pnewsymbol->no,
                            pnewsymbol->inicio,
                            pnewsymbol->comprimento,
                            pnewsymbol->linha);

 fseek (arq,pos_atual,SEEK_SET);
}


