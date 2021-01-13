/* ************************************************************************
*   File: act.item.c                                    Part of CircleMUD *
*  Usage: object handling routines -- get/drop and container handling     *
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
#include "dg_scripts.h"
#include "olc.h"

/* extern variables */
extern struct str_app_type str_app[];
extern struct room_data *world;
extern char *drinks[];
extern int drink_aff[][3];
extern char *where[];
extern int invalid_class(struct char_data *ch, struct obj_data *obj);
extern int invalid_race(struct char_data *ch, struct obj_data *obj);
extern void show_obj_to_char(struct obj_data * object, struct char_data * ch,
                        int mode);
void affect_from_char_II(struct char_data * ch, int skill, int type, int action);
void affect_remove_nonperm_II(struct char_data * ch, int type);
int hidden_eq(struct char_data *ch, struct obj_data *obj);
//void reveal(struct char_data *ch);
int has_materials(struct char_data * ch, int spellnum);

// Local functions:
ACMD(do_enchant);

// Function code

int not_restricted(struct char_data * ch, struct obj_data * obj)
{
  if (invalid_race(ch, obj))
  {
    send_to_char("Those of your race cannot use this object.\r\n", ch);
    return 0;
  }

  if (invalid_class(ch, obj))
  {
    send_to_char("This item is unusable by those of your skills.\r\n", ch);
    return 0;
  }
  if (GET_OBJ_LEVEL(obj) > (GET_LEVEL(ch) + 5))
  {
    send_to_char("You require more training before you can use that.\r\n", ch);
    return 0;
  }
  return 1;
}

void perform_put(struct char_data * ch, struct obj_data * obj, struct obj_data * cont)
{
  if (!drop_otrigger(obj, ch))
    return;
  if (GET_OBJ_WEIGHT(cont) + GET_OBJ_WEIGHT(obj) > GET_OBJ_VAL(cont, 0))
    act("$p won't fit in $P.", FALSE, ch, obj, cont, TO_CHAR);
  else {
    obj_from_char(obj);
    obj_to_obj(obj, cont);
    act("You put $p in $P.", FALSE, ch, obj, cont, TO_CHAR);
    act("$n puts $p in $P.", TRUE, ch, obj, cont, TO_ROOM);
  }
}


/* The following put modes are supported by the code below:

	1) put <object> <container>
	2) put all.<object> <container>
	3) put all <container>

	<container> must be in inventory or on ground.
	all objects to be put into container must be in inventory.
*/

ACMD(do_put)
{
  char arg1[MAX_INPUT_LENGTH];
  char arg2[MAX_INPUT_LENGTH];
  struct obj_data *obj, *next_obj, *cont;
  struct char_data *tmp_char;
  int obj_dotmode, cont_dotmode, found = 0;
  int put_num, amount;   /* Added by Darksword */

  two_arguments(argument, arg1, arg2);
  obj_dotmode = find_all_dots(arg1);
  cont_dotmode = find_all_dots(arg2);

  if (!*arg1)
    send_to_char("Put what in what?\r\n", ch);
  else if (cont_dotmode != FIND_INDIV)
   send_to_char("You can only put things into one container at a time.\r\n", ch);


  /* ADDED by Darksword */
  else if (is_number(arg1)) {
    put_num = atoi(arg1);
    argument = two_arguments(argument, arg1, arg2);
    one_argument(argument, arg1);
    if (!*arg2)
      send_to_char("Put what in what?\r\n", ch);
    else if (!*arg1) {
      sprintf(buf, "What do you want to put %s in?\r\n", arg2);
      send_to_char(buf, ch);
    } else {
    generic_find(arg1, FIND_OBJ_INV | FIND_OBJ_ROOM, ch, &tmp_char, &cont);
    if (!cont) {
      sprintf(buf, "You don't see %s %s here.\r\n", AN(arg1), arg1);
      send_to_char(buf, ch);
   } else if (GET_OBJ_TYPE(cont) != ITEM_CONTAINER && GET_OBJ_TYPE(cont) != ITEM_SHEATH)
      act("$p is not a container or a sheath.", FALSE, ch, cont, 0, TO_CHAR);
    else if (IS_SET(GET_OBJ_VAL(cont, 1), CONT_CLOSED))
      send_to_char("You'd better open it first!\r\n", ch);
    else {
      /* ADDED by Darksword put # <obj> <container> */
      for  (amount = 0; amount < put_num; amount++)
      if (!(obj = get_obj_in_list_vis(ch, arg2, ch->carrying))) {

sprintf(buf, "You aren't carrying %s %s.\r\n", AN(arg2), arg2);
        send_to_char(buf, ch);
        amount = put_num;
      } else if (obj == cont) {

  send_to_char("You attempt to fold it into itself, but fail.\r\n", ch);
          amount = put_num;
      }

else

  perform_put(ch, obj, cont);
    }
  }
  /* END of Darksword */

  } else if (!*arg2) {
    sprintf(buf, "What do you want to put %s in?\r\n",

    ((obj_dotmode == FIND_INDIV) ? "it" : "them"));
    send_to_char(buf, ch);
  } else {
    generic_find(arg2, FIND_OBJ_INV | FIND_OBJ_ROOM, ch, &tmp_char, &cont);
    if (!cont) {
      sprintf(buf, "You don't see %s %s here.\r\n", AN(arg2), arg2);
      send_to_char(buf, ch);
    } else if (GET_OBJ_TYPE(cont) != ITEM_CONTAINER && GET_OBJ_TYPE(cont) != ITEM_SHEATH)
      act("$p is not a container.", FALSE, ch, cont, 0, TO_CHAR);
    else if (IS_SET(GET_OBJ_VAL(cont, 1), CONT_CLOSED))
      send_to_char("You'd better open it first!\r\n", ch);
    else {
      if (obj_dotmode == FIND_INDIV) {
/* put <obj> <container> */

if (!(obj = get_obj_in_list_vis(ch, arg1, ch->carrying))) {

  sprintf(buf, "You aren't carrying %s %s.\r\n", AN(arg1), arg1);

  send_to_char(buf, ch);

} else if (obj == cont) {

  send_to_char("You attempt to fold it into itself, but fail.\r\n", ch);
}

else
  perform_put(ch, obj, cont);
      } else {

for (obj = ch->carrying; obj; obj = next_obj) {

  next_obj = obj->next_content;

  if (obj != cont && CAN_SEE_OBJ(ch, obj) &&

      (obj_dotmode == FIND_ALL || isname(arg1, obj->name))) {

    found = 1;

    perform_put(ch, obj, cont);

  }

}

if (!found) {

  if (obj_dotmode == FIND_ALL)

    send_to_char("You don't seem to have anything to put in it.\r\n", ch);

  else {

    sprintf(buf, "You don't seem to have any %ss.\r\n", arg1);

    send_to_char(buf, ch);

  }

}
      }
    }
  }
}

/* ACMD(do_put)
{
  char arg1[MAX_INPUT_LENGTH];
  char arg2[MAX_INPUT_LENGTH];
  struct obj_data *obj, *next_obj, *cont;
  struct char_data *tmp_char;
  int obj_dotmode, cont_dotmode, found = 0;
  two_arguments(argument, arg1, arg2);
  obj_dotmode = find_all_dots(arg1);
  cont_dotmode = find_all_dots(arg2);
  int put_num, amount;   

  if (!*arg1)
    send_to_char("Put what in what?\r\n", ch);
  else if (cont_dotmode != FIND_INDIV)
    send_to_char("You can only put things into one container at a time.\r\n", ch);
  else if (!*arg2) {
    sprintf(buf, "What do you want to put %s in?\r\n",
	    ((obj_dotmode == FIND_INDIV) ? "it" : "them"));
    send_to_char(buf, ch);
  } else {
    generic_find(arg2, FIND_OBJ_INV | FIND_OBJ_ROOM, ch, &tmp_char, &cont);
    if (!cont) {
      sprintf(buf, "You don't see %s %s here.\r\n", AN(arg2), arg2);
      send_to_char(buf, ch);
    } else if (GET_OBJ_TYPE(cont) != ITEM_CONTAINER && GET_OBJ_TYPE(cont) != ITEM_SHEATH)
      act("$p is not a container.", FALSE, ch, cont, 0, TO_CHAR);
    else if (IS_SET(GET_OBJ_VAL(cont, 1), CONT_CLOSED))
      send_to_char("You'd better open it first!\r\n", ch);
    else {
      if (obj_dotmode == FIND_INDIV) {	
	if (!(obj = get_obj_in_list_vis(ch, arg1, ch->carrying))) {
	  sprintf(buf, "You aren't carrying %s %s.\r\n", AN(arg1), arg1);
	  send_to_char(buf, ch);
	} else if (obj == cont)
	  send_to_char("You attempt to fold it into itself, but fail.\r\n", ch);
          else if (CAN_WEAR(cont, ITEM_WEAR_SHEATH) && !(cont->contains == NULL)) {
          send_to_char("There is already something sheathed in there!\r\n", ch);
          }
	else
	  perform_put(ch, obj, cont);
      } else {
	for (obj = ch->carrying; obj; obj = next_obj) {
	  next_obj = obj->next_content;
	  if (obj != cont && CAN_SEE_OBJ(ch, obj) &&
	      (obj_dotmode == FIND_ALL || isname(arg1, obj->name))) {
	    found = 1;
	    perform_put(ch, obj, cont);
	  }
	}
	if (!found) {
	  if (obj_dotmode == FIND_ALL)
	    send_to_char("You don't seem to have anything to put in it.\r\n", ch);
	  else {
	    sprintf(buf, "You don't seem to have any %ss.\r\n", arg1);
	    send_to_char(buf, ch);
	  }
	}
      }
    }
  }
} */


int can_take_obj(struct char_data * ch, struct obj_data * obj)
{
  /*  PDH  3/20/99
   *  no longer have Eq Level restrictions
   *
  if (GET_LEVEL(ch) < GET_OBJ_LEVEL(obj)) {
    act("$p: you are not powerfull enough to use this..",
	FALSE, ch, obj, 0, TO_CHAR);
    return 0;
   */

  if (IS_CARRYING_N(ch) >= CAN_CARRY_N(ch)) {
    act("$p: you can't carry that many items.", FALSE, ch, obj, 0, TO_CHAR);
    return 0;
  } else if ((IS_CARRYING_W(ch) + GET_OBJ_WEIGHT(obj)) > CAN_CARRY_W(ch)) {
    act("$p: you can't carry that much weight.", FALSE, ch, obj, 0, TO_CHAR);
    return 0;
  } else if (!(CAN_WEAR(obj, ITEM_WEAR_TAKE))) {
    act("$p: you can't take that!", FALSE, ch, obj, 0, TO_CHAR);
    return 0;
  }
  return 1;
}


void get_check_money(struct char_data * ch, struct obj_data * obj)
{
  struct money_data tmp;

  if ( (GET_OBJ_TYPE(obj) == ITEM_MONEY) &&
       ( (GET_OBJ_VAL(obj, 0) > 0) ||
	 (GET_OBJ_VAL(obj, 1) > 0) ||
	 (GET_OBJ_VAL(obj, 2) > 0) ||
	 (GET_OBJ_VAL(obj, 3) > 0) ) ) {
    obj_from_char(obj);

    tmp.platinum = GET_OBJ_VAL(obj, 0);
    tmp.steel    = GET_OBJ_VAL(obj, 1);
    tmp.gold     = GET_OBJ_VAL(obj, 2);
    tmp.copper   = GET_OBJ_VAL(obj, 3);

    if ( money_to_copper(tmp) > 0 ) {
      if ((money_to_copper(tmp) == 1) || (money_to_copper(tmp) == 10) ||
          (money_to_copper(tmp) == 100) || (money_to_copper(tmp) == 1000))
        sprintf(buf, "There was %s.\r\n", describe_money(tmp));
      else
        sprintf(buf, "There were %s.\r\n", describe_money(tmp));
      send_to_char(buf, ch);
    }

    (GET_MONEY(ch)).copper   += tmp.copper;
    (GET_MONEY(ch)).gold     += tmp.gold;
    (GET_MONEY(ch)).steel    += tmp.steel;
    (GET_MONEY(ch)).platinum += tmp.platinum;
    extract_obj(obj);
  }
}


void perform_get_from_container(struct char_data * ch, struct obj_data * obj, struct obj_data * cont, int mode)
{
  if (mode == FIND_OBJ_INV || can_take_obj(ch, obj)) {
    if (IS_CARRYING_N(ch) >= CAN_CARRY_N(ch)) {
      act("$p: you can't hold any more items.", FALSE, ch, obj, 0, TO_CHAR);
    } else if (get_otrigger(obj, ch)) {
      obj_from_obj(obj);

      if (GET_OBJ_LEVEL(obj) - GET_LEVEL(ch) > 5) {
        GET_NAME(ch, chname);

        sprintf(buf, "%s (%d) retrieved a powerful item (%s (%d)) from %s.",
                chname,
                GET_LEVEL(ch),
                obj->short_description,
                GET_OBJ_LEVEL(obj),
                cont->short_description);

        FREE_NAME(chname);
        mudlog(buf, BRF, MAX(LVL_GOD, GET_INVIS_LEV(ch)), TRUE);
      }

      obj_to_char(obj, ch);
      act("You get $p from $P.", FALSE, ch, obj, cont, TO_CHAR);
      act("$n gets $p from $P.", TRUE, ch, obj, cont, TO_ROOM);
      get_check_money(ch, obj);
    }
  }
}


void get_from_container(struct char_data * ch, struct obj_data * cont, char *arg, int mode) {
  struct obj_data *obj, *next_obj;
  int obj_dotmode, found = 0;

  obj_dotmode = find_all_dots(arg);

  if (IS_SET(GET_OBJ_VAL(cont, 1), CONT_CLOSED))
    act("$p is closed.", FALSE, ch, cont, 0, TO_CHAR);
  else if (obj_dotmode == FIND_INDIV) {
    if (!(obj = get_obj_in_list_vis(ch, arg, cont->contains))) {
      sprintf(buf, "There doesn't seem to be %s %s in $p.", AN(arg), arg);
      act(buf, FALSE, ch, cont, 0, TO_CHAR);
    } else
      perform_get_from_container(ch, obj, cont, mode);
  } else {
    if (obj_dotmode == FIND_ALLDOT && !*arg) {
      send_to_char("Get all of what?\r\n", ch);
      return;
    }
    for (obj = cont->contains; obj; obj = next_obj) {
      next_obj = obj->next_content;
      if (CAN_SEE_OBJ(ch, obj) &&
	  (obj_dotmode == FIND_ALL || isname(arg, obj->name))) {
	found = 1;
	perform_get_from_container(ch, obj, cont, mode);
      }
    }
    if (!found) {
      if (obj_dotmode == FIND_ALL)
	act("$p seems to be empty.", FALSE, ch, cont, 0, TO_CHAR);
      else {
	sprintf(buf, "You can't seem to find any %ss in $p.", arg);
	act(buf, FALSE, ch, cont, 0, TO_CHAR);
      }
    }
  }
}

int perform_get_from_room(struct char_data * ch, struct obj_data * obj)
{
  if (can_take_obj(ch, obj) && get_otrigger(obj, ch)) {
    obj_from_room(obj);

    if (GET_OBJ_LEVEL(obj) - GET_LEVEL(ch) > 5) {
      GET_NAME(ch, chname);

      sprintf(buf, "%s (%d) has picked up a powerful item (%s&g (%d)).",
              chname,
              GET_LEVEL(ch),
              obj->short_description,
              GET_OBJ_LEVEL(obj));

      FREE_NAME(chname);
      mudlog(buf, BRF, MAX(LVL_GOD, GET_INVIS_LEV(ch)), TRUE);
    }

    obj_to_char(obj, ch);
    act("You get $p.", FALSE, ch, obj, 0, TO_CHAR);
    act("$n gets $p.", TRUE, ch, obj, 0, TO_ROOM);
    get_check_money(ch, obj);
    return 1;
  }
  return 0;
}



void get_from_room(struct char_data * ch, char *arg)
{
  struct obj_data *obj, *next_obj;
  int dotmode, found = 0;

  dotmode = find_all_dots(arg);

  if (dotmode == FIND_INDIV) {
    if (!(obj = get_obj_in_list_vis(ch, arg, world[ch->in_room].contents))) {
      sprintf(buf, "You don't see %s %s here.\r\n", AN(arg), arg);
      send_to_char(buf, ch);
    } else
      perform_get_from_room(ch, obj);
  } else {
    if (dotmode == FIND_ALLDOT && !*arg) {
      send_to_char("Get all of what?\r\n", ch);
      return;
    }
    for (obj = world[ch->in_room].contents; obj; obj = next_obj) {
      next_obj = obj->next_content;
      if (CAN_SEE_OBJ(ch, obj) &&
         (dotmode == FIND_ALL || isname(arg, obj->name)) &&
         ((dotmode != FIND_ALL) || !IS_OBJ_STAT(obj, ITEM_HIDDEN)))
      {
	found = 1;
	perform_get_from_room(ch, obj);
      }
    }
    if (!found) {
      if (dotmode == FIND_ALL)
	send_to_char("There doesn't seem to be anything here.\r\n", ch);
      else {
	sprintf(buf, "You don't see any %ss here.\r\n", arg);
	send_to_char(buf, ch);
      }
    }
  }
}



