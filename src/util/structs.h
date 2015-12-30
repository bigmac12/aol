/* ************************************************************************
*   File: structs.h                                     Part of CircleMUD *
*  Usage: header file for central structures and contstants               *
*                                                                         *
*  All rights reserved.  See license.doc for complete information.        *
*                                                                         *
*  Copyright (C) 1993, 94 by the Trustees of the Johns Hopkins University *
*  CircleMUD is based on DikuMUD, Copyright (C) 1990, 1991.               *
************************************************************************ */



/*
 * Intended use of this macro is to allow external packages to work with
 * a variety of CircleMUD versions without modifications.  For instance,
 * an IS_CORPSE() macro was introduced in pl13.  Any future code add-ons
 * could take into account the CircleMUD version and supply their own
 * definition for the macro if used on an older version of CircleMUD.
 * You are supposed to compare this with the macro CIRCLEMUD_VERSION()
 * in utils.h.  See there for usage.
 */
#define _CIRCLEMUD      0x030010 /* Major/Minor/Patchlevel - MMmmPP */

/* preamble *************************************************************/

#define NOWHERE    -1    /* nil reference for room-database	*/
#define NOTHING	   -1    /* nil reference for objects		*/
#define NOBODY	   -1    /* nil reference for mobiles		*/

#define SPECIAL(name) \
   int (name)(struct char_data *ch, void *me, int cmd, char *argument)

/* misc editor defines **************************************************/

/* format modes for format_text */
#define FORMAT_INDENT		(1 << 0)


/* room-related defines *************************************************/


/* The cardinal directions: used as index to room_data.dir_option[] */
#define NORTH          0
#define EAST           1
#define SOUTH          2
#define WEST           3
#define UP             4
#define DOWN           5
#define NORTHWEST      6
#define NORTHEAST      7
#define SOUTHWEST      8
#define SOUTHEAST      9

/* Room flags: used in room_data.room_flags */
/* WARNING: In the world files, NEVER set the bits marked "R" ("Reserved") */
#define ROOM_DARK		(1 << 0)   /* Dark			*/
#define ROOM_DEATH		(1 << 1)   /* Death trap		*/
#define ROOM_NOMOB		(1 << 2)   /* MOBs not allowed		*/
#define ROOM_INDOORS		(1 << 3)   /* Indoors			*/
#define ROOM_PEACEFUL		(1 << 4)   /* Violence not allowed	*/
#define ROOM_SOUNDPROOF		(1 << 5)   /* Shouts, gossip blocked	*/
#define ROOM_NOTRACK		(1 << 6)   /* Track won't go through	*/
#define ROOM_NOMAGIC		(1 << 7)   /* Magic not allowed		*/
#define ROOM_TUNNEL		(1 << 8)   /* room for only 1 pers	*/
#define ROOM_PRIVATE		(1 << 9)   /* Can't teleport in		*/
#define ROOM_GODROOM		(1 << 10)  /* LVL_GOD+ only allowed	*/
#define ROOM_HOUSE		(1 << 11)  /* (R) Room is a house	*/
#define ROOM_HOUSE_CRASH	(1 << 12)  /* (R) House needs saving	*/
#define ROOM_ATRIUM		(1 << 13)  /* (R) The door to a house	*/
#define ROOM_OLC		(1 << 14)  /* (R) Modifyable/!compress	*/
#define ROOM_BFS_MARK		(1 << 15)  /* (R) breath-first srch mrk	*/
#define ROOM_PKILL              (1 << 16)
#define ROOM_NORECALL           (1 << 17)
#define ROOM_GUARDED            (1 << 18)
#define ROOM_PULSE_DAMAGE       (1 << 19)
#define ROOM_NO_OOC		(1 << 20)   /* NO ooc communication     */
#define ROOM_SALTWATER_FISH     (1 << 21)  /* Player can fish here     */
#define ROOM_FRESHWATER_FISH    (1 << 22)  /* Player can fish here too */
#define ROOM_NODIG		(1 << 23)
#define ROOM_NOBURY             (1 << 24)
#define ROOM_TOWNHS             (1 << 25)
#define ROOM_CUSTOMHS           (1 << 26)

/* Exit info: used in room_data.dir_option.exit_info */

#define EX_ISDOOR		(1 << 0)   /* Exit is a door		*/
#define EX_CLOSED		(1 << 1)   /* The door is closed	*/
#define EX_LOCKED		(1 << 2)   /* The door is locked	*/
#define EX_PICKPROOF		(1 << 3)   /* Lock can't be picked	*/
#define EX_HIDDEN               (1 << 4)   /* Door is hidden            */


/* Sector types: used in room_data.sector_type */
#define SECT_INSIDE          0		   /* Indoors			*/
#define SECT_CITY            1		   /* In a city			*/
#define SECT_FIELD           2		   /* In a field		*/
#define SECT_FOREST          3		   /* In a forest		*/
#define SECT_HILLS           4		   /* In the hills		*/
#define SECT_MOUNTAIN        5		   /* On a mountain		*/
#define SECT_WATER_SWIM      6		   /* Swimmable water		*/
#define SECT_WATER_NOSWIM    7		   /* Water - need a boat	*/
#define SECT_UNDERWATER	     8		   /* Underwater		*/
#define SECT_FLYING          9
#define SECT_UNUSED_1       10
#define SECT_UNUSED_2       11

/* char and mob-related defines *****************************************/

/* PC classes */
#define CLASS_BAD_STATS   -4
#define CLASS_NEWRACE	  -3
#define CLASS_BAD_RACE	  -2
#define CLASS_UNDEFINED	  -1

#define CLASS_KNIGHT       0
#define CLASS_PALADIN      1
#define CLASS_MAGE         2
#define CLASS_CLERIC       3
#define CLASS_RANGER       4
#define CLASS_ROGUE        5
#define CLASS_WARRIOR      6
#define CLASS_ADVENTURER   7   /* The Andon-oids live! */
#define CLASS_DRUID        8
#define CLASS_MONK         9
#define CLASS_BARD         10
#define CLASS_SORCEROR     11
#define CLASS_DEFENDER     12
#define CLASS_FILLER_TWO   13
#define CLASS_FILLER_THREE 14

#define NUM_CLASSES	   15  /* This must be the number of classes!! */

/* plr races */

#define RACE_UNDEFINED	-1 
#define RACE_BARBARIAN   0
#define RACE_HYLAR       1
#define RACE_DAEWAR      2
#define RACE_NEIDAR      3
#define RACE_SILVANESTI  4
#define RACE_QUALINESTI  5
#define RACE_KAGONESTI   6
#define RACE_HALFELVEN   7
#define RACE_KENDER      8
#define RACE_MINOTAUR    9
#define RACE_GNOME      10
#define RACE_HUMAN      11
#define NUM_RACES	12

/*  NPC classes -  Soli, 8/8/99  */

#define NPC_CLASS_UNDEFINED      0
#define NPC_CLASS_MAGE           1
#define NPC_CLASS_CLERIC         2
#define NPC_CLASS_WARRIOR        3
#define NPC_CLASS_ROGUE          4
#define NPC_CLASS_DRUID          5
#define NPC_CLASS_RANGER         6
#define NPC_CLASS_PALADIN        7
#define NPC_CLASS_UNDEAD_MAGE    8
#define NPC_CLASS_UNDEAD_CLERIC  9
#define NPC_CLASS_UNDEAD_FIGHTER 10
#define NUM_NPC_CLASSES          10

/* Clans --gan, 07/27/2001 */

#define CLAN_KOS         1
#define CLAN_CONCLAVE    2
#define CLAN_HOLY_ORDER  3
#define CLAN_BLACKBLADE  4

#define CLAN_MAX_CLAN    15
#define CLAN_MAX_LEVEL   101

/* Sex */

#define SEX_NEUTRAL   0
#define SEX_MALE      1
#define SEX_FEMALE    2

/* Positions */

#define POS_DEAD       0        /* dead                 */
#define POS_MORTALLYW  1        /* mortally wounded     */
#define POS_INCAP      2        /* incapacitated        */
#define POS_STUNNED    3        /* stunned              */
#define POS_SLEEPING   4        /* sleeping             */
#define POS_RESTING    5        /* resting              */
#define POS_SITTING    6        /* sitting              */
#define POS_FIGHTING   7        /* fighting             */
#define POS_STANDING   8        /* standing             */
#define POS_DIGGING    9	
#define POS_FISHING    10       /*  fishing            */
#define POS_RIDING     11        /* riding             */

