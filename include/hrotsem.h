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
// hrotsem.h 	%I%	%Y%	%E%	%U%
*/

/*-------------------------------------------------------------------*/
/*     Miscelaneous Funcions Header                                  */
/*-------------------------------------------------------------------*/

#ifndef _H_ROTSEM
#define _H_ROTSEM
#ifndef UTIL_H
#include "util.h"
#endif

#ifndef _HPARSERG
#include "hparserg.h"
#endif

#ifndef _CTE_
#include "cte.h"
#endif /* _CTE_ */

#ifndef TAMLEX
#include "tabelasli.h"
#endif /* TAMLEX */

/*
#define free_stack_var()   n_parenteses=0; \
                          mk_nl_pair(&stack_var);  \
                          reset_all(&flags)
			  */

#ifndef _POKEOP_
#include "newpokelib.h"
#endif /* _POKEOP_ */

/* Declaracao do tipo pilha de Vetores de Bits */

typedef struct stack_bitvector {
	int top;
	b_vector elements[MAXLENGTH];
	} STACKBITVECTOR;



/* Declaracao do tipo da Estrutura de Dados info_no */

typedef struct node_information {
	long start;
	long length;
	b_vector defg_i;
	b_vector undef;
	b_vector def_ref;
        b_vector c_use;
        b_vector p_use;
        v_source s_defg_i;
	v_source s_def_ref;
        v_source s_c_use;
        v_source s_p_use;	
        char to_monitor;
	} * NODEINFO, INFOCELL;

/* Declaracao do tipo da Tabela de Variaveis Definidas  PROTO (( tab_var_def)) */

typedef struct tab_def_cell {
	char description [NCARMAX+1];
	int id;
	int type;
        int nature;
	int blk;
        int mod;
        int nclass;
        int signal;
        int qualifier;
        int structure;
 	struct tab_def_cell * next;
        struct tab_def_cell * recdef; 
        struct tab_def_cell * lst_of_flds;
        struct tab_def_cell * lst_of_pars;
} * TABDEF, TABDEFCELL;

/* Definicao de Macros */

#define get_id(current)        (current->id)
#define get_type(current)      (current->type)
#define get_nature(current)    (current->nature)
#define get_blk(current)       (current->blk)
#define get_mod(current)       (current->mod)
#define get_nclass(current)    (current->nclass)
#define get_signal(current)    (current->signal)
#define get_structure(current) (current->structure)
#define get_qualifier(current) (current->qualifier)

/* Declaracao do tipo da Pilha de Inteiros */

typedef struct stack_int {
	int top;
        int elements[MAXLENGTH];
	} STACK;

/* Declaracao do tipo da Pilha de Pares de Inteiros */

typedef struct pair_int {
       int abs;   /* abscissa */
       int coor;  /* coordenada */
       } PAIRINT;

typedef struct stack_pair {
	int top;
	PAIRINT elements[MAXLENGTH];
	} STACKPAIR;

/* Declaracao do tipo da  Pilha de Pares Reais */

typedef struct pair_mix {
       int abs;      /* abscissa */
       double coor;  /* coordenada */
       } MIX;

typedef struct stack_mix {
	int top;
	MIX elements[MAXLENGTH];
	} STACKMIX;

typedef struct stack_outtable {
	int top;
	TABLEOUT elements[MAXLENGTH];
	} STACKTABLEOUT;

/* Declaracao do tipo da Pilha de Vetor de Bits */


typedef struct name_description {
      char name[NCARMAX+1];
      int id;
      struct name_description * next;
      } * DESPOINTER, DESCELL;

/* Declaracao do tipo da Pilha de ELEMSUC */


/* Tipo de dado INFODFNO */

typedef struct infodfno {  /* informacao sobre o no' */
     int num_no_G;         /* numero do no' no grafo G */
     b_vector deff;       /* conjunto deff do no' */
     } INFODFNO;


typedef struct list {
     INFODFNO sucessor;    /* sucessor do no' */
     struct list * next;   /* proximo sucessor do no' */
    } LIST;



