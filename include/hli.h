/*
// hli.h 	%I%	%Y%	%E%	%U%
*/

#ifndef _HLI
#define _HLI

#ifndef _HPARSERG
#include "hparserg.h"
#endif

#ifndef NCARMAX
#define NCARMAX 1024
#endif

typedef struct goto_label {
      char name[NCARMAX+1];
      int id;
      struct goto_label * next;
      } GOTOLABEL;

struct no_tipdef {                        /* */
		  char * tipdef;          /* */
		  struct no_tipdef * esq; /* */
		  struct no_tipdef * dir; /* */
		};


void insert_list PROTO ( ( char *, int, GOTOLABEL ** ));
void clear_list PROTO ( ( GOTOLABEL * ));
void msg_print PROTO ( (char * ) );
FILE * fabre PROTO ( (char *, char * ) );
void libera  PROTO ( (char * ) );
void error  PROTO ( (char * ) );
int trata_args PROTO ((int, char *[]));

#ifndef _HROTSEM
void lib_cabeca PROTO ( (struct cabeca * ) );
void lib_parser PROTO ( (struct nodo * ) );
#endif

void car_tipo PROTO ( (char *, char *) );
void lib_tipo PROTO ( (struct no_tipdef * ) );
void imprime_tipos PROTO ( (struct no_tipdef * ) );
void print_tree PROTO ( (struct no_tipdef *, FILE * ) );
struct no_tipdef *tree_tipo PROTO ( (struct no_tipdef*,char*,int,int* ) );
void grava_li PROTO ( (int,char* ) );
void calcula_posicao PROTO ( (void ) );
void reset_posicao PROTO ( (void ) );
void set_posicao PROTO ( (long,long,long ) );

#endif /* _HLI */
