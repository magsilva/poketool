/*
// stack.c 	1.1		94/12/29	10:34:04
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"
#include "stack.h"
#include "chanomat.h"


/* Funcoes que manipulam a pilha de inteiros */

void mk_nl_int(s)
STACK  * s;
{
 s->top = MAXLENGTH;
 return;
}

int empty_int(s)
STACK * s;
{
 if(s->top > MAXLENGTH - 1)
	    return(TRUE);
 else
	    return(FALSE);
}

int top_int(s)
STACK * s;
{
 if(empty_int(s))
      error("* * Erro Fatal: Pilha de inteiros vazia * *\r");
 return(s->elements[s->top]);
}

int pop_int(s)
STACK * s;
{

 /* Declaracao de Variaveis Locais */

 int topo;

 if(empty_int(s))
     error("* * Erro Fatal: Pilha de inteiros vazia * * \r");
 topo = s->elements[s->top];
 s->top = s->top+1;
 return(topo);
 }

 void push_int(x,s)
 int x;
 STACK * s;
 {
  if(s->top == 0)
      error("* * Erro Fatal: Pilha de inteiros cheia * *\r");
  s->top = s->top - 1;
  s->elements[s->top] = x;
  return;
 }



/********************************************************************/
/*                                                                  */
/*     FUNCOES ASSOCIADAS A PILHA DE PARES DE INTEIROS              */
/*                                                                  */
/********************************************************************/


void mk_nl_pair(s)
STACKPAIR  * s;
{
 s->top = MAXLENGTH;
 return;
}

int empty_pair(s)
STACKPAIR * s;
{
 if(s->top > MAXLENGTH - 1)
	    return(TRUE);
 else
	    return(FALSE);
}

PAIRINT top_pair(s)
STACKPAIR * s;
{
 if(empty_pair(s))
      error("* * Erro Fatal: Pilha vazia * *\r");
 return(s->elements[s->top]);
}

PAIRINT pop_pair(s)
STACKPAIR * s;
{

 /* Declaracao de Variaveis Locais */

 PAIRINT topo;

 if(empty_pair(s))
     error("* * Erro Fatal: Pilha vazia * *\r");
 topo = s->elements[s->top];
 s->top = s->top+1;
 return(topo);
 }

 void push_pair(x,s)
 PAIRINT * x;
 STACKPAIR * s;
 {
  if(s->top == 0)
      error("* * Erro Fatal: Pilha cheia * *\r");
  s->top = s->top - 1;
  s->elements[s->top] = *x;
  return;
 }

void insert_list(description, id, start)
char * description;
int id;
GOTOLABEL ** start;
{

 GOTOLABEL * name_aux, * new;

 

 new = (GOTOLABEL *) malloc(sizeof(GOTOLABEL));
 if(new == (GOTOLABEL *) NULL)
     error("* * Erro Fatal: Nao consegui alocar espaco para o vetor de descricoes das variaveis (names) * * r");

 /* Insere informacao */

 strcpy(new->name,description);
 new->id = id;
 new->next = NULL;

 /* posiciona no final da lista */

 if(*start != NULL)
     {
     for(name_aux = *start; name_aux->next != NULL; name_aux = name_aux->next);
     name_aux->next = new;
     }
 else
     *start = new;
 return;
}

void clear_list(list)
GOTOLABEL * list;
{
if(list != NULL)
	{
	clear_list(list->next);
	free(list);
	}
}

int find_node(description,head)
char * description;
GOTOLABEL * head;
{
 GOTOLABEL * name_aux;
 for(name_aux = head; name_aux != (GOTOLABEL *) NULL &&strcmp(name_aux->name,description);
     name_aux=name_aux->next);

 if(name_aux == (GOTOLABEL *) NULL)
     error("* * Erro Fatal: Rotulo invalido * *\n");

 return(name_aux->id);
}



/*
void main()
{

int n = 5;

char * label = "Cabeleira";

GOTOLABEL * comeco = NULL;

insert_list(label,n,&comeco);

printf("%s %d\n",label,find_node(label,comeco));
find_node("Marcos ",comeco);
}

void error(msg)
char * msg;
{
printf("%s\n",msg);
exit(1);
} */

