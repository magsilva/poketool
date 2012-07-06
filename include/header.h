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
// header.h 	1.1		94/12/27	09:14:15
*/


#define TRUE 1
#define FALSE 0
#define PRIMITIVO 'p'
#define HERDEIRO 'h'

struct no {                       /* elemento da lista de sucessores */
	    int num;              /* numero do no' */
	    char marca;           /* indica se o arco e' primitivo ou herderio e se exite marca de herdeiro */
	    struct no *proximo;   /* proximo no da lista de sucessores */
	  };

struct grafo {                    /* no' e seus sucessores */
	    int num;              /* numero do no' */
	    struct no *list_suc;  /* apontador para lista de sucessores */
	     };

struct conj_int {                 /* apontador para conjunto de inteiros */
	    struct no *apont;     /* apontador para lista ordenada de inteiros */
		};

struct conj_bit {
            b_vector set;        /* vetor de bits */
            };

/* Funcoes que incluem este header */

int inigrf PROTO (( char *, struct grafo ** )); 
int ins_elem PROTO (( struct no **,int )); 
int rins_elem PROTO (( struct no **,int )); 
int ret_elem PROTO (( struct no ** )); 
void lib_conj PROTO (( struct no * )); 
void lib_grf PROTO (( struct grafo *, int )); 
void lib_dom PROTO (( struct conj_int *, int )); 
void atr_conj PROTO (( struct no **, struct no ** )); 
void uniao PROTO (( struct no **, struct no **, struct no ** )); 
void intersecao PROTO (( struct no **, struct no **, struct no ** )); 
int e_igual PROTO (( struct no *, struct no * )); 
struct grafo * det_pred PROTO (( struct grafo *, int )); 
int * cal_rpos PROTO (( int, struct grafo *, int *, int * )); 
struct conj_bit * cal_dom PROTO (( struct grafo *, struct grafo *, int )); 
int tem_marca PROTO (( int, int, struct grafo * )); 
int membro PROTO (( int, struct no * )); 
void del_elem PROTO (( struct no **, int )); 
void rdel_elem PROTO (( struct no **, int )); 
int no_ocorren PROTO (( struct no *, int )); 
int find_path PROTO (( int,int,int,struct grafo *, struct grafo *,int,struct no ** )); 
int in PROTO (( int, int, struct grafo *, struct grafo *, struct no **,int )); 
int out PROTO (( int, int, struct grafo *, struct grafo *, struct no **,int )); 
int satisR1 PROTO (( int,struct no *,struct grafo *,struct grafo * )); 
int satisR2 PROTO (( int,struct no *,struct grafo *,struct grafo * )); 
int satisR4 PROTO (( int,struct no *,struct grafo *,struct grafo *,struct conj_bit * )); 
int cardinal PROTO (( struct no * )); 
int e_primitivo PROTO (( int, int, struct grafo * )); 
int out_no_mark PROTO (( int,int,struct grafo *, struct grafo *,int )); 
void elimina_arco PROTO (( int,struct no *,struct grafo * )); 
void marca_arco PROTO (( int,struct no *,struct grafo * )); 
b_vector calcula_resultados PROTO ((b_vector *, int, struct conj_bit *, struct grafo *, int)); 
void cal_prim PROTO (( struct grafo *, struct grafo *,int  )); 
int ident_nodes PROTO (( int, int, struct grafo *, struct grafo *, struct no **,int )); 
