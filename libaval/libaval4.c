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



/*******************************************************************/
/* void df_init_aval(AUTOMATO **, b_vector *, int, FILE *)         */
/* Autor: Marcos L. Chaim                                          */
/* Versao: 1.0                                                     */
/* Data: 12/10/99                                                  */
/*                                                                 */
/* Funcao: criar as listas que contem os automatas para avaliacao  */
/*         dos criterios todos-usos, todos-p-usos e todos-du-cami- */
/*         nhos.                                                   */
/*                                                                 */
/* Entradas: apontador para o apontador do inicio da lista         */
/*           de automatos e apontador para o conjunto dos nos do   */
/*           grafo G.                                              */
/*                                                                 */
/* Saida: nenhuma.                                                 */
/*                                                                 */
/*******************************************************************/

void df_init_aval(ptr_pu,N,criterio,descritores)
AUTOMATO ** ptr_pu;
b_vector * N;
int criterio;
FILE * descritores;
{

 /* Declaracao de Variaveis Locais */

 AUTOMATO * pu_start; /* apontador para o primeiro elemento da lista
                         de automatos um dos dos criterios "data flow" */

 AUTOMATO * pu_aux;

 char linha[MAXLINE], *ptr_linha;
 int no_nos, no_vars;
 int i=0;

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
     error_msg(mem_fault);

 pu_aux = pu_start;

 pu_aux->next = (AUTOMATO *) NULL;

 ptr_linha = le_linha_str(linha,"Descritores",descritores);

 while(ptr_linha != (char *) NULL)
    {
     /* determina o numero do grafo i */

     i = det_number(ptr_linha);

     /* determina Ni */
     switch(criterio)
          {
          case PUSOS:
          case USOS:
            le_conj(&Ni_aux,"Nnv = ",descritores);          
            break;
            
          case DU:            
            break;
            
          default:
            error_msg(invalid_criterion);
            break;
          }  

     /* cria elementos da lista de automatos para o criterio
        *todos-pot-du-caminhos */

     ptr_linha = le_linha_str(linha,"N*",descritores);

     while(ptr_linha != (char *) NULL)
       {
        pu_aux->next = (AUTOMATO *) malloc(sizeof(AUTOMATO));
        if(pu_aux == (AUTOMATO *) NULL)
          error_msg(mem_fault);
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
        
        b_vector_cons(&(pu_aux->Ni),no_nos/BYTE+1,0);
        b_vector_cons(&(pu_aux->Nnv),no_nos/BYTE+1,0);

        /* ajusta conjunto de bits conforme o criterio */

        switch(criterio)
          {
          case PUSOS:
          case USOS:
            b_vector_cpy(&(pu_aux->Ni),&Ni_aux);
            b_vector_cpy(&(pu_aux->Nnv),&Ni_aux);
            break;
            
          case DU:
            b_vector_cpy(&(pu_aux->Ni),N);
            b_vector_cpy(&(pu_aux->Nnv),N);
            break;
            
          default:
            error_msg(invalid_criterion);
            break;
          }

        /*  le a expressao regular */

        pu_aux->n_path =  det_number(linha); /* determina numero do caminho */

        pu_aux->exp_regular = (char *) malloc((strlen(ptr_linha)+1)*sizeof(char));
        if(pu_aux->exp_regular == (char *) NULL)
          error_msg(mem_fault);

        strcpy(pu_aux->exp_regular,ptr_linha); /* copia a expressao regular no
                                             espaco apontado por pu_aux->
                                             exp_regular */
	pu_aux->pos_corrente = pu_aux->exp_regular;

        /* le proxima expressao regular */

        if(fgets(linha,MAXLINE-1,descritores) == (char *) NULL)
          error_msg(incorrect_descriptor);

        ptr_linha = strstr(linha,"N*"); /* verifica se uma expressao regular */
       }

     switch(criterio)
          {
          case PUSOS:
          case USOS:
             ptr_linha = find_desc(linha,"para o par",descritores);           
            break;
            
          case DU:
            ptr_linha = find_desc(linha,"relativos ao",descritores);
            break;
            
          default:
            error_msg(invalid_criterion);
            break;
          }
     
   }

 /* faz os ponteiros passados por referencia apontarem para as listas criadas */

 *ptr_pu = pu_start->next;

 /* Destroi vetores de bits auxiliares */
 
 b_vector_destroy(&Ni_aux);
 b_vector_destroy(&Nt_aux);

 /* libera o espaco ocupado pelos registros AUTOMATO's auxiliares */

 free(pu_start);
}


/*
** End of aval4.c
*/






