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

		  /*  Constants' Definition */

#ifndef _CTE_

#define _CTE_

/* Teste com o numero de bytes */

#define VARTEST      10
#define NAME        -98
#define VAR         -99
#define NOTFOUND    -100
#define BYTE 8
#define SIZEHASH 53
#define MAXHASH 2*SIZEHASH-1
#define MAXLENGTH 500
#define LENGTHBYTES 30
#define NMAXVAR  ((LENGTHBYTES*8)-1)
#define NLOCALMAXVAR 100
#define NCARMAX 1024
#define TRUE 1
#define FALSE 0

#define DUMMY 202
#define ALREADY 'a'
#define NOTYET 'n'
#define INSIDE_BLK 'i'
#define IN_CASE 100
#define IN_LOOP 200
#define IN_LOOP_REPEAT 300
#define MEMORY 10
#define PATH -10
#define PIPE -20

enum country {
  brazil = 55,
  usa = 1,
};

enum type_of_parsing {
  DECL = 0,
  EXPR = 1,
};

enum error_messages {
  mem_fault = 0,
  bv_fault_size,
  out_of_range,
  wrong_file_desc,
  invalid_criterion,
  cannot_open_file,
  wrong_number_pars,
  incorrect_descriptor,
  incorrect_reg_expr,
};


enum type_symbol {
VOID,
SHORT,
INT,
LONG,
FLOAT,
DOUBLE,
CHAR,
STRUCT,                  
UNION,
TYPEDEF,
ENUM,
ENUMCONST,
FFUNC,
};

enum modifier {
BASIC,
PTR,
PTR_PTR,
PTR_FUNC,
};

enum name_class {
STATIC,
AUTOMATIC,
REGISTER,
GLOBAL,
STATIC_GLB,
TYPDEF,
};

enum structure {
SIMPLE,
VECTOR,
MATRIX,
};

enum signal {
SIGNED,
UNSIGNED,
};

enum qualifier {
COMMON,
CONSTANT,
VOLATILE,
};

enum nature {
VARIABLE = 0,      /* Simple variable: it may be an aggregated var */
FUNC = -1,         /* Name of a function */
RECDEF = -2,       /* Typedef, struct or union definition */
VARREC = -3,       /* Record variable: it can be an aggregated var */
ENUMNAME = -4,     /* A constant name defined using enum */
FLD = -5,          /* Simple field of a structure: it may be an aggregated var */
FLDREC = -6,       /* Record field of a structure: it may be an aggregated var */
PAR = -7,          /* Parmeter prototype; it may include the formal parameter  */
PAR_PAR = -8,      /* Parameter of a pointer to function parameter */
PARREC = -9,       /* Record Parmeter prototype; it may include the formal parameter  */
PAR_PARREC = -10,  /* Record Parameter of a pointer to function parameter */

CONST = -11,       /* Constant */
VARNAME = -12,     /* Name to be referred to in the source code */
};

enum opers {
  ARROW,
  POINT,
  DOUBLE_PLUS,
  DOUBLE_MINUS,
  UMPERSAND,
  STAR,
  STAR_ARROW,
  ASGOP_EQL,
  ASGOP_USE,
};

enum def_uses {
  CUSE,
  PUSE,
  DEF,
};

enum general {
  WILDCARD,
};

#endif /* _CTE_ */








