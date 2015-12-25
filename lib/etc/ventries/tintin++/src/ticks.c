/* Autoconf patching by David Hedbor, neotron@lysator.liu.se */
/*********************************************************************/
/* file: ticks.c - functions for the ticker stuff                    */
/*                             TINTIN III                            */
/*          (T)he K(I)cki(N) (T)ickin D(I)kumud Clie(N)t             */
/*                     coded by peter unold 1992                     */
/*********************************************************************/
#include <ctype.h>
#ifdef HAVE_STRING_H
#include <string.h>
#else
#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif
#endif
#include <signal.h>
#include "tintin.h"

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef HAVE_TIME_H
#include <time.h>
#endif

#ifndef BADSIG
#define BADSIG (void (*)())-1
#endif

/* externs */
struct session *sessionlist;

/* local globals */
int sec_to_tick, time0, tick_size=75;
int ticker_interrupted;

/*********************/
/* the #tick command */
/*********************/
void tick_command(ses)
     struct session *ses;
{
  if(ses) {
    if(ses->tickstatus) {
      char buf[100];
      sprintf(buf, "THERE'S NOW %d SECONDS TO NEXT TICK.", sec_to_tick);
      tintin_puts(buf, ses);
    }
    else
      tintin_puts("MY TICKER IS OFF! DUNNO SECONDS TO TICK", ses);
  }
  else
    tintin_puts("#NO SESSION ACTIVE => NO TICKER!", ses);
}

/************************/
/* the #tickoff command */
/************************/
void tickoff_command(ses)
     struct session *ses;
{
  if(ses) {
    ses->tickstatus=FALSE;
    tintin_puts("#TICKER IS NOW OFF.", ses);
  }
  else
   tintin_puts("#NO SESSION ACTIVE => NO TICKER!", ses);
}

/***********************/
/* the #tickon command */
/***********************/
void tickon_command(ses)
     struct session *ses;
{
  if(ses) {
    ses->tickstatus=TRUE;
    tintin_puts("#TICKER IS NOW ON.", ses);
  }
  else
    tintin_puts("#NO SESSION ACTIVE => NO TICKER!", ses);
}

/************************/
/* the #tickset command */
/************************/
void tickset_command(ses)
     struct session *ses;
{
  if(ses) 
    time0=time(NULL); /* we don't prompt! too many ticksets... */
  else
    tintin_puts("#NO SESSION ACTIVE => NO TICKER!", ses);
}

/*************************/
/* the #ticksize command */
/*************************/
void ticksize_command(arg, ses)
     char *arg;
     struct session *ses;
{
  if(*arg!='\0') {
    if(isdigit(*arg)) {
      tick_size=atoi(arg);
      time0=time(NULL);
      tintin_puts("#OK NEW TICKSIZE SET", ses);        
    }
    else
      tintin_puts("#SPECIFY A NUMBER!!!!TRYING TO CRASH ME EH?", ses);
  }
  else
    tintin_puts("#SET THE TICK-SIZE TO WHAT?", ses);
}     

/****************************************/
/* alarm signal handler used for ticker */
/****************************************/
static void tick_func()
{
  ticker_interrupted=TRUE;
  alarm(1);
  if(signal(SIGALRM, tick_func)==BADSIG)
    syserr("signal SIGALRM"); 

  sec_to_tick=tick_size-((time(NULL)-time0)%tick_size);
  if(sec_to_tick==tick_size || sec_to_tick==10) {
    struct session *sesptr;
    for(sesptr=sessionlist; sesptr; sesptr=sesptr->next)
      if(sesptr->tickstatus) 
        if(sec_to_tick==tick_size)
          tintin_puts("#TICK!!!", sesptr);
        else
          tintin_puts("#10 SECONDS TO TICK!!!", sesptr);
  }
}