typedef struct elemsuc {   /* elemento sucessor */
     INFODFNO infosuc;     /* informacao sobre o no' */
     LIST * apont;         /* ponteiro da lista de sucessores do no' onde
			      cada elemento desta lista e' do tipo
			      INFODFNO */
    } ELEMSUC;

/* Declaracao dos tipos de dados que representam o grafo PROTO (( i)) */

typedef struct no_grafo_i {  /* elemento da lista que contera' os nos do grafo (i) */
     int num_grafo_i;        /* numero desse no' no grafo PROTO (( i)) */
     INFODFNO infosuc;       /* informacoes de fluxo de dados */
     char repetido;          /* indica se o no' e' repetido ou nao */
     struct pred_list * list_pred; /* apontador para a lista dos enderecos dos predecessores desse no' no grafo(i) */
     struct no_grafo_i * next; /* apontador para o proximo no' do grafo(i) */
     struct s_grafo_i * sucgfi;/* apontador para os sucessores do no' no grafo(i) */
    } NOGRAFOI;

typedef struct s_grafo_i { /* elemento da lista de sucessores do no' no grafo(i) */
     int num_grafo_i;      /* numero do no' no grafo(i) */
     char tipo;            /* tipo do arco ( primitivo ou herdeiro ?) */
     NOGRAFOI * no_address; /* endereco do no'sucessor na lista de nos do grafo(i) */
     struct s_grafo_i * next; /* proximo no' sucessor */
    } SUCGRAFOI;

typedef struct pred_list { /* elemento da lista de enderecos dos predecessores no grafo(i) */
     NOGRAFOI * address;   /* contem o endereco do predecessor */
     struct pred_list * next; /* aponta para o proximo elemento da lista de predecessores */
     } LISTPRED;


typedef struct stack_elemsuc {
	int top;
	ELEMSUC elements[MAXLENGTH];
	} STACKELEMSUC;

/* Declaracao do tipo de Pilha de Caracteres */


typedef struct stack_address {
	int top;
	NOGRAFOI * elements[MAXLENGTH];
	} STACKADDRESS;

/* Declaracao do tipo de Pilha de Informacoes sobre os Arcos */

typedef struct arc_info {
        PAIRINT arco;
        char tipo;
        char des_meio[5];
        } ARCINFO;


typedef struct stack_arc {
	int top;
	ARCINFO elements[MAXLENGTH];
	} STACKARC;

/* Declaracao do tipo de Pilha de arcos repetidos */

typedef struct rep_info {
        PAIRINT arco;
        int nivel_rep;
        } REPINFO;


typedef struct stack_rep {
	int top;
	REPINFO elements[MAXLENGTH];
	} STACKREP;

/* Declaracao das Funcoes que utilizam esse Header */

