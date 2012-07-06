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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"
#include "cte.h"
#include "newpokelib.h"
#include "hexper.h"

void error(msg)
char msg[200];
{

 /* Declaracao de Variaveis Locais */

 FILE * avalog;

 /* escreve mensagem de erro */

 fprintf(stderr,"%s\n",msg);

 exit(1);

}

int get_scenario(scen)
     char *scen;
{
int ret;

  if(!strcmp(scen,"-tpc"))
    ret=TPC; 
  else
    if(!strcmp(scen,"-tpr"))
      ret=TPR; 
    else
      if(!strcmp(scen,"-tpf"))
        ret=TPF;  
      else
        if(!strcmp(scen,"-tgc"))
          ret=TGC;  
        else
          if(!strcmp(scen,"-tgr"))
            ret=TGR;  
          else
            if(!strcmp(scen,"-tgf"))
              ret=TGF;  
            else
              if(!strcmp(scen,"-mtc"))
                ret=MTC;  
              else
                if(!strcmp(scen,"-mtr"))
                  ret=MTR;  
                else
		  if(!strcmp(scen,"-mtf"))
		    ret=MTF;  
                  else
		    if(!strcmp(scen,"-tgdc"))
		      ret=TGDC; 
                    else
		      if(!strcmp(scen,"-tgdr"))
			ret=TGDR;  
                      else
			ret=0;  

 return ret;
}

int get_crit_number(crit)
char *crit;
{
int ret;

  if(!strcmp(crit,"pudu"))
    /* == pudu */
    ret=3; 
  else
    /* == pu */
    if(!strcmp(crit,"pu"))
      ret=2; 
    else
      if(!strcmp(crit,"pdu"))
        /* == nos */
        ret=1;  
      else
        if(!strcmp(crit,"arcs"))
          /* == arcs */
          ret=4;  
        else
          if(!strcmp(crit,"nos"))
            /* == nos */
            ret=5;  
          else
            if(!strcmp(crit,"uses"))
              /* == uses */
              ret=7;  
            else
              if(!strcmp(crit,"puses"))
                /* == puses */
                ret=6;  
              else
                if(!strcmp(crit,"du"))
                  /* == puses */
                  ret=8;  
                else
		  if(!strcmp(crit,"cuses"))
		    /* == cuses */
		    ret=9;  
                  else
		    if(!strcmp(crit,"rand"))
		      /* == rand */
		      ret=10;  
                   else
		      ret=0;


 return ret;
}

char * get_des_file(filename,crit)
char *filename;
char *crit;
{
  if(!strcmp(crit,"pudu"))
    /* == pudu */
    strcpy(filename,"des_pudu.tes");
  else
    /* == pu */
    if(!strcmp(crit,"pu"))
      strcpy(filename,"des_pu.tes");
    else
      if(!strcmp(crit,"pdu"))
        /* == nos */
        strcpy(filename,"des_pdu.tes");
      else
        if(!strcmp(crit,"arcs"))
          /* == arcs */
          strcpy(filename,"des_arc.tes");
        else
          if(!strcmp(crit,"nos"))
            /* == nos */
            strcpy(filename,"des_nos.tes");
          else
            if(!strcmp(crit,"uses"))
              /* == uses */
              strcpy(filename,"des_all_uses.tes");
            else
              if(!strcmp(crit,"puses"))
                /* == puses */
                strcpy(filename,"des_p_uses.tes");
              else
                if(!strcmp(crit,"du"))
                  /* == puses */
                  strcpy(filename,"des_all_du_paths.tes");
                else
                  error_msg(invalid_criterion);

}

char * get_freq_file(filename,crit,testn)
char *filename;
char *crit;
int testn;
{
  char str[100];
  
  if(!strcmp(crit,"pudu"))
    /* == pudu */
    strcpy(filename,"pudufreq");
  else
    /* == pu */
    if(!strcmp(crit,"pu"))
      strcpy(filename,"pufreq");
    else
      if(!strcmp(crit,"pdu"))
        /* == nos */
        strcpy(filename,"pdufreq");
      else
        if(!strcmp(crit,"arcs"))
          /* == arcs */
          strcpy(filename,"arcfreq");
        else
          if(!strcmp(crit,"nos"))
            /* == nos */
            strcpy(filename,"nosfreq");
          else
            if(!strcmp(crit,"uses"))
              /* == uses */
              strcpy(filename,"usesfreq");
            else
              if(!strcmp(crit,"puses"))
                /* == puses */
                strcpy(filename,"pusesfreq");
              else
                if(!strcmp(crit,"du"))
                  /* == puses */
                  strcpy(filename,"dupathsfreq");
                else
                  error_msg(invalid_criterion);

 sprintf(str,"%d.tes",testn);

 strcat(filename,str);
 
}

char * get_cases_file(filename,crit)
char *filename;
char *crit;
{
  char str[100];
  
  if(!strcmp(crit,"pudu"))
    /* == pudu */
    strcpy(filename,"puducases");
  else
    /* == pu */
    if(!strcmp(crit,"pu"))
      strcpy(filename,"pucases");
    else
      if(!strcmp(crit,"pdu"))
        /* == nos */
        strcpy(filename,"pducases");
      else
        if(!strcmp(crit,"arcs"))
          /* == arcs */
          strcpy(filename,"arccases");
        else
          if(!strcmp(crit,"nos"))
            /* == nos */
            strcpy(filename,"noscases");
          else
            if(!strcmp(crit,"uses"))
              /* == uses */
              strcpy(filename,"usescases");
            else
              if(!strcmp(crit,"puses"))
                /* == puses */
                strcpy(filename,"pusescases");
              else
                if(!strcmp(crit,"du"))
                  /* == puses */
                  strcpy(filename,"dupathscases");
                else
                  error_msg(invalid_criterion);

 strcat(filename,".tes");
 
}

