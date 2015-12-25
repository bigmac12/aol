#include "sysdep.h"
#include "conf.h"

#include "structs.h"
#include "db.h"
#include "interpreter.h"
#include "utils.h"
#include "spells.h"


const char *race_abbrevs[] = {   
  "Bar",
  "Hyl",
  "Dae",
  "Nei",
  "Sil",
  "Qua",
  "Kag",
  "Hal",
  "Ken",
  "Min",
  "Gno",
  "Hum",
  "\n"                           
};                               

const char *pc_race_types[] = {
  "Barbarian",
  "Hylar",
  "Daewar",
  "Neidar",
  "Silvanesti",
  "Qualinesti",
  "Kagonesti",
  "Halfelven",
  "Kender",
  "Minotaur",
  "Gnome",
  "Human",
  "\n"                           
};                               

/* The menu for choosing a race in interpreter.c: */ 
const char *race_menu =                              
"\r\n"                                               
"Select a race:	\r\n"                                 
"\r\n"
"  A)  Barbarian\r\n"
"  B)  Hylar  (Mountain Dwarf)\r\n"
"  C)  Daewar (Dark Dwarf)\r\n"
"  D)  Neidar (Hill Dwarf)\r\n"
"  E)  Silvanesti (Grey Elf)\r\n"
"  F)  Qualinesti (High Elf)\r\n"
"  G)  Kaganesti  (Sylvan Elf)\r\n"
"  H)  Halfelven\r\n"
"  I)  Kender\r\n"
"  J)  Minotaur\r\n"
"  K)  Gnome\r\n"
"  L)  Human\r\n";

/*
 * The code to interpret a race letter (used in interpreter.c when a 
 * new character is selecting a race).
 */
   extern char *affected_bits[];
   extern struct affected_type *af;
   extern struct char_data *ch;

int parse_race(char arg)
{
  arg = LOWER(arg);

  switch (arg) {
  case 'a':
     return RACE_BARBARIAN;
     break;
  case 'b':
     return RACE_HYLAR;
     break;
  case 'c':
     return RACE_DAEWAR;
     break;
  case 'd':
     return RACE_NEIDAR;
     break;
  case 'e':
     return RACE_SILVANESTI;
     break;
  case 'f':
     return RACE_QUALINESTI;
     break;
  case 'g':
     return RACE_KAGONESTI;
     break;
  case 'h':
     return RACE_HALFELVEN;
     break;
  case 'i':
     return RACE_KENDER;
     break;
  case 'j':
     return RACE_MINOTAUR;
     break;
  case 'k':
     return RACE_GNOME;
     break;
  case 'l':
     return RACE_HUMAN;
     break;

  default:
    return RACE_UNDEFINED;
    break;
  }
}

void race_affects(struct char_data *ch)
{
  int race;
  struct affected_type af;

  extern void affect_join(struct char_data *c, struct affected_type *a,
			  bool ad, bool avg, bool mod, bool avgmod);
 
  race = GET_RACE(ch);
      
  switch(race){

  case RACE_HYLAR:
  case RACE_DAEWAR:
  case RACE_NEIDAR:
    af.type = SPELL_INFRAVISION;
    af.duration = -1;
    af.modifier = 0;
    af.location = APPLY_NONE;
    af.bitvector = AFF_INFRAVISION;
    affect_join(ch, &af, FALSE, FALSE, FALSE, FALSE);

    if ( GET_SKILL(ch, SKILL_NATURAL_HARDINESS) < 60 ) {
      SET_SKILL(ch, SKILL_NATURAL_HARDINESS, 60);
    }
    break;

  case RACE_SILVANESTI:
  case RACE_QUALINESTI:
  case RACE_KAGONESTI:
  case RACE_HALFELVEN:
  case RACE_GNOME:
    af.type = SPELL_INFRAVISION;
    af.duration = -1;
    af.modifier = 0;
    af.location = APPLY_NONE;
    af.bitvector = AFF_INFRAVISION;
    affect_join(ch, &af, FALSE, FALSE, FALSE, FALSE);
    break;

  case RACE_MINOTAUR:

    if ( GET_SKILL(ch, SKILL_GORE) < 60 ) {
      SET_SKILL(ch, SKILL_GORE, 60);
    }

//  GET_AC(ch) -= 12;
  break;    

  case RACE_KENDER:
    af.type = SPELL_INFRAVISION;
    af.duration = -1;
    af.modifier = 0;
    af.location = APPLY_NONE;
    af.bitvector = AFF_INFRAVISION;
    affect_join(ch, &af, FALSE, FALSE, FALSE, FALSE);

    if ( GET_SKILL(ch, SKILL_HIDE) < 60 ) {
      SET_SKILL(ch, SKILL_HIDE, 60);
    }
    if ( GET_SKILL(ch, SKILL_SNEAK) < 60 ) {
      SET_SKILL(ch, SKILL_SNEAK, 60);
    }
    if ( GET_SKILL(ch, SKILL_PICK_LOCK) < 60 ) {
      SET_SKILL(ch, SKILL_PICK_LOCK, 60);
    }

    if ( GET_SKILL(ch, SKILL_STEAL) < 60 ) {
      SET_SKILL(ch, SKILL_STEAL, 60);
    }

    break;

  default:
    break;
  }
}



