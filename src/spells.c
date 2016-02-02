/* ************************************************************************
*   File: spells.c                                      Part of CircleMUD *
*  Usage: Implementation of "manual spells".  Circle 2.2 spell compat.    *
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
#include "spells.h"
#include "handler.h"
#include "db.h"
#include "interpreter.h"

extern struct room_data *world;
extern struct obj_data *object_list;
extern struct char_data *character_list;
//extern struct cha_app_type cha_app[];
extern struct cha_app_type *cha_app;
extern struct int_app_type int_app[];
extern struct index_data *obj_index;
extern struct index_data *mob_index;

extern struct weather_data weather_info;
extern struct descriptor_data *descriptor_list;
extern struct zone_data *zone_table;

extern int mini_mud;
extern int pk_allowed;

extern struct default_mobile_stats *mob_defaults;
extern char weapon_verbs[];
extern int *max_ac_applys;
extern struct apply_mod_defaults *apmd;

extern char *spells[];


int allowNewFollower(struct char_data* ch, int maxFollowerAllowed);
void clearMemory(struct char_data * ch);
void die(struct char_data * ch, struct char_data * killer);
void death_cry(struct char_data * ch);

void damage(struct char_data * ch, struct char_data * victim,
	    int damage, int weapontype);
void dismount_char(struct char_data * ch);

int is_turned(struct char_data * ch, struct char_data * vict, int spellnum);
int is_majored(struct char_data * ch, struct char_data * victim, int spellnum);
int level_exp(int class, int level);
void weight_change_object(struct obj_data * obj, int weight);
void add_follower(struct char_data * ch, struct char_data * leader);
int mag_savingthrow(struct char_data * ch, int type);
void affect_remove_nonperm(struct char_data * ch, int type);
int eff_spell_level(struct char_data * ch, int spellnum);
int spell_access(struct char_data * ch, int spellnum);

struct time_info_data *age(struct char_data * ch);


/*
 * Special spells appear below.
 */

ASPELL(spell_create_water)
{
  int water;

  void name_to_drinkcon(struct obj_data * obj, int type);
  /* void name_from_drinkcon(struct obj_data * obj); */

  if (ch == NULL || obj == NULL)
    return;
  level = MAX(MIN(level, LVL_IMPL), 1);

  if (GET_OBJ_TYPE(obj) == ITEM_DRINKCON) {
    if ((GET_OBJ_VAL(obj, 2) != LIQ_WATER) && (GET_OBJ_VAL(obj, 1) != 0)) {
      /*  PDH  4/27/99 - why remove the name ?  */
      /* name_from_drinkcon(obj); */
      GET_OBJ_VAL(obj, 2) = LIQ_SLIME;
      name_to_drinkcon(obj, LIQ_SLIME);
    } else {
      water = MAX(GET_OBJ_VAL(obj, 0) - GET_OBJ_VAL(obj, 1), 0);
      if (water > 0) {
	if (GET_OBJ_VAL(obj, 1) >= 0) {
	  /*  PDH  4/27/99 - why remove the name ?  */
	  /* name_from_drinkcon(obj); */
	}
	GET_OBJ_VAL(obj, 2) = LIQ_WATER;
	GET_OBJ_VAL(obj, 1) += water;
	name_to_drinkcon(obj, LIQ_WATER);
	weight_change_object(obj, water);
	act("$p is filled.", FALSE, ch, obj, 0, TO_CHAR);
      }
    }
  }
}


ASPELL(spell_recall)
{
  extern sh_int r_mortal_start_room;
  sh_int  recallRoom;


  if (victim == NULL || IS_NPC(victim)) {
    return;
  }

  if ( PLR_FLAGGED(victim, PLR_KILLER) ) {
    send_to_char("The Gods despise murderers, and stop your pathetic attempt to recall.\r\n", victim);
    return;
  }

   if (GET_POS(victim) == POS_RIDING) {
     send_to_char("Perhaps you should dismount first?\r\n", victim);
    return ;
}

  if ( (recallRoom = real_room(GET_LOADROOM(victim))) == NOWHERE ) {
    recallRoom = r_mortal_start_room;
  }

  act("$n disappears.", TRUE, victim, 0, 0, TO_ROOM);
  char_from_room(victim);
  char_to_room(victim, recallRoom);
  act("$n appears in the middle of the room.", TRUE, victim, 0, 0, TO_ROOM);
  look_at_room(victim, 0);
}


#if 0
ASPELL(spell_teleport)
{
  int to_room;

  if (victim != NULL)
    return;

  do {
    to_room = number(0, top_of_world);
  } while (ROOM_FLAGGED(to_room, ROOM_PRIVATE | ROOM_DEATH));

  act("$n slowly fades out of existence and is gone.",
      FALSE, victim, 0, 0, TO_ROOM);
  char_from_room(victim);
  char_to_room(victim, to_room);
  act("$n slowly fades into existence.", FALSE, victim, 0, 0, TO_ROOM);
  look_at_room(victim, 0);
}
#endif


#define SUMMON_FAIL "You failed.\r\n"

