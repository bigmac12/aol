/* ************************************************************************
*   File: spell_parser.c                                Part of CircleMUD *
*  Usage: top-level magic routines; outside points of entry to magic sys. *
*                                                                         *
*  All rights reserved.  See license.doc for complete information.        *
*                                                                         *
*  Copyright (C) 1993, 94 by the Trustees of the Johns Hopkins University *
*  CircleMUD is based on DikuMUD, Copyright (C) 1990, 1991.               *
************************************************************************ */

#include <math.h>
#include "conf.h"
#include "sysdep.h"
#include "structs.h"
#include "utils.h"
#include "interpreter.h"
#include "spells.h"
#include "handler.h"
#include "comm.h"
#include "db.h"

struct spell_info_type spell_info[TOP_SPELL_DEFINE + 1];
int spell_access(struct char_data * ch, int spellnum);
int find_dash_skill_num(char *name);

#define SINFO spell_info[spellnum]

extern struct room_data *world;
extern struct index_data *obj_index;
extern struct casting_type caster_info[NUM_CLASSES];
extern struct element_type elements[];
extern int mem_apply[26];
extern int min_memtime[10];
extern int start_memtime[10];
//extern struct component_type spell_components[];
extern struct area_aff_data *area_spells_list;

int check_components(struct char_data * ch, int spellnum);
void list_components(struct char_data * ch, int spellnum);
int mag_materials(struct char_data *ch, int item0, int extract, int position);
int consume_components(struct char_data * ch, int spellnum);
void mag_area_magic(int level, struct char_data * ch, int spellnum);

/*
 * This arrangement is pretty stupid, but the number of skills is limited by
 * the playerfile.  We can arbitrarily increase the number of skills by
 * increasing the space in the playerfile. Meanwhile, this should provide
 * ample slots for skills.
 */

char *get_spell_name(char *argument)
{
  char *s;

  s = strtok(argument, "'");
  s = strtok(NULL, "'");
  
  return s;
}

char *spells[] =
{
  "!RESERVED!",			/* 0 - reserved */

  /* SPELLS */

  "armor",			/* 1 */
  "teleport",
  "bless",
  "blindness",
  "burning hands",
  "call lightning",
  "charm person",
  "chill touch",
  "blur",
  "mass charm",			/* 10 */
  "thornflesh",
  "create food",
  "create water",
  "cure blind",
  "cure critical",
  "cure light",
  "bestow curse",
  "detect alignment",
  "detect invisibility",
  "detect magic",		/* 20 */
  "detect poison",
  "dispel evil",
  "cripple",
  "enchant weapon",
  "energy drain",
  "fireball",
  "harm",
  "heal",
  "invisibility",
  "lightning bolt",		/* 30 */
  "locate object",
  "magic missile",
  "poison",
  "protection from evil",
  "remove curse",
  "greater sanctuary",
  "shocking grasp",
  "sleep",
  "enhance strength",
  "summon",			/* 40 */
  "shield",
  "word of recall",
  "neutralize poison",
  "sense life",
  "legend lore",		/* 45 */
  "dispel good",
  "minor sanctuary",
  "healing light",
  "prayer",
  "infravision",		/* 50 */
  "water walk",
  "barkskin",
  "stoneskin",
  "harmful wrath",
  "cure deafness",		/* 55 */
  "statue",
  "mirror image",
  "flight",
  "weakness",
  "deafening wind",		/* 60 */
  "cure disease",
  "remove paralysis",
  "restoration",
  "regeneration",
  "haste",			/* 65 */
  "disease",
  "paralyze",
  "soul drain",
  "wither",
  "slow",			/* 70 */
  "portal",
  "elemental storm",
  "create spring",
  "protection from good",
  "protection from undead",	/* 75 */
  "protection from fire",
  "protection from frost",
  "protection from elements",
  "impervious mind",
  "calm",			/* 80 */
  "cloak of fear",
  "inspire fear",
  "heroes feast",
  "bravery",
  "greater elemental",		/* 85 */
  "!UNUSED!",
  "blade barrier",
  "faerie fire",
  "sunray",
  "moonbeam",			/* 90 */
  "dispel magic",
  "magic light",
  "darkness",
  "silence",
  "sphere of silence",		/* 95 */
  "cure moderate",
  "cure severe",
  "cause light",
  "cause moderate",
  "cause severe",		 /* 100 */
  "cause critical",
  "goodberry",
  "magical vestment",
  "free action",
  "magical stone",		 /* 105 */
  "shillelagh",
  "spiritual hammer",
  "flamestrike",
  "aid",
  "call animal spirit",		/* 110 */
  "summon insects",
  "animal summoning one",
  "animal summoning two",
  "animal summoning three",
  "creeping doom",		/* 115 */
  "insect plague",
  "rainbow",
  "entangle",
  "thornshield",
  "sticks to snakes",		/* 120 */
  "aerial servant",
  "summon guard",
  "dust devil",
  "flame blade",
  "water breathing",		/* 125 */
  "conjure elemental",
  "wind walk",
  "phantasmal snake",
  "identify",
  "breath of life",		/* 130 */
  "black plague",
  "reflecting pool",
  "recollection",
  "remove fear",
  "divine wrath",		/* 135 */
  "holy word",
  "mystic spirit",
  "storm summoning",
  "dehydration",
  "mire",			/* 140 */
  "abjure",
  "adamant mace",
  "hold person",
  "web",
  "gust of wind",		/* 145 */
  "minor globe",
  "major globe",
  "vampiric touch",
  "dancing sword",
  "scry",			/* 150 */
  "chain lightning",
  "teleport object",
  "arrow of bone",
  "cone of cold",
  "fire shield",		/* 155 */
  "blackmantle",
  "blink",
  "prismatic spray",
  "meteor swarm",
  "magical suscept",		/* 160 */ 
  "antimagic aura",
  "deceptive imagery",
  "spell turning",
  "recharge item",
  "vampiric aura",		/* 165 */
  "refresh",
  "power word kill",
  "conceal alignment",
  "polymorph", 
  "reveal illusion",   /* 170 */
  "enhance agility",
  "enhance endurance",
  "enhance insight",
  "enhance cunning",
  "enhance charisma", 
  "blessing of kings",
  "knightly challenge", 
  "assorted skills", 

  "!UNUSED!", "!UNUSED!",   /* 180 */
  "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!",   /* 185 */
  "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!",   /* 190 */
  "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!",   /* 195 */
  "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!",   /* 200 */

  /* SKILLS */

  "backstab",                   /* 201 */
  "bash",
  "hide",
  "kick",
  "pick locks",  /* 205 */
  "bandage",
  "rescue",
  "sneak",
  "steal",
  "track",                      /* 210 */
  "scan",
  "second attack",
  "third attack",
  "retreat",
  "dual wield",    /* 215 */
  "dodge",
  "disarm",
  "assess",
  "guard",
  "lay hands",  /* 220 */
  "natural hardiness",
  "scribe",
  "brew",
  "sense poison",   // DUPE spell name --gan
  "remove poison",  /* 225 */
  "defensive stance",
  "gore",
  "read magic",
  "assess magic",
  "envenom",   /* 230 */
  "disguise",
  "turn undead",
  "control undead",
  "calm",
  "fourth attack",  /* 235 */
  "throw",
  "chakra",
  "knockout",
  "parry",
  "pass without trace",   /* 240 */
  "trip",
  "riding", 
  "tame", 
  "skin", 
  "jawstrike",  /* 245 */
  "dirtkick", 
  "build", 
  "cook", 
  "detect disguise", 
  "enchant item",   /* 250 */
  "two handed style", 
  "two weapon style", 
  "one weapon style", 
  "sword and shield style", 
  "charge",   /* 255 */
  "improved charge", 
  "improved backstab", 
  "improved stealth", 
  "advanced backstab", 
  "skill timer",   /* 260 */
  "armor mastery", 
  "shield mastery", 
  "improved armor mastery", 
  "advanced armor mastery", 
  "improved shield mastery",   /* 265 */
  "advanced shield mastery", 
  "damage reduction", 
  "improved damage reduction", 
  "advanced damage reduction", 
  "dirty fighting",   /* 270 */
  "combat improvisation",
  "martial arts",
  "improved martial arts",
  "advanced martial arts", 
  "insight",   /* 275 */
  "unarmed defense", 
  "improved unarmed defense", 
  "advanced unarmed defense", 
  "throw opponent", 
  "challenge",   /* 280 */
  "mark opponent", 
  "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!",   /* 285 */
  "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!",   /* 290 */
  "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!",   /* 295 */
  "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!",   /* 300 */

  /* OBJECT SPELLS AND NPC SPELLS/SKILLS */

  "!UNUSED!",
  "fire breath",
  "gas breath",
  "frost breath",
  "acid breath",
  "lightning breath",

  "\n"				/* the end */
};

char *potion_names[] = 
{
	"milky white",
	"bubbling white",
	"glowing ivory",
	"glowing blue",
	"bubbling yellow",
	"light green",
	"gritty brown",
	"blood red",
	"swirling purple",
	"flickering green",
	"cloudy blue",
	"glowing red",
	"sparkling white",
    "bubbling green",
    "swirling red and blue",
    "sparkling blue",
    "clear blue",
    "iridescent green",
    "reddish brown",
    "aquamarine",
    "iridescent pink",
    "pink",
    "filmy red",
    "gray",
    "dark green",
    "sparkling red",
    "cloudy green",
    "swirling magenta",
    "flickering red",
    "dark blue",
    "cobalt blue",
    "tan",
    "dark yellow",
    "bright green",
    "bright blue",
    "sparkling ivory" 
};

struct syllable 
{
  char *org;
  char *new;
};


struct syllable syls[] = 
{
  {" ", " "},
  {"anger", "amarah"},
  {"dart", "anak"},
  {"missile", "anak"},
  {"fire", "api"},
  {"acid", "asam"},
  {"passion", "asmarah"},
  {"bull", "banteng"},
  {"hidden", "batin"},
  {"wind", "belit"},
  {"haste", "benci"},
  {"shape", "bentuk"},
  {"water", "berair"},
  {"walk", "berjalan"},
  {"blade", "bisau"},
  {"bird", "burun"},
  {"owl", "burun'hantu"},
  {"raven", "burun'hitim"},
  {"spoiled", "busuk"},
  {"putrid", "busuk"},
  {"melt", "cair"},
  {"paralyze", "capik"},
  {"paralysis", "capik"},
  {"cat", "cas"},
  {"swift", "cepat"},
  {"bright", "cerlang"},
  {"mirror", "cermin"},
  {"reflect", "cermin"},
  {"small", "cilin"},
  {"steal", "colang"},
  {"color", "corak"},
  {"peace", "damai"},
  {"blood", "darah"},
  {"power", "daya"},
  {"cold", "dingin"},
  {"dragon", "draconis"},
  {"dark", "dumak"},
  {"circle", "edar"},
  {"hawk", "elang"},
  {"eagle", "elang"},
  {"gold", "emas"},
  {"emotion", "emosi"},
  {"ice", "es"},
  {"large", "gedeng"},
  {"lightning", "halilintar"},
  {"ghost", "hantu"},
  {"black", "hitim"},
  {"air", "imbis"},
  {"hand", "kartu"},
  {"mimic", "keajukun"},
  {"copy", "keajukun"},
  {"duplicate", "keajukun"},
  {"life", "keawetan"},
  {"barrier", "kendala"},
  {"wall", "kendala"},
  {"illusion", "khalayan"},
  {"love", "kinta"},
  {"hide", "kulit"},
  {"spider", "labala"},
  {"like", "laksana"},
  {"similar", "laksana"},
  {"sphere", "lingkaran"},
  {"eye", "mata"},
  {"dead", "mati"},
  {"body", "mayat"},
  {"see", "mencelik"},
  {"sight", "penglihatan"},
  {"silver", "perak"},
  {"change", "perubahan"},
  {"shift", "perubahan"},
  {"dazzle", "persona"},
  {"enchantment", "persona"},
  {"featherfall", "pveatherfal"},
  {"feather fall", "pveatherfal"},
  {"fox", "rubah"},
  {"friend", "sahabat"},
  {"black magic", "santet"},
  {"more", "sekali"},
  {"safe", "selemat"},
  {"touch", "sentu"},
  {"light", "shirak"},
  {"magic", "sihir"},
  {"invisible", "tak'kelihatan"},
  {"invisibility", "tak'kelihatan"},
  {"unseen", "tak'kelihatan"},
  {"earth", "tanah"},
  {"shadow", "tanda"},
  {"sleep", "tidur"},
  {"slumber", "tidur"},
  {"snake", "ular"},
  {"i", "aku"},
  {"you", "ente"},
  {"yours", "mu"},
  {"me", "saya"},
  {"m", "mas"},
  {"we", "kami"},
  {"us", "kami"},
  {"he", "dia"},
  {"she", "betina"},
  {"it", "itu"},
  {"all", "semua"},
  {"away", "ayun"},
  {"every", "saba"},
  {"from", "dari"},
  {"only", "esa"},
  {"over", "berlua"},
  {"to", "ke"},
  {"un", "nir"},
  {"under", "bawa"},
  {"within", "dalam"},
  {"inside", "dalam"},
  {"one", "eka"},
  {"two", "dua"},
  {"three", "tiga"},
  {"four", "empat"},
  {"five", "elak"},
  {"six", "enam"},
  {"seven", "sapta"},
  {"eight", "delapan"},
  {"nine", "sembilan"},
  {"ten", "sepulah"},

  {"a", "i"}, {"b", "v"}, {"c", "q"}, {"d", "m"}, {"e", "o"}, {"f", "y"}, {"g", "t"},
  {"h", "p"}, {"j", "y"}, {"k", "t"}, {"l", "r"}, {"n", "b"},
  {"o", "a"}, {"p", "s"}, {"q", "d"}, {"r", "f"}, {"s", "g"}, {"t", "h"}, {"u", "e"},
  {"v", "z"}, {"w", "x"}, {"x", "n"}, {"y", "l"}, {"z", "k"}, {"", ""}
};

int has_materials(struct char_data * ch, int spellnum)
{
    switch (spellnum) 
    {
        case SPELL_LOCATE_OBJECT:
            return (mag_materials(ch, 27, FALSE, CMP_INV));
            break;
        case SPELL_SUMMON:
            return (mag_materials(ch, 28, FALSE, CMP_HOLD));
            break;
        case SPELL_DANCING_SWORD:
            return (mag_materials(ch, 29, FALSE, CMP_INV));
            break;
        case SPELL_FIRE_SHIELD:
            return (mag_materials(ch, 31, FALSE, CMP_INV));
            break;
        case SPELL_ANTIMAGIC_AURA:
            return (mag_materials(ch, 32, FALSE, CMP_HOLD));
            break;
        case SPELL_PRISMATIC_SPRAY:
            return (mag_materials(ch, 33, TRUE, CMP_INV));
            break;
        case SPELL_ENCHANT_WEAPON:
            return (mag_materials(ch, 35, TRUE, CMP_INV));
            break;
        case SPELL_IDENTIFY:
        case SKILL_ENCHANT_ITEM:
            return (mag_materials(ch, 36, TRUE, CMP_INV));
            break;
        case SPELL_HASTE:
            return (mag_materials(ch, 37, TRUE, CMP_INV));
            break;
        case SPELL_CHARM:
            return (mag_materials(ch, 38, TRUE, CMP_HOLD));
            break;
        case SPELL_SCRY:
            return (mag_materials(ch, 27, FALSE, CMP_INV));
            break;
        case SPELL_BLACKMANTLE:
            return (mag_materials(ch, 40, TRUE, CMP_HOLD));
            break;
        case SPELL_MAGICAL_SUSCEPT:
            return (mag_materials(ch, 41, TRUE, CMP_HOLD));
            break;
        case SPELL_DECEPTIVE_IMAGERY:
            return (mag_materials(ch, 42, FALSE, CMP_HOLD));
            break;
        case SPELL_LEGEND_LORE:
            return (mag_materials(ch, 43, TRUE, CMP_HOLD));
            break;
        case SPELL_MASS_CHARM:
            return (mag_materials(ch, 44, FALSE, CMP_INV));
            break;
        case SPELL_CRIPPLE:
            return (mag_materials(ch, 45, TRUE, CMP_HOLD));
            break;
        case SPELL_ENERGY_DRAIN:
            return (mag_materials(ch, 46, TRUE, CMP_INV));
            break;
        case SPELL_GATE:
            return (mag_materials(ch, 53, TRUE, CMP_HOLD));
            break;
        case SPELL_GREATER_ELEMENTAL:
            return (mag_materials(ch, 47, FALSE, CMP_INV));
            break;
        case SPELL_PHANTASMAL_SNAKE:
            return (mag_materials(ch, 48, TRUE, CMP_INV));
            break;
        case SPELL_RECHARGE_ITEM:
            return (mag_materials(ch, 49, TRUE, CMP_INV));
            break;
        case SPELL_SPELL_TURNING:
            return (mag_materials(ch, 50, TRUE, CMP_INV));
            break;
        case SPELL_RECOLLECTION:
            return (mag_materials(ch, 51, TRUE, CMP_HOLD));
            break;
        case SPELL_VAMPIRIC_AURA:
            return (mag_materials(ch, 52, TRUE, CMP_HOLD));
            break;
        case SPELL_SLEEP:
            return (mag_materials(ch, 54, TRUE, CMP_INV));
            break;
        default:
            return (mag_materials(ch, 0, TRUE, FALSE));
            break;
  }

  return 0;
}


int is_memmed(struct char_data * ch, int spellnum)
{
  int i, memming = 0;

  for (i = 0; i < MAX_SPELL_MEM; i++)
  {
    if (spell(ch, i).spellnum == spellnum)
    {
      if (spell(ch, i).ticks == 0)
        return 1;
      else
        memming = 1;
    }
  }

  if (memming)
    return -1;
  else
    return 0;
}

int get_mem_slot(struct char_data * ch, int spellnum)
{
  int i;

  if (is_memmed(ch, spellnum) < 1)
    return 0;

  for (i = 0; i < MAX_SPELL_MEM; i++)
  {
    if ((spell(ch, i).spellnum == spellnum) && (spell(ch, i).ticks == 0))
      return i;
  }

  return 0;
}

