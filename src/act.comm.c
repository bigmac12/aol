/* ************************************************************************
*   File: act.comm.c                                    Part of CircleMUD *
*  Usage: Player-level communication commands                             *
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
#include "screen.h"

#include "dg_scripts.h"
#include "spells.h"

/* extern variables */
extern int level_can_shout;
extern int holler_move_cost;
extern struct room_data *world;
extern struct char_data *character_list;

extern struct descriptor_data *descriptor_list;
extern struct same_zone shout_zone[];
extern struct zone_data *zone_table;

/* local functions */
void perform_tell(struct char_data* ch, struct char_data* vict, char* arg);
int is_tell_ok(struct char_data* ch, struct char_data* vict);

ACMD(do_say);
ACMD(do_gsay);
ACMD(do_tell);
ACMD(do_reply);
ACMD(do_spec_comm);
ACMD(do_write);
ACMD(do_page);
ACMD(do_gen_comm);
ACMD(do_qcomm);
ACMD(do_prayer);
ACMD(do_newbie);

void garble_text(char *string, int known, int lang)
{
  char letters[50] = "";
  int i;

  switch (lang) {
  default:
    strcpy (letters, "aehiopstuwxyz");
    break;
  }

  for (i = 0; i < (int) strlen(string); ++i) {
    if (isalpha(string[i]) && (!known)) {
      string[i] = letters[dice(1, (int) strlen(letters)) - 1];
    }
  }

}

