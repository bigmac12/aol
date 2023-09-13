/* ************************************************************************
*   File: interpreter.h                                 Part of CircleMUD *
*  Usage: header file: public procs, macro defs, subcommand defines       *
*                                                                         *
*  All rights reserved.  See license.doc for complete information.        *
*                                                                         *
*  Copyright (C) 1993, 94 by the Trustees of the Johns Hopkins University *
*  CircleMUD is based on DikuMUD, Copyright (C) 1990, 1991.               *
************************************************************************ */

#define ACMD(name)  \
   void name(struct char_data *ch, char *argument, int cmd, int subcmd)

#define CMD_NAME (complete_cmd_info[cmd].command)
#define CMD_IS(cmd_name) (!strcmp(cmd_name, complete_cmd_info[cmd].command))
#define IS_MOVE(cmdnum) (cmdnum >= 1 && cmdnum <= 6)

struct command_info {
   char *command;
   const char *sort_as;
   byte minimum_position;
   void	(*command_pointer)
    (struct char_data *ch, char *argument, int cmd, int subcmd);
   sh_int minimum_level;
   int subcmd;
};

/* necessary for CMD_IS macro */
#ifndef __INTERPRETER_C__

// I understand that you might want to put this here for safety reasons,
// but it still causes a problem; without knowing ahead of time what
// size to expect for the struct, the compiler will complain.
// -- Branc, 12/22/11
//extern struct command_info cmd_info[];

extern struct command_info *complete_cmd_info;

#endif

int	    search_block(char *arg, const char **list, int exact);
int	    fill_word(char *argument);
int	    is_abbrev(char *arg1, char *arg2);
int	    is_number(char *str);
int	    find_command(char *command);
int     is_money(char *str);
char	lower( char c );
char	*one_argument(char *argument, char *first_arg);
char    *one_arg_dash(char *argument, char *first_arg);
char    *one_arg_dots(char *argument, char *first_arg);
char	*one_word(char *argument, char *first_arg);
char	*any_one_arg(char *argument, char *first_arg);
char	*two_arguments(char *argument, char *first_arg, char *second_arg);
char	*delete_doubledollar(char *string);
void	command_interpreter(struct char_data *ch, char *argument);
void	half_chop(char *string, char *arg1, char *arg2);
void	nanny(struct descriptor_data *d, char *arg);
void	skip_spaces(char **string);
void    value_money(struct money_data * tar, char *str);

/* This should be 11 or 15, depending on the number of
   reserved commands there are. For this MUD, 11 is fine */
#define RESERVE_CMDS            11

/* for compatibility with 2.20: */
#define argument_interpreter(a, b, c) two_arguments(a, b, c)

struct alias_data {
  char *alias;
  char *replacement;
  int type;
  struct alias_data *next;
};

#define ALIAS_SIMPLE	0
#define ALIAS_COMPLEX	1

#define ALIAS_SEP_CHAR	';'
#define ALIAS_VAR_CHAR	'$'
#define ALIAS_GLOB_CHAR	'*'

/*
 * SUBCOMMANDS
 *   You can define these however you want to, and the definitions of the
 *   subcommands are independent from function to function.
 */

/* directions */
#define SCMD_NORTH	1
#define SCMD_EAST	2
#define SCMD_SOUTH	3
#define SCMD_WEST	4
#define SCMD_UP		5
#define SCMD_DOWN	6
#define SCMD_NORTHWEST  7
#define SCMD_NORTHEAST  8
#define SCMD_SOUTHWEST  9
#define SCMD_SOUTHEAST 10

/* do_gen_ps */
#define SCMD_INFO       0
#define SCMD_HANDBOOK   1 
#define SCMD_CREDITS    2
#define SCMD_NEWS       3
#define SCMD_WIZLIST    4
#define SCMD_POLICIES   5
#define SCMD_VERSION    6
#define SCMD_IMMLIST    7
#define SCMD_MOTD	    8
#define SCMD_IMOTD	    9
#define SCMD_CLEAR	    10
#define SCMD_WHOAMI	    11

