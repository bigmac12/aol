/* ************************************************************************
*   File: objsave.c                                     Part of CircleMUD *
*  Usage: loading/saving player objects for rent and crash-save           *
*                                                                         *
*  All rights reserved.  See license.doc for complete information.        *
*                                                                         *
*  Copyright (C) 1993, 94 by the Trustees of the Johns Hopkins University *
*  CircleMUD is based on DikuMUD, Copyright (C) 1990, 1991.               *
************************************************************************ */

/* now with auto-equip - BK */

#include "conf.h"
#include "sysdep.h"


#include "structs.h"
#include "comm.h"
#include "handler.h"
#include "db.h"
#include "interpreter.h"
#include "utils.h"
#include "spells.h"

/* these factors should be unique integers */
#define RENT_FACTOR 	1
#define CRYO_FACTOR 	4

#define LOC_INVENTORY   0
#define MAX_BAG_ROWS    5

extern struct room_data *world;
extern struct index_data *mob_index;
extern struct index_data *obj_index;
extern struct descriptor_data *descriptor_list;
extern struct player_index_element *player_table;
extern int top_of_p_table;
extern int rent_file_timeout, crash_file_timeout;
extern int free_rent;
extern int min_rent_cost;
extern int max_obj_save;

/* Extern functions */
ACMD(do_action);
SPECIAL(receptionist);
SPECIAL(cryogenicist);
int invalid_class(struct char_data *ch, struct obj_data *obj);
void affect_remove_nonperm(struct char_data * ch, int type);
void mag_checkeq(struct char_data *ch, int item0, int item1, int item2);
void affect_from_char_II(struct char_data * ch, int skill, int type, int action)
;

#define VNUM_FREE_RENT_TOKEN 1290

struct obj_data *Obj_from_store_to(struct obj_file_elem object, int *locate)
{
  struct obj_data *obj;
  int j;

  if (real_object(object.item_number) > -1) {
    obj = read_object(object.item_number, VIRTUAL);
    *locate = (int) object.locate;
    GET_OBJ_VAL(obj, 0) = object.value[0];
    GET_OBJ_VAL(obj, 1) = object.value[1];
    GET_OBJ_VAL(obj, 2) = object.value[2];
    GET_OBJ_VAL(obj, 3) = object.value[3];
    GET_OBJ_EXTRA(obj) = object.extra_flags;
    GET_OBJ_WEIGHT(obj) = object.weight;
    GET_OBJ_TIMER(obj) = object.timer;
    obj->obj_flags.bitvector = object.bitvector;

    for (j = 0; j < MAX_OBJ_AFFECT; j++)
      obj->affected[j] = object.affected[j];

    return obj;
  } else
    return NULL;
}



/* this function used in house.c */
struct obj_data *Obj_from_store(struct obj_file_elem object)
{
  int locate;

  return Obj_from_store_to(object, &locate);
}



int Obj_to_store_from(struct obj_data * obj, FILE * fl, int locate)
{
  int j;
  struct obj_file_elem object;

  object.item_number = GET_OBJ_VNUM(obj);
  object.locate = (sh_int) locate; /* where worn or inventory? */
  object.value[0] = GET_OBJ_VAL(obj, 0);
  object.value[1] = GET_OBJ_VAL(obj, 1);
  object.value[2] = GET_OBJ_VAL(obj, 2);
  object.value[3] = GET_OBJ_VAL(obj, 3);
  object.extra_flags = GET_OBJ_EXTRA(obj);
  object.weight = GET_OBJ_WEIGHT(obj);
  object.timer = GET_OBJ_TIMER(obj);
  object.bitvector = obj->obj_flags.bitvector;
  for (j = 0; j < MAX_OBJ_AFFECT; j++)
    object.affected[j] = obj->affected[j];

  if (fwrite(&object, sizeof(struct obj_file_elem), 1, fl) < 1) {
    perror("SYSERR: error writing object in Obj_to_store");
    return 0;
  }
  return 1;
}
/*
int Obj_to_store_from_new(struct obj_data * obj, FILE * fl, int locate)
{
  int j;

  fprintf(fl, "Vnum:\n%d\n", GET_OBJ_VNUM(obj));

  object.item_number = GET_OBJ_VNUM(obj);
  object.locate = (sh_int) locate; // where worn or inventory?
  object.value[0] = GET_OBJ_VAL(obj, 0);
  object.value[1] = GET_OBJ_VAL(obj, 1);
  object.value[2] = GET_OBJ_VAL(obj, 2);
  object.value[3] = GET_OBJ_VAL(obj, 3);
  object.extra_flags = GET_OBJ_EXTRA(obj);
  object.weight = GET_OBJ_WEIGHT(obj);
  object.timer = GET_OBJ_TIMER(obj);
  object.bitvector = obj->obj_flags.bitvector;
  for (j = 0; j < MAX_OBJ_AFFECT; j++)
    object.affected[j] = obj->affected[j];

  if (fwrite(&object, sizeof(struct obj_file_elem), 1, fl) < 1) {
    perror("SYSERR: error writing object in Obj_to_store");
    return 0;
  }
  return 1;
}
*/


int Obj_to_store(struct obj_data * obj, FILE * fl)
{
  return Obj_to_store_from(obj, fl, 0);
}



int Crash_delete_file(char *name)
{
  char filename[50];
  FILE *fl;

  if (!get_filename(name, filename, CRASH_FILE))
    return 0;
  if (!(fl = fopen(filename, "rb"))) {
    if (errno != ENOENT) {	/* if it fails but NOT because of no file */
      sprintf(buf1, "SYSERR: deleting crash file %s (1)", filename);
      perror(buf1);
    }
    return 0;
  }
  fclose(fl);

  if (remove(filename) < 0) {
    if (errno != ENOENT) {	/* if it fails, NOT because of no file */
      sprintf(buf1, "SYSERR: deleting crash file %s (2)", filename);
      perror(buf1);
    }
  }
  return (1);
}


int Crash_delete_crashfile(struct char_data * ch)
{
  char fname[MAX_INPUT_LENGTH];
  struct rent_info rent;
  FILE *fl;
  int flag=0;

  GET_NAME(ch, chname);
  if (!get_filename(chname, fname, CRASH_FILE))
    flag=1;
  FREE_NAME(chname);
  if (flag == 1)
    return 0;

  if (!(fl = fopen(fname, "rb"))) {
    if (errno != ENOENT) {	/* if it fails, NOT because of no file */
      sprintf(buf1, "SYSERR: checking for crash file %s (3)", fname);
      perror(buf1);
    }
    return 0;
  }
  if (!feof(fl))
    fread(&rent, sizeof(struct rent_info), 1, fl);
  fclose(fl);

  if (rent.rentcode == RENT_CRASH)
  {
    GET_NAME(ch, chname);
    Crash_delete_file(chname);
    FREE_NAME(chname);
  }

  return 1;
}


