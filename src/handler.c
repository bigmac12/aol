/* ************************************************************************
*   File: handler.c                                     Part of CircleMUD *
*  Usage: internal funcs: moving and finding chars/objs                   *
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
#include "db.h"
#include "handler.h"
#include "interpreter.h"
#include "spells.h"
#include "dg_scripts.h"

/* external vars */
extern room_rnum top_of_world;
extern struct room_data *world;
extern struct obj_data *object_list;
extern struct char_data *character_list;
extern struct index_data *mob_index;
extern struct index_data *obj_index;
extern struct descriptor_data *descriptor_list;
extern char *MENU;
extern char *spell_wear_off_msg[];

/* external functions */
void free_char(struct char_data * ch);
void stop_fighting(struct char_data * ch);
void remove_follower(struct char_data * ch);
void clearMemory(struct char_data * ch);
void dismount_char(struct char_data *ch);
void mount_char(struct char_data *ch, struct char_data *mount);
int mem_int_app(struct char_data * ch);
void shift_memtimes(struct char_data * ch, int shift);
void skip_dot(char **string);
int mag_savingthrow(struct char_data * ch, int type);
int hidden_eq(struct char_data *ch, struct obj_data *obj);
char *current_short_desc(struct char_data *ch);


char *fname(char *namelist)
{
  static char holder[30];
  register char *point;

  for (point = holder; isalpha(*namelist); namelist++, point++)
    *point = *namelist;

  *point = '\0';

  return (holder);
}

int check_one_name(char *str, char *namelist)
{
  register char *curname, *curstr;

  curname = namelist;
  for (;;) {
    for (curstr = str;; curstr++, curname++) {
      if (!*curstr && !isalpha(*curname))
	return (1);

      if (!*curname)
	return (0);

      if (!*curstr || *curname == ' ')
	break;

      if (LOWER(*curstr) != LOWER(*curname))
	break;
    }


    for (; isalpha(*curname); curname++);
    if (!*curname)
      return (0);
    curname++;
  }
}

int isname(char *str, char *namelist)
{
  char *restname, curname[256];

  restname = one_arg_dots(str, curname);
  while (*curname)
  {
    if (!check_one_name(curname, namelist))
      return 0;

    restname = one_arg_dots(restname, curname);
    skip_dot(&restname);
  }

  return 1;
}

void affect_modify(struct char_data * ch, byte loc, sbyte mod, long bitv,
		        bool add)
{
  if (add) {
    SET_BIT(AFF_FLAGS(ch), bitv);
  } else {
    REMOVE_BIT(AFF_FLAGS(ch), bitv);
    mod = -mod;
  }

  switch (loc) {
  case APPLY_NONE:
    break;

  case APPLY_STR:
    GET_STR(ch) += mod;
    break;
  case APPLY_DEX:
    GET_DEX(ch) += mod;
    break;
  case APPLY_INT:
    GET_INT(ch) += mod;
    break;
  case APPLY_WIS:
    GET_WIS(ch) += mod;
    break;
  case APPLY_CON:
    GET_CON(ch) += mod;
    break;
  case APPLY_CHA:
    GET_CHA(ch) += mod;
    break;

  case APPLY_CLASS:
    /* ??? GET_CLASS(ch) += mod; */
    break;

  case APPLY_LEVEL:
    /* ??? GET_LEVEL(ch) += mod; */
    break;

  case APPLY_AGE:
    ch->player.time.birth -= (mod * SECS_PER_MUD_YEAR);
    break;

  case APPLY_CHAR_WEIGHT:
    GET_WEIGHT(ch) += mod;
    break;

  case APPLY_CHAR_HEIGHT:
    GET_HEIGHT(ch) += mod;
    break;

  case APPLY_HIT:
    GET_MAX_HIT(ch) += mod;
    break;

  case APPLY_MOVE:
    GET_MAX_MOVE(ch) += mod;
    break;

  case APPLY_GOLD:
    break;

  case APPLY_EXP:
    break;

  case APPLY_AC:
    GET_AC(ch) += mod;
    break;

  case APPLY_HITROLL:
    GET_HITROLL(ch) += mod;
    break;

  case APPLY_DAMROLL:
    GET_DAMROLL(ch) += mod;
    break;

  case APPLY_SAVING_PARA:
    GET_SAVE(ch, SAVING_PARA) += mod;
    break;

  case APPLY_SAVING_ROD:
    GET_SAVE(ch, SAVING_ROD) += mod;
    break;

  case APPLY_SAVING_PETRI:
    GET_SAVE(ch, SAVING_PETRI) += mod;
    break;

  case APPLY_SAVING_BREATH:
    GET_SAVE(ch, SAVING_BREATH) += mod;
    break;

  case APPLY_SAVING_SPELL:
    GET_SAVE(ch, SAVING_SPELL) += mod;
    break;

  case APPLY_LIGHT:
    break;

  case APPLY_UNDEFINED:
  case APPLY_UNDEFINED_2:
    break;

  default:
    {
      log("SYSERR: Unknown apply adjust %p attempt (%s, affect_modify).",
        log, __FILE__);
    }
    break;

  } /* switch */
}



/* This updates a character by subtracting everything he is affected by */
/* restoring original abilities, and then affecting all again           */
void affect_total(struct char_data * ch)
{
  struct affected_type *af;
  int i, j, newint, change, oldint;

  oldint = mem_int_app(ch);

  for (i = 0; i < NUM_WEARS; i++) {
    if (GET_EQ(ch, i)) {
      for (j = 0; j < MAX_OBJ_AFFECT; j++) {
	affect_modify(ch, GET_EQ(ch, i)->affected[j].location,
		      GET_EQ(ch, i)->affected[j].modifier,
		      GET_EQ(ch, i)->obj_flags.bitvector, FALSE);
      }
    }
  }


  for (af = ch->affected; af; af = af->next) {
    affect_modify(ch, af->location, af->modifier, af->bitvector, FALSE);
  }

  ch->aff_abils = ch->real_abils;

  for (i = 0; i < NUM_WEARS; i++) {
    if (GET_EQ(ch, i)) {
      for (j = 0; j < MAX_OBJ_AFFECT; j++) {
	affect_modify(ch, GET_EQ(ch, i)->affected[j].location,
		      GET_EQ(ch, i)->affected[j].modifier,
		      GET_EQ(ch, i)->obj_flags.bitvector, TRUE);
      }
    }
  }


  for (af = ch->affected; af; af = af->next) {
    affect_modify(ch, af->location, af->modifier, af->bitvector, TRUE);
  }


  /* Make certain values are between 0..25, not < 0 and not > 25! */

//  i = (IS_NPC(ch) ?  25 :
//       (GET_LEVEL(ch) > LVL_IMMORT) ? 25 : 20);

  i = 25;

  GET_DEX(ch) = MAX(0, MIN(GET_DEX(ch), i));
  GET_INT(ch) = MAX(0, MIN(GET_INT(ch), i));
  GET_WIS(ch) = MAX(0, MIN(GET_WIS(ch), i));
  GET_CON(ch) = MAX(0, MIN(GET_CON(ch), i));
  GET_STR(ch) = MAX(0, GET_STR(ch));


  if (IS_NPC(ch)) {
    GET_STR(ch) = MIN(GET_STR(ch), i);
  } else {
    /*  PDH  4/29/99
     *  new method for STRs over 18 and such
     *  now, when PC str > 18/00 go up to 19, 20, etc...
     */
    int  addOver100 = 0;

    if ( (GET_STR(ch) > 18) && (ch->real_abils.str_add != -1) ) {
      /*  STR that is NOT naturally over 18  */

      i = GET_ADD(ch) + ((GET_STR(ch) - 18) * 10);

      if ( i >= 110 ) {
	/*  19, 20, 21, ... 25  */
	addOver100 = ( i - 100) / 10;
	GET_ADD(ch) = 0;
	GET_STR(ch) = MIN(25, 18 + addOver100);
      } else {
	GET_ADD(ch) = MIN(i, 100);
	GET_STR(ch) = 18;
      }
    } else {
      /*  STR that is naturally over 18 (ie. minotaur)  */
    }
  }

  newint = mem_int_app(ch);
  change = newint - oldint;
  if (change)
    shift_memtimes(ch, change);
}



