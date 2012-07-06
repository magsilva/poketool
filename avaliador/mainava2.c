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

#ifdef TURBO_C

#include <process.h>
#include <conio.h>

#endif

#include "util.h"
#include "hparserg.h"
#include "hrotsem.h"
#include "haval.h"

/* Declaracao de Variaveis Globais */

AUTOMATO * pointer = (AUTOMATO *) NULL;
int no_nos, no_vars, tot_req_exec[MAXCRIT+1];
int no_output=FALSE;
int criterio[MAXCRIT+1];

FILE * descritores[MAXCRIT+1];  /* aponta para o arquivo de descritores do criterio */
FILE * test_history[MAXCRIT+1]; /* aponta para o arquivo de historia da avaliacao */
FILE * output[MAXCRIT+1];       /* aponta para o arquivo de associacoes nao executadas */
FILE * exec[MAXCRIT+1];         /* aponta para o arquivo de associacoes executadas */
FILE * requisitos[MAXCRIT+1];   /* aponta para a descricao dos requisitos de teste */
FILE * test_freq[MAXCRIT+1];    /* aponta para a frequencia dos requisitos de teste */

void initiate_files(criterio,hist,des,req,out,exec,freq,hisname,desname,
                    reqname,outname,execname,freqname)
int criterio;
FILE **hist, **des, **req, **out, **exec, **freq;
char  * hisname, * desname, * reqname, *outname, * execname, *freqname;

{
   *hist = (FILE *) fopen(hisname,"r");

   /* *his nao e' testado porque pode nao existir */

   *freq = (FILE *) fopen(freqname,"w");

   if(*freq == (FILE *) NULL)
       error("* * Erro Fatal: Nao consegui abrir o arquivo de requisitos \
executados * *");
      
   *des = (FILE *) fopen(desname,"r");
   if(*des == (FILE *) NULL)
       error("* * Erro Fatal: Nao consegui abrir o arquivo de descritores \
* *");

   *req = (FILE *) fopen(reqname,"r");
   if(*req == (FILE *) NULL)
     error("* * Erro Fatal: Nao consegui abrir o arquivo de requisitos de \
teste * *");
   
   *out = (FILE *) fopen(outname,"w");
   if(*out == (FILE *) NULL)
       error("* * Erro Fatal: Nao consegui abrir o arquivo de requisitos \
nao-executados * *");

   *exec = (FILE *) fopen(execname,"w");
   if(*exec == (FILE *) NULL)
       error("* * Erro Fatal: Nao consegui abrir o arquivo de requisitos \
executados * *");
  
}