ASPELL(spell_summon)
{
  if (ch == NULL || victim == NULL)
    return;

  if (GET_LEVEL(victim) > MIN(LVL_IMMORT - 1, level + 3)) {
    send_to_char(SUMMON_FAIL, ch);
    return;
  }

  if (!IS_NPC(victim) && !(known_player(ch, victim))) {
     send_to_char("You don't know anyone by that name!\r\n", ch);
    return;
  }


  if (!pk_allowed) {
    if (MOB_FLAGGED(victim, MOB_AGGRESSIVE)) {
      act("As the words escape your lips and $N travels\r\n"
	  "through time and space towards you, you realize that $E is\r\n"
	  "aggressive and might harm you, so you wisely send $M back.",
	  FALSE, ch, 0, victim, TO_CHAR);
      return;
    }

/*  Making PRF_SUMMONABLE obsolete - Soli, 6/7/99
    if ( ! IS_NPC(victim) &&
	 ! PRF_FLAGGED(victim, PRF_SUMMONABLE) &&
	 ! PLR_FLAGGED(victim, PLR_KILLER)) {
      GET_NAME_II(ch, victim, chname);
      sprintf(buf, "%s just tried to summon you to: %s.\r\n"
	      "%s failed because you have summon protection on.\r\n"
	      "Type NOSUMMON to allow other players to summon you.\r\n",
	      chname, world[ch->in_room].name,
	      (ch->player.sex == SEX_MALE) ? "He" : "She");
      FREE_NAME(chname);
      send_to_char(buf, victim);

      GET_NAME_II(victim, ch, chname);
      sprintf(buf, "You failed because %s has summon protection on.\r\n",
        chname);
      FREE_NAME(chname);
      send_to_char(buf, ch);

      GET_NAME(ch, chname);
      GET_NAME(victim, victname);
      sprintf(buf, "%s failed summoning %s to %s.",
	      chname, victname, world[ch->in_room].name);
      FREE_NAME(victname);
      FREE_NAME(chname);
      mudlog(buf, BRF, LVL_IMMORT, TRUE);
      return;
    }

*/
  }

  if (MOB_FLAGGED(victim, MOB_NOSUMMON) ||
      (IS_NPC(victim) && mag_savingthrow(victim, SAVING_SPELL)) ||
       (ROOM_FLAGGED(victim->in_room, ROOM_NOMAGIC))) {
    send_to_char(SUMMON_FAIL, ch);
    return;
  }

 else if (RIDING(victim)) {
  dismount_char(victim);
  act("$n disappears suddenly, leaving $s mount behind!", TRUE, victim, 0, 0, TO_ROOM);
  char_from_room(victim);
  char_to_room(victim, ch->in_room);

  act("$n arrives suddenly.", TRUE, victim, 0, 0, TO_ROOM);
  act("$n has summoned you!", FALSE, ch, 0, victim, TO_VICT);
  look_at_room(victim, 0);
 }
  else {
  act("$n disappears suddenly.", TRUE, victim, 0, 0, TO_ROOM);
  char_from_room(victim);
  char_to_room(victim, ch->in_room);
  act("$n arrives suddenly.", TRUE, victim, 0, 0, TO_ROOM);
  act("$n has summoned you!", FALSE, ch, 0, victim, TO_VICT);
  look_at_room(victim, 0);
 }
}


ASPELL(spell_locate_object)
{
  room_rnum rm = 0, old_rm;
  struct obj_data *temp;

  temp = obj;

  old_rm = ch->in_room;
  while (temp->in_obj)
    temp = temp->in_obj;

  if (temp->carried_by)
    rm = temp->carried_by->in_room;
  else if (temp->worn_by)
    rm = temp->worn_by->in_room;
  else if (temp->in_room != NOWHERE)
    rm = temp->in_room;

  if (rm && (rm != NOWHERE))
  {
    send_to_char("A vision fills your eyes...\r\n", ch);
    char_from_room(ch);
    char_to_room(ch, rm);
    look_at_room(ch, 0);
    char_from_room(ch);
    char_to_room(ch, old_rm);
  } else
    send_to_char("An image begins to form, but suddenly dissipates.\r\n", ch);

  return;
}


ASPELL(spell_reveal_illusion)
{

if (!victim)
    return;

if (!PLR_FLAGGED(victim, PLR_BIRD) && !PLR_FLAGGED(victim, PLR_BEAR) && !PLR_FLAGGED(victim, PLR_BIRD) && !PLR_FLAGGED(victim, PLR_RABBIT) && !PLR_FLAGGED(victim, PLR_WOLF)){
    send_to_char("They are not an illusion!\r\n", ch);
    return;
  }

if (GET_LEVEL(victim) > MIN(LVL_IMMORT - 1, level + 3)) {
    send_to_char("You cannot see through the illusion!\r\n", ch);
    return;
  }

else {
sprintf(buf, victim->char_specials.name_dis);
GET_NAME(victim, chname);
sprintf(buf2, "Your eyes tell you that %s is actually an illusion of %s!", buf, chname);
act(buf2, FALSE, ch, 0, 0, TO_CHAR);
FREE_NAME(chname);
}

}


ASPELL(spell_scry)
{
  room_rnum rm = NOWHERE, old_rm = NOWHERE;

  if (!victim)
    return;

 if (GET_LEVEL(victim) > MIN(LVL_IMMORT - 1, level + 3)) {
    send_to_char("You cannot quite get a picture of them.\r\n", ch);
    return;
  }

  rm = victim->in_room;
  old_rm = ch->in_room;

  char_from_room(ch);
  char_to_room(ch, rm);
  look_at_room(ch, 0);
  char_from_room(ch);
  char_to_room(ch, old_rm);

}


ASPELL(spell_abjure)
{
  int abjure = 1;

  if (!victim)
    return;

  if (!IS_NPC(victim) || !victim->master)
    abjure = 0;
  else if (!GET_MOB_VNUM(victim) > 50)
    abjure = 0;
  else if (mag_savingthrow(victim, SAVING_SPELL))
    abjure = 0;

  if (abjure)
  {
    act("You banish $N back to whence it came.", 0, ch, 0, victim, TO_CHAR);
    act("$n banishes $n back where it came from.", 1, ch, 0, victim, TO_NOTVICT);
    extract_char(victim);
    return;
  }
  else
  {
    act("You fail to banish $N.", 0, ch, 0, victim, TO_CHAR);
    act("$n has failed to banish $N.", 0, ch, 0, victim, TO_NOTVICT);
    if (AWAKE(victim) && (ch != victim))
      hit(victim, ch, TYPE_UNDEFINED);
  }
}


ASPELL(spell_charm)
{
  struct affected_type af;

  if (victim == NULL || ch == NULL)
    return;

  if (victim == ch)
    send_to_char("You like yourself even better!\r\n", ch);
	else if (!AWAKE(victim)) {
	  send_to_char("You cannot charm a sleeping victim.\r\n", ch);
		return;
	}
  else if (MOB_FLAGGED(victim, MOB_NOCHARM)) {
    send_to_char("You fail.\r\n", ch);
    if ( IS_NPC(victim) ) {
      hit(victim, ch, TYPE_UNDEFINED);
    }
  } else if (IS_AFFECTED(ch, AFF_CHARM))
    send_to_char("You can't have any followers of your own!\r\n", ch);
  else if (IS_AFFECTED(victim, AFF_CHARM) || level < GET_LEVEL(victim))
    send_to_char("You fail.\r\n", ch);
  else if (circle_follow(victim, ch))
    send_to_char("Sorry, following in circles can not be allowed.\r\n", ch);

  else if (affected_by_spell(victim, SPELL_IMPERVIOUS_MIND) ||
           (dice(1, 100) > GET_CHA(ch) && mag_savingthrow(victim, SAVING_PARA)))
  {
    send_to_char("You fail.\r\n", ch);
    if ( IS_NPC(victim) ) {
      hit(victim, ch, TYPE_UNDEFINED);
    }

  } else {

    if (!allowNewFollower(ch, 3))
    {
      return;
    }

    if (victim->master)
      stop_follower(victim);

    add_follower(victim, ch);

    af.type      = SPELL_CHARM;
    af.duration  = GET_CHA(ch) * 2; 
    af.modifier  = 0;
    af.location  = 0;
    af.bitvector = AFF_CHARM;

    affect_to_char(victim, &af);

    act("Isn't $n just such a nice fellow?", FALSE, ch, 0, victim, TO_VICT);
    if (IS_NPC(victim)) {
      REMOVE_BIT(MOB_FLAGS(victim), MOB_AGGRESSIVE);
      REMOVE_BIT(MOB_FLAGS(victim), MOB_SPEC);
    }
  }
}



