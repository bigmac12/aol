/* ************************************************************************
*   File: class.c                                       Part of CircleMUD *
*  Usage: Source file for class-specific code                             *
*                                                                         *
*  All rights reserved.  See license.doc for complete information.        *
*                                                                         *
*  Copyright (C) 1993, 94 by the Trustees of the Johns Hopkins University *
*  CircleMUD is based on DikuMUD, Copyright (C) 1990, 1991.               *
************************************************************************ */

/*
 * This file attempts to concentrate most of the code which must be changed
 * in order for new classes to be added.  If you're adding a new class,
 * you should go through this entire file from beginning to end and add
 * the appropriate new special cases for your new class.
 */

/* Completely gutted, and reworked
 * --gan, 07/20/2001 (Nuitari)
 */

#include "conf.h"
#include "sysdep.h"

#include "structs.h"
#include "db.h"
#include "handler.h"
#include "utils.h"
#include "spells.h"
#include "interpreter.h"

extern int siteok_everyone;
extern const char *pc_race_types[];
extern struct spell_info_type spell_info[];
extern char *spells[];

/* external functions */
void race_affects(struct char_data *ch);
int num_slots(struct char_data *ch, int spell_level);
int spell_access(struct char_data *ch, int spellnum);
int caster_level(struct char_data *ch, int level);

// This function is actually defined in utils.h, but
// I added this to remove the 'implicit declaration'
// warning.
// -- Branc, 12/27/11
void send_to_char(char* message, struct char_data *ch);

char *class_display[NUM_CLASSES] = {
  " [K]night\r\n",
  " [P]aladin\r\n",
  " [M]age\r\n",
  " [C]leric\r\n",
  " [R]anger\r\n",
  " R[O]gue\r\n",
  " [W]arrior\r\n",
  " [A]dventurer\r\n",
  " [D]ruid\r\n",
  " Mo[N]k\r\n",
  " [B]ard\r\n",
  " [S]orceror\r\n",
  " D[E]fender\r\n",
  " Filler[2]\r\n",
  " Filler[3]\r\n"
};

/* Names first */

const char *class_abbrevs[] = {
   "Kn",
   "Pa",
   "Ma",
   "Cl",
   "Ra",
   "Th",
   "Wa",
   "Ad",
   "Dr",
   "Mk",
   "Ba",
   "So",
   "De",
   "F2",
   "F3",
   "\n"
};


const char *pc_class_types[] = {
   "Knight",
   "Paladin",
   "Mage",
   "Cleric",
   "Ranger",
   "Rogue",
   "Warrior",
   "Adventurer",
   "Druid",
   "Monk",
   "Bard",
   "Sorceror",
   "Defender",
   "Filler2",
   "Filler3",
   "\n"
};


/*  PDH  1/12/99 - min ability scores for each class  */
int class_abil_mins[NUM_CLASSES][6] = {
  {  9,  3,  3,  3,  9,  3 }, /* Kni */
  {  9,  3,  9,  9,  3,  3 }, /* Pal */
  {  3,  9,  3,  3,  3,  3 }, /* Mag */
  {  3,  9,  9,  3,  3,  3 }, /* Cle */
  {  3,  9,  3,  3,  9,  3 }, /* Ran */
  {  3,  3,  3,  9,  3,  3 }, /* Rog */
  {  9,  3,  3,  3,  3,  3 }, /* War */
  {  3,  3,  3,  3,  3,  3 }, /* Adv */
  {  3,  3,  9,  3,  3,  3 }, /* Dru */
  {  3,  3,  9,  9,  3,  3 }, /* Mon */
  {  3,  3,  3,  3,  3,  3 }, /* Bar */
  {  3,  3,  3,  3,  3,  9 }, /* Sor */
  {  9,  3,  3,  3,  9,  3 }, /* Def */
  {  3,  3,  3,  3,  3,  3 }, /* Fi2 */
  {  3,  3,  3,  3,  3,  3 }, /* Fi3 */

};

#define Y TRUE
#define N FALSE

int class_ok_race[NUM_RACES][NUM_CLASSES] = {
         /*  Kn Pa Ma Cl Ra Th Wa Ad Dr Mk Ba So De F2 F3 */
  /* Bar */ { N, N, N, N, Y, Y, Y, N, N, N, N, N, N, N, N },
  /* Hyl */ { N, N, N, Y, N, Y, Y, N, N, N, N, N, Y, N, N },
  /* Dae */ { N, N, N, Y, N, Y, Y, N, N, N, N, N, Y, N, N },
  /* Nei */ { N, N, N, Y, N, Y, Y, N, N, N, N, N, Y, N, N },
  /* Sil */ { N, N, Y, Y, Y, Y, Y, N, Y, N, N, N, N, N, N },
  /* Qua */ { N, N, Y, Y, Y, Y, Y, N, Y, N, N, N, N, N, N },
  /* Kag */ { N, N, N, Y, Y, Y, Y, N, Y, N, N, N, N, N, N },
  /* 1/2 */ { N, N, Y, Y, Y, Y, Y, N, Y, N, N, N, N, N, N },
  /* Ken */ { N, N, N, Y, N, Y, Y, N, N, N, N, N, N, N, N },
  /* Min */ { N, N, N, Y, N, N, Y, N, N, N, N, N, N, N, N },
  /* Gno */ { N, N, N, Y, N, Y, Y, N, N, N, N, N, N, N, N },
  /* Hum */ { Y, N, Y, Y, Y, Y, Y, N, Y, Y, N, N, N, N, N } 

};


/*  PDH  1/12/99
 *  see if the PC's stats are sufficient for the class chosen
 */
int class_ok_stats(struct char_data *ch, int class)
{
  /* int class_abil_mins[NUM_CLASSES][6]; */

   if((ch->real_abils.str < class_abil_mins[class][0]) ||
      (ch->real_abils.intel < class_abil_mins[class][1]) ||
      (ch->real_abils.wis < class_abil_mins[class][2]) ||
      (ch->real_abils.dex < class_abil_mins[class][3]) ||
      (ch->real_abils.con < class_abil_mins[class][4]) ||
      (ch->real_abils.cha < class_abil_mins[class][5]))
   {
       return 0;
   }
   return 1;
}

/*
 * The code to interpret a class letter -- used in interpreter.c when a
 * new character is selecting a class and by 'set class' in act.wizard.c.
 */
/*int parse_class(char arg)*/
int parse_class(struct char_data *ch, char arg)
{
   int class = CLASS_UNDEFINED;
   arg = LOWER(arg);
   if(arg == 'z')
   {
      return CLASS_NEWRACE;
   }
   switch (arg)
   {
      case 'c':
         class = CLASS_CLERIC;
         break;
      case 'd':
         class = CLASS_DRUID;
         break;
      case 'k':
         class = CLASS_KNIGHT;
         break;
      case 'm':
         class = CLASS_MAGE;
         break;
      case 'p':
         class = CLASS_PALADIN;
         break;
      case 'r':
         class = CLASS_RANGER;
         break;
      case 't':
      case 'o':
         class = CLASS_ROGUE;
         break;
      case 'a':
         class = CLASS_ADVENTURER;
         break;
      case 'w':
         class = CLASS_WARRIOR;
         break;
      case 'n':
         class = CLASS_MONK;
         break;
      case 'b':
         class = CLASS_BARD;
         break;
      case 's':
         class = CLASS_SORCEROR;
      case 'e':
         class = CLASS_DEFENDER;
         break;

      default:
         class = CLASS_UNDEFINED;
         break;
   }

   if(class == CLASS_ADVENTURER)
   {
      return class;
   }
   if(class >= 0 && class < NUM_CLASSES)
   {
      if(!class_ok_race[(int) GET_RACE(ch)][class])
      {
         return CLASS_BAD_RACE;
      }
   }
   return (class);
}

/*
 * bitvectors (i.e., powers of two) for each class, mainly for use in
 * do_who and do_users.  Add new classes at the end so that all classes
 * use sequential powers of two (1 << 0, 1 << 1, 1 << 2, 1 << 3, 1 << 4,
 * 1 << 5, etc.
 */

long find_class_bitvector(char arg)
{
   arg = LOWER(arg);
   switch (arg)
   {
      case 'a':
         return (1 << 0);
         break;
      case 'c':
         return (1 << 1);
         break;
      case 'd':
         return (1 << 2);
         break;
      case 'k':
         return (1 << 3);
         break;
      case 'm':
         return (1 << 4);
         break;
      case 'p':
         return (1 << 5);
         break;
      case 'r':
         return (1 << 6);
         break;
      case 't':
         return (1 << 7);
         break;
      case 'w':
         return (1 << 8);
         break;
      case 'n':
         return (1 << 9);
         break;
      case 'b':
         return (1 << 10);
         break;
      case 's':
         return (1 << 11);
         break;
      case 'e':
         return (1 << 12);
         break;

      default:
         return 0;
         break;
   }
}


/*
 * These are definitions which control the guildmasters for each class.
 *
 * The first field (top line) controls the highest percentage skill level
 * a character of the class is allowed to attain in any skill.  (After
 * this level, attempts to practice will say "You are already learned in
 * this area."
 * 
 * The second line controls the maximum percent gain in learnedness a
 * character is allowed per practice -- in other words, if the random
 * die throw comes out higher than this number, the gain will only be
 * this number instead.
 *
 * The third line controls the minimu percent gain in learnedness a
 * character is allowed per practice -- in other words, if the random
 * die throw comes out below this number, the gain will be set up to
 * this number.
 * 
 * The fourth line simply sets whether the character knows 'spells'
 * or 'skills'.  This does not affect anything except the message given
 * to the character when trying to practice (i.e. "You know of the
 * following spells" vs. "You know of the following skills"
 */


int prac_params[3][NUM_CLASSES] = {
  /* Knt Pal Mag Cle Ran Thi War Adv Dru Mon Bar Sor Def Fi2 Fi3 */
  {   70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70 },  /* learned level*/
  {   35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35 },  /* max per prac */
  {    5,  5,  5,  5,  5,  5,  5,  0,  5,  5,  5,  5,  5,  5,  5 },  /* min per prac */
};


int guild_info[][3] = {

   /* Kendermore */
   {CLASS_WARRIOR,       2413,   SCMD_SOUTH},

   /* this must go last -- add new guards above! */
   {-1, -1, -1}
};




/* THAC0 for classes and levels.  (To Hit Armor Class 0) */

