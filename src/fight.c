/* ************************************************************************
*   File: fight.c                                       Part of CircleMUD *
*  Usage: Combat system                                                   *
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
#include "handler.h"
#include "interpreter.h"
#include "db.h"
#include "spells.h"
#include "screen.h"
#include "constants.h"
#include "dg_scripts.h"

/* Structures */
struct char_data *combat_list = NULL;	/* head of l-list of fighting chars */
struct char_data *next_combat_list = NULL;

/* External structures */
extern struct index_data *mob_index;
extern struct room_data *world;
extern struct message_list fight_messages[MAX_MESSAGES];
extern struct obj_data *object_list;
extern int pk_allowed;		/* see config.c */
extern int max_exp_gain;	/* see config.c */
extern int max_exp_loss;	/* see config.c */
extern int xap_objs;
extern int holiday_exp_modifier;

extern struct char_data *character_list;
extern struct area_aff_data *area_spells_list;
extern int auto_save;		/* see config.c */
extern void mob_casting_acts(struct char_data * ch);
extern int get_element(struct char_data * ch, int type);
extern int protect_from_element(struct char_data * ch, int element);
extern void unaffect_one(struct char_data * ch, int type);
extern void affect_update_pulsebased(struct char_data * ch);
void weather_change(void);
void assign_skin_value(struct char_data *ch, struct obj_data *corpse);

/* External procedures */
char *fread_action(FILE * fl, int nr);
char *fread_string(FILE * fl, const char *error);
void stop_follower(struct char_data * ch);
ACMD(do_flee);
void hit(struct char_data * ch, struct char_data * victim, int type);
void forget(struct char_data * ch, struct char_data * victim);
void remember(struct char_data * ch, struct char_data * victim);
int ok_damage_shopkeeper(struct char_data * ch, struct char_data * victim);
int mag_savingthrow(struct char_data * ch, int type);
void clearMemory(struct char_data * ch);
void affect_from_char_II(struct char_data * ch, int skill, int type, int action);

// local procedures

int calc_weapon_damage(struct obj_data *wielded, struct char_data *ch);
int calc_ac(struct char_data *victim, struct char_data *ch);
int calc_thaco(struct char_data *ch);

int dam_poly[30][2] = {
 {0, 0}, {1, 6}, {1, 6}, {1, 8}, {2, 5}, {2, 5},
 {2, 6}, {2, 6}, {2, 7}, {2, 7}, {2, 8}, {2, 8},
 {2, 9}, {2, 9}, {2, 9}, {3, 6}, {3, 6}, {3, 6},
 {3, 6}, {3, 7}, {3, 8}, {3, 8}, {3, 8}, {4, 6},
 {4, 6}, {4, 7}, {4, 7}, {4, 7}, {4, 7}, {4, 8},
};

/* Weapon attack texts */
struct attack_hit_type attack_hit_text[] =
{
  {"hit", "hits"},		/* 0 */
  {"sting", "stings"},
  {"whip", "whips"},
  {"slash", "slashes"},
  {"bite", "bites"},
  {"bludgeon", "bludgeons"},	/* 5 */
  {"crush", "crushes"},
  {"pound", "pounds"},
  {"claw", "claws"},
  {"maul", "mauls"},
  {"thrash", "thrashes"},	/* 10 */
  {"pierce", "pierces"},
  {"blast", "blasts"},
  {"punch", "punches"},
  {"stab", "stabs"},
  {"spear", "spears"},
  {"cleave", "cleaves"}
};

#define IS_WEAPON(type) (((type) >= TYPE_HIT) && ((type) < MAX_WEAPON_DAMAGE))

/* The Fight related routines */

void increase_blood(int rm)
{
  RM_BLOOD(rm) = MIN(RM_BLOOD(rm) + 1, 20);
}

void appear(struct char_data * ch)
{
  if (affected_by_spell(ch, SPELL_INVISIBILITY))
    affect_from_char(ch, SPELL_INVISIBILITY);

  REMOVE_BIT(AFF_FLAGS(ch), AFF_INVISIBLE | AFF_HIDE | AFF_SNEAK);

  if (GET_LEVEL(ch) < LVL_IMMORT)
    act("$n slowly fades into existence.", FALSE, ch, 0, 0, TO_ROOM);
  else
    act("You feel a strange presence as $n appears, seemingly from nowhere.",
	FALSE, ch, 0, 0, TO_ROOM);
}



void load_messages(void)
{
  FILE *fl;
  int i, type;
  struct message_type *messages;
  char chk[128];

  if (!(fl = fopen(MESS_FILE, "r"))) {
    log("SYSERR: Error reading combat message file %s: %s", MESS_FILE,
      strerror(errno));
    exit(1);
  }

  for (i = 0; i < MAX_MESSAGES; i++) {
    fight_messages[i].a_type = 0;
    fight_messages[i].number_of_attacks = 0;
    fight_messages[i].msg = 0;
  }


  fgets(chk, 128, fl);
  while (!feof(fl) && (*chk == '\n' || *chk == '*'))
    fgets(chk, 128, fl);

  while (*chk == 'M') {
    fgets(chk, 128, fl);
    sscanf(chk, " %d\n", &type);
    for (i = 0; (i < MAX_MESSAGES) && (fight_messages[i].a_type != type) &&
	 (fight_messages[i].a_type); i++);

    if (i >= MAX_MESSAGES) {
      log("SYSERR: Too many combat messages.  Increase MAX_MESSAGES and recompile.");
      exit(1);
    }

    CREATE(messages, struct message_type, 1);
    fight_messages[i].number_of_attacks++;
    fight_messages[i].a_type = type;
    messages->next = fight_messages[i].msg;
    fight_messages[i].msg = messages;

    messages->die_msg.attacker_msg = fread_action(fl, i);
    messages->die_msg.victim_msg = fread_action(fl, i);
    messages->die_msg.room_msg = fread_action(fl, i);
    messages->miss_msg.attacker_msg = fread_action(fl, i);
    messages->miss_msg.victim_msg = fread_action(fl, i);
    messages->miss_msg.room_msg = fread_action(fl, i);
    messages->hit_msg.attacker_msg = fread_action(fl, i);
    messages->hit_msg.victim_msg = fread_action(fl, i);
    messages->hit_msg.room_msg = fread_action(fl, i);
    messages->god_msg.attacker_msg = fread_action(fl, i);
    messages->god_msg.victim_msg = fread_action(fl, i);
    messages->god_msg.room_msg = fread_action(fl, i);
    fgets(chk, 128, fl);
    while (!feof(fl) && (*chk == '\n' || *chk == '*'))
      fgets(chk, 128, fl);
  }

  fclose(fl);
}


void update_pos(struct char_data * victim)
{
  if ((GET_HIT(victim) > 0) && (GET_POS(victim) > POS_STUNNED))
    return;
  else if (GET_HIT(victim) > 0)
  {
    if (affected_by_spell(victim, SPELL_CRIPPLE))
      GET_POS(victim) = POS_RESTING;
    else
      GET_POS(victim) = POS_STANDING;
  }
  else if (GET_HIT(victim) <= -11)
    GET_POS(victim) = POS_DEAD;
  else if (GET_HIT(victim) <= -6)
    GET_POS(victim) = POS_MORTALLYW;
  else if (GET_HIT(victim) <= -3)
    GET_POS(victim) = POS_INCAP;
  else
    GET_POS(victim) = POS_STUNNED;
}

void weapon_spells(struct char_data *ch, struct char_data *vict, struct obj_data *wpn)
{
  int i=0, rndm;

  if(wpn&&HAS_SPELLS(wpn)) {
    while(GET_WEAPON_SPELL(wpn,i)&&i<MAX_SPELL_AFFECT) {

      /* Do some sanity checking, in case someone flees, etc. */
      /* Can't have enough of 'em.  */

      if (ch->in_room != vict->in_room) {
        if (FIGHTING(ch) && FIGHTING(ch) == vict)
          stop_fighting(ch);
        return;
      }

      rndm=number(1,100);
      if(GET_WEAPON_SPELL_PCT(wpn,i)>=rndm) {
        act("$p leaps to action with an attack of its own.",TRUE,ch,wpn,0,TO_CHAR);
        act("$p leaps to action with an attack of its own.",TRUE,ch,wpn,0,TO_ROOM);
        if(call_magic(ch,vict,NULL,GET_WEAPON_SPELL(wpn,i),GET_WEAPON_SPELL_LVL(wpn,i),CAST_WAND)<0) return;
      }
      i++;
    }
  }
}

void check_killer(struct char_data * ch, struct char_data * vict)
{
  char buf[256];

  if (PLR_FLAGGED(vict, PLR_KILLER) || PLR_FLAGGED(vict, PLR_THIEF))
    return;

  if (PLR_FLAGGED(ch, PLR_KILLER) || IS_NPC(ch) || IS_NPC(vict) || ch == vict)
    return;

  SET_BIT(PLR_FLAGS(ch), PLR_KILLER);
  ch->player.time.pkillAttempt = time(0);

  GET_NAME(ch, chname);
  GET_NAME(vict, victname);
  sprintf(buf, "PC Killer bit set on %s for initiating attack on %s at %s.",
    chname, victname, world[vict->in_room].name);
  FREE_NAME(victname);
  FREE_NAME(chname);
  mudlog(buf, BRF, LVL_LESSERGOD, TRUE);
  send_to_char("If you want to kill other mortals, so be it...\r\n", ch);
}


/* start one char fighting another (yes, it is horrible, I know... )  */
void set_fighting(struct char_data * ch, struct char_data * vict)
{
  if (ch == vict)
    return;

  if (FIGHTING(ch)) {
    core_dump();
    return;
  }

  ch->next_fighting = combat_list;
  combat_list = ch;

  if (IS_AFFECTED(ch, AFF_SLEEP))
    affect_from_char(ch, SPELL_SLEEP);

  if (IS_AFFECTED(ch, AFF_STANCE))
    affect_from_char_II(ch, SKILL_STANCE, SPELL_DONTUSEME, 2);


  FIGHTING(ch) = vict;

  if (!affected_by_spell(ch, SPELL_CRIPPLE))
    GET_POS(ch) = POS_FIGHTING;

  if (!pk_allowed)
    check_killer(ch, vict);
}



/* remove a char from the list of fighting chars */
void stop_fighting(struct char_data * ch)
{
  struct char_data *temp;

  if (ch == next_combat_list)
    next_combat_list = ch->next_fighting;

  REMOVE_FROM_LIST(ch, combat_list, next_fighting);
  ch->next_fighting = NULL;
  FIGHTING(ch) = NULL;

  if (!affected_by_spell(ch, SPELL_CRIPPLE))
    GET_POS(ch) = POS_STANDING;
  if (AFF_FLAGGED(ch, AFF_STANCE)) {
   affect_from_char(ch, SPELL_DONTUSEME);
  }
  else
    GET_POS(ch) = POS_STANDING;
  update_pos(ch);
}



