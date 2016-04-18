/* ************************************************************************

*   File: magic.c                                       Part of CircleMUD *

*  Usage: low-level functions for magic; spell template code              *

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
#include "spells.h"
#include "handler.h"
#include "db.h"

extern struct room_data *world;
extern struct obj_data *object_list;
extern struct obj_data *obj_proto;
extern struct char_data *character_list;
extern struct index_data *obj_index;
extern struct area_aff_data *area_spells_list;
extern struct spell_info_type spell_info[TOP_SPELL_DEFINE + 1];
extern struct weather_data weather_info;
extern struct descriptor_data *descriptor_list;
extern struct zone_data *zone_table;
extern int mini_mud;
extern int pk_allowed;
extern struct default_mobile_stats *mob_defaults;
extern char weapon_verbs[];
extern int *max_ac_applys;
extern struct apply_mod_defaults *apmd;

int allowNewFollower(struct char_data* ch, int maxFollowerAllowed);
void clearMemory(struct char_data * ch);
int mag_materials(struct char_data *ch, int item0, int extract, int position);
void damage(struct char_data * ch, struct char_data * victim, int damage, int weapontype);
void affect_from_char_II(struct char_data * ch, int skill, int type, int action);
void weight_change_object(struct obj_data * obj, int weight);
void add_follower(struct char_data * ch, struct char_data * leader);
int dice(int number, int size);
extern struct spell_info_type spell_info[];
struct char_data *read_mobile(int, int);

/*

 * Saving throws for:
 * MCTW
 *   PARA, ROD, PETRI, BREATH, SPELL
 *     Levels 0-40
 */

void lose_arm(struct char_data * ch);
const byte saving_throws[NUM_CLASSES][5][41] = {
  {				/* Knight */
		{90, 70, 68, 67, 65, 62, 58, 55, 53, 52, 50,	/* 0 - 10 */
/* PARA */	47, 43, 40, 38, 37, 35, 32, 28, 25, 24,		/* 11 - 20 */
		23, 22, 20, 19, 17, 16, 15, 14, 13, 12,		/* 21 - 30 */
		11, 10, 9, 8, 7, 6, 5, 4, 3, 2},		/* 31 - 40 */

		{90, 80, 78, 77, 75, 72, 68, 65, 63, 62, 60,	/* 0 - 10 */
/* ROD */	57, 53, 50, 48, 47, 45, 42, 38, 35, 34,		/* 11 - 20 */
		33, 32, 30, 29, 27, 26, 25, 24, 23, 22,		/* 21 - 30 */
		20, 18, 16, 14, 12, 10, 8, 6, 5, 4},		/* 31 - 40 */

		{90, 75, 73, 72, 70, 67, 63, 60, 58, 57, 55,	/* 0 - 10 */
/* PETRI */	52, 48, 45, 43, 42, 40, 37, 33, 30, 29,		/* 11 - 20 */
		28, 26, 25, 24, 23, 21, 20, 19, 18, 17,		/* 21 - 30 */
		16, 15, 14, 13, 12, 11, 10, 9, 8, 7},		/* 31 - 40 */

		{90, 85, 83, 82, 80, 75, 70, 65, 63, 62, 60,	/* 0 - 10 */
/* BREATH */	55, 50, 45, 43, 42, 40, 37, 33, 30, 29,		/* 11 - 20 */
		28, 26, 25, 24, 23, 21, 20, 19, 18, 17,		/* 21 - 30 */
		16, 15, 14, 13, 12, 11, 10, 9, 8, 7},		/* 31 - 40 */

		{90, 85, 83, 82, 80, 77, 73, 70, 68, 67, 65,	/* 0 - 10 */
/* SPELL */	62, 58, 55, 53, 52, 50, 47, 43, 40, 39,		/* 11 - 20 */
		38, 36, 35, 34, 33, 31, 30, 29, 28, 27,		/* 21 - 30 */
		25, 23, 21, 19, 17, 15, 13, 11, 9, 7}		/* 31 - 40 */
  },

  {				/* Paladin */
		{90, 70, 68, 67, 65, 62, 58, 55, 53, 52, 50,	/* 0 - 10 */
/* PARA */	47, 43, 40, 38, 37, 35, 32, 28, 25, 24,		/* 11 - 20 */
		23, 22, 20, 19, 17, 16, 15, 14, 13, 12,		/* 21 - 30 */
		11, 10, 9, 8, 7, 6, 5, 4, 3, 2},		/* 31 - 40 */

		{90, 80, 78, 77, 75, 72, 68, 65, 63, 62, 60,	/* 0 - 10 */
/* ROD */	57, 53, 50, 48, 47, 45, 42, 38, 35, 34,		/* 11 - 20 */
		33, 32, 30, 29, 27, 26, 25, 24, 23, 22,		/* 21 - 30 */
		20, 18, 16, 14, 12, 10, 8, 6, 5, 4},		/* 31 - 40 */

		{90, 75, 73, 72, 70, 67, 63, 60, 58, 57, 55,	/* 0 - 10 */
/* PETRI */	52, 48, 45, 43, 42, 40, 37, 33, 30, 29,		/* 11 - 20 */
		28, 26, 25, 24, 23, 21, 20, 19, 18, 17,		/* 21 - 30 */
		16, 15, 14, 13, 12, 11, 10, 9, 8, 7},		/* 31 - 40 */

		{90, 85, 83, 82, 80, 75, 70, 65, 63, 62, 60,	/* 0 - 10 */
/* BREATH */	55, 50, 45, 43, 42, 40, 37, 33, 30, 29,		/* 11 - 20 */
		28, 26, 25, 24, 23, 21, 20, 19, 18, 17,		/* 21 - 30 */
		16, 15, 14, 13, 12, 11, 10, 9, 8, 7},		/* 31 - 40 */

		{90, 85, 83, 82, 80, 77, 73, 70, 68, 67, 65,	/* 0 - 10 */
/* SPELL */	62, 58, 55, 53, 52, 50, 47, 43, 40, 39,		/* 11 - 20 */
		38, 36, 35, 34, 33, 31, 30, 29, 28, 27,		/* 21 - 30 */
		25, 23, 21, 19, 17, 15, 13, 11, 9, 7}		/* 31 - 40 */
  },

  {				/* Mages */
		{90, 70, 69, 68, 67, 66, 65, 63, 61, 60, 59,	/* 0 - 10 */
/* PARA */	57, 55, 54, 53, 53, 52, 51, 50, 48, 46,		/* 11 - 20 */
		45, 44, 42, 40, 38, 36, 34, 32, 30, 28,		/* 21 - 30 */
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0},			/* 31 - 40 */

		{90, 55, 53, 51, 49, 47, 45, 43, 41, 40, 39,	/* 0 - 10 */
/* ROD */	37, 35, 33, 31, 30, 29, 27, 25, 23, 21,		/* 11 - 20 */
		20, 19, 17, 15, 14, 13, 12, 11, 10, 9,		/* 21 - 30 */
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0},			/* 31 - 40 */

		{90, 65, 63, 61, 59, 57, 55, 53, 51, 50, 49,	/* 0 - 10 */
/* PETRI */	47, 45, 43, 41, 40, 39, 37, 35, 33, 31,		/* 11 - 20 */
		30, 29, 27, 25, 23, 21, 19, 17, 15, 13,		/* 21 - 30 */
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0},			/* 31 - 40 */

		{90, 75, 73, 71, 69, 67, 65, 63, 61, 60, 59,	/* 0 - 10 */
/* BREATH */	57, 55, 53, 51, 50, 49, 47, 45, 43, 41,		/* 11 - 20 */
		40, 39, 37, 35, 33, 31, 29, 27, 25, 23,		/* 21 - 30 */
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0},			/* 31 - 40 */

		{90, 60, 58, 56, 54, 52, 50, 48, 46, 45, 44,	/* 0 - 10 */
/* SPELL */	42, 40, 38, 36, 35, 34, 32, 30, 28, 26,		/* 11 - 20 */
		25, 24, 22, 20, 18, 16, 14, 12, 10, 8,		/* 21 - 30 */
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0},			/* 31 - 40 */
  },

  {				/* Clerics */
		{90, 50, 59, 48, 46, 45, 43, 40, 37, 35, 34,	/* 0 - 10 */
/* PARA */	33, 31, 30, 29, 27, 26, 25, 24, 23, 22,		/* 11 - 20 */
		21, 20, 18, 15, 14, 12, 10, 9, 8, 7,		/* 21 - 30 */
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0},			/* 31 - 40 */

		{90, 70, 69, 68, 66, 65, 63, 60, 57, 55, 54,	/* 0 - 10 */
/* ROD */	53, 51, 50, 49, 47, 46, 45, 44, 43, 42,		/* 11 - 20 */
		41, 40, 38, 35, 34, 32, 30, 29, 28, 27,		/* 21 - 30 */
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0},			/* 31 - 40 */

		{90, 65, 64, 63, 61, 60, 58, 55, 53, 50, 49,	/* 0 - 10 */
/* PETRI */	48, 46, 45, 44, 43, 41, 40, 39, 38, 37,		/* 11 - 20 */
		36, 35, 33, 31, 29, 27, 25, 24, 23, 22,		/* 21 - 30 */
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0},			/* 31 - 40 */

		{90, 80, 79, 78, 76, 75, 73, 70, 67, 65, 64,	/* 0 - 10 */
/* BREATH */	63, 61, 60, 59, 57, 56, 55, 54, 53, 52,		/* 11 - 20 */
		51, 50, 48, 45, 44, 42, 40, 39, 38, 37,		/* 21 - 30 */
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0},			/* 31 - 40 */

		{90, 75, 74, 73, 71, 70, 68, 65, 63, 60, 59,	/* 0 - 10 */
/* SPELL */	58, 56, 55, 54, 53, 51, 50, 49, 48, 47,		/* 11 - 20 */
		46, 45, 43, 41, 39, 37, 35, 34, 33, 32,		/* 21 - 30 */
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0}		/* 31 - 40 */
  },

  {				/* Ranger */
		{90, 70, 68, 67, 65, 62, 58, 55, 53, 52, 50,	/* 0 - 10 */
/* PARA */	47, 43, 40, 38, 37, 35, 32, 28, 25, 24,		/* 11 - 20 */
		23, 22, 20, 19, 17, 16, 15, 14, 13, 12,		/* 21 - 30 */
		11, 10, 9, 8, 7, 6, 5, 4, 3, 2},		/* 31 - 40 */

		{90, 80, 78, 77, 75, 72, 68, 65, 63, 62, 60,	/* 0 - 10 */
/* ROD */	57, 53, 50, 48, 47, 45, 42, 38, 35, 34,		/* 11 - 20 */
		33, 32, 30, 29, 27, 26, 25, 24, 23, 22,		/* 21 - 30 */
		20, 18, 16, 14, 12, 10, 8, 6, 5, 4},		/* 31 - 40 */

		{90, 75, 73, 72, 70, 67, 63, 60, 58, 57, 55,	/* 0 - 10 */
/* PETRI */	52, 48, 45, 43, 42, 40, 37, 33, 30, 29,		/* 11 - 20 */
		28, 26, 25, 24, 23, 21, 20, 19, 18, 17,		/* 21 - 30 */
		16, 15, 14, 13, 12, 11, 10, 9, 8, 7},		/* 31 - 40 */

		{90, 85, 83, 82, 80, 75, 70, 65, 63, 62, 60,	/* 0 - 10 */
/* BREATH */	55, 50, 45, 43, 42, 40, 37, 33, 30, 29,		/* 11 - 20 */
		28, 26, 25, 24, 23, 21, 20, 19, 18, 17,		/* 21 - 30 */
		16, 15, 14, 13, 12, 11, 10, 9, 8, 7},		/* 31 - 40 */

		{90, 85, 83, 82, 80, 77, 73, 70, 68, 67, 65,	/* 0 - 10 */
/* SPELL */	62, 58, 55, 53, 52, 50, 47, 43, 40, 39,		/* 11 - 20 */
		38, 36, 35, 34, 33, 31, 30, 29, 28, 27,		/* 21 - 30 */
		25, 23, 21, 19, 17, 15, 13, 11, 9, 7}		/* 31 - 40 */
  },

  {				/* Thieves */
		{90, 65, 64, 63, 62, 61, 60, 59, 58, 57, 56,	/* 0 - 10 */
/* PARA */	55, 54, 53, 52, 51, 50, 49, 48, 47, 46,		/* 11 - 20 */
		45, 44, 43, 42, 41, 40, 39, 38, 37, 36,		/* 21 - 30 */
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0},			/* 31 - 40 */

		{90, 70, 68, 66, 64, 62, 60, 58, 56, 54, 52,	/* 0 - 10 */
/* ROD */	50, 48, 46, 44, 42, 40, 38, 36, 34, 32,		/* 11 - 20 */
		30, 28, 26, 24, 22, 20, 18, 16, 14, 13,		/* 21 - 30 */
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0},			/* 31 - 40 */

		{90, 60, 59, 58, 58, 56, 55, 54, 53, 52, 51,	/* 0 - 10 */
/* PETRI */	50, 49, 48, 47, 46, 45, 44, 43, 42, 41,		/* 11 - 20 */
		40, 39, 38, 37, 36, 35, 34, 33, 32, 31,		/* 21 - 30 */
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0},			/* 31 - 40 */

		{90, 80, 79, 78, 77, 76, 75, 74, 73, 72, 71,	/* 0 - 10 */
/* BREATH */	70, 69, 68, 67, 66, 65, 64, 63, 62, 61,		/* 11 - 20 */
		60, 59, 58, 57, 56, 55, 54, 53, 52, 51,		/* 21 - 30 */
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0},			/* 31 - 40 */

		{90, 75, 73, 71, 69, 67, 65, 63, 61, 59, 57,	/* 0 - 10 */
/* SPELL */	55, 53, 51, 49, 47, 45, 43, 41, 39, 37,		/* 11 - 20 */
		35, 33, 31, 29, 27, 25, 23, 21, 19, 17,		/* 21 - 30 */
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0}			/* 31 - 40 */
  },

  {				/* Warriors */
		{90, 70, 68, 67, 65, 62, 58, 55, 53, 52, 50,	/* 0 - 10 */
/* PARA */	47, 43, 40, 38, 37, 35, 32, 28, 25, 24,		/* 11 - 20 */
		23, 22, 20, 19, 17, 16, 15, 14, 13, 12,		/* 21 - 30 */
		11, 10, 9, 8, 7, 6, 5, 4, 3, 2},		/* 31 - 40 */

		{90, 80, 78, 77, 75, 72, 68, 65, 63, 62, 60,	/* 0 - 10 */
/* ROD */	57, 53, 50, 48, 47, 45, 42, 38, 35, 34,		/* 11 - 20 */
		33, 32, 30, 29, 27, 26, 25, 24, 23, 22,		/* 21 - 30 */
		20, 18, 16, 14, 12, 10, 8, 6, 5, 4},		/* 31 - 40 */

		{90, 75, 73, 72, 70, 67, 63, 60, 58, 57, 55,	/* 0 - 10 */
/* PETRI */	52, 48, 45, 43, 42, 40, 37, 33, 30, 29,		/* 11 - 20 */
		28, 26, 25, 24, 23, 21, 20, 19, 18, 17,		/* 21 - 30 */
		16, 15, 14, 13, 12, 11, 10, 9, 8, 7},		/* 31 - 40 */

		{90, 85, 83, 82, 80, 75, 70, 65, 63, 62, 60,	/* 0 - 10 */
/* BREATH */	55, 50, 45, 43, 42, 40, 37, 33, 30, 29,		/* 11 - 20 */
		28, 26, 25, 24, 23, 21, 20, 19, 18, 17,		/* 21 - 30 */
		16, 15, 14, 13, 12, 11, 10, 9, 8, 7},		/* 31 - 40 */

		{90, 85, 83, 82, 80, 77, 73, 70, 68, 67, 65,	/* 0 - 10 */
/* SPELL */	62, 58, 55, 53, 52, 50, 47, 43, 40, 39,		/* 11 - 20 */
		38, 36, 35, 34, 33, 31, 30, 29, 28, 27,		/* 21 - 30 */
		25, 23, 21, 19, 17, 15, 13, 11, 9, 7}		/* 31 - 40 */
  },

  {                             /* Adventurer */
   /* Paralysis */
    {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100,     /* 0 - 10 */
     100, 100, 100, 100, 100, 100, 100, 100, 100, 100,          /* 11 - 20 */
     100, 100, 100, 100, 100, 100, 100, 100, 100, 100,          /* 21 - 30 */
     100, 100, 100, 100, 100, 100, 100, 100, 100, 100},         /* 31 - 40 */

/* Rod */
    {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100,     /* 0 - 10 */
     100, 100, 100, 100, 100, 100, 100, 100, 100, 100,          /* 11 - 20 */
     100, 100, 100, 100, 100, 100, 100, 100, 100, 100,          /* 21 - 30 */
     100, 100, 100, 100, 100, 100, 100, 100, 100, 100},         /* 31 - 40 */

/* Petri */
    {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100,     /* 0 - 10 */
     100, 100, 100, 100, 100, 100, 100, 100, 100, 100,          /* 11 - 20 */
     100, 100, 100, 100, 100, 100, 100, 100, 100, 100,          /* 21 - 30 */
     100, 100, 100, 100, 100, 100, 100, 100, 100, 100},         /* 31 - 40 */

/* Breath */
    {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100,     /* 0 - 10 */
     100, 100, 100, 100, 100, 100, 100, 100, 100, 100,          /* 11 - 20 */
     100, 100, 100, 100, 100, 100, 100, 100, 100, 100,          /* 21 - 30 */
     100, 100, 100, 100, 100, 100, 100, 100, 100, 100},         /* 31 - 40 */

/* Spell */
    {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100,     /* 0 - 10 */
     100, 100, 100, 100, 100, 100, 100, 100, 100, 100,          /* 11 - 20 */
     100, 100, 100, 100, 100, 100, 100, 100, 100, 100,          /* 21 - 30 */
     100, 100, 100, 100, 100, 100, 100, 100, 100, 100},         /* 31 - 40 */
  },

  {				/* Druid */
		{90, 50, 59, 48, 46, 45, 43, 40, 37, 35, 34,	/* 0 - 10 */
/* PARA */	33, 31, 30, 29, 27, 26, 25, 24, 23, 22,		/* 11 - 20 */
		21, 20, 18, 15, 14, 12, 10, 9, 8, 7,		/* 21 - 30 */
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0},			/* 31 - 40 */

		{90, 70, 69, 68, 66, 65, 63, 60, 57, 55, 54,	/* 0 - 10 */
/* ROD */	53, 51, 50, 49, 47, 46, 45, 44, 43, 42,		/* 11 - 20 */
		41, 40, 38, 35, 34, 32, 30, 29, 28, 27,		/* 21 - 30 */
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0},			/* 31 - 40 */

		{90, 65, 64, 63, 61, 60, 58, 55, 53, 50, 49,	/* 0 - 10 */
/* PETRI */	48, 46, 45, 44, 43, 41, 40, 39, 38, 37,		/* 11 - 20 */
		36, 35, 33, 31, 29, 27, 25, 24, 23, 22,		/* 21 - 30 */
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0},			/* 31 - 40 */

		{90, 80, 79, 78, 76, 75, 73, 70, 67, 65, 64,	/* 0 - 10 */
/* BREATH */	63, 61, 60, 59, 57, 56, 55, 54, 53, 52,		/* 11 - 20 */
		51, 50, 48, 45, 44, 42, 40, 39, 38, 37,		/* 21 - 30 */
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0},			/* 31 - 40 */

		{90, 75, 74, 73, 71, 70, 68, 65, 63, 60, 59,	/* 0 - 10 */
/* SPELL */	58, 56, 55, 54, 53, 51, 50, 49, 48, 47,		/* 11 - 20 */
		46, 45, 43, 41, 39, 37, 35, 34, 33, 32,		/* 21 - 30 */
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0}		/* 31 - 40 */
  },

  {				/* Monks */
		{90, 50, 59, 48, 46, 45, 43, 40, 37, 35, 34,	/* 0 - 10 */
/* PARA */	33, 31, 30, 29, 27, 26, 25, 24, 23, 22,		/* 11 - 20 */
		21, 20, 18, 15, 14, 12, 10, 9, 8, 7,		/* 21 - 30 */
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0},			/* 31 - 40 */

		{90, 70, 69, 68, 66, 65, 63, 60, 57, 55, 54,	/* 0 - 10 */
/* ROD */	53, 51, 50, 49, 47, 46, 45, 44, 43, 42,		/* 11 - 20 */
		41, 40, 38, 35, 34, 32, 30, 29, 28, 27,		/* 21 - 30 */
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0},			/* 31 - 40 */

		{90, 65, 64, 63, 61, 60, 58, 55, 53, 50, 49,	/* 0 - 10 */
/* PETRI */	48, 46, 45, 44, 43, 41, 40, 39, 38, 37,		/* 11 - 20 */
		36, 35, 33, 31, 29, 27, 25, 24, 23, 22,		/* 21 - 30 */
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0},			/* 31 - 40 */

/* BREATH */
		{90, 80, 79, 78, 76, 75, 73, 70, 67, 65, 64,	/* 0 - 10 */
     	 63, 61, 60, 59, 57, 56, 55, 54, 53, 52,		/* 11 - 20 */
		 51, 50, 48, 45, 44, 42, 40, 39, 38, 37,		/* 21 - 30 */
		 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},			/* 31 - 40 */

/* SPELL */
		{90, 75, 74, 73, 71, 70, 68, 65, 63, 60, 59,	/* 0 - 10 */
    	 58, 56, 55, 54, 53, 51, 50, 49, 48, 47,		/* 11 - 20 */
		 46, 45, 43, 41, 39, 37, 35, 34, 33, 32,		/* 21 - 30 */
		 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}		/* 31 - 40 */
  },

  {                             /* Bard */
   /* Paralysis */
    {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100,     /* 0 - 10 */
     100, 100, 100, 100, 100, 100, 100, 100, 100, 100,          /* 11 - 20 */
     100, 100, 100, 100, 100, 100, 100, 100, 100, 100,          /* 21 - 30 */
     100, 100, 100, 100, 100, 100, 100, 100, 100, 100},         /* 31 - 40 */

/* Rod */
    {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100,     /* 0 - 10 */
     100, 100, 100, 100, 100, 100, 100, 100, 100, 100,          /* 11 - 20 */
     100, 100, 100, 100, 100, 100, 100, 100, 100, 100,          /* 21 - 30 */
     100, 100, 100, 100, 100, 100, 100, 100, 100, 100},         /* 31 - 40 */

/* Petri */
    {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100,     /* 0 - 10 */
     100, 100, 100, 100, 100, 100, 100, 100, 100, 100,          /* 11 - 20 */
     100, 100, 100, 100, 100, 100, 100, 100, 100, 100,          /* 21 - 30 */
     100, 100, 100, 100, 100, 100, 100, 100, 100, 100},         /* 31 - 40 */

/* Breath */
    {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100,     /* 0 - 10 */
     100, 100, 100, 100, 100, 100, 100, 100, 100, 100,          /* 11 - 20 */
     100, 100, 100, 100, 100, 100, 100, 100, 100, 100,          /* 21 - 30 */
     100, 100, 100, 100, 100, 100, 100, 100, 100, 100},         /* 31 - 40 */

/* Spell */
    {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100,     /* 0 - 10 */
     100, 100, 100, 100, 100, 100, 100, 100, 100, 100,          /* 11 - 20 */
     100, 100, 100, 100, 100, 100, 100, 100, 100, 100,          /* 21 - 30 */
     100, 100, 100, 100, 100, 100, 100, 100, 100, 100},         /* 31 - 40 */
  },

  {                             /* Sorcerer */
   /* Paralysis */
    {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100,     /* 0 - 10 */
     100, 100, 100, 100, 100, 100, 100, 100, 100, 100,          /* 11 - 20 */
     100, 100, 100, 100, 100, 100, 100, 100, 100, 100,          /* 21 - 30 */
     100, 100, 100, 100, 100, 100, 100, 100, 100, 100},         /* 31 - 40 */

/* Rod */
    {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100,     /* 0 - 10 */
     100, 100, 100, 100, 100, 100, 100, 100, 100, 100,          /* 11 - 20 */
     100, 100, 100, 100, 100, 100, 100, 100, 100, 100,          /* 21 - 30 */
     100, 100, 100, 100, 100, 100, 100, 100, 100, 100},         /* 31 - 40 */

/* Petri */
    {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100,     /* 0 - 10 */
     100, 100, 100, 100, 100, 100, 100, 100, 100, 100,          /* 11 - 20 */
     100, 100, 100, 100, 100, 100, 100, 100, 100, 100,          /* 21 - 30 */
     100, 100, 100, 100, 100, 100, 100, 100, 100, 100},         /* 31 - 40 */

/* Breath */
    {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100,     /* 0 - 10 */
     100, 100, 100, 100, 100, 100, 100, 100, 100, 100,          /* 11 - 20 */
     100, 100, 100, 100, 100, 100, 100, 100, 100, 100,          /* 21 - 30 */
     100, 100, 100, 100, 100, 100, 100, 100, 100, 100},         /* 31 - 40 */

/* Spell */
    {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100,     /* 0 - 10 */
     100, 100, 100, 100, 100, 100, 100, 100, 100, 100,          /* 11 - 20 */
     100, 100, 100, 100, 100, 100, 100, 100, 100, 100,          /* 21 - 30 */
     100, 100, 100, 100, 100, 100, 100, 100, 100, 100},         /* 31 - 40 */
  },

  {                             /* Filler 1 */
   /* Paralysis */
    {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100,     /* 0 - 10 */
     100, 100, 100, 100, 100, 100, 100, 100, 100, 100,          /* 11 - 20 */
     100, 100, 100, 100, 100, 100, 100, 100, 100, 100,          /* 21 - 30 */
     100, 100, 100, 100, 100, 100, 100, 100, 100, 100},         /* 31 - 40 */

/* Rod */
    {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100,     /* 0 - 10 */
     100, 100, 100, 100, 100, 100, 100, 100, 100, 100,          /* 11 - 20 */
     100, 100, 100, 100, 100, 100, 100, 100, 100, 100,          /* 21 - 30 */
     100, 100, 100, 100, 100, 100, 100, 100, 100, 100},         /* 31 - 40 */

/* Petri */
    {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100,     /* 0 - 10 */
     100, 100, 100, 100, 100, 100, 100, 100, 100, 100,          /* 11 - 20 */
     100, 100, 100, 100, 100, 100, 100, 100, 100, 100,          /* 21 - 30 */
     100, 100, 100, 100, 100, 100, 100, 100, 100, 100},         /* 31 - 40 */

/* Breath */
    {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100,     /* 0 - 10 */
     100, 100, 100, 100, 100, 100, 100, 100, 100, 100,          /* 11 - 20 */
     100, 100, 100, 100, 100, 100, 100, 100, 100, 100,          /* 21 - 30 */
     100, 100, 100, 100, 100, 100, 100, 100, 100, 100},         /* 31 - 40 */

/* Spell */
    {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100,     /* 0 - 10 */
     100, 100, 100, 100, 100, 100, 100, 100, 100, 100,          /* 11 - 20 */
     100, 100, 100, 100, 100, 100, 100, 100, 100, 100,          /* 21 - 30 */
     100, 100, 100, 100, 100, 100, 100, 100, 100, 100},         /* 31 - 40 */
  },

  {                             /* Filler 2 */
   /* Paralysis */
    {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100,     /* 0 - 10 */
     100, 100, 100, 100, 100, 100, 100, 100, 100, 100,          /* 11 - 20 */
     100, 100, 100, 100, 100, 100, 100, 100, 100, 100,          /* 21 - 30 */
     100, 100, 100, 100, 100, 100, 100, 100, 100, 100},         /* 31 - 40 */

/* Rod */
    {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100,     /* 0 - 10 */
     100, 100, 100, 100, 100, 100, 100, 100, 100, 100,          /* 11 - 20 */
     100, 100, 100, 100, 100, 100, 100, 100, 100, 100,          /* 21 - 30 */
     100, 100, 100, 100, 100, 100, 100, 100, 100, 100},         /* 31 - 40 */

/* Petri */
    {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100,     /* 0 - 10 */
     100, 100, 100, 100, 100, 100, 100, 100, 100, 100,          /* 11 - 20 */
     100, 100, 100, 100, 100, 100, 100, 100, 100, 100,          /* 21 - 30 */
     100, 100, 100, 100, 100, 100, 100, 100, 100, 100},         /* 31 - 40 */

/* Breath */
    {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100,     /* 0 - 10 */
     100, 100, 100, 100, 100, 100, 100, 100, 100, 100,          /* 11 - 20 */
     100, 100, 100, 100, 100, 100, 100, 100, 100, 100,          /* 21 - 30 */
     100, 100, 100, 100, 100, 100, 100, 100, 100, 100},         /* 31 - 40 */

/* Spell */
    {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100,     /* 0 - 10 */
     100, 100, 100, 100, 100, 100, 100, 100, 100, 100,          /* 11 - 20 */
     100, 100, 100, 100, 100, 100, 100, 100, 100, 100,          /* 21 - 30 */
     100, 100, 100, 100, 100, 100, 100, 100, 100, 100},         /* 31 - 40 */
  },

  {                             /* Filler 3 */
   /* Paralysis */
    {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100,     /* 0 - 10 */
     100, 100, 100, 100, 100, 100, 100, 100, 100, 100,          /* 11 - 20 */
     100, 100, 100, 100, 100, 100, 100, 100, 100, 100,          /* 21 - 30 */
     100, 100, 100, 100, 100, 100, 100, 100, 100, 100},         /* 31 - 40 */

/* Rod */
    {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100,     /* 0 - 10 */
     100, 100, 100, 100, 100, 100, 100, 100, 100, 100,          /* 11 - 20 */
     100, 100, 100, 100, 100, 100, 100, 100, 100, 100,          /* 21 - 30 */
     100, 100, 100, 100, 100, 100, 100, 100, 100, 100},         /* 31 - 40 */

/* Petri */
    {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100,     /* 0 - 10 */
     100, 100, 100, 100, 100, 100, 100, 100, 100, 100,          /* 11 - 20 */
     100, 100, 100, 100, 100, 100, 100, 100, 100, 100,          /* 21 - 30 */
     100, 100, 100, 100, 100, 100, 100, 100, 100, 100},         /* 31 - 40 */

/* Breath */
    {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100,     /* 0 - 10 */
     100, 100, 100, 100, 100, 100, 100, 100, 100, 100,          /* 11 - 20 */
     100, 100, 100, 100, 100, 100, 100, 100, 100, 100,          /* 21 - 30 */
     100, 100, 100, 100, 100, 100, 100, 100, 100, 100},         /* 31 - 40 */

/* Spell */
    {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100,     /* 0 - 10 */
     100, 100, 100, 100, 100, 100, 100, 100, 100, 100,          /* 11 - 20 */
     100, 100, 100, 100, 100, 100, 100, 100, 100, 100,          /* 21 - 30 */
     100, 100, 100, 100, 100, 100, 100, 100, 100, 100},         /* 31 - 40 */
  },
};