ACMD(do_get)
{
  char arg1[MAX_INPUT_LENGTH];
  char arg2[MAX_INPUT_LENGTH];

  int cont_dotmode, found = 0, mode;
  struct obj_data *cont;
  struct char_data *tmp_char;

  two_arguments(argument, arg1, arg2);

  if (IS_CARRYING_N(ch) >= CAN_CARRY_N(ch))
    send_to_char("Your arms are already full!\r\n", ch);
  else if (!*arg1)
    send_to_char("Get what?\r\n", ch);
  else if (!*arg2)
    get_from_room(ch, arg1);
  else {
    cont_dotmode = find_all_dots(arg2);
    if (cont_dotmode == FIND_INDIV) {
      mode = generic_find(arg2, FIND_OBJ_INV | FIND_OBJ_ROOM, ch, &tmp_char, &cont);
      if (!cont) {
	sprintf(buf, "You don't have %s %s.\r\n", AN(arg2), arg2);
	send_to_char(buf, ch);
      } else if (GET_OBJ_TYPE(cont) != ITEM_CONTAINER && GET_OBJ_TYPE(cont) != ITEM_SHEATH)
	act("$p is not a container.", FALSE, ch, cont, 0, TO_CHAR);
      else
	get_from_container(ch, cont, arg1, mode);
    } else {
      if (cont_dotmode == FIND_ALLDOT && !*arg2) {
	send_to_char("Get from all of what?\r\n", ch);
	return;
      }
      for (cont = ch->carrying; cont; cont = cont->next_content)
	if (CAN_SEE_OBJ(ch, cont) &&
	    (cont_dotmode == FIND_ALL || isname(arg2, cont->name)))
        {
	  if (GET_OBJ_TYPE(cont) == ITEM_CONTAINER || GET_OBJ_TYPE(cont) == ITEM_SHEATH) {
	    found = 1;
	    get_from_container(ch, cont, arg1, FIND_OBJ_INV);
	  } else if (cont_dotmode == FIND_ALLDOT) {
	    found = 1;
	    act("$p is not a container.", FALSE, ch, cont, 0, TO_CHAR);
	  }
        }
      for (cont = world[ch->in_room].contents; cont; cont = cont->next_content)
	if (CAN_SEE_OBJ(ch, cont) &&
	    (cont_dotmode == FIND_ALL || isname(arg2, cont->name)))
        {
	  if (GET_OBJ_TYPE(cont) == ITEM_CONTAINER || GET_OBJ_TYPE(cont) == ITEM_SHEATH) {
	    get_from_container(ch, cont, arg1, FIND_OBJ_ROOM);
	    found = 1;
	  } else if (cont_dotmode == FIND_ALLDOT) {
	    act("$p is not a container.", FALSE, ch, cont, 0, TO_CHAR);
	    found = 1;
	  }
        }
      if (!found) {
	if (cont_dotmode == FIND_ALL)
	  send_to_char("You can't seem to find any containers.\r\n", ch);
	else {
	  sprintf(buf, "You can't seem to find any %ss here.\r\n", arg2);
	  send_to_char(buf, ch);
	}
      }
    }
  }
}


void perform_drop_gold(struct char_data * ch, struct money_data amount,
		            byte mode, sh_int RDR)
{
  struct obj_data *obj;
  int coins;


  if (money_to_copper(amount) <= 0) {
    send_to_char("Heh heh heh.. we are jolly funny today, eh?\r\n", ch);

  } else if ( money_compare(GET_MONEY(ch), amount, TRUE) < 0 ) {
    send_to_char("You don't have that many coins!\r\n", ch);

  } else {
    coins = amount.platinum + amount.steel + amount.gold + amount.copper;

      WAIT_STATE(ch, PULSE_VIOLENCE);	/* to prevent coin-bombing */
      obj = create_money(amount);
        if (!drop_wtrigger(obj, ch)) {
          extract_obj(obj);
          return;
        }
	send_to_char("You drop some coins.\r\n", ch);
	sprintf(buf, "$n drops %s.", money_desc(coins));
	act(buf, TRUE, ch, 0, 0, TO_ROOM);
	obj_to_room(obj, ch->in_room);

    (GET_MONEY(ch)).platinum -= amount.platinum;
    (GET_MONEY(ch)).steel -= amount.steel;
    (GET_MONEY(ch)).gold -= amount.gold;
    (GET_MONEY(ch)).copper -= amount.copper;

  }
}



int perform_drop(struct char_data * ch, struct obj_data * obj,
                     byte mode, char *sname, sh_int RDR)
{

  if (!drop_otrigger(obj, ch))
    return 0;
  if ((mode == SCMD_DROP) && !drop_wtrigger(obj, ch))
    return 0;

  if (IS_OBJ_STAT(obj, ITEM_NODROP)) {
    sprintf(buf, "You can't %s $p, it must be CURSED!", sname);
    act(buf, FALSE, ch, obj, 0, TO_CHAR);
    return 0;
  }
  sprintf(buf, "You %s $p.", sname);
  act(buf, FALSE, ch, obj, 0, TO_CHAR);
  sprintf(buf, "$n %ss $p.", sname);
  act(buf, TRUE, ch, obj, 0, TO_ROOM);
  obj_from_char(obj);

  switch (mode) {
  case SCMD_DROP:
    obj_to_room(obj, ch->in_room);
    return 0;
    break;
  default:
    log("SYSERR: Incorrect argument passed to perform_drop");
    break;
  }

  return 0;
}


ACMD(do_drop)
{
  struct obj_data *obj, *next_obj;
  sh_int RDR = 0;
  byte mode = SCMD_DROP;
  int dotmode, reward = 0;
  struct money_data amount;
  char *sname = "drop";


  amount.platinum = amount.steel = amount.gold = amount.copper = 0;


  argument = one_argument(argument, arg);

  if (!*arg) {
    sprintf(buf, "What do you want to %s?\r\n", sname);
    send_to_char(buf, ch);
    return;


  } else if (is_money(arg)) {
    struct money_data tmpAmt;

    tmpAmt.platinum = tmpAmt.steel = tmpAmt.gold = tmpAmt.copper = 0;
    amount.platinum = amount.steel = amount.gold = amount.copper = 0;

    /*
     *  parse out  #p #g #s #c  for the amount
     */
    value_money(&tmpAmt, arg);
    amount.platinum += tmpAmt.platinum;    amount.gold += tmpAmt.gold;
    amount.steel += tmpAmt.steel;    amount.copper += tmpAmt.copper;

    argument = one_argument(argument, arg);
    while ( (*arg) ) {
      /*  as long as it's money, keep adding...  */
      if (is_money(arg)) {
	tmpAmt.platinum = tmpAmt.steel = tmpAmt.gold = tmpAmt.copper = 0;
	value_money(&tmpAmt, arg);
	amount.platinum += tmpAmt.platinum;    amount.gold += tmpAmt.gold;
	amount.steel += tmpAmt.steel;    amount.copper += tmpAmt.copper;
      } else {
	/*  trying money and something else...  */
	send_to_char("Sorry, you can't do that to money and something else at the same time.\r\n", ch);
	return;
      }
      argument = one_argument(argument, arg);
    }

    perform_drop_gold(ch, amount, mode, RDR);

    return;



  } else {
    dotmode = find_all_dots(arg);

    if (dotmode == FIND_ALL) {
      if (!ch->carrying)
	send_to_char("You don't seem to be carrying anything.\r\n", ch);
      else
	for (obj = ch->carrying; obj; obj = next_obj) {
	  next_obj = obj->next_content;
	  reward += perform_drop(ch, obj, mode, sname, RDR);
	}
    } else if (dotmode == FIND_ALLDOT) {
      if (!*arg) {
	sprintf(buf, "What do you want to %s all of?\r\n", sname);
	send_to_char(buf, ch);
	return;
      }
      if (!(obj = get_obj_in_list_vis(ch, arg, ch->carrying))) {
	sprintf(buf, "You don't seem to have any %ss.\r\n", arg);
	send_to_char(buf, ch);
      }
      while (obj) {
	next_obj = get_obj_in_list_vis(ch, arg, obj->next_content);
	reward += perform_drop(ch, obj, mode, sname, RDR);
	obj = next_obj;
      }
    } else {
      if (!(obj = get_obj_in_list_vis(ch, arg, ch->carrying))) {
	sprintf(buf, "You don't seem to have %s %s.\r\n", AN(arg), arg);
	send_to_char(buf, ch);
      } else
	reward += perform_drop(ch, obj, mode, sname, RDR);
    }
  }

  if (reward) {
    send_to_char("You have been rewarded by the gods!\r\n", ch);
    act("$n has been rewarded by the gods!", TRUE, ch, 0, 0, TO_ROOM);
    (GET_MONEY(ch)).copper += reward;
  }
}

void wear_spells(struct char_data *ch, struct obj_data *item)
{
  int i = 0, k = 0, rndm;
  //int j = 0;

  struct affected_type *af, *next;

  if(item&&HAS_SPELLS(item)) {
    while(GET_WEAPON_SPELL(item,i)&&i<MAX_SPELL_AFFECT) {

      /* Do some sanity checking, in case someone flees, etc. */
      /* Can't have enough of 'em.  */

      rndm=number(1,100);
      if(GET_WEAPON_SPELL_PCT(item,i)>=rndm) {
        if(call_magic(ch,ch,NULL,GET_WEAPON_SPELL(item,i),GET_WEAPON_SPELL_LVL(item,i),CAST_WAND)<0) 
return;

for (af = ch->affected; af; af = next)
    {
      next = af->next;

      if (GET_WEAPON_SPELL(item,k) == af->type){
        af->duration = -1;
       }
      k++;
  }

      }
      i++;
    }
  }
}

 
void perform_give(struct char_data * ch, struct char_data * vict, struct obj_data * obj) {
  if (IS_OBJ_STAT(obj, ITEM_NODROP)) {
    if (GET_LEVEL(ch) < LVL_IMMORT)
    {
      act("You can't let go of $p!!  Yeech!", FALSE, ch, obj, 0, TO_CHAR);
      return;
    }
  }
  if (IS_CARRYING_N(vict) >= CAN_CARRY_N(vict)) {
    act("$N seems to have $S hands full.", FALSE, ch, 0, vict, TO_CHAR);
    return;
  }

  /* If an item is more than five item levels higher than
   * the intended recipient, prevent it from being given.
   */
  //if (GET_OBJ_LEVEL(obj) > GET_LEVEL(vict)) {
  if (GET_OBJ_LEVEL(obj) - GET_LEVEL(vict) > 5) {
    //act("$N seems to have $S level low.", FALSE, ch, 0, vict, TO_CHAR);
    act("You are not allowed to give $N this item.", FALSE, ch, 0, vict, TO_CHAR);
    GET_NAME(ch, chname);
    GET_NAME(vict, victname);

    sprintf(buf, "%s (%d) attempted to give a powerful item (%s (%d)) to %s (%d).",
            chname,
            GET_LEVEL(ch),
            obj->short_description,
            GET_OBJ_LEVEL(obj),
            victname,
            GET_LEVEL(vict));

    FREE_NAME(victname);
    FREE_NAME(chname);
    mudlog(buf, BRF, MAX(LVL_GOD, GET_INVIS_LEV(ch)), TRUE);
    return;
  }

  if (GET_OBJ_WEIGHT(obj) + IS_CARRYING_W(vict) > CAN_CARRY_W(vict)) {
    act("$E can't carry that much weight.", FALSE, ch, 0, vict, TO_CHAR);
    return;
  }
  if (!give_otrigger(obj, ch, vict) || !receive_mtrigger(vict, ch, obj))
    return;

  if (IS_NPC(ch) && MOB_FLAGGED(ch, MOB_MOUNTABLE) && CAN_WEAR(obj, ITEM_WEAR_ONBACK)) {
      obj_from_char(obj);
      obj_to_char(obj, vict);
      act("$N retrieves $p from your back.", FALSE, ch, obj, vict, TO_CHAR);
      act("You retrieve $p from the back of $n.", FALSE, ch, obj, vict, TO_VICT);
      act("$N retrieves $p from the back of $n.", TRUE, ch, obj, vict, TO_NOTVICT);
  }

 if (IS_NPC(vict) && MOB_FLAGGED(vict, MOB_MOUNTABLE) && CAN_WEAR(obj, ITEM_WEAR_ONBACK)) {
      obj_from_char(obj);
      obj_to_char(obj, vict);
      act("You put $p across $N's back.", FALSE, ch, obj, vict, TO_CHAR);
      act("$n puts $p across your back.", FALSE, ch, obj, vict, TO_VICT)
;
      act("$n puts $p across $N's back.", TRUE, ch, obj, vict, TO_NOTVICT);
  }


  else {
  obj_from_char(obj);
  obj_to_char(obj, vict);
  act("You give $p to $N.", FALSE, ch, obj, vict, TO_CHAR);
  act("$n gives you $p.", FALSE, ch, obj, vict, TO_VICT);
  act("$n gives $p to $N.", TRUE, ch, obj, vict, TO_NOTVICT);

}
}

/* utility function for give */
struct char_data *give_find_vict(struct char_data * ch, char *arg)
{
  struct char_data *vict;

  if (!*arg) {
    send_to_char("To who?\r\n", ch);
    return NULL;
  } else if (!(vict = get_char_vis(ch, arg, FIND_CHAR_ROOM))) {
    send_to_char(NOPERSON, ch);
    return NULL;
  } else if (vict == ch) {
    send_to_char("What's the point of that?\r\n", ch);
    return NULL;
  } else
    return vict;
}


void perform_give_gold(struct char_data * ch, struct char_data * vict, struct money_data amount) {
  int coins;

  if ( money_to_copper(amount) <= 0) {
    send_to_char("Heh heh heh ... we are jolly funny today, eh?\r\n", ch);
    return;
  }

  if ( (money_compare(GET_MONEY(ch), amount, TRUE) < 0) &&
       ( IS_NPC(ch) || (GET_LEVEL(ch) < LVL_GOD) ) ) {
    send_to_char("You don't have that many coins!\r\n", ch);
    return;
  }

  coins = amount.platinum + amount.steel + amount.gold + amount.copper;

  send_to_char(OK, ch);
  sprintf(buf, "$n gives you %s.", describe_money(amount));
  act(buf, FALSE, ch, 0, vict, TO_VICT);
  sprintf(buf, "$n gives %s to $N.", money_desc(coins));
  act(buf, TRUE, ch, 0, vict, TO_NOTVICT);

  if (IS_NPC(ch) || (GET_LEVEL(ch) < LVL_GOD)) {
    (GET_MONEY(ch)).platinum -= amount.platinum;
    (GET_MONEY(ch)).steel -= amount.steel;
    (GET_MONEY(ch)).gold -= amount.gold;
    (GET_MONEY(ch)).copper -= amount.copper;
  }

  (GET_MONEY(vict)).platinum += amount.platinum;
  (GET_MONEY(vict)).steel += amount.steel;
  (GET_MONEY(vict)).gold += amount.gold;
  (GET_MONEY(vict)).copper += amount.copper;

  bribe_mtrigger(vict, ch, money_to_copper(amount));
}


ACMD(do_give)
{
  int dotmode;
  struct money_data amount;
  struct char_data *vict;
  struct obj_data *obj, *next_obj;

  argument = one_argument(argument, arg);
  amount.platinum = amount.steel = amount.gold = amount.copper = 0;

  if (!*arg) {
    send_to_char("Give what to who?\r\n", ch);

  } else if (is_money(arg)) {
    struct money_data tmpAmt;

    tmpAmt.platinum = tmpAmt.steel = tmpAmt.gold = tmpAmt.copper = 0;
    amount.platinum = amount.steel = amount.gold = amount.copper = 0;

    /*
     *  parse out  #p #g #s #c  for the amount
     */
    value_money(&tmpAmt, arg);
    amount.platinum += tmpAmt.platinum;    amount.gold += tmpAmt.gold;
    amount.steel += tmpAmt.steel;    amount.copper += tmpAmt.copper;

    argument = one_argument(argument, arg);
    while ( (*arg) ) {
      /*  as long as it's money, keep adding...  */
      if (is_money(arg)) {
	tmpAmt.platinum = tmpAmt.steel = tmpAmt.gold = tmpAmt.copper = 0;
	value_money(&tmpAmt, arg);
	amount.platinum += tmpAmt.platinum;    amount.gold += tmpAmt.gold;
	amount.steel += tmpAmt.steel;    amount.copper += tmpAmt.copper;

      } else if ((vict = give_find_vict(ch, arg))) {
	perform_give_gold(ch, vict, amount);
	return;

      } else {
	/*  trying money and something else...  */
	send_to_char("Sorry, you can't do that to money and something else at the same time.\r\n", ch);
	return;
      }
      argument = one_argument(argument, arg);
    }

    send_to_char("Give to who?\r\n", ch);
    return;


    /*
      } else if (is_number(arg)) {
      amount = atoi(arg);
      argument = one_argument(argument, arg);
      if (!str_cmp("coins", arg) || !str_cmp("coin", arg)) {
      argument = one_argument(argument, arg);
      if ((vict = give_find_vict(ch, arg)))
      perform_give_gold(ch, vict, amount);
      return;
      } else {
      // code to give multiple items.  anyone want to write it? -je
      send_to_char("You can't give more than one item at a time.\r\n", ch);
      return;
      }
      */

  } else {
    one_argument(argument, buf1);

    if (!(vict = give_find_vict(ch, buf1)))
      return;
    dotmode = find_all_dots(arg);

    if (dotmode == FIND_INDIV) {
      if (!(obj = get_obj_in_list_vis(ch, arg, ch->carrying))) {
	sprintf(buf, "You don't seem to have %s %s.\r\n", AN(arg), arg);
	send_to_char(buf, ch);
      } else
	perform_give(ch, vict, obj);
    } else {
      if (dotmode == FIND_ALLDOT && !*arg) {
	send_to_char("All of what?\r\n", ch);
	return;
      }
      if (!ch->carrying)
	send_to_char("You don't seem to be holding anything.\r\n", ch);
      else
	for (obj = ch->carrying; obj; obj = next_obj) {
	  next_obj = obj->next_content;
	  if (CAN_SEE_OBJ(ch, obj) &&
	      ((dotmode == FIND_ALL || isname(arg, obj->name))))
	    perform_give(ch, vict, obj);
	}
    }
  }
}