void make_corpse(struct char_data * ch)
{
  struct obj_data *corpse, *o;
  struct obj_data *money;
  int i;
  extern int max_npc_corpse_time, max_pc_corpse_time;

  struct obj_data *create_money(struct money_data amount);

  corpse = create_obj();

  corpse->item_number = NOTHING;
  corpse->in_room = NOWHERE;
  corpse->name = str_dup("corpse");

  sprintf(buf2, "The corpse of %s is lying here.", ch->player.short_descr);
  corpse->description = str_dup(buf2);

  sprintf(buf2, "the corpse of %s", ch->player.short_descr);
  corpse->short_description = str_dup(buf2);

  GET_OBJ_TYPE(corpse) = ITEM_CONTAINER;
  GET_OBJ_WEAR(corpse) = ITEM_WEAR_TAKE;
  GET_OBJ_EXTRA(corpse) = 0;
  GET_OBJ_VAL(corpse, 0) = 0;	/* You can't store stuff in a corpse */
  GET_OBJ_VAL(corpse, 3) = 1;	/* corpse identifier */
  GET_OBJ_WEIGHT(corpse) = GET_WEIGHT(ch) + IS_CARRYING_W(ch);
  GET_OBJ_RENT(corpse) = 100000;
  if (IS_NPC(ch))
    GET_OBJ_TIMER(corpse) = max_npc_corpse_time;
  else
    GET_OBJ_TIMER(corpse) = max_pc_corpse_time;

//if (!xap_objs) {
  /* transfer character's inventory to the corpse */

  corpse->contains = ch->carrying;
  for (o = corpse->contains; o != NULL; o = o->next_content)
    o->in_obj = corpse;
  object_list_new_owner(corpse, NULL);

  /* transfer character's equipment to the corpse */

  for (i = 0; i < NUM_WEARS; i++)
    if (GET_EQ(ch, i))
      obj_to_obj(unequip_char(ch, i), corpse);

  /* transfer gold */
  if ( money_to_copper(GET_MONEY(ch)) > 0) {
    /* following 'if' clause added to fix gold duplication loophole */
    if (IS_NPC(ch) || (!IS_NPC(ch) && ch->desc)) {
      money = create_money(GET_MONEY(ch));
      obj_to_obj(money, corpse);
    }
    (GET_MONEY(ch)).platinum = 0;
    (GET_MONEY(ch)).steel = 0;
    (GET_MONEY(ch)).gold = 0;
    (GET_MONEY(ch)).copper = 0;
  }
  ch->carrying = NULL;
  IS_CARRYING_N(ch) = 0;
  IS_CARRYING_W(ch) = 0;

//  }
/* for skinning corpses */
   if(IS_NPC(ch))
   assign_skin_value(ch, corpse); 


  obj_to_room(corpse, ch->in_room);
}


/* When ch kills victim */
void change_alignment(struct char_data * ch, struct char_data * victim)
{
  /*  PDH  2/24/99
   *  static alignment now
   */
  return;

  /*
   * new alignment change algorithm: if you kill a monster with alignment A,
   * you move 1/16th of the way to having alignment -A.  Simple and fast.

  GET_ALIGNMENT(ch) += (-GET_ALIGNMENT(victim) - GET_ALIGNMENT(ch)) >> 4;

   */
}



void death_cry(struct char_data * ch)
{
  int door;

  act("Your blood freezes as you hear $n's death cry.", FALSE, ch, 0, 0, TO_ROOM);

  for (door = 0; door < NUM_OF_DIRS; door++) {
    if (CAN_GO(ch, door)) {
      send_to_room("Your blood freezes as you hear someone's death cry.\r\n",
        world[ch->in_room].dir_option[door]->to_room);
    }
  }
}



void raw_kill(struct char_data * ch, struct char_data * killer)
{
  if (FIGHTING(ch))
    stop_fighting(ch);

  GET_POS(ch) = POS_DEAD;

  while (ch->affected)
    affect_remove(ch, ch->affected, 0);

  if (killer) {
    forget(killer, ch);
    if (death_mtrigger(ch, killer))
      death_cry(ch);
  } else
      death_cry(ch);

  make_corpse(ch);
  extract_char(ch);
}



void die(struct char_data * ch, struct char_data * killer)
{
/* hope this works... --gan (Nui) */
   if(killer)
   {
      if(!IS_NPC(killer) && !IS_NPC(ch))
      {
         if(PLR_FLAGGED(killer, PLR_SUBDUING))
         {
            ch->points.hit = 1;
            stop_fighting(ch);
            stop_fighting(killer);
            forget(killer, ch);
            forget(ch, killer);
            clearMemory(killer);
            clearMemory(ch);
            GET_POS(killer) = POS_STANDING;
            GET_POS(ch) = POS_RESTING;
            WAIT_STATE(ch, PULSE_VIOLENCE * 20);
            send_to_char("You have subdued them.\r\n", killer);
            send_to_char("You've been subdued.\r\n", ch);
            act("$n has been subdued.", FALSE, ch, 0, ch, TO_ROOM);

            return;
         }
      }
   }
   increase_blood(ch->in_room);

   gain_exp(ch, -(GET_EXP(ch) / 4));

   if(!IS_NPC(ch))
   {
      REMOVE_BIT(PLR_FLAGS(ch), PLR_KILLER | PLR_THIEF);
   }
    
   if (PRF_FLAGGED(ch, PRF_HUNTED))
   REMOVE_BIT(PRF_FLAGS(ch), PRF_HUNTED);

/* This should fix that nasty hunter killer bug -- Gil 
   if(MOB_FLAGGED(killer, MOB_HUNTERKILLER) && !(MOB_FLAGGED(killer, MOB_QUEST)))
   {
      forget(killer, ch);
      HUNTING(killer) = NULL;
   } 

   clearMemory(killer); */
   raw_kill(ch, killer);
}



void perform_group_gain(struct char_data * ch, int base,
			     struct char_data * victim, int maxLev)
{
  int share = MAX(1, MIN(max_exp_gain, base));
  char msg[MAX_STRING_LENGTH];
  int orig_gain = share;

  if (share > 1) {
    sprintf(buf2, "You receive your share of experience -- %d points.\r\n",
	    share);
    send_to_char(buf2, ch);
  } else {
    send_to_char("You receive your share of experience: one measly point!\r\n",
		 ch);
  }

    if (GET_RPFACTOR(ch) > 100)
      sprintf(msg, "&YYou have gained an extra %d experience points for being a good role player.&n\r\n", (GET_RPFACTOR(ch) - 100) * share / 100);
    else if (GET_RPFACTOR(ch) < 100)
      sprintf(msg, "&YYou have missed out on gaining %d experience points for poor role playing.&n\r\n", (100 - GET_RPFACTOR(ch)) * share / 100);
   if (GET_RPFACTOR(ch) != 100)
      send_to_char(msg, ch);

  // Holiday exp bonus
    if (holiday_exp_modifier > 100) {
      sprintf(msg, "&YYou have gained an additional %d experience points as a holiday gift from the staff.&n\r\n", (holiday_exp_modifier - 100) * share / 100);
      send_to_char(msg, ch);
    }  

    if (GET_EXP_MULT(ch)) {
      share += (orig_gain * GET_EXP_MULT(ch)) / 100;
      sprintf(msg, "&YYou have gained an additional %d experience points as a group bonus.&n\r\n", GET_EXP_MULT(ch) * orig_gain / 100);
      send_to_char(msg, ch);
    }


  gain_exp(ch, share);
  change_alignment(ch, victim);
}


void group_gain(struct char_data * ch, struct char_data * victim)
{
  int tot_members = 0;
  int tot_gain    = 0;
  int base        = 0;
  int victExp     = 0;
  int maxLevel    = 0;
  int xp_int      = 0;
  float xp_float  = 0.0;
  int minLevel    = 0;
  int xp_factor   = 100;

  struct char_data *k;
  struct follow_type *f;

  k = ch->master;
  if (k == NULL)
  {
    k = ch;
  }

  if (IS_AFFECTED(k, AFF_GROUP) && (k->in_room == ch->in_room)) {
    tot_members = 1;
    maxLevel    = GET_LEVEL(k);
    minLevel    = GET_LEVEL(k);
  }

  for (f = k->followers; f; f = f->next)
  {
    if (IS_AFFECTED(f->follower, AFF_GROUP) &&
	f->follower->in_room == ch->in_room) 
    {
      if (!IS_NPC(f->follower) && !f->follower->desc)
      {
        /* Link-dead's get no share of pie. */
        continue;
      }

      tot_members++;

      if (GET_LEVEL(f->follower) > maxLevel)
      {
	maxLevel = GET_LEVEL(f->follower);
      }
      if (GET_LEVEL(f->follower) < minLevel)
        minLevel = GET_LEVEL(f->follower);
    }
  }

  /* round up to the next highest tot_members */
  victExp = GET_EXP(victim);

  if (!IS_NPC(victim)) {
    victExp = victExp / 10;
  }

  tot_gain = victExp;

  if (maxLevel - minLevel > 10)
    xp_factor = MAX(1, 100 - ((maxLevel - minLevel) * 20));

  tot_gain *= xp_factor;
  tot_gain /= 100;

  /* tot_gain = victExp + tot_members - 1; */

  /* prevent illegal xp creation when killing players */
  if (!IS_NPC(victim)) 
  {
    tot_gain = MIN(max_exp_loss * 2 / 3, tot_gain);
  }

  if (tot_members >= 1) {
    /*  PDH  3/26/99
     *  change the gain amt to be level based
     */
    /*  OLD  base = MAX(1, tot_gain / tot_members);  */
    base = MAX(1, tot_gain / tot_members);
  } else {
    base = 0;
  }

  if (IS_AFFECTED(k, AFF_GROUP) && k->in_room == ch->in_room) {

    xp_float = ((float) GET_LEVEL(k) / (float) maxLevel) * (float) base;
    xp_int = MAX(1, (int) xp_float);

    perform_group_gain(k, xp_int, victim, maxLevel);
  }

  for (f = k->followers; f; f = f->next) 
  {
    if (IS_AFFECTED(f->follower, AFF_GROUP) &&
	f->follower->in_room == ch->in_room) 
    {
      if (!IS_NPC(f->follower) && !f->follower->desc)
      {
        /* Link-dead's get no share of pie. */
        continue;
      }

      xp_float = ((float) GET_LEVEL(f->follower) / (float) maxLevel) 
                 * (float) base;

      xp_int = MAX(1, (int) xp_float);

      perform_group_gain(f->follower, xp_int, victim, maxLevel);
    }
  }
}


