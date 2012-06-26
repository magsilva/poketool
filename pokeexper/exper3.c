#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "util.h"
#include "newpokelib.h"
#include "hexper.h"
#include "haval.h"

void  GetScenFixedPars (sc,pargcounter,argv)
     Scenario * sc;
     int * pargcounter;
     char * argv[];
{
  int argc;

  argc = *pargcounter;

  if((sc->scenario=get_scenario(argv[argc]))==0) 
    error_msg(invalid_criterion);
  ++argc;

  /* Number of Tentatives */

  sc->no_trials = atoi(argv[argc]);
  ++argc;

 /* Get number of different test sets */

  sc->difftestsets = atoi(argv[argc]);
  ++argc;
 
  /* Get total number of test cases */

  sc->sizetestpool = atoi(argv[argc]);
  ++argc;

  /* Set info about the outputs to be determined */

  if(!strncmp(argv[argc],"-o", 2))
    {
      sc->output_file = strdup(argv[argc]+2);
      if(sc->output_file == NULL)
	error_msg(mem_fault);
      ++argc;
   
      if(!strncmp(argv[argc],"-v", 2))
	sc->fault_version=atoi(argv[argc]+2);
      else
	{
	  fprintf(stderr,"pokeexper: it is missing the number of the faulty version\n");
	  exit(1);
	}
      ++argc;
    }
  else
    {
      fprintf(stderr,"pokeexper: test report file is missing\n");
      exit(1);
    }

  /* Get info about the outputs */

   b_vector_cons(&(sc->output_info),sc->sizetestpool/BYTE+1,0);
   get_info_output(sc->output_file,&(sc->output_info),sc->fault_version,sc->sizetestpool);

 *pargcounter=argc;
}

void GetScenCritPars(sc,pargcounter,argv) 
     Scenario * sc;
     int * pargcounter;
     char * argv[];
{
  int argc;
  argc = *pargcounter;
     
  if(get_crit_number(argv[argc]+2)==0)       
    error_msg(invalid_criterion);     
  else       
    sc->criterion=argv[argc]+2;
  argc++;
   
  /* Get desired coverage */
   
  sc->desiredcover = atoi(argv[argc]);
  ++argc;

  /* Get the functions Name */
 
  sc->funcs = GetFuncsNames(argv,&argc);

  *pargcounter=argc;
}

void InitDSCrit(sc)
     Scenario * sc;
{
  sc->testscover = InitializeMaxCoverage(sc->funcs,sc->criterion,sc->sizetestpool,&(sc->maxcover));

  if(sc->desiredcover > 0)
    { /* Check if desired cover is feasible */
      /* Search sets for a given coverage */
      if(!IsDesiredCoverage(sc->funcs,sc->maxcover,sc->desiredcover))
	{
	  fprintf(stderr,"pokeexper: desired coverage is unfeasible\n");
	  exit(1);
	}
    }
  else
    /* Search sets for the maximum coverage */
    sc->desiredcover = CurrentCoverage(sc->maxcover,sc->funcs);

 /*
    Initialize Data Strutures to store, respectively, the current total 
    coverage of the selected test cases and the coverage of a particular test
    case (testn).
 */

 sc->curcover   = InitializeCurCoverage(sc->funcs,sc->criterion);
 sc->testncover = InitializeCurCoverage(sc->funcs,sc->criterion);

}

void CheckFaultDetectionAbility(sc)
     Scenario *sc;
{
  int i,k,j,tot_fail=0, tot_certo=0;
  element * laux;

  printf("\nCheckfault detection  ability\n\n");
     
  for(k=0,laux = sc->funcs; laux != NULL; k++,laux=laux->next)
       {
	 printf("\n* * %s * *\n",laux->name);
	 for(j=0; j <= sc->maxcover[k].maxfeatures; ++j)
	   {
	     tot_fail=tot_certo=0;
	     if(!test_bit(j,&(sc->maxcover[k].features)))
	       continue;

	     for(i=1; i <= sc->sizetestpool; ++i)
	       if(!test_bit(i, &(sc->output_info)))
		 if(test_bit(j,&((sc->testscover[i][k]).features)))
		   ++tot_certo;

	     printf("total succ %d: %d\n",j,tot_certo);

	     for(i=1; i <= sc->sizetestpool; ++i)
	       if(test_bit(i, &(sc->output_info)))
		 if(test_bit(j,&((sc->testscover[i][k]).features)))
		   ++tot_fail;

	     printf("total fail %d: %d\n",j,tot_fail);
    
	     printf("fault detection ability of %d: %1.3f\n\n", j, ((double)tot_fail)/(tot_fail+tot_certo));
	   }
       }
}

