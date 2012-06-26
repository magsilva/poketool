/*
** salvagrf.c 	%I%	%Y%	%E%	%U%
*/

#include <stdio.h>
#include <stdlib.h>

#include "newpokelib.h"
#include "util.h"
#include "header.h"
#include "hparserg.h"
#include "hrotsem.h"


/* Declaracao de Variaveis Globais */

extern int num_no;
extern int only_glb_used;

extern FILE * arqfonte;
extern DESPOINTER names;
extern NODEINFO info_no;
extern struct grafo * graph;

extern table_element * pvarname;

void elimina_variaveis_locais PROTO ((void));
/*******************************************************************/
/* salva_grafo_def(NODEINFO,char*)                                 */
/* Autor: Marcos L. Chaim                                          */
/* Data: 26/12/94                                                  */
/* Versao: 1.0                                                     */
/*                                                                 */
/*                                                                 */
/* funcao: salva o grafo def em arquivo no diretorio da funcao.    */
/*                                                                 */
/* Entradas: ponteiro para estrutura info_no e nome do diretorio.  */
/*                                                                 */
/* Saida: nenhuma.                                                 */
/*                                                                 */
/*******************************************************************/


void salva_grafo_def(info_no,dir)
NODEINFO info_no;
char * dir;
{
/* Declaracao de Variaveis Locais */

FILE * grafodef, *def_use_ptr;
int i;
char filename[300];
struct no * node;

/* Cria diretorio da funcao */

 cria_dir(dir);

/* Cria grafo def */

#ifdef TURBO_C
 strcpy(filename,".\\");
#else
 strcpy(filename,"./");
#endif

 strcat(filename,dir);

#ifdef TURBO_C
 strcat(filename,"\\");
#else
 strcat(filename,"/");
#endif

 strcat(filename,"grafodef.tes");

 grafodef = (FILE *) fopen(filename,"w");
 if(grafodef == (FILE *) NULL)
    error("* * Erro Fatal: Nao consegui abrir o arquivo grafodef.tes * *");

/* Cria arquivo def-use-ptr */

#ifdef TURBO_C
 strcpy(filename,".\\");
#else
 strcpy(filename,"./");
#endif

 strcat(filename,dir);

#ifdef TURBO_C
 strcat(filename,"\\");
#else
 strcat(filename,"/");
#endif
 /*
 strcat(filename,"def-use-ptr.tes");

 def_use_ptr = (FILE *) fopen(filename,"w");
 if(def_use_ptr == (FILE *) NULL)
    error("* * Erro Fatal: Nao consegui abrir o arquivo def-use-ptr.tes * *");
 */
 fprintf(grafodef,"VARIAVEIS DEFINIDAS nos NOS do modulo %s\n\n", dir);
 fprintf(grafodef,"Variaveis Definidas  = Vars Defs\n");
 fprintf(grafodef,"Variaveis Usos Computacionais  = Vars C-Usos\n");
 fprintf(grafodef,"Variaveis Usos Predicativos  = Vars P-Usos\n");
 fprintf(grafodef,"Variaveis possivelmente definidas por Referencia = Vars Refs\n\n");

 /*
 fprintf(def_use_ptr,"VARIAVEIS DEFINIDAS nos NOS do modulo %s\n\n", dir);
 fprintf(def_use_ptr,"Variaveis Definidas  = Vars Defs\n");
 fprintf(def_use_ptr,"Variaveis Usos Computacionais  = Vars C-Usos\n");
 fprintf(def_use_ptr,"Variaveis Usos Predicativos  = Vars P-Usos\n");
 fprintf(def_use_ptr,"Variaveis possivelmente definidas por Referencia = Vars Refs\n\n");
 */
 for(i=1;i>=1 && i <= num_no; ++i)
    {
     int j;
     fprintf(grafodef,"\n\n\nNO' %2d\n",i);
     fprintf(grafodef,"\nVars Defs : ");
     /*
     fprintf(def_use_ptr,"\n\n\nNO' %2d\n",i);
     fprintf(def_use_ptr,"\nVars Defs : ");
     */
     for(j=0; j>=0 && j <= get_name_counter(); ++j)
		if(test_bit(j,&(info_no[i].defg_i)))
		   {
                   print_names(grafodef,pvarname,j);
                   fprintf(grafodef," ");
                   /*
		   fprintf(def_use_ptr,"#%d ",j);
                   fprintf(def_use_ptr,"%ld %ld %ld\n",info_no[i].s_defg_i.vec_var_source[j].linha,info_no[i].s_defg_i.vec_var_source[j].inicio,info_no[i].s_defg_i.vec_var_source[j].comp);
                   print_names(def_use_ptr,pvarname,j);
		   fprintf(def_use_ptr,":");
		   v_source_print(&(info_no[i].s_defg_i), j,arqfonte,def_use_ptr);
                   fprintf(def_use_ptr,"\n");
                   */
                   }
     fprintf(grafodef,"\n");
     /*
     fprintf(def_use_ptr,"\n");
     */
     
     fprintf(grafodef,"\nVars Refs : ");
     /*
     fprintf(def_use_ptr,"\nVars Refs : ");
     */
     
     for(j=0; j>=0 && j <= get_name_counter(); ++j)
		if(test_bit(j,&(info_no[i].def_ref)))
		   {
		   print_names(grafodef,pvarname,j);
                   fprintf(grafodef," ");
     /*

		   fprintf(def_use_ptr,"#%d ",j);
                   fprintf(def_use_ptr,"%ld %ld %ld\n",info_no[i].s_def_ref.vec_var_source[j].linha,info_no[i].s_def_ref.vec_var_source[j].inicio,info_no[i].s_def_ref.vec_var_source[j].comp);
                   print_names(def_use_ptr,pvarname,j);
		   fprintf(def_use_ptr,":");
		   v_source_print(&(info_no[i].s_def_ref), j,arqfonte,def_use_ptr);
                   fprintf(def_use_ptr,"\n");
     */
                   }
     fprintf(grafodef,"\n");
     
     fprintf(grafodef,"\nVars C-Usos : ");
     for(j=0; j>=0 && j <= get_name_counter(); ++j)
		if(test_bit(j,&(info_no[i].c_use)))
		   {
		   print_names(grafodef,pvarname,j);
                   fprintf(grafodef," ");
                   /*
		   fprintf(def_use_ptr,"#%d ",j);
                   fprintf(def_use_ptr,"%ld %ld %ld\n",info_no[i].s_c_use.vec_var_source[j].linha,info_no[i].s_c_use.vec_var_source[j].inicio,info_no[i].s_c_use.vec_var_source[j].comp);
                   print_names(def_use_ptr,pvarname,j);
		   fprintf(def_use_ptr,":");
		   v_source_print(&(info_no[i].s_c_use), j,arqfonte,def_use_ptr);
                   fprintf(def_use_ptr,"\n");
                   */
                   }

     fprintf(grafodef,"\n");              
     /*
     fprintf(def_use_ptr,"\n");
     */
     if(!empty_bit(&(info_no[i].p_use)))
           {
             fprintf(grafodef,"\nVars P-Usos : \n");
             /*fprintf(def_use_ptr,"\nVars P-Usos : \n");*/

             for(node = graph[i].list_suc; node != NULL; node=node->proximo)
                {
		  fprintf(grafodef,"(%d,%d) ", i, node->num);
		  /*fprintf(def_use_ptr,"(%d,%d) ", i, node->num);*/
		}
                  
             fprintf(grafodef,": ");
             /*fprintf(def_use_ptr,": ");*/

             for(j=0; j>=0 && j <= get_name_counter(); ++j)
		if(test_bit(j,&(info_no[i].p_use)))
		  {                   
	          print_names(grafodef,pvarname,j);
                  fprintf(grafodef," ");

		   /*fprintf(def_use_ptr,"#%d ",j);
                   fprintf(def_use_ptr,"%ld %ld %ld\n",info_no[i].s_p_use.vec_var_source[j].linha,info_no[i].s_p_use.vec_var_source[j].inicio,info_no[i].s_p_use.vec_var_source[j].comp);
                   print_names(def_use_ptr,pvarname,j);
		   fprintf(def_use_ptr,":");
		   v_source_print(&(info_no[i].s_p_use), j,arqfonte,def_use_ptr);
                   fprintf(def_use_ptr,"\n");
                   */
                  }

             fprintf(grafodef,"\n");
             /*fprintf(def_use_ptr,"\n");*/
            }

    }


     
 fclose(grafodef); /* fecha o arquivo grafo def */
 /*fclose(def_use_ptr);*/
}

