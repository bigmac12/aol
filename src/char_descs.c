/* 2007 Stephen Squires */
// Updated for grammatical correctness.
// -- Branc, 12/30/2011

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

// This should have been extern to avoid changing the size of the symbol. -- Branc, 12/30/2011
extern const char *pc_race_types[]; 
extern const char *genders[];

const char *eye_descriptions[];
char *nose_descriptions[];
char *ear_descriptions[];
char *face_descriptions[];
char *scar_descriptions[];
char *hair_descriptions[];
char *build_descriptions[];
char *complexion_descriptions[];

const char *eye_descriptions[] = {
  "undefined",
  "blue",
  "green",
  "brown",
  "hazel",
  "aquamarine",
  "emerald",
  "sapphire",
  "chocolate",
  "honey-tinged",
  "wild",
  "crazed",
  "gleaming",
  "intelligent",
  "wise",
  "shimmering",
  "stern",
  "calm",
  "serene",
  "serious",
  "sad",
  "cheerful",
  "\n"
};

char *nose_descriptions[] = {
  "undefined",
  "big",
  "large",
  "small",
  "button",
  "crooked",
  "hooked",
  "hawk-like",
  "bulbous",
  "thin",
  "long",
  "misshapen",
  "\n"
};

char *ear_descriptions[] = {

  "undefined",
  "large",
  "big",
  "long",
  "small",
  "prominent",
  "misshapen",
  "pointed",
  "scarred",
  "tattooed",
  "pierced",
  "\n"
};


char *face_descriptions[] = {

  "undefined",
  "handsome",
  "pretty",
  "ugly",
  "attractive",
  "comely",
  "unattractive",
  "scarred",
  "monstrous",
  "fierce",
  "mouse-like",
  "hawk-like",
  "rat-like",
  "horse-like",
  "bullish",
  "fine",
  "noble",
  "chubby",
  "\n"
};

char *scar_descriptions[] = {

  "undefined",
  "a scar on the right cheek",
  "a scar on the left cheek",
  "a scar across the left eye",
  "a scar across the right eye",
  "a scar across the chin",
  "a scar across the nose",
  "a scar across the neck",
  "scars all over the face",
  "a scar on the left hand",
  "a scar on the right hand",
  "a scar on the left arm",
  "a scar on the right arm",
  "a scar across the chest",
  "a scar across the torso",
  "scars all over the body",
  "a scar on the back",
  "scars all over the back",  
  "a scar on the left leg",
  "a scar on the right leg",
  "a scar on the left foot",
  "a scar on the right foot",
  "scars all over the legs",
  "\n"
};

char *hair_descriptions[] = {

  "undefined",
  "short red hair",
  "long red hair",
  "cropped red hair",
  "a red pony-tail",
  "a red topknot",
  "a crown of red hair",
  "short auburn hair",
  "long auburn hair",
  "cropped auburn hair",
  "an auburn pony-tail",
  "an auburn topknot",
  "an auburn crown of hair",
  "short blonde hair",
  "long blonde hair",
  "cropped blonde hair",
  "a blonde pony-tail",
  "a blonde topknot",
  "a crown of blonde hair",
  "short sandy-blonde hair",
  "long sandy-blonde hair",
  "cropped sandy-blonde hair",
  "a sandy-blonde pony-tail",
  "a sandy-blonde topknot",
  "a crown of sandy-blonde hair",
  "short brown hair",
  "long brown hair",
  "cropped brown hair",
  "a brown pony-tail",
  "a brown topknot",
  "a crown of brown hair",
  "short black hair",
  "long black hair",
  "cropped black hair",
  "a black pony-tail",
  "a black topknot",
  "a crown of black hair",
  "\n"
};

char *build_descriptions[] = {

  "undefined",
  "tall",
  "short",
  "stocky",
  "average",
  "huge",
  "monstrous",
  "gigantic",
  "tiny",
  "delicate",
  "frail",
  "towering",
  "snake-like",
  "wiry",
  "bulging",
  "obese",
  "chubby",
  "\n"
};

char *complexion_descriptions[] = {

  "undefined",
  "white",
  "creamy",
  "dark",
  "tanned",
  "bronzed",
  "freckled",
  "weathered",
  "brown",
  "black",
  "chocolate",
  "olive",
  "fair",
  "\n"
};