void solo_gain(struct char_data * ch, struct char_data * victim)
{
  int exp, victExp;
  char msg[MAX_STRING_LENGTH];
  int orig_gain = 0;

  victExp = GET_EXP(victim);
  if (!IS_NPC(victim)) {
    victExp = victExp / 10;
  }

  exp = MIN(max_exp_gain, victExp);

  /* Calculate level-difference bonus */

  if (IS_NPC(ch)) {
    exp += MAX(0, (exp * MIN(4, (GET_LEVEL(victim) - GET_LEVEL(ch)))) / 8);
  } else {
    exp += MAX(0, (exp * MIN(8, (GET_LEVEL(victim) - GET_LEVEL(ch)))) / 8);
  }

  exp = MAX(exp, 1);
  orig_gain = exp;

  if (exp > 1) {
    sprintf(buf2, "You receive %d experience points.\r\n", exp);
    send_to_char(buf2, ch);
  } else
    send_to_char("You receive one lousy experience point.\r\n", ch);

    if (GET_RPFACTOR(ch) > 100)
      sprintf(msg, "&YYou have gained an extra %d experience points for being a good role player.&n\r\n", (GET_RPFACTOR(ch) - 100) * exp / 100);
    else if (GET_RPFACTOR(ch) < 100)
      sprintf(msg, "&YYou have missed out on gaining %d experience points for poor role playing.&n\r\n", (100 - GET_RPFACTOR(ch)) * exp / 100);
   if (GET_RPFACTOR(ch) != 100)
      send_to_char(msg, ch);

  // Holiday exp bonus
    if (holiday_exp_modifier > 100) {
      sprintf(msg, "&YYou have gained an additional %d experience points as a holiday gift from the staff.&n\r\n", (holiday_exp_modifier - 100) * exp / 100);
      send_to_char(msg, ch);
    }  


    if (GET_EXP_MULT(ch)) {
      exp += (orig_gain * GET_EXP_MULT(ch)) / 100;
      sprintf(msg, "&YYou have gained an additional %d experience points as a group bonus.&n\r\n", GET_EXP_MULT(ch) * orig_gain / 100);
      send_to_char(msg, ch);
    }


  gain_exp(ch, exp);
  change_alignment(ch, victim);
}


char *replace_string(char *str, char *weapon_singular, char *weapon_plural)
{
  static char buf[256];
  char *cp = buf;

  for (; *str; str++) {
    if (*str == '#') {
      switch (*(++str)) {
      case 'W':
	for (; *weapon_plural; *(cp++) = *(weapon_plural++));
	break;
      case 'w':
	for (; *weapon_singular; *(cp++) = *(weapon_singular++));
	break;
      default:
	*(cp++) = '#';
	break;
      }
    } else
      *(cp++) = *str;

    *cp = 0;
  }				/* For */

  return (buf);
}


/* message for doing damage with a weapon */
void dam_message(int dam, struct char_data * ch, struct char_data * victim,
		      int w_type)
{
  char *buf;
  int msgnum;

  static struct dam_weapon_type {
    char *to_room;
    char *to_char;
    char *to_victim;
  } dam_weapons[] = {

    /* use #w for singular (i.e. "slash") and #W for plural (i.e. "slashes") */

    // 0: no damage, a miss
    {
      "$n tries to #w $N, but misses.",
      "You try to #w $N, but miss.",
      "$n tries to #w you, but misses."
    },

    // 1: 1-2 damage
    {
      "$n tickles $N as $e #W $M.",	/* 1: 1..2  */
      "You tickle $N as you #w $M.",
      "$n tickles you as $e #W you."
    },

    // 2: 3-4 damage
    {
      "$n barely #W $N.",		/* 2: 3..4  */
      "You barely #w $N.",
      "$n barely #W you."
    },

    // 3: 5-6 damage
    {
      "$n #W $N.",			/* 3: 5..6  */
      "You #w $N.",
      "$n #W you."
    },

    // 4: 7-10 damage
    {
      "$n #W $N hard.",			/* 4: 7..10  */
      "You #w $N hard.",
      "$n #W you hard."
    },

    // 5: 11-14 damage
    {
      "$n #W $N very hard.",		/* 5: 11..14  */
      "You #w $N very hard.",
      "$n #W you very hard."
    },

    // 6: 15-18 damage
    {
      "$n #W $N extremely hard.",	/* 6: 15..19  */
      "You #w $N extremely hard.",
      "$n #W you extremely hard."
    },

    // 7: 19-22 damage
    {
      "$n batters $N with $s #w.",
      "You batter $N with your #w.",
      "$n batters you with $s #w."
    },		

    // 8: 23-26 damage
    {
      "$n gravely wounds $N with $s #w.",
			"You gravely wound $N with your #w.",
			"$n gravely wounds you with $s #w."
			
    },

    // 9: 27-30 damage
    {
      "$n massacres $N with $s mighty #w.",
      "You massacre $N with your mighty #w.",
      "$n massacres you with $s mighty #w."
    },

    // 10: 31-34 damage
    {
      "$n drenches $N with blood from $s #w.",
      "You drench $N with blood from your #w.",
      "$n drenches you with blood from $s #w."
    },

    // 11: 35-38 damage
    {
      "$n leaves $M bloodied and battered with $s #w.",
      "You leave $M bloodied and battered with your #w.",
      "$n leaves you bloodied and battered with $s #w."
    },

    // 12: 39 damage or more
    {
      "$n #W $N with incredible force and impact.",
      "You #w $N with incredible force and impact.",
      "$n #W you with incredible force and impact."
    }

  };


  w_type -= TYPE_HIT;		/* Change to base of table with text */

  if (dam == 0)		msgnum = 0;
  else if (dam <= 2)    msgnum = 1;
  else if (dam <= 4)    msgnum = 2;
  else if (dam <= 6)    msgnum = 3;
  else if (dam <= 10)    msgnum = 4;
  else if (dam <= 14)   msgnum = 5;
  else if (dam <= 18)   msgnum = 6;
  else if (dam <= 22)   msgnum = 7;
  else if (dam <= 26)   msgnum = 8;
  else if (dam <= 30)   msgnum = 9;
  else if (dam <= 34)   msgnum = 10;
  else if (dam <= 38)   msgnum = 11;
  else                  msgnum = 12;
// else  msgnum = 8;

  /* damage message to onlookers */
  buf = replace_string(dam_weapons[msgnum].to_room,
	  attack_hit_text[w_type].singular, attack_hit_text[w_type].plural);
  act(buf, FALSE, ch, NULL, victim, TO_NOTVICT);

  /* damage message to damager */
  send_to_char(CCYEL(ch, C_CMP), ch);
  buf = replace_string(dam_weapons[msgnum].to_char,
	  attack_hit_text[w_type].singular, attack_hit_text[w_type].plural);
  act(buf, FALSE, ch, NULL, victim, TO_CHAR);
  send_to_char(CCNRM(ch, C_CMP), ch);

  /* damage message to damagee */
  send_to_char(CCRED(victim, C_CMP), victim);
  buf = replace_string(dam_weapons[msgnum].to_victim,
	  attack_hit_text[w_type].singular, attack_hit_text[w_type].plural);
  act(buf, FALSE, ch, NULL, victim, TO_VICT | TO_SLEEP);
  send_to_char(CCNRM(victim, C_CMP), victim);
}


/*
 * message for doing damage with a spell or skill
 *  C3.0: Also used for weapon damage on miss and death blows
 */
int skill_message(int dam, struct char_data * ch, struct char_data * vict,
		      int attacktype)
{
  int i, j, nr;
  struct message_type *msg;

  struct obj_data *weap = GET_EQ(ch, WEAR_WIELD);

  if (attacktype > TYPE_SUFFERING)
    attacktype = TYPE_PUNCH;

  for (i = 0; i < MAX_MESSAGES; i++) {
    if (fight_messages[i].a_type == attacktype) {
      nr = dice(1, fight_messages[i].number_of_attacks);
      for (j = 1, msg = fight_messages[i].msg; (j < nr) && msg; j++)
	msg = msg->next;

      if (!IS_NPC(vict) && (GET_LEVEL(vict) >= LVL_IMMORT)) {
	act(msg->god_msg.attacker_msg, FALSE, ch, weap, vict, TO_CHAR);
	act(msg->god_msg.victim_msg, FALSE, ch, weap, vict, TO_VICT);
	act(msg->god_msg.room_msg, FALSE, ch, weap, vict, TO_NOTVICT);
      } else if (dam != 0) {
	if (GET_POS(vict) == POS_DEAD) {
	  send_to_char(CCYEL(ch, C_CMP), ch);
	  act(msg->die_msg.attacker_msg, FALSE, ch, weap, vict, TO_CHAR);
	  send_to_char(CCNRM(ch, C_CMP), ch);

	  send_to_char(CCRED(vict, C_CMP), vict);
	  act(msg->die_msg.victim_msg, FALSE, ch, weap, vict, TO_VICT | TO_SLEEP);
	  send_to_char(CCNRM(vict, C_CMP), vict);

	  act(msg->die_msg.room_msg, FALSE, ch, weap, vict, TO_NOTVICT);
	} else {
	  send_to_char(CCYEL(ch, C_CMP), ch);
	  act(msg->hit_msg.attacker_msg, FALSE, ch, weap, vict, TO_CHAR);
	  send_to_char(CCNRM(ch, C_CMP), ch);

	  send_to_char(CCRED(vict, C_CMP), vict);
	  act(msg->hit_msg.victim_msg, FALSE, ch, weap, vict, TO_VICT | TO_SLEEP);
	  send_to_char(CCNRM(vict, C_CMP), vict);

	  act(msg->hit_msg.room_msg, FALSE, ch, weap, vict, TO_NOTVICT);
	}
      } else if (ch != vict) {	/* Dam == 0 */
	send_to_char(CCYEL(ch, C_CMP), ch);
	act(msg->miss_msg.attacker_msg, FALSE, ch, weap, vict, TO_CHAR);
	send_to_char(CCNRM(ch, C_CMP), ch);

	send_to_char(CCRED(vict, C_CMP), vict);
	act(msg->miss_msg.victim_msg, FALSE, ch, weap, vict, TO_VICT | TO_SLEEP);
	send_to_char(CCNRM(vict, C_CMP), vict);

	act(msg->miss_msg.room_msg, FALSE, ch, weap, vict, TO_NOTVICT);
      }
      return 1;
    }
  }
  return 0;
}