/* Insert an affect_type in a char_data structure
   Automatically sets apropriate bits and apply's */
void affect_to_char(struct char_data * ch, struct affected_type * af)
{
  struct affected_type *affected_alloc;

  CREATE(affected_alloc, struct affected_type, 1);

  *affected_alloc = *af;
  affected_alloc->next = ch->affected;
  ch->affected = affected_alloc;

  affect_modify(ch, af->location, af->modifier, af->bitvector, TRUE);
  affect_total(ch);
}



/*
 * Remove an affected_type structure from a char (called when duration
 * reaches zero). Pointer *af must never be NIL!  Frees mem and calls
 * affect_location_apply
 */
void affect_remove(struct char_data * ch, struct affected_type * af, int output)
{
  struct affected_type *temp;
  struct affected_type aff;
  bool accum_affect = FALSE;
  int k;

  if (ch->affected == NULL) {
    core_dump();
    return;
  }

  switch (af->type)
  {

    case SPELL_CHARM:
    { 
      struct char_data* victim = ch->master;
      if (output == 0) break;

      affect_modify(ch, af->location, af->modifier, af->bitvector, FALSE);
      REMOVE_FROM_LIST(af, ch->affected, next);
      free(af);
      affect_total(ch);

      if (ch->master)
      {
        stop_follower(ch);
      }

      if (victim)
      {
        if(IS_NPC(ch))
        {
          SET_BIT(MOB_FLAGS(ch), MOB_AGGRESSIVE | MOB_MEMORY);
        }
        if (mag_savingthrow(victim, SAVING_SPELL))
        {
          hit(victim, ch, TYPE_UNDEFINED);
        }
      }
      return;
    }

    case SPELL_LIGHT:
      if (output == 0) break;
      if (!af->next || (af->next->type != af->type) ||
         (af->next->duration > 0)) 
      {
        if (world[ch->in_room].name != (char*) NULL)
        {
          world[ch->in_room].light -= 10;
        }
      }
      break;
    case SPELL_DARKNESS:
      if (output == 0) break;
      if (!af->next || (af->next->type != af->type) ||
         (af->next->duration > 0)) 
      {
        if (world[ch->in_room].name != (char*) NULL)
        {
          world[ch->in_room].light += 10;
        }
      }
      break;
    case SPELL_BLACK_PLAGUE:
      mag_affects(30, ch, ch, SPELL_BREATH_OF_LIFE, SAVING_SPELL);
      break;
    case SPELL_CALL_ANIMAL_SPIRIT:
    case SPELL_ANIMAL_SUMMONING:
    case SPELL_ANIMAL_SUMMONING_II:
    case SPELL_ANIMAL_SUMMONING_III:
    case SPELL_CONJURE_ELEMENTAL:
    case SPELL_GREATER_ELEMENTAL:
    case SPELL_DUST_DEVIL:
    case SPELL_STICKS_TO_SNAKES:
    case SPELL_SUMMON_INSECTS:
    case SPELL_AERIAL_SERVANT:
    case SPELL_SUMMON_GUARD:
      if (IS_NPC(ch))
      {
        if (GET_POS(ch) > POS_DEAD)
        {
          if (output == 1)
          {
            affect_modify(ch, af->location, af->modifier, af->bitvector, FALSE);
            REMOVE_FROM_LIST(af, ch->affected, next);
            free(af);
            affect_total(ch);

            GET_NAME(ch, chname);
	    stop_fighting(ch); /*Fighting Bug Fix Jasrags*/
            sprintf(buf, "%s disappears into thin air as the summoning ends.", 
              chname);
            act(buf, FALSE, world[ch->in_room].people, 0, 0, TO_ROOM);
            FREE_NAME(chname);
            extract_char(ch);
            ch = NULL;
            return;
          }
        }
      }
      break;
   case SPELL_POLYMORPH:
   if (!PRF_FLAGGED(ch, PRF_NOTSELF)) {
          affect_modify(ch, af->location, af->modifier, af->bitvector, FALSE);
          REMOVE_FROM_LIST(af, ch->affected, next);
          free(af);
          affect_total(ch);
          return;
   }
   break;
   case SPELL_DONTUSEME:
   if(AFF_FLAGGED(ch, AFF_STANCE) && !AFF_FLAGGED(ch, AFF_TIRED)) {
      aff.type = SKILL_STANCE;
      aff.duration = 2;
      aff.location = APPLY_STR;
      aff.modifier = -2;
      aff.bitvector = AFF_TIRED;
      accum_affect = FALSE;

      affect_to_char(ch, &aff);
    }
   break;
    default:
      break;
  }

  if (output && (af->type > 0) && (af->type <= MAX_SPELLS) && af->type != SPELL_POLYMORPH && af->type != SPELL_DONTUSEME) {
    if (!af->next || (af->next->type != af->type) ||
        (af->next->duration > 0)) {
      if (*spell_wear_off_msg[af->type]) {
        send_to_char(spell_wear_off_msg[af->type], ch);
        send_to_char("\r\n", ch);
      }
  }
}

if (output && (af->type > 0) && (af->type <= MAX_SPELLS) && af->type == SPELL_POLYMORPH)  {

  if (!af->next || (af->next->type != af->type) ||
        (af->next->duration > 0)) {
   if (PLR_FLAGGED(ch, PLR_RABBIT)){
REMOVE_BIT(PLR_FLAGS(ch), PLR_RABBIT);
REMOVE_BIT(PRF_FLAGS(ch), PRF_NOTSELF);
send_to_char("You feel yourself growing, and your ears shrinking. You no longer feel like a rabbit.\r\n", ch);
act("$n's body grows, $s ears shrinking. $n no longer looks like a rabbit.\r\n", 0, ch, 0, 0, TO_ROOM);

}
if (PLR_FLAGGED(ch, PLR_BIRD)) {
REMOVE_BIT(PLR_FLAGS(ch), PLR_BIRD);
REMOVE_BIT(PRF_FLAGS(ch), PRF_NOTSELF);
send_to_char("You feel yourself growing and your feathers falling away. You no longer feel like a bird.\r\n", ch);
act("$n's body grows, $s feathers falling away as it expands. $n no longer looks like a bird.\r\n", 0, ch, 0, 0, TO_ROOM);

}
if (PLR_FLAGGED(ch, PLR_WOLF)) {
REMOVE_BIT(PLR_FLAGS(ch), PLR_WOLF);
REMOVE_BIT(PRF_FLAGS(ch), PRF_NOTSELF);
send_to_char("You feel your your fur shed and your teeth shrink. You no longer feel like a wolf.\r\n", ch);
act("$n's teeth shrink, $s fur shedding. $n no longer looks like a wolf.\r\n", 0, ch, 0, 0, TO_ROOM);
}
if (PLR_FLAGGED(ch, PLR_BEAR)) {
REMOVE_BIT(PLR_FLAGS(ch), PLR_BEAR);
REMOVE_BIT(PRF_FLAGS(ch), PRF_NOTSELF);
send_to_char("Your claws shrink as does the rest of your body. You no longer feel like a bear.\r\n", ch);
act("$n's claws shrink as does the rest of $s body. $n no longer looks like a bear.\r\n", 0, ch, 0, 0, TO_ROOM);
}
if (PLR_FLAGGED(ch, PLR_CAT)){
REMOVE_BIT(PLR_FLAGS(ch), PLR_CAT);
REMOVE_BIT(PRF_FLAGS(ch), PRF_NOTSELF);
send_to_char("You feel your body growing, and your fur shedding. You no longer feel like a cat.\r\n", ch);
act("$n's body slowly grows, $s fur shedding. $n no longer looks like a cat.\r\n", 0, ch, 0, 0, TO_ROOM);
}

for (k = 0; k < NUM_WEARS; k++)
  if (GET_EQ(ch, k)){
    GET_OBJ_DISGUISE(GET_EQ(ch, k)) = 0;
  }

}
} 

  affect_modify(ch, af->location, af->modifier, af->bitvector, FALSE);
  REMOVE_FROM_LIST(af, ch->affected, next);
  free(af);
  affect_total(ch);
}


