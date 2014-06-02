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

#include <process.h>
#include <conio.h>

#endif

#include "util.h"
#include "hparserg.h"
#include "hrotsem.h"
#include "haval.h"

AvalStr * AvalStr_cons(char * funcname)
{
  int i;

  AvalStr * p;

  p = (AvalStr *) malloc(sizeof(AvalStr));
			 
  if(p == NULL)
    error_msg(mem_fault);

  p->funcname = strdup(funcname);

  if(p->funcname == NULL)
    error_msg(mem_fault);

  p->pointer = (AUTOMATO *) NULL;
  p->no_output=FALSE;
 
  p->no_nos = p->no_vars = p->no_arcs = p->seq_exec = p->no_func_invoc = 0;

  for(i = 0; i <= MAXCRIT; ++i)
    {
      p->criterio[i]=0;
      p->tot_req_exec[i]=0;
    
      p->descritores[i]=NULL;  
      p->test_history[i]=NULL; 
      p->output[i]=NULL;       
      p->exec[i]=NULL;         
      p->requisitos[i]=NULL;   
      p->test_freq[i]=NULL;   
    }
  return p; 
}

void open_des_file(funcname, des, desname)
     char * funcname;
     FILE ** des;
     char * desname;
{

   char bigstr[2000];

   strcpy(bigstr,"./");
   strcat(bigstr, funcname);
   strcat(bigstr,"/");
   strcat(bigstr, desname);
      
   *des = (FILE *) fopen(bigstr,"r");
   if(*des == (FILE *) NULL)
     error_msg(cannot_open_file);

}
void close_des_file(des)
FILE ** des;
{
 if(*des != NULL)
   fclose(*des);
 *des=NULL;
}


void initiate_files(funcname,no_output,des,req,out,exec,freq,desname,reqname,outname,execname,freqname)
char * funcname;
int no_output;
FILE  **des, **req, **out, **exec, **freq;
char * desname, * reqname, *outname, * execname, *freqname;

{

   char bigstr[2000];

   strcpy(bigstr,"./");
   strcat(bigstr, funcname);
   strcat(bigstr,"/");
   strcat(bigstr, freqname);

   *freq = (FILE *) fopen(bigstr,"w");

   if(*freq == (FILE *) NULL)
       error_msg(cannot_open_file);

   strcpy(bigstr,"./");
   strcat(bigstr, funcname);
   strcat(bigstr,"/");
   strcat(bigstr, desname);
      
   *des = (FILE *) fopen(bigstr,"r");
   if(*des == (FILE *) NULL)
     error_msg(cannot_open_file);
       
   if(!no_output)
     {
       strcpy(bigstr,"./");
       strcat(bigstr, funcname);
       strcat(bigstr,"/");
       strcat(bigstr, reqname);

       *req = (FILE *) fopen(bigstr,"r");
       if(*req == (FILE *) NULL)
	 error_msg(cannot_open_file);
   
       strcpy(bigstr,"./");
       strcat(bigstr, funcname);
       strcat(bigstr,"/");
       strcat(bigstr, outname);
   
       *out = (FILE *) fopen(bigstr,"w");
       if(*out == (FILE *) NULL)
	 error_msg(cannot_open_file);

       strcpy(bigstr,"./");
       strcat(bigstr, funcname);
       strcat(bigstr,"/");
       strcat(bigstr, execname);

       *exec = (FILE *) fopen(bigstr,"w");
       if(*exec == (FILE *) NULL)
	 error_msg(cannot_open_file);
     }
}

int get_number_features(funcname,filename)
char * funcname;
char * filename;
{
  FILE * desc;
  char linha[1000];
  char bigstr[2000];
  int no_req;
  
  strcpy(bigstr,"./");
  strcat(bigstr, funcname);
  strcat(bigstr,"/");
  strcat(bigstr,filename);

  desc = fopen(bigstr,"r");
  
  if(desc == NULL)
    error_msg(cannot_open_file);

 /*
   position in the end of the descriptor file less a certain quantity of bytes  */
 
  fseek(desc, (long) -50, SEEK_END);
 
  /* read number of requeriments */

  le_linha_str(linha,"Numero Total de Descritores =", desc);

  sscanf(linha,"Numero Total de Descritores = %d", &no_req);

  fclose(desc);
  return(no_req);
}
 
