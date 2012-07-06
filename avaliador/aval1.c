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

extern int tot_req_exec[];

/*******************************************************************/
/* void avaliador(AUTOMATO *, b_vector [])                         */
/* Autor: Marcos L. Chaim                                          */
/* Data: 15/01/90                                                  */
/* Versao: 1.0                                                     */
/*                                                                 */
/* Funcao: avalia se os caminhos executados pelos casos de teste   */
/*         sao reconhecidos pelos automatos que representam as ex- */
/*         pressoes regulares.                                     */
/*                                                                 */
/* Entradas: apontador para a lista de automados, apontador para o */
/*           conjunto com os nos do grafo G.                       */
/*                                                                 */
/* Saida: total de requisitos de teste executados.                 */
/*                                                                 */
/*******************************************************************/

void avaliador(automatos_inicio,N)
AUTOMATO * automatos_inicio;
b_vector N[];
{

/* Declaracao de Variaveis Locais */

AUTOMATO * atual_automato;
char literal[10], lit_aux[10], * ptr_aux;
int numero;
int node;
long seq_exec = 0;
long no_func_invoc = 0;

 FILE * executed_paths = (FILE *) fopen("path.tes","r");
 if(executed_paths == (FILE *) NULL)
  error("* * Erro Fatal: Nao consegui abrir o arquivo paths.tes * *");

 /* algoritmo de avaliacao */

 while(fscanf(executed_paths,"%d",&node)!= EOF) /* diferente de fim de arquivo */
  {
  /* controla a sequencia de execucao */
    
  switch(node)
    {
    case 0:
      seq_exec=0;
      break;
    case 1:
      seq_exec=1;
      no_func_invoc++;
      break;
    default:
      seq_exec++;
    }

 /* verifica os automatos */

  for(atual_automato = automatos_inicio; atual_automato != (AUTOMATO *) NULL;
         atual_automato = atual_automato->next)
    {
     
     if(atual_automato->estado == Q1 && node != 0)
       { /* esta' em processo de reconhecimento da expressao regular */
	peg_tok(literal,atual_automato->pos_corrente);

        /* verifica qual acao a ser tomada */

        if(!strcmp(literal,"N*"))
          { /* e' igual a N* */
           peg_next_tok(lit_aux,atual_automato->pos_corrente);

           if(lit_aux[0] == '\0')
             literal[0] = '\0'; /* nao existe proximo literal;
                                   fim da expressao regular */
           else /* existe proximo literal */
             if(e_numero(lit_aux))
               {
                numero = atoi(lit_aux); /* converte ASCII para int */
                if(node == numero)
                /* reconheceu o no'; caminha na expressao regular */
                atual_automato->pos_corrente = jump_tok(atual_automato->pos_corrente);
                else
                 if(!test_bit(node,&N[atual_automato->criterio]))
                  atual_automato->estado = Q2; /* nao pertence a N; nao reconhece */
                }
              else
               error("* * Erro Fatal: Descritor nao esta' correto * *");
           }
        if(!strcmp(literal,"Nnvlf*"))
	  { /* e' igual a Nnvlf* */
           peg_next_tok(lit_aux,atual_automato->pos_corrente);

           if(lit_aux[0] == '\0')
             {
              literal[0] = '\0'; /* nao existe proximo literal; fim da expressao regular */
              if(atual_automato->estado == Q1)
                atual_automato->estado = Q3; /* reconheceu a entrada */
             }
           else /* existe proximo literal */
             if(e_numero(lit_aux))
               {
                numero = atoi(lit_aux); /* converte ASCII para int */
                if(node == numero)
                /* reconheceu o no'; caminha na expressao regular */
                 atual_automato->pos_corrente = jump_tok(atual_automato->pos_corrente);
                else
		 if(!test_bit(node,&(atual_automato->Nnv)))
                  atual_automato->estado = Q2; /* nao pertence a N, nao reconhece */
                }
              else
               error("* * Erro Fatal: Descritor nao esta' correto * *");
          }
	if(!strcmp(literal,"Nnv*"))
	  { /* e' igual a Nnv* */
           peg_next_tok(lit_aux,atual_automato->pos_corrente);

           if(lit_aux[0] == '\0')
             {
              literal[0] = '\0'; /* nao existe proximo literal; fim da expressao regular */
              if(atual_automato->estado == Q1)
                atual_automato->estado = Q3; /* reconheceu a entrada */
             }
           else /* existe proximo literal */
             if(e_numero(lit_aux))
               {
                numero = atoi(lit_aux); /* converte ASCII para int */
                if(node == numero)
                /* reconheceu o no'; caminha na expressao regular */
                 atual_automato->pos_corrente = jump_tok(atual_automato->pos_corrente);
                else
	         if(!test_bit(node,&(atual_automato->Nnv)))
                  atual_automato->estado = Q2; /* nao pertence a N, nao reconhece */
                }
              else
               error("* * Erro Fatal: Descritor nao esta' correto * *");
           }

         if(!strcmp(literal,"Nlf*"))
	  { /* e' igual a Nlf* */
           peg_next_tok(lit_aux,atual_automato->pos_corrente);

           if(lit_aux[0] == '\0')
             {
              literal[0] = '\0'; /* nao existe proximo literal; fim da expressao regular */
              if(atual_automato->estado == Q1)
                atual_automato->estado = Q3; /* reconheceu a entrada */
             }
           else /* existe proximo literal */
             if(e_numero(lit_aux))
               {
                numero = atoi(lit_aux); /* converte ASCII para int */
                if(node == numero)
                /* reconheceu o no'; caminha na expressao regular */
                 atual_automato->pos_corrente = jump_tok(atual_automato->pos_corrente);
                else
		 if(!test_bit(node,&(atual_automato->Nnv)))
                  atual_automato->estado = Q2; /* nao pertence a N, nao reconhece */
                }
              else
               error("* * Erro Fatal: Descritor nao esta' correto * *");
           }

        if(e_numero(literal))
          { /* e' um numero */
           numero = atoi(literal); /* converte ASCII para int */
           if(numero == node)
           /* reconheceu o no'; caminha na expressao regular */
             atual_automato->pos_corrente = apont_next_tok(atual_automato->pos_corrente);
           else
             atual_automato->estado = Q2; /* nao reconheceu a entrada */
          }
        if(!strcmp(literal,"["))
 	  {
	   /* faz ptr_aux apontar para o token "]" */
	   ptr_aux = apont_next_tok(jump_tok(atual_automato->pos_corrente));

	   peg_next_tok(lit_aux,ptr_aux);

	   if(lit_aux[0] ==  '\0')
	      error("Erro Fatal: Expressao Regular errado no automato");
           else /* existe proximo literal */
             if(e_numero(lit_aux))
               {
                numero = atoi(lit_aux); /* converte ASCII para int */
                if(node == numero)
                /* reconheceu o no'; caminha na expressao regular */
	 	  atual_automato->pos_corrente = jump_tok(ptr_aux);
                else
		  if(!test_bit(node,&(atual_automato->Nnv)))
		    atual_automato->estado = Q2; /* nao pertence a N, nao reconhece */
		  else
		    atual_automato->pos_corrente = apont_next_tok(atual_automato->pos_corrente);
                }
              else
               error("* * Erro Fatal: Descritor nao esta' correto * *");
	  }
        if(!strcmp(literal,"]*"))
  	  {
	   peg_next_tok(lit_aux,atual_automato->pos_corrente);

	   if(lit_aux[0] ==  '\0')
	     error("Erro Fatal: Expressao Regular errado no automato");
           else /* existe proximo literal */
             if(e_numero(lit_aux))
               {
                numero = atoi(lit_aux); /* converte ASCII para int */
                if(node == numero)
                /* reconheceu o no'; caminha na expressao regular */
		  atual_automato->pos_corrente = jump_tok(atual_automato->pos_corrente);
                else
		  if(!test_bit(node,&(atual_automato->Nnv)))
		     atual_automato->estado = Q2; /* nao pertence a N, nao reconhece */
		  else
		     atual_automato->pos_corrente = apont_prev_tok(apont_prev_tok(atual_automato->pos_corrente));
                }
              else
               error("* * Erro Fatal: Descritor nao esta' correto * *");
	   }
      }
     
    if(atual_automato->estado == Q3 || (atual_automato->estado == Q1 &&
       fim_exp_reg(atual_automato->pos_corrente)))
      {
      /* registra a frequencia */
      atual_automato->estado = Q1;
      atual_automato->freq++;
      atual_automato->pos_corrente = atual_automato->exp_regular;
      if(atual_automato->i == node)        
        atual_automato->pos_corrente = jump_tok(atual_automato->exp_regular);

      if(atual_automato->criterio != PU && atual_automato->criterio != ARCS && atual_automato->criterio != NOS)
        {
        /* atual_automato->Nnv = atual_automato->Ni; */
        b_vector_cpy(&(atual_automato->Nnv),&(atual_automato->Ni));
        }          
        
      }
    
    if(atual_automato->estado == Q2 && atual_automato->i == node)
      { /* "reseta" o automato */
       atual_automato->estado = Q1;
       atual_automato->pos_corrente = jump_tok(atual_automato->exp_regular);
       /* se o criterio nao e todos-pot-usos, recupera o conjunto Nnv */

       if(atual_automato->criterio != PU && atual_automato->criterio != ARCS && atual_automato->criterio != NOS)
	  {
          /* atual_automato->Nnv = atual_automato->Ni; */
          b_vector_cpy(&(atual_automato->Nnv),&(atual_automato->Ni));
          }
      }

    switch(atual_automato->freq)
         {
         case 0:
           /* Primeira ocorrencia do automato, guarda a sequencia de execucao
              do no i */
           if(atual_automato->i == node)
             atual_automato->seq_exec_i = seq_exec;          
           break;
         case 1:
           /* Primeira ocorrencia do automato, guarda a sequencia de execucao
              do no j */
           if(atual_automato->seq_exec_j == 0)
	     {
             atual_automato->seq_exec_j = seq_exec;
             atual_automato->no_func_invoc = no_func_invoc;
	     }
           break;
         default:;
           /* Ja' foi registrada a primeira ocorrencia da associacao */
         }
           
/* se o criterio nao e todos-pot-usos, retira o no do conjunto Nnv */
  if((atual_automato->criterio == PDU && atual_automato->criterio == PUDU) &&
     atual_automato != (AUTOMATO *) NULL && strcmp(literal,"N*"))
     reset_bit(node,&(atual_automato->Nnv));

   }

  }

 /* terminou de avaliar um caso de teste agora ajusta automatos */

 for(atual_automato = automatos_inicio;atual_automato != (AUTOMATO *) NULL;
     atual_automato = atual_automato->next)
   if(atual_automato->freq == 0)
     { /* nao reconheceu a entrada */
       atual_automato->estado = Q1;
       atual_automato->pos_corrente = atual_automato->exp_regular;
     }
   else
     if(atual_automato->freq > 0)
       {
       atual_automato->estado = Q3; /* reconheceu a entrada */
       tot_req_exec[atual_automato->criterio] += atual_automato->freq;
       }

 fclose(executed_paths);
 return;
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


