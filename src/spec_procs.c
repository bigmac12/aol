/* ************************************************************************
*   File: spec_procs.c                                  Part of CircleMUD *
*  Usage: implementation of special procedures for mobiles/objects/rooms  *
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


/*   external vars  */
extern struct room_data *world;
extern struct char_data *character_list;
extern struct descriptor_data *descriptor_list;
extern struct index_data *mob_index;
extern struct index_data *obj_index;
extern struct time_info_data time_info;
extern struct command_info cmd_info[];
extern struct prerequisites prereq[];
extern struct spell_info_type spell_info[];

/* extern functions */
void add_follower(struct char_data * ch, struct char_data * leader);
extern int spell_access(struct char_data * ch, int spellnum);
extern int find_dash_skill_num(char *name);


struct social_type {
  char *cmd;
  int next_line;
};


/* ********************************************************************
*  Special procedures for mobiles                                     *
******************************************************************** */

int spell_sort_info[MAX_SKILLS+1];

extern char *spells[];

void sort_spells(void)
{
  int a, b, tmp;

  /* initialize array */
  for (a = 1; a < MAX_SKILLS; a++)
    spell_sort_info[a] = a;

  /* Sort.  'a' starts at 1, not 0, to remove 'RESERVED' */
  for (a = 1; a < MAX_SKILLS - 1; a++)
    for (b = a + 1; b < MAX_SKILLS; b++)
      if (strcmp(spells[spell_sort_info[a]], spells[spell_sort_info[b]]) > 0) {
	tmp = spell_sort_info[a];
	spell_sort_info[a] = spell_sort_info[b];
	spell_sort_info[b] = tmp;
      }
}


char *how_good(int percent)
{
  static char buf[256];
  int plusses = 0;
  int i = 0;

  if (percent == 0)
    strcpy(buf, " (not learned");
  else if (percent <= 10)
    strcpy(buf, " (awful");
  else if (percent <= 20)
    strcpy(buf, " (v. bad");
  else if (percent <= 30)
    strcpy(buf, " (bad");
  else if (percent <= 40)
    strcpy(buf, " (poor");
  else if (percent <= 50)
    strcpy(buf, " (average");
  else if (percent <= 60)
    strcpy(buf, " (above average");
  else if (percent <= 70)
    strcpy(buf, " (fair");
  else if (percent <= 80)
    strcpy(buf, " (good");
  else if (percent <= 90)
    strcpy(buf, " (very good");
  else
    strcpy(buf, " (superb");

  percent /= 10;
 
  plusses = percent / 3;

  for (i = 1; i <= plusses; i++)
    sprintf(buf, "%s+", buf);

  sprintf(buf, "%s)", buf);

  return (buf);
}

char *prac_types[] = {
  "spell",
  "skill"
};

#define LEARNED_LEVEL	0	/* % known which is considered "learned" */
#define MAX_PER_PRAC	1	/* max percent gain in skill per practice */
#define MIN_PER_PRAC	2	/* min percent gain in skill per practice */

/* actual prac_params are in class.c */
extern int prac_params[3][NUM_CLASSES];

#define LEARNED(ch) (prac_params[LEARNED_LEVEL][(int)GET_CLASS(ch)])
#define MINGAIN(ch) (prac_params[MIN_PER_PRAC][(int)GET_CLASS(ch)])
#define MAXGAIN(ch) (prac_params[MAX_PER_PRAC][(int)GET_CLASS(ch)])

void list_skills(struct char_data * ch)
{
  extern char *spells[];
  int i, sortpos;

  if (!GET_PRACTICES(ch)) {
    strcpy(buf, "You have no practice sessions remaining.\r\n");
  } else {
    sprintf(buf, "You have %d practice session%s remaining.\r\n",
	    GET_PRACTICES(ch), (GET_PRACTICES(ch) == 1 ? "" : "s"));
  }

  sprintf(buf, "%sYou know of the following spells:\r\n", buf);
  strcpy(buf2, buf);

  for (sortpos=1; sortpos<MAX_SKILLS; sortpos++) {
    i = spell_sort_info[sortpos];
    if (i > MAX_SPELLS)
      continue;

    if (strlen(buf2) >= MAX_STRING_LENGTH - 32) {
      strcat(buf2, "**OVERFLOW**\r\n");
      break;
    }

    if (spell_access(ch, i)) {
      sprintf(buf, "%-30s %s\r\n", spells[i], how_good(GET_SKILL(ch, i)));
      strcat(buf2, buf);
    }
  }

  strcat(buf2, "\r\nYou know of the following skills:\r\n");

  for (sortpos = 1; sortpos < MAX_SKILLS; sortpos++)
  {
    i = spell_sort_info[sortpos];
    if (i <= MAX_SPELLS)
      continue;

    if (strlen(buf2) >= MAX_STRING_LENGTH - 32) {
      strcat(buf2, "**OVERFLOW**\r\n");
      break;
    }

    if (spell_access(ch, i)) {
      sprintf(buf, "%-30s %s\r\n", spells[i], how_good(GET_SKILL(ch, i)));
      strcat(buf2, buf);
    }
  }

  page_string(ch->desc, buf2, 1);
}

