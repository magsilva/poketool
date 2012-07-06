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
// parserl4.c 	%I%	%Y%	%E%	%U%
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
#include "tabelasli.h"
#include "hrotsem.h"
#include "header.h"
#include "hmonitor.h"
#include "newpokelib.h"

/* Declaracao de Variaveis Globais */

extern FILE * arqfonte;
extern FILE * arqparser;
extern FILE * mod_arq;

extern struct cabeca * list, * hdeclaration,
		     * hstatement, * hexpression;
extern struct tableout  saida;
extern NODEINFO info_no;
extern b_vector undef;
extern b_vector flags;
extern DESPOINTER names;
extern STACK stack_loop;
extern STACK stack_break;
extern STACK stack_break_int;
extern STACK type_use;

extern char oldlabel[NCARMAX+1];
extern long inicio, comprimento, linha;
extern int num_no;
extern int tab_counter;

/* Declaracao de Variaveis locais */

int no_nos;
int break_counter=0;

extern table_element * pvarname;
struct grafo * graph;
static char nome [100];

/* Funcoes Gerais */

void set_nome (func_nome)
char * func_nome;
{
 strcpy(nome,func_nome);  
}

char * get_nome(void)
{
  return nome;
}

static int stdio=0;

void check_stdio(str)
char * str;
{
 
 if(!stdio)
   {
   str=strstr(str,"stdio");
   stdio = (str != NULL)?TRUE:FALSE;
   }

}

int get_stdio(void)
{
  return stdio;
}

/* Funcoes para Depuracao */

void print_sets ()
{
  int i=num_no;

     int j;
     fprintf(stdout,"\nNO' %2d ",i);
     fprintf(stdout,"\nVars defs: ");
     for(j=0; j>=0 && j <= NMAXVAR; ++j)
		if(test_bit(j,&(info_no[i].defg_i)))
		   print_names(stdout,pvarname,j);
     
     fprintf(stdout,"\nVars C-Usos: ");
     for(j=0; j>=0 && j <= NMAXVAR; ++j)
		if(test_bit(j,&(info_no[i].c_use)))
		   print_names(stdout,pvarname,j);

     fprintf(stdout,"\nVars P-Usos: ");
     for(j=0; j>=0 && j <= NMAXVAR; ++j)
		if(test_bit(j,&(info_no[i].p_use)))
		   print_names(stdout,pvarname,j);
     
     fprintf(stdout,"\nVars refs: ");
     for(j=0; j>=0 && j <= NMAXVAR; ++j)
		if(test_bit(j,&(info_no[i].def_ref)))
		   print_names(stdout,pvarname,j);

     fprintf(stdout,"\nVars undefs: ");
     for(j=0; j>=0 && j <= NMAXVAR; ++j)
		if(!test_bit(j,&(info_no[i].undef)))
		   print_names(stdout,pvarname,j);
   
printf("Digite <ENTER> para continuar..."); getchar();

}


/*******************************************************************/
/* void parserli(FILE *, FILE *)                                   */
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
/* FUNCOES CHAMADAS: error(), getsymli() parserg(),copy_command()  */
/*                   e statement().                                */
/*                                                                 */
/*******************************************************************/

