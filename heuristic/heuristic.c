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

/*
// heuristic.c			%I%	%Y%	%E% %U%
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "util.h"
#include "cte.h"
#include "newpokelib.h"
#include "haval.h"


#define SUCCESS 1
#define FAIL    0
#define FREQ    2
#define SIMPLE  3

/* Test Case Info */

typedef struct tc_info { 
  int result;           /* fail or success */
  b_vector req_exec;
} TCinfo;

/* Test Requirement Info */

typedef struct req_info {
  double heuristic;
  int freq_succ;
  int freq_fail;
  int req_no;
  long count_fail;
} Reqinfo;

/* Heuristic Info */

typedef struct heur_info {
  int exper;
  int type;
  int number;
  char * func_dir;
  char * criterion;
  int no_req;
  int no_nos;
  int * stmt_counter;
  int no_test_cases;
  int * tests_map;
  int totalfailed;
  int totalsucc;
  int fail_random;
  int succ_random;
  int stmt_reveal;
  int stmt_prone;
  b_vector dice_fail;
  b_vector dice_succ;
  b_vector fault_reveal;
  b_vector fault_nodes;
  b_vector sel_nodes;
  b_vector sel_reqs;
} heur;

void check_mem_error (void * ptr)
{
 if(ptr == (void *) NULL)
  {
  fprintf(stderr,"pokeheuristics Error: Error: run out of memory\n");
  exit(1);
  }
}

void get_req_number(h)
heur * h;
{
 FILE *desc;
 char filename[1000], linha[1000];
 
  strcpy(filename,h->func_dir);
  strcat(filename,"/");
  
  if(!strcmp(h->criterion,"pudu"))
    /* == pudu */
    strcat(filename,"des_pudu.tes");
  else
    /* == pu */
    if(!strcmp(h->criterion,"pu"))
      strcat(filename,"des_pu.tes");
    else
      if(!strcmp(h->criterion,"pdu"))
        /* == pdu */
        strcat(filename,"des_pdu.tes");
      else
        if(!strcmp(h->criterion,"arcs"))
          /* == arcs */
          strcat(filename,"des_arc.tes");
        else
          if(!strcmp(h->criterion,"nos"))
            /* == nos */
            strcat(filename,"des_nos.tes");
          else
            if(!strcmp(h->criterion,"uses"))
              /* == uses */
              strcat(filename,"des_all_uses.tes");
            else
              if(!strcmp(h->criterion,"puses"))
                /* == puses */
                strcat(filename,"des_p_uses.tes");
              else
                if(!strcmp(h->criterion,"du"))
                  /* == puses */
                  strcat(filename,"des_all_du_paths.tes");
  
  desc = fopen(filename,"r");
  
  if(desc == NULL)
    {
    fprintf(stderr,"pokeheuristics Error: it was not possible to open file %s\n",filename);
    exit(1);
    }

 /*
   position in the end of the descriptor file less a certain quantity of bytes  */
 
 fseek(desc, (long) -50, SEEK_END);
 
 /* read number of requeriments */

 le_linha_str(linha,"Numero Total de Descritores =", desc);

 sscanf(linha,"Numero Total de Descritores = %d", &(h->no_req));

 /* Se arcs, ajust the number of requisits */
 if(!strcmp(h->criterion,"arcs"))
    h->no_req--;

 b_vector_cons(&(h->sel_reqs),(h->no_req+1)/BYTE+1,0);
 b_vector_cons(&(h->fault_reveal),(h->no_req+1)/BYTE+1,0);

 fclose(desc);
}

void get_nodes_info(h,p_str)
heur * h;
char * p_str;
{
 FILE *desc, * fault_nodesfile;
 char filename[1000], linha[1000];
 int fn;

  strcpy(filename,h->func_dir);
  strcat(filename,"/des_nos.tes");
  
  desc = fopen(filename,"r");
  
  if(desc == NULL)
    {
    fprintf(stderr,"pokeheuristics Error: it was not possible to open file %s\n",filename);
    exit(1);
    }

 /*
   position in the end of the descriptor file less a certain quantity of bytes  */
 
 fseek(desc, (long) -50, SEEK_END);
 
 /* read number of requeriments */

 le_linha_str(linha,"Numero Total de Descritores =", desc);

 sscanf(linha,"Numero Total de Descritores = %d", &(h->no_nos));

 b_vector_cons(&(h->sel_nodes),(h->no_nos+1)/BYTE+1,0);

 fclose(desc);

 b_vector_cons(&(h->fault_nodes),(h->no_nos+1)/BYTE+1,0);

 if(p_str != NULL)
   {
   
   strcpy(filename,h->func_dir);
   strcat(filename,"/");
   strcat(filename,p_str);

   fault_nodesfile= fopen(filename,"r");
  
   if(fault_nodesfile == NULL)
     {
     /* There is no faulty nodes file */
     return;
     }

   while(!feof(fault_nodesfile))
     {
      fscanf(fault_nodesfile,"%d",&fn); 
      set_bit(fn,&(h->fault_nodes));
     }
   fclose(fault_nodesfile);
  }
}

void get_fault_revealing(h, p_str)
     heur * h;
     char * p_str;
{
  char filename[1000];
  FILE * fault_revealfile;
  struct stat file_info;
  int fr;
  
  strcpy(filename,"./");
  strcat(filename,h->func_dir);
  strcat(filename,"/");
  strcat(filename,p_str);

  fault_revealfile = fopen(filename,"r");
  
  if(fault_revealfile == NULL)
    {
    /* There is no fault-revealing file */
    return;
    }

  if(stat(filename,&file_info))
    return;

  if(file_info.st_size == 0)
    return;

  while(!feof(fault_revealfile))
    {
      fscanf(fault_revealfile,"%d",&fr); 
      set_bit(fr,&(h->fault_reveal));
    }
  fclose(fault_revealfile);
}

