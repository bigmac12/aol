/* ************************************************************************
*   File: spells.h                                      Part of CircleMUD *
*  Usage: header file: constants and fn prototypes for spell system       *
*                                                                         *
*  All rights reserved.  See license.doc for complete information.        *
*                                                                         *
*  Copyright (C) 1993, 94 by the Trustees of the Johns Hopkins University *
*  CircleMUD is based on DikuMUD, Copyright (C) 1990, 1991.               *
************************************************************************ */

#define DEFAULT_STAFF_LVL	12
#define DEFAULT_WAND_LVL	12

#define NUM_SPELL_FLAGS         28
#define CAST_UNDEFINED	-1
#define CAST_SPELL	0
#define CAST_POTION	1
#define CAST_WAND	2
#define CAST_STAFF	3
#define CAST_SCROLL	4

#define MAG_DAMAGE	(1 << 0)
#define MAG_AFFECTS	(1 << 1)
#define MAG_UNAFFECTS	(1 << 2)
#define MAG_POINTS	(1 << 3)
#define MAG_ALTER_OBJS	(1 << 4)
#define MAG_GROUPS	(1 << 5)
#define MAG_MASSES	(1 << 6)
#define MAG_AREAS	(1 << 7)
#define MAG_SUMMONS	(1 << 8)
#define MAG_CREATIONS	(1 << 9)
#define MAG_MANUAL	(1 << 10)
#define MAG_PULSE_BASED (1 << 11)
#define MAG_AREA_MAGIC  (1 << 12)


#define TYPE_UNDEFINED               -1
#define SPELL_RESERVED_DBC            0  /* SKILL NUMBER ZERO -- RESERVED */

/* PLAYER SPELLS -- Numbered from 1 to MAX_SPELLS */
/*  And now comes the fun part, as I renumber everything - Soli, 8/99  */