void AvalStr_init(pfunc,argc,argv)
AvalStr *pfunc;
int argc;
char * argv[];
{

/* Declaracao de Variaveis Locais */

int i,j,no_arcs;
AUTOMATO * pointer_aux = (AUTOMATO *) NULL, * pointer_aux2 ;

 if(argc < 2)
   error_msg(wrong_number_pars);

 /* Ajusta os criterios a serem testados */

 for(i=1, j=1; i < argc && i < MAXCRIT+1; i++)
   {
     switch(*argv[i])
       {
       case 'n':
       case 'N':
         pfunc->no_output = TRUE;
         ++j;
         break;

       case 'y':
       case 'Y':
         pfunc->no_output = FALSE;
         ++j;
         break;

       case '1':
         if(get_number_features(pfunc->funcname,"des_pdu.tes"))
           pfunc->criterio[j]=PDU;
         ++j;
         break;

       case '2':
         if(get_number_features(pfunc->funcname,"des_pu.tes"))
           pfunc->criterio[j]=PU;
         ++j;
         break;

       case '3':
         if(get_number_features(pfunc->funcname,"des_pudu.tes"))
           pfunc->criterio[j]=PUDU;
         ++j;
         break;

       case '4':
         pfunc->no_arcs=get_number_features(pfunc->funcname,"des_arc.tes");
         if(pfunc->no_arcs > 1) /* E' necessario pelo menos dois arcos */
           pfunc->criterio[j]=ARCS;
         ++j;
         break;

       case '5':
         if(get_number_features(pfunc->funcname,"des_nos.tes"))
           pfunc->criterio[j]=NOS;
         ++j;
         break;

       case '6':
         if(get_number_features(pfunc->funcname,"des_p_uses.tes"))
           pfunc->criterio[j]=PUSOS;
         ++j;
         break;

       case '7':
         if(get_number_features(pfunc->funcname,"des_all_uses.tes"))
           pfunc->criterio[j]=USOS;
         ++j;
         break;

       case '8':
         if(get_number_features(pfunc->funcname,"des_all_du_paths.tes"))
           pfunc->criterio[j]=DU;
         ++j;
         break;

       default:
         error_msg(invalid_criterion);         
       }
   }
 
 pfunc->pointer = NULL;

 /* Inicializa automatos */
 
 for(i=1; i <= MAXCRIT; ++i)
   {
    
    switch(pfunc->criterio[i])
      {
      case PDU:
	
      open_des_file(pfunc->funcname,&pfunc->descritores[pfunc->criterio[i]],"des_pdu.tes");
      
      pdu_init_aval(&pointer_aux,&(pfunc->N[pfunc->criterio[i]]),pfunc->descritores[pfunc->criterio[i]]);

      close_des_file(&pfunc->descritores[pfunc->criterio[i]]);
      
      break;

      case PU:

      /* abre os arquivos relativos ao criterio todos-pot-usos */

      open_des_file(pfunc->funcname,&pfunc->descritores[pfunc->criterio[i]],"des_pu.tes");

      pu_init_aval(&pointer_aux,&pfunc->N[pfunc->criterio[i]],pfunc->criterio[i],pfunc->descritores[pfunc->criterio[i]]);

      close_des_file(&pfunc->descritores[pfunc->criterio[i]]);
      
      break;
       
      case PUDU:
	
      open_des_file(pfunc->funcname,&pfunc->descritores[pfunc->criterio[i]],"des_pudu.tes");

      pu_init_aval(&pointer_aux,&pfunc->N[pfunc->criterio[i]],pfunc->criterio[i],pfunc->descritores[pfunc->criterio[i]]);

      close_des_file(&pfunc->descritores[pfunc->criterio[i]]);

      break;

      case USOS:
	
      open_des_file(pfunc->funcname,&pfunc->descritores[pfunc->criterio[i]],"des_all_uses.tes");
   
      df_init_aval(&pointer_aux,&pfunc->N[pfunc->criterio[i]],pfunc->criterio[i],pfunc->descritores[pfunc->criterio[i]]);

      close_des_file(&pfunc->descritores[pfunc->criterio[i]]);

      break;
   
      case PUSOS:
	
      open_des_file(pfunc->funcname,&pfunc->descritores[pfunc->criterio[i]],"des_p_uses.tes");
   
      df_init_aval(&pointer_aux,&pfunc->N[pfunc->criterio[i]],pfunc->criterio[i],pfunc->descritores[pfunc->criterio[i]]);

      close_des_file(&pfunc->descritores[pfunc->criterio[i]]);

      break;
   
      case DU:

      open_des_file(pfunc->funcname,&pfunc->descritores[pfunc->criterio[i]],"des_all_du_paths.tes");
   
      df_init_aval(&pointer_aux,&pfunc->N[pfunc->criterio[i]],pfunc->criterio[i],pfunc->descritores[pfunc->criterio[i]]);

      close_des_file(&pfunc->descritores[pfunc->criterio[i]]);

      break;

      case ARCS:
	
      open_des_file(pfunc->funcname,&pfunc->descritores[pfunc->criterio[i]],"des_arc.tes");

      no_arc_init_aval(&pointer_aux,&pfunc->N[pfunc->criterio[i]],pfunc->criterio[i],pfunc->descritores[pfunc->criterio[i]]);

      close_des_file(&pfunc->descritores[pfunc->criterio[i]]);
      break;

      case NOS:
	
      open_des_file(pfunc->funcname,&pfunc->descritores[pfunc->criterio[i]],"des_nos.tes");

      no_arc_init_aval(&pointer_aux,&pfunc->N[pfunc->criterio[i]],pfunc->criterio[i],pfunc->descritores[pfunc->criterio[i]]);

      close_des_file(&pfunc->descritores[pfunc->criterio[i]]);

      break;
 
      default:
		break;
 
      }
    
    if(pfunc->criterio[i] != 0)
      { /* E' um criterio valido */
      if(pfunc->pointer != NULL)
        {
        for(pointer_aux2 = pfunc->pointer; pointer_aux2->next != (AUTOMATO *) NULL;pointer_aux2 = pointer_aux2->next);

        pointer_aux2->next = pointer_aux;
        }
      else
       pfunc-> pointer = pointer_aux;
      
      pointer_aux=NULL;
      }
   }
   
} /* End AvalStr_init */

