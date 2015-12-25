/************************************************************************
 *  OasisOLC - redit.c						v1.5	*
 *  Copyright 1996 Harvey Gilpin.					*
 *  Original author: Levork						*
 ************************************************************************/

#include "conf.h"
#include "sysdep.h"
#include "structs.h"
#include "comm.h"
#include "utils.h"
#include "db.h"
#include "boards.h"
#include "olc.h"
#include "dg_olc.h"

/* List each room saved, was used for debugging. */
#if 0
#define REDIT_LIST	1
#endif

/*------------------------------------------------------------------------*/

/*
 * External data structures.
 */
extern room_rnum top_of_world;
/* extern int top_of_world; */
extern struct room_data *world;
extern struct obj_data *obj_proto;
extern struct char_data *mob_proto;
extern const char *room_bits[];
extern const char *sector_types[];
extern const char *exit_bits[];
extern struct zone_data *zone_table;
extern sh_int r_mortal_start_room;
extern sh_int r_immort_start_room;
extern sh_int r_frozen_start_room;
extern sh_int mortal_start_room;
extern sh_int immort_start_room;
extern sh_int frozen_start_room;
extern int top_of_zone_table;
extern struct descriptor_data *descriptor_list;
extern const char *room_specs[];
extern void assign_room(room_rnum room, int spec);

/*------------------------------------------------------------------------*/

/*
 * Function Prototypes
 */
void redit_disp_extradesc_menu(struct descriptor_data *d);
void redit_disp_exit_menu(struct descriptor_data *d);
void redit_disp_exit_flag_menu(struct descriptor_data *d);
void redit_disp_flag_menu(struct descriptor_data *d);
void redit_disp_sector_menu(struct descriptor_data *d);
void redit_disp_menu(struct descriptor_data *d);
void redit_parse(struct descriptor_data *d, char *arg);
void redit_setup_new(struct descriptor_data *d);
void redit_setup_existing(struct descriptor_data *d, int real_num);
void redit_save_to_disk(int zone);
void redit_save_internally(struct descriptor_data *d);
void redit_disp_specs(struct descriptor_data *d);
void free_room(struct room_data *room);

/*------------------------------------------------------------------------*/

#define  W_EXIT(room, num) (world[(room)].dir_option[(num)])

/*------------------------------------------------------------------------*\
  Utils and exported functions.
\*------------------------------------------------------------------------*/

void redit_setup_new(struct descriptor_data *d)
{
  CREATE(OLC_ROOM(d), struct room_data, 1);

  OLC_ROOM(d)->name = str_dup("An unfinished room");
  OLC_ROOM(d)->description = str_dup("You are in an unfinished room.\r\n");
  redit_disp_menu(d);
  OLC_VAL(d) = 0;
}

/*------------------------------------------------------------------------*/

void redit_setup_existing(struct descriptor_data *d, int real_num)
{
  struct room_data *room;
  struct trig_proto_list *proto, *fproto;
  int counter;

  /*
   * Build a copy of the room for editing.
   */
  CREATE(room, struct room_data, 1);

  *room = world[real_num];
  /*
   * Allocate space for all strings.
   */
  room->name = str_dup(world[real_num].name ? world[real_num].name : "undefined");
  room->description = str_dup(world[real_num].description ?
			world[real_num].description : "undefined\r\n");
  /*
   * Exits - We allocate only if necessary.
   */
  for (counter = 0; counter < NUM_OF_DIRS; counter++) {
    if (world[real_num].dir_option[counter]) {
      CREATE(room->dir_option[counter], struct room_direction_data, 1);

      /*
       * Copy the numbers over.
       */
      *room->dir_option[counter] = *world[real_num].dir_option[counter];
      /*
       * Allocate the strings.
       */
      room->dir_option[counter]->general_description =
		(world[real_num].dir_option[counter]->general_description ?
		str_dup(world[real_num].dir_option[counter]->general_description)
		: NULL);
      room->dir_option[counter]->keyword =
		(world[real_num].dir_option[counter]->keyword ?
		str_dup(world[real_num].dir_option[counter]->keyword) : NULL);
    }
  }

  /*
   * Extra descriptions, if necessary.
   */
  if (world[real_num].ex_description) {
    struct extra_descr_data *this, *temp, *temp2;
    CREATE(temp, struct extra_descr_data, 1);

    room->ex_description = temp;
    for (this = world[real_num].ex_description; this; this = this->next) {
      temp->keyword = (this->keyword ? str_dup(this->keyword) : NULL);
      temp->description = (this->description ? str_dup(this->description) : NULL);
      if (this->next) {
	CREATE(temp2, struct extra_descr_data, 1);
	temp->next = temp2;
	temp = temp2;
      } else
	temp->next = NULL;
    }
  }

  if (SCRIPT(&world[real_num]))
    script_copy(room, &world[real_num], WLD_TRIGGER);
  proto = world[real_num].proto_script;
  while (proto) {
    CREATE(fproto, struct trig_proto_list, 1);
    fproto->vnum = proto->vnum;
    if (room->proto_script==NULL)
      room->proto_script = fproto;
    proto = proto->next;
    fproto = fproto->next; /* NULL */
  }

  /*
   * Attach copy of room to player's descriptor.
   */
  OLC_ROOM(d) = room;
  OLC_VAL(d) = 0;
  OLC_ITEM_TYPE(d) = WLD_TRIGGER;
  dg_olc_script_copy(d);
  redit_disp_menu(d);
}

