/* ************************************************************************

*   File: act.movement.c                                Part of CircleMUD *

*  Usage: movement commands, door handling, & sleep/rest/etc state        *

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
#include "house.h"
#include "dg_scripts.h"
#include "constants.h"

/* external vars  */
extern struct room_data *world;
extern struct char_data *character_list;
extern struct descriptor_data *descriptor_list;
extern struct index_data *obj_index;
extern int rev_dir[];
extern int movement_loss[];
extern struct zone_data *zone_table;

/* external functs */
int special(struct char_data *ch, int cmd, char *arg);
void death_cry(struct char_data *ch);
int find_eq_pos(struct char_data * ch, struct obj_data * obj, char *arg);
void continue_track(struct char_data * ch);
void dismount_char(struct char_data * ch);
void mount_char(struct char_data *ch, struct char_data *mount);
int allowNewFollower(struct char_data* ch, int maxFollowerAllowed);
void add_follower(struct char_data * ch, struct char_data * leader);

/*int has_boat(struct char_data *ch)

{

  struct obj_data *obj;

  int i;



  if (IS_AFFECTED(ch, AFF_WATERWALK))

    return 1;

    

  if (IS_AFFECTED(ch, AFF_FLIGHT))

    return 1;



  for (obj = ch->carrying; obj; obj = obj->next_content)

    if (GET_OBJ_TYPE(obj) == ITEM_BOAT && (find_eq_pos(ch, obj, NULL) < 0))

      return 1;



  for (i = 0; i < NUM_WEARS; i++)

    if (GET_EQ(ch, i) && GET_OBJ_TYPE(GET_EQ(ch, i)) == ITEM_BOAT)

      return 1;



  return 0;

} */



int can_fly(struct char_data * ch)
{

  //int i;

  if (IS_AFFECTED(ch, AFF_FLIGHT))
    return 1;

  /*for (i = 0; i < NUM_WEARS; i++)

    if (GET_EQ(ch, i) && GET_OBJ_TYPE(GET_EQ(ch, i)) == ITEM_WINGS)

      return 1; */

  return 0;
}

int has_lung(struct char_data *ch)
{
  int i;

  if (IS_AFFECTED(ch, AFF_WATERBREATH))
    return 1;

  for (i = 0; i < NUM_WEARS; i++)
    if (GET_EQ(ch, i) && GET_OBJ_TYPE(GET_EQ(ch, i)) == ITEM_AQUALUNG)
      return 1;

  return 0;
}                       



/* do_simple_move assumes

 *    1. That there is no master and no followers.

 *    2. That the direction exists.

 *

 *   Returns :

 *   1 : If succes.

 *   0 : If fail

 */

int do_simple_move(struct char_data *ch, int dir, int need_specials_check)

