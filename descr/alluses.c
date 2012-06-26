
/*
// alluses.c 	%I%	%Y%	%E%	%U%
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
#include "newpokelib.h"
#include "headerli.h"
#include "hparserg.h"
#include "hrotsem.h"
#include "header.h"
#include "hpducam.h"

/* Macro to check if there is a p-use in the successor of the def node */

#define  p_use_def_no(var,no_i,no_m,info_no) ((no_i == no_m)?(test_bit(var,&(info_no[no_i].p_use))):(FALSE))

/* Declaracao de Variaveis Globais */

extern int n_des_all_uses;
extern int n_des_puses;
extern int n_des_du_paths;
extern no_nos;
extern no_vars;

void all_uses(g,dir,mod,info_no,no_nos,no_vars)
struct grafo * g;
char *dir;
char *mod;
NODEINFO info_no;
int no_nos;
int no_vars;
{
 char filename[300];
 b_vector Ni;
 int no, var, i, j;
 l_info * v_c_use = NULL, * v_p_use = NULL, * v_du_paths = NULL;

 FILE * all_uses;
 FILE * all_usesMB;
 FILE * des_all_uses;
 FILE * all_p_uses;
 FILE * des_p_uses;

 b_vector_cons(&Ni, no_nos);
 
 /*
   Create the file that will contain the descriptors for all-uses
   criterion
 */

 strcpy(filename,dir);

#ifdef TURBO_C
 strcat(filename,"\\");
#else
 strcat(filename,"/");
#endif
 strcat(filename,"des_all_uses.tes");
 
 des_all_uses = (FILE *) fopen(filename,"w");

 if(des_all_uses == (FILE *) NULL)
    error("* * Erro Fatal: Nao consegui abrir o arquivo des_all_uses.tes * *\n");

 /*
   Create the file that will contain the associations of the all-uses
   criterion
 */

 strcpy(filename,dir);
#ifdef TURBO_C
 strcat(filename,"\\");
#else
 strcat(filename,"/");
#endif
 strcat(filename,"all_uses.tes");
 
 all_uses = (FILE *) fopen(filename,"w");

 if(all_uses == (FILE *) NULL)
    error("* * Erro Fatal: Nao consegui abrir o arquivo all_uses.tes * *\n");

/* 
  all_uses_MB is similar to all_uses file but
  with numbers of variables, instead of
  variables names.Created to be used
  in the Marre e Bertolino algorithm.
 */

 strcpy(filename,dir);
#ifdef TURBO_C
 strcat(filename,"\\");
#else
 strcat(filename,"/");
#endif
 strcat(filename,"all_usesMB.tes");
 
 all_usesMB = (FILE *) fopen(filename,"w");

 if(all_usesMB == (FILE *) NULL)
    error("* * Erro Fatal: Nao consegui abrir o arquivo all_usesMB.tes * *\n");

 fprintf(des_all_uses,"DESCRITORES PARA O CRITERIO TODOS-USOS PARA O MODULO %s\n\n",mod);
 fprintf(des_all_uses,"Numero total de nos: %d\n", no_nos);
 fprintf(des_all_uses,"Numero total de variaveis: %d\n\n", no_vars);
 
 fprintf(all_uses,"ASSOCIACOES REQUERIDAS PELO CRITERIO TODOS-USOS PARA O MODULO %s",mod);

 fprintf(all_usesMB,"ASSOCIACOES REQUERIDAS PELO CRITERIO TODOS-USOS PARA O MODULO %s NO FORMATO PARA USO PELO ALGORITMO DE MARRE E BERTOLINO",mod);
 
 /*
   Create the file that will contain the descriptors for all-p-uses
   criterion
 */

 strcpy(filename,dir);

#ifdef TURBO_C
 strcat(filename,"\\");
#else
 strcat(filename,"/");
#endif
 strcat(filename,"des_p_uses.tes");
 
 des_p_uses = (FILE *) fopen(filename,"w");

 if(des_p_uses == (FILE *) NULL)
    error("* * Erro Fatal: Nao consegui abrir o arquivo des_p_uses.tes * *\n");

 /*
   Create the file that will contain the associations of the all-p-uses
   criterion
 */

 strcpy(filename,dir);
#ifdef TURBO_C
 strcat(filename,"\\");
#else
 strcat(filename,"/");
#endif
 strcat(filename,"all_p_uses.tes");
 
 all_p_uses = (FILE *) fopen(filename,"w");

 if(all_p_uses == (FILE *) NULL)
    error("* * Erro Fatal: Nao consegui abrir o arquivo all_uses.tes * *\n");

 fprintf(des_p_uses,"DESCRITORES PARA O CRITERIO TODOS-USOS-PREDICATIVOS PARA O MODULO %s\n\n",mod);
 fprintf(des_p_uses,"Numero total de nos: %d\n", no_nos);
 fprintf(des_p_uses,"Numero total de variaveis: %d\n\n", no_vars);


 fprintf(all_p_uses,"ASSOCIACOES REQUERIDAS PELO CRITERIO TODOS-USOS-PREDICATIVOS PARA O MODULO %s",mod);

 /* escreve o conjunto de nos do grafo do modulo */
 
 fprintf(des_all_uses,"N = ");
 fprintf(des_p_uses,"N = ");
 for(i=1; i <= no_nos; ++i)
    {
    fprintf(des_all_uses,"%d ",i);
    fprintf(des_p_uses,"%d ",i);
    }
 fprintf(des_all_uses,"\n");
 fprintf(des_p_uses,"\n");
 
 for(no=1; no < no_nos; ++no)
   {
   for(var=0; var < no_vars; var++)
     if(test_bit(var,&(info_no[no].defg_i)) ||
        test_bit(var,&(info_no[no].def_ref)))
        {
        l_info_insert_info(&v_du_paths,no);
        visit_node_all_uses(des_all_uses,all_uses,all_usesMB,des_p_uses,all_p_uses,g,info_no,no,var,no,&v_c_use,&v_p_use,&v_du_paths);
        v_c_use = l_info_free(v_c_use);
        v_p_use = l_info_free(v_p_use);
        v_du_paths = l_info_free(v_du_paths);
        }
   }

 b_vector_destroy(&Ni);

 fprintf(des_all_uses,"\nNumero Total de Descritores = %d\n",n_des_all_uses-1);
 fprintf(des_p_uses,"\nNumero Total de Descritores = %d\n",n_des_puses-1);

 fclose(all_uses);
 fclose(all_usesMB);
 fclose(des_all_uses);
 fclose(all_p_uses);
 fclose(des_p_uses);
}

