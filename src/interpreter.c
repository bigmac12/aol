/* ************************************************************************
*   File: interpreter.c                                 Part of CircleMUD *
*  Usage: parse user commands, search for specials, call ACMD functions   *
*                                                                         *
*  All rights reserved.  See license.doc for complete information.        *
*                                                                         *
*  Copyright (C) 1993, 94 by the Trustees of the Johns Hopkins University *
*  CircleMUD is based on DikuMUD, Copyright (C) 1990, 1991.               *
************************************************************************ */

//#define __INTERPRETER_C__

#include "conf.h"
#include "sysdep.h"

#include "structs.h"
#include "comm.h"
#include "interpreter.h"
#include "db.h"
#include "utils.h"
#include "spells.h"
#include "handler.h"
#include "mail.h"
#include "screen.h"
#include "olc.h"
#include "dg_scripts.h"
#include "char_descs.h"

/* For the building and devel ports, undef this. */
/* --gan, 08/07/02 (Nuitari) */
#define __MAIN_PORT__

extern int siteok_everyone;
extern const struct title_type titles[NUM_CLASSES][LVL_IMPL + 1];
extern char *motd;
extern char *imotd;
extern char *background;
extern char *MENU;
extern char *WELC_MESSG;
extern char *START_MESSG;
extern struct char_data *character_list;
extern struct descriptor_data *descriptor_list;
extern struct player_index_element *player_table;
extern int top_of_p_table;
extern int circle_restrict;
extern struct index_data *mob_index;
extern struct index_data *obj_index;
extern struct room_data *world;
extern const char *pc_race_types[];
extern const char *pc_class_types[];
extern int class_abil_mins[NUM_CLASSES][6];
extern room_rnum top_of_world;
extern int xap_objs;


const struct command_info cmd_info[];

// Char_descs.c

char *current_short_desc(struct char_data *ch);
void short_desc_descriptors_menu(struct char_data *ch);
void short_desc_adjectives_menu(struct char_data *ch, int which_type);
int count_adjective_types(int which_desc);


/* external functions */
void echo_on(struct descriptor_data *d);
void echo_off(struct descriptor_data *d);
void do_start(struct char_data *ch);
void init_char(struct char_data *ch);
int create_entry(char *name);
void dismount_char(struct char_data *ch);
int special(struct char_data *ch, int cmd, char *arg);
int isbanned(char *hostname);
int Valid_Name(char *newname);
int class_ok_race[NUM_RACES][NUM_CLASSES];
extern char *class_display[];
void oedit_parse(struct descriptor_data *d, char *arg);
void redit_parse(struct descriptor_data *d, char *arg);
void zedit_parse(struct descriptor_data *d, char *arg);
void medit_parse(struct descriptor_data *d, char *arg);
void sedit_parse(struct descriptor_data *d, char *arg);
void trigedit_parse(struct descriptor_data *d, char *arg);
void race_affects(struct char_data *ch);
void read_aliases(struct char_data *ch);
void immcharlog(const char *str);
void affect_from_char_II(struct char_data * ch, int skill, int type, int action);
void die_follower(struct char_data *ch);
int enter_player_game (struct descriptor_data *d);

/* prototypes for all do_x functions. */
ACMD(do_nw);
ACMD(do_ne);
ACMD(do_sw);
ACMD(do_se);
ACMD(do_action);
ACMD(do_advance);
ACMD(do_aecho);
ACMD(do_zecho);
ACMD(do_affected);
ACMD(do_alias);
ACMD(do_approve);
ACMD(do_assist);
ACMD(do_parse_assess);
ACMD(do_at);
ACMD(do_autocon);
ACMD(do_backstab);
ACMD(do_backpat);
ACMD(do_backupgear);
// ACMD(do_badmort);
ACMD(do_ban);
ACMD(do_bandage);
ACMD(do_bash);
ACMD(do_blink);
ACMD(do_boggle);
ACMD(do_break);
ACMD(do_brew);
ACMD(do_buck);
ACMD(do_build_fire);
//ACMD(do_bury);
ACMD(do_cast);
ACMD(do_castout);
ACMD(do_chakra);
ACMD(do_challenge);
ACMD(do_charge);
ACMD(do_chat);
//ACMD(do_clan);
ACMD(do_color);
ACMD(do_cook);
ACMD(do_copyover);
ACMD(do_commands);
ACMD(do_consider);
ACMD(do_control_undead);
ACMD(do_copyto);
ACMD(do_credits);
ACMD(do_date);
ACMD(do_dc);
ACMD(do_detect_poison);
ACMD(do_detectdisguise);
ACMD(do_diagnose);
ACMD(do_dig);
ACMD(do_dismiss);
ACMD(do_rouse);
ACMD(do_douse);
ACMD(do_gdig);
ACMD(do_gore);
ACMD(do_gore);
ACMD(do_dirtkick);
ACMD(do_disarm);
ACMD(do_display);
//ACMD(do_disguise);
ACMD(do_dismount);
ACMD(do_drink);
ACMD(do_drop);
ACMD(do_eat);
ACMD(do_echo);
ACMD(do_enchant);
ACMD(do_envenom);
ACMD(do_enter);
ACMD(do_equipment);
ACMD(do_examine);
ACMD(do_exchange);
ACMD(do_exit);
ACMD(do_exits);
ACMD(do_flee);
ACMD(do_follow);
ACMD(do_force);
ACMD(do_forget);
ACMD(do_gecho);
ACMD(do_gshow);
ACMD(do_gen_comm);
ACMD(do_gen_door);
ACMD(do_gen_ps);
ACMD(do_gen_tog);
ACMD(do_gen_write);
ACMD(do_get);
ACMD(do_give);
ACMD(do_gold);
ACMD(do_goto);
ACMD(do_grab);
ACMD(do_group);
ACMD(do_grumble);
ACMD(do_gsay);
ACMD(do_guard);
ACMD(do_headrub);
ACMD(do_hero);
ACMD(do_hang);
ACMD(do_hcontrol);
ACMD(do_help);
ACMD(do_hide);
ACMD(do_hit);
ACMD(do_house);
ACMD(do_iamgod);
ACMD(do_impnet);
ACMD(do_info);
ACMD(do_insult);
ACMD(do_inventory);
ACMD(do_invis);
ACMD(do_jawstrike);
ACMD(do_kick);
ACMD(do_knockout);
ACMD(do_kill);
//ACMD(do_lang_say);
ACMD(do_lay_hands);
ACMD(do_last);
ACMD(do_leave);
ACMD(do_levels);
ACMD(do_load);
ACMD(do_look);
ACMD(do_makeleader);
ACMD(do_mark);
ACMD(do_memorize);
ACMD(do_mlist);
ACMD(do_move);
ACMD(do_mount);
ACMD(do_mutter);
ACMD(do_mget);
ACMD(do_mgive);
ACMD(do_mreport);
ACMD(do_mrest);
ACMD(do_mstand);
//ACMD(do_mysql_convert);
ACMD(do_newbie);
ACMD(do_not_here);
ACMD(do_offer);
ACMD(do_olc);
ACMD(do_olist);
ACMD(do_order);
ACMD(do_objconv);
ACMD(do_page);
ACMD(do_pedit);
ACMD(do_poofset);
ACMD(do_players);
ACMD(do_plip);
ACMD(do_pounce);
ACMD(do_pour);
ACMD(do_practice);
ACMD(do_prayer);
ACMD(do_purge);
ACMD(do_put);
ACMD(do_qcomm);
ACMD(do_quest);
ACMD(do_quit);
ACMD(do_reboot);
ACMD(do_reveal);
ACMD(do_release);
ACMD(do_reel);
ACMD(do_reimburse);
ACMD(do_award);
ACMD(do_relax);
ACMD(do_remove);
ACMD(do_remove_poison);
ACMD(do_rent);
ACMD(do_reply);
ACMD(do_report);
ACMD(do_rescue);
ACMD(do_rest);
ACMD(do_restore);
ACMD(do_retreat);
ACMD(do_return);
ACMD(do_rlist);
ACMD(do_save);
ACMD(do_scribe);
ACMD(do_skin);
ACMD(do_say);
ACMD(do_sayto);
ACMD(do_scan);
ACMD(do_score);
ACMD(do_send);
ACMD(do_set);
ACMD(do_show);
ACMD(do_sheath);
ACMD(do_shred);
ACMD(do_shutdown);
ACMD(do_sit);
ACMD(do_skills);
ACMD(do_skillset);
ACMD(do_sleep);
ACMD(do_sneak);
ACMD(do_snoop);
ACMD(do_spec_comm);
ACMD(do_spells);
ACMD(do_split);
ACMD(do_stand);
ACMD(do_stat);
ACMD(do_steal);
ACMD(do_subdue);
ACMD(do_switch);
ACMD(do_syslog);
ACMD(do_stance);
ACMD(do_tame);
ACMD(do_teleport);
ACMD(do_tell);
ACMD(do_time);
ACMD(do_title);
ACMD(do_toggle);
ACMD(do_throw);
ACMD(do_track);
ACMD(do_trans);
ACMD(do_trip);
ACMD(do_turn_undead);
ACMD(do_unban);
ACMD(do_unearth);
ACMD(do_ungroup);
ACMD(do_use);
ACMD(do_users);
ACMD(do_unsheath);
ACMD(do_visible);
ACMD(do_vnum);
ACMD(do_vstat);
ACMD(do_vwear);
ACMD(do_wake);
ACMD(do_wear);
ACMD(do_weather);
ACMD(do_where);
ACMD(do_who);
ACMD(do_wield);
ACMD(do_wimpy);
ACMD(do_wizlock);
ACMD(do_wiznet);
ACMD(do_wizutil);
ACMD(do_write);
ACMD(do_zreset);
ACMD(do_zstat);

/* DG Script ACMD's */
ACMD(do_attach);
ACMD(do_detach);
ACMD(do_tlist);
ACMD(do_tstat);
ACMD(do_masound);
ACMD(do_mkill);
ACMD(do_mjunk);
ACMD(do_mechoaround);
ACMD(do_msend);
ACMD(do_mecho);
ACMD(do_mload);
ACMD(do_mpurge);
ACMD(do_mgoto);
ACMD(do_mat);
ACMD(do_mteleport);
ACMD(do_mforce);
ACMD(do_mexp);
ACMD(do_mgold);

struct command_info *complete_cmd_info;

/* This is the Master Command List(tm).

 * You can put new commands in, take commands out, change the order
 * they appear in, etc.  You can adjust the "priority" of commands
 * simply by changing the order they appear in the command list.
 * (For example, if you want "as" to mean "assist" instead of "ask",
 * just put "assist" above "ask" in the Master Command List(tm).
 *
 * In general, utility commands such as "at" should have high priority;
 * infrequently used and dangerously destructive commands should have low
 * priority.
 */

