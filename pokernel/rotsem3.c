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


/*
// rotsem3.c 	%I%	%Y%	%E%	%U%
*/

/*******************************************************************/
/*                                                                 */
/*               ROTINAS SEMANTICAS PARA A LINGUAGEM C             */
/*                                                                 */
/*******************************************************************/
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
#include "tabelasli.h"
#include "headerli.h"

/* Declaracao de Variaveis Globais para controle do Parser */

extern struct no_tipdef *raiz;
extern table_element * pvarname;

/* Declaracao de Variaveis Globais */

extern struct tableout saida;
extern NODEINFO info_no;
extern int num_no;
extern char oldlabel[NCARMAX+1];
extern int n_parenteses;
extern int no_blk;

extern STACKPAIR stack_var;
extern STACKPAIR symbol_type;
extern STACKMIX cur_id_bkt;
extern STACKTABLEOUT stack_descr;

extern STACK stack_nest; 
extern STACK stack_typedef;
extern STACK modifier;
extern STACK nclass;
extern STACK structure;
extern STACK signal;
extern STACK qualifier;
extern STACK nature;
extern STACK ident;
extern STACK aux_mod;
extern STACK operators;
extern STACK type_use;
extern STACK stackpar;

static char description[NCARMAX+1];

/* Flags */

static int parser_type=DECL;
static int dcl_pars=FALSE;
extern int old_ver;
extern int light_ver;

/* Miscelaneous Functions */

void set_var_info(id,bv,bs)
     int id;
     b_vector * bv;
     v_source * bs;
{
  TABLEOUT info;
  if(get_parser_type() == EXPR)
    {
      info=top_tableout(&stack_descr);
      v_source_set(bs,id,info.linha,info.inicio,info.comp);
    }
      
 set_bit(id,bv);
}

/*
** cat0() -- push pointer to token
*/

void cat0()
{
  TABLEOUT s;
  if(get_parser_type() == EXPR)
    {
      s.inicio = saida.inicio;
      s.comp = saida.comp;
      s.linha = saida.linha;
      push_tableout(&s,&stack_descr);
    }
}

void cat1()
{
  TABLEOUT stopo, sant, s;

  if(get_parser_type() == EXPR)
    {
      stopo=pop_tableout(&stack_descr);
      if(!empty_tableout(&stack_descr))
	{
	  sant=pop_tableout(&stack_descr);

	  s.inicio = sant.inicio;
	  s.comp = (stopo.inicio-sant.inicio)+stopo.comp;
	  s.linha = stopo.linha;
	}
      else
	s = stopo;
      push_tableout(&s,&stack_descr);
    }  
}

void cat2()
{
  TABLEOUT sprev, stopo, sant, s;
  if(get_parser_type() == EXPR)
    {
      stopo=pop_tableout(&stack_descr);
      sant=pop_tableout(&stack_descr);

      if(!empty_tableout(&stack_descr))
	sprev=pop_tableout(&stack_descr);
      else
	sprev=sant;

      s.inicio = sprev.inicio;
      s.comp = (stopo.inicio-sprev.inicio)+stopo.comp;
      s.linha = stopo.linha;
      push_tableout(&s,&stack_descr);
    }
}


void set_parser_type(type)
int type;
{
  parser_type = type;
}

int get_parser_type(void)
{
  return parser_type;
}

void set_pars_dcl(dcl)
int dcl;
{
  dcl_pars = dcl;
}

int get_pars_dcl(void)
{
  return dcl_pars;
}

int set_fields(b, v, bucket, start, prev_visited, mod)
b_vector * b;
v_source * v;
TABDEF bucket;
int start;
element * prev_visited;
int mod;
{
  TABDEF current, bucketorig;
 int id_number = start, eff_id;
 element * visited = NULL;
 
 if(bucket != NULL)
   {
   if(bucket->nature == RECDEF)
     {
     bucketorig = get_rec_def(bucket); // original bucket
     current = bucketorig->lst_of_flds;

     while(current != NULL)
       {
       if(current->nature == FLDREC)
         {
	 int ajust_rec;
         /* Check if it is a recursive record */
           
         
         ajust_rec=(current->mod > 0)?(current->mod-1):0;

           
         eff_id = check_recursive_rec(pvarname, id_number+ajust_rec);
	 if(eff_id < 0 && strcmp(bucketorig->description,current->recdef->description)) 
	   { /* to be recursive current cannot have the same name as bucketorig since
                it is a recursion.
	     */        
           switch(current->mod)
           {
	   case BASIC:     
	     break; 

	   case PTR:
	     set_var_info(id_number,b,v);
	     id_number+=2;
	     break;                  

	   case PTR_PTR:
	     set_var_info(id_number,b,v);
	     set_var_info(id_number+1,b,v);
	     id_number+=3;
	     break;                         
	   }                             
         
           /* It hasn't been visited yet */
           
           id_number=set_fields(b,v,get_rec_def(current),id_number,visited,
                                current->mod);
           
	   }
         else
           {
           /* Recursion disabled. By pass it. */
	   /* */
	   switch(current->mod)
	   {
	   case BASIC:     
	     set_var_info(id_number,b,v);
             ++id_number;         
	     break; 

	   case PTR:
	     set_var_info(id_number,b,v);
	     set_var_info(id_number+1,b,v);
	     id_number+=2;
	     break;                  

	   case PTR_PTR:
	     set_var_info(id_number,b,v);
	     set_var_info(id_number+1,b,v);
	     set_var_info(id_number+2,b,v);
	     id_number+=3;
	     break;                         
	   }                             
	   }
             
         }
       else            
	 switch(current->mod)
	   {
	   case BASIC:
	     set_var_info(id_number,b,v);
	     id_number++;
	     break;

	   case PTR:
	     set_var_info(id_number,b,v);
	     set_var_info(id_number+1,b,v);
	     id_number+=2;
	     break;                  

	   case PTR_PTR:
	     set_var_info(id_number,b,v);
	     set_var_info(id_number+1,b,v);
	     set_var_info(id_number+2,b,v);
	     id_number+=3;
	     break;
	     
	   }
       current=current->next;
       }
     }     
   }

 return id_number;
}

int set_all_fields(b, v, bucket, start)
b_vector * b;
v_source * v;
TABDEF bucket;
int start;
{
 int id_number = start;
 element * visited = NULL;

 if(bucket != NULL)
   {
   int ajust_rec;
   switch(get_nature(bucket))
     {            
     case FLDREC:

            if(get_mod(bucket) != BASIC)
	      {
              /* Check if it is a recursive record */
           
              ajust_rec=(get_mod(bucket) > 0)?(get_mod(bucket)-1):0;
              if(check_recursive_rec(pvarname, id_number+ajust_rec) < 0)
              	id_number=set_fields(b,v,get_rec_def(bucket),id_number+get_mod(bucket)+1,visited,bucket->mod);
	      }
            else
              id_number=set_fields(b,v,get_rec_def(bucket),id_number+1,visited,
                                   bucket->mod);
            break;
     case VARREC:
            if(get_mod(bucket) != BASIC)          
              {
              /* Check if it is a recursive record */
           
              ajust_rec=(get_mod(bucket) > 0)?(get_mod(bucket)-1):0;
              if(check_recursive_rec(pvarname, id_number+ajust_rec) < 0)
                id_number=set_fields(b,v,get_rec_def(bucket),id_number+get_mod(bucket)+1,visited,bucket->mod);
              }
            else
              id_number=set_fields(b,v,get_rec_def(bucket),id_number+1,visited,bucket->mod);

            break;
            
     case RECDEF:
          if(is_record(bucket))
            {
            id_number=set_fields(b,v,bucket,id_number+1,visited,bucket->mod);

            break;
            }

     case FLD:
     case VARNAME:
	 switch(bucket->mod)
	   {
	   case BASIC:
	     set_var_info(id_number,b,v);
	     id_number++;
	     break;

	   case PTR:
	     set_var_info(id_number,b,v);
	     set_var_info(id_number+1,b,v);
	     id_number+=2;
	     break;                  

	   case PTR_PTR:
	     set_var_info(id_number,b,v);
	     set_var_info(id_number+1,b,v);
	     set_var_info(id_number+2,b,v);
	     id_number+=3;
	     break;
	     
	   }
     }
   }

 return id_number;

}

int reset_fields(b, bucket, start, prev_visited, mod)
b_vector * b;
TABDEF bucket;
int start;
element * prev_visited;
int mod;
{
 TABDEF current;
 int id_number = start, eff_id;
 element * visited = NULL;
 
 if(bucket != NULL)
   {
   if(bucket->nature == RECDEF)
     {
     current = get_rec_def(bucket);
     current = current->lst_of_flds;

     while(current != NULL)
       {
       if(current->nature == FLDREC)
         {
	 int ajust_rec;
         /* Check if it is a recursive record */
           
         ajust_rec=(current->mod > 0)?(current->mod-1):0;
         eff_id = check_recursive_rec(pvarname, id_number+ajust_rec);
	 if(eff_id < 0)
	   {         
           switch(current->mod)
           {
	   case BASIC:     
	     break; 

	   case PTR:
	     reset_bit(id_number,b);
	     id_number+=2;
	     break;                  

	   case PTR_PTR:
	     reset_bit(id_number,b);
	     reset_bit(id_number+1,b);
	     id_number+=3;
	     break;                         
	   }                             
         
           /* It hasn't been visited yet */
           
           id_number=reset_fields(b,get_rec_def(current),id_number,visited,
                                current->mod);
           
	   }
         else
           {
           /* Recursion disabled. By pass it. */
	   /* */
	   switch(current->mod)
	   {
	   case BASIC:     
	     reset_bit(id_number,b);
             ++id_number;         
	     break; 

	   case PTR:
	     reset_bit(id_number,b);
	     reset_bit(id_number+1,b);
	     id_number+=2;
	     break;                  

	   case PTR_PTR:
	     reset_bit(id_number,b);
	     reset_bit(id_number+1,b);
	     reset_bit(id_number+2,b);
	     id_number+=3;
	     break;                         
	   }                             
	   }
             
         }
       else            
	 switch(current->mod)
	   {
	   case BASIC:
	     reset_bit(id_number,b);
	     id_number++;
	     break;

	   case PTR:
	     reset_bit(id_number,b);
	     reset_bit(id_number+1,b);
	     id_number+=2;
	     break;                  

	   case PTR_PTR:
	     reset_bit(id_number,b);
	     reset_bit(id_number+1,b);
	     reset_bit(id_number+2,b);
	     id_number+=3;
	     break;
	     
	   }
       current=current->next;
       }
     }     
   }

 return id_number;
}

