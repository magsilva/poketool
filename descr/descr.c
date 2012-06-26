#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef TURBO_C

#include <alloc.h>
#include <conio.h>

#else  /* SunOS, BSD4.2 e RISC 6000 */

#include <malloc.h>

#endif

#include "util.h"

#include "hrotsem.h"
#include "header.h"
#include "hpducam.h"

/* Variaveis Globais */

/* Variaveis para a criacao do Grafo Def da Funcao */

NODEINFO info_no;
b_vector flags;
b_vector undef;
DESPOINTER names;

/* Variaveis utilizadas no calculo dos arcos primitivos */

struct grafo * graph = (struct grafo *) NULL;
struct grafo * graphaux = (struct grafo *) NULL;

int no_nos;  /*  numero de nos do GFC */
int no_vars; /*  numero de vars do modulo */

/* Variaveis Utilizadas na Avaliacao */

int n_des_pdu; /* numero de descritores para o criterio todos pot-du-caminhos */
int n_des_pu;  /* numero de descritores para o criterio todos pot-usos e pot-usos/du */
int n_des_all_uses=1;
int n_des_puses=1;
int n_des_du_paths=1;

			 /* PROGRAMA PRINCIPAL */

enum criterios {
GFC, /* apenas criterios baseados em analise de fluxo de controle */
PU,  /* criterios GFC + pot-usos + pot-usos/du */
PDU, /* so' PDU */
DF,  /* criterios all-uses + all-p-uses */
DF_PU, /* criterios GFC + all-uses + all-p-uses + pot-usos + pot-usos/du */
DUP, /* criterio all-du-paths */
ALL, /* todos criterios */
};

void main(argc,argv)
int argc;
char *argv[];
{
/* Declaracao de Variaveis locais */

/* Apontadores para o arquivos utilizados */

FILE * grafodef;        /* aponta para arquivo que contem o grafo def */
INFODESCRITORES * pdes; /* apontador para o vetor de grafo(i) */
int i,j;
int no_grafo_i;
int criterio;

struct no * aux;
char dir[250];  /* nome do diretorio para armazenamento dos descritores*/
char gfc[200];  /* nome do arquivo que contem o GFC */
char func[200]; /* nome da funcao a ser analisada */

 if(argc!=4)
   {
    msg_print("uso: newdescr -c<criterios> <nome da funcao> -d<nome do diretorio>\n");
    exit(1);
   }

 if(!strncmp(argv[1],"-c",2))
   {
   if(!strcmp(argv[1]+2,"gfc"))
     criterio=GFC;
   else
     if(!strcmp(argv[1]+2,"pu"))
       criterio=PU;
     else
       if(!strcmp(argv[1]+2,"df"))
         criterio=DF;
       else
         if(!strcmp(argv[1]+2,"pdu"))
           criterio=PDU;
         else
           if(!strcmp(argv[1]+2,"dup"))
             criterio=DUP;
           else
             if(!strcmp(argv[1]+2,"df-pu"))
               criterio=DF_PU;
             else
               criterio=ALL;
     
   }
 else
   error("* * Erro Fatal: Criterio definido incorretamente * *\n");
 
 strcpy(func,argv[2]);

 if(!strncmp(argv[3],"-d",2))
   strcpy(dir,argv[3]+2);
 else
   error("* * Erro Fatal: Nome do diretorio ausente * *\n");

 strcpy(gfc,dir);

#ifdef TURBO_C
 strcat(gfc,"\\");
#else
 strcat(gfc,"/");
#endif

 strcat(gfc,func);
 strcat(gfc,".gfc");

 no_nos=inigrf(gfc,&graph);

#ifdef DEBUG

 for(i=1;i<=no_nos;++i)
  {
   printf("sucessores do no' %d: ",i);
   for(aux=graph[i].list_suc;aux!=NULL;aux=aux->proximo)
      printf("%d ",aux->num);
   printf("\n");
  }
 printf("\n\n");
#endif

 graphaux=det_pred(graph,no_nos); /* gera o GFC como um grafo de predecessores */

#ifdef DEBUG

 for(i=1;i<=no_nos;++i)
   {
    printf("predecessores do no' %d: ",i);
    for(aux=graphaux[i].list_suc;aux!=NULL;aux=aux->proximo)
       printf("%d ",aux->num);
    printf("\n");
   }

#endif

 msg_print("* * Calculando os arcos primitivos... * *\n");
 cal_prim(graph,graphaux,no_nos);

 /* inicia_glb(); */

/* Determina descritores do criterio todos-nos */

 det_nos(no_nos,func,dir);

/* Determina descritores do criterio todos-arcos */

 det_arcs(graph,no_nos,func,dir);

/* Obtem o Grafo Def da funcao */

 cria_grafo_def(dir);

#ifdef DEBUG

#endif
 
 switch(criterio)
   {
   case GFC:
     break;
   case DF:
     all_uses(graph,dir,func,info_no,no_nos,no_vars);
     break;
   case DUP:
     all_du_paths(graph,dir,func,info_no,no_nos,no_vars);
     break;
   case DF_PU:
     all_uses(graph,dir,func,info_no,no_nos,no_vars);
     pdes=det_pot_du_cam(graph,dir,&no_grafo_i);
     gera_descr_pu(graph,pdes,dir,no_grafo_i);
     libera_grfi(pdes,no_grafo_i);
     break;     
   case PU:
     pdes=det_pot_du_cam(graph,dir,&no_grafo_i);
     gera_descr_pu(graph,pdes,dir,no_grafo_i);
     libera_grfi(pdes,no_grafo_i);
     break;
   case PDU:
     pdes=det_pot_du_cam(graph,dir,&no_grafo_i);
     gera_descr_pdu(graph,pdes,dir,no_grafo_i);
     libera_grfi(pdes,no_grafo_i);
     break;
   default:
     pdes=det_pot_du_cam(graph,dir,&no_grafo_i);
     all_uses(graph,dir,func,info_no,no_nos,no_vars);
     all_du_paths(graph,dir,func,info_no,no_nos,no_vars);
     gera_descr_pu(graph,pdes,dir,no_grafo_i);
     gera_descr_pdu(graph,pdes,dir,no_grafo_i);
     libera_grfi(pdes,no_grafo_i);
     break;     
   }
 
 lib_mem("* * Geracao de Descritores da POKETOOL foi bem sucedida * *\n");

 exit(0);
}
/*
   imprime no campo mensagem
*/

