/* Autoconf patching by David Hedbor, neotron@lysator.liu.se */
/*********************************************************************/
/* file: main.c - main module - signal setup/shutdown etc            */
/*                             TINTIN++                              */
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
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#include <signal.h>
#include "tintin.h"
#include <fcntl.h>

#ifndef BADSIG
#define BADSIG (void (*)())-1
#endif

/*************** globals ******************/
int term_echoing=TRUE;
int echo=DEFAULT_ECHO;
int speedwalk=DEFAULT_SPEEDWALK;
int togglesubs=DEFAULT_TOGGLESUBS;
int presub=DEFAULT_PRESUB;
int redraw=DEFAULT_REDRAW;
int sessionsstarted;
int puts_echoing=TRUE;
int verbose=FALSE;
int alnum=0;
int acnum=0;
int subnum=0;
int varnum=0;
int hinum=0;
int pdnum=0;
int antisubnum=0;
int verbatim=0;
char homepath[1025];
char E=27;

struct session *sessionlist, *activesession;
struct listnode *common_aliases, *common_actions, *common_subs, *common_myvars;
struct listnode *common_highs, *common_antisubs, *common_pathdirs;
char vars[10][BUFFER_SIZE]; /* the %0, %1, %2,....%9 variables */
char tintin_char=DEFAULT_TINTIN_CHAR;
char verbatim_char=DEFAULT_VERBATIM_CHAR;    
char system_com[80]=SYSTEM_COMMAND_DEFAULT;
int mesvar[7];
int display_row, display_col, input_row, input_col;
int split_line, term_columns;
char k_input[BUFFER_SIZE];
char done_input[BUFFER_SIZE], prev_command[BUFFER_SIZE];
int hist_num;
int is_split;
int text_came;
void tintin();
void read_mud();
void do_one_line();
void snoop();
void tintin_puts2();
int interpret_buffer();

/************ externs *************/
extern int ticker_interrupted, time0;
extern int tick_size, sec_to_tick;

extern void check_all_actions(); /* new */
static void myquitsig();
static void tick_func();
extern struct session *newactive_session();
extern struct session *parse_input();
extern struct session *read_command();
extern struct completenode *complete_head;
extern struct listnode *init_list();
extern void term_noecho();
extern void read_complete();
extern void syserr();
extern void alarm();
extern int do_one_antisub();
extern void do_one_sub();
extern void do_one_high();
extern void prompt();
#ifdef HAVE_TIME_H
#include <time.h>
#endif
int read();
int select();
extern void do_history();
extern int read_buffer_mud();
extern void cleanup_session();
int write();
/* int fwrite(); */
/* int sscanf(); */
extern void term_echo();

int last_line_length;
#if defined(HAVE_TERMIO_H) && !defined(BSD_ECHO)
#include <termio.h>
#ifdef HAVE_TCFLAG_T
tcflag_t c_lflag;
cc_t c_cc[NCCS];
#else
unsigned char c_cc[NCC];
unsigned short c_lflag;
#endif
#endif

/* CHANGED to get rid of double-echoing bug when tintin++ gets suspended */
void tstphandler(sig, code, scp, addr)
   int sig;
   int code;
   struct sigcontext *scp;
   char *addr;
{
  ticker_interrupted=TRUE;
#if defined(HAVE_TERMIO_H) && !defined(BSD_ECHO)
  alarm(0);
#endif
  term_echo();
  kill(getpid(), SIGSTOP);
#if defined(HAVE_TERMIO_H) && !defined(BSD_ECHO)
  alarm(1);
#endif
  term_noecho();
}

/**************************************************************************/
/* main() - show title - setup signals - init lists - readcoms - tintin() */
/**************************************************************************/

