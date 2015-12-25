/* ***********************************************************************
*  File: alias.c				A utility to CircleMUD	 *
* Usage: writing/reading player's aliases.				 *
*									 *
* Code done by Jeremy Hess and Chad Thompson				 *
* Modifed by George Greer for inclusion into CircleMUD bpl15.		 *
*									 *
* Copyright (C) 1993, 94 by the Trustees of the Johns Hopkins University *
* CircleMUD is based on DikuMUD, Copyright (C) 1990, 1991.		 *
*********************************************************************** */

#include "conf.h"
#include "sysdep.h"
#include "structs.h"
#include "utils.h"
#include "interpreter.h"
#include "db.h"

void write_aliases(struct char_data *ch);
void read_aliases(struct char_data *ch);

void write_aliases(struct char_data *ch)
{
  FILE *file;
  char fn[MAX_STRING_LENGTH];
  struct alias_data *temp;

  GET_NAME(ch, chname);
  get_filename(chname, fn, ALIAS_FILE);
  FREE_NAME(chname);
  remove(fn);

  if (GET_ALIASES(ch) == NULL)
    return;

  if ((file = fopen(fn, "w")) == NULL) {
    GET_NAME(ch, chname);
    fprintf(stderr, "SYSERR: Couldn't save aliases for %s in '%s'.", 
      chname, fn);
    FREE_NAME(chname);
    perror("SYSERR: write_aliases");
    return;
  }

  for (temp = GET_ALIASES(ch); temp; temp = temp->next) {
    int aliaslen = strlen(temp->alias);
    int repllen = strlen(temp->replacement) - 1;

    fprintf(file, "%d\n%s\n"	/* Alias */
		  "%d\n%s\n"	/* Replacement */
		  "%d\n",	/* Type */
		aliaslen, temp->alias,
		repllen, temp->replacement + 1,
		temp->type);
  }
  
  fclose(file);
}

void read_aliases(struct char_data *ch)
{   
  FILE *file;
  char xbuf[MAX_STRING_LENGTH];
  struct alias_data *t2;
  int length;

  GET_NAME(ch, chname);
  get_filename(chname, xbuf, ALIAS_FILE);
  FREE_NAME(chname);

  if ((file = fopen(xbuf, "r")) == NULL) {
    if (errno != ENOENT) {
      GET_NAME(ch, chname);
      fprintf(stderr, "SYSERR: Couldn't open alias file '%s' for %s.", 
        xbuf, chname);
      perror("SYSERR: read_aliases");
      FREE_NAME(chname);
    }
    return;
  }
 
  CREATE(GET_ALIASES(ch), struct alias_data, 1);
  t2 = GET_ALIASES(ch); 

  for (;;) {
    /* Read the aliased command. */
    fscanf(file, "%d\n", &length);
    fgets(xbuf, length + 1, file);
    t2->alias = str_dup(xbuf);

    /* Build the replacement. */
    fscanf(file, "%d\n", &length);
    *xbuf = ' ';		/* Doesn't need terminated, fgets() will. */
    fgets(xbuf + 1, length + 1, file);
    t2->replacement = str_dup(xbuf); 

    /* Figure out the alias type. */
    fscanf(file, "%d\n", &length);
    t2->type = length; 

    if (feof(file))
      break;

    CREATE(t2->next, struct alias_data, 1);
    t2 = t2->next;
  }; 
 
  fclose(file);
} 
