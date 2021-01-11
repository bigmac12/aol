/* ************************************************************************
*   File: config.c                                      Part of CircleMUD *
*  Usage: Configuration of various aspects of CircleMUD operation         *
*                                                                         *
*  All rights reserved.  See license.doc for complete information.        *
*                                                                         *
*  Copyright (C) 1993, 94 by the Trustees of the Johns Hopkins University *
*  CircleMUD is based on DikuMUD, Copyright (C) 1990, 1991.               *
************************************************************************ */

#define __CONFIG_C__

#include "conf.h"
#include "sysdep.h"
#include "structs.h"
#include <unistd.h>
#include <stdio.h>
#include <limits.h>

#define TRUE	1
#define YES	1
#define FALSE	0
#define NO	0


/*
 * Below are several constants which you can change to alter certain aspects
 * of the way CircleMUD acts.  Since this is a .c file, all you have to do
 * to change one of the constants (assuming you keep your object files
around)
 * is change the constant in this file and type 'make'.  Make will recompile
 * this file and relink; you don't have to wait for the whole thing to
 * recompile as you do if you change a header file.
 *
 * I realize that it would be slightly more efficient to have lots of
 * #defines strewn about, so that, for example, the autowiz code isn't
 * compiled at all if you don't want to use autowiz.  However, the actual
 * code for the various options is quite small, as is the computational time
 * in checking the option you've selected at run-time, so I've decided the
 * convenience of having all your options in this one file outweighs the
 * efficency of doing it the other way.
 *
 */


/****************************************************************************/

/****************************************************************************/


/* GAME PLAY OPTIONS */

/*
 * pk_allowed sets the tone of the entire game.  If pk_allowed is set to
 * NO, then players will not be allowed to kill, summon, charm, or sleep
 * other players, as well as a variety of other "asshole player" protections.
 * However, if you decide you want to have an all-out knock-down drag-out
 * PK Mud, just set pk_allowed to YES - and anything goes.
 */
int pk_allowed = YES;

/* is playerthieving allowed? */
int pt_allowed = YES;

int level_can_shout = 1;

/* number of movement points it costs to holler */
int holler_move_cost = 10;

/* exp change limits */
int max_exp_gain = 100000;	/* max gainable per kill */
int max_exp_loss = 1000000;	/* max losable per death */

/* number of tics (usually 75 seconds) before PC/NPC corpses decompose */
int max_npc_corpse_time = 5;
int max_pc_corpse_time = 10;
int max_created_time = 10;

/* How many ticks before a player is sent to the void or idle-rented. */
int idle_void = 8;
int idle_rent_time = 48;

/* This level and up is immune to idling, LVL_IMPL+1 will disable it. */
int idle_max_level = LVL_GOD;

/* should items in death traps automatically be junked? */
int dts_are_dumps = YES;

/* "okay" etc. */
char *OK = "&BOkay&C.&n\r\n";
char *NOPERSON = "&BNo-one by that name here&C.&n\r\n";
char *NOEFFECT = "&BNothing seems to happen&C.&n\r\n";

/*
 * You can define or not define TRACK_THOUGH_DOORS, depending on whether
 * or not you want track to find paths which lead through closed or
 * hidden doors. A setting of 'NO' means to not go through the doors
 * while 'YES' will pass through doors to find the target.
 */
int track_through_doors = YES;

/****************************************************************************/

/* RENT/CRASHSAVE OPTIONS */
/* Should the MUD allow you to 'rent' for free?  (i.e. if you just quit,
 * your objects are saved at no cost, as in Merc-type MUDs.) */
int free_rent = NO;

/* maximum number of items players are allowed to rent */
int max_obj_save = 1000;

/* receptionist's surcharge on top of item costs */
int min_rent_cost = 5;

/*
 * Should the game automatically save people?  (i.e., save player data
 * every 4 kills (on average), and Crash-save as defined below.  This
 * option has an added meaning past bpl13.  If auto_save is YES, then
 * the 'save' command will be disabled to prevent item duplication via
 * game crashes.
 */

int auto_save = YES;

/*
 * if auto_save (above) is yes, how often (in minutes) should the MUD
 * Crash-save people's objects?   Also, this number indicates how often
 * the MUD will Crash-save players' houses.
 */
int autosave_time = 5;

/* Lifetime of crashfiles and forced-rent (idlesave) files in days */
int crash_file_timeout = 10;

/* Lifetime of normal rent files in days */
int rent_file_timeout = 30;


/****************************************************************************/
/****************************************************************************/


/* ROOM NUMBERS */

/* virtual number of room that mortals should enter at */
sh_int mortal_start_room = 1998;

/* virtual number of room that immorts should enter at by default */
sh_int immort_start_room = 1204;

/* virtual number of room that frozen players should enter at */
sh_int frozen_start_room = 1202;

/*
 * virtual numbers of donation rooms.  note: you must change code in
 * do_drop of act.item.c if you change the number of non-NOWHERE
 * donation rooms.
 */
