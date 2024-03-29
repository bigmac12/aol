/************************************************************************
 * Generic OLC Library - Mobiles / genmob.c			v1.0	*
 * Copyright 1996 by Harvey Gilpin					*
 * Copyright 1997-2001 by George Greer (greerga@circlemud.org)		*
 ************************************************************************/

#include "conf.h"
#include "sysdep.h"

#include "structs.h"
#include "utils.h"
#include "db.h"
#include "shop.h"
#include "handler.h"
#include "genolc.h"
#include "genmob.h"
#include "genzon.h"
#include "dg_olc.h"

int update_mobile_strings(struct char_data *t, struct char_data *f);
void check_mobile_strings(struct char_data *mob);
void check_mobile_string(mob_vnum i, char **string, const char *dscr);
int write_mobile_record(mob_vnum mvnum, struct char_data *mob, FILE *fd);
int write_mobile_espec(mob_vnum mvnum, struct char_data *mob, FILE *fd);
int free_mobile_strings(struct char_data *mob);
int copy_mobile_strings(struct char_data *t, struct char_data *f);
#if CONFIG_GENOLC_MOBPROG
int write_mobile_mobprog(mob_vnum mvnum, struct char_data *mob, FILE *fd);
#endif

extern int top_shop;
extern mob_rnum top_of_mobt;
extern zone_rnum top_of_zone_table;
extern struct zone_data *zone_table;
extern struct shop_data *shop_index;
extern struct char_data *character_list;
extern struct char_data *mob_proto;
extern struct index_data *mob_index;

int add_mobile(struct char_data *mob, mob_vnum vnum)
{
  int rnum, i, found = FALSE, shop, cmd_no;
  zone_rnum zone;
  struct char_data *live_mob;

  if ((rnum = real_mobile(vnum)) != NOBODY) {
    /* Copy over the mobile and free() the old strings. */
    copy_mobile(&mob_proto[rnum], mob);

    /* Now re-point all existing mobile strings to here. */
    for (live_mob = character_list; live_mob; live_mob = live_mob->next)
      if (rnum == live_mob->nr)
        update_mobile_strings(live_mob, &mob_proto[rnum]);

    add_to_save_list(zone_table[real_zone_by_thing(vnum)].number, SL_MOB);
    log("GenOLC: add_mobile: Updated existing mobile #%d.", vnum);
    return rnum;
  }

  RECREATE(mob_proto, struct char_data, top_of_mobt + 2);
  RECREATE(mob_index, struct index_data, top_of_mobt + 2);
  top_of_mobt++;

  for (i = top_of_mobt; i > 0; i--) {
    if (vnum > mob_index[i - 1].vnum) {
      mob_proto[i] = *mob;
      mob_proto[i].nr = i;
      copy_mobile_strings(&mob_proto[i], mob);
      mob_index[i].vnum = vnum;
      mob_index[i].number = 0;
      mob_index[i].func = 0;
      found = i;
      break;
    }
    mob_index[i] = mob_index[i - 1];
    mob_proto[i] = mob_proto[i - 1];
    mob_proto[i].nr++;
  }
  if (!found) {
    mob_proto[0] = *mob;
    mob_proto[0].nr = 0;
    copy_mobile_strings(&mob_proto[0], mob);
    mob_index[0].vnum = vnum;
    mob_index[0].number = 0;
    mob_index[0].func = 0;
  }

  log("GenOLC: add_mobile: Added mobile %d at index #%d.", vnum, found);

#if CONFIG_GENOLC_MOBPROG
  GET_MPROG(OLC_MOB(d)) = OLC_MPROGL(d);
  GET_MPROG_TYPE(OLC_MOB(d)) = (OLC_MPROGL(d) ? OLC_MPROGL(d)->type : 0);
  while (OLC_MPROGL(d)) {
    GET_MPROG_TYPE(OLC_MOB(d)) |= OLC_MPROGL(d)->type;
    OLC_MPROGL(d) = OLC_MPROGL(d)->next;
  }
#endif

  /*
   * Update live mobile rnums.
   */
  for (live_mob = character_list; live_mob; live_mob = live_mob->next)
    GET_MOB_RNUM(live_mob) += (GET_MOB_RNUM(live_mob) >= found);

  /*
   * Update zone table.
   */
  for (zone = 0; zone <= top_of_zone_table; zone++)
    for (cmd_no = 0; ZCMD(zone, cmd_no).command != 'S'; cmd_no++)
      if (ZCMD(zone, cmd_no).command == 'M')
	ZCMD(zone, cmd_no).arg1 += (ZCMD(zone, cmd_no).arg1 >= found);

  /*
   * Update shop keepers.
   */
  if (shop_index)
    for (shop = 0; shop <= top_shop - top_shop_offset; shop++)
      SHOP_KEEPER(shop) += (SHOP_KEEPER(shop) >= found);

  add_to_save_list(zone_table[real_zone_by_thing(vnum)].number, SL_MOB);
  return found;
}