void set_sel_nodes(h,p_str)
     heur * h;
     char * p_str;
{
  char * s;
  char number[20];
  int i;

  s = p_str;
  number[0]='\0';

  for(i = 0;*s!='\0';++s)
    {
      switch(*s)
	{
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	  number[i]=*s; 
	  ++i; 
	  number[i]='\0'; 
	  break;
	case ')': 
	case ',':
          set_bit(atoi(number),&(h->sel_nodes));
          i = 0;
	  break;
        default:
	  i = 0;
	}
    }
}

FILE * open_req(h)
heur * h;
{
 FILE * ret;
 char filename[1000];

  strcpy(filename,h->func_dir);
  strcat(filename,"/");
  
  if(!strcmp(h->criterion,"pudu") || !strcmp(h->criterion,"pu"))
     /* == pudu or == pu */
     strcat(filename,"puassoc.tes");
  else
     if(!strcmp(h->criterion,"pdu"))
       /* == pdu */
       strcat(filename,"pdupaths.tes");
     else
       if(!strcmp(h->criterion,"arcs"))
         /* == arcs */
         strcat(filename,"arcprim.tes");
       else
         if(!strcmp(h->criterion,"nos"))
           /* == nos */
           strcat(filename,"nos_grf.tes");
         else
           if(!strcmp(h->criterion,"uses"))
             /* == uses */
             strcat(filename,"all_uses.tes");
           else
             if(!strcmp(h->criterion,"puses"))
               /* == puses */
               strcat(filename,"all_p_uses.tes");
             else
               if(!strcmp(h->criterion,"du"))
                 /* == puses */
                 strcat(filename,"all_du_paths.tes");
  
  ret = fopen(filename,"r");
  if(ret == NULL)
    {
    fprintf(stderr,"pokeheuristics Error: it was not possible to open file %s\n",filename);
    exit(1);
    }
 return(ret);
}

int compare_rank_heur_freq(Reqinfo * p, Reqinfo *q)
{
  if (p->heuristic > q->heuristic)
    return (-1);
  if (p->heuristic < q->heuristic)
    return (1);
  if(p->heuristic == q->heuristic)
    {
    if(p->count_fail > q->count_fail)
      return (-1);
    if(p->count_fail < q->count_fail)
      return (1);
    return(0);
    }
  return (0);   
}

int compare_rank_heur(Reqinfo * p, Reqinfo *q)
{
  if (p->heuristic > q->heuristic)
    return (-1);
  if (p->heuristic < q->heuristic)
    return (1);
  if(p->heuristic == q->heuristic)
    {
    if(p->req_no > q->req_no)
      return (1);
    if(p->req_no < q->req_no)
      return (-1);
    return(0);
    }
  return (0);   
}

int order(Reqinfo * p, Reqinfo *q)
{
 if(p->req_no > q->req_no)
   return (1);
 if(p->req_no < q->req_no)
   return (-1);
 return(0);  
}

int freq(Reqinfo * p, Reqinfo *q)
{
 if(p->heuristic == q->heuristic)
    {
    if(p->count_fail > q->count_fail)
      return (-1);
    if(p->count_fail < q->count_fail)
      return (1);
    return(0);
    }
 return(0);
}

void get_heuristic_data(heur * h, int argc,char * argv[])
{
  int i;
  h->exper = 0; /* Experiment mode is not default */
 /* Check fixed parameters */

 if(!strncmp(argv[1],"-hFH",4))
   {
   h->type = FREQ;
   h->number = atoi(argv[1]+4);
   }
 else
   if(!strncmp(argv[1],"-hH",3))
     {
     h->type = SIMPLE;
     h->number = atoi(argv[1]+3);
     }
   else
     {
     fprintf(stderr,"pokeeuristics Error: heuristic type is missing\n");
     exit(1);
     }
 
 if(!strncmp(argv[2],"-d",2))
   {
   h->func_dir = strdup(argv[2]+2);
   check_mem_error ((void *)h->func_dir);
   }
 else
   {
   fprintf(stderr,"pokeheuristics Error: function directory is missing\n");
   exit(1);
   }

 if(!strncmp(argv[3],"-c",2))
   {
   h->criterion = strdup(argv[3]+2);
   check_mem_error ((void *)h->criterion);
   }
 else
   {
   fprintf(stderr,"pokeheuristics Error: criterion is missing\n");
   exit(1);
   }

/* Get the number of requirements defined by the chosen criterion */

 get_req_number(h); 

/* Get the fault revealing test case numbers from a file, if present. */

if(!strncmp(argv[4],"-F",2))
   {
     get_fault_revealing(h,argv[4]+2);
     h->exper+=1;
     i = 5;
   }
 else
   i = 4;

/* Set up bit_vectors associated to nodes and set the nodes in which faults occur.
    If this information is available */
 
if(!strncmp(argv[i],"-N",2))
   {
     get_nodes_info(h,argv[i]+2);
     h->exper+=1; ++i;
   }
 else
   get_nodes_info(h,NULL);

/* Set the number of test cases */

 if(!strcmp(argv[i],"-testset"))
   {
   h->no_test_cases = atoi(argv[i+1]);
   }
 else
   {
   fprintf(stderr,"pokeheuristics Error: the number of test cases is missing\n");
   exit(1);
   } 

}

