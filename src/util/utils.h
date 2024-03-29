/* ************************************************************************
*   File: utils.h                                       Part of CircleMUD *
*  Usage: header file: utility macros and prototypes of utility funcs     *
*                                                                         *
*  All rights reserved.  See license.doc for complete information.        *
*                                                                         *
*  Copyright (C) 1993, 94 by the Trustees of the Johns Hopkins University *
*  CircleMUD is based on DikuMUD, Copyright (C) 1990, 1991.               *
************************************************************************ */


/* external declarations and prototypes **********************************/

extern struct weather_data weather_info;

#define log basic_mud_log

/* public functions in utils.c */
char	*str_dup(const char *source);
int	str_cmp(const char *arg1, const char *arg2);
int	strn_cmp(const char *arg1, const char *arg2, int n);
void    basic_mud_log(const char *format, ...) __attribute__ ((format (printf, 1, 2)));
int	touch(const char *path);
void	mudlog(const char *str, int type, int level, int file);
void	log_death_trap(struct char_data *ch);
int	number(int from, int to);
int	dice(int number, int size);
void	sprintbit(bitvector_t vektor, const char *names[], char *result);
void    sprintspellbit(bitvector_t vektor, const char *names[], char *result);
void	sprinttype(int type, const char *names[], char *result);
int	get_line(FILE *fl, char *buf);
int	get_filename(char *orig_name, char *filename, int mode);
struct time_info_data *age(struct char_data *ch);
int	num_pc_in_room(struct room_data *room);
void	core_dump_real(const char *, int);
int is_color(struct char_data *ch);
int is_killer(struct char_data *ch);
int is_thief(struct char_data *ch);
int get_filename2(char *filename, size_t fbufsize, int mode, const char *orig_name);
void max_group_exp_mult(struct char_data *ch);
void check_improve(struct char_data * ch, int skill);

#define core_dump()             core_dump_real(__FILE__, __LINE__)

int     replace_str(char **string, char *pattern, char *replacement, int rep_all, int max_size);
void    format_text(char **ptr_string, int mode, struct descriptor_data *d, int maxlen);
int     check_skill(struct char_data * ch, int prob, int skillnum);
int     use_skill(struct char_data *ch, int percent, int skillnum);
int     assert_fighting(struct char_data * ch);
int     is_weapon(int type);

/*  GMB 10/29/99 - rewrite of GET_NAME and GET_NAME_II */
void get_name(struct char_data* ch, char** chname);
void get_name_II(struct char_data* ch, const struct char_data* vi, char** chname);
void get_name_IV(struct char_data* ch, char** chname);
void get_name_III(struct char_data* ch, const struct char_data* vi, char** chname);
void choose_name(struct char_data* ch, char** chname);
void choose_name_II(struct char_data* ch, const struct char_data* vi, char** chname);
void get_pers(struct char_data* ch, const struct char_data* vi, char** chname);
void get_pers_II(struct char_data* ch, const struct char_data* vi, char** chname);

#define GET_NAME(xch, xk) { char* xk = (char*) NULL; get_name(xch, &xk);
#define GET_NAME_II(xch, xvi, xk) { char* xk = (char*) NULL; get_name_II(xch, xvi, &xk);
#define GET_NAME_IV(xch, xk) { char* xk = (char*) NULL; get_name(xch, &xk);
#define GET_NAME_III(xch, xvi, xk) { char* xk = (char*) NULL; get_name_II(xch, xvi, &xk);
#define CHOOSE_NAME(xch, xk) { char* xk = (char*) NULL; choose_name(xch, &xk);
#define CHOOSE_NAME_II(xch, xvi, xk) { char* xk = (char*) NULL; choose_name_II(xch, xvi, &xk);
#define GET_PERS(xch, xvi, xk) { char* xk = (char*) NULL; get_pers(xch, xvi, &xk); 
#define GET_PERS_II(xch, xvi, xk) { char* xk = (char*) NULL; get_pers(xch, xvi, &xk);
#define FREE_NAME(xk) if (xk != (char*) NULL) free(xk); xk = (char*) NULL; }

/*  PDH  1/14/99 - debug util  */
char *  describe_align(int align);
char *  describe_ethos(int align);
char *  describe_clan(int clannum);

/*  PDH  3/29/99 - money system  */
int money_to_copper(struct money_data money);
void copper_to_money(struct money_data * target, int copper);
void money_subtract(struct money_data * target, struct money_data subAmt);
void money_assign(struct money_data * target, struct money_data amount);
char * describe_money(struct money_data money);
void copper_to_money_random(struct money_data * target, int copper);
int money_compare(struct money_data more, struct money_data less, int strict);

/* random functions in random.c */
void circle_srandom(unsigned long initial_seed);
unsigned long circle_random(void);

/* undefine MAX and MIN so that our functions are used instead */
#ifdef MAX
#undef MAX
#endif

