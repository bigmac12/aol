/* ************************************************************************
*   File: mobact.c                                      Part of CircleMUD *
*  Usage: Functions for generating intelligent (?) behavior in mobiles    *
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
#include "db.h"
#include "comm.h"
#include "interpreter.h"
#include "handler.h"
#include "spells.h"

/* external structs */
extern struct char_data *character_list;
extern struct index_data *mob_index;
extern struct room_data *world;
extern struct str_app_type str_app[];
extern int no_specials;

extern void hunt_victim(struct char_data * ch);

int HunterKiller(struct char_data * ch);
void dismount_char(struct char_data * ch);

ACMD(do_get);

#define MOB_AGGR_TO_ALIGN (MOB_AGGR_EVIL | MOB_AGGR_NEUTRAL | MOB_AGGR_GOOD)

void mob_casting_acts(struct char_data * ch)
{
  struct char_data *vict;
  int i, flags, chance, spellnum, numspell = 0;
  int spell_list[NUM_SPELL_FLAGS];
  extern int mob_spells_mage[NUM_SPELL_FLAGS];
  extern int mob_spells_cleric[NUM_SPELL_FLAGS];
  extern int mob_spells_ranger[NUM_SPELL_FLAGS];
  extern int mob_spells_druid[NUM_SPELL_FLAGS];
  extern int mob_spells_paladin[NUM_SPELL_FLAGS];


  if (GET_POS(ch) != POS_FIGHTING)
    return;

  if (GET_LEVEL(ch) > 25)
    chance = number(0, 2);
  else if (GET_LEVEL(ch) > 20)
    chance = number(0, 3);
  else if (GET_LEVEL(ch) > 15)
    chance = number(0, 4);
  else if (GET_LEVEL(ch) > 10)
    chance = number(0, 5);
  else
    chance = number(0, 6);

  if (chance)
    return;

  for (vict = world[ch->in_room].people; vict; vict = vict->next_in_room)
    if ((FIGHTING(vict) == ch) && !number(0, 3))
      break;

  if (vict == NULL)
    vict = FIGHTING(ch);

  flags = NUM_SPELL_FLAGS;

  if (IS_RANGER(ch) || IS_PALADIN(ch))
    flags /= 2;

  for (i = 0; i < NUM_SPELL_FLAGS; i++)
  {
    if (IS_SET(SPELL_FLAGS(ch), 1 << i))
    {
      spell_list[numspell] = i;
      numspell++;
    }
  }

  if (numspell == 0)
    return;

  spellnum = spell_list[number(1, numspell) - 1];

  if (IS_MAGE(ch))
    spellnum = mob_spells_mage[spellnum];
  else if (IS_CLERIC(ch))
    spellnum = mob_spells_cleric[spellnum];
  else if (IS_RANGER(ch))
    spellnum = mob_spells_ranger[spellnum];
  else if (IS_DRUID(ch))
    spellnum = mob_spells_druid[spellnum];
  else if (IS_PALADIN(ch))
    spellnum = mob_spells_paladin[spellnum];
  else
    return;

  switch (spellnum)
  {
    case SPELL_ARMOR:
    case SPELL_STRENGTH:
    case SPELL_CURE_LIGHT:
    case SPELL_CURE_MODERATE:
    case SPELL_CURE_SEVERE:
    case SPELL_CURE_CRITIC:
    case SPELL_HEAL:
    case SPELL_STONESKIN:
      vict = ch;
      break;
    default:
      break;
  }

  cast_spell(ch, vict, NULL, spellnum);

  return;
}

