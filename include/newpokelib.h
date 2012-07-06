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
** Type definitions for poketool's operation
*/ 

#ifndef _POKEOP_

#define _POKEOP_

#include <stdio.h>
#include "cte.h"

/* Tree for conditional expression */

typedef struct TreeExp {
  struct TreeExp * right;
  struct TreeExp * left;
  struct InfoToken {
    char * token;
    long int start;
    long int length;
    long int line;
  } position;
} tree_node;

/* List of nodes or arcs */

typedef struct list_info {                       
	    int x;              
	    int y;          
	    struct list_info *next;   
	  } l_info;

/* Bit Vector type definition */

typedef struct bit_vector {

  unsigned char * vector_bits;
  int type_in; /* indicates if the "common" state is totally set or reset. */
  int nbytes;  /* number of bytes allocated to b_vector */ 
	} b_vector;

typedef struct outtable {
		 int  linha;         /* */
                 int  comp;          /* */
		 long inicio;        /* */
                }TABLEOUT;

typedef struct source_vector {

  TABLEOUT * vec_var_source;
  int nvars;  /* number of variables in the unit */ 
	} v_source;

/* List of Names type definition */

typedef struct list_elements {
        int info;
        char * name;
        char * name1;
        struct list_elements * next;
} element;

/* Table of lists of Names type definition */

typedef struct table_elem {
  element * list;
  int id_no;          /* delivered id number */
  int id_no_eff;      /* if > 0, this a repeated field, and id_no_eff, the effective id number */
} table_element;


void set_size_vec_names(int);
int get_vec_names_size(void);
table_element * create_vec_names(int);
table_element * ajust_vec_names(table_element *, int, int);
table_element * insert_vec_names(table_element *,int,element *);
void set_id_no_eff(table_element *,int,int);
int check_recursive_rec(table_element *,int);
int check_recursion(table_element *,int, char *);
element * copy_list_vec_names(element *);
element * copy_list_info(element *);
int list_info_size(element *);
element * get_names(table_element * ,int);
void print_names(FILE *, table_element * ,int);
element * insert_list_vec_names(element *,char *);
element * insert_list_info(element *,char *, char *,int);
element * del_elem_list_info(element *,char *, char *);
element * delete_list_info(element *);
void delete_list(element *);
void delete_vec_names(table_element *);
int no_deref(table_element *,int);
int no_deref_compose(element *,int*);
int no_tot_deref(table_element *,int);
int no_tot_deref_compose(element *,int*);
double ajust_double(double);
void l_info_insert_info(l_info **,int);
void l_info_insert_2info(l_info **,int,int);
int l_info_is_in_2info(l_info *,int,int);
int l_info_is_in_info(l_info *,int);
void l_info_print(FILE *,l_info *);
l_info * l_info_free(l_info *);
void l_info_withdraw_last_info(l_info * *);
tree_node * mknode(char *,long int,long int,long int);
tree_node * free_tree(tree_node *);
tree_node * ajust_tree(tree_node *);
void record_tree(FILE *,tree_node *);
tree_node * get_last_right_node(tree_node * );
tree_node * get_last_left_node(tree_node * );

#endif /* _POKEOP_ */







