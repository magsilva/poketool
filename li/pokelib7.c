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
// poke_lib7.c 	%I%	%Y%	%E%	%U%
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


void car_tipo(nomling,tabpath)
char * nomling;
char * tabpath;
{

 /* Declaracao de Variaveis Locais */

 char linha[200];
 char nomtabs[500];
 FILE * fp;

/*
  msg_print("                                                         \r");
  msg_print("* * Carregando Tipos definidos pelo sistema... * *\r");
*/

  strcpy(nomtabs,tabpath);

#ifdef TURBO_C
  strcat(nomtabs,"\\tipos.");
#else
  strcat(nomtabs,"/tipos.");
#endif

  strcat(nomtabs,nomling);
  if ((fp=fopen(nomtabs,"r")) == 0)
     error("* * Erro Fatal: nao consegui abrir arquivo \"tipos.tes\"* *\n");
  raiz = NULL;
  while (fscanf(fp,"%s",linha) != EOF)
     { 
       int n_match = (int) NULL;
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

