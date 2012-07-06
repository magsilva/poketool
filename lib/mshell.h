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

/*----------------------------------------------------------------------
 *++
 * @(#)mshell.h	3.2       94/10/24
 *
 * 
 *
 *  mshell.h -- Dynamic memory handler interface
 *  Description: mshell.h provides the interface definitions for the dynamic
 *  memory handler.
 *  See mshell.c for complete documentation.
 *
 * Implantado por:  Moacir Pedroso Junior -  setembro de 1994
 *
 * Data da ultima alteracao:
 *
 *              94/10/24 14:12:44
 *+-
 *  $Log: mshell.h,v $
 *  Revision 1.1.1.1  2006-01-18 14:07:29  chaim
 *  Arquivo fonte lib
 *
 *--
 */

/* Compilation options */
#define MEM_LIST		/* Build internal list */
#define MEM_WHERE		/* Keep track of memory block source */

/* Interface functions */
unsigned long	Mem_Used(void) ;
void		Mem_Display(FILE *) ;
void		Mem_Store(char *) ;

/* Interface functions to access only through macros */
#if defined(MEM_WHERE)
void		*mem_alloc(size_t, char *, int) ;
void		*mem_calloc(size_t, size_t, char *, int) ;
void		*mem_realloc(void *, size_t, char *, int) ;
void		mem_free(void *, char *, int) ;
char		*mem_strdup(char *, char *, int) ;
#else
void		*mem_alloc(size_t) ;
void		*mem_calloc(size_t, size_t) ;
void		*mem_realloc(void *, size_t) ;
void		mem_free(void *) ;
char		*mem_strdup(char *) ;
#endif

/* Interface macros */
#if !defined(__MSHELL__)
#if defined(MEM_WHERE)
#define malloc(a)	mem_alloc((a),__FILE__,__LINE__)
#define calloc(a,b)	mem_calloc((a), (b),__FILE__,__LINE__)
#define realloc(a,b)	mem_realloc((a),(b),__FILE__,__LINE__)
#define free(a)		mem_free((a),__FILE__,__LINE__)
#define strdup(a)	mem_strdup((a),__FILE__,__LINE__)
#else
#define malloc(a)	mem_alloc(a)
#define calloc(a, b)	mem_calloc((a),(b))
#define realloc(a, b)	mem_realloc((a),(b))
#define free(a)		mem_free(a)
#define strdup(a)	mem_strdup(a)
#endif
#endif

/*----------------------------------------------------------------------*/