void weight_change_object(struct obj_data * obj, int weight) {
  struct obj_data *tmp_obj;
  struct char_data *tmp_ch;

  if (obj->in_room != NOWHERE) {
    GET_OBJ_WEIGHT(obj) += weight;
  } else if ((tmp_ch = obj->carried_by)) {
    obj_from_char(obj);
    GET_OBJ_WEIGHT(obj) += weight;
    obj_to_char(obj, tmp_ch);
  } else if ((tmp_obj = obj->in_obj)) {
    obj_from_obj(obj);
    GET_OBJ_WEIGHT(obj) += weight;
    obj_to_obj(obj, tmp_obj);
  } else {
    log("SYSERR: Unknown attempt to subtract weight from an object.");
  }
}


void name_from_drinkcon(struct obj_data * obj) {
  int i;
  char *new_name;
  extern struct obj_data *obj_proto;

  for (i = 0; (*((obj->name) + i) != ' ') && (*((obj->name) + i) != '\0'); i++);

  if (*((obj->name) + i) == ' ') {
    new_name = str_dup((obj->name) + i + 1);
    if (GET_OBJ_RNUM(obj) < 0 || obj->name != obj_proto[GET_OBJ_RNUM(obj)].name)
    {
      free(obj->name);
      obj->name = (char*) NULL;
    }
    obj->name = new_name;
  }
}


void name_to_drinkcon(struct obj_data * obj, int type) {
  char *new_name;
  extern struct obj_data *obj_proto;
  extern char *drinknames[];

  CREATE(new_name, char, strlen(obj->name) + strlen(drinknames[type]) + 2);
  sprintf(new_name, "%s %s", drinknames[type], obj->name);
  if (GET_OBJ_RNUM(obj) < 0 || obj->name != obj_proto[GET_OBJ_RNUM(obj)].name)
  {
    free(obj->name);
    obj->name = (char*) NULL;
  }
  obj->name = new_name;
}


ACMD(do_drink) {
  struct obj_data *temp;
  struct affected_type af;
  int amount, weight;
  int on_ground = 0;

  one_argument(argument, arg);

  if (!*arg) {
    send_to_char("Drink from what?\r\n", ch);
    return;
  }

  if (!(temp = get_obj_in_list_vis(ch, arg, ch->carrying))) {
    if (!(temp = get_obj_in_list_vis(ch, arg, world[ch->in_room].contents))) {
      act("You can't find it!", FALSE, ch, 0, 0, TO_CHAR);
      return;
    } else
      on_ground = 1;
  }

  if ((GET_OBJ_TYPE(temp) != ITEM_DRINKCON) &&
      (GET_OBJ_TYPE(temp) != ITEM_FOUNTAIN)) {
    send_to_char("You can't drink from that!\r\n", ch);
    return;
  }

  if (on_ground && (GET_OBJ_TYPE(temp) == ITEM_DRINKCON)) {
    send_to_char("You have to be holding that to drink from it.\r\n", ch);
    return;
  }

  if ((GET_COND(ch, DRUNK) > 10) && (GET_COND(ch, THIRST) > 0)) {
    /* The pig is drunk */
    send_to_char("You can't seem to get close enough to your mouth.\r\n", ch);
    act("$n tries to drink but misses $s mouth!", TRUE, ch, 0, 0, TO_ROOM);
    return;
  }

  if (GET_COND(ch, THIRST) > 20) {
    send_to_char("You aren't even thirsty!\r\n", ch);
    return;
  }

  if ((GET_COND(ch, FULL) > 20) && (int) drink_aff[GET_OBJ_VAL(temp, 2)][FULL])
  {
    send_to_char("Your stomach is too full to drink that!\r\n", ch);
    return;
  }

  if (!GET_OBJ_VAL(temp, 1)) {
    send_to_char("It's empty.\r\n", ch);
    return;
  }

  if (subcmd == SCMD_DRINK) {
    sprintf(buf, "$n drinks %s from $p.", drinks[GET_OBJ_VAL(temp, 2)]);
    act(buf, TRUE, ch, temp, 0, TO_ROOM);

    sprintf(buf, "You drink the %s.\r\n", drinks[GET_OBJ_VAL(temp, 2)]);
    send_to_char(buf, ch);

    if (drink_aff[GET_OBJ_VAL(temp, 2)][DRUNK] > 0)
      amount = (25 - GET_COND(ch, THIRST)) / drink_aff[GET_OBJ_VAL(temp, 2)][DRUNK];
    else
      amount = number(3, 10);

  } else {
    act("$n sips from $p.", TRUE, ch, temp, 0, TO_ROOM);
    sprintf(buf, "It tastes like %s.\r\n", drinks[GET_OBJ_VAL(temp, 2)]);
    send_to_char(buf, ch);
    amount = 1;
  }

  amount = MIN(amount, GET_OBJ_VAL(temp, 1));

  /* You can't subtract more than the object weighs */
  weight = MIN(amount, GET_OBJ_WEIGHT(temp));

  weight_change_object(temp, -weight);	/* Subtract amount */

  gain_condition(ch, DRUNK,
	 (int) ((int) drink_aff[GET_OBJ_VAL(temp, 2)][DRUNK] * amount) / 4);

  gain_condition(ch, FULL,
	  (int) ((int) drink_aff[GET_OBJ_VAL(temp, 2)][FULL] * amount) / 4);

  gain_condition(ch, THIRST,
	(int) ((int) drink_aff[GET_OBJ_VAL(temp, 2)][THIRST] * amount) / 4);

  if (GET_COND(ch, DRUNK) > 10)
    send_to_char("You feel drunk.\r\n", ch);

  if (GET_COND(ch, THIRST) > 20)
    send_to_char("You don't feel thirsty any more.\r\n", ch);

  if (GET_COND(ch, FULL) > 20)
    send_to_char("You are full.\r\n", ch);

  if (GET_OBJ_VAL(temp, 3)) {	/* The shit was poisoned ! */
    send_to_char("Oops, it tasted rather strange!\r\n", ch);
    act("$n chokes and utters some strange sounds.", TRUE, ch, 0, 0, TO_ROOM);

    af.type = SPELL_POISON;
    af.duration = amount * 3;
    af.modifier = 0;
    af.location = APPLY_NONE;
    af.bitvector = AFF_POISON;
    affect_join(ch, &af, FALSE, FALSE, FALSE, FALSE);
  }

  /* empty the container, and no longer poison. */
  GET_OBJ_VAL(temp, 1) -= amount;
  if (!GET_OBJ_VAL(temp, 1)) {	/* The last bit */
    GET_OBJ_VAL(temp, 2) = 0;
    GET_OBJ_VAL(temp, 3) = 0;
    /*  PDH  4/27/99 - why remove the name ?  */
    /* name_from_drinkcon(temp); */
  }
  return;
}


ACMD(do_eat) {
  struct obj_data *food;
  struct affected_type af;
  int amount, luck;

  one_argument(argument, arg);

  if (!*arg) {
    send_to_char("Eat what?\r\n", ch);
    return;
  }
  if (!(food = get_obj_in_list_vis(ch, arg, ch->carrying))) {
    sprintf(buf, "You don't seem to have %s %s.\r\n", AN(arg), arg);
    send_to_char(buf, ch);
    return;
  }
  if (subcmd == SCMD_TASTE && ((GET_OBJ_TYPE(food) == ITEM_DRINKCON) ||
			       (GET_OBJ_TYPE(food) == ITEM_FOUNTAIN))) {
    do_drink(ch, argument, 0, SCMD_SIP);
    return;
  }
  if (GET_OBJ_TYPE(food) != ITEM_FOOD && GET_OBJ_TYPE(food) != ITEM_RAW && GET_LEVEL(ch) < LVL_IMMORT) {
    send_to_char("You can't eat THAT!\r\n", ch);
    return;
  }
  if (GET_COND(ch, FULL) > 20) {/* Stomach full */
    act("You are too full to eat more!", FALSE, ch, 0, 0, TO_CHAR);
    return;
  }
  if (subcmd == SCMD_EAT) {
    act("You eat the $o.", FALSE, ch, food, 0, TO_CHAR);
    act("$n eats $p.", TRUE, ch, food, 0, TO_ROOM);
  } else {
    act("You nibble a little bit of the $o.", FALSE, ch, food, 0, TO_CHAR);
    act("$n tastes a little bit of $p.", TRUE, ch, food, 0, TO_ROOM);
  }

  amount = (subcmd == SCMD_EAT ? GET_OBJ_VAL(food, 0) : 1);

  gain_condition(ch, FULL, amount);

  if (GET_COND(ch, FULL) > 20)
    act("You are full.", FALSE, ch, 0, 0, TO_CHAR);

  if (GET_OBJ_TYPE(food) == ITEM_RAW && (GET_RACE(ch) != RACE_MINOTAUR)) {
    luck = number(1,10);
   if (luck >= 6) {
    send_to_char("Oops, that tasted rather strange!\r\n", ch);
    act("$n coughs and utters some strange sounds.", FALSE, ch, 0, 0, TO_ROOM);

    af.type = SPELL_POISON;
    af.duration = amount * 2;
    af.modifier = 0;
    af.location = APPLY_NONE;
    af.bitvector = AFF_POISON;
    affect_join(ch, &af, FALSE, FALSE, FALSE, FALSE);
   }

 else if (luck >=4) {
    send_to_char("You feel your stomach churn at the taste..bending down, you puke violently!\r\n", ch);
    act("$n suddenly turns green and bends down, puking violently! ", FALSE, ch, 0, 0, TO_ROOM);
    damage(ch, ch, dice(1,6), -1);
  }
 
 else {
  act("Your stomach roils at the taste of $o.", FALSE, ch, food, 0, TO_CHAR);
  act("$n turns slightly green at the taste of $p.", TRUE, ch, food, 0, TO_ROOM);
}
}

  if (GET_OBJ_VAL(food, 3) && (GET_LEVEL(ch) < LVL_IMMORT)) {
    /* The shit was poisoned ! */
    send_to_char("Oops, that tasted rather strange!\r\n", ch);
    act("$n coughs and utters some strange sounds.", FALSE, ch, 0, 0, TO_ROOM);

    af.type = SPELL_POISON;
    af.duration = amount * 2;
    af.modifier = 0;
    af.location = APPLY_NONE;
    af.bitvector = AFF_POISON;
    affect_join(ch, &af, FALSE, FALSE, FALSE, FALSE);
  }
  if (subcmd == SCMD_EAT)
    extract_obj(food);
  else {
    if (!(--GET_OBJ_VAL(food, 0))) {
      send_to_char("There's nothing left now.\r\n", ch);
      extract_obj(food);
    }
  }
}


ACMD(do_pour) {
  char arg1[MAX_INPUT_LENGTH];
  char arg2[MAX_INPUT_LENGTH];
  struct obj_data *from_obj = NULL, *to_obj = NULL;
  int amount;

  two_arguments(argument, arg1, arg2);

  if (subcmd == SCMD_POUR) {
    if (!*arg1) {		/* No arguments */
      act("From what do you want to pour?", FALSE, ch, 0, 0, TO_CHAR);
      return;
    }
    if (!(from_obj = get_obj_in_list_vis(ch, arg1, ch->carrying))) {
      act("You can't find it!", FALSE, ch, 0, 0, TO_CHAR);
      return;
    }
    if (GET_OBJ_TYPE(from_obj) != ITEM_DRINKCON) {
      act("You can't pour from that!", FALSE, ch, 0, 0, TO_CHAR);
      return;
    }
  }
  if (subcmd == SCMD_FILL) {
    if (!*arg1) {		/* no arguments */
      send_to_char("What do you want to fill?  And what are you filling it from?\r\n", ch);
      return;
    }
    if (!(to_obj = get_obj_in_list_vis(ch, arg1, ch->carrying))) {
      send_to_char("You can't find it!", ch);
      return;
    }
    if (GET_OBJ_TYPE(to_obj) != ITEM_DRINKCON) {
      act("You can't fill $p!", FALSE, ch, to_obj, 0, TO_CHAR);
      return;
    }
    if (!*arg2) {		/* no 2nd argument */
      act("What do you want to fill $p from?", FALSE, ch, to_obj, 0, TO_CHAR);
      return;
    }
    if (!(from_obj = get_obj_in_list_vis(ch, arg2, world[ch->in_room].contents))) {
      sprintf(buf, "There doesn't seem to be %s %s here.\r\n", AN(arg2), arg2);
      send_to_char(buf, ch);
      return;
    }
    if (GET_OBJ_TYPE(from_obj) != ITEM_FOUNTAIN) {
      act("You can't fill something from $p.", FALSE, ch, from_obj, 0, TO_CHAR);
      return;
    }
  }
  if (GET_OBJ_VAL(from_obj, 1) == 0) {
    act("The $p is empty.", FALSE, ch, from_obj, 0, TO_CHAR);
    return;
  }
  if (subcmd == SCMD_POUR) {	/* pour */
    if (!*arg2) {
      act("Where do you want it?  Out or in what?", FALSE, ch, 0, 0, TO_CHAR);
      return;
    }
    if (!str_cmp(arg2, "out")) {
      act("$n empties $p.", TRUE, ch, from_obj, 0, TO_ROOM);
      act("You empty $p.", FALSE, ch, from_obj, 0, TO_CHAR);

      weight_change_object(from_obj, -GET_OBJ_VAL(from_obj, 1)); /* Empty */

      GET_OBJ_VAL(from_obj, 1) = 0;
      GET_OBJ_VAL(from_obj, 2) = 0;
      GET_OBJ_VAL(from_obj, 3) = 0;
      /*  PDH  4/27/99 - why remove the name ?  */
      /* name_from_drinkcon(from_obj); */

      return;
    }
    if (!(to_obj = get_obj_in_list_vis(ch, arg2, ch->carrying))) {
      act("You can't find it!", FALSE, ch, 0, 0, TO_CHAR);
      return;
    }
    if ((GET_OBJ_TYPE(to_obj) != ITEM_DRINKCON) &&
	(GET_OBJ_TYPE(to_obj) != ITEM_FOUNTAIN)) {
      act("You can't pour anything into that.", FALSE, ch, 0, 0, TO_CHAR);
      return;
    }
  }
  if (to_obj == from_obj) {
    act("A most unproductive effort.", FALSE, ch, 0, 0, TO_CHAR);
    return;
  }
  if ((GET_OBJ_VAL(to_obj, 1) != 0) &&
      (GET_OBJ_VAL(to_obj, 2) != GET_OBJ_VAL(from_obj, 2))) {
    act("There is already another liquid in it!", FALSE, ch, 0, 0, TO_CHAR);
    return;
  }
  if (!(GET_OBJ_VAL(to_obj, 1) < GET_OBJ_VAL(to_obj, 0))) {
    act("There is no room for more.", FALSE, ch, 0, 0, TO_CHAR);
    return;
  }
  if (subcmd == SCMD_POUR) {
    sprintf(buf, "You pour the %s into the %s.\r\n",
	    drinks[GET_OBJ_VAL(from_obj, 2)], arg2);
    send_to_char(buf, ch);
  }
  if (subcmd == SCMD_FILL) {
    act("You gently fill $p from $P.", FALSE, ch, to_obj, from_obj, TO_CHAR);
    act("$n gently fills $p from $P.", TRUE, ch, to_obj, from_obj, TO_ROOM);
  }
  /* New alias */
  if (GET_OBJ_VAL(to_obj, 1) == 0)
    name_to_drinkcon(to_obj, GET_OBJ_VAL(from_obj, 2));

  /* First same type liq. */
  GET_OBJ_VAL(to_obj, 2) = GET_OBJ_VAL(from_obj, 2);

  /* Then how much to pour */
  GET_OBJ_VAL(from_obj, 1) -= (amount =
			 (GET_OBJ_VAL(to_obj, 0) - GET_OBJ_VAL(to_obj, 1)));

  GET_OBJ_VAL(to_obj, 1) = GET_OBJ_VAL(to_obj, 0);

  if (GET_OBJ_VAL(from_obj, 1) < 0) {	/* There was too little */
    GET_OBJ_VAL(to_obj, 1) += GET_OBJ_VAL(from_obj, 1);
    amount += GET_OBJ_VAL(from_obj, 1);
    GET_OBJ_VAL(from_obj, 1) = 0;
    GET_OBJ_VAL(from_obj, 2) = 0;
    GET_OBJ_VAL(from_obj, 3) = 0;
    /*  PDH  4/27/99 - why remove the name ?  */
    /* name_from_drinkcon(from_obj); */
  }
  /* Then the poison boogie */
  GET_OBJ_VAL(to_obj, 3) =
    (GET_OBJ_VAL(to_obj, 3) || GET_OBJ_VAL(from_obj, 3));

  /* And the weight boogie */
  weight_change_object(from_obj, -amount);
  weight_change_object(to_obj, amount);	/* Add weight */

  return;
}