/* Call affect_remove with every spell of spelltype "skill" */
void affect_from_char(struct char_data * ch, int type)
{
  struct affected_type *hjp, *next;
  int message = 0;

  for (hjp = ch->affected; hjp; hjp = next) {
    next = hjp->next;
    if (hjp->type == type)
    {
      if (message)
        affect_remove(ch, hjp, 0);
      else
        affect_remove(ch, hjp, 1);

      message = 1;
    }
  }
}

void affect_from_char_II(struct char_data * ch, int skill, int type, int action)
{

 struct affected_type *aff, *next;
 struct affected_type *temp;
 struct affected_type af[3];
 int i, k;
 bool accum_affect = FALSE, accum_duration = FALSE;
 
 for (aff = ch->affected; aff; aff = next) {
    next = aff->next;
    if (aff->type == type) {
    affect_modify(ch, aff->location, aff->modifier, aff->bitvector, FALSE);
  REMOVE_FROM_LIST(aff, ch->affected, next);
  free(aff);
  affect_total(ch);
 }
}

if (action == 2) {
switch (skill) {
      case SPELL_POLYMORPH:
   if (PLR_FLAGGED(ch, PLR_RABBIT)){
REMOVE_BIT(PLR_FLAGS(ch), PLR_RABBIT);
REMOVE_BIT(PRF_FLAGS(ch), PRF_NOTSELF);
send_to_char("You feel yourself growing, and your ears shrinking. You no longer feel like a rabbit.\r\n", ch);
act("$n's body grows, $s ears shrinking. $n no longer looks like a rabbit.\r\n", 0, ch, 0, 0, TO_ROOM);

}
if (PLR_FLAGGED(ch, PLR_BIRD)) {
REMOVE_BIT(PLR_FLAGS(ch), PLR_BIRD);
REMOVE_BIT(PRF_FLAGS(ch), PRF_NOTSELF);
send_to_char("You feel yourself growing and your feathers falling away. You no longer feel like a bird.\r\n", ch);
act("$n's body grows, $s feathers falling away as it expands. $n no longer looks like a bird.\r\n", 0, ch, 0, 0, TO_ROOM);

}
if (PLR_FLAGGED(ch, PLR_WOLF)) {
REMOVE_BIT(PLR_FLAGS(ch), PLR_WOLF);
REMOVE_BIT(PRF_FLAGS(ch), PRF_NOTSELF);
send_to_char("You feel your your fur shed and your teeth shrink. You no longer feel like a wolf.\r\n", ch);
act("$n's teeth shrink, $s fur shedding. $n no longer looks like a wolf.\r\n", 0, ch, 0, 0, TO_ROOM);
}
if (PLR_FLAGGED(ch, PLR_BEAR)) {
REMOVE_BIT(PLR_FLAGS(ch), PLR_BEAR);
REMOVE_BIT(PRF_FLAGS(ch), PRF_NOTSELF);
send_to_char("Your claws shrink as does the rest of your body. You no longer feel like a bear.\r\n", ch);
act("$n's claws shrink as does the rest of $s body. $n no longer looks like a bear.\r\n", 0, ch, 0, 0, TO_ROOM);
}
if (PLR_FLAGGED(ch, PLR_CAT)){
REMOVE_BIT(PLR_FLAGS(ch), PLR_CAT);
REMOVE_BIT(PRF_FLAGS(ch), PRF_NOTSELF);
send_to_char("You feel your body growing, and your fur shedding. You no longer feel like a cat.\r\n", ch);
act("$n's body slowly grows, $s fur shedding. $n no longer looks like a cat.\r\n", 0, ch, 0, 0, TO_ROOM);
}

  for (k = 0; k < NUM_WEARS; k++)
  if (GET_EQ(ch, k)){
    GET_OBJ_DISGUISE(GET_EQ(ch, k)) = 0;
  }

       if (affected_by_spell(ch, SPELL_FLIGHT))
       affect_from_char_II(ch, SPELL_FLIGHT, SPELL_FLIGHT, 1);
       if (affected_by_spell(ch, SPELL_HASTE))
       affect_from_char_II(ch, SPELL_HASTE, SPELL_HASTE, 1);
       break;
      case SKILL_STANCE:

   if (!AFF_FLAGGED(ch, AFF_TIRED)) {
   for (i = 0; i < 3; i++) {
    af[0].type     = SPELL_DONTUSEME;
    af[0].location = APPLY_HITROLL;
    af[0].modifier = 2;
    af[0].duration = 7;
    af[0].bitvector = AFF_STANCE;

    af[1].type      = SPELL_DONTUSEME;
    af[1].location = APPLY_AC;
    af[1].modifier = -50;
    af[1].duration = 7;
    af[1].bitvector = AFF_STANCE;
 
    af[2].type      = SPELL_DONTUSEME;
    af[2].location = APPLY_STR;
    af[2].modifier = 2;
    af[2].duration = 7;
    af[2].bitvector = AFF_STANCE;


      if (af[i].bitvector || (af[i].location != APPLY_NONE)) {
        affect_join(ch, af+i, accum_duration, FALSE, accum_affect, FALSE);
      }
    }
}
     break;

    default:
      break;
  }
 }
}


void unaffect_one(struct char_data * ch, int type)
{
  struct affected_type *hjp, *next;

  for (hjp = ch->affected; hjp; hjp = next)
  {
    next = hjp->next;
    if (hjp->type == type)
    {
      affect_remove(ch, hjp, 0);
      return;
    }
  }

  GET_NAME(ch, chname);
  sprintf(buf, "SYSERR: Couldn't unaffect_one spellnum %d from %s!", type,
          chname);
  FREE_NAME(chname);
  mudlog(buf, BRF, LVL_GRGOD, TRUE);
}

void affect_remove_nonperm(struct char_data * ch, int type)
{
  struct affected_type *hjp, *next;

  for (hjp = ch->affected; hjp; hjp = hjp->next)
  {
    next = hjp->next;
    if ((hjp->type == type) && (hjp->duration != -1))
      affect_remove(ch, hjp, 1);
  }
}

