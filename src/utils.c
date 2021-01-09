/* ************************************************************************

*   File: utils.c                                       Part of CircleMUD *

*  Usage: various internal functions of a utility nature                  *

*                                                                         *

*  All rights reserved.  See license.doc for complete information.        *

*                                                                         *

*  Copyright (C) 1993, 94 by the Trustees of the Johns Hopkins University *

*  CircleMUD is based on DikuMUD, Copyright (C) 1990, 1991.               *

************************************************************************ */

#include "conf.h"
#include "sysdep.h"
#include "char_descs.h"
#include "structs.h"
#include "utils.h"
#include "db.h"
#include "comm.h"
#include "screen.h"
#include "spells.h"
#include "handler.h"
#include "interpreter.h"

extern FILE *logfile;

extern struct descriptor_data *descriptor_list;
extern struct time_data time_info;
extern struct room_data *world;
extern struct spell_info_type spell_info[];

void ASSIGNMOB(int mob, SPECIAL(fname));
char *current_short_desc(struct char_data *ch);


/* local functions */

void get_name(struct char_data* ch, char** chname);

void get_name(struct char_data* ch, char** chname)
{
  *chname = (char*) NULL;

  if (ch != (struct char_data*) NULL)
  {
    if (IS_NPC(ch))
    {
      *chname = strdup((ch)->player.short_descr);
    }
    else
    {
      *chname = strdup((ch)->player.name);
      *chname = strtok(*chname, " ");
    }
  }
}

void get_name_II(struct char_data* ch, const struct char_data* vi, char** chname)
{
  *chname = (char*) NULL;

  if (ch != (struct char_data*) NULL)
  {
    if (IS_NPC(ch))
    {
      *chname = strdup((ch)->player.short_descr);
    }
    else if (known_player(vi, ch))
    {
      *chname = strdup((ch)->player.name);
      *chname = strtok(*chname, " ");
    }
    else
    {
      if (IS_APPROVED(ch))
        *chname = strdup((ch)->player.short_descr);
      else
        *chname = current_short_desc(ch);
    }
  }
}

void get_name_IV(struct char_data* ch, char** chname)
{
  *chname = (char*) NULL;

  if (ch != (struct char_data*) NULL)
  {
    if (IS_NPC(ch))
    {
      *chname = strdup((ch)->player.short_descr);
    }
    else if (PRF_FLAGGED(ch, PRF_NOTSELF)) 
    {
      *chname = strdup((ch)->char_specials.name_dis);
    }
    else
    {
      *chname = strdup((ch)->player.name);
      *chname = strtok(*chname, " ");
    }
  }
}

void get_name_III(struct char_data* ch, const struct char_data* vi, char** chname)
{
  *chname = (char*) NULL;

  if (ch != (struct char_data*) NULL)
  {
    if (IS_NPC(ch))
    {
      *chname = strdup((ch)->player.short_descr);
    }
    else if (known_player(vi, ch) && !PRF_FLAGGED(ch, PRF_NOTSELF))    
    {
      *chname = strdup((ch)->player.name);
      *chname = strtok(*chname, " ");
    }
    else if (known_player(vi, ch) && PRF_FLAGGED(ch, PRF_NOTSELF) && PRF_FLAGGED(vi, PRF_DETECT)) 
    {
      *chname = strdup((ch)->player.name);
      *chname = strtok(*chname, " ");
    }
   else if (PRF_FLAGGED(ch, PRF_NOTSELF) && !PRF_FLAGGED(vi, PRF_DETECT)) 
   {
      *chname = strdup((ch)->char_specials.name_dis);
   }
   else 
   {
      if (IS_APPROVED(ch))
        *chname = strdup((ch)->player.short_descr);
      else
        *chname = current_short_desc(ch);
    }
  }
}

void get_pers(struct char_data* ch, const struct char_data* vi, char** chname)
{
    if (PRF_FLAGGED(ch, PRF_NOTSELF)) 
    {
      *chname = strdup((ch)->char_specials.name_dis);
    }
    else if (CAN_SEE(vi, ch))
    {
        get_name_II(ch, vi, chname);
    }
    else
    {
        *chname = strdup("someone");
    }
}

void get_pers_II(struct char_data* ch, const struct char_data* vi, char** chname)
{
   if (CAN_SEE(vi, ch))
   {
      get_name_II(ch, vi, chname);
   }
   else
   {
      *chname = strdup("someone");
   }
}

void choose_name(struct char_data* ch,  char** chname)
{
    if (PRF_FLAGGED(ch, PRF_NOTSELF)) 
    {
        get_name_IV(ch, chname);
    }
    else
    {
        get_name(ch, chname);
    }
}

void choose_name_II(struct char_data* ch, const struct char_data* vi, char** chname)
{
    if (PRF_FLAGGED(ch, PRF_NOTSELF)) 
    {
        get_name_III(ch, vi, chname);
    }
    else 
    {
        get_name_II(ch, vi, chname);
    }
}