ACMD(do_sayto)
{
//  struct char_data *target;
//  struct char_data *next_target;

  char sayString[MAX_STRING_LENGTH];
  int emoteBegun = FALSE;
  int emoteDone = FALSE;
  int i = 0;
  struct char_data *vict;
  char buf2[MAX_STRING_LENGTH];
  char targ[MAX_STRING_LENGTH];
  char others[MAX_STRING_LENGTH];
  char self[MAX_STRING_LENGTH];
  char target[MAX_STRING_LENGTH];
  char sEmote[MAX_STRING_LENGTH];
  char tEmote[MAX_STRING_LENGTH];
  char oEmote[MAX_STRING_LENGTH];  
  char emote[MAX_STRING_LENGTH];

  strcpy(sEmote, "");
  strcpy(tEmote, "");
  strcpy(oEmote, "");  
  strcpy(sayString, "");
  strcpy(targ, "");
  strcpy(buf2, "");
  strcpy(buf, "");
  strcpy(others, "");  
  strcpy(self, "");
  strcpy(target, "");
  strcpy(emote, "");
  
  delete_doubledollar(argument);  
  half_chop(argument, targ, buf);

  if (!targ) {
    send_to_char("You must supply a target to speak to.\r\n", ch);
	return;
  }
  
  if (!(vict = get_char_vis(ch, targ, FIND_CHAR_ROOM))) {
    send_to_char("They aren't here.\r\n", ch);
	return;
  }

  // skip_spaces(&buf);
  
  if (affected_by_spell(ch, SPELL_SILENCE) ||
       affected_by_spell(ch, SPELL_SPHERE_SILENCE)) {
    send_to_char("You find your throat too tight to speak.\r\n", ch);
    return;
  }

  if (IS_AFFECTED(ch, AFF_JAWSTRIKE)) {
     send_to_char("Your swollen jaw makes you temporarily speechless.\r\n", ch);
    return;
  }


  if (!*buf) {
    send_to_char("Syntax: 'sayto *optional emote* string which you want to say'.\r\n", ch);
  } else {
  
    for (i = 0; i < strlen(buf); i++) {
	  if (buf[i] == '*') {
	    if (!emoteBegun && !emoteDone) {
		  emoteBegun = TRUE;
		  continue;
		}
		else if (emoteBegun && !emoteDone) {
		  emoteDone = TRUE;
		  i++;
		  continue;
		}
	  }
	  if (emoteBegun && !emoteDone) {
	    if (!emote)
		  sprintf(emote, "%c", buf[i]);
	    else
		  sprintf(emote, "%s%c", emote ? emote : "", buf[i]);
	  }
	  else if ((emoteBegun && emoteDone) || (!emoteBegun && !emoteDone)) {
	    if (!sayString)
	      sprintf(sayString, "%c", buf[i]);
	    else
		  sprintf(sayString, "%s%c", sayString ? sayString : "", buf[i]);
	  }
	}
	
	if ((strlen(emote) > 1) && (strlen(sayString) < 2)) {
    send_to_char("Syntax: 'sayto *optional emote* string which you want to say'.\r\n", ch);
	return;
	}	
	
	if (strlen(emote) > 1) {

	    for (i = 0; i < strlen(emote); i++) {
	  
		    if (emote[i] == '$') {
	
				switch (emote[i + 1]) {
					
					case 'e':
					  if (GET_SEX(ch) == SEX_MALE) {
						strcpy(self, "you");
						strcpy(target, "he");
						strcpy(others, "he");						
					  }
					  else {
						strcpy(self, "you");
						strcpy(target, "he");
						strcpy(others, "she");	
					  }			  
					  break;
					
					case 'E':
					  if (GET_SEX(vict) == SEX_MALE) {
						strcpy(self, "he");
						strcpy(target, "you");
						strcpy(others, "he");	
					  }
					  else {
						strcpy(self, "she");
						strcpy(target, "you");
						strcpy(others, "she");	
					  }
					  break;
					  
					case 's':
					  if (GET_SEX(ch) == SEX_MALE) {
						strcpy(self, "your");
						strcpy(target, "his");
						strcpy(others, "his");	
					  }
					  else {
						strcpy(self, "your");
						strcpy(target, "her");
						strcpy(others, "her");	
					  }		  
					  break;
					
					case 'S':
					  if (GET_SEX(vict) == SEX_MALE) {
						strcpy(self, "his");
						strcpy(target, "your");
						strcpy(others, "his");	
					  }
					  else {
						strcpy(self, "her");
						strcpy(target, "your");
						strcpy(others, "her");	
					  }
					  break;
					
					case 'm':
					  if (GET_SEX(ch) == SEX_MALE) {
						strcpy(self, "yourself");
						strcpy(target, "him");
						strcpy(others, "him");	
					  }
					  else {
						strcpy(self, "yourself");
						strcpy(target, "her");
						strcpy(others, "her");	
					  }		  
					  break;
					
					case 'M':
					  if (GET_SEX(vict) == SEX_MALE) {
						strcpy(self, "him");
						strcpy(target, "yourself");
						strcpy(others, "him");	
					  }
					  else {
						strcpy(self, "her");
						strcpy(target, "yourself");
						strcpy(others, "her");	
					  }		  
					  break;
					
					default:
					  continue;
				}
				  
				sprintf(oEmote, "%s%s", oEmote, others);
				sprintf(sEmote, "%s%s", sEmote, self);
				sprintf(tEmote, "%s%s", tEmote, target);

				i++;
			}
			else {
				sprintf(oEmote, "%s%c", oEmote, emote[i]);
				sprintf(sEmote, "%s%c", sEmote, emote[i]);
				sprintf(tEmote, "%s%c", tEmote, emote[i]);
			}
        }
		oEmote[0] = toupper(oEmote[0]);	
		sprintf(oEmote, "%s, ", oEmote);
		sEmote[0] = toupper(sEmote[0]);	
		sprintf(sEmote, "%s, ", sEmote);
		tEmote[0] = toupper(tEmote[0]);	
		sprintf(tEmote, "%s, ", tEmote);		
	}
	
	if (sayString)
	  sayString[strlen(sayString)] = '\0';

    char* say = "";

    if (subcmd == SCMD_OOCSAY)
    {
      if(ROOM_FLAGGED(ch->in_room, ROOM_NO_OOC))
      {
         send_to_char("You may not break character here.\r\n", ch);
         return;
      }
      say = " out of character";
    }
	if (ch == vict) {
	
	  sprintf(buf2, "&W%s$n &Gsays%s to $mself, &W'&C%s&W'.&n", 
	        oEmote ? oEmote : "", say ? say : "", sayString ? sayString : "");	
      act(buf2, FALSE, ch, 0, vict, TO_ROOM|DG_NO_TRIG);

      if (!IS_NPC(ch) && PRF_FLAGGED(ch, PRF_NOREPEAT)) {
        send_to_char(OK, ch);
      } else {	 
 
          REMOVE_BIT(AFF_FLAGS(ch), AFF_HIDE | AFF_SNEAK);

	    sprintf(buf2, "&W%sYou &Gsay%s to yourself, &W'&C%s&W'.&n", 
	        sEmote ? sEmote : "", say ? say : "", sayString ? sayString : "");	
        act(buf2, FALSE, ch, 0, vict, TO_CHAR|DG_NO_TRIG);	  
	  }  
	}
	else {
	
	  sprintf(buf2, "&W%s$n &Gsays%s to $N, &W'&C%s&W'.&n", 
	        oEmote ? oEmote : "", say ? say : "", sayString ? sayString : "");
      act(buf2, FALSE, ch, 0, vict, TO_NOTVICT|DG_NO_TRIG);
	  
	  sprintf(buf2, "&W%s$n &Gsays%s to you, &W'&C%s&W'.&n", 
	        tEmote ? tEmote : "", say ? say : "", sayString ? sayString : "");
      act(buf2, FALSE, ch, 0, vict, TO_VICT|DG_NO_TRIG);
	  
      if (!IS_NPC(ch) && PRF_FLAGGED(ch, PRF_NOREPEAT)) {
        send_to_char(OK, ch);
      } else {	  
	    sprintf(buf2, "&W%sYou &Gsay%s to $N, &W'&C%s&W'.&n", 
	        sEmote ? sEmote : "", say ? say : "", sayString ? sayString : "");
        act(buf2, FALSE, ch, 0, vict, TO_CHAR|DG_NO_TRIG);	  
	  }
	}
/* This is broken. Don't know why yet (wrote it drunk) --gan
    if(subcmd == SCMD_OOCSAY)
    {
	
	for(target = world[ch->in_room].people; target; target = next_target)
	{
		
		next_target = target->next_in_room;
		if(target == NULL)
			return;

		if(target == ch)
			continue;

		send_to_char(buf, next_target);
        }
    }
    else
    {
*/
//    }
  }


  /* trigger check */
  speech_mtrigger(ch, buf2);
  speech_wtrigger(ch, buf2);
}

