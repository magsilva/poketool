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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef TURBO_C

#include <alloc.h>

#else  /* SunOS, BSD4.2 e RISC 6000 */

#include <malloc.h>

#endif

#include "util.h"


#include "headerli.h"
#include "hparserg.h"
#include "hrotsem.h"
#include "header.h"
#include "hpducam.h"

/* Declaracao de Variaveis Globais */

extern NODEINFO info_no;
extern struct grafo * graph;
extern int no_nos;
extern int no_vars;
extern int n_des_pdu;
extern int n_des_pu;

/********************************************************************/
/* void det_pot_du_cam(struct grafo *, char *)                      */
/* Autor: Marcos L. Chaim                                           */
/* Data: 12/12/89                                                   */
/* Versao: 1.0                                                      */
/*                                                                  */
/* Funcao: esta funcao determina os grafo(i) de um grafo def.       */
/*                                                                  */
/* Entradas: apontador para o grafo de fluxo de controle (GFC),     */
/*           diretorio, numero de grafo(i).                         */
/* Saida: apontador para conjunto de grafo(i), no_grfi.             */
/*                                                                  */
/* Variaveis Globais Utilizadas: info_no.                           */
/*                                                                  */
/********************************************************************/

INFODESCRITORES * det_pot_du_cam(g_suc, dir, no_grfi)
struct grafo * g_suc;
char * dir;
int * no_grfi;
{

 /* Declaracao de Variaveis Locais */

 struct no * conodef = (struct no *) NULL; /* apontador para o conjunto
                                              de nos com definicao
			                      de variaveis */

 
 int x; /* variavel do tipo inteiro que contem os numero dos nos retirados de
	   conodef */

 b_vector deff; /* variavel que contem o conjunto de variaveis definidas
		    em um no' i mas nao redefinidas no particular caminho
		    sendo percorrido pelo algoritmo */

 b_vector N; /* conjunto que contem os nos do grafo G */

 ELEMSUC ref;    /* variavel que armazena as informacoes do no' i que da'
		    origem aos pot-du-caminhos */

 ELEMSUC elemento; /* elemento sucessor de ref sendo percorrido */

 INFODFNO nosucg; /* variavel que armazena as informacoes sobre o sucessor
		     de elemento */


 STACK pil_cam;   /* pilha que armazena o caminho percorrido no grafo G a
		     partir do no' i */

 STACKELEMSUC conjsuc; /* pilha de auxilio ao percorrimento do grafo def
			  onde os seus elementos sao do tipo ELEMSUC */

 INFODESCRITORES * pdes; /* esta variavel aponta para um vetor que
                           contem as informacoes necessarias para
                           a construcao dos descritores dos
                           grafo(i) */

 STACKPAIR ja_ocorridos; /* pilha de pares de inteiros que contem os nos que
                            ja' ocorreram durante a criacao dos grafo(i) */

 b_vector aux1;     /* variavel auxiliar */
 PAIRINT aux2;       /* variavel auxiliar */
 ELEMSUC elemaux;    /* variavel auxiliar */
 int i,j,k;          /* variaveis auxiliares */


 mk_nl_int(&pil_cam);     /* inicia pilha pil_cam */
 mk_nl_elemsuc(&conjsuc); /* inicia pilha conjsuc */
 
 b_vector_cons(&deff,no_vars/BYTE+1,0); /* inicia vetor de bits deff */
 b_vector_cons(&N,no_nos/BYTE+1,0);    /* inicia vetor de bits N */

 /* inicia os conjunto N de nos do grafo G */
 reset_all(&N);

 /* inicia variaveis auxiliares */

 b_vector_cons(&ref.infosuc.deff,no_vars/BYTE+1,0);

 b_vector_cons(&nosucg.deff,no_vars/BYTE+1,0);
 b_vector_cons(&aux1,no_vars/BYTE+1,0);
 b_vector_cons(&elemaux.infosuc.deff,no_vars/BYTE+1,0);
 
 conodef = det_conodef(conodef); /* determina os conjuntos de nos do
				    programa que possui definicao de
				    variaveis */
 *no_grfi = j = cardinal(conodef); /* j contem o numero de elementos de conodef */
 
 /* inicia estrutura de dados para criacao de descritores */
 
 pdes = (INFODESCRITORES *) malloc((j+1)*sizeof(INFODESCRITORES));
 
 if(pdes != (INFODESCRITORES *) NULL)
   {
   /* Inicia as diversas estruturas */

   for(i=0; i <= j; ++i)
      {
      b_vector_cons(&pdes[i].Ni,no_nos/BYTE+1,0);   
      b_vector_cons(&pdes[i].Nt,no_nos/BYTE+1,0);
      }
   }
 else
   error("* * Erro Fatal: Nao consegui alocar espaco para as informacoes \
relativas aos grafo(i) * *\n");


 i = 0; /* contador de grafo(i) iniciado */

 while(conodef != (struct no *) NULL)
   {
    mk_nl_int(&pil_cam);    /* inicia pilha do caminho */
    mk_nl_pair(&ja_ocorridos); /* inicia pilha dos arcos ja' ocorridos */

    x = ret_elem(&conodef); /* retira um no' de conodef */

    /* inicia ref com a informacoes do no' x */

    ref.infosuc.num_no_G = x;
    b_vector_cpy(&ref.infosuc.deff, &info_no[x].defg_i);
    (void) union_b_vector(&ref.infosuc.deff,&info_no[x].def_ref);
    ref.apont = lista_sucessores(x,g_suc);

#ifdef DEBUG
    
    fprintf(stderr,"\n\n****** GRAFO(%d) ******\n\n",ref.infosuc.num_no_G);

#endif

    /* inicia a estrutura de dados INFODESCRITORES do no' i */

    ++i; /* incrementa contador de grafo(i) */

    /* inicia as estruturas de dados do grafo(i) */

    ini_info_descritores(&pdes[i]);

    grfi_inicializacao(&(pdes[i].grafo_i),&ref.infosuc); /* inicia o grafo(i) */

    push_int(x,&pil_cam);  /* empilha x na pilha do caminho */

    push_elemsuc(&ref,&conjsuc);

    while(!empty_elemsuc(&conjsuc))
      {
       elemento = pop_elemsuc(&conjsuc);  /* desempilha um elemento de conjsuc */
       set_bit(elemento.infosuc.num_no_G,&(pdes[i].Ni)); /* coloca o numero de elemento no conjunto de nos do grafo(i) */

       /* atualiza a pilha do caminho */
       atualiza_pil_cam(elemento.infosuc.num_no_G,&pil_cam);


       /* deff = elemento.infosuc.deff; deff recebe o conjunto deff de elemento */
       b_vector_cpy(&deff,&elemento.infosuc.deff);

       if(elemento.apont != (LIST *) NULL)
         {
         b_vector_destroy(&(nosucg.deff));
         nosucg = head(&elemento.apont); /* nosucg recebe as informacoes do no' que e' sucessor de elemento */
         }
       else
	 {
	 /* Esta situacao ocorre quando se tem uma definicao de variavel no
            ultimo no' de um grafo de controle. Neste caso, grafo(i) e'
            composto apenas por este no'.
         */
         nosucg.num_no_G = 0;
         reset_all(&(nosucg.deff));
         /* error("* * Erro Fatal: tentei obter a cabeca de um elemento sem sucessores * *\n"); */
         set_bit(elemento.infosuc.num_no_G,&(pdes[i].Nt)); /* coloca o numero de elemento no conjunto de nos terminais do grafo(i) */
         continue;
         }
       if(elemento.apont != (LIST *) NULL)
          push_elemsuc(&elemento,&conjsuc); /* elemento ainda possui sucessores
                                              entao o mantem na pil_cam */

       /* verifica se o arco e' primitivo e se repete */

       aux2.abs = elemento.infosuc.num_no_G;
       aux2.coor = nosucg.num_no_G;

       if(e_primitivo(aux2.abs, aux2.coor, g_suc))
           ins_arc(&(pdes[i].prim_arcs),&aux2,&(elemento.infosuc.deff));

       /* determinacao do conjunto deff */

       b_vector_cpy(&aux1,&(info_no[nosucg.num_no_G].undef));

       /* (void) neg_b_vector(&aux1); */

       (void) inter_b_vector(&aux1,&info_no[nosucg.num_no_G].defg_i);

       (void) inter_b_vector(&aux1,&deff);

       (void) sub_b_vector(&deff, &aux1);

       /* nosucg.deff = deff; */
        b_vector_cpy(&nosucg.deff,&deff);

       if( !empty_bit(&deff) ) /* conjunto deff e' vazio ? */
         {
           if(pertence_pil_cam(nosucg.num_no_G,&pil_cam))
             /*if(nosucg.num_no_G == ref.infosuc.num_no_G) Checa se e' igual ao no' i */
              { 
            /* nosucg pertence a pilha do caminho */

            /* coloca o numero de nosucg no conjunto de nos terminais */
             set_bit(nosucg.num_no_G,&(pdes[i].Nt));

#ifdef DEBUG

             printf("rep ");

#endif

             insere_grafo_i(&elemento.infosuc,&nosucg,pdes[i].grafo_i,g_suc,E_REPETIDO);

             /*no' do caminho e' igual ao no' i do grafo(i) ? */

 /* maldonado xeretou no if abaixo */

           /*  if(ref.infosuc.num_no_G == nosucg.num_no_G) */
                 /* sim, faca-o pfi */
               faca_arco_pfi(&elemento.infosuc,&nosucg,pdes[i].grafo_i);
               ins_arc(&(pdes[i].prim_arcs),&aux2,&(elemento.infosuc.deff));
          
            }
          else
          {  /* nosucg nao pertence a pilha do caminho */

              if(existe_no_igual(&nosucg,pdes[i].grafo_i))
                {
#ifdef DEBUG

                 printf("eq ");

#endif

		 liga_grafo_i(&elemento.infosuc, &nosucg,pdes[i].grafo_i, g_suc);
		  /* Provalvelmente, desnecessario. */
		 if(!is_equal_b_vector(&(elemento.infosuc.deff),&(nosucg.deff) ))
			faca_arco_pfi(&elemento.infosuc,&nosucg,pdes[i].grafo_i);
                }
              else
                {
		 insere_grafo_i(&elemento.infosuc, &nosucg,pdes[i].grafo_i, g_suc,NAO_REPETIDO);

		 /* Verifica se alguma variavel foi "morta" por nosucg; em caso
		    afirmativo, torna-se este arco primitivo pfi.
		 */
		 if(!is_equal_b_vector(&(elemento.infosuc.deff),&(nosucg.deff)))
			{
			faca_arco_pfi(&elemento.infosuc,&nosucg,pdes[i].grafo_i);
			ins_arc(&(pdes[i].prim_arcs),&aux2,&(elemento.infosuc.deff));
			}


                 push_int(nosucg.num_no_G,&pil_cam);  /* empilha nosucg.num_no_G na pilha do caminho */

                 /* Copia campos da estrutura INFODFNO */
                 /* elemaux.infosuc = nosucg; */

                 b_vector_cpy(&elemaux.infosuc.deff,&nosucg.deff);
                 elemaux.infosuc.num_no_G=nosucg.num_no_G;
                 
                 elemaux.apont = lista_sucessores(nosucg.num_no_G,g_suc);

                 /* verifica se nosucg.num_no_G possui sucessores */
                 if(elemaux.apont != (LIST *) NULL)
                     push_elemsuc(&elemaux, &conjsuc); /* insere nosucg em conjsuc */
                 else
                    {
                     set_bit(nosucg.num_no_G,&(pdes[i].Ni)); /* no' terminal pertence a Ni */
                     set_bit(nosucg.num_no_G,&(pdes[i].Nt)); /* e' no' terminal */
                    }
                }
              }
        }
       else  /* deff = vazio */
          {

            /* if(pertence_pil_cam(nosucg.num_no_G,&pil_cam)) */
            if(nosucg.num_no_G == ref.infosuc.num_no_G) /* Checa se e' igual ao no' i */
               {
               /* coloca o numero de nosucg no conjunto de nos terminais */
               set_bit(nosucg.num_no_G,&(pdes[i].Nt)); 

#ifdef DEBUG

              printf("empty rep ");

#endif

              insere_grafo_i(&elemento.infosuc, &nosucg,pdes[i].grafo_i, g_suc,E_REPETIDO);
              faca_arco_pfi(&elemento.infosuc,&nosucg,pdes[i].grafo_i);
              ins_arc(&(pdes[i].prim_arcs),&aux2,&(elemento.infosuc.deff));
             }
             else 
              if(existe_no_igual(&nosucg,pdes[i].grafo_i))
                {
#ifdef DEBUG

                 printf("empty eq ");

#endif
                 set_bit(nosucg.num_no_G,&(pdes[i].Nt));
                 liga_grafo_i(&elemento.infosuc, &nosucg,pdes[i].grafo_i, g_suc);
                 faca_arco_pfi(&elemento.infosuc,&nosucg,pdes[i].grafo_i);
                 ins_arc(&(pdes[i].prim_arcs),&aux2,&(elemento.infosuc.deff));
                }
              else
                {
#ifdef DEBUG

                 printf("empty neq ");

#endif

                 set_bit(nosucg.num_no_G,&(pdes[i].Ni));
                 set_bit(nosucg.num_no_G,&(pdes[i].Nt));

                 insere_grafo_i(&elemento.infosuc, &nosucg,pdes[i].grafo_i, g_suc,NAO_REPETIDO);

                 /* faca o arco pfi */
                 faca_arco_pfi(&elemento.infosuc,&nosucg,pdes[i].grafo_i);
                 ins_arc(&(pdes[i].prim_arcs),&aux2,&(elemento.infosuc.deff));

                 /* insere na lista de arcos primitivos */

                 ins_arc(&(pdes[i].prim_arcs),&aux2,&(elemento.infosuc.deff));

                }
          }
      }
   }
 
return pdes;
}

