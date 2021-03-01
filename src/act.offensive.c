/* ************************************************************************
*   File: act.offensive.c                               Part of CircleMUD *
*  Usage: player-level commands of an offensive nature                    *
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
extern int pk_allowed;
extern char *musical[];

/* extern functions */
int allowNewFollower(struct char_data* ch, int maxFollowerAllowed);
void raw_kill(struct char_data * ch, struct char_data * killer);
void check_killer(struct char_data * ch, struct char_data * vict);
void add_follower(struct char_data * ch, struct char_data * leader);
int mag_savingthrow(struct char_data * ch, int type);
int calc_weapon_damage(struct obj_data *wielded, struct char_data *ch);
int calc_ac(struct char_data *victim, struct char_data *ch);
int calc_thaco(struct char_data *ch);


#define MAX_SPELL_AFFECTS 5     /* change if more needed */

/* local functions */
void perform_challenge_buff(struct char_data *ch);
ACMD(do_assist);
ACMD(do_hit);
ACMD(do_kill);
ACMD(do_backstab);
ACMD(do_order);
ACMD(do_flee);
ACMD(do_bash);
ACMD(do_rescue);
ACMD(do_kick);
ACMD(do_gore);
/*ACMD(do_spinkick); */
ACMD(do_trip);
ACMD(do_jawstrike);
ACMD(do_knockout);
ACMD(do_dirtkick);
ACMD(do_throw);
ACMD(do_assist)
{
  struct char_data *helpee, *opponent;

  if (FIGHTING(ch)) {
    send_to_char("You're already fighting!  How can you assist someone else?\r\n", ch);
    return;
  }
  one_argument(argument, arg);

 if (GET_POS(ch) == POS_FISHING || GET_POS(ch) == POS_DIGGING) {
     send_to_char("You are not in a proper position for that!\r\n", ch);
    return ;
}

  if (!*arg)
    send_to_char("Whom do you wish to assist?\r\n", ch);
  else if (!(helpee = get_char_vis(ch, arg, FIND_CHAR_ROOM)))
    send_to_char(NOPERSON, ch);
  else if (helpee == ch)
    send_to_char("You can't help yourself any more than this!\r\n", ch);
  else {
    if (FIGHTING(helpee))
      opponent = FIGHTING(helpee); 
    else
      for (opponent = world[ch->in_room].people;
	 opponent && (FIGHTING(opponent) != helpee);
	 opponent = opponent->next_in_room)
		;

    if (!opponent)
      act("But nobody is fighting $M!", FALSE, ch, 0, helpee, TO_CHAR);
    else if (!CAN_SEE(ch, opponent))
      act("You can't see who is fighting $M!", FALSE, ch, 0, helpee, TO_CHAR);
    else if (!pk_allowed && !IS_NPC(opponent))	/* prevent accidental pkill */
      act("Use 'murder' if you really want to attack $N.", FALSE,
	  ch, 0, opponent, TO_CHAR);
    else {
      send_to_char("You join the fight!\r\n", ch);
      act("$N assists you!", 0, helpee, 0, ch, TO_CHAR);
      act("$n assists $N.", FALSE, ch, 0, helpee, TO_NOTVICT);
      hit(ch, opponent, TYPE_UNDEFINED);
    }
  }
}


ACMD(do_hit)
{
  struct char_data *vict;

  one_argument(argument, arg);

 if (GET_POS(ch) == POS_FISHING || GET_POS(ch) == POS_DIGGING) {
     send_to_char("You are not in a proper position for that!\r\n", ch);
    return ;
}

  if (!*arg)
    send_to_char("Hit who?\r\n", ch);
  else if (!(vict = get_char_vis(ch, arg, FIND_CHAR_ROOM)))
    send_to_char("They don't seem to be here.\r\n", ch);
  else if (vict == ch) {
    send_to_char("You hit yourself...OUCH!.\r\n", ch);
    act("$n hits $mself, and says OUCH!", FALSE, ch, 0, vict, TO_ROOM);
  } else if (IS_AFFECTED(ch, AFF_CHARM) && (ch->master == vict))
    act("$N is just such a good friend, you simply can't hit $M.", FALSE, ch, 0, vict, TO_CHAR);
  else {
    if (!pk_allowed) {
      if (!IS_NPC(vict) && !IS_NPC(ch)) {
	if (subcmd != SCMD_MURDER) {
	  send_to_char("Use 'murder' to hit another player.\r\n", ch);
	  return;
	} else {
	  check_killer(ch, vict);
	}
      }
      if (IS_AFFECTED(ch, AFF_CHARM) && !IS_NPC(ch->master) && !IS_NPC(vict))
	return;			/* you can't order a charmed pet to attack a
				 * player */
    }
    if ((GET_POS(ch) == POS_STANDING || GET_POS(ch) == POS_RIDING) && (vict != FIGHTING(ch))) {
      hit(ch, vict, TYPE_UNDEFINED);
      WAIT_STATE(ch, PULSE_VIOLENCE + 2);
    } else
      send_to_char("You do the best you can!\r\n", ch);
  }
}



ACMD(do_kill)
{
  struct char_data *vict;

 if (GET_POS(ch) == POS_FISHING || GET_POS(ch) == POS_DIGGING) {
     send_to_char("You are not in a proper position for that!\r\n", ch);
    return ;
}
  if ((GET_LEVEL(ch) < LVL_IMPL) || IS_NPC(ch)) {
    do_hit(ch, argument, cmd, subcmd);
    return;
  }
  one_argument(argument, arg);

  if (!*arg) {
    send_to_char("Kill who?\r\n", ch);
  } else {
    if (!(vict = get_char_vis(ch, arg, FIND_CHAR_ROOM)))
      send_to_char("They aren't here.\r\n", ch);
    else if (ch == vict)
      send_to_char("Your mother would be so sad.. :(\r\n", ch);
    else {
      act("You chop $M to pieces!  Ah!  The blood!", FALSE, ch, 0, vict, TO_CHAR);
      act("$N chops you to pieces!", FALSE, vict, 0, ch, TO_CHAR);
      act("$n brutally slays $N!", FALSE, ch, 0, vict, TO_NOTVICT);
      raw_kill(vict, ch);
    }
  }
}



ACMD(do_backstab)
{
  struct char_data *vict;
  int skilltype = SKILL_BACKSTAB;

  one_argument(argument, buf);

 if (GET_POS(ch) == POS_FISHING || GET_POS(ch) == POS_DIGGING || GET_POS(ch) == POS_RIDING) {
     send_to_char("You are not in a proper position for that!\r\n", ch);
    return ;
}

  if (!(vict = get_char_vis(ch, buf, FIND_CHAR_ROOM))) {
    send_to_char("Backstab who?\r\n", ch);
    return;
  }
  if (vict == ch) {
    send_to_char("How can you sneak up on yourself?\r\n", ch);
    return;
  }
  if (!GET_EQ(ch, WEAR_WIELD)) {
    send_to_char("You need to wield a weapon to make it a success.\r\n", ch);
    return;
  }
  if (((GET_OBJ_VAL(GET_EQ(ch, WEAR_WIELD), 3) != TYPE_PIERCE - TYPE_HIT) &&
      (GET_OBJ_VAL(GET_EQ(ch, WEAR_WIELD), 3) != TYPE_STAB - TYPE_HIT)))
  {
    if (!GET_SKILL(ch, SKILL_ADVANCED_BACKSTAB)) {
      send_to_char("You're not using a piercing or stabbing weapon.\r\n", ch);
      return;
    }
    else {
      if (!check_skill(ch, 101, SKILL_ADVANCED_BACKSTAB)) {
        send_to_char("You fail to execute a proper sneak attack and are forced to attack normally.\r\n", ch);
        hit(ch, vict, TYPE_UNDEFINED);
        return;
      }
      skilltype = SKILL_ADVANCED_BACKSTAB;
    }
  }

  if (FIGHTING(vict) && (!check_skill(ch, 101, SKILL_IMPROVED_BACKSTAB) 
      || !check_skill(ch, 101, SKILL_HIDE) || !check_skill(ch, 101, SKILL_SNEAK))) {
    send_to_char("You try to sneak up and backstab them but their combat situation has made them too alert!\r\n", ch);	hit(ch, vict, TYPE_UNDEFINED);
    return;
  }
  if(IS_NPC(vict))
  {
    if (ch->desc && ch->desc->marked && ch->desc->marked == vict && ch->desc->marked_time >= 3) {
      if (!check_skill(ch, 101, SKILL_BACKSTAB) || check_skill(ch, 101, SKILL_BACKSTAB)) { // For skill increase only
        hit(ch, vict, skilltype);
        GET_BACKSTABBED(vict) = 1;
        return;
      }
    }

    if(GET_BACKSTABBED(vict) && !AFF_FLAGGED(vict, AFF_KNOCKOUT) &&
       (!check_skill(ch, 101, SKILL_ADVANCED_BACKSTAB) && 
        dice(1, 10) > (GET_SKILL(ch, SKILL_ADVANCED_BACKSTAB) / 30)))    {      
      act("You notice $N lunging at you!", FALSE, vict, 0, ch, TO_CHAR);
      act("$e notices you lunging at $m!", FALSE, vict, 0, ch, TO_VICT);
      act("$n notices $N lunging at $m!", FALSE, vict, 0, ch, TO_NOTVICT);
      hit(vict, ch, TYPE_UNDEFINED);
      return; 
    }
    GET_BACKSTABBED(vict) = 1;
  }

  if (MOB_FLAGGED(vict, MOB_AWARE) && AWAKE(vict) && !AFF_FLAGGED(vict, AFF_KNOCKOUT)) {
    act("You notice $N lunging at you!", FALSE, vict, 0, ch, TO_CHAR);
    act("$e notices you lunging at $m!", FALSE, vict, 0, ch, TO_VICT);
    act("$n notices $N lunging at $m!", FALSE, vict, 0, ch, TO_NOTVICT);
    hit(vict, ch, TYPE_UNDEFINED);
    return;
  }
  if (!check_skill(ch, 101, SKILL_BACKSTAB) && AWAKE(vict))
    damage(ch, vict, 0, skilltype);
  else
    hit(ch, vict, skilltype);

  WAIT_STATE(ch, 2 * PULSE_VIOLENCE);
}



