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
// stack.h 	1.1		94/12/27	09:14:18
*/


#define MAXLENGTH 500
#define NCARMAX 1024


/* Tipo de dado que implementa uma pilha de inteiros */

typedef struct stack_int {
	int top;
        int elements[MAXLENGTH];
	} STACK;

typedef struct goto_label {
      char name[NCARMAX+1];
      int id;
      struct goto_label * next;
      } GOTOLABEL;

/* Tipo de dado que implementa um pilha de pares de inteiros */

typedef struct pair_int {
       int abs;   /* abscissa */
       int coor;  /* coordenada */
       } PAIRINT;

typedef struct stack_pair {
	int top;
	PAIRINT elements[MAXLENGTH];
	} STACKPAIR;

void insert_list PROTO ( ( char *, int, GOTOLABEL ** ));
void clear_list PROTO ( ( GOTOLABEL * ));
int find_node PROTO ( ( char *, GOTOLABEL * ));
void mk_nl_int PROTO ( ( STACK * ));
int empty_int PROTO ( ( STACK * ));
int top_int PROTO ( ( STACK * ));
int pop_int PROTO ( ( STACK * ));
void push_int PROTO ( ( int,STACK * ));
void mk_nl_pair PROTO ( ( STACKPAIR * ));
int empty_pair PROTO ( ( STACKPAIR * ));
PAIRINT top_pair PROTO ( ( STACKPAIR * ));
PAIRINT pop_pair PROTO ( ( STACKPAIR * ));
void push_pair PROTO ( ( PAIRINT *,STACKPAIR * ));