char *current_short_desc(struct char_data *ch) {

  int i = 0;
  char desc[100];

  if (IS_APPROVED(ch)) {
    return strdup(ch->player.short_descr);
  }

  sprintf(desc, "a %s %s", genders[(int) GET_SEX(ch)], pc_race_types[(int) GET_RACE(ch)]);

  switch (GET_PC_DESCRIPTOR_1(ch)) {

  case FEATURE_TYPE_EYES:
    sprintf(desc, "%s with %s eyes", desc, eye_descriptions[GET_PC_ADJECTIVE_1(ch)]);
    break;
  case FEATURE_TYPE_NOSE:
    sprintf(desc, "%s with %s %s nose", desc, AN(nose_descriptions[GET_PC_ADJECTIVE_1(ch)]), nose_descriptions[GET_PC_ADJECTIVE_1(ch)]);
    break;
  case FEATURE_TYPE_EARS:
    sprintf(desc, "%s with %s ear", desc, ear_descriptions[GET_PC_ADJECTIVE_1(ch)]);
    break;
  case FEATURE_TYPE_FACE:
    sprintf(desc, "%s with %s features", desc, face_descriptions[GET_PC_ADJECTIVE_1(ch)]);
    break;
  case FEATURE_TYPE_SCAR:
    sprintf(desc, "%s with %s", desc, scar_descriptions[GET_PC_ADJECTIVE_1(ch)]);
    break;
  case FEATURE_TYPE_HAIR:
    sprintf(desc, "%s with %s", desc, hair_descriptions[GET_PC_ADJECTIVE_1(ch)]);
    break;
  case FEATURE_TYPE_BUILD:
    sprintf(desc, "%s with %s %s frame", desc, AN(build_descriptions[GET_PC_ADJECTIVE_1(ch)]), build_descriptions[GET_PC_ADJECTIVE_1(ch)]);
    break;
  case FEATURE_TYPE_COMPLEXION:
    sprintf(desc, "%s with %s %s complexion", desc, AN(complexion_descriptions[GET_PC_ADJECTIVE_1(ch)]), complexion_descriptions[GET_PC_ADJECTIVE_1(ch)]);
    break;
  
  }
  
  switch (GET_PC_DESCRIPTOR_2(ch)) {

  case FEATURE_TYPE_EYES:
    sprintf(desc, "%s and %s eyes", desc, eye_descriptions[GET_PC_ADJECTIVE_2(ch)]);
    break;
  case FEATURE_TYPE_NOSE:
    sprintf(desc, "%s and %s %s nose", desc, AN(nose_descriptions[GET_PC_ADJECTIVE_2(ch)]), nose_descriptions[GET_PC_ADJECTIVE_2(ch)]);
    break;
  case FEATURE_TYPE_EARS:
    sprintf(desc, "%s and %s eyar", desc, ear_descriptions[GET_PC_ADJECTIVE_2(ch)]);
    break;
  case FEATURE_TYPE_FACE:
    sprintf(desc, "%s and %s features", desc, face_descriptions[GET_PC_ADJECTIVE_2(ch)]);
    break;
  case FEATURE_TYPE_SCAR:
    sprintf(desc, "%s and %s", desc, scar_descriptions[GET_PC_ADJECTIVE_2(ch)]);
    break;
  case FEATURE_TYPE_HAIR:
    sprintf(desc, "%s and %s", desc, hair_descriptions[GET_PC_ADJECTIVE_2(ch)]);
    break;
  case FEATURE_TYPE_BUILD:
    sprintf(desc, "%s and %s %s frame", desc, AN(build_descriptions[GET_PC_ADJECTIVE_2(ch)]), build_descriptions[GET_PC_ADJECTIVE_2(ch)]);
    break;
  case FEATURE_TYPE_COMPLEXION:
    sprintf(desc, "%s and %s %s complexion", desc, AN(complexion_descriptions[GET_PC_ADJECTIVE_2(ch)]), complexion_descriptions[GET_PC_ADJECTIVE_2(ch)]);
    break;
  
  }
  
  for (i = 0; i < strlen(desc); i++)
    desc[i] = tolower(desc[i]);

  return strdup(desc);

}

void short_desc_descriptors_menu(struct char_data *ch) {

  SEND_TO_Q("Please choose a descriptor from the list.  This will determine what kind of feature\r\n"
               "you wish to add to your short description.  Once chosen, you will choose a specific\r\n"
               "adjective for that feature.\r\n\r\n", ch->desc);

  SEND_TO_Q("1) Describe Eyes\r\n"
            "2) Describe Nose\r\n"
            "3) Describe Ears\r\n"
            "4) Describe Face\r\n"
            "5) Describe Scars\r\n"
            "6) Describe Hair\r\n"
            "7) Describe Build\r\n"
            "8) Describe Complexion\r\n\r\n", ch->desc);

}

