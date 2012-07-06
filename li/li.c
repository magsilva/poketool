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
// li.c			%I%	%Y%	%E% %U%
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"

#include "tabelasli.h"  /* definicoes de tipo para o A.L. (analisador lexico) */

#include "hparserg.h" /* definicoes de tipos para o parser (analisador sinta-
                         tico */

#include "hli.h"     /* Declaracoes e definicoes de tipos e funcoes do pro-
			grama "li" */

/* DECLARACAO DE VARIAVEIS GLOBAIS */

/* Variavel utilizada para controle da depuracao */

/* Flag depuracao */

#ifdef DEBUG

int debug_flag = 0;

#endif

/* Variaveis globais utilizadas para armazenar as tabelas do analisador
   lexico e contrlar o A.L. */

int linha = 1;          /* linha do caracter lido pelo A.L. */
long offset = 0;        /* posicao do caracter lido pelo A.L. */
FILE *arqli;            /* arquivo que contem programa LI */
int pred = 1;           /* numero de predicados de uma expressao condicional */
int seq = 1;            /* numero do comando sequencial */
int cond = 1;           /* numero da expressao condicional */
int flag_vazio = 1;     /* indica ausencia de expressao no comando if */
long ini_token = 0;     /* posicao do 1o.token do item sintatico */
int comp_token = 0;     /* comprimento do item sintatico */
int comp_ant;           /* comprimento do item sintatico anterior */
int lin_token = 0;      /* linha do 1o.token do item sintatico */
char label[TAMLEX];     /* contem label de tranf.incondicional */
struct no_tipdef *raiz; /* raiz da arvore binaria de tipos definidos */
int flag_typedef = 0;   /* indica ocorrencia de definicao de tipo */
int pilha_grava_li = 0; /* indica a nao gravacao do comando LI */
int flag_dirpre = 0;    /* indica diretiva de pre-processamento */

int tamanho;            /* contem tamanho do identificador */
char ch;                /* ultimo caracter lido */
struct tableout  saida; /* ultimo token lido */
int n_chaves;           /* contem o numero de palavras chaves */
int ntablex;            /* contem o numero de trasicoes do A.L. */

struct table *tab_trans[NLEX]; /* armazena a tabela de transicao */
struct chave *tab_chave[NCHAVE]; /* armazena tabela de palavras chaves */

/* Variaveis Globais utilizadas para criacao do grafo sintatico na
   memoria */

struct cabeca * list, * hdeclaration,
		     * hstatement, * hexpression;
struct cabeca * sentinel;
struct cabeca * h;
struct nodo *p;
struct nter sym;
struct tableout saida;

/* Variaveis Globais que apontam para Arqivos */

FILE * arqfonte;
FILE * arqparser;

/* Variavel com o caminho das tabelas */

char * lipath;

/* Variavel com o caminho dos arquivos de include */

char *includes[MAXINC];

/* numero de linhas de codigo sem comentario */

int last_loc =0, loc = 0;

/* lista de resultados */

GOTOLABEL * lista_resul=(GOTOLABEL *) NULL ;

/* corpo da funcao main */

void main(argc,argv)
int argc;
char * argv[];
{

/* Declaracao de Variaveis Locais */

 int i, match;
 int imprime_tabs=0; /* Flag que controla a impressao da tabela de tipos definidos */

 char nom_arq[500];
 GOTOLABEL * aux_lista;

 /* Trata os argumentos passados na linha de comando */

 imprime_tabs=trata_args(argc,argv);

 /* Inicia os apontadores para a tabela de transicao e tabela de palavras
    chaves com o valor nulo */

 for(i=0; i>=0 && i < NLEX; ++i)
	 tab_trans[i] = (struct table *) NULL;
 for(i=0; i>=0 && i < NCHAVE; ++i)
	 tab_chave[i] = (struct chave *) NULL;

 /* Inicia apontadores para o grafo sintatico do parser especifico */

 list = (struct cabeca *) NULL;
 h = (struct cabeca *) NULL;
 hdeclaration = (struct cabeca *) NULL;
 hstatement = (struct cabeca *) NULL;
 hexpression = (struct cabeca *) NULL;
 p = (struct nodo *) NULL;


 

 lipath=getenv("NEWLITABS");
 if (lipath == (char *) NULL)
     error("* * Erro Fatal (li/li.c:133): Nao consegui encontrar caminho das tabelas * *\n");

 strcpy(nom_arq,lipath);

#ifdef TURBO_C
 strcat(nom_arq,"\\tabsin.");
#else
 strcat(nom_arq,"/tabsin.");
#endif

 strcat(nom_arq,argv[2+imprime_tabs]);
 arqparser = fabre(nom_arq, "r");


 strcpy(nom_arq,argv[1+imprime_tabs]);
 strcat(nom_arq,".");
 strcat(nom_arq,argv[2+imprime_tabs]);

#if defined(TURBO_C) || defined(DJGPP)
 arqfonte = fabre (nom_arq, "rb");
#else
 arqfonte = fabre (nom_arq, "r");
#endif

 strcpy(nom_arq,argv[1+imprime_tabs]);
 strcat(nom_arq,".li");
 arqli = fabre (nom_arq, "w+");

 carregador(argv[2+imprime_tabs]);  /* funcao que carrega tabela de transicoes e palavras
			  reservadas do analisador lexico */

 car_tipo(argv[2+imprime_tabs],lipath);    /* funcao que carrega tabela de tipos do sistema */

 producao();  /* funcao que carrega grafos sintaticos */

 h = list;
 while(h != sentinel)
  {
   if(!strcmp(h->nome,"stm_list"))
      break; /* h contem apontador para "stm_list" */
   h = h->suc;
  }

 yylex();
 match = parser(list,VERDADEIRO);
 if (match==VERDADEIRO && saida.classe[1]=='\0')
   {
    if(!imprime_tabs)
	{
    	printf("---------------------------------------------------------------------\n");
    	printf("|Nome da Funcao                  | No. Linhas de Codigo Executaveis |\n");
    	printf("---------------------------------------------------------------------\n");

    	for(aux_lista=lista_resul; aux_lista!=NULL;aux_lista = aux_lista->next)
		{
        	printf("|%-32s|               %5d              |\n",aux_lista->name, aux_lista->id);
        	printf("---------------------------------------------------------------------\n");
		}

    	printf("| Total %-25s|               %5d              |\n",
					 strcat(argv[1],".c") ,loc);
    	printf("---------------------------------------------------------------------\n");
	}
   }
 else
     {
     printf("* * Erro Fatal, linha %d: Analise Sintatica FALHOU por nao reconhecer atomo '%s %s'! * *\n",saida.linha,saida.label, saida.classe);
     error("");
     } 

/* Gera tabela de Tipos de Definidos para o pokernel */

if(imprime_tabs)
 	imprime_tipos(raiz);

 libera("");
 fclose(arqfonte);
 fclose(arqli);
 fclose(arqparser);
 exit(0);
}