void print_output(inicio,criterio)
AUTOMATO * inicio;
int criterio[];
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


 if(inicio == NULL)
   {
     fprintf(stderr, "* * Erro: Nao existem descritores para avaliacao dos criterios * *\n");
   exit(1);
   }

 for(i=1; i <= MAXCRIT; ++i)
  {
   total_automatos[criterio[i]] = ok_automatos[criterio[i]] = 0;
   ok_aut_grfi[criterio[i]] = tot_aut_grfi[criterio[i]] = 0;
   no_nos_deff[criterio[i]] = 1;
   cober_grfi[criterio[i]] = 0.0;

   switch(criterio[i])
   {
   case PDU:

    if(!no_output)
      {    
      fprintf(output[criterio[i]],"\n\nPOTENCIAIS-DU-CAMINHOS que nao foram executados:\n");
      fprintf(output[criterio[i]],"\nCaminhos:\n");
      fprintf(exec[criterio[i]],"\n\nPOTENCIAIS-DU-CAMINHOS que foram executados:\n");
      fprintf(exec[criterio[i]],"\nCaminhos:\n");
      }
    
    test_history[criterio[i]] = (FILE *) fopen("pduhis.tes","w");
    if(test_history[criterio[i]] == (FILE *) NULL)
      error("* * Erro Fatal: Nao consegui abrir o arquivo com o historico * *"); 

    break;

   case PU:

    if(!no_output)
      {    
      fprintf(output[criterio[i]],"\n\nASSOCIACOES DO CRITERIO TODOS POT-USOS nao executadas:\n\n");
      fprintf(exec[criterio[i]],"\n\nASSOCIACOES DO CRITERIO TODOS POT-USOS executadas:\n\n");
      }

    test_history[criterio[i]] = (FILE *) fopen("puhis.tes","w");
    if(test_history[criterio[i]] == (FILE *) NULL)
      error("* * Erro Fatal: Nao consegui abrir o arquivo com o historico * *"); 

    break;

   case PUDU:

    if(!no_output)
      {    
      fprintf(output[criterio[i]],"\n\nASSOCIACOES DO CRITERIO TODOS POT-USOS/DU nao executadas:\n\n");
      fprintf(exec[criterio[i]],"\n\nASSOCIACOES DO CRITERIO TODOS POT-USOS/DU executadas:\n\n");
      }
    test_history[criterio[i]] = (FILE *) fopen("puduhis.tes","w");
    if(test_history[criterio[i]] == (FILE *) NULL)
      error("* * Erro Fatal: Nao consegui abrir o arquivo com o historico * *"); 

     break;
     
   case NOS:

    if(!no_output)
      {    
      fprintf(output[criterio[i]],"\n\nNOS DO CRITERIO TODOS-NOS nao executados:\n\n");
      fprintf(exec[criterio[i]],"\n\nNOS DO CRITERIO TODOS-NOS executados:\n\n");   
      }

    test_history[criterio[i]] = (FILE *) fopen("noshis.tes","w");
    if(test_history[criterio[i]] == (FILE *) NULL)
      error("* * Erro Fatal: Nao consegui abrir o arquivo com o historico * *"); 

     break;

   case ARCS:

    if(!no_output)
      {    
      fprintf(output[criterio[i]],"\n\nARCOS DO CRITERIO TODOS-ARCOS nao executados:\n\n");
      fprintf(exec[criterio[i]],"\n\nARCOS DO CRITERIO TODOS-ARCOS executados:\n\n");
      }

    test_history[criterio[i]] = (FILE *) fopen("archis.tes","w");
    if(test_history[criterio[i]] == (FILE *) NULL)
      error("* * Erro Fatal: Nao consegui abrir o arquivo com o historico * *"); 

     break;
    
   case USOS:

    if(!no_output)
      {    
      fprintf(output[criterio[i]],"\n\nASSOCIACOES DO CRITERIO TODOS-USOS nao executadas:\n\n");
      fprintf(exec[criterio[i]],"\n\nASSOCIACOES DO CRITERIO TODOS-USOS executadas:\n\n");
      }

    test_history[criterio[i]] = (FILE *) fopen("useshis.tes","w");
    if(test_history[criterio[i]] == (FILE *) NULL)
      error("* * Erro Fatal: Nao consegui abrir o arquivo com o historico * *"); 

     break;

   case PUSOS:

    if(!no_output)
      {    
      fprintf(output[criterio[i]],"\n\nASSOCIACOES DO CRITERIO TODOS-USOS PREDICATIVOS nao executadas:\n\n");
      fprintf(exec[criterio[i]],"\n\nASSOCIACOES DO CRITERIO TODOS-USOS PREDICATIVOS executadas:\n\n");
      }
    
    test_history[criterio[i]] = (FILE *) fopen("puseshis.tes","w");
    if(test_history[criterio[i]] == (FILE *) NULL)
      error("* * Erro Fatal: Nao consegui abrir o arquivo com o historico * *"); 

     break;
     
   case DU:
     
    if(!no_output)
      {    
      fprintf(output[criterio[i]],"\n\nDU-CAMINHOS que nao foram executados:\n");
      fprintf(output[criterio[i]],"\nCaminhos:\n");
      fprintf(exec[criterio[i]],"\n\nDU-CAMINHOS que foram executados:\n");
      fprintf(exec[criterio[i]],"\nCaminhos:\n");
      }

    test_history[criterio[i]] = (FILE *) fopen("dupathshis.tes","w");
    if(test_history[criterio[i]] == (FILE *) NULL)
      error("* * Erro Fatal: Nao consegui abrir o arquivo com o historico * *"); 

    break;

   default:;

   }
  }


 
 
 for(current = inicio, no_i = current->i, cur_criterio=current->criterio;
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
          if(!no_output)
	    {
            ptr_path = pega_path(current->n_path,requisitos[current->criterio]);
            fprintf(output[current->criterio],"%s",apont_next_tok(ptr_path));
            free(ptr_path);
	    }
          }
       else
         {
          ok_automatos[current->criterio]++;
          ok_aut_grfi[current->criterio]++;
          fprintf(test_history[current->criterio],"%d\n",current->n_path);
          fprintf(test_freq[current->criterio],"%d (%d/%d) (%d %d/%d)\n",current->n_path,current->freq,tot_req_exec[current->criterio],current->no_func_invoc,current->seq_exec_i,current->seq_exec_j);
          if(!no_output)
	    {
            ptr_path = pega_path(current->n_path,requisitos[current->criterio]);
            fprintf(exec[current->criterio],"%s",apont_next_tok(ptr_path));
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
          if(!no_output)
	    {
            ptr_path = pega_path(current->n_path,requisitos[current->criterio]);
            fprintf(output[current->criterio],"%s",apont_next_tok(ptr_path));
            free(ptr_path);
            }
          }
        else
          {
           ok_automatos[current->criterio]++;
           fprintf(test_history[current->criterio],"%d\n",current->n_path);
           fprintf(test_freq[current->criterio],"%d (%d/%d) (%d %d/%d)\n",current->n_path,current->freq,tot_req_exec[current->criterio],current->no_func_invoc,current->seq_exec_i,current->seq_exec_j);
          if(!no_output)
	    {
            ptr_path = pega_path(current->n_path,requisitos[current->criterio]);
	    fprintf(exec[current->criterio],"%s",apont_next_tok(ptr_path));
            free(ptr_path);
            }
         }
       
    break;
   
   case NOS:     

        total_automatos[current->criterio]++;

	if(current->estado != Q3)
	  {
          if(!no_output)
            {
	    fprintf(output[current->criterio],"%3d ",current->n_path);
	    if(!((total_automatos[current->criterio]-ok_automatos[current->criterio])%10))
		fprintf(output[current->criterio],"\n");
            }
	  }
        else
          {
           ok_automatos[current->criterio]++;
           fprintf(test_history[current->criterio],"%d\n",current->n_path);
           fprintf(test_freq[current->criterio],"%d (%d %d/%d) (%d)\n",current->n_path,current->freq,tot_req_exec[current->criterio],current->no_func_invoc,current->seq_exec_i);
           if(!no_output)
             {
	     fprintf(exec[current->criterio],"%3d ",current->n_path);
	     if(!(ok_automatos[current->criterio]%10))
		fprintf(exec[current->criterio],"\n");
	     }
	   }
       

 
    break;

   default:
     error("* * Erro Fatal: Parametro do Criterio errado no Avaliador * *");
   }

   }
 
 for(i=1; i <= MAXCRIT; ++i)
   {
   switch(criterio[i])
     {
     case PU:
     case PUDU:
     case PDU:
       
     if(!no_output)
       {
       fprintf(output[criterio[i]],"\nCobertura Total = %f\n",
             100.*ok_automatos[criterio[i]]/total_automatos[criterio[i]]);
       fprintf(exec[criterio[i]],"\nCobertura Total = %f\n",
             100.*ok_automatos[criterio[i]]/total_automatos[criterio[i]]);
       fprintf(output[criterio[i]],"\nMedia da Cobertura dos Grafo(i) = %f\n",
             100.*cober_grfi[criterio[i]]/no_nos_deff[criterio[i]]);
       fprintf(exec[criterio[i]],"\nMedia da Cobertura dos Grafo(i) = %f\n",
             100.*cober_grfi[criterio[i]]/no_nos_deff[criterio[i]]);
       }

     fclose(test_history[criterio[i]]);
     fclose(test_freq[criterio[i]]);
     break;

   case NOS:
   case ARCS:      
   case USOS:
   case PUSOS:
   case DU:

     if(!no_output)
       {
       fprintf(output[criterio[i]],"\n\nCobertura Total = %f\n",
            100.*ok_automatos[criterio[i]]/total_automatos[criterio[i]]);
       fprintf(exec[criterio[i]],"\n\nCobertura Total = %f\n",
            100.*ok_automatos[criterio[i]]/total_automatos[criterio[i]]);
       }
     
     fclose(test_history[criterio[i]]);
     fclose(test_freq[criterio[i]]);
     break;
   default:;
   }
 }
}

