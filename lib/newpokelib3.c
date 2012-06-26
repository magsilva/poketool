/*
// newpokelib3.c 	%I%	%Y%	%E%	%U%
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

/*
 * Global Vars
*/

static int size_vec_names;

/*
** Functions that access, creates and deals with the vector of names
*/

void set_size_vec_names(size)
int size;
{
  size_vec_names=size;
}

int get_vec_names_size(void)
{
return size_vec_names;
}

table_element * create_vec_names(size)
int size;
{
int i;
table_element * p_vec_names;

 p_vec_names = (table_element *) malloc(size * sizeof(table_element));

 if(p_vec_names == (table_element *) NULL)
   error("* * Erro Fatal: falta de memoria para lista de nomes * *");

 for (i=0; i < size; ++i)
   {
    p_vec_names [i].id_no = p_vec_names [i].id_no_eff = -1;
    p_vec_names [i].list = (element *) NULL;
   }
 
 set_size_vec_names(size);

 return p_vec_names;

}

table_element  * ajust_vec_names(p_vec_names, oldsize, newsize)
table_element * p_vec_names;
int oldsize, newsize;
{
int i;
table_element * pv;

 if(newsize <= oldsize)
   error("* * Erro Fatal: Novo tamanho da lista de nomes tem que ser maior do que o antigo * *");

 pv = (table_element *) create_vec_names(newsize); 

 for (i=0; i < oldsize; ++i)
    pv[i] = p_vec_names[i];

 free (p_vec_names);

 return pv;
}

void set_id_no_eff(vnames,id_number,eff_id_number)
table_element * vnames;
int id_number;
int eff_id_number;
{
 if(vnames[id_number].list == NULL)
    error("* * Erro Fatal: numero de identificacao nao definido * *");

   vnames[id_number].id_no_eff = eff_id_number;

}

table_element * insert_vec_names(vnames,id_number,list)
table_element * vnames;
int id_number;
element * list;
{
  if(id_number >= get_vec_names_size())
    {
      vnames=ajust_vec_names(vnames, get_vec_names_size(),
                          2*get_vec_names_size());
    }

  if(vnames[id_number].list != NULL)
    error("* * Erro Fatal: numero de identificacao repetido * *");

  vnames[id_number].list = list;
  vnames[id_number].id_no = id_number;
  
 return vnames;
}

element * get_names(vnames,id_number)
table_element * vnames;
int id_number;
{
  if(vnames[id_number].list == NULL)
    error("* * Erro Fatal: numero de identificacao indefinido * *");

  return(vnames[id_number].list);
}

int print_buff(buffer, frame, string, sizebuff)
char * buffer;
char * frame;
char * string;
int *sizebuff;
{
int size;

 if(*sizebuff > strlen(string)+7)
    size=sprintf(buffer,frame,string);
 else
    error("* * Erro Fatal: variavel possui nome muito longo * *");

*sizebuff-=size;

return size;
}

int compose_field(list,buff,sizebuff)
element * list;
char * buff;
int * sizebuff;
{
  int op=POINT, prevop, size, ret=DUMMY;
  element * listaux;

  listaux = list;

  /* Determine the operator */
  
  if(!strcmp(listaux->name,"*"))
    {
    op = STAR;
    /* Get the pointer name */
    listaux = listaux->next;
    }
  else          
    if(!strcmp(listaux->name,"**"))                        
      {
      op = STAR_ARROW;
      /* Get the pointer name */
      listaux = listaux->next;
      }

  if(listaux->next != NULL)
    {
    switch(op)
       {
       case STAR:
            size=print_buff(buff,"%s->",listaux->name, sizebuff);
            break;
       case STAR_ARROW:
            size=print_buff(buff,"%*s->",listaux->name,sizebuff);
            break;         
       default: /* POINT */
            size=print_buff(buff,"%s.",listaux->name,sizebuff);
            break;
       }
    
    prevop=compose_field(listaux->next,buff+size,sizebuff);

    }
  else
    {
    print_buff(buff,"%s",listaux->name,sizebuff);
           
    switch(op)
      {
      case STAR:
           prevop = STAR;
           break;
           
      case STAR_ARROW:
           prevop = STAR_ARROW;
           break;
        
      default:
           prevop = DUMMY;
           break;
      }
    }

return prevop;
}

