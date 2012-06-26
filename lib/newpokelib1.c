#include <stdio.h>
#include <stdlib.h>

#include "cte.h"
#include "newpokelib.h"

/*
** b_vector_cons -- initializes bit vector allocating dynamicly its memory
**                  space.
*/

void b_vector_cons(b_vector * bv, int size, int type_in)
{
  int i;
  
  bv->nbytes=size;
  bv->type_in=type_in;
  bv->vector_bits=(unsigned char *) malloc(size * sizeof(unsigned char));
  if(bv->vector_bits == NULL)
      error_msg(mem_fault);
  
  if(bv->type_in)
    for(i=0; i < bv->nbytes; ++i)
       bv->vector_bits[i] = bv->vector_bits[i] | 0xFF;    
  else
    for(i=0; i < bv->nbytes; ++i)
       bv->vector_bits[i] = bv->vector_bits[i] & 0x00;     
}

/*
** b_vector_destroy -- destroys bit vector data structure.
*/

void b_vector_destroy(b_vector * bv)
{
 if (bv->vector_bits != NULL)
   {
   free(bv->vector_bits);
   bv->vector_bits=NULL;
   }
}
 
/*
** b_vector_increase_size -- increases memory space for bit vector.
*/

void b_vector_increase_size(b_vector * bv, int new_size)
{ 
int i;
unsigned char * aux;
 
 if(bv->nbytes >= new_size)
  return;

 aux =(unsigned char *) malloc(new_size * sizeof(unsigned char));
 if(aux == NULL)
      error_msg(mem_fault);


 for(i=0; i < new_size; ++i)
    if(i < bv->nbytes)
      aux[i]=bv->vector_bits[i];
    else
      {
      /* This flag indicates whether new bytes should be set ou reset. */
      if(bv->type_in)
        aux[i]=0xFF;
      else
        aux[i]=0x00;
      }
 bv->nbytes = new_size;

 free(bv->vector_bits);

 bv->vector_bits=aux; 
}

/*
** b_vector_cpy -- copies bv_orig over bv_target.
*/

void b_vector_cpy(b_vector * bv_target, b_vector * bv_orig)
{
int i;
 
 if(bv_orig->nbytes > bv_target->nbytes)
   b_vector_increase_size(bv_target,bv_orig->nbytes);

 for(i=0; i < bv_orig->nbytes; ++i)
   bv_target->vector_bits[i]=bv_orig->vector_bits[i];    
}


/*
** set_bit -- set bit of the bit vector.
*/

void set_bit(no_id,array)
int no_id;
b_vector * array;
{

 int q,r;

 q = no_id/BYTE;
 r = no_id % BYTE;

 if(q >= array->nbytes)
   b_vector_increase_size(array, 2*q);

 switch(r)
   {
    case 0 : array->vector_bits[q] = array->vector_bits[q] | 0x80;break;
    case 1 : array->vector_bits[q] = array->vector_bits[q] | 0x40;break;
    case 2 : array->vector_bits[q] = array->vector_bits[q] | 0x20;break;
    case 3 : array->vector_bits[q] = array->vector_bits[q] | 0x10;break;
    case 4 : array->vector_bits[q] = array->vector_bits[q] | 0x08;break;
    case 5 : array->vector_bits[q] = array->vector_bits[q] | 0x04;break;
    case 6 : array->vector_bits[q] = array->vector_bits[q] | 0x02;break;
    case 7 : array->vector_bits[q] = array->vector_bits[q] | 0x01;break;
   }
 return;
}

/*
** set_bit_ncond -- set bit in bit vector 1 provided it is not
**                  in bit vector 2.
*/

void set_bit_ncond(no_id,array1,array2)
int no_id;
b_vector * array1;
b_vector * array2;
{
  if(!test_bit(no_id,array2))
    set_bit(no_id, array1);
}

/*
** set_all -- set all bits of the bit vector.
*/


void set_all(array)
b_vector * array;
{
 int i;

 for(i=0; i < array->nbytes; ++i)
       array->vector_bits[i] = array->vector_bits[i] | 0xFF;
 return;
}

/*
** reset_bit -- reset bit of the bit vector.
*/


void reset_bit(no_id,array)
int no_id;
b_vector * array;
{

 int q,r;

 q = no_id/BYTE;
 r = no_id % BYTE;

 if(q >= array->nbytes)
   b_vector_increase_size(array, 2*q);


 switch(r)
   {
    case 0 : array->vector_bits[q] = array->vector_bits[q] & 0x7F;break;
    case 1 : array->vector_bits[q] = array->vector_bits[q] & 0xBF;break;
    case 2 : array->vector_bits[q] = array->vector_bits[q] & 0xDF;break;
    case 3 : array->vector_bits[q] = array->vector_bits[q] & 0xEF;break;
    case 4 : array->vector_bits[q] = array->vector_bits[q] & 0xF7;break;
    case 5 : array->vector_bits[q] = array->vector_bits[q] & 0xFB;break;
    case 6 : array->vector_bits[q] = array->vector_bits[q] & 0xFD;break;
    case 7 : array->vector_bits[q] = array->vector_bits[q] & 0xFE;break;
   }
 return;
}

/*
** reset_all -- reset all bits of the bit vector.
*/

void reset_all(array)
b_vector * array;
{
 int i;

 for(i=0; i < array->nbytes; ++i)
       array->vector_bits[i] = array->vector_bits[i] & 0x00;
 return;
}

/*
** test_bit -- tests a bit of the bit vector.
*/

