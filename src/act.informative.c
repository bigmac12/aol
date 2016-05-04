/* ************************************************************************
*   File: act.informative.c                             Part of CircleMUD *
*  Usage: Player-level commands of an informative nature                  *
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
#include "screen.h"
#include "constants.h"

/* extern variables */
extern struct room_data *world;
extern struct descriptor_data *descriptor_list;
extern struct char_data *character_list;
extern struct obj_data *object_list;
extern struct command_info cmd_info[];
extern struct title_type titles[NUM_CLASSES][LVL_IMPL + 1];
extern struct index_data *obj_index;
extern const int wear_order_index[NUM_WEARS];

extern char *credits;
extern char *news;
extern char *info;
extern char *motd;
extern char *imotd;
extern char *wizlist;
extern char *immlist;
extern char *policies;
extern char *handbook;
extern const char *color_liquid[];
extern char *class_abbrevs[];
extern char *race_abbrevs[];
extern const char *room_bits[];
extern char *spells[];

/* extern functions */
ACMD(do_action);
ACMD(do_insult);
long find_class_bitvector(char arg);
int level_exp(int class, int level);
void display_visual_effects(struct char_data* ch, struct char_data* to_ch);
void show_sheathed(struct char_data * ch, struct obj_data * obj,  struct obj_data * cont);
char *current_short_desc(struct char_data *ch);
int check_disabled(const struct command_info *command);


/* globals */
int boot_high;
//int *cmd_sort_info;


// Locals
void send_news(struct char_data *ch, char *arg);
void send_rules(struct char_data *ch);
void sort_commands(void);
int sort_commands_helper(const void *a, const void *b);

ACMD(do_autocon);

int hidden(struct obj_data *obj)
{
  if (obj->in_room == NOWHERE)
    return 0;

  if (IS_OBJ_STAT(obj, ITEM_HIDDEN))
    return 1;

  return 0;
}

int hidden_eq(struct char_data *ch, struct obj_data *obj)
{
  if (GET_OBJ_DISGUISE(obj) == 2 && PRF_FLAGGED(ch, PRF_NOTSELF))
    return 1;

  else
  return 0;
}


void show_obj_to_char(struct obj_data * object, struct char_data * ch, int mode)
{
  bool found;

  *buf = '\0';
if ((mode == 0) && object->description && GET_OBJ_CREATION(object) != 1)
    strcpy(buf, object->description);
  else if ((mode == 0) && object->description && GET_OBJ_CREATION(object) == 1 && GET_OBJ_NATURE(object) == 1 && IS_MAGE(ch))
    strcpy(buf, object->alt_description);

else if ((mode == 0) && object->description && GET_OBJ_CREATION(object) == 1 && GET_OBJ_NATURE(object) == 2 && IS_CLERIC(ch))
    strcpy(buf, object->alt_description);

else if ((mode == 0) && object->description && GET_OBJ_CREATION(object) == 1)
    strcpy(buf, object->description);

  else if (object->short_description && GET_OBJ_CREATION(object) != 1
&& ((mode == 1) || (mode == 2) || (mode == 3) || (mode == 4)))
    strcpy(buf, object->short_description);

  else if (object->short_description && GET_OBJ_CREATION(object) == 1 && IS_MAGE(ch)
&& GET_OBJ_NATURE(object) == 1
&& ((mode == 1) || (mode == 2) || (mode == 3) || (mode == 4)))
    strcpy(buf, object->altshort_description);

else if (object->short_description && GET_OBJ_CREATION(object) == 1 && IS_CLERIC(ch)
&& GET_OBJ_NATURE(object) == 2
&& ((mode == 1) || (mode == 2) || (mode == 3) || (mode == 4)))
    strcpy(buf, object->altshort_description);

 else if (object->short_description && GET_OBJ_CREATION(object) == 1
&& ((mode == 1) || (mode == 2) || (mode == 3) || (mode == 4)))    strcpy(buf, object->short_description);

  else if (mode == 5) {
    if (GET_OBJ_TYPE(object) == ITEM_NOTE) {
      if (object->action_description) {
        strcpy(buf, "There is something written upon it:\r\n\r\n");
        strcat(buf, object->action_description);
        page_string(ch->desc, buf, 1);
      } else
        act("It's blank.", FALSE, ch, 0, 0, TO_CHAR);
      return;
    }
    else if (GET_OBJ_TYPE(object) != ITEM_DRINKCON) {
      strcpy(buf, "You see nothing special..");
    } else                      /* ITEM_TYPE == ITEM_DRINKCON||FOUNTAIN */
      strcpy(buf, "It looks like a drink container.");
  }
  if (mode != 3) {
    found = FALSE;
    if (IS_OBJ_STAT(object, ITEM_INVISIBLE)) {
      strcat(buf, " (invisible)");
      found = TRUE;
    }

    /*if (IS_OBJ_STAT(object, ITEM_HIDDEN)) {
      strcat(buf, " (hidden)");
      found = TRUE;
    } */


    if (IS_OBJ_STAT(object, ITEM_BLESS) && IS_AFFECTED(ch, AFF_DETECT_ALIGN)) {
      strcat(buf, " ..It glows blue!");
      found = TRUE;
    }
    if (IS_OBJ_STAT(object, ITEM_MAGIC) && IS_AFFECTED(ch, AFF_DETECT_MAGIC)) {
      strcat(buf, " ..It glows yellow!");
      found = TRUE;
    }
    if (IS_OBJ_STAT(object, ITEM_GLOW)) {
      strcat(buf, " ..It has a soft glowing aura!");
      found = TRUE;
    }

     if (GET_OBJ_TYPE(object) == ITEM_RAW) {
      strcat(buf, "..It looks uncooked.");
      found = TRUE;
    }
  }

if (PRF_FLAGGED(ch, PRF_LEVEL_FLAGS))
 if (CAN_WEAR(object, ITEM_WEAR_TAKE))
      sprintf(buf, "%s &W(&C%d&W)&n ", buf, GET_OBJ_LEVEL(object));


  strcat(buf, "\r\n");
  page_string(ch->desc, buf, 1);
}

void list_obj_to_char(struct obj_data * list, struct char_data * ch, int mode, bool show)
{
  struct obj_data *i, *j;
  struct obj_data *o, *next_o;
  char buf[10];
  bool found;
  int num;

  found = FALSE;
 
  if (PLR_FLAGGED(ch, PLR_FIRE_ON)) {
    REMOVE_BIT(PLR_FLAGS(ch), PLR_FIRE_ON);
   }

  for (o = world[ch->in_room].contents; o; o = next_o) {
      next_o = o->next_content;
      if (GET_OBJ_TYPE(o) == ITEM_FIRE) {
          SET_BIT(PLR_FLAGS(ch), PLR_FIRE_ON);
                   }
           }

  for (i = list; i; i = i->next_content) {
      num = 0;
      for (j = list; j != i; j = j->next_content)
      {
        if (j->item_number == NOTHING) {
            if(strcmp(j->short_description, i->short_description) == 0)
              break;
        }
        else if (j->item_number == i->item_number)
          break;
      }
      if (j != i)
        continue;
      for (j = i; j; j = j->next_content)
      {
        if (j->item_number == NOTHING && IS_OBJ_STAT(j, ITEM_RAW)) {
          if(strcmp(j->short_description, i->short_description) == 0)
            num++;
        }

       else if (j->item_number == NOTHING && !IS_OBJ_STAT(j, ITEM_RAW)) {
          if(strcmp(j->short_description, i->short_description) == 0)
            num++;
        }

        else if (j->item_number==i->item_number && IS_OBJ_STAT(i, ITEM_RAW)) {
         num++;
        }

        else if (j->item_number==i->item_number && !IS_OBJ_STAT(i, ITEM_RAW)) {         num++;
        }

      }
      if (CAN_SEE_OBJ(ch, i) && !hidden(i)) {
        if (num != 1) {
          sprintf(buf,"(%2i) ",num);
          send_to_char(buf, ch);
        }
        show_obj_to_char(i, ch, mode);
        found = TRUE;
      }
  }
  if (!found && show)
    send_to_char(" Nothing.\r\n", ch);
}

void diag_char_to_char(struct char_data * i, struct char_data * ch)
{
  int percent;

  if (GET_MAX_HIT(i) > 0)
    percent = (100 * GET_HIT(i)) / GET_MAX_HIT(i);
  else
    percent = -1;		/* How could MAX_HIT be < 1?? */

  GET_PERS(i, ch, chname);
  strcpy(buf, chname);
  FREE_NAME(chname);
  CAP(buf);

  if (percent >= 100)
    strcat(buf, " is in excellent condition.\r\n");
  else if (percent >= 90)
    strcat(buf, " has a few scratches.\r\n");
  else if (percent >= 75)
    strcat(buf, " has some small wounds and bruises.\r\n");
  else if (percent >= 50)
    strcat(buf, " has quite a few wounds.\r\n");
  else if (percent >= 30)
    strcat(buf, " has some big nasty wounds and scratches.\r\n");
  else if (percent >= 15)
    strcat(buf, " looks pretty hurt.\r\n");
  else if (percent >= 0)
    strcat(buf, " is in awful condition.\r\n");
  else
    strcat(buf, " is bleeding awfully from big wounds.\r\n");

  send_to_char(buf, ch);
}

#define VNUM_BOTTLE   4125

void look_at_char(struct char_data * i, struct char_data * ch)
{
  int j, found;
  struct obj_data *o, *next_o;
  struct obj_data *item, *ob, *next_ob;
  struct obj_data *tmp_obj;
  char tmpBuf[1000];

  
  if (PLR_FLAGGED(ch, PLR_FIRE_ON)) {
    REMOVE_BIT(PLR_FLAGS(ch), PLR_FIRE_ON);
   }

  for (o = world[ch->in_room].contents; o; o = next_o) {
      next_o = o->next_content;
      if (GET_OBJ_TYPE(o) == ITEM_FIRE) {
          SET_BIT(PLR_FLAGS(ch), PLR_FIRE_ON);
                   }
           }

  
  if (PRF_FLAGGED(i, PRF_NOTSELF)) {
       sprintf(tmpBuf, "%s", i->char_specials.ldesc_dis);
        send_to_char(tmpBuf, ch);

} 


if (!ch->desc)
    return;

   if (i->player.description && !PRF_FLAGGED(i, PRF_NOTSELF)) {
    send_to_char(i->player.description, ch);
    }
  else {
    if (!PRF_FLAGGED(i, PRF_NOTSELF))
    act("You see nothing special about $m.", FALSE, i, 0, ch, TO_VICT);
  }


  if (RIDING(i) && RIDING(i)->in_room == i->in_room) {
    if (RIDING(i) == ch)
      act("$e is mounted on you.", FALSE, i, 0, ch, TO_VICT);
    else {
      GET_PERS(RIDING(i), ch, chname);
      sprintf(buf, "$e is mounted upon %s.", chname);
      FREE_NAME(chname);
      act(buf, FALSE, i, 0, ch, TO_VICT);
    }
  } else if (RIDDEN_BY(i) && RIDDEN_BY(i)->in_room == i->in_room) {
    if (RIDDEN_BY(i) == ch)
      act("You are mounted upon $m.", FALSE, i, 0, ch, TO_VICT);
    else {
      GET_PERS(RIDDEN_BY(i), ch, chname);
      sprintf(buf, "$e is mounted by %s.", chname);
       FREE_NAME(chname);
      act(buf, FALSE, i, 0, ch, TO_VICT);
    }
  }

   diag_char_to_char(i, ch);

  found = FALSE;
  for (j = 0; !found && j < NUM_WEARS; j++)
if (GET_EQ(i, j) && CAN_SEE_OBJ(ch, GET_EQ(i, j)) && !hidden_eq(i, GET_EQ(i, j))) 
      found = TRUE;

  if (found) {
    act("\r\n$n is using:", FALSE, i, 0, ch, TO_VICT);

    for (j = 0; j < NUM_WEARS; j++)
 if (GET_EQ(i, wear_order_index[j]) && CAN_SEE_OBJ(ch, GET_EQ(i, wear_order_index[j])) && !hidden_eq(i, GET_EQ(i, wear_order_index[j]))) {
        send_to_char(where[wear_order_index[j]], ch);
        show_obj_to_char(GET_EQ(i, wear_order_index[j]), ch, 1);
        item = GET_EQ(i, wear_order_index[j]);
        if (GET_OBJ_TYPE(item) == ITEM_SHEATH) {
          for (ob = item->contains; ob; ob = next_ob) {
            next_ob = ob->next_content;
       if (CAN_WEAR(ob, ITEM_WEAR_SHEATHED_H) || CAN_WEAR(ob, ITEM_WEAR_SHEATHED_WA) || CAN_WEAR(ob, ITEM_WEAR_SHEATHED_WR) || CAN_WEAR(ob, ITEM_WEAR_SHEATHED_A) || CAN_WEAR(ob, ITEM_WEAR_SHEATHED_B)) {
         show_sheathed(ch, ob, item);
        }  
        } 
      } 
  }
 }  
if ((ch != i) && (GET_LEVEL(i) < LVL_IMMORT) &&
      ((GET_CLASS(ch) == CLASS_ROGUE) || (GET_LEVEL(ch) >= LVL_IMMORT)))
  {
    found = FALSE;
    act("\r\nYou attempt to peek at $s inventory:", FALSE, i, 0, ch, TO_VICT);
    for (tmp_obj = i->carrying; tmp_obj; tmp_obj = tmp_obj->next_content) {
      if (CAN_SEE_OBJ(ch, tmp_obj) && (number(0, 20) < GET_LEVEL(ch))) {
	show_obj_to_char(tmp_obj, ch, 1);
	found = TRUE;
      }
    }

    if (!found)
      send_to_char("You can't see anything.\r\n", ch);
  }
}

