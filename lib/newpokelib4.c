/*
// newpokelib4.c 	%I%	%Y%	%E%	%U%
*/
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
#include "newpokelib.h"

/* Miscelaneous Functions used in this file */

void print_recdef(TABDEF);
void print_bucket_info(TABDEF);
TABDEF deallocate_bucket(TABDEF);


/* Declaracao de Variaveis Globais */

extern STACK stack_nest;
extern table_element * pvarname;
static TABDEF * tab_var_def[MAXHASH];
extern NODEINFO info_no;
extern b_vector undef;
extern b_vector fut_undef;

static int name_counter=0;

int get_name_counter(void)
{
  return name_counter;
}

void print_field(TABDEF bucket)
{

switch(bucket->type)
  {
  case VOID: printf("void ");
               break;

  case SHORT: printf("short ");
               break;

  case INT: printf("int ");
               break;

  case LONG: printf("long ");
               break;

  case FLOAT: printf("float ");
               break;

  case DOUBLE: printf("double ");
               break;

  case CHAR: printf("char ");
               break;

  case STRUCT: if(!strcmp(bucket->recdef->description,""))
                  print_recdef(bucket->recdef);
               else   
                  printf("struct %s ", bucket->recdef->description);
       
               break;                 
 
  case UNION: if(!strcmp(bucket->recdef->description,""))
                  print_recdef(bucket->recdef);
               else
                  printf("union %s", bucket->recdef->description);               
               break;
               
  case TYPEDEF: printf("%s ",bucket->recdef->description); 
               break;
               
  default: printf("Undefined type %d ", bucket->type); 
               break;
  }

switch(bucket->mod)
      {
      case BASIC: break;

      case PTR: printf(" * ");
              break;

      case PTR_PTR: printf(" * * ");
              break;

      case PTR_FUNC: printf (" ( * ) ");
              break;
              
      default: printf("Undefined modifier %d ", bucket->mod); 
               break;
}

printf(" %s", bucket->description);

if(bucket->nature != FUNC)
    {
    switch(bucket->structure)
      {
      case SIMPLE: break;

      case VECTOR: printf(" [] ");
              break;

      case MATRIX: printf(" [] [] ");
              break;

      default: printf("Undefined structure %d ", bucket->structure); 
               break;
      }
      printf("\n");
    }
else
    printf("()\n");


}
  
void print_name(TABDEF bucket)
{
TABDEF current;

  switch(bucket->nclass)
  {
  case STATIC: printf("static "); 
               break;
  case AUTOMATIC:  printf("automatic "); 
               break;
  case REGISTER:  printf("register "); 
               break;
  case GLOBAL: printf("global ");
               break;
  case STATIC_GLB:  printf("static global "); 
               break;
  case TYPDEF:  printf("typedef ");
               if(bucket->recdef != NULL)
                 return;
               break;
  default: printf("Undefined nclass %d ", bucket->nclass); 
               break;
  }

switch(bucket->type)
  {
  case VOID: printf("void ");
               break;

  case SHORT: printf("short ");
               break;

  case INT: printf("int ");
               break;

  case LONG: printf("long ");
               break;

  case FLOAT: printf("float ");
               break;

  case DOUBLE: printf("double ");
               break;

  case CHAR: printf("char ");
               break;

  case STRUCT: if(!strcmp(bucket->recdef->description,""))
                  print_recdef(bucket->recdef);
               else   
                  printf("struct %s ", bucket->recdef->description);
       
               break;                 
 
  case UNION: if(!strcmp(bucket->recdef->description,""))
                  print_recdef(bucket->recdef);
               else
                  printf("union %s", bucket->recdef->description);               
               break;
               
  case TYPEDEF: printf("%s ",bucket->recdef->description); 
               break;
               
  default: printf("Undefined type %d ", bucket->type); 
               break;
  }

switch(bucket->mod)
      {
      case BASIC: break;

      case PTR: printf(" * ");
              break;

      case PTR_PTR: printf(" * * ");
              break;

      default: printf("Undefined modifier %d ", bucket->mod); 
               break;
}

printf(" %s", bucket->description);

if(bucket->nature != FUNC)
    {
    switch(bucket->structure)
      {
      case SIMPLE: break;

      case VECTOR: printf(" [] ");
              break;

      case MATRIX: printf(" [] [] ");
              break;

      default: printf("Undefined structure %d ", bucket->structure); 
               break;
      }
      printf("\n");
    }
else
  {
    printf("(");
    /* Printing pars, if they exit. */

    if(bucket->lst_of_pars != NULL)
        {
         printf("\n");
         for(current = bucket->lst_of_pars; current != NULL;
             current=current->next)
	        {
                 print_bucket_info(current);
                }
         }
    printf(")\n");
  }


}

