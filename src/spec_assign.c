/* ************************************************************************
*   File: spec_assign.c                                 Part of CircleMUD *
*  Usage: Functions to assign function pointers to objs/mobs/rooms        *
*                                                                         *
*  All rights reserved.  See license.doc for complete information.        *
*                                                                         *
*  Copyright (C) 1993, 94 by the Trustees of the Johns Hopkins University *
*  CircleMUD is based on DikuMUD, Copyright (C) 1990, 1991.               *
************************************************************************ */

#include "conf.h"
#include "sysdep.h"

#include "structs.h"
#include "db.h"
#include "interpreter.h"
#include "utils.h"

extern struct room_data *world;
extern room_rnum top_of_world;
/* extern int top_of_world;*/
extern int mini_mud;
extern struct index_data *mob_index;
extern struct index_data *obj_index;
extern struct char_data *mob_proto;
extern int top_of_mobt;

/* functions to perform assignments */

void ASSIGNMOB(int mob, SPECIAL(fname))
{
/*  if (real_mobile(mob) >= 0)
    mob_index[real_mobile(mob)].func = fname;*/
int rnum;

 if ((rnum = real_mobile(mob)) >= 0)
  mob_index[rnum].func = fname;    
  else if (!mini_mud) {
    sprintf(buf, "SYSERR: Attempt to assign spec to non-existant mob #%d",
	    mob);
    log(buf);
  }
}

void ASSIGNOBJ(int obj, SPECIAL(fname))
{
  if (real_object(obj) >= 0)
    obj_index[real_object(obj)].func = fname;
      else if (!mini_mud) {
    sprintf(buf, "SYSERR: Attempt to assign spec to non-existant obj #%d",
	    obj);
    log(buf);
  }
}

void ASSIGNROOM(int room, SPECIAL(fname))
{
  if (real_room(room) >= 0)
    world[real_room(room)].func = fname;
  else if (!mini_mud) {
    sprintf(buf, "SYSERR: Attempt to assign spec to non-existant rm. #%d",
	    room);
    log(buf);
  }
}

/* ********************************************************************
*  Assignments                                                        *
******************************************************************** */


int assign_spec(int mob, int spec);

void assign_mobiles(void)
{
  int i, spec;
  
  for (i = 0; i <= top_of_mobt; i++)
  {
    spec = mob_proto[i].mob_specials.spec;
    if (spec)
      assign_spec(mob_index[i].virtual, spec);
  }
}

int assign_spec(int mob, int spec)
{
  int rnum = real_mobile(mob);
  SPECIAL(postmaster);
  SPECIAL(cityguard);
  SPECIAL(receptionist);
  SPECIAL(snake);
  SPECIAL(guild);
  SPECIAL(fido);
  SPECIAL(bank);
  SPECIAL(janitor);
  SPECIAL(thief);
  SPECIAL(cleric);
  SPECIAL(shop_keeper);
  SPECIAL(auto_equip_newbie);
  SPECIAL(auto_healer);

  if (rnum < 0)
    return 0;


/*  Do NOT reassign shopkeepers.  Soli, 8/10/99   */
  if (mob_index[rnum].func == shop_keeper)
    return 0;

  switch (spec)
  {
    case 0:
      mob_index[rnum].func = 0;
      return 0;
    case 1:
      ASSIGNMOB(mob, postmaster);
      break;
    case 2:
      ASSIGNMOB(mob, guild);
      break;
    case 3:
      ASSIGNMOB(mob, receptionist);
      break;
    case 4:
      ASSIGNMOB(mob, bank);
      break;
    case 5:
      ASSIGNMOB(mob, janitor);
      break;
    case 6:
      ASSIGNMOB(mob, thief);
      break;
    case 7:
      ASSIGNMOB(mob, cityguard);
      break;
    case 8:
      ASSIGNMOB(mob, fido);
      break;
    case 9:
      ASSIGNMOB(mob, snake);
      break;
    case 10:
      ASSIGNMOB(mob, cleric);
      break;
    case 11:
      ASSIGNMOB(mob, auto_equip_newbie);
      break;
    case 12:
        ASSIGNMOB(mob, auto_healer);
        break;
    default:
      mob_index[rnum].func = 0;
      return 0;
  }

  return 1;
}