/* Player flags: used by char_data.char_specials.act */


#define PLR_KILLER	    (1 << 0)   /* Player is a player-killer		*/
#define PLR_THIEF	    (1 << 1)   /* Player is a player-thief		*/
#define PLR_FROZEN	    (1 << 2)   /* Player is frozen			*/
#define PLR_DONTSET     (1 << 3)   /* Don't EVER set (ISNPC bit)	*/
#define PLR_WRITING	    (1 << 4)   /* Player writing (board/mail/olc)	*/
#define PLR_MAILING	    (1 << 5)   /* Player is writing mail		*/
#define PLR_CRASH	    (1 << 6)   /* Player needs to be crash-saved	*/
#define PLR_SITEOK	    (1 << 7)   /* Player has been site-cleared	*/
#define PLR_NOSHOUT	    (1 << 8)   /* Player not allowed to shout/goss	*/
#define PLR_RABBIT	    (1 << 9)   /* Player not allowed to set title	*/
#define PLR_DELETED	    (1 << 10)  /* Player deleted - space reusable	*/
#define PLR_LOADROOM	(1 << 11)  /* Player uses nonstandard loadroom	*/
#define PLR_NOWIZLIST   (1 << 12)  /* Player shouldn't be on wizlist	*/
#define PLR_NODELETE	(1 << 13)  /* Player shouldn't be deleted	*/
#define PLR_INVSTART	(1 << 14)  /* Player should enter game wizinvis	*/
#define PLR_CRYO	    (1 << 15)  /* Player is cryo-saved (purge prog)	*/
#define PLR_WOLF	    (1 << 16)  /* Player shouldn't be on wizlist	*/
#define PLR_BEAR	    (1 << 17)  /* Player shouldn't be deleted	*/
#define PLR_COURIER	    (1 << 18)  /* Player is either morphed/disguised */
#define PLR_CAT		    (1 << 19)  /* Player is cryo-saved (purge prog)	*/
#define PLR_BIRD	    (1 << 20)  /* Player is test char only          */
#define PLR_IMMCHAR     (1 << 21)  /* Player will have some imm notifications*/
#define PLR_FREERENT    (1 << 22)  /* Player will have free rent        */
#define PLR_SUBDUING    (1 << 23)  /* Player is in subdual mode         */
#define PLR_PRISONER    (1 << 24)  /* Player is a prisoner              */
#define PLR_FISHING     (1 << 25)  /* Player has a line in the water   */
#define PLR_FISH_ON     (1 << 26)  /* Player has a fish on their line  */
#define PLR_DIGGING     (1 << 27)  /* Player is digging   */
#define PLR_DIG_ON      (1 << 28)  /* Player has hit something   */
#define PLR_FIRE_ON     (1 << 29)
//#define PLR_MAGE	    (1 << 30)
//#define PLR_CLERIC	    (1 << 31)
//#define PLR_MONK	    (1 << 32)
//#define PLR_BARD	    (1 << 33)
//#define PLR_BEGGAR	    (1 << 34)
//#define PLR_KNIGHT	    (1 << 35)
#define PLR_BADMORT     (1 << 30)  /* Bad mort - no newbie */

/* Mobile flags: used by char_data.char_specials.act */

#define MOB_SPEC         (1 << 0)  /* Mob has a callable spec-proc	*/
#define MOB_SENTINEL     (1 << 1)  /* Mob should not move		*/
#define MOB_SCAVENGER    (1 << 2)  /* Mob picks up stuff on the ground	*/
#define MOB_ISNPC        (1 << 3)  /* (R) Automatically set on all Mobs	*/
#define MOB_AWARE	     (1 << 4)  /* Mob can't be backstabbed		*/
#define MOB_AGGRESSIVE   (1 << 5)  /* Mob hits players in the room	*/
#define MOB_STAY_ZONE    (1 << 6)  /* Mob shouldn't wander out of zone	*/
#define MOB_WIMPY        (1 << 7)  /* Mob flees if severely injured	*/
#define MOB_AGGR_EVIL	 (1 << 8)  /* auto attack evil PC's		*/
#define MOB_AGGR_GOOD	 (1 << 9)  /* auto attack good PC's		*/
#define MOB_AGGR_NEUTRAL (1 << 10) /* auto attack neutral PC's		*/
#define MOB_MEMORY	     (1 << 11) /* remember attackers if attacked	*/
#define MOB_HELPER	     (1 << 12) /* attack PCs fighting other NPCs	*/
#define MOB_NOCHARM	     (1 << 13) /* Mob can't be charmed		*/
#define MOB_NOSUMMON	 (1 << 14) /* Mob can't be summoned		*/
#define MOB_NOSLEEP	     (1 << 15) /* Mob can't be slept		*/
#define MOB_NOBASH	     (1 << 16) /* Mob can't be bashed (e.g. trees)	*/
#define MOB_NOBLIND	     (1 << 17) /* Mob can't be blinded		*/
#define MOB_HUNTERKILLER (1 << 18) /* Pkiller hunter/killer             */
#define MOB_USE_SPELLS   (1 << 19) /* Mob can cast spells               */
#define MOB_DBL_ATTACK   (1 << 20) /* Mob gets another attack           */
#define MOB_TRPL_ATTACK  (1 << 21) /* Mob gets another attack           */
#define MOB_QUAD_ATTACK  (1 << 22) /* Mob gets another attack           */
#define MOB_SENTRY       (1 << 23) /* Mob will block players            */
#define MOB_MOUNTABLE    (1 << 24) /* Mob may be mounted for riding     */
#define MOB_QUEST	     (1 << 25) /* Mob is used for quests		*/
#define MOB_PET          (1 << 26) /* Mob is a player pet               */

/* Preference flags: used by char_data.player_specials.pref */

#define PRF_BRIEF       (1 << 0)  /* Room descs won't normally be shown	*/
#define PRF_COMPACT     (1 << 1)  /* No extra CRLF pair before prompts	*/
#define PRF_DEAF	(1 << 2)  /* Can't hear shouts			*/
#define PRF_NOTELL	(1 << 3)  /* Can't receive tells		*/
#define PRF_DISPHP	(1 << 4)  /* Display hit points in prompt	*/
#define PRF_DETECT	(1 << 5)
#define PRF_DISPMOVE	(1 << 6)  /* Display move points in prompt	*/
#define PRF_AUTOEXIT	(1 << 7)  /* Display exits in a room		*/
#define PRF_NOHASSLE	(1 << 8)  /* Aggr mobs won't attack		*/
#define PRF_BANDAGED	(1 << 9)
#define PRF_NOBANDAGE   (1 << 10)
#define PRF_NOREPEAT	(1 << 11) /* No repetition of comm commands	*/
#define PRF_HOLYLIGHT	(1 << 12) /* Can see in dark			*/
#define PRF_COLOR_1	(1 << 13) /* Color (low bit)			*/
#define PRF_COLOR_2	(1 << 14) /* Color (high bit)			*/
#define PRF_NOWIZ	(1 << 15) /* Can't hear wizline			*/
#define PRF_LOG1	(1 << 16) /* On-line System Log (low bit)	*/
#define PRF_LOG2	(1 << 17) /* On-line System Log (high bit)	*/
#define PRF_NOTSELF     (1 << 18)
#define PRF_DISGUISE    (1 << 19)
#define PRF_HUNTED      (1 << 20)
#define PRF_ROOMFLAGS	(1 << 21) /* Can see room flags (ROOM_x)	*/
#define PRF_AFK		(1 << 22) /* AFK flag :)			*/
#define PRF_UNUSED_7    (1 << 23)
#define PRF_QUEST       (1 << 24)
#define PRF_LEVEL_FLAGS (1 << 25) /* so imm\imps can see level eq flags */
#define PRF_INTRO	(1 << 26) /* PC for intro toggle                */
#define PRF_NO_OOC	(1 << 27) /* No ooc chatter			*/
#define PRF_NO_NEWBIE   (1 << 28) /* Don't hear the newbie line         */
#define PRF_NO_CHAT     (1 << 29) /* The chat line is wiz/immchar only  */
#define PRF_TIRED       (1 << 30) /* After stance                       */
#define PRF_AUTOCON     (1 << 31) // player has auto consider turned on
 
/* Affect bits: used in char_data.char_specials.saved.affected_by */
/* WARNING: In the world files, NEVER set the bits marked "R" ("Reserved") */