sh_int donation_room_1 = 4718;
sh_int donation_room_2 = NOWHERE;	/* unused - room for expansion */
sh_int donation_room_3 = NOWHERE;	/* unused - room for expansion */



/****************************************************************************/
/****************************************************************************/


/* GAME OPERATION OPTIONS */

/*
 * This is the default port the game should run on if no port is given on
 * the command-line.  NOTE WELL: If you're using the 'autorun' script, the
 * port number there will override this setting.  Change the PORT= line in
 * instead of (or in addition to) changing this.
 */

/*
 * IP address to which the MUD should bind.  This is only useful if
 * you're running Circle on a host that host more than one IP interface,
 * and you only want to bind to *one* of them instead of all of them.
 * Setting this to NULL (the default) causes Circle to bind to all
 * interfaces on the host.  Otherwise, specify a numeric IP address in
 * dotted quad format, and Circle will only bind to that IP address.  (Of
 * course, that IP address must be one of your host's interfaces, or it
 * won't work.)
 */

ush_int DFLT_PORT = 6060;

const char *DFLT_IP = NULL; /* bind to all interfaces */
/* const char *DFLT_IP = "192.168.1.1"; -- bind only to one interface */

/* default directory to use as data directory */
char *DFLT_DIR = "lib";

/*
 * What file to log messages to (ex: "log/syslog").  Setting this to NULL
 * means you want to log to stderr, which was the default in earlier
 * versions of Circle.  If you specify a file, you don't get messages to
 * the screen. (Hint: Try 'tail -f' if you have a UNIX machine.)
 */
const char *LOGNAME = NULL;
/* const char *LOGNAME = "log/syslog";  -- useful for Windows users */

/* maximum number of players allowed before game starts to turn people away */
int max_playing = 300;

/* maximum size of bug, typo and idea files in bytes (to prevent bombing) */
int max_filesize = 50000;

/* maximum number of password attempts before disconnection */
int max_bad_pws = 3;

/*
 * Rationale for enabling this, as explained by naved@bird.taponline.com.
 *
 * Usually, when you select ban a site, it is because one or two people are
 * causing troubles while there are still many people from that site who you
 * want to still log on.  Right now if I want to add a new select ban, I need
 * to first add the ban, then SITEOK all the players from that site except for
 * the one or two who I don't want logging on.  Wouldn't it be more convenient
 * to just have to remove the SITEOK flags from those people I want to ban
 * rather than what is currently done?
 */
int siteok_everyone = TRUE;

/*
 * Some nameservers are very slow and cause the game to lag terribly every 
 * time someone logs in.  The lag is caused by the gethostbyaddr() function
 * which is responsible for resolving numeric IP addresses to alphabetic
names.
 * Sometimes, nameservers can be so slow that the incredible lag caused by
 * gethostbyaddr() isn't worth the luxury of having names instead of numbers
 * for players' sitenames.
 *
 * If your nameserver is fast, set the variable below to NO.  If your
 * nameserver is slow, of it you would simply prefer to have numbers
 * instead of names for some other reason, set the variable to YES.
 *
 * You can experiment with the setting of nameserver_is_slow on-line using
 * the SLOWNS command from within the MUD.
 */

int nameserver_is_slow = NO;

char *ANSI = "Do you support the ANSI color standard (Yn)? ";

char *MENU =
"\r\n"
"                     &c- -=( &CWelcome to Age of Legends &c)=- -\r\n"
"                        &g-  -{ &GTales of the Lance &g}-  -\r\n"
"\r\n"
"                       &bo -    -  - -)-=-(- -   -    - o\r\n"
"                         &w(&W1&w) &WEnter the world of Krynn\r\n"
"                                     -=-\r\n"
"                          &r(&R2&r) Enter char description\r\n"
"                             &r(&R3&r) Read background\r\n"
"                             &r(&R4&r) Change password\r\n"
"                          &r(&R5&r) Delete this character\r\n"
"                                     -=-\r\n"
"                         &b(&B0&b) &BExit from Age of Legends\r\n"
"                       &bo -    -  - -)-=-(- -   -    - o\r\n"
"                                &WYour choice: &n";

// TODO put this in a function. Can be useful elsewhere in the code.
char current_dir[PATH_MAX];
getcwd(current_dir, sizeof(current_dir));
char *dev_path = "dev";

char *in_dev_mode = strstr(current_dir, dev_path);