const struct command_info cmd_info[] = {
  { "RESERVED", "", 0, 0, 0, 0 },	/* this must be first -- for specprocs */

  /* directions must come before other commands but after RESERVED */
  { "north"    , "n",       POS_STANDING, do_move     , 1, SCMD_NORTH },
  { "east"     , "e",       POS_STANDING, do_move     , 1, SCMD_EAST },
  { "south"    , "s",       POS_STANDING, do_move     , 1, SCMD_SOUTH },
  { "west"     , "w",       POS_STANDING, do_move     , 1, SCMD_WEST },
  { "up"       , "u",       POS_STANDING, do_move     , 1, SCMD_UP },
  { "down"     , "d",       POS_STANDING, do_move     , 1, SCMD_DOWN },
  { "northwest", "northw",  POS_STANDING, do_move     , 1, SCMD_NORTHWEST },
  { "northeast", "northe",  POS_STANDING, do_move     , 1, SCMD_NORTHEAST },
  { "southwest", "southw",  POS_STANDING, do_move     , 1, SCMD_SOUTHWEST },
  { "southeast", "southe",  POS_STANDING, do_move     , 1, SCMD_SOUTHEAST },
  { "nw"       , "nw",      POS_STANDING, do_nw       , 1, SCMD_NORTHWEST },
  { "ne"       , "ne",      POS_STANDING, do_ne       , 1, SCMD_NORTHEAST },
  { "sw"       , "sw",      POS_STANDING, do_sw       , 1, SCMD_SOUTHWEST },
  { "se"       , "se",      POS_STANDING, do_se       , 1, SCMD_SOUTHEAST }, 
  
  /* now, the main list */
#ifdef __MAIN_PORT__
  { "at"       , "at",      POS_DEAD    , do_at       , LVL_DEMIGOD, 0 },
#else
  { "at"       , "at",      POS_DEAD    , do_at       , LVL_IMMORT, 0 },
#endif
  { "advance"  , "adv",     POS_DEAD    , do_advance  , LVL_GRGOD, 0 },
  { "aecho"    , "aec",     POS_DEAD    , do_aecho    , LVL_GOD, 0 },
  { "zecho"    , "zec",     POS_DEAD    , do_zecho    , LVL_GOD, 0 },
  { "alias"    , "ali",     POS_DEAD    , do_alias    , 1, 0 },
  { "accuse"   , "acc",     POS_SITTING , do_action   , 1, 0 },
  { "afk"      , "afk",     POS_DEAD    , do_gen_tog  , 1, SCMD_AFK },
  { "affected" , "aff",     POS_DEAD    , do_affected , 1, 0 },
  { "agree"    , "agr",     POS_RESTING , do_action   , 1, 0 },
  { "applaud"  , "appl",    POS_RESTING , do_action   , 1, 0 },
  { "approve"  , "appr",    POS_RESTING , do_approve  , LVL_APPROVE, SCMD_APPROVE },
  { "assist"   , "as",      POS_FIGHTING, do_assist   , 1, 0 },
  { "assess"   , "ass",     POS_STANDING, do_parse_assess, 1, 0 },
  { "ask"      , "ask",     POS_RESTING , do_spec_comm, 1, SCMD_ASK },
  { "autoexit" , "autoe",   POS_DEAD    , do_gen_tog  , 1, SCMD_AUTOEXIT },
  { "autocon"  , "autoc",   POS_DEAD    , do_autocon  , 1, 0 },
#ifdef __MAIN_PORT__
  { "objlevel" , "obj",     POS_DEAD    , do_gen_tog  , LVL_DEMIGOD, SCMD_LEVEL_FLAGS },
#else
  { "objlevel" , "obj",     POS_DEAD    , do_gen_tog  , LVL_IMMORT, SCMD_LEVEL_FLAGS },
#endif
  { "buy"      , "bu",      POS_RESTING, do_not_here , 1, 0 },
  { "badmortnonewbie", "bad", POS_DEAD, do_wizutil, LVL_DEPTHEAD, SCMD_BADMORTNONEWBIE },
  { "bounce"   , "bou",     POS_STANDING, do_action   , 1, 0 },
  { "buck"     , "buck",    POS_STANDING, do_buck	   , 0, 0 },
  //{ "bury"     , POS_STANDING, do_bury     , 1, 0 },
  { "build"    , "bui",     POS_STANDING, do_build_fire, 1, 0 },
  { "backstab" , "back",    POS_STANDING, do_backstab , 1, 0 },
  { "backpat"  , "back",    POS_STANDING, do_action   , 1, 0 },
  { "backupgear", "backup", POS_DEAD   , do_backupgear, 1, 0 },
  { "ban"      , "ban",     POS_DEAD    , do_ban      , LVL_ADMIN, 0 },
  { "bandage"  , "band",    POS_RESTING , do_bandage  , 1, 0 },
  { "balance"  , "bal",     POS_STANDING, do_not_here , 1, 0 },
  { "bash"     , "bash",    POS_FIGHTING, do_bash     , 1, 0 },
  { "beg"      , "beg",     POS_RESTING, do_action   , 1, 0 },
  { "bite"     , "bite",    POS_RESTING , do_action   , 1, 0 },
  { "bleed"    , "ble",     POS_RESTING , do_action   , 1, 0 },
  { "blink"    , "bli",     POS_RESTING , do_action   , 1, 0 },
  { "blush"    , "blu",     POS_RESTING , do_action   , 1, 0 },
  { "bow"      , "bow",     POS_STANDING, do_action   , 0, 0 },
  { "boggle"   , "bog",     POS_RESTING , do_action   , 1, 0 },
  { "brb"      , "brb",     POS_RESTING , do_action   , 1, 0 },
  { "break"    , "bre",     POS_DEAD    , do_break    , 1, 0 },
  { "brief"    , "bri",     POS_DEAD    , do_gen_tog  , 1, SCMD_BRIEF },
  { "brew"     , "brew",    POS_SITTING , do_brew     , 1, 0 },
  { "burp"     , "burp",    POS_RESTING , do_action   , 1, 0 },
  { "bug"      , "bug",     POS_DEAD    , do_gen_write, 1, SCMD_BUG },
  { "cast"     , "c",       POS_SITTING , do_cast     , 1, 0 },
  { "castout"  , "castout", POS_RESTING , do_castout  , 1, 0 },
  { "cackle"   , "cackle",  POS_RESTING , do_action   , 1, 0 },
  { "chakra"   , "cha",     POS_RESTING, do_chakra   , 1, 0 },
  { "challenge", "chal",    POS_FIGHTING, do_challenge, 1, 0 },
  { "charge"   , "char",    POS_FIGHTING, do_charge   , 1, 0 },
  { "chat"     , "chat",    POS_DEAD    , do_chat     , 0, 0 },
  { "check"    , "che",     POS_STANDING, do_not_here , 1, 0 },
  { "cheer"    , "cheer",   POS_RESTING , do_action   , 1, 0 },
  { "chuckle"  , "chuck",   POS_RESTING , do_action   , 1, 0 },
  //{ "clan"     , "",    POS_RESTING , do_clan     , 1, 0 },
  { "clap"     , "cl",      POS_RESTING , do_action   , 1, 0 },
  { "clear"    , "cle",     POS_DEAD    , do_gen_ps   , 0, SCMD_CLEAR },
  { "close"    , "cl",      POS_SITTING , do_gen_door , 1, SCMD_CLOSE },
  { "cls"      , "cls",     POS_DEAD    , do_gen_ps   , 0, SCMD_CLEAR },
  { "coin"     , "coi",     POS_RESTING , do_gold     , 1, 0 },
  { "consider" , "con",     POS_RESTING , do_consider , 1, 0 },
  { "control"  , "cont",    POS_STANDING, do_control_undead, 1, 0 },
  { "color"    , "col",     POS_DEAD    , do_color    , 0, 0 },
  { "cook"     , "cook",    POS_RESTING, do_cook     , 0, 0 },
  { "comfort"  , "comf",    POS_RESTING, do_action   , 1, 0 },
  { "comb"     , "comb",    POS_RESTING , do_action   , 1, 0 },
  { "commands" , "com",     POS_DEAD    , do_commands , 0, SCMD_COMMANDS },
  { "compact"  , "comp",    POS_DEAD    , do_gen_tog  , 1, SCMD_COMPACT },
  { "copyto"   , "copyt",   POS_DEAD    , do_copyto   , LVL_BUILDER, 0 },
  { "copyover" , "copyover",POS_DEAD    , do_copyover , LVL_IMPL, 0 },	
  { "cough"    , "cou",     POS_RESTING , do_action   , 1, 0 },
  { "credits"  , "cred",    POS_DEAD    , do_gen_ps   , 0, SCMD_CREDITS },
  { "cringe"   , "cri",     POS_RESTING , do_action   , 1, 0 },
  { "cry"      , "cry",     POS_RESTING , do_action   , 1, 0 },
  { "cuddle"   , "cud",     POS_RESTING , do_action   , 1, 0 },
  { "curse"    , "cur",     POS_RESTING , do_action   , 1, 0 },
  { "curtsey"  , "curt",    POS_STANDING, do_action   , 1, 0 },
  { "dance"    , "dan",     POS_STANDING, do_action   , 1, 0 },
  { "date"     , "da",      POS_DEAD    , do_date     , LVL_IMMORT, SCMD_DATE },
  { "dirtkick" , "dirt",    POS_FIGHTING, do_dirtkick , 1, 0 },
  { "dismiss"  , "dism",    POS_FIGHTING, do_dismiss  , 1, 0 },
  { "rouse"    , "rou",     POS_STANDING, do_rouse    , 1, 0 },   
  { "daydream" , "day",     POS_SLEEPING, do_action   , 1, 0 },
  { "dc"       , "dc",      POS_DEAD    , do_dc       , LVL_GRGOD, 0 },
  { "deposit"  , "dep",     POS_STANDING, do_not_here , 1, 0 },
  { "detect"   , "det",     POS_STANDING, do_detect_poison, 1, 0 },
  //{ "detectdis", "",    POS_STANDING, do_detectdisguise, 1, 0 },
  { "diagnose" , "diag",    POS_RESTING, do_diagnose , 1, 0 },
  { "display"  , "disp",    POS_DEAD    , do_display  , 1, 0 },
  { "dig    "  , "dig",     POS_STANDING, do_dig      , 1, 0 },
  //{ "disguise" , "",    POS_STANDING, do_disguise , 1, 0 },
  { "douse"    , "dou",     POS_STANDING, do_douse , 1, 0 },
  { "gdig"     , "gdig",    POS_DEAD    , do_gdig      , LVL_BUILDER, 0 },
  { "gore"     , "g",       POS_FIGHTING, do_gore      , 1, 0 },
  { "disarm"   , "dis",     POS_FIGHTING, do_disarm   , 1, 0 },
  { "dismount" , "dismo",   POS_RESTING, do_dismount , 0, 0 },
  { "disagree" , "disa",    POS_RESTING , do_action   , 1, 0 },
  { "drink"    , "dr",      POS_RESTING , do_drink    , 1, SCMD_DRINK },
  { "drop"     , "drop",    POS_RESTING, do_drop     , 1, SCMD_DROP },
  { "drool"    , "droo",    POS_RESTING , do_action   , 1, 0 },
  { "eat"      , "eat",     POS_RESTING , do_eat      , 1, SCMD_EAT },
  { "echo"     , "ec",      POS_SLEEPING, do_echo     , LVL_IMMORT, SCMD_ECHO },
  { "emote"    , "em",      POS_RESTING , do_echo     , 1, SCMD_EMOTE },
  { ":"        , ":",       POS_RESTING, do_echo      , 1, SCMD_EMOTE },
  { "embrace"  , "emb",     POS_STANDING, do_action   , 1, 0 },
  { "enter"    , "en",      POS_STANDING, do_enter    , 0, 0 },
  { "enchant"  , "ench",    POS_RESTING , do_enchant  , 0, 0 },
  { "envenom"  , "env",     POS_STANDING, do_envenom  , 1, 0 },
  { "equipment", "eq",      POS_SLEEPING, do_equipment, 0, 0 },
  { "exits"    , "ex",      POS_RESTING, do_exits    , 0, 0 },
  { "examine"  , "exa",     POS_RESTING , do_examine  , 0, 0 },
  { "exchange" , "exch",    POS_RESTING , do_exchange , 1, 0 },
  { "force"    , "fo",      POS_SLEEPING, do_force    , LVL_GOD, 0 },
  { "fart"     , "fart",    POS_RESTING , do_action   , 1, 0 },
  { "fill"     , "fi",      POS_STANDING, do_pour     , 1, SCMD_FILL },
  { "flee"     , "fl",      POS_FIGHTING, do_flee     , 1, 0 },
  { "flip"     , "flip",    POS_STANDING, do_action   , 1, 0 },
  { "flirt"    , "flirt",   POS_RESTING , do_action   , 1, 0 },
  { "follow"   , "fol",     POS_RESTING , do_follow   , 0, 0 },
  { "fondle"   , "fon",     POS_RESTING , do_action   , 1, 0 },
  { "forget"   , "forg",    POS_RESTING , do_forget   , 1, 0 },
  { "freeze"   , "freeze",  POS_DEAD    , do_wizutil  , LVL_FREEZE, SCMD_FREEZE },
  { "french"   , "french",  POS_RESTING , do_action   , 1, 0 },
  { "frown"    , "frown",   POS_RESTING , do_action   , 1, 0 },
  { "fume"     , "fume",    POS_RESTING , do_action   , 1, 0 },
  { "get"      , "get",     POS_RESTING , do_get      , 0, 0 },
  { "gasp"     , "gasp",    POS_RESTING , do_action   , 1, 0 },
  { "gecho"    , "ge",      POS_DEAD    , do_gecho    , LVL_GOD, 0 },
  { "give"     , "gi",      POS_RESTING , do_give     , 0, 0 },
  { "giggle"   , "gigg",    POS_RESTING , do_action   , 1, 0 },
  { "glance"   , "glance",  POS_RESTING , do_diagnose , 1, 0 },
  { "glare"    , "glare",   POS_RESTING , do_action   , 1, 0 },
#ifdef __MAIN_PORT__
  { "goto"     , "goto",    POS_SLEEPING, do_goto     , LVL_DEMIGOD, 0 },
#else
  { "goto"     , "goto",    POS_SLEEPING, do_goto     , LVL_IMMORT, 0 },
#endif
  { "group"    , "group",   POS_RESTING , do_group    , 1, 0 },
  { "grab"     , "grab",    POS_RESTING , do_grab     , 1, 0 },
  { "greet"    , "greet",   POS_RESTING , do_action   , 1, SCMD_GREET },
  { "grin"     , "grin",    POS_RESTING , do_action   , 1, 0 },
  { "groan"    , "groan",   POS_RESTING , do_action   , 1, 0 },
  { "grope"    , "grope",   POS_RESTING , do_action   , 1, 0 },
  { "grovel"   , "grovel",  POS_RESTING , do_action   , 1, 0 },
  { "growl"    , "growl",   POS_RESTING, do_action   , 1, 0 },
  { "grumble"  , "grumble", POS_RESTING , do_action   , 1, 0 },
  { "gsay"     , "gsay",    POS_SLEEPING, do_gsay     , 1, 0 },
  { "gtell"    , "gtell",   POS_SLEEPING, do_gsay     , 1, 0 },
  { "guard"    , "guard",   POS_STANDING, do_guard    , 1, 0 },
  { "help"     , "help",    POS_DEAD    , do_help     , 0, 0 },
  { "headrub"  , "headrub", POS_STANDING, do_action   , 1, 0 },
  { "hang"     , "hang",    POS_RESTING,  do_hang     , 1, 0 },
  { "handbook" , "hand",    POS_DEAD    , do_gen_ps   , LVL_IMMORT, SCMD_HANDBOOK },
  { "hcontrol" , "hcon",    POS_DEAD    , do_hcontrol , LVL_GRGOD, 0 },
//  { "hero"     , "",  POS_DEAD    , do_hero     , 30, 0 },
  { "hide"     , "hide",    POS_RESTING , do_hide     , 1, 0 },
  { "hiccup"   , "hicc",    POS_RESTING , do_action   , 1, 0 },
  { "hit"      , "hit",     POS_FIGHTING, do_hit      , 1, SCMD_HIT },
  { "hold"     , "hold",    POS_RESTING , do_grab     , 1, 0 },
  { "holler"   , "holler",  POS_RESTING , do_gen_comm , 1, SCMD_SHOUT },
  { "holylight", "holy",    POS_DEAD    , do_gen_tog  , LVL_IMMORT, SCMD_HOLYLIGHT },
  { "hop"      , "hop",     POS_RESTING , do_action   , 1, 0 },
  { "house"    , "house",   POS_RESTING , do_house    , 1, 0 },
  { "hug"      , "hug",     POS_RESTING , do_action   , 1, 0 },
  { "inventory", "i",       POS_DEAD    , do_inventory, 0, 0 },
  //{ "iamgodhearmeroar", "iamgod", POS_DEAD, do_iamgod, 0, 0 },
  { "idea"     , "idea",    POS_DEAD    , do_gen_write, 1, SCMD_IDEA },
#ifdef __MAIN_PORT__
  { "imotd"    , "imotd",   POS_DEAD    , do_gen_ps   , LVL_DEMIGOD, SCMD_IMOTD },
#else
  { "imotd"    , "imotd",   POS_DEAD    , do_gen_ps   , LVL_IMMORT, SCMD_IMOTD },
#endif
  { "immlist"  , "immlist", POS_DEAD    , do_gen_ps   , 0, SCMD_IMMLIST },
  { "imp"      , "imp",     POS_RESTING, do_impnet   , LVL_IMPL, 0 },
  { "info"     , "info",    POS_SLEEPING, do_gen_ps   , 0, SCMD_INFO },
  { "insult"   , "insult",  POS_RESTING , do_insult   , 1, 0 },
  { "intro"    , "intro",   POS_RESTING , do_action   , 1, SCMD_INTRO },
  { "jawstrike", "jaw",     POS_FIGHTING, do_jawstrike, 1, 0 },
#ifdef __MAIN_PORT__
  { "invis"    , "invis",   POS_DEAD    , do_invis    , LVL_DEMIGOD, 0 },
#else
  { "invis"    , "invis",   POS_DEAD    , do_invis    , LVL_IMMORT, 0 },
#endif
  { "kill"     , "k",       POS_FIGHTING, do_kill     , 1, 0 },
  { "kick"     , "kick",    POS_FIGHTING, do_kick     , 1, 0 },
  { "kit"      , "kit",     POS_FIGHTING, do_not_here , 1, 0 },
  { "knockout" , "knock",   POS_FIGHTING, do_knockout , 1, 0 },
  { "kiss"     , "kiss",    POS_RESTING , do_action   , 1, 0 },
  { "look"     , "l",       POS_RESTING , do_look     , 0, SCMD_LOOK },
  //{ "lsay"     , "lsay",    POS_RESTING , do_lang_say , 0, 0 },
  { "laugh"    , "laugh",   POS_RESTING , do_action   , 1, 0 },
  { "last"     , "last",    POS_DEAD    , do_last     , LVL_DEPTHEAD, 0 },
  { "lay"      , "lay",     POS_STANDING, do_lay_hands, 1, 0 },
  { "leave"    , "leave",   POS_STANDING, do_leave    , 1, 0 },
  { "levels"   , "levels",  POS_DEAD    , do_levels   , 0, 0 },
  { "list"     , "li",      POS_RESTING, do_not_here , 1, 0 },
  { "lick"     , "lick",    POS_RESTING , do_action   , 1, 0 },
  { "lock"     , "lock",    POS_RESTING , do_gen_door , 1, SCMD_LOCK },
  { "load"     , "load",    POS_DEAD    , do_load     , LVL_DEMIGOD, 0 },
  { "love"     , "love",    POS_RESTING , do_action   , 1, 0 },
  { "makeleader", "makeleader", POS_DEAD , do_makeleader, 1, 0 },
  //{ "mark"     , "mark",    POS_STANDING, do_mark     , 1, 0 },
  { "memorize" , "mem",     POS_RESTING , do_memorize , 1, 0 },
  { "mount"    , "mount",   POS_STANDING, do_mount	   , 1, 0 },
  { "mget"     , "mget",    POS_RESTING , do_mget     , 1, 0 },
  { "mgive"    , "mgive",   POS_RESTING , do_mgive    , 1, 0 },
  { "mreport"  , "mrep",    POS_DEAD    , do_mreport  , 1, 0 },
  { "mrest"    , "mrest",   POS_RESTING    , do_mrest    , 1, 0 },
  { "mstand"   , "msta",    POS_RESTING    , do_mstand   , 1, 0 },
  { "moan"     , "moan",    POS_RESTING , do_action   , 1, 0 },
  { "medit"    , "medit",   POS_DEAD    , do_olc      , LVL_GOD, SCMD_OLC_MEDIT},
  { "medit"    , "medit",   POS_DEAD    , do_olc      , LVL_BUILDER, SCMD_OLC_MEDIT},
  { "motd"     , "motd",    POS_DEAD    , do_gen_ps   , 0, SCMD_MOTD },
  { "mail"     , "mail",    POS_RESTING, do_not_here , 1, 0 },
  { "massage"  , "mass",    POS_RESTING , do_action   , 1, 0 },
  { "mlist"    , "mlist",   POS_DEAD    , do_mlist    , LVL_DEMIGOD, 0 },
  { "mute"     , "mute",    POS_DEAD    , do_wizutil  , LVL_GOD, SCMD_SQUELCH },
  { "murder"   , "mur",     POS_FIGHTING, do_hit      , 1, SCMD_MURDER },
  { "mutter"   , "mutter",  POS_RESTING , do_action   , 1, 0 },
  //{ "mysqlconv", "",    POS_DEAD    , do_mysql_convert, 37, 0 },
  { "newbie"   , "newbie",  POS_SLEEPING, do_newbie   , 0, 0 },
  //{ "news"     , "news",    POS_SLEEPING, do_gen_ps   , 0, SCMD_NEWS },
  { "nibble"   , "nibble",  POS_RESTING , do_action   , 1, 0 },
  { "nod"      , "nod",     POS_RESTING , do_action   , 0, 0 },
  { "nohassle" , "nohass",  POS_DEAD    , do_gen_tog  , LVL_IMMORT, SCMD_NOHASSLE },
  { "norepeat" , "norep",   POS_DEAD    , do_gen_tog  , 1, SCMD_NOREPEAT },
  { "noshout"  , "nosh",    POS_SLEEPING, do_gen_tog  , 1, SCMD_DEAF },
  /* { "nosummon" , POS_DEAD    , do_gen_tog  , 1, SCMD_NOSUMMON }, */
  { "notell"   , "notell",  POS_DEAD    , do_gen_tog  , 1, SCMD_NOTELL },
  { "nowiz"    , "nowiz",   POS_DEAD    , do_gen_tog  , 1, SCMD_NOWIZ },
  { "nonewbie" , "nonewb",  POS_DEAD    , do_gen_tog  , 1, SCMD_NONEWBIE },
  { "nochat"   , "nochat",  POS_DEAD    , do_gen_tog  , 1, SCMD_NOCHAT },
  { "noooc"    , "noooc",   POS_DEAD    , do_gen_tog  , 1, SCMD_NOOOC },
  { "nudge"    , "nudge",   POS_RESTING , do_action   , 1, 0 },
  { "nuzzle"   , "nuzzle",  POS_RESTING , do_action   , 1, 0 },
  { "ooc"      , "ooc",     POS_SLEEPING, do_say      , 1, SCMD_OOCSAY },
  { "order"    , "ord",     POS_RESTING , do_order    , 1, 0 },
  { "offer"    , "off",     POS_STANDING, do_not_here , 1, 0 },
  { "open"     , "op",      POS_SITTING , do_gen_door , 0, SCMD_OPEN },
  { "olc"      , "olc",     POS_DEAD    , do_olc      , LVL_BUILDER, SCMD_OLC_SAVEINFO },
  { "olist"    , "olist",   POS_DEAD    , do_olist    , LVL_DEMIGOD, 0 },
  { "oedit"    , "oedit",   POS_DEAD    , do_olc      , LVL_IMPL, SCMD_OLC_OEDIT},
  { "oedit"    , "oedit",   POS_DEAD    , do_olc      , LVL_BUILDER, SCMD_OLC_OEDIT},
  { "osayto"   , "osay",    POS_SLEEPING, do_sayto    , 1, SCMD_OOCSAY }, 
//  { "objconv"  , POS_DEAD    , do_objconv  , 37, 0 },	
  { "put"      , "put",     POS_RESTING, do_put      , 0, 0 },
  { "pat"      , "pat",     POS_RESTING , do_action   , 1, 0 },
  { "page"     , "page",    POS_DEAD    , do_page     , LVL_LESSERGOD, 0 },
  { "pardon"   , "pardon",  POS_DEAD    , do_wizutil  , LVL_ADMIN, SCMD_PARDON },
  { "peer"     , "peer",    POS_RESTING , do_action   , 1, 0 },
  { "pedit"    , "pedit",   POS_RESTING , do_pedit    , LVL_APPROVE, 0 },
  { "pick"     , "pick",    POS_STANDING, do_gen_door , 1, SCMD_PICK },
  { "point"    , "point",   POS_RESTING , do_action   , 1, 0 },
  { "poke"     , "poke",    POS_RESTING , do_action   , 1, 0 },
  { "policy"   , "policy",  POS_DEAD    , do_gen_ps   , 0, SCMD_POLICIES },
  { "ponder"   , "ponder",  POS_RESTING , do_action   , 1, 0 },
  { "poofin"   , "poofin",  POS_DEAD    , do_poofset  , LVL_IMMORT, SCMD_POOFIN },
  { "poofout"  , "poofout", POS_DEAD    , do_poofset  , LVL_IMMORT, SCMD_POOFOUT },
  { "pour"     , "pour",    POS_RESTING, do_pour     , 1, SCMD_POUR },
  { "pounce"   , "pounce",  POS_STANDING, do_action   , 1, 0 },
  { "players"  , "play",    POS_DEAD    , do_players  , LVL_GOD, 0 },
//  { "plip"     , "",  POS_RESTING , do_plip     , LVL_DEPTHEAD, 0 },
  { "pout"     , "pout",    POS_RESTING , do_action   , 1, 0 },
  { "prompt"   , "prompt",  POS_DEAD    , do_display  , 1, 0 },
  { "practice" , "prac",    POS_RESTING , do_practice , 1, 0 },
  { "pray"     , "pray",    POS_SITTING , do_action   , 1, 0 },
  { "prayer"   , "prayer",  POS_SITTING , do_prayer   , 1, 0 },
  { "puke"     , "puke",    POS_RESTING, do_action   , 1, 0 },
  { "punch"    , "punch",   POS_RESTING , do_action   , 1, 0 },
  { "purr"     , "purr",    POS_RESTING , do_action   , 1, 0 },
  { "purge"    , "purge",   POS_DEAD    , do_purge    , LVL_DEMIGOD, 0 },
  { "quaff"    , "quaff",   POS_RESTING , do_use      , 1, SCMD_QUAFF },
//{ "qecho"    , "",    POS_DEAD    , do_qcomm    , LVL_IMMORT, SCMD_QECHO },
//{ "quest"    , "",    POS_DEAD    , do_gen_tog  , LVL_IMMORT, SCMD_QUEST },
  { "qui"      , "qui",    POS_DEAD    , do_quit     , 1, 0 },
  { "quit"     , "quit",    POS_DEAD    , do_quit     , 0, 0 },
  { "quitno"   , "quitno",  POS_DEAD    , do_quit     , 0, 0 },
  { "quitnow"  , "quitnow", POS_DEAD    , do_quit     , 0, SCMD_QUIT },
//{ "qsay"     , "",    POS_RESTING , do_qcomm    , 1, SCMD_QSAY },
  { "reply"    , "r",       POS_SLEEPING, do_reply    , 0, 0 },
  { "reel"     , "reel",    POS_SITTING,  do_reel     , 0, 0 },
  { "reimburse", "reim",    POS_DEAD    , do_reimburse, LVL_GRGOD, 0 },
  { "award"    , "award",   POS_DEAD    , do_award    , LVL_GRGOD, 0 },
  { "relax"    , "relax",   POS_DEAD,     do_relax    , 1, 0 },
  { "raise"    , "raise",   POS_RESTING , do_action   , 1, 0 },
  { "rest"     , "rest",    POS_RESTING , do_rest     , 0, 0 },
  { "read"     , "read",    POS_RESTING , do_look     , 1, SCMD_READ },
  { "reload"   , "reload",  POS_DEAD    , do_reboot   , LVL_IMPL, 0 },
  { "recite"   , "recite",  POS_STANDING, do_use      , 1, SCMD_RECITE },
  { "receive"  , "receive", POS_STANDING, do_not_here , 1, 0 },
  { "reject"   , "reject",  POS_RESTING , do_approve  , LVL_APPROVE, SCMD_REJECT },
  { "remove"   , "rem",     POS_RESTING , do_remove   , 1, 0 },
  { "rent"     , "rent",    POS_STANDING, do_not_here , 1, 0 },
  { "report"   , "rep",     POS_RESTING , do_report   , 1, 0 },
  /* { "reroll"   , "", POS_DEAD    , do_wizutil  , LVL_IMPL, SCMD_REROLL }, */
  { "rescue"   , "res",     POS_FIGHTING, do_rescue   , 1, 0 },
  { "restore"  , "restore", POS_DEAD    , do_restore  , LVL_GRGOD, 0 },
  { "retreat"  , "retreat", POS_FIGHTING, do_retreat  , 1, 0 },
  { "return"   , "return",  POS_DEAD    , do_return   , 1, 0 },
  //{ "reveal"   , "",    POS_FIGHTING, do_reveal   , 1, 0 },
  { "release"  , "release", POS_STANDING, do_release  , 1, 0 },
  { "redit"    , "redit",   POS_DEAD    , do_olc      , LVL_IMPL, SCMD_OLC_REDIT},
  { "redit"    , "redit",   POS_DEAD    , do_olc      , LVL_BUILDER, SCMD_OLC_REDIT},
  { "rlist"    , "rlist",   POS_DEAD    , do_rlist    , LVL_DEMIGOD, 0 },
  { "roll"     , "roll",    POS_RESTING , do_action   , 1, 0 },
  { "rofl"     , "rofl",    POS_RESTING , do_action   , 1, 0 },
  { "roomflags", "roomf",   POS_DEAD    , do_gen_tog  , LVL_DEMIGOD, SCMD_ROOMFLAGS },
  { "ruffle"   , "ruffle",  POS_STANDING, do_action   , 1, 0 },
  { "rules"    , "rules",   POS_DEAD    , do_gen_ps   , 0, SCMD_POLICIES },
  { "say"      , "say",     POS_RESTING , do_say      , 0, 0 },
  { "'"        , "'",       POS_RESTING , do_say      , 0, 0 },
  { "sayto"    , "sayto",   POS_RESTING , do_sayto    , 0, 0 },  
  { "save"     , "save",    POS_SLEEPING, do_save     , 0, 0 },
  { "scribe"   , "scribe",  POS_SITTING,  do_scribe   , 0, 0 },
  { "scan"     , "sca",     POS_RESTING , do_scan     , 1, 0 },
  { "score"    , "sco",     POS_DEAD    , do_score    , 1, 0 },
  { "scold"    , "scold",   POS_STANDING, do_action   , 1, 0 },
  { "scream"   , "scream",  POS_RESTING , do_action   , 1, 0 },
  { "sell"     , "sell",    POS_STANDING, do_not_here , 1, 0 },
  { "send"     , "send",    POS_SLEEPING, do_send     , LVL_GOD, 0 },
  { "set"      , "set",     POS_DEAD    , do_set      , LVL_GOD, 0 },
  { "sedit"    , "sedit",   POS_DEAD    , do_olc      , LVL_IMPL, SCMD_OLC_SEDIT},
  { "sedit"    , "sedit",   POS_DEAD    , do_olc      , LVL_BUILDER, SCMD_OLC_SEDIT},
  { "shout"    , "sh",      POS_RESTING , do_gen_comm , 1, SCMD_SHOUT },
  { "shake"    , "shake",   POS_RESTING , do_action   , 1, 0 },
  { "shiver"   , "shiver",  POS_RESTING , do_action   , 1, 0 },
  { "show"     , "show",    POS_RESTING , do_show     , 1, 0 },
  { "gshow"    , "gshow",   POS_DEAD    , do_gshow    , LVL_DEMIGOD, 0 },
  { "gdetect"  , "gdetect", POS_DEAD    , do_gen_tog  , LVL_IMMORT, SCMD_DETECT },
  { "sheath"   , "sheath",  POS_DEAD    , do_sheath   , 1, 0 },
  { "shrug"    , "shrug",   POS_RESTING , do_action   , 1, 0 },
  { "shred"    , "shred",   POS_STANDING, do_not_here , 1, 0 },
  { "shutdow"  , "shutdow", POS_DEAD    , do_shutdown , LVL_IMPL, 0 },
  { "shutdown" , "shutdown",POS_DEAD    , do_shutdown , LVL_IMPL, SCMD_SHUTDOWN },
  { "sigh"     , "sigh",    POS_RESTING , do_action   , 1, 0 },
  { "sing"     , "sing",    POS_RESTING , do_action   , 1, 0 },
  { "sip"      , "sip",     POS_RESTING , do_drink    , 1, SCMD_SIP },
  { "sit"      , "sit",     POS_RESTING , do_sit      , 0, 0 },
  /* { "skills"   , "", POS_SLEEPING, do_skills   , 1, 0 }, */
  { "skillset" , "skills",  POS_SLEEPING, do_skillset , LVL_GRGOD, 0 },
  { "skin"     , "skin",    POS_STANDING, do_skin     , 0, 0 },
  { "sleep"    , "sl",      POS_SLEEPING, do_sleep    , 0, 0 },
  { "slap"     , "slap",    POS_RESTING , do_action   , 1, 0 },
  { "slowns"   , "slowns",  POS_DEAD    , do_gen_tog  , LVL_IMPL, SCMD_SLOWNS },
  { "smile"    , "smile",   POS_RESTING , do_action   , 1, 0 },
  { "smirk"    , "smirk",   POS_RESTING , do_action   , 1, 0 },
  { "sneak"    , "sn",      POS_RESTING , do_sneak    , 1, 0 },
  { "snicker"  , "snick",   POS_RESTING , do_action   , 1, 0 },
  { "snap"     , "snap",    POS_RESTING , do_action   , 1, 0 },
  { "snarl"    , "snarl",   POS_RESTING , do_action   , 1, 0 },
  { "sneeze"   , "sneeze",  POS_RESTING , do_action   , 1, 0 },
  { "sniff"    , "sniff",   POS_RESTING , do_action   , 1, 0 },
  { "snore"    , "snore",   POS_SLEEPING, do_action   , 1, 0 },
  { "snort"    , "snort",   POS_RESTING , do_action   , 1, 0 },
  //{ "snowball" , "",    POS_STANDING, do_action   , LVL_DEMIGOD, 0 },
  { "snowball" , "snowball",    POS_STANDING, do_action   , LVL_IMMORT, 0 },
  { "snoop"    , "snoop",   POS_DEAD    , do_snoop    , LVL_DEPTHEAD, 0 },
  { "snuggle"  , "snuggle", POS_RESTING , do_action   , 1, 0 },
  { "socials"  , "soc",     POS_DEAD    , do_commands , 1, SCMD_SOCIALS },
  { "split"    , "split",   POS_RESTING , do_split    , 1, 0 },
  { "spank"    , "spank",   POS_RESTING , do_action   , 1, 0 },
  { "spells"   , "sp",      POS_RESTING , do_spells   , 1, 0 },
  { "spit"     , "spit",    POS_STANDING, do_action   , 1, 0 },
  { "squeeze"  , "squeeze", POS_RESTING , do_action   , 1, 0 },
  { "stand"    , "sta",     POS_RESTING , do_stand    , 0, 0 },
  { "stagger"  , "stagger", POS_RESTING , do_action   , 1, 0 },
  { "stare"    , "stare",   POS_RESTING , do_action   , 1, 0 },
  { "stat"     , "stat",    POS_DEAD    , do_stat     , LVL_ADMIN, 0 },
  { "quest"    , "quest",   POS_DEAD    , do_quest    , LVL_LESSERGOD, 0 },
  { "steal"    , "steal",   POS_STANDING, do_steal    , 1, 0 },
  { "steam"    , "steam",   POS_RESTING , do_action   , 1, 0 },
  { "stroke"   , "stroke",  POS_RESTING , do_action   , 1, 0 },
  { "strut"    , "strut",   POS_STANDING, do_action   , 1, 0 },
  { "subdue"   , "sub",     POS_RESTING , do_subdue   , 1, 0 },
  { "sulk"     , "sulk",    POS_RESTING , do_action   , 1, 0 },
  { "surrender", "surr",    POS_RESTING , do_action   , 1, 0 },
  { "switch"   , "switch",  POS_DEAD    , do_switch   , LVL_DEPTHEAD, 0 },
  { "syslog"   , "syslog",  POS_DEAD    , do_syslog   , LVL_LESSERGOD, 0 },
  { "stance"   , "stance",  POS_STANDING, do_stance   , 1, 0 },
  { "tame"     , "tame",    POS_STANDING, do_tame     , 0, 0 },
  { "tell"     , "tell",    POS_DEAD    , do_tell     , 0, 0 },
  { "tackle"   , "tackle",  POS_RESTING , do_action   , 1, 0 },
  { "take"     , "take",    POS_RESTING , do_get      , 0, 0 },
  { "tango"    , "tango",   POS_STANDING, do_action   , 1, 0 },
  { "taunt"    , "taunt",   POS_RESTING , do_action   , 1, 0 },
  { "taste"    , "taste",   POS_RESTING , do_eat      , 1, SCMD_TASTE },
  { "teleport" , "tel",     POS_DEAD    , do_teleport , LVL_GOD, 0 },
  { "thank"    , "thank",   POS_RESTING , do_action   , 1, 0 },
  { "think"    , "think",   POS_RESTING , do_action   , 1, 0 },
  { "thaw"     , "thaw",    POS_DEAD    , do_wizutil  , LVL_FREEZE, SCMD_THAW },
  { "throw"    , "throw",   POS_FIGHTING, do_throw    , 1, 0 },
  { "title"    , "ti",      POS_DEAD    , do_title    , 2, 0 },
  { "tickle"   , "tickle",  POS_RESTING , do_action   , 1, 0 },
  { "time"     , "time",    POS_DEAD    , do_time     , 0, 0 },
  { "toggle"   , "toggle",  POS_DEAD    , do_toggle   , 1, 0 },
  { "track"    , "tra",     POS_STANDING, do_track    , 1, 0 },
  { "transfer" , "trans",   POS_SLEEPING, do_trans    , LVL_GOD, 0 },
  { "trigedit" , "triged",  POS_DEAD    , do_olc      , 35, SCMD_OLC_TRIGEDIT},
  { "trip"     , "trip",    POS_FIGHTING, do_trip     , 1, 0 },
  { "try"      , "try",     POS_STANDING, do_not_here , 1, 0 },
  { "turn"     , "turn",    POS_STANDING, do_turn_undead, 1, 0 },
  { "twiddle"  , "twiddle", POS_RESTING , do_action   , 1, 0 },
  { "typo"     , "typo",    POS_DEAD    , do_gen_write, 1, SCMD_TYPO },
  { "unlock"   , "unl",     POS_SITTING , do_gen_door , 1, SCMD_UNLOCK },
  { "unaffect" , "unaff",   POS_DEAD    , do_wizutil  , LVL_GOD, SCMD_UNAFFECT },
  { "ungroup"  , "ung",     POS_DEAD    , do_ungroup  , 1, 0 },
  { "unearth"  , "unearth", POS_RESTING, do_unearth  , 1, 0 },
  { "unban"    , "unban",   POS_DEAD    , do_unban    , LVL_IMPL, 0 },
  { "unpoison" , "unpoi",   POS_STANDING, do_remove_poison, 1, 0 },
  { "uptime"   , "uptime",  POS_DEAD    , do_date     , LVL_IMMORT, SCMD_UPTIME },
  { "use"      , "use",     POS_SITTING , do_use      , 1, SCMD_USE },
  { "users"    , "users",   POS_DEAD    , do_users    , LVL_DEMIGOD, 0 },
  { "unsheath" , "unsh",    POS_FIGHTING, do_unsheath , 1, 0 },
  { "value"    , "val",     POS_STANDING, do_not_here , 1, 0 },
  { "version"  , "ver",     POS_DEAD    , do_gen_ps   , 0, SCMD_VERSION },
  { "visible"  , "vis",     POS_RESTING , do_visible  , 1, 0 },
  { "vnum"     , "vnum",    POS_DEAD    , do_vnum     , LVL_DEMIGOD, 0 },
  { "vstat"    , "vst",     POS_DEAD    , do_vstat    , LVL_ADMIN, 0 },
  { "vwear"    , "vwear",   POS_DEAD    , do_vwear    , LVL_ADMIN, 0 },
  { "wake"     , "wake",    POS_SLEEPING, do_wake     , 0, 0 },
  { "wave"     , "wave",    POS_RESTING , do_action   , 1, 0 },
  { "wear"     , "wear",    POS_RESTING , do_wear     , 1, 0 },
  { "weather"  , "weather", POS_RESTING , do_weather  , 0, 0 },
  { "who"      , "who",     POS_DEAD    , do_who      , 0, 0 },
  { "whoami"   , "whoami",  POS_DEAD    , do_gen_ps   , 0, SCMD_WHOAMI },
  { "where"    , "where",   POS_RESTING , do_where    , LVL_IMMORT, 0 },
  { "whisper"  , "whis",    POS_RESTING , do_spec_comm, 1, SCMD_WHISPER },
  { "whimper"  , "whim",    POS_RESTING , do_action   , 1, 0 },
  { "whine"    , "whine",   POS_RESTING , do_action   , 1, 0 },
  { "whistle"  , "whistle", POS_RESTING , do_action   , 1, 0 },
  { "wield"    , "wi",      POS_RESTING , do_wield    , 1, 0 },
  { "wiggle"   , "wiggle",  POS_STANDING, do_action   , 1, 0 },
  { "wimpy"    , "wimp",    POS_DEAD    , do_wimpy    , 1, 0 },
  { "wink"     , "wink",    POS_RESTING , do_action   , 1, 0 },
  { "withdraw" , "with",    POS_STANDING, do_not_here , 1, 0 },
  { "wiznet"   , "wiz",     POS_DEAD    , do_wiznet   , 1, 0 },
  { ";"        , ";",       POS_DEAD    , do_wiznet   , 1, 0 },
  //{ "wiznet"   , "wiz",    POS_DEAD    , do_wiznet   , 1, 0 },
  //{ ";"        , "",    POS_DEAD    , do_wiznet   , 1, 0 },
  { "wizhelp"  , "wizhelp", POS_SLEEPING, do_commands , LVL_IMMORT, SCMD_WIZHELP },
  { "wizlist"  , "wizlist", POS_DEAD    , do_gen_ps   , 0, SCMD_WIZLIST },
  { "wizlock"  , "wizlock", POS_DEAD    , do_wizlock  , LVL_IMPL, 0 },
  { "worship"  , "worship", POS_RESTING , do_action   , 1, 0 },
  { "write"    , "write",   POS_STANDING, do_write    , 1, 0 },
  { "xapobjs"  , "xapobjs", POS_DEAD,     do_gen_tog  , 37, SCMD_XAP_OBJS },	
  { "yawn"     , "yawn",    POS_RESTING , do_action   , 1, 0 },
  { "yodel"    , "yodel",   POS_RESTING , do_action   , 1, 0 },
  { "zedit"    , "zedit",   POS_DEAD    , do_olc      , LVL_IMPL, SCMD_OLC_ZEDIT},
  { "zedit"    , "zedit",   POS_DEAD    , do_olc      , LVL_BUILDER, SCMD_OLC_ZEDIT},
  { "zreset"   , "zreset",  POS_DEAD    , do_zreset   , LVL_DEMIGOD, 0 },
  { "zstat"    , "zstat",   POS_DEAD    , do_zstat    , LVL_DEMIGOD, 0 },

  /* DG trigger commands */
  { "attach"   , "attach",  POS_DEAD    , do_attach   , LVL_IMPL, 0 },
  { "detach"   , "detach",  POS_DEAD    , do_detach   , LVL_IMPL, 0 },
  { "tlist"    , "tlist",   POS_DEAD    , do_tlist    , LVL_DEMIGOD, 0 },
  { "tstat"    , "tstat",   POS_DEAD    , do_tstat    , LVL_GOD, 0 },
  { "masound"  , "maso",    POS_DEAD    , do_masound  , 2, 0 },
  { "mkill"    , "mkill",   POS_STANDING, do_mkill    , 2, 0 },
  { "mjunk"    , "mjunk",   POS_SITTING , do_mjunk    , 2, 0 },
  { "mechoaround" , "mechoaround",     POS_DEAD , do_mechoaround    , 2, 0 },
  { "msend"    , "msend",   POS_DEAD    , do_msend    , 2, 0 },
  { "mecho"    , "mecho",   POS_DEAD    , do_mecho    , 2, 0 },
  { "mload"    , "mload",   POS_DEAD    , do_mload    , 2, 0 },
  { "mpurge"   , "mpurge",  POS_DEAD    , do_mpurge   , 2, 0 },
  { "mgoto"    , "mgoto",   POS_DEAD    , do_mgoto    , 2, 0 },
  { "mat"      , "mat",     POS_DEAD    , do_mat      , 2, 0 },
  { "mteleport", "mtele",   POS_DEAD    , do_mteleport, 2, 0 },
  { "mforce"   , "mforce",  POS_DEAD    , do_mforce   , 2, 0 },
  { "mexp"     , "mexp",    POS_DEAD    , do_mexp     , 2, 0 },
  { "\n", 0, 0, 0, 0 } };	/* this must be last */