int Crash_clean_file(char *name)
{
  char fname[MAX_STRING_LENGTH], filetype[20];
  struct rent_info rent;
  extern int rent_file_timeout, crash_file_timeout;
  FILE *fl;

  if (!get_filename(name, fname, CRASH_FILE))
    return 0;
  /*
   * open for write so that permission problems will be flagged now, at boot
   * time.
   */
  if (!(fl = fopen(fname, "r+b"))) {
    if (errno != ENOENT) {	/* if it fails, NOT because of no file */
      sprintf(buf1, "SYSERR: OPENING OBJECT FILE %s (4)", fname);
      perror(buf1);
    }
    return 0;
  }
  if (!feof(fl))
    fread(&rent, sizeof(struct rent_info), 1, fl);
  fclose(fl);

  if ((rent.rentcode == RENT_CRASH) ||
      (rent.rentcode == RENT_FORCED) || (rent.rentcode == RENT_TIMEDOUT)) {
    if (rent.time < time(0) - (crash_file_timeout * SECS_PER_REAL_DAY)) {
      Crash_delete_file(name);
      switch (rent.rentcode) {
      case RENT_CRASH:
	strcpy(filetype, "crash");
	break;
      case RENT_FORCED:
	strcpy(filetype, "forced rent");
	break;
      case RENT_TIMEDOUT:
	strcpy(filetype, "idlesave");
	break;
      default:
	strcpy(filetype, "UNKNOWN!");
	break;
      }
      sprintf(buf, "    Deleting %s's %s file.", name, filetype);
      log(buf);
      return 1;
    }
    /* Must retrieve rented items w/in 30 days */
  } else if (rent.rentcode == RENT_RENTED)
    if (rent.time < time(0) - (rent_file_timeout * SECS_PER_REAL_DAY)) {
      Crash_delete_file(name);
      sprintf(buf, "    Deleting %s's rent file.", name);
      log(buf);
      return 1;
    }
  return (0);
}



void update_obj_file(void)
{
  int i;

  for (i = 0; i <= top_of_p_table; i++)
    Crash_clean_file((player_table + i)->name);
  return;
}



void Crash_listrent(struct char_data * ch, char *name)
{
  FILE *fl;
  char fname[MAX_INPUT_LENGTH], buf[MAX_STRING_LENGTH];
  struct obj_file_elem object;
  struct obj_data *obj;
  struct rent_info rent;

  if (!get_filename(name, fname, CRASH_FILE))
    return;
  if (!(fl = fopen(fname, "rb"))) {
    sprintf(buf, "%s has no rent file.\r\n", name);
    send_to_char(buf, ch);
    return;
  }
  sprintf(buf, "%s\r\n", fname);
  if (!feof(fl))
    fread(&rent, sizeof(struct rent_info), 1, fl);
  switch (rent.rentcode) {
  case RENT_RENTED:
    strcat(buf, "Rent\r\n");
    break;
  case RENT_CRASH:
    strcat(buf, "Crash\r\n");
    break;
  case RENT_CRYO:
    strcat(buf, "Cryo\r\n");
    break;
  case RENT_TIMEDOUT:
  case RENT_FORCED:
    strcat(buf, "TimedOut\r\n");
    break;
  default:
    strcat(buf, "Undef\r\n");
    break;
  }
  while (!feof(fl)) {
    fread(&object, sizeof(struct obj_file_elem), 1, fl);
    if (ferror(fl)) {
      fclose(fl);
      return;
    }
    if (!feof(fl))
      if (real_object(object.item_number) > -1) {
	obj = read_object(object.item_number, VIRTUAL);
	sprintf(buf + strlen(buf), " [%5d] (%5dau) <%2d> %-20s\r\n",
		object.item_number, GET_OBJ_RENT(obj),
		object.locate, obj->short_description);
	extract_obj(obj);
        if (strlen(buf) > MAX_STRING_LENGTH - 80) {
          strcat(buf, "** Excessive rent listing. **\r\n");
          break;
        }
     }
  }
  send_to_char(buf, ch);
  fclose(fl);
}



int Crash_write_rentcode(struct char_data * ch, FILE * fl, struct rent_info * rent)
{
  if (fwrite(rent, sizeof(struct rent_info), 1, fl) < 1) {
    perror("SYSERR: writing rent code");
    return 0;
  }
  return 1;
}