void mobile_activity(void)
{
  register struct char_data *ch, *next_ch, *vict;
  struct obj_data *obj, *best_obj;
  int door, luck, found, max;
  memory_rec *names;

  for (ch = character_list; ch; ch = next_ch) {
    log(ch->player.name);
    next_ch = ch->next;
    log(next_ch->player.name);

    if (!IS_MOB(ch))
      continue;

    /* Examine call for special procedure */
    if (MOB_FLAGGED(ch, MOB_SPEC) && !no_specials) {
      log("%hd", ch->nr);
      log("%d", mob_index[GET_MOB_RNUM(ch)]);
      mob_index[GET_MOB_RNUM(ch)].func;
      if (mob_index[GET_MOB_RNUM(ch)].func == NULL) {
        GET_NAME(ch, chname);

        log("SYSERR: %s (#%d): Attempting to call non-existing mob func",
          chname, GET_MOB_VNUM(ch));
            FREE_NAME(chname);

        log("%ld", ch->char_specials.saved.act);
        REMOVE_BIT(MOB_FLAGS(ch), MOB_SPEC);
      } else {
        if ((mob_index[GET_MOB_RNUM(ch)].func) (ch, ch, 0, ""))
          continue;		/* go to next char */
          }
    }

    /* If the mob has no specproc, do the default actions */
    if (FIGHTING(ch) || !AWAKE(ch))
      continue;

    /*  PDH  4/ 9/99 - check for HunterKiller  */
    /*  Removed temporarily - Solinari, 8/99 */
    if (MOB_FLAGGED(ch, MOB_HUNTERKILLER) && !(MOB_FLAGGED(ch, MOB_QUEST)) && !FIGHTING(ch) && AWAKE(ch)) {
      hunt_victim(ch);
      continue;
    }

    /* Scavenger (picking up objects) */
    if (MOB_FLAGGED(ch, MOB_SCAVENGER) && !(MOB_FLAGGED(ch, MOB_QUEST)) && !FIGHTING(ch) && AWAKE(ch))
      if (world[ch->in_room].contents && !number(0, 10)) {
        max = 1;
        best_obj = NULL;
        for (obj = world[ch->in_room].contents; obj; obj = obj->next_content)
          if (CAN_GET_OBJ(ch, obj) && GET_OBJ_COST(obj) > max) {
            best_obj = obj;
            max = GET_OBJ_COST(obj);
          }

        if (best_obj != NULL) {
          obj_from_room(best_obj);
          obj_to_char(best_obj, ch);
          act("$n gets $p.", FALSE, ch, best_obj, 0, TO_ROOM);
        }
      }

    /* Mob Movement */
    if (!MOB_FLAGGED(ch, MOB_SENTINEL) && !MOB_FLAGGED(ch, MOB_QUEST) && (GET_POS(ch) == POS_STANDING) &&
	((door = number(0, 18)) < NUM_OF_DIRS) && CAN_GO(ch, door) &&
	!ROOM_FLAGGED(EXIT(ch, door)->to_room, ROOM_NOMOB | ROOM_DEATH) &&
	(!MOB_FLAGGED(ch, MOB_STAY_ZONE) ||
	 (world[EXIT(ch, door)->to_room].zone == world[ch->in_room].zone))) {
      perform_move(ch, door, 1);
    }

    /* Aggressive Mobs */
    if (MOB_FLAGGED(ch, MOB_AGGRESSIVE | MOB_AGGR_TO_ALIGN) && !IS_AFFECTED(ch, AFF_CONCEAL_ALIGN) && !(MOB_FLAGGED(ch, MOB_QUEST))) {
      found = FALSE;
      for (vict = world[ch->in_room].people; vict && !found; vict = vict->next_in_room) {
	if (IS_NPC(vict) || !CAN_SEE(ch, vict) || PRF_FLAGGED(vict, PRF_NOHASSLE))
	  continue;
	if (MOB_FLAGGED(ch, MOB_WIMPY) && AWAKE(vict))
	  continue;
	if ((!(MOB_FLAGGED(ch, MOB_AGGR_TO_ALIGN)) ||
	    (MOB_FLAGGED(ch, MOB_AGGR_EVIL) && IS_EVIL(vict)) ||
	    (MOB_FLAGGED(ch, MOB_AGGR_NEUTRAL) && IS_NEUTRAL(vict)) ||
	    (MOB_FLAGGED(ch, MOB_AGGR_GOOD) && IS_GOOD(vict))) &&
            !IS_AFFECTED(vict, AFF_CONCEAL_ALIGN) && !(MOB_FLAGGED(ch, MOB_QUEST))) {
	  hit(ch, vict, TYPE_UNDEFINED);
	  found = TRUE;
	}
      }
    }

    /* Mob Memory */
    if (MOB_FLAGGED(ch, MOB_MEMORY) && !(MOB_FLAGGED(ch, MOB_QUEST)) && MEMORY(ch)) {
      found = FALSE;
      for (vict = world[ch->in_room].people;
	   vict && !found;
	   vict = vict->next_in_room) {
	if (IS_NPC(vict) ||
	    !CAN_SEE(ch, vict) ||
	    PRF_FLAGGED(vict, PRF_NOHASSLE))
	  continue;
	for (names = MEMORY(ch); names && !found; names = names->next)
	  if (names->id == GET_IDNUM(vict)) {
	    found = TRUE;
	    act("'Hey!  You're the fiend that attacked me!', exclaims $n.",
		FALSE, ch, 0, 0, TO_ROOM);
	    hit(ch, vict, TYPE_UNDEFINED);
	  }
      }
    }

   /*Mountable Mobs* -- Luni*/
    if (MOB_FLAGGED(ch, MOB_MOUNTABLE) && !AFF_FLAGGED(ch, AFF_BLIND)) {
      found = FALSE;
      for (vict = world[ch->in_room].people;
           vict && !found;
           vict = vict->next_in_room) {
      if (ch == vict || IS_NPC(vict) | !FIGHTING(vict))
          continue;
      if (IS_NPC(FIGHTING(vict)) || ch == FIGHTING(vict))
          continue;
      if (CAN_SEE(ch, FIGHTING(vict))) {
        if (RIDING(vict) == ch) {
         luck = number(1, 10);
         if(luck <= 3) {
          act("$n jumps to the aid of $N!", FALSE, ch, 0, vict, TO_ROOM);
          hit(ch, FIGHTING(vict), TYPE_UNDEFINED);
          found = TRUE;
           }
         else {
           act("$N bucks upwards in fear, throwing you violently to the ground!", FALSE, vict, 0, ch, TO_CHAR);
   act("$n is thrown to the ground as $N violently bucks in fear!", TRUE, vict, 0, ch, TO_NOTVICT);
   act("You buck violently and throw $n to the ground.", FALSE, vict, 0, ch, TO_VICT);
    dismount_char(vict);
    damage(vict, vict, dice(1,6), -1);
    found = TRUE;
 } 
}
}             
} 
} 

    /* Helper Mobs */
    if (MOB_FLAGGED(ch, MOB_HELPER) &&
        !(MOB_FLAGGED(ch, MOB_QUEST)) &&
        !AFF_FLAGGED(ch, AFF_BLIND | AFF_CHARM)) {
      found = FALSE;
      for (vict = world[ch->in_room].people; vict && !found; vict = vict->next_in_room) {
        if (ch == vict || !IS_NPC(vict) | !FIGHTING(vict))
          continue;
        if (IS_NPC(FIGHTING(vict)) || ch == FIGHTING(vict))
          continue;
        if (CAN_SEE(ch, FIGHTING(vict))) { 
          act("$n jumps to the aid of $N!", FALSE, ch, 0, vict, TO_ROOM);
          hit(ch, FIGHTING(vict), TYPE_UNDEFINED);
          found = TRUE;
        }
      }
    }
    /* Add new mobile actions here */

  }				/* end for() */
}