{

  int was_in = 0;

  int need_movement = 0;

  struct char_data *sentry, *tmpch;

  int success, same_room = 0, riding = 0, ridden_by = 0, percent;

  char buf2[MAX_STRING_LENGTH];

  



  int special(struct char_data *ch, int cmd, char *arg);



  /*

   * Check for special routines (North is 1 in command list, but 0 here) Note

   * -- only check if following; this avoids 'double spec-proc' bug

   */



  percent = number(1,101);

  success = number(1,10);



  if (need_specials_check && special(ch, dir + 1, "")) {

    return 0;

  }



  if (RIDING(ch))

    riding = 1;

  if (RIDDEN_BY(ch))

    ridden_by = 1;



 

  /* if they're mounted, are they in the same room w/ their mount(ee)? */

  if (riding && RIDING(ch)->in_room == ch->in_room)

    same_room = 1;

  else if (ridden_by && RIDDEN_BY(ch)->in_room == ch->in_room)

    same_room = 1;



  /* tamed mobiles cannot move about (DAK) */

  if (ridden_by && same_room && AFF_FLAGGED(ch, AFF_TAMED))

 {

    send_to_char("You've been tamed. Now act it!\r\n", ch);

    return 0;

  } 





  /* charmed? */

  if (IS_AFFECTED(ch, AFF_CHARM) &&

      ch->master && ch->in_room == ch->master->in_room) {

    send_to_char("The thought of leaving your master makes you weep.\r\n", ch);

    act("$n bursts into tears.", FALSE, ch, 0, 0, TO_ROOM);

    return 0;

  }



  if (affected_by_spell(ch, SPELL_HOLD_PERSON))

  {

    send_to_char("You are magically held immobile!\r\n", ch);

    act("$n tries to leave, but is held immobile.", FALSE, ch, 0, 0, TO_ROOM);

    return 0;

  }



  if (affected_by_spell(ch, SPELL_PARALYZE))

  {

    send_to_char("You cannot leave while your body is paralyzed.\r\n", ch);

    return 0;

  }



  if (affected_by_spell(ch, SPELL_ENTANGLE))

  {

    send_to_char("The vines and roots entangling you stop movement.\r\n", ch);

    act("$n tries to break free of the entangling vines and roots, but fails.",

         FALSE, ch, 0, 0, TO_ROOM);

    return 0;

  }



  if (affected_by_spell(ch, SPELL_WEB))

  {

    send_to_char("You fail to break though the thick webs.\r\n", ch);

    act("$n tries to break through the thick webs, but fails.",

         FALSE, ch, 0, 0, TO_ROOM);

    return 0;

  }



  if (affected_by_spell(ch, SPELL_MIRE))

  {

    send_to_char("The mire still holds you fast.\r\n", ch);

    act("$n tries to leave, but is still stuck in the mire.",

         FALSE, ch, 0, 0, TO_ROOM);

    return 0;

  }



 if (GET_POS(ch) == POS_DIGGING) {

    send_to_char("But you are digging!\r\n", ch);

    return 0;

   }



  /* if this room or the one we're going to needs a boat, check for one 

  if ((SECT(ch->in_room) == SECT_WATER_NOSWIM) ||

      (SECT(EXIT(ch, dir)->to_room) == SECT_WATER_NOSWIM)) {

if ((riding && !has_boat(RIDING(ch))) || !has_boat(ch)) {      

send_to_char("You need a boat to go there.\r\n", ch);

      return 0;

    }

  } */



  /* if this room or the one we're going to needs fly, check for one */

  if ((SECT(ch->in_room) == SECT_FLYING) ||

      (SECT(EXIT(ch, dir)->to_room) == SECT_FLYING))

  {

    if (!can_fly(ch))

    {

      send_to_char("You're going to need to learn how to fly first.\r\n", ch);

      return 0;

    }

  }



  /* move points needed is avg. move loss for src and destination sect type */

  need_movement = (movement_loss[SECT(ch->in_room)] +

		   movement_loss[SECT(EXIT(ch, dir)->to_room)]) >> 1;



  if (riding) {

    if (GET_MOVE(RIDING(ch)) < need_movement) {

      send_to_char("Your mount is too exhausted.\r\n", ch);

      return 0;

    }

  }



  if (IS_AFFECTED(ch, AFF_FLIGHT))

    need_movement = (need_movement+1)/2;



  if(IS_RANGER(ch) || IS_DRUID(ch))

    need_movement = (need_movement+1)/2;



  if (GET_MOVE(ch) < need_movement && !IS_NPC(ch) && !riding) {

    if (need_specials_check && ch->master) {

      send_to_char("You are too exhausted to follow.\r\n", ch);

    } else {

      send_to_char("You are too exhausted.\r\n", ch);

    }



    return 0;

  }



 if (riding && !use_skill(ch, percent, SKILL_RIDING) && (GET_SKILL(ch, SKILL_RIDING) <= number(1, 55))) { 

    act("$N rears backwards, throwing you to the ground! Ouch!", FALSE, ch, 0, RIDING(ch), TO_CHAR);

    act("You rear backwards, throwing $n to the ground. Ouch!", FALSE, ch, 0, RIDING(ch), TO_VICT);

    act("$N rears backwards, throwing $n to the ground. Ouch!", FALSE, ch, 0, RIDING(ch), TO_NOTVICT);

    dismount_char(ch);

    GET_POS(ch) = POS_SITTING;

    damage(ch, ch, dice(1,6), -1);

    return 0;

  }





  /*  PDH  3/16/99 - PCs can't enter GODROOMs  */

  if (ROOM_FLAGGED(EXIT(ch, dir)->to_room, ROOM_GODROOM) &&

     (GET_LEVEL(ch) < LVL_IMMORT))

  {

    send_to_char("You are not Godly enough to go there.\r\n", ch);

    return 0;

  }





  /*  PDH  4/ 1/99 - TEST char can't enter Open zone, and vice versa  

  if ( PLR_FLAGGED(ch, PLR_TESTCHAR) &&

       zone_table[world[EXIT(ch, dir)->to_room].zone].zone_status ==

       ZONE_OPEN ) {

    send_to_char("You are merely for test purposes.  Stick to non-open zones.\r\n", ch);

    return 0;

  } */



  /*  PDH  4/12/99 - char can oly enter Open zone  

  if ( ! PLR_FLAGGED(ch, PLR_TESTCHAR) &&

       zone_table[world[EXIT(ch, dir)->to_room].zone].zone_status !=

       ZONE_OPEN &&

       GET_LEVEL(ch) < LVL_IMMORT ) {

    send_to_char("You are attempting to enter an area that is not open for play:\r\n", ch);

    send_to_char("Please contact an Immortal.\r\n", ch);

    return 0;

  } */





  if (IS_SET(ROOM_FLAGS(ch->in_room), ROOM_ATRIUM)) {

    if (!House_can_enter(ch, world[EXIT(ch, dir)->to_room].number)) {

      send_to_char("That's private property -- no trespassing!\r\n", ch);

      return 0;

    }

  }



if ((riding || ridden_by) && IS_SET(ROOM_FLAGS(EXIT(ch, dir)->to_room), ROOM_TUNNEL)) {

    send_to_char("There isn't enough room there, while mounted.\r\n", ch);

    return 0;

  } else {

  if (IS_SET(ROOM_FLAGS(EXIT(ch, dir)->to_room), ROOM_TUNNEL) &&

      num_pc_in_room(&(world[EXIT(ch, dir)->to_room])) > 1) {

    send_to_char("There isn't enough room there for more than one person!\r\n",

		 ch);

    return 0;

  }

}



  /*   The check for sentry mobs - Soli, 8/6/99   */

  if (IS_SET(ROOM_FLAGS(EXIT(ch, dir)->to_room), ROOM_GUARDED))

  {

    for (sentry = world[ch->in_room].people; sentry;

         sentry = sentry->next_in_room)

    {

      if (IS_NPC(sentry) && MOB_FLAGGED(sentry, MOB_SENTRY) && (GET_POS(sentry) != POS_SLEEPING && !AFF_FLAGGED(sentry, AFF_KNOCKOUT)))

      {

        act("$N steps in front of you and blocks your way.", FALSE, ch, 0,

             sentry, TO_CHAR);

        act("$N steps in front of $n and blocks $s way.", FALSE, ch, 0, sentry,

             TO_ROOM);

        return 0;

      }

    }

  }



  if (GET_LEVEL(ch) < LVL_IMMORT && !IS_NPC(ch) && !(riding || ridden_by)) {

    GET_MOVE(ch) -= need_movement;

  }



   else if (riding)

    GET_MOVE(RIDING(ch)) -= need_movement;

    else if (ridden_by)

    GET_MOVE(RIDDEN_BY(ch)) -= need_movement;



if (riding) {

        sprintf(buf2, "$n rides $N %s.", dirs[dir]);

        act(buf2, TRUE, ch, 0, RIDING(ch), TO_NOTVICT);

 } else if (ridden_by) {

    if (!AFF_FLAGGED(ch, AFF_TAMED)) {

    act("You rear backwards, throwing $N to the ground!", FALSE, ch, 0, RIDDEN_BY(ch), TO_CHAR); 

    act("$n rears backwards, throwing you to the ground. Ouch!", FALSE, ch, 0, RIDDEN_BY(ch), TO_VICT);

    act("$n rears backwards, throwing $N to the ground. Ouch!", FALSE, ch, 0, RIDDEN_BY(ch), TO_NOTVICT);

    damage(RIDDEN_BY(ch), RIDDEN_BY(ch), dice(1,6), -1);

    dismount_char(RIDDEN_BY(ch));

    GET_POS(ch) = POS_SITTING;

    /*sprintf(buf2, "$n leaves %s.", dirs[dir]);

    act(buf2, TRUE, ch, 0, 0, TO_ROOM); */

    return 0;

  } 

	else {

		sprintf(buf2, "$n rides $N %s.", dirs[dir]);

		act(buf2, TRUE, RIDDEN_BY(ch), 0, ch, TO_NOTVICT);

	}

  } 
  
  else {

    for (tmpch = world[ch->in_room].people; tmpch; tmpch = tmpch->next_in_room)

    {

      if (tmpch != ch && (!IS_AFFECTED(ch, AFF_SNEAK) || (IS_AFFECTED(ch, AFF_SNEAK) &&
	 (!check_skill(ch, 101, SKILL_SNEAK) || AFF_FLAGGED(tmpch, AFF_SENSE_LIFE))))) {
	sprintf(buf2, "$n leaves %s.", dirs[dir]);
	act(buf2, TRUE, ch, 0, tmpch, TO_VICT);
	continue;
      }	  

      if ((tmpch) && (GET_LEVEL(tmpch) >= LVL_IMMORT) && !IS_NPC(tmpch) &&

          (tmpch != ch))

      {

        sprintf(buf, "$n sneaks off to the %s.", dirs[dir]);

        act(buf, TRUE, ch, 0, tmpch, TO_VICT);

      }
	  
	  

    }

  }



  was_in = IN_ROOM(ch);

  char_from_room(ch);

  char_to_room(ch, world[was_in].dir_option[dir]->to_room);

  if (ch->desc && ch->desc->marked) {
    act("You lose your mark on $N.", FALSE, ch, 0, ch->desc->marked, TO_CHAR);
    ch->desc->marked = NULL;
    ch->desc->marked_time = 0;
  }


  /* move them first, then move them back if they aren't allowed to go. */

  /* see if an entry trigger disallows the move */



 if (!entry_mtrigger(ch) || !enter_wtrigger(&world[IN_ROOM(ch)], ch, dir)) {

    char_from_room(ch);

    char_to_room(ch, was_in);

    return 0;

  }



if (riding && same_room && RIDING(ch)->in_room != ch->in_room) {

    char_from_room(RIDING(ch));

    char_to_room(RIDING(ch), world[was_in].dir_option[dir]->to_room);

  } else if (ridden_by && same_room && RIDDEN_BY(ch)->in_room != ch->in_room) {

    char_from_room(RIDDEN_BY(ch));

    char_to_room(RIDDEN_BY(ch), world[was_in].dir_option[dir]->to_room);

  }





  if ((ROOM_FLAGGED(ch->in_room, ROOM_SALTWATER_FISH) ||

       ROOM_FLAGGED(ch->in_room, ROOM_FRESHWATER_FISH)) &&

      (PLR_FLAGGED(ch, PLR_FISHING) || PLR_FLAGGED(ch, PLR_FISH_ON))) {

    REMOVE_BIT(PLR_FLAGS(ch), PLR_FISHING);

    REMOVE_BIT(PLR_FLAGS(ch), PLR_FISH_ON);

    send_to_char("\r\nYou pack up your fishing gear and move on.\r\n\r\n", ch);

  }



 if (!IS_AFFECTED(ch, AFF_SNEAK) || (IS_AFFECTED(ch, AFF_SNEAK) && !check_skill(ch, 101, SKILL_SNEAK))) {

 if (riding && same_room && (!IS_AFFECTED(RIDING(ch), AFF_SNEAK)  || 
   (IS_AFFECTED(RIDING(ch), AFF_SNEAK) && !check_skill(RIDING(ch), 101, SKILL_SNEAK)))) {

    char arrStr[100];

   sprintf(arrStr, "$n has arrived from the %s, riding $N.", dirs[rev_dir[dir]]);

    act(arrStr, TRUE, ch, 0, RIDING(ch), TO_ROOM);

    }



if (ridden_by && same_room && (!IS_AFFECTED(RIDDEN_BY(ch), AFF_SNEAK) || 
   (IS_AFFECTED(RIDDEN_BY(ch), AFF_SNEAK) && !check_skill(RIDDEN_BY(ch), 101, SKILL_SNEAK)))) {

    char arrStr[100];

    sprintf(arrStr, "$n has arrived from the %s, ridden by $N.", dirs[rev_dir[dir]]);

    act(arrStr, TRUE, ch, 0, RIDDEN_BY(ch), TO_ROOM);

    }



else if (!riding || (riding && !same_room)) {

    char arrStr[100];

    sprintf(arrStr, "$n has arrived from the %s.", dirs[rev_dir[dir]]);

    act(arrStr, TRUE, ch, 0, 0, TO_ROOM);

   }

  }





else {

    char arrStr[100];

    sprintf(arrStr, "$n sneaks in from the %s.", dirs[rev_dir[dir]]);

    for (tmpch = world[ch->in_room].people; tmpch; tmpch = tmpch->next_in_room)

    {

      if ((tmpch) && !IS_NPC(tmpch) && (GET_LEVEL(tmpch) >= LVL_IMMORT) &&

          (tmpch != ch))

        act(arrStr, TRUE, ch, 0, tmpch, TO_VICT);

    }

  }





  if (ch->desc != NULL)

    look_at_room(ch, 0);



  if(IS_SET(ROOM_FLAGS(ch->in_room), ROOM_DEATH) && GET_LEVEL(ch)<LVL_IMMORT) {

    log_death_trap(ch);

    death_cry(ch);

    extract_char(ch);

    return 0;

  }



 if(IS_SET(ROOM_FLAGS(ch->in_room), ROOM_DEATH) && riding && GET_LEVEL(RIDING(ch))<LVL_IMMORT) {

    log_death_trap(ch);

    death_cry(ch);

    extract_char(ch);

    return 0;

  }



if(IS_SET(ROOM_FLAGS(ch->in_room), ROOM_DEATH) && ridden_by && GET_LEVEL(RIDDEN_BY(ch))<LVL_IMMORT) {

    log_death_trap(ch);

    death_cry(ch);

    extract_char(ch);

    return 0;

  }







  if (!greet_mtrigger(ch, dir)) {

    char_from_room(ch);

    char_to_room(ch, was_in);

    look_at_room(ch, 0);

  }



  for (tmpch = world[ch->in_room].people; tmpch; tmpch = tmpch->next_in_room)

  {

    if (affected_by_spell(tmpch, SPELL_MIRE))

    {

      struct affected_type new_af;

      struct affected_type *af;

      int duration = 0;

      for (af = tmpch->affected; af; af = af->next)

      {

        if (af->type == SPELL_MIRE)

          duration = af->duration;

      }

      send_to_char("You find that this room has become a mire.\r\n", ch);

      new_af.type = SPELL_MIRE;

      new_af.bitvector = 0;

      new_af.duration = duration;

      new_af.modifier = 0;

      new_af.location = APPLY_NONE;



      affect_to_char(ch, &new_af);

      break;

    }

  }



  if (HUNTING(ch) && !IS_NPC(ch))
    continue_track(ch);

  return 1;
}