void short_desc_adjectives_menu(struct char_data *ch, int which_desc) {

  char buf[100];
  int i = 0;

  SEND_TO_Q("Please choose an adjective for the descriptor you just chose.\r\n\r\n", ch->desc);


  switch (which_desc) {
  case FEATURE_TYPE_EYES:
    while (i < NUM_EYE_DESCRIPTORS) {
      sprintf(buf, "%d) %-30s ", i, eye_descriptions[i]);
      if (i % 2 == 1)
        sprintf(buf, "%s\r\n", buf);
      SEND_TO_Q(buf, ch->desc);
      i++;
    }
    break;
  case FEATURE_TYPE_NOSE:
    while (i < NUM_NOSE_DESCRIPTORS) {
      sprintf(buf, "%d) %-30s ", i, nose_descriptions[i]);
      if (i % 2 == 1)
        sprintf(buf, "%s\r\n", buf);
      SEND_TO_Q(buf, ch->desc);
      i++;
    }
    break;
  case FEATURE_TYPE_EARS:
    while (i < NUM_EAR_DESCRIPTORS) {
      sprintf(buf, "%d) %-30s n", i, ear_descriptions[i]);
      if (i % 2 == 1)
        sprintf(buf, "%s\r\n", buf);
      SEND_TO_Q(buf, ch->desc);
      i++;
    }
    break;
  case FEATURE_TYPE_FACE:      
    while (i < NUM_FACE_DESCRIPTORS) {
      sprintf(buf, "%d) %-30s ", i, face_descriptions[i]);
      if (i % 2 == 1)
        sprintf(buf, "%s\r\n", buf);
      SEND_TO_Q(buf, ch->desc);
      i++;
    }
    break;
  case FEATURE_TYPE_SCAR:      
    while (i < NUM_SCAR_DESCRIPTORS) {
      sprintf(buf, "%d) %-30s ", i, scar_descriptions[i]);
      if (i % 2 == 1)
        sprintf(buf, "%s\r\n", buf);
      SEND_TO_Q(buf, ch->desc);
      i++;
    }
    break;
  case FEATURE_TYPE_HAIR:      
    while (i < NUM_HAIR_DESCRIPTORS) {
      sprintf(buf, "%d) %-30s ", i, hair_descriptions[i]);
      if (i % 2 == 1)
        sprintf(buf, "%s\r\n", buf);
      SEND_TO_Q(buf, ch->desc);
      i++;
    }
    break;
  case FEATURE_TYPE_BUILD:     
    while (i < NUM_BUILD_DESCRIPTORS) {
      sprintf(buf, "%d) %-30s ", i, build_descriptions[i]); 
      if (i % 2 == 1)
        sprintf(buf, "%s\r\n", buf);
      SEND_TO_Q(buf, ch->desc);
      i++;
    }
    break;
  case FEATURE_TYPE_COMPLEXION:
    while (i < NUM_COMPLEXION_DESCRIPTORS) {
      sprintf(buf, "%d) %-30s ", i, complexion_descriptions[i]);
      if (i % 2 == 1)
        sprintf(buf, "%s\r\n", buf);
      SEND_TO_Q(buf, ch->desc);
      i++;
    }
    break;
  }
  
  if (i % 2 == 0)
    SEND_TO_Q("\r\n", ch->desc); 

  SEND_TO_Q("\r\n", ch->desc);

}


int count_adjective_types(int which_desc) {

  int i = 0;

  switch (which_desc) {
  case FEATURE_TYPE_EYES:
    while (i < NUM_EYE_DESCRIPTORS)
      i++;
    break;
  case FEATURE_TYPE_NOSE:
    while (i < NUM_NOSE_DESCRIPTORS)
      i++;
    break;
  case FEATURE_TYPE_EARS:
    while (i < NUM_EAR_DESCRIPTORS)
      i++;
    break;
  case FEATURE_TYPE_FACE:      
    while (i < NUM_FACE_DESCRIPTORS)
      i++;
    break;
  case FEATURE_TYPE_SCAR:      
    while (i < NUM_SCAR_DESCRIPTORS)
      i++;
    break;
  case FEATURE_TYPE_HAIR:      
    while (i < NUM_HAIR_DESCRIPTORS)
      i++;
    break;
  case FEATURE_TYPE_BUILD:     
    while (i < NUM_BUILD_DESCRIPTORS)
      i++;
    break;
  case FEATURE_TYPE_COMPLEXION:
    while (i < NUM_COMPLEXION_DESCRIPTORS)
      i++;
    break;
  }

  return i;

}