void print_recdef(TABDEF bucket)
{
TABDEF current;


if(bucket->nclass == TYPDEF)
  printf("typedef ");

switch(bucket->type)
  {
  case VOID: printf("void ");
               break;

  case SHORT: printf("short ");
               break;

  case INT: printf("int ");
               break;

  case LONG: printf("long ");
               break;

  case FLOAT: printf("float ");
               break;

  case DOUBLE: printf("double ");
               break;

  case CHAR: printf("char ");
               break;

  case STRUCT:   if(bucket->nclass == TYPDEF)
                   print_recdef(bucket->recdef);
                 else
                   {
                   printf("struct %s ", bucket->description);
    
                   /* Printing fields, if they exit. */

                   if(bucket->lst_of_flds != NULL)
                     {
                      printf("\t\t{\n");
                   
               
                     for(current = bucket->lst_of_flds; current != NULL;
                       current=current->next)
		      {
                      printf("\t\t");
                      print_bucket_info(current);
                      printf("\n");
                      }
                     }
                   printf("\t\t} ");
                   }
               break;                 
 
  case UNION:
                 if(bucket->nclass == TYPDEF)
                   print_recdef(bucket->recdef);
                 else
                   {
                  printf("union %s", bucket->description);
    
                 /* Printing fields, if they exit. */

                  if(bucket->lst_of_flds != NULL)
                   {
                   printf("\t\t{\n");
                  
               
                   for(current = bucket->lst_of_flds; current != NULL;
                       current=current->next)
		      {
                      printf("\t\t");
                      print_bucket_info(current);
                      printf("\n");
                      }
                   }
                    printf("\t\t} ");
                   }
               break;
               
  case TYPEDEF: printf("%s ",bucket->recdef->description); 
               break;
               
  default: printf("Undefined type %d ", bucket->type); 
               break;
  }

switch(bucket->mod)
      {
      case BASIC: break;

      case PTR: printf(" * ");
              break;

      case PTR_PTR: printf(" * * ");
              break;

      default: printf("Undefined modifier %d ", bucket->mod); 
               break;
}

if(bucket->nclass == TYPDEF)
  printf(" %s\n",bucket->description);


}

void print_bucket_info(TABDEF bucket)
{

#ifdef DEBUG_TSB /* Insert this code if necessary to debug the table of symbols */
  
TABDEF current;
switch(bucket->nature)
  {
  case VARREC: 
  case VARNAME: 
  case FUNC:  print_name(bucket); 
              break;

  case RECDEF: print_recdef(bucket);
              break;
              
  case ENUMNAME: printf("enum %s",bucket->description);
               break;
  case PAR:  print_field(bucket);
             /* Printing pars, if they exit. */

             if(bucket->lst_of_pars != NULL)
                 {
                 printf("\n");
                 for(current = bucket->lst_of_pars; current != NULL;
                 current=current->next)
	            {
                    printf("\t");
                    print_bucket_info(current);
                    }
                 }
               break;
  case FLDREC:
  case FLD: print_field(bucket);
               break;

  }
#endif /* DEBUG_TSB */
}


void dump_tab_var_def(void)
{
int i, j, k;
TABDEF * nest_tab, current;


for(i=0; i < MAXHASH; i++)
  {
  nest_tab=tab_var_def[i];
  
  if(nest_tab != NULL)
    {
    printf("---> Nesting #%3d:\n",i);
    for (j=0; j < SIZEHASH; ++j)
      for(current=nest_tab[j];current != NULL; current=current->next) 
        print_bucket_info(current);
    }
   
  }
}


/*******************************************************************/
/* make_null()                                                     */
/* Autor: Marcos L. Chaim                                          */
/* Data: 24/10/89                                                  */
/* Versao: 1.0                                                     */
/*                                                                 */
/* Funcao: inicia as entradas da tabela de variaveis definidas com */
/*         o valor vazio.                                          */
/*                                                                 */
/* Entradas: nenhuma.                                              */
/*                                                                 */
/* Saida: nenhuma.                                                 */
/*                                                                 */
/*******************************************************************/

void make_null()
{
 int i;
 for(i=0; i < MAXHASH; ++i)
     tab_var_def[i] = (TABDEF *) NULL;
 return;
}

/*******************************************************************/
/* inicia_tab_var_def(int, int)                                    */
/* Autor: Marcos L. Chaim                                          */
/* Data: 26/12/94                                                  */
/* Versao: 1.0                                                     */
/*                                                                 */
/* Funcao: inicia a tabela de variaveis definidas de um dado bloco.*/
/*                                                                 */
/* Entradas: bloco e tamanho da tabela.                            */
/*                                                                 */
/* Saida: nenhuma.                                                 */ 
/*                                                                 */
/*******************************************************************/

