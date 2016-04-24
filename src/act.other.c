/* ************************************************************************
*   File: act.other.c                                   Part of CircleMUD *
*  Usage: Miscellaneous player-level commands                             *
*                                                                         *
*  All rights reserved.  See license.doc for complete information.        *
*                                                                         *
*  Copyright (C) 1993, 94 by the Trustees of the Johns Hopkins University *
*  CircleMUD is based on DikuMUD, Copyright (C) 1990, 1991.               *
************************************************************************ */

#define __ACT_OTHER_C__

#include "conf.h"
#include "sysdep.h"

#include "structs.h"
#include "utils.h"
#include "comm.h"
#include "interpreter.h"
#include "handler.h"
#include "db.h"
#include "spells.h"
#include "screen.h"
#include "house.h"
#include "dg_scripts.h"

/* extern variables */
extern const char *dirs[];
extern struct str_app_type str_app[];
extern struct room_data *world;
extern struct descriptor_data *descriptor_list;
extern struct room_data *world;
extern struct dex_skill_type dex_app_skill[];
extern int top_of_p_table;

/* extern struct spell_info_type spell_info[]; */
extern struct index_data *mob_index;
extern char *class_abbrevs[];
extern int free_rent;
extern struct index_data *obj_index;
extern int xap_objs;

void extract_obj(struct obj_data *obj);

/* extern procedures */
int spell_access(struct char_data * ch, int spellnum);
void add_follower(struct char_data *ch, struct char_data *leader);
void list_skills(struct char_data * ch);
void appear(struct char_data * ch);
void write_aliases(struct char_data* ch);
void perform_immort_vis(struct char_data *ch);
SPECIAL(shop_keeper);
ACMD(do_gen_comm);
void die(struct char_data * ch, struct char_data * killer);
void list_player_skills(struct char_data * ch);
void Crash_rentsave(struct char_data *ch, int cost);
int mag_savingthrow(struct char_data * ch, int type);
void affect_from_char_II(struct char_data * ch, int skill, int type, int action);
;

#define VNUM_FIRE 16
#define MAX_DIS	  21

int armor_apply(struct char_data *ch, int spellnum)
{
  int weight;

  if (GET_EQ(ch, WEAR_BODY))
    weight = GET_OBJ_WEIGHT(GET_EQ(ch, WEAR_BODY));
  else
    return 0;   // no penalties for no armor

  if (spellnum == SKILL_HIDE)
    weight /= 4;
  else if (spellnum == SKILL_SNEAK)
    weight /= 3;
  else if (spellnum == SKILL_STEAL)
    weight /= 2;

  return weight;
}
ACMD(do_quit)
{
  struct descriptor_data *d, *next_d;

  if (IS_NPC(ch) || !ch->desc)
    return;

   if (GET_POS(ch) == POS_FISHING || GET_POS(ch) == POS_DIGGING || GET_POS(ch) ==  POS_RIDING) {
     send_to_char("You are not in a proper position for that!\r\n", ch);
    return;
}


  if (subcmd != SCMD_QUIT && GET_LEVEL(ch) < LVL_IMMORT) {
    send_to_char("If you quit without renting, YOUR EQUIPMENT WILL BE LOST!\r\n", ch);
    send_to_char("You have to type quitnow -- no less, to quit!\r\n", ch);
    // send_to_char("NOTE: You need to rent at an inn to save your items!\r\n", ch);
  } else if (GET_POS(ch) == POS_FIGHTING)
    send_to_char("No way!  You're fighting for your life!\r\n", ch);
  else if (GET_POS(ch) < POS_STUNNED) {
    send_to_char("You die before your time...\r\n", ch);
    die(ch, NULL);
  } else {
    int loadroom = ch->in_room;

    if (!GET_INVIS_LEV(ch)) {
      act("$n has left the game.", TRUE, ch, 0, 0, TO_ROOM);
    }

    GET_NAME(ch, chname);
    sprintf(buf, "%s has quit the game.", chname);
    FREE_NAME(chname);
    mudlog(buf, NRM, MAX(LVL_IMMORT, GET_INVIS_LEV(ch)), TRUE);
    send_to_char("Goodbye, friend.. Come back soon!\r\n", ch);

    /*
     * kill off all sockets connected to the same player as the one who is
     * trying to quit.  Helps to maintain sanity as well as prevent duping.
     */
    for (d = descriptor_list; d; d = next_d) {
      next_d = d->next;
      if (d == ch->desc)
        continue;
      if (d->character && (GET_IDNUM(d->character) == GET_IDNUM(ch)))
        STATE(d) = CON_DISCONNECT;
    }

    if (free_rent || GET_LEVEL(ch) >= LVL_IMMORT) {
      Crash_rentsave(ch, 0);
    }

    extract_char(ch);		/* Char is saved in extract char */

    /* If someone is quitting in their house, let them load back here */
    if (ROOM_FLAGGED(loadroom, ROOM_HOUSE))
      save_char(ch, loadroom);
  }
}



ACMD(do_save)
{
  if (IS_NPC(ch) || !ch->desc)
    return;

  /* Only tell the char we're saving if they actually typed "save" */
  if (cmd) {
    GET_NAME(ch, chname);
    sprintf(buf, "Saving %s.\r\n", chname);
    FREE_NAME(chname);
    send_to_char(buf, ch);
  }

  write_aliases(ch);
  save_char(ch, NOWHERE);
  Crash_crashsave(ch, FALSE);
  /* if (ROOM_FLAGGED(ch->in_room, ROOM_HOUSE_CRASH))
    House_crashsave(world[ch->in_room].number); */
}


/* generic function for commands which are normally overridden by
   special procedures - i.e., shop commands, mail commands, etc. */
ACMD(do_not_here)
{
  send_to_char("Sorry, but you cannot do that here!\r\n", ch);
}



ACMD(do_sneak)
{
  //struct affected_type af;
  //byte percent;

  if (GET_SKILL(ch, SKILL_SNEAK) < 1 && !PLR_FLAGGED(ch, PLR_CAT))
  {
    send_to_char("You're about as stealthy as the average Goon.\r\n", ch);
    return;
  }

   if (GET_POS(ch) == POS_FISHING || GET_POS(ch) == POS_DIGGING || GET_POS(ch) ==  POS_RIDING) {
     send_to_char("You are not in a proper position for that!\r\n", ch);
    return;
}

  if (affected_by_spell(ch, SPELL_FAERIE_FIRE))
  {
    send_to_char("Not while you're glowing with faerie fire.\r\n", ch);
    return;
  }
  if (!IS_AFFECTED(ch, AFF_SNEAK)) {
    send_to_char("Okay, you'll try to move silently for a while.\r\n", ch);
    SET_BIT(AFF_FLAGS(ch), AFF_SNEAK);
  }
  else {
    REMOVE_BIT(AFF_FLAGS(ch), AFF_SNEAK);
    send_to_char("Okay, you'll stop trying to move silently then.\r\n", ch);
  }
}



ACMD(do_hide)
{
  //byte percent;

   if (GET_POS(ch) == POS_FISHING || GET_POS(ch) == POS_DIGGING || GET_POS(ch) ==  POS_RIDING) {
     send_to_char("You are not in a proper position for that!\r\n", ch);
    return;
}


  if (GET_SKILL(ch, SKILL_HIDE) < 1)
  {
    send_to_char("Maybe you should think about running instead?\r\n", ch);
    return;
  }

  if (affected_by_spell(ch, SPELL_FAERIE_FIRE))
  {
    send_to_char("While glowing with faerie fire?  Yeah, right.\r\n", ch);
    return;
  }

//  send_to_char("You attempt to hide yourself.\r\n", ch);

  if (!IS_AFFECTED(ch, AFF_HIDE)) {
    SET_BIT(AFF_FLAGS(ch), AFF_HIDE);
    send_to_char("You will now begin to hide from others.\r\n", ch);
  }
  else {
    REMOVE_BIT(AFF_FLAGS(ch), AFF_HIDE);
    send_to_char("You will no longer try to hide from others.\r\n", ch);
  }

}


ACMD(do_slip) {
  struct char_data *vict;
  struct obj_data *obj;
  char vict_name[MAX_INPUT_LENGTH], obj_name[MAX_INPUT_LENGTH];
  int percent, prob, eq_pos, ohoh = 0, coinsGot;

  argument = one_argument(argument, obj_name);
  one_argument(argument, vict_name);

  if (GET_POS(ch) == POS_FISHING || GET_POS(ch) == POS_DIGGING || GET_POS(ch) ==  POS_RIDING) {
    send_to_char("You are not in a proper position for that!\r\n", ch);
    return;
  }

  if (!(vict = get_char_vis(ch, vict_name, FIND_CHAR_ROOM))) {
    send_to_char("Slip what to who?\r\n", ch);
    return;
  } else if (vict == ch) {
    send_to_char("Come on now, that's rather stupid!\r\n", ch);
    return;
  }


}


ACMD(do_steal) {
  struct char_data *vict;
  struct obj_data *obj;
  char vict_name[MAX_INPUT_LENGTH], obj_name[MAX_INPUT_LENGTH];
  int percent, prob, eq_pos, ohoh = 0, coinsGot;

  argument = one_argument(argument, obj_name);
  one_argument(argument, vict_name);

   if (GET_POS(ch) == POS_FISHING || GET_POS(ch) == POS_DIGGING || GET_POS(ch) ==  POS_RIDING) {
    send_to_char("You are not in a proper position for that!\r\n", ch);
    return;
   }

  if (!(vict = get_char_vis(ch, vict_name, FIND_CHAR_ROOM))) {
    send_to_char("Steal what from who?\r\n", ch);
    return;
  } else if (vict == ch) {
    send_to_char("Come on now, that's rather stupid!\r\n", ch);
    return;
  }

  prob = dex_app_skill[GET_DEX(ch)].p_pocket;
  prob -= armor_apply(ch, SKILL_STEAL);

  if (!IS_NPC(vict) && (GET_LEVEL(vict) > GET_LEVEL(ch)))
    prob -= (GET_LEVEL(vict) - GET_LEVEL(ch));

  if ((GET_SKILL(ch, SKILL_STEAL) + prob) > 80)  // max at 80% chance to steal
    prob = 80 - GET_SKILL(ch, SKILL_STEAL);

  percent = MAX(1, number(1, 101) - prob);  // so high dex people can't steal

  if (GET_POS(vict) < POS_SLEEPING)
    percent = -1;		/* ALWAYS SUCCESS */

  if (GET_LEVEL(vict) >= LVL_IMMORT || GET_MOB_SPEC(vict) == shop_keeper)
    percent = 101;		/* Failure */

  if (str_cmp(obj_name, "coins") && str_cmp(obj_name, "gold")) {
    if (!(obj = get_obj_in_list_vis(ch, obj_name, vict->carrying))) {
        for (eq_pos = 0; eq_pos < NUM_WEARS; eq_pos++)
            if (GET_EQ(vict, eq_pos) && (isname(obj_name, GET_EQ(vict, eq_pos)->name)) && CAN_SEE_OBJ(ch, GET_EQ(vict, eq_pos))) {
                obj = GET_EQ(vict, eq_pos);
                break;
            }

            if (!obj) {
                act("$E hasn't got that item.", FALSE, ch, 0, vict, TO_CHAR);
                return;
            } else {			/* It is equipment */
                if ((GET_POS(vict) > POS_STUNNED)) {
                    send_to_char("Steal the equipment now?  Impossible!\r\n", ch);
                    return;
                } else {
                    act("You unequip $p and steal it.", FALSE, ch, obj, 0, TO_CHAR);
                    act("$n steals $p from $N.", FALSE, ch, obj, vict, TO_NOTVICT);
                    obj_to_char(unequip_char(vict, eq_pos), ch);
                }
            }
    } else {			/* obj found in inventory */
        percent += GET_OBJ_WEIGHT(obj);	/* Make heavy harder */

        if (AWAKE(vict) && (!use_skill(ch, percent, SKILL_STEAL))) {
            ohoh = TRUE;
            act("Oops..", FALSE, ch, 0, 0, TO_CHAR);
            act("$n tried to steal something from you!", FALSE, ch, 0, vict, TO_VICT);
            act("$n tries to steal something from $N.", TRUE, ch, 0, vict, TO_NOTVICT);
        } else {			/* Steal the item */
            if ((IS_CARRYING_N(ch) + 1 < CAN_CARRY_N(ch))) {
                if ((IS_CARRYING_W(ch) + GET_OBJ_WEIGHT(obj)) < CAN_CARRY_W(ch)) {
                    obj_from_char(obj);
                    obj_to_char(obj, ch);
                    send_to_char("Got it!\r\n", ch);
                }
            } else {
              send_to_char("You cannot carry that much.\r\n", ch);
            }
        }
    }
  } else {			/* Steal some coins */
    if (AWAKE(vict) && (!use_skill(ch, percent, SKILL_STEAL)))
    {
      ohoh = TRUE;
      act("Oops..", FALSE, ch, 0, 0, TO_CHAR);
      act("You discover that $n has $s hands in your wallet.",
	  FALSE, ch, 0, vict, TO_VICT);
      act("$n tries to steal money from $N.", TRUE, ch, 0, vict, TO_NOTVICT);

    } else {
      /*
       *  steal some coins - randomly decide on which ones
       *  line up all the coins, and select which randomly
       */
      int               total_coins = 0, loop_coins, which_coin;
      struct money_data  stolen;

      stolen.platinum = stolen.steel = stolen.gold = stolen.copper = 0;

      /*  determine how many coins  */
      total_coins = (GET_MONEY(vict)).platinum +
	(GET_MONEY(vict)).steel +
	(GET_MONEY(vict)).gold +
	(GET_MONEY(vict)).copper;
      coinsGot = (int) ((total_coins * number(1, 10)) / 100);
      coinsGot = MIN(1782, coinsGot);

      if (coinsGot > 0) {

	for(loop_coins=0; loop_coins<coinsGot; loop_coins++) {
	  /*  randomly select  */
	  total_coins = (GET_MONEY(vict)).platinum +
	    (GET_MONEY(vict)).steel +
	    (GET_MONEY(vict)).gold +
	    (GET_MONEY(vict)).copper;

	  which_coin = number(1, total_coins);

	  if ( which_coin <= (GET_MONEY(vict)).platinum ) {
	    (GET_MONEY(vict)).platinum--;
	    (GET_MONEY(ch)).platinum++;
	    stolen.platinum++;
	  } else if ( which_coin <= (GET_MONEY(vict)).steel ) {
	    (GET_MONEY(vict)).steel--;
	    (GET_MONEY(ch)).steel++;
	    stolen.steel++;
	  } else if ( which_coin <= (GET_MONEY(vict)).gold ) {
	    (GET_MONEY(vict)).gold--;
	    (GET_MONEY(ch)).gold++;
	    stolen.gold++;
	  } else {
	    (GET_MONEY(vict)).copper--;
	    (GET_MONEY(ch)).copper++;
	    stolen.copper++;
	  }

	}

	sprintf(buf, "Bingo!  You got %s.\r\n", describe_money(stolen));
	send_to_char(buf, ch);

      } else {
	send_to_char("You couldn't get any coins...\r\n", ch);
      }
    }
  }

  if (ohoh && IS_NPC(vict) && AWAKE(vict)) {
    hit(vict, ch, TYPE_UNDEFINED);
  }

  if (!IS_NPC(ch) && !IS_NPC(vict))
  {
    ch->player.time.stealAttempt = time(0);
    SET_BIT(PLR_FLAGS(ch), PLR_THIEF);
    GET_NAME(ch, chname);
    GET_NAME(vict, victname);
    sprintf(buf, "%s tried to steal from %s at %s.",
      chname, victname, world[ch->in_room].name);
    FREE_NAME(victname);
    FREE_NAME(chname);
    mudlog(buf, BRF, LVL_LESSERGOD, TRUE);
  }

  WAIT_STATE(ch, 3 * PULSE_VIOLENCE);
}