/* so this is gonna be the auto equip (hopefully) */
void auto_equip(struct char_data *ch, struct obj_data *obj, int locate)
{
  int j;

  if (locate > 0) { /* was worn */
    switch (j = locate-1) {
    case WEAR_LIGHT:
      break;
    case WEAR_FINGER_R:
    case WEAR_FINGER_L:
      if (!CAN_WEAR(obj,ITEM_WEAR_FINGER)) /* not fitting :( */
	locate = 0;
      break;
    case WEAR_NECK_1:
    case WEAR_NECK_2:
      if (!CAN_WEAR(obj,ITEM_WEAR_NECK))
	locate = 0;
      break;
    case WEAR_BODY:
      if (!CAN_WEAR(obj,ITEM_WEAR_BODY))
	locate = 0;
      break;
    case WEAR_HEAD:
      if (!CAN_WEAR(obj,ITEM_WEAR_HEAD))
	locate = 0;
      break;
    case WEAR_LEGS:
      if (!CAN_WEAR(obj,ITEM_WEAR_LEGS))
	locate = 0;
      break;
    case WEAR_FEET:
      if (!CAN_WEAR(obj,ITEM_WEAR_FEET))
	locate = 0;
      break;
    case WEAR_HANDS:
      if (!CAN_WEAR(obj,ITEM_WEAR_HANDS))
	locate = 0;
      break;
    case WEAR_ARMS:
      if (!CAN_WEAR(obj,ITEM_WEAR_ARMS))
	locate = 0;
      break;
    case WEAR_SHIELD:
      if (!CAN_WEAR(obj,ITEM_WEAR_SHIELD))
	locate = 0;
      break;
    case WEAR_ABOUT:
      if (!CAN_WEAR(obj,ITEM_WEAR_ABOUT))
	locate = 0;
      break;
    case WEAR_WAIST_1:
      if (!CAN_WEAR(obj,ITEM_WEAR_WAIST))
	locate = 0;
      break;
    case WEAR_WAIST_2:
      if (!CAN_WEAR(obj,ITEM_WEAR_WAIST))
        locate = 0;
      break;
    case WEAR_WRIST_R:
    case WEAR_WRIST_L:
      if (!CAN_WEAR(obj,ITEM_WEAR_WRIST))
	locate = 0;
      break;
    case WEAR_WIELD:
      if (!CAN_WEAR(obj,ITEM_WEAR_WIELD))
	locate = 0;
      break;
    case WEAR_HOLD:
      if (!CAN_WEAR(obj,ITEM_WEAR_HOLD) &&
	  !(IS_WARRIOR(ch) &&
	    CAN_WEAR(obj,ITEM_WEAR_WIELD) && GET_OBJ_TYPE(obj) == ITEM_WEAPON))
	locate = 0;
      break;
case WEAR_FACE:
       if (!CAN_WEAR(obj,ITEM_WEAR_FACE))
        locate = 0;
       break;
     case WEAR_EAR_R:
     case WEAR_EAR_L:
       if (!CAN_WEAR(obj,ITEM_WEAR_EAR))
        locate = 0;
       break;
     case WEAR_ANKLE_R:
     case WEAR_ANKLE_L:
       if (!CAN_WEAR(obj,ITEM_WEAR_ANKLE))
        locate = 0;
       break;
case WEAR_ABOVE:
       if (!CAN_WEAR(obj,ITEM_WEAR_ABOVE))
        locate = 0;
       break;
     case WEAR_BACK_1:
       if (!CAN_WEAR(obj,ITEM_WEAR_BACK))
        locate = 0;
       break;
     case WEAR_SHOULDER:
       if (!CAN_WEAR(obj,ITEM_WEAR_SHOULDER))
        locate = 0;
       break;
     case WEAR_NOSE:
       if (!CAN_WEAR(obj,ITEM_WEAR_NOSE))
        locate = 0;
      break;
      case WEAR_ONBELT:
       if (!CAN_WEAR(obj,ITEM_WEAR_ONBELT))
        locate = 0;
      break;
      case WEAR_ONBACK_1:
      case WEAR_ONBACK_2:
      case WEAR_ONBACK_3:
       if (!CAN_WEAR(obj,ITEM_WEAR_ONBACK))
        locate = 0;
       break;
     default:
      locate = 0;
    }
    if (locate > 0)
    {
      if (!GET_EQ(ch,j)) {
/* check ch's alignment to prevent $M from being zapped through auto-equip */
	if ((IS_OBJ_STAT(obj, ITEM_ANTI_EVIL) && IS_EVIL(ch)) ||
	    (IS_OBJ_STAT(obj, ITEM_ANTI_GOOD) && IS_GOOD(ch)) ||
	    (IS_OBJ_STAT(obj, ITEM_ANTI_NEUTRAL) && IS_NEUTRAL(ch)))
        {
	  locate = 0;
        }
	else
        {
	  equip_char(ch, obj, j);
        }
      }
      else  /* oops - saved player with double equipment[j]? */
      {
	locate = 0;
      }
    }
  }
  if (locate <= 0)
    obj_to_char(obj, ch);
}



#define MAX_BAG_ROW 5
/* should be enough - who would carry a bag in a bag in a bag in a
   bag in a bag in a bag ?!? */