static char *positions[] = 
{
  " is lying here, dead.",
  " is lying here, mortally wounded.",
  " is lying here, incapacitated.",
  " is lying here, unconcious.",
  " is sleeping here.",
  " is resting here.",
  " is sitting here.",
  "!FIGHTING!",
  " is standing here.",
  " is digging here.",
  " is fishing here.",
  " is riding here."
};

void list_one_pc(struct char_data * i, struct char_data * ch)
{

  if (known_player(ch, i))
  {
    CHOOSE_NAME_II(i, ch, chname);
    sprintf(buf, "%s, %s,", chname, IS_APPROVED(i) ? i->player.short_descr : current_short_desc(i));
    FREE_NAME(chname);
  }
  
  else {
    GET_NAME_II(i, ch, chname);
    sprintf(buf, "%s", chname);
    FREE_NAME(chname);
  }
 

  CAP(buf);


  if (IS_AFFECTED(i, AFF_INVISIBLE))
    strcat(buf, " (invisible)");
  if (AFF_FLAGGED(i, AFF_HIDE) && (!check_skill(i, 101, SKILL_HIDE) || AFF_FLAGGED(ch, AFF_SENSE_LIFE)))
    strcat(buf, " (hidden)");
  if (!IS_NPC(i) && !i->desc)
    strcat(buf, " (linkless)");
  if (PLR_FLAGGED(i, PLR_WRITING))
    strcat(buf, " (writing)");

if (RIDING(i) && RIDING(i)->in_room == i->in_room) {
 strcat(buf, " is here, mounted upon ");
      if (RIDING(i) == ch)
        strcat(buf, "YOU!");
  else {
    GET_PERS(RIDING(i), ch, chname);
    strcat(buf, chname);
    strcat(buf, ".");
    FREE_NAME(chname);
}
}
 else  if (GET_POS(i) != POS_FIGHTING && PRF_FLAGGED(i, PRF_NOTSELF) && !PRF_FLAGGED(ch, PRF_DETECT))  {
   strcpy(buf, CCCYN(ch, C_NRM));
  strcat(buf, i->char_specials.desc_dis);

  }

 else if (GET_POS(i) != POS_FIGHTING) 
    strcat(buf, positions[(int) GET_POS(i)]);
  else {
    if (FIGHTING(i)) {
      strcat(buf, " is here, fighting ");
      if (FIGHTING(i) == ch)
	strcat(buf, "YOU!");
      else {
	if (i->in_room == FIGHTING(i)->in_room)
        {
          GET_PERS(FIGHTING(i), ch, chname);
	  strcat(buf, chname);
          //strcat(buf, ".");
          FREE_NAME(chname);
        }
	else
	  strcat(buf, "someone who has already left");
	strcat(buf, "!");
      }
    } else			/* NIL fighting pointer */
      strcat(buf, " is here struggling with thin air.");
  }

  if (IS_AFFECTED(ch, AFF_DETECT_ALIGN))
  {
    if(IS_AFFECTED(i, AFF_CONCEAL_ALIGN))
    {
      if (IS_EVIL(ch))
        strcat(buf, " (Red Aura)");
      else if (IS_GOOD(ch))
        strcat(buf, " (Blue Aura)");
    }
    else
    {
      if (IS_EVIL(i))
        strcat(buf, " (Red Aura)");
      else if (IS_GOOD(i))
        strcat(buf, " (Blue Aura)");
    }
  }
}

ACMD(do_autocon)
{
  if (PRF_FLAGGED(ch, PRF_AUTOCON)) {
	  REMOVE_BIT(PRF_FLAGS(ch), PRF_AUTOCON);
		SEND_TO_CHAR(ch, "Auto-consider has been turned off.\r\n");
		return;
	}
	else {
	  SET_BIT(PRF_FLAGS(ch), PRF_AUTOCON);
		SEND_TO_CHAR(ch, "Auto-consider has been turned on.\r\n");
		return;
	}
}

void list_one_npc(struct char_data * i, struct char_data * ch)
{
  char mobLev[10];
	char *color = NULL;
	int j, lvl_diff;
	char buf2[MAX_STRING_LENGTH];
	
		if (PRF_FLAGGED(ch, PRF_AUTOCON)) {
		  switch (lvl_diff = (GET_LEVEL(i) - GET_LEVEL(ch))) {
			  case 0:
				  color = strdup("&W");
					break;
			  case 1:
				  color = strdup("&m");
					break;
			  case 2:
				  color = strdup("&M");
					break;
			  case 3:
				  color = strdup("&r");
					break;
			  case 4:
				  color = strdup("&R");
					break;
			  case -1:
				  color = strdup("&y");
					break;
			  case -2:
				  color = strdup("&Y");
					break;
				case -3:
				  color = strdup("&g");
					break;
			  case -4:
				  color = strdup("&G");
					break;
			  case -5:
				  color = strdup("&c");
					break;
			  case -6:
				  color = strdup("&C");
					break;
			  case -7:
				  color = strdup("&b");
					break;
			  case -8:
				  color = strdup("&B");
					break;
			  default:
				  if (lvl_diff < -8)
				    color = strdup("&K");
					else if (lvl_diff > 4)
					  color = strdup("&u&R");
					break;				
			}
			sprintf(buf2, "%s(&n", color);
			send_to_char(buf2, ch);
			for (j = 0; j < MIN(5, MAX(1, GET_MAX_HIT(i) / (GET_LEVEL(i) * 20))); j++) {
	      if (lvl_diff > 0)		{
			    sprintf(buf2, "%s+&n", color);
					send_to_char(buf2, ch);
				}
				else if (lvl_diff == 0) {
				  sprintf(buf2, "%s=&n", color);
					send_to_char(buf2, ch);
				}
				else {
	        sprintf(buf2, "%s-&n", color);				
					send_to_char(buf2, ch);
				}		
  		}
		  sprintf(buf2, "%s)&y ", color);
			send_to_char(buf2, ch);			
	  
	  }	
	
  if (IS_AFFECTED(i, AFF_INVISIBLE)) {
    strcpy(buf, "*");
  } else {
    *buf = '\0';
  }

  if (AFF_FLAGGED(i, AFF_HIDE) && (!check_skill(i, 101, SKILL_HIDE) || AFF_FLAGGED(ch, AFF_SENSE_LIFE)))
    strcat(buf, "(hiding) ");

  if (IS_NPC(i) && MOB_FLAGGED(i, MOB_QUEST) && GET_LEVEL(ch) > LVL_IMMORT)
    strcat(buf, "(quest mob) ");

  if (IS_AFFECTED(ch, AFF_DETECT_ALIGN)) {
    if (IS_EVIL(i)) {
      strcat(buf, "(Red Aura) ");
    } else if (IS_GOOD(i)) {
      strcat(buf, "(Blue Aura) ");
    }
  }

//  strcat(buf, i->player.long_descr);

  if(PRF_FLAGGED(ch, PRF_LEVEL_FLAGS))
  {
    sprintf(mobLev, " (%2d)", (int)GET_LEVEL(i));
  }
  else
  {
    strcpy(mobLev, "");
  }
  strcat(buf, mobLev);
  strcat(buf, i->player.long_descr);

}
void list_one_char(struct char_data * i, struct char_data * ch)
{

  if (IS_NPC(i) && i->player.long_descr && GET_POS(i) == GET_DEFAULT_POS(i)) {
    list_one_npc(i, ch);
    send_to_char(buf, ch);
    display_visual_effects(i, ch);
  }
  else
  {
    list_one_pc(i, ch);
    strcat(buf, "\r\n");
    send_to_char(buf, ch);
    display_visual_effects(i, ch);
  }
}

void list_char_to_char(struct char_data * list, struct char_data * ch)
{
  struct char_data *i;
  struct obj_data *o, *next_o;

  if (PLR_FLAGGED(ch, PLR_FIRE_ON)) {
    REMOVE_BIT(PLR_FLAGS(ch), PLR_FIRE_ON);
   }

  for (o = world[ch->in_room].contents; o; o = next_o) {
      next_o = o->next_content;
      if (GET_OBJ_TYPE(o) == ITEM_FIRE) {
          SET_BIT(PLR_FLAGS(ch), PLR_FIRE_ON);
                   }
           }

  for (i = list; i; i = i->next_in_room) {
    if (ch != i) {
  if (RIDDEN_BY(i) && RIDDEN_BY(i)->in_room == i->in_room)
        continue;    
      if (CAN_SEE(ch, i) || (AFF_FLAGGED(i, AFF_HIDE) && 
          (!check_skill(i, 101, SKILL_HIDE) || AFF_FLAGGED(ch, AFF_SENSE_LIFE)))) {
	list_one_char(i, ch);
      } else if (IS_DARK(ch->in_room) && !CAN_SEE_IN_DARK(ch) && 
               IS_AFFECTED(i, AFF_INFRAVISION) &&
               (GET_INVIS_LEV(i) <= GET_REAL_LEVEL(ch))) {
	send_to_char("You see a pair of glowing red eyes looking your way.\r\n", ch);
      }
    }
  }
}


void do_auto_exits(struct char_data * ch)
{
  int door;

  *buf = '\0';

  for (door = 0; door < NUM_OF_DIRS; door++)
    if (EXIT(ch, door) && EXIT(ch, door)->to_room != NOWHERE &&
	!IS_SET(EXIT(ch, door)->exit_info, EX_CLOSED))
      sprintf(buf, "%s%s ", buf, sdirs[door]);

  sprintf(buf2, "%s[ Exits: %s]%s\r\n", CCCYN(ch, C_NRM),
	  *buf ? buf : "None! ", CCNRM(ch, C_NRM));

  send_to_char(buf2, ch);
}


ACMD(do_exits)
{
  int door;
  struct obj_data *o, *next_o;


  *buf = '\0';

  if (IS_AFFECTED(ch, AFF_BLIND)) {
    send_to_char("You can't see a damned thing, you're blind!\r\n", ch);
    return;
  }

   if (PLR_FLAGGED(ch, PLR_FIRE_ON)) {
    REMOVE_BIT(PLR_FLAGS(ch), PLR_FIRE_ON);
   }

  for (o = world[ch->in_room].contents; o; o = next_o) {
      next_o = o->next_content;
      if (GET_OBJ_TYPE(o) == ITEM_FIRE) {
          SET_BIT(PLR_FLAGS(ch), PLR_FIRE_ON);
                   }
           }

  for (door = 0; door < NUM_OF_DIRS; door++)
if ((EXIT(ch, door) && EXIT(ch, door)->to_room != NOWHERE && !EXIT_FLAGGED(EXIT(ch, door), EX_CLOSED) && !EXIT_FLAGGED(EXIT(ch, door), EX_HIDDEN)) || (EXIT(ch, door) && EXIT(ch, door)->to_room != NOWHERE && !EXIT_FLAGGED(EXIT(ch, door), EX_CLOSED) && EXIT_FLAGGED(EXIT(ch, door), EX_ISDOOR) && EXIT_FLAGGED(EXIT(ch, door), EX_HIDDEN))) { 
      if (GET_LEVEL(ch) >= LVL_IMMORT)
	sprintf(buf2, "%-5s - [%5d] %s\r\n", dirs[door],
		world[EXIT(ch, door)->to_room].number,
		world[EXIT(ch, door)->to_room].name);
      else {
	sprintf(buf2, "%-5s - ", dirs[door]);
if (IS_DARK(EXIT(ch, door)->to_room) && !CAN_SEE_IN_DARK(ch))
	  strcat(buf2, "Too dark to tell\r\n");
	else {
	  strcat(buf2, world[EXIT(ch, door)->to_room].name);
	  strcat(buf2, "\r\n");
	}
      }
      strcat(buf, CAP(buf2));
    }
  send_to_char("Obvious exits:\r\n", ch);

  if (*buf)
    send_to_char(buf, ch);
  else
    send_to_char(" None.\r\n", ch);
}



void look_at_room(struct char_data * ch, int ignore_brief)
{

 struct obj_data *o, *next_o;

  const char *blood_messages[] = {
    "Should never see this.",
    "There are some old blood stains on the walls here.",
    "There is a small pool of blood here.",
    "You're standing a pool of blood.",
    "There is a fairly large pool of blood here.",
    "You are standing in a large pool of blood.",
    "Pools of blood are scattered around the room.",
    "The floor is covered with blood making it slippery.",
    "Blood flows down the walls onto the floor.",
    "The room is literally covered in blood.",
    "Blood stains the room and all those in it.",
    "\n"
  };

    if (PLR_FLAGGED(ch, PLR_FIRE_ON)) {
    REMOVE_BIT(PLR_FLAGS(ch), PLR_FIRE_ON);
   }

  for (o = world[ch->in_room].contents; o; o = next_o) {
      next_o = o->next_content;
      if (GET_OBJ_TYPE(o) == ITEM_FIRE) {
          SET_BIT(PLR_FLAGS(ch), PLR_FIRE_ON);
                   }
           }

  if (!ch->desc)
    return;

  if (IS_DARK(ch->in_room) && !CAN_SEE_IN_DARK(ch)) {
    send_to_char("It is pitch black...\r\n", ch);
    return;
  } else if (IS_AFFECTED(ch, AFF_BLIND)) {
    send_to_char("You see nothing but infinite darkness...\r\n", ch);
    return;
  }

  send_to_char(CCCYN(ch, C_NRM), ch);

  if (PRF_FLAGGED(ch, PRF_ROOMFLAGS)) {
    sprintbit(ROOM_FLAGS(ch->in_room), room_bits, buf);
    sprintf(buf2, "[%5d] %s [%s] [%s]", world[ch->in_room].number,
	    world[ch->in_room].name, buf,
	    sector_types[world[ch->in_room].sector_type]);
    send_to_char(buf2, ch);
  } else {
    send_to_char(world[ch->in_room].name, ch);
  }

  send_to_char(CCNRM(ch, C_NRM), ch);
  send_to_char("\r\n", ch);

  if (!PRF_FLAGGED(ch, PRF_BRIEF) || ignore_brief ||
      ROOM_FLAGGED(ch->in_room, ROOM_DEATH)) {
    send_to_char(world[ch->in_room].description, ch);
  }

  /* autoexits */
  if (PRF_FLAGGED(ch, PRF_AUTOEXIT) || IS_NPC(ch)) {
    do_auto_exits(ch);
  }

  /* now list characters & objects */
  send_to_char(CCGRN(ch, C_NRM), ch);
  list_obj_to_char(world[ch->in_room].contents, ch, 0, FALSE);
  send_to_char(CCYEL(ch, C_NRM), ch);
  list_char_to_char(world[ch->in_room].people, ch);
  send_to_char(CCNRM(ch, C_NRM), ch);

  if (RM_BLOOD(ch->in_room) > 0)
    act(blood_messages[MAX((RM_BLOOD(ch->in_room) / 2), 1)], FALSE, ch, 0, 0, TO_CHAR);

}



