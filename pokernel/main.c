/*
// main.c			%I%	%Y%	%E% %U%
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef TURBO_C

#include <alloc.h>
#include <conio.h>

#else  /* SunOS, BSD4.2 e RISC 6000 */

#include <malloc.h>

#endif

#include "util.h"
#include "hparserg.h"
#include "hrotsem.h"
#include "header.h"
#include "headerli.h"
#include "tabelasli.h"
#include "hli.h"
#include "newpokelib.h" 

/* Variaveis Globais */

/* Apontadores para o arquivos utilizados */

FILE * arqfonte;   /* aponta para arquivo que contem o modulo fonte */
FILE * arqparser;  /* aponta para arquivo que contem a descricao do parser */
FILE * mod_arq;    /* aponta para arquivo que contem o modulo fonte modificado */

/* Variaveis globais utilizadas para armazenar as tabelas do analisador
   lexico */

int tamanho;            /* contem tamanho do identificador */
int ch;                /* ultimo caracter lido */
struct tableout  saida; /* ultimo token lido */

/* NLEX = 100 (numero maximo de elementos da tab. de trans.) */
/* NCHAVE = 100 (numero maximo de palavras chaves) */


struct table *tab_trans[NLEX]; /* armazena a tabela de transicao */
struct chave *tab_chave[NCHAVE]; /* armazena tabela de palavras chaves */
int n_chaves; /* armazena o numero de chaves de uma linguagem */
int ntablex;  /* armazena o numero de elementos da tab. de transicao */

struct no_tipdef *raiz; /* ponteiro para a arvore de tipos definidos */

/* Variaveis globais utilizadas na leitura do parser do arquivo e construcao
   do grafo sintatico na memoria */

struct cabeca * list, * hdeclaration,
		     * hstatement, * hexpression;
struct cabeca * sentinel;
struct cabeca *h;
struct nodo *p;
struct nter sym;


/* Estruturas de Dados utilizadas na extensao do GFC */

STACK stack_nest;
STACKPAIR stack_var;
STACK stack_typedef;
STACKPAIR symbol_type;
STACKMIX cur_id_bkt;
STACK modifier;
STACK nclass;
STACK structure;
STACK signal;
STACK qualifier;
STACK nature;
STACK ident;
STACK aux_mod;
STACK operators;
STACK type_use;
STACK stackpar;
STACKTABLEOUT stack_descr;
STACKBITVECTOR stack_undef;
STACKBITVECTOR stack_fut_undef;
b_vector flags;
NODEINFO info_no;
b_vector undef;
b_vector fut_undef;
DESPOINTER names;
char oldlabel[NCARMAX+1];
int n_parenteses;
int no_blk;

table_element * pvarname;

/* Variaveis utilizadas para acesso ao arquivo fonte */

long inicio, comprimento, linha;
int num_no;

/* Variaveis utilizadas na monitoracao */

STACK stack_break;   /* empilha o numero do break a ser convertido para goto */
STACK stack_break_int;   /* empilha info que ocorreu break */
STACK stack_loop;    /* empilha indicativo de case ou loop */
int tab_counter;     /* contador do numero de tabulacoes a serem usadas no ar-
                        quivo fonte modificado */
long int position_for; /* variavel global que salva a posicao em que foi escrito
                         a numeracao do comando "for" */


char * tabpath; /* variavel com o endereco do diretorio de tabelas. */

/* flags usados para controlar o nivel do modelo de dados */

/* default:           /* Modelo de dados mais estrito (Frankl's approach) */
int old_ver = 0;      /* Modelo de dados da primeira versao da POKE-TOOL */ 
int light_ver = 0;    /* Modelo de dados com pointerios mais simples */
int only_glb_used = 0;/* Modelo de dados apenas com variaveis globais usadas
                         pelo menos uma vez na unidade
                      */
int type_monitor = PATH; /* Define o type de monitoracao para avaliacao */

#ifdef DEBUG

int debug_flag = 0;

#endif
			 /* PROGRAMA PRINCIPAL */

/* unsigned _stklen = 0xC000; */
void main(argc,argv)
int argc;
char *argv[];
{

 /* declaracao de variaveis locais */

 FILE * arq1;
 int i,j;
 char nome[NCARMAX];  /* contem o nome do arquivo sem extensao */
 char nome_path [NCARMAX];  /* contem o nome do diretorio de tabelas */
 char func[NCARMAX];  /* nome da funcao a ser analisada */


 /* Insere nomes das funcoes a serem analisadas na estruturas funcs. */

 (void) getfuncs(argc, argv); /* delta indica se ha' um parametro
                                  especificando o nivel do modelo de dados */

 tabpath = getenv("NEWPOKETABS");
 if(tabpath == (char *) NULL)
   {
    msg_print("* * Erro Fatal (pokernel/main.c:156): Nao consegui o caminho das tabelas * *\n");
    exit(1);
   }

 strcpy(nome,get_file_name());
 strcat(nome,".nli");

#ifdef TURBO_C
 arq1= fopen(nome,"rb");
#else 			/* SunOs, BSD4.2 e RISC 6000 */
 arq1= fopen(nome,"r");
#endif

 if(arq1==(FILE *)NULL)
    error("* * Erro Fatal (pokernel/main.c:170): Nao consegui abrir o arquivo com linguagem intermediaria * *\n");

 strcpy(nome,get_file_name());
 strcat(nome,".c");

#if defined(TURBO_C) || defined(DJGPP)
 arqfonte= fopen(nome,"rb"); 
#else 			/* SunOs, BSD4.2 e RISC 6000 */
 arqfonte= fopen(nome,"r");
#endif


 if(arqfonte==(FILE *)NULL)
     error("* * Erro Fatal: Nao consegui abrir o arquivo fonte * *\n");

 mod_arq = fopen("testeprog.c","w");

 if(mod_arq==(FILE *)NULL)
     error("* * Erro Fatal: Nao consegui abrir o arquivo \"testeprog.c\" * *\n");



 strcpy(nome_path, tabpath);
#ifdef TURBO_C
 strcat(nome_path,"\\tabsin.c");
#else
 strcat(nome_path,"/tabsin.c");
#endif

 arqparser= fopen(nome_path,"r");

 if(arqparser==(FILE *)NULL)
    error("* * Erro Fatal: Nao consegui abrir o arquivo \"parserot.c\" * *");

 inicia_glb();

 /* Nao sao mais necessarios os tipos definidos porque o arquivo e'
    preprocessado */
 /* car_tipo("tes",".");   carrega tipos definidos */

 carregador("c");

 producao();

 names = (DESPOINTER) NULL;

 parserli(arq1);   /* faz analise sintatica e monitora programa modificado */

 fclose(mod_arq);
 fclose(arq1);


 fclose(arqfonte); 
 fclose(arqparser);

 lib_mem("* * Nucleo POKETOOL foi bem sucedido * *\n");
 exit(0);
}

/*
** msg_print() -- imprime no campo mensagem.
*/

void msg_print(msg)
char msg[200];
{
 /* apaga as linhas de mensagens */
 printf("%s",msg);
}

/*
**				Fim de main.c
*/