void list_player_skills(struct char_data * ch)
{
  /* Show skills/spells for players class */
                                
  extern char *spells[];
  /* extern char *pc_class_types[]; */
  extern struct spell_info_type spell_info[];
  int i, sortpos;

  sprintf(buf, "This is a list of skills and spells for your class:\r\n");
  strcpy(buf2, buf);

  for (sortpos=1; sortpos<MAX_SKILLS; sortpos++) {
    i = spell_sort_info[sortpos];
    if (strlen(buf2) >= MAX_STRING_LENGTH - 32) {
      strcat(buf2, "**OVERFLOW**\r\n");
      break;
    }

    if ((spell_info[i].min_level[(int)GET_CLASS(ch)]) <= 100) {
      sprintf(buf, "Level: %3d  Name: %20s  Learned: %15s\r\n",
              spell_info[i].min_level[(int)GET_CLASS(ch)], spells[i],
              (GET_LEVEL(ch) >= spell_info[i].min_level[(int)GET_CLASS(ch)] ?
	       how_good(GET_SKILL(ch, i)) : "Advance First!"));
      strcat(buf2, buf);
    }
  
    
    if ((spell_info[i].min_level_race[GET_RACE(ch)]) <= 100) {
      sprintf(buf, "Level: %3d  Name: %20s  Learned: %15s\r\n",
              spell_info[i].min_level_race[GET_RACE(ch)], spells[i],
              (GET_LEVEL(ch) >= spell_info[i].min_level_race[GET_RACE(ch)] ?
               how_good(GET_SKILL(ch, i)) : "Advance First!"));
      strcat(buf2, buf);
    }

  }

  page_string(ch->desc, buf2, 1);
}


SPECIAL(auto_equip_newbie) {

  if (IS_NPC(ch) || !CMD_IS("kit"))
    return 0;
/*
  if (GET_LEVEL(ch) > 5)
    return 0;
*/
  send_to_char("You have received a kit of new adventurer gear.  Check your inventory!\r\n", ch);

	  struct obj_data * objNew, * objCont;

	  void obj_to_char(struct obj_data *object, struct char_data *ch);

	  objCont = read_object( 2496, VIRTUAL);                         /* backpack */
	  objNew=read_object(1945,VIRTUAL);  obj_to_obj(objNew,objCont); /* bread    */
	  objNew=read_object(1945,VIRTUAL);  obj_to_obj(objNew,objCont); /* bread    */
	  objNew=read_object(1945,VIRTUAL);  obj_to_obj(objNew,objCont); /* bread    */
	  objNew=read_object(1945,VIRTUAL);  obj_to_obj(objNew,objCont); /* bread    */
	  objNew=read_object(1945,VIRTUAL);  obj_to_obj(objNew,objCont); /* bread    */
	  objNew=read_object(1945,VIRTUAL);  obj_to_obj(objNew,objCont); /* bread    */
	  objNew=read_object(1945,VIRTUAL);  obj_to_obj(objNew,objCont); /* bread    */
	  objNew=read_object(1945,VIRTUAL);  obj_to_obj(objNew,objCont); /* bread    */
	  obj_to_char(objCont, ch);                                      /* backpack */
	  obj_to_char(read_object( 1905, VIRTUAL), ch);                  /* waterskin*/
	  obj_to_char(read_object(10171, VIRTUAL), ch);                  /* torch    */

	    obj_to_char(read_object(5007, VIRTUAL), ch);                 /* Kar map  */ 
	    obj_to_char(read_object(10000, VIRTUAL), ch);                /* Kal map  */


	  obj_to_char(read_object( 2487, VIRTUAL), ch);
	  obj_to_char(read_object( 2488, VIRTUAL), ch);
	  obj_to_char(read_object( 2489, VIRTUAL), ch);

	  switch (GET_CLASS(ch)) {

	  case CLASS_KNIGHT:
	  case CLASS_PALADIN:
	  case CLASS_RANGER:
	  case CLASS_DEFENDER:
	  case CLASS_WARRIOR:
	    obj_to_char(read_object( 2485, VIRTUAL), ch);
	    obj_to_char(read_object( 2486, VIRTUAL), ch);
	    break;

	  case CLASS_MAGE:
	    obj_to_char( read_object( 2493, VIRTUAL), ch);
	    obj_to_char( read_object( 2492, VIRTUAL), ch);
	    break;

	  case CLASS_CLERIC:
	  case CLASS_DRUID:
	    if (GET_CLASS(ch) == CLASS_CLERIC)
	    {
	      obj_to_char(read_object(25, VIRTUAL), ch); /* Priestly symbol */
	    }
	    obj_to_char( read_object( 2491, VIRTUAL), ch);
	    obj_to_char( read_object( 2492, VIRTUAL), ch);
	    break;

	  case CLASS_ROGUE:
	    obj_to_char( read_object( 2494, VIRTUAL), ch);
	    obj_to_char( read_object( 2495, VIRTUAL), ch);
	    break;

	  case CLASS_ADVENTURER:
	  case CLASS_MONK:
	     obj_to_char( read_object( 10198, VIRTUAL), ch);
	    break;

          }

  return 1;

}