int Crash_load(struct char_data * ch, int backup)
/* return values:
	0 - successful load, keep char in rent room.
	1 - load failure or load of crash items -- put char in temple.
	2 - rented equipment lost (no $)
*/
{
  void Crash_crashsave(struct char_data * ch, int backup);

  FILE *fl;
  char fname[MAX_STRING_LENGTH];
  struct obj_file_elem object;
  struct rent_info rent;
  int cost, orig_rent_code = 0;
  float num_of_days;
  struct obj_data *obj;
  int locate, j;
  struct obj_data *obj1;
  struct obj_data *cont_row[MAX_BAG_ROW];
  struct obj_data *objEntry = (struct obj_data*) NULL;

  int flag=0;

  /* Empty all of the container lists (you never know ...) */
  for (j = 0; j < MAX_BAG_ROWS; j++)
    cont_row[j] = NULL;

  GET_NAME(ch, chname);

  if (!get_filename(chname, fname, backup ? BACKUP_FILE : CRASH_FILE))
    flag=1;
  FREE_NAME(chname);
  if (flag == 1)
    return 1;

  if (!(fl = fopen(fname, "r+b"))) {
    if (errno != ENOENT) {	/* if it fails, NOT because of no file */
      log("SYSERR: READING OBJECT FILE %s (5): %s", fname, strerror(errno));
      send_to_char("\r\n********************* NOTICE *********************\r\n"
		   "There was a problem loading your objects from disk.\r\n"
		   "Contact a God for assistance.\r\n", ch);
    }
    GET_NAME(ch, chname);
    sprintf(buf, "%s entering game with no equipment.", chname);
    FREE_NAME(chname);
    mudlog(buf, NRM, MAX(LVL_IMMORT, GET_INVIS_LEV(ch)), TRUE);
    return 1;
  }

  if (!feof(fl))
    fread(&rent, sizeof(struct rent_info), 1, fl);
  else {
    GET_NAME(ch, chname);
    log("SYSERR: Crash_load: %s's rent file was empty!", chname);
    FREE_NAME(chname);
    return (1);
  }

  if (!backup && (rent.rentcode == RENT_RENTED || rent.rentcode == RENT_TIMEDOUT)) {
    num_of_days = (float) (time(0) - rent.time) / SECS_PER_REAL_DAY;
    cost = (int) (rent.net_cost_per_diem * num_of_days);

    if (cost > (money_to_copper(GET_MONEY(ch)) + GET_BANK_GOLD(ch))) {
      GET_NAME(ch, chname);
      sprintf(buf, "%s entering game, receiving anonymous npc donation to retain equipment.", chname);
      FREE_NAME(chname);
      mudlog(buf, BRF, MAX(LVL_IMMORT, GET_INVIS_LEV(ch)), TRUE);
      send_to_char("A mysterious benefactor has paid your rent and left you with enough to rent another day in the bank.\r\n", ch); 
      (GET_MONEY(ch)).platinum = 0;
      (GET_MONEY(ch)).steel = 0;
      (GET_MONEY(ch)).gold = 0;
      (GET_MONEY(ch)).copper = 0;
      GET_BANK_GOLD(ch) = rent.net_cost_per_diem;
      /* Try to sell off equipment ??? */
      fclose(fl);
      GET_NAME(ch, chname);
      sprintf(buf, "%s entering game, rented equipment lost (no $).", chname);
      FREE_NAME(chname);
      mudlog(buf, BRF, MAX(LVL_IMMORT, GET_INVIS_LEV(ch)), TRUE);
      Crash_crashsave(ch, FALSE);
      return 2;

    } else {

      if ( cost > money_to_copper(GET_MONEY(ch)) ) {
	/*  take all on-hand money, and some bank money too  */
	cost -= money_to_copper(GET_MONEY(ch));

	(GET_MONEY(ch)).platinum = 0;
	(GET_MONEY(ch)).steel = 0;
	(GET_MONEY(ch)).gold = 0;
	(GET_MONEY(ch)).copper = 0;
	GET_BANK_GOLD(ch) -= cost;

	if ( (cost < 0) || (GET_BANK_GOLD(ch) < 0) ) {
	  log("SYSERR: cost or bank money is less than zero");
	}

      } else {
	/*  just take from on-hand money  */
	int cpLeft = money_to_copper(GET_MONEY(ch)) - cost;
	copper_to_money( &(GET_MONEY(ch)), cpLeft);

	if ( cpLeft < 0 ) {
	  log("SYSERR: copper left in hand is less than zero");
	}
      }

      /*  OLD below
      GET_BANK_GOLD(ch) -= MAX(cost - GET_GOLD(ch), 0);
      GET_GOLD(ch) = MAX(GET_GOLD(ch) - cost, 0);
      */

      save_char(ch, NOWHERE);
    }
  }

  if (!backup) {
  switch (orig_rent_code = rent.rentcode) {
  case RENT_RENTED:
    GET_NAME(ch, chname);
    sprintf(buf, "%s un-renting and entering game.", chname);
    FREE_NAME(chname);
    mudlog(buf, NRM, MAX(LVL_IMMORT, GET_INVIS_LEV(ch)), TRUE);
    break;
  case RENT_CRASH:
    GET_NAME(ch, chname);
    sprintf(buf, "%s retrieving crash-saved items and entering game.", chname);
    FREE_NAME(chname);
    mudlog(buf, NRM, MAX(LVL_IMMORT, GET_INVIS_LEV(ch)), TRUE);
    break;
  case RENT_CRYO:
    GET_NAME(ch, chname);
    sprintf(buf, "%s un-cryo'ing and entering game.", chname);
    FREE_NAME(chname);
    mudlog(buf, NRM, MAX(LVL_IMMORT, GET_INVIS_LEV(ch)), TRUE);
    break;
  case RENT_FORCED:
  case RENT_TIMEDOUT:
    GET_NAME(ch, chname);
    sprintf(buf, "%s retrieving force-saved items and entering game.", chname);
    FREE_NAME(chname);
    mudlog(buf, NRM, MAX(LVL_IMMORT, GET_INVIS_LEV(ch)), TRUE);
    break;
  default:
    GET_NAME(ch, chname);
    sprintf(buf, "WARNING: %s entering game with undefined rent code.", chname);
    FREE_NAME(chname);
    mudlog(buf, BRF, MAX(LVL_IMMORT, GET_INVIS_LEV(ch)), TRUE);
    break;
  }
  }

  while (!feof(fl)) {
    fread(&object, sizeof(struct obj_file_elem), 1, fl);
    if (ferror(fl)) {
      perror("SYSERR: Reading crash file: Crash_load");
      fclose(fl);
      return 1;
    }
    if (!feof(fl))
      if ((obj = Obj_from_store_to(object, &locate))) {
	auto_equip(ch, obj, locate);

/*
what to do with a new loaded item:

  if there's a list with <locate> less than 1 below this:
    (equipped items are assumed to have <locate>==0 here) then its
    container has disappeared from the file   *gasp*
     -> put all the list back to ch's inventory

  if there's a list of contents with <locate> 1 below this:
    check if it's a container
    - if so: get it from ch, fill it, and give it back to ch (this way the
        container has its correct weight before modifying ch)
    - if not: the container is missing -> put all the list to ch's inventory

  for items with negative <locate>:
    if there's already a list of contents with the same <locate> put obj to it
    if not, start a new list

Confused? Well maybe you can think of some better text to be put here ...

since <locate> for contents is < 0 the list indices are switched to
non-negative
*/

	if (locate > 0) { /* item equipped */
	  for (j = MAX_BAG_ROW-1;j > 0;j--)
	    if (cont_row[j]) { /* no container -> back to ch's inventory */
	      for (;cont_row[j];cont_row[j] = obj1) {
		obj1 = cont_row[j]->next_content;
		obj_to_char(cont_row[j], ch);
	      }
	      cont_row[j] = NULL;
	    }
	  if (cont_row[0]) { /* content list existing */
	    if (GET_OBJ_TYPE(obj) == ITEM_CONTAINER || GET_OBJ_TYPE(obj) == ITEM_SHEATH) {
	      /* rem item ; fill ; equip again */
	      obj = unequip_char(ch, locate-1);
	      obj->contains = NULL; /* should be empty - but who knows */
	      for (;cont_row[0];cont_row[0] = obj1) {
		obj1 = cont_row[0]->next_content;
		obj_to_obj(cont_row[0], obj);
	      }
	      equip_char(ch, obj, locate-1);
	    } else { /* object isn't container -> empty content list */
	      for (;cont_row[0];cont_row[0] = obj1) {
		obj1 = cont_row[0]->next_content;
		obj_to_char(cont_row[0], ch);
	      }
	      cont_row[0] = NULL;
	    }
	  }
	} else { /* locate <= 0 */
	  for (j = MAX_BAG_ROW-1;j > -locate;j--)
	    if (cont_row[j]) { /* no container -> back to ch's inventory */
	      for (;cont_row[j];cont_row[j] = obj1) {
		obj1 = cont_row[j]->next_content;
		obj_to_char(cont_row[j], ch);
	      }
	      cont_row[j] = NULL;
	    }

	  if (j == -locate && cont_row[j]) { /* content list existing */
	    if (GET_OBJ_TYPE(obj) == ITEM_CONTAINER || GET_OBJ_TYPE(obj) == ITEM_SHEATH) {
	      /* take item ; fill ; give to char again */
	      obj_from_char(obj);
	      obj->contains = NULL;
	      for (;cont_row[j];cont_row[j] = obj1) {
		obj1 = cont_row[j]->next_content;
		obj_to_obj(cont_row[j], obj);
	      }
	      obj_to_char(obj, ch); /* add to inv first ... */
	    } else { /* object isn't container -> empty content list */
	      for (;cont_row[j];cont_row[j] = obj1) {
		obj1 = cont_row[j]->next_content;
		obj_to_char(cont_row[j], ch);
	      }
	      cont_row[j] = NULL;
	    }
	  }

	  if (locate < 0 && locate >= -MAX_BAG_ROW) {
		/* let obj be part of content list
		   but put it at the list's end thus having the items
		   in the same order as before renting */
	    obj_from_char(obj);
	    if ((obj1 = cont_row[-locate-1])) {
	      while (obj1->next_content)
		obj1 = obj1->next_content;
	      obj1->next_content = obj;
	    } else
	      cont_row[-locate-1] = obj;
	  }
	}
      }
  }

  /* Check for free rent token */
  for (objEntry = ch->carrying; objEntry; objEntry = objEntry->next_content)
  {
    if (!backup && GET_OBJ_VNUM(objEntry) == VNUM_FREE_RENT_TOKEN)
    {
      extract_obj(objEntry);
      break;
    }
}

if (!backup) {

 if (PLR_FLAGGED(ch, PLR_FISHING))
   REMOVE_BIT(PLR_FLAGS(ch), PLR_FISHING);

if (PLR_FLAGGED(ch, PLR_FISH_ON))
   REMOVE_BIT(PLR_FLAGS(ch), PLR_FISH_ON);

if (PLR_FLAGGED(ch, PLR_DIGGING))
   REMOVE_BIT(PLR_FLAGS(ch), PLR_DIGGING);

if (PLR_FLAGGED(ch, PLR_DIG_ON))
   REMOVE_BIT(PLR_FLAGS(ch), PLR_DIG_ON);

if (PLR_FLAGGED(ch, PLR_FIRE_ON))
   REMOVE_BIT(PLR_FLAGS(ch), PLR_FIRE_ON);

if (AFF_FLAGGED(ch, AFF_POLYMORPH))
affect_from_char_II(ch, SPELL_POLYMORPH, SPELL_POLYMORPH, 2);


}

  /* turn this into a crash file by re-writing the control block */
  rent.rentcode = RENT_CRASH;
  rent.time = time(0);
  rewind(fl);
  Crash_write_rentcode(ch, fl, &rent);

  fclose(fl);

  if ((orig_rent_code == RENT_RENTED) || (orig_rent_code == RENT_CRYO))
    return 0;
  else
    return 1;
}



