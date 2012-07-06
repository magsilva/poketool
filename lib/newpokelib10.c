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

#include "cte.h"
#include "newpokelib.h"

/*
** v_source_cons -- initializes bit vector allocating dynamicly its memory
**                  space.
*/

void v_source_cons(v_source * bvec_source, int size)
{
  int i;
  TABLEOUT * vaux;
  
  vaux=(TABLEOUT *) malloc((size+1) * sizeof(TABLEOUT));
  if(vaux  == NULL)
      error_msg(mem_fault);
  
  for(i=0; i < size; ++i)
     {
     vaux[i].linha = 0;
     vaux[i].inicio = 0;
     vaux[i].comp = 0;
     }    
  bvec_source->nvars=size;
  bvec_source->vec_var_source=vaux;
}

/*
** v_source_destroy -- destroys bit vector data structure.
*/

void v_source_destroy(v_source * bv)
{
 if (bv->vec_var_source != NULL)
   {
   free(bv->vec_var_source);
   bv->vec_var_source=NULL;
   }
}
 
/*
** v_source_increase_size -- increases memory space for bit vector.
*/

void v_source_increase_size(v_source * bv, int newsize)
{ 
 int i;
 TABLEOUT * vaux;

 if(bv->nvars >= newsize)
    return;


 vaux=(TABLEOUT *) malloc(newsize * sizeof(TABLEOUT));

 if(vaux == NULL)
      error_msg(mem_fault);

 for(i=0; i < newsize; ++i)
    if(i < bv->nvars)
      vaux[i]=bv->vec_var_source[i];
    else
      {
      /* This flag indicates whether new bytes should be set ou reset. */
      vaux[i].linha = 0;
      vaux[i].inicio = 0;
      vaux[i].comp = 0;
      }

 
 free(bv->vec_var_source);

 bv->nvars = newsize;
 bv->vec_var_source=vaux; 
}

/*
** v_source_cpy -- copies bv_orig over bv_target.
*/

void v_source_cpy(v_source * bv_target, v_source * bv_orig)
{
int i;
 
 if(bv_orig->nvars > bv_target->nvars)
   v_source_increase_size(bv_target,bv_orig->nvars);

 for(i=0; i < bv_orig->nvars; ++i)
   bv_target->vec_var_source[i]=bv_orig->vec_var_source[i];    
}

void v_source_set(v_source *bv, int id, int linha, long inicio, int comp)
{

 if(id >= bv->nvars)
   v_source_increase_size(bv,2*id);

  (bv->vec_var_source[id]).linha = linha;
  (bv->vec_var_source[id]).comp = comp;
  (bv->vec_var_source[id]).inicio = inicio;

}

void v_source_print(v_source *bv, int id, FILE * srcptrfile, FILE * outfile)
{
  int c;
  long auxsize;

 auxsize = (bv->vec_var_source[id]).comp;

 fseek(srcptrfile,(bv->vec_var_source[id]).inicio-1,SEEK_SET);

 c = getc(srcptrfile);
 while(auxsize > 0 )
    {
     putc(c,outfile);

     c = getc(srcptrfile);
     --auxsize;
    }
}















