/*
//
// pokelib10.c			%I%	%Y%	%E% %U%
//
// Funcao: este arquivo contem funcoes que manipulam a lista ligada que associa
//	   o numero de identificacao de variavel e a sua descricao. 
//
//
// Autor: Marcos L. Chaim
// Data: 30/08/90
//
// Observacao: estas funcoes faziam parte do arquivo rotsem1.c
//
*/

#include "util.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef TURBO_C

#include <alloc.h>

#else  /* SunOS, BSD4.2 e RISC 6000 */

#include <malloc.h>

#endif

#include "hrotsem.h"

/* Declaracao de Variaveis Globais */

extern DESPOINTER names;

/*
** insert_name() -- insere o nome de uma variavel e o seu numero de 
**		    identificacao numa lista ligada.
*/

void insert_name(description, id)
char * description;
int id;
{

 DESPOINTER name_aux;

 name_aux = names;

 names = (DESPOINTER) malloc(sizeof(DESCELL));
 if(names == (DESPOINTER) NULL)
     error("* * Erro Fatal: Nao consegui alocar espaco para o vetor de descricoes das variaveis (names) * * r");
 strcpy(names->name,description);
 names->id = id;
 names->next = name_aux;
 return;
}

/*
** show_description() -- obtem o nome da variavel cujo numero de identificacao
**		    	 e' id e escreve este nome em archive.
*/

void show_description(id,archive)
int id;
FILE * archive;
{
 DESPOINTER name_aux;
 for(name_aux = names;id != name_aux->id && name_aux != (DESPOINTER) NULL;
     name_aux=name_aux->next);

 if(name_aux == (DESPOINTER) NULL)
     error("* * Erro Fatal: Numero de Identificacao de Variavel Invalido * *\n");
 else
     fprintf(archive," %s", name_aux->name);
 return;
}


/*
**			Fim de pokelib10.c
*/
