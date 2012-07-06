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
