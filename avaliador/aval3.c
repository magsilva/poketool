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

extern int no_nos, no_vars;

/*******************************************************************/
/* void pu_init_aval(AUTOMATO **, b_vector *)                      */
/* Autor: Marcos L. Chaim                                          */
/* Versao: 1.0                                                     */
/* Data: 17/01/90                                                  */
/*                                                                 */
/* Funcao: criar as listas que contem os automatas para avaliacao  */
/*         dos criterios todos-pot-usos/du caminhos e todos-       */
/*         pot-usos.                                               */
/*                                                                 */
/* Entradas: apontador para o apontador do inicio da lista         */
/*           de automatos e apontador para o conjunto dos nos do   */
/*           grafo G.                                              */
/*                                                                 */
/* Saida: nenhuma.                                                 */
/*                                                                 */
/*******************************************************************/

void pu_init_aval(ptr_pu,N,criterio,test_history,descritores)
AUTOMATO ** ptr_pu;
b_vector * N;
int criterio;
FILE * test_history;
FILE * descritores;
{

/* Declaracao de Variaveis Locais */

AUTOMATO * pu_start; /* apontador para o primeiro elemento da lista
                         de automatos do criterio todos-pot-du-caminhos */

AUTOMATO * pu_aux;

char linha[MAXLINE], *ptr_linha;
int ac_path, dummy1, dummy2, dummy3, dummy4, dummy5;
int i;

 b_vector Ni_aux, Nt_aux;

 /*  le o cabecalho do arquivo */

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

 /*  le o conjunto N do arquivo */

 le_conj(N,"N = ",descritores);

 /* aloca espaco para o regitros AUTOMATO auxiliares para construcao das listas
   de automatos */

 pu_start = (AUTOMATO *) malloc(sizeof(AUTOMATO));
 if(pu_start == (AUTOMATO *) NULL)
     error("* * Erro Fatal: Nao consegui alocar espaco * *");

 pu_aux = pu_start;

 pu_aux->next = (AUTOMATO *) NULL;

 /* pega o numero do primeiro descritor satisfeito se existir arquivo .his */

 if(test_history != (FILE *) NULL)
    fscanf(test_history,"%d (%d/%d) (%d %d/%d)", &ac_path, &dummy1, &dummy2, &dummy3, &dummy4, &dummy5);/* le o numero do proximo caminho */

 ptr_linha = le_linha_str(linha,"Grafo(",descritores);

 while(ptr_linha != (char *) NULL)
    {
     /* determina o numero do grafo i */

     i = det_number(ptr_linha);

     /* determina Ni */

     le_conj(&Ni_aux,"Ni = ",descritores);

     /* determina Nh */

     le_conj(&Nt_aux,"Nt = ",descritores);

     /* cria elementos da lista de automatos para o criterio
        *todos-pot-du-caminhos */

     ptr_linha = le_linha_str(linha,"N*",descritores);

     while(ptr_linha != (char *) NULL)
       {
        pu_aux->next = (AUTOMATO *) malloc(sizeof(AUTOMATO));
        if(pu_aux == (AUTOMATO *) NULL)
          error("* * Erro Fatal: Nao consegui alocar espaco * *");
        /* preenche os campos do registro AUTOMATO */

        pu_aux = pu_aux->next;
        pu_aux->next = (AUTOMATO *) NULL;
        pu_aux->i = i;
        pu_aux->estado = Q1;
        pu_aux->freq=0;
        pu_aux->seq_exec_i = 0;
        pu_aux->seq_exec_j = 0;
        pu_aux->no_func_invoc = 0;
        pu_aux->criterio = criterio;

        /* pu_aux->Ni = Ni_aux; */

        b_vector_cons(&(pu_aux->Ni),no_nos/BYTE+1,0);
        b_vector_cpy(&(pu_aux->Ni),&Ni_aux);
       
        b_vector_cons(&(pu_aux->Nnv),no_nos/BYTE+1,0);

        /*  le a expressao regular */

        pu_aux->n_path =  det_number(linha); /* determina numero do caminho */

        /* Verifica se caminho ja' foi satisfeito por algum caso de teste anterior */

	if(test_history != (FILE *) NULL)
          { /* ja' foram executados outros casos de teste previamente */
	  if(pu_aux->n_path == ac_path && ac_path !=0 )
             { /* caminho ja' foi aceito */
	      pu_aux->estado = Q3;
              fscanf(test_history,"%d (%d/%d) (%d %d/%d)", &ac_path, &dummy1, &dummy2, &dummy3, &dummy4, &dummy5);/* le o numero do proximo caminho */
             }
          }

        pu_aux->exp_regular = (char *) malloc((strlen(ptr_linha)+1)*sizeof(char));
        if(pu_aux->exp_regular == (char *) NULL)
         error("* * Erro Fatal: Nao consegui alocar espaco * *");

        strcpy(pu_aux->exp_regular,ptr_linha); /* copia a expressao regular no
                                             espaco apontado por pu_aux->
                                             exp_regular */
	pu_aux->pos_corrente = pu_aux->exp_regular;

	/* le conjunto de nos onde nao existem definicao de variaveis contidas
	   no deff */

	switch(criterio)
	 {
	  case PU:   le_conj(&(pu_aux->Nnv),"Nnv = ",descritores); break;
          case PUDU: le_conj(&(pu_aux->Nnv),"Nnvlf = ",descritores); break;
	  default: error("* * Erro Fatal: Especificacao de Criterio errado * *");
	 }

        /* faz Ni igual ao conjunto de nos onde nao existe definicao de varia-
	   veis do deff */

        /* pu_aux->Ni = pu_aux->Nnv; */

        b_vector_cpy(&(pu_aux->Ni),&(pu_aux->Nnv));

        /* le proxima expressao regular */

        if(fgets(linha,MAXLINE-1,descritores) == (char *) NULL)
          error("* * Erro Fatal: Arquivo des_pu.tes imperfeito * *");

        if(fgets(linha,MAXLINE-1,descritores) == (char *) NULL)
          error("* * Erro Fatal: Arquivo des_pu.tes imperfeito * *");

        ptr_linha = strstr(linha,"N*"); /* verifica se uma expressao regular */
       }
     ptr_linha = find_grfi(linha,descritores);
   }

 /* faz os ponteiros passados por referencia apontarem para as listas criadas */

 *ptr_pu = pu_start->next;

 /* Destroi vetores de bits auxiliares */
 
 b_vector_destroy(&Ni_aux);
 b_vector_destroy(&Nt_aux);

 /* libera o espaco ocupado pelos registros AUTOMATO's auxiliares */

 free(pu_start);
}


