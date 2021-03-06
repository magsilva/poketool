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
// hparserg.h 	1.1		94/12/27	09:14:17
*/

#ifndef _HPARSERG
#define _HPARSERG

#define MARCADO -100
#define CARMAX 20
#define NDIGMAX 6
#define EMPTY '@'
#define FIMDEARQUIVO 'z'
#define VERDADEIRO 'V'
#define FALSO 'F'

struct nodo {
       struct nodo *suc;    /* aponta para o sucessor */
       struct nodo *alt;    /* aponta para uma alternativa */
       char nome[CARMAX+1]; /* contem o nome do terminal ou do nao terminal */
       char tipo;           /* indica tipo do elemnto do nodo: terminal ou nao
                               terminal */
       struct cabeca * nterminal;
                            /* aponta para a lista de nao terminais, caso seja
                               um nao terminal */
       int sem;             /* numero da rotina "semantica" a ser acionada apos
                               identificacao do nodo */
       };

struct cabeca {
       char nome[CARMAX+1];  /* nome do nao terminal */
       struct nodo *entrada; /* entrada para o grafo sintatico desse nao
                                terminal */
       struct cabeca *suc;   /* sucessor desse nao terminal */
       };

struct nter {
       char nome[CARMAX+1];
       char tipo;            /* diz se e' terminal, nao terminal, numeral ou
                                simbolo da metalinguagem */
       int num;              /* contem o valor do numeral e -10 caso
                                contrario */
       };


/* Funcoes que utilizam este header */

void find PROTO (( char *, struct cabeca ** )); 
void factor PROTO (( struct nodo **, struct nodo ** )); 
void termo PROTO (( struct nodo **, struct nodo **, struct nodo ** )); 
void expressao PROTO (( struct nodo **, struct nodo ** )); 
void producao PROTO (( void )); 
char parser PROTO (( struct cabeca *, char )); 
void pegsim PROTO (( void )); 
void erro PROTO (( void )); 
void yylex PROTO (( void )); 
void rot_sem PROTO (( int )); 
void msg_print PROTO (( char * )); 
void apaga_linha PROTO (( int, int ));
void lib_mem PROTO (( char * )); 

#endif /* fim _HPARSERG */
