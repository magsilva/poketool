/*
// headerli.h 	%I%	%Y%	%E%	%U%
*/


/*-------------------------------------------------------------------*/
/*           CABECALHO DAS FUNCOES DE ANALISE SINTATICA              */
/*-------------------------------------------------------------------*/


#define STATEMENT 10
#define DECLARATION 20

#define MAXNOS 150
#define MAXSYM 1024

struct symbol    /* estrutura que contem os simbolos da li */
    {
     char simbolo[MAXSYM];
     long inicio;
     long comprimento;
     long linha;
    };

void copy_command PROTO (( struct symbol * ));
void parserli PROTO (( FILE * ));
int parserg PROTO (( struct symbol * )); 
void parsercond PROTO (( void )); 
void getsymli PROTO (( struct symbol *, FILE * )); 
void statement PROTO (( struct symbol *, struct symbol *,FILE * )); 
int getfuncs PROTO (( int,char * [] ));
void freefuncs PROTO (( void ));
int is_func PROTO (( char * ));
char * func_cur PROTO (( void ));
char * get_file_name PROTO ((void));
int get_number_func_by_name PROTO ((char *));
char * get_func_by_number PROTO ((int));
int get_number_funcs PROTO ((void));
char * get_str_funcs PROTO ((void));
