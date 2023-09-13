/* ************************************************************************
*   File: act.social.c                                  Part of CircleMUD *
*  Usage: Functions to handle socials                                     *
*                                                                         *
*  All rights reserved.  See license.doc for complete information.        *
*                                                                         *
*  Copyright (C) 1993, 94 by the Trustees of the Johns Hopkins University *
*  CircleMUD is based on DikuMUD, Copyright (C) 1990, 1991.               *
************************************************************************ */

#include "conf.h"
#include "sysdep.h"


#include "structs.h"
#include "utils.h"
#include "comm.h"
#include "interpreter.h"
#include "handler.h"
#include "db.h"
#include "spells.h"

/* extern variables */
extern struct room_data *world;
extern struct descriptor_data *descriptor_list;

/* local globals */
static int list_top = -1;

/* local functions */
char *fread_action(FILE * fl, int nr);
int find_action(int cmd);
ACMD(do_action);
ACMD(do_insult);
void boot_social_messages(void);
void create_command_list(void);
void free_social_messages(void);
void free_command_list(void);


struct social_messg 
{
  char *command;            /* Holds a copy of activating command */
  int act_nr;
  int hide;
  int min_victim_position;	/* Position of victim */
  int min_char_position;    /* Position of character */
  int min_level_char;       /* Minimum level of socialing char */
  
  char *sort_as;            /* The abbreviation used to sort the
                               command in the command list */

  /* No argument was supplied */
  char *char_no_arg;
  char *others_no_arg;

  /* An argument was there, and a victim was found */
  char *char_found;		/* if NULL, read no further, ignore args */
  char *others_found;
  char *vict_found;

  /* An argument was there, but no victim was found */
  char *not_found;

  /* The victim turned out to be the character */
  char *char_auto;
  char *others_auto;
} *soc_mess_list = NULL;

int find_action(int cmd)
{
  int bot, top, mid;

  bot = 0;
  top = list_top;

  if (top < 0)
    return (-1);

  for (;;) {
    mid = (bot + top) / 2;

    if (soc_mess_list[mid].act_nr == cmd)
      return (mid);
    if (bot >= top)
      return (-1);

    if (soc_mess_list[mid].act_nr > cmd)
      top = --mid;
    else
      bot = ++mid;
  }
}

/*  PDH 11/24/98 - intro system  */
int pop_off_intro(struct char_data * vict, struct char_data * who)
{
  int  i, j, found = -1;

  if (known_player(vict, who))
  {
    for (i = 0; i < MAX_INTROS; i++) 
    {
      GET_NAME(who, chname);

      if ((str_cmp(GET_INTRO(vict, i), chname) == 0) && (GET_INTRONUM(vict, i) == GET_MAX_MANA(who)))
      {
    	found = 1;

	    for (j = i + 1; j < MAX_INTROS && strlen(GET_INTRO(vict, j - 1)); j++)
        {
          strcpy(GET_INTRO(vict, j - 1), GET_INTRO(vict, j));
          GET_INTRONUM(vict, j - 1) = GET_INTRONUM(vict, j);
	    }

	    if ( j == MAX_INTROS ) 
        {
	      strcpy(GET_INTRO(vict, MAX_INTROS-1), "");
          GET_INTRONUM(vict, MAX_INTROS - 1) = 0;
	    }
     }

      FREE_NAME(chname);
    }

    if ( found == -1 ) 
    {
      log("SYSERR: pop_off_intro: unable to find name on player's intro list");
    }

    return 1;
  }

  return 0;
}

void add_to_intro(struct char_data * vict, struct char_data * ch)
{
  int i;

  for (i = 0; i < MAX_INTROS; i++)
  {
    if (strlen(GET_INTRO(vict, i)) == 0)
    {
      GET_NAME(ch, chname);
      strcpy(GET_INTRO(vict, i), chname);
      FREE_NAME(chname);
      GET_INTRONUM(vict, i) = GET_MAX_MANA(ch);
      return;
    }
  }
  /*  no empty slots, so force one  */
  for (i = 0; i < MAX_INTROS - 1; i++)
  {
    strcpy(GET_INTRO(vict, i), GET_INTRO(vict, i + 1));
    GET_INTRONUM(vict, i) = GET_INTRONUM(vict, i + 1);
  }

  GET_NAME(ch, chname);
  strcpy(GET_INTRO(vict, MAX_INTROS - 1), chname);
  FREE_NAME(chname);
  GET_INTRONUM(vict, MAX_INTROS - 1) = GET_MAX_MANA(ch);

  return;
}