/********************************************************************/
/* void gera_descr_pdu(struct grafo *, INFODESCRITORES *, char *,   */
/*   int)                                                           */
/* Autor: Marcos L. Chaim                                           */
/* Data: 04/10/99                                                   */
/* Versao: 1.0                                                      */
/*                                                                  */
/* Funcao: gera arquivos de descritores e com pot-du-caminhos.      */
/*                                                                  */
/* Entradas: apontador para grafo(i), diretorio e numero de         */
/*           grafo(i).                                              */
/*                                                                  */
/* Saida: nenhuma.                                                  */
/*                                                                  */
/* Variaveis Globais Utilizadas:                                    */
/*                                                                  */
/********************************************************************/

void gera_descr_pdu(g_suc,pdescr, dir,no_grfi)
struct grafo * g_suc;
INFODESCRITORES * pdescr;
char *dir;
int no_grfi;
{
 char filename [300];
 int i,k;
 FILE * arq_des_pdu, * arq_pdu_paths;
 b_vector N;

 b_vector_cons(&N,no_nos/BYTE+1,0);
	       
 msg_print("* * Gerando descritores criterio todos-pot-du-caminos... * *\n");

 /* geracao dos descritores e caminhos relativos ao criterio PDU */

 /* criando arquivo que contera' os descritores */

 strcpy(filename,dir);

#ifdef TURBO_C
 strcat(filename,"\\");
#else
 strcat(filename,"/");
#endif
 strcat(filename,"des_pdu.tes");
 
 arq_des_pdu = (FILE *) fopen(filename,"w");

 if(arq_des_pdu == (FILE *) NULL)
    error("* * Erro Fatal: Nao consegui abrir o arquivo des_pdu.tes * *\n");

/* criando o arquivo que contera' os potenciais du-caminhos */

 strcpy(filename,dir);
#ifdef TURBO_C
 strcat(filename,"\\");
#else
 strcat(filename,"/");
#endif
 strcat(filename,"pdupaths.tes");
 
 arq_pdu_paths = (FILE *) fopen(filename,"w");

 if(arq_pdu_paths == (FILE *) NULL)
    error("* * Erro Fatal: Nao consegui abrir o arquivo pdupaths.tes * *\n");


 fprintf(arq_des_pdu,"DESCRITORES PARA O CRITERIO TODOS POT-DU-CAMINHOS \n\n");
 fprintf(arq_des_pdu,"Numero total de nos: %d\n", no_nos);
 fprintf(arq_des_pdu,"Numero total de variaveis: %d\n\n", no_vars);

 fprintf(arq_pdu_paths,"CAMINHOS REQUERIDOS PELO CRITERIO TODOS POT-DU-CAMINHOS \n\n");

 /* escreve o conjunto de nos do grafo do modulo */
 
 fprintf(arq_des_pdu,"N = ");
 for(i=1; i <= no_nos; ++i)
  {
   set_bit(i,&N);  /* preenche N com os nos do grafo G */
   fprintf(arq_des_pdu,"%d ",i);
  }
 
 fprintf(arq_des_pdu,"\n");

 for(i=1; i <= no_grfi; i++)
 {
  fprintf(arq_des_pdu,"\n\nDescritores para o Grafo(%2d)\n\n",((pdescr[i].grafo_i)->infosuc).num_no_G);
  fprintf(arq_pdu_paths,"\n\nCaminhos requeridos pelo Grafo(%2d)\n\n",((pdescr[i].grafo_i)->infosuc).num_no_G);
  
  /* escreve o conjunto Ni do grafo(i) */

  fprintf(arq_des_pdu,"Ni = ");

  for(k=1; k>=0 && k <= no_nos; ++k)
     if(test_bit(k,&(pdescr[i].Ni)))
       fprintf(arq_des_pdu,"%d ",k);
     
  fprintf(arq_des_pdu,"\n");


  /* escreve o conjunto Nt do grafo(i) */

  fprintf(arq_des_pdu,"Nt = ");

  for(k=1; k>=0 && k <= no_nos; ++k)
     if(test_bit(k,&(pdescr[i].Nt)))
       fprintf(arq_des_pdu,"%d ",k);

  fprintf(arq_des_pdu,"\n\n");

  des_pdu_gen(&pdescr[i], g_suc,arq_des_pdu,arq_pdu_paths);
 }

 fprintf(arq_des_pdu,"\nNumero Total de Descritores = %d\n",n_des_pdu);

 fclose(arq_des_pdu);
 fclose(arq_pdu_paths);
 b_vector_destroy(&N);
}