/*******************************************************************/
/* salva_tab_var_def(NODEINFO,char*)                               */
/* Autor: Marcos L. Chaim                                          */
/* Data: 26/12/94                                                  */
/* Versao: 1.0                                                     */
/*                                                                 */
/*                                                                 */
/* funcao: salva a tab_var_def da funcao em arquivo no seu         */
/* diretorio.                                                      */ 
/*                                                                 */
/*                                                                 */
/* Entradas: ponteiro para estrutura info_no e nome do diretorio.  */
/*                                                                 */
/* Saida: nenhuma.                                                 */
/*                                                                 */
/*******************************************************************/

void salva_tab_var_def(info_no,dir)
NODEINFO info_no;
char * dir;
{
FILE * tabvardef;
int i;
char filename[300];
DESPOINTER aux_names;


/* Cria diretorio para a funcao */

 cria_dir(dir);

/* Cria a tabela de variaveis definidas */

#ifdef TURBO_C
 strcpy(filename,".\\");
#else
 strcpy(filename,"./");
#endif

 strcat(filename,dir);

#ifdef TURBO_C
 strcat(filename,"\\");
#else
 strcat(filename,"/");
#endif
 strcat(filename,"tabvardef.tes");

 tabvardef = (FILE *) fopen(filename,"w");
 if(tabvardef == (FILE *) NULL)
    error("* * Erro Fatal: Nao consegui abrir o arquivo tabvardef.tes * *");

 fprintf(tabvardef,"# Tabela de Variaveis Definidas do Modulo %s\n\n", dir);

/* Imprime as variaveis e seus numeros de identificacao*/

 fprintf(tabvardef,"$ %d\n",get_name_counter());

 for(i=0; i < get_name_counter(); ++i)
	{	
	fprintf(tabvardef,"@ %d ", i);
        print_names(tabvardef,pvarname,i);
        fprintf(tabvardef,"\n");
	}

 fprintf(tabvardef,"@@\n");


/* Imprime Grafo Sintetico */

 fprintf(tabvardef,"\n# Grafo Def Sintetico do Modulo %s\n", dir);

 for(i=1;i>=1 && i <= num_no; ++i)
    {
     int j;
     fprintf(tabvardef,"\n@@ %2d",i);
     fprintf(tabvardef,"\nDefs: ");
     for(j=0; j>=0 && j < get_name_counter(); ++j)
	if(test_bit(j,&(info_no[i].defg_i)))
     			fprintf(tabvardef," %2d ",j);
     fprintf(tabvardef,"@");

     fprintf(tabvardef,"\nC-Uses: ");
     for(j=0; j>=0 && j < get_name_counter(); ++j)
	if(test_bit(j,&(info_no[i].c_use)))
     			fprintf(tabvardef," %2d ",j);
     fprintf(tabvardef,"@");
     
     fprintf(tabvardef,"\nP-Uses: ");
     for(j=0; j>=0 && j < get_name_counter(); ++j)
	if(test_bit(j,&(info_no[i].p_use)))
     			fprintf(tabvardef," %2d ",j);
     fprintf(tabvardef,"@");
     
     fprintf(tabvardef,"\nRefs: ");
     for(j=0; j>=0 && j < get_name_counter(); ++j)
		if(test_bit(j,&(info_no[i].def_ref)))
     			fprintf(tabvardef," %2d ",j);
     fprintf(tabvardef,"@");
     
     fprintf(tabvardef,"\nUndefs: ");
     for(j=0; j>=0 && j < get_name_counter(); ++j)
		if(!test_bit(j,&(info_no[i].undef)))
     			fprintf(tabvardef," %2d ",j);
     fprintf(tabvardef,"@");     
    }

 fclose(tabvardef); /* fecha o arquivo grafo def */
}

