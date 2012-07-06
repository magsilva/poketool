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
#include <strings.h>
#include <string.h>

#include "util.h"
#include "cte.h"
#include "newpokelib.h"
#include "haval.h"

void error(msg)
char msg[200];
{

 /* Declaracao de Variaveis Locais */

 FILE * avalog;

 /* escreve mensagem de erro */

 fprintf(stderr,"%s\n",msg);

 exit(1);

}

/*******************************************************************/
/* char * le_linha_str_gen(char **, char *, FILE *)                */
/* Autor: Marcos L. Chaim                                          */
/* Data: 10/12/99                                                  */
/* Versao: 1.0                                                     */
/*                                                                 */
/* Funcao: ler a primeira linha do arquivo fonte que contem o "st- */
/*         ring" passado como parametro.                           */
/*                                                                 */
/* Entradas: "string" que deve estar contido na linha do arquivo,  */
/*            ponteiro para arquivo                                */
/*                                                                 */
/* Saida: endereco da posicao que inicia o "string".               */
/*                                                                 */
/*******************************************************************/

char * le_linha_str_gen(string, file)
char * string;
FILE * file;
{

 /* Declaracao de Variaveis Locais */
 int i;
 char * ptr_linha;
 char linha_aux[MAXLINE-1];
 char * p_linha_aux=NULL,* p_linha_aux2=NULL;
 
 do
   {
    i = 1;

    if(p_linha_aux != NULL)
      {
      free(p_linha_aux);
      p_linha_aux=NULL;
      }
    do
      {     
      if(fgets(linha_aux,MAXLINE-1,file) == (char *) NULL)
         error("* * Erro Fatal: Arquivo de avaliacao imperfeito * *");
      
      if((strlen(linha_aux) == MAXLINE-2 && linha_aux[MAXLINE-2] != '\n')
         || i > 1)
        {         
        /* linha muito grande */

        if(strlen(linha_aux) + 1 == MAXLINE-1)
          {
          ++i;
        
          p_linha_aux2 = (char *) malloc(sizeof(char)* (i * (MAXLINE-1)));

          if(p_linha_aux2 == (char *) NULL)
             error("* * Erro Fatal: Falta de Memoria * *");
        
          if(i == 2)
            {
            /* primeiro incremento da linha */
            p_linha_aux = p_linha_aux2;  
            strcpy(p_linha_aux, linha_aux);         
            }
          else
            {
            /* nos demais incrementos da linha, concatena */
            strcpy(p_linha_aux2, p_linha_aux);
            free( p_linha_aux);
            p_linha_aux = p_linha_aux2;
            strcat(p_linha_aux, linha_aux);
            }
          }
        else
          { /* Tamanho da linha cabe em p_linha_aux */
          strcat(p_linha_aux, linha_aux);
          }
        }
      else
        {
        p_linha_aux=strdup(linha_aux);
        if(p_linha_aux == (char *) NULL)
          error("* * Erro Fatal: Falta de Memoria * *");
        }
        
      }
    while(p_linha_aux[strlen(p_linha_aux)-1] != '\n');
    
    ptr_linha = strstr(p_linha_aux,string);
   }
 while(ptr_linha == (char *) NULL);
 return(p_linha_aux);
}

/*******************************************************************/
/* char * le_linha_str(char *, char *, FILE *)                     */
/* Autor: Marcos L. Chaim                                          */
/* Data: 17/01/90                                                  */
/* Versao: 1.0                                                     */
/*                                                                 */
/* Funcao: ler a primeira linha do arquivo fonte que contem o "st- */
/*         ring" passado como parametro.                           */
/*                                                                 */
/* Entradas: vetor onde sera' armazenada o linha do arquivo, "str- */
/*           ing" que deve estar contido nesta linha.              */
/*                                                                 */
/* Saida: endereco da posicao que inicia o "string".               */
/*                                                                 */
/*******************************************************************/

char * le_linha_str(linha,string, file)
char * linha;
char * string;
FILE * file;
{

 /* Declaracao de Variaveis Locais */

 char * ptr_linha;

 do
   {
    if(fgets(linha,MAXLINE-1,file) == (char *) NULL)
       error("* * Erro Fatal: Arquivo de avaliacao imperfeito * *");
    ptr_linha = strstr(linha,string);
   }
 while(ptr_linha == (char *) NULL);
 return(ptr_linha);
}