int mag_savingthrow_new(struct char_data * caster, struct char_data * victim, int aggressive, int type, int spell) {
    int victimSave = 0;
    int casterSkill = 0;
    int victimLevel = 0;
    int i = 0;

    if (aggressive) {
        victimLevel = GET_LEVEL(victim);
    } else {
      if (GET_SPELL_SOURCE(caster) == CAST_SPELL) {
          victimLevel = spell_info[spell].min_level[(int) GET_CLASS(caster)];
      } else {
          for (i = 0; i < NUM_CLASSES; i++) {
              victimLevel = MIN(victimLevel, spell_info[spell].min_level[i]);
          }
      }
  }

  victimSave = dice(1, 100 - saving_throws[(int)GET_CLASS(victim)][type][victimLevel]);

  if (GET_SPELL_SOURCE(caster) == CAST_SPELL)
    casterSkill = dice(1, GET_SKILL(caster, spell));
  else if (GET_SPELL_SOURCE(caster) == CAST_POTION)
    return 100;
  else
    casterSkill = dice(1, 100);

  victimSave += dice(victimLevel, 4);
  casterSkill += dice(GET_LEVEL(caster), 4);
  casterSkill *= 100;
  casterSkill /= victimSave;

  if (casterSkill < 50 && aggressive)
    return 0;
  else if (casterSkill < 100)
    return casterSkill;
  else if (casterSkill < 150)
    return 100;
  else if (casterSkill < 200) {
    send_to_char("Your spell has been cast at a slightly heightened power!\r\n", caster);
    return 110;
  }
  else if (casterSkill < 300) {
    send_to_char("Your spell has been cast at a moderately heightened power!\r\n", caster);
    return 125;
  }
  else if (casterSkill < 400) {
    send_to_char("Your spell has been cast at a greatly heightened power!\r\n", caster);
    return 150;
  }
  else if (casterSkill < 500) {
    send_to_char("Your spell has been cast at an incredibly heightened power!\r\n", caster);
    return 175;
  }
  else {
    send_to_char("Your spell has been cast at an unbelievably heightened power!\r\n", caster);
    return 200;
  }
}

int mag_savingthrow(struct char_data * ch, int type)
{
  int save;

  /* negative apply_saving_throw values make saving throws better! */
  if (IS_NPC(ch)) /* NPCs use warrior tables according to some book */
    save = saving_throws[(int)CLASS_WARRIOR][type][GET_LEVEL(ch)];
  else
    save = saving_throws[(int)GET_CLASS(ch)][type][GET_LEVEL(ch)];
    save += GET_SAVE(ch, type);

  /* throwing a 0 is always a failure */
  if (MAX(1, save) < number(0, 99))
    return TRUE;
  else
    return FALSE;
}

/* affect_update: called from comm.c (causes spells to wear off) */

void affect_update(void)
{
  static struct affected_type *af, *next;
  static struct char_data *i;

  for (i = character_list; i; i = i->next)
  {
    for (af = i->affected; af; af = next)
    {
      next = af->next;

      if (af->duration >= 1)
        af->duration--;

      else if (af->duration == -1)
        af->duration = -1;

      else
      {
        struct char_data* j = i->next;
        affect_remove(i, af, 1);

        if (i == (struct char_data*) NULL)
        {
           i = j;
           break;
        }
      }
    }   // End inner 'for'.
  } // End outer 'for'.
}



/*

 *  mag_materials:

 *  Checks for up to 3 vnums (spell reagents) in the player's inventory.

 *

 * No spells implemented in Circle use mag_materials, but you can use

 * it to implement your own spells which require ingredients (i.e., some

 * heal spell which requires a rare herb or some such.)

*/
int mag_materials(struct char_data *ch, int item0, int extract, int position) {
    struct obj_data *tobj;
    struct obj_data *obj0 = NULL;

    if (position == CMP_INV) {
        if (GET_EQ(ch, WEAR_HOLD)) {
            if ((item0 > 0) && (GET_OBJ_VNUM(GET_EQ(ch, WEAR_HOLD)) == item0)) {
                send_to_char("You need to have this component in your inventory!\r\n", ch);
            }
        }

        for (tobj = ch->carrying; tobj; tobj = tobj->next_content) {
            if ((item0 > 0) && (GET_OBJ_VNUM(tobj) == item0)) {
                obj0 = tobj;
                item0 = -1;
            }
        }
    } else if (position == CMP_HOLD) {

        for (tobj = ch->carrying; tobj; tobj = tobj->next_content) {
            if ((item0 > 0) && (GET_OBJ_VNUM(tobj) == item0)) {
            send_to_char("You need to hold this component in your hand!\r\n", ch);
            }
        }

        if (GET_EQ(ch, WEAR_HOLD)) {
            if ((item0 > 0) && (GET_OBJ_VNUM(GET_EQ(ch, WEAR_HOLD)) == item0)) {
                item0 = -1;

                if (extract == 1) {
                    obj0 = unequip_char(ch, WEAR_HOLD);
                    obj_to_char(obj0, ch);
                }
            }
        }
    }

    if (item0 > 0) {
        return (FALSE);
    }

    if (extract == 1) {
        if (item0 < 0) {
            act("$p crumbles to dust in your hands.", FALSE, ch, obj0, 0, TO_CHAR);
            act("$p crumbles to dust in $n's hands.", FALSE, ch, obj0, 0, TO_ROOM);
            extract_obj(obj0);
        }
    }

    return (TRUE);
}