FILE * fabre(file_name, mode )
char file_name[200];
char mode[10];
{
 FILE * ptr_arq;
 ptr_arq = (FILE *) fopen(file_name, mode);

 if(ptr_arq == (FILE *) NULL)
     {
      printf("* * Erro Fatal: Nao consegui abrir arquivo '%s' * *\n", file_name);
      exit(1);
     }

 return (ptr_arq);
}

void msg_print(s)
char * s;
{
 printf("%s",s);
}

void insert_list(description, id, start)
char * description;
int id;
GOTOLABEL ** start;
{

 GOTOLABEL * name_aux, * new;

 

 new = (GOTOLABEL *) malloc(sizeof(GOTOLABEL));
 if(new == (GOTOLABEL *) NULL)
     error("* * Erro Fatal: Nao consegui alocar espaco para o vetor de descricoes das variaveis (names) * * r");

 /* Insere informacao */

 strcpy(new->name,description);
 new->id = id;
 new->next = NULL;

 /* posiciona no final da lista */

 if(*start != NULL)
     {
     for(name_aux = *start; name_aux->next != NULL; name_aux = name_aux->next);
     name_aux->next = new;
     }
 else
     *start = new;
 return;
}

void clear_list(list)
GOTOLABEL * list;
{
if(list != NULL)
	{
	clear_list(list->next);
	free(list);
	}
}

int trata_args(argc,argv)
int argc;
char *argv[];
{

/* Declaracao de Variaveis Locais */

int gera_tab_tipos = 0;

 if(argc > 1 && argv[1]!= NULL)
 	if(!strcmp(argv[1],"-t"))
		gera_tab_tipos = 1;

 if (argc <  3+gera_tab_tipos)
  {
    printf("Uso: newli <nome_arquivo> <extensao_ling> -I<dir com includes>\n");
    exit(1);
  }

if(!gera_tab_tipos)
  {
   printf("li 2.2 -- Gerador da Linguagem Intermediaria para Programas em C\n");
   printf("(C) Copyright 1995 Grupo de Teste Software DCA/FEE/UNICAMP\n");
  }

 if(argc >= 3+gera_tab_tipos) 
   {
   int i;
   if(argc-(3+gera_tab_tipos) > MAXINC)
	{
        printf("* * Erro Fatal: Numero excessivo de diretorios de include * *\n");
     	exit(1);
	}

   /* Insere o diretorio corrente na lista de diretorios de Include */

   includes[0] = (char *) malloc(strlen(".")+1);
   if(includes[0] == (char *) NULL)
	{
	printf("* * Erro Fatal: Nao consegui alocar espaco em memoria!\n");
	exit(1);
	}	

   strcpy(includes[0],".");

   for(i=3+gera_tab_tipos; i < argc; ++i)
   	if(strncmp(argv[i],"-I",2))
    		{
     		printf("* * Erro Fatal:\"%s\" descricao incorreta do diretorio de include * *\n",argv[i]);
     		exit(1);
    		}
   	else
		{
		includes[i-(3+gera_tab_tipos)+1] = (char *) malloc(strlen(argv[i]+2)+1); 

		if(includes[i-(3+gera_tab_tipos)+1] == (char *) NULL)
			{
			printf("* * Erro Fatal: Nao consegui alocar espaco em memoria!\n");
			exit(1);
			}	
      		strcpy(includes[i-(3+gera_tab_tipos)+1],argv[i]+2);
		}
   }
return(gera_tab_tipos);
}