void make_null PROTO (( void )); 
PAIRINT retr_id PROTO (( char * )); 
PAIRINT change_id PROTO (( char *, int ));
void insert_var PROTO (( char *, int, int )); 
int hash PROTO (( char * )); 
void mk_nl_int PROTO (( STACK * )); 
int empty_int PROTO (( STACK * )); 
int top_int PROTO (( STACK * )); 
int pop_int PROTO (( STACK * )); 
void push_int PROTO (( int,STACK * )); 
void mk_nl_pair PROTO (( STACKPAIR * )); 
int empty_pair PROTO (( STACKPAIR * )); 
PAIRINT top_pair PROTO (( STACKPAIR * )); 
PAIRINT pop_pair PROTO (( STACKPAIR * )); 
void push_pair PROTO (( PAIRINT *,STACKPAIR * ));
void mk_nl_tableout PROTO (( STACKTABLEOUT * )); 
int empty_tableout PROTO (( STACKTABLEOUT * )); 
TABLEOUT top_tableout PROTO (( STACKTABLEOUT * )); 
TABLEOUT pop_tableout PROTO (( STACKTABLEOUT * )); 
void push_tableout PROTO (( TABLEOUT *,STACKTABLEOUT * ));
void mk_nl_mix PROTO (( STACKMIX * )); 
int empty_mix PROTO (( STACKMIX * )); 
MIX top_mix PROTO (( STACKMIX * )); 
MIX pop_mix PROTO (( STACKMIX * )); 
void push_mix PROTO (( MIX *,STACKMIX * ));
void mk_nl_bitvector PROTO (( STACKBITVECTOR * )); 
int empty_bitvector PROTO (( STACKBITVECTOR * )); 
b_vector top_bitvector PROTO (( STACKBITVECTOR * )); 
b_vector pop_bitvector PROTO (( STACKBITVECTOR * )); 
void push_bitvector PROTO (( b_vector *,STACKBITVECTOR *)); 
void mk_nl_elemsuc PROTO (( STACKELEMSUC * )); 
int empty_elemsuc PROTO (( STACKELEMSUC * )); 
ELEMSUC top_elemsuc PROTO (( STACKELEMSUC * )); 
ELEMSUC pop_elemsuc PROTO (( STACKELEMSUC * )); 
void push_elemsuc PROTO (( ELEMSUC *,STACKELEMSUC * )); 
void mk_nl_address PROTO (( STACKADDRESS * )); 
int empty_address PROTO (( STACKADDRESS * )); 
NOGRAFOI * top_address PROTO (( STACKADDRESS * )); 
NOGRAFOI * pop_address PROTO (( STACKADDRESS * )); 
void push_address PROTO (( NOGRAFOI *,STACKADDRESS * )); 
void mk_nl_arc PROTO (( STACKARC * )); 
int empty_arc PROTO (( STACKARC * )); 
ARCINFO top_arc PROTO (( STACKARC * )); 
ARCINFO pop_arc PROTO (( STACKARC * )); 
void push_arc PROTO (( ARCINFO *,STACKARC * )); 
void mk_nl_rep PROTO (( STACKREP * )); 
int empty_rep PROTO (( STACKREP * )); 
REPINFO top_rep PROTO (( STACKREP * )); 
REPINFO pop_rep PROTO (( STACKREP * )); 
void push_rep PROTO (( REPINFO *,STACKREP * )); 
void set_bit PROTO (( int, b_vector * )); 
void set_all PROTO (( b_vector * )); 
void reset_bit PROTO (( int, b_vector * )); 
void reset_all PROTO (( b_vector * )); 
int test_bit PROTO (( int, b_vector * )); 
int vazio_bit PROTO (( b_vector * )); 
b_vector neg_bitvector PROTO (( b_vector * )); 
b_vector uniao_bitvector PROTO (( b_vector *, b_vector * )); 
b_vector inter_bitvector PROTO (( b_vector *, b_vector * )); 
b_vector sub_bitvector PROTO (( b_vector *, b_vector * )); 
int is_equal_bitvector PROTO (( b_vector *, b_vector * )); 
int esta_contido_bitvector PROTO (( b_vector *, b_vector * )); 
void insert_name PROTO (( char *, int )); 
void show_description PROTO (( int,FILE * )); 

#ifndef _HLI
void lib_cabeca PROTO (( struct cabeca * )); 
void lib_parser PROTO (( struct nodo * )); 
#endif