#ifdef MIN
#undef MIN
#endif

int MAX(int a, int b);
int MIN(int a, int b);
char *CAP(char *txt);

/* in magic.c */
bool	circle_follow(struct char_data *ch, struct char_data * victim);

/* in act.informative.c */
void	look_at_room(struct char_data *ch, int mode);

/* in act.movmement.c */
int	do_simple_move(struct char_data *ch, int dir, int following);
int	perform_move(struct char_data *ch, int dir, int following);

/* in limits.c */
int	hit_limit(struct char_data *ch);
int	move_limit(struct char_data *ch);
int	hit_gain(struct char_data *ch);
int	move_gain(struct char_data *ch);
void	advance_level(struct char_data *ch);
void	set_title(struct char_data *ch, char *title);
void	gain_exp(struct char_data *ch, int gain);
void	gain_exp_regardless(struct char_data *ch, int gain);
void	gain_condition(struct char_data *ch, int condition, int value);
void	check_idling(struct char_data *ch);
void	point_update(void);
void	update_pos(struct char_data *victim);


/* various constants *****************************************************/


/* defines for mudlog() */
#define OFF	0
#define BRF	1
#define NRM	2
#define CMP	3

/* get_filename() */
#define CRASH_FILE	0
#define ETEXT_FILE	1
#define ALIAS_FILE      2
#define BACKUP_FILE     3

/* breadth-first searching */
#define BFS_ERROR		-1
#define BFS_ALREADY_THERE	-2
#define BFS_NO_PATH		-3

/* mud-life time */
#define SECS_PER_MUD_HOUR	75
#define SECS_PER_MUD_DAY	(24*SECS_PER_MUD_HOUR)
#define SECS_PER_MUD_MONTH	(35*SECS_PER_MUD_DAY)
#define SECS_PER_MUD_YEAR	(17*SECS_PER_MUD_MONTH)

/* real-life time (remember Real Life?) */
#define SECS_PER_REAL_MIN	60
#define SECS_PER_REAL_HOUR	(60*SECS_PER_REAL_MIN)
#define SECS_PER_REAL_DAY	(24*SECS_PER_REAL_HOUR)
#define SECS_PER_REAL_YEAR	(365*SECS_PER_REAL_DAY)


/* string utils **********************************************************/


#define YESNO(a) ((a) ? "YES" : "NO")
#define ONOFF(a) ((a) ? "ON" : "OFF")

#define LOWER(c)   (((c)>='A'  && (c) <= 'Z') ? ((c)+('a'-'A')) : (c))
#define UPPER(c)   (((c)>='a'  && (c) <= 'z') ? ((c)+('A'-'a')) : (c) )

#define ISNEWL(ch) ((ch) == '\n' || (ch) == '\r') 
#define IF_STR(st) ((st) ? (st) : "\0")

#define AN(string) (strchr("aeiouAEIOU", *string) ? "an" : "a")


/* memory utils **********************************************************/


#define CREATE(result, type, number)  do {\
    if ((number) * sizeof(type) <= 0) \
        { log("SYSERR: Zero bytes or less requested at %s:%d.", __FILE__, __LINE__); }\
	if (!((result) = (type *) calloc ((number), sizeof(type))))\
		{ perror("malloc failure"); abort(); } } while(0)

#define RECREATE(result, type, number) do {\
  if (!((result) = (type *) realloc ((result), sizeof(type) * (number))))\
		{ perror("realloc failure"); abort(); } } while(0)

/*
 * the source previously used the same code in many places to remove an item
 * from a list: if it's the list head, change the head, else traverse the
 * list looking for the item before the one to be removed.  Now, we have a
 * macro to do this.  To use, just make sure that there is a variable 'temp'
 * declared as the same type as the list to be manipulated.  BTW, this is
 * a great application for C++ templates but, alas, this is not C++.  Maybe
 * CircleMUD 4.0 will be...
 */
#define REMOVE_FROM_LIST(item, head, next)	\
   if ((item) == (head))		\
      head = (item)->next;		\
   else {				\
      temp = head;			\
      while (temp && (temp->next != (item))) \
	 temp = temp->next;		\
      if (temp)				\
         temp->next = (item)->next;	\
   }					\


/* basic bitvector utils *************************************************/


#define IS_SET(flag,bit)  ((flag) & (bit))
#define SET_BIT(var,bit)  ((var) |= (bit))
#define REMOVE_BIT(var,bit)  ((var) &= ~(bit))
#define TOGGLE_BIT(var,bit) ((var) = (var) ^ (bit))