void inicia_tab_var_def(blk,tam)
int blk;
int tam;
{

int i;

if(blk < MAXHASH)
	{
	if(tam < MAXHASH-1) /* Deixa um espaco para marcar o fim das tabelas */
		{

		/* E' alocado um espaco a mais para marcar o final da 
		** tabela de variaveis definidas do nivel
		*/

		tab_var_def[blk] = (TABDEF *) malloc((tam+1)*sizeof(TABDEF));
		if (tab_var_def[blk] == (TABDEF *) NULL)
			error("Erro Fatal: memoria insuficiente para alocacao\
 da tabela de simbolos");

		/* Inicia com NULL os ponteiros da tab_var_def do nivel */
 		for(i=0; i < tam; ++i)
    			 tab_var_def[blk][i] = (TABDEF) NULL;

		/* Marca o final da tab_var_def do nivel. Util p/ liberar 
		** memoria.
		 */
    		tab_var_def[blk][i] = (TABDEF) -1;
		}
	else
		error("Erro Fatal: tamanho para alocacao da tabela de simbolos \
excedeu o permitido");
	}
else
	error("Erro Fatal: Nivel de aninhamento excedeu o permitido");
}

/*******************************************************************/
/* libera_tabs_from(int )                                          */
/* Autor: Marcos L. Chaim                                          */
/* Data: 26/12/94                                                  */
/* Versao: 1.0                                                     */
/*                                                                 */
/* Funcao: desaloca todas as tabelas de variaveis definidas a par- */
/* de um dado bloco.                                               */
/*                                                                 */
/* Entradas: bloco.                                                */
/*                                                                 */
/* Saida: nenhuma.                                                 */
/*                                                                 */
/*******************************************************************/

void libera_tabs_from(blk)
{
int i;

for(i=blk; tab_var_def[i] != (TABDEF *) NULL; ++i)
	libera_tab_var_def(i);
}

/*******************************************************************/
/* libera_tab_var_def(int)                                         */
/* Autor: Marcos L. Chaim                                          */
/* Data: 26/12/94                                                  */
/* Versao: 1.0                                                     */
/*                                                                 */
/* Funcao: desaloca a tabela de variaveis definidas de um dado     */
/* bloco.                                                          */
/*                                                                 */
/* Entradas: bloco e tamanho da tabela.                            */
/*                                                                 */
/* Saida: nenhuma.                                                 */ 
/*                                                                 */
/*******************************************************************/

void libera_tab_var_def(blk)
int blk;
{
int i;

for(i=0; tab_var_def[blk][i] != (TABDEF) -1; ++i)
	libera_bucket(tab_var_def[blk][i]);

free (tab_var_def[blk]);

tab_var_def[blk]=(TABDEF *) NULL;

}

/*******************************************************************/
/* libera_bucket(TABDEFCELL *)                                     */
/* Autor: Marcos L. Chaim                                          */
/* Data: 26/12/94                                                  */
/* Versao: 1.0                                                     */
/*                                                                 */
/* Funcao: desaloca a lista de buckets da tabela de variaveis      */
/* definidas de um dado bloco.                                     */
/*                                                                 */
/* Entradas: ponteiro para o bucket.                               */
/*                                                                 */
/* Saida: nenhuma.                                                 */ 
/*                                                                 */
/*******************************************************************/

void libera_bucket(p_bucket)
TABDEFCELL * p_bucket;
{
if(p_bucket != (TABDEFCELL *) NULL)
	{
	libera_bucket(p_bucket->next);
	free(p_bucket);
	}
}


/*
** lookup -- it returns the bucket associated to a description, if present
**           in the table; if not, returns NULL.
*/

TABDEF lookup(description)
char * description;
{

  /* Declaracao de Variaveis Locais */

  TABDEF current;
  STACK stack_aux;
  int blk_aux, bucket;
  int found=FALSE;

  mk_nl_int(&stack_aux);

  /* Procura description na Tabela de Variaveis Definidas */

  bucket =  hash(description);

  while(!empty_int(&stack_nest) && !found)
      {
       blk_aux = pop_int(&stack_nest);
       push_int(blk_aux,&stack_aux);
       current = tab_var_def[blk_aux][bucket];
       while( current != (TABDEF) NULL )
	   {
	    if(!strcmp(description,current->description))
	      {
               if(current->nature != FUNC)
                  {
                  found=TRUE;
	          break;
                  }
	      }
	    current = current->next;
	   }
       }

   /* Restabelece pilha de aninhamento stack_nest */

   while(!empty_int(&stack_aux))
	push_int(pop_int(&stack_aux),&stack_nest);

   return(current);
}
/*
** lookup_func -- it returns the bucket associated to a description, and which
**                nature is equal to FUNC, if present in the table; if not,
**                returns NULL.
**                
*/

