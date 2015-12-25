/* Autoconf patching by David Hedbor, neotron@lysator.liu.se */
/*********************************************************************/
/* file: net.c - do all the net stuff                                */
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
#include <sys/errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "tintin.h"

/* NOTE!  Some systems might require a #include <net/errno.h>,
 * try adding this if you are really stuck and net.c won't compile.
 * Thanks to Brian Ebersole [Harm@GrimneMUD] for this suggestion.
 */

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#include <arpa/inet.h>
#endif


void do_telnet_protecol();

extern int sessionsstarted, ticker_interrupted;
extern struct listnode *common_aliases, *common_actions, *common_subs;
extern struct session *sessionlist, *activesession;
extern int errno;
/**************************************************/
/* try connect to the mud specified by the args   */
/* return fd on success / 0 on failure            */
/**************************************************/
int connect_mud(host, port, ses)
     char *host;
     char *port;
     struct session *ses;
{
  int sock, connectresult;
  struct sockaddr_in sockaddr;

  if(isdigit(*host))		/* interprete host part */
    sockaddr.sin_addr.s_addr=inet_addr(host);
  else {
    struct hostent *hp;
    if((hp=gethostbyname(host))==NULL) {
      tintin_puts("#ERROR - UNKNOWN HOST.", ses);
      prompt(NULL); 
      return 0;
    }
    memcpy((char *)&sockaddr.sin_addr, hp->h_addr, sizeof(sockaddr.sin_addr));
  }

  if(isdigit(*port))  
    sockaddr.sin_port=htons(atoi(port)); /* inteprete port part */
  else {
    tintin_puts("#THE PORT SHOULD BE A NUMBER.", ses);
    prompt(NULL);
    return 0;
  }

  if((sock=socket(AF_INET, SOCK_STREAM, 0))<0) 
    syserr("socket");

  sockaddr.sin_family=AF_INET;


  tintin_puts("#Trying to connect..", ses);

  alarm(15);			/* We'll allow connect to hang in 15seconds! NO MORE! */
  ticker_interrupted=FALSE;
  connectresult=connect(sock, (struct sockaddr *)&sockaddr, sizeof(sockaddr));
#if defined(DEBUG)
  alarm(0);
#else
  alarm(1); 
#endif

  if(connectresult || ticker_interrupted) {
    close(sock);
    if(ticker_interrupted)
      tintin_puts("#CONNECTION TIMED OUT.", ses);
    else
      switch(errno) {
       case ECONNREFUSED:
	tintin_puts("#ERROR - CONNECTION REFUSED.", ses);
	break;
       case ENETUNREACH:
	tintin_puts("#ERROR - THE NETWORK IS NOT REACHABLE FROM THIS HOST.", ses);
	break;
       default:
	tintin_puts("#Couldn't connect", ses);
      } 
    prompt(NULL);
    return 0;
  }
  return sock;
}

/************************************************************/
/* write line to the mud ses is connected to - add \n first */
/************************************************************/
void write_line_mud(line, ses)
     char *line;
     struct session *ses;
{
  char outtext[BUFFER_SIZE+2];

  strcpy(outtext, line);
  strcat(outtext, "\n\r");
    
  if(write(ses->socket, outtext, strlen(outtext))== -1)
    syserr("write in write_to_mud");
}


/*******************************************************************/
/* read at most BUFFER_SIZE chars from mud - parse protecol stuff  */
/*******************************************************************/
int read_buffer_mud(buffer, ses)
     char *buffer;
     struct session *ses;
{
  int i, didget;
  char tmpbuf[BUFFER_SIZE], *cpsource, *cpdest;
  didget=read(ses->socket, tmpbuf, 512);
  ses->old_more_coming=ses->more_coming;
  if (didget==512) 
    ses->more_coming=1;
  else 
    ses->more_coming=0;
  if(didget<0) 
    return 0; /*syserr("read from socket");  we do this here instead - dunno quite 
                why, but i got some mysterious connection read by peer on some hps */
  
  else if(didget==0) 
    return 0;

  else {
    cpsource=tmpbuf;
    cpdest=buffer;
    i=didget;
    while(i>0) {
      if(*(unsigned char *)cpsource==255) {
        do_telnet_protecol(*cpsource, *(cpsource+1), *(cpsource+2), ses);
        i-=3;
        cpsource+=3;
      }
      else {
        *cpdest++= *cpsource++;
        i--;
      }
    }
  }
  *cpdest='\0';
  return didget;
}


/*****************************************************************/
/* respond according to the telnet protecol - weeeeelllllll..... */
/*****************************************************************/
void do_telnet_protecol(dat0, dat1, dat2, ses)
     int dat0;
     int dat1;
     int dat2;
     struct session *ses;
{
/* we don't do anything here.. why should we? add the stuff yourself if
   you feel like being nice..... */
}