void affect_update_pulsebased(struct char_data * ch)

{

  static struct affected_type *af, *next;

  int message[MAX_SKILLS + 1];





  for (af = ch->affected; af; af = next)

  {

    next = af->next;



    if (IS_SET(spell_info[af->type].routines, MAG_PULSE_BASED))

    {

      if (af->duration >= 1)

        af->duration--;

      else if (af->duration == -1)

        af->duration = -1;

      else

      {

        message[af->type]++;

        if (message[af->type] > 1)

          affect_remove(ch, af, 0);

        else

          affect_remove(ch, af, 1);

      }

    }

  }

}



/*

 * Every spell that does damage comes through here.  This calculates the

 * amount of damage, adds in any modifiers, determines what the saves are,

 * tests for save and calls damage().

 */



void mag_damage(int level, struct char_data * ch, struct char_data * victim, int spellnum, int savetype)
{
  int is_mage = 0, is_cleric = 0, is_druid = 0;
  int aggressive = TRUE;
  int dam = 0, loopThru = 1, i;
  int factor = 0;

  if (victim == NULL || ch == NULL)
    return;

  is_mage = (GET_CLASS(ch) == (CLASS_MAGE));
  is_cleric = (GET_CLASS(ch) == (CLASS_CLERIC | CLASS_DRUID));
  is_druid = (GET_CLASS(ch) == (CLASS_DRUID));

  switch (spellnum) 
  {
    case SPELL_MAGIC_MISSILE:
        dam = dice(2, 4);
        loopThru = ((level - 1) / 5) + 1;

        if (affected_by_spell(victim, SPELL_SHIELD))
            dam = 0;

        break;

    case SPELL_CHILL_TOUCH:	
        dam = dice(1, 8) + 4 + MIN(level, 8);
        break;

    case SPELL_BURNING_HANDS:
        dam = dice(6,6) + (level/2);
        /*dam = MIN(dice((level / 4) + 5, 4), 35);*/ /* 01-30-2000 */
        break;

    case SPELL_SHOCKING_GRASP:
        dam = dice(6,9) + level;
        /* dam = MIN(dice((level + 3), 8), 60); */ /* 01-30-2000 */
        break;

    case SPELL_VAMPIRIC_TOUCH:
        dam = MIN(dice((level / 6) + 3, 10), 50); 
        /* dam = MIN(dice((level + 3), 8), 60); */
        break;

    case SPELL_LIGHTNING_BOLT:
        dam = dice(level, 8) + (level);
        /* dam = MIN(dice((level + 4), 8), 70); */ /* 01-30-2000 */
        break;

    case SPELL_MOONBEAM:
    case SPELL_ARROW_OF_BONE:
        dam = MIN(dice((level / 4) + 4, 8), 70); 
        /* dam = MIN(dice((level + 4), 8), 70); */
        break;

    case SPELL_FIREBALL:
    case SPELL_PRISMATIC_SPRAY:
        dam = number(40, 70) + level;
        /* dam = MIN(dice((level + 5), 8), 100); */
        break;

    case SPELL_CONE_OF_COLD:
        dam = number(20, 60) + (2 * level);
        break;

    case SPELL_METEOR_SWARM:
        dam = dice(4, (2 * level));
        /* dam = MIN(dice((level + 6), 8), 150); */
        break;

    case SPELL_POWER_WORD_KILL:
        dam = dice(2, (level * 15));
        break;

        /* Mostly clerics */

    case SPELL_CAUSE_LIGHT:
        dam = dice(2, 4) + (level / 4);
        break;

    case SPELL_CAUSE_MODERATE:
        dam = dice(4, 4) + (level / 3);
        break;

    case SPELL_CAUSE_SEVERE:
        dam = dice(6, 4) + (level / 2);
        break;

    case SPELL_CAUSE_CRITIC:
        dam = dice(10, 4) + (level / 2);
        break;

    case SPELL_HARM:
        dam = dice(12, 4) + level;
        break;

    case SPELL_HARMFUL_WRATH:
        dam = dice(15, 8) + level;
        break;

    case SPELL_MAGICAL_STONE:
        if(is_cleric)
            dam = dice(1, 6) + (level / 4);
        else
            dam = dice(1, 4) + (level / 8);

        loopThru = MIN(5, (level + 4) / 5);

        break;

    case SPELL_FLAMESTRIKE:
        dam = dice(8, 4) + level;
        break;

    case SPELL_DISPEL_EVIL:
        dam = dice(6, 8) + 6;

        if (IS_EVIL(ch)) 
        {
            victim = ch;
            dam = MIN(GET_HIT(ch) - 1, 3 * dam / 2);
        } 
        else if (IS_GOOD(victim)) 
        {
            act("The gods protect $N.", FALSE, ch, 0, victim, TO_CHAR);
            dam = 0;
            return;
        }

        break;

    case SPELL_DISPEL_GOOD:
        dam = dice(6, 8) + 6;

        if (IS_GOOD(ch)) 
        {
            victim = ch;
            dam = MIN(GET_HIT(ch) - 1, 3 * dam / 2);
        } 
        else if (IS_EVIL(victim)) 
        {
            act("The gods protect $N.", FALSE, ch, 0, victim, TO_CHAR);
            dam = 0;
            return;
        }
        break;

    case SPELL_CALL_LIGHTNING:
        // if (weather_info.sky < SKY_RAINING)
        // {
        //     send_to_char("There is no storm to call it from right now.\r\n", ch);
        //     return;
        // }

        dam = dice(MIN(level, 15), 20);

        // if (weather_info.sky != SKY_LIGHTNING)
        //     dam >>= 1;

        break;

    case SPELL_SUNRAY:
        // if ((weather_info.sunlight == SUN_DARK) || (weather_info.sky > SKY_CLOUDY) || !OUTSIDE(ch))
        // {
        //     send_to_char("Your sunray fizzles before reaching the target.\r\n", ch);
        //     return;
        // }

        dam = dice(3, 20) + 90;

        if(is_druid)
        {
            dam = dice(6, 20) + 120;
        }

        // if (weather_info.sky > SKY_CLOUDLESS)
        //     dam /= 2;

        // if (weather_info.sunlight != SUN_LIGHT)
        //     dam /= 2;

        if (IS_UNDEAD(victim))
        {
            // if (GET_LEVEL(victim) < 20)
            //     dam *= 3;
            // else
            //     dam *= 2;
            dam *= 3;
        }

        break;

  } /* switch(spellnum) */

  if ((factor = mag_savingthrow_new(ch, victim, aggressive, savetype, spellnum)) < 100 )
  {
   dam = dam * factor / 100;

    switch (spellnum)
    {
      case SPELL_VAMPIRIC_TOUCH:

       if (dam == 0)
         break;
       else
         mag_points(level / 2, ch, ch, spellnum, savetype);
         break;

      default:
        break;
    }
  }
  else   // They blew the save - call added effects
  {
    dam = dam * factor / 100;

    switch (spellnum)
    {

      case SPELL_CONE_OF_COLD:
        mag_affects(level, ch, victim, spellnum, savetype);
        break;

      case SPELL_VAMPIRIC_TOUCH:
        mag_points(level, ch, ch, spellnum, savetype);
        break;

      default:
        break;
    }

  }

  for (i = 0; i < loopThru; i++) 
  {
    if (ch && victim)
    {
      /* Attempt at fixing Magic-Missile crash. --gmb 19991115 */
      /* Die() should not be called twice. */
      if (ch->in_room == victim->in_room)
      {
         if (GET_POS(victim) > POS_DEAD)
         { 
            damage(ch, victim, dam, spellnum);
         }
         else
         {
            sprintf(buf, "SYSERR: Gilean's trap worked: %s cast on %s and killed it.\n", ch->player.name, victim->player.name);
         }
      }
      else
      {
        sprintf(buf, "SYSERR: Gilean's trap worked: %s cast on %s and killed it.\n", ch->player.name, victim->player.name);
      }
    }
  }
}

/*
 * Every spell that does an affect comes through here.  This determines
 * the effect, whether it is added or replacement, whether it is legal or
 * not, etc.
 *
 * affect_join(vict, aff, add_dur, avg_dur, add_mod, avg_mod)
*/

#define MAX_SPELL_AFFECTS 6	/* change if more needed */

