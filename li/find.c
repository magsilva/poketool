
/*--------------------------------------------------
 *
 *   Program Name:      Find ( C version)         
 *                                              
 *
 *--------------------------------------------------
 */

#include  "prog31.h"  /* ESTE COMANDO EU RETIREI*/ 
#include  "util.h"  /* ESTE COMANDO EU RETIREI*/ 
EXEC SQL BEGIN DECLARE SECTION;
 
EXEC SQL END DECLARE SECTION;


int a [256+1];

void find(n,f)
int n;
int f;
{   int   m, ns, i, j, w;
    int b;


    b = false;
    m = 1;
    ns = n;
EXEC SQL BEGIN DECLARE SECTION;
 
EXEC SQL END DECLARE SECTION;
    
   while ((m < ns) || b)
   {   if   (!b) 
       {  i = m;
          j = ns;
       }
       else
          b = false;
   /*-------------------------------*/
       if   (i > j)
       {    if   (f > j)
                 {  if  (i > f)
                        m = ns;
                    else 
                        m = i;
                 }
            else
                 ns = j;
       }
   /*-------------------------------*/
       else
       {    while  (a[i] < a[f])
                   i = i + 1 ;
            while  (a[f] < a[j])
                   j = j - 1 ;
            if   (i <= j)
            {    w = a[i];
		 a[i] = a[j]; 
                 a[j] = w;
                 i = i + 1;
                 j = j - 1;
            }
            b = true;
       }
   }
}


main()
{

find();
}