/********************************************************************/
/* void gera_descr_pu(struct grafo *,INFODESCRITORES *, char *, int)*/
/* Autor: Marcos L. Chaim                                           */
/* Data: 04/10/99                                                   */
/* Versao: 1.0                                                      */
/*                                                                  */
/* Funcao: gera arquivos de descritores e com associacoes pot-usos e*/
/*         pot-usos/du.                                             */
/*                                                                  */
/* Entradas: apontador para gfc e grafo(i), diretorio e numero de   */
/*           grafo(i).                                              */
/*                                                                  */
/* Saida: nenhuma.                                                  */
/*                                                                  */
/* Variaveis Globais Utilizadas:                                    */
/*                                                                  */
/********************************************************************/

void gera_descr_pu(g_suc,pdescr, dir, no_grfi)
struct grafo * g_suc;
INFODESCRITORES * pdescr;
char *dir;
int no_grfi;
{
 FILE * arq_des_pu, * arq_des_pudu, * arq_pu_assoc;
 int i,k;
 char filename [300];
 b_vector N;

 b_vector_cons(&N,no_nos/BYTE+1,0);

 msg_print("* * Gerando descritores dos criterios todos-pu-usos e todos-pu-usos/du\n");

 /* geracao dos descritores e caminhos relativos ao criterios */

 /* criando arquivo que contera' os descritores */

 strcpy(filename,dir);
#ifdef TURBO_C
 strcat(filename,"\\");
#else
 strcat(filename,"/");
#endif
 strcat(filename,"des_pu.tes");
  
 arq_des_pu = (FILE *) fopen(filename,"w");

 if(arq_des_pu == (FILE *) NULL)
    error("* * Erro Fatal: Nao consegui abrir o arquivo des_pu.tes * *\n");

 strcpy(filename,dir);
#ifdef TURBO_C
 strcat(filename,"\\");
#else
 strcat(filename,"/");
#endif
 strcat(filename,"des_pudu.tes");
 
 arq_des_pudu = (FILE *) fopen(filename,"w");

 if(arq_des_pudu == (FILE *) NULL)
    error("* * Erro Fatal: Nao consegui abrir o arquivo des_pudu.tes * *\n");


 /* criando o arquivo que contera' as associacoes */

 strcpy(filename,dir);
#ifdef TURBO_C
 strcat(filename,"\\");
#else
 strcat(filename,"/");
#endif
 strcat(filename,"puassoc.tes");
 
 arq_pu_assoc = (FILE *) fopen(filename,"w");

 if(arq_pu_assoc == (FILE *) NULL)
    error("* * Erro Fatal: Nao consegui abrir o arquivo puassoc.tes * *\n");


 fprintf(arq_des_pu,"DESCRITORES PARA O CRITERIO TODOS POT-USOS \n\n");
 fprintf(arq_des_pu,"Numero total de nos: %d\n", no_nos);
 fprintf(arq_des_pu,"Numero total de variaveis: %d\n\n", no_vars);

 fprintf(arq_des_pudu,"DESCRITORES PARA O CRITERIO TODOS POT-USOS/DU \n\n");
 fprintf(arq_des_pudu,"Numero total de nos: %d\n", no_nos);
 fprintf(arq_des_pudu,"Numero total de variaveis: %d\n\n", no_vars);

 fprintf(arq_pu_assoc,"ASSOCIACOES REQUERIDAS PELOS CRITERIOS TODOS POT-USOS E POT-USOS/DU\n\n");


 /* escreve o conjunto de nos do grafo do modulo */

 fprintf(arq_des_pu,"N = ");
 fprintf(arq_des_pudu,"N = ");
 
 for(i=1; i <= no_nos; ++i)
    {
    set_bit(i,&N);  /* preenche N com os nos do grafo G */
    fprintf(arq_des_pu,"%d ",i);
    fprintf(arq_des_pudu,"%d ",i);
   }

 fprintf(arq_des_pu,"\n");
 fprintf(arq_des_pudu,"\n");

 for(i=1; i <= no_grfi; i++)
 {
  fprintf(arq_des_pu,"\n\nDescritores para o Grafo(%2d)\n\n",((pdescr[i].grafo_i)->infosuc).num_no_G);
  fprintf(arq_des_pudu,"\n\nDescritores para o Grafo(%2d)\n\n",((pdescr[i].grafo_i)->infosuc).num_no_G);
  fprintf(arq_pu_assoc,"\n\nAssociacoes requeridas pelo Grafo(%2d)\n\n",((pdescr[i].grafo_i)->infosuc).num_no_G);

  /* escreve o conjunto Ni do grafo(i) */

  fprintf(arq_des_pu,"Ni = ");
  fprintf(arq_des_pudu,"Ni = ");

  for(k=1; k>=0 && k <= no_nos; ++k)
  if(test_bit(k,&(pdescr[i].Ni)))
     {
      fprintf(arq_des_pu,"%d ",k);
      fprintf(arq_des_pudu,"%d ",k);
     }

  fprintf(arq_des_pu,"\n");
  fprintf(arq_des_pudu,"\n");

  /* escreve o conjunto Nt do grafo(i) */

  fprintf(arq_des_pu,"Nt = ");
  fprintf(arq_des_pudu,"Nt = ");

  for(k=1; k>=0 && k <= no_nos; ++k)
  if(test_bit(k,&(pdescr[i].Nt)))
    {
     fprintf(arq_des_pu,"%d ",k);
     fprintf(arq_des_pudu,"%d ",k);
    }

  fprintf(arq_des_pu,"\n\n");
  fprintf(arq_des_pudu,"\n\n");


  des_pu_gen(&pdescr[i],arq_des_pu,arq_des_pudu,arq_pu_assoc);

 }

 fprintf(arq_des_pu,"\nNumero Total de Descritores = %d\n",n_des_pu);
 fprintf(arq_des_pudu,"\nNumero Total de Descritores = %d\n",n_des_pu);

 fclose(arq_des_pu);
 fclose(arq_des_pudu);
 fclose(arq_pu_assoc);
 b_vector_destroy(&N);
}

