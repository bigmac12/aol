/* ************************************************************************
*   File: constants.c                                   Part of CircleMUD *
*  Usage: Numeric and string contants used by the MUD                     *
*                                                                         *
*  All rights reserved.  See license.doc for complete information.        *
*                                                                         *
*  Copyright (C) 1993, 94 by the Trustees of the Johns Hopkins University *
*  CircleMUD is based on DikuMUD, Copyright (C) 1990, 1991.               *
************************************************************************ */

#include "conf.h"
#include "sysdep.h"

#include "structs.h"
#include "spells.h"
//  We need to include this for mob spellcasters - Soli, 8/4/99

cpp_extern const char *circlemud_version = "CircleMUD, version 3.00 beta patchlevel 17-GAN";

// cpp_extern const char circlemud_version[] = 
// 	{"CircleMUD, version 3.00 beta patchlevel 17-GAN", "\0"};

/* strings corresponding to ordinals/bitvectors in structs.h ***********/


/* (Note: strings for class definitions in class.c instead of here) */


int holiday_exp_modifier = 100;

/* cardinal directions */
const char *dirs[] =
{
  "north",
  "east",
  "south",
  "west",
  "up",
  "down",
  "northwest",
  "northeast",
  "southwest",
  "southeast",
  "\n"
};

const char *sdirs[] =
{
   "n",
   "e",
   "s",
   "w",
   "u",
   "d",
   "nw",
   "ne",
   "sw",
   "se",
   "\n"
};

const char *doordirs[] =
{
  "north",
  "east",
  "south",
  "west",
  "up",
  "down",
  "northwest",
  "northeast",
  "southwest",
  "southeast",
  "nwest",
  "neast",
  "swest",
  "seast",
   "\n"
};
   
/* ROOM_x */
const char *room_bits[] = {
  "DARK",
  "DEATH",
  "!MOB",
  "INDOORS",
  "PEACEFUL",
  "SOUNDPROOF",
  "!TRACK",
  "!MAGIC",
  "TUNNEL",
  "PRIVATE",
  "GODROOM",
  "HOUSE",
  "HCRSH",
  "ATRIUM",
  "OLC",
  "*",				/* BFS MARK */
  "PKILL",
  "NO_RECALL",
  "GUARDED",
  "PULSE_DAMAGE",
  "NO OOC",
  "SALTWATER_FISHING",
  "FRESHWATER_FISHING",
  "!DIG",
  "!BURY",
  "TOWN_HOUSE",
  "CUSTOM_HOUSE",
  "\n"
};


/* EX_x */
const char *exit_bits[] = {
  "DOOR",
  "CLOSED",
  "LOCKED",
  "PICKPROOF",
  "HIDDEN",
  "\n"
};


/* SECT_ */
const char *sector_types[] = {
  "Inside",
  "City",
  "Field",
  "Forest",
  "Hills",
  "Mountains",
  "Water (Swim)",
  "Water (No Swim)",
  "Underwater",
  "In Flight",
  "\n"
};


/* SEX_x Not used in sprinttype() so no \n. */
const char *genders[] =
{
  "Neutral",
  "Male",
  "Female"
};

/* POS_x */
const char *position_types[] = {
  "Dead",
  "Mortally wounded",
  "Incapacitated",
  "Stunned",
  "Sleeping",
  "Resting",
  "Sitting",
  "Fighting",
  "Standing",
  "Digging",
  "Fishing",
  "Riding",
  "\n"
};


/* PLR_x */
const char *player_bits[] = {
  "KILLER",
  "THIEF",
  "FROZEN",
  "DONTSET",
  "WRITING",
  "MAILING",
  "CRASH",
  "SITEOK",
  "NOSHOUT",
  "RABBIT",
  "DELETED",
  "LOADRM",
  "WOLF",
  "BEAR",
  "COURIER",
  "CAT",
  "BIRD",
  "IMMCHAR",
  "FREERENT",
  "SUBDUING",
  "PRISONER",
  "FISHING",
  "FISH_ON", 
  "DIGGING",
  "DIG_ON",
  "FIRE_ON",
  "MAGE",
  "CLERIC",
  "MONK",
  "BARD",
  "BEGGAR",
  "KNIGHT",
  "BADMORT",
  "\n"
};


/* MOB_x */
const char *action_bits[] = {
  "SPEC",
  "SENTINEL",
  "SCAVENGER",
  "ISNPC",
  "AWARE",
  "AGGR",
  "STAY-ZONE",
  "WIMPY",
  "AGGR_EVIL",
  "AGGR_GOOD",
  "AGGR_NEUTRAL",
  "MEMORY",
  "HELPER",
  "!CHARM",
  "!SUMMN",
  "!SLEEP",
  "!BASH",
  "!BLIND",
  "HUNTER_KILLER",
  "USE_SPELLS",
  "SECOND_ATTACK",
  "THIRD_ATTACK",
  "FOURTH_ATTACK",
  "SENTRY",
  "MOUNTABLE",
  "QUEST",
  "PET",
  "\n"
};


/* PRF_x */
const char *preference_bits[] = {
  "BRIEF",
  "COMPACT",
  "DEAF",
  "!TELL",
  "D_HP",
  "DETECT",
  "D_MOVE",
  "AUTOEX",
  "!HASS",
  "BANDAGED",
  "!BANDAGE",
  "!REP",
  "LIGHT",
  "C1",
  "C2",
  "!WIZ",
  "L1",
  "L2",
  "NOTSELF",
  "DISGUISE",
  "HUNTED",
  "RMFLG",
  "&RAFK&n",
  "UNUSED_7",
  "QUEST",
  "LEVEL_FLAGS",
  "AFK",
  "UNUSED_7",
  "QUEST",
  "LEVEL_FLAGS",
  "INTRO",
  "NO_OOC",
  "NO_NEWBIE",
  "NO_CHAT",
  "TIRED",
  "AUTO-CONSIDER",
  "\n"
};


/* AFF_x */
const char *affected_msg[] =
{
  "You are blind.",
  "You are invisible.",
  "You are aware of others alignment.",
  "You are aware of the presense of invisible things.",
  "You are sensitive of magical items.",
  "You have a keen sense of others life forces.",
  "You can walk upon water.",
  "You feel protected by sanctuary.",
  "AFF_GROUP",
  "You are cursed.",
  "Your eyes glow red.",
  "You have been poisoned.",
  "You feel protected from evil.",
  "You feel protected from good.",
  "You are feeling very sleepy.",
  "You step lightly, leaving no trail.",
  "You feel light enough to fly.",
  "Your movements are substantically quicker.",
  "You are moving quietly.",
  "You are hidden from sight.",
  "You are a solid block of ice.",
  "You have been charmed.",
  "You feel beastly.",
  "Your eyes can see through illusions.",
  "You are able to breathe under water.",
  "Your skin is covered in a hard shell.",
  "You feel protected from the heat.",
  "You feel protected from the cold.",
  "You are surrounded by a magical vestment.",
  "Your alignment is concealed.",
  "\n"
};


/* AFF_x */
const char *affected_bits[] =
{
  "BLIND",
  "INVIS",
  "DET-ALIGN",
  "DET-INVIS",
  "DET-MAGIC",
  "SENSE-LIFE",
  "WATWALK",
  "SANCT",
  "GROUP",
  "CURSE",
  "INFRA",
  "POISON",
  "PROT-EVIL",
  "PROT-GOOD",
  "SLEEP",
  "!TRACK",
  "FLY",
  "HASTE",
  "SNEAK",
  "HIDE",
  "DEFENSIVE-STANCE",
  "CHARM",
  "POLYMORPH",
  "TIRED",
  "WATERBREATH",
  "STONESKIN",
  "DETECT DISGUISE",
  "KNOCKOUT",
  "MAGIC VEST",
  "CONCEALED ALIGN",
  "TAMED",
  "JAWSTRIKE",
  "\n"
};


/* MOB  SPELL_FLAGS_x */
const char *spell_bits_mage[] =
{
  "Magic Missile",
  "Chill Touch",
  "Burning Hands",
  "Shocking Grasp",
  "Lightning Bolt",
  "Fireball",
  "Chain Lightning",
  "Cone of Cold",
  "Prismatic Spray",
  "Energy Drain",
  "Meteor Swarm",
  "Shield",
  "Blur",
  "Mirror Image",
  "Haste",
  "Minor Globe",
  "Stoneskin",
  "Major Globe",
  "Blink",
  "Fire Shield",
  "Spell Turning",
  "Blindness",
  "Weakness",
  "Silence",
  "Slow",
  "Hold Person",
  "Magical Suscep",
  "Blackmantle",
  "\n"
};

