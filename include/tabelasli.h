
/*
// tabelasli.h			%I%	%Y%	%E% %U%
*/

#ifndef _H_TABELASLI
#define _H_TABELASLI

#define NLEX 150    /* Numero maximo de elementos da tabela de transicao */
#define NCHAVE 100  /* Numero maximo de elementos da tabela de palavras chaves */
#define MAXINC 31   /* Numero maximo de diretorios de include */
#define POSITIVO 4
#define NEGATIVO 5
#define TRUE     1
#define FALSE    0
#define END_MARKER -100
#define FINAL_STATE 1000
#define TAMLEX 1024   /* */

struct table {
               char simbolo_lido[30];
               int estado_atual;
               int proximo_estado;
               int acao_semantica;
               int proxima_transicao;
              };

struct chave {
              char pal_chave[30];
              int referencia;
             };

struct tableout {
		 char label[TAMLEX+1];
		 char classe[TAMLEX+1];
		 int  linha;        /* */
                 int  comp;          /* */
		 long inicio;        /* */
                };

void carregador PROTO ( (char * ) );
int compara_simbolo PROTO ( (char ,struct table * ) );
char * lowstring PROTO ( (char * ) );
int pesq_tab PROTO ( (char * ) );
void a1 PROTO ( (void ) ), a2 PROTO ( (void ) ), a3 PROTO ( (void ) ), 
     a4 PROTO ( (void ) ), a5 PROTO ( (void ) ), a6 PROTO ( (void ) ), 
     a7 PROTO ( (void ) ), a8 PROTO ( (void ) ), a9 PROTO ( (void ) ),      
     a10 PROTO ( (void ) ),a11 PROTO ( (void ) );
void peg_ch PROTO ( (void ) );
void dev_ch PROTO ( (void ) );
void yylex PROTO ( (void ) );
void error PROTO ( (char * ) );

#endif /* _H_TABELASLI */