int main(argc, argv, environ)
     int argc;
     char **argv;
     char **environ;
{
  struct session *ses;
  char *strptr, temp[BUFFER_SIZE];
  int arg_num;
  int fd;

#if defined(HAVE_TERMIO_H) && !defined(BSD_ECHO)
  init_echo();
#endif
  term_noecho();
  is_split=FALSE;
  text_came=FALSE;
  read_complete();
  *k_input='\0';
  hist_num= -1;
  ses=NULL;

  tintin_puts2("##################################################", ses);
  sprintf(temp,"#              T I N T I N + + %12s      #", VERSION_NUM);
  tintin_puts2(temp, ses); 
  tintin_puts2("#  (T)he k(I)cki(N) (T)ickin d(I)kumud clie(N)t  #", ses);
  tintin_puts2("#                a DIKU-mud client               #", ses);
  tintin_puts2("#     new code by Bill Reiss, David A. Wagner    #", ses);
  tintin_puts2("#      Joann Ellsworth, Jeremy C. Jack 1994      #", ses);
  tintin_puts2("# thanks to Peter Unold for original TINTIN code #", ses);
  tintin_puts2("##################################################", ses);  

  if(signal(SIGTERM, myquitsig)==BADSIG)
    syserr("signal SIGTERM");
  if(signal(SIGINT, myquitsig)==BADSIG)
    syserr("signal SIGINT");
  if(signal(SIGALRM, tick_func)==BADSIG)
    syserr("signal SIGALRM");
  /* CHANGED to get rid of double-echoing bug when tintin++ gets suspended */
  if(signal(SIGTSTP, tstphandler)==BADSIG)
    syserr("signal SIGTSTP");
  time0=time(NULL);
#if defined(DEBUG)
  alarm(0);
#else
  alarm(1);
#endif

  common_aliases=init_list();
  common_actions=init_list();
  common_subs=init_list();
  common_myvars=init_list();
  common_highs=init_list();
  common_antisubs=init_list();
  common_pathdirs=init_list();
  mesvar[0]=DEFAULT_ALIAS_MESS;
  mesvar[1]=DEFAULT_ACTION_MESS;
  mesvar[2]=DEFAULT_SUB_MESS;
  mesvar[3]=DEFAULT_ANTISUB_MESS;
  mesvar[4]=DEFAULT_HIGHLIGHT_MESS;
  mesvar[5]=DEFAULT_VARIABLE_MESS;
  mesvar[6]=DEFAULT_PATHDIR_MESS;
  *homepath='\0';
  if (!strcmp(DEFAULT_FILE_DIR, "HOME"))
    if (strptr = (char *)getenv("HOME"))
      strcpy(homepath, strptr);
    else *homepath = '\0';
  else strcpy(homepath, DEFAULT_FILE_DIR);
  arg_num=1;
  if(argc > 1 && argv[1]) {
    if (*argv[1]=='-' && *(argv[1]+1)=='v') {
      arg_num=2;
      verbose=TRUE;
    }
  }
  if(argc > arg_num && argv[arg_num]) {
    activesession=read_command(argv[arg_num], NULL);
  }
  else {
    strcpy(temp,homepath);
    strcat(temp,"/.tintinrc");
    if((fd=open(temp, O_RDONLY)) > 0) { /* Check if it exists */
      close(fd);
      activesession=read_command(temp, NULL);
    }
    else {
      if(strptr = (char *)getenv("HOME")) {
        strcpy(homepath, strptr);
        strcpy(temp, homepath);
        strcat(temp,"/.tintinrc");
        if((fd=open(temp, O_RDONLY)) > 0) { /* Check if it exists */
          close(fd);
          activesession=read_command(temp, NULL);
        }
      }
    }
  }
  tintin();

  return 0;
}

/***************************/
/* the main loop of tintin */
/***************************/
void tintin()
{
  char buffer[BUFFER_SIZE], strng[80];
  int didget, done, readfdmask;
  struct session *sesptr;
  struct session *j;
  *k_input='\0';
  while(TRUE) {
    readfdmask=1;
    for(sesptr=sessionlist; sesptr; sesptr=sesptr->next)
      readfdmask|=sesptr->socketbit;
    ticker_interrupted=FALSE;
    if(select(32, (fd_set*) &readfdmask,  0, 0, 0)<0 && !ticker_interrupted)
      syserr("select");
    if(ticker_interrupted)
      ticker_interrupted=FALSE;
    else {
      if(readfdmask&1) {
        if((didget=read(0, buffer, sizeof(buffer)))<0)
          syserr("read from fd 0");
        *(buffer+didget)='\0';
        done=interpret_buffer(buffer, activesession); 
	if (done) {
          hist_num= -1;
          if (term_echoing) {
            if (activesession && *done_input) 
              if (strcmp(done_input, prev_command))
	      
		do_history(done_input,activesession);
            if (is_split)
              tintin_puts2(done_input, activesession);
            else
              tintin_puts2("", activesession);
          }
          if (*done_input)
	    strcpy(prev_command, done_input);
	  if (is_split) {
            sprintf(strng, "[%d;1f[0J", split_line+1);
            write(1, strng, strlen(strng));
          }
          activesession=parse_input(done_input, activesession);
        } 
      }
     
      for(sesptr=sessionlist; sesptr; )
      {
        j = sesptr->next;
        if(sesptr->socketbit&readfdmask) {
          read_mud(sesptr);
	}
        sesptr = j;
      }
    } 
  }

}

