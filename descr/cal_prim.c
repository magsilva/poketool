#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef TURBO_C

#include <alloc.h>

#else  /* SunOS, BSD4.2 e RISC 6000 */

#include <malloc.h>

#endif

#include "util.h"

/********************************************************************/
/* int satisR1(int, struct no *, struct grafo *, struct grafo *)    */
/* Autor: Marcos L. Chaim                                           */
/* Versao: 1.0                                                      */
/* Data: 28/06/89                                                   */
/*                                                                  */
/* FUNCAO: verifica se um dado arco satisfaz a regra R1 de redu-    */
/*         cao de Chusho.                                           */
/*                                                                  */
/* ENTRADA:numero do no' fonte do arco, apontador para o no' alvo   */
/*         do arco no grafo de sucessores e apontador para o grafo  */
/*         de predecessores.                                        */
/*                                                                  */
/* SAIDA:  inteiro indicando (TRUE ou FALSE) se o arco satisfaz a   */
/*         R1.                                                      */
/*                                                                  */
/* FUNCOES CHAMADAS: cardinal().                                    */
/*                                                                  */
/********************************************************************/

#include "hrotsem.h"
#include "header.h"
#include "hparserg.h"

int satisR1(fonte,alvo,g_pred,g_suc)
int fonte;
struct no *alvo;
struct grafo *g_pred;
struct grafo *g_suc;
{
 if(fonte==alvo->num) return(FALSE);   /* condicao 1 */

 if((cardinal(g_pred[fonte].list_suc)!=0)  && (cardinal(g_suc[fonte].list_suc)==1))
	    return(TRUE); /* IN(x)!=0 && OUT(x)==1 */
 else
	    return(FALSE);
}


/********************************************************************/
/* int satisR2(int, struct no *, struct grafo *, struct grafo *)    */
/* Autor: Marcos L. Chaim                                           */
/* Versao: 1.0                                                      */
/* Data: 28/06/89                                                   */
/*                                                                  */
/* FUNCAO: verifica se um dado arco satisfaz a regra R2 de redu-    */
/*         cao de Chusho.                                           */
/*                                                                  */
/* ENTRADA:numero do no' fonte do arco, apontador para o no' alvo   */
/*         do arco no grafo de sucessores e apontador para o grafo  */
/*         de predecessores.                                        */
/*                                                                  */
/* SAIDA:  inteiro indicando (TRUE ou FALSE) se o arco satisfaz a   */
/*         R2.                                                      */
/*                                                                  */
/* FUNCOES CHAMADAS: cardinal().                                    */
/*                                                                  */
/********************************************************************/

int satisR2(fonte,alvo,g_pred,g_suc)
int fonte;
struct no *alvo;
struct grafo *g_pred;
struct grafo *g_suc;
{

 /* declaracao de variaveis locais */

 struct no  *list0_pred=(struct no *)NULL, *list0_sc=(struct no *)NULL, *list_pred=(struct no *)NULL, *list_sc=(struct no *)NULL;
 int i,o, retorno=FALSE;

 /* calculando os predecessores e os sucessores de x (fonte) e y(alvo->num) */
 in(0,fonte,g_suc,g_pred,&list0_pred,0);
 out(0,fonte,g_suc,g_pred,&list0_sc,0);


 i=in(0,alvo->num,g_suc,g_pred,&list_pred,0);
 o= out(0,alvo->num,g_suc,g_pred,&list_sc,0);

 if(fonte==alvo->num || (e_igual(list0_pred,list_pred) && e_igual(list0_sc,list_sc)))
     return(retorno);  /* condicao 1 */

 if((i==1) && (o!=0))
	    retorno=TRUE; /*  IN(y)==1 && OUT(y)!=0 */

 lib_conj(list_pred);
 lib_conj(list_sc);
 lib_conj(list0_pred);
 lib_conj(list0_sc);


 return(retorno);
}

/********************************************************************/
/* int satisR4(int, struct no *, struct grafo *, struct grafo *,    */
/* struct conj_bit *)                                               */
/* Autor: Marcos L. Chaim                                           */
/* Versao: 1.0                                                      */
/* Data: 28/06/89                                                   */
/*                                                                  */
/* FUNCAO: verifica se um dado arco satisfaz a regra R4 de redu-    */
/*         cao de Chusho.                                           */
/*                                                                  */
/* ENTRADA:numero do no' fonte do arco, apontador para o no' alvo   */
/*         do arco no grafo de sucessores, apontador para o grafo   */
/*         de predecessores e apontador para o conjunto de domina-  */
/*	   dores dos nos do grafo.                                  */
/*                                                                  */
/* SAIDA:  inteiro indicando (TRUE ou FALSE) se o arco satisfaz a   */
/*         R4.                                                      */
/*                                                                  */
/* FUNCOES CHAMADAS: cardinal(), in() e ret_elem().                 */
/*                                                                  */
/********************************************************************/