ACMD(do_order)
{
  char name[MAX_INPUT_LENGTH], message[MAX_INPUT_LENGTH];
  char buf[MAX_INPUT_LENGTH];
  bool found = FALSE;
  room_rnum org_room;
  struct char_data *vict;
  struct follow_type *k;

  half_chop(argument, name, message);

  if (!*name || !*message)
    send_to_char("Order who to do what?\r\n", ch);
  else if (!(vict = get_char_vis(ch, name, FIND_CHAR_ROOM)) && !is_abbrev(name, "followers"))
    send_to_char("That person isn't here.\r\n", ch);
  else if (ch == vict)
    send_to_char("You obviously suffer from skitzofrenia.\r\n", ch);

  else {
    if (IS_AFFECTED(ch, AFF_CHARM)) {
      send_to_char("Your superior would not aprove of you giving orders.\r\n", ch);
      return;
    }
    if (vict) {
      sprintf(buf, "$N orders you to '%s'", message);
      act(buf, FALSE, vict, 0, ch, TO_CHAR);
      act("$n gives $N an order.", FALSE, ch, 0, vict, TO_ROOM);

      if ((vict->master != ch) || !IS_AFFECTED(vict, AFF_CHARM))
	act("$n has an indifferent look.", FALSE, vict, 0, 0, TO_ROOM);
      else {
	send_to_char(OK, ch);
	command_interpreter(vict, message);
      }
    } else {			/* This is order "followers" */
      sprintf(buf, "$n issues the order '%s'.", message);
      act(buf, FALSE, ch, 0, vict, TO_ROOM);

      org_room = ch->in_room;

      for (k = ch->followers; k; k = k->next) {
        if (org_room == k->follower->in_room)
          if (IS_AFFECTED(k->follower, AFF_CHARM)) {
            found = TRUE;
            command_interpreter(k->follower, message);
          }
      }

      if (found)
        send_to_char(OK, ch);
      else
        send_to_char("Nobody here is a loyal subject of yours!\r\n", ch);
    }
  }
}



ACMD(do_flee)
{
  int i, attempt, loss;
  struct char_data *was_fighting;


  if (GET_POS(ch) < POS_FIGHTING) {
    send_to_char("You are in pretty bad shape, unable to flee!\r\n", ch);
    return;
  }

  if (!FIGHTING(ch) && !affected_by_spell(ch, SPELL_INSPIRE_FEAR))
  {
    send_to_char("But you aren't fighting anyone!\r\n", ch);
    return;
  }

  was_fighting = FIGHTING(ch);

  for (i = 0; i < 6; i++) {
    attempt = number(0, NUM_OF_DIRS - 1);	/* Select a random direction */
    if (CAN_GO(ch, attempt) &&
	!IS_SET(ROOM_FLAGS(EXIT(ch, attempt)->to_room), ROOM_DEATH)) {
      act("$n panics, and attempts to flee!", TRUE, ch, 0, 0, TO_ROOM);
      if (do_simple_move(ch, attempt, TRUE)) {
	send_to_char("You flee head over heels.\r\n", ch);

	if (was_fighting && !IS_NPC(ch)) {
	  loss = GET_MAX_HIT(was_fighting) - GET_HIT(was_fighting);
	  loss *= GET_LEVEL(was_fighting);
	  gain_exp(ch, -loss);	  
	}
      } else {
	act("$n tries to flee, but can't!", TRUE, ch, 0, 0, TO_ROOM);
      }

      return;
    }
  }
  if(!IS_NPC(ch))
     send_to_char("PANIC!  You couldn't escape!\r\n", ch);
}

ACMD(do_retreat)
{
  int prob, loss, direction;
  struct char_data * was_fighting;

  if (!FIGHTING(ch))
  {
    send_to_char("But you're not even fighting anyone!\r\n", ch);
    return;
  }

  if (!GET_SKILL(ch, SKILL_RETREAT))
  {
    send_to_char("You are unskilled in retreating.  Try fleeing.\r\n", ch);
    return;
  }

  prob = number(1, 101) - (GET_LEVEL(ch) / 2) + GET_LEVEL(FIGHTING(ch));
  one_argument(argument, arg);

  if (!*arg)
  {
    send_to_char("Which way?\r\n", ch);
    return;
  }

  if (is_abbrev(arg, "north"))
    direction = NORTH;
  else if (is_abbrev(arg, "south"))
    direction = SOUTH;
  else if (is_abbrev(arg, "east"))
    direction = EAST;
  else if (is_abbrev(arg, "west"))
    direction = WEST;
  else if (is_abbrev(arg, "up"))
    direction = UP;
  else if (is_abbrev(arg, "down"))
    direction = DOWN;
  else if (is_abbrev(arg, "northeast") || is_abbrev(arg, "ne"))
    direction = NORTHEAST;
  else if (is_abbrev(arg, "northwest") || is_abbrev(arg, "nw"))
    direction = NORTHWEST;
  else if (is_abbrev(arg, "southeast") || is_abbrev(arg, "se"))
    direction = SOUTHEAST;
  else if (is_abbrev(arg, "southwest") || is_abbrev(arg, "sw"))
    direction = SOUTHWEST;
  else
  {
    send_to_char("That's not a valid direction!\r\n", ch);
    return;
  }

  if (!EXIT(ch, direction) || (EXIT(ch, direction)->to_room == NOWHERE) ||
     (IS_SET(EXIT(ch, direction)->exit_info, EX_CLOSED)))
  {
    send_to_char("You won't be able to escape that way!\r\n", ch);
    return;
  }

  act("$n attempts to retreat from battle.", TRUE, ch, 0, 0, TO_ROOM);

  was_fighting = FIGHTING(ch);

  if (!use_skill(ch, prob, SKILL_RETREAT) || !do_simple_move(ch, direction, 0))
  {
    send_to_char("You attempt to retreat, but cannot.\r\n", ch);
    act("$n finds $s retreat has been cut off!", TRUE, ch, 0, 0, TO_ROOM);
  }
  else
  {
    send_to_char("You retreat from battle.\r\n", ch);

    if (was_fighting && FIGHTING(was_fighting) && FIGHTING(was_fighting) == ch)
      stop_fighting(was_fighting);

    if (was_fighting)
    {
      loss = (GET_HIT(was_fighting) - GET_MAX_HIT(was_fighting));
      loss *= GET_LEVEL(was_fighting);
      loss /= 3;
    }
    else
      loss = -10;

    gain_exp(ch, loss);
  }

  WAIT_STATE(ch, PULSE_VIOLENCE);
}