#define SPELL_ARMOR                   1
#define SPELL_TELEPORT                2
#define SPELL_BLESS                   3
#define SPELL_BLINDNESS               4
#define SPELL_BURNING_HANDS           5
#define SPELL_CALL_LIGHTNING          6
#define SPELL_CHARM                   7
#define SPELL_CHILL_TOUCH             8
#define SPELL_BLUR                    9
#define SPELL_MASS_CHARM             10
#define SPELL_THORNFLESH             11
#define SPELL_CREATE_FOOD            12
#define SPELL_CREATE_WATER           13
#define SPELL_CURE_BLIND             14
#define SPELL_CURE_CRITIC            15
#define SPELL_CURE_LIGHT             16
#define SPELL_BESTOW_CURSE           17
#define SPELL_DETECT_ALIGN           18
#define SPELL_DETECT_INVIS           19
#define SPELL_DETECT_MAGIC           20
#define SPELL_DETECT_POISON          21
#define SPELL_DISPEL_EVIL            22
#define SPELL_CRIPPLE                23
#define SPELL_ENCHANT_WEAPON         24
#define SPELL_ENERGY_DRAIN           25
#define SPELL_FIREBALL               26
#define SPELL_HARM                   27
#define SPELL_HEAL                   28
#define SPELL_INVISIBILITY           29
#define SPELL_LIGHTNING_BOLT         30
#define SPELL_LOCATE_OBJECT          31
#define SPELL_MAGIC_MISSILE          32
#define SPELL_POISON                 33
#define SPELL_PROT_FROM_EVIL         34
#define SPELL_REMOVE_CURSE           35
#define SPELL_SANCTUARY              36
#define SPELL_SHOCKING_GRASP         37
#define SPELL_SLEEP                  38
#define SPELL_STRENGTH               39
#define SPELL_SUMMON                 40
#define SPELL_SHIELD                 41
#define SPELL_WORD_OF_RECALL         42
#define SPELL_NEUTRALIZE_POISON      43
#define SPELL_SENSE_LIFE             44
#define SPELL_LEGEND_LORE            45
#define SPELL_DISPEL_GOOD            46
#define SPELL_MINOR_SANCTUARY        47
#define SPELL_HEALING_LIGHT          48
#define SPELL_PRAYER                 49
#define SPELL_INFRAVISION            50
#define SPELL_WATER_WALK             51
#define SPELL_BARKSKIN               52
#define SPELL_STONESKIN              53
#define SPELL_HARMFUL_WRATH          54 
#define SPELL_CURE_DEAFNESS          55
#define SPELL_STATUE                 56
#define SPELL_MIRROR_IMAGE           57
#define SPELL_FLIGHT                 58
#define SPELL_WEAKNESS               59
#define SPELL_DEAFENING_WIND         60
#define SPELL_CURE_DISEASE           61
#define SPELL_REMOVE_PARALYSIS       62
#define SPELL_RESTORATION            63
#define SPELL_REGENERATION           64
#define SPELL_HASTE                  65
#define SPELL_DISEASE                66
#define SPELL_PARALYZE               67
#define SPELL_SOUL_DRAIN             68
#define SPELL_WITHER                 69
#define SPELL_SLOW                   70
#define SPELL_GATE                   71
#define SPELL_ELEMENTAL_STORM        72
#define SPELL_CREATE_SPRING          73
#define SPELL_PROT_FROM_GOOD         74
#define SPELL_PROT_FROM_UNDEAD       75
#define SPELL_PROT_FROM_FIRE         76
#define SPELL_PROT_FROM_FROST        77
#define SPELL_PROT_FROM_ELEMENTS     78
#define SPELL_IMPERVIOUS_MIND        79
#define SPELL_CALM                   80
#define SPELL_CLOAK_OF_FEAR          81
#define SPELL_INSPIRE_FEAR           82
#define SPELL_HEROES_FEAST           83 
#define SPELL_BRAVERY                84 
#define SPELL_GREATER_ELEMENTAL      85
#define SPELL_UNUSED                 86
#define SPELL_BLADE_BARRIER          87
#define SPELL_FAERIE_FIRE            88
#define SPELL_SUNRAY                 89
#define SPELL_MOONBEAM               90
#define SPELL_DISPEL_MAGIC           91
#define SPELL_LIGHT                  92
#define SPELL_DARKNESS               93
#define SPELL_SILENCE                94
#define SPELL_SPHERE_SILENCE         95
#define SPELL_CURE_MODERATE          96
#define SPELL_CURE_SEVERE            97
#define SPELL_CAUSE_LIGHT            98
#define SPELL_CAUSE_MODERATE         99
#define SPELL_CAUSE_SEVERE          100
#define SPELL_CAUSE_CRITIC          101
#define SPELL_GOODBERRY             102
#define SPELL_MAGIC_VEST            103
#define SPELL_FREE_ACTION           104
#define SPELL_MAGICAL_STONE         105
#define SPELL_SHILLELAGH            106
#define SPELL_SPIRIT_HAMMER         107
#define SPELL_FLAMESTRIKE           108
#define SPELL_AID                   109
#define SPELL_CALL_ANIMAL_SPIRIT    110
#define SPELL_SUMMON_INSECTS        111
#define SPELL_ANIMAL_SUMMONING      112
#define SPELL_ANIMAL_SUMMONING_II   113
#define SPELL_ANIMAL_SUMMONING_III  114
#define SPELL_CREEPING_DOOM         115
#define SPELL_INSECT_PLAGUE         116
#define SPELL_RAINBOW               117
#define SPELL_ENTANGLE              118
#define SPELL_THORNSHIELD           119
#define SPELL_STICKS_TO_SNAKES      120
#define SPELL_AERIAL_SERVANT        121
#define SPELL_SUMMON_GUARD          122
#define SPELL_DUST_DEVIL            123
#define SPELL_FLAME_BLADE           124
#define SPELL_WATER_BREATHING       125
#define SPELL_CONJURE_ELEMENTAL     126
#define SPELL_WIND_WALK             127
#define SPELL_PHANTASMAL_SNAKE      128
#define SPELL_IDENTIFY              129
#define SPELL_BREATH_OF_LIFE        130
#define SPELL_BLACK_PLAGUE          131
#define SPELL_REFLECTING_POOL       132
#define SPELL_RECOLLECTION          133
#define SPELL_REMOVE_FEAR           134
#define SPELL_DIVINE_WRATH          135
#define SPELL_HOLY_WORD             136
#define SPELL_MYSTIC_SPIRIT         137
#define SPELL_STORM_SUMMONING       138
#define SPELL_DEHYDRATION           139
#define SPELL_MIRE                  140
#define SPELL_ABJURE                141
#define SPELL_ADAMANT_MACE          142
#define SPELL_HOLD_PERSON           143
#define SPELL_WEB                   144
#define SPELL_GUST_OF_WIND          145
#define SPELL_MINOR_GLOBE           146
#define SPELL_MAJOR_GLOBE           147
#define SPELL_VAMPIRIC_TOUCH        148
#define SPELL_DANCING_SWORD         149
#define SPELL_SCRY                  150
#define SPELL_CHAIN_LIGHTNING       151
#define SPELL_TELEPORT_OBJECT       152
#define SPELL_ARROW_OF_BONE         153
#define SPELL_CONE_OF_COLD          154
#define SPELL_FIRE_SHIELD           155
#define SPELL_BLACKMANTLE           156
#define SPELL_BLINK                 157
#define SPELL_PRISMATIC_SPRAY       158
#define SPELL_METEOR_SWARM          159
#define SPELL_MAGICAL_SUSCEPT       160
#define SPELL_ANTIMAGIC_AURA        161
#define SPELL_DECEPTIVE_IMAGERY     162
#define SPELL_SPELL_TURNING         163
#define SPELL_RECHARGE_ITEM         164
#define SPELL_VAMPIRIC_AURA         165
#define SPELL_REFRESH               166
#define SPELL_POWER_WORD_KILL       167
#define SPELL_CONCEAL_ALIGN         168
#define SPELL_POLYMORPH             169
#define SPELL_REVEAL_ILLUSION       170
#define SPELL_ENHANCE_AGILITY       171
#define SPELL_ENHANCE_ENDURANCE     172
#define SPELL_ENHANCE_INSIGHT       173
#define SPELL_ENHANCE_CUNNING       174
#define SPELL_ENHANCE_CHARISMA      175
#define SPELL_BLESSING_OF_KINGS     176
#define SPELL_SK_CHALLENGE          177
#define SPELL_DONTUSEME		    178    /*Must always be the 2nd last spell*/