SPECIAL(guild)
{
  int skill_num, percent, i, need_prereq = 0;

  extern struct int_app_type int_app[];

  if (IS_NPC(ch) || !CMD_IS("practice"))
    return 0;

  skip_spaces(&argument);

  if (!*argument) {
    list_skills(ch);
    return 1;
  }

  if (GET_PRACTICES(ch) <= 0) {
    send_to_char("You do not seem to be able to practice now.\r\n", ch);
    return 1;
  }

  skill_num = find_dash_skill_num(argument);

  if ((skill_num < 1) || (!spell_access(ch, skill_num))) {
    if (skill_num > MAX_SPELLS)
      sprintf(buf, "You do not know of that skill.\r\n");
    else
      sprintf(buf, "You do not know of that spell.\r\n");
    send_to_char(buf, ch);
    return 1;
  }

  if (GET_SKILL(ch, skill_num) >= LEARNED(ch))
  {
    if (GET_SKILL(ch, skill_num) >= spell_info[skill_num].max_percent)
      send_to_char("You are already learned in that skill.\r\n", ch);
    else
      send_to_char("You cannot practice that skill any more.\r\n", ch);

    return 1;
  }

  for (i = 0; prereq[i].spell; i++)
  {
    if ((prereq[i].spell == skill_num) && (!GET_SKILL(ch, prereq[i].pre) ||
       (((prereq[i].pre > MAX_SPELLS) == (skill_num > MAX_SPELLS)) &&
        (GET_SKILL(ch, prereq[i].pre) <= GET_SKILL(ch, skill_num)))))
    {
      if (spell_access(ch, prereq[i].pre))
      {
        sprintf(buf, "You must practice %s first.", spells[prereq[i].pre]);
        send_to_char(buf, ch);
        need_prereq++;
      }
    }
  }

  if (need_prereq)
    return 1;

  send_to_char("You practice for a while...\r\n", ch);
  GET_PRACTICES(ch)--;

  percent = GET_SKILL(ch, skill_num);
  percent += MIN(MAXGAIN(ch), MAX(MINGAIN(ch), int_app[GET_INT(ch)].learn));

  SET_SKILL(ch, skill_num, MIN(LEARNED(ch), percent));

  if (GET_SKILL(ch, skill_num) >= LEARNED(ch))
    send_to_char("You have learned all that can be taught about this.\r\n", ch);

  return 1;
}


SPECIAL(dump)
{
  struct obj_data *k;

  ACMD(do_drop);
  char *fname(char *namelist);

  for (k = world[ch->in_room].contents; k; k = world[ch->in_room].contents) {
    act("$p vanishes in a puff of smoke!", FALSE, 0, k, 0, TO_ROOM);
    extract_obj(k);
  }

  if (!CMD_IS("drop"))
    return 0;

  do_drop(ch, argument, cmd, 0);

  for (k = world[ch->in_room].contents; k; k = world[ch->in_room].contents) {
    act("$p vanishes in a puff of smoke!", FALSE, 0, k, 0, TO_ROOM);
    extract_obj(k);
  }

  return 1;
}