void damage(struct char_data * ch, struct char_data * victim, int dam,
	    int attacktype)
{
  ACMD(do_get);
  ACMD(do_split);
  struct char_data *tmp;
  int element;
  int damResist = 0;

  if (GET_POS(victim) <= POS_DEAD) {
    log("SYSERR: Attempt to damage a corpse.");
    die(victim, ch);
    return;			/* -je, 7/7/92 */
  }

  if (!victim)
  {
    mudlog("SYSERR: Damage called with no victim.", BRF, LVL_GRGOD, TRUE);
    return;
  }

  if (!ch)
  {
    mudlog("SYSERR: Damage called with no attacker.", BRF, LVL_GRGOD, TRUE);
    return;
  }

  /* peaceful rooms */
  if (ch != victim && ROOM_FLAGGED(ch->in_room, ROOM_PEACEFUL)) {
    send_to_char("This room just has such a peaceful, easy feeling...\r\n",
		 ch);
    return;
  }

  if ((ch != victim) && affected_by_spell(ch, SPELL_STATUE))
  {
    send_to_char("Being a statue, you find yourself unable to fight.\r\n", ch);
    if (FIGHTING(ch))
      stop_fighting(ch);
    return;
  }

  if ((ch != victim) && affected_by_spell(victim, SPELL_STATUE))
  {
    send_to_char("You can't damage a statue!\r\n", ch);
    if (FIGHTING(ch) && (FIGHTING(ch) == victim))
      stop_fighting(ch);
    return;
  }

  if ((ch == victim) && affected_by_spell(victim, SPELL_STATUE))
    return;

  /*  Now we'll check for inspire fear... Soli, 9/29/99  */
  if (affected_by_spell(ch, SPELL_INSPIRE_FEAR) && number(0, 1))
  {
    send_to_char("The great fear you feel suddenly overwhelms you!\r\n", ch);
    do_flee(ch, "", 0, 0);
    return;
  }

  /*  Let's put the check for guard in here - Soli, 9/2/99  */

  if (victim != ch)
  {
    for (tmp = world[ch->in_room].people; tmp; tmp = tmp->next)
    {
      if (affected_by_spell(tmp, SPELL_SUMMON_GUARD) && !FIGHTING(ch) &&
         (GUARDING(tmp) == victim))
      {
        if (GET_POS(ch) > POS_STUNNED)
          stop_fighting(ch);
        act("$n moves $N out of harm's way.",
          TRUE, tmp, 0, victim, TO_NOTVICT);
        act("You push $N aside, saving $M.",
          FALSE, tmp, 0, victim, TO_CHAR);
        act("$n pushes you out of harm's way.",
          FALSE, tmp, 0, victim, TO_VICT);
        victim = tmp;
      }

      if ((tmp != ch) && (tmp != victim) && (GUARDING(tmp) == victim) &&
          (victim->in_room == tmp->in_room) && (GET_POS(tmp) == POS_STANDING))
      {
        if (check_skill(tmp, 101, SKILL_GUARD))
        {
          if (GET_POS(ch) > POS_STUNNED)
            stop_fighting(ch);
          act("$n moves $N out of harm's way.",
               TRUE, tmp, 0, victim, TO_NOTVICT);
          act("You push $N aside, saving $M.",
               FALSE, tmp, 0, victim, TO_CHAR);
          act("$n pushes you out of harm's way.",
               FALSE, tmp, 0, victim, TO_VICT);
          victim = tmp;
          GUARDING(victim) = NULL;
          break;
        }
        else
        {
          sprintf(buf, "You find yourself unable to protect $N.");
          act(buf, FALSE, tmp, 0, victim, TO_CHAR);
          GUARDING(tmp) = NULL;
        }
      }
    }
  }

  /* shopkeeper protection */
  if (!ok_damage_shopkeeper(ch, victim))
    return;

  /* You can't damage an immortal! */
  if (!IS_NPC(victim) && (GET_LEVEL(victim) >= LVL_IMMORT))
    dam = 0;

  if (victim != ch) {
    /* Start the attacker fighting the victim */
    if (GET_POS(ch) > POS_STUNNED && (FIGHTING(ch) == NULL))
      set_fighting(ch, victim);

    /* Start the victim fighting the attacker */
    if (GET_POS(victim) > POS_STUNNED && (FIGHTING(victim) == NULL)) {
      set_fighting(victim, ch);
      if (MOB_FLAGGED(victim, MOB_MEMORY) && !IS_NPC(ch))
	remember(victim, ch);
      if (MOB_FLAGGED(victim, MOB_HUNTERKILLER) && !(MOB_FLAGGED(victim, MOB_QUEST)) && !IS_NPC(ch)) {
        HUNTING(victim) = ch;
        SET_BIT(PRF_FLAGS(ch), PRF_HUNTED);
      }
    }
  }

  /* GMB -- Bash Bug Fix.  If you stand after failing a bash, a
   * player's fighting stat line should re-appear. 
   */

  if ((GET_POS(ch) == POS_STANDING) && (ch != victim))
     GET_POS(ch) = POS_FIGHTING;

  /* If you attack a pet, it hates your guts */
  if (victim->master == ch)
    stop_follower(victim);

  /* If the attacker is invisible, he becomes visible */
  if (IS_AFFECTED(ch, AFF_INVISIBLE))
    appear(ch);

  if (PLR_FLAGGED(victim, PLR_FISHING) && dam >= 4)
        dam = ((float) dam * 1.5);
  if ((attacktype == SKILL_BACKSTAB || attacktype == SKILL_ADVANCED_BACKSTAB)  && check_skill(ch, 101, SKILL_IMPROVED_BACKSTAB))
     dam += dice(1, GET_SKILL(ch, SKILL_IMPROVED_BACKSTAB)) / 5;

  /*  Here come sanc, stone, and all those.  Let's be fun, though.  */
  if ((attacktype != TYPE_PULSE_DAMAGE) && dam >= 2)
  {

    if (check_skill(victim, 101, SKILL_NATURAL_HARDINESS))
      damResist += dice(1, GET_SKILL(victim, SKILL_NATURAL_HARDINESS) / 10);
    if (check_skill(victim, 101, SKILL_DAMAGE_REDUCTION))
      damResist += dice(1, GET_SKILL(victim, SKILL_DAMAGE_REDUCTION) / 10);
    if (check_skill(victim, 101, SKILL_IMPROVED_DAMAGE_REDUCTION))
      damResist += dice(1, GET_SKILL(victim, SKILL_IMPROVED_DAMAGE_REDUCTION) / 10);
    if (check_skill(victim, 101, SKILL_ADVANCED_DAMAGE_REDUCTION))
      damResist += dice(1, GET_SKILL(victim, SKILL_ADVANCED_DAMAGE_REDUCTION) / 10);
    dam = dam * (100 - damResist) / 100;
    if (IS_AFFECTED(victim, AFF_SANCTUARY))
      dam = (dam * 2) / 3;   /*  66% normal damage - 2/3    */
    else if (affected_by_spell(victim, SPELL_MINOR_SANCTUARY))
      dam = dam * .80;       /*  80% normal damage - 4/5    */
    else if (GET_CLASS(victim) == CLASS_DEFENDER && GET_LEVEL(victim) >= 13) {
    if (GET_POS(victim) == POS_FIGHTING)
  dam =  dam * .80; 
}
          
    else if (IS_AFFECTED(victim, AFF_STONESKIN))
    {
      switch (attacktype)
      {
        case TYPE_HIT:
        case TYPE_STING:
        case TYPE_WHIP:
        case TYPE_SLASH:
        case TYPE_BITE:
        case TYPE_CLAW:
        case TYPE_THRASH:
        case TYPE_PIERCE:
        case TYPE_BLAST:
        case TYPE_PUNCH:
        case TYPE_STAB:
        case TYPE_SPEAR:
        case TYPE_CLEAVE:
        case SKILL_BACKSTAB:
        case SKILL_ADVANCED_BACKSTAB:
          dam /= 2;
          break;
        default:
          break;
      }
    }

    if ((element = get_element(ch, attacktype)) > 0)
      if (protect_from_element(victim, element))
        dam = (dam * 2) / 3;    /*  2/3 damage from protected elements  */

    if (IS_UNDEAD(ch) && affected_by_spell(victim, SPELL_PROT_FROM_UNDEAD))
      dam = dam * .80; 
    else if (IS_EVIL(ch) && affected_by_spell(victim, SPELL_PROT_FROM_EVIL))
      dam = dam * .80; 
    else if (IS_GOOD(ch) && affected_by_spell(victim, SPELL_PROT_FROM_GOOD))
      dam = dam * .80; 
  }  /*  end of the pulse_damage check  */

  check_killer(ch, victim);

  /* Set the maximum damage per round and subtract the hit points */
  if ((attacktype == SPELL_THORNSHIELD) || (attacktype == SPELL_FIRE_SHIELD))
    dam = MAX(MIN(dam, 25), 0);
  else if (is_weapon(attacktype))
    dam = MAX(MIN(dam, 150), 0);
  else
    dam = MAX(MIN(dam, 200), 0);

/* special super bonus */
  if(GET_ID(ch) == 5334)
    dam = dam * 4;

  GET_HIT(victim) -= dam;

  if (is_weapon(attacktype) && affected_by_spell(ch, SPELL_VAMPIRIC_AURA))
  {
    GET_HIT(ch) = MIN((GET_HIT(ch) + dam/4), GET_MAX_HIT(ch));
    act("Your aura drains some energy from $N.", 0, ch, 0, victim, TO_CHAR);
    act("$n's aura drains some of $N's energy.", 1, ch, 0, victim, TO_NOTVICT);
    act("$n's aura drains some energy from you!", 0, ch, 0, victim, TO_VICT);
  }

  /* Gain exp for the hit */
  if (ch != victim) {
    gain_exp(ch, GET_LEVEL(victim) * dam);
  }

  if((ch != victim) && (!IS_NPC(victim)) && (!IS_NPC(ch)) && (PLR_FLAGGED(ch, PLR_SUBDUING)))
  {
    if(GET_HIT(victim) < 1)
    {
      die(victim, ch);
    }
  }
  else
  {
    update_pos(victim);
  }

  if (attacktype > TYPE_SUFFERING)
    attacktype = TYPE_PUNCH;

  if (!IS_WEAPON(attacktype))
    skill_message(dam, ch, victim, attacktype);
  else
    dam_message(dam, ch, victim, attacktype);

  /* Use send_to_char -- act() doesn't send message if you are DEAD. */
  switch (GET_POS(victim)) {
  case POS_MORTALLYW:
    act("$n is mortally wounded, and will die soon, if not aided.",
	TRUE, victim, 0, 0, TO_ROOM);
    send_to_char("You are mortally wounded, and will die soon, if not aided.\r\n", victim);
    break;
  case POS_INCAP:
    act("$n is incapacitated and will slowly die, if not aided.",
	TRUE, victim, 0, 0, TO_ROOM);
    send_to_char("You are incapacitated an will slowly die, if not aided.\r\n",
		 victim);
    break;
  case POS_STUNNED:
    act("$n is stunned, but will probably regain consciousness again.",
	TRUE, victim, 0, 0, TO_ROOM);
    send_to_char("You're stunned, but will probably regain consciousness again.\r\n", victim);
    break;
  case POS_DEAD:
    act("$n is dead!  R.I.P.", FALSE, victim, 0, 0, TO_ROOM);
    send_to_char("You are dead!  Sorry...\r\n", victim);
    break;

  default:			/* >= POSITION SLEEPING */
    if (dam > (GET_MAX_HIT(victim) / 4))
      act("That really did HURT!", FALSE, victim, 0, 0, TO_CHAR);

    if (GET_HIT(victim) < (GET_MAX_HIT(victim) / 4)) {
      sprintf(buf2,
	      "%sYou wish that your wounds would stop BLEEDING so much!%s\r\n",
	      CCRED(victim, C_SPR), CCNRM(victim, C_SPR));
      send_to_char(buf2, victim);

      /*      if (MOB_FLAGGED(victim, MOB_WIMPY) && (ch != victim))*/
      if (IS_NPC(victim) && (ch != victim) && MOB_FLAGGED(victim, MOB_WIMPY) && !(MOB_FLAGGED(victim, MOB_QUEST)))
	do_flee(victim, "", 0, 0);
    }

    if (!IS_NPC(victim) && GET_WIMP_LEV(victim) && (victim != ch) &&
	/*	GET_HIT(victim) < GET_WIMP_LEV(victim)) {*/
	GET_HIT(victim) < GET_WIMP_LEV(victim) && GET_HIT(victim) > 0) {
      send_to_char("You wimp out, and attempt to flee!\r\n", victim);
      do_flee(victim, "", 0, 0);
    }
    break;
  }


  /* Help out poor linkless people who are attacked */
  if (!IS_NPC(victim) && !(victim->desc)) {
    do_flee(victim, "", 0, 0);
    if (!FIGHTING(victim)) {
      act("$n is rescued by divine forces.", FALSE, victim, 0, 0, TO_ROOM);
      GET_WAS_IN(victim) = victim->in_room;
      char_from_room(victim);
      char_to_room(victim, 0);
    }
  }

  /* stop someone from fighting if they're stunned or worse */
  if ((GET_POS(victim) <= POS_STUNNED) && (FIGHTING(victim) != NULL))
    stop_fighting(victim);

  /* Uh oh.  Victim died. */
  if (GET_POS(victim) == POS_DEAD) {
    if ((ch != victim) && (IS_NPC(victim) || victim->desc)) {
      if (IS_AFFECTED(ch, AFF_GROUP))
	group_gain(ch, victim);
      else
        solo_gain(ch, victim);
    }

    if (!IS_NPC(victim)) {
      GET_NAME(ch, chname);
      GET_NAME(victim, victname);
      sprintf(buf2, "%s (%d) killed by %s (%d) at %s (%d)",
              victname,
              GET_LEVEL(victim),
              chname,
              GET_LEVEL(ch),
              world[victim->in_room].name,
              world[victim->in_room].number);
      FREE_NAME(victname);
      FREE_NAME(chname);
      mudlog(buf2, BRF, LVL_IMMORT, TRUE);
      if (MOB_FLAGGED(ch, MOB_MEMORY))
	forget(ch, victim);
    }

    die(victim, ch);


  }
  else    // And if the victim didn't die, let's run some more checks
  {
    if (attacktype == SPELL_PHANTASMAL_SNAKE)
    {
      if (!number(0, 4))
        mag_affects(GET_LEVEL(ch), ch, victim, SPELL_POISON, SAVING_SPELL);
      if (!number(0, 4) && !mag_savingthrow(victim, SAVING_SPELL))
        mag_affects(GET_LEVEL(ch), ch, victim, SPELL_HOLD_PERSON, SAVING_SPELL);
    }

    if (is_weapon(attacktype) && affected_by_spell(ch, SKILL_ENVENOM))
    {
      struct affected_type af;
      af.type = SPELL_POISON;
      af.duration = GET_LEVEL(ch) / 2;
      af.location = APPLY_STR;
      af.bitvector = 0;
      af.modifier = 0 - (((GET_LEVEL(ch) - 1) / 10) + 1);

      if (!check_skill(ch, 101, SKILL_ENVENOM))
      {
        send_to_char("Your weapon, poorly envenomed, poisons you instead!\r\n", ch);
        affect_join(ch, &af, TRUE, FALSE, TRUE, FALSE);
      }
      else
      {
        sprintf(buf, "$n's weapon seems to have been poisoned!");
        act(buf, FALSE, ch, 0, victim, TO_VICT);
        act(buf, TRUE, ch, 0, victim, TO_NOTVICT);
        sprintf(buf, "Your poisoned weapon has a distinct effect upon $N.");
        act(buf, FALSE, ch, 0, victim, TO_CHAR);
        affect_join(victim, &af, FALSE, FALSE, FALSE, FALSE);
      }

      affect_from_char(ch, SKILL_ENVENOM);
    }   // end of the check for envenom

    if (is_weapon(attacktype) && affected_by_spell(victim, SPELL_THORNSHIELD))
      damage(victim, ch, dam/4, SPELL_THORNSHIELD);

    if (is_weapon(attacktype) && ch &&
        affected_by_spell(victim, SPELL_FIRE_SHIELD))
      damage(victim, ch, dam/3, SPELL_FIRE_SHIELD);

    if (ch && (affected_by_spell(victim, SPELL_CLOAK_OF_FEAR)))
    {
      if (!number(0, 2))
        mag_affects(GET_LEVEL(victim), victim, ch, SPELL_INSPIRE_FEAR,
                    SAVING_SPELL);
    }

    if (ch && affected_by_spell(ch, SPELL_INSPIRE_FEAR))
    {
      send_to_char("A great fear grips your heart, causing you to panic.\r\n", ch);
      do_flee(ch, "", 0, 0);
    }
  }  // end of checks after damage if the victim survived
}