void get_testset_data(tcases, h, argc, argv)
TCinfo * tcases;
heur * h;
int argc;
char * argv[];
{
 int i,test_idx;
 int totalfailed =0, totalsucc=0;
 
 /* create the test map vector */

  h->tests_map = (int *) malloc(sizeof(int) * (h->no_test_cases+1));

  check_mem_error ((void *) h->tests_map);

 /* set the fields of the vector of test cases */
 
 for(i=0; i <= h->no_test_cases; ++i)
   {
   h->tests_map[i]=i+1;
   tcases[i].result = SUCCESS;
   b_vector_cons(&(tcases[i].req_exec),h->no_req/BYTE+1,0);
   }
 
 i=h->exper+6; /* set number of the next parameter */

 if(!strcmp(argv[i],"-fail"))
   { 
   /* set the FAIL test cases */
     for(i++, test_idx=0; (i < argc) && (strcmp(argv[i],"-succ")  && strcmp(argv[i],"-random")); ++i, test_idx++)
     {
       h->tests_map[test_idx]=atoi(argv[i]);
       tcases[test_idx].result = FAIL;
       set_bit(test_idx,&(h->dice_fail));
       totalfailed++; 
      }
   }

 if((i < argc) && !strcmp(argv[i],"-succ"))
    {
     /* set the SUCCESS test cases */
     for(i++; (i < argc)  && strcmp(argv[i],"-random"); ++i, test_idx++)
       {
       h->tests_map[test_idx]=atoi(argv[i]);
       tcases[test_idx].result = SUCCESS;
       set_bit(test_idx,&(h->dice_succ));
       totalsucc++;
       }
     h->totalsucc=totalsucc;
     h->totalfailed=totalfailed;

     if(h->totalsucc+h->totalfailed != h->no_test_cases)
        {
         fprintf(stderr,"pokeheuristics Error: number of failed tests plus number of passed test (succ) is different of total nubmer of tests\n");
         exit(1);
        }      
    }
 else
    {
    fprintf(stderr,"pokeheuristics Error: description of random heuristic is wrong\n");
    exit(1);
    }

 if((i < argc) && !strcmp(argv[i],"-random"))
    {
     i++;
     if((i < argc) && !strcmp(argv[i],"-fail"))
        {
        ++i;
        h->fail_random = atoi(argv[i]);
        }
     else
        {
        fprintf(stderr,"pokeheuristics Error: description of random heuristic is wrong\n");
        exit(1);
        }

     i++;
     if((i < argc) && !strcmp(argv[i],"-succ"))
        {
        ++i;
        h->succ_random = atoi(argv[i]);          
        }
     else
        {
        fprintf(stderr,"pokeheuristics Error: description of random heuristic is wrong\n");
        exit(1);
        }        
    }
 else
   {
   h->fail_random = h->succ_random = 0;
   }
 /*
 if((i < argc) && (!strcmp(argv[i],"-dice")))
    {
     i++;
     if((i < argc) && (!strcmp(argv[i],"-fail")))
        {
        for(i++;(i < argc) &&  strcmp(argv[i],"-succ"); ++i)
          set_bit(get_testcase_id(h->tests_map,atoi(argv[i])),&(h->dice_fail));
        }
     else
        {
        fprintf(stderr,"pokeheuristics Error: description of dice heuristic is wrong\n");
        exit(1);
        }
     
     
     if((i < argc) && (!strcmp(argv[i],"-succ")))
        {
        for(i++; (i < argc) &&  strcmp(argv[i],"-fail"); ++i)
          set_bit(atoi(get_testcase_id(h->tests_map,atoi(argv[i])),&(h->dice_succ));
        }
     else
        {
        fprintf(stderr,"pokeheuristics Error: description of dice heuristic is wrong\n");
        exit(1);
        }       
    }
 */
}
    

void get_req_data(h,tcases,reqs,argc,argv)
heur * h;
TCinfo * tcases;
Reqinfo * reqs;
int argc;
char * argv[];
{
 int i, seq_i, seq_j;
 char  * freqfile, * freqcte;
 FILE * freq;
 struct stat file_info;
 
 for(i=0; i <= h->no_req;++i)
    {
    reqs[i].req_no = i;
    reqs[i].heuristic = 0;
    reqs[i].freq_succ = 0;
    reqs[i].freq_fail = 0;
    reqs[i].count_fail = 0;
    }
  
  /* set the satisfied requirements */

  freqfile = malloc(strlen(h->func_dir)+strlen(h->criterion)+strlen("freq")+strlen(".tes")+12);
  check_mem_error ((void *) freqfile);

  freqcte = malloc(strlen(h->func_dir)+strlen(h->criterion)+strlen("freq")+strlen(".tes")+12);
  check_mem_error ((void *) freqcte);
  
  strcpy(freqcte,h->func_dir);
  strcat(freqcte,"/");

  if(!strcmp(h->criterion,"arcs"))
    strcat(freqcte,"arc");
  else
    strcat(freqcte,h->criterion);

  strcat(freqcte,"freq");

  for(i=0; i < h->no_test_cases; ++i)
    {
    char str[15];
    int exec_req, freq_exec, freq_tot;

    /* find the name of the frequency file */
    
    strcpy(freqfile, freqcte);
    sprintf(str,"%d",h->tests_map[i]);
    strcat(freqfile,str);
    strcat(freqfile,".tes");

    if(stat(freqfile,&file_info))
       {
	 /* fprintf(stderr, "heuristc: Msg: impossible to get information on test case %d\n", i); */
      continue;
      }     

    if(file_info.st_size == 0)
      {
        /* fprintf(stderr, "heuristc: Msg: no testing requeriments was executed by test case %d\n", i); */
      continue;
      }

    
    freq = fopen(freqfile,"r");

    if(freq == NULL)
      {     
      fprintf(stderr, "heuristc: Msg: file of frequencies of test case %d missing\n", i);     
      continue;
      }

    fscanf(freq,"Numero total de instancias de requisitos = %d\n",&exec_req);

    while(!feof(freq))
      { 
        int func_inst;
	/* Code for reexecution */
	if(!strcmp(h->criterion,"nos"))
	  fscanf(freq,"%d (%d %d/%d) (%d)\n",&exec_req,&freq_exec,&freq_tot,&func_inst,&seq_i);
	else /* != nos */
	  fscanf(freq,"%d (%d/%d) (%d %d/%d)\n",&exec_req,&freq_exec,&freq_tot,&func_inst,&seq_i,&seq_j); 
 
	/* Correct Code for the new version of the frequency files        
	if(!strcmp(h->criterion,"nos"))
	  fscanf(freq,"%d %d (%d %d)",&exec_req,&freq_exec,&func_inst,&seq_i);
	else 
	  fscanf(freq,"%d %d (%d %d/%d)",&exec_req,&freq_exec,&func_inst,&seq_i,&seq_j);

	if(freq_exec > 1)
	  {
	  if(!strcmp(h->criterion,"nos"))
	    fscanf(freq,"(%d %d)",&func_inst,&seq_i);
	  else
	    fscanf(freq,"(%d %d/%d)",&func_inst,&seq_i,&seq_j);
	  }
	*/
	/* Codigo modificado para tratar bug monitoracao merge  in Sort-21 

        if(!strcmp(h->func_dir, "merge") && !strcmp(h->criterion,"uses"))
          {
	    if((exec_req < 436 ||  exec_req > 450) && ( exec_req < 433 || exec_req > 434))
	      set_bit(exec_req,&(tcases[i].req_exec));              
            else
	      continue;
	  }
	else
          if(!strcmp(h->func_dir, "merge") && !strcmp(h->criterion,"pu"))
            {
	    if(exec_req < 914 ||  exec_req > 950)
	      set_bit(exec_req,&(tcases[i].req_exec));              
            else
	      continue;
	    }
	  else
           set_bit(exec_req,&(tcases[i].req_exec));
	*/
	/* Codigo original */
	set_bit(exec_req,&(tcases[i].req_exec));
	

        if(tcases[i].result != SUCCESS)
          { /* Fail */
          reqs[exec_req].freq_fail++;  
          reqs[exec_req].count_fail+=freq_exec;
          }
        else
          { /* Success */
          reqs[exec_req].freq_succ++;  
          }
      }
    
    fclose(freq);
    }
}