int Crash_save(struct obj_data * obj, FILE * fp, int locate)
{
  struct obj_data *tmp;
  int result;

  if (obj) {
    Crash_save(obj->next_content, fp, locate);
    Crash_save(obj->contains, fp, MIN(0,locate)-1);
    result = Obj_to_store_from(obj, fp, locate);

    for (tmp = obj->in_obj; tmp; tmp = tmp->in_obj)
      GET_OBJ_WEIGHT(tmp) -= GET_OBJ_WEIGHT(obj);

    if (!result)
      return 0;
  }
  return TRUE;
}


void Crash_restore_weight(struct obj_data * obj)
{
  if (obj) {
    Crash_restore_weight(obj->contains);
    Crash_restore_weight(obj->next_content);
    if (obj->in_obj)
      GET_OBJ_WEIGHT(obj->in_obj) += GET_OBJ_WEIGHT(obj);
  }
}



void Crash_extract_objs(struct obj_data * obj)
{
  if (obj) {
    Crash_extract_objs(obj->contains);
    Crash_extract_objs(obj->next_content);
    extract_obj(obj);
  }
}


int Crash_is_unrentable(struct obj_data * obj)
{
  if (!obj)
    return 0;

  if (IS_OBJ_STAT(obj, ITEM_NORENT) || GET_OBJ_RENT(obj) < 0 ||
      GET_OBJ_RNUM(obj) <= NOTHING || GET_OBJ_TYPE(obj) == ITEM_KEY)
    return 1;

  return 0;
}


void Crash_extract_norents(struct obj_data * obj)
{
  if (obj) {
    Crash_extract_norents(obj->contains);
    Crash_extract_norents(obj->next_content);
    if (Crash_is_unrentable(obj))
      extract_obj(obj);
  }
}


/* get norent items from eq to inventory and
   extract norents out of worn containers */
void Crash_extract_norents_from_equipped(struct char_data * ch)
{
  int j;

  for (j = 0;j < NUM_WEARS;j++) {
    if (GET_EQ(ch,j)) {
      if (IS_OBJ_STAT(GET_EQ(ch,j), ITEM_NORENT) ||
	  GET_OBJ_RENT(GET_EQ(ch,j)) < 0 ||
	  GET_OBJ_RNUM(GET_EQ(ch,j)) <= NOTHING ||
	  GET_OBJ_TYPE(GET_EQ(ch,j)) == ITEM_KEY)
	obj_to_char(unequip_char(ch,j),ch);
      else
	Crash_extract_norents(GET_EQ(ch,j));
    }
  }
}


void Crash_extract_expensive(struct obj_data * obj)
{
  struct obj_data *tobj, *max;

  max = obj;
  for (tobj = obj; tobj; tobj = tobj->next_content)
    if (GET_OBJ_RENT(tobj) > GET_OBJ_RENT(max))
      max = tobj;
  extract_obj(max);
}



void Crash_calculate_rent(struct obj_data * obj, int *cost)
{
  if (obj) {
    *cost += MAX(0, GET_OBJ_RENT(obj));
    Crash_calculate_rent(obj->contains, cost);
    Crash_calculate_rent(obj->next_content, cost);
  }
}


void Crash_crashsave(struct char_data * ch, int backup)
{
  char buf[MAX_INPUT_LENGTH];
  struct rent_info rent;
  int j;
  FILE *fp;
  int flag=0;

  if (IS_NPC(ch))
    return;

  GET_NAME(ch, chname);

  if (!get_filename(chname, buf, backup ? BACKUP_FILE : CRASH_FILE))
    flag=1;

  FREE_NAME(chname);

  if (flag == 1) 
    return;

  if (!(fp = fopen(buf, "wb")))
    return;

  rent.rentcode = RENT_CRASH;
  rent.time = time(0);
  if (!Crash_write_rentcode(ch, fp, &rent)) {
    fclose(fp);
    return;
  }
  for (j = 0; j < NUM_WEARS; j++)
    if (GET_EQ(ch,j)) {
      if (!Crash_save(GET_EQ(ch,j), fp, j+1)) {
	fclose(fp);
	return;
      }
      Crash_restore_weight(GET_EQ(ch,j));
    }

  if (!Crash_save(ch->carrying, fp, 0)) {
    fclose(fp);
    return;
  }
  Crash_restore_weight(ch->carrying);

  fclose(fp);
  REMOVE_BIT(PLR_FLAGS(ch), PLR_CRASH);
}


