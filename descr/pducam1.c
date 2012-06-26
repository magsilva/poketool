#include "headerli.h"
#include "hparserg.h"
#include "hrotsem.h"
#include "header.h"
#include "hpducam.h"

/* Declaracao de Variaveis Globais */

extern NODEINFO info_no;
extern struct grafo * graph;
extern int no_nos;

/********************************************************************/
/* void det_pot_du_cam(struct grafo *)                              */
/* Autor: Marcos L. Chaim                                           */
/* Data: 12/12/89                                                   */
/* Versao: 1.0                                                      */
/*                                                                  */
/* Funcao: esta funcao determina os potenciais du-caminhos de um    */
/*         grafo def escrevendo esses caminhos no arquivo "potdu-   */
/*         cam.tes".                                                */
/*                                                                  */
/* Entradas: apontador para o grafo de fluxo de controle (GFC).     */
/*                                                                  */
/* Saida: nenhuma.                                                  */
/*                                                                  */
/* Variaveis Globais Utilizadas: info_no.                           */
/*                                                                  */
/********************************************************************/

void det_pot_du_cam(g_suc)
struct grafo * g_suc;
{

 /* Declaracao de Variaveis Locais */

 FILE * potducam;  /* arquivo que contera' os pot-du-caminhos */

 struct no * conodef; /* apontador para o conjunto de nos com definicao
			        de variaveis */

 int x; /* variavel do tipo inteiro que contem os numero dos nos retirados de
	   conodef */

 BITVECTOR deff; /* variavel que contem o conjunto de variaveis definidas
		    em um no' i mas nao redefinidas no particular caminho
		    sendo percorrido pelo algoritmo */

 BITVECTOR aux1;

 ELEMSUC ref;    /* variavel que armazena as informacoes do no' i que da'
		    origem aos pot-du-caminhos */

 ELEMSUC elemento; /* elemento sucessor de ref sendo percorrido */
 ELEMSUC elemaux;  /* variavel auxiliar */

 INFODFNO nosucg; /* variavel que armazena as informacoes sobre o sucessor
		     de elemento */


 STACK pil_cam;   /* pilha que armazena o caminho percorrido no grafo G a
		     partir do no' i */

 STACKELEMSUC conjsuc; /* pilha de auxilio ao percorrimento do grafo def
			  onde os seus elementos sao do tipo ELEMSUC */

 potducam = fopen("potducam.tes","w");

 if(potducam == (FILE *) NULL)
    error("PROBLEMAS! Nao consegui abrir o arquivo potducam.tes\n");

 mk_nl_int(&pil_cam);     /* inicia pilha pil_cam */
 mk_nl_elemsuc(&conjsuc); /* inicia pilha conjsuc */

 conodef = (struct no *) NULL;
 conodef = det_conodef(conodef); /* determina os conjuntos de nos do
				    programa que possui definicao de
				    variaveis */


 while(conodef != (struct no *) NULL)
   {
    mk_nl_int(&pil_cam);    /* inicia pilha do caminho */
    x = ret_elem(&conodef); /* retira um no' de conodef */

    /* inicia ref com a informacoes do no' x */

    fprintf(potducam,"\n\n@%d \n",x);

    ref.infosuc.num_no_G = x;
    ref.infosuc.deff = uniao_bitvector(&info_no[x].defg_i,&info_no[x].def_ref);
    ref.apont = lista_sucessores(x,g_suc);

    push_int(x,&pil_cam);  /* empilha x na pilha do caminho */

    push_elemsuc(&ref,&conjsuc);

    while(!empty_elemsuc(&conjsuc))
      {
       elemento = pop_elemsuc(&conjsuc);  /* desempilha um elemento de conjsuc */

       /* atualiza a pilha do caminho */
       atualiza_pil_cam(elemento.infosuc.num_no_G,&pil_cam);

       deff = elemento.infosuc.deff; /* deff recebe o conjunto deff de elemento */

       if(elemento.apont != (LIST *) NULL)
          nosucg = head(&elemento.apont); /* nosucg recebe as informacoes do no' que e' sucessor de elemento */
       else
          {
           nosucg.num_no_G = 0;
           reset_all(&(nosucg.deff));
           printf("tentei fazer a cabeca de um elemento sem sucessores\n");
          }
       if(elemento.apont != (LIST *) NULL)
          push_elemsuc(&elemento,&conjsuc); /* elemento ainda possui sucessores
                                              entao o mantem na pil_cam */

  /* determinacao do conjunto deff */

       aux1 = neg_bitvector(&info_no[nosucg.num_no_G].undef);

       aux1 = inter_bitvector(&info_no[nosucg.num_no_G].defg_i, &aux1);

       aux1 = inter_bitvector(&deff,&aux1);

       deff = sub_bitvector(&deff, &aux1);

       nosucg.deff = deff;

       if( !vazio_bit(&deff) ) /* conjunto deff e' vazio ? */
         {
          if( !pertence_pil_cam(nosucg.num_no_G,&pil_cam))
             { /* nosucg nao pertence a pilha do caminho */

              push_int(nosucg.num_no_G,&pil_cam); /* insere na pilha do caminho */
              elemaux.infosuc = nosucg;
              elemaux.apont = lista_sucessores(nosucg.num_no_G,g_suc);

              if(elemaux.apont != (LIST *) NULL)
                 push_elemsuc(&elemaux, &conjsuc); /* insere nosucg em conjsuc */
              else
                 imprime_int_stack(&pil_cam, potducam);
             }
          else
            {
             push_int(nosucg.num_no_G,&pil_cam); /* insere na pilha do caminho */
             imprime_int_stack(&pil_cam, potducam);
             pop_int(&pil_cam);  /* retira no' repetido de pil_cam e (nao
                                    implementado) insere-o no conjunto Nr */
            }
         }
       else
          {
           push_int(nosucg.num_no_G,&pil_cam); /* insere na pilha do caminho */
           imprime_int_stack(&pil_cam, potducam);
           pop_int(&pil_cam);  /* retira no' repetido de pil_cam e (nao
                                  implementado) insere-o no conjunto N0 */
          }
      }
 }
}