int perform_move(struct char_data *ch, int dir, int need_specials_check) {
    log("mobile_activity->perform_move");
    int was_in;
    struct follow_type *k, *next;

    log("mobile_activity->perform_move->fishing");
    if (GET_POS(ch) == POS_FISHING) {
        send_to_char("But you are fishing!\r\n", ch);
        return 0;
    }

    log("mobile_activity->perform_move->fighting");
    if (GET_POS(ch) == POS_DIGGING) {
        send_to_char("But you are digging!\r\n", ch);
        return 0;
    }

    log("mobile_activity->perform_move->riding");
    if (RIDING(ch)) {
        if (GET_POS(RIDING(ch)) == POS_RESTING || GET_POS(RIDING(ch)) == POS_SITTING) {
            send_to_char("But your mount is resting!\r\n", ch);
            return 0;
        }
    }

    if (ch == NULL || dir < 0 || dir >= NUM_OF_DIRS) {
        log("Null char or invalid direction.");
        return 0;

    } else if (!EXIT(ch, dir) || EXIT(ch, dir)->to_room == NOWHERE) {
        log("Can't go that way.");
        send_to_char("Alas, you cannot go that way...\r\n", ch);

    } else if (IS_SET(EXIT(ch, dir)->exit_info, EX_CLOSED)) {
        if (EXIT(ch, dir)->keyword && !IS_SET(EXIT(ch, dir)->exit_info, EX_HIDDEN)) {
            log("Door closed.");
            sprintf(buf2, "The %s seems to be closed.\r\n", fname(EXIT(ch, dir)->keyword));
            send_to_char(buf2, ch);
        } else if (!IS_SET(EXIT(ch, dir)->exit_info, EX_HIDDEN)) {
            log("Hidden door closed.");
            send_to_char("It seems to be closed.\r\n", ch);
        } else {
            log("Can't got that way 2.");
            send_to_char("Alas, you cannot go that way...\r\n", ch);
        }
    } else {

        if (!ch->followers) {
            return (do_simple_move(ch, dir, need_specials_check));
        }

        was_in = ch->in_room;

        if (!do_simple_move(ch, dir, need_specials_check)) {
            return 0;
        }

        log("For loop");
        for (k = ch->followers; k; k = next) {
            next = k->next;

            if ((k->follower->in_room == was_in) &&
                (GET_POS(k->follower) >= POS_STANDING) &&
                CAN_SEE(k->follower, ch)) {
                    act("You follow $N.\r\n", FALSE, k->follower, 0, ch, TO_CHAR);
                    perform_move(k->follower, dir, 1);
            }

        }
        log("For loop end.");

        return 1;
    }

  return 0;
  log("End move");
}