ACMD(do_say)
{
//  struct char_data *target;
//  struct char_data *next_target;

  char sayString[MAX_STRING_LENGTH];
  int emoteBegun = FALSE;
  int emoteDone = FALSE;
  int i = 0;
  char buf2[MAX_STRING_LENGTH];
  char targ[MAX_STRING_LENGTH];
  char others[MAX_STRING_LENGTH];
  char self[MAX_STRING_LENGTH];
  char sEmote[MAX_STRING_LENGTH];
  char oEmote[MAX_STRING_LENGTH];  
  char emote[MAX_STRING_LENGTH];

  strcpy(sEmote, "");
  strcpy(oEmote, "");  
  strcpy(sayString, "");
  strcpy(targ, "");
  strcpy(buf2, "");
  strcpy(others, "");  
  strcpy(self, "");
  strcpy(emote, "");

  delete_doubledollar(argument);  
  skip_spaces(&argument);
  
  if (affected_by_spell(ch, SPELL_SILENCE) ||
       affected_by_spell(ch, SPELL_SPHERE_SILENCE)) {
    send_to_char("You find your throat too tight to speak.\r\n", ch);
    return;
  }

  if (IS_AFFECTED(ch, AFF_JAWSTRIKE)) {
     send_to_char("Your swollen jaw makes you temporarily speechless.\r\n", ch);
    return;
  }


  if (!*argument) {
    send_to_char("Syntax: 'say *optional emote* string which you want to say'.\r\n", ch);
  } else {
  
    for (i = 0; i < strlen(argument); i++) {
	  if (argument[i] == '*') {
	    if (!emoteBegun && !emoteDone) {
		  emoteBegun = TRUE;
		  continue;
		}
		else if (emoteBegun && !emoteDone) {
		  emoteDone = TRUE;
		  i++;
		  continue;
		}
	  }
	  if (emoteBegun && !emoteDone) {
	    if (!emote)
		  sprintf(emote, "%c", argument[i]);
	    else
		  sprintf(emote, "%s%c", emote ? emote : "", argument[i]);
	  }
	  else if ((emoteBegun && emoteDone) || (!emoteBegun && !emoteDone)) {
	    if (!sayString)
	      sprintf(sayString, "%c", argument[i]);
	    else
		  sprintf(sayString, "%s%c", sayString ? sayString : "", argument[i]);
	  }
	}
	
	if ((strlen(emote) > 1) && (strlen(sayString) < 2)) {
    send_to_char("Syntax: 'say *optional emote* string which you want to say'.\r\n", ch);
	return;
	}
	
	if (strlen(emote) > 1) {

	
	    for (i = 0; i < strlen(emote); i++) {
	  
		    if (emote[i] == '$') {
	
				switch (emote[i + 1]) {
					
					case 'e':
					  if (GET_SEX(ch) == SEX_MALE) {
						strcpy(self, "you");
						strcpy(others, "he");						
					  }
					  else {
						strcpy(self, "you");
						strcpy(others, "she");	
					  }			  
					  break;
					
					case 's':
					  if (GET_SEX(ch) == SEX_MALE) {
						strcpy(self, "your");
						strcpy(others, "his");	
					  }
					  else {
						strcpy(self, "your");
						strcpy(others, "her");	
					  }		  
					  break;
										
					case 'm':
					  if (GET_SEX(ch) == SEX_MALE) {
						strcpy(self, "yourself");
						strcpy(others, "him");	
					  }
					  else {
						strcpy(self, "yourself");
						strcpy(others, "her");	
					  }		  
					  break;
					
					default:
					  continue;
				}
				  
				sprintf(oEmote, "%s%s", oEmote, others);
				sprintf(sEmote, "%s%s", sEmote, self);

				i++;
			}
			else {
				sprintf(oEmote, "%s%c", oEmote, emote[i]);
				sprintf(sEmote, "%s%c", sEmote, emote[i]);
			}	  
        }
		oEmote[0] = toupper(oEmote[0]);	
		sprintf(oEmote, "%s, ", oEmote);
		sEmote[0] = toupper(sEmote[0]);	
		sprintf(sEmote, "%s, ", sEmote);
	
	}
	if (sayString)
	  sayString[strlen(sayString)] = '\0';  

    char* say = "";

    if (subcmd == SCMD_OOCSAY)
    {
      if(ROOM_FLAGGED(ch->in_room, ROOM_NO_OOC))
      {
         send_to_char("You may not break character here.\r\n", ch);
         return;
      }
      say = " out of character";
    }
	
	sprintf(buf2, "&W%s$n &Gsays%s, &W'&C%s&W'.&n", 
	        oEmote ? oEmote : "", say ? say : "", sayString ? sayString : "");
    act(buf2, FALSE, ch, 0, 0, TO_ROOM|DG_NO_TRIG);
	  
    if (!IS_NPC(ch) && PRF_FLAGGED(ch, PRF_NOREPEAT)) {
      send_to_char(OK, ch);
    } else {	 
 
        REMOVE_BIT(AFF_FLAGS(ch), AFF_HIDE | AFF_SNEAK);

	  sprintf(buf2, "&W%sYou &Gsay%s, &W'&C%s&W'.&n", 
	        sEmote ? sEmote : "", say ? say : "", sayString ? sayString : "");
      act(buf2, FALSE, ch, 0, 0, TO_CHAR|DG_NO_TRIG);	  
	}
/* This is broken. Don't know why yet (wrote it drunk) --gan
    if(subcmd == SCMD_OOCSAY)
    {
	for(target = world[ch->in_room].people; target; target = next_target)
	{
		
		next_target = target->next_in_room;
		if(target == NULL)
			return;

		if(target == ch)
			continue;

		send_to_char(buf, next_target);
        }
    }
    else
    {
*/
//    }
  }

  /* trigger check */

  speech_mtrigger(ch, argument);
  speech_wtrigger(ch, argument);
}


