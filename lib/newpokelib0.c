#include <stdio.h>
#include <stdlib.h>

#include "cte.h"
#include "newpokelib.h"

#ifdef MSGPORT     /* Portuguese Messages  */

static char * ErrorLabel[] = {"POKE Erro Interno"};

static char * ErrorMsgs [] = {
    "Nao ha' memoria disponivel.",
    "Tamanho dos vetores de bits distintos.",
    "Valor fora da faixa permitida.",
    "Descricao do arquivo incorreta.",
    "Criterio Invalido.",
    "Nao foi possivel abrir arquivo.",
    "Programa foi iniciado com numero de argumentos errado.",
    "Arquivo de descritores incorreto.",
    "Expressao Regular incorreta no automato.",
};

#endif

#ifdef MSGENG      /* English Messages  */

static char * ErrorLabel[] = {"POKE Internal Error"};

static char * ErrorMsgs[] = {
    "There is no memory avaiable.",
    "Bit vectors size are different.",
    "Value out of specified range.",
    "File description is incorrect.",
    "Invalid Criterion.",
    "Cannot open file.",
    "Program executed with wrong number of arguments.",
    "Descriptors file incorrect.",
    "Incorrect regular expression in the automaton.",
};

#endif



/*
** error_msg -- prints an error message and deallocates the heap memory.
**                  
*/

void error_msg(int msg)
{

 printf("%s [%d]: %s\n", ErrorLabel[0], msg, ErrorMsgs[msg]);

 exit(1);

}

/*
** End of newpokelib0.c
*/
