void clear_spell(struct char_data * ch, int slot)
{
  spell(ch, slot).spellnum = 0;
  spell(ch, slot).ticks = -1;
}

int caster_level(struct char_data * ch, int level)
{
  int i, j, max_level, first_level, caster_level, num_levels;

  max_level = caster_info[(int)GET_CLASS(ch)].num_spell_levels;
  first_level = caster_info[(int)GET_CLASS(ch)].starting_spell_level;
  caster_level = level - first_level + 1;
  num_levels = LVL_IMMORT - first_level;

  j = (caster_level * max_level) % num_levels;
  i = (caster_level * max_level) / num_levels;

  if (j)
    i++;

  return MIN(i, max_level);
}


int eff_spell_level(struct char_data * ch, int spellnum)
{
  int level = spell_info[spellnum].min_level[(int)GET_CLASS(ch)];

  return (caster_level(ch, level));
}

int num_slots(struct char_data * ch, int spell_level)
{
  int i, j;
  int number = caster_info[(int)GET_CLASS(ch)].num_spell_levels;
  int first = caster_info[(int)GET_CLASS(ch)].starting_spell_level;
  int mod = caster_info[(int)GET_CLASS(ch)].slotmod;
  //int max_level = 0;

  int wis_mod[26] = {
   -99,
   -95, -90, -80, -70, -60,
   -45, -30, -15, -10, -5,
    -1,   0,   0,   1,  2,
     5,  10,  15,  20,  25,
    30,  35,  40,  45,  50  };

  i = (((LVL_IMMORT - first) / number) * (spell_level - 1)) + (first - 1);

  if ((first == 1) && (spell_level == 1))
    i--;

  mod = ((100 + wis_mod[GET_WIS(ch)]) * mod) / 100;

  j = 0;
  while (i < (GET_MEM_LEVEL_FLAG(ch) ? GET_LEVEL(ch) - 1 : GET_LEVEL(ch)))
  {
    j++;
//  i += (((LVL_IMMORT - 1) * j) / (mod - (3 * spell_level)));
    i += MAX(((LVL_IMMORT - 1) * j) / MAX(mod - (3 * spell_level), 1), 1);
  }

  if (spell_level > caster_level(ch,
      GET_MEM_LEVEL_FLAG(ch) ? GET_LEVEL(ch) - 1 : GET_LEVEL(ch)))
    return 0;

  return j;
}

void assert_slots(struct char_data * ch)
{
  int i, level, lost = 0, slots[10];

  if (caster_info[(int)GET_CLASS(ch)].num_spell_levels < 1)
    return;

  for (i = 0; i < 10; i++)
    slots[i] = 0;

  for (i = 0; i < MAX_SPELL_MEM; i++)
  {
    if (spell(ch, i).spellnum)
    {
      level = eff_spell_level(ch, spell(ch, i).spellnum);
      slots[level]++;
      if (slots[level] > num_slots(ch, level))
      {
        lost++;
        clear_spell(ch, i);
      }
    }
  }

  if (lost)
    send_to_char("You lack the wisdom to recall so many spells.\r\n", ch);

  return;
} 

int free_slots(struct char_data * ch, int slot)
{
  int slots = 0, max, i;

  max = num_slots(ch, slot);

  for (i = 0; i < MAX_SPELL_MEM; i++)
  {
    if (spell(ch, i).spellnum &&
       (eff_spell_level(ch, spell(ch, i).spellnum) == slot))
      slots++;
  }

  if (slots > max)
  {
    assert_slots(ch);
    return 0;
  }

  if (slots == max)
    return 0;

  return (max - slots);
}

void update_mem(struct char_data * ch)
{
  int i;

  if (caster_info[(int)GET_CLASS(ch)].num_spell_levels < 1)
    return;

  for (i = 0; i < MAX_SPELL_MEM; i++)
  {
    if (spell(ch, i).spellnum && (spell(ch, i).ticks > 0))
    {
      if ((GET_POS(ch) == POS_RESTING) || (GET_POS(ch) == POS_SITTING))
      {
         spell(ch, i).ticks -= 3;
      }
      else if (GET_POS(ch) == POS_STANDING)
      {
         spell(ch, i).ticks -= 1;
      }

      if (spell(ch,i).ticks < 0)
         spell(ch,i).ticks = 0;

      if (spell(ch, i).ticks == 0)
      {
        sprintf(buf, "You have completed memorizing %s.\r\n", spells[spell(ch, i).spellnum]);
        send_to_char(buf, ch);
      }
    }
  }
}

void update_skills(struct char_data * ch)
{
  int i;

  ch->reflection_ticks = MAX(0, ch->reflection_ticks - 1);

  if (ch->reflection_ticks > 1)
    return;

  if (ch->standcounter < 2)
    return;

  ch->reflection_ticks = 24;

  for (i = 1; i <= MAX_SKILLS; i++)
  {
    if (GET_SKILL(ch, i) && (GET_SKILL(ch, i) < spell_info[i].max_percent) &&
       (ch->player_specials->saved.skills[i].reflections))
    {
      sprintf(buf, "You reflect on your skill at '%s'.\r\n", spells[i]);
      send_to_char(buf, ch);

      while ((GET_SKILL(ch, i) < spell_info[i].max_percent) &&
             (ch->player_specials->saved.skills[i].reflections > 0))
      {
        GET_SKILL(ch, i)++;
        ch->player_specials->saved.skills[i].reflections--;
      }
    }

    if (ch->player_specials->saved.skills[i].reflections < 0)
      ch->player_specials->saved.skills[i].reflections = 0;

  }
}

int mem_int_app(struct char_data * ch)
{
  return -mem_apply[GET_INT(ch)];
}

void shift_memtimes(struct char_data * ch, int shift)
{
  int i, min;

  for (i = 0; i < MAX_SPELL_MEM; i++)
  {
    if (spell(ch, i).spellnum && (spell(ch, i).ticks  > 0))
    {
      if (shift > 0)
        spell(ch, i).ticks += shift;
      else
      {
        min = min_memtime[eff_spell_level(ch, spell(ch, i).spellnum)];
        if (spell(ch, i).ticks > min)
        {
          spell(ch, i).ticks += shift;
          spell(ch, i).ticks = MAX(spell(ch, i).ticks, min);
        }
      }
    }
  }
}

int memorize_spell(struct char_data * ch, int spellnum)
{
  int i, tick, level, found = 0, same = 0, max = 0;
  int placeholder = -1;

  level = eff_spell_level(ch, spellnum);
  if (!free_slots(ch, level))
  {
    send_to_char("You cannot mem any more spells at that level.\r\n", ch);
    return 0;
  }

  for (i = 0; i < MAX_SPELL_MEM; i++)
  {
    if ((spell(ch, i).spellnum == 0) && !found)
    {
      spell(ch, i).spellnum = spellnum;
      found = 1;
      placeholder = i;
    }
    else if (eff_spell_level(ch, spell(ch, i).spellnum) == level)
    {
      same++;
      if (spell(ch, i).ticks > max)
        max = spell(ch, i).ticks;
    }
  }

  if (!found)
  {
    send_to_char("You cannot memorize any more spells.\r\n", ch);
    return 0;
  }

  i = placeholder;
  if (!max)
  {
    spell(ch, i).ticks = start_memtime[level];
  }
  else
  {
    same = same / 3;
    tick = level * same;
    if (tick < (max + (level / 3) - mem_int_app(ch)))
      tick = max + (level / 3);
    spell(ch, i).ticks = MAX(tick, start_memtime[level]);
  }

  spell(ch, i).ticks += mem_int_app(ch);
  spell(ch, i).ticks = MAX(spell(ch, i).ticks, min_memtime[level]);

  return 1;
}

void do_mem_table(struct char_data * ch)
{
  int count, h, c, k;

  int i, j, max_level;

  if ((caster_info[(int)GET_CLASS(ch)].num_spell_levels == 0) ||
      (caster_info[(int)GET_CLASS(ch)].starting_spell_level > GET_LEVEL(ch)))
  {
    sprintf(buf, "SYSERR: do_mem_table called by non_casting player!");
    mudlog(buf, BRF, LVL_DEMIGOD, TRUE);
    return;
  }

  max_level = caster_level(ch, GET_LEVEL(ch));

  sprintf(buf, "Spells memorized:");
  for (i = 1; i <= max_level; i++)
  {
    count = 0;
    for (h = 1; h < MAX_SPELLS; h++)
    {
      if ((eff_spell_level(ch, h) == i) && (is_memmed(ch, h) > 0))
      {
        for (j = 0, c = 0; j < MAX_SPELL_MEM; j++)
        {
          if ((spell(ch, j).spellnum == h) && (spell(ch, j).ticks == 0))
            c++;
        }

        if (c > 0)
        {
          if (count == 0)
            sprintf(buf + strlen(buf), "\r\n%2d: ", i);
          if (count && (count % 3 == 0))
            strcat(buf, "\r\n    ");
          sprintf(buf + strlen(buf), "(%2d) %-19s", c, spells[h]);
          count++;
        }
      }
    }
  }

 strcat(buf, "\r\n\r\nSpells being memorized:");
  for (i = 1; i <= max_level; i++)
  {
    count = 0;
    for (j = 0; j < MAX_SPELL_MEM; j++)
    {
      if ((spell(ch, j).spellnum > 0) && (spell(ch, j).ticks > 0) &&
          (eff_spell_level(ch, spell(ch, j).spellnum) == i))
      {
        int ticktime = 0;
        if (count == 0)
          sprintf(buf + strlen(buf), "\r\n%2d: ", i);
        if (count && (count % 3 == 0))
          strcat(buf, "\r\n    ");

        ticktime = (spell(ch,j).ticks + (ch->standcounter ? 0 : 1));

        if ((GET_POS(ch) == POS_RESTING) || (GET_POS(ch) == POS_SITTING))
        {
          // Where the hell did ceil() go? --nui
          ticktime = (int) ceil((double) ticktime / 3.0);
        }

        sprintf(buf + strlen(buf), "(%2dh) %-18s", ticktime,
                spells[spell(ch, j).spellnum]);
        count++;
      }
    }
  }


  strcat(buf, "\r\n\r\nSpells left:\r\n  ");
  for (i = 1; i <= max_level; i++)
  {
    k = num_slots(ch, i);
    sprintf(buf + strlen(buf), "%d: %d/%d  ", i, free_slots(ch, i), k);
  }
  strcat(buf, "\r\n");

  send_to_char(buf, ch);
  return;
}

ACMD(do_memorize)
{
  int spellnum;
  char s[MAX_INPUT_LENGTH];

  if ((caster_info[(int)GET_CLASS(ch)].num_spell_levels == 0) ||
      (caster_info[(int)GET_CLASS(ch)].starting_spell_level > GET_LEVEL(ch)))
  {
    send_to_char("But you know nothing about spellcasting!\r\n", ch);
    return;
  }

  any_one_arg(argument, s);

  if ((s == NULL) || !strcmp(s, ""))
  {
    do_mem_table(ch);
    return;
  }

  spellnum = find_dash_skill_num(s);

  if ((spellnum < 1) || (spellnum > MAX_SPELLS)) {
    send_to_char("Memorize what?\r\n", ch);
    return;
  }

  if (GET_SKILL(ch, spellnum) == 0)
  {
    send_to_char("You are unfamiliar with that spell.\r\n", ch);
    return;
  }

  if (!spell_access(ch, spellnum))
  {
    send_to_char("You do not know that spell.\r\n", ch);
    return;
  }

  if (memorize_spell(ch, spellnum))
    send_to_char("Ok.\r\n", ch);
  return;
}
 
ACMD(do_forget)
{
  int i, spellnum, low, ilow;
  char s[MAX_STRING_LENGTH];

  any_one_arg(argument, s);

  if ((s == NULL) || (!strcmp(s, "")))
  {
    send_to_char("Forget what?\r\n", ch);
    return;
  }

  if (strcmp(s, "all") == 0)
  {
     int j = 0;
     for (j = 0; j < MAX_SPELL_MEM; j++)
     {
       clear_spell(ch, j);
     }
     return;
  }
  else
    spellnum = find_dash_skill_num(s);

  if ((spellnum < 1) || (spellnum > MAX_SPELLS))
  {
    send_to_char("What spell?\r\n", ch);
    return;
  }

  if (!is_memmed(ch, spellnum))
  {
    send_to_char("But you're not even memorizing it!\r\n", ch);
    return;
  }

  low = -1;
  ilow = -1;

  for (i = 0; i < MAX_SPELL_MEM; i++)
  {
    if ((spell(ch, i).spellnum == spellnum) && (spell(ch, i).ticks > low))
    {
      low = spell(ch, i).ticks;
      ilow = i;
    }
  }

  if (ilow == -1)
  {
    sprintf(buf, "SYSERR: ilow of -1 in do_forget");
    mudlog(buf, BRF, LVL_DEMIGOD, TRUE);
    return;
  }

  clear_spell(ch, ilow);
  send_to_char("Ok.\r\n", ch);
  return;
}

ACMD(do_exchange)
{
  int level, spell1, spell2, i;

  char s[MAX_STRING_LENGTH], t[MAX_STRING_LENGTH];

  /*  A bit recursive, but it works.  s is the first, t the second.  Soli  */
  any_one_arg(any_one_arg(argument, s), t);

  if ((caster_info[(int)GET_CLASS(ch)].num_spell_levels == 0) ||
      (caster_info[(int)GET_CLASS(ch)].starting_spell_level > GET_LEVEL(ch)))
  {
    send_to_char("You don't have spells in the first place!\r\n", ch);
    return;
  }

  if (!affected_by_spell(ch, SPELL_RECOLLECTION))
  {
    send_to_char("You cannot exchange spells without magical aid.\r\n", ch);
    return;
  }

  spell1 = find_dash_skill_num(s);  // the trade-in
  spell2 = find_dash_skill_num(t);  // the new spell

  if ((spell1 < 1) || (spell2 < 1) ||
      (spell1 > MAX_SPELLS) || (spell2 > MAX_SPELLS))
  {
    send_to_char("Exchange what?\r\n", ch);
    return;
  }

  if ((GET_SKILL(ch, spell1) == 0) || (GET_SKILL(ch, spell2) == 0))
  {
    send_to_char("You don't know both of those spells.\r\n", ch);
    return;
  }

  if (!spell_access(ch, spell1) || !spell_access(ch, spell2))
  {
    send_to_char("Try spells you can actually cast instead.\r\n", ch);
    return;
  }

  level = eff_spell_level(ch, spell1);
  if (level != eff_spell_level(ch, spell2))
  {
    send_to_char("Those spells aren't of the same levels.\r\n", ch);
    return;
  }

  if (!is_memmed(ch, spell1))
  {
    send_to_char("But you don't even have that spell memorized!\r\n", ch);
    return;
  }

  if (ch->exchange < level)
  {
    if (ch->exchange > 1)
      sprintf(buf, "You can only exchange %d more spell levels.\r\n",
              ch->exchange);
    else if (ch->exchange == 1)
      sprintf(buf, "You have only one spell level remaining.\r\n");
    else
      sprintf(buf, "You have no more spell levels available to exchange.\r\n");

    send_to_char(buf, ch);
    return;
  }

  i = get_mem_slot(ch, spell1);

  if (spell(ch, i).ticks)
    return;

  ch->exchange -= level;
  spell(ch, i).spellnum = spell2;

  if (ch->exchange > 1)
    sprintf(buf, "Done.  %d spell levels remain.", ch->exchange);
  else if (ch->exchange == 1)
    sprintf(buf, "Done.  One spell level remains.");
  else
    sprintf(buf, "Done.  No spell levels remain.");

  send_to_char(buf, ch);
}

ACMD(do_spells)
{
  int i, j, count, max_level;

  if (IS_NPC(ch))
    return;

  strcpy(buf, "");
  max_level = caster_level(ch, GET_LEVEL(ch));

  for (i = 1; i <= max_level; i++)
  {
    count = 0;
    sprintf(buf2, "\r\n %d:  ", i);

    for (j = 0; j <= MAX_SPELLS; j++)
    {
      if(j != SPELL_DONTUSEME) {

      if (spell_access(ch, j) && (eff_spell_level(ch, j) == i))
      {
        count++;
        sprintf(buf2, "%s  %-20s", buf2, spells[j]);
        if (count == 3)
        {
          strcat(buf, buf2);
          sprintf(buf2, "\r\n     ");
          count = 0;
        }
      }
    }
}

    if (count)
      strcat(buf, buf2);
  }
  strcat(buf, "\r\n");

  page_string(ch->desc, buf, 1);
}

/* say_spell erodes buf, buf1, buf2 */
void say_spell(struct char_data * ch, int spellnum, struct char_data * tch, struct obj_data * tobj)
{
  char lbuf[256];

  struct char_data *i;
  int j, ofs = 0;

  *buf = '\0';
  strcpy(lbuf, spells[spellnum]);
  while (*(lbuf + ofs)) {
	for (j = 0; *(syls[j].org); j++) {
	  if (!strncmp(syls[j].org, lbuf + ofs, strlen(syls[j].org))) {
	strcat(buf, syls[j].new);
	ofs += strlen(syls[j].org);
	  }
	}
  }

  sprintf(lbuf, "$n utters the words, '%%s'.");

  sprintf(buf1, lbuf, spells[spellnum]);
  sprintf(buf2, lbuf, buf);

  for (i = world[ch->in_room].people; i; i = i->next_in_room) {
    if (i == ch || !i->desc || !AWAKE(i))
      continue;
    if (affected_by_spell(i, SPELL_DEAFENING_WIND))
    {
      act("$n utters some words, but you cannot hear them.",
           TRUE, ch, 0, i, TO_VICT);
      continue;
    }
    if (GET_LEVEL(ch) >= LVL_IMMORT && GET_LEVEL(i) >= LVL_IMMORT) {
		if ((IS_MAGE(ch) && IS_MAGE(i)) || (!IS_MAGE(ch)))
		  perform_act(buf1, ch, tobj, tch, i);
		else
		  perform_act(buf2, ch, tobj, tch, i);
    }

    if (GET_LEVEL(ch) < LVL_IMMORT) {
		if ((IS_MAGE(ch) && IS_MAGE(i)) || (!IS_MAGE(ch)))
		  perform_act(buf1, ch, tobj, tch, i);
		else
		  perform_act(buf2, ch, tobj, tch, i);
    }
  }
}