/*
void get_name(struct char_data* ch, char** chname)
{

  *chname = (char*) NULL;



  if (ch != (struct char_data*) NULL)

  {

    if (IS_NPC(ch))

    {

      *chname = strdup((ch)->player.short_descr);

    }

    else if (PRF_FLAGGED(ch, PRF_NOTSELF)) {

      *chname = strdup((ch)->char_specials.name_dis);

    }



   else {

    *chname = strdup((ch)->player.name);

      *chname = strtok(*chname, " ");

    }



  }

}

void get_name_II(struct char_data* ch, const struct char_data* vi, char** chname)
{

  *chname = (char*) NULL;



  if (ch != (struct char_data*) NULL)

  {

    if (IS_NPC(ch))

    {

      *chname = strdup((ch)->player.short_descr);

    }

    else if (known_player(vi, ch) && !PRF_FLAGGED(ch, PRF_NOTSELF))    {

      *chname = strdup((ch)->player.name);

      *chname = strtok(*chname, " ");

    }



  else if (known_player(vi, ch) && PRF_FLAGGED(ch, PRF_NOTSELF) && PRF_FLAGGED(vi, PRF_DETECT)) {

      *chname = strdup((ch)->player.name);

      *chname = strtok(*chname, " ");

    }





     else if (PRF_FLAGGED(ch, PRF_NOTSELF) && !PRF_FLAGGED(vi, PRF_DETECT)) {

      *chname = strdup((ch)->char_specials.name_dis);

    }



   else {

      *chname = strdup((ch)->player.short_descr);

    }

  }

}

void get_name_III(struct char_data* ch, char** chname)
{

  *chname = (char*) NULL;



  if (ch != (struct char_data*) NULL)

  {

    if (IS_NPC(ch))

    {

      *chname = strdup((ch)->player.short_descr);

    }

    else

    {

      *chname = strdup((ch)->player.name);

      *chname = strtok(*chname, " ");

    }

  }

}

void get_name_IV(struct char_data* ch, const struct char_data* vi, char** chname)
{

  *chname = (char*) NULL;



  if (ch != (struct char_data*) NULL)

  {

    if (IS_NPC(ch))

    {

      *chname = strdup((ch)->player.short_descr);

    }

    else if (known_player(vi, ch))

    {

      *chname = strdup((ch)->player.name);

      *chname = strtok(*chname, " ");

    }

    else

    {

      *chname = strdup((ch)->player.short_descr);

    }

  }

}

void get_pers(struct char_data* ch, const struct char_data* vi, char** chname)
{

   if (CAN_SEE(vi, ch))

   {

      get_name_II(ch, vi, chname);

   }

   else

   {

      *chname = strdup("someone");

   }

} */

// creates a random number in interval [from; to]
int number(int from, int to)
{
  // error checking in case people call number() incorrectly
  //
  // That's all well and good, but it uses an extra int,
  // which is an extra 8-bytes that we don't need. Let's
  // trim that back by 6-bytes and use pointers to do an XOR
  // swap instead. This will perform the swap in-place and
  // does not require the extra variable for use as temporary
  // storage.
  // -- Branc, 12/30/2011
  if (from > to) 
  {
    //int tmp = from;
    //from = to;
    //to = tmp;

    // A little pneumonic: from = this, to = that..."from this, to that".
    // Aren't I just so clever? Probably not.
    // -- Branc, 12/30/2011
    int *this, *that;

    this = &from;
    that = &to;

    *this ^= *that;
    *that ^= *this;
    *this ^= *that;

    log("SYSERR: number() should be called with lowest, than highest.  number(%d, %d), not number(%d, %d).", from, to, to, from);
  }

  return ((circle_random() % (to - from + 1)) + from);
}

/* simulates dice roll */
int dice(int number, int size)
{
  int sum = 0;

  if (size <= 0 || number <= 0)
    return (0);

  while (number-- > 0)
    sum += ((circle_random() % size) + 1);

  return (sum);
}

int MIN(int a, int b)
{
  return (a < b ? a : b);
}

int MAX(int a, int b)
{
  return (a > b ? a : b);
}

char *CAP(char *txt)
{
  *txt = UPPER(*txt);
  return (txt);
}

/* Create a duplicate of a string */
char *str_dup(const char *source)
{
  char *new_z;

  CREATE(new_z, char, strlen(source) + 1);

  return (strcpy(new_z, source));
}


// Strips \r\n from end of string.
void prune_crlf(char *txt)
{
  int i = strlen(txt) - 1;

  while (txt[i] == '\n' || txt[i] == '\r')
  {
    txt[i--] = '\0';
  }
}

/* str_cmp: a case-insensitive version of strcmp */
/* returns: 0 if equal, 1 if arg1 > arg2, -1 if arg1 < arg2  */
/* scan 'till found different or end of both                 */
int str_cmp(const char *arg1, const char *arg2)
{
  int chk, i;

  if (arg1 == NULL || arg2 == NULL) 
  {
    log("SYSERR: str_cmp() passed a NULL pointer, %p or %p.", arg1, arg2);
    return (0);
  }

  for (i = 0; arg1[i] || arg2[i]; i++)
  {
    if ((chk = LOWER(arg1[i]) - LOWER(arg2[i])) != 0)
    {
      return(chk);
    }
  } 

  return (0);
}

/* strn_cmp: a case-insensitive version of strncmp */
/* returns: 0 if equal, 1 if arg1 > arg2, -1 if arg1 < arg2  */
/* scan 'till found different, end of both, or n reached     */
int strn_cmp(const char *arg1, const char *arg2, int n)
{
  int chk, i;

  if (arg1 == NULL || arg2 == NULL) 
  {
    log("SYSERR: strn_cmp() passed a NULL pointer, %p or %p.", arg1, arg2);

    return(0);
  }

  for (i = 0; (arg1[i] || arg2[i]) && (n > 0); i++, n--)
  {
    if ((chk = LOWER(arg1[i]) - LOWER(arg2[i])) != 0)
    {
      return (chk);
    }
  }

  return (0);

}