/***************************************************************************/
/* new for 1.2...since we're in raw mode, we have control over the keys    */
/* that people press as they are pressed.  This routine parses that input  */
/* and responds accordingly                                                */
/***************************************************************************/

int interpret_buffer(buffer, ses)
     char *buffer;
     struct session *ses;
{
  char *tptr, *tptr2, *cptr, *cptr2, strng[BUFFER_SIZE];
  struct completenode *tcomp;
  int flag, counter;
  tptr=buffer;
  tptr2=buffer;  
  if (!*k_input)
    text_came=FALSE;
  while (*tptr) {
    /* return was pressed, get ready to send line */
    if (*tptr=='\n' || *tptr=='\r') {
      text_came=FALSE;
      input_row=split_line+1;
      input_col=1;
      strcpy(done_input, k_input);
      *k_input='\0';
      return 1;
    }
    /* this is backspace (and delete is mapped onto it too) */
    else if (*tptr==127 || *tptr==8) {
      *tptr='\0';
      if (strlen(k_input)!=0) {
        if (!is_split && text_came) {
          sprintf(strng, "\n\r%s", k_input);
          write(1,strng,strlen(strng));
        }
        text_came=FALSE;
        if (input_col>2 || !is_split || input_row == split_line+1) {
          input_col--;
          sprintf(strng, "\b \b");        
        }
        else if (is_split) {
          input_row--;
          input_col=SCREEN_WIDTH;
          sprintf(strng, "\b \b%c[%d;%df", E, input_row, input_col);
        }  
        if(term_echoing) write(1, strng, strlen(strng));
        *(k_input+strlen(k_input)-1)='\0';
      }
      tptr2=tptr+1;
    } 
    else if (*tptr==18) {
      if (!is_split) {
        sprintf(strng, "\n\r");
	write(1, strng, strlen(strng));
	write(1, k_input, strlen(k_input));
	text_came=FALSE;
      }      
    }
    /* ^D */
    else if (*tptr==4) {
      strcpy(k_input, prev_command);
      if (is_split) {
        sprintf(strng, "%c[%d;1f%c[0J", E, split_line+1, E);
        write(1, strng, strlen(strng));
        input_row=split_line+1;
        input_col=strlen(prev_command);
        cptr2=k_input;
        while (input_col+strlen(cptr2) >= SCREEN_WIDTH) {
          counter=SCREEN_WIDTH-input_col;
          write(1, cptr2, counter);
          input_row++;
          input_col=1;
          sprintf(strng, "%c[%d;%df", E, input_row, input_col);
          write(1, strng, strlen(strng));
          cptr2+=counter;
        }
        input_col=strlen(cptr2)+1;
        write(1, cptr2, strlen(cptr2));
      }
      else {
        sprintf(strng, "\n%s", k_input);
        write(1, strng, strlen(strng));
      }
    } 


    /* ^U */
    else if (*tptr==21) {
      if (is_split) {
        sprintf(strng, "%c[%d;1f%c[0J", E, split_line+1, E);
        write(1, strng, strlen(strng));
        input_row=split_line+1;
        input_col=1; 
      }
      else {
        sprintf(strng, "\n");
        write(1, strng, strlen(strng)); 
      }
      *k_input=0;
    }
  
    /* this is history scroll, I still have some work to do on this one */
    else if (*tptr==16) {        
      flag=TRUE;
      *tptr='\0';
      if (ses) {
        if (hist_num<HISTORY_SIZE-1) {
          if (ses->history[hist_num+1]) {
            hist_num++;
            strcpy(k_input, ses->history[hist_num]);
            if (is_split) {
              input_row=split_line+1;
              input_col=1;
              sprintf(strng, "%c[%d;1f%c[0J", E, split_line+1, E);
              write(1, strng, strlen(strng));
              cptr2=ses->history[hist_num];
              while (input_col+strlen(cptr2) >= SCREEN_WIDTH) {
                counter=SCREEN_WIDTH-input_col;
                write(1, cptr2, counter);
                input_row++;
                input_col=1;
                sprintf(strng, "%c[%d;%df", E, input_row, input_col);
                write(1, strng, strlen(strng));
                cptr2+=counter;
              }
              input_col=strlen(cptr2)+1;
              write(1, cptr2, strlen(cptr2));
            }
            else {
              sprintf(strng, "\n\r");
              write(1, strng, strlen(strng));
              write(1, ses->history[hist_num], strlen(ses->history[hist_num]));
            }
            flag=FALSE;
          }
        }
      }
      if (!flag)
        return 0;
      tptr2=tptr+1;
    } 
    /* also history scroll */
    else if (*tptr==14) {
      flag=TRUE;
      *tptr='\0';
      if (ses) {
        if (hist_num>0) {
          if (ses->history[hist_num-1]) {
            hist_num--;
            strcpy(k_input, ses->history[hist_num]);
            if (is_split) {
              input_row=split_line+1;
              input_col=1;
              sprintf(strng, "%c[%d;1f%c[0J", E, split_line+1, E);
              write(1, strng, strlen(strng));
              cptr2=ses->history[hist_num];
              while (input_col+strlen(cptr2) >= SCREEN_WIDTH) {
                counter=SCREEN_WIDTH-input_col;
                write(1, cptr2, counter);
                input_row++;
                input_col=1;
                sprintf(strng, "%c[%d;%df", E, input_row, input_col);
                write(1, strng, strlen(strng));
                cptr2+=counter;
              }
              input_col=strlen(cptr2)+1;
              write(1, cptr2, strlen(cptr2));
            }
            else {
              sprintf(strng, "\n\r");
              write(1, strng, strlen(strng));
              write(1, ses->history[hist_num], strlen(ses->history[hist_num]));
            }
            flag=FALSE;
          }
        }  
        else if (hist_num==0) {
          hist_num--;
          strcpy(k_input, "");
          if (is_split) {
            sprintf(strng, "%c[%d;1f%c[0J", E, split_line+1, E);
            write(1,strng,strlen(strng));
          }
          else tintin_puts2("", ses);
          flag=FALSE;
        } 
      } 
      if (!flag)
        return 0;  
      tptr2=tptr+1;
    }
    /* tab key pressed, this is tab completion */
    else if (*tptr==9) {
      *tptr='\0';
      cptr=k_input+strlen(k_input)-1;
      counter=0;
      while (*cptr!=' ' && cptr!=k_input) {
        cptr--;
        counter++;
      }
      
      if (*cptr==' ')
        cptr++;
      tcomp=complete_head;
      flag=TRUE;
      while ((tcomp=tcomp->next) && flag) {
        if (!strncmp(tcomp->strng, cptr, strlen(cptr))) {
          cptr2=tcomp->strng+strlen(cptr);
          strcat(k_input, cptr2);
          if (input_col+strlen(cptr2) >= SCREEN_WIDTH) {
	    while (input_col+strlen(cptr2) >= SCREEN_WIDTH) {
              counter=SCREEN_WIDTH-input_col;
              write(1, cptr2, counter);
              input_row++;
              input_col=1;
              sprintf(strng, "%c[%d;%df", E, input_row, input_col);
              write(1, strng, strlen(strng));
              cptr2+=counter;
            }
            input_col=strlen(cptr2)+1;
            write(1, cptr2, strlen(cptr2));
          }
          else {
	    write(1, cptr2, strlen(cptr2));
	    input_col+=strlen(cptr2);
          }
	  flag=FALSE;
	}
      }
      tptr2=tptr+1;
    }
    else {
      if (*tptr==27)
        *tptr='^';
      if (input_col>=SCREEN_WIDTH && is_split && term_echoing) {
        input_row++;
        input_col=1;
        sprintf(strng, "%c[%d;1f", E, input_row);
        write(1, strng, strlen(strng));
      }
      if (term_echoing) {
        write(1, tptr, 1);
        display_col++;
      }
      input_col++;
      *(k_input+strlen(k_input)+1)='\0';
      *(k_input+strlen(k_input))= *tptr;      
    }
    tptr++;
  }
  return 0;
}
/*************************************************************/
/* read text from mud and test for actions/snoop/substitutes */
/*************************************************************/
void read_mud(ses)
     struct session *ses;
{
  char buffer[BUFFER_SIZE], linebuffer[BUFFER_SIZE], *cpsource, *cpdest;
  char temp[BUFFER_SIZE]; 
  char *result2, result[BUFFER_SIZE];
  int didget,n, length, count;