void affect_remove_nonperm_II(struct char_data * ch, int type)
{
  struct affected_type *hjp, *next;

  for (hjp = ch->affected; hjp; hjp = hjp->next)
  {
    next = hjp->next;
    if (hjp->type == type)
      affect_remove(ch, hjp, 1);
  }
}



/*
 * Return if a char is affected by a spell (SPELL_XXX), NULL indicates
 * not affected
 */
bool affected_by_spell(struct char_data * ch, int type)
{
  struct affected_type *hjp;

  for (hjp = ch->affected; hjp; hjp = hjp->next)
    if (hjp->type == type)
      return TRUE;

  return FALSE;
}



void affect_join(struct char_data * ch, struct affected_type * af,
		      bool add_dur, bool avg_dur, bool add_mod, bool avg_mod)
{
  struct affected_type *hjp;
  bool found = FALSE;

  for (hjp = ch->affected; !found && hjp; hjp = hjp->next) {

    if ((hjp->type == af->type) && (hjp->location == af->location)) {
      if (add_dur) {
	af->duration += hjp->duration;
      }

      if (avg_dur) {
	af->duration >>= 1;
      }

      if (add_mod) {
	af->modifier += hjp->modifier;
      }

      if (avg_mod) {
	af->modifier >>= 1;
      }

      affect_remove(ch, hjp, 0);
      affect_to_char(ch, af);
      found = TRUE;
    }
  }

  if (!found) {
    affect_to_char(ch, af);
  }

}


/* move a player out of a room */
void char_from_room(struct char_data * ch)
{
  struct char_data *temp;

  if (ch == NULL || ch->in_room == NOWHERE) {
    log("SYSERR: NULL or NOWHERE in handler.c, char_from_room");
    exit(1);
  }

  if (FIGHTING(ch) != NULL) {
    stop_fighting(ch);
  }

  if ( affected_by_spell(ch, SPELL_LIGHT) ) {
    world[ch->in_room].light -= 10;
  }
  if ( affected_by_spell(ch, SPELL_DARKNESS) ) {
    world[ch->in_room].light += 10;
  }

  if (GET_EQ(ch, WEAR_LIGHT) != NULL) {
    if (GET_OBJ_TYPE(GET_EQ(ch, WEAR_LIGHT)) == ITEM_LIGHT) {
      if (GET_OBJ_VAL(GET_EQ(ch, WEAR_LIGHT), 2)) {     /* Light is ON */
	world[ch->in_room].light--;
      }
    }
  }

/*
  if (affected_by_spell(ch, SPELL_ENTANGLE))
    affect_from_char(ch, SPELL_ENTANGLE);
  if (affected_by_spell(ch, SPELL_MIRE))
    affect_from_char(ch, SPELL_MIRE);
*/

  REMOVE_FROM_LIST(ch, world[ch->in_room].people, next_in_room);
  ch->in_room = NOWHERE;
  ch->next_in_room = NULL;
}


/* place a character in a room */
void char_to_room(struct char_data * ch, room_rnum room)
{
  if (ch == NULL || room < 0 || room > top_of_world)
    log("SYSERR: Illegal value(s) passed to char_to_room");
  else {
    ch->next_in_room = world[room].people;
    world[room].people = ch;
    ch->in_room = room;

    if ( affected_by_spell(ch, SPELL_LIGHT) ) {
      world[ch->in_room].light += 10;
    }
    if ( affected_by_spell(ch, SPELL_DARKNESS) ) {
      world[ch->in_room].light -= 10;
    }

    if (GET_EQ(ch, WEAR_LIGHT))
      if (GET_OBJ_TYPE(GET_EQ(ch, WEAR_LIGHT)) == ITEM_LIGHT)
	if (GET_OBJ_VAL(GET_EQ(ch, WEAR_LIGHT), 2))	/* Light ON */
	  world[room].light++;
  }
}


/* give an object to a char   */
void obj_to_char(struct obj_data * object, struct char_data * ch)
{
  if (object && ch) {
    object->next_content = ch->carrying;
    ch->carrying = object;
    object->carried_by = ch;
    object->in_room = NOWHERE;
    IS_CARRYING_W(ch) += GET_OBJ_WEIGHT(object);
    IS_CARRYING_N(ch)++;

    /* set flag for crash-save system */
    SET_BIT(PLR_FLAGS(ch), PLR_CRASH);
  } else
    log("SYSERR: NULL obj or char passed to obj_to_char");
}


/* take an object from a char */
void obj_from_char(struct obj_data * object)
{
  struct obj_data *temp;

  if (object == NULL) {
    log("SYSERR: NULL object passed to obj_from_char");
    return;
  }
  REMOVE_FROM_LIST(object, object->carried_by->carrying, next_content); 

  /* set flag for crash-save system */
  SET_BIT(PLR_FLAGS(object->carried_by), PLR_CRASH); 

  IS_CARRYING_W(object->carried_by) -= GET_OBJ_WEIGHT(object);
  IS_CARRYING_N(object->carried_by)--;
  object->carried_by = NULL;
  object->next_content = NULL;
}



/* Return the effect of a piece of armor in position eq_pos */
int apply_ac(struct char_data * ch, int eq_pos)
{
  int armor;

  assert(GET_EQ(ch, eq_pos));

  if (!(GET_OBJ_TYPE(GET_EQ(ch, eq_pos)) == ITEM_ARMOR))
    return 0;

  armor = GET_OBJ_VAL(GET_EQ(ch, eq_pos), 0);

  switch (eq_pos)
  {
    case WEAR_BODY:
      armor *= 3;
      break;

    case WEAR_SHIELD:
      armor *= 2;
      break;

    case WEAR_ARMS:
    case WEAR_LEGS:
      armor *= 1.5;
      break;

    case WEAR_ANKLE_L:
    case WEAR_ANKLE_R:
    case WEAR_WRIST_L:
    case WEAR_WRIST_R:
      armor *= .5;
      break;

    case WEAR_FINGER_L:
    case WEAR_FINGER_R:
    case WEAR_NOSE:
      armor *= 0;
      break;
    default:
      break;

  }

  return armor;
}



void equip_char(struct char_data * ch, struct obj_data * obj, int pos)
{
  int j;
  int invalid_class(struct char_data *ch, struct obj_data *obj);
  int invalid_race(struct char_data *ch, struct obj_data *obj);

  assert(pos >= 0 && pos < NUM_WEARS);

  if (GET_EQ(ch, pos)) {
    GET_NAME(ch, chname);
    sprintf(buf, "SYSERR: Char is already equipped: %s, %s", chname,
	    obj->short_description);
    FREE_NAME(chname);
    log(buf);
    return;
  }

if (obj->carried_by) {
  log("SYSERR: EQUIP: Obj is carried_by when equip.");
    return;
  }

  if (obj->in_room != NOWHERE) {
    log("SYSERR: EQUIP: Obj is in_room when equip.");
    return;
  }

  /*  Let's change the messages here.  Soli, 8/12/99  */
  if ((IS_OBJ_STAT(obj, ITEM_ANTI_EVIL) && IS_EVIL(ch)) ||
      (IS_OBJ_STAT(obj, ITEM_ANTI_GOOD) && IS_GOOD(ch)) ||
      (IS_OBJ_STAT(obj, ITEM_ANTI_NEUTRAL) && IS_NEUTRAL(ch)))
  {
      act("You are zapped by $p and instantly let go of it.", FALSE, ch, obj, 0, TO_CHAR);
      act("$n is zapped by $p and instantly lets go of it.", FALSE, ch, obj, 0, TO_ROOM);
      obj_to_char(obj, ch);	/* changed to drop in inventory instead of
				 * ground */
      return;
  }

  /*  I'm moving the other two to a seperate function, checked before we
      actually wear the eq.  Soli, 8/12/99  */

  GET_EQ(ch, pos) = obj;
  obj->worn_by = ch;
  obj->worn_on = pos;

  if (GET_OBJ_TYPE(obj) == ITEM_ARMOR)
    GET_AC(ch) -= apply_ac(ch, pos);

  if (ch->in_room != NOWHERE) {
    if (pos == WEAR_LIGHT && GET_OBJ_TYPE(obj) == ITEM_LIGHT)
      if (GET_OBJ_VAL(obj, 2))	/* if light is ON */
	world[ch->in_room].light++;
  } else {
    log("SYSERR: ch->in_room = NOWHERE when equipping char.");
  }

  for (j = 0; j < MAX_OBJ_AFFECT; j++)
    affect_modify(ch, obj->affected[j].location,
		  obj->affected[j].modifier,
		  obj->obj_flags.bitvector, TRUE);
  affect_total(ch);
}