void Crash_idlesave(struct char_data * ch)
{
  char buf[MAX_INPUT_LENGTH];
  struct rent_info rent;
  int j;
  int cost, cost_eq;
  FILE *fp;
  int flag=0;

  if (IS_NPC(ch))
    return;

  GET_NAME(ch, chname);

  if (!get_filename(chname, buf, CRASH_FILE))
    flag=1;

  FREE_NAME(chname);

  if (flag == 1)
    return;

  if (!(fp = fopen(buf, "wb")))
    return;

  Crash_extract_norents_from_equipped(ch);

  Crash_extract_norents(ch->carrying);

  cost = 0;
  Crash_calculate_rent(ch->carrying, &cost);

  cost_eq = 0;
  for (j = 0; j < NUM_WEARS; j++)
    Crash_calculate_rent(GET_EQ(ch,j), &cost_eq);

  cost <<= 1;			/* forcerent cost is 2x normal rent */
  cost_eq <<= 1;

  if ( (cost+cost_eq) > (money_to_copper(GET_MONEY(ch)) + GET_BANK_GOLD(ch))) {
    for (j = 0; j < NUM_WEARS; j++) /* unequip player with low money */
      if (GET_EQ(ch,j))
	obj_to_char(unequip_char(ch, j), ch);
    cost += cost_eq;
    cost_eq = 0;

    while ((cost > (money_to_copper(GET_MONEY(ch)) + GET_BANK_GOLD(ch))) &&
	   ch->carrying) {
      Crash_extract_expensive(ch->carrying);
      cost = 0;
      Crash_calculate_rent(ch->carrying, &cost);
      cost <<= 1;
    }
  }

  if (!ch->carrying) {
    for (j = 0; j < NUM_WEARS && !(GET_EQ(ch,j)); j++)
      ;
    if (j == NUM_WEARS) { /* no eq nor inv */
      fclose(fp);
      GET_NAME(ch, chname);
      Crash_delete_file(chname);
      FREE_NAME(chname);
      return;
    }
  }
  rent.net_cost_per_diem = cost;

  rent.rentcode = RENT_TIMEDOUT;
  rent.time = time(0);
  rent.gold = money_to_copper(GET_MONEY(ch));
  rent.account = GET_BANK_GOLD(ch);
  if (!Crash_write_rentcode(ch, fp, &rent)) {
    fclose(fp);
    return;
  }

  for (j = 0; j < NUM_WEARS; j++)
    if (GET_EQ(ch,j)) {
      if (!Crash_save(GET_EQ(ch,j), fp, j+1)) {
	fclose(fp);
	return;
      }
      Crash_restore_weight(GET_EQ(ch,j));
      Crash_extract_objs(GET_EQ(ch,j));
    }
  if (!Crash_save(ch->carrying, fp, 0)) {
    fclose(fp);
    return;
  }
  fclose(fp);

  Crash_extract_objs(ch->carrying);
}


void Crash_rentsave(struct char_data * ch, int cost)
{
  char buf[MAX_INPUT_LENGTH];
  struct rent_info rent;
  int j;
  FILE *fp;
  int flag=0;

  if (IS_NPC(ch))
    return;

  GET_NAME(ch, chname);

  if (!get_filename(chname, buf, CRASH_FILE))
    flag=1;

  FREE_NAME(chname);

  if (flag == 1)
    return;

  if (!(fp = fopen(buf, "wb")))
    return;

  Crash_extract_norents_from_equipped(ch);

  Crash_extract_norents(ch->carrying);

  rent.net_cost_per_diem = cost;

  rent.rentcode = RENT_RENTED;
  rent.time = time(0);
  rent.gold = money_to_copper(GET_MONEY(ch));
  rent.account = GET_BANK_GOLD(ch);
  if (!Crash_write_rentcode(ch, fp, &rent)) {
    fclose(fp);
    return;
  }
  for (j = 0; j < NUM_WEARS; j++)
    if (GET_EQ(ch,j)) {
      if (!Crash_save(GET_EQ(ch,j), fp, j+1)) {
	fclose(fp);
	return;
      }
      Crash_restore_weight(GET_EQ(ch,j));
      Crash_extract_objs(GET_EQ(ch,j));
    }
  if (!Crash_save(ch->carrying, fp, 0)) {
    fclose(fp);
    return;
  }
  fclose(fp);

  Crash_extract_objs(ch->carrying);
}


void Crash_cryosave(struct char_data * ch, int cost)
{
  char buf[MAX_INPUT_LENGTH];
  struct rent_info rent;
  int j;
  FILE *fp;
  int flag=0;

  if (IS_NPC(ch))
    return;

  if (TIME_ONLINE(ch) < 15)
    cost = 0;

  GET_NAME(ch, chname);

  if (!get_filename(chname, buf, CRASH_FILE))
    flag=1;

  FREE_NAME(chname);

  if (flag == 1)
    return;

  if (!(fp = fopen(buf, "wb")))
    return;

  Crash_extract_norents_from_equipped(ch);

  Crash_extract_norents(ch->carrying);

  if ( cost > money_to_copper(GET_MONEY(ch)) ) {
    (GET_MONEY(ch)).platinum = 0;
    (GET_MONEY(ch)).steel = 0;
    (GET_MONEY(ch)).gold = 0;
    (GET_MONEY(ch)).copper = 0;
  } else {
    struct money_data tmp;
    tmp.platinum = 0;    tmp.steel = 0;    tmp.gold = 0;
    tmp.copper = cost;

    money_subtract( &(GET_MONEY(ch)), tmp);
  }
  /*  GET_GOLD(ch) = MAX(0, GET_GOLD(ch) - cost);  */

  rent.rentcode = RENT_CRYO;
  rent.time = time(0);
  rent.gold = money_to_copper(GET_MONEY(ch));
  rent.account = GET_BANK_GOLD(ch);
  rent.net_cost_per_diem = 0;
  if (!Crash_write_rentcode(ch, fp, &rent)) {
    fclose(fp);
    return;
  }
  for (j = 0; j < NUM_WEARS; j++)
    if (GET_EQ(ch,j)) {
      if (!Crash_save(GET_EQ(ch,j), fp, j+1)) {
	fclose(fp);
	return;
      }
      Crash_restore_weight(GET_EQ(ch,j));
      Crash_extract_objs(GET_EQ(ch,j));
    }
  if (!Crash_save(ch->carrying, fp, 0)) {
    fclose(fp);
    return;
  }
  fclose(fp);

  Crash_extract_objs(ch->carrying);
  /*SET_BIT(PLR_FLAGS(ch), PLR_CRYO); */
}


/* ************************************************************************
* Routines used for the receptionist					  *
************************************************************************* */

void Crash_rent_deadline(struct char_data * ch, struct char_data * recep,
			      long cost)
{
  long rent_deadline;

  if (!cost)
    return;

  rent_deadline =((money_to_copper(GET_MONEY(ch)) + GET_BANK_GOLD(ch)) / cost);
  sprintf(buf,
      "$n tells you, 'You can rent for %ld day%s with the money you have\r\n"
	  "on hand and in the bank.'\r\n",
	  rent_deadline, (rent_deadline > 1) ? "s" : "");
  act(buf, FALSE, recep, 0, ch, TO_VICT);
}

int Crash_report_unrentables(struct char_data * ch, struct char_data * recep,
			         struct obj_data * obj)
{
  char buf[128];
  int has_norents = 0;

  if (obj) {
    if (Crash_is_unrentable(obj)) {
      has_norents = 1;
      sprintf(buf, "$n tells you, 'You cannot store %s.'", OBJS(obj, ch));
      act(buf, FALSE, recep, 0, ch, TO_VICT);
    }
    has_norents += Crash_report_unrentables(ch, recep, obj->contains);
    has_norents += Crash_report_unrentables(ch, recep, obj->next_content);
  }
  return (has_norents);
}