/********************************************************************/
/* struct no * det_conodef(struct no *)                             */
/* Autor: Marcos L. Chaim                                           */
/* Data: 13/12/89                                                   */
/* Versao: 1.0                                                      */
/*                                                                  */
/* Funcao: esta funcao determina o conjunto de nos do GFC que pos-  */
/*         suem definicao de variaveis.                             */
/*                                                                  */
/* Entradas: apontador para a lista que contera' os que possuem de- */
/*           finicoes de variaveis.                                 */
/*                                                                  */
/* Saida: apontador para a lista conodef.                           */
/*                                                                  */
/* Variaveis Globais Utilizadas: info_no e no_nos.                  */
/*                                                                  */
/********************************************************************/

struct no * det_conodef(conodef)
struct no * conodef;
{

 /* Declaracao de Variaveis Locais */

 int i;
 BITVECTOR bitaux;

 conodef = (struct no *) NULL;

 for(i=1; i >= 1 && i <= no_nos;++i)
    {
     bitaux = uniao_bitvector(&info_no[i].defg_i,&info_no[i].def_ref);
     if(!vazio_bit(&bitaux))
        ins_elem(&conodef,i);
    }
 return(conodef);
}

/********************************************************************/
/* void atualiza_pil_cam(int, STACK *)                              */
/* Autor: Marcos L. Chaim                                           */
/* Data: 13/12/89                                                   */
/* Versao: 1.0                                                      */
/*                                                                  */
/* Funcao: esta funcao atualiza o apontador da pilha do caminho     */
/*         pil_cam de forma que o topo da pilha seja sempre igual a */
/*         a variavel elemento.infosuc.num_no_G.                    */
/*                                                                  */
/* Entradas: inteiro para o qual sera' ajustado o topo de pil_cam e */
/*           apontador para pil_cam.                                */
/*                                                                  */
/* Saida: nenhuma.                                                  */
/*                                                                  */
/********************************************************************/

void atualiza_pil_cam(elemento, pil_cam)
int elemento;
STACK * pil_cam;
{

 /* Declaracao de Variaveis Locais */

 int aux;

 while(!empty_int(pil_cam))
   {
    aux = pop_int(pil_cam);
    if(aux == elemento)
      {
       push_int(aux,pil_cam);
       return;
      }
   }
 error("PROBLEMAS! Pilha do caminho vazia e nao encontrei elemento\n");
}

/********************************************************************/
/* int pertence_pil_cam(int, STACK *)                               */
/* Autor: Marcos L. Chaim                                           */
/* Data: 13/12/89                                                   */
/* Versao: 1.0                                                      */
/*                                                                  */
/* Funcao: esta funcao retorna verdadeiro caso o no' passado como   */
/*         parametro esteja presenta na pilha do caminho e falso    */
/*         caso contrario.                                          */
/*                                                                  */
/* Entradas: numero do no' e apontador para pil_cam.                */
/*                                                                  */
/* Saida: inteiro indicando verdadeiro ou falso.                    */
/*                                                                  */
/********************************************************************/

