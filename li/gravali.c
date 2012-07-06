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
// gravali.c 	1.1		94/12/26	11:27:03
*/

#include <stdio.h>

#include "util.h"
#include "tabelasli.h"
#include "hli.h"

extern struct tableout saida;
extern FILE *arqli;
extern long ini_token;
extern int  comp_token,comp_ant;
extern int  lin_token;
extern int  seq;
extern int  pred;
extern int  cond;
extern int  pilha_grava_li;
extern int loc;

void grava_li  (arg,s) 
int arg;
char *s;
{
 void calcula_posicao();
 if  (!pilha_grava_li)
 {
  switch (arg) 
	{
  	case 1:

    	fprintf (arqli,"%-10s %ld %6d %6d\n",s,ini_token,comp_token,lin_token);
    	break;

  	case 2:

    	fprintf (arqli,"%s%0.2d       %ld %6d %6d\n",s,seq++,ini_token,comp_token,lin_token);
    	break;

  	case 3:

    	fprintf (arqli,"%s(%.02d)%0.2d   %ld %6d %6d\n",s,pred,cond++,ini_token,comp_token,lin_token);
    	break;

	case 4:
	   break;

  	default: printf("erro nos argumentos de grava_li\n");
	   break; 
	}

  ini_token = 0;
 }
/* if (strcmp(s,"$DCL") && strcmp(s,"$C") && strcmp(s,"$UNTIL") && strcmp(s,"$ELSE") && strcmp(s,"{") && strcmp(s,"}") && strcmp(s,"$NC") && strcmp(s,"$CC") && strcmp(s,"$ROTC") && strcmp(s,"$ROTD")&& strcmp(s,"$GOTO"))
*/
 	if (!strcmp(s,"$S") || !strcmp(s,"$IF") || !strcmp(s,"$FOR") || !strcmp(s,"$GOTO") || !strcmp(s,"$REPEAT") || !strcmp(s,"$BREAK") || !strcmp(s,"$CONTINUE") || !strcmp(s,"$CASE")|| !strcmp(s,"$WHILE") || !strcmp(s,"$RETURN"))
	
		++loc;
}

void calcula_posicao PROTO ( (void) ) { if (ini_token == 0)
   {
    ini_token=saida.inicio;
    lin_token=saida.linha;
   }
 comp_ant=comp_token;
 comp_token=((int)(saida.inicio-ini_token))+saida.comp;
}


