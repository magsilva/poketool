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
/* struct conj_bit * cal_dom(struct grafo *, int)                  */
/* Autor: Marcos L. Chaim                                          */
/* Versao: 2.0                                                     */
/* Data: 19/06/89                                                  */
/*                                                                 */
/* FUNCAO: esta funcao calcula o conjunto de dominadores de um da- */
/*         do grafo de fluxo de controle aplicando o algoritmo de  */
/*         iteracao rPOSTORDER.                                    */
/*                                                                 */
/* ENTRADA:apontador para o grafo de fluxo de controle e o numero  */
/*         de nos desse grafo.                                     */
/*                                                                 */
/* SAIDA:  apontador para os conjuntos de dominadores dos nos do   */
/*         grafo.                                                  */
/*                                                                 */
/* FUNCOES CHAMADAS: atr_conj(), cal_rpos(), det_pred(),           */
/*                   ins_elem(), ret_elem().                       */
/*                                                                 */
/* HISTORICO DE ALTERACAO:                                         */
/*  Autor da Modificacao: Marcos L. Chaim                          */
/*                                                                 */
/*  Motivo: aumentar o desempelho dessa funcao.                    */
/*                                                                 */
/*  Alteracao:                                                     */
/*     versao 1.0 alterada em 26/12/89 - estrutura de dados dos    */
/*     conjuntos de dominadores foi alterada de um lista de intei- */
/*     para um conjunto de "bits" (vetor de "bits"). As funcoes de */
/*     de manipulacao desses conjuntos de listas (uniao, intersec- */
/*     cao, etc) foram alteradas para funcoes de manipulacao de    */
/*     conjuntos de "bits".                                        */
/*                                                                 */
/*******************************************************************/

#include "hrotsem.h"
#include "header.h"
#include "hparserg.h"

struct conj_bit * cal_dom(g_pred,g_suc,n)
struct grafo *g_pred;
struct grafo *g_suc;
int n;
{

 /* Declaracao de Variaveis Locais */

 b_vector temp;
 struct conj_bit *dom;
 int change, i, j, *rpostorder;

 /* Inicia vetor de bits temp  */

 b_vector_cons(&temp,n/BYTE+1,0);

 /* Ordenacao rPOSTORDER */

 rpostorder=(int *)malloc((n+1)*sizeof(int));
 
 if(rpostorder==NULL)
      error("* * Erro Fatal: Nao consegui alocar espaco para ordenar o \
      grafo em rPOSTORDER * *\n");

 for(i=0; i<=n;++i) rpostorder[i]= -10;    /* inicia vetor rpostorder */

 i=n;

 rpostorder=cal_rpos(1,g_suc,&i,rpostorder);  /* calcula vetor rpostorder */

 /* inicia valores dos conjuntos de dominadores dom(i) */

 dom=(struct conj_bit *)malloc((n+1)*sizeof(struct conj_bit));

 if(dom==NULL)
      error("* * Erro Fatal: Nao consegui alocar espaco para os conjuntos de \
      dominadores dos nos do grafo * *\n");

 for(i=0;i<=n;++i)
    {
    b_vector_cons(&dom[i].set,n/BYTE+1,0);
    reset_all(&dom[i].set);
    }

 /* iniciando os conjuntos dom[i].set e i!=1   */
 /* com o valor maximo do semi-reticulado      */

  for(i=2;i<=n;++i)
    for(j=1;j<=n;++j)
       set_bit(j,&(dom[i].set));


 /* Calculo dos Dominadores */

 /* dom[1].set ja' foi iniciado com o valor minimo  */
 /* do semi_reticulado (NULL)                         */

#ifdef DEBUG

 printf("\n");
 system("cls");
 for(i=1; i<=n; ++i)
    printf("rpostorder[%d] = %d\n",i,rpostorder[i]);

#endif

 /* inicia i com a ordenacao rPOSTORDER */

 for(j = 0, i = rpostorder[j]; j<=n; i = rpostorder[++j])
       {
        if(i==1)
          continue;
        if(i==-10 && (j == 0 || j == 1))
           continue;
        if(i==-10 && j > 1)
          {
           msg_print("* * No' nao e' alcancavel a partir da entrada * *\n");
           }
        else
           {
           /* temp = calcula_resultados(i,dom,g_pred,n); */
            calcula_resultados(&temp,i,dom,g_pred,n);
	   /* dom[i].set = temp; */
              
           b_vector_cpy(&dom[i].set,&temp);
           /* b_vector_destroy(&temp); */
           }
       }

 /* iteracao rPOSTORDER */

 change=TRUE;
 while(change==TRUE)
     {
      change=FALSE;
      j=0;
      for(i=rpostorder[j];j<=n;i=rpostorder[++j])
         {
           if(i==-10 || i == 1)
              continue;
           /* temp = calcula_resultados(i,dom,g_pred,n); */
           calcula_resultados(&temp,i,dom,g_pred,n);
#ifdef DEBUG
if(i==72)
  {
    printf("dominadores de %d: ",i);
    for(j=1; j <= n; ++j)
      if(test_bit(j,&dom[i].set))
         printf("%d ",j);
    printf("\n\n");
    
    printf("temp de %d: ",i);
    for(j=1; j <= n; ++j)
      if(test_bit(j,&temp))
         printf("%d ",j);
    printf("\n\n");

  }
#endif
	   if(!is_equal_b_vector(&temp,&dom[i].set))
	     {
	      change=TRUE;
	      /* dom[i].set = temp; */
              
              b_vector_cpy(&dom[i].set,&temp);
              
	     }
      	 }
     }
 b_vector_destroy(&temp);
 free(rpostorder);

 return(dom);
}