void RankingHeur(h,reqs,comp,resultsfile)
heur *h;
Reqinfo * reqs;
int (*comp)(__const void *, __const void *);
FILE * resultsfile;
{
 int i,j, exec;
 char * ptr_path;  
 FILE * requisitos;
 double rank_rate;
 
 for(i=1; i <= h->no_req;++i)    
    reqs[i].heuristic = ((double)reqs[i].freq_fail)/(1+((double)reqs[i].freq_succ)/h->no_test_cases);
    
 qsort((char *)reqs, h->no_req, sizeof(Reqinfo), comp);

  requisitos = open_req(h);

  exec = 0;
  
  for(i=0,j=0; i <= h->no_req;++i)
    {
    if(reqs[i].heuristic != 0)
      ++exec;
    
    if(reqs[i].heuristic == reqs[0].heuristic && reqs[i].heuristic != 0.0)
      {
      ++j;
      set_bit(reqs[i].req_no,&(h->sel_reqs));
      if(!strcmp(h->criterion,"nos"))
	{
	fprintf(resultsfile,"%d) %2.3f %d\n",j,reqs[i].heuristic,reqs[i].req_no);
	set_bit(reqs[i].req_no, &(h->sel_nodes));
	}
      else
	{      
        ptr_path  = pega_path(reqs[i].req_no,requisitos);
	fprintf(resultsfile,"%d) %2.3f %s\n",j,reqs[i].heuristic,apont_next_tok(ptr_path));
	set_sel_nodes(h,apont_next_tok(ptr_path));
	free(ptr_path);
	}
      }
    }

  if(exec != 0)
    fprintf(resultsfile,"\nRanking Rate for %s = %2.3f\n", h->func_dir, ((double)j)/((double)exec));
  else
    fprintf(resultsfile,"\nRanking Rate for %s = 0.0\n", h->func_dir);
    
  
  fclose(requisitos);
}

void RankingHeur2(h,reqs,comp,resultsfile)
heur *h;
Reqinfo * reqs;
int (*comp)(__const void *, __const void *);
FILE * resultsfile;
{
 int i,j, exec;
 char * ptr_path;  
 FILE * requisitos;
 double rank_rate, rank1, rank2;
 
 for(i=1; i <= h->no_req;++i)    
    reqs[i].heuristic = ((double)reqs[i].freq_fail)/(1+((double)reqs[i].freq_succ)/h->no_test_cases);
    
  qsort((char *)reqs, h->no_req, sizeof(Reqinfo), comp);

  /* ajust ranking level 1 and 2 */

  rank1 = reqs[0].heuristic;
  for(i=1; i <= h->no_req;++i)
     {
       if(reqs[i].heuristic < rank1)
	 {
	  rank2=reqs[i].heuristic;
	  break;
	 }
     }

  requisitos = open_req(h);

  exec = 0;
  
  for(i=0,j=0; i <= h->no_req;++i)
    {
    if(reqs[i].heuristic != 0)
      ++exec;
    
    if((reqs[i].heuristic == rank1 || reqs[i].heuristic == rank2) 
        && reqs[i].heuristic != 0.0)
      {
      ++j;
      set_bit(reqs[i].req_no,&(h->sel_reqs));
      if(!strcmp(h->criterion,"nos"))
	{
	fprintf(resultsfile,"%d) %2.3f %d\n",j,reqs[i].heuristic,reqs[i].req_no);
	set_bit(reqs[i].req_no, &(h->sel_nodes));
	}
      else
	{      
        ptr_path  = pega_path(reqs[i].req_no,requisitos);
	fprintf(resultsfile,"%d) %2.3f %s\n",j,reqs[i].heuristic,apont_next_tok(ptr_path));
	set_sel_nodes(h,apont_next_tok(ptr_path));
	free(ptr_path);
	}
      }
    }

  if(exec != 0)
    fprintf(resultsfile,"\nRanking Rate for %s = %2.3f\n", h->func_dir, ((double)j)/((double)exec));
  else
    fprintf(resultsfile,"\nRanking Rate for %s = 0.0\n", h->func_dir);
    
  
  fclose(requisitos);
}