#define MOB_FLAGS(ch) ((ch)->char_specials.saved.act)
#define PLR_FLAGS(ch) ((ch)->char_specials.saved.act)
#define PRF_FLAGS(ch) ((ch)->player_specials->saved.pref)
#define AFF_FLAGS(ch) ((ch)->char_specials.saved.affected_by)
#define AFF2_FLAGS(ch) ((ch)->char_specials.saved.affected_by_2)
#define SPELL_FLAGS(ch) ((ch)->char_specials.saved.mob_spell)
#define ROOM_FLAGS(loc) (world[(loc)].room_flags)
#define OBJ_FLAGS(obj)  (obj->obj_flags.extra_flags)
#define OBJ_WEAR(obj)   (obj->obj_flags.wear_flags)

#define IS_NPC(ch)  (IS_SET(MOB_FLAGS(ch), MOB_ISNPC))
#define IS_MOB(ch)  (IS_NPC(ch) && ((ch)->nr >-1))

#define MOB_FLAGGED(ch, flag) (IS_NPC(ch) && IS_SET(MOB_FLAGS(ch), (flag)))
#define PLR_FLAGGED(ch, flag) (!IS_NPC(ch) && IS_SET(PLR_FLAGS(ch), (flag)))
#define AFF_FLAGGED(ch, flag) (IS_SET(AFF_FLAGS(ch), (flag)))
#define AFF2_FLAGGED(ch, flag) (IS_SET(AFF2_FLAGS(ch), (flag)))
#define PRF_FLAGGED(ch, flag) (IS_SET(PRF_FLAGS(ch), (flag)))
#define ROOM_FLAGGED(loc, flag) (IS_SET(ROOM_FLAGS(loc), (flag)))
#define EXIT_FLAGGED(exit, flag) (IS_SET((exit)->exit_info, (flag)))
#define OBJ_FLAGGED(obj, flag)  (IS_SET(OBJ_FLAGS(obj), (flag)))
#define OBJWEAR_FLAGGED(obj, flag) (IS_SET(OBJ_WEAR(obj), (flag)))

/* IS_AFFECTED for backwards compatibility */
#define IS_AFFECTED(ch, skill) (AFF_FLAGGED((ch), (skill)))

#define PLR_TOG_CHK(ch,flag) ((TOGGLE_BIT(PLR_FLAGS(ch), (flag))) & (flag))
#define PRF_TOG_CHK(ch,flag) ((TOGGLE_BIT(PRF_FLAGS(ch), (flag))) & (flag))

#define HAS_SPELLS(obj) ((obj)->has_spells)
#define GET_WEAPON_SPELL(obj,i) ((obj)->wpn_spells[i].spellnum)
#define GET_WEAPON_SPELL_LVL(obj,i) ((obj)->wpn_spells[i].level)
#define GET_WEAPON_SPELL_PCT(obj,i) ((obj)->wpn_spells[i].percent)

/* room utils ************************************************************/


#define SECT(room)	(world[(room)].sector_type)

#define OLD_IS_DARK(room)  ( !world[room].light && \
                         ( ROOM_FLAGGED(room, ROOM_DARK) || \
			   ( ( SECT(room) != SECT_INSIDE && \
			       SECT(room) != SECT_CITY ) && \
			     ( weather_info.sunlight == SUN_SET || \
			       weather_info.sunlight == SUN_DARK ) ) ) )

#define IS_DARK(room)   ((world[room].light < 0) || \
                        ((world[room].light < 1) && \
			  ( ROOM_FLAGGED(room, ROOM_DARK) || \
			    ( ( SECT(room) != SECT_INSIDE && \
				SECT(room) != SECT_CITY ) && \
			      ( weather_info.sunlight == SUN_SET || \
                                weather_info.sunlight == SUN_DARK ) ) ) ) )

#define IS_LIGHT(room)  (!IS_DARK(room))

#define VALID_RNUM(rnum) ((rnum) >= 0 && (rnum) <= top_of_world)
#define GET_ROOM_VNUM(rnum) \
	((room_vnum)(VALID_RNUM(rnum) ? world[(rnum)].number : NOWHERE))
#define GET_ROOM_SPEC(room) (VALID_RNUM(room) ? world[(room)].func : NULL)
#define ROOM_SPEC(room)     (world[(room)].specnum)


// display macros

#define SEND_TO_CHAR(ch, string) (send_to_char(string, ch))

/* char utils ************************************************************/



#define IN_ROOM(ch)	((ch)->in_room)
#define GET_WAS_IN(ch)	((ch)->was_in_room)
#define GET_AGE(ch)     (age(ch).year)

int known_player(const struct char_data *ch, struct char_data *who);

#define GET_PC_NAME(ch)    (IS_NPC(ch) ? \
                          (ch)->player.short_descr : strtok((ch)->player.name, ""))

#if 0


#define GET_NAME(ch)    (IS_NPC(ch) ? \
                          (ch)->player.short_descr : strtok((ch)->player.name, ""))

/*  PDH 11/24/98  */