void print_names(output,vnames,id_number)
FILE * output;
table_element * vnames;     
int id_number;
{
  element * list, * listaux;
  char buff[500];
  int  sizebuff = 500, size;
  
  int op=POINT, prevop;
    
  list = get_names(vnames,id_number);

  if(list != NULL)
    listaux = list;
  else
    error("* * Erro Fatal: Nao e' possivel imprimir lista de nomes vazia * *");
   
  /* Determine the operator */  

  if(!strcmp(listaux->name,"*"))
    {
    op = STAR;
    /* Get the pointer name */
    listaux = listaux->next;
    }
  else          
    if(!strcmp(listaux->name,"**"))                        
       {
       op = STAR_ARROW;
       /* Get the pointer name */
       listaux = listaux->next;
       }
 
  if(listaux->next != NULL)
    {
    switch(op)
       {
       case STAR:
            size=print_buff(buff,"%s->",listaux->name,&sizebuff);
            break;
       case STAR_ARROW:
            size=print_buff(buff,"*%s->",listaux->name,&sizebuff);
            break;         
       default: /* POINT */
            size=print_buff(buff,"%s.",listaux->name,&sizebuff);
            break;
       }
    
    prevop=compose_field(listaux->next,buff+size,&sizebuff);
    switch(prevop)
      {
      case STAR:
           fprintf(output,"*(%s)",buff);
           break;
      case STAR_ARROW:
           fprintf(output,"*(*(%s))",buff);
           break;
      default:        
           fprintf(output,"%s",buff);
      }
    }
  else
    switch(op)
      {
      case STAR:
           fprintf(output,"*%s",listaux->name);
           break;
           
      case STAR_ARROW:
           fprintf(output,"**%s",listaux->name);
           break;
        
      default:
           fprintf(output,"%s",listaux->name);
           break;
      }
}

int no_tot_deref_compose(list,sizederef)
element * list;
int * sizederef;
{
  int op=POINT, prevop, size=0, ret=DUMMY;
  element * listaux;

  listaux = list;

  /* Determine the operator */
  
  if(!strcmp(listaux->name,"*"))
    {
    op = STAR;
    /* Get the pointer name */
    listaux = listaux->next;
    }
  else          
    if(!strcmp(listaux->name,"**"))                        
      {
      op = STAR_ARROW;
      /* Get the pointer name */
      listaux = listaux->next;
      }

  if(listaux->next != NULL)
    {
    switch(op)
       {
       case STAR_ARROW:
            size+=2; 
            break;         
       case STAR: /* ARROW */
            ++size;
            break;
       default:;   /* POINT */
       }

    
    prevop=no_tot_deref_compose(listaux->next,&size);

    }
  else
    {
    switch(op)
      {
      case STAR:
           prevop = STAR;
           break;
           
      case STAR_ARROW:
           prevop = STAR_ARROW;
           break;
        
      default:
           prevop = DUMMY;
           break;
      }
    }
*sizederef = *sizederef + size;
return prevop;
}

int no_tot_deref(vnames,id_number)
table_element * vnames;     
int id_number;
{
  element * list, * listaux;

  int size=0;
  
  int op=POINT, prevop;
    
  list = get_names(vnames,id_number);

  if(list != NULL)
    listaux = list;
  else
    error("* * Erro Fatal: Nao e' possivel imprimir lista de nomes vazia * *");
   
  /* Determine the operator */  

  if(!strcmp(listaux->name,"*"))
    {
    op = STAR;
    /* Get the pointer name */
    listaux = listaux->next;
    }
  else          
    if(!strcmp(listaux->name,"**"))                        
       {
       op = STAR_ARROW;
       /* Get the pointer name */
       listaux = listaux->next;
       }
 
  if(listaux->next != NULL)
    {
    switch(op)
       {
       case STAR_ARROW:
            size+=2;
            break;
       case STAR: /* ARROW */
            ++size;
            break;
       default: ; /* POINT */	    
       }
    
    prevop=no_tot_deref_compose(listaux->next,&size);
    switch(prevop)
      {
      case STAR_ARROW:
	   size+=2;
           break;
      case STAR: /* ARROW */
	   ++size;
           break;	   
      default: ; /* POINT */      
           
      }
    }
  else
    switch(op)
      {
      case STAR:
	   ++size;
           break;
           
      case STAR_ARROW:
           size+=2;
           break;
        
      default:
           
           break;
      }
return size;
}

