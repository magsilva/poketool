/*
// trataext.c 	%I%	%Y%	%E%	%U%
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef TURBO_C

#include <conio.h>
#include <alloc.h>

#else  /* SunOS, BSD4.2 e RISC 6000 */

#include <malloc.h>

#endif

#include "util.h"
#include "hrotsem.h"
#include "tabelasli.h"
#include "hli.h"
#include "headerli.h"
#include "header.h"
#include "newpokelib.h"

/* Declaracao de Variaveis Globais */


extern struct table * tab_trans[];
extern struct chave * tab_chave[];
extern int n_chaves;
extern int ntablex;
extern struct no_tipdef * raiz;

extern struct cabeca * list,
					 * h,
					 * hdeclaration,
					 * hstatement,
					 * hexpression,
					 * sentinel;
extern struct nodo * p;

extern NODEINFO info_no;
extern b_vector undef;
extern b_vector fut_undef;
extern STACK stack_loop;
extern STACK stack_break;
extern STACK stack_break_int;
extern STACK stack_nest;
extern STACKPAIR stack_var;
extern STACKMIX cur_id_bkt;
extern STACKPAIR symbol_type;
extern STACK modifier;
extern STACK nclass;
extern STACK structure;
extern STACK signal;
extern STACK qualifier;
extern STACK nature;
extern STACK ident;
extern STACK aux_mod;
extern STACK operators;
extern STACK type_use;
extern STACK stackpar;
extern table_element * pvarname;
extern STACKTABLEOUT stack_descr;

extern STACK stack_typedef;
extern STACKBITVECTOR stack_undef;
extern STACKBITVECTOR stack_fut_undef;
extern DESPOINTER names;
extern int n_parenteses;
extern int tab_counter;
extern int num_no;
extern int no_blk;

/*******************************************************************/
/* void inicia_glb()                                               */
/* Autor: Marcos L. Chaim                                          */
/* Data: 6/11/89                                                   */
/* Versao: 1.0                                                     */
/*                                                                 */
/* Funcao: inicia as estruturas de dados globais. Tabelas de tran- */
/*         sicao e de palavras chaves, os vetores de bits das es-  */
/*         truturas undef, info_no, as pilhas stack_var, stack_un- */
/*         def e stack_nest e a tab_var_def sao iniciadas.         */
/*                                                                 */
/* Entrada: nenhuma.                                               */
/*                                                                 */
/* Saida: nenhuma.                                                 */
/*                                                                 */
/* Variaveis Globais Utilizadas: undef, info_no, stack_var, stack_ */
/*                               nest, stack_scope e tab_var_def.  */
/*                                                                 */
/*                                                                 */
/*******************************************************************/

