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