int is_majored(struct char_data * ch, struct char_data * victim, int spellnum)
{
  int slev;

  slev = eff_spell_level(ch, spellnum);

  if (((slev < 4) && affected_by_spell(victim, SPELL_MINOR_GLOBE)) ||
      ((slev < 5) && affected_by_spell(victim, SPELL_MAJOR_GLOBE)))
  {
    send_to_char("The spell is absorbed by the globe.\r\n", victim);
    act("The spell is absorbed by the globe.", 0, victim, 0, 0, TO_ROOM);
    return 1;
  }

  return 0;
}

int is_turned(struct char_data * ch, struct char_data * vict, int spellnum)
{
  int slev;
  struct affected_type *af, *next;

  slev = eff_spell_level(ch, spellnum);

  if (!affected_by_spell(vict, SPELL_SPELL_TURNING))
    return 0;

  for (af = vict->affected; af; af = next)
  {
    next = af->next;

    if (af->type == SPELL_SPELL_TURNING)
    {
      if (af->duration > slev)
        af->duration -= slev;
      else if (af->duration == -1)
        af->duration = -1;
      else                         // the spell turning ends
        affect_from_char(vict, SPELL_SPELL_TURNING);

      send_to_char("Your aura repels the offensive spell.\r\n", vict);
      act("$n turns the spell back upon $N!", TRUE, vict, 0, ch, TO_NOTVICT);
      act("$n turns the spell back upon you!", FALSE, vict, 0, ch, TO_VICT);

      return 1;
    }
  }
  CHOOSE_NAME(vict, victname);

  sprintf(buf, "SYSERR: Couldn't find spell_turning on %s!", victname);
  FREE_NAME(victname);
  mudlog(buf, BRF, LVL_GRGOD, TRUE);

  return 0;
}

char *skill_name(int num)
{
  int i = 0;

  if (num <= 0) {
    if (num == -1)
      return "UNUSED";
    else
      return "UNDEFINED";
  }

  while (num && *spells[i] != '\n') {
    num--;
    i++;
  }

  if (*spells[i] != '\n')
    return spells[i];
  else
    return "UNDEFINED";
}

int find_skill_num(char *name)
{
  int index = 0, ok;
  char *temp, *temp2;
  char first[256], first2[256];

  while (*spells[++index] != '\n') {
    if (is_abbrev(name, spells[index]))
      return index;

    ok = 1;
    temp = any_one_arg(spells[index], first);
    temp2 = any_one_arg(name, first2);
    while (*first && *first2 && ok) {
      if (!is_abbrev(first2, first))
	ok = 0;
      temp = any_one_arg(temp, first);
      temp2 = any_one_arg(temp2, first2);
    }

    if (ok && !*first2)
      return index;
  }

  return -1;
}

int find_dash_skill_num(char *name)
{
  int index = 0, ok;
  char *temp, *temp2;
  char first[256], first2[256];

  while (*spells[++index] != '\n')
  {
    if (is_abbrev(name, spells[index]))
      return index;

    ok = 1;
    temp = any_one_arg(spells[index], first);
    temp2 = one_arg_dash(name, first2);
    while (*first && *first2 && ok)
    {
      if (!is_abbrev(first2, first))
        ok = 0;
      temp = any_one_arg(temp, first);
      temp2 = one_arg_dash(temp2, first2);
    }

    if (ok && !*first2)
      return index;
  }

  return -1;
}

/*
 * This function is the very heart of the entire magic system.  All
 * invocations of all types of magic -- objects, spoken and unspoken PC
 * and NPC spells, the works -- all come through this function eventually.
 * This is also the entry point for non-spoken or unrestricted spells.
 * Spellnum 0 is legal but silently ignored here, to make callers simpler.
 */
int call_magic(struct char_data * caster, struct char_data * cvict, struct obj_data * ovict, int spellnum, int level, int casttype)
{
  int savetype;

  if (spellnum < 1 || spellnum > TOP_SPELL_DEFINE)
    return 0;

  if (ROOM_FLAGGED(caster->in_room, ROOM_NOMAGIC)) {
    send_to_char("Your magic fizzles out and dies.\r\n", caster);
    act("$n's magic fizzles out and dies.", FALSE, caster, 0, 0, TO_ROOM);
    return 0;
  }

  if (IS_SET(ROOM_FLAGS(caster->in_room), ROOM_PEACEFUL) &&
      (SINFO.violent || IS_SET(SINFO.routines, MAG_DAMAGE))) {
    send_to_char("A flash of white light fills the room, dispelling your violent magic!\r\n", caster);
    act("White light from no particular source suddenly fills the room, then vanishes.", FALSE, caster, 0, 0, TO_ROOM);
    return 0;
  }

/*  if (SINFO.violent || IS_SET(SINFO.routines, MAG_DAMAGE))
  {
    if (cvict)
    {
      if (caster == cvict)
      {
        send_to_char("You shouldn't cast that on yourself.  It could be bad for your health!\r\n", caster);
        return 0;
      }
    }
  } */

  GET_SPELL_SOURCE(caster) = casttype;

  /* determine the type of saving throw */
  switch (casttype) {
  case CAST_STAFF:
  case CAST_SCROLL:
  case CAST_POTION:
  case CAST_WAND:
    savetype = SAVING_ROD;
    break;
  case CAST_SPELL:
    savetype = SAVING_SPELL;
    break;
  default:
    savetype = SAVING_BREATH;
    break;
  }


  if (IS_SET(SINFO.routines, MAG_DAMAGE))
  {
    if (is_majored(caster, cvict, spellnum))
      return 1;
    if (is_turned(caster, cvict, spellnum))
      cvict = caster;
    mag_damage(level, caster, cvict, spellnum, savetype);
  }


  if (IS_SET(SINFO.routines, MAG_AFFECTS))
    mag_affects(level, caster, cvict, spellnum, savetype);

  if (IS_SET(SINFO.routines, MAG_UNAFFECTS))
    mag_unaffects(level, caster, cvict, spellnum, savetype);

  if (IS_SET(SINFO.routines, MAG_POINTS))
    mag_points(level, caster, cvict, spellnum, savetype);

  if (IS_SET(SINFO.routines, MAG_ALTER_OBJS))
    mag_alter_objs(level, caster, ovict, spellnum, savetype);

  if (IS_SET(SINFO.routines, MAG_GROUPS))
    mag_groups(level, caster, spellnum, savetype);

  if (IS_SET(SINFO.routines, MAG_MASSES))
    mag_masses(level, caster, spellnum, savetype);

  if (IS_SET(SINFO.routines, MAG_AREAS))
    mag_areas(level, caster, spellnum, savetype);

  if (IS_SET(SINFO.routines, MAG_SUMMONS))
    mag_summons(level, caster, cvict, ovict, spellnum, savetype);

  if (IS_SET(SINFO.routines, MAG_CREATIONS))
    mag_creations(level, caster, spellnum);

  if (IS_SET(SINFO.routines, MAG_AREA_MAGIC))
    mag_area_magic(level, caster, spellnum);

  if (IS_SET(SINFO.routines, MAG_MANUAL))
    switch (spellnum) {
    case SPELL_CHARM:		MANUAL_SPELL(spell_charm); break;
    case SPELL_CREATE_WATER:	MANUAL_SPELL(spell_create_water); break;
    case SPELL_DETECT_POISON:	MANUAL_SPELL(spell_detect_poison); break;
    case SPELL_ENCHANT_WEAPON:  MANUAL_SPELL(spell_enchant_weapon); break;
    case SPELL_IDENTIFY:	MANUAL_SPELL(spell_identify); break;
    case SPELL_LEGEND_LORE:     MANUAL_SPELL(spell_legend_lore); break;
    case SPELL_LOCATE_OBJECT:   MANUAL_SPELL(spell_locate_object); break;
    case SPELL_SUMMON:		MANUAL_SPELL(spell_summon); break;
    case SPELL_WORD_OF_RECALL:  MANUAL_SPELL(spell_recall); break;
    case SPELL_TELEPORT:        MANUAL_SPELL(spell_teleport); break;
    case SPELL_GATE:            MANUAL_SPELL(spell_gate); break;
    case SPELL_CREATE_SPRING:   MANUAL_SPELL(spell_create_spring); break;
    case SPELL_CALM:            MANUAL_SPELL(spell_calm); break;
    case SPELL_DISPEL_MAGIC:    MANUAL_SPELL(spell_dispel_magic); break;
    case SPELL_HOLY_WORD:       MANUAL_SPELL(spell_holy_word); break;
    case SPELL_REFLECTING_POOL: MANUAL_SPELL(spell_scry); break;
    case SPELL_SCRY:            MANUAL_SPELL(spell_scry); break;
    case SPELL_ABJURE:          MANUAL_SPELL(spell_abjure); break;
    case SPELL_GUST_OF_WIND:    MANUAL_SPELL(spell_gust); break;
    case SPELL_TELEPORT_OBJECT: MANUAL_SPELL(spell_teleport_object); break;
    case SPELL_ENERGY_DRAIN:    MANUAL_SPELL(spell_energy_drain); break;
    case SPELL_CHAIN_LIGHTNING: MANUAL_SPELL(spell_chain); break;
    case SPELL_RECHARGE_ITEM:   MANUAL_SPELL(spell_recharge_item); break;
    case SPELL_CRIPPLE:         MANUAL_SPELL(spell_cripple); break;
    case SPELL_REVEAL_ILLUSION: MANUAL_SPELL(spell_reveal_illusion); break;

    case SPELL_SHILLELAGH:
      if ( (ovict != NULL) &&
	   ( (isname("wood", ovict->name)) ||
	     (isname("wooden", ovict->name)) ) ) {
	MANUAL_SPELL(spell_enchant_weapon);
      } else {
	send_to_char("Sorry - the object must be made of wood.\r\n", caster);
	return 0;
      }
      break;

    }

  return 1;
}

/*
 * mag_objectmagic: This is the entry-point for all magic items.  This should
 * only be called by the 'quaff', 'use', 'recite', etc. routines.
 *
 * For reference, object values 0-3:
 * staff  - [0]	level	[1] max charges	[2] num charges	[3] spell num
 * wand   - [0]	level	[1] max charges	[2] num charges	[3] spell num
 * scroll - [0]	level	[1] spell num	[2] spell num	[3] spell num
 * potion - [0] level	[1] spell num	[2] spell num	[3] spell num
 * salve  - [0] level   [1] spell num   [2] spell num   [3] spell num
 *
 * Staves and wands will default to level 14 if the level is not specified;
 * the DikuMUD format did not specify staff and wand levels in the world
 * files (this is a CircleMUD enhancement).
 */

void mag_objectmagic(struct char_data * ch, struct obj_data * obj, char *argument)
{
  int i, k;
  struct char_data *tch = NULL, *next_tch;
  struct obj_data *tobj = NULL;
  int level, spell1, spell2, spell3;  // new scroll code - Soli, 10/99

  one_argument(argument, arg);

  k = generic_find(arg, FIND_CHAR_ROOM | FIND_OBJ_INV | FIND_OBJ_ROOM |
		   FIND_OBJ_EQUIP, ch, &tch, &tobj);

  switch (GET_OBJ_TYPE(obj)) {
  case ITEM_STAFF:
    act("You tap $p three times on the ground.", FALSE, ch, obj, 0, TO_CHAR);
      act("$n taps $p three times on the ground.", FALSE, ch, obj, 0, TO_ROOM);

    if (GET_OBJ_VAL(obj, 2) <= 0) {
      act("It seems powerless.", FALSE, ch, obj, 0, TO_CHAR);
      act("Nothing seems to happen.", FALSE, ch, obj, 0, TO_ROOM);
    } else {
      GET_OBJ_VAL(obj, 2)--;
      WAIT_STATE(ch, 2 * PULSE_VIOLENCE);
      for (tch = world[ch->in_room].people; tch; tch = next_tch) {
	next_tch = tch->next_in_room;
	if (ch == tch)
	  continue;
	if (GET_OBJ_VAL(obj, 0))
	  call_magic(ch, tch, NULL, GET_OBJ_VAL(obj, 3),
		     GET_OBJ_VAL(obj, 0), CAST_STAFF);
	else
	  call_magic(ch, tch, NULL, GET_OBJ_VAL(obj, 3),
		     DEFAULT_STAFF_LVL, CAST_STAFF);
      }
    }
    break;
  case ITEM_WAND:
    if (k == FIND_CHAR_ROOM) {
      if (tch == ch) {
	act("You point $p at yourself.", FALSE, ch, obj, 0, TO_CHAR);
	act("$n points $p at $mself.", FALSE, ch, obj, 0, TO_ROOM);
      } else {
	act("You point $p at $N.", FALSE, ch, obj, tch, TO_CHAR);
	  act("$n points $p at $N.", TRUE, ch, obj, tch, TO_ROOM);
      }
    } else if (tobj != NULL) {
      act("You point $p at $P.", FALSE, ch, obj, tobj, TO_CHAR);
	act("$n points $p at $P.", TRUE, ch, obj, tobj, TO_ROOM);
    } else {
      act("At what should $p be pointed?", FALSE, ch, obj, NULL, TO_CHAR);
      return;
    }

    if (GET_OBJ_VAL(obj, 2) <= 0) {
      act("It seems powerless.", FALSE, ch, obj, 0, TO_CHAR);
      act("Nothing seems to happen.", FALSE, ch, obj, 0, TO_ROOM);
      return;
    }
    GET_OBJ_VAL(obj, 2)--;
    WAIT_STATE(ch, 2 * PULSE_VIOLENCE);
    if (GET_OBJ_VAL(obj, 0))
      call_magic(ch, tch, tobj, GET_OBJ_VAL(obj, 3),
		 GET_OBJ_VAL(obj, 0), CAST_WAND);
    else
      call_magic(ch, tch, tobj, GET_OBJ_VAL(obj, 3),
		 DEFAULT_WAND_LVL, CAST_WAND);
    break;
  case ITEM_SCROLL:
    if (*arg) {
      if (!k) {
	act("There is nothing to here to affect with $p.", FALSE,
	    ch, obj, NULL, TO_CHAR);
	return;
      }
    } else
      tch = ch;

    level = GET_OBJ_VAL(obj, 0);
    spell1 = GET_OBJ_VAL(obj, 1);
    spell2 = GET_OBJ_VAL(obj, 2);
    spell3 = GET_OBJ_VAL(obj, 3);

    if (obj)
      extract_obj(obj);
    WAIT_STATE(ch, 2 * PULSE_VIOLENCE);

  if (!check_skill(ch, 101, SKILL_READ_MAGIC))
    {
      act("You recite $p incorrectly, which vanishes.", 0, ch, obj, 0, TO_CHAR);
      act("$n botches $s recitation of $p.", 0, ch, obj, NULL, TO_ROOM);
      return;
    }
    else
    {
      act("You recite $p, which dissolves.", TRUE, ch, obj, 0, TO_CHAR);
      act("$n recites $p.", FALSE, ch, obj, NULL, TO_ROOM);
    }

    if (call_magic(ch, tch, tobj, spell1, level, CAST_SCROLL))
      if (call_magic(ch, tch, tobj, spell2, level, CAST_SCROLL))
        call_magic(ch, tch, tobj, spell3, level, CAST_SCROLL);

    break;

  case ITEM_POTION:
    tch = ch;
      
      act("You quaff $p.", FALSE, ch, obj, NULL, TO_CHAR);
      act("$n quaffs $p.", TRUE, ch, obj, NULL, TO_ROOM);

    WAIT_STATE(ch, 2 * PULSE_VIOLENCE);
    for (i = 1; i < 4; i++)
      if (!(call_magic(ch, ch, NULL, GET_OBJ_VAL(obj, i),
		       GET_OBJ_VAL(obj, 0), CAST_POTION)))
	break;

    if (obj != NULL)
      extract_obj(obj);
    break;
  case ITEM_SALVE:
    if (*arg) {
      if (!k) {
        act("There is nothing to here to affect with $p.", FALSE,
            ch, obj, NULL, TO_CHAR);
        return;
      }
    }
    else
      tch = ch;

  if (tch == ch) {
    act("You use $p on yourself.", FALSE, ch, obj, tch, TO_CHAR);
    act("$n uses $p on $mself.", TRUE, ch, obj, tch, TO_NOTVICT);
   }
   
  else {
    act("You use $p on $N.", FALSE, ch, obj, tch, TO_CHAR);
    act("$n uses $p on you.", FALSE, ch, obj, tch, TO_VICT);
    act("$n uses $p on $N.", TRUE, ch, obj, tch, TO_NOTVICT);
  }

    WAIT_STATE(ch, 2 * PULSE_VIOLENCE);

    for (i = 1; i < 4; i++)
      if (!(call_magic(ch, tch, NULL, GET_OBJ_VAL(obj, i),
                       GET_OBJ_VAL(obj, 0), CAST_POTION)))
        break;

    if (obj != NULL)
      extract_obj(obj);
    break;

  default:
    log("SYSERR: Unknown object_type in mag_objectmagic");
    break;
  }
}

/*
 *
 *For scribing (Luni -10/03/04, a day before I turn 24!!)
 *
 */

char *garble_spell(int spellnum)
{
  char lbuf[256];
  int j, ofs = 0;

  *buf = '\0';
  strcpy(lbuf, spells[spellnum]);

  while (*(lbuf + ofs)) {
    for (j = 0; *(syls[j].org); j++) {
      if (!strncmp(syls[j].org, lbuf + ofs, strlen(syls[j].org))) {
      strcat(buf, syls[j].new);
      ofs += strlen(syls[j].org);
      }
    }
  }
  return buf;
}

/*
 *
 *For  brewing - pretty much like casting spells (Luni-8/03/04)
 *
 */

