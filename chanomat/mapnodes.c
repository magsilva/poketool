#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

struct symbol {
	int inicio;
        int node;
	int comprimento;
	int linha;
	char simbolo[1024];
	} newsymbol;

#define MAXLINE 1000

int * cria_vec_int(size)
     int size;
{
  int i, * pvec;
  pvec = (int *) malloc(sizeof(int)*(size+1));

  if(pvec == NULL)
    {
      fprintf(stderr,"Error: Out of Memory\n");
      exit(1);
    }

  for (i=0; i <= size; ++i)
    pvec[i]=0;

  return pvec;
}

char * le_linha_str(linha,string, file)
char * linha;
char * string;
FILE * file;
{

 /* Declaracao de Variaveis Locais */

 char * ptr_linha;

 do
   {
    if(fgets(linha,MAXLINE-1,file) == (char *) NULL)
       {
	 fprintf(stderr,"Error: file incorrectly described.");
	 exit(1);
       }
    ptr_linha = strstr(linha,string);
   }
 while(ptr_linha == (char *) NULL);
 return(ptr_linha);
}

int get_no_nodes(pdir)
     char * pdir;
{
 FILE *desc, * fault_nodesfile;
 char filename[1000], linha[1000];
 int no_nodes;

  strcpy(filename,pdir);
  strcat(filename,"/des_nos.tes");
 
  desc = fopen(filename,"r");
  
  if(desc == NULL)
    {
    fprintf(stderr,"Error: it was not possible to open file %s\n",filename);
    exit(1);
    }

 /*
   position in the end of the descriptor file less a certain quantity of bytes  */
 
 fseek(desc, (long) -50, SEEK_END);
 
 /* read number of requeriments */

 le_linha_str(linha,"Numero Total de Descritores =", desc);

 sscanf(linha,"Numero Total de Descritores = %d", &no_nodes);

 fclose(desc);
 return no_nodes;
}

/*
** cria_dir() -- cria um subdiretorio com o nome passado como parametro no dire-
** 		 torio corrente.
*/


void cria_dir(nome)
char * nome;
{
/* Declaracao de Variaveis Locais */

char path[200];

#ifdef TURBO_C
strcpy(path,".\\");
#else
strcpy(path,"./");
#endif

strcat(path,nome);

if(mkdir(path,0751))
	{
	/* Se o erro nao foi provocado pela ja' existencia do diretorio, 
	** fim da execucao! 
	*/
	if(errno != EEXIST)
		{
		fprintf(stderr,"Erro Fatal: Nao consegui criar diretorio\n");
		exit(1);
		}
	}

/* Diretorio criado com permissao total para o dono, leitura e execucao para 
** o grupo e leitura para os outros.
*/
}

void grava_arq(pfuncname,ploc,no_nodes)
     char * pfuncname;
     int * ploc;
     int no_nodes;
{
 FILE *desc;
 char filename[1000];
 int i;

  strcpy(filename,pfuncname);
  strcat(filename,"/");
  strcat(filename,pfuncname);
  strcat(filename,".stm");
 
  desc = fopen(filename,"w");
  
  if(desc == NULL)
    {
    fprintf(stderr,"Error: it was not possible to open file %s\n",filename);
    exit(1);
    }

  for(i=1; i <= no_nodes; ++i)
    fprintf(desc,"%3d %3d\n",i, ploc[i]);

  fclose(desc);
}

int main(argc, argv)
int argc;
char * argv[];
{
 FILE * arqli;
 char name[100], func_name[100], s[10];
 struct symbol *pnewsymbol;
 int no_nodes, func_active=0, * loc_nodes=NULL;

 pnewsymbol=&newsymbol;

 if(argc != 2)
	{
	fprintf(stderr,"uso: mapnodes <nome do arquivo sem ext>\n");
	exit(1);
	}

 strcpy(name,argv[1]);
 strcat(name,".");
 strcat(name,"nli");

 if((arqli=(FILE *) fopen(name,"r"))==(FILE *) NULL)
	{
	printf("Nao consegui abrir o arquivo %s\n",name);
	exit(1);
	}

 fscanf(arqli,"%s %ld %ld %ld %ld\n", pnewsymbol->simbolo,
	                    &pnewsymbol->node,&pnewsymbol->inicio,
                            &pnewsymbol->comprimento,
                            &pnewsymbol->linha);

  while(!feof(arqli))
	{

	if(pnewsymbol->simbolo[0] == '@')
		{
		  if(func_active)
		    /* grava arquivo */
		    {
		      grava_arq(func_name,loc_nodes, no_nodes);
		      free(loc_nodes);
		    }

		  func_active=1;
		  strcpy(func_name,pnewsymbol->simbolo+1);
		  /* Verifica existencia do diretorio */
		  cria_dir(func_name);

		  /* Pega o numero de nos */
		  no_nodes=get_no_nodes(func_name);
		  /* Aloca o vetor de nos */
		  /* Zera vetor */
		  loc_nodes = cria_vec_int(no_nodes);
		}

	if(func_active)
	  {
	  /* Se diferente deste comandos, incrementa. */
	    
	  if (strcmp(pnewsymbol->simbolo,"$DCL") && 
	      strncmp(pnewsymbol->simbolo,"$C",2) && 
	      strncmp(pnewsymbol->simbolo,"#",1) && 
	      strcmp(pnewsymbol->simbolo,"$REPEAT") && 
	      strcmp(pnewsymbol->simbolo,"$ELSE") && 
	      strcmp(pnewsymbol->simbolo,"{") && 
	      strcmp(pnewsymbol->simbolo,"}") && 
	      strncmp(pnewsymbol->simbolo,"$NC",3) && 
	      strncmp(pnewsymbol->simbolo,"$CC",3) && 
	      strcmp(pnewsymbol->simbolo,"$ROTC") &&
	      strcmp(pnewsymbol->simbolo,"$ROTD") &&
	      strcmp(pnewsymbol->simbolo,"$GOTO") && 
	      strcmp(pnewsymbol->simbolo,"$FOR") && 
	      strcmp(pnewsymbol->simbolo,"$GOTO"))
		{
		  loc_nodes[pnewsymbol->node]++;
		}	  
	  if(!strcmp(pnewsymbol->simbolo,"$FOR"))
	    loc_nodes[pnewsymbol->node+1]++;
	  if(!strcmp(pnewsymbol->simbolo,"$CASE"))
	    loc_nodes[pnewsymbol->node]++;
	  if(!strcmp(pnewsymbol->simbolo,"$CONTINUE"))
	    loc_nodes[pnewsymbol->node]++;
	  }

	fscanf(arqli,"%s %ld %ld %ld %ld\n", pnewsymbol->simbolo,
	                    &pnewsymbol->node,&pnewsymbol->inicio,
                            &pnewsymbol->comprimento,
                            &pnewsymbol->linha);
	}

 /* grava arquivo */
 if(func_active)
   {
     grava_arq(func_name,loc_nodes, no_nodes);
     free(loc_nodes);
   }
 return 0;
}