ACMD(do_practice)
{
  one_argument(argument, arg);

  if (*arg)
    send_to_char("You can only practice skills in your guild.\r\n", ch);
  else
    list_skills(ch);
}



ACMD(do_visible)
{
  if (GET_LEVEL(ch) >= LVL_IMMORT) {
    perform_immort_vis(ch);
    return;
  }

  if IS_AFFECTED(ch, AFF_INVISIBLE) {
    appear(ch);
    send_to_char("You break the spell of invisibility.\r\n", ch);
  } else
    send_to_char("You are already visible.\r\n", ch);
}



ACMD(do_title)
{
  skip_spaces(&argument);
  delete_doubledollar(argument);

  if (IS_NPC(ch)) {
    send_to_char("Your title is fine... go away.\r\n", ch);
  } else if (GET_LEVEL(ch) < LVL_IMMORT) {
    send_to_char("You can't title yourself - ask an IMM (and say please).\r\n", ch);
  } else if (strstr(argument, "(") || strstr(argument, ")")) {
    send_to_char("Titles can't contain the ( or ) characters.\r\n", ch);
  } else if (strlen(argument) > MAX_TITLE_LENGTH) {
    sprintf(buf, "Sorry, titles can't be longer than %d characters.\r\n",
	    MAX_TITLE_LENGTH);
    send_to_char(buf, ch);
  } else {
    set_title(ch, argument);
    /* sprintf(buf, "Okay, you're now %s %s.\r\n", GET_NAME(ch), GET_TITLE(ch)); */
    sprintf(buf, "Okay, you're now: %s.\r\n", GET_TITLE(ch));
    send_to_char(buf, ch);
  }
}

/* The stubs of the scribe skill
ACMD(do_scribe)
{
   if(IS_NPC(ch))
   {
      send_to_char("I think not.\r\n", ch);


*/


int perform_group(struct char_data *ch, struct char_data *vict)
{
  if (IS_AFFECTED(vict, AFF_GROUP) || !CAN_SEE(ch, vict))
    return 0;

  SET_BIT(AFF_FLAGS(vict), AFF_GROUP);
  if (ch != vict)
    act("$N is now a member of your group.", FALSE, ch, 0, vict, TO_CHAR);
  act("You are now a member of $n's group.", FALSE, ch, 0, vict, TO_VICT);
  act("$N is now a member of $n's group.", FALSE, ch, 0, vict, TO_NOTVICT);
  return 1;
}

/*  PDH 11/24/98 - group diagnosis  */
char * group_diag(struct char_data * i)
{
  int  percent;
  char output[MAX_STRING_LENGTH];

  if (GET_MAX_HIT(i) > 0) {
    percent = (100 * GET_HIT(i)) / GET_MAX_HIT(i);
  } else {
    percent = -1;/* How could MAX_HIT be < 1?? */
  }

  sprintf(output, "health: (");

  if (percent >= 100) {
    sprintf(output, "%ssuperb", output);
  } else if (percent >= 90) {
    sprintf(output, "%sfew scratches", output);
  } else if (percent >= 80) {
    sprintf(output, "%snicks and cuts", output);
  } else if (percent >= 70) {
    sprintf(output, "%sbleeding slightly", output);
  } else if (percent >= 60) {
    sprintf(output, "%slight wounds", output);
  } else if (percent >= 50) {
    sprintf(output, "%smoderate wounds", output);
  } else if (percent >= 40) {
    sprintf(output, "%swounded", output);
  } else if (percent >= 30) {
    sprintf(output, "%sheavy wounds", output);
  } else if (percent >= 20) {
    sprintf(output, "%ssevere wounds", output);
  } else if (percent >= 10) {
    sprintf(output, "%sfading awayd", output);
  } else if (percent >= 0) {
   sprintf(output, "%snear death", output);
  } else {
    sprintf(output, "%sbrink of death", output);
  }

  if (GET_MAX_MOVE(i) > 0) {
    percent = (100 * GET_MOVE(i)) / GET_MAX_MOVE(i);
  }
  else {
    percent = -1;
  }

  sprintf(output, "%s)  |  stamina: (", output);

  if (percent >= 100)
    sprintf(output, "%senergetic", output);
  else if (percent >= 90)
    sprintf(output, "%sfresh", output);
  else if (percent >= 75)
    sprintf(output, "%swinded", output);
  else if (percent >= 50)
    sprintf(output, "%sout of breath", output);
  else if (percent >= 30)
    sprintf(output, "%sgetting tired", output);
  else if (percent >= 15)
    sprintf(output, "%stired", output);
  else if (percent >= 0)
    sprintf(output, "%shaggard", output);
  else
    sprintf(output, "%sexhausted", output);

  sprintf(output, "%s)", output);

  return strdup(output);
}


int getGroupSize(struct char_data * leader)
{
  int found = 0;
  struct follow_type * f;


  if (leader->master)
  {
    leader = leader->master;
  }

  if (IS_AFFECTED(leader, AFF_GROUP))
    found++;

  for (f=leader->followers; f; f=f->next)
  {
    if (IS_AFFECTED(f->follower, AFF_GROUP))
    {
      found++;
    }
  }

  return found;
}


int maxGroupSize(struct char_data * leader)
{
  int found = 0, cha_tot = 0;
  struct follow_type * f;


  if (leader->master)
  {
    leader = leader->master;
  }

  if (!IS_AFFECTED(leader, AFF_GROUP))
  {
    return (GET_CHA(leader) / 2);
  }

  cha_tot = GET_CHA(leader);
  found = 1;

  for (f=leader->followers; f; f=f->next)
  {
    if (IS_AFFECTED(f->follower, AFF_GROUP))
    {
      cha_tot += GET_CHA(f->follower);
      found++;
    }
  }

  return (cha_tot / (2 * found));
}


void print_group(struct char_data *ch)
{
  struct char_data *k;
  struct follow_type *f;

  if (!IS_AFFECTED(ch, AFF_GROUP)) {
    send_to_char("But you are not the member of a group!\r\n", ch);

  } else {
    char grpMsg[200];

    sprintf(grpMsg,
	    "Your group has %d members, out of a possible %d members.\r\n",
	    getGroupSize(ch), maxGroupSize(ch));
    send_to_char(grpMsg, ch);
    send_to_char("Your group consists of:\r\n", ch);

    k = (ch->master ? ch->master : ch);

    if (IS_AFFECTED(k, AFF_GROUP)) {
      if ( CAN_SEE(ch, k) ) {
        GET_NAME_II(k, ch, chname);
        sprintf(buf, "  %-20s %s (Head of group)\r\n",
                chname, group_diag(k));
        FREE_NAME(chname);

      } else {
        sprintf(buf, "  %-20s %s (Head of group)\r\n",
               "someone", "unknown");
      }

      send_to_char(buf, ch);


    }

    for (f = k->followers; f; f = f->next) {
      if (!IS_AFFECTED(f->follower, AFF_GROUP)) {
	continue;
      }

      if ( CAN_SEE(ch, f->follower) ) {
        GET_NAME_II(f->follower, ch, chname);

	sprintf(buf, "  %-20s %s\r\n", chname, group_diag(f->follower));
        FREE_NAME(chname);
      } else {
	sprintf(buf, "  %-20s (%s)\r\n", "someone", "unknown");
      }

      send_to_char(buf, ch);

    }
  }
}


ACMD(do_group) {
  struct char_data *vict;
  struct follow_type *f;
  int found;

  one_argument(argument, buf);

  if (!*buf) {
    print_group(ch);
    return;
  }

  /*  PDH  4/ 1/99
   *  groups are now limited in size by Charisma
   *  (group avg Cha)/2 is max size of group
   */

  if (ch->master) {
    act("You can not enroll group members without being head of a group.", FALSE, ch, 0, 0, TO_CHAR);
    return;
  }

  if (!str_cmp(buf, "all")) {
    if (!IS_AFFECTED(ch, AFF_GROUP)) {
      if ((getGroupSize(ch) + 1) > maxGroupSize(ch)) {
        send_to_char("You can't even group yourself!\r\n", ch);
        return;
      }

      perform_group(ch, ch);
    }

    for (found = 0, f = ch->followers; f; f = f->next) {
      if (IS_AFFECTED(f->follower, AFF_GROUP))
        continue;
      else if ((getGroupSize(ch) + 1) > maxGroupSize(ch))
        send_to_char("Sorry - you do not possess the qualities to lead any more people.\r\n", ch);
      else
        found += perform_group(ch, f->follower);
    }

    if (!found) {
      send_to_char("Everyone following you is already in your group.\r\n", ch);
    }

    return;
  }

  if (!(vict = get_char_vis(ch, buf, FIND_CHAR_ROOM))) {
    send_to_char(NOPERSON, ch);
  } else if ((vict->master != ch) && (vict != ch)) {
    act("$N must follow you to enter your group.", FALSE, ch, 0, vict, TO_CHAR);
  } else {
    if (!IS_AFFECTED(vict, AFF_GROUP)) {
      if ((getGroupSize(ch) + 1) > maxGroupSize(ch))
	send_to_char("Sorry - you do not possess the qualities to lead any more people.\r\n", ch);
      else
	perform_group(ch, vict);
    } else {
      if (ch != vict)
        act("$N is no longer a member of your group.", FALSE, ch, 0, vict, TO_CHAR);
        act("You have been kicked out of $n's group!", FALSE, ch, 0, vict, TO_VICT);
        act("$N has been kicked out of $n's group!", FALSE, ch, 0, vict, TO_NOTVICT);
        REMOVE_BIT(AFF_FLAGS(vict), AFF_GROUP);
        max_group_exp_mult(vict);
        GET_EXP_MULT(vict) = 0;
    }
  }
}


ACMD(do_ungroup) {
  struct follow_type *f, *next_fol;
  struct char_data *tch;
  void stop_follower(struct char_data * ch);

  one_argument(argument, buf);

  if (!*buf) {
    if (ch->master || !(IS_AFFECTED(ch, AFF_GROUP))) {
      send_to_char("But you lead no group!\r\n", ch);
      return;
    }

    for (f = ch->followers; f; f = next_fol) {
      next_fol = f->next;
      if (IS_AFFECTED(f->follower, AFF_GROUP)) {
        REMOVE_BIT(AFF_FLAGS(f->follower), AFF_GROUP);
        max_group_exp_mult(f->follower);
        GET_EXP_MULT(f->follower) = 0;
        GET_NAME_II(ch, f->follower, chname);
        sprintf(buf2, "%s has disbanded the group.\r\n", chname);
        FREE_NAME(chname);
        send_to_char(buf2, f->follower);
        if (!IS_AFFECTED(f->follower, AFF_CHARM))
          stop_follower(f->follower);
      }
    }

    REMOVE_BIT(AFF_FLAGS(ch), AFF_GROUP);
    max_group_exp_mult(ch);
    GET_EXP_MULT(ch) = 0;
    send_to_char("You disband the group.\r\n", ch);
    return;
  }
  if (!(tch = get_char_vis(ch, buf, FIND_CHAR_ROOM))) {
    send_to_char("There is no such person!\r\n", ch);
    return;
  }
  if (tch->master != ch) {
    send_to_char("That person is not following you!\r\n", ch);
    return;
  }

  if (!IS_AFFECTED(tch, AFF_GROUP)) {
    send_to_char("That person isn't in your group.\r\n", ch);
    return;
  }

  REMOVE_BIT(AFF_FLAGS(tch), AFF_GROUP);
  max_group_exp_mult(tch);
  GET_EXP_MULT(tch) = 0;

  act("$N is no longer a member of your group.", FALSE, ch, 0, tch, TO_CHAR);
  act("You have been kicked out of $n's group!", FALSE, ch, 0, tch, TO_VICT);
  act("$N has been kicked out of $n's group!", FALSE, ch, 0, tch, TO_NOTVICT);

  if (!IS_AFFECTED(tch, AFF_CHARM))
    stop_follower(tch);
}


ACMD(do_report)
{
  struct char_data *k;
  struct follow_type *f;

  if (!IS_AFFECTED(ch, AFF_GROUP)) {
    send_to_char("But you are not a member of any group!\r\n", ch);
    return;
  }

  k = (ch->master ? ch->master : ch);

  for (f = k->followers; f; f = f->next)
    if (IS_AFFECTED(f->follower, AFF_GROUP) && f->follower != ch)
    {
      GET_NAME_II(ch, f->follower, chname);
      sprintf(buf, "%s reports: %d/%dH, %d/%dV\r\n",
          chname, GET_HIT(ch), GET_MAX_HIT(ch),
          GET_MOVE(ch), GET_MAX_MOVE(ch));
      FREE_NAME(chname);
      CAP(buf);
      send_to_char(buf, f->follower);
    }

  if (k != ch)
  {
    GET_NAME_II(ch, k, chname);
    sprintf(buf, "%s reports: %d/%dH, %d/%dV\r\n",
        chname, GET_HIT(ch), GET_MAX_HIT(ch),
        GET_MOVE(ch), GET_MAX_MOVE(ch));
    FREE_NAME(chname);
    CAP(buf);
    send_to_char(buf, k);
  }

  send_to_char("You report to the group.\r\n", ch);
}


