# Generated automatically from Makefile.in by configure.
# CircleMUD 3.0 Makefile.in - Makefile template used by 'configure'
#

# C compiler to use
CC = gcc

# Any special flags you want to pass to the compiler
MYFLAGS = -Wall

#flags for profiling (see hacker.doc for more information)
PROFILE = 

##############################################################################
# Do Not Modify Anything Below This Line (unless you know what you're doing) #
##############################################################################

BINDIR = ../bin

CFLAGS = -g -O2 $(MYFLAGS) $(PROFILE)

LIBS = -lcrypt -lm

OBJFILES = comm.o act.comm.o act.informative.o act.movement.o act.item.o \
	act.offensive.o act.other.o act.social.o act.wizard.o ban.o boards.o \
	char_descs.o class.o config.o constants.o db.o fight.o graph.o handler.o \
	house.o interpreter.o limits.o magic.o mail.o mobact.o modify.o \
	objsave.o olc.o random.o shop.o spec_assign.o spec_procs.o \
	spell_parser.o spells.o utils.o weather.o redit.o oedit.o zedit.o \
	color.o alias.o races.o medit.o sedit.o \
	dg_comm.o dg_db_scripts.o dg_event.o dg_handler.o dg_mobcmd.o \
	dg_objcmd.o dg_scripts.o dg_triggers.o dg_wldcmd.o dg_olc.o

CXREF_FILES = act.comm.c act.informative.c act.item.c act.movement.c \
	act.offensive.c act.other.c act.social.c act.wizard.c ban.c boards.c \
	char_descs.c class.c comm.c config.c constants.c db.c fight.c graph.c \
	handler.c house.c interpreter.c limits.c magic.c mail.c mobact.c \
	modify.c objsave.c olc.c random.c shop.c spec_assign.c spec_procs.c \
	spell_parser.c spells.c utils.c weather.c redit.c oedit.c zedit.c \
	color.c alias.c races.c medit.c sedit.c \
	dg_comm.c dg_db_scripts.c dg_event.c dg_handler.c dg_mobcmd.c \
	dg_objcmd.c dg_scripts.c dg_triggers.c dg_wldcmd.c dg_olc.c

default: all

all: .accepted
	$(MAKE) $(BINDIR)/circle
	$(MAKE) utils

.accepted:
	@./licheck more

utils: .accepted
	(cd util; $(MAKE) all)
circle:
	$(MAKE) $(BINDIR)/circle

$(BINDIR)/circle : $(OBJFILES)
	$(CC) -o $(BINDIR)/circle $(PROFILE) $(OBJFILES) $(LIBS)

clean:
	rm -f *.o
ref:
#
# Create the cross reference files
# Note, this is not meant to be used unless you've installed cxref...
#
	@for file in $(CXREF_FILES) ; do \
	  echo Cross referencing $$file ; \
	  /home/jelson/cxref/cxref -D__CXREF__ -xref -Odoc -Ncircle $$file ; \
	done
#
# Create the source files using /home/jelson/cxref/cxref
#
	@for file in $(CXREF_FILES) ; do \
	   echo Documenting $$file ; \
	   ( cd . ; /home/jelson/cxref/cxref -D__CXREF__ -warn-xref -xref -Odoc -Ncircle -html $$file ) ; \
	   rm -f $(DOCS) ; \
	done
#
# Create the index using /home/jelson/cxref/cxref
#
	@echo Indexing
	@( cd . ; /home/jelson/cxref/cxref -D__CXREF__ -index-all -Odoc -Ncircle -html )
	@rm -f $(DOCS)
#
# Make html files for the .h files
#
	@echo Creating .h.html files...
	@for file in *.h ; do \
	  echo $$file ; \
	  cat /home/jelson/mud/htmlh-head $$file /home/jelson/mud/htmlh-tail > doc/$$file.html ; \
	done