#define GET_NAME_II(ch, fr)     (IS_NPC(ch) ? \
                                 (ch)->player.short_descr : \
                                 (known_player(fr, ch) ? \
                                 strtok((ch)->player.name, " ") : (ch)->player.short_descr ))

#endif
#define GET_TITLE(ch)      ((ch)->player.title)
#define POOFIN(ch)         ((ch)->player.poofin)
#define POOFOUT(ch)        ((ch)->player.poofout)
#define GET_LEVEL(ch)      ((int) (ch)->player.level)
/*  No, no, no, no, no, this is very, very bad.... Soli, 6/5/99
#define GET_TRUST_LEVEL(ch)  \
                           ((ch)->player_specials->saved.trust)
  Let's try THIS instead... */
#define GET_TRUST_LEVEL(ch)  (IS_MOB(ch) ? GET_LEVEL(ch) : \
                             ((ch)->player_specials->saved.trust))
#define GET_PASSWD(ch)     ((ch)->player.passwd)
#define GET_PFILEPOS(ch)   ((ch)->pfilepos)

#define TIME_ONLINE(ch)    ((ch)->player_specials->saved.timeOnline)

/*
 * I wonder if this definition of GET_REAL_LEVEL should be the definition
 * of GET_LEVEL?  JE
 */
#define GET_REAL_LEVEL(ch) \
   (ch->desc && ch->desc->original ? GET_LEVEL(ch->desc->original) : \
    GET_LEVEL(ch))

#define GET_CLASS(ch)     ((ch)->player.class)
#define GET_SPEC(ch)      (IS_NPC(ch) ? (ch)->mob_specials.spec : 0)
#define GET_RACE(ch)      ((ch)->player_specials->saved.race)
#define GET_RPFACTOR(ch)  ((ch)->player_specials->saved.RPpercent)
#define GET_APPROVED(ch)  ((ch)->player_specials->saved.approved)
#define GET_CLAN(ch)      ((ch)->player_specials->saved.clans)
#define GET_MEM_LEVEL_FLAG(ch) ((ch)->player_specials->saved.memLevelFlag)

#define GET_CLANLEVEL(ch) ((ch)->player_specials->saved.clan_level)
#define GET_HOME(ch)      ((ch)->player.hometown)
#define GET_HEIGHT(ch)    ((ch)->player.height)
#define GET_WEIGHT(ch)    ((ch)->player.weight)
#define GET_SEX(ch)       ((ch)->player.sex)

#define GET_STR(ch)     ((ch)->aff_abils.str)
#define GET_ADD(ch)     ((ch)->aff_abils.str_add)
#define GET_DEX(ch)     ((ch)->aff_abils.dex)
#define GET_INT(ch)     ((ch)->aff_abils.intel)
#define GET_WIS(ch)     ((ch)->aff_abils.wis)
#define GET_CON(ch)     ((ch)->aff_abils.con)
#define GET_CHA(ch)     ((ch)->aff_abils.cha)

#define GET_EXP(ch)	  ((ch)->points.exp)
#define GET_AC(ch)        ((ch)->points.armor)
#define GET_HIT(ch)	  ((ch)->points.hit)
#define GET_MAX_HIT(ch)	  ((ch)->points.max_hit)
#define GET_MOVE(ch)	  ((ch)->points.move)
#define GET_MAX_MOVE(ch)  ((ch)->points.max_move)
#define GET_MANA(ch)	  ((ch)->points.mana)
#define GET_MAX_MANA(ch)  ((ch)->points.max_mana)

/*  #define GET_GOLD(ch)      ((ch)->points.gold)  */
#define GET_MONEY(ch)     ((ch)->points.money)

#define GET_QUESTPOINTS(ch) ((ch)->points.questpoints)
#define GET_BANK_GOLD(ch) ((ch)->points.bank_gold)
#define GET_HITROLL(ch)	  ((ch)->points.hitroll)
#define GET_DAMROLL(ch)   ((ch)->points.damroll)
#define GET_MAXMV(ch)     ((ch)->points.max_move)
#define GET_POS(ch)	  ((ch)->char_specials.position)
#define GET_IDNUM(ch)	  ((ch)->char_specials.saved.idnum)
#define GET_ID(x)         ((x)->id)
#define IS_CARRYING_W(ch) ((ch)->char_specials.carry_weight)
#define IS_CARRYING_N(ch) ((ch)->char_specials.carry_items)
#define FIGHTING(ch)	  ((ch)->char_specials.fighting)
#define HUNTING(ch)	  ((ch)->char_specials.hunting)
#define GUARDING(ch)      ((ch)->char_specials.guarding)
#define RIDING(ch)	  ((ch)->char_specials.riding)		/* (DAK) */
#define RIDDEN_BY(ch)	  ((ch)->char_specials.ridden_by)	/* (DAK) */
#define NAME_DIS(ch)	  ((ch)->char_specials.name_dis)
#define SEX_DIS(ch)       ((ch)->char_specials.sex_dis)