ACMD(do_split)
{
  int num;
  struct money_data amount, tmpAmt, share;
  struct char_data *k;
  struct follow_type *f;
  char amt1[300], amt2[300];

  if (IS_NPC(ch))
    return;

  argument = one_argument(argument, buf);

  if ( ! *buf ) {
    send_to_char("Split what?\r\n", ch);
    return;

  } else if (is_money(buf)) {

    amount.platinum = amount.steel = amount.gold = amount.copper = 0;
    tmpAmt.platinum = tmpAmt.steel = tmpAmt.gold = tmpAmt.copper = 0;

    while ( *buf ) {
      if ( is_money(buf) ) {
	tmpAmt.platinum = tmpAmt.steel = tmpAmt.gold = tmpAmt.copper = 0;
	value_money( &tmpAmt, buf);
	amount.platinum += tmpAmt.platinum;    amount.gold += tmpAmt.gold;
	amount.steel += tmpAmt.steel;    amount.copper += tmpAmt.copper;
      } else {
	/*  can only split money  */
	send_to_char("You can only split money.\r\n", ch);
	return;
      }

      argument = one_argument(argument, buf);
    }

    if ( money_to_copper(amount) <= 0) {
      send_to_char("Sorry, you can't do that.\r\n", ch);
      return;
    }

    if ( money_compare(GET_MONEY(ch), amount, TRUE) < 0 ) {
      send_to_char("You don't seem to have that many coins.\r\n", ch);
      return;
    }


    k = (ch->master ? ch->master : ch);

    if (IS_AFFECTED(k, AFF_GROUP) && (k->in_room == ch->in_room)) {
      num = 1;
    } else {
      num = 0;
    }

    for (f = k->followers; f; f = f->next) {
      if (IS_AFFECTED(f->follower, AFF_GROUP) &&
	  (!IS_NPC(f->follower)) &&
	  (f->follower->in_room == ch->in_room)) {
	num++;
      }
    }

    if (num && IS_AFFECTED(ch, AFF_GROUP)) {
      share.platinum = amount.platinum / num;
      share.steel = amount.steel / num;
      share.gold = amount.gold / num;
      share.copper = amount.copper / num;
    } else {
      send_to_char("With whom do you wish to share your gold?\r\n", ch);
      return;
    }

    strcpy(amt1, describe_money(amount));
    strcpy(amt2, describe_money(share));

    (GET_MONEY(ch)).platinum -= share.platinum * (num - 1);
    (GET_MONEY(ch)).steel -= share.steel * (num - 1);
    (GET_MONEY(ch)).gold -= share.gold * (num - 1);
    (GET_MONEY(ch)).copper -= share.copper * (num - 1);

    if (IS_AFFECTED(k, AFF_GROUP) && (k->in_room == ch->in_room)
        && !(IS_NPC(k)) && k != ch)
    {
      (GET_MONEY(k)).platinum += share.platinum;
      (GET_MONEY(k)).steel += share.steel;
      (GET_MONEY(k)).gold += share.gold;
      (GET_MONEY(k)).copper += share.copper;
      GET_NAME_II(ch, k, chname); 
      sprintf(buf, "%s splits %s; you receive %s.\r\n", chname, amt1, amt2);
      FREE_NAME(chname);
      send_to_char(buf, k);
    }

    for (f = k->followers; f; f = f->next) {
      if (IS_AFFECTED(f->follower, AFF_GROUP) &&
	  (!IS_NPC(f->follower)) &&
	  (f->follower->in_room == ch->in_room) &&
	  f->follower != ch) {
	(GET_MONEY(f->follower)).platinum += share.platinum;
	(GET_MONEY(f->follower)).steel += share.steel;
	(GET_MONEY(f->follower)).gold += share.gold;
	(GET_MONEY(f->follower)).copper += share.copper;
        GET_NAME_II(ch, f->follower, chname);
	sprintf(buf, "%s splits %s; you receive %s.\r\n", chname,
              amt1, amt2);
        FREE_NAME(chname);
	send_to_char(buf, f->follower);
      }
    }

    sprintf(buf, "You split %s among %d members -- %s each.\r\n",
            amt1, num, amt2);
    send_to_char(buf, ch);

  } else {
    send_to_char("How many coins do you wish to split with your group?\r\n",
		 ch);
    return;
  }
}


ACMD(do_use)
{
  struct obj_data *mag_item;
  int equipped = 1;

  half_chop(argument, arg, buf);

   if (GET_POS(ch) == POS_FISHING || GET_POS(ch) == POS_DIGGING) {
     send_to_char("You are not in a proper position for that!\r\n", ch);
    return;
}


  if (!*arg) {
    sprintf(buf2, "What do you want to %s?\r\n", CMD_NAME);
    send_to_char(buf2, ch);
    return;
  }

  mag_item = GET_EQ(ch, WEAR_HOLD);

  if (!mag_item || !isname(arg, mag_item->name)) {
    switch (subcmd) {
    case SCMD_RECITE:
      if (affected_by_spell(ch, SPELL_SILENCE)) {
        send_to_char("You need to be able to speak in order to recite a scroll.\r\n", ch);
        return;
      }
    case SCMD_QUAFF:
      equipped = 0;
      if (!(mag_item = get_obj_in_list_vis(ch, arg, ch->carrying))) {
	sprintf(buf2, "You don't seem to have %s %s.\r\n", AN(arg), arg);
	send_to_char(buf2, ch);
	return;
      }
      break;
    case SCMD_USE:
      sprintf(buf2, "You don't seem to be holding %s %s.\r\n", AN(arg), arg);
      send_to_char(buf2, ch);
      return;
      break;
    default:
      log("SYSERR: Unknown subcmd passed to do_use");
      return;
      break;
    }
  }

  switch (subcmd) {
  case SCMD_QUAFF:
    if (GET_OBJ_TYPE(mag_item) != ITEM_POTION) {
      send_to_char("You can only quaff potions.", ch);
      return;
    }
    break;

  case SCMD_RECITE:
    if (GET_OBJ_TYPE(mag_item) != ITEM_SCROLL) {
      send_to_char("You can only recite scrolls.", ch);
      return;
    }
    if (!GET_SKILL(ch, SKILL_READ_MAGIC))
    {
      send_to_char("You are unable to read magic.\r\n", ch);
      return;
    }
    break;

  case SCMD_USE:
    if ((GET_OBJ_TYPE(mag_item) != ITEM_WAND) &&
        (GET_OBJ_TYPE(mag_item) != ITEM_STAFF) &&
        (GET_OBJ_TYPE(mag_item) != ITEM_SALVE)) {
      send_to_char("You can't seem to figure out how to use it.\r\n", ch);
      return;
    }
    break;
  }

  mag_objectmagic(ch, mag_item, buf);
}


ACMD(do_wimpy)
{
  int wimp_lev;

  one_argument(argument, arg);

  if (!*arg) {
    if (GET_WIMP_LEV(ch)) {
      sprintf(buf, "Your current wimp level is %d hit points.\r\n",
	      GET_WIMP_LEV(ch));
      send_to_char(buf, ch);
      return;
    } else {
      send_to_char("At the moment, you're not a wimp.  (sure, sure...)\r\n", ch);
      return;
    }
  }
  if (isdigit(*arg)) {
    if ((wimp_lev = atoi(arg))) {
      if (wimp_lev < 0)
	send_to_char("Heh, heh, heh.. we are jolly funny today, eh?\r\n", ch);
      else if (wimp_lev > GET_MAX_HIT(ch))
	send_to_char("That doesn't make much sense, now does it?\r\n", ch);
      else if (wimp_lev > (GET_MAX_HIT(ch) >> 1))
	send_to_char("You can't set your wimp level above half your hit points.\r\n", ch);
      else {
	sprintf(buf, "Okay, you'll wimp out if you drop below %d hit points.\r\n",
		wimp_lev);
	send_to_char(buf, ch);
	GET_WIMP_LEV(ch) = wimp_lev;
      }
    } else {
      send_to_char("Okay, you'll now tough out fights to the bitter end.\r\n", ch);
      GET_WIMP_LEV(ch) = 0;
    }
  } else
    send_to_char("Specify at how many hit points you want to wimp out at.  (0 to disable)\r\n", ch);

  return;

}


ACMD(do_display)
{
  size_t i;

  if (IS_NPC(ch)) {
    send_to_char("Monsters don't really need displays.  Go away.\r\n", ch);
    return;
  }

  skip_spaces(&argument);

  if (!*argument) {
    send_to_char("Usage: prompt { { H | V } | all | none }\r\n", ch);
    return;
  }

  if ((!str_cmp(argument, "on")) || (!str_cmp(argument, "all"))) {
    SET_BIT(PRF_FLAGS(ch), PRF_DISPHP | PRF_DISPMOVE);
  } else if (!str_cmp(argument, "none")) {
    // Special case for turning off the prompt since C does not like
    // using str_cmp (or the native "strcmp") in a case statement.
    REMOVE_BIT(PRF_FLAGS(ch), PRF_DISPHP | PRF_DISPMOVE);
  } else {
    REMOVE_BIT(PRF_FLAGS(ch), PRF_DISPHP | PRF_DISPMOVE);

    for (i = 0; i < strlen(argument); i++) {
      switch (LOWER(argument[i])) {
        case 'h':
            SET_BIT(PRF_FLAGS(ch), PRF_DISPHP);
            break;
        case 'v':
            SET_BIT(PRF_FLAGS(ch), PRF_DISPMOVE);
            break;
        default:
            send_to_char("Usage: prompt { { H | V } | all | none }\r\n", ch);
            return;
            break;
      }
    }
  }

  send_to_char(OK, ch);
}


ACMD(do_gen_write)
{
  FILE *fl;
  char *tmp, *filename, buf[MAX_STRING_LENGTH];
  struct stat fbuf;
  extern int max_filesize;
  time_t ct;

  switch (subcmd) {
  case SCMD_BUG:
    filename = BUG_FILE;
    break;
  case SCMD_TYPO:
    filename = TYPO_FILE;
    break;
  case SCMD_IDEA:
    filename = IDEA_FILE;
    break;
  default:
    return;
  }

  ct = time(0);
  tmp = asctime(localtime(&ct));

  if (IS_NPC(ch)) {
    send_to_char("Monsters can't have ideas - Go away.\r\n", ch);
    return;
  }

  skip_spaces(&argument);
  delete_doubledollar(argument);

  if (!*argument) {
    send_to_char("That must be a mistake...\r\n", ch);
    return;
  }
  GET_NAME(ch, chname);
  sprintf(buf, "%s %s: %s", chname, CMD_NAME, argument);
  FREE_NAME(chname);
  mudlog(buf, CMP, LVL_IMMORT, FALSE);

  if (stat(filename, &fbuf) < 0) {
    perror("Error statting file");
    return;
  }
  if (fbuf.st_size >= max_filesize) {
    send_to_char("Sorry, the file is full right now.. try again later.\r\n", ch);
    return;
  }
  if (!(fl = fopen(filename, "a"))) {
    perror("do_gen_write");
    send_to_char("Could not open the file.  Sorry.\r\n", ch);
    return;
  }
  GET_NAME(ch, chname);
  fprintf(fl, "%-8s (%6.6s) [%5d] %s\n", chname, (tmp + 4),
	  world[ch->in_room].number, argument);
  FREE_NAME(chname);
  fclose(fl);
  send_to_char("Okay.  Thanks!\r\n", ch);
}


#define TOG_OFF 0
#define TOG_ON  1

#define PRF_TOG_CHK(ch,flag) ((TOGGLE_BIT(PRF_FLAGS(ch), (flag))) & (flag))


ACMD(do_gen_tog)
{
  long result;
  extern int nameserver_is_slow;

  char *tog_messages[][2] = {
   {"\r\n",
    "\r\n"},
   {"Nohassle disabled.\r\n",
    "Nohassle enabled.\r\n"},
   {"Brief mode off.\r\n",
    "Brief mode on.\r\n"},
   {"Compact mode off.\r\n",
    "Compact mode on.\r\n"},
   {"You can now hear tells.\r\n",
    "You are now deaf to tells.\r\n"},
   {"\r\n",
    "\r\n"},
   {"You can now hear shouts.\r\n",
    "You are now deaf to shouts.\r\n"},
   {"\r\n",
    "\r\n"},
   {"\r\n",
    "\r\n"},
   {"You can now hear the Wiz-channel.\r\n",
    "You are now deaf to the Wiz-channel.\r\n"},
   {"\r\n",
    "\r\n"},
   {"You will no longer see the room flags.\r\n",
    "You will now see the room flags.\r\n"},
   {"You will now have your communication repeated.\r\n",
    "You will no longer have your communication repeated.\r\n"},
   {"HolyLight mode off.\r\n",
    "HolyLight mode on.\r\n"},
   {"Nameserver_is_slow changed to NO; IP addresses will now be resolved.\r\n",
    "Nameserver_is_slow changed to YES; sitenames will no longer be resolved.\r\n"},
   {"Autoexits disabled.\r\n",
    "Autoexits enabled.\r\n"},
   {"AFK flag is now off.\r\n",
    "AFK flag is now on.\r\n"},
   {"\r\n",
    "\r\n"},
   {"\r\n",
    "\r\n"},
   {"You will not see levels on eq.\r\n",
    "You can now see levels on eq.\r\n"},
    {"You will not receive introductions.\r\n",
    "You will now receive introductions.\r\n"},
   {"You will no longer hear OOC chatter.\r\n",
    "You will now hear OOC chatter.\r\n"},
   {"You will now hear the newbie communication.\r\n",
    "You will no longer the newbie communication.\r\n"},
   {"You will now hear the drivel on the chat line.\r\n",
    "You will no longer hear the drivel on the chat line.\r\n"},
   {"You  will not detect disguised/morphed chars. \r\n",
    "You can now detect disguised/morphed chars.\r\n"},
    {"Ascii objects turned off.\r\n",
    "Ascii objects turned on.\r\n"}		
  };

   if (IS_NPC(ch) && subcmd != SCMD_AUTOEXIT)
    return;

  switch (subcmd) {
  case SCMD_NOHASSLE:
    result = PRF_TOG_CHK(ch, PRF_NOHASSLE);
    break;
  case SCMD_BRIEF:
    result = PRF_TOG_CHK(ch, PRF_BRIEF);
    break;
  case SCMD_COMPACT:
    result = PRF_TOG_CHK(ch, PRF_COMPACT);
    break;
  case SCMD_NOTELL:
    result = PRF_TOG_CHK(ch, PRF_NOTELL);
    break;
  case SCMD_DEAF:
    result = PRF_TOG_CHK(ch, PRF_DEAF);
    break;
  case SCMD_NOWIZ:
    if(!PLR_FLAGGED(ch, PLR_IMMCHAR) && (GET_LEVEL(ch) < 31))
    {
       send_to_char("Huh?!\r\n", ch);
       return;
    }
    result = PRF_TOG_CHK(ch, PRF_NOWIZ);
    break;
  case SCMD_ROOMFLAGS:
    result = PRF_TOG_CHK(ch, PRF_ROOMFLAGS);
    break;
  case SCMD_NOREPEAT:
    result = PRF_TOG_CHK(ch, PRF_NOREPEAT);
    break;
  case SCMD_HOLYLIGHT:
    result = PRF_TOG_CHK(ch, PRF_HOLYLIGHT);
    break;
  case SCMD_SLOWNS:
    result = (nameserver_is_slow = !nameserver_is_slow);
    break;
  case SCMD_AUTOEXIT:
    result = PRF_TOG_CHK(ch, PRF_AUTOEXIT);
    break;
  case SCMD_LEVEL_FLAGS:
    result = PRF_TOG_CHK(ch, PRF_LEVEL_FLAGS);
    break;
  case SCMD_INTRO:
    result = PRF_TOG_CHK(ch, PRF_INTRO);
    break;
  case SCMD_NOOOC:
    result = PRF_TOG_CHK(ch, PRF_NO_OOC);
    break;
  case SCMD_NONEWBIE:
    result = PRF_TOG_CHK(ch, PRF_NO_NEWBIE);
    break;
  case SCMD_NOCHAT:
    result = PRF_TOG_CHK(ch, PRF_NO_CHAT);
    break;
		
  case SCMD_XAP_OBJS:
 /* I guess we'll have interesing results if xap objs aren't 1 or 0 */
    result = (xap_objs = !xap_objs);  		

case SCMD_AFK:
  result = PRF_TOG_CHK(ch, PRF_AFK);
  if (PRF_FLAGGED(ch, PRF_AFK))
    act("$n has gone AFK.", TRUE, ch, 0, 0, TO_ROOM);
  else
    act("$n has come back from AFK.", TRUE, ch, 0, 0, TO_ROOM);
  break;

  case SCMD_DETECT:
    if(!PLR_FLAGGED(ch, PLR_IMMCHAR) && (GET_LEVEL(ch) < 31))
    {
       send_to_char("Huh?!\r\n", ch);
       return;
    }
    result = PRF_TOG_CHK(ch, PRF_DETECT);
    break;

  default:
    log("SYSERR: Unknown subcmd in do_gen_toggle");
    return;
    break;
  }

  if (result)
    send_to_char(tog_messages[subcmd][TOG_ON], ch);
  else
    send_to_char(tog_messages[subcmd][TOG_OFF], ch);

  return;
}