SPECIAL(mayor)
{
  ACMD(do_gen_door);

  static char open_path[] =
  "W3a3003b33000c111d0d111Oe333333Oe22c222112212111a1S.";

  static char close_path[] =
  "W3a3003b33000c111d0d111CE333333CE22c222112212111a1S.";

  static char *path;
  static int index;
  static bool move = FALSE;

  if (!move) {
    if (time_info.hours == 6) {
      move = TRUE;
      path = open_path;
      index = 0;
    } else if (time_info.hours == 20) {
      move = TRUE;
      path = close_path;
      index = 0;
    }
  }
  if (cmd || !move || (GET_POS(ch) < POS_SLEEPING) ||
      (GET_POS(ch) == POS_FIGHTING))
    return FALSE;

  switch (path[index]) {
  case '0':
  case '1':
  case '2':
  case '3':
    perform_move(ch, path[index] - '0', 1);
    break;

  case 'W':
    GET_POS(ch) = POS_STANDING;
    act("$n awakens and groans loudly.", FALSE, ch, 0, 0, TO_ROOM);
    break;

  case 'S':
    GET_POS(ch) = POS_SLEEPING;
    act("$n lies down and instantly falls asleep.", FALSE, ch, 0, 0, TO_ROOM);
    break;

  case 'a':
    act("$n says 'Hello Honey!'", FALSE, ch, 0, 0, TO_ROOM);
    act("$n smirks.", FALSE, ch, 0, 0, TO_ROOM);
    break;

  case 'b':
    act("$n says 'What a view!  I must get something done about that dump!'",
	FALSE, ch, 0, 0, TO_ROOM);
    break;

  case 'c':
    act("$n says 'Vandals!  Youngsters nowadays have no respect for anything!'",
	FALSE, ch, 0, 0, TO_ROOM);
    break;

  case 'd':
    act("$n says 'Good day, citizens!'", FALSE, ch, 0, 0, TO_ROOM);
    break;

  case 'e':
    act("$n says 'I hereby declare the bazaar open!'", FALSE, ch, 0, 0, TO_ROOM);
    break;

  case 'E':
    act("$n says 'I hereby declare Midgaard closed!'", FALSE, ch, 0, 0, TO_ROOM);
    break;

  case 'O':
    do_gen_door(ch, "gate", 0, SCMD_UNLOCK);
    do_gen_door(ch, "gate", 0, SCMD_OPEN);
    break;

  case 'C':
    do_gen_door(ch, "gate", 0, SCMD_CLOSE);
    do_gen_door(ch, "gate", 0, SCMD_LOCK);
    break;

  case '.':
    move = FALSE;
    break;

  }

  index++;
  return FALSE;
}


/* ********************************************************************
*  General special procedures for mobiles                             *
******************************************************************** */

SPECIAL(auto_healer) {
  return FALSE;
}

void npc_steal(struct char_data * ch, struct char_data * victim)
{
  int gold;

  if (IS_NPC(victim))
    return;
  if (GET_LEVEL(victim) >= LVL_IMMORT)
    return;

  if (AWAKE(victim) && (number(0, GET_LEVEL(ch)) == 0)) {
    act("You discover that $n has $s hands in your wallet.", FALSE, ch, 0, victim, TO_VICT);
    act("$n tries to steal money from $N.", TRUE, ch, 0, victim, TO_NOTVICT);
  } else {
    int totcoins, which_coin;

    totcoins = (GET_MONEY(victim)).platinum +
      (GET_MONEY(victim)).steel +
      (GET_MONEY(victim)).gold +
      (GET_MONEY(victim)).copper;

    /* Steal some gold coins */
    gold = (int) ((totcoins * number(1, 10)) / 100);

    for(;gold>0; gold--) {
      totcoins = (GET_MONEY(victim)).platinum +
	(GET_MONEY(victim)).steel +
	(GET_MONEY(victim)).gold +
	(GET_MONEY(victim)).copper;

      which_coin = number(1, totcoins);

      if ( which_coin <= (GET_MONEY(victim)).platinum ) {
	(GET_MONEY(victim)).platinum--;
	(GET_MONEY(ch)).platinum++;
      } else if ( which_coin <= (GET_MONEY(victim)).steel ) {
	(GET_MONEY(victim)).steel--;
	(GET_MONEY(ch)).steel++;
      } else if ( which_coin <= (GET_MONEY(victim)).gold ) {
	(GET_MONEY(victim)).gold--;
	(GET_MONEY(ch)).gold++;
      } else {
	(GET_MONEY(victim)).copper--;
	(GET_MONEY(ch)).copper++;
      }

    }
  }
}