const char *spell_bits_cleric[] =
{
  "Cure Light",
  "Cure Moderate",
  "Cure Severe",
  "Cure Critic",
  "Heal",
  "Bless",
  "Armor",
  "Minor Sanctuary",
  "Sanctuary",
  "Prot From Elements",
  "Impervious Mind",
  "Cause Light",
  "Cause Moderate",
  "Dispel Good",
  "Dispel Evil",
  "Cause Severe",
  "Flamestrike",
  "Cause Critic",
  "Harm",
  "Harmful Wrath",
  "Blindness",
  "Deafening Wind",
  "Bestow Curse",
  "Silence",
  "Disease",
  "Paralyze",
  "Soul Drain",
  "Wither",
  "\n"
};

const char *spell_bits_druid[] =
{
  "Cure Light",
  "Cure Moderate",
  "Cure Severe",
  "Thornflesh",
  "Barkskin",
  "Stoneskin",
  "Thornshield",
  "Mystic Spirit",
  "Aid",
  "Faerie Fire",
  "Entangle",
  "Magical Stone",
  "Call Lightning",
  "Insect Plague",
  "Rainbow",
  "Storm Summoning",
  "Dehydration",
  "Moonbeam",
  "Sunray",
  "Elemental Storm",
  "Creeping Doom",
  "Call Animal Spirit",
  "Animal Summoning",
  "Summon Insects",
  "Animal Summoning II",
  "Sticks to Snakes",
  "Conjure Elemental",
  "Animal Summoning III",
  "\n"
};

const char *spell_bits_ranger[] =
{
  "Thornflesh",
  "Barkskin",
  "Aid",
  "Mystic Spirit",
  "Faerie Fire",
  "Entangle",
  "Magical Stone",
  "Flamestrike",
  "Call Lightning",
  "Call Animal Spirit",
  "Animal Summoning",  
  "Summon Insects",
  "Animal Summoning II",
  "Insect Plague",
  "\n"
};

const char *spell_bits_paladin[] =
{
  "Cure Light",
  "Cure Moderate",
  "Cure Severe",
  "Armor",
  "Bless",
  "Protection from Good",
  "Protection from Evil",
  "Minor Sanctuary",
  "Silence",
  "Dispel Good",
  "Dispel Evil",
  "Flamestrike",
  "Holy Strength",
  "Weakness",
  "\n"
};

const int mob_spells_cleric[] =
{
  SPELL_CURE_LIGHT,
  SPELL_CURE_MODERATE,
  SPELL_CURE_SEVERE,
  SPELL_CURE_CRITIC,
  SPELL_HEAL,
  SPELL_BLESS,
  SPELL_ARMOR,
  SPELL_MINOR_SANCTUARY,
  SPELL_SANCTUARY,
  SPELL_PROT_FROM_ELEMENTS,
  SPELL_IMPERVIOUS_MIND,
  SPELL_CAUSE_LIGHT,
  SPELL_CAUSE_MODERATE,
  SPELL_DISPEL_GOOD,
  SPELL_DISPEL_EVIL,
  SPELL_CAUSE_SEVERE,
  SPELL_FLAMESTRIKE,
  SPELL_CAUSE_CRITIC,
  SPELL_HARM,
  SPELL_HARMFUL_WRATH,
  SPELL_BLINDNESS,
  SPELL_DEAFENING_WIND,
  SPELL_BESTOW_CURSE,
  SPELL_SILENCE,
  SPELL_DISEASE,
  SPELL_PARALYZE,
  SPELL_SOUL_DRAIN,
  SPELL_WITHER,
  0
};

const int mob_spells_mage[] =
{
  SPELL_MAGIC_MISSILE,
  SPELL_CHILL_TOUCH,
  SPELL_BURNING_HANDS,
  SPELL_SHOCKING_GRASP,
  SPELL_LIGHTNING_BOLT,
  SPELL_FIREBALL,
  SPELL_CHAIN_LIGHTNING,
  SPELL_CONE_OF_COLD,
  SPELL_PRISMATIC_SPRAY,
  SPELL_ENERGY_DRAIN,
  SPELL_METEOR_SWARM,
  SPELL_SHIELD,
  SPELL_BLUR,
  SPELL_MIRROR_IMAGE,
  SPELL_HASTE,
  SPELL_MINOR_GLOBE,
  SPELL_STONESKIN,
  SPELL_MAJOR_GLOBE,
  SPELL_BLINK,
  SPELL_FIRE_SHIELD,
  SPELL_SPELL_TURNING,
  SPELL_BLINDNESS,
  SPELL_WEAKNESS,
  SPELL_SILENCE,
  SPELL_SLOW,
  SPELL_HOLD_PERSON,
  SPELL_MAGICAL_SUSCEPT,
  SPELL_BLACKMANTLE,
  0
};

const int mob_spells_druid[] =
{
  SPELL_CURE_LIGHT,
  SPELL_CURE_MODERATE,
  SPELL_CURE_SEVERE,
  SPELL_THORNFLESH,
  SPELL_BARKSKIN,
  SPELL_STONESKIN,
  SPELL_THORNSHIELD,
  SPELL_MYSTIC_SPIRIT,
  SPELL_AID,
  SPELL_FAERIE_FIRE,
  SPELL_ENTANGLE,
  SPELL_MAGICAL_STONE,
  SPELL_CALL_LIGHTNING,
  SPELL_INSECT_PLAGUE,
  SPELL_RAINBOW,
  SPELL_STORM_SUMMONING,
  SPELL_DEHYDRATION,
  SPELL_MOONBEAM,
  SPELL_SUNRAY,
  SPELL_ELEMENTAL_STORM,
  SPELL_CREEPING_DOOM,
  SPELL_CALL_ANIMAL_SPIRIT,
  SPELL_ANIMAL_SUMMONING,
  SPELL_SUMMON_INSECTS,
  SPELL_ANIMAL_SUMMONING_II,
  SPELL_STICKS_TO_SNAKES,
  SPELL_CONJURE_ELEMENTAL,
  SPELL_ANIMAL_SUMMONING_III,
  0
};

const int mob_spells_ranger[] =
{
  SPELL_THORNFLESH,
  SPELL_BARKSKIN,
  SPELL_AID,
  SPELL_MYSTIC_SPIRIT,
  SPELL_FAERIE_FIRE,
  SPELL_ENTANGLE,
  SPELL_MAGICAL_STONE,
  SPELL_FLAMESTRIKE,
  SPELL_CALL_LIGHTNING,
  SPELL_CALL_ANIMAL_SPIRIT,
  SPELL_ANIMAL_SUMMONING,
  SPELL_SUMMON_INSECTS,
  SPELL_ANIMAL_SUMMONING_II,
  SPELL_INSECT_PLAGUE,
  0
};

const int mob_spells_paladin[] =
{
  SPELL_CURE_LIGHT,
  SPELL_CURE_MODERATE,
  SPELL_CURE_SEVERE,
  SPELL_ARMOR,
  SPELL_BLESS,
  SPELL_PROT_FROM_GOOD,
  SPELL_PROT_FROM_EVIL,
  SPELL_MINOR_SANCTUARY,
  SPELL_SILENCE,
  SPELL_DISPEL_GOOD,
  SPELL_DISPEL_EVIL,
  SPELL_FLAMESTRIKE,
  SPELL_STRENGTH,
  SPELL_WEAKNESS,
  0
};

/* CON_x */
const char *connected_types[] = {
  "Playing",
  "Disconnecting",
  "Get name",
  "Confirm name",
  "Get password",
  "Get new PW",
  "Confirm new PW",
  "Select sex",
  "Select class",
  "Reading MOTD",
  "Main Menu",
  "Get descript.",
  "Changing PW 1",
  "Changing PW 2",
  "Changing PW 3",
  "Self-Delete 1",
  "Self-Delete 2",
  "Object edit",
  "Room edit",
  "Zone edit",
  "Mobile edit",
  "Shop edit",
  "Trigger Edit",
  "Select race",
  "Select ANSI",
  "Select Short Desc",
  "Select One Line Desc",
  "Select Long Desc",
  "Select Ability",
  "Select Alias",
  "Char Done",
  "Select Align",
  "Select God",
  "Gen Desc Intro",
  "Gen Desc Desc 1",
  "Gen Desc Desc 2",
  "Gen Desc Adj. 1",
  "Gen Desc Adj. 2",
  "Gen Desc Menu 1",
  "Gen Desc Menu 2",
  "\n"
};