ASPELL(spell_legend_lore)
{
  int i, found, avg;


  extern const char *item_types[];
  extern const char *apply_types[];
  extern const char *affected_bits[];

//  if(victim != NULL)
//    return;

  if (obj) {
    send_to_char("You feel informed:\r\n", ch);

    sprintf(buf, "Object: %s\r\nItem type: ", obj->short_description);
    sprinttype(GET_OBJ_TYPE(obj), item_types, buf2);
    if (IS_OBJ_STAT(obj, ITEM_MAGIC))
      strcat(buf2, " <magic>");
    strcat(buf, buf2);
    strcat(buf, "\r\n");
    send_to_char(buf, ch);

    if (obj->obj_flags.bitvector) {
      send_to_char("Affects: ", ch);
      sprintbit(obj->obj_flags.bitvector, affected_bits, buf);
      strcat(buf, "\r\n");
      send_to_char(buf, ch);
    }

/*
    send_to_char("Item is: ", ch);
    sprintbit(GET_OBJ_EXTRA(obj), extra_bits, buf);
    strcat(buf, "\r\n");
    send_to_char(buf, ch);
*/

    sprintf(buf, "Weight: %d pounds.   Object value: %d.\r\n",
            GET_OBJ_WEIGHT(obj), GET_OBJ_COST(obj));
    send_to_char(buf, ch);

    switch (GET_OBJ_TYPE(obj)) {
    case ITEM_SCROLL:
    case ITEM_POTION:
    case ITEM_SALVE:
      sprintf(buf, "Level %d spells of: ", GET_OBJ_VAL(obj, 0));

      if (GET_OBJ_VAL(obj, 1) >= 1)
        sprintf(buf, "%s %s", buf, spells[GET_OBJ_VAL(obj, 1)]);
      if (GET_OBJ_VAL(obj, 2) >= 1)
        sprintf(buf, "%s, %s", buf, spells[GET_OBJ_VAL(obj, 2)]);
      if (GET_OBJ_VAL(obj, 3) >= 1)
        sprintf(buf, "%s, %s", buf, spells[GET_OBJ_VAL(obj, 3)]);
      sprintf(buf, "%s\r\n", buf);
      send_to_char(buf, ch);
      break;
    case ITEM_WAND:
    case ITEM_STAFF:
      sprintf(buf, "Level %d spells (%d of %d charges) of: ",
              GET_OBJ_VAL(obj, 0), GET_OBJ_VAL(obj, 1), GET_OBJ_VAL(obj, 2));
      sprintf(buf, "%s %s\r\n", buf, spells[GET_OBJ_VAL(obj, 3)]);
      send_to_char(buf, ch);
      break;
    case ITEM_WEAPON:
      avg = ((GET_OBJ_VAL(obj, 2) * GET_OBJ_VAL(obj, 1)) + GET_OBJ_VAL(obj, 1));
      avg /= 2;

      if (avg % 3)
      {
        if ((avg % 3) > 1)
          avg++;
        else
          avg--;
      }

      sprintf(buf, "Approximate average damage:  %d", avg);
      send_to_char(buf, ch);
      break;
    case ITEM_ARMOR:
      avg = (GET_OBJ_VAL(obj, 0));

      if (avg % 3)
      {
        if ((avg % 3) > 1)
          avg++;
        else
          avg--;
      }

      sprintf(buf, "Approximate AC:  %d", avg);
      send_to_char(buf, ch);
      break;
    }

    found = FALSE;

    for (i = 0; i < MAX_OBJ_AFFECT; i++) {
      if ((obj->affected[i].location != APPLY_NONE) &&
	  (obj->affected[i].modifier != 0)) {
	if (!found) {
          send_to_char("Additional affects:\r\n", ch);
	  found = TRUE;
	}
	sprinttype(obj->affected[i].location, apply_types, buf2);
        if (obj->affected[i].modifier > 0)
          sprintf(buf, " - Increases %s\r\n", buf2);
        else
          sprintf(buf, " - Decreases %s\r\n", buf2);
	send_to_char(buf, ch);
      }
    }
  }
}



ASPELL(spell_enchant_weapon)
{
  int i;

  if (ch == NULL || obj == NULL)
    return;

  if ((GET_OBJ_TYPE(obj) == ITEM_WEAPON) &&
      !IS_SET(GET_OBJ_EXTRA(obj), ITEM_MAGIC)) {

    for (i = 0; i < MAX_OBJ_AFFECT; i++)
      if (obj->affected[i].location != APPLY_NONE)
	return;

    SET_BIT(GET_OBJ_EXTRA(obj), ITEM_MAGIC);

    obj->affected[0].location = APPLY_HITROLL;
    obj->affected[0].modifier = 1 + (level >= 18);

    obj->affected[1].location = APPLY_DAMROLL;
    obj->affected[1].modifier = 1 + (level >= 20);

    if (IS_GOOD(ch)) {
      SET_BIT(GET_OBJ_EXTRA(obj), ITEM_ANTI_EVIL);
      act("$p glows blue.", FALSE, ch, obj, 0, TO_CHAR);
    } else if (IS_EVIL(ch)) {
      SET_BIT(GET_OBJ_EXTRA(obj), ITEM_ANTI_GOOD);
      act("$p glows red.", FALSE, ch, obj, 0, TO_CHAR);
    } else {
      act("$p glows yellow.", FALSE, ch, obj, 0, TO_CHAR);
    }
  }
}


ASPELL(spell_detect_poison)
{
  if (victim) {
    if (victim == ch) {
      if (IS_AFFECTED(victim, AFF_POISON))
        send_to_char("You can sense poison in your blood.\r\n", ch);
      else
        send_to_char("You feel healthy.\r\n", ch);
    } else {
      if (IS_AFFECTED(victim, AFF_POISON))
        act("You sense that $E is poisoned.", FALSE, ch, 0, victim, TO_CHAR);
      else
        act("You sense that $E is healthy.", FALSE, ch, 0, victim, TO_CHAR);
    }
  }

  if (obj) {
    switch (GET_OBJ_TYPE(obj)) {
    case ITEM_DRINKCON:
    case ITEM_FOUNTAIN:
    case ITEM_FOOD:
      if (GET_OBJ_VAL(obj, 3))
	act("You sense that $p has been contaminated.",FALSE,ch,obj,0,TO_CHAR);
      else
	act("You sense that $p is safe for consumption.", FALSE, ch, obj, 0,
	    TO_CHAR);
      break;
    default:
      send_to_char("You sense that it should not be consumed.\r\n", ch);
    }
  }
}