SPECIAL(snake)
{
  if (cmd)
    return FALSE;

  if (GET_POS(ch) != POS_FIGHTING)
    return FALSE;

  if (FIGHTING(ch) && (FIGHTING(ch)->in_room == ch->in_room) &&
      (number(0, 42 - GET_LEVEL(ch)) == 0)) {
    act("$n bites $N!", 1, ch, 0, FIGHTING(ch), TO_NOTVICT);
    act("$n bites you!", 1, ch, 0, FIGHTING(ch), TO_VICT);
    call_magic(ch, FIGHTING(ch), 0, SPELL_POISON, GET_LEVEL(ch), CAST_SPELL);
    return TRUE;
  }
  return FALSE;
}


SPECIAL(thief)
{
  struct char_data *cons;

  if (cmd)
    return FALSE;

  if (GET_POS(ch) != POS_STANDING)
    return FALSE;

  for (cons = world[ch->in_room].people; cons; cons = cons->next_in_room)
    if (!IS_NPC(cons) && (GET_LEVEL(cons) < LVL_IMMORT) && (!number(0, 4))) {
      npc_steal(ch, cons);
      return TRUE;
    }
  return FALSE;
}


SPECIAL(spellcaster_mage)
{
  struct char_data *vict;
  int i, chance, numspell = 0, spellnum;
  int spell_list[NUM_SPELL_FLAGS];
  extern int mob_spells_mage[NUM_SPELL_FLAGS];

  if (GET_POS(ch) != POS_FIGHTING)
    return FALSE;

  chance = number(0, 1);

  if (chance)
    return FALSE;

  for (vict = world[ch->in_room].people; vict; vict = vict->next_in_room)
    if ((FIGHTING(vict) == ch) && !number(0, 3))
      break;

  if (vict == NULL)
    vict = FIGHTING(ch);

  for (i = 0; i < NUM_SPELL_FLAGS; i++)
  {
    if (IS_SET(SPELL_FLAGS(ch), 1 << i))
    {
      spell_list[numspell] = i;
      numspell++;
    }
  }

  if (numspell == 0)
    return FALSE;

  spellnum = spell_list[number(1, numspell) - 1];

  switch (mob_spells_mage[spellnum])
  {
    case SPELL_ARMOR:
      vict = ch;
      break;
    default:
      break;
  }

  cast_spell(ch, vict, NULL, mob_spells_mage[spellnum]);

  return TRUE;
}

/* ********************************************************************
*  Special procedures for mobiles                                      *
******************************************************************** */

SPECIAL(guild_guard)
{
  int i;
  extern int guild_info[][3];
  struct char_data *guard = (struct char_data *) me;
  char *buf = "The guard humiliates you, and blocks your way.\r\n";
  char *buf2 = "The guard humiliates $n, and blocks $s way.";

  if (!IS_MOVE(cmd) || IS_AFFECTED(guard, AFF_BLIND))
    return FALSE;

  if (GET_LEVEL(ch) >= LVL_IMMORT)
    return FALSE;

  for (i = 0; guild_info[i][0] != -1; i++) {
    if ((IS_NPC(ch) || GET_CLASS(ch) != guild_info[i][0]) &&
	world[ch->in_room].number == guild_info[i][1] &&
	cmd == guild_info[i][2]) {
      send_to_char(buf, ch);
      act(buf2, FALSE, ch, 0, 0, TO_ROOM);
      return TRUE;
    }
  }

  return FALSE;
}


SPECIAL(puff)
{
  ACMD(do_say);

  if (cmd)
    return (0);

  switch (number(0, 60)) {
  case 0:
    do_say(ch, "My god!  It's full of stars!", 0, 0);
    return (1);
  case 1:
    do_say(ch, "How'd all those fish get up here?", 0, 0);
    return (1);
  case 2:
    do_say(ch, "I'm a very female dragon.", 0, 0);
    return (1);
  case 3:
    do_say(ch, "I've got a peaceful, easy feeling.", 0, 0);
    return (1);
  default:
    return (0);
  }
}


SPECIAL(fido)
{

  struct obj_data *i, *temp, *next_obj;

  if (cmd || !AWAKE(ch))
    return (FALSE);

  for (i = world[ch->in_room].contents; i; i = i->next_content) {
    if (IS_CORPSE(i)) {
      act("$n savagely devours a corpse.", FALSE, ch, 0, 0, TO_ROOM);
      for (temp = i->contains; temp; temp = next_obj) {
	next_obj = temp->next_content;
	obj_from_obj(temp);
	obj_to_room(temp, ch->in_room);
      }
      extract_obj(i);
      return (TRUE);
    }
  }
  return (FALSE);
}