int reset_all_fields(b, bucket, start)
b_vector * b;
TABDEF bucket;
int start;
{
 int id_number = start;
 element * visited = NULL;

 if(bucket != NULL)
   {
   switch(get_nature(bucket))
     {            
     case FLDREC:

            if(get_mod(bucket) != BASIC)
              id_number=reset_fields(b,get_rec_def(bucket),
                        id_number+get_mod(bucket)+1,visited,bucket->mod);
            else
              id_number=reset_fields(b,get_rec_def(bucket),id_number+1,visited,
                                   bucket->mod);
            break;
     case VARREC:
            if(get_mod(bucket) != BASIC)          
              {

              id_number=reset_fields(b,get_rec_def(bucket),id_number+get_mod(bucket)+1,visited,bucket->mod);
              }
            else
              id_number=reset_fields(b,get_rec_def(bucket),id_number+1,visited,bucket->mod);

            break;
            
     case RECDEF:
          if(is_record(bucket))
            {
            id_number=reset_fields(b,bucket,id_number+1,visited,bucket->mod);

            break;
            }

     case FLD:
     case VARNAME:
	 switch(bucket->mod)
	   {
	   case BASIC:
	     reset_bit(id_number,b);
	     id_number++;
	     break;

	   case PTR:
	     reset_bit(id_number,b);
	     reset_bit(id_number+1,b);
	     id_number+=2;
	     break;                  

	   case PTR_PTR:
	     reset_bit(id_number,b);
	     reset_bit(id_number+1,b);
	     reset_bit(id_number+2,b);
	     id_number+=3;
	     break;
	     
	   }
     }
   }

 return id_number;

}

int is_def_by_ref(id, bucket)
double id;
TABDEF bucket;
{
int ret = FALSE;

 switch(get_structure(bucket))
   {
   case VECTOR:
     if(is_one_branket(id))
       ret = TRUE;
     break;
    
   case MATRIX:
     if(is_double_branket(id))
       ret = TRUE;
     break;
    
   default:
     ret = FALSE;
   }
return ret;
}


int is_address(id)
     double id;
{
 int ret = FALSE;

 
 if(id > 0)
   {
    double tmp;
    tmp = ajust_double(id - (int) id);

     if(tmp == 0.0073 || tmp == 0.0094 || tmp == 0.0115 ||
        tmp == 0.0074 || tmp == 0.0095 || tmp == 0.0116) /* To cope with recursive pointers */
       ret = TRUE;
   }
     
 return ret;
}

int is_one_branket(id)
     double id;
{
 int ret = FALSE;

 
 if(id > 0)
   {
    double tmp;
    tmp = ajust_double(id - (int) id);

     if(tmp == 0.0021 || tmp == 0.0094 ||
        tmp == 0.0022 || tmp == 0.0095) /* To cope with recursive pointers */
       ret = TRUE;
   }
     
 return ret;
}

int is_double_branket(id)
     double id;
{
 int ret = FALSE;
 
 if(id > 0)
   {
    double tmp;
    tmp = ajust_double(id - (int) id);

     if(tmp == 0.0042 || tmp == 0.0115 ||
        tmp == 0.0043 || tmp == 0.0116) /* To cope with recursive pointers */
       ret = TRUE;
   }
     
 return ret; 
}
    
int result_var(v1, v2)
MIX * v1;
MIX * v2;
{
  TABDEF  b1,  b2;
  int id1, id2, is_array;
  int mod1=0, mod2=0, ret=0;

  b1 = (TABDEF) v1->abs;
  b2 = (TABDEF) v2->abs;
  
  id1 = v1->coor;
  id2 = v2->coor;

  /* verify address presence */
  
  switch(id1)
    {
    case FUNC:
    case CONST:
      mod1=-1;
	break;
    default:
      is_array = get_structure(b1) - is_one_branket(v1->coor) - is_double_branket(v1->coor);
      if(is_address(v1->coor)|| is_array)
	mod1=2; /* It is an address, a vector or a matrix */
      else
        mod1=0;
    }

  switch(id2)
    {
    case FUNC:
    case CONST:
      mod2=-1;
	break;
    default:
      is_array = get_structure(b2) - is_one_branket(v2->coor) - is_double_branket(v2->coor);
      if(is_address(v2->coor) || is_array)
	mod2=2; /* It is an address, a vector or a matrix */
      else
        mod2=0;
    }

  if(mod2 == 2)
    return 2;
  else
    if(mod1 == 2)
      return 1;
  
  if(b1 != NULL && mod1 != -1)
    {
    mod1 = get_mod(b1) + number_deref(id1);
    }
  
  if(b2 != NULL && mod2 != -1)
     mod2 = get_mod(b2) + number_deref(id2);

   if(mod1 > mod2)
     ret=1;
   else
     if(mod2 > mod1)
       ret = 2;
     else
       if(mod1 == mod2 && mod1 != -1)
	 ret = 2;
       else
	 ret = -1; /* both are cte */
  return ret;
}

void clear_stacks()
{
 mk_nl_int(&stack_typedef);
 mk_nl_pair(&symbol_type);
 mk_nl_int(&modifier);
 mk_nl_int(&nclass);
 mk_nl_int(&structure);
 mk_nl_int(&signal);
 mk_nl_int(&qualifier);
 mk_nl_int(&nature);
 mk_nl_int(&aux_mod);
 mk_nl_pair(&cur_id_bkt);
 mk_nl_pair(&stack_var);
 mk_nl_int(&operators);
 mk_nl_int(&type_use);
 mk_nl_int(&stackpar);
 mk_nl_tableout(&stack_descr);

}

void reset_def_var(bucket, v)
TABDEF bucket;
MIX * v;
{
int id_number;
MIX vaux;

vaux = *v;
id_number = vaux.coor;
 
 switch(get_mod(bucket))
   {
   case BASIC:
     if(is_def_by_ref(vaux.coor,bucket)||is_deref(vaux.coor))
       {
       if(is_record(bucket))
         reset_all_fields(&(info_no[num_no].def_ref),bucket,id_number);
       else
         reset_bit(id_number,&(info_no[num_no].def_ref));
       }
     else
       {
       if(is_record(bucket))
         reset_all_fields(&(info_no[num_no].defg_i),bucket,id_number);
       else
         reset_bit(id_number,&(info_no[num_no].defg_i));
       }
       break;
                  
    case PTR:
    case PTR_FUNC:
      if(is_deref(vaux.coor))
        { /* There was a dereferention operation*/
        switch(is_heap(id_number))
          { 
          case 0: /* It is a field */
            reset_bit(id_number,&(info_no[num_no].def_ref));
            /* Set def by ref of all deref vars */
	    if(is_record(bucket))
              reset_all_fields(&(info_no[num_no].def_ref),
                                    bucket,id_number);
	    else
	      reset_bit(id_number+1,&(info_no[num_no].def_ref));
            break;

	  case 1: /* It is a deref of the field */
	    if(is_record(bucket))
              reset_all_fields(&(info_no[num_no].def_ref),
                                          bucket,id_number-1);
	     else
	      reset_bit(id_number,&(info_no[num_no].def_ref));
            break;
	  }
	}
      else                     
	if(is_def_by_ref(vaux.coor,bucket))
	  {
	  reset_bit(id_number,&(info_no[num_no].def_ref));
	  /* Set def by ref of all deref vars */
          if(is_record(bucket))
	     reset_all_fields(&(info_no[num_no].def_ref),bucket,id_number);
          else
             reset_bit(id_number+1,&(info_no[num_no].def_ref));
	  }
	else
	  {
	  reset_bit(id_number,&(info_no[num_no].defg_i));
	  /* Set def of all deref vars */
	  if(is_record(bucket))
	    reset_all_fields(&(info_no[num_no].defg_i),bucket,id_number);
          else
            reset_bit(id_number+1,&(info_no[num_no].defg_i));
	  }
                      
      break;

   case PTR_PTR:

     if(is_deref(vaux.coor))
       { /* There was a dereferention operation*/
       switch(is_heap(id_number))
	 { 
	 case 0: /* It is a field */

	   reset_bit(id_number,&(info_no[num_no].def_ref));
	   reset_bit(id_number+1,&(info_no[num_no].def_ref));

	   /* Set def by ref of all deref vars */

	   if(is_record(bucket))
	     reset_all_fields(&(info_no[num_no].def_ref),bucket,id_number);
	   else
	     reset_bit(id_number+2,&(info_no[num_no].def_ref));
	   break;

	 case 1: /* It is a deref of the field */
	   reset_bit(id_number,&(info_no[num_no].def_ref));
	   if(is_record(bucket))
	     reset_all_fields(&(info_no[num_no].def_ref),bucket,id_number-1);
	   else
	     reset_bit(id_number+1,&(info_no[num_no].def_ref));
	   break;

	 case 2:
	   if(is_record(bucket))
	     reset_all_fields(&(info_no[num_no].def_ref),bucket,id_number-2);
	   else
	     reset_bit(id_number,&(info_no[num_no].def_ref));		       
	 }
       }
     else                     
       if(is_def_by_ref(vaux.coor,bucket))
	 {
	 reset_bit(id_number,&(info_no[num_no].def_ref));
	 reset_bit(id_number+1,&(info_no[num_no].def_ref));

	 /* Set def by ref of all deref vars */

	 if(is_record(bucket))
	   reset_all_fields(&(info_no[num_no].def_ref),bucket,id_number);
	 else
	   reset_bit(id_number+2,&(info_no[num_no].def_ref));
	 }
       else
	 {
	 reset_bit(id_number,&(info_no[num_no].defg_i));
	 reset_bit(id_number+1,&(info_no[num_no].defg_i));

	 /* Set def by ref of all deref vars */

	 if(is_record(bucket))
	   reset_all_fields(&(info_no[num_no].defg_i),bucket,id_number);
	 else
	   reset_bit(id_number+2,&(info_no[num_no].defg_i));	 
	 }	       
     break;
   }
}

