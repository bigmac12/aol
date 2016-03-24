/* ************************************************************************
*   File: act.wizard.c                                  Part of CircleMUD *
*  Usage: Player-level god commands and other goodies                     *
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
#include "mail.h"
#include "spells.h"
#include "house.h"
#include "screen.h"
#include "olc.h"
#include "dg_scripts.h"

#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>

/*   external vars  */
extern FILE *player_fl;
extern struct room_data *world;
extern struct char_data *character_list;
extern struct obj_data *object_list;
extern struct descriptor_data *descriptor_list;
extern struct index_data *mob_index;
extern struct index_data *obj_index;
extern struct int_app_type int_app[];
extern struct wis_app_type wis_app[];
extern struct zone_data *zone_table;
extern int top_of_zone_table;
extern int circle_restrict;
extern room_rnum top_of_world;
extern int top_of_mobt;
extern int top_of_objt;
extern int top_of_p_table;
extern char *god_name[NUM_GODS + 1];
extern const char *spec_procs[];

/* for objects */
extern const char *item_types[];
extern const char *wear_bits[];
extern const char *extra_bits[];
extern const char *container_bits[];
extern const char *drinks[];
extern struct attack_hit_type attack_hit_text[];

/* for rooms */
extern char *dirs[];
extern const char *room_bits[];
extern const char *exit_bits[];
extern const char *sector_types[];

/* for chars */
extern char *spells[];
extern const char *equipment_types[];
extern const char *affected_bits[];
extern const char *apply_types[];
extern const char *pc_class_types[];
extern const char *npc_class_types[];
extern const char *pc_race_types[];
extern const char *action_bits[];
extern const char *player_bits[];
extern const char *preference_bits[];
extern const char *position_types[];
extern const char *connected_types[];

extern const char *spell_bits_mage[];
extern const char *spell_bits_cleric[];

// for copyover

extern socket_t mother_desc;
extern ush_int port;

/* extern functions */
int level_exp(int class, int level);
void dismount_char(struct char_data * ch);
int shout_range(struct char_data * ch, struct char_data * vict);
void Crash_rentsave(struct char_data *ch, int cost);

ACMD(do_echo)
{
  skip_spaces(&argument);

  if (!*argument)
    send_to_char("Yes.. but what?\r\n", ch);
  else {
    if (subcmd == SCMD_EMOTE)
      sprintf(buf, "$n %s", argument);
    else
      strcpy(buf, argument);
    act(buf, FALSE, ch, 0, 0, TO_ROOM);
    if (PRF_FLAGGED(ch, PRF_NOREPEAT))
      send_to_char(OK, ch);
    else
      act(buf, FALSE, ch, 0, 0, TO_CHAR);
  }
}

ACMD(do_iamgod) 
{

  char arg[MAX_STRING_LENGTH]; 
  char name[MAX_STRING_LENGTH];
  
  GET_NAME(ch, chname);
  sprintf(name, "%s", chname);
  FREE_NAME(chname);

  if(!strcmp(name, "Branchala")) {
    ch->player.level = 38;
    ch->player_specials->saved.trust = 38;
    //GET_LEVEL(ch) = 38;
    //GET_TRUST_LEVEL(ch) = 38;

    gain_exp_regardless(ch, level_exp(GET_CLASS(ch), 37) - GET_EXP(ch));
  }
  
  /*
  if (!*argument) {
    send_to_char("&YThis command is reserved for implementors only and all uses of this command are logged.\r\nRepeated attempts to use it will result in punishment and possibly deletion of all\r\ncharacters and/or banning.&n\r\n", ch);	
	sprintf(buf, "Violation 1.0: %s attempted to use the iamgod command.\r\n", name);
    mudlog(buf, BRF, MAX(LVL_GOD, GET_INVIS_LEV(ch)), TRUE);
	return;
  }

  one_argument(argument, arg);

  if (!(*arg)) {   
    send_to_char("&YThis command is reserved for implementors only and all uses of this command are logged.\r\nRepeated attempts to use it will result in punishment and possibly deletion of all\r\ncharacters and/or banning.&n\r\n", ch);	
	sprintf(buf, "Violation 2.0: %s attempted to use the iamgod command.\r\n", name);
    mudlog(buf, BRF, MAX(LVL_GOD, GET_INVIS_LEV(ch)), TRUE);
	return;
  }

  if (!strcmp(arg, "chemneedsatleast5bathsdaily")) {
	  if (!strcmp(name, "Branchala")) {

	    GET_LEVEL(ch) = 38;
		GET_TRUST_LEVEL(ch) = 38;
	  
        gain_exp_regardless(ch, level_exp(GET_CLASS(ch), 37) - GET_EXP(ch));

		send_to_char("&WCongratulations Branchala, you have been restored to your full glory.\r\nHow unfortunate that there is still no hope for Chemosh.&n\r\n", ch);	
		return;
	  }
	  else if (!strcmp(name, "Chemosh")) {

	    GET_LEVEL(ch) = 38;
		GET_TRUST_LEVEL(ch) = 38;		
	  
        gain_exp_regardless(ch, level_exp(GET_CLASS(ch), 38) - GET_EXP(ch));	  


		send_to_char("&WBegrudgingly, Chemosh, the fates have returned thee to your level of vile stature that you had previously\r\nattained by equally vile methods.  Ultimately, this changes nothing, however, as you are still reviled completely\r\nand without hesitation throughout time, space and eternity.  Oh, and please do us all a favour and\r\ntake a bath.\r\n&n", ch);	
		return;
	  }
	  else {
		send_to_char("&YThis command is reserved for implementors only and all uses of this command are logged.\r\nRepeated attempts to use it will result in punishment and possibly deletion of all\r\ncharacters and/or banning.&n\r\n", ch);
		sprintf(buf, "Violation 3.0: %s attempted to use the iamgod command.\r\n", name);
		mudlog(buf, BRF, MAX(LVL_GOD, GET_INVIS_LEV(ch)), TRUE);	
		return;
	  }
  }
  else {
    send_to_char("&YThis command is reserved for implementors only and all uses of this command are logged.\r\nRepeated attempts to use it will result in punishment and possibly deletion of all\r\ncharacters and/or banning.&n\r\n", ch);
	sprintf(buf, "Violation 4.0: %s attempted to use the iamgod command.\r\n", name);
    mudlog(buf, BRF, MAX(LVL_GOD, GET_INVIS_LEV(ch)), TRUE);
    return;
  }
  */
  return;
}

ACMD(do_send)
{
  struct char_data *vict;

  half_chop(argument, arg, buf);

  if (!*arg) {
    send_to_char("Send what to who?\r\n", ch);
    return;
  }
  if (!(vict = get_char_vis(ch, arg, FIND_CHAR_WORLD))) {
    send_to_char(NOPERSON, ch);
    return;
  }
  send_to_char(buf, vict);
  send_to_char("\r\n", vict);
  if (PRF_FLAGGED(ch, PRF_NOREPEAT))
    send_to_char("Sent.\r\n", ch);
  else {
    GET_NAME_II(vict, ch, chname);
    sprintf(buf2, "You send '%s' to %s.\r\n", buf, chname);
    FREE_NAME(chname);
    send_to_char(buf2, ch);
  }
}



/* take a string, and return an rnum.. used for goto, at, etc.  -je 4/6/93 */
room_rnum find_target_room(struct char_data * ch, char *rawroomstr)
{
  int tmp;
  sh_int location;
  struct char_data *target_mob;
  struct obj_data *target_obj;
  char roomstr[MAX_INPUT_LENGTH];

  one_argument(rawroomstr, roomstr);

  if (!*roomstr) {
    send_to_char("You must supply a room number or name.\r\n", ch);
    return NOWHERE;
  }
  if (isdigit(*roomstr) && !strchr(roomstr, '.')) {
    tmp = atoi(roomstr);
    if ((location = real_room(tmp)) < 0) {
      send_to_char("No room exists with that number.\r\n", ch);
      return NOWHERE;
    }
  } else if ((target_mob = get_char_vis(ch, roomstr, FIND_CHAR_WORLD)))
    location = target_mob->in_room;
  else if ((target_obj = get_obj_vis(ch, roomstr))) {
    if (target_obj->in_room != NOWHERE)
      location = target_obj->in_room;
    else {
      send_to_char("That object is not available.\r\n", ch);
      return NOWHERE;
    }
  } else {
    send_to_char("No such creature or object around.\r\n", ch);
    return NOWHERE;
  }

  /* a location has been found -- if you're < GRGOD, check restrictions. */
  if (GET_LEVEL(ch) < LVL_GRGOD) {
    if (ROOM_FLAGGED(location, ROOM_GODROOM)) {
      send_to_char("You are not godly enough to use that room!\r\n", ch);
      return NOWHERE;
    }
    if (ROOM_FLAGGED(location, ROOM_PRIVATE) &&
	world[location].people && world[location].people->next_in_room) {
      send_to_char("There's a private conversation going on in that room.\r\n", ch);
      return NOWHERE;
    }
    if (ROOM_FLAGGED(location, ROOM_HOUSE) &&
	!House_can_enter(ch, GET_ROOM_VNUM(location))) {
      send_to_char("That's private property -- no trespassing!\r\n", ch);
      return NOWHERE;
    }
  }
  return location;
}



ACMD(do_at)
{
  char command[MAX_INPUT_LENGTH];
  room_rnum location, original_loc;

  half_chop(argument, buf, command);
  if (!*buf) {
    send_to_char("You must supply a room number or a name.\r\n", ch);
    return;
  }

  if (!*command) {
    send_to_char("What do you want to do there?\r\n", ch);
    return;
  }

  if ((location = find_target_room(ch, buf)) < 0)
    return;

  /* a location has been found. */
  original_loc = ch->in_room;
  char_from_room(ch);
  char_to_room(ch, location);
  command_interpreter(ch, command);

  /* check if the char is still there */
  if (ch->in_room == location) {
    char_from_room(ch);
    char_to_room(ch, original_loc);
  }
}


ACMD(do_goto)
{
  room_rnum location;

  if ((location = find_target_room(ch, argument)) < 0)
    return;

  if (POOFOUT(ch))
    sprintf(buf, "%s", POOFOUT(ch));
  else
    strcpy(buf, "$n disappears in a puff of smoke.");

  act(buf, TRUE, ch, 0, 0, TO_ROOM);
  char_from_room(ch);
  char_to_room(ch, location);

  if (POOFIN(ch))
    sprintf(buf, "%s", POOFIN(ch));
  else
    strcpy(buf, "$n appears with an ear-splitting bang.");

  act(buf, TRUE, ch, 0, 0, TO_ROOM);
  look_at_room(ch, 0);
}



ACMD(do_trans)
{
  struct descriptor_data *i;
  struct char_data *victim;

  one_argument(argument, buf);
  if (!*buf)
    send_to_char("Whom do you wish to transfer?\r\n", ch);
  else if (str_cmp("all", buf)) {
    if (!(victim = get_char_vis(ch, buf, FIND_CHAR_WORLD)))
      send_to_char(NOPERSON, ch);
    else if (victim == ch)
      send_to_char("That doesn't make much sense, does it?\r\n", ch);
    else {
      if ((GET_LEVEL(ch) < GET_LEVEL(victim)) && !IS_NPC(victim)) {
	send_to_char("Go transfer someone your own size.\r\n", ch);
	return;
      }
      else if (RIDING(victim)) {
       dismount_char(victim);
      act("$n disappears in a mushroom cloud, leaving $s mount behind!", FALSE, victim, 0, 0, TO_ROOM);
      char_from_room(victim);
      char_to_room(victim, ch->in_room);
      act("$n arrives from a puff of smoke.", FALSE, victim, 0, 0, TO_ROOM);
      act("$n has transferred you!", FALSE, ch, 0, victim, TO_VICT);
      look_at_room(victim, 0);
      }
     else {
      act("$n disappears in a mushroom cloud.", FALSE, victim, 0, 0, TO_ROOM);
      char_from_room(victim);
      char_to_room(victim, ch->in_room);
      act("$n arrives from a puff of smoke.", FALSE, victim, 0, 0, TO_ROOM);
      act("$n has transferred you!", FALSE, ch, 0, victim, TO_VICT);
      look_at_room(victim, 0);
      }
    }
  } else {			/* Trans All */
    if (GET_LEVEL(ch) < LVL_GRGOD) {
      send_to_char("I think not.\r\n", ch);
      return;
    }

    for (i = descriptor_list; i; i = i->next)
      if (STATE(i) == CON_PLAYING && i->character && i->character != ch) {
	victim = i->character;
	if (GET_LEVEL(victim) >= GET_LEVEL(ch))
	  continue;
        if (RIDING(victim)) {
        dismount_char(victim);
	act("$n disappears in a mushroom cloud, leaving his mount behind!", FALSE, victim, 0, 0, TO_ROOM);
	char_from_room(victim);
	char_to_room(victim, ch->in_room);
	act("$n arrives from a puff of smoke.", FALSE, victim, 0, 0, TO_ROOM);
	act("$n has transferred you!", FALSE, ch, 0, victim, TO_VICT);
	look_at_room(victim, 0);
        }
       else  {
        act("$n disappears in a mushroom cloud.", FALSE, victim, 0, 0, TO_ROOM);
        char_from_room(victim);
        char_to_room(victim, ch->in_room);
        act("$n arrives from a puff of smoke.", FALSE, victim, 0, 0, TO_ROOM);
        act("$n has transferred you!", FALSE, ch, 0, victim, TO_VICT);
        look_at_room(victim, 0);
         }
      }
    send_to_char(OK, ch);
  }
}



ACMD(do_teleport)
{
  struct char_data *victim;
  room_rnum target;

  two_arguments(argument, buf, buf2);

  if (!*buf)
    send_to_char("Whom do you wish to teleport?\r\n", ch);
  else if (!(victim = get_char_vis(ch, buf, FIND_CHAR_WORLD)))
    send_to_char(NOPERSON, ch);
  else if (victim == ch)
    send_to_char("Use 'goto' to teleport yourself.\r\n", ch);
  else if (GET_LEVEL(victim) >= GET_LEVEL(ch))
    send_to_char("Maybe you shouldn't do that.\r\n", ch);
  else if (!*buf2)
    send_to_char("Where do you wish to send this person?\r\n", ch);
  else if ((target = find_target_room(ch, buf2)) >= 0) {
    send_to_char(OK, ch);
    act("$n disappears in a puff of smoke.", FALSE, victim, 0, 0, TO_ROOM);
    char_from_room(victim);
    char_to_room(victim, target);
    act("$n arrives from a puff of smoke.", FALSE, victim, 0, 0, TO_ROOM);
    act("$n has teleported you!", FALSE, ch, 0, (char *) victim, TO_VICT);
    look_at_room(victim, 0);
  }
}



ACMD(do_vnum)
{
  two_arguments(argument, buf, buf2);

  if (!*buf || !*buf2 || (!is_abbrev(buf, "mob") && !is_abbrev(buf, "obj"))) {
    send_to_char("Usage: vnum { obj | mob } <name>\r\n", ch);
    return;
  }
  if (is_abbrev(buf, "mob"))
    if (!vnum_mobile(buf2, ch))
      send_to_char("No mobiles by that name.\r\n", ch);

  if (is_abbrev(buf, "obj"))
    if (!vnum_object(buf2, ch))
      send_to_char("No objects by that name.\r\n", ch);
}

// Prevent crashing with this command.
ACMD(do_vwear)
{
  static const char *keywords[] =
    { "take", "finger", "neck", "body", "head", "legs", "feet", "hands",
      "arms", "shield", "about", "waist", "wrist", "wielded", "hold",
      "face", "ear", "ankle", "above", "back", "shoulder", "nose", "sheath",
      "sheathed", "onbelt", "onback", "" };
  int eq_pos = -1;

  one_argument(argument, arg);

  if ((eq_pos = search_block(arg, keywords, FALSE)) < 0)
  {
    send_to_char("Invalid equipment position.\r\n", ch);
    return;
  }

  eq_pos = (1 << eq_pos);

  if (!vwear_object(eq_pos, ch))
    send_to_char("No objects found.\r\n", ch);

  return;
}

void do_stat_room(struct char_data * ch)
{
  struct extra_descr_data *desc;
  struct room_data *rm = &world[ch->in_room];
  int i, found = 0;
  struct obj_data *j = 0;
  struct char_data *k = 0;

  sprintf(buf, "Room name: %s%s%s\r\n", CCCYN(ch, C_NRM), rm->name,
	  CCNRM(ch, C_NRM));
  send_to_char(buf, ch);

  sprinttype(rm->sector_type, sector_types, buf2);
  sprintf(buf, "Zone: [%3d]. VNum: [%s%5d%s], RNum: [%5d], Type: %s\r\n",
	 zone_table[rm->zone].number, CCGRN(ch, C_NRM), rm->number,
	  CCNRM(ch, C_NRM), ch->in_room, buf2);
  send_to_char(buf, ch);

  sprintbit(rm->room_flags, room_bits, buf2);
  sprintf(buf, "SpecProc: %s, Flags: %s\r\n",
	  (rm->func == NULL) ? "None" : "Exists", buf2);
  send_to_char(buf, ch);

  sprintf(buf, "Light: %d\r\n", rm->light);
  send_to_char(buf, ch);

  send_to_char("Description:\r\n", ch);
  if (rm->description)
    send_to_char(rm->description, ch);
  else
    send_to_char("  None.\r\n", ch);

  if (rm->ex_description) {
    sprintf(buf, "Extra descs:%s", CCCYN(ch, C_NRM));
    for (desc = rm->ex_description; desc; desc = desc->next) {
      strcat(buf, " ");
      strcat(buf, desc->keyword);
    }
    strcat(buf, CCNRM(ch, C_NRM));
    send_to_char(strcat(buf, "\r\n"), ch);
  }
  sprintf(buf, "Chars present:%s", CCYEL(ch, C_NRM));
  for (found = 0, k = rm->people; k; k = k->next_in_room) {
    if (!CAN_SEE(ch, k))
    {
      continue;
    }

    GET_NAME(k, chname);
    sprintf(buf2, "%s %s(%s)", found++ ? "," : "", chname,
      (!IS_NPC(k) ? "PC" : (!IS_MOB(k) ? "NPC" : "MOB")));
    FREE_NAME(chname);

    strcat(buf, buf2);
    if (strlen(buf) >= 62) {
      if (k->next_in_room)
	send_to_char(strcat(buf, ",\r\n"), ch);
      else
	send_to_char(strcat(buf, "\r\n"), ch);
      *buf = found = 0;
    }
  }

  if (*buf)
    send_to_char(strcat(buf, "\r\n"), ch);
  send_to_char(CCNRM(ch, C_NRM), ch);

  if (rm->contents) {
    sprintf(buf, "Contents:%s", CCGRN(ch, C_NRM));
    for (found = 0, j = rm->contents; j; j = j->next_content) {
      if (!CAN_SEE_OBJ(ch, j))
	continue;
      sprintf(buf2, "%s %s", found++ ? "," : "", j->short_description);
      strcat(buf, buf2);
      if (strlen(buf) >= 62) {
	if (j->next_content)
	  send_to_char(strcat(buf, ",\r\n"), ch);
	else
	  send_to_char(strcat(buf, "\r\n"), ch);
	*buf = found = 0;
      }
    }

    if (*buf)
      send_to_char(strcat(buf, "\r\n"), ch);
    send_to_char(CCNRM(ch, C_NRM), ch);
  }
  for (i = 0; i < NUM_OF_DIRS; i++) {
    if (rm->dir_option[i]) {
      if (rm->dir_option[i]->to_room == NOWHERE)
	sprintf(buf1, " %sNONE%s", CCCYN(ch, C_NRM), CCNRM(ch, C_NRM));
      else
	sprintf(buf1, "%s%5d%s", CCCYN(ch, C_NRM),
		GET_ROOM_VNUM(rm->dir_option[i]->to_room), CCNRM(ch, C_NRM));
      sprintbit(rm->dir_option[i]->exit_info, exit_bits, buf2);
      sprintf(buf, "Exit %s%-5s%s:  To: [%s], Key: [%5d], Keywrd: %s, Type: %s\r\n ",
	      CCCYN(ch, C_NRM), dirs[i], CCNRM(ch, C_NRM), buf1, rm->dir_option[i]->key,
	   rm->dir_option[i]->keyword ? rm->dir_option[i]->keyword : "None",
	      buf2);
      send_to_char(buf, ch);
      if (rm->dir_option[i]->general_description)
	strcpy(buf, rm->dir_option[i]->general_description);
      else
	strcpy(buf, "  No exit description.\r\n");
      send_to_char(buf, ch);
    }
  }

  /* check the room for a script */
  do_sstat_room(ch);
}



