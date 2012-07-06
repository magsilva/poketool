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

     
#include "editcons.h"

extern int line1, line2, curln;

extern char pat[31];

stcode subst(char * sub, int gflag,int glob)
/*
char *sub;
int gflag;
int glob;
*/
/* Substitui "sub" por ocorrencias de pattern */

{
  char new[MAXSTR], old[MAXSTR];
  int j,k,lastm,line,m,done,subbed;
  stcode stat;

  if (glob)
    stat = OK;
  else
    stat = ERR;
  done = (line1 <= 0);
  line = line1;
  while ((!done) && (line <= line2))
  {
    j = 0;
    subbed = 0;
    gettxt(line,old);
    lastm = -1;
    k = 0;
    while (old[k] != ENDSTR)
    {
     if ((gflag) || (!subbed))
       m = amatch(old,k,pat,0);
     else
       m = -1; 
     if ((m >= 0) && (lastm != m))
     {
       subbed = 1;
       catsub(old,k,m,sub,new,&j,MAXSTR);
       lastm = m;
     }
     if ((m == -1) || (m == k))
     {
       addstr(old[k],new,&j,MAXSTR);
       k++;
     }
     else
       k = m;
    }
    if (subbed)
    { 
      if (!addstr(ENDSTR,new,&j,MAXSTR))
      {
        stat = ERR;
        done = 1;
      }
      else
      {
        stat = lndelete(line,line,&stat);
        stat = puttxt(new);
        line2 = line2 + curln - line;
        line = curln;
        if (stat == ERR)
          done = 1;
        else
          stat = OK;
      }
    }
   line = line + 1;
  }
  return(stat);
}


void gettxt(n,s)
int n;
char *s;
                                                          
{
  strcpy(s,buf[n].txt);
}


void reverse(n1,n2)
int n1;
int n2;

/* Inverte posicoes de buf[n1] ate' buf[n2] */


{
  struct buftype temp;
  while (n1<n2)
  {
    strcpy(temp.txt,buf[n1].txt);
    temp.mark = buf[n1].mark;
    strcpy(buf[n1].txt,buf[n2].txt);
    buf[n1].mark = buf[n2].mark;
    strcpy(buf[n2].txt,temp.txt);
    buf[n2].mark = temp.mark;
    n1++;
    n2--;
  }
}


void blkmove(n1,n2,n3)
int n1;
int n2;
int n3;

/* Move blocos de linhas n1..n2 para depois de n3, n3 nao deve
   estar entre n1 e n2 */

{
  if (n3 < n1 - 1)
  {
    reverse(n3+1,n1-1);
    reverse(n1,n2);
    reverse(n3+1,n2);
  }
  else if (n3 > n2)
       {
	 reverse(n1,n2);
	 reverse(n2+1,n3);
	 reverse(n1,n3);
       }
}

void putmark(n,m)
int n;
int m;

/* Imprime mark m na n-esima linha */

{
  buf[n].mark = m;
}


stcode puttxt(lin)
char *lin;

/* Adiciona nova linha apos curln se ela couber. */

{
  if (lastln < MAXLINES)
  {
    lastln++;
    strcpy(buf[lastln].txt,lin);
    putmark(lastln,0);
    /* Adiciona no fim e move para depois de curln */
    blkmove(lastln,lastln,curln);
    curln++;
    return OK;
   }
   else
     return ERR;
 }



int addstr(c, outset, j, maxset)
int c, *j, maxset;
char *outset;

 /* Coloca c em outset[j] se for possivel, e incrementa j */

{
   if (*j >= maxset)
     return 0;
   else
   {
     outset[*j] = c;
     *j = *j + 1;
     outset[*j] = ENDSTR;
     return 1;
   }
 }


int prevln(n)
int n;

/* Obtem a linha anterior a n */

{
  if (n <= 0)
    return(lastln);
  else
    return(n - 1);
}


stcode lndelete(n1,n2,status)
int n1;
int n2;
stcode *status;

/* Deleta linhas de n1 ate' n2 */

{
  if (n1 <= 0)
    *status = ERR;
  else
  {
    /* move linhas para final do buffer, apos lastln */
    blkmove(n1,n2,lastln);
    lastln = lastln - n2 + n1 -1;  /* descontar nro de linhas removido */
    curln = prevln(n1);  /* linha corrente fica sendo a anterior */
    *status = OK;
  }
  return(*status);
}


int patsize(pat,n)
char *pat;
int n;

/* Retorna o tamanho de pattern em pat[n] */

{
  if (pat[n] == LITCHAR)
    return 2;
  else if ((pat[n] == BOL) || (pat[n] == EOL) || (pat[n] == ANY))
         return 1;
       else if ((pat[n] == CCL) || (pat[n] == NCCL))
              return (pat[n+1] + 2);
              /* pat [n+1] contem nro de repeticoes */
            else
              if (pat[n] == CLOSURE)
                return (CLOSIZE);
              else
              {
                printf ("in patsize: can''t happen");
                return -1;
              }
}




int getline(s,fd,maxsize)
char *s;
FILE *fd;
int maxsize;