#define AFF_BLIND             (1 << 0)	   /* (R) Char is blind		*/
#define AFF_INVISIBLE         (1 << 1)	   /* Char is invisible		*/
#define AFF_DETECT_ALIGN      (1 << 2)	   /* Char is sensitive to align*/
#define AFF_DETECT_INVIS      (1 << 3)	   /* Char can see invis chars  */
#define AFF_DETECT_MAGIC      (1 << 4)	   /* Char is sensitive to magic*/
#define AFF_SENSE_LIFE        (1 << 5)	   /* Char can sense hidden life*/
#define AFF_WATERWALK	      (1 << 6)	   /* Char can walk on water	*/
#define AFF_SANCTUARY         (1 << 7)	   /* Char protected by sanct.	*/
#define AFF_GROUP             (1 << 8)	   /* (R) Char is grouped	*/
#define AFF_CURSE             (1 << 9)	   /* Char is cursed		*/
#define AFF_INFRAVISION       (1 << 10)	   /* Char can see in dark	*/
#define AFF_POISON            (1 << 11)	   /* (R) Char is poisoned	*/
#define AFF_PROTECT_EVIL      (1 << 12)	   /* Char protected from evil  */
#define AFF_PROTECT_GOOD      (1 << 13)	   /* Char protected from good  */
#define AFF_SLEEP             (1 << 14)	   /* (R) Char magically asleep	*/
#define AFF_NOTRACK	      (1 << 15)	   /* Char can't be tracked	*/
#define AFF_FLIGHT            (1 << 16)
#define AFF_HASTE	      (1 << 17)	   /* Char moves rapidly        */
#define AFF_SNEAK             (1 << 18)	   /* Char can move quietly	*/
#define AFF_HIDE              (1 << 19)	   /* Char is hidden		*/
#define AFF_STANCE	      (1 << 20)	   /* Room for future expansion	*/
#define AFF_CHARM             (1 << 21)	   /* Char is charmed		*/
#define AFF_POLYMORPH         (1 << 22)
#define AFF_TIRED             (1 << 23)    /* Undead Timer              */
#define AFF_WATERBREATH       (1 << 24)    /* Underwater Breathing      */
#define AFF_STONESKIN         (1 << 25)    /* Stoneskin                 */
#define AFF_DETECT_DISGUISE   (1 << 26)    /* Detect Disguise      */
#define AFF_KNOCKOUT          (1 << 27)    /* Knockout     */
#define AFF_MAGIC_VEST        (1 << 28)    /* Magical Vestment          */
#define AFF_CONCEAL_ALIGN     (1 << 29)    /* Cannot tell align         */
#define AFF_TAMED	      (1 << 30)	   /* (R) Char has been tamed   */
#define AFF_JAWSTRIKE         (1 << 31) 

/* Modes of connectedness: used by descriptor_data.state */

#define CON_PLAYING	 0		/* Playing - Nominal state	*/
#define CON_CLOSE	 1		/* Disconnecting		*/
#define CON_GET_NAME	 2		/* By what name ..?		*/
#define CON_NAME_CNFRM	 3		/* Did I get that right, x?	*/
#define CON_PASSWORD	 4		/* Password:			*/
#define CON_NEWPASSWD	 5		/* Give me a password for x	*/
#define CON_CNFPASSWD	 6		/* Please retype password:	*/
#define CON_QSEX	 7		/* Sex?				*/
#define CON_QCLASS	 8		/* Class?			*/
#define CON_RMOTD	 9		/* PRESS RETURN after MOTD	*/
#define CON_MENU	 10		/* Your choice: (main menu)	*/
#define CON_EXDESC	 11		/* Enter a new description:	*/
#define CON_CHPWD_GETOLD 12             /* Changing passwd: get old     */
#define CON_CHPWD_GETNEW 13             /* Changing passwd: get new     */
#define CON_CHPWD_VRFY   14		/* Verify new password		*/
#define CON_DELCNF1	 15		/* Delete confirmation 1	*/
#define CON_DELCNF2	 16		/* Delete confirmation 2	*/
#define CON_OEDIT	 17		/*. OLC mode - object edit     .*/
#define CON_REDIT	 18		/*. OLC mode - room edit       .*/
#define CON_ZEDIT	 19		/*. OLC mode - zone info edit  .*/
#define CON_MEDIT	 20		/*. OLC mode - mobile edit `   .*/
#define CON_SEDIT	 21		/*. OLC mode - shop edit       .*/
#define CON_TRIGEDIT	 22		/*. OLC mode - trigger edit    .*/
#define CON_QRACE        23             /* Race?                        */ 
#define CON_QANSI	 24		/* ANSI colors YES? no		*/
/*  PDH 12/ 2/98 - char gen  */
#define CON_QSHORT_D     25             /* short description            */
#define CON_QONELINE_D   26             /* one line description         */
#define CON_QLONG_D      27             /* long (paragraph) description */
#define CON_QABILITY     28             /* ability scores               */
#define CON_ALIAS        29             /* alias list                   */
#define CON_AFTER_DESC   30             /* after char complete          */
#define CON_ALIGNMENT    31             /* static alignment selection   */
#define CON_GODSELECT    32             /* cleric/paladin god selection */
#define CON_GEN_DESCS_INTRO 33           /* Introduction text for generic short descs */
#define CON_GEN_DESCS_DESCRIPTORS_1 34 /* Set descriptor 1 for generic short descs */
#define CON_GEN_DESCS_DESCRIPTORS_2 35 /* Set descriptor 2 for generic short descs */
#define CON_GEN_DESCS_ADJECTIVES_1 36  /* Set adjective 1 for generic short descs */
#define CON_GEN_DESCS_ADJECTIVES_2 37  /* Set adjective 2 for generic short descs */
#define CON_GEN_DESCS_MENU 38          /* Generic short desc menu */
#define CON_GEN_DESCS_MENU_PARSE 39    /* Decide what to do from generic short desc menu choice */
#define CON_DISCONNECT   99             /* in game disconnection        */

/*  PDH  2/26/99
 *  God Selection defines - one for each Krynn God
 */

#define  GOD_PALADINE      1
#define  GOD_KIRIJOLITH    2
#define  GOD_HABBAKUK      3
#define  GOD_MISHAKAL      4
#define  GOD_BRANCHALA     5
#define  GOD_SOLINARI      6
#define  GOD_MAJERE        7

#define  GOD_GILEAN       11
#define  GOD_REORX        12
#define  GOD_LUNITARI     13
#define  GOD_CHISLEV      14
#define  GOD_SIRRION      15
#define  GOD_ZIVILYN      16
#define  GOD_SHINARE      17

#define  GOD_TAKHISIS     21
#define  GOD_SARGONNAS    22
#define  GOD_NUITARI      23
#define  GOD_MORGION      24
#define  GOD_CHEMOSH      25
#define  GOD_ZEBOIM       26
#define  GOD_HIDDUKEL     27

#define  NUM_GODS         30

/* Character equipment positions: used as index for char_data.equipment[] */
/* NOTE: Don't confuse these constants with the ITEM_ bitvectors
   which control the valid places you can wear a piece of equipment */

#define WEAR_LIGHT      0
#define WEAR_FINGER_R   1
#define WEAR_FINGER_L   2
#define WEAR_NECK_1     3
#define WEAR_NECK_2     4
#define WEAR_BODY       5
#define WEAR_HEAD       6
#define WEAR_LEGS       7
#define WEAR_FEET       8
#define WEAR_HANDS      9
#define WEAR_ARMS      10
#define WEAR_SHIELD    11
#define WEAR_ABOUT     12
#define WEAR_WAIST_1   13
#define WEAR_WRIST_R   14
#define WEAR_WRIST_L   15
#define WEAR_WIELD     16
#define WEAR_HOLD      17
#define WEAR_FACE      18
#define WEAR_EAR_R     19
#define WEAR_EAR_L     20
#define WEAR_ANKLE_R   21
#define WEAR_ANKLE_L   22
#define WEAR_ABOVE     23
#define WEAR_BACK_1    24
#define WEAR_SHOULDER  25
#define WEAR_NOSE      26
#define WEAR_ONBELT    27
#define WEAR_ONBACK_1  28
#define WEAR_ONBACK_2  29
#define WEAR_ONBACK_3  30
#define WEAR_WAIST_2   31
#define WEAR_SHEATHED_H   32
#define WEAR_SHEATHED_B1 33
#define WEAR_SHEATHED_B2 34
#define WEAR_SHEATHED_WA1 35
#define WEAR_SHEATHED_WA2 36
#define WEAR_SHEATHED_A1 37
#define WEAR_SHEATHED_A2 38
#define WEAR_SHEATHED_WR1 39
#define WEAR_SHEATHED_WR2 40