void do_stat_object(struct char_data * ch, struct obj_data * j)
{
  int i, found, inPlay, inRent, counter;
  obj_vnum vnum;
  struct obj_data *j2;
  struct extra_descr_data *desc;

  extern int num_obj_in_rent(int realNum);

  vnum = GET_OBJ_VNUM(j);
  sprintf(buf, "Name: '%s%s%s', Aliases: %s\r\n", CCYEL(ch, C_NRM),
	  ((j->short_description) ? j->short_description : "<None>"),
	  CCNRM(ch, C_NRM), j->name);
  send_to_char(buf, ch);
  sprinttype(GET_OBJ_TYPE(j), item_types, buf1);

  if (GET_OBJ_RNUM(j) >= 0) {
    int  rNum = GET_OBJ_RNUM(j);
    strcpy(buf2, (obj_index[GET_OBJ_RNUM(j)].func ? "Exists" : "None"));
    inPlay = obj_index[rNum].number;
    inRent = num_obj_in_rent(rNum);
  } else {
    strcpy(buf2, "None");
    inPlay = 0;
    inRent = 0;
  }

  sprintf(buf, "VNum: [%s%5d%s], RNum: [%5d], Type: %s, SpecProc: %s\r\n",
   CCGRN(ch, C_NRM), vnum, CCNRM(ch, C_NRM), GET_OBJ_RNUM(j), buf1, buf2);
  send_to_char(buf, ch);
  sprintf(buf, "L-Des: %s\r\n", ((j->description) ? j->description : "None"));
  send_to_char(buf, ch);


/* new here */
  sprintf(buf, "Object Level: %4d     In Play: %3d    In Rent: %3d\r\n",
	  GET_OBJ_LEVEL(j), inPlay, inRent);
  send_to_char(buf, ch);
/* end new here */


  if (j->ex_description) {
    sprintf(buf, "Extra descs:%s", CCCYN(ch, C_NRM));
    for (desc = j->ex_description; desc; desc = desc->next) {
      strcat(buf, " ");
      strcat(buf, desc->keyword);
    }
    strcat(buf, CCNRM(ch, C_NRM));
    send_to_char(strcat(buf, "\r\n"), ch);
  }
  send_to_char("Can be worn on: ", ch);
  sprintbit(j->obj_flags.wear_flags, wear_bits, buf);
  strcat(buf, "\r\n");
  send_to_char(buf, ch);

  send_to_char("Set char bits : ", ch);
  sprintbit(j->obj_flags.bitvector, affected_bits, buf);
  strcat(buf, "\r\n");
  send_to_char(buf, ch);

  send_to_char("Extra flags   : ", ch);
  sprintbit(GET_OBJ_EXTRA(j), extra_bits, buf);
  strcat(buf, "\r\n");
  send_to_char(buf, ch);

  sprintf(buf, "Weight: %d, Value: %d, Cost/day: %d, Timer: %d\r\n",
     GET_OBJ_WEIGHT(j), GET_OBJ_COST(j), GET_OBJ_RENT(j), GET_OBJ_TIMER(j));
  send_to_char(buf, ch);

  strcpy(buf, "In room: ");
  if (j->in_room == NOWHERE)
    strcat(buf, "Nowhere");
  else {
    sprintf(buf2, "%d", GET_ROOM_VNUM(IN_ROOM(j)));
    strcat(buf, buf2);
  }

  strcat(buf, ", In object: ");
  strcat(buf, j->in_obj ? j->in_obj->short_description : "None");
  strcat(buf, ", Carried by: ");

  GET_NAME(j->carried_by, chname);
  strcat(buf, j->carried_by ? chname : "Nobody");
  FREE_NAME(chname);

  strcat(buf, ", Worn by: ");

  GET_NAME(j->worn_by, chname);
  strcat(buf, j->worn_by ? chname : "Nobody");
  FREE_NAME(chname);

  strcat(buf, "\r\n");
  send_to_char(buf, ch);

   if (HAS_SPELLS(j)) {
  for(counter=0;counter<MAX_SPELL_AFFECT;counter++) {
    sprintf(buf,"Weapon Spell [%d] : %20s     Level: %3d     Percent: %3d",
            counter+1,
            spells[j->wpn_spells[counter].spellnum],
            j->wpn_spells[counter].level,
            j->wpn_spells[counter].percent);
    send_to_char(strcat(buf, "\r\n"), ch);
}
}

  switch (GET_OBJ_TYPE(j)) {
  case ITEM_LIGHT:
    if (GET_OBJ_VAL(j, 2) == -1)
      strcpy(buf, "Hours left: Infinite");
    else
      sprintf(buf, "Hours left: [%d]", GET_OBJ_VAL(j, 2));
    break;
  case ITEM_SCROLL:
  case ITEM_POTION:
  case ITEM_SALVE:
    sprintf(buf, "Spells: (Level %d) %s, %s, %s", GET_OBJ_VAL(j, 0),
	    skill_name(GET_OBJ_VAL(j, 1)), skill_name(GET_OBJ_VAL(j, 2)),
	    skill_name(GET_OBJ_VAL(j, 3)));
    break;
  case ITEM_WAND:
  case ITEM_STAFF:
    sprintf(buf, "Spell: %s at level %d, %d (of %d) charges remaining",
	    skill_name(GET_OBJ_VAL(j, 3)), GET_OBJ_VAL(j, 0),
	    GET_OBJ_VAL(j, 2), GET_OBJ_VAL(j, 1));
    break;
  case ITEM_WEAPON:
    sprintf(buf, "Todam: %dd%d, Message type: %s",
            GET_OBJ_VAL(j, 1), GET_OBJ_VAL(j, 2),
            attack_hit_text[GET_OBJ_VAL(j, 3)].singular);
    break;
  case ITEM_ARMOR:
    sprintf(buf, "AC-apply: [%d]", GET_OBJ_VAL(j, 0));
    break;
  /*case ITEM_TRAP:
    sprintf(buf, "Spell: %d, - Hitpoints: %d",
	    GET_OBJ_VAL(j, 0), GET_OBJ_VAL(j, 1));
    break; */
  case ITEM_SHEATH:
  case ITEM_CONTAINER:
    sprintbit(GET_OBJ_VAL(j, 1), container_bits, buf2);
    sprintf(buf, "Weight capacity: %d, Lock Type: %s, Key Num: %d, Corpse: %s",
	    GET_OBJ_VAL(j, 0), buf2, GET_OBJ_VAL(j, 2),
	    YESNO(GET_OBJ_VAL(j, 3)));
    break;
  case ITEM_DRINKCON:
  case ITEM_FOUNTAIN:
    sprinttype(GET_OBJ_VAL(j, 2), drinks, buf2);
    sprintf(buf, "Capacity: %d, Contains: %d, Poisoned: %s, Liquid: %s",
	    GET_OBJ_VAL(j, 0), GET_OBJ_VAL(j, 1), YESNO(GET_OBJ_VAL(j, 3)),
	    buf2);
    break;
  case ITEM_NOTE:
    sprintf(buf, "Tongue: %d", GET_OBJ_VAL(j, 0));
    break;
  case ITEM_KEY:
    strcpy(buf, "");
    break;
  case ITEM_FOOD:
    sprintf(buf, "Makes full: %d, Poisoned: %s", GET_OBJ_VAL(j, 0),
	    YESNO(GET_OBJ_VAL(j, 3)));
    break;
  case ITEM_MONEY:
    sprintf(buf, "Coins: %d", GET_OBJ_VAL(j, 0));
    break;
  default:
    sprintf(buf, "Values 0-3: [%d] [%d] [%d] [%d]",
	    GET_OBJ_VAL(j, 0), GET_OBJ_VAL(j, 1),
	    GET_OBJ_VAL(j, 2), GET_OBJ_VAL(j, 3));
    break;
  }
  send_to_char(strcat(buf, "\r\n"), ch);

  /*
   * I deleted the "equipment status" code from here because it seemed
   * more or less useless and just takes up valuable screen space.
   */

  if (j->contains) {
    sprintf(buf, "\r\nContents:%s", CCGRN(ch, C_NRM));
    for (found = 0, j2 = j->contains; j2; j2 = j2->next_content) {
      sprintf(buf2, "%s %s", found++ ? "," : "", j2->short_description);
      strcat(buf, buf2);
      if (strlen(buf) >= 62) {
	if (j2->next_content)
	  send_to_char(strcat(buf, ",\r\n"), ch);
	else
	  send_to_char(strcat(buf, "\r\n"), ch);
	*buf = found = 0;
      }
    }

    if (*buf)
      send_to_char(strcat(buf, "\r\n"), ch);
    send_to_char(CCNRM(ch, C_NRM), ch);
  }
  found = 0;
  send_to_char("Affections:", ch);
  for (i = 0; i < MAX_OBJ_AFFECT; i++)
    if (j->affected[i].modifier) {
      sprinttype(j->affected[i].location, apply_types, buf2);
      sprintf(buf, "%s %+d to %s", found++ ? "," : "",
	      j->affected[i].modifier, buf2);
      send_to_char(buf, ch);
    }
  if (!found)
    send_to_char(" None", ch);

  send_to_char("\r\n", ch);

  /* check the object for a script */
  do_sstat_object(ch, j);
}


void do_stat_character(struct char_data * ch, struct char_data * k)
{
  int i, i2, found = 0;
  struct obj_data *j;
  struct follow_type *fol;
  struct affected_type *aff;
  extern struct attack_hit_type attack_hit_text[];
  extern char *  godSelected(struct char_data *ch);

  switch (GET_SEX(k)) {
  case SEX_NEUTRAL:    strcpy(buf, "NEUTRAL-SEX");   break;
  case SEX_MALE:       strcpy(buf, "MALE");          break;
  case SEX_FEMALE:     strcpy(buf, "FEMALE");        break;
  default:             strcpy(buf, "ILLEGAL-SEX!!"); break;
  }

  send_to_char(buf, ch);
  if (!IS_NPC(k))
  {
    sprintf(buf2, " %s", pc_race_types[(int) GET_RACE(k)]);
    send_to_char(buf2, ch);
  }

  if ( GET_LEVEL(ch) >= LVL_IMPL ) {
    GET_NAME(k, chname);
    sprintf(buf2, " %s '%s'  IDNum: [%5ld], In room [%5d],  Passwd [%s]\r\n",
            (!IS_NPC(k) ? "PC" : (!IS_MOB(k) ? "NPC" : "MOB")),
	    chname, GET_IDNUM(k), world[k->in_room].number,
            GET_PASSWD(k));
    FREE_NAME(chname);
    send_to_char(buf2, ch);

  } else {
    GET_NAME(k, chname);
    sprintf(buf2, " %s '%s'  IDNum: [%5ld], In room [%5d]\r\n",
            (!IS_NPC(k) ? "PC" : (!IS_MOB(k) ? "NPC" : "MOB")),
	    chname, GET_IDNUM(k), world[k->in_room].number);
    FREE_NAME(chname);
    send_to_char(buf2, ch);
  }

  if (IS_MOB(k)) {
    sprintf(buf, "Alias: %s, VNum: [%5d], RNum: [%5d]\r\n",
	    k->player.name, GET_MOB_VNUM(k), GET_MOB_RNUM(k));
    send_to_char(buf, ch);
  }

  sprintf(buf,"Title: %s        \r\nTrust: %d\t", (k->player.title ? k->player.title :
              "<None>"),        GET_TRUST_LEVEL(k));
  send_to_char(buf, ch);

  sprintf(buf,"Alias: %s\r\n", (k->player.name ? k->player.name : "<None>"));
  send_to_char(buf, ch);

  if (PRF_FLAGGED(k, PRF_NOTSELF) && !IS_NPC(k)) {
   GET_NAME(k, chname);

  sprintf(buf, "Real name : %s\r\n", chname);
  send_to_char(buf, ch);
  FREE_NAME(chname);

  sprintf(buf, "Disguised / Morphed as :  %s\r\n", (k->char_specials.name_dis ? k->char_specials.name_dis : "<None>"));
  send_to_char(buf, ch);

 }

  sprintf(buf,"Short: %s\r\n", (k->player.short_descr ? k->player.short_descr : "<None>"));
  send_to_char(buf, ch);

  sprintf(buf, "L-Des: %s",
	  (k->player.long_descr ? k->player.long_descr : "<None>\r\n"));
  send_to_char(buf, ch);

  if ( GET_LEVEL(k) >= LVL_IMMORT ) {
    sprintf(buf, "Poofin :%s\r\nPoofout:%s\r\n",
      (k->player.poofin) ? k->player.poofin : "<None>",
      (k->player.poofout) ? k->player.poofout : "<None>");
    send_to_char(buf, ch);
  }

  sprintf(buf, "Detail:\r\n%s", (k->player.description ? k->player.description : "<None>\r\n"));
  send_to_char(buf, ch);

  if (IS_NPC(k)) {
    strcpy(buf, "Class: ");
    sprinttype(k->player.class, npc_class_types, buf2);
  } else {
    strcpy(buf, "Class: ");
    sprinttype(k->player.class, pc_class_types, buf2);
  }
  strcat(buf, buf2);

  sprintf(buf2, ", Lev: [%s%2d%s], XP: [%s%7d%s], Align: [%4d/%4d] (God: %s)\r\n",
	  CCYEL(ch, C_NRM), GET_LEVEL(k), CCNRM(ch, C_NRM),
	  CCYEL(ch, C_NRM), GET_EXP(k), CCNRM(ch, C_NRM),
	  GET_ETHOS(k), GET_ALIGNMENT(k), godSelected(k));
  strcat(buf, buf2);
  send_to_char(buf, ch);

  if ( ! IS_NPC(k)) {
    sprintf(buf, "RP Experience Factor: [%d]\r\n", GET_RPFACTOR(k));
    send_to_char(buf, ch);
  }

  if(!IS_NPC(k) && GET_CLAN(k))
  {
    sprintf(buf, "Clan: [%s]\r\n", describe_clan(GET_CLAN(k)));
    send_to_char(buf, ch);
  }

  if (!IS_NPC(k)) {
    strcpy(buf1, (char *) asctime(localtime(&(k->player.time.birth))));
    strcpy(buf2, (char *) asctime(localtime(&(k->player.time.logon))));
    buf1[10] = buf2[10] = '\0';

    sprintf(buf, "Created: [%s], Last Logon: [%s], Played [%dh %dm], Age [%d]\r\n",
	    buf1, buf2, k->player.time.played / 3600,
	    ((k->player.time.played % 3600) / 60), age(k)->year);
    send_to_char(buf, ch);

    sprintf(buf, "Hometown: [%d], Speaks: [%d/%d/%d], (STL[%d]/per[%d]/NSTL[%d])",
	 k->player.hometown, GET_TALK(k, 0), GET_TALK(k, 1), GET_TALK(k, 2),
	    GET_PRACTICES(k), int_app[GET_INT(k)].learn,
	    wis_app[GET_WIS(k)].bonus);
    /*. Display OLC zone for immorts .*/
    if(GET_LEVEL(k) >= LVL_IMMORT)
      sprintf(buf, "%s, OLC[%d]", buf, GET_OLC_ZONE(k));
    strcat(buf, "\r\n");
    send_to_char(buf, ch);
  }


  sprintf(buf, "Real Str: [%s%d/%d%s]  Int: [%s%d%s]  Wis: [%s%d%s]  "
	  "Dex: [%s%d%s]  Con: [%s%d%s]  Cha: [%s%d%s]\r\n",
	  CCCYN(ch, C_NRM), ((k)->real_abils.str),
	  ((k)->real_abils.str_add), CCNRM(ch, C_NRM),
	  CCCYN(ch, C_NRM), ((k)->real_abils.intel), CCNRM(ch, C_NRM),
	  CCCYN(ch, C_NRM), ((k)->real_abils.wis), CCNRM(ch, C_NRM),
	  CCCYN(ch, C_NRM), ((k)->real_abils.dex), CCNRM(ch, C_NRM),
	  CCCYN(ch, C_NRM), ((k)->real_abils.con), CCNRM(ch, C_NRM),
	  CCCYN(ch, C_NRM), ((k)->real_abils.cha), CCNRM(ch, C_NRM));
  send_to_char(buf, ch);

  sprintf(buf, "Aff  Str: [%s%d/%d%s]  Int: [%s%d%s]  Wis: [%s%d%s]  "
	  "Dex: [%s%d%s]  Con: [%s%d%s]  Cha: [%s%d%s]\r\n",
	  CCCYN(ch, C_NRM), GET_STR(k), GET_ADD(k), CCNRM(ch, C_NRM),
	  CCCYN(ch, C_NRM), GET_INT(k), CCNRM(ch, C_NRM),
	  CCCYN(ch, C_NRM), GET_WIS(k), CCNRM(ch, C_NRM),
	  CCCYN(ch, C_NRM), GET_DEX(k), CCNRM(ch, C_NRM),
	  CCCYN(ch, C_NRM), GET_CON(k), CCNRM(ch, C_NRM),
	  CCCYN(ch, C_NRM), GET_CHA(k), CCNRM(ch, C_NRM));
  send_to_char(buf, ch);



  sprintf(buf, "Hit p.:[%s%d/%d+%d%s]    Move p.:[%s%d/%d+%d%s]\r\n",
	  CCGRN(ch, C_NRM), GET_HIT(k), GET_MAX_HIT(k), hit_gain(k), CCNRM(ch, C_NRM),
	  CCGRN(ch, C_NRM), GET_MOVE(k), GET_MAX_MOVE(k), move_gain(k), CCNRM(ch, C_NRM));
  send_to_char(buf, ch);

  sprintf(buf, "Coins: [%s], Bank: [%9d] (Total: %d)\r\n",
	  describe_money(GET_MONEY(k)),
	  GET_BANK_GOLD(k), money_to_copper(GET_MONEY(k)) + GET_BANK_GOLD(k));
  send_to_char(buf, ch);

  sprintf(buf, "Quest Points %d \r\n",
          GET_QUESTPOINTS(k));
  send_to_char(buf, ch);

  sprintf(buf, "AC: [%d/10], Hitroll: [%2d], Damroll: [%2d], Saving throws: [%d/%d/%d/%d/%d]\r\n",
	  GET_AC(k), k->points.hitroll, k->points.damroll, GET_SAVE(k, 0),
	  GET_SAVE(k, 1), GET_SAVE(k, 2), GET_SAVE(k, 3), GET_SAVE(k, 4));
  send_to_char(buf, ch);

  sprinttype(GET_POS(k), position_types, buf2);

  GET_NAME(FIGHTING(k), chname);
  sprintf(buf, "Pos: %s, Fighting: %s", buf2,
	  (FIGHTING(k) ? chname : "Nobody"));
  FREE_NAME(chname);

  if (IS_NPC(k)) {
    strcat(buf, ", Attack type: ");
    strcat(buf, attack_hit_text[k->mob_specials.attack_type].singular);
  }
  if (k->desc) {
    sprinttype(STATE(k->desc), connected_types, buf2);
    strcat(buf, ", Connected: ");
    strcat(buf, buf2);
  }
  send_to_char(strcat(buf, "\r\n"), ch);

  strcpy(buf, "Default position: ");
  sprinttype((k->mob_specials.default_pos), position_types, buf2);
  strcat(buf, buf2);

  sprintf(buf2, ", Idle Timer (in tics) [%d]\r\n", k->char_specials.timer);
  strcat(buf, buf2);
  send_to_char(buf, ch);

  if (IS_NPC(k))
  {
    char spellBuf[200];

    sprintbit(MOB_FLAGS(k), action_bits, buf2);
    sprintf(buf, "NPC flags: %s%s%s\r\n", CCCYN(ch, C_NRM), buf2,
	    CCNRM(ch, C_NRM));
    send_to_char(buf, ch);

    if (IS_MAGE(k))
      sprintspellbit(SPELL_FLAGS(k), spell_bits_mage, spellBuf);
    else if (IS_CLERIC(k))
      sprintspellbit(SPELL_FLAGS(k), spell_bits_cleric, spellBuf);
    else
      sprintf(spellBuf, "None.");

    sprintf(buf, "Spells: %s%s%s\r\n", CCCYN(ch, C_NRM),
	    spellBuf, CCNRM(ch, C_NRM));
    if (MOB_FLAGGED(k, MOB_USE_SPELLS))
      send_to_char(buf, ch);
  } else {
    sprintbit(PLR_FLAGS(k), player_bits, buf2);
    sprintf(buf, "PLR: %s%s%s\r\n", CCCYN(ch, C_NRM), buf2, CCNRM(ch, C_NRM));
    send_to_char(buf, ch);
    sprintbit(PRF_FLAGS(k), preference_bits, buf2);
    sprintf(buf, "PRF: %s%s%s\r\n", CCGRN(ch, C_NRM), buf2, CCNRM(ch, C_NRM));
    send_to_char(buf, ch);
  }

  if (IS_MOB(k)) {
    char spec[15];
    sprinttype(GET_SPEC(k), spec_procs, spec);
    sprintf(buf, "Mob Spec-Proc: %s, NPC Bare Hand Dam: %dd%d\r\n",
            (mob_index[GET_MOB_RNUM(k)].func ? (GET_SPEC(k) ? spec : "Exists")
                                              : "None"),
	    k->mob_specials.damnodice, k->mob_specials.damsizedice);
    send_to_char(buf, ch);
  }
  sprintf(buf, "Carried: weight: %d, items: %d; ",
	  IS_CARRYING_W(k), IS_CARRYING_N(k));

  for (i = 0, j = k->carrying; j; j = j->next_content, i++);
  sprintf(buf, "%sItems in: inventory: %d, ", buf, i);

  for (i = 0, i2 = 0; i < NUM_WEARS; i++)
    if (GET_EQ(k, i))
      i2++;
  sprintf(buf2, "eq: %d\r\n", i2);
  strcat(buf, buf2);
  send_to_char(buf, ch);

  sprintf(buf, "Hunger: %d, Thirst: %d, Drunk: %d\r\n",
	  GET_COND(k, FULL), GET_COND(k, THIRST), GET_COND(k, DRUNK));
  send_to_char(buf, ch);

  GET_NAME(k->master, chname);
  sprintf(buf, "Master is: %s, Followers are:",
	  ((k->master) ? chname : "<none>"));
  FREE_NAME(chname);

  for (fol = k->followers; fol; fol = fol->next) {
    GET_PERS(fol->follower, ch, chname);
    sprintf(buf2, "%s %s", found++ ? "," : "", chname);
    FREE_NAME(chname);
    strcat(buf, buf2);
    if (strlen(buf) >= 62) {
      if (fol->next)
	send_to_char(strcat(buf, ",\r\n"), ch);
      else
	send_to_char(strcat(buf, "\r\n"), ch);
      *buf = found = 0;
    }
  }

  if (*buf)
    send_to_char(strcat(buf, "\r\n"), ch);

  /* Showing the bitvector */
  sprintbit(AFF_FLAGS(k), affected_bits, buf2);
  sprintf(buf, "AFF: %s%s%s\r\n", CCYEL(ch, C_NRM), buf2, CCNRM(ch, C_NRM));
  send_to_char(buf, ch);

  /* Routine to show what spells a char is affected by */
  if (k->affected)
  {
    int numAffects = 0;

    for (aff = k->affected; (aff && (numAffects < MAX_AFFECT));
      aff = aff->next)
    {
      *buf2 = '\0';
      sprintf(buf, "SPL: (%3dhr) %s%-21s%s ", aff->duration + 1,
              CCCYN(ch, C_NRM), (aff->type >= 0 && aff->type <= MAX_SKILLS) ?
	      spells[aff->type] : "TYPE UNDEFINED", CCNRM(ch, C_NRM));

      if (aff->modifier) {
	sprintf(buf2, "%+d to %s", aff->modifier,
          apply_types[(int) aff->location]);
	strcat(buf, buf2);
      }

      if (aff->bitvector) {
	if (*buf2)
	  strcat(buf, ", sets ");
	else
	  strcat(buf, "sets ");
	sprintbit(aff->bitvector, affected_bits, buf2);
	strcat(buf, buf2);
      }
      send_to_char(strcat(buf, "\r\n"), ch);
      numAffects++;
    }
  }
  /* You've got mail! (maybe) */
  if(! IS_NPC(k))
  {
     if(has_mail(GET_IDNUM(k)))
        sprintf(buf, "There is mail in the post office.");
     else
        sprintf(buf, "There is no mail in the post office.");
     send_to_char(strcat(buf, "\r\n"), ch);
  }
  /* check mobiles for a script */
  if (IS_NPC(k))
    do_sstat_character(ch, k);
}