struct obj_data *unequip_char(struct char_data * ch, int pos)
{
  int j;
  struct obj_data *obj;

  assert(pos >= 0 && pos < NUM_WEARS);
  assert(GET_EQ(ch, pos));

  obj = GET_EQ(ch, pos);
  obj->worn_by = NULL;
  obj->worn_on = -1;

  if (GET_OBJ_TYPE(obj) == ITEM_ARMOR)
    GET_AC(ch) += apply_ac(ch, pos);

  if (ch->in_room != NOWHERE) {
    if (pos == WEAR_LIGHT && GET_OBJ_TYPE(obj) == ITEM_LIGHT)
      if (GET_OBJ_VAL(obj, 2))	/* if light is ON */
	world[ch->in_room].light--;
  } else {
    log("SYSERR: ch->in_room = NOWHERE when un-equipping char.");
  }

  GET_EQ(ch, pos) = NULL;

  for (j = 0; j < MAX_OBJ_AFFECT; j++)
    affect_modify(ch, obj->affected[j].location,
		  obj->affected[j].modifier,
		  obj->obj_flags.bitvector, FALSE);

  if (affected_by_spell(ch, SKILL_ENVENOM) && (pos == WEAR_WIELD))
  {
    send_to_char("As your weapon leaves your hand, the venom on it dissipates.\r\n", ch);
    affect_from_char(ch, SKILL_ENVENOM);
  }

  if (affected_by_spell(ch, SPELL_ADAMANT_MACE) && (pos == WEAR_WIELD))
    affect_from_char(ch, SPELL_ADAMANT_MACE);

  affect_total(ch);

  return (obj);
}


int get_number(char **name)
{
  int i;
  char *ppos;
  char number[MAX_INPUT_LENGTH];

  *number = '\0';

  if ((ppos = strchr(*name, '.'))) {
    *ppos++ = '\0';
    strcpy(number, *name);
    strcpy(*name, ppos);

    for (i = 0; *(number + i); i++)
      if (!isdigit(*(number + i)))
	return 0;

    return (atoi(number));
  }
  return 1;
}



/* Search a given list for an object number, and return a ptr to that obj */
struct obj_data *get_obj_in_list_num(int num, struct obj_data * list)
{
  struct obj_data *i;

  for (i = list; i; i = i->next_content)
    if (GET_OBJ_RNUM(i) == num)
      return i;

  return NULL;
}



/* search the entire world for an object number, and return a pointer  */
struct obj_data *get_obj_num(obj_rnum nr)
{
  struct obj_data *i;

  for (i = object_list; i; i = i->next)
    if (GET_OBJ_RNUM(i) == nr)
      return i;

  return NULL;
}

struct obj_data * get_obj_world_vnum(int vnum)
{
  struct obj_data *i;

  for (i = object_list; i; i = i->next)
    if (GET_OBJ_VNUM(i) == vnum)
      return i;

  return 0;
}



/* search a room for a char, and return a pointer if found..  */
struct char_data *get_char_room(char *name, room_rnum room)
{
  struct char_data *i;
  int j = 0, number;
  char tmpname[MAX_INPUT_LENGTH];
  char *tmp = tmpname;

  strcpy(tmp, name);
  if (!(number = get_number(&tmp)))
  {
    number = 1;
    tmp = name;
  }

  for (i = world[room].people; i && (j <= number); i = i->next_in_room)
    if (isname(tmp, i->player.name))
      if (++j == number)
	return i;

  return NULL;
}



/* search all over the world for a char num, and return a pointer if found */
struct char_data *get_char_num(mob_rnum nr)
{
  struct char_data *i;

  for (i = character_list; i; i = i->next)
    if (GET_MOB_RNUM(i) == nr)
      return i;

  return NULL;
}



/* put an object in a room */
void obj_to_room(struct obj_data * object, room_rnum room)
{
  if (!object || room < 0 || room > top_of_world)
    log("SYSERR: Illegal value(s) passed to obj_to_room");
  else {
    object->next_content = world[room].contents;
    world[room].contents = object;
    object->in_room = room;
    object->carried_by = NULL;
      if (ROOM_FLAGGED(room, ROOM_HOUSE))
      SET_BIT(ROOM_FLAGS(room), ROOM_HOUSE_CRASH); 
  }
}


/* Take an object from a room */
void obj_from_room(struct obj_data * object)
{
  struct obj_data *temp;

  if (!object || object->in_room == NOWHERE) {
    log("SYSERR: NULL object or obj not in a room passed to obj_from_room");
    return;
  }

  REMOVE_FROM_LIST(object, world[object->in_room].contents, next_content);

   if (ROOM_FLAGGED(object->in_room, ROOM_HOUSE))
    SET_BIT(ROOM_FLAGS(object->in_room), ROOM_HOUSE_CRASH); 
  object->in_room = NOWHERE;
  object->next_content = NULL;
}


/* put an object in an object (quaint)  */
void obj_to_obj(struct obj_data * obj, struct obj_data * obj_to)
{
  struct obj_data *tmp_obj;

  if (!obj || !obj_to || obj == obj_to) {
    log("SYSERR: NULL object or same source and target obj passed to obj_to_obj");
    return;
  }

  obj->next_content = obj_to->contains;
  obj_to->contains = obj;
  obj->in_obj = obj_to;

  for (tmp_obj = obj->in_obj; tmp_obj->in_obj; tmp_obj = tmp_obj->in_obj)
    GET_OBJ_WEIGHT(tmp_obj) += GET_OBJ_WEIGHT(obj);

  /* top level object.  Subtract weight from inventory if necessary. */
  GET_OBJ_WEIGHT(tmp_obj) += GET_OBJ_WEIGHT(obj);
  if (tmp_obj->carried_by)
    IS_CARRYING_W(tmp_obj->carried_by) += GET_OBJ_WEIGHT(obj);
}