if (in_dev_mode) {
   char *GREETINGS = 
      "\r\n\r\n"
      " +--------------------------------------------------------------------+\r\n"
      " |                                                                    |\r\n"
      " |                                                                    |\r\n"
      " |                                                                    |\r\n"
      " |                                                                    |\r\n"
      " |  ██████╗ ███████╗██╗   ██╗    ███╗   ███╗ ██████╗ ██████╗ ███████╗ |\r\n"
      " |  ██╔══██╗██╔════╝██║   ██║    ████╗ ████║██╔═══██╗██╔══██╗██╔════╝ |\r\n"
      " |  ██║  ██║█████╗  ██║   ██║    ██╔████╔██║██║   ██║██║  ██║█████╗   |\r\n"
      " |  ██║  ██║██╔══╝  ╚██╗ ██╔╝    ██║╚██╔╝██║██║   ██║██║  ██║██╔══╝   |\r\n"
      " |  ██████╔╝███████╗ ╚████╔╝     ██║ ╚═╝ ██║╚██████╔╝██████╔╝███████╗ |\r\n"
      " |  ╚═════╝ ╚══════╝  ╚═══╝      ╚═╝     ╚═╝ ╚═════╝ ╚═════╝ ╚══════╝ |\r\n"
      " |                                                                    |\r\n"
      " |                                                                    |\r\n"
      " |                                                                    |\r\n"
      " |                                                                    |\r\n"
      " +--------------------------------------------------------------------+\r\n"
      "\r\n"
      " By what unique name do you wish to be known? ";
} else {
   char *GREETINGS =
   "\r\n\r\n"
   " +--------------------------------------------------------------------+\r\n"
   " |                   __                                               |\r\n"
   " |                .-'  `.    _.-----------'-----------..__            |\r\n"
   " |              .'       | .'             |`.            /            |\r\n"
   " |            .'     o   |.      .--.    .' |    _____  /             |\r\n"
   " |          .'           ||     '   __\\ /__ |       / `/              |\r\n"
   " |         /.-.    .     ||    |   /_     / |    __/  .\\              |\r\n"
   " |        ,'   `. / \\    |`.   `.___.'   |  |    \\__.-' \\             |\r\n"
   " |               `  | _  |  .            |  |          __\\            |\r\n"
   " |                  '` `.|   `-._____    | .'____..-- '  `            |\r\n"
   " |      _____            `          /    |                            |\r\n"
   " |     `.   .'                   of/_____|                            |\r\n"
   " |     /   /      .--._.   .-.    .--._.  _.-.    ____.----.   .----. |\r\n"
   " |    /   /      /  .__/ .'   `. /  .__/  .   |  /  .'/     `./ .-. / |\r\n" 
   " |   /   /   _.//   _\\  /  .^-,//   _\\    /   | /  / /  /|   /\\ `. `  |\r\n"
   " |  /    `--' //   (_.-.  / ___/   (_.-. /    |/  / /  //   //\\  `.   |\r\n"
   " | /         / |      /|  \\_> /|      / /  /|    / /  /'   //  `   /  |\r\n"
   " |'  _..--../  `.___./ `.___.' `.___./ /  / `.__/.'______.' `.___.'   |\r\n"
   " ||.'                                 | /                             |\r\n"
   " |'                                   '/                              |\r\n"
   " +----------o-     T a l e s   o f   t'h e   L a n c e    -o----------+\r\n"
   " Based on CircleMUD 3.0, Created by Jeremy Elson                       \r\n"
   " A derivative of DikuMUD (GAMMA 0.0)                                   \r\n"
   " Created by, Hans Henrik Staerfeldt, Katja Nyboe, Tom Madsen,          \r\n"
   " Michael Seifert, and Sebastian Hammer                                 \r\n"
   "\r\n\r\n"
   "Before creating please goto: \r\n"
   "http://ageoflegends.com/old/charactercreationtips.html for rules on "
   "creating a character.\r\n\r\n"
   "To email the owner: chemosh@ageoflegends.com\r\n\r\n"
   "By what unique name do you wish to be known? ";
}

char *WELC_MESSG =
"\r\n"
"Welcome to Krynn.  May you find laughter, sorrow, pleasure and pain, and all\r\n"
"fine points in between as you enter the world of DragonLance."
"\r\n\r\n";

char *START_MESSG =
"\r\nWelcome!  Please wait while your character is being reviewed -- you will\r\n"
"not be able to perform basic actions until then.  If after a few minutes, a\r\n"
"god does not appear or you are not approved, please come back later.\r\n\r\n";


/****************************************************************************/

/****************************************************************************/


/* AUTOWIZ OPTIONS */

/*
 * Should the game automatically create a new wizlist/immlist every time
 * someone immorts, or is promoted to a higher (or lower) god level?
 * NOTE: this only works under UNIX systems.
 */
int use_autowiz = NO;

/* If yes, what is the lowest level which should be on the wizlist?  (All
   immort levels below the level you specify will go on the immlist instead.)
*/
int min_wizlist_lev = LVL_LESSERGOD;

/*
 * Set maximum number of attacks mobs/npcs can have per combat round.
 * The number of attacks are based on the formula moblvl/mob_attacks.....
 *
 */
const int max_mob_attacks_per_round = 6;   /* Maximum number of hits*/
                                           /* a NPC is allowed */
const int mob_attacks_every_x_levels = 10;  /* GET_LEVEL(mob) / x_levels */