void mag_affects(int level, struct char_data * ch, struct char_data * victim, int spellnum, int savetype)
{
  struct affected_type af[MAX_SPELL_AFFECTS];
  struct char_data *mob = NULL;
  struct char_data *target = NULL, *next_target;

  int is_mage = FALSE, is_cleric = FALSE;
  int a = 0, thing, b = 0;
  int i, random, rand = 0;
  int factor = 0, aggressive = spell_info[spellnum].violent;

  bool accum_affect = FALSE, accum_duration = FALSE;
  char *to_vict = NULL, *to_room = NULL;

  if (victim == NULL || ch == NULL)
  {
    return;
  }

  if (GET_POS(victim) <= POS_DEAD) 
  {
    return;
  }

  is_mage = (GET_CLASS(ch) == (CLASS_MAGE));
  is_cleric = (GET_CLASS(ch) == (CLASS_CLERIC | CLASS_DRUID));

   for (i = 0; i < MAX_SPELL_AFFECTS; i++) 
   {
     af[i].type = spellnum;
     af[i].bitvector = 0;
     af[i].modifier = 0;
     af[i].location = APPLY_NONE;
  }

  if ((factor = mag_savingthrow_new(ch, victim, aggressive, savetype, spellnum)) < 50 )
  {
    if (!aggressive) 
    {
      send_to_char("You were unable to summon enough energy and concentration to complete the spell.\r\n", ch);
      return;
    }
  }


  while (affected_by_spell(victim, spellnum) && spell_info[spellnum].violent == FALSE)
    affect_from_char(victim, spellnum);

  switch (spellnum) 
  {
  case SPELL_SK_CHALLENGE:
    af[0].location = APPLY_HITROLL;
    af[0].modifier = (MAX(1, GET_LEVEL(ch) / 15));
    af[0].duration = GET_CHA(ch) / 3;
    af[0].bitvector = 0;

    af[1].location = APPLY_AC;
    af[1].modifier = -(GET_LEVEL(ch));
    af[1].duration = GET_CHA(ch) / 3;
    af[1].bitvector = 0;

    af[2].location = APPLY_DAMROLL;
    af[2].modifier = (MAX(1, GET_LEVEL(ch) / 15));
    af[2].duration = GET_CHA(ch) / 3;
    af[2].bitvector = 0;

    accum_duration = TRUE;

    to_vict = "The knightly challenge has boosted your morale.";
    return;

  case SPELL_CHILL_TOUCH:
    if (!affected_by_spell(victim,SPELL_CHILL_TOUCH) && factor > 49) 
    {
       af[0].location = APPLY_STR;
       af[0].duration = level + 3;
       af[0].modifier = -1;
    }

    break;

  case SPELL_POLYMORPH:
  if (PLR_FLAGGED(victim, PLR_RABBIT)   || PLR_FLAGGED(victim, PLR_BIRD) ||
      PLR_FLAGGED(victim, PLR_WOLF)     || PLR_FLAGGED(victim, PLR_BEAR) || 
      PLR_FLAGGED(victim, PLR_CAT))
  {
     return;
  }

   for (i = 0; i < NUM_WEARS; i++)

    if (GET_EQ(ch, i))
    {
        if (i == WEAR_WIELD) 
        {
            send_to_char("But you can't be wielding a weapon to cast this spell!\r\n", ch);
            return;
        }
    }

  thing = number(GET_LEVEL(ch)/2, GET_LEVEL(ch));

  thing += GET_STR(ch);



  free(ch->char_specials.name_dis);

 ch->char_specials.name_dis = (char*) NULL;

 free(ch->char_specials.desc_dis);

 ch->char_specials.desc_dis = (char*) NULL;

 free(ch->char_specials.ldesc_dis);

 ch->char_specials.ldesc_dis = (char*) NULL;





if (!IS_MAGE(ch)) {

    if (thing <= 15) 

   rand = 1;

    else if (thing <= 25) 

    rand = number(1, 2);

   else if (thing <= 25) 

     rand = number(1, 3);

  else if (thing <= 35)

     rand = number(1, 4);

  else 

    rand = number(1, 5);

}



else if (IS_MAGE(ch)) {

  if (IS_GOOD(ch))

    rand = number(6, 7);

   else if (IS_NEUTRAL(ch))

    rand = number(8, 9);

  else

      rand = number(9, 10);

}

switch (rand) {

case 1:

     sprintf(buf, "a small hummingbird");

     SET_BIT(PLR_FLAGS(victim), PLR_BIRD);

     SET_BIT(PRF_FLAGS(victim), PRF_NOTSELF);  



      af[0].duration = (GET_LEVEL(ch) / 2) + 5;

      af[0].modifier = 0;

      af[0].bitvector = AFF_FLIGHT;

      af[0].location = APPLY_NONE;

      af[0].type = SPELL_FLIGHT;



      af[1].type = SPELL_POLYMORPH;

      af[1].location = 0;

      af[1].modifier = 0;

      af[1].duration = (GET_LEVEL(ch) / 2) + 5;

      af[1].bitvector = AFF_INFRAVISION;



      af[2].duration = (GET_LEVEL(ch) / 2) + 5;

      af[2].location = APPLY_DEX;

      af[2].modifier = 1;

      af[2].bitvector = AFF_POLYMORPH;



      af[3].duration = (GET_LEVEL(ch) / 2) + 5;

      af[3].location = APPLY_STR;

      af[3].modifier = -(2+ (GET_LEVEL(ch) >= 30));

      af[3].bitvector = AFF_POLYMORPH;



      af[4].duration = (GET_LEVEL(ch) / 2) + 5;

      af[4].location = APPLY_AC;

      af[4].modifier = +10;

      af[4].bitvector = AFF_POLYMORPH;



       accum_affect = FALSE;

       accum_duration = FALSE;

 

 mob = read_mobile( 2319, VIRTUAL);

 char_to_room(mob, 0);

 ch->char_specials.name_dis = str_dup("a small hummingbird");

   free(ch->char_specials.desc_dis);

   ch->char_specials.desc_dis = str_dup("&yA small hummingbird constantly darts about here.&n");

break;

  case 2:

     sprintf(buf, "a fluffy-tailed white rabbit");

     SET_BIT(PLR_FLAGS(victim), PLR_RABBIT);

     SET_BIT(PRF_FLAGS(victim), PRF_NOTSELF);



      af[0].type = SPELL_HASTE;

      af[0].duration = (GET_LEVEL(ch) / 2) + 5;

      af[0].modifier = 0;

      af[0].bitvector = AFF_HASTE;

      af[0].location = APPLY_NONE;



      af[1].type = SPELL_POLYMORPH;

      af[1].location = 0;

      af[1].modifier = 0;

      af[1].duration = (GET_LEVEL(ch) / 2) + 5;

      af[1].bitvector = AFF_INFRAVISION;



      af[2].duration = (GET_LEVEL(ch) / 2) + 5;

      af[2].location = APPLY_DEX;

      af[2].modifier = 1;

      af[2].bitvector = AFF_POLYMORPH;



      af[3].duration = (GET_LEVEL(ch) / 2) + 5;

      af[3].location = APPLY_STR;

      af[3].modifier = -(2 + (GET_LEVEL(ch) >= 30));

      af[3].bitvector = AFF_POLYMORPH;



      af[4].duration = (GET_LEVEL(ch) / 2) + 5;

      af[4].location = APPLY_AC;

      af[4].modifier = +10;

      af[4].bitvector = AFF_POLYMORPH;



       accum_affect = FALSE;

       accum_duration = FALSE;



     mob = read_mobile(6001, VIRTUAL);

     char_to_room(mob, 0);

      ch->char_specials.name_dis = str_dup("a fluffy-tailed white rabbit");

       free(ch->char_specials.desc_dis);

    ch->char_specials.desc_dis = str_dup("&yA fluffy-tailed white rabbit hops along here.&n");

  break;

case 3:

     sprintf(buf, "a large black cat");

     SET_BIT(PLR_FLAGS(victim), PLR_CAT);

     SET_BIT(PRF_FLAGS(victim), PRF_NOTSELF);

      af[1].type = SPELL_POLYMORPH;

      af[1].location = 0;

      af[1].modifier = 0;

      af[1].duration = (GET_LEVEL(ch) / 2) + 5;

      af[1].bitvector = AFF_INFRAVISION;



      af[2].duration = (GET_LEVEL(ch) / 2) + 5;

      af[2].location = APPLY_DEX;

      af[2].modifier = 1;

      af[2].bitvector = AFF_POLYMORPH;



      af[3].duration = (GET_LEVEL(ch) / 2) + 5;

      af[3].location = APPLY_STR;

      af[3].modifier = -(1 + (GET_LEVEL(ch) >= 30));

      af[3].bitvector = AFF_POLYMORPH;



      af[4].duration = (GET_LEVEL(ch) / 2) + 5;

      af[4].location = APPLY_AC;

      af[4].modifier = +10;

      af[4].bitvector = AFF_POLYMORPH;



       accum_affect = FALSE;

       accum_duration = FALSE;

     

     mob = read_mobile( 6283, VIRTUAL);

     char_to_room(mob, 0);

      ch->char_specials.name_dis = str_dup("a large black cat");

      free(ch->char_specials.desc_dis);

     ch->char_specials.desc_dis = str_dup("&yA large black cat stretches lazily here.&n");

break;

case 4:

     sprintf(buf, "a large timber wolf");

     SET_BIT(PLR_FLAGS(victim), PLR_WOLF);

     SET_BIT(PRF_FLAGS(victim), PRF_NOTSELF);



      af[0].type = SPELL_HASTE;

      af[0].duration = (GET_LEVEL(ch) / 2) + 5;

      af[0].modifier = 0;

      af[0].bitvector = AFF_HASTE;

      af[0].location = APPLY_NONE;



      af[1].type = SPELL_POLYMORPH;

      af[1].location = 0;

      af[1].modifier = 0;

      af[1].duration = (GET_LEVEL(ch) / 2) + 5;

      af[1].bitvector = AFF_INFRAVISION;



      af[2].duration = (GET_LEVEL(ch) / 2) + 5;

      af[2].location = APPLY_HITROLL;

      af[2].modifier = 1;

      af[2].bitvector = AFF_POLYMORPH;



      af[3].duration = (GET_LEVEL(ch) / 2) + 5;

      af[3].location = APPLY_STR;

      af[3].modifier = -1;

      af[3].bitvector = AFF_POLYMORPH;



      af[4].duration = (GET_LEVEL(ch) / 2) + 5;

      af[4].location = APPLY_AC;

      af[4].modifier = -2;

      af[4].bitvector = AFF_POLYMORPH;



      accum_affect = FALSE;

    accum_duration = FALSE;



     mob = read_mobile( 4992, VIRTUAL);

     char_to_room(mob, 0);

      ch->char_specials.name_dis = str_dup("a sleek, grey and black timberwolf");

       free(ch->char_specials.desc_dis);

     ch->char_specials.desc_dis = str_dup("&yA  sleek, grey and black timberwolf paces around, its eyes searching the area restlessly.&n");

break;

case 5:

     sprintf(buf, "a black bear");

     SET_BIT(PLR_FLAGS(victim), PLR_BEAR);

     SET_BIT(PRF_FLAGS(victim), PRF_NOTSELF);



      af[0].type = SPELL_POLYMORPH;

      af[0].location = 0;

      af[0].modifier = 0;

      af[0].duration = (GET_LEVEL(ch) / 2) + 5;

      af[0].bitvector = AFF_INFRAVISION;



      af[1].location = APPLY_DEX;

      af[1].modifier = -(1 + (GET_LEVEL(ch) >= 30));

      af[1].duration = (GET_LEVEL(ch) / 2) + 5;

      af[1].bitvector = AFF_POLYMORPH;



      af[2].duration = (GET_LEVEL(ch) / 2) + 5;

      af[2].location = APPLY_AC;

      af[2].modifier = -3;

      af[2].bitvector = AFF_POLYMORPH;



      af[3].duration = (GET_LEVEL(ch) / 2) + 5;

      af[3].location = APPLY_STR;

      af[3].modifier = 1;

      af[3].bitvector = AFF_POLYMORPH;



     af[4].location = APPLY_DAMROLL;

      af[4].modifier = 1;

      af[4].duration = (GET_LEVEL(ch) / 2) + 5;

      af[4].bitvector = AFF_POLYMORPH;



    accum_affect = FALSE;

    accum_duration = FALSE;

      



    mob = read_mobile(79, VIRTUAL);

     char_to_room(mob, 0);

     ch->char_specials.name_dis = str_dup("a black bear");

       free(ch->char_specials.desc_dis);

    ch->char_specials.desc_dis = str_dup("&yA black bear rears up on its hind legs, sniffing the air.&n");

break;

case 6:

sprintf(buf, "a snow-white owl");

     SET_BIT(PLR_FLAGS(victim), PLR_BIRD);

     SET_BIT(PRF_FLAGS(victim), PRF_NOTSELF);  



      af[0].duration = (GET_LEVEL(ch) / 2) + 5;

      af[0].modifier = 0;

      af[0].bitvector = AFF_FLIGHT;

      af[0].location = APPLY_NONE;

      af[0].type = SPELL_FLIGHT;



      af[1].type = SPELL_POLYMORPH;

      af[1].location = 0;

      af[1].modifier = 0;

      af[1].duration = (GET_LEVEL(ch) / 2) + 5;

      af[1].bitvector = AFF_INFRAVISION;



      af[2].duration = (GET_LEVEL(ch) / 2) + 5;

      af[2].location = APPLY_MOVE;

      af[2].modifier = +50;

      af[2].bitvector = AFF_POLYMORPH;



      af[3].duration = (GET_LEVEL(ch) / 2) + 5;

      af[3].location = APPLY_STR;

      af[3].modifier = -(2+ (GET_LEVEL(ch) >= 30));

      af[3].bitvector = AFF_POLYMORPH;



      af[4].duration = (GET_LEVEL(ch) / 2) + 5;

      af[4].location = APPLY_AC;

      af[4].modifier = +10;

      af[4].bitvector = AFF_POLYMORPH;



       accum_affect = FALSE;

       accum_duration = FALSE;

  mob = read_mobile(6042, VIRTUAL);

  char_to_room(mob, 0);

  ch->char_specials.name_dis = str_dup("a snow-white owl");

       free(ch->char_specials.desc_dis);

    ch->char_specials.desc_dis = str_dup("&yA snow-white owl flies over the area, perhaps looking for prey.&n");

break;

case 7:

 sprintf(buf, "a large white cat");

     SET_BIT(PLR_FLAGS(victim), PLR_CAT);

     SET_BIT(PRF_FLAGS(victim), PRF_NOTSELF);

      af[1].type = SPELL_POLYMORPH;

      af[1].location = 0;

      af[1].modifier = 0;

      af[1].duration = (GET_LEVEL(ch) / 2) + 5;

      af[1].bitvector = AFF_INFRAVISION;



      af[2].duration = (GET_LEVEL(ch) / 2) + 5;

      af[2].location = APPLY_DEX;

      af[2].modifier = 1;

      af[2].bitvector = AFF_POLYMORPH;



      af[3].duration = (GET_LEVEL(ch) / 2) + 5;

      af[3].location = APPLY_STR;

      af[3].modifier = -(1 + (GET_LEVEL(ch) >= 30));

      af[3].bitvector = AFF_POLYMORPH;



      af[4].duration = (GET_LEVEL(ch) / 2) + 5;

      af[4].location = APPLY_AC;

      af[4].modifier = +10;

      af[4].bitvector = AFF_POLYMORPH;



       accum_affect = FALSE;

       accum_duration = FALSE;

      mob = read_mobile(3519, VIRTUAL);

     char_to_room(mob, 0);

      ch->char_specials.name_dis = str_dup("a large white cat");

      free(ch->char_specials.desc_dis);

     ch->char_specials.desc_dis = str_dup("&yA large white cat wanders the area, looking a bit sleepy.&n");

break;

case 8:

     sprintf(buf, "a large tabby");

     SET_BIT(PLR_FLAGS(victim), PLR_CAT);

     SET_BIT(PRF_FLAGS(victim), PRF_NOTSELF);

      af[1].type = SPELL_POLYMORPH;

      af[1].location = 0;

      af[1].modifier = 0;

      af[1].duration = (GET_LEVEL(ch) / 2) + 5;

      af[1].bitvector = AFF_INFRAVISION;



      af[2].duration = (GET_LEVEL(ch) / 2) + 5;

      af[2].location = APPLY_DEX;

      af[2].modifier = 1;

      af[2].bitvector = AFF_POLYMORPH;



      af[3].duration = (GET_LEVEL(ch) / 2) + 5;

      af[3].location = APPLY_STR;

      af[3].modifier = -(1 + (GET_LEVEL(ch) >= 30));

      af[3].bitvector = AFF_POLYMORPH;



      af[4].duration = (GET_LEVEL(ch) / 2) + 5;

      af[4].location = APPLY_AC;

      af[4].modifier = +10;

      af[4].bitvector = AFF_POLYMORPH;



       accum_affect = FALSE;

       accum_duration = FALSE;

      mob = read_mobile(4731, VIRTUAL);

     char_to_room(mob, 0);

      ch->char_specials.name_dis = str_dup("a large tabby");

      free(ch->char_specials.desc_dis);

     ch->char_specials.desc_dis = str_dup("&yA large red tabby pads quietly about the area, looking for prey.&n");

break;

case 9:

sprintf(buf, "a majestic falcon");

     SET_BIT(PLR_FLAGS(victim), PLR_BIRD);

     SET_BIT(PRF_FLAGS(victim), PRF_NOTSELF);  

      af[0].duration = (GET_LEVEL(ch) / 2) + 5;

      af[0].modifier = 0;

      af[0].bitvector = AFF_FLIGHT;

      af[0].location = APPLY_NONE;

      af[0].type = SPELL_FLIGHT;



      af[1].type = SPELL_POLYMORPH;

      af[1].location = 0;

      af[1].modifier = 0;

      af[1].duration = (GET_LEVEL(ch) / 2) + 5;

      af[1].bitvector = AFF_INFRAVISION;



      af[2].duration = (GET_LEVEL(ch) / 2) + 5;

      af[2].location = APPLY_MOVE;

      af[2].modifier = +30;

      af[2].bitvector = AFF_POLYMORPH;



      af[3].duration = (GET_LEVEL(ch) / 2) + 5;

      af[3].location = APPLY_STR;

      af[3].modifier = -(2+ (GET_LEVEL(ch) >= 30));

      af[3].bitvector = AFF_POLYMORPH;



      af[4].duration = (GET_LEVEL(ch) / 2) + 5;

      af[4].location = APPLY_AC;

      af[4].modifier = +10;

      af[4].bitvector = AFF_POLYMORPH;



       accum_affect = FALSE;

       accum_duration = FALSE;

  mob = read_mobile(13, VIRTUAL);

  char_to_room(mob, 0);

  ch->char_specials.name_dis = str_dup("a majestic falcon");

       free(ch->char_specials.desc_dis);

    ch->char_specials.desc_dis = str_dup("&yA beautiful bird majestically soars over the area.&n");

break;

case 10:

sprintf(buf, "a common crow");

     SET_BIT(PLR_FLAGS(victim), PLR_BIRD);

     SET_BIT(PRF_FLAGS(victim), PRF_NOTSELF);  

      af[0].duration = (GET_LEVEL(ch) / 2) + 5;

      af[0].modifier = 0;

      af[0].bitvector = AFF_FLIGHT;

      af[0].location = APPLY_NONE;

      af[0].type = SPELL_FLIGHT;



      af[1].type = SPELL_POLYMORPH;

      af[1].location = 0;

      af[1].modifier = 0;

      af[1].duration = (GET_LEVEL(ch) / 2) + 5;

      af[1].bitvector = AFF_INFRAVISION;



      af[2].duration = (GET_LEVEL(ch) / 2) + 5;

      af[2].location = APPLY_MOVE;

      af[2].modifier = +30;

      af[2].bitvector = AFF_POLYMORPH;



      af[3].duration = (GET_LEVEL(ch) / 2) + 5;

      af[3].location = APPLY_STR;

      af[3].modifier = -(2+ (GET_LEVEL(ch) >= 30));

      af[3].bitvector = AFF_POLYMORPH;



      af[4].duration = (GET_LEVEL(ch) / 2) + 5;

      af[4].location = APPLY_AC;

      af[4].modifier = +10;

      af[4].bitvector = AFF_POLYMORPH;



       accum_affect = FALSE;

       accum_duration = FALSE;



      mob = read_mobile(4124, VIRTUAL);

     char_to_room(mob, 0);

  ch->char_specials.name_dis = str_dup("a common crow");

       free(ch->char_specials.desc_dis);

    ch->char_specials.desc_dis = str_dup("&yA common crow flies overhead, looking for a place to perch.&n");

break;

case 11:

 sprintf(buf, "a black cat");

     SET_BIT(PLR_FLAGS(victim), PLR_CAT);

     SET_BIT(PRF_FLAGS(victim), PRF_NOTSELF);

      af[1].type = SPELL_POLYMORPH;

      af[1].location = 0;

      af[1].modifier = 0;

      af[1].duration = (GET_LEVEL(ch) / 2) + 5;

      af[1].bitvector = AFF_INFRAVISION;



      af[2].duration = (GET_LEVEL(ch) / 2) + 5;

      af[2].location = APPLY_DEX;

      af[2].modifier = 1;

      af[2].bitvector = AFF_POLYMORPH;



      af[3].duration = (GET_LEVEL(ch) / 2) + 5;

      af[3].location = APPLY_STR;

      af[3].modifier = -(1 + (GET_LEVEL(ch) >= 30));

      af[3].bitvector = AFF_POLYMORPH;



      af[4].duration = (GET_LEVEL(ch) / 2) + 5;

      af[4].location = APPLY_AC;

      af[4].modifier = +10;

      af[4].bitvector = AFF_POLYMORPH;



       accum_affect = FALSE;

       accum_duration = FALSE;



     mob = read_mobile(4125, VIRTUAL);

     char_to_room(mob, 0);

      ch->char_specials.name_dis = str_dup("a black cat");

      free(ch->char_specials.desc_dis);

     ch->char_specials.desc_dis = str_dup("&yA large cat the color of midnight prowls the area here.&n");

break;

          }

ch->char_specials.ldesc_dis = str_dup(mob->player.description);

ch->char_specials.sex_dis = mob->player.sex;

extract_char(mob);



for (i = 0; i < NUM_WEARS; i++)

  if (GET_EQ(ch, i)){

    GET_OBJ_DISGUISE(GET_EQ(ch, i)) = 2;

  }



  for (target = world[ch->in_room].people; target; target = next_target) {

    next_target = target->next_in_room;

    if (target == ch) {
        sprintf(buf2, "You have transformed yourself into %s!", buf);
        send_to_char(buf2, ch);
    } else {
        GET_NAME_II(ch, target, chname);
        sprintf(buf2, "%s has transformed $mself into a %s!", chname, buf);
        FREE_NAME(chname);
        act(buf2, FALSE, ch, 0, target, TO_VICT);
    }
  }

  break;

  case SPELL_ARMOR:
    if (affected_by_spell(victim, SPELL_BARKSKIN) ||
        affected_by_spell(victim, SPELL_MAGIC_VEST))
    {
      send_to_char("Nothing seems to happen.\r\n", ch);
      return;
    }

    af[0].location = APPLY_AC;
    af[0].modifier = -10;
    af[0].duration = 24;
    accum_duration = FALSE;
    to_vict = "You feel someone protecting you.";

    break;

  case SPELL_AID:
    af[2].location = APPLY_HIT;
    af[2].modifier = dice(10, 2) + 10;
    af[2].duration = 6;

    /*  Aid also affects with bless - therefore, no break for this case  */
  case SPELL_BLESS:
    if (affected_by_spell(victim, SPELL_THORNFLESH) ||
       (affected_by_spell(victim, SPELL_DIVINE_WRATH)) ||
       (affected_by_spell(victim, SPELL_BLESS)))
    {
      send_to_char(NOEFFECT, ch);
      return;
    }

    af[0].type = SPELL_BLESS;
    af[0].location = APPLY_HITROLL;
    af[0].modifier = 1;
    af[0].duration = 6;

    af[1].type = SPELL_BLESS;   // necessary due to aid
    af[1].location = APPLY_SAVING_SPELL;
    af[1].modifier = -1;
    af[1].duration = 6;

    accum_duration = FALSE;
    to_vict = "You feel righteous.";

    break;

  case SPELL_BLINDNESS:

    if (MOB_FLAGGED(victim,MOB_NOBLIND) || factor < 50) {

      if (GET_POS(victim) != POS_FIGHTING && IS_NPC(victim))

        hit(victim, ch, TYPE_UNDEFINED);

      return;

    }



    af[0].location = APPLY_HITROLL;

    af[0].modifier = -4;

    af[0].duration = 2;

    af[0].bitvector = AFF_BLIND;



    af[1].location = APPLY_AC;

    af[1].modifier = 40;

    af[1].duration = 2;

    af[1].bitvector = AFF_BLIND;



    to_room = "$n seems to be blinded!";

    to_vict = "You have been blinded!";

    break;

  

  case SPELL_BESTOW_CURSE:



    if (factor < 50)

    {

      if (IS_NPC(victim) && AWAKE(victim) && !FIGHTING(victim))

        hit(victim, ch, TYPE_UNDEFINED);

      return;

    }



    if (IS_AFFECTED(victim, AFF_CURSE))

    {

      send_to_char(NOEFFECT, ch);

      return;

    }



    random = number(1, 5);

    af[0].duration = 215;

    af[0].bitvector = AFF_CURSE;



    switch (random)

    {

      case 1:

        af[0].location = APPLY_HITROLL;

        af[0].modifier = -(2 + (GET_LEVEL(ch) / 15));

        break;

      case 2:

        af[0].location = APPLY_DAMROLL;

        af[0].modifier = -(1 + (GET_LEVEL(ch) / 15));

        break;

      case 3:

        af[0].location = APPLY_STR;

        af[0].modifier = -(1 + (GET_LEVEL(ch) >= 30));

        break;

      case 4:

        af[0].location = APPLY_DEX;

        af[0].modifier = -(1 + (GET_LEVEL(ch) >= 30));

        break;

      case 5:

        af[0].location = APPLY_CON;

        af[0].modifier = -(1 + (GET_LEVEL(ch) >= 30));

        break;

    }



    to_room = "$n briefly glows red!";

    to_vict = "You feel very uncomfortable.";

    break;



  case SPELL_DETECT_ALIGN:

    af[0].duration = 12 + level;

    af[0].bitvector = AFF_DETECT_ALIGN;

    accum_duration = FALSE;

    to_vict = "Your eyes tingle.";

    break;



  case SPELL_CONCEAL_ALIGN:

    af[0].duration = 12 + (GET_LEVEL(ch) >> 2);

    af[0].bitvector = AFF_CONCEAL_ALIGN;

    accum_duration = FALSE;

    to_vict = "The hairs on the back of your neck stand up.";

    break;



  case SPELL_DETECT_INVIS:

    af[0].duration = 12 + level;

    af[0].bitvector = AFF_DETECT_INVIS;

    accum_duration = FALSE;

    to_vict = "Your eyes tingle.";

    break;



  case SPELL_DETECT_MAGIC:

    af[0].duration = 12 + level;

    af[0].bitvector = AFF_DETECT_MAGIC;

    accum_duration = FALSE;

    to_vict = "Your eyes tingle.";

    break;



  case SPELL_INFRAVISION:

    af[0].duration = 12 + level;

    af[0].bitvector = AFF_INFRAVISION;

    accum_duration = FALSE;

    to_vict = "Your eyes glow red.";

    to_room = "$n's eyes glow red.";

    break;



  case SPELL_INVISIBILITY:

    if (!victim)

      victim = ch;



    af[0].duration = 12 + (GET_LEVEL(ch) >> 2);

    af[0].modifier = -40;

    af[0].location = APPLY_AC;

    af[0].bitvector = AFF_INVISIBLE;

    accum_duration = FALSE;

    to_vict = "You vanish.";

    to_room = "$n slowly fades out of existence.";

    break;



  case SPELL_POISON:

    if (factor < 50) {

      if (IS_NPC(victim) && AWAKE(victim) && !FIGHTING(victim) && victim != ch)

        hit(victim, ch, TYPE_UNDEFINED);

      return;

    }



    af[0].location = APPLY_STR;

    af[0].duration = GET_LEVEL(ch);

    af[0].modifier = -2;

    af[0].bitvector = AFF_POISON;

    to_vict = "You feel very sick.";

    to_room = "$n gets violently ill.";

    accum_affect = TRUE;

    break;



  case SPELL_PROT_FROM_EVIL:

    af[0].type = SPELL_PROT_FROM_EVIL;

    af[0].bitvector = AFF_PROTECT_EVIL; 

    af[0].duration = 24;

    af[0].location = APPLY_AC;

    af[0].modifier = -10;

    

    af[1].type = SPELL_PROT_FROM_EVIL;

    af[1].location = APPLY_SAVING_SPELL;

    af[1].modifier = -3;

    af[1].duration = 24;



    to_vict = "You feel protected from evil.";

    break;



  case SPELL_PROT_FROM_GOOD:

    af[0].type = SPELL_PROT_FROM_GOOD;

    af[0].bitvector = AFF_PROTECT_GOOD; 

    af[0].duration = 24;

    af[0].location = APPLY_AC;

    af[0].modifier = -10;



    af[1].type = SPELL_PROT_FROM_GOOD;

    af[1].location = APPLY_SAVING_SPELL;

    af[1].modifier = -3;

    af[1].duration = 24;



    to_vict = "You feel protected from good.";

    break;



  case SPELL_PROT_FROM_FIRE:

    af[0].type = SPELL_PROT_FROM_FIRE;

    af[0].duration = 24;

    af[0].location = APPLY_AC;

    af[0].modifier = -10;



    to_vict = "You feel protected from fire.";

    break;



  case SPELL_PROT_FROM_FROST:

    af[0].type = SPELL_PROT_FROM_FROST;

    af[0].duration = 24;

    af[0].location = APPLY_AC;

    af[0].modifier = -10;

    

    to_vict = "You feel protected from cold.";

    break;



  case SPELL_PROT_FROM_ELEMENTS:

    af[0].duration = 24;

    af[0].location = APPLY_AC;

    af[0].modifier = 0;

    to_vict = "You feel protected from the elements.";

    break;



  case SPELL_PROT_FROM_UNDEAD:

    af[0].type = SPELL_PROT_FROM_UNDEAD;

    af[0].duration = 24;

    af[0].location = APPLY_AC;

    af[0].modifier = -10;



    af[1].type = SPELL_PROT_FROM_UNDEAD;

    af[1].location = APPLY_SAVING_SPELL;

    af[1].modifier = -3;

    af[1].duration = 24;

    to_vict = "You feel protected from undead beings.";

    break;



  case SPELL_SANCTUARY:

    if (affected_by_spell(victim, SPELL_MINOR_SANCTUARY) ||

        affected_by_spell(victim, SPELL_FIRE_SHIELD) ||

        affected_by_spell(victim, SPELL_THORNSHIELD))

    {

      send_to_char(NOEFFECT, ch);

      return;

    }



    af[0].duration = 4;

    af[0].bitvector = AFF_SANCTUARY;

    to_vict = "A white aura momentarily surrounds you.";

    to_room = "$n is surrounded by a white aura.";

    break;



  case SPELL_MINOR_SANCTUARY:

    if (affected_by_spell(victim, SPELL_SANCTUARY) ||

        affected_by_spell(victim, SPELL_FIRE_SHIELD) ||

        affected_by_spell(victim, SPELL_THORNSHIELD))

    {

      send_to_char(NOEFFECT, ch);

      return;

    }



    af[0].duration = 4;

    af[0].location = APPLY_AC;

    af[0].modifier = 0;

    to_vict = "A faint white aura momentarily surrounds you.";

    to_room = "$n is surrounded by a faint white aura.";

    break;



  case SPELL_SLEEP:
    if (MOB_FLAGGED(victim, MOB_NOSLEEP) || factor < 50 || affected_by_spell(victim, SPELL_IMPERVIOUS_MIND))
    {
      if (IS_NPC(victim) && AWAKE(victim) && !FIGHTING(victim))
        hit(victim, ch, TYPE_UNDEFINED);
      return;
    }

    af[0].duration = 4 + (GET_LEVEL(ch) >> 2);
    af[0].bitvector = AFF_SLEEP;

    if (GET_POS(victim) > POS_SLEEPING) {
      act("You feel very sleepy...  Zzzz......", FALSE, victim, 0, 0, TO_CHAR);
      act("$n goes to sleep.", TRUE, victim, 0, 0, TO_ROOM);
      GET_POS(victim) = POS_SLEEPING;
    }

    break;

  case SPELL_STRENGTH:

    af[0].location = APPLY_STR;
    af[0].duration = (GET_LEVEL(ch) >> 1) + 4;

    if ( is_cleric ) {
      af[0].modifier = 1 + (level / 10);
    } else {
      af[0].modifier = 1 + (level > 18);
    }

    if (GET_STR(victim) < 16)
      af[0].modifier = MAX(af[0].modifier, 16 - GET_STR(victim));

    accum_duration = FALSE;
    accum_affect = FALSE;

    to_vict = "You feel stronger.";

    break;

  case SPELL_ENHANCE_AGILITY:

    af[0].location = APPLY_DEX;
    af[0].duration = (GET_LEVEL(ch) >> 1) + 4;

    if ( is_cleric ) {
      af[0].modifier = 1 + (level / 10);
    } else {
      af[0].modifier = 1 + (level > 18);
    }

    if (GET_DEX(victim) < 16)
      af[0].modifier = MAX(af[0].modifier, 16 - GET_DEX(victim));

    accum_duration = FALSE;
    accum_affect = FALSE;

    to_vict = "You feel more agile.";

    break;

  case SPELL_ENHANCE_ENDURANCE:

    af[0].location = APPLY_CON;
    af[0].duration = (GET_LEVEL(ch) >> 1) + 4;

    if ( is_cleric ) {
      af[0].modifier = 1 + (level / 10);
    } else {
      af[0].modifier = 1 + (level > 18);
    }

    if (GET_CON(victim) < 16)
      af[0].modifier = MAX(af[0].modifier, 16 - GET_CON(victim));

    accum_duration = FALSE;
    accum_affect = FALSE;

    to_vict = "You feel healthier and more energetic.";

    break;

  case SPELL_ENHANCE_INSIGHT:

    af[0].location = APPLY_WIS;
    af[0].duration = (GET_LEVEL(ch) >> 1) + 4;

    if ( is_cleric ) {
      af[0].modifier = 1 + (level / 10);
    } else {
      af[0].modifier = 1 + (level > 18);
    }

    if (GET_WIS(victim) < 16)
      af[0].modifier = MAX(af[0].modifier, 16 - GET_WIS(victim));

    accum_duration = FALSE;
    accum_affect = FALSE;

    to_vict = "You feel more insightful.";

    break;

  case SPELL_ENHANCE_CUNNING:

    af[0].location = APPLY_INT;
    af[0].duration = (GET_LEVEL(ch) >> 1) + 4;

    if ( is_cleric ) {
      af[0].modifier = 1 + (level / 10);
    } else {
      af[0].modifier = 1 + (level > 18);
    }

    if (GET_INT(victim) < 16)
      af[0].modifier = MAX(af[0].modifier, 16 - GET_INT(victim));

    accum_duration = FALSE;
    accum_affect = FALSE;

    to_vict = "You feel smarter.";

    break;

  case SPELL_ENHANCE_CHARISMA:

    af[0].location = APPLY_CHA;
    af[0].duration = (GET_LEVEL(ch) >> 1) + 4;

    if ( is_cleric ) {
      af[0].modifier = 1 + (level / 10);
    } else {
      af[0].modifier = 1 + (level > 18);
    }

    if (GET_CHA(victim) < 16)
      af[0].modifier = MAX(af[0].modifier, 16 - GET_CHA(victim));

    accum_duration = FALSE;
    accum_affect = FALSE;

    to_vict = "You feel more socially adept.";

    break;



  case SPELL_SENSE_LIFE:

    to_vict = "Your feel your awareness improve.";

    af[0].duration = GET_LEVEL(ch);

    af[0].bitvector = AFF_SENSE_LIFE;

    accum_duration = FALSE;

    break;



  case SPELL_WATER_WALK:

    af[0].duration = 12;

    af[0].bitvector = AFF_WATERWALK;

    accum_duration = FALSE;

    to_vict = "You feel webbing between your toes.";

    break;



  case SPELL_WATER_BREATHING:

    af[0].duration = 12;

    af[0].bitvector = AFF_WATERBREATH;

    accum_duration = FALSE;

    to_vict = "Gils sprout on the sides of your throat!";

    break;



  case SPELL_BARKSKIN:

    if (affected_by_spell(victim, SPELL_ARMOR) ||

        affected_by_spell(victim, SPELL_MAGIC_VEST))

    {

      send_to_char(NOEFFECT, ch);

      return;

    }

    af[0].location = APPLY_AC;

    af[0].modifier = -35;

    af[0].duration = 24;

    accum_duration = FALSE;

    to_vict = "Your skin begins to feel thicker.";

    break;

                            

  case SPELL_STONESKIN:

    af[0].duration = 4;

    af[0].bitvector = AFF_STONESKIN;

    to_vict = "Your skin suddenly turns incredibly hard.";

    to_room = "$n's skin suddenly turns granite-like.";

    break;

                            

  case SPELL_FLIGHT:

    af[0].duration = 6;

    af[0].bitvector = AFF_FLIGHT;

    accum_duration = FALSE;

    to_vict = "Your feet rise from the ground.";

    to_room = "$n's feet rise from the ground.";

    break;



  case SPELL_WEAKNESS:

    if (factor < 50) {

      send_to_char(NOEFFECT, ch);

      return;

    }



    af[0].location = APPLY_STR;



    if (is_cleric) {

      af[0].modifier = -1 - (level / 10);

      af[0].duration = (GET_LEVEL(ch) >> 1) + 4;

    } else {

      af[0].modifier = -3;

      af[0].duration = 24;



      af[1].duration = 24;

      af[1].location = APPLY_AC;

      af[1].modifier = 20;

    }



    accum_affect = FALSE;

    accum_duration = FALSE;

    to_vict = "You feel weaker.";

    break;



  case SPELL_HASTE:

    af[0].duration = 6;

    af[0].bitvector = AFF_HASTE;

    accum_duration = FALSE;

    to_vict = "You start to move rapidly.";

    break;



  case SPELL_LIGHT:

    if (!victim)

      victim = ch;



    af[0].location = APPLY_AC;

    af[0].modifier = 0;

    af[0].duration = GET_LEVEL(ch) / 2;

    accum_duration = FALSE;

    to_vict = "Your body begins to shine brightly.";

    to_room = "$n's body begins to shine brightly.";

    world[victim->in_room].light += 10;

    break;



  case SPELL_DARKNESS:

    if ((ch != victim) && (factor < 50)) {

      send_to_char(NOEFFECT, ch);

      return;

    }



    af[0].location = APPLY_AC;

    af[0].modifier = 0;

    af[0].duration = GET_LEVEL(ch) / 2;

    accum_duration = FALSE;

    to_vict = "A darkness flows out from your body.";

    to_room = "A darkness flows out from $n's body.";

    world[victim->in_room].light -= 10;

    break;



   case SPELL_DONTUSEME:

    af[0].location = APPLY_AC;

    af[0].modifier = 0;

    af[0].duration = GET_LEVEL(ch) / 2;

    accum_duration = FALSE;

    break;



  case SPELL_SILENCE:

    if (factor < 50 ) {

      send_to_char(NOEFFECT, ch);

      if (IS_NPC(victim) && AWAKE(victim) && !FIGHTING(victim) && victim != ch)

        hit(victim, ch, TYPE_UNDEFINED);

      return;

    }

    af[0].location = APPLY_AC;

    af[0].modifier = 0;

    af[0].duration = GET_LEVEL(ch) / 2;

    accum_duration = FALSE;

    to_vict = "You are unable to say anything now.";

    to_room = "$n appears to be rather silent now.";

    break;



  case SPELL_MAGIC_VEST:

    if (affected_by_spell(victim, SPELL_ARMOR) ||

        affected_by_spell(victim, SPELL_BARKSKIN))

    {

      send_to_char(NOEFFECT, ch);

      return;

    }

    af[0].location = APPLY_AC;

    af[0].modifier = -40;

    af[0].duration = 4;

    af[0].bitvector = AFF_MAGIC_VEST;

    to_vict = "You feel your skin begin to harden.";

    break;



  case SPELL_FAERIE_FIRE:

    af[0].location = APPLY_AC;

    af[0].modifier = 20;

    af[0].duration = 1 + (GET_LEVEL(ch) / 5);



    if (AFF_FLAGGED(victim, AFF_HIDE))

       REMOVE_BIT(AFF_FLAGS(victim), AFF_HIDE);

    if (AFF_FLAGGED(victim, AFF_SNEAK))

       REMOVE_BIT(AFF_FLAGS(victim), AFF_SNEAK);



    to_room = "$n is outlined in flickering faerie fire!";

    to_vict = "$N outlines you in flickering faerie fire!";

    break;



  case SPELL_SPIRIT_HAMMER:

    af[0].location = APPLY_AC;

    af[0].modifier = 0;

    af[0].duration = GET_LEVEL(ch);

    to_vict = "A magical hammer begins floating above your head.";

    to_room = "A magical hammer begins floating above $n's head.";

    break;



  case SPELL_FLAME_BLADE:

    af[0].duration = GET_LEVEL(ch);

    af[0].location = APPLY_AC;

    af[0].modifier = 0;

    to_vict = "A blade of fire comes into existence.";
    to_room = "A blade of fire manifests in the hands of $n.";

    break;



  case SPELL_DEAFENING_WIND:

    if (factor < 50)

    {

      if (IS_NPC(victim) && !FIGHTING(victim) && AWAKE(victim) && victim != ch)

        hit(victim, ch, TYPE_UNDEFINED);

      return;

    }

    af[0].duration = 6;

    af[0].location = APPLY_AC;

    af[0].modifier = 0;

    to_vict = "The sound of wind fills your ears, totally deafening you.";

    break;



  case SPELL_THORNSHIELD:

    if (affected_by_spell(victim, SPELL_SANCTUARY) ||

        affected_by_spell(victim, SPELL_MINOR_SANCTUARY) ||

        affected_by_spell(victim, SPELL_FIRE_SHIELD))

      return;



    af[0].duration = 4;

    af[0].location = APPLY_AC;

    af[0].modifier = -10;

    to_vict = "You are surrounded by a shield of thorns.";

    break;



  case SPELL_FREE_ACTION:

    af[0].location = APPLY_AC;

    af[0].modifier = 0;

    af[0].duration = 4;

    to_vict = "You feel as though there was no weight on your body.";

    break;



  case SPELL_DISEASE:

    if (factor < 50)

    {

      if (IS_NPC(victim) && AWAKE(victim) && !FIGHTING(victim) && victim != ch)

        hit(victim, ch, TYPE_UNDEFINED);

      return;

    }

    af[0].duration = -1;

    af[0].location = APPLY_CON;

    af[0].modifier = -2;



    af[1].duration = -1;

    af[1].location = APPLY_STR;

    af[1].modifier = -1;



    to_vict = "You have become infected with some sort of disease!";

    to_room = "$n has become affected with some form of disease.";

    break;



  case SPELL_WITHER:

    if (factor < 50)

    {

      if (IS_NPC(victim) && AWAKE(victim) && !FIGHTING(victim) && victim != ch)

        hit(victim, ch, TYPE_UNDEFINED);

      return;

    }



    if (number(0, 1))      // arm withered

    {

      af[0].location = APPLY_STR;

      af[0].modifier = -2;

      af[0].duration = -1;



      to_vict = "One of your arms has become withered and useless!";

      to_room = "One of $n's arms has become withered!";

      lose_arm(victim);

    }

    else   // leg withered

    {

      af[0].location = APPLY_DEX;

      af[0].modifier = -2;

      af[0].duration = -1;



      af[1].location = APPLY_MOVE;

      af[1].modifier = -(GET_MAX_MOVE(victim) / 2);

      af[1].duration = -1;



      to_vict = "One of your legs becomes withered and useless!";

      to_room = "One of $n's legs has become withered!";

    }



    break;



  case SPELL_PARALYZE:

    if (affected_by_spell(victim, SPELL_FREE_ACTION) ||

        affected_by_spell(victim, SPELL_IMPERVIOUS_MIND) ||

        factor < 50)

    {

      if (IS_NPC(victim) && AWAKE(victim) && !FIGHTING(victim) && victim != ch)

        hit(victim, ch, TYPE_UNDEFINED);

      return;

    }



    af[0].duration = level/3;

    af[0].location = APPLY_AC;

    af[0].modifier = 60;



    af[1].duration = 3;

    af[1].location = APPLY_HITROLL;

    af[1].modifier = -3;



    af[2].duration = 3;

    af[2].location = APPLY_DAMROLL;

    af[2].modifier = -3;

  

    to_vict = "Your body locks up, completely paralyzing you.";

    to_room = "$n has become completely paralyzed.";



    break;



  case SPELL_HOLD_PERSON:

    if (affected_by_spell(victim, SPELL_FREE_ACTION) ||

        affected_by_spell(victim, SPELL_IMPERVIOUS_MIND))

      return;



    af[0].location = APPLY_AC;

    af[0].modifier = 5;

    af[0].duration = 2;



    to_vict = "All the muscles in your body fail to respond.";

    to_room = "$n appears to be unable to move now.";



    break;



  case SPELL_ENTANGLE:

    switch(world[ch->in_room].sector_type)

    {

      case SECT_INSIDE:

      case SECT_CITY:

      case SECT_WATER_SWIM:

      case SECT_WATER_NOSWIM:

      case SECT_FLYING:

      case SECT_UNUSED_1:

      case SECT_UNUSED_2:

        send_to_char("Entangle them here, with no undergrowth?", ch);

        return;

      default:

        break;

    }



    if (affected_by_spell(victim, SPELL_FREE_ACTION))

      return;



    af[0].location = APPLY_AC;

    af[0].modifier = 5;

    af[0].duration = 4;



    to_vict = "Vines and roots shoot up out of the ground and entangle you.";

    to_room = "Vines and roots suddenly entangle $n.";



    break;



  case SPELL_BRAVERY:

    af[0].location = APPLY_HITROLL;

    af[0].modifier = dice(1, MAX(1, (GET_CHA(ch) - 6) / 4));

    af[0].duration = 2 + dice(1, MAX(1, (GET_CHA(ch) - 6) / 4));



    af[1].location = APPLY_DAMROLL;

    af[1].modifier = dice(1, MAX(1, (GET_CHA(ch) - 6) / 4));

    af[1].duration = 2 + dice(1, MAX(1, (GET_CHA(ch) - 6) / 4));



    to_vict = "You feel braver.";



    break;



  case SPELL_INSPIRE_FEAR:

    if (affected_by_spell(victim, SPELL_BRAVERY) ||

        affected_by_spell(victim, SPELL_IMPERVIOUS_MIND) ||

        (dice(1, 100) > GET_CHA(ch) && factor < 50))

    {

      if (IS_NPC(victim) && AWAKE(victim) && !FIGHTING(victim) && victim != ch)

        hit(victim, ch, TYPE_UNDEFINED);

      return;

    }



    af[0].location = APPLY_HITROLL;

    af[0].modifier = -1;

    af[0].duration = 4;



    af[0].location = APPLY_DAMROLL;

    af[0].modifier = -1;

    af[0].duration = 4;



    to_vict = "You are suddenly very afraid!";

    to_room = "$n suddenly looks very afraid.";



    break;



  case SPELL_CLOAK_OF_FEAR:

    af[0].location = APPLY_AC;

    af[0].modifier = -10;

    af[0].duration = 2;



    to_vict = "You are surrounded by a cloak of fear.";

    to_room = "$n has surrounded $mself with a cloak of fear.";

    break;



  case SPELL_THORNFLESH:

    if (affected_by_spell(victim, SPELL_BLESS))

    {

      send_to_char(NOEFFECT, ch);

      return;

    }



    af[0].location = APPLY_HITROLL;

    af[0].modifier = 1;

    af[0].duration = 5;



    af[1].location = APPLY_DAMROLL;

    af[1].modifier = 1;

    af[1].duration = 5;



    to_vict = "Thorns spring from your flesh enveloping your body.";

    to_room = "Thick, sharp thorns sprout from $n's body.";

    break;



  case SPELL_ADAMANT_MACE:

    if (affected_by_spell(victim, SPELL_ADAMANT_MACE))

    {

      send_to_char(NOEFFECT, ch);

      return;

    }

    else

    {

      struct obj_data *wielded = GET_EQ(ch, WEAR_WIELD);



      if (!wielded)

      {

        send_to_char("But you don't even have a weapon!\r\n", ch);

        return;

      }



      if (IS_OBJ_STAT(wielded, ITEM_MAGIC))

      {

        send_to_char("The magic in your weapon resists the spell.\r\n", ch);

        return;

      }

    }

    

    af[0].location = APPLY_HITROLL;

    af[0].modifier = 2;

    af[0].duration = 4;



    af[1].location = APPLY_DAMROLL;

    af[1].modifier = 2;

    af[1].duration = 4;



    to_vict = "Your weapon takes on the hardness of adamantite.";

    break;



  case SPELL_SOUL_DRAIN:

    if (factor < 50)

    {

      if (IS_NPC(victim) && AWAKE(victim) && !FIGHTING(victim) && victim != ch)

        hit(victim, ch, TYPE_UNDEFINED);

      return;

    }



    af[0].location = APPLY_WIS;

    af[0].modifier = -3;

    af[0].duration = -1;



    af[1].location = APPLY_AGE;

    af[1].modifier = 30;

    af[1].duration = -1;



    accum_affect = TRUE;



    to_vict = "You feel a strong drain on your soul, sapping your willpower.";

    to_room = "$n's soul appears to have been partially drained!";

    break;



  case SPELL_IMPERVIOUS_MIND:

    af[0].location = APPLY_SAVING_SPELL;

    af[0].modifier = -2;

    af[0].duration = 6;



    to_vict = "Your mind feels quite safe from magical assaults.";

    break;



  case SPELL_MYSTIC_SPIRIT:

    if (affected_by_spell(victim, SPELL_MYSTIC_SPIRIT) ||

        IS_AFFECTED(victim, AFF_CURSE))

    {

      send_to_char(NOEFFECT, ch);

      return;

    }



    random = number(1, 4);



    if (random == 1)

    {

      af[0].location = APPLY_AC;

      af[0].modifier = -number(GET_LEVEL(ch) / 2, GET_LEVEL(ch));

      to_vict = "The spirit of the armadillo augments your defensive power.";

      to_room = "$n has been granted the power of the armadillo.";

    } else if (random == 2) {

      af[0].location = APPLY_STR;

      af[0].modifier = 2;

      to_vict = "You feel the spirit of the bear infuse you with strength.";

      to_room = "$n has been infused with the spirit of the bear.";

    } else if (random == 3) {

      af[0].location = APPLY_HIT;

      af[0].modifier = number(10, GET_LEVEL(ch));

      to_vict = "You feel the spirit of the tiger infuse you with energy.";

      to_room = "$n briefly assumes the appearance of a tiger.";

    } else {  // random == 4

      af[0].location = APPLY_HITROLL;

      af[0].modifier = 1;



      af[1].location = APPLY_DAMROLL;

      af[1].modifier = 1;

      af[1].duration = 6;

      to_vict = "The spirit of the wolverine augments your combat skills.";

      to_room = "$n is augmented by the power of the wolverine.";

    }



    af[0].duration = 6;

    break;



  case SPELL_DIVINE_WRATH:

    af[0].location = APPLY_DAMROLL;

    af[0].modifier = 2;

    af[0].duration = 6;

    af[0].bitvector = AFF_HASTE;



    to_vict = "You are transported into a state of divine frenzy.";

    break;



  case SPELL_MOONBEAM:

    mag_affects(level, ch, victim, SPELL_FAERIE_FIRE, savetype);

    mag_affects(level, ch, victim, SPELL_SLOW, savetype);

    break;



  case SPELL_MIRE:
    if (affected_by_spell(victim, SPELL_FREE_ACTION))
    {
      return;
    }

    af[0].location = APPLY_AC;
    af[0].modifier = 5;
    af[0].duration = 4;

    to_vict = "You suddenly find yourself trapped in some sort of quagmire!.";
    to_room = "$n is suddenly trapped in some sort of quagmire!.";

    break;

  case SPELL_RAINBOW:
    mag_affects(level, ch, victim, SPELL_BLINDNESS, savetype);
    mag_affects(level, ch, victim, SPELL_SILENCE, savetype);
    mag_affects(level, ch, victim, SPELL_POISON, savetype);
    mag_affects(level, ch, victim, SPELL_DEAFENING_WIND, savetype);

    break;

  case SPELL_BLACK_PLAGUE:
    if (affected_by_spell(victim, SPELL_BREATH_OF_LIFE) || factor < 50)
    {
      return;
    }

    af[0].location = APPLY_CON;
    af[0].modifier = -3;
    af[0].duration = 24;

    to_vict = "You feel very ill as a dark disease falls upon you.";
    to_room = "$n appears to have caught some sort of disease.";

    break;

  case SPELL_BREATH_OF_LIFE:
    af[0].location = APPLY_AC;
    af[0].modifier = 0;
    af[0].duration = 48;

    to_vict = "You feel somewhat resistant to the spread of plague.";

    break;

  case SPELL_SLOW:
    if (factor < 50)
    {
      if (IS_NPC(victim) && AWAKE(victim) && !FIGHTING(victim) && victim != ch)
      {
        hit(victim, ch, TYPE_UNDEFINED);
      }

      return;
    }

    af[0].location = APPLY_AC;
    af[0].modifier = 1;
    af[0].duration = level;

    to_vict = "You begin moving a bit slower.";
    to_room = "$n begins moving a bit slower now.";

    break;

  case SPELL_BLACKMANTLE:
    if (factor < 50)
    {
      if (IS_NPC(victim) && AWAKE(victim) && !FIGHTING(victim) && victim != ch)
      {
        hit(victim, ch, TYPE_UNDEFINED);
      }

      return;
    }

    af[0].location = APPLY_AC;
    af[0].modifier = 0;
    af[0].duration = level >> (a + b);

    to_vict = "A black aura begins clinging to you.";
    to_room = "A black aura begins clinging to $n.";

    break;

  case SPELL_FIRE_SHIELD:
    if (affected_by_spell(ch, SPELL_SANCTUARY) || affected_by_spell(ch, SPELL_MINOR_SANCTUARY) ||
        affected_by_spell(ch, SPELL_THORNSHIELD))
    {
      send_to_char(NOEFFECT, ch);
      return;
    }

    af[0].location = APPLY_AC;
    af[0].modifier = 0;
    af[0].duration = 4;

    to_vict = "A red shield of fire surrounds your body.";
    to_room = "$n is surrounded by a fire shield.";

    break;



  case SPELL_BLINK:

     af[0].location = APPLY_AC;

     af[0].modifier = 0;

     af[0].duration = 4;



     to_vict = "Your body momentarily blinks out of view.";

     to_room = "$n begins blinking in and out of existence.";

     break;



  case SPELL_DANCING_SWORD:

    af[0].location = APPLY_AC;

    af[0].modifier = 0;

    af[0].duration = level * 3;



    to_vict = "A dancing sword fades into existence.";

    to_room = "A magical dancing sword fades into existence above $n.";

    break;



  case SPELL_CONE_OF_COLD:

    af[0].type = SPELL_SLOW;

    af[0].location = APPLY_AC;

    af[0].modifier = 0;

    af[0].duration = level;

    break;



  case SPELL_PRISMATIC_SPRAY:

    if (number(0, 1)) {

      mag_affects(level, ch, victim, SPELL_BLINDNESS, savetype);

     }

    if (number(0, 1)) {

      mag_affects(level, ch, victim, SPELL_SILENCE, savetype);

    }

    if (number(0, 1)) {

      mag_affects(level, ch, victim, SPELL_WEB, savetype);

    }

    if (number(0, 1)) {

      mag_affects(level, ch, victim, SPELL_POISON, savetype);

    }

    break;



  case SPELL_ARROW_OF_BONE:

    if (affected_by_spell(victim, SPELL_FREE_ACTION))

      return;



    if (factor < 50)

    {

      af[0].type = SPELL_SLOW;

    to_vict = "You begin moving a bit slower.";

    to_room = "$n begins moving a bit slower now.";

    } else {

      af[0].type = SPELL_PARALYZE; 

    to_vict = "Your body locks up, completely paralyzing you.";

    to_room = "$n has become completely paralyzed.";

    }

    af[0].location = APPLY_AC;

    af[0].modifier = 10;

    af[0].duration = level/2;



    break;



  case SPELL_WEB:

    if (factor < 50  ||

        affected_by_spell(victim, SPELL_FREE_ACTION))

    {

      if (IS_NPC(victim) && AWAKE(victim) && !FIGHTING(victim) && victim != ch)

        hit(victim, ch, TYPE_UNDEFINED);

      return;

    }



    af[0].location = APPLY_AC;

    af[0].modifier = 0;

    af[0].duration = 5;



    to_vict = "Thick webs leap out and cover you.";

    to_room = "Thick webs leap out and cover $n.";

    break;



  case SPELL_MINOR_GLOBE:

    if (affected_by_spell(victim, SPELL_MAJOR_GLOBE))
      return;

    af[0].location = APPLY_AC;
    af[0].modifier = -5;
    af[0].duration = level;

    to_vict = "You are surrounded by a semi-transparent globe.";
    to_room = "$n is surrounded by a semi-transparent globe.";
    break;



  case SPELL_MAJOR_GLOBE:

    if (affected_by_spell(victim, SPELL_MINOR_GLOBE))
      affect_from_char(victim, SPELL_MINOR_GLOBE);


    af[0].location = APPLY_AC;
    af[0].modifier = -10;
    af[0].duration = level;

    to_vict = "You are surrounded by a transparent shimmering globe.";
    to_room = "$n is surrounded by a transparent shimmering globe.";
    break;



  case SPELL_BLUR:

    af[0].location = APPLY_AC;

    af[0].modifier = -20;

    af[0].duration = level/3;



    to_vict = "Your body becomes blurry and unfocused.";

    to_room = "$n's body becomes blurry and unfocused.";

    break;



  case SPELL_SHIELD:

    af[0].location = APPLY_SAVING_SPELL;

    af[0].modifier = -15;

    af[0].duration = level/3;



    to_vict = "A faintly visible shield surrounds you.";

    to_room = "$n is surrounded by a faintly visible shield.";

    break;



  case SPELL_MIRROR_IMAGE:

  case SPELL_DECEPTIVE_IMAGERY:

    if (affected_by_spell(victim, SPELL_MIRROR_IMAGE) ||

        affected_by_spell(victim, SPELL_DECEPTIVE_IMAGERY))

      return;



    a = number(3, 5);



    for (b = 0; b < a; b++)

    {

      af[b].location = 1 + b;

      af[b].modifier = 0;

      af[b].duration = 4 + b;

    }



    to_vict = "A few shimmering images appear around you.";

    to_room = "$n has created some images of $mself!";

    break;



  case SPELL_SPELL_TURNING:

    if (affected_by_spell(victim, SPELL_SPELL_TURNING))

      return;



    af[0].location = APPLY_AC;

    af[0].duration = level;



    to_vict = "You ready yourself to turn oncoming spells.";

    to_room = "A faint blue glow momentarily surrounds $n.";

    break;



  case SPELL_ANTIMAGIC_AURA:

    if (affected_by_spell(victim, SPELL_SILENCE) ||

        affected_by_spell(victim, SPELL_ANTIMAGIC_AURA) ||

        factor < 50 ) {

      send_to_char(NOEFFECT, ch);

      return;

    }



    af[0].location = APPLY_AC;

    af[0].duration = level / 2;



    to_vict = "A faint yellow aura suddenly clings to you.";

    to_room = "A faint yellowish aura suddenly clings to $n!";

    break;

  

  case SPELL_MAGICAL_SUSCEPT:

    if (factor < 50)

    {

      send_to_char(NOEFFECT, ch);

      if (IS_NPC(victim) && AWAKE(victim) && !FIGHTING(victim) && victim != ch)

        hit(victim, ch, TYPE_UNDEFINED);

      return;

    }



    af[0].location = APPLY_SAVING_SPELL;

    af[0].modifier = 5;

    af[0].duration = -1;



    accum_affect = TRUE;

    to_vict = "You suddenly feel more susceptable to magical assault.";



    break;



  case SPELL_STATUE:

    af[0].location = APPLY_AC;

    af[0].modifier = -100;

    af[0].duration = 4;



    to_vict = "You become a stone statue.";

    to_room = "$n suddenly becomes a stone statue.";



    break;



  case SPELL_VAMPIRIC_AURA:

    af[0].location = APPLY_HITROLL;

    af[0].modifier = 0;

    af[0].duration = 2;



    to_vict = "You begin emitting a pale black glow.";

    to_room = "$n begins to emit a pale black glow.";

    break;



  case SPELL_PHANTASMAL_SNAKE:

    af[0].location = APPLY_AC;

    af[0].modifier = 0;

    af[0].duration = level;



    to_vict = "A phantasmal serpent appears next to you.";

    to_room = "A large serpent appears next to $n.";

    break;



  case SPELL_RECOLLECTION:

    af[0].duration = 1;

    af[0].modifier = APPLY_INT;

    af[0].modifier = 0;



    ch->exchange = level;

    to_vict = "Your memory suddenly becomes very sharp.";

    break;



  }



  /*

   * If this is a mob that has this affect set in its mob file, do not

   * perform the affect.  This prevents people from un-sancting mobs

   * by sancting them and waiting for it to fade, for example.

   */

  if (IS_NPC(victim) && !affected_by_spell(victim, spellnum)) {

    for (i = 0; i < MAX_SPELL_AFFECTS; i++) {

      if (IS_AFFECTED(victim, af[i].bitvector)) {

	send_to_char(NOEFFECT, ch);

	return;

      }

    }

  }



  if (GET_POS(victim) <= POS_DEAD) return;



  /*

   * If the victim is already affected by this spell, and the spell does

   * not have an accumulative effect, then fail the spell.

   */

  if (affected_by_spell(victim,spellnum) && !(accum_duration||accum_affect)) {

    send_to_char(NOEFFECT, ch);

    return;

  }

   for (i = 0; i < MAX_SPELL_AFFECTS; i++) {

    af[i].modifier *= factor;
    af[i].modifier /= 100;

    af[i].duration *= factor;
    af[i].duration /= 100;

  }



  for (i = 0; i < MAX_SPELL_AFFECTS; i++) {

    if (af[i].bitvector || (af[i].location != APPLY_NONE)) {

      affect_join(victim, af+i, accum_duration, FALSE, accum_affect, FALSE);

    }

  }



  if (to_vict != NULL)

    act(to_vict, FALSE, victim, 0, ch, TO_CHAR);



  if (to_room != NULL)

    act(to_room, TRUE, victim, 0, ch, TO_ROOM);

 

  GET_MOVE(victim) = MIN(GET_MOVE(victim), GET_MAX_MOVE(victim));



}