#define NUM_WEARS      41      /* This must be the # of eq positions!! */

/* object-related defines ********************************************/

/* Item types: used by obj_data.obj_flags.type_flag */

#define ITEM_LIGHT      1		/* Item is a light source	*/
#define ITEM_SCROLL     2		/* Item is a scroll		*/
#define ITEM_WAND       3		/* Item is a wand		*/
#define ITEM_STAFF      4		/* Item is a staff		*/
#define ITEM_WEAPON     5		/* Item is a weapon		*/
#define ITEM_SALVE      6               /* Item is a salve              */
#define ITEM_BELT       7		/* Unimplemented		*/
#define ITEM_TREASURE   8		/* Item is a treasure, not gold	*/
#define ITEM_ARMOR      9		/* Item is armor		*/
#define ITEM_POTION    10 		/* Item is a potion		*/
#define ITEM_WORN      11		/* Unimplemented		*/
#define ITEM_OTHER     12		/* Misc object			*/
#define ITEM_TRASH     13		/* Trash - shopkeeps won't buy	*/
#define ITEM_BOTTLE    14		/* Unimplemented		*/
#define ITEM_CONTAINER 15		/* Item is a container		*/
#define ITEM_NOTE      16		/* Item is note 		*/
#define ITEM_DRINKCON  17		/* Item is a drink container	*/
#define ITEM_KEY       18		/* Item is a key		*/
#define ITEM_FOOD      19		/* Item is food			*/
#define ITEM_MONEY     20		/* Item is money (gold)		*/
#define ITEM_PEN       21		/* Item is a pen		*/
#define ITEM_SHOVEL    22		/* Item is a boat		*/
#define ITEM_FOUNTAIN  23		/* Item is a fountain		*/
#define ITEM_FIRE      24                /* Item for flying             */ 
#define ITEM_AQUALUNG  25
#define ITEM_SHEATH    26
#define ITEM_RAW       27
#define ITEM_PORTAL    28
#define ITEM_POLE      29
#define ITEM_FIREWOOD  30

/* Take/Wear flags: used by obj_data.obj_flags.wear_flags */

#define ITEM_WEAR_TAKE		(1 << 0)  /* Item can be takes		*/
#define ITEM_WEAR_FINGER	(1 << 1)  /* Can be worn on finger	*/
#define ITEM_WEAR_NECK		(1 << 2)  /* Can be worn around neck 	*/
#define ITEM_WEAR_BODY		(1 << 3)  /* Can be worn on body 	*/
#define ITEM_WEAR_HEAD		(1 << 4)  /* Can be worn on head 	*/
#define ITEM_WEAR_LEGS		(1 << 5)  /* Can be worn on legs	*/
#define ITEM_WEAR_FEET		(1 << 6)  /* Can be worn on feet	*/
#define ITEM_WEAR_HANDS		(1 << 7)  /* Can be worn on hands	*/
#define ITEM_WEAR_ARMS		(1 << 8)  /* Can be worn on arms	*/
#define ITEM_WEAR_SHIELD	(1 << 9)  /* Can be used as a shield	*/
#define ITEM_WEAR_ABOUT		(1 << 10) /* Can be worn about body 	*/
#define ITEM_WEAR_WAIST 	(1 << 11) /* Can be worn around waist 	*/
#define ITEM_WEAR_WRIST		(1 << 12) /* Can be worn on wrist 	*/
#define ITEM_WEAR_WIELD		(1 << 13) /* Can be wielded		*/
#define ITEM_WEAR_HOLD		(1 << 14) /* Can be held		*/
#define ITEM_WEAR_FACE          (1 << 15) /* new position face          */
#define ITEM_WEAR_EAR           (1 << 16) /* new position ear           */
#define ITEM_WEAR_ANKLE         (1 << 17) /* new position ankle         */
#define ITEM_WEAR_ABOVE         (1 << 18) /* new position above head    */
#define ITEM_WEAR_BACK          (1 << 19) /* new position back          */
#define ITEM_WEAR_SHOULDER      (1 << 20) /* new position sholders      */
#define ITEM_WEAR_NOSE          (1 << 21) /* new position nose          */
#define ITEM_WEAR_SPARE_1       (1 << 22) /* new position sheathed      */
#define ITEM_WEAR_SHEATHED_B    (1 << 23) /* new position for sheath obj*/
#define ITEM_WEAR_ONBELT        (1 << 24) /* new position for sheath obj*/
#define ITEM_WEAR_ONBACK        (1 << 25)
#define ITEM_WEAR_SHEATHED_WA   (1 << 26)
#define ITEM_WEAR_SHEATHED_WR   (1 << 27)
#define ITEM_WEAR_SHEATHED_A    (1 << 28)
#define ITEM_WEAR_SHEATHED_H    (1 << 29)

/* Extra object flags: used by obj_data.obj_flags.extra_flags */

#define ITEM_GLOW          (1 << 0)	/* Item is glowing		*/
#define ITEM_HIDDEN        (1 << 1)     /* Item is hiding               */
#define ITEM_NORENT        (1 << 2)	/* Item cannot be rented	*/
#define ITEM_NO_REMOVE     (1 << 3)     /* Item cannot be removed       */
#define ITEM_NOINVIS	   (1 << 4)	/* Item cannot be made invis	*/
#define ITEM_INVISIBLE     (1 << 5)	/* Item is invisible		*/
#define ITEM_MAGIC         (1 << 6)	/* Item is magical		*/
#define ITEM_NODROP        (1 << 7)	/* Item is cursed: can't drop	*/
#define ITEM_BLESS         (1 << 8)     // Temporary holder value       */
#define ITEM_UNIQUE_SAVE   (1 << 8)	/* Item is unique		*/
#define ITEM_ANTI_GOOD     (1 << 9)	/* Not usable by good people	*/
#define ITEM_ANTI_EVIL     (1 << 10)	/* Not usable by evil people	*/
#define ITEM_ANTI_NEUTRAL  (1 << 11)	/* Not usable by neutral people	*/
#define ITEM_ANTI_MAGE     (1 << 12)    /* Not usable by mages          */
#define ITEM_ANTI_CLERIC   (1 << 13)    /* Not usable by clerics        */
#define ITEM_ANTI_ROGUE    (1 << 14)    /* Not usable by rogues         */
#define ITEM_ANTI_WARRIOR  (1 << 15)    /* Not usable by warriors       */
#define ITEM_NOSELL	   (1 << 16)	/* Shopkeepers won't touch it	*/
#define ITEM_ANTI_KNIGHT   (1 << 17)	/* Not usable by knights        */
#define ITEM_ANTI_PALADIN  (1 << 18)	/* Not usable by paladins       */
#define ITEM_ANTI_RANGER   (1 << 19)	/* Not usable by rangers        */
#define ITEM_ALLOW_MONK    (1 << 20)    /* item usable by monks         */
#define ITEM_ANTI_DRUID    (1 << 21)	/* Not usable by druids         */
#define ITEM_ANTI_BARBAR   (1 << 22)	/* Not usable by barbarians     */
#define ITEM_ANTI_DWARF    (1 << 23)	/* Not usable by dwarves        */
#define ITEM_ANTI_ELF      (1 << 24)	/* Not usable by elves          */
#define ITEM_ANTI_HALFELF  (1 << 25)	/* Not usable by halfelves      */
#define ITEM_ANTI_KENDER   (1 << 26)	/* Not usable by kender         */
#define ITEM_ANTI_MINOTAUR (1 << 27)	/* Not usable by minnies        */
#define ITEM_ANTI_GNOME    (1 << 28)	/* Not usable by gnomes         */
#define ITEM_ANTI_HUMAN    (1 << 29)	/* Not usable by humans         */
#define ITEM_TWO_HANDED    (1 << 30)    /*  Two-handed weapon           */
#define ITEM_CREATE        (1 << 31)

/* Modifier constants used with obj affects ('A' fields) */

