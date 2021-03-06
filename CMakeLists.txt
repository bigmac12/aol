cmake_minimum_required(VERSION 3.3)
project(dev)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

#add_custom_target(circle COMMAND circle)
#add_custom_command(TARGET Debug COMMAND make -C /home/d/Documents/workspace/aol/dev/src
#                          CLION_EXE_DIR=/home/d/Documents/workspace/aol/dev/bin)

set(SOURCE_FILES
    cnf/acconfig.h
    src/util/autowiz.c
    src/util/delobjs.c
    src/util/listrent.c
    src/util/mudpasswd.c
    src/util/play2to3.c
    src/util/purgeplay.c
    src/util/shopconv.c
    src/util/showplay.c
    src/util/sign.c
    src/util/split.c
    src/util/wld2html.c
    src/act.comm.c
    src/act.informative.c
    src/act.item.c
    src/act.movement.c
    src/act.offensive.c
    src/act.other.c
    src/act.social.c
    src/act.wizard.c
    src/alias.c
    src/assedit.c
    src/assemblies.c
    src/assemblies.h
    src/autowiz.c
    src/ban.c
    src/boards.c
    src/boards.h
    src/bsd-snprintf.c
    src/bsd-snprintf.h
    src/castle.c
    src/char_descs.c
    src/char_descs.h
    src/clan.c
    src/class.c
    src/color.c
    src/comm.c
    src/comm.h
    src/conf.h
    src/config.c
    src/constants.c
    src/constants.h
    src/context_help.c
    src/db.c
    src/db.h
    src/dg_comm.c
    src/dg_db_scripts.c
    src/dg_event.c
    src/dg_event.h
    src/dg_handler.c
    src/dg_misc.c
    src/dg_mobcmd.c
    src/dg_objcmd.c
    src/dg_olc.c
    src/dg_olc.h
    src/dg_scripts.c
    src/dg_scripts.h
    src/dg_triggers.c
    src/dg_wldcmd.c
    src/diskio.c
    src/diskio.h
    src/fight.c
    src/genmob.c
    src/genmob.h
    src/genobj.c
    src/genobj.h
    src/genolc.c
    src/genolc.h
    src/genshp.c
    src/genshp.h
    src/genwld.c
    src/genwld.h
    src/genzon.c
    src/genzon.h
    src/graph.c
    src/handler.c
    src/handler.h
    src/house.c
    src/house.h
    src/improved-edit.c
    src/improved-edit.h
    src/interpreter.c
    src/interpreter.h
    src/limits.c
    src/magic.c
    src/mail.c
    src/mail.h
    src/Makefile
    src/medit.c
    src/mobact.c
    src/modify.c
    src/oasis.c
    src/oasis.h
    src/objsave.c
    src/oedit.c
    src/olc.c
    src/olc.h
    src/pfdefaults.h
    src/races.c
    src/random.c
    src/redit.c
    src/screen.h
    src/sedit.c
    src/shop.c
    src/shop.h
    src/spec_assign.c
    src/spec_procs.c
    src/spell_parser.c
    src/spells.c
    src/spells.h
    src/structs.h
    src/sysdep.h
    src/tedit.c
    src/tedit.h
    src/telnet.h
    src/utils.c
    src/utils.h
    src/weather.c
    src/zedit.c)

add_executable(circle ${SOURCE_FILES})