void act_greet(struct char_data * ch, struct char_data * vict)
{
  if (pop_off_intro(vict, ch))
    add_to_intro(vict, ch);

  return;
}

void act_intro(struct char_data * ch, struct char_data * vict)
{
  if (!PRF_FLAGGED(vict, PRF_INTRO))
    return;

  if (!known_player(vict, ch))
    add_to_intro(vict, ch);
  else
    act_greet(vict, ch);

  return;
}

ACMD(do_action)
{
  int act_nr;
  struct social_messg *action;
  struct char_data *vict;

  if ((act_nr = find_action(cmd)) < 0) {
    send_to_char("That action is not supported.\r\n", ch);
    return;
  }
  action = &soc_mess_list[act_nr];

  if (action->char_found && argument)
    one_argument(argument, buf);
  else
    *buf = '\0';

  if (!*buf) {
    /*  PDH 11/24/98 - intro system  */
    if ( subcmd == SCMD_INTRO ) {
      /*  toggle the INTRO flag  */
      ACMD(do_gen_tog);

      do_gen_tog(ch, argument, cmd, subcmd);
    } else {
      send_to_char(action->char_no_arg, ch);
      send_to_char("\r\n", ch);
      act(action->others_no_arg, action->hide, ch, 0, 0, TO_ROOM);
    }
    return;
  }

  if (!(vict = get_char_vis(ch, buf, FIND_CHAR_ROOM))) {
    send_to_char(action->not_found, ch);
    send_to_char("\r\n", ch);
  } else if (vict == ch) {
    send_to_char(action->char_auto, ch);
    send_to_char("\r\n", ch);
    act(action->others_auto, action->hide, ch, 0, 0, TO_ROOM);
  } else {
    if (GET_POS(vict) < action->min_victim_position)
      act("$N is not in a proper position for that.",
	  FALSE, ch, 0, vict, TO_CHAR | TO_SLEEP);
    else {
      /*  PDH 11/24/98 - intro system  */
      if ( GET_LEVEL(ch) < LVL_IMMORT ) {
	if ( subcmd == SCMD_GREET ) {
	  act_greet(ch, vict);
	}
        if (subcmd == SCMD_INTRO) {
	  act_intro(ch, vict);
	}
      }

      act(action->char_found, 0, ch, 0, vict, TO_CHAR | TO_SLEEP);
      act(action->others_found, action->hide, ch, 0, vict, TO_NOTVICT);
      act(action->vict_found, action->hide, ch, 0, vict, TO_VICT);
    }
  }
}

ACMD(do_insult)
{
  struct char_data *victim;

  one_argument(argument, arg);

  if (*arg) {
    if (!(victim = get_char_vis(ch, arg, FIND_CHAR_ROOM)))
      send_to_char("Can't hear you!\r\n", ch);
    else {
      if (victim != ch) {
        CHOOSE_NAME_II(victim, ch, chname);       
	sprintf(buf, "You insult %s.\r\n", chname);
        FREE_NAME(chname);
	send_to_char(buf, ch);

	switch (number(0, 2)) {
	case 0:
	  if (GET_SEX(ch) == SEX_MALE) {
	    if (GET_SEX(victim) == SEX_MALE)
	      act("$n accuses you of fighting like a woman!", FALSE, ch, 0, victim, TO_VICT);
	    else
	      act("$n says that women can't fight.", FALSE, ch, 0, victim, TO_VICT);
	  } else {		/* Ch == Woman */
	    if (GET_SEX(victim) == SEX_MALE)
	      act("$n accuses you of having the smallest... (brain?)",
		  FALSE, ch, 0, victim, TO_VICT);
	    else
	      act("$n tells you that you'd lose a beauty contest against a troll.",
		  FALSE, ch, 0, victim, TO_VICT);
	  }
	  break;
	case 1:
	  act("$n calls your mother a bitch!", FALSE, ch, 0, victim, TO_VICT);
	  break;
	default:
	  act("$n tells you to get lost!", FALSE, ch, 0, victim, TO_VICT);
	  break;
	}			/* end switch */

	act("$n insults $N.", TRUE, ch, 0, victim, TO_NOTVICT);
      } else {			/* ch == victim */
	send_to_char("You feel insulted.\r\n", ch);
      }
    }
  } else
    send_to_char("I'm sure you don't want to insult *everybody*...\r\n", ch);
}