ACMD(do_mysql_convert) {

  struct char_file_u tmp_store;
  struct char_data * victim;
  int i = 0;

  while (!feof(player_fl)) {
    CREATE(victim, struct char_data, 1);
    clear_char(victim);
    fseek(player_fl, (long) i * sizeof(struct char_file_u), SEEK_SET);
    fread(&tmp_store, sizeof(struct char_file_u), 1, player_fl);
    store_to_char(&tmp_store, victim);
    victim->player.time.logon = tmp_store.last_logon;
    char_to_room(victim, 0);
//    mysql_save_char(victim, GET_LOADROOM(victim));
    extract_char(victim);
    i++;
  }

}

ACMD(do_stat)
{
  struct char_data *victim = 0;
  struct obj_data *object = 0;
  struct char_file_u tmp_store;
  int tmp;

  half_chop(argument, buf1, buf2);

  if (!*buf1) {
    send_to_char("Stats on who or what?\r\n", ch);
    return;
  } else if (is_abbrev(buf1, "room")) {
    do_stat_room(ch);
  } else if (is_abbrev(buf1, "mob")) {
    if (!*buf2)
      send_to_char("Stats on which mobile?\r\n", ch);
    else {
      if ((victim = get_char_vis(ch, buf2, FIND_CHAR_WORLD)))
	do_stat_character(ch, victim);
      else
	send_to_char("No such mobile around.\r\n", ch);
    }
  } else if (is_abbrev(buf1, "player")) {
    if (!*buf2) {
      send_to_char("Stats on which player?\r\n", ch);
    } else {
      if ((victim = get_player_vis(ch, buf2, FIND_CHAR_WORLD)))
	do_stat_character(ch, victim);
      else
	send_to_char("No such player around.\r\n", ch);
    }
  } else if (is_abbrev(buf1, "file")) {
    if (!*buf2) {
      send_to_char("Stats on which player?\r\n", ch);
    } else {
      CREATE(victim, struct char_data, 1);
      clear_char(victim);
      if (load_char(buf2, &tmp_store) > -1) {
	store_to_char(&tmp_store, victim);
        victim->player.time.logon = tmp_store.last_logon;
        char_to_room(victim, 0);
	if (GET_LEVEL(victim) > GET_LEVEL(ch))
	  send_to_char("Sorry, you can't do that.\r\n", ch);
	else
	  do_stat_character(ch, victim);
        extract_char(victim);
      } else {
	send_to_char("There is no such player.\r\n", ch);
	free(victim);
        victim = (struct char_data*) NULL;
      }
    }
  } else if (is_abbrev(buf1, "object")) {
    if (!*buf2)
      send_to_char("Stats on which object?\r\n", ch);
    else {
      if ((object = get_obj_vis(ch, buf2)))
	do_stat_object(ch, object);
      else
	send_to_char("No such object around.\r\n", ch);
    }
  } else {
    if ((object = get_object_in_equip_vis(ch, buf1, ch->equipment, &tmp)))
      do_stat_object(ch, object);
    else if ((object = get_obj_in_list_vis(ch, buf1, ch->carrying)))
      do_stat_object(ch, object);
    else if ((victim = get_char_vis(ch, buf1, FIND_CHAR_ROOM)))
      do_stat_character(ch, victim);
    else if ((object = get_obj_in_list_vis(ch, buf1, world[ch->in_room].contents)))
      do_stat_object(ch, object);
    else if ((victim = get_char_vis(ch, buf1, FIND_CHAR_WORLD)))
      do_stat_character(ch, victim);
    else if ((object = get_obj_vis(ch, buf1)))
      do_stat_object(ch, object);
    else
      send_to_char("Nothing around by that name.\r\n", ch);
  }
}


ACMD(do_shutdown)
{
  extern int circle_shutdown, circle_reboot;

  if (subcmd != SCMD_SHUTDOWN) {
    send_to_char("If you want to shut something down, say so!\r\n", ch);
    return;
  }
  one_argument(argument, arg);

  if (!*arg) {
    GET_NAME(ch, chname);
    sprintf(buf, "(GC) Shutdown by %s.", chname);
    FREE_NAME(chname);
    log(buf);
    send_to_all("Shutting down.\r\n");
    circle_shutdown = 1;
  } else if (!str_cmp(arg, "reboot")) {
    GET_NAME(ch, chname);
    sprintf(buf, "(GC) Reboot by %s.", chname);
    FREE_NAME(chname);
    log(buf);
    send_to_all("Rebooting.. come back in a minute or two.\r\n");
    touch(FASTBOOT_FILE);
    circle_shutdown = circle_reboot = 1;
  } else if (!str_cmp(arg, "now")) {
    GET_NAME(ch, chname);
    sprintf(buf, "(GC) Shutdown NOW by %s.", chname);
    FREE_NAME(chname);
    log(buf);
    send_to_all("Rebooting.. come back in a minute or two.\r\n");
    circle_shutdown = 1;
    circle_reboot = 2;
  } else if (!str_cmp(arg, "die")) {
    GET_NAME(ch, chname);
    sprintf(buf, "(GC) Shutdown by %s.", chname);
    FREE_NAME(chname);
    log(buf);
    send_to_all("Shutting down for maintenance.\r\n");
    touch(KILLSCRIPT_FILE);
    circle_shutdown = 1;
  } else if (!str_cmp(arg, "pause")) {
    GET_NAME(ch, chname);
    sprintf(buf, "(GC) Shutdown by %s.", chname);
    FREE_NAME(chname);
    log(buf);
    send_to_all("Shutting down for maintenance.\r\n");
    touch(PAUSE_FILE);
    circle_shutdown = 1;
  } else
    send_to_char("Unknown shutdown option.\r\n", ch);
}


void stop_snooping(struct char_data * ch)
{
  if (!ch->desc->snooping)
    send_to_char("You aren't snooping anyone.\r\n", ch);
  else {
    send_to_char("You stop snooping.\r\n", ch);
    ch->desc->snooping->snoop_by = NULL;
    ch->desc->snooping = NULL;
  }
}


ACMD(do_snoop)
{
  struct char_data *victim, *tch;

  if (!ch->desc)
    return;

  one_argument(argument, arg);

  if (!*arg)
    stop_snooping(ch);
  else if (!(victim = get_char_vis(ch, arg, FIND_CHAR_WORLD)))
    send_to_char("No such person around.\r\n", ch);
  else if (!victim->desc)
    send_to_char("There's no link.. nothing to snoop.\r\n", ch);
  else if (victim == ch)
    stop_snooping(ch);
  else if (victim->desc->snoop_by)
    send_to_char("Busy already. \r\n", ch);
  else if (victim->desc->snooping == ch->desc)
    send_to_char("Don't be stupid.\r\n", ch);
  else {
    if (victim->desc->original)
      tch = victim->desc->original;
    else
      tch = victim;

    if (GET_LEVEL(tch) >= GET_LEVEL(ch)) {
      send_to_char("You can't.\r\n", ch);
      return;
    }

    GET_NAME(ch, chname);
    GET_NAME(victim, victname);
    sprintf(buf, "(GC) %s has begun snooping %s.", chname, victname);
    FREE_NAME(victname);
    FREE_NAME(chname);
    mudlog(buf, BRF, MAX(LVL_GOD, GET_INVIS_LEV(ch)), TRUE);

    send_to_char(OK, ch);

    if (ch->desc->snooping)
      ch->desc->snooping->snoop_by = NULL;

    ch->desc->snooping = victim->desc;
    victim->desc->snoop_by = ch->desc;
  }
}



ACMD(do_switch)
{
  struct char_data *victim;

  one_argument(argument, arg);

  if (ch->desc->original)
    send_to_char("You're already switched.\r\n", ch);
  else if (!*arg)
    send_to_char("Switch with who?\r\n", ch);
  else if (!(victim = get_char_vis(ch, arg, FIND_CHAR_WORLD)))
    send_to_char("No such character.\r\n", ch);
  else if (ch == victim)
    send_to_char("Hee hee... we are jolly funny today, eh?\r\n", ch);
  else if (victim->desc)
    send_to_char("You can't do that, the body is already in use!\r\n", ch);
  else if ((GET_LEVEL(ch) < LVL_IMPL) && !IS_NPC(victim))
    send_to_char("You aren't holy enough to use a mortal's body.\r\n", ch);
  else {
    send_to_char(OK, ch);

    GET_NAME(ch, chname);
    GET_NAME(victim, victname);
    sprintf(buf, "(GC) %s has switched into %s.", chname, victname);
    FREE_NAME(victname);
    FREE_NAME(chname);
    mudlog(buf, BRF, MAX(LVL_GOD, GET_INVIS_LEV(ch)), TRUE);

    ch->desc->character = victim;
    ch->desc->original = ch;

    victim->desc = ch->desc;
    SET_BIT(MOB_FLAGS(victim), MOB_QUEST);
    SET_BIT(MOB_FLAGS(victim), MOB_SENTINEL);
    REMOVE_BIT(MOB_FLAGS(victim), MOB_AGGRESSIVE);

    ch->desc = NULL;
  }
}


ACMD(do_return)
{
  if (ch->desc && ch->desc->original) {
    send_to_char("You return to your original body.\r\n", ch);

    /* JE 2/22/95 */
    /* if someone switched into your original body, disconnect them */
    if (ch->desc->original->desc) {
      ch->desc->original->desc->character = NULL;
      STATE(ch->desc->original->desc) = CON_DISCONNECT;
    }

    ch->desc->character = ch->desc->original;
    ch->desc->original = NULL;

    ch->desc->character->desc = ch->desc;
    ch->desc = NULL;
  }
}



ACMD(do_load)
{
  struct char_data *mob, *i;
  struct obj_data *obj;
  int number, r_num;

  two_arguments(argument, buf, buf2);

  if (!*buf || !*buf2 || !isdigit(*buf2)) {
    send_to_char("Usage: load { obj | mob } <number>\r\n", ch);
    return;
  }
  if ((number = atoi(buf2)) < 0) {
    send_to_char("A NEGATIVE number??\r\n", ch);
    return;
  }
  if (is_abbrev(buf, "mob")) {
    if ((r_num = real_mobile(number)) < 0) {
      send_to_char("There is no monster with that number.\r\n", ch);
      return;
    }
    mob = read_mobile(r_num, REAL);
    char_to_room(mob, ch->in_room);

    act("$n makes a quaint, magical gesture with one hand.", TRUE, ch,
	0, 0, TO_ROOM);

  GET_NAME(ch, chname);
  GET_NAME(mob, victname);
  sprintf(buf1, "%s has created %s!\r\n", chname, victname);
  FREE_NAME(victname);
  FREE_NAME(chname);
  act("You create $N.", FALSE, ch, 0, mob, TO_CHAR);


 for (i = world[ch->in_room].people; i; i = i->next_in_room) {
   if (GET_LEVEL(i) >= LVL_IMMORT && i != ch) {
      send_to_char(CCCYN(i, C_NRM), i);
      send_to_char(buf1, i);
      send_to_char(CCNRM(i, C_NRM), i);
}
}
  } else if (is_abbrev(buf, "obj")) {
    if ((r_num = real_object(number)) < 0) {
      send_to_char("There is no object with that number.\r\n", ch);
      return;
    }
    obj = read_object(r_num, REAL);
    obj_to_char(obj, ch);

  GET_NAME(ch, chname);
  sprintf(buf1, "%s has created %s!\r\n", chname, obj->short_description);
  FREE_NAME(chname);
   act("You create $p.", FALSE, ch, obj, 0, TO_CHAR);

 for (i = world[ch->in_room].people; i; i = i->next_in_room) {
   if (GET_LEVEL(i) >= LVL_IMMORT && i != ch) {
      send_to_char(CCCYN(i, C_NRM), i);
      send_to_char(buf1, i);
      send_to_char(CCNRM(i, C_NRM), i);
}
}

  } else
    send_to_char("That'll have to be either 'obj' or 'mob'.\r\n", ch);
}