ACMD(do_move)

{

  /*

   * This is basically a mapping of cmd numbers to perform_move indices.

   * It cannot be done in perform_move because perform_move is called

   * by other functions which do not require the remapping.

   */

  perform_move(ch, cmd - 1, 0);

}



/*

 * The following four are a hack since perform_move interprets additional

 * directions (even as shortcuts) to be different from existing directions.

 * To prevent chars from needing aliases, hardcoding the directions was 

 * necessary, for now at least. Don't like it?  Rewrite it.

 * It's quick and dirty, but it WORKS.  I like quick.  Let's me code other

 * stuff.  I'll make it clean later.

 */



ACMD(do_nw)

{

perform_move(ch, NORTHWEST, 0);

}

ACMD(do_ne)

{

perform_move(ch, NORTHEAST, 0);

}

ACMD(do_sw)

{

perform_move(ch, SOUTHWEST, 0);

}

ACMD(do_se)

{

perform_move(ch, SOUTHEAST, 0);

}





int find_door(struct char_data *ch, char *type, char *dir, char *cmdname)

{

  int door;



  if (*dir) {			/* a direction was specified */



    /*

    if ((door1 = search_block(dir, dirs, FALSE) == -1) &&

	(door2 = search_block(dir, sdirs, FALSE) == -1)) {



    if ( (door = search_block(dir, sdirs, FALSE)) == -1 ) {



      */



    if ( (door = search_block(dir, doordirs, FALSE)) == -1 ) {



      send_to_char("That's not a direction.\r\n", ch);

      return -1;

    }



    if ( door > 9 ) {

      door -= 4;

    }



    /*

    if ( door1 != -1 ) {

      door = door1;

    } else if ( door2 != -1 ) {

      door = door2;

    } else {

      log("SYSERR: door was found, then is wasn't!");

      return -1;

    }

    */



    /* {char tmp[100]; sprintf(tmp, "find_door(): door1 %d  door2 %d  door %d", door1, door2, door); log(tmp); } */



    if (EXIT(ch, door)) {

      if (EXIT(ch, door)->keyword) {

	if (isname(type, EXIT(ch, door)->keyword)) {

	  return door;

	} else {

	  sprintf(buf2, "I see no %s there.\r\n", type);

	  send_to_char(buf2, ch);

	  return -1;

	}

      } else {

	return door;

      }



    } else {

      send_to_char("I really don't see how you can open or close anything there.\r\n",

		   ch);

      return -1;

    }



  } else {			/* try to locate the keyword */

    if (!*type) {

      sprintf(buf2, "What is it you want to %s?\r\n", cmdname);

      send_to_char(buf2, ch);

      return -1;

    }



    for (door = 0; door < NUM_OF_DIRS; door++)

      if (EXIT(ch, door))

	if (EXIT(ch, door)->keyword)

	  if (isname(type, EXIT(ch, door)->keyword))

	    return door;



    sprintf(buf2, "There doesn't seem to be %s %s here.\r\n", AN(type), type);

    send_to_char(buf2, ch);

    return -1;

  }

}





int has_key(struct char_data *ch, int key)

{

  struct obj_data *o;



  for (o = ch->carrying; o; o = o->next_content)

    if (GET_OBJ_VNUM(o) == key)

      return 1;



  if (GET_EQ(ch, WEAR_HOLD))

    if (GET_OBJ_VNUM(GET_EQ(ch, WEAR_HOLD)) == key)

      return 1;



  return 0;

}







#define NEED_OPEN	1

#define NEED_CLOSED	2

#define NEED_UNLOCKED	4

#define NEED_LOCKED	8



char *cmd_door[] =

{

  "open",

  "close",

  "unlock",

  "lock",

  "pick"

};



const int flags_door[] =

{

  NEED_CLOSED | NEED_UNLOCKED,

  NEED_OPEN,

  NEED_CLOSED | NEED_LOCKED,

  NEED_CLOSED | NEED_UNLOCKED,

  NEED_CLOSED | NEED_LOCKED

};





#define EXITN(room, door)		(world[room].dir_option[door])

#define OPEN_DOOR(room, obj, door)	((obj) ? (TOGGLE_BIT(GET_OBJ_VAL(obj, 1), CONT_CLOSED)) : (TOGGLE_BIT(EXITN(room, door)->exit_info, EX_CLOSED)))

#define LOCK_DOOR(room, obj, door)	((obj) ? (TOGGLE_BIT(GET_OBJ_VAL(obj, 1), CONT_LOCKED)) : (TOGGLE_BIT(EXITN(room, door)->exit_info, EX_LOCKED)))



void do_doorcmd(struct char_data *ch, struct obj_data *obj, int door, int scmd)

{

  int other_room = 0;

  struct room_direction_data *back = 0;



  sprintf(buf, "$n %ss ", cmd_door[scmd]);

  if (!obj && ((other_room = EXIT(ch, door)->to_room) != NOWHERE))

    if ((back = world[other_room].dir_option[rev_dir[door]]))

      if (back->to_room != ch->in_room)

	back = 0;



  switch (scmd) {

  case SCMD_OPEN:

  case SCMD_CLOSE:

    OPEN_DOOR(ch->in_room, obj, door);

    if (back)

      OPEN_DOOR(other_room, obj, rev_dir[door]);

    send_to_char(OK, ch);

    break;

  case SCMD_UNLOCK:

  case SCMD_LOCK:

    LOCK_DOOR(ch->in_room, obj, door);

    if (back)

      LOCK_DOOR(other_room, obj, rev_dir[door]);

    send_to_char("*Click*\r\n", ch);

    break;

  case SCMD_PICK:

    LOCK_DOOR(ch->in_room, obj, door);

    if (back)

      LOCK_DOOR(other_room, obj, rev_dir[door]);

    send_to_char("The lock quickly yields to your skills.\r\n", ch);

    strcpy(buf, "$n skillfully picks the lock on ");

    break;

  }



  /* Notify the room */

  sprintf(buf + strlen(buf), "%s%s.", ((obj) ? "" : "the "), (obj) ? "$p" :

	  (EXIT(ch, door)->keyword ? "$F" : "door"));

  if (!(obj) || (obj->in_room != NOWHERE))

    act(buf, FALSE, ch, obj, obj ? 0 : EXIT(ch, door)->keyword, TO_ROOM);



  /* Notify the other room */

  if ((scmd == SCMD_OPEN || scmd == SCMD_CLOSE) && back) {

    sprintf(buf, "The %s is %s%s from the other side.\r\n",

	 (back->keyword ? fname(back->keyword) : "door"), cmd_door[scmd],

	    (scmd == SCMD_CLOSE) ? "d" : "ed");

    if (world[EXIT(ch, door)->to_room].people) {

      act(buf, FALSE, world[EXIT(ch, door)->to_room].people, 0, 0, TO_ROOM);

      act(buf, FALSE, world[EXIT(ch, door)->to_room].people, 0, 0, TO_CHAR);

    }

  }

}





