/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *                                                                         *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael          *
 *  Chastain, Michael Quan, and Mitchell Tse.                              *
 *                                                                         *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc       *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.                                               *
 *                                                                         *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 ***************************************************************************/

/***************************************************************************
 *  The MOBprograms have been contributed by N'Atas-ha.  Any support for   *
 *  these routines should not be expected from Merc Industries.  However,  *
 *  under no circumstances should the blame for bugs, etc be placed on     *
 *  Merc Industries.  They are not guaranteed to work on all systems due   *
 *  to their frequent use of strxxx functions.  They are also not the most *
 *  efficient way to perform their tasks, but hopefully should be in the   *
 *  easiest possible way to install and begin using. Documentation for     *
 *  such installation can be found in INSTALL.  Enjoy........    N'Atas-Ha *
 ***************************************************************************/

#include "conf.h"
#include "sysdep.h"


#include "structs.h"
#include "dg_scripts.h"
#include "db.h"
#include "utils.h"
#include "handler.h"
#include "interpreter.h"
#include "comm.h"
#include "spells.h"


extern struct descriptor_data *descriptor_list;
extern sh_int find_target_room(char_data * ch, char *rawroomstr);
extern struct index_data *mob_index;
extern struct room_data *world;

void sub_write(char *arg, char_data *ch, byte find_invis, int targets);

/*
 * Local functions.
 */

/* attaches mob's name and vnum to msg and sends it to script_log */
void mob_log(char_data *mob, char *msg)
{
    char buf[MAX_INPUT_LENGTH + 100];

    void script_log(char *msg);

    sprintf(buf, "Mob (%s, VNum %d): %s",
	    GET_SHORT(mob), GET_MOB_VNUM(mob), msg);
    script_log(buf);
}
/*
** macro to determine if a mob is permitted to use these commands
*/
#define MOB_OR_IMPL(ch) \
  (IS_NPC(ch) && (!(ch)->desc || GET_LEVEL((ch)->desc->original)>=LVL_IMPL))



/* mob commands */

/* prints the argument to all the rooms aroud the mobile */
ACMD(do_masound)
{
    sh_int was_in_room;
    int  door;
  
    if (!MOB_OR_IMPL(ch))
    {
	send_to_char("Huh?!?\r\n", ch);
	return;
    }

    if (AFF_FLAGGED(ch, AFF_CHARM))
	return;
  
    if (!*argument)
    {
	mob_log(ch, "masound called with no argument");
	return;
    }

    skip_spaces(&argument);
  
    was_in_room = IN_ROOM(ch);
    for (door = 0; door < NUM_OF_DIRS; door++)
    {
	struct room_direction_data *exit;
    
	if (((exit = world[was_in_room].dir_option[door]) != NULL) &&
	    exit->to_room != NOWHERE && exit->to_room != was_in_room)
	{
	    IN_ROOM(ch) = exit->to_room;
	    sub_write(argument, ch, TRUE, TO_ROOM);
	}
    }
  
    IN_ROOM(ch) = was_in_room;
}


/* lets the mobile kill any player or mobile without murder*/
ACMD(do_mkill)
{
    char arg[MAX_INPUT_LENGTH];
    char_data *victim;
  
    if (!MOB_OR_IMPL(ch)) {
	send_to_char("Huh?!?\r\n", ch);
	return;
    }

    if (AFF_FLAGGED(ch, AFF_CHARM))
	return;
  
    one_argument(argument, arg);
    
    if (!*arg) {
	mob_log(ch, "mkill called with no argument");
	return;
    }

    if (*arg == UID_CHAR) {
      if (!(victim = get_char(arg))) {
        sprintf(buf, "mkill: victim (%s) not found",arg);
	mob_log(ch, buf);
	return;
      }
    } else if (!(victim = get_char_vis(ch, arg, FIND_CHAR_ROOM))) {
        sprintf(buf, "mkill: victim (%s) not found",arg);
	mob_log(ch, buf);
	return;
    }

    if (victim == ch) {
	mob_log(ch, "mkill: victim is self");
	return;
    }

    if (IS_AFFECTED(ch, AFF_CHARM) && ch->master == victim ) {
	mob_log(ch, "mkill: charmed mob attacking master");
	return;
    }

    if (FIGHTING(ch)) {
	mob_log(ch, "mkill: already fighting");
	return;
    }
    
    hit(ch, victim, TYPE_UNDEFINED);
    return;
}