ACMD(do_skills)
{
  list_player_skills(ch);
}

ACMD(do_bandage)
{
  struct char_data * vict;
  //char message[MAX_STRING_LENGTH];

  if (!GET_SKILL(ch, SKILL_BANDAGE))
  {
    send_to_char("You are unskilled in the art of bandaging.\r\n", ch);
    return;
  }

 if (GET_POS(ch) == POS_FISHING || GET_POS(ch) == POS_DIGGING || GET_POS(ch) ==  POS_RIDING) {
     send_to_char("You are not in a proper position for that!\r\n", ch);
    return;
}


  one_argument(argument, arg);

  if (!(vict = get_char_vis(ch, arg, FIND_CHAR_ROOM)))
  {
    send_to_char("Who do you want to bandage?\r\n", ch);
    return;
  }

  if (GET_HIT(vict) >= 0)
  {
    if (FIGHTING(ch) || FIGHTING(vict)) {
      send_to_char("The frays of combat are too busy for you to bandage them.\r\n", ch);
      return;
    }
    if (IS_SET(PRF_FLAGS(vict), PRF_NOBANDAGE) || IS_SET(PRF_FLAGS(vict), PRF_BANDAGED) ||
        GET_HIT(vict) >= GET_MAX_HIT(vict)) {
      send_to_char("That character cannot be bandaged right now.\r\n", ch);
      return;
    }
    if (!check_skill(ch, 101, SKILL_BANDAGE)) {
      if (vict == ch) {
        act("You apply the bandages to your wounds.", FALSE, ch, 0, vict, TO_CHAR);
        act("$n applies bandages to $s wounds.", FALSE, ch, 0, vict, TO_ROOM);
      }
      else {
        act("You apply the bandages to $N's wounds.", FALSE, ch, 0, vict, TO_CHAR);
        act("$n applies bandages to your wounds.", FALSE, ch, 0, vict, TO_VICT);
        act("$n applies bandages to $N's wounds.", FALSE, ch, 0, vict, TO_ROOM);
      }
      SET_BIT(PRF_FLAGS(vict), PRF_NOBANDAGE);

      return;
    }

    if (vict == ch) {
      act("You apply the bandages to your wounds.", FALSE, ch, 0, vict, TO_CHAR);
      act("$n applies bandages to $s wounds.", FALSE, ch, 0, vict, TO_ROOM);
    }
    else {
      act("You apply the bandages to $N's wounds.", FALSE, ch, 0, vict, TO_CHAR);
      act("$n applies bandages to your wounds.", FALSE, ch, 0, vict, TO_VICT);
      act("$n applies bandages to $N's wounds.", FALSE, ch, 0, vict, TO_ROOM);
    }

    SET_BIT(PRF_FLAGS(vict), PRF_BANDAGED);

    return;
  }

  WAIT_STATE(ch, PULSE_VIOLENCE * 2);

  if (!check_skill(ch, 101, SKILL_BANDAGE))
  {
    act("Oops..", FALSE, ch, 0, 0, TO_CHAR);
    act("$n tries to bandage $N, but fails miserably.",
           TRUE, ch, 0, vict, TO_NOTVICT);
    damage(vict, vict, 2, TYPE_SUFFERING);
    return;
  }

  act("You bandage $N.", FALSE, ch, 0, vict, TO_CHAR);
  act("$n bandages $N, who looks a bit better now.", TRUE, ch, 0, vict, TO_NOTVICT);
  act("Someone bandages you, and you feel a bit better now.",
         FALSE, ch, 0, vict, TO_VICT);
  GET_HIT(vict) = 0;
}

ACMD(do_remove_poison)
{
  struct char_data *vict;

  if (!GET_SKILL(ch, SKILL_REMOVE_POISON))
  {
    send_to_char("You don't know how to remove poison.\r\n", ch);
    return;
  }

   if (GET_POS(ch) == POS_FISHING || GET_POS(ch) == POS_DIGGING || GET_POS(ch) ==  POS_RIDING) {
     send_to_char("You are not in a proper position for that!\r\n", ch);
    return;
}


  one_argument(argument, arg);

  if (!(vict = get_char_vis(ch, arg, FIND_CHAR_ROOM)))
  {
    send_to_char("Whom do you want to remove poison from?\r\n", ch);
    return;
  }

  if (!affected_by_spell(vict, SPELL_POISON))
  {
    send_to_char("But they're not even poisoned!\r\n", ch);
    return;
  }

  if (!check_skill(ch, 101, SKILL_REMOVE_POISON))
  {
    send_to_char("Oops...\r\n", ch);
    act("$n unsuccessfully tries to remove poison from $N.",
         TRUE, ch, 0, vict, TO_NOTVICT);
    act("$n fails to remove poison from you.", FALSE, ch, 0, vict, TO_VICT);
    damage(vict, vict, 2, SPELL_POISON);
  }
  else
  {
    act("$n removes the poison from $N's system.", 1, ch, 0, vict, TO_NOTVICT);
    act("$n remove the poison from your body.", 0, ch, 0, vict, TO_VICT);
    send_to_char("You remove the poison.\r\n", ch);
    affect_from_char(vict, SPELL_POISON);
  }

  WAIT_STATE(ch, PULSE_VIOLENCE * 2);

  return;
}

ACMD(do_detect_poison)
{
  struct char_data *vict;
  struct obj_data *obj = NULL;
  int poisoned = 0;

  if (!GET_SKILL(ch, SKILL_DETECT_POISON))
  {
    send_to_char("You have no idea how to detect poison.\r\n", ch);
    return;
  }

  one_argument(argument, arg);

  if (!(vict = get_char_vis(ch, arg, FIND_CHAR_ROOM)))
    if (!(obj = get_obj_in_list_vis(ch, arg, ch->carrying)))
      if (!(obj = get_obj_in_list_vis(ch, arg, world[ch->in_room].contents)))
      {
        send_to_char("What or who are you trying to detect poison in?\r\n", ch);
        return;
      }

  if (vict)
  {
    poisoned = IS_AFFECTED(vict, AFF_POISON);

    if (!check_skill(ch, 101, SKILL_DETECT_POISON))
      poisoned = !poisoned;

    if (vict == ch)
    {
      if (poisoned)
        send_to_char("You seem to be healthy.\r\n", ch);
      else
        send_to_char("You seem to be healthy.\r\n", ch);
    }
    else
    {
      if (poisoned)
        act("$N seems to be poisoned.", 0, ch, 0, vict, TO_CHAR);
      else
        act("$N seems to be healthy.", 0, ch, 0, vict, TO_CHAR);
    }
  }
  else if (obj)
  {
    poisoned = GET_OBJ_VAL(obj, 3);

    if (!check_skill(ch, 101, SKILL_DETECT_POISON))
      poisoned = !poisoned;

    switch (GET_OBJ_TYPE(obj))
    {
      case ITEM_DRINKCON:
      case ITEM_FOUNTAIN:
      case ITEM_FOOD:
        if (poisoned)
          act("You sense that $p is poisoned.", 0, ch, obj, 0, TO_CHAR);
        else
          act("$p seems to be safe for consumption.", 0, ch, obj, 0, TO_CHAR);
        break;
      default:
        send_to_char("That item is not a food or a drink.\r\n", ch);
    }
  }
  else  // no vict or obj?
  {
    sprintf(buf, "SYSERR: detect poison with no victim or obj");
    mudlog(buf, BRF, LVL_GRGOD, TRUE);
  }

  WAIT_STATE(ch, PULSE_VIOLENCE * 2);
}

ACMD(do_lay_hands)
{
  int amount, cha_modifier;
  int BASE_CHA_MODIFIER = 4;
  int BASE_CHA = 12;
  int char_charisma = GET_CHA(ch);
  struct char_data * vict;

  if (!GET_SKILL(ch, SKILL_LAY_HANDS))
  {
    send_to_char("You are unskilled at the laying-on of hands.\r\n", ch);
    return;
  }

   if (GET_POS(ch) == POS_FISHING || GET_POS(ch) == POS_DIGGING || GET_POS(ch) ==  POS_RIDING) {
     send_to_char("You are not in a proper position for that!\r\n", ch);
    return;
}

  one_argument(argument, arg);

  if (*arg)
  {
    if (!(vict = get_char_vis(ch, arg, FIND_CHAR_ROOM)))
    {
      send_to_char("Whom do you wish to heal?\r\n", ch);
      return;
    }
  } else
    vict = ch;

// 1800 seconds = 30 minutes = 24 ticks = one game day
  if ((time(0) - ch->player.time.last_lay_hands) < 1800)
  {
    send_to_char("You can only use this skill once per day.\r\n", ch);
    return;
  }

  if (FIGHTING(vict))
  {
    send_to_char("They'll have to stop fighting first.\r\n", ch);
    return;
  }

  if (!check_skill(ch, 101, SKILL_LAY_HANDS))
  {
    send_to_char("You attempt to lay on hands, but fail.", ch);
    ch->player.time.last_lay_hands = time(0);
    return;
  }

  if (char_charisma > BASE_CHA) {
    cha_modifier = BASE_CHA + char_charisma;
  } else {
    cha_modifier= BASE_CHA - char_charisma;
  }

  amount = GET_LEVEL(ch) * (BASE_CHA_MODIFIER + cha_modifier);

  GET_HIT(vict) = MIN(GET_HIT(vict) + amount, GET_MAX_HIT(vict));

  if (vict != ch)
  {
    act("You place your hands on $N, healing $M.", FALSE, ch, 0, vict, TO_CHAR);
    act("$n places $s hands on you, healing you.", 0, ch, 0, vict, TO_VICT);
    act("$n places $s hands on $N, healing $M.", TRUE, ch, 0, vict, TO_NOTVICT);
  } else {
    send_to_char("You place your hands on yourself, healing you.\r\n", ch);
    act("$n places $s hands on $mself, healing $m.", TRUE, ch, 0, 0, TO_ROOM);
  }

  ch->player.time.last_lay_hands = time(0);
  return;
}

ACMD(do_stance)
{
  int hours = 0, i;
  bool accum_affect = FALSE, accum_duration = FALSE;


  if (!GET_SKILL(ch, SKILL_STANCE))
  {
    send_to_char("You don't know how to do that.\r\n", ch);
    return;
  }

   if (GET_POS(ch) == POS_FISHING || GET_POS(ch) == POS_DIGGING || GET_POS(ch) ==  POS_RIDING) {
     send_to_char("You are not in a proper position for that!\r\n", ch);
    return;
}

 if (FIGHTING(ch)) {
    send_to_char("But you are fighting!\r\n", ch);
    return;
  }


if (GET_LEVEL(ch) == 15)
hours = 1800;
else if (GET_LEVEL(ch) == 18)
hours = 900;
else if (GET_LEVEL(ch) == 21)
hours = 600;
else if (GET_LEVEL(ch) == 24)
hours = 600;
else if (GET_LEVEL(ch) == 27)
hours = 400;
else if (GET_LEVEL(ch) > 27)
hours = 361;

  if ((time(0) - ch->player.time.last_stance) < hours)
  {
    send_to_char("You still need to wait awhile before you can use this skill!\r\n", ch);
    return;
  }

  if(!check_skill(ch, 101, SKILL_STANCE))
  {
    send_to_char("You attempt to focus, but you lose your concentration.", ch);
    ch->player.time.last_stance = time(0);
    return;
  }

struct affected_type af[3];

   for (i = 0; i < 3; i++) {
    af[0].type     = SPELL_DONTUSEME;
    af[0].location = APPLY_HITROLL;
    af[0].modifier = 2;
    af[0].duration = 10;
    af[0].bitvector = AFF_STANCE;

    af[1].type      = SPELL_DONTUSEME;
    af[1].location = APPLY_AC;
    af[1].modifier = -50;
    af[1].duration = 10;
    af[1].bitvector = AFF_STANCE;
 
    af[2].type      = SPELL_DONTUSEME;
    af[2].location = APPLY_STR;
    af[2].modifier = 2;
    af[2].duration = 10;
    af[2].bitvector = AFF_STANCE;


      if (af[i].bitvector || (af[i].location != APPLY_NONE)) {
        affect_join(ch, af+i, accum_duration, FALSE, accum_affect, FALSE);
      }
    }
  send_to_char("You take a defensive stance.\r\n", ch);
  act("$n takes a defensive stance.", TRUE, ch, 0, 0, TO_ROOM);

  ch->player.time.last_stance = time(0);
  return;
}


ACMD(do_chakra)
{
  int amount; //, ft;
 
  if (!GET_SKILL(ch, SKILL_CHAKRA))
  {
    send_to_char("You don't know how to do that.\r\n", ch);
    return;
  }

   if (GET_POS(ch) == POS_FISHING || GET_POS(ch) == POS_DIGGING || GET_POS(ch) ==  POS_RIDING) {
     send_to_char("You are not in a proper position for that!\r\n", ch);
    return;
}

 
// 600 seconds = 10 minutes = 8 ticks = 8 game hours
  if ((time(0) - ch->player.time.last_chakra) < 600)
  {
    send_to_char("You can only use this skill every eight hours.\r\n", ch);
    return;
  }
 
  if(!check_skill(ch, 101, SKILL_CHAKRA))
  {
    send_to_char("You attempt to focus, but you lose your concentration.", ch);
    ch->player.time.last_chakra = time(0);
    return;
  }

  //ft = GET_MOVE(ch);
  GET_MOVE(ch) = GET_MOVE(ch) / 2;
 
  //amount = ft * 2;
  amount = GET_MAX_HIT(ch) / 2;
 
  GET_HIT(ch) = MIN(GET_HIT(ch) + amount, GET_MAX_HIT(ch));
 
  send_to_char("You concentrate your inner power, and no longer feel your wounds.\r\n", ch);
  act("$n focuses, and appears refreshed.", TRUE, ch, 0, 0, TO_ROOM);
 
  ch->player.time.last_chakra = time(0);
  return;
}


