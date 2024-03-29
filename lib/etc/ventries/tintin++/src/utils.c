/* Autoconf patching by David Hedbor, neotron@lysator.liu.se */
/*********************************************************************/
/* file: utils.c - some utility-functions                            */
/*                             TINTIN III                            */
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
#include "tintin.h"

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

void syserr();

/*********************************************/
/* return: TRUE if s1 is an abrevation of s2 */
/*********************************************/
int is_abrev(s1, s2)
     char *s1;
     char *s2;
{
  return(!strncmp(s2, s1, strlen(s1)));
}

/********************************/
/* strdup - duplicates a string */
/* return: address of duplicate */
/********************************/
char *mystrdup(s)
     char *s;
{
  char *dup;

  if((dup=(char *)malloc(strlen(s)+1))==NULL)
    syserr("Not enought memory for strdup.");
  strcpy(dup, s);
  return dup;
}


/*************************************************/
/* print system call error message and terminate */
/*************************************************/
void syserr(msg)
     char *msg;
{
  extern int errno, sys_nerr;
//  extern char *sys_errlist[];

  fprintf(stderr,"ERROR: %s (%d",msg, errno);
  if(errno>0 && errno<sys_nerr)
    fprintf(stderr,": %s)\n",sys_errlist[errno]);
  else
    fprintf(stderr,")\n");
  exit(1);
}