void wear_message(struct char_data * ch, struct obj_data * obj, int where) {
  char *wear_messages[][2] = {
    {"$n lights $p and holds it.",
    "You light $p and hold it."},

    {"$n slides $p on to $s right ring finger.",
    "You slide $p on to your right ring finger."},

    {"$n slides $p on to $s left ring finger.",
    "You slide $p on to your left ring finger."},

    {"$n wears $p around $s neck.",
    "You wear $p around your neck."},

    {"$n wears $p around $s neck.",
    "You wear $p around your neck."},

    {"$n wears $p on $s body.",
    "You wear $p on your body.",},

    {"$n wears $p on $s head.",
    "You wear $p on your head."},

    {"$n puts $p on $s legs.",
    "You put $p on your legs."},

    {"$n wears $p on $s feet.",
    "You wear $p on your feet."},

    {"$n puts $p on $s hands.",
    "You put $p on your hands."},

    {"$n wears $p on $s arms.",
    "You wear $p on your arms."},

    {"$n straps $p around $s arm as a shield.",
    "You start to use $p as a shield."},

    {"$n wears $p about $s body.",
    "You wear $p around your body."},

    {"$n wears $p around $s waist.",
    "You wear $p around your waist."},
 
    {"$n puts $p around $s right wrist.",
    "You put $p around your right wrist."},

    {"$n puts $p around $s left wrist.",
    "You put $p around your left wrist."},
 
    {"$n wields $p.",
    "You wield $p."},

    {"$n grabs $p.",
    "You grab $p."},

    /*  PDH  2/1/99
     *  additional wear messages below
     */
    {"$n puts $p over $s face.",
    "You put $p over your face."},

    {"$n wears $p in $s right ear.",
    "You wear $p in your right ear."},

    {"$n wears $p in $s left ear.",
    "You wear $p in your left ear."},

    {"$n puts $p around $s right ankle.",
    "You put $p around your right ankle."},

    {"$n puts $p around $s left ankle.",
    "You put $p around your left ankle."},

    {"$p begins to float above $n's head.",
    "$p begins to float above your head."},

    {"$n puts $p across $s back.",
    "You put $p across your back."},

    {"$n throws $p over $s shoulders.",
    "You throw $p over your shoulders."},

    {"$n wears $p as a nose ring.",
    "You put $p on as a nose ring."},

        {"$n hang $p on $s belt.",
    "You hang $p on your belt."},

    {"$n now carries $p on $s back.",
    "You now carry $p on your back."},

    {"$n now carries $p on $s back.",
    "You now carry $p on your back."},

    {"$n now carries $p on $s back.",
    "You now carry $p on your back."},

      {"$n wears $p around $s waist.",
    "You wear $p around your waist."},
 
     {"$n sheathes $p.",
    "You sheathe $p."},

       {"$n sheathes $p.",
    "You sheathe $p."},

      {"$n sheathes $p.",
    "You sheathe $p."},

     {"$n sheathes $p.",
    "You sheathe $p."},

     {"$n sheathes $p.",
    "You sheathe $p."},

     {"$n sheathes $p.",
    "You sheathe $p."},

      {"$n sheathes $p.",
    "You sheathe $p."},

      {"$n sheathes $p.",
    "You sheathe $p."},

     {"$n sheathes $p.",
    "You sheathe $p."},
  };

  act(wear_messages[where][0], TRUE, ch, obj, 0, TO_ROOM);
  act(wear_messages[where][1], FALSE, ch, obj, 0, TO_CHAR);
}


int hands_full(struct char_data * ch) {
  int free_hands = 2;
  struct affected_type *af;

  for (af = ch->affected; af; af = af->next)
  {
    if ((af->type == SPELL_WITHER) && (af->location == APPLY_STR))
    {
      free_hands = 1;
      break;
    }
  }


  if (GET_EQ(ch, WEAR_HOLD))
    free_hands--;

  if (GET_EQ(ch, WEAR_SHIELD))
    free_hands--;

  if (GET_EQ(ch, WEAR_WIELD))
  {
    if (IS_OBJ_STAT(GET_EQ(ch, WEAR_WIELD), ITEM_TWO_HANDED))
      if (GET_RACE(ch) == RACE_MINOTAUR && GET_STR(ch) >= 19)
        free_hands--;
      else
        free_hands -= 2;
    else
      free_hands--;
  }

  return (free_hands < 1);
}

void lose_arm(struct char_data * ch) {
  int free_hands = 1;

  if (GET_EQ(ch, WEAR_HOLD))
    free_hands--;

  if (GET_EQ(ch, WEAR_SHIELD))
    free_hands--;

  if (GET_EQ(ch, WEAR_WIELD))
  {
    if (IS_OBJ_STAT(GET_EQ(ch, WEAR_WIELD), ITEM_TWO_HANDED))
    {
      free_hands -= 2;
    }
    else
    {
      free_hands--;
    }
  }

  if ((free_hands < 0) && (GET_EQ(ch, WEAR_HOLD)))
  {
    obj_to_char(unequip_char(ch, WEAR_HOLD), ch);
    free_hands++;
  }

  if ((free_hands < 0) && (GET_EQ(ch, WEAR_SHIELD)))
  {
    obj_to_char(unequip_char(ch, WEAR_SHIELD), ch);
    free_hands++;
  }

  if ((free_hands < 0) && (GET_EQ(ch, WEAR_WIELD)))
  {
    if (IS_OBJ_STAT(GET_EQ(ch, WEAR_WIELD), ITEM_TWO_HANDED))
      free_hands += 2;
    else
      free_hands++;

    obj_to_char(unequip_char(ch, WEAR_WIELD), ch);
  }

  return;
}

void show_sheathed(struct char_data * ch, struct obj_data * obj,  struct obj_data * cont)
{
  /*
   * ITEM_WEAR_TAKE is used for objects that do not require special bits
   * to be put into that position (e.g. you can hold any object, not just
   * an object with a HOLD bit.)
   */

  //struct obj_data *o, *next_o;
  int pos, pos_sheath = 0;

/*
  int wear_bitvectors[] = {
    ITEM_WEAR_TAKE,  
    ITEM_WEAR_FINGER,   
    ITEM_WEAR_FINGER, 
    ITEM_WEAR_NECK,     
    ITEM_WEAR_NECK,  
    ITEM_WEAR_BODY,     
    ITEM_WEAR_HEAD,   
    ITEM_WEAR_LEGS,     
    ITEM_WEAR_FEET,  
    ITEM_WEAR_HANDS,    
    ITEM_WEAR_ARMS,     
    ITEM_WEAR_SHIELD,  
    ITEM_WEAR_ABOUT, 
    ITEM_WEAR_WAIST,
    ITEM_WEAR_WRIST,    
    ITEM_WEAR_WRIST,    
    ITEM_WEAR_WIELD,    
    ITEM_WEAR_TAKE,
    ITEM_WEAR_FACE,   
    ITEM_WEAR_EAR,    
    ITEM_WEAR_EAR,       
    ITEM_WEAR_ANKLE, 
    ITEM_WEAR_ANKLE,  
    ITEM_WEAR_ABOVE,  
    ITEM_WEAR_BACK,    
    ITEM_WEAR_SHOULDER,   
    ITEM_WEAR_NOSE, 
    ITEM_WEAR_ONBELT,    
    ITEM_WEAR_ONBACK,  
    ITEM_WEAR_ONBACK,  
    ITEM_WEAR_ONBACK, 
    ITEM_WEAR_WAIST, 
    ITEM_WEAR_SHEATHED_H, 
    ITEM_WEAR_SHEATHED_B, 
    ITEM_WEAR_SHEATHED_B, 
    ITEM_WEAR_SHEATHED_WA,
    ITEM_WEAR_SHEATHED_WA, 
    ITEM_WEAR_SHEATHED_A,
    ITEM_WEAR_SHEATHED_A,  
    ITEM_WEAR_SHEATHED_WR, 
    ITEM_WEAR_SHEATHED_WR
    };
*/

   pos = cont->worn_on;

  if  (pos == WEAR_WAIST_1)
  {
    pos_sheath = pos + 22;
  }
  else if  (pos == WEAR_WAIST_2)
  {
    pos_sheath = pos + 5;
  }
  else if (pos == WEAR_ANKLE_R)
  {
  pos_sheath = pos + 16;
  }
  else if ( pos == WEAR_ANKLE_L)
  {
   pos_sheath = pos + 16;
  }
  else if (pos == WEAR_WRIST_R)    
  {
   pos_sheath = pos + 25;
  }
  else if (pos == WEAR_WRIST_L)
  {
    pos_sheath = pos + 25;
  }
  else if (pos == WEAR_HANDS)    
  {
   pos_sheath = pos + 23;
  }
  else if (pos == WEAR_BACK_1) 
  {
   pos_sheath = pos + 9;
  }


  /* first, make sure that the wear position is valid. 
  if (!CAN_WEAR(obj, wear_bitvectors[pos_sheath])) {
   send_to_char("Cant wear here!\r\n", ch);
       return;
  } */

   send_to_char(where[pos_sheath], ch);
   show_obj_to_char(obj, ch, 1);

  return;
}

void perform_sheath(struct char_data * ch, struct obj_data * obj,  struct obj_data * cont)
{
  /*
   * ITEM_WEAR_TAKE is used for objects that do not require special bits
   * to be put into that position (e.g. you can hold any object, not just
   * an object with a HOLD bit.)
   */

  //struct obj_data *o, *next_o;
  int pos, pos_sheath = 0;

  int wear_bitvectors[] = {
    ITEM_WEAR_TAKE,  
    ITEM_WEAR_FINGER,   
    ITEM_WEAR_FINGER, 
    ITEM_WEAR_NECK,     
    ITEM_WEAR_NECK,  
    ITEM_WEAR_BODY,     
    ITEM_WEAR_HEAD,   
    ITEM_WEAR_LEGS,     
    ITEM_WEAR_FEET,  
    ITEM_WEAR_HANDS,    
    ITEM_WEAR_ARMS,     
    ITEM_WEAR_SHIELD,  
    ITEM_WEAR_ABOUT, 
    ITEM_WEAR_WAIST,
    ITEM_WEAR_WRIST,    
    ITEM_WEAR_WRIST,    
    ITEM_WEAR_WIELD,    
    ITEM_WEAR_TAKE,
    ITEM_WEAR_FACE,   
    ITEM_WEAR_EAR,    
    ITEM_WEAR_EAR,       
    ITEM_WEAR_ANKLE, 
    ITEM_WEAR_ANKLE,  
    ITEM_WEAR_ABOVE,  
    ITEM_WEAR_BACK,    
    ITEM_WEAR_SHOULDER,   
    ITEM_WEAR_NOSE, 
    ITEM_WEAR_ONBELT,    
    ITEM_WEAR_ONBACK,  
    ITEM_WEAR_ONBACK,  
    ITEM_WEAR_ONBACK, 
    ITEM_WEAR_WAIST, 
    ITEM_WEAR_SHEATHED_H, 
    ITEM_WEAR_SHEATHED_B, 
    ITEM_WEAR_SHEATHED_B, 
    ITEM_WEAR_SHEATHED_WA,
    ITEM_WEAR_SHEATHED_WA, 
    ITEM_WEAR_SHEATHED_A,
    ITEM_WEAR_SHEATHED_A,  
    ITEM_WEAR_SHEATHED_WR, 
    ITEM_WEAR_SHEATHED_WR
    };

 pos = cont->worn_on;

  if  (pos == WEAR_WAIST_1)
    pos_sheath = pos + 22;
 else if  (pos == WEAR_WAIST_2)
    pos_sheath = pos + 5;
 else if (pos == WEAR_ANKLE_R)
  pos_sheath = pos + 16;
 else if ( pos == WEAR_ANKLE_L)
   pos_sheath = pos + 16;
  else if (pos == WEAR_WRIST_R)
    pos_sheath = pos + 25;
 else if (pos == WEAR_WRIST_L)
    pos_sheath = pos + 25;
 else if (pos == WEAR_HANDS)
    pos_sheath = pos + 23;
 else if (pos == WEAR_BACK_1)
    pos_sheath = pos + 9;


  /* first, make sure that the wear position is valid. */
  if (!CAN_WEAR(obj, wear_bitvectors[pos_sheath])) {
      act("$p cannot use that as a sheath!", FALSE, ch, obj, 0, TO_CHAR);
    return;
}
 
   if (PLR_FLAGGED(ch, PLR_RABBIT) ||     PLR_FLAGGED(ch, PLR_BIRD) ||
     PLR_FLAGGED(ch, PLR_WOLF) ||     PLR_FLAGGED(ch, PLR_BEAR) ||
     PLR_FLAGGED(ch, PLR_CAT)){
     send_to_char("Animals don't need to wear clothes.\r\n", ch);
     return;
  }

  if ( ! wear_otrigger(obj, ch, pos_sheath)) {
    return;
  }

  if (not_restricted(ch, obj))
  {
    wear_message(ch, obj, pos_sheath);
    obj_from_char(obj);
    obj_to_obj(obj, cont);
    }

  return;
}

