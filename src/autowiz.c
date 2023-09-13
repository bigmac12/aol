/* ************************************************************************
*  file:  autowiz.c                                     Part of CircleMUD *
*  Usage: self-updating wizlists                                          *
*  Written by Jeremy Elson                                                *
*  Edited for Ascii Pfiles by Catherine Gore                              *
*  All Rights Reserved                                                    *
*  Copyright (C) 1993 The Trustees of The Johns Hopkins University        *
************************************************************************* */


/*
   WARNING:  THIS CODE IS A HACK.  WE CAN NOT AND WILL NOT BE RESPONSIBLE
   FOR ANY NASUEA, DIZZINESS, VOMITING, OR SHORTNESS OF BREATH RESULTING
   FROM READING THIS CODE.  PREGNANT WOMEN AND INDIVIDUALS WITH BACK
   INJURIES, HEART CONDITIONS, OR ARE UNDER THE CARE OF A PHYSICIAN SHOULD
   NOT READ THIS CODE.

   -- The Management
 */

#include "conf.h"
#include "sysdep.h"

#include <signal.h>

#include "structs.h"
#include "utils.h"
#include "comm.h"
#include "db.h"
#include "interpreter.h"

#define IMM_LMARG "   "
#define IMM_NSIZE  16
#define LINE_LEN   64
#define MIN_LEVEL LVL_IMMORT

/* max level that should be in columns instead of centered */
#define COL_LEVEL LVL_IMMORT

/* globals */
extern int use_autowiz;
extern int min_wizlist_lev;
extern char *wizlist;
extern char *immlist;

/* extern functions */
int file_to_string_alloc(const char *name, char **buf);

/* local functions */
void initialize(void);
void read_file(void);
void add_name(byte level, char *name);
void sort_names(void);
void write_wizlist(FILE * out, int minlev, int maxlev);
void create_wizlists(void);
void run_autowiz(struct char_data * ch);
void reboot_wizlists(void);
ACMD(do_wizupdate);

struct name_rec {
  char name[25];
  struct name_rec *next;
};

struct control_rec {
  int level;
  char *level_name;
};

struct level_rec {
  struct control_rec *params;
  struct level_rec *next;
  struct name_rec *names;
};

/* Change these if you have different Immortal level names */
struct control_rec level_params[] =
{
  {LVL_IMMORT, "Immortals"},
  {LVL_GOD, "Gods"},
  {LVL_GRGOD, "Greater Gods"},
  {LVL_IMPL, "Implementors"},
  {0, ""}
};


struct level_rec *levels = 0;

void initialize(void)
{
  struct level_rec *tmp;
  int i = 0;

  while (level_params[i].level > 0) {
    tmp = (struct level_rec *) malloc(sizeof(struct level_rec));
    tmp->names = 0;
    tmp->params = &(level_params[i++]);
    tmp->next = levels;
    levels = tmp;
  }
}

/* Changed from the original to work with ascii pfiles */
void read_file(void)
{
  void add_name(byte level, char *name);
  struct char_data *player = NULL;
  int load, i;
  extern struct player_index_element *player_table;
  extern int top_of_p_table;

  for (i = 0; i <= top_of_p_table; i++) {
    if (player_table[i].level >= LVL_IMMORT) {
      CREATE(player, struct char_data, 1);
      clear_char(player);
      CREATE(player->player_specials, struct player_special_data, 1);
      if ((load = load_char(player_table[i].name, player)) > -1) {
        if (!PLR_FLAGGED(player, PLR_FROZEN) &&
           !PLR_FLAGGED(player, PLR_DELETED))
          add_name(GET_LEVEL(player), GET_NAME(player));
      }
      free_char(player);
    }
  }
}

void add_name(byte level, char *name)
{
  struct name_rec *tmp;
  struct level_rec *curr_level;
  char *ptr;

  if (!*name)
    return;

  for (ptr = name; *ptr; ptr++)
    if (!isalpha(*ptr))
      return;

  tmp = (struct name_rec *) malloc(sizeof(struct name_rec));
  strcpy(tmp->name, name);
  tmp->next = 0;

  curr_level = levels;
  while (curr_level->params->level > level)
    curr_level = curr_level->next;

  tmp->next = curr_level->names;
  curr_level->names = tmp;
}

