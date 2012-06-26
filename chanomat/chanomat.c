/*
// chanomat.c 	%I%	%Y%	%E%	%U%
*/

#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef TURBO_C
#include <malloc.h>
#else
#include <alloc.h>
#endif

#include "stack.h"
#include "chanomat.h"

/* Declaracao de Variaveis Globais */

int sem_metricas=0; /* flag que controla impressao das metricas */
int num_no; /* numero do no' corrente */
int no_vazio; /* flag que indica se existe um no vazio */
long int chave_pos;
char last_label[25]; /* contem o ultimo simbolo lido */
char mat[MATRIXSIZE][MATRIXSIZE];

 GOTOLABEL * lista_goto = (GOTOLABEL *) NULL,
           *  lista_label = (GOTOLABEL *) NULL,
           *  lista_resul = (GOTOLABEL *) NULL,
           *  aux_lista = (GOTOLABEL *) NULL;
/*******************************************************************/
/* void program(FILE *, FILE *)                                    */
/* Autor: Marcos L. Chaim                                          */
/* Versao: 1.0                                                     */
/* Data: 4/07/89                                                   */
/*                                                                 */
/* FUNCAO: faz a analise sintatica da linguagem intermediaria (li).*/
/*                                                                 */
/* ENTRADA:arquivo que contem a linguagem intermediaria.           */
/*                                                                 */
/* SAIDA: nenhuma.                                                 */
/*                                                                 */
/* FUNCOES CHAMADAS: error(), getsymli() parserg() e statement().  */
/*                                                                 */
/*******************************************************************/

void program(arqli,arqnewli)
FILE *arqli;
FILE *arqnewli;
{

 /* Declaracao de Variaveis Locais */

 struct symbol newsymbol, *pnewsymbol;
 STACK liga_no;
 STACK pilha_break, pilha_return, pilha_continue;

/* Variaveis Auxiliares */

 FILE * gfc;
 int i, j, num_arcos;  
 char nome[1000], nome_gfc[1000];
 
 pnewsymbol = &newsymbol;

 /* Program */
do {

    num_no = 0;                      /* inicia o no' corrente */
    num_arcos = 0;                   /* inicia numero de arcos */
    no_vazio = TRUE;                 /* inicia flag no_vazio */

    strcpy(last_label,"          "); /* inicia last_label */
    mk_nl_int(&liga_no);
    mk_nl_int(&pilha_break);
    mk_nl_int(&pilha_continue);      /* inicia pilhas */
    mk_nl_int(&pilha_return);

    for(i=0; i< MATRIXSIZE; i++)           /* inicia matriz */
      for(j=0; j< MATRIXSIZE; j++)
           mat[i][j] = '.';

    getsymli(pnewsymbol,arqli);      /* pega simbolo */

    while(!strcmp(pnewsymbol->simbolo,"$DCL") ||
          !strcmp(pnewsymbol->simbolo,"$S") ||
          !strcmp(pnewsymbol->simbolo,"$INL") ||
          !strncmp(pnewsymbol->simbolo,"#",1))
          
        {
        pnewsymbol->no =  num_no;
        copia_sym_li(pnewsymbol,arqnewli);
        getsymli(pnewsymbol,arqli);
        }

    if(strncmp(pnewsymbol->simbolo,"@",1))
        error("* * Erro Fatal: Era esperado um '@<nome da funcao>' para iniciar uma funcao na LI\n");
     num_no++;

     strcpy(nome,(pnewsymbol->simbolo+1));

     pnewsymbol->no =  num_no;
     copia_sym_li(pnewsymbol,arqnewli);
     getsymli(pnewsymbol,arqli);

    while(!strcmp(pnewsymbol->simbolo,"$DCL") ||
          !strncmp(pnewsymbol->simbolo,"#",1))
        {
        pnewsymbol->no =  num_no;
        copia_sym_li(pnewsymbol,arqnewli);
        getsymli(pnewsymbol,arqli);
        }

    if(strcmp(pnewsymbol->simbolo,"{"))
        error("* * Erro Fatal: Era esperado um '{' para iniciar o programa da LI\n");

     pnewsymbol->no =  num_no;
     copia_sym_li(pnewsymbol,arqnewli);
     getsymli(pnewsymbol,arqli);

     while(strcmp(pnewsymbol->simbolo,"}"))
        statement(pnewsymbol,arqli,arqnewli,&liga_no,&pilha_break,
            &pilha_continue,&pilha_return,&lista_goto,&lista_label);

      liga_goto(lista_goto, lista_label);
      if(!empty_int(&pilha_return) && !no_vazio)
          {
           ++num_no;
           liga_int(&liga_no,num_no);
          }

       liga_int(&pilha_return, num_no);

       if(strcmp(pnewsymbol->simbolo,"}"))
           error("* * Erro Fatal: Era esperado um '}' para encerrar o programa da LI\n");


        pnewsymbol->no =  num_no;
        copia_sym_li(pnewsymbol,arqnewli);

        if(!empty_int(&pilha_continue) || !empty_int(&pilha_break))
                  printf("** Erro: uso indevido de \"break\" ou \"continue\"\n");


        strcpy(nome_gfc,nome);
        strcat(nome_gfc,".gfc");
        gfc = fopen(nome_gfc, "w");

       if( gfc == (FILE *) NULL)
            error("Erro Fatal: Nao consegui abrir arquivo que contem o grafo \n");

       fprintf(gfc,"%d\n\n",num_no);
       
       for(i=1; i<= num_no; i++)
         {
          fprintf(gfc,"%d\n   ",i);

          for(j=1; j<= num_no; j++)
          if(mat[i][j] == '1')
              {
	       fprintf(gfc,"%d ",j);
	       ++num_arcos;
	      }   

          fprintf(gfc,"0\n");
         }

        insert_list(nome,num_arcos-num_no+2,&lista_resul);
	clear_list(lista_goto);
	clear_list(lista_label);
  	lista_goto = (GOTOLABEL *) NULL,
        lista_label = (GOTOLABEL *) NULL;

        fclose(gfc);
   }
while(!feof(arqli));

if(!sem_metricas)
   {
   printf("-------------------------------------------\n");
   printf("| Nome da Funcao                 | McCabe |\n");
   printf("-------------------------------------------\n");
   for(aux_lista=lista_resul; aux_lista!=NULL;aux_lista = aux_lista->next)
	{
	printf("|%-32s| %5d  |\n",aux_lista->name, aux_lista->id);
	printf("-------------------------------------------\n");
	}
   }

clear_list(lista_resul);
}

