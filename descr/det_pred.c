#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef TURBO_C

#include <alloc.h>

#else  /* SunOS, BSD4.2 e RISC 6000 */

#include <malloc.h>

#endif

#include "util.h"

/*******************************************************************/
/* struct grafo * det_pred(struct grafo *, int);                   */
/* Autor: Marcos L. Chaim                                          */
/* Versao: 1.0                                                     */
/* Data: 16/06/89                                                  */
/*                                                                 */
/* FUNCAO: esta funcao cria, a partir de um grafo de fluxo de con- */
/*         representado por listas de sucessores, um outro grafo   */
/*         analogo porem representado por listas de sucessores.    */
/*                                                                 */
/* ENTRADA:ponteiro para o novo grafo representado por lista de    */
/*         sucessores e o numero de nos desse grafo.               */
/*                                                                 */
/* SAIDA:  ponteiro para o novo grafo de lista de predecessores.   */
/*                                                                 */
/* FUNCOES CHAMADAS: void ins_elem().                              */
/*                                                                 */
/*******************************************************************/

#include "hrotsem.h"
#include "header.h"
#include "hparserg.h"

struct grafo *det_pred(g,n)
struct grafo *g;
int n;
{
/* declaracao de variaveis locais */

struct grafo *g_pred;
struct no *noaux;
int i;

/* iniciando o grafo de predecessores */

g_pred=(struct grafo *) malloc((n+1)*sizeof(struct grafo));
if(g_pred==NULL)
      error("* * Erro Fatal: Nao consegui alocar \
      espaco para os conjuntos de predecessores * *\n");
for(i=1;i<=n;++i)
      g_pred[i].list_suc=NULL;

/* construindo o grafo de predecessores */

for(i=1;i<=n;++i)
     {
      for(noaux=g[i].list_suc;noaux!=NULL;noaux=noaux->proximo)
	ins_elem(&(g_pred[noaux->num].list_suc),i);
     }
return(g_pred);
}

/*******************************************************************/
/* int * cal_rpos(int, struct grafo *, int *, int *)               */
/* Autor: Marcos L. Chaim                                          */
/* Versao: 1.0                                                     */
/* Data: 19/06/89                                                  */
/*                                                                 */
/* FUNCAO: esta funcao determina a ordenacao rPOSTORDER dos nos    */
/*         de um grafo de fluxo de controle atraves de uma busca   */
/*         depth-first no grafo.                                   */
/*                                                                 */
/* ENTRADA:no' onde iniciara' a busca depth-first, ponteiro para o */
/*         grafo de fluxo controle, apontador para o inteiro que   */
/*         indicara' quando o no' foi visitado e vetor de inteiros */
/*         contendo a ordenacao rPOSTORDER.                        */
/*                                                                 */
/* SAIDA:  apontador para o vetor contendo a ordenacao rPOSTORDER. */
/*                                                                 */
/* FUNCOES CHAMADAS: atr_conj(), cal_rpos() e ret_elem().          */
/*                                                                 */
/*******************************************************************/

int * cal_rpos(x,g,p,rpost)
int x;
struct grafo *g;
int *p;
int *rpost;
{
 /* declaracao de variaveis locais */
 struct no *suc;
 int y;

 suc=NULL;
 rpost[x]=0;		 /* marcando o no' que esta' sendo visitado */
 atr_conj(&suc,&(g[x].list_suc));  /* atribuindo a lista de sucessores de x para suc */

 while(suc!=NULL)
     {
      y=ret_elem(&suc);
      if(rpost[y] < 0)                 /* testando se o no' foi visitado */
	  rpost=cal_rpos(y,g,p,rpost); /* nao foi visitado entao visite-o */
     }
 rpost[x]= *p;
 --(*p);
 return(rpost);
}
