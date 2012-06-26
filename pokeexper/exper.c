#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "util.h"
#include "newpokelib.h"
#include "hexper.h"
#include "haval.h"

/*
- Initialize the data structures;

-- Create DS to store the maximum number of features exercised by each 
   function;
*/


int get_number_features(filename)
char *filename;
{
  FILE * desc;
  char linha[1000];
  int no_req;
  
  desc = fopen(filename,"r");
  
  if(desc == NULL)
    {
    fprintf(stderr,"pokeexper: it was not possible to open file %s\n",filename);
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

void get_info_output(output_file,output_vector,fault_ver, sizetestpool)
char * output_file;
b_vector * output_vector;
int fault_ver;
int sizetestpool;
{
  FILE * outputf;
  
  char linha[MAXLINE], * ptr_line, *ptr_aux;
  int no_case, output_res;

  outputf = fopen(output_file,"r");

  if(outputf == NULL)
    {
    fprintf(stderr,"pokeexper: it was not possible to open file %s\n",output_file);
    exit(1);
    }
  
  while((ptr_line=le_linha_gen(outputf)) != NULL)
    {
    sscanf(ptr_line,"case-%d",&no_case);

    if(no_case < 1 || no_case > sizetestpool)
      error_msg(out_of_range);
          
    if(ptr_line == NULL)
      error_msg(wrong_file_desc);

    output_res = *(ptr_line+8+2*fault_ver) - '0';

    if(output_res)
       set_bit(no_case, output_vector);
    
    free(ptr_line);
    }
  
  fclose(outputf);
}

void read_features(freq,bv)
FILE * freq;
b_vector * bv;
{
 int exec_req, freq_exec, freq_tot, func_invoc, seq_i, seq_j;
 while(!feof(freq))
      {
      fscanf(freq,"%d (%d/%d) (%d %d/%d)\n",&exec_req,&freq_exec,&freq_tot,
             &func_invoc,&seq_i,&seq_j);
      set_bit(exec_req,bv);
      }
 fclose(freq);    
}

void read_cases_features(cases, testncases,sizetestpool,no_func)
FILE * cases;
Coverage ** testncases;
int sizetestpool;
int no_func;
{
  char linha[MAXLINE], * ptr_features_line, *ptr_aux;
  int no_case, feature;
  
  while(le_linha_str_exper(linha,"Case ",cases) != NULL)
    {
    sscanf(linha,"Case %d",&no_case);

    if(no_case < 1 || no_case > sizetestpool)
      error_msg(out_of_range);
    
    ptr_aux=ptr_features_line=le_linha_gen(cases);

    if(ptr_features_line == NULL)
      error_msg(wrong_file_desc);

    while(ptr_features_line != NULL)
      {
      ptr_features_line = le_no_feature(ptr_features_line,&feature);
      if(feature != -1)
	if(feature >= 1 && feature <= testncases[no_case][no_func].maxfeatures)
          set_bit(feature, &(testncases[no_case][no_func].features));
        else
          error_msg(out_of_range);
      }
    free(ptr_aux);
    }
}

Coverage ** InitializeMaxCoverage(l_funcs,criterion,sizetestpool,mcover)
element * l_funcs;
char * criterion;
int sizetestpool;
Coverage ** mcover;
{
  int i, j, no_funcs, maxfeatures, exec_features, totmaxfeatures, totexec_features;
  Coverage * maxcover, ** testscover;
  element * laux;
  char straux[1000], straux2[100];
  FILE * cases;
  
  no_funcs = list_info_size(l_funcs);

  testscover = (Coverage **) malloc(sizeof(Coverage *)*(sizetestpool+1));
  if(testscover == NULL)
    error_msg(mem_fault);

  totmaxfeatures=totexec_features=0;

  for(j=1; j <= sizetestpool; ++j)
     {
     testscover[j]= (Coverage *) malloc(sizeof(Coverage)*(no_funcs));

     if(testscover[j] == NULL)
        error_msg(mem_fault);
     }      

  maxcover = (Coverage *) malloc(sizeof(Coverage)*(no_funcs));

  if(maxcover == NULL)
    error_msg(mem_fault);
    
  printf("--------------------------------------+\n");
  printf("| Maximum Coverage for %-4s Criterion |\n",criterion);
  printf("--------------------------------------+\n");
  
  for(i=0,laux = l_funcs; laux != NULL; i++,laux=laux->next)
    {
      /* get the executed features */
      
      maxcover[i].criterion = get_crit_number(criterion);
      maxcover[i].funcname = strdup(laux->name);
      
      if(maxcover[i].funcname == NULL)
        error_msg(mem_fault);

      strcpy(straux,"./");
      strcat(straux,laux->name);
      strcat(straux,"/");
      
      get_des_file(straux2,criterion);
      strcat(straux,straux2);

      maxfeatures = get_number_features(straux);
      totmaxfeatures+=maxfeatures;

      b_vector_cons(&(maxcover[i].features),maxfeatures/BYTE+1,0);

      maxcover[i].maxfeatures=maxfeatures;
      
      strcpy(straux,"./");
      strcat(straux,laux->name);
      strcat(straux,"/");

      get_cases_file(straux2,criterion);
      strcat(straux,straux2);
      cases = fopen(straux,"r");

      if(cases == NULL)
        {
        fprintf(stderr, "pokeexper: Msg: file %scases.tes of executed features for function %s is missing\n",criterion, laux->name);
        exit(1);
        }
          
      for(j=1; j <= sizetestpool; ++j)
        {
        (testscover[j][i]).criterion = get_crit_number(criterion);
        (testscover[j][i]).funcname = strdup(laux->name);

        if((testscover[j][i]).funcname == NULL)
          error_msg(mem_fault);

        b_vector_cons(&((testscover[j][i]).features),maxfeatures/BYTE+1,0);
        (testscover[j][i]).maxfeatures=maxfeatures;
        }

      read_cases_features(cases,testscover,sizetestpool,i);
      
      fclose(cases);

      for(j=1; j <= sizetestpool; ++j)
          union_b_vector(&(maxcover[i].features),&((testscover[j][i]).features));

      exec_features = count_b_vector(&(maxcover[i].features));
      totexec_features+=exec_features;

      printf("| %-32s | %4d/%-4d (%4.2f%%)      |\n",laux->name, exec_features,maxfeatures,((double)exec_features/maxfeatures)*100);

    }
 printf("--------------------------------------+\n");
      printf("| %-32s | %4d/%-4d (%4.2f%%)      |\n","Total", totexec_features,totmaxfeatures,((double)totexec_features/totmaxfeatures)*100);
 printf("--------------------------------------+\n");

 *mcover=maxcover;
 
 return(testscover);
}

Coverage * InitializeCurCoverage(l_funcs,criterion)
element * l_funcs;
char * criterion;
{
  int i, no_funcs, maxfeatures;
  Coverage * curcover;
  element * laux;
  char straux[1000],straux2[100];
  FILE  * info;
  
  no_funcs = list_info_size(l_funcs);

  curcover = (Coverage *) malloc(sizeof(Coverage)*(no_funcs));

  for(i=0,laux = l_funcs; laux != NULL; i++,laux=laux->next)
    {
      /* get the executed features */
      
      curcover[i].criterion = get_crit_number(criterion);
      curcover[i].funcname = strdup(laux->name);
      
      if(curcover[i].funcname == NULL)
        error_msg(mem_fault);

      strcpy(straux,"./");
      strcat(straux,laux->name);
      strcat(straux,"/");
      
      get_des_file(straux2,criterion);
      strcat(straux,straux2);

      curcover[i].maxfeatures = maxfeatures = get_number_features(straux);
    
      b_vector_cons(&(curcover[i].features),maxfeatures/BYTE+1,0);
    }
  
 return(curcover);
}

void RefreshCurCoverage(curcover, l_funcs)
Coverage * curcover;
element * l_funcs;
{
 int i;
 element * laux;
 for(i=0,laux = l_funcs; laux != NULL; i++,laux=laux->next)
    reset_all(&(curcover[i].features));
}

Coverage * GetTestCoverage(tcover, testscover,testN, l_funcs)
Coverage * tcover;
Coverage ** testscover;
int testN;
element * l_funcs;
{
 int i;
 element * laux;
 
 RefreshCurCoverage(tcover, l_funcs);
 
 for(i=0,laux = l_funcs; laux != NULL; i++,laux=laux->next)
     b_vector_cpy(&(tcover[i].features),&((testscover[testN]+i)->features));

 return (tcover);
}

double CurrentCoverage(curcoverage,l_funcs)
Coverage * curcoverage;
element * l_funcs;
{
 int i;
 element * laux;
 int tot_features=0;
 int exec_features=0;
 double cover=0.0;
 
 for(i=0,laux = l_funcs; laux != NULL; i++,laux=laux->next)
   {
   tot_features += curcoverage[i].maxfeatures;
   exec_features += count_b_vector(&(curcoverage[i].features));
   }

 cover = ((double)exec_features)/((double)tot_features)*100;
 return cover;
}

int IsCoverageImproved(curcover,testcover,l_funcs)
Coverage * curcover, * testcover;
element * l_funcs;
{
 int ret = 0, i;
 element * laux;
 b_vector acc;
 
 for(i=0,laux = l_funcs; laux != NULL; i++,laux=laux->next)
    {
    b_vector_cons(&acc,curcover[i].features.nbytes,0);
    b_vector_cpy(&acc,&(curcover[i].features));
    union_b_vector(&acc,&(testcover[i].features));
    if(!is_equal_b_vector(&acc,&(curcover[i].features)))
      {
      ret = 1;
      b_vector_destroy(&acc);
      break;
      }
    b_vector_destroy(&acc);
    }
 return ret;
}

void AddFeatures(curcover,testcover,l_funcs)
Coverage * curcover, * testcover;
element * l_funcs;
{
 int ret = 0, i;
 element * laux;
 b_vector acc;
 
 for(i=0,laux = l_funcs; laux != NULL; i++,laux=laux->next)
    union_b_vector(&(curcover[i].features),&(testcover[i].features));
    
}

/*
-- Create DS containing the sets of selected test cases (1..DiffTestSets);

------ SelectDiffSets (1..DiffTestSets);
------ Bitvector Seltests;
*/

Testset * InitializeSelectDiffSets(difftestsets,sizetestpool)
int difftestsets;
int sizetestpool;
{
 int i;
 Testset * selectdiffsets;

 selectdiffsets = (Testset *) malloc(sizeof(Testset)*difftestsets);

 for(i=0; i < difftestsets; ++i)
   {
   selectdiffsets[i].no_test_set = i;
   selectdiffsets[i].coverage = 0;
   b_vector_cons(&(selectdiffsets[i].testcases),sizetestpool/BYTE+1,0);
   }
 
 return (selectdiffsets);
}

void SetTestNSelectDiffSets(selectdiffsets,setno,testn,difftestsets,sizetestpool)
Testset * selectdiffsets;
int setno;
int testn;
int difftestsets;
int sizetestpool;
{
  if(testn < 0 && testn > sizetestpool-1)
    error_msg(out_of_range);
      
  if(setno < 0 && setno > difftestsets-1)
    error_msg(out_of_range);

  set_bit(testn,&(selectdiffsets[setno].testcases));
}

int IsDesiredCoverage(l_funcs,curcoverage,desiredcover)
element * l_funcs;
Coverage * curcoverage;
double desiredcover;
{
 int i;
 element * laux;
 int tot_features=0;
 int exec_features=0;
 double cover=0.0;
 
 for(i=0,laux = l_funcs; laux != NULL; i++,laux=laux->next)
   {
   tot_features += curcoverage[i].maxfeatures;
   exec_features += count_b_vector(&(curcoverage[i].features));
   }

 cover = ((double)exec_features)/((double)tot_features)*100;

 if(cover >= desiredcover)
   return 1;
 else
   return 0;
}

int IsEqual(selectdiffsets,setno,difftestsets)
Testset * selectdiffsets;
int setno;
int difftestsets;
{
 int i;
 int ret = 0;
 
 if(setno < 0 && setno > difftestsets-1)
   error_msg(out_of_range);

 for(i=0; i < setno; ++i)
   {
   if(is_equal_b_vector(&(selectdiffsets[i].testcases),
                        &(selectdiffsets[setno].testcases)))
     {
     ret = 1;
     break;
     }
   }
 return ret;
}

int IsThereAnFaultRevealingTestCase(selectdiffsets,outputinfo,setno,difftestsets,testpoolsize)
Testset * selectdiffsets;
b_vector * outputinfo;
int setno;
int difftestsets;
int testpoolsize;
{
 int i;
 int ret = 0;
 
 if(setno < 0 && setno > difftestsets-1)
   error_msg(out_of_range);

 for(i=1; i <= testpoolsize; ++i)
   {
   if(test_bit(i,&(selectdiffsets[setno].testcases)))
     {
       if(test_bit(i,outputinfo))
	 {
	   ret = 1;
	   break;
	 }
     }
   }
 return ret;
}

/*
--- Select randomly a test case(SelectDiffSets[i] (1..Sizetestpool):testN;

*/

int SelectRandomTestCase(selectdiffsets,setno,sizetestpool,difftestsets)
Testset * selectdiffsets;
int setno;
int sizetestpool;
int difftestsets; 
{
 int testn, isdifferent;
 
 if(setno < 0 && setno > difftestsets-1)
   error_msg(out_of_range);

 do
   {
   testn = (rand()%sizetestpool)+1;
   if(test_bit(testn,&(selectdiffsets[setno].testcases)))
     isdifferent=0;
   else
     isdifferent=1;
   }
 while(!isdifferent);
 
 return testn;
}

/*
  
Input:
------

- Program:

- list of functions;

- Desired coverage: DesiredCover;

- Number of different sets: DiffTestSets;

- Number maximum of test cases: Sizetestpool;

Output:
-------

- Set of test cases with specified coverage;
  (sequence of test cases)
*/


element * GetFuncsNames(argv, index)
char * argv[];
int *index;
{
 element * l_funcs=NULL;

 while(argv[*index] != NULL && !strncmp(argv[*index],"-f",2))
   {
   l_funcs = insert_list_vec_names(l_funcs,argv[*index]+2);
   ++*index;
   }
 return l_funcs;
}

int main(argc, argv)
int argc;
char * argv[];
{
 /* Local Vars */

 Scenario Scen;
 
 /* Auxiliary vars */

 int argcounter; 
 
 /*
    Check the number of input parameters
 */

 if(argc < 4)
   {
   fprintf(stderr,"pokeexer -scenario notrials diffsets testpoolsize -o<output results> -v<faulty version> [testsetsize] -ccriterion coverage  -ffuncname1 [-ffunc2 ... -ffuncn] \n");
   fprintf(stderr,"notrials : maximum number of trials to achieve test sets\n");
   fprintf(stderr,"diffsets : number of different test sets\n");
   fprintf(stderr,"testpoolsize: number of test cases of the testing pool\n");
   fprintf(stderr,"<output results>: name of the file with the results of the validation of the test cases\n");
   fprintf(stderr,"<faulty version>: number of the faulty version being analyzed\n");
   fprintf(stderr,"testsetsize: number of test cases of every test set\n");
   fprintf(stderr,"-criterion: one of the following criteria:\n");
   fprintf(stderr,"\tpdu  : all potential du-paths\n"); 
   fprintf(stderr,"\tpu   : all potential uses\n"); 
   fprintf(stderr,"\tpudu : all potential uses/du\n"); 
   fprintf(stderr,"\tnos  : all nodes\n"); 
   fprintf(stderr,"\tarcs : all arcs\n"); 
   fprintf(stderr,"\tpuses: all p-uses\n"); 
   fprintf(stderr,"\tuses : all uses\n"); 
   fprintf(stderr,"\tdu   : all du-paths\n"); 
   fprintf(stderr,"\trand : random testing\n");
   fprintf(stderr,"coverage : desired coverage of the test sets (if < 0, search for sets with maximum coverage)\n");
   fprintf(stderr,"-ffuncname1 [-ffunc2 ... -ffuncN]: function names\n");
   exit(1);
   }

 /* Get fixed parameters */

  argcounter=1;
  GetScenFixedPars(&Scen,&argcounter,argv);
 
 /*
    Initialize Data Structure that store the selected test cases for each
    test set.
 */

 Scen.selectdiffsets = InitializeSelectDiffSets(Scen.difftestsets,Scen.sizetestpool);
 
 /* Depending on the scenario type, get the size of the set */

 switch(Scen.scenario)
   {
   case TGR:
   case TGF:
     Scen.testsetsize = atoi(argv[argcounter]);
     ++argcounter;
     if(Scen.testsetsize > Scen.sizetestpool)
       {
	 fprintf(stderr,"pokeexper: desired size of the random test set is unfeasible\n");
	 exit(1);
       }     
     /* Get criterion info */

     GetScenCritPars (&Scen,&argcounter,argv);       
     InitDSCrit(&Scen);
     TGRandomScenario(&Scen);
     break;

   case TGDR:
     Scen.testsetsize = atoi(argv[argcounter]);
     ++argcounter;
     if(Scen.testsetsize > Scen.sizetestpool)
       {
	 fprintf(stderr,"pokeexper: desired size of the random test set is unfeasible\n");
	 exit(1);
       }     
     /* Get criterion info */

     GetScenCritPars (&Scen,&argcounter,argv);       
     InitDSCrit(&Scen);
     TGDebugRandomScenario(&Scen);
     break;

   case TGC:
     /* Get criterion info */
 
     GetScenCritPars (&Scen,&argcounter,argv);       
     InitDSCrit(&Scen);
     TGStructuralCriterionScenario(&Scen);

     break;

   case TGDC:
     /* Get criterion info */
 
     GetScenCritPars (&Scen,&argcounter,argv);       
     InitDSCrit(&Scen);
     TGDebugStructuralCriterionScenario(&Scen);

     break;

   case TPF:
   case TPR:
     /* Get criterion info */

     GetScenCritPars (&Scen,&argcounter,argv);       
     InitDSCrit(&Scen);
     TPRandomScenario(&Scen);
     break;

   case TPC:
      /* Get criterion info */
 
     GetScenCritPars (&Scen,&argcounter,argv);       
     InitDSCrit(&Scen);
     TPStructuralCriterionScenario(&Scen);

     break;
  case MTC:
     /* Get criterion info */
 
     GetScenCritPars (&Scen,&argcounter,argv);       
     InitDSCrit(&Scen);
     MTStructuralCriterionScenario(&Scen);

     break;
   case MTF:
   case MTR:
     Scen.testsetsize = atoi(argv[argcounter]);
     ++argcounter;
     if(Scen.testsetsize > Scen.sizetestpool)
       {
	 fprintf(stderr,"pokeexper: desired size of the random test set is unfeasible\n");
	 exit(1);
       } 
     /* Get criterion info */
 
     GetScenCritPars (&Scen,&argcounter,argv);       
     InitDSCrit(&Scen);
     MTRandomScenario(&Scen);

     break;

   default: error_msg(invalid_criterion);
   }

 CheckFaultDetectionAbility(&Scen);
 PrintOutput(&Scen);

 return 0;
}