int ok_pick(struct char_data *ch, int keynum, int pickproof, int scmd)

{

  if (scmd == SCMD_PICK) {

    if (keynum < 0)

      send_to_char("Odd - you can't seem to find a keyhole.\r\n", ch);

    else if (pickproof)

      send_to_char("It resists your attempts to pick it.\r\n", ch);

    else if (!check_skill(ch, 101, SKILL_PICK_LOCK))

      send_to_char("You failed to pick the lock.\r\n", ch);

    else

      return (1);

    return (0);

  }

  return (1);

}





#define DOOR_IS_OPENABLE(ch, obj, door)	((obj) ? ((GET_OBJ_TYPE(obj) == ITEM_CONTAINER) && (IS_SET(GET_OBJ_VAL(obj, 1), CONT_CLOSEABLE))) : (IS_SET(EXIT(ch, door)->exit_info, EX_ISDOOR)))

#define DOOR_IS_OPEN(ch, obj, door)	((obj) ? (!IS_SET(GET_OBJ_VAL(obj, 1), CONT_CLOSED)) : (!IS_SET(EXIT(ch, door)->exit_info, EX_CLOSED)))

#define DOOR_IS_UNLOCKED(ch, obj, door)	((obj) ? (!IS_SET(GET_OBJ_VAL(obj, 1), CONT_LOCKED)) : (!IS_SET(EXIT(ch, door)->exit_info, EX_LOCKED)))

#define DOOR_IS_PICKPROOF(ch, obj, door) ((obj) ? \
			(IS_SET(GET_OBJ_VAL(obj, 1), CONT_PICKPROOF)) : \
			(IS_SET(EXIT(ch, door)->exit_info, EX_PICKPROOF)))



#define DOOR_IS_CLOSED(ch, obj, door)	(!(DOOR_IS_OPEN(ch, obj, door)))
#define DOOR_IS_LOCKED(ch, obj, door)	(!(DOOR_IS_UNLOCKED(ch, obj, door)))
#define DOOR_KEY(ch, obj, door)		((obj) ? (GET_OBJ_VAL(obj, 2)) : \
					(EXIT(ch, door)->key))
#define DOOR_LOCK(ch, obj, door)	((obj) ? (GET_OBJ_VAL(obj, 1)) : \
					(EXIT(ch, door)->exit_info))



ACMD(do_gen_door)

{

  int door = -1, keynum;

  char type[MAX_INPUT_LENGTH], dir[MAX_INPUT_LENGTH];

  struct obj_data *obj = NULL;

  struct char_data *victim = NULL;



  skip_spaces(&argument);

  if (!*argument) {

    sprintf(buf, "%s what?\r\n", cmd_door[subcmd]);

    send_to_char(CAP(buf), ch);

    return;

  }



  two_arguments(argument, type, dir);



  if (!generic_find(type, FIND_OBJ_INV | FIND_OBJ_ROOM, ch, &victim, &obj)) {

    door = find_door(ch, type, dir, cmd_door[subcmd]);

  }



  if ((obj) || (door >= 0)) {

    keynum = DOOR_KEY(ch, obj, door);

    if (!(DOOR_IS_OPENABLE(ch, obj, door)))

      act("You can't $F that!", FALSE, ch, 0, cmd_door[subcmd], TO_CHAR);

    else if (!DOOR_IS_OPEN(ch, obj, door) &&

	     IS_SET(flags_door[subcmd], NEED_OPEN))

      send_to_char("But it's already closed!\r\n", ch);

    else if (!DOOR_IS_CLOSED(ch, obj, door) &&

	     IS_SET(flags_door[subcmd], NEED_CLOSED))

      send_to_char("But it's currently open!\r\n", ch);

    else if (!(DOOR_IS_LOCKED(ch, obj, door)) &&

	     IS_SET(flags_door[subcmd], NEED_LOCKED))

      send_to_char("Oh.. it wasn't locked, after all..\r\n", ch);

    else if (!(DOOR_IS_UNLOCKED(ch, obj, door)) &&

	     IS_SET(flags_door[subcmd], NEED_UNLOCKED))

      send_to_char("It seems to be locked.\r\n", ch);

    else if (!has_key(ch, keynum) && (GET_LEVEL(ch) < LVL_GOD) &&

	     ((subcmd == SCMD_LOCK) || (subcmd == SCMD_UNLOCK)))

      send_to_char("You don't seem to have the proper key.\r\n", ch);

    else if (ok_pick(ch, keynum, DOOR_IS_PICKPROOF(ch, obj, door), subcmd))

      do_doorcmd(ch, obj, door, subcmd);

  }

  return;

}







ACMD(do_enter)

{

  int door, portDestRoom = NOWHERE;

  struct obj_data * portal;





  one_argument(argument, buf);



  if (*buf) {			/* an argument was supplied, search for door

				 * or portal keyword */

    for (door = 0; door < NUM_OF_DIRS; door++) {

      if (EXIT(ch, door)) {

	if (EXIT(ch, door)->keyword) {

	  if (!str_cmp(EXIT(ch, door)->keyword, buf)) {

	    perform_move(ch, door, 1);

	    return;

	  }

	}

      }

    }



    /*  PDH  4/27/99

     *  now scan room for portal

     */

    if ( (portal=get_obj_in_list_vis(ch, buf, world[ch->in_room].contents)) &&

	 ( GET_OBJ_VNUM(portal) == PORTAL_OBJ ) ) {

      portDestRoom = GET_OBJ_VAL(portal, 0);



      if ( portDestRoom < 0 ) {

	log("SYSERR: portal destination room is bogus.");

	send_to_char("Problem with your portal - contact an Imm please.\r\n",

		     ch);

	return;



      } else {

	sprintf(buf2, "$n enters %s.", portal->short_description);

	act(buf2, TRUE, ch, 0, 0, TO_ROOM);



	char_from_room(ch);

	char_to_room(ch, portDestRoom);

	if (ch->desc != NULL) {

	  look_at_room(ch, 0);

	}



	sprintf(buf2, "$n drifts out of %s.", portal->short_description);

	act(buf2, TRUE, ch, 0, 0, TO_ROOM);



	return;

      }



    } else {

      send_to_char("That is not a portal.\r\n", ch);

      return;

    }



    sprintf(buf2, "There is no %s here.\r\n", buf);

    send_to_char(buf2, ch);



  } else if (IS_SET(ROOM_FLAGS(ch->in_room), ROOM_INDOORS)) {

    send_to_char("You are already indoors.\r\n", ch);



  } else {

    /* try to locate an entrance */

    for (door = 0; door < NUM_OF_DIRS; door++)

      if (EXIT(ch, door))

	if (EXIT(ch, door)->to_room != NOWHERE)

	  if (!IS_SET(EXIT(ch, door)->exit_info, EX_CLOSED) &&

	      IS_SET(ROOM_FLAGS(EXIT(ch, door)->to_room), ROOM_INDOORS)) {

	    perform_move(ch, door, 1);

	    return;

	  }

    send_to_char("You can't seem to find anything to enter.\r\n", ch);

  }

}





