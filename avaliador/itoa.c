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


/*******************************************************************/
/* char * itoa(char *, int, int)                                   */
/* Autor: Marcos L. Chaim                                          */
/* Versao: 1.0                                                     */
/* Data: 19/07/91                                                  */
/*                                                                 */
/* Funcao: converte um inteiro para uma cadeia de caracteres.      */
/*                                                                 */
/* Entrada: ponteiro para a cadeia de caracteres, valor e base de  */
/*          conversao.                                             */
/*                                                                 */
/* Saida: ponteiro para a cadeia de caracteres.                    */
/*                                                                 */
/* Obs.: Esta funcao foi codificada para ser utilizada em ambien-  */
/*       te SunOS.                                                 */
/*                                                                 */
/*******************************************************************/

char * itoa(value, string, basis)
int value;
char * string;
int basis;
{
 int quo; 	/* quociente */
 int r;  	/* resto */
 char aux[25]; 	/* variavel auxiliar */
 int i;
 int j = 0;

 /* Inicializacao */

 i = 0;

 if(value < 0)
   /* se o valor e' negativo */
   quo = -value;
 else
   quo =  value;

/* Faz conversao de inteiro para caracter */

 do
   {
    r = quo%basis;
    aux[i] = r + '0';
    ++i;
    aux[i]='\0';
    quo = quo/basis;
   }
 while( quo > 0 );

 if(value < 0)
   { /* Se o valor e' negativo */
    aux[i]='-';
    ++i;
    aux[i]='\0';
   }


/* Inverte vetor auxiliar */

 j = 0;
 while(i != j)
  {
   *(string + j) = aux[i-j - 1];
   ++j;
   *(string + j) = '\0';
  }

return string;
}