/* assign special procedures to objects */
void assign_objects(void)
{
  SPECIAL(bank);
  SPECIAL(gen_board);

 /* Palanthas boards, added by Paladine */
  ASSIGNOBJ(1999, gen_board);   /* InChar board */
  ASSIGNOBJ(1998, gen_board);   /* Crime board */
  ASSIGNOBJ(1997, gen_board);   /* Trade board */
  ASSIGNOBJ(1996, gen_board);   /* Policy board */
  ASSIGNOBJ(1995, gen_board);   /* Bug board */
  ASSIGNOBJ(1994, gen_board);   /* News of other worlds board */
  /* Midgaard boards */
  ASSIGNOBJ(3096, gen_board);   /* social board */
  ASSIGNOBJ(3097, gen_board);   /* freeze board */
  ASSIGNOBJ(3098, gen_board);   /* immortal board */
  ASSIGNOBJ(3099, gen_board);   /* mortal board */
  ASSIGNOBJ(1115, gen_board);   /* Reorx */
  ASSIGNOBJ(1250, gen_board);   /* bug board */
  ASSIGNOBJ(1296, gen_board);   /* Imm Enforcement Board */
  ASSIGNOBJ(1297, gen_board);   /* Imm RP/Quest Board */
  ASSIGNOBJ(1298, gen_board);   /* imp board */
  ASSIGNOBJ(1299, gen_board);   /* Imm Wishlist Board */
  ASSIGNOBJ(1251, gen_board);   /* builders board */
  ASSIGNOBJ(3034, bank);        /* atm */
  ASSIGNOBJ(3036, bank);        /* cashcard */
  ASSIGNOBJ(6030, gen_board);   /* mortal board */

  ASSIGNOBJ(4797, gen_board);   /* Conclave Board */

  /* Talenrock boards */
  ASSIGNOBJ(4701, gen_board);
  ASSIGNOBJ(4702, gen_board);
  ASSIGNOBJ(4703, gen_board);
  ASSIGNOBJ(4704, gen_board);
  ASSIGNOBJ(4705, gen_board);
  ASSIGNOBJ(4706, gen_board);

  /* Karthay Boards and Bank Built by Lunitari Added by Chemosh */
  ASSIGNOBJ(5094, gen_board);

  /* Talenrock boards */
  ASSIGNOBJ(4701, gen_board);
  ASSIGNOBJ(4702, gen_board);
  ASSIGNOBJ(4703, gen_board);
  ASSIGNOBJ(4704, gen_board);
  ASSIGNOBJ(4705, gen_board);
  ASSIGNOBJ(4706, gen_board);

  /* Karthay Boards and Bank Built by Lunitari Added by Chemosh */
  ASSIGNOBJ(5094, gen_board);
  ASSIGNOBJ(5095, gen_board);
  ASSIGNOBJ(5096, gen_board);
  ASSIGNOBJ(5097, gen_board);
  ASSIGNOBJ(5098, gen_board);
  ASSIGNOBJ(5099, gen_board);
  ASSIGNOBJ(5119, bank);        /*bank type thing*/

  /*  Morning Dew - Soli, 8/5/99  */
  ASSIGNOBJ(4101, gen_board);
  ASSIGNOBJ(4102, gen_board);
  /* Kalaman Boards, added by Chemosh/Apharot */
  ASSIGNOBJ(10051, gen_board);  /* Policy Board  */
  ASSIGNOBJ(10073, gen_board);  /* Main/IC board */
  ASSIGNOBJ(10074, gen_board);  /* World News Board */
  ASSIGNOBJ(10075, gen_board);  /* Trade Board */
  ASSIGNOBJ(10078, gen_board);  /* Crime Board */
  ASSIGNOBJ(10079, gen_board);  /* OOC Board */

}



/* assign special procedures to rooms */
void assign_room(room_rnum room, int spec)
{
  extern int dts_are_dumps;
  SPECIAL(dump);
  SPECIAL(auto_equip_newbie);

  switch (spec)
  {
    case 1:
      world[room].func = dump;
      break;
    case 2:
      world[room].func = auto_equip_newbie;
      break;  
    case 0:
    default:
      if (dts_are_dumps && IS_SET(ROOM_FLAGS(room), ROOM_DEATH))
        world[room].func = dump;
      else
        world[room].func = 0;
      break;
  }
}

void assign_rooms(void)
{
  int i;


  for (i = 0; i < top_of_world; i++)
    /* if (IS_SET(ROOM_FLAGS(i), ROOM_DEATH)) */
      assign_room(i, ROOM_SPEC(i));
}
