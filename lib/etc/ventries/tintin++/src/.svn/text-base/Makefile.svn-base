# Generated automatically from Makefile.in by configure.
#
# Makefile for TINTIN++ 
# 

# You may need to change some of this Makefile if you have trouble compiling.
# Type 'make help' for a few instructions, if you like.
# If you change the Makefile, you *may* need to type 'make clean', then 'make'.

# If you have troubles compiling, or prefer the gcc compiler,
# try uncommenting the 'gcc' line and commenting the 'cc' one.
# Tintin++ doesn't *need* an ANSI compiler anymore, but gcc
# is still better than cc on many platforms...
CC = gcc
CFLAGS = -g -DCOMPRESSED_HELP=0 -DDIRENT=1 -DHAVE_STRINGS_H=1 -DHAVE_STRING_H=1 -DHAVE_STDLIB_H=1 -DHAVE_TIME_H=1 -DHAVE_BCOPY=1 -DHAVE_UNISTD_H=1 -DHAVE_TERMIO_H=1
LIBS =  -lnsl
PIPE = -pipe
# If you plan on doing debugging (with gdb), it is very helpful to turn all
# the alarms off so that you can step through the code without having to 
# jump to the ticker function.  This will also disable the internal
# tickcounter.
F4 = -DDEBUG

# BINDIR is the directory you wish tt++ to be placed if you wish to use
# make install.  
BINDIR = ..

# DEFAULT_FILE_DIR is the path to tintin files. 
DEFAULT_FILE_DIR = @HOME@

#########################################
# You shouldn't need to change anything #
# below this line.                      #
#########################################
# GREP-FOR-ME (this is for gripe)       #
#########################################

FFLAGS= $(F4)
LFLAGS= 
CFILES = main.c parse.c action.c alias.c substitute.c session.c \
	files.c history.c ticks.c misc.c path.c net.c llist.c utils.c echo.c \
	variables.c highlight.c antisub.c ivars.c help.c text.c glob.c
OFILES = $(CFILES:.c=.o)

all: tintin++

tintin++: $(OFILES) tintin.h
	@echo "Linking..."
	$(CC) $(CFLAGS) $(FFLAGS) $(LFLAGS) -o tt++ $(OFILES) $(LIBS)

# install: all
#	@./install.sh $(BINDIR) $(DEFAULT_FILE_DIR) $(COMPRESSED_HELP)

# Autocompile all .c files into .o files using this rule:
.c.o:
	$(CC) $(PIPE) $(CFLAGS) $(FFLAGS) -c $<

clean:
	rm -f *.o ../tt++ tt++ core

dist: 
	@/bin/sh ./dist.sh

depend:
	makedepend -o ".o" $(CFILES)

# DO NOT DELETE THIS LINE -- make depend depends on it.