void dehydrate(struct char_data * ch)
{
  struct obj_data *obj;
  int thirst, i;

  if (weather_info.sky == SKY_CLOUDLESS)
    thirst = -6;
  else
    thirst = -3;

  for (i = 0; i < NUM_WEARS; i++)
  {
    if ((obj = GET_EQ(ch, i)))
    {
      if ((GET_OBJ_TYPE(obj) == ITEM_DRINKCON) && (GET_OBJ_VAL(obj, 1) > 0))
        GET_OBJ_VAL(obj, 1) = MIN(0, GET_OBJ_VAL(obj, 1) + thirst);
    }
  }

  obj = ch->carrying;
  while (obj)
  {
    if ((GET_OBJ_TYPE(obj) == ITEM_DRINKCON) && (GET_OBJ_VAL(obj, 1) > 0))
      GET_OBJ_VAL(obj, 1) = MIN(0, GET_OBJ_VAL(obj, 1) + thirst);
    obj = obj->next_content;
  }

  gain_condition(ch, THIRST, thirst);
  send_to_char("You feel as if all the water in your body was drying up.\r\n",
    ch);
}

void hit(struct char_data * ch, struct char_data * victim, int type)
{
  struct obj_data *wielded = GET_EQ(ch, WEAR_WIELD);
  struct obj_data *hold = GET_EQ(ch, WEAR_HOLD);
  int w_type, victim_ac1, calc_thaco1, dam, diceroll;
  //int imod, dam2;
  int percent, prob, tmpdam;
  int weapavg, bhdavg;
  //extern int thaco[NUM_CLASSES][LVL_IMPL+1];
  int backstab_mult_marked = 0;

  int backstab_mult(int level);

  if ((ch == (struct char_data*) NULL) || (victim == (struct char_data*) NULL))
  {
    return;
  }

  if (ch->in_room != victim->in_room)
  {
    if (FIGHTING(ch) && FIGHTING(ch) == victim)
      stop_fighting(ch);
    return;
  }

  if ((type != SPELL_SPIRIT_HAMMER) && (type != SPELL_FLAME_BLADE) &&
      (type != SPELL_DANCING_SWORD) && (type != SPELL_PHANTASMAL_SNAKE))
  {
    if (type == SKILL_DUAL_WIELD)
    {
      if (hold && (GET_OBJ_TYPE(hold) == ITEM_WEAPON))
        w_type = GET_OBJ_VAL(hold, 3) + TYPE_HIT;
      else
        w_type = TYPE_HIT;   // How did we call with dual without a offhand?
    }
    else
    {
      if (wielded && (GET_OBJ_TYPE(wielded) == ITEM_WEAPON))
        w_type = GET_OBJ_VAL(wielded, 3) + TYPE_HIT;
      else if (IS_NPC(ch))
        w_type = ch->mob_specials.attack_type + TYPE_HIT;
      else   // PC with no weapon
        w_type = TYPE_HIT;
    }
  }
  else   // spiritual hammer, and similar spells
    w_type = type;


  calc_thaco1 = calc_thaco(ch);
  
  victim_ac1 = calc_ac(victim, ch);

  if (type == SKILL_BACKSTAB || type == SKILL_ADVANCED_BACKSTAB) {
    if (check_skill(ch, 101, SKILL_HIDE) && check_skill(ch, 101, SKILL_SNEAK))
      calc_thaco1 -= 2; // automatic +2 bonus for attacking from behind
    if (check_skill(ch, 101, SKILL_IMPROVED_BACKSTAB))
      calc_thaco1 -= dice(1, MAX(1, GET_SKILL(ch, SKILL_IMPROVED_BACKSTAB) / 30));
    if (check_skill(ch, 101, SKILL_ADVANCED_BACKSTAB))
      calc_thaco1 -= dice(1, MAX(1, GET_SKILL(ch, SKILL_ADVANCED_BACKSTAB) / 30));
    if (ch->desc && ch->desc->marked && ch->desc->marked == victim && ch->desc->marked_time >= 3) {
      calc_thaco1 -= 1000; // Marked opponents are autmoatically hit with backstabs
      backstab_mult_marked = 1; // marked backstabs do more damage
      ch->desc->marked = NULL;
      ch->desc->marked_time = 0;      
    }
  }
	
  /* roll the die and take your chances... */
  diceroll = number(1, 20);

  /* decide whether this is a hit or a miss */
  if ((((diceroll < 20) && AWAKE(victim)) &&
       ((diceroll == 1) || ((calc_thaco1 - diceroll) > victim_ac1)))) {
    /* the attacker missed the victim */  
    if (type == SKILL_BACKSTAB) {
      damage(ch, victim, 0, SKILL_BACKSTAB);
    } 
    else if (type == SKILL_ADVANCED_BACKSTAB) {
      damage(ch, victim, 0, SKILL_ADVANCED_BACKSTAB);
    }
    else {
      damage(ch, victim, 0, w_type);
    }

  } else {

  if (affected_by_spell(ch, SPELL_SLOW) && number(0, 1))
  {
    act("$N easily avoids your slowed attack.", FALSE, ch, 0, victim, TO_CHAR);
    act("$N avoids $n's slowed attack.", FALSE, ch, 0, victim, TO_NOTVICT);
    act("$n's attack is so slow you avoid it.", FALSE, ch, 0, victim, TO_VICT);
    return;
  }



  if (victim && !IS_NPC(victim) && !GET_EQ(victim, WEAR_SHIELD) && !GET_EQ(victim, WEAR_HOLD) && 
      GET_EQ(victim, WEAR_WIELD) && !OBJ_FLAGGED(GET_EQ(victim, WEAR_WIELD), ITEM_TWO_HANDED) &&
      check_skill(victim, 101, SKILL_ONE_WEAPON_STYLE) &&
      (GET_DEX(victim) * 2) >= dice(1, 100)) {

    act("$N parries your attack with $S weapon.",
         FALSE, ch, 0, victim, TO_CHAR);
    act("$N parries $n's attack with $S weapon.",
         FALSE, ch, 0, victim, TO_NOTVICT);
    act("You parry $n's attack with your weapon.",
         FALSE, ch, 0, victim, TO_VICT);
    return;
  }

  if (!IS_NPC(victim) && GET_EQ(victim, WEAR_SHIELD) &&
      number(1, 100) <= (MIN(20, 
      (check_skill(victim, 101, SKILL_SWORD_SHIELD_STYLE) ? 
       dice(1, GET_SKILL(victim, SKILL_SWORD_SHIELD_STYLE) / 15) : 0) +
      (check_skill(victim, 101, SKILL_SHIELD_MASTERY) ?
       dice(1, GET_SKILL(victim, SKILL_SHIELD_MASTERY) / 15) : 0) +
      (check_skill(victim, 101, SKILL_IMPROVED_SHIELD_MASTERY) ?
       dice(1, GET_SKILL(victim, SKILL_IMPROVED_SHIELD_MASTERY) / 15) : 0) +
      (check_skill(victim, 101, SKILL_ADVANCED_SHIELD_MASTERY) ?
       dice(1, GET_SKILL(victim, SKILL_ADVANCED_SHIELD_MASTERY) / 15) : 0) )))
  {

    act("$N blocks your attack on $S shield.",
         FALSE, ch, 0, victim, TO_CHAR);
    act("$N blocks $n's attack on $S shield.",
         FALSE, ch, 0, victim, TO_NOTVICT);
    act("You block $n's attack on your shield.",
         FALSE, ch, 0, victim, TO_VICT);
    return;
  }


  if (!number(0, 3) && affected_by_spell(victim, SPELL_BLINK))
  {

    act("$N blinks out of view as you move to attack $M.",
         FALSE, ch, 0, victim, TO_CHAR);
    act("$N blinks out of view as $n moves to attack $M.",
         FALSE, ch, 0, victim, TO_NOTVICT);
    act("You blink out of view just in time to avoid $n's attack.",
         FALSE, ch, 0, victim, TO_VICT);
    return;
  }

  if (number(0, 2) && affected_by_spell(victim, SPELL_MIRROR_IMAGE))
  {
    act("You shatter an image in the likeness of $N.",
        FALSE, ch, 0, victim, TO_CHAR);
    act("$n shatters one of $N's mirror images.", 0, ch, 0, victim, TO_NOTVICT);
    act("$n shatters one of your mirror images.", 0, ch, 0, victim, TO_VICT);
    unaffect_one(victim, SPELL_MIRROR_IMAGE);
    return;
  }

  if (number(0, 1) && affected_by_spell(victim, SPELL_DECEPTIVE_IMAGERY))
  {
    act("You attack $N, and find it's only an image.",
         FALSE, ch, 0, victim, TO_CHAR);
    act("$n attacks the wrong copy of $N.", TRUE, ch, 0, victim, TO_NOTVICT);
    act("$n attacks one of your images instead of you.",
         FALSE, ch, 0, victim, TO_VICT);

    if(mag_savingthrow(ch, SAVING_SPELL))
    {
      unaffect_one(victim, SPELL_DECEPTIVE_IMAGERY);
      act("The image shatters like glass.", FALSE, ch, 0, victim, TO_ROOM);
      act("You shatter the image.", FALSE, ch, 0, victim, TO_CHAR);
      return;
    }
    else
    {
      act("Your image blinks out of view just in time to avoid $n's attack",
          FALSE, ch, 0, victim, TO_VICT);
      act("$N blinks out of view as you move to attack $M.",
          FALSE, ch, 0, victim, TO_CHAR);
      act("$N blinks out of view as $n moves to attack $M.",
          FALSE, ch, 0, victim, TO_NOTVICT);
      return;
    }
  }


    /* snip dodge and paryy */
    if ( ! IS_NPC(victim)) {
      if (GET_SKILL(victim, SKILL_DODGE)) {
        percent = number(75, 150) + GET_HITROLL(ch);
        percent += (GET_DEX(ch) - GET_DEX(victim));
        if (GET_LEVEL(victim) >= 17 && (GET_CLASS(victim) == CLASS_ROGUE || GET_CLASS(victim) == CLASS_RANGER))
          percent -= GET_DEX(victim);

        if ((GET_POS(victim) == POS_FIGHTING) &&
             use_skill(victim, percent, SKILL_DODGE))
        {
	  act("$N swiftly dodges your attack.", FALSE, ch, 0, victim, TO_CHAR);
	  act("$N swiftly dodges $n's attack.",
	      FALSE, ch, 0, victim, TO_NOTVICT);
	  act("You swiftly dodge $n's attack.", FALSE, ch, 0, victim, TO_VICT);

	  return;
	}
      }
    }


    if (IS_NPC(victim)) {
      percent = number(1, 150) + GET_HITROLL(ch) + GET_DEX(ch);

      prob = MIN(85, GET_LEVEL(victim)) + GET_DEX(victim);

      if (GET_POS(victim) == POS_FIGHTING && prob > percent) {
	act("$N swiftly blocks your attack.", FALSE, ch, 0, victim, TO_CHAR);
	act("$N swiftly blocks $n's attack.", FALSE, ch, 0, victim,TO_NOTVICT);
	act("You swiftly blocks $n's attack.", FALSE, ch, 0, victim, TO_VICT);

	return;
      }
    }

  /*  Damage calculations.  Retooled for attacking spells.  Soli, 9/99 */

    if (type == SPELL_FLAME_BLADE)
    {
      dam = dice(1, 5) + 1;
    }
    else if (type == SPELL_SPIRIT_HAMMER)
    {
      dam = dice(2, 3);
    }

    else if (type == SPELL_DANCING_SWORD)
    {
      dam = dice(2, GET_LEVEL(ch) / 4);
    }
    else if (type == SPELL_PHANTASMAL_SNAKE)
    {
      dam = number(GET_LEVEL(ch) / 3, GET_LEVEL(ch) / 2);
    }
    else
    {
      if (type == SKILL_DUAL_WIELD)
      {
        if (hold) {
          dam = dice(GET_OBJ_VAL(hold, 1), GET_OBJ_VAL(hold, 2));

          if (check_skill(ch, 101, SKILL_TWO_WEAPON_STYLE) && GET_DEX(ch) >= dice(1,100)) {

            dam *= 100 + (GET_SKILL(ch, SKILL_TWO_WEAPON_STYLE) * 5);
            dam /= 100;

            act("You batter $N's vitals in a flurry of strikes!", FALSE, ch, 0, victim, TO_CHAR);
            act("$n batters your vitals in a flurry of strikes!", FALSE, ch, 0, victim, TO_VICT);
            act("$nbatters $N's vitals in a flurry of strikes!", FALSE, ch, 0, victim, TO_NOTVICT);

          }
        }
        else
          dam = dice(1, 2);
      }
      else
      {
        if (!wielded)
        {
          
          if (IS_NPC(ch))
          {
            dam = dice(ch->mob_specials.damnodice,
                        ch->mob_specials.damsizedice);
          }
        /* else if (PRF_FLAGGED(ch, PRF_NOTSELF) && !PRF_FLAGGED(ch, PRF_DISGUISE)) {
         dam = dice(dam_poly[(int)GET_LEVEL(ch)][0], dam_poly[(int)GET_LEVEL(ch)][1]);
}*/
          else
          {
             w_type = TYPE_PUNCH;
             if(IS_MONK(ch))
             {
                dam = dice(2, 4);

                if (check_skill(ch, 101, SKILL_MARTIAL_ARTS))
                  dam += MAX(dice(2, MAX(1, GET_SKILL(ch, SKILL_MARTIAL_ARTS) / 15)), 
                             check_skill(ch, 101, SKILL_INSIGHT) ?
                             dice(2, MAX(1, GET_SKILL(ch, SKILL_INSIGHT) / 15)) : 0);
                if (check_skill(ch, 101, SKILL_IMPROVED_MARTIAL_ARTS))
                  dam += MAX(dice(1, MAX(1, GET_SKILL(ch, SKILL_IMPROVED_MARTIAL_ARTS) / 15)), 
                             check_skill(ch, 101, SKILL_INSIGHT) ?
                             dice(1, MAX(1, GET_SKILL(ch, SKILL_INSIGHT) / 15)) : 0);
                if (check_skill(ch, 101, SKILL_ADVANCED_MARTIAL_ARTS))
                  dam += MAX(dice(1, MAX(1, GET_SKILL(ch, SKILL_ADVANCED_MARTIAL_ARTS) / 15)), 
                             check_skill(ch, 101, SKILL_INSIGHT) ?
                             dice(1, MAX(1, GET_SKILL(ch, SKILL_INSIGHT) / 15)) : 0);
             } else {
                dam = dice(1, 4);
             }
          }
        }
        else
        {
          if (IS_NPC(ch))
          {
            weapavg = (1 + GET_OBJ_VAL(wielded, 2)) * GET_OBJ_VAL(wielded, 1);
            weapavg /= 2;
            bhdavg = (1 + ch->mob_specials.damsizedice);
            bhdavg = (bhdavg * ch->mob_specials.damnodice) / 2;
            if (weapavg > bhdavg)
              dam = dice(GET_OBJ_VAL(wielded, 1), GET_OBJ_VAL(wielded, 2));
            else
              dam = dice(ch->mob_specials.damnodice,
                          ch->mob_specials.damsizedice);
          }
          else 
            dam = dice(GET_OBJ_VAL(wielded, 1), GET_OBJ_VAL(wielded, 2));

          dam += str_app[STRENGTH_APPLY_INDEX(ch)].todam + GET_DAMROLL(ch);
        
          if (check_skill(ch, 101, SKILL_TWO_HANDED_STYLE) && wielded && 
              OBJ_FLAGGED(wielded, ITEM_TWO_HANDED) && dice(1, 25) == 1 &&
              GET_POS(victim) > POS_SITTING && (!MOB_FLAGGED(victim, MOB_NOBASH))) {
            GET_POS(victim) = POS_SITTING;
            WAIT_STATE(victim, PULSE_VIOLENCE);
            act("You stagger $N with your blow knocking $M down!", FALSE, ch, 0, victim, TO_CHAR);
            act("$n staggers you with $s blow knocking you down!", FALSE, ch, 0, victim, TO_VICT);
            act("$n staggers $N with $s blow knocking $M down!", FALSE, ch, 0, victim, TO_NOTVICT);
          }

          if (wielded && OBJ_FLAGGED(wielded, ITEM_TWO_HANDED))
            dam += MAX(1, GET_STR(ch) / 5);
  
          if (!IS_NPC(ch) && !GET_EQ(ch, WEAR_SHIELD) && !GET_EQ(ch, WEAR_HOLD) && 
            GET_EQ(ch, WEAR_WIELD) &&
            !OBJ_FLAGGED(GET_EQ(ch, WEAR_WIELD), ITEM_TWO_HANDED) &&
            check_skill(ch, 101, SKILL_ONE_WEAPON_STYLE) &&
            GET_DEX(ch) >= dice(1, 100)) {
            
            dam *= MAX(20, GET_DEX(ch));
            dam /= 10;

            act("You strike $N in a vital area!", FALSE, ch, 0, victim, TO_CHAR);
            act("$n strikes you in a vital area!", FALSE, ch, 0, victim, TO_VICT);
            act("$n strikes $N in a vital area!", FALSE, ch, 0, victim, TO_NOTVICT); 
          }

          weapon_spells(ch,victim,wielded);
        }
      }
    }


 /*  if ( ! IS_NPC(victim)) {
 if (GET_SKILL(victim, SKILL_PARRY)) {
        percent = number(75, 150) + GET_HITROLL(ch);
        percent += (GET_DEX(ch) - GET_DEX(victim));

        if (GET_POS(victim) == POS_FIGHTING &&  use_skill(victim, percent, SKILL_PARRY) && GET_OBJ_TYPE(GET_EQ(victim, WEAR_WIELD)) == ITEM_WEAPON)
        {
          act("$N swiftly parries your attack with $S weapon!", FALSE, ch, 0, victim, TO_CHAR);
          act("$N swiftly parries $n's attack with $S weapon!", FALSE, ch, 0, victim, TO_NOTVICT);
          act("You swiftly parries $n's attack with your weapon!", FALSE, ch, 0, victim, TO_VICT);

          dam2 = dice(GET_OBJ_VAL(wielded, 1), GET_OBJ_VAL(wielded, 2));
          damage(victim, ch, dam2, -1); 

          return;
        }
      }
    } */


    /*  snip dodge and parry above*/  
    /*
     * Include a damage multiplier if victim isn't ready to fight:
     *
     * Position sitting  1.33 x normal
     * Position resting  1.66 x normal
     * Position sleeping 2.00 x normal
     * Position stunned  2.33 x normal
     * Position incap    2.66 x normal
     * Position mortally 3.00 x normal
     *
     * Note, this is a hack because it depends on the particular
     * values of the POSITION_XXX constants.
     */
    if (GET_POS(victim) < POS_FIGHTING) {
      tmpdam = dam * (POS_FIGHTING - GET_POS(victim));
      dam += (tmpdam / 3);
    }

    /* at least 1 hp damage min per hit */
    dam = MAX(1, dam);

    if (type == SKILL_BACKSTAB) {
      dam *= (backstab_mult(GET_LEVEL(ch)) + backstab_mult_marked);
      damage(ch, victim, dam, SKILL_BACKSTAB);
    }
    else if (type == SKILL_ADVANCED_BACKSTAB) {
      dam *= (backstab_mult(GET_LEVEL(ch)) + backstab_mult_marked);
      damage(ch, victim, dam, SKILL_ADVANCED_BACKSTAB);
    } 
    else {
      damage(ch, victim, dam, w_type);
    }
  }

  /* check if the victim has a hitprcnt trigger */
  hitprcnt_mtrigger(victim);
}


