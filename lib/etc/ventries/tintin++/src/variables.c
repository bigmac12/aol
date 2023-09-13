/* Autoconf patching by David Hedbor, neotron@lysator.liu.se */
/*********************************************************************/
/* file: variables.c - functions related the the variables           */
/*                             TINTIN ++                             */
/*          (T)he K(I)cki(N) (T)ickin D(I)kumud Clie(N)t             */
/*                     coded by peter unold 1992                     */
/*********************************************************************/
#ifdef HAVE_STRING_H
#include <string.h>
#else
#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif
#endif
#include <ctype.h>
#include "tintin.h"

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

extern char *get_arg_in_braces();
extern struct listnode *search_node_with_wild();
extern struct listnode *searchnode_list();

extern struct listnode *common_myvars;
extern char vars[10][BUFFER_SIZE]; /* the %0, %1, %2,....%9 variables */
extern int varnum;
extern int mesvar[5];
/*************************/
/* the #variable command */
/*************************/
void var_command(arg, ses)
     char *arg;
     struct session *ses;
{
  char left[BUFFER_SIZE], right[BUFFER_SIZE], arg2[BUFFER_SIZE];
  struct listnode *tempvars, *ln;
  /* char right2[BUFFER_SIZE]; */
  tempvars=(ses) ? ses->myvars : common_myvars;
  arg=get_arg_in_braces(arg, left,0);
  arg=get_arg_in_braces(arg, right,1);
  if(!*left) {
    tintin_puts2("#THESE VARIABLES HAVE BEEN SET:", ses);
    show_list(tempvars);
    prompt(ses);
  }

  else if(*left && !*right) {
    if ((ln=search_node_with_wild(tempvars,left))!=NULL) {
      while((tempvars=search_node_with_wild(tempvars, left))!=NULL) {
        shownode_list(tempvars);
      }
      prompt(ses);
    }
    else
      if (mesvar[5])
        tintin_puts2("#THAT VARIABLE IS NOT DEFINED.", ses);
  }

  else {
    if((ln=searchnode_list(tempvars, left))!=NULL)
      deletenode_list(tempvars, ln);
    insertnode_list(tempvars, left, right, "0", ALPHA);
    varnum++;
    if (mesvar[5]) {
      sprintf(arg2, "#Ok. $%s is now set to {%s}.",left, right);
      tintin_puts2(arg2, ses);
    }
  }
}
/************************/
/* the #unvar   command */
/************************/
void unvar_command(arg, ses)
     char *arg;
     struct session *ses;
{
  char left[BUFFER_SIZE] ,result[BUFFER_SIZE];
  struct listnode *tempvars, *ln, *temp;
  int flag;
  flag=FALSE;
  tempvars=(ses) ? ses->myvars : common_myvars;
  temp=tempvars;
  arg=get_arg_in_braces(arg,left,1);
  while ((ln=search_node_with_wild(temp, left))!=NULL) {
    if (mesvar[5]) {
      sprintf(result, "#Ok. $%s is no longer a variable.", ln->left);
      tintin_puts2(result, ses);
    }
    deletenode_list(tempvars, ln);
    flag=TRUE;
    temp=ln;
  }
  if (!flag && mesvar[5])
    tintin_puts2("#THAT VARIABLE IS NOT DEFINED.", ses);
}
/*************************************************************************/
/* copy the arg text into the result-space, but substitute the variables */
/* $<string> with the values they stand for                              */
/*************************************************************************/
void substitute_myvars(arg, result, ses)
     char *arg;
     char *result;
     struct session *ses;
{
  /* int varflag=FALSE;
  char *right; */
  char varname[20];
  int nest=0,counter,varlen;
  struct listnode *ln, *tempvars;
  tempvars=(ses) ? ses->myvars : common_myvars;
  while(*arg) {

    if(*arg=='$') { /* substitute variable */
      counter=0;
      while (*(arg+counter)=='$')
	counter++;
        varlen=0;
      while(isalpha(*(arg+varlen+counter))) 
        varlen++;
      if (varlen>0)
        strncpy(varname,arg+counter,varlen);
      *(varname+varlen)='\0';
      if (counter==nest+1 && !isdigit(*(arg+counter+1))) {
        if((ln=searchnode_list(tempvars, varname))!= NULL) {
	  strcpy(result, ln->right);
          result+=strlen(ln->right);
          arg+=counter+varlen;
	}
        else {
          strncpy(result,arg,counter+varlen);
          result+=varlen+counter;
          arg+=varlen+counter;
        }
      }
      else  {  
          strncpy(result,arg,counter+varlen);
          result+=varlen+counter;
          arg+=varlen+counter;
      }
    }
    else if (*arg==DEFAULT_OPEN) {
      nest++;
      *result++= *arg++;
    }
    else if (*arg==DEFAULT_CLOSE) {
      nest--;
      *result++= *arg++;
    }
    else if (*arg=='\\' && *(arg+1)=='$' && nest==0) {
      arg++;
      *result++= *arg++;
    }
       
    else
      *result++= *arg++;
  }
  *result='\0';
}

/*************************/
/* the #tolower command */
/*************************/
void tolower_command(arg, ses)
     char *arg;
     struct session *ses;
{
  char left[BUFFER_SIZE], right[BUFFER_SIZE], arg2[BUFFER_SIZE], *p;
  struct listnode *tempvars, *ln;

  tempvars = (ses) ? ses->myvars : common_myvars;
  arg = get_arg_in_braces(arg, left,0);
  arg = get_arg_in_braces(arg, right,1);
  if (!*left || !*right) {
    tintin_puts2("#Syntax: #tolower <var> <text>", ses);
  } else {
    if ((ln=searchnode_list(tempvars, left)) != NULL)
      deletenode_list(tempvars, ln);
    for (p=right; *p; p++)
      *p = tolower(*p);
    insertnode_list(tempvars, left, right, "0", ALPHA);
    varnum++;
    if (mesvar[5]) {
      sprintf(arg2, "#Ok. $%s is now set to {%s}.",left, right);
      tintin_puts2(arg2, ses);
    }
  }
}

/*************************/
/* the #toupper command */
/*************************/
void toupper_command(arg, ses)
     char *arg;
     struct session *ses;
{
  char left[BUFFER_SIZE], right[BUFFER_SIZE], arg2[BUFFER_SIZE], *p;
  struct listnode *tempvars, *ln;

  tempvars = (ses) ? ses->myvars : common_myvars;
  arg = get_arg_in_braces(arg, left,0);
  arg = get_arg_in_braces(arg, right,1);
  if (!*left || !*right) {
    tintin_puts2("#Syntax: #toupper <var> <text>", ses);
  } else {
    if ((ln=searchnode_list(tempvars, left)) != NULL)
      deletenode_list(tempvars, ln);
    for (p=right; *p; p++)
      *p = toupper(*p);
    insertnode_list(tempvars, left, right, "0", ALPHA);
    varnum++;
    if (mesvar[5]) {
      sprintf(arg2, "#Ok. $%s is now set to {%s}.",left, right);
      tintin_puts2(arg2, ses);
    }
  }
}