/*------------------------------------------------------------------------*/

#define ZCMD (zone_table[zone].cmd[cmd_no])

void redit_save_internally(struct descriptor_data *d)
{
  int i, j, room_num, found = 0, zone, cmd_no;
  struct room_data *new_world;
  struct char_data *temp_ch;
  struct obj_data *temp_obj;
  struct descriptor_data *dsc;

  room_num = real_room(OLC_NUM(d));

  /*
   * Room exists: move contents over then free and replace it.
   */
  if (room_num > 0) {
    OLC_ROOM(d)->contents = world[room_num].contents;
    OLC_ROOM(d)->people = world[room_num].people;
    free_room(world + room_num);
    world[room_num] = *OLC_ROOM(d);
    world[room_num].proto_script = OLC_SCRIPT(d);

  } else {			/* Room doesn't exist, hafta add it. */
    CREATE(new_world, struct room_data, top_of_world + 2);

    /*
     * Count through world tables.
     */
    for (i = 0; i <= top_of_world; i++) {
      if (!found) {
	/*
	 * Is this the place? 
	 */
	if (world[i].number > OLC_NUM(d)) {
	  found = TRUE;
	  new_world[i] = *(OLC_ROOM(d));
	  new_world[i].number = OLC_NUM(d);
	  new_world[i].func = NULL;
          new_world[i].proto_script = OLC_SCRIPT(d);
	  room_num = i;

	  /*
	   * Copy from world to new_world + 1.
	   */
	  new_world[i + 1] = world[i];

	  /*
	   * People in this room must have their numbers moved up one.
	   */
	  for(temp_ch=world[i].people;temp_ch; temp_ch=temp_ch->next_in_room) {
	    if (temp_ch->in_room != NOWHERE) {
	      temp_ch->in_room = i + 1;
	    }
	  }

	  /*
	   * Move objects up one room.
	   */
	  for(temp_obj=world[i].contents; temp_obj;
	      temp_obj=temp_obj->next_content) {
	    if (temp_obj->in_room != NOWHERE) {
	      temp_obj->in_room = i + 1;
	    }
	  }

	} else {   /* Not yet placed, copy straight over. */
	  new_world[i] = world[i];
	}

      } else {		/* Already been found. */
	/*
	 * People in this room must have their in_rooms moved.
	 */
	for(temp_ch=world[i].people; temp_ch; temp_ch=temp_ch->next_in_room) {
	  if (temp_ch->in_room != NOWHERE) {
	    temp_ch->in_room = i + 1;
	  }
	}

	/*
	 * Move objects too.
	 */
	for(temp_obj=world[i].contents; temp_obj;
	    temp_obj=temp_obj->next_content) {
	  if (temp_obj->in_room != -1) {
	    temp_obj->in_room = i + 1;
	  }
	}

	new_world[i + 1] = world[i];
      }
    }


    if (!found) {	/* Still not found, insert at top of table. */
      new_world[i] = *(OLC_ROOM(d));
      new_world[i].number = OLC_NUM(d);
      new_world[i].func = NULL;
      new_world[i].proto_script = OLC_SCRIPT(d);
      room_num = i;
    }

    /*
     * Copy world table over to new one.
     */
    free(world);
    world = new_world;
    top_of_world++;

    /*
     * Update zone table.
     */
    for (zone = 0; zone <= top_of_zone_table; zone++) {
      for (cmd_no = 0; ZCMD.command != 'S'; cmd_no++) {
	switch (ZCMD.command) {
	case 'M':
	case 'O':
	  if (ZCMD.arg3 >= room_num)
	    ZCMD.arg3++;
	  break;
	case 'D':
	case 'R':
	  if (ZCMD.arg1 >= room_num)
	    ZCMD.arg1++;
	case 'G':
	case 'P':
	case 'E':
	case '*':
	  break;
	default:
	  mudlog("SYSERR: OLC: redit_save_internally: Unknown comand", BRF, LVL_BUILDER, TRUE);
	}
      }
    }

    /*
     * Update load rooms, to fix creeping load room problem.
     */
    if (room_num <= r_mortal_start_room) {
      r_mortal_start_room++;
    }
    if (room_num <= r_immort_start_room) {
      r_immort_start_room++;
    }
    if (room_num <= r_frozen_start_room) {
      r_frozen_start_room++;
    }

    /*
     * Update world exits.
     */
    for (i = 0; i < top_of_world + 1; i++) {
      for (j = 0; j < NUM_OF_DIRS; j++) {
	if (W_EXIT(i, j)) {
	  if (W_EXIT(i, j)->to_room >= room_num) {
	    W_EXIT(i, j)->to_room++;
	  }
	}
      }
    }

    /*
     * Update any rooms being edited.
     */
    for (dsc = descriptor_list; dsc; dsc = dsc->next) {
      if (STATE(dsc) == CON_REDIT) {
	for (j = 0; j < NUM_OF_DIRS; j++) {
	  if (OLC_ROOM(dsc)->dir_option[j]) {
	    if (OLC_ROOM(dsc)->dir_option[j]->to_room >= room_num) {
	      OLC_ROOM(dsc)->dir_option[j]->to_room++;
	    }
	  }
	}
      }
    }

  }

  assign_triggers(&world[room_num], WLD_TRIGGER);
  assign_room(room_num, world[room_num].specnum);
  olc_add_to_save_list(zone_table[OLC_ZNUM(d)].number, OLC_SAVE_ROOM);

}