void RankingHeur3(h,reqs,comp,resultsfile)
heur *h;
Reqinfo * reqs;
int (*comp)(__const void *, __const void *);
FILE * resultsfile;
{
 int i,j, exec;
 char * ptr_path;  
 FILE * requisitos;
 double rank_rate, rank1, rank2;
 
 for(i=1; i <= h->no_req;++i)  
   {
     if(reqs[i].freq_fail == 0 && reqs[i].freq_succ == 0)
       reqs[i].heuristic = 0; /* requeriment was not exercised, thus is not suspicious */
     else
        reqs[i].heuristic = 
     (((double)reqs[i].freq_fail)/h->totalfailed)/((((double)reqs[i].freq_fail)/h->totalfailed)+(((double)reqs[i].freq_succ)/h->totalsucc));
   }

  qsort((char *)reqs, h->no_req, sizeof(Reqinfo), comp);


  requisitos = open_req(h);
  exec = 0;
  
  for(i=0,j=0; i <= h->no_req;++i)
    {
    if(reqs[i].heuristic != 0)
      ++exec;
    
    if(/* reqs[i].heuristic == reqs[0].heuristic && */ reqs[i].heuristic != 0.0)
      {
      ++j;
      set_bit(reqs[i].req_no,&(h->sel_reqs));
      if(!strcmp(h->criterion,"nos"))
	{
	fprintf(resultsfile,"%d) %2.3f %d\n",j,reqs[i].heuristic,reqs[i].req_no);
	set_bit(reqs[i].req_no, &(h->sel_nodes));
	}
      else
	{      
        ptr_path  = pega_path(reqs[i].req_no,requisitos);
	fprintf(resultsfile,"%d) %2.3f %s\n",j,reqs[i].heuristic,apont_next_tok(ptr_path));
	set_sel_nodes(h,apont_next_tok(ptr_path));
	free(ptr_path);
	}
      }
    }

  if(exec != 0)
    fprintf(resultsfile,"\nRanking Rate for %s = %2.3f\n", h->func_dir, ((double)j)/((double)exec));
  else
    fprintf(resultsfile,"\nRanking Rate for %s = 0.0\n", h->func_dir);
    
  
  fclose(requisitos);

}



void H1(h, tcases, reqs,comp,resultsfile)
heur * h;
TCinfo * tcases;
Reqinfo * reqs;
int (*comp)(__const void *, __const void *);
FILE * resultsfile;
{
 int i,j;
 b_vector acum;
 char * ptr_path;
 FILE * requisitos;
 int thereisfail=FALSE;

 /* Initially, accumulator bits are all set */
   
 b_vector_cons(&acum,h->no_req/BYTE+1,1);

 for(i=0; i < h->no_test_cases; ++i)
   {
    if(tcases[i].result == FAIL)
      {
	thereisfail=TRUE;
	inter_b_vector(&acum,&(tcases[i].req_exec));
      }
   }

 if(!thereisfail)
   reset_all(&acum);

 for(i=1; i <= h->no_req; ++i)
   if(test_bit(i,&acum))
      reqs[i].heuristic=1.0;
 
 qsort((char *)reqs, h->no_req, sizeof(Reqinfo), comp);

 requisitos = open_req(h);

 for(i=1,j=0; i <= h->no_req;++i)
   if(reqs[i].heuristic  != 0.0)
     {
     ++j;
     set_bit(reqs[i].req_no,&(h->sel_reqs));
     if(!strcmp(h->criterion,"nos"))
       {
       fprintf(resultsfile,"%d) %2.3f %d\n",j,reqs[i].heuristic,reqs[i].req_no);
       set_bit(reqs[i].req_no, &(h->sel_nodes));
       }
     else
       {
       ptr_path  = pega_path(reqs[i].req_no,requisitos);
       fprintf(resultsfile,"%d) %2.3f %s\n",j,reqs[i].heuristic,apont_next_tok(ptr_path));
       set_sel_nodes(h,apont_next_tok(ptr_path));
       free(ptr_path);
       }
     }
 
 fclose(requisitos);
}

void H2(h, tcases, reqs,comp,resultsfile)
heur * h;
TCinfo * tcases;
Reqinfo * reqs;
int (*comp)(__const void *, __const void *);
FILE * resultsfile;
{
 int i,j;
 b_vector acum;
 int fail_test;
 int succ_test;
 char * ptr_path;
 FILE * requisitos;
 
 for(i=0; i < h->no_test_cases; ++i)
   {
    if(test_bit(i,&(h->dice_fail)))
      {
      fail_test = i;
      break;
      }
   }
    
 for(i=0; i < h->no_test_cases; ++i)
   {
    if(test_bit(i,&(h->dice_succ)))
      {
      succ_test = i;
      break;
      }
   }

/* Initially, accumulator bits are all set */
   
 b_vector_cons(&acum,h->no_req/BYTE+1,1);

 b_vector_cpy(&acum,&(tcases[fail_test].req_exec));

 sub_b_vector(&acum,&(tcases[succ_test].req_exec));

 for(i=1; i <= h->no_req; ++i)
   if(test_bit(i,&acum))
      reqs[i].heuristic=1.0;
 
 qsort((char *)reqs, h->no_req, sizeof(Reqinfo), comp);

 requisitos = open_req(h);
 
 for(i=1; i <= h->no_req;++i)  
   if(reqs[i].heuristic  != 0.0)      
     {
     set_bit(reqs[i].req_no,&(h->sel_reqs));
     if(!strcmp(h->criterion,"nos"))
       {
       fprintf(resultsfile,"%d) %2.3f %d\n",i+1,reqs[i].heuristic,reqs[i].req_no);
       set_bit(reqs[i].req_no, &(h->sel_nodes));
       }
     else
       {
       ptr_path  = pega_path(reqs[i].req_no,requisitos);
       fprintf(resultsfile,"%d) %2.3f %s\n",i+1,reqs[i].heuristic,apont_next_tok(ptr_path));
       set_sel_nodes(h,apont_next_tok(ptr_path));
       free(ptr_path);
       }
     }
 
 fclose(requisitos);
}