#define DESC_DIS(ch)      ((ch)->char_specials.desc_dis)

#define GET_SAVE(ch, i)	  ((ch)->char_specials.saved.apply_saving_throw[i])
#define GET_ALIGNMENT(ch) ((ch)->char_specials.saved.alignment)
#define GET_ETHOS(ch)     ((ch)->char_specials.saved.ethos)


#define IS_APPROVED(ch)   ((ch)->player_specials->saved.approved)

#define GET_PC_DESCRIPTOR_1(ch) (ch->player_specials->saved.sdesc_descriptor_1)
#define GET_PC_DESCRIPTOR_2(ch) (ch->player_specials->saved.sdesc_descriptor_2)
#define GET_PC_ADJECTIVE_1(ch) (ch->player_specials->saved.sdesc_adjective_1)
#define GET_PC_ADJECTIVE_2(ch) (ch->player_specials->saved.sdesc_adjective_2)

#define GET_SPELL_SOURCE(ch) (ch->player_specials->saved.spell_source)

/*  PDH  2/25/99 - god selection (clerics and paladins only)  */
#define GET_GODSELECT(ch) ((ch)->player_specials->saved.god_selection)

#define GET_COND(ch, i)		((ch)->player_specials->saved.conditions[(i)])
#define GET_LOADROOM(ch)	((ch)->player_specials->saved.load_room)
#define GET_APPROVED(ch)        ((ch)->player_specials->saved.approved)
#define GET_PRACTICES(ch)	((ch)->player_specials->saved.spells_to_learn)
#define GET_INVIS_LEV(ch)	((ch)->player_specials->saved.invis_level)
#define GET_WIMP_LEV(ch)	((ch)->player_specials->saved.wimp_level)
#define GET_FREEZE_LEV(ch)	((ch)->player_specials->saved.freeze_level)
#define GET_BAD_PWS(ch)		((ch)->player_specials->saved.bad_pws)
#define GET_TALK(ch, i)		((ch)->player_specials->saved.talks[i])
#define GET_LAST_OLC_TARG(ch)	((ch)->player_specials->last_olc_targ)
#define GET_LAST_OLC_MODE(ch)	((ch)->player_specials->last_olc_mode)
#define GET_ALIASES(ch)		((ch)->player_specials->aliases)
#define GET_LAST_TELL(ch)	((ch)->player_specials->last_tell)


/*  PDH 11/24/98 - intro system  */
#define GET_INTRO(ch, i)        ((ch)->player_specials->saved.intro[i].name)
#define GET_INTRONUM(ch, i)     ((ch)->player_specials->saved.intro[i].charnum)

#define spell(ch, i)            ((ch)->player_specials->saved.spells[i])

#define GET_SKILL(ch, i)        ((ch)->player_specials->saved.skills[i].percent)
#define SET_SKILL(ch, i, pct)   { (ch)->player_specials->saved.skills[i].percent = pct; }

#define GET_EXP_MULT(ch)        ((ch)->player_specials->saved.exp_multiplier)

#define GET_EQ(ch, i)		((ch)->equipment[i])

#define GET_MOB_SPEC(ch) 	(IS_MOB(ch) ? mob_index[(ch->nr)].func : NULL)
#define GET_MOB_RNUM(mob)	((mob)->nr)
#define GET_MOB_VNUM(mob)	(IS_MOB(mob) ? \
				 mob_index[GET_MOB_RNUM(mob)].virtual : -1)

/* A safer get_vnum for the script driver --gan */
#define GET_ACTOR_VNUM(m)       (IS_NPC(m) ? \
                                 mob_index[GET_MOB_RNUM(m)].virtual : -1)

#define GET_MOB_WAIT(ch)	((ch)->mob_specials.wait_state)
#define GET_BACKSTABBED(ch)     ((ch)->mob_specials.backstabbed)
#define GET_DEFAULT_POS(ch)	((ch)->mob_specials.default_pos)
#define MEMORY(ch)		((ch)->mob_specials.memory)
#define QUEST(ch)		((ch)->mob_specials.quest)

#define STRENGTH_APPLY_INDEX(ch) \
        ( ((GET_ADD(ch)==0) || (GET_STR(ch) != 18)) ? GET_STR(ch) :\
          (GET_ADD(ch) <= 50) ? 26 :( \
          (GET_ADD(ch) <= 75) ? 27 :( \
          (GET_ADD(ch) <= 90) ? 28 :( \
          (GET_ADD(ch) <= 99) ? 29 :  30 ) ) )                   \
        )

#define CAN_CARRY_W(ch) (str_app[STRENGTH_APPLY_INDEX(ch)].carry_w)
#define CAN_CARRY_N(ch)   (5 + (GET_DEX(ch) / 2))
#define AWAKE(ch) (GET_POS(ch) > POS_SLEEPING)
#define CAN_SEE_IN_DARK(ch) \
   (AFF_FLAGGED(ch, AFF_INFRAVISION) ||  PLR_FLAGGED(ch, PLR_FIRE_ON) || \
    PRF_FLAGGED(ch, PRF_HOLYLIGHT))