const char *where[] = {
  "<used as light>      ",
  "<worn on finger>     ",
  "<worn on finger>     ",
  "<worn around neck>   ",
  "<worn around neck>   ",
  "<worn on body>       ",
  "<worn on head>       ",
  "<worn on legs>       ",
  "<worn on feet>       ",
  "<worn on hands>      ",
  "<worn on arms>       ",
  "<worn as shield>     ",
  "<worn about body>    ",
  "<worn about waist>   ",
  "<worn around wrist>  ",
  "<worn around wrist>  ",
  "<wielded>            ",
  "<held>               ",
  "<worn on face>       ",
  "<worn on ear>        ",
  "<worn on ear>        ",
  "<worn on ankle>      ",
  "<worn on ankle>      ",
  "<floating above head>",
  "<worn on back>       ",
  "<worn on shoulders>  ",
  "<worn on nose>       ",
  "<hanging from belt>  ",
  "<carried on back>    ",
  "<carried on back>    ",
  "<carried on back>    ",
  "<worn about waist>   ",
  "<sheathed on hand>   ",
  "<sheathed on back>   ",
  "<sheathed on back>   ",
  "<sheathed on waist>  ",
  "<sheathed on waist>  ",
  "<sheathed on ankle>  ",
  "<sheathed on ankle>  ",
  "<sheathed on wrist>  ",
  "<sheathed on wrist>  "
};


/* WEAR_x - for stat */
const char *equipment_types[] = {
    "Used as light",
    "Worn on finger",
    "Worn on finger",
    "Worn around neck",
    "Worn around neck",
    "Worn on body",
    "Worn on head",
    "Worn on legs",
    "Worn on feet",
    "Worn on hands",
    "Worn on arms",
    "Worn as shield",
    "Worn about body",
    "Worn about waist",
    "Worn around wrist",
    "Worn around wrist",
    "Wielded",
    "Held",
    "Worn on face",
    "Worn on ear",
    "Worn on ear",
    "Worn on ankle",
    "Worn on ankle",
    "Floating above head",
    "Worn on back",
    "Worn on shoulders",
    "Worn on nose",
    "Hanging from belt",
    "Carried on back",
    "Carried on back",
    "Carried on back",
    "Worn about waist",
    "Sheathed on hand",
    "Sheathed on back",
    "Sheathed on back",
    "Sheathed on waist",
    "Sheathed on waist",
    "Sheathed on ankle",
    "Sheathed on ankle",
    "Sheathed on wrist",
    "Sheathed on wrist",
    "\n"
};
const int wear_order_index[NUM_WEARS] = {
 WEAR_LIGHT,
 WEAR_ABOVE,
 WEAR_HEAD,
 WEAR_FACE,
 WEAR_NOSE,
 WEAR_EAR_R,
 WEAR_EAR_L,
 WEAR_NECK_1,
 WEAR_NECK_2,
 WEAR_SHOULDER,
 WEAR_BODY,
 WEAR_ABOUT,
 WEAR_ARMS,
 WEAR_WRIST_R,
 WEAR_WRIST_L,
 WEAR_SHEATHED_WR1,
 WEAR_SHEATHED_WR2,
 WEAR_HANDS,
 WEAR_SHEATHED_H,
 WEAR_FINGER_R,
 WEAR_FINGER_L,
 WEAR_SHIELD,
 WEAR_WIELD,
 WEAR_HOLD,
 WEAR_WAIST_1,
 WEAR_WAIST_2,
 WEAR_SHEATHED_WA1,
 WEAR_SHEATHED_WA2,
 WEAR_ONBELT,
 WEAR_LEGS,
 WEAR_ANKLE_R,
 WEAR_ANKLE_L,
 WEAR_SHEATHED_A1,
 WEAR_SHEATHED_A2,
 WEAR_FEET,
 WEAR_ONBACK_1,
 WEAR_ONBACK_2,
 WEAR_ONBACK_3,
 WEAR_BACK_1,
 WEAR_SHEATHED_B1,
 WEAR_SHEATHED_B2,
};

/* ITEM_x (ordinal object types) */
const char *item_types[] = {
  "UNDEFINED",
  "LIGHT",
  "SCROLL",
  "WAND",
  "STAFF",
  "WEAPON",
  "SALVE",
  "BELT",
  "TREASURE",
  "ARMOR",
  "POTION",
  "WORN",
  "OTHER",
  "TRASH",
  "BOTTLE",
  "CONTAINER",
  "NOTE",
  "LIQ CONTAINER",
  "KEY",
  "FOOD",
  "MONEY",
  "PEN",
  "SHOVEL",
  "FOUNTAIN",
  "FIRE",
  "AQUALUNG",
  "SHEATH",
  "RAW",
  "PORTAL",
  "FISHING POLE", 
  "FIREWOOD",
   "\n"
};


/* ITEM_WEAR_ (wear bitvector) */
const char *wear_bits[] = {
  "TAKE",
  "FINGER",
  "NECK",
  "BODY",
  "HEAD",
  "LEGS",
  "FEET",
  "HANDS",
  "ARMS",
  "SHIELD",
  "ABOUT",
  "WAIST",
  "WRIST",
  "WIELD",
  "HOLD",
  "FACE",
  "EAR",
  "ANKLE",
  "ABOVE",
  "BACK",
  "SHOULDER",
  "NOSE",
  "SPARE_DONTUSE",
  "SHEATHED_B",
  "ONBELT",
  "ONBACK",
  "SHEATHED_WA",
  "SHEATHED_WR",
  "SHEATHED_A",
  "SHEATHED_H",
  "\n"
};


/* ITEM_x (extra bits) */
const char *extra_bits[] = {
  "GLOW",
  "HIDDEN",
  "!RENT",
  "!REMOVE",
  "!INVIS",
  "INVISIBLE",
  "MAGIC",
  "!DROP",
  "BLESS",
  "!GOOD",
  "!EVIL",
  "!NEUTRAL",
  "!MAGE",
  "!CLERIC",
  "!ROGUE",
  "!WARRIOR",
  "!SELL",

  /*  PDH  2/18/99 - added new race and class OBJ restrictions  */
  "!KNIGHT",
  "!PALADIN",
  "!RANGER",
  "+MONK",
  "!DRUID",

  "!BARBAR",
  "!DWARF",
  "!ELF",
  "!HALFELF",
  "!KENDER",
  "!MINOTAUR",
  "!GNOME",
  "!HUMAN",
  "TWO_HANDED",
  "UNIQUE",
  "\n"
};


/* APPLY_x */
const char *apply_types[] = {
  "NONE",
  "STR",
  "DEX",
  "INT",
  "WIS",
  "CON",
  "CHA",
  "CLASS",
  "LEVEL",
  "AGE",
  "CHAR_WEIGHT",
  "CHAR_HEIGHT",
  "UNDEFINED",
  "MAXHIT",
  "MAXMOVE",
  "GOLD",
  "EXP",
  "ARMOR",
  "HITROLL",
  "DAMROLL",
  "SAVING_PARA",
  "SAVING_ROD",
  "SAVING_PETRI",
  "SAVING_BREATH",
  "SAVING_SPELL",
  "QUESTPOINTS",
  "LIGHT",
  "SILENCE",
  "\n"
};


/* CONT_x */
const char *container_bits[] = {
  "CLOSEABLE",
  "PICKPROOF",
  "CLOSED",
  "LOCKED",
  "\n",
};


/* LIQ_x */
const char *drinks[] =
{
  "water",
  "beer",
  "wine",
  "ale",
  "dark ale",
  "whiskey",
  "lemonade",
  "firebreather",
  "local speciality",
  "slime mold juice",
  "milk",
  "tea",
  "coffee",
  "blood",
  "salt water",
  "clear water",
  "\n"
};

/* other constants for liquids */


/* one-word alias for each drink */
const char *drinknames[] =
{
  "water",
  "beer",
  "wine",
  "ale",
  "ale",
  "whiskey",
  "lemonade",
  "firebreather",
  "local",
  "juice",
  "milk",
  "tea",
  "coffee",
  "blood",
  "salt",
  "water",
  "\n"
};


/* effect of drinks on hunger, thirst, and drunkenness -- see values.doc */
const int drink_aff[][3] = {
  {0, 0, 13},
  {3, 2, 5},
  {5, 2, 5},
  {2, 2, 5},
  {1, 2, 5},
  {6, 1, 4},
  {0, 1, 8},
  {10, 0, 0},
  {3, 3, 3},
  {0, 4, -8},
  {0, 3, 6},
  {0, 1, 6},
  {0, 1, 6},
  {0, 2, -1},
  {0, 1, -2},
  {0, 0, 13}
};


/* color of the various drinks */
const char *color_liquid[] =
{
  "clear",
  "brown",
  "clear",
  "brown",
  "dark",
  "golden",
  "red",
  "green",
  "clear",
  "light green",
  "white",
  "brown",
  "black",
  "red",
  "clear",
  "crystal clear"
  "\n"
};


