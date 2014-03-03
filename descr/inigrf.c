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

/********************************************************************/
/* Funcao ini_grf(char *, struct grafo**)                           */
/* Autor: Marcos L. Chaim                                           */
/* Data: 4/06/89                                                    */
/*                                                                  */
/* TAREFA: esta funcao tem como entrada um arquivo que contem um    */
/*         grafo representado como uma lista de sucessores cada no' */
/*         e o endereco de um apontador para a estrutura de dados   */
/*         que contera' o grafo na memoria. A funcao ira' colocar   */
/*         o grafo na memoria e retornara' o numero de nos desse    */
/*         desse grafo.                                             */
/*                                                                  */
/* ENTRADA: string de caracteres com o nome do arquivo e o endere-  */
/*          co do apontador para o grafo.                           */
/* SAIDA:   inteiro contendo o numero de nos do grafo.              */
/********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef TURBO_C

#include <alloc.h>

#else  /* SunOS, BSD4.2 e RISC 6000 */

#include <malloc.h>

#endif

#include "util.h"

#include "hrotsem.h"
#include "header.h"
#include "hparserg.h"

int inigrf(char * fnome, struct grafo ** g) {
	int i, j, k, n;
	struct grafo *gaux;
	FILE *fp;

	/* iniciando */
	fp = fopen(fnome, "r");      /* abrindo o arquivo que contem o grafo de fluxo de controle */
	if (fp == NULL) {
		char mensagem[255];
		sprintf(mensagem, "Error: could not open file with data of the control flow graph (%s)\n", fnome);
		error(mensagem);
	}

	fscanf(fp, "%d", &n); /* obtive numero de nos */
	gaux = (struct grafo *) malloc((n + 1) * sizeof(struct grafo)); /* aloquei memoria para o nos do grafo */
	if (gaux == NULL) {
		error("Error: could not alloc memory to store the control flow graph");
	}

	/* iniciando com todos os apontadores de listas de sucessores */
	for (i = 0; i <= n; ++i) {
		gaux[i].list_suc = NULL;
	}

	/* construindo o grafo na memoria */
	k=1;
	while (fscanf(fp, "%d", &i) != EOF && k <= n) {
		(gaux + i)->num = i;
		(gaux +i )->list_suc = NULL;
		if (i > n || i <= 0) {
			error("Error: CFG is not correctly specified in the file");
		}
		for (fscanf(fp, "%d", &j); j != 0; fscanf(fp,"%d",&j)) {
			ins_elem(&((gaux+i)->list_suc),j);
		}
		k++;
	}
	if (fscanf(fp, "%d", &i) != EOF) {
		error("Error: there are more nodes than expected to be read");
	}

	fclose(fp);
	*g = gaux;
	return n;
}

/********************************************************************/
/* void lib_grf(struct grafo * , int)                               */
/* Autor: Marcos L. Chaim                                           */
/* Versao: 1.0                                                      */
/* Data: 20/07/89                                                   */
/*                                                                  */
/* FUNCAO: liberar espaco ocupado por um grafo.                     */
/*                                                                  */
/* ENTRADA:apontador para o grafo e numero de nos do grafo.         */
/*                                                                  */
/* SAIDA:  nenhuma.                                                 */
/*                                                                  */
/* FUNCOES CHAMADAS: lib_conj().                                    */
/*                                                                  */
/********************************************************************/

void lib_grf(g,n)
struct grafo *g;
int n;
{

 /* declaracao de variaveis locais */
 int i;
 for(i=1;i<=n;++i)
    lib_conj(g[i].list_suc);
 if(g != (struct grafo *) NULL)
    free(g);
 return;

}

/********************************************************************/
/* void lib_dom(struct con_int * , int)                             */
/* Autor: Marcos L. Chaim                                           */
/* Versao: 1.0                                                      */
/* Data: 20/07/89                                                   */
/*                                                                  */
/* FUNCAO: liberar espaco ocupado pelo conjunto de dominadores      */
/*                                                                  */
/* ENTRADA:apontador para o conjunto de dominadores e numero de nos */
/*         do grafo.                                                */
/*                                                                  */
/* SAIDA:  nenhuma.                                                 */
/*                                                                  */
/* FUNCOES CHAMADAS: lib_conj().                                    */
/*                                                                  */
/********************************************************************/

void lib_dom(dom,n)
struct conj_int *dom;
int n;
{

 /* declaracao de variaveis locais */
 int i;
 for(i=1;i<=n;++i)
    lib_conj(dom[i].apont);
 if(dom != (struct conj_int *) NULL)
    free(dom);
 return;

}