void look_in_direction(struct char_data * ch, int dir)
{
  if (EXIT(ch, dir)) {
    if (EXIT(ch, dir)->general_description)
      send_to_char(EXIT(ch, dir)->general_description, ch);
    else
      send_to_char("You see nothing special.\r\n", ch);

    if (IS_SET(EXIT(ch, dir)->exit_info, EX_CLOSED) && EXIT(ch, dir)->keyword
        && !IS_SET(EXIT(ch, dir)->exit_info, EX_HIDDEN))
    {
      sprintf(buf, "The %s is closed.\r\n", fname(EXIT(ch, dir)->keyword));
      send_to_char(buf, ch);
    } else if (IS_SET(EXIT(ch, dir)->exit_info, EX_ISDOOR) &&
              (EXIT(ch, dir)->keyword) && !IS_SET(EXIT(ch, dir)->exit_info, EX_HIDDEN))
    {
      sprintf(buf, "The %s is open.\r\n", fname(EXIT(ch, dir)->keyword));
      send_to_char(buf, ch);
    }
  } else
    send_to_char("You see nothing special.\r\n", ch);
}



void look_in_obj(struct char_data * ch, char *arg)
{
  struct obj_data *obj = NULL;
  struct char_data *dummy = NULL;
  int amt, bits;

  if (!*arg)
    send_to_char("Look in what?\r\n", ch);
  else if (!(bits = generic_find(arg, FIND_OBJ_INV | FIND_OBJ_ROOM |
				 FIND_OBJ_EQUIP, ch, &dummy, &obj))) {
    sprintf(buf, "There doesn't seem to be %s %s here.\r\n", AN(arg), arg);
    send_to_char(buf, ch);
  } else if ((GET_OBJ_TYPE(obj) != ITEM_DRINKCON) &&
	     (GET_OBJ_TYPE(obj) != ITEM_FOUNTAIN) &&
	     (GET_OBJ_TYPE(obj) != ITEM_CONTAINER) &&
             (GET_OBJ_TYPE(obj) != ITEM_SHEATH))
    send_to_char("There's nothing inside that!\r\n", ch);
  else {
    if (GET_OBJ_TYPE(obj) == ITEM_CONTAINER || GET_OBJ_TYPE(obj) == ITEM_SHEATH) {
      if (IS_SET(GET_OBJ_VAL(obj, 1), CONT_CLOSED))
	send_to_char("It is closed.\r\n", ch);
      else {
	send_to_char(fname(obj->name), ch);
	switch (bits) {
	case FIND_OBJ_INV:
	  send_to_char(" (carried): \r\n", ch);
	  break;
	case FIND_OBJ_ROOM:
	  send_to_char(" (here): \r\n", ch);
	  break;
	case FIND_OBJ_EQUIP:
	  send_to_char(" (used): \r\n", ch);
	  break;
	}

	list_obj_to_char(obj->contains, ch, 2, TRUE);
      }
    } else {		/* item must be a fountain or drink container */
      if (GET_OBJ_VAL(obj, 1) <= 0)
	send_to_char("It is empty.\r\n", ch);
      else {
	if (GET_OBJ_VAL(obj,0) <= 0 || GET_OBJ_VAL(obj,1)>GET_OBJ_VAL(obj,0)) {
	  sprintf(buf, "Its contents seem somewhat murky.\r\n"); /* BUG */
	} else {
	  amt = (GET_OBJ_VAL(obj, 1) * 3) / GET_OBJ_VAL(obj, 0);
	  sprinttype(GET_OBJ_VAL(obj, 2), color_liquid, buf2);
	  sprintf(buf, "It's %sfull of a %s liquid.\r\n", fullness[amt], buf2);
	}
	send_to_char(buf, ch);
      }
    }
  }
}



char *find_exdesc(char *word, struct extra_descr_data * list)
{
  struct extra_descr_data *i;

  for (i = list; i; i = i->next)
    if (isname(word, i->keyword))
      return (i->description);

  return NULL;
}


/*
 * Given the argument "look at <target>", figure out what object or char
 * matches the target.  First, see if there is another char in the room
 * with the name.  Then check local objs for exdescs.
 */
void look_at_target(struct char_data * ch, char *arg)
{
  int bits, found = 0, j;
  //int i = 0;
  struct char_data *found_char = NULL;
  struct obj_data *obj = NULL, *found_obj = NULL;
  char *desc;
  struct obj_data *o, *next_o;  int sneaking = FALSE;

     if (PLR_FLAGGED(ch, PLR_FIRE_ON)) {
    REMOVE_BIT(PLR_FLAGS(ch), PLR_FIRE_ON);
   }

  for (o = world[ch->in_room].contents; o; o = next_o) {
      next_o = o->next_content;
      if (GET_OBJ_TYPE(o) == ITEM_FIRE) {
          SET_BIT(PLR_FLAGS(ch), PLR_FIRE_ON);
                   }
           }


  if (!ch->desc)
    return;

  if (!*arg) {
    send_to_char("Look at what?\r\n", ch);
    return;
  }

  bits = generic_find(arg, FIND_OBJ_INV | FIND_OBJ_ROOM | FIND_OBJ_EQUIP |
                      FIND_CHAR_ROOM, ch, &found_char, &found_obj);


  /* Is the target a character? */
  if (found_char != NULL) {
    look_at_char(found_char, ch);	if (AFF_FLAGGED(ch, AFF_SNEAK) && check_skill(ch, 101, SKILL_SNEAK) && !AFF_FLAGGED(found_char, AFF_SENSE_LIFE)) 	  sneaking = TRUE;	  	
    if ((ch != found_char) && !sneaking)
    {
      if (CAN_SEE(found_char, ch))
	act("$n looks at you.", TRUE, ch, 0, found_char, TO_VICT);
      act("$n looks at $N.", TRUE, ch, 0, found_char, TO_NOTVICT);
    }
    return;
  }
  /* Does the argument match an extra desc in the room? */
  if ((desc = find_exdesc(arg, world[ch->in_room].ex_description)) != NULL) {
    page_string(ch->desc, desc, 0);
    return;
  }
  /* Does the argument match an extra desc in the char's equipment? */
  for (j = 0; j < NUM_WEARS && !found; j++)
    if (GET_EQ(ch, j) && CAN_SEE_OBJ(ch, GET_EQ(ch, j)))
      if ((desc = find_exdesc(arg, GET_EQ(ch, j)->ex_description)) != NULL) {
       send_to_char(desc, ch);	
       found = 1;
      }
  /* Does the argument match an extra desc in the char's inventory? */
  for (obj = ch->carrying; obj && !found; obj = obj->next_content) {
    if (CAN_SEE_OBJ(ch, obj))
	if ((desc = find_exdesc(arg, obj->ex_description)) != NULL) {
	send_to_char(desc, ch);
	found = 1;
      }
  }

  /* Does the argument match an extra desc of an object in the room? */
  for (obj = world[ch->in_room].contents; obj && !found; obj = obj->next_content)
    if (CAN_SEE_OBJ(ch, obj))
	if ((desc = find_exdesc(arg, obj->ex_description)) != NULL) {
	send_to_char(desc, ch);
	found = 1;
      }
  if (bits) {			/* If an object was found back in
				 * generic_find */
    if (!found)
      show_obj_to_char(found_obj, ch, 5);	/* Show no-description */
    else
      show_obj_to_char(found_obj, ch, 6);	/* Find hum, glow etc */
  } else if (!found)
    send_to_char("You do not see that here.\r\n", ch);
}


ACMD(do_look)
{
  static char arg2[MAX_INPUT_LENGTH];
  int look_type;
  struct obj_data *o, *next_o;

    if (PLR_FLAGGED(ch, PLR_FIRE_ON)) {
    REMOVE_BIT(PLR_FLAGS(ch), PLR_FIRE_ON);
   }

  for (o = world[ch->in_room].contents; o; o = next_o) {
      next_o = o->next_content;
      if (GET_OBJ_TYPE(o) == ITEM_FIRE) {
          SET_BIT(PLR_FLAGS(ch), PLR_FIRE_ON);
                   }
           }

  if (!ch->desc)
    return;

  if (GET_POS(ch) < POS_SLEEPING)
    send_to_char("You can't see anything but stars!\r\n", ch);
  else if (IS_AFFECTED(ch, AFF_BLIND))
    send_to_char("You can't see a damned thing, you're blind!\r\n", ch);
  else if (IS_DARK(ch->in_room) && !CAN_SEE_IN_DARK(ch)) {
    send_to_char("It is pitch black...\r\n", ch);
    list_char_to_char(world[ch->in_room].people, ch);	/* glowing red eyes */
  } else {
    half_chop(argument, arg, arg2);

    if (subcmd == SCMD_READ) {
      if (!*arg)
	send_to_char("Read what?\r\n", ch);
      else
	look_at_target(ch, arg);
      return;
    }
    if (!*arg)			/* "look" alone, without an argument at all */
      look_at_room(ch, 1);
    else if (is_abbrev(arg, "in"))
      look_in_obj(ch, arg2);
    /* did the char type 'look <direction>?' */
    else if ((look_type = search_block(arg, dirs, FALSE)) >= 0)
      look_in_direction(ch, look_type);
    else if ((look_type = search_block(arg, sdirs, FALSE)) >= 0)
      look_in_direction(ch, look_type);
    else if (is_abbrev(arg, "at"))
      look_at_target(ch, arg2);
    else
      look_at_target(ch, arg);
  }
}



ACMD(do_examine)
{
  int bits;
  struct char_data *tmp_char;
  struct obj_data *tmp_object;

  one_argument(argument, arg);

  if (!*arg) {
    send_to_char("Examine what?\r\n", ch);
    return;
  }
  look_at_target(ch, arg);

  bits = generic_find(arg, FIND_OBJ_INV | FIND_OBJ_ROOM | FIND_CHAR_ROOM |
		      FIND_OBJ_EQUIP, ch, &tmp_char, &tmp_object);

  if (tmp_object) {
    if ((GET_OBJ_TYPE(tmp_object) == ITEM_DRINKCON) ||
	(GET_OBJ_TYPE(tmp_object) == ITEM_FOUNTAIN) ||
	(GET_OBJ_TYPE(tmp_object) == ITEM_CONTAINER) ||
        (GET_OBJ_TYPE(tmp_object) == ITEM_SHEATH)) {

      send_to_char("When you look inside, you see:\r\n", ch);
      look_in_obj(ch, arg);
    }
  }
}



ACMD(do_gold)
{
  int copper = money_to_copper(GET_MONEY(ch));

  if (copper == 0) {
    send_to_char("You're broke!\r\n", ch);
  } else {
    sprintf(buf, "You have %s.\r\n", describe_money(GET_MONEY(ch)));
    send_to_char(buf, ch);
  }
}