ACMD(do_assess)
{
  char arg2[100];
  int value, offset, value2;
  bool is_weapon = FALSE, is_armor = FALSE;
  struct obj_data *obj;
  struct obj_data *obj2 = NULL;
  //char plusses[5];
  //int num_plus = 0;
  //int i = 0;

  const char *weap_mesg[] =
  { "minimal", "nominal", "serious", "critical", "massive"};
  const char *armor_mesg[] =
  { "very light", "light", "moderate", "heavy", "very heavy"};


  if (!GET_SKILL(ch, SKILL_ASSESS))
  {
    send_to_char("Your assessment isn't very revealing.\r\n", ch);
    mudlog("SYSERR: ch without skill_assess in do_assess", BRF, LVL_GRGOD, 1);
    return;
  }

  two_arguments(argument, arg, arg2);

  if (!*arg || !(obj = get_obj_in_list_vis(ch, arg, ch->carrying)))
  {
    send_to_char("Assess what?\r\n", ch);
    return;
  }

  if (*arg2 && !(obj2 = get_obj_in_list_vis(ch, arg2, ch->carrying)))
  {
    send_to_char("Assess it against what?\r\n", ch);
    return;
  }

  if (GET_OBJ_TYPE(obj) == ITEM_WEAPON)
    is_weapon = TRUE;
  else if (GET_OBJ_TYPE(obj) == ITEM_ARMOR)
    is_armor = TRUE;
  else
  {
    send_to_char("Your assessment reveals nothing.\r\n", ch);
    return;
  }

  if (GET_OBJ_LEVEL(obj) > MAX(1, GET_LEVEL(ch) + (GET_SKILL(ch, SKILL_ASSESS) / 10) - 2)) {
    send_to_char("Your assessment reveals nothing.\r\n", ch);
    return;
  }

  if (*arg2 && obj && obj2) {

    if (GET_OBJ_LEVEL(obj2) > MAX(1, GET_LEVEL(ch) + (GET_SKILL(ch, SKILL_ASSESS) / 10) - 2)) {
      send_to_char("Your assessment reveals nothing.\r\n", ch);
      return;
    }

    if (is_weapon) {
      value = ((GET_OBJ_VAL(obj, 1) * GET_OBJ_VAL(obj, 2)) + 1) * 100 / 2;
      value2 = ((GET_OBJ_VAL(obj2, 1) * GET_OBJ_VAL(obj2, 2)) + 1) * 100 / 2;
    }
    else {
      value = GET_OBJ_VAL(obj, 0);
      value2 = GET_OBJ_VAL(obj2, 0);
    }

    if (value > value2) {
       sprintf(buf, "%s seems to be better in quality than %s.\r\n", obj->short_description, obj2->short_description);
       send_to_char(buf, ch);
    }
    else if (value2 > value) {
       sprintf(buf, "%s seems to be better in quality than %s.\r\n", obj2->short_description, obj->short_description);
       send_to_char(buf, ch);
    }
    else {
       sprintf(buf, "%s seems to be about the same in quality as %s.\r\n", obj->short_description, obj2->short_description);
       send_to_char(buf, ch);
    }
    value = check_skill(ch, 0, SKILL_ASSESS);  // just for skill improvement
    return;
  }


  offset = (100 - GET_SKILL(ch, SKILL_ASSESS)) / 10;
  offset = number(-offset, offset);

  if (is_weapon)
  {
    value = ((GET_OBJ_VAL(obj, 1) + 1) * GET_OBJ_VAL(obj, 2)) / 2;
    value = (value - 1) / 4;
    value = MAX(0, MIN(4, value));
    sprintf(buf, "Damage: %s\r\n", weap_mesg[value]);

  }
  else
  {
    value = GET_OBJ_VAL(obj, 0);
    value = (value - 1) / 3;
    value = MAX(0, MIN(4, value));
    sprintf(buf, "Armor class: %s\r\n", armor_mesg[value]);
  }

  send_to_char(buf, ch);

  if (is_weapon)
    send_to_char("Guide: minimal < nominal < serious < critical < massive\r\n", ch);
  else
    send_to_char("Guide: very light < light < moderate < heavy < very heavy\r\n", ch);

  value = check_skill(ch, 0, SKILL_ASSESS);  // just for skill improvement
  return;
}


ACMD(do_assess_magic)
{
  int value;
  struct obj_data *obj;

  if (!GET_SKILL(ch, SKILL_ASSESS_MAGIC))
  {
    send_to_char("Your assessment isn't very revealing.\r\n", ch);
    mudlog("SYSERR: assess_magic entry without the skill", BRF, LVL_GRGOD, 1);
    return;
  }

  one_argument(argument, arg);

  if (!*arg || !(obj = get_obj_in_list_vis(ch, arg, ch->carrying)))
  {
    send_to_char("Assess what?\r\n", ch);
    return;
  }

  switch (GET_OBJ_TYPE(obj))
  {
    case ITEM_SCROLL:
    case ITEM_POTION:
      value = 0;
      if (GET_OBJ_VAL(obj, 1))
        value++;
      if (GET_OBJ_VAL(obj, 2))
        value++;
      if (GET_OBJ_VAL(obj, 3))
        value++;

      switch (value)
      {
        case 0:
          act("You sense nothing within $p.", FALSE, ch, obj, 0, TO_CHAR);
          return;
        case 1:
          act("You sense an aura of magic within $p.", 0, ch, obj, 0, TO_CHAR);
          break;
        case 2:
        case 3:
          act("You sense several magical auras in $p.", 0, ch, obj, 0, TO_CHAR);
          break;
        default:
          return;
      }
      break;

    case ITEM_WAND:
    case ITEM_STAFF:
      if (GET_OBJ_VAL(obj, 2) == 0)
      {
        act("$p seems to be powerless.", FALSE, ch, obj, 0, TO_CHAR);
        return;
      }

      if (GET_OBJ_VAL(obj, 2) == GET_OBJ_VAL(obj, 3))
        act("$p does not appear to have been used.", 0, ch, obj, 0, TO_CHAR);
      else
        act("$p has been used, but you sense magic.", 0, ch, obj, 0, TO_CHAR);
      break;

    default:
      act("You examine $p, but sense nothing.", 0, ch, obj, 0, TO_CHAR);
      return;
  }

  value = number(0, 100 - GET_SKILL(ch, SKILL_ASSESS_MAGIC));
  value /= 4;
  if (number(0, 1))
    value = -value;

  value += GET_OBJ_VAL(obj, 0);
  value = MAX(1, MIN(40, value));

  if (value <= 10)
    send_to_char("The magic seems to be reasonably faint.\r\n", ch);
  else if (value <= 20)
    send_to_char("The magic seems moderately strong.\r\n", ch);
  else if (value <= 30)
    send_to_char("The magic seems fairly powerful.\r\n", ch);
  else if (value <= 40)
    send_to_char("The magic seems to be exceptionally strong.\r\n", ch);

  value = use_skill(ch, 101, SKILL_ASSESS_MAGIC);  // for learning only
  return;
}


ACMD(do_parse_assess)
{
  if (GET_SKILL(ch, SKILL_ASSESS))
    do_assess(ch, argument, cmd, subcmd);
  else if (GET_SKILL(ch, SKILL_ASSESS_MAGIC))
    do_assess_magic(ch, argument, cmd, subcmd);
  else
    send_to_char("Your assessment isn't very revealing.", ch);

  return;
}


ACMD(do_break)
{
  if (!affected_by_spell(ch, SPELL_STATUE))
  {
    send_to_char("But you aren't even a statue to begin with!\r\n", ch);
    return;
  }

  affect_from_char(ch, SPELL_STATUE);

  send_to_char("Your body returns to normal.\r\n", ch);
  act("$n suddenly reverts to normal as $e breaks the statue spell.",
       TRUE, ch, 0, 0, TO_ROOM);

  return;
}


ACMD(do_relax)
{
  if (!AFF_FLAGGED(ch, AFF_STANCE))
  {
    send_to_char("But you aren't even in a defensive stance to begin with!\r\n", ch);
    return;
  }

  affect_from_char_II(ch, SKILL_STANCE, SPELL_DONTUSEME, 1);

  send_to_char("You relax your muscles.\r\n", ch);
  act("$n relaxes $s muscles.", TRUE, ch, 0, 0, TO_ROOM);

  return;
}


ACMD(do_subdue)
{
   if(PLR_FLAGGED(ch, PLR_SUBDUING))
   {
      REMOVE_BIT(PLR_FLAGS(ch), PLR_SUBDUING);
      send_to_char("You will no longer fight to subdue.\r\n", ch);
      return;
   }
   SET_BIT(PLR_FLAGS(ch), PLR_SUBDUING);
   send_to_char("You will now fight to subdue.\r\n", ch);
   return;
}


ACMD(do_skin)
{
    char arg1[MAX_INPUT_LENGTH];
    struct obj_data *cont, *obj;
    int temp, i, found=0;
    int percent;
    struct obj_data *jj, *next_thing2;
    one_argument(argument, arg1);

   
  if (!GET_SKILL(ch, SKILL_SKIN))
  {
    act("You are unfamiliar with this skill!", FALSE, ch, 0, 0, TO_CHAR);
    return;
  }

 if (GET_POS(ch) == POS_FISHING || GET_POS(ch) == POS_DIGGING || GET_POS(ch) ==  POS_RIDING) {
     send_to_char("You are not in a proper position for that!\r\n", ch);
    return;
}


    if (!*arg1)
    { send_to_char("Skin what?\r\n", ch);
      return;
    }
 /* need to check to make sure they are trying to skin a corpse */
    if(strcmp(arg1, "corpse"))
    { send_to_char("Just skin corpses.\r\n", ch);
      return;
    }
 /* need to make sure they are wielding an object before checking its type */
    if(!GET_EQ(ch, WEAR_WIELD))
    { send_to_char("Not with your bare hands.\r\n", ch);
      return;
    } 
 /* of course if you have races, maybe clawed races could skin corpses
     without a weapon */
 /* You may want to change this to some type of cutting weapon. */
    if(GET_OBJ_TYPE(GET_EQ(ch, WEAR_WIELD)) != ITEM_WEAPON)
    { send_to_char("Can't skin with that!\r\n", ch);
      return; 
    }
    if((cont = get_obj_in_list_vis(ch, &arg1[0],world[ch->in_room].contents)))
      found = 1;
         
    if (found)
    {
  
      percent = number(1, 101);  
   
      if ((!use_skill(ch, percent, SKILL_SKIN)) || (number(0, GET_SKILL(ch, SKILL_SKIN))) < 10)
      {
 /* Don't bother imms with skill checks */
      if(GET_LEVEL(ch) < LVL_IMMORT)
      {
        send_to_char("You crudely hack at the corpse awhile.\r\n", ch);
        act("$n spends some time crudely hacking at the corpse.", TRUE, ch, 0,
          0, TO_ROOM);
        return;
      }
     }
   found = 0;
   for(i = 0; i < 4; i++)
   {
   if(cont->obj_flags.skin_data[i] == 0)
     continue;
/* Note: you can't use object vnum 0 with this. */
   if((temp = real_object(cont->obj_flags.skin_data[i])) > 0)
    {   
    found = 1;
    obj = read_object( temp, REAL);

    if (IS_CARRYING_N(ch) >= CAN_CARRY_N(ch)) {
    act("You skin $p from the corpse and put it down.", TRUE, ch, obj, 0, TO_CHAR);
    act("$n skins $p from the corpse and puts it down.", TRUE, ch, obj, 0, TO_ROOM);
    obj_to_room(obj, ch->in_room);
    }

    else if ((IS_CARRYING_W(ch) + GET_OBJ_WEIGHT(obj)) > CAN_CARRY_W(ch)) {
    act("You skin $p from the corpse and put it down.", TRUE, ch, obj, 0, TO_CHAR);
    act("$n skins $p from the corpse and puts it down.", TRUE, ch, obj, 0, TO_ROOM);

    obj_to_room(obj, ch->in_room);
  }
    else {
    obj_to_char(obj, ch);
    act("You skin $p from the corpse!", TRUE, ch, obj, 0, TO_CHAR);
    act("$n skins $p from the corpse!", TRUE, ch, obj, 0, TO_ROOM);
     }
    }
   }
   if(!found)
   {
   act("You spend some time crudely hacking at the corpse.", TRUE, ch, 0,
     0, TO_CHAR);
   act("$n spends some time crudely hacking at the corpse.", TRUE, ch, 0,
      0, TO_ROOM);
   }

   for (jj = cont->contains; jj; jj = next_thing2) {
          next_thing2 = jj->next_content;       /* Next in inventory */
          obj_from_obj(jj);

          if (cont->in_obj) {
            obj_to_obj(jj, cont->in_obj);
          } else if (cont->carried_by) {
            obj_to_room(jj, cont->carried_by->in_room);
          } else if (cont->in_room != NOWHERE) {
            obj_to_room(jj, cont->in_room);
          } else {
            assert(FALSE);
          }
        }
   extract_obj(cont);
   return; 
 }
send_to_char("Just skin corpses ok?\r\n", ch);
}


void assign_skin_value(struct char_data *ch, struct obj_data *corpse)
{ int i;
/* DON'T pass  PC to this routine! */
for(i = 0; i < 4; i++)
 corpse->obj_flags.skin_data[i] = ch->mob_specials.skin_data[i];
return;
}


ACMD(do_douse)
{
    struct obj_data *fire;
    int d_num, douse_num;
    struct obj_data *douse;
    one_argument(argument, buf);

    if (!*buf) {
      send_to_char("Douse what?\r\n", ch);
    } else {
        fire = get_obj_in_list_vis(ch, buf, world[ch->in_room].contents);

//        if (fire && GET_OBJ_VNUM(fire) == VNUM_FIRE) {
        if (fire && GET_OBJ_VNUM(fire) == VNUM_FIRE) {
            send_to_char("You douse the fire.\r\n", ch);
            act("$n douses the fire.", FALSE, ch, 0, 0, TO_ROOM);
            extract_obj(fire);
            douse_num = 2;
            d_num = real_object(douse_num);
            douse = read_object(d_num, REAL);
            obj_to_room(douse, ch->in_room);
        } else  {
            send_to_char("You can't douse that.\r\n", ch);
        }
    }
}


ACMD(do_build_fire) {
    struct obj_data *fire;
    int burn, percent;
    struct obj_data *firewood;
    //one_argument(argument, arg);

    percent = number(1, 101);

//    if (!*arg) {
//        send_to_char("Build fire with what?\r\n", ch);
//        return;
//    }

    if (GET_POS(ch) == POS_FISHING || GET_POS(ch) == POS_DIGGING || GET_POS(ch) == POS_RIDING) {
        send_to_char("You are not in a proper position for that!\r\n", ch);
        return;
    }

    if (GET_SKILL(ch, SKILL_BUILD_FIRE) == 0) {
        send_to_char("That skill is unfamiliar to you.\r\n", ch);
        return;
    }

    if ((SECT(ch->in_room) == SECT_INSIDE) || (SECT(ch->in_room) == SECT_CITY)) {
        send_to_char("You cannot build a fire here!\r\n", ch);
        return;
    }

    if ((SECT(ch->in_room) == SECT_WATER_SWIM) || (SECT(ch->in_room) == SECT_WATER_NOSWIM)) {
        send_to_char("How can you build a fire in water?\r\n", ch);
        return;
    }

    if (SECT(ch->in_room) == SECT_UNDERWATER) {
        send_to_char("You cannot build a fire underwater!\r\n", ch);
        return;
    }

//    if (!(firewood = get_obj_in_list_vis(ch, arg, ch->carrying))) {
//        sprintf(buf, "You don't seem to have %s %s.\r\n", AN(arg), arg);
//        send_to_char(buf, ch);
//        return;
//    } else {
//
//        if (GET_OBJ_TYPE(firewood) != ITEM_FIREWOOD) {
//            send_to_char("You want to build a fire with that?!!\r\n", ch);
//            return ;
//        } else if (!use_skill(ch, percent, SKILL_BUILD_FIRE)) {
//            burn = number(1, 10);
//
//            if (burn <= 4) {
//                send_to_char("You only succeed in burning your fingers. Ouch!\r\n", ch);
//                damage(ch, ch, dice(1,6), -1);
//                return;
//            } else {
//                sprintf(buf, "You fail to get the fire going with %s.\r\n", firewood->short_description);
//                act("$n fails to get the fire going with $p.\r\n",
//                FALSE, ch, firewood, 0, TO_ROOM);
//                send_to_char(buf, ch);
//                return;
//            }
//        }
//
//        fire = read_object(VNUM_FIRE, VIRTUAL);
//        GET_OBJ_TIMER(fire) = 12;
//        obj_to_room(fire, ch->in_room);
//        world[ch->in_room].light += 10;
//
//        act("You have built a fire from $p.", FALSE, ch, firewood, 0, TO_CHAR);
//        act("$n has built a fire from $p.", TRUE, ch, firewood, 0, TO_ROOM);
//
//        extract_obj(firewood);
//        return;
//    }

    if (!use_skill(ch, percent, SKILL_BUILD_FIRE)) {
        burn = number(1, 10);

        // 40% chance to burn yourself upon failing.
        if (burn <= 4) {
            send_to_char("You only succeed in burning your fingers. Ouch!\r\n", ch);
            damage(ch, ch, dice(1, 6), -1);
            return;
        } else {
            sprintf(buf, "You fail to get the fire going with %s.\r\n", firewood->short_description);
            act("$n fails to get the fire going with $p.\r\n",
            FALSE, ch, firewood, 0, TO_ROOM);
            send_to_char(buf, ch);
            return;
        }
    }

    fire = read_object(VNUM_FIRE, VIRTUAL);
    GET_OBJ_TIMER(fire) = 12;
    obj_to_room(fire, ch->in_room);

    act("You have built a fire.", FALSE, ch, 0, 0, TO_CHAR);
    act("$n has built a fire.", TRUE, ch, 0, 0, TO_ROOM);

    return;
}