char *fread_action(FILE * fl, int nr)
{
  char buf[MAX_STRING_LENGTH];

  fgets(buf, MAX_STRING_LENGTH, fl);
  if (feof(fl)) {
    fprintf(stderr, "SYSERR: fread_action: unexpected EOF near action #%d", nr);
    exit(1);
  }

  if (*buf == '#')
    return (NULL);

  buf[strlen(buf) - 1] = '\0';
  return(str_dup(buf));

#if GMB
  else {
    *(buf + strlen(buf) - 1) = '\0';
    CREATE(rslt, char, strlen(buf) + 1);
    strcpy(rslt, buf);
    return (rslt);
  }
#endif
}

void boot_social_messages(void)
{
  FILE *fl;
  int nr, i, hide, min_pos, curr_soc = -1;
  char next_soc[100];
  struct social_messg temp;
  extern struct command_info cmd_info[];

  /* open social file */
  if (!(fl = fopen(SOCMESS_FILE, "r"))) {
    sprintf(buf, "SYSERR: can't open socials file '%s'", SOCMESS_FILE);
    perror(buf);
    exit(1);
  }
  /* count socials & allocate space */
  for (nr = 0; *cmd_info[nr].command != '\n'; nr++)
    if (cmd_info[nr].command_pointer == do_action)
      list_top++;

  CREATE(soc_mess_list, struct social_messg, list_top + 1);

  /* now read 'em */
  for (;;) {
    fscanf(fl, " %s ", next_soc);
    if (*next_soc == '$')
      break;

#if GMB 
    if ((nr = find_command(next_soc)) < 0) {
      sprintf(buf, "Unknown social '%s' in social file", next_soc);
      log(buf);
    }
#endif

    if (fscanf(fl, " %d %d \n", &hide, &min_pos) != 2) {
      fprintf(stderr, "SYSERR: format error in social file near social '%s'\n",
	      next_soc);
      exit(1);
    }
    /* read the stuff */
    if (++curr_soc > list_top)
    {
      fprintf(stderr, "SYSERR: ran out of slots in social array (%d > %d)\n",
        curr_soc, list_top);
      break;
    }

    /* read the stuff */
    soc_mess_list[curr_soc].act_nr = nr = find_command(next_soc);
    soc_mess_list[curr_soc].hide = hide;
    soc_mess_list[curr_soc].min_victim_position = min_pos;

    soc_mess_list[curr_soc].char_no_arg = fread_action(fl, nr);
    soc_mess_list[curr_soc].others_no_arg = fread_action(fl, nr);
    soc_mess_list[curr_soc].char_found = fread_action(fl, nr);

    /* if no char_found, the rest is to be ignored */
    if (!soc_mess_list[curr_soc].char_found)
      continue;

    soc_mess_list[curr_soc].others_found = fread_action(fl, nr);
    soc_mess_list[curr_soc].vict_found = fread_action(fl, nr);
    soc_mess_list[curr_soc].not_found = fread_action(fl, nr);
    soc_mess_list[curr_soc].char_auto = fread_action(fl, nr);
    soc_mess_list[curr_soc].others_auto = fread_action(fl, nr);

    /* If social not fount, re-use this slot. */
    if (nr < 0) {
      fprintf(stderr, "SYSERR: Unknown social '%s' in social file.", next_soc);
      memset(&soc_mess_list[curr_soc--], 0, sizeof(struct social_messg));
      continue;
    }

    /* If command is not do_action, it is a command. */
    if (cmd_info[nr].command_pointer != do_action) {
      fprintf(stderr, "SYSERR: Social '%s' assigned to a command.", next_soc);
      memset(&soc_mess_list[curr_soc--], 0, sizeof(struct social_messg));
    }
  }

  /* close file & set top */
  fclose(fl);
  list_top = curr_soc;

  /* now, sort 'em */
  for (curr_soc = 0; curr_soc < list_top; curr_soc++) {
    min_pos = curr_soc;
    for (i = curr_soc + 1; i <= list_top; i++)
      if (soc_mess_list[i].act_nr < soc_mess_list[min_pos].act_nr)
	min_pos = i;
    if (curr_soc != min_pos) {
      temp = soc_mess_list[curr_soc];
      soc_mess_list[curr_soc] = soc_mess_list[min_pos];
      soc_mess_list[min_pos] = temp;
    }
  }
}