/********************************************************************/
/* void libera_grfi(INFODESCRITORES *, int)                         */
/* Autor: Marcos L. Chaim                                           */
/* Data: 04/10/99                                                   */
/* Versao: 1.0                                                      */
/*                                                                  */
/* Funcao: esta funcao libera espaco ocupado pelos grafo(i).        */
/*                                                                  */
/* Entradas: apontador para grafo(i) e numero de grafo(i).          */
/*                                                                  */
/* Saida: nenhuma.                                                  */
/*                                                                  */
/* Variaveis Globais Utilizadas: nenhuma.                           */
/*                                                                  */
/********************************************************************/

void libera_grfi(pdescr,no_grfi)
INFODESCRITORES * pdescr;
int no_grfi;
{
 int i;

 for(i=1; i <= no_grfi; i++) 
    reorganiza_grafo(pdescr[i].grafo_i,&(pdescr[i].grafo_i));
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
 /* b_vector bitaux; */

 conodef = (struct no *) NULL;

 for(i=1; i >= 1 && i <= no_nos;++i)
    {
      /* bitaux = union_b_vector(&info_no[i].defg_i,&info_no[i].def_ref); */
     if(!empty_bit(&info_no[i].defg_i)||!empty_bit(&info_no[i].def_ref))
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
 error("* * Erro Fatal: Pilha do caminho vazia e nao encontrei elemento * *\n");
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

 ptr_lista_sucessores = suc = (LIST *) malloc(sizeof(LIST));
 if(suc == (LIST *) NULL)
   error("* * Erro Fatal: Nao consegui alocar espaco para a lista de sucessores * *\n");
 ptr_lista_sucessores->next = (LIST *) NULL;

 for(gaux = g_suc[elemento].list_suc;gaux != (struct no *) NULL;
     gaux = gaux->proximo)
     {
      suc->next = (LIST *)malloc(sizeof(LIST));
      if(suc->next == (LIST *) NULL)
        error("* * Erro Fatal: Nao consegui alocar espaco para a lista de sucessores * *\n");
      suc = suc->next;
      (suc->sucessor).num_no_G = gaux->num;
      /* (suc->sucessor).deff = info_no[elemento].defg_i; */
      
      b_vector_cons(&((suc->sucessor).deff),info_no[elemento].defg_i.nbytes,info_no[elemento].defg_i.type_in);
      b_vector_cpy(&((suc->sucessor).deff),&info_no[elemento].defg_i);
      
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

 INFODFNO aux;
 LIST * cabaux;

 cabaux = *cabeca;

 if(cabaux == (LIST *) NULL)
   error("* * Erro Fatal: Tentei retirar a \"cabeca\" de uma lista sem \"cabeca\" * *\n");

 *cabeca = cabaux->next;
 aux = cabaux->sucessor;
 free(cabaux);

 return(aux);
}

/********************************************************************/
/* int existe_nd_pil_cam(int, STACK *)                              */
/* Autor: Marcos L. Chaim                                           */
/* Data: 05/09/90                                                   */
/* Versao: 1.0                                                      */
/*                                                                  */
/* Funcao: esta funcao retorna verdadeiro caso o no' passado como   */
/*         parametro caso nao exita nenhum elemento na pil_cam que  */
/*         seja pertencente ao Nd; e falso caso contrario.          */
/*                                                                  */
/* Entradas: apontador para pil_cam e o conjunto Nd.                */
/*                                                                  */
/* Saida: inteiro indicando verdadeiro ou falso.                    */
/*                                                                  */
/********************************************************************/

int existe_Nd_pil_cam(pil_cam,Nd)
STACK * pil_cam;
b_vector * Nd;
{

 /* Declaracao de Variaveis Locais */

 STACK stack_aux;

 int aux, retorno = FALSE;

 mk_nl_int(&stack_aux);  /* inicia pilha auxiliar */

 while(!empty_int(pil_cam))
   {
    aux = pop_int(pil_cam);
    push_int(aux,&stack_aux);
    if(test_bit(aux,Nd))
        retorno = TRUE;
   }

 while(!empty_int(&stack_aux))
    push_int(pop_int(&stack_aux),pil_cam);

 return(retorno);
}