/*
 * lets the mobile destroy an object in its inventory
 * it can also destroy a worn object and it can destroy 
 * items using all.xxxxx or just plain all of them
 */
ACMD(do_mjunk)
{
    char arg[MAX_INPUT_LENGTH];
    int pos;
    obj_data *obj;
    obj_data *obj_next;
  
    if (!MOB_OR_IMPL(ch)) {
	send_to_char("Huh?!?\r\n", ch);
	return;
    }
  
    if (AFF_FLAGGED(ch, AFF_CHARM))
	return;

    one_argument(argument, arg);
  
    if (!*arg) {
	mob_log(ch, "mjunk called with no argument");
	return;
    }
  
    if (find_all_dots(arg) != FIND_INDIV) {
	if ((obj=get_object_in_equip_vis(ch,arg,ch->equipment,&pos))!= NULL) {
	    unequip_char(ch, pos);
	    extract_obj(obj);
	    return;
	}
	if ((obj = get_obj_in_list_vis(ch, arg, ch->carrying)) != NULL )
	    extract_obj(obj);
	return;
    } else {
	for (obj = ch->carrying; obj != NULL; obj = obj_next) {
	    obj_next = obj->next_content;
	    if (arg[3] == '\0' || isname(arg+4, obj->name)) {
		extract_obj(obj);
	    }
	}
	while ((obj=get_object_in_equip_vis(ch,arg,ch->equipment,&pos)))
	{
	    unequip_char(ch, pos);
	    extract_obj(obj);
	}   
    }
    return;
}


/* prints the message to everyone in the room other than the mob and victim */
ACMD(do_mechoaround)
{
    char arg[MAX_INPUT_LENGTH];
    char_data *victim;
    char *p;

    if (!MOB_OR_IMPL(ch)) {
	send_to_char( "Huh?!?\r\n", ch );
	return;
    }
  
    if (AFF_FLAGGED(ch, AFF_CHARM))
	return;
  
    p = one_argument(argument, arg);
    skip_spaces(&p);
    
    if (!*arg) {
	mob_log(ch, "mechoaround called with no argument");
	return;
    }
  
    if (*arg == UID_CHAR) {
      if (!(victim = get_char(arg))) {
        sprintf(buf, "mechoaround: victim (%s) does not exist",arg);
	mob_log(ch, buf);
	return;
      }
    } else if (!(victim = get_char_vis(ch, arg, FIND_CHAR_ROOM))) {
        sprintf(buf, "mechoaround: victim (%s) does not exist",arg);
	mob_log(ch, buf);
	return;
    }

    sub_write(p, victim, TRUE, TO_ROOM);
}


/* sends the message to only the victim */
ACMD(do_msend)
{
    char arg[MAX_INPUT_LENGTH];
    char_data *victim;
    char *p;

    if (!MOB_OR_IMPL(ch)) {
	send_to_char( "Huh?!?\r\n", ch );
	return;
    }

    if (AFF_FLAGGED(ch, AFF_CHARM))
	return;
  
    p = one_argument(argument, arg);
    skip_spaces(&p);
  
    if (!*arg) {
	mob_log(ch, "msend called with no argument");
	return;
    }

    if (*arg == UID_CHAR) {
      if (!(victim = get_char(arg))) {
        sprintf(buf, "msend: victim (%s) does not exist",arg);
	mob_log(ch, buf);
	return;
      }
    } else if (!(victim = get_char_vis(ch, arg, FIND_CHAR_ROOM))) {
        sprintf(buf, "msend: victim (%s) does not exist",arg);
	mob_log(ch, buf);
	return;
    }
  
    sub_write(p, victim, TRUE, TO_CHAR);
}