void trata_in_blk PROTO (( void )); 
void trata_fim_blk PROTO (( void )); 
void inicia_glb PROTO (( void )); 
void error PROTO (( char * )); 
void inicia_tab_var_def PROTO (( int, int ));
void libera_tabs_from PROTO (( int ));
void libera_tab_var_def PROTO (( int ));
void libera_bucket PROTO (( TABDEFCELL * ));
void zera_local_counter PROTO (( void )); 
void gerencia_fim_func PROTO (( char * )) ;
void cria_grafo_def PROTO (( char *)) ;
void cria_dir PROTO (( char * )); 
void rot1_sem PROTO (( void )), 
     rot2_sem PROTO (( void )), 
     rot3_sem PROTO (( void )), 
     rot4_sem PROTO (( void )), 
     rot5_sem PROTO (( void )), 
     rot6_sem PROTO (( void )), 
     rot7_sem PROTO (( void )), 
     rot8_sem PROTO (( void )), 
     rot9_sem PROTO (( void )), 
     rot10_sem PROTO (( void )), 
     rot11_sem PROTO (( void )), 
     rot12_sem PROTO (( void )), 
     rot13_sem PROTO (( void )), 
     rot14_sem PROTO (( void )), 
     rot15_sem PROTO (( void )), 
     rot16_sem PROTO (( void )), 
     rot17_sem PROTO (( void )), 
     rot18_sem PROTO (( void )), 
     rot19_sem PROTO (( void )), 
     rot20_sem PROTO (( void )), 
     rot21_sem PROTO (( void )), 
     rot22_sem PROTO (( void )), 
     rot23_sem PROTO (( void )), 
     rot24_sem PROTO (( void )), 
     rot25_sem PROTO (( void )), 
     rot26_sem PROTO (( void )), 
     rot27_sem PROTO (( void )),
     rot28_sem PROTO (( void )),
     rot29_sem PROTO (( void )),
     rot30_sem PROTO (( void )),
     rot31_sem PROTO (( void )),
     rot32_sem PROTO (( void )),
     rot33_sem PROTO (( void )),
     rot34_sem PROTO (( void )),
     rot35_sem PROTO (( void )),
     rot36_sem PROTO (( void )),
     rot37_sem PROTO (( void )),
     rot38_sem PROTO (( void )),
     rot39_sem PROTO (( void )),
     rot40_sem PROTO (( void )),
     rot41_sem PROTO (( void )),
     rot42_sem PROTO (( void )),
     rot43_sem PROTO (( void )),
     rot44_sem PROTO (( void )),
     rot45_sem PROTO (( void )),
     rot46_sem PROTO (( void )),
     rot47_sem PROTO (( void )),
     rot48_sem PROTO (( void )),
     rot49_sem PROTO (( void )),
     rot50_sem PROTO (( void )),
     rot51_sem PROTO (( void )),
     rot52_sem PROTO (( void )),
     rot53_sem PROTO (( void )),
     rot54_sem PROTO (( void )),
     rot55_sem PROTO (( void )),
     rot56_sem PROTO (( void )),
     rot57_sem PROTO (( void )),
     rot58_sem PROTO (( void )),
     rot59_sem PROTO (( void )),
     rot60_sem PROTO (( void )),
     rot61_sem PROTO (( void )),
     rot62_sem PROTO (( void )),
     rot63_sem PROTO (( void )),
     rot64_sem PROTO (( void )),
     rot65_sem PROTO (( void )),
     rot66_sem PROTO (( void ));

TABDEF lookup(char *);
TABDEF lookup_field(TABDEF,char *);
TABDEF lookup_func(char *);
void insert(char *, int, int, int, int, int, int, int, TABDEF);
int  insert_varname(char *, int, int, int, int, int, int);
void insert_field(char * ,int, int, int, int, int, int, int, TABDEF, TABDEF);
TABDEF insert_pars(char * ,int, int, int, int, int, int, int, TABDEF, TABDEF);
int no_of_fields(TABDEF);
int get_no_fields(TABDEF,int, element *, element *);
TABDEF allocate_bucket(char *, int, int, int, int, int, int, int, TABDEF);
double get_field_id(TABDEF, char *, int, int);
TABDEF get_rec_def(TABDEF);
int is_deref(double);
int number_deref(int);
int is_heap(int);
int is_record(TABDEF);
int is_par_cons(TABDEF,int);

void push_char(char *,STACK *);
void pop_char(STACK *);
char * top_char(STACK *);
int get_parser_type(void);
void set_parser_type(int);
int get_use_type(void);
void set_use_type(int);
int get_name_counter(void);
void set_nome (char *);
char * get_nome(void);
void set_pars_dcl(int);
int  get_pars_dcl(void);
int get_stdio(void);

int set_all_fields(b_vector *, v_source *, TABDEF, int);	       
int reset_all_fields(b_vector *,  TABDEF, int);
void reset_def_var(TABDEF,MIX *);

#endif /* _H_ROTSEM */








