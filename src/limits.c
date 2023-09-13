/* ************************************************************************
*   File: limits.c                                      Part of CircleMUD *
*  Usage: limits & gain funcs for HMV, exp, hunger/thirst, idle time      *
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
#include "spells.h"
#include "comm.h"
#include "db.h"
#include "handler.h"
#include "interpreter.h"

extern struct char_data *character_list;
extern struct obj_data *object_list;
extern struct room_data *world;
extern int max_exp_gain;
extern int max_exp_loss;
extern int idle_rent_time;
extern int idle_max_level;
extern int idle_void;
extern int use_autowiz;
extern int min_wizlist_lev;
extern int free_rent;

extern int holiday_exp_modifier;

extern int has_lung(struct char_data *ch);
extern const char *pc_race_types[];
extern void reboot_wizlists(void);

int level_exp(int class, int level);
void race_affects(struct char_data *ch);

/* When age < 15 return the value p0 */
/* When age in 15..29 calculate the line between p1 & p2 */
/* When age in 30..44 calculate the line between p2 & p3 */
/* When age in 45..59 calculate the line between p3 & p4 */
/* When age in 60..79 calculate the line between p4 & p5 */
/* When age >= 80 return the value p6 */
int graf(int age, int p0, int p1, int p2, int p3, int p4, int p5, int p6)
{

  if (age < 15)
    return (p2);		/* < 15   */
  else if (age <= 29)
    return (p3);
//    return (int) (p1 + (((age - 15) * (p2 - p1)) / 15));	/* 15..29 */
  else if (age <= 44)
    return (p2);
//    return (int) (p2 + (((age - 30) * (p3 - p2)) / 15));	/* 30..44 */
  else if (age <= 59)
    return (p1);
//    return (int) (p3 + (((age - 45) * (p4 - p3)) / 15));	/* 45..59 */
  else if (age <= 79)
    return (p0);
//    return (int) (p4 + (((age - 60) * (p5 - p4)) / 20));	/* 60..79 */
  else
    return (p0);		/* >= 80 */
}


int hit_gain(struct char_data * ch)
/* Hitpoint gain pr. game hour */
{
  int gain;

  if (IS_NPC(ch)) {
    gain = GET_LEVEL(ch);
    /* Neat and fast */
  } else {

    gain = graf(age(ch)->year, 3, 5, 8, 10, 8, 5, 3);

    gain += GET_CON(ch);

    if (IS_SET(PRF_FLAGS(ch), PRF_BANDAGED))
      gain += 10;

    /* Class/Level calculations */

    /* Skill/Spell calculations */

    /* Position calculations    */

    switch (GET_POS(ch)) {
    case POS_SLEEPING:
      gain += GET_CON(ch);	/* Divide by 2 */
      break;
    case POS_RESTING:
    case POS_SITTING:
      gain += GET_CON(ch) / 2;	/* Divide by 4 */
      break;
    }

    if(IS_MONK(ch))
    {
      gain += (gain / 4);
    }

    if ( (GET_CLASS(ch) == CLASS_MAGE) ||
	 (GET_CLASS(ch) == CLASS_CLERIC) ||
	 (GET_CLASS(ch) == CLASS_DRUID) ) {
      gain /= 2;
    }

    if ((GET_COND(ch, FULL) == 0) || (GET_COND(ch, THIRST) == 0))
      gain /= 4;

  }

  if (IS_AFFECTED(ch, AFF_POISON))
    gain /= 4;

  return (gain);
}



int move_gain(struct char_data * ch)
/* move gain pr. game hour */
{
  int gain;

  if (IS_NPC(ch)) {
    return (GET_LEVEL(ch));
    /* Neat and fast */
  } else {
    gain = graf(age(ch)->year, 16, 20, 24, 20, 16, 12, 10);

    /* Class/Level calculations */

    /* Skill/Spell calculations */


    /* Position calculations    */
    switch (GET_POS(ch)) {
    case POS_SLEEPING:
      gain += (gain / 2);	/* Divide by 2 */
      break;
    case POS_RESTING:
      gain += (gain / 4);	/* Divide by 4 */
      break;
    case POS_SITTING:
      gain += (gain / 8);	/* Divide by 8 */
      break;
    }

    if ((GET_COND(ch, FULL) == 0) || (GET_COND(ch, THIRST) == 0))
      gain /= 4;
  }

  if (IS_AFFECTED(ch, AFF_POISON))
    gain /= 4;

  return (gain);
}



