/*
// rotsem.c               %I%                     %Y% %E% %U%
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "util.h"
#include "hli.h"
#include "tabelasli.h"
#include "hparserg.h"

extern FILE * arqfonte;
extern FILE * arqli;
extern struct tableout saida;


extern int   comp_token, comp_ant;
extern int   lin_token;
extern long  ini_token;
extern int   seq;
extern int   cond;
extern int   pred;
extern int   flag_vazio;
extern char  label[];
extern int   flag_typedef;
extern char  ch;
extern int   pilha_grava_li;
extern struct cabeca *list;
extern int   linha;
extern long  offset;
extern flag_dirpre;
extern struct no_tipdef *raiz;
extern char * includes[];
extern int loc, last_loc;
extern GOTOLABEL * lista_resul;

#ifdef DEBUG

extern int debug_flag;

#endif

char nome_func[NCARMAX], nome_final[NCARMAX];
int enc_func = FALSE; /* encontrei um identificador */
int enc_par = FALSE;  /* encontrei um parenteses */
int enc_composto = FALSE; /* estou dentro do corpo da funcao */

static int argsfile  = 0;
static long int linefile  = 0;

long int pos_marcada, pos_final;



void rot1_sem()    /* escreve <dcl> */
{
 grava_li(1,"$DCL");
 flag_typedef=0;
}

void rot2_sem()    /* escreve <dcl> em definicao de funcao */
{
 if (strcmp(saida.classe,";") != 0)
    {
     comp_token=comp_ant;
     grava_li(1,"$DCL");
     calcula_posicao();
    }
}


void rot3_sem()    /* escreve <{> */
{
int comp, i;
char * buff;

if(enc_par && !enc_composto)
	{
	strcpy(nome_final,nome_func);
	enc_func = enc_par = FALSE;
  	if (! pilha_grava_li )
		{
		fflush(arqli);
		pos_final=ftell(arqli);
		comp=pos_final-pos_marcada;

		buff=(char *) malloc(sizeof(int)*(comp +1));
		if(buff==(char*)NULL)
			error("Erro Fatal: Nao consegui alocar memoria para o buffer\n");

		fseek(arqli,pos_marcada,SEEK_SET);
		
		comp=pos_final - pos_marcada;
		for(i=0; comp; ++i,--comp)
			{
			buff[i]=fgetc(arqli);
			if(buff[i] == EOF)
				break;
			}

		buff[i]='\0';

		fseek(arqli,pos_marcada,SEEK_SET);
		fprintf(arqli,"@%s 0 0 0\n",nome_final);
		fprintf(arqli,"%s",buff);
		free(buff);
		}
       }

enc_composto = TRUE;
grava_li(1,"{");
}


void rot4_sem()    /* escreve <}> */
{
 grava_li(1,"}");
}

void rot5_sem()    /* armazena possivel label */
{
 strcpy(label,saida.label);
}

void rot6_sem()    /* escreve <rotc-atm> */
{
 grava_li(1,"$ROTC");
}

void rot7_sem()    /* escreve <rotd-atm> */
{
 grava_li(1,"$ROTD");
}

void rot8_sem()    /* escreve <s> */
{
 grava_li(2,"$S");
}

void rot9_sem()    /* escreve <if-atm> */
{
 grava_li(1,"$IF");
}

void rot10_sem()    /* escreve <cond-atm> */
{
 grava_li(3,"$C");
 pred = 1;
}

void rot11_sem()    /* escreve <else-atm> */
{
 grava_li(1,"$ELSE");
}

void rot12_sem()    /* escreve <case-atm> */
{
 grava_li(1,"$CASE");
}

void rot13_sem()    /* escreve <case-cond-atm> */
{
 grava_li(1,"$CC");
}

void rot14_sem()    /* escreve <while-atm> */
{
 grava_li(1,"$WHILE");
}

void rot15_sem()    /* escreve <cond-while-atm> */
{
 grava_li(3,"$C");
 pred = 1;
}