  if(!(didget=read_buffer_mud(buffer, ses))) {
    cleanup_session(ses);
    if(ses==activesession)
      activesession=newactive_session();
    free(ses);
  }

  else {
    if(ses->logfile) {
      if (!OLD_LOG) {
      count=0;
      for(n=0;n<=didget;n++) 
         if (buffer[n]!='\r') { 
            temp[count]=buffer[n]; 
            count++;
         }
      fwrite(temp, count, 1, ses->logfile);
      } /* then */ 
      else 
        fwrite(buffer, didget, 1, ses->logfile);
    }
    cpsource=buffer; 
    cpdest=linebuffer;
    if (ses->old_more_coming==1) {
      strcpy(linebuffer,ses->last_line);
      cpdest+=strlen(linebuffer);
    } 
    result2=result;
    if (is_split) {
      sprintf(result, "%c8", E); 
      result2+=strlen(result);
    }
    length=0;
    while(*cpsource) {  /*cut out each of the lines and process*/
      length++;
      if(*cpsource=='\n' || *cpsource=='\r') {
        length=0;
        *cpdest='\0';      
        do_one_line(linebuffer, ses);

        if(!(*linebuffer=='.' && !*(linebuffer+1))) {
          n=strlen(linebuffer);
          memcpy(result2, linebuffer, n);
          result2+=n;
          *result2++= *cpsource++;
          if(*cpsource=='\n' || *cpsource=='\r') 
          *result2++= *cpsource++;
        }
        else 
          if(*++cpsource=='\n' || *cpsource=='\r') 
            cpsource++;

        cpdest=linebuffer;
      }
      else
        *cpdest++= *cpsource++;
    }
    *cpdest='\0';
    if (ses->more_coming==1) {
      strcpy(ses->last_line,linebuffer);
      length=0;
    }
    else { 
      do_one_line(linebuffer,ses);
      n=strlen(linebuffer);
      memcpy(result2, linebuffer, n);
      result2+=n;
    }
    *result2='\0';
    if (ses==activesession) {
      if (strlen(result))
        text_came=TRUE;
      if (is_split) {
        sprintf(temp, "%c7%c[%d;%df", E, E, input_row, input_col);
        strcat(result, temp);
      }
      write(1, result,strlen(result));        
      display_col=length+1;
    }
    else if (ses->snoopstatus) {
      if (strlen(result))
        text_came=TRUE;
      snoop(result,ses);
    }
    if (!is_split && redraw && text_came==TRUE && term_echoing) {
      write(1, k_input, strlen(k_input));
      text_came=FALSE;
      display_col=strlen(k_input);
    }
  }
}
/**********************************************************/
/* do all of the functions to one line of buffer          */
/**********************************************************/
void do_one_line(line, ses)
     char *line;
     struct session *ses;
{   
    if (!presub && !ses->ignore)
      check_all_actions(line,ses);
    if (!togglesubs) 
      if(!do_one_antisub(line,ses))
        do_one_sub(line,ses);
    if (presub && !ses->ignore)
      check_all_actions(line,ses);
    do_one_high(line, ses);
}  
/**********************************************************/
/* snoop session ses - chop up lines and put'em in buffer */
/**********************************************************/
void snoop(buffer, ses)
     char *buffer;
     struct session *ses; 
{
  /* int n; */
  char *cpsource, *cpdest, line[BUFFER_SIZE], linebuffer[BUFFER_SIZE], 
       header[BUFFER_SIZE];
  *linebuffer='\0';
  if (is_split)
    sprintf(linebuffer, "%c8", E); 
  if (display_col!=1) {
    sprintf(line, "\n\r");
    strcat(linebuffer, line);
  }
  cpsource=buffer; 
  sprintf(header,"%s%% ", ses->name);
  strcpy(line,header);
  cpdest=line+strlen(line);
  while(*cpsource) {     
    if(*cpsource=='\n' || *cpsource=='\r') {
      *cpdest++= *cpsource++;      
      if(*cpsource=='\n' || *cpsource=='\r') 
        *cpdest++= *cpsource++;
      *cpdest='\0';
      strcat(linebuffer,line);
      cpdest=line+strlen(header); 
    }
    else
      *cpdest++= *cpsource++;
  }
  if (cpdest != line+strlen(header)) {
    *cpdest++='\r';
    *cpdest++='\n';
    *cpdest='\0';
    strcat(linebuffer,line);
  }
  if (is_split) {
    sprintf(line,"%c7%c[%d;%df", E, E, input_row, input_col);
    strcat(linebuffer, line);
  }
  write(1,linebuffer,strlen(linebuffer));
  display_col=1;
}