int satisR4(fonte,alvo,g_pred,g_suc,dom)
int fonte;
struct no *alvo;
struct grafo *g_pred;
struct grafo *g_suc;
struct conj_bit *dom;
{

 /* declaracao de variaveis locais */

 struct no *list0_pred=(struct no *)NULL, *list0_sc=(struct no *)NULL, *list_pred=(struct no *)NULL, *list_sc=(struct no *)NULL;
 int i,w, retorno=FALSE;

 /* verifica se o no' alvo e' no terminal */

 if(g_suc[alvo->num].list_suc == (struct no *) NULL)
    return(retorno);

 /* calculando os predecessores e os sucessores de x (fonte) e y(alvo->num) */

 in(0,fonte,g_suc,g_pred,&list0_pred,0);
 out(0,fonte,g_suc,g_pred,&list0_sc,0);


 i=in(0,alvo->num,g_suc,g_pred,&list_pred,0);
   out(0,alvo->num,g_suc,g_pred,&list_sc,0);

 if(fonte==alvo->num || (e_igual(list0_pred,list_pred) && e_igual(list0_sc,list_sc)))
    {
     return(retorno);  /* condicao 1 */
    }

 if(i>=2)              /* IN(y)>=2 */
    {
     del_elem(&list_pred,fonte);
     while(list_pred!=(struct no *)NULL)
       {
	retorno=TRUE;
	w=ret_elem(&list_pred);
	if(alvo->num!=w && !test_bit(alvo->num,&dom[w].set))   /* y pertence a DOM[w] e x!=w */
	    {
	     retorno=FALSE;
	     break;
	    }
       }
     }
 lib_conj(list_pred);      /* libera listas */
 lib_conj(list_sc);
 lib_conj(list0_pred);
 lib_conj(list0_sc);

 return(retorno);
}


/********************************************************************/
/* int cardinal(struct no *)                                        */
/* Autor: Marcos L. Chaim                                           */
/* Versao: 1.0                                                      */
/* Data: 28/06/89                                                   */
/*                                                                  */
/* FUNCAO: calcula a cardinalidade de um conjunto representado por  */
/*         uma lista de sucessores.                                 */
/*                                                                  */
/* ENTRADA:apontador para a lista.                                  */
/*                                                                  */
/* SAIDA:  valor da cardinalidade do conjunto.                      */
/*                                                                  */
/********************************************************************/

int cardinal(lista)
struct no *lista;
{

 /* declaracao de variaveis locais */

 struct no *aux;
 int i;
 i=0;

 for(aux=lista;aux!=(struct no *)NULL;aux=aux->proximo)
		++i;
 return(i);
}

/********************************************************************/
/* int out_no_mark(int,struct grafo *)                              */
/* Autor: Marcos L. Chaim                                           */
/* Versao: 1.0                                                      */
/* Data: 28/06/89                                                   */
/*                                                                  */
/* FUNCAO: verifica se existe pelo menos um arco de saida de um da- */
/*         dado no' sem marca de herdeiro.                          */
/*                                                                  */
/* ENTRADA:no' de onde saem os arcos e o apontador para o grafo de  */
/*         fluxo de controle.                                       */
/*                                                                  */
/* SAIDA:  inteiro indicando (TRUE ou FALSE) a existencia de pelo   */
/*         menos um arco sem marca de herdeiro.                     */
/*                                                                  */
/********************************************************************/
int out_no_mark(x,y,g_suc,g_pred,retorno)
int x;
int y;
struct grafo *g_suc;
struct grafo *g_pred;
int retorno;
{

 /* declaracao de variaveis locais */

 struct no *noaux;

 /* search forward */

 for(noaux=g_suc[y].list_suc;noaux!=(struct no *)NULL && retorno!=TRUE;noaux=noaux->proximo)
	{
	 if(noaux->marca=='p')
	     retorno=TRUE;
	 else
	    {
	     if(noaux->num!=x && noaux->marca!='t')
		      retorno=out_no_mark(y,noaux->num,g_suc,g_pred,retorno);
	    }
	}

 /* search backward */

 for(noaux=g_pred[y].list_suc;noaux!=(struct no *)NULL && retorno!=TRUE;noaux=noaux->proximo)
	if(noaux->marca=='h' && x!=noaux->num)
		      retorno=out_no_mark(y,noaux->num,g_suc,g_pred,retorno);

 return(retorno);
}