void perform_wear(struct char_data * ch, struct obj_data * obj, int where)
{
  /*
   * ITEM_WEAR_TAKE is used for objects that do not require special bits
   * to be put into that position (e.g. you can hold any object, not just
   * an object with a HOLD bit.)
   */

  //struct obj_data *o, *next_o;

  int wear_bitvectors[] = {
    ITEM_WEAR_TAKE,  
    ITEM_WEAR_FINGER,   
    ITEM_WEAR_FINGER, 
    ITEM_WEAR_NECK,     
    ITEM_WEAR_NECK,  
    ITEM_WEAR_BODY,     
    ITEM_WEAR_HEAD,   
    ITEM_WEAR_LEGS,     
    ITEM_WEAR_FEET,  
    ITEM_WEAR_HANDS,    
    ITEM_WEAR_ARMS,     
    ITEM_WEAR_SHIELD,  
    ITEM_WEAR_ABOUT, 
    ITEM_WEAR_WAIST,
    ITEM_WEAR_WRIST,    
    ITEM_WEAR_WRIST,    
    ITEM_WEAR_WIELD,    
    ITEM_WEAR_TAKE,
    ITEM_WEAR_FACE,   
    ITEM_WEAR_EAR,    
    ITEM_WEAR_EAR,       
    ITEM_WEAR_ANKLE, 
    ITEM_WEAR_ANKLE,  
    ITEM_WEAR_ABOVE,  
    ITEM_WEAR_BACK,    
    ITEM_WEAR_SHOULDER,   
    ITEM_WEAR_NOSE, 
    ITEM_WEAR_ONBELT,    
    ITEM_WEAR_ONBACK,  
    ITEM_WEAR_ONBACK,  
    ITEM_WEAR_ONBACK, 
    ITEM_WEAR_WAIST, 
    ITEM_WEAR_SHEATHED_H, 
    ITEM_WEAR_SHEATHED_B, 
    ITEM_WEAR_SHEATHED_B, 
    ITEM_WEAR_SHEATHED_WA,
    ITEM_WEAR_SHEATHED_WA, 
    ITEM_WEAR_SHEATHED_A,
    ITEM_WEAR_SHEATHED_A,  
    ITEM_WEAR_SHEATHED_WR, 
    ITEM_WEAR_SHEATHED_WR
    };

 char *already_wearing[] = {
    "You're already using a light.\r\n",
    "YOU SHOULD NEVER SEE THIS MESSAGE.  PLEASE REPORT 1.\r\n",
    "You're already wearing something on both of your ring fingers.\r\n",
    "YOU SHOULD NEVER SEE THIS MESSAGE.  PLEASE REPORT 2.\r\n",
    "You can't wear anything else around your neck.\r\n",
    "You're already wearing something on your body.\r\n",
    "You're already wearing something on your head.\r\n",
    "You're already wearing something on your legs.\r\n",
    "You're already wearing something on your feet.\r\n",
    "You're already wearing something on your hands.\r\n",
    "You're already wearing something on your arms.\r\n",
    "You're already using a shield.\r\n",
    "You're already wearing something about your body.\r\n",
    "YOU SHOULD NEVER SEE THIS MESSAGE. PLEASE REPORT 3.\r\n",
     "YOU SHOULD NEVER SEE THIS MESSAGE. PLEASE REPORT 3.\r\n",
    "You're already wearing something around both of your wrists.\r\n",
    "You're already wielding a weapon.\r\n",
    "You're already holding something.\r\n",
    "You're already wearing something on your face.\r\n",
    "YOU SHOULD NEVER SEE THIS MESSAGE. PLEASE REPORT 4.\r\n",
    "You're already wearing something on both of your ears\r\n",
    "YOU SHOULD NEVER SEE THIS MESSAGE. PLEASE REPORT 5.\r\n",
    "You're already wearing something on both of your ankles.\r\n",
    "You're already wearing something floating above your head.\r\n",
    "You are already wearing something on your back.\r\n",
    "You're already wearing something on your shoulders.\r\n",
    "You're already wearing something on your nose.\r\n",
    "You already have something hanging down your belt.\r\n",
     "You are already carrying something on your back.\r\n",
    "YOU SHOULD NEVER SEE THIS MESSAGE.  PLEASE REPORT 6.\r\n",
    "YOU SHOULD NEVER SEE THIS MESSAGE.  PLEASE REPORT 6.\r\n",
    "You are already wearing enough on your waist.\r\n",
    "You already have something sheathed there!\r\n",
      "You already have something sheathed there!\r\n",
    "You already have something sheathed there!\r\n",
      "You already have something sheathed there!\r\n",
    "You already have something sheathed there!\r\n",
      "You already have something sheathed there!\r\n",
    "You already have something sheathed there!\r\n",
      "You already have something sheathed there!\r\n",
    "You already have something sheathed there!\r\n"
  };

  /* first, make sure that the wear position is valid. */
  if ( ! CAN_WEAR(obj, wear_bitvectors[where])) {
    act("You can't wear $p there.", FALSE, ch, obj, 0, TO_CHAR);
    return;
  }

   if (PLR_FLAGGED(ch, PLR_RABBIT) ||
     PLR_FLAGGED(ch, PLR_BIRD) ||
     PLR_FLAGGED(ch, PLR_WOLF) ||
     PLR_FLAGGED(ch, PLR_BEAR) ||
     PLR_FLAGGED(ch, PLR_CAT)){
     send_to_char("Animals don't need to wear clothes.\r\n", ch);
     return;
  }

  /* for neck, finger, and wrist, try pos 2 if pos 1 is already full */
  if ((where == WEAR_FINGER_R) || (where == WEAR_NECK_1) ||
      (where == WEAR_WRIST_R)  || (where == WEAR_EAR_R)  ||
      (where == WEAR_ANKLE_R)) {
    if (GET_EQ(ch, where)) {
      where++;
    }
  }


if  (where == WEAR_WAIST_1) {
     if (GET_EQ(ch, where)) {
     where = where + 18;
}     
}     

  if (where == WEAR_ONBACK_1) {
      if (GET_EQ(ch, where) && IS_NPC(ch)) {
      where++;
        if (GET_EQ(ch, where) && IS_NPC(ch)) {
      where++;
          }
    }
  }


/*  Check for two-handers before checking to see if something's wielded.  */
//  if ((where == WEAR_WIELD) && (IS_OBJ_STAT(obj, ITEM_TWO_HANDED)) &&
//          (GET_EQ(ch, WEAR_WIELD) || GET_EQ(ch, WEAR_HOLD) || GET_EQ(ch, WEAR_SHIELD))) {
//    send_to_char("You need both hands to wield this.\r\n", ch);
//    return;
//  }

  if (where == WEAR_WIELD && IS_OBJ_STAT(obj, ITEM_TWO_HANDED)) {
    if (GET_RACE(ch) == RACE_MINOTAUR) {
      // Str over 19 and a mino? Wield as one-hander
      if (GET_STR(ch) >= 19 && not_restricted(ch, obj) && !GET_EQ(ch, where)) {
        wear_message(ch, obj, where);
        obj_from_char(obj);
        equip_char(ch, obj, where);
        wear_spells(ch, obj);
        return;
      
      // Str under 19 and a mino? Wield as two-hander  
      } else if (GET_STR(ch) < 19 && not_restricted(ch, obj) && !GET_EQ(ch, where)) {
        if ((IS_OBJ_STAT(obj, ITEM_TWO_HANDED)) &&
                (GET_EQ(ch, WEAR_WIELD) || GET_EQ(ch, WEAR_HOLD) || GET_EQ(ch, WEAR_SHIELD))) {
            send_to_char("You need both hands to wield this.\r\n", ch);
            return;
        }
      } else {
        send_to_char("You are too weak to wield this weapon with one hand.\r\n", ch);
        return;
      }
    
    // Not a mino? Wield as two-hander    
    } else {
      if ((IS_OBJ_STAT(obj, ITEM_TWO_HANDED)) &&
              (GET_EQ(ch, WEAR_WIELD) || GET_EQ(ch, WEAR_HOLD) || GET_EQ(ch, WEAR_SHIELD))) {
        send_to_char("You need both hands to wield this.\r\n", ch);
        return;
      }
    }
  }

  if (GET_EQ(ch, where)) {
    send_to_char(already_wearing[where], ch);
    return;
  }


  /*  Let's not let people have three hands.  Soli, 8/12/99  */

  if (((where == WEAR_HOLD) || (where == WEAR_SHIELD) || (where == WEAR_WIELD)) && hands_full(ch)) {
    send_to_char("But your hands are full!\r\n", ch);
    return;
  }

  if ( ! wear_otrigger(obj, ch, where)) {
    return;
  }

  if (not_restricted(ch, obj))
  {
    wear_message(ch, obj, where);
    obj_from_char(obj);
    equip_char(ch, obj, where);
    wear_spells(ch, obj);
  }

  return;
}


int find_eq_pos(struct char_data * ch, struct obj_data * obj, char *arg)
{
  int where = -1;

  static const char *keywords[] = {
    "!RESERVED!",
    "finger",
    "!RESERVED!",
    "neck",
    "!RESERVED!",
    "body",
    "head",
    "legs",
    "feet",
    "hands",
    "arms",
    "shield",
    "about",
    "waist",
    "wrist",
    "!RESERVED!",
    "!RESERVED!",
    "!RESERVED!",
    "face",
    "ear",
    "!RESERVED!",
    "ankle",
    "!RESERVED!",
    "above",
    "back",
    "shoulder",
    "nose",
    "!RESERVED!",
    "onback",
    "!RESERVED!",
    "!RESERVED!",
    "!RESERVED!",
    "!RESERVED!",
    "!RESERVED!",
    "!RESERVED!",
    "!RESERVED!",
    "!RESERVED!",
    "!RESERVED!",
    "!RESERVED!",
    "!RESERVED!",
    "!RESERVED!",
    "!RESERVED!",
    "!RESERVED!",
    "\n"
  };

  if (!arg || !*arg) {
    if (CAN_WEAR(obj, ITEM_WEAR_FINGER))      where = WEAR_FINGER_R;
    if (CAN_WEAR(obj, ITEM_WEAR_NECK))        where = WEAR_NECK_1;
    if (CAN_WEAR(obj, ITEM_WEAR_BODY))        where = WEAR_BODY;
    if (CAN_WEAR(obj, ITEM_WEAR_HEAD))        where = WEAR_HEAD;
    if (CAN_WEAR(obj, ITEM_WEAR_LEGS))        where = WEAR_LEGS;
    if (CAN_WEAR(obj, ITEM_WEAR_FEET))        where = WEAR_FEET;
    if (CAN_WEAR(obj, ITEM_WEAR_HANDS))       where = WEAR_HANDS;
    if (CAN_WEAR(obj, ITEM_WEAR_ARMS))        where = WEAR_ARMS;
    if (CAN_WEAR(obj, ITEM_WEAR_SHIELD))      where = WEAR_SHIELD;
    if (CAN_WEAR(obj, ITEM_WEAR_ABOUT))       where = WEAR_ABOUT;
    if (CAN_WEAR(obj, ITEM_WEAR_WAIST))       where = WEAR_WAIST_1;
    if (CAN_WEAR(obj, ITEM_WEAR_WRIST))       where = WEAR_WRIST_R;
    if (CAN_WEAR(obj, ITEM_WEAR_FACE))        where = WEAR_FACE;
    if (CAN_WEAR(obj, ITEM_WEAR_EAR))         where = WEAR_EAR_R;
    if (CAN_WEAR(obj, ITEM_WEAR_ANKLE))       where = WEAR_ANKLE_R;
    if (CAN_WEAR(obj, ITEM_WEAR_ABOVE))       where = WEAR_ABOVE;
    if (CAN_WEAR(obj, ITEM_WEAR_BACK))        where = WEAR_BACK_1;
    if (CAN_WEAR(obj, ITEM_WEAR_SHOULDER))    where = WEAR_SHOULDER;
    if (CAN_WEAR(obj, ITEM_WEAR_NOSE))        where = WEAR_NOSE;
    if (CAN_WEAR(obj, ITEM_WEAR_ONBACK))      where = WEAR_ONBACK_1;

  } else {
    if ((where = search_block(arg, keywords, FALSE)) < 0) {
      sprintf(buf, "'%s'?  What part of your body is THAT?\r\n", arg);
      send_to_char(buf, ch);
    }
  }


  /*  mess. to zorn from zorn: fix for later use or get the bag meth. out */
  /*
    if (GET_OBJ_LEVEL(obj) > GET_LEVEL(ch)) {
    sprintf(buf, "&YYou must be level &W(&r%d&W)&Y to wear this&n\r\n",
    GET_OBJ_LEVEL(obj));
    send_to_char(buf, ch);
    return 0;
    } */
  return where;
}



ACMD(do_wear)
{
  char arg1[MAX_INPUT_LENGTH];
  char arg2[MAX_INPUT_LENGTH];
  struct obj_data *obj, *next_obj;
  int where, dotmode, items_worn = 0;

  two_arguments(argument, arg1, arg2);

 if (GET_POS(ch) == POS_FISHING || GET_POS(ch) == POS_DIGGING) {
     send_to_char("You are not in a proper position for that!\r\n", ch);
    return;
}
  if (!*arg1) {
    send_to_char("Wear what?\r\n", ch);
    return;
  }

  dotmode = find_all_dots(arg1);

  if (*arg2 && (dotmode != FIND_INDIV)) {
    send_to_char("You can't specify the same body location for more than one item!\r\n", ch);
    return;
  }

  if (dotmode == FIND_ALL) {
    for (obj = ch->carrying; obj; obj = next_obj) {
      next_obj = obj->next_content;
      if (CAN_SEE_OBJ(ch, obj) && (where = find_eq_pos(ch, obj, 0)) >= 0) {
	items_worn++;
	perform_wear(ch, obj, where);
      }
    }

    if (!items_worn) {
      send_to_char("You don't seem to have anything wearable.\r\n", ch);
    }

  } else if (dotmode == FIND_ALLDOT) {
    if (!*arg1) {
      send_to_char("Wear all of what?\r\n", ch);
      return;
    }

    if (!(obj = get_obj_in_list_vis(ch, arg1, ch->carrying))) {
      sprintf(buf, "You don't seem to have any %ss.\r\n", arg1);
      send_to_char(buf, ch);

    } else {
      while (obj) {
	next_obj = get_obj_in_list_vis(ch, arg1, obj->next_content);
	if ((where = find_eq_pos(ch, obj, 0)) >= 0)
	  perform_wear(ch, obj, where);
	else
	  act("You can't wear $p.", FALSE, ch, obj, 0, TO_CHAR);
	obj = next_obj;
      }
    }

  } else {
    if (!(obj = get_obj_in_list_vis(ch, arg1, ch->carrying))) {
      sprintf(buf, "You don't seem to have %s %s.\r\n", AN(arg1), arg1);
      send_to_char(buf, ch);
    } else {
      if ((where = find_eq_pos(ch, obj, arg2)) >= 0)
	perform_wear(ch, obj, where);
      else if (!*arg2)
	act("You can't wear $p.", FALSE, ch, obj, 0, TO_CHAR);
    }
  }
}



ACMD(do_wield)
{
  struct obj_data *obj;

  one_argument(argument, arg);

 if (GET_POS(ch) == POS_FISHING || GET_POS(ch) == POS_DIGGING) {
     send_to_char("You are not in a proper position for that!\r\n", ch);
    return ;
}
  if (!*arg)
    send_to_char("Wield what?\r\n", ch);
  else if (!(obj = get_obj_in_list_vis(ch, arg, ch->carrying))) {
    sprintf(buf, "You don't seem to have %s %s.\r\n", AN(arg), arg);
    send_to_char(buf, ch);
  } else {
    if (!CAN_WEAR(obj, ITEM_WEAR_WIELD))
      send_to_char("You can't wield that.\r\n", ch);
    else if (GET_OBJ_WEIGHT(obj) > str_app[STRENGTH_APPLY_INDEX(ch)].wield_w)
      send_to_char("It's too heavy for you to use.\r\n", ch);
    else
      perform_wear(ch, obj, WEAR_WIELD);
  }
}

ACMD(do_hang)
{
  char arg1[MAX_INPUT_LENGTH];
  char arg2[MAX_INPUT_LENGTH];
  struct obj_data *obj, *belt;
  int i, obj_dotmode;

  two_arguments(argument, arg1, arg2);
  obj_dotmode = find_all_dots(arg1);

 if (GET_POS(ch) == POS_FISHING || GET_POS(ch) == POS_DIGGING) {
     send_to_char("You are not in a proper position for that!\r\n", ch);
    return ;
}
  if (!*arg1)
    send_to_char("Hang what on what?\r\n", ch);
   else if (!*arg2) {
    sprintf(buf, "What do you want to hang %s on?\r\n",
            ((obj_dotmode == FIND_INDIV) ? "it" : "them"));
    }
    else {
      if  (!(belt = get_object_in_equip_vis(ch, arg2, ch->equipment, &i))) {
      sprintf(buf, "You must wear %s %s first.\r\n", AN(arg2), arg2);
      send_to_char(buf, ch);
    } else if (GET_OBJ_TYPE(belt) != ITEM_BELT)
      act("$p is not a belt.", FALSE, ch, belt, 0, TO_CHAR);
     else {
        if (!(obj = get_obj_in_list_vis(ch, arg1, ch->carrying))) {
          sprintf(buf, "You aren't carrying %s %s.\r\n", AN(arg1), arg1);
          send_to_char(buf, ch);
        }
         else if (!CAN_WEAR(obj, ITEM_WEAR_ONBELT))
          send_to_char("You can't wear that on your belt!\r\n", ch);
        else if (obj == belt)
          send_to_char("You attempt to fold it into itself, but fail.\r\n", ch);
        else 
          perform_wear(ch, obj, WEAR_ONBELT);
        
       }
      }
     } 

ACMD(do_unsheath)
{
  char arg1[MAX_INPUT_LENGTH];
  char arg2[MAX_INPUT_LENGTH];
  int obj_dotmode, i;
  struct obj_data *cont;
  struct obj_data *obj;

  two_arguments(argument, arg1, arg2);

  obj_dotmode = find_all_dots(arg1);

 if (GET_POS(ch) == POS_FISHING || GET_POS(ch) ==  POS_DIGGING) {
     send_to_char("You are not in a proper position for that!\r\n", ch);
    return; 
}
  if (!*arg1)
    send_to_char("Unsheathe what from what?\r\n", ch);
  else if (!*arg2)
    send_to_char("Unsheathe from what?\r\n", ch);
  else {
        if (!(cont = get_object_in_equip_vis(ch, arg2, ch->equipment, &i))) {
        sprintf(buf, "You don't have %s %s.\r\n", AN(arg2), arg2);
        send_to_char(buf, ch);
      } else if (GET_OBJ_TYPE(cont) != ITEM_CONTAINER && GET_OBJ_TYPE(cont) != ITEM_SHEATH)
        act("$p is not a container.", FALSE, ch, cont, 0, TO_CHAR);
       else {
       if (obj_dotmode == FIND_INDIV) {
         if (!(obj = get_obj_in_list_vis(ch, arg1, cont->contains))) {
           sprintf(buf, "There doesn't seem to be %s %s in $p.", AN(arg1), arg1);
           act(buf, FALSE, ch, cont, 0, TO_CHAR);
    } else {
        if (IS_CARRYING_N(ch) >= CAN_CARRY_N(ch)) {
          act("$p: you can't hold any more items.", FALSE, ch, obj, 0, TO_CHAR);
        } else if (get_otrigger(obj, ch)) {
                obj_from_obj(obj);
                obj_to_char(obj, ch);
                act("You unsheathe $p.", FALSE, ch, obj, 0, TO_CHAR);
                act("$n unsheathes $p.", TRUE, ch, obj, 0, TO_ROOM);

       if (!CAN_WEAR(obj, ITEM_WEAR_WIELD))
          send_to_char("You can't wield that.\r\n", ch);
       else if (GET_OBJ_WEIGHT(obj) > str_app[STRENGTH_APPLY_INDEX(ch)].wield_w)
         send_to_char("It's too heavy for you to use.\r\n", ch);
       else if (GET_EQ(ch, WEAR_WIELD) && !GET_EQ(ch, WEAR_HOLD) && CAN_WEAR(obj, ITEM_WEAR_HOLD))
        perform_wear(ch, obj, WEAR_HOLD);
       else if (!GET_EQ(ch, WEAR_WIELD))
        perform_wear(ch, obj, WEAR_WIELD);
      }
    }
   }
  }
 }
}


ACMD(do_grab)
{
  struct obj_data *obj;

  one_argument(argument, arg);

   if (GET_POS(ch) == POS_FISHING || GET_POS(ch) == POS_DIGGING) {
     send_to_char("You are not in a proper position for that!\r\n", ch);
    return ;
}

  if (!*arg)
    send_to_char("Hold what?\r\n", ch);
  else if (!(obj = get_obj_in_list_vis(ch, arg, ch->carrying))) {
    sprintf(buf, "You don't seem to have %s %s.\r\n", AN(arg), arg);
    send_to_char(buf, ch);
  } else {
    if (GET_OBJ_TYPE(obj) == ITEM_LIGHT)
      perform_wear(ch, obj, WEAR_LIGHT);
    else {
      if (!CAN_WEAR(obj, ITEM_WEAR_HOLD) && GET_OBJ_TYPE(obj) != ITEM_WAND &&
      GET_OBJ_TYPE(obj) != ITEM_STAFF && GET_OBJ_TYPE(obj) != ITEM_SCROLL &&
      GET_OBJ_TYPE(obj) != ITEM_POTION && GET_OBJ_TYPE(obj) != ITEM_SALVE)
	send_to_char("You can't hold that.\r\n", ch);
      else
	perform_wear(ch, obj, WEAR_HOLD);
    }
  }
}