/*******************************************************************/
/* void no_arc_init_aval(AUTOMATO **, b_vector *)                  */
/* Autor: Marcos L. Chaim                                          */
/* Versao: 1.0                                                     */
/* Data: 29/12/92 (Dia da Renuncia do Presidente Collor)           */
/*                                                                 */
/* Funcao: criar as listas que contem os automatas para avaliacao  */
/*         do criterio todos-arcos.                                */
/*                                                                 */
/* Entradas: apontador para o apontador do inicio da lista         */
/*           de automatos e apontador para o conjunto dos nos do   */
/*           grafo G.                                              */
/*                                                                 */
/* Saida: nenhuma.                                                 */
/*                                                                 */
/*******************************************************************/

void no_arc_init_aval(ptr_arc,N,criterio,test_history,descritores)
AUTOMATO ** ptr_arc;
b_vector * N;
int criterio;
FILE * test_history;
FILE * descritores;
{

 /* Declaracao de Variaveis Locais */

 AUTOMATO * arc_start; /* apontador para o primeiro elemento da lista
			 de automatos do criterio todos-arcos */

 AUTOMATO * arc_aux;
 char linha[MAXLINE], *ptr_linha;
 int ac_path, dummy1, dummy2, dummy3, dummy4, dummy5;;

 /*  le o cabecalho do arquivo */

 le_linha_str(linha,"Descritores",descritores);

 /* le o numero de nos */

 le_linha_str(linha,"Numero total de nos:", descritores);

 sscanf(linha,"Numero total de nos: %d", &no_nos);

 /* inicia vetores de bits */

 b_vector_cons(N,no_nos/BYTE+1,0);

 /*  le o conjunto N do arquivo */

 le_conj(N,"N = ",descritores);

 /* aloca espaco para o regitros AUTOMATO auxiliares para construcao das listas
   de automatos */

 arc_start = (AUTOMATO *) malloc(sizeof(AUTOMATO));
 if(arc_start == (AUTOMATO *) NULL)
     error("* * Erro Fatal: Nao consegui alocar espaco * *");

 arc_aux = arc_start;

 arc_aux->next = (AUTOMATO *) NULL;

 /* pega o numero do primeiro descritor satisfeito se existir arquivo .his */

 if(test_history != (FILE *) NULL)
   {
    if(criterio == NOS)
     fscanf(test_history,"%d (%d %d/%d) (%d)", &ac_path, &dummy1, &dummy2, &dummy3, &dummy4,&dummy5);/* le o numero do proximo caminho */
    else /* ARCS */
      fscanf(test_history,"%d (%d/%d) (%d %d/%d)", &ac_path, &dummy1, &dummy2, &dummy3, &dummy4, &dummy5);
   }

 /* cria elementos da lista de automatos para o criterio todos-arcos */

 ptr_linha = le_linha_str(linha,"N*",descritores);

 while(ptr_linha != (char *) NULL)
       {
	arc_aux->next = (AUTOMATO *) malloc(sizeof(AUTOMATO));
	if(arc_aux == (AUTOMATO *) NULL)
          error("* * Erro Fatal: Nao consegui alocar espaco * *");
        /* preenche os campos do registro AUTOMATO */

	arc_aux = arc_aux->next;
	arc_aux->next = (AUTOMATO *) NULL;

	arc_aux->estado = Q1;
        arc_aux->freq=0;
        arc_aux->seq_exec_i=0;
        arc_aux->seq_exec_j=0;
        arc_aux->no_func_invoc=0;
        arc_aux->criterio=criterio;

        /*  le a expressao regular */

	arc_aux->n_path =  det_number(linha); /* determina numero do arco */

	if(criterio == NOS)
	  arc_aux->i = arc_aux->n_path;
	else    /* ARCS */
	  arc_aux->i = det_num_arc_i(linha);

	/* Verifica se o arco ja' foi satisfeito por algum caso de teste anterior */

	if(test_history != (FILE *) NULL)
          { /* ja' foram executados outros casos de teste previamente */
	  if(arc_aux->n_path == ac_path && ac_path !=0 )
	     { /* arco ja' foi aceito */
	      arc_aux->estado = Q3;
	      if(criterio != NOS)
		/* ARCS */
		fscanf(test_history,"%d (%d/%d) (%d %d/%d)", &ac_path, &dummy1, &dummy2, &dummy3, &dummy4, &dummy5); /* le o numero do proximo caminho */
	      else 
		fscanf(test_history,"%d (%d %d/%d) (%d)", &ac_path, &dummy1, &dummy2, &dummy3, &dummy4);
             }
          }

	arc_aux->exp_regular = (char *) malloc((strlen(ptr_linha)+1)*sizeof(char));
	if(arc_aux->exp_regular == (char *) NULL)
         error("* * Erro Fatal: Nao consegui alocar espaco * *");

	strcpy(arc_aux->exp_regular,ptr_linha); /* copia a expressao regular no
					     espaco apontado por arc_aux->
                                             exp_regular */
	arc_aux->pos_corrente = arc_aux->exp_regular;

        /* faz Ni igual ao conjunto de nos onde nao existe definicao de varia-
	    veis do deff */

        /* arc_aux->Ni = arc_aux->Nnv = *N; */
        
        b_vector_cons(&(arc_aux->Ni),no_nos/BYTE+1,0);     
        b_vector_cons(&(arc_aux->Nnv),no_nos/BYTE+1,0);

        b_vector_cpy(&(arc_aux->Nnv),N);
        b_vector_cpy(&(arc_aux->Ni),N);

        /* le proxima expressao regular */

        if(fgets(linha,MAXLINE-1,descritores) == (char *) NULL)
	  error("* * Erro Fatal: Arquivo \"des_arc.tes\" imperfeito * *");


        ptr_linha = strstr(linha,"N*"); /* verifica se uma expressao regular */
	}

 /* faz os ponteiros passados por referencia apontarem para as listas criadas */

 *ptr_arc = arc_start->next;

 /* libera o espaco ocupado pelos registros AUTOMATO's auxiliares */

 free(arc_start);

}