void set_def_var(bucket, v)
TABDEF bucket;
MIX * v;
{
int id_number;
MIX vaux;

vaux = *v;
id_number = vaux.coor;
 
 switch(get_mod(bucket))
   {
   case BASIC:
     if(is_def_by_ref(vaux.coor,bucket)||is_deref(vaux.coor))
       {
       if(is_record(bucket))
         set_all_fields(&(info_no[num_no].def_ref),&(info_no[num_no].s_def_ref),
                                 bucket,id_number);
       else
         set_var_info(id_number,&(info_no[num_no].def_ref),&(info_no[num_no].s_def_ref));
       }
     else
       {
       if(is_record(bucket))
         set_all_fields(&(info_no[num_no].defg_i),&(info_no[num_no].s_defg_i),
                                 bucket,id_number);
       else
         set_var_info(id_number,&(info_no[num_no].defg_i),&(info_no[num_no].s_defg_i));
       }
       break;
                  
    case PTR:
    case PTR_FUNC:
      if(is_deref(vaux.coor))
        { /* There was a dereferention operation*/
        switch(is_heap(id_number))
          { 
          case 0: /* It is a field */
            set_var_info(id_number,&(info_no[num_no].def_ref),&(info_no[num_no].s_def_ref));
            if(!light_ver)
              {
              /* Set def by ref of all deref vars */
	      if(is_record(bucket))
                set_all_fields(&(info_no[num_no].def_ref),&(info_no[num_no].s_def_ref),
                                    bucket,id_number);
	      else
	        set_var_info(id_number+1,&(info_no[num_no].def_ref),&(info_no[num_no].s_def_ref));
	      }
            break;

	  case 1: /* It is a deref of the field */
	    if(is_record(bucket))
              set_all_fields(&(info_no[num_no].def_ref),&(info_no[num_no].s_def_ref),
                                          bucket,id_number-1);
	     else
	      set_var_info(id_number,&(info_no[num_no].def_ref),&(info_no[num_no].s_def_ref));
            break;
	  }
	}
      else                     
	if(is_def_by_ref(vaux.coor,bucket))
	  {
	  set_var_info(id_number,&(info_no[num_no].def_ref),&(info_no[num_no].s_def_ref));
          if(!light_ver)
            {
            /* Set def by ref of all deref vars */
            if(is_record(bucket))
              set_all_fields(&(info_no[num_no].def_ref),&(info_no[num_no].s_def_ref),bucket,id_number);
            else
              set_var_info(id_number+1,&(info_no[num_no].def_ref),&(info_no[num_no].s_def_ref));
            }
	  }
	else
	  {
	  set_var_info(id_number,&(info_no[num_no].defg_i),&(info_no[num_no].s_defg_i));
          if(!light_ver)
            {
	    /* Set def of all deref vars */
	    if(is_record(bucket))
	      set_all_fields(&(info_no[num_no].defg_i),&(info_no[num_no].s_defg_i),bucket,id_number);
            else
              set_var_info(id_number+1,&(info_no[num_no].defg_i),&(info_no[num_no].s_defg_i));
	    }
	  }
                      
      break;

   case PTR_PTR:

     if(is_deref(vaux.coor))
       { /* There was a dereferention operation*/
       switch(is_heap(id_number))
	 { 
	 case 0: /* It is a field */

	   set_var_info(id_number,&(info_no[num_no].def_ref),&(info_no[num_no].s_def_ref));
           if(!light_ver)
             {
	     set_var_info(id_number+1,&(info_no[num_no].def_ref),&(info_no[num_no].s_def_ref));

	     /* Set def by ref of all deref vars */

	     if(is_record(bucket))
	       set_all_fields(&(info_no[num_no].def_ref),&(info_no[num_no].s_def_ref),bucket,id_number);
	     else
	       set_var_info(id_number+2,&(info_no[num_no].def_ref),&(info_no[num_no].s_def_ref));
	     }
	   break;

	 case 1: /* It is a deref of the field */
	   set_var_info(id_number,&(info_no[num_no].def_ref),&(info_no[num_no].s_def_ref));
           if(!light_ver)
             {
 	     if(is_record(bucket))
	       set_all_fields(&(info_no[num_no].def_ref),&(info_no[num_no].s_def_ref),bucket,id_number-1);
	     else
	       set_var_info(id_number+1,&(info_no[num_no].def_ref),&(info_no[num_no].s_def_ref));
	     }
	   break;

	 case 2:
	   if(is_record(bucket))
	     set_all_fields(&(info_no[num_no].def_ref),&(info_no[num_no].s_def_ref),bucket,id_number-2);
	   else
	     set_var_info(id_number,&(info_no[num_no].def_ref),&(info_no[num_no].s_def_ref));		       
	 }
       }
     else                     
       if(is_def_by_ref(vaux.coor,bucket))
	 {
	 set_var_info(id_number,&(info_no[num_no].def_ref),&(info_no[num_no].s_def_ref));
         if(!light_ver)
           {
           set_var_info(id_number+1,&(info_no[num_no].def_ref),&(info_no[num_no].s_def_ref));

	   /* Set def by ref of all deref vars */

	   if(is_record(bucket))
	     set_all_fields(&(info_no[num_no].def_ref),&(info_no[num_no].s_def_ref),bucket,id_number);
	   else
	     set_var_info(id_number+2,&(info_no[num_no].def_ref),&(info_no[num_no].s_def_ref));
	   }
	 }
       else
	 {
	 set_var_info(id_number,&(info_no[num_no].defg_i),&(info_no[num_no].s_defg_i));
         if(!light_ver)
           {
	   set_var_info(id_number+1,&(info_no[num_no].defg_i),&(info_no[num_no].s_defg_i));

	   /* Set def by ref of all deref vars */

	   if(is_record(bucket))
	     set_all_fields(&(info_no[num_no].defg_i),&(info_no[num_no].s_defg_i),bucket,id_number);
	   else
	     set_var_info(id_number+2,&(info_no[num_no].defg_i),&(info_no[num_no].s_defg_i));	 
	   }
	 }	       
     break;
   }
}

void set_use_var(bucket, v)
TABDEF bucket;
MIX * v;
{
int id_number;
MIX vaux;

vaux = *v;
id_number = vaux.coor;
 
 switch(get_mod(bucket))
   {
   case BASIC:

     if(top_int(&type_use) == CUSE)
       set_all_fields(&(info_no[num_no].c_use),&(info_no[num_no].s_c_use),bucket,id_number);
     else
       set_all_fields(&(info_no[num_no].p_use),&(info_no[num_no].s_p_use),bucket,id_number);          
     break;
                  

   case PTR:

   case PTR_FUNC:

     if(is_deref(id_number))
       { /* There was a dereferention operation*/
	 switch(is_heap(id_number))
	   {
	   case 0:

	     if(top_int(&type_use) == CUSE)
	       set_var_info(id_number,&(info_no[num_no].c_use),&(info_no[num_no].s_c_use));
	     else
	       set_var_info(id_number,&(info_no[num_no].p_use),&(info_no[num_no].s_p_use));
	     
	     break;

	   case 1: /* It is a deref of the field */

	     if(is_record(bucket))
	       {
	       if(top_int(&type_use) == CUSE)
		 set_all_fields(&(info_no[num_no].c_use),&(info_no[num_no].s_c_use),bucket,id_number-1);
	       else
		 set_all_fields(&(info_no[num_no].p_use),&(info_no[num_no].s_p_use),bucket,id_number-1);
	       }

	     else
	       {
	       if(top_int(&type_use) == CUSE)
		 set_var_info(id_number,&(info_no[num_no].c_use),&(info_no[num_no].s_c_use));
	       else
		 set_var_info(id_number,&(info_no[num_no].p_use),&(info_no[num_no].s_p_use));
	       }
	     break;
	   }

       }
     else
       {
       if(top_int(&type_use) == CUSE)
	 set_var_info(id_number,&(info_no[num_no].c_use),&(info_no[num_no].s_c_use));
       else
	 set_var_info(id_number,&(info_no[num_no].p_use),&(info_no[num_no].s_p_use));
       }		  
     break;

   case PTR_PTR:

     if(is_deref(id_number))
       { /* There was a dereferention operation*/
	 switch(is_heap(id_number))
	   { 
	   case 0: /* It is a field */			 
	   case 1: /* It is a deref of the field */
				   
	     if(top_int(&type_use) == CUSE)
	       set_var_info(id_number,&(info_no[num_no].c_use),&(info_no[num_no].s_c_use));
	     else
	       set_var_info(id_number,&(info_no[num_no].p_use),&(info_no[num_no].s_p_use));
	     break;
			 

	   case 2:

	     if(is_record(bucket))
	       {
	       if(top_int(&type_use) == CUSE)
		 set_all_fields(&(info_no[num_no].c_use),&(info_no[num_no].s_c_use),bucket,id_number-2);
	       else
		 set_all_fields(&(info_no[num_no].p_use),&(info_no[num_no].s_p_use),bucket,id_number-2);
	       }			   
	     else
	       {			   
	       if(top_int(&type_use) == CUSE)
		 set_var_info(id_number,&(info_no[num_no].c_use),&(info_no[num_no].s_c_use));
	       else
		 set_var_info(id_number,&(info_no[num_no].p_use),&(info_no[num_no].s_p_use)); 
	       }		 
	   }
       }
     else
      {
       if(top_int(&type_use) == CUSE)
	 set_var_info(id_number,&(info_no[num_no].c_use),&(info_no[num_no].s_c_use));
       else
	 set_var_info(id_number,&(info_no[num_no].p_use),&(info_no[num_no].s_p_use));
       }		  
     break;                                
   }
          
}

void rot1_sem()
{
  PAIRINT vaux;

  vaux.coor = VOID;  /* define de type */
  vaux.abs  = 0;     /* extra info not necessary */
  
  if(empty_int(&operators))
    {
    if(!empty_int(&signal))
      {
        pop_pair( &symbol_type); /* remove default type */
        pop_pair(&signal);
      }
    push_pair(&vaux, &symbol_type);
    }
 
}
void rot2_sem()
{
  PAIRINT vaux;

  vaux.coor = CHAR;  /* define de type */
  vaux.abs  = 0;     /* extra info not necessary */
  
  if(empty_int(&operators))
    {
    if(!empty_int(&signal))
      {
        pop_pair( &symbol_type); /* remove default type */
        pop_pair(&signal);
      }
    push_pair(&vaux, &symbol_type);
    }
}
void rot3_sem()
{
    PAIRINT vaux;

  vaux.coor = SHORT;  /* define de type */
  vaux.abs  = 0;     /* extra info not necessary */

 if(!empty_int(&signal))
   {
    pop_pair( &symbol_type); /* remove default type */
    pop_pair(&signal);
   }
  push_pair(&vaux, &symbol_type);

}
void rot4_sem()
{
  PAIRINT vaux;

  vaux.coor = INT;  /* define de type */
  vaux.abs  = 0;     /* extra info not necessary */
  
  if(empty_int(&operators))
    {
    if(!empty_int(&signal))
      {
         pop_pair(&symbol_type);
         pop_pair(&signal);
      }
    push_pair(&vaux, &symbol_type);
    }
}
void rot5_sem()
{
  PAIRINT vaux;

  vaux.coor =LONG;  /* define de type */
  vaux.abs  = 0;     /* extra info not necessary */
  
  if(empty_int(&operators))
    {
     if(!empty_int(&signal))
       {
        pop_pair( &symbol_type); /* remove default type */
        pop_pair(&signal);
       }
     push_pair(&vaux, &symbol_type);
    }
}
void rot6_sem()
{
  PAIRINT vaux;

  vaux.coor = FLOAT;  /* define de type */
  vaux.abs  = 0;      /* extra info not necessary */

  if(empty_int(&operators))
    {
      /* if(!empty_int(&signal))
         pop_pair( &symbol_type);  remove default type */
    push_pair(&vaux, &symbol_type);
    }
}
void rot7_sem()
{
  PAIRINT vaux;

  vaux.coor = DOUBLE;  /* define de type */
  vaux.abs  = 0;     /* extra info not necessary */

  if(empty_int(&operators))
    {
      /* if(!empty_int(&signal))
        pop_pair( &symbol_type);  remove default type */
    push_pair(&vaux, &symbol_type);
    }
}

