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
CFLAGS = -std=c11 -pedantic -Wall ${INCS} -DVERSION=\"$(VERSION)\"
LDFLAGS = ${LIBS}

# compiler and linker
CC = gcc-4.9

### Makefile

EXE = ratium
SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)

all: options $(EXE)

options:
	@echo $(EXE) build options:
	@echo "CC      = $(CC)"
	@echo "CFLAGS  = $(CFLAGS)"
	@echo "LDFLAGS = $(LDFLAGS)"

.o:
	@echo LD $@
	@$(LD) -o $@ $< $(LDFLAGS)

.c.o:
	@echo CC $<
	@$(CC) -c -o $@ $< $(CFLAGS)

$(EXE): $(OBJ)
	@echo CC -o $@
	@$(CC) -o $@ $(OBJ) $(LDFLAGS)

run: all
	./$(EXE)

clean:
	@echo -n cleaning...
	@rm -f $(OBJ) $(EXE)
	@echo \ done

dist: clean
	@echo -n creating dist tarball...
	@mkdir -p $(EXE)-$(VERSION)
	@cp -R Makefile src/ data/ misc/ \
		$(EXE).1 $(EXE)-$(VERSION)
	@tar -cf $(EXE)-$(VERSION).tar $(EXE)-$(VERSION)
	@gzip $(EXE)-$(VERSION).tar
	@rm -rf $(EXE)-$(VERSION)
	@echo \ done

install: all
	@echo -n installing executable file to $(DESTDIR)$(PREFIX)/bin...
	@mkdir -p $(DESTDIR)$(PREFIX)/bin
	@cp -f $(EXE) $(DESTDIR)$(PREFIX)/bin
	@chmod 755 $(DESTDIR)$(PREFIX)/bin/$(EXE)
	@echo \ done
	@echo -n installing manual page to $(DESTDIR)$(MANPREFIX)/man1...
	@mkdir -p $(DESTDIR)$(MANPREFIX)/man1
	@sed "s/VERSION/$(VERSION)/g" < $(EXE).1 > $(DESTDIR)$(MANPREFIX)/man1/$(EXE).1
	@chmod 644 $(DESTDIR)$(MANPREFIX)/man1/$(EXE).1
	@echo \ done

uninstall:
	@echo -n removing executable file from $(DESTDIR)$(PREFIX)/bin...
	@rm -f $(DESTDIR)$(PREFIX)/bin/$(EXE)
	@echo \ done
	@echo -n removing manual page from $(DESTDIR)$(MANPREFIX)/man1...
	@rm -f $(DESTDIR)$(MANPREFIX)/man1/$(EXE).1
	@echo \ done

update-man:
	@echo -n updating man page $(EXE).1...
	@cat README.md | sed "s/# $(EXE)/# $(EXE) 1\n\n##NAME\n\n$(EXE) /" \
		| md2man-roff > $(EXE).1
	@echo \ done

.PHONY: all options run clean dist install uninstall update-man
