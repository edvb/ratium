# ratium version number
VERSION = 0.0.0

### Change the varibles below for your system
# paths
PREFIX = /usr/local
MANPREFIX = ${PREFIX}/share/man

# includes and libs
INCS = -Iinclude
LIBS = -lncurses

# flags
CFLAGS = -std=c11 -Wall ${INCS} -DVERSION=\"$(VERSION)\"
LDFLAGS = ${LIBS}

# compiler and linker
CC = gcc-4.7