/*

 * This function is used to provide services to mag_groups.  This function

 * is the one you should change to add new group spells.

 */


void perform_mag_groups(int level, struct char_data * ch, struct char_data * tch, int spellnum, int savetype)
{

  int percent = 0;
  int factor = 0, aggressive = spell_info[spellnum].violent;

  switch (spellnum) {

    case SPELL_HEALING_LIGHT:

      mag_points(level, ch, tch, SPELL_CURE_CRITIC, savetype);

      break;

    case SPELL_SK_CHALLENGE:
      mag_affects(level, ch, tch, SPELL_SK_CHALLENGE, savetype);
      break;

    case SPELL_DIVINE_WRATH:

      mag_affects(level, ch, tch, SPELL_AID, savetype);


      if (!IS_NPC(ch) && !IS_NPC(tch) &&
         (IS_CLERIC(tch) || IS_PALADIN(tch)) && IS_CLERIC(ch) &&
         (GET_GODSELECT(ch) == GET_GODSELECT(tch)))

        mag_affects(level, ch, tch, SPELL_DIVINE_WRATH, savetype);

      break;


    case SPELL_BLESSING_OF_KINGS:

      if (!IS_NPC(ch) && !IS_NPC(tch) &&
         (IS_CLERIC(tch) || IS_PALADIN(tch)) && IS_CLERIC(ch) &&
         (GET_GODSELECT(ch) == GET_GODSELECT(tch)))
        percent = 101;


      if (percent < 101) 
        percent = dice(1,100);
      if (percent > 50)
        mag_affects(level, ch, tch, SPELL_STRENGTH, savetype);
      if (percent < 101) 
        percent = dice(1,100);
      if (percent > 50)
        mag_affects(level, ch, tch, SPELL_ENHANCE_AGILITY, savetype);
      if (percent < 101) 
        percent = dice(1,100);
      if (percent > 50)
        mag_affects(level, ch, tch, SPELL_ENHANCE_ENDURANCE, savetype);
      if (percent < 101) 
        percent = dice(1,100);
      if (percent > 50)
        mag_affects(level, ch, tch, SPELL_ENHANCE_INSIGHT, savetype);
      if (percent < 101) 
        percent = dice(1,100);
      if (percent > 50)
        mag_affects(level, ch, tch, SPELL_ENHANCE_CUNNING, savetype);
      if (percent < 101) 
        percent = dice(1,100);
      if (percent > 50)
        mag_affects(level, ch, tch, SPELL_ENHANCE_CHARISMA, savetype);

      break;

    case SPELL_WIND_WALK:

      mag_affects(level, ch, tch, SPELL_FLIGHT, savetype);

      break;

    case SPELL_HEROES_FEAST:

      if ((factor = mag_savingthrow_new(ch, tch, aggressive, savetype, spellnum)) <50 ) {
        if (!aggressive) {
          send_to_char("You were unable to summon enough energy and concentration to complete the spell.\r\n", ch);
          return;
        }
      }

      send_to_char("You eat your fill of the Heroes' Feast.\r\n", tch);

      if (GET_COND(tch, FULL) != -1)

        GET_COND(tch, FULL) = 24;

      if (GET_COND(tch, THIRST) != -1)

        GET_COND(tch, THIRST) = 24;

      break;

    case SPELL_PRAYER:

      mag_affects(level, ch, tch, SPELL_BLESS, savetype);

      break;

  }

}