ACMD(do_bash)
{
  struct char_data *vict;
  int percent;
  int dam = 0;

  one_argument(argument, arg);

 if (GET_POS(ch) == POS_FISHING || GET_POS(ch) == POS_DIGGING || GET_POS(ch) == POS_RIDING) {
     send_to_char("You are not in a proper position for that!\r\n", ch);
    return ;
}
  
  if (!GET_SKILL(ch, SKILL_BASH))
  {
    act("You are unfamiliar with this skill!", FALSE, ch, 0, 0, TO_CHAR);
    return;
}


  if (!*arg || (!(vict = get_char_vis(ch, arg, FIND_CHAR_ROOM))))
  {
    if (FIGHTING(ch) && IN_ROOM(ch) == IN_ROOM(FIGHTING(ch)))
    {
      vict = FIGHTING(ch);
    } else {
      send_to_char("Bash whom?\r\n", ch);
      return;
    }
  }
  if (vict == ch) {
    send_to_char("Aren't we funny today...\r\n", ch);
    return;
  }

   if (AFF_FLAGGED(vict, AFF_FLIGHT)) {
  act("But their feet aren't exactly on the ground!", FALSE, ch, 0, 0, TO_CHAR);
   return;
}

  if (!GET_EQ(ch, WEAR_SHIELD)) {
    send_to_char("You need to use a shield to make it a success.\r\n", ch);
    return;
  }
  percent = number(1, 101);	/* 101% is a complete failure */

  if (MOB_FLAGGED(vict, MOB_NOBASH))
    percent = 101;

  if (!use_skill(ch, percent, SKILL_BASH))
  {
    damage(ch, vict, 0, SKILL_BASH);
    GET_POS(ch) = POS_SITTING;
  } else {
    /* There's a bug in the two lines below. We could fail bash and they sit.*/
    dam = 1 + 
          (check_skill(ch, 101, SKILL_SHIELD_MASTERY) ? 
           dice(1, GET_SKILL(ch, SKILL_SHIELD_MASTERY) / 15) : 0) +
          (check_skill(ch, 101, SKILL_IMPROVED_SHIELD_MASTERY) ? 
           dice(1, GET_SKILL(ch, SKILL_IMPROVED_SHIELD_MASTERY) / 15) : 0) +
          (check_skill(ch, 101, SKILL_ADVANCED_SHIELD_MASTERY) ? 
           dice(1, GET_SKILL(ch, SKILL_ADVANCED_SHIELD_MASTERY) / 15) : 0);

    damage(ch, vict, dam, SKILL_BASH);
    if (affected_by_spell(vict, SPELL_CRIPPLE))
      GET_POS(vict) = POS_RESTING;
    else
      GET_POS(vict) = POS_SITTING;
    WAIT_STATE(vict, PULSE_VIOLENCE);
  }
  WAIT_STATE(ch, PULSE_VIOLENCE * 2);
}


ACMD(do_rescue)
{
  struct char_data *vict, *tmp_ch;

  one_argument(argument, arg);

 if (GET_POS(ch) == POS_FISHING || GET_POS(ch) == POS_DIGGING) {
     send_to_char("You are not in a proper position for that!\r\n", ch);
    return ;
}

  if (!(vict = get_char_vis(ch, arg, FIND_CHAR_ROOM))) {
    send_to_char("Whom do you want to rescue?\r\n", ch);
    return;
  }
  if (vict == ch) {
    send_to_char("What about fleeing instead?\r\n", ch);
    return;
  }
  if (FIGHTING(ch) == vict) {
    send_to_char("How can you rescue someone you are trying to kill?\r\n", ch);
    return;
  }
  for (tmp_ch = world[ch->in_room].people; tmp_ch &&
       (FIGHTING(tmp_ch) != vict); tmp_ch = tmp_ch->next_in_room);

  if (!tmp_ch) {
    act("But nobody is fighting $M!", FALSE, ch, 0, vict, TO_CHAR);
    return;
  }

  if (!GET_SKILL(ch, SKILL_RESCUE))
  {
    send_to_char("You are unfamiliar with the art of rescuing.\r\n", ch);
  }
  else {
    if (!check_skill(ch, 101, SKILL_RESCUE))
    {
      send_to_char("You fail the rescue!\r\n", ch);
      return;
    }
    act("You bravely step into the fray, rescuing $N from harm.", FALSE, ch, 0, vict, TO_CHAR);

    act("$N steps in front of you, shielding you from harm.", FALSE, vict, 0, ch, TO_CHAR);
    act("$n heroically rescues $N!", FALSE, ch, 0, vict, TO_NOTVICT);

    if (FIGHTING(vict) == tmp_ch)
      stop_fighting(vict);
    if (FIGHTING(tmp_ch))
      stop_fighting(tmp_ch);
    if (FIGHTING(ch))
      stop_fighting(ch);

    set_fighting(ch, tmp_ch);
    set_fighting(tmp_ch, ch);

    WAIT_STATE(vict, 2 * PULSE_VIOLENCE);
  }

}

ACMD(do_guard)
{
  struct char_data *vict;

 if (GET_POS(ch) == POS_FISHING || GET_POS(ch) == POS_DIGGING) {
     send_to_char("You are not in a proper position for that!\r\n", ch);
    return ;
}

  if (!GET_SKILL(ch, SKILL_GUARD))
  {
    send_to_char("You are unskilled in the art of guarding.\r\n", ch);
    return;
  }

  one_argument(argument, arg);

  if (!*arg)
  { 
    if (GUARDING(ch) == NULL)
    {
      send_to_char("Whom did you wish to guard?\r\n", ch);
    }
    else
    {
      act("You are guarding $N.", FALSE, ch, 0, GUARDING(ch), TO_CHAR);
    }
    return;
  }

  if (!(vict = get_char_vis(ch, arg, FIND_CHAR_ROOM)))
  {
    send_to_char("Whom did you wish to guard?\r\n", ch);
    return;
  }

  if (vict == ch)
  {
    if (GUARDING(ch) == NULL)
    {
      send_to_char("You do the best you can!\r\n", ch);
    }
    else
    {
      act("You stop guarding $N.", FALSE, ch, 0, GUARDING(ch), TO_CHAR);
      act("$n stops guarding you.", TRUE, ch, 0, GUARDING(ch), TO_VICT);
      act("$n stops guarding $N.", TRUE, ch, 0, GUARDING(ch), TO_NOTVICT);
      GUARDING(ch) = NULL;
    }
    return;
  }

  if (FIGHTING(vict))
  {
    send_to_char("It's a little late for that, isn't it?  Perhaps a rescue instead?\r\n", ch);
    return;
  }

  act("You start guarding $N.", FALSE, ch, 0, vict, TO_CHAR);
  act("$n starts guarding you.", TRUE, ch, 0, vict, TO_VICT);
  act("$n starts guarding $N.", TRUE, ch, 0, vict, TO_NOTVICT);
  GUARDING(ch) = vict;

  return;
}



ACMD(do_kick)
{
  struct char_data *vict;
  int percent;

 if (GET_POS(ch) == POS_FISHING || GET_POS(ch) == POS_DIGGING || GET_POS(ch) == POS_RIDING) {
     send_to_char("You are not in a proper position for that!\r\n", ch);
    return ;
}

  if (!GET_SKILL(ch, SKILL_KICK)) {
    act("You are unfamiliar with this skill!", FALSE, ch, 0, 0, TO_CHAR);
    return;
  }

  one_argument(argument, arg);

  if ( ! *arg ) {
    if (FIGHTING(ch)) {
      vict = FIGHTING(ch);
    } else {
      send_to_char("Kick whom?\r\n", ch);
      return;
    }

  } else {
    if (!(vict = get_char_vis(ch, arg, FIND_CHAR_ROOM))) {
      send_to_char("Kick whom?\r\n", ch);
      return;
    }
  }

  if (vict == ch) {
    send_to_char("You might want to rethink that idea...\r\n", ch);
    return;
  }

  percent = ((10 - (GET_AC(vict) / 10)) * 2) + number(1, 101);

  if (!use_skill(ch, percent, SKILL_KICK))
    damage(ch, vict, 0, SKILL_KICK);
  else {
    damage(ch, vict, GET_LEVEL(ch) / 2, SKILL_KICK);
  }

  WAIT_STATE(ch, PULSE_VIOLENCE * 2);
}