//#define LANG_QUALINESTI 190
//#define LANG_SILVANESTI 191
//#define LANG_DWARVEN    192
//#define LANG_KOTHIAN    193


/* Insert new spells here, up to MAX_SPELLS */
/*  Or change max_spells and corrupt the pfile - Soli, 8/99  */
#define MAX_SPELLS                  200

/* PLAYER SKILLS - Numbered from MAX_SPELLS+1 to MAX_SKILLS */
#define SKILL_BACKSTAB                  201
#define SKILL_BASH                      202
#define SKILL_HIDE                      203
#define SKILL_KICK                      204
#define SKILL_PICK_LOCK                 205
#define SKILL_BANDAGE                   206
#define SKILL_RESCUE                    207
#define SKILL_SNEAK                     208
#define SKILL_STEAL                     209
#define SKILL_TRACK                     210
#define SKILL_SCAN                      211
#define SKILL_SECOND_ATTACK             212
#define SKILL_THIRD_ATTACK              213
#define SKILL_RETREAT                   214
#define SKILL_DUAL_WIELD                215
#define SKILL_DODGE                     216
#define SKILL_DISARM                    217
#define SKILL_ASSESS                    218
#define SKILL_GUARD                     219
#define SKILL_LAY_HANDS                 220
#define SKILL_NATURAL_HARDINESS         221
#define SKILL_SCRIBE                    222
#define SKILL_BREW                      223
#define SKILL_DETECT_POISON             224
#define SKILL_REMOVE_POISON             225
#define SKILL_STANCE                    226
#define SKILL_GORE                      227
#define SKILL_READ_MAGIC                228
#define SKILL_ASSESS_MAGIC              229
#define SKILL_ENVENOM                   230
#define SKILL_DISGUISE                  231
#define SKILL_TURN_UNDEAD               232
#define SKILL_CONTROL_UNDEAD            233
#define SKILL_CALM                      234
// new skills for monks --gan
#define SKILL_FOURTH_ATTACK             235
#define SKILL_THROW                     236
#define SKILL_CHAKRA                    237
#define SKILL_KNOCKOUT                  238
#define SKILL_PARRY                     239
#define SKILL_PASS_WO_TRACE             240
//  new skills for rogues and monks - JAD 5-8-03
#define SKILL_TRIP                      241
#define SKILL_RIDING		            242 /* (R) Riding (DAK)               */
#define SKILL_TAME		                243 /* (R) Ability to tame (DAK)      */
#define SKILL_SKIN                      244
#define SKILL_JAWSTRIKE	                245
#define SKILL_DIRTKICK	                246
#define SKILL_BUILD_FIRE                247
#define SKILL_COOK                      248
#define SKILL_DETECTDISGUISE            249
#define SKILL_ENCHANT_ITEM              250
#define SKILL_TWO_HANDED_STYLE          251
#define SKILL_TWO_WEAPON_STYLE          252
#define SKILL_ONE_WEAPON_STYLE          253
#define SKILL_SWORD_SHIELD_STYLE        254
#define SKILL_CHARGE                    255
#define SKILL_IMPROVED_CHARGE           256
#define SKILL_IMPROVED_BACKSTAB         257
#define SKILL_IMPROVED_STEALTH          258
#define SKILL_ADVANCED_BACKSTAB         259
#define SKILL_TIMER                     260
#define SKILL_ARMOR_MASTERY             261
#define SKILL_SHIELD_MASTERY            262
#define SKILL_IMPROVED_ARMOR_MASTERY    263
#define SKILL_ADVANCED_ARMOR_MASTERY    264
#define SKILL_IMPROVED_SHIELD_MASTERY   265
#define SKILL_ADVANCED_SHIELD_MASTERY   266
#define SKILL_DAMAGE_REDUCTION          267
#define SKILL_IMPROVED_DAMAGE_REDUCTION 268
#define SKILL_ADVANCED_DAMAGE_REDUCTION 269
#define SKILL_DIRTY_FIGHTING            270
#define SKILL_COMBAT_IMPROVISATION      271
#define SKILL_MARTIAL_ARTS              272
#define SKILL_IMPROVED_MARTIAL_ARTS     273
#define SKILL_ADVANCED_MARTIAL_ARTS     274
#define SKILL_INSIGHT                   275
#define SKILL_UNARMED_DEFENSE           276
#define SKILL_IMPROVED_UNARMED_DEFENSE  277
#define SKILL_ADVANCED_UNARMED_DEFENSE  278
#define SKILL_THROW_OPPONENT            279
#define SKILL_CHALLENGE                 280
#define SKILL_MARK                      281
#define SKILL_SMITE_GOOD                282
#define SKILL_SMITE_EVIL                283
#define SKILL_SMITE_LAW                 284
#define SKILL_SMITE_CHAOS               285
#define SKILL_SLIP                      286
/*  New skills up to 300  - Luni  */