long find_race_bitvector(char arg)                                   
{                                                                    
  arg = LOWER(arg);                                                  
                                                                     
  switch (arg) {                                                     
    case 'a':                                                        
      return (1 << 0);                     
      break;                                                         
    case 'b':                                                        
      return (1 << 1);                                                      
      break;                                                         
    case 'c':                                                        
      return (1 << 2);                                                      
      break;                                                         
    case 'd':                                                        
      return (1 << 3);                                                      
      break;                                                         
    case 'e':                                                        
      return (1 << 4);                                                      
      break;                                                         
    case 'f':                                                        
      return (1 << 5);                                                      
      break;                                                         
    case 'g':                                                        
      return (1 << 6);                                                      
      break;                                                         
    case 'h':                                                        
      return (1 << 7);                                                      
      break;                                                         
    case 'i':                                                        
      return (1 << 8);                                                      
      break;                                                         
    case 'j':                                                        
      return (1 << 9);                                                      
      break;                                                         
    default:                                                         
      return 0;                                                      
      break;                                                         
  }                                                                  
}


int raceBonus[NUM_RACES][6] = {
  /* St  In  Wi  De  Co  Ch              */
  {   1, -1,  0,  0,  0,  0  },   /* Bar */
  {   0,  0,  0,  0,  0,  0  },   /* Hyl */
  {   0,  0,  0,  0,  0,  0  },   /* Dae */
  {   0,  0,  0,  0,  0,  0  },   /* Nei */
  {   0,  0,  0,  0,  0,  0  },   /* Sil */
  {   0,  0,  0,  0,  0,  0  },   /* Qua */
  {   0,  0,  0,  0,  0,  0  },   /* Kag */
  {   0,  0,  0,  0,  0,  0  },   /* Hal */
  {  -1,  0,  0,  1,  0,  0  },   /* Ken */
  {   2,  0,  0, -1,  0, -1  },   /* Min */
  {   0,  0,  0,  0,  0,  0  },   /* Gno */
  {   0,  0,  0,  0,  0,  0  }    /* Hum */
};

int raceAbils[NUM_RACES][6][2] = {
  /*  Str       Int       Wis       Dex       Con       Cha                */
  { {10,18 }, { 7,18 }, { 8,18 }, { 8,16 }, {12,18 }, { 4,18 } },   /* Bar */
  { { 8,18 }, { 7,18 }, { 6,18 }, { 5,17 }, {12,19 }, { 4,16 } },   /* Hyl */
  { { 8,18 }, { 7,18 }, { 6,18 }, { 5,17 }, {12,19 }, { 4,16 } },   /* Dae */
  { { 9,18 }, { 7,18 }, { 6,18 }, { 5,17 }, {14,19 }, { 4,12 } },   /* Nei */
  { { 6,18 }, {10,18 }, { 6,18 }, { 7,19 }, { 8,18 }, {12,18 } },   /* Sil */
  { { 7,18 }, { 8,18 }, { 6,18 }, { 7,19 }, { 8,18 }, { 8,18 } },   /* Qua */
  { { 8,18 }, { 7,12 }, { 8,18 }, { 8,19 }, { 8,18 }, { 8,18 } },   /* Kag */
  { { 6,18 }, { 7,18 }, { 6,18 }, { 6,18 }, { 8,18 }, { 4,18 } },   /* Hal */
  { { 6,16 }, { 7,18 }, { 6,16 }, { 8,19 }, { 8,18 }, { 6,18 } },   /* Ken */
  { {16,20 }, { 6,18 }, { 6,18 }, { 6,18 }, {12,20 }, { 4,16 } },   /* Min */
  { { 6,18 }, {10,19 }, { 6,12 }, { 8,18 }, { 8,18 }, { 6,18 } },   /* Gno */
  { { 6,18 }, { 7,18 }, { 6,18 }, { 5,18 }, { 8,18 }, { 4,18 } }    /* Hum */
};


/*
 * invalid_race is used by handler.c to determine if a piece of equipment is
 * usable by a particular race, based on the ITEM_ANTI_{race} bitvectors.
 */
int invalid_race(struct char_data *ch, struct obj_data *obj) {

  if ( IS_OBJ_STAT(obj, ITEM_ANTI_BARBAR) && IS_BARBARIAN(ch) ) {
    return 1;
  }

  if ( IS_OBJ_STAT(obj, ITEM_ANTI_DWARF) &&
       ( IS_HYLAR(ch) || IS_DAEWAR(ch) || IS_NEIDAR(ch) ) ) {
    return 1;
  }

  if ( IS_OBJ_STAT(obj, ITEM_ANTI_ELF) &&
       ( IS_SILVANESTI(ch) || IS_QUALINESTI(ch) || IS_KAGONESTI(ch) ) ) {
    return 1;
  }

  if ( IS_OBJ_STAT(obj, ITEM_ANTI_HALFELF) && IS_HALFELVEN(ch) ) {
    return 1;
  }

  if ( IS_OBJ_STAT(obj, ITEM_ANTI_KENDER) && IS_KENDER(ch) ) {
    return 1;
  }

  if ( IS_OBJ_STAT(obj, ITEM_ANTI_MINOTAUR) && IS_MINOTAUR(ch) ) {
    return 1;
  }

  if ( IS_OBJ_STAT(obj, ITEM_ANTI_GNOME) && IS_GNOME(ch) ) {
    return 1;
  }

  if ( IS_OBJ_STAT(obj, ITEM_ANTI_HUMAN) && IS_HUMAN(ch) ) {
    return 1;
  }


  return 0;
}