/* prints the message to the room at large */
ACMD(do_mecho)
{
    char *p;
  
    if (!MOB_OR_IMPL(ch)) {
	send_to_char( "Huh?!?\r\n", ch );
	return;
    }
  
    if (AFF_FLAGGED(ch, AFF_CHARM))
	return;
  
    if (!*argument) {
	mob_log(ch, "mecho called with no arguments");
	return;
    }
    p = argument;
    skip_spaces(&p);
  
    sub_write(p, ch, TRUE, TO_ROOM);
}


/*
 * lets the mobile load an item or mobile.  All items
 * are loaded into inventory, unless it is NO-TAKE. 
 */
ACMD(do_mload)
{
    char arg1[MAX_INPUT_LENGTH], arg2[MAX_INPUT_LENGTH];
    int number = 0;
    char_data *mob;
    obj_data *object;
  
    if (!MOB_OR_IMPL(ch)) {
	send_to_char("Huh?!?\r\n", ch);
	return;
    }

    if (AFF_FLAGGED(ch, AFF_CHARM))
	return;

    if( ch->desc && GET_LEVEL(ch->desc->original) < LVL_IMPL)
	return;
  
    two_arguments(argument, arg1, arg2);
  
    if (!*arg1 || !*arg2 || !is_number(arg2) || ((number = atoi(arg2)) < 0)) {
	mob_log(ch, "mload: bad syntax");
	return;
    }

    if (is_abbrev(arg1, "mob")) {
	if ((mob = read_mobile(number, VIRTUAL)) == NULL) {
	    mob_log(ch, "mload: bad mob vnum");
	    return;
	}
	char_to_room(mob, IN_ROOM(ch));
    }
  
    else if (is_abbrev(arg1, "obj")) {
	if ((object = read_object(number, VIRTUAL)) == NULL) {
	    mob_log(ch, "mload: bad object vnum");
	    return;
	}
	if (CAN_WEAR(object, ITEM_WEAR_TAKE)) {
	    obj_to_char(object, ch);
	} else {
	    obj_to_room(object, IN_ROOM(ch));
	} 
    }

    else
	mob_log(ch, "mload: bad type");
}


/*
 * lets the mobile purge all objects and other npcs in the room,
 * or purge a specified object or mob in the room.  It can purge
 *  itself, but this will be the last command it does.
 */
ACMD(do_mpurge)
{
    char arg[MAX_INPUT_LENGTH];
    char_data *victim;
    obj_data  *obj;
  
    if (!MOB_OR_IMPL(ch)) {
	send_to_char("Huh?!?\r\n", ch);
	return;
    }
  
    if (AFF_FLAGGED(ch, AFF_CHARM))
	return;

    if (ch->desc && (GET_LEVEL(ch->desc->original) < LVL_IMPL))
	return;
  
    one_argument(argument, arg);
  
    if (!*arg) {
	/* 'purge' */
	char_data *vnext;
	obj_data  *obj_next;
    
	for (victim = world[IN_ROOM(ch)].people; victim; victim = vnext) {
	    vnext = victim->next_in_room;
	    if (IS_NPC(victim) && victim != ch)
		extract_char(victim);
	}
    
	for (obj = world[IN_ROOM(ch)].contents; obj; obj = obj_next) {
	    obj_next = obj->next_content;
	    extract_obj(obj);
	}
    
	return;
    }
  
    if (*arg == UID_CHAR)
      victim = get_char(arg);
    else victim = get_char_vis(ch, arg, FIND_CHAR_ROOM);

    if (victim == NULL) {
	if ((obj = get_obj_vis(ch, arg))) {
	    extract_obj(obj);
	} else 
	    mob_log(ch, "mpurge: bad argument");
	
	return;
    }
    
    if (!IS_NPC(victim)) {
	mob_log(ch, "mpurge: purging a PC");
	return;
    }
    
    extract_char(victim);
}