int make_potion(struct char_data *ch, int spellnum, struct obj_data *container)
{
  struct obj_data *final_potion;
  struct extra_descr_data *new_descr;
  int can_make = FALSE, dam;
  int slot, fail, percent;
  int num;

  if (spellnum < 0 || spellnum > TOP_SPELL_DEFINE) {
    sprintf(buf, "SYSERR: cast_spell trying to call spellnum %d\n", spellnum);
    log(buf);
    return 0;
  }

  if (!has_materials(ch, spellnum) && GET_LEVEL(ch) < LVL_IMMORT) {
  send_to_char("You dont seem to have the spell components necessary for this spell!\r\n", ch);
  return 0;
  }

  if (GET_POS(ch) < SINFO.min_position) {
    switch (GET_POS(ch)) {
      case POS_SLEEPING:
      send_to_char("You dream about great magical powers.\r\n", ch);
      break;
    case POS_RESTING:
      send_to_char("You cannot concentrate while resting.\r\n", ch);
      break;
    case POS_SITTING:
      send_to_char("You can't do this sitting!\r\n", ch);
      break;
    case POS_FIGHTING:
      send_to_char("Impossible!  You can't concentrate enough!\r\n", ch);
      break;
    default:
      send_to_char("You can't do much of anything like this!\r\n", ch);
      break;
    }
    return 0;
  }

num = number(0, 35);

switch (spellnum) {

case SPELL_ARMOR:
case SPELL_BLINDNESS:
case SPELL_BLUR:
case SPELL_THORNFLESH:
case SPELL_CURE_BLIND:
case SPELL_CURE_CRITIC:
case SPELL_CURE_LIGHT:
case SPELL_DETECT_INVIS:
case SPELL_DETECT_MAGIC:
case SPELL_INVISIBILITY:
case SPELL_HEAL:
case SPELL_POISON:
case SPELL_SLEEP:
case SPELL_STRENGTH:
case SPELL_SHIELD:
case SPELL_WORD_OF_RECALL:
case SPELL_SENSE_LIFE:
case SPELL_NEUTRALIZE_POISON:
case SPELL_INFRAVISION:
case SPELL_STONESKIN:
case SPELL_CURE_DEAFNESS:
case SPELL_STATUE:
case SPELL_FLIGHT:
case SPELL_WEAKNESS:
case SPELL_CURE_DISEASE:
case SPELL_REMOVE_PARALYSIS:
case SPELL_HASTE:
case SPELL_DISEASE:
case SPELL_PARALYZE:
case SPELL_SLOW:
case SPELL_WITHER:
case SPELL_BLADE_BARRIER:
case SPELL_SILENCE:
case SPELL_CURE_MODERATE:
case SPELL_CURE_SEVERE:
case SPELL_MAGIC_VEST:
case SPELL_FREE_ACTION:
case SPELL_THORNSHIELD:
case SPELL_DEHYDRATION:
case SPELL_REFRESH:
case SPELL_ENHANCE_AGILITY:
case SPELL_ENHANCE_ENDURANCE:
case SPELL_ENHANCE_INSIGHT:
case SPELL_ENHANCE_CUNNING:
case SPELL_ENHANCE_CHARISMA:
case SPELL_BLESSING_OF_KINGS:

        can_make = TRUE;
                break;
        default:
        can_make = FALSE;
                break;
        }

      if (can_make == FALSE) {
           send_to_char("That spell cannot be mixed into a potion.\r\n", ch);
           return 0;
        }


      else {

      slot = get_mem_slot(ch, spellnum);
      if ((slot < 0) || (slot > MAX_SPELL_MEM))
      {
        sprintf(buf, "SYSERR: get_mem_slot failed, spell %d", spellnum);
        log(buf);
        return 0;
      }

      WAIT_STATE(ch, SINFO.wait * PULSE_VIOLENCE);
      clear_spell(ch, slot);
      memorize_spell(ch, spellnum);

      if (affected_by_spell(ch, SPELL_DEAFENING_WIND) && number(0, 1))
      {
          send_to_char("Unable to hear your own voice, you botch the spell.\r\n", ch);
          return 0;
      }


percent = number(1, 101) - GET_WIS(ch);

if (use_skill(ch, percent, SKILL_BREW)) 
      {

 final_potion = create_obj();

        final_potion->item_number = NOTHING;
        final_potion->in_room = NOWHERE;
        sprintf(buf2, "%s %s %s potion", spells[spellnum], garble_spell(spellnum), potion_names[num]);
        final_potion->name = str_dup(buf2);


       sprintf(buf2, "A %s potion lies here.", potion_names[num]);
        final_potion->description = str_dup(buf2);

       sprintf(buf2, "A '%s' potion of %s lies here.",  spells[spellnum], potion_names[num]);
        final_potion->alt_description = str_dup(buf2);


        sprintf(buf2, "a %s potion", potion_names[num]);
        final_potion->short_description = str_dup(buf2);

        sprintf(buf2, "a %s potion of %s",  spells[spellnum], potion_names[num]);
        final_potion->altshort_description = str_dup(buf2);

        GET_OBJ_TIMER(final_potion) = 10; 

 CREATE(new_descr, struct extra_descr_data, 1);
       new_descr->keyword = str_dup(spells[spellnum]);
       new_descr->description = str_dup("It appears to be potion of inscrutable magical nature. Quaff at your own risk!");
       new_descr->next = NULL;
       final_potion->ex_description = new_descr;

        GET_OBJ_TYPE(final_potion) = ITEM_POTION;
        GET_OBJ_WEAR(final_potion) = ITEM_WEAR_TAKE;
        GET_OBJ_EXTRA(final_potion) = ITEM_NORENT;
        GET_OBJ_VAL(final_potion, 0) = GET_LEVEL(ch);
        GET_OBJ_VAL(final_potion, 1) = spellnum;
        GET_OBJ_VAL(final_potion, 2) = -1;
        GET_OBJ_VAL(final_potion, 3) = -1;
        GET_OBJ_COST(final_potion) = GET_LEVEL(ch) * 500;
        GET_OBJ_WEIGHT(final_potion) = 1;
        GET_OBJ_RENT(final_potion) = 0;
        GET_OBJ_CREATION(final_potion) = 1;

      if (GET_CLASS(ch) == CLASS_MAGE)
        GET_OBJ_NATURE(final_potion) = 1;

      else
         GET_OBJ_NATURE(final_potion) = 2;

        obj_to_char(final_potion, ch);

sprintf(buf, "You create a '%s' potion of %s.\r\n", spells[spellnum], potion_names[num]);
        send_to_char(buf, ch);
        act("$n creates $p!", FALSE, ch, final_potion, 0, TO_ROOM);
        extract_obj(container);

}
  else {

  fail = number(1,10);

   send_to_char("As you begin mixing the potion, it violently explodes!\r\n", ch);
     act("$n begins to mix a potion, and it suddenly explodes!", FALSE, ch, 0, 0, TO_ROOM);
       extract_obj(container);
       dam = number(5, 50);
       GET_HIT(ch) -= dam;
       update_pos(ch);

      if (fail <= 5) {
       if (SINFO.violent || IS_SET(SINFO.routines, MAG_DAMAGE)) {
       return (call_magic(ch, ch, NULL, spellnum, GET_LEVEL(ch), CAST_SPELL));
     }
   }
}
}
    return 0;
}

int make_scroll(struct char_data *ch, int spellnum, struct obj_data *paper)
{
    struct obj_data *final_scroll;
    struct extra_descr_data *new_descr;
    int can_make = TRUE, dam = 0;
    int slot, fail, percent;

  if (spellnum < 0 || spellnum > TOP_SPELL_DEFINE) {
    sprintf(buf, "SYSERR: cast_spell trying to call spellnum %d\n", spellnum);
    log(buf);
    return 0;
  }

   if (!has_materials(ch, spellnum) && GET_LEVEL(ch) < LVL_IMMORT) {
 send_to_char("You dont seem to have the spell components necessary for this spell!\r\n", ch);

  return 0;
  }


  if (GET_POS(ch) < SINFO.min_position) {
    switch (GET_POS(ch)) {
      case POS_SLEEPING:
      send_to_char("You dream about great magical powers.\r\n", ch);
      break;
    case POS_RESTING:
      send_to_char("You cannot concentrate while resting.\r\n", ch);
      break;
    case POS_SITTING:
      send_to_char("You can't do this sitting!\r\n", ch);
      break;
    case POS_FIGHTING:
      send_to_char("Impossible!  You can't concentrate enough!\r\n", ch);
      break;
    default:
      send_to_char("You can't do much of anything like this!\r\n", ch);
      break;
    }
    return 0;
  }


    switch (spellnum) 
    {
        case SPELL_ARMOR:
        case SPELL_CHILL_TOUCH:
        case SPELL_DETECT_MAGIC:
        case SPELL_MAGIC_MISSILE:
        case SPELL_SHIELD:
        case SPELL_LIGHT:
        case SPELL_BURNING_HANDS:
        case SPELL_BLUR:
        case SPELL_DETECT_INVIS:
        case SPELL_INFRAVISION:
        case SPELL_MIRROR_IMAGE:
        case SPELL_WEB:
        case SPELL_BLINDNESS:
        case SPELL_INVISIBILITY:
        case SPELL_LOCATE_OBJECT:
        case SPELL_SHOCKING_GRASP:
        case SPELL_STRENGTH:
        case SPELL_WEAKNESS:
        case SPELL_DISPEL_MAGIC:
        case SPELL_GUST_OF_WIND:
        case SPELL_POLYMORPH:
        case SPELL_ENCHANT_WEAPON:
        case SPELL_LIGHTNING_BOLT:
        case SPELL_FLIGHT:
        case SPELL_HASTE:
        case SPELL_SILENCE:
        case SPELL_WATER_BREATHING:
        case SPELL_IDENTIFY:
        case SPELL_MINOR_GLOBE:
        case SPELL_VAMPIRIC_TOUCH:
        case SPELL_CHARM:
        case SPELL_FIREBALL:
        case SPELL_SLEEP:
        case SPELL_STONESKIN:
        case SPELL_SLOW:
        case SPELL_DANCING_SWORD:
        case SPELL_ENHANCE_AGILITY:
        case SPELL_ENHANCE_ENDURANCE:
        case SPELL_ENHANCE_INSIGHT:
        case SPELL_ENHANCE_CUNNING:
        case SPELL_ENHANCE_CHARISMA:
        case SPELL_BLESSING_OF_KINGS:
            can_make = TRUE;
            break;

        default:
            can_make = FALSE;
            break;
    }
    
    if (can_make == FALSE) 
    {
        send_to_char("That spell cannot be scribed into a scroll.\r\n", ch);
        return 0;
    }
    else 
    {
      slot = get_mem_slot(ch, spellnum);
      if ((slot < 0) || (slot > MAX_SPELL_MEM))
      {
        sprintf(buf, "SYSERR: get_mem_slot failed, spell %d", spellnum);
        log(buf);
        return 0;
      }

      WAIT_STATE(ch, SINFO.wait * PULSE_VIOLENCE);
      clear_spell(ch, slot);
      memorize_spell(ch, spellnum);

      if (affected_by_spell(ch, SPELL_DEAFENING_WIND) && number(0, 1))
      {
        send_to_char("Unable to hear your own voice, you botch the spell.\r\n", ch);
        return 0;
      }


    percent = number(1, 101) - GET_WIS(ch);

    if (use_skill(ch, percent, SKILL_SCRIBE)) 
    {
        final_scroll = create_obj();
        final_scroll->item_number = NOTHING;
        final_scroll->in_room = NOWHERE;
        sprintf(buf2, "%s %s scroll", spells[spellnum], garble_spell(spellnum));
        final_scroll->name = str_dup(buf2);

        sprintf(buf2, "Some scroll inscribed with the runes '%s' lies here.", garble_spell(spellnum));
        final_scroll->description = str_dup(buf2);

        sprintf(buf2, "A '%s' scroll lies here.",  spells[spellnum]);
        final_scroll->alt_description = str_dup(buf2);

        sprintf(buf2, "a %s scroll", garble_spell(spellnum));
        final_scroll->short_description = str_dup(buf2);

        sprintf(buf2, "a %s scroll",  spells[spellnum]);
        final_scroll->altshort_description = str_dup(buf2);

        GET_OBJ_TIMER(final_scroll) = 10;

        CREATE(new_descr, struct extra_descr_data, 1);
        new_descr->keyword = str_dup(spells[spellnum]);
        new_descr->description = str_dup("It appears to be a scroll inscribed with mysterious runes. Use at your own risk!");
        new_descr->next = NULL;
        final_scroll->ex_description = new_descr;

        GET_OBJ_TYPE(final_scroll) = ITEM_SCROLL;
        GET_OBJ_WEAR(final_scroll) = ITEM_WEAR_TAKE;
        GET_OBJ_EXTRA(final_scroll) = ITEM_NORENT;
        GET_OBJ_CREATION(final_scroll) = 1;
        GET_OBJ_VAL(final_scroll, 0) = GET_LEVEL(ch);
        GET_OBJ_VAL(final_scroll, 1) = spellnum;
        GET_OBJ_VAL(final_scroll, 2) = -1;
        GET_OBJ_VAL(final_scroll, 3) = -1;
        GET_OBJ_COST(final_scroll) = GET_LEVEL(ch) * 500;
        GET_OBJ_WEIGHT(final_scroll) = 1;
        GET_OBJ_RENT(final_scroll) = 0;

        if (GET_CLASS(ch) == CLASS_MAGE) 
            GET_OBJ_NATURE(final_scroll) = 1;
        else
            GET_OBJ_NATURE(final_scroll) = 2;

        obj_to_char(final_scroll, ch);

        sprintf(buf, "You create a scroll of %s.\r\n", spells[spellnum]);
        send_to_char(buf, ch);
        act("$n creates $p!", FALSE, ch, final_scroll, 0, TO_ROOM);
        extract_obj(paper);
    }
    else 
    {
        fail = number(1,10);

        send_to_char("As you begin inscribing the final rune, the scroll violently explodes!\r\n",ch);
        act("$n tries to scribe a spell, but it explodes!", FALSE, ch, 0,0, TO_ROOM);
        extract_obj(paper);
        dam = number(5, 50);
        GET_HIT(ch) -= dam;
        update_pos(ch);

        if (fail <= 5) 
        {
            if (SINFO.violent || IS_SET(SINFO.routines, MAG_DAMAGE)) 
            {
                return (call_magic(ch, ch, NULL, spellnum, GET_LEVEL(ch), CAST_SPELL));
            }
        }

    }
}
    return 0;
}

/*
 * cast_spell is used generically to cast any spoken spell, assuming we
 * already have the target char/obj and spell number.  It checks all
 * restrictions, etc., prints the words, etc.
 *
 * Entry point for NPC casts.  Recommended entry point for spells cast
 * by NPCs via specprocs.
 */

int cast_spell(struct char_data * ch, struct char_data * tch, struct obj_data * tobj, int spellnum)
{
  char buf[256];
  int slot;

  if (spellnum < 0 || spellnum > TOP_SPELL_DEFINE) {
    sprintf(buf, "SYSERR: cast_spell trying to call spellnum %d\n", spellnum);
    log(buf);
    return 0;
  }
    
  if (GET_POS(ch) < SINFO.min_position) {
    switch (GET_POS(ch)) {
      case POS_SLEEPING:
      send_to_char("You dream about great magical powers.\r\n", ch);
      break;
    case POS_RESTING:
      send_to_char("You cannot concentrate while resting.\r\n", ch);
      break;
    case POS_SITTING:
      send_to_char("You can't do this sitting!\r\n", ch);
      break;
    case POS_FIGHTING:
      send_to_char("Impossible!  You can't concentrate enough!\r\n", ch);
      break;
    default:
      send_to_char("You can't do much of anything like this!\r\n", ch);
      break;
    }
    return 0;
  }

  if (IS_AFFECTED(ch, AFF_CHARM) && (ch->master == tch)) {
    send_to_char("You are afraid you might hurt your master!\r\n", ch);
    return 0;
  }

  if ((tch != ch) && IS_SET(SINFO.targets, TAR_SELF_ONLY)) {
    send_to_char("You can only cast this spell upon yourself!\r\n", ch);
    return 0;
  }

  if ((tch == ch) && IS_SET(SINFO.targets, TAR_NOT_SELF)) {
    send_to_char("You cannot cast this spell upon yourself!\r\n", ch);
    return 0;
  }

  if (IS_SET(SINFO.routines, MAG_GROUPS) && !IS_AFFECTED(ch, AFF_GROUP)) {
    send_to_char("You can't cast this spell if you're not in a group!\r\n",ch);
    return 0;
  }

  if (affected_by_spell(ch, SPELL_SILENCE) ||
      affected_by_spell(ch, SPELL_SPHERE_SILENCE))
  {
    send_to_char("You open your mouth to speak the words, but nothing comes out.\r\n", ch);
    return 0;
  } else if (affected_by_spell(ch, SPELL_ANTIMAGIC_AURA))
  {
    send_to_char("The aura surrounding you seems to prevent all spellcasting.\r\n", ch);
    return 0;
  } else if (tch && affected_by_spell(tch, SPELL_STATUE))
  {
    send_to_char("You can't cast spells on statues.\r\n", ch);
    return 0;
  } else
  {
    send_to_char(OK, ch);
    say_spell(ch, spellnum, tch, tobj);
/*  We're going to stick all the new pc casting stuff here - purging components,
    checking the mem slot, doing the wait_state, et al.  It makes more sense to
    remove the spell from memory and purge the components if the words to the
    spell were said - not just if call_magic returns 1.  Soli, 9/1/99   */
    if (!IS_NPC(ch) && (GET_LEVEL(ch) < LVL_IMMORT))
    {
      if (!consume_components(ch, spellnum))
      {
        list_components(ch, spellnum);
        sprintf(buf, "SYSERR:  destroy_components failed, spell %d", spellnum);
        log(buf);
        return 0;
      }

      slot = get_mem_slot(ch, spellnum);
      if ((slot < 0) || (slot > MAX_SPELL_MEM))
      {
        sprintf(buf, "SYSERR: get_mem_slot failed, spell %d", spellnum);
        log(buf);
        return 0;
      }

      WAIT_STATE(ch, SINFO.wait * PULSE_VIOLENCE);
      clear_spell(ch, slot);
      memorize_spell(ch, spellnum);

      if (affected_by_spell(ch, SPELL_DEAFENING_WIND) && number(0, 1))
      {
          send_to_char("Unable to hear your own voice, you botch the spell.\r\n", ch);
          return 0;
      }
    } // end of mortal PC stuff
  } // successfully cast spell

  return (call_magic(ch, tch, tobj, spellnum, GET_LEVEL(ch), CAST_SPELL));
}