TABDEF lookup_func(description)
char * description;
{

  /* Declaracao de Variaveis Locais */

  TABDEF current;
  STACK stack_aux;
  int blk_aux, bucket;
  int found=FALSE;

  mk_nl_int(&stack_aux);

  /* Procura description na Tabela de Variaveis Definidas */

  bucket =  hash(description);

  while(!empty_int(&stack_nest) && !found)
      {
       blk_aux = pop_int(&stack_nest);
       push_int(blk_aux,&stack_aux);
       current = tab_var_def[blk_aux][bucket];
       while( current != (TABDEF) NULL )
	   {
	    if(!strcmp(description,current->description))
	      {
                if(current->nature == FUNC)
                  {
                  found=TRUE;
	          break;
                  }
	      }
	    current = current->next;
	   }
       }

   /* Restabelece pilha de aninhamento stack_nest */

   while(!empty_int(&stack_aux))
	push_int(pop_int(&stack_aux),&stack_nest);

   return(current);
}

/*
** lookup_field -- it returns the bucket associated to a field of a bucket,
**                 if present in the table; if not, returns NULL.
*/

TABDEF lookup_field(elemtab, description)
TABDEF elemtab;
char * description;
{
 /* Local vars definition */

  TABDEF current = NULL, field = NULL;
  

  if(elemtab != NULL)
    {
    if(elemtab->nature==RECDEF)
      if(elemtab->nclass != TYPDEF)
       /* Union and Struct */
       current = elemtab->lst_of_flds;
      else
       {/* typedef */
	 while(elemtab->recdef != NULL)
	   elemtab = elemtab->recdef;
          current = elemtab->lst_of_flds;  
       }
    while(current != NULL)
      {
      if(!strcmp(description,current->description))
         {
         field = current;
         break;
         }
      current=current->next;
      }
    }
  return(field);
}

/*
** get_rec_def() -- gets the address of the bucket that defines 
**                   a variable or field.
*/

TABDEF get_rec_def(bucket)
TABDEF bucket;
{

  TABDEF def = NULL;
  

  if(bucket != NULL)
    {
    if(bucket->nature==RECDEF)
      {
      if(bucket->nclass != TYPDEF)
      /* Union and Struct */
        def = bucket;
      else
        /* typedef */
	while (bucket->recdef != NULL) bucket = bucket->recdef;
        def = bucket;
      }
    else
      {
	if(bucket->recdef != NULL) def = get_rec_def(bucket->recdef);
      }
     }
    
 return def;
}
/*
** verify_undef -- it verifies the occurrence of a undefinition of a variable.
**                 This function should only be invoked if it has not happened
**                 a variable redeclaration (checked by function
**                 verify_redecl()).
*/

void verify_undef(description,nat)
char * description;
int nat;
{
TABDEF current;

 if(nat == VARREC || nat == VARNAME || nat == PAR || nat == PARREC)
   {
   current = lookup(description);

   if(current != NULL)
     { /* It is a local var */
     nat = get_nature(current);
     if(nat == VARREC || nat == VARNAME || nat == PAR || nat == PARREC)
       {
       if(is_record(current))
         switch(get_mod(current))
           {
           case BASIC:
             break;
           case PTR_PTR:
             reset_bit(get_id(current)+1,&undef);
           case PTR:
           case PTR_FUNC:
             reset_bit(get_id(current),&undef);
           }
       reset_all_fields(&undef,current,get_id(current));
       }
    }
  }
  
}

/*
** set_fut_undef -- it sets the declared var as future undefined var when
**                  its scope finishes.
*/

void set_fut_undef(bucket)
TABDEF bucket;
{

if(bucket != NULL)
  { /* It is a local var */

    if(is_record(bucket))
      switch(get_mod(bucket))
        {
        case BASIC:
          break;
        case PTR_PTR:
          reset_bit(get_id(bucket)+1,&fut_undef);
        case PTR:
        case PTR_FUNC:
          reset_bit(get_id(bucket),&fut_undef);
        }
    reset_all_fields(&fut_undef,bucket,get_id(bucket));
  }
  
}

/*
** verify_field -- it checks whether there is a recursive definition in a
**                 struct definition. If so, it returns the number of the effective id number;
**                 otherwise, -1.
**                 
*/

int verify_field(father, visitedlist, description,type_of_analysis)
TABDEF father;
element * visitedlist;
char * description;
int type_of_analysis;
{
 /* Local vars definition */

 element * listaux;

 for(listaux = visitedlist; listaux != NULL; listaux=listaux->next)
   if(father == (TABDEF) listaux->name &&
      (!strcmp(description, listaux->name1) || type_of_analysis == WILDCARD))
     return listaux->info;

 return -1;
}