/* here is a revised version of the score :) */ 
ACMD(do_score)
{
    extern  char *  godSelected(struct char_data *ch);
    extern  const char *pc_class_types[];
    extern  const char *pc_race_types[];

    /* struct time_info_data playing_time; */
    if (IS_NPC(ch)) {
        return;
    }

    if ((age(ch)->month == 0) && (age(ch)->day == 0)) {
        strcat(buf, "  It's your birthday today.\r\n");
    } else {
        strcat(buf, "\r\n");
    }

    sprintf(buf, "\r\n");

    GET_NAME(ch, chname);
    sprintf(buf, "%s&CName: &R%s &W[ &RLevel %d %s %s&W ]&n", buf, chname,  
                                                             GET_LEVEL(ch), 
                                                             pc_race_types[(int)GET_RACE(ch)], 
                                                             pc_class_types[(int)GET_CLASS(ch)]);
    FREE_NAME(chname);

    sprintf(buf,"%s\r\n&CMoney: &W[ &R%s&W ]", buf, describe_money(GET_MONEY(ch)));

    //sprintf(buf,"%s \r\n&CAlignment: &W[&R%s %s&W]&n", buf, describe_ethos(GET_ETHOS(ch)), describe_align(GET_ALIGNMENT(ch)));
    sprintf(buf,"%s \r\n&CAlignment:     &W[ &R%s&W ]&n", buf, describe_char_align(GET_ETHOS(ch), GET_ALIGNMENT(ch)));

    /*  PDH  2/25/99 - god selection code  */
    if ( GET_CLASS(ch) == CLASS_CLERIC || GET_CLASS(ch) == CLASS_PALADIN ) {
        sprintf(buf,"%s (God: %s)", buf, godSelected(ch));
    }

    sprintf(buf, "%s\r\n&CHP (curr/max): &W[ &R%d&W / &R%d&W ]&n", buf, GET_HIT(ch), GET_MAX_HIT(ch));
    sprintf(buf, "%s\r\n&CMV (curr/max): &W[ &R%d&W / &R%d&W ]&n", buf, GET_MOVE(ch), GET_MAX_MOVE(ch));

    int full = GET_COND(ch, FULL);
    int thirst = GET_COND(ch, THIRST);
    char full_meter[24] = "";
    char thirst_meter[24] = "";
    int i = 0;
    int j = 0;

    if (full != -1) {
        for (i = 0; i < full; i++) {
            strcat(full_meter, "=");
        }
    } else {
        strcat(full_meter, "==========FULL==========");
    }

    if (thirst != -1) {
        for (j = 0; j < thirst; j++) {
            strcat(thirst_meter, "=");
        }
    } else {
        strcat(thirst_meter, "==========FULL==========");
    }

    sprintf(buf, "%s \r\n&CHunger: &W[&R%-24s&W]&n", buf, full_meter);
    sprintf(buf, "%s \r\n&CThirst: &W[&R%-24s&W]&n", buf, thirst_meter);

    sprintf(buf, "%s\r\n&CRP Experience Factor: &W[ &R%d&W ]&n", buf, GET_RPFACTOR(ch));

// New exp % code by Gicker
   int int_xp = 0;
   int int_percent;
   float percent = 0.0;
   float xp = (((float) GET_EXP(ch)) - ((float) level_exp(GET_CLASS(ch), GET_LEVEL(ch)))) /
              (((float) level_exp(GET_CLASS(ch), (GET_LEVEL(ch) + 1)) -
              (float) level_exp(GET_CLASS(ch), GET_LEVEL(ch))));
	
   if (((float) GET_EXP(ch) - (float) level_exp(GET_CLASS(ch), GET_LEVEL(ch))) < 0)	{
	
	  xp = (((float) level_exp(GET_CLASS(ch), GET_LEVEL(ch))) - (float) GET_EXP(ch)) /
	     ((float) level_exp(GET_CLASS(ch), (GET_LEVEL(ch) + 1)) -
              (float) level_exp(GET_CLASS(ch), GET_LEVEL(ch)));
    
	  xp *= (float) 1000.0;
	  percent = (int) xp % 10;
	  xp /= (float) 10;
	  int_xp = MAX(0, (int) xp);
	  int_percent = MAX(0, MIN((int) percent, 99));
	  
	  int_xp = int_xp - (int_xp * 2);
	
   } else {
	   xp *= (float) 1000.0;
	   percent = (int) xp % 10;
	   xp /= (float) 10;
	   int_xp = MAX(0, (int) xp);
	   int_percent = MAX(0, MIN((int) percent, 99));   
   }

   if (GET_LEVEL(ch) < 30) {
     sprintf(buf, "%s \r\n&CProgress toward next level: &W[ &R%d.%d%%&W ]&n", buf, int_xp, int_percent);
     sprintf(buf, "%s\r\n", buf);
   }
   //else
     //sprintf(buf, "%s\r\n&CExperience Points: &W[&R%d&W]&n\r\n", buf, GET_EXP(ch));
     // No-op

   sprintf(buf, "%s\r\n", buf);
  /* no need to change anything down here...yet */
  switch (GET_POS(ch)) {
  case POS_DEAD:
    strcat(buf, "You are DEAD!\r\n");
    break;
  case POS_MORTALLYW:
    strcat(buf, "You are mortally wounded!  You should seek help!\r\n");
    break;
  case POS_INCAP:
    strcat(buf, "You are incapacitated, slowly fading away...\r\n");
    break;
  case POS_STUNNED:
    strcat(buf, "You are stunned!  You can't move!\r\n");
    break;
  case POS_SLEEPING:
    strcat(buf, "You are sleeping.\r\n");
    break;
  case POS_RIDING:
    strcat(buf, "You are riding.\r\n");
    break;
  case POS_FISHING:
    strcat(buf, "You are fishing.\r\n");
    break;
  case POS_DIGGING:
    strcat(buf, "You are digging.\r\n");
    break;
  case POS_RESTING:
    strcat(buf, "You are resting.\r\n");
    break;
  case POS_SITTING:
    strcat(buf, "You are sitting.\r\n");
    break;
  case POS_FIGHTING:
    if (FIGHTING(ch))
    {
      GET_PERS(FIGHTING(ch), ch, chname);
      sprintf(buf + strlen(buf), "You are fighting %s.\r\n", chname);
      FREE_NAME(chname);
    }
    else
      strcat(buf, "You are fighting thin air.\r\n");
    break;
  case POS_STANDING:
    strcat(buf, "You are standing.\r\n");
    break;
  default:
    strcat(buf, "You are floating.\r\n");
    break;
  }
  
  {
    int drunkValue  = GET_COND(ch, DRUNK);
    int hungerValue = GET_COND(ch, FULL);
    int thirstValue = GET_COND(ch, THIRST);

    if (!IS_NPC(ch))
    {
      if (drunkValue > 10)
        strcat(buf, "You are very intoxicated.\r\n");
      else if (drunkValue > 8)
        strcat(buf, "You are intoxicated.\r\n");
      else if (drunkValue > 5)
        strcat(buf, "You are beginning to feel intoxicated.\r\n");
   
      if (hungerValue >= 0)
      { 
        if (hungerValue == 0)
          strcat(buf, "You are very hungry.\r\n");
        else if (hungerValue < 3) 
          strcat(buf, "You are hungry.\r\n");
        else if (hungerValue < 5) 
          strcat(buf, "You are beginning to feel hungry.\r\n");
      }

      if (thirstValue >= 0)
      {
        if (thirstValue == 0)
          strcat(buf, "You are very thirsty.\r\n");
        else if (thirstValue < 3) 
          strcat(buf, "You are thirsty.\r\n");
        else if (thirstValue < 5) 
          strcat(buf, "You are beginning to feel thirsty.\r\n");
      }
    }
  }

  if(GET_CLAN(ch))
  {
    sprintf(buf, "%sYou are a member of the %s.\r\n", buf, describe_clan(GET_CLAN(ch)));
  }

  send_to_char(buf, ch);
}
/* end of new_do_score */



ACMD(do_inventory)
{
  send_to_char("You are carrying:\r\n", ch);
  list_obj_to_char(ch->carrying, ch, 1, TRUE);
}


ACMD(do_equipment)
{
  int i, found = 0;
    struct obj_data *o, *next_o;
    struct obj_data *item, *ob, *next_ob;

   if (PLR_FLAGGED(ch, PLR_FIRE_ON)) {
    REMOVE_BIT(PLR_FLAGS(ch), PLR_FIRE_ON);
   }

  for (o = world[ch->in_room].contents; o; o = next_o) {
      next_o = o->next_content;
      if (GET_OBJ_TYPE(o) == ITEM_FIRE) {
          SET_BIT(PLR_FLAGS(ch), PLR_FIRE_ON);
                   }
           }

  send_to_char("You are using:\r\n", ch);
  for (i = 0; i < NUM_WEARS; i++) {
   if (GET_EQ(ch, wear_order_index[i]) && !hidden_eq(ch, GET_EQ(ch, wear_order_index[i]))) {
      if (CAN_SEE_OBJ(ch, GET_EQ(ch, wear_order_index[i]))) {
        send_to_char(where[wear_order_index[i]], ch);
        show_obj_to_char(GET_EQ(ch, wear_order_index[i]), ch, 1);
        item = GET_EQ(ch, wear_order_index[i]);
        if (GET_OBJ_TYPE(item) == ITEM_SHEATH) {
          for (ob = item->contains; ob; ob = next_ob) {
            next_ob = ob->next_content;
     if (CAN_WEAR(ob, ITEM_WEAR_SHEATHED_H) || CAN_WEAR(ob, ITEM_WEAR_SHEATHED_WA) || CAN_WEAR(ob, ITEM_WEAR_SHEATHED_WR) || CAN_WEAR(ob, ITEM_WEAR_SHEATHED_A) || CAN_WEAR(ob, ITEM_WEAR_SHEATHED_B)) {

         show_sheathed(ch, ob, item);
        }
      }
}
	found = TRUE;
      } else {
        send_to_char(where[wear_order_index[i]], ch);
	send_to_char("Something.\r\n", ch);
	found = TRUE;
      }
    }
  }
  if (!found) {
    send_to_char(" Nothing.\r\n", ch);
  }
}


ACMD(do_time)
{
  char *suf;
  int weekday, day;
  extern struct time_info_data time_info;
  extern const char *weekdays[];
  extern const char *month_name[];

  sprintf(buf, "It is %d o'clock %s, on ",
	  ((time_info.hours % 12 == 0) ? 12 : ((time_info.hours) % 12)),
	  ((time_info.hours >= 12) ? "pm" : "am"));

  /* 35 days in a month */
  weekday = ((35 * time_info.month) + time_info.day + 1) % 7;

  strcat(buf, weekdays[weekday]);
  strcat(buf, "\r\n");
  send_to_char(buf, ch);

  day = time_info.day + 1;	/* day in [1..35] */

  if (day == 1)
    suf = "st";
  else if (day == 2)
    suf = "nd";
  else if (day == 3)
    suf = "rd";
  else if (day < 20)
    suf = "th";
  else if ((day % 10) == 1)
    suf = "st";
  else if ((day % 10) == 2)
    suf = "nd";
  else if ((day % 10) == 3)
    suf = "rd";
  else
    suf = "th";

  sprintf(buf, "The %d%s Day of the %s, Year %d.\r\n",
	  day, suf, month_name[(int) time_info.month], time_info.year);

  send_to_char(buf, ch);
}


ACMD(do_weather)
{
  static char *sky_look[] = {
    "cloudless",
    "cloudy",
    "rainy",
  "lit by flashes of lightning"};

  if (OUTSIDE(ch)) {
    sprintf(buf, "The sky is %s and %s.\r\n", sky_look[weather_info.sky],
	    (weather_info.change >= 0 ? "you feel a warm wind from south" :
	     "your foot tells you bad weather is due"));
    send_to_char(buf, ch);
  } else
    send_to_char("You have no feeling about the weather at all.\r\n", ch);
}


ACMD(do_help)
{
  extern int top_of_helpt;
  extern struct help_index_element *help_table;
  extern char *help;

  int chk, bot, top, mid, minlen;

  if (!ch->desc)
    return;

  skip_spaces(&argument);

  if (!*argument) {
    page_string(ch->desc, help, 0);
    return;
  }
  if (!help_table) {
    send_to_char("No help available.\r\n", ch);
    return;
  }

  bot = 0;
  top = top_of_helpt;
  minlen = strlen(argument);

  for (;;) {
    mid = (bot + top) / 2;

    if (bot > top) {
      send_to_char("There is no help on that word.\r\n", ch);
      return;
    } else if (!(chk = strn_cmp(argument, help_table[mid].keyword, minlen))) {
      /* trace backwards to find first matching entry. Thanks Jeff Fink! */
      while ((mid > 0) &&
	 (!(chk = strn_cmp(argument, help_table[mid - 1].keyword, minlen))))
	mid--;
      page_string(ch->desc, help_table[mid].entry, 0);
      return;
    } else {
      if (chk > 0)
        bot = mid + 1;
      else
        top = mid - 1;
    }
  }
}


/*
 *  PDH 11/24/98
 *  check's a player's intro list to see if the player's name is known
 */
int known_player(const struct char_data * ch, struct char_data * who)
{
  int i;

  /*  Imms know everyone, and everyone knows imms, but nobody knows mobs  */
    if (IS_NPC(ch) || IS_NPC(who))
      return 0;
    if ((ch == who) || (GET_LEVEL(ch) >= LVL_IMMORT) ||
        (GET_LEVEL(who) >= LVL_IMMORT))
      return 1;

  for (i = 0; i < MAX_INTROS; i++)
  {
    GET_NAME(who, chname);
    if (!(str_cmp(GET_INTRO(ch, i), chname)) &&
         (GET_INTRONUM(ch, i) == GET_MAX_MANA(who)))
    {
      return 1;
    }
    FREE_NAME(chname);
  }

  return 0;
}


#define WHO_USAGE \
"format: who [minlev[-maxlev]] [-n name] [-c classlist] [-s] [-o] [-q] [-r] [-z]\r\n"