struct obj_data * get_obj_list_vnum(struct char_data * ch, struct obj_data *obj, int vnum)
{
  if (!obj)
    return 0;

  for (obj = obj; obj; obj = obj->next_content)
    if (CAN_SEE_OBJ(ch, obj) && (GET_OBJ_VNUM(obj) == vnum))
      return obj;

  return 0;
}

void list_components(struct char_data * ch, int spellnum)
{
/*
  int i, j, count = 0;
  long flags;
  struct obj_data * tmp_obj;

  for (i = 0; spell_components[i].spellnum; i++)
  {
    if (spell_components[i].spellnum == spellnum)
    {
      if (count == 0)
        send_to_char("You need the following to cast this spell:", ch);
      else
        send_to_char("Or you can use the following instead:", ch);

      count++;

      for (j = 0; j <= 1; j++)
      {
        if (spell_components[i].comps[j].vnum == CMP_UNDEFINED)
          continue;

        if ((tmp_obj = get_obj_world_vnum(spell_components[i].comps[j].vnum)))
        {
          sprintf(buf, "\r\n%s", OBJS(tmp_obj, ch));
          send_to_char(buf, ch);
        }
        else
          send_to_char("\r\nsomething", ch);

        flags = spell_components[i].comps[j].flags;

        if ((flags == 0) || (flags - CMP_INV == 0))
          continue;

        send_to_char(":", ch);

        if (IS_SET(flags, CMP_HOLD))
          send_to_char("  Must be held", ch);
        else if (IS_SET(flags, CMP_EQUIP))
          send_to_char("  Must be equipped", ch);
        else if (IS_SET(flags, CMP_GROUND))
          send_to_char("  Must be in room", ch);

        if (IS_SET(flags, CMP_CONSUMED))
          send_to_char("   Consumed when cast", ch);
      }
    }
  }

  send_to_char("\r\n", ch);
*/
  return;
}

/*
int consume_components(struct char_data * ch, int spellnum)
{

  int i, j, k, vnum, pass = 1, exists = 0, comps = 0;
  long flags;
  struct obj_data * obj[1];

  if (IS_NPC(ch) || (GET_LEVEL(ch) >= LVL_IMMORT))
    return 1;

  for (i = 0; spell_components[i].spellnum; i++)
  {
    pass = 1;
    if (spell_components[i].spellnum == spellnum)
    {
      exists++;
      for (j = 0; j <= 1; j++)
      {
        vnum = spell_components[i].comps[j].vnum;
        flags = spell_components[i].comps[j].flags;
        if (vnum == CMP_UNDEFINED)
          continue;

        if (IS_SET(flags, CMP_INV))
        {
          if (!(obj[j] = get_obj_list_vnum(ch, ch->carrying, vnum)))
          {
            pass = 0;
            continue;
          }
        }
        else if (IS_SET(flags, CMP_HOLD))
        {
          if (!GET_EQ(ch, WEAR_HOLD) ||
             (GET_OBJ_VNUM(GET_EQ(ch, WEAR_HOLD)) != vnum))
          {
            pass = 0;
            continue;
          }
          obj[j] = GET_EQ(ch, WEAR_HOLD);
        }
        else if (IS_SET(flags, CMP_EQUIP))
        {
          for (k = 0; k < NUM_WEARS; k++)
          {
            if (GET_EQ(ch, k) && (GET_OBJ_VNUM(GET_EQ(ch, k)) == vnum))
              obj[j] = GET_EQ(ch, k);
          }
          if (obj[j] == NULL)
          {
            pass = 0;
            continue;
          }
        }
        else if (IS_SET(flags, CMP_GROUND))
        {
          if (!(obj[j] = get_obj_list_vnum(ch, world[ch->in_room].contents,
                vnum)))
          {
            pass = 0;
            continue;
          }
        }
        else
        {
          sprintf(buf, "SYSERR: spell %d components, line %d comp %d",
                    spellnum, i, j);
          return 1;
        }
      } // for j = 1 to 2

      if (pass && (spell_components[i].spellnum == spellnum))
      {
        comps = 1;
        break;
      }
    }  // if the component at i is the right spell

    if (comps)
      break;   // out of the i loop

    obj[0] = obj[1] = NULL;
  }   // for i = 1 to spellnum

  if (!exists)
    return 1;

  if (!comps)
    return 0;

  flags = spell_components[i].comps[0].flags;
  if (IS_SET(flags, CMP_CONSUMED) && obj[0])
  {
    if (IS_SET(flags, CMP_GROUND))
      act("$p fades from existence.", FALSE, ch, obj[0], 0, TO_ROOM);
    act("$p is consumed as you cast the spell.", FALSE, ch, obj[0], 0, TO_CHAR);
    extract_obj(obj[0]);
  }

  flags = spell_components[i].comps[1].flags;
  if (IS_SET(flags, CMP_CONSUMED) && obj[1])
  {
    if (IS_SET(flags, CMP_GROUND))
      act("$p fades from existence.", FALSE, ch, obj[1], 0, TO_ROOM);
    act("$p is consumed as you cast the spell.", FALSE, ch, obj[1], 0, TO_CHAR);
    extract_obj(obj[1]);
  }

  return 1;
}
*/

int consume_components(struct char_data * ch, int spellnum)
{
   /* return 1 for now. easier than ripping things out.
    * --gan
  int i, j, k, vnum, pass = 1, exists = 0, comps = 0, compnumber = 0;
  long flags;
  struct obj_data * obj[1];

  if (IS_NPC(ch) || (GET_LEVEL(ch) >= LVL_IMMORT))
    return 1;

  for (i = 0; spell_components[i].spellnum; i++)
  {
    pass = 1;
    if (spell_components[i].spellnum == spellnum)
    {
      exists++;
      for (j = 0; j <= 1; j++)
      {
        vnum = spell_components[i].comps[j].vnum;
        flags = spell_components[i].comps[j].flags;
        if (vnum == CMP_UNDEFINED)
          continue;

        if (IS_SET(flags, CMP_INV))
        {
          if (!get_obj_list_vnum(ch, ch->carrying, vnum))
          {
            pass = 0;
            continue;
          }
        }
        else if (IS_SET(flags, CMP_HOLD))
        {
          if (!GET_EQ(ch, WEAR_HOLD) ||
             (GET_OBJ_VNUM(GET_EQ(ch, WEAR_HOLD)) != vnum))
          {
            pass = 0;
            continue;
          }
          continue;
        }
        else if (IS_SET(flags, CMP_EQUIP))
        {
          for (k = 0; k < NUM_WEARS; k++)
          {
            if (GET_EQ(ch, k) && (GET_OBJ_VNUM(GET_EQ(ch, k)) == vnum))
            {
              pass = 2;
              break;
            }
          }
          pass--;
          continue;
        }
        else if (IS_SET(flags, CMP_GROUND))
        {
          if (!get_obj_list_vnum(ch, world[ch->in_room].contents, vnum))
          {
            pass = 0;
            continue;
          }
        }
        else
        {
          sprintf(buf, "SYSERR: spell %d components, line %d comp %d",
                    spellnum, i, j);
          log(buf);
          return 1;
        }
        if (!pass)
          break;
      } // for j = 1 to 2

      if (pass && (spell_components[i].spellnum == spellnum))
      {
        comps = 1;
        compnumber = i;
        break;
      }
    }  // if the component at i is the right spell

    if (comps)
      break;   // out of the i loop

  }   // for i = 1 to spellnum

  if (!exists)
    return 1;
  if (!comps)
    return 0;

  for (j = 0; j <= 1; j++)
  {
    flags = spell_components[compnumber].comps[j].flags;
    vnum = spell_components[compnumber].comps[j].vnum;
    if (vnum == CMP_UNDEFINED)
      continue;

    if (IS_SET(flags, CMP_CONSUMED))
    {
      if (IS_SET(flags, CMP_INV))
        obj[j] = get_obj_list_vnum(ch, ch->carrying, vnum);
      else if (IS_SET(flags, CMP_HOLD))
        obj[j] = GET_EQ(ch, WEAR_HOLD);
      else if (IS_SET(flags, CMP_EQUIP))
      {
        for (k = 0; k < NUM_WEARS; k++)
        {
          if (GET_EQ(ch, k) && (GET_OBJ_VNUM(GET_EQ(ch, k)) == vnum))
            obj[j] = GET_EQ(ch, k);
        }
      }
      else if (IS_SET(flags, CMP_GROUND))
        obj[j] = get_obj_list_vnum(ch, world[ch->in_room].contents, vnum);

      if (IS_SET(flags, CMP_GROUND))
        act("$p fades from existance.", FALSE, ch, obj[j], 0, TO_ROOM);
      act("$p is consumed as you cast the spell.", FALSE, ch, obj[j], 0, TO_CHAR);
      
      extract_obj(obj[j]);
    }
  }
   */
  return 1;
}

int check_components(struct char_data * ch, int spellnum)
{
/*
  int i, j, k, vnum, pass = 1, exists = 0, comps = 0;
  long flags;

  if (IS_NPC(ch) || (GET_LEVEL(ch) >= LVL_IMMORT))
    return 1;

  for (i = 0; spell_components[i].spellnum; i++)
  {
    pass = 1;
    if (spell_components[i].spellnum == spellnum)
    {
      exists++;
      for (j = 0; j <= 1; j++)
      {
        vnum = spell_components[i].comps[j].vnum;
        flags = spell_components[i].comps[j].flags;
        if (vnum == CMP_UNDEFINED)
          continue;

        if (IS_SET(flags, CMP_INV))
        {
          if (!get_obj_list_vnum(ch, ch->carrying, vnum))
          {
            pass = 0;
            continue;
          }
        }
        else if (IS_SET(flags, CMP_HOLD))
        {
          if (!GET_EQ(ch, WEAR_HOLD) ||
             (GET_OBJ_VNUM(GET_EQ(ch, WEAR_HOLD)) != vnum))
          {
            pass = 0;
            continue;
          }
          continue;
        }
        else if (IS_SET(flags, CMP_EQUIP))
        {
          for (k = 0; k < NUM_WEARS; k++)
          {
            if (GET_EQ(ch, k) && (GET_OBJ_VNUM(GET_EQ(ch, k)) == vnum))
            {
              pass = 2;
              break;
            }
          }
          pass--;
          continue;
        }
        else if (IS_SET(flags, CMP_GROUND))
        {
          if (!get_obj_list_vnum(ch, world[ch->in_room].contents, vnum))
          {
             pass = 0;
            continue;
          }
        }
        else
        {
          sprintf(buf, "SYSERR: spell %d components, line %d comp %d",
                    spellnum, i, j);
          log(buf);
          return 1;
        }

        if (!pass)
          break;
      } // for j = 1 to 2

      if (pass && (spell_components[i].spellnum == spellnum))
      {
        comps = 1;
        break;
      }
    }  // if the component at i is the right spell

    if (comps)
      break;   // out of the i loop

  }   // for i = 1 to spellnum

  if (!exists)
    return 1;
  if (!comps)
    return 0;
*/
  return 1;
}

int get_element(struct char_data * ch, int type)
{
  extern struct element_type elements[];
  int i;
  struct area_aff_data *af;

  if (type == SPELL_ELEMENTAL_STORM)
  {
    for (af = area_spells_list; af; af = af->next)
    {
      if (af->type == SPELL_ELEMENTAL_STORM)
        switch (af->other)
        {
          case 1:
            return ELEMENT_FIRE;
          case 2:
            return ELEMENT_WATER;
          case 3:
            return ELEMENT_AIR;
          case 4:
            return ELEMENT_EARTH;
        }
    }

    return 0;
  }

  for (i = 0; elements[i].type; i++)
  {
    if (elements[i].type == type)
      return elements[i].element;
  }

  return 0;
}

int protect_from_element(struct char_data * ch, int element)
{
  if ((affected_by_spell(ch, SPELL_PROT_FROM_FIRE) &&
                        (element == ELEMENT_FIRE)) ||
      (affected_by_spell(ch, SPELL_PROT_FROM_FROST) &&
                        (element == ELEMENT_COLD)))
    return 1;

  if (affected_by_spell(ch, SPELL_PROT_FROM_ELEMENTS) &&
      ((element == ELEMENT_FIRE) || (element == ELEMENT_COLD) ||
       (element == ELEMENT_ACID) || (element == ELEMENT_LIGHTNING) ||
       (element == ELEMENT_WATER) || (element == ELEMENT_EARTH) ||
       (element == ELEMENT_AIR)))
    return 1;

  return 0;
}

ACMD(do_scribe)
{
    struct obj_data *paper = NULL;
    struct obj_data *obj, *next_obj;
    char paper_name[MAX_STRING_LENGTH];
    char spell_name[MAX_STRING_LENGTH];
    int spellnum, found = FALSE;

    two_arguments(argument, paper_name, spell_name);

    if (GET_POS(ch) == POS_FISHING || GET_POS(ch) == POS_DIGGING || GET_POS(ch) == POS_RIDING) 
    {
        send_to_char("You are not in a proper position for that!\r\n", ch);
        return ;
    }

    /* sanity check */
    /*if (temp1) {
        temp2 = get_spell_name(temp1);
        if (temp2)
    strcpy(spell_name, temp2);
    } else {
        paper_name[0] = '\0';
        spell_name[0] = '\0';
    }*/

    if (!GET_SKILL(ch, SKILL_SCRIBE))
    {
        act("You are unfamiliar with this skill!", FALSE, ch, 0, 0, TO_CHAR);
        return;
    }

    if (IS_WARRIOR(ch) || IS_KNIGHT(ch) || IS_ROGUE(ch) || IS_MONK(ch)) 
    {
        act("Your class is not granted this skill.", FALSE, ch, 0, 0, TO_CHAR);
        return;
    }

    if (!*paper_name || !*spell_name) 
    {
        send_to_char("What do you wish to scribe where?\r\n", ch);
        return;
    }

    for (obj = ch->carrying; obj; obj = next_obj) 
    {
        next_obj = obj->next_content;

        if (obj == NULL)
            return;
        else if (!(paper = get_obj_in_list_vis(ch, paper_name, ch->carrying))) 
            continue;
        else
            found = TRUE;
    }

    if (found && (GET_OBJ_TYPE(paper) != ITEM_NOTE)) 
    {
        send_to_char("You can't write on that!\r\n", ch);
        return;
    }

    if (found == FALSE) 
    {
        sprintf(buf, "You don't have %s in your inventory!\r\n", paper_name);
        send_to_char(buf, ch);
        return;
    }

    if (!spell_name || !*spell_name) 
    {
        send_to_char("What do you want to scribe and where?\r\n", ch);
        return;
    } 

    spellnum = find_dash_skill_num(spell_name);

    if ((spellnum < 1) || (spellnum > MAX_SPELLS)) 
    {
        send_to_char("Scribe what spell?!?\r\n", ch);
        return;
    }

    if (GET_SKILL(ch, spellnum) == 0)
    {
        send_to_char("You are unfamiliar with that spell.\r\n", ch);
        return;
    }

    if (!spell_access(ch, spellnum))
    {
        send_to_char("You do not know that spell.\r\n", ch);
        return;
    }

    if ((GET_LEVEL(ch) < LVL_IMMORT) && (is_memmed(ch, spellnum) < 1))
    {
        if (is_memmed(ch, spellnum) == 0)
        send_to_char("But you don't have that spell memorized.\r\n", ch);
        else
        send_to_char("You don't have that spell memorized yet.\r\n", ch);
        return;
    }

    make_scroll(ch, spellnum, paper);
}


ACMD(do_brew)
{
  char bottle_name[MAX_STRING_LENGTH];
  char spell_name[MAX_STRING_LENGTH];
  struct obj_data *container = NULL;
  struct obj_data *obj, *next_obj;

  int spellnum, found = FALSE;

/*  temp1 = one_argument(argument, bottle_name);

 if (GET_POS(ch) == POS_FISHING || GET_POS(ch) == POS_DIGGING || GET_POS(ch) == POS_RIDING) {
     send_to_char("You are not in a proper position for that!\r\n", ch);
    return ;
}
if (temp1) {
    temp2 = get_spell_name(temp1);
    if (temp2)
strcpy(spell_name, temp2);
} else {
    bottle_name[0] = '\0';
    spell_name[0] = '\0';
} */

two_arguments(argument, bottle_name, spell_name); 

 if (!GET_SKILL(ch, SKILL_BREW))
  {
    act("You are unfamiliar with this skill!", FALSE, ch, 0, 0, TO_CHAR);
    return;
  }

 if (IS_WARRIOR(ch) || IS_KNIGHT(ch) || IS_ROGUE(ch) || IS_MONK(ch)) {
    act("Your class is not granted this skill.", FALSE, ch, 0, 0, TO_CHAR);
    return;
  }


    if (!*bottle_name || !*spell_name) {
                send_to_char("What do you wish to mix in where?\r\n", ch);
                return;
   }

 for (obj = ch->carrying; obj; obj = next_obj) {
                next_obj = obj->next_content;
                if (obj == NULL)
                        return ;
    else if (!(container = get_obj_in_list_vis(ch, bottle_name, ch->carrying)))
                        continue;
                else
                        found = TRUE;
        }
        if (found != FALSE && (GET_OBJ_TYPE(container) != ITEM_BOTTLE)) {
                send_to_char("That item is not a suitable for this purpose!!\r\n", ch);
                return;
        }
        if (found == FALSE) {
         sprintf(buf, "You don't have %s in your inventory!\r\n", bottle_name);
         send_to_char(buf, ch);
         return;
        }

  spellnum = find_dash_skill_num(spell_name);

  if ((spellnum < 1) || (spellnum > MAX_SPELLS)) {
    send_to_char("Mix what spell?!?\r\n", ch);
    return;
  }

  if (GET_SKILL(ch, spellnum) == 0)
  {
    send_to_char("You are unfamiliar with that spell.\r\n", ch);
    return;
  }

  if (!spell_access(ch, spellnum))
  {
    send_to_char("You do not know that spell.\r\n", ch);
    return;
  }

  if ((GET_LEVEL(ch) < LVL_IMMORT) && (is_memmed(ch, spellnum) < 1))
  {
    if (is_memmed(ch, spellnum) == 0)
      send_to_char("But you don't have that spell memorized.\r\n", ch);
    else
      send_to_char("You don't have that spell memorized yet.\r\n", ch);
    return;
  }


    make_potion(ch, spellnum, container);
}