/* do_gen_tog */
#define SCMD_NOHASSLE   1
#define SCMD_BRIEF      2
#define SCMD_COMPACT    3
#define SCMD_NOTELL 	4
#define SCMD_DEAF	    6
#define SCMD_NOWIZ	    9
#define SCMD_QUEST	    10
#define SCMD_ROOMFLAGS	11
#define SCMD_NOREPEAT	12
#define SCMD_HOLYLIGHT	13
#define SCMD_SLOWNS	    14
#define SCMD_AUTOEXIT	15
#define SCMD_AFK	    16
#define SCMD_LEVEL_FLAGS 19
#define SCMD_INTRO       20
#define SCMD_NOOOC	    21
#define SCMD_NONEWBIE   22
#define SCMD_NOCHAT     23
#define SCMD_DETECT     24
#define SCMD_XAP_OBJS   25

/* do_wizutil */
#define SCMD_REROLL	    0
#define SCMD_PARDON     1
#define SCMD_SQUELCH    3
#define SCMD_FREEZE	    4
#define SCMD_THAW	    5
#define SCMD_UNAFFECT	6
#define SCMD_BADMORTNONEWBIE 7

/* do_spec_com */
#define SCMD_WHISPER	0
#define SCMD_ASK	    1

/* do_gen_com */
#define SCMD_HOLLER	0
#define SCMD_SHOUT	1

/* do_shutdown */
#define SCMD_SHUTDOW	0
#define SCMD_SHUTDOWN   1

/* do_quit */
#define SCMD_QUI	0
#define SCMD_QUIT	1

/* do_date */
#define SCMD_DATE	0
#define SCMD_UPTIME	1

/* do_commands */
#define SCMD_COMMANDS	0
#define SCMD_SOCIALS	1
#define SCMD_WIZHELP	2

/* do_drop */
#define SCMD_DROP	0

/* do_gen_write */
#define SCMD_BUG	0
#define SCMD_TYPO	1
#define SCMD_IDEA	2

/* do_look */
#define SCMD_LOOK	0
#define SCMD_READ	1

/* do_qcomm */
#define SCMD_QSAY	0
#define SCMD_QECHO	1

/* do_pour */
#define SCMD_POUR	0
#define SCMD_FILL	1

/* do_poof */
#define SCMD_POOFIN	    0
#define SCMD_POOFOUT	1

/* do_hit */
#define SCMD_HIT	0
#define SCMD_MURDER	1

/* do_eat */
#define SCMD_EAT	0
#define SCMD_TASTE	1
#define SCMD_DRINK	2
#define SCMD_SIP	3

/* do_use */
#define SCMD_USE	0
#define SCMD_QUAFF	1
#define SCMD_RECITE	2

/* do_echo */
#define SCMD_ECHO	0
#define SCMD_EMOTE	1

/* do_gen_door */
#define SCMD_OPEN       0
#define SCMD_CLOSE      1
#define SCMD_UNLOCK     2
#define SCMD_LOCK       3
#define SCMD_PICK       4

/* do_action  PDH 11/24/98 */
#define SCMD_GREET      1
/*  SEE ABOVE: do_gen_tog    #define SCMD_INTRO      2   */

/*. do_olc .*/
#define SCMD_OLC_REDIT  0
#define SCMD_OLC_OEDIT  1
#define SCMD_OLC_ZEDIT  2
#define SCMD_OLC_MEDIT  3
#define SCMD_OLC_SEDIT  4
#define SCMD_OLC_TRIGEDIT  5
#define SCMD_OLC_SAVEINFO  6


/*  PDH  1/15/99  */
/* do_approve */
#define SCMD_APPROVE    1
#define SCMD_REJECT     2


/*  PDH  3/20/99  */
/* do_day */
#define SCMD_OOCSAY     1