/* log a death trap hit */
void log_death_trap(struct char_data * ch)
{
  char buf[256];

  GET_NAME(ch, chname);
  sprintf(buf, "%s hit death trap #%d (%s)", chname, GET_ROOM_VNUM(IN_ROOM(ch)), world[ch->in_room].name);

  FREE_NAME(chname);
  mudlog(buf, BRF, LVL_IMMORT, TRUE);
}

/* writes a string to the log */
void basic_mud_log(const char *format, ...)
{
  va_list args;
  time_t ct = time(0);
  char *time_s = asctime(localtime(&ct));

  if (logfile == NULL)
  {
    puts("SYSERR: Using log() before stream was initialized!");
  }

  if (format == NULL)
  {
    format = "SYSERR: log() received a NULL format.";
  }



  time_s[strlen(time_s) - 1] = '\0';



  fprintf(logfile, "%-15.15s :: ", time_s + 4);



  va_start(args, format);

  vfprintf(logfile, format, args);

  va_end(args);



  fprintf(logfile, "\n");

  fflush(logfile);

}





/* the "touch" command, essentially. */

int touch(const char *path)

{

  FILE *fl;



  if (!(fl = fopen(path, "a"))) {

    log("SYSERR: %s: %s", path, strerror(errno));

    return (-1);

  } else {

    fclose(fl);

    return (0);

  }

}





/*

 * mudlog -- log mud messages to a file & to online imm's syslogs

 * based on syslog by Fen Jul 3, 1992

 */

void mudlog(const char *str, int type, int level, int file)

{

  char buf[MAX_STRING_LENGTH], tp;

  struct descriptor_data *i;



  if (str == NULL)

    return;     /* eh, oh well. */

  if (file)

    log(str);

  if (level < 0)

    return;



  sprintf(buf, "[ %s ]\r\n", str);



  for (i = descriptor_list; i; i = i->next) {

    if (STATE(i) != CON_PLAYING || IS_NPC(i->character)) /* switch */

      continue;

    if (GET_LEVEL(i->character) < level)

      continue;

    if (PLR_FLAGGED(i->character, PLR_WRITING))

      continue;

    tp = ((PRF_FLAGGED(i->character, PRF_LOG1) ? 1 : 0) +

          (PRF_FLAGGED(i->character, PRF_LOG2) ? 2 : 0));

    if (tp < type)

      continue;



    send_to_char(CCGRN(i->character, C_NRM), i->character);

    send_to_char(buf, i->character);

    send_to_char(CCNRM(i->character, C_NRM), i->character);

  }

}



void immcharlog(const char *str)

{

  char b[MAX_STRING_LENGTH];

  struct descriptor_data *i;



  if (str == NULL)

    return;     /* eh, oh well. */



  sprintf(b, "[ %s ]\r\n", str);



  for (i = descriptor_list; i; i = i->next) {

    if (STATE(i) != CON_PLAYING || IS_NPC(i->character)) /* switch */

      continue;

    if (PLR_FLAGGED(i->character, PLR_WRITING))

      continue;

    if((PLR_FLAGGED(i->character, PLR_IMMCHAR)) || (GET_LEVEL(i->character) > 30))

    {

      send_to_char(CCGRN(i->character, C_NRM), i->character);

      send_to_char(b, i->character);

      send_to_char(CCNRM(i->character, C_NRM), i->character);

    }

  }

}





/*

 * If you don't have a 'const' array, just cast it as such.  It's safer

 * to cast a non-const array as const than to cast a const one as non-const.

 * Doesn't really matter since this function doesn't change the array though.

 */

void sprintbit(bitvector_t bitvector, const char *names[], char *result) {
  long nr;
  *result = '\0';

  for (nr = 0; bitvector; bitvector >>= 1) {

    if (IS_SET(bitvector, 1)) {

      if (*names[nr] != '\n') {
        strcat(result, names[nr]);
        strcat(result, " ");
      } else {
        strcat(result, "UNDEFINED ");
      }
    }

    if (*names[nr] != '\n') {
      nr++;
    }
  }

  if (!*result) {
    strcpy(result, "NOBITS ");
  }
}


void sprintspellbit(bitvector_t bitvector, const char *names[], char *result)

{

  long nr;



  *result = '\0';



  for (nr = 0; bitvector; bitvector >>= 1) {

    if (IS_SET(bitvector, 1)) {

      if (*names[nr] != '\n') {

        strcat(result, names[nr]);

        strcat(result, ", ");

      } else

        strcat(result, "UNDEFINED ");

    }



    if (*names[nr] != '\n')

      nr++;

  }



  if (!*result)

    strcpy(result, "NOBITS ");

}





void sprinttype(int type, const char *names[], char *result)

{

  int nr = 0;



  while (type && *names[nr] != '\n') {

    type--;

    nr++;

  }



  if (*names[nr] != '\n')

    strcpy(result, names[nr]);

  else

    strcpy(result, "UNDEFINED");

}





/* Calculate the REAL time passed over the last t2-t1 centuries (secs) */

struct time_info_data *real_time_passed(time_t t2, time_t t1)

{

  long secs;

  static struct time_info_data now;



  secs = (long) (t2 - t1);



  now.hours = (secs / SECS_PER_REAL_HOUR) % 24;	/* 0..23 hours */

  secs -= SECS_PER_REAL_HOUR * now.hours;



  now.day = (secs / SECS_PER_REAL_DAY);	/* 0..34 days  */

  secs -= SECS_PER_REAL_DAY * now.day;