void inicia_glb()
{

 /* Declaracao de Variaveis Locais */

 int i;


 /* Inicia os apontadores para a tabela de transicao e tabela de palavras
	chaves com o valor nulo */

 for(i=0; i>=0 && i < NLEX; ++i)
	 tab_trans[i] = (struct table *) NULL;
 for(i=0; i>=0 && i < NCHAVE; ++i)
	 tab_chave[i] = (struct chave *) NULL;

 /* Inicia apontadores para o grafo sintatico do parser especifico */

 list = (struct cabeca *) NULL;
 h = (struct cabeca *) NULL;
 hdeclaration = (struct cabeca *) NULL;
 hstatement = (struct cabeca *) NULL;
 hexpression = (struct cabeca *) NULL;
 p = (struct nodo *) NULL;

 /* Aloca espaco para a Estrutura de Dados info_no */

 n_parenteses = 0;
 no_blk = 0; /* numero de blocos igual a zero */
 info_no = (NODEINFO)  malloc( (MAXNOS+1) * sizeof(INFOCELL));
 if(info_no == (NODEINFO) NULL)
	error("* * Erro Fatal: Nao consegui alocar espaco para info_no * *\n");

 /* Inicia valores dos vetores de bits de info_no */

 for(i = 0; i >=0 && i <= MAXNOS; ++i)
    {
      b_vector_cons(&(info_no[i].defg_i),VARTEST,0);
      v_source_cons(&(info_no[i].s_defg_i), VARTEST);
      reset_all(&(info_no[i].defg_i));
      
      b_vector_cons(&(info_no[i].def_ref),VARTEST,0);
      v_source_cons(&(info_no[i].s_def_ref), VARTEST);
      reset_all(&(info_no[i].def_ref));
      
      b_vector_cons(&(info_no[i].undef),VARTEST,1);
      set_all(&(info_no[i].undef));

      b_vector_cons(&(info_no[i].c_use),VARTEST,0);
      v_source_cons(&(info_no[i].s_c_use), VARTEST);
      reset_all(&(info_no[i].c_use));

      b_vector_cons(&(info_no[i].p_use),VARTEST,0);
      v_source_cons(&(info_no[i].s_p_use), VARTEST);
      reset_all(&(info_no[i].p_use));

      info_no[i].to_monitor = NOTYET;
    }

 /* Inicia vetores de bits undef e fut_undef */
 
 b_vector_cons(&undef,VARTEST,1);
 set_all(&undef);
 
 b_vector_cons(&fut_undef,VARTEST,1);
 set_all(&fut_undef);

 /* Inicia pilhas */

 mk_nl_int(&stack_loop);
 mk_nl_int(&stack_break);
 mk_nl_int(&stack_break_int);
 mk_nl_int(&stack_nest);
 mk_nl_int(&stack_typedef);
 mk_nl_pair(&stack_var);
 mk_nl_mix(&cur_id_bkt);  
 mk_nl_pair(&symbol_type); 
 mk_nl_int(&modifier);
 mk_nl_int(&nclass);         
 mk_nl_int(&structure);        
 mk_nl_int(&signal);
 mk_nl_int(&qualifier);
 mk_nl_int(&nature);
 mk_nl_int(&ident);
 mk_nl_int(&aux_mod);
 mk_nl_int(&operators);
 mk_nl_int(&type_use);
 mk_nl_int(&stackpar);
 mk_nl_tableout(&stack_descr);
 mk_nl_bitvector(&stack_undef);
 mk_nl_bitvector(&stack_fut_undef);
 
 push_int(0,&stack_nest);  /* empilha o bloco 0 na pilha de aninhamento */

 /* Inicia tab_var_def's */

 make_null();

 /* Inicia tab_var_def do bloco 0 */

 inicia_tab_var_def(0,SIZEHASH);

 /* Inicia armazenador de nomes das variaveis (provisorio) */

 names = (DESPOINTER) NULL;

 set_size_vec_names(SIZEHASH);
 pvarname = create_vec_names(get_vec_names_size());
 
 /* Inicia o contador de tabulacao */

 tab_counter = 0;

 return;
}
 /******************************************************************/
 /* void info_no_clear()                                           */
 /* Autor: Marcos L. Chaim                                         */
 /* Data: 19/03/95                                                 */
 /* Versao: 1.0                                                    */
 /*                                                                */
 /* Funcao: limpa as variaveis do tipo info_no para a proxima      */
 /*         funcao. 						   */
 /*                                                                */
 /* Entradas: nenhuma.                                             */
 /*                                                                */
 /******************************************************************/
void info_no_clear()
{
int i;

 /* Inicia valores dos vetores de bits de info_no */

 for(i = 1; i >=1 && i <= MAXNOS; ++i)
    {
      reset_all(&(info_no[i].defg_i));
      set_all(&(info_no[i].undef));
      reset_all(&(info_no[i].def_ref));
      reset_all(&(info_no[i].c_use));
      reset_all(&(info_no[i].p_use));
      info_no[i].to_monitor = NOTYET;
    }

 /* Ajusta (set) todos os bits do vetor de indefinicoes */

 set_all(&undef);
 set_all(&fut_undef);
}
 /******************************************************************/
 /* void info_no_in()                                              */
 /* Autor: Marcos L. Chaim                                         */
 /* Data: 24/06/95                                                 */
 /* Versao: 1.0                                                    */
 /*                                                                */
 /* Funcao: inicia o vetor de bits info_no[1].defg_i com as varia- */
 /*         veis globais definidas em info_no[0]defg_i.            */
 /*                                                                */
 /* Entradas: nenhuma.                                             */
 /*                                                                */
 /******************************************************************/
