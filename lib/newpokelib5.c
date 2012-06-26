/*
// newpokelib5.c 	%I%	%Y%	%E%	%U%
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

extern table_element * pvarname;

double ajust_double(val)
     double val;
{
 double tmp;
 char buff[20];

 sprintf(buff, "%10.5f", val);
 tmp = atof(buff);
 return tmp;
}

int det_id_max(bucket,id_number,mod)
TABDEF bucket;
double id_number;
int mod;
{
TABDEF current=NULL;

/* Jump the general pointer record dereferentiation */
      
id_number+=mod;

if(bucket != NULL)
  {
  
  if(bucket->nature==RECDEF)
     if(bucket->nclass != TYPDEF)
       /* Union and Struct */
       current = bucket->lst_of_flds;
     else
       /* typedef */
       if(bucket->recdef != NULL)
         {
	   current = get_rec_def(bucket);
	   current = current->lst_of_flds;
	 }
       else
         return(id_number);
     

     while(current != NULL)
       {
       if(is_record(current))
            {
            if(check_recursive_rec(pvarname,id_number+1)<0)
              id_number=det_id_max(get_rec_def(current),id_number+1,current->mod);
            else
              /* Jump the general pointer record dereferentiation */
              id_number=id_number+mod+1;
            }
        else
          switch(current->mod)
            {
            case BASIC: id_number++;
                   break;
            case PTR:   
            case PTR_FUNC:
                   id_number+=2;
                   break;
            case PTR_PTR:
                   id_number+=3;
                   break;
            }
            
       current=current->next;
       }     
     
  }

  return(id_number);
}

double get_field_id(bucket, field, base, operator)
TABDEF bucket;
char * field;
int base;
int operator;

{
TABDEF current;

double id_number=NOTFOUND;

 if(bucket != NULL)
   {
   if(bucket->nature==RECDEF)
     {
      if(bucket->nclass != TYPDEF)
        /* Union and Struct */
        current = bucket->lst_of_flds;
      else
        /* typedef */
        if(bucket->recdef != NULL)
         {
	   current = get_rec_def(bucket);
	   current = current->lst_of_flds;
	 }
        else
          return(id_number);

     /* Ajust according to the operator */
      
     switch(operator)
       {
       case POINT: base--;
            break;
       case ARROW:
            break;
       case STAR:           
            break;
       }

     if((id_number = check_recursive_rec(pvarname,base))<0)
       id_number=base; 

      /* Jump the general pointer record dereferentiation */
      
      ++id_number;
      
      while(current != NULL)
        {
        if(!strcmp(current->description,field))
          {
          id_number++;
          break;
          }
        else
          {
         if(is_record(current))
            {
            int base;
            if(check_recursive_rec(pvarname,id_number+1)<0)
              id_number=det_id_max(get_rec_def(current),id_number+1,current->mod);
            else
              /* Jump the general pointer record dereferentiation */
              id_number=id_number+current->mod+1;
            }
         else
           switch(current->mod)
              {                
              case BASIC:
                id_number++;
                break;
              case PTR:   
              case PTR_FUNC:
                id_number+=2;
                break;
              case PTR_PTR:
                id_number+=3;
                break;
              }         
          }
         
        current=current->next;
        }
      
      /* Check if this field is the tail of a recursive record. */
      if((base=check_recursive_rec(pvarname,id_number)) > 0)
        {
        id_number=base; /* It is a recursive record field. */
        id_number+=0.0001;
        }        
     }
   }

  return(id_number);
}



/*
**				End of newpokelib5.c
*/