ASPELL(spell_teleport)
{
  struct room_data *newroom = 0;
  struct room_data *casting_room, *target_room;
  struct obj_data *foolz_objs;
  int randroom = 0, prob = 0, NR = 0, i;

  if (ch == NULL || victim == NULL)
    return;

  casting_room = &world[ch->in_room];
  target_room = &world[victim->in_room];
    	      
  if (GET_LEVEL(victim) > LVL_IMMORT && GET_LEVEL(ch) < LVL_IMMORT) {
    send_to_char("Your mortal magic is foiled.\r\n", ch);
    return;
  }
   
  if (IS_SET(casting_room->room_flags, ROOM_NOMAGIC)) {
    send_to_char("Your magic fizzles out and dies.\r\n", ch);
    return;
  }
  if (IS_NPC(victim)) {
    send_to_char("You cannot get an image of the location..\r\n", ch);
    return;
  }
    	       	  	    
  newroom = &world[victim->in_room];
  if (IS_SET(newroom->room_flags, ROOM_NOMAGIC)) {
    send_to_char("Your destination's anti-magic barrier prevents the spell.\r\n", ch);
    return;
  }
  if (IS_SET(newroom->room_flags, ROOM_TUNNEL)) {
    send_to_char("The size of the destination prevents your teleportation.\r\n", ch);
    return;
  }

  prob = number(1,100);


  if (GET_LEVEL(ch) >= LVL_IMMORT) 
    prob = 50;
  	  
  if ((known_player(ch, victim) && prob <= 90) ||
      (prob >= 10 && prob <= 80))
  {
    send_to_char("Your magic takes affect as your begin to shift locations...\r\n", ch);
    act("$n slowly disappears, teleporting away.", TRUE, ch, 0, 0, TO_NOTVICT);
    NR = victim->in_room;
    char_from_room(ch);
    char_to_room(ch, NR);
    act("$n teleports into the room.", TRUE, ch, 0, 0, TO_ROOM);
    send_to_char("You have teleported on target.\r\n", ch);
    look_at_room(ch, 0);
    return;
  }

  if (prob < 10)
  {
    send_to_char("Your spell goes awry as you teleport into solid matter!\r\n", ch);
    act("$n miscasts a teleport spell, and disappears.", TRUE, ch, 0, 0, TO_NOTVICT);
    for (i = 0; i < NUM_WEARS; i++) 
      if (GET_EQ(ch, i))      	
	unequip_char(ch, i);
    while (ch->carrying) {
      foolz_objs = ch->carrying;
      extract_obj(foolz_objs);
    }
    extract_char(ch);
    GET_NAME(ch, chname);
    GET_NAME(victim, victname);
    sprintf(buf, "(GC) %s has misteleported, attempting to teleport to %s, and died.", chname, victname);
    FREE_NAME(victname);
    FREE_NAME(chname);
    mudlog(buf, BRF, LVL_IMMORT, TRUE);
    log(buf);
    return;
  }

  if (prob > 80)
  {
    send_to_char("Your spell goes awry as you teleport into an unknown place!\r\n", ch);
    act("$n miscasts a teleport spell, and disappears.", TRUE, 0, 0, 0, TO_NOTVICT);
    do {
      randroom = number(0, top_of_world);
    }
    while (ROOM_FLAGGED(randroom, ROOM_PRIVATE | ROOM_DEATH | ROOM_GODROOM) ||
          (zone_table[world[randroom].zone].zone_status != ZONE_OPEN));
    char_from_room(ch);
    char_to_room(ch, randroom);
    act("$n teleports into the room.", TRUE, ch, 0, 0, TO_ROOM);
    send_to_char("You luckily have not died during teleportation.\r\n", ch);
    look_at_room(victim, 0);
    return;
  }
}


#define GATE_IMMORT "You may not create a gateway to immortals, silly!"
#define GATE_MOB "You may not create a gateway to mobs, silly!"
#define GATE_MOB_FAR "That creature is too far away for a portal to be made."
#define GATE_FAIL "You fail!"
#define ROOM_FAIL "Creating a gateway to the desired room is not allowed!"

ASPELL(spell_gate)
{
  struct obj_data *portal, *tportal;
  struct extra_descr_data *new_descr, *new_tdescr;

  if (ch == NULL || victim == NULL)
    return;
  
  if (IS_NPC(victim))
  {
    if (world[ch->in_room].zone != world[victim->in_room].zone)
    {
      act(GATE_MOB_FAR, FALSE, ch, 0, 0, TO_CHAR);
      return;
    }
  }

#if 0          
  if (IS_NPC(victim)) {
    act(GATE_MOB, FALSE, ch, 0, 0, TO_CHAR);
    return;
  }
#endif

  if (!IS_NPC(victim))
  {
    if (!known_player(ch, victim))
    {
      send_to_char("You don't know anyone by that name.\r\n", ch);
      return;
    }
  }

  if ((GET_LEVEL(victim) >= LVL_IMMORT) &&
      (GET_LEVEL(ch) < GET_LEVEL(victim))) {
    act(GATE_IMMORT, FALSE, ch, 0, 0, TO_CHAR);
    return;
  }

  if (ROOM_FLAGGED(victim->in_room, ROOM_OLC) ||
      ROOM_FLAGGED(victim->in_room, ROOM_GODROOM) ||
      ROOM_FLAGGED(victim->in_room, ROOM_NOMAGIC) || 
      ROOM_FLAGGED(victim->in_room, ROOM_PRIVATE) ||
      (world[victim->in_room].number < 100) ||
      ((world[victim->in_room].number >= 1200) && 
       (world[victim->in_room].number < 1300))) {
    act(ROOM_FAIL, FALSE, ch, 0, 0, TO_CHAR);
    return;
  }                                    

  /* create the portal */
  portal = read_object(PORTAL_OBJ, VIRTUAL);
  GET_OBJ_VAL(portal, 0) = victim->in_room;
  GET_OBJ_TIMER(portal) = 2; /* (int) (GET_LEVEL(ch) / 10);*/
  CREATE(new_descr, struct extra_descr_data, 1);

  new_descr->keyword = str_dup("portal gate gateway");
  sprintf(buf, "You can barely make out %s.\r\n", world[victim->in_room].name);
  new_descr->description = str_dup(buf);
  new_descr->next = portal->ex_description;
  portal->ex_description = new_descr;
  obj_to_room(portal, ch->in_room);

  act("$n conjures a portal out of thin air.",
      TRUE, ch, 0, 0, TO_ROOM);
  act("You conjure a portal out of thin air.",
      TRUE, ch, 0, 0, TO_CHAR);

  /* create the portal at the other end */
  tportal = read_object(PORTAL_OBJ, VIRTUAL);
  GET_OBJ_VAL(tportal, 0) = ch->in_room;
  GET_OBJ_TIMER(tportal) = 2; /* (int) (GET_LEVEL(ch) / 10);*/
  CREATE(new_tdescr, struct extra_descr_data, 1);

  new_tdescr->keyword = str_dup("portal gate gateway");
  sprintf(buf, "You can barely make out %s.\r\n", world[ch->in_room].name);
  new_tdescr->description = str_dup(buf);
  new_tdescr->next = tportal->ex_description;
  tportal->ex_description = new_tdescr;
  obj_to_room(tportal, victim->in_room);

  act("A glowing portal appears out of thin air.",
      TRUE, victim, 0, 0, TO_ROOM);
  act("A glowing portal opens here for you.",
      TRUE, victim, 0, 0, TO_CHAR);
}