int no_deref_compose(list,sizederef)
element * list;
int * sizederef;
{
  int op=POINT, prevop, size=0, ret=DUMMY;
  element * listaux;

  listaux = list;

  /* Determine the operator */
  
  if(!strcmp(listaux->name,"*"))
    {
    op = STAR;
    /* Get the pointer name */
    listaux = listaux->next;
    }
  else          
    if(!strcmp(listaux->name,"**"))                        
      {
      op = STAR_ARROW;
      /* Get the pointer name */
      listaux = listaux->next;
      }

  if(listaux->next != NULL)
    {
    switch(op)
       {
       case STAR:
 
            break;
       case STAR_ARROW:
            size+=1; 
            break;         
       default: /* POINT */

            break;
       }

    
    prevop=no_deref_compose(listaux->next,&size);

    }
  else
    {
    switch(op)
      {
      case STAR:
           prevop = STAR;
           break;
           
      case STAR_ARROW:
           prevop = STAR_ARROW;
           break;
        
      default:
           prevop = DUMMY;
           break;
      }
    }
*sizederef = *sizederef + size;
return prevop;
}

int no_deref(vnames,id_number)
table_element * vnames;     
int id_number;
{
  element * list, * listaux;

  int size=0;
  
  int op=POINT, prevop;
    
  list = get_names(vnames,id_number);

  if(list != NULL)
    listaux = list;
  else
    error("* * Erro Fatal: Nao e' possivel imprimir lista de nomes vazia * *");
   
  /* Determine the operator */  

  if(!strcmp(listaux->name,"*"))
    {
    op = STAR;
    /* Get the pointer name */
    listaux = listaux->next;
    }
  else          
    if(!strcmp(listaux->name,"**"))                        
       {
       op = STAR_ARROW;
       /* Get the pointer name */
       listaux = listaux->next;
       }
 
  if(listaux->next != NULL)
    {
    switch(op)
       {
       case STAR:

            break;
       case STAR_ARROW:
            size+=1;
            break;         
       default: /* POINT */
            
            break;
       }
    
    prevop=no_deref_compose(listaux->next,&size);
    switch(prevop)
      {
      case STAR:
	   ++size;
           break;
      case STAR_ARROW:
	   size+=2;
           break;
      default: ;
           
      }
    }
  else
    switch(op)
      {
      case STAR:
	   ++size;
           break;
           
      case STAR_ARROW:
           size+=2;
           break;
        
      default:
           
           break;
      }
return size;
}

int no_deref_pointer(vnames,id_number)
table_element * vnames;     
int id_number;
{
  element * list, * listaux;

  int size=0;
  
  int op=POINT, prevop;
    
  list = get_names(vnames,id_number);

  if(list != NULL)
    listaux = list;
  else
    error("* * Erro Fatal: Nao e' possivel imprimir lista de nomes vazia * *");
   
  /* Determine the operator */  

  if(!strcmp(listaux->name,"*"))
    {
    op = STAR;
    /* Get the pointer name */
    listaux = listaux->next;
    }
  else          
    if(!strcmp(listaux->name,"**"))                        
       {
       op = STAR_ARROW;
       /* Get the pointer name */
       listaux = listaux->next;
       }
 
  if(listaux->next != NULL)
    {
    switch(op)
       {
       case STAR:
	    size+=1;
            break;
       case STAR_ARROW:
            size+=1;
            break;         
       default: /* POINT */
            
            break;
       }
    
    prevop=no_deref_compose(listaux->next,&size);
    switch(prevop)
      {
      case STAR:
	   ++size;
           break;
      case STAR_ARROW:
	   size+=2;
           break;
      default:;
           
      }
    }
  else
    switch(op)
      {
      case STAR:
	   ++size;
           break;
           
      case STAR_ARROW:
           size+=2;
           break;
        
      default:
           
           break;
      }
return size;
}

int list_info_size(list)
element * list;
{
 int size=0;
 element * listaux;

 for(listaux=list; listaux != NULL; listaux=listaux->next) 
   size++;

 return size;
}


element * insert_list_vec_names(list, name)
element * list;
char * name;
{
element * laux, * laux2;
  

  laux = malloc(sizeof(element));
  if(laux == (element *) NULL)
      error("* * Erro Fatal: falta de memoria para lista de nomes * *");

  laux->name=strdup(name);
  
  if (laux->name ==  NULL)
      error("* * Erro Fatal: falta de memoria para lista de nomes * *");

  laux->next=NULL;
  
  if(list != NULL)
    {
    for(laux2=list; laux2->next != NULL; laux2=laux2->next);
    laux2->next=laux;
    }
  else
    list=laux;

return list;
}

element * insert_list_info(list, name, name1, number)
element * list;
char * name;
char * name1;
int number;
{
element * laux, * laux2;
  

  laux = malloc(sizeof(element));
  if(laux == (element *) NULL)
      error("* * Erro Fatal: falta de memoria para lista de nomes * *");

  laux->name=name;
  laux->info=number;
  laux->name1=strdup(name1);
  
  if (laux->name1 ==  NULL)
      error("* * Erro Fatal: falta de memoria para lista de nomes * *");

  laux->next=NULL;
  
  if(list != NULL)
    {
    for(laux2=list; laux2->next != NULL; laux2=laux2->next);
    laux2->next=laux;
    }
  else
    list=laux;

return list;
}