/********************************************************************/
/* void elimina_arco(int,struct no *,struct grafo *)                */
/* Autor: Marcos L. Chaim                                           */
/* Versao: 1.0                                                      */
/* Data: 28/06/89                                                   */
/*                                                                  */
/* FUNCAO: elimina um arco do grafo de fluxo de controle marcando-o */
/*         como um arco herdeiro.                                   */
/*                                                                  */
/* ENTRADA:numero do no' fonte do arco, apontador para o no' alvo   */
/*         do arco no grafo de sucessores, apontador para o grafo   */
/*         de predecessores.                                        */
/*                                                                  */
/* SAIDA:  nenhuma.                                                 */
/*                                                                  */
/********************************************************************/

void elimina_arco(fonte,alvo,g_pred)
int fonte;
struct no *alvo;
struct grafo *g_pred;
{

 /* declaracao de variaveis locais */
 struct no *aux;

 alvo->marca='h';

 /* eliminando no grafo de predecessores */

 for(aux=g_pred[alvo->num].list_suc;aux!=(struct no *)NULL;aux=aux->proximo)
    {
     if(aux->num==fonte)
	{
	 aux->marca='h';
	 break;
	}
     }
}

/********************************************************************/
/* void marca_arco(int,struct no *,struct grafo *)                  */
/* Autor: Marcos L. Chaim                                           */
/* Versao: 1.0                                                      */
/* Data: 28/06/89                                                   */
/*                                                                  */
/* FUNCAO: marca um arco do grafo de fluxo de controle marcando-o   */
/*         com uma marca de herdeiro ('t').                         */
/*                                                                  */
/* ENTRADA:numero do no' fonte do arco, apontador para o no' alvo   */
/*         do arco no grafo de sucessores, apontador para o grafo   */
/*         de predecessores.                                        */
/*                                                                  */
/* SAIDA:  nenhuma.                                                 */
/*                                                                  */
/********************************************************************/

void marca_arco(fonte,alvo,g_pred)
int fonte;
struct no *alvo;
struct grafo *g_pred;
{

 /* declaracao de variaveis locais */

 struct no *aux;

 alvo->marca='t';

 /* marcando no grafo de predecessores */

 for(aux=g_pred[alvo->num].list_suc;aux!=(struct no *)NULL;aux=aux->proximo)
    {
     if(aux->num==fonte)
	{
	 aux->marca='t';
	 break;
	}
     }
}

/********************************************************************/
/* int find_path(int,int,int,struct grafo *,struct grafo *,int,\    */
/* struct no **)                                                    */
/* Autor: Marcos L. Chaim                                           */
/* Versao: 1.0						            */
/* Data: 26/06/89                                                   */
/*                                                                  */
/* FUNCAO: verificar se existe um caminho contendo pelo menos um    */
/*         um arco onde nao existe uma marca de herdeiro.           */
/*                                                                  */
/* ENTRADA:no' de partida, no' de chegada, status das marcas no ca- */
/*         minho, apontador para o grafo representado por listas de */
/*         sucessores, apontador para o grafo representado por lis- */
/*         tas de predecessores, no de inicio da pesquisa, lista de */
/*         nos do caminho sendo pesquisado, respectivamente.        */
/*                                                                  */
/* SAIDA:  inteiro indicando(TRUE ou FALSE) se existe pelo menos um */
/*         caminho que satisfaz a condicao.                         */
/*                                                                  */
/* FUNCOES CHAMADAS: atr_conj(), del_elem(), find_path(),ins_elem(),*/
/*                   membro(), tem_marca(), ret_elem().             */
/*                                                                  */
/********************************************************************/



int find_path(x,y,sem_marca,g_pred,g_suc,ini_pesq,list_prof)
int x;
int y;
int sem_marca;
struct grafo *g_pred;
struct grafo *g_suc;
int ini_pesq;
struct no ** list_prof;
{
 /* declaracao de variaveis locais */

 struct no *conj_no=(struct no *)NULL; /* contera' os predecessores de x */
 int z, ja_encontrei;     /* z contera' um predecessor de x e ja_encontrei contera' a resposta `a pesquisa */
 int velha_marca;         /* armazena a condicao das marcas ate' aqui no caminho sendo pesquisado */

 /* inicializacao */

 atr_conj(&conj_no, &(g_pred[x].list_suc));  /* coloca os predecessores de x em conj_no */
 ja_encontrei=FALSE;
 velha_marca=sem_marca;