struct command_info *complete_cmd_info;

static const char *fill[] =
{
  "in",
  "from",
  "with",
  "the",
  "on",
  "at",
  "to",
  "\n"
};

static const char *reserved[] =
{
  "a",
  "an",
  "self",
  "me",
  "all",
  "room",
  "someone",
  "something",
  "\n"
};

/*
 * This is the actual command interpreter called from game_loop() in comm.c
 * It makes sure you are the proper level and position to execute the command,
 * then calls the appropriate function.
 */
void command_interpreter(struct char_data *ch, char *argument)
{
  int cmd, length;
  extern int no_specials;
  char *line;
  //struct char_data *temp;

  //REMOVE_BIT(AFF_FLAGS(ch), AFF_HIDE);

  /* just drop to next line for hitting CR */
  skip_spaces(&argument);
  if (!*argument)
    return;

  /*
   * special case to handle one-character, non-alphanumeric commands;
   * requested by many people so "'hi" or ";godnet test" is possible.
   * Patch sent by Eric Green and Stefan Wasilewski.
   */
  if (!isalpha(*argument)) {
    arg[0] = argument[0];
    arg[1] = '\0';
    line = argument + 1;
  } else
    line = any_one_arg(argument, arg);

  if (PLR_FLAGGED(ch, PLR_FROZEN) && (GET_LEVEL(ch) < LVL_IMPL))
  {
    send_to_char("You cannot move due to the mind-numbing cold...\r\n", ch);
    return;
  }

  if (affected_by_spell(ch, SPELL_PARALYZE) && (GET_LEVEL(ch) < LVL_IMMORT))
  {
    send_to_char("Your muscles refuse to respond.\r\n", ch);
    return;
  }

  if ((GET_LEVEL(ch) < LVL_IMMORT) && !affected_by_spell(ch, SPELL_STATUE) && !AFF_FLAGGED(ch, AFF_STANCE) &&
      (command_wtrigger(ch, arg, line) ||
       command_mtrigger(ch, arg, line) ||
       command_otrigger(ch, arg, line)))
    return; /* command trigger took over */

  /* otherwise, find the command */
  for (length = strlen(arg), cmd = 0; *cmd_info[cmd].command != '\n'; cmd++)
    if (!strncmp(cmd_info[cmd].command, arg, length))
      if (GET_TRUST_LEVEL(ch) >= cmd_info[cmd].minimum_level || (IS_NPC(ch) && MOB_FLAGGED(ch, MOB_QUEST) && ( strcmp(cmd_info[cmd].command, "echo") || strcmp(cmd_info[cmd].command, "zecho") || strcmp(cmd_info[cmd].command, "aecho"))))
	break;

  if (strcmp(cmd_info[cmd].command, "break") && strcmp(cmd_info[cmd].command, "look") && strcmp(cmd_info[cmd].command, "say") && affected_by_spell(ch, SPELL_STATUE))
  {
    send_to_char("You can do nothing now except break the spell.\r\n", ch);
    return;
  }

 if (strcmp(cmd_info[cmd].command, "relax") && strcmp(cmd_info[cmd].command, "look") && strcmp(cmd_info[cmd].command, "say") && strcmp(cmd_info[cmd].command, "emote") && AFF_FLAGGED(ch, AFF_STANCE))
  {
    send_to_char("You can do nothing in this stance.\r\n", ch);
    return;
  }

  if (*cmd_info[cmd].command == '\n') {
    send_to_char("Huh?!?\r\n", ch);
  } else if (cmd_info[cmd].command_pointer == NULL) {
    send_to_char("Sorry, that command hasn't been implemented yet.\r\n", ch);
  } else if (IS_NPC(ch) && cmd_info[cmd].minimum_level >= LVL_IMMORT && strcmp(cmd_info[cmd].command, "echo") && strcmp(cmd_info[cmd].command, "zecho") && strcmp(cmd_info[cmd].command, "aecho")) {
    send_to_char("You can't use immortal commands while switched.\r\n", ch);
  } 
     else if (GET_POS(ch) < cmd_info[cmd].minimum_position) {
    switch (GET_POS(ch)) {
    case POS_DEAD:
      send_to_char("Lie still; you are DEAD!!! :-(\r\n", ch);
      break;
    case POS_INCAP:
    case POS_MORTALLYW:
      send_to_char("You are in a pretty bad shape, unable to do anything!\r\n", ch);
      break;
    case POS_STUNNED:
      send_to_char("You are unconscious!\r\n", ch);
      break;
    case POS_SLEEPING:
      send_to_char("Perhaps when you awaken.\r\n", ch);
      break;
    case POS_DIGGING:
      send_to_char("Perhaps you should stop digging first!\r\n", ch);
      break;
    case POS_RESTING:
      send_to_char("Why not relax awhile longer?\r\n", ch);
      break;
    case POS_SITTING:
      send_to_char("Oh, but it's so comfortable where you're sitting.\r\n", ch);
      break;
    case POS_FISHING:
      send_to_char("But you are fishing!\r\n", ch);
      break;
    case POS_RIDING:
      send_to_char("Perhaps you should dismount first?\r\n", ch);
      break;
    case POS_FIGHTING:
      send_to_char("You can't find a way to do that and fight at the same time.\r\n", ch);
      break;
    }
  } else if (no_specials || !special(ch, cmd, line)) {
    ((*cmd_info[cmd].command_pointer) (ch, line, cmd, cmd_info[cmd].subcmd));
  }
}