void rot16_sem()    /* escreve <repeat-atm> */
{
 grava_li(1,"$REPEAT");
}

void rot17_sem()    /* escreve <until-atm> */
{
 grava_li(1,"$UNTIL");
}

void rot18_sem()    /* escreve <cond-until-atm> */
{
 grava_li(3,"$NC");
 pred = 1;
}

void rot19_sem()    /* escreve <for-atm> */
{
 grava_li(1,"$FOR");
}

void rot20_sem()    /* escreve <s> em <for> */
{
 if (flag_vazio == 1)
    ini_token = comp_token = lin_token = 0;
 else
    {
     flag_vazio = 1;
     comp_token=comp_ant;
    }
 grava_li(2,"$S");
 calcula_posicao();
}

void rot21_sem()    /* escreve <cond-for-atm> */
{
 if (flag_vazio == 1)
    ini_token = comp_token = lin_token = 0;
 else
    flag_vazio = 1;
 grava_li(3,"$C");
 pred = 1;
}

void rot22_sem()   /* reset flag de item inexistente */
{
 flag_vazio = 0;
}

void rot23_sem()    /* escreve <goto> */
{
 grava_li(1,"$GOTO");
}

void rot24_sem()    /* escreve <label> */
{
 grava_li(1,label);
}

void rot25_sem()    /* escreve <continue> */
{
 grava_li(1,"$CONTINUE");
}

void rot26_sem()    /* escreve <break> */
{
 grava_li(1,"$BREAK");
}

void rot27_sem()    /* escreve <return> */
{
 grava_li(1,"$RETURN");
}

void rot28_sem()    /* incrementa somador de predicados */
{
 ++pred;
}

void rot29_sem()    /* seta flag_typedef */
{
 flag_typedef=1;
}

void rot30_sem()    /* escreve <dir> */
{
 comp_token=comp_ant;
 /* grava_li(1,"$DIR"); */
 grava_li(1,"$DCL");
 calcula_posicao();
}

void rot31_sem()    /* devolve caracter '@' */
{

 ch='@';
 dev_ch();

 
}

void rot32_sem()    /* seta flag_nostoreli */
{
int aux;

/* pilha_grava_li += 1; printf("Rot32\n"); */
}

void rot33_sem()    /* reseta flag_nostoreli */
{ 
/* pilha_grava_li -= 1; printf("Rot33\n"); */
ini_token=0;
}

void rot34_sem()    /* expande tipos definidos em include */
{
 FILE *save_fonte    = arqfonte;
 char *s             = saida.label+1,
      fonte[TAMLEX], nome_inc[1000];
 int  i              = 0,
      save_linha     = linha,
      save_comp      = comp_token,
      save_lin_token = lin_token;
 long save_ini_token = ini_token,
      save_offset    = offset;
 char * home;
 
 pilha_grava_li += 1; 

 /* Tira os asteriscos do nome do arquivo fonte .h */

 for (;*s!='\"';i++ , s++)
 *(fonte+i)=*s;
 fonte[i] = '\0';



 for(i=0; includes[i] != (char *) NULL; i++)
	{
 	strcpy(nome_inc,includes[i]);

	/* Verifica se existe sinonimo para o diretorio home atraves do caracter
	   "~"
	*/
	
	if(nome_inc[0] == '~')
		{
		/* Pega o diretorio HOME */

		if((home = getenv("HOME")) == (char *) NULL)
			error("Erro Fatal: Nao consegui obter o valor da variavel HOME\n");
		strcpy(nome_inc,home);
		strcat(nome_inc,includes[i]+1);
	
		}
 
#ifdef TURBO_C
	strcat(nome_inc,"\\"); 
#else
  	strcat(nome_inc,"/"); 
#endif

  	strcat(nome_inc,fonte);
  
#ifdef TURBO_C
  	if ((arqfonte=fopen(nome_inc,"rb")) == (FILE*) NULL)
#else
  	if ((arqfonte=fopen(nome_inc,"r")) == (FILE*) NULL)
#endif
    		{
		if(includes[i+1] == (char *) NULL)
			{
    			printf("%s\n",nome_inc);
    			error("* * Erro Fatal: Nao arquivo existe para ser expandido * *\n");
    			}
		else
			continue;
		}
	else
		break;
	}

 	linha    = 1;
 	yylex();
 	if (parser(list,VERDADEIRO)==VERDADEIRO && saida.classe[1] == '\0')
   	/* printf("* * Arquivo %s expandido OK * *\n",fonte); */;
 	else
   		printf("* * Erro Nao-Fatal: erro sintatico ao expandir arquivo %s, atomo %s, linha %d * *\n",fonte,saida.label,saida.linha);
	
 fclose(arqfonte);
 arqfonte        = save_fonte;
 pilha_grava_li -= 1;  /* printf("Rot34-\n"); */
 offset          = save_offset;
 linha           = save_linha;
 comp_token      = save_comp;
 lin_token       = save_lin_token;
 ini_token       = save_ini_token;
}

