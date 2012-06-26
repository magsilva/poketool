/*
// parserg2.c 	1.1		94/12/26	11:27:05
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef TURBO_C
#include <malloc.h>
#else
#include <alloc.h>
#endif

#include "util.h"
#include "headerli.h"
#include "hparserg.h"
#include "tabelasli.h"
#include "hrotsem.h"
#include "hli.h"

/* Declaracao de Variaveis Globais */

extern struct cabeca * list, * hdeclaration,
		     * hstatement, * hexpression;
extern struct cabeca * sentinel;
extern struct cabeca * h;
extern struct nodo *p;
extern struct nter sym;
extern struct tableout saida;
extern FILE * arqfonte;
extern char oldlabel[];

#ifdef DEBUG

extern int debug_flag;

#endif

/*******************************************************************/
/* void expressao(struct nodo **, struct nodo **)                  */
/* Autor: Marcos L. Chaim                                          */
/* Data: 2/10/89                                                   */
/* Versao: 1.0                                                     */
/*                                                                 */
/* Funcao: cria grafo sintatico na memoria.                        */
/*                                                                 */
/* Entradas: endereco dos apontadores para os registros do tipo    */
/*           nodo.                                                 */
/*                                                                 */
/* Saida: nenhuma.                                                 */
/*                                                                 */
/*******************************************************************/

void expressao(p,q)
struct nodo **p;
struct nodo **q;
{

 /* Declaracao de Variaveis Locais */

 struct nodo *a, *b, *c;
 termo(p,&a,&c);
 c->suc = (struct nodo *) NULL;
 while(!strcmp(sym.nome,","))
   {
    pegsim();

#ifdef DEBUG
    printf("%s\n",sym.nome);
#endif

    termo(&(a->alt), &b, &c);
    c->suc = (struct nodo * ) NULL;
    a = b;
   }
 *q = a;
}

/*******************************************************************/
/* void producao()                                                 */
/* Autor: Marcos L. Chaim                                          */
/* Data: 2/10/89                                                   */
/* Versao: 1.0                                                     */
/*                                                                 */
/* Funcao: cria grafo sintatico na memoria.                        */
/*                                                                 */
/* Entradas: nenhuma.                                              */
/*                                                                 */
/* Saida: nenhuma.                                                 */
/*                                                                 */
/*******************************************************************/

void producao()
{

 /* Declaracao de Variaveis Locais */

 /*
 msg_print("                                                             \r");
 msg_print("* * Carregando Parser Especifico da Linguagem Fonte... * *\r");
 */

 pegsim();
#ifdef DEBUG
    printf("%s\n",sym.nome);
#endif
 sentinel = (struct cabeca *) malloc(sizeof(struct cabeca));
 if( sentinel == (struct cabeca *) NULL)
	 error(
"* * Erro Fatal: Nao consegui alocar espaco para sentinel em \"producao()\" * *\r");

 list = sentinel;
 while(sym.tipo != FIMDEARQUIVO)
    {
     find(sym.nome, &h);
     pegsim();
     if(!strcmp(sym.nome,"="))
		{pegsim();
#ifdef DEBUG
    printf("%s\n",sym.nome);
#endif
                }
	 else
		error("* * Erro Fatal: Era esperado '=' na descricao do parser * *\r");
     expressao(&(h->entrada),&p);
     p->alt = (struct nodo *) NULL;
	 if(strcmp(sym.nome,"."))
		error("* * Erro Fatal: Era esperado '.' na descricao do parser * *\r");
	 pegsim();
#ifdef DEBUG
    printf("%s\n",sym.nome);
#endif
    }

  /* Verificando se todos os simbolos foram definidos */

 h = list;
 while(h != sentinel)
    {
     if(h->entrada == (struct nodo *) NULL)
                { printf("%s\n", h->nome);
		 error("* * Erro Fatal: Simbolo indefinido na descricao do parser * *\r");
                }
     if(!strncmp(h->nome,"declaration",8))
		 hdeclaration = h;
     if(!strncmp(h->nome,"expression",8))
		 hexpression = h;
     if(!strncmp(h->nome,"statement",8))
		 hstatement= h;
     h = h->suc;
   }
}

/*******************************************************************/
/* char parser(struct cabeca *, char)                              */
/* Autor: Marcos L. Chaim                                          */
/* Versao: 1.0                                                     */
/*                                                                 */
/* Funcao: faz a analise sintatica especifica do codigo fonte.     */
/*                                                                 */
/* Entradas: apontador para o nao terminal a ser analisado e flag  */
/*           indicando o resultado da reducao dos outros nao ter-  */
/*           minais.                                               */
/*                                                                 */
/* Saida: resultado da analise sintatica do nao terminal.          */
/*                                                                 */
/*******************************************************************/

