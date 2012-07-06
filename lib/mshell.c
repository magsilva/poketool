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

static char *sccsid = "@(#)mshell.c	3.4           97/06/12";

/*----------------------------------------------------------------------
 *++
 * @(#)mshell.c	3.4       97/06/12
 *
 * 
 *
 *  Memory management utilities
 *
 *  Description
 *
 *	mshell.c contains routines to protect the programmer
 *	from errors in calling memory allocation/free routines.
 *	The programmer must use the memory calls defined
 *	in mshell.h. When these calls are used, the
 *	allocation routines in this module add a data structure
 *	to the top of allocated memory blocks which tags them as
 *	legal memory blocks.
 *
 *	When the free routine is called, the memory block to
 *	be freed is checked for legality tag.  If the block
 *	is not legal, the memory list is dumped to stderr and
 *	the program is terminated.
 *
 *  Compilation Options
 *
 *	MEM_LIST	Link all allocated memory blocks onto
 *				an internal list. The list can be
 *				displayed using Mem_Display().
 *
 *	MEM_WHERE	Save the file/line number of allocated
 *				blocks in the header.
 *				Requires that the compilier supports
 *				__FILE__ and __LINE__ preprocessor
 *				directives.
 *				Also requires that the __FILE__ string
 *				have a static or global scope.
 *
 * Implantado por: Moacir Pedroso Junior - setembro de 1994
 *
 * Data da ultima alteracao:
 *
 *              97/06/12 12:30:20
 *+-
 *
 *  $Log: mshell.c,v $
 *  Revision 1.1.1.1  2006-01-18 14:07:29  chaim
 *  Arquivo fonte lib
 *
 *
 *--
 */

#define __MSHELL__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mshell.h"

/* Constants */
/* --------- */
#define MEMTAG	0xa55a			/* Value for mh_tag */

/* Structures */
/* ---------- */
typedef struct memnod			/* Memory block header info	*/
	{				/* ---------------------------- */
	unsigned int	mh_tag ;	/* Special ident tag		*/
	size_t		mh_size ;	/* Size of allocation block	*/
#if defined(MEM_LIST)
	struct memnod	*mh_next ;	/* Next memory block		*/
	struct memnod	*mh_prev ;	/* Previous memory block	*/
#endif
#if defined(MEM_WHERE)
	char		*mh_file ;	/* File allocation was from	*/
	unsigned int	mh_line ;	/* Line allocation was from	*/
#endif
	} MEMHDR ;

/* Alignment macros */
/* ---------------- */
#define ALIGN_SIZE (2*sizeof(double))
#define HDR_SIZE sizeof(MEMHDR)
#define RESERVE_SIZE (((HDR_SIZE+(ALIGN_SIZE-1))/ALIGN_SIZE) \
			*ALIGN_SIZE)

/* Conversion macros */
/* ----------------- */
#define CLIENT_2_HDR(a) ((MEMHDR *) (((char *) (a)) - RESERVE_SIZE))
#define HDR_2_CLIENT(a) ((void *) (((char *) (a)) + RESERVE_SIZE))

/* Local variables */
/* --------------- */
static unsigned long	mem_size = 0 ;	/* Amount of memory used */
#if defined(MEM_LIST)
static MEMHDR	*memlist = NULL ;	/* List of memory blocks */
#endif

/* Local functions */
/* --------------- */
void	mem_tag_err(void *, char *, int) ;	/* Tag error */
#if defined(MEM_LIST)
void	mem_list_add(MEMHDR *) ;		/* Add block to list */
void	mem_list_delete(MEMHDR *) ;		/* Delete block from list */
#define Mem_Tag_Err(a) mem_tag_err(a,fil,lin)
#else
#define Mem_Tag_Err(a) mem_tag_err(a,__FILE__,__LINE__)
#endif

/************************************************************************/
/**** Functions accessed only through macros ****************************/
/************************************************************************/