/* [class], [level] (all) */
const int thaco[NUM_CLASSES][LVL_IMPL + 1] = {

/* KNIGHT */
  {100, 20, 19, 18, 18, 17, 17, 16, 16, 15, 15, 14, 14, 13, 12, 11, /*0-15*/
        11, 10, 10,  9,  8,  7,  6,  5,  4,  3,  2,  1,  0,  0,  0, /*16-30*/
         0,  0,  0,  0,  0,  0,  0,  0},                            /*31-38*/

/* PALADIN */
  {100, 20, 19, 18, 18, 17, 17, 16, 16, 15, 15, 14, 14, 13, 13, 12, /*0-15*/
        12, 11, 10,  9,  8,  7,  6,  5,  4,  3,  2,  1,  0,  0,  0, /*16-30*/
         0,  0,  0,  0,  0,  0,  0,  0},                            /*31-38*/

/* MAGE */
  {100, 20, 20, 19, 19, 19, 18, 18, 18, 17, 17, 16, 16, 15, 15, 14, /*0-15*/
        14, 14, 13, 13, 13, 13, 12, 12, 11, 11, 11, 11, 10, 10, 10, /*16-30*/
         0,  0,  0,  0,  0,  0,  0,  0},                            /*31-38*/

/* CLERIC */
  {100, 20, 19, 19, 18, 18, 17, 17, 17, 16, 15, 15, 14, 14, 13, 13, /*0-15*/
        12, 12, 11, 11, 10, 10,  9,  8,  7,  6,  6,  5,  5,  5,  4, /*16-30*/
         0,  0,  0,  0,  0,  0,  0,  0},                            /*31-38*/

/* RANGER */
  {100, 20, 19, 18, 18, 17, 17, 16, 16, 15, 15, 14, 14, 13, 12, 11, /*0-15*/
        11, 10, 10, 9,  8,  7,  6,  5,  4,  3,  2,  1,  0,  0,  0,  /*16-30*/
         0,  0,  0,  0,  0,  0,  0,  0},                            /*31-38*/

/* ROGUE */
  {100, 20, 19, 19, 18, 18, 17, 17, 16, 16, 15, 15, 14, 14, 13, 13, /*0-15*/
        12, 11, 10, 10,  9,  8,  8,  7,  6,  5,  5,  4,  4,  3,  2, /*16-30*/
         0,  0,  0,  0,  0,  0,  0,  0},                            /*31-38*/

/* WARRIOR */
  {100, 20, 19, 18, 18, 17, 17, 16, 16, 15, 15, 14, 14, 13, 12, 11, /*0-15*/
        11, 10, 10,  9,  9,  8,  8,  7,  6,  5,  4,  3,  2,  1,  0, /*16-30*/
         0,  0,  0,  0,  0,  0,  0,  0},                            /*31-38*/

/* ADVENTURER */
  {100, 20, 20, 19, 19, 19, 18, 18, 18, 17, 17, 16, 16, 15, 15, 14, /*0-15*/
        14, 14, 13, 13, 13, 13, 12, 12, 11, 11, 11, 11, 10, 10, 10, /*16-30*/
         0,  0,  0,  0,  0,  0,  0,  0},                            /*31-38*/

/* DRUID */
  {100, 20, 19, 19, 18, 18, 17, 17, 16, 16, 15, 15, 14, 14, 13, 12, /*0-15*/
        11, 11, 10,  9,  9,  9,  8,  7,  7,  7,  6,  6,  5,  5,  4, /*16-30*/
         0,  0,  0,  0,  0,  0,  0,  0},                            /*31-38*/

/* MONK */
  {100, 17, 16, 15, 14, 13, 12, 12, 11, 11, 10, 10, 9, 9, 8, 8,     /*0-15*/
        7,   7,  6,  6,  5,  5,  4,  3,  2,  1,  0, 0, 0, 0, 0,     /*16-30*/
        0,   0,  0,  0,  0,  0,  0,  0},                            /*31-38*/

/* BARD */
  {100, 20, 19, 19, 18, 18, 17, 17, 16, 16, 15, 15, 14, 14, 13, 12, /*0-15*/
        11, 11, 10,  9,  9,  9,  8,  7,  7,  7,  6,  6,  5,  5,  4, /*16-30*/
         0,  0,  0,  0,  0,  0,  0,  0},                            /*31-38*/

/* SORCEROR */
  {100, 20, 19, 19, 18, 18, 17, 17, 16, 16, 15, 15, 14, 14, 13, 12, /*0-15*/
        11, 11, 10,  9,  9,  9,  8,  7,  7,  7,  6,  6,  5,  5,  4, /*16-30*/
         0,  0,  0,  0,  0,  0,  0,  0},                            /*31-38*/

/* DEFENDER */
  {100, 20, 19, 18, 18, 17, 17, 16, 16, 15, 15, 14, 14, 13, 12, 11, /*0-15*/
        11, 10, 10,  9,  8,  7,  6,  5,  4,  3,  2,  1,  0,  0,  0, /*16-30*/
         0,  0,  0,  0,  0,  0,  0,  0},                            /*31-38*/

/* FILLER2 */
  {100, 20, 19, 19, 18, 18, 17, 17, 16, 16, 15, 15, 14, 14, 13, 12, /*0-15*/
        11, 11, 10,  9,  9,  9,  8,  7,  7,  7,  6,  6,  5,  5,  4, /*16-30*/
         0,  0,  0,  0,  0,  0,  0,  0},                            /*31-38*/

/* FILLER3 */
  {100, 20, 19, 19, 18, 18, 17, 17, 16, 16, 15, 15, 14, 14, 13, 12, /*0-15*/
        11, 11, 10,  9,  9,  9,  8,  7,  7,  7,  6,  6,  5,  5,  4, /*16-30*/
         0,  0,  0,  0,  0,  0,  0,  0}                             /*31-38*/

};


/* Some initializations for characters, including initial skills */
void do_start(struct char_data * ch) {
  struct obj_data * objNew, * objCont;

  void advance_level(struct char_data * ch);
  void obj_to_char(struct obj_data *object, struct char_data *ch);
  void equip_char(struct char_data *ch, struct obj_data *obj, int pos);

  //GET_LEVEL(ch) = 1;
  ch->player.level = 1;
  GET_EXP(ch) = 1;

  set_title(ch, NULL);
  ch->points.max_hit = 10;
  race_affects(ch);

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

  if(GET_HOME(ch) == 2) {
    obj_to_char(read_object(5007, VIRTUAL), ch);                 /* Kar map  */ 
  } else {
    obj_to_char(read_object(10000, VIRTUAL), ch);                /* Kal map  */
  }

  equip_char(ch, read_object( 2487, VIRTUAL), WEAR_LEGS);
  equip_char(ch, read_object( 2488, VIRTUAL), WEAR_FEET);
  equip_char(ch, read_object( 2489, VIRTUAL), WEAR_ABOUT);

  switch (GET_CLASS(ch)) {
  case CLASS_KNIGHT:
  case CLASS_PALADIN:
  case CLASS_RANGER:
  case CLASS_DEFENDER:
  case CLASS_WARRIOR:
    equip_char(ch, read_object( 2485, VIRTUAL), WEAR_WIELD);
    equip_char(ch, read_object( 2486, VIRTUAL), WEAR_BODY);
    break;

  case CLASS_MAGE:
    equip_char(ch, read_object( 2493, VIRTUAL), WEAR_WIELD);
    equip_char(ch, read_object( 2492, VIRTUAL), WEAR_BODY);
    break;

  case CLASS_CLERIC:
  case CLASS_DRUID:
    if (GET_CLASS(ch) == CLASS_CLERIC)
    {
      obj_to_char(read_object(25, VIRTUAL), ch); /* Priestly symbol */
    }
    equip_char(ch, read_object( 2491, VIRTUAL), WEAR_WIELD);
    equip_char(ch, read_object( 2492, VIRTUAL), WEAR_BODY);
    break;

  case CLASS_ROGUE:
    equip_char(ch, read_object( 2494, VIRTUAL), WEAR_WIELD);
    equip_char(ch, read_object( 2495, VIRTUAL), WEAR_BODY);
    break;

  case CLASS_ADVENTURER:
  case CLASS_MONK:
     equip_char(ch, read_object( 10198, VIRTUAL), WEAR_BODY);
    break;

  default:
    log("SYSERR: unknown class type in do_start()");
    break;
  }

  advance_level(ch);

  (GET_MONEY(ch)).platinum = 0;
  (GET_MONEY(ch)).steel = 0;
  (GET_MONEY(ch)).gold = 15;
  (GET_MONEY(ch)).copper = 50;

  GET_HIT(ch) = GET_MAX_HIT(ch);
  GET_MOVE(ch) = GET_MAX_MOVE(ch);

  GET_COND(ch, THIRST) = 24;
  GET_COND(ch, FULL) = 24;
  GET_COND(ch, DRUNK) = 0;

  switch (GET_CLASS(ch))  // for initial skills - different than the eq lists
  {
    case CLASS_PALADIN:
      SET_SKILL(ch, SKILL_LAY_HANDS, 100);
      break;
    case CLASS_ROGUE:
      SET_SKILL(ch, SKILL_SNEAK, 10);
      break;
    case CLASS_CLERIC:
      SET_SKILL(ch, SKILL_BANDAGE, 75);
      SET_SKILL(ch, SKILL_READ_MAGIC, 25);
      break;
    case CLASS_MAGE:
      SET_SKILL(ch, SKILL_READ_MAGIC, 40);
      break;
    default:
      break;
  }

  ch->player.time.played = 0;
  ch->player.time.logon = time(0);

  if (siteok_everyone)
    SET_BIT(PLR_FLAGS(ch), PLR_SITEOK);
}



void advance_level(struct char_data * ch)
{
  int add_hp = 0, add_move = 0;
  int i, j;
  int add_prac = 0;
  int num_of_slots = 0;
  struct follow_type *f;
  struct char_data *k;
  int count = 0;

  if (IS_SET(AFF_FLAGS(ch), AFF_GROUP)) {
    if (ch->master || ch->followers) {
      if (ch->master)
        k = ch->master;
      else
        k = ch;

      count = 1;


      for (f = k->followers; f; f = f->next) {
        count++;
      }
 
      for (f = k->followers; f; f = f->next) {
        if (count <= 1)
          continue;
        GET_EXP_MULT(f->follower) = count * 10;
      }
       
      if (count > 1)
        GET_EXP_MULT(k) = count * 10;
    }
  }

  char message[MAX_STRING_LENGTH];

  extern struct wis_app_type wis_app[];
  extern struct con_app_type con_app[];

  add_hp = con_app[GET_CON(ch)].hitp;

  switch (GET_CLASS(ch)) {

  case CLASS_MAGE:
  case CLASS_SORCEROR:
  case CLASS_ADVENTURER:
    add_hp += number(3, 8);
    add_move = number(0, 2);
    break;

  case CLASS_CLERIC:
    add_hp += number(5, 10);
    add_move = number(1, 2);
    break;

  case CLASS_DRUID:
    add_hp += number(5, 10);
    add_move = number(1, 2);
    break;

  case CLASS_MONK:
    add_hp += number(8, 14);
    add_move = number(3, 5);
    break;

  case CLASS_ROGUE:
    add_hp += number(6, 11);
    add_move = number(1, 3);
    break;
   
   case CLASS_BARD:
    add_hp += number(7, 12);
    add_move = number(2, 4);
    break;

  case CLASS_RANGER:
    add_hp += number(10, 14);
    add_move = number(1, 3);
    break;

  case CLASS_WARRIOR:
    add_hp += number(10, 14);
    add_move = number(1, 3);
    break;

  case CLASS_PALADIN:
    add_hp += number(10, 14);
    add_move = number(1, 3);
    break;

  case CLASS_KNIGHT:
    add_hp += number(10, 14);
    add_move = number(1, 3);
    break;

  case CLASS_DEFENDER:
    add_hp += number(16, 22);
    add_move = number(0, 2);
    break;

  default:
    log("SYSERR: unknown class in advance_level()");
    break;
  }

  if (GET_RACE(ch) == RACE_MINOTAUR) 
   add_hp += number(2, 3);

  ch->points.max_hit += MAX(1, add_hp);
  ch->points.max_move += MAX(1, add_move);


  if ( (GET_CLASS(ch) == CLASS_MAGE) ||
       (GET_CLASS(ch) == CLASS_CLERIC) ||
       (GET_CLASS(ch) == CLASS_MONK) ||
       (GET_CLASS(ch) == CLASS_BARD) ||
       (GET_CLASS(ch) == CLASS_SORCEROR) ||
       (GET_CLASS(ch) == CLASS_DRUID) ) {
    add_prac += 2 + MAX(2, wis_app[GET_WIS(ch)].bonus);
  } else {
    add_prac += MAX(2, wis_app[GET_WIS(ch)].bonus);
  }

  GET_PRACTICES(ch) += add_prac;

  if (GET_LEVEL(ch) >= LVL_IMMORT) {
    for (i = 0; i < 3; i++) {
      GET_COND(ch, i) = (char) -1;
    }

    SET_BIT(PRF_FLAGS(ch), PRF_HOLYLIGHT);
  }

  sprintf(message, "&YYou are now a level %d %s %s.&n\r\n", 
          GET_LEVEL(ch), pc_race_types[(int)GET_RACE(ch)], pc_class_types[(int)GET_CLASS(ch)]);
  sprintf(message, "%s&YYou have gained %d hit points.&n\r\n", message, add_hp);
  sprintf(message, "%s&YYou have gained %d move points.&n\r\n", message, add_move);
  sprintf(message, "%s&YYou have gained %d practices.&n\r\n", message, add_prac);

// Print off new spell slots
  for (j = 1; j <= caster_level(ch, GET_LEVEL(ch)); j++) {
    num_of_slots = num_slots(ch, j);
    GET_MEM_LEVEL_FLAG(ch) = 1;
    num_of_slots -= num_slots(ch, j);
    GET_MEM_LEVEL_FLAG(ch) = 0;
    if (num_of_slots > 0)
      sprintf(message, "%s&YYou have gained %d level %d spell slots.&n\r\n", message, num_of_slots, j);
    num_of_slots = 0;
  }

  for (j = 1; j <= MAX_SKILLS; j++) {
    if (spell_info[j].min_level[(int)GET_CLASS(ch)] == GET_LEVEL(ch) && spell_access(ch, j))
      sprintf(message, "%s&YYou have gained the %s %s.&n\r\n", message, spells[j], (j > MAX_SPELLS) ?
              "skill" : "spell");
  }

  send_to_char(message, ch);

  //GET_TRUST_LEVEL(ch) = GET_LEVEL(ch);
  ch->player_specials->saved.trust = GET_LEVEL(ch);
  save_char(ch, NOWHERE);
}


