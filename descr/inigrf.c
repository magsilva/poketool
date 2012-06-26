/********************************************************************/
/* Funcao ini_grf(char *, struct grafo**)                           */
/* Autor: Marcos L. Chaim                                           */
/* Data: 4/06/89                                                    */
/*                                                                  */
/* TAREFA: esta funcao tem como entrada um arquivo que contem um    */
/*         grafo representado como uma lista de sucessores cada no' */
/*         e o endereco de um apontador para a estrutura de dados   */
/*         que contera' o grafo na memoria. A funcao ira' colocar   */
/*         o grafo na memoria e retornara' o numero de nos desse    */
/*         desse grafo.                                             */
/*                                                                  */
/* ENTRADA: string de caracteres com o nome do arquivo e o endere-  */
/*          co do apontador para o grafo.                           */
/* SAIDA:   inteiro contendo o numero de nos do grafo.              */
/********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef TURBO_C

#include <alloc.h>

#else  /* SunOS, BSD4.2 e RISC 6000 */

#include <malloc.h>

#endif

#include "util.h"

#include "hrotsem.h"
#include "header.h"
#include "hparserg.h"

int inigrf(fnome, g)
char *fnome;
struct grafo **g;

{
/* declaracao de variaveis locais */

int i,j,k,n;
struct grafo *gaux;
FILE *fp;

/* iniciando */

fp=fopen(fnome,"r");      /* abrindo o arquivo que contem o grafo de fluxo de controle */
if(fp==NULL)
   error("* * Erro Fatal: Nao consegui abrir o arquivo com o grafo de fluxo de controle * *\n");

fscanf(fp,"%d",&n); /* obtive numero de nos */
gaux=(struct grafo *)malloc((n+1)*sizeof(struct grafo)); /* aloquei memoria para o nos do grafo */

if(gaux==NULL)
  error("* * Erro Fatal: Nao consegui alocar espaco para o grafo * *\n");

/* iniciando com todos os apontadores de listas de sucessores */

for(i = 0; i <= n; ++i)
  gaux[i].list_suc = (struct no *) NULL;

/* construindo o grafo na memoria */

k=1;

while(fscanf(fp,"%d",&i)!=EOF && k<=n)
     {
      (gaux+i)->num=i;
      (gaux+i)->list_suc=NULL;
      if(i>n || i<=0)
          error("* * Erro Fatal: Este grafo nao esta' \
	  especificado corretamente * *\n");
      for(fscanf(fp,"%d",&j);j!=0;fscanf(fp,"%d",&j))
	   ins_elem(&((gaux+i)->list_suc),j);
      ++k;
     }
if(fscanf(fp,"%d",&i)!=EOF)
    error("* * Erro Fatal: Mais nos do que o especificado * *\n");

fclose(fp);
*g=gaux;
return(n);
}

/********************************************************************/
/* void lib_grf(struct grafo * , int)                               */
/* Autor: Marcos L. Chaim                                           */
/* Versao: 1.0                                                      */
/* Data: 20/07/89                                                   */
/*                                                                  */
/* FUNCAO: liberar espaco ocupado por um grafo.                     */
/*                                                                  */
/* ENTRADA:apontador para o grafo e numero de nos do grafo.         */
/*                                                                  */
/* SAIDA:  nenhuma.                                                 */
/*                                                                  */
/* FUNCOES CHAMADAS: lib_conj().                                    */
/*                                                                  */
/********************************************************************/

void lib_grf(g,n)
struct grafo *g;
int n;
{

 /* declaracao de variaveis locais */
 int i;
 for(i=1;i<=n;++i)
    lib_conj(g[i].list_suc);
 if(g != (struct grafo *) NULL)
    free(g);
 return;

}

/********************************************************************/
/* void lib_dom(struct con_int * , int)                             */
/* Autor: Marcos L. Chaim                                           */
/* Versao: 1.0                                                      */
/* Data: 20/07/89                                                   */
/*                                                                  */
/* FUNCAO: liberar espaco ocupado pelo conjunto de dominadores      */
/*                                                                  */
/* ENTRADA:apontador para o conjunto de dominadores e numero de nos */
/*         do grafo.                                                */
/*                                                                  */
/* SAIDA:  nenhuma.                                                 */
/*                                                                  */
/* FUNCOES CHAMADAS: lib_conj().                                    */
/*                                                                  */
/********************************************************************/

void lib_dom(dom,n)
struct conj_int *dom;
int n;
{

 /* declaracao de variaveis locais */
 int i;
 for(i=1;i<=n;++i)
    lib_conj(dom[i].apont);
 if(dom != (struct conj_int *) NULL)
    free(dom);
 return;

}