/*
** check_nature() -- verify is the nature of the tokens with same description
**                   is compliant.
*/

int check_nature(nat_redef, nat_new)
int nat_redef;
int nat_new;
{
 int ret;
 switch(nat_new)
   {
   case VARREC:
   case VARNAME:
   case PAR:
   case PARREC: ret = (nat_redef == VARREC || nat_redef == VARNAME || 
		   nat_redef == PAR || nat_redef == PARREC) ;
                break;
   default: ret = (nat_new == nat_redef);
   }
return ret;
}

int verify_redecl(start, description, nature)
TABDEF * start;
char * description;
int nature;

{
 TABDEF current, previous;
 int ret=FALSE;
 MIX vaux;
 
 current = previous = *start;
 
 while(current != (TABDEF) NULL)
   {
   if(!strcmp(description,current->description) && check_nature(current->nature,nature)) /* same description and same nature ? */
      {
          if(current == *start)            
             * start = previous = current->next;
          else /* != start */
             previous->next = current->next;
                    
          switch(nature)
            {
             case FUNC:
	       /* printf("* * Aviso: Funcao \"%s\" redeclarada no mesmo escopo * *\n",description); */
               deallocate_bucket(current);
               break;
		  
            case ENUMNAME:
	      /* printf("* * Aviso: Constante \"%s\" redeclarada no mesmo escopo * *\n",description); */
              deallocate_bucket(current);
              break;
	      
            case RECDEF:
	      /* printf("* * Aviso: Tipo \"%s\" redefinido no mesmo escopo * *\n",description); */
                        
              break;
	      
            case VARREC:
            case VARNAME:
	      /* printf("* * Aviso: Variavel \"%s\" redeclarada no mesmo escopo * *\n",description); */

              vaux.abs = (int) current;
              vaux.coor = get_id(current);

	      if(get_pars_dcl() || top_int(&stack_nest) == 0)
		{ /* It is a parameter or a global var that has been redefined */
                reset_def_var(current, &vaux);
		}
              else
                { /* It is a local var */
                if(is_record(current))
                  switch(get_mod(current))
                    {
                    case BASIC:
                      break;
                    case PTR_PTR:
                      reset_bit(get_id(current)+1,&undef);
                    case PTR:
                    case PTR_FUNC:
                      reset_bit(get_id(current),&undef);
                    }
                reset_all_fields(&undef,current,get_id(current));
                }
              
              deallocate_bucket(current);
              break;
	      
            case FLD:
            case FLDREC:
	      /* printf("* * Aviso: Campo \"%s\" redeclarado no mesmo escopo * *\n",description); */
              deallocate_bucket(current);
              break;
            }
	  
            ret = TRUE;
            current=(previous == NULL) ? NULL : previous->next;
      }
   else
      {
       previous = current;     
       current = current->next;
      }
   }

return ret;
}

element * prepare_id_allocation(TABDEF bucket, int * p_name_counter, element * list)
{
 
 int name_counter;
 
 name_counter = * p_name_counter;
 
 bucket->id = name_counter;
 
 switch(bucket->mod)
      {
       case BASIC:
          list = insert_list_vec_names(copy_list_vec_names(list), bucket->description);

          if(bucket->nature == VARREC)
            {
             pvarname=insert_vec_names(pvarname,name_counter,copy_list_vec_names(list));
            
             name_counter++;  
            }
          
          break;
                 
       case PTR:
       case PTR_FUNC:         
          /* id number for the pointer */
                 
          pvarname=insert_vec_names(pvarname,name_counter,insert_list_vec_names(copy_list_vec_names(list), bucket->description));
          
          name_counter++;

          /* prepare list for the next fields */
                 
          list = insert_list_vec_names(insert_list_vec_names(copy_list_vec_names(list), "*"),bucket->description);
          if(is_record(bucket)) /* Create the derefenced record. */            
            {
            pvarname=insert_vec_names(pvarname,name_counter,copy_list_vec_names(list));            
            name_counter++;
            }
          
          break;
                 
          case PTR_PTR:
          /* id number for the pointer */
                 
          pvarname=insert_vec_names(pvarname,name_counter,insert_list_vec_names(copy_list_vec_names(list), bucket->description));
          
          name_counter++;
                 
          /* id number for the first pointer dereferention */

          pvarname=insert_vec_names(pvarname,name_counter,insert_list_vec_names(insert_list_vec_names(copy_list_vec_names(list), "*"),bucket->description));
                    
          name_counter++;
       
          /* prepare list for the next fields */
                 
          list = insert_list_vec_names(insert_list_vec_names(copy_list_vec_names(list), "**"), bucket->description);
          
          if(is_record(bucket)) /* Create the derefenced record. */            
            {
            pvarname=insert_vec_names(pvarname,name_counter,copy_list_vec_names(list));  
            name_counter++;
            }
          
          break;
            
      }
* p_name_counter = name_counter;

return list;
      
}
    