ACMD(do_cook) 
{
  char arg1[MAX_INPUT_LENGTH];
  char arg2[MAX_INPUT_LENGTH];
  
  int burn, b_num, obj_dotmode = 0, burnt_num;
  struct obj_data *burnt;
  struct obj_data *fire;
  struct obj_data *food;

  two_arguments(argument, arg1, arg2);

 if (!*arg1) {
    send_to_char("Cook what with what?\r\n", ch);
    return;
}
 if (!*arg2) {
    sprintf(buf, "You need at least a fire to cook %s!\r\n",
            ((obj_dotmode == FIND_INDIV) ? "it" : "them"));
    send_to_char(buf, ch);
  return;
  }

  if (GET_POS(ch) == POS_FISHING || GET_POS(ch) == POS_DIGGING || GET_POS(ch) == POS_RIDING) {
     send_to_char("You are not in a proper position for that!\r\n", ch);
    return;
}
 
if (GET_SKILL(ch, SKILL_COOK) == 0)
    {
      send_to_char("That skill is unfamiliar to you.\r\n", ch);
      return ;
    }

  if ((SECT(ch->in_room) == SECT_WATER_SWIM) ||
     (SECT(ch->in_room) == SECT_WATER_NOSWIM))
    {
      send_to_char("How can you cook in water?\r\n", ch);
      return;
    }

  if(SECT(ch->in_room) == SECT_UNDERWATER)
    {
      send_to_char("You cannot cook underwater!\r\n", ch);
      return;
    }

 if (!(fire = get_obj_in_list_vis(ch, arg2, world[ch->in_room].contents))) {
    sprintf(buf, "There doesn't seem to be %s %s here.\r\n", AN(arg2), arg2);
    send_to_char(buf, ch);
    return;
  }

else { 
if (GET_OBJ_TYPE(fire) != ITEM_FIRE)  {
      send_to_char("You can't possibility cook with that!\n", ch);
      return ;
    }
else if (!(food = get_obj_in_list_vis(ch, arg1, ch->carrying))) {
          sprintf(buf, "You aren't carrying %s %s.\r\n", AN(arg1), arg1);
          send_to_char(buf, ch);
        }

else if (GET_OBJ_TYPE(food) != ITEM_RAW && GET_OBJ_TYPE(food) == ITEM_FOOD) {
          sprintf(buf, "But %s %s is already cooked!\r\n", AN(arg1), arg1);
          send_to_char(buf, ch);
          return;
        } 

else if (GET_OBJ_TYPE(food) != ITEM_RAW) {
          send_to_char("You can't cook that!\r\n", ch);
        } 

else {

if (!check_skill(ch, 101, SKILL_COOK))
    {
      burn = number(1, 10);
      if (burn <=4) {
      send_to_char("You only succeed in burning your fingers. Ouch!\r\n", ch);
      damage(ch, ch, dice(1,6), -1);
      return;
      }
      else {
       burnt_num = 12907;
       b_num = real_object(burnt_num);
       burnt = read_object(b_num, REAL);
       sprintf(buf, "Opps! Your poor cooking skills have turned your food into %s!\r\n", burnt->short_description);
      act("Opps! $n's poor cooking skills have turned the food into $p!\r\n",
        FALSE, ch, burnt, 0, TO_ROOM);
    send_to_char(buf, ch);
    extract_obj(food);
    obj_to_char(burnt, ch);
    return;
    }
}
  act("You have cooked $o to perfection!", FALSE, ch, food, 0, TO_CHAR);
  act("$n have cooked $p to perfection!", TRUE, ch, food, 0, TO_ROOM);

  GET_OBJ_TYPE(food) = ITEM_FOOD; 
  return;
}

}
}

char *disguise[] =
{
 "!RESERVED!",                 /* 0 - reserved */
 "beggar1",
 "beggar2",
 "beggar3",
 "beggar4",
 "knight1",
 "knight2",
 "knight3",
 "knight4",
 "mage1",
 "mage2",
 "cleric1",
 "cleric2",
 "cleric3",
 "cleric4",
 "monk1",
 "monk2",
 "courier1",
 "courier2",
 "bard1",
 "bard2",
 "bard3",
 "\n"                          /* the end */
};

int find_disguise_num(char *name)
{
  int index = 0, ok;
  char *temp, *temp2;
  char first[256], first2[256];

  while (*disguise[++index] != '\n') {
    if (is_abbrev(name, disguise[index]))
      return index;

    ok = 1;
    temp = any_one_arg(disguise[index], first);
    temp2 = any_one_arg(name, first2);
    while (*first && *first2 && ok) {
      if (!is_abbrev(first2, first))
        ok = 0;
      temp = any_one_arg(temp, first);
      temp2 = any_one_arg(temp2, first2);
    }

    if (ok && !*first2)
      return index;
  }

  return -1;
}


int mag_dis(struct char_data *ch, int item0, int item1, int item2, int item3)
{
  int j;
  struct obj_data *tobj;

 for (j = 0; j < NUM_WEARS; j++) {
    if (GET_EQ(ch, j)) {
     if ((item0 > 0) && (GET_OBJ_VNUM(GET_EQ(ch, j)) == item0)) {
     GET_OBJ_DISGUISE(GET_EQ(ch, j)) = 1;
      item0 = -1;
    } else if ((item1 > 0) && (GET_OBJ_VNUM(GET_EQ(ch, j)) == item1)) {
     GET_OBJ_DISGUISE(GET_EQ(ch, j)) = 1;
      item1 = -1;
    } else if ((item2 > 0) && (GET_OBJ_VNUM(GET_EQ(ch, j)) == item2)) {
     GET_OBJ_DISGUISE(GET_EQ(ch, j)) = 2;
      item2 = -1;
    }
  }
}

for (tobj = ch->carrying; tobj; tobj = tobj->next_content) {
    if ((item3 > 0) && (GET_OBJ_VNUM(tobj) == item3)) {
      item3 = -1;
    } 
  }

  if ((item0 > 0) || (item1 > 0) || (item2 > 0) || (item3 > 0)) {
       return (FALSE);
  }

  return (TRUE);
}

void mag_checkeq(struct char_data *ch, int item0, int item1, int item2)  {
  int j;

for (j = 0; j < NUM_WEARS; j++) {
    if (GET_EQ(ch, j)) {
     if ((item0 > 0) && (GET_OBJ_VNUM(GET_EQ(ch, j)) == item0)) {
     GET_OBJ_DISGUISE(GET_EQ(ch, j)) = 0;
    } else if ((item1 > 0) && (GET_OBJ_VNUM(GET_EQ(ch, j)) == item1)) {
     GET_OBJ_DISGUISE(GET_EQ(ch, j)) = 0;
    } else if ((item2 > 0) && (GET_OBJ_VNUM(GET_EQ(ch, j)) == item2)) {
     GET_OBJ_DISGUISE(GET_EQ(ch, j)) = 0;
    }
  }
}
}

void mag_rem(struct char_data *ch, int item0, int item1, int item2)  {
  int j;

 REMOVE_BIT(PRF_FLAGS(ch), PRF_NOTSELF);
 REMOVE_BIT(PRF_FLAGS(ch), PRF_DISGUISE);


 for (j = 0; j < NUM_WEARS; j++) {
    if (GET_EQ(ch, j)) {
     if ((item0 > 0) && (GET_OBJ_VNUM(GET_EQ(ch, j)) == item0)) {
       GET_OBJ_DISGUISE(GET_EQ(ch, j)) = 0;
    } else if ((item1 > 0) && (GET_OBJ_VNUM(GET_EQ(ch, j)) == item1)) {
       GET_OBJ_DISGUISE(GET_EQ(ch, j)) = 0;
    } else if ((item2 > 0) && (GET_OBJ_VNUM(GET_EQ(ch, j)) == item2)) {
       GET_OBJ_DISGUISE(GET_EQ(ch, j)) = 0;
    }
  }
}
  
}


int has_eq(struct char_data * ch, int disguise)
{
  switch (disguise) {
    case BEGGAR_1:
    case BEGGAR_2:
    case BEGGAR_3:
    case BEGGAR_4:
      return (mag_dis(ch, 98, 0, 99, 0));
      break;
    case KNIGHT_1:
     case KNIGHT_2:
     case KNIGHT_3:
     case KNIGHT_4:
       return (mag_dis(ch, 97, 0, 95, 96));
       break;
    case MAGE_1:
    case MAGE_2:
       return (mag_dis(ch, 94, 93, 99, 0));
       break;
    case CLERIC_1:
     case CLERIC_2:
      case CLERIC_3:
      case CLERIC_4:
       return (mag_dis(ch, 25, 0, 99, 0));
       break;
    case MONK_1:
    case MONK_2:
        return (mag_dis(ch, 92, 0, 99, 0));
       break;
    case COURIER_1:
    case COURIER_2:
        return (mag_dis(ch, 90, 0, 99, 0));
       break;
    case BARD_1:
    case BARD_2:
     case BARD_3:
        return (mag_dis(ch, 91, 0, 99, 0));
       break;
     }
  return 0;
}
    