/*********************************************************************/
/* void statement(struct symbol *, FILE *, FILE *)                   */
/* Autor:  Marcos L. Chaim & Jose Carlos Maldonado                   */
/* Versao: 2.0                                                       */
/* Data: 6/7/90 (4/07/89)                                            */
/*                                                                   */
/* FUNCAO: faz a analise sintatica de um statement da linguagem in-  */
/*         termediaria.                                              */
/*                                                                   */
/* ENTRADA:apontador para uma estrutura que contem um simbolo da li  */
/*         apontador para o arquivo que contem a li e apontador do   */
/*         arquivo que contem a nova li.                             */
/*                                                                   */
/* SAIDA:  nenhuma.                                                  */
/*                                                                   */
/* FUNCOES CHAMADAS: getsymli(), parserli() e statement().           */
/*                                                                   */
/*********************************************************************/

void statement(pnewsymbol,arqli, arqnewli,p_liga_no,p_break,
              p_continue,p_return, l_goto, l_label)
struct symbol *pnewsymbol;
FILE *arqli;
FILE *arqnewli;
STACK * p_liga_no;
STACK * p_break;
STACK * p_continue;
STACK * p_return;
GOTOLABEL ** l_goto;
GOTOLABEL ** l_label;
{

 /* Declaracao de Variaveis Locais */

 int no_cond, no_incremento;
 long int atual_pos, marcada_pos;

 if(empty_int(p_liga_no))
    push_int(num_no,p_liga_no);

 if(!strcmp(pnewsymbol->simbolo,"{"))
      {
       pnewsymbol->no =  num_no;
       copia_sym_li(pnewsymbol,arqnewli);
       getsymli(pnewsymbol,arqli);

       while(strcmp(pnewsymbol->simbolo,"}"))
         {

          statement(pnewsymbol,arqli,arqnewli,p_liga_no,p_break,p_continue,
                    p_return,l_goto,l_label);
         }

       if(!strcmp(last_label,"$BREAK") ||
          !strcmp(last_label,"$CONTINUE") ||
          !strcmp(last_label,"$RETURN") ||
          !strcmp(last_label,"$GOTO"))
             pnewsymbol->no = num_no - 1;
       else
             pnewsymbol->no = num_no;


       chave_pos = ftell(arqnewli);
       copia_sym_li(pnewsymbol,arqnewli);
       getsymli(pnewsymbol,arqli);
       return;
      }

 if(!strncmp(pnewsymbol->simbolo,"#",1))
       {
        pnewsymbol->no = num_no;
        copia_sym_li(pnewsymbol,arqnewli);
	getsymli(pnewsymbol,arqli);
	strcpy(last_label,"");
	return;
      }

 if(!strncmp(pnewsymbol->simbolo,"$S",2))
      {
        pnewsymbol->no = num_no;
        copia_sym_li(pnewsymbol,arqnewli);
        getsymli(pnewsymbol,arqli);
	no_vazio = FALSE;
	strcpy(last_label,"$S");
	return;
       }
 if(!strcmp(pnewsymbol->simbolo,"$DCL"))
       {
        pnewsymbol->no = num_no;
        copia_sym_li(pnewsymbol,arqnewli);
	getsymli(pnewsymbol,arqli);
	strcpy(last_label,"$DCL");
	return;
      }
 if(!strcmp(pnewsymbol->simbolo,"$IF"))
      {
       STACK p_aux_break, p_aux_continue;
       STACK liga_no_aux;

        mk_nl_int(&p_aux_break);
        mk_nl_int(&p_aux_continue);
        mk_nl_int(&liga_no_aux);

       pnewsymbol->no = num_no;
       copia_sym_li(pnewsymbol,arqnewli);

       no_cond = num_no;

       getsymli(pnewsymbol,arqli);
       if(!strncmp(pnewsymbol->simbolo,"$C",2) ||
          !strncmp(pnewsymbol->simbolo,"$NC",3))
	 {
          pnewsymbol->no = num_no;
          copia_sym_li(pnewsymbol,arqnewli);
          getsymli(pnewsymbol,arqli);

          ++num_no;
          no_vazio = TRUE;

          liga_int(p_liga_no,num_no);
          push_int(num_no,p_liga_no);

          if(!strcmp(pnewsymbol->simbolo,"{"))
             statement(pnewsymbol,arqli,arqnewli,p_liga_no,&p_aux_break,
                       &p_aux_continue,p_return,l_goto,l_label);
          else {
                insere_abre_chave(arqnewli);
                statement(pnewsymbol,arqli,arqnewli,p_liga_no,&p_aux_break,
                           &p_aux_continue,p_return,l_goto,l_label);
                insere_fecha_chave(arqnewli);
               }

          if(car_int(p_liga_no) > 1)
            {
             ++num_no;
             no_vazio = FALSE;
             liga_int(p_liga_no,num_no);
             push_int(num_no,p_liga_no);
             /* atualiza arquivo com novo numero */
            }

	  if(!strcmp(pnewsymbol->simbolo,"$ELSE"))
	    {
            if(!(!strcmp(last_label,"$BREAK") ||
               !strcmp(last_label,"$CONTINUE") ||
               !strcmp(last_label,"$RETURN") ||
               !strcmp(last_label,"$GOTO")))
               {
                ++num_no;
                no_vazio = TRUE;
               }
             strcpy(last_label,"        ");
             push_int(no_cond,&liga_no_aux);
             liga_int(&liga_no_aux,num_no);
             push_int(num_no,&liga_no_aux);

             pnewsymbol->no = num_no;

             copia_sym_li(pnewsymbol,arqnewli);
             getsymli(pnewsymbol,arqli);

	     if(!strcmp(pnewsymbol->simbolo,"{"))
                statement(pnewsymbol,arqli,arqnewli,&liga_no_aux,&p_aux_break,
                           &p_aux_continue,p_return,l_goto,l_label);
             else {
                   insere_abre_chave(arqnewli);
                   statement(pnewsymbol,arqli,arqnewli,&liga_no_aux,&p_aux_break,
                           &p_aux_continue,p_return,l_goto,l_label);
                   insere_fecha_chave(arqnewli);
                  }
            if(car_int(&liga_no_aux) > 1)
             {
             ++num_no;
             no_vazio = FALSE;
             liga_int(&liga_no_aux,num_no);
             push_int(num_no,&liga_no_aux);
             /* atualiza arquivo com novo numero */
            }
           cat_int(p_liga_no,&liga_no_aux);

	    }
         else
               push_int(no_cond,p_liga_no);
       }
	 else
	   error("* * Erro Fatal: `Cnnnn' estava sendo esperado depois do `if' na li * *            \r");
/*         if(!empty_int(p_liga_no))
           {                        */
            if(!(!strcmp(last_label,"$BREAK") ||
                 !strcmp(last_label,"$CONTINUE") ||
                 !strcmp(last_label,"$RETURN") ||
                 !strcmp(last_label,"$GOTO")))
             {
              ++num_no;
              no_vazio = TRUE;
            }
	    strcpy(last_label,"$IF"); /* Era cadeia de brancos */
            liga_int(p_liga_no,num_no);
            push_int(num_no,p_liga_no);
        /*   } */
         cat_int(p_break,&p_aux_break);
         cat_int(p_continue, &p_aux_continue);
	 return;
      }
 if(!strcmp(pnewsymbol->simbolo,"$WHILE"))
      {
       if(!no_vazio || num_no == 1)
       {
        ++num_no;
        liga_int(p_liga_no,num_no);
        push_int(num_no,p_liga_no);
       }
       no_cond = num_no;

       pnewsymbol->no = num_no;

       copia_sym_li(pnewsymbol,arqnewli);
       getsymli(pnewsymbol,arqli);
       if(!strncmp(pnewsymbol->simbolo,"$C",2) ||
	  !strncmp(pnewsymbol->simbolo,"$NC",3))
	 {
          STACK p_aux_break, p_aux_continue;
          STACK liga_no_aux;

          mk_nl_int(&p_aux_break);
          mk_nl_int(&p_aux_continue);
          mk_nl_int(&liga_no_aux);

          pnewsymbol->no = num_no;

          copia_sym_li(pnewsymbol,arqnewli);
          getsymli(pnewsymbol,arqli);

          ++num_no;
          no_vazio = TRUE;
          liga_int(p_liga_no,num_no);

          push_int(num_no,p_liga_no);

          if(!strcmp(pnewsymbol->simbolo,"{"))
             statement(pnewsymbol,arqli,arqnewli,p_liga_no,&p_aux_break,
                       &p_aux_continue,p_return,l_goto,l_label);
          else
            {
             insere_abre_chave(arqnewli);
             statement(pnewsymbol,arqli,arqnewli,p_liga_no,&p_aux_break,
                       &p_aux_continue,p_return,l_goto,l_label);
             chave_pos = ftell(arqnewli);
             insere_fecha_chave(arqnewli);
            }
            if(car_int(p_liga_no) > 1)
             {
             ++num_no;
             no_vazio = FALSE;
             liga_int(p_liga_no,num_no);
             push_int(num_no,p_liga_no);
             /* atualiza arquivo com novo numero */
            }
           if(!empty_int(&p_aux_continue))
            {
             if(!no_vazio)
              {
               ++num_no;
               no_vazio = FALSE;
               liga_int(p_liga_no,num_no);
              }

            push_int(num_no,p_liga_no);
            liga_int(&p_aux_continue,num_no);
            strcpy(last_label,"          ");
            }
           if(empty_int(p_liga_no))
            {
             push_int(num_no,p_liga_no);
             strcpy(last_label,"          ");
            }
           atual_pos = ftell(arqnewli);
           atualiza_chave(arqnewli,atual_pos,chave_pos,num_no);
           liga_int(p_liga_no,no_cond);
           if(!(!strcmp(last_label,"$BREAK") ||
              !strcmp(last_label,"$CONTINUE") ||
              !strcmp(last_label,"$RETURN") ||
              !strcmp(last_label,"$GOTO")))
                   ++num_no;

           strcpy(last_label,"        ");

           no_vazio = TRUE;

           push_int(no_cond,p_liga_no);
           liga_int(p_liga_no,num_no);
           liga_int(&p_aux_break,num_no);
     /*      cat_int(p_liga_no,&p_aux_break); os elementos de p_aux_break tem
                                              num_stack.coor = TRUE */

           push_int(num_no,p_liga_no);
          return;
         }
       else
	  error("* * Erro Fatal: `Cnnnn' estava sendo esperado depois do `while' na li * *                     \r");
      }
 if(!strcmp(pnewsymbol->simbolo,"$REPEAT"))
      {
      STACK p_aux_break, p_aux_continue;
      STACK liga_no_aux;

      mk_nl_int(&p_aux_break);
      mk_nl_int(&p_aux_continue);
      mk_nl_int(&liga_no_aux);

      if(!no_vazio || num_no == 1)
        {
         ++num_no;
         no_vazio = TRUE;
         liga_int(p_liga_no,num_no);
         push_int(num_no,p_liga_no);
        }

      no_cond = num_no;

      pnewsymbol->no = num_no;

      copia_sym_li(pnewsymbol,arqnewli);
      getsymli(pnewsymbol,arqli);
      if(!strcmp(pnewsymbol->simbolo,"{"))
          statement(pnewsymbol,arqli,arqnewli,p_liga_no,&p_aux_break,
                    &p_aux_continue,p_return,l_goto,l_label);
      else
          {
           insere_abre_chave(arqnewli);
           statement(pnewsymbol,arqli,arqnewli,p_liga_no,&p_aux_break,
                    &p_aux_continue,p_return,l_goto,l_label);
           insere_fecha_chave(arqnewli);
          }
      if(car_int(p_liga_no) > 1)
             {
             ++num_no;
             no_vazio = TRUE;
             liga_int(p_liga_no,num_no);
             push_int(num_no,p_liga_no);
             /* atualiza arquivo */
            }

      if(!strcmp(pnewsymbol->simbolo,"$UNTIL"))
	 {
         if(!no_vazio)
           {
            ++num_no;
            no_vazio = FALSE;
            liga_int(p_liga_no,num_no);
           }
       push_int(num_no,p_liga_no);
       no_vazio = FALSE;
       liga_int(&p_aux_continue,num_no);
       liga_int(p_liga_no,no_cond);
       no_cond = num_no;

       pnewsymbol->no = num_no;
       copia_sym_li(pnewsymbol,arqnewli);
       getsymli(pnewsymbol,arqli);
       if(!strncmp(pnewsymbol->simbolo,"$NC",3) ||
	  !strncmp(pnewsymbol->simbolo,"$C",2))
	       {
                pnewsymbol->no = num_no;
                copia_sym_li(pnewsymbol,arqnewli);
                getsymli(pnewsymbol,arqli);

                ++num_no;
                no_vazio = TRUE;

                push_int(no_cond,p_liga_no);
                liga_int(p_liga_no,num_no);
                push_int(num_no,p_liga_no);
                liga_int(&p_aux_break,num_no);

                /* cat_int(p_liga_no,&p_aux_break); os elementos de p_aux_break tem
                                              num_stack.coor = TRUE */
		return;
	       }
	     else
		 error("* * Erro Fatal: `NCnnnn' ou `Cnnnn' estava sendo esperado depois do `until' na li * *                    \r");
	  }
	 else
	    error("* * Erro Fatal: `until' estava sendo esperado depois do `repeat' na li * *                  \r");
      }
 if(!strcmp(pnewsymbol->simbolo,"$FOR"))
	{
         STACK p_aux_break, p_aux_continue;
         STACK liga_no_aux;

         mk_nl_int(&p_aux_break);
         mk_nl_int(&p_aux_continue);
         mk_nl_int(&liga_no_aux);

         pnewsymbol->no = num_no;

         copia_sym_li(pnewsymbol,arqnewli);
         getsymli(pnewsymbol,arqli);
	 if(!strncmp(pnewsymbol->simbolo,"$S",2))
	       {
                pnewsymbol->no = num_no;
                copia_sym_li(pnewsymbol,arqnewli);
                getsymli(pnewsymbol,arqli);
		if(!strncmp(pnewsymbol->simbolo,"$C",2) ||
		   !strncmp(pnewsymbol->simbolo,"$NC",3))
		  {
                   ++num_no;
                   liga_int(p_liga_no,num_no);
                   no_cond = num_no;

                   pnewsymbol->no = num_no;

                   copia_sym_li(pnewsymbol,arqnewli);
                   getsymli(pnewsymbol,arqli);

		   if(!strncmp(pnewsymbol->simbolo,"$S",2))
		     {
                      /* guardar posicao onde atualizar este numero */
                      marcada_pos = ftell(arqnewli);

                      pnewsymbol->no = num_no;

                      copia_sym_li(pnewsymbol,arqnewli);
                      getsymli(pnewsymbol,arqli);

                      ++num_no;
                      no_vazio = TRUE;
                      push_int(no_cond,p_liga_no);
                      liga_int(p_liga_no,num_no);
                      push_int(num_no,p_liga_no);

		      if(!strcmp(pnewsymbol->simbolo,"{"))
                          statement(pnewsymbol,arqli,arqnewli,p_liga_no,&p_aux_break,
                                    &p_aux_continue,p_return,l_goto,l_label);
                      else
                          {
                           insere_abre_chave(arqnewli);
                           statement(pnewsymbol,arqli,arqnewli,p_liga_no,&p_aux_break,
                                    &p_aux_continue,p_return,l_goto,l_label);
                           chave_pos = ftell(arqnewli);
                           insere_fecha_chave(arqnewli);
                          }
/*                       if(!no_vazio)
                         {
                           ++num_no;
                           liga_int(p_liga_no, num_no);
                         }                                */
                       no_incremento = num_no;

                       if(empty_int(&p_aux_continue))
                         {
                          push_int(no_incremento,p_liga_no);
                          liga_int(p_liga_no,no_cond);
                         }
                       else
                         {
                          if(!no_vazio)
                             ++num_no;
                          push_int(no_incremento,p_liga_no);
                          liga_int(p_liga_no,num_no);
                          liga_int(&p_aux_continue,num_no);
                          push_int(num_no,p_liga_no);
                          liga_int(p_liga_no,no_cond);
                         }
                       atual_pos = ftell(arqnewli);
                       atualiza_chave(arqnewli, atual_pos, chave_pos,num_no);
                       atual_pos = ftell(arqnewli);
                       atualiza_chave(arqnewli, atual_pos, marcada_pos,num_no);
                       ++num_no;
                       no_vazio = TRUE;
                       push_int(no_cond,p_liga_no);
                       liga_int(p_liga_no,num_no);
                       liga_int(&p_aux_break,num_no);
                       push_int(num_no,p_liga_no);
                       /* cat_int(p_liga_no,&p_aux_break); os elementos de p_aux_break tem
                                              num_stack.coor = TRUE */
                      }
                    else
		      error("* * Erro Fatal: `Snnnn' estava sendo esperado depois do `for Snnnn Cnnnn' na li * * \r");
		  }
		else
		  error("* * Erro Fatal: `Cnnnn' estava sendo esperado depois do `for Snnnn' na li * *                    \r");
		}
	 else
	    error("* * Erro Fatal: 'Snnnn' estava sendo esperado depois do 'for' na li * *\n");

        return;
	}
 if(!strcmp(pnewsymbol->simbolo,"$CASE"))
      {
       STACK p_aux_break, p_aux_continue;
       STACK liga_no_aux;
       int tem_rotd = FALSE;

       mk_nl_int(&p_aux_break);
       mk_nl_int(&p_aux_continue);
       mk_nl_int(&liga_no_aux);

       no_cond = num_no;

       pnewsymbol->no = num_no;
       copia_sym_li(pnewsymbol,arqnewli);
       getsymli(pnewsymbol,arqli);
       strcpy(last_label,"        ");

       if(!strcmp(pnewsymbol->simbolo,"$CC"))
	 {
          no_vazio = FALSE;
          pnewsymbol->no = num_no;
          copia_sym_li(pnewsymbol,arqnewli);
          getsymli(pnewsymbol,arqli);
	  if(!strcmp(pnewsymbol->simbolo,"{"))
             {
             pnewsymbol->no = num_no;
             copia_sym_li(pnewsymbol,arqnewli);
             getsymli(pnewsymbol,arqli);
             }
	  else
	     error("* * Erro Fatal: `{' estava sendo esperado na li * *\n");
          do {
              if(!(!strcmp(last_label,"$BREAK") ||
                 !strcmp(last_label,"$CONTINUE") ||
                 !strcmp(last_label,"$RETURN") ||
                 !strcmp(last_label,"$GOTO")))
                  {
                  ++num_no;
                  no_vazio = TRUE;
                  }
              strcpy(last_label,"        ");

              liga_int(p_liga_no,num_no);
              push_int(no_cond,p_liga_no);
              liga_int(p_liga_no,num_no);
              push_int(num_no,p_liga_no);

              if(!strcmp(pnewsymbol->simbolo,"$ROTC") ||
                 !strcmp(pnewsymbol->simbolo,"$ROTD"))
                 {
                 if(!strcmp(pnewsymbol->simbolo,"$ROTD"))
                    tem_rotd = TRUE;
                 pnewsymbol->no = num_no;
                 copia_sym_li(pnewsymbol,arqnewli);
                 getsymli(pnewsymbol,arqli);
                 if(strcmp(pnewsymbol->simbolo,"{"))
                      {
                       insere_abre_chave(arqnewli);
                       while (strcmp(pnewsymbol->simbolo,"$ROTC") &&
                              strcmp(pnewsymbol->simbolo,"$ROTD") &&
                              strcmp(pnewsymbol->simbolo,"}"))
                                statement(pnewsymbol,arqli,arqnewli,p_liga_no,&p_aux_break,
                                          &p_aux_continue,p_return,l_goto,l_label);
                       insere_fecha_chave(arqnewli);
                      }
                 else while (strcmp(pnewsymbol->simbolo,"$ROTC") &&
                              strcmp(pnewsymbol->simbolo,"$ROTD") &&
                              strcmp(pnewsymbol->simbolo,"}"))
                                statement(pnewsymbol,arqli,arqnewli,p_liga_no,&p_aux_break,
                                          &p_aux_continue,p_return,l_goto,l_label);
                }
              else
                error("* * Erro Fatal: Estava esperando um 'ROTC' ou 'ROTD'\n");
             }
          while(strcmp(pnewsymbol->simbolo,"}"));

          if(!(!strcmp(last_label,"$BREAK") ||
               !strcmp(last_label,"$CONTINUE") ||
               !strcmp(last_label,"$RETURN") ||
               !strcmp(last_label,"$GOTO")))
                  {
                   no_vazio = TRUE;
                   ++num_no;
                  }
          strcpy(last_label,"        ");
          liga_int(p_liga_no,num_no);
          liga_int(&p_aux_break, num_no);
          if(!tem_rotd)
             {
              push_int(no_cond,p_liga_no);
              liga_int(p_liga_no,num_no);
             }

          push_int(num_no,p_liga_no);
          cat_int(p_continue,&p_aux_continue);


          pnewsymbol->no = num_no;
          copia_sym_li(pnewsymbol,arqnewli);
          getsymli(pnewsymbol,arqli);
          return;
	  }
	  else
            error("* * Erro Fatal: `CCnnnn' estava sendo esperado depois do `case' na li * *\n");
       }
 if(!strcmp(pnewsymbol->simbolo,"$BREAK"))
   {
    strcpy(last_label,pnewsymbol->simbolo);
    push_int(num_no,p_break);

    pnewsymbol->no = num_no;

    copia_sym_li(pnewsymbol,arqnewli);
    getsymli(pnewsymbol,arqli);

    no_vazio = TRUE;
    ++num_no;
    mk_nl_int(p_liga_no);

    return;
   }
 if(!strcmp(pnewsymbol->simbolo,"$RETURN"))
   {
    strcpy(last_label,pnewsymbol->simbolo);
    push_int(num_no,p_return);

    pnewsymbol->no = num_no;

    copia_sym_li(pnewsymbol,arqnewli);
    getsymli(pnewsymbol,arqli);

    no_vazio = TRUE;
    ++num_no;
    mk_nl_int(p_liga_no);

    return;
   }
 if(!strcmp(pnewsymbol->simbolo,"$CONTINUE"))
   {
    strcpy(last_label,pnewsymbol->simbolo);
    push_int(num_no,p_continue);

    pnewsymbol->no = num_no;

    copia_sym_li(pnewsymbol,arqnewli);
    getsymli(pnewsymbol,arqli);

    no_vazio = TRUE;
    ++num_no;
    mk_nl_int(p_liga_no);

    return;
   }
 if(!strcmp(pnewsymbol->simbolo,"$GOTO"))
   {
    strcpy(last_label,pnewsymbol->simbolo);
    pnewsymbol->no = num_no;

    copia_sym_li(pnewsymbol,arqnewli);
    getsymli(pnewsymbol,arqli);
    insert_list(pnewsymbol->simbolo,num_no,l_goto);

    pnewsymbol->no = num_no;
    copia_sym_li(pnewsymbol,arqnewli);

    getsymli(pnewsymbol,arqli);

    mk_nl_int(p_liga_no);
    no_vazio = TRUE;
    ++num_no;
    return;
  }
if(!(!strcmp(last_label,"$BREAK") ||
     !strcmp(last_label,"$LABEL") ||
     !strcmp(last_label,"$CONTINUE") ||
     !strcmp(last_label,"$RETURN") ||
     !strcmp(last_label,"$GOTO")))
  {
   ++num_no;
/* no_vazio = TRUE; */  /* Manut. 01 */
   liga_int(p_liga_no, num_no);
   push_int(num_no, p_liga_no);
  }
 no_vazio = FALSE; /* Manut. 01 */
 insert_list(pnewsymbol->simbolo,num_no,l_label);

 strcpy(last_label,"$LABEL"); /* Indica em last_label que ja' ocorreu um
				 label */

/* if(top_int(p_liga_no) != num_no)
    {
     liga_int(p_liga_no, num_no);
     push_int(num_no, p_liga_no);
    }
*/
 pnewsymbol->no = num_no;
 copia_sym_li(pnewsymbol,arqnewli);
 getsymli(pnewsymbol,arqli);
 return;
}