void perform_remove(struct char_data * ch, int pos)
{
  struct obj_data *obj;
  int k = 0;

  if (!(obj = GET_EQ(ch, pos))) {
    log("Error in perform_remove: bad pos passed.");
    return;
  } 


  if (IS_OBJ_STAT(obj, ITEM_NO_REMOVE))
  {
    send_to_char("You cannot seem to remove it!\r\n", ch);
    return;
  }

  if (IS_CARRYING_N(ch) >= CAN_CARRY_N(ch))
    act("$p: you can't carry that many items!", FALSE, ch, obj, 0, TO_CHAR);

  else if (IS_NPC(ch) && (GET_EQ(ch, pos) == GET_EQ(ch, WEAR_ONBACK_1) || GET_EQ(ch, pos) == GET_EQ(ch, WEAR_ONBACK_2) || GET_EQ(ch, pos) == GET_EQ(ch, WEAR_ONBACK_3))){
   obj_to_char(unequip_char(ch, pos), ch);
 }


 else if (GET_EQ(ch, pos) == GET_EQ(ch, WEAR_WAIST_1)){
   if (GET_OBJ_TYPE(obj) == ITEM_BELT && GET_EQ(ch, WEAR_ONBELT)){
     obj_to_char(unequip_char(ch, WEAR_ONBELT), ch);
     obj_to_char(unequip_char(ch, WEAR_WAIST_1), ch);
     act("You stop using $p.", FALSE, ch, obj, 0, TO_CHAR);
     act("$n stops using $p.", TRUE, ch, obj, 0, TO_ROOM);
   }

  else {
     obj_to_char(unequip_char(ch, WEAR_WAIST_1), ch);
     act("You stop using $p.", FALSE, ch, obj, 0, TO_CHAR);
     act("$n stops using $p.", TRUE, ch, obj, 0, TO_ROOM);
   }
 }

 else if (GET_EQ(ch, pos) == GET_EQ(ch, WEAR_WAIST_2)){
   if (GET_OBJ_TYPE(obj) == ITEM_BELT && GET_EQ(ch, WEAR_ONBELT)){
     obj_to_char(unequip_char(ch, WEAR_ONBELT), ch);
     obj_to_char(unequip_char(ch, WEAR_WAIST_2), ch);
     act("You stop using $p.", FALSE, ch, obj, 0, TO_CHAR);
     act("$n stops using $p.", TRUE, ch, obj, 0, TO_ROOM);
   }

  else {
     obj_to_char(unequip_char(ch, WEAR_WAIST_2), ch);
     act("You stop using $p.", FALSE, ch, obj, 0, TO_CHAR);
     act("$n stops using $p.", TRUE, ch, obj, 0, TO_ROOM);
   }
 }

  else {
    obj_to_char(unequip_char(ch, pos), ch);
    act("You stop using $p.", FALSE, ch, obj, 0, TO_CHAR);
    act("$n stops using $p.", TRUE, ch, obj, 0, TO_ROOM);
  }

    /*
    if (GET_OBJ_DISGUISE(obj) == 1 || GET_OBJ_DISGUISE(obj) == 2) {
      act("You have revealed yourself by removing $p!", FALSE, ch, obj, 0, TO_CHAR);
      act("$n has revealed $mself by removing $p!", TRUE, ch, obj, 0, TO_ROOM);
      reveal(ch);
    }
    */

    if(obj&&HAS_SPELLS(obj)) {
    while(GET_WEAPON_SPELL(obj,k)&&k<MAX_SPELL_AFFECT) {

      affect_remove_nonperm_II(ch, GET_WEAPON_SPELL(obj,k));

      k++;
    }
  }

}


ACMD(do_remove)
{
  struct obj_data *obj;
  int i, dotmode, found;

if (GET_POS(ch) == POS_FISHING || GET_POS(ch) == POS_DIGGING) {
     send_to_char("You are not in a proper position for that!\r\n", ch);
    return ;
}

  one_argument(argument, arg);

  if (!*arg) {
    send_to_char("Remove what?\r\n", ch);
    return;
  }
  dotmode = find_all_dots(arg);

  if (dotmode == FIND_ALL) {
    found = 0;
    for (i = 0; i < NUM_WEARS; i++)
      if (GET_EQ(ch, i) && !hidden_eq(ch, GET_EQ(ch, i))) {
        perform_remove(ch, i);
        found = 1;
      }
    if (!found)
      send_to_char("You're not using anything.\r\n", ch);
  } else if (dotmode == FIND_ALLDOT) {
    if (!*arg)
      send_to_char("Remove all of what?\r\n", ch);
    else {
      found = 0;
      for (i = 0; i < NUM_WEARS; i++)
        if (GET_EQ(ch, i) && CAN_SEE_OBJ(ch, GET_EQ(ch, i)) && !hidden_eq(ch, GET_EQ(ch, i)) &&
            isname(arg, GET_EQ(ch, i)->name)) {
          perform_remove(ch, i);
          found = 1;
        }
      if (!found) {
        sprintf(buf, "You don't seem to be using any %ss.\r\n", arg);
        send_to_char(buf, ch);
      }
    }
  } else {
    if (!(obj = get_object_in_equip_vis(ch, arg, ch->equipment, &i))) {
      sprintf(buf, "You don't seem to be using %s %s.\r\n", AN(arg), arg);
      send_to_char(buf, ch);
    } else
      perform_remove(ch, i);
  }
}


ACMD(do_mrest)
{
  bool found = FALSE;
  room_rnum org_room;
  struct char_data *vict;
  struct follow_type *k;

  one_argument(argument, arg);

 if (GET_POS(ch) == POS_FISHING || GET_POS(ch) == POS_DIGGING) {
     send_to_char("You are not in a proper position for that!\r\n", ch);
    return;
}

 if (GET_POS(ch) == POS_RIDING) {
     send_to_char("YPerhaps you should dismount first?\r\n", ch);
    return ;
}

  if (!*arg) {
    send_to_char("Who?!?\r\n", ch);
    return;
  }
  else if (!(vict = get_char_vis(ch, arg, FIND_CHAR_ROOM)) && !is_abbrev(arg, "followers"))
    send_to_char("That person isn't here.\r\n", ch);
  else if (ch == vict)
    send_to_char("You obviously suffer from skitzofrenia.\r\n", ch);

  else {
    if (IS_AFFECTED(ch, AFF_CHARM)) {
      send_to_char("Your superior would not aprove of you giving orders.\r\n", ch);
      return;
    }
    if (vict) {
      
      act("$n gives $N an order.", FALSE, ch, 0, vict, TO_ROOM);

      if (vict->master != ch)
        act("$n has an indifferent look.", FALSE, vict, 0, 0, TO_ROOM);

      else if (!IS_NPC(vict) || !MOB_FLAGGED(vict, MOB_MOUNTABLE))  {
      send_to_char("They are not a mount!!\r\n", ch);
      return;
     }

      else {
        send_to_char(OK, ch);
        command_interpreter(vict, "rest");
      }
    } else {                    
      
      org_room = ch->in_room;

      for (k = ch->followers; k; k = k->next) {
        if (org_room == k->follower->in_room)
          if (IS_AFFECTED(k->follower, AFF_CHARM) || IS_AFFECTED(k->follower, AFF_TAMED)) {
            found = TRUE;
            command_interpreter(k->follower, "rest");
          }
      }
      if (found)
        send_to_char(OK, ch);
      else
        send_to_char("Nobody here is a loyal subject of yours!\r\n", ch);
    }
  }
}

ACMD(do_mreport)
{
  bool found = FALSE;
  room_rnum org_room;
  struct char_data *vict;
  struct follow_type *k;
  struct char_data *l;
	struct follow_type *f;	

  one_argument(argument, arg);

 if (GET_POS(ch) == POS_FISHING || GET_POS(ch) == POS_DIGGING) {
     send_to_char("You are not in a proper position for that!\r\n", ch);
    return;
}

if (!IS_AFFECTED(ch, AFF_GROUP)) {
	send_to_char("But you are not a member of any group!\r\n", ch);
	return;
}

  if (!*arg) {
    send_to_char("Who?!?\r\n", ch);
    return;
  }
  else if (!(vict = get_char_vis(ch, arg, FIND_CHAR_ROOM)) && !is_abbrev(arg, "followers"))
    send_to_char("That person isn't here.\r\n", ch);
  else if (ch == vict)
    send_to_char("You obviously suffer from skitzofrenia.\r\n", ch);

  else {
    if (IS_AFFECTED(ch, AFF_CHARM)) {
      send_to_char("Your superior would not aprove of you giving orders.\r\n", ch);
      return;
    }
    if (vict) {
      
      act("$n gives $N an order.", FALSE, ch, 0, vict, TO_ROOM);

      if (vict->master != ch) {
        act("$n has an indifferent look.", FALSE, vict, 0, 0, TO_ROOM);
        return;
      }
      else if (!IS_NPC(vict) || !MOB_FLAGGED(vict, MOB_MOUNTABLE))  {
      send_to_char("They are not a mount!!\r\n", ch);
      return;
     }

		  l = (ch->master ? ch->master : ch);

		  for (f = l->followers; f; f = f->next)
		    if (f->follower != ch)
		    {
		      GET_NAME_II(vict, f->follower, chname);
		      sprintf(buf, "%s reports: %d/%dH, %d/%dV\r\n",
		          chname, GET_HIT(vict), GET_MAX_HIT(vict),
		          GET_MOVE(vict), GET_MAX_MOVE(vict));
		      FREE_NAME(chname);
		      CAP(buf);
		      send_to_char(buf, f->follower);
		    }

		  if (l != ch)
		  {
		    GET_NAME_II(vict, l, chname);
		    sprintf(buf, "%s reports: %d/%dH, %d/%dV\r\n",
		        chname, GET_HIT(vict), GET_MAX_HIT(vict),
		        GET_MOVE(vict), GET_MAX_MOVE(vict));
		    FREE_NAME(chname);
		    CAP(buf);
		    send_to_char(buf, l);
		  }
		 
			GET_NAME_II(vict, ch, chname);
			sprintf(buf, "%s reports: %d/%dH, %d/%dV\r\n",
					chname, GET_HIT(vict), GET_MAX_HIT(vict),
					GET_MOVE(vict), GET_MAX_MOVE(vict));
			FREE_NAME(chname);
			CAP(buf);
			send_to_char(buf, ch);		 

		 
    } else {                    

      send_to_char("Nobody here is a loyal subject of yours!\r\n", ch);
      return;

      org_room = ch->in_room;

      for (k = ch->followers; k; k = k->next) {
        if (org_room == k->follower->in_room)
          if (IS_AFFECTED(k->follower, AFF_CHARM) || IS_AFFECTED(k->follower, AFF_TAMED)) {
            found = TRUE;

			  		l = (ch->master ? ch->master : ch);

					  for (f = l->followers; f; f = f->next)
					    if (f->follower != ch)
					    {
					      GET_NAME_II(k->follower, f->follower, chname);
					      sprintf(buf, "%s reports: %d/%dH, %d/%dV\r\n",
					          chname, GET_HIT(k->follower), GET_MAX_HIT(k->follower),
					          GET_MOVE(k->follower), GET_MAX_MOVE(k->follower));
					      FREE_NAME(chname);
					      CAP(buf);
					      send_to_char(buf, f->follower);
					    }

						  if (l != ch)
						  {
						    GET_NAME_II(k->follower, l, chname);
						    sprintf(buf, "%s reports: %d/%dH, %d/%dV\r\n",
						        chname, GET_HIT(k->follower), GET_MAX_HIT(k->follower),
						        GET_MOVE(k->follower), GET_MAX_MOVE(k->follower));
						    FREE_NAME(chname);
						    CAP(buf);
						    send_to_char(buf, l);
						  }
						 
							GET_NAME_II(k->follower, ch, chname);
							sprintf(buf, "%s reports: %d/%dH, %d/%dV\r\n",
									chname, GET_HIT(k->follower), GET_MAX_HIT(k->follower),
									GET_MOVE(k->follower), GET_MAX_MOVE(k->follower));
							FREE_NAME(chname);
							CAP(buf);
							send_to_char(buf, ch);
          }
      }
      if (found)
        send_to_char(OK, ch);
      else
        send_to_char("Nobody here is a loyal subject of yours!\r\n", ch);
    }
  }
}

ACMD(do_release)
{
  bool found = FALSE;
  room_rnum org_room;
  struct char_data *vict;
  struct follow_type *k;

  one_argument(argument, arg);

 if (GET_POS(ch) == POS_FISHING || GET_POS(ch) == POS_DIGGING) {
     send_to_char("You are not in a proper position for that!\r\n", ch);
    return;
}

 if (GET_POS(ch) == POS_RIDING) {
     send_to_char("Perhaps you should dismount first?\r\n", ch);
    return ;
}

  if (!*arg) {
    send_to_char("Who?!?\r\n", ch);
    return;
  }
  else if (!(vict = get_char_vis(ch, arg, FIND_CHAR_ROOM)))
    send_to_char("They aren't here.\r\n", ch);
  else if (ch == vict)
    send_to_char("You obviously suffer from skitzofrenia.\r\n", ch);

  else {
    if (IS_AFFECTED(ch, AFF_CHARM)) {
      send_to_char("Your superior would not aprove of you giving orders.\r\n", ch);
      return;
    }
    if (vict) {

      if (vict->master != ch)
        act("$n is not your mount!", FALSE, ch, 0, 0, TO_ROOM);

      else if (!IS_NPC(vict) || !MOB_FLAGGED(vict, MOB_MOUNTABLE))  {
      send_to_char("They are not a mount!!\r\n", ch);
      return;
     }

      else {
        act("$n releases $N's tether, freeing $M.", FALSE, ch, 0, vict, TO_ROOM);
        send_to_char(OK, ch);
        stop_follower(vict);
        affect_from_char_II(vict, SPELL_CHARM, SPELL_CHARM, 1);
             }
    } else {

      org_room = ch->in_room;

      for (k = ch->followers; k; k = k->next) {
        if (org_room == k->follower->in_room)
          if (IS_AFFECTED(k->follower, AFF_TAMED)) {
            found = TRUE;
         act("$n releases $N's tether, freeing $M.", FALSE, ch, 0, vict, TO_ROOM);
        stop_follower(vict);
        affect_from_char_II(vict, SPELL_CHARM, SPELL_CHARM, 1);

               }
      }
      if (found)
        send_to_char(OK, ch);
      else
        send_to_char("You don't seem to own a mount!\r\n", ch);
    }
  }
}

ACMD(do_mstand) {
  bool found = FALSE;
  room_rnum org_room;
  struct char_data *vict;
  struct follow_type *k;

  one_argument(argument, arg);

  if (!*arg) {
    org_room = ch->in_room;

    for (k = ch->followers; k; k = k->next) {
      if (org_room == k->follower->in_room)
        if (IS_AFFECTED(k->follower, AFF_CHARM) || IS_AFFECTED(k->follower, AFF_TAMED)) {
          found = TRUE;
          command_interpreter(k->follower, "stand");
        }
    }
    return;

  } else if (!(vict = get_char_vis(ch, arg, FIND_CHAR_ROOM)) && !is_abbrev(arg, "followers")) {
    send_to_char("That person isn't here.\r\n", ch);
  } else if (ch == vict) {
    send_to_char("You obviously suffer from skitzofrenia.\r\n", ch);

  } else {
    if (IS_AFFECTED(ch, AFF_CHARM)) {
      send_to_char("Your superior would not aprove of you giving orders.\r\n", ch);
      return;
    }

    if (vict) {
      act("$n gives $N an order.", FALSE, ch, 0, vict, TO_ROOM);

      if (vict->master != ch) {
        act("$n has an indifferent look.", FALSE, vict, 0, 0, TO_ROOM);
      } else if (!IS_NPC(vict) || !MOB_FLAGGED(vict, MOB_MOUNTABLE))  {
        send_to_char("They are not a mount!\r\n", ch);
        return;
      } else {
        send_to_char(OK, ch);
        command_interpreter(vict, "stand");
      }
    } else {
      org_room = ch->in_room;

      for (k = ch->followers; k; k = k->next) {
        if (org_room == k->follower->in_room)
          if (IS_AFFECTED(k->follower, AFF_CHARM) || IS_AFFECTED(k->follower, AFF_TAMED)) {
            found = TRUE;
            command_interpreter(k->follower, "stand");
          }
      }

      if (found)
        send_to_char(OK, ch);
      else
        send_to_char("Nobody here is a loyal subject of yours!\r\n", ch);
    }
  }
}