ACMD(do_gsay)
{
  struct char_data *k = (struct char_data*) NULL;
  struct follow_type *f = (struct follow_type*) NULL;

  skip_spaces(&argument);

  if (!IS_AFFECTED(ch, AFF_GROUP)) {
    send_to_char("But you are not the member of a group!\r\n", ch);
    return;
  }

  if (affected_by_spell(ch, SPELL_SILENCE) ||
      affected_by_spell(ch, SPELL_SPHERE_SILENCE)) {
    send_to_char("You find your throat too tight to speak.\r\n", ch);
    return;
  }

  if (!*argument)
    send_to_char("Yes, but WHAT do you want to group-say?\r\n", ch);
  else {
    if (ch->master)
      k = ch->master;
    else
      k = ch;

    sprintf(buf, "&W$n &Rtells &Gthe group&W, '&C%s&W'&n", argument);

    if (IS_AFFECTED(k, AFF_GROUP) && (k != ch))
    {
      if (!affected_by_spell(k, SPELL_DEAFENING_WIND))
      {
        act(buf, FALSE, ch, 0, k, TO_VICT | TO_SLEEP);
      }
      else
      {
        act("$n tells the group something, but you do not hear it.",
                FALSE, ch, 0, k, TO_VICT | TO_SLEEP);
      }
    }

    for (f = k->followers; f; f = f->next)
      if (IS_AFFECTED(f->follower, AFF_GROUP) && (f->follower != ch))
	act(buf, FALSE, ch, 0, f->follower, TO_VICT | TO_SLEEP);

    if (!IS_NPC(ch) && PRF_FLAGGED(ch, PRF_NOREPEAT)) {
      send_to_char(OK, ch);
    } else {
      sprintf(buf, "&WYou &Rtell &Gthe group&W, '&C%s'&n\r\n", argument);
      send_to_char(buf, ch);
    }
  }
}


void perform_tell(struct char_data *ch, struct char_data *vict, char *arg)
{
  send_to_char(CCRED(vict, C_NRM), vict);
  sprintf(buf, "&R$n &rtells &Wyou, '&C%s&W'&n", arg);
  act(buf, FALSE, ch, 0, vict, TO_VICT | TO_SLEEP);
  send_to_char(CCNRM(vict, C_NRM), vict);

  if (!IS_NPC(ch) && PRF_FLAGGED(ch, PRF_NOREPEAT))
    send_to_char(OK, ch);
  else {
    send_to_char(CCRED(ch, C_CMP), ch);
    sprintf(buf, "&WYou &Rtell &Y$N&W, '&C%s&W'&n", arg);
    act(buf, FALSE, ch, 0, vict, TO_CHAR | TO_SLEEP);
    send_to_char(CCNRM(ch, C_CMP), ch);
  }

  if (!IS_NPC(vict) && !IS_NPC(ch))
    GET_LAST_TELL(vict) = GET_IDNUM(ch);
}

int is_tell_ok(struct char_data *ch, struct char_data *vict)
{
  if (GET_POS(ch) == POS_SLEEPING)
  {
    send_to_char("While asleep?\r\n", ch);
  }
  else if (ch == vict)
  {
    send_to_char("You try to tell yourself something.\r\n", ch);
  }
  else if (!IS_NPC(ch) && PRF_FLAGGED(ch, PRF_NOTELL))
  {
    send_to_char("You can't communicate with that person right now.\r\n", ch);
  }
  else if (!IS_NPC(ch) && !IS_NPC(vict) && 
    GET_LEVEL(ch) < LVL_IMMORT && GET_LEVEL(vict) < LVL_IMMORT &&
    !known_player(ch, vict))
  {
    send_to_char(NOPERSON, ch);
  }
  else if (!IS_NPC(ch) && PRF_FLAGGED(vict, PRF_AFK))
  {
    act("$E's away from keyboard right now, please try again later.",
      FALSE, ch, 0, vict, TO_CHAR | TO_SLEEP);
  }
  else if (affected_by_spell(ch, SPELL_SILENCE) ||
            affected_by_spell(ch, SPELL_SPHERE_SILENCE)) 
  {
    send_to_char("You find your throat too tight to speak.\r\n", ch);
  }
  else if (ROOM_FLAGGED(ch->in_room, ROOM_SOUNDPROOF))
  {
    send_to_char("The walls seem to absorb your words.\r\n", ch);
  }
  else if (!IS_NPC(vict) && !vict->desc)        /* linkless */
  {
    act("$E's linkless at the moment.", FALSE, ch, 0, vict, TO_CHAR | TO_SLEEP);
  }
  else if (PLR_FLAGGED(vict, PLR_WRITING))
  {
    act("$E's writing a message right now; try again later.", FALSE, ch, 0, 
      vict, TO_CHAR | TO_SLEEP);
  }
  else if ((!IS_NPC(vict) && PRF_FLAGGED(vict, PRF_NOTELL)) ||
           ROOM_FLAGGED(vict->in_room, ROOM_SOUNDPROOF) ||
           affected_by_spell(vict, SPELL_DEAFENING_WIND))
  {
    act("$E can't hear you.", FALSE, ch, 0, vict, TO_CHAR | TO_SLEEP);
  }
  else
    return (TRUE);

  return (FALSE);
}

/*
 * Yes, do_tell probably could be combined with whisper and ask, but
 * called frequently, and should IMHO be kept as tight as possible.
 */
