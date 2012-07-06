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