#define IS_GOOD(ch)    (GET_ALIGNMENT(ch) >= 350)
#define IS_EVIL(ch)    (GET_ALIGNMENT(ch) <= -350)
#define IS_NEUTRAL(ch) (!IS_GOOD(ch) && !IS_EVIL(ch))


/* descriptor-based utils ************************************************/


#define WAIT_STATE(ch, cycle) { \
        if (!IS_NPC(ch) && (GET_LEVEL(ch) >= LVL_IMMORT)) (ch)->desc->wait = 0;\
        else if ((ch)->desc) (ch)->desc->wait = (cycle); \
	else if (IS_NPC(ch)) GET_MOB_WAIT(ch) = (cycle); }

#define CHECK_WAIT(ch)	(((ch)->desc) ? ((ch)->desc->wait > 1) : 0)
#define STATE(d)	((d)->connected)
#define GET_WAIT_STATE(ch) ((ch)->wait)

/* object utils **********************************************************/


#define GET_OBJ_TYPE(obj)	((obj)->obj_flags.type_flag)
#define GET_OBJ_COST(obj)	((obj)->obj_flags.cost)
#define GET_OBJ_RENT(obj)	((obj)->obj_flags.cost_per_day)
#define GET_OBJ_EXTRA(obj)	((obj)->obj_flags.extra_flags)
#define GET_OBJ_WEAR(obj)	((obj)->obj_flags.wear_flags)
#define GET_OBJ_VAL(obj, val)	((obj)->obj_flags.value[(val)])
#define GET_OBJ_WEIGHT(obj)	((obj)->obj_flags.weight)
#define GET_OBJ_LEVEL(obj)	((obj)->obj_flags.obj_level)
#define GET_OBJ_TIMER(obj)      ((obj)->obj_flags.timer)
#define GET_OBJ_CREATION(obj)   ((obj)->obj_flags.creation)
#define GET_OBJ_DISGUISE(obj)   ((obj)->obj_flags.disguise)
#define GET_OBJ_NATURE(obj)     ((obj)->obj_flags.nature)
#define GET_OBJ_RNUM(obj)	((obj)->item_number)
#define GET_OBJ_VNUM(obj)	(GET_OBJ_RNUM(obj) >= 0 ? \
				 obj_index[GET_OBJ_RNUM(obj)].virtual : -1)
#define IS_OBJ_STAT(obj,stat)	(IS_SET((obj)->obj_flags.extra_flags,stat))
#define IS_CORPSE(obj)		(GET_OBJ_TYPE(obj) == ITEM_CONTAINER && \
					GET_OBJ_VAL((obj), 3) == 1)

#define IS_HIDDEN(obj) (IS_SET(GET_OBJ_EXTRA(obj), ITEM_HIDDEN))

#define GET_OBJ_SPEC(obj) ((obj)->item_number >= 0 ? \
	(obj_index[(obj)->item_number].func) : NULL)

#define CAN_WEAR(obj, part) (IS_SET((obj)->obj_flags.wear_flags, (part)))


/* compound utilities and other macros **********************************/


#define HSHR(ch) (GET_SEX(ch) ? (GET_SEX(ch)==SEX_MALE ? "his":"her") :"its")
#define HSSH(ch) (GET_SEX(ch) ? (GET_SEX(ch)==SEX_MALE ? "he" :"she") : "it")
#define HMHR(ch) (GET_SEX(ch) ? (GET_SEX(ch)==SEX_MALE ? "him":"her") : "it")
#define HSHRD(ch) (SEX_DIS(ch) ? (SEX_DIS(ch)==SEX_MALE ? "his":"her") :"its")
#define HSSHD(ch) (SEX_DIS(ch) ? (SEX_DIS(ch)==SEX_MALE ? "he" :"she") : "it")
#define HMHRD(ch) (SEX_DIS(ch) ? (SEX_DIS(ch)==SEX_MALE ? "him":"her") : "it")

#define ANA(obj) (strchr("aeiouyAEIOUY", *(obj)->name) ? "An" : "A")
#define SANA(obj) (strchr("aeiouyAEIOUY", *(obj)->name) ? "an" : "a")


/* Various macros building up to CAN_SEE */

#define LIGHT_OK(sub)	(!IS_AFFECTED(sub, AFF_BLIND) && \
   (IS_LIGHT((sub)->in_room) || IS_AFFECTED((sub), AFF_INFRAVISION) || \
   PLR_FLAGGED(sub, PLR_FIRE_ON)))