void holdhit(struct char_data * ch, struct char_data * victim, int type)
{
  struct obj_data *hold = GET_EQ(ch, WEAR_HOLD);
  int w_type, victim_ac1, calc_thaco1, dam, diceroll;

  //extern int thaco[NUM_CLASSES][LVL_IMPL+1];

  int backstab_mult(int level);

  if (ch->in_room != victim->in_room) {
    if (FIGHTING(ch) && FIGHTING(ch) == victim)
      stop_fighting(ch);
    return;
  }

  if (hold && GET_OBJ_TYPE(hold) == ITEM_WEAPON)
    w_type = GET_OBJ_VAL(hold, 3) + TYPE_HIT;
  else {
    if (IS_NPC(ch) && (ch->mob_specials.attack_type != 0))
      w_type = ch->mob_specials.attack_type + TYPE_HIT;
    else
      w_type = TYPE_HIT;
  }

  /* Calculate the raw armor including magic armor.  Lower AC is better. */

  calc_thaco1 = calc_thaco(ch);

  victim_ac1 = calc_ac(victim, ch);

  if (type == SKILL_BACKSTAB || type == SKILL_ADVANCED_BACKSTAB) {
    if (check_skill(ch, 101, SKILL_HIDE) && check_skill(ch, 101, SKILL_SNEAK))
      calc_thaco1 -= 2; // automatic +2 bonus for attacking from behind
    if (check_skill(ch, 101, SKILL_IMPROVED_BACKSTAB))
      calc_thaco1 -= dice(1, MAX(1, GET_SKILL(ch, SKILL_IMPROVED_BACKSTAB) / 30));
    if (check_skill(ch, 101, SKILL_ADVANCED_BACKSTAB))
      calc_thaco1 -= dice(1, MAX(1, GET_SKILL(ch, SKILL_ADVANCED_BACKSTAB) / 30));
  }

	

  diceroll = number(1, 20);

  /* decide whether this is a hit or a miss */
  if ((((diceroll < 20) && AWAKE(victim)) &&
       ((diceroll == 1) || ((calc_thaco1 - diceroll) > victim_ac1)))) {
    if (type == SKILL_BACKSTAB)
      damage(ch, victim, 0, SKILL_BACKSTAB);
    else if (type == SKILL_ADVANCED_BACKSTAB)
      damage(ch, victim, 0, SKILL_ADVANCED_BACKSTAB);
    else
      damage(ch, victim, 0, w_type);
  } else {
    /* okay, we know the guy has been hit.  now calculate damage. */
    dam = str_app[STRENGTH_APPLY_INDEX(ch)].todam;
    dam += GET_DAMROLL(ch);

    if (hold)
      dam += dice(GET_OBJ_VAL(hold, 1), GET_OBJ_VAL(hold, 2));
    else {
      if (IS_NPC(ch)) {
	dam += dice(ch->mob_specials.damnodice, ch->mob_specials.damsizedice);
      } else
	dam += number(0, 2);	/* Max. 2 dam with bare hands */
    }

    if (GET_POS(victim) < POS_FIGHTING)
      dam *= 1 + (POS_FIGHTING - GET_POS(victim)) / 3;
    /* Position  sitting  x 1.33 */
    /* Position  resting  x 1.66 */
    /* Position  sleeping x 2.00 */
    /* Position  stunned  x 2.33 */
    /* Position  incap    x 2.66 */
    /* Position  mortally x 3.00 */
 
    dam = MAX(1, dam);		/* at least 1 hp damage min per hit */
    if (type == SKILL_BACKSTAB) {
      dam *= backstab_mult(GET_LEVEL(ch));
      damage(ch, victim, dam, SKILL_BACKSTAB);
    }
    else if (type == SKILL_ADVANCED_BACKSTAB) {
      dam *= backstab_mult(GET_LEVEL(ch));
      damage(ch, victim, dam, SKILL_ADVANCED_BACKSTAB);
    }
    else
      damage(ch, victim, dam, w_type);
  }
  
}