void set_title(struct char_data * ch, char *title)
{
  char tmpTitle[MAX_TITLE_LENGTH];

  if (title == NULL) {
    GET_NAME(ch, chname);
    if (strlen(chname) + strlen(pc_race_types[GET_RACE(ch)]) + 6 >
      MAX_TITLE_LENGTH)
    {
      log("SYSERR: Default title too long for %s the %s",
        chname, pc_race_types[GET_RACE(ch)]);
      exit(1);
    }
    else
    { 
      sprintf(tmpTitle, "%s the %s", chname, pc_race_types[GET_RACE(ch)]);
    }
    FREE_NAME(chname);
    title = tmpTitle;
  }

  if (strlen(title) > MAX_TITLE_LENGTH) {
    title[MAX_TITLE_LENGTH] = '\0';
  }

  if (GET_TITLE(ch) != NULL) {
    free(GET_TITLE(ch));
  }

  GET_TITLE(ch) = str_dup(title);
}


void check_autowiz(struct char_data * ch)
{
#ifndef CIRCLE_UNIX
  return;
#else
  char buf[128];

  if (use_autowiz && GET_LEVEL(ch) >= LVL_IMMORT) {
    sprintf(buf, "nice ../bin/autowiz %d %s %d %s %d &", min_wizlist_lev,
	    WIZLIST_FILE, LVL_IMMORT, IMMLIST_FILE, (int) getpid());
    mudlog("Initiating autowiz.", CMP, LVL_IMMORT, FALSE);
    system(buf);
    reboot_wizlists();
  }
#endif /* CIRCLE_UNIX */
}


/*  PDH  4/26/99
 *  requires N hours of play at level N before leveling to N+1
 */
int timeEnoughToLevel(struct char_data * ch)
{
  int hoursNeeded=0, lev;

  return TRUE;

  if ( GET_LEVEL(ch) < 5 ) {
    return TRUE;
  }

  for(lev=5; lev<=GET_LEVEL(ch); lev++) {
    hoursNeeded += lev;
  }

  if ( (ch->player.time.played / 3600) >= hoursNeeded ) {
    return TRUE;
  }

  /* {char tmp[200]; sprintf(tmp, "timeEnough: %s (lev %d) has %d hours -- needs %d", GET_NAME(ch), GET_LEVEL(ch), ch->player.time.played / 3600, hoursNeeded); log(tmp); } */

  return FALSE;
}


int levelCappedGain(struct char_data * ch)
{
  int curLevExp, nextLevExp;
  int level = GET_LEVEL(ch);


  curLevExp  = level_exp((int)GET_CLASS(ch), GET_LEVEL(ch));
  nextLevExp = level_exp((int)GET_CLASS(ch), GET_LEVEL(ch) + 1);

  if ( (level < 1) || (level >= LVL_IMMORT) ) {
    return 1;
  }

  return ( (nextLevExp - curLevExp) / 10 );
}


