/* Type to store Information about a Testing Requirement */

typedef struct test_req {
  int classif;
  int type;
  int criterion;
  int reqno;
  int nodef;
  int nouse;
  int nopred;
  int nosuc;
   
  /* First dua occurrence */

  long seq_exec_i;     /* posicao do no i na sequencia de execucao */
  long seq_exec_j;     /* posicao do no j na sequencia de execucao */
  long no_func_invoc;  /* numero da invocacao da funcao */

  /* Last dua occurrence */

  long seq_exec_i_last;     /* posicao do no i na sequencia de execucao */
  long seq_exec_j_last;     /* posicao do no j na sequencia de execucao */
  long no_func_invoc_last;  /* numero da invocacao da funcao */

  long freq; /* count of the dua */
} TestReq; 

/* Type to Store Information about a Function */

typedef struct func_info {
  char * funcname;
  NODEINFO info_no;
  int no_vars;
  int no_nos;
  struct grafo * graph;
  struct grafo * graphaux;
  Hash tab_var;
  reHash * retab_var;
  l_info * entry_points;
  l_info * exit_points;
  TestReq * pusereq;
  int nopuses;
  int testcasepuses;
  long instancepuses;
  TestReq * cusereq;
  int nocuses;
  int testcasecuses;
  long instancecuses;
  TestReq * pureq;
  int nopus;
  int testcasepus;
  long instancepus;
  TestReq * pudureq;
  int nopudus;
  int testcasepudus;
  long instancepudus;
  struct func_info * next;
} FuncInfo;


int 
ReadTestReq(funcinfo,crit)
     FuncInfo * funcinfo;
     int crit;
{
 /* Local Vars Decls */

  int req_no, no_req, err_ret=-1;
  char desfile[200],reqfile[200], *ptrstr;
  FILE * desc, *req;
  TestReq * testreq;

  strcpy(desfile,"./");
  strcat(desfile,funcinfo->funcname);
  strcat(desfile,"/");
  strcpy(reqfile,"./");
  strcat(reqfile,funcinfo->funcname);
  strcat(reqfile,"/");

  switch(crit)
    {
    case PU:
      if(funcinfo->pureq != NULL)
	return 0;
      strcat(desfile,"des_pu.tes");
      strcat(reqfile,"puassoc.tes");
      break;
    case PUSOS:
      if(funcinfo->pusereq != NULL)
	return 0;
      strcat(desfile,"des_p_uses.tes");
      strcat(reqfile,"all_p_uses.tes");
      break;
    case USOS:
      if(funcinfo->cusereq != NULL)
	return 0;
      strcat(desfile,"des_all_uses.tes");
      strcat(reqfile,"all_uses.tes");
      break;
    default: return invalid_criterion;
    }
 
  /* Get the number of testing requirements */

   desc = fopen(desfile,"r");
  
  if(desc == NULL)
    return  cannot_open_file;

  /*
   position in the end of the descriptor file less a certain quantity of bytes
  */
 
 fseek(desc, (long) -50, SEEK_END);
 
 /* read number of requeriments */

 ptrstr = get_line_gen("Numero Total de Descritores =", desc, &err_ret);
 if(err_ret != -1)
     return err_ret;
 
 sscanf(ptrstr,"Numero Total de Descritores = %d", &no_req);
 free(ptrstr);

 /* Create vector of requirements */

 fclose(desc);

 /* Allocate space for the requirements */

 testreq = (TestReq *) malloc(no_req*sizeof(TestReq));

 /* Get the requirement description */
 
 req = fopen(reqfile,"r");
  
 if(req == NULL)
      return cannot_open_file;

 err_ret = -1;
 for(req_no=0; (req_no < no_req); ++req_no)
   {
     char * auxstr;  
     auxstr = ptrstr = get_line_gen("<", req, &err_ret);
     if(err_ret != -1)
       {
	 return err_ret;
       }
     /* Initialize testreq[req_no] */
     /* Null values */
     testreq[req_no].reqno = req_no;
     testreq[req_no].type = -1;
     testreq[req_no].varlist = NULL;
     testreq[req_no].nodef = testreq[req_no].nouse = 
     testreq[req_no].nopred = testreq[req_no].nosuc = -1;
     testreq[req_no].seq_exec_i = testreq[req_no].seq_exec_j = 
     testreq[req_no].seq_exec_i_last = testreq[req_no].seq_exec_j_last = 
     testreq[req_no].no_func_invoc = testreq[req_no].no_func_invoc_last = -1;
     testreq[req_no].criterion = crit;
     testreq[req_no].freq = 0;

     free(auxstr);
   }

 fclose(req);
 switch(crit)
    {
    case PU:
      funcinfo->pureq = testreq;
      funcinfo->nopus = no_req;
      break;
    case PUSOS:
      funcinfo->pusereq = testreq;
      funcinfo->nopuses = no_req;
      break;
    case USOS:
      funcinfo->cusereq = testreq;
      funcinfo->nocuses = no_req;
      break;
    }

 return -1;
}