ASPELL(spell_create_spring)
{
  struct obj_data *spring = '\0';
  *buf = '\0';

  if (GET_SKILL(ch, SPELL_CREATE_SPRING) == 0)
    {
      send_to_char("That spell is unfamiliar to you.\r\n", ch);
      return ;
    }

  if((SECT(ch->in_room) == SECT_INSIDE) || (SECT(ch->in_room) == SECT_CITY))
    {
      send_to_char("You cannot create a spring here!\r\n", ch);
      return ;
    }

  if((SECT(ch->in_room) == SECT_WATER_SWIM) ||
     (SECT(ch->in_room) == SECT_WATER_NOSWIM))
    {
      send_to_char("How can you create a spring in water?\r\n", ch);
      return;
    }

  if(SECT(ch->in_room) == SECT_UNDERWATER)
    {
      send_to_char("You cannot create a spring underwater!\r\n", ch);
      return;
    }
 
  if (ROOM_FLAGGED(ch->in_room, ROOM_NOMAGIC)) {
    send_to_char("An unforseen force prevents you from casting spells.\r\n",
		 ch);
    return;
  }

  spring = read_object(VNUM_SPRING, VIRTUAL);
  GET_OBJ_TIMER(spring) = 12;
  obj_to_room(spring, ch->in_room);

  sprintf(buf, "You have created a bubbling spring of water.\r\n");
  send_to_char(buf, ch);
  act("$n has created a bubbling spring of water.", TRUE, ch, 0, 0, TO_ROOM);

  return;
}


ASPELL(spell_fear)
{
  struct char_data *target = (struct char_data *) victim;
  struct char_data *next_target;
  int rooms_to_flee = 0;

  ACMD(do_flee);

  if (ch == NULL)
    return;

  send_to_char("You radiate an aura of fear into the room!\r\n", ch);
  act("$n is surrounded by an aura of fear!", TRUE, ch, 0, 0, TO_ROOM);

  for (target = world[ch->in_room].people; target; target = next_target) {
    next_target = target->next_in_room;

    if (target == NULL) 
      return;
		
    if (target == ch)
      continue;
				
    if (GET_LEVEL(target) >= LVL_IMMORT)
      continue;
						
    if (mag_savingthrow(target, 1)) {
     CHOOSE_NAME(ch, chname);
      sprintf(buf, "%s is unaffected by the fear!\r\n", chname);
      FREE_NAME(chname);
      act(buf, TRUE, ch, 0, 0, TO_ROOM);
      send_to_char("Your victim is not afraid of the likes of you!\r\n", ch);
      if (IS_NPC(target))
	hit(target, ch, TYPE_UNDEFINED);
    }
    else {
      for(rooms_to_flee = level / 10; rooms_to_flee > 0; rooms_to_flee--) {
	send_to_char("You flee in terror!\r\n", target);
	do_flee(target, "", 0, 0);
      }
    }
  }
}


ASPELL(spell_calm)
{
  struct char_data *vict;
  act("$n calls upon the gods to make peace.", TRUE, ch, 0, 0, TO_ROOM);
  send_to_room("Everything is quite calm now.\r\n", ch->in_room);
  for (vict = world[ch->in_room].people; vict; vict = vict->next_in_room)
  {
    if (FIGHTING(vict))
      stop_fighting(vict);
  }
}					


ASPELL(spell_dispel_magic)
{
  int remove_spells[] =
  {
    SPELL_ARMOR, SPELL_BLESS, SPELL_CHARM, SPELL_CHILL_TOUCH,
    SPELL_DETECT_ALIGN, SPELL_DETECT_INVIS, SPELL_DETECT_MAGIC,
    SPELL_DETECT_POISON, SPELL_INVISIBILITY, SPELL_PROT_FROM_EVIL,
    SPELL_SANCTUARY, SPELL_SHOCKING_GRASP, SPELL_SLEEP, SPELL_STRENGTH,
    SPELL_SENSE_LIFE, SPELL_MINOR_SANCTUARY, SPELL_PRAYER, SPELL_INFRAVISION,
    SPELL_WATER_WALK, SPELL_BARKSKIN, SPELL_STONESKIN, SPELL_FLIGHT,
    SPELL_WEAKNESS, SPELL_HASTE, SPELL_SLOW, SPELL_PROT_FROM_GOOD,
    SPELL_PROT_FROM_UNDEAD, SPELL_PROT_FROM_FIRE, SPELL_PROT_FROM_FROST,
    SPELL_PROT_FROM_ELEMENTS, SPELL_THORNFLESH, SPELL_CLOAK_OF_FEAR,
    SPELL_BRAVERY, SPELL_FAERIE_FIRE, SPELL_THORNFLESH, SPELL_CRIPPLE,
    SPELL_LIGHT, SPELL_DARKNESS, SPELL_SILENCE, SPELL_SPHERE_SILENCE,
    SPELL_MAGIC_VEST, SPELL_FREE_ACTION, SPELL_SPIRIT_HAMMER, SPELL_AID,
    SPELL_THORNSHIELD, SPELL_FLAME_BLADE, SPELL_WATER_BREATHING,
    SPELL_WIND_WALK, SPELL_IMPERVIOUS_MIND, SPELL_DIVINE_WRATH,
    SPELL_ADAMANT_MACE, SPELL_BLUR, SPELL_SHIELD, SPELL_MIRROR_IMAGE,
    SPELL_PHANTASMAL_SNAKE, SPELL_HOLD_PERSON, SPELL_WEB, SPELL_MINOR_GLOBE,
    SPELL_MAJOR_GLOBE, SPELL_DANCING_SWORD, SPELL_BLINK, SPELL_ANTIMAGIC_AURA,
    SPELL_SPELL_TURNING, SPELL_VAMPIRIC_AURA, SPELL_CONCEAL_ALIGN,
    SPELL_ENHANCE_AGILITY, SPELL_ENHANCE_ENDURANCE, SPELL_ENHANCE_INSIGHT,
    SPELL_ENHANCE_CUNNING, SPELL_ENHANCE_CHARISMA,
    -1
  };

  int i;

  if ((ch == NULL) || ((victim == NULL) && (obj == NULL)))
    return;

  if (obj)
  {
  if (IS_OBJ_STAT(obj, ITEM_INVISIBLE)) {
    REMOVE_BIT(obj->obj_flags.extra_flags, ITEM_INVISIBLE);
    act("$p slowly fades into existence.", 0, ch, obj, 0, TO_CHAR);
    }
  else
    send_to_char("Nothing seems to happen.\r\n", ch);
    } 


  if (victim)
  {
    for (i = 0; remove_spells[i] != -1; i++)
    {
      if (affected_by_spell(victim, remove_spells[i]))
        affect_remove_nonperm(victim, remove_spells[i]);
    }

    if (affected_by_spell(victim, SKILL_DIRTKICK))
    affect_from_char(victim, SKILL_DIRTKICK);
  }

  return;
}