#define APPLY_NONE              0	/* No effect			*/
#define APPLY_STR               1	/* Apply to strength		*/
#define APPLY_DEX               2	/* Apply to dexterity		*/
#define APPLY_INT               3	/* Apply to constitution	*/
#define APPLY_WIS               4	/* Apply to wisdom		*/
#define APPLY_CON               5	/* Apply to constitution	*/
#define APPLY_CHA		6	/* Apply to charisma		*/
#define APPLY_CLASS             7	/* Reserved			*/
#define APPLY_LEVEL             8	/* Reserved			*/
#define APPLY_AGE               9	/* Apply to age			*/
#define APPLY_CHAR_WEIGHT      10	/* Apply to weight		*/
#define APPLY_CHAR_HEIGHT      11	/* Apply to height		*/
#define APPLY_UNDEFINED        12
#define APPLY_HIT              13	/* Apply to max hit points	*/
#define APPLY_MOVE             14	/* Apply to max move points	*/
#define APPLY_GOLD             15	/* Reserved			*/
#define APPLY_EXP              16	/* Reserved			*/
#define APPLY_AC               17	/* Apply to Armor Class		*/
#define APPLY_HITROLL          18	/* Apply to hitroll		*/
#define APPLY_DAMROLL          19	/* Apply to damage roll		*/
#define APPLY_SAVING_PARA      20	/* Apply to save throw: paralz	*/
#define APPLY_SAVING_ROD       21	/* Apply to save throw: rods	*/
#define APPLY_SAVING_PETRI     22	/* Apply to save throw: petrif	*/
#define APPLY_SAVING_BREATH    23	/* Apply to save throw: breath	*/
#define APPLY_SAVING_SPELL     24	/* Apply to save throw: spells	*/
#define APPLY_QUESTPOINTS      25       /* Apply quest points i hope	*/
#define APPLY_LIGHT            26       /* Apply to light source        */
#define APPLY_UNDEFINED_2      27

/* Container flags - value[1] */

#define CONT_CLOSEABLE      (1 << 0)	/* Container can be closed	*/
#define CONT_PICKPROOF      (1 << 1)	/* Container is pickproof	*/
#define CONT_CLOSED         (1 << 2)	/* Container is closed		*/
#define CONT_LOCKED         (1 << 3)	/* Container is locked		*/

/* Some different kind of liquids for use in values of drink containers */

#define LIQ_WATER      0
#define LIQ_BEER       1
#define LIQ_WINE       2
#define LIQ_ALE        3
#define LIQ_DARKALE    4
#define LIQ_WHISKY     5
#define LIQ_LEMONADE   6
#define LIQ_FIREBRT    7
#define LIQ_LOCALSPC   8
#define LIQ_SLIME      9
#define LIQ_MILK       10
#define LIQ_TEA        11
#define LIQ_COFFE      12
#define LIQ_BLOOD      13
#define LIQ_SALTWATER  14
#define LIQ_CLEARWATER 15

/*  Object material types - Soli, 10/1/99  */

#define MAT_UNDEFINED   (1 << 0)
#define MAT_CLOTH       (1 << 1)
#define MAT_LEATHER     (1 << 2)
#define MAT_GLASS       (1 << 3)
#define MAT_STONE       (1 << 4)
#define MAT_ORGANIC     (1 << 5)
#define MAT_PAPER       (1 << 6)
#define MAT_METAL       (1 << 7)
#define MAT_WOOD        (1 << 8)
#define MAT_BONE        (1 << 9)
#define MAT_CRYSTALLINE (1 << 10)
#define MAT_ETHER       (1 << 11)
#define MAT_ADAMANTINE  (1 << 12)
#define MAT_MITHRIL     (1 << 13)

/* other miscellaneous defines *******************************************/

/* Player conditions */

#define DRUNK        0
#define FULL         1
#define THIRST       2

/* Sun state for weather_data */

#define SUN_DARK	0
#define SUN_RISE	1
#define SUN_LIGHT	2
#define SUN_SET		3

/* Sky conditions for weather_data */

#define SKY_CLOUDLESS	0
#define SKY_CLOUDY	1
#define SKY_RAINING	2
#define SKY_LIGHTNING	3

/* Rent codes */

#define RENT_UNDEF      0
#define RENT_CRASH      1
#define RENT_RENTED     2
#define RENT_CRYO       3
#define RENT_FORCED     4
#define RENT_TIMEDOUT   5

/* other #defined constants **********************************************/

/*
 * **DO**NOT** blindly change the number of levels in your MUD merely by
 * changing these numbers and without changing the rest of the code to match.
 * Other changes throughout the code are required.  See coding.doc for
 * details.
 *
 * LVL_IMPL should always be the HIGHEST possible immortal level, and
 * LVL_IMMORT should always be the LOWEST immortal level.  The number of
 * mortal levels will always be LVL_IMMORT - 1.
 */

#define LVL_IMPL        38
#define LVL_ADMIN       37
#define LVL_GRGOD       36
#define LVL_DEPTHEAD    35
#define LVL_GOD         34
#define LVL_LESSERGOD   33
#define LVL_DEMIGOD     32
#define LVL_IMMORT      31

/* Level of the 'freeze' command */

#define LVL_FREEZE      LVL_GRGOD
#define LVL_APPROVE     LVL_GOD

#define NUM_OF_DIRS	10	/* number of directions in a room (nsewud) */
#define MAGIC_NUMBER    (0x06)  /* Arbitrary number that won't be in a string */

#define OPT_USEC	100000	/* 10 passes per second */
#define PASSES_PER_SEC	(1000000 / OPT_USEC)
#define RL_SEC		* PASSES_PER_SEC

#define PULSE_ZONE      (10 RL_SEC)
#define PULSE_MOBILE    (10 RL_SEC)
#define PULSE_VIOLENCE  (2 RL_SEC)

/* Variables for the output buffering system */

#define MAX_SOCK_BUF            (12 * 1024) /* Size of kernel's sock buf   */
#define MAX_PROMPT_LENGTH       512          /* Max length of prompt        */
#define GARBAGE_SPACE		32          /* Space for **OVERFLOW** etc  */
#define SMALL_BUFSIZE		1024        /* Static output buffer size   */

/* Max amount of output that can be buffered */
#define LARGE_BUFSIZE	   (MAX_SOCK_BUF - GARBAGE_SPACE - MAX_PROMPT_LENGTH)

#define HISTORY_SIZE            5   /* Keep last 5 commands. */

/* Let's up the size of strings. */
//#define MAX_STRING_LENGTH	16384
//#define MAX_INPUT_LENGTH	256	/* Max length per *line* of input */
//#define MAX_RAW_INPUT_LENGTH	512	/* Max size of *raw* input */
//
#define MAX_STRING_LENGTH       32768
#define MAX_INPUT_LENGTH        512     /* Max length per *line* of input */
#define MAX_RAW_INPUT_LENGTH    1024    /* Max size of *raw* input */


#define MAX_MESSAGES            120
/*  PDH 12/30/98 - we added alias list to the name  (was 20)  */
#define MAX_NAME_LENGTH		100 /* Used in char_file_u *DO*NOT*CHANGE* */
#define MAX_PWD_LENGTH		10  /* Used in char_file_u *DO*NOT*CHANGE* */
#define MAX_TITLE_LENGTH	80  /* Used in char_file_u *DO*NOT*CHANGE* */
#define HOST_LENGTH		60  /* Used in char_file_u *DO*NOT*CHANGE* */
#define POOF_LENGTH		90  /* for poof saving 90 charters max	   */
#define EXDSCR_LENGTH		600 /* Used in char_file_u *DO*NOT*CHANGE* */
#define LINE_DSCR_LENGTH	60  /* Used in char_file_u *DO*NOT*CHANGE* */
#define MAX_TONGUE		3   /* Used in char_file_u *DO*NOT*CHANGE* */
/*  I always hated the capped lines that say do not change - especially when
   most of them are perfectly safe if you know what you're doing.  The line that
   follows, of course, is not one of those.  Soli, 8/99   */
#define MAX_SKILLS              300  /*  Was 200   */
#define MAX_SPELL_MEM           80   /*  Maximum number of memmed spells  */
#define MAX_AFFECT		32  /* Used in char_file_u *DO*NOT*CHANGE* */
#define MAX_OBJ_AFFECT		6 /* Used in obj_file_elem *DO*NOT*CHANGE* */
#define MAX_SPELL_AFFECT	3 /* Used in obj_file_elem *DO*NOT*CHANGE* */

/*
 *  PDH 11/24/98
 *  used for intro/greet/tell system
 */

#define MAX_INTROS              30  /* used in player_special_data_saved   */


/**********************************************************************
* Structures                                                          *
**********************************************************************/


typedef signed char		    sbyte;
typedef unsigned char		ubyte;
typedef signed short int	sh_int;
typedef unsigned short int	ush_int;
typedef char bool;
typedef unsigned long int   bitvector_t;