# Copy over to the html directory
	cp doc/*.html /home/jelson/public_html/circle/cdp/cxref
	chmod 644 /home/jelson/public_html/circle/cdp/cxref/*.html

# Dependencies for the object files (automagically generated with
# gcc -MM)

act.comm.o: act.comm.c conf.h sysdep.h structs.h utils.h comm.h interpreter.h \
  handler.h db.h screen.h dg_scripts.h
	$(CC) -c $(CFLAGS) act.comm.c
act.informative.o: act.informative.c conf.h sysdep.h structs.h utils.h comm.h \
  interpreter.h handler.h db.h spells.h screen.h
	$(CC) -c $(CFLAGS) act.informative.c
act.item.o: act.item.c conf.h sysdep.h structs.h utils.h comm.h interpreter.h \
  handler.h db.h spells.h dg_scripts.h
	$(CC) -c $(CFLAGS) act.item.c
act.movement.o: act.movement.c conf.h sysdep.h structs.h utils.h comm.h \
  interpreter.h handler.h db.h spells.h house.h dg_scripts.h
	$(CC) -c $(CFLAGS) act.movement.c
act.offensive.o: act.offensive.c conf.h sysdep.h structs.h utils.h comm.h \
  interpreter.h handler.h db.h spells.h
	$(CC) -c $(CFLAGS) act.offensive.c
act.other.o: act.other.c conf.h sysdep.h structs.h utils.h comm.h interpreter.h \
  handler.h db.h spells.h screen.h house.h dg_scripts.h
	$(CC) -c $(CFLAGS) act.other.c
act.social.o: act.social.c conf.h sysdep.h structs.h utils.h comm.h \
  interpreter.h handler.h db.h spells.h
	$(CC) -c $(CFLAGS) act.social.c
act.wizard.o: act.wizard.c conf.h sysdep.h structs.h utils.h comm.h \
  interpreter.h handler.h db.h spells.h house.h screen.h dg_scripts.h
	$(CC) -c $(CFLAGS) act.wizard.c
alias.o: alias.c conf.h sysdep.h structs.h utils.h interpreter.h db.h
	$(CC) -c $(CFLAGS) alias.c
ban.o: ban.c conf.h sysdep.h structs.h utils.h comm.h interpreter.h handler.h db.h
	$(CC) -c $(CFLAGS) ban.c
boards.o: boards.c conf.h sysdep.h structs.h utils.h comm.h db.h boards.h \
  interpreter.h handler.h
	$(CC) -c $(CFLAGS) boards.c
char_descs.o: char_descs.c conf.h sysdep.h structs.h db.h utils.h spells.h interpreter.h
	$(CC) -c $(CFLAGS) char_descs.c
class.o: class.c conf.h sysdep.h structs.h db.h utils.h spells.h interpreter.h
	$(CC) -c $(CFLAGS) class.c
color.o: color.c conf.h
	$(CC) -c $(CFLAGS) color.c
comm.o: comm.c conf.h sysdep.h structs.h utils.h comm.h interpreter.h handler.h \
  db.h house.h dg_scripts.h
	$(CC) -c $(CFLAGS) comm.c
config.o: config.c conf.h sysdep.h structs.h
	$(CC) -c $(CFLAGS) config.c
constants.o: constants.c conf.h sysdep.h structs.h
	$(CC) -c $(CFLAGS) constants.c
db.o: db.c conf.h sysdep.h structs.h utils.h db.h comm.h handler.h spells.h mail.h \
  interpreter.h house.h dg_scripts.h
	$(CC) -c $(CFLAGS) db.c
fight.o: fight.c conf.h sysdep.h structs.h utils.h comm.h handler.h interpreter.h \
  db.h spells.h screen.h dg_scripts.h
	$(CC) -c $(CFLAGS) fight.c
graph.o: graph.c conf.h sysdep.h structs.h utils.h comm.h interpreter.h handler.h \
  db.h spells.h
	$(CC) -c $(CFLAGS) graph.c
handler.o: handler.c conf.h sysdep.h structs.h utils.h comm.h db.h handler.h \
  interpreter.h spells.h dg_scripts.h
	$(CC) -c $(CFLAGS) handler.c
house.o: house.c conf.h sysdep.h structs.h comm.h handler.h db.h interpreter.h \
  utils.h house.h
	$(CC) -c $(CFLAGS) house.c
interpreter.o: interpreter.c conf.h sysdep.h structs.h comm.h interpreter.h db.h \
  utils.h spells.h handler.h mail.h screen.h dg_scripts.h
	$(CC) -c $(CFLAGS) interpreter.c
limits.o: limits.c conf.h sysdep.h structs.h utils.h spells.h comm.h db.h \
  handler.h
	$(CC) -c $(CFLAGS) limits.c
magic.o: magic.c conf.h sysdep.h structs.h utils.h comm.h spells.h handler.h db.h
	$(CC) -c $(CFLAGS) magic.c
mail.o: mail.c conf.h sysdep.h structs.h utils.h comm.h db.h interpreter.h \
  handler.h mail.h
	$(CC) -c $(CFLAGS) mail.c
mobact.o: mobact.c conf.h sysdep.h structs.h utils.h db.h comm.h interpreter.h \
  handler.h spells.h
	$(CC) -c $(CFLAGS) mobact.c
modify.o: modify.c conf.h sysdep.h structs.h utils.h interpreter.h handler.h db.h \
  comm.h spells.h mail.h boards.h
	$(CC) -c $(CFLAGS) modify.c
objsave.o: objsave.c conf.h sysdep.h structs.h comm.h handler.h db.h \
  interpreter.h utils.h spells.h
	$(CC) -c $(CFLAGS) objsave.c
olc.o: olc.c conf.h sysdep.h structs.h utils.h comm.h interpreter.h handler.h db.h \
  olc.h screen.h dg_olc.h
	$(CC) -c $(CFLAGS) olc.c
races.o: races.c utils.h sysdep.h conf.h structs.h db.h interpreter.h spells.h
	$(CC) -c $(CFLAGS) races.c
random.o: random.c utils.h
	$(CC) -c $(CFLAGS) random.c
shop.o: shop.c conf.h sysdep.h structs.h comm.h handler.h db.h interpreter.h \
  utils.h shop.h
	$(CC) -c $(CFLAGS) shop.c
spec_assign.o: spec_assign.c conf.h sysdep.h structs.h db.h interpreter.h \
  utils.h
	$(CC) -c $(CFLAGS) spec_assign.c
spec_procs.o: spec_procs.c conf.h sysdep.h structs.h utils.h comm.h \
  interpreter.h handler.h db.h spells.h
	$(CC) -c $(CFLAGS) spec_procs.c
spell_parser.o: spell_parser.c conf.h sysdep.h structs.h utils.h interpreter.h \
  spells.h handler.h comm.h db.h
	$(CC) -c $(CFLAGS) spell_parser.c
spells.o: spells.c conf.h sysdep.h structs.h utils.h comm.h spells.h handler.h \
  db.h
	$(CC) -c $(CFLAGS) spells.c
utils.o: utils.c conf.h sysdep.h structs.h utils.h comm.h screen.h spells.h \
  handler.h
	$(CC) -c $(CFLAGS) utils.c
weather.o: weather.c conf.h sysdep.h structs.h utils.h comm.h handler.h \
  interpreter.h db.h
	$(CC) -c $(CFLAGS) weather.c
redit.o: redit.c conf.h sysdep.h structs.h utils.h comm.h boards.h \
  db.h olc.h dg_olc.h
	$(CC) -c $(CFLAGS) redit.c
oedit.o: oedit.c conf.h sysdep.h structs.h utils.h comm.h boards.h spells.h \
  db.h olc.h shop.h dg_olc.h
	$(CC) -c $(CFLAGS) oedit.c
zedit.o: zedit.c conf.h sysdep.h structs.h utils.h comm.h db.h olc.h
	$(CC) -c $(CFLAGS) zedit.c 
medit.o: medit.c conf.h sysdep.h structs.h utils.h comm.h spells.h shop.h \
  db.h olc.h handler.h dg_olc.h
	$(CC) -c $(CFLAGS) medit.c 
sedit.o: sedit.c conf.h sysdep.h structs.h utils.h comm.h db.h olc.h shop.h
	$(CC) -c $(CFLAGS) sedit.c 
dg_comm.o: dg_comm.c conf.h sysdep.h structs.h dg_scripts.h utils.h comm.h \
  handler.h db.h
	$(CC) -c $(CFLAGS) dg_comm.c
dg_db_scripts.o: dg_db_scripts.c conf.h sysdep.h structs.h dg_scripts.h \
  utils.h comm.h handler.h db.h dg_event.h
	$(CC) -c $(CFLAGS) dg_db_scripts.c
dg_event.o: dg_event.c conf.h sysdep.h dg_event.h utils.h structs.h
	$(CC) -c $(CFLAGS) dg_event.c
dg_handler.o: dg_handler.c conf.h sysdep.h structs.h dg_scripts.h utils.h \
  comm.h handler.h db.h dg_event.h
	$(CC) -c $(CFLAGS) dg_handler.c
dg_mobcmd.o: dg_mobcmd.c conf.h sysdep.h structs.h dg_scripts.h utils.h \
  comm.h handler.h db.h interpreter.h comm.h spells.h
	$(CC) -c $(CFLAGS) dg_mobcmd.c
dg_objcmd.o: dg_objcmd.c conf.h sysdep.h structs.h dg_scripts.h utils.h \
  comm.h handler.h db.h interpreter.h comm.h
	$(CC) -c $(CFLAGS) dg_objcmd.c
dg_scripts.o: dg_scripts.c conf.h sysdep.h structs.h dg_scripts.h utils.h \
  comm.h handler.h db.h comm.h screen.h
	$(CC) -c $(CFLAGS) dg_scripts.c
dg_triggers.o: dg_triggers.c conf.h sysdep.h structs.h dg_scripts.h \
  utils.h comm.h handler.h db.h interpreter.h
	$(CC) -c $(CFLAGS) dg_triggers.c
dg_wldcmd.o: dg_wldcmd.c conf.h sysdep.h structs.h dg_scripts.h utils.h \
  comm.h handler.h db.h comm.h interpreter.h
	$(CC) -c $(CFLAGS) dg_wldcmd.c
dg_olc.o: dg_olc.c conf.h sysdep.h structs.h dg_scripts.h utils.h \
  comm.h db.h olc.h dg_olc.h dg_event.h
	$(CC) -c $(CFLAGS) dg_olc.c