/*ACMD(do_spinkick)
{
  struct char_data *vict;
  int percent;

 if (GET_POS(ch) == POS_FISHING || GET_POS(ch) == POS_DIGGING || GET_POS(ch) == POS_RIDING) {
     send_to_char("You are not in a proper position for that!\r\n", ch);
    return ;
}
 
  if (!GET_SKILL(ch, SKILL_SPINKICK)) {
    act("You are unfamiliar with this skill!", FALSE, ch, 0, 0, TO_CHAR);
    return;
  }
 
  one_argument(argument, arg);
 
  if ( ! *arg ) {
    if (FIGHTING(ch)) {
      vict = FIGHTING(ch);
    } else {
      send_to_char("Spinkick who?\r\n", ch);
      return;
    }
 
  } else {
    if (!(vict = get_char_vis(ch, arg, FIND_CHAR_ROOM))) {
      send_to_char("Spinkick who?\r\n", ch);
      return;
    }
  }
 
  if (vict == ch) {
    send_to_char("Aren't we funny today...\r\n", ch);
    return;
  }
 
  percent = ((10 - (GET_AC(vict) / 10)) * 2) + number(1, 101);
 
  if (!use_skill(ch, percent, SKILL_SPINKICK))
    damage(ch, vict, 0, SKILL_SPINKICK);
  else {
    damage(ch, vict, GET_LEVEL(ch) * 2, SKILL_SPINKICK);
  }
 
  WAIT_STATE(ch, PULSE_VIOLENCE * 3);
}
*/

ACMD(do_disarm)
{
  int eq_pos, percent;
  struct char_data *vict;
  struct obj_data *obj;

 if (GET_POS(ch) == POS_FISHING || GET_POS(ch) == POS_DIGGING || GET_POS(ch) == POS_RIDING) {
     send_to_char("You are not in a proper position for that!\r\n", ch);
    return ;
}

  if (!GET_SKILL(ch, SKILL_DISARM))
  {
    send_to_char("You are unskilled in the art of disarming.\r\n", ch);
    return;
  }

  one_argument(argument, arg);

  if (!arg || !(vict = get_char_vis(ch, arg, FIND_CHAR_ROOM)))
  {
    if (!FIGHTING(ch))
    {
      send_to_char("Who do you want to disarm?\r\n", ch);
      return;
    }
    else
      vict = FIGHTING(ch);
  }

  if (vict == ch)
  {
    send_to_char("Disarming yourself would be a really neat trick.\r\n", ch);
    return;
  }

  if((!GET_EQ(ch, WEAR_WIELD)) && (GET_CLASS(ch) != CLASS_MONK))
  {
    send_to_char("What are you, a monk?  Try using a weapon for that!\r\n", ch);
    return;
  }

  if (!GET_EQ(vict, WEAR_WIELD))
  {
    if (GET_EQ(vict, WEAR_HOLD) &&
       (GET_OBJ_TYPE(GET_EQ(vict, WEAR_HOLD)) == ITEM_WEAPON))
      eq_pos = WEAR_HOLD;
    else
    {
      send_to_char("But they aren't even using a weapon!\r\n", ch);
      return;
    }
  }
  else
    eq_pos = WEAR_WIELD;

  if (GET_CLASS(ch) == CLASS_MONK) {
    percent = 30;
  } else {
    percent = number(1, 101) + GET_DEX(vict) - GET_DEX(ch);
  }

  if (!use_skill(ch, percent, SKILL_DISARM))
  {
    if(GET_CLASS(ch) == CLASS_MONK) {
      send_to_char("Be careful!  You almost lost a hand there!\r\n", ch);
    } else {
      send_to_char("Be careful!  You almost lost your own weapon.\r\n", ch);
    }
  }
  else
  {
    obj = unequip_char(vict, eq_pos);
    sprintf(buf, "You knock $p from $N's hands!");
    act(buf, FALSE, ch, obj, vict, TO_CHAR);

    sprintf(buf, "$n knocks $p from your hands!");
    act(buf, FALSE, ch, obj, vict, TO_VICT);

    sprintf(buf, "$n knocks $p from $N's hands!");
    act(buf, TRUE, ch, obj, vict, TO_NOTVICT);

    // Disarmed weapons should go to the char's inventory, not the room.
    obj_to_char(obj, vict);

    /*
    if (IS_NPC(vict))
      obj_to_char(obj, vict);
    else
      obj_to_room(obj, vict->in_room);
    */
  }

  WAIT_STATE(ch, PULSE_VIOLENCE * 2);
  if (!FIGHTING(ch))
    set_fighting(ch, vict);
  if (!FIGHTING(vict))
    set_fighting(vict, ch);

  return;
}

ACMD(do_trip)
{
   struct char_data *vict = NULL;
   int percent;

  one_argument(argument, arg);

 if (GET_POS(ch) == POS_FISHING || GET_POS(ch) == POS_DIGGING || GET_POS(ch) == POS_RIDING) {
     send_to_char("You are not in a proper position for that!\r\n", ch);
    return ;
}

  if (!GET_SKILL(ch, SKILL_TRIP))
  {
    act("You are not trained in this ability.", FALSE, ch, 0, 0, TO_CHAR);
    return;
 }

 if (!*arg || (!(vict == get_char_vis(ch, arg, FIND_CHAR_ROOM))))
 {
    if (FIGHTING(ch) && IN_ROOM(ch) == IN_ROOM(FIGHTING(ch)))
    {
      vict = FIGHTING(ch);
    } else {
      send_to_char("Trip who, exactly?\r\n", ch);
      return;
    }
  }
  if (vict == ch)  {
    send_to_char("Does that really seem like such a good idea?\r\n", ch);
    return;
  }

   if (AFF_FLAGGED(vict, AFF_FLIGHT)) {
   act("But their feet aren't exactly on the ground!", FALSE, ch, 0, 0, TO_CHAR);
   return;
}

  percent = number(1, 101); /*101% is a complete failure*/

  if (MOB_FLAGGED(vict, MOB_NOBASH))
    percent = 101;

  if (!use_skill(ch, percent, SKILL_TRIP))
  {
    damage(ch, vict, 0, SKILL_TRIP);
    GET_POS(ch) = POS_SITTING;
  }
  else {
    damage(ch, vict, 1, SKILL_TRIP);
    if(affected_by_spell(vict, SPELL_CRIPPLE))
      GET_POS(vict) = POS_RESTING;
    else  
      GET_POS(vict) = POS_SITTING;
    WAIT_STATE(vict, PULSE_VIOLENCE);
  }
  WAIT_STATE(ch, PULSE_VIOLENCE * 2);
}



ACMD(do_envenom)
{
  int w_type;
  struct obj_data *weapon, *poison;
  struct affected_type af;

 if (GET_POS(ch) == POS_FISHING || GET_POS(ch) == POS_DIGGING || GET_POS(ch) == POS_RIDING) {
     send_to_char("You are not in a proper position for that!\r\n", ch);
    return ;
}

  if (!GET_SKILL(ch, SKILL_ENVENOM))
  {
    send_to_char("You are unskilled at the art of poisoning weapons.\r\n", ch);
    return;
  }

  if (affected_by_spell(ch, SKILL_ENVENOM))
  {
    send_to_char("But it already is!\r\n", ch);
    return;
  }

  if (!(poison = get_obj_list_vnum(ch, ch->carrying, VNUM_ENVENOM_POISON)))
  {
    send_to_char("You seem to lack the necessary poison.\r\n", ch);
    return;
  }

  if (!GET_EQ(ch, WEAR_WIELD))
  {
    send_to_char("Without even having a weapon wielded?\r\n", ch);
    return;
  }

  weapon = GET_EQ(ch, WEAR_WIELD);
  if (!(GET_OBJ_TYPE(weapon) == ITEM_WEAPON))
  {
    send_to_char("But that's not a weapon you're wielding!\r\n", ch);
    return;
  }

  w_type = GET_OBJ_VAL(weapon, 3) + TYPE_HIT;

  switch (w_type)
  {
    case TYPE_STING:
    case TYPE_SLASH:
    case TYPE_BITE:
    case TYPE_CLAW:
    case TYPE_PIERCE:
    case TYPE_STAB:
      break;
    default:
      send_to_char("You cannot use poison on that type of weapon.\r\n", ch);
      return;
  }

  af.type = SKILL_ENVENOM;
  af.duration = (GET_LEVEL(ch) / 5);
  af.modifier = 0;
  af.location = APPLY_NONE;
  af.bitvector = 0;

  act("Your weapon has been coated with poison.",  0, ch, 0, 0, TO_CHAR);
  act("$n's weapon has been coated with poison..", 0, ch, 0, 0, TO_ROOM);
 

  extract_obj(poison);
  affect_to_char(ch, &af);

  return;
}