/*******************************************************************/
/* void peg_tok(char *, char *)                                    */
/* Autor: Marcos L. Chaim                                          */
/* Data: 17/01/90                                                  */
/* Versao: 1.0                                                     */
/*                                                                 */
/* Funcao: coloca em um vetor o primeiro "token" encontrado no     */
/*         no "string" de caracteres contendo a expressao regular. */
/*                                                                 */
/* Entradas: apontador para o vetor e apontador para o "string"    */
/*           com a expressao regular.                              */
/*                                                                 */
/* Saida: nenhuma.                                                 */
/*                                                                 */
/*******************************************************************/

void peg_tok(array, string)
char * array;
char * string;
{

 /* Declaracao de Variaveis Locais */

 int i = 0;
 char c;

 c = string[i]; array[i] = '\0';
 while(c != '\0' && c != ' ' && c != '\n' && c != '\r' && c != '\f' &&
       c != '\b' && c != '\t' && i < 10)
       {
        array[i] = c;  /* enquanto for diferente de carcateres de controle e' literal */
        ++i;
        c = string[i];
       }
 array[i] = '\0';
}

/*******************************************************************/
/* char * apont_next_tok(char *)                                   */
/* Autor: Marcos L. Chaim                                          */
/* Data: 17/01/90                                                  */
/* Versao: 1.0                                                     */
/*                                                                 */
/* Funcao: faz o ponteiro de posicao corrente do vetor expressao   */
/*         regular apontar para o proximo "token" da expressao re- */
/*         gular.                                                  */
/*                                                                 */
/* Entradas: apontador para o "string" com a expressao regular.    */
/*                                                                 */
/* Saida: novo endereco corrente na expressao regular.             */
/*                                                                 */
/*******************************************************************/

char * apont_next_tok(string)
char * string;
{

 /* Declaracao de Variaveis Locais */

 int i = 0;
 char c;

 c = string[i];

/* pula "brancos" antes do "token" */

 while(c == ' ' || c == '\n' || c == '\r' || c == '\f' ||
       c == '\b' || c == '\t')
       {
        ++i;
        c = string[i];
       }

 /* pula "token" atual */

 while(c != '\0' && c != ' ' && c != '\n' && c != '\r' && c != '\f' &&
		 c != '\b' && c != '\t' && i < 10)
       {
        ++i;
        c = string[i];
       }

/* aponta para o inicio do proximo "token" */

 while(c == ' ' || c == '\n' || c == '\r' || c == '\f' ||
       c == '\b' || c == '\t')
       {
        ++i;
        c = string[i];
       }

return(string + i);

}
/*******************************************************************/
/* char * apont_prev_tok(char *)                                   */
/* Autor: Marcos L. Chaim                                          */
/* Data: 17/09/90                                                  */
/* Versao: 1.0                                                     */
/*                                                                 */
/* Funcao: faz o ponteiro de posicao corrente do vetor expressao   */
/*         regular apontar para o "token" anterior da expressao re-*/
/*         gular.                                                  */
/*                                                                 */
/* Entradas: apontador para o "string" com a expressao regular.    */
/*                                                                 */
/* Saida: novo endereco corrente na expressao regular.             */
/*                                                                 */
/*******************************************************************/

char * apont_prev_tok(string)
char * string;
{

 /* Declaracao de Variaveis Locais */

 char c;

/* pula espaco em branco atual */

 --string;
 c =*string;

 while(c == ' ' || c == '\n' || c == '\r' || c == '\f' ||
       c == '\b' || c == '\t')
      {
	--string;
	c = *string;
       }

/* aponta para o inicio do "token" anterior */

 while(c != '\0' && c != ' ' && c != '\n' && c != '\r' && c != '\f' &&
		 c != '\b' && c != '\t' )
       {
	--string;
	c = *string;
       }

++string;
return(string);
}

/*******************************************************************/
/* void peg_next_tok(char *, char *)                               */
/* Autor: Marcos L. Chaim                                          */
/* Data: 17/01/90                                                  */
/* Versao: 1.0                                                     */
/*                                                                 */
/* Funcao: coloca em um vetor o segundo "token" encontrado no      */
/*         no "string" de caracteres contendo a expressao regular. */
/*                                                                 */
/* Entradas: apontador para o vetor e apontador para o "string"    */
/*           com a expressao regular.                              */
/*                                                                 */
/* Saida: nenhuma.                                                 */
/*                                                                 */
/*******************************************************************/