/* control the fights going on.  Called every 2 seconds from comm.c. */
void perform_violence(void)
{
  struct char_data *ch;
  struct obj_data *holding;
  struct obj_data *wielded;
  extern struct index_data *mob_index;

//  struct obj_data *wielded and int percent, prob, i, j now obsolete - Soli

  for (ch = combat_list; ch; ch = next_combat_list) {
    next_combat_list = ch->next_fighting;

    if (FIGHTING(ch) == NULL || ch->in_room != FIGHTING(ch)->in_room) {
      stop_fighting(ch);
      continue;
    }

    if (!assert_fighting(ch))
      continue;

    affect_update_pulsebased(ch);

    if (IS_NPC(ch)) {
      if (GET_MOB_WAIT(ch) > 0) {
	GET_MOB_WAIT(ch) -= PULSE_VIOLENCE;
	continue;
      }

      GET_MOB_WAIT(ch) = 0;
     
       if ((GET_POS (FIGHTING(ch)) == POS_MORTALLYW) &&
         (MOB_FLAGGED(ch, MOB_QUEST))) {
        stop_fighting(FIGHTING(ch));
        stop_fighting(ch);
          }

      if (GET_POS(ch) < POS_FIGHTING) {
        if (!affected_by_spell(ch, SPELL_CRIPPLE)) {
	  GET_POS(ch) = POS_FIGHTING;
          act("$n scrambles to $s feet!", TRUE, ch, 0, 0, TO_ROOM);
        } else {
          GET_POS(ch) = POS_RESTING;
        }
      }
    }

  /*  The original code ignored the fact that people can flee and relied on
      hit() and damage() to save fleeing characters.  But spells don't check
      rooms... That's why people were killed after wimpying.  Easy fix.  We
      need to call it 3 times, so I made a function for it instead.  */

    hit(ch, FIGHTING(ch), TYPE_UNDEFINED);
    if (!assert_fighting(ch))
      continue;

    if ((IS_NPC(ch) && MOB_FLAGGED(ch, MOB_DBL_ATTACK) && number(0, 4)) ||
       (!IS_NPC(ch) && check_skill(ch, 101, SKILL_SECOND_ATTACK)))
    {
      hit(ch, FIGHTING(ch), TYPE_UNDEFINED);
      if (!assert_fighting(ch))
        continue;
    }


    if ((IS_NPC(ch) && MOB_FLAGGED(ch, MOB_TRPL_ATTACK) && number(0, 4)) ||
       (!IS_NPC(ch) && check_skill(ch, 101, SKILL_THIRD_ATTACK)))
    {
      hit(ch, FIGHTING(ch), TYPE_UNDEFINED);
      if (!assert_fighting(ch))
        continue;
    }

    if ((IS_NPC(ch) && MOB_FLAGGED(ch, MOB_QUAD_ATTACK) && number(0, 4)) ||
       (!IS_NPC(ch) && check_skill(ch, 101, SKILL_FOURTH_ATTACK)))
    {
      hit(ch, FIGHTING(ch), TYPE_UNDEFINED);
      if (!assert_fighting(ch))
        continue;
    }

    holding = GET_EQ(ch, WEAR_HOLD);

    if (!IS_NPC(ch) && holding && (GET_OBJ_TYPE(holding) == ITEM_WEAPON))
    {
      if (check_skill(ch, 101, SKILL_DUAL_WIELD))
      {
        hit(ch, FIGHTING(ch), SKILL_DUAL_WIELD);
        if (!assert_fighting(ch))
          continue;
        if (check_skill(ch, 101, SKILL_TWO_WEAPON_STYLE) && dice(1, 3) == 1) {
          hit(ch, FIGHTING(ch), SKILL_DUAL_WIELD);
          if (!assert_fighting(ch))
            continue;
        }
      }
    }

    wielded = GET_EQ(ch, WEAR_WIELD);

    if (check_skill(ch, 101, SKILL_TWO_HANDED_STYLE) && wielded && 
        OBJ_FLAGGED(wielded, ITEM_TWO_HANDED)) {
      hit(ch, FIGHTING(ch), TYPE_UNDEFINED);
      if (!assert_fighting(ch))
        continue;
    }

    if (IS_AFFECTED(ch, AFF_HASTE) && (number(1, 101) <= 35))
    {
      hit(ch, FIGHTING(ch), TYPE_UNDEFINED);
      if (!assert_fighting(ch))
        continue;
    }

    if (affected_by_spell(ch, SPELL_SPIRIT_HAMMER))
    {
      hit(ch, FIGHTING(ch), SPELL_SPIRIT_HAMMER);
      if (!assert_fighting(ch))
        continue;
    }

    if (affected_by_spell(ch, SPELL_FLAME_BLADE))
    {
      hit(ch, FIGHTING(ch), SPELL_FLAME_BLADE);
      if (!assert_fighting(ch))
        continue;
    }

    if (affected_by_spell(ch, SPELL_DANCING_SWORD))
    {
      hit(ch, FIGHTING(ch), SPELL_DANCING_SWORD);
      if (!assert_fighting(ch))
        continue;
    }

    if (affected_by_spell(ch, SPELL_PHANTASMAL_SNAKE))
    {
      hit(ch, FIGHTING(ch), SPELL_PHANTASMAL_SNAKE);
      if (!assert_fighting(ch))
        continue;
    }

    if (FIGHTING(ch) && dice(1, 100) <= (
      ((check_skill(ch, 101, SKILL_MARTIAL_ARTS)) ? GET_SKILL(ch, SKILL_MARTIAL_ARTS) / 9 : 0) +
      ((check_skill(ch, 101, SKILL_IMPROVED_MARTIAL_ARTS)) ? GET_SKILL(ch, SKILL_IMPROVED_MARTIAL_ARTS) / 9 : 0) +
      ((check_skill(ch, 101, SKILL_ADVANCED_MARTIAL_ARTS)) ? GET_SKILL(ch, SKILL_ADVANCED_MARTIAL_ARTS) / 9 : 0))) {
      damage(ch, FIGHTING(ch), dice(MAX(1, 
             (check_skill(ch, 101, SKILL_MARTIAL_ARTS) ? 1 : 0) + 
             (check_skill(ch, 101, SKILL_IMPROVED_MARTIAL_ARTS) ? 1 : 0) + 
             (check_skill(ch, 101, SKILL_ADVANCED_MARTIAL_ARTS) ? 1 : 0)), 
             (GET_SKILL(ch, SKILL_MARTIAL_ARTS) / 15) +
             (GET_SKILL(ch, SKILL_IMPROVED_MARTIAL_ARTS) / 15) +
             (GET_SKILL(ch, SKILL_ADVANCED_MARTIAL_ARTS) / 15)), SKILL_KICK);
      if (!assert_fighting(ch))
        continue;
    }


    if (FIGHTING(ch) &&
        check_skill(ch, 101, SKILL_THROW_OPPONENT) && dice(1, 100) <=
        (GET_SKILL(ch, SKILL_THROW_OPPONENT) / 9) && (IS_NPC(FIGHTING(ch)) &&
        !MOB_FLAGGED(FIGHTING(ch), MOB_NOBASH))) {
      GET_POS(FIGHTING(ch)) = POS_SITTING;
      WAIT_STATE(FIGHTING(ch), PULSE_VIOLENCE);
      damage(ch, FIGHTING(ch), 1, SKILL_THROW_OPPONENT);
      if (!assert_fighting(ch))
        continue;
    }

    if (FIGHTING(ch) && 
        check_skill(ch, 101, SKILL_DIRTY_FIGHTING) && dice(1, 100) <= MAX(1, int_app[GET_INT(ch)].learn) ) {
      damage(ch, FIGHTING(ch), dice(1, GET_LEVEL(ch) / 2), SKILL_DIRTY_FIGHTING);
      if (!assert_fighting(ch))
        continue;
    }
    if (FIGHTING(ch) &&
        check_skill(ch, 101, SKILL_COMBAT_IMPROVISATION) && dice(1, 100) <= MAX(1, int_app[GET_INT(ch)].learn) ) {
      damage(ch, FIGHTING(ch), dice(1, GET_LEVEL(ch) / 2), SKILL_COMBAT_IMPROVISATION);
      if (!assert_fighting(ch))
        continue;
    }

    if (IS_NPC(ch) && MOB_FLAGGED(ch, MOB_SPEC) &&
       (mob_index[GET_MOB_RNUM(ch)].func != NULL))
    {
      (mob_index[GET_MOB_RNUM(ch)].func) (ch, ch, 0, "");
      if (!assert_fighting(ch))
        continue;
    }

    if (IS_NPC(ch) && MOB_FLAGGED(ch, MOB_USE_SPELLS))
      mob_casting_acts(ch);
  }

}