/********************************************************************/
/* void getsymli(struct symbol *, FILE *)                           */
/* Autor: Marcos L. Chaim                                           */
/* Versao: 1.0                                                      */
/* Data: 4/07/89                                                    */
/*                                                                  */
/* FUNCAO: busca um simbolo da linguagem intermediaria e coloca na  */
/*         estrutura tipo symbol.                                   */
/*                                                                  */
/* ENTRADA:apontador para estrutura do tipo symbol e apontador o    */
/*         arquivo aberto contendo a li.                            */
/*                                                                  */
/* SAIDA:  nenhuma.                                                 */
/*                                                                  */
/********************************************************************/

void getsymli(pnewsymbol,arq)
struct symbol *pnewsymbol;
FILE *arq;
{
static int flag = FALSE;
static int flag_eof = FALSE;
static struct symbol nextsymbol, *pnextsymbol;

pnextsymbol = &nextsymbol;
if(!flag)
   {
     if(!feof(arq))
 	fscanf(arq,"%s %ld %ld %ld",
	pnewsymbol->simbolo,&pnewsymbol->inicio,
                            &pnewsymbol->comprimento,
                            &pnewsymbol->linha);
      else
      	error("Erro de Leitura do arquivo contendo a LI\n");

     if(!feof(arq))
	fscanf(arq,"%s %ld %ld %ld",
	pnextsymbol->simbolo,&pnextsymbol->inicio,
                            &pnextsymbol->comprimento,
                            &pnextsymbol->linha);
      else
	error("* * Erro Fatal: Erro na Leitura do arquivo contendo a LI * *\n");
     flag = TRUE;
    }
 else
    {
      if(!flag_eof)
	*pnewsymbol = *pnextsymbol;
      else
 	error("* * Erro Fatal: Erro na Leitura do arquivo contendo a LI * *\n");

      if(!feof(arq))
	fscanf(arq,"%s %ld %ld %ld",
	pnextsymbol->simbolo,&pnextsymbol->inicio,
                            &pnextsymbol->comprimento,
                            &pnextsymbol->linha);
      else
	flag_eof = TRUE;
     }
/*
 if((pnewsymbol->inicio+pnewsymbol->comprimento-1)==pnextsymbol->inicio)
    --pnewsymbol->comprimento;
*/
}

