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
// pesqtab2.c 	1.1		94/12/26	11:27:06
*/

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#ifndef TURBO_C
#include <malloc.h>
#else
#include <alloc.h>
#endif

#include <stdlib.h>

#include "util.h"
#include "tabelasli.h"

 /* Declaracao de Variaveis Globais */

 extern struct chave *tab_chave[];
 extern int n_chaves;

/**********************************************************************/
/* int pesq_tab(char *, int)                                          */
/* Autor: Marcos L. Chaim                                             */
/* Versao: 1.1                                                        */
/* Data: 19/09/89                                                     */
/*                                                                    */
/* Entradas: apontador para a palavra a ser pesquisada e o numero de  */
/*           elementos da tabela de palavras reservadas.              */
/*                                                                    */
/* Saida:    inteiro indicando verdadeiro ou falso.                   */
/*                                                                    */
/* Variaveis Globais: tab_chave,n_chave;                              */
/*                                                                    */
/**********************************************************************/

int pesq_tab(pal)
char *pal;
{

 /* Declaracao de Variaveis Globais */

 extern struct chave *tab_chave[];
 extern int n_chaves;

 /* Declaracao de Variaveis Locais */

 int inicio,fim,meio,cond;
 inicio=0;
 fim=n_chaves-1;
 while(inicio<=fim)
     {
      meio=(inicio+fim)/2;
      if((cond=strcmp(pal,tab_chave[meio]->pal_chave))<0)
      fim=meio-1;
      else if(cond>0)
           inicio=meio+1;
           else
           return(tab_chave[meio]->referencia);
     }
 return(0);
}