/*******************************************************************/
/* b_vector  calcula_resultados(int, struct conj_bit *,            */
/* struct grafo *, int nodes)                                      */
/* Autor: Marcos L. Chaim                                          */
/* Versao: 1.0                                                     */
/* Data: 26/12/89                                                  */
/*                                                                 */
/* FUNCAO: esta funcao aplica a funcao fi no conjunto de dominado- */
/*         res dos antecessores do no' passado como parametro e    */
/*         e aplica a operacao "meet" nos resultados da aplicacao  */
/*         de fi nos conjunto de dominadores dos antecessores.     */
/*                                                                 */
/* ENTRADA:inteiro com o numero do no', apontador para os conjuntos*/
/*         de dominadores,  apontador para o grafo de fluxo de con-*/
/*         trole e numero de nos do grafo.                         */
/*                                                                 */
/* SAIDA:  conjunto resultante da aplicacao de fi e da operacao    */
/*         "meet".                                                 */
/*                                                                 */
/* FUNCOES CHAMADAS: atr_conj(), inter_b_vector(), ret_elem().     */
/*                                                                 */
/*******************************************************************/

b_vector calcula_resultados(res,i,dom,g_pred,nodes)
int i;
b_vector * res;
struct conj_bit *dom;
struct grafo * g_pred;
int nodes;
{

 struct no * aux = (struct no *) NULL;
 int k, flag=TRUE;
 int j;
 
 b_vector temp0, anaux;
 
 /* Inicia vetor de bits auxiliares */

 b_vector_cons(&temp0,nodes/BYTE+1,0);
  
 b_vector_cons(&anaux,nodes/BYTE+1,0);
  
 atr_conj(&aux,&(g_pred[i].list_suc));

#ifdef DEBUG

    printf("dominadores de %d: ",i);
    for(j=1; j <= nodes; ++j)
      if(test_bit(j,&dom[i].set))
         printf("%d ",j);
    printf("\n\n");

#endif

 while(aux!=NULL)
	  {
           /* retirando um elemento do conjunto de predecessores */
	   k=ret_elem(&aux);
           /* temp0 vai receber o resultado da aplicacao da funcao fi */
	   /* temp0 = dom[k].set; */
           
           b_vector_cpy(&temp0, &dom[k].set);
           
           /* aplicando a funcao fi */
           set_bit(k,&temp0);  

           if (flag)
	      {
              /* primeira iteracao do loop inicializa anaux com o valor de temp0 */
              /* anaux = temp0; */
              b_vector_cpy(&anaux,&temp0);
	      flag=FALSE;
	      }
           /* anaux recebe resultado da interseccao */ 
	   (void) inter_b_vector(&anaux,&temp0);     /* operacao meet */ 
	  }

#ifdef DEBUG

    printf("dominadores de %d: ",i);
    for(j=1; j <= nodes; ++j)
      if(test_bit(j,&anaux))
         printf("%d ",j);
    printf("\n\n");

#endif
    
 b_vector_cpy(res,&anaux);
 b_vector_destroy(&temp0);
 b_vector_destroy(&anaux);
 return(anaux);
}