void rot8_sem()
{
 PAIRINT vaux;
  push_int(SIGNED, &signal);

 /* Set default type */

 vaux.coor = INT;  /* define de type */
 vaux.abs  = 0;     /* extra info not necessary */
  
 push_pair(&vaux, &symbol_type);

}
void rot9_sem()
{
 PAIRINT vaux;
  push_int(UNSIGNED, &signal);

 /* Set default type */

 vaux.coor = INT;  /* define de type */
 vaux.abs  = 0;     /* extra info not necessary */
  
  push_pair(&vaux, &symbol_type);
  
}
void rot10_sem()
{
  PAIRINT vaux;

  vaux.coor = STRUCT;  /* define de type */
  vaux.abs  = 0;       /* extra info not necessary */
  
  if(empty_int(&operators))
    push_pair(&vaux, &symbol_type);
}

void rot11_sem()
{
  PAIRINT vaux;

  vaux.coor = UNION;  /* define de type */
  vaux.abs  = 0;     /* extra info not necessary */
  
  if(empty_int(&operators))
    push_pair(&vaux, &symbol_type);
}

void rot12_sem()
{
PAIRINT vaux;
TABDEF bucket=NULL, bucket1=NULL;
char * plabel;

/* Get the type of the name */

vaux = pop_pair(&symbol_type);

if(!empty_int(&ident))
  { /* == IDENT */
   plabel = top_char(&ident);
   bucket=lookup(plabel);
   
   if(bucket == NULL)
     {
     /* create struct, union or enum entry */
     
     insert(plabel,vaux.coor, BASIC,
     ((top_int(&stack_nest) == 0)? GLOBAL:AUTOMATIC),SIMPLE,UNSIGNED,COMMON,
            ((vaux.coor == ENUM)?ENUMNAME:RECDEF),NULL);

     bucket=lookup(plabel);

     /* pop the identifier used for the struct label */

     
     }
   else
     {
       if (bucket->nclass == TYPDEF)
	 {
	   /* Bucket should be equal to the bucket that defines the
	      typedef */
	   strcpy(bucket->recdef->description,"");
	   bucket=bucket->recdef;
	 }
     }
   /* pop the identifier used for the struct label */
   
   pop_char(&ident);
  }
else
  { /* != IDENT */
   bucket=allocate_bucket("",vaux.coor, BASIC,
   ((top_int(&stack_nest) == 0)? GLOBAL:AUTOMATIC),SIMPLE,UNSIGNED,COMMON,
            ((vaux.coor == ENUM)?ENUMNAME:RECDEF),NULL);

  }

/* push struct bucket for latter recover */

vaux.abs=(int)bucket;

push_pair(&vaux,&symbol_type);

}

void rot13_sem()
{
  push_int(AUTOMATIC,&nclass);
}

void rot14_sem()
{
  push_int(REGISTER,&nclass);
}

void rot15_sem()
{
 push_int(STATIC,&nclass); 
}

void rot16_sem()
{
 push_int(GLOBAL,&nclass); 
}

void rot17_sem()
{
TABDEF bucket;
PAIRINT vaux;
 
/* Typedef declaration */       
         
/* push typedef info for latter recover */

 push_int(1,&stack_typedef);

}

void rot18_sem()
{
 push_char(saida.label, &ident); 
}

void rot19_sem()
{
 TABDEF bucket, buckaux;
 PAIRINT vaux;

 bucket = lookup(saida.label);
 vaux.coor = TYPEDEF;
 vaux.abs = (int) bucket;

 push_pair(&vaux, &symbol_type);

 buckaux = get_rec_def(bucket);

 if(buckaux != NULL)
  {
   if(buckaux->lst_of_flds != NULL)
     /* It is a record */
     push_int(VARREC,&nature);
   else
     push_int(VARNAME,&nature); 
  }
else
  push_int(VARNAME,&nature);

}

void rot20_sem()
{
 push_int(VARREC,&nature);  
}

void rot21_sem()
{
int stru;

 if(!empty_int(&structure))
   {
   stru = pop_int(&structure);
   if(stru == VECTOR)
      push_int(MATRIX, &structure);
   else
      push_int(VECTOR, &structure);
   }
 else
    push_int(VECTOR, &structure);

 if(!empty_int(&aux_mod))
   pop_int(&aux_mod);
 
}

void rot22_sem()
{
  TABDEF bucket;
  PAIRINT vaux;
  STACKPAIR stack_aux;
  
  int type;
  
  if(!empty_int(&aux_mod))
    { /* It is a pointer to function */
    
    if(!empty_pair(&symbol_type))
        {
        mk_nl_pair(&stack_aux);
        vaux = pop_pair(&symbol_type);
        push_pair(&vaux,&stack_aux);

        if(!empty_pair(&symbol_type))
          {
          vaux = pop_pair(&symbol_type);
          push_pair(&vaux,&stack_aux);

          type = vaux.coor;
        
          if(type == STRUCT || type == UNION)
             {
             push_int(FLD, & nature); /* field */
             bucket = (TABDEF) vaux.abs;
             }
          else
             push_int(VARNAME, & nature); /* variable */
          }
        else /* It is a var */
          push_int(VARNAME, & nature);
        
        while(!empty_pair(&stack_aux))
          {
          vaux = pop_pair(&stack_aux);
          push_pair(&vaux,&symbol_type);  
          }
        }
    else /* It is a var */
        push_int(VARNAME, & nature);
    
    if(!empty_int(&modifier))
      {
        pop_int(&modifier);
      }
    
    push_int(PTR_FUNC,&modifier);
   
    }
  else
    {
    push_int(FUNC, &nature);
    }
  
  if(!empty_int(&ident))
     strcpy(description, top_char(&ident));
  else
     { /* probably, ident has already been inserted */
       
     if(!empty_int(&nature))
       pop_int(&nature);

     /* To avoid the structure of the return values of the previously
        inserted be "inherited" by the parameters */
     
     if(!empty_int(&structure))
       pop_int(&structure);
     
     return;
     }

  /* Insert it in tab_var_def */

  if(top_int(&nature) != FLD)
    {
    rot36_sem(); /* insert as a variable */
    bucket = lookup_func(description);
    if (bucket == NULL)
        /*  Defined as a typedef or pointer to function variable */
        bucket = lookup(description);
        
    }
  else
    {
    rot25_sem(); /* insert as a field */
    bucket = lookup_field(bucket,description);
    }
    

  /* push function (FFUNC) and bucket in symbol_type stack */
  
  vaux.coor = FFUNC;
  vaux.abs = (int) bucket;

  push_pair(&vaux, &symbol_type);
  
}

void rot23_sem()
{
int mod;

 if(empty_int(&operators))
   if(!empty_int(&modifier))
     {
       mod = pop_int(&modifier);
       if(mod == PTR)
	 push_int(PTR_PTR, &modifier);
       else
	 push_int(PTR, &modifier);
     }
   else
     push_int(PTR, &modifier);
}

void rot24_sem()
{
 PAIRINT vaux;
 TABDEF bucket=NULL, bucket2=NULL;
 STACKPAIR stack_aux;
 char * descr;
 
 int stru, cl, sig, mod, type, nat, qual;

  mk_nl_pair(&stack_aux);
  
 /* Set symbol attributes to be inserted in tabvardef */

 /* Type */
 
 if(!empty_pair(&symbol_type))
   {
    vaux = pop_pair(&symbol_type);
    type = vaux.coor;
    if(type == STRUCT || type == UNION || type == TYPEDEF|| type == ENUM)
         bucket2 = (TABDEF)vaux.abs;  
    push_pair(&vaux,&stack_aux);
   }
 else
    type = INT;  
 
 /* Modifier */

 if(!empty_int(&modifier))
    mod = pop_int(&modifier);
 else
    mod = BASIC;

/* Name class */
 
 cl = AUTOMATIC; /* Fields follow record's class. This is a dummy value */
 
 /* Structure */
  
 if(!empty_int(&structure))
   stru = pop_int(&structure);
 else
   stru = SIMPLE;

 /* Signal */

  if(!empty_int(&signal))
    sig = top_int(&signal);
  else
    sig = SIGNED;
  
 /* Qualifier */

  if(!empty_int(&qualifier))
    qual = top_int(&qualifier);
  else
    qual = COMMON;

  /* Nature */

  if(!empty_int(&nature))
    {
      nat = top_int(&nature);
      if(nat == VARREC)
         nat = FLDREC;
      else
         nat = FLD;
    }
  else
      nat = FLD;

vaux.coor = -1; /* Dummy value, just to differentiate of STRUCT or UNION */

while(!empty_pair(&symbol_type) && vaux.coor != STRUCT && vaux.coor != UNION && vaux.coor != ENUM)
  {
    /* Finding the enclosing record definition */
    
    vaux=pop_pair(&symbol_type);
    push_pair(&vaux,&stack_aux);
  }
    
if(vaux.coor != STRUCT && vaux.coor != UNION && vaux.coor != ENUM)
  error("Erro: Nao existe definicao de estrutura para inserir campo");

bucket = (TABDEF) vaux.abs;

/* Recover stackpair symbol_type */

while(!empty_pair(&stack_aux))
      {
      vaux=pop_pair(&stack_aux);
      push_pair(&vaux,&symbol_type);  
      }
    
/* Get the name to be inserted */

 descr = top_char(&ident);

insert_field(descr, type, mod, cl, stru, sig, qual, nat, bucket, bucket2);

/* Remove ident inserted */

pop_char(&ident);

}

void rot25_sem()
{
  PAIRINT vaux;
  TABDEF bucket=NULL, bucket2=NULL;
  char * descr;
  int stru, cl, sig, mod, type, nat, qual;

 /* Set symbol attributes to be inserted in tabvardef */

 /* Type */
 
 if(!empty_pair(&symbol_type))
   {
    vaux = pop_pair(&symbol_type);
    type = vaux.coor;

    if(type == FFUNC) /* token has already been inserted */
      return;
    
    if(type == STRUCT || type == UNION || type == TYPEDEF || type == ENUM)
         bucket2 = (TABDEF)vaux.abs;
   }
 else
    type = INT;  
 
 /* Modifier */

 if(!empty_int(&modifier))
    mod = pop_int(&modifier);
 else
    mod = BASIC;

/* Name class */
 
 cl = AUTOMATIC; /* Fields follow record's class. This is a dummy value */
 
 /* Structure */
  
 if(!empty_int(&structure))
   stru = pop_int(&structure);
 else
   stru = SIMPLE;

 /* Signal */

  if(!empty_int(&signal))
    sig = pop_int(&signal);
  else
    sig = SIGNED;
  
 /* Qualifier */

  if(!empty_int(&qualifier))
   qual = pop_int(&qualifier);
  else
   qual = COMMON;
  
  /* Nature */

  if(!empty_int(&nature))
    {
      nat = pop_int(&nature);
      if(nat == VARREC)
         nat = FLDREC;
      else
         nat = FLD;
    }
  else
      nat = FLD;

 vaux.coor = -1; /* Dummy value, just to differentiate of STRUCT or UNION */

 while(!empty_pair(&symbol_type) && vaux.coor != STRUCT && vaux.coor != UNION  && vaux.coor != ENUM)
  {
    /* Finding the enclosing record definition */
    vaux=pop_pair(&symbol_type);   
  }
    
 if(vaux.coor != STRUCT && vaux.coor != UNION && vaux.coor != ENUM)
  error("Erro: Nao existe definicao de estrutura para inserir campo");

 bucket = (TABDEF) vaux.abs;
    
/* push the struct or union being defined */
    
 push_pair (&vaux, &symbol_type);
    
/* Get the name to be inserted */

 descr = top_char(&ident);

 insert_field(descr, type, mod, cl, stru, sig, qual, nat, bucket, bucket2);

/* Remove ident inserted */

 pop_char(&ident);


}