/*****************************************************/
/* output to screen should go throught this function */
/* text gets checked for actions                     */
/*****************************************************/
void tintin_puts(cptr, ses)
     char *cptr;
     struct session *ses;
{
  tintin_puts2(cptr,ses);  
  if(ses)
   check_all_actions(cptr, ses); 
  
}
/*****************************************************/
/* output to screen should go throught this function */
/* not checked for actions                           */
/*****************************************************/
void tintin_puts2(cptr, ses)
     char *cptr;
     struct session *ses;
{
  char strng[1024];
  if((ses==activesession || ses==NULL) && puts_echoing) {
    if (is_split) 
      sprintf(strng,"%c8%s\n\r%c7%c[%d;%df", E, cptr, E, E, input_row, input_col);
    else
      sprintf(strng,"%s\n\r", cptr);
    write(1,strng, strlen(strng)+1);
    display_col=1;
    if (redraw && term_echoing && !is_split)
      write(1, k_input, strlen(k_input));
    text_came=TRUE;
  } 
  
}
/*****************************************************/
/* output to screen should go throught this function */
/* not checked for actions                           */
/*****************************************************/
void tintin_puts3(cptr, ses)
     char *cptr;
     struct session *ses;
{
  char strng[1024];
  if((ses==activesession || ses==NULL) && puts_echoing) {
    if (is_split) {
      if (*cptr=='^') {
        cptr++; 
        sprintf(strng,"%c8%s\n\r%c7", E, cptr, E);
      }
      else {
        cptr++;
        sprintf(strng,"%s\n\r%c7%c[%d;%df", cptr, E, E, input_row, input_col);
      }
    }
    else {
      cptr++;
      sprintf(strng,"%s\n\r", cptr);
    }
    write(1,strng, strlen(strng)+1);
    display_col=1;
  } 
  text_came=TRUE;  
}