  now.month = -1;

  now.year = -1;



  return (&now);

}







/* Calculate the MUD time passed over the last t2-t1 centuries (secs) */

struct time_info_data *mud_time_passed(time_t t2, time_t t1)

{

  long secs;

  static struct time_info_data now;



  secs = (long) (t2 - t1);



  now.hours = (secs / SECS_PER_MUD_HOUR) % 24;	/* 0..23 hours */

  secs -= SECS_PER_MUD_HOUR * now.hours;



  now.day = (secs / SECS_PER_MUD_DAY) % 35;	/* 0..34 days  */

  secs -= SECS_PER_MUD_DAY * now.day;



  now.month = (secs / SECS_PER_MUD_MONTH) % 12;	/* 0..16 months */

  secs -= SECS_PER_MUD_MONTH * now.month;



  now.year = (secs / SECS_PER_MUD_YEAR);	/* 0..XX? years */



  return (&now);

}







struct time_info_data *age(struct char_data * ch)

{

  static struct time_info_data player_age;



  player_age = *mud_time_passed(time(0), ch->player.time.birth);



  player_age.year += 17;	/* All players start at 17 */



  return (&player_age);

}





/* Check if making CH follow VICTIM will create an illegal */

/* Follow "Loop/circle"                                    */

bool circle_follow(struct char_data * ch, struct char_data * victim)

{

  struct char_data *k;



  for (k = victim; k; k = k->master) {

    if (k == ch)

      return (TRUE);

  }



  return (FALSE);

}







/* Called when stop following persons, or stopping charm */

/* This will NOT do if a character quits/dies!!          */

void stop_follower(struct char_data * ch)

{

  struct char_data* chMaster = ch->master;

  struct follow_type *j, *k;



  if (ch->master == NULL) {

//    core_dump();

    return;

  }



  if (ch->master->followers->follower == ch) {	/* Head of follower-list? */

    k = ch->master->followers;

    ch->master->followers = k->next;

    free(k);

  } else {			/* locate follower who is not head of list */

    for (k = ch->master->followers; k->next->follower != ch; k = k->next);



    j = k->next;

    k->next = j->next;

    free(j);

  }



  ch->master = NULL;



  REMOVE_BIT(AFF_FLAGS(ch), AFF_CHARM | AFF_GROUP);



  if (IS_AFFECTED(ch, AFF_CHARM)) {

    act("You realize that you can no longer follow $N!", FALSE, ch, 0, 

      chMaster, TO_CHAR);

    act("$n can no longer follow $N!", FALSE, ch, 0, chMaster, TO_NOTVICT);

    act("$n can no longer follow you!", FALSE, ch, 0, chMaster, TO_VICT);

    if (affected_by_spell(ch, SPELL_CHARM))

      affect_from_char(ch, SPELL_CHARM);

  } else {

    act("You stop following $N.", FALSE, ch, 0, chMaster, TO_CHAR);

    act("$n stops following $N.", TRUE, ch, 0, chMaster, TO_NOTVICT);

    if (CAN_SEE(chMaster, ch))

      act("$n stops following you.", TRUE, ch, 0, chMaster, TO_VICT);

  }

}







/* Called when a character that follows/is followed dies */

void die_follower(struct char_data * ch)

{

  struct follow_type *j, *k;



  if (ch->master)

    stop_follower(ch);



  for (k = ch->followers; k; k = j) {

    j = k->next;

    stop_follower(k->follower);

  }

}







/* Do NOT call this before having checked if a circle of followers */

/* will arise. CH will follow leader                               */

void add_follower(struct char_data * ch, struct char_data * leader)

{

  struct follow_type *k;



  if (ch->master) {

    core_dump();

    return;

  }



  ch->master = leader;



  CREATE(k, struct follow_type, 1);



  k->follower = ch;

  k->next = leader->followers;

  leader->followers = k;



  act("You now follow $N.", FALSE, ch, 0, leader, TO_CHAR);

  if (AFF_FLAGGED(ch, AFF_SNEAK) && check_skill(ch, 101, SKILL_SNEAK) && 
      !AFF_FLAGGED(leader, AFF_SENSE_LIFE))
    return;



  if (CAN_SEE(leader, ch))

    act("$n starts following you.", TRUE, ch, 0, leader, TO_VICT);

  act("$n starts to follow $N.", TRUE, ch, 0, leader, TO_NOTVICT);

}



/*

 * get_line reads the next non-blank line off of the input stream.

 * The newline character is removed from the input.  Lines which begin

 * with '*' are considered to be comments.

 *

 * Returns the number of lines advanced in the file.

 */

int get_line(FILE * fl, char *buf)

{

  char temp[256];

  int lines = 0;



  do {

    fgets(temp, 256, fl);

    if (feof(fl))

      return (0);

    lines++;

  } while (*temp == '*' || !*temp);



  temp[strlen(temp) - 1] = '\0';

  strcpy(buf, temp);

  return (lines);

}