/*
 *  NON-PLAYER AND OBJECT SPELLS AND SKILLS
 *  The practice levels for the spells and skills below are _not_ recorded
 *  in the playerfile; therefore, the intended use is for spells and skills
 *  associated with objects or NPCs.
 */

#define ABIL_FIRE_BREATH             302
#define ABIL_GAS_BREATH              304
#define ABIL_FROST_BREATH            304
#define ABIL_ACID_BREATH             305
#define ABIL_LIGHTNING_BREATH        306


#define TOP_SPELL_DEFINE	           329

/*  Other stuff here until 329  */


/* WEAPON ATTACK TYPES */

#define TYPE_HIT                     330
#define TYPE_STING                   331
#define TYPE_WHIP                    332
#define TYPE_SLASH                   333
#define TYPE_BITE                    334
#define TYPE_BLUDGEON                335
#define TYPE_CRUSH                   336
#define TYPE_POUND                   337
#define TYPE_CLAW                    338
#define TYPE_MAUL                    339
#define TYPE_THRASH                  340
#define TYPE_PIERCE                  341
#define TYPE_BLAST	             342
#define TYPE_PUNCH	             343
#define TYPE_STAB	             344
#define TYPE_SPEAR                   345
#define TYPE_CLEAVE                  346

#define MAX_WEAPON_DAMAGE            390

/* new attack types can be added here - up to TYPE_SUFFERING */
/*  Okay, I will.   Soli, 8/6/99, paving the way for ticdam  */
#define TYPE_DROWNING                396
#define TYPE_PULSE_DAMAGE            397
#define TYPE_MALNOURISHMENT          398
#define TYPE_SUFFERING		     399

#define  SPHERE_CLE_ALL            0
#define  SPHERE_CLE_CREATION       1
#define  SPHERE_CLE_HEALING        2
#define  SPHERE_CLE_HEALING_REV    3
#define  SPHERE_CLE_NECROMANCY     4
#define  SPHERE_CLE_NECRO_REV      5
#define  SPHERE_CLE_DIVINATION     6
#define  SPHERE_CLE_PROTECTION     7
#define  SPHERE_CLE_CHARM          8
#define  SPHERE_CLE_COMBAT         9
#define  SPHERE_CLE_ANIMAL        10
#define  SPHERE_CLE_WEATHER       11
#define  SPHERE_CLE_PLANT         12
#define  SPHERE_CLE_SUMMONING     13
#define  SPHERE_CLE_ELEMENTAL     14