void parserli(arqli)
FILE *arqli;
{

 /* Declaracao de Variaveis Locais */

 struct symbol newsymbol, oldsymbol, *pnewsymbol, *poldsymbol;
 int ja_fechei_paths = FALSE;
 int ja_inclui_poketoolh = FALSE;

 char gfc[200];  /* nome do arquivo que contem o GFC */

 pnewsymbol = &newsymbol;
 poldsymbol = &oldsymbol;

 msg_print("* * Fazendo a analise sintatica do codigo fonte... * *\n");

 /* Program */

 
 getsymli(pnewsymbol,arqli);    /* pega simbolo */
 while(!feof(arqli))
	{
 	while(!strcmp(pnewsymbol->simbolo,"$DCL") ||
       	      !strncmp(pnewsymbol->simbolo,"$S",2) ||
       	      !strcmp(pnewsymbol->simbolo,"$INL") ||
              !strncmp(pnewsymbol->simbolo,"#",1))
   		{
                 if(strncmp(pnewsymbol->simbolo,"#",1)) /* != de # */
                   {
                   parserg(pnewsymbol);    /* faz analise sintatica especifica 
					    da linguagem */
    		   copy_command(pnewsymbol);
                   
    		   fprintf(mod_arq,"\n");
                   }
                 else
                   check_stdio(pnewsymbol->simbolo);

    		 strcpy(poldsymbol->simbolo,pnewsymbol->simbolo);
    		 getsymli(pnewsymbol,arqli);
   		}

	if(!ja_inclui_poketoolh)
                {
		inicia_monitoracao();       /*   inicia o arquivo modificado com "includes" */
		ja_inclui_poketoolh = TRUE;
		}

    	if(strncmp(pnewsymbol->simbolo,"@",1))
        	error("* * Erro Fatal: Era esperado um '@<nome da funcao>' para iniciar uma funcao na LI\n");

     	
        set_nome((pnewsymbol->simbolo+1));

    	getsymli(pnewsymbol,arqli);
	if(is_a_func(get_nome()))
		{
		/* Pega o nome da unidade */

		ja_fechei_paths = FALSE;
		strcpy(gfc,func_cur());
		strcat(gfc,".gfc");

                /* Ajusta tamanho dos vetores de bits */

                info_no_b_vector_ajust();
                
 		info_no_in();
 		trata_in_blk();

                /* Indica Inicio de Declaracao de Parametros */
                
                set_pars_dcl(TRUE);
                
		/* carrega o gfc da unidade */

		no_nos=inigrf(gfc,&graph);	

    		fprintf(mod_arq,"\n");
 		while(!strcmp(pnewsymbol->simbolo,"$DCL")  ||
       	      	      !strncmp(pnewsymbol->simbolo,"$S",2) ||
                       !strncmp(pnewsymbol->simbolo,"#",1))
   			{
                         if(strncmp(pnewsymbol->simbolo,"#",1)) /* != de # */
                          { 
    		 	   parserg(pnewsymbol);    /* faz analise sintatica 
						    especifica da linguagem */
    		           copy_command(pnewsymbol);
    		 	   fprintf(mod_arq,"\n");
                          }
                         else
                           check_stdio(pnewsymbol->simbolo);
    		 	 strcpy(poldsymbol->simbolo,pnewsymbol->simbolo);
    		 	 getsymli(pnewsymbol,arqli);
   		   	}

 		if(strcmp(pnewsymbol->simbolo,"{"))
   			error("* * Erro Fatal: Era esperado um '{' para iniciar o unidade da LI\n");

                /* Indica fim da Declaracao de Parametros */
                
                set_pars_dcl(FALSE);
                
 		writeIniciador(num_no);
 		/* trata_in_blk(); */
 		meio_monitoracao();   /* escreve os comandos que abrem o arquivo "paths.tes" */

 		getsymli(pnewsymbol,arqli);

 		while(strcmp(pnewsymbol->simbolo,"}"))
   			{
    			if(!strcmp(pnewsymbol->simbolo,"$RETURN"))
          			ja_fechei_paths = TRUE;
    			statement(pnewsymbol,poldsymbol,arqli);
   			}

 		if(strcmp(pnewsymbol->simbolo,"}"))
   			error("* * Erro Fatal: Era esperado um '}' para terminar a funcao da LI\n");
 		trata_fim_blk();
 		if(!ja_fechei_paths)
    			{
     			std_monitor(num_no);
     			finaliza_monitoracao();
    			}

 		writeFinalizador(num_no);

                /* Ajusta novamente tamanho dos vetores de bits */

                info_no_b_vector_ajust();
                                
		/* Realiza as tarefas relativas ao final da analise da funcao */

		gerencia_fim_func(nome);

		/* Desaloca o gfc da unidade */

 		lib_grf(graph,no_nos);

                
		info_no_clear();
 	 	getsymli(pnewsymbol,arqli);

		}
		else  /* Nao precisa ser monitorado. */
		    {
		     int no_par = 0; /* no. de parametros */

    		     fprintf(mod_arq,"\n");
 		     while(!strcmp(pnewsymbol->simbolo,"$DCL") ||
       	      	           !strncmp(pnewsymbol->simbolo,"$S",2)||
                           !strncmp(pnewsymbol->simbolo,"#",1))
   		         {
                         if(strncmp(pnewsymbol->simbolo,"#",1)) /* != de # */
                             {
    		 	      copy_command(pnewsymbol);
    		 	      fprintf(mod_arq,"\n");
                             }
                         else
                           check_stdio(pnewsymbol->simbolo);
 			 getsymli(pnewsymbol,arqli);
			 }

 		     if(strcmp(pnewsymbol->simbolo,"{"))
   			 error("* * Erro Fatal: Era esperado um '{' para iniciar o programa da LI\n");
		     else
			{/* escreve o abre chave inicial */
			ajusta_formato(no_par);   /* formata */
    		 	fprintf(mod_arq,"{\n");
			no_par++;
			}

 		    getsymli(pnewsymbol,arqli);
 		    while(no_par > 0)
			{
                        if(strncmp(pnewsymbol->simbolo,"#",1)) /* != de # */
                         { 
 		         if(!strncmp(pnewsymbol->simbolo,"$DCL",4) ||
			   !strncmp(pnewsymbol->simbolo,"$S",2) ||
			   !strcmp(pnewsymbol->simbolo,"$BREAK") ||
			   !strcmp(pnewsymbol->simbolo,"$CONTINUE") ||
			   !strcmp(pnewsymbol->simbolo,"$ELSE") ||
			   !strcmp(pnewsymbol->simbolo,"$REPEAT")
			   ) 
				{
				ajusta_formato(no_par);   /* formata */
    		 		copy_command(pnewsymbol);
    		 		fprintf(mod_arq,"\n"); 
				}
			 else
				{ /* Todos os comandos seguidos de $C ou Rotulo 				  */
 		        	if(!strcmp(pnewsymbol->simbolo,"$IF"))
					{ /* Copia a condicao do if */
					ajusta_formato(no_par);   /* formata */
    		 			copy_command(pnewsymbol);
				
 					getsymli(pnewsymbol,arqli);
    		 			copy_command(pnewsymbol);
    		 			fprintf(mod_arq,"\n");
					}
				else
 		        	   if(!strcmp(pnewsymbol->simbolo,"$WHILE"))
					{ /* Copia a condicao do while */
					ajusta_formato(no_par);   /* formata */
    		 			copy_command(pnewsymbol);
				
 					getsymli(pnewsymbol,arqli);
    		 			copy_command(pnewsymbol);
    		 			fprintf(mod_arq,"\n");
					}
				   else
 		        	     if(!strcmp(pnewsymbol->simbolo,"$UNTIL"))
					{ /* Copia a condicao do repeat */
					ajusta_formato(no_par);   /* formata */
    		 			copy_command(pnewsymbol);
				
 					getsymli(pnewsymbol,arqli);
    		 			copy_command(pnewsymbol);
    		 			fprintf(mod_arq,"\n");
					}
				     else
 		        		if(!strcmp(pnewsymbol->simbolo,"$FOR"))
					{
					ajusta_formato(no_par);   /* formata */
    		 			copy_command(pnewsymbol);
				
					/* Copia o primeiro $S do For */
    		 			fprintf(mod_arq,"(");

 					getsymli(pnewsymbol,arqli);
    		 			copy_command(pnewsymbol);

					/* Copia o $C do For */
 					getsymli(pnewsymbol,arqli);
    		 			copy_command(pnewsymbol);

					/* Copia o segundo $S do For */
 					getsymli(pnewsymbol,arqli);
    		 			copy_command(pnewsymbol);

    		 			fprintf(mod_arq,")");
    		 			fprintf(mod_arq,"\n");
					}
					else
 		        		   if(!strcmp(pnewsymbol->simbolo,
							   "$GOTO"))
						{
						ajusta_formato(no_par);   
    		 				copy_command(pnewsymbol);
				
						/* Copia o rotulo do Goto */
 						getsymli(pnewsymbol,arqli);
    		 				fprintf(mod_arq," ");
    		 				copy_command(pnewsymbol);
    		 				fprintf(mod_arq,"\n");
						}
					   else
 		        	            if(!strcmp(pnewsymbol->simbolo,"{"))
						{
						no_par++;
						ajusta_formato(no_par);   
    		 				fprintf(mod_arq,"{\n");
						}
					    else
 		        		     if(!strcmp(pnewsymbol->simbolo,"}")) 
						{
						if(no_par == 1)
							{ /* ultimo chave */
							no_par--;
							ajusta_formato(no_par);
    		 					fprintf(mod_arq,"}\n");
							}
						else 
							{ /* chave intermediaria 							  */
							ajusta_formato(no_par);
    		 					fprintf(mod_arq,"}\n");
							no_par--;
							}
						}
					      else /* Rotulo */
						{
						ajusta_formato(no_par);   
    		 				copy_command(pnewsymbol);
    		 				fprintf(mod_arq,"\n");
						}
				}
                         }
                        else
                          check_stdio(pnewsymbol->simbolo);
 			getsymli(pnewsymbol,arqli);
			}

		    }
	}
 
 cria_include(); /* cria arquivo "poketool.h" */
}