ACMD(do_turn_undead)
{
  struct char_data *victim, *tmp;
  int percent, turned = 0;
  struct affected_type af;

 if (GET_POS(ch) == POS_FISHING || GET_POS(ch) == POS_DIGGING) {
     send_to_char("You are not in a proper position for that!\r\n", ch);
    return ;
}

  if (!GET_SKILL(ch, SKILL_TURN_UNDEAD))
  {
    send_to_char("You lack the ability to turn undead beings.\r\n", ch);
    return;
  }

  for (tmp = world[ch->in_room].people; tmp; )
  {
    victim = tmp;
    tmp = tmp->next;

    if (!IS_UNDEAD(victim))
      continue;

    percent = number(1, 100) + (GET_LEVEL(victim) * 2);

    if ((GET_LEVEL(victim) >= GET_LEVEL(ch) + 10) || (GET_LEVEL(victim) > 30)
        || (affected_by_spell(victim, SPELL_BRAVERY)) ||
           (affected_by_spell(victim, SPELL_IMPERVIOUS_MIND)))
      percent = 101;

    if (use_skill(ch, percent, SKILL_TURN_UNDEAD))
    {
      turned = 1;
      af.type = SPELL_INSPIRE_FEAR;
      af.duration = 1;
      af.modifier = 0;
      af.location = APPLY_NONE;
      af.bitvector = 0;
      affect_join(victim, &af, 0, 0, 0, 0);
      do_flee(victim, "", 0, 0);
    }
    else
      hit(victim, ch, TYPE_UNDEFINED);
  }

  if (turned)
  {
    act("You have managed to turn some undead.", 0, ch, 0, 0, TO_CHAR);
    act("$n has managed to turn undead.", 0, ch, 0, 0, TO_ROOM);
  } else
  {
    act("You fail to turn any undead.", 0, ch, 0, 0, TO_CHAR);
    act("$n tries to turn undead, but fails.", 0, ch, 0, 0, TO_ROOM);
  }

  WAIT_STATE(ch, PULSE_VIOLENCE * 2);
  return;
}


ACMD(do_control_undead)
{
  struct char_data *victim;
  struct affected_type af;
  int percent;

  if (!GET_SKILL(ch, SKILL_CONTROL_UNDEAD))
  {
    send_to_char("You know nothing of how to control the undead.\r\n", ch);
    return;
  }

  one_argument(argument, arg);

  if (!*arg || !(victim = get_char_vis(ch, arg, FIND_CHAR_ROOM)))
  {
    send_to_char("Who do you want to attempt to control?\r\n", ch);
    return;
  }

  if (!IS_UNDEAD(victim))
  {
    send_to_char("But they aren't even undead!\r\n", ch);
    return;
  }

  if (circle_follow(victim, ch))
  {
    send_to_char("Sorry, no following in loops.\r\n", ch);
    return;
  }

  percent = number(1, 60) + (GET_LEVEL(victim));

  if ((GET_LEVEL(victim) > MIN(30, GET_LEVEL(ch))) ||
       affected_by_spell(victim, SPELL_IMPERVIOUS_MIND) ||
       mag_savingthrow(victim, SAVING_PARA))
    percent = 101;

  if (!use_skill(ch, percent, SKILL_CONTROL_UNDEAD))
  {
    send_to_char("Your attempt at control has gone awry.\r\n", ch);
    hit(victim, ch, TYPE_UNDEFINED);
  } else
  {

    if (allowNewFollower(ch, 3))
    {
      af.type      = SPELL_CHARM;
      af.location  = APPLY_NONE;
      af.bitvector = AFF_CHARM;
      af.modifier  = 0;
      af.duration  = (GET_LEVEL(ch) / 2) + 5;

      if (victim->master)
        stop_follower(victim);

      add_follower(victim, ch);
      affect_to_char(victim, &af);

      act("You have managed to control $N.", FALSE, ch, 0, victim, TO_CHAR);
      act("You find yourself controlled by $n!", FALSE, ch, 0, victim, TO_VICT);
      act("$n has managed to gain control over $N.", 1, ch, 0, victim, TO_ROOM);
    }
  }

  WAIT_STATE(ch, PULSE_VIOLENCE * 1);

  return;
}

ACMD(do_jawstrike)
{
  struct char_data *vict;

  struct affected_type af;

  bool accum_affect = FALSE;

  one_argument(argument, arg);

 if (GET_POS(ch) == POS_FISHING || GET_POS(ch) == POS_DIGGING || GET_POS(ch) == POS_RIDING) {
     send_to_char("You are not in a proper position for that!\r\n", ch);
    return ;
}

  if (!GET_SKILL(ch, SKILL_JAWSTRIKE))
  {
    act("You are unfamiliar with this skill!", FALSE, ch, 0, 0, TO_CHAR);
    return;
  }

    if (FIGHTING(ch)) {
    act("You can't do that when you are fighting!", FALSE, ch, 0, 0, TO_CHAR);
    return;
  }
  
  if (!*arg || (!(vict = get_char_vis(ch, arg, FIND_CHAR_ROOM))))
  {
      send_to_char("Whose jaw do you want to break?\r\n", ch);
      return;
 }

  if (IS_AFFECTED(vict, AFF_JAWSTRIKE)) {
     send_to_char("Victim has already been silenced!\r\n", ch);
      return ;
    }

 if (GET_POS(vict) == POS_SLEEPING) {
    send_to_char("But they are sleeping..perhaps you want to silence their snoring?\r\n", ch);
    return;
}

  if (vict == ch) {
    send_to_char("Aren't we funny today...\r\n", ch);
    return;
  }


      if (IS_NPC(vict) && AWAKE(vict) && MOB_FLAGGED(vict, MOB_AWARE) && !FIGHTING(vict) && vict != ch)
{
act("You notice $N lunging at you and dodges $S fist!", FALSE, vict, 0, ch, TO_CHAR);
act("$e notices you lunging at $m and dodges your fist!", FALSE, vict, 0, ch, TO_VICT);
act("$n notices $N lunging at $m and dodges $S fist!", FALSE, vict, 0, ch, TO_NOTVICT);

     hit(vict, ch, TYPE_UNDEFINED);
      return;
    }

if ((GET_LEVEL(vict) >= GET_LEVEL(ch) + 10) || (GET_LEVEL(vict) > 30)) {
act("You notice $N lunging at you and dodges $S fist!", FALSE, vict, 0, ch, TO_CHAR);
act("$e notices you lunging at $m and dodges your fist!", FALSE, vict, 0, ch, TO_VICT);
act("$n notices $N lunging at $m and dodges $S fist!", FALSE, vict, 0, ch, TO_NOTVICT);

     hit(vict, ch, TYPE_UNDEFINED);
      return;
    }

if (!check_skill(ch, 101, SKILL_JAWSTRIKE))
  {
  act("You notice $N lunging at you and dodges $S fist!", FALSE, vict, 0, ch, TO_CHAR);
act("$e notices you lunging at $m and dodges your fist!", FALSE, vict, 0, ch, TO_VICT);
act("$n notices $N lunging at $m and dodges $S fist!", FALSE, vict, 0, ch, TO_NOTVICT);

    damage(ch, vict, 0, SKILL_JAWSTRIKE);
  }

 else {
      af.type      = SPELL_DONTUSEME;
      af.location  = APPLY_NONE;
      af.bitvector = AFF_JAWSTRIKE;
      af.modifier  = 0;
      af.duration  = 1;

      accum_affect = FALSE;

      affect_to_char(vict, &af);


act("$N pulls back $S fist and strikes you squarely on the jaw, rendering you speechless with pain!", FALSE, vict, 0, ch, TO_CHAR);
act("You pull back your fist and strike $n squarely on the jaw, rendering $m speechless with pain!", FALSE, vict, 0, ch, TO_VICT);
act("$N pulls back $S fist and strikes $n squarely on the jaw, rendering $m speechless with pain!", FALSE, vict, 0, ch, TO_NOTVICT);

WAIT_STATE(ch, PULSE_VIOLENCE * 3);
   
  }
 WAIT_STATE(ch, PULSE_VIOLENCE * 3);
return;
}

