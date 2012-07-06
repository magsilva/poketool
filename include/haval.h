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
// haval.h 	1.1		94/12/27	09:14:14
*/


#define Q1  1 /* em processo de reconhecimento */
#define Q2  2 /* nao reconheceu a entrada */
#define Q3  3 /* reconheceu a entrada */

#define MAXCRIT 8   /* constante que indica o numero de criterios suportado pela ferramenta */
#define PDU   1   /* constante que indica o criterio todos pot-du-caminhos */
#define PU    2   /* constante que indica o criterio todos pot-usos */
#define PUDU  3   /* constante que indica o criterio todos pot-usos/du */
#define ARCS  4   /* constante que indica o criterio todos arcos */
#define NOS   5   /* constante que indica o criterio todos no's */
#define PUSOS 6   /* constante que indica o criterio todos todos-p-usos */
#define USOS  7   /* constante que indica o criterio todos todos-usos */
#define DU    8   /* constante que indica o criterio todos todos-du-caminhos */
#define CUSOS 9   /* constante que indica o criterio todos todos-c-usos */
#define RAND 10   /* constante que indica o criterio de selecao randomica */

#define MAXLINE 1200 /* constante que indica o numero maximo de caracteres numa linha */


/* Declaracao do Tipo do registro que contera' os automata */

typedef struct automato {
   int criterio;        /* contem o criterio que esta' sendo avaliado */
   char * exp_regular;  /* contem a "string" com a expressao regular */
   int i;               /* no' i */
   int estado;          /* estado em que se encontra o automato */
   int n_path;          /* numero do caminho a ser percorrido */
   int assoc_id;
   char * pos_corrente; /* posicao corrente do ponteiro para a expressao regular */
   b_vector Ni;         /* vetor com os no' do grafo(i) */
   b_vector Nnv;        /* vetor com os nos do conjunto Nh */
   int freq;            /* frequencia de ocorrencia desta associacao no caso de teste */
   long seq_exec_i;     /* posicao do no i na sequencia de execucao */
   long seq_exec_j;     /* posicao do no j na sequencia de execucao */
   long no_func_invoc;  /* numero da invocacao da funcao */
   struct automato * next;
   } AUTOMATO;

typedef struct aval_str {
          char * funcname;
          AUTOMATO * pointer;
          int no_nos, no_vars, no_arcs, tot_req_exec[MAXCRIT+1];
          int no_output;
          long no_func_invoc, seq_exec;
          int criterio[MAXCRIT+1];
          b_vector N[MAXCRIT+1];

          FILE * descritores[MAXCRIT+1];  /* aponta para o arquivo de descritores do criterio */
          FILE * test_history[MAXCRIT+1]; /* aponta para o arquivo de historia da avaliacao */
          FILE * output[MAXCRIT+1];       /* aponta para o arquivo de associacoes nao executadas */
          FILE * exec[MAXCRIT+1];         /* aponta para o arquivo de associacoes executadas */
          FILE * requisitos[MAXCRIT+1];   /* aponta para a descricao dos requisitos de teste */
          FILE * test_freq[MAXCRIT+1];    /* aponta para a frequencia dos requisitos de teste */
} AvalStr;

 /* Declaracao das Funcoes que incluem esse header */

void avaliador PROTO ((AUTOMATO *, b_vector[]));
void peg_tok PROTO ((char *, char *));
char * apont_next_tok PROTO ((char *));
char * apont_prev_tok PROTO ((char *));
void peg_next_tok PROTO ((char *, char *));
char * jump_tok PROTO ((char *));
int fim_exp_reg PROTO ((char *));
int e_numero PROTO ((char *));
void pu_init_aval PROTO ((AUTOMATO **, b_vector *, int, FILE *, FILE *));
void pdu_init_aval PROTO ((AUTOMATO **, b_vector *, FILE *, FILE *));
void no_arc_init_aval PROTO ((AUTOMATO **, b_vector *, int, FILE *, FILE *));
void df_init_aval PROTO ((AUTOMATO **, b_vector *, int, FILE *, FILE *));
void le_conj PROTO ((b_vector *, char *, FILE *));
char * le_linha_str PROTO ((char *, char *, FILE *));
char * le_linha_str_gen PROTO (( char *, FILE *));
int det_number PROTO ((char *));
void error PROTO ((char *));
char * find_grfi PROTO ((char *, FILE *));
char * find_desc PROTO ((char *, char *, FILE *));
char * pega_path PROTO (( int, FILE *));
void lib_mem PROTO ((char *));
void lib_automato PROTO ((AUTOMATO *));
void msg_print PROTO ((char *));
void apaga_linha PROTO ((int,int));
int det_num_arc_i PROTO ((char *));
