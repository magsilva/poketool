/*
// leparser.c 	1.1		94/12/26	11:27:04
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef TURBO_C
#include <malloc.h>
#else
#include <alloc.h>
#endif


/*******************************************************************/
/* void pegsim()                                                   */
/* Autor: Marcos L. Chaim                                          */
/* Data: 28/09/89                                                  */
/* Versao: 1.1 (2/10/89)                                           */
/*                                                                 */
/* Funcao: le um simbolo do arquivo que contem a descricao do par- */
/*         ser especifico da linguagem fonte.                      */
/*                                                                 */
/* Entrada: nenhuma.                                               */
/*                                                                 */
/* Saida: nenhuma.                                                 */
/*                                                                 */
/* Variaveis Globais Utilizadas: sym e arparser.                   */
/*                                                                 */
/*******************************************************************/

#include "util.h"
#include "headerli.h"
#include "hparserg.h"
#include "tabelasli.h"

/* Declaracao de Variaveis Globais */

extern struct nter sym;
extern FILE * arqparser;
extern FILE * arqfonte;
extern struct cabeca *list;
extern struct cabeca *sentinel;
extern struct tableout saida;
extern long inicio, comprimento;

void pegsim()
{

 /* Declaracao de Variaveis Locais */

 int c;
 int k;
 int num;
 c = getc(arqparser);
 while(c == ' ' || c == '\n' || c == '\t' || c == '\f'
       || c == '\r' || c == '\b') c = getc(arqparser);
 if((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
    {
     k=0;
     while((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')
          || c == '_' || (c >= '0' && c<= '9'))
          {
	   if (k < CARMAX)
             {
              sym.nome[k] = c;
              sym.nome[++k] = '\0';
             }
           c = getc(arqparser);
          }
     ungetc(c, arqparser);
     sym.tipo = 'n';  /* nao terminal */
     sym.num = -10;
     return;
    }
 if(c >= '0' && c <= '9')     /* numero */
    {
     k=0;
     num = 0;
     sym.tipo = 'r';     /* numeral */
     while( c >= '0' && c <= '9' )
       {
	++k;
        num = 10*num + (c - '0');
        c = getc(arqparser);
       }
     ungetc(c,arqparser);
     sym.nome[0] = '\0';
     sym.num = num;
     if(k > NDIGMAX)
		 error("* * Erro Fatal: Numero da ac_sem do parser muito grande * *\r");
     return;
    }
if(c == '\'' )
    {
     c = getc(arqparser);
     k=0;
     while( c != '\'' || k == 0 )
       {
        if(c == EOF)
            error("* * Erro Fatal: Descricao do parser incorreta * * \r");
	if(k < CARMAX)
	  {
	   sym.nome[k] = c;
	   ++k;
	  }
	 c = getc(arqparser);
       }
     sym.nome[k] = '\0';
     sym.tipo = 't';       /* terminal */
     sym.num = -10;
/*     printf("nome = %s\n",sym.nome);
     for(k=0;k<10;++k)
	{
	 printf("%c\n",sym.nome[k]);
	} */
     return;
    }
if(c == EOF)
    {
     sym.nome[0] = '\0';
     sym.num = 0;
     sym.tipo = 'z';
     return;
    }
if( c != ',' && c != '.' && c != '=' && c != ']' && c != '[' && c != '*')
    error("* * Erro Fatal: Descricao do parser incorreta * * \r");
if(c == '*')                /* metalinguagem */
   sym.nome[0] = EMPTY;
else
   sym.nome[0] = c;
sym.nome[1] = '\0';
sym.tipo = 'm';
return;
}
