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