void insere_abre_chave(arq)
FILE *arq;
{

 fprintf(arq,"%-20s %6d %6d %6d %6d\n","{ ",num_no,0,0,0);

 return;
}

void insere_fecha_chave(arq)
FILE *arq;
{

int numero;

if(!strcmp(last_label,"$BREAK") ||
   !strcmp(last_label,"$CONTINUE") ||
   !strcmp(last_label,"$RETURN") ||
   !strcmp(last_label,"$GOTO"))
      numero = num_no - 1;
else
      numero = num_no;

 fprintf(arq,"%-20s %6d %6d %6d %6d\n","} ",numero,0,0,0);
 return;
}
/*
void wf_insere_fecha_chave(arq)
FILE *arq;
{

 fprintf(arq,"%-20s %6d %6d %6d\n","} ",0,0,0);

 return;
} */

void  copia_sym_li(pnewsymbol,arq)
struct symbol *pnewsymbol;
FILE *arq;
{
 fprintf(arq,"%-20s %6d %6ld %6ld %6ld\n",
	pnewsymbol->simbolo,pnewsymbol->no,
                            pnewsymbol->inicio,
                            pnewsymbol->comprimento,
                            pnewsymbol->linha);
/* fprintf(stderr,"%-20s %6d %6ld %6ld %6ld\n",
	pnewsymbol->simbolo,pnewsymbol->no,
                            pnewsymbol->inicio,
                            pnewsymbol->comprimento,
                            pnewsymbol->linha);
        */
 return;
}