/*********************************************************************/
/* void statement(struct symbol *, FILE *)                           */
/* Autor: Marcos L. Chaim                                            */
/* Versao: 1.0                                                       */
/* Data: 4/07/89                                                     */
/*                                                                   */
/* FUNCAO: faz a analise sintatica de um statement da linguagem in-  */
/*         termediaria.                                              */
/*                                                                   */
/* ENTRADA:apontador para uma estrutura que contem um simbolo da li  */
/*         e string do arquivo que contem a li.                      */
/*                                                                   */
/* SAIDA:  nenhuma.                                                  */
/*                                                                   */
/* FUNCOES CHAMADAS: getsymli(), parserli() e statement().           */
/*                                                                   */
/*********************************************************************/

void statement(pnewsymbol,poldsymbol,arqli)
struct symbol *pnewsymbol;
struct symbol *poldsymbol;
FILE *arqli;
{

 /* Declaracao de Variaveis Locais */

 int  num_while, num_for, num_incremento, num_label;
 int num_aux;

 if(!strcmp(pnewsymbol->simbolo,"{"))
      {
       trata_in_blk();          /* trata inicio de bloco */

       writeIniciador(num_no);  /* escreve o iniciador de bloco de comandos */

       strcpy(poldsymbol->simbolo,pnewsymbol->simbolo);

       getsymli(pnewsymbol,arqli);

       while(strcmp(pnewsymbol->simbolo,"}"))
            statement(pnewsymbol,poldsymbol,arqli);

       if(strcmp(pnewsymbol->simbolo,"}"))
	   error("* * Erro Fatal: `}' estava sendo esperado na li * *               \n");

       trata_fim_blk();   /* trata fim de bloco */

       num_aux = num_no; /* salva o numero do no' do fecha chave */

       strcpy(poldsymbol->simbolo,pnewsymbol->simbolo);
       getsymli(pnewsymbol,arqli);             /* pega proximo simbolo */

       if(strcmp(pnewsymbol->simbolo,"$WHILE") && num_no != num_aux)
            fecha_chave_monitor(num_no);  /* monitora o fecha chave se ja nao foi */

       writeFinalizador(num_no);
       return;
      }
 if(!strncmp(pnewsymbol->simbolo,"$S",2))
      {
	if(parserg(pnewsymbol)== STATEMENT)  /* faz analise sintatica especifica */
          std_monitor(num_no);

	write_no(num_no);              /* formata */
	ajusta_formato(tab_counter);   /* formata */

        copy_command(pnewsymbol);    /* escreve o comando no arquivo modificado */

	fprintf(mod_arq,"\n");

        strcpy(poldsymbol->simbolo,pnewsymbol->simbolo);
	getsymli(pnewsymbol,arqli);

	return;
       }
 if(!strcmp(pnewsymbol->simbolo,"$DCL"))
       {
        if(parserg(pnewsymbol)== STATEMENT)  /* faz analise sintatica especifica */
          std_monitor(num_no);

	write_no(num_no);              /* formata */
	ajusta_formato(tab_counter);   /* formata */
        copy_command(pnewsymbol);

	fprintf(mod_arq,"\n");
        strcpy(poldsymbol->simbolo,pnewsymbol->simbolo);
	getsymli(pnewsymbol,arqli);

	return;
      }

 if(!strncmp(pnewsymbol->simbolo,"#",1)) /* == de # */
      {
      check_stdio(pnewsymbol->simbolo);
      strcpy(poldsymbol->simbolo,pnewsymbol->simbolo);
      getsymli(pnewsymbol,arqli);

      return;
      }
 
 if(!strcmp(pnewsymbol->simbolo,"$IF"))
      {
       std_monitor(num_no);
       escreve_md(pnewsymbol,num_no); /* escreve o "if" */

       strcpy(poldsymbol->simbolo,pnewsymbol->simbolo);
       getsymli(pnewsymbol,arqli);
       if(!strncmp(pnewsymbol->simbolo,"$C",2))
	 {
	  parsercond();
          copy_command(pnewsymbol);
	  fprintf(mod_arq,"\n");
	  strcpy(poldsymbol->simbolo,pnewsymbol->simbolo);
	  getsymli(pnewsymbol,arqli);
	  if(!strcmp(pnewsymbol->simbolo,"{"))
	     {
              trata_in_blk();
	      writeIniciador(num_no);
              strcpy(poldsymbol->simbolo,pnewsymbol->simbolo);
	      getsymli(pnewsymbol,arqli);

	      do
		 statement(pnewsymbol,poldsymbol,arqli);
	      while(strcmp(pnewsymbol->simbolo,"}"));  /* diferente de '}' */

	      fecha_chave_monitor(num_no);
	      writeFinalizador(num_no);
	      trata_fim_blk();
              strcpy(poldsymbol->simbolo,pnewsymbol->simbolo);
	      getsymli(pnewsymbol,arqli);
	     }
	  else
            error("Erro Fatal: Era esperado um '}' no bloco de comandos da parte $THEN");

	  if(!strcmp(pnewsymbol->simbolo,"$ELSE"))
	    {
	     escreve_md(pnewsymbol,num_no); /* escreve o "else" */
	     fprintf(mod_arq,"\n");

	     strcpy(poldsymbol->simbolo,pnewsymbol->simbolo);
	     getsymli(pnewsymbol,arqli);
	     if(!strcmp(pnewsymbol->simbolo,"{"))
	       {
	        trata_in_blk();

	        writeIniciador(num_no);

	        strcpy(poldsymbol->simbolo,pnewsymbol->simbolo);
	        getsymli(pnewsymbol,arqli);

	        do
	          statement(pnewsymbol,poldsymbol,arqli);
	        while(strcmp(pnewsymbol->simbolo,"}"));  /* diferente de '}' */

                trata_fim_blk();

		fecha_chave_monitor(num_no);
		writeFinalizador(num_no);

		strcpy(poldsymbol->simbolo,pnewsymbol->simbolo);
		getsymli(pnewsymbol,arqli);

	       }
             else
               error("Error Fatal: Era esperado um '}' na parte $ELSE\n");
	    }
	 }
	 else
	   error("* * Erro Fatal: `Cnnnn' estava sendo esperado depois do `if' na li * *            \n");

	 return;
      }
 if(!strcmp(pnewsymbol->simbolo,"$WHILE"))
      {

       if(num_no == 2)
         std_monitor(num_no-1);

       push_int(IN_LOOP,&stack_loop); /* indicativo de entrada em um loop */
       push_int(break_counter++,&stack_break);
       push_int(0,&stack_break_int);
       
       num_while = num_no;
       escreve_md(pnewsymbol,num_no); /* escreve o "while" */

       strcpy(poldsymbol->simbolo,pnewsymbol->simbolo);
       getsymli(pnewsymbol,arqli);

       if(!strncmp(pnewsymbol->simbolo,"$C",2))
	 {
	  parsercond();
          copy_command(pnewsymbol);
	  fprintf(mod_arq,"\n");
	  strcpy(poldsymbol->simbolo,pnewsymbol->simbolo);
          getsymli(pnewsymbol,arqli);
          if(!strcmp(pnewsymbol->simbolo,"{"))
	     {
	      writeIniciador(num_no);
	      trata_in_blk();

	      in_while_monitor(num_while);
	      strcpy(poldsymbol->simbolo,pnewsymbol->simbolo);
	      getsymli(pnewsymbol,arqli);
              
	      while(strcmp(pnewsymbol->simbolo,"}"))  /* diferente de '}' */

		 statement(pnewsymbol,poldsymbol,arqli);
	      
              trata_fim_blk();

	      fecha_chave_monitor(num_no);
	      writeFinalizador(num_no);

	      fim_while_monitor(num_while);
              strcpy(poldsymbol->simbolo,pnewsymbol->simbolo);
              getsymli(pnewsymbol,arqli);

	     }
          else
            error("Erro Fatal: Era esperado o '}' do corpo do $WHILE");

          pop_int(&stack_loop);   /* desempilha indicativo de loop */
          pop_int(&stack_break);
          pop_int(&stack_break_int);
	  return;
	 }
	else
	  error("* * Erro Fatal: `Cnnnn' estava sendo esperado depois do `while' na li * *                     \n");
      }
 if(!strcmp(pnewsymbol->simbolo,"$REPEAT"))
      {
       push_int(IN_LOOP_REPEAT,&stack_loop); /* indicativo de entrada em um loop */
       push_int(break_counter++,&stack_break);
       push_int(0,&stack_break_int);

       if(num_no == 2)
         std_monitor(num_no-1);

       escreve_md(pnewsymbol,num_no); /* escreve o "repeat" */

       strcpy(poldsymbol->simbolo,pnewsymbol->simbolo);
       getsymli(pnewsymbol,arqli);
       if(!strcmp(pnewsymbol->simbolo,"{"))
	  {
	   trata_in_blk();
	   writeIniciador(num_no);
	   strcpy(poldsymbol->simbolo,pnewsymbol->simbolo);
	   getsymli(pnewsymbol,arqli);

	   while(strcmp(pnewsymbol->simbolo,"}"))  /* diferente de '}' */
	      statement(pnewsymbol,poldsymbol,arqli);
	   

           trata_fim_blk();
	   num_aux = num_no;
	   fecha_chave_monitor(num_aux);
	   strcpy(poldsymbol->simbolo,pnewsymbol->simbolo);
	   getsymli(pnewsymbol,arqli);
           if(!strcmp(pnewsymbol->simbolo,"$UNTIL"))
		 in_until_monitor(num_no);
	    else
	       error("* * Erro Fatal: Era esperado um 'until' na li * *                            \n");
	    write_no(num_aux);
	   --tab_counter;
	   ajusta_formato(tab_counter);
	   fprintf(mod_arq,"}\n");

	  }
       else
         error("Erro Fatal: Era esperado o '}' do corpo do $REPEAT\n");

       if(!strcmp(pnewsymbol->simbolo,"$UNTIL"))
	 {
	  escreve_md(pnewsymbol,num_no);

	  getsymli(pnewsymbol,arqli);
	  if(!strncmp(pnewsymbol->simbolo,"$NC",3) ||
	     !strncmp(pnewsymbol->simbolo,"$C",2))
	       {
		parsercond();
                copy_command(pnewsymbol);

		fprintf(mod_arq,";"); /* escreve ";" para terminar o comando
				         "repeat" "until" (especifico do C) */
		fprintf(mod_arq,"\n");


		strcpy(poldsymbol->simbolo,pnewsymbol->simbolo);
		getsymli(pnewsymbol,arqli);
	       }
	     else
		 error("* * Erro Fatal: `NCnnnn' ou `Cnnnn' estava sendo esperado depois do `until' na li * *                    \n");
	    }
	 else
	    error("* * Erro Fatal: `until' estava sendo esperado depois do `repeat' na li * *                  \n");
          pop_int(&stack_loop);   /* desempilha indicativo de loop */
          pop_int(&stack_break);
          pop_int(&stack_break_int);
	return;
      }
 if(!strcmp(pnewsymbol->simbolo,"$FOR"))
	{
         push_int(IN_LOOP,&stack_loop); /* empilha indicativo de loop */
         push_int(break_counter++,&stack_break);
         push_int(0,&stack_break_int);

	/* printf("Coloquei um IN_LOOP para o FOR\n Digite um tecla p/ continuar..");
	getchar();
	*/
         if(num_no == 2)
           std_monitor(num_no-1);

	 num_for = num_no;
	 in_for_monitor(num_for); /* inicia monitoracao do "for" */
	 escreve_md(pnewsymbol,num_no); /* escreve o "for" */
	 fprintf(mod_arq,"(");  /* escreve o "abre parenteses" do "for"
			           (especifico do C) */

	 strcpy(poldsymbol->simbolo,pnewsymbol->simbolo);
	 getsymli(pnewsymbol,arqli);
	 if(!strncmp(pnewsymbol->simbolo,"$S",2))
	       {
		parserg(pnewsymbol);
                copy_command(pnewsymbol);
                getsymli(pnewsymbol,arqli);
		if(!strncmp(pnewsymbol->simbolo,"$C",2))
		  {
                   if(pnewsymbol->comprimento != 0)
                     {
		     parsercond();
                     copy_command(pnewsymbol);
                     }
                   else
                     fprintf(mod_arq,";"); /* Nao ha' expressao no for (especifico C)*/
		   
		   strcpy(poldsymbol->simbolo,pnewsymbol->simbolo);
                   getsymli(pnewsymbol,arqli);
		   if(!strncmp(pnewsymbol->simbolo,"$S",2))
		     {
                      num_incremento = num_no;

                      atualiza_num_for(num_for,num_incremento);

		      parserg(pnewsymbol);
                      copy_command(pnewsymbol);

		      fprintf(mod_arq,")\n"); /* especifico do "for" do C */

                      strcpy(poldsymbol->simbolo,pnewsymbol->simbolo);
                      getsymli(pnewsymbol,arqli);

		      if(!strcmp(pnewsymbol->simbolo,"{"))
			 {
			  writeIniciador(num_no);
			  trata_in_blk();
			  meio_for_monitor(num_for);
			  strcpy(poldsymbol->simbolo,pnewsymbol->simbolo);
			  getsymli(pnewsymbol,arqli);

                          while(strcmp(pnewsymbol->simbolo,"}"))  /* diferente de '}' */
			      statement(pnewsymbol,poldsymbol,arqli);
			 

                          trata_fim_blk();

			  std_monitor(num_incremento); /* monitora o incremento */
                          fecha_chave_monitor(num_no);

			  writeFinalizador(num_no);

			  fim_for_monitor(num_for);
                          strcpy(poldsymbol->simbolo,pnewsymbol->simbolo);
                          getsymli(pnewsymbol,arqli);
			 }
		       else
			 error("Erro Fatal: Era esperado o '}' do corpo do $FOR\n");
		       }
		   else
		      error("* * Erro Fatal: `Snnnn' estava sendo esperado depois do `for Snnnn Cnnnn' na li * * \n");
		  }
		else
		  error("* * Erro Fatal: `Cnnnn' estava sendo esperado depois do `for Snnnn' na li * *                    \n");
                pop_int(&stack_loop);   /* desempilha indicativo de loop */
                pop_int(&stack_break);
                pop_int(&stack_break_int);

	/* printf("Retirei um IN_LOOP para o FOR\n Digite um tecla p/ continuar..");
	getchar();
*/
		}
	 else
	    error("* * Erro Fatal: 'Snnnn' estava sendo esperado depois do 'for' na li * *                           \n");
        return;

	}
 if(!strcmp(pnewsymbol->simbolo,"$CASE"))
      {
       push_int(IN_CASE,&stack_loop); /* empilha indicativo de case */
	/* printf("Coloquei um IN_CASE \n Digite um tecla p/ continuar..");
	getchar();
	*/
       std_monitor(num_no);

       escreve_md(pnewsymbol,num_no); /* escreve o case */

       strcpy(poldsymbol->simbolo,pnewsymbol->simbolo);
       getsymli(pnewsymbol,arqli);

       if(!strcmp(pnewsymbol->simbolo,"$CC"))
	 {
	  parsercond();
          copy_command(pnewsymbol);
	  fprintf(mod_arq,"\n");

	  strcpy(poldsymbol->simbolo,pnewsymbol->simbolo);
	  getsymli(pnewsymbol,arqli);

	  if(!strcmp(pnewsymbol->simbolo,"{"))
	    {
             trata_in_blk();
	     writeIniciador(num_no); /* escreve o "abre chave" */
	     strcpy(poldsymbol->simbolo,pnewsymbol->simbolo);
	     getsymli(pnewsymbol,arqli);
	    }
	  else
	     error("* * Erro Fatal: `{' estava sendo esperado na li * *                                 \n");
	  do
	    {
	     if(!strcmp(pnewsymbol->simbolo,"$ROTC") ||
        	!strcmp(pnewsymbol->simbolo,"$ROTD"))
		{
		 escreve_md(pnewsymbol,num_no); /* escreve o rotulo
						            do "case" */

                 fprintf(mod_arq,"\n");
                 num_aux = num_no;
		 strcpy(poldsymbol->simbolo,pnewsymbol->simbolo);
		 getsymli(pnewsymbol,arqli);
                 if(!strcmp(pnewsymbol->simbolo,"$ROTC") || !strcmp(pnewsymbol->simbolo,"$ROTD"))
                   std_monitor(num_aux);
		}
	     else
		error("* * Erro Fatal: `rotc'(rotulo do case) era esperado na li * *                       \n");
             if(!strcmp(pnewsymbol->simbolo,"{"))
                {
                 trata_in_blk();
                 writeIniciador(num_no);
                 strcpy(poldsymbol->simbolo,pnewsymbol->simbolo);
		 getsymli(pnewsymbol,arqli);

                 while(strcmp(pnewsymbol->simbolo,"}"))
                  statement(pnewsymbol,poldsymbol,arqli);
                 

                 trata_fim_blk();

                 fecha_chave_monitor(num_no);
                 writeFinalizador(num_no);
                 getsymli(pnewsymbol,arqli);

                }
              else
                {
                 while(strcmp(pnewsymbol->simbolo,"$ROTC") &&
                       strcmp(pnewsymbol->simbolo,"$ROTD") &&
                       strcmp(pnewsymbol->simbolo,"}"))
                      statement(pnewsymbol,poldsymbol,arqli);
                }
	    }
	    while(strcmp(pnewsymbol->simbolo,"}"));

            trata_fim_blk();
	    writeFinalizador(num_no);
            std_monitor(num_no);

	    strcpy(poldsymbol->simbolo,pnewsymbol->simbolo);
	    getsymli(pnewsymbol,arqli);
	  }
	  else
            error("* * Erro Fatal: `CCnnnn' estava sendo esperado depois do `case' na li * *\n");
          pop_int(&stack_loop); /* desempilha indicativo de case */
	/* printf("Retirei um IN_CASE \n Digite um tecla p/ continuar..");
	getchar();
	*/
	    return;
       }
 if(!strcmp(pnewsymbol->simbolo,"$BREAK"))
   {
    break_monitor(num_no);

    strcpy(poldsymbol->simbolo,pnewsymbol->simbolo);
    getsymli(pnewsymbol,arqli);
    return;
   }
 if(!strcmp(pnewsymbol->simbolo,"$RETURN"))
   {
    std_monitor(num_no);
    last_node_write(num_no);   /* escreve o numero do ultimo no' */
    finaliza_monitoracao();
    parserg(pnewsymbol);       /* analisa expressao do 'return'  */
    escreve_md(pnewsymbol,num_no);
    fprintf(mod_arq,"\n");
    strcpy(poldsymbol->simbolo,pnewsymbol->simbolo);
    getsymli(pnewsymbol,arqli);
    return;
   }
 if(!strcmp(pnewsymbol->simbolo,"$CONTINUE"))
   {
    cont_monitor(num_no);
    escreve_md(pnewsymbol,num_no);

    strcpy(poldsymbol->simbolo,pnewsymbol->simbolo);
    getsymli(pnewsymbol,arqli);
    return;
   }
 if(!strcmp(pnewsymbol->simbolo,"$GOTO"))
   {
    std_monitor(num_no);
    escreve_md(pnewsymbol,num_no);

    strcpy(poldsymbol->simbolo,pnewsymbol->simbolo);
    getsymli(pnewsymbol,arqli);

    copy_command(pnewsymbol);
    fprintf(mod_arq,"\n");

    strcpy(poldsymbol->simbolo,pnewsymbol->simbolo);
    getsymli(pnewsymbol,arqli);
    return;
   }

/* strcpy(poldsymbol->simbolo,pnewsymbol->simbolo);
 getsymli(pnewsymbol,arqli);
 escreve_md(pnewsymbol,num_no);
  */

 /* Rotulo */

 if(num_no == 2)
   std_monitor(num_no-1);

 num_label = num_no;
 escreve_md(pnewsymbol,num_no);
 fprintf(mod_arq,"\n");    /* necessario para completar  a sintaxe de rotulo */

 strcpy(poldsymbol->simbolo,pnewsymbol->simbolo);
 getsymli(pnewsymbol,arqli);
 if(!strcmp(pnewsymbol->simbolo,"{"))
    {
    std_monitor(num_label); /* Inserido em 29-09-99 */
    trata_in_blk();
    escreve_md(pnewsymbol,num_no);
    std_monitor(num_label);
    strcpy(poldsymbol->simbolo,pnewsymbol->simbolo);
    getsymli(pnewsymbol,arqli);
    do
      statement(pnewsymbol,poldsymbol,arqli);
    while(strcmp(pnewsymbol->simbolo,"}"));  /* diferente de '}' */

    trata_fim_blk();
    fecha_chave_monitor(num_no);
    escreve_md(pnewsymbol,num_no);

   }
  else
    {
      /*
     ajusta_ponta_de_prova(tab_counter);
     fprintf(mod_arq,"\n");
     ++tab_counter;
     */
     std_monitor(num_label);
     statement(pnewsymbol,poldsymbol,arqli);
     /*
       --tab_counter;
     ajusta_ponta_de_prova(tab_counter);
     fprintf(mod_arq,"}\n"); */
    }

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
 /* buscando um novo simbolo da li */

 fscanf(arq,"%s %d %ld %ld %ld",
	pnewsymbol->simbolo,&num_no,&inicio,&comprimento,&linha);

 /* salvando os ponteiros para o simbolo no codigo fonte */
 /* if(ret == EOF)   chegou-se ao fim do arquivo */

 pnewsymbol->inicio = inicio;
 pnewsymbol->comprimento = comprimento;
 pnewsymbol->linha = linha;

 return;
}

