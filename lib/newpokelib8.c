/*
// newpokelib8.c 	%I%	%Y%	%E%	%U%
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

void l_info_insert_info(list,info)
l_info * * list;
int info;
{
 l_info * listaux, * listaux1;

 listaux = malloc(sizeof(l_info));
 if(listaux == NULL)
   error_msg(mem_fault);

 listaux->x=info;
 listaux->next = NULL;

 if(*list == NULL)
   *list = listaux;
 else
   {
   for(listaux1=*list; listaux1->next != NULL; listaux1=listaux1->next);
   listaux1->next = listaux;
   }
}

void l_info_insert_2info(list,info1, info2)
l_info * * list;
int info1;
int info2;
{
 l_info * listaux, * listaux1;

 listaux = malloc(sizeof(l_info));
 if(listaux == NULL)
   error_msg(mem_fault);

 listaux->x=info1;
 listaux->y=info2;
 listaux->next = NULL;

 if(*list == NULL)
    *list = listaux;
 else
   {
   for(listaux1=*list; listaux1->next != NULL; listaux1=listaux1->next);
   listaux1->next = listaux;
   }
}

int l_info_is_in_2info(list,info1, info2)
l_info * list;
int info1;
int info2;
{
 l_info * listaux;
 int ret = FALSE;

 for(listaux=list; listaux != NULL; listaux=listaux->next)
   {
   if(listaux->x == info1 && listaux->y == info2)
     {
     ret = TRUE;
     break;
     }
   }
 return ret;
}

int l_info_is_in_info(list,info)
l_info * list;
int info;
{
 l_info * listaux;
 int ret = FALSE;

 for(listaux=list; listaux != NULL; listaux=listaux->next)
  {
  if(listaux->x == info)
    {
    ret = TRUE;
    break;
    }
  }
 return ret;
}

void l_info_print(file,list)
FILE * file;
l_info * list;
{
 l_info * listaux;
 for(listaux=list; listaux != NULL; listaux=listaux->next)
  fprintf(file," %d", listaux->x);
    
}

l_info * l_info_free(list)
l_info * list;
{
 if(list != NULL)
   {
   if(list->next != NULL)
     l_info_free(list->next);
   free(list);
   }
 return NULL;
}

void l_info_withdraw_last_info(list)
l_info * * list;
{
 l_info * listaux, * listaux1;

 if(*list != NULL)
   {
   for(listaux=listaux1=*list; listaux->next != NULL; listaux1=listaux, listaux=listaux->next);
   if(*list == listaux)
     {
     free(*list);
     *list = NULL;
     }
   else
     {
     listaux1->next = NULL;
     free(listaux);
     }
   }
}

/*
** End of newpokelib8.c
*/