 /* faz a busca */

 while(conj_no!=(struct no *)NULL && ja_encontrei!=TRUE)
    {
     z=ret_elem(&conj_no);
     if((no_ocorren(*list_prof,z)<=2) && (z!=ini_pesq || x!=y))
	 {
	  if(tem_marca(z,x,g_suc)==FALSE) sem_marca=TRUE;
	  if(z==y)    /* e' igual ao no' de chegada? */
	      {
	       if(sem_marca==TRUE)    /* esta' sem marca de herdeiro ? */
		      ja_encontrei=TRUE;
	      }
	  else
	      {
	       rins_elem(list_prof,z);  /* continua a pesquisa */
	       ja_encontrei=find_path(z,y,sem_marca,g_pred,g_suc,ini_pesq,list_prof);
	       if(ja_encontrei==FALSE)
		       sem_marca=velha_marca;
	       rdel_elem(list_prof,z);
	      }
	 }
     }
     lib_conj(conj_no);
     return(ja_encontrei);
}

/********************************************************************/
/* int in(int,struct grafo *,struct grafo *,struct no **,int)       */
/* Autor: Marcos L. Chaim                                           */
/* Versao: 1.0                                                      */
/* Data: 27/06/89                                                   */
/*                                                                  */
/* FUNCAO: determinar os predecessores de um dado no' no grafo redu-*/
/*         zido pelas regras de reducao de Chusho e o numero de ar- */
/*         entrando neste no'.                                      */
/*                                                                  */
/* ENTRADA:no' do qual quer se calcular os predecessores, grafo re- */
/*         presentado por predecessores, grafo representado por su- */
/*         cessores, apontador para o apontador da lista que conte- */
/*	   ra' os predecessores e numero de arcos entrando no no'.  */
/*                                                                  */
/* SAIDA:  numero de arcos entrando no no'.                         */
/*                                                                  */
/* FUNCOES CHAMADAS: in(), rins_elem().                             */
/*                                                                  */
/********************************************************************/

int in(x,y,g_suc,g_pred,list_pred,i)
int x;
int y;
struct grafo *g_suc;
struct grafo *g_pred;
struct no **list_pred;
int i;
{

 /* declaracao de variaveis locais */

 struct no *noaux;

 /* search backward */

 for(noaux=g_pred[y].list_suc;noaux!=(struct no *)NULL;noaux=noaux->proximo)
	{
	 if(noaux->marca=='p' || noaux->marca == 't')
		    {
		     ++i;
		     rins_elem(list_pred,noaux->num);
		    }
	 else
	   {
	    if(noaux->num!=x)
		     i=in(y,noaux->num,g_suc,g_pred,list_pred,i);
	   }
	}

 /* search forward */

 for(noaux=g_suc[y].list_suc;noaux!=(struct no *)NULL;noaux=noaux->proximo)
	if(noaux->marca=='h' && x!=noaux->num)
		     i=in(y,noaux->num,g_suc,g_pred,list_pred,i);

 return(i);
}

/********************************************************************/
/* void out(int,struct grafo *,struct grafo *,struct no **)         */
/* Autor: Marcos L. Chaim                                           */
/* Versao: 1.0                                                      */
/* Data: 29/06/89                                                   */
/*                                                                  */
/* FUNCAO: determinar os sucessores de um dado no' no grafo redu-   */
/*         zido pelas regras de reducao de Chusho e o numero de ar- */
/*         cos saindo do no'.                                       */
/*                                                                  */
/* ENTRADA:no' do qual quer se calcular os sucessores, grafo re-    */
/*         presentado por predecessores, grafo representado por su- */
/*         cessores, apontador para o apontador da lista que conte- */
/*         ra' os sucessores e o numero de arcos saindo do no'.     */
/*                                                                  */
/* SAIDA:  numero de arcos saindo do no'.                           */
/*                                                                  */
/* FUNCOES CHAMADAS: out(), rins_elem().                            */
/*                                                                  */
/********************************************************************/

int out(x,y,g_suc,g_pred,list_sc,o)
int x;
int y;
struct grafo *g_suc;
struct grafo *g_pred;
struct no **list_sc;
int o;
{

 /* declaracao de variaveis locais */

 struct no *noaux;

 /* search forward */

 for(noaux=g_suc[y].list_suc;noaux!=(struct no *)NULL;noaux=noaux->proximo)
	{
	 if(noaux->marca=='p' || noaux->marca == 't')
		     {
		      ++o;
		      rins_elem(list_sc,noaux->num);
		     }
	 else
	    {
	     if(noaux->num!=x)
		      o=out(y,noaux->num,g_suc,g_pred,list_sc,o);
	    }
	}

 /* search backward */

 for(noaux=g_pred[y].list_suc;noaux!=(struct no *)NULL;noaux=noaux->proximo)
	if(noaux->marca=='h' && x!=noaux->num)
		      o=out(y,noaux->num,g_suc,g_pred,list_sc,o);

 return(o);
}