/*
 * This simply calculates the backstab multiplier based on a character's
 * level.  This used to be an array, but was changed to be a function so
 * that it would be easier to add more levels to your MUD.  This doesn't
 * really create a big performance hit because it's not used very often.
 */
int backstab_mult(int level)
{
  if (level <= 0)
    return 1;	  /* level 0 */
  else if (level <= 7)
    return 2;	  /* level 1 - 7 */
  else if (level <= 13)
    return 3;	  /* level 8 - 13 */
  else if (level <= 20)
    return 4;	  /* level 14 - 20 */
  else if (level <= 28)
    return 5;	  /* level 21 - 28 */
  else if (level < LVL_IMMORT)
    return 6;	  /* all remaining mortal levels */
  else
    return 20;	  /* immortals */
}


int invalid_class(struct char_data *ch, struct obj_data *obj)
{
   if(IS_OBJ_STAT(obj, ITEM_ANTI_MAGE) && IS_MAGE(ch))
   {
      return 1;
   }
   if(IS_OBJ_STAT(obj, ITEM_ANTI_KNIGHT) && IS_KNIGHT(ch))
   {
      return 1;
   }
   if(IS_OBJ_STAT(obj, ITEM_ANTI_PALADIN) && IS_PALADIN(ch))
   {
      return 1;
   }
   if(IS_OBJ_STAT(obj, ITEM_ANTI_CLERIC) && IS_CLERIC(ch))
   {
      return 1;
   }
   if(IS_OBJ_STAT(obj, ITEM_ANTI_RANGER) && IS_RANGER(ch))
   {
      return 1;
   }
   if(IS_OBJ_STAT(obj, ITEM_ANTI_ROGUE) && IS_ROGUE(ch))
   {
     return 1;
   }
   if(IS_OBJ_STAT(obj, ITEM_ANTI_WARRIOR) && IS_WARRIOR(ch))
   {
      return 1;
   }
   if(IS_OBJ_STAT(obj, ITEM_ANTI_DRUID) && IS_DRUID(ch))
   {
      return 1;
   }
   if(!IS_OBJ_STAT(obj, ITEM_ALLOW_MONK) && IS_MONK(ch))
   {
      return 1;
   }
   return 0;
}


/*
 * SPELLS AND SKILLS.  This area defines which spells are assigned to
 * which classes, and the minimum level the character must be to use
 * the spell or skill.
 */