void rot35_sem()    /* seta flag de diretiva de prepocessamento */
{
 flag_dirpre = 1;
}

void rot36_sem()    /* elimina parenteses do comando for */
{
 ini_token = 0;
}

void rot37_sem()    /* inclui identificador na tabela de typedefs */
{
 int nomatch=1;

 if ((flag_typedef) && (!strcmp(saida.classe,"IDENT")))
    {
     raiz=tree_tipo(raiz,saida.label,flag_typedef,&nomatch);
     if  (!nomatch)
	 printf("redefinicao de tipo --> %s\n",saida.label);
    }
} 

void rot38_sem()    /* trata defines */
{
argsfile = 1;
linefile = (long) atol(saida.label); 
linha = linefile-1;
}
 

void rot39_sem()
{
 int nomatch=1;

 if ((flag_typedef > 1) && (!strcmp(saida.classe,"IDENT")))
    {
     raiz=tree_tipo(raiz,saida.label,flag_typedef,&nomatch);
     if  (!nomatch)
	 printf("redefinicao de tipo --> %s\n",saida.label);
    }

 if(! enc_par )
 	{
	strcpy(nome_func,saida.label);
	}
}

void rot40_sem()
{
enc_par = TRUE;
}


void rot41_sem()
{
fprintf (arqli,"#%s %ld", saida.label, linefile);

}



void rot42_sem()
{
grava_li(1,"$DCL");
flag_typedef=0;
enc_par = FALSE; strcpy(nome_func,"");
fflush(arqli);
pos_marcada=ftell(arqli);

#ifdef DEBUG

if(debug_flag)
   printf("\n<< fim decl>>\n\n");

#endif

}

void rot43_sem()
{
insert_list(nome_final,loc-last_loc,&lista_resul);
last_loc = loc;
enc_par=FALSE;
enc_composto = FALSE;
fflush(arqli);
pos_marcada=ftell(arqli);
}

void rot44_sem()
{
  if(flag_typedef==1) 
	++flag_typedef;
}

void rot45_sem()
{
if(argsfile < 3)
	{
	fprintf(arqli," %2d",atoi(saida.label));
	argsfile++;
	}

}
void rot46_sem()
{

/* Ajusta linha */


while(argsfile < 3)
	{
	fprintf(arqli," 0");
	argsfile++;
	}

fprintf(arqli,"\n");

grava_li(4,"");
calcula_posicao();
}

void rot47_sem()
{
  pilha_grava_li+=1;
}



void rot48_sem()
{
  if(pilha_grava_li >0)
    {
      pilha_grava_li-=1; /* reset by-pass das funcoes inline */      
      comp_token=comp_ant;
      grava_li(1,"$INL"); /* grava texto da funcao inline */

      /* ajusta os ponteiros para codigo fonte */
      ini_token=0; 
      calcula_posicao();
     
   }

}