int copy_mobile(struct char_data *to, struct char_data *from)
{
  free_mobile_strings(to);
  *to = *from;
  check_mobile_strings(from);
  copy_mobile_strings(to, from);
  return TRUE;
}

void extract_mobile_all(mob_vnum vnum)
{
  struct char_data *next, *ch;

  for (ch = character_list; ch; ch = next) {
    next = ch->next;
    if (GET_MOB_VNUM(ch) == vnum)
      extract_char(ch);
  }
}

int delete_mobile(mob_rnum refpt)
{
  struct char_data *live_mob;
  int vnum, counter, zone, cmd_no;

  if (refpt == NOBODY || refpt > top_of_mobt) {
    log("SYSERR: GenOLC: delete_mobile: Invalid rnum %d.", refpt);
    return FALSE;
  }

  vnum = mob_index[refpt].vnum;
  add_to_save_list(zone_table[real_zone_by_thing(vnum)].number, SL_MOB);
  extract_mobile_all(vnum);
  free_mobile_strings(&mob_proto[refpt]);

  for (counter = refpt; counter < top_of_mobt; counter++) {
    mob_index[counter] = mob_index[counter + 1];
    mob_proto[counter] = mob_proto[counter + 1];
    mob_proto[counter].nr--;
  }

  top_of_mobt--;
  RECREATE(mob_index, struct index_data, top_of_mobt + 1);
  RECREATE(mob_proto, struct char_data, top_of_mobt + 1);

  /*
   * Update live mobile rnums.
   */
  for (live_mob = character_list; live_mob; live_mob = live_mob->next)
    GET_MOB_RNUM(live_mob) -= (GET_MOB_RNUM(live_mob) >= refpt);

  /*
   * Update zone table.
   */
  for (zone = 0; zone <= top_of_zone_table; zone++)
    for (cmd_no = 0; ZCMD(zone, cmd_no).command != 'S'; cmd_no++)
      if (ZCMD(zone, cmd_no).command == 'M')
	ZCMD(zone, cmd_no).arg1 -= (ZCMD(zone, cmd_no).arg1 >= refpt);

  /*
   * Update shop keepers.
   */
  if (shop_index)
    for (counter = 0; counter <= top_shop - top_shop_offset; counter++)
      SHOP_KEEPER(counter) -= (SHOP_KEEPER(counter) >= refpt);

  return TRUE;
}

/* New copy_mobile_strings copies any attached scripts too - Welcor */
int copy_mobile_strings(struct char_data *t, struct char_data *f)
{

  if (f->player.name)
    t->player.name = strdup(f->player.name);
  if (f->player.title)
    t->player.title = strdup(f->player.title);
  if (f->player.short_descr)
    t->player.short_descr = strdup(f->player.short_descr);
  if (f->player.long_descr)
    t->player.long_descr = strdup(f->player.long_descr);
  if (f->player.description)
    t->player.description = strdup(f->player.description);

  return TRUE;
}

int update_mobile_strings(struct char_data *t, struct char_data *f)
{
  if (f->player.name)
    t->player.name = f->player.name;
  if (f->player.title)
    t->player.title = f->player.title;
  if (f->player.short_descr)
    t->player.short_descr = f->player.short_descr;
  if (f->player.long_descr)
    t->player.long_descr = f->player.long_descr;
  if (f->player.description)
    t->player.description = f->player.description;

  return TRUE;
}

int free_mobile_strings(struct char_data *mob)
{
  if (mob->player.name)
    free(mob->player.name);
  if (mob->player.title)
    free(mob->player.title);
  if (mob->player.short_descr)
    free(mob->player.short_descr);
  if (mob->player.long_descr)
    free(mob->player.long_descr);
  if (mob->player.description)
    free(mob->player.description);

  return TRUE;
}

/*
 * Free a mobile structure that has been edited.
 * Take care of existing mobiles and their mob_proto!
 */