ACMD(do_mget) {
  char name[MAX_INPUT_LENGTH], message[MAX_INPUT_LENGTH];
  char buf[MAX_INPUT_LENGTH];
  bool found = FALSE;
  room_rnum org_room;
  struct char_data *vict;
  struct follow_type *k;

  struct obj_data *obj;
  int i;

  half_chop(argument, message, name);

 if (GET_POS(ch) == POS_FISHING || GET_POS(ch) == POS_DIGGING) {
     send_to_char("You are not in a proper position for that!\r\n", ch);
    return;
}
  if (!*name || !*message)
    send_to_char("Get what from who?\r\n", ch);
  else if (!(vict = get_char_vis(ch, name, FIND_CHAR_ROOM)) && !is_abbrev(name,
"followers"))
    send_to_char("That person isn't here.\r\n", ch);
  else if (ch == vict)
    send_to_char("You obviously suffer from skitzofrenia.\r\n", ch);
  else if (IS_CARRYING_N(ch) >= CAN_CARRY_N(ch))
    send_to_char("Your arms are already full!\r\n", ch);

  else {
    if (IS_AFFECTED(ch, AFF_CHARM)) {
      send_to_char("Your superior would not aprove of you giving orders.\r\n", ch);
      return;
    }
    if (vict) {
        if (vict->master != ch && (vict != ch))
        send_to_char("They are not your mount!\r\n", ch);
      else {
        send_to_char(OK, ch);
        if  (!(obj = get_object_in_equip_vis(vict, message, vict->equipment, &i))) {
      sprintf(buf, "They don't seem to have %s %s.\r\n", AN(message), message);
      send_to_char(buf, ch);
    }

       else if ((IS_CARRYING_W(ch) + GET_OBJ_WEIGHT(obj)) > CAN_CARRY_W(ch)) {
    act("$p: you can't carry that much weight.", FALSE, ch, obj, 0, TO_CHAR);
    }
        else if ((IS_CARRYING_W(vict) + GET_OBJ_WEIGHT(obj)) > CAN_CARRY_W(vict)) {
    act("$p: they can't carry that much weight.", FALSE, ch, obj, 0, TO_CHAR);
    }

       else {
        perform_remove(vict, i);
        perform_give(vict, ch, obj);
        
      }
    }
  }
   else { 

      org_room = ch->in_room;

      for (k = ch->followers; k; k = k->next) {
        if (org_room == k->follower->in_room)
          if (IS_AFFECTED(k->follower, AFF_CHARM) || IS_AFFECTED(k->follower, AFF_TAMED)) {
            found = TRUE;
             if  (!(obj = get_object_in_equip_vis(k->follower, message, k->follower->equipment, &i))) {
                sprintf(buf, "They don't seem to have %s %s.\r\n", AN(message), message);
                send_to_char(buf, ch);
              }
             else {
              perform_remove(k->follower, i);
              perform_give(k->follower, ch, obj);
           }
          }
      }
      if (found)
        send_to_char(OK, ch);
      else
        send_to_char("Nobody here is a loyal subject of yours!\r\n", ch);
    }
  }
}

ACMD(do_mgive)
{
  int dotmode;
  struct char_data *vict;
  struct follow_type *k;
  struct obj_data *obj, *next_obj;
  char buf[MAX_INPUT_LENGTH];
  bool found = FALSE;
  room_rnum org_room;

  argument = one_argument(argument, arg);

 if (GET_POS(ch) == POS_FISHING || GET_POS(ch) == POS_DIGGING) {
     send_to_char("You are not in a proper position for that!\r\n", ch);
    return;
}
  if (!*arg) {
    send_to_char("Give what to who?\r\n", ch);

  } else if (is_money(arg)) {
     send_to_char("You can't give money to them!.\r\n", ch);

  } else {
    one_argument(argument, buf1);
    if (!(vict = give_find_vict(ch, buf1)))
      return;
   else if (!IS_NPC(vict) || !MOB_FLAGGED(vict, MOB_MOUNTABLE))  {
      send_to_char("They are not a mount!!\r\n", ch);
      return;
   }
   else if ((vict->master != ch) && (vict != ch)) {
        send_to_char("They are not your mount!\r\n", ch);
        return;
    }


    dotmode = find_all_dots(arg);
    if (dotmode == FIND_INDIV) {
      if (!(obj = get_obj_in_list_vis(ch, arg, ch->carrying))) {
        sprintf(buf, "You don't seem to have %s %s.\r\n", AN(arg), arg);
        send_to_char(buf, ch);
      } else if (!CAN_WEAR(obj, ITEM_WEAR_ONBACK)) {
        send_to_char("You can't put that on your mount's back!\r\n", ch);
        return;
        }
       else if (GET_EQ(vict, WEAR_ONBACK_1) && GET_EQ(vict, WEAR_ONBACK_2) && GET_EQ(vict, WEAR_ONBACK_3)) {
    send_to_char("Your mount could not carry that much!\r\n", ch);
    return;
  }
      else {
        perform_give(ch, vict, obj);
        perform_wear(vict, obj, WEAR_ONBACK_1);
       }
      } else {
 
       org_room = ch->in_room;

      for (k = ch->followers; k; k = k->next) {
        if (org_room == k->follower->in_room)
          if (IS_AFFECTED(k->follower, AFF_CHARM)) {
            found = TRUE;
            if (dotmode == FIND_ALLDOT && !*arg) {
        send_to_char("All of what?\r\n", ch);
        return;
      }
      if (!ch->carrying)
        send_to_char("You don't seem to be holding anything.\r\n", ch);
      else
        for (obj = ch->carrying; obj; obj = next_obj) {
          next_obj = obj->next_content;
          if (CAN_WEAR(obj, ITEM_WEAR_ONBACK) && CAN_SEE_OBJ(ch, obj) &&
              ((dotmode == FIND_ALL || isname(arg, obj->name))))
            perform_give(ch, vict, obj);
            perform_wear(vict, obj, WEAR_ONBACK_1);
            }
          }
      }
      if (found)
        send_to_char(OK, ch);
      else
        send_to_char("Nobody here is a loyal subject of yours!\r\n", ch);
    }
}

}
     



/*ACMD(do_mgive)
{
  int dotmode;
  struct char_data *vict;
  struct obj_data *obj, *next_obj;

  argument = one_argument(argument, arg);

  if (!*arg) {
    send_to_char("Give what to who?\r\n", ch);

  } else if (is_money(arg)) {
     send_to_char("You can't give money to them!.\r\n", ch);

  } else {
    one_argument(argument, buf1);
    if (!(vict = give_find_vict(ch, buf1)))
      return;
   else if (!IS_NPC(vict) || !MOB_FLAGGED(vict, MOB_MOUNTABLE))  {
      send_to_char("They are not a mount!!\r\n", ch);
      return;
   }
  
    dotmode = find_all_dots(arg);
    if (dotmode == FIND_INDIV) {
      if (!(obj = get_obj_in_list_vis(ch, arg, ch->carrying))) {
        sprintf(buf, "You don't seem to have %s %s.\r\n", AN(arg), arg);
        send_to_char(buf, ch);
      } else if (!CAN_WEAR(obj, ITEM_WEAR_ONBACK)) {
        send_to_char("You can't put that on your mount's back!\r\n", ch);
        return;
        }
       else if (GET_EQ(ch, WEAR_ONBACK_1) && GET_EQ(ch, WEAR_ONBACK_2) && GET_EQ(ch, WEAR_ONBACK_3)) {
    send_to_char("Your mount could not carry that much!\r\n", ch);
    return;
  }
      else { 
        perform_give(ch, vict, obj);
        perform_wear(vict, obj, WEAR_ONBACK_1);
       }
      } else {
      if (dotmode == FIND_ALLDOT && !*arg) {
        send_to_char("All of what?\r\n", ch);
        return;
      }
      if (!ch->carrying)
        send_to_char("You don't seem to be holding anything.\r\n", ch);
      else
        for (obj = ch->carrying; obj; obj = next_obj) {
          next_obj = obj->next_content;
          if (CAN_WEAR(obj, ITEM_WEAR_ONBACK) && CAN_SEE_OBJ(ch, obj) &&
              ((dotmode == FIND_ALL || isname(arg, obj->name))))
            perform_give(ch, vict, obj);
            perform_wear(vict, obj, WEAR_ONBACK_1);
        }
    }
  }
}
*/


ACMD(do_sheath)
{
  char arg1[MAX_INPUT_LENGTH];
  char arg2[MAX_INPUT_LENGTH];
  struct obj_data *obj, *cont, *weapon;
  //struct obj_data *sheath, *next_obj;
  int i, obj_dotmode, cont_dotmode;
  //int found = 0;

  two_arguments(argument, arg1, arg2);
  obj_dotmode = find_all_dots(arg1);
  cont_dotmode = find_all_dots(arg2);

 if (GET_POS(ch) == POS_FISHING || GET_POS(ch) == POS_DIGGING) {
     send_to_char("You are not in a proper position for that!\r\n", ch);
    return ;
}

  if (!*arg1)
    send_to_char("Sheath what in what?\r\n", ch);
  else if (cont_dotmode != FIND_INDIV)
  send_to_char("You can only put things into one container at a time.\r\n", ch);
  else if (!*arg2) {
    sprintf(buf, "What do you want to sheathe %s in?\r\n",
            ((obj_dotmode == FIND_INDIV) ? "it" : "them"));
    send_to_char(buf, ch);
  } else if  (!(cont = get_object_in_equip_vis(ch, arg2, ch->equipment, &i)))
{
      sprintf(buf, "You must wear %s %s first.\r\n", AN(arg2), arg2);
      send_to_char(buf, ch);
    }
    else if (GET_OBJ_TYPE(cont) != ITEM_SHEATH)
      act("$p is not a sheath or scabbard.", FALSE, ch, cont, 0, TO_CHAR);
    else if (IS_SET(GET_OBJ_VAL(cont, 1), CONT_CLOSED))
      send_to_char("You'd better open it first!\r\n", ch);
    else if (cont->contains !=NULL)
      send_to_char("There's already something in there!\r\n", ch);
     else if (!(obj = get_object_in_equip_vis(ch, arg1, ch->equipment, &i))) {
        if (!(obj = get_obj_in_list_vis(ch, arg1, ch->carrying))) {
          sprintf(buf, "You aren't carrying %s %s.\r\n", AN(arg1), arg1);
          send_to_char(buf, ch);
        }

        else
      perform_sheath(ch, obj, cont);

}
     else {
    if (!(weapon = get_object_in_equip_vis(ch, arg1, ch->equipment, &i))) {
          sprintf(buf, "You aren't using %s %s.\r\n", AN(arg1), arg1);
          send_to_char(buf, ch);
        }
    else if ((weapon != GET_EQ(ch, WEAR_WIELD) || !GET_EQ(ch, WEAR_WIELD)) && (weapon != GET_EQ(ch, WEAR_HOLD) || !GET_EQ(ch, WEAR_HOLD))) {
     send_to_char("You aren't wielding or holding a weapon that can be sheathed!\r\n", ch);
      }

  else {
    if (weapon == GET_EQ(ch, WEAR_WIELD)) { 
    perform_remove(ch, WEAR_WIELD);
     if (!(obj = get_obj_in_list_vis(ch, arg1, ch->carrying))) {
          sprintf(buf, "You aren't carrying %s %s.\r\n", AN(arg1), arg1);
          send_to_char(buf, ch);
        }
     else
    perform_sheath(ch, obj, cont);

       }
    else if (weapon == GET_EQ(ch, WEAR_HOLD)) {
      perform_remove(ch, WEAR_HOLD);
     if (!(obj = get_obj_in_list_vis(ch, arg1, ch->carrying))) {
          sprintf(buf, "You aren't carrying %s %s.\r\n", AN(arg1), arg1);
          send_to_char(buf, ch);
        }
     else
    perform_sheath(ch, obj, cont);
       }

     }
   }
}

ACMD(do_castout)
{
  struct obj_data *pole;
  int fail;

 if (GET_POS(ch) == POS_RIDING || GET_POS(ch) == POS_DIGGING) {
     send_to_char("You are not in a proper position for that!\r\n", ch);
    return ;
}


  if (PLR_FLAGGED(ch, PLR_FISHING)) {
    send_to_char("You are already fishing!\r\n", ch);
    return;
  }
  if (!(pole = GET_EQ(ch, WEAR_HOLD)) ||
      (GET_OBJ_TYPE(pole) != ITEM_POLE)) {
    send_to_char("You need to be holding a fishing pole first.\r\n", ch);
    return;
  }
  if (!ROOM_FLAGGED(ch->in_room, ROOM_SALTWATER_FISH) &&
      !ROOM_FLAGGED(ch->in_room, ROOM_FRESHWATER_FISH)) {
    send_to_char("This is not a good place to fish, you'll want to find a"
                 " better spot.\r\n", ch);
    return;
  }
  fail = number(1, 10);
  if (fail <= 6) {
    send_to_char("You pull your arm back and try to cast out your line, but "
                 "it gets all tangled up.\r\nTry again.\r\n", ch);
    act("$n pulls $s arm back, trying to cast $s fishing line out into the "
        "water,\r\nbut ends up just a bit tangled.\r\n",
         FALSE, ch, 0, 0, TO_ROOM);
    return;
  }
  /* Ok, now they've gone through the checks, now set them fishing */
  SET_BIT(PLR_FLAGS(ch), PLR_FISHING);
  GET_POS(ch) = POS_FISHING;
  send_to_char("You cast your line out into the water, hoping for a bite.\r\n", ch);
  act("$n casts $s line out into the water, hoping to catch some food.\r\n",
       FALSE, ch, 0, 0, TO_ROOM);
  return;
}

ACMD(do_reel)
{
  int success, f_num, fish_num;
  struct obj_data *fish;

  GET_POS(ch) = POS_STANDING;

 if (GET_POS(ch) == POS_RIDING || GET_POS(ch) == POS_DIGGING) {
     send_to_char("You are not in a proper position for that!\r\n", ch);
    return ;
}

  if (!PLR_FLAGGED(ch, PLR_FISHING)) {
    send_to_char("You aren't even fishing!\r\n", ch);
    return;
  }
  if (!PLR_FLAGGED(ch, PLR_FISH_ON)) {
    send_to_char("You reel in your line, but alas... nothing on the end.\r\n"
                 "Better luck next time.\r\n", ch);
    REMOVE_BIT(PLR_FLAGS(ch), PLR_FISHING);
    act("$n reels $s line in, but with nothing on the end.\r\n",
        FALSE, ch, 0, 0, TO_ROOM);
    return;
  }

  /* Ok, they are fishing and have a fish on */
  success = number(1, 10);

  REMOVE_BIT(PLR_FLAGS(ch), PLR_FISHING);
  REMOVE_BIT(PLR_FLAGS(ch), PLR_FISH_ON);

  if (success <= 4) {
    send_to_char("You reel in your line, putting up a good fight, but you "
                 "lose him!\r\nTry again.\r\n", ch);
    act("$n reels $s line in, fighting with whatever is on the end, but loses "
        "the catch.\r\n", FALSE, ch, 0, 0, TO_ROOM);
    return;
  }

  /* We used object vnums 10030-10050 for our fish that people could
   * catch. The below numbers reflect that use. If you wish to change
   * the vnums of the fish, just change the numbers below. You can
   * see that we seperated the type of fish by freshwater and salt
   * water.
   */
  if (ROOM_FLAGGED(ch->in_room, ROOM_SALTWATER_FISH)) {
    fish_num = number(66, 72);
    f_num = real_object(fish_num);
    fish = read_object(f_num, REAL);
    sprintf(buf, "You reel in %s! Nice catch!\r\n", fish->short_description);
    act("Wow! $n reels in a helluva catch! Looks like $p!\r\n",
        FALSE, ch, fish, 0, TO_ROOM);
    send_to_char(buf, ch);

    if (IS_CARRYING_N(ch) >= CAN_CARRY_N(ch)) {
    act("$p: you can't carry that many items.", FALSE, ch, fish, 0, TO_CHAR);
    obj_to_room(fish, ch->in_room);

  } else if ((IS_CARRYING_W(ch) + GET_OBJ_WEIGHT(fish)) > CAN_CARRY_W(ch)) {
    act("$p: you can't carry that much weight.", FALSE, ch, fish, 0, TO_CHAR);
    obj_to_room(fish, ch->in_room);
  }

  else {
    obj_to_char(fish, ch);
   }

    return;
  } else
  if (ROOM_FLAGGED(ch->in_room, ROOM_FRESHWATER_FISH)) {
    fish_num = number(60, 66);
    f_num = real_object(fish_num);
    fish = read_object(f_num, REAL);
    sprintf(buf, "You reel in %s! Nice catch!\r\n", fish->short_description);
    act("Wow! $n reels in a helluva catch! Looks like $p!\r\n",
        FALSE, ch, fish, 0, TO_ROOM);
    send_to_char(buf, ch);

  if (IS_CARRYING_N(ch) >= CAN_CARRY_N(ch)) {
    act("$p: you can't carry that many items.", FALSE, ch, fish, 0, TO_CHAR);
    obj_to_room(fish, ch->in_room);
  
  } else if ((IS_CARRYING_W(ch) + GET_OBJ_WEIGHT(fish)) > CAN_CARRY_W(ch)) {
    act("$p: you can't carry that much weight.", FALSE, ch, fish, 0, TO_CHAR);
    obj_to_room(fish, ch->in_room);
  } 
    
  else {
    obj_to_char(fish, ch);
   }
   return;

  } else
  send_to_char("You should never see this message, please report it.\r\n", ch);
  return;
}