/*
** insert -- insert a symbol in tabvardef.
*/ 

void insert(description, 
            type, 
            mod, 
            nclass, 
            structure, 
            signal,
	    qualifier,
            nature, 
            recdef)
char * description;
int type;
int mod;
int nclass;
int structure;
int signal;
int qualifier;
int nature;
TABDEF recdef;
{

 /* Local Var Declaration */

 TABDEF oldheader;
 TABDEF current;
 int bucket, blk_cur, id_number;

 element * list = NULL, * visited = NULL;

 blk_cur = top_int(&stack_nest);     /* # of the current block */

 /* Verify if tab_var_def is iniated at current blk */
 
 if(tab_var_def[blk_cur] == (TABDEF *) NULL)
	inicia_tab_var_def(blk_cur,SIZEHASH);
	

 bucket = hash(description);
 

 /* Verify if it is a Var Redef */

 if(!verify_redecl(&(tab_var_def[blk_cur][bucket]), description, nature))
   { /* Check the occurrence of a undefinition */
     verify_undef(description,nature);
   }

 oldheader = tab_var_def[blk_cur][bucket];
 
 /* Memory allocation for bucket  */

 tab_var_def[blk_cur][bucket]  = (TABDEF) malloc(sizeof(TABDEFCELL));
 
 if(tab_var_def[blk_cur][bucket] == (TABDEF) NULL)
     error("* * Erro Fatal: Nao consegui alocar espaco para a tabela de variaveis definidas * *\n");

 /* Salve info in tab_var_def */

 strcpy((tab_var_def[blk_cur][bucket])->description,description);
 (tab_var_def[blk_cur][bucket])->blk = blk_cur;
 (tab_var_def[blk_cur][bucket])->type = type;
 (tab_var_def[blk_cur][bucket])->next = oldheader;

 (tab_var_def[blk_cur][bucket])->mod=mod;
 (tab_var_def[blk_cur][bucket])->nclass=nclass;
 (tab_var_def[blk_cur][bucket])->structure=structure;
 (tab_var_def[blk_cur][bucket])->signal=signal;
 (tab_var_def[blk_cur][bucket])->qualifier=qualifier;
 (tab_var_def[blk_cur][bucket])->nature=nature;
 (tab_var_def[blk_cur][bucket])->recdef=recdef;
 (tab_var_def[blk_cur][bucket])->lst_of_flds = NULL;
 (tab_var_def[blk_cur][bucket])->lst_of_pars = NULL;

 /* Allocate id for variables */

 
 if(nature == VARNAME || nature == VARREC)
   {
   list=prepare_id_allocation((tab_var_def[blk_cur][bucket]), &name_counter,NULL);
            
    switch (nature)
      {
      case VARNAME:
             
        pvarname=insert_vec_names(pvarname,name_counter,list);
          
        name_counter++;

        break;
             
      case VARREC:

        if(get_mod((tab_var_def[blk_cur][bucket])) != BASIC)          
          /* If a pointer, insert the record as visited. */
          visited = insert_list_info(visited, (char *) get_rec_def((tab_var_def[blk_cur][bucket])->recdef),(tab_var_def[blk_cur][bucket])->description,name_counter-2);
          
        
        name_counter=get_no_fields((tab_var_def[blk_cur][bucket])->recdef,name_counter,visited, list);
        visited = delete_list_info(visited);
         
        break;
 
      }
   set_fut_undef((tab_var_def[blk_cur][bucket]));
   }
 
 print_bucket_info((tab_var_def[blk_cur][bucket])); 
 return;
}



/*
** insert_field -- it inserts a symbol in tabvardef.
*/ 

void insert_field(description,
                  type, 
                  mod, 
                  nclass,
                  structure, 
                  signal,
		  qualifier,
                  nature,
                  recdef,
                  recorig)