void rot26_sem()
{
 if(get_parser_type() == DECL)
   rot30_sem();
 else
   ; /* to be inserted yet */

}

void rot27_sem()
{
 if(get_parser_type() == DECL)
   rot31_sem();
 else
   ; /* to be inserted yet */

}

void rot28_sem()
{
  int mod;

  if(!empty_int(&modifier))
  {
    mod=top_int(&modifier);
    if(mod == PTR || mod == PTR_PTR)
      push_int(1,&aux_mod);
    
  }  
  
}

void rot29_sem()
{
  if(!empty_int(&aux_mod))
     pop_int(&aux_mod);

}

void rot30_sem()
{
  /* Rot26_sem */
  
  PAIRINT vaux;
  TABDEF bucket=NULL, bucket2=NULL;
  char * descr;
  int stru, cl, sig, mod, qual, type, nat;
 
 /* Set symbol attributes to be inserted in tabvardef */

 /* Type */
 
 if(!empty_pair(&symbol_type))
   {
    vaux = pop_pair(&symbol_type);
    type = vaux.coor;

    if(type == FFUNC)
      {
      push_pair(&vaux,&symbol_type);
      return; /* This name has already been inserted */
      }
    
    if(type == STRUCT || type == UNION || type == TYPEDEF || type == ENUM)
         bucket2 = (TABDEF)vaux.abs;
   }
 else
    type = INT;  
 
 /* Modifier */

 if(!empty_int(&modifier))
    mod = pop_int(&modifier);
 else
    mod = BASIC;

/* Name class */
 
 cl = AUTOMATIC; /* Fields follow record's class. This is a dummy value */
 
 /* Structure */
  
 if(!empty_int(&structure))
   stru = pop_int(&structure);
 else
   stru = SIMPLE;

 /* Signal */

  if(!empty_int(&signal))
    sig = pop_int(&signal);
  else
    sig = SIGNED;
  
 /* Qualifier */

  if(!empty_int(&qualifier))
    qual = pop_int(&qualifier);
  else
    qual = COMMON;

  /* Nature */

  if(!empty_int(&aux_mod))
      /* It is a parameter for a pointer to function. */
      nat = (bucket2 != NULL)?(PAR_PARREC):(PAR_PAR); 
  else
      nat = (bucket2 != NULL)?(PARREC):(PAR);

     
  /* Get the Function where to insert pars */

  vaux.coor = -1; /* Dummy value, just to differentiate of FFUNC */

  while(!empty_pair(&symbol_type) && vaux.coor != FFUNC)
  {
    /* Finding the enclosed function definition */
    
    vaux=pop_pair(&symbol_type);
  }
    
  if(vaux.coor != FFUNC)
     error("Erro: Nome da funcao nao foi encontrado para inserir parametro");

  bucket = (TABDEF) vaux.abs;
    
/* push the function being defined */
    
  push_pair (&vaux, &symbol_type);
    
/* Get the name to be inserted */

 if(!empty_int(&ident))
   { /* there is the name of the par to be inserted */
    descr = top_char(&ident);
    (void) insert_pars(descr, type, mod, cl, stru, sig, qual, nat, bucket, bucket2);

    if(get_pars_dcl() && (nat == PAR || nat == PARREC))
      {
        /* Insert the local parameters as local vars. */
        int nat_par;
        if(nat == PAR)
	  nat_par = VARNAME; /* simple var */
	else
	  nat_par = VARREC; /* REC var. It can a PTR or a PTR_PTR */

        insert(descr,type, mod, cl, stru, sig, qual, nat_par, bucket2);

        /* Set bit to indicate definition. */
 
        bucket = lookup(descr);
        if(bucket != NULL)
          {
          MIX vaux;

          vaux.abs = (int) bucket;
          vaux.coor = get_id(bucket);
          
          if(!old_ver) 
            set_def_var(bucket, &vaux);
          else
            /* no pointer treatment */
            set_var_info((int)vaux.coor,&(info_no[num_no].defg_i),&(info_no[num_no].s_defg_i));
          }             
      }
   
    
    /* Remove ident inserted */
 
    pop_char(&ident);
   }
 else /* there is no name */
   (void) insert_pars("", type, mod, cl, stru, sig, qual, nat, bucket, bucket2);

}

void rot31_sem()
{

 /* Rot27_sem */
  
 TABDEF bucket=NULL, bucket2=NULL;
 PAIRINT vaux;
 int stru, cl, sig, nat, mod, qual, type;
 char *descr;

 /* Set symbol attributes to be inserted in tabvardef */
 /* Structure */
  
  if(!empty_int(&structure))
        stru = pop_int(&structure);
  else
        stru = SIMPLE;

  /* Name class */
 
  if(!empty_int(&nclass))
       {
        cl=pop_int(&nclass);
        if(top_int(&stack_nest) == 0)
          {
          if(cl == STATIC)
             cl = STATIC_GLB;
          else
             cl = GLOBAL;
          }
       }
  else
       {
        if(top_int(&stack_nest) == 0)
           cl = GLOBAL;
        else
           cl = AUTOMATIC;
       }
 
  /* Signal */

  if(!empty_int(&signal))
       sig = pop_int(&signal);
  else
       sig = SIGNED;

  /* Qualifier */

  if(!empty_int(&qualifier))
       qual = pop_int(&qualifier);
  else
       qual = COMMON;

  /* Modifier */

  if(!empty_int(&modifier))
     {
       mod = pop_int(&modifier);
       if(mod != PTR)
         error("* * Erro Fatal: Esperado esperado \"ponteiro para funcao\" nao encontrado * *");
       mod = PTR_FUNC;
     }
  else
      error("* * Erro Fatal: Esperado esperado \"ponteiro para funcao\" nao encontrado * *");

  /* Nature */

  if(!empty_int(&aux_mod))
      /* It is a parameter for a pointer to function. */
      nat = (bucket2 != NULL)?(PAR_PARREC):(PAR_PAR); 
  else
      nat = (bucket2 != NULL)?(PARREC):(PAR);

  /* Type */
 
 if(!empty_pair(&symbol_type))
   {
    vaux = pop_pair(&symbol_type);
    type = vaux.coor;
    if(type == STRUCT || type == UNION || type == TYPEDEF || type == ENUM)
         bucket2 = (TABDEF)vaux.abs;
   }
 else
    type = INT;  

 /* Get bucket of the function */
 
  if(!empty_pair(&symbol_type))
      {
      vaux = top_pair(&symbol_type);
  
      if(vaux.coor == FFUNC)
             bucket = (TABDEF)vaux.abs;
      else
             error("* * Erro Fatal: Esperado \"ponteiro para funcao\" e nao encontrado * *");
      }
  else
      error("* * Erro Fatal: Esperado \"ponteiro para funcao\" e nao encontrado * *");

   /* Get the name to be inserted */

   if(!empty_int(&ident))
     strcpy(description,top_char(&ident));
   else
     strcpy(description,"");
  
   bucket = insert_pars(description, type, mod, cl, stru, sig, qual, nat, bucket, bucket2);

   if(get_pars_dcl() && (nat == PAR || nat == PARREC))
      {
        /* Insert the local parameters as local vars. */
        insert(descr,type, mod, cl, stru, sig, qual, VARNAME, bucket);

        /* Set bit to indicate definition. */
 
        bucket = lookup(descr);
        if(bucket != NULL)
          {
          MIX vaux;

          vaux.abs = (int) bucket;
          vaux.coor = get_id(bucket);

          if(!old_ver) 
            set_def_var(bucket, &vaux);
          else
            /* no pointer treatment */
            set_var_info((int)vaux.coor,&(info_no[num_no].defg_i),&(info_no[num_no].s_defg_i));
          }            
      }

   /* Remove ident inserted */
   
   if(!empty_int(&ident))
        pop_char(&ident);

  /* push function (FFUNC) and bucket in symbol_type stack. The objective
     is to indicate that next parameters must be inserted in list of the
     pointer to function parameter just inserted. */
  
  vaux.coor = FFUNC;
  vaux.abs = (int) bucket;

  push_pair(&vaux, &symbol_type);
 
}

void rot32_sem()
{
 MIX vaux;
 int op=DUMMY;
 TABDEF bucket;
  
 int id_number=0; /* dummy value */

 if(get_parser_type() == EXPR)
   {
   if(!empty_mix(&cur_id_bkt))
     {
      
      vaux = pop_mix(&cur_id_bkt);

      /* Check the "real" value of id_number */
      
      id_number = vaux.coor;
      bucket = (TABDEF) vaux.abs;
            
      if(id_number > 0 && !is_address(vaux.coor) /*&&
						   is_def_by_ref(vaux.coor,bucket)*/)  /* check if there is a use of arrays */      
        { /* Set use of id number */
        if(!old_ver)
          set_use_var((TABDEF)vaux.abs, &vaux);
        else
          {
	  /* no pointer treatment */
          if(!is_pointer_deref(id_number))
            if(top_int(&type_use) == CUSE)
              set_var_info(id_number,&(info_no[num_no].c_use),&(info_no[num_no].s_c_use));
            else
              set_var_info(id_number,&(info_no[num_no].p_use),&(info_no[num_no].s_p_use));
	  }
        }
     }     
   }
}

void rot33_sem()
{
 MIX vaux;
 TABDEF bucket;
 
 int id_number=0; /* dummy value */
 if(get_parser_type() == EXPR)
   if(!empty_mix(&cur_id_bkt))
     {
      vaux = top_mix(&cur_id_bkt);

      id_number = vaux.coor;
      bucket = (TABDEF) vaux.abs;

      if(bucket != NULL)
        if(id_number >= 0)        
          if(!old_ver)
            /* Set def of id number */
	    set_def_var(bucket,&vaux);                     
          else
            {
	    /* no pointer treatment */
            if(!is_pointer_deref(id_number))
	      {
              if(is_def_by_ref(vaux.coor,bucket))
                set_var_info(id_number,&(info_no[num_no].def_ref),&(info_no[num_no].s_def_ref));
              else
                set_var_info(id_number,&(info_no[num_no].defg_i),&(info_no[num_no].s_defg_i));
              }
	    }
     }
rot63_sem();
}