#define  NUM_SPHERES              14

#define  SCHOOL_ALL               15
#define  SCHOOL_WHITE             30
#define  SCHOOL_RED               45
#define  SCHOOL_BLACK             60
#define  SCHOOL_NO_WHITE          75
#define  SCHOOL_NO_RED            90
#define  SCHOOL_NO_BLACK         105

#define  MAX_SCHOOL              105


#define SAVING_PARA   0
#define SAVING_ROD    1
#define SAVING_PETRI  2
#define SAVING_BREATH 3
#define SAVING_SPELL  4


#define TAR_IGNORE        1
#define TAR_CHAR_ROOM     2
#define TAR_CHAR_WORLD    4
#define TAR_FIGHT_SELF    8
#define TAR_FIGHT_VICT   16
#define TAR_SELF_ONLY    32 /* Only a check, use with i.e. TAR_CHAR_ROOM */
#define TAR_NOT_SELF     64 /* Only a check, use with i.e. TAR_CHAR_ROOM */
#define TAR_OBJ_INV     128
#define TAR_OBJ_ROOM    256
#define TAR_OBJ_WORLD   512
#define TAR_OBJ_EQUIP  1024
#define TAR_OBJ_AND_WLD 2048


struct spell_info_type {
  byte min_position;	/* Position for caster	 */

  int min_level[NUM_CLASSES];
  int routines;
  byte violent;
  int targets;         /* See below for use with TAR_XXX  */

  int     spellSphere;
  int     wait;        /*  Wait state per spell */

  int     applymod;    /*  Amount of learning, skill-dependant  */
  int     attrib;      /*  Attribute associated with the skill  */
  int     max_day;     /*  Max amount of improves per day       */
  int     max_percent; /*  Max percentage of the skill          */
  int min_level_race[NUM_RACES];
};

struct single_comp_type {
  int vnum;
  long flags;
};

struct component_type {
  int spellnum;
  struct single_comp_type comps[2];
};

struct prerequisites {
  int spell;
  int pre;
};

struct element_type {
  int type;
  int element;
};

#define ELEMENT_FIRE        1
#define ELEMENT_LIGHTNING   2
#define ELEMENT_ACID        3
#define ELEMENT_COLD        4
#define ELEMENT_AIR         5
#define ELEMENT_WATER       6
#define ELEMENT_EARTH       7


/*  Flags for spell components - Soli, 8/99   */

#define CMP_INV      1
#define CMP_HOLD     2

/*  Constants for certain objects necessary and vital to many spells and skills.
    Defined globally as opposed to locally to make changing them easier,
    especially when multiple spells/skills require them.  Soli, 9/3/99  */

#define VNUM_WAYBREAD           0
#define VNUM_BERRIES            1
#define VNUM_MINOR_SALVE        2
#define VNUM_MODERATE_SALVE     3
#define VNUM_MAJOR_SALVE        4
#define VNUM_REFRESHING_SALVE   5
#define VNUM_BREW_CAULDRON      6
#define VNUM_BREW_VIAL          7
#define VNUM_SCRIBE_PAPER       8
#define VNUM_SCRIBE_PEN         9
#define VNUM_ENVENOM_POISON     10
#define VNUM_SPRING             15

/*  And constants for mob vnums for conjuration/summoning - Soli, 9/3/99  */

#define VNUM_INSECTS            1
#define VNUM_ANIMAL_SPIRIT      2
#define VNUM_ANIMAL_SPIRIT_2    3
#define VNUM_ANIMAL_SPIRIT_3    4
#define VNUM_ANIMAL_SUM_I       5
#define VNUM_ANIMAL_SUM_I_2     6
#define VNUM_ANIMAL_SUM_I_3     7
#define VNUM_ANIMAL_SUM_II      8
#define VNUM_ANIMAL_SUM_II_2    9
#define VNUM_ANIMAL_SUM_II_3    10
#define VNUM_ANIMAL_SUM_III     11
#define VNUM_ANIMAL_SUM_III_2   12
#define VNUM_ANIMAL_SUM_III_3   13
#define VNUM_ANIMAL_SUM_III_4   14
#define VNUM_AERIAL_SERVANT     15
#define VNUM_DUST_DEVIL         16
#define VNUM_ELEMENTAL          17
#define VNUM_ELEMENTAL_2        18
#define VNUM_ELEMENTAL_3        19
#define VNUM_ELEMENTAL_4        20
#define VNUM_SNAKES             21
#define VNUM_GUARD              22
#define VNUM_GREATER_ELEMENTAL  23
#define VNUM_GREATER_ELEM_2     24
#define VNUM_GREATER_ELEM_3     25
#define VNUM_GREATER_ELEM_4     26

 /*  Attributes - for skill learning  */