/*

 * Every spell that affects the group should run through here

 * perform_mag_groups contains the switch statement to send us to the right

 * magic.

 *

 * group spells affect everyone grouped with the caster who is in the room,

 * caster last.

 *

 * To add new group spells, you shouldn't have to change anything in

 * mag_groups -- just add a new case to perform_mag_groups.

 */

void mag_groups(int level, struct char_data * ch, int spellnum, int savetype)
{

  struct char_data *tch, *k;
  struct follow_type *f;

  if (ch == NULL)
    return;

  if (!IS_AFFECTED(ch, AFF_GROUP))
    return;

  perform_mag_groups(level, ch, ch, spellnum, savetype);

  if (ch->master != NULL)
    k = ch->master;
  else
    k = ch;

  for (f = k->followers; f; f = f->next) {
    if (IS_AFFECTED(f->follower, AFF_GROUP) &&
        f->follower->in_room == ch->in_room)
    {
      tch = f->follower;
      if (ch == tch)
        continue;
      perform_mag_groups(level, ch, tch, spellnum, savetype);
    }
  }

  if ((k != ch) && IS_AFFECTED(k, AFF_GROUP) && (k->in_room == ch->in_room))
    perform_mag_groups(level, ch, k, spellnum, savetype);

}

/*

 * mass spells affect every creature in the room, even the caster.

 */


