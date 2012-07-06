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
// carrega2.c 	1.1		94/12/26	11:27:02
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
#include "hparserg.h"
#include "hrotsem.h"
#include "headerli.h"
#include "tabelasli.h"
#include "hli.h"

/* NLEX = 100 (numero maximo de elementos da tab. de trans.) */
/* NCHAVE = 100 (numero maximo de palavras chaves) */

/* Declaracao de Variaveis Globais */

extern struct table *tab_trans[NLEX]; /* armazena a tabela de transicao */
extern struct chave *tab_chave[NCHAVE]; /* armazena tabela de palavras chaves */
extern int n_chaves; /* armazena o numero de chaves de uma linguagem */
extern int ntablex;

extern char * lipath; /* caminho das tabelas */

/*******************************************************************/
/* void carregador(char *)                                         */
/* Autor: Mauro Carnassale                                         */
/* Versao: 1.1 modificada por Marcos L. Chaim em 20/09/89          */
/* Data: 1988                                                      */
/*                                                                 */
/* Funcao: carrega as tabela de transicao do analisador lexico (AL)*/
/*         do parser especifico da linguagem fonte.                */
/*                                                                 */
/* Entrada: apontador para um string que contem a abreviatura da   */
/*          linguagem do codigo fonte.                             */
/*                                                                 */
/* Saida:  nenhuma.                                                */
/*                                                                 */
/* Variaveis Globais: tab_trans[], tab_chave[], n_chaves, ntablex. */
/*                                                                 */
/*******************************************************************/


void carregador(nomling)
char *nomling;
{

 /* Declaracao de Variaveis Locais */

 char linha[200];
 char *pl;
 char nomtabs[500];
 FILE *fp;
 int i,j;
 char aux[200];

                       /* carregando tabela de transicao */

/* 
  msg_print("                                                    \r");
  msg_print("* * Carregando Tabela de Transicao Lexica... * *\r");
*/ 

  strcpy(nomtabs,lipath);

#ifdef TURBO_C
  strcat(nomtabs,"\\tabtrans.");
#else
  strcat(nomtabs,"/tabtrans.");
#endif

  strcat(nomtabs,nomling);

  if ((fp = fopen(nomtabs,"r")) == 0)
	error("* * Erro Fatal: Nao encontrei tabela de transicoes lexicas * *");
   
  for(i=0;((pl=fgets(linha,201,fp))!=NULL);++i)
    {
    pl++;
/*  for(j=1;((linha[j] != '#') && (linha[j] != '\0'));++j);*/  /* despreza comentario */
/*  linha[j] = '\0'; */
    tab_trans[i]= (struct table *) malloc(sizeof(struct table));
    if(tab_trans[i] == NULL)
      error("* * Erro Fatal: Nao consegui alocar memoria no carregador lexico");
    for(j=0;(*pl != '\"');++j)
      {
      if (*pl == '\\')
        {
        switch (*(pl+1))
          {
          case 'n'  : aux[j] = '\n';
                      break;

	 case 'r'  : aux[j] = '\r';
                      break;

	 case 'f'  : aux[j] = '\f';
                      break;

          case 't'  : aux[j] = '\t';
                      break;

          case '\\' : aux[j] = '\\';
                      break;

          case '\"'  : aux[j] = '\"';
                      break;

	  case '\''  : aux[j] = '\'';
		      break;

          case '0'  : aux[j] = '\0';
                      break;

	  case 'b'  : aux[j] = '\b';
                      break;
          }
        pl += 2;
        }
      else
        aux[j] = *(pl++);
      }
    aux[j] = '\0';
    strcpy(tab_trans[i]->simbolo_lido,aux);
    pl += 2;
    sscanf(pl,"%d %d %d %d ",&tab_trans[i]->estado_atual,
                                &tab_trans[i]->proximo_estado,
                                &tab_trans[i]->acao_semantica,
                                &tab_trans[i]->proxima_transicao);
    ntablex = i;
    }
  fclose(fp);



                  /* carregando tabela de palavras chaves */

/*
  msg_print("                                                        \r");
  msg_print("* * Carregando Tabela de Palavras Chaves... * *\r");
*/
  strcpy(nomtabs,lipath);

#ifdef TURBO_C
  strcat(nomtabs,"\\tabchave.");
#else
  strcat(nomtabs,"/tabchave.");
#endif

  strcat(nomtabs,nomling);

  if ((fp = fopen(nomtabs,"r")) == 0)
	 error("* * Erro Fatal: Nao encontrei tabela de palavras chaves * *");

  n_chaves = 0;
  for(i=0;fgets(linha,201,fp)!=NULL;++i)
    {
    for(j=1;((linha[j] != '#') && (linha[j] != '\0'));++j);  /* despreza comentario */
    linha[j] = '\0';
    tab_chave[i]= (struct chave *) malloc(sizeof(struct chave));
    if(tab_chave[i] == NULL)
      error("* * Erro Fatal: Nao consegui alocar memoria no carregador lexico");
    sscanf(linha,"%s %d",tab_chave[i]->pal_chave,
			 &tab_chave[i]->referencia);
    n_chaves++;     /* complementa numero de chaves para uma linguagem */
    }
  fclose(fp);

}
