#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct symbol {
	int inicio;
	int comprimento;
	int linha;
	char simbolo[1024];
	} newsymbol;

main(argc, argv)
int argc;
char * argv[];
{
FILE * arqli, * arqfonte;
char name[100];
struct symbol *pnewsymbol;
int c;

pnewsymbol=&newsymbol;

if(argc != 3)
	{
	printf("uso: chekcout <nome do arquivo sem ext> <ext>\n");
	exit(1);
	}
strcpy(name,argv[1]);
strcat(name,".");
strcat(name,argv[2]);

if((arqfonte=(FILE *) fopen(name,"r"))==(FILE *) NULL)
	{
	printf("Nao consegui abrir o arquivo %s\n",name);
	exit(1);
	}

strcpy(name,argv[1]);
strcat(name,".");
strcat(name,"li");

if((arqli=(FILE *) fopen(name,"r"))==(FILE *) NULL)
	{
	printf("Nao consegui abrir o arquivo %s\n",name);
	exit(1);
	}

fscanf(arqli,"%s %ld %ld %ld\n", pnewsymbol->simbolo,&pnewsymbol->inicio,
                            &pnewsymbol->comprimento,
                            &pnewsymbol->linha);
while(!feof(arqli))
	{
	fprintf(stdout,"%s %ld %ld %ld\n",
	pnewsymbol->simbolo,pnewsymbol->inicio,
                            pnewsymbol->comprimento,
                            pnewsymbol->linha);
	if(pnewsymbol->simbolo[0] != '@')
		{
		fseek(arqfonte,pnewsymbol->inicio-1, SEEK_SET);
        	while(pnewsymbol->comprimento > 0)
        		{
        		c=fgetc(arqfonte);
			fputc(c,stdout);
			pnewsymbol->comprimento--;
        		}
		fprintf(stdout,"\n");
		}

	fscanf(arqli,"%s %ld %ld %ld\n", pnewsymbol->simbolo,&pnewsymbol->inicio,
                            &pnewsymbol->comprimento,
                            &pnewsymbol->linha);
	}
}