/*----------------------------------------------------------------------
 *+
 *  mem_alloc
 *  Allocate a memory block
 *
 *  Usage
 *
 *	void *
 *	mem_alloc(
 *		size_t		size
 *	)
 *
 *  Parameters
 *
 *	size		Size of block in bytes to allocate
 *
 *  Return Value
 *
 *	Pointer to allocated memory block
 *	NULL if not enough memory
 *
 *  Description
 *
 *	mem_alloc() makes a protected call to malloc()
 *
 *  Notes
 *
 *	Access this routine using the malloc() macro in mshell.h
 *
 *-
 */

void *
mem_alloc(
#if defined(MEM_WHERE)
	size_t		size,
	char		*fil,
	int		lin
#else
	size_t		size
#endif
)

{
	MEMHDR		*p ;

	/* Allocate memory block */
	/* --------------------- */

	p = malloc(RESERVE_SIZE + size) ;

	if (p == NULL) {

		return NULL ;
	}

	/* Init header */
	/* ----------- */

	p->mh_tag = MEMTAG ;
	p->mh_size = size ;
	mem_size += size ;
#if defined(MEM_WHERE)
	p->mh_file = fil ;
	p->mh_line = lin ;
#endif

#if defined(MEM_LIST)
	mem_list_add(p) ;
#endif

	/* Return pointer to client data */
	/* ----------------------------- */

	return HDR_2_CLIENT(p) ;
}


/*----------------------------------------------------------------------
 *+
 *  mem_calloc
 *  Allocate a memory block
 *
 *  Usage
 *
 *	void *
 *	mem_calloc(
 *		size_t		nelem
 *		size_t		size
 *	)
 *
 *  Parameters
 *
 *	nelem		Number of elements to be allocated
 *	size		Size of each  element 
 *
 *  Return Value
 *
 *	Pointer to allocated memory block
 *	NULL if not enough memory
 *
 *  Description
 *
 *	mem_calloc() makes a protected call to calloc()
 *
 *  Notes
 *
 *	Access this routine using the calloc() macro in mshell.h
 *
 *-
 */

void *
mem_calloc(
#if defined(MEM_WHERE)
	size_t		nelem,
	size_t		size,
	char		*fil,
	int		lin
#else
	size_t		nelem,
	size_t		size
#endif
)

{
	void		*p ;

	/* Allocate memory block */
	/* --------------------- */

#if defined(MEM_WHERE)
	p = mem_alloc(size*nelem,fil,lin);
#else
	p = mem_alloc(size*nelem) ;
#endif

	if (p == NULL) {

		return NULL ;
	}

	/* Return pointer to client data */
	/* ----------------------------- */

	memset(p, 0, size*nelem);

	return p;
}

/*----------------------------------------------------------------------
 *+
 *  mem_realloc
 *  Reallocate a memory block
 *
 *  Usage
 *
 *	void *
 *	mem_realloc(
 *		void		*ptr,
 *		size_t		 size
 *	)
 *
 *  Parameters
 *
 *	ptr		Pointer to current block
 *	size		Size to adjust block to
 *
 *  Return Value
 *
 *	Pointer to new memory block
 *	NULL if memory cannot be reallocated
 *
 *  Description
 *
 *	mem_realloc() makes a protected call to realloc().
 *
 *  Notes
 *
 *	Access this routine using the realloc() macro in mshell.h
 *
 *-
 */

void *
mem_realloc(
#if defined(MEM_WHERE)
	void		*ptr,
	size_t		size,
	char		*fil,
	int		lin
#else
	void		*ptr,
	size_t		size
#endif
)