typedef sh_int        zone_vnum; /* A virtual zone number.      */
typedef sh_int	      zone_rnum; /* A zone's real (array index) number.	*/

#ifndef CIRCLE_WINDOWS
typedef char byte;
#endif

typedef sh_int	room_vnum;	/* A room's vnum type */
typedef sh_int	obj_vnum;	/* An object's vnum type */
typedef sh_int	mob_vnum;	/* A mob's vnum type */

typedef sh_int	room_rnum;	/* A room's real (internal) number type */
typedef sh_int	obj_rnum;	/* An object's real (internal) num type */
typedef sh_int	mob_rnum;	/* A mobile's real (internal) num type */


/*  PDH  3/29/99
 *  money system: plat, steel, gold, copper
 */

struct money_data {  /*   plat  stee  gold  copp  */
  int platinum;      /*      1    10   100  1000  */
  int steel;         /*   1/10     1    10   100  */
  int gold;          /*  1/100  1/10     1    10  */
  int copper;        /* 1/1000 1/100  1/10     1  */
};

struct area_aff_data {
  struct char_data *caster;
  room_rnum in_room;

  int type;
  int initial_duration;
  int duration_left;

  int other;

  struct area_aff_data *next;
};

/* Extra description: used in objects, mobiles, and rooms */

struct extra_descr_data {
   char	*keyword;                 /* Keyword in look/examine          */
   char	*description;             /* What to see                      */
   struct extra_descr_data *next; /* Next in list                     */
};

/* object-related structures ******************************************/

/* object flags; used in obj_data */

struct obj_flag_data {
  int	value[4];	/* Values of the item (see list)    */
  int  skin_data[4];    /* vnums of items for do_skin corpse*/
  byte type_flag;	/* Type of item			    */
  int	wear_flags;	/* Where you can wear it	    */

  /*  PDH  2/18/99 - changed int to long for all the new ITEM flags  */
  /*  int	extra_flags;                                         */

  long	extra_flags;	/* If it hums, glows, etc.	    */

  int weight;		/* Weigt what else                  */
  int cost;		/* Value when sold (gp.)            */
  int cost_per_day;	/* Cost to keep pr. real day        */
  int obj_level;	/* object leveling by Zorn	    */
  int timer;		/* Timer for object                 */
  long bitvector;	/* To set chars bits                */
  int creation;
  int nature;
  int disguise;
};

/* Used in obj_file_elem *DO*NOT*CHANGE* */

struct obj_affected_type {
   byte location;      /* Which ability to change (APPLY_XXX) */
   sbyte modifier;     /* How much it changes by              */
};

/* Used in obj_file_elem *DONT CHANGE* */

struct obj_spell_type {
	ush_int spelltype;  /*number of spell*/
	ush_int level;	/*level of spell*/
	ush_int percentage; /*percentage of success*/
};

struct weapon_spells {
  int spellnum;
  int level;
  int percent;
};


/* ================== Memory Structure for Objects ================== */

struct obj_data {
   obj_vnum item_number;	/* Where in data-base			*/
   room_rnum in_room;		/* In what room -1 when conta/carr	*/

   bool   has_spells;
   struct weapon_spells wpn_spells[MAX_SPELL_AFFECT];

   struct obj_flag_data obj_flags;/* Object information               */
   struct obj_affected_type affected[MAX_OBJ_AFFECT];  /* affects */
   struct obj_spell_type spell_affect[MAX_SPELL_AFFECT];

   char	*name;                    /* Title of object :get etc.        */
   char	*description;		  /* When in room                     */
   char	*short_description;       /* when worn/carry/in cont.         */
   char *alt_description;         /* for brew/scribe                  */
   char *altshort_description;

   char	*action_description;      /* What to write when used          */
   struct extra_descr_data *ex_description; /* extra descriptions     */
   struct char_data *carried_by;  /* Carried by :NULL in room/conta   */
   struct char_data *worn_by;	  /* Worn by?			      */
   sh_int worn_on;		  /* Worn where?		      */

   struct obj_data *in_obj;       /* In what object NULL when none    */
   struct obj_data *contains;     /* Contains objects                 */

   long id;                       /* used by DG triggers              */
   struct trig_proto_list *proto_script; /* list of default triggers  */
   struct script_data *script;    /* script info for the object       */

   struct obj_data *next_content; /* For 'contains' lists             */
   struct obj_data *next;         /* For the object list              */
};

/* ======================================================================= */

/* ====================== File Element for Objects ======================= */
/*                 BEWARE: Changing it will ruin rent files		   */

struct obj_file_elem {
  obj_vnum item_number;
  sh_int locate;  /* that's the (1+)wear-location (when equipped) or
                     (20+)index in obj file (if it's in a container) BK */
  int	value[4];

  long	extra_flags;	/* If it hums, glows, etc.	    */

  int	weight;
  int	timer;
  long	bitvector;
  struct obj_affected_type affected[MAX_OBJ_AFFECT];
  struct obj_spell_type spell_affect[MAX_SPELL_AFFECT];
};

struct new_obj_file_elem {
  obj_vnum item_number;
  sh_int locate;  /* that's the (1+)wear-location (when equipped) or
                     (20+)index in obj file (if it's in a container) BK */
  int	value[4];

  /*  PDH  2/18/99 - changed int to long for all the new ITEM flags  */
  /*  int	extra_flags;                                         */
  long	extra_flags;	/* If it hums, glows, etc.	    */

  long  material;
  int   effects[4];
  long  spare0;
  long  spare1;
  long  spare2;
  long  spare3;

  int	weight;
  int	timer;
  long	bitvector;
  struct obj_affected_type affected[MAX_OBJ_AFFECT];
  struct obj_spell_type spell_affect[MAX_SPELL_AFFECT];
};

/* header block for rent files.  BEWARE: Changing it will ruin rent files  */

struct rent_info {
   int	time;
   int	rentcode;
   int	net_cost_per_diem;
   int	gold;
   int	account;
   int	nitems;
   int	spare0;
   int	spare1;
   int	spare2;
   int	spare3;
   int	spare4;
   int	spare5;
   int	spare6;
   int	spare7;
};

/* ======================================================================= */


/* room-related structures ************************************************/


struct room_direction_data {
   char	*general_description;       /* When look DIR.			*/

   char	*keyword;		/* for open/close			*/

   sh_int exit_info;		/* Exit info				*/
   obj_vnum key;		/* Key's number (-1 for no key)		*/
   room_rnum to_room;		/* Where direction leads (NOWHERE)	*/
};


/* ================== Memory Structure for room ======================= */

struct room_data {
   room_vnum number;		/* Rooms number	(vnum)		      */
   sh_int zone;                 /* Room zone (for resetting)          */
   int	sector_type;            /* sector type (move/hide)            */
   char	*name;                  /* Rooms name 'You are ...'           */
   char	*description;           /* Shown when entered                 */
   struct extra_descr_data *ex_description; /* for examine/look       */
   struct room_direction_data *dir_option[NUM_OF_DIRS]; /* Directions */
   int room_flags;		/* DEATH,DARK ... etc                 */

   int pulse_dam;               /*  self-explanatory                  */
   int specnum;                 /*  The number of the room's spec     */
   byte light;                  /* Number of lightsources in room     */
   byte blood;                  /* Amount of blood in the room        */
   SPECIAL(*func);

   struct trig_proto_list *proto_script; /* list of default triggers  */
   struct script_data *script;  /* script info for the object         */

   struct obj_data *contents;   /* List of items in room              */
   struct char_data *people;    /* List of NPC / PC in room           */
};

/* ====================================================================== */

/* char-related structures ************************************************/

/* memory structure for characters */

struct memory_rec_struct {
   long	id;
   struct memory_rec_struct *next;
};

typedef struct memory_rec_struct memory_rec;

/* This structure is purely intended to be an easy way to transfer */
/* and return information about time (real or mudwise).            */

struct time_info_data {
   int hours, day, month;
   sh_int year;
};

/* These data contain information about a players time data */

struct time_data {
  time_t birth;    /* This represents the characters age                */
  time_t logon;    /* Time of the last logon (used to calculate played) */
  int    played;   /* This is the total accumulated time played in secs */

  /*  PDH  4/12/99
   *  pkillers can't rent or recall for 15 minutes after attempting
   *  a pkill - this variable holds the time of last pkill attempt.
   *  checked and removed in points_update()
   */

  time_t  pkillAttempt;
  time_t  stealAttempt;