ACMD(do_who)
{
  struct descriptor_data *d;
  struct char_data *wch;
  char godbuf[MAX_STRING_LENGTH];
  char mortbuf[MAX_STRING_LENGTH];
  char name_search[MAX_NAME_LENGTH+1];
  char mode;
  struct obj_data *o, *next_o;

  int flag=0;
  int low = 0, high = LVL_IMPL, showclass = 0;
  bool who_room = FALSE, who_zone = FALSE, who_quest = 0;
  bool outlaws = FALSE, noimm = FALSE, nomort = FALSE;
  int gods = 0, morts = 0, known = 0, current_boot = 0;
  size_t i;

  /*
  const char *godlevs[LVL_IMPL - (LVL_IMMORT-1)] = {
    "&W  Immortal 101&n",
    "&Y  Immortal 102&n",
    "&R  Immortal 103&n",
    "&G  Immortal 104&n",
    "&W    God    105&n",
    "&C    God    106&n",
    "&Y    God    107&n",
    "&B    God    108&n",
    "&M    God    109&n",
    "&W   GrGod   110&n",
    "&C   GrGod   111&n",
    "&Y   GrGod   112&n",
    "&G   GrGod   113&n",
    "&M   GrGod   114&n",
    "&W -->&RImp&W<-- 115&n",
  };
  */



  skip_spaces(&argument);
  strcpy(buf, argument);
  name_search[0] = '\0';

  
    if (PLR_FLAGGED(ch, PLR_FIRE_ON)) {
    REMOVE_BIT(PLR_FLAGS(ch), PLR_FIRE_ON);
   }

  for (o = world[ch->in_room].contents; o; o = next_o) {
      next_o = o->next_content;
      if (GET_OBJ_TYPE(o) == ITEM_FIRE) {
          SET_BIT(PLR_FLAGS(ch), PLR_FIRE_ON);
                   }
           }


  if ( GET_LEVEL(ch) >= LVL_IMMORT ) {
    while (*buf) {
      half_chop(buf, arg, buf1);

      if (isdigit(*arg)) {
	sscanf(arg, "%d-%d", &low, &high);
	strcpy(buf, buf1);

      } else if (*arg == '-') {
	mode = *(arg + 1);        /* just in case; we destroy arg in the
				     switch */
	switch (mode) {
	case 'o':
	  outlaws = TRUE;
	  strcpy(buf, buf1);
	  break;
	case 'z':
	  who_zone = TRUE;
	  strcpy(buf, buf1);
	  break;
	case 'q':
	  who_quest = TRUE;
	  strcpy(buf, buf1);
	  break;
	case 'l':
	  half_chop(buf1, arg, buf);
	  sscanf(arg, "%d-%d", &low, &high);
	  break;
	case 'n':
	  half_chop(buf1, name_search, buf);
	  break;
	case 'r':
	  who_room = TRUE;
	  strcpy(buf, buf1);
	  break;
	case 'c':
	  half_chop(buf1, arg, buf);
	  for (i = 0; i < strlen(arg); i++)
	    showclass |= find_class_bitvector(arg[i]);
	  break;
	case 'i':
	  nomort = TRUE;
	  strcpy(buf, buf1);
	  break;
	case 'm':
	  noimm = TRUE;
	  strcpy(buf, buf1);
	  break;
	default:
	  send_to_char(WHO_USAGE, ch);
	  return;
	  break;
	}                         /* end of switch */

      } else {                    /* endif */
	send_to_char(WHO_USAGE, ch);
	return;
      }
    }                             /* end while (parser) */
  }

  strcpy(godbuf, "&Y&CGods of Krynn\r\n&Y-=-=-=-=-=-=-=-=-=-=-&n\r\n");
  strcpy(mortbuf,"&Y&RKnown Mortals\r\n&Y-=-=-=-=-=-=-=-=-=-=-&n\r\n");

  for (d = descriptor_list; d; d = d->next) {
    if (STATE(d) != CON_PLAYING)
      continue;

    if (d->original)
      wch = d->original;
    else if (!(wch = d->character))
      continue;
    if(GET_LEVEL(wch) < 31)   /* Only want to count player, not imms -gan */
      current_boot++;  /*  more accurate count of max connections this boot  */

    if (!CAN_SEE(ch, wch))
      continue;
    if (GET_LEVEL(wch) < low || GET_LEVEL(wch) > high)
      continue;
    if ((noimm && GET_LEVEL(wch) >= LVL_IMMORT) || (nomort &&
GET_LEVEL(wch) < LVL_IMMORT))
      continue;
    flag=0;
    GET_NAME(wch, chname);
    if (*name_search && str_cmp(chname, name_search) &&
      !strstr(GET_TITLE(wch), name_search))
    {
      flag=1;
    }
    FREE_NAME(chname);
   
    if (flag == 1) continue; 

    if (outlaws && !PLR_FLAGGED(wch, PLR_KILLER) &&
	!PLR_FLAGGED(wch, PLR_THIEF))
      continue;
    if (who_quest && !PRF_FLAGGED(wch, PRF_QUEST))
      continue;
    if (who_zone && world[ch->in_room].zone != world[wch->in_room].zone)
      continue;
    if (who_room && (wch->in_room != ch->in_room))
      continue;
    if (showclass && !(showclass & (1 << GET_CLASS(wch))))
      continue;

    /*  PDH 11/24/98 - intro system  */

 /*  Let's fix people with a NULL title, too - Soli, 8/99  */
    if (GET_TITLE(wch))
      sprintf(buf, "%s", GET_TITLE(wch));
    else
    {
      GET_NAME(wch, chname);
      sprintf(buf, "%s", chname);
      FREE_NAME(chname);
    }

    if (GET_LEVEL(wch) >= LVL_IMMORT) {
      sprintf(godbuf, "%s%s", godbuf, buf);
      gods++;
    } else {
      /* sprintf(mortbuf, "%s%s %s", mortbuf, GET_NAME(wch), GET_TITLE(wch)); */
      /* Okay, intros can be checked here now.  Soli, 7/29/99  */
      morts++;
      if (known_player(ch, wch))
      {
        sprintf(mortbuf, "%s%s", mortbuf, buf);
        known++;
      }
      else
      {
        if (IS_APPROVED(wch))
          sprintf(mortbuf, "%s%s", mortbuf, wch->player.short_descr);
        else
          sprintf(mortbuf, "%s%s", mortbuf, current_short_desc(wch));
      }
    }

    *buf = '\0';
    if (!IS_APPROVED(wch) && GET_LEVEL(wch) < LVL_IMMORT && GET_LEVEL(ch) >= LVL_IMMORT && wch->player.short_descr)
      strcat(buf, " &W(&Cawaiting approval&W)&n");
    if (GET_INVIS_LEV(wch))
      sprintf(buf, "%s &W(&Gi&C%3d&W)&n", buf, GET_INVIS_LEV(wch));
    else if (IS_AFFECTED(wch, AFF_INVISIBLE))
      strcat(buf, " &W(&Cinvis&W)&n");
    if (PLR_FLAGGED(wch, PLR_MAILING))
      strcat(buf, " &W(&Gmailing&W)&n");
    else if (PLR_FLAGGED(wch, PLR_WRITING))
      strcat(buf, " &W(&Gwriting&W)&n");

    if (PRF_FLAGGED(wch, PRF_DEAF))
      strcat(buf, " &Y(&Rdeaf&Y)&n");
    if (PRF_FLAGGED(wch, PRF_NOTELL))
      strcat(buf, " &W(&Rnotell&W)&n");
    if (PRF_FLAGGED(wch, PRF_QUEST))
      strcat(buf, " &B(&Mquest&B)&n");
    if ( (PLR_FLAGGED(wch, PLR_THIEF)) && (GET_LEVEL(ch) > LVL_IMMORT) )
    {
      if (GET_RACE(wch) == RACE_KENDER)
        strcat(buf, "&R(&YHANDLER&R)&n");
      else
        strcat(buf, " &R(&YTHIEF&R)&n");
    }
    if ( (PLR_FLAGGED(wch, PLR_KILLER)) && (GET_LEVEL(ch) > LVL_IMMORT) )
      strcat(buf, " &Y(&RKILLER&Y)&n");
    if (PRF_FLAGGED(wch, PRF_AFK))
      strcat(buf, " &B(&MAFK&B)&n");

    strcat(buf, "\r\n");

    if (GET_LEVEL(wch) >= LVL_IMMORT)
      strcat(godbuf, buf);
    else
      strcat(mortbuf, buf);

  }                             /* end of for */

  if (gods == 0)
    sprintf(godbuf, "%s<none>\r\n", godbuf);

  /*
    sprintf(godbuf, "%s\r\nThere %s %d visible immortal%s.\r\n", godbuf, (gods == 1 ? "is" : "are"), gods, (gods == 1 ? "" : "s"));
    */
  page_string(ch->desc, godbuf, 0);
  send_to_char("\r\n", ch);

  if (morts == 0)
    sprintf(mortbuf, "%s<none>\r\n", mortbuf);

  /*
    sprintf(mortbuf, "%s\r\nThere %s %d visible mortal%s.",
    mortbuf, (morts == 1 ? "is" : "are"), morts, (morts == 1 ? "" : "s"));
    */
  page_string(ch->desc, mortbuf, 0);
  send_to_char("\r\n", ch);

  if (current_boot > boot_high) {
    boot_high = current_boot;
  }

/*  Fixing the visible invisible imm bug - Soli, 7/27/99
  if ( GET_LEVEL(ch) >= LVL_IMMORT ) {
    sprintf(buf, "%sTotal visible players: %d.\r\n", buf, gods+morts);
    sprintf(buf, "%sMaximum number of players on this boot: %d.\r\n",
	    buf, boot_high);
  } else {
    sprintf(buf, "%sTotal players: %d.\r\n", buf, current_boot);
  }

  The problem is morts use the number of connections and not the number of
visible players.  So we can remedy that like this.  */

/*  Or we thought we could - then they miss intros.  Fixed - Soli, 7/29/99 */

  if (GET_LEVEL(ch) >= LVL_IMMORT)
  {
    sprintf(buf, "%sTotal visible players: %d.\r\n", buf, gods+morts);
    sprintf(buf, "%sMaximum number of players on this boot: %d.\r\n",
      buf, boot_high);
  }
  else
  {
    sprintf(buf, "%sTotal known players: %d.\r\n", buf, gods+known);
    sprintf(buf, "%sTotal visible players: %d.\r\n", buf, gods+morts);
  }

  send_to_char(buf, ch);
}

/* end of new do_who */

#define USERS_FORMAT \
"format: users [-l minlevel[-maxlevel]] [-n name] [-h host] [-c classlist] [-o] [-p]\r\n"


ACMD(do_users)
{
  char line[200], line2[220], idletime[10], classname[20];
  char state[30], *timeptr, *format, mode;
  char name_search[MAX_INPUT_LENGTH], host_search[MAX_INPUT_LENGTH];
  struct char_data *tch;
  struct descriptor_data *d;
  size_t i;
  int flag=0;
  int low = 0, high = LVL_IMPL, num_can_see = 0;
  int showclass = 0, outlaws = 0, playing = 0, deadweight = 0;

  host_search[0] = name_search[0] = '\0';

  strcpy(buf, argument);
  while (*buf) {
    half_chop(buf, arg, buf1);
    if (*arg == '-') {
      mode = *(arg + 1);  /* just in case; we destroy arg in the switch */
      switch (mode) {
      case 'o':
      case 'k':
	outlaws = 1;
	playing = 1;
	strcpy(buf, buf1);
	break;
      case 'p':
	playing = 1;
	strcpy(buf, buf1);
	break;
      case 'd':
	deadweight = 1;
	strcpy(buf, buf1);
	break;
      case 'l':
	playing = 1;
	half_chop(buf1, arg, buf);
	sscanf(arg, "%d-%d", &low, &high);
	break;
      case 'n':
	playing = 1;
	half_chop(buf1, name_search, buf);
	break;
      case 'h':
	playing = 1;
	half_chop(buf1, host_search, buf);
	break;
      case 'c':
	playing = 1;
	half_chop(buf1, arg, buf);
	for (i = 0; i < strlen(arg); i++)
	  showclass |= find_class_bitvector(arg[i]);
	break;
      default:
	send_to_char(USERS_FORMAT, ch);
	return;
	break;
      }				/* end of switch */

    } else {			/* endif */
      send_to_char(USERS_FORMAT, ch);
      return;
    }
  }				/* end while (parser) */
  strcpy(line,
	 "Num Class   Name         State          Idl Login@   Site\r\n");
  strcat(line,
	 "--- ------- ------------ -------------- --- -------- ------------------------\r\n");
  send_to_char(line, ch);

  one_argument(argument, arg);

  for (d = descriptor_list; d; d = d->next) {
    if (STATE(d) != CON_PLAYING && playing)
      continue;
    if (STATE(d) == CON_PLAYING && deadweight)
      continue;
    if (STATE(d) == CON_PLAYING) {
      if (d->original)
	tch = d->original;
      else if (!(tch = d->character))
	continue;

      if (*host_search && !strstr(d->host, host_search))
	continue;
      flag=0;
      GET_NAME(tch, chname);
      if (*name_search && str_cmp(chname, name_search))
      {
        flag=1;
      }
      FREE_NAME(chname);

      if (flag == 1) continue;

      if (!CAN_SEE(ch, tch) || GET_LEVEL(tch) < low || GET_LEVEL(tch) > high)
	continue;
      if (outlaws && !PLR_FLAGGED(tch, PLR_KILLER) &&
	  !PLR_FLAGGED(tch, PLR_THIEF))
	continue;
      if (showclass && !(showclass & (1 << GET_CLASS(tch))))
	continue;
      if (GET_INVIS_LEV(ch) > GET_LEVEL(ch))
	continue;

      if (d->original) {
	sprintf(classname, "[%2d %s]", GET_LEVEL(d->original),
		CLASS_ABBR(d->original));
      } else {
	sprintf(classname, "[%2d %s]", GET_LEVEL(d->character),
		CLASS_ABBR(d->character));
      }
    } else {
      strcpy(classname, "   -   ");
    }

    timeptr = asctime(localtime(&d->login_time));
    timeptr += 11;
    *(timeptr + 8) = '\0';

    if (STATE(d) == CON_PLAYING && d->original) {
      strcpy(state, "Switched");
    } else {
      strcpy(state, connected_types[STATE(d)]);
    }

    if (d->character && STATE(d) == CON_PLAYING && 
      GET_LEVEL(d->character) < 39) {
      sprintf(idletime, "%3d", d->character->char_specials.timer *
	      SECS_PER_MUD_HOUR / SECS_PER_REAL_MIN);
    } else {
      strcpy(idletime, "");
    }

    format = "%3d %-7s %-12s %-14s %-3s %-8s ";

    if (d->character && d->character->player.name) {
      if (d->original) {
        GET_NAME(d->original, chname);
	sprintf(line, format, d->desc_num, classname,
		chname, state, idletime, timeptr);
        FREE_NAME(chname);
      } else {
        GET_NAME(d->character, chname);
	sprintf(line, format, d->desc_num, classname,
		chname, state, idletime, timeptr);
        FREE_NAME(chname);
      }
    } else {
      sprintf(line, format, d->desc_num, "   -   ", "UNDEFINED",
	      state, idletime, timeptr);
    }

    if (d->host && *d->host)
      sprintf(line + strlen(line), "[%s]\r\n", d->host);
    else
      strcat(line, "[Hostname unknown]\r\n");

    if (STATE(d) != CON_PLAYING) {
      sprintf(line2, "%s%s%s", CCGRN(ch, C_SPR), line, CCNRM(ch, C_SPR));
      strcpy(line, line2);
    }
    if (STATE(d) != CON_PLAYING || 
      (STATE(d) == CON_PLAYING && CAN_SEE(ch, d->character))) {
      send_to_char(line, ch);
      num_can_see++;
    }
  }

  sprintf(line, "\r\n%d visible sockets connected.\r\n", num_can_see);
  send_to_char(line, ch);
}