// Checks that the person is hiding and is not fighting, sub doesn't have sense life
// and the skill check is made for hide plus modifiers from improved stealth skill
// and human race bonus
#define HIDE_OK(sub, obj)  ( (IS_NPC(obj)) ? (!IS_AFFECTED(obj, AFF_HIDE) || \
  FIGHTING(obj) || IS_AFFECTED(sub, AFF_SENSE_LIFE)) : \
  (!IS_AFFECTED((obj), AFF_HIDE)) || (FIGHTING(obj)) || \
  (IS_AFFECTED(sub, AFF_SENSE_LIFE)) || (!check_skill(obj, 101, 203)))


#define INVIS_OK(sub, obj) \
 (!IS_AFFECTED((obj),AFF_INVISIBLE) || IS_AFFECTED(sub,AFF_DETECT_INVIS))

#define MORT_CAN_SEE(sub, obj) (LIGHT_OK(sub) && INVIS_OK(sub, obj) && HIDE_OK(sub, obj))

#define IMM_CAN_SEE(sub, obj) \
   (MORT_CAN_SEE(sub, obj) || (!IS_NPC(sub) && PRF_FLAGGED(sub, PRF_HOLYLIGHT)))

#define SELF(sub, obj)  ((sub) == (obj))

/* Can subject see character "obj"? */
#define CAN_SEE(sub, obj) (SELF(sub, obj) || \
   ((GET_REAL_LEVEL(sub) >= (IS_NPC(obj) ? 0 : GET_INVIS_LEV(obj))) && \
   IMM_CAN_SEE(sub, obj)))

/* End of CAN_SEE */

#define INVIS_OK_OBJ(sub, obj) \
  (!IS_OBJ_STAT((obj), ITEM_INVISIBLE) || IS_AFFECTED((sub), AFF_DETECT_INVIS))

#define CAN_SEE_OBJ_CARRIER(sub, obj) \
  ((!obj->carried_by || CAN_SEE(sub, obj->carried_by)) &&       \
   (!obj->worn_by || CAN_SEE(sub, obj->worn_by)))

#define MORT_CAN_SEE_OBJ(sub, obj) \
  (LIGHT_OK(sub) && INVIS_OK_OBJ(sub, obj) && CAN_SEE_OBJ_CARRIER(sub, obj))


#define CAN_SEE_OBJ(sub, obj) \
   (MORT_CAN_SEE_OBJ(sub, obj) || (!IS_NPC(sub) && PRF_FLAGGED((sub), PRF_HOLYLIGHT)))

#define CAN_CARRY_OBJ(ch,obj)  \
   (((IS_CARRYING_W(ch) + GET_OBJ_WEIGHT(obj)) <= CAN_CARRY_W(ch)) &&   \
    ((IS_CARRYING_N(ch) + 1) <= CAN_CARRY_N(ch)))

#define CAN_GET_OBJ(ch, obj)   \
   (CAN_WEAR((obj), ITEM_WEAR_TAKE) && CAN_CARRY_OBJ((ch),(obj)) && \
    CAN_SEE_OBJ((ch),(obj)))


#if 0
#define PERS(ch, vict)   (CAN_SEE(vict, ch) ? GET_NAME_II(ch, vict) : "someone")
#endif

#define OBJS(obj, vict) (CAN_SEE_OBJ((vict), (obj)) ? \
	(obj)->short_description  : "something")

#define OBJT(obj, vict) (CAN_SEE_OBJ((vict), (obj)) ? \
        (obj)->altshort_description  : "something")

#define OBJN(obj, vict) (CAN_SEE_OBJ((vict), (obj)) ? \
	fname((obj)->name) : "something")


#define EXIT(ch, door)  (world[(ch)->in_room].dir_option[door])

#define CAN_GO(ch, door) (EXIT(ch,door) && \
			 (EXIT(ch,door)->to_room != NOWHERE) && \
			 !IS_SET(EXIT(ch, door)->exit_info, EX_CLOSED))


#define CLASS_ABBR(ch)  (IS_NPC(ch) ? "--" : class_abbrevs[(int)GET_CLASS(ch)])
#define RACE_ABBR(ch)   (IS_NPC(ch) ? "--" : race_abbrevs[(int)GET_RACE(ch)])

#define IS_KNIGHT(ch)	(!IS_NPC(ch) && \
				(GET_CLASS(ch) == CLASS_KNIGHT))
#define IS_PALADIN(ch)  (IS_NPC(ch) ? \
                          (GET_CLASS(ch) == NPC_CLASS_PALADIN) : \
                          (GET_CLASS(ch) == CLASS_PALADIN))
#define IS_MAGE(ch)     (IS_NPC(ch) ? \
                         ((GET_CLASS(ch) == NPC_CLASS_MAGE) || \
                          (GET_CLASS(ch) == NPC_CLASS_UNDEAD_MAGE)) : \
                          (GET_CLASS(ch) == CLASS_MAGE))