void info_no_in()
{
(void) union_b_vector(&(info_no[1].defg_i),&(info_no[0].defg_i));
}

 /******************************************************************/
 /* void info_no_b_vector_ajust()                                     */
 /* Autor: Marcos L. Chaim                                         */
 /* Data: 13/08/99                                                 */
 /* Versao: 1.0                                                    */
 /*                                                                */
 /* Funcao: ajusta os vetores de bits para terem o mesmo tamanho.  */
 /*                                                                */
 /* Entradas: nenhuma.                                             */
 /*                                                                */
 /******************************************************************/

void info_no_b_vector_ajust()
{
  int maxsize, auxsize;
  int i;

   maxsize = (info_no[0].defg_i).nbytes;
  
   for(i = 0; i >=0 && i <= MAXNOS; ++i)
    {
      auxsize = (info_no[i].defg_i).nbytes;
      if(maxsize < auxsize)
         maxsize = auxsize;
      auxsize = (info_no[i].def_ref).nbytes;
      if(maxsize < auxsize)
         maxsize = auxsize;
      auxsize = (info_no[i].undef).nbytes;
      if(maxsize < auxsize)
         maxsize = auxsize;
      auxsize = (info_no[i].c_use).nbytes;
      if(maxsize < auxsize)
         maxsize = auxsize;
      auxsize = (info_no[i].p_use).nbytes;
      if(maxsize < auxsize)
         maxsize = auxsize;      
    }

   for(i = 0; i >=0 && i <= MAXNOS; ++i)
    {
      auxsize = (info_no[i].defg_i).nbytes;
      if(maxsize > auxsize)
         b_vector_increase_size(&(info_no[i].defg_i),maxsize);
      auxsize = (info_no[i].def_ref).nbytes;
      if(maxsize > auxsize)
         b_vector_increase_size(&(info_no[i].def_ref),maxsize);
      auxsize = (info_no[i].undef).nbytes;
      if(maxsize > auxsize)
         b_vector_increase_size(&(info_no[i].undef),maxsize);
      auxsize = (info_no[i].c_use).nbytes;
      if(maxsize > auxsize)
         b_vector_increase_size(&(info_no[i].c_use),maxsize);
      auxsize = (info_no[i].p_use).nbytes;
      if(maxsize > auxsize)
         b_vector_increase_size(&(info_no[i].p_use),maxsize);
    }   
}

 /******************************************************************/
 /* void trata_in_blk()                                            */
 /* Autor: Marcos L. Chaim                                         */
 /* Data: 6/11/89                                                  */
 /* Versao: 1.0                                                    */
 /*                                                                */
 /* Funcao: incrementa o contador de blocos e empilha em stack_    */
 /*         nest este novo valor e empilha vetor de bits undef do  */
 /*         bloco anterior.                                        */
 /*                                                                */
 /* Entradas: nenhuma.                                             */
 /*                                                                */
 /* Saidas: nenhuma.                                               */
 /*                                                                */
 /* Variaveis Globais Utilizadas: stack_nest, undef e stack_undef. */
 /*                                                                */
 /******************************************************************/

 void trata_in_blk()
 {

  /* Declaracao de Variaveis Locais */

  info_no[num_no].to_monitor = INSIDE_BLK;
  ++no_blk;
  push_int(no_blk,&stack_nest);
  push_bitvector(&undef,&stack_undef);
  push_bitvector(&fut_undef,&stack_fut_undef);
  

  inicia_tab_var_def(no_blk,SIZEHASH);
  
/*
    if(num_no > 0)
    {
      printf("Inicio do bloco %d\n",no_blk);
      printf("Undef:");
      print_b_vector(&undef);
      printf("Fut_undef:");
      print_b_vector(&fut_undef);
    }
   
*/
  set_all(&fut_undef); 
  return;
 }
 /******************************************************************/
 /* void trata_fim_blk()                                           */
 /* Autor: Marcos L. Chaim                                         */
 /* Data: 6/11/89                                                  */
 /* Versao: 1.0                                                    */
 /*                                                                */
 /* Funcao: desempilha de stack_nest o ultimo bloco e desempilha o */
 /*         vetor de bits stack_undef e coloca em undef.           */
 /*                                                                */
 /* Entradas: nenhuma.                                             */
 /*                                                                */
 /* Saidas: nenhuma.                                               */
 /*                                                                */
 /* Variaveis Globais Utilizadas: stack_nest, undef e stack_undef. */
 /*                                                                */
 /******************************************************************/

 void trata_fim_blk()
 {
  b_vector undefaux;
  b_vector fut_undefaux;
  
  libera_tab_var_def(pop_int(&stack_nest));
    
  undefaux = pop_bitvector(&stack_undef);
  (void) inter_b_vector(&undefaux,&fut_undef);
  b_vector_cpy(&undef,&undefaux);

  b_vector_destroy(&undefaux);
  

  fut_undefaux = pop_bitvector(&stack_fut_undef);

  (void) inter_b_vector(&fut_undef,&fut_undefaux);

  b_vector_destroy(&fut_undefaux);
  
  no_blk=top_int(&stack_nest);
 
/*
    if(num_no > 0)
    {
      printf("Fim do bloco %d\n",no_blk);
      printf("Undef:");
      print_b_vector(&undef);
      printf("Fut_undef:");
      print_b_vector(&fut_undef);

    }
*/
 
  return;
 }