void msg_print(msg)
char *msg;
{
 printf("%s",msg);
}

/*
** void det_arcs() -- determina os descritores para o criterio todos os arcos.
*/

void det_arcs(grf,nos,funcao,dir)
struct grafo * grf;
int nos;
char *funcao;
char *dir;
{
 /* Declaracao de Variaveis Locais */

 FILE * arcprim;    /* aponta para arquivo que contem os arcos primitivos */
 FILE * des_arc;    /* aponta para arquivo que contem os descritores dos 
                      arcos primitivos */
 int i,j;
 struct no * aux;
 char filename[300];

/* Cria arquivo com os requisitos de teste do criterio todos arcos */

 strcpy(filename,dir);
#ifdef TURBO_C
 strcat(filename,"\\");
#else
 strcat(filename,"/");
#endif
 strcat(filename,"arcprim.tes");

 arcprim = (FILE *) fopen(filename,"w");

 if(arcprim == (FILE *) NULL)
   error("* * Erro Fatal: Nao consegui abrir o arquivo \"arcprim.tes\" * *\n");

 /* Cria arquivo com os descritores do criterio todos arcos */

 strcpy(filename,dir);
#ifdef TURBO_C
 strcat(filename,"\\");
#else
 strcat(filename,"/");
#endif
 strcat(filename,"des_arc.tes");

des_arc= fopen(filename,"w");

 if(des_arc==(FILE *)NULL)
    error("* * Erro Fatal: Nao consegui abrir o arquivo \"des_nos.tes\" * *");

 fprintf(arcprim,"ARCOS PRIMITIVOS DO MODULO %s\n\n",funcao);
 fprintf(des_arc,"Descritores do Criterio Todos-Arcos do Modulo %s\n\n",funcao);
 fprintf(des_arc,"Numero total de nos: %d\n",nos);

 fprintf(des_arc,"\nN = ");

 for(i=1;i<=nos;++i) fprintf(des_arc,"%d ",i);
 fprintf(des_arc,"\n\n");

 j = 1;
 for(i=1;i<=nos;++i)
  for(aux=grf[i].list_suc;aux!=NULL;aux=aux->proximo)
    if(aux->marca=='p')
      {
      fprintf(arcprim,"%3d) arco (%2d,%2d)\n",j,i,aux->num);
      fprintf(des_arc,"%3d) N* %2d %2d\n",j,i,aux->num);
      ++j;
      }

 fprintf(des_arc,"\nNumero Total de Descritores = %d\n",j);

 fclose(des_arc);
 fclose(arcprim);
}

/*
** void det_nos() -- determina os descritores para o criterio todos os nos.
*/

void det_nos(nos,funcao,dir)
int nos;
char * funcao;
char *dir;
{
 /* Declaracao de Variaveis Locais */

 FILE * des_nos; /* arquivo que contem os descritores dos nos */
 FILE * nos_grf; /* arquivo que contem os nos do grafo de fluxo de controle */
 int i;
 char filename[300];

 /* Cria arquivo com o requisitos de teste para criterio todos nos */

 strcpy(filename,dir);
#ifdef TURBO_C
 strcat(filename,"\\");
#else
 strcat(filename,"/");
#endif
 strcat(filename,"nos_grf.tes");

nos_grf = (FILE *) fopen(filename,"w");

 if(nos_grf == (FILE *) NULL)
   error("* * Erro Fatal: Nao consegui abrir o arquivo \"nos_grf.tes\" * *\n");

 fprintf(nos_grf,"NO'S DO MODULO %s\n\n",funcao);

 for(i=1;i<=nos;++i)
	{
	if((i%10) != 0)
		fprintf(nos_grf,"%3d ",i);
	else
		fprintf(nos_grf,"%3d\n",i);
	}

 fprintf(nos_grf,"\n");
 fclose(nos_grf);

 /* Cria descritor para criterio todos nos */

 strcpy(filename,dir);
#ifdef TURBO_C
 strcat(filename,"\\");
#else
 strcat(filename,"/");
#endif
 strcat(filename,"des_nos.tes");

 des_nos=fopen(filename,"w");

 if(des_nos==(FILE *)NULL)
    error("* * Erro Fatal: Nao consegui abrir o arquivo \"des_nos.tes\" * *");

 fprintf(des_nos,"Descritores do Criterio Todos-No's do Modulo %s\n\n",funcao);
 fprintf(des_nos,"Numero total de nos: %d\n",nos);

 fprintf(des_nos,"\nN = ");

 for(i=1;i<=nos;++i) fprintf(des_nos,"%d ",i);
 fprintf(des_nos,"\n\n");

 for(i=1;i<=nos;++i)
     fprintf(des_nos,"%3d) N* %3d \n",i,i);

 fprintf(des_nos,"\nNumero Total de Descritores = %d\n",nos);
 fclose(des_nos);
}

/*
** 			Fim descr.c
*/