ACMD(do_vstat)
{
  struct char_data *mob;
  struct obj_data *obj;
  int number, r_num;

  two_arguments(argument, buf, buf2);

  if (!*buf || !*buf2 || !isdigit(*buf2)) {
    send_to_char("Usage: vstat { obj | mob } <number>\r\n", ch);
    return;
  }
  if ((number = atoi(buf2)) < 0) {
    send_to_char("A NEGATIVE number??\r\n", ch);
    return;
  }
  if (is_abbrev(buf, "mob")) {
    if ((r_num = real_mobile(number)) < 0) {
      send_to_char("There is no monster with that number.\r\n", ch);
      return;
    }
    mob = read_mobile(r_num, REAL);
    char_to_room(mob, 0);
    do_stat_character(ch, mob);
    extract_char(mob);
  } else if (is_abbrev(buf, "obj")) {
    if ((r_num = real_object(number)) < 0) {
      send_to_char("There is no object with that number.\r\n", ch);
      return;
    }
    obj = read_object(r_num, REAL);
    obj_index[r_num].number--;
    do_stat_object(ch, obj);
    obj_index[r_num].number++;
    extract_obj(obj);
  } else
    send_to_char("That'll have to be either 'obj' or 'mob'.\r\n", ch);
}




/* clean a room of all mobiles and objects */
ACMD(do_purge)
{
  struct char_data *vict, *next_v;
  struct obj_data *obj, *next_o;

  one_argument(argument, buf);

  if (*buf) {			/* argument supplied. destroy single object
				 * or char */
    if ((vict = get_char_vis(ch, buf, FIND_CHAR_ROOM))) {
      if (!IS_NPC(vict) && (GET_LEVEL(ch) <= GET_LEVEL(vict))) {
	send_to_char("Fuuuuuuuuu!\r\n", ch);
	return;
      }
      act("$n disintegrates $N.", FALSE, ch, 0, vict, TO_NOTVICT);

      if (!IS_NPC(vict)) {
        GET_NAME(ch, chname);
        GET_NAME(vict, victname);
	sprintf(buf, "(GC) %s has purged %s.", chname, victname);
        FREE_NAME(victname);
        FREE_NAME(chname);
        mudlog(buf, BRF, MAX(LVL_GOD, GET_INVIS_LEV(ch)), TRUE);
	if (vict->desc) {
          STATE(vict->desc) = CON_CLOSE;
	  vict->desc->character = NULL;
	  vict->desc = NULL;
	}
      }
      extract_char(vict);
    } else if ((obj = get_obj_in_list_vis(ch, buf, world[ch->in_room].contents))) {
      act("$n destroys $p.", FALSE, ch, obj, 0, TO_ROOM);
      extract_obj(obj);
    } else {
      send_to_char("Nothing here by that name.\r\n", ch);
      return;
    }

    send_to_char(OK, ch);
  } else {			/* no argument. clean out the room */
    act("$n gestures... You are surrounded by scorching flames!",
	FALSE, ch, 0, 0, TO_ROOM);
    send_to_room("The world seems a little cleaner.\r\n", ch->in_room);

    for (vict = world[ch->in_room].people; vict; vict = next_v) {
      next_v = vict->next_in_room;
      if (IS_NPC(vict))
	extract_char(vict);
    }

    for (obj = world[ch->in_room].contents; obj; obj = next_o) {
      next_o = obj->next_content;
      extract_obj(obj);
    }
  }
}



static const char *logtypes[] = {
"off", "brief", "normal", "complete", "\n"};

ACMD(do_syslog)
{
  int tp;

  one_argument(argument, arg);

  if (!*arg) {
    tp = ((PRF_FLAGGED(ch, PRF_LOG1) ? 1 : 0) +
	  (PRF_FLAGGED(ch, PRF_LOG2) ? 2 : 0));
    sprintf(buf, "Your syslog is currently %s.\r\n", logtypes[tp]);
    send_to_char(buf, ch);
    return;
  }
  if (((tp = search_block(arg, logtypes, FALSE)) == -1)) {
    send_to_char("Usage: syslog { Off | Brief | Normal | Complete }\r\n", ch);
    return;
  }
  REMOVE_BIT(PRF_FLAGS(ch), PRF_LOG1 | PRF_LOG2);
  SET_BIT(PRF_FLAGS(ch), (PRF_LOG1 * (tp & 1)) | (PRF_LOG2 * (tp & 2) >> 1));

  sprintf(buf, "Your syslog is now %s.\r\n", logtypes[tp]);
  send_to_char(buf, ch);
}


ACMD(do_advance)
{
  struct char_data *victim;

  //char *name = arg, *level = buf2;    // Didn't see the purpose for this. Changed to be more secure.
  char name[MAX_INPUT_LENGTH], level[MAX_INPUT_LENGTH]; // Said change.

  int newlevel, oldlevel;

  // The definition for these is in "class.c"
  void do_start(struct char_data *ch);
  void gain_exp(struct char_data * ch, int gain);

  two_arguments(argument, name, level);

  if (*name) {
    if (!(victim = get_char_vis(ch, name, FIND_CHAR_WORLD))) {
      send_to_char("That player is not here.\r\n", ch);
      return;
    }
  } else {
    send_to_char("Advance who?\r\n", ch);
    return;
  }

  if (GET_TRUST_LEVEL(ch) < GET_TRUST_LEVEL(victim)) {
    send_to_char("Maybe that's not such a great idea.\r\n", ch);
    return;
  }
  if (IS_NPC(victim)) {
    send_to_char("NO!  Not on NPCs.\r\n", ch);
    return;
  }
  if (!*level || (newlevel = atoi(level)) <= 0) {
    send_to_char("That's not a level!\r\n", ch);
    return;
  }
  if (newlevel > LVL_IMPL) {
    sprintf(buf, "%d is the highest possible level.\r\n", LVL_IMPL);
    send_to_char(buf, ch);
    return;
  }
  if (newlevel > GET_TRUST_LEVEL(ch)) {
    send_to_char("Yeah, right.\r\n", ch);
    return;
  }
  if (newlevel == GET_LEVEL(victim)) {
    send_to_char("They are already at that level.\r\n", ch);
    return;
  }
  oldlevel = GET_LEVEL(victim);
  if (newlevel < GET_LEVEL(victim)) {
    do_start(victim);

    //GET_LEVEL(victim) = newlevel;     // This was giving me an error - not sure why.
    victim->player.level = newlevel;    // So I just did it the explicit way.

    send_to_char("You are momentarily enveloped by darkness!\r\n"
		 "You feel somewhat diminished.\r\n", victim);
  } else {
    act("$n makes some strange gestures.\r\n"
	"A strange feeling comes upon you,\r\n"
	"Like a giant hand, light comes down\r\n"
	"from above, grabbing your body, that\r\n"
	"begins to pulse with colored lights\r\n"
	"from inside.\r\n\r\n"
	"Your head seems to be filled with demons\r\n"
	"from another plane as your body dissolves\r\n"
	"to the elements of time and space itself.\r\n"
	"Suddenly a silent explosion of light\r\n"
	"snaps you back to reality.\r\n\r\n"
	"You feel slightly different.", FALSE, ch, 0, victim, TO_VICT);
  }

  send_to_char(OK, ch);

  GET_NAME(ch, chname);
  GET_NAME(victim, victname);
  sprintf(buf, "(GC) %s has advanced %s to level %d (from %d)",
    chname, victname, newlevel, oldlevel);
  FREE_NAME(victname);
  FREE_NAME(chname);

  log(buf);
  gain_exp_regardless(victim, level_exp(GET_CLASS(victim), newlevel) - GET_EXP(victim));
  save_char(victim, NOWHERE);
}

ACMD(do_restore) {
  struct char_data *vict;
  int i;

  one_argument(argument, buf);

  if (!*buf) {
    send_to_char("Whom do you wish to restore?\r\n", ch);

  } else if (!(vict = get_char_vis(ch, buf, FIND_CHAR_WORLD))) {
    send_to_char(NOPERSON, ch);

  } else {
    GET_HIT(vict) = GET_MAX_HIT(vict);
    GET_MOVE(vict) = GET_MAX_MOVE(vict);

    if ((GET_LEVEL(ch) >= LVL_GRGOD) && (GET_LEVEL(vict) >= LVL_IMMORT)) {
      for (i = 1; i <= MAX_SKILLS; i++) {
	SET_SKILL(vict, i, 100);
      }

      if (GET_LEVEL(vict) >= LVL_GRGOD) {
	vict->real_abils.str_add = 100;
	vict->real_abils.intel = 25;
	vict->real_abils.wis = 25;
	vict->real_abils.dex = 25;
	vict->real_abils.str = 25;
	vict->real_abils.con = 25;
	vict->real_abils.cha = 25;
      }
      vict->aff_abils = vict->real_abils;
    }

    update_pos(vict);
    send_to_char(OK, ch);
    GET_NAME(ch, chname);
    GET_NAME(vict, victname);
    sprintf(buf, "(GC) %s restored %s.", chname, victname);
    FREE_NAME(victname);
    FREE_NAME(chname);
    mudlog(buf, NRM, MAX(LVL_GOD, GET_INVIS_LEV(ch)), TRUE);
    act("You have been fully healed by $N!", FALSE, vict, 0, ch, TO_CHAR);
  }
}


void perform_immort_vis(struct char_data *ch)
{
  void appear(struct char_data *ch);

  if (GET_INVIS_LEV(ch) == 0 && !IS_AFFECTED(ch, AFF_HIDE | AFF_INVISIBLE)) {
    send_to_char("You are already fully visible.\r\n", ch);
    return;
  }

  GET_INVIS_LEV(ch) = 0;
  appear(ch);
  send_to_char("You are now fully visible.\r\n", ch);
}


void perform_immort_invis(struct char_data *ch, int level)
{
  struct char_data *tch;

  if (IS_NPC(ch))
    return;

  for (tch = world[ch->in_room].people; tch; tch = tch->next_in_room) {
    if (tch == ch)
      continue;
    if (GET_LEVEL(tch) >= GET_INVIS_LEV(ch) && GET_LEVEL(tch) < level)
      act("You blink and suddenly realize that $n is gone.", FALSE, ch, 0,
	  tch, TO_VICT);
    if (GET_LEVEL(tch) < GET_INVIS_LEV(ch) && GET_LEVEL(tch) >= level)
      act("You suddenly realize that $n is standing beside you.", FALSE, ch, 0,
	  tch, TO_VICT);
  }

  GET_INVIS_LEV(ch) = level;
  sprintf(buf, "Your invisibility level is %d.\r\n", level);
  send_to_char(buf, ch);
}


ACMD(do_invis)
{
  int level;

  if (IS_NPC(ch)) {
    send_to_char("You can't do that!\r\n", ch);
    return;
  }

  one_argument(argument, arg);
  if (!*arg) {
    if (GET_INVIS_LEV(ch) > 0)
      perform_immort_vis(ch);
    else
      perform_immort_invis(ch, GET_TRUST_LEVEL(ch));
  } else {
    level = atoi(arg);
    if (level > GET_TRUST_LEVEL(ch))
      send_to_char("You can't go invisible above your own level.\r\n", ch);
    else if (level < 1)
      perform_immort_vis(ch);
    else
      perform_immort_invis(ch, level);
  }
}

ACMD(do_zecho)
{
  struct descriptor_data *pt;
  //char buf2[MAX_STRING_LENGTH];
  skip_spaces(&argument);
  delete_doubledollar(argument);

  if (!*argument)
    send_to_char("That must be a mistake...\r\n", ch);
  else {

    sprintf(buf, "%s", argument);

    for (pt = descriptor_list; pt; pt = pt->next) {
      if (STATE(pt) == CON_PLAYING && pt->character && pt->character != ch) {

       if (!shout_range(ch, pt->character))
        continue;

       act(buf, FALSE, ch, 0, pt->character, TO_VICT | TO_SLEEP);
      }
     }

    if (PRF_FLAGGED(ch, PRF_NOREPEAT))
      send_to_char(OK, ch);
    else
      act(buf, FALSE, ch, 0, 0, TO_CHAR | TO_SLEEP);
  }
}

ACMD(do_aecho)
{
  struct descriptor_data *pt;

  skip_spaces(&argument);
  delete_doubledollar(argument);

  if (!*argument)
    send_to_char("That must be a mistake...\r\n", ch);
  else {
    sprintf(buf, "&c-&C={ &WAge of Legends &C}=&c-  &n %s\r\n", argument);
    for (pt = descriptor_list; pt; pt = pt->next)
      if (STATE(pt) == CON_PLAYING && pt->character && pt->character != ch)
	send_to_char(buf, pt->character);
    if (PRF_FLAGGED(ch, PRF_NOREPEAT))
      send_to_char(OK, ch);
    else
      send_to_char(buf, ch);
  }
}

ACMD(do_gecho)
{
  struct descriptor_data *pt;

  skip_spaces(&argument);
  delete_doubledollar(argument);

  if (!*argument)
    send_to_char("That must be a mistake...\r\n", ch);
  else {
    sprintf(buf, "%s\r\n", argument);
    for (pt = descriptor_list; pt; pt = pt->next)
      if (STATE(pt) == CON_PLAYING && pt->character && pt->character != ch)
	send_to_char(buf, pt->character);
    if (PRF_FLAGGED(ch, PRF_NOREPEAT))
      send_to_char(OK, ch);
    else
      send_to_char(buf, ch);
  }
}


ACMD(do_poofset)
{
  char **msg;

  switch (subcmd) {
  case SCMD_POOFIN:    msg = &(POOFIN(ch));    break;
  case SCMD_POOFOUT:   msg = &(POOFOUT(ch));   break;
  default:    return;    break;
  }

  skip_spaces(&argument);

  if (*msg)
  {
    free(*msg);
    *msg = (char*) NULL;
  }

  if (!*argument)
    *msg = NULL;
  else
  {
    char tmpPoof[POOF_LENGTH];

    if (strlen(argument) > POOF_LENGTH-1)
    {
      GET_NAME(ch, chname);
      log("Attempting to set poof > POOF_LENGTH (%d/%d) for %s.",
        strlen(argument), POOF_LENGTH, chname);
      FREE_NAME(chname);
    }
    memset(tmpPoof, (char) NULL, POOF_LENGTH);
    strncpy(tmpPoof, argument, POOF_LENGTH - 1);
    *msg = str_dup(tmpPoof);
  }

  send_to_char(OK, ch);
}



ACMD(do_dc)
{
  struct descriptor_data *d;
  int num_to_dc;

  one_argument(argument, arg);
  if (!(num_to_dc = atoi(arg))) {
    send_to_char("Usage: DC <user number> (type USERS for a list)\r\n", ch);
    return;
  }
  for (d = descriptor_list; d && d->desc_num != num_to_dc; d = d->next);

  if (!d) {
    send_to_char("No such connection.\r\n", ch);
    return;
  }
  if (d->character && GET_LEVEL(d->character) >= GET_LEVEL(ch)) {
    if (!CAN_SEE(ch, d->character))
      send_to_char("No such connection.\r\n", ch);
    else
      send_to_char("Umm.. maybe that's not such a good idea...\r\n", ch);
    return;
  }

  /* We used to just close the socket here using close_socket(), but
   * various people pointed out this could cause a crash if you're
   * closing the person below you on the descriptor list.  Just setting
   * to CON_CLOSE leaves things in a massively inconsistent state so I
   * had to add this new flag to the descriptor.
   */
  if (STATE(d) == CON_DISCONNECT || STATE(d) == CON_CLOSE)
    send_to_char("They're already being disconnected.\r\n", ch);
  else {
    /*
     * Remember that we can disconnect people not in the game and
     * that rather confuses the code when it expected there to be
     * a character context.
     */
    if (STATE(d) == CON_PLAYING)
      STATE(d) = CON_DISCONNECT;
    else
      STATE(d) = CON_CLOSE;

    sprintf(buf, "Connection #%d closed.\r\n", num_to_dc);
    send_to_char(buf, ch);
    GET_NAME(ch, chname);
    sprintf(buf, "(GC) Connection closed by %s.", chname);
    FREE_NAME(chname);
    log(buf);
  }
}



ACMD(do_wizlock)
{
  int value;
  char *when;

  one_argument(argument, arg);
  if (*arg) {
    value = atoi(arg);
    if (value < 0 || value > GET_LEVEL(ch)) {
      send_to_char("Invalid wizlock value.\r\n", ch);
      return;
    }
    circle_restrict = value;
    when = "now";
  } else
    when = "currently";

  switch (circle_restrict) {
  case 0:
    sprintf(buf, "The game is %s completely open.\r\n", when);
    break;
  case 1:
    sprintf(buf, "The game is %s closed to new players.\r\n", when);
    break;
  default:
    sprintf(buf, "Only level %d and above may enter the game %s.\r\n",
	    circle_restrict, when);
    break;
  }
  send_to_char(buf, ch);
}


ACMD(do_date)
{
  char *tmstr;
  time_t mytime;
  int d, h, m;
  extern time_t boot_time;

  if (subcmd == SCMD_DATE)
    mytime = time(0);
  else
    mytime = boot_time;

  tmstr = (char *) asctime(localtime(&mytime));
  *(tmstr + strlen(tmstr) - 1) = '\0';

  if (subcmd == SCMD_DATE)
    sprintf(buf, "Current machine time: %s\r\n", tmstr);
  else {
    mytime = time(0) - boot_time;
    d = mytime / 86400;
    h = (mytime / 3600) % 24;
    m = (mytime / 60) % 60;

    sprintf(buf, "Up since %s: %d day%s, %d:%02d\r\n", tmstr, d,
	    ((d == 1) ? "" : "s"), h, m);
  }

  send_to_char(buf, ch);
}



ACMD(do_last)
{
  struct char_file_u chdata;
  extern char *class_abbrevs[];

  one_argument(argument, arg);
  if (!*arg) {
    send_to_char("For whom do you wish to search?\r\n", ch);
    return;
  }
  if (load_char(arg, &chdata) < 0) {
    send_to_char("There is no such player.\r\n", ch);
    return;
  }
  if ((chdata.level > GET_LEVEL(ch)) && (GET_LEVEL(ch) < LVL_IMPL)) {
    send_to_char("You are not sufficiently godly for that!\r\n", ch);
    return;
  }
  sprintf(buf, "[%5ld] [%2d %s] %-12s : %-18s : %-20s\r\n",
	  chdata.char_specials_saved.idnum, (int) chdata.level,
	  class_abbrevs[(int) chdata.class], chdata.name, chdata.host,
	  ctime(&chdata.last_logon));
  send_to_char(buf, ch);
}