/* Generic page_string function for displaying text */
ACMD(do_gen_ps)
{

  char arg[MAX_STRING_LENGTH];

  one_argument(argument, arg);

  switch (subcmd) {
  case SCMD_CREDITS:
    page_string(ch->desc, credits, 0);
    break;
  case SCMD_NEWS:
    //send_news(ch, arg);
    //page_string(ch->desc, news, 0);
    break;
  case SCMD_INFO:
    page_string(ch->desc, info, 0);
    break;
  case SCMD_WIZLIST:
    page_string(ch->desc, wizlist, 0);
    break;
  case SCMD_IMMLIST:
    page_string(ch->desc, immlist, 0);
    break;
  case SCMD_HANDBOOK:
    page_string(ch->desc, handbook, 0);
    break;
  case SCMD_POLICIES:
    //send_rules(ch);
    //page_string(ch->desc, policies, 0);
    break;
  case SCMD_MOTD:
    page_string(ch->desc, motd, 0);
    break;
  case SCMD_IMOTD:
    page_string(ch->desc, imotd, 0);
    break;
  case SCMD_CLEAR:
    send_to_char("\033[H\033[J", ch);
    break;
  case SCMD_VERSION:
    send_to_char(strcat(strcpy(buf, circlemud_version), "\r\n"), ch);
    break;
  case SCMD_WHOAMI:
    GET_NAME(ch, chname);
    send_to_char(strcat(strcpy(buf, chname), "\r\n"), ch);
    FREE_NAME(chname);
    break;
  default:
    return;
    break;
  }
}

/*
void send_rules(struct char_data *ch)
{

  MYSQL *conn;
  MYSQL_RES *res;
  MYSQL_ROW *row;

  char msg[MAX_STRING_LENGTH];
  char query[MAX_STRING_LENGTH];
  char output[MAX_STRING_LENGTH];

  char *server = "localhost";
  char *user = "web";
  char *password = "cowboys";
  char *database = "portaldb"; 

  conn = mysql_init(NULL);

  if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0)) {
    GET_NAME(ch, chname);
    sprintf(msg, "%s tried to read rules but MYSQL function could not connect to database '%s'\r\n", chname, database);
    FREE_NAME(chname);
    mudlog(msg, BRF, 31, TRUE);
    return;
  }

  sprintf(query, "SELECT * FROM jos_content WHERE id = 68");

  mysql_query(conn, query);

  res = mysql_use_result(conn);

  if (!res) {
    GET_NAME(ch, chname);
    sprintf(msg, "%s tried to read rules but MYSQL function could not pull up any records", chname);
    FREE_NAME(chname)
    mudlog(msg, BRF, 31, TRUE);
    return;
  }

  if ((row = mysql_fetch_row(res)) == NULL) {
    send_to_char("There are no rules posted currently.\r\n", ch);
    return;
  }

  sprintf(output, "%s:\r\n\r\nMore information on the rules can be found on the forum at http://ageoflegends.com/forums/"
                  "\r\n%s\r\n\r\n%s\r\n\r\n", row[1], row[5], row[6]);

  mysql_free_result(res);
  
  mysql_close(conn);

  page_string(ch->desc, output, 0);
 
}

void send_news(struct char_data *ch, char *arg)
{


  MYSQL *conn;
  MYSQL_RES *res;
  MYSQL_ROW *row;

  char msg[MAX_STRING_LENGTH];
  char query[MAX_STRING_LENGTH];
  char output[MAX_STRING_LENGTH];

  char *server = "localhost";
  char *user = "web";
  char *password = "cowboys";
  char *database = "portaldb"; 

  conn = mysql_init(NULL);

  if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0)) {
    GET_NAME(ch, chname);
    sprintf(msg, "%s tried to read news but MYSQL function could not connect to database '%s'\r\n", chname, database);
    FREE_NAME(chname);
    mudlog(msg, BRF, 31, TRUE);
    return;
  }

  if (!*arg) {

    sprintf(query, "SELECT * FROM jos_content WHERE state = 1 AND sectionid = 1 ORDER BY id DESC");

    mysql_query(conn, query);

    res = mysql_use_result(conn);

    if (!res) {
      GET_NAME(ch, chname);
      sprintf(msg, "%s tried to read news but MYSQL function could not pull up any records", chname);
      FREE_NAME(chname);
      mudlog(msg, BRF, 31, TRUE);
      return;
    }

    sprintf(output, "Age of Legends News Topics:\r\n(Type news <article id number> to read)\r\n\r\n");
    sprintf(output, "%sID # | Date                | Title\r\n", output);
    sprintf(output, "%s~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\r\n", output);    

    while ((row = mysql_fetch_row(res)) != NULL) {
      sprintf(output, "%s%-4s | %-19s | %-55s\r\n", output, row[0], row[11], row[1]);
    }

    mysql_free_result(res);

  }
  else {

    if (!isdigit(arg[0])) {
      send_to_char("The proper format for reading news is news <article id number>.  Ie. 'news 5'.\r\n", ch);
      return;
    }

    sprintf(query, "SELECT * from jos_content WHERE id = %s AND sectionid = 1 AND state = 1", arg);

    mysql_query(conn, query);

    res = mysql_use_result(conn);

    if (!res) {
      GET_NAME(ch, chname);
      sprintf(msg, "%s tried to read news article id '%s' but MYSQL function could not find it.", chname, arg);
      FREE_NAME(chname);
      mudlog(msg, BRF, 31, TRUE);
      return;
    }


    if ((row = mysql_fetch_row(res)) == NULL) {
      send_to_char("That article Id doesn't exist.\r\n", ch);
      return;
    }

    sprintf(output, "Article ID # %s: %s\r\n", row[0], row[1]);
    sprintf(output, "%s(Posted on %s)\r\n\r\n", output, row[11]);
    sprintf(output, "%s%s%s\r\n", output, row[5], row[6]);


    mysql_free_result(res);
  }
  
mysql_close(conn);

page_string(ch->desc, output, 0);


}
*/

/*  copied from act.build.c  */
#define ZONE_BUF(ch) ((ch)->player.zone_edit)
char  lvlStr[20];

char * areas_level(bool * lev, int toFile)
{
  int   i;

  strcpy(lvlStr, "");

  for(i=0; i<15; i++) {
    if ( lev[i] ) {
      strcat(lvlStr, (toFile) ? "1" : "|" );
    } else {
      strcat(lvlStr, (toFile) ? "0" : " " );
    }
  }

  return lvlStr;
}


void print_object_location(int num, struct obj_data * obj, struct char_data * ch,
			        int recur)
{
  if (num > 0)
    sprintf(buf, "O%3d. %-25s - ", num, obj->short_description);
  else
    sprintf(buf, "%33s", " - ");

  if (obj->in_room > NOWHERE) {
    sprintf(buf + strlen(buf), "[%5d] %s\n\r",
	    world[obj->in_room].number, world[obj->in_room].name);
    send_to_char(buf, ch);
  } else if (obj->carried_by) {
    GET_PERS(obj->carried_by, ch, chname);
    sprintf(buf + strlen(buf), "carried by %s\n\r", chname);
    FREE_NAME(chname);
    send_to_char(buf, ch);
  } else if (obj->worn_by) {
    GET_PERS(obj->worn_by, ch, chname);
    sprintf(buf + strlen(buf), "worn by %s\n\r", chname);
    FREE_NAME(chname);
    send_to_char(buf, ch);
  } else if (obj->in_obj) {
    sprintf(buf + strlen(buf), "inside %s%s\n\r",
	    obj->in_obj->short_description, (recur ? ", which is" : " "));
    send_to_char(buf, ch);
    if (recur)
      print_object_location(0, obj->in_obj, ch, recur);
  } else {
    sprintf(buf + strlen(buf), "in an unknown location\n\r");
    send_to_char(buf, ch);
  }
}



void perform_immort_where(struct char_data * ch, char *arg)
{
  register struct char_data *i;
  register struct obj_data *k;
  struct descriptor_data *d;
  int num = 0, found = 0;

  if (!*arg) {
    send_to_char("Players\r\n-------\r\n", ch);
    for (d = descriptor_list; d; d = d->next)
      if (STATE(d) == CON_PLAYING) {
	i = (d->original ? d->original : d->character);
	if (i && CAN_SEE(ch, i) && (i->in_room != NOWHERE)) {
	  if (d->original)
          {
            GET_NAME(i, chname);
            GET_NAME(d->character, chname2);
	    sprintf(buf, "%-20s - [%5d] %s (in %s)\r\n",
		    chname, world[d->character->in_room].number,
		 world[d->character->in_room].name, chname2);
            FREE_NAME(chname2);
            FREE_NAME(chname);
          }
	  else
          {
            GET_NAME(i, chname);
	    sprintf(buf, "%-20s - [%5d] %s\r\n", chname,
		    world[i->in_room].number, world[i->in_room].name);
            FREE_NAME(chname);
          }
	  send_to_char(buf, ch);
	}
      }
  } else {
    for (i = character_list; i; i = i->next)
      if (CAN_SEE(ch, i) && i->in_room != NOWHERE && isname(arg, i->player.name)) {
	found = 1;
        GET_NAME(i, chname);
	sprintf(buf, "M%3d. %-25s - [%5d] %s\r\n", ++num, chname,
		world[i->in_room].number, world[i->in_room].name);
        FREE_NAME(chname);
	send_to_char(buf, ch);
      }
    for (num = 0, k = object_list; k; k = k->next)
      if (CAN_SEE_OBJ(ch, k) && isname(arg, k->name)) {
	found = 1;
	print_object_location(++num, k, ch, TRUE);
      }
    if (!found)
      send_to_char("Couldn't find any such thing.\r\n", ch);
  }
}



ACMD(do_where)
{
  one_argument(argument, arg);

  if (GET_LEVEL(ch) >= LVL_IMMORT) {
    perform_immort_where(ch, arg);
  }
}



ACMD(do_levels)
{
  int i;

  if (IS_NPC(ch)) {
    send_to_char("You ain't nothin' but a hound-dog.\r\n", ch);
    return;
  }
  *buf = '\0';

  for (i = 1; i < LVL_IMMORT; i++) {
    sprintf(buf + strlen(buf), "[%2d] %8d-%-8d",
	    i, level_exp(GET_CLASS(ch), i)+1,
	    level_exp(GET_CLASS(ch), i+1));

    /*  PDH  2/5/99 - NOT USED
    switch (GET_SEX(ch)) {
    case SEX_MALE:
    case SEX_NEUTRAL:
      strcat(buf, title_male(GET_CLASS(ch), i));
      break;
    case SEX_FEMALE:
      strcat(buf, title_female(GET_CLASS(ch), i));
      break;
    default:
      send_to_char("Oh dear.  You seem to be sexless.\r\n", ch);
      break;
    }
    */

    strcat(buf, "\r\n");
  }

  sprintf(buf + strlen(buf), "[%2d] %8d          : Immortality\r\n",
	  LVL_IMMORT, level_exp(GET_CLASS(ch), LVL_IMMORT)+1);
  page_string(ch->desc, buf, 1);
}



ACMD(do_consider)
{
  struct char_data *victim;
  int diff;

  one_argument(argument, buf);

  if (!(victim = get_char_vis(ch, buf, FIND_CHAR_ROOM))) {
    send_to_char("Consider killing who?\r\n", ch);
    return;
  }

  if (victim == ch) {
    send_to_char("Easy!  Very easy indeed!\r\n", ch);
    return;
  }

  if (!IS_NPC(victim)) {
    send_to_char("Attack players at your own peril!\r\n", ch);
    return;
  }

  diff = (GET_LEVEL(victim) - GET_LEVEL(ch));

  if (diff <= -10)
    send_to_char("A waste of time!\r\n", ch);
  else if (diff <= -5)
    send_to_char("This foe stands no chance!\r\n", ch);
  else if (diff <= -2)
    send_to_char("You outclass your opponent.\r\n", ch);
  else if (diff <= -1)
    send_to_char("You barely outclass your opponent.\r\n", ch);
  else if (diff == 0)
    send_to_char("The perfect match!\r\n", ch);
  else if (diff <= 1)
    send_to_char("You feel slightly outclassed.\r\n", ch);
  else if (diff <= 2)
    send_to_char("You feel rather outclassed.\r\n", ch);
  else if (diff <= 3)
    send_to_char("You may want to get some friends.\r\n", ch);
  else if (diff <= 5)
    send_to_char("Did you bring a small army with you?\r\n", ch);
  else if (diff <= 10)
    send_to_char("Are you MAD!?\r\n", ch);
  else if (diff <= 100)
    send_to_char("You ARE mad!\r\n", ch);

}



ACMD(do_diagnose)
{
  struct char_data *vict;

  one_argument(argument, buf);

  if (*buf) {
    if (!(vict = get_char_vis(ch, buf, FIND_CHAR_ROOM))) {
      send_to_char(NOPERSON, ch);
    } else
      diag_char_to_char(vict, ch);
  } else {
    if (FIGHTING(ch))
      diag_char_to_char(FIGHTING(ch), ch);
    else
      send_to_char("Diagnose who?\r\n", ch);
  }
}


static const char *ctypes[] = {
"off", "sparse", "normal", "complete", "\n"};