ACMD(do_tell)
{
  struct char_data *vict = (struct char_data*) NULL;

  half_chop(argument, buf, buf2);

  if (!*buf || !*buf2)
    send_to_char("Who do you wish to tell what??\r\n", ch);
  else if (!(vict = get_char_vis(ch, buf, FIND_CHAR_WORLD)))
    send_to_char(NOPERSON, ch);
  else if (is_tell_ok(ch, vict))
    perform_tell(ch, vict, buf2);
}


ACMD(do_reply)
{
  struct char_data *tch = character_list;

  if (IS_NPC(ch))
    return;

  skip_spaces(&argument);

  if (GET_LAST_TELL(ch) == NOBODY)
  {
    send_to_char("You have no-one to reply to!\r\n", ch);
  }
  else if (!*argument)
  {
    send_to_char("What is your reply?\r\n", ch);
  } 
  else 
  {
    /*
     * Make sure the person you're replying to is still playing by searching
     * for them.  Note, now last tell is stored as player IDnum instead of
     * a pointer, which is much better because it's safer, plus will still
     * work if someone logs out and back in again.
     */

    /*
     * XXX: A descriptor list based search would be faster although
     *      we could not find link dead people.  Not that they can
     *      hear tells anyway. :) -gg 2/24/98
     */

    while (tch != NULL && (IS_NPC(tch) || GET_IDNUM(tch) != GET_LAST_TELL(ch)))
      tch = tch->next;

    if (tch == NULL)
    {
      send_to_char("They are no longer playing.\r\n", ch);
    }
    else if (is_tell_ok(ch, tch))
    {
      perform_tell(ch, tch, argument);
    }
  }
}


ACMD(do_spec_comm)
{
  struct char_data *vict = (struct char_data*) NULL;
  char *action_sing      = (char*) NULL;
  char *action_plur      = (char*) NULL;
  char *action_others    = (char*) NULL;

  if (subcmd == SCMD_WHISPER) {
    action_sing = "whisper to";
    action_plur = "whispers to";
    action_others = "$n whispers something to $N.";
  } else {
    action_sing = "ask";
    action_plur = "asks";
    action_others = "$n asks $N a question.";
  }

  half_chop(argument, buf, buf2);

  if (!*buf || !*buf2) {
    sprintf(buf, "Whom do you want to %s.. and what??\r\n", action_sing);
    send_to_char(buf, ch);
  } else if (!(vict = get_char_vis(ch, buf, FIND_CHAR_ROOM)))
    send_to_char(NOPERSON, ch);
  else if (vict == ch) {
    send_to_char(
      "You can't get your mouth close enough to your ear...\r\n", ch);
  } else if ( affected_by_spell(ch, SPELL_SILENCE) ||
              affected_by_spell(ch, SPELL_SPHERE_SILENCE) ) {
    send_to_char("You find your throat too tight to speak.\r\n", ch);
  } else if ((GET_POS(vict) <= POS_SLEEPING) ||
             (affected_by_spell(vict, SPELL_DEAFENING_WIND)))
  {
    send_to_char("They can't hear you.\r\n", ch);
  } else {
    sprintf(buf, "$n %s you, '%s'", action_plur, buf2);
    act(buf, FALSE, ch, 0, vict, TO_VICT);
    if (!IS_NPC(ch) && PRF_FLAGGED(ch, PRF_NOREPEAT))
      send_to_char(OK, ch);
    else {
      CHOOSE_NAME_II(vict, ch, chname);
      sprintf(buf, "You %s %s, '%s'\r\n", action_sing, chname, buf2);
      FREE_NAME(chname);
      send_to_char(buf, ch);

   }
    act(action_others, FALSE, ch, 0, vict, TO_NOTVICT);
  }
}

#define MAX_NOTE_LENGTH 1000	/* arbitrary */