void perform_pulse_damage(void)
{
  static struct char_data *ch;

  for (ch = character_list; ch; ch = ch->next)
  {
    if ((IS_NPC(ch) && !MOB_FLAGGED(ch, MOB_MOUNTABLE) && !(ch->master)) ||GET_LEVEL(ch) >= LVL_IMMORT)
      continue;
    if (ROOM_FLAGGED(ch->in_room, ROOM_PULSE_DAMAGE))
    {
      damage(ch, ch, 1, TYPE_PULSE_DAMAGE);
    }
  }
}


void perform_area_magic(void)
{
  struct area_aff_data *i, *next_i, *temp;
  struct char_data *ch, *caster, *vict;
  int rm, dam, num, random = 1, spellnum = 0;
  int level = 0, savetype = 0;

  if (!area_spells_list)
    return;

  for (i = area_spells_list; i; i = next_i)
  {
    rm = i->in_room;
    next_i = i->next;

    i->duration_left--;
    if (!i->duration_left)
    {
      if (world[rm].people && *spell_wear_off_msg[i->type])
      {
          send_to_room(spell_wear_off_msg[i->type], rm);
          send_to_room("\r\n", rm);
       }

      REMOVE_FROM_LIST(i, area_spells_list, next);
      free(i);
      continue;
    }

    if (i->type == SPELL_STORM_SUMMONING)
    {
      weather_info.pressure -= 15;
      weather_change();
    } else if (i->type == SPELL_DEHYDRATION)
    {
      weather_info.pressure += 15;
      weather_change();
    }

    if (!world[rm].people)
      continue;

    if (i->caster)
      caster = i->caster;
    else
      caster = NULL;

    for (vict = world[rm].people; vict; vict = vict->next_in_room)
    {
      if (!IS_NPC(vict) && GET_LEVEL(vict) >= LVL_IMMORT)
        continue;

      if (i->caster && ((i->caster == vict) ||
         (((i->caster->master == vict) || (vict->master == i->caster)) &&
         (IS_AFFECTED(i->caster, AFF_GROUP) && IS_AFFECTED(vict, AFF_GROUP)))))
        continue;

      switch (i->type)
      {
        case SPELL_CREEPING_DOOM:
          num = MAX(8 - i->duration_left, i->duration_left - 8);
          num = 9 - ((num + 1) / 2);
          dam = dice(num, 2);
          random = number(0, 3);
          spellnum = SPELL_DISEASE;  level = 10;
          savetype = SAVING_SPELL;
          break;
        case SPELL_BLADE_BARRIER:
          dam = dice(6, 4);
          break;
        case SPELL_INSECT_PLAGUE:
          dam = dice(3, 4);
          random = number(0, 3);
          spellnum = SPELL_POISON;  level = 10;
          savetype = SAVING_SPELL;
          break;
        case SPELL_STORM_SUMMONING:
          if (weather_info.sky != SKY_LIGHTNING)
            continue;
          dam = dice(2, 5);
          break;
        case SPELL_DEHYDRATION:
          dehydrate(vict);
          if (weather_info.sky != SKY_CLOUDLESS)
            continue;
          dam = dice(2, 5);
          break;
        case SPELL_ELEMENTAL_STORM:
          dam = dice(4, 5);
          break;
        default:
          dam = 0;
          break;
      }

      if (i->caster && i->caster->in_room == vict->in_room)
        damage(i->caster, vict, dam, i->type);
      else
        damage(vict, vict, dam, i->type);

      if (!random && spellnum)
        mag_affects(level, vict, vict, spellnum, savetype);
    }
  }

  for (ch = character_list; ch; ch = ch->next)
  {
    if (affected_by_spell(ch, SPELL_BLACK_PLAGUE))
    {
      for (vict = world[ch->in_room].people; vict; vict = vict->next_in_room)
      {
        if (vict != ch)
          mag_affects(30, ch, vict, SPELL_BLACK_PLAGUE, SAVING_SPELL);
      }
    }
  }
}

int calc_thaco(struct char_data *ch)
{
  extern int thaco[NUM_CLASSES][LVL_IMPL+1];
  
  int calc_thaco;
  int imod;

  /* Calculate the THAC0 of the attacker */
  if (!IS_NPC(ch)) {
    calc_thaco = thaco[(int)GET_CLASS(ch)][GET_LEVEL(ch)];
  } else {             /* THAC0 for monsters is set in the HitRoll */
    calc_thaco = 20;
  }

  calc_thaco -= str_app[STRENGTH_APPLY_INDEX(ch)].tohit;
  calc_thaco -= GET_HITROLL(ch);
  /*  Changing the amount that int and wis affect thaco - Soli, 8/15/99  */
  imod = (int) ((GET_INT(ch) + GET_WIS(ch) - 18) / 3);
  calc_thaco -= MIN(3, MAX(-3, imod));

  return calc_thaco;  
  
}

int calc_ac(struct char_data *victim, struct char_data *ch)
{
  int victim_ac = 0;
  struct obj_data *shield;

 /* Calculate the raw armor including magic armor.  Lower AC is better. */
  victim_ac = (check_skill(victim, 101, SKILL_ARMOR_MASTERY) ? 
               GET_AC(victim) - (dice(1, (GET_SKILL(victim, SKILL_ARMOR_MASTERY) / 10) + 
               (check_skill(victim, 101, SKILL_IMPROVED_ARMOR_MASTERY) ? 
               GET_SKILL(victim, SKILL_IMPROVED_ARMOR_MASTERY) / 20 : 0) +
               (check_skill(victim, 101, SKILL_ADVANCED_ARMOR_MASTERY) ?
               GET_SKILL(victim, SKILL_ADVANCED_ARMOR_MASTERY) / 20 : 0))
               * GET_AC(victim) / 100) : GET_AC(victim) ) / 10;


  if ((shield = GET_EQ(victim, WEAR_SHIELD)) && GET_OBJ_TYPE(shield) == ITEM_ARMOR) {
    if (check_skill(victim, 101, SKILL_SHIELD_MASTERY))
      victim_ac -= dice(1, GET_OBJ_VAL(shield, 0) / 2);
    if (check_skill(victim, 101, SKILL_IMPROVED_SHIELD_MASTERY))
      victim_ac -= dice(1, GET_OBJ_VAL(shield, 0) / 2);
    if (check_skill(victim, 101, SKILL_ADVANCED_SHIELD_MASTERY))
      victim_ac -= dice(1, GET_OBJ_VAL(shield, 0) / 2);
  }

  if (AWAKE(victim)) {
    victim_ac -= dex_app[GET_DEX(victim)].defensive;
  }

  if (!IS_NPC(victim)) {
    if (check_skill(victim, 101, SKILL_UNARMED_DEFENSE))
      victim_ac -= MAX(1, MAX(dice(1, GET_SKILL(victim, SKILL_UNARMED_DEFENSE)),
                       check_skill(victim, 101, SKILL_INSIGHT) ? 
                       dice(1, GET_SKILL(victim, SKILL_INSIGHT)) : 0)) / 2;
    if (check_skill(victim, 101, SKILL_IMPROVED_UNARMED_DEFENSE))
      victim_ac -= MAX(1,  MAX(dice(1, GET_SKILL(victim, SKILL_IMPROVED_UNARMED_DEFENSE)),
                       check_skill(victim, 101, SKILL_INSIGHT) ? 
                       dice(1, GET_SKILL(victim, SKILL_INSIGHT)) : 0)) / 2;
    if (check_skill(victim, 101, SKILL_ADVANCED_UNARMED_DEFENSE))
      victim_ac -= MAX(1,  MAX(dice(1, GET_SKILL(victim, SKILL_ADVANCED_UNARMED_DEFENSE)),
                       check_skill(victim, 101, SKILL_INSIGHT) ? 
                       dice(1, GET_SKILL(victim, SKILL_INSIGHT)) : 0)) / 2;
  }

  if (affected_by_spell(victim, SPELL_PROT_FROM_UNDEAD) && IS_UNDEAD(ch))
    victim_ac -= 2;
  else if (affected_by_spell(victim, SPELL_PROT_FROM_EVIL) && IS_EVIL(ch))
    victim_ac -= 2;
  else if (affected_by_spell(victim, SPELL_PROT_FROM_GOOD) && IS_GOOD(ch))
    victim_ac -= 2;

  victim_ac = MAX(-10, victim_ac);	/* -10 is lowest */

  return victim_ac;
}

int calc_weapon_damage(struct obj_data *wielded, struct char_data *ch)
{
  int dam = 0;

  if (!wielded)
    dam = dice(1, 4);
  else 
    dam = dice(GET_OBJ_VAL(wielded, 1), GET_OBJ_VAL(wielded, 2));

  dam += str_app[STRENGTH_APPLY_INDEX(ch)].todam + GET_DAMROLL(ch);

  return dam;
}