/**************************************************************************
 * Routines to handle aliasing                                             *
  **************************************************************************/


struct alias_data *find_alias(struct alias_data *alias_list, char *str)
{
  while (alias_list != NULL) {
    if (*str == *alias_list->alias)	/* hey, every little bit counts :-) */
      if (!strcmp(str, alias_list->alias))
	return alias_list;

    alias_list = alias_list->next;
  }

  return NULL;
}


void free_alias(struct alias_data *a)
{
  if (a->alias)
    free(a->alias);
  if (a->replacement)
    free(a->replacement);
  free(a);
}


/* The interface to the outside world: do_alias */
ACMD(do_alias)
{
  char *repl;
  struct alias_data *a, *temp;

  if (IS_NPC(ch))
    return;

  repl = any_one_arg(argument, arg);

  if (!*arg) {			/* no argument specified -- list currently defined aliases */
    send_to_char("Currently defined aliases:\r\n", ch);
    if ((a = GET_ALIASES(ch)) == NULL)
      send_to_char(" None.\r\n", ch);
    else {
      while (a != NULL) {
	sprintf(buf, "%-15s %s\r\n", a->alias, a->replacement);
	send_to_char(buf, ch);
	a = a->next;
      }
    }
  } else {			/* otherwise, add or remove aliases */
    /* is this an alias we've already defined? */
    if ((a = find_alias(GET_ALIASES(ch), arg)) != NULL) {
      REMOVE_FROM_LIST(a, GET_ALIASES(ch), next);
      free_alias(a);
    }
    /* if no replacement string is specified, assume we want to delete */
    if (!*repl) {
      if (a == NULL)
	send_to_char("No such alias.\r\n", ch);
      else
	send_to_char("Alias deleted.\r\n", ch);
    } else {			/* otherwise, either add or redefine an alias */
      if (!str_cmp(arg, "alias")) {
	send_to_char("You can't alias 'alias'.\r\n", ch);
	return;
      }
      CREATE(a, struct alias_data, 1);
      a->alias = str_dup(arg);
      delete_doubledollar(repl);
      a->replacement = str_dup(repl);
      if (strchr(repl, ALIAS_SEP_CHAR) || strchr(repl, ALIAS_VAR_CHAR))
	a->type = ALIAS_COMPLEX;
      else
	a->type = ALIAS_SIMPLE;
      a->next = GET_ALIASES(ch);
      GET_ALIASES(ch) = a;
      send_to_char("Alias added.\r\n", ch);
    }
  }
}

/*
 * Valid numeric replacements are only $1 .. $9 (makes parsing a little
 * easier, and it's not that much of a limitation anyway.)  Also valid
 * is "$*", which stands for the entire original line after the alias.
 * ";" is used to delimit commands.
 */
#define NUM_TOKENS       9

void perform_complex_alias(struct txt_q *input_q, char *orig, struct alias_data *a)
{
  struct txt_q temp_queue;
  char *tokens[NUM_TOKENS], *temp, *write_point;
  int num_of_tokens = 0, num;

  /* First, parse the original string */
  temp = strtok(strcpy(buf2, orig), " ");
  while (temp != NULL && num_of_tokens < NUM_TOKENS) {
    tokens[num_of_tokens++] = temp;
    temp = strtok(NULL, " ");
  }

  /* initialize */
  write_point = buf;
  temp_queue.head = temp_queue.tail = NULL;

  /* now parse the alias */
  for (temp = a->replacement; *temp; temp++) {
    if (*temp == ALIAS_SEP_CHAR) {
      *write_point = '\0';
      buf[MAX_INPUT_LENGTH - 1] = '\0';
      write_to_q(buf, &temp_queue, 1);
      write_point = buf;
    } else if (*temp == ALIAS_VAR_CHAR) {
      temp++;
      if ((num = *temp - '1') < num_of_tokens && num >= 0) {
	strcpy(write_point, tokens[num]);
	write_point += strlen(tokens[num]);
      } else if (*temp == ALIAS_GLOB_CHAR) {
	strcpy(write_point, orig);
	write_point += strlen(orig);
      } else if ((*(write_point++) = *temp) == '$')	/* redouble $ for act safety */
	*(write_point++) = '$';
    } else
      *(write_point++) = *temp;
  }

  *write_point = '\0';
  buf[MAX_INPUT_LENGTH - 1] = '\0';
  write_to_q(buf, &temp_queue, 1);

  /* push our temp_queue on to the _front_ of the input queue */
  if (input_q->head == NULL)
    *input_q = temp_queue;
  else {
    temp_queue.tail->next = input_q->head;
    input_q->head = temp_queue.head;
  }
}


/*
 * Given a character and a string, perform alias replacement on it.
 *
 * Return values:
 *   0: String was modified in place; call command_interpreter immediately.
 *   1: String was _not_ modified in place; rather, the expanded aliases
 *      have been placed at the front of the character's input queue.
 */
int perform_alias(struct descriptor_data *d, char *orig)
{
  char first_arg[MAX_INPUT_LENGTH], *ptr;
  struct alias_data *a, *tmp;

  /* Mobs don't have aliases. */
  if (IS_NPC(d->character))
    return (0);

  /* bail out immediately if the guy doesn't have any aliases */
  if ((tmp = GET_ALIASES(d->character)) == NULL)
    return 0;

  /* find the alias we're supposed to match */
  ptr = any_one_arg(orig, first_arg);

  /* bail out if it's null */
  if (!*first_arg)
    return 0;

  /* if the first arg is not an alias, return without doing anything */
  if ((a = find_alias(tmp, first_arg)) == NULL)
    return 0;

  if (a->type == ALIAS_SIMPLE) {
    strcpy(orig, a->replacement);
    return 0;
  } else {
    perform_complex_alias(&d->input, ptr, a);
    return 1;
  }
}



/***************************************************************************
 * Various other parsing utilities                                         *
 **************************************************************************/

/*
 * searches an array of strings for a target string.  "exact" can be
 * 0 or non-0, depending on whether or not the match must be exact for
 * it to be returned.  Returns -1 if not found; 0..n otherwise.  Array
 * must be terminated with a '\n' so it knows to stop searching.
 */
int search_block(char *arg, const char **list, int exact)
{
  register int i, l;

  /* Make into lower case, and get length of string */
  for (l = 0; *(arg + l); l++)
    *(arg + l) = LOWER(*(arg + l));

  if (exact) {
    for (i = 0; **(list + i) != '\n'; i++)
      if (!strcmp(arg, *(list + i)))
	return (i);
  } else {
    if (!l)
      l = 1;			/* Avoid "" to match the first available string */
    for (i = 0; **(list + i) != '\n'; i++)
      if (!strncmp(arg, *(list + i), l))
	return (i);
  }

  return -1;
}


int is_number(char *str)
{
  while (*str)
    if (!isdigit(*(str++)))
      return 0;

  return 1;
}


int is_money(char *str)
{
  /*
   *  [0-9]+[psgc]
   */
  if (str == NULL)
    return 0;

  while (*str) {
    if ( ! ( ( isdigit(*(str)) ) || ( *(str)=='p' ) ||
	     ( *(str)=='g' ) || ( *(str)=='s' ) || ( *(str)=='c' ) ) ) {
      return 0;
    }
    str++;
  }

  return 1;
}


void value_money(struct money_data * tar, char *str)
{
  int              val = 0;
  char             type;

  tar->platinum = tar->steel = tar->gold = tar->copper = 0;

  val = atoi(str);

  type = str[strlen(str)-1];

  if ( val < 0 ) {
    log("value_money: val < 0");
  }

  switch(type) {
  case 'p':
    tar->platinum = val;
    break;
  case 's':
    tar->steel = val;
    break;
  case 'g':
    tar->gold = val;
    break;
  case 'c':
    tar->copper = val;
    break;
  default:
    log("value_money: switch error");
    break;
  }
}



/*
 * Function to skip over the leading spaces of a string.
 */
void skip_spaces(char **string)
{
  for (; **string && isspace(**string); (*string)++);
}

void skip_dash(char **string)
{
  for (; **string && (**string == '-'); (*string)++);
}

void skip_dot(char **string)
{
  for (; **string && (**string == '.'); (*string)++);
}


/*
 * Given a string, change all instances of double dollar signs ($$) to
 * single dollar signs ($).  When strings come in, all $'s are changed
 * to $$'s to avoid having users be able to crash the system if the
 * inputted string is eventually sent to act().  If you are using user
 * input to produce screen output AND YOU ARE SURE IT WILL NOT BE SENT
 * THROUGH THE act() FUNCTION (i.e., do_gecho, do_title, but NOT do_say),
 * you can call delete_doubledollar() to make the output look correct.
 *
 * Modifies the string in-place.
 */
char *delete_doubledollar(char *string)
{
  char *read, *write;

  /* If the string has no dollar signs, return immediately */
  if ((write = strchr(string, '$')) == NULL)
    return string;


  /* If the string has no dollar signs, return immediately */
  if ((write = strchr(string, '$')) == NULL)
    return string;

  /* Start from the location of the first dollar sign */
  read = write;


  while (*read)   /* Until we reach the end of the string... */
    if ((*(write++) = *(read++)) == '$') /* copy one char */
      if (*read == '$')
	read++; /* skip if we saw 2 $'s in a row */

  *write = '\0';

  return string;
}


int fill_word(char *argument)
{
  return (search_block(argument, fill, TRUE) >= 0);
}


int reserved_word(char *argument)
{
  return (search_block(argument, reserved, TRUE) >= 0);
}


/*
 * copy the first non-fill-word, space-delimited argument of 'argument'
 * to 'first_arg'; return a pointer to the remainder of the string.
 */
char *one_argument(char *argument, char *first_arg)
{
  char *begin = first_arg;

  if (!argument) {
    log("SYSERR: one_argument received a NULL pointer!");
    first_arg = '\0';
    return (NULL);;
  }

  do {
    skip_spaces(&argument);

    first_arg = begin;
    while (*argument && !isspace(*argument)) {
      *(first_arg++) = LOWER(*argument);
      argument++;
    }

    *first_arg = '\0';
  } while (fill_word(begin));

  return argument;
}

char *one_arg_dash(char *argument, char *first_arg)
{
  skip_dash(&argument);

  while (*argument && !(*argument == '-'))
  {
    *(first_arg++) = LOWER(*argument);
    argument++;
  }

  *first_arg = '\0';

  return argument;
}

char *one_arg_dots(char *argument, char *first_arg)
{
  skip_dot(&argument);

  while (*argument && !(*argument == '.'))
  {
    *(first_arg++) = LOWER(*argument);
    argument++;
  }

  *first_arg = '\0';

  return argument;
}


/*
 * one_word is like one_argument, except that words in quotes ("") are
 * considered one word.
 */
char *one_word(char *argument, char *first_arg)
{
  char *begin = first_arg;

  do {
    skip_spaces(&argument);

    first_arg = begin;

    if (*argument == '\"') {
      argument++;
      while (*argument && *argument != '\"') {
        *(first_arg++) = LOWER(*argument);
        argument++;
      }
      argument++;
    } else {
      while (*argument && !isspace(*argument)) {
        *(first_arg++) = LOWER(*argument);
        argument++;
      }
    }

    *first_arg = '\0';
  } while (fill_word(begin));

  return argument;
}


/* same as one_argument except that it doesn't ignore fill words */
char *any_one_arg(char *argument, char *first_arg)
{
  skip_spaces(&argument);

  while (*argument && !isspace(*argument)) {
    *(first_arg++) = LOWER(*argument);
    argument++;
  }

  *first_arg = '\0';

  return argument;
}


/*
 * Same as one_argument except that it takes two args and returns the rest;
 * ignores fill words
 */
char *two_arguments(char *argument, char *first_arg, char *second_arg)
{
  return one_argument(one_argument(argument, first_arg), second_arg); /* :-) */
}



/*
 * determine if a given string is an abbreviation of another
 * (now works symmetrically -- JE 7/25/94)
 *
 * that was dumb.  it shouldn't be symmetrical.  JE 5/1/95
 * 
 * returnss 1 if arg1 is an abbreviation of arg2
 */
int is_abbrev(char *arg1, char *arg2)
{
  if (!*arg1)
    return 0;

  for (; *arg1 && *arg2; arg1++, arg2++)
    if (LOWER(*arg1) != LOWER(*arg2))
      return 0;

  if (!*arg1)
    return 1;
  else
    return 0;
}


/*  PDH  2/25/99 - god selection to string  */
char *  godSelected(struct char_data *ch)
{
  if (!IS_NPC(ch) && (GET_CLASS(ch) == CLASS_CLERIC ||
                     (GET_CLASS(ch) == CLASS_PALADIN)))
  {
    switch(GET_GODSELECT(ch)) {

    case GOD_PALADINE:    return "Paladine";
    case GOD_KIRIJOLITH:  return "Kiri-Jolith";
    case GOD_HABBAKUK:    return "Habbakuk";
    case GOD_MISHAKAL:    return "Mishakal";
    case GOD_BRANCHALA:   return "Branchala";
    case GOD_SOLINARI:    return "Solinari - BUG";
    case GOD_MAJERE:      return "Majere";

    case GOD_GILEAN:      return "Gilean";
    case GOD_REORX:       return "Reorx";
    case GOD_LUNITARI:    return "Lunitari - BUG";
    case GOD_CHISLEV:     return "Chislev";
    case GOD_SIRRION:     return "Sirrion";
    case GOD_ZIVILYN:     return "Zivilyn";
    case GOD_SHINARE:     return "Shinare";

    case GOD_TAKHISIS:    return "Takhisis";
    case GOD_SARGONNAS:   return "Sargonnas";
    case GOD_NUITARI:     return "Nuitari - BUG";
    case GOD_MORGION:     return "Morgion";
    case GOD_CHEMOSH:     return "Chemosh";
    case GOD_ZEBOIM:      return "Zeboim";
    case GOD_HIDDUKEL:    return "Hiddukel";

    default:
      log("SYSERR: godSelected(): bad God selection");
      return "-ERROR-";
      break;
    }

  } else {
    return "n/a";
  }
}

char * ability_string(struct descriptor_data *d);
extern int raceAbils[NUM_RACES][6][2];
void perform_init_stats(struct descriptor_data *d)
{
  GET_MANA(d->character) = 64;
  d->character->real_abils.str  =
    MAX(raceAbils[(int)GET_RACE(d->character)][0][0],
        class_abil_mins[(int)GET_CLASS(d->character)][0]);
  d->character->real_abils.str_add  = 0;
  d->character->real_abils.intel  =
    MAX(raceAbils[(int)GET_RACE(d->character)][1][0],
        class_abil_mins[(int)GET_CLASS(d->character)][1]);
  d->character->real_abils.wis  =
    MAX(raceAbils[(int)GET_RACE(d->character)][2][0],
        class_abil_mins[(int)GET_CLASS(d->character)][2]);
  d->character->real_abils.dex  =
    MAX(raceAbils[(int)GET_RACE(d->character)][3][0],
        class_abil_mins[(int)GET_CLASS(d->character)][3]);
  d->character->real_abils.con  =
    MAX(raceAbils[(int)GET_RACE(d->character)][4][0],
        class_abil_mins[(int)GET_CLASS(d->character)][4]);
  d->character->real_abils.cha  =
    MAX(raceAbils[(int)GET_RACE(d->character)][5][0],
        class_abil_mins[(int)GET_CLASS(d->character)][5]);

  GET_MANA(d->character) += ( 3 - d->character->real_abils.str) +
      ( 3 - d->character->real_abils.intel) +
      ( 3 - d->character->real_abils.wis) +
      ( 3 - d->character->real_abils.dex) +
      ( 3 - d->character->real_abils.con) +
      ( 3 - d->character->real_abils.cha);

  SEND_TO_Q("\r\nYour stats have been pre-set to the minimum required for both the class and\r\n", d);
  SEND_TO_Q("race you have selected.\r\n\r\n", d);
  SEND_TO_Q(ability_string(d), d);
}

void display_god_menu(struct descriptor_data *d)
{
  char  menu[1000];

  strcpy(menu, "\r\nSelect your God:\r\n");
  strcat(menu, "     Good             Neut             Evil\r\n");

  strcat(menu, " a) Paladine      h) Gilean        o) Takhisis\r\n");
  strcat(menu, " b) Kiri-Jolith   i) Reorx         p) Sargonnas\r\n");
  strcat(menu, " c) Habbakuk      k) Chislev       r) Morgion\r\n");
  strcat(menu, " d) Mishakal      l) Sirrion       s) Chemosh\r\n");
  strcat(menu, " e) Branchala     m) Zivilyn       t) Zeboim\r\n");
  strcat(menu, " g) Majere        n) Shinare       u) Hiddukel\r\n\r\n");

  /* Chopped open spaces for moons just to look nice, Bran =)  -gil */

  SEND_TO_Q(menu, d);
}


void display_classes(struct descriptor_data *d)
{
  int x=0;
  send_to_char("Class selection menu - Please choose a class\r\n",
	       d->character);
  send_to_char("-------------------------------------------\r\n",
	       d->character);
 
  for (x=0; x<NUM_CLASSES; x++) {
    if ( class_ok_race[(int) GET_RACE(d->character)][x] ) {
      /* (class_ok_stats(d->character, x)) ) { */
      send_to_char(class_display[x], d->character);
    }
  }

  send_to_char("\r\n", d->character);
  send_to_char(" [Z] Select New Race\r\n", d->character);

  send_to_char("--------------------------------------------", d->character);
  send_to_char("\r\nClass: ", d->character);
}