char parser(goal,match)
struct cabeca * goal;
char match;
{

/* Declaracao de Variaveis Locais */

 struct nodo *s;
 s = goal->entrada;
 do
  {
#ifdef DEBUG

  if(debug_flag) 
   if(s->tipo == 't')
    {
      printf("<<%s %s %c >> ",s->nome, saida.classe, match); 
    }
   else
     printf("$$%s %c %s$$ ",s->nome, match, saida.label); 

#endif

  if(s->tipo == 't')
     { /* terminal */
      if(!strcmp(s->nome, saida.classe))
		{
		 match = VERDADEIRO;
#ifdef DEBUG
 		 if (!strcmp(saida.label,"errline_printed"))
		   {
		   printf("%s ", saida.label);
                   debug_flag=0;
		   }
#endif

		 rot_sem(s->sem);
		 if (strcmp(saida.classe,"FALHE")) /* pega o proximo simbolo so'se for diferente de FALHE */
		  yylex();
		}
      else
	   {
		if(s->nome[0] == EMPTY)
		  match = VERDADEIRO;
		else
		  match = FALSO;
	   }
     }
   else
      match = parser(s->nterminal,match);
   if(match == VERDADEIRO)
     {
      if(s->tipo=='n') /* Nao-terminal? Em caso verdadeiro, executa rot_sem. */
	   rot_sem(s->sem);
      s = s->suc;
     }
   else
      s=s->alt;

#ifdef DEBUG
 if(debug_flag) 
   {
     if(s!= NULL)
       if(s->tipo == 't')
	printf("<%s %c %d> ",s->nome, match, s->sem); 
      else
        printf("$%s %c %d %s $ ",s->nome, match, s->sem, saida.classe); 
   }
#endif

  }
  while(s != (struct nodo *) NULL);
  return(match);
 }

/*******************************************************************/
/* void rot_sem(int)                                               */
/* Autor: Marcos L. Chaim                                          */
/* Data: 6/11/89                                                   */
/* Versao: 1.0                                                     */
/*                                                                 */
/* Funcao: selecionar qual rotina semantica associada ao reconhe-  */
/*         cimento de um terminal ou nao-terminal a ser executada. */
/*                                                                 */
/* Entradas: inteiro que especifica qual rotina a ser executada.   */
/*                                                                 */
/* Saida: nenhuma                                                  */
/*                                                                 */
/*******************************************************************/

void rot_sem(rsem)
int rsem;
{

 switch(rsem)
   {
    case 1:  rot1_sem();  break;
    case 2:  rot2_sem();  break;
    case 3:  rot3_sem();  break;
    case 4:  rot4_sem();  break;
    case 5:  rot5_sem();  break;
    case 6:  rot6_sem();  break;
    case 7:  rot7_sem();  break;
    case 8:  rot8_sem();  break;
    case 9:  rot9_sem();  break;
    case 10: rot10_sem(); break;
    case 11: rot11_sem(); break;
    case 12: rot12_sem(); break;
    case 13: rot13_sem(); break;
    case 14: rot14_sem(); break;
    case 15: rot15_sem(); break;
    case 16: rot16_sem(); break;
    case 17: rot17_sem(); break;
    case 18: rot18_sem(); break;
    case 19: rot19_sem(); break;
    case 20: rot20_sem(); break;
    case 21: rot21_sem(); break;
    case 22: rot22_sem(); break;
    case 23: rot23_sem(); break;
    case 24: rot24_sem(); break;
    case 25: rot25_sem(); break;
    case 26: rot26_sem(); break;
    case 27: rot27_sem(); break;
    case 28: rot28_sem(); break;
    case 29: rot29_sem(); break;
    case 30: rot30_sem(); break;
    case 31: rot31_sem(); break;
    case 32: rot32_sem(); break;
    case 33: rot33_sem(); break;
    case 34: rot34_sem(); break;
    case 35: rot35_sem(); break;
    case 36: rot36_sem(); break;
    case 37: rot37_sem(); break;
    case 38: rot38_sem(); break;
    case 39: rot39_sem(); break;
    case 40: rot40_sem(); break;
    case 41: rot41_sem(); break;
    case 42: rot42_sem(); break;
    case 43: rot43_sem(); break;
    case 44: rot44_sem(); break;
    case 45: rot45_sem(); break;
    case 46: rot46_sem(); break;
    case 47: rot47_sem(); break;
    case 48: rot48_sem(); break;
    case 49: rot49_sem(); break;
    case 50: rot50_sem(); break;
    case 51: rot51_sem(); break;
    case 52: rot52_sem(); break;
    case 53: rot53_sem(); break;
    case 54: rot54_sem(); break;
    case 55: rot55_sem(); break;
    case 56: rot56_sem(); break;
    case 57: rot57_sem(); break;
    case 58: rot58_sem(); break;
    case 59: rot59_sem(); break;
    case 60: rot60_sem(); break;
    case 61: rot61_sem(); break;
    case 62: rot62_sem(); break;
    case 63: rot63_sem(); break;
    case 64: rot64_sem(); break;
    case 65: rot65_sem(); break;
    case 66: rot66_sem(); break;      
    default: ;
   }
   return;
 }