/*

int get_filename2(char *filename, size_t fbufsize, int mode, const char *orig_name)

{

  const char *prefix, *middle, *suffix;

  char name[PATH_MAX], *ptr;



  if (orig_name == NULL || *orig_name == '\0' || filename == NULL) {

    log("SYSERR: NULL pointer or empty string passed to get_filename(), %p or %p.",

		orig_name, filename);

    return (0);

  }



  switch (mode) {

  case CRASH_FILE:

    prefix = "plrobjs";

    suffix = "objs";

    break;

  case BACKUP_FILE:

    prefix = "plrobjs";

    suffix = "obj_bak";

    break;

  case ALIAS_FILE:

    prefix = "plralias";

    suffix = "alias";

    break;

  case ETEXT_FILE:

    prefix = "plrtext";

    suffix = "text";

    break;

  default:

    return (0);

  }



  strlcpy(name, orig_name, sizeof(name));

  for (ptr = name; *ptr; ptr++)

    *ptr = LOWER(*ptr);



  switch (LOWER(*name)) {

  case 'a':  case 'b':  case 'c':  case 'd':  case 'e':

    middle = "A-E";

    break;

  case 'f':  case 'g':  case 'h':  case 'i':  case 'j':

    middle = "F-J";

    break;

  case 'k':  case 'l':  case 'm':  case 'n':  case 'o':

    middle = "K-O";

    break;

  case 'p':  case 'q':  case 'r':  case 's':  case 't':

    middle = "P-T";

    break;

  case 'u':  case 'v':  case 'w':  case 'x':  case 'y':  case 'z':

    middle = "U-Z";

    break;

  default:

    middle = "ZZZ";

    break;

  }



  snprintf(filename, fbufsize, "%s%s"SLASH"%s.%s", prefix, middle, name, suffix);

  return (1);

}*/



int get_filename(char *orig_name, char *filename, int mode)

{

  char *prefix, *middle, *suffix;

  char name[64], *ptr;



  if (orig_name == NULL || *orig_name == '\0' || filename == NULL) {

    log("SYSERR: NULL pointer or empty string passed to get_filename(), %p or %p.", orig_name, filename);

    return (0);

  }



  switch (mode) {

  case CRASH_FILE:

    prefix = "plrobjs";

    suffix = "objs";

    break;

  case BACKUP_FILE:

    prefix = "plrobjs";

    suffix = "obj_bak";

    break;

  case ALIAS_FILE:

    prefix = "plralias";

    suffix = "alias";

    break;

  case ETEXT_FILE:

    prefix = "plrtext";

    suffix = "text";

    break;

  default:

    return 0;

    break;

  }



  strcpy(name, orig_name);

  for (ptr = name; *ptr; ptr++)

    *ptr = LOWER(*ptr);



  switch (LOWER(*name)) {

  case 'a':  case 'b':  case 'c':  case 'd':  case 'e':

    middle = "A-E";

    break;

  case 'f':  case 'g':  case 'h':  case 'i':  case 'j':

    middle = "F-J";

    break;

  case 'k':  case 'l':  case 'm':  case 'n':  case 'o':

    middle = "K-O";

    break;

  case 'p':  case 'q':  case 'r':  case 's':  case 't':

    middle = "P-T";

    break;

  case 'u':  case 'v':  case 'w':  case 'x':  case 'y':  case 'z':

    middle = "U-Z";

    break;

  default:

    middle = "ZZZ";

    break;

  }



  sprintf(filename, "%s/%s/%s.%s", prefix, middle, name, suffix);

  return (1);

}





int num_pc_in_room(struct room_data *room)

{

  int i = 0;

  struct char_data *ch;



  for (ch = room->people; ch != NULL; ch = ch->next_in_room)

    if (!IS_NPC(ch))

      i++;



  return (i);

}



/*

 * This function (derived from basic fork(); abort(); idea by Erwin S.

 * Andreasen) causes your MUD to dump core (assuming you can) but

 * continue running.  The core dump will allow post-mortem debugging

 * that is less severe than assert();  Don't call this directly as

 * core_dump_unix() but as simply 'core_dump()' so that it will be

 * excluded from systems not supporting them. (e.g. Windows '95).

 *

 * You still want to call abort() or exit(1) for

 * non-recoverable errors, of course...

 *

 * XXX: Wonder if flushing streams includes sockets?

 */

void core_dump_real(const char *who, int line)

{

  log("SYSERR: Assertion failed at %s:%d!", who, line);



#if defined(CIRCLE_UNIX)

  /* These would be duplicated otherwise... */

  fflush(stdout);

  fflush(stderr);

  fflush(logfile);



  /*

   * Kill the child so the debugger or script doesn't think the MUD

   * crashed.  The 'autorun' script would otherwise run it again.

   */

  if (fork() == 0)

    abort();

#endif

}



/* string manipulation fucntion originally by Darren Wilson */

/* (wilson@shark.cc.cc.ca.us) improved and bug fixed by Chris (zero@cnw.com) */

/* completely re-written again by M. Scott 10/15/96 (scottm@workcommn.net), */

/* substitute appearances of 'pattern' with 'replacement' in string */

/* and return the # of replacements */

int replace_str(char **string, char *pattern, char *replacement, int rep_all,

		int max_size) {

   char *replace_buffer = NULL;

   char *flow, *jetsam, temp;

   int len, i;

   

   if ((strlen(*string) - strlen(pattern)) + strlen(replacement) > max_size)

     return -1;

   

   CREATE(replace_buffer, char, max_size);

   i = 0;

   jetsam = *string;

   flow = *string;

   *replace_buffer = '\0';

   if (rep_all) {

      while ((flow = (char *)strstr(flow, pattern)) != NULL) {

	 i++;

	 temp = *flow;

	 *flow = '\0';

	 if ((strlen(replace_buffer) + strlen(jetsam) + strlen(replacement)) > max_size) {

	    i = -1;

	    break;

	 }

	 strcat(replace_buffer, jetsam);

	 strcat(replace_buffer, replacement);

	 *flow = temp;

	 flow += strlen(pattern);

	 jetsam = flow;

      }

      strcat(replace_buffer, jetsam);

   }

   else {

      if ((flow = (char *)strstr(*string, pattern)) != NULL) {

	 i++;

	 flow += strlen(pattern);  

	 len = ((char *)flow - (char *)*string) - strlen(pattern);

   

	 strncpy(replace_buffer, *string, len);

	 strcat(replace_buffer, replacement);

	 strcat(replace_buffer, flow);

      }

   }

   if (i == 0) return 0;

   if (i > 0) {

      RECREATE(*string, char, strlen(replace_buffer) + 3);

      strcpy(*string, replace_buffer);

   }

   free(replace_buffer);

   return i;

}