/*
ACMD(do_disguise)
{
   //int thing, i;
   int disnum;
   char disguise_name[MAX_INPUT_LENGTH];
   struct char_data *mob = NULL;
   struct char_data *target = NULL, *next_target;

  one_argument(argument, disguise_name);

   if (GET_POS(ch) == POS_FISHING || GET_POS(ch) == POS_DIGGING || GET_POS(ch) ==  POS_RIDING) {
     send_to_char("You are not in a proper position for that!\r\n", ch);
    return;
}


  if (!*disguise_name) {
    send_to_char("Disguise yourself as what?\r\n", ch);
    return;
  }

  if (PRF_FLAGGED(ch, PRF_NOTSELF)) { 
     send_to_char("Perhaps you should reveal yourself first?\r\n", ch);
     return;
  }

    if (!GET_SKILL(ch, SKILL_DISGUISE))
  {
    send_to_char("You are unskilled in the art of disguising.\r\n", ch);
    return;
  }


disnum = find_disguise_num(disguise_name);

if ((disnum < 1) || (disnum > MAX_DIS)) {
send_to_char("Disguise yourself as what?!?\r\n", ch);
return;
}

 if (!has_eq(ch, disnum)) {
  send_to_char("You dont seem to be suitably equipped for this disguise!\r\n", ch);
  return;
  }


 free(ch->char_specials.name_dis);
 ch->char_specials.name_dis = (char*) NULL;
 free(ch->char_specials.desc_dis);
 ch->char_specials.desc_dis = (char*) NULL;
 free(ch->char_specials.ldesc_dis);
 ch->char_specials.ldesc_dis = (char*) NULL;

switch (disnum) {
case 1:
     sprintf(buf, "beggar");
     SET_BIT(PLR_FLAGS(ch), PLR_BEGGAR);
     mob = read_mobile(2813, VIRTUAL);
     char_to_room(mob, 0);
     ch->char_specials.name_dis = str_dup("a bedraggled old beggar");
     ch->char_specials.desc_dis = str_dup("&yA bedraggled looking beggar sits here, wrapped in a thin patched blanket.");
     break;
case 2:
     sprintf(buf, "beggar");
     SET_BIT(PLR_FLAGS(ch), PLR_BEGGAR);
      mob = read_mobile(10004, VIRTUAL);
      char_to_room(mob, 0);
      free(ch->char_specials.name_dis);
     ch->char_specials.name_dis =  str_dup("a beggar");
     free(ch->char_specials.desc_dis);
     ch->char_specials.desc_dis =  str_dup("&yA beggar asks you to spare a copper.");
     break;
case 3:
     sprintf(buf, "beggar");
     SET_BIT(PLR_FLAGS(ch), PLR_BEGGAR);
      mob = read_mobile(8018, VIRTUAL);
      char_to_room(mob, 0);
     free(ch->char_specials.name_dis);
     ch->char_specials.name_dis = str_dup("the drunken beggar");
     free(ch->char_specials.desc_dis);
     ch->char_specials.desc_dis = str_dup("&yA beggar lies slumped against the wall in a half-conscious state.");
     break;
case 4:
     sprintf(buf, "beggar");
     SET_BIT(PLR_FLAGS(ch), PLR_BEGGAR);
      mob = read_mobile(8019, VIRTUAL);
      char_to_room(mob, 0);
        free(ch->char_specials.name_dis);
     ch->char_specials.name_dis = str_dup("the wide-eyed beggar");
     free(ch->char_specials.desc_dis);
     ch->char_specials.desc_dis = str_dup("&yA wide-eyed beggar with a rusty cup meekly beseeches the crowd for change.");
      break;
case 5:
     sprintf(buf, "knight");
     SET_BIT(PLR_FLAGS(ch), PLR_KNIGHT);
      mob = read_mobile(10015, VIRTUAL);
      char_to_room(mob, 0);
        free(ch->char_specials.name_dis);
     ch->char_specials.name_dis = str_dup("a somber-looking knight");
     free(ch->char_specials.desc_dis);
     ch->char_specials.desc_dis = str_dup("&yA somber-looking knight patrols the streets of Kalaman, lending his aid to the local garrison.");
      break;

case 6:
     sprintf(buf, "knight");
     SET_BIT(PLR_FLAGS(ch), PLR_KNIGHT);
      mob = read_mobile(1940, VIRTUAL);
      char_to_room(mob, 0);
      free(ch->char_specials.name_dis);
     ch->char_specials.name_dis = str_dup("a knight patrolling Palanthas");
     free(ch->char_specials.desc_dis);
     ch->char_specials.desc_dis = str_dup("&yA sword knight on patrol duty walks the streets.");
    break;
case 7:
     sprintf(buf, "knight");
     SET_BIT(PLR_FLAGS(ch), PLR_KNIGHT);
      mob = read_mobile(11421, VIRTUAL);
          char_to_room(mob, 0);
       free(ch->char_specials.name_dis);
     ch->char_specials.name_dis = str_dup("a patrolling knight");
     free(ch->char_specials.desc_dis);
     ch->char_specials.desc_dis = str_dup("&yA stern-looking knight patrols the area.");
     break;
case 8:
     sprintf(buf, "knight");
     SET_BIT(PLR_FLAGS(ch), PLR_KNIGHT);
       mob = read_mobile(11814, VIRTUAL);
        char_to_room(mob, 0);
      free(ch->char_specials.name_dis);
     ch->char_specials.name_dis = str_dup("a patrolling knight");
     free(ch->char_specials.desc_dis);
     ch->char_specials.desc_dis = str_dup("&yA patrolling knight walks through the city with a watchful eye.");
     break;
case 9:
     sprintf(buf, "mage");
     SET_BIT(PLR_FLAGS(ch), PLR_MAGE);
     mob = read_mobile(6131, VIRTUAL);
     char_to_room(mob, 0);
     ch->char_specials.name_dis = str_dup("a novice");     
     ch->char_specials.desc_dis = str_dup("&yA novice wanders the area, apparently deep in thought.");
     break;
case 10:
     sprintf(buf, "mage");
     SET_BIT(PLR_FLAGS(ch), PLR_MAGE);
     mob = read_mobile(6139, VIRTUAL);
     char_to_room(mob, 0);
     ch->char_specials.name_dis = str_dup("a young novice");     
     ch->char_specials.desc_dis = str_dup("&y A young novice wanders the area here, going about his business.");
     break;
case 11:     
     sprintf(buf, "cleric");
     SET_BIT(PLR_FLAGS(ch), PLR_CLERIC);
       mob = read_mobile(10836, VIRTUAL);
      char_to_room(mob, 0);
     ch->char_specials.name_dis = str_dup("a wandering missionary");     
     ch->char_specials.desc_dis = str_dup("&yA wandering missionary roams about the area, seeking refuge for the night.");
     break;
case 12:
     sprintf(buf, "cleric");
     SET_BIT(PLR_FLAGS(ch), PLR_CLERIC);
        mob = read_mobile(1912, VIRTUAL);    
       char_to_room(mob, 0);
       ch->char_specials.name_dis = str_dup("an acolyte");    
        ch->char_specials.desc_dis = str_dup("&yAn acolyte from the church of Paladine stands here preaching.");
      break;
case 13:
     sprintf(buf, "cleric");
     SET_BIT(PLR_FLAGS(ch), PLR_CLERIC);
       mob = read_mobile(11225, VIRTUAL);
        char_to_room(mob, 0); 
 ch->char_specials.name_dis = str_dup("an acolyte");     
ch->char_specials.desc_dis = str_dup("&yAn acolyte walks around the temple gathering donations.");
     break;
 case 14:
     sprintf(buf, "cleric");
     SET_BIT(PLR_FLAGS(ch), PLR_CLERIC);
      mob = read_mobile(402, VIRTUAL);   
       char_to_room(mob, 0); 
ch->char_specials.name_dis = str_dup("a dark cleric");     
  ch->char_specials.desc_dis = str_dup("&yA dark cleric stands here, absentmindedly fingering his pendant.");
      break;
case 15:
     sprintf(buf, "monk");
     SET_BIT(PLR_FLAGS(ch), PLR_MONK);
    mob = read_mobile(2588, VIRTUAL); 
     char_to_room(mob, 0); 
  ch->char_specials.name_dis = str_dup("a wandering monk");     
  ch->char_specials.desc_dis = str_dup("&yA wandering monk walks here, lost in thought.");
     break;
case 16:
     sprintf(buf, "monk");
     SET_BIT(PLR_FLAGS(ch), PLR_MONK);
     mob = read_mobile(2525, VIRTUAL); 
        char_to_room(mob, 0); 
 ch->char_specials.name_dis = str_dup("a female monk");     
ch->char_specials.desc_dis = str_dup("&yA female monk walks the area here, running a windmill drum through her hands.");
break;
case 17:
     sprintf(buf, "young courier");
     SET_BIT(PLR_FLAGS(ch), PLR_COURIER);      
     mob = read_mobile(8750, VIRTUAL);
       char_to_room(mob, 0); 
     ch->char_specials.name_dis = str_dup("a young courier");    
 ch->char_specials.desc_dis =  str_dup("&yA young courier is here, running messages to their destinations.");
break;
case 18:
     sprintf(buf, "female courier");
     SET_BIT(PLR_FLAGS(ch), PLR_COURIER);
     mob = read_mobile(8665, VIRTUAL);
       char_to_room(mob, 0);
     ch->char_specials.name_dis = str_dup("a female courier");   
 ch->char_specials.desc_dis =  str_dup("&yA female courier is here, a searchful gaze upon her expression.");
break;
case 19:
     sprintf(buf, "bard");
     SET_BIT(PLR_FLAGS(ch), PLR_BARD);     
     mob = read_mobile(8601, VIRTUAL);
     char_to_room(mob, 0);
     ch->char_specials.name_dis = str_dup("a dignified and elegant bard");     
     ch->char_specials.desc_dis = str_dup("&yA dignified and elegant bard motions for you to come closer.");

break;
case 20:     
     sprintf(buf, "bard");
     SET_BIT(PLR_FLAGS(ch), PLR_BARD);
       mob = read_mobile(8202, VIRTUAL);
     char_to_room(mob, 0);
     ch->char_specials.name_dis = str_dup("a travelling minstrel");     
     ch->char_specials.desc_dis = str_dup("&yA minstrel walks along the highway, earning his income with a song or two.");
break;
case 21:
     sprintf(buf, "bard");
     SET_BIT(PLR_FLAGS(ch), PLR_BARD);
      mob = read_mobile(8600, VIRTUAL);
     char_to_room(mob, 0);
     ch->char_specials.name_dis = str_dup("a brightly dressed minstrel");     
     ch->char_specials.desc_dis = str_dup("&yA brightly dressed minstrel hums to himself.");
     break;  
   }
SET_BIT(PRF_FLAGS(ch), PRF_NOTSELF);
SET_BIT(PRF_FLAGS(ch), PRF_DISGUISE);
ch->char_specials.ldesc_dis = str_dup(mob->player.description);
ch->char_specials.sex_dis = mob->player.sex;
extract_char(mob); 

for (target = world[ch->in_room].people; target; target = next_target) {
    next_target = target->next_in_room;
if (target == ch) {
   sprintf(buf2, "You have disguised yourself as %s!", buf);
  send_to_char(buf2, ch);
}
else {
 GET_NAME_II(ch, target, chname);
 sprintf(buf2, "%s has disguised $mself as %s!", chname, buf);
 FREE_NAME(chname);
 act(buf2, FALSE, ch, 0, target, TO_VICT);

}
}
}

void reveal(struct char_data *ch) {

if (PLR_FLAGGED(ch, PLR_MAGE)) {
      REMOVE_BIT(PLR_FLAGS(ch), PLR_MAGE);
       mag_rem(ch, 94, 93, 99);
}
if (PLR_FLAGGED(ch, PLR_MONK)) {
      REMOVE_BIT(PLR_FLAGS(ch), PLR_MONK);
       mag_rem(ch, 92, 0, 99);
}
if (PLR_FLAGGED(ch, PLR_KNIGHT)) {
      REMOVE_BIT(PLR_FLAGS(ch), PLR_KNIGHT);
       mag_rem(ch, 97, 0, 95);
}
if (PLR_FLAGGED(ch, PLR_CLERIC)) {
       REMOVE_BIT(PLR_FLAGS(ch), PLR_CLERIC);
       mag_rem(ch, 25, 0, 99);
}
if (PLR_FLAGGED(ch, PLR_BARD)) {
       REMOVE_BIT(PLR_FLAGS(ch), PLR_BARD);
       mag_rem(ch, 91, 0, 99);
}
if (PLR_FLAGGED(ch, PLR_BEGGAR)) {
       REMOVE_BIT(PLR_FLAGS(ch), PLR_BEGGAR);
       mag_rem(ch, 98, 0, 99);
}

if (PLR_FLAGGED(ch, PLR_COURIER)) {
       REMOVE_BIT(PLR_FLAGS(ch), PLR_COURIER);
       mag_rem(ch, 90, 0, 99);
}
return;
}

ACMD(do_reveal) 
{

 if (!PRF_FLAGGED(ch, PRF_NOTSELF)) { 
     send_to_char("But you are not disguised as anything!\r\n", ch);
     return;
  }

else if (PRF_FLAGGED(ch, PRF_NOTSELF) && PRF_FLAGGED(ch, PRF_DISGUISE)) {
    reveal(ch);
}

else if (PRF_FLAGGED(ch, PRF_NOTSELF) && !PRF_FLAGGED(ch, PRF_DISGUISE)) {
affect_from_char_II(ch, SPELL_POLYMORPH, SPELL_POLYMORPH, 2);
}

 send_to_char("You are transformed back to your old self.\r\n", ch);
  act("$n is transformed back to $s old self.", TRUE, ch, 0, 0, TO_ROOM);


}

ACMD(do_detectdisguise)
{
  struct char_data *vict;
  
  int percent;

  one_argument(argument, arg);

  if (GET_SKILL(ch, SKILL_DETECTDISGUISE) < 1)
  {
    send_to_char("Your perception is not keen enough!\r\n", ch);
    return;
  }

   if (IS_AFFECTED(ch, AFF_BLIND))
  {
    send_to_char("But you can't see a thing!\r\n", ch);
    return;
  }

 if (!*arg || !(vict = get_char_vis(ch, arg, FIND_CHAR_ROOM)))
  {
    send_to_char("Whose disguise do you want to see through?\r\n", ch);
    return;
  }

if (!PLR_FLAGGED(vict, PLR_KNIGHT) && !PLR_FLAGGED(vict, PLR_MAGE) && !PLR_FLAGGED(vict, PLR_MONK) && !PLR_FLAGGED(vict, PLR_CLERIC) && !PLR_FLAGGED(vict, PLR_BARD) && !PLR_FLAGGED(vict, PLR_BEGGAR)&& !PLR_FLAGGED(vict, PLR_COURIER)){
    send_to_char("But they are not in disguise!\r\n", ch);
    return;
  }


  if (vict == ch) {
    send_to_char("Aren't we funny today...\r\n", ch);
    return;
  }

if (GET_LEVEL(vict) > MIN(LVL_IMMORT - 1, GET_LEVEL(ch) + 3)) {
    percent = 101;
}

  percent = number(1, 101) - (GET_WIS(ch) * 2);

 if (!use_skill(ch, percent, SKILL_DETECTDISGUISE)) {
  send_to_char("Your eyes fail to see through the disguise!.\r\n", ch);
}

 else {

sprintf(buf, vict->char_specials.name_dis);
GET_NAME(vict, chname); 
sprintf(buf2, "Your eyes tell you that %s is actually %s in disguise!", buf, chname);
act(buf2, FALSE, ch, 0, 0, TO_CHAR);
FREE_NAME(chname);
   
WAIT_STATE(ch, 2 * PULSE_VIOLENCE);

}

  WAIT_STATE(ch, 2 * PULSE_VIOLENCE);
  
}
*/

/*
ACMD(do_call)
{

  char arg[MAX_STRING_LENGTH];
  struct char_data *mob = NULL;
  int statFactor = 100; 
  struct affected_type af;
  int messageType = 1;
 
  one_argument(argument, arg);

  if (!*arg) {
    send_to_char("What are you trying to call? (mount)\r\n", ch);
    return;
  }  
  else if (is_abbrev(arg, "mount")) {
    if (affected_by_spell(ch, SKILL_CALL_MOUNT)) {
      send_to_char("Your mount refuses to answer your call.\r\n", ch);
      return;
    }
    if (!check_skill(ch, 101, SKILL_CALL_MOUNT)) {
      if (!check_skill(ch, 101, SKILL_CALL_MOUNT)) {
        send_to_char("Your mount refuses your summons and cannot be called for some time.\r\n", ch);

        af.bitvector = AFF_NONE;
        af.location = APPLY_NONE;
        af.type = SKILL_CALL_MOUNT;
        af.modifier = 0;
        af.duration = 6;

        affect_to_char(ch, &af);
  
        return;
      }
      statFactor = 75;
      messageType = 2;
    }

    mob = read_mobile(4560, VIRTUAL);
  }
  else { 
    send_to_char("What are you trying to call? (mount)\r\n", ch);
    return;
  }

  // Set descriptions:
  GET_ALIAS(mob) = str_dup("warhorse horse black sleek solid coated");
  GET_SDESC(mob) = str_dup("a sleek and solid, black-coated warhorse");
  GET_LDESC(mob) = str_dup("A sleek and solid, black-coated warhorse stands here patiently and obediently.");
  mob->player.description = strdup(
    "Before you is a tall warhorse, solidly built with well-defined muscles that ripple across\r\n"
    "a sleek coat of night-black hair.  The horse seems well trained and well mannered, though\r\n"
    "it pays little attention to anyone or anything except its master and anything they are\r\n"
    "focused on.");

  // Set stats:

  


}
*/

ACMD(do_makeleader)
{

  char arg[MAX_STRING_LENGTH];
  //struct char_data *k;
  struct follow_type *f;
  struct char_data *vict;
  char msg[MAX_STRING_LENGTH];
  struct follow_type *tch;

  one_argument(argument, arg);

  if (!IS_SET(AFF_FLAGS(ch), AFF_GROUP)) {
    send_to_char("You must be grouped to assign the group leader to someone else.\r\n", ch);
    return;
  }

  if (ch->master) {
    send_to_char("You must be the group leader to assign a new one.\r\n", ch);
    return;
  }

  if (!*arg) {
    send_to_char("Whom do you wish to make the new group leader?\r\n", ch);
    return;
  }
  
  if (!(vict = get_char_vis(ch, arg, FIND_CHAR_WORLD))) {
    send_to_char("You have no knowledge of that person.\r\n", ch);
    return;
  }

  if (IS_NPC(vict)) {
    send_to_char("You cannot assign a mob as the group leader.\r\n", ch);
    return;
  }

  if (vict->master != ch) {
    send_to_char("That person is not following you.\r\n", ch);
    return;
  }

  if (!IS_SET(AFF_FLAGS(vict), AFF_GROUP)) {
    send_to_char("That person is not grouped.\r\n", ch);
    return;
  }

  stop_follower(vict);

  SET_BIT(AFF_FLAGS(vict), AFF_GROUP);

  for (f = ch->followers; f; f = f->next) {
    if (f->follower != vict) {

      f->follower->master = vict;
      tch = vict->followers;
      vict->followers = f;
      vict->followers->next = tch;

      add_follower(f->follower, vict);
      GET_NAME_II(ch, f->follower, chname);
      sprintf(msg, "%s has made $N the group leader.  You are now following $M.", chname);
      FREE_NAME(chname);
      act(msg, FALSE, f->follower, 0, vict, TO_CHAR);
    }
  }

//  ch->followers = NULL;

  stop_follower(ch);
  add_follower(ch, vict);
  act("You have changed the party leader to $N.  You are now following $M.", FALSE, ch, 0, vict, TO_CHAR);
  act("$n has made you the group leader.  Everyone in the party is now following you.", FALSE, ch, 0, vict, TO_VICT);
//  vict->master = NULL;
}