/*  Obtem em s uma linha do arquivo fd, no final acrescenta
    um caracter nulo */

{
  int i,c;
  i = 0;
  do
  {
    s[i] = c = fgetc(fd);
    i++;
  }while ((c!=ENDFILE) && (c!=NEWLINE) && (i<maxsize));
   
  if ((c == ENDFILE))
    i--;
  s[i] = ENDSTR;
  return (c != ENDFILE);
}



int locate(c,pat,offset)
int c, offset;
char *pat;

/* Procura por c num conjunto de caracteres em pat[offset] */

{
  int i,j=0;
  /* conjunto a partir de pat[offset] */
  i = offset + pat[offset] - 48;   /* ultima posicao  do conjunto */
  while (i>offset)
    if (c==pat[i])
    {
      j = 1;
      i = offset;      /* sair do loop */
    }
    else
      i--;
  return(j);
}





int omatch(lin,i,pat,j)
char *lin, *pat;
int *i, j;

{
  int advance = -1;
  if (lin[*i] == ENDSTR)
    advance = -1;
  else switch (pat[j])
  {
    case LITCHAR : if (lin[*i] == pat[j+1]) advance = 1;
                   break;

    case BOL : if (*i==0) advance = 0;
               break;

    case ANY : if (lin[*i] != NEWLINE) advance = 1;
               break;

    case EOL : if (lin[*i] == NEWLINE) advance = 0;
               break;

    case CCL : if (locate(lin[*i],pat,j+1)) advance = 1;
               break;

    case NCCL : if ((lin[*i] != NEWLINE) &&
                    (!locate(lin[*i],pat,j+1))) advance =1;
                break;

    default : if (pat[j] != CLOSURE)
      { printf("in omatch: can't happen");
        return 0;
      }
  }
  if (advance >= 0)
  {
    *i = *i + advance;
    return 1;
  }
  else
    return 0;
}


  
int amatch(lin,offset,pat,j)
char *lin, *pat;
int offset, j;


{
  int i,k,done;
  done = 0;
  while ((!done) && (pat[j] != ENDSTR))
  {
    if (pat[j] == CLOSURE)
    {
      j = j + patsize(pat,j);
      /* j aponta para elemento a ser repetido */
      i = offset;
      while ((!done) && (lin[i] != ENDSTR))
        if (!omatch(lin,&i,pat,j))
          done = 1;
      /* i aponta para o caracter que provocou o erro, fez done true */
      done = 0;
      while ((!done) && (i >= offset))
      {
        /* tentar outros possiveis casamentos */
        k = amatch(lin,i,pat,j+patsize(pat,j));
        if (k >= 0)
          done = 1;      /* casou  com o resto de pattern */
        else 
          i--;
      }
      offset = k;
      done = 1;
    }
    else if (!omatch(lin,&offset,pat,j))
         {
            offset = -1;   /* nao estrela */
           done = 1;
         }
         else
           j = j + patsize(pat,j);
  }
  return(offset);
}

void catsub(lin, s1,s2,sub,new,k,maxnew)
char *lin;
int s1; int s2;
char *sub; char *new;
int *k; int maxnew;

/* Adiciona texto de substituicao para final de new */

{
  int i,j;
  i = 0;
  while (sub[i] != ENDSTR)
  {
    if (sub[i] == DITTO)
      for (j=s1; j<=s2-1; j++)
        addstr(lin[j],new,k,maxnew);
    else
        addstr(sub[i],new,k,maxnew);
    i++;
  }
}



void main(argc, argv)
int argc;
char *argv[];

{
   int i,ind;
   FILE * fd;
   char null[10];
   char nome[10];

   null[0] = '\0'; 
   if (argc != 12)
    {
      printf("Erro no numero de argumentos");
       exit(1);
     }
   
   strcpy(nome,argv[8]); 
   strcat(nome,".dat"); 
   /* printf("%s\n", nome); */
   if (fd = fopen(nome,"r"))
   {   
     for (ind=0; getline(buf[ind].txt,fd,MAXSTR); ind++);
     fclose(fd);
     /* ind = ind+1; */ 
     strcpy(buf[ind].txt,argv[10]);
     strcpy(nome,argv[9]); 
     strcat(nome,".dat");   
 
     if (fd = fopen(nome,"r"))
     {
       for (ind = ind+1; getline(buf[ind].txt,fd,MAXSTR); ind++);
    
       fclose(fd);
       strcpy(buf[ind].txt,null);
       strcpy(buf[ind+1].txt,argv[11]);

       line1 = atoi(argv[3]); 
       line2 = atoi(argv[4]);
       strcpy(pat,argv[6]);
       lastln = atoi(argv[7]);
       curln = lastln;

       /* printf("%d %d %s %d ", line1, line2, pat , lastln); */
  
       printf("%d\n", subst(argv[5],atoi(argv[1]),atoi(argv[2]))); 
  
       for (i=0; i<=ind+3; i++)
         puts(buf[i].txt);
      }
   }
   else
     printf("Nao consegui abrir o arquivo arq.dat");
 }
 