/*------------------------------------------------------------------------*/

void redit_save_to_disk(int zone_num)
{
  int counter, counter2, realcounter;
  FILE *fp;
  struct room_data *room;
  struct extra_descr_data *ex_desc;

  if (zone_num < 0 || zone_num > top_of_zone_table) {
    log("SYSERR: redit_save_to_disk: Invalid real zone passed!");
    return;
  }

  sprintf(buf, "%s/%d.new", WLD_PREFIX, zone_table[zone_num].number);
  if (!(fp = fopen(buf, "w+"))) {
    mudlog("SYSERR: OLC: Cannot open room file!", BRF, LVL_BUILDER, TRUE);
    return;
  }
  for (counter = zone_table[zone_num].number * 100;
       counter <= zone_table[zone_num].top; counter++) {
    if ((realcounter = real_room(counter)) >= 0) {
      room = (world + realcounter);

#if defined(REDIT_LIST)
      sprintf(buf1, "OLC: Saving room %d.", room->number);
      log(buf1);
#endif

      /*
       * Remove the '\r\n' sequences from description.
       */
      strcpy(buf1, room->description ? room->description : "Empty");
      strip_string(buf1);

      /*
       * Forget making a buffer, lets just write the thing now.
         * of course, that makes this look horrid when we add stuff.  Soli
       */
      fprintf(fp, "#%d\n%s~\n%s~\n%d %d %d %d %d\n", counter,
	      room->name ? room->name : "undefined", buf1,
	      zone_table[room->zone].number,
              room->room_flags, room->sector_type, room->pulse_dam,
              room->specnum);

      /*
       * Handle exits.
       */
      for (counter2 = 0; counter2 < NUM_OF_DIRS; counter2++) {
	if (room->dir_option[counter2]) {
	  int temp_door_flag;

	  /*
	   * Again, strip out the garbage.
	   */
	  if (room->dir_option[counter2]->general_description) {
	    strcpy(buf1, room->dir_option[counter2]->general_description);
	    strip_string(buf1);
	  } else
	    *buf1 = 0;

	  /*
	   * Figure out door flag. 
	   */
	  if (IS_SET(room->dir_option[counter2]->exit_info, EX_ISDOOR)) {
#if 0
            temp_door_flag = room->dir_option[counter2]->exit_info;
#endif
#if 1
	    if (IS_SET(room->dir_option[counter2]->exit_info, EX_PICKPROOF)) {
               if (IS_SET(room->dir_option[counter2]->exit_info, EX_HIDDEN)) 
	      temp_door_flag = 5;
               else 
               temp_door_flag = 2;
            }
            else if (IS_SET(room->dir_option[counter2]->exit_info, EX_HIDDEN))
               temp_door_flag = 4;
	    else
	      temp_door_flag = 1;
#endif
	  }
      else if (IS_SET(room->dir_option[counter2]->exit_info, EX_HIDDEN) && !IS_SET(room->dir_option[counter2]->exit_info, EX_ISDOOR)) 
           temp_door_flag = 3;
         else
	    temp_door_flag = 0;

	  /*
	   * Check for keywords.
	   */
	  if (room->dir_option[counter2]->keyword)
	    strcpy(buf2, room->dir_option[counter2]->keyword);
	  else
	    *buf2 = '\0';

	  /*
	   * Ok, now wrote output to file.
	   */
	  fprintf(fp, "D%d\n%s~\n%s~\n%d %d %d\n", counter2, buf1, buf2,
		  temp_door_flag, room->dir_option[counter2]->key,
		  room->dir_option[counter2]->to_room != -1 ?
		  world[room->dir_option[counter2]->to_room].number : -1);
	}
      }
      /*
       * Home straight, just deal with extra descriptions.
       */
      if (room->ex_description) {
	for (ex_desc = room->ex_description; ex_desc; ex_desc = ex_desc->next) {
	  strcpy(buf1, ex_desc->description);
	  strip_string(buf1);
	  fprintf(fp, "E\n%s~\n%s~\n", ex_desc->keyword, buf1);
	}
      }
      fprintf(fp, "S\n");
      script_save_to_disk(fp, room, WLD_TRIGGER);
    }
  }
  /*
   * Write final line and close.
   */
  fprintf(fp, "$~\n");
  fclose(fp);
  sprintf(buf2, "%s/%d.wld", WLD_PREFIX, zone_table[zone_num].number);
  /*
   * We're fubar'd if we crash between the two lines below.
   */
  remove(buf2);
  rename(buf, buf2);

  olc_remove_from_save_list(zone_table[zone_num].number, OLC_SAVE_ROOM);
}