ACMD(do_cast)
{
  struct char_data *tch = NULL;
  struct obj_data *tobj = NULL;
  char s[255], t[255];
  char *restarg;
  int i, target = 0, spellnum;

  if (IS_NPC(ch))
    return;

 if (GET_POS(ch) == POS_FISHING || GET_POS(ch) == POS_DIGGING) {
     send_to_char("You are not in a proper position for that!\r\n", ch);
    return ;
}
/*
  s = strtok(argument, "'");

  if (s == NULL) {
    send_to_char("Cast what where?\r\n", ch);
    return;
  }

  s = strtok(NULL, "'");

  if (s == NULL) {
    send_to_char("Spell names must be enclosed in the Holy Magic Symbols: '\r\n"
, ch);
    return;
  }

  t = strtok(NULL, "\0");

  spellnum = find_skill_num(s);
*/

  restarg = any_one_arg(argument, s);

  if ((s == NULL) || !strcmp(s, ""))
  {
    send_to_char("Cast what where?\r\n", ch);
    return;
  }

  spellnum = find_dash_skill_num(s);

  if ((spellnum < 1) || (spellnum > MAX_SPELLS)) {
    send_to_char("Cast what?!?\r\n", ch);
    return;
  }

  if (PLR_FLAGGED(ch, PLR_RABBIT) ||
     PLR_FLAGGED(ch, PLR_BIRD) ||
     PLR_FLAGGED(ch, PLR_WOLF) ||
     PLR_FLAGGED(ch, PLR_BEAR) ||
     PLR_FLAGGED(ch, PLR_CAT)){
     send_to_char("You feel too beastly to cast spells.\r\n", ch);
     return;
  }

  if (GET_SKILL(ch, spellnum) == 0)
  {
    send_to_char("You are unfamiliar with that spell.\r\n", ch);
    return;
  }

  if (!spell_access(ch, spellnum))
  {
    send_to_char("You do not know that spell.\r\n", ch);
    return;
  }

  if ((GET_LEVEL(ch) < LVL_IMMORT) && (is_memmed(ch, spellnum) < 1))
  {
    if (is_memmed(ch, spellnum) == 0)
      send_to_char("But you don't have that spell memorized.\r\n", ch);
    else
      send_to_char("You don't have that spell memorized yet.\r\n", ch);
    return;
  }

/*  if (!check_components(ch, spellnum))
  {
    list_components(ch, spellnum);
    return;
  } */

  if (restarg != NULL) {
    two_arguments(strcpy(arg, restarg), t, s);
  }

  if (IS_SET(SINFO.targets, TAR_IGNORE)) {
    target = TRUE;
  } else if (IS_SET(SINFO.targets, TAR_OBJ_AND_WLD))
  {
    if ((t != NULL) && (s != NULL) && *t && *s)
    {
      if ((tobj = get_obj_in_list_vis(ch, t, ch->carrying)))
        if ((tch = get_char_vis(ch, s, FIND_CHAR_WORLD)))
          target = TRUE;
    }
  } else if (t != NULL && *t) {
    if (!target && (IS_SET(SINFO.targets, TAR_CHAR_ROOM))) {
      if ((tch = get_char_vis(ch, t, FIND_CHAR_ROOM)) != NULL)
         target = TRUE;
    }
    if (!target && IS_SET(SINFO.targets, TAR_CHAR_WORLD))
      if ((tch = get_char_vis(ch, t, FIND_CHAR_WORLD)))
        target = TRUE;

    if (!target && IS_SET(SINFO.targets, TAR_OBJ_INV))
      if ((tobj = get_obj_in_list_vis(ch, t, ch->carrying)))
        target = TRUE;

    if (!target && IS_SET(SINFO.targets, TAR_OBJ_EQUIP)) {
      for (i = 0; !target && i < NUM_WEARS; i++)
        if (GET_EQ(ch, i) && isname(t, GET_EQ(ch, i)->name)) {
          tobj = GET_EQ(ch, i);
          target = TRUE;
        }
    }
    if (!target && IS_SET(SINFO.targets, TAR_OBJ_ROOM))
      if ((tobj = get_obj_in_list_vis(ch, t, world[ch->in_room].contents)))
              target = TRUE;
  
      if (!target && IS_SET(SINFO.targets, TAR_OBJ_WORLD))
      if ((tobj = get_obj_vis(ch, t)))
        target = TRUE;
  
    } else {                    
    if (!target && IS_SET(SINFO.targets, TAR_FIGHT_SELF))
      if (FIGHTING(ch) != NULL) {
        tch = ch;
        target = TRUE;
      }
    if (!target && IS_SET(SINFO.targets, TAR_FIGHT_VICT))
      if (FIGHTING(ch) != NULL) {
        tch = FIGHTING(ch);
        target = TRUE;
      }
    if (!target && IS_SET(SINFO.targets, TAR_CHAR_ROOM) &&
        !SINFO.violent) {
      tch = ch;
      target = TRUE;
    }
    if (!target) {
      sprintf(buf, "Upon %s should the spell be cast?\r\n",
         IS_SET(SINFO.targets, TAR_OBJ_ROOM | TAR_OBJ_INV | TAR_OBJ_WORLD) ?
              "what" : "who");
       send_to_char(buf, ch);
      return;
    }
  }

/*  if (target && (tch == ch) && SINFO.violent) {
    send_to_char("You shouldn't cast that on yourself -- could be bad for your health!\r\n", ch);
    return;
  } */

  if (!target) {
    send_to_char("Cannot find the target of your spell!\r\n", ch);
    return;
  }

  /*
  if (!has_materials(ch, spellnum) && GET_LEVEL(ch) < LVL_IMMORT) {
    send_to_char("You dont seem to have the spell components necessary for this spell!\r\n", ch);
    return;
  }
  */

  check_improve(ch, spellnum);
  cast_spell(ch, tch, tobj, spellnum);
}

void spell_level_race(int spell, int race, int level, int sphere)
{
  char buf[256];
  int bad = 0;

  if (spell < 0 || spell > TOP_SPELL_DEFINE) {
    sprintf(buf, "SYSERR: attempting assign to illegal spellnum %d", spell);
    log(buf);
    return;
  }

  if (race < 0 || race >= NUM_RACES) {
    sprintf(buf, "SYSERR: assigning '%s' to illegal race %d",
            skill_name(spell), race);
    log(buf);
    bad = 1;
  }

  if (level < 1 || level > LVL_IMPL) {
    sprintf(buf, "SYSERR: assigning '%s' to illegal level %d",
            skill_name(spell), level);
    log(buf);
    bad = 1;
  }

  if ( sphere < 0 ) {
    sprintf(buf, "SYSERR: assigning '%s' to illegal sphere %d",
            skill_name(spell), sphere);
    log(buf);
    bad = 1;
  }

  if ( ! bad ) {
    spell_info[spell].min_level_race[race] = level;
    spell_info[spell].spellSphere = sphere;
  }
}

void spell_level(int spell, int class, int level, int sphere)
{
  char buf[256];
  int bad = 0;

  if (spell < 0 || spell > TOP_SPELL_DEFINE) {
    sprintf(buf, "SYSERR: attempting assign to illegal spellnum %d", spell);
    log(buf);
    return;
  }

  if (class < 0 || class >= NUM_CLASSES) {
    sprintf(buf, "SYSERR: assigning '%s' to illegal class %d",
	    skill_name(spell), class);
    log(buf);
    bad = 1;
  }

  if (level < 1 || level > LVL_IMPL) {
    sprintf(buf, "SYSERR: assigning '%s' to illegal level %d",
	    skill_name(spell), level);
    log(buf);
    bad = 1;
  }

  if ( sphere < 0 ) {
    sprintf(buf, "SYSERR: assigning '%s' to illegal sphere %d",
	    skill_name(spell), sphere);
    log(buf);
    bad = 1;
  }

  if ( ! bad ) {
    spell_info[spell].min_level[class] = level;
    spell_info[spell].spellSphere = sphere;
  }
}

int sphereMajorLev[NUM_SPHERES+1] = 
{
    /*  AL  CR  HE  HR  NE  NR  DI  PR  CH  CO  AN  WE  PL  SU  EL */
        31, 14, 12, 12, 17, 17, 13, 17, 23, 20, 17, 20, 23, 19, 21
};

/*  0 =  no   access
 *  1 = minor access
 *  2 = major access

  Of course, whoever did this should have just made 0 the all sphere to make it
  easier on both me and them - something I'm doing now.  Soli, 8/99

 */