int pertence_pil_cam(elemento, pil_cam)
int elemento;
STACK * pil_cam;
{

 /* Declaracao de Variaveis Locais */

 STACK stack_aux;

 int aux, retorno = FALSE;

 mk_nl_int(&stack_aux);  /* inicia pilha auxiliar */

 while(!empty_int(pil_cam))
   {
    aux = pop_int(pil_cam);
    push_int(aux,&stack_aux);
    if(aux == elemento)
       retorno = TRUE;
   }

 while(!empty_int(&stack_aux))
    push_int(pop_int(&stack_aux),pil_cam);

 return(retorno);
}


/********************************************************************/
/* void imprime_int_stack(STACK *, FILE *)                          */
/* Autor: Marcos L. Chaim                                           */
/* Data: 12/12/89                                                   */
/* Versao: 1.0                                                      */
/*                                                                  */
/* Funcao: esta funcao imprime o conteudo de uma pilha de inteiros  */
/*         num arquivo cujo descritor e' passado como parametro.    */
/*                                                                  */
/* Entradas: apontador para a pilha de inteiros e apontador para o  */
/*           arquivo.                                               */
/*                                                                  */
/* Saida: nenhuma.                                                  */
/*                                                                  */
/********************************************************************/

void imprime_int_stack(stack, archive)
STACK * stack;
FILE * archive;
{

 /* Declaracao de Variaveis Locais */

 STACK stack_aux;

 int aux;

 mk_nl_int(&stack_aux);  /* inicia pilha auxiliar */

 while(!empty_int(stack))
    push_int(pop_int(stack),&stack_aux);

while(!empty_int(&stack_aux))
   {
    aux = pop_int(&stack_aux);
    push_int(aux,stack);
    fprintf(archive,"%d ",aux);
   }
 fprintf(archive,"\n");
}

/********************************************************************/
/* LIST * lista_sucessores(int, struct grafo *)                     */
/* Autor: Marcos L. Chaim                                           */
/* Data: 13/12/89                                                   */
/* Versao: 1.0                                                      */
/*                                                                  */
/* Funcao: cria a partir do GFC uma lista de sucessores do no' pas- */
/*         sado como parametro e retorna um apontador para a lista. */
/*         Cada elemento e' do tipo LIST_SUC.                       */
/*                                                                  */
/* Entradas: numero do no' e apontador para o GFC.                  */
/*                                                                  */
/* Saida: apontador para a lista de elementos do tipo ELEMSUC.      */
/*                                                                  */
/********************************************************************/

LIST * lista_sucessores(elemento,g_suc)
int elemento;
struct grafo * g_suc;
{

 /* Declaracao de Variaveis Locais */

 struct no * gaux;
 LIST * suc = (LIST *)NULL, *ptr_lista_sucessores = (LIST *)NULL;

 /*
  int i;
  for(i=1; i <= no_nos; ++i)
   {
    printf("Sucessores do no' %d\n",i);
    for(gaux=graph[i].list_suc; gaux != NULL; gaux = gaux->proximo)
      printf("%d ",gaux->num);
    printf("\n");
   }

   */
 ptr_lista_sucessores = suc = (LIST *) malloc(sizeof(LIST));
 ptr_lista_sucessores->next = (LIST *) NULL;

 for(gaux = g_suc[elemento].list_suc;gaux != (struct no *) NULL;
     gaux = gaux->proximo)
     {
      suc->next = (LIST *)malloc(sizeof(LIST));
      suc = suc->next;
      (suc->sucessor).num_no_G = gaux->num;
      (suc->sucessor).deff = info_no[elemento].defg_i;
      suc->next = (LIST *) NULL ;
     }

 suc =  ptr_lista_sucessores;
 ptr_lista_sucessores = ptr_lista_sucessores->next;  /* ajusta apontador da
                                                        lista de sucessores */
 free(suc);          /* libera a primeira posicao alocada */

 return(ptr_lista_sucessores);
}

/********************************************************************/
/* INFODFNO head(LIST **)                                           */
/* Autor: Marcos L. Chaim                                           */
/* Data: 13/12/89                                                   */
/* Versao: 1.0                                                      */
/*                                                                  */
/* Funcao: retira o primeiro elemento de uma lista de elementos do  */
/*         INFODFNO e retorna esse elemento.                        */
/*                                                                  */
/* Entradas: endereco do apontador para a lista.                    */
/*                                                                  */
/* Saida: primeiro elemento da lista de elementos do tipo INFODFNO. */
/*                                                                  */
/********************************************************************/

INFODFNO head(cabeca)
LIST ** cabeca;
{

 /* Declaracao de Variaveis Locais */

 LIST * cabaux;
 INFODFNO aux;

 cabaux = *cabeca;

 if(cabaux == (LIST *) NULL)
   error("PROBLEMAS! Tentei retirar a \"cabeca\" de uma lista sem \"cabeca\"\n");

 *cabeca = cabaux->next;
 aux = cabaux->sucessor;
 free(cabaux);

 return(aux);