/********************************************************************/
/* void error(char *)                                               */
/* Autor: Marcos L. Chaim                                           */
/* Versao: 1.0                                                      */
/* Data: 4/07/89                                                    */
/*                                                                  */
/* FUNCAO: desaloca espaco ocupado e imprime mensagem de erro.      */
/*                                                                  */
/* ENTRADA: "string" de caracteres que descreve o erro ocorrido.    */
/*                                                                  */
/* SAIDA: nenhuma.                                                  */
/*                                                                  */
/********************************************************************/

void error(msg)
char * msg;
{

 /* declaracao de variaveis locais */

 int i;
 struct cabeca *aux;
 DESPOINTER aux_names;
 FILE * logerror;

 /* libera espaco ocupado pelas tabelas de transicao e tabela de palavras
    reservas */

 /* dump_tab_var_def(); */

 for(i=0; i>=0 && i<= ntablex; ++i)
	 free(tab_trans[i]);

 for(i=0; i>=0 && i<= n_chaves; ++i)
	 free(tab_chave[i]);

 /* libera espaco ocupado pela arvore de tipos definidos */

 lib_tipo(raiz);

 /* libera espaco ocupado pelo grafo sintatico do parser especifico */

 for(aux = list; aux != sentinel; aux = aux->suc)
	lib_parser(aux->entrada);

 /* libera espaco ocupado pelos apontadores dos grafos relativos aos
	nao-terminais */

 lib_cabeca(list);

 /* libera espaco ocupado pela estrutura de dados info_no */

 free(info_no);

 /* libera espaco ocupado pela lista dos nomes das variaveis (provisorio) */

 for(aux_names = names;
	 aux_names != (DESPOINTER) NULL; aux_names = aux_names->next)
	  free(aux_names);

 /* libera espaco ocupado pelas tabelas de variaveis definidas */

 libera_tabs_from(0);

 /* libera espaco ocupado pelos nomes das funcoes */

 freefuncs();

 if(sentinel != (struct cabeca *) NULL)
	  free(sentinel);

 printf("%s",msg);

 logerror = (FILE *) fopen("logerror.tes","w");
 if(logerror == (FILE *) NULL)
   {
    printf("%s\n",msg);
    msg_print("* * Erro Fatal: Nao existe espaco para arquivo logerror.tes * *");
    exit(1);
   }
 fprintf(logerror,"%d",-1);
 fprintf(logerror,"%s",msg);
 fclose(logerror);

 exit(1);
}

/********************************************************************/
/* void lib_mem(char *)                                             */
/* Autor: Marcos L. Chaim                                           */
/* Versao: 1.0                                                      */
/* Data: 4/07/89                                                    */
/*                                                                  */
/* FUNCAO: desaloca espaco ocupado e imprime mensagem.              */
/*                                                                  */
/* ENTRADA: "string" de caracteres que descreve o erro ocorrido.    */
/*                                                                  */
/* SAIDA: nenhuma.                                                  */
/*                                                                  */
/********************************************************************/