void AvalStr_end(AvalStr *pfunc)
{

 AUTOMATO * current = (AUTOMATO *) NULL;
 int i, cur_criterio;
 char desc_path[1000];  /* contem a descricao do potencial du-caminho */
 char * ptr_path;       /* aponta para a descricao do requisito de teste */
 
 /* variaveis para o calculo das coberturas */

 int total_automatos[MAXCRIT+1];
 int ok_automatos[MAXCRIT+1];
 int no_nos_deff[MAXCRIT+1];
 float cober_grfi[MAXCRIT+1];
 int ok_aut_grfi[MAXCRIT+1];
 int tot_aut_grfi[MAXCRIT+1];
 int no_i;

 for(i=1; i <= MAXCRIT; ++i)
  {
   total_automatos[pfunc->criterio[i]] = ok_automatos[pfunc->criterio[i]] = 0;
   ok_aut_grfi[pfunc->criterio[i]] = tot_aut_grfi[pfunc->criterio[i]] = 0;
   no_nos_deff[pfunc->criterio[i]] = 1;
   cober_grfi[pfunc->criterio[i]] = 0.0;

   switch(pfunc->criterio[i])
   {
   case PDU:

     /* abre os arquivos relativos ao criterio todos-pot-du-caminhos */

     initiate_files(pfunc->funcname,pfunc->no_output,&(pfunc->descritores[pfunc->criterio[i]]),&(pfunc->requisitos[pfunc->criterio[i]]),&(pfunc->output[pfunc->criterio[i]]),&(pfunc->exec[pfunc->criterio[i]]),&(pfunc->test_freq[pfunc->criterio[i]]),"des_pdu.tes","pdupaths.tes","pduoutput.tes","exec_pdu.tes","pdufreq.tes");
      
    if(!pfunc->no_output)
      {    
      fprintf(pfunc->output[pfunc->criterio[i]],"\n\nPOTENCIAIS-DU-CAMINHOS que nao foram executados:\n");
      fprintf(pfunc->output[pfunc->criterio[i]],"\nCaminhos:\n");
      fprintf(pfunc->exec[pfunc->criterio[i]],"\n\nPOTENCIAIS-DU-CAMINHOS que foram executados:\n");
      fprintf(pfunc->exec[pfunc->criterio[i]],"\nCaminhos:\n");
      }
    
    break;

   case PU:

     /* abre os arquivos relativos ao criterio todos-pot-usos */

     initiate_files(pfunc->funcname,pfunc->no_output,&pfunc->descritores[pfunc->criterio[i]],&pfunc->requisitos[pfunc->criterio[i]],&pfunc->output[pfunc->criterio[i]],&pfunc->exec[pfunc->criterio[i]],&pfunc->test_freq[pfunc->criterio[i]],"des_pu.tes","puassoc.tes","puoutput.tes","exec_pu.tes","pufreq.tes");      

    if(!pfunc->no_output)
      {    
      fprintf(pfunc->output[pfunc->criterio[i]],"\n\nASSOCIACOES DO CRITERIO TODOS POT-USOS nao executadas:\n\n");
      fprintf(pfunc->exec[pfunc->criterio[i]],"\n\nASSOCIACOES DO CRITERIO TODOS POT-USOS executadas:\n\n");
      }

    break;

   case PUDU:

    /* abre os arquivos relativos ao criterio todos-pot-usos */
   
    initiate_files(pfunc->funcname,pfunc->no_output,&pfunc->descritores[pfunc->criterio[i]],&pfunc->requisitos[pfunc->criterio[i]],&pfunc->output[pfunc->criterio[i]],&pfunc->exec[pfunc->criterio[i]],&pfunc->test_freq[pfunc->criterio[i]],"des_pudu.tes","puassoc.tes","puduoutput.tes","exec_pudu.tes","pudufreq.tes");
   

    if(!pfunc->no_output)
      {    
      fprintf(pfunc->output[pfunc->criterio[i]],"\n\nASSOCIACOES DO CRITERIO TODOS POT-USOS/DU nao executadas:\n\n");
      fprintf(pfunc->exec[pfunc->criterio[i]],"\n\nASSOCIACOES DO CRITERIO TODOS POT-USOS/DU executadas:\n\n");
      }

     break;
     
   case NOS:
      /* abre os arquivos relativos ao criterio todos-arcos */

    initiate_files(pfunc->funcname,pfunc->no_output,&pfunc->descritores[pfunc->criterio[i]],&pfunc->requisitos[pfunc->criterio[i]],&pfunc->output[pfunc->criterio[i]],&pfunc->exec[pfunc->criterio[i]],&pfunc->test_freq[pfunc->criterio[i]],"des_nos.tes","nos_grf.tes","nosoutput.tes","exec_nos.tes","nosfreq.tes");

    if(!pfunc->no_output)
      {    
      fprintf(pfunc->output[pfunc->criterio[i]],"\n\nNOS DO CRITERIO TODOS-NOS nao executados:\n\n");
      fprintf(pfunc->exec[pfunc->criterio[i]],"\n\nNOS DO CRITERIO TODOS-NOS executados:\n\n");   
      }

     break;

   case ARCS:

     /* abre os arquivos relativos ao criterio todos-arcos */

    initiate_files(pfunc->funcname,pfunc->no_output,&pfunc->descritores[pfunc->criterio[i]],&pfunc->requisitos[pfunc->criterio[i]],&pfunc->output[pfunc->criterio[i]],&pfunc->exec[pfunc->criterio[i]],&pfunc->test_freq[pfunc->criterio[i]],"des_arc.tes","arcprim.tes","arcoutput.tes","exec_arc.tes","arcfreq.tes");

    if(!pfunc->no_output)
      {    
      fprintf(pfunc->output[pfunc->criterio[i]],"\n\nARCOS DO CRITERIO TODOS-ARCOS nao executados:\n\n");
      fprintf(pfunc->exec[pfunc->criterio[i]],"\n\nARCOS DO CRITERIO TODOS-ARCOS executados:\n\n");
      }

     break;
    
   case USOS:

     /* abre os arquivos relativos ao criterio todos-usos */
   
     initiate_files(pfunc->funcname,pfunc->no_output,&pfunc->descritores[pfunc->criterio[i]],&pfunc->requisitos[pfunc->criterio[i]],&pfunc->output[pfunc->criterio[i]],&pfunc->exec[pfunc->criterio[i]],&pfunc->test_freq[pfunc->criterio[i]],"des_all_uses.tes","all_uses.tes","usesoutput.tes","exec_uses.tes","usesfreq.tes");
     
    if(!pfunc->no_output)
      {    
      fprintf(pfunc->output[pfunc->criterio[i]],"\n\nASSOCIACOES DO CRITERIO TODOS-USOS nao executadas:\n\n");
      fprintf(pfunc->exec[pfunc->criterio[i]],"\n\nASSOCIACOES DO CRITERIO TODOS-USOS executadas:\n\n");
      }

     break;

   case PUSOS:

     /* abre os arquivos relativos ao criterio todos-p-usos */
   
     initiate_files(pfunc->funcname,pfunc->no_output,&pfunc->descritores[pfunc->criterio[i]],&pfunc->requisitos[pfunc->criterio[i]],&pfunc->output[pfunc->criterio[i]],&pfunc->exec[pfunc->criterio[i]],&pfunc->test_freq[pfunc->criterio[i]],"des_p_uses.tes","all_p_uses.tes","pusesoutput.tes","exec_puses.tes","pusesfreq.tes");
      
    if(!pfunc->no_output)
      {    
      fprintf(pfunc->output[pfunc->criterio[i]],"\n\nASSOCIACOES DO CRITERIO TODOS-USOS PREDICATIVOS nao executadas:\n\n");
      fprintf(pfunc->exec[pfunc->criterio[i]],"\n\nASSOCIACOES DO CRITERIO TODOS-USOS PREDICATIVOS executadas:\n\n");
      }

     break;
     
   case DU:

     /* abre os arquivos relativos ao criterio todos-du-caminhos */
   
     initiate_files(pfunc->funcname,pfunc->no_output,&pfunc->descritores[pfunc->criterio[i]],&pfunc->requisitos[pfunc->criterio[i]],&pfunc->output[pfunc->criterio[i]],&pfunc->exec[pfunc->criterio[i]],&pfunc->test_freq[pfunc->criterio[i]],"des_all_du_paths.tes","all_du_paths.tes","dupathsoutput.tes","exec_dupaths.tes","dupathsfreq.tes");
      
    if(!pfunc->no_output)
      {    
      fprintf(pfunc->output[pfunc->criterio[i]],"\n\nDU-CAMINHOS que nao foram executados:\n");
      fprintf(pfunc->output[pfunc->criterio[i]],"\nCaminhos:\n");
      fprintf(pfunc->exec[pfunc->criterio[i]],"\n\nDU-CAMINHOS que foram executados:\n");
      fprintf(pfunc->exec[pfunc->criterio[i]],"\nCaminhos:\n");
      }

    break;

   default:
	break;
   }
  }
 
 if(pfunc->pointer != NULL)
   for(current = pfunc->pointer, no_i = current->i, cur_criterio=current->criterio;
     current != (AUTOMATO *) NULL;
     cur_criterio=current->criterio, current = current->next)
   {
   
   switch(current->criterio)
     {
     case PU:
     case PUDU:
     case PDU:

       if(cur_criterio != current->criterio)
         no_i=current->i;
     
       /* Calcula resultados */
 
       total_automatos[current->criterio]++;

       if(no_i == current->i)
          tot_aut_grfi[current->criterio]++;
       else
         {
          no_i = current->i; /* muda o no' i e incrementa o numero de grafo(i) */
          no_nos_deff[current->criterio]++;

          /* ajusta a cobertura dos grafo(i) */

          cober_grfi[current->criterio] += (float) ok_aut_grfi[current->criterio]/tot_aut_grfi[current->criterio];

          /* reinicia os contadores de cobertura parcial */

          tot_aut_grfi[current->criterio] = 1;
          ok_aut_grfi[current->criterio] = 0;
         }

       if(current->estado != Q3)
          {
          if(!pfunc->no_output)
	    {
            ptr_path = pega_path(current->n_path,pfunc->requisitos[current->criterio]);
            fprintf(pfunc->output[current->criterio],"%s",apont_next_tok(ptr_path));
            free(ptr_path);
	    }
          }
       else
         {
          ok_automatos[current->criterio]++;
          ok_aut_grfi[current->criterio]++;
          fprintf(pfunc->test_freq[current->criterio],"%d (%d/%d) (%d %d/%d)\n",current->n_path,current->freq,pfunc->tot_req_exec[current->criterio],current->no_func_invoc,current->seq_exec_i,current->seq_exec_j);
	  
          if(!pfunc->no_output)
	    {
            ptr_path = pega_path(current->n_path,pfunc->requisitos[current->criterio]);
            fprintf(pfunc->exec[current->criterio],"%s",apont_next_tok(ptr_path));
            free(ptr_path);
	    }
         }
       

     /* ajusta a cobertura dos grafo(i) 

       cober_grfi[current->criterio] += (float) ok_aut_grfi[current->criterio]/tot_aut_grfi[current->criterio];
*/
    break;
     

   case ARCS:      
   case USOS:
   case PUSOS:
   case DU:
     /* Calcula resultados */
 
        total_automatos[current->criterio]++;

        if(current->estado != Q3)
	  {
          if(!pfunc->no_output)
	    {
            ptr_path = pega_path(current->n_path,pfunc->requisitos[current->criterio]);
            fprintf(pfunc->output[current->criterio],"%s",apont_next_tok(ptr_path));
            free(ptr_path);
            }
          }
        else
          {
           ok_automatos[current->criterio]++;
           fprintf(pfunc->test_freq[current->criterio],"%d (%d/%d) (%d %d/%d)\n",current->n_path,current->freq,pfunc->tot_req_exec[current->criterio],current->no_func_invoc,current->seq_exec_i,current->seq_exec_j);
	   
          if(!pfunc->no_output)
	    {
            ptr_path = pega_path(current->n_path,pfunc->requisitos[current->criterio]);
	    fprintf(pfunc->exec[current->criterio],"%s",apont_next_tok(ptr_path));
            free(ptr_path);
            }
         }
       
    break;
   
   case NOS:     

        total_automatos[current->criterio]++;

	if(current->estado != Q3)
	  {
          if(!pfunc->no_output)
            {
	    fprintf(pfunc->output[current->criterio],"%3d ",current->n_path);
	    if(!((total_automatos[current->criterio]-ok_automatos[current->criterio])%10))
		fprintf(pfunc->output[current->criterio],"\n");
            }
	  }
        else
          {
           ok_automatos[current->criterio]++;
           fprintf(pfunc->test_freq[current->criterio],"%d (%d %d/%d) (%d)\n",current->n_path,current->freq,pfunc->tot_req_exec[current->criterio],current->no_func_invoc,current->seq_exec_i);
           
            if(!pfunc->no_output)
             {
	     fprintf(pfunc->exec[current->criterio],"%3d ",current->n_path);
	     if(!(ok_automatos[current->criterio]%10))
		fprintf(pfunc->exec[current->criterio],"\n");
	     }
	   }
 
    break;

   default:
     error_msg(wrong_number_pars);
   }

   }
 
 for(i=1; i <= MAXCRIT; ++i)
   {
   switch(pfunc->criterio[i])
     {
     case PU:
     case PUDU:
     case PDU:
       
     if(!pfunc->no_output)
       {
       fprintf(pfunc->output[pfunc->criterio[i]],"\nCobertura Total = %f\n",
             100.*ok_automatos[pfunc->criterio[i]]/total_automatos[pfunc->criterio[i]]);
       fprintf(pfunc->exec[pfunc->criterio[i]],"\nCobertura Total = %f\n",
             100.*ok_automatos[pfunc->criterio[i]]/total_automatos[pfunc->criterio[i]]);
       fprintf(pfunc->output[pfunc->criterio[i]],"\nMedia da Cobertura dos Grafo(i) = %f\n",
             100.*cober_grfi[pfunc->criterio[i]]/no_nos_deff[pfunc->criterio[i]]);
       fprintf(pfunc->exec[pfunc->criterio[i]],"\nMedia da Cobertura dos Grafo(i) = %f\n",
             100.*cober_grfi[pfunc->criterio[i]]/no_nos_deff[pfunc->criterio[i]]);
       }

     break;

   case NOS:
   case ARCS:      
   case USOS:
   case PUSOS:
   case DU:

     if(!pfunc->no_output)
       {
       fprintf(pfunc->output[pfunc->criterio[i]],"\n\nCobertura Total = %f\n",
            100.*ok_automatos[pfunc->criterio[i]]/total_automatos[pfunc->criterio[i]]);
       fprintf(pfunc->exec[pfunc->criterio[i]],"\n\nCobertura Total = %f\n",
            100.*ok_automatos[pfunc->criterio[i]]/total_automatos[pfunc->criterio[i]]);
       }
     
     break;
   default:
	break;
   }
 }

 for(i=1; i<=MAXCRIT; ++i)
   {
   /* Destroi vetor de bits */
 
   if(pfunc->criterio[i] != 0 )
     {
     b_vector_destroy(&pfunc->N[pfunc->criterio[i]]);
     
     if(pfunc->descritores[pfunc->criterio[i]] != NULL)
	 fclose(pfunc->descritores[pfunc->criterio[i]]);
     
     if(pfunc->exec[pfunc->criterio[i]] != NULL)
	 fclose(pfunc->exec[pfunc->criterio[i]]);
     
     if(pfunc->output[pfunc->criterio[i]] != NULL)
         fclose(pfunc->output[pfunc->criterio[i]]);
     
     if(pfunc->requisitos[pfunc->criterio[i]] != NULL)
	 fclose(pfunc->requisitos[pfunc->criterio[i]]);

     if(pfunc->test_freq[pfunc->criterio[i]] != NULL)
	 fclose(pfunc->test_freq[pfunc->criterio[i]]);
     }
   }
 
}

AvalStr_destroy(AvalStr * pfunc)
{
 int i;
 if(pfunc->funcname !=NULL)
    free(pfunc->funcname);

 for(i=1; i<=MAXCRIT; ++i)
   /* Destroi vetor de bits */
   if(pfunc->criterio[i] != 0 )
     b_vector_destroy(&pfunc->N[pfunc->criterio[i]]);

 /* libera espaco ocupado pelos automados */


 if(pfunc->pointer != (AUTOMATO *) NULL)
   {
    lib_automato(pfunc->pointer->next);
    b_vector_destroy(&(pfunc->pointer->Nnv));
    b_vector_destroy(&(pfunc->pointer->Ni));
    free(pfunc->pointer->exp_regular);
    free(pfunc->pointer);
   }
}


/*
 lib_automato(); "desaloca o espaco ocupado por um automato"

*/

void lib_automato(aut)
AUTOMATO * aut;
{

AUTOMATO * aux, * an_aux;

aux = aut;

while(aux != (AUTOMATO *) NULL)
   {
    an_aux = aux;
    aux = aux->next;
    b_vector_destroy(&(an_aux->Nnv));
    b_vector_destroy(&(an_aux->Ni));
    free(an_aux->exp_regular);
    free(an_aux);
   }
}