ACMD(do_leave)

{

  int door;



  if (!IS_SET(ROOM_FLAGS(ch->in_room), ROOM_INDOORS))

    send_to_char("You are outside.. where do you want to go?\r\n", ch);

  else {

    for (door = 0; door < NUM_OF_DIRS; door++)

      if (EXIT(ch, door))

	if (EXIT(ch, door)->to_room != NOWHERE)

	  if (!IS_SET(EXIT(ch, door)->exit_info, EX_CLOSED) &&

	    !IS_SET(ROOM_FLAGS(EXIT(ch, door)->to_room), ROOM_INDOORS)) {

	    perform_move(ch, door, 1);

	    return;

	  }

    send_to_char("I see no obvious exits to the outside.\r\n", ch);

  }

}





ACMD(do_stand)

{

  switch (GET_POS(ch)) {

  case POS_RIDING:

    act("But you are riding!", FALSE, ch, 0, 0, TO_CHAR);

    break;

  case POS_STANDING:

    act("You are already standing.", FALSE, ch, 0, 0, TO_CHAR);

    break;

  case POS_DIGGING:

    act("Do you not consider digging as standing?", FALSE, ch, 0, 0, TO_CHAR);

    break;

  case POS_FISHING:

    act("But you are fishing standing up!", FALSE, ch, 0, 0, TO_CHAR);

    break;

  case POS_SITTING:

    if (affected_by_spell(ch, SPELL_CRIPPLE))

    {

      send_to_char("You try to stand, but feel too crippled.\r\n", ch);

      return;

    }

    act("You stand up.", FALSE, ch, 0, 0, TO_CHAR);

    act("$n clambers to $s feet.", TRUE, ch, 0, 0, TO_ROOM);

    GET_POS(ch) = POS_STANDING;

    break;

  case POS_RESTING:

    if (affected_by_spell(ch, SPELL_CRIPPLE))

    {

      send_to_char("You try to stand, but feel too crippled.\r\n", ch);

      return;

    }

    act("You stop resting, and stand up.", FALSE, ch, 0, 0, TO_CHAR);

    act("$n stops resting, and clambers on $s feet.", TRUE, ch, 0, 0, TO_ROOM);

    GET_POS(ch) = POS_STANDING;

    break;

  case POS_SLEEPING:

    act("You have to wake up first!", FALSE, ch, 0, 0, TO_CHAR);

    break;

  case POS_FIGHTING:

    act("Do you not consider fighting as standing?", FALSE, ch, 0, 0, TO_CHAR);

    break;

  default:

    act("You stop floating around, and put your feet on the ground.",

	FALSE, ch, 0, 0, TO_CHAR);

    act("$n stops floating around, and puts $s feet on the ground.",

	TRUE, ch, 0, 0, TO_ROOM);

    GET_POS(ch) = POS_STANDING;

    break;

  }



  ch->standcounter = 0;

}





ACMD(do_sit)

{

  switch (GET_POS(ch)) {

  case POS_STANDING:

    act("You sit down.", FALSE, ch, 0, 0, TO_CHAR);

    act("$n sits down.", FALSE, ch, 0, 0, TO_ROOM);

    GET_POS(ch) = POS_SITTING;

    break;

  case POS_DIGGING:

    act("But you're digging!", FALSE, ch, 0, 0, TO_CHAR);

    break;

  case POS_FISHING:

    act("But you're fishing!", FALSE, ch, 0, 0, TO_CHAR);

    break;

  case POS_RIDING:

    act("But you're sitting atop your mount!", FALSE, ch, 0, 0, TO_CHAR);

    break;

  case POS_SITTING:

    send_to_char("You're sitting already.\r\n", ch);

    break;

  case POS_RESTING:

    if (affected_by_spell(ch, SPELL_CRIPPLE))

    {

      send_to_char("You try to sit up, but you're still too crippled.\r\n", ch);

      return;

    }

    act("You stop resting, and sit up.", FALSE, ch, 0, 0, TO_CHAR);

    act("$n stops resting.", TRUE, ch, 0, 0, TO_ROOM);

    GET_POS(ch) = POS_SITTING;

    break;

  case POS_SLEEPING:

    act("You have to wake up first.", FALSE, ch, 0, 0, TO_CHAR);

    break;

  case POS_FIGHTING:

    act("Sit down while fighting? are you MAD?", FALSE, ch, 0, 0, TO_CHAR);

    break;

  default:

    act("You stop floating around, and sit down.", FALSE, ch, 0, 0, TO_CHAR);

    act("$n stops floating around, and sits down.", TRUE, ch, 0, 0, TO_ROOM);

    GET_POS(ch) = POS_SITTING;

    break;

  }

}





ACMD(do_rest)

{

  switch (GET_POS(ch)) {

  case POS_RIDING:

 act("You settle yourself more comfortably atop your mount.", FALSE, ch, 0, 0, TO_CHAR);

 act("$n settles $mself more comfortably atop $s mount .", TRUE, ch, 0, 0, TO_ROOM);

  case POS_STANDING:

    act("You sit down and rest your tired bones.", FALSE, ch, 0, 0, TO_CHAR);

    act("$n sits down and rests.", TRUE, ch, 0, 0, TO_ROOM);

    GET_POS(ch) = POS_RESTING;

    break;

  case POS_DIGGING:

    act("But you are digging!", FALSE, ch, 0, 0, TO_CHAR);

    break;

  case POS_FISHING:

    act("But you are fishing!", FALSE, ch, 0, 0, TO_CHAR);

    break;

  case POS_SITTING:

    act("You rest your tired bones.", FALSE, ch, 0, 0, TO_CHAR);

    act("$n rests.", TRUE, ch, 0, 0, TO_ROOM);

    GET_POS(ch) = POS_RESTING;

    break;

  case POS_RESTING:

    act("You are already resting.", FALSE, ch, 0, 0, TO_CHAR);

    break;

  case POS_SLEEPING:

    act("You have to wake up first.", FALSE, ch, 0, 0, TO_CHAR);

    break;

  case POS_FIGHTING:

    act("Rest while fighting?  Are you MAD?", FALSE, ch, 0, 0, TO_CHAR);

    break;

  default:

    act("You stop floating around, and stop to rest your tired bones.",

	FALSE, ch, 0, 0, TO_CHAR);

    act("$n stops floating around, and rests.", FALSE, ch, 0, 0, TO_ROOM);

    GET_POS(ch) = POS_SITTING;

    break;

  }

}





ACMD(do_sleep)