/********************************************************************/
/* int parserg(struct symbol *)                                     */
/* Autor: Marcos L. Chaim                                           */
/* Versao: 1.0                                                      */
/* Data: 4/07/89                                                    */
/*                                                                  */
/* FUNCAO: faz a analise sintatica de um comando sequencial.        */
/*                                                                  */
/* ENTRADA: ponteiro para as informacoes sobre o simbolo.           */
/*                                                                  */
/* SAIDA:  nenhuma.                                                 */
/*                                                                  */
/********************************************************************/

int parserg(pnewsymbol)
struct symbol * pnewsymbol;
{
 int retorno;
 char match = VERDADEIRO;
 saida.classe[0] = '\0';


 fseek(arqfonte,inicio-1,SEEK_SET);
 if(!strncmp(pnewsymbol->simbolo,"$DCL",2))
   	{
        /* Inicializa estruturas de dados */
        set_parser_type(DECL);
        clear_stacks();
        push_int(CUSE,&type_use);

        yylex();

    	match = parser(hdeclaration,match);
    	retorno=DECLARATION;
   	}
 else
   	{
        /* Inicializa estruturas de dados */
        set_parser_type(EXPR);
        clear_stacks();
        push_int(CUSE,&type_use);
        
    	yylex();
        
    	match = parser(hstatement,match);
    	retorno=STATEMENT;
   	}