/*******************************************************************/
/* void cal_prim(struct grafo *,struct grafo *,int)                */
/* Autor: Marcos L. Chaim                                          */
/* Versao: 1.0                                                     */
/* Data: 28/06/89                                                  */
/*                                                                 */
/* FUNCAO: calcula os arcos primitivos de um grafo de fluxo de con-*/
/*         controle segundo o algoritmo modificado de Chusho para  */
/*         dos herdeiros de fluxo de dados.                        */
/*                                                                 */
/* ENTRADA:apontador para o grafo de fluxo de controle representa- */
/*         do por listas de sucessores, apontador para o grafo re- */
/*         presentado por listas de predecessores e numero de nos  */
/*         do grafo de controle.                                   */
/*                                                                 */
/* SAIDA:  nenhuma.                                                */
/*                                                                 */
/* FUNCOES CHAMADAS: elemina_arco(), find_path(), lib_conj(),      */
/*                   marca_arco(), out_no_mark(), satisR1(),       */
/*		     satisR2(), satisR4().                         */
/*                                                                 */
/*******************************************************************/

void cal_prim(g_suc,g_pred,n)
struct grafo *g_suc;
struct grafo *g_pred;
int n;
{

 /* declaracao de variaveis locais */

 struct no *noaux=(struct no *)NULL, *list_prof=(struct no *)NULL;
 struct conj_bit *dom;
 int i;

 /* calculo dos dominadores e iniciacao do grafo */

 dom=cal_dom(g_pred,g_suc,n);     /* calculo dos dominadores */

#ifdef DEBUG

 for(i=1; i <= n; ++i)
   {
    int j;
    printf("\n\n");
    printf("dominadores de %d: ",i);
    for(j=1; j <= n; ++j)
      if(test_bit(j,&dom[i].set))
         printf("%d ",j);
    printf("\n\n");
   }

#endif

 for(i=1;i<=n;++i)
   {
    for(noaux=g_suc[i].list_suc;noaux!=(struct no *)NULL;noaux=noaux->proximo)
	     noaux->marca='p';
    for(noaux=g_pred[i].list_suc;noaux!=(struct no *)NULL;noaux=noaux->proximo)
	     noaux->marca='p';
   }

 /* aplicacao das regras de reducao */

 /* regra R1 */

 for(i=1;i<=n;++i)
    for(noaux=g_suc[i].list_suc;noaux!=(struct no *)NULL;noaux=noaux=noaux->proximo)
	   if(satisR1(i,noaux,g_pred,g_suc)==TRUE)
		       elimina_arco(i,noaux,g_pred);    /* aplica R1 */

 /* regra R2 */

 for(i=1;i<=n;++i)
    for(noaux=g_suc[i].list_suc;noaux!=(struct no *)NULL;noaux=noaux->proximo)
       if(noaux->marca!='h' && satisR2(i,noaux,g_pred,g_suc)==TRUE)
		       elimina_arco(i,noaux,g_pred);    /* aplica R2 */

 /* regra R4 */

 for(i=1;i<=n;++i)
    for(noaux=g_suc[i].list_suc;noaux!=(struct no *)NULL;noaux=noaux->proximo)
	if(noaux->marca!='h' && satisR4(i,noaux,g_pred,g_suc,dom))
	   {
	    if(out_no_mark(0,noaux->num,g_suc,g_pred,FALSE) || find_path(noaux->num,noaux->num,FALSE,g_pred,g_suc,i,&list_prof))
	      {
	       marca_arco(i,noaux,g_pred);
	       lib_conj(list_prof);
	       list_prof=(struct no *)NULL;
	      }
	   }

 for(i=1;i<=n;++i)
     for(noaux=g_suc[i].list_suc;noaux!=(struct no *)NULL;noaux=noaux->proximo)
       if(noaux->marca=='t')  elimina_arco(i,noaux,g_pred);  /* elimina arco pela R4 */

 /* libera o espaco ocupado pelos conjuntos de dominadores */

 for(i=0;i<=n;++i)
   {
   b_vector_destroy(&dom[i].set); 
   }  
 free(dom);
 
 return;
 }