ACMD(do_write)
{
  struct obj_data *paper = (struct obj_data*) NULL;
  struct obj_data *pen   = (struct obj_data*) NULL;
  char *papername        = (char*) NULL;
  char *penname          = (char*) NULL;

  papername = buf1;
  penname = buf2;

  two_arguments(argument, papername, penname);

  if (!ch->desc)
    return;

  if (!*papername) {            /* nothing was delivered */
    send_to_char(
      "Write?  With what?  ON what?  What are you trying to do?!?\r\n", ch);
    return;
  }
  if (*penname) {               /* there were two arguments */
    if (!(paper = get_obj_in_list_vis(ch, papername, ch->carrying))) {
      sprintf(buf, "You have no %s.\r\n", papername);
      send_to_char(buf, ch);
      return;
    }
    if (!(pen = get_obj_in_list_vis(ch, penname, ch->carrying))) {
      sprintf(buf, "You have no %s.\r\n", penname);
      send_to_char(buf, ch);
      return;
    }
  } else {              /* there was one arg.. let's see what we can find */
    if (!(paper = get_obj_in_list_vis(ch, papername, ch->carrying))) {
      sprintf(buf, "There is no %s in your inventory.\r\n", papername);
      send_to_char(buf, ch);
      return;
    }
    if (GET_OBJ_TYPE(paper) == ITEM_PEN) {      /* oops, a pen.. */
      pen = paper;
      paper = NULL;
    } else if (GET_OBJ_TYPE(paper) != ITEM_NOTE) {
      send_to_char("That thing has nothing to do with writing.\r\n", ch);
      return;
    }
    /* One object was found.. now for the other one. */
    if (!GET_EQ(ch, WEAR_HOLD)) {
      sprintf(buf, "You can't write with %s %s alone.\r\n", AN(papername),
              papername);
      send_to_char(buf, ch);
      return;
    }
    if (!CAN_SEE_OBJ(ch, GET_EQ(ch, WEAR_HOLD))) {
      send_to_char("The stuff in your hand is invisible!  Yeech!!\r\n", ch);
      return;
    }
    if (pen)
      paper = GET_EQ(ch, WEAR_HOLD);
    else
      pen = GET_EQ(ch, WEAR_HOLD);
  }


  /* ok.. now let's see what kind of stuff we've found */
  if (GET_OBJ_TYPE(pen) != ITEM_PEN)
    act("$p is no good for writing with.", FALSE, ch, pen, 0, TO_CHAR);
  else if (GET_OBJ_TYPE(paper) != ITEM_NOTE)
    act("You can't write on $p.", FALSE, ch, paper, 0, TO_CHAR);
  else if (paper->action_description)
    send_to_char("There's something written on it already.\r\n", ch);
  else {
    /* we can write - hooray! */
     /* this is the PERFECT code example of how to set up:
      * a) the text editor with a message already loaed
      * b) the abort buffer if the player aborts the message
      */
     ch->desc->backstr = NULL;
     send_to_char("Write your note.  (/s saves /h for help)\r\n", ch);
     /* ok, here we check for a message ALREADY on the paper */
     if (paper->action_description) {
        /* we str_dup the original text to the descriptors->backstr */
        ch->desc->backstr = str_dup(paper->action_description);
        /* send to the player what was on the paper (cause this is already */
        /* loaded into the editor) */
        send_to_char(paper->action_description, ch);
     }
    act("$n begins to jot down a note.", TRUE, ch, 0, 0, TO_ROOM);
     /* assign the descriptor's->str the value of the pointer to the text */
     /* pointer so that we can reallocate as needed (hopefully that made */
     /* sense :>) */
    ch->desc->str = &paper->action_description;
    ch->desc->max_str = MAX_NOTE_LENGTH;
    SET_BIT(GET_OBJ_EXTRA(paper),ITEM_UNIQUE_SAVE);
	}
}


ACMD(do_page)
{
  struct descriptor_data *d = (struct descriptor_data*) NULL;
  struct char_data *vict = (struct char_data*) NULL;

  half_chop(argument, arg, buf2);

  if (IS_NPC(ch))
    send_to_char("Monsters can't page.. go away.\r\n", ch);
  else if (!*arg)
    send_to_char("Whom do you wish to page?\r\n", ch);
  else {
    GET_NAME(ch, chname);
    sprintf(buf, "\007\007&W*&R%s&W*&C %s&n\r\n", chname, buf2);
    FREE_NAME(chname);
    if (!str_cmp(arg, "all")) {
      if (GET_LEVEL(ch) > LVL_GOD) {
	for (d = descriptor_list; d; d = d->next)
	  if (STATE(d) == CON_PLAYING && d->character)
	    act(buf, FALSE, ch, 0, d->character, TO_VICT);
      } else
	send_to_char("You will never be godly enough to do that!\r\n", ch);
      return;
    }
    if ((vict = get_char_vis(ch, arg, FIND_CHAR_WORLD)) != NULL) {
      act(buf, FALSE, ch, 0, vict, TO_VICT);
      if (!IS_NPC(ch) && PRF_FLAGGED(ch, PRF_NOREPEAT))
	send_to_char(OK, ch);
      else
	act(buf, FALSE, ch, 0, vict, TO_CHAR);
    } else
      send_to_char("There is no such person in the game!\r\n", ch);
  }
}


/**********************************************************************
 * generalized communication func, originally by Fred C. Merkel (Torg) *
  *********************************************************************/

int shout_range(struct char_data * ch, struct char_data * vict)
{
  int czone, tzone, i, j, k;

  czone = world[ch->in_room].zone;
  tzone = world[vict->in_room].zone;

 /*  Let's try a new angle and see what happens - Soli, 10/28/99  */

  czone = zone_table[czone].number;
  tzone = zone_table[tzone].number;

  /* if ((GET_LEVEL(ch) >= LVL_IMMORT) || (GET_LEVEL(vict) >= LVL_IMMORT))
    return 1; */

  if (czone == tzone)
    return 1;

  for (i = 0; shout_zone[i].name; i++)
  {
    for (j = 0; j < SHOUT_AREA_MAX; j++)
      if (shout_zone[i].z[j] == czone)
      {
        for (k = 0; k < SHOUT_AREA_MAX; k++)
        {
          if (shout_zone[i].z[k] == tzone)
            return 1;
        }  // end of k loop, checking for target zone (tzone)

        return 0;
      } // end of if (czone found)
  } // end of i loop, checking for character's zone (czone)

  return 0;
}