void error(msg)
char msg[200];
{
 fprintf(stderr,"%s",msg);
 clear_list(lista_goto);
 clear_list(lista_label);
 clear_list(lista_resul);
 exit(1);
}

/* unsigned _stklen = 0xC000; */

void main(argc, argv)
int argc;
char * argv[];
{
char nome[NCARMAX];
FILE * li, * newli;
int i,j;

if(argv[1] != NULL)
	if(!strcmp(argv[1],"-t"))
		sem_metricas=1;

if(argc != 2+sem_metricas)
 	{
	 printf("uso: newchanomat <nome do arquivo>\n");
	 exit(1);
	}

if(!sem_metricas)
  {
   printf("chanomat 2.0 -- Gerador do Grafo de Fluxo de Controle para Programas em LI \n");
   printf("(C) Copyright 1992 Grupo de Teste Software DCA/FEE/UNICAMP\n");
  }
strcpy(nome, argv[1+sem_metricas]);
strcat(nome,".li");

li = (FILE *) fopen(nome, "r");

if( li == (FILE *) NULL)
    error("Erro Fatal: Nao consegui abrir arquivo que contem a LI \n");

strcpy(nome, argv[1+sem_metricas]);
strcat(nome,".nli");

newli =(FILE *) fopen(nome,"w+");

if( newli == (FILE *) NULL)
    error("Erro Fatal: Nao consegui abrir arquivo que contem a nova LI \n");

program(li, newli);

fclose(li);
fclose(newli);
exit(0);
}