char * description;
int type;
int mod;
int nclass;
int structure;
int signal;
int qualifier;
int nature;
TABDEF recdef;  /* Pointer to type name being defined */
TABDEF recorig; /* Pointer to type name of the field, if a record */
{

 /* Local Var Declaration  */

 TABDEF bucket, current;

 /* Memory allocation for bucket  */

 bucket  = (TABDEF) malloc(sizeof(TABDEFCELL));
 if(bucket == (TABDEF) NULL)
     error("* * Erro Fatal: Nao consegui alocar espaco para a tabela de variaveis definidas * *\n");

 strcpy(bucket->description,description);
 bucket->blk = top_int(&stack_nest);
 bucket->type = type;
 
 bucket->mod=mod;
 bucket->nclass=nclass;
 bucket->structure=structure;
 bucket->signal=signal;
 bucket->qualifier=qualifier;
 bucket->nature=nature;
 bucket->recdef=recorig;
 bucket->lst_of_flds=NULL;
 bucket->lst_of_pars=NULL;
 bucket->next=NULL;
 
 /* Insert new field */

 if(recdef->lst_of_flds != NULL)
   {
   current = recdef->lst_of_flds;
   while (current->next != NULL) current=current->next;
   current->next=bucket;
   }
 else
   {
   recdef->lst_of_flds = bucket;
   }

 print_bucket_info(bucket);
 
 return;
}

/*
** insert_pars -- it inserts a formal parameter or just its prototype
**                in tabvardef.
*/ 

TABDEF insert_pars(description,
                  type, 
                  mod, 
                  nclass,
                  structure, 
                  signal,
		  qualifier,
                  nature,
                  recdef,
                  recorig)
char * description;
int type;
int mod;
int nclass;
int structure;
int signal; 
int qualifier;
int nature;
TABDEF recdef;  /* Pointer to type name being defined */
TABDEF recorig; /* Pointer to type name of the field, if a record */
{

 /* Local Var Declaration  */

 TABDEF bucket, current;

 /* Memory allocation for bucket  */

 bucket  = (TABDEF) malloc(sizeof(TABDEFCELL));
 if(bucket == (TABDEF) NULL)
     error("* * Erro Fatal: Nao consegui alocar espaco para a tabela de variaveis definidas * *\n");

 strcpy(bucket->description,description);
 bucket->blk = top_int(&stack_nest);
 bucket->type = type;
 
 bucket->mod=mod;
 bucket->nclass=nclass;
 bucket->structure=structure;
 bucket->signal=signal;
 bucket->qualifier=qualifier;
 bucket->nature=nature;
 bucket->recdef=recorig;
 bucket->lst_of_flds=NULL;
 bucket->lst_of_pars=NULL;
 bucket->next=NULL;
 
 /* Insert new parameter */

 if(recdef->lst_of_pars != NULL)
   {
   current = recdef->lst_of_pars;
   while (current->next != NULL) current=current->next;
   current->next=bucket;
   }
 else
   {
   recdef->lst_of_pars = bucket;
   }

 print_bucket_info(bucket);
 
 return bucket;
}

int get_no_fields(recdef,base,prev_visited,list)
TABDEF recdef;
int base;
element * prev_visited;
element * list;
{
int no_fields;
TABDEF current; 
element * list_aux;
element * visited=NULL;

no_fields = base;

if(recdef != NULL)
  {
   if(recdef->nature == RECDEF)
     {  
     current = get_rec_def(recdef);

     if(current != NULL)
       for(current = current->lst_of_flds; current != NULL; current=current->next)
	  {
          int eff_id;
         
	  list_aux=prepare_id_allocation(current, &no_fields,list);
  	  switch(current->nature)
	    {
	    case FLDREC:
              /* Check if record was previously allocated */
             
              eff_id = verify_field(get_rec_def(current->recdef),prev_visited,current->description,WILDCARD);

	      if(eff_id < 0)
	        {
                 /* It hasn't been visited yet */
                 /* Make a copy of the records previously visited */

                visited = copy_list_info(prev_visited);

	        visited = insert_list_info(visited, (char *) get_rec_def(current->recdef),current->description,no_fields-2);
	        no_fields=get_no_fields(get_rec_def(current),no_fields,visited,list_aux);
               
                visited = delete_list_info(visited);
	        }
              else
                {
	        /* Recursion disabled. */
	        /* Set the pointer as a repeated pointer */

                set_id_no_eff(pvarname,no_fields-2,eff_id);
               
	        /* Insert the deref pointer just to inform the existence of the heap memory object */

	        /* pvarname=insert_vec_names(pvarname,no_fields,list_aux);
	        no_fields++; */         
                }
             
	      break;
            	     
	    default:
              pvarname=insert_vec_names(pvarname,no_fields,list_aux);
	      no_fields++;
	      break;
	    }
          }
     else
       { /* It is a typedef, so a RECDEF, because it defines a type, but it is not a record. */
       list_aux=prepare_id_allocation(recdef, &no_fields,list);
       pvarname=insert_vec_names(pvarname,no_fields,list_aux);
       no_fields++;         
       }
     }
  }
return no_fields;
}