ASPELL(spell_holy_word)
{
  struct affected_type af[2];
  struct char_data *tmp, *i;

  act("You yell a holy word of great power.", FALSE, ch, 0, 0, TO_CHAR);
  act("$n yells out a holy word of great power.", FALSE, ch, 0, 0, TO_ROOM);

  i = world[ch->in_room].people;

  while (i)
  {
    tmp = i;
    i = i->next_in_room;

    if ((IS_GOOD(ch) && IS_GOOD(tmp)) || (IS_EVIL(ch) && IS_EVIL(tmp)) ||
        (IS_NEUTRAL(ch) && IS_NEUTRAL(tmp)))
      continue;

    if (affected_by_spell(tmp, SPELL_DEAFENING_WIND))
      continue;

    if (GET_LEVEL(tmp) < 6)
    {
      send_to_char("The holy word snuffs out your life force.", tmp);
      act("$n's life force has been snuffed!", TRUE, tmp, 0, 0, TO_ROOM);
      die(tmp, ch);
      continue;
    } else if (GET_LEVEL(tmp) < 11)
    {
      af[0].type = SPELL_PARALYZE;
      af[0].duration = 5;
      af[0].modifier = 0;
      af[0].bitvector = 0;
      af[0].location = APPLY_NONE;

      affect_join(tmp, &af[0], FALSE, FALSE, FALSE, FALSE);
      act("You have been stunned by the holy word.", FALSE, tmp, 0, 0, TO_CHAR);
      act("$n seems to be stunned.", TRUE, tmp, 0, 0, TO_ROOM);
      continue;
    } else if (GET_LEVEL(tmp) < 21)
    {

      af[0].type = SPELL_SLOW;
      af[0].location = APPLY_HITROLL;
      af[0].modifier = -4;
      af[0].duration = 5;
      af[0].bitvector = 0;

      af[1].type = SPELL_SLOW;
      af[1].location = APPLY_DAMROLL;
      af[1].modifier = -4;
      af[1].duration = 5;
      af[1].bitvector = 0;

      affect_join(tmp, &af[0], FALSE, FALSE, FALSE, FALSE);
      affect_join(tmp, &af[1], FALSE, FALSE, FALSE, FALSE);

      act("You start moving a bit slower now.", FALSE, tmp, 0, 0, TO_CHAR);
      act("$n starts to move slower.", TRUE, tmp, 0, 0, TO_ROOM);
      continue;
    } else if ((GET_LEVEL(tmp) < 31) || IS_NPC(tmp))
    {

      af[0].type = SPELL_DEAFENING_WIND;
      af[0].location = APPLY_HITROLL;
      af[0].modifier = -2;
      af[0].duration = 2;
      af[0].bitvector = 0;

      af[1].type = SPELL_DEAFENING_WIND;
      af[1].location = APPLY_DAMROLL;
      af[1].modifier = -2;
      af[1].duration = 2;
      af[1].bitvector = 0;

      affect_join(tmp, &af[0], FALSE, FALSE, FALSE, FALSE);
      affect_join(tmp, &af[1], FALSE, FALSE, FALSE, FALSE);

      act("You have lost your sense of hearing.", 0, tmp, 0, 0, TO_CHAR);
      act("$n seems to have been deafened.", 1, tmp, 0, 0, TO_ROOM);
      continue;
    } else
    {
      act("You are unaffected.", 0, tmp, 0, 0, TO_CHAR);
      act("$n seems to be unaffected.", 1, tmp, 0, 0, TO_ROOM);
    }
  }
}


ASPELL(spell_gust)
{
  int attempt, was_in, i = 0, gone = 0;

  if ((ch == NULL) || (victim == NULL))
    return;

  if (mag_savingthrow(victim, SAVING_SPELL))
    return;

  if (affected_by_spell(victim, SPELL_WEB) ||
      affected_by_spell(victim, SPELL_ENTANGLE))
    return;

  if (IS_SET(ROOM_FLAGS(ch->in_room), ROOM_ATRIUM))
    return;

  while ((i < 20) && !gone)
  {
    attempt = number(0, NUM_OF_DIRS - 1);
    if (!CAN_GO(ch, attempt))
      i++;
    else
      gone = 1;
  }

  if (gone == 0)
  {
    /* No directions to go. */
    return;
  }

  act("$n is blown away by a gust of wind.", TRUE, victim, 0, 0, TO_ROOM);
  send_to_char("You are blown away by a gust of wind.\r\n", victim);

  was_in = victim->in_room;
  char_from_room(victim);
  char_to_room(victim, world[was_in].dir_option[attempt]->to_room);

  look_at_room(victim, 0);

  if (IS_SET(ROOM_FLAGS(ch->in_room), ROOM_DEATH) && GET_LEVEL(ch) < LVL_IMMORT)
  {
    log_death_trap(ch);
    death_cry(ch);
    extract_char(ch);
    return;
  }
}