ACMD(do_dismiss)
{

  char arg[100];
  struct char_data *vict;

  one_argument(argument, arg);

  if (!*arg) {
    send_to_char("Whom do you wish to dismiss?\r\n", ch);
    return;
  }
  
  if (!(vict = get_char_vis(ch, arg, FIND_CHAR_WORLD))) {
    send_to_char("You have no knowledge of that person.\r\n", ch);
    return;
  }

  if (!IS_NPC(vict)) {
    send_to_char("You cannot dismiss another player.\r\n", ch);
    return;
  }

  if (vict->master != ch) {
    send_to_char("That mob is not a follower of yours.\r\n", ch);
    return;
  }

  if (!MOB_FLAGGED(vict, MOB_MOUNTABLE) && !AFF_FLAGGED(vict, AFF_CHARM)) {
    send_to_char("You can only dismiss mounts and charmed mobs.\r\n", ch);
    return;
  }

  if (RIDDEN_BY(vict) == ch) {
    RIDING(ch) = NULL;
    GET_POS(ch) = POS_STANDING;
  }

  act("$n is dismissed by $N.", TRUE, vict, 0, ch, TO_ROOM);
  extract_char(vict);
}

ACMD(do_rouse) {
    struct follow_type *j, *k;
    char buf[100];


    for (k = ch->followers; k; k = j) {
        j = k->next;

        if (IS_AFFECTED(k->follower, AFF_KNOCKOUT)) {
            act("$N has been knocked unconscious. You can't wake $M up!", FALSE, ch, 0, k->follower, TO_CHAR);
        } else if (IS_AFFECTED(k->follower, AFF_SLEEP) || GET_POS(k->follower) == POS_SLEEPING) {
            act("You are unable to rouse $N from their slumber. Try waking $M first!", FALSE, ch, 0, k->follower, TO_CHAR);
        } else if (affected_by_spell(k->follower, SPELL_CRIPPLE)) {
            act("$N has been crippled and cannot stand!", FALSE, ch, 0, k->follower, TO_CHAR);
        } else if (GET_POS(k->follower) < POS_SLEEPING) {
            act("$N has been mortally-wounded and cannot be roused!", FALSE, ch, 0, k->follower, TO_CHAR);
        } else if (GET_POS(k->follower) == POS_STANDING) {
            // No-op
        } else {
            GET_POS(k->follower) = POS_STANDING;

            GET_NAME(ch, chname);

            sprintf(buf, "%s forces you to your feet.\r\n", chname);
            act("$n is forced to their feet.", TRUE, k->follower, 0, 0, TO_ROOM);

            FREE_NAME(chname);

            send_to_char(buf, k->follower);
        }   // End if chain
    }   // End for
}   // End do_rouse

ACMD(do_paraderest) {
    struct follow_type *j, *k;
    char buf[100];

    for (k = ch->followers; k; k = j) {
        j = k->next;

        if (IS_AFFECTED(k->follower, AFF_KNOCKOUT)) {
            act("$N has been knocked unconscious. You can't wake $M up!", FALSE, ch, 0, k->follower, TO_CHAR);
        } else if (IS_AFFECTED(k->follower, AFF_SLEEP) || GET_POS(k->follower) == POS_SLEEPING) {
            act("You are unable to call $N to rest. Try waking $M first!", FALSE, ch, 0, k->follower, TO_CHAR);
        } else if (GET_POS(k->follower) < POS_SLEEPING) {
            act("$N has been mortally-wounded and cannot be called to rest!", FALSE, ch, 0, k->follower, TO_CHAR);
        } else if (GET_POS(k->follower) == POS_FIGHTING) {
            act("$N is fighting and cannot be called to rest!", FALSE, ch, 0, k->follower, TO_CHAR);
        } else {
            GET_POS(k->follower) = POS_RESTING;
            GET_NAME(ch, chname);

            sprintf(buf, "%s calls you to rest.\r\n", chname);
            act("$N is called to rest.", TRUE, k->follower, 0, 0, TO_ROOM);

            FREE_NAME(chname);
            send_to_char(buf, k->follower);
        }
    }
}

/*
ACMD(do_hero)
{
  int found = FALSE;
  int num = 0;
  char arg[100];
  char arg2[100];
  char arg3[100];
  extern struct spell_info_type spell_info[];
  extern int raceAbils[NUM_RACES][6][2];
  extern char *spells[];


  one_argument(one_argument(one_argument(argument, arg), arg2), arg3);

  if (GET_LEVEL(ch) < 30) {
    send_to_char("You must be level 30 to use the hero command.\r\n", ch);
    return;
  }

  if (!*arg) {
    send_to_char("You may use the following options with the hero command:\r\n"
		"buy <bonus> - varies      - purchase stat increases for your character.\r\n"
		"heal        - 500,000 exp  - restore hit points to maximum.\r\n"
		"restore     - 250,000 exp  - restore movement points to maximum.\r\n"
		"coins       - varies      - Adds coins to your bank account *\r\n"
		"\r\n"
		"* At a rate of 1 copper coin per 10 exp point spent.r\n", ch);
    return;
  }

  if (!strcmp(arg, "buy")) {
  if (!*arg2) {
  send_to_char("You may currently purchase the following:\r\n"
		"hp      - Extra Hit Die Rolls     : 10,000,000 exp\r\n"
		"move    - Extra 1d5 Move Points   :  2,500,000 exp\r\n"
		"prac    - Extra Practice Session  :  1,000,000 exp\r\n"
		"skillup - +1% Skill Increase      :  1,000,000 exp *\r\n"
		"str     - +1 to Real Strength     : 25,000,000 exp **\r\n"
		"con     - +1 to Real Constitution : 15,000,000 exp **\r\n"
		"dex     - +1 to Real Dexterity    : 20,000,000 exp **\r\n"
		"int     - +1 to Real Intelligence : 15,000,000 exp **\r\n"
		"wis     - +1 to Real Wisdom       : 25,000,000 exp **\r\n"
		"cha     - +1 to Real Charisma     : 10,000,000 exp **\r\n"
		"\r\n"
		"* Must be a skill you have and maxes out at 95%.\r\n"

		"** Will not go over racial maximum\r\n", ch);
  return;
  }
  if (!strcmp(arg2, "hp")) {

    if (GET_EXP(ch) < 10000000) {
      send_to_char("You must have at least 10,000,000 experience points to buy an extra hit die.r\n", ch);
      return;
    }


    switch (GET_CLASS(ch)) {

    case CLASS_MAGE:
    case CLASS_SORCEROR:
    case CLASS_ADVENTURER:
      num += number(3, 8);
      break;
    case CLASS_CLERIC:
      num += number(5, 10);
      break;
    case CLASS_DRUID:
      num += number(5, 10);
      break;
    case CLASS_MONK:
      num += number(8, 14);
      break;
    case CLASS_ROGUE:
      num += number(7, 12);
      break;
     case CLASS_RANGER:
       num += number(10, 14);
       break;
    case CLASS_WARRIOR:
      num += number(10, 14);
      break;
    case CLASS_PALADIN:
      num += number(11, 15);
      break;
    case CLASS_KNIGHT:
      num += number(12, 17);
      break;
    case CLASS_DEFENDER:
      num += number(16, 22);
      break;
    default:
      num += number(5, 10);
      break;
    }
  
    if (GET_RACE(ch) == RACE_MINOTAUR)
      num += number(2, 3);

    GET_MAX_HIT(ch) += num;
    GET_HIT(ch) += num;

    GET_EXP(ch) -= 10000000;

    sprintf(buf, "You have gained %d hit points.\r\n", num);
    send_to_char(buf, ch);
    return;
  }

  else if (!strcmp(arg2, "move")) {

    if (GET_EXP(ch) < 2500000) {
      send_to_char("You must have at least 2,500,000 experience points to buy an extra 1d5 movement points.r\n", ch);
      return;
    }

    GET_EXP(ch) -= 2500000;

    num = dice(1, 5);

    GET_MAX_MOVE(ch) += num;
    GET_MOVE(ch) += num;

    sprintf(buf, "You have gained %d move points.\r\n", num);
    send_to_char(buf, ch);
    return;
  }

  else if (!strcmp(arg2, "prac")) {

    if (GET_EXP(ch) < 1000000) {
      send_to_char("You must have at least 1,000,000 experience points to buy an extra practice session.r\n", ch);
      return;
    }

    GET_EXP(ch) -= 1000000;

    GET_PRACTICES(ch) += 1;

    sprintf(buf, "You have gained 1 practice session.\r\n");
    send_to_char(buf, ch);
    return;
  }

  else if (!strcmp(arg2, "skillup")) {

    if (GET_EXP(ch) < 1000000) {
      send_to_char("You must have at least 1,000,000 experience points to buy a skill increase.r\n", ch);
      return;
    }

    if (!*arg3) {
      send_to_char("Please specific a skill or spell name separated by dashes instead of spaces.  Ie. cure-light\r\n", ch);
      return;
    }

    for (num = 0; num < strlen(arg3); num++)
      if (arg3[num] == '-')
        arg3[num] = ' ';

    for (num = 0; num < MAX_SKILLS; num++) {
      if (!strcmp(spells[num], arg3) && (spell_info[num].min_level[GET_CLASS(ch)] <= 30 || spell_info[num].min_level_race[GET_RACE(ch)] <= 30)
          && spell_access(ch, num)) {
        found = TRUE;
        break;
      }
    }

    if (!found) {
      send_to_char("That is not a valid spell or skill.\r\n", ch);
      return;
    }
  
   if (GET_SKILL(ch, num) >= 95) {
     send_to_char("You have already raised this skill as high as it will go.\r\n", ch);
     return; 
   }

    GET_EXP(ch) -= 1000000;

    GET_SKILL(ch, num) += 1;

    sprintf(buf, "Your %s skill has increased by 1%%.\r\n", spells[num]);
    send_to_char(buf, ch);
    return;
  }

  else if (!strcmp(arg2, "str")) {

    if (GET_EXP(ch) < 25000000) {
      send_to_char("You must have at least 25,000,000 experience points to increase your natural strength by one.r\n", ch);
      return;
    }

    if (ch->real_abils.str >= raceAbils[(int)GET_RACE(ch)][0][1]) {
      send_to_char("Your strength is already at its racial maximum.\r\n", ch);
      return;
    }

    GET_EXP(ch) -= 25000000;

    ch->real_abils.str += 1;
    ch->aff_abils.str += 1;

    affect_total(ch);

    sprintf(buf, "Your natural strength has improved by one.\r\n");
    send_to_char(buf, ch);
    return;
  }
  else if (!strcmp(arg2, "int")) {

    if (GET_EXP(ch) < 15000000) {
      send_to_char("You must have at least 15,000,000 experience points to increase your natural intelligence by one.r\n", ch);
      return;
    }

    if (ch->real_abils.intel >= raceAbils[(int)GET_RACE(ch)][1][1]) {
      send_to_char("Your intelligence is already at its racial maximum.\r\n", ch);
      return;
    }

    GET_EXP(ch) -= 15000000;

    ch->real_abils.intel += 1;
    ch->aff_abils.intel += 1;

    affect_total(ch);

    sprintf(buf, "Your natural intelligence has improved by one.\r\n");
    send_to_char(buf, ch);
    return;
  }
  else if (!strcmp(arg2, "wis")) {

    if (GET_EXP(ch) < 25000000) {
      send_to_char("You must have at least 25,000,000 experience points to increase your natural wisdom by one.r\n", ch);
      return;
    }

    if (ch->real_abils.wis >= raceAbils[(int)GET_RACE(ch)][2][1]) {
      send_to_char("Your wisdom is already at its racial maximum.\r\n", ch);
      return;
    }

    GET_EXP(ch) -= 25000000;

    ch->real_abils.wis += 1;
    ch->aff_abils.wis += 1;

    affect_total(ch);

    sprintf(buf, "Your natural wisdom has improved by one.\r\n");
    send_to_char(buf, ch);
    return;
  }
  else if (!strcmp(arg2, "dex")) {

    if (GET_EXP(ch) < 20000000) {
      send_to_char("You must have at least 20,000,000 experience points to increase your natural dexterity by one.r\n", ch);
      return;
    }

    if (ch->real_abils.dex >= raceAbils[(int)GET_RACE(ch)][3][1]) {
      send_to_char("Your dexterity is already at its racial maximum.\r\n", ch);
      return;
    }

    GET_EXP(ch) -= 20000000;

    ch->real_abils.dex += 1;
    ch->aff_abils.dex += 1;

    affect_total(ch);

    sprintf(buf, "Your natural dexterity has improved by one.\r\n");
    send_to_char(buf, ch);
    return;
  }
  else if (!strcmp(arg2, "con")) {

    if (GET_EXP(ch) < 15000000) {
      send_to_char("You must have at least 15,000,000 experience points to increase your natural constitution by one.r\n", ch);
      return;
    }

    if (ch->real_abils.con >= raceAbils[(int)GET_RACE(ch)][4][1]) {
      send_to_char("Your constitution is already at its racial maximum.\r\n", ch);
      return;
    }

    GET_EXP(ch) -= 15000000;

    ch->real_abils.con += 1;
    ch->aff_abils.con += 1;

    affect_total(ch);

    sprintf(buf, "Your natural constitution has improved by one.\r\n");
    send_to_char(buf, ch);
    return;
  }
  else if (!strcmp(arg2, "cha")) {

    if (GET_EXP(ch) < 10000000) {
      send_to_char("You must have at least 10,000,000 experience points to increase your natural charisma by one.r\n", ch);
      return;
    }

    if (ch->real_abils.cha >= raceAbils[(int)GET_RACE(ch)][5][1]) {
      send_to_char("Your charisma is already at its racial maximum.\r\n", ch);
      return;
    }

    GET_EXP(ch) -= 10000000;

    ch->real_abils.cha += 1;
    ch->aff_abils.cha += 1;

    affect_total(ch);

    sprintf(buf, "Your natural charisma has improved by one.\r\n");
    send_to_char(buf, ch);
    return;
  }
  } // END "BUY"
  else if (!strcmp(arg, "heal")) {

    if (GET_EXP(ch) < 500000) {
      send_to_char("You must have at least 500,000 experience points to restore your hp to full.r\n", ch);
      return;
    }

    GET_EXP(ch) -= 500000;

    GET_HIT(ch) = GET_MAX_HIT(ch);

    sprintf(buf, "Your hp has been restored to full.\r\n");
    send_to_char(buf, ch);
    return;
  }
  else if (!strcmp(arg, "restore")) {

    if (GET_EXP(ch) < 250000) {
      send_to_char("You must have at least 250,000 experience points to restore your moves to full.r\n", ch);
      return;
    }

    GET_EXP(ch) -= 250000;

    GET_MOVE(ch) = GET_MAX_MOVE(ch);

    sprintf(buf, "Your moves have been restored to full.\r\n");
    send_to_char(buf, ch);
    return;
  }
  else if (!strcmp(arg, "coins")) {

    if (!*arg2) {
      send_to_char("How many exp points to you want to convert to copper coins? (1 exp = 1 copper coin)\r\n", ch);
      return;
    }

    num = atoi(arg2);

    if (GET_EXP(ch) < num) {
      send_to_char("You do not have that many exp points to convert to money.r\n", ch);
      return;
    }

    GET_EXP(ch) -= num;

    num /= 10;

    GET_BANK_GOLD(ch) += num;

    sprintf(buf, "Your bank account has increased by %d copper coins.\r\n", num);
    send_to_char(buf, ch);
    return;
  }
  else {
    send_to_char("That is not a valid option.  Type hero by itself to see what options there are.\r\n", ch);
    return;
  }
}
*/