/* return first space-delimited token in arg1; remainder of string in arg2 */
void half_chop(char *string, char *arg1, char *arg2)
{
  char *temp;

  temp = any_one_arg(string, arg1);
  skip_spaces(&temp);
  strcpy(arg2, temp);
}



/* Used in specprocs, mostly.  (Exactly) matches "command" to cmd number */
int find_command(char *command)
{
  int cmd;

  for (cmd = 0; *cmd_info[cmd].command != '\n'; cmd++)
    if (!strcmp(cmd_info[cmd].command, command))
      return cmd;

  return -1;
}


int special(struct char_data *ch, int cmd, char *arg)
{
  register struct obj_data *i;
  register struct char_data *k;
  int j;

  /* special in room? */
  if (GET_ROOM_SPEC(ch->in_room) != NULL)
    if (GET_ROOM_SPEC(ch->in_room) (ch, world + ch->in_room, cmd, arg))
      return 1;

  /* special in equipment list? */
  for (j = 0; j < NUM_WEARS; j++)
    if (GET_EQ(ch, j) && GET_OBJ_SPEC(GET_EQ(ch, j)) != NULL)
      if (GET_OBJ_SPEC(GET_EQ(ch, j)) (ch, GET_EQ(ch, j), cmd, arg))
	return 1;

  /* special in inventory? */
  for (i = ch->carrying; i; i = i->next_content)
    if (GET_OBJ_SPEC(i) != NULL)
      if (GET_OBJ_SPEC(i) (ch, i, cmd, arg))
	return 1;

  /* special in mobile present? */
  for (k = world[ch->in_room].people; k; k = k->next_in_room)
    if (GET_MOB_SPEC(k) != NULL)
      if (GET_MOB_SPEC(k) (ch, k, cmd, arg))
	return 1;

  /* special in object present? */
  for (i = world[ch->in_room].contents; i; i = i->next_content)
    if (GET_OBJ_SPEC(i) != NULL)
      if (GET_OBJ_SPEC(i) (ch, i, cmd, arg))
	return 1;

  return 0;
}



/* *************************************************************************
*  Stuff for controlling the non-playing sockets (get name, pwd etc)       *
************************************************************************* */


/* locate entry in p_table with entry->name == name. -1 mrks failed search */
int find_name(char *name)
{
  int i;
  int found = 0;

  for (i = 0; i <= top_of_p_table; i++) {
    if (!str_cmp((player_table + i)->name, name)) {
	  if (!found && !strcmp(name, "takhisis"))
	    found++;
	  else
  	    return i;
	}
  }

  return -1;
}


int _parse_name(char *arg, char *name, int maxSize)
{
  int i;

  /* skip whitespaces */
  for (; isspace(*arg); arg++);

  memset(name, (char) NULL, maxSize);

  for (i = 0; (*name = *arg); arg++, i++, name++) {

    if (strlen(name) == (maxSize - 1))
    {
      return 1;
    }

    if ( (!isalpha(*arg)) && (*arg != '-') ) {
      return 1;
    }

    if ( (*arg == '-') && (i == 0) ) {
      /*  can't start with anything but a-z  */
      return 1;
    }
  }

  if ( ! i ) {
    return 1;
  }

  return 0;
}


#define RECON		1
#define USURP		2
#define UNSWITCH	3

int perform_dupe_check(struct descriptor_data *d)
{
  struct descriptor_data *k, *next_k;
  struct char_data *target = NULL, *ch, *next_ch;
  int mode = 0;

  int id = GET_IDNUM(d->character);

  /*
   * Now that this descriptor has successfully logged in, disconnect all
   * other descriptors controlling a character with the same ID number.
   */

  for (k = descriptor_list; k; k = next_k) {
    next_k = k->next;

    if (k == d)
      continue;

    if (k->original && (GET_IDNUM(k->original) == id)) {    /* switched char */
      SEND_TO_Q("\r\nMultiple login detected -- disconnecting.\r\n", k);
      STATE(k) = CON_CLOSE;
      if (!target) {
	target = k->original;
	mode = UNSWITCH;
      }
      if (k->character)
	k->character->desc = NULL;
      k->character = NULL;
      k->original = NULL;
    } else if (k->character && (GET_IDNUM(k->character) == id)) {
      if (!target && STATE(k) == CON_PLAYING) {
	SEND_TO_Q("\r\nThis body has been usurped!\r\n", k);
	target = k->character;
	mode = USURP;
      }
      k->character->desc = NULL;
      k->character = NULL;
      k->original = NULL;
      SEND_TO_Q("\r\nMultiple login detected -- disconnecting.\r\n", k);
      STATE(k) = CON_CLOSE;
    }
  }

 /*
  * now, go through the character list, deleting all characters that
  * are not already marked for deletion from the above step (i.e., in the
  * CON_HANGUP state), and have not already been selected as a target for
  * switching into.  In addition, if we haven't already found a target,
  * choose one if one is available (while still deleting the other
  * duplicates, though theoretically none should be able to exist).
  */

  for (ch = character_list; ch; ch = next_ch) {
    next_ch = ch->next;

    if (IS_NPC(ch))
      continue;
    if (GET_IDNUM(ch) != id)
      continue;

    /* ignore chars with descriptors (already handled by above step) */
    if (ch->desc)
      continue;

    /* don't extract the target char we've found one already */
    if (ch == target)
      continue;

    /* we don't already have a target and found a candidate for switching */
    if (!target) {
      target = ch;
      mode = RECON;
      continue;
    }

    /* we've found a duplicate - blow him away, dumping his eq in limbo. */
    if (ch->in_room != NOWHERE)
      char_from_room(ch);
    char_to_room(ch, 1);
    extract_char(ch);
  }

  /* no target for swicthing into was found - allow login to continue */
  if (!target)
    return 0;

  /* Okay, we've found a target.  Connect d to target. */
  free_char(d->character); /* get rid of the old char */
  d->character = target;
  d->character->desc = d;
  d->original = NULL;
  d->character->char_specials.timer = 0;
  REMOVE_BIT(PLR_FLAGS(d->character), PLR_MAILING | PLR_WRITING);
  /* REMOVE_BIT(AFF_FLAGS(d->character), AFF_GROUP); */
  STATE(d) = CON_PLAYING;

  switch (mode) {
  case RECON:
    SEND_TO_Q("Reconnecting.\r\n", d);
    act("$n has reconnected.", TRUE, d->character, 0, 0, TO_ROOM);
    GET_NAME(d->character, chname);
    sprintf(buf, "%s [%s] has reconnected.", chname, d->host);
    FREE_NAME(chname);
    mudlog(buf, NRM, MAX(LVL_IMMORT, GET_INVIS_LEV(d->character)), TRUE);
    break;
  case USURP:
    SEND_TO_Q("You take over your own body, already in use.\r\n", d);
    act("$n begins to convulse, then jerks to a stop, deathly still...\r\n"
        "$n suddenly regains control, returning to life.",
	TRUE, d->character, 0, 0, TO_ROOM);
    GET_NAME(d->character, chname);
    sprintf(buf, "%s has re-logged in ... disconnecting old socket.", chname);
    FREE_NAME(chname);
    mudlog(buf, NRM, MAX(LVL_IMMORT, GET_INVIS_LEV(d->character)), TRUE);
    break;
  case UNSWITCH:
    SEND_TO_Q("Reconnecting to unswitched char.", d);
    GET_NAME(d->character, chname);
    sprintf(buf, "%s [%s] has reconnected.", chname, d->host);
    FREE_NAME(chname);
    immcharlog(buf);
    break;
  }

  return 1;
}



/*
 *  PDH 8/4/97
 *  for choosing character abilities
 */

#define ABIL_DONE         1
#define ABIL_BADRACE      2
#define ABIL_NOTENUFPTS   3
#define ABIL_CONTINUE     4
#define ABIL_NEWRACE      5
#define ABIL_BADCLASS     6

extern int raceBonus[NUM_RACES][6];


int valid_race_ability(struct char_data * ch)
{
  int race = GET_RACE(ch);

  if ( (ch->real_abils.str >= raceAbils[race][0][0]) &&
       (ch->real_abils.str <= raceAbils[race][0][1]) &&

       (ch->real_abils.intel >= raceAbils[race][1][0]) &&
       (ch->real_abils.intel <= raceAbils[race][1][1]) &&

       (ch->real_abils.wis >= raceAbils[race][2][0]) &&
       (ch->real_abils.wis <= raceAbils[race][2][1]) &&

       (ch->real_abils.dex >= raceAbils[race][3][0]) &&
       (ch->real_abils.dex <= raceAbils[race][3][1]) &&

       (ch->real_abils.con >= raceAbils[race][4][0]) &&
       (ch->real_abils.con <= raceAbils[race][4][1]) &&

       (ch->real_abils.cha >= raceAbils[race][5][0]) &&
       (ch->real_abils.cha <= raceAbils[race][5][1]) ) {

    return 1;
  }

  return 0;
}


int ptsNeeded(int current, int change)
{
  int mult = 1;
  int pts  = 0, a;

  if ( change < 0 ) {
    mult = -1;
    current += change;
    change *= -1;
  }

  for(a=0; (a<change) && (current<20); a++) {
    switch(current) {

    case 3:  case 4:  case 5:  case 6:  case 7:
    case 8:  case 9:  case 10: case 11: case 12:
    case 13:  case 14:
      pts += 1;
      break;
    case 15:  case 16:
      pts += 2;
      break;
    case 17:
      pts += 3;
      break;
    case 18:  case 19:
      pts += 4;
      break;

    default:
      log("SYSERR: unexpected current value in ptsNeeded()");
      pts += 100;
      break;
    }

    current++;
  }

  return ( pts * mult );
}


int abilityResults(struct descriptor_data *d, char * arg)
{
  struct char_data * ch = d->character;
  char   abil[20], value[20];
  int    val, chPts = GET_MANA(ch);
  int    race = GET_RACE(ch);
  extern int class_ok_stats(struct char_data *ch, int class);


  two_arguments(arg, abil, value);
  val = atoi(value);


  if ( str_cmp(abil, "done") == 0 ) {
    /*  PDH  1/11/99 - check for valid race limits  */
    if ( valid_race_ability(ch) ) {
      if ( class_ok_stats(ch, (int)GET_CLASS(ch)) ) {
	return ABIL_DONE;
      } else {
	return ABIL_BADCLASS;
      }
    } else {
      return ABIL_BADRACE;
    }

  } else if ( str_cmp(abil, "return") == 0 ) {
    return ABIL_NEWRACE;

  } else if ( str_cmp(abil, "str") == 0 ) {
    int tmp = ch->real_abils.str + val;
    if ( (tmp < raceAbils[race][0][0]) || (tmp > raceAbils[race][0][1]) ) {
      return ABIL_BADRACE;
    }

    if ( ptsNeeded(ch->real_abils.str, val) > chPts ) {
      return ABIL_NOTENUFPTS;
    } else {
      /*  affect the change  */
      GET_MANA(ch) -= ptsNeeded(ch->real_abils.str, val);
      ch->real_abils.str += val;
    }

  } else if ( str_cmp(abil, "int") == 0 ) {
    int tmp = ch->real_abils.intel + val;
    if ( (tmp < raceAbils[race][1][0]) || (tmp > raceAbils[race][1][1]) ) {
      return ABIL_BADRACE;
    }

    if ( ptsNeeded(ch->real_abils.intel, val) > chPts ) {
      return ABIL_NOTENUFPTS;
    } else {
      /*  affect the change  */
      GET_MANA(ch) -= ptsNeeded(ch->real_abils.intel, val);
      ch->real_abils.intel += val;
    }

  } else if ( str_cmp(abil, "wis") == 0 ) {
    int tmp = ch->real_abils.wis + val;
    if ( (tmp < raceAbils[race][2][0]) || (tmp > raceAbils[race][2][1]) ) {
      return ABIL_BADRACE;
    }

    if ( ptsNeeded(ch->real_abils.wis, val) > chPts ) {
      return ABIL_NOTENUFPTS;
    } else {
      /*  affect the change  */
      GET_MANA(ch) -= ptsNeeded(ch->real_abils.wis, val);
      ch->real_abils.wis += val;
    }

  } else if ( str_cmp(abil, "dex") == 0 ) {
    int tmp = ch->real_abils.dex + val;
    if ( (tmp < raceAbils[race][3][0]) || (tmp > raceAbils[race][3][1]) ) {
      return ABIL_BADRACE;
    }

    if ( ptsNeeded(ch->real_abils.dex, val) > chPts ) {
      return ABIL_NOTENUFPTS;
    } else {
      /*  affect the change  */
      GET_MANA(ch) -= ptsNeeded(ch->real_abils.dex, val);
      ch->real_abils.dex += val;
    }

  } else if ( str_cmp(abil, "con") == 0 ) {
    int tmp = ch->real_abils.con + val;
    if ( (tmp < raceAbils[race][4][0]) || (tmp > raceAbils[race][4][1]) ) {
      return ABIL_BADRACE;
    }

    if ( ptsNeeded(ch->real_abils.con, val) > chPts ) {
      return ABIL_NOTENUFPTS;
    } else {
      /*  affect the change  */
      GET_MANA(ch) -= ptsNeeded(ch->real_abils.con, val);
      ch->real_abils.con += val;
    }

  } else if ( str_cmp(abil, "cha") == 0 ) {
    int tmp = ch->real_abils.cha + val;
    if ( (tmp < raceAbils[race][5][0]) || (tmp > raceAbils[race][5][1]) ) {
      return ABIL_BADRACE;
    }

    if ( ptsNeeded(ch->real_abils.cha, val) > chPts ) {
      return ABIL_NOTENUFPTS;
    } else {
      /*  affect the change  */
      GET_MANA(ch) -= ptsNeeded(ch->real_abils.cha, val);
      ch->real_abils.cha += val;
    }

  } else {
    SEND_TO_Q("\r\nUnknown option...\r\n\r\n", d);
  }

  return ABIL_CONTINUE;
}


char ability_menu[500];

char * ability_string(struct descriptor_data *d)
{
  struct char_data * ch = d->character;
  int  race = GET_RACE(ch);
  int class = GET_CLASS(ch);


  sprintf(ability_menu, "\r\n\r\nPlease select your ability scores.\r\n");


  sprintf(ability_menu,
	  "%s\r\nRace: %-15s    Class: %-10s     Points Remaining: %3d\r\n",
	  ability_menu, pc_race_types[race], pc_class_types[class],
	  GET_MANA(ch));

  sprintf(ability_menu,
	  "%s[Str]ength : %2d [%2d-%2d]    [Int]elligence: %2d [%2d-%2d]    [Wis]dom  : %2d [%2d-%2d]  \r\n",
	  ability_menu,
	  ch->real_abils.str, raceAbils[race][0][0], raceAbils[race][0][1],
	  ch->real_abils.intel, raceAbils[race][1][0], raceAbils[race][1][1],
	  ch->real_abils.wis, raceAbils[race][2][0], raceAbils[race][2][1]);

  sprintf(ability_menu,
	  "%s[Dex]terity: %2d [%2d-%2d]    [Con]stitution: %2d [%2d-%2d]    [Cha]risma: %2d [%2d-%2d]  \r\n",
	  ability_menu,
	  ch->real_abils.dex, raceAbils[race][3][0], raceAbils[race][3][1],
	  ch->real_abils.con, raceAbils[race][4][0], raceAbils[race][4][1],
	  ch->real_abils.cha, raceAbils[race][5][0], raceAbils[race][5][1]);

  sprintf(ability_menu,
	  "%s[Return]   : To return to the race selection menu\r\n",
	  ability_menu);

  sprintf(ability_menu,
	  "%s[Done]     : When complete\r\n",
	  ability_menu);

  sprintf(ability_menu,
	  "%s\r\nSelect an attribute, followed by a +/- and a number.\r\n",
	  ability_menu);

  sprintf(ability_menu,
	  "%sFor example:  Str +3       or       Wis -2\r\n",
	  ability_menu);

  sprintf(ability_menu,
	  "%sEnter --> ",
	  ability_menu);

  return ability_menu;
}


/*  PDH 12/29/98
 *  first gets the original name only, then
 *  adds to the character's char_data->player.name
 *  using the needed free() and CREATE()
 *  also adds the char's race name in at the end
 */
void add_to_char_name(struct char_data * ch, char * addName)
{
  char tmpName[MAX_NAME_LENGTH];
  int  amtToCopy = 0;

  strcpy(tmpName, "");

  if ( ch->player.name ) {
    GET_NAME(ch, chname);
    strcpy(tmpName, chname); 
    FREE_NAME(chname);
    free(ch->player.name);
    ch->player.name = NULL;
  }

  strcat(tmpName, " ");

  amtToCopy = MAX_NAME_LENGTH - strlen(tmpName) - 2;
  strncat(tmpName, addName, amtToCopy);
  strcat(tmpName, " ");

  amtToCopy = MAX_NAME_LENGTH - strlen(tmpName) - 2;
  strncat(tmpName, pc_race_types[GET_RACE(ch)], amtToCopy);
  CREATE(ch->player.name, char, strlen(tmpName) + 1);
  strcpy(ch->player.name, CAP(tmpName));

/*
  {
    char errStr[200];
    sprintf(errStr, "add_to_char_name: name:%s:  add:%s:",
	    ch->player.name, addName);
    sprintf(errStr, "%s\r\n   NEW:%s:",
	    errStr, ch->player.name);
    log(errStr);
  }
*/
}


/* deal with newcomers and other non-playing sockets */
/*  PDH 12/29/98
 *  new char gen format:
 *
 *  REMOVED ** ansi question
 *  name
 *  name confirm
 *  password
 *  password confirm
 *  gender
 *  race
 *  class
 *  abilities
 *  long descr
 *  short descr
 *  alias list
 *  static alignment
 *  save
 *  MOTD
 *  menu
 */