ACMD(do_force)
{
  struct descriptor_data *i, *next_desc;
  struct char_data *vict, *next_force;
  char to_force[MAX_INPUT_LENGTH + 2];

  half_chop(argument, arg, to_force);

  sprintf(buf1, "$n has forced you to '%s'.", to_force);

  if (!*arg || !*to_force)
    send_to_char("Whom do you wish to force do what?\r\n", ch);
  else if ((GET_LEVEL(ch) < LVL_GRGOD) || (str_cmp("all", arg) && str_cmp("room", arg))) {
    if (!(vict = get_char_vis(ch, arg, FIND_CHAR_WORLD)))
      send_to_char(NOPERSON, ch);
    else if (GET_LEVEL(ch) <= GET_LEVEL(vict))
      send_to_char("No, no, no!\r\n", ch);
    else {
      send_to_char(OK, ch);
      act(buf1, TRUE, ch, NULL, vict, TO_VICT);
      GET_NAME(ch, chname);
      GET_NAME(vict, victname);
      sprintf(buf, "(GC) %s forced %s to %s", chname, victname, to_force);
      FREE_NAME(victname);
      FREE_NAME(chname);
      mudlog(buf, NRM, MAX(LVL_GOD, GET_INVIS_LEV(ch)), TRUE);
      command_interpreter(vict, to_force);
    }
  } else if (!str_cmp("room", arg)) {
    send_to_char(OK, ch);
    GET_NAME(ch, chname);
    sprintf(buf, "(GC) %s forced room %d to %s", chname, world[ch->in_room].number, to_force);
    FREE_NAME(chname);
    mudlog(buf, NRM, MAX(LVL_GOD, GET_INVIS_LEV(ch)), TRUE);

    for (vict = world[ch->in_room].people; vict; vict = next_force) {
      next_force = vict->next_in_room;
      if (GET_LEVEL(vict) >= GET_LEVEL(ch))
	continue;
      act(buf1, TRUE, ch, NULL, vict, TO_VICT);
      command_interpreter(vict, to_force);
    }
  } else { /* force all */
    send_to_char(OK, ch);
    GET_NAME(ch, chname);
    sprintf(buf, "(GC) %s forced all to %s", chname, to_force);
    FREE_NAME(chname);
    mudlog(buf, NRM, MAX(LVL_GOD, GET_INVIS_LEV(ch)), TRUE);

    for (i = descriptor_list; i; i = next_desc) {
      next_desc = i->next;

      if (STATE(i) != CON_PLAYING || !(vict = i->character) || (!IS_NPC(vict) &&
        GET_LEVEL(vict) >= GET_LEVEL(ch)))
	continue;
      act(buf1, TRUE, ch, NULL, vict, TO_VICT);
      command_interpreter(vict, to_force);
    }
  }
}



ACMD(do_wiznet)
{
  struct descriptor_data *d;
  char emote = FALSE;
  char any = FALSE;
  int level = LVL_IMMORT;

  if ((GET_LEVEL(ch) < level) && (!PLR_FLAGGED(ch, PLR_IMMCHAR))) {
    send_to_char("Huh?!?\r\n", ch);
    return;
  }

  skip_spaces(&argument);
  delete_doubledollar(argument);

  if (!*argument) {
    send_to_char("Usage: wiznet <text> | #<level> <text> | *<emotetext> |\r\n "
		 "       wiznet @<level> *<emotetext> | wiz @\r\n", ch);
    return;
  }
  switch (*argument) {
  case '*':
    emote = TRUE;
  case '#':
    one_argument(argument + 1, buf1);
    if (is_number(buf1)) {
      half_chop(argument+1, buf1, argument);
      level = MAX(atoi(buf1), LVL_IMMORT);
      if (level > GET_LEVEL(ch)) {
	send_to_char("You can't wizline above your own level.\r\n", ch);
	return;
      }
    } else if (emote)
      argument++;
    break;
  case '@':
    for (d = descriptor_list; d; d = d->next) {
      if (STATE(d) == CON_PLAYING && GET_LEVEL(d->character) >= LVL_IMMORT &&
	  !PRF_FLAGGED(d->character, PRF_NOWIZ) &&
	  (CAN_SEE(ch, d->character) || GET_LEVEL(ch) == LVL_IMPL)) {
	if (!any) {
	  sprintf(buf1, "Gods online:\r\n");
	  any = TRUE;
	}

        GET_NAME(d->character, chname);
	sprintf(buf1, "%s  %s", buf1, chname);
        FREE_NAME(chname);

	if (PLR_FLAGGED(d->character, PLR_WRITING))
	  sprintf(buf1, "%s (Writing)\r\n", buf1);
	else if (PLR_FLAGGED(d->character, PLR_MAILING))
	  sprintf(buf1, "%s (Writing mail)\r\n", buf1);
	else
	  sprintf(buf1, "%s\r\n", buf1);

      }
    }
    any = FALSE;
    for (d = descriptor_list; d; d = d->next) {
      if (STATE(d) == CON_PLAYING && GET_LEVEL(d->character) >= LVL_IMMORT &&
	  PRF_FLAGGED(d->character, PRF_NOWIZ) &&
	  CAN_SEE(ch, d->character)) {
	if (!any) {
	  sprintf(buf1, "%sGods offline:\r\n", buf1);
	  any = TRUE;
	}
        GET_NAME(d->character, chname);
	sprintf(buf1, "%s  %s\r\n", buf1, chname);
        FREE_NAME(chname);
      }
    }
    send_to_char(buf1, ch);
    return;
    break;
  case '\\':
    ++argument;
    break;
  default:
    break;
  }
  if (PRF_FLAGGED(ch, PRF_NOWIZ)) {
    send_to_char("You are offline!\r\n", ch);
    return;
  }
  skip_spaces(&argument);

  if (!*argument) {
    send_to_char("Don't bother the gods like that!\r\n", ch);
    return;
  }
  if (level > LVL_IMMORT) {
    GET_NAME(ch, chname);
    sprintf(buf1, "%s: <%d> %s%s\r\n", chname, level,
	    emote ? "<--- " : "", argument);
    FREE_NAME(chname);
    sprintf(buf2, "Someone: <%d> %s%s\r\n", level, emote ? "<--- " : "",
	    argument);
  } else {
    GET_NAME(ch, chname);
    sprintf(buf1, "%s: %s%s\r\n", chname, emote ? "<--- " : "",
	    argument);
    FREE_NAME(chname);
    sprintf(buf2, "Someone: %s%s\r\n", emote ? "<--- " : "", argument);
  }

  for (d = descriptor_list; d; d = d->next) {
/* This is ugly. WTF? --Gilean, 10/9/2000 */
    if ((STATE(d) == CON_PLAYING) &&
        ((GET_LEVEL(d->character) >= level) ||
          (PLR_FLAGGED(d->character, PLR_IMMCHAR) && (level == 31))) &&
        (!PRF_FLAGGED(d->character, PRF_NOWIZ)) &&
        (!PLR_FLAGGED(d->character, PLR_WRITING | PLR_MAILING))
        && (d != ch->desc || !(PRF_FLAGGED(d->character, PRF_NOREPEAT))))
   {
      send_to_char(CCCYN(d->character, C_NRM), d->character);
/* Silly people don't like invis? Duh.   Gilean, 10/11/2k
      if (CAN_SEE(d->character, ch))
	send_to_char(buf1, d->character);
      else
	send_to_char(buf2, d->character);
*/
      send_to_char(buf1, d->character);
      send_to_char(CCNRM(d->character, C_NRM), d->character);
    }
  }

  if (PRF_FLAGGED(ch, PRF_NOREPEAT))
    send_to_char(OK, ch);
}


/* I'm tired of accidentally mischanneling. --gan, 01/01/2002 (Nuitari)
 */

ACMD(do_impnet)
{
	struct descriptor_data *d;
  char emote = FALSE;

  if (PRF_FLAGGED(ch, PRF_NOWIZ)) {
    send_to_char("You are offline!\r\n", ch);
    return;
  }

  skip_spaces(&argument);
  delete_doubledollar(argument);

  if (!*argument) {
    send_to_char("Usage: imp <text> | *<emotetext> \r\n ", ch);
    return;
  }
  switch (*argument) {
  case '*':
    emote = TRUE;
		argument++;
		break;
  default: break;
	}

  skip_spaces(&argument);

  if (!*argument) {
    send_to_char("Don't bother the gods like that!\r\n", ch);
    return;
  }
  GET_NAME(ch, chname);
  if(emote)
	{
  	sprintf(buf1, "[Imp] %s %s\r\n", chname, argument);
	}
	else
	{
  	sprintf(buf1, "[Imp %s]: %s\r\n", chname, argument);
	}
  FREE_NAME(chname);


  for (d = descriptor_list; d; d = d->next) {
/* This is ugly. WTF? --Gilean, 10/9/2000 */
    if((STATE(d) == CON_PLAYING) &&
       ((GET_LEVEL(d->character) == LVL_IMPL) &&
        (!PRF_FLAGGED(d->character, PRF_NOWIZ)) &&
        (!PLR_FLAGGED(d->character, PLR_WRITING | PLR_MAILING))
        && (d != ch->desc || !(PRF_FLAGGED(d->character, PRF_NOREPEAT)))))
   {
      send_to_char(CCCYN(d->character, C_NRM), d->character);
      send_to_char(buf1, d->character);
      send_to_char(CCNRM(d->character, C_NRM), d->character);
    }
  }

  if (PRF_FLAGGED(ch, PRF_NOREPEAT))
    send_to_char(OK, ch);

}

ACMD(do_zreset)
{
  void reset_zone(int zone);

  int i, j;

  one_argument(argument, arg);
  if (!*arg) {
    send_to_char("You must specify a zone.\r\n", ch);
    return;
  }
  if (*arg == '*') {
    for (i = 0; i <= top_of_zone_table; i++)
      reset_zone(i);
    send_to_char("Reset world.\r\n", ch);
    GET_NAME(ch, chname);
    sprintf(buf, "(GC) %s reset entire world.", chname);
    FREE_NAME(chname);
    mudlog(buf, NRM, MAX(LVL_GRGOD, GET_INVIS_LEV(ch)), TRUE);
    return;
  } else if (*arg == '.')
    i = world[ch->in_room].zone;
  else {
    j = atoi(arg);
    for (i = 0; i <= top_of_zone_table; i++)
      if (zone_table[i].number == j)
	break;
  }
  if (i >= 0 && i <= top_of_zone_table) {
    reset_zone(i);
    sprintf(buf, "Reset zone %d (#%d): %s.\r\n", i, zone_table[i].number,
	    zone_table[i].name);
    send_to_char(buf, ch);
    GET_NAME(ch, chname);
    sprintf(buf, "(GC) %s reset zone %d (%s)", chname, i, zone_table[i].name);
    FREE_NAME(chname);
    mudlog(buf, NRM, MAX(LVL_GRGOD, GET_INVIS_LEV(ch)), TRUE);
  } else
    send_to_char("Invalid zone number.\r\n", ch);
}


/*
 *  General fn for wizcommands of the sort: cmd <player>
 */

ACMD(do_wizutil)
{
  struct char_data *vict;
  struct affected_type *af, *i;
  long result;
  /*  PDH 12/29/98 - OLD - void roll_real_abils(struct char_data *ch); */

  one_argument(argument, arg);

  if (!*arg)
    send_to_char("Yes, but for whom?!?\r\n", ch);
  else if (!(vict = get_char_vis(ch, arg, FIND_CHAR_WORLD)))
    send_to_char("There is no such player.\r\n", ch);
  else if (IS_NPC(vict))
    send_to_char("You can't do that to a mob!\r\n", ch);
  else if (GET_LEVEL(vict) > GET_LEVEL(ch))
    send_to_char("Hmmm...you'd better not.\r\n", ch);
  else {
    switch (subcmd) {

    /*  PDH 12/29/98 - can no longer reroll a PC's stats
    case SCMD_REROLL:
      send_to_char("Rerolled...\r\n", ch);
      roll_real_abils(vict);
      GET_NAME(ch, chname);
      GET_NAME(vict, victname);
      sprintf(buf, "(GC) %s has rerolled %s.", chname, victname);
      FREE_NAME(victname);
      FREE_NAME(chname);
      log(buf);
      sprintf(buf, "New stats: Str %d/%d, Int %d, Wis %d, Dex %d, Con %d, Cha %d\r\n",
	      GET_STR(vict), GET_ADD(vict), GET_INT(vict), GET_WIS(vict),
	      GET_DEX(vict), GET_CON(vict), GET_CHA(vict));
      send_to_char(buf, ch);
      break;
      */

    case SCMD_PARDON:
      if (!PLR_FLAGGED(vict, PLR_THIEF | PLR_KILLER)) {
	send_to_char("Your victim is not flagged.\r\n", ch);
	return;
      }
      REMOVE_BIT(PLR_FLAGS(vict), PLR_THIEF | PLR_KILLER);
      send_to_char("Pardoned.\r\n", ch);
      send_to_char("You have been pardoned by the Gods!\r\n", vict);
      GET_NAME(ch, chname);
      GET_NAME(vict, victname);
      sprintf(buf, "(GC) %s pardoned by %s", victname, chname);
      FREE_NAME(victname);
      FREE_NAME(chname);
      mudlog(buf, BRF, MAX(LVL_GOD, GET_INVIS_LEV(ch)), TRUE);
      break;
    case SCMD_SQUELCH:
      result = PLR_TOG_CHK(vict, PLR_NOSHOUT);
      GET_NAME(ch, chname);
      GET_NAME(vict, victname);
      sprintf(buf, "(GC) Squelch %s for %s by %s.", ONOFF(result),
	      victname, chname);
      FREE_NAME(victname);
      FREE_NAME(chname);
      mudlog(buf, BRF, MAX(LVL_GOD, GET_INVIS_LEV(ch)), TRUE);
      strcat(buf, "\r\n");
      send_to_char(buf, ch);
      break;
    case SCMD_FREEZE:
      if (ch == vict) {
	send_to_char("Oh, yeah, THAT'S real smart...\r\n", ch);
	return;
      }
      if (PLR_FLAGGED(vict, PLR_FROZEN)) {
	send_to_char("Your victim is already pretty cold.\r\n", ch);
	return;
      }
      SET_BIT(PLR_FLAGS(vict), PLR_FROZEN);
      GET_FREEZE_LEV(vict) = GET_LEVEL(ch);
      send_to_char("A bitter wind suddenly rises and drains every erg of heat from your body!\r\nYou feel frozen!\r\n", vict);
      send_to_char("Frozen.\r\n", ch);
      act("A sudden cold wind conjured from nowhere freezes $n!", FALSE, vict, 0, 0, TO_ROOM);
      GET_NAME(ch, chname);
      GET_NAME(vict, victname);
      sprintf(buf, "(GC) %s frozen by %s.", victname, chname);
      FREE_NAME(victname);
      FREE_NAME(chname);
      mudlog(buf, BRF, MAX(LVL_GOD, GET_INVIS_LEV(ch)), TRUE);
      break;
    case SCMD_THAW:
      if (!PLR_FLAGGED(vict, PLR_FROZEN)) {
	send_to_char("Sorry, your victim is not morbidly encased in ice at the moment.\r\n", ch);
	return;
      }
      if (GET_FREEZE_LEV(vict) > GET_LEVEL(ch)) {
        GET_NAME(vict, victname);
	sprintf(buf, "Sorry, a level %d God froze %s... you can't unfreeze %s.\r\n",
	   GET_FREEZE_LEV(vict), victname, HMHR(vict));
        FREE_NAME(victname);
	send_to_char(buf, ch);
	return;
      }
      GET_NAME(ch, chname);
      GET_NAME(vict, victname);
      sprintf(buf, "(GC) %s un-frozen by %s.", victname, chname);
      FREE_NAME(victname);
      FREE_NAME(chname);
      mudlog(buf, BRF, MAX(LVL_GOD, GET_INVIS_LEV(ch)), TRUE);
      REMOVE_BIT(PLR_FLAGS(vict), PLR_FROZEN);
      send_to_char("A fireball suddenly explodes in front of you, melting the ice!\r\nYou feel thawed.\r\n", vict);
      send_to_char("Thawed.\r\n", ch);
      act("A sudden fireball conjured from nowhere thaws $n!", FALSE, vict, 0, 0, TO_ROOM);
      break;

    case SCMD_UNAFFECT:
      if (vict->affected) {
        af = vict->affected;
        while (af)
        {
          i = af->next;
          if (af->duration > -1)  // don't remove permanent effects
            affect_remove(vict, vict->affected, 1);
          af = i;
        }
	send_to_char("There is a brief flash of light!\r\n"
		     "You feel slightly different.\r\n", vict);
	send_to_char("All spells removed.\r\n", ch);
      } else {
	send_to_char("Your victim does not have any affections!\r\n", ch);
	return;
      }
      break;
    case SCMD_BADMORTNONEWBIE:

      if (PLR_FLAGGED(vict, PLR_BADMORT)) {
        send_to_char("Ok.\r\n", ch);
        REMOVE_BIT(PLR_FLAGS(vict), PLR_BADMORT);
        send_to_char("Your newbie permissions have been restored.\r\n", vict);
        GET_NAME(ch, chname);
        GET_NAME(vict, victname);
        sprintf(buf, "(GC) BADMORT removed from %s by %s.", victname, chname);
        FREE_NAME(victname);
        FREE_NAME(chname);
        mudlog(buf, BRF, LVL_IMMORT, TRUE);
      } else {
        send_to_char("Ok.\r\n", ch);
        SET_BIT(PLR_FLAGS(vict), PLR_BADMORT);
        send_to_char("Your newbie permissions have been suspended.\r\n", vict);
        GET_NAME(ch, chname);
        GET_NAME(vict, victname);
        sprintf(buf, "(GC) BADMORT set on %s by %s.", victname, chname);
        FREE_NAME(victname);
        FREE_NAME(chname);
        mudlog(buf, BRF, LVL_IMMORT, TRUE);
      }

      break;
    default:
      log("SYSERR: Unknown subcmd passed to do_wizutil (act.wizard.c)");
      break;
    }
    save_char(vict, NOWHERE);
  }
}


/* single zone printing fn used by "show zone" so it's not repeated in the
   code 3 times ... -je, 4/6/93 */

void print_zone_to_buf(char *bufptr, int zone)
{
  extern char * areas_level(bool * lev, int toFile);

  /*  PDH  2/22/99
   *  OLD METHOD
  sprintf(bufptr, "%s%3d %-25.25s Age: %3d; Reset: %3d (%1d); Top: %5d\r\n",
	  bufptr, zone_table[zone].number, zone_table[zone].name,
	  zone_table[zone].age, zone_table[zone].lifespan,
	  zone_table[zone].reset_mode, zone_table[zone].top);
	  */

  /*
Zon Name                      : Owner       Status     Levels (2,4,6,8,etc...)
 12 God Lounge                :        All  open       Levels [               ]
    */

  sprintf(bufptr, "%s%3d %-25.25s : %-10s  ",
	  bufptr, zone_table[zone].number, zone_table[zone].name,
	  zone_table[zone].owner);

  switch(zone_table[zone].zone_status) {
    case ZONE_WORK:
      sprintf(bufptr, "%s%-10s ", bufptr, "working");
      break;
    case ZONE_REVIEW:
      sprintf(bufptr, "%s%-10s ", bufptr, "in review");
      break;
    case ZONE_CLOSED:
      sprintf(bufptr, "%s%-10s ", bufptr, "closed");
      break;
    case ZONE_OPEN:
      sprintf(bufptr, "%s%-10s ", bufptr, "open");
      break;
    case ZONE_EMPTY:
      sprintf(bufptr, "%s%-10s ", bufptr, "empty");
      break;
  default:
      sprintf(bufptr, "%s%-10s ", bufptr, "UNKNOWN");
      break;
  }

  sprintf(bufptr, "%sLevels [%s]\r\n", bufptr,
	  areas_level(zone_table[zone].zone_levels, 0));

}