/* level of fullness for drink containers Not used in sprinttype() so no \n. */
const char *fullness[] =
{
  "less than half ",
  "about half ",
  "more than half ",
  ""
};


/* str, int, wis, dex, con applies **************************************/


/* [ch] strength apply (all) */
const struct str_app_type str_app[] = {
  {-5, -4, 0, 0},	/* str = 0 */
  {-5, -4, 3, 1},	/* str = 1 */
  {-3, -2, 3, 2},
  {-3, -1, 10, 3},
  {-2, -1, 25, 4},
  {-2, -1, 55, 5},	/* str = 5 */
  {-1, 0, 80, 6},
  {-1, 0, 90, 7},
  {0, 0, 100, 8},
  {0, 0, 100, 9},
  {0, 0, 115, 10},	/* str = 10 */
  {0, 0, 115, 11},
  {0, 0, 140, 12},
  {0, 0, 140, 13},
  {0, 0, 170, 14},
  {0, 0, 170, 15},	/* str = 15 */
  {0, 1, 195, 16},
  {1, 1, 220, 18},
  {1, 2, 255, 20},	/* dex = 18 */
  {3, 7, 640, 40},
  {3, 8, 700, 40},	/* str = 20 */
  {4, 9, 810, 40},
  {4, 10, 970, 40},
  {5, 11, 1130, 40},
  {6, 12, 1440, 40},
  {7, 14, 1750, 40},	/* str = 25 */
  {1, 3, 280, 22},	/* str = 18/0 - 18-50 */
  {2, 3, 305, 24},	/* str = 18/51 - 18-75 */
  {2, 4, 330, 26},	/* str = 18/76 - 18-90 */
  {2, 5, 380, 28},	/* str = 18/91 - 18-99 */
  {3, 6, 480, 30}	/* str = 18/100 */
};



/* [dex] skill apply (thieves only) */
const struct dex_skill_type dex_app_skill[] = {
  {-99, -99, -90, -99, -60},	/* dex = 0 */
  {-90, -90, -60, -90, -50},	/* dex = 1 */
  {-80, -80, -40, -80, -45},
  {-70, -70, -30, -70, -40},
  {-60, -60, -30, -60, -35},
  {-50, -50, -20, -50, -30},	/* dex = 5 */
  {-40, -40, -20, -40, -25},
  {-30, -30, -15, -30, -20},
  {-20, -20, -15, -20, -15},
  {-15, -10, -10, -20, -10},
  {-10, -5, -10, -15, -5},	/* dex = 10 */
  {-5, 0, -5, -10, 0},
  {0, 0, 0, -5, 0},
  {0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0},		/* dex = 15 */
  {0, 5, 0, 0, 0},
  {5, 10, 0, 5, 5},
  {10, 15, 5, 10, 10},		/* dex = 18 */
  {15, 20, 10, 15, 15},
  {15, 20, 10, 15, 15},		/* dex = 20 */
  {20, 25, 10, 15, 20},
  {20, 25, 15, 20, 20},
  {25, 25, 15, 20, 20},
  {25, 30, 15, 25, 25},
  {25, 30, 15, 25, 25}		/* dex = 25 */
};



/* [dex] apply (all) */
struct dex_app_type dex_app[] = {
  {-7, -7, 6},		/* dex = 0 */
  {-6, -6, 5},		/* dex = 1 */
  {-4, -4, 5},
  {-3, -3, 4},
  {-2, -2, 3},
  {-1, -1, 2},		/* dex = 5 */
  {0, 0, 1},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},		/* dex = 10 */
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, -1},		/* dex = 15 */
  {1, 1, -2},
  {2, 2, -3},
  {2, 2, -4},		/* dex = 18 */
  {3, 3, -4},
  {3, 3, -4},		/* dex = 20 */
  {4, 4, -5},
  {4, 4, -5},
  {4, 4, -5},
  {5, 5, -6},
  {5, 5, -6}		/* dex = 25 */
};



/* [con] apply (all) */
struct con_app_type con_app[] = {
  {-4, 20},		/* con = 0 */
  {-3, 25},		/* con = 1 */
  {-2, 30},
  {-2, 35},
  {-1, 40},
  {-1, 45},		/* con = 5 */
  {-1, 50},
  {0, 55},
  {0, 60},
  {0, 65},
  {0, 70},		/* con = 10 */
  {0, 75},
  {0, 80},
  {0, 85},
  {0, 88},
  {1, 90},		/* con = 15 */
  {2, 95},
  {2, 97},
  {3, 99},		/* con = 18 */
  {3, 99},
  {4, 99},		/* con = 20 */
  {5, 99},
  {5, 99},
  {5, 99},
  {6, 99},
  {6, 99}		/* con = 25 */
};



/* [int] apply (all) */
struct int_app_type int_app[] = {
  {1},		/* int = 0 */
  {2},		/* int = 1 */
  {3},
  {3},
  {4},
  {5},		/* int = 5 */
  {6},
  {6},
  {7},
  {8},
  {9},		/* int = 10 */
  {10},
  {11},
  {12},
  {15},
  {17},		/* int = 15 */
  {20},
  {23},
  {25},		/* int = 18 */
  {26},
  {27},		/* int = 20 */
  {28},
  {29},
  {30},
  {31},
  {32}		/* int = 25 */
};


/* [wis] apply (all) */
struct wis_app_type wis_app[] = {
  {0},	/* wis = 0 */
  {0},  /* wis = 1 */
  {0},
  {0},
  {1},
  {2},  /* wis = 5 */
  {3},
  {4},
  {5},
  {6},
  {6},  /* wis = 10 */
  {7},
  {8},
  {8},
  {9},
  {9},  /* wis = 15 */
  {10},
  {11},
  {11},	/* wis = 18 */
  {12},
  {13},  /* wis = 20 */
  {14},
  {14},
  {15},
  {16},
  {16}  /* wis = 25 */
};