{

  switch (GET_POS(ch)) {

  case POS_STANDING:

  case POS_SITTING:

    send_to_char("You go to sleep.\r\n", ch);

    act("$n lies down and falls asleep.", TRUE, ch, 0, 0, TO_ROOM);

    GET_POS(ch) = POS_SLEEPING;

    break;

  case POS_RIDING:

  send_to_char("You go to sleep atop your mount.\r\n", ch);

    act("$n falls asleep atop $s mount.", TRUE, ch, 0, 0, TO_ROOM);

    GET_POS(ch) = POS_SLEEPING;

    break;

 case POS_DIGGING:

    act("But you are digging!", FALSE, ch, 0, 0, TO_CHAR);

    break;

  case POS_FISHING:

    act("But you are fishing!", FALSE, ch, 0, 0, TO_CHAR);

    break;

  case POS_RESTING:

    send_to_char("You go to sleep.\r\n", ch);

    act("$n lies down and falls asleep.", TRUE, ch, 0, 0, TO_ROOM);

    GET_POS(ch) = POS_SLEEPING;

    break;

  case POS_SLEEPING:

    send_to_char("You are already sound asleep.\r\n", ch);

    break;

  case POS_FIGHTING:

    send_to_char("Sleep while fighting?  Are you MAD?\r\n", ch);

    break;

  default:

    act("You stop floating around, and lie down to sleep.",

	FALSE, ch, 0, 0, TO_CHAR);

    act("$n stops floating around, and lie down to sleep.",

	TRUE, ch, 0, 0, TO_ROOM);

    GET_POS(ch) = POS_SLEEPING;

    break;

  }



  ch->standcounter = 0;

}





ACMD(do_wake)

{

  struct char_data *vict;

  int self = 0;



  one_argument(argument, arg);

  if (*arg) {

    if (GET_POS(ch) == POS_SLEEPING)

      send_to_char("Maybe you should wake yourself up first.\r\n", ch);

    else if ((vict = get_char_vis(ch, arg, FIND_CHAR_ROOM)) == NULL)

      send_to_char(NOPERSON, ch);

    else if (vict == ch)

      self = 1;

    else if (GET_POS(vict) > POS_SLEEPING)

      act("$E is already awake.", FALSE, ch, 0, vict, TO_CHAR);

    else if (IS_AFFECTED(vict, AFF_SLEEP))

      act("You can't wake $M up!", FALSE, ch, 0, vict, TO_CHAR);

    else if (IS_AFFECTED(vict, AFF_KNOCKOUT))

      act("$N is unconscious..you can't wake $M up!", FALSE, ch, 0, vict, TO_CHAR);

    else if (GET_POS(vict) < POS_SLEEPING)

      act("$E's in pretty bad shape!", FALSE, ch, 0, vict, TO_CHAR);

    else {

      act("You wake $M up.", FALSE, ch, 0, vict, TO_CHAR);

      act("You are awakened by $n.", FALSE, ch, 0, vict, TO_VICT | TO_SLEEP);

      if (affected_by_spell(vict, SPELL_CRIPPLE))

        GET_POS(vict) = POS_RESTING;

      if (RIDING(ch))

        GET_POS(vict) = POS_RIDING;

      else

        GET_POS(vict) = POS_SITTING;

    }

    if (!self)

      return;

  }

  if (IS_AFFECTED(ch, AFF_SLEEP))

    send_to_char("You can't wake up!\r\n", ch);

  else if (IS_AFFECTED(ch, AFF_KNOCKOUT))

    send_to_char("You are unconscious..you can't wake up!\r\n", ch);



  else if (GET_POS(ch) > POS_SLEEPING)

    send_to_char("You are already awake...\r\n", ch);

  else {

    act("$n awakens.", TRUE, ch, 0, 0, TO_ROOM);

    if (affected_by_spell(ch, SPELL_CRIPPLE))

    {

      send_to_char("You awaken, still resting your legs.\r\n", ch);

      GET_POS(ch) = POS_RESTING;

    }

    else if (RIDING(ch)) {

        send_to_char("You awaken, and sit up atop your mount.\r\n", ch);

      GET_POS(ch) = POS_RIDING;

    }

 else

    {

      send_to_char("You awaken, and sit up.\r\n", ch);

      GET_POS(ch) = POS_SITTING;

    }

  }

}





ACMD(do_follow)

{

  struct char_data *leader;



  void stop_follower(struct char_data *ch);

  void add_follower(struct char_data *ch, struct char_data *leader);



  one_argument(argument, buf);



  if (*buf) {

    if (!(leader = get_char_vis(ch, buf, FIND_CHAR_ROOM))) {

      send_to_char(NOPERSON, ch);

      return;

    }

  } else {

    send_to_char("Whom do you wish to follow?\r\n", ch);

    return;

  }



  if (ch->master == leader) {

    act("You are already following $M.", FALSE, ch, 0, leader, TO_CHAR);

    return;

  }



  if (IS_AFFECTED(ch, AFF_CHARM) && (ch->master)) {

    act("But you only feel like following $N!", FALSE, ch, 0, ch->master, TO_CHAR);



  } else {			/* Not Charmed follow person */

    if (leader == ch) {

      if (!ch->master) {

	send_to_char("You are already following yourself.\r\n", ch);

	return;

      }

      stop_follower(ch);



    } else {

      if (circle_follow(ch, leader)) {

	act("Sorry, but following in loops is not allowed.", FALSE, ch, 0, 0, TO_CHAR);

	return;

      }



      if (ch->master) {

	stop_follower(ch);

      }



      REMOVE_BIT(AFF_FLAGS(ch), AFF_GROUP);
      max_group_exp_mult(ch);
      GET_EXP_MULT(ch) = 0;

      add_follower(ch, leader);

    }

  }



}



/* Mounts (DAK) */