void Crash_report_rent(struct char_data * ch, struct char_data * recep,
		            struct obj_data * obj, long *cost, long *nitems, int display, int factor)
{
  static char buf[256];
  int chCha = 0;
  int objCost = 0;

  if (obj) {
    if (!Crash_is_unrentable(obj)) {
      (*nitems)++;

      objCost = MAX(0, (GET_OBJ_RENT(obj) * factor));

      /* UGLY PEOPLE MUST PAY!  CHA FACTORS INTO RENT HERE. */

      chCha = GET_CHA(ch);
      
      if (chCha > 16)      /* CHA 17-18+*/
      {
        objCost = (int)((float) objCost * 0.8);
      }
      else if (chCha > 12) /* CHA 13-16 */
      {
        objCost = (int)((float) objCost * 0.9);
      }
      else if (chCha > 8)  /* CHA 9-12 */
      {
        objCost = (int)((float) objCost * 1.0);
      }
      else if (chCha == 8)  /* CHA 8 */
      {
        objCost = (int)((float) objCost * 1.1);
      }
      else if (chCha == 7)  /* CHA 7 */
      {
        objCost = (int)((float) objCost * 1.2);
      }
      else if (chCha > 3)  /* CHA 4 - 6 */
      {
        objCost = (int)((float) objCost * 1.3);
      }
      else                 /* CHA 1 - 3 */
      {
        objCost = (int)((float) objCost * 1.4);
      }

/* Rent is too high, according to popular belief. Let's see. */
      objCost = objCost / 2;

/* Ok, let's factor in RP value. --gan */
      objCost = (int)((objCost * 100) / (GET_LEVEL(ch) >= LVL_IMMORT ? 100 : GET_RPFACTOR(ch)));

      *cost += objCost;

      if (display) {
	struct money_data costMoney;

	copper_to_money(&costMoney, objCost);
	sprintf(buf, "$n tells you, '%s for %s..'",
		describe_money(costMoney), OBJS(obj, ch));
	act(buf, FALSE, recep, 0, ch, TO_VICT);
      }
    }
    Crash_report_rent(ch, recep, obj->contains, cost, nitems, display, factor);
    Crash_report_rent(ch, recep, obj->next_content, cost, nitems, display, factor);
  }
}



int Crash_offer_rent(struct char_data * ch, struct char_data * receptionist,
		         int display, int factor)
{
  extern int max_obj_save;	/* change in config.c */
  char buf[MAX_INPUT_LENGTH];
  int i;
  long totalcost = 0, numitems = 0, norent = 0;
  struct obj_data* objEntry = (struct obj_data *) NULL;
  int rent_for_free = 0;
  int oldDisplay = display;

/* wonder where this code went? --gan */
  if (PLR_FLAGGED(ch, PLR_FREERENT))
  {
    rent_for_free = 1;
  }

  if (TIME_ONLINE(ch) < 15)
    rent_for_free = 1;

  /* Check for free rent token */
  for (objEntry = ch->carrying; objEntry; objEntry = objEntry->next_content)
  {
    if (GET_OBJ_VNUM(objEntry) == VNUM_FREE_RENT_TOKEN)
    {
      rent_for_free = 1;
      break;
    }
  }           

  norent = Crash_report_unrentables(ch, receptionist, ch->carrying);

  for (i = 0; i < NUM_WEARS; i++)
  {
    norent += Crash_report_unrentables(ch, receptionist, GET_EQ(ch,i));
  }

  if (norent)
  {
    return 0;
  }

  totalcost = min_rent_cost * factor;

  if (rent_for_free == 1)
  {
    display = 0;
  }

  Crash_report_rent(ch, receptionist, ch->carrying, &totalcost, &numitems, 
    display, factor);

  for (i = 0; i < NUM_WEARS; i++)
  {
    Crash_report_rent(ch, receptionist, GET_EQ(ch,i), &totalcost, &numitems, 
       display, factor);
  } 

  if (!numitems) 
  {
    act("$n tells you, 'But you are not carrying anything!  Just quit!'",
      FALSE, receptionist, 0, ch, TO_VICT);
    return (0);
  }

  if (numitems > max_obj_save)
  {
    sprintf(buf, 
      "$n tells you, 'Sorry, but I cannot store more than %d items.'",
      max_obj_save);
    act(buf, FALSE, receptionist, 0, ch, TO_VICT);
    return (0);
  }

  if (ROOM_FLAGGED(ch->in_room, ROOM_TOWNHS))
   {
     totalcost -= (int)((float) totalcost * 0.15);
  }

  if (ROOM_FLAGGED(ch->in_room, ROOM_CUSTOMHS))
   {
     totalcost -= (int)((float) totalcost * 0.10);
  } 

  if (rent_for_free == 1)
  {
    totalcost = -1;
  }

  if (oldDisplay) 
  {
    struct money_data tot;

    copper_to_money(&tot, totalcost);
    
    if (rent_for_free == 0)
    {
      sprintf(buf, "$n tells you, 'Plus, my %d copper fee..'",
        min_rent_cost * factor);
      act(buf, FALSE, receptionist, 0, ch, TO_VICT);

      sprintf(buf, "$n tells you, 'For a total of %s%s.'",
        describe_money(tot), (factor == RENT_FACTOR ? " per day" : ""));
      act(buf, FALSE, receptionist, 0, ch, TO_VICT);
    }
    else
    {
      sprintf(buf, 
        "$n tells you, 'You may rent for free with the token that you have.'");
      act(buf, FALSE, receptionist, 0, ch, TO_VICT);
    }

    if (totalcost > money_to_copper(GET_MONEY(ch))) {
      act("$n tells you, '...which I see you can't afford.'",
	  FALSE, receptionist, 0, ch, TO_VICT);
      return (0);
    } else if (factor == RENT_FACTOR) {
      if (totalcost > 0)
      {
        Crash_rent_deadline(ch, receptionist, totalcost);
      }
    }
  }

  return (totalcost);
}