ACMD(do_gen_comm)
{
  struct descriptor_data *i;
  char color_on[24];

  /* Array of flags which must _not_ be set in order for comm to be heard */
  static int channels[] = {
    0,
    PRF_DEAF,
    0,
    0,
    0,
    0
  };

  /*
   * com_msgs: [0] Message if you can't perform the action because of noshout
   *           [1] name of the action
   *           [2] message if you're not on the channel
   *           [3] a color string.
   */
  static char *com_msgs[][4] = {
    {"You cannot holler!!\r\n",
      "holler",
      "",
    KYEL},

    {"You cannot shout!!\r\n",
      "shout",
      "Turn off your noshout flag first!\r\n",
    KYEL},

    {"You cannot gossip!!\r\n",
      "gossip",
      "You aren't even on the channel!\r\n",
    KYEL},

    {"You cannot auction!!\r\n",
      "auction",
      "You aren't even on the channel!\r\n",
    KMAG},

    {"You cannot congratulate!\r\n",
      "congrat",
      "You aren't even on the channel!\r\n",
    KGRN}
  };

  /* to keep pets, etc from being ordered to shout */
  if (!ch->desc) {
    return;
  }

  if (PLR_FLAGGED(ch, PLR_NOSHOUT)) {
    send_to_char(com_msgs[subcmd][0], ch);
    return;
  } 

  if (ROOM_FLAGGED(ch->in_room, ROOM_SOUNDPROOF)) {
    send_to_char("The walls seem to absorb your words.\r\n", ch);
    return;
  }

  /* level_can_shout defined in config.c */
  if (GET_LEVEL(ch) < level_can_shout) {
    sprintf(buf1, "You must be at least level %d before you can %s.\r\n",
	    level_can_shout, com_msgs[subcmd][1]);
    send_to_char(buf1, ch);
    return;
  }

  /* make sure the char is on the channel */
  if (!IS_NPC(ch) && PRF_FLAGGED(ch, channels[subcmd])) {
    send_to_char(com_msgs[subcmd][2], ch);
    return;
  }

  /* skip leading spaces */
  skip_spaces(&argument);

  /* make sure that there is something there to say! */
  if (!*argument) {
    sprintf(buf1, "Yes, %s, fine, %s we must, but WHAT???\r\n",
	    com_msgs[subcmd][1], com_msgs[subcmd][1]);
    send_to_char(buf1, ch);
    return;
  }

  if ( affected_by_spell(ch, SPELL_SILENCE) ||
       affected_by_spell(ch, SPELL_SPHERE_SILENCE) ) {
    send_to_char("You find your throat too tight to speak.\r\n", ch);
    return;
  }

  if (GET_LEVEL(ch) < LVL_IMMORT)
  {
    if (subcmd == SCMD_SHOUT) 
    {
      if (GET_MOVE(ch) < holler_move_cost) 
      {
        send_to_char("You're too exhausted to shout anything.\r\n", ch);
        return;
      } 
      else 
      {
        GET_MOVE(ch) -= holler_move_cost;
      }
    }
  }

  /* set up the color on code */
  strcpy(color_on, com_msgs[subcmd][3]);

  /* first, set up strings to be given to the communicator */
  if (!IS_NPC(ch) && PRF_FLAGGED(ch, PRF_NOREPEAT)) {
    send_to_char(OK, ch);
  } else {
    if (COLOR_LEV(ch) >= C_CMP) {
      sprintf(buf1, "%sYou %s, '%s'%s", color_on, com_msgs[subcmd][1],
	      argument, KNRM);
    } else {
      sprintf(buf1, "You %s, '%s'", com_msgs[subcmd][1], argument);
    }

    act(buf1, FALSE, ch, 0, 0, TO_CHAR | TO_SLEEP);
  }

  sprintf(buf, "$n %ss, '%s'", com_msgs[subcmd][1], argument);

  /* now send all the strings out */
  for (i = descriptor_list; i; i = i->next) {
    if (STATE(i) == CON_PLAYING && i != ch->desc && i->character &&
	!PRF_FLAGGED(i->character, channels[subcmd]) &&
	!PLR_FLAGGED(i->character, PLR_WRITING) &&
        !affected_by_spell(i->character, SPELL_DEAFENING_WIND) &&
	!ROOM_FLAGGED(i->character->in_room, ROOM_SOUNDPROOF)) {

      if ((subcmd == SCMD_SHOUT) && (!shout_range(ch, i->character)))
	continue;

      if (COLOR_LEV(i->character) >= C_NRM) {
	send_to_char(color_on, i->character);
      }

      act(buf, FALSE, ch, 0, i->character, TO_VICT | TO_SLEEP);

      if (COLOR_LEV(i->character) >= C_NRM) {
	send_to_char(KNRM, i->character);
      }
    }
  }
}

/*
ACMD(do_qcomm)
{
  struct descriptor_data *i = (struct descriptor_data*) NULL;

  if (IS_NPC(ch) || !PRF_FLAGGED(ch, PRF_QUEST)) {
    send_to_char("You aren't even part of the quest!\r\n", ch);
    return;
  }
  skip_spaces(&argument);

  if (!*argument) {
    sprintf(buf, "%s?  Yes, fine, %s we must, but WHAT??\r\n", CMD_NAME,
	    CMD_NAME);
    CAP(buf);
    send_to_char(buf, ch);
  } else if ( affected_by_spell(ch, SPELL_SILENCE) ||
	      affected_by_spell(ch, SPELL_SPHERE_SILENCE) ) {
    send_to_char("You find your throat too tight to speak.\r\n", ch);
  } else {
    if (!IS_NPC(ch) && PRF_FLAGGED(ch, PRF_NOREPEAT))
      send_to_char(OK, ch);
    else {
      if (subcmd == SCMD_QSAY)
	sprintf(buf, "You quest-say, '%s'", argument);
      else
	strcpy(buf, argument);
      act(buf, FALSE, ch, 0, argument, TO_CHAR);
    }

    if (subcmd == SCMD_QSAY)
      sprintf(buf, "$n quest-says, '%s'", argument);
    else
      strcpy(buf, argument);

    for (i = descriptor_list; i; i = i->next)
      if (STATE(i) == CON_PLAYING && i != ch->desc &&
	  !IS_NPC(ch) && PRF_FLAGGED(i->character, PRF_QUEST))
	act(buf, 0, ch, 0, i->character, TO_VICT | TO_SLEEP);
  }
}
*/