int free_mobile(struct char_data *mob)
{
  int i;

  if (mob == NULL)
    return FALSE;

  /*
   * Non-prototyped mobile.  Also known as new mobiles.
   */
  if ((i = GET_MOB_RNUM(mob)) == NOBODY) {
    free_mobile_strings(mob);
    free_proto_script(mob, MOB_TRIGGER);
  } else {	/* Prototyped mobile. */
    if (mob->player.name && mob->player.name != mob_proto[i].player.name)
      free(mob->player.name);
    if (mob->player.title && mob->player.title != mob_proto[i].player.title)
      free(mob->player.title);
    if (mob->player.short_descr && mob->player.short_descr != mob_proto[i].player.short_descr)
      free(mob->player.short_descr);
    if (mob->player.long_descr && mob->player.long_descr != mob_proto[i].player.long_descr)
      free(mob->player.long_descr);
    if (mob->player.description && mob->player.description != mob_proto[i].player.description)
      free(mob->player.description);
    if (mob->proto_script && mob->proto_script != mob_proto[i].proto_script)
      free_proto_script(mob, MOB_TRIGGER);
  }
  while (mob->affected)
    affect_remove(mob, mob->affected, 0);

  
  free(mob);
  return TRUE;
}

int save_mobiles(zone_rnum rznum)
{
  zone_vnum vznum;
  FILE *mobfd;
  room_vnum i;
  mob_rnum rmob;
  int written;
  char mobfname[64], usedfname[64];

  if (rznum == NOWHERE || rznum > top_of_zone_table) {
    log("SYSERR: GenOLC: save_mobiles: Invalid real zone number %d. (0-%d)", rznum, top_of_zone_table);
    return FALSE;
  }

  vznum = zone_table[rznum].number;
  sprintf(mobfname, "%s%d.new", MOB_PREFIX, vznum);
  if ((mobfd = fopen(mobfname, "w")) == NULL) {
    mudlog(BRF, LVL_GOD, TRUE, "SYSERR: GenOLC: Cannot open mob file for writing.");
    return FALSE;
  }

  for (i = genolc_zone_bottom(rznum); i <= zone_table[rznum].top; i++) {
    if ((rmob = real_mobile(i)) == NOBODY)
      continue;
    check_mobile_strings(&mob_proto[rmob]);
    if (write_mobile_record(i, &mob_proto[rmob], mobfd) < 0)
      log("SYSERR: GenOLC: Error writing mobile #%d.", i);
  }
  fputs("$\n", mobfd);
  written = ftell(mobfd);
  fclose(mobfd);
  sprintf(usedfname, "%s%d.mob", MOB_PREFIX, vznum);
  remove(usedfname);
  rename(mobfname, usedfname);
  remove_from_save_list(vznum, SL_MOB);
  log("GenOLC: '%s' saved, %d bytes written.", usedfname, written);
  return written;
}

#if CONFIG_GENOLC_MOBPROG
int write_mobile_mobprog(mob_vnum mvnum, struct char_data *mob, FILE *fd)
{
  char wmmarg[MAX_STRING_LENGTH], wmmcom[MAX_STRING_LENGTH];
  MPROG_DATA *mob_prog;

  for (mob_prog = GET_MPROG(mob); mob_prog; mob_prog = mob_prog->next) {
    wmmarg[MAX_STRING_LENGTH - 1] = '\0';
    wmmcom[MAX_STRING_LENGTH - 1] = '\0';
    strip_cr(strncpy(wmmarg, mob_prog->arglist, MAX_STRING_LENGTH - 1));
    strip_cr(strncpy(wmmcom, mob_prog->comlist, MAX_STRING_LENGTH - 1));
    fprintf(fd,	"%s %s~\n"
		"%s%c\n",
	medit_get_mprog_type(mob_prog), wmmarg,
	wmmcom, STRING_TERMINATOR
    );
    if (mob_prog->next == NULL)
      fputs("|\n", fd);
  }
  return TRUE;
}
#endif

int write_mobile_espec(mob_vnum mvnum, struct char_data *mob, FILE *fd)
{
  if (GET_ATTACK(mob) != 0)
    fprintf(fd, "BareHandAttack: %d\n", GET_ATTACK(mob));
  if (GET_STR(mob) != 11)
    fprintf(fd, "Str: %d\n", GET_STR(mob));
  if (GET_ADD(mob) != 0)
    fprintf(fd, "StrAdd: %d\n", GET_ADD(mob));
  if (GET_DEX(mob) != 11)
    fprintf(fd, "Dex: %d\n", GET_DEX(mob));
  if (GET_INT(mob) != 11)
    fprintf(fd, "Int: %d\n", GET_INT(mob));
  if (GET_WIS(mob) != 11)
    fprintf(fd, "Wis: %d\n", GET_WIS(mob));
  fputs("E\n", fd);
  return TRUE;
}


/*
 * NOTE: This clobbers some of the global variable strings. Do not
 *	save anything in them around calls to this function.
 *
 *	buf1, buf2
 */