/* remove an object from an object */
void obj_from_obj(struct obj_data * obj)
{
  struct obj_data *temp, *obj_from;

  if (obj->in_obj == NULL) {
    log("error (handler.c): trying to illegally extract obj from obj");
    return;
  }
  obj_from = obj->in_obj;
  REMOVE_FROM_LIST(obj, obj_from->contains, next_content);

  /* Subtract weight from containers container */
  for (temp = obj->in_obj; temp->in_obj; temp = temp->in_obj)
    GET_OBJ_WEIGHT(temp) -= GET_OBJ_WEIGHT(obj);

  /* Subtract weight from char that carries the object */
  GET_OBJ_WEIGHT(temp) -= GET_OBJ_WEIGHT(obj);
  if (temp->carried_by)
    IS_CARRYING_W(temp->carried_by) -= GET_OBJ_WEIGHT(obj);

  obj->in_obj = NULL;
  obj->next_content = NULL;
}


/* Set all carried_by to point to new owner */
void object_list_new_owner(struct obj_data * list, struct char_data * ch)
{
  if (list) {
    object_list_new_owner(list->contains, ch);
    object_list_new_owner(list->next_content, ch);
    list->carried_by = ch;
  }
}


/* Extract an object from the world */
void extract_obj(struct obj_data * obj)
{
  struct obj_data *temp;

  if (obj->worn_by != NULL)
    if (unequip_char(obj->worn_by, obj->worn_on) != obj)
      log("SYSERR: Inconsistent worn_by and worn_on pointers!!");
  if (obj->in_room != NOWHERE)
    obj_from_room(obj);
  else if (obj->carried_by)
    obj_from_char(obj);
  else if (obj->in_obj)
    obj_from_obj(obj);

  /* Get rid of the contents of the object, as well. */
  while (obj->contains)
    extract_obj(obj->contains);

  REMOVE_FROM_LIST(obj, object_list, next);

  if (GET_OBJ_RNUM(obj) >= 0)
    (obj_index[GET_OBJ_RNUM(obj)].number)--;

  if (SCRIPT(obj))
    extract_script(SCRIPT(obj));

  free_obj(obj);
}



void update_object(struct obj_data * obj, int use)
{
  if (GET_OBJ_TIMER(obj) > 0)
    GET_OBJ_TIMER(obj) -= use;
  if (obj->contains)
    update_object(obj->contains, use);
  if (obj->next_content)
    update_object(obj->next_content, use);
}


void update_char_objects(struct char_data * ch)
{
  int i;

  if (GET_EQ(ch, WEAR_LIGHT) != NULL)
    if (GET_OBJ_TYPE(GET_EQ(ch, WEAR_LIGHT)) == ITEM_LIGHT)
      if (GET_OBJ_VAL(GET_EQ(ch, WEAR_LIGHT), 2) > 0) {
	i = --GET_OBJ_VAL(GET_EQ(ch, WEAR_LIGHT), 2);
	if (i == 1) {
	  act("Your light begins to flicker and fade.", FALSE, ch, 0, 0, TO_CHAR);
	  act("$n's light begins to flicker and fade.", FALSE, ch, 0, 0, TO_ROOM);
	} else if (i == 0) {
	  act("Your light sputters out and dies.", FALSE, ch, 0, 0, TO_CHAR);
	  act("$n's light sputters out and dies.", FALSE, ch, 0, 0, TO_ROOM);
	  world[ch->in_room].light--;
	}
      }

  for (i = 0; i < NUM_WEARS; i++)
    if (GET_EQ(ch, i))
      update_object(GET_EQ(ch, i), 2);

  if (ch->carrying)
    update_object(ch->carrying, 1);
}



/* Extract a ch completely from the world, and leave his stuff behind */
void extract_char(struct char_data * ch)
{
  struct char_data *k, *temp;
  struct descriptor_data *t_desc;
  struct obj_data *obj;
  int i, freed = 0;
  //int j;

  extern struct char_data *combat_list;

  ACMD(do_return);

  void die_follower(struct char_data * ch);


  if (!IS_NPC(ch) && !ch->desc) {
    for (t_desc = descriptor_list; t_desc; t_desc = t_desc->next) {
      if (t_desc->original == ch) {
	do_return(t_desc->character, "", 0, 0);
      }
    }
  }

  if (ch->in_room == NOWHERE) {
    log("SYSERR: NOWHERE extracting char. (handler.c, extract_char)");
    exit(1);
  }

  if (ch->followers || ch->master) {
    die_follower(ch);
  }

  if (RIDING(ch) || RIDDEN_BY(ch))
   dismount_char(ch);

   REMOVE_BIT(PLR_FLAGS(ch), PLR_FISHING);

   REMOVE_BIT(PLR_FLAGS(ch), PLR_FISH_ON);

   REMOVE_BIT(PLR_FLAGS(ch), PLR_DIGGING);

   REMOVE_BIT(PLR_FLAGS(ch), PLR_DIG_ON);

   REMOVE_BIT(PLR_FLAGS(ch), PLR_FIRE_ON);

   REMOVE_BIT(PRF_FLAGS(ch), PRF_NOTSELF);

   //REMOVE_BIT(PRF_FLAGS(ch), PRF_DISGUISE);
   //REMOVE_BIT(PLR_FLAGS(ch), PLR_MAGE);
   //REMOVE_BIT(PLR_FLAGS(ch), PLR_MONK);
   //REMOVE_BIT(PLR_FLAGS(ch), PLR_KNIGHT);
   //REMOVE_BIT(PLR_FLAGS(ch), PLR_CLERIC);
   //REMOVE_BIT(PLR_FLAGS(ch), PLR_BARD);
   //REMOVE_BIT(PLR_FLAGS(ch), PLR_BEGGAR);
   REMOVE_BIT(PLR_FLAGS(ch), PLR_COURIER);
   REMOVE_BIT(PLR_FLAGS(ch), PLR_BEAR);
   REMOVE_BIT(PLR_FLAGS(ch), PLR_BIRD);
   REMOVE_BIT(PLR_FLAGS(ch), PLR_WOLF);
   REMOVE_BIT(PLR_FLAGS(ch), PLR_RABBIT);
   REMOVE_BIT(PLR_FLAGS(ch), PLR_CAT);

  /* Forget snooping, if applicable */
  if (ch->desc) {
    if (ch->desc->snooping) {
      ch->desc->snooping->snoop_by = NULL;
      ch->desc->snooping = NULL;
    }

    if (ch->desc->snoop_by) {
      SEND_TO_Q("Your victim is no longer among us.\r\n",
		ch->desc->snoop_by);
      ch->desc->snoop_by->snooping = NULL;
      ch->desc->snoop_by = NULL;
    }
  }

  /* transfer objects to room, if any */
  while (ch->carrying) {
    obj = ch->carrying;
    obj_from_char(obj);
    obj_to_room(obj, ch->in_room);
  }

  /* transfer equipment to room, if any */
  for (i = 0; i < NUM_WEARS; i++) {
    if (GET_EQ(ch, i)) {
      obj_to_room(unequip_char(ch, i), ch->in_room);
    }
  }

  if (FIGHTING(ch)) {
    stop_fighting(ch);
  }

  for (k = combat_list; k; k = temp) {
    temp = k->next_fighting;
    if (FIGHTING(k) == ch) {
      stop_fighting(k);
    }
  }

  char_from_room(ch);

  /* pull the char from the list */
  REMOVE_FROM_LIST(ch, character_list, next);

  if (ch->desc && ch->desc->original) {
    do_return(ch, NULL, 0, 0);
  }

  if (!IS_NPC(ch)) {
    save_char(ch, NOWHERE);
    Crash_delete_crashfile(ch);
  } else {
    if (GET_MOB_RNUM(ch) > -1) {     /* if mobile */
      mob_index[GET_MOB_RNUM(ch)].number--;
    }
    clearMemory(ch);		/* Only NPC's can have memory */

    if (SCRIPT(ch)) {
      extract_script(SCRIPT(ch));
    }

    free_char(ch);
    freed = 1;
  }

  if (!freed && ch->desc != NULL) {
    STATE(ch->desc) = CON_MENU;
    SEND_TO_Q(MENU, ch->desc);
  } else {  /* if a player gets purged from within the game */
    if (ch->master || ch->followers)
      die_follower(ch);
    if (!freed) {
      free_char(ch);
    }
  }
}