void gain_exp(struct char_data * ch, int gain)
{
  int is_altered = FALSE;
  int num_levels = 0;
  char buf[128];
  int orig_gain = 0;

  // Holiday Exp Bonus

    gain *= holiday_exp_modifier;
    gain /= 100;

  if (!IS_NPC(ch) && ((GET_LEVEL(ch) < 1 || GET_LEVEL(ch) >= LVL_IMMORT))) {
    return;
  }

  if (IS_NPC(ch)) {
    /*  PDH  4/ 1/99 - NPC no longer gain exp  */
    /* GET_EXP(ch) += gain; */
    return;
  }

  if (gain > 0) {
    /* put a cap on the max gain per kill */
    gain = MIN(max_exp_gain, gain);


    /*  PDH  5/14/99
     *  level based exp capping rather than group based level diffs
     */
    gain = MIN(gain, levelCappedGain(ch));


    /*  PDH  4/ 9/99
        RP experience factor   */
 /*  Okay, RP factor is checked here, like so...
    gain = (gain * GET_RPFACTOR(ch) ) / 100;
 The problem with this is it lessens the penalty - the character loses exp
at the same rate.  Let's make the exp loss go down less - Soli, 7/99   */
    orig_gain = gain;

    if (gain > 0)
        gain = (gain * GET_RPFACTOR(ch) ) / 100;
    else
        gain = (gain * (GET_RPFACTOR(ch) + 100) ) / 200;


    GET_EXP(ch) += gain;

	/* 
	      If the character is an evil monk, let's slap him/her with a penalty so they don't get to level 30 super fast.
	      Right now this penalty is a 25% decrease or 75% exp gain after all modifiers compared to any other class/align combination
           */
	
    while (GET_LEVEL(ch) < (LVL_IMMORT - 1)  && /* gotta be imm -1 */
	   GET_EXP(ch) >= level_exp(GET_CLASS(ch), GET_LEVEL(ch) + 1) &&
	   timeEnoughToLevel(ch) == TRUE ) {
      if (!IS_APPROVED(ch) && GET_LEVEL(ch) == 10)
        return;
      //GET_LEVEL(ch) += 1;
      ch->player.level += 1;
      num_levels++;
      advance_level(ch);
      is_altered = TRUE;
    }

    if (is_altered) {
      GET_NAME(ch, chname);
      sprintf(buf, "%s advanced %d level%s to level %d.",
        chname, num_levels, num_levels == 1 ? "" : "s",
        GET_LEVEL(ch));
      FREE_NAME(chname);
      mudlog(buf, BRF, MAX(LVL_IMMORT, GET_INVIS_LEV(ch)), TRUE);
      if (num_levels == 1) 
        send_to_char("You rise a level!\r\n", ch);
      else {
        sprintf(buf, "You rise %d levels!\r\n", num_levels);
        send_to_char(buf, ch);
      }
      
      check_autowiz(ch);
    }
  } else if (gain < 0) {
    gain = MAX(-max_exp_loss, gain);	/* Cap max exp lost per death */
    GET_EXP(ch) += gain;
    if (GET_EXP(ch) < 0)
      GET_EXP(ch) = 0;
  }
}


void gain_exp_regardless(struct char_data * ch, int gain)
{
  int is_altered = FALSE;
  int num_levels = 0;

  GET_EXP(ch) += gain;
  if (GET_EXP(ch) < 0)
    GET_EXP(ch) = 0;

  if (!IS_NPC(ch)) {
    while (GET_LEVEL(ch) < LVL_IMPL &&
	GET_EXP(ch) >= level_exp(GET_CLASS(ch), GET_LEVEL(ch) + 1)) {
      //GET_LEVEL(ch) += 1;
      ch->player.level += 1;
      num_levels++;
      advance_level(ch);
      is_altered = TRUE;
    }

    if (is_altered) {
      GET_NAME(ch, chname);
      sprintf(buf, "%s advanced %d level%s to level %d.",
        chname, num_levels, num_levels == 1 ? "" : "s", 
        GET_LEVEL(ch));
      FREE_NAME(chname);
      mudlog(buf, BRF, MAX(LVL_IMMORT, GET_INVIS_LEV(ch)), TRUE);
      if (num_levels == 1)
        send_to_char("You rise a level!\r\n", ch);
      else {
	sprintf(buf, "You rise %d levels!\r\n", num_levels);
	send_to_char(buf, ch);
      }

      check_autowiz(ch);
    }
  }
}