ACMD(do_color)
{
  int tp;

  one_argument(argument, arg);

  if (!*arg) {
    sprintf(buf, "Your current color level is %s.\r\n", ctypes[COLOR_LEV(ch)]);
    send_to_char(buf, ch);
    return;
  }
  if (((tp = search_block(arg, ctypes, FALSE)) == -1)) {
    send_to_char("Usage: color { Off | Sparse | Normal | Complete }\r\n", ch);
    return;
  }
  REMOVE_BIT(PRF_FLAGS(ch), PRF_COLOR_1 | PRF_COLOR_2);
  SET_BIT(PRF_FLAGS(ch), (PRF_COLOR_1 * (tp & 1)) | (PRF_COLOR_2 * (tp & 2) >> 1));

  sprintf(buf, "Your %scolor%s is now %s.\r\n", CCRED(ch, C_SPR),
	  CCNRM(ch, C_OFF), ctypes[tp]);
  send_to_char(buf, ch);
}


ACMD(do_toggle)
{
  if (IS_NPC(ch))
    return;
  if (GET_WIMP_LEV(ch) == 0)
    strcpy(buf2, "OFF");
  else
    sprintf(buf2, "%-3d", GET_WIMP_LEV(ch));

  sprintf(buf,
	  "Hit Pnt Display: %-3s    "
          "     Brief Mode: %-3s\r\n"

	  "   Move Display: %-3s    "
          "   Compact Mode: %-3s\r\n"

	  "         NoTell: %-3s    "
	  "   Repeat Comm.: %-3s\r\n"

	  " Auto Show Exit: %-3s    "
	  "           Deaf: %-3s\r\n"

	  "     Wimp Level: %-3s    "
	  "    Color Level: %s\r\n",

	  ONOFF(PRF_FLAGGED(ch, PRF_DISPHP)),
	  ONOFF(PRF_FLAGGED(ch, PRF_BRIEF)),

	  ONOFF(PRF_FLAGGED(ch, PRF_DISPMOVE)),
	  ONOFF(PRF_FLAGGED(ch, PRF_COMPACT)),

	  ONOFF(PRF_FLAGGED(ch, PRF_NOTELL)),
	  YESNO(!PRF_FLAGGED(ch, PRF_NOREPEAT)),

	  ONOFF(PRF_FLAGGED(ch, PRF_AUTOEXIT)),
	  YESNO(PRF_FLAGGED(ch, PRF_DEAF)),

	  buf2,
	  ctypes[COLOR_LEV(ch)]);

  send_to_char(buf, ch);
}

struct sort_struct {
  int sort_pos;
  byte is_social;
} *cmd_sort_info = NULL;

int num_of_cmds;

int sort_commands_helper(const void *a, const void *b)
{
    return strcmp(complete_cmd_info[*(const int *)a].sort_as,
                  complete_cmd_info[*(const int *)b].sort_as);
}

void sort_commands(void) {
    int a, b, tmp;

    ACMD(do_action);

    num_of_cmds = 0;

    /*
     * first, count commands (num_of_commands is actually one greater than the
     * number of commands; it inclues the '\n'.
     */
    while (*cmd_info[num_of_cmds].command != '\n')
        num_of_cmds++;

    /* create data array */
    CREATE(cmd_sort_info, struct sort_struct, num_of_cmds);

    /* initialize it */
    for (a = 1; a < num_of_cmds; a++) {
        cmd_sort_info[a].sort_pos = a;
        cmd_sort_info[a].is_social = (cmd_info[a].command_pointer == do_action);
    }

    /* the infernal special case */
    cmd_sort_info[find_command("insult")].is_social = TRUE;

    /* Sort.  'a' starts at 1, not 0, to remove 'RESERVED' */
    for (a = 1; a < num_of_cmds - 1; a++)
        for (b = a + 1; b < num_of_cmds; b++)
            if (strcmp(cmd_info[cmd_sort_info[a].sort_pos].command,
                       cmd_info[cmd_sort_info[b].sort_pos].command) > 0) {
                tmp = cmd_sort_info[a].sort_pos;
                cmd_sort_info[a].sort_pos = cmd_sort_info[b].sort_pos;
                cmd_sort_info[b].sort_pos = tmp;
            }

}

//void sort_commands_bak(void)
//{
//  int a, b, tmp;
//
//  ACMD(do_action);
//
//  num_of_cmds = 0;
//
//  /*
//   * first, count commands (num_of_commands is actually one greater than the
//   * number of commands; it includes the '\n'.
//   */
//  while (*cmd_info[num_of_cmds].command != '\n')
//    num_of_cmds++;
//
//  printf("Num of commands: %d\n", num_of_cmds);
//  while(complete_cmd_info[num_of_cmds].command[0] != '\n')
//    printf("Command: %s\n", complete_cmd_info[num_of_cmds].command[0]);
//    num_of_cmds++;
//  num_of_cmds++;
//
//  /* create data array */
//  //CREATE(cmd_sort_info, struct sort_struct, num_of_cmds);
//  printf("Before create\n");
//  CREATE(cmd_sort_info, int, num_of_cmds);
//
//  /* initialize it */
//  printf("Before init\n");
//  for (a = 0; a < num_of_cmds; a++)
//  {
//    cmd_sort_info[a] = a;
//    //cmd_sort_info[a].is_social = (cmd_info[a].command_pointer == do_action);
//    //cmd_sort_info[a].is_social = (complete_cmd_info[a].command_pointer == do_action);
//  }
//
//  /* the infernal special case */
//  //cmd_sort_info[find_command("insult")].is_social = TRUE;
//
//  // Replaced the code block below with the qsort that is
//  // prototyped by the compiler.
//  // -- Branc, 12/21/11
//  printf("Before sort\n");
//  qsort(cmd_sort_info + 1, num_of_cmds - 2, sizeof(int), sort_commands_helper);
//
//  /* Sort.  'a' starts at 1, not 0, to remove 'RESERVED' */
//  for (a = 1; a < num_of_cmds - 1; a++)
//    for (b = a + 1; b < num_of_cmds; b++)
//      if (strcmp(cmd_info[cmd_sort_info[a].sort_pos].command, cmd_info[cmd_sort_info[b].sort_pos].command) > 0)
//      {
//        tmp = cmd_sort_info[a].sort_pos;
//        cmd_sort_info[a].sort_pos = cmd_sort_info[b].sort_pos;
//        cmd_sort_info[b].sort_pos = tmp;
//      }
//}

ACMD(do_commands)
{
    log("do_commands");
  int no, i, cmd_num;
  int wizhelp = 0, socials = 0;
  struct char_data *vict;

  one_argument(argument, arg);

  if (*arg) {
    if (!(vict = get_char_vis(ch, arg, FIND_CHAR_WORLD)) || IS_NPC(vict)) {
      send_to_char("Who is that?\r\n", ch);
      return;
   }
  if (ch != vict && GET_LEVEL(ch) < LVL_IMMORT) {
      send_to_char("You can't see other people's commands!\r\n", ch);
      return;
    }
  } else
    vict = ch;

  if (subcmd == SCMD_SOCIALS)
    socials = 1;
  else if (subcmd == SCMD_WIZHELP)
    wizhelp = 1;

  GET_NAME_II(vict, ch, chname);
  sprintf(buf, "The following %s%s are available to %s:\r\n",
	  wizhelp ? "privileged " : "",
	  socials ? "socials" : "commands",
	  vict == ch ? "you" : chname);
  FREE_NAME(chname);

  /* cmd_num starts at 1, not 0, to remove 'RESERVED' */
  for (no = 1, cmd_num = 1; cmd_num < num_of_cmds; cmd_num++)
  {
    i = cmd_sort_info[cmd_num].sort_pos;
    if (complete_cmd_info[i].minimum_level >= 0 && GET_TRUST_LEVEL(vict) >= complete_cmd_info[i].minimum_level && 
        (complete_cmd_info[i].minimum_level >= LVL_IMMORT) == wizhelp && (wizhelp || socials == cmd_sort_info[i].is_social))
    {
      sprintf(buf + strlen(buf), "%-11s", complete_cmd_info[i].command);
      if (!(no % 7))
        strcat(buf, "\r\n");
      no++;
    }
  }

  // Rewriting the above for loop to match updated code:
  /*for(no = 1, cmd_num = 1; complete_cmd_info[cmd_sort_info[cmd_num]].command[0] != '\n'; cmd_num++)
  {
    i = cmd_sort_info[cmd_num];

    if(complete_cmd_info[i].minimum_level < 0 || GET_LEVEL(vict) < complete_cmd_info[i].minimum_level)
    {
        continue;
    }

    if((complete_cmd_info[i].minimum_level >= LVL_IMMORT) != wizhelp)
    {
        continue;
    }

    if(!wizhelp && socials != (complete_cmd_info[i].command_pointer == do_action || complete_cmd_info[i].command_pointer == do_insult))
    {
        continue;
    }

    // This is for future use; I'll write the method, but
    // it won't be fully implemented. 
    *//*
    if(check_disabled(&complete_cmd_info[i]))
    {
        sprintf(arg, "(%s)", complete_cmd_info[i].command);
    }
    else
    {
        sprintf(arg, "%s", complete_cmd_info[i].command);
    }
    *//*

    // Comment out this line when the "check_disabled" method
    // is fully implemented. Use the "if" statement above to
    // determine how to display a command to the character if
    // it is disabled or not.
    sprintf(arg, "%s", complete_cmd_info[i].command);

    // I'll need to overload the "send_to_char" method for the
    // following line to work.
    //send_to_char(ch, "%-11s%s", arg, no++ % 7 == 0 ? "\r\n" : "");

    // For now, I'll do it this way.
    sprintf(buf, "%-11s%s", arg, no++ % 7 == 0 ? "\r\n" : "");
  }*/

  /*if(no % 7 != 1)
  {
    //send_to_char(ch, "\r\n");
    strcat(buf, "\r\n");
  }*/

  strcat(buf, "\r\n");
  send_to_char(buf, ch);
}


int bitVToInt(long bit)
{
  int ret;

  for(ret=-1; bit>0; ret++, bit /= 2) {
    ;
  }

  return ret;
}


int printAffStr(int val, struct char_data * ch)
{
  if ( val < 1 ) {
    send_to_char("\\c02You are affected by the following spells / skills\\c03:\\c00\r\n", ch);
    send_to_char("\\c06-------------------------------------------------\\c00\r\n", ch);
  }

  return 1;
}

   
ACMD(do_affected)
{
  /*
   * Routine to show the character what spells and skills (s)he is affected
   * from, and for how long a duration.
   * This is basically the same type of code present in do_show 'spells'
   *
   */
  int i, affPrint = 0;
//struct affected_type *aff;
//extern const char *affected_msg[];
  extern const char *skill_affect_mesg[];

  
/*
  if (ch->affected) {
    for (aff = ch->affected; aff; aff = aff->next)
    {
      int affV = bitVToInt(aff->bitvector);

      if ( affV < 0 ) {
	continue;
      }

      switch(aff->bitvector) {
      case AFF_GROUP:
      case AFF_SLEEP:
      case AFF_KNOCKOUT:
      case AFF_NOTRACK:
      case AFF_SNEAK:
      case AFF_HIDE:
      case AFF_FREEZE:
      case AFF_UNDEAD:
      case AFF_UNUSED_1:
	continue;
	break;

      default:
	affPrint += printAffStr(affPrint, ch);
	sprintf(buf, "%s\r\n", affected_msg[affV]);
	send_to_char(buf, ch);
	break;
      }
    }

    if (affected_by_spell(ch, SPELL_ARMOR)) {
      affPrint += printAffStr(affPrint, ch);
      send_to_char("You feel protected.\r\n", ch);
    }
    if (affected_by_spell(ch, SPELL_STRENGTH)) {
      affPrint += printAffStr(affPrint, ch);
      send_to_char("You feel unusually strong.\r\n", ch);
    }
    if (affected_by_spell(ch, SPELL_LIGHT)) {
      affPrint += printAffStr(affPrint, ch);
      send_to_char("Your body emits a bright light.\r\n", ch);
    }
    if (affected_by_spell(ch, SPELL_DARKNESS)) {
      affPrint += printAffStr(affPrint, ch);
      send_to_char("A cloak of darkness surrounds you.\r\n", ch);
    }
    if (affected_by_spell(ch, SPELL_SILENCE) ||
	affected_by_spell(ch, SPELL_SPHERE_SILENCE) ) {
      affPrint += printAffStr(affPrint, ch);
      send_to_char("Your throat is constricted.\r\n", ch);
    }

   if (IS_AFFECTED(ch, AFF_JAWSTRIKE)) {
      affPrint += printAffStr(affPrint, ch);
      send_to_char("Your jaw is badly swollen.\r\n", ch);
    }

    if (affected_by_spell(ch, SPELL_SPIRIT_HAMMER)) {
      affPrint += printAffStr(affPrint, ch);
      send_to_char("A spiritual hammer floats beside you.\r\n", ch);
    }
    if (affected_by_spell(ch, SPELL_AID) ||
	affected_by_spell(ch, SPELL_AID) ) {
      affPrint += printAffStr(affPrint, ch);
      send_to_char("You feel righteous.\r\n", ch);
    }


    if ( ! affPrint ) {
      send_to_char("You are not affected by any spells or skills!\r\n", ch);
    }

*/
/*
  if (!ch->affected)
  {
    send_to_char("You are not affected by any spells.\r\n", ch);
    return;
  }
*/
  for (i = 0; i <= MAX_SPELLS; i++)
  {
    if (affected_by_spell(ch, i))
    {
    if ((affected_by_spell(ch, i) == affected_by_spell(ch, SPELL_DONTUSEME)) && AFF_FLAGGED(ch, AFF_JAWSTRIKE)) {
      affPrint += printAffStr(affPrint, ch);
      sprintf(buf, "Your jaw is swollen so bad you could not speak.\r\n");
      send_to_char(buf, ch);
     }
  else if ((affected_by_spell(ch, i) == affected_by_spell(ch, SPELL_DONTUSEME)) && AFF_FLAGGED(ch, AFF_KNOCKOUT)) {
      affPrint += printAffStr(affPrint, ch);
      sprintf(buf, "You are unconscious.\r\n");
      send_to_char(buf, ch);
     } 

  else {
      affPrint += printAffStr(affPrint, ch);
      sprintf(buf, "%s\r\n", skill_affect_mesg[i]);
      send_to_char(buf, ch);
     }
    }
  }

  if (IS_AFFECTED(ch, AFF_SNEAK)) {
    affPrint += printAffStr(affPrint, ch);
    sprintf(buf, "You are currently attempting to move silently.\r\n");
    send_to_char(buf, ch);
  }

  if (IS_AFFECTED(ch, AFF_HIDE)) {
    affPrint += printAffStr(affPrint, ch);
    sprintf(buf, "You are currently attempting to hide from others.\r\n");
    send_to_char(buf, ch);
  }

  if (!affPrint)
  {
    send_to_char("You are not affected by any spells.\r\n", ch);
    return;
  }

}

          