ACMD(do_newbie)
{
	struct descriptor_data *d;

	skip_spaces(&argument);
	delete_doubledollar(argument);

	if(!*argument)
	{
		send_to_char("What would you like to say on the newbie channel?\r\n", ch);
		return;
	}

       if(PLR_FLAGGED(ch, PLR_BADMORT))
       {
                 send_to_char("Your newbie privledges are currently suspended.\r\n", ch);
                 return;
       }

        CHOOSE_NAME(ch, chname);
        sprintf(buf1, "[Newbie] %s : %s\r\n", chname, argument);

        for(d = descriptor_list; d; d = d->next)
        {
         if((STATE(d) == CON_PLAYING) && (!PLR_FLAGGED(d->character, PLR_WRITING | PLR_MAILING)) && 
            (!PRF_FLAGGED(d->character, PRF_NO_NEWBIE)) && (d != ch->desc || !(PRF_FLAGGED(d->character, PRF_NOREPEAT))))
                {
                        send_to_char(CCWHT(d->character, C_NRM), d->character);
                        send_to_char(buf1, d->character);
                        send_to_char(CCNRM(d->character, C_NRM), d->character);
                }
        }
}
}

ACMD(do_chat)
{
        struct descriptor_data *d;

        skip_spaces(&argument);
        delete_doubledollar(argument);

/*
	if(!PLR_FLAGGED(ch, PLR_IMMCHAR) && (GET_LEVEL(ch) < 31))
        {
        	send_to_char("Huh?!\r\n", ch);
        	return;
        }
*/
        if(!*argument)
        {
                send_to_char("What would you like to say on the chat channel?\r\n", ch);
                return;
        }

        CHOOSE_NAME(ch, chname);

        sprintf(buf1, "[Chat] %s : %s\r\n", chname, argument);

        for(d = descriptor_list; d; d = d->next)
        {
		if ((STATE(d) == CON_PLAYING) && (!PRF_FLAGGED(d->character, PRF_NO_CHAT)) && (d != ch->desc || !(PRF_FLAGGED(d->character, PRF_NOREPEAT))))
		{
                        send_to_char(CCWHT(d->character, C_NRM), d->character);
                        send_to_char(buf1, d->character);
                        send_to_char(CCNRM(d->character, C_NRM), d->character);
                }
        }
}
}

ACMD(do_prayer)
{
	struct descriptor_data *d;

	skip_spaces(&argument);
	delete_doubledollar(argument);

	if(!*argument)
	{
		send_to_char("What would you like to pray?\r\n", ch);
		return;
	}

	CHOOSE_NAME(ch, chname);
	sprintf(buf1, "%s (prayer): %s\r\n", chname, argument);

	for(d = descriptor_list; d; d = d->next)
	{
		if ((STATE(d) == CON_PLAYING) && ((GET_LEVEL(d->character) >= 31) || PLR_FLAGGED(d->character, PLR_IMMCHAR)) && (!PRF_FLAGGED(d->character, PRF_NOWIZ)) && (!PLR_FLAGGED(d->character, PLR_WRITING | PLR_MAILING)) && (d != ch->desc || !(PRF_FLAGGED(d->character, PRF_NOREPEAT))))
		{
			send_to_char(CCWHT(d->character, C_NRM), d->character);
			send_to_char(buf1, d->character);
			send_to_char(CCNRM(d->character, C_NRM), d->character);
		}
	}
	basic_mud_log(buf1);
	send_to_char("Your prayer has been sent to the heavens.\r\n", ch);
}

}


void garble_text(char *string, int percent) {
    char letters[] = "aeiousthpwxyz";
    int i;

    for (i = 0; i < strlen(string); i++) {
        if (isalpha(string[i] && number(0, 1) && number(0, 100) > percent) {
            string[i] = letters[number(0, 12)];
        }
    }
}

ACMD(do_lang_say) {
    extern char *languages[];
    char ibuf[MAX_INPUT_LENGTH];
    char obuf[MAX_INPUT_LENGTH];
    int ofs = 190;
    struct char_data *tch;

    skip_spaces(&argument);

    if(!*argument) {
        send_to_char("What do you want to say?\r\n", ch);
        return;
    }

    strcpy(ibuf, argument);

    garble_text(ibuf, GET_SKILL(ch, SPEAKING(ch)));

    for (tch = world[ch->in_room].people; tch; tch = tch->next_in_room) {
        if (tch != ch && AWAKE(tch) && tch->desc) {
            strcpy(obuf, ibuf);

            garble_text(obuf, GET_SKILL(tch, SPEAKING(ch)));

            if (GET_SKILL(tch, SPEAKING(ch)) < 1) {
                sprintf(buf, "$n says, in an unfamiliar language, '%s'", obuf);
            } else {
                sprintf(buf, "$n says, in %s, '%s'", languages[(SPEAKING(ch) - ofs)], obuf);
            }
            act(buf, TRUE, ch, 0, tch, TO_VICT);
        }
    }

    sprintf(buf, "You say, in %s, '%s'", languages[(SPEAKING(ch) - ofs)], argument);
    act(buf, TRUE, ch, 0, 0, TO_CHAR);
}