  time_t  last_lay_hands;
  time_t  last_chakra;
  time_t  last_stance;
};

/* general player-related info, usually PCs and NPCs */

struct char_player_data {
   char passwd[MAX_PWD_LENGTH+1]; /* character's password      */
   char *name;         /* PC / NPC s name (kill ...  )         */
   char *short_descr;  /* for NPC 'actions'                    */
   char *long_descr;   /* for 'look'                           */
   char *description;  /* Extra descriptions                   */
   char *title;        /* PC / NPC's title                     */
   char *poofin;
   char *poofout;
   byte sex;           /* PC / NPC's sex                       */
   byte class;         /* PC / NPC's class                     */

  /*  PDH 12/29/98 - moved to struct player_special_data_saved {}
      byte race;
      */

   byte level;         /* PC / NPC's level                     */
   int  hometown;      /* PC s Hometown (zone)                 */
   struct time_data time;  /* PC's AGE in days                 */
   ubyte weight;       /* PC / NPC's weight                    */
   ubyte height;       /* PC / NPC's height                    */
};

/* Char's abilities.  Used in char_file_u *DO*NOT*CHANGE* */

struct char_ability_data {
   sbyte str;
   sbyte str_add;      /* 000 - 100 if strength 18             */
   sbyte intel;
   sbyte wis;
   sbyte dex;
   sbyte con;
   sbyte cha;
};

/* Char's points.  Used in char_file_u *DO*NOT*CHANGE* */

struct char_point_data {
  sh_int mana;
  sh_int max_mana;     /* Max move for PC/NPC  */
  sh_int hit;
  sh_int max_hit;      /* Max hit for PC/NPC                      */
  sh_int move;
  sh_int max_move;     /* Max move for PC/NPC                     */

  sh_int armor;        /* Internal -100..100, external -10..10 AC */
  struct money_data money; /* Money carried                       */
  int   bank_gold;      /* Gold the char has in a bank account    */
  int   exp;            /* The experience of the player           */
  int questpoints;      /* for the auto quest \ trainings i hope :) */
  byte hitroll;       /* Any bonus or penalty to the hit roll    */
  byte damroll;       /* Any bonus or penalty to the damage roll */
};

/* 
 * char_special_data_saved: specials which both a PC and an NPC have in
 * common, but which must be saved to the playerfile for PC's.
 *
 * WARNING:  Do not change this structure.  Doing so will ruin the
 * playerfile.  If you want to add to the playerfile, use the spares
 * in player_special_data.
 */

struct char_special_data_saved {
  int  alignment;               /* +-1000 for good/evil alignments          */
  int  ethos;                   /* +-1000 for law/chaos alignments          */
  long idnum;                   /* player's idnum; -1 for mobiles           */
  long act;                     /* act flag for NPC's; player flag for PC's */

  long affected_by;             /* Bitvector for spells/skills affected by  */
  long mob_spell;               /* bitV for spells the MOB gets from OLC    */
  sh_int apply_saving_throw[5]; /* Saving throw (Bonuses)                   */
};

struct intro_type
{
  char name[MAX_NAME_LENGTH + 1];
  int charnum;
};

struct memmed_spell {
  int spellnum;
  int ticks;
};


/* Special playing constants shared by PCs and NPCs which aren't in pfile */

struct char_special_data {
   struct char_data *fighting;  /* Opponent				*/
   struct char_data *hunting;	/* Char hunted by this char		*/
   struct char_data *guarding;  /* Char guarded by this char            */
   struct char_data *riding;	/* Who are they riding? (DAK) */
   struct char_data *ridden_by; /* Who is riding them? (DAK) */
   char *name_dis;
   char *desc_dis;
   char *ldesc_dis;
   byte sex_dis;
   /*byte sex_dis; */
   int disnum;

   byte position;		/* Standing, fighting, sleeping, etc.	*/

   int	carry_weight;		/* Carried weight			*/
   byte carry_items;		/* Number of items carried		*/
   int	timer;			/* Timer for update			*/

   struct char_special_data_saved saved; /* constants saved in plrfile	*/
};

struct skill_data
{
  byte percent;
  byte reflections;
};


/*
 *  If you want to add new values to the playerfile, do it here.  DO NOT
 * ADD, DELETE OR MOVE ANY OF THE VARIABLES - doing so will change the
 * size of the structure and ruin the playerfile.  However, you can change
 * the names of the spares to something more meaningful, and then use them
 * in your new code.  They will automatically be transferred from the
 * playerfile into memory when players log in.
 */

struct player_special_data_saved {
  struct skill_data skills[MAX_SKILLS+1];  /* array of skills plus 0    */
  byte sparen;		        /* used to be spells_to_learn		*/
  bool talks[MAX_TONGUE];	/* PC s Tongues 0 for NPC		*/
  int	wimp_level;		/* Below this # of hit points, flee!	*/
  byte freeze_level;		/* Level of god who froze char, if any	*/
  sh_int invis_level;		/* level of invisibility		*/
  room_vnum load_room;		/* Which room to place char in		*/
  long	pref;			/* preference flags for PC's.		*/
  ubyte bad_pws;		/* number of bad password attemps	*/
  sbyte conditions[3];          /* Drunk, full, thirsty			*/
  sh_int approved;		/* are we approved? */
  int clans;                    /* clan membership? */

  /*  PDH 11/24/98 - saved intro's for players  */
  /*  And now we have to change it because this is really messed... Soli  */
  struct intro_type intro[MAX_INTROS];

  /*  SJM (Solinari), 8/29/99 - The rise of the magi at the cost of the pfile */
  struct memmed_spell spells[MAX_SPELL_MEM];

  /* spares below for future expansion.  You can change the names from
     'sparen' to something meaningful, but don't change the order.  */

  ubyte sdesc_descriptor_1;
  ubyte sdesc_descriptor_2;
  ubyte sdesc_adjective_1;
  ubyte sdesc_adjective_2;
  ubyte spell_source;
  ubyte spare5;
  int spells_to_learn;		/* How many can you learn yet this level*/
  int spare7;
  int trust;
  int spare9;
  int clan_level;

  int olc_zone;

  /*  PDH 12/29/98 - added race info  */
  int race;

  /*  PDH  2/25/99 - added god selection (cleric and paladin only)  */
  int god_selection;

  /*  PDH  4/ 9/99 - added for RP percentage of exp received  */
  int RPpercent;


  int notself;
  int memLevelFlag;
  int exp_multiplier;
  int speaking;
  int subClassTwo;
  int subClassThree;
  long	timeOnline;
  long	affected_by_2;
  long	spare19;
  long	spare20;
  long	spare21;
};

/*
 * Specials needed only by PCs, not NPCs.  Space for this structure is
 * not allocated in memory for NPCs, but it is for PCs and the portion
 * of it labelled 'saved' is saved in the playerfile.  This structure can
 * be changed freely; beware, though, that changing the contents of
 * player_special_data_saved will corrupt the playerfile.
 */
struct player_special_data {
   struct player_special_data_saved saved;

   struct alias_data *aliases;	/* Character's aliases			*/
   long last_tell;		/* idnum of last tell from		*/
   void *last_olc_targ;		/* olc control				*/
   int last_olc_mode;		/* olc control				*/
};


/* Specials used by NPCs, not PCs */
struct mob_special_data {
   byte last_direction;     /* The last direction the monster went     */
   int	attack_type;        /* The Attack Type Bitvector for NPC's     */
   byte default_pos;        /* Default position for NPC                */
   memory_rec *memory;	    /* List of attackers to remember	       */
   byte damnodice;          /* The number of damage dice's	       */
   byte damsizedice;        /* The size of the damage dice's           */
   int wait_state;	    /* Wait state for bashed mobs	       */
   int skin_data[4];        /* for skinning of corpses. vnum what to load */
   int spec;                /* Special functions for mobs - Soli, 8/99 */
   int backstabbed;         /* Been backstabbed yet?                   */
   int quest;
};


/* An affect structure.  Used in char_file_u *DO*NOT*CHANGE* */
struct affected_type {
   int type;             /* The type of spell that caused this      */
   sh_int duration;      /* For how long its effects will last      */
   sbyte modifier;       /* This is added to apropriate ability     */
   byte location;        /* Tells which ability to change(APPLY_XXX)*/
   long	bitvector;       /* Tells which bits to set (AFF_XXX)       */

   struct affected_type *next;
};


/* Structure used for chars following other chars */
struct follow_type {
   struct char_data *follower;
   struct follow_type *next;
};