/*******************************************************************/
/* gerencia_fim_func(char *)                                       */
/* Autor: Marcos L. Chaim                                          */
/* Data: 26/12/94                                                  */
/* Versao: 1.0                                                     */
/*                                                                 */
/*                                                                 */
/* funcao: funcao que executa todas as atividades relativas ao fi- */
/* nal da analise sintatica.                                       */
/*                                                                 */
/* Entradas: ponteiro para nome da funcao.                         */
/*                                                                 */
/* Saida: nenhuma.                                                 */
/*                                                                 */
/*******************************************************************/

void gerencia_fim_func(nome)
char * nome;
{
 int i,j, found;

 if(only_glb_used)
   {  
   /* Elimina variaveis globais que nao sao usadas pelo menos uma vez */
   for(j=0; j>=0 && j <= get_name_counter(); ++j)
      if(test_bit(j,&(info_no[0].defg_i)))
        {
        found = FALSE;
        for(i=1;i>=1 && i <= num_no; ++i)
           if(test_bit(j,&(info_no[i].c_use)) ||
              test_bit(j,&(info_no[i].p_use)))
             {
             found = TRUE;
             break;
             }
        if(!found)
          reset_bit(j,&(info_no[1].defg_i)); 
        }
   }
 
 salva_grafo_def(info_no,nome);
 salva_tab_var_def(info_no,nome);
}

void elimina_variaveis_locais()
{
 DESPOINTER aux_names, prev_names;

 prev_names=names;
 aux_names=names; 
 while(aux_names != (DESPOINTER) NULL) 
	{
	if(aux_names->id < 100)
		{
		if(prev_names==aux_names)
			{
			names=prev_names=aux_names->next;
			free(aux_names);
			aux_names=prev_names;
			continue;
			}
		else
			{
			prev_names->next=aux_names->next;
			free(aux_names);
			aux_names=prev_names;
			}
		}
	else
		{
		prev_names=aux_names;
		}

	aux_names=aux_names->next;
	}
}
/*
**			Fim salvagrf.c
*/