int get_number_features(filename)
char *filename;
{
  FILE * desc;
  char linha[1000];
  int no_req;
  
  desc = fopen(filename,"r");
  
  if(desc == NULL)
    {
    fprintf(stderr,"* * Erro: nao foi possivel abrir arquivo %s * *\n",filename);
    exit(1);
    }

 /*
   position in the end of the descriptor file less a certain quantity of bytes  */
 
  fseek(desc, (long) -50, SEEK_END);
 
  /* read number of requeriments */

  le_linha_str(linha,"Numero Total de Descritores =", desc);

  sscanf(linha,"Numero Total de Descritores = %d", &no_req);

  fclose(desc);
  return(no_req);
}

/* PROGRAMA PRINCIPAL PARA A AVALIACAO DE UM PROGRAMA */

int main(argc,argv)
int argc;
char * argv[];
{

/* Declaracao de Variaveis Locais */

int i,j,no_arcs;
b_vector N[MAXCRIT+1];
AUTOMATO * pointer_aux = (AUTOMATO *) NULL, * pointer_aux2 ;

 if(argc < 2)
   error("* * Erro Fatal: Programa de avaliacao foi iniciado com numero de argumentos errado * *\n");

 for(i=1, j=1; i < argc && i < MAXCRIT+1; i++)
   {
     switch(*argv[i])
       {
      case 'N':
         no_output = TRUE;
         ++j;
         break;
       case 'y':
       case 'Y':
         no_output = FALSE;
         ++j;
         break;
       case '1':
         if(get_number_features("des_pdu.tes"))
           criterio[j]=PDU;
         else
           printf("* * Mensagem: criterio todos pot-du-caminhos nao possui descritores * *\n");
         ++j;
         break;
       case '2':
         if(get_number_features("des_pu.tes"))
           criterio[j]=PU;
         else
           printf("* * Mensagem: criterio todos pot-usos nao possui descritores * *\n");
         ++j;
         break;
       case '3':
         if(get_number_features("des_pudu.tes"))
           criterio[j]=PUDU;
         else
           printf("* * Mensagem: criterio todos pot-usos/du nao possui descritores * *\n");
         ++j;
         break;
       case '4':
         no_arcs=get_number_features("des_arc.tes");
         if(no_arcs > 1) /* E' necessario pelo menos dois arcos */
           criterio[j]=ARCS;
         else
           printf("* * Mensagem: criterio todos arcos nao possui descritores * *\n");
         ++j;
         break;
       case '5':
         if(get_number_features("des_nos.tes"))
           criterio[j]=NOS;
         else
           printf("* * Mensagem: criterio todos nos nao possui descritores * *\n");
         ++j;
         break;
       case '6':
         if(get_number_features("des_p_uses.tes"))
           criterio[j]=PUSOS;
         else
           printf("* * Mensagem: criterio todos p-usos nao possui descritores * *\n");
         ++j;
         break;
       case '7':
         if(get_number_features("des_all_uses.tes"))
           criterio[j]=USOS;
         else
           printf("* * Mensagem: criterio todos usos nao possui descritores * *\n");
         ++j;
         break;
       case '8':
         if(get_number_features("des_all_du_paths.tes"))
           criterio[j]=DU;
         else
           printf("* * Mensagem: criterio todos du-caminhos nao possui descritores * *\n");
         ++j;
         break;
       default:
         error("* * Erro Fatal: Parametro do Criterio errado no Avaliador * *");         
       }
   }
 
 pointer = NULL;
 
 for(i=1; i <= MAXCRIT; ++i)
   {
    switch(criterio[i])
      {
      case PDU:

      /* abre os arquivos relativos ao criterio todos-pot-du-caminhos */

      initiate_files(criterio[i],&test_history[criterio[i]],&descritores[criterio[i]],&requisitos[criterio[i]],&output[criterio[i]],&exec[criterio[i]],&test_freq[criterio[i]],"pduhis.tes","des_pdu.tes","pdupaths.tes","pduoutput.tes","exec_pdu.tes","pdufreq.tes");

      pdu_init_aval(&pointer_aux,&N[criterio[i]],test_history[criterio[i]],descritores[criterio[i]]);

      if(test_history[criterio[i]] != (FILE *) NULL)
        fclose(test_history[criterio[i]]);

      break;

      case PU:

      /* abre os arquivos relativos ao criterio todos-pot-usos */

      initiate_files(criterio[i],&test_history[criterio[i]],&descritores[criterio[i]],&requisitos[criterio[i]],&output[criterio[i]],&exec[criterio[i]],&test_freq[criterio[i]],"puhis.tes","des_pu.tes","puassoc.tes","puoutput.tes","exec_pu.tes","pufreq.tes");      

      pu_init_aval(&pointer_aux,&N[criterio[i]],criterio[i],test_history[criterio[i]],descritores[criterio[i]]);

      if(test_history[criterio[i]] != (FILE *) NULL)
        fclose(test_history[criterio[i]]);

      break;
       
      case PUDU:

      /* abre os arquivos relativos ao criterio todos-pot-usos */
   
      initiate_files(criterio[i],&test_history[criterio[i]],&descritores[criterio[i]],&requisitos[criterio[i]],&output[criterio[i]],&exec[criterio[i]],&test_freq[criterio[i]],"puduhis.tes","des_pudu.tes","puassoc.tes","puduoutput.tes","exec_pudu.tes","pudufreq.tes");
   
      pu_init_aval(&pointer_aux,&N[criterio[i]],criterio[i],test_history[criterio[i]],descritores[criterio[i]]);

      if(test_history[criterio[i]] != (FILE *) NULL)
        fclose(test_history[criterio[i]]);

      break;

      case USOS:
      /* abre os arquivos relativos ao criterio todos-usos */
   
      initiate_files(criterio[i],&test_history[criterio[i]],&descritores[criterio[i]],&requisitos[criterio[i]],&output[criterio[i]],&exec[criterio[i]],&test_freq[criterio[i]],"useshis.tes","des_all_uses.tes","all_uses.tes","usesoutput.tes","exec_uses.tes","usesfreq.tes");
   
      df_init_aval(&pointer_aux,&N[criterio[i]],criterio[i],test_history[criterio[i]],descritores[criterio[i]]);

      if(test_history[criterio[i]] != (FILE *) NULL)
        fclose(test_history[criterio[i]]);

      break;
   
      case PUSOS:

      /* abre os arquivos relativos ao criterio todos-p-usos */
   
      initiate_files(criterio[i],&test_history[criterio[i]],&descritores[criterio[i]],&requisitos[criterio[i]],&output[criterio[i]],&exec[criterio[i]],&test_freq[criterio[i]],"puseshis.tes","des_p_uses.tes","all_p_uses.tes","pusesoutput.tes","exec_puses.tes","pusesfreq.tes");
   
      df_init_aval(&pointer_aux,&N[criterio[i]],criterio[i],test_history[criterio[i]],descritores[criterio[i]]);

      if(test_history[criterio[i]] != (FILE *) NULL)
        fclose(test_history[criterio[i]]);

      break;
   
      case DU:
      /* abre os arquivos relativos ao criterio todos-du-caminhos */
   
      initiate_files(criterio[i],&test_history[criterio[i]],&descritores[criterio[i]],&requisitos[criterio[i]],&output[criterio[i]],&exec[criterio[i]],&test_freq[criterio[i]],"dupathshis.tes","des_all_du_paths.tes","all_du_paths.tes","dupathsoutput.tes","exec_dupaths.tes","dupathsfreq.tes");
   
      df_init_aval(&pointer_aux,&N[criterio[i]],criterio[i],test_history[criterio[i]],descritores[criterio[i]]);

      if(test_history[criterio[i]] != (FILE *) NULL)
        fclose(test_history[criterio[i]]);

      break;

      case ARCS:

      /* abre os arquivos relativos ao criterio todos-arcos */

      initiate_files(criterio[i],&test_history[criterio[i]],&descritores[criterio[i]],&requisitos[criterio[i]],&output[criterio[i]],&exec[criterio[i]],&test_freq[criterio[i]],"archis.tes","des_arc.tes","arcprim.tes","arcoutput.tes","exec_arc.tes","arcfreq.tes");

      no_arc_init_aval(&pointer_aux,&N[criterio[i]],criterio[i],test_history[criterio[i]],descritores[criterio[i]]);

      if(test_history[criterio[i]] != (FILE *) NULL)
        fclose(test_history[criterio[i]]);

      break;

      case NOS:

      /* abre os arquivos relativos ao criterio todos-arcos */

      initiate_files(criterio[i],&test_history[criterio[i]],&descritores[criterio[i]],&requisitos[criterio[i]],&output[criterio[i]],&exec[criterio[i]],&test_freq[criterio[i]],"noshis.tes","des_nos.tes","nos_grf.tes","nosoutput.tes","exec_nos.tes","nosfreq.tes");

      no_arc_init_aval(&pointer_aux,&N[criterio[i]],criterio[i],test_history[criterio[i]],descritores[criterio[i]]);

      if(test_history[criterio[i]] != (FILE *) NULL)
        fclose(test_history[criterio[i]]);

      break;
 
      default:;
 
      }

    if(criterio[i] != 0)
      if(pointer != NULL)
        {
        for(pointer_aux2 = pointer; pointer_aux2->next != (AUTOMATO *) NULL;
            pointer_aux2 = pointer_aux2->next);

        pointer_aux2->next = pointer_aux;
        }
      else
        pointer = pointer_aux;
   }
   
  msg_print("* * Realizando a avaliacao do caso de teste * *\n");

 /* Verificar a passagem de parametros aqui */
 
 avaliador(pointer,N);

 /* Verificar a passagem de parametros aqui */

 print_output(pointer,criterio);

 for(i=1; i<=MAXCRIT; ++i)
   {
   /* Destroi vetor de bits */
 
   if(criterio[i] != 0 )
     {
     b_vector_destroy(&N[criterio[i]]);
     if(descritores[criterio[i]] != NULL)
       fclose(descritores[criterio[i]]);
     if(exec[criterio[i]] != NULL)
       fclose(exec[criterio[i]]);
     if(output[criterio[i]] != NULL)
       fclose(output[criterio[i]]);
     if(requisitos[criterio[i]] != NULL)
       fclose(requisitos[criterio[i]]);
     }
   }
 
 lib_mem("* * Avaliacao do caso de teste foi bem sucedida * *");
 exit(0);
}