const char *spell_wear_off_msg[] = {
  "RESERVED DB.C",		/* 0 */
  "You feel less protected.",	/* 1 */
  "!Teleport!",
  "You feel less righteous.",
  "The cloak of blindness dissolves.",
  "!Burning Hands!",		/* 5 */
  "!Call Lightning!",
  "The charm spell dissipates.",
  "You feel your strength return.",
  "Your body gradually comes back into focus.",
  "!Mass Charm!",		/* 10 */
  "The thorns on your skin disappear.",
  "!Create Food!",
  "!Create Water!",
  "!Cure Blind!",
  "!Cure Critic!",		/* 15 */
  "!Cure Light!",
  "You feel less cursed.",
  "You feel less aware.",
  "Your eyes stop tingling.",
  "The detect magic wears off.",/* 20 */
  "The detect poison wears off.",
  "!Dispel Evil!",
  "The pain in your legs subsides.",
  "!Enchant Weapon!",
  "!Energy Drain!",		/* 25 */
  "!Fireball!",
  "!Harm!",
  "!Heal!",
  "The invisibility spell wears off.",
  "!Lightning Bolt!",		/* 30 */
  "!Locate Object!",
  "!Magic Missile!",
  "The poison in your blood thins.",
  "You feel less protected from evil.",
  "!Remove Curse!",		/* 35 */
  "The white aura around your body fades.",
  "!Shocking Grasp!",
  "You feel less tired.",
  "You feel weaker.",
  "!Summon!",			/* 40 */
  "The shield surrounding you dissipates.",
  "!Word of Recall!",
  "!Neutralize Poison!",
  "You feel less aware of your surroundings.",
  "!Legend Lore!",		/* 45 */
  "!Dispel Good!",
  "The white aura around your body fades.",
  "!Healing Light!",
  "The sounds of prayer leave your ears.",  
  "Your night vision seems to fade.",	/* 50 */
  "You feel less capable of walking on water.",
  "Your skin loses its bark-like texture.",
  "Your skin begins to soften.",
  "!Harmful Wrath!",
  "!Cure Deafness!",            /* 55 */
  "Your body reverts to normal.",
  "A mirror image of you flickers and dies.",
  "Your feet come back to the ground.",
  "You feel a bit stronger now.",
  "The wind filling your ears fades away.", /* 60 */
  "!Cure Disease!",
  "!Remove Paralysis!",
  "!Restoration!",
  "!Regeneration!",
  "Your movements slow.",	/* 65 */
  "The disease infecting you fades away.",
  "You seem to be able to move again.",
  "Your spirit seems to seep back into your body.",
  "!Wither!",
  "Your body begins to respond as the slow spell wears off.",  /* 70 */
  "!Gate!",
  "!Elemental Storm!",
  "!Create Spring!",
  "You feel less protected from good.",
  "You feel less protected from the undead.", /* 75 */
  "You feel vulnerable to fire.",
  "You feel vulnerable to frost.",
  "You feel vulnerable to the elements.",
  "You feel vulnerable to mental assault.",
  "!Calm!",			/* 80 */
  "The cloak of fear shrouding you fades away.", 
  "You feel braver as the magical fear dissipates.",
  "!Heroes Feast!",
  "The magically-inspired bravery fades.",
  "!Greater Elemental!",	/* 85 */
  "!Unused!",
  "The barrier of blades dissipates.",
  "The faerie fire lining your body disappears.",
  "!Sunray!",
  "!Moonbeam!", 		/* 90 */
  "!Dispel Magic!",
  "The aura of light around you fades.",
  "The blanket of darkness flowing from you dissipates.",
  "Your vocal cords loosen up.",
  "Your vocal cords loosen up.", /* 95 */
  "!Cure Moderate!",
  "!Cure Severe!",
  "!Cause Light!",
  "!Cause Moderate!",
  "!Cause Severe!", 		/* 100 */
  "!Cause Critical!",
  "!Goodberry!",
  "You sense the magical armor wearing off.",
  "The feeling of weightlessness fades.",
  "!Magical Stone!", 		/* 105 */
  "|Shillelagh!",
  "Your floating hammer vanishes.",
  "!Flamestrike!",
  "You feel less aided.",
  "!Call Animal Spirit!", 	/* 110 */
  "The swarm of insects dissipates.",
  "!Animal Summoning!",
  "!Animal Summoning II!",
  "!Animal Summoning III!",
  "The creeping mass of insects disperses.", /* 115 */
  "The plague of insects drifts apart.",
  "!Rainbow!",
  "The vines and roots release their iron grip on you.",
  "The shield of thorns protecting you falls apart.",
  "!Sticks to Snakes!", 	/* 120 */
  "!Aerial Servant!",
  "!Summon Guard!",
  "!Dust Devil!",
  "The blade of flame slowly fades away.",
  "You feel less sure of your ability to breathe underwater.", /* 125 */
  "!Conjure Elemental!",
  "!Wind Walk!",
  "Your phantasmal snake vanishes.",
  "!Identify!",
  "You feel less resistant to plague.",  /* 130 */
  "Your body has finally purged itself of the deadly plague.",
  "!Reflecting Pool!",
  "The spell of recollection ends suddenly.",
  "!Remove Fear!",
  "The divine wrath fades away.",   /* 135 */
  "You feel the effects of the holy word wear off.",
  "The spirits leave your body.",
  "The power of the summoned storm dissipates.",
  "The intense dehydration you have experienced seems to have ended.",
  "The ground beneath your feet reverts to normal, freeing you.",  /* 140 */
  "!Abjure!",
  "Your weapon reverts to its normal hardness as the spell fades.",
  "Your body begins to respond as the holding spell wears off.",
  "The webs encasing you crumble to dust.",
  "!Gust of Wind!",		/* 145 */
  "The globe of invulnerability surrounding you fades away.",
  "The globe of invulnerability surrounding you fades away.",
  "!Vampiric Touch!",
  "The dancing sword vaporizes.",
  "!Scry!",			/* 150 */
  "!Chain Lightning!",
  "!Teleport Object!",
  "!Arrow of Bone!",
  "!Cone of Cold!",
  "The fiery shield surrounding you flickers and dies.", /* 155 */
  "The black aura encasing you vanishes.",
  "The effects of the blink spell wear off.",
  "!Prismatic Spray!",
  "!Meteor Swarm!",
  "You feel less susceptable to magical assault.",	/* 160 */
  "The aura of antimagic surrounding you vanishes.",
  "One of your images flickers and dies.",
  "Your aura of spell turning vanishes, leaving you vulnerable.",
  "!Recharge Item!",
  "Your vampiric aura seeps back into your body.",	/* 165 */
  "!Refresh!",
  "!Power Word Kill!",
  "You feel like a curtain has been pulled open.",
  "!Polymorph!",
  "!Reveal Illusion!",	/* 170 */
  "You feel less agile.",
  "You feel less healthy  and energetic.",
  "You feel less insightful.",
  "You feel dumber.",
  "You feel less socially adept.",  /* 175 */
  "!Blessing of Kings!",
  "The effects on your morale from the knightly challenge have worn off.", 
  "!UNUSED!", "!UNUSED!", "!UNUSED!",  /* 180 */
  "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!",  /* 185 */
  "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!",  /* 190 */
  "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!",  /* 195 */
  "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!",  /* 200 */
  "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!",  /* 205 */
  "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!",  /* 210 */
  "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!",  /* 215 */
  "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!",  /* 220 */
  "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!",  /* 225 */
  "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!",  /* 230 */
  "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!",  /* 235 */
  "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!",  /* 240 */
  "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!",  /* 245 */
  "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!"   /* 250 */
};



const char *npc_class_types[] = {
  "Undefined",
  "Magi",
  "Priest",
  "Warrior",
  "Rogue",
  "Druid",
  "Ranger",
  "Paladin",
  "Undead Mage",
  "Undead Cleric",
  "Undead Fighter",
  "\n"
};

const char *pc_pos_types[] = {
  "dead",
  "mortallyw",
  "incap",
  "stunned",
  "sleeping",
  "resting",
  "sitting",
  "fighting",
  "standing",
  "digging",
  "fishing",
  "riding",
  "\n"
};


/*  PDH  1/14/99
 *  moved this definition out of the middle of the
 *  following rev_dir[] struct
 *
 *  why the hell was it in there to begin with?
 */

#if defined(OASIS_MPROG)
/*
 * Definitions necessary for MobProg support in OasisOLC
 */
const char *mobprog_types[] = {
  "INFILE",
  "ACT",
  "SPEECH",
  "RAND",
  "FIGHT",
  "DEATH",
  "HITPRCNT",
  "ENTRY",
  "GREET",
  "ALL_GREET",
  "GIVE",
  "BRIBE",
  "\n"
};
#endif


const int rev_dir[] =
{
  2,
  3,
  0,
  1,
  5,
  4,
  9,
  8,
  7,
  6
};


const int movement_loss[] =
{
  1,	/* Inside     */
  1,	/* City       */
  2,	/* Field      */
  3,	/* Forest     */
  4,	/* Hills      */
  6,	/* Mountains  */
  4,	/* Swimming   */
  1,	/* Unswimable */
  1,	/* Flying     */
  5     /* Underwater */
};

/* Not used in sprinttype(). */
const char *weekdays[] = {
  "the Day of Gilean",
  "the Day of Lunitari",
  "the Day of Nuitari",
  "the Day of Solinari",
  "the Day of Majere",
  "the day of Shinare",
"the Day of Zivilyn"};

/* Not used in sprinttype(). */
const char *month_name[] = {
  "Month of Chemosh",		/* 0 */
  "Month of Zeboim",
  "Month of Mishakal",
  "Month of Chislev",
  "Month of Branchala",
  "Month of Kiri-Jolith",
  "Month of Sargonnas",
  "Month of Sirrion",
  "Month of Reorx",
  "Month of Hiddukel",
  "Month of Morgion",
  "Month of Habbakuk",
  "Month of the Dark Shades",
  "Month of the Shadows",
  "Month of the Long Shadows",
  "Month of the Ancient Darkness",
  "Month of the Great Evil"
};