int checkSphere[NUM_GODS+1][NUM_SPHERES+1] = 
{
    /* AL CR HE HR NE NR DI PR CH CO AN WE PL SU EL  */
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, /* no god      0 */
    { 2, 2, 1, 0, 0, 0, 1, 2, 0, 2, 0, 0, 0, 1, 0 }, /* PALADINE    1 */
    { 2, 1, 1, 1, 2, 0, 0, 2, 0, 2, 0, 0, 0, 0, 0 }, /* KIRIJOLITH  2 */
    { 2, 2, 1, 0, 0, 0, 0, 1, 0, 0, 2, 0, 1, 0, 2 }, /* HABBAKUK    3 */
    { 2, 1, 2, 0, 2, 0, 1, 1, 2, 0, 0, 0, 0, 0, 0 }, /* MISHAKAL    4 */
    { 2, 2, 1, 0, 0, 0, 2, 0, 1, 0, 0, 2, 1, 0, 0 }, /* BRANCHALA   5 */
    { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, /* SOLINARI    6 */
    { 2, 2, 1, 0, 0, 0, 2, 1, 0, 1, 0, 0, 0, 2, 0 }, /* MAJERE      7 */
    { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 
    { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 
    { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 
    { 2, 2, 1, 0, 0, 0, 2, 2, 1, 0, 0, 0, 0, 1, 0 }, /* GILEAN     11 */
    { 2, 2, 1, 0, 0, 0, 0, 1, 0, 2, 0, 0, 0, 1, 2 }, /* REORX      12 */
    { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, /* LUNITARI   13 */
    { 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 0, 1 }, /* CHISLEV    14 */
    { 2, 1, 2, 0, 1, 0, 0, 0, 0, 2, 0, 0, 0, 1, 2 }, /* SIRRION    15 */
    { 2, 1, 1, 0, 1, 0, 2, 0, 0, 0, 0, 0, 2, 2, 0 }, /* ZIVILYN    16 */
    { 2, 2, 1, 0, 0, 0, 2, 1, 2, 0, 0, 0, 0, 0, 1 }, /* SHINARE    17 */
    { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 
    { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 
    { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 
    { 2, 1, 1, 2, 0, 0, 0, 2, 0, 2, 0, 0, 0, 2, 0 }, /* TAKHISIS   21 */
    { 2, 1, 1, 2, 0, 1, 0, 0, 0, 2, 0, 0, 0, 2, 0 }, /* SARGONNAS  22 */
    { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, /* NUITARI    23 */
    { 2, 2, 1, 2, 0, 2, 1, 0, 0, 0, 0, 0, 0, 1, 0 }, /* MORGION    24 */
    { 2, 1, 1, 0, 0, 2, 2, 0, 1, 2, 0, 0, 0, 0, 0 }, /* CHEMOSH    25 */
    { 2, 2, 1, 0, 0, 0, 1, 0, 0, 0, 1, 2, 0, 0, 2 }, /* ZEBOIM     26 */
    { 2, 2, 1, 0, 0, 0, 2, 1, 2, 0, 0, 0, 0, 1, 0 }, /* HIDDUKEL   27 */
    { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 
    { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 
    { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }  /* no god     30 */
};

/*  Since this has become the main checker, major overhaul time.  */
int spell_access(struct char_data * ch, int spellnum)
{
  int sphere, school = 0, access, major;
  int domain = GET_GODSELECT(ch);


  if ((spellnum < 1) || (spellnum > MAX_SKILLS))
    return FALSE;

   if ((GET_LEVEL(ch) < SINFO.min_level[(int) GET_CLASS(ch)]) &&
    (GET_LEVEL(ch) < SINFO.min_level_race[(int) GET_RACE(ch)])) {
    return FALSE;
   }

/*  Handling for alignment-restricted spells and skills - Soli, 10/2/99  */

  if (IS_GOOD(ch))
  {
    if ((spellnum == SPELL_DISPEL_GOOD) || (spellnum == SPELL_PROT_FROM_GOOD) ||
        (spellnum == SKILL_CONTROL_UNDEAD))
      return FALSE;
  }
  else if (IS_EVIL(ch))
  {
    if ((spellnum == SPELL_DISPEL_EVIL) || (spellnum == SPELL_PROT_FROM_EVIL) ||
        (spellnum == SKILL_TURN_UNDEAD))
      return FALSE;
  }
  else if (IS_NEUTRAL(ch))
  {
    if ((spellnum == SPELL_DISPEL_GOOD) || (spellnum == SPELL_DISPEL_EVIL) ||
        (spellnum == SPELL_PROT_FROM_GOOD) ||
        (spellnum == SPELL_PROT_FROM_EVIL) ||
        (spellnum == SKILL_CONTROL_UNDEAD))
      return FALSE;
  }

  if (spellnum > MAX_SPELLS)
    return TRUE;

  sphere = SINFO.spellSphere;

  while (sphere > NUM_SPHERES)
  {
    school += (NUM_SPHERES + 1);
    sphere -= (NUM_SPHERES + 1);
  }

  if ((sphere < 0) || (sphere > NUM_SPHERES) || (school > MAX_SCHOOL) ||
      (school % (NUM_SPHERES + 1)))
  {
    char tmp[100];
    sprintf(tmp, "SYSERR: spell %d has sphere %d, school %d",
      spellnum, sphere, school);
    log(tmp);
    return 1;
  }

  if (!IS_NPC(ch) && (GET_LEVEL(ch) >= LVL_GRGOD))
    return TRUE;

  if ((GET_CLASS(ch) == CLASS_CLERIC) || (GET_CLASS(ch) == CLASS_PALADIN))
  {
    access = checkSphere[domain][sphere];
    major = sphereMajorLev[sphere];

    if (GET_CLASS(ch) == CLASS_PALADIN)
    {
      access = MAX(access - 1, 0);
      major += 10;
    }

    switch(access) {
      case 0:
        return FALSE;
      case 2:
        return TRUE;
      case 1:
        if (SINFO.min_level[(int)GET_CLASS(ch)] < major)
          return TRUE;
        else
          return FALSE;
      default:
      {
        char tmp[100];
        sprintf(tmp, "SYSERR: spell %s has access of %d",
          skill_name(spellnum), access);
        log(tmp);
        return FALSE;
      }
    }

    return FALSE;
  }

  if (GET_CLASS(ch) == CLASS_MAGE)
  {
    switch (school)
    {
      case SCHOOL_ALL:
      case 0:
        return 1;
      case SCHOOL_WHITE:
        if (IS_GOOD(ch))
          return 1;
        else
          return 0;
      case SCHOOL_RED:
        if (IS_NEUTRAL(ch))
          return 1;
        else
          return 0;
      case SCHOOL_BLACK:
        if (IS_EVIL(ch))
          return 1;
        else
          return 0;
      case SCHOOL_NO_WHITE:
        if (IS_GOOD(ch))
          return 0;
        else
          return 1;
      case SCHOOL_NO_RED:
        if (IS_NEUTRAL(ch))
          return 0;
        else
          return 1;
      case SCHOOL_NO_BLACK:
        if (IS_EVIL(ch))
          return 0;
        else
          return 1;
      default:
        return 0;
    }
  }

  return TRUE;
}

/* Assign the spells on boot up */
void spello(int spl, int minpos, int targets, int violent, int routines, int wait, int applymod, int attrib, int maxperday, int maxpercent)
{
  int i;

  for (i = 0; i < NUM_CLASSES; i++) {
    spell_info[spl].min_level[i] = LVL_IMMORT;
  }

  for (i = 0; i < NUM_RACES; i++) {
    spell_info[spl].min_level_race[i] = LVL_IMMORT;
  }


  spell_info[spl].min_position = minpos;
  spell_info[spl].targets = targets;
  spell_info[spl].violent = violent;
  spell_info[spl].routines = routines;
  spell_info[spl].wait = wait;
  spell_info[spl].applymod = applymod;
  spell_info[spl].attrib = attrib;
  spell_info[spl].max_day = maxperday;
  spell_info[spl].max_percent = maxpercent;
}

void unused_spell(int spl)
{
  int i;

  for (i = 0; i < NUM_CLASSES; i++)
    spell_info[spl].min_level[i] = LVL_IMPL + 1;

   for (i = 0; i < NUM_RACES; i++)
    spell_info[spl].min_level_race[i] = LVL_IMPL + 1;

  spell_info[spl].min_position = 0;
  spell_info[spl].targets = 0;
  spell_info[spl].violent = 0;
  spell_info[spl].routines = 0;
  spell_info[spl].wait = 0;
  spell_info[spl].applymod = 0;
  spell_info[spl].attrib = 0;
  spell_info[spl].max_day = 1;
  spell_info[spl].max_percent = 0;
}

#define skillo(skill, applymod, attrib, max_day, max_percent) \
        spello(skill, 0, 0, 0, 0, 1, applymod, attrib, max_day, max_percent);

/*
 * Arguments for spello calls:
 *
 * spellnum, minpos, targets, violent, routines, wait
 *
 * spellnum:  Number of the spell.  Usually the symbolic name as defined in
 * spells.h (such as SPELL_HEAL).
 *
 * minpos  :  Minimum position the caster must be in for the spell to work
 * (usually fighting or standing).
 *
 * targets :  A "list" of the valid targets for the spell, joined with
 * bitwise OR ('|').
 *
 * violent :  TRUE or FALSE, depending on if this is considered a violent
 * spell and should not be cast in PEACEFUL rooms or on yourself.  Should be
 * set on any spell that inflicts damage, is considered aggressive (i.e.
 * charm, curse), or is otherwise nasty.
 *
 * routines:  A list of magic routines which are associated with this spell
 * if the spell uses spell templates.  Also joined with bitwise OR ('|').
 *
 * wait: the number of rounds (PULSE_VIOLENCES) the character must wait after
 * casting before performing any other actions.
 */

void mag_assign_spells(void)
{
  int i;

  /* Do not change the loop below */
  for (i = 1; i <= TOP_SPELL_DEFINE; i++)
    unused_spell(i);
  /* Do not change the loop above */


  spello(SPELL_ARMOR, POS_FIGHTING,
	 TAR_CHAR_ROOM, FALSE, MAG_AFFECTS, 1,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_TELEPORT, POS_FIGHTING,
         TAR_CHAR_WORLD , FALSE, MAG_MANUAL, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_BLESS, POS_STANDING,
	 TAR_CHAR_ROOM | TAR_OBJ_INV, FALSE,
	 MAG_AFFECTS | MAG_ALTER_OBJS, 1,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_BLINDNESS, POS_FIGHTING,
         TAR_CHAR_ROOM | TAR_NOT_SELF, TRUE, MAG_AFFECTS, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_BURNING_HANDS, POS_FIGHTING,
	 TAR_CHAR_ROOM | TAR_FIGHT_VICT, TRUE, MAG_DAMAGE, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_CALL_LIGHTNING, POS_FIGHTING,
	 TAR_CHAR_ROOM | TAR_FIGHT_VICT, TRUE, MAG_DAMAGE, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_CHARM, POS_FIGHTING,
	 TAR_CHAR_ROOM | TAR_NOT_SELF, TRUE, MAG_MANUAL, 3,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_CHILL_TOUCH, POS_FIGHTING,
	 TAR_CHAR_ROOM | TAR_FIGHT_VICT, TRUE, MAG_DAMAGE | MAG_AFFECTS, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_BLUR, POS_FIGHTING,
         TAR_CHAR_ROOM, FALSE, MAG_AFFECTS, 1,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_MASS_CHARM, POS_STANDING,
         TAR_IGNORE, FALSE, MAG_MASSES, 3,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_THORNFLESH, POS_FIGHTING,
         TAR_CHAR_ROOM, FALSE, MAG_AFFECTS, 1,
         100, ATTR_WIS, 1, 99); 

  spello(SPELL_CREATE_FOOD, POS_STANDING,
	 TAR_IGNORE, FALSE, MAG_CREATIONS, 1,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_CREATE_WATER, POS_STANDING,
	 TAR_OBJ_INV | TAR_OBJ_EQUIP, FALSE, MAG_MANUAL, 1,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_CURE_BLIND, POS_FIGHTING,
	 TAR_CHAR_ROOM, FALSE, MAG_UNAFFECTS, 1,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_CURE_CRITIC, POS_FIGHTING,
	 TAR_CHAR_ROOM, FALSE, MAG_POINTS, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_CURE_LIGHT, POS_FIGHTING,
	 TAR_CHAR_ROOM, FALSE, MAG_POINTS, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_BESTOW_CURSE, POS_FIGHTING,
	 TAR_CHAR_ROOM | TAR_OBJ_INV, TRUE,
	 MAG_AFFECTS | MAG_ALTER_OBJS, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_DETECT_ALIGN, POS_STANDING,
	 TAR_CHAR_ROOM | TAR_SELF_ONLY, FALSE, MAG_AFFECTS, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_DETECT_INVIS, POS_STANDING,
	 TAR_CHAR_ROOM | TAR_SELF_ONLY, FALSE, MAG_AFFECTS, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_DETECT_MAGIC, POS_STANDING,
	 TAR_CHAR_ROOM | TAR_SELF_ONLY, FALSE, MAG_AFFECTS, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_DETECT_POISON, POS_STANDING,
	 TAR_CHAR_ROOM | TAR_OBJ_INV | TAR_OBJ_ROOM, FALSE, MAG_MANUAL, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_DISPEL_EVIL, POS_FIGHTING,
	 TAR_CHAR_ROOM | TAR_FIGHT_VICT, TRUE, MAG_DAMAGE, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_CRIPPLE, POS_FIGHTING,
         TAR_CHAR_ROOM, TRUE, MAG_MANUAL, 2,
         100, ATTR_WIS, 1, 99);
 
  spello(SPELL_ENCHANT_WEAPON, POS_STANDING,
         TAR_OBJ_INV | TAR_OBJ_EQUIP, FALSE, MAG_MANUAL, 3,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_ENERGY_DRAIN, POS_FIGHTING,
         TAR_CHAR_ROOM | TAR_FIGHT_VICT, TRUE, MAG_MANUAL, 3,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_FIREBALL, POS_FIGHTING,
	 TAR_CHAR_ROOM | TAR_FIGHT_VICT, TRUE, MAG_DAMAGE, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_HARM, POS_FIGHTING,
	 TAR_CHAR_ROOM | TAR_FIGHT_VICT, TRUE, MAG_DAMAGE, 3,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_HEAL, POS_FIGHTING, TAR_CHAR_ROOM, FALSE,
	 MAG_POINTS | MAG_AFFECTS | MAG_UNAFFECTS, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_INVISIBILITY, POS_STANDING,
	 TAR_CHAR_ROOM | TAR_OBJ_INV, FALSE,
	 MAG_AFFECTS | MAG_ALTER_OBJS, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_LIGHTNING_BOLT, POS_FIGHTING,
	 TAR_CHAR_ROOM | TAR_FIGHT_VICT, TRUE, MAG_DAMAGE, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_LOCATE_OBJECT, POS_STANDING,
	 TAR_OBJ_WORLD, FALSE, MAG_MANUAL, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_MAGIC_MISSILE, POS_FIGHTING,
	 TAR_CHAR_ROOM | TAR_FIGHT_VICT, TRUE, MAG_DAMAGE, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_POISON, POS_STANDING,
	 TAR_CHAR_ROOM | TAR_NOT_SELF | TAR_OBJ_INV, TRUE,
	 MAG_AFFECTS | MAG_ALTER_OBJS, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_PROT_FROM_EVIL, POS_STANDING,
	 TAR_CHAR_ROOM | TAR_SELF_ONLY, FALSE, MAG_AFFECTS, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_REMOVE_CURSE, POS_FIGHTING,
	 TAR_CHAR_ROOM | TAR_OBJ_INV, FALSE,
	 MAG_UNAFFECTS | MAG_ALTER_OBJS, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_SANCTUARY, POS_FIGHTING,
	 TAR_CHAR_ROOM, FALSE, MAG_AFFECTS, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_SHOCKING_GRASP, POS_FIGHTING,
	 TAR_CHAR_ROOM | TAR_FIGHT_VICT, TRUE, MAG_DAMAGE, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_SLEEP, POS_STANDING,
	 TAR_CHAR_ROOM, FALSE, MAG_AFFECTS, 3,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_STRENGTH, POS_FIGHTING,
	 TAR_CHAR_ROOM, FALSE, MAG_AFFECTS, 1,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_BLESSING_OF_KINGS, POS_FIGHTING,
         TAR_CHAR_ROOM, FALSE, MAG_GROUPS, 1,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_ENHANCE_AGILITY, POS_FIGHTING,
	 TAR_CHAR_ROOM, FALSE, MAG_AFFECTS, 1,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_ENHANCE_ENDURANCE, POS_FIGHTING,
	 TAR_CHAR_ROOM, FALSE, MAG_AFFECTS, 1,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_ENHANCE_INSIGHT, POS_FIGHTING,
	 TAR_CHAR_ROOM, FALSE, MAG_AFFECTS, 1,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_ENHANCE_CUNNING, POS_FIGHTING,
	 TAR_CHAR_ROOM, FALSE, MAG_AFFECTS, 1,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_ENHANCE_CHARISMA, POS_FIGHTING,
	 TAR_CHAR_ROOM, FALSE, MAG_AFFECTS, 1,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_SUMMON, POS_STANDING,
	 TAR_CHAR_WORLD | TAR_NOT_SELF, FALSE, MAG_MANUAL, 3,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_SHIELD, POS_STANDING,
         TAR_CHAR_ROOM, FALSE, MAG_AFFECTS, 1,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_WORD_OF_RECALL, POS_FIGHTING,
         TAR_CHAR_ROOM | TAR_SELF_ONLY, FALSE, MAG_MANUAL, 3,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_NEUTRALIZE_POISON, POS_STANDING,
	 TAR_CHAR_ROOM | TAR_OBJ_INV | TAR_OBJ_ROOM, FALSE,
	 MAG_UNAFFECTS | MAG_ALTER_OBJS, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_SENSE_LIFE, POS_STANDING,
	 TAR_CHAR_ROOM | TAR_SELF_ONLY, FALSE, MAG_AFFECTS, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_LEGEND_LORE, POS_STANDING,
         TAR_OBJ_INV, FALSE, MAG_MANUAL, 3,
         100, ATTR_WIS, 1, 99); 
  
  spello(SPELL_DISPEL_GOOD, POS_FIGHTING,
         TAR_CHAR_ROOM | TAR_FIGHT_VICT, TRUE, MAG_DAMAGE, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_MINOR_SANCTUARY, POS_FIGHTING,
         TAR_CHAR_ROOM, FALSE, MAG_AFFECTS, 1,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_HEALING_LIGHT, POS_FIGHTING,
         TAR_IGNORE, FALSE, MAG_GROUPS, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_PRAYER, POS_FIGHTING,
         TAR_IGNORE, FALSE, MAG_GROUPS, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_INFRAVISION, POS_STANDING,
         TAR_CHAR_ROOM | TAR_SELF_ONLY, FALSE, MAG_AFFECTS, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_WATER_WALK, POS_STANDING,
         TAR_CHAR_ROOM, FALSE, MAG_AFFECTS, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_BARKSKIN, POS_FIGHTING,
         TAR_CHAR_ROOM, FALSE, MAG_AFFECTS, 2,
         100, ATTR_WIS, 1, 99);
  
  spello(SPELL_STONESKIN, POS_FIGHTING,
         TAR_CHAR_ROOM, FALSE, MAG_AFFECTS, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_HARMFUL_WRATH, POS_FIGHTING,
         TAR_IGNORE, FALSE, MAG_MASSES, 3,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_CURE_DEAFNESS, POS_FIGHTING,
         TAR_CHAR_ROOM, FALSE, MAG_UNAFFECTS, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_STATUE, POS_FIGHTING,
         TAR_CHAR_ROOM | TAR_SELF_ONLY, FALSE, MAG_AFFECTS, 3,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_MIRROR_IMAGE, POS_FIGHTING,
         TAR_CHAR_ROOM | TAR_SELF_ONLY, FALSE, MAG_AFFECTS, 1,
         100, ATTR_WIS, 1, 99); 

  spello(SPELL_FLIGHT, POS_STANDING,
	 TAR_CHAR_ROOM, FALSE, MAG_AFFECTS, 2,
         100, ATTR_WIS, 1, 99); 
                      
  spello(SPELL_WEAKNESS, POS_FIGHTING, 
	 TAR_CHAR_ROOM | TAR_NOT_SELF, TRUE, MAG_AFFECTS, 2,
         100, ATTR_WIS, 1, 99);
  	    	   
  spello(SPELL_DEAFENING_WIND, POS_FIGHTING,
         TAR_CHAR_ROOM | TAR_FIGHT_VICT, TRUE, MAG_AFFECTS, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_CURE_DISEASE, POS_FIGHTING,
         TAR_CHAR_ROOM, FALSE, MAG_UNAFFECTS, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_REMOVE_PARALYSIS, POS_FIGHTING,
         TAR_CHAR_ROOM, FALSE, MAG_UNAFFECTS, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_RESTORATION, POS_FIGHTING,
         TAR_CHAR_ROOM, FALSE, MAG_UNAFFECTS, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_REGENERATION, POS_FIGHTING,
         TAR_CHAR_ROOM, FALSE, MAG_UNAFFECTS, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_HASTE, POS_FIGHTING,
	 TAR_CHAR_ROOM, FALSE, MAG_AFFECTS | MAG_UNAFFECTS, 2,
         100, ATTR_WIS, 1, 99);
	
  spello(SPELL_DISEASE, POS_FIGHTING,
         TAR_CHAR_ROOM, TRUE, MAG_AFFECTS, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_PARALYZE, POS_FIGHTING,
         TAR_CHAR_ROOM, TRUE, MAG_AFFECTS, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_SOUL_DRAIN, POS_FIGHTING,
         TAR_CHAR_ROOM | TAR_FIGHT_VICT, TRUE, MAG_AFFECTS, 3,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_WITHER, POS_FIGHTING,
         TAR_CHAR_ROOM, TRUE, MAG_AFFECTS, 3,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_SLOW, POS_FIGHTING,
         TAR_CHAR_ROOM, TRUE, MAG_AFFECTS | MAG_UNAFFECTS | MAG_PULSE_BASED, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_GATE, POS_STANDING,
         TAR_CHAR_WORLD | TAR_NOT_SELF, FALSE, MAG_MANUAL, 4,
         100, ATTR_WIS, 1, 99);
	
  spello(SPELL_ELEMENTAL_STORM, POS_FIGHTING,
         TAR_IGNORE, FALSE, MAG_AREA_MAGIC, 3,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_CREATE_SPRING, POS_STANDING,
         TAR_CHAR_ROOM, FALSE, MAG_MANUAL, 2,
         100, ATTR_WIS, 1, 99);
	
  spello(SPELL_PROT_FROM_GOOD, POS_STANDING,
         TAR_CHAR_ROOM, FALSE, MAG_AFFECTS, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_PROT_FROM_UNDEAD, POS_STANDING,
         TAR_CHAR_ROOM, FALSE, MAG_AFFECTS, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_PROT_FROM_FIRE, POS_STANDING,
         TAR_CHAR_ROOM, FALSE, MAG_AFFECTS, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_PROT_FROM_FROST, POS_STANDING,
         TAR_CHAR_ROOM, FALSE, MAG_AFFECTS, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_PROT_FROM_ELEMENTS, POS_STANDING,
         TAR_CHAR_ROOM, FALSE, MAG_AFFECTS, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_IMPERVIOUS_MIND, POS_STANDING,
         TAR_CHAR_ROOM, FALSE, MAG_AFFECTS | MAG_UNAFFECTS, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_CALM, POS_FIGHTING,
         TAR_IGNORE, FALSE, MAG_MANUAL, 3,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_CLOAK_OF_FEAR, POS_FIGHTING,
         TAR_CHAR_ROOM | TAR_SELF_ONLY, FALSE, MAG_AFFECTS, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_INSPIRE_FEAR, POS_FIGHTING,
         TAR_CHAR_ROOM, TRUE, MAG_AFFECTS, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_HEROES_FEAST, POS_STANDING,
         TAR_IGNORE, FALSE, MAG_GROUPS, 3,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_BRAVERY, POS_FIGHTING,
         TAR_CHAR_ROOM, FALSE, MAG_AFFECTS | MAG_UNAFFECTS, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_GREATER_ELEMENTAL, POS_FIGHTING,
         TAR_IGNORE, FALSE, MAG_SUMMONS, 3,
         100, ATTR_WIS, 1, 99);

  /*  Unused - was morale  */

  spello(SPELL_BLADE_BARRIER, POS_FIGHTING,
         TAR_IGNORE, FALSE, MAG_AREA_MAGIC, 3,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_FAERIE_FIRE, POS_FIGHTING,
         TAR_CHAR_ROOM, TRUE, MAG_AFFECTS, 1,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_SUNRAY, POS_FIGHTING,
         TAR_CHAR_ROOM | TAR_FIGHT_VICT, TRUE, MAG_DAMAGE, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_MOONBEAM, POS_FIGHTING,
         TAR_IGNORE, FALSE, MAG_MASSES, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_DISPEL_MAGIC, POS_FIGHTING,
         TAR_CHAR_ROOM | TAR_OBJ_INV, FALSE, MAG_MANUAL, 2,
         100, ATTR_WIS, 1, 99);
  
  spello(SPELL_LIGHT, POS_STANDING,
	 TAR_CHAR_ROOM, FALSE, MAG_AFFECTS, 1,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_DARKNESS, POS_STANDING,
	 TAR_CHAR_ROOM, FALSE, MAG_AFFECTS, 1,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_SILENCE, POS_FIGHTING,
	 TAR_CHAR_ROOM, TRUE, MAG_AFFECTS | MAG_PULSE_BASED, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_SPHERE_SILENCE, POS_FIGHTING,
	 TAR_IGNORE, TRUE, MAG_MASSES, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_CURE_MODERATE, POS_FIGHTING,
	 TAR_CHAR_ROOM, FALSE, MAG_POINTS, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_CURE_SEVERE, POS_FIGHTING,
	 TAR_CHAR_ROOM, FALSE, MAG_POINTS, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_CAUSE_LIGHT, POS_FIGHTING,
	 TAR_CHAR_ROOM | TAR_FIGHT_VICT, TRUE, MAG_DAMAGE, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_CAUSE_MODERATE, POS_FIGHTING,
	 TAR_CHAR_ROOM | TAR_FIGHT_VICT, TRUE, MAG_DAMAGE, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_CAUSE_SEVERE, POS_FIGHTING,
	 TAR_CHAR_ROOM | TAR_FIGHT_VICT, TRUE, MAG_DAMAGE, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_CAUSE_CRITIC, POS_FIGHTING,
	 TAR_CHAR_ROOM | TAR_FIGHT_VICT, TRUE, MAG_DAMAGE, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_GOODBERRY, POS_STANDING,
         TAR_IGNORE, FALSE, MAG_CREATIONS, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_MAGIC_VEST, POS_FIGHTING,
	 TAR_CHAR_ROOM, FALSE, MAG_AFFECTS, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_FREE_ACTION, POS_STANDING,
	 TAR_CHAR_ROOM, FALSE, MAG_UNAFFECTS | MAG_AFFECTS, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_MAGICAL_STONE, POS_FIGHTING,
	 TAR_CHAR_ROOM | TAR_FIGHT_VICT, TRUE, MAG_DAMAGE, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_SHILLELAGH, POS_STANDING,
	 TAR_OBJ_INV | TAR_OBJ_EQUIP, FALSE, MAG_MANUAL, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_SPIRIT_HAMMER, POS_FIGHTING,
	 TAR_CHAR_ROOM | TAR_SELF_ONLY, FALSE,
         MAG_AFFECTS | MAG_PULSE_BASED, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_FLAMESTRIKE, POS_FIGHTING,
	 TAR_CHAR_ROOM | TAR_FIGHT_VICT, TRUE, MAG_DAMAGE, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_AID, POS_STANDING,
	 TAR_CHAR_ROOM, FALSE, MAG_AFFECTS, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_CALL_ANIMAL_SPIRIT, POS_STANDING,
         TAR_IGNORE, FALSE, MAG_SUMMONS, 3,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_SUMMON_INSECTS, POS_FIGHTING,
         TAR_CHAR_ROOM, TRUE, MAG_SUMMONS, 3,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_ANIMAL_SUMMONING, POS_STANDING,
         TAR_IGNORE, FALSE, MAG_SUMMONS, 3,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_ANIMAL_SUMMONING_II, POS_STANDING,
         TAR_IGNORE, FALSE, MAG_SUMMONS, 3,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_ANIMAL_SUMMONING_III, POS_STANDING,
         TAR_IGNORE, FALSE, MAG_SUMMONS, 3,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_CREEPING_DOOM, POS_FIGHTING,
         TAR_IGNORE, FALSE, MAG_AREA_MAGIC, 3,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_INSECT_PLAGUE, POS_FIGHTING,
         TAR_IGNORE, FALSE, MAG_AREA_MAGIC, 3,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_RAINBOW, POS_FIGHTING,
         TAR_CHAR_ROOM | TAR_FIGHT_VICT, TRUE, MAG_AFFECTS | MAG_DAMAGE, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_ENTANGLE, POS_FIGHTING,
         TAR_CHAR_ROOM, TRUE, MAG_AFFECTS, 1,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_THORNSHIELD, POS_FIGHTING,
         TAR_CHAR_ROOM | TAR_SELF_ONLY, FALSE, MAG_AFFECTS, 3,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_STICKS_TO_SNAKES, POS_FIGHTING,
         TAR_CHAR_ROOM, TRUE, MAG_SUMMONS, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_AERIAL_SERVANT, POS_STANDING,
         TAR_IGNORE, FALSE, MAG_SUMMONS, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_SUMMON_GUARD, POS_STANDING,
         TAR_IGNORE, FALSE, MAG_SUMMONS, 3,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_DUST_DEVIL, POS_FIGHTING,
         TAR_CHAR_ROOM, TRUE, MAG_SUMMONS, 3,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_FLAME_BLADE, POS_FIGHTING,
         TAR_CHAR_ROOM | TAR_SELF_ONLY, FALSE,
         MAG_AFFECTS | MAG_PULSE_BASED, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_WATER_BREATHING, POS_STANDING,
         TAR_CHAR_ROOM, FALSE, MAG_AFFECTS, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_CONJURE_ELEMENTAL, POS_FIGHTING,
         TAR_IGNORE, FALSE, MAG_SUMMONS, 3,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_WIND_WALK, POS_STANDING,
         TAR_IGNORE, FALSE, MAG_GROUPS, 3,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_PHANTASMAL_SNAKE, POS_FIGHTING,
         TAR_SELF_ONLY | TAR_CHAR_ROOM, FALSE, MAG_AFFECTS, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_IDENTIFY, POS_STANDING,
         TAR_OBJ_INV, FALSE, MAG_MANUAL, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_BREATH_OF_LIFE, POS_STANDING,
         TAR_IGNORE, FALSE, MAG_MASSES, 3,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_BLACK_PLAGUE, POS_STANDING,
         TAR_IGNORE, FALSE, MAG_MASSES, 3,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_REFLECTING_POOL, POS_STANDING,
         TAR_CHAR_WORLD, FALSE, MAG_MANUAL, 3,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_RECOLLECTION, POS_STANDING,
         TAR_SELF_ONLY | TAR_CHAR_ROOM, FALSE, MAG_AFFECTS, 0,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_REMOVE_FEAR, POS_FIGHTING,
         TAR_CHAR_ROOM, FALSE, MAG_UNAFFECTS, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_DIVINE_WRATH, POS_FIGHTING,
         TAR_IGNORE, FALSE, MAG_GROUPS, 3,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_HOLY_WORD, POS_FIGHTING,
         TAR_IGNORE, FALSE, MAG_MANUAL, 3,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_MYSTIC_SPIRIT, POS_STANDING,
         TAR_CHAR_ROOM, FALSE, MAG_AFFECTS, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_STORM_SUMMONING, POS_FIGHTING,
         TAR_IGNORE, FALSE, MAG_AREA_MAGIC, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_DEHYDRATION, POS_FIGHTING,
         TAR_IGNORE, FALSE, MAG_AREA_MAGIC, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_MIRE, POS_FIGHTING,
         TAR_IGNORE, FALSE, MAG_MASSES, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_ABJURE, POS_FIGHTING,
         TAR_CHAR_ROOM, TRUE, MAG_MANUAL, 3,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_ADAMANT_MACE, POS_FIGHTING,
         TAR_CHAR_ROOM | TAR_SELF_ONLY, FALSE, MAG_AFFECTS, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_HOLD_PERSON, POS_FIGHTING,
         TAR_CHAR_ROOM, FALSE, MAG_AFFECTS, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_WEB, POS_FIGHTING,
         TAR_CHAR_ROOM | TAR_FIGHT_VICT, FALSE, MAG_AFFECTS, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_GUST_OF_WIND, POS_FIGHTING,
         TAR_CHAR_ROOM, FALSE, MAG_MANUAL, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_MINOR_GLOBE, POS_FIGHTING,
         TAR_SELF_ONLY | TAR_CHAR_ROOM, FALSE, MAG_AFFECTS, 1,
         100, ATTR_WIS, 1, 99);
 
  spello(SPELL_MAJOR_GLOBE, POS_FIGHTING,
         TAR_SELF_ONLY | TAR_CHAR_ROOM, FALSE, MAG_AFFECTS, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_VAMPIRIC_TOUCH, POS_FIGHTING,
         TAR_CHAR_ROOM | TAR_FIGHT_VICT, TRUE, MAG_DAMAGE, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_DANCING_SWORD, POS_STANDING,
         TAR_CHAR_ROOM | TAR_SELF_ONLY, FALSE, MAG_AFFECTS, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_SCRY, POS_STANDING,
         TAR_CHAR_WORLD, FALSE, MAG_MANUAL, 3,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_CHAIN_LIGHTNING, POS_FIGHTING,
         TAR_IGNORE, TRUE, MAG_MANUAL, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_TELEPORT_OBJECT, POS_STANDING,
         TAR_OBJ_AND_WLD, FALSE, MAG_MANUAL, 3,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_ARROW_OF_BONE, POS_FIGHTING,
         TAR_CHAR_ROOM | TAR_FIGHT_VICT, TRUE, MAG_DAMAGE | MAG_AFFECTS, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_CONE_OF_COLD, POS_FIGHTING,
         TAR_CHAR_ROOM | TAR_FIGHT_VICT, TRUE, MAG_DAMAGE, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_FIRE_SHIELD, POS_FIGHTING,
         TAR_CHAR_ROOM | TAR_SELF_ONLY, FALSE, MAG_AFFECTS, 3,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_BLACKMANTLE, POS_FIGHTING,
         TAR_CHAR_ROOM | TAR_FIGHT_VICT, TRUE, MAG_AFFECTS, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_BLINK, POS_FIGHTING,
         TAR_CHAR_ROOM | TAR_SELF_ONLY, FALSE, MAG_AFFECTS, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_PRISMATIC_SPRAY, POS_FIGHTING,
         TAR_CHAR_ROOM | TAR_FIGHT_VICT, TRUE, MAG_DAMAGE | MAG_AFFECTS, 3,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_METEOR_SWARM, POS_FIGHTING,
         TAR_IGNORE, TRUE, MAG_MASSES, 3,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_MAGICAL_SUSCEPT, POS_FIGHTING,
         TAR_CHAR_ROOM, TRUE, MAG_AFFECTS, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_ANTIMAGIC_AURA, POS_FIGHTING,
         TAR_CHAR_ROOM, TRUE, MAG_AFFECTS | MAG_PULSE_BASED, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_DECEPTIVE_IMAGERY, POS_FIGHTING,
         TAR_CHAR_ROOM | TAR_SELF_ONLY, FALSE, MAG_AFFECTS, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_SPELL_TURNING, POS_FIGHTING,
         TAR_CHAR_ROOM | TAR_SELF_ONLY, FALSE, MAG_AFFECTS, 3,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_RECHARGE_ITEM, POS_STANDING,
         TAR_OBJ_INV, FALSE, MAG_MANUAL, 4,
         200, ATTR_WIS, 1, 99);

  spello(SPELL_VAMPIRIC_AURA, POS_FIGHTING,
         TAR_CHAR_ROOM | TAR_SELF_ONLY, FALSE, MAG_AFFECTS, 2,
         100, ATTR_WIS, 1, 99);
 
  spello(SPELL_REFRESH, POS_STANDING,
         TAR_CHAR_ROOM, FALSE, MAG_POINTS, 1,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_POWER_WORD_KILL, POS_FIGHTING,
	 TAR_CHAR_ROOM | TAR_FIGHT_VICT, TRUE, MAG_DAMAGE, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_CONCEAL_ALIGN, POS_STANDING,
         TAR_CHAR_ROOM | TAR_SELF_ONLY, FALSE, MAG_AFFECTS, 2,
         100, ATTR_WIS, 1, 99);

  spello(SPELL_POLYMORPH, POS_STANDING,
         TAR_CHAR_ROOM | TAR_SELF_ONLY, FALSE, MAG_AFFECTS, 3,
         100, ATTR_WIS, 1, 99);

   spello(SPELL_REVEAL_ILLUSION, POS_STANDING,
         TAR_CHAR_ROOM, FALSE, MAG_MANUAL, 3,
         100, ATTR_WIS, 1, 99);

   spello(SPELL_SK_CHALLENGE, POS_FIGHTING,
         TAR_CHAR_ROOM, FALSE, MAG_GROUPS, 3,
         100, ATTR_WIS, 1, 99);

   spello(SPELL_DONTUSEME, POS_STANDING,
         TAR_CHAR_ROOM, FALSE, MAG_AFFECTS | MAG_PULSE_BASED, 3,
         100, ATTR_WIS, 1, 99);
  
  skillo(SKILL_BACKSTAB, 100, ATTR_DEX, 1, 90);
  skillo(SKILL_BASH, 100, ATTR_STR, 1, 85);
  skillo(SKILL_HIDE, 100, ATTR_DEX, 1, 90);
  skillo(SKILL_KICK, 100, ATTR_STR, 1, 85);
  skillo(SKILL_PICK_LOCK, 100, ATTR_DEX, 1, 95);
  skillo(SKILL_RESCUE, 100, ATTR_DEX, 1, 90);
  skillo(SKILL_SNEAK, 100, ATTR_DEX, 1, 95);
  skillo(SKILL_STEAL, 100, ATTR_DEX, 1, 80);
  skillo(SKILL_TRACK, 100, ATTR_WIS, 1, 90);
  skillo(SKILL_SCAN, 100, ATTR_DEX, 1, 90);
  skillo(SKILL_SECOND_ATTACK, 100, ATTR_STR, 1, 90);
  skillo(SKILL_THIRD_ATTACK, 100, ATTR_STR, 1, 90);
  skillo(SKILL_FOURTH_ATTACK, 100, ATTR_STR, 1, 90);
  skillo(SKILL_DUAL_WIELD, 100, ATTR_STR, 1, 90);
  skillo(SKILL_DODGE, 100, ATTR_DEX, 1, 80);
  skillo(SKILL_ASSESS, 100, ATTR_WIS, 1, 90);
  skillo(SKILL_DISARM, 100, ATTR_STR, 1, 85);
  skillo(SKILL_RETREAT, 100, ATTR_DEX, 1, 85);
  skillo(SKILL_GUARD, 100, ATTR_DEX , 1, 80);
  skillo(SKILL_LAY_HANDS, 100, ATTR_WIS, 1, 100);
  skillo(SKILL_TURN_UNDEAD, 100, ATTR_CON, 1, 80);
  skillo(SKILL_CONTROL_UNDEAD, 100, ATTR_CON, 1, 80);
  skillo(SKILL_STANCE, 100, ATTR_STR, 1, 90);
  skillo(SKILL_GORE, 100, ATTR_STR, 1, 85);
  skillo(SKILL_DETECT_POISON, 100, ATTR_WIS, 1, 95);
  skillo(SKILL_REMOVE_POISON, 100, ATTR_WIS, 1, 90);
  skillo(SKILL_READ_MAGIC, 100, ATTR_WIS, 1, 90);
  skillo(SKILL_ASSESS_MAGIC, 100, ATTR_WIS, 1, 85);
  skillo(SKILL_SCRIBE, 200, ATTR_WIS, 1, 85 );
  skillo(SKILL_BREW, 200, ATTR_WIS, 1, 85);
  skillo(SKILL_NATURAL_HARDINESS, 100, ATTR_CON, 1, 90);
  skillo(SKILL_CALM, 100, ATTR_CHA, 1, 90);
  skillo(SKILL_BANDAGE, 100, ATTR_WIS, 1, 95);
  skillo(SKILL_ENVENOM, 100, ATTR_DEX, 1, 85);
  skillo(SKILL_DISGUISE, 100, ATTR_DEX, 1, 95);
  skillo(SKILL_CHAKRA, 100, ATTR_WIS, 1, 100);
  skillo(SKILL_THROW, 100, ATTR_DEX, 1, 100);
  skillo(SKILL_RIDING, 100, ATTR_DEX , 1, 85);
  skillo(SKILL_TAME, 100, ATTR_CHA , 1, 80);
  skillo(SKILL_SKIN, 100, ATTR_DEX , 1, 85);
  skillo(SKILL_JAWSTRIKE, 100, ATTR_DEX , 1, 85);
  skillo(SKILL_DIRTKICK, 100, ATTR_DEX , 1, 85);
  skillo(SKILL_BUILD_FIRE, 100, ATTR_INT , 1, 90);
  skillo(SKILL_COOK, 100, ATTR_WIS , 1, 90);
  skillo(SKILL_DETECTDISGUISE, 100, ATTR_WIS , 1, 90);
  skillo(SKILL_KNOCKOUT, 100, ATTR_DEX , 1, 80);
  skillo(SKILL_PARRY, 100, ATTR_DEX, 1, 80);
  skillo(SKILL_PASS_WO_TRACE , 100, ATTR_DEX, 1, 85);
  skillo(SKILL_TRIP, 100, ATTR_DEX, 1, 85);
  skillo(SKILL_ENCHANT_ITEM, 100, ATTR_INT, 1, 90);
  skillo(SKILL_TWO_HANDED_STYLE, 100, ATTR_STR, 1, 90);
  skillo(SKILL_TWO_WEAPON_STYLE, 100, ATTR_DEX, 1, 90);
  skillo(SKILL_ONE_WEAPON_STYLE, 100, ATTR_DEX, 1, 90);
  skillo(SKILL_SWORD_SHIELD_STYLE, 100, ATTR_INT, 1, 90);
  skillo(SKILL_CHARGE, 100, ATTR_STR, 1, 90);
  skillo(SKILL_IMPROVED_CHARGE, 100, ATTR_STR, 1, 90);
  skillo(SKILL_IMPROVED_BACKSTAB, 100, ATTR_DEX, 1, 90);
  skillo(SKILL_IMPROVED_STEALTH, 100, ATTR_DEX, 1, 90);
  skillo(SKILL_ADVANCED_BACKSTAB, 100, ATTR_DEX, 1, 90);
  skillo(SKILL_ARMOR_MASTERY, 100, ATTR_DEX, 1, 90);
  skillo(SKILL_SHIELD_MASTERY, 100, ATTR_DEX, 1, 90);
  skillo(SKILL_IMPROVED_ARMOR_MASTERY, 100, ATTR_DEX, 1, 90);
  skillo(SKILL_ADVANCED_ARMOR_MASTERY, 100, ATTR_DEX, 1, 90);
  skillo(SKILL_IMPROVED_SHIELD_MASTERY, 100, ATTR_DEX, 1, 90);
  skillo(SKILL_ADVANCED_SHIELD_MASTERY, 100, ATTR_DEX, 1, 90);
  skillo(SKILL_DAMAGE_REDUCTION, 100, ATTR_CON, 1, 90);
  skillo(SKILL_IMPROVED_DAMAGE_REDUCTION, 100, ATTR_CON, 1, 90);
  skillo(SKILL_ADVANCED_DAMAGE_REDUCTION, 100, ATTR_CON, 1, 90);
  skillo(SKILL_DIRTY_FIGHTING, 100, ATTR_INT, 1, 90);
  skillo(SKILL_COMBAT_IMPROVISATION, 100, ATTR_INT, 1, 90);
  skillo(SKILL_MARTIAL_ARTS, 100, ATTR_DEX, 1, 90);
  skillo(SKILL_IMPROVED_MARTIAL_ARTS, 100, ATTR_DEX, 1, 90);
  skillo(SKILL_ADVANCED_MARTIAL_ARTS, 100, ATTR_DEX, 1, 90);
  skillo(SKILL_INSIGHT, 100, ATTR_WIS, 1, 90);
  skillo(SKILL_UNARMED_DEFENSE, 100, ATTR_DEX, 1, 90);
  skillo(SKILL_IMPROVED_UNARMED_DEFENSE, 100, ATTR_DEX, 1, 90);
  skillo(SKILL_ADVANCED_UNARMED_DEFENSE, 100, ATTR_DEX, 1, 90);
  skillo(SKILL_THROW_OPPONENT, 100, ATTR_STR, 1, 90);
  skillo(SKILL_CHALLENGE, 100, ATTR_CHA, 1, 90);
  skillo(SKILL_MARK, 100, ATTR_DEX, 1, 90);
}