/* Mob Memory Routines */

/* make ch remember victim */
void remember(struct char_data * ch, struct char_data * victim)
{
  memory_rec *tmp;
  bool present = FALSE;

  if (!IS_NPC(ch) || IS_NPC(victim) || (GET_LEVEL(victim) >= LVL_IMMORT))
    return;

  for (tmp = MEMORY(ch); tmp && !present; tmp = tmp->next)
    if (tmp->id == GET_IDNUM(victim))
      present = TRUE;

  if (!present) {
    CREATE(tmp, memory_rec, 1);
    tmp->next = MEMORY(ch);
    tmp->id = GET_IDNUM(victim);
    MEMORY(ch) = tmp;
  }
}


/* make ch forget victim */
void forget(struct char_data * ch, struct char_data * victim)
{
  memory_rec *curr, *prev = NULL;

  if (!(curr = MEMORY(ch)))
    return;

  while (curr && curr->id != GET_IDNUM(victim)) {
    prev = curr;
    curr = curr->next;
  }

  if (!curr)
    return;			/* person wasn't there at all. */

  if (curr == MEMORY(ch))
    MEMORY(ch) = curr->next;
  else
    prev->next = curr->next;

  free(curr);
}


/* erase ch's memory */
void clearMemory(struct char_data * ch)
{
  memory_rec *curr, *next;

  curr = MEMORY(ch);

  while (curr) {
    next = curr->next;
    free(curr);
    curr = next;
  }

  MEMORY(ch) = NULL;
}


int HunterKiller(struct char_data * ch)
{
  struct char_data * tch, * next_tch, * killer = NULL;
  int dir, found = 0;
  extern int find_first_step(sh_int src, sh_int target);
  ACMD(do_say);


  if ( ! AWAKE(ch) || FIGHTING(ch) )
    return FALSE;

  for (tch=character_list; tch; tch=next_tch) {
    next_tch = tch->next;

    if ( ! IS_NPC(tch) &&
	 CAN_SEE(ch, tch) &&
	 IS_SET(PLR_FLAGS(tch), PLR_KILLER) &&
	 (world[tch->in_room].zone == world[ch->in_room].zone) ) {
      killer = tch;
      found = 1;
      break;
    }
  }

  if ( ! found ) {
    return FALSE;
  }

  HUNTING(ch) = killer;
  /*
    hunt_victim(ch);
    */

  dir = find_first_step(ch->in_room, HUNTING(ch)->in_room);

  switch (dir) {
  case BFS_ERROR:
    send_to_char("Hmm.. something seems to be wrong.\r\n", ch);
    break;
  case BFS_ALREADY_THERE:
    send_to_char("You're already in the same room!!\r\n", ch);
    break;
  case BFS_NO_PATH:
    sprintf(buf, "You can't sense a trail to %s from here.\r\n",
	    HMHR(HUNTING(ch)));
    send_to_char(buf, ch);
    break;
  default:
    perform_move(ch, dir, 1);
    break;
  }

  if (ch->in_room == HUNTING(ch)->in_room) {
    hit(ch, HUNTING(ch), TYPE_UNDEFINED);
  }

  return TRUE;
}
