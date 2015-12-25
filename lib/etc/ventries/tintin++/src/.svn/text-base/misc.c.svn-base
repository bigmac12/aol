/* Autoconf patching by David Hedbor, neotron@lysator.liu.se */
/*********************************************************************/
/* file: misc.c - misc commands                                      */
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
#include <ctype.h>
#include "tintin.h"

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif

/* externs */
extern struct session *newactive_session();
extern struct listnode *common_aliases, *common_actions, *common_subs, *common_myvars;
extern struct listnode *common_highs, *common_antisubs, *common_pathdirs;
extern char *get_arg_in_braces();
extern void term_echo();
extern int redraw, is_split;
extern struct session *sessionlist;
extern struct completenode *complete_head;
extern char tintin_char;
extern int echo;
extern int speedwalk;
extern int presub;
extern int togglesubs;
extern char vars[10][BUFFER_SIZE]; /* the %0, %1, %2,....%9 variables */
extern int mesvar[7];
extern int verbatim;
/****************************/
/* the cr command           */
/****************************/
void cr_command(ses)
   struct session *ses;
{
  if (ses != NULL)
    write_line_mud("\n", ses);
}
/****************************/
/* the version command      */
/****************************/
void version_command()
{ 
  char temp[80];
  sprintf(temp,"#You are using TINTIN++ %s\n\r", VERSION_NUM);
  tintin_puts2(temp, NULL);
  prompt(NULL);
} 
/****************************/
/* the verbatim command,    */
/* used as a toggle         */
/****************************/
void verbatim_command()
{
  verbatim=!verbatim;
  if (verbatim) 
    tintin_puts2("#All text is now sent 'as is'.", (struct sesssion *)NULL);
  else 
    tintin_puts2("#Text is no longer sent 'as is'.", (struct session *)NULL); 
  prompt(NULL);
}

/********************/
/* the #all command */
/********************/
struct session *all_command(arg, ses)
     char *arg;
     struct session *ses;
{
  struct session *sesptr;
  if(sessionlist) {
    get_arg_in_braces(arg, arg, 1);
    for(sesptr=sessionlist; sesptr; sesptr=sesptr->next)
      parse_input(arg, sesptr);
  }
  else
    tintin_puts("BUT THERE ISN'T ANY SESSION AT ALL!", ses);
  return ses;
}

void redraw_command()
{
  redraw=!redraw;
  if (redraw) 
    tintin_puts2("#Ok. I now redraw input line when text arrives.", (struct session *)NULL);
  else
    tintin_puts2("#Ok. I no longer redraw the input line.", (struct session *)NULL);
  prompt(NULL);
}
/*********************/
/* the #bell command */
/*********************/
void bell_command(ses)
     struct session *ses;

{
  char temp[2];
  temp[0]=7;
  temp[1]=0;
  write(1, temp, 2);
}


/*********************/
/* the #boss command */
/*********************/
void boss_command(ses)
     struct session *ses;
{
  char temp[80];
  int i;
  for(i=0; i<50; i++) {
    sprintf(temp, "in-order traverse of tree starting from node %d resulted in %d red nodes\n", i, 50-i);
    tintin_puts2(temp, (struct session *)NULL);
  }
  getchar(); /* stop screen from scrolling stuff */
}

/*********************/
/* the #char command */
/*********************/
void char_command(arg, ses)
     char *arg;
     struct session *ses;
{
  char strng[80];
  get_arg_in_braces(arg, arg,1);
  if(ispunct(*arg)) {
    tintin_char= *arg;
    sprintf(strng, "#OK. TINTIN-CHAR is now {%c}\n", tintin_char);
    tintin_puts2(strng, (struct session *)NULL);
  }
  else
    tintin_puts2("#SPECIFY A PROPER TINTIN-CHAR! SOMETHING LIKE # OR /!", (struct session *)NULL);
}


/*********************/
/* the #echo command */
/*********************/
void echo_command(ses)
     struct session *ses;
{
  echo=!echo;
  if(echo)
    tintin_puts("#ECHO IS NOW ON.", ses);
  else
    tintin_puts("#ECHO IS NOW OFF.", ses);
}

/*********************/
/* the #end command */
/*********************/
void end_command(command, ses)
     char *command;
     struct session *ses;
{
  if(strcmp(command, "end"))
    tintin_puts("#YOU HAVE TO WRITE #end - NO LESS, TO END!", ses);
  else {
    struct session *sesptr;
    struct session *j;
    for(sesptr=sessionlist; sesptr; sesptr->next)
    {
      j=sesptr->next;
      cleanup_session(sesptr);
      free(sesptr);
      sesptr=j;
    }
    ses=NULL;  
    if (is_split) 
      write(1, "c", 3);
    tintin_puts2("TINTIN suffers from bloodlack, and the lack of a beating heart...", ses);
    tintin_puts2("TINTIN is dead! R.I.P.", ses);
    tintin_puts2("Your blood freezes as you hear TINTIN's death cry.", ses);
    term_echo();
    exit(0);
  }
}

