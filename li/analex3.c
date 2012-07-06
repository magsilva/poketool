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
// analex3.c 	1.1		94/12/26	11:27:01
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
#include "headerli.h"
#include "tabelasli.h"
#include "hparserg.h"

/* Declaracao de Variaveis Globais */

extern struct table *tab_trans[];/* apontador para a tabela de transicao */
extern FILE * arqfonte;       /* contem ponteiro para arquivo fonte */
extern char ch;               /* ultimo caracter lido */
extern struct tableout saida;        /* ultimo token lido */

extern int linha;          /* linha do caracter lido */
extern long offset;        /* posicao do caracter lido */

/*******************************************************************/
/* int yylex()                                                     */
/* Autor: Marcos L. Chaim                                          */
/* Versao: 2.0                                                     */
/* Data: 18/09/89                                                  */
/*                                                                 */
/* Funcao: e' um analisador lexico generico dirigido por tabelas   */
/*         e retorna um inteiro identificando um token.            */
/*                                                                 */
/* Entradas: nenhuma                                               */
/*                                                                 */
/* Saida:    inteiro identificandor do tipo do token               */
/*                                                                 */
/* Funcoes Chamadas: compara_simbolo(), peg_ch(), a1(),...e a8().  */
/*                                                                 */
/* Variaveis Externas: comprimento, arqfonte, ch, tab_trans[].     */
/*                                                                 */
/*******************************************************************/

void yylex()
{

/* Declaracao de Variaveis Locais */

int l,s;               /* Variaveis de controle do AF */

/* Inicializacao de Variaveis Locais */

l=0;s=0;

saida.classe[0]=EOF;
saida.classe[1]='\0';

/* AUTOMATO FINITO (AF) */
peg_ch();
while(ch != EOF && s!= FINAL_STATE)
   {
    if(compara_simbolo(ch,tab_trans[l])==TRUE)
       {
          s=tab_trans[l]->proximo_estado;
          switch(tab_trans[l]->acao_semantica)
            {
	     case 1: a1();break;
	     case 2: a2();break;
	     case 3: a3();break;
	     case 4: a4();break;
	     case 5: a5();break;
	     case 6: a6();break;
	     case 7: a7();break;
	     case 8: a8();break;
	     case 9: a9();break;
	     case 10: a10();break;
	     case 11: a11();break;
	     default:;
	    }
	  if(s!=FINAL_STATE) {
		  peg_ch();
		  l=tab_trans[l]->proxima_transicao;
		  }
        }
        else
            {
             ++l;
             if(s==tab_trans[l]->estado_atual) continue;
             else
		  error("* * Erro Fatal: transicao errada no ANALISADOR LEXICO * *\n");
            }
    }
return;
}

/**********************************************************************/
/* void peg_ch()                                                      */
/* Autor: Marcos L. Chaim                                             */
/* Versao: 1.0                                                        */
/* Data: 18/09/89                                                     */
/*                                                                    */
/* Funcao: pega o proximo caracter a ser analisado pelo AL.           */
/*                                                                    */
/* Entradas: nenhuma                                                  */
/*                                                                    */
/* Saida: nenhuma                                                     */
/*                                                                    */
/* Variaveis Externas: ch, comprimemto, arqfonte.                     */
/*                                                                    */
/**********************************************************************/

void peg_ch()
{
 ch = getc(arqfonte);

 if (ch == '\n')         /* */
    ++linha;             /* */
 ++offset;               /* */
}

/**********************************************************************/
/* void dev_ch()                                                      */
/* Autor: Marcos L. Chaim                                             */
/* Versao: 1.0                                                        */
/* Data: 18/09/89                                                     */
/*                                                                    */
/* Funcao: devolve o ultimo caracter analisado pelo AL.               */
/*                                                                    */
/* Entradas: nenhuma                                                  */
/*                                                                    */
/* Saida: nenhuma                                                     */
/*                                                                    */
/* Variaveis Externas: comprimemto, arqfonte, ch.                     */
/*                                                                    */
/**********************************************************************/

void dev_ch()
{
 --offset;               /* */
 if (ch == '\n')         /* */
    --linha;             /* */
 ungetc(ch,arqfonte);
}