void visit_node_all_uses(des_uses,req_uses,req_usesMB,des_puses,req_puses,g,info_no,no_i,var,no_m,vv_c_use,vv_p_use,vv_du_paths)
FILE *des_uses;
FILE *req_uses;
FILE *req_usesMB;
FILE *des_puses;
FILE *req_puses;
struct grafo * g;
NODEINFO info_no;
int no_i;
int var;
int no_m;
l_info ** vv_c_use;
l_info ** vv_p_use;
l_info ** vv_du_paths;
{
struct grafo * gaux;
struct no * sucnodes,* sucnodes1;
int no_j, no_k, i;

 /* Check p-use in the succesors of no_i */

 if(test_bit(var,&(info_no[no_m].p_use)))
     for(sucnodes1=g[no_m].list_suc; sucnodes1 != NULL;
         sucnodes1=sucnodes1->proximo)
        {
        no_k = sucnodes1->num;
        if(*vv_c_use == NULL && *vv_p_use == NULL)
          {
          fprintf(des_uses,"\n\nDescritores para o par (");
          show_description(var,des_uses);
          fprintf(des_uses,",%2d)\n\n",no_m);
          fprintf(des_uses,"Nnv = ");
          for(i=1; i <= no_nos;++i)
            if(!test_bit(var,&(info_no[i].defg_i)))
              fprintf(des_uses,"%d ", i);

          fprintf(des_uses,"\n\n");
          
          fprintf(req_uses,"\n\nAssociacoes relativas ao par (");
          fprintf(req_usesMB,"\n\nAssociacoes relativas ao par ( ");

          show_description(var,req_uses);
          fprintf(req_usesMB," %d ",var);

          fprintf(req_uses,",%2d)\n\n",no_m);
          fprintf(req_usesMB,", %2d )\n\n",no_m);

          }
        
        if(*vv_p_use == NULL)
          {
          fprintf(des_puses,"\n\nDescritores para o par (");
          show_description(var,des_puses);
          fprintf(des_puses,",%2d)\n\n",no_m);
          fprintf(des_puses,"Nnv = ");
          for(i=1; i <= no_nos;++i)
            if(!test_bit(var,&(info_no[i].defg_i)))
              fprintf(des_puses,"%d ", i);

          fprintf(des_puses,"\n\n");
          
          fprintf(req_puses,"\n\nAssociacoes relativas ao par (");
          show_description(var,req_puses);
          fprintf(req_puses,",%2d)\n\n",no_m);
          }

        if(!l_info_is_in_2info(*vv_p_use,no_m,no_k))
          {
          l_info_insert_2info(vv_p_use,no_m,no_k);

          /* Describe the p-use in the all-p-uses descriptors */

          fprintf(req_puses,"%d) <%d,(%d,%d),",n_des_puses,no_m,no_m,no_k);
          show_description(var,req_puses);
          fprintf(req_puses,">\n");
 
          fprintf(des_puses,"%d) N* %d [ Nnv* %d ]* %d\n",n_des_puses,no_m,no_m,no_k);
          ++n_des_puses;

          /* Describe the p-use in the all-uses descriptors */

          fprintf(req_uses,"%d) <%d,(%d,%d),",n_des_all_uses,no_m,no_m,no_k);
          fprintf(req_usesMB,"%d ) < %d , ( %d , %d ) ,",n_des_all_uses,no_m,no_m,no_k);

          show_description(var,req_uses);
          fprintf(req_usesMB," %d ",var);

          fprintf(req_uses,">\n");
          fprintf(req_usesMB,">\n");
 
          fprintf(des_uses,"%d) N* %d [ Nnv* %d ]* %d\n",n_des_all_uses,no_m,no_m,no_k);
          ++n_des_all_uses;
          }
   
        }

 for(sucnodes=g[no_m].list_suc; sucnodes != NULL; sucnodes=sucnodes->proximo)
   {
   no_j = sucnodes->num;
  
   if(test_bit(var,&(info_no[no_j].c_use)))
     {
     if(*vv_c_use == NULL && *vv_p_use == NULL)
       {
       fprintf(des_uses,"\n\nDescritores para o par (");
       show_description(var,des_uses);
       fprintf(des_uses,",%2d)\n\n",no_i);
       fprintf(des_uses,"Nnv = ");
       for(i=1; i <= no_nos;++i)
         {
         if(!test_bit(var,&(info_no[i].defg_i)))
           fprintf(des_uses,"%d ", i);
         }
       fprintf(des_uses,"\n\n");

       fprintf(req_uses,"\n\nAssociacoes relativas ao par (");
       fprintf(req_usesMB,"\n\nAssociacoes relativas ao par ( ");

       show_description(var,req_uses);
       fprintf(req_usesMB," %d ",var);

       fprintf(req_uses,",%2d)\n\n",no_i);
       fprintf(req_usesMB,", %2d )\n\n",no_i);
       }
     
     if(!l_info_is_in_info(*vv_c_use,no_j))
       {
       l_info_insert_info(vv_c_use,no_j);
       fprintf(req_uses,"%d) <%d,%d,",n_des_all_uses,no_i,no_j);
       fprintf(req_usesMB,"%d ) < %d , %d ,",n_des_all_uses,no_i,no_j);

       show_description(var,req_uses);
       fprintf(req_usesMB," %d",var);

       fprintf(req_uses,">\n");
       fprintf(req_usesMB," >\n");

       fprintf(des_uses,"%d) N* %d Nnv* %d\n",n_des_all_uses,no_i,no_j);
       ++n_des_all_uses;
       }
     /* l_info_print(stdout,*vv_du_paths); */
     }

    
   if(test_bit(var,&(info_no[no_j].p_use)))
     for(sucnodes1=g[no_j].list_suc; sucnodes1 != NULL;
         sucnodes1=sucnodes1->proximo)
        {
        no_k = sucnodes1->num;
        if(*vv_c_use == NULL && *vv_p_use == NULL)
          {
          fprintf(des_uses,"\n\nDescritores para o par (");
          show_description(var,des_uses);
          fprintf(des_uses,",%2d)\n\n",no_i);
          fprintf(des_uses,"Nnv = ");
          for(i=1; i <= no_nos;++i)
            if(!test_bit(var,&(info_no[i].defg_i)))
              fprintf(des_uses,"%d ", i);

          fprintf(des_uses,"\n\n");
          
          fprintf(req_uses,"\n\nAssociacoes relativas ao par (");
          fprintf(req_usesMB,"\n\nAssociacoes relativas ao par ( ");

          show_description(var,req_uses);
          fprintf(req_usesMB," %d ",var);

          fprintf(req_uses,",%2d)\n\n",no_i);
          fprintf(req_usesMB,", %2d )\n\n",no_i);
          }
        
        if(*vv_p_use == NULL)
          {
          fprintf(des_puses,"\n\nDescritores para o par (");
          show_description(var,des_puses);
          fprintf(des_puses,",%2d)\n\n",no_i);
          fprintf(des_puses,"Nnv = ");
          for(i=1; i <= no_nos;++i)
            if(!test_bit(var,&(info_no[i].defg_i)))
              fprintf(des_puses,"%d ", i);

          fprintf(des_puses,"\n\n");
          
          fprintf(req_puses,"\n\nAssociacoes relativas ao par (");
          show_description(var,req_puses);
          fprintf(req_puses,",%2d)\n\n",no_i);
          }

        if(!l_info_is_in_2info(*vv_p_use,no_j,no_k))
          {
          l_info_insert_2info(vv_p_use,no_j,no_k);

          /* Describe the p-use in the all-p-uses descriptors */

          fprintf(req_puses,"%d) <%d,(%d,%d),",n_des_puses,no_i,no_j,no_k);
          show_description(var,req_puses);
          fprintf(req_puses,">\n");
 
          if(no_i == no_j)
          	fprintf(des_uses,"%d) N* %d [ Nnv* %d ]* %d\n",n_des_all_uses,no_i,no_j,no_k);
	  else
          	fprintf(des_puses,"%d) N* %d Nnv* %d [ Nnv* %d ]* %d\n",n_des_puses,no_i,no_j,no_j,no_k);
          ++n_des_puses;

          /* Describe the p-use in the all-uses descriptors */

          fprintf(req_uses,"%d) <%d,(%d,%d),",n_des_all_uses,no_i,no_j,no_k);
          fprintf(req_usesMB,"%d ) < %d , ( %d , %d ) , ",n_des_all_uses,no_i,no_j,no_k);

          show_description(var,req_uses);
          fprintf(req_usesMB," %d",var);

          fprintf(req_uses,">\n");
          fprintf(req_usesMB," >\n");
 
          if(no_i == no_j)
          	fprintf(des_uses,"%d) N* %d [ Nnv* %d ]* %d\n",n_des_all_uses,no_i,no_j,no_k);
	  else
          	fprintf(des_uses,"%d) N* %d Nnv* %d [ Nnv* %d ]* %d\n",n_des_all_uses,no_i,no_j,no_j,no_k);
          ++n_des_all_uses;
          }
   
        }

   if(!test_bit(var, &(info_no[no_j].defg_i)))
     {
     if(!l_info_is_in_info(*vv_du_paths,no_j))
       {
       l_info_insert_info(vv_du_paths,no_j);
       visit_node_all_uses(des_uses,req_uses,req_usesMB,des_puses,req_puses,g,info_no,no_i,var,no_j,vv_c_use,vv_p_use,vv_du_paths);
       }
     }
   }
}

