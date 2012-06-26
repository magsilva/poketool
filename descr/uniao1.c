#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef TURBO_C

#include <alloc.h>

#else  /* SunOS, BSD4.2 e RISC 6000 */

#include <malloc.h>

#endif

#include "util.h"


/*******************************************************************/
/* funcao void uniao(struct no **, struct no **, struct no **)     */
/* Autor: Marcos L. Chaim                                          */
/* Versao: 1.0                                                     */
/* Data: 13/06/89                                                  */
/*                                                                 */
/* FUNCAO: esta funcao calcula a uniao de duas listas ordenadas de */
/*         inteiros positivos e coloca o resultado na terceira lis-*/
/*         ta.                                                     */
/*                                                                 */
/* ENTRADA:ponteiros para os ponteiros das listas a serem manipu-  */
/*         ladas.                                                  */
/*                                                                 */
/* SAIDA:  nenhuma.                                                */
/*                                                                 */
/* FUNCOES CHAMADAS: void lib_conj(), void atr_conj().             */
/*******************************************************************/

#include "hrotsem.h"
#include "header.h"
#include "hparserg.h"

void uniao(conj_a,conj_b,conj_c)
struct no **conj_a;
struct no **conj_b;
struct no **conj_c;
{
 /* declaracao de variaveis locais */
 struct no *acurrent, *bcurrent, *ccurrent, *noaux, *anaux;
 int i;

 /* iniciando */

 noaux=NULL;
 anaux=NULL;
 atr_conj(&noaux, conj_a);   /* atribui *conj_a e *conj_b para listas auxiliares */
 atr_conj(&anaux, conj_b);

 lib_conj(*conj_c);     /* libera espaco ocupado por *conj_c */

 *conj_c=(struct no *) malloc(sizeof(struct no));

 if(*conj_c==NULL)
     error("* * Erro Fatal: Nao consegui alocar memoria para este no' * *\n");
  acurrent= noaux;
  bcurrent= anaux;
  ccurrent= *conj_c;

  while((acurrent!=NULL)&&(bcurrent!=NULL))
    {
     if(acurrent->num==bcurrent->num)
       {    /* insere um so' */
	ccurrent->proximo=(struct no *) malloc(sizeof(struct no));
	if(ccurrent->proximo==NULL)
	  error("* * Erro Fatal: Nao consegui alocar memoria para este no' * *\n");
	 ccurrent=ccurrent->proximo;
	 ccurrent->num=acurrent->num;
	 acurrent=acurrent->proximo;
	 bcurrent=bcurrent->proximo;
       }
     else
       {
	if(acurrent->num < bcurrent->num)
	    {
	     i=acurrent->num;
	     acurrent=acurrent->proximo;
	    }
	 else
	    {
	     i=bcurrent->num;
	     bcurrent=bcurrent->proximo;
	    }
	ccurrent->proximo=(struct no *) malloc(sizeof(struct no));
	if(ccurrent->proximo==NULL)
	  error("* * Erro Fatal: Nao consegui alocar memoria para este no' * *\n");
	 ccurrent=ccurrent->proximo;
	 ccurrent->num=i;
       }
    }
 ccurrent->proximo=NULL;

 if(acurrent!=NULL)
     atr_conj(&(ccurrent->proximo),&acurrent);
 else   /* bcurrent!=NULL */
     atr_conj(&(ccurrent->proximo),&bcurrent);

 lib_conj(noaux);   /* libera o espaco das listas auxiliares */
 lib_conj(anaux);

 noaux=(*conj_c)->proximo;
 free(*conj_c);     /* libera a primeira celula nao ocupada */
 *conj_c=noaux;
 return;
}

/*******************************************************************/
/* int e_igual(struct no *, struct no *)                           */
/* Autor: Marcos Lordello Chaim                                    */
/* Versao: 1.0                                                     */
/* Data: 16/06/89                                                  */
/*                                                                 */
/* FUNCAO: esta funcao compara duas listas ordenadas de inteiros   */
/*         e retorna o valor TRUE se todo elemento da primeira lis-*/
/*         ta pertence `a segunda lista e se todo elemento da se-  */
/*         gunda tambem pertence `a primeira. O valor retornado e' */
/*         FALSE no caso contrario.                                */
/*                                                                 */
/* ENTRADA: ponteiros para as listas a serem comparadas.           */
/*                                                                 */
/* SAIDA  : valores constantes inteiros TRUE ou FALSE.             */
/*******************************************************************/

int e_igual(conj_a,conj_b)
struct no *conj_a;
struct no *conj_b;
{

/* declaracao de variaveis locais */
struct no * noaux, *anaux;
int retorno;

/* iniciando */

noaux=conj_a;
anaux=conj_b;

if(noaux==NULL && anaux==NULL)
	    return(TRUE);       /* as duas listas estao vazias */

retorno = FALSE;

while(noaux!=NULL && anaux!=NULL)
     {
      retorno = TRUE;
      if(noaux->num==anaux->num)
	  {
	   noaux=noaux->proximo;
	   anaux=anaux->proximo;
	  }
      else
	  {
	   retorno=FALSE;
	   break;
	  }
      }
if(noaux!=anaux) retorno=FALSE;    /* listas de comprimentos diferentes */
return(retorno);
}