void rot34_sem()
{
 MIX vaux;
 TABDEF bucket;

 int id_number=0; /* dummy value */
 
 if(get_parser_type() == EXPR)
   if(!empty_mix(&cur_id_bkt))
     {
      vaux = top_mix(&cur_id_bkt);
      id_number = vaux.coor;
      bucket = (TABDEF) vaux.abs;

      if(bucket != NULL)      
        if(id_number >= 0)
	  {
          if(!old_ver)
            {
            /* Set def & use of id number */
	    set_def_var(bucket,&vaux);
	    set_use_var(bucket,&vaux);
	    }
          else
	    {
	    /* no pointer treatment */
            if(!is_pointer_deref(id_number))
              {
              /* Set def & use of id number */
              if(is_def_by_ref(vaux.coor,bucket))
                set_var_info(id_number,&(info_no[num_no].def_ref),&(info_no[num_no].s_def_ref));
              else
                set_var_info(id_number,&(info_no[num_no].defg_i),&(info_no[num_no].s_defg_i));

              if(top_int(&type_use) == CUSE)
                set_var_info(id_number,&(info_no[num_no].c_use),&(info_no[num_no].s_c_use));
              else
                set_var_info(id_number,&(info_no[num_no].p_use),&(info_no[num_no].s_p_use));
	      }
	    }
	  }
     }  
 rot63_sem();
}

void rot35_sem()
{
  push_int(CONSTANT, &qualifier); 
}
 
void rot36_sem()
{
 PAIRINT vaux;
 TABDEF bucket=NULL;
 char * descr;
 int stru, cl, sig, nat, mod, qual, type;


 /* Set symbol attributes to be inserted in tabvardef */

 /* Structure */
  
 if(!empty_int(&structure))
   stru = pop_int(&structure);
 else
   stru = SIMPLE;

 /* Name class */
 
 if(!empty_int(&nclass))
   {
   cl=top_int(&nclass);
   if(top_int(&stack_nest) == 0)
     {
     if(cl == STATIC)
         cl = STATIC_GLB;
     else
         cl = GLOBAL;
     }
   }
 else
   {
   if(top_int(&stack_nest) == 0)
     cl = GLOBAL;
   else
     cl = AUTOMATIC;
    
   }
 
 /* Signal */

  if(!empty_int(&signal))
    sig = top_int(&signal);
  else
    sig = SIGNED;

  /* Qualifier */

  if(!empty_int(&qualifier))
    qual = top_int(&qualifier);
  else
    qual = COMMON;


 /* Modifier */

 if(!empty_int(&modifier))
    mod = pop_int(&modifier);
 else
    mod = BASIC;

 /* Nature */

 if(!empty_int(&nature))
      nat = top_int(&nature);  
 else
      nat = VARNAME;

 /* Type */

  if(!empty_pair(&symbol_type))
   {
    vaux = top_pair(&symbol_type);
    type = vaux.coor;

    if(type == FFUNC)
      {
      pop_pair(&symbol_type);
      return; /* This name has already been inserted */
      }
    
    if(type == STRUCT || type == UNION || type == TYPEDEF || type == ENUM)
         bucket = (TABDEF)vaux.abs;  
   }
 else
    type = INT;
  
 /* Get the name to be inserted */

 descr = top_char(&ident);

 /* Verify typedef */
 
 if(!empty_int(&stack_typedef))
  { /* It is a typedef definition */
   (void)top_int(&stack_typedef);
   insert(descr, type, mod, TYPDEF, stru, sig, qual, RECDEF, bucket);
  }
 else
   insert(descr, type, mod, cl, stru, sig, qual, nat, bucket);

  /* Define variable, if a global variable */

 
 if(top_int(&stack_nest) == 0 || get_pars_dcl() || (!empty_int(&operators)? (pop_int(&operators)== ASGOP_EQL): FALSE))
   { /* Global Variable or Formal Parameters Declaration */
     bucket = lookup(descr);
     if(bucket != NULL)
       {
       if(get_nature(bucket) == VARNAME || get_nature(bucket) == VARREC || get_nature(bucket) == PAR ||
          get_nature(bucket) == PARREC)
         {
         MIX vaux;

         vaux.abs = (int) bucket;
         vaux.coor = get_id(bucket);

         if(!old_ver) 
           set_def_var(bucket, &vaux);
         else
           /* no pointer treatment */
           set_var_info((int)vaux.coor,&(info_no[num_no].defg_i),&(info_no[num_no].s_defg_i));
         }
       }
   }
 
 /* Remove ident inserted */

 pop_char(&ident);

 
}

void rot37_sem()
{

PAIRINT vaux;
TABDEF bucket=NULL;

int stru, cl, sig, nat, mod, qual, type;
char *descr;

/* Set symbol attributes to be inserted in tabvardef */

 
/* Structure */
  
 if(!empty_int(&structure))
   stru = pop_int(&structure);
 else
   stru = SIMPLE;

/* Name class */
 
 if(!empty_int(&nclass))
   {
   cl=pop_int(&nclass);
   if(top_int(&stack_nest) == 0)
     {
     if(cl == STATIC)
         cl = STATIC_GLB;
     else
         cl = GLOBAL;
     }
   }
 else
   {
   if(top_int(&stack_nest) == 0)
     cl = GLOBAL;
   else
     cl = AUTOMATIC;
    
   }
 
 /* Signal */

  if(!empty_int(&signal))
    sig = pop_int(&signal);
  else
    sig = SIGNED;

 /* Qualifier */

  if(!empty_int(&qualifier))
    qual = pop_int(&qualifier);
  else
    qual = COMMON;

 /* Modifier */

 if(!empty_int(&modifier))
    mod = pop_int(&modifier);
 else
    mod = BASIC;

 /* Nature */

 if(!empty_int(&nature))
      nat = pop_int(&nature);  
 else
      nat = VARNAME;

 /* Type */

  if(!empty_pair(&symbol_type))
   {
    vaux = pop_pair(&symbol_type);
    type = vaux.coor;
    
    if(type == FFUNC)
      {
      clear_stacks();
      return; /* This name has already been inserted */
      }
    
    if(type == STRUCT || type == UNION || type == TYPEDEF || type == ENUM)
         bucket = (TABDEF)vaux.abs;
   }
 else
    type = INT;

/* Get the name to be inserted */
  
 if(empty_int(&ident))
   {
    clear_stacks();
    if(type == ENUM)
      /* deallocate bucket because there is no variable pointing to it */
      deallocate_bucket(bucket);
    
    return; /* There is no name, so it has already been inserted */
   }
 
 descr = top_char(&ident);

 /* Verify typedef */
 

 if(!empty_int(&stack_typedef))
  { /* It is a typedef definition */
   (void)pop_int(&stack_typedef);
   insert(descr, type, mod, TYPDEF, stru, sig, qual, RECDEF, bucket);
  }
 else
   {
     if(bucket != NULL) 
       mod +=bucket->mod; /* In case of a record defined as a typedef, 
				  it is need to include the modifier used
				  in the typedef definition
			       */

    insert(descr, type, mod, cl, stru, sig, qual, nat, bucket);
   }

 /* Define variable, if a global variable */
 
 if(top_int(&stack_nest) == 0 || get_pars_dcl() || (!empty_int(&operators)? (pop_int(&operators)== ASGOP_EQL): FALSE))
   { /* Global Variable or Formal Parameters Declaration */
     bucket = lookup(descr);
     if(bucket != NULL)
       {
       if(get_nature(bucket) == VARNAME || get_nature(bucket) == VARREC || get_nature(bucket) == PAR ||
          get_nature(bucket) == PARREC)
         {
         MIX vaux;

         vaux.abs = (int) bucket;
         vaux.coor = get_id(bucket);

         if(!old_ver) 
           set_def_var(bucket, &vaux);
         else
           /* no pointer treatment */
           set_var_info((int)vaux.coor,&(info_no[num_no].defg_i),&(info_no[num_no].s_defg_i));
         }
       }
  }
 
/* Remove ident inserted */

 pop_char(&ident);

 clear_stacks();

}

void rot38_sem()
{
/* strcpy(description,saida.label); */
 push_char(saida.label,&ident);
}

void rot39_sem()
{
 PAIRINT vaux;

 insert(saida.label,ENUMCONST, BASIC,((top_int(&stack_nest) == 0)? GLOBAL:AUTOMATIC),SIMPLE,UNSIGNED,COMMON, CONST,NULL);

 /* data to be inserted as a field */
 
 vaux.coor = ENUMCONST;  /* define de type */
 vaux.abs  = 0;          /* extra info not necessary */
  
 push_pair(&vaux, &symbol_type);
 push_int(CONST,&nature);
 
 push_char(saida.label, &ident);
 
}

void rot40_sem()
{
  PAIRINT vaux;

  vaux.coor = ENUM;  /* define de type */
  vaux.abs  = 0;     /* extra info not necessary */
  
  push_pair(&vaux, &symbol_type);
  
}

void rot41_sem()
{

  TABDEF newbucket=NULL, curbucket=NULL;
  MIX vaux;
  
  int id_number;
  
  /* Verify if the IDENT belongs to tab_var_def */

  newbucket = lookup(saida.label);

  if(newbucket != NULL)
    { /* It is a variable. */
    vaux.abs = (int) newbucket;
    vaux.coor = get_id(newbucket);
    }
  else 
    { /* It is not variable. Let's check if it is a function. */     
    newbucket = lookup_func(saida.label);
    if(newbucket != NULL)
        { /* It is a function. */
        vaux.abs = (int) newbucket;
        vaux.coor = FUNC;        
        }
    else
        { /* Function has not been defined nor declared. */
        vaux.abs = (int) NULL;
        vaux.coor = FUNC; 
        }
    }
  
 push_mix(&vaux, &cur_id_bkt);
 cat0();
}

void rot42_sem()
{

 TABDEF newbucket=NULL, curbucket=NULL;
 MIX vaux;
  
 int id_number;

 if(!empty_mix(&cur_id_bkt))
     {
      vaux = pop_mix(&cur_id_bkt);
      id_number = vaux.coor;
      curbucket = (TABDEF) vaux.abs;

      newbucket = lookup_field(curbucket->recdef, saida.label);

      if(newbucket != NULL)        
        {
        if(!old_ver)
          { 
          /* Find fields only in the new version */
          /* Set use of pointer */
          set_use_var(curbucket,&vaux);
	  }
        else
          {
	  /* no pointer treatment */
          if(!is_pointer_deref(id_number))
            if(top_int(&type_use) == CUSE)
              set_var_info(id_number,&(info_no[num_no].c_use),&(info_no[num_no].s_c_use));
            else
              set_var_info(id_number,&(info_no[num_no].p_use),&(info_no[num_no].s_p_use));
          }
        
	/* Get id number, and push it */
        vaux.coor =  get_field_id(curbucket->recdef, saida.label, 
                                    id_number, ARROW);
        vaux.abs = (int) newbucket;
        push_mix(&vaux,&cur_id_bkt);
        }
      else
         error("* * Erro Fatal: Nao encontrei campo na estrutura * *");
      }
 else
      error("* * Erro Fatal: Nao existe nome empilhado * *");
 rot63_sem();
}