SPECIAL(janitor)
{
  struct obj_data *i;

  if (cmd || !AWAKE(ch))
    return (FALSE);

  for (i = world[ch->in_room].contents; i; i = i->next_content) {
    if (!CAN_WEAR(i, ITEM_WEAR_TAKE))
      continue;
    if (GET_OBJ_TYPE(i) != ITEM_DRINKCON && GET_OBJ_COST(i) >= 15)
      continue;
    act("$n picks up some trash.", FALSE, ch, 0, 0, TO_ROOM);
    obj_from_room(i);
    obj_to_char(i, ch);
    return TRUE;
  }

  return FALSE;
}


SPECIAL(cityguard)
{
  struct char_data *tch, *evil;
  int max_evil;

  if (cmd || !AWAKE(ch) || FIGHTING(ch))
    return FALSE;

  max_evil = 1000;
  evil = 0;

  for (tch = world[ch->in_room].people; tch; tch = tch->next_in_room) {
    if ( ! IS_NPC(tch) && CAN_SEE(ch, tch) &&
	 IS_SET(PLR_FLAGS(tch), PLR_KILLER)) {
      act("$n screams 'Hey!!  You're one of those murderers!'",
	  FALSE, ch, 0, 0, TO_ROOM);
      hit(ch, tch, TYPE_UNDEFINED);
      return (TRUE);
    }
  }

  for (tch = world[ch->in_room].people; tch; tch = tch->next_in_room) {
    if (!IS_NPC(tch) && CAN_SEE(ch, tch) &&
	IS_SET(PLR_FLAGS(tch), PLR_THIEF)) {
      act("$n screams 'HEY!!!  You're one of those PLAYER THIEVES!!!!!!'",
	  FALSE, ch, 0, 0, TO_ROOM);
      hit(ch, tch, TYPE_UNDEFINED);
      return (TRUE);
    }
  }

  for (tch = world[ch->in_room].people; tch; tch = tch->next_in_room) {
    if (CAN_SEE(ch, tch) && FIGHTING(tch)) {
      if (!IS_AFFECTED(tch, AFF_CONCEAL_ALIGN) &&
          (GET_ALIGNMENT(tch) < max_evil) &&
	  (IS_NPC(tch) || IS_NPC(FIGHTING(tch)))) {
	max_evil = GET_ALIGNMENT(tch);
	evil = tch;
      }
    }
  }

  if (evil && (GET_ALIGNMENT(FIGHTING(evil)) >= 0)) {
    act("$n screams 'PROTECT THE INNOCENT!  BANZAI!  CHARGE!  ARARARAGGGHH!'",
	FALSE, ch, 0, 0, TO_ROOM);
    hit(ch, evil, TYPE_UNDEFINED);
    return (TRUE);
  }

  return (FALSE);
}


#define PET_PRICE(pet) (GET_LEVEL(pet) * 300)

SPECIAL(pet_shops)
{
  char buf[MAX_STRING_LENGTH], pet_name[256];
  int pet_room;
  struct char_data *pet;

  pet_room = ch->in_room + 1;

  if (CMD_IS("list")) {
    send_to_char("Available pets are:\r\n", ch);
    for (pet = world[pet_room].people; pet; pet = pet->next_in_room) {
      GET_NAME(pet, chname);
      sprintf(buf, "%8d - %s\r\n", PET_PRICE(pet), chname);
      FREE_NAME(chname);
      send_to_char(buf, ch);
    }
    return (TRUE);
  } else if (CMD_IS("buy")) {

    argument = one_argument(argument, buf);
    argument = one_argument(argument, pet_name);

    if (!(pet = get_char_room(buf, pet_room))) {
      send_to_char("There is no such pet!\r\n", ch);
      return (TRUE);
    }

    if (money_to_copper(GET_MONEY(ch)) < PET_PRICE(pet)) {
      send_to_char("You don't have enough money!\r\n", ch);
      return (TRUE);
    }
    {
      struct money_data sub;

      sub.platinum = 0;      sub.steel = 0;      sub.gold = 0;
      sub.copper = PET_PRICE(pet);
      money_subtract( &(GET_MONEY(ch)), sub);
    }

    pet = read_mobile(GET_MOB_RNUM(pet), REAL);
    GET_EXP(pet) = 0;
    SET_BIT(AFF_FLAGS(pet), AFF_CHARM);

    if (*pet_name) {
      sprintf(buf, "%s %s", pet->player.name, pet_name);
      /* free(pet->player.name); don't free the prototype! */
      pet->player.name = str_dup(buf);

      sprintf(buf, "%sA small sign on a chain around the neck says 'My name is %s'\r\n",
	      pet->player.description, pet_name);
      /* free(pet->player.description); don't free the prototype! */
      pet->player.description = str_dup(buf);
    }
    char_to_room(pet, ch->in_room);
    add_follower(pet, ch);

    /* Be certain that pets can't get/carry/use/wield/wear items */
    IS_CARRYING_W(pet) = 1000;
    IS_CARRYING_N(pet) = 100;

    send_to_char("May you enjoy your pet.\r\n", ch);
    act("$n buys $N as a pet.", FALSE, ch, 0, pet, TO_ROOM);

    return 1;
  }
  /* All commands except list and buy */
  return 0;
}