int gen_receptionist(struct char_data * ch, struct char_data * recep,
		         int cmd, char *arg, int mode)
{
  int cost = 0;
  extern int free_rent;
  sh_int save_room;
  char *action_table[] = {"smile", "dance", "sigh", "blush", "burp",
  "cough", "fart", "twiddle", "yawn"};

  if (!ch->desc || IS_NPC(ch))
    return FALSE;

  if (!cmd && !number(0, 5)) {
    do_action(recep, "", find_command(action_table[number(0, 8)]), 0);
    return FALSE;
  }

  if (!CMD_IS("offer") && !CMD_IS("rent")) {
    return FALSE;
  }

  if (!AWAKE(recep)) {
    send_to_char("Unfortunately, they can't talk to you right now.\r\n", ch);
    return TRUE;
  }

  if (!CAN_SEE(recep, ch)) {
    act("$n says, 'I don't deal with people I can't see!'",
	FALSE, recep, 0, 0, TO_ROOM);
    return TRUE;
  }

  if (free_rent) {
    act("$n tells you, 'Rent is free here.  Just quit, and your objects will be saved!'",
	FALSE, recep, 0, ch, TO_VICT);
    return 1;
  }

  if (CMD_IS("rent")) {
    /*  PDH  4/12/99 - pkiller can't rent  */
    if ( PLR_FLAGGED(ch, PLR_KILLER) ) {
      act("$n says, 'I don't deal with murderers!'",
	  FALSE, recep, 0, 0, TO_ROOM);
      return TRUE;
    }

    if (PLR_FLAGGED(ch, PLR_THIEF))
    {
      act("$n says, 'I don't deal with those who take other's things.'",
           FALSE, recep, 0, 0, TO_ROOM);
      return TRUE;
    }

    cost = Crash_offer_rent(ch, recep, FALSE, mode);
    
    if (cost != -1) 
    {
      if (!(cost = Crash_offer_rent(ch, recep, FALSE, mode))) 
      {
        return TRUE;
      }
    }
    else
    {
      cost = 0;
    }

    if (mode == RENT_FACTOR) {
      struct money_data cM;
      copper_to_money(&cM, cost);
      sprintf(buf, "$n tells you, 'Rent will cost you %s per day.'",
	      describe_money(cM));

    } else if (mode == CRYO_FACTOR) {
      struct money_data cM;
      copper_to_money(&cM, cost);
      sprintf(buf,
	      "$n tells you, 'It will cost you %s to be frozen.'",
	      describe_money(cM));
    }

    act(buf, FALSE, recep, 0, ch, TO_VICT);

    if (cost > money_to_copper(GET_MONEY(ch))) {
      act("$n tells you, '...which I see you can't afford.'",
	  FALSE, recep, 0, ch, TO_VICT);
      return TRUE;
    }

    if (cost && (mode == RENT_FACTOR)) {
      Crash_rent_deadline(ch, recep, cost);
    }

    if (mode == RENT_FACTOR) {
      act("$n stores your belongings and helps you into your private chamber.",
	  FALSE, recep, 0, ch, TO_VICT);
      Crash_cryosave(ch, cost);
      GET_NAME(ch, chname);
      sprintf(buf, "%s has rented (%d/day, %d tot.)", chname,
	      cost, money_to_copper(GET_MONEY(ch)) + GET_BANK_GOLD(ch));
      FREE_NAME(chname);
    } else {			/* cryo */
      act("$n stores your belongings and helps you into your private chamber.\r\n"
	  "A white mist appears in the room, chilling you to the bone...\r\n"
	  "You begin to lose consciousness...",
	  FALSE, recep, 0, ch, TO_VICT);
      Crash_cryosave(ch, cost);
      GET_NAME(ch, chname);
      sprintf(buf, "%s has cryo-rented.", chname);
      FREE_NAME(chname);
      /*SET_BIT(PLR_FLAGS(ch), PLR_CRYO);*/
    }

    mudlog(buf, NRM, MAX(LVL_IMMORT, GET_INVIS_LEV(ch)), TRUE);
    act("$n helps $N into $S private chamber.",
	FALSE, recep, 0, ch, TO_NOTVICT);

    /*  PDH  4/ 6/99 - to have morts load back into rent room  */
    REMOVE_BIT(PLR_FLAGS(ch), PLR_LOADROOM);

    save_room = ch->in_room;
    extract_char(ch);
    save_char(ch, save_room);

    if (ch->player_specials != NULL)
    {
      ch->player_specials->saved.load_room  = world[save_room].number;
      /*  PDH  4/ 6/99 - to have morts load back into rent room  */
      SET_BIT(PLR_FLAGS(ch), PLR_LOADROOM);
    }
    else
    {
      return TRUE;
    }
  } else {
    Crash_offer_rent(ch, recep, TRUE, mode);
    act("$N gives $n an offer.", FALSE, ch, 0, recep, TO_ROOM);
  }
  return TRUE;
}


SPECIAL(receptionist)
{
  return (gen_receptionist(ch, me, cmd, argument, RENT_FACTOR));
}


SPECIAL(cryogenicist)
{
  return (gen_receptionist(ch, me, cmd, argument, CRYO_FACTOR));
}


void Crash_save_all(void)
{
  struct descriptor_data *d;

  for (d = descriptor_list; d; d = d->next) {
    if ((STATE(d) == CON_PLAYING) && !IS_NPC(d->character)) {
      if (PLR_FLAGGED(d->character, PLR_CRASH)) {
	Crash_crashsave(d->character, FALSE);
	save_char(d->character, NOWHERE);
	REMOVE_BIT(PLR_FLAGS(d->character), PLR_CRASH);
      }


    REMOVE_BIT(PLR_FLAGS(d->character), PLR_FISHING);
    REMOVE_BIT(PLR_FLAGS(d->character), PLR_FISH_ON);
    REMOVE_BIT(PLR_FLAGS(d->character), PLR_DIGGING);
    REMOVE_BIT(PLR_FLAGS(d->character), PLR_DIG_ON);
    REMOVE_BIT(PLR_FLAGS(d->character), PLR_FIRE_ON);
    REMOVE_BIT(PRF_FLAGS(d->character), PRF_NOTSELF);
    //REMOVE_BIT(PRF_FLAGS(d->character), PRF_DISGUISE);
    //REMOVE_BIT(PLR_FLAGS(d->character), PLR_MAGE);
    //REMOVE_BIT(PLR_FLAGS(d->character), PLR_MONK);
    //REMOVE_BIT(PLR_FLAGS(d->character), PLR_KNIGHT);
    //REMOVE_BIT(PLR_FLAGS(d->character), PLR_CLERIC);
    //REMOVE_BIT(PLR_FLAGS(d->character), PLR_BARD);
    //REMOVE_BIT(PLR_FLAGS(d->character), PLR_BEGGAR);
    REMOVE_BIT(PLR_FLAGS(d->character), PLR_COURIER);
    REMOVE_BIT(PLR_FLAGS(d->character), PLR_BEAR);
    REMOVE_BIT(PLR_FLAGS(d->character), PLR_BIRD);
    REMOVE_BIT(PLR_FLAGS(d->character), PLR_WOLF);
    REMOVE_BIT(PLR_FLAGS(d->character), PLR_RABBIT);
    REMOVE_BIT(PLR_FLAGS(d->character), PLR_CAT);

    }
  }
}

void Crash_save_all_II(void)
{
  struct descriptor_data *d;

  for (d = descriptor_list; d; d = d->next) {
    if ((STATE(d) == CON_PLAYING) && !IS_NPC(d->character)) {
        Crash_crashsave(d->character, FALSE);
        save_char(d->character, NOWHERE);
        REMOVE_BIT(PLR_FLAGS(d->character), PLR_CRASH);
      }
  }
}