void mag_masses(int level, struct char_data * ch, int spellnum, int savetype)
{

  struct char_data *tch, *next;

  struct affected_type af[MAX_SPELL_AFFECTS];

  bool accum_affect = FALSE, accum_duration = FALSE;

  char *to_vict = NULL, *to_room = NULL;

  int i;
  int factor = 0, aggressive = spell_info[spellnum].violent;

  if (spellnum == SPELL_BLACK_PLAGUE)

  {

    af[0].type = SPELL_BREATH_OF_LIFE;

    af[0].location = APPLY_AC;

    af[0].duration = 36;

    affect_join(ch, af, 0, 0, 0, 0);

  }



  if (spellnum == SPELL_MOONBEAM && !OUTSIDE(ch))

  {

    send_to_char("You cannot focus the rays of the moons inside.\r\n", ch);

    return;

  }



  for (tch = world[ch->in_room].people; tch; tch = next)

  {

    next = tch->next_in_room;

   if ((factor = mag_savingthrow_new(ch, tch, aggressive, savetype, spellnum)) <50 ) {
    if (!aggressive) {
      send_to_char("You were unable to summon enough energy and concentration to complete the spell.\r\n", ch);
      return;
    }
  }



    for (i=0; i<MAX_SPELL_AFFECTS; i++) {

      af[i].type = spellnum;

      af[i].bitvector = 0;

      af[i].modifier = 0;

      af[i].location = APPLY_NONE;

      af[i].duration = 0;

    }

    to_vict = NULL;



    if (!IS_NPC(tch) && (GET_LEVEL(tch) > LVL_IMMORT))

      continue;



    switch(spellnum)

    {

      case SPELL_SPHERE_SILENCE:

     if (ch == tch)

     continue;

     else

        mag_affects(level, ch, tch, SPELL_SILENCE, savetype);

        break;



      case SPELL_MIRE:

        af[0].duration = 3;

        to_vict = "The entire room suddenly becomes a mire, entrapping you!";

        mag_affects(level, ch, tch, SPELL_MIRE, savetype);

        break;



      case SPELL_MOONBEAM:

        if ((ch == tch) || (IS_AFFECTED(ch, AFF_GROUP) &&

             IS_AFFECTED(tch, AFF_GROUP) &&

           ((ch->master == tch) || (tch->master == ch) ||

            (ch->master == tch->master))))

          continue;

        mag_affects(level, ch, tch, SPELL_MOONBEAM, savetype);

        mag_damage(level, ch, tch, SPELL_MOONBEAM, savetype);

        break;



      case SPELL_METEOR_SWARM:

      case SPELL_HARMFUL_WRATH:

        if ((ch == tch) || (IS_AFFECTED(ch, AFF_GROUP) && 

             IS_AFFECTED(tch, AFF_GROUP) &&

           ((ch->master == tch) || (tch->master == ch) ||

            (ch->master == tch->master))))

          continue;

        mag_damage(level, ch, tch, spellnum, savetype);

        break;



      case SPELL_MASS_CHARM:

        if ((ch == tch) || (IS_AFFECTED(ch, AFF_GROUP) &&

             IS_AFFECTED(tch, AFF_GROUP) &&

           ((ch->master == tch) || (tch->master == ch) ||

            (ch->master == tch->master))))

          continue;

        if (GET_LEVEL(ch) - 10 < GET_LEVEL(tch))

          continue;

        spell_charm(level, ch, tch, 0);



        break;



      case SPELL_BLACK_PLAGUE:

        mag_affects(level, ch, tch, SPELL_BLACK_PLAGUE, savetype);

        break;



      case SPELL_BREATH_OF_LIFE:

        mag_unaffects(level, ch, tch, SPELL_BREATH_OF_LIFE, savetype);

        break;

    }



    /*  below copied from mag_affects  */

    /*

     * If this is a mob that has this affect set in its mob file, do not

     * perform the affect.  This prevents people from un-sancting mobs

     * by sancting them and waiting for it to fade, for example.

     */

    if (IS_NPC(tch) && !affected_by_spell(tch, spellnum)) {

      for (i = 0; i < MAX_SPELL_AFFECTS; i++) {

	if (IS_AFFECTED(tch, af[i].bitvector)) {

	  continue;

	}

      }

    }



    /*

     * If the victim is already affected by this spell, and the spell does

     * not have an accumulative effect, then fail the spell.

     */

    if (affected_by_spell(tch,spellnum) &&

	!(accum_duration||accum_affect)) {

      continue;

    }



    for (i = 0; i < MAX_SPELL_AFFECTS; i++) {

      af[i].modifier *= factor;
      af[i].modifier /= 100;

      af[i].duration *= factor;
      af[i].duration /= 100;

      if (af[i].bitvector || (af[i].location != APPLY_NONE)) {

	affect_join(tch, af+i, accum_duration, FALSE, accum_affect, FALSE);

      }

    }



    if (to_vict != NULL)

      act(to_vict, FALSE, tch, 0, ch, TO_CHAR);



    if (to_room != NULL)

      act(to_room, TRUE, tch, 0, ch, TO_ROOM);



  }

}


/*

 * Every spell that affects an area (room) runs through here.  These are

 * generally offensive spells.  This calls mag_damage to do the actual

 * damage -- all spells listed here must also have a case in mag_damage()

 * in order for them to work.

 *

 *  area spells have limited targets within the room.

*/



void mag_areas(int level, struct char_data * ch, int spellnum, int savetype)
{

  struct char_data *tch, *next_tch;

  char *to_char = NULL;

  char *to_room = NULL;



  if (ch == NULL)

    return;



  /*

   * to add spells to this fn, just add the message here plus an entry

   * in mag_damage for the damaging part of the spell.

   */

  switch (spellnum) {

    default:

      break;

  }



  if (to_char != NULL)

    act(to_char, FALSE, ch, 0, 0, TO_CHAR);

  if (to_room != NULL)

    act(to_room, FALSE, ch, 0, 0, TO_ROOM);

  



  for (tch = world[ch->in_room].people; tch; tch = next_tch) {

    next_tch = tch->next_in_room;



    /*

     * The skips: 1: the caster

     *            2: immortals

     *            3: if no pk on this mud, skips over all players

     *            4: pets (charmed NPCs)

     * 

     *   Yeah, but that's stupid.  If PK is allowed, which it is, why not hit

     * your victim's pets?  It's supposed to hit everything in the room -

     * Let it.   Soli, 9/5/99

     */



    if (tch == ch)

      continue;

    if (!IS_NPC(tch) && GET_LEVEL(tch) >= LVL_IMMORT)

      continue;



    mag_damage(GET_LEVEL(ch), ch, tch, spellnum, 1);

  }

}



/*

 *  Every spell which summons/gates/conjures a mob comes through here.

 */


static char *mag_summon_msgs[] = {

  "$n animates a corpse!\r\n",

  "$n has called an animal spirit.\r\n",

  "$n has summoned an animal.\r\n",

  "$n has summoned some angry insects!\r\n",

  "$n summons an enraged dust devil!\r\n",

  "$n has summoned an aerial servant.\r\n",

  "$n has conjured an elemental.\r\n",

  "$n transforms some sticks into snakes!\r\n",

  "$n has summoned a large guardian.\r\n"

};


static char *mag_summon_fail_msgs[] = 
{
  "You fail to animate the spirits of the dead.\r\n",
  "Your call goes unanswered.\r\n",
  "Your summoning goes unanswered.\r\n",
  "The snakes fail to answer your call.\r\n"
};


void mag_summons(int level, struct char_data * ch, struct char_data * victim, struct obj_data * obj, int spellnum, int savetype)
{
  struct char_data *mob = NULL;
  struct affected_type af;
  int pfail = 0;
  int msg = 0, fail_msg = 0;
  int num = 1, mob_num = 0;
  int i, affect = 0;
  int factor = 0, aggressive = spell_info[spellnum].violent;
  bool hit_vict = FALSE, charmed = TRUE;

  if (ch == NULL)
  {
    return;
  }

  if ((factor = mag_savingthrow_new(ch, ch, aggressive, savetype, spellnum)) <50 ) 
  {
    if (!aggressive) 
    {
      send_to_char("You were unable to summon enough energy and concentration to complete the spell.\r\n", ch);
      return;
    }
  }



  switch (spellnum) 
  {
    case SPELL_AERIAL_SERVANT:
        mob_num = VNUM_AERIAL_SERVANT;
        pfail = 75;
        msg = 5;  fail_msg = 2;
        affect = 1;   af.duration = 5 + (level / 4);
        break;

    case SPELL_DUST_DEVIL:
        if (!victim || (victim == ch))
        {
            send_to_char("That is not a valid target!\r\n", ch);
            return;
        }

        mob_num = VNUM_DUST_DEVIL;
        pfail = 75;
        hit_vict = TRUE;  charmed = FALSE;
        msg = 4;  fail_msg = 2;
        affect = 1;   af.duration = 6;
        break;

    case SPELL_SUMMON_INSECTS:
        if (!victim || (victim == ch))
        {
            send_to_char("That is not a valid target!\r\n", ch);
            return;
        }

        mob_num = VNUM_INSECTS;
        pfail = 80;
        hit_vict = TRUE;  charmed = FALSE;
        msg = 3;  fail_msg = 2;
        affect = 1;   af.duration = 4;
        break;

    case SPELL_ANIMAL_SUMMONING:
        mob_num = VNUM_ANIMAL_SUM_I + number(0, 2);
        pfail = GET_SKILL(ch, SPELL_ANIMAL_SUMMONING) - 25;
        msg = 2;  fail_msg = 2;
        affect = 1;   af.duration = 3 + (level / 3); 
        break;

    case SPELL_ANIMAL_SUMMONING_II:
        mob_num = VNUM_ANIMAL_SUM_II + number(0, 2);
        pfail = GET_SKILL(ch, SPELL_ANIMAL_SUMMONING_II) - 30;
        msg = 2;   fail_msg = 2;
        affect = 1;   af.duration = 4 + (level / 3);
        break;

    case SPELL_ANIMAL_SUMMONING_III:
        mob_num = VNUM_ANIMAL_SUM_III + number(0, 3);
        pfail = GET_SKILL(ch, SPELL_ANIMAL_SUMMONING_III) - 35;
        msg = 2;  fail_msg = 2;
        affect = 1;   af.duration = 5 + (level / 3);
        break;

    case SPELL_CALL_ANIMAL_SPIRIT:
        mob_num = VNUM_ANIMAL_SPIRIT + number(0, 2);
        pfail = GET_SKILL(ch, SPELL_CALL_ANIMAL_SPIRIT) - 15;
        msg = 1;  fail_msg = 1;
        affect = 1;   af.duration = 5 + (level / 3);
        break;

    case SPELL_CONJURE_ELEMENTAL:
        mob_num = VNUM_ELEMENTAL + number(0, 3);
        pfail = GET_SKILL(ch, SPELL_CONJURE_ELEMENTAL) - 15;
        msg = 6;  fail_msg = 2;
        affect = 1;   af.duration = 4 + (level / 4);
        break;

    case SPELL_GREATER_ELEMENTAL:
        mob_num = VNUM_GREATER_ELEMENTAL + number(0, 3);
        pfail = GET_SKILL(ch, SPELL_GREATER_ELEMENTAL);
        msg = 6;  fail_msg = 2;
        affect = 1;   af.duration = 4 + (level / 4);
        break;

    case SPELL_SUMMON_GUARD:
        mob_num = VNUM_GUARD;
        pfail = GET_SKILL(ch, SPELL_SUMMON_GUARD) - 20;
        msg = 9;  fail_msg = 2;
        charmed = FALSE;
        affect = 1;   af.duration = 12;
        break;

    case SPELL_STICKS_TO_SNAKES:
        if (!victim || (victim == ch))
        {
            send_to_char("That is not a valid target!\r\n", ch);
            return;
        }

        mob_num = VNUM_SNAKES;
        pfail = GET_SKILL(ch, SPELL_STICKS_TO_SNAKES) - 20;
        msg = 8;  fail_msg = 3;
        hit_vict = TRUE;
        charmed = FALSE;
        affect = 1;   af.duration = 3;
        break;

    default:
        return;
  }

  if (IS_AFFECTED(ch, AFF_CHARM)) 
  {
    send_to_char("You are too giddy to have any followers!\r\n", ch);
    return;
  }

  if (number(0, 101) > GET_SKILL(ch, spellnum)) 
  {
    send_to_char(mag_summon_fail_msgs[fail_msg], ch);
    return;
  }

  if (hit_vict && (victim == NULL))
  {
    send_to_char("Upon whom should the spell be cast?\r\n", ch);
    return;
  }

  if (!allowNewFollower(ch, 3))
  { 

    return;

  }



  for (i = 0; i < num; i++) {

    mob = read_mobile(mob_num, VIRTUAL);



    if (mob == (struct char_data*) NULL)

    {

      sprintf(buf, 

        "SYSERR: (mag_summons) Attempt to create a mob that does not exist (%d)\n",

        mob_num);

      mudlog(buf, BRF, LVL_GRGOD, TRUE);

      return;

    }



    char_to_room(mob, ch->in_room);

    IS_CARRYING_W(mob) = 0;

    IS_CARRYING_N(mob) = 0;

    if (charmed)

      add_follower(mob, ch);

    if (spellnum == SPELL_SUMMON_GUARD)

      GUARDING(mob) = ch;



    act(mag_summon_msgs[msg], FALSE, ch, 0, mob, TO_ROOM);



/*  Pending - Soli, 11/1/99 */

#if 1

    if (affect)

    {

      af.location  = APPLY_AC;

      af.modifier  = 0;

      af.type      = spellnum;

      af.bitvector = 0;

      af.duration *= factor;
      af.duration /= 100;

      affect_join(mob, &af, 0, 0, 0, 0);

    }

#endif



    if (spellnum == SPELL_STICKS_TO_SNAKES)

      FIGHTING(victim) = mob;

    if (hit_vict)

      hit(mob, victim, TYPE_UNDEFINED);

  }

}