/* re-formats message type formatted char * */

/* (for strings edited with d->str) (mostly olc and mail)     */

void format_text(char **ptr_string, int mode, struct descriptor_data *d, int maxlen) {

   int total_chars, cap_next = TRUE, cap_next_next = FALSE;

   char *flow, *start = NULL, temp;

   /* warning: do not edit messages with max_str's of over this value */

   char formated[MAX_STRING_LENGTH];

   

   flow   = *ptr_string;

   if (!flow) return;



   if (IS_SET(mode, FORMAT_INDENT)) {

      strcpy(formated, "   ");

      total_chars = 3;

   }

   else {

      *formated = '\0';

      total_chars = 0;

   } 



   while (*flow != '\0') {

      while ((*flow == '\n') ||

	     (*flow == '\r') ||

	     (*flow == '\f') ||

	     (*flow == '\t') ||

	     (*flow == '\v') ||

	     (*flow == ' ')) flow++;



      if (*flow != '\0') {



	 start = flow++;

	 while ((*flow != '\0') &&

		(*flow != '\n') &&

		(*flow != '\r') &&

		(*flow != '\f') &&

		(*flow != '\t') &&

		(*flow != '\v') &&

		(*flow != ' ') &&

		(*flow != '.') &&

		(*flow != '?') &&

		(*flow != '!')) flow++;



	 if (cap_next_next) {

	    cap_next_next = FALSE;

	    cap_next = TRUE;

	 }



	 /* this is so that if we stopped on a sentance .. we move off the sentance delim. */

	 while ((*flow == '.') || (*flow == '!') || (*flow == '?')) {

	    cap_next_next = TRUE;

	    flow++;

	 }

	 

	 temp = *flow;

	 *flow = '\0';



	 if ((total_chars + strlen(start) + 1) > 79) {

	    strcat(formated, "\r\n");

	    total_chars = 0;

	 }



	 if (!cap_next) {

	    if (total_chars > 0) {

	       strcat(formated, " ");

	       total_chars++;

	    }

	 }

	 else {

	    cap_next = FALSE;

	    *start = UPPER(*start);

	 }



	 total_chars += strlen(start);

	 strcat(formated, start);



	 *flow = temp;

      }



      if (cap_next_next) {

	 if ((total_chars + 3) > 79) {

	    strcat(formated, "\r\n");

	    total_chars = 0;

	 }

	 else {

	    strcat(formated, "  ");

	    total_chars += 2;

	 }

      }

   }

   strcat(formated, "\r\n");



   if (strlen(formated) > maxlen) formated[maxlen] = '\0';

   RECREATE(*ptr_string, char, MIN(maxlen, strlen(formated)+3));

   strcpy(*ptr_string, formated);

}

char * describe_align(int align) {
    if (align == 850) {
        return "Good";
    } else if (align == 0) {
        return "Neutral";
    } else if (align == -850) {
        return "Evil";
    } else {
        return "";
    }
}

char * describe_ethos(int ethos) {
    if (ethos == 850) {
        return "Lawful";
    } else if (ethos == 0) {
        return "Neutral";
    } else if (ethos == -850) {
        return "Chaotic";
    } else {
        return "";
    }
}

char * describe_char_align(int ethos, int align) {
    char align_str[40];

    if (ethos == 0 && align == 0) {
        return "True Neutral";
    } else {
        strcpy(align_str, describe_ethos(ethos));
        strcat(align_str, " ");
        strcat(align_str, describe_align(align));
    }

    return align_str;
}


char * describe_clan(int clannum) {
  switch(clannum) {
    case CLAN_KOS :
      return "Knights of Solamnia";

    case CLAN_CONCLAVE :
      return "Conclave of Magic";

    case CLAN_HOLY_ORDER :
      return "Holy Order of the Stars";

    case CLAN_BLACKBLADE :
      return "Guild of the Black Blade";

    default:
      return "no clan";
  }
}

/*  PDH  3/29/99

 *  new money system: plat, steel, gold, copper

 *        plat  stee  gold  copp

 * plat      1     2    10   100

 * stee    1/2     1     5    50

 * gold   1/10   1/5     1    10

 * copp  1/100  1/50  1/10     1

 */

int money_to_copper(struct money_data money)
{

  int copper;



  copper = money.platinum * 1000 +

    money.steel * 100 +

    money.gold * 10 +

    money.copper;



  return copper;

}


void copper_to_money(struct money_data * tar, int copper)
{
  int pp, ss, gg, cc;

  pp = copper / 1000;   copper -= (1000 * pp);

  ss = copper /  100;   copper -= ( 100 * ss);

  gg = copper /   10;   copper -= (  10 * gg);

  cc = copper;          copper -=         cc ;



  if ( copper != 0 ) {

    log("SYSERR: copper_to_money() error in conversion");

  }



  tar->platinum = pp;

  tar->steel = ss;

  tar->gold = gg;

  tar->copper = cc;

}