void rot43_sem()
{
 TABDEF newbucket=NULL, curbucket=NULL;
 MIX vaux;
  
 int id_number;

 if(!empty_mix(&cur_id_bkt))
     {
      vaux = pop_mix(&cur_id_bkt);
      id_number = vaux.coor;
      curbucket = (TABDEF) vaux.abs;

      newbucket = lookup_field(curbucket->recdef, saida.label);

      if(newbucket != NULL)        
        {
        if(!old_ver)
          { 
	  /* Set use of pointer 
          set_use_var(curbucket,&vaux); */

          /* Get id number, and push it */
          vaux.coor =  get_field_id(curbucket->recdef, saida.label, id_number, POINT);
          vaux.abs = (int) newbucket;
	  }
        else
	  {
	  /* no pointer treatment */
          if(!is_pointer_deref(id_number))
            if(top_int(&type_use) == CUSE)
              set_var_info(id_number,&(info_no[num_no].c_use),&(info_no[num_no].s_c_use));
            else
              set_var_info(id_number,&(info_no[num_no].p_use),&(info_no[num_no].s_p_use));
	  }
        push_mix(&vaux,&cur_id_bkt);
        }
      else
         error("* * Erro Fatal: Nao encontrei campo na estrutura * *");
      }
 else
      error("* * Erro Fatal: Nao existe nome empilhado * *");
rot63_sem();
}

void rot44_sem()
{
  push_int(STAR,&operators);
  rot60_sem();
}

void rot45_sem()
{
  push_int(DOUBLE_PLUS,&operators);
  rot60_sem();
}

void rot46_sem()
{
  push_int(DOUBLE_MINUS,&operators);
  rot60_sem();
}

void rot47_sem()
{
 TABDEF  curbucket=NULL;
 MIX vaux;
  
 int id_number;

 if(!empty_mix(&cur_id_bkt))
     {
      int op;

      /* Pop up LHS */
      
      vaux = pop_mix(&cur_id_bkt);
      id_number = vaux.coor;
      curbucket = (TABDEF) vaux.abs;

      if((id_number != FUNC) && (id_number != CONST) && !empty_int(&operators)) /* if a pointer returned by a function 
										   or a constant (id_number == FUNC or ==
										   CONST), just don't get in!
										*/
          {
          op = pop_int(&operators);
          switch(op)
            {
            case UMPERSAND:
              /* Check if there is a previous deref */
              if(is_heap(id_number))
                { /* There is already a deref. */
                vaux.coor = id_number-1;
                }
              else
                /* 0.0073 delta indicates that it is an address */
                vaux.coor += 0.0073;           
              break;
            
            case STAR:
              if(!is_address(vaux.coor) && !(get_mod(curbucket) == BASIC &&  
                                             get_structure(curbucket) != SIMPLE))
                {
                /* Former id number was used */
		if(!old_ver || !is_pointer_deref(id_number))
		  set_use_var(curbucket, &vaux);
 
	        /* Get the dereferenced var id number */
                vaux.coor = id_number+1;
                }
              else /* Do not increment id_number if it is an address or an
                      array being assigned using dereferention */
                {
                 if(!is_address(vaux.coor))
                   vaux.coor +=0.0021;
                }
              break;
            
            case DOUBLE_PLUS:
            case DOUBLE_MINUS:
            
              if(id_number >= 0)
                { 
                if(!old_ver)
                  {
                  /* Set def & use of id number */
	          set_def_var(curbucket, &vaux);
	          set_use_var(curbucket, &vaux);
		  }
                else
                  {
	          /* no pointer treatment */
                  if(!is_pointer_deref(id_number))
                    {
                    /* Set def & use of id number */
                    if(is_def_by_ref(vaux.coor,curbucket))
                      set_var_info(id_number,&(info_no[num_no].def_ref),&(info_no[num_no].s_def_ref));
                    else
                      set_var_info(id_number,&(info_no[num_no].defg_i),&(info_no[num_no].s_defg_i));

                    if(top_int(&type_use) == CUSE)
                      set_var_info(id_number,&(info_no[num_no].c_use),&(info_no[num_no].s_c_use));
                    else
                      set_var_info(id_number,&(info_no[num_no].p_use),&(info_no[num_no].s_p_use));
	            }
                  }
                }
              break;
           }
          }
     
      /* Push back LHS */
      push_mix(&vaux,&cur_id_bkt);
     }
 rot63_sem();
}

void rot48_sem()
{
  PAIRINT vaux;
  TABDEF bucket=NULL, bucket2=NULL;
  char * descr;
  int stru, cl, sig, mod, type, qual, nat;

  /* Parameter without type definition */

  /* Set symbol attributes to be inserted in tabvardef */
  
  stru = SIMPLE;
  cl   = AUTOMATIC;
  sig  = SIGNED;
  qual = COMMON;
  mod  = BASIC;
  type = INT;
  nat  = PAR;
  
  /* Get the Function where to insert pars */

  vaux.coor = -1; /* Dummy value, just to differentiate of FFUNC */

  while(!empty_pair(&symbol_type) && vaux.coor != FFUNC)
  {
    /* Finding the enclosed function definition */
    
    vaux=pop_pair(&symbol_type);
  }
    
  if(vaux.coor != FFUNC)
     error("Erro: Nome da funcao nao foi encontrado para inserir parametro");

  bucket = (TABDEF) vaux.abs;
    
 /* push the function being defined */
    
  push_pair (&vaux, &symbol_type);
    
 /* Insert par */

 (void) insert_pars(saida.label, type, mod, cl, stru, sig, qual, nat, bucket, bucket2);

 if(get_pars_dcl() && nat == PAR)
   {
   /* Insert the local parameters as local vars. */
   insert(saida.label,type, mod, cl, stru, sig, qual, VARNAME, bucket);

   /* Set bit to indicate definition. */
   bucket = lookup(saida.label);
   if(bucket != NULL)
       set_var_info(get_id(bucket),&(info_no[1].defg_i),&(info_no[1].s_defg_i));

   }

}

void rot49_sem()
{
 push_int(ASGOP_EQL,&operators);
}

void rot50_sem()
{
 TABDEF  curbucket=NULL;
 MIX vaux;
  
 int id_number;
 int op = DUMMY;
 
 if(!empty_mix(&cur_id_bkt)) 
     {
      /* Pop up LHS */
      
      vaux = pop_mix(&cur_id_bkt);
      id_number = vaux.coor;
      curbucket = (TABDEF) vaux.abs;

      if(curbucket != NULL && (id_number != FUNC)) /* if a pointer returned by a function (id_number == FUNC), just get away.*/
        switch(get_mod(curbucket))
          {
          case BASIC:
            /* 0.0021 delta indicates that a branket has passed through */
            
            vaux.coor += 0.0021;
 
            break;
            
          case PTR:
          case PTR_FUNC:
          case PTR_PTR:
	    if(get_structure(curbucket)==SIMPLE)
              {
              /* Former id number was used */
              if(!old_ver)
                set_use_var(curbucket, &vaux);
              else
                {
        	/* no pointer treatment */
                if(!is_pointer_deref(id_number))
                  if(top_int(&type_use) == CUSE)
                    set_var_info(id_number,&(info_no[num_no].c_use),&(info_no[num_no].s_c_use));
                  else
                    set_var_info(id_number,&(info_no[num_no].p_use),&(info_no[num_no].s_p_use));
		}

              /* Get the dereferenced var id number */
              vaux.coor = id_number+1;
              }
	    else
	      /* MATRIX or VECTOR of pointers */
	       vaux.coor += 0.0021;
            break;
          }
                   
      /* Push back LHS */   
      push_mix(&vaux,&cur_id_bkt);
     }
 rot64_sem();
}

void rot51_sem()
{
 push_int(UMPERSAND,&operators);
 rot60_sem();
}