/*------------------------------------------------------------------------*/

void free_room(struct room_data *room)
{
  int i;
  struct extra_descr_data *this, *next;

  if (room->name) {
    free(room->name);
  }

  if (room->description) {
    free(room->description);
  }

  /*
   * Free exits.
   */
  for (i=0; i<NUM_OF_DIRS; i++) {
    if (room->dir_option[i]) {
      if (room->dir_option[i]->general_description) {
	free(room->dir_option[i]->general_description);
      }

      if (room->dir_option[i]->keyword) {
	free(room->dir_option[i]->keyword);
      }
    }

    free(room->dir_option[i]);
  }

  /*
   * Free extra descriptions.
   */
  for (this=room->ex_description; this; this=next) {
    next = this->next;

    if (this->keyword) {
      free(this->keyword);
    }

    if (this->description) {
      free(this->description);
    }

    free(this);
  }
}

/**************************************************************************
 Menu functions 
 **************************************************************************/

/*
 * For extra descriptions.
 */
void redit_disp_extradesc_menu(struct descriptor_data *d)
{
  struct extra_descr_data *extra_desc = OLC_DESC(d);

  sprintf(buf,
#if defined(CLEAR_SCREEN)
	  "[H[J"
#endif
	  "%s1%s) Keyword: %s%s\r\n"
	  "%s2%s) Description:\r\n%s%s\r\n"
	  "%s3%s) Goto next description: ",

	  grn, nrm, yel, extra_desc->keyword ? extra_desc->keyword : "<NONE>",
	  grn, nrm, yel, extra_desc->description ? extra_desc->description : "<NONE>",
	  grn, nrm
	  );

  strcat(buf, !extra_desc->next ? "<NOT SET>\r\n" : "Set.\r\n");
  strcat(buf, "Enter choice (0 to quit) : ");
  send_to_char(buf, d->character);
  OLC_MODE(d) = REDIT_EXTRADESC_MENU;
}

/*
 * For exits.
 */
void redit_disp_exit_menu(struct descriptor_data *d)
{
  /*
   * if exit doesn't exist, alloc/create it 
   */
  if (!OLC_EXIT(d))
    CREATE(OLC_EXIT(d), struct room_direction_data, 1);

  /*
   * Weird door handling! 
   */
  if (IS_SET(OLC_EXIT(d)->exit_info, EX_ISDOOR) && !IS_SET(OLC_EXIT(d)->exit_info, EX_HIDDEN)) {
    if (IS_SET(OLC_EXIT(d)->exit_info, EX_PICKPROOF))
      strcpy(buf2, "Pickproof");
    else
      strcpy(buf2, "Is a door");
  } else
    if (IS_SET(OLC_EXIT(d)->exit_info, EX_HIDDEN) && 
	!IS_SET(OLC_EXIT(d)->exit_info, EX_ISDOOR))
	strcpy(buf2, "Hidden exit");
    else if (IS_SET(OLC_EXIT(d)->exit_info, EX_ISDOOR) && 
	!IS_SET(OLC_EXIT(d)->exit_info, EX_PICKPROOF))
	strcpy(buf2, "Hidden door");
    else if (IS_SET(OLC_EXIT(d)->exit_info, EX_PICKPROOF))
	strcpy(buf2, "Hidden pickproof door");
    else
	strcpy(buf2, "No door");

  get_char_cols(d->character);
  sprintf(buf,
#if defined(CLEAR_SCREEN)
	  "[H[J"
#endif
	  "%s1%s) Exit to     : %s%d\r\n"
	  "%s2%s) Description :-\r\n%s%s\r\n"
	  "%s3%s) Door name   : %s%s\r\n"
	  "%s4%s) Key         : %s%d\r\n"
	  "%s5%s) Door flags  : %s%s\r\n"
	  "%s6%s) Purge exit.\r\n"
	  "Enter choice, 0 to quit : ",

	  grn, nrm, cyn, OLC_EXIT(d)->to_room != -1 ? world[OLC_EXIT(d)->to_room].number : -1,
	  grn, nrm, yel, OLC_EXIT(d)->general_description ? OLC_EXIT(d)->general_description : "<NONE>",
	  grn, nrm, yel, OLC_EXIT(d)->keyword ? OLC_EXIT(d)->keyword : "<NONE>",
	  grn, nrm, cyn, OLC_EXIT(d)->key,
	  grn, nrm, cyn, buf2, grn, nrm
	  );

  send_to_char(buf, d->character);
  OLC_MODE(d) = REDIT_EXIT_MENU;
}

/*
 * For exit flags.
 */