void all_du_paths(g,dir,mod,info_no,no_nos,no_vars)
struct grafo * g;
char *dir;
char *mod;
NODEINFO info_no;
int no_nos;
int no_vars;
{
 char filename[300];
 int no, var, i, j, header = FALSE;
 l_info * v_du_paths = NULL;
 b_vector deff;
 FILE * all_du_paths;
 FILE * des_all_du_paths;
 struct no * sucnodes;

 b_vector_cons(&deff,no_vars/BYTE+1,0);
 
 /*
   Create the file that will contain the descriptors for all-du-paths
   criterion
 */

 strcpy(filename,dir);

#ifdef TURBO_C
 strcat(filename,"\\");
#else
 strcat(filename,"/");
#endif
 strcat(filename,"des_all_du_paths.tes");
 
 des_all_du_paths = (FILE *) fopen(filename,"w");

 if(des_all_du_paths == (FILE *) NULL)
    error("* * Erro Fatal: Nao consegui abrir o arquivo des_all_du_paths.tes * *\n");

 /*
   Create the file that will contain the associations of the all-du-paths
   criterion
 */

 strcpy(filename,dir);
#ifdef TURBO_C
 strcat(filename,"\\");
#else
 strcat(filename,"/");
#endif
 strcat(filename,"all_du_paths.tes");
 
 all_du_paths = (FILE *) fopen(filename,"w");

 if(all_du_paths == (FILE *) NULL)
    error("* * Erro Fatal: Nao consegui abrir o arquivo all_du_paths.tes * *\n");

 fprintf(des_all_du_paths,"DESCRITORES PARA O CRITERIO TODOS-DU-CAMINHOS PARA O MODULO %s\n\n",mod);
 fprintf(des_all_du_paths,"Numero total de nos: %d\n", no_nos);
 fprintf(des_all_du_paths,"Numero total de variaveis: %d\n\n", no_vars);

 
 fprintf(all_du_paths,"CAMINHOS REQUERIDOS PELO CRITERIO TODOS-DU-CAMINHOS PARA O MODULO %s",mod);

 /* escreve o conjunto de nos do grafo do modulo */
 
 fprintf(des_all_du_paths,"N = ");
 for(i=1; i <= no_nos; ++i)
    fprintf(des_all_du_paths,"%d ",i);
    
 fprintf(des_all_du_paths,"\n");

 for(no=1; no < no_nos; ++no)
   {
     if(!empty_bit(&(info_no[no].defg_i)) ||
        !empty_bit(&(info_no[no].def_ref)))
        {
        b_vector_cpy(&deff,&(info_no[no].defg_i));
        (void) union_b_vector(&deff,&(info_no[no].def_ref));

        (void) inter_b_vector(&deff,&(info_no[no].p_use));

        if(!empty_bit(&deff))
          {
          for(sucnodes=g[no].list_suc; sucnodes != NULL;
              sucnodes=sucnodes->proximo)
             {
             j = sucnodes->num;

             if(!header)
               {
               fprintf(des_all_du_paths,"\n\nDescritores relativos ao no' (%2d)\n\n",no);
               fprintf(all_du_paths,"\n\nCaminhos relativos ao no' (%2d)\n\n",no);
               header = TRUE;
               }
        
             /* Print no_i, no_j */
        
             fprintf(des_all_du_paths,"%d) N* %d %d\n",n_des_du_paths,no,j);  
             fprintf(all_du_paths,"%d) %d %d\n",n_des_du_paths,no,j);

             ++n_des_du_paths;
             }

          }
        
        b_vector_cpy(&deff,&(info_no[no].defg_i));
        (void) union_b_vector(&deff,&(info_no[no].def_ref));
                                
        /* l_info_insert_info(&v_du_paths,no); */
        visit_node_du_paths(des_all_du_paths,all_du_paths,g,info_no,&deff,no,no,&v_du_paths,&header);
        v_du_paths = l_info_free(v_du_paths);
        header = FALSE;
        }
   }

 fprintf(des_all_du_paths,"\nNumero Total de Descritores = %d\n",n_des_du_paths-1);
 
 fclose(all_du_paths);
 fclose(des_all_du_paths);
}

