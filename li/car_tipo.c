/*
// car_tipo.c 	%I%	%Y%	%E%	%U%
*/

#include <stdio.h>
#include <string.h>

#ifndef TURBO_C
#include <malloc.h>
#else
#include <alloc.h>
#endif

#include "util.h"
#include "hparserg.h"
#include "tabelasli.h"
#include "hli.h"

/* variaveis globais */

extern struct no_tipdef * raiz;
extern char * lipath;


void car_tipo(nomling)
char * nomling;
{

 /* Declaracao de Variaveis Locais */

 struct no_tipdef *tree_tipo PROTO ( (struct no_tipdef*,char*,int,int* ) );
 char linha[200];
 char nomtabs[500];
 FILE * fp;

/*
  msg_print("                                                         \r");
  msg_print("* * Carregando Tipos definidos pelo sistema... * *\r");
*/

  strcpy(nomtabs,lipath);

#ifdef TURBO_C
  strcat(nomtabs,"\\tipos.");
#else
  strcat(nomtabs,"/tipos.");
#endif

  strcat(nomtabs,nomling);
  if ((fp=fopen(nomtabs,"r")) == 0)
     error("Cannot open file tipos.c");
  raiz = NULL;
  while (fscanf(fp,"%s",linha) != EOF)
     { 
       int n_match = NULL;
       raiz = tree_tipo(raiz,linha,1,&n_match);
     }
  fclose(fp);
}

struct no_tipdef *tree_tipo (p , w, tipodef, nomatch)

struct no_tipdef *p;
char *w;
int tipodef; 
int *nomatch;
{
 int cond;

 if (p==NULL)
 {
    if (tipodef)
     {
	p = (struct no_tipdef *) malloc(sizeof(struct no_tipdef));
	if ((p->tipdef = malloc(strlen(w)+1)) != NULL)
	   strcpy(p->tipdef,w);
	p->esq = p->dir = NULL;
     }
 }
 else if ((cond = strcmp(w,p->tipdef)) < 0)
	 p->esq = tree_tipo(p->esq,w,tipodef, nomatch);
      else if (cond > 0)
	       p->dir = tree_tipo(p->dir,w,tipodef,nomatch);
	   else
	       *nomatch = 0;
 return p;
}

void lib_tipo(p)
struct no_tipdef *p;
{

 if (p!=NULL)
 {
   lib_tipo(p->esq);
   lib_tipo(p->dir);
   free(p->tipdef);
   free(p);
 }
}

void print_tree(p,f)
struct no_tipdef *p;
FILE * f;
{

 if (p!=NULL)
 {
   fprintf(f,"%s\n",p->tipdef);
   print_tree(p->esq,f);
   print_tree(p->dir,f);
 }
}

void imprime_tipos(p)
struct no_tipdef *p;
{
FILE * tipfile;

tipfile=(FILE *) fopen("tipos.tes","w");

if(tipfile == (FILE *) NULL)
	error("* * Erro Fatal: Nao consegui abri arquivo tipos.tes * *");

print_tree(p,tipfile);
fclose(tipfile);
}