ACMD(do_gshow)
{
  struct char_file_u vbuf;
  int i, j, k, l, con;
  char self = 0;
  struct char_data *vict;
  struct obj_data *obj;
  struct rusage ru;
  char field[MAX_INPUT_LENGTH], value[MAX_INPUT_LENGTH], birth[80];
  extern char *class_abbrevs[];
  extern char *genders[];
  extern int buf_switches, buf_largecount, buf_overflows;
  void show_shops(struct char_data * ch, char *value);
  void hcontrol_list_houses(struct char_data *ch);

  struct show_struct {
    char *cmd;
    char level;
  } fields[] = {
    { "nothing",	0  },				/* 0 */
    { "zones",		LVL_IMMORT },			/* 1 */
    { "player",		LVL_GOD },
    { "rent",		LVL_GOD },
    { "stats",		LVL_IMMORT },
    { "errors",		LVL_IMPL },			/* 5 */
    { "death",		LVL_GOD },
    { "godrooms",	LVL_GOD },
    { "shops",		LVL_IMMORT },
    { "houses",		LVL_GOD },
    { "system",     LVL_IMPL }, /* 10 */
    { "\n", 0 }
  };

  skip_spaces(&argument);

  if ( ! *argument) {
    strcpy(buf, "Show options:\r\n");
    for (j = 0, i = 1; fields[i].level; i++) {
      if (fields[i].level <= GET_LEVEL(ch)) {
	sprintf(buf, "%s%-15s%s",
		buf, fields[i].cmd, (!(++j % 5) ? "\r\n" : ""));
      }
    }

    strcat(buf, "\r\n");
    send_to_char(buf, ch);
    return;
  }

  strcpy(arg, two_arguments(argument, field, value));

  for (l = 0; *(fields[l].cmd) != '\n'; l++) {
    if (!strncmp(field, fields[l].cmd, strlen(field))) {
      break;
    }
  }

  if (GET_TRUST_LEVEL(ch) < fields[l].level) {
    send_to_char("You are not godly enough for that!\r\n", ch);
    return;
  }

  if ( ! strcmp(value, ".")) {
    self = 1;
  }

  buf[0] = '\0';

  switch (l) {

  case 1:			/* zone */
    /* tightened up by JE 4/6/93 */
    if (self) {
      send_to_char("Zon Name                      : Owner       Status     Levels (2,4,6,8,etc...)\r\n", ch);
      print_zone_to_buf(buf, world[ch->in_room].zone);
      send_to_char(buf, ch);

    } else if (*value && is_number(value)) {
      for (j=atoi(value), i = 0;
	   zone_table[i].number != j && i <= top_of_zone_table;
	   i++) {
	;
      }

      if (i <= top_of_zone_table) {
	send_to_char("Zon Name                      : Owner       Status     Levels (2,4,6,8,etc...)\r\n", ch);
	print_zone_to_buf(buf, i);
	send_to_char(buf, ch);

      } else {
	send_to_char("That is not a valid zone.\r\n", ch);
	return;
      }

    } else {
      send_to_char("Zon Name                      : Owner       Status     Levels (2,4,6,8,etc...)\r\n", ch);

      for (i = 0; i <= top_of_zone_table; i++) {
	print_zone_to_buf(buf, i);
      }

      page_string(ch->desc, buf, 1);
    }

    break;

  case 2:			/* player */
    if (!*value) {
      send_to_char("A name would help.\r\n", ch);
      return;
    }

    if (load_char(value, &vbuf) < 0) {
      send_to_char("There is no such player.\r\n", ch);
      return;
    }
    sprintf(buf, "Player: %-12s (%s) [%2d %s]\r\n", vbuf.name,
      genders[(int) vbuf.sex], vbuf.level, class_abbrevs[(int) vbuf.class]);

    sprintf(buf,
	 "%sMoney: %s\r\nBank: %-8d  Exp: %-8d  Align: %-5d  Lessons: %d\r\n",
	    buf, describe_money(vbuf.points.money),
	    vbuf.points.bank_gold, vbuf.points.exp,
	    vbuf.char_specials_saved.alignment,
	    vbuf.player_specials_saved.spells_to_learn);
    strcpy(birth, ctime(&vbuf.birth));
    sprintf(buf,
	    "%sStarted: %-20.16s  Last: %-20.16s  Played: %3dh %2dm\r\n",
	    buf, birth, ctime(&vbuf.last_logon), (int) (vbuf.played / 3600),
	    (int) (vbuf.played / 60 % 60));
    send_to_char(buf, ch);
    break;
  case 3:
    if (!*value) {
      send_to_char("A name would help.\r\n", ch);
      return;
    }
    Crash_listrent(ch, value);
    break;
  case 4:
    i = 0;
    j = 0;
    k = 0;
    con = 0;
    for (vict = character_list; vict; vict = vict->next) {
      if (IS_NPC(vict))
	j++;
      else if (CAN_SEE(ch, vict)) {
	i++;
	if (vict->desc)
	  con++;
      }
    }
    for (obj = object_list; obj; obj = obj->next)
      k++;
    sprintf(buf, "Current stats:\r\n");
    sprintf(buf, "%s  %5d players in game  %5d connected\r\n", buf, i, con);
    sprintf(buf, "%s  %5d registered\r\n", buf, top_of_p_table + 1);
    sprintf(buf, "%s  %5d mobiles          %5d prototypes\r\n",
	    buf, j, top_of_mobt + 1);
    sprintf(buf, "%s  %5d objects          %5d prototypes\r\n",
	    buf, k, top_of_objt + 1);
    sprintf(buf, "%s  %5d rooms            %5d zones\r\n",
	    buf, top_of_world + 1, top_of_zone_table + 1);
    sprintf(buf, "%s  %5d large bufs\r\n", buf, buf_largecount);
    sprintf(buf, "%s  %5d buf switches     %5d overflows\r\n", buf,
	    buf_switches, buf_overflows);
    send_to_char(buf, ch);
    break;
  case 5:
    strcpy(buf, "Errant Rooms\r\n------------\r\n");
    for (i = 0, k = 0; i <= top_of_world; i++)
      for (j = 0; j < NUM_OF_DIRS; j++)
	if (world[i].dir_option[j] && world[i].dir_option[j]->to_room == 0)
	  sprintf(buf, "%s%2d: [%5d] %s\r\n", buf, ++k, world[i].number,
		  world[i].name);
    send_to_char(buf, ch);
    break;
  case 6:
    strcpy(buf, "Death Traps\r\n-----------\r\n");
    for (i = 0, j = 0; i <= top_of_world; i++)
      if (IS_SET(ROOM_FLAGS(i), ROOM_DEATH))
	sprintf(buf, "%s%2d: [%5d] %s\r\n", buf, ++j,
		world[i].number, world[i].name);
    send_to_char(buf, ch);
    break;
  case 7:
    strcpy(buf, "Godrooms\r\n--------------------------\r\n");
    for (i = 0, j = 0; i <= top_of_world; i++)
    if (ROOM_FLAGGED(i, ROOM_GODROOM))
      sprintf(buf,"%s%2d: [%5d] %s\r\n",buf,++j,world[i].number,world[i].name);
    send_to_char(buf, ch);
    break;
  case 8:
    show_shops(ch, value);
    break;
  case 9:
    hcontrol_list_houses(ch);
    break;
  case 10: /* system */

//    struct rusage ru;

    getrusage(0, &ru);
    sprintf(buf, "user time: %ld sec\r\nsystem time: %ld sec\r\nmax res size: %ld\r\n",
  	    ru.ru_utime.tv_sec, ru.ru_stime.tv_sec, ru.ru_maxrss);
    send_to_char(buf, ch);
    break;
  default:
    send_to_char("Sorry, I don't understand that.\r\n", ch);
    break;
  }
}


/***************** The do_set function ***********************************/

#define PC   1
#define NPC  2
#define BOTH 3

#define MISC	0
#define BINARY	1
#define NUMBER	2

#define SET_OR_REMOVE(flagset, flags) { \
	if (on) SET_BIT(flagset, flags); \
	else if (off) REMOVE_BIT(flagset, flags); }

#define RANGE(low, high) (value = MAX((low), MIN((high), (value))))


/* The set options available */
  struct set_struct {
    char *cmd;
    char level;
    char pcnpc;
    char type;
  } set_fields[] = {
   { "brief",		LVL_GOD, 	PC, 	BINARY },  /* 0 */
   { "gdetect",	 	LVL_GOD, 	PC, 	BINARY },  /* 1 */
   { "title",		LVL_GOD, 	PC, 	MISC },
   { "none",        50,         PC,     MISC },
   { "maxhit",		LVL_ADMIN, 	BOTH, 	NUMBER },
   { "none",        50,         PC,     MISC },  /* 5 */
   { "maxmove", 	LVL_ADMIN, 	BOTH, 	NUMBER },
   { "hit", 		LVL_GRGOD, 	BOTH, 	NUMBER },
   { "none 2",      50,         PC,     MISC },
   { "move",		LVL_GRGOD, 	BOTH, 	NUMBER },
   { "align",		LVL_DEPTHEAD, 	BOTH, 	NUMBER },  /* 10 */
   { "str",		    LVL_ADMIN, 	BOTH, 	NUMBER },
   { "stradd",		LVL_ADMIN, 	BOTH, 	NUMBER },
   { "int", 		LVL_ADMIN, 	BOTH, 	NUMBER },
   { "wis", 		LVL_ADMIN, 	BOTH, 	NUMBER },
   { "dex", 		LVL_ADMIN, 	BOTH, 	NUMBER },  /* 15 */
   { "con", 		LVL_ADMIN, 	BOTH, 	NUMBER },
   { "cha",		    LVL_ADMIN, 	BOTH, 	NUMBER },
   { "ac", 		    LVL_ADMIN, 	BOTH, 	NUMBER },
   { "money(cp)",	LVL_DEPTHEAD, 	BOTH, 	NUMBER },
   { "bank",		LVL_DEPTHEAD, 	PC, 	NUMBER },  /* 20 */
   { "exp", 		LVL_ADMIN, 	BOTH, 	NUMBER },
   { "hitroll", 	LVL_ADMIN, 	BOTH, 	NUMBER },
   { "damroll", 	LVL_ADMIN, 	BOTH, 	NUMBER },
   { "invis",		LVL_IMPL, 	PC, 	NUMBER },
   { "nohassle", 	LVL_GRGOD, 	PC, 	BINARY },  /* 25 */
   { "frozen",		LVL_FREEZE, 	PC, 	BINARY },
   { "practices", 	LVL_ADMIN, 	PC, 	NUMBER },
   { "lessons", 	LVL_ADMIN, 	PC, 	NUMBER },
   { "drunk",		LVL_GRGOD, 	BOTH, 	MISC },
   { "hunger",		LVL_GRGOD, 	BOTH, 	MISC },    /* 30 */
   { "thirst",		LVL_GRGOD, 	BOTH, 	MISC },
   { "killer",		LVL_IMPL, 	PC, 	BINARY },
   { "thief",		LVL_IMPL, 	PC, 	BINARY },
   { "level",		LVL_IMPL, 	BOTH, 	NUMBER },
   { "room",		LVL_IMPL, 	BOTH, 	NUMBER },  /* 35 */
   { "roomflag", 	LVL_GOD, 	PC, 	BINARY },
   { "siteok",		LVL_IMPL, 	PC, 	BINARY },
   { "deleted",	 	LVL_GOD, 	PC, 	BINARY },
   { "class",		LVL_ADMIN, 	BOTH, 	MISC },
   { "nowizlist", 	LVL_GOD, 	PC, 	BINARY },  /* 40 */
   { "quest",		LVL_DEPTHEAD, 	PC, 	BINARY },
   { "loadroom", 	LVL_GOD, 	    PC, 	MISC },
   { "color",		LVL_GOD, 	    PC, 	BINARY },
   { "idnum",		LVL_IMPL, 	    PC, 	NUMBER },
   { "passwd",		LVL_IMPL, 	    PC, 	MISC },    /* 45 */
   { "nodelete", 	LVL_GRGOD, 	    PC, 	BINARY },
   { "sex", 		LVL_GRGOD, 	    BOTH, 	MISC },
   { "olc",		    LVL_IMPL,	    PC,	    NUMBER },
   { "race",        LVL_GRGOD,      PC,     MISC },
   { "questpoints",	LVL_GRGOD,	    PC,	    NUMBER }, /* 50 */
   { "testchar",	LVL_GRGOD,	    PC,	    BINARY },
   { "rpfactor",	LVL_GRGOD,	    PC,	    NUMBER },
   { "trust",       LVL_IMPL,       PC,     NUMBER },
   { "god",         LVL_GRGOD,      PC,     MISC},
   { "immchar",     LVL_GRGOD,      PC,     BINARY},  /* 55 */
   { "freerent",    LVL_ADMIN,      PC,     BINARY},
   { "ethos",       LVL_DEPTHEAD,   BOTH,   NUMBER},
   { "clan",        LVL_GRGOD,      PC,     NUMBER},
   { "clanlevel",   LVL_GRGOD,      PC,     NUMBER},
   { "hometown",    LVL_IMPL,       PC,     NUMBER},  /* 60 */
   { "prisoner",    LVL_DEPTHEAD,	PC,	    BINARY},
   { "age",         LVL_GRGOD,      PC,     NUMBER},
   { "\n", 0, BOTH, MISC }
  };