ACMD(do_mount) {

  char arg[MAX_INPUT_LENGTH];

  struct char_data *vict;

  int chance;

  one_argument(argument, arg);

  

  if (!arg || !*arg) {

    send_to_char("Mount who?\r\n", ch);

    return;

  } else if (!(vict = get_char_room_vis(ch, arg))) {

    send_to_char("There is no-one by that name here.\r\n", ch);

    return;

  } else if (!IS_NPC(vict)) {

    send_to_char("Ehh... no.\r\n", ch);

    return;

  } else if (RIDING(ch) || RIDDEN_BY(ch)) {

    send_to_char("You are already mounted.\r\n", ch);

    return;

  } else if (RIDING(vict) || RIDDEN_BY(vict)) {

    send_to_char("It is already mounted.\r\n", ch);

    return;

  } else if (GET_LEVEL(ch) < LVL_IMMORT && IS_NPC(vict) && !MOB_FLAGGED(vict, MOB_MOUNTABLE)) {

    send_to_char("You can't mount that!\r\n", ch);

    return;

  } else if (GET_SKILL(ch, SKILL_RIDING) <= number(1, 70)) {

    act("You try to mount $N, but slip and fall off. Ouch! Brush up on your riding skills first!", FALSE, ch, 0, vict, TO_CHAR);

    act("$n tries to mount you, but slips and falls off. Ouch!", FALSE, ch, 0, vict, TO_VICT);

    act("$n tries to mount $N, but slips and falls off. Ouch!", TRUE, ch, 0, vict, TO_NOTVICT);

    damage(ch, ch, dice(1, 2), -1);

    return;

  }

  

  act("You mount $N.", FALSE, ch, 0, vict, TO_CHAR);

  act("$n mounts you.", FALSE, ch, 0, vict, TO_VICT);

  act("$n mounts $N.", TRUE, ch, 0, vict, TO_NOTVICT);



 if (vict->master != ch) {

  if (vict->master)

      stop_follower(vict);



    add_follower(vict, ch);

 }



  GET_POS(ch) = POS_RIDING;



  mount_char(ch, vict);

  

  if (IS_NPC(vict) && !AFF_FLAGGED(vict, AFF_TAMED) &&!AFF_FLAGGED(vict, AFF_CHARM) && GET_SKILL(ch, SKILL_RIDING) <= number(1, 55)) {

    act("$N suddenly bucks upwards, throwing you violently to the ground!", FALSE, ch, 0, vict, TO_CHAR);

    act("$n is thrown to the ground as $N violently bucks!", TRUE, ch, 0, vict, TO_NOTVICT);

    act("You buck violently and throw $n to the ground.", FALSE, ch, 0, vict, TO_VICT);

    dismount_char(ch);

    GET_POS(ch) = POS_SITTING;



    chance = number(1, 10);



    if (chance <= 3) 

    damage(vict, ch, dice(1,3), -1);



    else

     damage(ch, ch, dice(1,3), -1);



  }

}





ACMD(do_dismount) {

  char arg[MAX_INPUT_LENGTH];

  struct char_data *vict;



  one_argument(argument, arg);



  if (!arg || !*arg) {

    send_to_char("Dismount from who?\r\n", ch);

    return;

  } else if (!(vict = get_char_room_vis(ch, arg))) {

    send_to_char("There is no-one by that name here.\r\n", ch);

    return;

  }

    else if (!RIDING(ch)) {

    send_to_char("You aren't even riding anything.\r\n", ch);

    return;

  } else if (SECT(ch->in_room) == SECT_WATER_NOSWIM) {

    send_to_char("Yah, right, and then drown...\r\n", ch);

    return;

  }

  

  act("You dismount $N.", FALSE, ch, 0, RIDING(ch), TO_CHAR);

  act("$n dismounts from you.", FALSE, ch, 0, RIDING(ch), TO_VICT);

  act("$n dismounts $N.", TRUE, ch, 0, RIDING(ch), TO_NOTVICT);





  GET_POS(ch) = POS_STANDING;



/*if ((vict->master == ch) || AFF_FLAGGED(vict, AFF_CHARM) || AFF_FLAGGED(vict, AFF_TAMED)){

   

   if (!allowNewFollower(ch, 3))

    {

      return;

    }

                                                                                

    add_follower(vict, ch);

} */

 

   dismount_char(ch);

}





ACMD(do_buck) {

  if (!RIDDEN_BY(ch)) {

    send_to_char("You're not even being ridden!\r\n", ch);

    return;

  } else if (AFF_FLAGGED(ch, AFF_TAMED) || AFF_FLAGGED(ch, AFF_CHARM)){

    send_to_char("But you're tamed!\r\n", ch);

    return;

  }

  

  act("You quickly buck, throwing $N to the ground.", FALSE, ch, 0, RIDDEN_BY(ch), TO_CHAR);

  act("$n quickly bucks, throwing you to the ground.", FALSE, ch, 0, RIDDEN_BY(ch), TO_VICT);

  act("$n quickly bucks, throwing $N to the ground.", FALSE, ch, 0, RIDDEN_BY(ch), TO_NOTVICT);

  GET_POS(RIDDEN_BY(ch)) = POS_SITTING;

  if (number(0, 4)) {

    send_to_char("You hit the ground hard!\r\n", RIDDEN_BY(ch));

    damage(RIDDEN_BY(ch), RIDDEN_BY(ch), dice(2,4), -1);

  }

  dismount_char(ch);

  

  

  /*

   * you might want to call set_fighting() or some nonsense here if you

   * want the mount to attack the unseated rider or vice-versa.

   */

}



ACMD(do_tame) {

  char arg[MAX_INPUT_LENGTH];

  struct affected_type af;

  struct char_data *vict;

  int percent;



  percent = number(1, 101) - GET_CHA(ch) ;





  one_argument(argument, arg);





  if (!arg || !*arg) {

    send_to_char("Tame who?\r\n", ch);

    return;

  } else if (!(vict = get_char_room_vis(ch, arg))) {

    send_to_char("They're not here.\r\n", ch);

    return;

  } else if (GET_LEVEL(ch) < LVL_IMMORT && IS_NPC(vict) && !MOB_FLAGGED(vict, MOB_MOUNTABLE)) {

    send_to_char("You can't do that to them.\r\n", ch);

    return;

  } else if (!GET_SKILL(ch, SKILL_TAME)) {

    send_to_char("You don't even know how to tame something.\r\n", ch);

    return;

  } else if (!IS_NPC(vict) && GET_LEVEL(ch) < LVL_IMMORT) {

    send_to_char("You can't do that.\r\n", ch);

    return;

  } else if (RIDING(vict) || RIDDEN_BY(vict)) {

    send_to_char("But they are mounted by someone!\r\n", ch);

    return;

  } else if (IS_AFFECTED(vict, AFF_CHARM) && vict->master != ch ){

    send_to_char("They have already a master.\r\n", ch);

    return;

  }

  else if (IS_AFFECTED(vict, AFF_TAMED) && vict->master != ch){

    send_to_char("They have already a master.\r\n", ch);

    return;

  }

  else if ((IS_AFFECTED(vict, AFF_CHARM) || IS_AFFECTED(vict, AFF_TAMED)) && vict->master == ch ){

    send_to_char("You've already tamed them!\r\n", ch);

    return;

  } else if (IS_AFFECTED(ch, AFF_CHARM)){

    send_to_char("Your master might not approve of that!.\r\n", ch);

    return;

  } else if (!use_skill(ch, percent, SKILL_TAME))  {

    send_to_char("You fail to tame it.\r\n", ch);

    return;

  }



 else {



    if (!allowNewFollower(ch, 3))

    {

      return;

    }



    if (vict->master)

      stop_follower(vict);



    add_follower(vict, ch);



    af.type      = SPELL_CHARM;

    af.duration  = -1;

    af.modifier  = 0;

    af.location  = APPLY_NONE;

    af.bitvector = AFF_TAMED;



 affect_join(vict, &af, FALSE, FALSE, FALSE, FALSE);



    act("You tame $N.", FALSE, ch, 0, vict, TO_CHAR);

    act("$n tames you.", FALSE, ch, 0, vict, TO_VICT);

    act("$n tames $N.", FALSE, ch, 0, vict, TO_NOTVICT);





    if (IS_NPC(vict)) {

      REMOVE_BIT(MOB_FLAGS(vict), MOB_AGGRESSIVE);

      REMOVE_BIT(MOB_FLAGS(vict), MOB_SPEC);

}

}



}