/*  Totally redone, Soli, 10/22/99  */
void init_spell_levels(void)
{

  int i;

  for (i = 0; i < NUM_CLASSES; i++) {
    spell_level(SKILL_SKIN, i, 3, 0);
    spell_level(SKILL_BANDAGE, i, 1, 0);
  }

  /***** MINOTAUR ****/
  spell_level_race(SKILL_GORE, RACE_MINOTAUR, 1, 0);

  /***** KENDER ****/
  spell_level_race(SKILL_STEAL, RACE_KENDER, 1, 0);


  /***** DWARVES ****/
  spell_level_race(SKILL_NATURAL_HARDINESS, RACE_HYLAR, 1, 0);
  spell_level_race(SKILL_NATURAL_HARDINESS, RACE_DAEWAR, 1, 0);
  spell_level_race(SKILL_NATURAL_HARDINESS, RACE_NEIDAR, 1, 0);


  /***** BARBARIANS ****/
  //spell_level_race(SKILL_NATURAL_HARDINESS, RACE_BARBARIAN, 1, 0);


  /* *** KNIGHTS *** */
  spell_level(SKILL_KICK, CLASS_KNIGHT, 1, 0);
  spell_level(SKILL_RIDING, CLASS_KNIGHT, 2, 0);
  spell_level(SKILL_TAME, CLASS_KNIGHT, 2, 0);
  spell_level(SKILL_RESCUE, CLASS_KNIGHT, 3, 0);
  spell_level(SKILL_COOK, CLASS_KNIGHT, 4, 0);
  spell_level(SKILL_ASSESS, CLASS_KNIGHT, 5, 0);

  spell_level(SKILL_BUILD_FIRE, CLASS_KNIGHT, 6, 0);
  spell_level(SKILL_BASH, CLASS_KNIGHT, 7, 0);
  spell_level(SKILL_GUARD, CLASS_KNIGHT, 9, 0);
  spell_level(SKILL_ARMOR_MASTERY, CLASS_KNIGHT, 10, 0);

  spell_level(SKILL_SHIELD_MASTERY, CLASS_KNIGHT, 11, 0);
  spell_level(SKILL_DUAL_WIELD, CLASS_KNIGHT, 12, 0);
  spell_level(SKILL_CHARGE, CLASS_KNIGHT, 13, 0);
  spell_level(SKILL_CHALLENGE, CLASS_KNIGHT, 14, 0);
  spell_level(SKILL_DISARM, CLASS_KNIGHT, 15, 0);
  spell_level(SKILL_SECOND_ATTACK, CLASS_KNIGHT, 18, 0);
  spell_level(SKILL_RETREAT, CLASS_KNIGHT, 19, 0);
  spell_level(SKILL_IMPROVED_ARMOR_MASTERY, CLASS_KNIGHT, 19, 0);
  spell_level(SKILL_IMPROVED_SHIELD_MASTERY, CLASS_KNIGHT, 20, 0);
  spell_level(SKILL_DODGE, CLASS_KNIGHT, 22, 0);
  spell_level(SKILL_IMPROVED_CHARGE, CLASS_KNIGHT, 23, 0);
  spell_level(SKILL_ADVANCED_ARMOR_MASTERY, CLASS_KNIGHT, 24, 0);
  spell_level(SKILL_ADVANCED_SHIELD_MASTERY, CLASS_KNIGHT, 25, 0);;
  spell_level(SKILL_THIRD_ATTACK, CLASS_KNIGHT, 26, 0);

  /* *** PALADINS *** */
  spell_level(SKILL_KICK, CLASS_PALADIN, 1, 0);
  spell_level(SKILL_LAY_HANDS, CLASS_PALADIN, 1, 0);
  spell_level(SKILL_RIDING, CLASS_PALADIN, 2, 0);
  spell_level(SKILL_TAME, CLASS_PALADIN, 2, 0);

  spell_level(SKILL_RESCUE, CLASS_PALADIN, 4, 0);
  spell_level(SKILL_COOK, CLASS_PALADIN, 4, 0);

  spell_level(SKILL_TURN_UNDEAD, CLASS_PALADIN, 5, 0);
  spell_level(SKILL_CONTROL_UNDEAD, CLASS_PALADIN, 5, 0);
  spell_level(SKILL_BUILD_FIRE, CLASS_PALADIN, 6, 0);

  spell_level(SKILL_ASSESS, CLASS_PALADIN, 6, 0);
  spell_level(SKILL_BASH, CLASS_PALADIN, 7, 0);
  spell_level(SKILL_GUARD, CLASS_PALADIN, 11, 0);
  spell_level(SKILL_DISARM, CLASS_PALADIN, 14, 0);
  spell_level(SKILL_SECOND_ATTACK, CLASS_PALADIN, 18, 0);
  spell_level(SKILL_RETREAT, CLASS_PALADIN, 19, 0);
  spell_level(SKILL_DODGE, CLASS_PALADIN, 22, 0);
  spell_level(SKILL_THIRD_ATTACK, CLASS_PALADIN, 26, 0);

  spell_level(SPELL_BLESS, CLASS_PALADIN, 11, 0);
  spell_level(SPELL_DISPEL_MAGIC, CLASS_PALADIN, 23, 0);
  spell_level(SPELL_PRAYER, CLASS_PALADIN, 26, 0);
  spell_level(SPELL_DETECT_ALIGN, CLASS_PALADIN, 1, 0); 

  spell_level(SPELL_LIGHT, CLASS_PALADIN, 12, SPHERE_CLE_CREATION);
  spell_level(SPELL_DARKNESS, CLASS_PALADIN, 12, SPHERE_CLE_CREATION);
  spell_level(SPELL_CREATE_FOOD, CLASS_PALADIN, 14, SPHERE_CLE_CREATION);
  spell_level(SPELL_CREATE_WATER, CLASS_PALADIN, 15, SPHERE_CLE_CREATION);
  spell_level(SPELL_SILENCE, CLASS_PALADIN, 27, SPHERE_CLE_CREATION);

  spell_level(SPELL_CURE_LIGHT, CLASS_PALADIN, 11, SPHERE_CLE_HEALING);
  spell_level(SPELL_CURE_MODERATE, CLASS_PALADIN, 16, SPHERE_CLE_HEALING);
  spell_level(SPELL_CURE_SEVERE, CLASS_PALADIN, 21, SPHERE_CLE_HEALING);
  spell_level(SPELL_NEUTRALIZE_POISON, CLASS_PALADIN, 23, SPHERE_CLE_HEALING);

  spell_level(SPELL_CAUSE_LIGHT, CLASS_PALADIN, 11, SPHERE_CLE_HEALING_REV);
  spell_level(SPELL_CAUSE_MODERATE, CLASS_PALADIN, 16, SPHERE_CLE_HEALING_REV);
  spell_level(SPELL_CAUSE_SEVERE, CLASS_PALADIN, 21, SPHERE_CLE_HEALING_REV);
  spell_level(SPELL_POISON, CLASS_PALADIN, 23, SPHERE_CLE_HEALING_REV);

  spell_level(SPELL_CURE_BLIND, CLASS_PALADIN, 18, SPHERE_CLE_NECROMANCY);
  spell_level(SPELL_CURE_DEAFNESS, CLASS_PALADIN, 22, SPHERE_CLE_NECROMANCY);
  spell_level(SPELL_REMOVE_CURSE, CLASS_PALADIN, 28, SPHERE_CLE_NECROMANCY);
  spell_level(SPELL_CURE_DISEASE, CLASS_PALADIN, 30, SPHERE_CLE_NECROMANCY);

  spell_level(SPELL_BLINDNESS, CLASS_PALADIN, 18, SPHERE_CLE_NECRO_REV);
  spell_level(SPELL_DEAFENING_WIND, CLASS_PALADIN, 22, SPHERE_CLE_NECRO_REV);
  spell_level(SPELL_BESTOW_CURSE, CLASS_PALADIN, 28, SPHERE_CLE_NECRO_REV);
  spell_level(SPELL_DISEASE, CLASS_PALADIN, 30, SPHERE_CLE_NECRO_REV);

  spell_level(SPELL_DETECT_MAGIC, CLASS_PALADIN, 13, SPHERE_CLE_DIVINATION);
  /* spell_level(SPELL_DETECT_ALIGN, CLASS_PALADIN, 15, SPHERE_CLE_DIVINATION); */
  spell_level(SPELL_DETECT_POISON, CLASS_PALADIN, 17, SPHERE_CLE_DIVINATION);
  spell_level(SPELL_INFRAVISION, CLASS_PALADIN, 24, SPHERE_CLE_DIVINATION);
  spell_level(SPELL_SENSE_LIFE, CLASS_PALADIN, 29, SPHERE_CLE_DIVINATION);

  spell_level(SPELL_ARMOR, CLASS_PALADIN, 13, SPHERE_CLE_PROTECTION);
  spell_level(SPELL_PROT_FROM_UNDEAD, CLASS_PALADIN, 18, SPHERE_CLE_PROTECTION);
  spell_level(SPELL_PROT_FROM_GOOD, CLASS_PALADIN, 23, SPHERE_CLE_PROTECTION);
  spell_level(SPELL_PROT_FROM_EVIL, CLASS_PALADIN, 23, SPHERE_CLE_PROTECTION);
  spell_level(SPELL_MAGIC_VEST, CLASS_PALADIN, 30, SPHERE_CLE_PROTECTION);

  spell_level(SPELL_INSPIRE_FEAR, CLASS_PALADIN, 17, SPHERE_CLE_CHARM);
  spell_level(SPELL_REMOVE_FEAR, CLASS_PALADIN, 17, SPHERE_CLE_CHARM);
  spell_level(SPELL_HOLD_PERSON, CLASS_PALADIN, 27, SCHOOL_NO_RED + SPHERE_CLE_CHARM);
  spell_level(SPELL_CHARM, CLASS_PALADIN, 30, SCHOOL_NO_RED + SPHERE_CLE_CHARM);

  spell_level(SPELL_MAGICAL_STONE, CLASS_PALADIN, 14, SPHERE_CLE_COMBAT);
  spell_level(SPELL_FLAMESTRIKE, CLASS_PALADIN, 24, SPHERE_CLE_COMBAT);
  spell_level(SPELL_STRENGTH, CLASS_PALADIN, 27, SPHERE_CLE_COMBAT);
  spell_level(SPELL_ENHANCE_AGILITY, CLASS_PALADIN, 27, SPHERE_CLE_COMBAT);
  spell_level(SPELL_ENHANCE_ENDURANCE, CLASS_PALADIN, 27, SPHERE_CLE_COMBAT);
  spell_level(SPELL_ENHANCE_INSIGHT, CLASS_PALADIN, 27, SPHERE_CLE_DIVINATION);
  spell_level(SPELL_ENHANCE_CUNNING, CLASS_PALADIN, 27, SPHERE_CLE_DIVINATION);
  spell_level(SPELL_ENHANCE_CHARISMA, CLASS_PALADIN, 27, SPHERE_CLE_CHARM);
  spell_level(SPELL_WEAKNESS, CLASS_PALADIN, 29, SPHERE_CLE_COMBAT);

  spell_level(SPELL_CALL_ANIMAL_SPIRIT, CLASS_PALADIN, 16, SPHERE_CLE_ANIMAL);
  spell_level(SPELL_THORNFLESH, CLASS_PALADIN, 16, SPHERE_CLE_ANIMAL);
  spell_level(SPELL_ANIMAL_SUMMONING, CLASS_PALADIN, 21, SPHERE_CLE_ANIMAL);
  spell_level(SPELL_SUMMON_INSECTS, CLASS_PALADIN, 26, SPHERE_CLE_ANIMAL);
  spell_level(SPELL_ANIMAL_SUMMONING_II, CLASS_PALADIN, 29, SPHERE_CLE_ANIMAL);

  spell_level(SPELL_FAERIE_FIRE, CLASS_PALADIN, 20, SPHERE_CLE_WEATHER);
  spell_level(SPELL_CALL_LIGHTNING, CLASS_PALADIN, 26, SPHERE_CLE_WEATHER);
  spell_level(SPELL_RAINBOW, CLASS_PALADIN, 30, SPHERE_CLE_WEATHER);

  spell_level(SPELL_THORNFLESH, CLASS_PALADIN, 20, SPHERE_CLE_PLANT);
  spell_level(SPELL_ENTANGLE, CLASS_PALADIN, 22, SPHERE_CLE_PLANT);
  spell_level(SPELL_GOODBERRY, CLASS_PALADIN, 27, SPHERE_CLE_PLANT);
  spell_level(SPELL_BARKSKIN, CLASS_PALADIN, 30, SPHERE_CLE_PLANT);

  spell_level(SPELL_DISPEL_EVIL, CLASS_PALADIN, 18, SPHERE_CLE_SUMMONING);
  spell_level(SPELL_DISPEL_GOOD, CLASS_PALADIN, 18, SPHERE_CLE_SUMMONING);
  spell_level(SPELL_SUMMON, CLASS_PALADIN, 22, SPHERE_CLE_SUMMONING);
  spell_level(SPELL_WORD_OF_RECALL, CLASS_PALADIN, 26, SPHERE_CLE_SUMMONING);
  spell_level(SPELL_ABJURE, CLASS_PALADIN, 29, SPHERE_CLE_SUMMONING);

  spell_level(SPELL_DUST_DEVIL, CLASS_PALADIN, 22, SPHERE_CLE_ELEMENTAL);
  spell_level(SPELL_WATER_WALK, CLASS_PALADIN, 25, SPHERE_CLE_ELEMENTAL);
  spell_level(SPELL_WATER_BREATHING, CLASS_PALADIN, 28, SPHERE_CLE_ELEMENTAL);
  spell_level(SPELL_FLAME_BLADE, CLASS_PALADIN, 30, SPHERE_CLE_ELEMENTAL);

  spell_level(SKILL_SWORD_SHIELD_STYLE, CLASS_PALADIN, 10, 0);
  spell_level(SKILL_ARMOR_MASTERY, CLASS_PALADIN, 13, 0);
  spell_level(SKILL_ONE_WEAPON_STYLE, CLASS_PALADIN, 5, 0);
  spell_level(SKILL_IMPROVED_ARMOR_MASTERY, CLASS_PALADIN, 19, 0);
  spell_level(SKILL_IMPROVED_SHIELD_MASTERY, CLASS_PALADIN, 20, 0);
  spell_level(SKILL_ADVANCED_ARMOR_MASTERY, CLASS_PALADIN, 24, 0);
  spell_level(SKILL_ADVANCED_SHIELD_MASTERY, CLASS_PALADIN, 25, 0);;


 
 /* *** MAGES *** */
  spell_level(SKILL_READ_MAGIC, CLASS_MAGE, 1, 0);
  spell_level(SKILL_ASSESS_MAGIC, CLASS_MAGE, 5, 0);
  spell_level(SKILL_COOK, CLASS_MAGE, 4, 0);
  spell_level(SKILL_BUILD_FIRE, CLASS_MAGE, 6, 0);
  spell_level(SKILL_SCRIBE, CLASS_MAGE, 10, 0);
  spell_level(SKILL_THROW, CLASS_MAGE, 15, 0);
  spell_level(SKILL_BREW, CLASS_MAGE, 14, 0);

  spell_level(SPELL_MAGIC_MISSILE, CLASS_MAGE, 1, 0);
  spell_level(SPELL_DETECT_MAGIC, CLASS_MAGE, 1, SPHERE_CLE_DIVINATION);
  spell_level(SPELL_ARMOR, CLASS_MAGE, 2, 0);
  spell_level(SPELL_LIGHT, CLASS_MAGE, 2, SPHERE_CLE_CREATION);
  spell_level(SPELL_SHIELD, CLASS_MAGE, 3, 0);
  spell_level(SPELL_CHILL_TOUCH, CLASS_MAGE, 3, 0);
  spell_level(SPELL_WEB, CLASS_MAGE, 4, 0);
  spell_level(SPELL_BLUR, CLASS_MAGE, 4, 0);
  spell_level(SPELL_BURNING_HANDS, CLASS_MAGE, 5, 0);
  spell_level(SPELL_INFRAVISION, CLASS_MAGE, 5, SPHERE_CLE_DIVINATION);
  spell_level(SKILL_RIDING, CLASS_MAGE, 5, 0);
  spell_level(SKILL_TAME, CLASS_MAGE, 5, 0);

  spell_level(SPELL_DETECT_INVIS, CLASS_MAGE, 6, SPHERE_CLE_DIVINATION);
  spell_level(SPELL_MIRROR_IMAGE, CLASS_MAGE, 6, 0);
  spell_level(SPELL_SHOCKING_GRASP, CLASS_MAGE, 7, 0);
  spell_level(SPELL_INVISIBILITY, CLASS_MAGE, 7, 0);
  spell_level(SPELL_STRENGTH, CLASS_MAGE, 8, SPHERE_CLE_COMBAT);
  spell_level(SPELL_ENHANCE_AGILITY, CLASS_MAGE, 8, SPHERE_CLE_COMBAT);
  spell_level(SPELL_ENHANCE_ENDURANCE, CLASS_MAGE, 8, SPHERE_CLE_COMBAT);
  spell_level(SPELL_ENHANCE_INSIGHT, CLASS_MAGE, 8, SPHERE_CLE_DIVINATION);
  spell_level(SPELL_ENHANCE_CUNNING, CLASS_MAGE, 8, SPHERE_CLE_DIVINATION);
  spell_level(SPELL_ENHANCE_CHARISMA, CLASS_MAGE, 8, SPHERE_CLE_CHARM);
  spell_level(SPELL_GUST_OF_WIND, CLASS_MAGE, 8, 0);
  spell_level(SPELL_WEAKNESS, CLASS_MAGE, 9, SPHERE_CLE_COMBAT);
  spell_level(SPELL_BLINDNESS, CLASS_MAGE, 9, SPHERE_CLE_NECRO_REV);
  spell_level(SPELL_INSPIRE_FEAR, CLASS_MAGE, 9, SCHOOL_BLACK);
  spell_level(SPELL_DISPEL_MAGIC, CLASS_MAGE, 10, 0);
  spell_level(SPELL_LOCATE_OBJECT, CLASS_MAGE, 10, SPHERE_CLE_DIVINATION);
  spell_level(SPELL_REVEAL_ILLUSION, CLASS_MAGE, 10, 0);

  /*  This is where the mage spells get interesting - Soli, 10/22/99  */

  spell_level(SPELL_LIGHTNING_BOLT, CLASS_MAGE, 11, 0);
  spell_level(SPELL_FLIGHT, CLASS_MAGE, 11, 0);
  spell_level(SPELL_SILENCE, CLASS_MAGE, 12, SPHERE_CLE_CREATION);
  spell_level(SPELL_ENCHANT_WEAPON, CLASS_MAGE, 12, SCHOOL_NO_RED);
  spell_level(SPELL_WATER_BREATHING, CLASS_MAGE, 12, SCHOOL_RED);
  spell_level(SPELL_IDENTIFY, CLASS_MAGE, 13, SPHERE_CLE_DIVINATION);
  spell_level(SPELL_MINOR_GLOBE, CLASS_MAGE, 13, SCHOOL_WHITE);
  spell_level(SPELL_HASTE, CLASS_MAGE, 13, SCHOOL_RED);
  spell_level(SPELL_VAMPIRIC_TOUCH, CLASS_MAGE, 13, SCHOOL_BLACK);
  spell_level(SPELL_CHARM, CLASS_MAGE, 14, SCHOOL_NO_WHITE);
  spell_level(SPELL_DANCING_SWORD, CLASS_MAGE, 14, SCHOOL_NO_WHITE);
  spell_level(SPELL_SLOW, CLASS_MAGE, 14, SCHOOL_RED);
  spell_level(SPELL_REVEAL_ILLUSION, CLASS_MAGE, 14, 0);
  spell_level(SPELL_FIREBALL, CLASS_MAGE, 15, 0);
  spell_level(SPELL_SLEEP, CLASS_MAGE, 15, SCHOOL_NO_RED);
  spell_level(SPELL_TELEPORT, CLASS_MAGE, 16, SPHERE_CLE_SUMMONING);
  spell_level(SPELL_STONESKIN, CLASS_MAGE, 16, SCHOOL_NO_BLACK);
  spell_level(SPELL_HOLD_PERSON, CLASS_MAGE, 17,
              SCHOOL_NO_RED + SPHERE_CLE_CHARM);
  spell_level(SPELL_TELEPORT_OBJECT, CLASS_MAGE, 17, SCHOOL_RED);
  spell_level(SPELL_CHAIN_LIGHTNING, CLASS_MAGE, 18, 0);
  spell_level(SPELL_SCRY, CLASS_MAGE, 19, 0);
  spell_level(SPELL_CONJURE_ELEMENTAL, CLASS_MAGE, 19,
              SCHOOL_NO_WHITE + SPHERE_CLE_ELEMENTAL);
  spell_level(SPELL_MAJOR_GLOBE, CLASS_MAGE, 20, SCHOOL_WHITE);
  spell_level(SPELL_BLINK, CLASS_MAGE, 20, SCHOOL_NO_BLACK);
  spell_level(SPELL_ARROW_OF_BONE, CLASS_MAGE, 20, SCHOOL_BLACK);
  spell_level(SPELL_FIRE_SHIELD, CLASS_MAGE, 21, 0);
  spell_level(SPELL_FREE_ACTION, CLASS_MAGE, 21,
              SCHOOL_NO_RED + SPHERE_CLE_CHARM);
  spell_level(SPELL_STATUE, CLASS_MAGE, 21, SCHOOL_RED);
  spell_level(SPELL_CONE_OF_COLD, CLASS_MAGE, 22, 0);
  spell_level(SPELL_POLYMORPH, CLASS_MAGE, 22, 0);
  spell_level(SPELL_CLOAK_OF_FEAR, CLASS_MAGE,   22, SCHOOL_BLACK);
  spell_level(SPELL_BLACKMANTLE, CLASS_MAGE, 23, SCHOOL_BLACK);
  spell_level(SPELL_MAGICAL_SUSCEPT, CLASS_MAGE, 23, SCHOOL_RED);
  spell_level(SPELL_ANTIMAGIC_AURA, CLASS_MAGE, 24, SCHOOL_WHITE);
  spell_level(SPELL_PRISMATIC_SPRAY, CLASS_MAGE, 24, SCHOOL_NO_WHITE);
  spell_level(SPELL_DECEPTIVE_IMAGERY, CLASS_MAGE, 25, SCHOOL_NO_BLACK);
  spell_level(SPELL_LEGEND_LORE, CLASS_MAGE, 25, 0);
  spell_level(SPELL_MASS_CHARM, CLASS_MAGE, 26, SCHOOL_NO_RED);
  spell_level(SPELL_CRIPPLE, CLASS_MAGE, 26, SCHOOL_RED);
  spell_level(SPELL_ENERGY_DRAIN, CLASS_MAGE, 26, SCHOOL_BLACK);
  spell_level(SPELL_METEOR_SWARM, CLASS_MAGE, 27, 0);
  spell_level(SPELL_GREATER_ELEMENTAL, CLASS_MAGE, 27, SCHOOL_RED);
  spell_level(SPELL_PHANTASMAL_SNAKE, CLASS_MAGE, 28, SCHOOL_NO_BLACK);
  spell_level(SPELL_RECHARGE_ITEM, CLASS_MAGE, 28, SCHOOL_NO_RED);
  spell_level(SPELL_VAMPIRIC_AURA, CLASS_MAGE, 29, SCHOOL_BLACK);
  spell_level(SPELL_GATE, CLASS_MAGE, 29, 0);
  spell_level(SPELL_SPELL_TURNING, CLASS_MAGE, 30, SCHOOL_WHITE);
  spell_level(SPELL_RECOLLECTION, CLASS_MAGE, 30, SCHOOL_RED);
  spell_level(SPELL_POWER_WORD_KILL, CLASS_MAGE, 30, SCHOOL_BLACK);


  /* *** CLERICS *** */

  spell_level(SKILL_READ_MAGIC, CLASS_CLERIC, 1, 0);
  spell_level(SKILL_RIDING, CLASS_CLERIC, 5, 0);
  spell_level(SKILL_TAME, CLASS_CLERIC, 5, 0);
  spell_level(SKILL_COOK, CLASS_CLERIC, 4, 0);
  spell_level(SKILL_BUILD_FIRE, CLASS_CLERIC, 6, 0);
  spell_level(SKILL_SECOND_ATTACK, CLASS_CLERIC, 18, 0);

  spell_level(SKILL_TURN_UNDEAD, CLASS_CLERIC, 6, 0);
  spell_level(SKILL_CONTROL_UNDEAD, CLASS_CLERIC, 6, 0);
  spell_level(SKILL_ASSESS_MAGIC, CLASS_CLERIC, 9, 0);
  spell_level(SKILL_THROW, CLASS_CLERIC, 10, 0);
 //  spell_level(SKILL_SCRIBE, CLASS_CLERIC, 13, 0);
  spell_level(SKILL_BREW, CLASS_CLERIC, 16, 0);


  spell_level(SPELL_BLESS, CLASS_CLERIC,            1, SPHERE_CLE_ALL);
  spell_level(SPELL_CONCEAL_ALIGN, CLASS_CLERIC,    9,
     SCHOOL_BLACK + SPHERE_CLE_ALL);
  spell_level(SPELL_MINOR_SANCTUARY, CLASS_CLERIC, 10, SPHERE_CLE_ALL);
  spell_level(SPELL_DISPEL_MAGIC, CLASS_CLERIC,    12, SPHERE_CLE_ALL);
  spell_level(SPELL_PRAYER, CLASS_CLERIC,          14, SPHERE_CLE_ALL);
  spell_level(SPELL_SANCTUARY, CLASS_CLERIC,       19, SPHERE_CLE_ALL);

  spell_level(SPELL_LIGHT, CLASS_CLERIC,            2, SPHERE_CLE_CREATION);
  spell_level(SPELL_DARKNESS, CLASS_CLERIC,         2, SPHERE_CLE_CREATION);
  spell_level(SPELL_CREATE_FOOD, CLASS_CLERIC,      3, SPHERE_CLE_CREATION);
  spell_level(SPELL_CREATE_WATER, CLASS_CLERIC,     4, SPHERE_CLE_CREATION);
  spell_level(SPELL_SILENCE, CLASS_CLERIC,         13, SPHERE_CLE_CREATION);
  spell_level(SPELL_CREATE_SPRING, CLASS_CLERIC,   16, SPHERE_CLE_CREATION);
  spell_level(SPELL_SPHERE_SILENCE, CLASS_CLERIC,  21, SPHERE_CLE_CREATION);
  spell_level(SPELL_HEROES_FEAST, CLASS_CLERIC,    23, SPHERE_CLE_CREATION);
  spell_level(SPELL_BLADE_BARRIER, CLASS_CLERIC,   28, SPHERE_CLE_CREATION);

  spell_level(SPELL_CURE_LIGHT, CLASS_CLERIC,       1, SPHERE_CLE_HEALING);
  spell_level(SPELL_CURE_MODERATE, CLASS_CLERIC,    5, SPHERE_CLE_HEALING);
  spell_level(SPELL_CURE_SEVERE, CLASS_CLERIC,      9, SPHERE_CLE_HEALING);
  spell_level(SPELL_NEUTRALIZE_POISON,CLASS_CLERIC,10, SPHERE_CLE_HEALING);
  spell_level(SPELL_CURE_CRITIC, CLASS_CLERIC,     13, SPHERE_CLE_HEALING);
  spell_level(SPELL_HEAL, CLASS_CLERIC,            18, SPHERE_CLE_HEALING);
  spell_level(SPELL_HEALING_LIGHT, CLASS_CLERIC,   22, SPHERE_CLE_HEALING);
  spell_level(SPELL_REGENERATION, CLASS_CLERIC,    29, SPHERE_CLE_HEALING);

  spell_level(SPELL_CAUSE_LIGHT, CLASS_CLERIC,      1, SPHERE_CLE_HEALING_REV);
  spell_level(SPELL_CAUSE_MODERATE, CLASS_CLERIC,   5, SPHERE_CLE_HEALING_REV);
  spell_level(SPELL_CAUSE_SEVERE, CLASS_CLERIC,     9, SPHERE_CLE_HEALING_REV);
  spell_level(SPELL_POISON, CLASS_CLERIC,          10, SPHERE_CLE_HEALING_REV);
  spell_level(SPELL_CAUSE_CRITIC, CLASS_CLERIC,    13, SPHERE_CLE_HEALING_REV);
  spell_level(SPELL_HARM, CLASS_CLERIC,            18, SPHERE_CLE_HEALING_REV);
  spell_level(SPELL_HARMFUL_WRATH, CLASS_CLERIC,   22, SPHERE_CLE_HEALING_REV);
  spell_level(SPELL_WITHER, CLASS_CLERIC,          29, SPHERE_CLE_HEALING_REV);

  spell_level(SPELL_CURE_BLIND, CLASS_CLERIC,       6, SPHERE_CLE_NECROMANCY);
  spell_level(SPELL_CURE_DEAFNESS, CLASS_CLERIC,    9, SPHERE_CLE_NECROMANCY);
  spell_level(SPELL_REMOVE_CURSE, CLASS_CLERIC,    14, SPHERE_CLE_NECROMANCY);
  spell_level(SPELL_CURE_DISEASE, CLASS_CLERIC,    15, SPHERE_CLE_NECROMANCY);
  spell_level(SPELL_REMOVE_PARALYSIS, CLASS_CLERIC,20, SPHERE_CLE_NECROMANCY);
  spell_level(SPELL_RESTORATION, CLASS_CLERIC,     23, SPHERE_CLE_NECROMANCY);
  spell_level(SPELL_BREATH_OF_LIFE, CLASS_CLERIC,  30, SPHERE_CLE_NECROMANCY);

  spell_level(SPELL_BLINDNESS, CLASS_CLERIC,        6, SPHERE_CLE_NECRO_REV);
  spell_level(SPELL_DEAFENING_WIND, CLASS_CLERIC,   9, SPHERE_CLE_NECRO_REV);
  spell_level(SPELL_BESTOW_CURSE, CLASS_CLERIC,    14, SPHERE_CLE_NECRO_REV);
  spell_level(SPELL_DISEASE, CLASS_CLERIC,         15, SPHERE_CLE_NECRO_REV);
  spell_level(SPELL_PARALYZE, CLASS_CLERIC,        20, SPHERE_CLE_NECRO_REV);
  spell_level(SPELL_SOUL_DRAIN, CLASS_CLERIC,      23, SPHERE_CLE_NECRO_REV);
  spell_level(SPELL_BLACK_PLAGUE, CLASS_CLERIC,    30, SPHERE_CLE_NECRO_REV);

  spell_level(SPELL_DETECT_MAGIC, CLASS_CLERIC,     2, SPHERE_CLE_DIVINATION);
  spell_level(SPELL_DETECT_ALIGN, CLASS_CLERIC,     4, SPHERE_CLE_DIVINATION);
  spell_level(SPELL_DETECT_POISON, CLASS_CLERIC,    6, SPHERE_CLE_DIVINATION);
  spell_level(SPELL_INFRAVISION, CLASS_CLERIC,     11, SPHERE_CLE_DIVINATION);
  spell_level(SPELL_SENSE_LIFE, CLASS_CLERIC,      14, SPHERE_CLE_DIVINATION);
  spell_level(SPELL_LOCATE_OBJECT, CLASS_CLERIC,   18, SPHERE_CLE_DIVINATION);
  spell_level(SPELL_DETECT_INVIS, CLASS_CLERIC,    20, SPHERE_CLE_DIVINATION);
  spell_level(SPELL_REFLECTING_POOL, CLASS_CLERIC, 24, SPHERE_CLE_DIVINATION);
  spell_level(SPELL_LEGEND_LORE, CLASS_CLERIC,     27, SPHERE_CLE_DIVINATION);

  spell_level(SPELL_ARMOR, CLASS_CLERIC,            5, SPHERE_CLE_PROTECTION);
  spell_level(SPELL_PROT_FROM_UNDEAD, CLASS_CLERIC, 8, SPHERE_CLE_PROTECTION);
  spell_level(SPELL_PROT_FROM_GOOD, CLASS_CLERIC,  12, SPHERE_CLE_PROTECTION);
  spell_level(SPELL_PROT_FROM_EVIL, CLASS_CLERIC,  12, SPHERE_CLE_PROTECTION);
  spell_level(SPELL_MAGIC_VEST, CLASS_CLERIC,      15, SPHERE_CLE_PROTECTION);
  spell_level(SPELL_PROT_FROM_FIRE, CLASS_CLERIC,  18, SPHERE_CLE_PROTECTION);
  spell_level(SPELL_PROT_FROM_FROST, CLASS_CLERIC, 20, SPHERE_CLE_PROTECTION);
  spell_level(SPELL_PROT_FROM_ELEMENTS,CLASS_CLERIC,25, SPHERE_CLE_PROTECTION);
  spell_level(SPELL_IMPERVIOUS_MIND, CLASS_CLERIC, 29, SPHERE_CLE_PROTECTION);

  spell_level(SPELL_INSPIRE_FEAR, CLASS_CLERIC,     8, SPHERE_CLE_CHARM);
  spell_level(SPELL_REMOVE_FEAR, CLASS_CLERIC,      8, SPHERE_CLE_CHARM);
  spell_level(SPELL_HOLD_PERSON, CLASS_CLERIC,     13,
              SCHOOL_NO_RED + SPHERE_CLE_CHARM);
  spell_level(SPELL_CHARM, CLASS_CLERIC,           15,
              SCHOOL_NO_RED + SPHERE_CLE_CHARM);
  spell_level(SPELL_BRAVERY, CLASS_CLERIC,         18, SPHERE_CLE_CHARM);
  spell_level(SPELL_CLOAK_OF_FEAR, CLASS_CLERIC,   20, SPHERE_CLE_CHARM);
  spell_level(SPELL_FREE_ACTION, CLASS_CLERIC,     25,
              SCHOOL_NO_RED + SPHERE_CLE_CHARM);
  spell_level(SPELL_CALM, CLASS_CLERIC,            28, SPHERE_CLE_CHARM);

  spell_level(SPELL_MAGICAL_STONE, CLASS_CLERIC,    3, SPHERE_CLE_COMBAT);
  spell_level(SPELL_FLAMESTRIKE, CLASS_CLERIC,     10, SPHERE_CLE_COMBAT);
  spell_level(SPELL_STRENGTH, CLASS_CLERIC,        13, SPHERE_CLE_COMBAT);
  spell_level(SPELL_ENHANCE_AGILITY, CLASS_CLERIC, 13, SPHERE_CLE_COMBAT);
  spell_level(SPELL_ENHANCE_ENDURANCE, CLASS_CLERIC, 13, SPHERE_CLE_COMBAT);
  spell_level(SPELL_ENHANCE_INSIGHT, CLASS_CLERIC, 13, SPHERE_CLE_DIVINATION);
  spell_level(SPELL_ENHANCE_CUNNING, CLASS_CLERIC, 13, SPHERE_CLE_DIVINATION);
  spell_level(SPELL_ENHANCE_CHARISMA, CLASS_CLERIC, 13, SPHERE_CLE_CHARM);
  spell_level(SPELL_WEAKNESS, CLASS_CLERIC,        15, SPHERE_CLE_COMBAT);
  spell_level(SPELL_SPIRIT_HAMMER, CLASS_CLERIC,   16, SPHERE_CLE_COMBAT);
  spell_level(SPELL_AID, CLASS_CLERIC,             21, SPHERE_CLE_COMBAT);
  spell_level(SPELL_DIVINE_WRATH, CLASS_CLERIC,    25, SPHERE_CLE_COMBAT);
  spell_level(SPELL_HOLY_WORD, CLASS_CLERIC,       28, SPHERE_CLE_COMBAT);

  spell_level(SPELL_CALL_ANIMAL_SPIRIT,CLASS_CLERIC,7, SPHERE_CLE_ANIMAL);
  spell_level(SPELL_ANIMAL_SUMMONING, CLASS_CLERIC,10, SPHERE_CLE_ANIMAL);
  spell_level(SPELL_SUMMON_INSECTS, CLASS_CLERIC,  13, SPHERE_CLE_ANIMAL);
  spell_level(SPELL_ANIMAL_SUMMONING_II,CLASS_CLERIC,14,SPHERE_CLE_ANIMAL);
  spell_level(SPELL_INSECT_PLAGUE, CLASS_CLERIC,   18, SPHERE_CLE_ANIMAL);
  spell_level(SPELL_MYSTIC_SPIRIT, CLASS_CLERIC,   21, SPHERE_CLE_ANIMAL);
  spell_level(SPELL_ANIMAL_SUMMONING_III,CLASS_CLERIC,24,SPHERE_CLE_ANIMAL);
  spell_level(SPELL_CREEPING_DOOM, CLASS_CLERIC,   28, SPHERE_CLE_ANIMAL);

  spell_level(SPELL_FAERIE_FIRE, CLASS_CLERIC,      9, SPHERE_CLE_WEATHER);
  spell_level(SPELL_CALL_LIGHTNING, CLASS_CLERIC,  15, SPHERE_CLE_WEATHER);
  spell_level(SPELL_RAINBOW, CLASS_CLERIC,         20, SPHERE_CLE_WEATHER);
  spell_level(SPELL_STORM_SUMMONING, CLASS_CLERIC, 23, SPHERE_CLE_WEATHER);
  spell_level(SPELL_DEHYDRATION, CLASS_CLERIC,     24, SPHERE_CLE_WEATHER);
  spell_level(SPELL_MOONBEAM, CLASS_CLERIC,        27, SPHERE_CLE_WEATHER);
  spell_level(SPELL_SUNRAY, CLASS_CLERIC,          30, SPHERE_CLE_WEATHER);

  spell_level(SPELL_THORNFLESH, CLASS_CLERIC,       8, SPHERE_CLE_PLANT);
  spell_level(SPELL_ENTANGLE, CLASS_CLERIC,        10, SPHERE_CLE_PLANT);
  spell_level(SPELL_GOODBERRY, CLASS_CLERIC,       13, SPHERE_CLE_PLANT);
  spell_level(SPELL_BARKSKIN, CLASS_CLERIC,        15, SPHERE_CLE_PLANT);
  spell_level(SPELL_THORNSHIELD, CLASS_CLERIC,     19, SPHERE_CLE_PLANT);
  spell_level(SPELL_STICKS_TO_SNAKES, CLASS_CLERIC,22, SPHERE_CLE_PLANT);
  spell_level(SPELL_MIRE, CLASS_CLERIC,            25, SPHERE_CLE_PLANT);
  spell_level(SPELL_BLESSING_OF_KINGS, CLASS_CLERIC,30, SPHERE_CLE_ALL);  
  /*  Some high-level plant spell, at level 28, eventually  */

  spell_level(SPELL_DISPEL_GOOD, CLASS_CLERIC,      7, SPHERE_CLE_SUMMONING);
  spell_level(SPELL_DISPEL_EVIL, CLASS_CLERIC,      7, SPHERE_CLE_SUMMONING);
  spell_level(SPELL_SUMMON, CLASS_CLERIC,          12, SPHERE_CLE_SUMMONING);
  spell_level(SPELL_WORD_OF_RECALL, CLASS_CLERIC,  13, SPHERE_CLE_SUMMONING);
  spell_level(SPELL_ABJURE, CLASS_CLERIC,          15, SPHERE_CLE_SUMMONING);
  spell_level(SPELL_TELEPORT, CLASS_CLERIC,        15, SPHERE_CLE_SUMMONING);
  spell_level(SPELL_AERIAL_SERVANT, CLASS_CLERIC,  19, SPHERE_CLE_SUMMONING);
  spell_level(SPELL_SUMMON_GUARD, CLASS_CLERIC,    24, SPHERE_CLE_SUMMONING);
  spell_level(SPELL_GATE, CLASS_CLERIC,            28, SPHERE_CLE_SUMMONING);

  spell_level(SPELL_DUST_DEVIL, CLASS_CLERIC,        9, SPHERE_CLE_ELEMENTAL);
  spell_level(SPELL_WATER_WALK, CLASS_CLERIC,       11, SPHERE_CLE_ELEMENTAL);
  spell_level(SPELL_WATER_BREATHING, CLASS_CLERIC,  14, SPHERE_CLE_ELEMENTAL);
  spell_level(SPELL_FLAME_BLADE, CLASS_CLERIC,      15, SPHERE_CLE_ELEMENTAL);
  spell_level(SPELL_ADAMANT_MACE, CLASS_CLERIC,     18, SPHERE_CLE_ELEMENTAL);
  spell_level(SPELL_CONJURE_ELEMENTAL, CLASS_CLERIC,22,
              SCHOOL_NO_WHITE + SPHERE_CLE_ELEMENTAL);
  spell_level(SPELL_WIND_WALK, CLASS_CLERIC,        26, SPHERE_CLE_ELEMENTAL);
  spell_level(SPELL_ELEMENTAL_STORM, CLASS_CLERIC,  29, SPHERE_CLE_ELEMENTAL);



  /* *** RANGERS *** */
  spell_level(SKILL_KICK, CLASS_RANGER, 1, 0);
  spell_level(SKILL_RIDING, CLASS_RANGER, 1, 0);
  spell_level(SKILL_TAME, CLASS_RANGER, 1, 0);

  spell_level(SKILL_TRACK, CLASS_RANGER, 3, 0);
  spell_level(SKILL_BUILD_FIRE, CLASS_RANGER, 3, 0);
  spell_level(SKILL_COOK, CLASS_RANGER, 4, 0);
  spell_level(SKILL_PASS_WO_TRACE, CLASS_RANGER, 4, 0);
  spell_level(SKILL_RESCUE, CLASS_RANGER, 5, 0);

  spell_level(SKILL_ASSESS, CLASS_RANGER, 7, 0);
  spell_level(SKILL_DODGE, CLASS_RANGER, 8, 0);
  //spell_level(SKILL_BASH, CLASS_RANGER, 9, 0);
  spell_level(SKILL_SNEAK, CLASS_RANGER, 10, 0);

  spell_level(SPELL_GOODBERRY, CLASS_RANGER, 11, SPHERE_CLE_PLANT);
  spell_level(SKILL_THROW, CLASS_RANGER, 11, 0);
  spell_level(SPELL_THORNFLESH, CLASS_RANGER, 11, SPHERE_CLE_PLANT);
  spell_level(SKILL_DUAL_WIELD, CLASS_RANGER, 12, 0);
  spell_level(SPELL_CREATE_SPRING, CLASS_RANGER, 12, SPHERE_CLE_CREATION);
  spell_level(SPELL_ENTANGLE, CLASS_RANGER, 12, SPHERE_CLE_PLANT);
  spell_level(SKILL_GUARD, CLASS_RANGER, 13, 0);
  spell_level(SPELL_DETECT_MAGIC, CLASS_RANGER, 14, SPHERE_CLE_DIVINATION);
  //spell_level(SPELL_CALL_ANIMAL_SPIRIT, CLASS_RANGER, 15, SPHERE_CLE_ANIMAL);
  spell_level(SKILL_HIDE, CLASS_RANGER, 15, 0);
  spell_level(SPELL_ANIMAL_SUMMONING, CLASS_RANGER, 15, SPHERE_CLE_ANIMAL);

  spell_level(SPELL_FAERIE_FIRE, CLASS_RANGER, 16, SPHERE_CLE_WEATHER);
  spell_level(SPELL_BARKSKIN, CLASS_RANGER, 16, SPHERE_CLE_PLANT);
  spell_level(SKILL_RETREAT, CLASS_RANGER, 17, 0);
  spell_level(SKILL_SECOND_ATTACK, CLASS_RANGER, 18, 0);
  spell_level(SPELL_DUST_DEVIL, CLASS_RANGER, 19, SPHERE_CLE_ELEMENTAL);

  //spell_level(SPELL_MAGICAL_STONE, CLASS_RANGER, 21, SPHERE_CLE_COMBAT);
  spell_level(SPELL_ANIMAL_SUMMONING_II, CLASS_RANGER, 21, SPHERE_CLE_ANIMAL);
  spell_level(SPELL_SUMMON_INSECTS, CLASS_RANGER, 22, SPHERE_CLE_ANIMAL);
  //spell_level(SPELL_WATER_WALK, CLASS_RANGER, 23, SPHERE_CLE_ELEMENTAL);
  spell_level(SPELL_THORNSHIELD, CLASS_RANGER, 24, SPHERE_CLE_PLANT);
  spell_level(SPELL_STONESKIN, CLASS_RANGER, 25, SCHOOL_NO_BLACK);
  spell_level(SPELL_DISPEL_MAGIC, CLASS_RANGER, 25, 0);

  spell_level(SKILL_THIRD_ATTACK, CLASS_RANGER, 26, 0);
  spell_level(SPELL_CALL_LIGHTNING, CLASS_RANGER, 26, SPHERE_CLE_WEATHER);
  spell_level(SPELL_MIRE, CLASS_RANGER, 26, SPHERE_CLE_PLANT);
  spell_level(SPELL_WATER_BREATHING, CLASS_RANGER, 28, SPHERE_CLE_ELEMENTAL);
  spell_level(SPELL_ANIMAL_SUMMONING_III, CLASS_RANGER, 28, SPHERE_CLE_ANIMAL);
  spell_level(SKILL_TWO_WEAPON_STYLE, CLASS_RANGER, 28, 0);
  spell_level(SPELL_MYSTIC_SPIRIT, CLASS_RANGER, 29, SPHERE_CLE_ANIMAL);
  spell_level(SPELL_AID, CLASS_RANGER, 30, SPHERE_CLE_COMBAT);

  /* *** ROGUES *** */
  spell_level(SKILL_SNEAK, CLASS_ROGUE, 1, 0);
  spell_level(SKILL_KICK, CLASS_ROGUE, 2, 0);
  spell_level(SKILL_BACKSTAB, CLASS_ROGUE, 3, 0);
  spell_level(SKILL_RIDING, CLASS_ROGUE, 3, 0);
  spell_level(SKILL_TAME, CLASS_ROGUE, 3, 0);
  spell_level(SKILL_BUILD_FIRE, CLASS_ROGUE, 4, 0);
  spell_level(SKILL_COOK, CLASS_ROGUE, 4, 0);
  //spell_level(SKILL_MARK, CLASS_ROGUE, 4, 0);
  spell_level(SKILL_TRACK, CLASS_ROGUE, 5, 0);

  spell_level(SKILL_STEAL, CLASS_ROGUE, 6, 0);
  spell_level(SKILL_HIDE, CLASS_ROGUE, 7, 0);
  spell_level(SKILL_DODGE, CLASS_ROGUE, 8, 0);
  spell_level(SKILL_JAWSTRIKE, CLASS_ROGUE, 8, 0);
  spell_level(SKILL_ASSESS, CLASS_ROGUE, 9, 0);
  spell_level(SKILL_THROW, CLASS_ROGUE, 9, 0);
  spell_level(SKILL_PICK_LOCK, CLASS_ROGUE, 10, 0);

  spell_level(SKILL_IMPROVED_BACKSTAB, CLASS_ROGUE, 11, 0);
  spell_level(SKILL_IMPROVED_STEALTH, CLASS_ROGUE, 12, 0);
  spell_level(SKILL_DUAL_WIELD, CLASS_ROGUE, 12, 0);
  spell_level(SKILL_TRIP, CLASS_ROGUE, 13, 0);

  spell_level(SKILL_RETREAT, CLASS_ROGUE, 16, 0);
  spell_level(SKILL_DIRTKICK, CLASS_ROGUE, 16, 0);
  spell_level(SKILL_ENVENOM, CLASS_ROGUE, 18, 0);
  spell_level(SKILL_SECOND_ATTACK, CLASS_ROGUE, 18, 0);

  spell_level(SKILL_DISGUISE, CLASS_ROGUE, 21, 0);
  spell_level(SKILL_DETECTDISGUISE, CLASS_ROGUE, 21, 0);
  spell_level(SKILL_READ_MAGIC, CLASS_ROGUE, 22, 0);
  spell_level(SKILL_DIRTY_FIGHTING, CLASS_ROGUE, 23, 0);
  spell_level(SKILL_KNOCKOUT, CLASS_ROGUE, 24, 0);
  spell_level(SKILL_ADVANCED_BACKSTAB, CLASS_ROGUE, 25, 0);
  spell_level(SKILL_TWO_WEAPON_STYLE, CLASS_ROGUE, 30, 0);

  /* WARRIORS */
  spell_level(SKILL_KICK, CLASS_WARRIOR, 1, 0);
  spell_level(SKILL_RIDING, CLASS_WARRIOR, 2, 0);
  spell_level(SKILL_TAME, CLASS_WARRIOR, 2, 0);
  spell_level(SKILL_RESCUE, CLASS_WARRIOR, 4, 0);
  spell_level(SKILL_ONE_WEAPON_STYLE, CLASS_WARRIOR, 5, 0);
  spell_level(SKILL_COOK, CLASS_WARRIOR, 5, 0);

  spell_level(SKILL_ASSESS, CLASS_WARRIOR, 6, 0);
  spell_level(SKILL_BUILD_FIRE, CLASS_WARRIOR, 7, 0); 
  spell_level(SKILL_BASH, CLASS_WARRIOR, 7, 0);
  spell_level(SKILL_THROW, CLASS_WARRIOR, 9, 0);
  spell_level(SKILL_SWORD_SHIELD_STYLE, CLASS_WARRIOR, 10, 0);

  spell_level(SKILL_GUARD, CLASS_WARRIOR, 11, 0);
  spell_level(SKILL_TWO_HANDED_STYLE, CLASS_WARRIOR, 12, 0);
  spell_level(SKILL_DUAL_WIELD, CLASS_WARRIOR, 12, 0);
  spell_level(SKILL_ARMOR_MASTERY, CLASS_WARRIOR, 13, 0);
  spell_level(SKILL_CHARGE, CLASS_WARRIOR, 13, 0);
  spell_level(SKILL_DISARM, CLASS_WARRIOR, 14, 0);

  spell_level(SKILL_RETREAT, CLASS_WARRIOR, 16, 0);
  spell_level(SKILL_JAWSTRIKE, CLASS_WARRIOR, 17, 0);
  spell_level(SKILL_SECOND_ATTACK, CLASS_WARRIOR, 18, 0);
  spell_level(SKILL_SHIELD_MASTERY, CLASS_WARRIOR, 19, 0);
  spell_level(SKILL_IMPROVED_ARMOR_MASTERY, CLASS_WARRIOR, 19, 0);
  spell_level(SKILL_IMPROVED_SHIELD_MASTERY, CLASS_WARRIOR, 20, 0);
  spell_level(SKILL_TWO_WEAPON_STYLE, CLASS_WARRIOR, 20, 0);

  spell_level(SKILL_COMBAT_IMPROVISATION, CLASS_WARRIOR, 21, 0);
  spell_level(SKILL_ADVANCED_ARMOR_MASTERY, CLASS_WARRIOR, 24, 0);
  spell_level(SKILL_ADVANCED_SHIELD_MASTERY, CLASS_WARRIOR, 25, 0);;
  spell_level(SKILL_DODGE, CLASS_WARRIOR, 22, 0);
  spell_level(SKILL_THIRD_ATTACK, CLASS_WARRIOR, 26, 0);

  /* MONKS */
  spell_level(SKILL_KICK, CLASS_MONK, 1, 0);
  spell_level(SKILL_COOK, CLASS_MONK, 2, 0);
  spell_level(SKILL_TAME, CLASS_MONK, 3, 0);
  spell_level(SKILL_RIDING, CLASS_MONK, 3, 0);
  spell_level(SKILL_UNARMED_DEFENSE, CLASS_MONK, 4, 0);
  spell_level(SKILL_MARTIAL_ARTS, CLASS_MONK, 5, 0);

  spell_level(SKILL_BUILD_FIRE, CLASS_MONK, 6, 0);
  spell_level(SKILL_HIDE, CLASS_MONK, 7, 0);
  //spell_level(SKILL_SNEAK, CLASS_MONK, 8, 0);
  spell_level(SKILL_THROW, CLASS_MONK, 8, 0);

  spell_level(SKILL_TRIP, CLASS_MONK, 12, 0);
  spell_level(SKILL_DETECTDISGUISE, CLASS_MONK, 13, 0);
  spell_level(SKILL_DISARM, CLASS_MONK, 14, 0);
  spell_level(SKILL_IMPROVED_UNARMED_DEFENSE, CLASS_MONK, 14, 0);
  spell_level(SKILL_IMPROVED_MARTIAL_ARTS, CLASS_MONK, 15, 0);

  spell_level(SKILL_RETREAT, CLASS_MONK, 16, 0);
  spell_level(SKILL_TRACK, CLASS_MONK, 16, 0);
  spell_level(SKILL_SECOND_ATTACK, CLASS_MONK, 18, 0);
  spell_level(SKILL_DODGE, CLASS_MONK, 18, 0);
  spell_level(SKILL_CHAKRA, CLASS_MONK, 20, 0);

  spell_level(SKILL_THROW_OPPONENT, CLASS_MONK, 21, 0);
  spell_level(SKILL_ADVANCED_UNARMED_DEFENSE, CLASS_MONK, 24, 0);
  spell_level(SKILL_ADVANCED_MARTIAL_ARTS, CLASS_MONK, 25, 0);
  spell_level(SKILL_THIRD_ATTACK, CLASS_MONK, 26, 0);

  spell_level(SKILL_INSIGHT, CLASS_MONK, 27, 0);
  spell_level(SKILL_FOURTH_ATTACK, CLASS_MONK, 30, 0);

  /* DEFENDERS */
  spell_level(SKILL_KICK, CLASS_DEFENDER, 1, 0);
  spell_level(SKILL_RIDING, CLASS_DEFENDER, 2, 0);
  spell_level(SKILL_TAME, CLASS_DEFENDER, 2, 0);
  spell_level(SKILL_RESCUE, CLASS_DEFENDER, 3, 0);
  spell_level(SKILL_ASSESS, CLASS_DEFENDER, 4, 0);

  spell_level(SKILL_BUILD_FIRE, CLASS_DEFENDER, 6, 0);
  spell_level(SKILL_BASH, CLASS_DEFENDER, 7, 0);
  spell_level(SKILL_THROW, CLASS_DEFENDER, 8, 0);
  spell_level(SKILL_GUARD, CLASS_DEFENDER, 9, 0);
  spell_level(SKILL_ARMOR_MASTERY, CLASS_DEFENDER, 9, 0);
  spell_level(SKILL_SHIELD_MASTERY, CLASS_DEFENDER, 10, 0);

  spell_level(SKILL_DAMAGE_REDUCTION, CLASS_DEFENDER, 11, 0);
  spell_level(SKILL_DUAL_WIELD, CLASS_DEFENDER, 12, 0);
  spell_level(SKILL_DISARM, CLASS_DEFENDER, 14, 0);
  spell_level(SKILL_STANCE, CLASS_DEFENDER, 15, 0);

  spell_level(SKILL_SECOND_ATTACK, CLASS_DEFENDER, 18, 0);
  spell_level(SKILL_IMPROVED_DAMAGE_REDUCTION, CLASS_DEFENDER, 18, 0);
  spell_level(SKILL_IMPROVED_ARMOR_MASTERY, CLASS_DEFENDER, 19, 0);
  spell_level(SKILL_IMPROVED_SHIELD_MASTERY, CLASS_DEFENDER, 20, 0);

  spell_level(SKILL_DODGE, CLASS_DEFENDER, 22, 0);
  spell_level(SKILL_ADVANCED_DAMAGE_REDUCTION, CLASS_DEFENDER, 25, 0);

  spell_level(SKILL_THIRD_ATTACK, CLASS_DEFENDER, 26, 0);

  spell_level(SKILL_ADVANCED_ARMOR_MASTERY, CLASS_DEFENDER, 29, 0);
  spell_level(SKILL_ADVANCED_SHIELD_MASTERY, CLASS_DEFENDER, 30, 0);

  /* *** ADVENTURERS *** */
  spell_level(SKILL_KICK, CLASS_ADVENTURER, 1, 0);
  spell_level(SKILL_RIDING, CLASS_ADVENTURER, 2, 0);
  spell_level(SKILL_TAME, CLASS_ADVENTURER, 2, 0);
  spell_level(SKILL_COOK, CLASS_ADVENTURER, 8, 0);
  spell_level(SKILL_BUILD_FIRE, CLASS_ADVENTURER, 6, 0);
  spell_level(SKILL_THROW, CLASS_ADVENTURER, 8, 0);
  

  /* *** DRUIDS *** */
  spell_level(SKILL_RIDING, CLASS_DRUID, 2, 0);
  spell_level(SKILL_TAME, CLASS_DRUID, 2, 0);
  spell_level(SKILL_BUILD_FIRE, CLASS_DRUID, 3, 0);
  spell_level(SKILL_COOK, CLASS_DRUID, 4, 0);
  spell_level(SKILL_THROW, CLASS_DRUID, 9, 0);
  spell_level(SKILL_TRACK, CLASS_DRUID, 4, 0);
  spell_level(SKILL_PASS_WO_TRACE, CLASS_DRUID, 5, 0);
  spell_level(SKILL_SNEAK, CLASS_DRUID, 7, 0);
  spell_level(SKILL_DETECT_POISON, CLASS_DRUID, 12, 0);
  spell_level(SKILL_HIDE, CLASS_DRUID, 14, 0);
  spell_level(SPELL_REVEAL_ILLUSION, CLASS_DRUID, 14, 0);
  spell_level(SKILL_REMOVE_POISON, CLASS_DRUID, 17, 0);
  
  spell_level(SPELL_REFRESH, CLASS_DRUID, 1, 0);
  spell_level(SPELL_GOODBERRY, CLASS_DRUID, 1, SPHERE_CLE_PLANT);
  spell_level(SPELL_CALL_ANIMAL_SPIRIT, CLASS_DRUID, 2, SPHERE_CLE_ANIMAL);
  spell_level(SPELL_CREATE_WATER, CLASS_DRUID, 2, SPHERE_CLE_CREATION);
  spell_level(SPELL_DETECT_MAGIC, CLASS_DRUID, 3, SPHERE_CLE_DIVINATION);
  spell_level(SPELL_SHILLELAGH, CLASS_DRUID, 3, 0);
  spell_level(SPELL_CURE_LIGHT, CLASS_DRUID, 4, SPHERE_CLE_HEALING);
  spell_level(SPELL_CREATE_SPRING, CLASS_DRUID, 4, SPHERE_CLE_CREATION);
  spell_level(SPELL_FAERIE_FIRE, CLASS_DRUID, 5, SPHERE_CLE_WEATHER);
  spell_level(SPELL_THORNFLESH, CLASS_DRUID, 6, SPHERE_CLE_PLANT);
  spell_level(SPELL_ANIMAL_SUMMONING, CLASS_DRUID, 6, SPHERE_CLE_ANIMAL);
  spell_level(SPELL_DUST_DEVIL, CLASS_DRUID, 7, SPHERE_CLE_ELEMENTAL);
  spell_level(SPELL_ENTANGLE, CLASS_DRUID, 8, SPHERE_CLE_PLANT);
  spell_level(SPELL_SUMMON_INSECTS, CLASS_DRUID, 9, SPHERE_CLE_ANIMAL);
  spell_level(SPELL_MAGICAL_STONE, CLASS_DRUID, 10, SPHERE_CLE_COMBAT);
  spell_level(SPELL_POISON, CLASS_DRUID, 10, SPHERE_CLE_HEALING_REV);
  spell_level(SPELL_WATER_WALK, CLASS_DRUID, 11, SPHERE_CLE_ELEMENTAL);
  spell_level(SPELL_BARKSKIN, CLASS_DRUID, 11, SPHERE_CLE_PLANT);
  spell_level(SPELL_DISPEL_MAGIC, CLASS_DRUID, 12, SPHERE_CLE_ALL);
  spell_level(SPELL_GUST_OF_WIND, CLASS_DRUID, 12, SPHERE_CLE_ELEMENTAL);
  spell_level(SPELL_CALL_LIGHTNING, CLASS_DRUID, 13, SPHERE_CLE_WEATHER);
  spell_level(SPELL_CURE_MODERATE, CLASS_DRUID, 13, SPHERE_CLE_HEALING);
  spell_level(SPELL_ANIMAL_SUMMONING_II, CLASS_DRUID, 14, SPHERE_CLE_ANIMAL);
  spell_level(SPELL_SENSE_LIFE, CLASS_DRUID, 14, SPHERE_CLE_DIVINATION);
  spell_level(SPELL_MYSTIC_SPIRIT, CLASS_DRUID, 15, SPHERE_CLE_ANIMAL);
  spell_level(SPELL_FLAMESTRIKE, CLASS_DRUID, 15, SPHERE_CLE_COMBAT);
  spell_level(SPELL_STRENGTH, CLASS_DRUID, 15, SPHERE_CLE_COMBAT);
  spell_level(SPELL_ENHANCE_AGILITY, CLASS_DRUID, 15, SPHERE_CLE_COMBAT);
  spell_level(SPELL_ENHANCE_ENDURANCE, CLASS_DRUID, 15, SPHERE_CLE_COMBAT);
  spell_level(SPELL_ENHANCE_INSIGHT, CLASS_DRUID, 15, SPHERE_CLE_DIVINATION);
  spell_level(SPELL_ENHANCE_CUNNING, CLASS_DRUID, 15, SPHERE_CLE_DIVINATION);
  spell_level(SPELL_ENHANCE_CHARISMA, CLASS_DRUID, 15, SPHERE_CLE_CHARM);
  spell_level(SPELL_WATER_BREATHING, CLASS_DRUID, 16, SPHERE_CLE_ELEMENTAL);
  spell_level(SPELL_FLAME_BLADE, CLASS_DRUID, 16, SPHERE_CLE_ELEMENTAL);
  spell_level(SPELL_AID, CLASS_DRUID, 17, SPHERE_CLE_COMBAT);
  spell_level(SPELL_STONESKIN, CLASS_DRUID, 18, SCHOOL_NO_BLACK);
  spell_level(SPELL_INSECT_PLAGUE, CLASS_DRUID, 19, SPHERE_CLE_ANIMAL);
  spell_level(SPELL_CURE_SEVERE, CLASS_DRUID, 19, SPHERE_CLE_HEALING);
  spell_level(SPELL_THORNSHIELD, CLASS_DRUID, 20, SPHERE_CLE_PLANT);
  spell_level(SPELL_SUNRAY, CLASS_DRUID, 20, SPHERE_CLE_WEATHER);
  spell_level(SPELL_POLYMORPH, CLASS_DRUID, 20, 0);
  spell_level(SPELL_STORM_SUMMONING, CLASS_DRUID, 21, SPHERE_CLE_WEATHER);
  spell_level(SPELL_DEHYDRATION, CLASS_DRUID, 21, SPHERE_CLE_WEATHER);
  spell_level(SPELL_ANIMAL_SUMMONING_III, CLASS_DRUID, 22, SPHERE_CLE_ANIMAL);
  spell_level(SPELL_WORD_OF_RECALL, CLASS_DRUID,  22, SPHERE_CLE_SUMMONING);
  spell_level(SPELL_MIRE, CLASS_DRUID, 23, SPHERE_CLE_PLANT);
  spell_level(SPELL_STICKS_TO_SNAKES, CLASS_DRUID, 23, SPHERE_CLE_PLANT);
  spell_level(SPELL_CONJURE_ELEMENTAL, CLASS_DRUID, 24,
              SCHOOL_NO_WHITE + SPHERE_CLE_ELEMENTAL);
  spell_level(SPELL_RAINBOW, CLASS_DRUID, 25, SPHERE_CLE_WEATHER);
  spell_level(SPELL_PROT_FROM_ELEMENTS, CLASS_DRUID, 26, SPHERE_CLE_PROTECTION);
  spell_level(SPELL_MOONBEAM, CLASS_DRUID, 27, SPHERE_CLE_WEATHER);
  spell_level(SPELL_CREEPING_DOOM, CLASS_DRUID, 28, SPHERE_CLE_ANIMAL);
  spell_level(SPELL_ELEMENTAL_STORM, CLASS_DRUID, 29, SPHERE_CLE_ELEMENTAL);
  spell_level(SKILL_ENCHANT_ITEM, CLASS_DRUID, 30, 0);
  spell_level(SPELL_REGENERATION, CLASS_DRUID, 30, SPHERE_CLE_HEALING);
  spell_level(SPELL_WITHER, CLASS_DRUID, 30, SPHERE_CLE_HEALING_REV);

 
}