void redit_disp_exit_flag_menu(struct descriptor_data *d)
{
  get_char_cols(d->character);
   sprintf(buf, "%s0%s) No door\r\n"
	  "%s1%s) Closeable door\r\n"
	  "%s2%s) Pickproof\r\n"
	  "%s3%s) Hidden exit\r\n"
	  "%s4%s) Hidden door\r\n"
	  "%s5%s) Hidden pickproof door\r\n"
	  "Enter choice : ", grn, nrm, grn, nrm, grn, nrm, grn, nrm, grn, nrm, grn, nrm);

  send_to_char(buf, d->character);
}

/*
 * For room flags.
 */
void redit_disp_flag_menu(struct descriptor_data *d)
{
  int counter, columns = 0;

  get_char_cols(d->character);
#if defined(CLEAR_SCREEN)
  send_to_char("[H[J", d->character);
#endif
  for (counter = 0; counter < NUM_ROOM_FLAGS; counter++) {
    sprintf(buf, "%s%2d%s) %-20.20s %s", grn, counter + 1, nrm,
		room_bits[counter], !(++columns % 2) ? "\r\n" : "");
    send_to_char(buf, d->character);
  }
  sprintbit(OLC_ROOM(d)->room_flags, room_bits, buf1);
  sprintf(buf, "\r\nRoom flags: %s%s%s\r\n"
	  "Enter room flags, 0 to quit : ", cyn, buf1, nrm);
  send_to_char(buf, d->character);
  OLC_MODE(d) = REDIT_FLAGS;
}

/*
 * For sector type.
 */
void redit_disp_sector_menu(struct descriptor_data *d)
{
  int counter, columns = 0;

#if defined(CLEAR_SCREEN)
  send_to_char("[H[J", d->character);
#endif
  for (counter = 0; counter < NUM_ROOM_SECTORS; counter++) {
    sprintf(buf, "%s%2d%s) %-20.20s %s", grn, counter, nrm,
		sector_types[counter], !(++columns % 2) ? "\r\n" : "");
    send_to_char(buf, d->character);
  }
  send_to_char("\r\nEnter sector type : ", d->character);
  OLC_MODE(d) = REDIT_SECTOR;
}

void redit_disp_specs(struct descriptor_data *d)
{
  int counter;

  for (counter = 0;counter <= NUM_ROOM_SPECS; counter++)
  {
    sprintf(buf, "%s%2d%s) %s\r\n", grn, counter, nrm, room_specs[counter]);
    send_to_char(buf, d->character);
  }
  send_to_char("\r\nEnter room spec: ", d->character);
}

/*
 * The main menu.
 */
void redit_disp_menu(struct descriptor_data *d)
{
  struct room_data *room;
  char spec[15];

  get_char_cols(d->character);
  room = OLC_ROOM(d);

  sprintbit((long)room->room_flags, room_bits, buf1);
  sprinttype(room->sector_type, sector_types, buf2);
  sprinttype(room->specnum, room_specs, spec);
  sprintf(buf,
#if defined(CLEAR_SCREEN)
	  "[H[J"
#endif
	  "-- Room number : [%s%d%s]  	Room zone: [%s%d%s]\r\n"
	  "%s1%s) Name        : %s%s\r\n"
	  "%s2%s) Description :\r\n%s%s"
	  "%s3%s) Room flags  : %s%s\r\n"
          "%s4%s) Sector type : %s%-15s   %sP%s) Pulse Dam : %s%d\r\n"
	  "%s5%s) Exit north  : %s%d\r\n"
	  "%s6%s) Exit east   : %s%d\r\n"
	  "%s7%s) Exit south  : %s%d\r\n"
	  "%s8%s) Exit west   : %s%d\r\n"
	  "%s9%s) Exit up     : %s%d\r\n"
	  "%sA%s) Exit down   : %s%d\r\n"
    	  "%sC%s) Exit NW     : %s%d\r\n"
    	  "%sD%s) Exit NE     : %s%d\r\n"	
	  "%sE%s) Exit SW     : %s%d\r\n"
          "%sF%s) Exit SE     : %s%d\r\n"	 
	  "%sB%s) Extra descriptions menu\r\n"
          "%sR%s) Spec        : %s%s\r\n"
          "%sS%s) Script      : %s%s\r\n"
	  "%sQ%s) Quit\r\n"
	  "Enter choice : ",

	  cyn, OLC_NUM(d), nrm,
	  cyn, zone_table[OLC_ZNUM(d)].number, nrm,
	  grn, nrm, yel, room->name,
	  grn, nrm, yel, room->description,
	  grn, nrm, cyn, buf1,
	  grn, nrm, cyn, buf2,
          grn, nrm, cyn, room->pulse_dam,
	  grn, nrm, cyn,
	  room->dir_option[NORTH] && room->dir_option[NORTH]->to_room != -1 ?
	  world[room->dir_option[NORTH]->to_room].number : -1,
	  grn, nrm, cyn,
	  room->dir_option[EAST] && room->dir_option[EAST]->to_room != -1 ?
	  world[room->dir_option[EAST]->to_room].number : -1,
	  grn, nrm, cyn,
	  room->dir_option[SOUTH] && room->dir_option[SOUTH]->to_room != -1 ?
	  world[room->dir_option[SOUTH]->to_room].number : -1,
	  grn, nrm, cyn,
	  room->dir_option[WEST] && room->dir_option[WEST]->to_room != -1 ?
	  world[room->dir_option[WEST]->to_room].number : -1,
	  grn, nrm, cyn,
	  room->dir_option[UP] && room->dir_option[UP]->to_room != -1 ? 
	  world[room->dir_option[UP]->to_room].number : -1,
	  grn, nrm, cyn,
	  room->dir_option[DOWN] && room->dir_option[DOWN]->to_room != -1 ?
	  world[room->dir_option[DOWN]->to_room].number : -1,
	  grn, nrm, cyn,
	  room->dir_option[NORTHWEST] && room->dir_option[NORTHWEST]->to_room != -1 ?
	  world[room->dir_option[NORTHWEST]->to_room].number : -1,
	  grn, nrm, cyn,
	  room->dir_option[NORTHEAST] && room->dir_option[NORTHEAST]->to_room != -1 ?
	  world[room->dir_option[NORTHEAST]->to_room].number : -1,
	  grn, nrm, cyn,
	  room->dir_option[SOUTHWEST] && room->dir_option[SOUTHWEST]->to_room != -1 ?
	  world[room->dir_option[SOUTHWEST]->to_room].number : -1,
	  grn, nrm, cyn,
	  room->dir_option[SOUTHEAST] && room->dir_option[SOUTHEAST]->to_room != -1 ?
	  world[room->dir_option[SOUTHEAST]->to_room].number : -1,
	  grn, nrm, 
          grn, nrm, cyn, spec,
          grn, nrm, cyn, room->proto_script?"Set.":"Not Set.",
          grn, nrm
	  );
  send_to_char(buf, d->character);

  OLC_MODE(d) = REDIT_MAIN_MENU;
}