{
	MEMHDR		*p ;

	/* Convert client pointer to header pointer */
	/* ---------------------------------------- */

	p = CLIENT_2_HDR(ptr) ;

	/* Check for valid block */
	/* --------------------- */

	if (p->mh_tag != MEMTAG) {

		Mem_Tag_Err(p) ;

		return NULL ;
	}

	/* Invalidate header */
	/* ----------------- */

	p->mh_tag = ~MEMTAG ;
	mem_size -= p->mh_size ;

#if defined(MEM_WHERE)
	mem_list_delete(p) ;	/* Remove block from list */
#endif

	/* Reallocate memory block */
	/* ----------------------- */

	p = (MEMHDR *) realloc(p, RESERVE_SIZE + size) ;

	if (p == NULL) {

		return NULL ;
	}

	/* Update header */
	/* ------------- */

	p->mh_tag = MEMTAG ;
	p->mh_size = size ;
	mem_size += size ;
#if defined(MEM_LIST)
	p->mh_file = fil ;
	p->mh_line = lin ;
#endif

#if defined(MEM_WHERE)
	mem_list_add(p) ;	/* Add block to list */
#endif

	/* Return pointer to client data */
	/* ----------------------------- */

	return HDR_2_CLIENT(p) ;
}

/*----------------------------------------------------------------------
 *+
 *  mem_strdup
 *  Save a string in dynamic memory
 *
 *  Usage
 *
 *	char *
 *	mem_strdup(
 *		char		*str
 *	)
 *
 *  Parameters
 *
 *	str		String to save
 *
 *  Return Value
 *
 *	Pointer to allocated string
 *	NULL if not enough memory
 *
 *  Description
 *
 *	mem_strdup() saves the specified string in dynamic memory.
 *
 *  Notes
 *
 *	Access this routine using the strdup() macro in mshell.h
 *
 *-
 */

char *
mem_strdup(
#if defined(MEM_WHERE)
	char		*str,
	char		*fil,
	int		lin
#else
	char		*str
#endif
)

{
	char * s ;

#if defined(MEM_WHERE)
	s = mem_alloc(strlen(str)+1, fil, lin) ;
#else
	s = mem_alloc(strlen(str)+1) ;
#endif

	if (s != NULL) {

		strcpy(s, str) ;
	}

	return s ;
}

/*----------------------------------------------------------------------
 *+
 *  mem_free
 *  Free a memory block
 *
 *  Usage
 *
 *	void
 *	mem_free(
 *		void		*ptr
 *	)
 *
 *  Parameters
 *
 *	ptr		Pointer to memory to free
 *
 *  Return Value
 *
 *	None
 *
 *  Description
 *
 *	mem_free() frees the specified memory block. The
 *	block must be allocated using mem_alloc(), mem_realloc()
 *	or mem_strdup().
 *
 *  Notes
 *
 *	Access this routine using the free() macro in mshell.h
 *
 *-
 */

void
mem_free(
#if defined(MEM_WHERE)
	void		*ptr,
	char		*fil,
	int		lin
#else
	void		*ptr
#endif
)

{
	MEMHDR		 *p ;

	/* Convert client pointer to header pointer */
	/* ---------------------------------------- */

	p = CLIENT_2_HDR(ptr) ;

	/* Check for valid block */
	/* --------------------- */

	if (p->mh_tag != MEMTAG) {

		Mem_Tag_Err(p) ;

		return ;
	}

	/* Invalidate header */
	/* ----------------- */

	p->mh_tag = ~MEMTAG ;

	mem_size -= p->mh_size ;

#if defined(MEM_LIST)
	mem_list_delete(p) ;	/* Remove block from list */
#endif

	/* Free memory block */
	/* ----------------- */

	free(p) ;
}

/************************************************************************/
/**** Functions accessed directly ***************************************/
/************************************************************************/

/*----------------------------------------------------------------------
 *+
 *  Mem_Used
 *  Return amount of memory currently allocated
 *
 *  Usage
 *
 *	unsigned long
 *	Mem_Used(
 *	)
 *
 *  Parameters
 *
 *	None.
 *
 *  Description
 *
 *	Mem_Used() returns the number of bytes currently allocated
 *	using the memory management system. The value returned is
 *	simply the sum of the size requests to allocation routines.
 *	It does not reflect any overhead required by the memory
 *	management system.
 *
 *  Notes
 *
 *	None
 *
 *-
 */