ACMD(do_dirtkick)
{
  struct char_data *vict;
  bool accum_affect = FALSE, accum_duration = FALSE;

  int i;

  one_argument(argument, arg);

  if (!GET_SKILL(ch, SKILL_DIRTKICK))
  {
    act("You are unfamiliar with this skill!", FALSE, ch, 0, 0, TO_CHAR);
    return;
  }

   if (FIGHTING(ch)) {
    act("But you are fighting!", FALSE, ch, 0, 0, TO_CHAR);
    return;
  }


  if (!*arg || (!(vict = get_char_vis(ch, arg, FIND_CHAR_ROOM))))
  {
      send_to_char("Whose face do you intend to shower with dirt?\r\n", ch);
      return;
 }

 if (IS_AFFECTED(vict, AFF_BLIND)) {
     send_to_char("Victim is already blind!\r\n", ch);
      return ;
    }

 if((SECT(ch->in_room) == SECT_INSIDE) || (SECT(ch->in_room) == SECT_CITY))
    {
      send_to_char("There is no sand or dirt to kick at!\r\n", ch);
      return ;
    }

  if((SECT(ch->in_room) == SECT_WATER_SWIM) ||
     (SECT(ch->in_room) == SECT_WATER_NOSWIM))
    {
      send_to_char("Try getting your feet on the ground first.\r\n", ch);
      return;
    }

  if(SECT(ch->in_room) == SECT_UNDERWATER)
    {
      send_to_char("You cannot possibly do this underwater!\r\n", ch);
      return;
    }

  if (vict == ch) {
    send_to_char("Aren't we funny today...\r\n", ch);
    return;
  }

if (GET_POS(vict) == POS_SLEEPING || AFF_FLAGGED(vict, AFF_KNOCKOUT)) {
  send_to_char("But they are sleeping!\r\n", ch);
  return;
}

if ((MOB_FLAGGED(vict,MOB_NOBLIND)) || (GET_LEVEL(vict) >= GET_LEVEL(ch) + 10) || (GET_LEVEL(vict) > 30) || mag_savingthrow(vict, SAVING_PARA) )
    {
      if (IS_NPC(vict) && AWAKE(vict) && !FIGHTING(vict) && vict != ch)
{
act("You shield your face just in time to avoid the sand getting into your eyes!", FALSE, vict, 0, ch, TO_CHAR);
act("$n shields $s face just in time to avoid the sand getting into $s eyes!", FALSE, vict, 0, ch, TO_VICT);
act("$N shields $S face just in time to avoid the sand getting into $S eyes", FALSE, vict, 0, ch, TO_NOTVICT);


        hit(vict, ch, TYPE_UNDEFINED);
      return;
     }
    }

if (!check_skill(ch, 101, SKILL_DIRTKICK))
  {
    act("You shield your face just in time to avoid the sand getting into your eyes!", FALSE, vict, 0, ch, TO_CHAR);
act("$n shields $s face just in time to avoid the sand getting into $s eyes!", FALSE, vict, 0, ch, TO_VICT);
act("$N shields $S face just in time to avoid the sand getting into $S eyes", FALSE, vict, 0, ch, TO_NOTVICT);

    damage(ch, vict, 0, SKILL_DIRTKICK);
  }

 else {
      
struct affected_type af[2];

   for (i = 0; i < 2; i++) {
    af[0].type     = SKILL_DIRTKICK;
    af[0].location = APPLY_HITROLL;
    af[0].modifier = -4;
    af[0].duration = 1;
    af[0].bitvector = AFF_BLIND;
 
    af[1].type      = SKILL_DIRTKICK;
    af[1].location = APPLY_AC;
    af[1].modifier = 40;
    af[1].duration = 1;
    af[1].bitvector = AFF_BLIND;

      if (af[i].bitvector || (af[i].location != APPLY_NONE)) {
        affect_join(vict, af+i, accum_duration, FALSE, accum_affect, FALSE);
      }
    }


act("$N suddenly kicks the ground, spraying sand in every direction and into your eyes, blinding you temporarily!", FALSE, vict, 0, ch, TO_CHAR);
act("You suddenly kick the ground, spraying sand in every direction and into $n's eyes, blinding $m temporarily!", FALSE, vict, 0, ch, TO_VICT);
act("$N suddenly kicks the ground, spraying sand in every direction and into $n's eyes, blinding $m temporarily!", FALSE, vict, 0, ch, TO_NOTVICT);

WAIT_STATE(ch, PULSE_VIOLENCE * 3);
 
  }
WAIT_STATE(ch, PULSE_VIOLENCE * 3);
return;
}

int chance(int num)
{
    if (number(1,100) <= num) return 1;
    else return 0;
}

ACMD(do_gore)
{
  struct char_data *vict;
  int damage_val;
  int percent;

 if (GET_POS(ch) == POS_FISHING || GET_POS(ch) == POS_DIGGING || GET_POS(ch) ==
 POS_RIDING) {
     send_to_char("You are not in a proper position for that!\r\n", ch);
    return ;
}

  if (!GET_SKILL(ch, SKILL_GORE)) {
    act("You are unfamiliar with this skill!", FALSE, ch, 0, 0, TO_CHAR);
    return;
  }

  one_argument(argument, arg);

  if ( ! *arg ) {
    if (FIGHTING(ch)) {
      vict = FIGHTING(ch);
    } else {
      send_to_char("Gore who?\r\n", ch);
      return;
    }

  } else {
    if (!(vict = get_char_vis(ch, arg, FIND_CHAR_ROOM))) {
      send_to_char("Gore who?\r\n", ch);
      return;
    }
  }

  if (vict == ch) {
    send_to_char("You might want to rethink that idea...\r\n", ch);
    return;
  }

  percent = ((10 - (GET_AC(vict) / 10)) * 2) + number(1, 101);

  damage_val = GET_STR(ch) / 3 + GET_LEVEL(ch)/3 + dice(2, 6);

  if (!use_skill(ch, percent, SKILL_GORE)) {
 act("You notice $N lunging at you and duck!", FALSE, vict, 0, ch, TO_CHAR);
act("$e notices you lunging at $m and ducks!", FALSE, vict, 0, ch, TO_VICT);
act("$n notices $N lunging at $m and ducks!", FALSE, vict, 0, ch, TO_NOTVICT);
    damage(ch, vict, 0, SKILL_GORE);
}
  else {

 act("Blood sprays everywhere $N gores you with $S horn!", FALSE, vict, 0, ch, TO_CHAR);
act("Blood sprays everywhere as you gore $n with your horn!", FALSE, vict, 0, ch, TO_VICT);
act("Blood sprays everywhere as $N gores $n with $S horn!", FALSE, vict, 0, ch, TO_NOTVICT);

    damage(ch, vict, damage_val, SKILL_GORE);
  }

  WAIT_STATE(ch, PULSE_VIOLENCE * 2);
}