const int default_mob[LVL_IMPL + 1][mdefault_max] = {
  {      0, 20,  150,   1, 1, 0,     5, 1, 0},  /*  0  */
  {      7, 20,  150,   1, 4, 0,     1, 2, 5},
  {     13, 19,  140,   1, 5, 1,     1, 3, 12},
  {     25, 19,  130,   1, 6, 1,     1, 5, 25},
  {     50, 18,  120,   1, 8, 1,     1, 5, 45},
  {     85, 17,  110,   2, 5, 0,     2, 5, 65},  /*  5  */
  {    175, 17,  100,   2, 5, 2,     2, 5, 70},
  {    400, 16,   90,   2, 6, 1,     3, 5, 85},
  {    800, 15,   80,   2, 6, 2,     5, 5, 110},
  {   1300, 15,   70,   2, 7, 1,     5, 5, 130},
  {   1900, 14,   60,   2, 7, 2,     5, 4, 160},  /*  10  */
  {   2900, 14,   50,   2, 8, 2,     5, 4, 190},
  {   4700, 13,   40,   2, 8, 3,     5, 6, 210},
  {   6625, 13,   30,   2, 9, 2,     6, 5, 240},
  {   7000, 12,   20,   2, 8, 4,     6, 8, 240},
  {   8400, 11,   10,   2, 8, 4,     5,10, 300},  /*  15  */
  {  11000, 11,    0,   3, 6, 4,     3,20, 320},
  {  11800, 10,  -10,   3, 6, 5,     3,20, 340},
  {  14000, 11,  -20,   3, 6, 6,     3,20, 380},
  {  17000,  9,  -30,   3, 6, 6,     3,20, 410},
  {  25000,  8,  -40,   3, 7, 6,     3,20, 440},  /*  20  */
  {  33500,  8,  -50,   3, 8, 7,     3,33, 450},
  {  40000,  7,  -60,   3, 8, 7,     3,33, 550},
  {  52000,  6,  -70,   3, 8, 8,     3,33, 650},
  {  68000,  6,  -80,   4, 6, 8,     3,33, 750},
  {  87000,  5,  -90,   4, 6, 8,     3,33, 850},  /*  25  */
  { 115000,  4, -100,   4, 7, 7,     3,33, 950},
  { 135000,  3, -110,   4, 7, 8,     3,33,1100},
  { 175000,  2, -120,   4, 7, 9,     3,33,1250},
  { 213000,  2, -130,   4, 7, 9,     3,33,1450},
  { 250000,  1, -140,   4, 8, 9,     3,33,1700},  /*  30  */
  { 300000,  0, -150,   4, 8,10,     3,33,2000},
  { 310000,  0, -160,   4, 8,10,     3,33,2250},
  { 320000, -1, -170,   4, 8,11,     3,33,2600},
  { 345000, -1, -175,   4, 8,11,     3,33,2700},
  { 365000, -2, -180,   4, 8,12,     3,33,2800},  /*  35  */
  { 380000, -2, -185,   4, 8,14,     3,33,2900},
  { 400000, -3, -190,   4, 8,15,     3,33,3000},
  { 500000, -5, -200,   4, 8,16,    20,20,3000},  /*  38  */
};

const char *god_name[NUM_GODS + 1] = {
  "n/a",
  "Paladine",
  "Kiri-Jolith",
  "Habbakuk",
  "Mishakal",
  "Branchala",
  "Solinari",
  "Majere",
  "none", "none", "none",
  "Gilean",
  "Reorx",
  "Lunitari",
  "Chislev",
  "Sirrion",
  "Zivilyn",
  "Shinare",
  "none", "none", "none",
  "Takhisis",
  "Sargonnas",
  "Nuitari",
  "Morgion",
  "Chemosh",
  "Zeboim",
  "Hiddukel",
  "none", "none", "none",
};

const char *spec_procs[] = {
  "None",
  "Postmaster",
  "Guildmaster",
  "Innkeeper",
  "Banker",
  "Streetsweeper",
  "Thief",
  "City Guard",
  "Corpse Eater",
  "Snake",
  "Cleric",
  "Auto Newbie Equipper",
  "\n"
};

const char *room_specs[] = {
  "None",
  "Dump",
  "Auto Newbie Equipper",
  "\n"
};

/*  num_spell_levels, starting_spell_level, slotmod  */
const struct casting_type caster_info[NUM_CLASSES] =
{
  {0, 0, 0},    /* Knights */
  {4, 11, 35},  /* Paladins */
  {9, 1, 45},   /* Mages */
  {7, 1, 45},   /* Clerics */
  {4, 11, 35},  /* Rangers */
  {0, 0, 0},    /* Rogues */
  {0, 0, 0},    /* Warriors */
  {0, 0, 0},    /* undefined */
  {7, 1, 45}    /* Druids */
};

const int mem_apply[26] =
{
  -5, -5, -5,  /*  0-2  */
  -4, -4, -4,  /*  3-5  */
  -3, -3, -3,  /*  6-8  */
  -2, -2, -2,  /*  9-11 */
  -1, -1, -1,  /* 12-14 */
   0,  0,  0,  /* 15-17 */
   1,  1,  1,  /* 18-20 */
   2,  2,  2,  /* 21-23 */
   3,          /*   24  */
   4           /*   25  */
};

/* Taken out untili can do it right.
 * --gan, 07/21/2001 (Nuitari)

struct component_type spell_components[] =
{
//{ SPELL_MIRROR_IMAGE,         {{ 26, CMP_HOLD },
//                               { CMP_UNDEFINED, 0 }}},
  { SPELL_LOCATE_OBJECT,	{{ 27, CMP_INV },
                                 { CMP_UNDEFINED, 0 }}},
  { SPELL_SUMMON,               {{ 28, CMP_INV },
                                 { CMP_UNDEFINED, 0 }}},
  { SPELL_DANCING_SWORD,	{{ 29, CMP_INV },
                                 { CMP_UNDEFINED, 0 }}},
  { SPELL_BLINK,		{{ 30, CMP_HOLD },
                                 { CMP_UNDEFINED, 0 }}},
  { SPELL_FIRE_SHIELD,		{{ 31, CMP_INV },
                                 { CMP_UNDEFINED, 0 }}},
  { SPELL_ANTIMAGIC_AURA,	{{ 32, CMP_HOLD },
                                 { CMP_UNDEFINED, 0 }}},
  { SPELL_PRISMATIC_SPRAY,	{{ 33, CMP_INV },
                                 { CMP_UNDEFINED, 0 }}},
//{ SPELL_INVISIBILITY, 	{{ 34, CMP_HOLD | CMP_CONSUMED },
//                               { CMP_UNDEFINED, 0 }}},
  { SPELL_ENCHANT_WEAPON,	{{ 35, CMP_INV | CMP_CONSUMED },
                                 { CMP_UNDEFINED, 0 }}},
  { SPELL_IDENTIFY,		{{ 36, CMP_HOLD | CMP_CONSUMED },
                                 { CMP_UNDEFINED, 0 }}},
  { SPELL_HASTE,		{{ 37, CMP_INV | CMP_CONSUMED },
                                 { CMP_UNDEFINED, 0 }}},
  { SPELL_CHARM,		{{ 38, CMP_HOLD | CMP_CONSUMED },
                                 { CMP_UNDEFINED, 0 }}},
//{ SPELL_STONESKIN,		{{ 39, CMP_HOLD | CMP_CONSUMED },
//                               { CMP_UNDEFINED, 0 }}},
  { SPELL_SCRY,			{{ 27, CMP_INV | CMP_CONSUMED },
                                 { CMP_UNDEFINED, 0 }}},
  { SPELL_BLACKMANTLE,		{{ 40, CMP_HOLD | CMP_CONSUMED },
                                 { CMP_UNDEFINED, 0 }}},
  { SPELL_MAGICAL_SUSCEPT,	{{ 41, CMP_HOLD | CMP_CONSUMED },
                                 { CMP_UNDEFINED, 0 }}},
  { SPELL_DECEPTIVE_IMAGERY,	{{ 42, CMP_HOLD | CMP_CONSUMED },
                                 { CMP_UNDEFINED, 0 }}},
  { SPELL_LEGEND_LORE,		{{ 43, CMP_HOLD | CMP_CONSUMED },
                                 { CMP_UNDEFINED, 0 }}},
  { SPELL_MASS_CHARM,		{{ 44, CMP_INV | CMP_CONSUMED },
                                 { CMP_UNDEFINED, 0 }}},
  { SPELL_CRIPPLE,		{{ 45, CMP_HOLD | CMP_CONSUMED },
                                 { CMP_UNDEFINED, 0 }}},
  { SPELL_ENERGY_DRAIN,		{{ 46, CMP_INV | CMP_CONSUMED },
                                 { CMP_UNDEFINED, 0 }}},
  { SPELL_GATE,                 {{ 53, CMP_HOLD | CMP_CONSUMED },
				 {CMP_UNDEFINED, 0}}},
  { SPELL_GREATER_ELEMENTAL,	{{ 47, CMP_INV | CMP_CONSUMED },
                                 { CMP_UNDEFINED, 0 }}},
  { SPELL_PHANTASMAL_SNAKE,	{{ 48, CMP_INV | CMP_CONSUMED },
                                 { CMP_UNDEFINED, 0 }}},
  { SPELL_RECHARGE_ITEM,	{{ 49, CMP_INV | CMP_CONSUMED },
                                 { CMP_UNDEFINED, 0 }}},
  { SPELL_SPELL_TURNING,	{{ 50, CMP_HOLD | CMP_CONSUMED },
                                 { CMP_UNDEFINED, 0 }}},
  { SPELL_RECOLLECTION,		{{ 51, CMP_HOLD | CMP_CONSUMED },
                                 { CMP_UNDEFINED, 0 }}},
  { SPELL_VAMPIRIC_AURA,	{{ 52, CMP_INV | CMP_CONSUMED },
                                 { CMP_UNDEFINED, 0 }}},
  { 0, {{CMP_UNDEFINED, 0}, {CMP_UNDEFINED, 0}}}
};

*/

