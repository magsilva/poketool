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
#include "haval.h"

/* Declaracao de Variaveis Globais */


/*******************************************************************/
/* void pdu_init_aval(AUTOMATO **, b_vector *, FILE *)             */
/* Autor: Marcos L. Chaim                                          */
/* Versao: 1.0                                                     */
/* Data: 17/01/90                                                  */
/*                                                                 */
/* Funcao: criar as listas que contem os automatas para avaliacao  */
/*         dos criterios todos-pot-du-caminhos e todos-pot-usos.   */
/*                                                                 */
/* Entradas: apontador para o apontador do inicio da lista         */
/*           de automatos e apontador para o conjunto dos nos do   */
/*           grafo G.                                              */
/*                                                                 */
/* Saida: nenhuma.                                                 */
/*                                                                 */
/*******************************************************************/

void pdu_init_aval(ptr_pdu,N,descritores)
AUTOMATO ** ptr_pdu;
b_vector * N;
FILE * descritores;
{

 /* Declaracao de Variaveis Locais */
 int no_nos;
 int no_vars;

 AUTOMATO * pdu_start; /* apontador para o primeiro elemento da lista
                         de automatos do criterio todos-pot-du-caminhos */

 AUTOMATO * pdu_aux;

 char linha[MAXLINE], *ptr_linha;
 int i;

 b_vector Ni_aux, Nt_aux;

 /* le o cabecalho do arquivo */

 le_linha_str(linha,"DESCRITORES",descritores);

 /* le o numero de nos */

 le_linha_str(linha,"Numero total de nos:", descritores);

 sscanf(linha,"Numero total de nos: %d", &no_nos);

 /* le o numero de variaveis */

 le_linha_str(linha,"Numero total de variaveis:", descritores);

 sscanf(linha,"Numero total de variaveis: %d", &no_vars);

 /* inicia vetores de bits */

 b_vector_cons(N,no_nos/BYTE+1,0);
 b_vector_cons(&Ni_aux,no_nos/BYTE+1,0);
 b_vector_cons(&Nt_aux,no_nos/BYTE+1,0);

 /* le o conjunto N do arquivo */

 le_conj(N,"N = ",descritores);

 /* aloca espaco para o regitros AUTOMATO auxiliares para construcao das listas
   de automatos */

 pdu_start = (AUTOMATO *) malloc(sizeof(AUTOMATO));
 if(pdu_start == (AUTOMATO *) NULL)
     error_msg(mem_fault);

 pdu_aux = pdu_start;

 pdu_aux->next = (AUTOMATO *) NULL;

 ptr_linha = le_linha_str(linha,"Grafo(",descritores);

 while(ptr_linha != (char *) NULL)
    {
     /* determina o numero do grafo i */

     i = det_number(ptr_linha);

     /* determina Ni */

     le_conj(&Ni_aux,"Ni = ",descritores);

     /* tira i de Ni */

     reset_bit(i,&Ni_aux);

     /* determina Nh */

     le_conj(&Nt_aux,"Nt = ",descritores);

     /* cria elementos da lista de automatos para o criterio
        *todos-pot-du-caminhos */

     ptr_linha = le_linha_str(linha,"N*",descritores);

     while(ptr_linha != (char *) NULL)
       {
        pdu_aux->next = (AUTOMATO *) malloc(sizeof(AUTOMATO));
        if(pdu_aux == (AUTOMATO *) NULL)
          error_msg(mem_fault);
        /* preenche os campos do registro AUTOMATO */

        pdu_aux = pdu_aux->next;
        pdu_aux->next = (AUTOMATO *) NULL;
        pdu_aux->i = i;
        pdu_aux->estado = Q1;
        pdu_aux->freq=0;
        pdu_aux->seq_exec_i = 0;
        pdu_aux->seq_exec_j = 0;
        pdu_aux->no_func_invoc = 0;
        pdu_aux->criterio = PDU;

        /* inicia vetores de bits */
        
        /*
        pdu_aux->Ni = Ni_aux;
	pdu_aux->Nnv = Ni_aux;
        */

        b_vector_cons(&(pdu_aux->Ni),no_nos/BYTE+1,0);
        b_vector_cons(&(pdu_aux->Nnv),no_nos/BYTE+1,0);

        b_vector_cpy(&(pdu_aux->Ni),&Ni_aux);
        b_vector_cpy(&(pdu_aux->Nnv),&Ni_aux);
        
        /*  le a expressao regular */

        pdu_aux->n_path =  det_number(linha); /* determina numero do caminho */

        pdu_aux->exp_regular = (char *) malloc((strlen(ptr_linha)+1)*sizeof(char));
        if(pdu_aux->exp_regular == (char *) NULL)
         error_msg(mem_fault);

        strcpy(pdu_aux->exp_regular,ptr_linha); /* copia a expressao regular no
                                             espaco apontado por pdu_aux->
                                             exp_regular */
        pdu_aux->pos_corrente = pdu_aux->exp_regular;

        /* le proxima expressao regular */

        if(fgets(linha,MAXLINE-1,descritores) == (char *) NULL)
			 error_msg(incorrect_descriptor);

        ptr_linha = strstr(linha,"N*"); /* verifica se uma expressao regular */
       }
     ptr_linha = find_grfi(linha,descritores);
   }

 /* faz os ponteiros passados por referencia apontarem para as listas criadas */

 *ptr_pdu = pdu_start->next;

 /* Destroi vetores de bits auxiliares */
 
 b_vector_destroy(&Ni_aux);
 b_vector_destroy(&Nt_aux);

 /* libera o espaco ocupado pelos registros AUTOMATO's auxiliares */

 free(pdu_start);
}