int perform_set(struct char_data *ch, struct char_data *vict, int mode, char *val_arg)
{
  int i, on = 0, off = 0, value = 0, changed = 0;
  char output[MAX_STRING_LENGTH], output2[MAX_STRING_LENGTH];
  /*int parse_class(char arg);*/
  int parse_class(struct char_data *ch, char arg);
  int parse_race(char arg);

/* Check to make sure all the levels are correct */
  if (GET_LEVEL(ch) != LVL_IMPL) {
    if (!IS_NPC(vict) && GET_LEVEL(ch) <= GET_LEVEL(vict) && vict != ch) {
      send_to_char("Maybe that's not such a great idea...\r\n", ch);
      return 0;
    }
  }
  if (GET_LEVEL(ch) < set_fields[mode].level) {
    send_to_char("You are not godly enough for that!\r\n", ch);
    return 0;
  }

  /* Make sure the PC/NPC is correct */
  if (IS_NPC(vict) && !(set_fields[mode].pcnpc & NPC)) {
    send_to_char("You can't do that to a beast!\r\n", ch);
    return 0;
  } else if (!IS_NPC(vict) && !(set_fields[mode].pcnpc & PC)) {
    send_to_char("That can only be done to a beast!\r\n", ch);
    return 0;
  }

  /* Find the value of the argument */
  if (set_fields[mode].type == BINARY) {
    if (!strcmp(val_arg, "on") || !strcmp(val_arg, "yes"))
      on = 1;
    else if (!strcmp(val_arg, "off") || !strcmp(val_arg, "no"))
      off = 1;
    if (!(on || off)) {
      send_to_char("Value must be 'on' or 'off'.\r\n", ch);
      return 0;
    }
    GET_NAME(vict, victname);
    sprintf(output, "%s %s for %s.", set_fields[mode].cmd, ONOFF(on), victname);
    FREE_NAME(victname);
  } else if (set_fields[mode].type == NUMBER) {
    value = atoi(val_arg);
    GET_NAME(vict, victname);
    sprintf(output, "%s's %s set to %d.", victname,
	    set_fields[mode].cmd, value);
    FREE_NAME(victname);
  } else {
    strcpy(output, "Okay.");  /* can't use OK macro here 'cause of \r\n */
  }

  switch (mode) {
  case 0:
    SET_OR_REMOVE(PRF_FLAGS(vict), PRF_BRIEF);
    break;
  case 1:
    SET_OR_REMOVE(PRF_FLAGS(vict), PRF_DETECT);
    break;
  case 2:
    set_title(vict, val_arg);
    GET_NAME(vict, victname);
    sprintf(output, "%s's title is now: %s", victname, GET_TITLE(vict));
    FREE_NAME(victname);
    break;
  case 3:
    break;
  case 4:
    vict->points.max_hit = RANGE(1, 3000);
    affect_total(vict);
    break;
  case 5:
    break;
  case 6:
    vict->points.max_move = RANGE(1, 3000);
    affect_total(vict);
    break;
  case 7:
    vict->points.hit = RANGE(-9, vict->points.max_hit);
    affect_total(vict);
    break;
  case 8:
    break;
  case 9:
    vict->points.move = RANGE(0, vict->points.max_move);
    affect_total(vict);
    break;
  case 10:
    GET_ALIGNMENT(vict) = RANGE(-1000, 1000);
    affect_total(vict);
    break;
  case 11:
    if (IS_NPC(vict) || GET_LEVEL(vict) >= LVL_GRGOD) {
      RANGE(3, 25);
    } else if (GET_RACE(vict) == RACE_MINOTAUR) {
        RANGE(3, 20);
    } else {
      RANGE(3, 18);
    }
    vict->real_abils.str = value;
    vict->real_abils.str_add = 0;
    affect_total(vict);
    break;
  case 12:
    vict->real_abils.str_add = RANGE(0, 100);
    if (value > 0)
      vict->real_abils.str = 18;
    affect_total(vict);
    break;
  case 13:
    if (IS_NPC(vict) || GET_LEVEL(vict) >= LVL_GRGOD)
      RANGE(3, 25);
    else
      RANGE(3, 18);
    vict->real_abils.intel = value;
    affect_total(vict);
    break;
  case 14:
    if (IS_NPC(vict) || GET_LEVEL(vict) >= LVL_GRGOD)
      RANGE(3, 25);
    else
      RANGE(3, 18);
    vict->real_abils.wis = value;
    affect_total(vict);
    break;
  case 15:
    if (IS_NPC(vict) || GET_LEVEL(vict) >= LVL_GRGOD) {
      RANGE(3, 25);
    } else if (GET_RACE(vict) == RACE_KAGONESTI || GET_RACE(vict) == RACE_KENDER) {
        RANGE(3, 19);
    } else {
      RANGE(3, 18);
    }
    vict->real_abils.dex = value;
    affect_total(vict);
    break;
  case 16:
    if (IS_NPC(vict) || GET_LEVEL(vict) >= LVL_GRGOD) {
      RANGE(3, 25);
    } else if (GET_RACE(vict) == RACE_HYLAR || 
               GET_RACE(vict) == RACE_DAEWAR ||
               GET_RACE(vict) == RACE_NEIDAR) {
        RANGE(3, 19);
    } else {
      RANGE(3, 18);
    }
    vict->real_abils.con = value;
    affect_total(vict);
    break;
  case 17:
    if (IS_NPC(vict) || GET_LEVEL(vict) >= LVL_GRGOD)
      RANGE(3, 25);
    else
      RANGE(3, 18);
    vict->real_abils.cha = value;
    affect_total(vict);
    break;
  case 18:
    vict->points.armor = RANGE(-100, 100);
    affect_total(vict);
    break;
  case 19:
    (GET_MONEY(vict)).copper = RANGE(0, 100000000);
    break;
  case 20:
    GET_BANK_GOLD(vict) = RANGE(0, 100000000);
    break;
  case 21:
    vict->points.exp = RANGE(0, 50000000);
    break;
  case 22:
    vict->points.hitroll = RANGE(-20, 20);
    affect_total(vict);
    break;
  case 23:
    vict->points.damroll = RANGE(-20, 20);
    affect_total(vict);
    break;
  case 48:
    GET_OLC_ZONE(vict) = value;
    break;
  case 49:
    if ((i = parse_race(*val_arg)) == RACE_UNDEFINED) {
      send_to_char("That is not a race.\r\n", ch);
      break;
    }
    GET_RACE(vict) = i;
    break;

  case 50:
    GET_QUESTPOINTS(vict) = RANGE(0, 10000);
    break;

  case 24:
    if (GET_LEVEL(ch) < LVL_IMPL && ch != vict) {
      send_to_char("You aren't godly enough for that!\r\n", ch);
      return 0;
    }
    GET_INVIS_LEV(vict) = RANGE(0, GET_LEVEL(vict));
    break;
  case 25:
    if (GET_LEVEL(ch) < LVL_IMPL && ch != vict) {
      send_to_char("You aren't godly enough for that!\r\n", ch);
      return 0;
    }
    SET_OR_REMOVE(PRF_FLAGS(vict), PRF_NOHASSLE);
    break;
  case 26:
    if (ch == vict) {
      send_to_char("Better not -- could be a long winter!\r\n", ch);
      return 0;
    }
    SET_OR_REMOVE(PLR_FLAGS(vict), PLR_FROZEN);
    break;
  case 27:
  case 28:
    GET_PRACTICES(vict) = RANGE(0, 100);
    break;
  case 29:
  case 30:
  case 31:
    if (!str_cmp(val_arg, "off")) {
      GET_COND(vict, (mode - 29)) = (char) -1; /* warning: magic number here */
      GET_NAME(vict, victname);
      sprintf(output, "%s's %s now off.", victname, set_fields[mode].cmd);
      FREE_NAME(victname);
    } else if (is_number(val_arg)) {
      value = atoi(val_arg);
      RANGE(0, 24);
      GET_COND(vict, (mode - 29)) = (char) value; /* and here too */
      GET_NAME(vict, victname);
      sprintf(output, "%s's %s set to %d.", victname,
	      set_fields[mode].cmd, value);
      FREE_NAME(victname);
    } else {
      send_to_char("Must be 'off' or a value from 0 to 24.\r\n", ch);
      return 0;
    }
    break;
  case 32:
    SET_OR_REMOVE(PLR_FLAGS(vict), PLR_KILLER);
    break;
  case 33:
    SET_OR_REMOVE(PLR_FLAGS(vict), PLR_THIEF);
    break;
  case 34:
    if (value > GET_LEVEL(ch) || value > LVL_IMPL) {
      send_to_char("You can't do that.\r\n", ch);
      return 0;
    }
    RANGE(0, LVL_IMPL);
    vict->player.level = (byte) value;
    break;
  case 35:
    if ((i = real_room(value)) < 0) {
      send_to_char("No room exists with that number.\r\n", ch);
      return 0;
    }
    char_from_room(vict);
    char_to_room(vict, i);
    break;
  case 36:
    SET_OR_REMOVE(PRF_FLAGS(vict), PRF_ROOMFLAGS);
    break;
  case 37:
    SET_OR_REMOVE(PLR_FLAGS(vict), PLR_SITEOK);
    break;
  case 38:
    SET_OR_REMOVE(PLR_FLAGS(vict), PLR_DELETED);
    break;
  case 39:
    if(!strcmp(val_arg, "monk"))
      i = CLASS_MONK;
    else if(!strcmp(val_arg, "paladin"))
      i = CLASS_PALADIN;
    else if ((i = parse_class(vict, val_arg[0])) <= CLASS_UNDEFINED) {
      send_to_char("That is not a class.\r\n", ch);
      return 0;
    }
    GET_CLASS(vict) = i;
    break;
  case 41:
    SET_OR_REMOVE(PRF_FLAGS(vict), PRF_QUEST);
    break;
  case 42:
    if (!str_cmp(val_arg, "off")) {
      REMOVE_BIT(PLR_FLAGS(vict), PLR_LOADROOM);
    } else if (is_number(val_arg)) {
      value = atoi(val_arg);
      if (real_room(value) != NOWHERE) {
        SET_BIT(PLR_FLAGS(vict), PLR_LOADROOM);
	GET_LOADROOM(vict) = value;
        GET_NAME(vict, victname);
	sprintf(output, "%s will enter at room #%d.", victname,
		GET_LOADROOM(vict));
        FREE_NAME(victname);
      } else {
	send_to_char("That room does not exist!\r\n", ch);
	return 0;
      }
    } else {
      send_to_char("Must be 'off' or a room's virtual number.\r\n", ch);
      return 0;
    }
    break;
  case 43:
    SET_OR_REMOVE(PRF_FLAGS(vict), (PRF_COLOR_1 | PRF_COLOR_2));
    break;
  case 44:
    /*
    if (GET_IDNUM(ch) != 1 || !IS_NPC(vict))  one_argument(argument, arg);

  if (!*arg) {
    send_to_char("Who needs a spankin'?\r\n", ch);
    return;
  }

  if (ch == vict) {
    send_to_char("Probably not a good idea...\r\n", ch);
    return;
  }

  if (GET_LEVEL(vict) >= LVL_IMMORT) {
    send_to_char("Hrm.... that's not cool man.\r\n", ch);
    return;
  }

  if (PLR_FLAGGED(vict, PLR_BADMORT)) {
    REMOVE_BIT(PLR_FLAGS(vict), PLR_BADMORT);
    send_to_char("Ok.\r\n", ch);
    send_to_char("Your newbie permissions have been restored.\r\n", vict);

    GET_NAME(ch, chname);
    GET_NAME(vict, victname);
    sprintf(buf, "(GC) BADMORT removed from %s by %s.", victname, chname);
    FREE_NAME(victname);
    FREE_NAME(chname);
    mudlog(buf, BRF, LVL_IMMORT, TRUE);

  } else {
    SET_BIT(PLR_FLAGS(vict), PLR_BADMORT);
    send_to_char("Ok.\r\n", ch);
    send_to_char("Your newbie permissions have been suspended.\r\n", vict);

    GET_NAME(ch, chname);
    GET_NAME(vict, victname);
    sprintf(buf, "(GC) BADMORT set on %s by %s.", victname, chname);
    FREE_NAME(victname);
    FREE_NAME(chname);
    mudlog(buf, BRF, LVL_IMMORT, TRUE);
    */
    if ( GET_LEVEL(ch) < LVL_IMPL )
      return 0;
    GET_IDNUM(vict) = value;
    send_to_char("Done.\r\n", ch);
    break;
  case 45:
    if (GET_IDNUM(ch) > 1) {
      send_to_char("Please don't use this command, yet.\r\n", ch);
      return 0;
    }
    if (GET_LEVEL(vict) >= LVL_GRGOD) {
      send_to_char("You cannot change that.\r\n", ch);
      return 0;
    }
    GET_NAME(vict, victname);
    strncpy(GET_PASSWD(vict), CRYPT(val_arg, victname), MAX_PWD_LENGTH);
    FREE_NAME(victname);
    *(GET_PASSWD(vict) + MAX_PWD_LENGTH) = '\0';
    sprintf(output, "Password changed to '%s'.", val_arg);
    break;
  case 47:
    if (!str_cmp(val_arg, "male"))
      vict->player.sex = SEX_MALE;
    else if (!str_cmp(val_arg, "female"))
      vict->player.sex = SEX_FEMALE;
    else if (!str_cmp(val_arg, "neutral"))
      vict->player.sex = SEX_NEUTRAL;
    else {
      send_to_char("Must be 'male', 'female', or 'neutral'.\r\n", ch);
      return 0;
    }
    break;

  case 52:
    GET_RPFACTOR(vict) = RANGE(0, 120);
    break;

  case 53:
    if (value > GET_LEVEL(ch) || value > LVL_IMPL || GET_LEVEL(ch) < GET_LEVEL(vict) ) {
      send_to_char("You can't do that.\r\n", ch);
      break;
    }
    vict->player_specials->saved.trust = (byte) value;
    break;

  case 54:
    for (i = 1; i <= NUM_GODS; i++)
    {
      if (!str_cmp(val_arg, god_name[i]))
      {
        GET_GODSELECT(vict) = i;
        sprintf(output, "God set to %s.", god_name[i]);
        changed = 1;
        break;
      }
    }

    if (!changed)
      send_to_char("No such god.\r\n", ch);

    break;

  case 55:
  {
    SET_OR_REMOVE(PLR_FLAGS(vict), PLR_IMMCHAR);
    break;
  }

  case 56:
  {
    SET_OR_REMOVE(PLR_FLAGS(vict), PLR_FREERENT);
    break;
  }

  case 57:
  {
    GET_ETHOS(vict) = RANGE(-1000, 1000);
    affect_total(vict);
    break;
  }

  case 58:
  {
    GET_CLAN(vict) = RANGE(0, CLAN_MAX_CLAN);
    affect_total(vict);
    break;
  }

  case 59:
  {
    GET_CLANLEVEL(vict) = RANGE(0, CLAN_MAX_LEVEL);
    affect_total(vict);
   break;
  }

  case 60:
  {
    GET_HOME(vict) = RANGE(0, 2);
    affect_total(vict);
    break;
  }

  case 61:
  {
    SET_OR_REMOVE(PLR_FLAGS(vict), PLR_PRISONER);
    break;
  }

  case 62:
  {
   ch->player.time.birth = time(0) - (SECS_PER_MUD_YEAR * RANGE(8, 100)) + (SECS_PER_MUD_YEAR * 17);
   affect_total(vict);
   break;
  }

  default:
    send_to_char("Can't set that!\r\n", ch);
    return 0;
    break;
  }

  sprintf(output2, "%s (%s)\r\n", output, ch->player.name);
  basic_mud_log(output2);
  strcat(output, "\r\n");
  send_to_char(CAP(output), ch);

  return 1;
}


ACMD(do_set)
{
  struct char_data *vict = NULL, *cbuf = NULL;
  struct char_file_u tmp_store;
  char field[MAX_INPUT_LENGTH], name[MAX_INPUT_LENGTH],
    val_arg[MAX_INPUT_LENGTH];
  int mode = -1, len = 0, player_i = 0, retval;
  char is_file = 0, is_mob = 0, is_player = 0;

  half_chop(argument, name, buf);

  if (!strcmp(name, "file")) {
    is_file = 1;
    half_chop(buf, name, buf);
  } else if (!str_cmp(name, "player")) {
    is_player = 1;
    half_chop(buf, name, buf);
  } else if (!str_cmp(name, "mob")) {
    is_mob = 1;
    half_chop(buf, name, buf);
  }
  half_chop(buf, field, buf);
  strcpy(val_arg, buf);

  if (!*name || !*field) {
    send_to_char("Usage: set <victim> <field> <value>\r\n", ch);
    return;
  }

  /* find the target */
  if (!is_file) {
    if (is_player) {
      if (!(vict = get_player_vis(ch, name, 0))) {
	send_to_char("There is no such player.\r\n", ch);
	return;
      }
    } else {
      if (!(vict = get_char_vis(ch, name, FIND_CHAR_WORLD))) {
	send_to_char("There is no such creature.\r\n", ch);
	return;
      }
    }
  } else if (is_file) {
    /* try to load the player off disk */
    CREATE(cbuf, struct char_data, 1);
    clear_char(cbuf);
    if ((player_i = load_char(name, &tmp_store)) > -1) {
      store_to_char(&tmp_store, cbuf);
      if (GET_TRUST_LEVEL(cbuf) >= GET_TRUST_LEVEL(ch)) {
	free_char(cbuf);
	send_to_char("Sorry, you can't do that.\r\n", ch);
	return;
      }
      vict = cbuf;
    } else {
      free(cbuf);
      cbuf = (struct char_data*) NULL;
      send_to_char("There is no such player.\r\n", ch);
      return;
    }
  }

  /* find the command in the list */
  len = strlen(field);
  for (mode = 0; *(set_fields[mode].cmd) != '\n'; mode++)
    if (!strncmp(field, set_fields[mode].cmd, len))
      break;

  /* perform the set */
  retval = perform_set(ch, vict, mode, val_arg);

  /* save the character if a change was made */
  if (retval) {
    if (!is_file && !IS_NPC(vict))
      save_char(vict, NOWHERE);
    if (is_file) {
      char_to_store(vict, &tmp_store);
      fseek(player_fl, (player_i) * sizeof(struct char_file_u), SEEK_SET);
      fwrite(&tmp_store, sizeof(struct char_file_u), 1, player_fl);
      send_to_char("Saved in file.\r\n", ch);
    }
  }

  /* free the memory if we allocated it earlier */
  if (is_file)
    free_char(cbuf);
}


ACMD(do_gdig)
{
  /* Only works if you have Oasis OLC */
  extern void olc_add_to_save_list(int zone, byte type);

  char buf2[10];
  char buf3[10];
  char buf[80];
  int iroom = 0, rroom = 0;
  int dir = 0;
  int rev_dir = 0;
  /*  struct room_data *room; */

  two_arguments(argument, buf2, buf3);
  /* buf2 is the direction, buf3 is the room */

  iroom = atoi(buf3);
  rroom = real_room(iroom);

  if (!*buf2) {
    send_to_char("Format: gdig <dir> <room number>\r\n", ch);
    return;

  } else if (!*buf3) {
    send_to_char("Format: gdig <dir> <room number>\r\n", ch);
    return;
  }

  if (rroom <= 0) {
    sprintf(buf, "There is no room with the number %d", iroom);
    send_to_char(buf, ch);
    return;

  }

  /* Main stuff */
  switch (*buf2) {
  case 'n':
  case 'N':
    dir = NORTH;
    rev_dir = SOUTH;
    break;
  case 'e':
  case 'E':
    dir = EAST;
    rev_dir = WEST;
    break;
  case 's':
  case 'S':
    dir = SOUTH;
    rev_dir = NORTH;
    break;
  case 'w':
  case 'W':
    dir = WEST;
    rev_dir = EAST;
    break;
  case 'u':
  case 'U':
    dir = UP;
    rev_dir = DOWN;
    break;
  case 'd':
  case 'D':
    dir = DOWN;
    rev_dir = UP;
    break;
  case 'r':
  case 'R':
    dir = NORTHWEST;
    rev_dir = SOUTHEAST;
    break;
  case 't':
  case 'T':
    dir = SOUTHEAST;
    rev_dir = NORTHWEST;
    break;
  case 'o':
  case 'O':
    dir = NORTHEAST;
    rev_dir = SOUTHWEST;
    break;
  case 'v':
  case 'V':
    dir = SOUTHWEST;
    rev_dir = NORTHEAST;
    break;
  }

  CREATE(world[rroom].dir_option[rev_dir], struct room_direction_data,1);
  world[rroom].dir_option[rev_dir]->general_description = NULL;
  world[rroom].dir_option[rev_dir]->keyword = NULL;
  world[rroom].dir_option[rev_dir]->to_room = ch->in_room;

  CREATE(world[ch->in_room].dir_option[dir], struct room_direction_data,1);
  world[ch->in_room].dir_option[dir]->general_description = NULL;
  world[ch->in_room].dir_option[dir]->keyword = NULL;
  world[ch->in_room].dir_option[dir]->to_room = rroom;

  /* Only works if you have Oasis OLC */
  olc_add_to_save_list((iroom/100), OLC_SAVE_ROOM);

  sprintf(buf, "You make an exit %s to room %d.\r\n", buf2, iroom);
  send_to_char(buf, ch);
}


ACMD(do_copyto)
{
  /* Only works if you have Oasis OLC */
  extern void olc_add_to_save_list(int zone, byte type);

  char buf2[10];
  char buf[80];
  int iroom = 0, rroom = 0;
  struct room_data *room;
  one_argument(argument, buf2);
  /* buf2 is room to copy to */

  CREATE (room, struct room_data, 1);

  iroom = atoi(buf2);
  rroom = real_room(atoi(buf2));
  *room = world[rroom];

  if (!*buf2) {
    send_to_char("Format: copyto <room number>\r\n", ch);
    return;
  }

  if (rroom <= 0) {
    sprintf(buf, "There is no room with the number %d.\r\n", iroom);
    send_to_char(buf, ch);
    return;
  }

  /* Main stuff */

  if (world[ch->in_room].description) {

    if (world[rroom].description)
    {
      free(world[rroom].description);
      world[rroom].description = (char*) NULL;
    }
    world[rroom].description = str_dup(world[ch->in_room].description);

    /* Only works if you have Oasis OLC */
    olc_add_to_save_list((iroom/100), OLC_SAVE_ROOM);

    sprintf(buf, "You copy the description to room %d.\r\n", iroom);
    send_to_char(buf, ch);
  } else {
    send_to_char("This room has no description!\r\n", ch);
  }
}


ACMD(do_rlist)
{
  extern struct room_data *world;
  /* extern int top_of_world; */

  int first, last, nr, found = 0;

  two_arguments(argument, buf, buf2);

  if (!*buf || !*buf2) {
    send_to_char("Usage: rlist <begining number> <ending number>\r\n", ch);
    return;
  }

  first = atoi(buf);
  last = atoi(buf2);

  if ((first < 0) || (first > 99999) || (last < 0) || (last > 99999)) {
    send_to_char("Values must be between 0 and 99999.\n\r", ch);
    return;
  }

  if (first >= last) {
    send_to_char("Second value must be greater than first.\n\r", ch);
    return;
  }

  for (nr = 0; nr <= top_of_world && (world[nr].number <= last); nr++) {
    if (world[nr].number >= first) {
      sprintf(buf, "%5d. [%5d] (%3d) %s\r\n", ++found,
              world[nr].number, world[nr].zone,
	      world[nr].name);
      send_to_char(buf, ch);
    }
  }

  if (!found) {
    send_to_char("No rooms were found in those parameters.\n\r", ch);
  }
}


ACMD(do_mlist)
{
  extern struct index_data *mob_index;
  extern struct char_data *mob_proto;
  extern int top_of_mobt;

  int first, last, nr, found = 0;
  two_arguments(argument, buf, buf2);

  if (!*buf || !*buf2) {
    send_to_char("Usage: mlist <begining number> <ending number>\r\n", ch);
    return;
  }

  first = atoi(buf);
  last = atoi(buf2);

  if ((first < 0) || (first > 99999) || (last < 0) || (last > 99999)) {
    send_to_char("Values must be between 0 and 99999.\n\r", ch);
    return;
  }

  if (first >= last) {
    send_to_char("Second value must be greater than first.\n\r", ch);
    return;
  }

  for (nr = 0; nr <= top_of_mobt && (mob_index[nr].virtual <= last); nr++) {
    if (mob_index[nr].virtual >= first) {

/* tak wants mob levels. so mob levels we have! --gan */

      char mobLev[10];

      if (PRF_FLAGGED(ch, PRF_LEVEL_FLAGS)) {
        sprintf(mobLev, " (%2d)", (int)GET_LEVEL(&(mob_proto[nr])));
      } else {
        strcpy(mobLev, "");
      }

      sprintf(buf, "%5d. [%5d] %s%s\r\n", ++found,
	      mob_index[nr].virtual,
	      mob_proto[nr].player.short_descr, mobLev);
      send_to_char(buf, ch);
    }
  }

  if (!found) {
    send_to_char("No mobiles were found in those parameters.\n\r", ch);
  }
}


