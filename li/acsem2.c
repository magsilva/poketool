/*
// acsem2.c 	1.1		94/12/26	11:25:48
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
#include "tabelasli.h"
#include "hli.h"

/**********************************************************************/
/* int an()                                                           */
/* Autor: Marcos L. Chaim                                             */
/* Versao: 1.1                                                        */
/* Data: 18/09/89                                                     */
/*                                                                    */
/* Funcao: realizar as acoes semanticas do AL.                        */
/*                                                                    */
/* Entradas: nenhuma                                                  */
/*                                                                    */
/* Saida:    nenhuma                                                  */
/*                                                                    */
/**********************************************************************/

/* Declaracao de Variaveis Globais */

extern struct tableout saida;
extern char ch;
extern int tamanho;

extern int linha;                  /* */
extern long offset;                /* */
extern struct no_tipdef *raiz;     /* */
extern int flag_typedef;           /* */
extern int flag_dirpre;            /* */

void a1()
{
 tamanho = 0;
 saida.inicio = offset;
 saida.linha  = linha;
 saida.label[tamanho] = ch;
 saida.label[++tamanho]='\0';
}

void a2()
{
 if(tamanho < TAMLEX)
  {
   saida.label[tamanho]=ch;
   ++tamanho;
   saida.label[tamanho]='\0';
  }
 else
  ++tamanho;
}

void a3()
{
 int  no_match=1;
 char vetor[TAMLEX];

 strcpy(vetor,saida.label);
 if (flag_dirpre &&
    (strcmp(vetor,"include") == 0 || strcmp(vetor,"define") == 0 ||
     strcmp(vetor,"undef")   == 0 || strcmp(vetor,"error")  == 0 ||
     strcmp(vetor,"pragma")  == 0 || strcmp(vetor,"line")   == 0 ||
     strcmp(vetor,"ifdef")   == 0 || strcmp(vetor,"ifndef") == 0 ||
     strcmp(vetor,"elif")    == 0 || strcmp(vetor,"endif")  == 0))
   strcpy(saida.classe,saida.label);
 else
  if (pesq_tab(vetor) != 0)
     strcpy(saida.classe,saida.label);
  else
   {
    tree_tipo(raiz,vetor,0,&no_match);
    if (!no_match)
       strcpy(saida.classe,"TIPDEF");
    else strcpy(saida.classe,"IDENT");
   }
 flag_dirpre = 0;
 saida.comp = tamanho;
 dev_ch();
 calcula_posicao();
}

void a4()
{
}

void a5()
{
 if (tamanho < TAMLEX)
    {
    saida.label[tamanho]=ch;
    ++tamanho;
    saida.label[tamanho]='\0';
    }
 else
    ++tamanho;
 saida.comp=tamanho;
 strcpy(saida.classe,"CONSTANT");
 calcula_posicao();
}

void a6()
{
 strcpy(saida.classe,"CONSTANT");
 saida.comp=tamanho;
 dev_ch();
 calcula_posicao();
}

void a7()
{
 if (tamanho < TAMLEX)
    {
    saida.label[tamanho]=ch;
    ++tamanho;
    saida.label[tamanho]='\0';
    }
 else
    ++tamanho;
 strcpy(saida.classe,saida.label);
 saida.comp=tamanho;
 calcula_posicao();
}

void a8()
{
 strcpy(saida.classe,saida.label);
 saida.comp=tamanho;
 dev_ch();
 calcula_posicao();
}
void a9()
{
 saida.label[0]=ch;
 saida.label[1]='\0';
 strcpy(saida.classe,saida.label);
 saida.inicio=offset;
 saida.linha=linha;
 saida.comp=1;
 calcula_posicao();
}

void a10()
{
 strcpy(saida.classe,"SEQCOD");
 saida.comp=tamanho;
 dev_ch();
 calcula_posicao();
}

void a11()
{
 strcpy(saida.classe,"SEQCOD");
 saida.comp=tamanho-1;
 dev_ch();
 ch='/';
 dev_ch();
 calcula_posicao();
}

/**********************************************************************/
/* char *lowstring(char *)                                            */
/* Autor: Marcos L. Chaim                                             */
/* Versao: 1.1                                                        */
/* Data: 18/09/89                                                     */
/*                                                                    */
/* Funcao: transforma um string contendo maiusculas e minusculas em   */
/*         um vetor contendo somente minusculas.                      */
/*                                                                    */
/* Entradas: apontador para o vetor contendo o string.                */
/*                                                                    */
/* Saida:    apontador para o vetor contendo o string.                */
/*                                                                    */
/* Funcoes Chamadas: tolower().                                       */
/*                                                                    */
/**********************************************************************/

char * lowstring(string)
char * string;
{
  int apontador;
  apontador=0;
  while(*(string+apontador)!='\0')
    {
     if(*(string+apontador) >= 'A' && *(string+apontador)<='Z')
     *(string+apontador)=tolower(*(string+apontador));
     ++apontador;
    }
 return(string);
}