unsigned long
Mem_Used(
void)

{
	return mem_size ;
}

/*----------------------------------------------------------------------
 *+
 *  Mem_Display
 *  Display memory allocation list
 *
 *  Usage
 *
 *	void
 *	Mem_Display(
 *		FILE		*fp
 *	)
 *
 *  Parameters
 *
 *	fp		File to output data to
 *
 *  Description
 *
 *	Mem_Display() displays the contents of the memory
 *	allocation list.
 *
 *	This function is a no-op if MEM_LIST is not defined.
 *
 *  Notes
 *
 *	None
 *
 *-
 */

void
Mem_Display(
	FILE		*fp
)

{
#if defined(MEM_LIST)
	MEMHDR		*p ;
	int		idx ;

#if defined(MEM_WHERE)
	fprintf(fp, "Index   Size  File(Line) - total size %lu\n", mem_size) ;
#else
	fprintf(fp, "Index   Size - total size %lu\n", mem_size) ;
#endif
	fflush(fp);

	idx = 0 ;

	p = memlist ;

	while (p != NULL) {

		fprintf(fp, "%-5d %6u", idx++, p->mh_size) ;

#if defined(MEM_WHERE)
		fprintf(fp, "  %s(%d)", p->mh_file, p->mh_line) ;
#endif

		if (p->mh_tag != MEMTAG) {

			fprintf(fp, " INVALID") ;
		}

		fprintf(fp, "\n") ;

		p = p->mh_next ;
		fflush(fp);
	}
#else
	fprintf(fp, "Memory list not compiled (MEM_LIST not defined)\n") ;
#endif
}

/************************************************************************/
/**** Memory list manipulation functions ********************************/
/************************************************************************/

/*
 * mem_list_add()
 * Add block to list
 */

#if defined(MEM_LIST)
static void
mem_list_add(
	MEMHDR	*p
)

{
	p->mh_next = memlist ;

	p->mh_prev = NULL ;

	if (memlist != NULL) {

		memlist->mh_prev = p ;
	}

	memlist = p ;

#if defined(DEBUG_LIST)
	printf("mem_list_add()\n") ;
	Mem_Display(stdout) ;
#endif
}
#endif

/*----------------------------------------------------------------------*/

/*
 * mem_list_delete()
 * Delete block from list
 */

#if defined(MEM_LIST)
static void
mem_list_delete(
	MEMHDR	*p
)

{
	if (p->mh_next != NULL)	{

		p->mh_next->mh_prev = p->mh_prev ;
	}

	if (p->mh_prev != NULL)	{

		p->mh_prev->mh_next = p->mh_next ;
	}
	else {

		memlist = p->mh_next ;
	}

#if defined(DEBUG_LIST)
	printf("mem_list_delete()\n") ;
	Mem_Display(stdout) ;
#endif
}
#endif

/************************************************************************/
/**** Error display *****************************************************/
/************************************************************************/

/*
 *  mem_tag_err()
 *  Display memory tag error
 */

static void
mem_tag_err(
	void		*p,
	char		*fil,
	int		lin
)

{
	fprintf(stderr, "Memory tag error - %p - %s(%d)\n", p, fil, lin) ;

#if defined(MEM_LIST)
	Mem_Display(stderr) ;
#endif

	exit(1) ;
}

/*	
/*	Teste de Memoria - Grava o resultado no arquivo memolog
/*
/*	Implementada por Carla Geovana Macario
/*
*/
void	Mem_Store(module)
char	*module;
{
	FILE	*fsai;

	fsai = fopen("memolog", "a");
	fprintf(fsai, "Modulo: %s\n", module);
	fflush(fsai);
	Mem_Display(fsai);
	fclose(fsai);
}