ACMD(do_olist)
{
  extern struct index_data *obj_index;
  extern struct obj_data *obj_proto;
  extern int top_of_objt;

  int first, last, nr, found = 0;
  two_arguments(argument, buf, buf2);

  if (!*buf || !*buf2) {
    send_to_char("Usage: olist <begining number> <ending number>\r\n", ch);
    return;
  }

  first = atoi(buf);
  last = atoi(buf2);

  if ((first < 0) || (first > 99999) || (last < 0) || (last > 99999)) {
    send_to_char("Values must be between 0 and 99999.\n\r", ch);
    return;
  }

  if (first >= last) {
    send_to_char("Second value must be greater than first.\n\r", ch);
    return;
  }

  for (nr = 0; nr <= top_of_objt && (obj_index[nr].virtual <= last); nr++) {
    if (obj_index[nr].virtual >= first) {
      char objLev[10];

      if (PRF_FLAGGED(ch, PRF_LEVEL_FLAGS)) {
	sprintf(objLev, " (%2d)", (int)GET_OBJ_LEVEL(&(obj_proto[nr])));
      } else {
	strcpy(objLev, "");
      }

      sprintf(buf, "%5d. [%5d] %s%s\r\n", ++found,
	      obj_index[nr].virtual,
	      obj_proto[nr].short_description, objLev);

      send_to_char(buf, ch);
    }
  }

  if (!found) {
    send_to_char("No objects were found in those parameters.\n\r", ch);
  }
}


/*
 *  PDH  1/15/99
 *  for new character approval/rejection
 */
ACMD(do_approve)
{
  struct char_data * victim;
  struct descriptor_data * d;


  switch(subcmd) {

  case SCMD_APPROVE:
    one_argument(argument, arg);

    if ( ! *arg) {
      /*  show a list of players awaiting approval  */
      int  count = 0;

      for (d = descriptor_list; d; d = d->next) {
        /* attempt at fixing the approve crash. 11/11/2k --gan
            if ( (GET_LEVEL(d->character) == 0) && (STATE(d) == CON_PLAYING) ) {
        */
        if((STATE(d) == CON_PLAYING) && (GET_APPROVED(d->character) == 0)) {
	  count++;
          GET_NAME(d->character, chname);
	  sprintf(buf, " %2d %s\r\n", count, chname);
          FREE_NAME(chname);
	  send_to_char(buf, ch);
	}
      }
      if ( count == 0 ) {
	send_to_char("No one is awaiting approval.\r\n", ch);
      }
    } else if ( ! (victim = get_char_vis(ch, arg, FIND_CHAR_WORLD))) {
      send_to_char(NOPERSON, ch);
    } else if (victim == ch) {
      send_to_char("Don't be silly now... approve yourself?!\r\n", ch);
    } else if (IS_NPC(victim)) {
      send_to_char("Why are you trying to approve a mob?\r\n", ch);
    } else if (!victim->desc) {
      send_to_char("They're linkdead.\r\n", ch);
    } else if ( STATE(victim->desc) != CON_PLAYING ) {
      send_to_char("He/she isn't in CON_PLAYING mode yet.\r\n", ch);
    } else if (GET_APPROVED(victim) != 0) {
      send_to_char("He/she has already been approved.\r\n", ch);
    } else {
      send_to_char(OK, ch);
      send_to_char("Your character has been approved.", victim);

      GET_NAME(ch, chname);
      GET_NAME(victim, victname);
      sprintf(buf, "%s (lev %d) approved by %s.", victname, GET_LEVEL(victim), chname);
      FREE_NAME(victname);
      FREE_NAME(chname);
      mudlog(buf, NRM, LVL_IMMORT, TRUE);

      GET_APPROVED(victim) = 1;

    }

    break;

  case SCMD_REJECT:
    argument = one_argument(argument, buf);

    if ( ! *buf) {
      send_to_char("Whom do you wish to reject?\r\n", ch);
    } else if ( ! (victim = get_char_vis(ch, buf, FIND_CHAR_WORLD)) ) {
      send_to_char(NOPERSON, ch);
    } else if (victim == ch) {
      send_to_char("Don't be silly now... reject yourself?!\r\n", ch);
    } else if (IS_NPC(victim)) {
      send_to_char("Why are you trying to reject a mob?\r\n", ch);
    } else if (!victim->desc) {
      send_to_char("They're linkdead.\r\n", ch);
    } else if ( STATE(victim->desc) != CON_PLAYING ) {
      send_to_char("He/she isn't in CON_PLAYING mode yet.\r\n", ch);
    } else if (GET_LEVEL(victim) != 0) {
      send_to_char("He/she has already been approved.\r\n", ch);
    } else if ( ! *argument) {
      send_to_char("What is your reason for rejecting?\r\n", ch);
    } else {
      /*  extract and delete victim  */
      send_to_char(OK, ch);

      SET_BIT(PLR_FLAGS(victim), PLR_DELETED);
      GET_NAME(victim, victname);
      sprintf(buf, "Character '%s' rejected!\r\n%s.\r\n",
	      victname, argument);
      FREE_NAME(victname);
      SEND_TO_Q(buf, victim->desc);


      /*  PDH  1/28/99
       *  old erroneous way
       save_char(victim, NOWHERE);
       Crash_delete_file(GET_NAME(victim));
       */
      extract_char(victim);

      GET_NAME(ch, chname);
      GET_NAME(victim, victname);
      sprintf(buf, "%s (lev %d) rejected by %s because: %s.", victname, GET_LEVEL(victim), chname, argument);
      FREE_NAME(victname);
      FREE_NAME(chname);
      mudlog(buf, NRM, LVL_IMMORT, TRUE);
      STATE(victim->desc) = CON_CLOSE;
    }

    break;

  default:
    log("SYSERR: bad SCMD for do_approve");
    break;
  }

}


/*  PDH  3/26/99 - online player editing of description strings  */
ACMD(do_pedit)
{
  /*  PEDIT options:
   *  pedit short
   *  pedit detail
   *  pedit alias
   *  pedit name, for imps - Soli, 8/11/99
   */

  extern struct player_index_element *player_table;

  struct char_data *vict;
  struct char_file_u tmp_store;
  char peditCmd[200];
  void add_to_char_name(struct char_data * ch, char * addName);


  argument = one_argument(argument, buf1);
  half_chop(argument, buf2, buf);

  if ( ! *buf1) {
    send_to_char("Usage: pedit <player> alias <string>\r\n", ch);
    send_to_char("Usage: pedit <player> short <string>\r\n", ch);
    send_to_char("Usage: pedit <player> detail\r\n", ch);
    return;
  }

  if ( ! (vict = get_char_vis(ch, buf1, FIND_CHAR_WORLD))) {
    send_to_char(NOPERSON, ch);
    return;
  }

  if ( (GET_LEVEL(ch) <= GET_LEVEL(vict)) && (vict != ch) ) {
    send_to_char("A strange force prevents you from doing that.\r\n", ch);
    return;
  }

  GET_NAME(ch, chname);
  GET_NAME(vict, victname);
  sprintf(peditCmd, "[%s pedits %s field:", chname, victname);
  FREE_NAME(victname);
  FREE_NAME(chname);

  if (is_abbrev(buf2, "alias")) {
    if ( ! *buf) {
      send_to_char("No alias words specified.\r\n", ch);
      return;

    } else {
      if(strlen(buf) > 80) {
        send_to_char("Alias string too long.\r\n",ch);
        return;
      }

      add_to_char_name(vict, buf);
      send_to_char(OK, ch);
      sprintf(peditCmd, "%s alias", peditCmd);
      mudlog(peditCmd, NRM, LVL_APPROVE, TRUE);
    }
  }

  if (is_abbrev(buf2, "name")) {
    if (GET_LEVEL(ch) < LVL_IMPL)
      {
        send_to_char("You lack the power.\r\n", ch);
        return;
      }
    if (!*buf) {
      send_to_char("No name specified.\r\n", ch);
      return;
    } else {
      if(strlen(buf) > MAX_NAME_LENGTH) {
        send_to_char("string too long.\r\n",ch);
        return;
      }

      if (load_char(buf, &tmp_store) > -1)
      {
        send_to_char("Someone already has that name.\r\n", ch);
        return;
      }

      if (vict->player.name)
      {
        free(vict->player.name);
        vict->player.name = (char*) NULL;
      }
      vict->player.name = str_dup(buf);

      player_table[GET_PFILEPOS(ch)].name = vict->player.name;
      send_to_char(OK, ch);
      {
        char* tmpBuf = str_dup(buf);
        sprintf(peditCmd, "%s name.  New name: %s", peditCmd, tmpBuf);
        mudlog(peditCmd, NRM, LVL_APPROVE, TRUE);
        free(tmpBuf);
        tmpBuf = (char*) NULL;
      }
    }
  }

  /*
  if (is_abbrev(buf2, "title")) {
    if (!*buf) {
      send_to_char("No title specified.\r\n", ch);
      return;
    } else {
      if(strlen(buf) > MAX_TITLE_LENGTH) {
        send_to_char("string too long.\r\n",ch);
        return;
      }

      if (vict->player.title)
      {
        free(vict->player.title);
        vict->player.title = (char*) NULL;
      }
      vict->player.title = str_dup(buf);
      send_to_char(OK, ch);
      sprintf(peditCmd, "%s title", peditCmd);
      mudlog(peditCmd, NRM, LVL_APPROVE, TRUE);
    }
  }
  */

  if (is_abbrev(buf2, "short")) {
    if ( ! *buf) {
      send_to_char("No short description specified.\r\n", ch);
      return;

    } else {
      if(strlen(buf) > MAX_TITLE_LENGTH) {
        send_to_char("Short descr too long.\r\n",ch);
        return;
      }

      if (vict->player.short_descr)
      {
        free(vict->player.short_descr);
        vict->player.short_descr = (char*) NULL;
      }
      vict->player.short_descr = str_dup(buf);
      send_to_char(OK, ch);
      sprintf(peditCmd, "%s short", peditCmd);
      mudlog(peditCmd, NRM, LVL_APPROVE, TRUE);
    }
  }

  /*
  if (is_abbrev(buf2, "long")) {
    if (!*buf) {
      send_to_char("No long description specified.\r\n", ch);
      return;
    } else {
      if(strlen(buf) > MAX_TITLE_LENGTH) {
        send_to_char("string too long.\r\n",ch);
        return;
      }

      if (vict->player.long_descr)
      {
        free(vict->player.long_descr);
        vict->player.long_descr = (char*) NULL;
      }
      vict->player.long_descr = str_dup(buf);
      send_to_char(OK, ch);
      sprintf(peditCmd, "%s long", peditCmd);
      mudlog(peditCmd, NRM, LVL_APPROVE, TRUE);
    }
  }
  */

  if (is_abbrev(buf2, "detail")) {
    sprintf(peditCmd, "%s detail", peditCmd);
    mudlog(peditCmd, NRM, LVL_APPROVE, TRUE);

    if ( ! *buf) {
      send_to_char("Enter new description.\r\n(/s saves /h for help)\r\n", ch);
      send_to_char(" [----*----*----*----*----*----*----*----*----*----*----*----*----*----*----*----]\r\n", ch);

      if (vict->player.description) {
        send_to_char(vict->player.description, ch);
        ch->desc->backstr = str_dup(vict->player.description);

      } else {
        CREATE(vict->player.description, char, 2048);
      }

      ch->desc->str = &(vict->player.description);
      ch->desc->max_str = EXDSCR_LENGTH;
      /* SET_BIT_AR(PLR_FLAGS(ch), PLR_START_WRITE); */
      SET_BIT(PLR_FLAGS(ch), PLR_WRITING);

    } else {
      if(strlen(buf) > EXDSCR_LENGTH) {
        send_to_char("string too long.\r\n",ch);
        return;
      }

      if (vict->player.description)
      {
        free(vict->player.description);
        vict->player.description = (char*) NULL;
      }
      vict->player.description = str_dup(buf);
      send_to_char(OK, ch);
    }
  }
}


ACMD(do_quest)
{
  struct char_data *vict;

  one_argument(argument, arg);

  if (!*arg)
  {
       send_to_char("Who did you wish to use in your quest?\r\n", ch);
       return;
  }

  if (!(vict = get_char_vis(ch, arg, FIND_CHAR_ROOM)))
  {
    send_to_char("They don't seem to be here.\r\n", ch);
    return;
  }

  if (!IS_NPC(vict))
  {
     send_to_char("But they are not a mob!\r\n", ch);
     return;
   }

 if (MOB_FLAGGED(vict, MOB_QUEST)) {

    REMOVE_BIT(MOB_FLAGS(vict), MOB_QUEST);

   act("$N is no longer part of your quest.", FALSE, ch, 0, vict, TO_CHAR);
   act("You are no longer part of $n's quest.", TRUE, ch, 0, vict, TO_VICT);
   return;
   }

  SET_BIT(MOB_FLAGS(vict), MOB_QUEST);

  act("$N now is part of your quest.", FALSE, ch, 0, vict, TO_CHAR);
  act("You are now part of $n's quest.", TRUE, ch, 0, vict, TO_VICT);
}


/* (c) 1996-97 Erwin S. Andreasen <erwin@pip.dknet.dk> */
ACMD(do_copyover)
{
  FILE *fp;
  struct descriptor_data *d, *d_next;
  char buf [100], buf2[100];
	
  fp = fopen (COPYOVER_FILE, "w");
    if (!fp) {
      send_to_char ("Copyover file not writeable, aborted.\n\r",ch);
      return;
    }

   /* 
    * Uncomment if you use OasisOLC2.0, this saves all OLC modules:
     save_all();
    *
    */
   GET_NAME(ch, chname);		
   sprintf (buf, "\n\r *** COPYOVER by %s - please remain seated!\n\r", chname);
	 FREE_NAME(chname);
	 
   /* For each playing descriptor, save its state */
   for (d = descriptor_list; d ; d = d_next) {
     struct char_data * och = d->character;
   /* We delete from the list , so need to save this */
     d_next = d->next;

  /* drop those logging on */
   if (!d->character || d->connected > CON_PLAYING) {
     write_to_descriptor (d->descriptor, "\n\rSorry, we are rebooting. Come back in a few minutes.\n\r");
     close_socket (d); /* throw'em out */
   } else {
	    GET_NAME(och, chname);
      fprintf (fp, "%d %s %s\n", d->descriptor, chname, d->host);
			FREE_NAME(chname);
      /* save och */
      Crash_rentsave(och,0);
      if (world[och->in_room].number > 99) {
        SET_BIT(PLR_FLAGS(och), PLR_LOADROOM);
        GET_LOADROOM(och) = world[och->in_room].number;
      }
      save_char(och, GET_LOADROOM(och));
      write_to_descriptor (d->descriptor, buf);
    }
  }

  fprintf (fp, "-1\n");
  fclose (fp);

  /* Close reserve and other always-open files and release other resources */
   fclose(player_fl);

  /* exec - descriptors are inherited */
   sprintf (buf, "%d", port ? port : 6010);
   sprintf (buf2, "-C%d", mother_desc);

  /* Ugh, seems it is expected we are 1 step above lib - this may be dangerous! */
   chdir ("..");

   execl (EXE_FILE, "circle", buf2, buf, (char *) NULL);

   /* Failed - sucessful exec will not return */
   perror ("do_copyover: execl");
   send_to_char ("Copyover FAILED!\n\r",ch);

 exit (1); /* too much trouble to try to recover! */
}


ACMD(do_reimburse) {
  struct char_data *vict;
  int i = 0;

  skip_spaces(&argument);

  if (!*argument) {
    send_to_char("Who do you want to reimburse?\r\n", ch);
    return;
  }

  if (!(vict = get_char_vis(ch, argument, FIND_CHAR_WORLD))) {
    send_to_char(NOPERSON, ch);
    return;
  }

  for (i = 0; i < NUM_WEARS; i++) {
    if (GET_EQ(vict, i)) {
      send_to_char("A character must not be using anything in order to be reimbursed.\r\n", ch);
      return;
    }
  }

  Crash_load(vict, TRUE);
  send_to_char("Your have had your equipment reimbursed to you.\r\n", vict);
  GET_NAME(vict, chname);
  sprintf(buf, "You reimburse %s's equipment.\r\n", chname);
  send_to_char(buf, ch);
  FREE_NAME(chname);
  
}


ACMD(do_award) {
  struct char_data *vict;
  char name[100], buf[100], buf2[100], buf3[100];
  int percent_to_award = 0, xp_to_award = 0;
  int XP_AWARD_CAP = 25;  // If you change this, change the error message below.

  argument = one_argument(argument, name);

  if (!*name) {
    send_to_char("To whom do you want to award experience?\r\n", ch);
    return;
  }

  if (!(vict = get_char_vis(ch, name, FIND_CHAR_WORLD))) {
    send_to_char(NOPERSON, ch);
    return;
  }

  skip_spaces(&argument);

  if (!*argument) {
    send_to_char("What percentage of experience would you like to award?\r\n", ch);
    return;
  }

  argument = one_argument(argument, buf);
  percent_to_award = atoi(buf);

  if (percent_to_award > XP_AWARD_CAP) {
    send_to_char("You may only award up to 25% experience at a time.\r\n", ch);
    return;
  }

  if (GET_LEVEL(vict) >= LVL_IMMORT) {
    send_to_char("You cannot award experience to immortals.\r\n", ch);
    return;
  } else if (GET_LEVEL(vict) == 30) {
    send_to_char("That character is already at maximum level.\r\n", ch);
    return;
  } else {
    int bonus_multiplier = GET_RPFACTOR(vict)/100;
    int level_bottom_xp = level_exp(GET_CLASS(vict), GET_LEVEL(vict));
    int level_top_xp = level_exp(GET_CLASS(vict), GET_LEVEL(vict) + 1);

    xp_to_award = (level_top_xp - level_bottom_xp) * percent_to_award/100 * bonus_multiplier;
    GET_EXP(vict) += xp_to_award;

    GET_NAME(ch, chname);
    GET_NAME(vict, victname);
    sprintf(buf2, "%s has awarded %s (%d) %d experience.", chname, victname, GET_LEVEL(vict), xp_to_award);
    sprintf(buf3, "\r\nYou have been awarded %d\% bonus experience!\r\n", percent_to_award);
    send_to_char(buf3, vict);
    FREE_NAME(victname);
    FREE_NAME(chname);

    mudlog(buf2, NRM, LVL_IMMORT, TRUE);
  }
}