struct same_zone shout_zone[] =
{
  { "Kalaman",       {  98,  99, 100, 101,  -1,  -1 } }, 
  { "Karthay",       {  50,  51,  -1,  -1,  -1,  -1 } },
  { "Vingaard Keep", {  16,  17,  -1,  -1,  -1,  -1 } },
  { "Istar",         {  80,  81,  -1,  -1,  -1,  -1 } },
  { 0,               {  -1,  -1,  -1,  -1,  -1,  -1 } }
};

const int min_memtime[10] =
{
  0,  /* level 0 spells shouldn't exist anyway */
  /* 1, 1, 2, 2, 3, 3, 4, 4, 5 */
  3, 3, 6, 6, 9, 9, 12, 12, 15
};

const int start_memtime[10] =
{
  0,  /*  Again, irrelevant, but necessary to the array  */
  /* 2, 2, 3, 3, 4, 4, 5, 5, 6 */
  6, 6, 9, 9, 12, 12, 15, 15, 18
};

const struct prerequisites prereq[] = {
  {SPELL_DETECT_MAGIC, SKILL_READ_MAGIC},
  {SKILL_ASSESS_MAGIC, SPELL_DETECT_MAGIC},
  {SKILL_SCRIBE, SKILL_ASSESS_MAGIC},
  {SKILL_BREW, SKILL_ASSESS_MAGIC},
  {SKILL_GUARD, SKILL_RESCUE},
  {SKILL_THIRD_ATTACK, SKILL_SECOND_ATTACK},
  {SKILL_STEAL, SKILL_SNEAK},
  {SKILL_REMOVE_POISON, SKILL_DETECT_POISON},
  {SKILL_DISGUISE, SKILL_HIDE},
  {SPELL_CREATE_SPRING, SPELL_CREATE_WATER},
  {SPELL_CURE_MODERATE, SPELL_CURE_LIGHT},
  {SPELL_CAUSE_MODERATE, SPELL_CAUSE_LIGHT},
  {SPELL_DETECT_INVIS, SPELL_DETECT_MAGIC},
  {SPELL_MIRROR_IMAGE, SPELL_BLUR},
  {SPELL_ANIMAL_SUMMONING, SPELL_CALL_ANIMAL_SPIRIT},
  {SPELL_DISPEL_GOOD, SPELL_DETECT_ALIGN},
  {SPELL_DISPEL_EVIL, SPELL_DETECT_ALIGN},
  {SPELL_INVISIBILITY, SPELL_BLUR},
  {SPELL_CURE_SEVERE, SPELL_CURE_MODERATE},
  {SPELL_CAUSE_SEVERE, SPELL_CAUSE_MODERATE},
  {SPELL_CURE_DEAFNESS, SPELL_CURE_BLIND},
  {SPELL_DEAFENING_WIND, SPELL_BLINDNESS},
  {SPELL_SUMMON_INSECTS, SPELL_ANIMAL_SUMMONING},
  {SPELL_DISPEL_MAGIC, SPELL_DETECT_MAGIC},
  {SPELL_LIGHTNING_BOLT, SPELL_SHOCKING_GRASP},
  {SPELL_BARKSKIN, SPELL_ARMOR},
  {SPELL_PROT_FROM_EVIL, SPELL_DETECT_ALIGN},
  {SPELL_PROT_FROM_GOOD, SPELL_DETECT_ALIGN},
  {SPELL_WATER_BREATHING, SPELL_WATER_WALK},
  {SPELL_CURE_CRITIC, SPELL_CURE_SEVERE},
  {SPELL_CAUSE_CRITIC, SPELL_CAUSE_SEVERE},
  {SPELL_SLOW, SPELL_HASTE},
  {SPELL_MAGIC_VEST, SPELL_ARMOR},
  {SPELL_PRAYER, SPELL_BLESS},
  {SPELL_SENSE_LIFE, SPELL_DETECT_MAGIC},
  {SPELL_ANIMAL_SUMMONING_II, SPELL_ANIMAL_SUMMONING},
  {SPELL_ABJURE, SPELL_SUMMON},
  {SPELL_FIREBALL, SPELL_BURNING_HANDS},
  {SPELL_SLEEP, SPELL_WEAKNESS},
  {SPELL_MYSTIC_SPIRIT, SPELL_CALL_ANIMAL_SPIRIT},
  {SPELL_TELEPORT, SPELL_SUMMON},
  {SPELL_STONESKIN, SPELL_BARKSKIN},
  {SPELL_TELEPORT_OBJECT, SPELL_TELEPORT},
  {SPELL_AID, SPELL_BLESS},
  {SPELL_BRAVERY, SPELL_REMOVE_FEAR},
  {SPELL_HEAL, SPELL_CURE_CRITIC},
  {SPELL_HARM, SPELL_CAUSE_CRITIC},
  {SPELL_INSECT_PLAGUE, SPELL_SUMMON_INSECTS},
  {SPELL_CHAIN_LIGHTNING, SPELL_LIGHTNING_BOLT},
  {SPELL_SANCTUARY, SPELL_MINOR_SANCTUARY},
  {SPELL_THORNSHIELD, SPELL_THORNFLESH},
  {SPELL_SCRY, SPELL_LOCATE_OBJECT},
  {SPELL_REMOVE_PARALYSIS, SPELL_CURE_DISEASE},
  {SPELL_PARALYZE, SPELL_DISEASE},
  {SPELL_CLOAK_OF_FEAR, SPELL_INSPIRE_FEAR},
  {SPELL_MAJOR_GLOBE, SPELL_MINOR_GLOBE},
  {SPELL_BLINK, SPELL_BLUR},
  {SPELL_ARROW_OF_BONE, SPELL_HOLD_PERSON},
  {SPELL_SPHERE_SILENCE, SPELL_SILENCE},
  {SPELL_FIRE_SHIELD, SPELL_FIREBALL},
  {SPELL_FREE_ACTION, SPELL_HOLD_PERSON},
  {SPELL_STORM_SUMMONING, SPELL_CALL_LIGHTNING},
  {SPELL_HEALING_LIGHT, SPELL_HEAL},
  {SPELL_HARMFUL_WRATH, SPELL_HARM},
  {SPELL_STICKS_TO_SNAKES, SPELL_ANIMAL_SUMMONING_II},
  {SPELL_CONE_OF_COLD, SPELL_CHILL_TOUCH},
  {SPELL_ANIMAL_SUMMONING_III, SPELL_ANIMAL_SUMMONING_II},
  {SPELL_HEROES_FEAST, SPELL_CREATE_FOOD},
  {SPELL_MIRE, SPELL_ENTANGLE},
  {SPELL_REFLECTING_POOL, SPELL_LOCATE_OBJECT},
  {SPELL_ANTIMAGIC_AURA, SPELL_SILENCE},
  {SPELL_DIVINE_WRATH, SPELL_PRAYER},
  {SPELL_PROT_FROM_ELEMENTS, SPELL_PROT_FROM_FIRE},
  {SPELL_PROT_FROM_ELEMENTS, SPELL_PROT_FROM_FROST},
  {SPELL_DECEPTIVE_IMAGERY, SPELL_MIRROR_IMAGE},
  {SPELL_DECEPTIVE_IMAGERY, SPELL_BLINK},
  {SPELL_LEGEND_LORE, SPELL_IDENTIFY},
  {SPELL_MASS_CHARM, SPELL_CHARM},
  {SPELL_CRIPPLE, SPELL_WEAKNESS},
  {SPELL_CRIPPLE, SPELL_SLEEP},
  {SPELL_ENERGY_DRAIN, SPELL_VAMPIRIC_TOUCH},
  {SPELL_GATE, SPELL_SUMMON},
  {SPELL_GATE, SPELL_TELEPORT},
  {SPELL_MOONBEAM, SPELL_RAINBOW},
  {SPELL_METEOR_SWARM, SPELL_FIREBALL},
  {SPELL_GREATER_ELEMENTAL, SPELL_CONJURE_ELEMENTAL},
  {SPELL_CREEPING_DOOM, SPELL_INSECT_PLAGUE},
  {SPELL_PHANTASMAL_SNAKE, SPELL_DANCING_SWORD},
  {SPELL_RECHARGE_ITEM, SPELL_ENCHANT_WEAPON},
  {SPELL_ELEMENTAL_STORM, SPELL_STORM_SUMMONING},
  {SPELL_ELEMENTAL_STORM, SPELL_DEHYDRATION},
  {SPELL_SUNRAY, SPELL_MOONBEAM},
  {SPELL_BREATH_OF_LIFE, SPELL_CURE_DISEASE},
  {SPELL_BLACK_PLAGUE, SPELL_DISEASE},
  {SPELL_SPELL_TURNING, SPELL_MAJOR_GLOBE},
  {SPELL_VAMPIRIC_AURA, SPELL_ENERGY_DRAIN},
  {0, 0}
};