main.o: /usr/include/signal.h /usr/include/features.h
main.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
main.o: /usr/include/gnu/types.h /usr/include/sigset.h /usr/include/signum.h
main.o: /usr/include/sigaction.h /usr/include/sigcontext.h
main.o: /usr/include/asm/sigcontext.h
main.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stddef.h tintin.h
main.o: /usr/include/stdio.h /usr/include/libio.h /usr/include/_G_config.h
main.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stdarg.h
main.o: /usr/include/stdio_lim.h /usr/include/fcntl.h
main.o: /usr/include/fcntlbits.h /usr/include/sys/types.h /usr/include/time.h
main.o: /usr/include/endian.h /usr/include/bytesex.h
main.o: /usr/include/sys/select.h /usr/include/selectbits.h
parse.o: /usr/include/ctype.h /usr/include/features.h
parse.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
parse.o: /usr/include/endian.h /usr/include/bytesex.h /usr/include/signal.h
parse.o: /usr/include/gnu/types.h /usr/include/sigset.h /usr/include/signum.h
parse.o: /usr/include/sigaction.h /usr/include/sigcontext.h
parse.o: /usr/include/asm/sigcontext.h
parse.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stddef.h tintin.h
parse.o: /usr/include/stdio.h /usr/include/libio.h /usr/include/_G_config.h
parse.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stdarg.h
parse.o: /usr/include/stdio_lim.h
action.o: /usr/include/ctype.h /usr/include/features.h
action.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
action.o: /usr/include/endian.h /usr/include/bytesex.h tintin.h
action.o: /usr/include/stdio.h /usr/include/libio.h /usr/include/_G_config.h
action.o: /usr/include/gnu/types.h
action.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stddef.h
action.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stdarg.h
action.o: /usr/include/stdio_lim.h
alias.o: tintin.h /usr/include/stdio.h /usr/include/libio.h
alias.o: /usr/include/features.h /usr/include/sys/cdefs.h
alias.o: /usr/include/gnu/stubs.h /usr/include/_G_config.h
alias.o: /usr/include/gnu/types.h
alias.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stddef.h
alias.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stdarg.h
alias.o: /usr/include/stdio_lim.h
substitute.o: tintin.h /usr/include/stdio.h /usr/include/libio.h
substitute.o: /usr/include/features.h /usr/include/sys/cdefs.h
substitute.o: /usr/include/gnu/stubs.h /usr/include/_G_config.h
substitute.o: /usr/include/gnu/types.h
substitute.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stddef.h
substitute.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stdarg.h
substitute.o: /usr/include/stdio_lim.h
session.o: /usr/include/ctype.h /usr/include/features.h
session.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
session.o: /usr/include/endian.h /usr/include/bytesex.h tintin.h
session.o: /usr/include/stdio.h /usr/include/libio.h /usr/include/_G_config.h
session.o: /usr/include/gnu/types.h
session.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stddef.h
session.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stdarg.h
session.o: /usr/include/stdio_lim.h
files.o: tintin.h /usr/include/stdio.h /usr/include/libio.h
files.o: /usr/include/features.h /usr/include/sys/cdefs.h
files.o: /usr/include/gnu/stubs.h /usr/include/_G_config.h
files.o: /usr/include/gnu/types.h
files.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stddef.h
files.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stdarg.h
files.o: /usr/include/stdio_lim.h
history.o: /usr/include/ctype.h /usr/include/features.h
history.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
history.o: /usr/include/endian.h /usr/include/bytesex.h tintin.h
history.o: /usr/include/stdio.h /usr/include/libio.h /usr/include/_G_config.h
history.o: /usr/include/gnu/types.h
history.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stddef.h
history.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stdarg.h
history.o: /usr/include/stdio_lim.h
ticks.o: /usr/include/ctype.h /usr/include/features.h
ticks.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
ticks.o: /usr/include/endian.h /usr/include/bytesex.h /usr/include/signal.h
ticks.o: /usr/include/gnu/types.h /usr/include/sigset.h /usr/include/signum.h
ticks.o: /usr/include/sigaction.h /usr/include/sigcontext.h
ticks.o: /usr/include/asm/sigcontext.h
ticks.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stddef.h tintin.h
ticks.o: /usr/include/stdio.h /usr/include/libio.h /usr/include/_G_config.h
ticks.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stdarg.h
ticks.o: /usr/include/stdio_lim.h
misc.o: /usr/include/ctype.h /usr/include/features.h /usr/include/sys/cdefs.h
misc.o: /usr/include/gnu/stubs.h /usr/include/endian.h /usr/include/bytesex.h
misc.o: tintin.h /usr/include/stdio.h /usr/include/libio.h
misc.o: /usr/include/_G_config.h /usr/include/gnu/types.h
misc.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stddef.h
misc.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stdarg.h
misc.o: /usr/include/stdio_lim.h
path.o: tintin.h /usr/include/stdio.h /usr/include/libio.h
path.o: /usr/include/features.h /usr/include/sys/cdefs.h
path.o: /usr/include/gnu/stubs.h /usr/include/_G_config.h
path.o: /usr/include/gnu/types.h
path.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stddef.h
path.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stdarg.h
path.o: /usr/include/stdio_lim.h
net.o: /usr/include/ctype.h /usr/include/features.h /usr/include/sys/cdefs.h
net.o: /usr/include/gnu/stubs.h /usr/include/endian.h /usr/include/bytesex.h
net.o: /usr/include/sys/errno.h /usr/include/errno.h /usr/include/errnos.h
net.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
net.o: /usr/include/sys/types.h /usr/include/gnu/types.h /usr/include/time.h
net.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stddef.h
net.o: /usr/include/sys/select.h /usr/include/selectbits.h
net.o: /usr/include/sys/socket.h /usr/include/socketbits.h
net.o: /usr/include/sockaddrcom.h /usr/include/asm/socket.h
net.o: /usr/include/asm/sockios.h /usr/include/netinet/in.h
net.o: /usr/include/netdb.h /usr/include/rpc/netdb.h tintin.h
net.o: /usr/include/stdio.h /usr/include/libio.h /usr/include/_G_config.h
net.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stdarg.h
net.o: /usr/include/stdio_lim.h
llist.o: tintin.h /usr/include/stdio.h /usr/include/libio.h
llist.o: /usr/include/features.h /usr/include/sys/cdefs.h
llist.o: /usr/include/gnu/stubs.h /usr/include/_G_config.h
llist.o: /usr/include/gnu/types.h
llist.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stddef.h
llist.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stdarg.h
llist.o: /usr/include/stdio_lim.h
utils.o: tintin.h /usr/include/stdio.h /usr/include/libio.h
utils.o: /usr/include/features.h /usr/include/sys/cdefs.h
utils.o: /usr/include/gnu/stubs.h /usr/include/_G_config.h
utils.o: /usr/include/gnu/types.h
utils.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stddef.h
utils.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stdarg.h
utils.o: /usr/include/stdio_lim.h
echo.o: tintin.h /usr/include/stdio.h /usr/include/libio.h
echo.o: /usr/include/features.h /usr/include/sys/cdefs.h
echo.o: /usr/include/gnu/stubs.h /usr/include/_G_config.h
echo.o: /usr/include/gnu/types.h
echo.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stddef.h
echo.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stdarg.h
echo.o: /usr/include/stdio_lim.h /usr/include/sgtty.h
echo.o: /usr/include/sys/ioctl.h /usr/include/ioctls.h
echo.o: /usr/include/asm/ioctls.h /usr/include/asm/ioctl.h
echo.o: /usr/include/linux/sockios.h /usr/include/asm/sockios.h
echo.o: /usr/include/ioctl-types.h /usr/include/sys/ttydefaults.h
variables.o: /usr/include/ctype.h /usr/include/features.h
variables.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
variables.o: /usr/include/endian.h /usr/include/bytesex.h tintin.h
variables.o: /usr/include/stdio.h /usr/include/libio.h
variables.o: /usr/include/_G_config.h /usr/include/gnu/types.h
variables.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stddef.h
variables.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stdarg.h
variables.o: /usr/include/stdio_lim.h
highlight.o: /usr/include/ctype.h /usr/include/features.h
highlight.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
highlight.o: /usr/include/endian.h /usr/include/bytesex.h tintin.h
highlight.o: /usr/include/stdio.h /usr/include/libio.h
highlight.o: /usr/include/_G_config.h /usr/include/gnu/types.h
highlight.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stddef.h
highlight.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stdarg.h
highlight.o: /usr/include/stdio_lim.h
antisub.o: tintin.h /usr/include/stdio.h /usr/include/libio.h
antisub.o: /usr/include/features.h /usr/include/sys/cdefs.h
antisub.o: /usr/include/gnu/stubs.h /usr/include/_G_config.h
antisub.o: /usr/include/gnu/types.h
antisub.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stddef.h
antisub.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stdarg.h
antisub.o: /usr/include/stdio_lim.h
ivars.o: tintin.h /usr/include/stdio.h /usr/include/libio.h
ivars.o: /usr/include/features.h /usr/include/sys/cdefs.h
ivars.o: /usr/include/gnu/stubs.h /usr/include/_G_config.h
ivars.o: /usr/include/gnu/types.h
ivars.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stddef.h
ivars.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stdarg.h
ivars.o: /usr/include/stdio_lim.h /usr/include/ctype.h /usr/include/endian.h
ivars.o: /usr/include/bytesex.h
help.o: /usr/include/ctype.h /usr/include/features.h /usr/include/sys/cdefs.h
help.o: /usr/include/gnu/stubs.h /usr/include/endian.h /usr/include/bytesex.h
help.o: tintin.h /usr/include/stdio.h /usr/include/libio.h
help.o: /usr/include/_G_config.h /usr/include/gnu/types.h
help.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stddef.h
help.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stdarg.h
help.o: /usr/include/stdio_lim.h
text.o: tintin.h /usr/include/stdio.h /usr/include/libio.h
text.o: /usr/include/features.h /usr/include/sys/cdefs.h
text.o: /usr/include/gnu/stubs.h /usr/include/_G_config.h
text.o: /usr/include/gnu/types.h
text.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stddef.h
text.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stdarg.h
text.o: /usr/include/stdio_lim.h