void visit_node_du_paths(des,req,g,info_no,prevdeff,no_i,no_m,vv_du_paths,pheader)
FILE *des;
FILE *req;
struct grafo * g;
NODEINFO info_no;
b_vector * prevdeff;
int no_i;
int no_m;
l_info ** vv_du_paths;
int *pheader;
{
 struct grafo * gaux;
 struct no * sucnodes,* sucnodes1;
 int no_j, no_k, i;
 b_vector curdeff;
 b_vector res, res1;
 
 b_vector_cons(&curdeff,no_vars/BYTE+1,0);
 b_vector_cons(&res,no_vars/BYTE+1,0);
 b_vector_cons(&res1,no_vars/BYTE+1,0);
 
 
 
 for(sucnodes=g[no_m].list_suc; sucnodes != NULL; sucnodes=sucnodes->proximo)
   {
   no_j = sucnodes->num;
   b_vector_cpy(&curdeff,prevdeff);

   b_vector_cpy(&res,&curdeff);
   (void) inter_b_vector(&res,&(info_no[no_j].c_use));

   b_vector_cpy(&res1,&curdeff);
   (void) inter_b_vector(&res1,&(info_no[no_m].p_use));
   
   if(!empty_bit(&res) && empty_bit(&res1))
     {
     if(!*pheader)
       {
       fprintf(des,"\n\nDescritores relativos ao no' (%2d)\n\n",no_i);
       fprintf(req,"\n\nCaminhos relativos ao no' (%2d)\n\n",no_i);
       *pheader = TRUE;
       }

     /* Print no_i */

     fprintf(des,"%d) N* %d",n_des_du_paths,no_i);  
     fprintf(req,"%d) %d",n_des_du_paths,no_i);
          
     l_info_print(des,*vv_du_paths);
     l_info_print(req,*vv_du_paths);

     /* Print no_j */

     fprintf(des," %d\n",no_j);
     fprintf(req," %d\n",no_j);
     ++n_des_du_paths;
     }

   b_vector_cpy(&res,&curdeff);
   
   (void) inter_b_vector(&res,&(info_no[no_j].p_use));
   if(!empty_bit(&res) && !l_info_is_in_info(*vv_du_paths,no_j) &&
      no_i != no_j)
     for(sucnodes1=g[no_j].list_suc; sucnodes1 != NULL;
         sucnodes1=sucnodes1->proximo)
        {
        no_k = sucnodes1->num;

        if(!*pheader)
          {
          fprintf(des,"\n\nDescritores relativos ao no' (%2d)\n\n",no_i);
          fprintf(req,"\n\nCaminhos relativos ao no' (%2d)\n\n",no_i);
          *pheader = TRUE;
          }
        
        /* Print no_i */
        
        fprintf(des,"%d) N* %d",n_des_du_paths,no_i);  
        fprintf(req,"%d) %d",n_des_du_paths,no_i);
        
        l_info_print(des,*vv_du_paths);
        l_info_print(req,*vv_du_paths);

        /* Print no_j, no_k */

        fprintf(des," %d %d\n",no_j, no_k);
        fprintf(req," %d %d\n",no_j, no_k);
        ++n_des_du_paths;
        }

   b_vector_cpy(&res,&(info_no[no_j].defg_i));
   (void) neg_b_vector(&res);
   (void) inter_b_vector(&curdeff,&res);
   
   if(!empty_bit(&curdeff))
     {
     if(!l_info_is_in_info(*vv_du_paths,no_j) && no_i != no_j)
       {
       l_info_insert_info(vv_du_paths,no_j);
       visit_node_du_paths(des,req,g,info_no,&curdeff,no_i,no_j,vv_du_paths,pheader);
       /* Withdraw the recently visited node no_j */
       l_info_withdraw_last_info(vv_du_paths);
       }
     }
   }
 b_vector_destroy(&curdeff);
 b_vector_destroy(&res);
 b_vector_destroy(&res1);
}


/*
** End of alluses.c
*/





