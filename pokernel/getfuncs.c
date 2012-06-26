
/*
// getfucns.c                        %I%     %Y%     %E% %U%
// 
// Este arquivo contem funcoes que manipulam a estrutura funcs. Trata-se de
// um vetor que contem os nomes da funcoes a serem analisdas pelo "pokernel".
//
// Autor: Marcos L. Chaim
// Data: 26/12/94
//
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"
#include "headerli.h"
#include "cte.h"

/*
** Declaracao de estrutura de dados
*/

#define MAXFUNCS 300
static char file_name[100];
static char * funcs [MAXFUNCS+1];
static char  cur_func [100];

extern int old_ver;
extern int light_ver;
extern int only_glb_used;
extern int type_monitor;

/*
** getfuncs() -- inicia o vetor funcs com os nomes da funcoes e verifica se
**		 a descricao dos parametros esta' correta.
*/

int getfuncs(argc,argv)
int argc;
char * argv[];
{

/* Declaracao de Variaveis Locais */

int i,delta;

 if (argc <  3)
  {
    printf("Uso: newpokernel [-L<nivel do modelo de dados>] [-M|-F|-P] <nome_arquivo sem extensao> -f<nome da funcao> [-f<nome da funcao>]\n");
    exit(1);
  }

 if(argc >= 3)
   {
   if(argc-2 > MAXFUNCS)
	{
        printf("* * Erro Fatal: Numero excessivo de funcoes* *\n");
     	exit(1);
	}


   /* Verifica o nivel do modelo de dados a ser utilizado */

   if(!strncmp(argv[1],"-L",2))
     {
     switch(atoi(argv[1]+2))
       {
       case 0:
           old_ver = 1;
           break;
       case 1:
           light_ver = 1;
           break;
       case 2:
           break;
       case 3:
           old_ver = 1;
           only_glb_used = 1;
           break;         
       case 4:
           light_ver = 1;
           only_glb_used = 1;
           break;
       case 5:
           only_glb_used = 1;
           break;
       default:;
       }
      delta = 3; /* inicio do loop das funcoes deve ser incrementado */  
     }
   else
     delta = 2; /* inicio do loop das funcoes */

   /* Type of monitoration: during execution */

   if(!strncmp(argv[2],"-M",2))
     {
       type_monitor = MEMORY;
       delta++; /* inicio do loop das funcoes deve ser incrementado */
     }

   /* Default type of monitoration: file path.tes */

   if(!strncmp(argv[2],"-F",2))
     {
       type_monitor = PATH;
       delta++; /* inicio do loop das funcoes deve ser incrementado */
     }

   if(!strncmp(argv[2],"-P",2))
     {
       type_monitor = PIPE;
       delta++; /* inicio do loop das funcoes deve ser incrementado */
     }

   /* Salva o nome do arquivo */

   strcpy(file_name,argv[delta-1]);

   /* Insere funcao na lista de funcoes a serem analisadas */

   for(i=delta; i < argc; ++i)
   	if(strncmp(argv[i],"-f",2))
    		{
     		printf("* * Erro Fatal:\"%s\" descricao incorreta de parametros* *\n",argv[i]);
     		exit(1);
    		}
	else
		{
		char * p_aux;
		funcs[i-delta] =p_aux= (char *) malloc(strlen(argv[i]+2)+1);

		if(funcs[i-delta] == (char *) NULL)
			{
			printf("* * Erro Fatal: Nao consegui alocar espaco em memoria!\n");
			exit(1);
			}
		strcpy(p_aux,argv[i]+2);

                if(!strcmp(funcs[i-delta],"_todas_") && type_monitor != PATH)
			{
			printf("* * Erro Fatal: Parametros incompativeis: \"-M\" ou \"-P\" e \"-f_todas_\"\n");
			exit(1);
			}
                       
		}
    strcpy(cur_func,funcs[0]);
   }

 /* Indica a existencia de um parametro adicional */
 
 if(delta == 3)
   return 1;
 else
  return 0;

}

/*
** freefuncs() -- libera espaco ocupado pelos nomes das funcoes.
*/

void freefuncs()
{
int i;

 for(i=0; (i>=0 && i < MAXFUNCS)&& (funcs[i] != (char *) NULL); ++i)
        free(funcs[i]);

}

/*
** is_a_func() -- retorna 1 se o nome passado como parametro esta em funcs;
**		  caso contrario, 0.
*/

int is_a_func(name)
char * name;
{
int i;
int ret = 0;
 for(i=0; (i>=0 && i < MAXFUNCS)&& (funcs[i] != (char *) NULL); ++i)
       if(!strcmp(funcs[i],name) || !strcmp(funcs[i],"_todas_")) 
		{
		ret = 1;
		strcpy(cur_func, name);
		break;
		} 

return ret;
}

/*
** get_number_funcs() -- retorna o numero de funcoes em analise.
*/

int get_number_funcs(void)
{
 int i;

 for(i=0; (i>=0 && i < MAXFUNCS)&& (funcs[i] != (char *) NULL); ++i);

return i;
}

/*
** get_func_by_number() -- retorna a funcao de acordo com o numero.
*/

char * get_func_by_number(func_no)
int func_no;
{
 
  if(func_no < 0 || func_no > get_number_funcs())
    error_msg(out_of_range);

 return(funcs[func_no]);
}

/*
** get_number_funcs() -- retorna o numero da funcao a partir do nome.
*/

int get_number_func_by_name(funcname)
char * funcname;
{
 int i;
 int found=FALSE;

 for(i=0; (i>=0 && i < MAXFUNCS)&& (funcs[i] != (char *) NULL); ++i)
       if(!strcmp(funcs[i],funcname)) 
	 {
           found=TRUE;
	   break;
	 }	       

 if(!found)
   error_msg(out_of_range);

 return i;
}

/*
** func_cur() -- retorna o nome da funcao corrente.
*/


char * func_cur()
{
 return(cur_func);
}

/*
** get_file_name() -- retorna o nome do arquivo em analise.
*/

char * get_file_name(void)
{
  return file_name;
}

/*
** get_str_funcs() -- retorna o string com os nomes das funcoes.
*/

char * get_str_funcs(void)
{
  static char bigstr[1000];
  int size=0;
  int i;

  strcpy(bigstr,"");

  size=strlen(bigstr);

  for(i=0; (i>=0 && i < MAXFUNCS) && (funcs[i] != (char *) NULL); ++i)
     {
       size+=strlen(funcs[i]);
       if(size >= 1000)
         error_msg(out_of_range);
       strcat(bigstr,"\"");
       strcat(bigstr,funcs[i]);
       strcat(bigstr,"\"");
       if(funcs[i+1] != NULL)
	 strcat(bigstr,",");
     }	       

  return bigstr;  
}

/*
**			Fim de getfuncs.c
*/