void lib_mem(msg)
char msg[200];
{

 /* declaracao de variaveis locais */

 int i;
 struct cabeca *aux;
 DESPOINTER aux_names;
 FILE * logerror;

  /* dump_tab_var_def(); */
  
 /* libera espaco ocupado pelas tabelas de transicao e tabela de palavras
    reservas */

 for(i=0; i>=0 && i<= ntablex; ++i)
	 free(tab_trans[i]);

 for(i=0; i>=0 && i<= n_chaves; ++i)
	 free(tab_chave[i]);

 /* libera espaco ocupado pela arvore de tipos definidos */

 lib_tipo(raiz);

 /* libera espaco ocupado pelo grafo sintatico do parser especifico */

 for(aux = list; aux != sentinel; aux = aux->suc)
	if(aux != NULL)
		lib_parser(aux->entrada);

 /* libera espaco ocupado pelos apontadores dos grafos relativos aos
	nao-terminais */

 lib_cabeca(list);

 /* libera espaco ocupado pela estrutura de dados info_no */

 free(info_no);

 /* libera espaco ocupado pela lista dos nomes das variaveis (provisorio) */

 for(aux_names = names;
	 aux_names != (DESPOINTER) NULL; aux_names = aux_names->next)
	  free(aux_names);

 /* libera espaco ocupado pelas tabelas de variaveis definidas */

 libera_tabs_from(0);

 /* libera espaco ocupado pelos nomes das funcoes */

 freefuncs();

 if(sentinel != (struct cabeca *) NULL)
	  free(sentinel);

 msg_print(msg);

 logerror = (FILE *) fopen("logerror.tes","w");
 if(logerror == (FILE *) NULL)
   {
    msg_print("* * Erro Fatal: Nao existe espaco para arquivo logerror.tes * *");
    exit(1);
   }
 fprintf(logerror,"%d",0);
 fclose(logerror);
}

/********************************************************************/
/* void lib_cabeca(struct cabeca *)                                 */
/* Autor: Marcos L. Chaim                                           */
/* Versao: 1.0                                                      */
/* Data: 16/11/89                                                   */
/*                                                                  */
/* FUNCAO: desaloca espaco ocupado pelos apontadores dos nao-termi- */
/*         nais.                                                    */
/*                                                                  */
/* ENTRADA: apontador para a posicao de memoria onde esta o aponta- */
/*          dor do nao-terminal.                                    */
/*                                                                  */
/* SAIDA: nenhuma.                                                  */
/*                                                                  */
/********************************************************************/

void lib_cabeca(head)
struct cabeca * head;
{
 if( head != sentinel )
	{
	 lib_cabeca(head->suc);
	 free(head);
	}
 return;
}

/********************************************************************/
/* void lib_parser(struct nodo *)                                   */
/* Autor: Marcos L. Chaim                                           */
/* Versao: 1.0                                                      */
/* Data: 16/11/89                                                   */
/*                                                                  */
/* FUNCAO: desaloca espaco ocupado pelos nos do grafo sintatico do  */
/*         parser especifico.                                       */
/*                                                                  */
/* ENTRADA: apontador para a posicao de memoria onde esta o aponta- */
/*          dor do nao-terminal.                                    */
/*                                                                  */
/* SAIDA: nenhuma.                                                  */
/*                                                                  */
/********************************************************************/


void lib_parser(no_parser)
struct nodo * no_parser;
{

 /* Declaracao de Variaveis Locais */

 struct nodo * aux;

 if(no_parser != (struct nodo *) NULL)
   {
    no_parser->sem = MARCADO;
	aux = no_parser->suc;
	if( aux != (struct nodo *) NULL )
	   {
		if(aux->sem != MARCADO)
		   lib_parser(no_parser->suc);
	   }
	aux = no_parser->alt;
	if( aux != (struct nodo *) NULL )
	   {
		if(aux->sem != MARCADO)
		   lib_parser(no_parser->alt);
	   }
	free(no_parser);
   }
 return;
}

/*
**			Fim de trataext.c
*/