/***********************/
/* the #ignore command */
/***********************/
void ignore_command(ses)
     struct session *ses;
{
  if (ses) {
    if(ses->ignore=!ses->ignore)
      tintin_puts("#ACTIONS ARE IGNORED FROM NOW ON.", ses);
    else
      tintin_puts("#ACTIONS ARE NO LONGER IGNORED.", ses);
  } else tintin_puts("#No session active => Nothing to ignore!", ses);     
}

/**********************/
/* the #presub command*/
/**********************/
void presub_command(ses)
     struct session *ses;
{
  presub=!presub;
  if(presub)
    tintin_puts("#ACTIONS ARE NOW PROCESSED ON SUBSTITUTED BUFFER.",ses);
  else
    tintin_puts("#ACTIONS ARE NO LONGER DONE ON SUBSTITUTED BUFFER.",ses);
}

/**************************/
/* the #togglesubs command*/
/**************************/
void togglesubs_command(ses)
     struct session *ses;
{
  togglesubs=!togglesubs;
  if(togglesubs)
    tintin_puts("#SUBSTITUTES ARE NOW IGNORED.",ses);
  else 
    tintin_puts("#SUBSTITUTES ARE NO LONGER IGNORED.", ses);
}

/***********************/
/* the #showme command */
/***********************/
void showme_command(arg, ses)
     char *arg;
     struct session *ses;
{
  char result[BUFFER_SIZE], strng[BUFFER_SIZE];
  get_arg_in_braces(arg,arg,1);
  prepare_actionalias(arg,result,ses);
  sprintf(strng,"%s",result);
  tintin_puts(strng, ses);
}
/***********************/
/* the #loop command   */
/***********************/
void loop_command(arg, ses)
     char *arg;
     struct session *ses;
{
  char left[BUFFER_SIZE],right[BUFFER_SIZE];
  char result[BUFFER_SIZE];
  int flag, bound1, bound2, counter;
  arg=get_arg_in_braces(arg,left,0);
  arg=get_arg_in_braces(arg,right,1);
  flag =1;
  if (sscanf(left,"%d,%d",&bound1,&bound2)!=2) 
    tintin_puts2("#Wrong number of arguments in #loop",ses);
  else {
    flag=1;
    counter=bound1;
    while(flag==1) {
      sprintf(vars[0], "%d", counter);
      substitute_vars(right,result);
      parse_input(result,ses);
      if (bound1<bound2) {
        counter++;
        if (counter>bound2)
          flag=0;
      }
      else {
        counter--;
        if (counter<bound2)
          flag=0;
      }
    } 
  }  
}
/************************/
/* the #message command */
/************************/
void message_command(arg, ses)
     char *arg;
     struct session *ses;
{
  char offon[2][20];
  int mestype;
  char ms[6][20], tpstr[80];
  sscanf("aliases actions substitutes antisubstitutes highlights variables",
    "%s %s %s %s %s %s",ms[0],ms[1],ms[2],ms[3],ms[4],ms[5]);
  strcpy(offon[0],"OFF.");
  strcpy(offon[1],"ON.");
  get_arg_in_braces(arg,arg,1);
  mestype=0;
  while (!is_abrev(arg,ms[mestype]) && mestype<6)
    mestype++;
  if (mestype==6)
    tintin_puts2("#Invalid message type to toggle.", ses);
  else {
    mesvar[mestype]=!mesvar[mestype];
    sprintf(tpstr,"#Ok. messages concerning %s are now %s",
      ms[mestype],offon[mesvar[mestype]]);
    tintin_puts2(tpstr,ses);
  }
}

/**********************/
/* the #snoop command */
/**********************/
void snoop_command(arg, ses)
     char *arg;
     struct session *ses;
{
  char buf[100];
  struct session *sesptr=ses;

  if(ses) {
    get_arg_in_braces(arg, arg,1);
    if(*arg) {
      for(sesptr=sessionlist; sesptr && strcmp(sesptr->name, arg); sesptr=sesptr->next);
      if(!sesptr) {
        tintin_puts("#NO SESSION WITH THAT NAME!", ses);
        return;
      }
    }
    if(sesptr->snoopstatus) {
      sesptr->snoopstatus=FALSE;
      sprintf(buf, "#UNSNOOPING SESSION '%s'", sesptr->name);
      tintin_puts(buf, ses);
    }
    else {
      sesptr->snoopstatus=TRUE;
      sprintf(buf, "#SNOOPING SESSION '%s'", sesptr->name);
      tintin_puts(buf, ses);
    }
  }
  else
    tintin_puts("#NO SESSION ACTIVE => NO SNOOPING", ses);
}