/*******************************************************************/
/* int det_num_arc_i(char *)                                       */
/* Autor: Marcos L. Chaim                                          */
/* Versao: 1.0                                                     */
/* Data: 29/12/92 (Dia da Renuncia do Presidente Collor)           */
/*                                                                 */
/* Funcao: determinar o numero do primeiro no' do arco primitivo.  */
/*                                                                 */
/* Entradas: apontador para a expressao regular do arco primitivo. */
/*                                                                 */
/* Saida: o numero do primeiro no'.                                */
/*                                                                 */
/*******************************************************************/

int det_num_arc_i(string)
char * string;
{
/* Declaracao de Variaveis Locais */

 char * ptr, num [100];

 ptr = strstr(string,"N*");
 if(ptr == NULL)
   error("* * Erro Fatal: arquivo de descritores do criterio \
todos-arcos, ou todos-nos, erroneo * *");

 ptr = strstr(ptr," ");

 if(ptr != NULL)
   strcpy(num,ptr);
 else
   error("* * Erro Fatal: arquivo de descritores do criterio \
todos-arcos, ou todos-nos, erroneo * *");

 ptr = num;
 while(*ptr == ' ') ++ptr;
 while(*ptr >= '0' && *ptr <= '9') ++ptr;

 *ptr = '\0';

 return(atoi(num));
}









