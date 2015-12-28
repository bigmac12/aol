// FILE: dg_comm.c
// This file contains routines to hand MUD to player communication.

#include "conf.h"
#include "sysdep.h"

#include "structs.h"
#include "dg_scripts.h"
#include "utils.h"
#include "comm.h"
#include "handler.h"
#include "db.h"
#include "spells.h"
#include "constants.h"

extern struct room_data *world;
extern struct descriptor_data *descriptor_list;

/* same as any_one_arg except that it stops at punctuation */
char *any_one_name(char *argument, char *first_arg)
{
    char* arg;

    /* Find first non blank */
    while(isspace(*argument))
        argument++; 
 
    /* Find length of first word */
    for(arg = first_arg ;
        *argument && !isspace(*argument) &&
          (!ispunct(*argument) || *argument == '#' || *argument == '-') ;
        arg++, argument++)
        *arg = LOWER(*argument);
    *arg = '\0';

    return argument;
}


void sub_write_to_char(char_data *ch, char *tokens[], void *otokens[], char type[])
{
    char sb[MAX_STRING_LENGTH];
    int i;

    strcpy(sb,"");

    for (i = 0; tokens[i + 1]; i++)
    {
	strcat(sb,tokens[i]);

	switch (type[i])
	{
	case '~':
	    if (!otokens[i])
		strcat(sb,"someone");
	    else if ((char_data *)otokens[i] == ch)
		strcat(sb,"you");
	    else
            {
                GET_PERS((char_data*) otokens[i], ch, chname);
		strcat(sb, chname);
                FREE_NAME(chname);
            }
	    break;

	case '@':
	    if (!otokens[i])
		strcat(sb,"someone's");
	    else if ((char_data *)otokens[i] == ch)
		strcat(sb,"your");
	    else
	    {
                GET_PERS((char_data*) otokens[i], ch, chname);
		strcat(sb, chname);
                FREE_NAME(chname);
		strcat(sb,"'s");
	    }
	    break;

	case '^':
	    if (!otokens[i] || !CAN_SEE(ch, (char_data *) otokens[i]))
		strcat(sb,"its");
	    else if (otokens[i] == ch)
		strcat(sb,"your");
	    else
		strcat(sb,HSHR((char_data *) otokens[i]));
	    break;
		
	case '&':
	    if (!otokens[i] || !CAN_SEE(ch, (char_data *) otokens[i]))
		strcat(sb,"it");
	    else if (otokens[i] == ch)
		strcat(sb,"you");
	    else
		strcat(sb,HSSH((char_data *) otokens[i]));
	    break;
	    
	case '*':
	    if (!otokens[i] || !CAN_SEE(ch, (char_data *) otokens[i]))
		strcat(sb,"it");
	    else if (otokens[i] == ch)
		strcat(sb,"you");
	    else
		strcat(sb,HMHR((char_data *) otokens[i]));
	    break;

	case '`':
	    if (!otokens[i])
		strcat(sb,"something");
	    else
		strcat(sb,OBJS(((obj_data *) otokens[i]), ch));
	    break;
	}
    }

    strcat(sb,tokens[i]);
    strcat(sb,"\n\r");
    sb[0] = toupper(sb[0]);
    send_to_char(sb,ch);
}


void sub_write(char *arg, char_data *ch, byte find_invis, int targets)
{
    char str[MAX_INPUT_LENGTH * 2];
    char type[MAX_INPUT_LENGTH], name[MAX_INPUT_LENGTH];
    char *tokens[MAX_INPUT_LENGTH], *s, *p;
    void *otokens[MAX_INPUT_LENGTH];
    char_data *to;
    obj_data *obj;
    int i, tmp;
    int sleep = 1, deaf = 0;

    if (!arg)
	return;
    
    tokens[0] = str;
    
    for (i = 0, p = arg, s = str; *p;)
    {
	switch (*p) {
	case '~':
	case '@':
	case '^':
	case '&':
	case '*':
	    /* get char_data, move to next token */
	    type[i] = *p;
	    *s = '\0';
	    p = any_one_name(++p, name);

        // See comment below.
	    //(char_data *)otokens[i] = find_invis ? get_char(name) : get_char_vis(ch, name, FIND_CHAR_ROOM);

        // I'm not sure why "otokens[i]" was cast to a char_data type.
        // Doing so caused GCC to complain, so I removed it.
        // Bran - '11
	    otokens[i] = find_invis ? get_char(name) : get_char_vis(ch, name, FIND_CHAR_ROOM);
	    tokens[++i] = ++s;
	    break;
	    
	case '`':
	    /* get obj_data, move to next token */
	    type[i] = *p;
	    *s = '\0';
	    p = any_one_name(++p, name);
        //(obj_data *)otokens[i] = find_invis ? (obj = get_obj(name)) : 
        otokens[i] = find_invis ? (obj = get_obj(name)) : 
        ((obj = get_obj_in_list_vis(ch, name, world[IN_ROOM(ch)].contents)) ? obj : 
        (obj = get_object_in_equip_vis(ch, name, ch->equipment, &tmp)) ? obj : 
        (obj = get_obj_in_list_vis(ch, name, ch->carrying)));

        //if (find_invis) obj = get_obj_in_room(&world[IN_ROOM(ch)], name);
        //else if (!(obj = get_obj_in_list_vis(ch, name, world[IN_ROOM(ch)].contents))) ;
        //else if (!(obj = get_object_in_equip_vis(ch, name, &tmp, ch->equipment)));
        //else obj = get_obj_in_list_vis(ch, name, ch->carrying);

	    //(obj_data *)otokens[i] = obj;

        // Once again I'm not sure why this was cast.
        // See comment above.
	    otokens[i] = obj;
	    tokens[++i] = ++s;
	    break;

	case '\\':
	    p++;
	    *s++ = *p++;
	    break;
	    
	default:
	    *s++ = *p++;
	}
    }

    *s = '\0';
    tokens[++i] = NULL;

    if (IS_SET(targets, TO_CHAR) && SENDOK(ch))
	sub_write_to_char(ch, tokens, otokens, type);

    if (IS_SET(targets, TO_ROOM))
	for (to = world[ch->in_room].people;
	     to; to = to->next_in_room)
	    if (to != ch && SENDOK(to))
		sub_write_to_char(to, tokens, otokens, type);
}

void send_to_zone(char *messg, int zone_rnum)
{
  struct descriptor_data *i;

  if (!messg || !*messg)
    return;

  for (i = descriptor_list; i; i = i->next)
    if (STATE(i) == CON_PLAYING && i->character && AWAKE(i->character) &&
        (IN_ROOM(i->character) != NOWHERE) &&
        (world[IN_ROOM(i->character)].zone == zone_rnum))
      SEND_TO_Q(messg, i);
}