int test_bit(position,vb)
int position;
b_vector * vb;
{

 int q,r;
 unsigned char resul;
 
 q = position/BYTE;
 r = position % BYTE;

 if(q >= vb->nbytes)
   return(FALSE);

 switch(r)
   {
    case 0 : resul = vb->vector_bits[q] & 0x80;break;
    case 1 : resul = vb->vector_bits[q] & 0x40;break;
    case 2 : resul = vb->vector_bits[q] & 0x20;break;
    case 3 : resul = vb->vector_bits[q] & 0x10;break;
    case 4 : resul = vb->vector_bits[q] & 0x08;break;
    case 5 : resul = vb->vector_bits[q] & 0x04;break;
    case 6 : resul = vb->vector_bits[q] & 0x02;break;
    case 7 : resul = vb->vector_bits[q] & 0x01;break;
   }
  if(resul)
    return(TRUE);
  else
    return(FALSE);
}


/*
** empty_bit -- tests if there is at least one  bit true.
*/

int empty_bit(vb)
b_vector * vb;
{


 int i;
 unsigned char resul;

 for(i=0; i>=0 && i < vb->nbytes; ++i)
   {
     resul = vb->vector_bits[i] & 0xff;
     if(resul)
        return(FALSE);
   }

 return(TRUE);
}

/*
** neg_b_vector -- inverts all bits.
*/

void neg_b_vector(vb)
b_vector * vb;
{
 int i;
 b_vector bitaux;

 for(i=0;  i < vb->nbytes; ++i)
        vb->vector_bits[i] = ~vb->vector_bits[i];

 return;
}


/*
** union_b_vector -- produces the union of two bit vectors.
*/

b_vector * union_b_vector(vb1,vb2)
b_vector * vb1;
b_vector * vb2;
{
 int i;

 if(vb1->nbytes != vb2->nbytes)
     error_msg(bv_fault_size);
 
 for(i=0; i>=0 && i < vb1->nbytes ; ++i)
     vb1->vector_bits[i] = vb1->vector_bits[i] | vb2->vector_bits[i];


 return(vb1);
}

/*
** inter_b_vector -- produces the intersection of two bit vectors.
*/

b_vector * inter_b_vector(vb1,vb2)
b_vector * vb1;
b_vector * vb2;
{
 int i;
 

 if(vb1->nbytes != vb2->nbytes)
   {
   if(vb1->nbytes > vb2->nbytes)
     b_vector_increase_size(vb2,vb1->nbytes);
   else
     b_vector_increase_size(vb1,vb2->nbytes);
   }

 for(i=0; i>=0 && i < vb1->nbytes; ++i)
      vb1->vector_bits[i] = vb1->vector_bits[i] & vb2->vector_bits[i];


 return(vb1);
}

/*
** sub_b_vector -- subtracts one bit vector from another.
*/

b_vector * sub_b_vector(vb1,vb2)
b_vector * vb1;
b_vector * vb2;
{
 int i;
 
 if(vb1->nbytes != vb2->nbytes)
   error_msg(bv_fault_size);

 for(i=0; i>=0 && i < vb1->nbytes; ++i)
     vb1->vector_bits[i] = vb1->vector_bits[i] & (~vb2->vector_bits[i]);


 return(vb1);
}

/*
** is_equal_b_vector -- checks whether two bit vectors are equal.
*/

int is_equal_b_vector(vb1,vb2)
b_vector * vb1;
b_vector * vb2;
{
 int retorno = TRUE, i;

 if(vb1->nbytes != vb2->nbytes)
   error_msg(bv_fault_size);

 for(i=0; i>=0 && i < vb1->nbytes; ++i)
   {
     if(vb1->vector_bits[i] != vb2->vector_bits[i])
       {
        retorno = FALSE;
        break;
       }
   }
 return(retorno);
}

/*
** is_contained_in_b_vector -- checks if the first bit vector is contained
**                             in the second.
*/

b_vector * conj_aux;

int is_contained_in_b_vector(conj1, conj2)
b_vector * conj1;
b_vector * conj2;
{
 int ret=FALSE;
 b_vector conj_aux;
 b_vector_cons(&conj_aux, conj1->nbytes, conj1->type_in);
 b_vector_cpy(&conj_aux, conj1);
 inter_b_vector(&conj_aux,conj2);
 
 ret=is_equal_b_vector(&conj_aux,conj1);
 b_vector_destroy(&conj_aux);
 
 return (ret);
}


void print_b_vector(b)
b_vector * b;
{
 int j;
  for(j=0; j>=0 && j < b->nbytes*BYTE; ++j)
     if(b->type_in)
       {
       if(!test_bit(j,b))
	   fprintf(stdout,"# %d",j);
       }
     else
       {
       if(test_bit(j,b))
	   fprintf(stdout,"# %d",j);
       }
    
  fprintf(stdout,"\n");

}

int count_b_vector(b)
b_vector * b;
{
 int j;
 int count=0;
 
  for(j=0; j>=0 && j < b->nbytes*BYTE; ++j)
     if(b->type_in)
       {
       if(!test_bit(j,b))
	   count++;
       }
     else
       {
       if(test_bit(j,b))
	   count++;
       }
    
 return count;
}

int identify_b_vector(b)
b_vector * b;
{
 int j;
 int count=0;
 
  for(j=0; j>=0 && j < b->nbytes*BYTE; ++j)
     if(b->type_in)
       {
       if(!test_bit(j,b))
	   printf("%d ",j);
       }
     else
       {
       if(test_bit(j,b))
	  printf("%d ",j); 
       }
  printf("\n");   
 return count;
}

/*
** End of newpokelib1.c
*/