/* ***********************************************************************
* Here follows high-level versions of some earlier routines, ie functions*
* which incorporate the actual player-data                               *.
*********************************************************************** */


struct char_data *get_player_vis(struct char_data * ch, char *name, int inroom)
{
  struct char_data *i;

  for (i = character_list; i; i = i->next) {
   if (PRF_FLAGGED(i, PRF_NOTSELF) && !PRF_FLAGGED(ch, PRF_DETECT) && !IS_NPC(ch)) {
    if (!IS_NPC(i) && (!inroom || i->in_room == ch->in_room) &&
	!str_cmp(i->char_specials.name_dis, name) && CAN_SEE(ch, i))
      return i;
    }

else {
if (!IS_NPC(i) && (!inroom || i->in_room == ch->in_room) &&
    !str_cmp(i->player.name, name)) 

      return i;
}
}
  return NULL;
}

struct char_data *get_char_vis(struct char_data *ch, char *name, int where)
{
  struct char_data *i;
  int j = 0, number;
  char tmpname[MAX_INPUT_LENGTH];
  char *tmp = tmpname;

  /* check the room first */
  if (where == FIND_CHAR_ROOM)
    return get_char_room_vis(ch, name);
  else if (where == FIND_CHAR_WORLD) {
    if ((i = get_char_room_vis(ch, name)) != NULL)
      return (i);

    strcpy(tmp, name);
    if (!(number = get_number(&tmp)))
      return get_player_vis(ch, tmp, 0);

    for (i = character_list; i && (j <= number); i = i->next) {
 if (PRF_FLAGGED(i, PRF_NOTSELF) && !PRF_FLAGGED(ch, PRF_DETECT) && !IS_NPC(ch)) {
     if (isname(tmp, i->char_specials.name_dis) && CAN_SEE(ch, i))
        if (++j == number)
          return (i);
  }
  else if (!IS_APPROVED(i) && !IS_NPC(i)) {
      GET_NAME(i, chname);
        if ((isname(tmp, current_short_desc(i)) || isname(tmp, chname)) && CAN_SEE(ch, i))
          if (++j == number)
            return (i);
      FREE_NAME(chname);
  }

else {
      if (isname(tmp, i->player.name) && CAN_SEE(ch, i))
        if (++j == number)
          return (i);
  }
}
}

  return (NULL);
}

struct char_data *get_char_room_vis(struct char_data * ch, char *name)
{
  struct char_data *i;
  int j = 0, number;
  char tmpname[MAX_INPUT_LENGTH];
  char *tmp = tmpname;

  /* JE 7/18/94 :-) :-) */
  if (!str_cmp(name, "self") || !str_cmp(name, "me"))
    return ch;

  strcpy(tmp, name);
  if (!(number = get_number(&tmp)))
  {
    number = 1;
    tmp = name;
  }


  for (i = world[ch->in_room].people; i && j <= number; i = i->next_in_room) {
 if (PRF_FLAGGED(i, PRF_NOTSELF) && !PRF_FLAGGED(ch, PRF_DETECT) && !IS_NPC(ch)) {
    if (isname(tmp, i->char_specials.name_dis))
      if (CAN_SEE(ch, i))
	if (++j == number)
	  return i;
}
  else if (!IS_APPROVED(i) && !IS_NPC(i)) {
      GET_NAME(i, chname);
        if ((isname(tmp, current_short_desc(i)) || isname(tmp, chname)) && CAN_SEE(ch, i))
          if (++j == number)
            return (i);
      FREE_NAME(chname);
  }

else {
   if (isname(tmp, i->player.name))
      if (CAN_SEE(ch, i))
        if (++j == number)
          return i;
}
}

  return NULL;
}

#if 0
struct char_data *get_char_vis(struct char_data * ch, char *name)
{
  struct char_data *i;
  int j = 0, number;
  char tmpname[MAX_INPUT_LENGTH];
  char *tmp = tmpname;

  /* check the room first */
  if ((i = get_char_vis(ch, name, FIND_CHAR_ROOM)) != NULL)
    return i;

  strcpy(tmp, name);
  if (!(number = get_number(&tmp)))
  {
    number = 1;
    tmp = name;
  }


  for (i = character_list; i && (j <= number); i = i->next)
    if (isname(tmp, i->player.name) && CAN_SEE(ch, i))
      if (++j == number)
	return i;

  return NULL;
}
#endif


struct obj_data *get_obj_in_list_vis(struct char_data * ch, char *name,
				              struct obj_data * list)
{
  struct obj_data *i;
  int j = 0, number;
  char tmpname[MAX_INPUT_LENGTH];
  char *tmp = tmpname;

  strcpy(tmp, name);
  if (!(number = get_number(&tmp)))
  {
    number = 1;
    tmp = name;
  }


  for (i = list; i && (j <= number); i = i->next_content)
    if (isname(tmp, i->name))
      if (CAN_SEE_OBJ(ch, i))
	if (++j == number)
	  return i;

  return NULL;
}




/* search the entire world for an object, and return a pointer  */
struct obj_data *get_obj_vis(struct char_data * ch, char *name)
{
  struct obj_data *i;
  int j = 0, number;
  char tmpname[MAX_INPUT_LENGTH];
  char *tmp = tmpname;

  /* scan items carried */
  if ((i = get_obj_in_list_vis(ch, name, ch->carrying)))
    return i;

  /* scan room */
  if ((i = get_obj_in_list_vis(ch, name, world[ch->in_room].contents)))
    return i;

  strcpy(tmp, name);
  if (!(number = get_number(&tmp)))
  {
    number = 1;
    tmp = name;
  }

  /* ok.. no luck yet. scan the entire obj list   */
  for (i = object_list; i && (j <= number); i = i->next)
    if (isname(tmp, i->name))
      if (CAN_SEE_OBJ(ch, i))
	if (++j == number)
	  return i;

  return NULL;
}


struct obj_data *get_object_in_equip_vis(struct char_data * ch,
                           char *arg, struct obj_data * equipment[], int *j)
{

  int  number, tot = 0;
  char tmpname[MAX_INPUT_LENGTH];
  char *tmp = tmpname;

  strcpy(tmp, arg);
  if (!(number = get_number(&tmp)))
  {
    number = 1;
    tmp = arg;
  }

  for ((*j) = 0; (*j) < NUM_WEARS; (*j)++)
    if (equipment[(*j)])
      if (CAN_SEE_OBJ(ch, equipment[(*j)]) && !hidden_eq(ch, equipment[(*j)]))
        if (isname(tmp, equipment[(*j)]->name)) {
         tot ++;
          if (tot == number) {
          return (equipment[(*j)]);
           }
          }

  return NULL;
}