void gain_condition(struct char_data * ch, int condition, int value)
{
  int new_value = 0;
  int old_value = 0;
  bool intoxicated;

  if (GET_COND(ch, condition) == -1)	/* No change */
    return;

  intoxicated = (GET_COND(ch, DRUNK) > 0);
  old_value   = GET_COND(ch, condition);

  GET_COND(ch, condition) += value;

  GET_COND(ch, condition) = MAX(0, GET_COND(ch, condition));
  GET_COND(ch, condition) = MIN(24, GET_COND(ch, condition));

  if (PLR_FLAGGED(ch, PLR_WRITING))
    return;

  new_value = GET_COND(ch, condition);

  switch (condition)
  {
    case DRUNK:
    {
      if (new_value > 10)
        send_to_char("You are very intoxicated.\r\n", ch);
      else if (new_value > 7)
        send_to_char("You are intoxicated.\r\n", ch);
      else if (new_value > 5)
        send_to_char("You are beginning to feel intoxicated.\r\n", ch);
      else if (intoxicated)
        send_to_char("You are now sober.\r\n", ch);
      break;
    }

    case THIRST:
    {
      if ((new_value == 0) && (old_value == 0) && (GET_LEVEL(ch) > 5)) {
        send_to_char("You are extremely thirsty.\r\n", ch);
        if ((ch->desc) && (ch->in_room != 1) && 
          (GET_HIT(ch) > 1) &&
          (ch->char_specials.timer <= 8) &&
          (!(PLR_FLAGGED(ch, PLR_FROZEN))) &&
          (STATE(ch->desc) == CON_PLAYING))
          damage(ch, ch, number(1, 5), TYPE_MALNOURISHMENT);
      } else if (new_value == 0)
        send_to_char("You are very thirsty.\r\n", ch);
      else if (new_value < 3)
        send_to_char("You are thirsty.\r\n", ch);
      else if (new_value < 5)
        send_to_char("You are beginning to feel thirsty.\r\n", ch);
      break;
    }

    case FULL:
    {
      if ((new_value == 0) && (old_value == 0) && (GET_LEVEL(ch) > 5)) {
        send_to_char("You are extremely hungry.\r\n", ch);
        if ((ch->desc) && (ch->in_room != 1) && 
          (GET_HIT(ch) > 1) &&
          (ch->char_specials.timer <= 8) &&
          (!(PLR_FLAGGED(ch, PLR_FROZEN))) &&
          (STATE(ch->desc) == CON_PLAYING))
          damage(ch, ch, number(1, 5), TYPE_MALNOURISHMENT);
      } else if (new_value == 0)
        send_to_char("You are very hungry.\r\n", ch);
      else if (new_value < 3)
        send_to_char("You are hungry.\r\n", ch);
      else if (new_value < 5)
        send_to_char("You are beginning to feel hungry.\r\n", ch);
      break;
    }

    default:
    {
      break;
    }
  }
}

void check_idling(struct char_data * ch)
{
  extern int free_rent;
  void Crash_rentsave(struct char_data *ch, int cost);

  if((++(ch->char_specials.timer) > 8) && (GET_LEVEL(ch) < 32))
  {
    if (GET_WAS_IN(ch) == NOWHERE && ch->in_room != NOWHERE) {
      GET_WAS_IN(ch) = ch->in_room;

      if (FIGHTING(ch)) {
	stop_fighting(FIGHTING(ch));
	stop_fighting(ch);
      }

      act("$n disappears into the void.", TRUE, ch, 0, 0, TO_ROOM);
      send_to_char("You have been idle, and are pulled into a void.\r\n", ch);
      save_char(ch, NOWHERE);
      Crash_crashsave(ch, FALSE);
      char_from_room(ch);
      char_to_room(ch, 1);

    } else if((ch->char_specials.timer > 48) && (GET_LEVEL(ch) < 32)) {

      if (ch->in_room != NOWHERE) {
	char_from_room(ch);
      }

      char_to_room(ch, 3);

      if (ch->desc) {
        STATE(ch->desc) = CON_DISCONNECT;
        /*
         * For the 'if (d->character)' test in close_socket().
         * -gg 3/1/98 (Happy anniversary.)
         */
        ch->desc->character = NULL;
        ch->desc = NULL;
      }

      if (free_rent) {
	Crash_rentsave(ch, 0);
      } else {
	Crash_idlesave(ch);
      }

      GET_NAME(ch, chname);
      sprintf(buf, "%s force-rented and extracted (idle).", chname);
      FREE_NAME(chname);
      mudlog(buf, CMP, LVL_GOD, TRUE);
      extract_char(ch);
    }
  }
}