void DiceHeur(h, tcases, reqs,comp,resultsfile)
heur * h;
TCinfo * tcases;
Reqinfo * reqs;
int (*comp)(__const void *, __const void *);
FILE * resultsfile;
{
 int i,j;
 b_vector acum1;
 b_vector acum2;
 char * ptr_path;
 FILE * requisitos;
 int thereisfail=FALSE;

 for(i=0; i < h->no_test_cases; ++i)
   {
    if(tcases[i].result == FAIL)
      {
	thereisfail=TRUE;
	break;
      }
   }

 /* Set accumulators */

 switch(h->number)
   {
   case 2:
      b_vector_cons(&acum1,h->no_req/BYTE+1,0);
      b_vector_cons(&acum2,h->no_req/BYTE+1,0);
      break;     
   case 3:    
      if(thereisfail) 
	b_vector_cons(&acum1,h->no_req/BYTE+1,1);
      else
	b_vector_cons(&acum1,h->no_req/BYTE+1,0);
      b_vector_cons(&acum2,h->no_req/BYTE+1,0);
      break;
   case 4:
      b_vector_cons(&acum1,h->no_req/BYTE+1,0);
      b_vector_cons(&acum2,h->no_req/BYTE+1,0);
      break;
   case 5:
      b_vector_cons(&acum1,h->no_req/BYTE+1,0);
      b_vector_cons(&acum2,h->no_req/BYTE+1,1);
      break;
   case 6:
      if(thereisfail)
	b_vector_cons(&acum1,h->no_req/BYTE+1,1);
      else
	b_vector_cons(&acum1,h->no_req/BYTE+1,0);	
      b_vector_cons(&acum2,h->no_req/BYTE+1,1);
      break;      
    }
 
 for(i=0; i < h->no_test_cases; ++i)
   {
    if(test_bit(i,&(h->dice_fail)))
      {
      switch(h->number)
        {
        case 2:
          b_vector_cpy(&acum1,&(tcases[i].req_exec));
          break;
        case 3:
        case 6:
          inter_b_vector(&acum1,&(tcases[i].req_exec));
          break;
        case 4:
        case 5:
          union_b_vector(&acum1,&(tcases[i].req_exec));
          break;
        }
      }
   }
    
 for(i=0; i < h->no_test_cases; ++i)
   {
    if(test_bit(i,&(h->dice_succ)))
      {
      switch(h->number)
        {
        case 2:
          b_vector_cpy(&acum2,&(tcases[i].req_exec));
          break;         
        case 3:
        case 4:
          union_b_vector(&acum2,&(tcases[i].req_exec));
          break;
        case 5:
        case 6:
          inter_b_vector(&acum2,&(tcases[i].req_exec));
          break;
        }
      }
   }

 sub_b_vector(&acum1,&acum2);

 for(i=1; i <= h->no_req; ++i)
   if(test_bit(i,&acum1))
      reqs[i].heuristic=1.0;
 
 qsort((char *)reqs, h->no_req, sizeof(Reqinfo), comp);

 requisitos = open_req(h);
 
 for(i=1; i <= h->no_req;++i)  
   if(reqs[i].heuristic != 0.0)       
     {
     set_bit(reqs[i].req_no,&(h->sel_reqs));
     if(!strcmp(h->criterion,"nos"))
       {
       fprintf(resultsfile,"%d) %2.3f %d\n",i+1,reqs[i].heuristic,reqs[i].req_no);
       set_bit(reqs[i].req_no, &(h->sel_nodes));
       }
     else     
       {
       ptr_path  = pega_path(reqs[i].req_no,requisitos);
       fprintf(resultsfile,"%d) %2.3f %s\n",i+1,reqs[i].heuristic,apont_next_tok(ptr_path));
       set_sel_nodes(h,apont_next_tok(ptr_path));
       free(ptr_path);
       }
     }
 
 fclose(requisitos);
}