void PrintOutput(sc)
     Scenario *sc;
{
 /* Auxiliary vars */

 int i, setno, fault_reveal, no_test_cases; 
 double * vec_fault_reveal, * vec_no_test_cases, * vec_cov,
   tot_fault_reveal=0, tot_no_test_cases=0, tot_cov, cov_set;

 double eff_dev, eff_max, eff_min, acum_fault_reveal = 0;
 double size_dev, size_max, size_min, acum_no_test_cases = 0;
 double cov_dev, cov_max, cov_min, acum_cov = 0;

 char  outfilename[100], scriptfilename[100];
 FILE * outfile, *scriptfile;
     
 switch(sc->scenario)
   {
   case TPC:
     strcpy(outfilename,sc->criterion);    
     strcat(outfilename,"effect.tpc");
     strcpy(scriptfilename,sc->criterion);    
     strcat(scriptfilename,"script.tpc");
     break;
   case TPR:
     strcpy(outfilename,sc->criterion);    
     strcat(outfilename,"effect.tpr");
     strcpy(scriptfilename,sc->criterion);    
     strcat(scriptfilename,"script.tpr");
     break;
   case TPF:
     strcpy(outfilename,sc->criterion);    
     strcat(outfilename,"effect.tpf");
     strcpy(scriptfilename,sc->criterion);    
     strcat(scriptfilename,"script.tpf");
     break;

   case TGC:   
     strcpy(outfilename,sc->criterion);    
     strcat(outfilename,"effect.tgc");
     strcpy(scriptfilename,sc->criterion);    
     strcat(scriptfilename,"script.tgc");
     break;
   case TGR:
     strcpy(outfilename,sc->criterion);    
     strcat(outfilename,"effect.tgr");
     strcpy(scriptfilename,sc->criterion);    
     strcat(scriptfilename,"script.tgr");
     break;
   case TGDC:   
     strcpy(outfilename,sc->criterion);    
     strcat(outfilename,"effect.tgdc");
     strcpy(scriptfilename,sc->criterion);    
     strcat(scriptfilename,"script.tgdc");
     break;
   case TGDR:
     strcpy(outfilename,sc->criterion);    
     strcat(outfilename,"effect.tgdr");
     strcpy(scriptfilename,sc->criterion);    
     strcat(scriptfilename,"script.tgdr");
     break;
   case TGF:
     strcpy(outfilename,sc->criterion);    
     strcat(outfilename,"effect.tgf");
     strcpy(scriptfilename,sc->criterion);    
     strcat(scriptfilename,"script.tgf");
     break;
   case MTC:   
     strcpy(outfilename,sc->criterion);    
     strcat(outfilename,"effect.mtc");
     strcpy(scriptfilename,sc->criterion);    
     strcat(scriptfilename,"script.mtc");
     break;
   case MTR:
     strcpy(outfilename,sc->criterion);    
     strcat(outfilename,"effect.mtr");
     strcpy(scriptfilename,sc->criterion);    
     strcat(scriptfilename,"script.mtr");
     break;
   case MTF:
     strcpy(outfilename,sc->criterion);    
     strcat(outfilename,"effect.mtf");
     strcpy(scriptfilename,sc->criterion);    
     strcat(scriptfilename,"script.mtf");
     break;
   default:
     error_msg(invalid_criterion);
   }
     
 outfile  = fopen(outfilename,"w");
     
 if(outfile == NULL)
       {
       fprintf(stderr,"pokeexper: it was not possible to open file \"outfilename\"\n");
       exit(1);
       }

 scriptfile  = fopen(scriptfilename,"w");
     
 if(scriptfile == NULL)
   {
     fprintf(stderr,"pokeexper: it was not possible to open file \"outfilename\"\n");
     exit(1);
   }

 fprintf(scriptfile,"#!/bin/sh\n");       
 fprintf(scriptfile,"# Test Sets for Script Generation\n\n",sc->criterion);
    
 vec_fault_reveal = (double *) malloc(sizeof(double)*sc->difftestsets);
 if(vec_fault_reveal == NULL)
   error_msg(mem_fault);
  
 vec_no_test_cases = (double *) malloc(sizeof(double)*sc->difftestsets);
 if(vec_no_test_cases == NULL)
   error_msg(mem_fault);

 vec_cov = (double *) malloc(sizeof(double)*sc->difftestsets);
 if(vec_cov == NULL)
   error_msg(mem_fault);

 for(setno=0; setno <= sc->difftestsets-1; ++setno)
   {

   fault_reveal=0;   
   no_test_cases = count_b_vector(&(sc->selectdiffsets[setno].testcases));
   cov_set=(sc->selectdiffsets[setno]).coverage;

   /* Size info */
   
   tot_no_test_cases+=no_test_cases;
   vec_no_test_cases[setno]=no_test_cases;

   /* Coverage info */

   tot_cov+=cov_set;
   vec_cov[setno]=cov_set;

   /* Script info */
  
   fprintf(scriptfile,"\n -testset %d -fail ",no_test_cases);

   for(i=1; i <= sc->sizetestpool; ++i)
     {
      if(test_bit(i,&(sc->selectdiffsets[setno].testcases)))
         if(test_bit(i, &(sc->output_info)))	    
            {
            fault_reveal++;
            fprintf(scriptfile,"%d ",i);
	    }
     }

   fprintf(scriptfile,"-succ ");
   
   for(i=1; i <= sc->sizetestpool; ++i)
     {
      if(test_bit(i,&(sc->selectdiffsets[setno].testcases)))
          if(!test_bit(i, &(sc->output_info)))	    
            fprintf(scriptfile,"%d ",i);
     }
   
   fprintf(scriptfile,"\n");

   /* Effectiveness info */

   tot_fault_reveal+=fault_reveal;
   vec_fault_reveal[setno]=fault_reveal;

   /* Output Data */

   fprintf(outfile,"%d\t%d\t%d\t%3.2f\n",setno+1,fault_reveal,no_test_cases,cov_set);
   }
     
 /* Determine Standard deviation */
 /* Averages */
     
 tot_fault_reveal/=sc->difftestsets;
 tot_no_test_cases/=sc->difftestsets;
 tot_cov/=sc->difftestsets;

 eff_max = eff_min = vec_fault_reveal[0];
 size_max = size_min = vec_no_test_cases[0];
 cov_max = cov_min = vec_cov[0];
     
 for(setno=0; setno <= sc->difftestsets-1; ++setno)
   {
     acum_fault_reveal+=pow(vec_fault_reveal[setno]-tot_fault_reveal,2);
     acum_no_test_cases+=pow(vec_no_test_cases[setno]-tot_no_test_cases,2);
     acum_cov+=pow(vec_cov[setno]-tot_cov,2);

     eff_max = MAX(eff_max,vec_fault_reveal[setno]);
     eff_min = MIN(eff_min,vec_fault_reveal[setno]);
     size_max = MAX(size_max,vec_no_test_cases[setno]);
     size_min = MIN(size_min,vec_no_test_cases[setno]);
     cov_max = MAX(cov_max,vec_cov[setno]);
     cov_min = MIN(cov_min,vec_cov[setno]);
   }
     
 /* find deviation */
     
 eff_dev = sqrt(acum_fault_reveal/(sc->difftestsets-1));
 size_dev = sqrt(acum_no_test_cases/(sc->difftestsets-1));
 cov_dev = sqrt(acum_cov/(sc->difftestsets-1));
     
 fprintf(outfile,"\n* * Effectiveness Data * *\n");
 fprintf(outfile,"Average=%3.3f\n", tot_fault_reveal);
 fprintf(outfile,"Std deviation=%3.3f\n", eff_dev);
 fprintf(outfile,"Max=%3.3f Min=%3.3f\n", eff_max, eff_min);

 fprintf(outfile,"\n* * Size Data * *\n");
 fprintf(outfile,"Average=%3.3f\n", tot_no_test_cases);
 fprintf(outfile,"Std deviation=%3.3f\n", size_dev);
 fprintf(outfile,"Max=%3.3f Min=%3.3f\n", size_max, size_min);
 
 fprintf(outfile,"\n* * Coverage Data * *\n");
 fprintf(outfile,"Average=%3.3f\n", tot_cov);
 fprintf(outfile,"Std deviation=%3.3f\n", cov_dev);
 fprintf(outfile,"Max=%3.3f Min=%3.3f\n", cov_max, cov_min);
     
 free(vec_fault_reveal);
 free(vec_no_test_cases);
 free(vec_cov);
     
 fclose(outfile);
 fclose(scriptfile);
}