const char* msgs[][2] = {

{"You start to break some floor boards when you dig.\r\n",

 "$n starts to break some floor boards as $e starts digging.\r\n" },

{"You wonder if this is a good place after all, with all the gravel.\r\n",

 "$n breaks a sweat digging up all the gravel here.\r\n" },

{"You make a nice hole while digging up a lot of dirt.\r\n",

 "$n digs a hole and goes about $s business.\r\n" },

{"You seem to be hitting alot of roots when you dig.\r\n",

 "$n look like $e is trying to dig up a tree!\r\n" },

{"You dig up more clay than dirt here.\r\n",

 "$n seems to be digging up alot of clay.\r\n" },

{"You start to chip away at the rock here.\r\n",

 "$n bangs away at the side of the mountain.\r\n" },

{"You can't dig in the water!\r\n",

 NULL },

{"You can't dig in the water!\r\n",

 NULL },

{"You can't dig in the water!\r\n",

 NULL },

{"You can't dig up air!\r\n",

 NULL },

/* always keep this as the last message */

{ "If you see this please tell a god.\r\n", NULL }
};

/*
ACMD(do_bury)
{
    struct obj_data *obj;
    struct obj_data *shovel;

    half_chop(argument, arg, buf);

    if (GET_POS(ch) == POS_FISHING || GET_POS(ch) == POS_DIGGING || GET_POS(ch) == POS_RIDING) 
    {
        send_to_char("You are not in a proper position for that!\r\n", ch);
        return ;
    }
    
    if (!*arg) 
    {
        sprintf(buf2, "What do you want to %s?\r\n", CMD_NAME);
        send_to_char(buf2, ch);
        return;
    }

    if (!(shovel = GET_EQ(ch, WEAR_HOLD)) || (GET_OBJ_TYPE(shovel) != ITEM_SHOVEL)) 
    {
        send_to_char("You need to be holding a shovel first.\r\n", ch);
        return;
    }

    if (ROOM_FLAGGED(ch->in_room, ROOM_NOBURY)) 
    {
        send_to_char("This is not a good place to bury that, you'll want to find a better spot.\r\n", ch);
        return;
    }

    if(SECT(ch->in_room) == SECT_INSIDE)
    {
        send_to_char("This is not a good place to bury that, you'll want to find a better spot.\r\n", ch);
        return ;
    }

    if (!(obj = get_obj_in_list_vis(ch, arg, ch->carrying))) 
    {
        sprintf(buf, "You don't have %s %s.\r\n", AN(arg), arg);
        send_to_char(buf, ch);
        return;
    }

// find the sector types that you don't want people
// to be able to dig or bury in.

    if((world[IN_ROOM(ch)].sector_type == SECT_WATER_SWIM) || (world[IN_ROOM(ch)].sector_type == SECT_WATER_NOSWIM) ||
       (world[IN_ROOM(ch)].sector_type == SECT_UNDERWATER) || (world[IN_ROOM(ch)].sector_type == SECT_FLYING)) 
    {
        // display the messages if available


        if(msgs[world[IN_ROOM(ch)].sector_type][0] != NULL)
        {
            send_to_char(msgs[world[IN_ROOM(ch)].sector_type][0], ch);
        }


        if(msgs[world[IN_ROOM(ch)].sector_type][1] != NULL)
        {
            act(msgs[world[IN_ROOM(ch)].sector_type][1], TRUE, ch, NULL, NULL,TO_ROOM);
        }

        return;

    }

    // set a wait state
    WAIT_STATE(ch, 10 RL_SEC);

    if(msgs[world[IN_ROOM(ch)].sector_type][0] != NULL)
    {
        send_to_char(msgs[world[IN_ROOM(ch)].sector_type][0], ch);
    }

    if(msgs[world[IN_ROOM(ch)].sector_type][1] != NULL)
    {
        act(msgs[world[IN_ROOM(ch)].sector_type][1], TRUE, ch, NULL, NULL,TO_ROOM);
    }

    act("You bury $a $o here.\r\n", TRUE, ch, obj, NULL, TO_CHAR);

    act("$n buries $a $o here.\r\n", TRUE, ch, obj, NULL, TO_ROOM);


    obj_from_char(obj);

    SET_BIT(GET_OBJ_EXTRA(obj), ITEM_HIDDEN);

    obj_to_room(obj, IN_ROOM(ch));
};
*/

ACMD(do_dig)
{
  struct obj_data *shovel;
  int fail;

  if (PLR_FLAGGED(ch, PLR_DIGGING)) {
    send_to_char("You are already digging!\r\n", ch);
    return;
  }

 if (GET_POS(ch) == POS_FISHING || GET_POS(ch) == POS_RIDING) {
     send_to_char("You are not in a proper position for that!\r\n", ch);
    return ;
}
  if (!(shovel = GET_EQ(ch, WEAR_HOLD)) ||
      (GET_OBJ_TYPE(shovel) != ITEM_SHOVEL)) {
    send_to_char("You need to be holding a shovel first.\r\n", ch);
    return;
  }
  if (ROOM_FLAGGED(ch->in_room, ROOM_NODIG)) {
    send_to_char("This is not a good place to dig, you'll want to find a"
                 " better spot.\r\n", ch);
    return;
  }

   if(SECT(ch->in_room) == SECT_INSIDE)
    {
    send_to_char("This is not a good place to dig, you'll want to find a"
                 " better spot.\r\n", ch);
     return ;
    }

  fail = number(1, 10);
  if (fail <= 4) {
    send_to_char("Your shovel seems to be stuck in the ground.\r\nTry again.\r\n", ch);
    act("$n's shovel seems to be stuck in the ground.\r\n",
         FALSE, ch, 0, 0, TO_ROOM);
    return;
  }

  /* Ok, now they've gone through the checks, now set them fishing */
  SET_BIT(PLR_FLAGS(ch), PLR_DIGGING);
  GET_POS(ch) = POS_DIGGING;

if ((world[IN_ROOM(ch)].sector_type == SECT_WATER_SWIM) ||
(world[IN_ROOM(ch)].sector_type == SECT_WATER_NOSWIM) ||
(world[IN_ROOM(ch)].sector_type == SECT_UNDERWATER) ||
(world[IN_ROOM(ch)].sector_type == SECT_FLYING)) {


/* display the messages if available */
if(msgs[world[IN_ROOM(ch)].sector_type][0] != NULL)
send_to_char(msgs[world[IN_ROOM(ch)].sector_type][0], ch);
if(msgs[world[IN_ROOM(ch)].sector_type][1] != NULL)
act(msgs[world[IN_ROOM(ch)].sector_type][1], TRUE, ch, NULL, NULL,TO_ROOM);
return;

  }

else {
send_to_char("You start to dig industriously with your shovel.\r\n", ch);
  act("$n starts to dig industriously with $s shovel.\r\n", FALSE, ch, 0, 0, TO_ROOM);
}

}

ACMD(do_unearth)
{

int success, j_num, junk_num;
struct obj_data *obj, *next_o;
struct obj_data *junk;
int found_item = 0;

 if (GET_POS(ch) == POS_FISHING || GET_POS(ch) == POS_RIDING) {
     send_to_char("You are not in a proper position for that!\r\n", ch);
    return ;
}
obj = world[IN_ROOM(ch)].contents;

GET_POS(ch) = POS_STANDING;

if (obj == NULL ||!PLR_FLAGGED(ch, PLR_DIG_ON)) {
send_to_char("You bend down swiftly to claim your find, but alas, there seems to be nothing there.\r\n"
             "Better luck next time.\r\n", ch);
REMOVE_BIT(PLR_FLAGS(ch), PLR_DIGGING);

act("$n bends down swiftly to claim $s find , but alas, there seems to be nothing there.\r\n", FALSE, ch, 0, 0, TO_ROOM);
}

else if (obj == NULL && PLR_FLAGGED(ch, PLR_DIG_ON)) {
    REMOVE_BIT(PLR_FLAGS(ch), PLR_DIGGING);
    REMOVE_BIT(PLR_FLAGS(ch), PLR_DIG_ON);
    GET_POS(ch) = POS_STANDING;
    junk_num = number(17, 19);
    j_num = real_object(junk_num);
    junk = read_object(j_num, REAL);
    sprintf(buf, "You have unearthed %s!\r\n", junk->short_description);
    act("Wow! $n has unearth something that looks like $p!\r\n",
        FALSE, ch, junk, 0, TO_ROOM);

    send_to_char(buf, ch);

  if (IS_CARRYING_N(ch) >= CAN_CARRY_N(ch)) {
    act("$p: you can't carry that many items.", FALSE, ch, junk, 0, TO_CHAR);
    obj_to_room(junk, ch->in_room);

  } else if ((IS_CARRYING_W(ch) + GET_OBJ_WEIGHT(junk)) > CAN_CARRY_W(ch)) {
    act("$p: you can't carry that much weight.", FALSE, ch, junk, 0, TO_CHAR);
    obj_to_room(junk, ch->in_room);
  }

  else { 
    obj_to_char(junk, ch);
    get_check_money(ch, junk);
   }

    return;
}

else if (obj != NULL && PLR_FLAGGED(ch, PLR_DIG_ON)) {

REMOVE_BIT(PLR_FLAGS(ch), PLR_DIGGING);
REMOVE_BIT(PLR_FLAGS(ch), PLR_DIG_ON);

obj = world[IN_ROOM(ch)].contents;

success = number(1, 10);

REMOVE_BIT(PLR_FLAGS(ch), PLR_DIGGING);
REMOVE_BIT(PLR_FLAGS(ch), PLR_DIG_ON);

if (success <=4) {
    junk_num = number(17, 19);
    j_num = real_object(junk_num);
    junk = read_object(j_num, REAL);
    sprintf(buf, "You have unearthed %s!\r\n", junk->short_description);
    act("Wow! $n has unearth something that looks like $p!\r\n",
        FALSE, ch, junk, 0, TO_ROOM);
   
    send_to_char(buf, ch);

   if (IS_CARRYING_N(ch) >= CAN_CARRY_N(ch)) {
    act("$p: you can't carry that many items.", FALSE, ch, junk, 0, TO_CHAR);
    obj_to_room(junk, ch->in_room);

  } else if ((IS_CARRYING_W(ch) + GET_OBJ_WEIGHT(junk)) > CAN_CARRY_W(ch)) {
    act("$p: you can't carry that much weight.", FALSE, ch, junk, 0, TO_CHAR);
    obj_to_room(junk, ch->in_room);
  }

  else {
    obj_to_char(junk, ch);
    get_check_money(ch, junk);
   }

    return;
}

else {

for (obj = world[ch->in_room].contents; obj; obj = next_o) {
if(IS_HIDDEN(obj))
{
REMOVE_BIT(GET_OBJ_EXTRA(obj), ITEM_HIDDEN);

if(CAN_SEE_OBJ(ch, obj)) {
found_item = 1;     /* player found something */


act("You unearth $a $o buried here!\r\n", TRUE, ch, obj, NULL, TO_CHAR);

act("$n has unearthed $a $o buried here!\r\n", TRUE, ch, obj, NULL,TO_ROOM);
  
obj_from_room(obj);

if (IS_CARRYING_N(ch) >= CAN_CARRY_N(ch)) {
    act("$p: you can't carry that many items.", FALSE, ch, obj, 0, TO_CHAR);
    obj_to_room(obj, ch->in_room);

  } else if ((IS_CARRYING_W(ch) + GET_OBJ_WEIGHT(obj)) > CAN_CARRY_W(ch)) {
    act("$p: you can't carry that much weight.", FALSE, ch, obj, 0, TO_CHAR);
    obj_to_room(obj, ch->in_room);
  }

  else {
    obj_to_char(obj, ch);
    get_check_money(ch, obj);
   }
}

else {
/* add the bit back becuase the player can't unbury what
* what he can't find...  */
SET_BIT(GET_OBJ_EXTRA(obj), ITEM_HIDDEN);
}
return;
}

else {
   next_o = obj->next_content;
}
}

if(!found_item)  {
           send_to_char("You bend down swiftly to claim your find, but alas, there seems to be nothing of interest there.\r\n"
             "Better luck next time.\r\n", ch);
act("$n bends down swiftly to claim $s find , but alas, there seems to be nothing there.\r\n", FALSE, ch, 0, 0, TO_ROOM);
  }
}
}

else {
send_to_char("You bend down swiftly to claim your find, but alas, there seems to be nothing of interest there.\r\n"
             "Better luck next time.\r\n", ch);
REMOVE_BIT(PLR_FLAGS(ch), PLR_DIGGING);

act("$n bends down swiftly to claim $s find , but alas, there seems to be nothing there.\r\n", FALSE, ch, 0, 0, TO_ROOM);
}
}

ACMD(do_enchant)
{

  char arg1[MAX_STRING_LENGTH], arg2[MAX_STRING_LENGTH];
  struct obj_data *obj1, *obj2;
  //struct extra_descr_data *this, *temp, *temp2;
  char buf1[MAX_STRING_LENGTH];
  int j;
  //int sameWear = FALSE;
	
	two_arguments(argument, arg1, arg2);
	
	if (GET_SKILL(ch, SKILL_ENCHANT_ITEM) < 1) {
	  send_to_char("You don't know how to enchant other items.\r\n", ch);
		return;
	}	

        if (GET_CLASS(ch) != CLASS_DRUID) {
          send_to_char("Only druids are able to enchant items with this skill.\r\n", ch);
          return;
        }
	
	if (!*arg1) {
	  send_to_char("Syntax: enchant <item with stats you want> <item with descs you want>\r\n", ch);
		return;
	}

	if (!*arg2) {
	  send_to_char("Syntax: enchant <item with stats you want> <item with descs you want>\r\n", ch);
		return;
	}
	
	if (!(obj1 = get_obj_in_list_vis(ch, arg1, ch->carrying))) {
	  send_to_char("The item you are trying to enchant to is not in your inventory.\r\n", ch);
		return;
	}
	
	if (!(obj2 = get_obj_in_list_vis(ch, arg2, ch->carrying))) {
	  send_to_char("The item you are trying to enchant from is not in your inventory.\r\n", ch);
		return;
	}

        if (obj1 == obj2) {
          send_to_char("You cannot enchant an item with itself.\r\n", ch);
          return;
        }

        if (GET_OBJ_LEVEL(obj1) > GET_LEVEL(ch) + (GET_SKILL(ch, SKILL_ENCHANT_ITEM) / 4)) {
          act("$p is too high level for you to enchant.", TRUE, ch, obj1, ch, TO_CHAR);
          return;
        }

        if (GET_OBJ_LEVEL(obj2) > GET_LEVEL(ch) + (GET_SKILL(ch, SKILL_ENCHANT_ITEM) / 4)) {
          act("$p is too high level for you to enchant.", TRUE, ch, obj2, ch, TO_CHAR);
          return;
        }

	if (GET_OBJ_TYPE(obj1) != GET_OBJ_TYPE(obj2)) {
	  send_to_char("When enchanting, both items must be of the same type.\r\n", ch);
		return;
	}

       if (OBJ_WEAR(obj1) != OBJ_WEAR(obj2)) {
         send_to_char("Only objects whose wear locations are the same can be enchanted.\r\n", ch);
         return;  
       }


	if (!has_materials(ch, SKILL_ENCHANT_ITEM)) {
	  send_to_char("You need a perfectly cut diamond to enchant another item.\r\n", ch);
		return;
	}



	if (!check_skill(ch, 101, SKILL_ENCHANT_ITEM)) {
	  send_to_char("You failed the enchantment process.\r\n", ch);
  	  act("$n tried to enchant $p but failed.", TRUE, ch, obj2, ch, TO_NOTVICT);
          if (!check_skill(ch, 101, SKILL_ENCHANT_ITEM)) {
		act("$p crumbles to dust.", TRUE, ch, obj1, 0, TO_CHAR);
		act("$p crumbles to dust.", TRUE, 0, obj1, 0, TO_ROOM);
		obj_from_char(obj1);
		extract_obj(obj1);
          }
          if (!check_skill(ch, 101, SKILL_ENCHANT_ITEM)) {
		act("$p crumbles to dust.", TRUE, ch, obj2, 0, TO_CHAR);		
		act("$p crumbles to dust.", TRUE, 0, obj2, 0, TO_ROOM);
		obj_from_char(obj2);
		extract_obj(obj2);
          }
		return;
	}

	
  sprintf(buf1, "You have enchanted %s with the attributes of %s\r\n", obj2->short_description, obj1->short_description);
	act(buf1, TRUE, ch, 0, 0, TO_CHAR);
  sprintf(buf1, "$n has enchanted %s with the attributes of %s\r\n", obj2->short_description, obj1->short_description);
	act(buf1, TRUE, ch, 0, ch, TO_NOTVICT);
	
  

    GET_OBJ_VAL(obj2, 0) = GET_OBJ_VAL(obj1, 0);
    GET_OBJ_VAL(obj2, 1) = GET_OBJ_VAL(obj1, 1);
    GET_OBJ_VAL(obj2, 2) = GET_OBJ_VAL(obj1, 2);
    GET_OBJ_VAL(obj2, 3) = GET_OBJ_VAL(obj1, 3);
    GET_OBJ_EXTRA(obj2) = GET_OBJ_EXTRA(obj1);
    GET_OBJ_WEIGHT(obj2) = GET_OBJ_WEIGHT(obj2);
    GET_OBJ_TIMER(obj2) = GET_OBJ_TIMER(obj2);
    obj2->obj_flags.bitvector = obj1->obj_flags.bitvector;

    for (j = 0; j < MAX_OBJ_AFFECT; j++)
      obj2->affected[j] = obj1->affected[j];

	obj_from_char(obj1);
	extract_obj(obj1);	
}

ACMD(do_backupgear)
{

  send_to_char("Your gear has been backed up to file.\r\n", ch);
  Crash_crashsave(ch, TRUE);

}