const struct element_type elements[] = {
  { SPELL_FIREBALL, ELEMENT_FIRE },
  { SPELL_BURNING_HANDS, ELEMENT_FIRE },
  { SPELL_SUNRAY, ELEMENT_FIRE },
  { SPELL_MOONBEAM, ELEMENT_FIRE },
  { SPELL_LIGHTNING_BOLT, ELEMENT_LIGHTNING },
  { ABIL_FIRE_BREATH, ELEMENT_FIRE },
  { ABIL_FROST_BREATH, ELEMENT_COLD },
  { ABIL_LIGHTNING_BREATH, ELEMENT_LIGHTNING },
  { ABIL_ACID_BREATH, ELEMENT_ACID },
};

const char *skill_affect_mesg[] = {
  "!Unused!",						/*  0  */
  "You feel someone protecting you.",
  "!Teleport!",
  "You feel righteous.",
  "A cloak of blindness surrounds you.",
  "!Burning Hands!",					/*  5  */
  "!Call Lightning!",
  "You feel quite charmed.",
  "!Chill Touch!",
  "Your body is blurry and unfocused.",
  "!Mass Charm!",					/*  10 */
  "Thorns from your flesh protrude from your skin.",
  "!Create Food!",
  "!Create Water!",
  "!Cure Blind!",
  "!Cure Critic!",					/*  15 */
  "!Cure Light!",
  "You feel quite accursed.",
  "You are sensitive to the alignment of others.",
  "You feel sensitive to the presence of invisible beings.",
  "You feel sensitive to the presence of magic.",	/*  20 */
  "!Detect Poison!",
  "!Dispel Evil!",
  "Your legs feel quite crippled.",
  "!Enchant Weapon!",
  "!Energy Drain!",					/*  25 */
  "!Fireball!",
  "!Harm!",
  "!Heal!",
  "You are invisible to the naked eye.",
  "!Lightning Bolt!",					/*  30 */
  "!Locate Object!",
  "!Magic Missile!",
  "You feel very sick.",
  "You feel protected from evil beings.",
  "!Remove Curse!",					/*  35 */
  "A white aura surrounds you.",
  "!Shocking Grasp!",
  "You feel very sleepy.",
  "You feel stronger than normal.",
  "!Summon!",						/*  40 */
  "You are surrounded by a faintly visible shield.",
  "!Word of Recall!",
  "!Neutralize Poison!",
  "You are sensitive to the presence of hidden beings.",
  "!Legend Lore!",					/*  45 */
  "!Dispel Good!",
  "A faint white aura surrounds your body.",
  "!Healing Light!",
  "The sounds or prayer fill you with warmth.",
  "Your eyes glow a faint red.",			/*  50 */
  "You feel very bouyant.",
  "Your skin has taken on the texture of bark.",
  "Your skin is incredibly hard.",
  "!Harmful Wrath!",
  "!Cure Deafness!",					/*  55 */
  "You are a stone statue.",
  "Some mirror images surround you.",
  "You feel capable of flight.",
  "You feel somewhat weaker.",
  "The roar of wind fills your ears, deafening you.",	/*  60 */
  "!Cure Disease!",
  "!Remove Paralysis!",
  "!Restoration!",
  "!Regeneration!",
  "You are moving quite fast.",				/*  65 */
  "You feel diseased.",
  "Your body is totally paralyzed.",
  "Your spirit has been somewhat drained.",
  "You feel quite withered.",
  "You are moving somewhat slower.",			/*  70 */
  "!Portal!",
  "!Elemental Storm!",
  "!Create Spring!",
  "You feel protected from good beings.",
  "You feel protected from the undead.",		/*  75 */
  "You feel somewhat resistant to fire.",
  "You are somewhat resistant to frost.",
  "You feel resistant to all the elements.",
  "You seem to be immune to mental assault.",
  "!Calm!",						/*  80 */
  "A cloak of fear surrounds you.",
  "A terrifying fear has gripped you.",
  "!Heroes Feast!",
  "You feel quite brave.",
  "!Greater Elemental!",				/*  85 */
  "!Unused!",
  "!Blade Barrier!",
  "A flickering aura of faerie fire surrounds you.",
  "!Sunray!",
  "!Moonbeam!",						/*  90 */
  "!Dispel Magic!",
  "Light shines out from you in all directions.",
  "Darkness seeps out from you in all directions.",
  "You are quite unable to speak.",
  "You are quite unable to speak.",			/*  95 */
  "!Cure Moderate!",
  "!Cure Severe!",
  "!Cause Light!",
  "!Cause Moderate!",
  "!Cause Severe!",					/* 100 */
  "!Cause Critic!",
  "!Goodberry!",
  "You sense a magical vestment protecting you.",
  "Your movements seem quite unrestricted.",
  "!Magical Stone!",					/* 105 */
  "!Shillelagh!",
  "A magical hammer floats above your head.",
  "!Flamestrike!",
  "You feel aided.",
  "!Call Animal Spirit!",				/* 110 */
  "!Summon Insects!",
  "!Animal Summoning!",
  "!Animal Summoning II!",
  "!Animal Summoning III!",
  "!Creeping Doom!",					/* 115 */
  "!Insect Plague!",
  "!Rainbow!",
  "Vines and roots have entangled you.",
  "A shield of thorns surrounds you.",
  "!Sticks to Snakes!",					/* 120 */
  "!Aerial Servant!",
  "!Summon Guard!",
  "!Dust Devil!",
  "A blade of flame floats nearby.",
  "You feel able to breathe underwater.",		/* 125 */
  "!Conjure Elemental!",
  "You feel capable of walking on the winds.",
  "A phantasmal snake waits by your side.",
  "!Identify!",
  "You feel somewhat resistant to plague.",		/* 130 */
  "You have been inflicted with a deadly plague.",
  "!Reflecting Pool!",
  "You feel able to ingeniously recall select dweomers and enchantments.",
  "!Remove Fear!",
  "You feel inspired with a divine wrath.",		/* 135 */
  "!Holy Word!",
  "You feel your life force merged with another spirit.",
  "!Storm Summoning!",
  "!Dehydration!",
  "You are stuck in some sort of quagmire.",		/* 140 */
  "!Abjure!",
  "Your weapon seems to be incredibly hard.",
  "You are magically held immobile.",
  "You are trapped by a layer of thick webs.",
  "!Gust of Wind!",					/* 145 */
  "You are surrounded by a semi-transparent globe.",
  "You are surrounded by a transparent shimmering globe.",
  "!Vampiric Touch!",
  "A magical sword dances around your head.",
  "!Scry!",						/* 150 */
  "!Chain Lightning!",
  "!Teleport Object!",
  "!Arrow of Bone!",
  "!Cone of Cold!",
  "You are surrounded by a fiery aura.",		/* 155 */
  "A black aura clings to you.",
  "You are blinking in and out of view.",
  "!Prismatic Spray!",
  "!Meteor Swarm!",
  "You feel more susceptable to the powers of magic.",	/* 160 */
  "A faint yellow aura surrounds you.",
  "A few deceptive images surround you.",
  "You are prepared to turn oncoming magic.",
  "!Recharge Item!",
  "A vampiric aura seeps out of your flesh.",		/* 165 */
  "!Refresh!",
  "!Power Word Kill!",
  "Your nature is concealed from others.", 
  "You feel quite beastly.",
  "!Reveasl Illusion!", /* 170 */
  "You feel more agile.",
  "You feel more healthy and energetic.", 
  "You feel more insightful.",
  "You feel smarter.", 
  "You feel more socially adept.",	/* 175 */
  "!Blessing of Kings!",
  "Your morale has been influenced by a knightly challenge.", 
  "!UNUSED!", "!UNUSED!", "!UNUSED!",   /* 180 */
  "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!",   /* 185 */
  "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!",   /* 190 */
  "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!",   /* 195 */
  "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!",   /* 200 */
  "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!",   /* 205 */
  "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!",   /* 210 */
  "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!",   /* 215 */
  "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!",   /* 220 */
  "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!",   /* 225 */
  "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!",   /* 230 */
  "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!",   /* 235 */
  "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!",   /* 240 */
  "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!",   /* 245 */
  "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!"	/* 250 */
};

const char *languages[] = {
    "silvanesti",
    "qualinesti",
    "dwarven",
    "kothian"
};
