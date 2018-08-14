# ratium version number
VERSION = 0.0.0

### Change the variables below for your system

# paths
PREFIX = /usr/local
MANPREFIX = $(PREFIX)/share/man
PREF = $(HOME)/.local/share/$(EXE)

# includes and libraries
INCS = -Iinclude
LIBS = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -llua5.3

# flags
CPPFLAGS = -DVERSION=\"$(VERSION)\" -D_DEFAULT_SOURCE -D_BSD_SOURCE -D_XOPEN_SOURCE=600
CFLAGS = -g -std=c99 -pedantic -Wall ${INCS} ${CPPFLAGS}
LDFLAGS = ${LIBS}

# compiler and linker
CC = gcc
