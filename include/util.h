/*
// util.h 	%I%	%Y%	%E%	%U%
*/

/*------------------------------------------------------------------------*/
/* util.h                                                                 */
/*------------------------------------------------------------------------*/

#ifndef UTIL_H
#define UTIL_H

#ifdef  MSDOS            /* Microsoft C Compiler */
#define ANSI_C           /* Accepts ANSI Prototyping */
#define REVERSE_BYTES    /* Intel -> bytes reversed */
#endif

#ifdef  __TURBOC__       /* Borland's Turbo C Compiler */
#define ANSI_C           /* Accepts ANSI Prototyping */
#define REVERSE_BYTES    /* Intel -> bytes reversed */
#endif 

#ifndef __TURBOC__
#define SEEK_SET  0      /* define const for fseek */
#endif


/*------------------------------------------------------------------------*/
#ifdef  ANSI_C
#define PROTO(prototype) prototype
#else
#define PROTO(prototype) ()
#endif

#ifdef  ANSI_C
#define Void void
#else
#define Void char
#define const
#undef  NULL
#define NULL 0
#endif
/*------------------------------------------------------------------------*/

#undef  TRUE
#undef  FALSE
#undef  NIL
#undef  PI
#undef  SWAP
#undef  ABS
#undef  MIN
#undef  MAX
#undef  SQR
#undef  SET_POINT
#undef  SET_VECTOR
#undef  SET_RGB

/* ---------- useful macros ----------- */

#define NIL           0
#define PI            3.14159265358979323846
#define SWAP(TYPE,A,B)     { TYPE p_tmp_swap; \
                             p_tmp_swap=(A);  (A)=(B);  (B)=p_tmp_swap; }
#define ABS(A)             ( (A)>=0  ? (A) : -(A) )
#define MIN(A,B)           ( (A)<(B) ? (A) : (B) )
#define MAX(A,B)           ( (A)>(B) ? (A) : (B) )
#define SQR(X)		   ( (X)*(X) )
#define SET_POINT(P,X,Y,Z) ((P).x=(X), (P).y=(Y), (P).z=(Z))
#define SET_VECTOR(V,X,Y,Z)((V).x=(X), (V).y=(Y), (V).z=(Z))
#define SET_RGB(C,R,G,B)   ((C).r=(R), (C).g=(G), (C).b=(B))
#define SSIGN(X) 	   ( (X) < 0. ? -1 : 1 )
#define CSIGN(X) 	   ( (X) > 0. ? 1 : ( (X) < 0. ? -1 : 0 ) )

/* --------- basic data types --------- */
typedef enum
        { FALSE, TRUE }  Boolean;    /* Boolean:       FALSE=0, TRUE=1 */
typedef char            *String;     /* String:       character string */
typedef int              Index;      /* Index:             array index */
typedef unsigned char    byte;       /* byte:         at least  8 bits */
typedef unsigned short   word;       /* word:         at least 16 bits */
typedef unsigned long    longword;   /* longword:     at least 32 bits */
typedef          float   real4;      /* real:     32 bits, IEEE format */
typedef          double  real8;      /* real:     64 bits, IEEE format */

/* ------ basic structured types ------- */

typedef struct  { double x, y, z; } Point;
typedef struct  { double x, y, z; } Vector;
typedef struct  { double r, g, b; } Color;


/* ---------------- other utilities ----------- */

extern int debug_level;
#define dprintf  if (!debug_level) ; else printf
#define dgetchar if (!debug_level) ; else getchar
#define Debug    (debug_level = 1)


/*------------------------------------------------------------------------*/
#define UTIL_H
#endif