/*
  lib_mem(); "Libera a memoria ocupada pelos automatos e imprime no arquivo
              avalog.tes o numero 0"
*/

void lib_mem(msg)
char msg[200];
{

 /* Declaracao de Variaveis Locais */

 FILE * avalog;

 avalog = (FILE *) fopen("avalog.tes","w");
 if(avalog == (FILE *) NULL)
   {
    msg_print("* * Erro Fatal: Nao consegui abrir o arquivo avalog.tes * *\n");
    exit(1);
   }

 /* libera espaco ocupado pelos automados */


 if(pointer != (AUTOMATO *) NULL)
   {
    lib_automato(pointer->next);
    b_vector_destroy(&(pointer->Nnv));
    b_vector_destroy(&(pointer->Ni));
    free(pointer->exp_regular);
    free(pointer);
   }

 msg_print(msg);
 fprintf(avalog,"0");
 fclose(avalog);
}

/*
 lib_automato(); "desaloca o espaco ocupado por um automato"


void lib_automato(aut)
AUTOMATO * aut;
{
 if(aut->next != (AUTOMATO *) NULL)
   {
    lib_automato(aut->next);
    free(aut->exp_regular);
    free(aut);
   }
}
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

void error(msg)
char msg[200];
{

 /* Declaracao de Variaveis Locais */

 FILE * avalog;

 /* escreve mensagem de erro */

 printf("%s\n",msg);

 avalog = (FILE *) fopen("avalog.tes","w");
 if(avalog == (FILE *) NULL)
   {
    msg_print("* * Erro Fatal: Nao consegui abrir o arquivo avalog.tes * *\n");
    exit(1);
   }

 /* libera espaco ocupado pelos automados */

 if(pointer != (AUTOMATO *) NULL)
   {
    lib_automato(pointer->next);
    b_vector_destroy(&(pointer->Nnv));
    b_vector_destroy(&(pointer->Ni));
    free(pointer->exp_regular);
    free(pointer);
   }

 fprintf(avalog,"-1");
 fprintf(avalog,"%s",msg);
 fclose(avalog);
 exit(1);
}

/*
   imprime no campo mensagem
*/

void msg_print(msg)
char msg[200];
{
 /* posiciona cursor para escrever mensagem */
 printf("%s",msg);
 printf("\n");
}









