void RandomDiceHeur(h, tcases, reqs,comp,resultsfile)
heur * h;
TCinfo * tcases;
Reqinfo * reqs;
int (*comp)(__const void *, __const void *);
FILE * resultsfile;
{
 int i,j, max_fail=0, max_succ=0;
 int elem, chosen, succ_elem, fail_elem;
 int * fail_rand_choice, * succ_rand_choice;
 char * ptr_path;
 FILE * requisitos;
 
 b_vector acum1;
 b_vector acum2;

 /* Check the number of fail and succ test cases */

 for(i=0; i < h->no_test_cases; ++i)
   {
     if(tcases[i].result == FAIL)
       max_fail++;
     else
       max_succ++;
   }

 /* Reset the dice vector of bits */

 reset_all(&(h->dice_fail));
 reset_all(&(h->dice_succ));

 if(h->fail_random > max_fail)
   {
   fprintf(stderr,"pokeheuristics Error: number of random fail test cases is anadequate\n");
   exit(1);
   }

 if(h->succ_random > max_succ)
   {
   fprintf(stderr,"pokeheuristics Error: number of random success test cases is anadequate\n");
   exit(1);
   }

 /* Pick the random fail testcases */

 srand((unsigned int) time(NULL));

 fail_rand_choice = (int *)malloc(sizeof(int)*(max_fail+1));

 for(i=0; i <= max_fail; ++i)
   {
   fail_rand_choice[i] = 0;
   }

 /* Choose random */
 
 chosen = h->fail_random;
 do
   {
   elem = (rand()%max_fail)+1;
   if(fail_rand_choice[elem] == 0)
     {
     chosen--;
     fail_rand_choice[elem] = 1;
     }
   }
 while(chosen > 0);

 /* Pick the random succ testcases */

 succ_rand_choice = (int *)malloc(sizeof(int)*(max_succ+1));

 for(i=0; i <= max_succ; ++i)
   {
   succ_rand_choice[i] = 0;
   }

 /* Choose random */
 
 chosen = h->succ_random;
 do
   {
   elem = (rand()%max_succ)+1;
   if(succ_rand_choice[elem] == 0)
     {
     chosen--;
     succ_rand_choice[elem] = 1;
     }
   }
 while(chosen > 0);

 /* Set h->dice_fail and h->dice_succ bitvectors */

 succ_elem = fail_elem = 0;
 for(i=0; i<h->no_test_cases; ++i)
   {
   if(tcases[i].result == FAIL)
     {
     ++fail_elem;
     if(fail_rand_choice[fail_elem])
       set_bit(i,&(h->dice_fail));
     }
   else
     {
     ++succ_elem;
     if(succ_rand_choice[succ_elem])
       set_bit(i,&(h->dice_succ));
     }
   }
 
 /* Set accumulators */

 switch(h->number)
   {
   case 2:
      b_vector_cons(&acum1,h->no_req/BYTE+1,0);
      b_vector_cons(&acum2,h->no_req/BYTE+1,0);
      break;     
   case 3:     
      b_vector_cons(&acum1,h->no_req/BYTE+1,1);
      b_vector_cons(&acum2,h->no_req/BYTE+1,0);
      break;
   case 4:
      b_vector_cons(&acum1,h->no_req/BYTE+1,0);
      b_vector_cons(&acum2,h->no_req/BYTE+1,0);
      break;
   case 5:
      b_vector_cons(&acum1,h->no_req/BYTE+1,0);
      b_vector_cons(&acum2,h->no_req/BYTE+1,1);
      break;
   case 6:
      b_vector_cons(&acum1,h->no_req/BYTE+1,1);
      b_vector_cons(&acum2,h->no_req/BYTE+1,1);
      break;      
    }
 
 for(i=1; i <= h->no_test_cases; ++i)
   {
    if(test_bit(i,&(h->dice_fail)))
      {
      switch(h->number)
        {
        case 2:
          b_vector_cpy(&acum1,&(tcases[i].req_exec));
          break;
        case 3:
        case 6:
          inter_b_vector(&acum1,&(tcases[i].req_exec));
          break;
        case 4:
        case 5:
          union_b_vector(&acum1,&(tcases[i].req_exec));
          break;
        }
      }
   }
    
 for(i=0; i < h->no_test_cases; ++i)
   {
    if(test_bit(i,&(h->dice_succ)))
      {
      switch(h->number)
        {
        case 2:
          b_vector_cpy(&acum2,&(tcases[i].req_exec));
          break;         
        case 3:
        case 4:
          union_b_vector(&acum2,&(tcases[i].req_exec));
          break;
        case 5:
        case 6:
          inter_b_vector(&acum2,&(tcases[i].req_exec));
          break;
        }
      }
   }

 sub_b_vector(&acum1,&acum2);

 for(i=1; i <= h->no_req; ++i)
   if(test_bit(i,&acum1))
      reqs[i].heuristic=1.0;
 
 qsort((char *)reqs, h->no_req, sizeof(Reqinfo), comp);

 requisitos = open_req(h);
 
 for(i=1; i <= h->no_req;++i)  
   if(reqs[i].heuristic != 0.0)
     {
     set_bit(reqs[i].req_no,&(h->sel_reqs));
     if(!strcmp(h->criterion,"nos"))
       {
       fprintf(resultsfile,"%d) %2.3f %d\n",i+1,reqs[i].heuristic,reqs[i].req_no);
       set_bit(reqs[i].req_no, &(h->sel_nodes));
       }
     else
       {
       ptr_path  = pega_path(reqs[i].req_no,requisitos);
       fprintf(resultsfile,"%d) %2.3f %s\n",i+1,reqs[i].heuristic,apont_next_tok(ptr_path));
       set_sel_nodes(h,apont_next_tok(ptr_path));
       free(ptr_path);
       }
     }
 
 fclose(requisitos);
}