/*******************************************************************/
/* char * le_linha_gen(char **, char *, FILE *)                    */
/* Autor: Marcos L. Chaim                                          */
/* Data: 10/12/99                                                  */
/* Versao: 1.0                                                     */
/*                                                                 */
/* Funcao: ler a primeira linha do arquivo fonte que contem o "st- */
/*         ring" passado como parametro.                           */
/*                                                                 */
/* Entradas: "string" que deve estar contido na linha do arquivo,  */
/*            ponteiro para arquivo                                */
/*                                                                 */
/* Saida: endereco da posicao que inicia o "string".               */
/*                                                                 */
/*******************************************************************/

char * le_linha_gen(file)
FILE * file;
{

 /* Declaracao de Variaveis Locais */
 int i;
 char * ptr_linha;
 char linha_aux[MAXLINE-1];
 char * p_linha_aux=NULL,* p_linha_aux2=NULL;
 
 i = 1;

 
 do
      {     
      if(fgets(linha_aux,MAXLINE-1,file) == (char *) NULL)
         {
           p_linha_aux=NULL;
           break;
         }
      
      if((strlen(linha_aux) == MAXLINE-2 && linha_aux[MAXLINE-2] != '\n')
         || i > 1)
        {         
        /* linha muito grande */

        if(strlen(linha_aux) + 1 == MAXLINE-1)
          {
          ++i;
        
          p_linha_aux2 = (char *) malloc(sizeof(char)* (i * (MAXLINE-1)));

          if(p_linha_aux2 == (char *) NULL)
             error_msg(mem_fault);
        
          if(i == 2)
            {
            /* primeiro incremento da linha */
            p_linha_aux = p_linha_aux2;  
            strcpy(p_linha_aux, linha_aux);         
            }
          else
            {
            /* nos demais incrementos da linha, concatena */
            strcpy(p_linha_aux2, p_linha_aux);
            free( p_linha_aux);
            p_linha_aux = p_linha_aux2;
            strcat(p_linha_aux, linha_aux);
            }
          }
        else
          { /* Tamanho da linha cabe em p_linha_aux */
          strcat(p_linha_aux, linha_aux);
          }
        }
      else
        {
        p_linha_aux=strdup(linha_aux);
        if(p_linha_aux == (char *) NULL)
          error_msg(mem_fault);
        }
        
      }
  while(p_linha_aux[strlen(p_linha_aux)-1] != '\n');
    
  return(p_linha_aux);
}
/*******************************************************************/
/* char * le_linha_str(char *, char *, FILE *)                     */
/* Autor: Marcos L. Chaim                                          */
/* Data: 17/01/90                                                  */
/* Versao: 1.0                                                     */
/*                                                                 */
/* Funcao: ler a primeira linha do arquivo fonte que contem o "st- */
/*         ring" passado como parametro.                           */
/*                                                                 */
/* Entradas: vetor onde sera' armazenada o linha do arquivo, "str- */
/*           ing" que deve estar contido nesta linha.              */
/*                                                                 */
/* Saida: endereco da posicao que inicia o "string".               */
/*                                                                 */
/*******************************************************************/

char * le_linha_str(linha,string, file)
char * linha;
char * string;
FILE * file;
{

 /* Declaracao de Variaveis Locais */

 char * ptr_linha;

 do
   {
    if(fgets(linha,MAXLINE-1,file) == (char *) NULL)
       error_msg(wrong_file_desc);
    ptr_linha = strstr(linha,string);
   }
 while(ptr_linha == (char *) NULL);
 return(ptr_linha);
}

/*******************************************************************/
/* char * le_linha_str_exper(char *, char *, FILE *)               */
/* Autor: Marcos L. Chaim                                          */
/* Data: 14/02/99                                                  */
/* Versao: 1.0                                                     */
/*                                                                 */
/* Funcao: ler a primeira linha do arquivo fonte que contem o "st- */
/*         ring" passado como parametro.                           */
/*                                                                 */
/* Entradas: vetor onde sera' armazenada o linha do arquivo, "str- */
/*           ing" que deve estar contido nesta linha.              */
/*                                                                 */
/* Saida: endereco da posicao que inicia o "string".               */
/*                                                                 */
/*******************************************************************/

char * le_linha_str_exper(linha,string, file)
char * linha;
char * string;
FILE * file;
{

 /* Declaracao de Variaveis Locais */

 char * ptr_linha;

 do
   {
    if(fgets(linha,MAXLINE-1,file) == (char *) NULL)
      {
      ptr_linha = NULL;
      break;
      }
    ptr_linha = strstr(linha,string);
   }
 while(ptr_linha == (char *) NULL);
 return(ptr_linha);
}

char * le_no_feature(linha, feature)
char * linha;
int * feature;
{
 /* Declaracao de Variaveis Locais */

 char * ptr_linha, * newlinha;

 *feature = -1;

 ptr_linha = linha;

 if(*ptr_linha == '\0' || *ptr_linha == '\n')
   return NULL;
 
 while(*ptr_linha != '\n' && *ptr_linha != ' ' && *ptr_linha != '\0')
   ptr_linha++;

 switch(*ptr_linha)
   {
   case '\0': newlinha = ptr_linha; break;
   case ' ' : 
   case '\n': newlinha = ptr_linha+1; break;
   }

 *ptr_linha = '\0';

 *feature=atoi(linha);
 
 return(newlinha);
}



/*
** Fim exper2.c
*/
