/* lets the mobile goto any location it wishes that is not private */
ACMD(do_mgoto)
{
    char arg[MAX_INPUT_LENGTH];
    sh_int location;
  
    if (!MOB_OR_IMPL(ch)) {
	send_to_char("Huh?!?\r\n", ch);
	return;
    }

    if (AFF_FLAGGED(ch, AFF_CHARM))
	return;
  
    one_argument(argument, arg);

    if (!*arg) {
	mob_log(ch, "mgoto called with no argument");
	return;
    }
  
    if ((location = find_target_room(ch, arg)) == NOWHERE) {
	mob_log(ch, "mgoto: invalid location");
	return;
    }
  
    if (FIGHTING(ch))
	stop_fighting(ch);
    
    char_from_room(ch);
    char_to_room(ch, location);
}


/* lets the mobile do a command at another location. Very useful */
ACMD(do_mat)
{
    char arg[MAX_INPUT_LENGTH];
    sh_int location;
    sh_int original;
  
    if (!MOB_OR_IMPL(ch)) {
	send_to_char("Huh?!?\r\n", ch);
	return;
    }
  
    if (AFF_FLAGGED(ch, AFF_CHARM))
	return;
  
    argument = one_argument( argument, arg );
  
    if (!*arg || !*argument) {
	mob_log(ch, "mat: bad argument");
	return;
    }
  
    if ((location = find_target_room(ch, arg)) == NOWHERE) {
	mob_log(ch, "mat: invalid location");
	return;
    }
  
    original = IN_ROOM(ch);
    char_from_room(ch);
    char_to_room(ch, location);
    command_interpreter(ch, argument);
    
    /*
     * See if 'ch' still exists before continuing!
     * Handles 'at XXXX quit' case.
     */
    if (IN_ROOM(ch) == location) {
	char_from_room(ch);
	char_to_room(ch, original);
    }
}


/*
 * lets the mobile transfer people.  the all argument transfers
 * everyone in the current room to the specified location
 */
ACMD(do_mteleport)
{
    char arg1[MAX_INPUT_LENGTH], arg2[MAX_INPUT_LENGTH];
    sh_int target;
    char_data *vict, *next_ch;
  
    if (!MOB_OR_IMPL(ch)) {
	send_to_char("Huh?!?\r\n", ch);
	return;
    }
  
    if (AFF_FLAGGED(ch, AFF_CHARM))
	return;

    argument = two_arguments(argument, arg1, arg2);
    
    if (!*arg1 || !*arg2) {
	mob_log(ch, "mteleport: bad syntax");
	return;
    }

    target = find_target_room(ch, arg2);

    if (target == NOWHERE)
	mob_log(ch, "mteleport target is an invalid room");

    else if (!str_cmp(arg1, "all")) {
	if (target == IN_ROOM(ch)) {
	    mob_log(ch, "mteleport all target is itself");
	    return;
	}

	for (vict = world[IN_ROOM(ch)].people; vict; vict = next_ch) {
	    next_ch = vict->next_in_room;

	    if (GET_LEVEL(vict)<LVL_IMMORT) {
		char_from_room(vict);
		char_to_room(vict, target);
	    }
	}
    }

    else {
      if (*arg1 == UID_CHAR) {
        if (!(vict = get_char(arg1))) {
          sprintf(buf, "mteleport: victim (%s) does not exist",arg1);
          mob_log(ch, buf);
          return;
        }
      } else if (!(vict = get_char_vis(ch, arg1, FIND_CHAR_WORLD))) {
        sprintf(buf, "mteleport: victim (%s) does not exist",arg1);
	mob_log(ch, buf);
	return;
      }

      if (GET_LEVEL(vict)<LVL_IMMORT) {
	char_from_room(vict);
	char_to_room(vict, target);
      }
    }
}


/*
 * lets the mobile force someone to do something.  must be mortal level
 * and the all argument only affects those in the room with the mobile
 */
