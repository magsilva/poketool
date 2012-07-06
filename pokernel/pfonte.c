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
// pfonte.c			%I%	%Y%	%E% %U%
*/

#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef TURBO_C

#include <alloc.h>
#include <conio.h>

#else  /* SunOS, BSD4.2 e RISC 6000 */

#include <malloc.h>

#endif

#include "hparserg.h"
#include "hrotsem.h"
#include "header.h"
#include "headerli.h"
#include "tabelasli.h"
#include "hli.h"
#include "newpokelib.h" 

/* Variaveis Globais */

extern STACKTABLEOUT stack_descr;
extern struct tableout saida;

/*
** cat0() -- push pointer to token
*/

void cat0()
{
  TABLEOUT s;
  s.inicio = saida.inicio;
  s.comp = saida.comp;
  s.linha = saida.linha;
  push_tableout(&s,&stack_descr);
}

void cat1()
{
  TABLEOUT stopo, sant, s;

  stopo=pop_tableout(&stack_descr);
  sant=pop_tableout(&stack_descr);

  s.inicio = sant.inicio;
  s.comp = (stopo.inicio-sant.inicio)+stopo.comp;
  s.linha = stopo.linha;
  push_tableout(&s,&stack_descr);
}

void cat2()
{
  TABLEOUT sprev, stopo, sant, s;

  stopo=pop_tableout(&stack_descr);
  sant=pop_tableout(&stack_descr);
  sprev=pop_tableout(&stack_descr);

  s.inicio = sprev.inicio;
  s.comp = (stopo.inicio-sprev.inicio)+stopo.comp;
  s.linha = stopo.linha;
  push_tableout(&s,&stack_descr);
}