char *money_desc(int amount)
{
  static char buf[128];

  if (amount <= 0) {
    log("SYSERR: Try to create negative or 0 money.");
    return NULL;
  }

  if (amount == 1)
    strcpy(buf, "a coin");
  else if (amount <= 10)
    strcpy(buf, "a tiny pile of coins");
  else if (amount <= 20)
    strcpy(buf, "a handful of coins");
  else if (amount <= 75)
    strcpy(buf, "a little pile of coins");
  else if (amount <= 200)
    strcpy(buf, "a small pile of coins");
  else if (amount <= 1000)
    strcpy(buf, "a pile of coins");
  else if (amount <= 5000)
    strcpy(buf, "a big pile of coins");
  else if (amount <= 10000)
    strcpy(buf, "a large heap of coins");
  else if (amount <= 20000)
    strcpy(buf, "a huge mound of coins");
  else if (amount <= 75000)
    strcpy(buf, "an enormous mound of coins");
  else if (amount <= 150000)
    strcpy(buf, "a small mountain of coins");
  else if (amount <= 250000)
    strcpy(buf, "a mountain of coins");
  else if (amount <= 500000)
    strcpy(buf, "a huge mountain of coins");
  else if (amount <= 1000000)
    strcpy(buf, "an enormous mountain of coins");
  else
    strcpy(buf, "an absolutely colossal mountain of coins");

  return buf;
}


struct obj_data *create_money(struct money_data amount)
{
  struct obj_data *obj;
  struct extra_descr_data *new_descr;
  char buf[200];
  int coins;

  if ( money_to_copper(amount) <= 0) {
    log("SYSERR: Try to create negative or 0 money.");
    return NULL;
  }

  obj = create_obj();
  CREATE(new_descr, struct extra_descr_data, 1);

  coins = amount.platinum + amount.steel + amount.gold + amount.copper;

  if (coins == 1) {
    obj->name = str_dup("one coin");
    obj->short_description = str_dup("a coin");
    obj->description = str_dup("One miserable coin is lying here.");
    new_descr->keyword = str_dup("coin");
    new_descr->description = str_dup("It's just one miserable little coin.");
  } else {
    obj->name = str_dup("coins");
    obj->short_description = str_dup(money_desc(coins));
    sprintf(buf, "%s is lying here.", money_desc(coins));
    obj->description = str_dup(CAP(buf));

    new_descr->keyword = str_dup("coins");
    if (coins < 10) {
      sprintf(buf, "There are %d coins.", coins);
      new_descr->description = str_dup(buf);
    } else if (coins < 100) {
      sprintf(buf, "There are about %d coins.", 10 * (coins / 10));
      new_descr->description = str_dup(buf);
    } else if (coins < 1000) {
      sprintf(buf, "It looks to be about %d coins.", 100 * (coins / 100));
      new_descr->description = str_dup(buf);
    } else if (coins < 100000) {
      sprintf(buf, "You guess there are, maybe, %d coins.",
	      1000 * ((coins / 1000) + number(0, (coins / 1000))));
      new_descr->description = str_dup(buf);
    } else
      new_descr->description = str_dup("There are a LOT of coins.");
  }

  new_descr->next = NULL;
  obj->ex_description = new_descr;

  GET_OBJ_TYPE(obj) = ITEM_MONEY;
  GET_OBJ_WEAR(obj) = ITEM_WEAR_TAKE;
  GET_OBJ_VAL(obj, 0) = amount.platinum;
  GET_OBJ_VAL(obj, 1) = amount.steel;
  GET_OBJ_VAL(obj, 2) = amount.gold;
  GET_OBJ_VAL(obj, 3) = amount.copper;
  GET_OBJ_COST(obj) = money_to_copper(amount);
  obj->item_number = NOTHING;

  return obj;
}


/* Generic Find, designed to find any object/character                    */
/* Calling :                                                              */
/*  *arg     is the sting containing the string to be searched for.       */
/*           This string doesn't have to be a single word, the routine    */
/*           extracts the next word itself.                               */
/*  bitv..   All those bits that you want to "search through".            */
/*           Bit found will be result of the function                     */
/*  *ch      This is the person that is trying to "find"                  */
/*  **tar_ch Will be NULL if no character was found, otherwise points     */
/* **tar_obj Will be NULL if no object was found, otherwise points        */
/*                                                                        */
/* The routine returns a pointer to the next word in *arg (just like the  */
/* one_argument routine).                                                 */

int generic_find(char *arg, int bitvector, struct char_data * ch,
		     struct char_data ** tar_ch, struct obj_data ** tar_obj)
{
  int o;
  //int i, found;

  char name[256];

  *tar_ch = NULL;
  *tar_obj = NULL;

  one_argument(arg, name);

  if (!*name)
    return (0);

  if (IS_SET(bitvector, FIND_CHAR_ROOM)) {	/* Find person in room */
    if ((*tar_ch = get_char_vis(ch, name, FIND_CHAR_ROOM))) {
      return (FIND_CHAR_ROOM);
    }
  }
  if (IS_SET(bitvector, FIND_CHAR_WORLD)) {
    if ((*tar_ch = get_char_vis(ch, name, FIND_CHAR_WORLD))) {
      return (FIND_CHAR_WORLD);
    }
  }
  if (IS_SET(bitvector, FIND_OBJ_EQUIP)) {
    if ((*tar_obj = get_object_in_equip_vis(ch, name, ch->equipment, &o))) {
      return (FIND_OBJ_INV);
    }
  }


  if (IS_SET(bitvector, FIND_OBJ_INV)) {
    if ((*tar_obj = get_obj_in_list_vis(ch, name, ch->carrying))) {
      return (FIND_OBJ_INV);
    }
  }
  if (IS_SET(bitvector, FIND_OBJ_ROOM)) {
    if ((*tar_obj = get_obj_in_list_vis(ch, name, world[ch->in_room].contents))) {
      return (FIND_OBJ_ROOM);
    }
  }
  if (IS_SET(bitvector, FIND_OBJ_WORLD)) {
    if ((*tar_obj = get_obj_vis(ch, name))) {
      return (FIND_OBJ_WORLD);
    }
  }
  return (0);
}


/* a function to scan for "all" or "all.x" */
int find_all_dots(char *arg)
{
  if (!strcmp(arg, "all"))
    return FIND_ALL;
  else if (!strncmp(arg, "all.", 4)) {
    strcpy(arg, arg + 4);
    return FIND_ALLDOT;
  } else
    return FIND_INDIV;
}

/* dismount_char() / fr: Daniel Koepke (dkoepke@california.com)
 *   If a character is mounted on something, we dismount them.  If
 *   someone is mounting our character, then we dismount that someone.
 *   This is used for cleaning up after a mount is cancelled by
 *   something (either intentionally or by death, etc.)
 */
void dismount_char(struct char_data *ch) 
{
  if (RIDING(ch)) {
    RIDDEN_BY(RIDING(ch)) = NULL;
    RIDING(ch) = NULL;
    GET_POS(ch) = POS_STANDING;

  }
  
  if (RIDDEN_BY(ch)) {
    RIDING(RIDDEN_BY(ch)) = NULL;
    RIDDEN_BY(ch) = NULL;
    GET_POS(ch) = POS_STANDING;
  }
}


/* mount_char() / fr: Daniel Koepke (dkoepke@california.com)
 *   Sets _ch_ to mounting _mount_.  This does not make any checks
 *   what-so-ever to see if the _mount_ is mountable, etc.  That is
 *   left up to the calling function.  This does not present any
 *   messages, either.
 */
void mount_char(struct char_data *ch, struct char_data *mount) 
{
  RIDING(ch) = mount;
  RIDDEN_BY(mount) = ch;
}