/**************************************************************************
  The main loop
 **************************************************************************/

void redit_parse(struct descriptor_data *d, char *arg)
{
  int number;

  switch (OLC_MODE(d)) {
  case REDIT_CONFIRM_SAVESTRING:
    switch (*arg) {
    case 'y':
    case 'Y':
      redit_save_internally(d);
      GET_NAME(d->character, chname);
      sprintf(buf, "OLC: %s edits room %d.", chname, OLC_NUM(d));
      FREE_NAME(chname);
      mudlog(buf, CMP, MAX(LVL_BUILDER, GET_INVIS_LEV(d->character)), TRUE);
      /*
       * Do NOT free strings! Just the room structure. 
       */
      cleanup_olc(d, CLEANUP_STRUCTS);
      send_to_char("Room saved to memory.\r\n", d->character);
      break;
    case 'n':
    case 'N':
      /*
       * Free everything up, including strings, etc.
       */
      cleanup_olc(d, CLEANUP_ALL);
      break;
    default:
      send_to_char("Invalid choice!\r\nDo you wish to save this room internally? : ", d->character);
      break;
    }

    return;

  case REDIT_MAIN_MENU:
    switch (*arg) {
    case 'q':
    case 'Q':
      if (OLC_VAL(d)) { /* Something has been modified. */
	send_to_char("Do you wish to save this room internally? : ", d->character);
	OLC_MODE(d) = REDIT_CONFIRM_SAVESTRING;
      } else
	cleanup_olc(d, CLEANUP_ALL);
      return;
    case '1':
      send_to_char("Enter room name:-\r\n] ", d->character);
      OLC_MODE(d) = REDIT_NAME;
      break;

    case '2':
      OLC_MODE(d) = REDIT_DESC;

#if defined(CLEAR_SCREEN)
      SEND_TO_Q("\x1B[H\x1B[J", d);
#endif

      SEND_TO_Q("Enter room description: (/s saves /h for help)\r\n\r\n", d);
      d->backstr = NULL;

      if (OLC_ROOM(d)->description) {
	SEND_TO_Q(OLC_ROOM(d)->description, d);
	d->backstr = str_dup(OLC_ROOM(d)->description);
      }

      d->str = &OLC_ROOM(d)->description;
      d->max_str = MAX_ROOM_DESC;
      d->mail_to = 0;
      OLC_VAL(d) = 1;
      break;

    case '3':
      redit_disp_flag_menu(d);
      break;
    case '4':
      redit_disp_sector_menu(d);
      break;
    case '5':
      OLC_VAL(d) = NORTH;
      redit_disp_exit_menu(d);
      break;
    case '6':
      OLC_VAL(d) = EAST;
      redit_disp_exit_menu(d);
      break;
    case '7':
      OLC_VAL(d) = SOUTH;
      redit_disp_exit_menu(d);
      break;
    case '8':
      OLC_VAL(d) = WEST;
      redit_disp_exit_menu(d);
      break;
    case '9':
      OLC_VAL(d) = UP;
      redit_disp_exit_menu(d);
      break;
    case 'a':
    case 'A':
      OLC_VAL(d) = DOWN;
      redit_disp_exit_menu(d);
      break;
    case 'c':
    case 'C':
      OLC_VAL(d) = NORTHWEST;
      redit_disp_exit_menu(d);
      break;
    case 'd':
    case 'D':
      OLC_VAL(d) = NORTHEAST;
      redit_disp_exit_menu(d);
      break; 
    case 'e':
    case 'E':
      OLC_VAL(d) = SOUTHWEST;
      redit_disp_exit_menu(d);
      break;
    case 'f':
    case 'F':
      OLC_VAL(d) = SOUTHEAST;
      redit_disp_exit_menu(d);
      break;       
    case 'b':
    case 'B':
      /*
       * If the extra description doesn't exist.
       */
      if (!OLC_ROOM(d)->ex_description) {
	CREATE(OLC_ROOM(d)->ex_description, struct extra_descr_data, 1);
	OLC_ROOM(d)->ex_description->next = NULL;
      }
      OLC_DESC(d) = OLC_ROOM(d)->ex_description;
      redit_disp_extradesc_menu(d);
      break;
    case 's':
    case 'S':
      OLC_SCRIPT_EDIT_MODE(d) = SCRIPT_MAIN_MENU;
      dg_script_menu(d);
      return;
    case 'P':
    case 'p':
      send_to_char("Enter new value : ", d->character);
      OLC_MODE(d) = REDIT_PULSE_DAM;
      break;
    case 'R':
    case 'r':
      redit_disp_specs(d);
      OLC_MODE(d) = REDIT_SPECS;
      break;
    default:
      send_to_char("Invalid choice!", d->character);
      redit_disp_menu(d);
      break;
    }
    return;

  case OLC_SCRIPT_EDIT:
    if (dg_script_edit_parse(d, arg)) return;
    break;

  case REDIT_NAME:
    if (OLC_ROOM(d)->name)
      free(OLC_ROOM(d)->name);
    if (strlen(arg) > MAX_ROOM_NAME)
      arg[MAX_ROOM_NAME - 1] = '\0';
    OLC_ROOM(d)->name = str_dup((arg && *arg) ? arg : "undefined");
    break;


  case REDIT_DESC:
    /*
     * We will NEVER get here, we hope.
     */
    mudlog("SYSERR: Reached REDIT_DESC case in parse_redit", BRF, LVL_BUILDER, TRUE);
    break;

  case REDIT_FLAGS:
    number = atoi(arg);
    if ((number < 0) || (number > NUM_ROOM_FLAGS)) {
      send_to_char("That is not a valid choice!\r\n", d->character);
      redit_disp_flag_menu(d);
    } else if (number == 0)
	break;
    else {
      /*
       * Toggle the bit.
       */
      TOGGLE_BIT(OLC_ROOM(d)->room_flags, 1 << (number - 1));
      redit_disp_flag_menu(d);
    }
    return;

  case REDIT_SECTOR:
    number = atoi(arg);
    if (number < 0 || number >= NUM_ROOM_SECTORS) {
      send_to_char("Invalid choice!", d->character);
      redit_disp_sector_menu(d);
      return;
    } else
      OLC_ROOM(d)->sector_type = number;
    break;

  case REDIT_SPECS:
    OLC_ROOM(d)->specnum = MIN(NUM_ROOM_SPECS, MAX(0, atoi(arg)));
    break;

  case REDIT_EXIT_MENU:
    switch (*arg) {
    case '0':
      break;
    case '1':
      OLC_MODE(d) = REDIT_EXIT_NUMBER;
      send_to_char("Exit to room number : ", d->character);
      return;
    case '2':
      OLC_MODE(d) = REDIT_EXIT_DESCRIPTION;
      SEND_TO_Q("Enter exit description: (/s saves /h for help)\r\n\r\n", d);
      d->backstr = NULL;
      if (OLC_EXIT(d)->general_description) {
	SEND_TO_Q(OLC_EXIT(d)->general_description, d);
	d->backstr = str_dup(OLC_EXIT(d)->general_description);
      }
      d->str = &OLC_EXIT(d)->general_description;
      d->max_str = MAX_EXIT_DESC;
      d->mail_to = 0;
      return;
    case '3':
      OLC_MODE(d) = REDIT_EXIT_KEYWORD;
      send_to_char("Enter keywords : ", d->character);
      return;
    case '4':
      OLC_MODE(d) = REDIT_EXIT_KEY;
      send_to_char("Enter key number : ", d->character);
      return;
    case '5':
      redit_disp_exit_flag_menu(d);
      OLC_MODE(d) = REDIT_EXIT_DOORFLAGS;
      return;
    case '6':
      /*
       * Delete an exit.
       */
      if (OLC_EXIT(d)->keyword)
	free(OLC_EXIT(d)->keyword);
      if (OLC_EXIT(d)->general_description)
	free(OLC_EXIT(d)->general_description);
      if (OLC_EXIT(d))
	free(OLC_EXIT(d));
      OLC_EXIT(d) = NULL;
      break;
    default:
      send_to_char("Try again : ", d->character);
      return;
    }
    break;

  case REDIT_EXIT_NUMBER:
    if ((number = atoi(arg)) != -1)
      if ((number = real_room(number)) < 0) {
	send_to_char("That room does not exist, try again : ", d->character);
	return;
      }
    OLC_EXIT(d)->to_room = number;
    redit_disp_exit_menu(d);
    return;

  case REDIT_EXIT_DESCRIPTION:
    /*
     * We should NEVER get here, hopefully.
     */
    mudlog("SYSERR: Reached REDIT_EXIT_DESC case in parse_redit", BRF, LVL_BUILDER, TRUE);
    break;

  case REDIT_EXIT_KEYWORD:
    if (OLC_EXIT(d)->keyword)
      free(OLC_EXIT(d)->keyword);
    OLC_EXIT(d)->keyword = ((arg && *arg) ? str_dup(arg) : NULL);
    redit_disp_exit_menu(d);
    return;

  case REDIT_EXIT_KEY:
    OLC_EXIT(d)->key = atoi(arg);
    redit_disp_exit_menu(d);
    return;

  case REDIT_EXIT_DOORFLAGS:
    number = atoi(arg);
    if ((number < 0) || (number > 4)) {
      send_to_char("That's not a valid choice!\r\n", d->character);
      redit_disp_exit_flag_menu(d);
    } else {
      switch (number)
      {
        case 0:
          OLC_EXIT(d)->exit_info = 0;
          break;
        case 1:
          OLC_EXIT(d)->exit_info = EX_ISDOOR;
          break;
        case 2:
          OLC_EXIT(d)->exit_info = EX_ISDOOR | EX_PICKPROOF;
          break;
        case 3:
          OLC_EXIT(d)->exit_info = EX_HIDDEN;
          break;
        case 4:
          OLC_EXIT(d)->exit_info = EX_ISDOOR | EX_HIDDEN;
          break;
        case 5:
          OLC_EXIT(d)->exit_info = EX_ISDOOR | EX_HIDDEN | EX_PICKPROOF;
          break;
        default:
          OLC_EXIT(d)->exit_info = 0;
          break;
      }
      redit_disp_exit_menu(d);
    }
    return;

  case REDIT_EXTRADESC_KEY:
    OLC_DESC(d)->keyword = ((arg && *arg) ? str_dup(arg) : NULL);
    redit_disp_extradesc_menu(d);
    return;

  case REDIT_EXTRADESC_MENU:
    switch ((number = atoi(arg))) {
    case 0:
      {
	/*
	 * If something got left out, delete the extra description
	 * when backing out to the menu.
	 */
	if (!OLC_DESC(d)->keyword || !OLC_DESC(d)->description) {
	  struct extra_descr_data **tmp_desc;

	  if (OLC_DESC(d)->keyword)
	    free(OLC_DESC(d)->keyword);
	  if (OLC_DESC(d)->description)
	    free(OLC_DESC(d)->description);

	  /*
	   * Clean up pointers.
	   */
	  for (tmp_desc = &(OLC_ROOM(d)->ex_description); *tmp_desc;
	       tmp_desc = &((*tmp_desc)->next))
	    if (*tmp_desc == OLC_DESC(d)) {
	      *tmp_desc = NULL;
	      break;
	    }
	  free(OLC_DESC(d));
	}
      }
      break;
    case 1:
      OLC_MODE(d) = REDIT_EXTRADESC_KEY;
      send_to_char("Enter keywords, separated by spaces : ", d->character);
      return;
    case 2:
      OLC_MODE(d) = REDIT_EXTRADESC_DESCRIPTION;
      SEND_TO_Q("Enter extra description: (/s saves /h for help)\r\n\r\n", d);
      d->backstr = NULL;
      if (OLC_DESC(d)->description) {
	SEND_TO_Q(OLC_DESC(d)->description, d);
	d->backstr = str_dup(OLC_DESC(d)->description);
      }
      d->str = &OLC_DESC(d)->description;
      d->max_str = MAX_MESSAGE_LENGTH;
      d->mail_to = 0;
      return;

    case 3:
      if (!OLC_DESC(d)->keyword || !OLC_DESC(d)->description) {
	send_to_char("You can't edit the next extra desc without completing this one.\r\n", d->character);
	redit_disp_extradesc_menu(d);
      } else {
	struct extra_descr_data *new_extra;

	if (OLC_DESC(d)->next)
	  OLC_DESC(d) = OLC_DESC(d)->next;
	else {
	  /*
	   * Make new extra description and attach at end.
	   */
	  CREATE(new_extra, struct extra_descr_data, 1);
	  OLC_DESC(d)->next = new_extra;
	  OLC_DESC(d) = new_extra;
	}
	redit_disp_extradesc_menu(d);
      }
      return;
    }
    break;

  case REDIT_PULSE_DAM:
    OLC_ROOM(d)->pulse_dam = atoi(arg);
    break;

  default:
    /*
     * We should never get here.
     */
    mudlog("SYSERR: Reached default case in parse_redit", BRF, LVL_BUILDER, TRUE);
    break;
  }
  /*
   * If we get this far, something has been changed.
   */
  OLC_VAL(d) = 1;
  redit_disp_menu(d);
}