/*******************************************************************/
/* AUTOMATO * cria_pu_automatos(AUTOMATO *, b_vector *, int)      */
/* Autor: Marcos L. Chaim                                          */
/* Versao: 1.0                                                     */
/* Data: 17/01/90                                                  */
/*                                                                 */
/* Funcao: cria os automatos relativos aos nos do grafo(i) para o  */
/*         criterio todos pot-usos.                                */
/*                                                                 */
/* Entradas: apontador para o ultimo elemento da lista de automa-  */
/*           tos do criterio todos pot-usos.                       */
/*                                                                 */
/* Saida: apontador para o ultimo elemento da lista de automatos.  */
/*                                                                 */
/*******************************************************************/

AUTOMATO * cria_pu_automatos(pu_aux, Ni, i, pu_test_history)
AUTOMATO * pu_aux;
b_vector * Ni;
int i;
FILE * pu_test_history;
{

 /* Declaracao de Variaveis Locais */

 static int assoc_id = 0;  /* numero de identificacao da associacao */
 static int acep_assoc = -10; /* numero da associacao ja' aceita */
 int k;
 char expreg[30], ascii_number[5];

 if(pu_test_history != (FILE *) NULL && acep_assoc <= 0)
     fscanf(pu_test_history,"%d",&acep_assoc); /* le o numero da proxima associacao */

 for(k = 0; k>=0 && k <= NMAXVAR; ++k)
   {
    if(test_bit(k,Ni) && k != i)
      {
        pu_aux->next = (AUTOMATO *) malloc(sizeof(AUTOMATO));
        if(pu_aux == (AUTOMATO *) NULL)
          error_msg(mem_fault);

        /* preenche os campos do registro AUTOMATO */

        ++assoc_id;  /* incrementa o numero de identificacao de associacao */

        pu_aux = pu_aux->next;
        pu_aux->next = (AUTOMATO *) NULL;
        pu_aux->i = i;
        pu_aux->n_path = k;
        pu_aux->assoc_id = assoc_id;
        pu_aux->estado = Q1;
        pu_aux->Ni = *Ni;

        /* verifica se associacao ja' foi satisfeita */

        if(pu_test_history != (FILE *) NULL)
          { /* ja' foram executados outros casos de teste previamente */

           if(pu_aux->assoc_id == acep_assoc && acep_assoc != 0 )
             { /* associacao ja' foi aceita */
              pu_aux->estado = Q3;
              fscanf(pu_test_history,"%d", &acep_assoc); /* le o numero do proxima associacao */
             }
          }

        /* cria expressao regular */

        strcpy(expreg,"N* ");

        itoa(i,ascii_number,10);

        strcat(expreg,ascii_number);
        strcat(expreg," Ni* ");

        itoa(k,ascii_number,10);

        strcat(expreg,ascii_number);

        pu_aux->exp_regular = (char *) malloc((strlen(expreg)+1)*sizeof(char));
        if(pu_aux->exp_regular == (char *) NULL)
          error_msg(mem_fault);

        strcpy(pu_aux->exp_regular,expreg); /* copia a expressao regular no
                                             espaco apontado por pu_aux->
                                             exp_regular */
        pu_aux->pos_corrente = pu_aux->exp_regular;

      }
   }

 return(pu_aux);
}

/*******************************************************************/
/* void le_conj(b_vector *, char *, FILE *)                        */
/* Autor: Marcos L. Chaim                                          */
/* Versao: 1.0                                                     */
/* Data: 17/01/90                                                  */
/*                                                                 */
/* Funcao: ler um conjunto de nos a partir do arquivo descritores. */
/*                                                                 */
/* Entradas: conjunto de bits, string que determina o conjunto e   */
/*           apontador para o arquivo de descritores.              */
/*                                                                 */
/* Saida: nenhuma.                                                 */
/*                                                                 */
/*******************************************************************/