#define IS_CLERIC(ch)   (IS_NPC(ch) ? \
                         ((GET_CLASS(ch) == NPC_CLASS_CLERIC) || \
                          (GET_CLASS(ch) == NPC_CLASS_UNDEAD_CLERIC)) : \
                          (GET_CLASS(ch) == CLASS_CLERIC))
#define IS_RANGER(ch)   (IS_NPC(ch) ? \
                          (GET_CLASS(ch) == NPC_CLASS_RANGER) : \
                          (GET_CLASS(ch) == CLASS_RANGER))
#define IS_ROGUE(ch)    (!IS_NPC(ch) && \
                                (GET_CLASS(ch) == CLASS_ROGUE))
#define IS_WARRIOR(ch)	(!IS_NPC(ch) && \
				(GET_CLASS(ch) == CLASS_WARRIOR))
#define IS_DRUID(ch)    (IS_NPC(ch) ? \
                           (GET_CLASS(ch) == NPC_CLASS_DRUID) : \
                           (GET_CLASS(ch) == CLASS_DRUID))

#define IS_MONK(ch)     (!IS_NPC(ch) && \
                                (GET_CLASS(ch) == CLASS_MONK))

/* #define IS_CITIZEN(ch)  (name == "andon")  snicker. --Gilly-poo */
#define IS_ADVENTURER(ch)  (!IS_NPC(ch) && \
                           (GET_CLASS(ch) == CLASS_ADVENTURER))

#define IS_UNDEAD(ch)   (IS_NPC(ch) && \
                            ((GET_CLASS(ch) == NPC_CLASS_UNDEAD_MAGE) || \
                             (GET_CLASS(ch) == NPC_CLASS_UNDEAD_CLERIC) || \
                             (GET_CLASS(ch) == NPC_CLASS_UNDEAD_FIGHTER)))


#define IS_HUMAN(ch)		(!IS_NPC(ch) && \
				(GET_RACE(ch) == RACE_HUMAN))
#define IS_KENDER(ch)		(!IS_NPC(ch) && \
				(GET_RACE(ch) == RACE_KENDER))
#define IS_BARBARIAN(ch)	(!IS_NPC(ch) && \
				(GET_RACE(ch) == RACE_BARBARIAN))
#define IS_MINOTAUR(ch)		(!IS_NPC(ch) && \
				(GET_RACE(ch) == RACE_MINOTAUR))
#define IS_HYLAR(ch)            ( ! IS_NPC(ch) && \
                                (GET_RACE(ch) == RACE_HYLAR))
#define IS_DAEWAR(ch)            ( ! IS_NPC(ch) && \
                                (GET_RACE(ch) == RACE_DAEWAR))
#define IS_NEIDAR(ch)            ( ! IS_NPC(ch) && \
                                (GET_RACE(ch) == RACE_NEIDAR))
#define IS_SILVANESTI(ch)            ( ! IS_NPC(ch) && \
                                (GET_RACE(ch) == RACE_SILVANESTI))
#define IS_QUALINESTI(ch)            ( ! IS_NPC(ch) && \
                                (GET_RACE(ch) == RACE_QUALINESTI))
#define IS_KAGONESTI(ch)            ( ! IS_NPC(ch) && \
                                (GET_RACE(ch) == RACE_KAGONESTI))
#define IS_HALFELVEN(ch)            ( ! IS_NPC(ch) && \
                                (GET_RACE(ch) == RACE_HALFELVEN))
#define IS_GNOME(ch)            ( ! IS_NPC(ch) && \
                                (GET_RACE(ch) == RACE_GNOME))



#define OUTSIDE(ch) (!ROOM_FLAGGED((ch)->in_room, ROOM_INDOORS))


/* OS compatibility ******************************************************/


/* there could be some strange OS which doesn't have NULL... */
#ifndef NULL
#define NULL (void *)0
#endif

#if !defined(FALSE)
#define FALSE 0
#endif

#if !defined(TRUE)
#define TRUE  (!FALSE)
#endif

/* defines for fseek */
#ifndef SEEK_SET
#define SEEK_SET	0
#define SEEK_CUR	1
#define SEEK_END	2
#endif

/*
 * NOCRYPT can be defined by an implementor manually in sysdep.h.
 * CIRCLE_CRYPT is a variable that the 'configure' script
 * automatically sets when it determines whether or not the system is
 * capable of encrypting.
 */
#if defined(NOCRYPT) || !defined(CIRCLE_CRYPT)
#define CRYPT(a,b) (a)
#else
#define CRYPT(a,b) ((char *) crypt((a),(b)))
#endif

#define SENDOK(ch) (((IS_NPC(ch) || (ch)->desc ) && (AWAKE(ch) || sleep) && \
                    !(deaf && affected_by_spell(ch, SPELL_DEAFENING_WIND))))

#define RM_BLOOD(rm)   (world[rm].blood)