/****************************************/
/* alarm signal handler used for ticker */
/****************************************/
static void tick_func()
{ 
  ticker_interrupted=TRUE;
#if defined(DEBUG)
  alarm(0);
#else
  alarm(1);
#endif
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

/**********************************************************/
/* Here's where we go when we wanna quit TINTIN FAAAAAAST */
/**********************************************************/
static void myquitsig()
{
  struct session *sesptr;
  struct session *j;
  char strng[BUFFER_SIZE];
  for(sesptr=sessionlist; sesptr; sesptr->next)
  {
    j = sesptr->next;
    cleanup_session(sesptr);
    sesptr = j;
  }
  sesptr=NULL;   
  sprintf(strng, "%cc%c[2J", E, E);

  if (is_split)
    write(1, strng, strlen(strng));
  printf("\n\rYour fireball hits TINTIN with full force, causing an immediate death.\n\r");
  printf("TINTIN is dead! R.I.P.\n\r");
  printf("Your blood freezes as you hear TINTIN's death cry.\n\r");
  term_echo();
  exit(0);
}

void split_command(arg)
     char *arg;
{
  int temp;
  char fn[BUFFER_SIZE];
  sscanf(arg, "%d", &temp);
  if (temp>0) 
    split_line=temp;
  else 
    split_line=21;
  display_row=split_line-1;
  input_row=split_line+1;
  input_col=1;  
  sprintf(fn, "%c[2J%c[1;%dr%c[%d;1f", E, E, display_row, E, split_line);
  write(1,fn, strlen(fn));
  write(1,"-------------------------------------------------------------------------------", 80);
  sprintf(fn, "%c[%d;1f%c7", E, split_line-1, E);
  write(1,fn,strlen(fn));
  sprintf(fn, "%c[%d;1f", E, input_row);
  write(1,fn, strlen(fn));
  is_split=TRUE;
  tintin_puts2("#Ok, split screen activated.", (struct session *)NULL);
  prompt(NULL);
}

void unsplit_command()
{
  char temp[15];
  sprintf(temp, "%cc%c[2J", E, E);
  write(1, temp, strlen(temp));
  is_split=FALSE;
  prompt(NULL);
}