int is_par_cons(func_bucket, no_par)
TABDEF func_bucket;
int no_par;
{
 int ret = FALSE; 
 TABDEF current;
 int i = 0;

 if(func_bucket != NULL)
   for(current = func_bucket->lst_of_pars; current != NULL;
       current=current->next)
     {
     if(i == no_par)       
       if(get_qualifier(current) == CONSTANT)
	 {
	 ret = TRUE;
         break;
         }
       else
	 break;
     ++i;
     }
return ret;
}

int number_deref(id_no)
int id_no;
{
 return  no_deref(pvarname,id_no);
}

int is_deref(id_no)
double id_no;
{
 int no, ret = FALSE;

 no = no_tot_deref(pvarname,id_no);

 if(no > 0)
   ret = TRUE;
 else
   { /* To cope with recursive pointers */
   double tmp;
   tmp = ajust_double(id_no - ((int) id_no));
   if(tmp == 0.0022 || tmp == 0.0095 || tmp == 0.0043 || tmp == 0.0116 || 
      tmp == 0.0074 || tmp == 0.0095 || tmp == 0.0116 || tmp == 0.0001)
     ret = TRUE;
   }
 
 return ret;
}

int is_heap(id_no)
int id_no;
{
 int no;

 no = no_deref(pvarname,id_no);

 if(no > 0)
   return no;
 else
   return FALSE;
}

int is_pointer_deref(id_no)
int id_no;
{
 int no;

 no = no_deref_pointer(pvarname,id_no);

 if(no > 0)
   return no;
 else
   return FALSE;
}

int is_record(bucket)
TABDEF bucket;
{
 TABDEF current;
 int ret = FALSE;
 
 if(bucket != NULL)
   {
   switch(get_nature(bucket))
     {
     case RECDEF:
       if(get_rec_def(bucket) != NULL)
         ret = TRUE;
       
       break;

     default:
       
       switch(get_type(bucket))
         {
         case UNION:
         case STRUCT:
         case ENUM:
           ret = TRUE;
           break;
         
         case TYPEDEF:
           if(get_rec_def(bucket) != NULL)
             ret = TRUE;
           break;
         }
      }
   }
 
 return ret;
}

/*
** allocate_bucket -- this function allocates space for one bucket in
**                    heap memory.
*/

TABDEF allocate_bucket(description, 
            type, 
            mod, 
            nclass, 
            structure, 
            signal,
	    qualifier,
            nature, 
            recdef)
char * description;
int type;
int mod;
int nclass;
int structure;
int signal;
int qualifier;
int nature;
TABDEF recdef;
{
TABDEF bucket;

bucket = (TABDEF) malloc(sizeof(TABDEFCELL));

if(bucket == (TABDEF) NULL)
     error("* * Erro Fatal: Nao consegui alocar espaco para a tabela de variaveis definidas * *\n");

 strcpy(bucket->description,description);
 bucket->blk = top_int(&stack_nest);     /* # of the current block */

 bucket->type = type;
 bucket->next = NULL;

 bucket->mod=mod;
 bucket->nclass=nclass;
 bucket->structure=structure;
 bucket->signal=signal;
 bucket->qualifier=qualifier;
 bucket->nature=nature;
 bucket->recdef = recdef;
 bucket->lst_of_flds = NULL;
 bucket->lst_of_pars = NULL;

return (bucket);
}

/*
** deallocate_fields -- It dellocates the fields of bucket.
*/

void deallocate_fields(fld)
TABDEF fld;
{
if(fld->next != NULL)
  deallocate_fields(fld->next);
(void) deallocate_bucket(fld);
}

/*
** deallocate_bucket -- this function deallocates a bucket from tab_var_def
**                      and returns the address of the next bucket with
**                      the same hash number.
*/

TABDEF deallocate_bucket(bucket)
TABDEF bucket;
{
 TABDEF next;
 
  if(bucket->lst_of_flds!=NULL)
     deallocate_fields(bucket->lst_of_flds);

  if(bucket->lst_of_pars!=NULL)
     deallocate_fields(bucket->lst_of_pars);

  next=bucket->next;

  free(bucket);

  return(next);
}

/********************************************************************/
/* int hash(char *)                                                 */
/* Autor: Marcos L. Chaim                                           */
/* Data: 30/10/89                                                   */
/* Versao: 1.0                                                      */
/*                                                                  */
/* Funcao: calcula o indice de uma tabela "hash" a partir de um     */
/*	   "string" de caracteres.                                  */
/*                                                                  */
/* Entradas: "string" de caracteres que descreve a variavel.        */
/*                                                                  */
/********************************************************************/

int hash(description)
char * description;
{

 /* Declaracao de Variaveis Locais */

 int sum, i ;

 sum = 0;
 for(i=0; description[i] != '\0' ; ++i)
    sum = sum + description[i];
 return(sum % SIZEHASH);

}

/*
**				End of newpokelib4.c
*/