ACMD(do_throw) { 
    struct char_data *vict;
    struct obj_data *obj;
    int percent, prob;
    int damage_val;
    int obj_type;
    int item_damage;
    int stat_bonus_damage;
    int i;
    bool remove_item;
    two_arguments(argument, buf, buf2);

    if (GET_POS(ch) == POS_FISHING || GET_POS(ch) == POS_DIGGING) {
        send_to_char("You are not in a proper position for that!\r\n", ch);
        return ;
    }

    if (!(vict = get_char_room_vis(ch, buf2))) {
        send_to_char("They aren't here.\r\n", ch);
        return;
    }

    if (!(obj = get_obj_in_list_vis(ch, buf, ch->carrying))) {
        send_to_char("Throw what?\r\n", ch);
        return;
    }

    if (vict == ch) {
        send_to_char("That would be funny to see.\r\n", ch);
        return;
    }

    if (IS_CORPSE(obj)) {
        send_to_char("You can't throw THAT!\r\n", ch);
        return;
    }

    percent = number(1, 101);	/* 101% is a complete failure */
    prob = GET_SKILL(ch, SKILL_THROW);

    //damage_val = GET_STR(ch) / 2 + GET_OBJ_WEIGHT(obj) / 4 + GET_LEVEL(ch) / 2;
    //obj_type = GET_OBJ_TYPE(obj);

    if (GET_OBJ_TYPE(obj) == ITEM_WEAPON) {
        item_damage = dice(GET_OBJ_VAL(obj, 1), GET_OBJ_VAL(obj, 2));
    } else {
        item_damage = GET_OBJ_WEIGHT(obj);
    }

    stat_bonus_damage = GET_STR(ch)/2 + GET_DEX(ch)/2;
    damage_val = stat_bonus_damage + item_damage + (GET_SKILL(ch, SKILL_THROW) / 4);

    if (!use_skill(ch, percent, SKILL_THROW) || percent > prob) {
        /* miss */
        damage(ch, vict, 0, SKILL_THROW);
        act("$N throws $p at you and misses by a long shot!", FALSE, vict, obj, ch, TO_CHAR);         /* victim */
        act("You throw $p at $n but, miss by a long shot!", FALSE, vict, obj, ch, TO_VICT);           /* ch */
        act("$N throws $p at $n but, misses by a long shot!", FALSE, vict, obj, ch, TO_NOTVICT);      /* everyone else */

        obj_from_char(obj);
        obj_to_room(obj, ch->in_room);

        return;

    } else {
        if (GET_OBJ_TYPE(obj) == ITEM_SCROLL || (GET_OBJ_TYPE(obj) == ITEM_NOTE)) {
            act("$N hits you upside the head hard with $p!", FALSE, vict, obj, ch, TO_CHAR);
            act("You hit $n in the head hard with $p!", FALSE, vict, obj, ch, TO_VICT);
            act("$N hits $n in the head hard with $p!", FALSE, vict, obj, ch, TO_NOTVICT);

        } else if (GET_OBJ_TYPE(obj) == ITEM_WEAPON) {
            act("$N throws $p at you, cutting your chest open!", FALSE, vict, obj, ch, TO_CHAR);
            act("You throw $p at $n, cutting $s chest open!", FALSE, vict, obj, ch, TO_VICT);
            act("$N throws $p at $n, cutting $s chest open!", FALSE, vict, obj, ch, TO_NOTVICT);

        } else if (GET_OBJ_TYPE(obj) == ITEM_POTION) {
            act("$N throws $p at you and it shatters in your face!", FALSE, vict, obj, ch, TO_CHAR);
            act("You throw $p at $n and it shatters in $s face!", FALSE, vict, obj, ch, TO_VICT);
            act("$N throws $p at $n and it shatters in $s face!", FALSE, vict, obj, ch, TO_NOTVICT);

            remove_item = TRUE;

            for (i = 1; i < 4; i++) {
                if (!(call_magic(ch, vict, NULL, GET_OBJ_VAL(obj, i), GET_OBJ_VAL(obj, 0), CAST_POTION))){
                    continue;
                }
            }

        } else {
            act("$N throws $p and hits you square in the chest!", FALSE, vict, obj, ch, TO_CHAR);
            act("You throw $p at $n and hit $m in the chest!", FALSE, vict, obj, ch, TO_VICT);
            act("$N throws $p at $n and hits $m in the chest!", FALSE, vict, obj, ch, TO_NOTVICT);
        }
    }

    // Check for followers - no aggro
    // Consume item when thrown if potion
    if(IS_NPC(vict)){
        damage(ch, vict, damage_val, SKILL_THROW);
    } else {
        // Target was a player - no action.
    }

    WAIT_STATE(ch, PULSE_VIOLENCE * 3);
    obj_from_char(obj);

    if(!remove_item){
        obj_to_char(obj, vict);
    }
}

ACMD(do_knockout)
{
  struct char_data *vict;
  struct obj_data *weapon;
  struct affected_type af;
  int percent;

  bool accum_affect = FALSE;

  one_argument(argument, arg);

 if (GET_POS(ch) == POS_FISHING || GET_POS(ch) == POS_DIGGING || GET_POS(ch) == POS_RIDING) {
     send_to_char("You are not in a proper position for that!\r\n", ch);
    return ;
}

  if (!GET_SKILL(ch, SKILL_KNOCKOUT))
  {
    act("You are unfamiliar with this skill!", FALSE, ch, 0, 0, TO_CHAR);
    return;
  }

  if (FIGHTING(ch)) {
    act("But you are fighting!", FALSE, ch, 0, 0, TO_CHAR);
    return;
  }


  if (!*arg || (!(vict = get_char_vis(ch, arg, FIND_CHAR_ROOM))))
  {
      send_to_char("Who do you want to knock out?\r\n", ch);
      return;
 }

 if (!GET_EQ(ch, WEAR_WIELD))
  {
    send_to_char("Without even having a weapon wielded?\r\n", ch);
    return;
  }


  weapon = GET_EQ(ch, WEAR_WIELD);
  if (!(GET_OBJ_TYPE(weapon) == ITEM_WEAPON))
  {
    send_to_char("But that's not a weapon you're wielding!\r\n", ch);
    return;
  }


  if (IS_AFFECTED(vict, AFF_KNOCKOUT)) {
     send_to_char("But they are already unconscious!\r\n", ch);
      return ;
    }

  if (GET_POS(vict) == POS_SLEEPING) {
     send_to_char("But they are already sleeping!\r\n", ch);
     return;
     }

  if (vict == ch) {
    send_to_char("Aren't we funny today...\r\n", ch);
    return;
  }


      if (MOB_FLAGGED(vict, MOB_AWARE) && AWAKE(vict) && vict != ch)
{
act("You turn around just in time to avoid $N's strike!", FALSE, vict, 0, ch, TO_CHAR);
act("$e turns around just in time to avoid your strike!", FALSE, vict, 0, ch, TO_VICT);
act("$n turns around just in time to avoid $N's strike!", FALSE, vict, 0, ch, TO_NOTVICT);
        hit(vict, ch, TYPE_UNDEFINED);
      return;
     }

if ((GET_LEVEL(vict) >= GET_LEVEL(ch) + 10) || (GET_LEVEL(vict) > 30)) {

act("You turn around just in time to avoid $N's strike!", FALSE, vict, 0, ch, TO_CHAR);
act("$e turns around just in time to avoid your strike!", FALSE, vict, 0, ch, TO_VICT);
act("$n turns around just in time to avoid $N's strike!", FALSE, vict, 0, ch, TO_NOTVICT);
        hit(vict, ch, TYPE_UNDEFINED);
      return;
     }

percent = ((10 - (GET_DEX(vict) / 10)) * 2) + number(1, 101);

if (!use_skill(ch, percent, SKILL_KNOCKOUT) && AWAKE(vict))
  {
 act("You turn around just in time to avoid $N's strike!", FALSE, vict, 0, ch, TO_CHAR);
act("$e turns around just in time to avoid your strike!", FALSE, vict, 0, ch, TO_VICT);
act("$n turns around just in time to avoid $N's strike!", FALSE, vict, 0, ch, TO_NOTVICT);
    damage(ch, vict, 0, SKILL_KNOCKOUT);
  }

 else {

act("Someone suddenly brings $S weapon down hard at the base of your neck, rendering you unconscious!", FALSE, vict, 0, ch, TO_CHAR);
act("You bring your weapon down hard at the base of $n's neck, rendering $m unconscious!", FALSE, vict, 0, ch, TO_VICT);
act("$N suddenly brings $S weapon down hard at the base of $n's neck, rendering $m unconscious!", FALSE, vict, 0, ch, TO_NOTVICT);

GET_POS(vict) = POS_SLEEPING;

      af.type      = SPELL_DONTUSEME;
      af.location  = APPLY_NONE;
      af.bitvector = AFF_KNOCKOUT;
      af.modifier  = 0;
      af.duration  = 1;

      accum_affect = FALSE;

      affect_to_char(vict, &af);


WAIT_STATE(ch, PULSE_VIOLENCE * 3);

  }
 WAIT_STATE(ch, PULSE_VIOLENCE * 3);
return;
}