void money_subtract(struct money_data * tar, struct money_data subAmt)
{
  int tarCp;

  if ( money_to_copper(*tar) < money_to_copper(subAmt) ) {
    log("SYSERR: money_subtract() - target less than sub amount");
    return;
  }

  if ( (tar->platinum >= subAmt.platinum) &&

       (tar->steel >= subAmt.steel) &&

       (tar->gold >= subAmt.gold) &&

       (tar->copper >= subAmt.copper) ) {

    tar->platinum -= subAmt.platinum;

    tar->steel -= subAmt.steel;

    tar->gold -= subAmt.gold;

    tar->copper -= subAmt.copper;



    return;

  }



  /*  otherwise, convert tar to copper, subtract, then reconvert  */

  tarCp = money_to_copper(*tar) - money_to_copper(subAmt);

  if ( tarCp < 0 ) {

    log("SYSERR: money_subtract() - target less than sub amount (II)");

    return;

  }



  copper_to_money(tar, tarCp);

}


void money_assign(struct money_data * tar, struct money_data amount)
{

}

/*
 *  returns a   char *   string of the coins
 *
 *  IMPORTANT - multiple calls MUST occur on separate command lines,
 *  otherwise the reference to the string is the same, and the SAME
 *  text appears in all the calls
 */
char  describeMoneyBuf[1000];

char * describe_money(struct money_data amount)
{
  int   comma = 0, coins = 0;

  strcpy(describeMoneyBuf, "");

  comma = 0;

  if ( amount.platinum > 0 ) {
    sprintf(describeMoneyBuf, "%s%s%d platinum",

	    describeMoneyBuf, (comma ? ", " : ""),

	    amount.platinum);

    comma++;

    if ( amount.platinum > 1 ) {

      coins++;

    }

  }



  if ( amount.steel > 0 ) {

    sprintf(describeMoneyBuf, "%s%s%d steel",

	    describeMoneyBuf, (comma ? ", " : ""),

	    amount.steel);

    comma++;

    if ( amount.steel > 1 ) {

      coins++;

    }

  }



  if ( amount.gold > 0 ) {

    sprintf(describeMoneyBuf, "%s%s%d gold",

	    describeMoneyBuf, (comma ? ", " : ""),

	    amount.gold);

    comma++;

    if ( amount.gold > 1 ) {

      coins++;

    }

  }



  if ( amount.copper > 0 ) {

    sprintf(describeMoneyBuf, "%s%s%d copper",

	    describeMoneyBuf, (comma ? ", " : ""),

	    amount.copper);

    comma++;

    if ( amount.copper > 1 ) {

      coins++;

    }

  }



  if ( ! comma ) {

    sprintf(describeMoneyBuf, "%sno coins", describeMoneyBuf);

  } else {

    if ( coins  ||  (comma > 1) ) {

      sprintf(describeMoneyBuf, "%s coins", describeMoneyBuf);

    } else {

      sprintf(describeMoneyBuf, "%s coin", describeMoneyBuf);

    }

  }



  return describeMoneyBuf;

}


void copper_to_money_random(struct money_data * tar, int copper)
{

  int rand;



  tar->platinum = tar->steel = tar->gold = tar->copper = 0;



  if ( copper >= 1000 ) {

    rand = number(1,100);

    tar->platinum = ( ( rand * copper ) / 100 ) / 1000;

    copper -= ( tar->platinum * 1000 );

  }

  if ( copper >= 100 ) {

    rand = number(1,100);

    tar->steel = ( ( rand * copper ) / 100 ) / 100;

    copper -= ( tar->steel * 100 );

  }

  if ( copper >= 10 ) {

    rand = number(1,100);

    tar->gold = ( ( rand * copper ) / 100 ) / 10;

    copper -= ( tar->gold * 10 );

  }

  if ( copper > 0 ) {

    tar->copper = copper;

  }

}


/*
 * takes 2 money_data structs and compares them based on:
 * total value, if strict==FALSE
 * actual coin type values, if strict==TRUE
 *
 * return values:
 * if ( more <  less )    -1
 * if ( more == less )     0
 * if ( more  > less )     1
 */
int money_compare(struct money_data more, struct money_data less, int strict)
{

  int moreCp = money_to_copper(more);

  int lessCp = money_to_copper(less);

  struct money_data  temp;





  if ( strict == FALSE ) {

    if ( moreCp == lessCp ) {

      return 0;

    } else if ( moreCp > lessCp ) {

      return 1;

    } else {

      return -1;

    }

  }





  temp.platinum = more.platinum - less.platinum;

  temp.steel    = more.steel - less.steel;

  temp.gold     = more.gold - less.gold;

  temp.copper   = more.copper - less.copper;



  if ( (temp.platinum < 0) || (temp.gold < 0) ||

       (temp.steel < 0) || (temp.copper < 0) ) {

    return -1;

  }



  if ( (temp.platinum == 0) && (temp.gold == 0) &&

       (temp.steel == 0) && (temp.copper == 0) ) {

    return 0;

  }



  return 1;

}


int check_skill(struct char_data * ch, int prob, int skillnum)
{
  int percent = number(1, prob);
  
  if (percent == prob)
    return FALSE;

  return use_skill(ch, percent, skillnum);

}