void peg_next_tok(array, string)
char * array;
char * string;
{

 /* Declaracao de Variaveis Locais */

 char * aux_string;
 aux_string = string;
 aux_string = apont_next_tok(aux_string); /* aponta para o proximo "token" */
 peg_tok(array, aux_string); /* pega proximo "token" */
}

/*******************************************************************/
/* char * jump_tok(char *)                                         */
/* Autor: Marcos L. Chaim                                          */
/* Data: 17/01/90                                                  */
/* Versao: 1.0                                                     */
/*                                                                 */
/* Funcao: faz o ponteiro de posicao corrente do vetor expressao   */
/*         regular apontar para um "token" da expressao regular    */
/*         depois de "pular" os dois primeiros "tokens" apontados  */
/*         atualmente.                                             */
/*                                                                 */
/* Entradas: apontador para o "string" com a expressao regular.    */
/*                                                                 */
/* Saida: novo endereco corrente na expressao regular.             */
/*                                                                 */
/*******************************************************************/

char * jump_tok(string)
char * string;
{
 return(apont_next_tok(apont_next_tok(string)));
}

/*******************************************************************/
/* int e_numero(char *)                                            */
/* Autor: Marcos L. Chaim                                          */
/* Data: 17/01/90                                                  */
/* Versao: 1.0                                                     */
/*                                                                 */
/* Funcao: verifica se o conteudo do vetor e' contituido por um    */
/*         numero.                                                 */
/*                                                                 */
/* Entradas: apontador para o vetor.                               */
/*                                                                 */
/* Saida: inteiro indicando TRUE se for numero e FALSE , caso con- */
/*        contrario.                                               */
/*                                                                 */
/*******************************************************************/

int e_numero(array)
char * array;
{

 /* Declaracao de Variaveis Locais */

 int retorno = FALSE;
 char c;

 c = *array;

 while(c != '\0')
   {
    retorno = TRUE;
    if(c < '0' || c > '9')
      {
       retorno = FALSE;
       break;
      }
     c = *(++array);
    }
 return(retorno);
}

/*******************************************************************/
/* int fim_exp_reg(char *)                                         */
/* Autor: Marcos L. Chaim                                          */
/* Data: 17/01/90                                                  */
/* Versao: 1.0                                                     */
/*                                                                 */
/* Funcao: verifica se o apontador para o "string" de caracteres   */
/*         e' um NULL ou nao, ou seja, e' fim do "string" ou nao.  */
/*                                                                 */
/* Entradas: apontador para o vetor.                               */
/*                                                                 */
/* Saida: inteiro indicando TRUE se for fim e FALSE , caso con-    */
/*        contrario.                                               */
/*                                                                 */
/*******************************************************************/

int fim_exp_reg(array)
char * array;
{
if(*array == '\0')
    return(TRUE);
return(FALSE);
}

/*******************************************************************/
/* char * pega_path( int, FILE *)                                  */
/* Autor: Marcos L. Chaim                                          */
/* Data: 19/01/90                                                  */
/* Versao: 1.0                                                     */
/*                                                                 */
/* Funcao: encontra os potencial du-caminho que resta ser execu-   */
/*         tudo no arquivo "pdupaths.tes" e  coloca em vetor.      */
/*                                                                 */
/* Entradas: numero do arquivo e apontador o arquivo               */
/*           "pdupaths.tes".                                       */
/*                                                                 */
/* Saida: apontador para o caminho.                                */
/*                                                                 */
/*******************************************************************/

char * pega_path(path_id, file)
int path_id;
FILE * file;
{

 /* Declaracao de Variaveis Locais */

 static last_path_id = 0;
 char ascii_path_id[10];
 char * caminho;

 itoa(path_id,ascii_path_id,10);

 /* monta padrao para encontrar caminho */

 strcat(ascii_path_id,") ");

 if(last_path_id > path_id)
   rewind(file); /* o ultimo caminho estava na frente deste, entao
                    "rebobina-se" o arquivo para procurar do comeco */

 caminho = le_linha_str_gen(ascii_path_id, file);
 /* caminho = apont_next_tok(caminho); */
 last_path_id = path_id;
 return(caminho);
}

/*
** Fim heuristic2.c
*/
