int allowNewFollower(struct char_data* ch, int maxFollowerAllowed)
{

  int permitNewFollower = 1;

  int numCharmed = 0;

  struct follow_type* fol = (struct follow_type*) NULL;



  for (fol = ch->followers; fol; fol = fol->next)

  {

    struct char_data* tmpFollower = fol->follower;



    if (tmpFollower == (struct char_data*) NULL)

      continue;



    if (IS_AFFECTED(tmpFollower, AFF_CHARM))

    {

      numCharmed++;

    }

  }



  if (numCharmed >= maxFollowerAllowed)

  {

    sprintf(buf, "You may have no more than %d followers at a time.\r\n",

      maxFollowerAllowed);

    send_to_char(buf, ch);

    permitNewFollower = 0;

  }



  return(permitNewFollower);

}


void mag_points(int level, struct char_data * ch, struct char_data * victim, int spellnum, int savetype)
{

  int hit = 0;

  int move = 0;

  int factor = 0, aggressive = spell_info[spellnum].violent;

  if (ch == NULL)

    return;

  if ((factor = mag_savingthrow_new(ch, victim, aggressive, savetype, spellnum)) <50 ) {
    if (!aggressive) {
      send_to_char("You were unable to summon enough energy and concentration to complete the spell.\r\n", ch);
      return;
    }
  }


  if (victim == NULL)

    return;



  switch (spellnum) {

  case SPELL_CURE_LIGHT:

    hit = dice(2, 4) + (level / 4);

    /* hit = dice(2, 4) + (MIN(level, 8)); REVAMP */

    send_to_char("You feel better.\r\n", victim);

    break;

  case SPELL_CURE_MODERATE:

    hit = dice(4, 4) + (level / 3);

    /* hit = dice(5, 4) + (MIN(level, 10)); REVAMP */

    send_to_char("You feel better.\r\n", victim);

    break;

  case SPELL_CURE_SEVERE:

    hit = dice(6, 4) + (level / 2);

    /* hit = dice(8, 4) + (MIN(level, 15)); REVAMP */

    send_to_char("You feel better.\r\n", victim);

    break;

  case SPELL_CURE_CRITIC:

    hit = dice(10, 4) + (level / 2);

    /* hit = dice(10, 4) + (level); REVAMP */

    send_to_char("You feel better.\r\n", victim);

    break;

  case SPELL_HEAL:

    hit = dice(14, 4) + (2 *level);

    send_to_char("A warm feeling fills your body.\r\n", victim);

    break;

  case SPELL_VAMPIRIC_TOUCH:

    hit = MIN(dice((level / 6) + 3, 10), 50) / 2;

    break;

  case SPELL_REFRESH:

    move = number(level * 3, level * 5);

    send_to_char("You feel refreshed.\r\n", victim);

    break;

  }


  hit *= factor;
  hit /= 100;

  move *= factor;
  move /= 100;

  if (affected_by_spell(victim, SPELL_BLACK_PLAGUE) ||

      affected_by_spell(victim, SPELL_BLACKMANTLE))

  {

    hit = hit / 2;

    move = move / 2;

  } else if (affected_by_spell(victim, SPELL_DISEASE))

  {

    hit = (2 * hit) / 3;

    move = (2 * move) / 3;

  }



  GET_HIT(victim) = MIN(GET_MAX_HIT(victim), GET_HIT(victim) + hit);

  GET_MOVE(victim) = MIN(GET_MAX_MOVE(victim), GET_MOVE(victim) + move);

  update_pos(victim);

}


void mag_unaffects(int level, struct char_data * ch, struct char_data * victim, int spellnum, int type)
{
  int spell=0, spell2=0, spell3=0, spell4=0, spell5=0;
  char *to_vict = NULL, *to_room = NULL;
  int tot_unaffects = 0;
  int factor = 0, aggressive = spell_info[spellnum].violent;

  if (ch == NULL)
    return;

  if ((factor = mag_savingthrow_new(ch, victim, aggressive, type, spellnum)) <50 ) {
    if (!aggressive) {
      send_to_char("You were unable to summon enough energy and concentration to complete the spell.\r\n", ch);
      return;
    }
  }

  if (victim == NULL)
    return;

  switch (spellnum) {
    case SPELL_CURE_BLIND:
    case SPELL_HEAL:
        spell = SPELL_BLINDNESS;
        break;

    case SPELL_NEUTRALIZE_POISON:
        spell = SPELL_POISON;
        to_room = "$n looks better.";
        break;

    case SPELL_CURE_DEAFNESS:
        spell = SPELL_DEAFENING_WIND;
        break;

    case SPELL_CURE_DISEASE:
        spell = SPELL_DISEASE;
        break;

    case SPELL_REMOVE_PARALYSIS:
        spell = SPELL_PARALYZE;
        spell2 = SPELL_HOLD_PERSON;
        break;

    case SPELL_REGENERATION:
        spell = SPELL_WITHER;
        break;

    case SPELL_REMOVE_CURSE:
        spell = SPELL_BESTOW_CURSE;
        spell2 = SPELL_MAGICAL_SUSCEPT;
        break;

    case SPELL_FREE_ACTION:
        spell = SPELL_PARALYZE;
        spell2 = SPELL_HOLD_PERSON;
        spell3 = SPELL_ENTANGLE;
        spell4 = SPELL_WEB;
        break;

    case SPELL_REMOVE_FEAR:
        spell = SPELL_INSPIRE_FEAR;
        break;

    case SPELL_IMPERVIOUS_MIND:
        spell = SPELL_CHARM;
        spell = SPELL_CHARM;
        spell2 = SPELL_HOLD_PERSON;
        spell3 = SPELL_PARALYZE;
        spell4 = SPELL_INSPIRE_FEAR;
        spell5 = SPELL_SLEEP;
        break;

    case SPELL_BREATH_OF_LIFE:
        spell = SPELL_BLACK_PLAGUE;
        break;

    case SPELL_SLOW:
        spell = SPELL_HASTE;
        break;

    case SPELL_HASTE:
        spell = SPELL_SLOW;
        break;

    case SPELL_BRAVERY:
        spell = SPELL_INSPIRE_FEAR;
        break;

    case SPELL_RESTORATION:
        spell = SPELL_SOUL_DRAIN;
        break;

    default:
        sprintf(buf, "SYSERR: unknown spellnum %d passed to mag_unaffects", spellnum);
        mudlog(buf, BRF, LVL_GRGOD, TRUE);
        return;
        break;
  }



  if (spell && affected_by_spell(victim, spell)) {

    tot_unaffects++;

    affect_from_char(victim, spell);

  }

  if (spell2 && affected_by_spell(victim, spell2)) {

    tot_unaffects++;

    affect_from_char(victim, spell2);

  }

  if (spell3 && affected_by_spell(victim, spell3)) {

    tot_unaffects++;

    affect_from_char(victim, spell3);

  }

  if (spell4 && affected_by_spell(victim, spell4)) {

    tot_unaffects++;

    affect_from_char(victim, spell4);

  }

  if (spell5 && affected_by_spell(victim, spell5)) {

    tot_unaffects++;

    affect_from_char(victim, spell5);

  }



  if (to_vict != NULL)

    act(to_vict, FALSE, victim, 0, ch, TO_CHAR);

  if (to_room != NULL)

    act(to_room, TRUE, victim, 0, ch, TO_ROOM);



}


void mag_alter_objs(int level, struct char_data * ch, struct obj_data * obj, int spellnum, int savetype)
{

  char *to_char = NULL;

  char *to_room = NULL;

  int factor = 0, aggressive = spell_info[spellnum].violent;

  if (ch == NULL)

    return;

  if ((factor = mag_savingthrow_new(ch, ch, aggressive, savetype, spellnum)) <50 ) {
    if (!aggressive) {
      send_to_char("You were unable to summon enough energy and concentration to complete the spell.\r\n", ch);
      return;
    }
  }

  if (obj == NULL) {

    return;

  }



  switch (spellnum) {

    case SPELL_BLESS:

      if (!IS_OBJ_STAT(obj, ITEM_BLESS) &&

	  (GET_OBJ_WEIGHT(obj) <= 5 * GET_LEVEL(ch))) {

	SET_BIT(GET_OBJ_EXTRA(obj), ITEM_BLESS);

	to_char = "$p glows briefly.";

      }

      break;

    case SPELL_BESTOW_CURSE:

      if (!IS_OBJ_STAT(obj, ITEM_NODROP)) {

	SET_BIT(GET_OBJ_EXTRA(obj), ITEM_NODROP);

	if (GET_OBJ_TYPE(obj) == ITEM_WEAPON)

	  GET_OBJ_VAL(obj, 2)--;

	to_char = "$p briefly glows red.";

      }

      break;

    case SPELL_INVISIBILITY:

      if (!IS_OBJ_STAT(obj, ITEM_NOINVIS | ITEM_INVISIBLE)) {

        SET_BIT(obj->obj_flags.extra_flags, ITEM_INVISIBLE);

        to_char = "$p vanishes.";

      }

      break;

    case SPELL_POISON:

      if (((GET_OBJ_TYPE(obj) == ITEM_DRINKCON) ||

         (GET_OBJ_TYPE(obj) == ITEM_FOUNTAIN) ||

         (GET_OBJ_TYPE(obj) == ITEM_FOOD)) && !GET_OBJ_VAL(obj, 3)) {

      GET_OBJ_VAL(obj, 3) = 1;

      to_char = "$p steams briefly.";

      }

      break;

    case SPELL_REMOVE_CURSE:

      if (IS_OBJ_STAT(obj, ITEM_NODROP)) {

        REMOVE_BIT(obj->obj_flags.extra_flags, ITEM_NODROP);

        if (GET_OBJ_TYPE(obj) == ITEM_WEAPON)

          GET_OBJ_VAL(obj, 2)++;

        to_char = "$p briefly glows blue.";

      }

      break;

    case SPELL_NEUTRALIZE_POISON:

      if (((GET_OBJ_TYPE(obj) == ITEM_DRINKCON) ||

         (GET_OBJ_TYPE(obj) == ITEM_FOUNTAIN) ||

         (GET_OBJ_TYPE(obj) == ITEM_FOOD)) && GET_OBJ_VAL(obj, 3)) {

        GET_OBJ_VAL(obj, 3) = 0;

        to_char = "$p steams briefly.";

      }

      break;



  }





  if (to_char != NULL)

    act(to_char, TRUE, ch, obj, 0, TO_CHAR);



  if (to_room != NULL)

    act(to_room, TRUE, ch, obj, 0, TO_ROOM);

  else if (to_char != NULL)

    act(to_char, TRUE, ch, obj, 0, TO_ROOM);



}


void mag_creations(int level, struct char_data * ch, int spellnum)
{

  struct obj_data *tobj;

  int z, objToRoom = FALSE;

  int i, loop = 1;

  int factor = 0, aggressive = spell_info[spellnum].violent;

  if (ch == NULL)

    return;

  if ((factor = mag_savingthrow_new(ch, ch, aggressive, SAVING_SPELL, spellnum)) <50 ) {
    if (!aggressive) {
      send_to_char("You were unable to summon enough energy and concentration to complete the spell.\r\n", ch);
      return;
    }
  }


  if (ch == NULL)

    return;

  level = MAX(MIN(level, LVL_IMPL), 1);



  switch (spellnum) {



  case SPELL_CREATE_FOOD:

    z = VNUM_WAYBREAD;

    break;

  case SPELL_GOODBERRY:

    z = VNUM_BERRIES;

    loop = 1 + (level / 6);

    break;



  default:

    send_to_char("Spell unimplemented, it would seem.\r\n", ch);

    return;

    break;

  }



  if (!(tobj = read_object(z, VIRTUAL))) {

    send_to_char("I seem to have goofed.\r\n", ch);

    sprintf(buf, "SYSERR: spell_creations, spell %d, obj %d: obj not found",

	    spellnum, z);

    mudlog(buf, BRF, LVL_GRGOD, TRUE);

    return;

  }



  for (i = 1; i <= loop; i++)

  {

    if (objToRoom == TRUE)

    {

      obj_to_room(tobj, ch->in_room);

    } else {

      obj_to_char(tobj, ch);

    }



    if (i < loop)

      tobj = read_object(z, VIRTUAL);

  }



  act("$n creates $p.", FALSE, ch, tobj, 0, TO_ROOM);

  act("You create $p.", FALSE, ch, tobj, 0, TO_CHAR);

}


void mag_area_magic(int level, struct char_data * ch, int spellnum)
{

  int duration, random;

  struct area_aff_data *area_af;

  int factor = 0, aggressive = spell_info[spellnum].violent;

  if (ch == NULL)

    return;

  if ((factor = mag_savingthrow_new(ch,ch, aggressive, SAVING_SPELL, spellnum)) <50 ) {
    if (!aggressive) {
      send_to_char("You were unable to summon enough energy and concentration to complete the spell.\r\n", ch);
      return;
    }
  }


  if (spellnum == SPELL_STORM_SUMMONING && !OUTSIDE(ch))

  {

    send_to_char("Summon a storm?  Inside?\r\n", ch);

    return;

  }



  CREATE(area_af, struct area_aff_data, 1);



  area_af->caster = ch;

  area_af->in_room = ch->in_room;

  area_af->type = spellnum;



  if (area_spells_list)

    area_af->next = area_spells_list;



  area_spells_list = area_af;

  switch (spellnum)

  {

    case SPELL_INSECT_PLAGUE:

      send_to_char("A plague of insects come to your aid.\r\n", ch);

      act("$n summons a plague of insects to $s aid.", 0, ch, 0, 0, TO_ROOM);

      duration = (GET_LEVEL(ch) / 4);

      break;

    case SPELL_CREEPING_DOOM:

      send_to_char("A creeping doom of insects somes to your aid.\r\n", ch);

      act("$n has summoned a creeping doom to $s aid.",

           FALSE, ch, 0, 0, TO_ROOM);

      duration = (GET_LEVEL(ch) / 2);

      break;

    case SPELL_BLADE_BARRIER:

      send_to_char("You create a shimmering wall of blades.\r\n", ch);

      act("$n has created a shimmering wall of blades!", 0, ch, 0, 0, TO_ROOM);

      duration = (GET_LEVEL(ch) / 3);

      break;

    case SPELL_STORM_SUMMONING:

      duration = (GET_LEVEL(ch) / 3);

      send_to_char("You hear a faint rumble in the distance.\r\n", ch);

      act("You hear a faint rumble in the distance.", 0, ch, 0, 0, TO_ROOM);

      break;

    case SPELL_DEHYDRATION:

      duration = (GET_LEVEL(ch) / 3);

      send_to_char("The air suddenly begins to feel much more dry.\r\n", ch);

      act("The air around you begins to dry up.", 0, ch, 0, 0, TO_ROOM);

      break;

    case SPELL_ELEMENTAL_STORM:

      random = number(1, 4);

      switch (random)

      {

        case 1:

          send_to_char("A storm of fire blasts around you.\r\n", ch);

          act("$n creates a firestorm.", FALSE, ch, 0, 0, TO_ROOM);

          break;

        case 2:

          send_to_char("A massive wave of water suddenly appears.\r\n", ch);

          act("A massive wave of water suddenly rushes over you.",

               FALSE, ch, 0, 0, TO_ROOM);

          break;

        case 3:

          send_to_char("The wind begins to stir, causing a tornado.\r\n", ch);

          act("The wind stirs, creating a tornado.", 0, ch, 0, 0, TO_ROOM);

          break;

        case 4:

          send_to_char("The ground begins to tremble and shake...\r\n", ch);

          act("The ground begins to tremble and shake.", 0, ch, 0, 0, TO_ROOM);

          break;

      }

      area_af->other = random;

      duration = GET_LEVEL(ch) / 2;

      break;



    default:

      sprintf(buf, "SYSERR: Invalid area spell, spellnum %d", spellnum);

      mudlog(buf, BRF, LVL_DEMIGOD, TRUE);

      return;

  }

  duration *= factor;
  duration /= 100;

  area_af->initial_duration = area_af->duration_left = duration;



}