ACMD(do_charge)
{

  struct char_data *vict;
  char arg[MAX_STRING_LENGTH];
  //int percent = 0;
  //int prob = 0;
  int damageMultiplier = 100 + dice(1, GET_SKILL(ch, SKILL_CHARGE));
  int improvedChargeSuccessful = FALSE;
  int diceroll = 0;
  int calc_thaco1 = 0, victim_ac1 = 0;
  
  one_argument(argument, arg);

  if (!*arg) {
    send_to_char("Who do you wish to charge?\r\n", ch);
    return;
  }


  if (!GET_SKILL(ch, SKILL_CHARGE)) {
    send_to_char("You are not proficient with the charge skill", ch);
	return;
  }

  if (!(vict = get_char_room_vis(ch, arg))) {
    send_to_char("That person is no where to be found.\r\n", ch);
	return;
  }

  if (FIGHTING(ch)) {
    send_to_char("You can only charge to begin a battle.\r\n", ch);
    return;
  }
	
  if (!check_skill(ch, 101, SKILL_CHARGE)) {
    act("$N sidesteps $n's charge who tramples past $M.", FALSE, ch, 0, vict, TO_NOTVICT);
	act("$N sidesteps your charge and you trample past $M.", FALSE, ch, 0, vict, TO_CHAR);
	act("You sidestep $n's charge who tramples past you.", FALSE, ch, 0, vict, TO_VICT);
    WAIT_STATE(ch, PULSE_VIOLENCE);
    return;
  }

 calc_thaco1 = calc_thaco(ch);
 
 victim_ac1 = calc_ac(vict, ch);
  
  diceroll = number(1, 20);

  /* decide whether this is a hit or a miss */
  if ((((diceroll < 20) &&
       ((diceroll == 1) || ((calc_thaco1 - diceroll) > victim_ac1))))) {
    act("$N sidesteps $n's charge who tramples past $M.", FALSE, ch, 0, vict, TO_NOTVICT);
    act("$N sidesteps your charge and you trample past $M.", FALSE, ch, 0, vict, TO_CHAR);
    act("You sidestep $n's charge who tramples past you.", FALSE, ch, 0, vict, TO_VICT);
    WAIT_STATE(ch, PULSE_VIOLENCE * 2);
    return;	   
  }

  if (check_skill(ch, 101, SKILL_IMPROVED_CHARGE)) {
    improvedChargeSuccessful = TRUE;
	damageMultiplier += dice(1, GET_SKILL(ch, SKILL_IMPROVED_CHARGE) / 2);
  }
  
  if (RIDING(ch)) {
    damageMultiplier += dice(1, GET_SKILL(ch, SKILL_CHARGE));
    if (improvedChargeSuccessful == TRUE)
	  damageMultiplier += dice(1, GET_SKILL(ch, SKILL_IMPROVED_CHARGE) / 2);
  }
  
  damage(ch, vict, damageMultiplier * calc_weapon_damage(GET_EQ(ch, WEAR_WIELD), ch) / 100, SKILL_CHARGE);

  WAIT_STATE(ch, PULSE_VIOLENCE * 4);
  WAIT_STATE(vict, PULSE_VIOLENCE * 2);

  
}

ACMD(do_challenge) {

  char arg[100];
  struct char_data *vict = NULL;
  struct affected_type af[2];
  int i;
  struct char_data *tch, *k;
  struct follow_type *f;

  one_argument(argument, arg);

  if (!*arg && !FIGHTING(ch)) {
    send_to_char("Who do you wish to challenge?\r\n", ch);
    return;
  }  
  else if (!*arg && FIGHTING(ch)) {
    vict = FIGHTING(ch);
  }
  else if (!(vict = get_char_room_vis(ch, arg))) {
    send_to_char("That person is no where to be found.\r\n", ch);
    return;
  }

  if (ch == vict) {
    send_to_char("That's the spirit.  The only one you're in competition with is yourself!\r\n", ch);
    return;
  }

  if (!GET_SKILL(ch, SKILL_CHALLENGE)) {
    send_to_char("Only knights can issue a knightly challenge to their opponents.\r\n", ch);
    return;
  }

  if (check_skill(ch, 101, SKILL_CHALLENGE) && dice(1, (GET_CHA(ch) + GET_WIS(ch) + GET_INT(ch)) / 2) > dice(1, GET_WIS(ch))) {
    act("$n's knightly challenge has $N shaken and low in morale.", FALSE, ch, 0, vict, TO_NOTVICT);
    act("Your knightly challenge has $N shaken and low in morale.", FALSE, ch, 0, vict, TO_CHAR);
    act("$n's knightly challenge has you shaken and low in morale.", FALSE, ch, 0, vict, TO_VICT);
  }
  else {
    act("$n offers $N a knightly challenge, who simply laughs at $s attempts.", FALSE, ch, 0, vict, TO_NOTVICT);
    act("You offer $N a knightly challenge, but $E simply laughs at your attempts.", FALSE, ch, 0, vict, TO_CHAR);
    act("$n offers you a knightly challenge, but you simply laugh at $s attempts", FALSE, ch, 0, vict, TO_VICT);
    if (!FIGHTING(vict))
      hit(vict, ch, TYPE_UNDEFINED);
    WAIT_STATE(ch, PULSE_VIOLENCE * 2);
    return;
  }

    af[0].type     = SPELL_SK_CHALLENGE;
    af[0].location = APPLY_HITROLL;
    af[0].modifier = -(MAX(1, GET_LEVEL(ch) / 15));
    af[0].duration = GET_CHA(ch) / 3;
    af[0].bitvector = 0;
 
    af[1].type      = SPELL_SK_CHALLENGE;
    af[1].location = APPLY_AC;
    af[1].modifier = (GET_LEVEL(ch));
    af[1].duration = GET_CHA(ch) / 3;
    af[1].bitvector = 0;

    af[2].type      = SPELL_SK_CHALLENGE;
    af[2].location = APPLY_DAMROLL;
    af[2].modifier = -(MAX(1, GET_LEVEL(ch) / 15));
    af[2].duration = GET_CHA(ch) / 3;
    af[2].bitvector = 0;


   for (i = 0; i < 3; i++) {
      affect_join(vict, af+i, TRUE, FALSE, FALSE, FALSE);
    }

  if (check_skill(ch, 101, SKILL_CHALLENGE) && dice(1, GET_CHA(ch) * 2) >= 15) {

    if (ch == NULL)
      return;

    perform_challenge_buff(ch);

    if (!IS_AFFECTED(ch, AFF_GROUP))
      return;

    if (ch->master != NULL)
      k = ch->master;
    else
      k = ch;

    for (f = k->followers; f; f = f->next) {
      if (IS_AFFECTED(f->follower, AFF_GROUP) &&
          f->follower->in_room == ch->in_room)
      {
        tch = f->follower;
        if (ch == tch)
          continue;
        perform_challenge_buff(tch);
      }
    }

    if ((k != ch) && IS_AFFECTED(k, AFF_GROUP) && (k->in_room == ch->in_room))
      perform_challenge_buff(k);

  }

  if (!FIGHTING(vict))
    hit(vict, ch, TYPE_UNDEFINED);

  WAIT_STATE(ch, PULSE_VIOLENCE * 2);


}
void perform_challenge_buff(struct char_data *ch) {
  struct affected_type af[3];
  int i;

    af[0].type     = SPELL_SK_CHALLENGE;
    af[0].location = APPLY_HITROLL;
    af[0].modifier = (MAX(1, GET_LEVEL(ch) / 15));
    af[0].duration = GET_CHA(ch) / 3;
    af[0].bitvector = 0;

    af[1].type      = SPELL_SK_CHALLENGE;
    af[1].location = APPLY_AC;
    af[1].modifier = -(GET_LEVEL(ch) / 2);
    af[1].duration = GET_CHA(ch) / 3;
    af[1].bitvector = 0;

    af[2].type      = SPELL_SK_CHALLENGE;
    af[2].location = APPLY_DAMROLL;
    af[2].modifier = (MAX(1, GET_LEVEL(ch) / 15));
    af[2].duration = GET_CHA(ch) / 3;
    af[2].bitvector = 0;


   for (i = 0; i < 3; i++) {
      affect_join(ch, af+i, TRUE, FALSE, FALSE, FALSE);
    }
}

ACMD(do_mark)
{

  struct char_data *vict;

  one_argument(argument, arg);

  if (!*arg) {
    send_to_char("Who do you wish to mark?\r\n", ch);
    return;
  }  

  if (FIGHTING(ch)) {
    send_to_char("You cannot mark a target while fighting.\r\n", ch);
    return;
  }

  if (!(vict = get_char_room_vis(ch, arg))) {
    send_to_char("That person is no where to be found.\r\n", ch);
    return;
  }

  if (ch == vict) {
    send_to_char("If you need someone to talk to I know a of a good hotline.\r\n", ch);
    return;
  }

  if (!GET_SKILL(ch, SKILL_MARK)) {
    send_to_char("You have no idea how to mark your opponents.\r\n", ch);
    return;
  }

  ch->desc->marked = vict;

  act("You mark $N for assassination.", FALSE, ch, 0, vict, TO_CHAR);

}