void sort_names(void)
{
  struct level_rec *curr_level;
  struct name_rec *a, *b;
  char temp[100];

  for (curr_level = levels; curr_level; curr_level = curr_level->next) {
    for (a = curr_level->names; a && a->next; a = a->next) {
      for (b = a->next; b; b = b->next) {
       if (strcmp(a->name, b->name) > 0) {
         strcpy(temp, a->name);
         strcpy(a->name, b->name);
         strcpy(b->name, temp);
       }
      }
    }
  }
}

/* Change this to suit your taste in format for the wizlists */
void write_wizlist(FILE * out, int minlev, int maxlev)
{
  char buf[100];
  struct level_rec *curr_level;
  struct name_rec *curr_name;
  int i, j;

  fprintf(out,
"*************************************************************************\n"
"* The following people have reached immortality on CircleMUD.  They are *\n"
"* to be treated with respect and awe.  Occasional prayers to them are   *\n"
"* advisable.  Annoying them is not recommended.  Stealing from them is  *\n"
"* punishable by immediate death.                                        *\n"
"*************************************************************************\n\n");

  for (curr_level = levels; curr_level; curr_level = curr_level->next) {
    if (curr_level->params->level < minlev ||
       curr_level->params->level > maxlev)
      continue;
    i = 39 - (strlen(curr_level->params->level_name) >> 1);
    for (j = 1; j <= i; j++)
      fputc(' ', out);
    fprintf(out, "%s\n", curr_level->params->level_name);
    for (j = 1; j <= i; j++)
      fputc(' ', out);
    for (j = 1; j <= strlen(curr_level->params->level_name); j++)
      fputc('~', out);
    fprintf(out, "\n");

    strcpy(buf, "");
    curr_name = curr_level->names;
    while (curr_name) {
      strcat(buf, curr_name->name);
      if (strlen(buf) > LINE_LEN) {
       if (curr_level->params->level <= COL_LEVEL)
         fprintf(out, IMM_LMARG);
       else {
         i = 40 - (strlen(buf) >> 1);
         for (j = 1; j <= i; j++)
           fputc(' ', out);
       }
       fprintf(out, "%s\n", buf);
       strcpy(buf, "");
      } else {
       if (curr_level->params->level <= COL_LEVEL) {
         for (j = 1; j <= (IMM_NSIZE - strlen(curr_name->name)); j++)
           strcat(buf, " ");
       }
       if (curr_level->params->level > COL_LEVEL)
         strcat(buf, "   ");
      }
      curr_name = curr_name->next;
    }

    if (*buf) {
      if (curr_level->params->level <= COL_LEVEL)
       fprintf(out, "%s%s\n", IMM_LMARG, buf);
      else {
       i = 40 - (strlen(buf) >> 1);
       for (j = 1; j <= i; j++)
         fputc(' ', out);
       fprintf(out, "%s\n", buf);
      }
    }
    fprintf(out, "\n");
  }
}

/* Many thanks to Templar Viper for the heads up and Edward Glamkowski for the fix
 * to the um, minor? bug of the old create_wizlists.
 */
void create_wizlists(void)
{
        FILE *fl;

        if (wizlist) {
                free(wizlist);
                wizlist = NULL;
        }
        if (immlist) {
                free(immlist);
                immlist = NULL;
        }
        if (levels) {
                free(levels);
                levels = NULL;
        }

        initialize();
        read_file();
        sort_names();

        if (remove(WIZLIST_FILE)) {
                mudlog(BRF, LVL_GRGOD, TRUE,"Could not delete WIZLIST_FILE in create_wizlists()");
                return;
        }
        fl = fopen(WIZLIST_FILE, "w");
        write_wizlist(fl, min_wizlist_lev, LVL_IMPL);
        fclose(fl);

        if (remove(IMMLIST_FILE)) {
                mudlog(BRF, LVL_GRGOD, TRUE,"Could not delete IMMLIST_FILE in create_wizlists()");
                return;
        }
        fl = fopen(IMMLIST_FILE, "w");
        write_wizlist(fl, LVL_IMMORT, min_wizlist_lev - 1);
        fclose(fl);
}


void run_autowiz(struct char_data * ch)
{
  if (use_autowiz && GET_LEVEL(ch) >= LVL_IMMORT) {
    mudlog(CMP, LVL_IMMORT, FALSE,"Initiating autowiz.");
    create_wizlists();
    reboot_wizlists();
  }
}

void reboot_wizlists(void)
{
  file_to_string_alloc(WIZLIST_FILE, &wizlist);
  file_to_string_alloc(IMMLIST_FILE, &immlist);
}

ACMD(do_wizupdate)
{
       run_autowiz(ch);
       send_to_char(ch, "Wizlists updated.\n\r");
}