/**************************/
/* the #speedwalk command */
/**************************/
void speedwalk_command(ses)
     struct session *ses;
{
  speedwalk=!speedwalk;
  if(speedwalk)
    tintin_puts("#SPEEDWALK IS NOW ON.", ses);
  else
    tintin_puts("#SPEEDWALK IS NOW OFF.", ses);
}

/***********************/
/* the #system command */
/***********************/
void system_command(arg, ses)
     char *arg;
     struct session *ses;
{
  get_arg_in_braces(arg, arg, 1);
  if(*arg) {
    tintin_puts3("^#OK EXECUTING SHELL COMMAND.", ses);
    term_echo();
#if defined(DEBUG)
    system(arg);
#else
    alarm(0);
    system(arg);
#endif
#if defined(DEBUG)
    alarm(0);
#else
    alarm(1);
#endif
    term_noecho();
    tintin_puts3("!#OK COMMAND EXECUTED.", ses);
  }
  else
    tintin_puts2("#EXECUTE WHAT COMMAND?", ses);
  prompt(NULL);

}


/********************/
/* the #zap command */
/********************/
struct session *zap_command(ses)
     struct session *ses;
{
  tintin_puts("#ZZZZZZZAAAAAAAAPPPP!!!!!!!!! LET'S GET OUTTA HERE!!!!!!!!", ses);
  if(ses) {  
    cleanup_session(ses);
    free(ses);
    return newactive_session();
  }
  else {
    end_command("end", (struct session *) NULL);
  }
}




/************************/
/* the #wizlist command */
/************************/
/* void wizlist_command(ses)
     struct session *ses;
{
  tintin_puts2("==========================================================================", ses);
  tintin_puts2("                           Implementor:", ses);
  tintin_puts2("                              Valgar ", ses);
  tintin_puts2("", ses);
  tintin_puts2("          Special thanks to Grimmy for all her help :)", ses);
  tintin_puts2("\n\r                         TINTIN++ testers:", ses);
  tintin_puts2(" Nemesis, Urquan, Elvworn, Kare, Merjon, Grumm, Tolebas, Winterblade ", ses); 
  tintin_puts2("\n\r A very special hello to Zoe, Oura, GODDESS, Reyna, Randela, Kell, ", ses);
  tintin_puts2("                  and everyone else at GrimneDIKU\n\r", ses);
  tintin_puts2("==========================================================================", ses);
  prompt(ses);
} */

void wizlist_command(ses)
  struct session *ses;
{

  tintin_puts2("==========================================================================", ses);
  tintin_puts2("  There are too many people to thank for making tintin++ into one of the", ses);
  tintin_puts2("finest clients available.  Those deserving mention though would be ", ses);
  tintin_puts2("Peter Unold, Bill Reiss, Joann Ellsworth, Jeremy Jack, and the many people", ses);
  tintin_puts2("who send us bug reports and suggestions.", ses);
  tintin_puts2("            Enjoy!!!  And keep those suggestions coming in!!\n\r", ses);
  tintin_puts2("                       The Management...", ses);
  tintin_puts2("==========================================================================", ses);
}


/*********************************************************************/
/*   tablist will display the all items in the tab completion file   */
/*********************************************************************/
void tablist(tcomplete)
     struct completenode *tcomplete;
{
   int count, done;
   char tbuf[BUFFER_SIZE];
   struct completenode *tmp;

done=0;
if (tcomplete==NULL)
  {
   tintin_puts2("Sorry.. But you have no words in your tab completion file",NULL);
   return;
  }
count=1;
*tbuf='\0';

/* 
   I'll search through the entire list, printing thre names to a line then
   outputing the line.  Creates a nice 3 column effect.  To increase the # 
   if columns, just increase the mod #.  Also.. decrease the # in the %s's
*/

for(tmp=tcomplete->next;tmp!=NULL;tmp=tmp->next) 
  {
  if ((count % 3))
     {
     if (count == 1)
        sprintf(tbuf,"%25s", tmp->strng); 
     else
        sprintf(tbuf,"%s%25s",tbuf,tmp->strng);
     done=0;
     ++count;
     }
  else
     {
     sprintf(tbuf,"%s%25s",tbuf,tmp->strng);
     tintin_puts2(tbuf, NULL);
     done=1;
     *tbuf='\0';
     ++count;
     }
  }
  if (!done)
     tintin_puts2(tbuf, NULL);
  prompt(NULL);
}