int write_mobile_record(mob_vnum mvnum, struct char_data *mob, FILE *fd)
{
  char ldesc[MAX_STRING_LENGTH], ddesc[MAX_STRING_LENGTH];
  char fbuf1[MAX_STRING_LENGTH];
  char fbuf2[MAX_STRING_LENGTH];
  char fbuf3[MAX_STRING_LENGTH];
  char fbuf4[MAX_STRING_LENGTH];
  char abuf1[MAX_STRING_LENGTH];
  char abuf2[MAX_STRING_LENGTH];
  char abuf3[MAX_STRING_LENGTH];
  char abuf4[MAX_STRING_LENGTH];
  char sbuf1[MAX_STRING_LENGTH];
  char sbuf2[MAX_STRING_LENGTH];
  char sbuf3[MAX_STRING_LENGTH];
  char sbuf4[MAX_STRING_LENGTH];

  ldesc[MAX_STRING_LENGTH - 1] = '\0';
  ddesc[MAX_STRING_LENGTH - 1] = '\0';

  strip_cr(strncpy(ldesc, GET_LDESC(mob), MAX_STRING_LENGTH - 1));
  strip_cr(strncpy(ddesc, GET_DDESC(mob), MAX_STRING_LENGTH - 1));

  fprintf(fd,	"#%d\n"
		"%s%c\n"
		"%s%c\n"
		"%s%c\n"
		"%s%c\n",
	mvnum,
	GET_ALIAS(mob), STRING_TERMINATOR,
	GET_SDESC(mob), STRING_TERMINATOR,
	ldesc, STRING_TERMINATOR,
	ddesc, STRING_TERMINATOR
  );

  sprintascii(fbuf1, MOB_FLAGS(mob)[0]);
  sprintascii(fbuf2, MOB_FLAGS(mob)[1]);
  sprintascii(fbuf3, MOB_FLAGS(mob)[2]);
  sprintascii(fbuf4, MOB_FLAGS(mob)[3]);
  sprintascii(abuf1, AFF_FLAGS(mob)[0]);
  sprintascii(abuf2, AFF_FLAGS(mob)[1]);
  sprintascii(abuf3, AFF_FLAGS(mob)[2]);
  sprintascii(abuf4, AFF_FLAGS(mob)[3]);
  sprintascii(sbuf1, SPELL_FLAGS(mob)[0]);
  sprintascii(sbuf2, SPELL_FLAGS(mob)[1]);
  sprintascii(sbuf3, SPELL_FLAGS(mob)[2]);
  sprintascii(sbuf4, SPELL_FLAGS(mob)[3]);

  fprintf(fd, "%s %s %s %s %s %s %s %s %s %s %s %s %d E\n"
		"%d %d %d %dd%d+%d %dd%d+%d\n",
		fbuf1, fbuf2, fbuf3, fbuf4,
		abuf1, abuf2, abuf3, abuf4,
                sbuf1, sbuf2, sbuf3, sbuf4,
	        GET_ALIGNMENT(mob),
		GET_LEVEL(mob), 20 - GET_HITROLL(mob), GET_AC(mob) / 10, GET_HIT(mob),
	        GET_MANA(mob), GET_MOVE(mob), GET_NDD(mob), GET_SDD(mob),
		GET_DAMROLL(mob));

  fprintf(fd, 	"%d %d\n"
		"%d %d %d\n",
		money_to_copper(GET_MONEY(mob)), GET_EXP(mob),
		GET_POS(mob), GET_DEFAULT_POS(mob), GET_SEX(mob)
  );

  if (write_mobile_espec(mvnum, mob, fd) < 0)
    log("SYSERR: GenOLC: Error writing E-specs for mobile #%d.", mvnum);

  script_save_to_disk(fd, mob, MOB_TRIGGER);

#if CONFIG_GENOLC_MOBPROG
  if (write_mobile_mobprog(mvnum, mob, fd) < 0)
    log("SYSERR: GenOLC: Error writing MobProgs for mobile #%d.", mvnum);
#endif

  return TRUE;
}

void check_mobile_strings(struct char_data *mob)
{
  mob_vnum mvnum = mob_index[mob->nr].vnum;
  check_mobile_string(mvnum, &GET_LDESC(mob), "long description");
  check_mobile_string(mvnum, &GET_DDESC(mob), "detailed description");
  check_mobile_string(mvnum, &GET_ALIAS(mob), "alias list");
  check_mobile_string(mvnum, &GET_SDESC(mob), "short description");
}

void check_mobile_string(mob_vnum i, char **string, const char *dscr)
{
  if (*string == NULL || **string == '\0') {
    char smbuf[128];
    sprintf(smbuf, "GenOLC: Mob #%d has an invalid %s.", i, dscr);
    mudlog(BRF, LVL_GOD, TRUE, smbuf);
    if (*string)
      free(*string);
    *string = strdup("An undefined string.");
  }
}
