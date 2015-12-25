/* Autoconf patching by David Hedbor, neotron@lysator.liu.se */
/*********************************************************************/
/* file: echo.c - funtions to turn keyboard echo on/off              */
/*                             TINTIN III                            */
/*          (T)he K(I)cki(N) (T)ickin D(I)kumud Clie(N)t             */
/*                     coded by peter unold 1992                     */
/*********************************************************************/
/* This might not work on some exotic unix versions. Nothin I can do */
/* about it, coz most of this was lifted from other programs...      */
/*********************************************************************/
#include "tintin.h"

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#include <sys/ioctl.h>
#endif

#if defined(HAVE_TERMIO_H) && !defined(BSD_ECHO)
#include <termio.h>
#else
#include <sgtty.h>
#if DIRTY_REDEFINE
#undef TIOCGETP
#undef TIOCSETP
#define TIOCGETP 0x40067408
#define TIOCSETP 0x80067409
#endif
#endif

#if defined(HAVE_TERMIO_H) && !defined(BSD_ECHO)
#ifdef HAVE_TCFLAG_T
extern tcflag_t c_lflag;
extern cc_t c_cc[NCCS];
#else
extern unsigned short c_lflag;
extern unsigned char c_cc[NCC];
#endif

void init_echo()
{
  struct  termio io;

  if(ioctl(0, TCGETA, &io)<0)
    syserr("ioctl");
  c_lflag = io.c_lflag;
  c_cc[VMIN] = io.c_cc[VMIN];
  c_cc[VTIME] = io.c_cc[VTIME];
}

/********************************/
/* turn echo on - HP-UX version */
/********************************/
void term_echo()
{
  struct  termio io;

  if(ioctl(0, TCGETA, &io)<0)
    syserr("ioctl");
  io.c_lflag = c_lflag;
  io.c_cc[VMIN] = c_cc[VMIN];
  io.c_cc[VTIME] = c_cc[VTIME];
  if(ioctl(0, TCSETA, &io)<0)
    syserr("ioctl");
}

/*********************************/
/* turn echo off - HP-UX version */
/*********************************/
void term_noecho()
{
  struct  termio io;

  if(ioctl(0, TCGETA, &io)<0)
    syserr("ioctl");
  io.c_lflag &= ~ECHO; 
  io.c_lflag &= ~ICANON;
  io.c_cc[VMIN]=1;
  io.c_cc[VTIME]=0;
  if(ioctl(0, TCSETA, &io) < 0)
    syserr("ioctl");
}

#else /*  BSD-ISH machine  */
/********************************/
/* turn echo on - SunOS version */
/********************************/
void term_echo()
{
  struct  sgttyb sgbuf;
  if(ioctl(0, TIOCGETP, &sgbuf)<0)
    syserr("ioctl");
  sgbuf.sg_flags &= ~CBREAK;
  sgbuf.sg_flags |= ECHO;
  if(ioctl(0, TIOCSETP, &sgbuf)<0)
    syserr("ioctl");
}

/**********************************/
/* turn echo off  - SunOS version */
/**********************************/
void term_noecho()
{
  struct  sgttyb sgbuf;
  struct tchars tco;
  if(ioctl(0, TIOCGETP, &sgbuf)<0)
    syserr("ioctl");
  sgbuf.sg_flags &= ~ECHO;
  sgbuf.sg_flags |= CBREAK;
  if(ioctl(0, TIOCSETP, &sgbuf)<0)
    syserr("opctl");
}
#endif

