/*
// tabelas.h 	1.1		94/12/27	09:14:19
*/

#define NLEX 100    /* Numero maximo de elementos da tabela de transicao */
#define NCHAVE 100  /* Numero maximo de elementos da tabela de palavras chaves */
#define POSITIVO 4
#define NEGATIVO 5
#define TRUE     1
#define FALSE    0
#define END_MARKER -100
#define FINAL_STATE 1000

#ifndef NCARMAX
#define NCARMAX 20
#endif

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
		 char label[NCARMAX+1];
		 char classe[NCARMAX+1];
                };
void carregador PROTO (( char * )); 
int compara_simbolo PROTO (( char ,struct table * )); 
char * lowstring PROTO (( char * )); 
int pesq_tab PROTO (( char * )); 
void a1 PROTO (( void )),  
     a2 PROTO (( void )),
     a3 PROTO (( void )),
     a4 PROTO (( void )),
     a5 PROTO (( void )),
     a6 PROTO (( void )),
     a7 PROTO (( void )),
     a8 PROTO (( void )),
     a9 PROTO (( void )),
     a10 PROTO (( void )),
     a11 PROTO (( void )),  
     a12 PROTO (( void )),
     a13 PROTO (( void )),
     a14 PROTO (( void )),
     a15 PROTO (( void )),
     a16 PROTO (( void )),
     a17 PROTO (( void )),
     a18 PROTO (( void ) ); 
void peg_ch PROTO (( void )); 
void dev_ch PROTO (( void )); 
void yylex PROTO (( void )); 