void rot52_sem()
{
 MIX vaux, vaux1;
 TABDEF bucket;
 int mod,deref,addr; 
 int id_number=0; /* dummy value */
 int no_par; /* parameter number */
 
 if(get_parser_type() == EXPR)
   {
   if(!empty_mix(&cur_id_bkt))
     {
      TABDEF func_bucket;
      int par_cons;
      int no_deref;
      
      vaux = pop_mix(&cur_id_bkt);
      id_number = vaux.coor;
      bucket = (TABDEF) vaux.abs;

      /* Determine if the parameter is a constant */
      
      vaux1 = top_mix(&cur_id_bkt);
      func_bucket = (TABDEF) vaux1.abs;
      no_par = pop_int(&stackpar);

      par_cons = is_par_cons(func_bucket, no_par);
      
      if(id_number >= 0)
	{
        mod = get_mod(bucket);
        deref = is_heap(id_number);
        addr = is_address(vaux.coor);

        if(!old_ver)
          {        
          if(mod != BASIC)
            /* This calculus is performed if it is a pointer, or a pointer
               to pointer */
            mod = mod - deref + addr;

          /* Ajust to modifier acceptable range */

          if(mod > 2)
            mod = 2;
          else
            if(mod < 0)
              mod = 0;
        
          switch(mod)
            {
            case BASIC:
              switch(get_structure(bucket))
                {
                case MATRIX:
                  if(!par_cons && (!is_double_branket(vaux.coor) || addr))
                    {
                    if(is_record(bucket))
                      set_all_fields(&(info_no[num_no].def_ref),&(info_no[num_no].s_def_ref),bucket,id_number-deref);
                    else
                      set_var_info(id_number,&(info_no[num_no].def_ref),&(info_no[num_no].s_def_ref));
                    }
                  break;
                
                case VECTOR:
                  if(!par_cons && (!is_one_branket(vaux.coor) || addr))
                    {
                    if(is_record(bucket))
                      set_all_fields(&(info_no[num_no].def_ref),&(info_no[num_no].s_def_ref),bucket,id_number-deref);
                    else
                      set_var_info(id_number,&(info_no[num_no].def_ref),&(info_no[num_no].s_def_ref));
                    }
                  break;
             
                case SIMPLE:
                  if(!par_cons && addr)
                    {
                    if(is_record(bucket))
                      set_all_fields(&(info_no[num_no].def_ref),&(info_no[num_no].s_def_ref),bucket,id_number-deref);            
                    else
                      set_var_info(id_number,&(info_no[num_no].def_ref),&(info_no[num_no].s_def_ref));
                    }
                  break;                  
                }
              /* Set the use of the variable */

              if(!par_cons && !addr)
                {
                if(top_int(&type_use) == CUSE)
                  {
                  if(is_record(bucket))
                    set_all_fields(&(info_no[num_no].c_use),&(info_no[num_no].s_c_use),bucket,id_number-deref);
                  else
                    set_var_info(id_number,&(info_no[num_no].c_use),&(info_no[num_no].s_c_use));
                  }
                else
                  {
                  if(is_record(bucket))
                    set_all_fields(&(info_no[num_no].p_use),&(info_no[num_no].s_p_use),bucket,id_number-deref);
                  else
                   set_var_info(id_number,&(info_no[num_no].p_use),&(info_no[num_no].s_p_use));
                  }
                }
              break;
		
            case PTR_PTR:
              /* Set the def by ref of the dereferenced var */
              
              if(!par_cons)
                set_var_info(id_number+1-addr,&(info_no[num_no].def_ref),&(info_no[num_no].s_def_ref));
              
              if(!par_cons)
                {
                if(is_record(bucket))
                  set_all_fields(&(info_no[num_no].def_ref),&(info_no[num_no].s_def_ref),bucket,id_number-deref);
                else
                  set_var_info(id_number+2-addr,&(info_no[num_no].def_ref),&(info_no[num_no].s_def_ref));
                }
              
              /* Set the use of the pointer vars */
              
              if(top_int(&type_use) == CUSE)
                set_var_info(id_number,&(info_no[num_no].c_use),&(info_no[num_no].s_c_use));
              else
                set_var_info(id_number,&(info_no[num_no].p_use),&(info_no[num_no].s_p_use));

              /* Set the use of the dereferenced vars */
              
              if(top_int(&type_use) == CUSE)
                set_var_info(id_number+1-addr,&(info_no[num_no].c_use),&(info_no[num_no].s_c_use));
              else
                set_var_info(id_number+1-addr,&(info_no[num_no].p_use),&(info_no[num_no].s_p_use));

              if(is_record(bucket))
                {
                if(top_int(&type_use) == CUSE)
                  set_all_fields(&(info_no[num_no].c_use),&(info_no[num_no].s_c_use),bucket,id_number-deref);
                else
                  set_all_fields(&(info_no[num_no].p_use),&(info_no[num_no].s_p_use),bucket,id_number-deref);
                }
              else
                {
                if(top_int(&type_use) == CUSE)
                  set_var_info(id_number+2-addr,&(info_no[num_no].c_use),&(info_no[num_no].s_c_use));
                else
                  set_var_info(id_number+2-addr,&(info_no[num_no].p_use),&(info_no[num_no].s_p_use));
                }
              break;
              
            case PTR:
            case PTR_FUNC:
              /* Set the use of the pointer */

              if(!addr)
                if(top_int(&type_use) == CUSE)
                  set_var_info(id_number,&(info_no[num_no].c_use),&(info_no[num_no].s_c_use));
                else
                  set_var_info(id_number,&(info_no[num_no].p_use),&(info_no[num_no].s_p_use));

              /* Set the def by ref of the dereferenced var */

              if(!is_record(bucket))
                {                
                if(!par_cons)
                  set_var_info(id_number+1-addr, &(info_no[num_no].def_ref),&(info_no[num_no].s_def_ref));
                if(top_int(&type_use) == CUSE)
                  set_var_info(id_number+1-addr,&(info_no[num_no].c_use),&(info_no[num_no].s_c_use));
                else
                  set_var_info(id_number+1-addr,&(info_no[num_no].p_use),&(info_no[num_no].s_p_use));
                }
              else
                {
                if(!par_cons)
                  set_all_fields(&(info_no[num_no].def_ref),&(info_no[num_no].s_def_ref), bucket,id_number-deref);

                /* Set the use of the dereferenced var */
                
                if(top_int(&type_use) == CUSE)
                  set_all_fields(&(info_no[num_no].c_use),&(info_no[num_no].s_c_use),bucket,id_number-deref);
                else
                  set_all_fields(&(info_no[num_no].p_use),&(info_no[num_no].s_p_use),bucket,id_number-deref);                   
              }
                                  
             break;

            }
	  }
        else 
	  {
	    /* No pointer treatment */
	  if(!is_pointer_deref(id_number))
	    { 
            switch(get_mod(bucket))
            {
            case BASIC:
              switch(get_structure(bucket))
                {
                case SIMPLE: 
                  break;
                case VECTOR:
                  if(!par_cons && (!is_one_branket(vaux.coor) || addr))
                     set_var_info(id_number,&(info_no[num_no].def_ref),&(info_no[num_no].s_def_ref));                  
                   break;
                case MATRIX:
                  if(!par_cons && (!is_double_branket(vaux.coor) || addr))
                     set_var_info(id_number,&(info_no[num_no].def_ref),&(info_no[num_no].s_def_ref));                  
                   break;
                }
            
              /* Set the use of the variable */

              if(!par_cons && !addr)
                {
                if(top_int(&type_use) == CUSE)
                  set_var_info(id_number,&(info_no[num_no].c_use),&(info_no[num_no].s_c_use));                
                else
                  set_var_info(id_number,&(info_no[num_no].p_use),&(info_no[num_no].s_p_use));
                }    
              break;  
            case PTR:
            case PTR_FUNC:
            case PTR_PTR:   
              switch(get_structure(bucket))
                {
                case SIMPLE:
                  break;
                case VECTOR:
                  if(!par_cons && !deref && (!is_one_branket(vaux.coor) || addr))
                    set_var_info(id_number,&(info_no[num_no].def_ref),&(info_no[num_no].s_def_ref));                  
 
                  break;
                case MATRIX:
                  if(!par_cons &&  !deref && (!is_double_branket(vaux.coor) || addr))
                    set_var_info(id_number,&(info_no[num_no].def_ref),&(info_no[num_no].s_def_ref));                  
 
                   break;
                 }
	      if(/*!par_cons &&*/ !addr)
                 {
                 if(top_int(&type_use) == CUSE)
                   set_var_info(id_number-deref,&(info_no[num_no].c_use),&(info_no[num_no].s_c_use));
                 else
                   set_var_info(id_number-deref,&(info_no[num_no].p_use),&(info_no[num_no].s_p_use));
                 }    
               break;  
	    }
	    }
	  }
	}
     
     /* Push the number of the parameter */
     ++no_par;
     push_int(no_par,&stackpar);
     }
   }
}


void rot53_sem()
{
 push_int(CUSE,&type_use);
}

void rot54_sem()
{
 rot32_sem();
 pop_int(&type_use);
}

void rot55_sem()
{
 MIX vaux;
 
 /* Just to indicate the presence of a constant. */
 
 if(get_parser_type() == EXPR)
   {  
   vaux.abs = (int) NULL;
   vaux.coor = CONST;       
   push_mix(&vaux, &cur_id_bkt);
   }
 cat0();
}

void rot56_sem()
{
 push_int(VOLATILE, &qualifier); 
}

void rot57_sem()
{
 push_int(0,&stackpar);
 rot63_sem();
}

void rot58_sem()
{
 MIX vaux1, vaux2;
 int id_number1=0, id_number2=0; /* dummy value */

 if(get_parser_type() == EXPR)
   {
   if(!empty_mix(&cur_id_bkt))
     {      
     vaux2 = pop_mix(&cur_id_bkt);
     id_number2 = vaux2.coor;
     if(id_number2 >= 0)
       {
       if(!old_ver)
         set_use_var((TABDEF)vaux2.abs, &vaux2);
       else
         {
         /* no pointer treatment */
         if(!is_pointer_deref(id_number2))
           if(top_int(&type_use) == CUSE)
             set_var_info(id_number2,&(info_no[num_no].c_use),&(info_no[num_no].s_c_use));
           else
             set_var_info(id_number2,&(info_no[num_no].p_use),&(info_no[num_no].s_p_use));
         }       
       }
     }
   else
     error("* * Erro Fatal: elemento ausente da pilha de variaveis * *");
   
   if(!empty_mix(&cur_id_bkt))
     {      
      vaux1 = pop_mix(&cur_id_bkt);
      id_number1 = vaux1.coor;
      if(id_number1 >= 0)
       {
       if(!old_ver)
         set_use_var((TABDEF) vaux1.abs, &vaux1); 
       else
         {              
         /* no pointer treatment */
         if(!is_pointer_deref(id_number1))
           if(top_int(&type_use) == CUSE)
             set_var_info(id_number1,&(info_no[num_no].c_use),&(info_no[num_no].s_c_use));
           else
             set_var_info(id_number1,&(info_no[num_no].p_use),&(info_no[num_no].s_p_use));
	 }
       }
     }   
   else
     error("* * Erro Fatal: elemento ausente da pilha de variaveis * *");

   /* Decide which var to push */

   switch(result_var(&vaux1,&vaux2))
     {
     case 1:
       push_mix(&vaux1,&cur_id_bkt);       
       break;

     case 0: /* both elements have the same priority, so push the newer one. */
     case 2:
     default:/* both elments are cte */    
       push_mix(&vaux2,&cur_id_bkt);
       break;
     }
   
   /* Set use of id number */
   
   if(top_int(&type_use) == CUSE)
     {
     if(id_number1 >= 0)
       {
       if(!old_ver)
         set_var_info(id_number1,&(info_no[num_no].c_use),&(info_no[num_no].s_c_use));
       else
	 {
         if(!is_pointer_deref(id_number1))
           set_var_info(id_number1,&(info_no[num_no].c_use),&(info_no[num_no].s_c_use));
	 }
       }
     if(id_number2 >= 0)
       {
       if(!old_ver)
         set_var_info(id_number2,&(info_no[num_no].c_use),&(info_no[num_no].s_c_use));
       else
	 {
         if(!is_pointer_deref(id_number2))
           set_var_info(id_number2,&(info_no[num_no].c_use),&(info_no[num_no].s_c_use));
	 }
       }
     }
   else
     {
     if(id_number1 >= 0)
       {
       if(!old_ver)
         set_var_info(id_number1,&(info_no[num_no].p_use),&(info_no[num_no].s_p_use));
       else
	 {
         if(!is_pointer_deref(id_number1))
           set_var_info(id_number1,&(info_no[num_no].p_use),&(info_no[num_no].s_p_use));
	 }
       }
     if(id_number2 >= 0)
       {
       if(!old_ver)
         set_var_info(id_number2,&(info_no[num_no].p_use),&(info_no[num_no].s_p_use));
       else
	 {
         if(!is_pointer_deref(id_number2))
           set_var_info(id_number2,&(info_no[num_no].p_use),&(info_no[num_no].s_p_use));
	 }
       }       
     }   
   } 
 rot61_sem();
}
void rot59_sem()
{
 pop_int(&stackpar);
 rot64_sem();
}
void rot60_sem()
{
  cat0();
}
void rot61_sem()
{
  cat1();
}
void rot62_sem()
{
  cat2();
}
void rot63_sem()
{
  cat0();
  cat1();
}
void rot64_sem()
{
  cat0();
  cat2();
}
void rot65_sem()
{
  rot32_sem();
  cat1();
}
void rot66_sem()
{
 MIX vaux;

 rot58_sem();
  if(get_parser_type() == EXPR)
    {
      (void) pop_mix(&cur_id_bkt);
      vaux.coor = -1;
      vaux.abs = (int) NULL; 
      push_mix(&vaux, &cur_id_bkt);
    }
}