void le_conj(conj, string, file)
b_vector * conj;
char string[20];
FILE * file;
{

 /* Declaracoes de Variaveis Locais */

 char * ptr_linha, linha[MAXLINE], token[5];
 int numero;

  do
    {
     if(fgets(linha,MAXLINE-1,file) == (char *) NULL)
       error_msg(incorrect_descriptor);
     ptr_linha = strstr(linha,string);
    }
  while(ptr_linha == (char *) NULL);

 /* Determina o conjunto */

 reset_all(conj);
 ptr_linha = jump_tok(ptr_linha); /* pula o string */

 while(*ptr_linha != '\0')
      {
       peg_tok(token,ptr_linha);

       if(!e_numero(token))
         error_msg(out_of_range);

       numero = atoi(token);
       set_bit(numero, conj);  /* coloca numero em conj */

       ptr_linha = apont_next_tok(ptr_linha); /* faz ptr_linha apontar para proximo token */
      }
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
       error_msg(wrong_file_desc);
    ptr_linha = strstr(linha,string);
   }
 while(ptr_linha == (char *) NULL);
 return(ptr_linha);
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
         error_msg(wrong_file_desc);
      
      if((strlen(linha_aux) == MAXLINE-2 && linha_aux[MAXLINE-2] != '\n')
         || i > 1)
        {         
        /* linha muito grande */

        if(strlen(linha_aux) + 1 == MAXLINE-1)
          {
          ++i;
        
          p_linha_aux2 = (char *) malloc(sizeof(char)* (i * (MAXLINE-1)));

          if(p_linha_aux2 == (char *) NULL)
             error_msg(mem_fault);
        
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
          error_msg(mem_fault);
        }
        
      }
    while(p_linha_aux[strlen(p_linha_aux)-1] != '\n');
    
    ptr_linha = strstr(p_linha_aux,string);
   }
 while(ptr_linha == (char *) NULL);
 return(p_linha_aux);
}

/*******************************************************************/
/* int det_number(char *)                                          */
/* Autor: Marcos L. Chaim                                          */
/* Data: 17/01/90                                                  */
/* Versao: 1.0                                                     */
/*                                                                 */
/* Funcao: determina o numero contido num "string" do tipo XXnnnn) */
/*         onde X e' um ASCII nao numeral, n e' um numeral e ")"   */
/*         e' um fecha parenteses.                                 */
/*                                                                 */
/* Entradas: apontador para o "string".                            */
/*                                                                 */
/* Saida: inteiro com o valor do numero.                           */
/*                                                                 */
/*******************************************************************/

int det_number(string)
char * string;
{

 /* Declaracao de Variaveis Locais */

 char aux_number[MAXLINE];
 int i = 0;

 while(*string != ')' && *string != '\0' && i < MAXLINE)
   {
    if( *string >= '0' && *string <= '9' )
      {
       aux_number[i] = *string;
       ++i;
      }
    ++string;
   }
 aux_number[i] = '\0';
 return(atoi(aux_number));
}

/*******************************************************************/
/* char * find_grfi(char *, FILE *)                                */
/* Autor: Marcos L. Chaim                                          */
/* Data: 18/01/90                                                  */
/* Versao: 1.0                                                     */
/*                                                                 */
/* Funcao: encontra a linha do arquivo que inicia os descritores;  */
/*         caso nao exista mais descritores retorna NULL.          */
/*                                                                 */
/* Entradas: vetor onde sera' armazenada o linha do arquivo e      */
/*           apontador para  o arquivo.                            */
/*                                                                 */
/* Saida: endereco da posicao que inicia o "string" "Grafo("; se   */
/*        nao existir retorna NULL.                                */
/*                                                                 */
/*******************************************************************/

char * find_grfi(linha, file)
char * linha;
FILE * file;
{

 /* Declaracao de Variaveis Locais */

 char * ptr_linha;

 do
   {
    if(fgets(linha,MAXLINE-1,file) == (char *) NULL)
		 error_msg(incorrect_descriptor);

    ptr_linha = strstr(linha,"Grafo(");
    if(ptr_linha != (char *) NULL)
            break;
    else
       {
        if(strstr(linha,"Numero") != (char *) NULL)
           {
            ptr_linha = (char *) NULL;
            break;
           }
       }
   }
 while(TRUE);
 return(ptr_linha);
}



/*******************************************************************/
/* char * find_desc(char *, char *, FILE *)                        */
/* Autor: Marcos L. Chaim                                          */
/* Data: 12/10/99                                                  */
/* Versao: 1.0                                                     */
/*                                                                 */
/* Funcao: encontra a linha do arquivo que inicia os descritores;  */
/*         caso nao exista mais descritores retorna NULL.          */
/*                                                                 */
/* Entradas: vetor onde sera' armazenada o linha do arquivo e      */
/*           apontador para  o arquivo.                            */
/*                                                                 */
/* Saida: endereco da posicao que inicia o "string" "Descritores"; */
/*        se nao existir retorna NULL.                             */
/*                                                                 */
/*******************************************************************/

char * find_desc(linha,token,file)
char * linha;
char * token;
FILE * file;
{

 /* Declaracao de Variaveis Locais */

 char * ptr_linha;

 do
   {
    if(fgets(linha,MAXLINE-1,file) == (char *) NULL)
       error_msg(incorrect_descriptor);

    ptr_linha = strstr(linha,token);
    if(ptr_linha != (char *) NULL)
            break;
    else
       {
        if(strstr(linha,"Numero") != (char *) NULL)
           {
            ptr_linha = (char *) NULL;
            break;
           }
       }
   }
 while(TRUE);
 return(ptr_linha);
}