#define MOUNT_PRICE 1000
/*
SPECIAL(stable)
{

  struct char_data *mount = NULL;
  struct follow_type *f;

  int horses[] = {1636, 3910, 4564, 4565, 6034, 6036, 8518, 12119, 12187, 12382, 19023};

  if (CMD_IS("buy")) {
    if (money_to_copper(GET_MONEY(ch)) < MOUNT_PRICE) {                                                                  
      send_to_char("You don't have enough money!\r\n", ch);                                                                 
      return (TRUE);                                                                                                        
    }                                                                                                                       
    {                                                                                                                       
      struct money_data sub;                                                                                                
                                                                                                                            
      sub.platinum = 0;      sub.steel = 0;      sub.gold = 0;                                                              
      sub.copper = MOUNT_PRICE;                                                                                          
    }

    if (money_to_copper(GET_MONEY(ch)) < MOUNT_PRICE) {
      send_to_char("You don't have enough money!\r\n", ch);
      return (TRUE);
    }
    {
      struct money_data sub;

      sub.platinum = 0;      sub.steel = 0;      sub.gold = 0;
      sub.copper = MOUNT_PRICE;
      money_subtract( &(GET_MONEY(ch)), sub);
    }
    
    mount = read_mobile(horses[dice(1, sizeof(horses)) - 1], VIRTUAL);

    if (mount == NULL)
      RETURN FALSE;

    add_follower(ch, mount);

    return TRUE;
  }
  else if (CMD_IS("sell")) {
    for (mount = ch->followers->follower; ch->followers; mount = ch->followers->next) {
      if (IS_NPC(mount) && MOB_FLAGGED(mount, MOB_MOUNTABLE))
      stop_follower(mount);  
      char_from_room(mount);
    }
    return TRUE;
  }
  return FALSE;
}
*/

/* ********************************************************************
*  Special procedures for objects                                     *
******************************************************************** */