/* Update PCs, NPCs, and objects */
void point_update(void)
{
  void update_char_objects(struct char_data * ch);      /* handler.c */
  void extract_obj(struct obj_data * obj);              /* handler.c */
  void update_mem(struct char_data * ch);      /* spell_parser.c - Soli, 8/99 */
  void update_skills(struct char_data * ch);   /* spell_parser - Soli, 10/99  */
  void assert_slots(struct char_data * ch);    /* spell_parser - Soli, 11/99  */
  struct char_data *i, *next_char;
  struct obj_data *j, *next_thing, *jj, *next_thing2;

  sh_int metab;

  /* characters */
  for (i=character_list; i; i=next_char) {
    next_char = i->next;

    /*  PDH  4/12/99
     *  check/remove PLR_KILLER flag if > 15 minutes since last pkillAttempt
     */
    if ( PLR_FLAGGED(i, PLR_KILLER) ) {
      if ( ( time(0) - i->player.time.pkillAttempt ) >= 900 ) {
	char tmp[200];

	i->player.time.pkillAttempt = 0;
	REMOVE_BIT(PLR_FLAGS(i), PLR_KILLER);
        GET_NAME(i, chname);
	sprintf(tmp, "Removing PLR_KILLER flag from %s - 15 minutes over.",
		chname);
        FREE_NAME(chname);
	mudlog(tmp, NRM, LVL_LESSERGOD, TRUE);
      }
    }

    if (PLR_FLAGGED(i, PLR_THIEF))
    {
      if ((time(0) - i->player.time.stealAttempt) >= 900)
      {
        char tmp2[200];

        i->player.time.stealAttempt = 0;
        REMOVE_BIT(PLR_FLAGS(i), PLR_THIEF);
        GET_NAME(i, chname);
        sprintf(tmp2, "Removing PLR_THIEF flag from %s - 15 minutes over.",
          chname);
        FREE_NAME(chname);
        mudlog(tmp2, NRM, LVL_LESSERGOD, TRUE);
      }
    }

   metab = 4;   /* 3 is the default. Lower means they use less food, */
                /* higher means more. The bonuses even stack. */


   race_affects(i);   

   if(IS_KENDER(i))
     metab++;
   if(IS_MINOTAUR(i))
     metab++;
   if(IS_WARRIOR(i))
     metab++;
   if(IS_MONK(i))
     metab--;
   if(IS_RANGER(i))
     metab--;
   if(IS_SILVANESTI(i))
     metab--;
   if(IS_QUALINESTI(i))
     metab--;
   if(IS_KAGONESTI(i))
     metab--;

   if(metab < 2)
     metab = 2;

   TIME_ONLINE(i) += 1;

   if(number(0, metab) > 1)
   {
      gain_condition(i, FULL, -1);
      gain_condition(i, DRUNK, -1);
      gain_condition(i, THIRST, -1);
   }

    if (GET_POS(i) >= POS_STUNNED) {
      GET_HIT(i) = MIN(GET_HIT(i) + hit_gain(i), GET_MAX_HIT(i));
      GET_MOVE(i) = MIN(GET_MOVE(i) + move_gain(i), GET_MAX_MOVE(i));

      if (GET_HIT(i) >= GET_MAX_HIT(i))
        if (IS_SET(PRF_FLAGS(i), PRF_BANDAGED | PRF_NOBANDAGE))
          REMOVE_BIT(PRF_FLAGS(i), PRF_BANDAGED | PRF_NOBANDAGE);

      if (IS_AFFECTED(i, AFF_POISON)) {
	damage(i, i, 2, SPELL_POISON);
      }

      if (GET_POS(i) <= POS_STUNNED) {
	update_pos(i);
      }

    } else if (GET_POS(i) == POS_INCAP && !IS_NPC(i)) {
      damage(i, i, 1, TYPE_SUFFERING);

    } else if (GET_POS(i) == POS_MORTALLYW && !IS_NPC(i)) {
      damage(i, i, 2, TYPE_SUFFERING);
    }

    if ( ! IS_NPC(i)) {
      update_char_objects(i);

   if (GET_LEVEL(i) < LVL_IMMORT) {
	if (SECT(i->in_room) == SECT_WATER_NOSWIM && !(IS_AFFECTED(i, AFF_POISON))) {
	  act("$n thrashes about in the water straining to stay afloat.",
	      FALSE, i, 0, 0, TO_ROOM);
	  send_to_char("You are drowning!\r\n", i);
	  damage(i, i, GET_MAX_HIT(i)/5, TYPE_DROWNING); /* TYPE_DROWNING ? */
	}
      }

      if (GET_LEVEL(i) < LVL_IMMORT) {
	if (SECT(i->in_room) == SECT_UNDERWATER && !has_lung(i) && !(IS_AFFECTED(i, AFF_POISON))) {
	  act("$n thrashes about in the water gasping for breath.",
	      FALSE, i, 0, 0, TO_ROOM);
	  send_to_char("You are drowning!\r\n", i);
	  damage(i, i, GET_MAX_HIT(i)/5, TYPE_DROWNING); 
	}
      }

      assert_slots(i);

#if 0
      if ((GET_POS(i) != POS_RESTING) && (GET_POS(i) != POS_SITTING))
        i->standcounter = 0;
      else if (!i->standcounter)
        i->standcounter = 1;
      else
      {
        update_mem(i);
        i->standcounter = 2;
      }
#else
      if ((GET_POS(i) == POS_RESTING) || (GET_POS(i) == POS_SITTING) ||
         (GET_POS(i) == POS_STANDING))
      {
         if (GET_POS(i) == POS_STANDING)
         {
            i->standcounter = 0;
         }
         else
         {
            if (!i->standcounter)
               i->standcounter = 1;
            else
               i->standcounter = 2;
         }
         update_mem(i);
      }
      else
      {
         i->standcounter = 0;
      }
#endif

      update_skills(i);
      /*  PDH  4/ 8/99
       *  force rent idle (linkdead) people were crashing MUD
       */
//      if (GET_LEVEL(i) < LVL_DEMIGOD) {
	check_idling(i);
//      }

    }

  }

  /* objects */
  for (j=object_list; j; j=next_thing) {
    next_thing = j->next;      /* Next in object list */

    /* If this object is in water. */
    if (j->in_room != NOWHERE &&
	(SECT(j->in_room) == SECT_WATER_NOSWIM ||
	 SECT(j->in_room) == SECT_WATER_SWIM ||
	 SECT(j->in_room) == SECT_UNDERWATER)) {
      if ((GET_OBJ_TYPE(j) != ITEM_CONTAINER) && GET_OBJ_VAL(j, 3)) {
	/* Give everything a random chance of sinking, some may never. */
	if (number(0, GET_OBJ_WEIGHT(j)) > 0) {
	  act("$p sinks into the murky depths.", FALSE, 0, j, 0, TO_ROOM);
	  extract_obj(j);
	  continue;
	} 
      } else {
	act("$p floats unsteadily in the area.", FALSE, 0, j, 0, TO_ROOM);
      }
    }

    /* If this is a corpse */
    if (IS_CORPSE(j)) {
      /* timer count down */
      if (GET_OBJ_TIMER(j) > 0) {
	GET_OBJ_TIMER(j)--;
      }

      if (!GET_OBJ_TIMER(j)) {
	if (j->carried_by) {
	  act("$p decays in your hands.", FALSE, j->carried_by, j, 0, TO_CHAR);
	} else if ((j->in_room != NOWHERE) && (world[j->in_room].people)) {
	  act("A quivering horde of maggots consumes $p.",
	      TRUE, world[j->in_room].people, j, 0, TO_ROOM);
	  act("A quivering horde of maggots consumes $p.",
	      TRUE, world[j->in_room].people, j, 0, TO_CHAR);
	}

	for (jj = j->contains; jj; jj = next_thing2) {
	  next_thing2 = jj->next_content;	/* Next in inventory */
	  obj_from_obj(jj);

	  if (j->in_obj) {
	    obj_to_obj(jj, j->in_obj);
	  } else if (j->carried_by) {
	    obj_to_room(jj, j->carried_by->in_room);
	  } else if (j->in_room != NOWHERE) {
	    obj_to_room(jj, j->in_room);
	  } else {
	    assert(FALSE);
	  }
	}
	extract_obj(j);
	continue;
      }
    }

  /* If this is a potion/scroll 
    if (j->creation == 1) {
    if ((j->in_room != NOWHERE) && (world[j->in_room].people)) {
       if (GET_OBJ_TIMER(j) > 0) {
        GET_OBJ_TIMER(j)--;
      }
    }

      if (!GET_OBJ_TIMER(j)) {
          act("$p slowly decays .... and dissapear into thin air!",
              TRUE, world[j->in_room].people, j, 0, TO_ROOM);
          act("$p slowly decays ... and dissapear into thin air!",
              TRUE, world[j->in_room].people, j, 0, TO_CHAR);
        extract_obj(j);
        continue;
      }
    } */



  /* If this is food */
    if (GET_OBJ_TYPE(j) == ITEM_FOOD) {
      /* timer count down */
      if (GET_OBJ_TIMER(j) > 0) {
        GET_OBJ_TIMER(j)--;
      }

      if (!GET_OBJ_TIMER(j)) {
        if (j->carried_by) {
          act("$p decays in your hands.", FALSE, j->carried_by, j, 0, TO_CHAR);
        } else if ((j->in_room != NOWHERE) && (world[j->in_room].people)) {
          act("A quivering horde of maggots consumes $p.",
              TRUE, world[j->in_room].people, j, 0, TO_ROOM);
          act("A quivering horde of maggots consumes $p.",
              TRUE, world[j->in_room].people, j, 0, TO_CHAR);
        }

        for (jj = j->contains; jj; jj = next_thing2) {
          next_thing2 = jj->next_content;       /* Next in inventory */
          obj_from_obj(jj);

          if (j->in_obj) {
            obj_to_obj(jj, j->in_obj);
          } else if (j->carried_by) {
            obj_to_room(jj, j->carried_by->in_room);
          } else if (j->in_room != NOWHERE) {
            obj_to_room(jj, j->in_room);
          } else {
            assert(FALSE);
          }
        }
        extract_obj(j);
        continue;
      }
    }


    /* *** Portal addition *** */
    if (GET_OBJ_TYPE(j) == ITEM_PORTAL) {
      if (GET_OBJ_TIMER(j) > 0) {
	GET_OBJ_TIMER(j)--;
      }

      if (!GET_OBJ_TIMER(j)) {
	act("A glowing portal fades from existance.",
	    TRUE, world[j->in_room].people, j, 0, TO_ROOM);
	act("A glowing portal fades from existance.",
	    TRUE, world[j->in_room].people, j, 0, TO_CHAR);
	extract_obj(j);
	continue;
      }
    }

    /* *** Build fire  *** */
    if (GET_OBJ_RNUM(j) == real_object(16)) {
      if (GET_OBJ_TIMER(j) > 0) {
        GET_OBJ_TIMER(j)--;
      }

      if (GET_OBJ_TIMER(j) == 1) {
        if ((j->in_room != NOWHERE) &&(world[j->in_room].people)) {
          act("$p starts to die down.",
              FALSE, world[j->in_room].people, j, 0, TO_ROOM);
          act("$p starts to die down.",
              FALSE, world[j->in_room].people, j, 0, TO_CHAR);
        }
      }

      if (GET_OBJ_TIMER(j) == 0) {
        if ((j->in_room != NOWHERE) &&(world[j->in_room].people)) {
          act("$p has stopped burning!",
              FALSE, world[j->in_room].people, j, 0, TO_ROOM);
          act("$p has stopped burning!",
              FALSE, world[j->in_room].people, j, 0, TO_CHAR);
        }
        extract_obj(j);
        continue;
      }
    }



    /* *** Create Spring spell *** */
    if (GET_OBJ_RNUM(j) == real_object(15)) {
      if (GET_OBJ_TIMER(j) > 0) {
	GET_OBJ_TIMER(j)--;
      }

      if (GET_OBJ_TIMER(j) == 1) {
	if ((j->in_room != NOWHERE) &&(world[j->in_room].people)) {
	  act("$p starts to slow down.",
	      FALSE, world[j->in_room].people, j, 0, TO_ROOM);
	  act("$p starts to slow down.",
	      FALSE, world[j->in_room].people, j, 0, TO_CHAR);
	}
      }

      if (GET_OBJ_TIMER(j) == 0) {
	if ((j->in_room != NOWHERE) &&(world[j->in_room].people)) {
	  act("$p has stopped flowing!",
	      FALSE, world[j->in_room].people, j, 0, TO_ROOM);
	  act("$p has stopped flowing!",
	      FALSE, world[j->in_room].people, j, 0, TO_CHAR);
	}
	extract_obj(j);
	continue;
      }
    }

  }
}

void ten_second_update(void)
{

  struct char_data *i, *next_char;

  //sh_int metab;

  /* characters */
  for (i=character_list; i; i=next_char) {
    next_char = i->next;

    if (i->desc && i->desc->marked && i->desc->marked->in_room != i->in_room ) {
      act("You lose your mark on $N.", FALSE, i, 0, i->desc->marked, TO_CHAR);
      i->desc->marked = NULL;
      i->desc->marked_time = 0;
    }


   if (i->desc && i->desc->marked && i->desc->marked->in_room == i->in_room ) {
     if (check_skill(i, 101, SKILL_MARK) && check_skill(i, 101, SKILL_HIDE))
       i->desc->marked_time += 1;
     else {
       act("You lose your mark on $N.", FALSE, i, 0, i->desc->marked, TO_CHAR);
       i->desc->marked_time = 0;
       i->desc->marked = NULL;
     }
   }

   if (i->desc && i->desc->marked && i->desc->marked->in_room == i->in_room && i->desc->marked_time >= 3) {
    act("You have $N marked for assassination now.",  FALSE, i, 0, i->desc->marked, TO_CHAR);
    return;
   }

  }


}