/*
 * This is the exp given to implementors -- it must always be greater
 * than the exp required for immortality, plus at least 20,000 or so.
 */
#define EXP_MAX  2000200000

/* Function to return the exp required for each class/level */
int level_exp(int class, int level)
{
   float  modifier = 1.00;
   if(level > LVL_IMPL || level < 0)
   {
      //log("SYSERR: Requesting exp for invalid level!");
      return 0;
   }
   switch(class)
   {
      case CLASS_MONK:
         modifier = 1.50;
         break;
      case CLASS_KNIGHT:
         modifier = 1.20;
         break;
      case CLASS_PALADIN:
         modifier = 1.15;
         break;
      case CLASS_MAGE:
         modifier = 1.40;
         break;
      case CLASS_CLERIC:
         modifier = 0.85;
         break;
      case CLASS_RANGER:
         modifier = 0.95;
         break;
      case CLASS_ROGUE:
         modifier = 1.20;
         break;
      case CLASS_WARRIOR:
         modifier = 1.00;
         break;
      case CLASS_ADVENTURER:
         modifier = 2.00;
         break;
      case CLASS_DRUID:
         modifier = 0.80;
         break;
      case CLASS_DEFENDER:
         modifier = 0.95;
         break;

      default:
         log("SYSERR: level_exp: unknown class passed in!");
         break;
   }

   /* generic difficulty factor --gan, 07/27/2001 */
//   modifier = modifier * 1.2;

   switch(level)
   {
      case   0: return 0;                    break;
      case   1: return 1;                    break;
      case   2: return(modifier *     2000); break;
      case   3: return(modifier *     4000); break;
      case   4: return(modifier *     8000); break;
      case   5: return(modifier *    15000); break;
      case   6: return(modifier *    28000); break;
      case   7: return(modifier *    50000); break;
      case   8: return(modifier *   100000); break;
      case   9: return(modifier *   175000); break;
      case  10: return(modifier *   300000); break;
      case  11: return(modifier *   450000); break;
      case  12: return(modifier *   650000); break;
      case  13: return(modifier *   900000); break;
      case  14: return(modifier *  1200000); break;
      case  15: return(modifier *  1500000); break;
      case  16: return(modifier *  1900000); break;
      case  17: return(modifier *  2400000); break;
      case  18: return(modifier *  2950000); break;
      case  19: return(modifier *  3750000); break;
      case  20: return(modifier *  4600000); break;
      case  21: return(modifier *  5750000); break;
      case  22: return(modifier *  7200000); break;
      case  23: return(modifier *  8950000); break;
      case  24: return(modifier * 11200000); break;
      case  25: return(modifier * 14000000); break;
      case  26: return(modifier * 17500000); break;
      case  27: return(modifier * 22000000); break;
      case  28: return(modifier * 27250000); break;
      case  29: return(modifier * 34000000); break;
      case  30: return(modifier * 43000000); break;

      case LVL_IMMORT:     return(70000000); break;
      case LVL_DEMIGOD:    return(71000000); break;
      case LVL_LESSERGOD:  return(72000000); break;
      case LVL_GOD:        return(73000000); break;
      case LVL_DEPTHEAD:   return(74000000); break;
      case LVL_GRGOD:      return(75000000); break;
      case LVL_ADMIN:      return(76000000); break;
      case LVL_IMPL:       return(77000000); break;
   }

  /*
   * This statement should never be reached if the exp tables in this function
   * are set up properly.  If you see exp of 123456 then the tables above are
   * incomplete -- so, complete them!
   */
  log("SYSERR: XP tables not set up correctly in class.c!");
  return 123456;

}