ASPELL(spell_teleport_object)
{
  int percent, target = 0;
  struct char_data *tch;

  if (!obj)
  {
    send_to_char("What item do you want to teleport?\r\n", ch);
  }

  if (!victim)
  {
    send_to_char("Who do you want to send it to?\r\n", ch);
  }

  percent = number(1, 101);

  if (percent < 3)
  {
    act("$p disappears in a bright flash of light.", 0, ch, obj, 0, TO_CHAR);
    obj_from_char(obj);
    extract_obj(obj);
    return;
  }

  if (!check_skill(ch, percent, SPELL_TELEPORT_OBJECT))
  {
    act("The spell is miscast, sending $p to a random location!",
         0, ch, obj, 0, TO_CHAR);
    while (!target)
    {
      for (tch = character_list; tch; tch = tch->next)
      {
        if (!number(0, 99))
        {
          obj_from_char(obj);
          obj_to_char(obj, tch);
          act("$p appears in your hands.", FALSE, tch, obj, 0, TO_CHAR);
          target = 1;
          break;
        }
      }
    }
  }
  else
  {
    act("$p disappears as it teleports.", 0, ch, obj, 0, TO_CHAR);
    act("$p suddenly appears in your hands.", 0, victim, obj, 0, TO_CHAR);
    obj_from_char(obj);
    obj_to_char(obj, victim);
  }

}


ASPELL(spell_energy_drain)
{
  int exp, dam, drained = 0;

  if (!ch && !victim)
    return;

  if (is_turned(ch, victim, SPELL_ENERGY_DRAIN))
    victim = ch;

  if (is_majored(ch, victim, SPELL_ENERGY_DRAIN))
    return;

  if (!mag_savingthrow(victim, SAVING_SPELL))
  {
    exp = ((level_exp(GET_CLASS(victim), GET_LEVEL(victim)) -
           (level_exp(GET_CLASS(victim), GET_LEVEL(victim) - 1)))
           * IS_NPC(victim));
    drained = 1;

    if (GET_LEVEL(ch) < GET_LEVEL(victim))
      exp >>= (GET_LEVEL(victim) - GET_LEVEL(ch));

    gain_exp(victim, -exp);

    dam = number(80, 110) + GET_LEVEL(ch);
    GET_MOVE(victim) >>= 2;
  }
  else
  {
    dam = number(40, 55) + ((2 * GET_LEVEL(ch)) / 3);
  }

  GET_HIT(ch) = MIN((GET_HIT(ch) + dam/2), GET_MAX_HIT(ch));
  if (drained == 1)
    send_to_char("Your life force is drained!\r\n", victim);

  damage(ch, victim, dam, SPELL_ENERGY_DRAIN);

  return;
}


struct char_data *get_chain_vict(struct char_data * ch)
{
  struct char_data *tmp;
  struct char_data *victs[20];
  int numvict = 0;

  for (tmp = world[ch->in_room].people; tmp; tmp = tmp->next_in_room)
  {
    if (numvict > 19)
      continue;
    if ((ch == tmp) || (((ch->master == tmp) || (tmp->master == ch) ||
                         (ch->master == tmp->master)) &&
         IS_AFFECTED(ch, AFF_GROUP) && IS_AFFECTED(tmp, AFF_GROUP)))
      continue;

    if (!IS_NPC(tmp) && GET_LEVEL(tmp) >= LVL_IMMORT)
      continue;

    victs[numvict] = tmp;

    if (is_turned(ch, tmp, SPELL_CHAIN_LIGHTNING))
      victs[numvict] = ch;
    numvict++;
  }

  if (numvict == 0)
    return NULL;

  return (victs[number(0, numvict - 1)]);
}


ASPELL(spell_chain)
{
/*
  int numvict = 0, damdice = 12, dam;
  struct char_data *tmp, *vict, *victs[11];
*/
  int damdice = 12, dam;
  struct char_data *vict;

/*
  for (tmp = world[ch->in_room].people; tmp; tmp = tmp->next_in_room)
  {
    if ((ch == tmp) || (((ch->master == tmp) || (tmp->master == ch)) &&
         IS_AFFECTED(ch, AFF_GROUP) && IS_AFFECTED(tmp, AFF_GROUP)))
      continue;

    if (!IS_NPC(tmp) && GET_LEVEL(tmp) >= LVL_IMMORT)
      continue;

    victs[numvict] = tmp;

    if (is_turned(ch, tmp, SPELL_CHAIN_LIGHTNING))
      victs[numvict] = ch;
    numvict++;
  }

  if (numvict == 0)
  {
    send_to_char("Your chain lightning fizzles and dies.\r\n", ch);
    return;
  }
*/

  while (ch && (damdice > 1))
  {
    damdice--;

    if ((vict = get_chain_vict(ch)) == NULL)
      return;

    dam = dice(damdice, 3);

    if (mag_savingthrow(vict, SAVING_SPELL))
      dam /= 2;

    if (is_majored(ch, vict, SPELL_CHAIN_LIGHTNING))
      continue;

    dam = MAX(damdice, dam);

    damage(ch, vict, dam, SPELL_CHAIN_LIGHTNING);
  }
}