gen_nodes_results(h, hname)
     heur *h;
     char *hname;
{
  char results_name[1000], stmt_name[1000];
  int i, node, no_stmt;
  FILE * resultsfile, * stmtfile;
 
 /* create the vector with statement count */

  h->stmt_counter = (int *) malloc(sizeof(int) * (h->no_nos+1));
  check_mem_error((void*)h->stmt_counter);

  h->stmt_counter[0]=0;
  h->stmt_reveal = h->stmt_prone = 0;

  strcpy(results_name,h->func_dir);
  strcat(results_name,"/");
  strcat(results_name,hname);
  strcat(results_name,"-");
  strcat(results_name,h->func_dir);
  strcat(results_name,"-nodes.tes");

  resultsfile = (FILE *) fopen(results_name,"w");
  if(resultsfile == NULL)
    {
    fprintf(stderr,"pokeheuristics Error: it was not possible to open file %s\n",results_name);
    exit(1);
    }

  strcpy(stmt_name,h->func_dir);
  strcat(stmt_name,"/");
  strcat(stmt_name,h->func_dir);
  strcat(stmt_name,".stm");

  stmtfile = (FILE *) fopen(stmt_name,"r");
  if(stmtfile == NULL)
    {
    fprintf(stderr,"pokeheuristics Error: it was not possible to open file %s\n",stmt_name);
    exit(1);
    }

  while(!feof(stmtfile))
    {
      fscanf(stmtfile,"%d %d\n",&node,&no_stmt);
      if(node >=1 && node <= h->no_nos)
	{
	if(test_bit(node,&(h->sel_nodes)))
	  {
	    fprintf(resultsfile,"%3d %3d\n",node, no_stmt );
	    h->stmt_counter[node]=no_stmt;
	    h->stmt_prone+=no_stmt;
	  }
	}
      else
	{
	  fprintf(stderr,"pokeheuristics Error: incorrect node number\n"); 
	  exit(1);
	}
    }

 for(i=1; i <= h->no_nos;++i)  
   {
   if(test_bit(i,&(h->fault_nodes)))
     h->stmt_reveal+=h->stmt_counter[i];
   }

  fclose(resultsfile);
  fclose(stmtfile);
}

int main(argc,argv)
int argc;
char * argv[];
{

  char * results_name;
  int i, j,  seq_i, seq_j;
  TCinfo * tcases;
  Reqinfo * reqs;
  heur H;
  FILE * resultsfile;

  if(argc < 10)
    {
     fprintf(stderr,"heuristics -h<heuristic name> -d<function dir> -c<criterion> [-F<fault-revealing testing features file> -N<faulty nodes file>] -testset <# of test cases> -fail <sequence of fail test cases> [-random -fail <# of fail test cases> -succ <# of succ test cases>]\n");
      fprintf(stderr,"Wrong number of parameters\n");
      exit(1);  
    }

  get_heuristic_data(&H,argc,argv); 

  /* set heuristic sets */

  b_vector_cons(&(H.dice_fail),H.no_test_cases/BYTE+1,0);
  b_vector_cons(&(H.dice_succ),H.no_test_cases/BYTE+1,0);
                
  /* allocate space for test case info */
 
  tcases = (TCinfo *) malloc(sizeof(TCinfo)*(H.no_test_cases+1));
  check_mem_error ((void *) tcases);
 
  get_testset_data(tcases,&H,argc,argv);
  
  /* allocate space for requirements info */

  reqs = (Reqinfo *) malloc(sizeof(Reqinfo)*(H.no_req+1));
  check_mem_error ((void *) reqs);

  /* determine heuristics */

  get_req_data(&H,tcases,reqs,argc,argv);

  /* open file of results */

  results_name = malloc(2*strlen(H.func_dir)+strlen(argv[1]+2)+strlen("-results")+strlen(".tes")+20);
  check_mem_error ((void *) results_name);
  
  strcpy(results_name,H.func_dir);
  strcat(results_name,"/");
  strcat(results_name,argv[1]+2);
  strcat(results_name,"-");
  strcat(results_name,H.func_dir);
  strcat(results_name,"-results.tes");

  resultsfile = (FILE *) fopen(results_name,"w");
  if(resultsfile == NULL)
    {
    fprintf(stderr,"pokeheuristics Error: it was not possible to open file %s\n",results_name);
    exit(1);
    }
  
  switch(H.number)
    {
    case 1:
      if(H.type == SIMPLE)
        H1(&H,tcases,reqs,compare_rank_heur, resultsfile);
      else /* FREQ */
        H1(&H,tcases,reqs,compare_rank_heur_freq, resultsfile);
      break;      
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
      if(H.type == SIMPLE)
        {
        if(H.fail_random)
          { /* Random selection of test cases */
          RandomDiceHeur(&H,tcases,reqs,compare_rank_heur, resultsfile); 
          }
        else
          {
          DiceHeur(&H,tcases,reqs,compare_rank_heur, resultsfile);
          }
        }
      else
        {
        if(H.fail_random)
          { /* Random selection of test cases */
          RandomDiceHeur(&H,tcases,reqs,compare_rank_heur_freq, resultsfile); 
          }
        else
          {
          DiceHeur(&H,tcases,reqs,compare_rank_heur_freq, resultsfile);
          }
        }
      
      break;
    case 7:
      if(H.type == SIMPLE)
        RankingHeur(&H,reqs,compare_rank_heur, resultsfile);
      else /* FREQ */
        RankingHeur(&H,reqs,compare_rank_heur_freq, resultsfile);
      break;
    case 8:

      if(H.type == SIMPLE)
        RankingHeur2(&H,reqs,compare_rank_heur, resultsfile);
      else /* FREQ */
        RankingHeur2(&H,reqs,compare_rank_heur_freq, resultsfile);

    case 9:

      if(H.type == SIMPLE)
        RankingHeur3(&H,reqs,compare_rank_heur, resultsfile);
      else /* FREQ */
        RankingHeur3(&H,reqs,compare_rank_heur_freq, resultsfile);


      break;
    }
 
  if(H.exper)
    { /* Info experiment mode */
    inter_b_vector(&(H.fault_reveal),&(H.sel_reqs));
    inter_b_vector(&(H.fault_nodes),&(H.sel_nodes));

    gen_nodes_results(&H,argv[1]+2);

    fprintf(resultsfile,"Fault-revealing-statements: %d\n",H.stmt_reveal);
    fprintf(resultsfile,"Fault-prone-statements: %d\n",H.stmt_prone);
    fprintf(resultsfile,"Fault-revealing testing features: %d\n",count_b_vector(&(H.fault_reveal)));
    fprintf(resultsfile,"Fault-prone testing features: %d\n",count_b_vector(&(H.sel_reqs)));
    }

  fclose(resultsfile);
 
  return 0;
}
