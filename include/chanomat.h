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
// chanomat.h 	1.1		94/12/27	09:14:14
*/


/*-------------------------------------------------------------------*/
/*           CABECALHO DAS FUNCOES DE ANALISE SINTATICA              */
/*-------------------------------------------------------------------*/

#ifndef CHANOMAT_H

#define CHANOMAT_H

#define TRUE 1
#define FALSE 0
#define MEIO -1
#define STATEMENT 10
#define DECLARATION 20

#ifdef TURBO_C
#define MATRIXSIZE 100
#else
#define MATRIXSIZE 1000
#endif

#ifndef NCARMAX
#define NCARMAX 1024
#endif

struct symbol    /* estrutura que contem os simbolos da li */
    {
     char simbolo[NCARMAX];
     int no;
     long int inicio;
     long int comprimento;
     long int linha;
    };

void program PROTO ( ( FILE *, FILE * ));
void statement PROTO ( ( struct symbol *, FILE *, FILE *, STACK *,STACK *,STACK *,
               STACK *,GOTOLABEL **,GOTOLABEL ** ));
void getsymli PROTO ( ( struct symbol *, FILE * ));
void insere_abre_chave PROTO ( ( FILE * ));
void insere_fecha_chave PROTO ( ( FILE * ));
void wf_insere_fecha_chave PROTO ( ( FILE * ));
void copia_sym_li PROTO ( ( struct symbol *, FILE * ));
void error PROTO ( ( char * ));
int car_int PROTO ( ( STACK * ));
void liga_pair PROTO ( ( STACKPAIR *, int ));
void liga_int PROTO ( ( STACK *, int ));
void liga_goto PROTO ( ( GOTOLABEL *, GOTOLABEL * ));
void cat_pair PROTO ( ( STACKPAIR *, STACKPAIR * ));
void cat_mix PROTO ( ( STACKPAIR *, STACK * ));
void cat_int PROTO ( ( STACK *, STACK * ));
void atualiza_chave PROTO ( ( FILE *, long int, long int, int ));
void atualiza_comprimento PROTO ( ( FILE *, long int, long int, int ));

#endif /* CHANOMAT_H */