void nanny(struct descriptor_data *d, char *arg)
{
  int count = 0;
  char buf[MAX_INPUT_LENGTH], upChar;
  int player_i, load_result, abilRet;
  char tmp_name[MAX_INPUT_LENGTH];
  struct char_file_u tmp_store;
  /* extern const char *class_menu; */
  extern const char *race_menu;
  extern int max_bad_pws;
  int color = 0;
  int invalid_align[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
  //room_vnum load_room;


  int load_char(char *name, struct char_file_u *char_element);
  /*int parse_class(char arg);*/
  int parse_class(struct char_data *ch, char arg);
  int parse_race(char arg);
  
  //SEND_TO_Q("SKIPPING SPACES IN ARGUMENT\r\n", d);
  skip_spaces(&arg);

#ifdef GMB_DEBUG
  //SEND_TO_Q("GMB_DEBUG BEGUN\r\n", d);
  if (d->character == NULL) {
     CREATE(d->character, struct char_data, 1);
     clear_char(d->character);
     CREATE(d->character->player_specials, struct player_special_data,1);
     d->character->desc = d;
   }
#endif

  switch (STATE(d)) {

  /*. OLC states .*/
  case CON_OEDIT:
    oedit_parse(d, arg);
    break;
  case CON_REDIT:
    redit_parse(d, arg);
    break;
  case CON_ZEDIT:
    zedit_parse(d, arg);
    break;
  case CON_MEDIT:
    medit_parse(d, arg);
    break;
  case CON_SEDIT:
    sedit_parse(d, arg);
    break;
  case CON_TRIGEDIT:
    trigedit_parse(d, arg);
    break;
  /*. End of OLC states .*/

  case CON_GET_NAME:		/* wait for input of name */
    //SEND_TO_Q("CON_GET_NAME SEQUENCE BEGUN.\r\n", d);
    if (d->character == NULL) {
      CREATE(d->character, struct char_data, 1);
      clear_char(d->character);
      CREATE(d->character->player_specials, struct player_special_data, 1);
      d->character->desc = d;
    }

    if (!*arg) {
      STATE(d) = CON_CLOSE;
    } else {
	/*
	  if (!strcmp(arg, "takhisis")) {
	    if (!Valid_Name(arg)) {
	      SEND_TO_Q("&RInvalid name, please try another&W.&n\r\n", d);
	      SEND_TO_Q("&YName&W:&n ", d);
	      return;
	    }
	    CREATE(d->character->player.name, char, strlen(arg) + 1);
	    strcpy(d->character->player.name, CAP(arg));
	    sprintf(buf, "&YDid I get that right&W,&C %s &W(&MY&W/&MN&W)?&n",
		        arg);
	    SEND_TO_Q(buf, d);
	    STATE(d) = CON_NAME_CNFRM;
		break;	  
	  }
	  */
	  //SEND_TO_Q("PARSE NAME CHECK.\r\n", d);
      if ((_parse_name(arg, tmp_name, MAX_INPUT_LENGTH)) ||
          strlen(tmp_name) < 3 ||
		  strlen(tmp_name) > MAX_NAME_LENGTH || !Valid_Name(tmp_name) ||
		  fill_word(strcpy(buf, tmp_name)) || reserved_word(buf)) 
	  {
		SEND_TO_Q("&RInvalid name, please try another&W.&n\r\n&YName&W:&n ", d);
		return;
      }
     
 
	  //SEND_TO_Q("LOAD CHARACTER\r\n", d);
      if ((player_i = load_char(tmp_name, &tmp_store)) > -1 //&&
          /* load_sql_player(d->character)*/) 
	  {
        if (PRF_FLAGGED(d->character, PRF_COLOR_1)) {
          color = 1;
	    }

		//SEND_TO_Q("STORE TO CHAR\r\n", d);
	    store_to_char(&tmp_store, d->character);
        GET_PFILEPOS(d->character) = player_i;

		//SEND_TO_Q("IS PLAYER SET AS DELETED?\r\n", d);
	    if (PLR_FLAGGED(d->character, PLR_DELETED)) 
		{
//		  SEND_TO_Q("YES, PLAYER IS SET AS DELETED.\r\n", d);
		  if (!strcmp(tmp_name, "takhisis")) {
		    SEND_TO_Q("&WWelcome Back Takhisis, let's try to get you back in business!&n\r\n", d);
		  }
		  else 
	        free_char(d->character);
          if (!Valid_Name(tmp_name)) {
	        SEND_TO_Q("&RInvalid name, please try another&W.&n\r\n", d);
	        SEND_TO_Q("&YName&W:&n ", d);
            return;
          }
	      CREATE(d->character, struct char_data, 1);
	      clear_char(d->character);
	      CREATE(d->character->player_specials, struct player_special_data, 1);
	      d->character->desc = d;
	      CREATE(d->character->player.name, char, strlen(tmp_name) + 1);
	      strcpy(d->character->player.name, CAP(tmp_name));
	      GET_PFILEPOS(d->character) = player_i;
    	  sprintf(buf, "&YDid I get that right&W,&C %s &W(&MY&W/&MN&W)?&n", tmp_name);
	      SEND_TO_Q(buf, d);
	      STATE(d) = CON_NAME_CNFRM;
	    } 
		else 
		{
		  //SEND_TO_Q("NO, PLAYER IS NOT SET AS DELETED.\r\n", d);
	      /* undo it just in case they are set */
	      REMOVE_BIT(PLR_FLAGS(d->character), PLR_WRITING | PLR_MAILING);
          REMOVE_BIT(AFF_FLAGS(d->character), AFF_GROUP);
          //max_group_exp_mult(d->character);
          GET_EXP_MULT(d->character) = 0;
          REMOVE_BIT(PRF_FLAGS(d->character), PRF_NOTSELF);
          //REMOVE_BIT(PRF_FLAGS(d->character), PRF_DISGUISE);
          REMOVE_BIT(PLR_FLAGS(d->character), PLR_FISHING);
          REMOVE_BIT(PLR_FLAGS(d->character), PLR_FISH_ON);
          REMOVE_BIT(PLR_FLAGS(d->character), PLR_DIGGING);
          REMOVE_BIT(PLR_FLAGS(d->character), PLR_DIG_ON);
          REMOVE_BIT(PLR_FLAGS(d->character), PLR_FIRE_ON);
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
          if (AFF_FLAGGED(d->character, AFF_POLYMORPH))
            affect_from_char_II(d->character, SPELL_POLYMORPH, SPELL_POLYMORPH, 2);
	      SEND_TO_Q("&YPassword&W:&n ", d);
	      echo_off(d);
	      d->idle_tics = 0;
	      STATE(d) = CON_PASSWORD;
	    }
      } 
	  else 
	  {
	    //SEND_TO_Q("PLAYER WAS NOT FOUND.\r\n", d);
	    /* player unknown -- make new character */

	    if (!Valid_Name(tmp_name)) {
	      SEND_TO_Q("&RInvalid name, please try another&W.&n\r\n", d);
	      SEND_TO_Q("&YName&W:&n ", d);
	      return;
	    }
	    CREATE(d->character->player.name, char, strlen(tmp_name) + 1);
	    strcpy(d->character->player.name, CAP(tmp_name));
	    sprintf(buf, "&YDid I get that right&W,&C %s &W(&MY&W/&MN&W)?&n",
		        tmp_name);
	    SEND_TO_Q(buf, d);
	    STATE(d) = CON_NAME_CNFRM;
      }
    }
	//SEND_TO_Q("GET CHARACTER NAME PROCESS COMPLETED SUCCESSFULLY.\r\n", d);
    break;

  case CON_NAME_CNFRM:		/* wait for conf. of new name    */
    if (UPPER(*arg) == 'Y') {
      if (isbanned(d->host) == BAN_NEW || isbanned(d->host) == BAN_ALL) {
        GET_NAME(d->character, chname);
	sprintf(buf, "Request for new char %s denied from [%s] (siteban)",
		chname, d->host);
        FREE_NAME(chname);
	mudlog(buf, NRM, LVL_GOD, TRUE);
	SEND_TO_Q("Sorry, new characters are not allowed from your site!\r\n",
		  d);
	STATE(d) = CON_CLOSE;
	return;
      }

      if (circle_restrict) {
	SEND_TO_Q("Sorry, new players can't be created at the moment.\r\n", d);
        GET_NAME(d->character, chname); 
	sprintf(buf, "Request for new char %s denied from [%s] (wizlock)",
	  chname, d->host);
        FREE_NAME(chname);
	mudlog(buf, NRM, LVL_GOD, TRUE);
	STATE(d) = CON_CLOSE;
	return;
      }

      SEND_TO_Q("&RN&Be&Yw &Cc&Mh&Wa&Gr&Ra&Yc&Mt&Be&Cr&W.&n\r\n", d);
      GET_NAME(d->character, chname);
      sprintf(buf, "&YGive me a password for&R %s&W: &n", chname);
      FREE_NAME(chname);
      SEND_TO_Q(buf, d);
      echo_off(d);
      STATE(d) = CON_NEWPASSWD;
    } else if (*arg == 'n' || *arg == 'N') {
      SEND_TO_Q("&ROkay, what IS it, then&W?&n ", d);
      free(d->character->player.name);
      d->character->player.name = NULL;
      STATE(d) = CON_GET_NAME;
    } else {
      SEND_TO_Q("&YPlease type Yes or No&W: &n", d);
    }
    break;

  case CON_PASSWORD:		/* get pwd for known player      */
    /*
     * To really prevent duping correctly, the player's record should
     * be reloaded from disk at this point (after the password has been
     * typed).  However I'm afraid that trying to load a character over
     * an already loaded character is going to cause some problem down the
     * road that I can't see at the moment.  So to compensate, I'm going to
     * (1) add a 15 or 20-second time limit for entering a password, and (2)
     * re-add the code to cut off duplicates when a player quits.  JE 6 Feb 96

     *  Yeah, but that doesn't help much, because the duping still happens.
     *  We just unload the character and reload it again - Soli, 9/7/99
     */

    echo_on(d);    /* turn echo back on */

    /* New echo_on() eats the return on telnet. Extra space better than none. */
    SEND_TO_Q("\r\n", d);

    if (!*arg) {
      STATE(d) = CON_CLOSE;
    } else {
      if (strncmp(CRYPT(arg, GET_PASSWD(d->character)), GET_PASSWD(d->character), MAX_PWD_LENGTH)) {
        GET_NAME(d->character, chname);
	sprintf(buf, "Bad PW: %s [%s]", chname, d->host);
        FREE_NAME(chname);
	mudlog(buf, BRF, LVL_GOD, TRUE);
	GET_BAD_PWS(d->character)++;
	save_char(d->character, NOWHERE);
	if (++(d->bad_pws) >= max_bad_pws) {	/* 3 strikes and you're out. */
	  SEND_TO_Q("&RWrong password&W... &Ydisconnecting&W.&n\r\n", d);
	  STATE(d) = CON_CLOSE;
	} else {
	  SEND_TO_Q("&RWrong password&W.\r\n&YPassword&W:&n ", d);
	  echo_off(d);
	}
	return;
      }

      /* Password was correct. */
      load_result = GET_BAD_PWS(d->character);
      GET_BAD_PWS(d->character) = 0;
      d->bad_pws = 0;

      if (isbanned(d->host) == BAN_SELECT &&
	  !PLR_FLAGGED(d->character, PLR_SITEOK)) {
	SEND_TO_Q("Sorry, this char has not been cleared for login from your site!\r\n", d);
	STATE(d) = CON_CLOSE;
        GET_NAME(d->character, chname);
	sprintf(buf, "Connection attempt for %s denied from %s",
		chname, d->host);
        FREE_NAME(chname);
	mudlog(buf, NRM, LVL_GOD, TRUE);
	return;
      }

      if (GET_LEVEL(d->character) < circle_restrict) {
	SEND_TO_Q("The game is temporarily restricted.. try again later.\r\n", d);
	STATE(d) = CON_CLOSE;
        GET_NAME(d->character, chname);
	sprintf(buf, "Request for login denied for %s [%s] (wizlock)",
		chname, d->host);
        FREE_NAME(chname);
	mudlog(buf, NRM, LVL_GOD, TRUE);
	return;
      }

      /* check and make sure no other copies of this player are logged in */
      if (perform_dupe_check(d)) {
	return;
      }

#ifdef GMB_DEBUG
      /*  That's nice.  Meanwhile, if they already rented in the meantime,
          they just got some free money.  Let's fix this right now.  */

      strcpy(tmp_name, strtok(d->character->player.name, " "));

      free_char(d->character);
      CREATE(d->character, struct char_data, 1);
      clear_char(d->character);
      CREATE(d->character->player_specials, struct player_special_data, 1);
      d->character->desc = d;

      player_i = load_char(tmp_name, &tmp_store);
      if (player_i <= -1)
      {
        sprintf(buf, "SYSERR: Error in load_char in nanny()!");
        mudlog(buf, BRF, LVL_DEMIGOD, TRUE);
        send_to_char("An error has occured.\r\n", d->character);
        close_socket(d);
        return;
      }
      store_to_char(&tmp_store, d->character);
      GET_PFILEPOS(d->character) = player_i;
#endif

      if (GET_LEVEL(d->character) >= LVL_IMMORT) {
	SEND_TO_Q(imotd, d);
      } else {
	SEND_TO_Q(motd, d);
      }

      GET_NAME(d->character, chname);
      sprintf(buf, "%s [%s] has connected.", chname, d->host);
      FREE_NAME(chname);
      immcharlog(buf);

      if (load_result) {
	sprintf(buf, "\r\n\r\n\007\007\007"
		"%s%d LOGIN FAILURE%s SINCE LAST SUCCESSFUL LOGIN.%s\r\n",
		CCRED(d->character, C_SPR), load_result,
		(load_result > 1) ? "S" : "", CCNRM(d->character, C_SPR));
	SEND_TO_Q(buf, d);
	GET_BAD_PWS(d->character) = 0;
      }

      SEND_TO_Q("\r\n\n&W*** &YPRESS RETURN&W:&n ", d);
      STATE(d) = CON_RMOTD;
    }
    break;

  case CON_NEWPASSWD:
  case CON_CHPWD_GETNEW:
  {
    int flag=0;
    GET_NAME(d->character, chname);
    if (!*arg || strlen(arg) > MAX_PWD_LENGTH || strlen(arg) < 3 ||
	!str_cmp(arg, chname)) {
      flag=1;
      SEND_TO_Q("\r\n&RIllegal &Ypassword&W.&n\r\n", d);
      SEND_TO_Q("&YPassword&W:&n ", d);
    }
    FREE_NAME(chname);
    if (flag == 1) return;

    GET_NAME(d->character, chname);
    strncpy(GET_PASSWD(d->character), CRYPT(arg, chname), MAX_PWD_LENGTH);
    FREE_NAME(chname);
    *(GET_PASSWD(d->character) + MAX_PWD_LENGTH) = '\0';

    SEND_TO_Q("\r\n&YPlease retype password&W:&n ", d);
    if (STATE(d) == CON_NEWPASSWD) {
      STATE(d) = CON_CNFPASSWD;
    } else {
      STATE(d) = CON_CHPWD_VRFY;
    }

    break;
  }

  case CON_CNFPASSWD:
  case CON_CHPWD_VRFY:
    if (strncmp(CRYPT(arg, GET_PASSWD(d->character)), GET_PASSWD(d->character),
		MAX_PWD_LENGTH)) {
      SEND_TO_Q("\r\nPasswords don't match... start over.\r\n", d);
      SEND_TO_Q("&YPassword&W:&n ", d);
      if (STATE(d) == CON_CNFPASSWD) {
	STATE(d) = CON_NEWPASSWD;
      } else {
	STATE(d) = CON_CHPWD_GETNEW;
      }
      return;
    }

    echo_on(d);

    if (STATE(d) == CON_CNFPASSWD) {
      SEND_TO_Q("\r\n&YWhat is your sex &W(&MM&W/&MF&W)?&n ", d);
      STATE(d) = CON_QSEX;
    } else {
      save_char(d->character, NOWHERE);
      echo_on(d);
      SEND_TO_Q("\r\n&CDone&W.&n\n\r", d);
      SEND_TO_Q(MENU, d);
      STATE(d) = CON_MENU;
    }

    break;

  case CON_QSEX:		/* query sex of new user         */
    switch (*arg) {
    case 'm':
    case 'M':
      d->character->player.sex = SEX_MALE;
      break;
    case 'f':
    case 'F':
      d->character->player.sex = SEX_FEMALE;
      break;
    default:
      SEND_TO_Q("&RYou either have one or you don't&W.&n\r\n"
                "&YSo, what IS your sex&W?&n ", d);
      return;
    }

    SEND_TO_Q(race_menu, d);
    SEND_TO_Q("\r\n&MRace&W:&n ", d);
    STATE(d) = CON_QRACE;
    break;

 case CON_QRACE:
    load_result = parse_race(*arg);

    if (load_result == RACE_UNDEFINED) {
      SEND_TO_Q("\r\n&RThat's not a race&W.\r\n&MRace&W:&n ", d);
      return;
    } else {
      GET_RACE(d->character) = load_result;
    }

    display_classes(d);
    STATE(d) = CON_QCLASS;

    break;

  case CON_QCLASS:
    load_result = parse_class(d->character, *arg);

    if (load_result == CLASS_NEWRACE) {
      SEND_TO_Q(race_menu, d);
      SEND_TO_Q("\r\nRace: ", d);
      STATE(d) = CON_QRACE;
      return;
    } else if (load_result == CLASS_UNDEFINED) {
      SEND_TO_Q("\r\n&RThat's not a class&W.\r\n&YClass&W:&n ", d);
      return;
    } else if (load_result == CLASS_BAD_RACE) {
      SEND_TO_Q("\r\nYour race cannot become that class.\r\nClass: ", d);
      return;
    } else if (load_result == CLASS_BAD_STATS) {
      SEND_TO_Q("\r\nYour abilities do not measure up to that class.\r\n", d);
      SEND_TO_Q("Class: ", d);
      return;
    } else {
      GET_CLASS(d->character) = load_result;
    }

    SEND_TO_Q("\r\nSelect your static alignment.\r\n", d);

/* We're adding more alignments, so let's do this differently...
 * --gan, 07/25/2001 (Nuitari)

    if ((GET_CLASS(d->character) == CLASS_DRUID) ||
      (GET_CLASS(d->character) == CLASS_RANGER))
    {
      SEND_TO_Q("(G)ood  (N)eutral  or (Z) to go back: ", d);
    }
    else
    {
      SEND_TO_Q("(G)ood  (N)eutral  (E)vil, or (Z) to go back: ", d);
    }

*/


/* invalid_align
 * positions are: LG, NG, CG, LN, TN, CN, LE, NE, CE.
 * set it to 1 if it should be illegal.
 * --gan
 *
 * TO DO: change all these hardcoded values to defines
 */

    switch(GET_CLASS(d->character))
    {
       case CLASS_DRUID:
       case CLASS_RANGER:
          SEND_TO_Q("1) Lawful Good     2) Neutral Good  3) Chaotic Good\r\n", d);
          SEND_TO_Q("4) Lawful Neutral  5) True Neutral  6) Chaotic Neutral\r\n", d);
          invalid_align[7] = 1;
          invalid_align[8] = 1;
          invalid_align[9] = 1;
          break;

       case CLASS_MONK:
          SEND_TO_Q("1) Lawful Good\r\n", d);
          SEND_TO_Q("4) Lawful Neutral\r\n", d);
          SEND_TO_Q("7) Lawful Evil\r\n", d);
          invalid_align[2] = 1;
          invalid_align[3] = 1;
          invalid_align[5] = 1;
          invalid_align[6] = 1;
          invalid_align[8] = 1;
          invalid_align[9] = 1;
          break;

       case CLASS_KNIGHT:
          SEND_TO_Q("1) Lawful Good\r\n", d);
          invalid_align[2] = 1;
          invalid_align[3] = 1;
          invalid_align[4] = 1;
          invalid_align[5] = 1;
          invalid_align[6] = 1;
          invalid_align[7] = 1;
          invalid_align[8] = 1;
          invalid_align[9] = 1;
          break;

       case CLASS_PALADIN:
       case CLASS_DEFENDER:
          SEND_TO_Q("1) Lawful Good\r\n", d);
          SEND_TO_Q("4) Lawful Neutral\r\n", d);
          SEND_TO_Q("7) Lawful Evil\r\n", d);
          invalid_align[2] = 1;
          invalid_align[3] = 1;
          invalid_align[5] = 1;
          invalid_align[6] = 1;
          invalid_align[8] = 1;
          invalid_align[9] = 1;
          break;

       default:
          SEND_TO_Q("1) Lawful Good     2) Neutral Good  3) Chaotic Good\r\n", d);
          SEND_TO_Q("4) Lawful Neutral  5) True Neutral  6) Chaotic Neutral\r\n", d);
          SEND_TO_Q("7) Lawful Evil     8) Neutral Evil  9) Chaotic Evil\r\n", d);
          break;
    }

    SEND_TO_Q("You may also use '0' to go back to class selection.\r\nAlignment:", d);
    STATE(d) = CON_ALIGNMENT;
    break;


  case CON_QSHORT_D:
    if ((!*arg) || (strlen(arg) < 10) || (strlen(arg) > (LINE_DSCR_LENGTH-1))) {
      SEND_TO_Q("Illegal short description.\r\n", d);
      SEND_TO_Q("Enter your short description:\r\n", d);
      break;
    }

    if (d->character->player.short_descr) {
      free(d->character->player.short_descr);
      d->character->player.short_descr = NULL;
    }

    CREATE(d->character->player.short_descr, char, strlen(arg)+1);
    memset(d->character->player.short_descr, (char) NULL, strlen(arg)+1);
    strncpy(d->character->player.short_descr, arg, strlen(arg));

    SEND_TO_Q("\r\nPlease enter your 'alias list' -- words people can use\r\n", d);
    SEND_TO_Q("to identify you by.  This should include all attributes in\r\n", d);
    SEND_TO_Q("your short description.  For example, if your short description was:\r\n", d);
    SEND_TO_Q("   a fat, balding man with shifty eyes.\r\n", d);
    SEND_TO_Q("Your alias list would be: fat balding shifty\r\n--> ", d);
    STATE(d) = CON_ALIAS;
    break;

  case CON_ALIAS:
    if (!*arg) {
      SEND_TO_Q("You must enter an alias list:\r\n", d);
      break;
    }
    else
    {
      int aliasSize = 0;
      GET_NAME(d->character, chname);
      if (chname)
      {
        aliasSize = strlen(chname);
      }
      FREE_NAME(chname);
      aliasSize += 3 + strlen(pc_race_types[GET_RACE(d->character)]);
      aliasSize += strlen(arg);
 
      if (aliasSize > MAX_NAME_LENGTH) {
        SEND_TO_Q("Your alias list was too long, please re-enter:\r\n", d);
        break; 
      }
    }
    
    add_to_char_name(d->character, arg);

    SEND_TO_Q("\r\nYour character is complete.  Press enter.\r\n", d);
    STATE(d) = CON_AFTER_DESC;
    break;

  case CON_ALIGNMENT:
    /*  PDH  2/24/99
     *  added static alignment to char gen - this will affect clerics
     *  and mages in the spells that they can get
     *
     *  at some point, clerics will also select a God
     */
    if (!*arg) {
      SEND_TO_Q("You must select an alignment:\r\n", d);
      break;
    }

//    upChar = UPPER(*arg);
    upChar = (*arg);

/*
    if ((GET_CLASS(d->character) == CLASS_RANGER) && (upChar == 'E'))
    {
      SEND_TO_Q("A ranger is not permitted to be evil.\r\n", d);
      SEND_TO_Q("Please select your static alignment:\r\n", d);
      break;
    }

    if ((GET_CLASS(d->character) == CLASS_DRUID) && (upChar == 'E'))
    {
      SEND_TO_Q("A druid is not permitted to be evil.\r\n", d);
      SEND_TO_Q("Please select your static alignment:\r\n", d);
      break;
    }

    if ( upChar == 'G' ) {
      GET_ALIGNMENT(d->character) =  850;
    } else if ( upChar == 'N' ) {
      GET_ALIGNMENT(d->character) =    0;
    } else if ( upChar == 'E' ) {
      GET_ALIGNMENT(d->character) = -850;
    } else if ( upChar == 'Z' ) {
      display_classes(d);
      STATE(d) = CON_QCLASS;
      break;
    } else {
      SEND_TO_Q("Invalid alignment:\r\n", d);
      break;
    }
*/
    if(upChar == '0')
    {
      display_classes(d);
      STATE(d) = CON_QCLASS;
      break;
    }
    else if(upChar == '1')
    {
        if(invalid_align[1])
        {
          SEND_TO_Q("You cannot choose that alignment.\r\n", d);
          SEND_TO_Q("Please select your static alignment:\r\n", d);
          break;
        }
        GET_ALIGNMENT(d->character) = 850;
        GET_ETHOS(d->character) = 850;
        GET_HOME(d->character) = 1;
    }    
    else if(upChar == '2')
    {
      if(invalid_align[2])
      {
        SEND_TO_Q("You cannot choose that alignment.\r\n", d);
        SEND_TO_Q("Please select your static alignment:\r\n", d);
        break;
      }
      GET_ALIGNMENT(d->character) = 850;
      GET_ETHOS(d->character) = 0;
      GET_HOME(d->character) = 1;                                                                                              
    }
    else if(upChar == '3')
    {
      if(invalid_align[3])
      {
        SEND_TO_Q("You cannot choose that alignment.\r\n", d);
        SEND_TO_Q("Please select your static alignment:\r\n", d);
        break;
      }
      GET_ALIGNMENT(d->character) = 850;
      GET_ETHOS(d->character) = -850;
      GET_HOME(d->character) = 1;                                                                                              
    }
    else if(upChar == '4')
    {
      if(invalid_align[4])
      {
        SEND_TO_Q("You cannot choose that alignment.\r\n", d);
        SEND_TO_Q("Please select your static alignment:\r\n", d);
        break;
      }
      GET_ALIGNMENT(d->character) = 0;
      GET_ETHOS(d->character) = 850;
      GET_HOME(d->character) = dice(1, 2);
    }
    else if(upChar == '5')
    {
      if(invalid_align[5])
      {
        SEND_TO_Q("You cannot choose that alignment.\r\n", d);
        SEND_TO_Q("Please select your static alignment:\r\n", d);
        break;
      }
      GET_ALIGNMENT(d->character) = 0;
      GET_ETHOS(d->character) = 0;
      GET_HOME(d->character) = dice(1, 2); 
    }
    else if(upChar == '6')
    {
      if(invalid_align[6])
      {
        SEND_TO_Q("You cannot choose that alignment.\r\n", d);
        SEND_TO_Q("Please select your static alignment:\r\n", d);
        break;
      }
      GET_ALIGNMENT(d->character) = 0;
      GET_ETHOS(d->character) = -850;
      GET_HOME(d->character) = dice(1, 2);
    }
    else if(upChar == '7')
    {
      if(invalid_align[7])
      {
        SEND_TO_Q("You cannot choose that alignment.\r\n", d);
        SEND_TO_Q("Please select your static alignment:\r\n", d);
        break;
      }
      GET_ALIGNMENT(d->character) = -850;
      GET_ETHOS(d->character) = 850;
      GET_HOME(d->character) = 2;                                                                                              
    }
    else if(upChar == '8')
    {
      if(invalid_align[8])
      {
        SEND_TO_Q("You cannot choose that alignment.\r\n", d);
        SEND_TO_Q("Please select your static alignment:\r\n", d);
        break;
      }
      GET_ALIGNMENT(d->character) = -850;
      GET_ETHOS(d->character) = 0;
      GET_HOME(d->character) = 2;                                                                                              
    }
    else if(upChar == '9')
    {
      if(invalid_align[9])
      {
        SEND_TO_Q("You cannot choose that alignment.\r\n", d);
        SEND_TO_Q("Please select your static alignment:\r\n", d);
        break;
      }
      GET_ALIGNMENT(d->character) = -850;
      GET_ETHOS(d->character) = -850;
      GET_HOME(d->character) = 2;                                                                                              
    }
    else
    {
      SEND_TO_Q("Invalid alignment:\r\n", d);
      break;
    }

    if(GET_CLASS(d->character) == CLASS_CLERIC ||
       GET_CLASS(d->character) == CLASS_PALADIN)
    {
      display_god_menu(d);
      SEND_TO_Q("--> ", d);
      STATE(d) = CON_GODSELECT;

    }
    else
    {
      perform_init_stats(d);
      STATE(d) = CON_QABILITY;
    }
    break;


  case CON_GODSELECT:
    if ( ! *arg ) {
      SEND_TO_Q("You must select a god:\r\n", d);
      break;
    }

    upChar = UPPER(*arg);

    if ( IS_GOOD(d->character) ) {
      if ( (upChar >= 'A') && (upChar <= 'G') ) {
	switch(upChar) {
	case 'A':
	  GET_GODSELECT(d->character) = GOD_PALADINE;
	  STATE(d) = CON_AFTER_DESC;
	  break;
	case 'B':
	  GET_GODSELECT(d->character) = GOD_KIRIJOLITH;
	  STATE(d) = CON_AFTER_DESC;
	  break;
	case 'C':
	  GET_GODSELECT(d->character) = GOD_HABBAKUK;
	  STATE(d) = CON_AFTER_DESC;
	  break;
	case 'D':
	  GET_GODSELECT(d->character) = GOD_MISHAKAL;
	  STATE(d) = CON_AFTER_DESC;
	  break;
	case 'E':
	  GET_GODSELECT(d->character) = GOD_BRANCHALA;
	  STATE(d) = CON_AFTER_DESC;
	  break;
	  /*
	    case 'F':
	    GET_GODSELECT(d->character) = GOD_SOLINARI;
	    STATE(d) = CON_AFTER_DESC;
	    break;
	    */
	case 'G':
	  GET_GODSELECT(d->character) = GOD_MAJERE;
	  STATE(d) = CON_AFTER_DESC;
	  break;

	default:
	  SEND_TO_Q("Invalid choice.  Choose again.\r\n", d);
	  log("SYSERR: God selection");
	  break;
	}

      } else {
	SEND_TO_Q("Invalid choice.  Choose again.\r\n", d);
	break;
      }

    } else if ( IS_EVIL(d->character) ) {
      if ( (upChar >= 'O') && (upChar <= 'U') ) {
	switch(upChar) {
	case 'O':
	  GET_GODSELECT(d->character) = GOD_TAKHISIS;
          SEND_TO_Q("She's a harsh mistress, but she rewards like no other...\r\n", d);
	  STATE(d) = CON_AFTER_DESC;
	  break;
	case 'P':
	  GET_GODSELECT(d->character) = GOD_SARGONNAS;
	  STATE(d) = CON_AFTER_DESC;
	  break;
	  /*
	    case 'Q':
	    GET_GODSELECT(d->character) = GOD_NUITARI;
	    STATE(d) = CON_AFTER_DESC;
	    break;
	    */
	case 'R':
	  GET_GODSELECT(d->character) = GOD_MORGION;
	  STATE(d) = CON_AFTER_DESC;
	  break;
	case 'S':
	  GET_GODSELECT(d->character) = GOD_CHEMOSH;
	  STATE(d) = CON_AFTER_DESC;
	  break;
	case 'T':
	  GET_GODSELECT(d->character) = GOD_ZEBOIM;
	  STATE(d) = CON_AFTER_DESC;
	  break;
	case 'U':
	  GET_GODSELECT(d->character) = GOD_HIDDUKEL;;
	  STATE(d) = CON_AFTER_DESC;
	  break;

	default:
	  SEND_TO_Q("Invalid choice.  Choose again.\r\n", d);
	  log("SYSERR: God selection");
	  break;
	}

      } else {
	SEND_TO_Q("Invalid choice.  Choose again.\r\n", d);
	break;
      }

    } else {
      if ( (upChar >= 'H') && (upChar <= 'N') ) {
	switch(upChar) {
	case 'H':
	  GET_GODSELECT(d->character) = GOD_GILEAN;
/* I'm not gil anymore.. sniffle! --gan, 12.15.2k
          SEND_TO_Q("You made the right choice! Good for you!\r\n", d);
*/
	  STATE(d) = CON_AFTER_DESC;
	  break;
	case 'I':
	  GET_GODSELECT(d->character) = GOD_REORX;
	  STATE(d) = CON_AFTER_DESC;
	  break;
	  /*
	    case 'J':
	    GET_GODSELECT(d->character) = GOD_LUNITARI;
	    STATE(d) = CON_AFTER_DESC;
	    break;
	    */
	case 'K':
	  GET_GODSELECT(d->character) = GOD_CHISLEV;
	  STATE(d) = CON_AFTER_DESC;
	  break;
	case 'L':
	  GET_GODSELECT(d->character) = GOD_SIRRION;
	  STATE(d) = CON_AFTER_DESC;
	  break;
	case 'M':
	  GET_GODSELECT(d->character) = GOD_ZIVILYN;
	  STATE(d) = CON_AFTER_DESC;
	  break;
	case 'N':
	  GET_GODSELECT(d->character) = GOD_SHINARE;
	  STATE(d) = CON_AFTER_DESC;
	  break;

	default:
	  SEND_TO_Q("Invalid choice.  Choose again.\r\n", d);
	  log("SYSERR: God selection");
	  break;
	}

      } else {
	SEND_TO_Q("Invalid choice.  Choose again.\r\n", d);
	break;
      }

    }

    perform_init_stats(d);
    STATE(d) = CON_QABILITY;

    break;


  case CON_QABILITY:
    abilRet = abilityResults(d, arg);

    switch(abilRet) {
    case ABIL_DONE:
      /*  PDH 12/29/98 - put back to display_classes() on 1/12/99
	  SEND_TO_Q(class_menu, d);
	  SEND_TO_Q("\r\nClass: ", d);
	  */

      /*  PDH  4/29/99 - fighter classes with 18str get str_add bonus  */
      if ( ( (GET_CLASS(d->character) == CLASS_WARRIOR) ||
	     (GET_CLASS(d->character) == CLASS_KNIGHT) ||
	     (GET_CLASS(d->character) == CLASS_PALADIN) ||
	     (GET_CLASS(d->character) == CLASS_RANGER) ) &&
	   ( d->character->real_abils.str == 18 ) ) {
	d->character->real_abils.str_add = number(1,100);
      } else if ( d->character->real_abils.str > 18 ) {
	if ( GET_RACE(d->character) != RACE_MINOTAUR ) {
	  log("SYSERR: select ability: non-Minotaur got str > 18 !!!");
	}
	d->character->real_abils.str_add = -1;
      }

      if (GET_PFILEPOS(d->character) < 0) {
        GET_NAME(d->character, chname);
	GET_PFILEPOS(d->character) = create_entry(chname);
        FREE_NAME(chname);
      }

      init_char(d->character);
      save_char(d->character, NOWHERE);

      SEND_TO_Q("Now you are required to enter some descriptions in order to make your character\r\n", d);
      SEND_TO_Q("playable in our description-based game.  The first descriptions you enter are\r\n", d);
      SEND_TO_Q("mandatory at this stage.  They are based on a series of features you choose for\r\n", d);
      SEND_TO_Q("your character from lists.  The second set of descriptions are optional at\r\n", d);
      SEND_TO_Q("this point, though they need to be entered at some stage.  The second set of\r\n", d);
      SEND_TO_Q("descriptions will be your permanent descriptions and are completely custom-written\r\n", d);
      SEND_TO_Q("by you, but they need to be approved by a staff member.  Until the second set are\r\n", d);
      SEND_TO_Q("written and approved, you will use the first set, the ones you are about to do now,\r\n", d);
      SEND_TO_Q("and until the second set are written and approved, you will only be able to level\r\n", d);
      SEND_TO_Q("up to level 10.  This is usually about 15-30+ hours of game play so it should\r\n", d);
      SEND_TO_Q("be plenty of time for you to come up with the descriptions and have an immortal\r\n", d);
      SEND_TO_Q("approve them.  Don't forget as well, that you are required to write at least a few\r\n", d);
      SEND_TO_Q("sentences of a character background or personality profile to be sent to the staff\r\n", d);
      SEND_TO_Q("for approval as well.\r\n", d);
      SEND_TO_Q("\r\n", d);
      SEND_TO_Q("So without further ado, let's get ready to enter your descriptions.  You will first\r\n", d);
      SEND_TO_Q("choose an initial descrtiptor type from the list below.  Then you will choose a word\r\n", d);
      SEND_TO_Q("or phrase to describe it.  At this point you can accept your description or customize\r\n", d);
      SEND_TO_Q("it further with a second descriptor and describing word/phrase.  Try to keep your\r\n", d);
      SEND_TO_Q("description somewhat short as this will be used in place of your name in any situation\r\n", d);
      SEND_TO_Q("where someone has not been told your name using the introduce command.\r\n", d);
      SEND_TO_Q("\r\n", d);
      SEND_TO_Q("&Y(Press Enter to Continue)&n\r\n", d);

      STATE(d) = CON_GEN_DESCS_INTRO;

    break;

    /*
     *  PDH long desc
     */
/* Old example, has clothing in it.   10/15/2k --Gilean

      SEND_TO_Q("   Enter a description you'd like others to see when they look at you.\r\n", d);
      SEND_TO_Q("NOTE:  You must have a long description to be approved, with a *minimum of\r\n", d);
      SEND_TO_Q("       four lines* about your character.  Suggestions for things to write\r\n", d);
      SEND_TO_Q("       about are color of eyes and hair, facial hair, facial distinctions\r\n", d);
      SEND_TO_Q("       such as prominent cheekbones, body type/health, attractiveness, age,\r\n", d);
      SEND_TO_Q("       the character's apparant attitude, scars, tattoos, etc.\r\n", d);
      SEND_TO_Q("       Here is an example:\r\n", d);
      SEND_TO_Q("\r\n", d);
      SEND_TO_Q("Before you stands a man with heavy muscles and sturdy build.  His\r\n", d);
      SEND_TO_Q("hair is thick and brown and shoulder length, and he is tanned, with a\r\n", d);
      SEND_TO_Q("few battle scars criss crossing his burly arms.  His eyes are also\r\n", d);
      SEND_TO_Q("brown, and he appears confident and proud in what he does for a living. \r\n", d);
      SEND_TO_Q("\r\n", d);
      SEND_TO_Q("When done, type '/fi' on a blank line to format the description.\r\n", d);
      SEND_TO_Q("Then type '/s' to save the description.  ('/h' for help)\r\n", d);
*/

/* Backgroundy stuff for takkie --gan (Nui) */
/*
      SEND_TO_Q("This is where you need to enter your background. I'll have takling write\r\n", d);
      SEND_TO_Q("something inspiring here.\r\n", d);
      SEND_TO_Q("When done, type '/fi' on a blank line to format the description.\r\n", d);
      SEND_TO_Q("Then type '/s' to save the description.  ('/h' for help)\r\n", d);
 
      if (d->character->player.long_descr) {
        free(d->character->player.long_descr);
        d->character->player.long_descr = NULL;
      }
 
      d->str = &d->character->player.long_descr;
      d->max_str = EXDSCR_LENGTH;
      STATE(d) = CON_QBG_D;
*/
/* End backgroundy stuff */


    case ABIL_NEWRACE:
      SEND_TO_Q(race_menu, d);
      SEND_TO_Q("\r\nRace: ", d);
      STATE(d) = CON_QRACE;
      return;
      break;

    case ABIL_BADRACE:
      SEND_TO_Q("Your stats are not within your race's limits.\r\n", d);
      SEND_TO_Q(ability_string(d), d);
      break;

    case ABIL_BADCLASS:
      SEND_TO_Q("Your stats are not above your class minimums.\r\n", d);
      SEND_TO_Q(ability_string(d), d);
      break;

    case ABIL_NOTENUFPTS:
      SEND_TO_Q("You don't have enough points for that.\r\n", d);
      SEND_TO_Q("Try reducing another stat first.\r\n", d);
      SEND_TO_Q(ability_string(d), d);
      break;

    case ABIL_CONTINUE:
      SEND_TO_Q(ability_string(d), d);
      break;

    default:
      log("SYSERR: bad return code from abilityResults()");
      break;
    }

    /*  PDH  1/8/99 - removed - OLD
      if ( str_cmp(arg, "done") == 0 ) {
      SEND_TO_Q("Your stats have been set.  Enjoy!\r\n", d);
      SEND_TO_Q("\r\nYour character is complete.  Press enter.\r\n", d);
      STATE(d) = CON_AFTER_DESC;
    } else {
      SEND_TO_Q(ability_string(d), d);
    }
    */

      return;

  case CON_GEN_DESCS_INTRO:

      SEND_TO_Q("Current short description: &W", d);
      SEND_TO_Q(current_short_desc(d->character), d);
      SEND_TO_Q("&n\r\n\r\n", d);
      short_desc_descriptors_menu(d->character);
      STATE(d) = CON_GEN_DESCS_DESCRIPTORS_1;

      return;

  case CON_GEN_DESCS_DESCRIPTORS_1:

      count = NUM_FEATURE_TYPES;

      if (atoi(arg) < 1 || atoi(arg) > count) {
        SEND_TO_Q("That number is out of range.  Please choose again.\r\n\r\n", d);
        return;
      }

      GET_PC_DESCRIPTOR_1(d->character) = atoi(arg);
      short_desc_adjectives_menu(d->character, GET_PC_DESCRIPTOR_1(d->character));
      
      STATE(d) = CON_GEN_DESCS_ADJECTIVES_1;

      return;
	
    case CON_GEN_DESCS_ADJECTIVES_1:

      count = count_adjective_types(GET_PC_DESCRIPTOR_1(d->character));

      if (atoi(arg) < 1 || atoi(arg) > count) {
        SEND_TO_Q("That number is out of range.  Please choose again.\r\n\r\n", d);
        return;
      }

      GET_PC_ADJECTIVE_1(d->character) = atoi(arg);

      SEND_TO_Q("&Y(Press return to continue)&n", d);
      STATE(d) = CON_GEN_DESCS_MENU;
      return;

    case CON_GEN_DESCS_DESCRIPTORS_2:

      count = NUM_FEATURE_TYPES;

      if (atoi(arg) < 1 || atoi(arg) > count) {
        SEND_TO_Q("That number is out of range.  Please choose again.\r\n\r\n", d);
        return;
      }

      GET_PC_DESCRIPTOR_2(d->character) = atoi(arg);

      short_desc_adjectives_menu(d->character, GET_PC_DESCRIPTOR_2(d->character));
      
      STATE(d) = CON_GEN_DESCS_ADJECTIVES_2;

      return;

    case CON_GEN_DESCS_ADJECTIVES_2:

      count = count_adjective_types(GET_PC_DESCRIPTOR_2(d->character));

      if (atoi(arg) < 1 || atoi(arg) > count) {
        SEND_TO_Q("That number is out of range.  Please choose again.\r\n\r\n", d);
        return;
      }

      GET_PC_ADJECTIVE_2(d->character) = atoi(arg);

      SEND_TO_Q("&Y(Press return to continue)&n", d);
      STATE(d) = CON_GEN_DESCS_MENU;
      return;


    case CON_GEN_DESCS_MENU:

      SEND_TO_Q("Current short description: &W", d);
      SEND_TO_Q(current_short_desc(d->character), d);
      SEND_TO_Q("&n\r\n\r\n", d);
      SEND_TO_Q("Are you happy with this short description?\r\n", d);
      SEND_TO_Q("\r\n", d);
      SEND_TO_Q("1) I'm happy with it and am ready to play the game!\r\n", d);
      SEND_TO_Q("2) I'm happy with it and want to make my custom descs now as well.\r\n", d);
      SEND_TO_Q("3) I'm not happy with it and want to start over.\r\n", d);
      if (GET_PC_DESCRIPTOR_2(d->character) == 0)
        SEND_TO_Q("4) I'm not happy with it because I want to add a second descriptor.\r\n", d);
      SEND_TO_Q("\r\n", d);
      SEND_TO_Q("What would you like to do? (1-4): ", d);      

      STATE(d) = CON_GEN_DESCS_MENU_PARSE;

      return;

    case CON_GEN_DESCS_MENU_PARSE:

      switch (atoi(arg)) {

      case 1:
        STATE(d) = CON_AFTER_DESC;
        SEND_TO_Q("Your character is complete.  Press return to continue!\r\n", d);
        break;

      case 2:

        SEND_TO_Q("   Enter a description you'd like others to see when they look at you.\r\n", d);
        SEND_TO_Q("NOTE:  You must have a long description to be approved, with a *minimum of\r\n", d);
        SEND_TO_Q("       three lines* about your character.  Suggestions for things to write\r\n", d);
        SEND_TO_Q("       about are color of eyes and hair, facial hair, facial distinctions\r\n", d);
        SEND_TO_Q("       such as prominent cheekbones, body type/health, attractiveness, age,\r\n", d);
        SEND_TO_Q("       the character's apparant attitude, scars, tattoos, etc.\r\n", d);
        SEND_TO_Q("       Here is an example:\r\n\r\n", d);
        SEND_TO_Q("   Before you stands a tall man with bright golden blond hair and fierce\r\n", d);
        SEND_TO_Q("emerald green eyes.  His freshly shaven face surveys his surroundings searching\r\n", d);
        SEND_TO_Q("for anything that might be a threat or a benefit to him.  While not tall, \r\n", d);
        SEND_TO_Q("overly muscular or particularly handsome there is something about this man that\r\n", d);
        SEND_TO_Q("makes him stand out in a crowd.\r\n", d);
        SEND_TO_Q("\r\n", d);
        SEND_TO_Q("When done, type '/fi' on a blank line to format the description.\r\n", d);
        SEND_TO_Q("Then type '/s' to save the description.  ('/h' for help)\r\n", d);
  
        if (d->character->player.description) {
  	  free(d->character->player.description);
  	  d->character->player.description = NULL;
        }

        d->str = &d->character->player.description;
        d->max_str = EXDSCR_LENGTH;
        STATE(d) = CON_QLONG_D;

        /*
         *  PDH 12/29/98 - moved else where
         *
         save_char(d->character, NOWHERE);
         SEND_TO_Q(motd, d);
         SEND_TO_Q("\r\n\n&G*** &RPRESS RETURN&W:&n ", d);
         STATE(d) = CON_RMOTD;

         GET_NAME(d->character, chname);
         sprintf(buf, "%s [%s] new player.", chname, d->host);
         FREE_NAME(chname);
         immcharlog(buf);
         *
         */
        break;

      case 3:

        GET_PC_DESCRIPTOR_1(d->character) = 0;
        GET_PC_ADJECTIVE_1(d->character) = 0;
        GET_PC_DESCRIPTOR_2(d->character) = 0;
        GET_PC_ADJECTIVE_2(d->character) = 0;
        STATE(d) = CON_GEN_DESCS_INTRO;
        SEND_TO_Q("&YPress enter to continue)&n\r\n", d);
        break;

      case 4:
        if (GET_PC_DESCRIPTOR_2(d->character) == 0) {
          STATE(d) = CON_GEN_DESCS_DESCRIPTORS_2;

          SEND_TO_Q("Current short description: &W", d);
          SEND_TO_Q(current_short_desc(d->character), d);
          SEND_TO_Q("&n\r\n\r\n", d);
          short_desc_descriptors_menu(d->character);

          SEND_TO_Q("\r\n&Y(Press enter to choose your second descriptor)&n\r\n", d);
          return;
        }
        else {
          SEND_TO_Q("You have already set your second descriptor.  Please choose another option\r\n", d);
          return;
        }
        break;

      default:
        SEND_TO_Q("That is not a valid option.  Please choose again.\r\n", d);
        break;

      }

      return;	

  case CON_AFTER_DESC:
    /*  PDH 12/29/98
     *  old code has been moved here
     */
    if (GET_LEVEL(d->character) == 0)
      do_start(d->character);
    save_char(d->character, NOWHERE);
    SEND_TO_Q(motd, d);
    SEND_TO_Q("\r\n\n*** PRESS RETURN: ", d);
    STATE(d) = CON_RMOTD;

    GET_NAME(d->character, chname);
    sprintf(buf, "%s [%s] new player.", chname, d->host);
    FREE_NAME(chname);

    mudlog(buf, NRM, LVL_IMMORT, TRUE);
    break;


  case CON_RMOTD:		/* read CR after printing motd   */
    /*  PDH  2/20/99 - taken from CON_QANSI  */
 /*  Let's UN-take it and actually save our color settings.  Soli, 8/4/99  */

//  REMOVE_BIT(PRF_FLAGS(d->character), PRF_COLOR_1 | PRF_COLOR_2);
    SEND_TO_Q(MENU, d);
    STATE(d) = CON_MENU;
    break;

  case CON_MENU:		/* get selection from main menu  */
    switch (*arg) {
    case '0':
      if (d->character->followers || d->character->master)
        die_follower(d->character);
      SEND_TO_Q("&WGoodbye.&n\r\n", d);
      STATE(d) = CON_CLOSE;
      break;

    case '1':
    {
      struct descriptor_data *k, *next_k;
      int d_id = 0;

      d_id = GET_IDNUM(d->character);

      for (k = descriptor_list; k; k = next_k) 
      {
        next_k = k->next;

        if (!(k->character)) continue;
        if (!(d->character)) continue;

        if (d_id == GET_IDNUM(k->character)) continue;

        if (strcmp(d->host, k->host) == 0)
        {
          int level = MAX(GET_LEVEL(d->character), GET_LEVEL(k->character));

          char tmpBuf[MAX_INPUT_LENGTH];
          GET_NAME(d->character, chname);
          GET_NAME(k->character, chname2);
          sprintf(tmpBuf, "Multiplay detected from %s (%s/%s).", d->host, 
            chname, chname2);
          FREE_NAME(chname2);
          FREE_NAME(chname);

          if (level < LVL_DEMIGOD)
          {
            level = LVL_DEMIGOD;
          }

          mudlog(tmpBuf, NRM, level, TRUE);
        }
      }

     load_result = enter_player_game(d);
			
      send_to_char(WELC_MESSG, d->character);

      REMOVE_BIT(PLR_FLAGS(d->character), PLR_MAILING | PLR_WRITING);
      act("&Y$n &Mhas entered the game&W.&n", TRUE, d->character, 0, 0, TO_ROOM);
      STATE(d) = CON_PLAYING;
      if ( ! GET_LEVEL(d->character)) {
	/*
	 *  character approval
	 *  done by do_approve() now
	 */
	/*  do_start(d->character);  */
        GET_NAME(d->character, chname);
        sprintf(buf, "%s [%s]  New character - awaiting approval.",
		chname, d->host);
        FREE_NAME(chname);

/*	mudlog(buf, NRM, LVL_IMMORT, TRUE);   Nope nope nope --Gil */

        immcharlog(buf);
	send_to_char(START_MESSG, d->character);
      }

      if (affected_by_spell(d->character, SPELL_CRIPPLE))
        GET_POS(d->character) = POS_RESTING;
      if (affected_by_spell(d->character, SPELL_SLEEP))
        GET_POS(d->character) = POS_SLEEPING;

      race_affects(d->character);
      look_at_room(d->character, 0);

      if (has_mail(GET_IDNUM(d->character))) {
	send_to_char("You have mail waiting.\r\n", d->character);
      }

      if (load_result == 2) {	/* rented items lost */
	send_to_char("\r\n\007You could not afford your rent!\r\n"
          "Your possesions have been sold to make up for past rent payments.\r\n",
		     d->character);
      }

      d->has_prompt = 0;
      break;
    }

    case '2':
  
      if (IS_APPROVED(d->character)) {
        SEND_TO_Q("Your character's descriptions have already been approved and can only be changed\r\n", d);
        SEND_TO_Q("by an immortal.  Please contac an immortal in game to see if you are eligable for\r\n", d);
        SEND_TO_Q("a change.  Note that it is our policy to disallow changes except in the most\r\n", d);
        SEND_TO_Q("serious of situations.  Thanks!\r\n", d);
        return;
      }

        SEND_TO_Q("   Enter a description you'd like others to see when they look at you.\r\n", d);
        SEND_TO_Q("NOTE:  You must have a long description to be approved, with a *minimum of\r\n", d);
        SEND_TO_Q("       three lines* about your character.  Suggestions for things to write\r\n", d);
        SEND_TO_Q("       about are color of eyes and hair, facial hair, facial distinctions\r\n", d);
        SEND_TO_Q("       such as prominent cheekbones, body type/health, attractiveness, age,\r\n", d);
        SEND_TO_Q("       the character's apparant attitude, scars, tattoos, etc.\r\n", d);
        SEND_TO_Q("       Here is an example:\r\n\r\n", d);
        SEND_TO_Q("   Before you stands a tall man with bright golden blond hair and fierce\r\n", d);
        SEND_TO_Q("emerald green eyes.  His freshly shaven face surveys his surroundings searching\r\n", d);
        SEND_TO_Q("for anything that might be a threat or a benefit to him.  While not tall, \r\n", d);
        SEND_TO_Q("overly muscular or particularly handsome there is something about this man that\r\n", d);
        SEND_TO_Q("makes him stand out in a crowd.\r\n", d);
        SEND_TO_Q("\r\n", d);
        SEND_TO_Q("When done, type '/fi' on a blank line to format the description.\r\n", d);
        SEND_TO_Q("Then type '/s' to save the description.  ('/h' for help)\r\n", d);
  
        if (d->character->player.description) {
  	  free(d->character->player.description);
  	  d->character->player.description = NULL;
        }

        d->str = &d->character->player.description;
        d->max_str = EXDSCR_LENGTH;
        STATE(d) = CON_QLONG_D;

        /*
         *  PDH 12/29/98 - moved else where
         *
         save_char(d->character, NOWHERE);
         SEND_TO_Q(motd, d);
         SEND_TO_Q("\r\n\n&G*** &RPRESS RETURN&W:&n ", d);
         STATE(d) = CON_RMOTD;

         GET_NAME(d->character, chname);
         sprintf(buf, "%s [%s] new player.", chname, d->host);
         FREE_NAME(chname);
         immcharlog(buf);
         *
         */

      return;

/*
      SEND_TO_Q("You can only enter your description at creation time\r\n", d);
      SEND_TO_Q("or by asking a god to change it for you.  Sorry!  =)\r\n", d);
      return;
*/
      /*
      if (d->character->player.description) {
	SEND_TO_Q("Current description:\r\n", d);
	SEND_TO_Q(d->character->player.description, d);
	 *
	 * Don't free this now... so that the old description gets loaded
	 * as the current buffer in the editor.  Do setup the ABORT buffer
	 * here, however.
	 *
	 * free(d->character->player.description);
	 * d->character->player.description = NULL;
	 *  *** COMMENT END HERE
	d->backstr = str_dup(d->character->player.description);
      }

      SEND_TO_Q("Enter the new text you'd like others to see when they look at you.\r\n", d);
      SEND_TO_Q("(/s saves /h for help)\r\n", d);
      d->str = &d->character->player.description;
      d->max_str = EXDSCR_LENGTH;
      STATE(d) = CON_EXDESC;
      */
      break;

    case '3':
      page_string(d, background, 0);
      STATE(d) = CON_RMOTD;
      break;

    case '4':
      SEND_TO_Q("\r\n&YEnter your old password&W:&n ", d);
      echo_off(d);
      STATE(d) = CON_CHPWD_GETOLD;
      break;

    case '5':
      SEND_TO_Q("\r\n&YEnter your password for verification&W:&n ", d);
      echo_off(d);
      STATE(d) = CON_DELCNF1;
      break;

    default:
      SEND_TO_Q("\r\n&RThat's not a menu choice&W!&n\r\n", d);
      SEND_TO_Q(MENU, d);
      break;
    }

    break;

  case CON_CHPWD_GETOLD:
    if (strncmp(CRYPT(arg, GET_PASSWD(d->character)), GET_PASSWD(d->character), MAX_PWD_LENGTH)) {
      echo_on(d);
      SEND_TO_Q("\r\n&RIncorrect password&W.&n\r\n", d);
      SEND_TO_Q(MENU, d);
      STATE(d) = CON_MENU;
      return;
    } else {
      SEND_TO_Q("\r\n&YEnter a new password&W:&n ", d);
      STATE(d) = CON_CHPWD_GETNEW;
      return;
    }
    break;

  case CON_DELCNF1:
    echo_on(d);
    if (strncmp(CRYPT(arg, GET_PASSWD(d->character)), GET_PASSWD(d->character), MAX_PWD_LENGTH)) {
      SEND_TO_Q("\r\n&RIncorrect password&W.&n\r\n", d);
      SEND_TO_Q(MENU, d);
      STATE(d) = CON_MENU;
    } else {
      SEND_TO_Q("\r\nYOU ARE ABOUT TO DELETE THIS CHARACTER PERMANENTLY.\r\n"
                "ARE YOU ABSOLUTELY SURE YOU WANT TO LEAVE THE WORLD OF KRYNN?\r\n\r\n"
		"Please type \"yes\" to confirm: ", d);
      STATE(d) = CON_DELCNF2;
    }
    break;

  case CON_DELCNF2:
    if (!strcmp(arg, "yes") || !strcmp(arg, "YES")) {
      if (PLR_FLAGGED(d->character, PLR_FROZEN)) {
	SEND_TO_Q("You try to kill yourself, but the ice stops you.\r\n", d);
	SEND_TO_Q("Character not deleted.\r\n\r\n", d);
	STATE(d) = CON_CLOSE;
	SEND_TO_Q("Character not deleted.\r\n\r\n", d);
	STATE(d) = CON_CLOSE;
	return;
      }
      if (GET_LEVEL(d->character) < LVL_GRGOD)
	SET_BIT(PLR_FLAGS(d->character), PLR_DELETED);
      save_char(d->character, NOWHERE);
      GET_NAME(d->character, chname);
      Crash_delete_file(chname);
      sprintf(buf, "Your character, '%s', is deleted!\r\n"
	      "Goodbye.\r\n", chname);
      SEND_TO_Q(buf, d);
      sprintf(buf, "%s (lev %d) has self-deleted and left Krynn forever.", 
         chname, GET_LEVEL(d->character));
      FREE_NAME(chname);
      mudlog(buf, NRM, LVL_GOD, TRUE);
      STATE(d) = CON_CLOSE;
      return;
    } else {
      SEND_TO_Q("\r\nCharacter not deleted.\r\n", d);
      SEND_TO_Q(MENU, d);
      STATE(d) = CON_MENU;
    }
    break;

  case CON_CLOSE:
    if (d->character->master || d->character->followers)
      die_follower(d->character);
    break;

  default:
    log("SYSERR: Nanny: illegal state of con'ness; closing connection");
    STATE(d) = CON_DISCONNECT;
    break; 
  }
}

/* load the player, put them in the right room - used by copyover_recover too */
int enter_player_game (struct descriptor_data *d)
{

    int load_result;
    room_vnum load_room;
  extern sh_int r_mortal_start_room;
  extern sh_int r_immort_start_room;
  extern sh_int r_frozen_start_room;		
  //struct char_data *ch = d->character;    
  //int i;

      reset_char(d->character);
      read_aliases(d->character);

//      if (PLR_FLAGGED(d->character, PLR_INVSTART))
//	GET_INVIS_LEV(d->character) = GET_LEVEL(d->character);


      TIME_ONLINE(d->character) = 0;
      /*
       * We have to place the character in a room before equipping them
       * or equip_char() will gripe about the person in NOWHERE.
       */
	if ((load_room = GET_LOADROOM(d->character)) != NOWHERE)
          load_room = real_room(load_room);

/* If char was saved with NOWHERE, or real_room above failed... */
	if (load_room == NOWHERE) {
		if (GET_LEVEL(d->character) >= LVL_IMMORT) {
		load_room = r_immort_start_room;
		}
		else if(GET_APPROVED(d->character) == 0)
		{
			 load_room = r_mortal_start_room;      /* you not approved yet, newbieboy */
		}
		/* hometowns --gan, 07/27/2001 (Nuitari) */
		else if(GET_HOME(d->character) == 1)     /* Kalaman */
		{
			load_room = real_room(10172);
		 // load_room = 10172;                     /* Kalaman Inn */
		}
		else if(GET_HOME(d->character) == 2)     /* Karthay */
		{
			load_room = real_room(5113);
			//load_room = 5113;                      /* Karthay Inn */
		}
		else {
			load_room = r_mortal_start_room;
		}
	}

      if (PLR_FLAGGED(d->character, PLR_FROZEN)) {
	load_room = r_frozen_start_room;
      }

//      char_from_room(d->character);  /*  PDH  4/16/99  VERY important  */

      /* with the copyover patch, this next line goes in enter_player_game() */
      GET_ID(d->character) = GET_IDNUM(d->character);			
			

     /* Fix for increasing max skills */
/*
      for (i = 200; i < 300; i++) {
        GET_SKILL(ch, i + 300) = GET_SKILL(ch, i);
        GET_SKILL(ch, i) = 0;
      }
*/
      d->character->next = character_list;
      character_list = d->character;
      char_to_room(d->character, load_room);
      load_result = Crash_load(d->character, FALSE);
      save_char(d->character, NOWHERE);

    return load_result;
}