int 
ReadTestCaseInfo(funcinfo,testcaseno,crit)
     FuncInfo * funcinfo;
     int testcaseno;
     int crit;
{
 /* Local Vars Decls */

  int req_no, no_req, ret=-1, *curtestcase, parsread;
  long * noinstances;
  char freqfile[200], number[20];
  FILE * test_freq;
  TestReq * testreq;

  if(testcaseno <= 0)
    {
      printf_filtered("No test case has been set. Test case 1 will be loaded.\n");
      testcaseno=1;
      settestcase_command1(testcaseno);
    }

  strcpy(freqfile,"./");
  strcat(freqfile,funcinfo->funcname);
  strcat(freqfile,"/");

  switch(crit)
    {
    case PU:
      if(funcinfo->pureq == NULL)
	return 0;
      testreq = funcinfo->pureq;
      no_req = funcinfo->nopus;
      curtestcase = &(funcinfo->testcasepus);
      noinstances = &(funcinfo->instancepus);

      strcat(freqfile,"pufreq");
      sprintf(number,"%d",testcaseno);
      strcat(freqfile,number);
      strcat(freqfile,".tes");
      break;
    case PUSOS:
      if(funcinfo->pusereq == NULL)
	return 0;
      testreq = funcinfo->pusereq;
      no_req = funcinfo->nopuses;
      curtestcase = &(funcinfo->testcasepuses);
      noinstances = &(funcinfo->instancepuses);

      strcat(freqfile,"pusesfreq");
      sprintf(number,"%d",testcaseno);
      strcat(freqfile,number);
      strcat(freqfile,".tes");
      break;
    case USOS:
      if(funcinfo->cusereq == NULL)
	return 0;
      testreq = funcinfo->cusereq;
      no_req = funcinfo->nocuses;
      curtestcase = &(funcinfo->testcasecuses);
      noinstances = &(funcinfo->instancecuses);

      strcat(freqfile,"usesfreq");
      sprintf(number,"%d",testcaseno);
      strcat(freqfile,number);
      strcat(freqfile,".tes");
      break;
    default: return invalid_criterion;
    }
 
  /* Get the number of testing requirements */

  test_freq = fopen(freqfile,"r");
  
  if(test_freq == NULL)
    {
      printf_filtered("There is no information for test case %d.\n",testcaseno);
      return  cannot_open_file;
    }

  parsread=fscanf(test_freq,"Numero total de instancias de requisitos = %d\n",noinstances);
  if(parsread != 1)
    return wrong_file_desc;
  *curtestcase = testcaseno;

  for(req_no=0; req_no < no_req; ++req_no)
    {
     testreq[req_no].seq_exec_i = testreq[req_no].seq_exec_j = 
     testreq[req_no].seq_exec_i_last = testreq[req_no].seq_exec_j_last = 
     testreq[req_no].no_func_invoc = testreq[req_no].no_func_invoc_last = -1;
    }

  while((parsread=fscanf(test_freq,"%d",&req_no)) != EOF)
    {
      if(parsread != 1)
	return wrong_file_desc;
      if(req_no <= 0 || req_no > no_req)
	return wrong_file_desc;
      req_no--;
      parsread = fscanf(test_freq," %d (%d %d/%d)", 
			&(testreq[req_no].freq), 
			&(testreq[req_no].no_func_invoc), 
			&(testreq[req_no].seq_exec_i), 
			&(testreq[req_no].seq_exec_j));
      if(parsread != 4)
	return wrong_file_desc;
      if(testreq[req_no].freq > 1)
	{
	parsread = fscanf(test_freq," (%d %d/%d)", 
			&(testreq[req_no].no_func_invoc_last), 
			&(testreq[req_no].seq_exec_i_last), 
			&(testreq[req_no].seq_exec_j_last));
	if(parsread != 3)
	  return wrong_file_desc;
	}
    }
  fclose (test_freq);
  
  return -1;
}

int main(argc,argv)
int argc;
char * argv[];
{

  char * results_name;
  int i, j;
  FILE * resultsfile;
  FuncInfo datafunc;

  if(argc != 5)
    {
     fprintf(stderr,"errorreveal <function> <crit> <test case no> <correct dir> <wrong dir>\n");
      fprintf(stderr,"Wrong number of parameters\n");
      exit(1);  
    }

}