SPECIAL(bank)
{
  struct money_data amount;


  if (CMD_IS("balance")) {
    if (GET_BANK_GOLD(ch) > 0) {
      struct money_data money;

      copper_to_money(&money, GET_BANK_GOLD(ch));
      sprintf(buf, "Your current balance is %s.\r\n", describe_money(money));

    } else {
      sprintf(buf, "You currently have no money deposited.\r\n");
    }

    send_to_char(buf, ch);
    return 1;

  } else if (CMD_IS("deposit")) {

    argument = one_argument(argument, arg);

    amount.platinum = amount.steel = amount.gold = amount.copper = 0;

    while( *arg ) {
      if ( is_money(arg) ) {
	struct money_data tmpAmt;

	tmpAmt.platinum = tmpAmt.steel = tmpAmt.gold = tmpAmt.copper = 0;
	value_money(&tmpAmt, arg);

	amount.platinum += tmpAmt.platinum;
	amount.steel += tmpAmt.steel;
	amount.gold += tmpAmt.gold;
	amount.copper += tmpAmt.copper;

      } else {
	/*  trying money and something else...  */
	send_to_char("Sorry, you can't do that to money and something else at the same time.\r\n", ch);
	return 1;
      }

      argument = one_argument(argument, arg);
    }

    if ( money_to_copper(amount) <= 0 ) {
      send_to_char("How much do you want to deposit?\r\n", ch);
      return 1;
    }

    if ( money_compare(GET_MONEY(ch), amount, TRUE) < 0 ) {
      send_to_char("You don't have that many coins!\r\n", ch);
      return 1;
    }

    money_subtract( &(GET_MONEY(ch)), amount);

    GET_BANK_GOLD(ch) += money_to_copper(amount);

    sprintf(buf, "You deposit %s.\r\n", describe_money(amount));
    send_to_char(buf, ch);
    act("$n makes a bank transaction.", TRUE, ch, 0, FALSE, TO_ROOM);

    return 1;

  } else if (CMD_IS("withdraw")) {

    argument = one_argument(argument, arg);

    amount.platinum = amount.steel = amount.gold = amount.copper = 0;

    while( *arg ) {
      if ( is_money(arg) ) {
	struct money_data tmpAmt;

	tmpAmt.platinum = tmpAmt.steel = tmpAmt.gold = tmpAmt.copper = 0;
	value_money( &tmpAmt, arg);

	amount.platinum += tmpAmt.platinum;
	amount.steel += tmpAmt.steel;
	amount.gold += tmpAmt.gold;
	amount.copper += tmpAmt.copper;

      } else {
	/*  trying money and something else...  */
	send_to_char("Sorry, you can't do that to money and something else at the same time.\r\n", ch);
	return 1;
      }
      argument = one_argument(argument, arg);
    }

    if ( money_to_copper(amount) <= 0 ) {
      send_to_char("How much do you want to withdraw?\r\n", ch);
      return 1;
    }

    /*  no longer need to withdraw based on "exact" coins in bank
     *  ie.  will convert to any denomination  */
    if ( GET_BANK_GOLD(ch) < money_to_copper(amount) ) {
      send_to_char("You don't have that much money deposited!\r\n", ch);
      return 1;
    }

    GET_BANK_GOLD(ch) -= money_to_copper(amount);

    (GET_MONEY(ch)).platinum += amount.platinum;
    (GET_MONEY(ch)).steel += amount.steel;
    (GET_MONEY(ch)).gold += amount.gold;
    (GET_MONEY(ch)).copper += amount.copper;

    sprintf(buf, "You withdraw %s.\r\n", describe_money(amount));
    send_to_char(buf, ch);
    act("$n makes a bank transaction.", TRUE, ch, 0, FALSE, TO_ROOM);
    return 1;

  } else {
    return 0;
  }
}

SPECIAL(cleric)
{
  struct char_data *vict;

  /*  3500  13th  cure light
   *  3501  15th  cure serious
   *  3502  16th  heal
   *  3506  17th  heal/blind
   *  3507  17th  heal/blind
   *  3508  15th  fireball
   *  3513  17th  heal/blind
   *  3517  28th  heal/harm/blind
   */

  if (cmd || GET_POS(ch) != POS_FIGHTING) {
    return FALSE;
  }

  if ( GET_MOB_WAIT(ch) > 0 ) {
    /*  wait for spell lags  */
    return FALSE;
  }

  /* pseudo-randomly choose someone in the room who is fighting me */
  for (vict = world[ch->in_room].people; vict; vict = vict->next_in_room) {
    if (FIGHTING(vict) == ch && !number(0, 4)) {
      break;
    }
  }

  /* if I didn't pick any of those, then just slam the guy I'm fighting */
  if (vict == NULL) {
    vict = FIGHTING(ch);
  }

  return TRUE;

}

SPECIAL(berserker)
{
  int i = 0, j;
  struct char_data *vict, *targ = NULL;

  if (IS_NPC(ch) || (GET_LEVEL(ch) >= LVL_IMMORT))
    return FALSE;

  if (number(0, 4))
    return FALSE;

  for (vict = world[ch->in_room].people; vict; vict = vict->next_in_room)
  {
    if (!vict || IS_NPC(vict) || GET_LEVEL(vict) >= LVL_IMMORT)
      continue;

    if (number(0, 2))
    {
      targ = vict;
      break;
    }

    i++;
  }

  if (i == 0)
    return FALSE;

  if (!targ)
  {
    j = number(1, i);
    i = 0;
    vict = world[ch->in_room].people;
    while (i < j)
    {
      if (!vict)
        return FALSE;
      if (IS_NPC(vict) || GET_LEVEL(vict) >= LVL_IMMORT)
        i--;
      i++;
      if (i < j)
        vict = vict->next_in_room;
    }

    if (vict)
      targ = vict;
    else
      return FALSE;
  }

  if (targ)
  {
    hit(ch, targ, TYPE_UNDEFINED);
    return TRUE;
  }

  return FALSE;
}
