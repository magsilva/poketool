/*
// parserg1.c 	1.1		94/12/26	16:07:08
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef TURBO_C

#include <alloc.h>

#else  /* SunOS, BSD4.2 e RISC 6000 */

#include <malloc.h>

#endif

#include "util.h"


#include "hparserg.h"
#include "headerli.h"

/* Declaracao de Variaveis Globais */

extern struct cabeca *list, * hdeclaration,
	      *hstatement, *hexpression;
extern struct cabeca * sentinel;
extern struct cabeca *h;
extern struct nodo *p;
extern struct nter sym;
extern FILE * arqparser;
extern FILE * arqfonte;

/*******************************************************************/
/* void find(char *, struct cabeca **)                             */
/* Autor: Marcos L. Chaim                                          */
/* Data: 2/10/89                                                   */
/* Versao: 1.0                                                     */
/*                                                                 */
/* Funcao: localiza simbolo na lista de nao terminais, se nao pre- */
/*         sente, insere-o.                                        */
/*                                                                 */
/* Entradas: string de caracteres com o simbolo nao terminal a ser */
/*           localizado e endereco do apontador do registro do ti- */
/*           po cabeca que contera' o simbolo nao terminal locali- */
/*           zado.                                                 */
/*                                                                 */
/* Saida: nenhuma.                                                 */
/*                                                                 */
/* Variaveis Globais Utilizadas: list e sentinel.                  */
/*                                                                 */
/*******************************************************************/

void find(s,h)
char *s;
struct cabeca ** h;
{

 /* Declaracao de Variaveis Locais */

 struct cabeca * h1;
 strcpy(sentinel->nome, s);
 for(h1 = list; strcmp(h1->nome,s); h1=h1->suc);
 if(h1 == sentinel)
   {
    sentinel = (struct cabeca *) malloc(sizeof(struct cabeca));
    if(sentinel == (struct cabeca *) NULL)
	error("* * Erro Fatal: Nao consegui alocar espaco para sentinel * * \n");
	h1->suc = sentinel;
	sentinel->suc = (struct cabeca *) NULL;
    h1->entrada = (struct nodo *) NULL;
   }
 *h=h1;
 return;
}

/*******************************************************************/
/* void fator(struct nodo **, struct nodo **)                      */
/* Autor: Marcos L. Chaim                                          */
/* Data: 2/10/89                                                   */
/* Versao: 1.0                                                     */
/*                                                                 */
/* Funcao: cria grafo sintatico.                                   */
/*                                                                 */
/* Entradas: enderecos dos apontadores para os registros do tipo   */
/*           nodo.                                                 */
/*                                                                 */
/* Saida: nenhuma.                                                 */
/*                                                                 */
/*******************************************************************/

void factor(p,q)
struct nodo **p;
struct nodo **q;
{

 /* Declaracao de Variaveis Locais */

 struct nodo * a, * b;
 struct cabeca * h;
 if(sym.tipo == 't' || sym.tipo == 'n' || sym.nome[0] == EMPTY)
    {
     a = (struct nodo *) malloc(sizeof(struct nodo));
     if(a== (struct nodo *) NULL)
        error("* * Erro Fatal: Nao consegui alocar espaco para \"a\" em factor() * *\n");
     a->suc = (struct nodo *) NULL;
     a->alt = (struct nodo *) NULL;
     if(sym.tipo == 'n')  /* nao terminal */
	  {
	   find(sym.nome,&h);
	   a->tipo = 'n';
	   a->nterminal = h;
	   strcpy(a->nome,sym.nome);
	   pegsim();
	   if(sym.tipo == 'r') /* numeral */
		{
		 a->sem = sym.num;
		 pegsim();
		}
	   else a->sem = 0;
	}
       else /* terminal */
	 {
	  a->tipo = 't';
	  if(sym.nome[0] == EMPTY) /* empty ? */
	      {
	       a->nome[0] = EMPTY;
	       a->nome[1] = '\0';
	      }
	  else
	       strcpy(a->nome, sym.nome);
          pegsim();
	  if(sym.tipo == 'r') /* numeral */
	     {
	      a->sem = sym.num;
	      pegsim();
	     }
	  else a->sem = 0;
	  a->nterminal = (struct cabeca *)NULL;
	}
       *p = a; *q=a;
    }
 else
	{
	 if(!strcmp(sym.nome,"["))
	   {
	    pegsim();
	    termo(p,&a,&b);
	    b->suc = *p;
	    b = (struct nodo *) malloc(sizeof(struct nodo));
	    if (b == (struct nodo *) NULL)
	       error("* * Erro Fatal: Nao consegui alocar espaco para \"b\" em factor() * *\n");
            b->suc = (struct nodo *) NULL;
            b->alt = (struct nodo *) NULL;
	    b->tipo = 't';
	    b->nome[0] = EMPTY;
            b->nome[1] = '\0';
	    b->sem = 0;
	    a->alt = b;
	    *q = b;
	    if(!strcmp(sym.nome,"]"))
	      pegsim();
		else error(
		"* * Error Fatal: Nao estou conseguindo carregar o grafo sintatico... * *\n");
	   }
	 else error(
	 "* * Error Fatal: Nao estou conseguindo carregar o grafo sintatico... * *\n");
	}
 }
/*******************************************************************/
/* void termo(struct nodo **, struct nodo **, struct nodo **)      */
/* Autor: Marcos L. Chaim                                          */
/* Data: 2/10/89                                                   */
/* Versao: 1.0                                                     */
/*                                                                 */
/* Funcao: cria grafo sintatico.                                   */
/*                                                                 */
/* Entradas: enderecos dos apontadores para os registros do tipo   */
/*           nodo.                                                 */
/*                                                                 */
/* Saida: nenhuma.                                                 */
/*                                                                 */
/*******************************************************************/

void termo(p,q,r)
struct nodo **p;
struct nodo **q;
struct nodo **r;
{

 /* Declaracao de Variaveis Locais */

 struct nodo * a, *b;
 factor(p,&a);
 *q = a;
 while(sym.tipo == 't' || sym.tipo == 'n' || !strcmp(sym.nome,"[")
       || sym.nome[0] == EMPTY)
       {
	factor(&(a->suc),&b);
	b->alt = (struct nodo *) NULL;
	a = b;
       }
 *r = a;
}