void tab_add(arg)
   char *arg;
{
   struct completenode *tmp, *tmpold, *tcomplete;
   struct completenode *newt;
   char *newcomp, buff[BUFFER_SIZE];

   tcomplete=complete_head;
  
   if ((arg == NULL) || (strlen(arg)<=0)) {
     tintin_puts("Sorry, you must have some word to add.", NULL);
     prompt(NULL);
     return;
   }
   get_arg_in_braces(arg, buff, 1);

   if ((newcomp=(char *)(malloc(strlen(buff)+1)))==NULL) {
       fprintf(stderr, "Could not allocate enough memory for that Completion word.\n");
       exit(1);
   }
   strcpy(newcomp, buff);
   tmp=tcomplete;
   while (tmp->next != NULL) {
      tmpold=tmp;
      tmp=tmp->next;
   } 

   if ((newt=(struct completenode *)(malloc(sizeof(struct completenode))))==NULL) {
       fprintf(stderr, "Could not allocate enough memory for that Completion word.\n");
       exit(1);
   }
       
   newt->strng=newcomp;
   newt->next=NULL;
   tmp->next=newt;
   tmp=newt;
   sprintf(buff,"#New word %s added to tab completion list.", arg);
   tintin_puts(buff, NULL);
   prompt(NULL);
}
   
void tab_delete(arg)
   char *arg;
{
   struct completenode *tmp, *tmpold, *tmpnext, *tcomplete;
   struct completenode *newt;
   char *oldcomp, s_buff[BUFFER_SIZE], c_buff[BUFFER_SIZE];

   tcomplete=complete_head;

   if ((arg == NULL) || (strlen(arg)<=0)) {
     tintin_puts("#Sorry, you must have some word to delete.", NULL);
     prompt(NULL);
     return;
   }
   get_arg_in_braces(arg, s_buff, 1);
   tmp=tcomplete->next;
   tmpold=tcomplete;
   if (tmpold->strng == NULL) {   /* (no list if the second node is null) */
     tintin_puts("#There are no words for you to delete!", NULL);
     prompt(NULL);
     return;
   } 
   strcpy(c_buff, tmp->strng); 
   while ((tmp->next != NULL) && (strcmp(c_buff, s_buff) != 0)) {
      tmpold=tmp;
      tmp=tmp->next;
      strcpy(c_buff, tmp->strng); 
   }
   if (tmp->next != NULL) {
      tmpnext=tmp->next;
      tmpold->next=tmpnext;
      free(tmp);
      tintin_puts("#Tab word deleted.", NULL);
      prompt(NULL);
    } else {
      if (strcmp(c_buff, s_buff) == 0) { /* for the last node to delete */
        tmpold->next=NULL;
        free(tmp);
        tintin_puts("#Tab word deleted.", NULL);
        prompt(NULL);
        return;
      }
      tintin_puts("Word not found in list.", NULL);
      prompt(NULL);
    }
}    
 
void display_info(ses)
     struct session *ses;
{
     char buf[BUFFER_SIZE];
     int actions=0;
     int aliases=0;
     int vars=0;
     int subs=0;
     int antisubs=0;
     int highs=0;
     int ignore;

    actions=count_list((ses) ? ses->actions : common_actions);
    aliases=count_list((ses) ? ses->aliases : common_aliases);
    subs=count_list((ses) ? ses->subs : common_subs);
    antisubs=count_list((ses) ? ses->antisubs : common_antisubs);
    vars=count_list((ses) ? ses->myvars : common_myvars);
    highs=count_list((ses) ? ses->highs : common_highs);
    if (ses)
        ignore = ses->ignore;
    else
        ignore =0;

    tintin_puts2("You have defined the following:",ses);
    sprintf(buf,"Actions : %d", actions);
    tintin_puts2(buf, ses);
    sprintf(buf,"Aliases : %d", aliases);
    tintin_puts2(buf, ses);
    sprintf(buf,"Substitutes : %d", subs);
    tintin_puts2(buf, ses);
    sprintf(buf,"Antisubstitutes : %d",antisubs);
    tintin_puts2(buf, ses);
    sprintf(buf,"Variables : %d", vars);
    tintin_puts2(buf, ses);
    sprintf(buf,"Highlights : %d", highs);
    tintin_puts2(buf, ses);
    sprintf(buf,"Echo : %d (1 - on, 0 - off)    Speedwalking : %d   Redraw: %d", echo, speedwalk, redraw);
    tintin_puts2(buf, ses);
    sprintf(buf,"Toggle Subs: %d   Ignore Actions : %d   PreSub-ing: %d", togglesubs, ignore, presub);
    tintin_puts2(buf, ses);


    prompt(ses);
}