ACMD(do_mforce)
{
    char arg[MAX_INPUT_LENGTH];

    if (!MOB_OR_IMPL(ch)) {
	send_to_char("Huh?!?\r\n", ch);
	return;
    }
  
    if (AFF_FLAGGED(ch, AFF_CHARM))
	return;
  
    if (ch->desc && (GET_LEVEL(ch->desc->original) < LVL_IMPL))
	return;
  
    argument = one_argument(argument, arg);
  
    if (!*arg || !*argument) {
	mob_log(ch, "mforce: bad syntax");
	return;
    }
  
    if (!str_cmp(arg, "all")) {
	struct descriptor_data *i;
	char_data *vch;
    
	for (i = descriptor_list; i ; i = i->next) {
	    if ((i->character != ch) && STATE(i) == CON_PLAYING &&
		(IN_ROOM(i->character) == IN_ROOM(ch))) {
		vch = i->character;
		if (GET_LEVEL(vch) < GET_LEVEL(ch) && CAN_SEE(ch, vch) &&
		    GET_LEVEL(vch)<LVL_IMMORT) {
		    command_interpreter(vch, argument);
		}
	    }
	}
    } else {
	char_data *victim;
	
        if (*arg == UID_CHAR) {
          if (!(victim = get_char(arg))) {
            sprintf(buf, "mforce: victim (%s) does not exist",arg);
	    mob_log(ch, buf);
	    return;
          }
	} else if ((victim = get_char_vis(ch, arg, FIND_CHAR_ROOM)) == NULL) {
	    mob_log(ch, "mforce: no such victim");
	    return;
	}
    
	if (victim == ch) {
	    mob_log(ch, "mforce: forcing self");
	    return;
	}
    
	if (GET_LEVEL(victim)<LVL_IMMORT)
	    command_interpreter(victim, argument);
    }
}


/* increases the target's exp */
ACMD(do_mexp)
{
    char_data *victim;
    char name[MAX_INPUT_LENGTH], amount[MAX_INPUT_LENGTH];

    if (!MOB_OR_IMPL(ch)) {
	send_to_char("Huh?!?\r\n", ch);
	return;
    }
  
    if (AFF_FLAGGED(ch, AFF_CHARM))
	return;
  
    if (ch->desc && (GET_LEVEL(ch->desc->original) < LVL_IMPL))
	return;  

    two_arguments(argument, name, amount);

    if (!*name || !*amount) {
	mob_log(ch, "mexp: too few arguments");
	return;
    }

    if (*name == UID_CHAR) {
      if (!(victim = get_char(name))) {
        sprintf(buf, "mexp: victim (%s) does not exist",name);
	mob_log(ch, buf);
	return;
      }
    } else if (!(victim = get_char_vis(ch, name, FIND_CHAR_WORLD))) {
        sprintf(buf, "mexp: victim (%s) does not exist",name);
	mob_log(ch, buf);
	return;
    }
  
    gain_exp(victim, atoi(amount));      
}


/* increases the target's gold */
ACMD(do_mgold)
{
    char_data *victim;
    char name[MAX_INPUT_LENGTH], amount[MAX_INPUT_LENGTH];

    if (!MOB_OR_IMPL(ch)) {
	send_to_char("Huh?!?\r\n", ch);
	return;
    }
  
    if (AFF_FLAGGED(ch, AFF_CHARM))
	return;
  
    if (ch->desc && (GET_LEVEL(ch->desc->original) < LVL_IMPL))
	return;  

    two_arguments(argument, name, amount);

    if (!*name || !*amount) {
	mob_log(ch, "mgold: too few arguments");
	return;
    }

    if (*name == UID_CHAR) {
      if (!(victim = get_char(name))) {
        sprintf(buf, "mgold: victim (%s) does not exist",name);
	mob_log(ch, buf);
	return;
      }
    } else if (!(victim = get_char_vis(ch, name, FIND_CHAR_WORLD))) {
        sprintf(buf, "mgold: victim (%s) does not exist",name);
	mob_log(ch, buf);
	return;
    }
  
    if ( ( (GET_MONEY(victim)).copper += atoi(amount)) < 0 ) {
	mob_log(ch, "mgold subtracting more gold than character has");
	(GET_MONEY(victim)).platinum = 0;
	(GET_MONEY(victim)).steel = 0;
	(GET_MONEY(victim)).gold = 0;
	(GET_MONEY(victim)).copper = 0;
    }
}