int assert_fighting(struct char_data * ch)
{

  if (!ch || !FIGHTING(ch) || (FIGHTING(ch) == NULL))

    return 0;



  if (ch->in_room != FIGHTING(ch)->in_room)

  {

    stop_fighting(ch);

    return 0;

  }



  return 1;

}


void check_improve(struct char_data * ch, int skill)
{

  int chance, skilled;



  if ((ch->player_specials->saved.skills[skill].reflections) >=

       spell_info[skill].max_day)

    return;



  chance = spell_info[skill].applymod;



  switch (spell_info[skill].attrib)

  {

    case ATTR_STR:

      skilled = GET_STR(ch);

      break;

    case ATTR_DEX:

      skilled = GET_DEX(ch);

      break;

    case ATTR_WIS:

      skilled = GET_WIS(ch);

      break;

    case ATTR_CON:

      skilled = GET_CON(ch);

      break;

    case ATTR_CHA:

      skilled = GET_CHA(ch);

      break;

    case ATTR_INT:

      skilled = GET_INT(ch);

      break;

    default:

      skilled = 0;

  }



  skilled = ((skilled + GET_INT(ch)) / 2);



  skilled *= 5;  // figure that's, at most, 100 for skilled

  chance *= skilled;  // so that's probably around 5k at worst, we hope



  chance *= (100 - GET_SKILL(ch, skill));  // modify for learnedness



  // figure we're looking at, at worst, about 10-20k, and probably higher.

  // that makes the following more understandable.



  chance /= 20000;



  if (chance > number(0, 100))

    ch->player_specials->saved.skills[skill].reflections++;

}


int use_skill(struct char_data * ch, int percent, int skillnum)
{

  int mod = 0;
  int skilled = 0;
  int skillval = 0;

  if (!GET_SKILL(ch, skillnum) && !IS_NPC(ch))
    return 0;

  if (!IS_NPC(ch))
    check_improve(ch, skillnum);

  switch (spell_info[skillnum].attrib)

  {

    case ATTR_STR:

      skilled = GET_STR(ch);

      break;

    case ATTR_DEX:

      skilled = GET_DEX(ch);

      break;

    case ATTR_WIS:

      skilled = GET_WIS(ch);

      break;

    case ATTR_CON:

      skilled = GET_CON(ch);

      break;

    case ATTR_CHA:

      skilled = GET_CHA(ch);

      break;

    case ATTR_INT:

      skilled = GET_INT(ch);

      break;

    default:

      skilled = 0;

  }
  
  skilled -= 10;
  skilled /= 2;
  mod += skilled;
  
  if (skillnum == SKILL_BACKSTAB && check_skill(ch, 101, SKILL_IMPROVED_BACKSTAB))
    mod += dice(1, GET_SKILL(ch, SKILL_IMPROVED_BACKSTAB)) / 20;
	
  if ((skillnum == SKILL_HIDE || skillnum == SKILL_SNEAK) && check_skill(ch, 101, SKILL_IMPROVED_STEALTH))
    mod += dice(1, GET_SKILL(ch, SKILL_IMPROVED_STEALTH)) / 20;
	
  if (GET_RACE(ch) == RACE_HUMAN)
    mod += 2;

  skillval = GET_SKILL(ch, skillnum);
  
  if (PLR_FLAGGED(ch, PLR_CAT) && skillnum == SKILL_SNEAK)
    skillval = MAX(95, GET_SKILL(ch, SKILL_SNEAK));

  if (IS_NPC(ch) && spell_info[skillnum].min_level[(int)GET_CLASS(ch)] <= GET_LEVEL(ch))
    skillval = 90;

  if (IS_NPC(ch) && 
     ((IS_AFFECTED(ch, AFF_SNEAK) && skillnum == SKILL_SNEAK) ||
      (IS_AFFECTED(ch, AFF_HIDE) && skillnum == SKILL_HIDE)))
    skillval = 90;
	
  if (percent > skillval + mod)

    return 0;

  return 1;

}


int is_weapon(int type)
{

  if (type == SKILL_BACKSTAB)

    return 1;

  if (type == SKILL_KICK)

    return 1;

  if (type == SKILL_BASH)

    return 1;

  if (type == SKILL_GORE)

    return 1;





  if (type < TYPE_HIT)

    return 0;

  if (type > TYPE_CLEAVE)

    return 0;



  return 1;

}


int is_color(struct char_data *ch)
{

if ((PRF_FLAGGED(ch, PRF_COLOR_1) || PRF_FLAGGED(ch, PRF_COLOR_2)) || (IS_NPC(ch) && MOB_FLAGGED(ch, MOB_QUEST)))

  return 1;

else

  return 0;

}


int is_killer(struct char_data *ch)
{

if (PLR_FLAGGED(ch, PLR_KILLER))

  return 1;

else

   return 0;

}


int is_thief(struct char_data *ch)
{

if (PLR_FLAGGED(ch, PLR_THIEF))

  return 1;

else

   return 0;

}


void max_group_exp_mult(struct char_data *ch)
{

  struct char_data *k;
  struct follow_type *f;
  int count = 0;
  int mult = 0;

  if (!ch->master || !ch->followers)
    return;

  if (ch->master)
    k = ch->master;
  else
    k = ch;

  for (f = k->followers; f; f = f->next) {
    count++;
  }

  if (count > 0)
    mult = (count + 1) * 10;

  for (f = k->followers; f; f = f->next) {
    GET_EXP_MULT(f->follower) = mult;
  }

  GET_EXP_MULT(k) = mult;

}


int stat_bonus(struct char_data *ch) {
    return 0;
}
