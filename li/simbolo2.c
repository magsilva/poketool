/*
// simbolo2.c 	1.1		94/12/26	11:27:07
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef TURBO_C
#include <malloc.h>
#else
#include <alloc.h>
#endif

#include "util.h"
#include "tabelasli.h"

/**********************************************************************/
/* int compara_simbolo(char, struct table *)                          */
/* Autor: Marcos L. Chaim                                             */
/* Versao: 1.0                                                        */
/* Data: 02/88                                                        */
/*                                                                    */
/* Funcao: compara um caracter com a tabela de transicao.             */
/*                                                                    */
/* Entradas: o caracter a ser comparado e a linha da tabela de transi-*/
/*           cao.                                                     */
/*                                                                    */
/* Saida:    inteiro indicando verdadeiro ou falso.                   */
/*                                                                    */
/**********************************************************************/

int compara_simbolo(caracter,ptr)
char caracter;
struct table *ptr;

{
 char aux;
 int resposta;
 int retorno;
 int length;
 length=0;
 aux= *ptr->simbolo_lido;
 ++length;
 resposta=NEGATIVO;
 while(resposta!=POSITIVO)
    {
    if(*(ptr->simbolo_lido+length)=='\0') break;
     switch(*(ptr->simbolo_lido+length))
      {
       case 'l':if((caracter>='a' && caracter<='z')||(caracter>='A' && caracter<='Z'))
               {
                resposta=POSITIVO;
                break;
               }
                else break;
       case 'd':if(caracter>='0' && caracter<='9')
               {
                resposta=POSITIVO;
                break;
               }
                else break;
       case 'b':if(caracter==' ')
               {
                resposta=POSITIVO;
                break;
               }
                else break;
       default :if(caracter==*(ptr->simbolo_lido+length))
               {
                resposta=POSITIVO;
               }
      }
      ++length;
      }
if(aux=='c')
    {
     if(resposta==POSITIVO) retorno=FALSE;
     else retorno=TRUE;
    }
else
    {
     if(resposta==POSITIVO) retorno=TRUE;
     else retorno=FALSE;
    }
  return(retorno);
}