ACMD(do_scan)
{
  struct char_data *i;
  int is_in, dir, dis, maxdis, found = 0;
	int hidden = FALSE;

  const char *distance[] = {
    "right here",
    "immediately ",
    "nearby ",
    "a ways ",
    "far ",
    "very far ",
    "extremely far ",
    "impossibly far ",
  };

  if (IS_AFFECTED(ch, AFF_BLIND)) {
    act("You can't see anything, you're blind!", TRUE, ch, 0, 0, TO_CHAR);
    return;
  }

  if ((GET_MOVE(ch) < 3) && (GET_LEVEL(ch) < LVL_IMMORT)) {
    act("You are too exhausted.", TRUE, ch, 0, 0, TO_CHAR);
    return;
  }

  maxdis = (1 + ((GET_SKILL(ch, SKILL_SCAN) * 5) / 100));

  if (GET_LEVEL(ch) >= LVL_IMMORT) {
    maxdis = 7;
  }

  act("You quickly scan the area and see:", TRUE, ch, 0, 0, TO_CHAR);

  if (GET_LEVEL(ch) < LVL_IMMORT) {
    GET_MOVE(ch) -= 3;
  }

  is_in = ch->in_room;
	
  for (dir = 0; dir < NUM_OF_DIRS; dir++) {
    ch->in_room = is_in;
    for (dis = 0; dis <= maxdis; dis++) {
      if (((dis == 0) && (dir == 0)) || (dis > 0)) {
        for (i = world[ch->in_room].people; i; i = i->next_in_room) {

          if (AFF_FLAGGED(i, AFF_HIDE) && check_skill(i, 101, SKILL_HIDE) && !IS_AFFECTED(ch, AFF_SENSE_LIFE))
            hidden = TRUE;
          if ( (!((ch == i) && (dis == 0))) &&
               CAN_SEE(ch, i) && !hidden) {
            CHOOSE_NAME_II(i, ch, chname);
            sprintf(buf, "%33s: %s%s%s%s", chname, distance[dis],
                    ((dis > 0) && (dir < (NUM_OF_DIRS - 2))) ? "to the " : "",
                    (dis > 0) ? dirs[dir] : "",
                    ((dis > 0) && (dir > (NUM_OF_DIRS - 3))) ? "wards" : "");
            FREE_NAME(chname);
            act(buf, TRUE, ch, 0, 0, TO_CHAR);
            found++;
          }
          hidden = FALSE;
        }
      }

      if (!CAN_GO(ch, dir) ||
	  (world[ch->in_room].dir_option[dir]->to_room == is_in)) {
        break;
      } else {
        ch->in_room = world[ch->in_room].dir_option[dir]->to_room;
      }
    }
  }

  if (found == 0) {
    act("Nobody anywhere near you.", TRUE, ch, 0, 0, TO_CHAR);
  }

  ch->in_room = is_in;
}

extern struct player_index_element *player_table;
extern int top_of_p_table;

ACMD(do_players)
{
/*
  struct descriptor_data *d;
  char buf[200];

  sprintf(buf, "&Y%-20s %-5s %-4s %-5s %-5s %-5s %-7s %-7s %-14s\r\n", "Name", "Level", "Race", "Class", "Ethos", "Align", "Room", "ImmChar", "Needs Approval");
  send_to_char(buf, ch);

  sprintf(buf, "--------------------------------------------------------------------------------&n\r\n");
  send_to_char(buf, ch);

  for (d = descriptor_list; d; d = d->next) {
    if (STATE(d) == CON_PLAYING) {

      GET_NAME(d->character, chname);
      sprintf(buf, "&w%-20s %-5d %-4s %-5s %-5d %-5d %-7d %-7s %-14s&n\r\n",
        chname,
        GET_LEVEL(d->character),
        race_abbrevs[GET_RACE(d->character)],
        class_abbrevs[(int)GET_CLASS(d->character)],
        GET_ETHOS(d->character),
        GET_ALIGNMENT(d->character),
        GET_ROOM_VNUM(IN_ROOM(d->character)),
        (PLR_FLAGGED(d->character, PLR_IMMCHAR) || GET_LEVEL(d->character) >= LVL_IMMORT) ? "Yes" : "",
        IS_APPROVED(d->character) ? "" : "Yes"
      );
      send_to_char(buf, ch);
      FREE_NAME(chname);
    }
    else {
      GET_NAME(d->character, chname);
      sprintf(buf, "&w%-20s %-5d %-4s %-5s %-5d %-5d %-5s %-7s %-14s&n\r\n",
        chname,
        GET_LEVEL(d->character),
        race_abbrevs[GET_RACE(d->character)],
        class_abbrevs[(int)GET_CLASS(d->character)],
        GET_ETHOS(d->character),
        GET_ALIGNMENT(d->character),
        "Offline",
        (PLR_FLAGGED(d->character, PLR_IMMCHAR) || GET_LEVEL(d->character) >= LVL_IMMORT) ? "Yes" : "",
        IS_APPROVED(d->character) ? "" : "Yes"
      );
      send_to_char(buf, ch);
      FREE_NAME(chname);
    }
  }

  return;
*/
// Below is old do_players code
  int i, count = 0;
  *buf = 0;
 
  for (i = 0; i <= top_of_p_table; i++) {
    if ((player_table + i)->name != (char*) NULL)
    { 
      sprintf(buf, "%s  %-20.20s", buf, (player_table + i)->name);
      count++;
      if (count == 3) {
        count = 0;
        strcat(buf, "\r\n");
      }
    }
  }
  if(strlen(buf) >= MAX_STRING_LENGTH - 32)
     mudlog("[] Players file is too big to be parsed with 'players'.", BRF, 31, TRUE);
  else
    page_string(ch->desc, buf, 1);
}

ACMD(do_plip)
{
  int i = 0;
  char *nm;
  char *char_status;
  struct char_file_u chdata;

  *buf = 0;

  for (i = 0; i <= top_of_p_table; i++)
  {
    if(strlen(buf) >= MAX_STRING_LENGTH - 32)
    {
       mudlog("[] Players file is too big to be parsed with 'plip'.", BRF, 31, TRUE);
       return;
    }

    if ((player_table + i)->name != (char*) NULL)
    {
       nm = (player_table + i)->name;
       load_char(nm, &chdata);

       if (IS_SET(chdata.char_specials_saved.act, PLR_DELETED)) {
           char_status = "D";
       } else {
           char_status = "A";
       }

       sprintf(buf, "%s%-20s (%s) [%s]\r\n", buf, chdata.name, char_status, chdata.host);
    }
  }
  if(strlen(buf) >= MAX_STRING_LENGTH - 32)
     mudlog("[] Players file is too big to be parsed with 'plip'.", BRF, 31, TRUE);
  else
    page_string(ch->desc, buf, 1);
}

void display_visual_effects(struct char_data* ch, struct char_data* to_ch)
{
  struct affected_type* aff = (struct affected_type*) NULL;
  int displayedSanct = 0;

  if (IS_NPC(ch))
  {
    if (IS_AFFECTED(ch, AFF_SANCTUARY))
    {
      act("...$e glows with a bright light!", FALSE, ch, 0, to_ch, TO_VICT);
      displayedSanct = 1;
    }
    if (IS_AFFECTED(ch, AFF_BLIND))
    {
      act("...$e is groping around blindly!", FALSE, ch, 0, to_ch, TO_VICT);
    }
  }

  if (ch->affected) 
  {
    for (aff = ch->affected; aff; aff = aff->next) 
    {
      memset(buf, (char) NULL, MAX_STRING_LENGTH);

      if ((aff->type >= 0) && (aff->type <= MAX_SPELLS))
      {
        switch (aff->type)
        {
           case SPELL_SANCTUARY:
           {
             if (displayedSanct)
             {
               continue;
             }

             strcpy(buf, "...$e glows with a bright light!");
             break;
           }
  
           case SPELL_MINOR_SANCTUARY:
           {
             if (displayedSanct)
             {
               continue;
             }

             strcpy(buf, "...$e glows with a faint light!");
             break;
           } 
  
           case SPELL_FAERIE_FIRE:
           {
             strcpy(buf, "...$e is outlined in flickering faerie fire!");
             break;
           }
  
           case SPELL_BLUR:
           {
             strcpy(buf, "...$s body is blurry and unfocused!");
             break;
           }

           case SPELL_MIRROR_IMAGE:
           case SPELL_DECEPTIVE_IMAGERY:
           {
             if (IS_NPC(ch))
             {
               list_one_npc(ch, to_ch);
             }
             else
             {
               list_one_pc(ch, to_ch);
             }
             break;
           }

           default:
           {
             /* No visual effect. */
             continue;
           } 
        }
        act(buf, FALSE, ch, 0, to_ch, TO_VICT);
      }
    }
  }
}


ACMD(do_show)
{
        char arg1[MAX_INPUT_LENGTH];
        char arg2[MAX_INPUT_LENGTH];
        char *desc;
        struct char_data *victim;
        struct obj_data *obj;
        struct obj_data *o, *next_o;
        int tmp;

        two_arguments(argument, arg1, arg2);

    if (PLR_FLAGGED(ch, PLR_FIRE_ON)) {
    REMOVE_BIT(PLR_FLAGS(ch), PLR_FIRE_ON);
   }

  for (o = world[ch->in_room].contents; o; o = next_o) {
      next_o = o->next_content;
      if (GET_OBJ_TYPE(o) == ITEM_FIRE) {
          SET_BIT(PLR_FLAGS(ch), PLR_FIRE_ON);
                   }
           }

        if(!*arg1){
          send_to_char("Show what to whom?\r\n", ch);
          return;
        }
        else if(!*arg2){
          send_to_char("Who did you want to show that to?\r\n", ch);
          return;
        }
        if(!(victim = get_char_vis(ch, arg2, FIND_CHAR_ROOM))){
          send_to_char(NOPERSON, ch);
          return;
        }
        else if(victim == ch){
          send_to_char("Ha Ha!  Why don''t you just look at it?\r\n", ch);
          return;
        }
	else if(GET_POS(victim) == POS_SLEEPING) {
	  send_to_char("You COULD show it to them, but they're sleeping...\r\n", ch);
	  return;
	}
        else if(AFF_FLAGGED(victim, AFF_BLIND)) {
          CHOOSE_NAME(victim, victname);
           sprintf(buf, "%s is blind and can see nothing.\r\n", victname);
          FREE_NAME(victname);
          send_to_char(buf, ch);
          return;
       }

   else if (IS_DARK(victim->in_room) && !CAN_SEE_IN_DARK(victim)) {
          send_to_char("It's too dark!\r\n", ch);

          CHOOSE_NAME(ch, chname);
      sprintf(buf, "%s tries to show you something, but it's too dark to see.\r\n", chname);
          FREE_NAME(chname);
          send_to_char(buf, victim);
          return;
       }
        else
          if((obj = get_obj_in_list_vis(ch, arg1, ch->carrying))  ||
             (obj = get_object_in_equip_vis(ch, arg1, ch->equipment, &tmp))) {
            if ((desc = find_exdesc(arg1, obj->ex_description)) != NULL) {
       if ((GET_OBJ_TYPE(obj) == ITEM_DRINKCON) ||
             (GET_OBJ_TYPE(obj) == ITEM_FOUNTAIN) ||
             (GET_OBJ_TYPE(obj) == ITEM_CONTAINER) ||
              (GET_OBJ_TYPE(obj) == ITEM_SHEATH)) {
       CHOOSE_NAME(ch, chname);
       CHOOSE_NAME(victim, victname);

              sprintf(buf, "You show %s to %s.\r\n", obj->short_description, victname);
              send_to_char(buf, ch);
           sprintf(buf, "%s shows you %s.\r\n", chname, obj->short_description);
              send_to_char(buf, victim);
              send_to_char(desc, victim);
              send_to_char("It contains :\r\n", victim);
              list_obj_to_char(obj->contains, victim, 2, TRUE);
             FREE_NAME(victname);
             FREE_NAME(chname);
}

else {
     
       CHOOSE_NAME(ch, chname);
       CHOOSE_NAME(victim, victname);

       sprintf(buf, "You show %s to %s.\r\n", obj->short_description, victname);
            send_to_char(buf, ch);
           sprintf(buf, "%s shows you %s.\r\n", chname, obj->short_description);
              send_to_char(buf, victim);
              send_to_char(desc, victim);
             FREE_NAME(victname);
             FREE_NAME(chname);
}
}
            else {
          CHOOSE_NAME(ch, chname);
          CHOOSE_NAME(victim, victname);

              sprintf(buf, "You show %s to %s.\r\n", obj->short_description, victname);
              send_to_char(buf, ch);
              sprintf(buf, "%s shows you %s.\r\nYou see nothing special about it.\r\n", chname,
                          obj->short_description);
              send_to_char(buf, victim);
              FREE_NAME(victname);
              FREE_NAME(chname);

                  }
          }
          else {
            sprintf(buf, "You dont seem to have %s %s.\r\n", AN(arg1), arg1);
            send_to_char(buf, ch);
          }

}