/* ================== Structure for player/non-player ===================== */
struct char_data {
   int pfilepos;			 /* playerfile pos		  */
   mob_rnum nr;                          /* Mob's rnum			  */
   room_rnum in_room;                    /* Location (real room number)	  */
   room_rnum was_in_room;		 /* location for linkdead people  */

   struct char_player_data player;       /* Normal data                   */
   struct char_ability_data real_abils;	 /* Abilities without modifiers   */
   struct char_ability_data aff_abils;	 /* Abils with spells/stones/etc  */
   struct char_point_data points;        /* Points                        */
   struct char_special_data char_specials;	/* PC/NPC specials	  */
   struct player_special_data *player_specials; /* PC specials		  */
   struct mob_special_data mob_specials;	/* NPC specials		  */

   struct affected_type *affected;       /* affected by what spells       */
   struct obj_data *equipment[NUM_WEARS];/* Equipment array               */

   struct obj_data *carrying;            /* Head of list                  */
   struct descriptor_data *desc;         /* NULL for mobiles              */

   long id;                            /* used by DG triggers             */
   struct trig_proto_list *proto_script; /* list of default triggers      */
   struct script_data *script;         /* script info for the object      */

   struct char_data *next_in_room;     /* For room->people - list         */
   struct char_data *next;             /* For either monster or ppl-list  */
   struct char_data *next_fighting;    /* For fighting list               */

   struct follow_type *followers;        /* List of chars followers       */
   struct char_data *master;             /* Who is char following?        */
   byte standcounter;                    /*  Used for the mem system      */
   int reflection_ticks;
   int exchange;
};
/* ====================================================================== */


/* ==================== File Structure for Player ======================= */
/*             BEWARE: Changing it will ruin the playerfile		  */
struct char_file_u {
   /* char_player_data */
   char	name[MAX_NAME_LENGTH+1];
   char	alias[MAX_NAME_LENGTH+1];
   char	description[EXDSCR_LENGTH];

   /*  PDH 11/25/98 - saving char short description  */
   char short_description[LINE_DSCR_LENGTH];

   char	title[MAX_TITLE_LENGTH+1];
   char poofin[POOF_LENGTH + 1];		/* poof saving		*/
   char poofout[POOF_LENGTH + 1];		/* poof saving		*/
   byte sex;
   byte class;
   byte race; 
   byte level;
   sh_int hometown;
   time_t birth;   /* Time of birth of character     */
   int	played;    /* Number of secs played in total */
   ubyte weight;
   ubyte height;

   char	pwd[MAX_PWD_LENGTH+1];    /* character's password */

   struct char_special_data_saved char_specials_saved;
   struct player_special_data_saved player_specials_saved;
   struct char_ability_data abilities;
   struct char_point_data points;
   struct affected_type affected[MAX_AFFECT];

   time_t last_logon;		/* Time (in secs) of last logon */
   char host[HOST_LENGTH+1];	/* host of last logon */
};
/* ====================================================================== */


/* descriptor-related structures ******************************************/


struct txt_block {
   char	*text;
   int aliased;
   struct txt_block *next;
};


struct txt_q {
   struct txt_block *head;
   struct txt_block *tail;
};


struct descriptor_data {
   socket_t	descriptor;	/* file descriptor for socket		*/
   char	host[HOST_LENGTH+1];	/* hostname				*/
   byte close_me;               /* flag: this desc. should be closed    */
   byte	bad_pws;		/* number of bad pw attemps this login	*/
   byte idle_tics;		/* tics idle at password prompt		*/
   int	connected;		/* mode of 'connectedness'		*/
   int	wait;			/* wait for how many loops		*/
   int	desc_num;		/* unique num assigned to desc		*/
   time_t login_time;		/* when the person connected		*/
   char *showstr_head;		/* for keeping track of an internal str	*/
   char **showstr_vector;	/* for paging through texts		*/
   int  showstr_count;		/* number of pages to page through	*/
   int  showstr_page;		/* which page are we currently showing?	*/
   char	**str;			/* for the modify-str system		*/
   size_t max_str;	        /*		-			*/
   char *backstr;		/* added for handling abort buffers	*/
   long	mail_to;		/* name for mail system			*/
   int	has_prompt;		/* is the user at a prompt?             */
   char	inbuf[MAX_RAW_INPUT_LENGTH];  /* buffer for raw input		*/
   char	last_input[MAX_INPUT_LENGTH]; /* the last input			*/
   char small_outbuf[SMALL_BUFSIZE];  /* standard output buffer		*/
   char *output;		/* ptr to the current output buffer	*/
   char **history;              /* History of commands, for ! mostly.   */
   int  history_pos;            /* Circular array position.             */
   int  bufptr;			/* ptr to end of current output		*/
   int	bufspace;		/* space left in the output buffer	*/
   struct txt_block *large_outbuf; /* ptr to large buffer, if we need it */
   struct txt_q input;		/* q of unprocessed input		*/
   struct char_data *character;	/* linked to char			*/
   struct char_data *original;	/* original char if switched		*/
   struct descriptor_data *snooping; /* Who is this char snooping	*/
   struct descriptor_data *snoop_by; /* And who is snooping this char	*/
   struct descriptor_data *next; /* link to next descriptor		*/
   struct olc_data *olc;	     /*. OLC info - defined in olc.h   .*/
   struct char_data *marked;    /* Character/Mob being marked for backstab */
   int marked_time;    /* Character/Mob being marked for backstab */
};


/* other miscellaneous structures ***************************************/

#define SHOUT_AREA_MAX     6
struct same_zone {

  char *name;

  int z[SHOUT_AREA_MAX];
};

struct casting_type
{
  int num_spell_levels;
  int starting_spell_level;
  int slotmod;
};

struct msg_type {
   char	*attacker_msg;  /* message to attacker */
   char	*victim_msg;    /* message to victim   */
   char	*room_msg;      /* message to room     */
};


struct message_type {
   struct msg_type die_msg;	/* messages when death			*/
   struct msg_type miss_msg;	/* messages when miss			*/
   struct msg_type hit_msg;	/* messages when hit			*/
   struct msg_type god_msg;	/* messages when hit on god		*/
   struct message_type *next;	/* to next messages of this kind.	*/
};


struct message_list {
   int	a_type;			/* Attack type				*/
   int	number_of_attacks;	/* How many attack messages to chose from. */
   struct message_type *msg;	/* List of messages.			*/
};


struct dex_skill_type {
   sh_int p_pocket;
   sh_int p_locks;
   sh_int traps;
   sh_int sneak;
   sh_int hide;
};


struct dex_app_type {
   sh_int reaction;
   sh_int miss_att;
   sh_int defensive;
};


struct str_app_type {
   sh_int tohit;    /* To Hit (THAC0) Bonus/Penalty        */
   sh_int todam;    /* Damage Bonus/Penalty                */
   sh_int carry_w;  /* Maximum weight that can be carrried */
   sh_int wield_w;  /* Maximum weight that can be wielded  */
};


struct wis_app_type {
   byte bonus;       /* how many practices player gains per lev */
};


struct int_app_type {
   byte learn;       /* how many % a player learns a spell/skill */
};


struct con_app_type {
   sh_int hitp;
   sh_int shock;
};


struct weather_data {
   int	pressure;	/* How is the pressure ( Mb ) */
   int	change;	/* How fast and what way does it change. */
   int	sky;	/* How is the sky. */
   int	sunlight;	/* And how much sun. */
};


struct title_type {
   char	*title_m;
   char	*title_f;
   int	exp;
};


/* element in monster and object index-tables   */
struct index_data {
   int	virtual;    /* virtual number of this mob/obj           */
   int	number;     /* number of existing units of this mob/obj	*/
   SPECIAL(*func);

   char *farg;         /* string argument for special function     */
   struct trig_data *proto;     /* for triggers... the trigger     */
};

struct function_data {
  SPECIAL(*func);
};

/* linked list for mob/object prototype trigger lists */
struct trig_proto_list {
  int vnum;                             /* vnum of the trigger   */
  struct trig_proto_list *next;         /* next trigger          */
};

typedef enum mdefault_fields {
  mdefault_exp,
  mdefault_thaco,
  mdefault_ac,
  mdefault_dam_nodice,
  mdefault_dam_sizedice,
  mdefault_dam_plus,
  mdefault_hit_nodice,
  mdefault_hit_sizedice,
  mdefault_hit_plus,
  mdefault_max,
} mdefault_fields;