element * copy_list_info(list)
element * list;
{
element * laux=NULL, * laux1 = NULL, * laux2=NULL;
  

while (list != NULL) 
  {
  /* allocate new element */
    
  laux2 = malloc(sizeof(element));
  if(laux2 == (element *) NULL)
      error("* * Erro Fatal: falta de memoria para lista de nomes * *");
  
  laux2->name=list->name;
  laux2->info=list->info;
  laux2->name1=strdup(list->name1);
  
  if (laux2->name1 ==  NULL)
      error("* * Erro Fatal: falta de memoria para lista de nomes * *");

  laux2->next = NULL;

  /* Insert element in the new list */
  
  if(laux == NULL)
    { /* First element */
      laux = laux2;
    }
  else
    { /* Inner element */
      for(laux1=laux; laux1->next != NULL; laux1=laux1->next);
      laux1->next = laux2;
    }
  
  list=list->next;
  }

return laux;
}

element *  del_elem_list_info(list, name, name1)
element * list;
char * name;
char * name1;
{
  element * laux, * laux2;
  laux=laux2=list;
  
  while(laux != NULL)
    {
      if(laux->name == name && !strcmp(laux->name1, name1))
         {
           if(laux2 == laux)
             { /* Delete the first element. */
               list = laux2 = laux->next;
               free(laux->name1);
               free(laux);
               laux=list;
               continue;
             }
           else
             { /* Delete a middle element. */
               laux2->next = laux->next;
               free(laux->name1);
               free(laux);
               laux = laux2;
             }
         }
      else
        laux2=laux;
      
     laux=laux->next; 
    }

 return list;
}

element * delete_list_info(list)
element * list;
{

  if(list != NULL)
    {
    list->next = delete_list_info(list->next);
    free(list->name1);
    free(list);
    }
 return NULL;
}

element * copy_list_vec_names(list)
element * list;
{
element * laux=NULL, * laux1 = NULL, * laux2=NULL;
  

while (list != NULL) 
  {
  /* allocate new element */
    
  laux2 = malloc(sizeof(element));
  if(laux2 == (element *) NULL)
      error("* * Erro Fatal: falta de memoria para lista de nomes * *");

  laux2->name=strdup(list->name);
  
  if (laux2->name ==  NULL)
      error("* * Erro Fatal: falta de memoria para lista de nomes * *");

  laux2->next = NULL;

  /* Insert element in the new list */
  
  if(laux == NULL)
    { /* First element */
      laux = laux2;
    }
  else
    { /* Inner element */
      for(laux1=laux; laux1->next != NULL; laux1=laux1->next);
      laux1->next = laux2;
    }
  
  list=list->next;
  }

return laux;
}

void delete_list(element * list)
{
 if(list != NULL)
   delete_list(list->next);
 else
   return;

}

void delete_vec_names(table_element * vnames)
{
  int i;
  for(i=0; i < get_vec_names_size(); ++i)
    delete_list(vnames[i].list);
  free(vnames);
}

int check_recursive_rec(vnames,id_number)
table_element * vnames;
int id_number;
{
 if(vnames[id_number].list == NULL)
    error("* * Erro Fatal: numero de identificacao nao definido * *");

 return(vnames[id_number].id_no_eff);
}

int check_recursion(vnames,id_number,field)
table_element * vnames;
int id_number;
char * field;
{
 element * laux;
 int ret = 1;
 
 if(vnames[id_number].list == NULL)
    error("* * Erro Fatal: numero de identificacao nao definido * *");

 /* jump var name */

 laux = vnames[id_number].list;

 if(laux != NULL)
   {
   if(!strcmp(laux->name,"**") || !strcmp(laux->name,"*"))
     { /* Skip pointer */
     if(laux->next == NULL)
       error("* * Erro Fatal: Na descricao do Nome * *");

     /* Point to the name of the var */
   
     laux=laux->next;
      
     }
   }
 else
   error("* * Erro Fatal: Na descricao do Nome * *");
 
 /* find out if field has already been defined */

 for(laux=laux->next; laux != NULL; laux=laux->next)
   {
   if(!strcmp(laux->name1,field))
     {
     ret = 0;
     break;
     }     
   }
 return(ret);
}

/*
**				End of newpokelib3.c
*/