 if(!match == VERDADEIRO)
   error("* * Erro Fatal: Erro na analise sintatica do codigo fonte * *\n");

 /* Copia o vetor de variaveis indefinidas. */

 b_vector_cpy(&(info_no[num_no].undef),&undef);

 return(retorno);
}

/********************************************************************/
/* void parsercond()                                                */
/* Autor: Marcos L. Chaim                                           */
/* Versao: 1.0                                                      */
/* Data: 4/07/89                                                    */
/*                                                                  */
/* FUNCAO: faz a analise sintatica de uma condicao.                 */
/*                                                                  */
/* ENTRADA:nenhuma                                                  */
/*                                                                  */
/* SAIDA:  nenhuma.                                                 */
/*                                                                  */
/********************************************************************/

void parsercond()
{
 char match = VERDADEIRO;

 saida.classe[0] = '\0';

 fseek(arqfonte,inicio-1,SEEK_SET);

 /* Inicializa estruturas de dados */
 
  set_parser_type(EXPR);
  clear_stacks();
  push_int(PUSE,&type_use);

  
  yylex();

  match = parser(hexpression,match);
  if(!match == VERDADEIRO)
    error("* * Erro Fatal: Erro na analise sintatica do codigo fonte * *\n");

 /* Copia o vetor de variaveis indefinidas. */
 
 b_vector_cpy(&(info_no[num_no].undef),&undef);

 return;
}

/********************************************************************/
/* void copy_command(struct symbol *)                               */
/* Autor: Marcos L. Chaim                                           */
/* Versao: 1.0                                                      */
/* Data: 12/02/90                                                   */
/*                                                                  */
/* FUNCAO: copy o comando analisado no arquivo modificado.          */
/*                                                                  */
/* ENTRADA: apontador para as  informacoes do simbolo.              */
/*                                                                  */
/* SAIDA:  nenhuma.                                                 */
/*                                                                  */
/********************************************************************/

void copy_command(simb)
struct symbol * simb;
{

 char c;
 long auxcomprimento;

 auxcomprimento = simb->comprimento;

 fseek(arqfonte,simb->inicio-1,SEEK_SET);

 c = getc(arqfonte);
 while(auxcomprimento > 0 )
    {
      if(c != '\n')
        fprintf(mod_arq,"%c",c);

      if(c == '\f' || c == '\n')
        fprintf(mod_arq,"\n");

      c = getc(arqfonte);
      --auxcomprimento;
    }
}




























