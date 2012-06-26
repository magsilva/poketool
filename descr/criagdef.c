#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"
#include "hrotsem.h"

extern int no_vars;
extern int no_nos;
extern NODEINFO info_no;

void cria_grafo_def(dir)
char * dir;
{
 /* Declaracao de Variaveis Locais */

 FILE * tabvardef;
 int i,id,no,set;
 int final_tab_var=FALSE;
 char filename[300];
 char token[300];

 /* Aloca espaco para a Estrutura de Dados info_no */

 info_no = (NODEINFO)  malloc( (no_nos+1) * sizeof(INFOCELL));
 if(info_no == (NODEINFO) NULL)
	error("* * Erro Fatal: Nao consegui alocar espaco para info_no * *\n");

 /* Abre arquivo com tabela de variaveis definidas */
 
 strcpy(filename,dir);
#ifdef TURBO_C
 strcat(filename,"\\");
#else
 strcat(filename,"/");
#endif
 strcat(filename,"tabvardef.tes");

 tabvardef = (FILE *) fopen(filename,"r");
 if(tabvardef == (FILE *) NULL)
    error("* * Erro Fatal: Nao consegui abrir o arquivo tabvardef.tes * *\n");
 
 

 /*  while(!feof(tabvardef)) 
 	{
        int par;
        
	par = fscanf(tabvardef,"%s",token);
 */
     while((fscanf(tabvardef,"%s",token)) != EOF)
        {
        /* cria tab_var_def */

	if(!final_tab_var)
	   {
	    switch(token[0])
	       {
                case '$': fscanf(tabvardef,"%d",&no_vars);
                          inicia_glb();
                          break;
                          
	        case '#': /* comentario */
	    	          while(fgetc(tabvardef)!='\n');
	    	          break;
                          
	        case '@': /* informacao */
	    	          if(strcmp(token,"@@"))
           	            {
	    		     fscanf(tabvardef,"%d",&id);
	    		     fscanf(tabvardef,"%s",token);
			     insert_name(token, id);
           	            }
	    	          else /* token = @@ */
				final_tab_var = TRUE; /* fim da tab_var_def */
			 break;
                default:
    		 	 error("* * Erro Fatal: Nao consegui abrir o arquivo tabvardef.tes * *\n");
               }

	   }
 	else /* cria grafo def */
	   {
	    switch(token[0])
	       {
	        case '#': /* comentario */
		          while(fgetc(tabvardef)!='\n');
		          break;
	        case '@': /* informacao */
		          if(!strcmp(token,"@@"))
           	            {
			     fscanf(tabvardef,"%d",&no);
           	            }
		          else /* token = @@ */
    		 	     error("* * Erro Fatal: Nao consegui abrir o arquivo tabvardef.tes * *\n");
			  break;
                default:  /* informacao */
			    if(!strcmp(token,"Defs:"))
                              set=1;
			    else
			       if(!strcmp(token,"Refs:"))
	                         set=2;
                               else
			         if(!strcmp(token,"Undefs:"))
                                   set=3;
                                 else
			           if(!strcmp(token,"C-Uses:"))
                                     set=4;
                                   else
			             if(!strcmp(token,"P-Uses:"))
                                       set=5;
                                     else
                                       set=0;
                            
	                    switch(set)
                              {
                              case 1:
			 	fscanf(tabvardef,"%s",token);
				 while(strcmp(token,"@"))
				   {
				   id = atoi(token);
				   set_bit(id,&(info_no[no].defg_i));
			 	   fscanf(tabvardef,"%s",token);
				   }
                                 break;
                              case 2:
                                fscanf(tabvardef,"%s",token);
			        while(strcmp(token,"@"))
				  {
				  id = atoi(token);
				  set_bit(id,&(info_no[no].def_ref));
			 	  fscanf(tabvardef,"%s",token);
				  }
                                break;
                              case 3:
			 	fscanf(tabvardef,"%s",token);
				 while(strcmp(token,"@"))
				   {
				   id = atoi(token);
				   reset_bit(id,&(info_no[no].undef));
			 	   fscanf(tabvardef,"%s",token);
				   }
                                 break;
                              case 4:
                                fscanf(tabvardef,"%s",token);
			        while(strcmp(token,"@"))
				  {
				  id = atoi(token);
				  set_bit(id,&(info_no[no].c_use));
			 	  fscanf(tabvardef,"%s",token);
				  }
                                break;
                              case 5:
                                fscanf(tabvardef,"%s",token);
			        while(strcmp(token,"@"))
				  {
				  id = atoi(token);
				  set_bit(id,&(info_no[no].p_use));
			 	  fscanf(tabvardef,"%s",token);
				  }
                                break;
                                
                              default:                                 
    		                error("* * Erro Fatal: arquivo tabvardef.tes gerado incorretamente * *\n");
                              }
		           break;

               }
	    }
	}

}

/*
**				Fim de criagdef.c
*/

