#define ATTR_STR     1
#define ATTR_DEX     2
#define ATTR_INT     3
#define ATTR_WIS     4
#define ATTR_CON     5
#define ATTR_CHA     6

/* Possible Targets:

   bit 0 : IGNORE TARGET
   bit 1 : PC/NPC in room
   bit 2 : PC/NPC in world
   bit 3 : Object held
   bit 4 : Object in inventory
   bit 5 : Object in room
   bit 6 : Object in world
   bit 7 : If fighting, and no argument, select tar_char as self
   bit 8 : If fighting, and no argument, select tar_char as victim (fighting)
   bit 9 : If no argument, select self, if argument check that it IS self.

*/

#define SPELL_TYPE_SPELL   0
#define SPELL_TYPE_POTION  1
#define SPELL_TYPE_WAND    2
#define SPELL_TYPE_STAFF   3
#define SPELL_TYPE_SCROLL  4


#define PORTAL_OBJ  11   /* the vnum of the portal object */


/* Attacktypes with grammar */

struct attack_hit_type {
   char	*singular;
   char	*plural;
};


#define ASPELL(spellname) \
void	spellname(int level, struct char_data *ch, \
		  struct char_data *victim, struct obj_data *obj)

#define MANUAL_SPELL(spellname)	spellname(level, caster, cvict, ovict);

ASPELL(spell_create_water);
ASPELL(spell_recall);
ASPELL(spell_teleport);
ASPELL(spell_summon);
ASPELL(spell_locate_object);
ASPELL(spell_charm);
ASPELL(spell_information);
ASPELL(spell_identify);
ASPELL(spell_legend_lore);
ASPELL(spell_enchant_weapon);
ASPELL(spell_detect_poison);
ASPELL(spell_enchant_armor);
ASPELL(spell_gate);
ASPELL(spell_create_spring);
ASPELL(spell_fear);
ASPELL(spell_calm);
ASPELL(spell_dispel_magic);
ASPELL(spell_holy_word);
ASPELL(spell_abjure);
ASPELL(spell_scry);
ASPELL(spell_gust);
ASPELL(spell_teleport_object);
ASPELL(spell_energy_drain);
ASPELL(spell_chain);
ASPELL(spell_cripple);
ASPELL(spell_recharge_item);
ASPELL(spell_reveal_illusion);

/* basic magic calling functions */

int find_skill_num(char *name);

void mag_damage(int level, struct char_data *ch, struct char_data *victim,
  int spellnum, int savetype);

void mag_affects(int level, struct char_data *ch, struct char_data *victim,
  int spellnum, int savetype);

void mag_group_switch(int level, struct char_data *ch, struct char_data *tch, 
  int spellnum, int savetype);

void mag_groups(int level, struct char_data *ch, int spellnum, int savetype);

void mag_masses(int level, struct char_data *ch, int spellnum, int savetype);

void mag_areas(int level, struct char_data *ch, int spellnum, int savetype);

void mag_summons(int level, struct char_data *ch, struct char_data *victim,
                 struct obj_data *obj, int spellnum, int savetype);

void mag_points(int level, struct char_data *ch, struct char_data *victim,
 int spellnum, int savetype);

void mag_unaffects(int level, struct char_data *ch, struct char_data *victim,
  int spellnum, int type);

void mag_alter_objs(int level, struct char_data *ch, struct obj_data *obj,
  int spellnum, int type);

void mag_creations(int level, struct char_data *ch, int spellnum);

int	call_magic(struct char_data *caster, struct char_data *cvict,
  struct obj_data *ovict, int spellnum, int level, int casttype);

void	mag_objectmagic(struct char_data *ch, struct obj_data *obj,
			char *argument);

int	cast_spell(struct char_data *ch, struct char_data *tch,
  struct obj_data *tobj, int spellnum);


/* other prototypes */
void spell_level(int spell, int class, int level, int sphere);
void spell_level_race(int spell, int race, int level, int sphere);
void init_spell_levels(void);
char *skill_name(int num);


int  spell_sphere_access(int chDomain, int chClass, int spellNum);