void create_command_list(void)
{
    int i, j, k;
    struct social_messg temp;

    log("Pre-Struct");
    extern struct command_info cmd_info[];
    extern struct command_info *complete_cmd_info;
    log("Post Struct");

    // Free up the old command list.
    log("Pre-cmd_info");
    if(complete_cmd_info) {
        free_command_list();
    }
    log("Post-cmd_info");

    // Recheck the sort on socials.
    log("Sort socials");
    for(j = 0; j < list_top; j++) {
        k = j;
        
        for(i = j + 1; i <= list_top; i++) {
            if(str_cmp(soc_mess_list[i].sort_as, soc_mess_list[k].sort_as) < 0) {
                k = i;
            }
        }

        if(j != k) {
            temp = soc_mess_list[j];
            soc_mess_list[j] = soc_mess_list[k];
            soc_mess_list[k] = temp;
        }
    }
    log("Post sort socials");

    // Count the commands in the command list.
    i = 0;

    while(*cmd_info[i].command != '\n') {
        i++;
    }
    i++;

    CREATE(complete_cmd_info, struct command_info, list_top + i + 2);

    // Now we'll sort the socials and commands together as one list.
    i = 0;
    j = 0;
    k = 0;

    log("While");
    log("List top: %d", list_top);
    while((*cmd_info[i].command != '\n') || (j <= list_top)) {
        log("J: %d", j);
//        log("End of line? %d", *cmd_info[i].command != '\n');
//        log("J <= List top? %d", j <= list_top);
//        log("CMD.command: %s", cmd_info[i].command);
        //log("CMD sort_as: %s", cmd_info[i]);
        //log("Soc sort_as: %s", soc_mess_list[j].sort_as);
        if((i < RESERVE_CMDS) || (j > list_top) || (str_cmp(cmd_info[i].sort_as, soc_mess_list[j].sort_as) < 1))
        {
//            log("if");
            complete_cmd_info[k++] = cmd_info[i++];
        }
        else
        {
//            log("else");
            soc_mess_list[j].act_nr = k;
            complete_cmd_info[k].command            = soc_mess_list[j].command;
            complete_cmd_info[k].sort_as            = soc_mess_list[j].sort_as;
            complete_cmd_info[k].minimum_position   = soc_mess_list[j].min_char_position;
            complete_cmd_info[k].command_pointer    = do_action;
            complete_cmd_info[k].minimum_level      = soc_mess_list[j++].min_level_char;
            complete_cmd_info[k++].subcmd           = 0;
        }
    }

    log("Post-while");
    complete_cmd_info[k].command            = strdup("\n");
    complete_cmd_info[k].sort_as            = strdup("zzzzzzz");
    complete_cmd_info[k].minimum_position   = 0;
    complete_cmd_info[k].command_pointer    = 0;
    complete_cmd_info[k].minimum_level      = 0;
    complete_cmd_info[k++].subcmd           = 0;
    log("Command info rebuilt, %d total commands.", k);
}   // End "create_command_list".

void free_social_messages(void)
{
}

void free_command_list(void)
{
    int i;

    for(i = 0; *complete_cmd_info[i].command != '\n'; i++);

    free((char *)complete_cmd_info[i].command);
    free((char *)complete_cmd_info[i].sort_as);
    free(complete_cmd_info);
    complete_cmd_info = NULL;
}