ASPELL(spell_identify)
{
  int i, found, avg;

  const char *weap_mesg[] =
    { "minimal", "nominal", "serious", "critical", "massive"};
  const char *armor_mesg[] =
    { "very light", "light", "moderate", "heavy", "very heavy"};


  extern const char *item_types[];
  extern const char *apply_types[];

//  if(victim != NULL)
//    return;

  if (obj) {
    send_to_char("You feel informed:\r\n", ch);

    sprintf(buf, "Object: %s\r\nItem type: ", obj->short_description);
    sprinttype(GET_OBJ_TYPE(obj), item_types, buf2);
    if (IS_OBJ_STAT(obj, ITEM_MAGIC))
      strcat(buf2, " <magic>");
    strcat(buf, buf2);
    strcat(buf, "\r\n");
    send_to_char(buf, ch);

    sprintf(buf, "Weight: %d pounds.   Object value: %d.\r\n",
            GET_OBJ_WEIGHT(obj), GET_OBJ_COST(obj));
    send_to_char(buf, ch);

    switch (GET_OBJ_TYPE(obj)) {

    case ITEM_SCROLL:
    case ITEM_POTION:
    case ITEM_SALVE:
      sprintf(buf, "Spells: ");
      if (GET_OBJ_VAL(obj, 1) >= 1)
        sprintf(buf, "%s %s", buf, spells[GET_OBJ_VAL(obj, 1)]);
      if (GET_OBJ_VAL(obj, 2) >= 1)
        sprintf(buf, "%s, %s", buf, spells[GET_OBJ_VAL(obj, 2)]);
      if (GET_OBJ_VAL(obj, 3) >= 1)
        sprintf(buf, "%s, %s", buf, spells[GET_OBJ_VAL(obj, 3)]);
      sprintf(buf, "%s\r\n", buf);
      send_to_char(buf, ch);
      break;

    case ITEM_WAND:
    case ITEM_STAFF:
      if (GET_OBJ_VAL(obj, 1) == 0)
        sprintf(buf, "It seems to be powerless..\r\n");
      else
      {
        if (GET_OBJ_VAL(obj, 1) == 1)
          sprintf(buf, "You sense a faint charge of: ");
        else if (GET_OBJ_VAL(obj, 1) == GET_OBJ_VAL(obj, 2))
          sprintf(buf, "You sense a full charge of: ");
        else
          sprintf(buf, "You sense a moderate charge of: ");

        sprintf(buf, "%s %s\r\n", buf, spells[GET_OBJ_VAL(obj, 3)]);
      }
      send_to_char(buf, ch);
      break;
   
    case ITEM_WEAPON:
      avg = ((GET_OBJ_VAL(obj, 2) * GET_OBJ_VAL(obj, 1)) + GET_OBJ_VAL(obj, 1));
      avg /= 2;
  
      avg = (avg - 1) / 4;
      avg = MAX(0, MIN(4, avg));

      sprintf(buf, "Damage class:  %s\r\n", weap_mesg[avg]);
      send_to_char(buf, ch);
      break;

    case ITEM_ARMOR:
      avg = (GET_OBJ_VAL(obj, 0));

      avg = (avg - 1) / 3;
      avg = MAX(0, MIN(4, avg));

      sprintf(buf, "Armor class:  %s\r\n", armor_mesg[avg]);
      send_to_char(buf, ch);
      break;
    }

    found = FALSE;
    for (i = 0; i < MAX_OBJ_AFFECT; i++) {
      if ((obj->affected[i].location != APPLY_NONE) &&
          (obj->affected[i].modifier != 0)) {
        if (!found) {
          send_to_char("Additional affects:\r\n", ch);
          found = TRUE;
        }
        sprinttype(obj->affected[i].location, apply_types, buf2);
        if (obj->affected[i].modifier > 0)
          sprintf(buf, " - Increases %s", buf2);
        else
          sprintf(buf, " - Decreases %s", buf2);
        send_to_char(buf, ch);
      }
    }
  }
}


ASPELL(spell_cripple)
{
  struct affected_type af;

  if (victim == NULL || ch == NULL)
  {
    log("SYSERR: spell_cripple without ch or vict?");
    return;
  }

  if (!mag_savingthrow(victim, SAVING_SPELL))
  {
    if (IS_NPC(victim) && AWAKE(victim) && (victim != ch) && !FIGHTING(victim))
      hit(victim, ch, TYPE_UNDEFINED);
    return;
  }

  af.type = SPELL_CRIPPLE;
  af.duration = GET_LEVEL(ch) / 5;
  af.location = APPLY_NONE;
  af.bitvector = 0;
  af.modifier = 0;

  affect_join(victim, &af, 0, 0, 0, 0);
  send_to_char("You fall to the ground, clutching your legs.\r\n", victim);
  sprintf(buf, "$N falls to the ground and grabs $S legs in pain.");
  act(buf, FALSE, ch, 0, victim, TO_CHAR);
  act(buf, TRUE, ch, 0, victim, TO_NOTVICT);
  GET_POS(victim) = POS_RESTING;

}


ASPELL(spell_recharge_item)
{
  int old_charges, new_charges, max_charges, percent;
  int dam, spell, spell_level;

  if (!ch || !obj)
    return;

  if ((GET_OBJ_TYPE(obj) != ITEM_WAND) && (GET_OBJ_TYPE(obj) != ITEM_STAFF))
  {
    send_to_char("You cannot recharge this type of item.\r\n", ch);
    return;
  }

  spell = GET_OBJ_VAL(obj, 3);
  if (!GET_SKILL(ch, spell) || !spell_access(ch, spell))
  {
    send_to_char("You don't know that spell well enough.\r\n", ch);
    return;
  }

  spell_level = eff_spell_level(ch, spell);
  old_charges = GET_OBJ_VAL(obj, 1);
  max_charges = GET_OBJ_VAL(obj, 2);

  percent = (GET_SKILL(ch, SPELL_RECHARGE_ITEM) - (spell_level * 5));
  percent = MAX(MIN(percent, 90), 30);
  if (old_charges == 0)
    percent -= 25;   // attempt to recharge a dead item

  if (number(1, 100) > percent)  // recharge attempt failed
  {
    if (old_charges == 0)
      percent -= 25;

    percent = MAX(percent, 5);
    if (number(1, 100) > percent)  // and the item got totally botched
    {
      dam = dice(level, 4);
      act("You have destroyed $p instead!", FALSE, ch, obj, 0, TO_CHAR);
      obj_from_char(obj);
      extract_obj(obj);
      damage(ch, ch, dam, SPELL_RECHARGE_ITEM);
      return;
    }

    act("You fail to recharge $p.", FALSE, ch, obj, 0, TO_CHAR);
    return;
  }

  switch (spell_level)
  {
    case 1:
    case 2:
      new_charges = dice(2, 3);
      break;
    case 3:
    case 4:
      new_charges = dice(1, 6);
      break;
    case 5:
    case 6:
      new_charges = dice(1, 4);
      break;
    case 7:
    case 8:
      new_charges = dice(2, 2) - 1;
      break;
    case 9:
      new_charges = dice(1, 2);
      break;
    default:
      return;
  }

  if ((old_charges > (max_charges * .79)) || (old_charges == max_charges))
  {
    if ((old_charges + new_charges) > max_charges)  // overload!
    {
      dam = dice(spell_level * MAX(4, max_charges), 8);
      if (mag_savingthrow(ch, SAVING_SPELL))
        dam /= 2;

      act("You have overloaded $p!", FALSE, ch, obj, 0, TO_CHAR);
      obj_from_char(obj);
      extract_obj(obj);
      damage(ch, ch, dam, SPELL_RECHARGE_ITEM);
      return;
    }
  }

  new_charges = MIN(max_charges, old_charges + new_charges);
  GET_OBJ_VAL(obj, 1) = new_charges;

  act("You have successfully recharged $p.", 0, ch, obj, 0, TO_CHAR);
  return;
}

