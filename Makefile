# ratium
# See LICENSE file for copyright and license details.

include config.mk

EXE = ratium
SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)
LUA = $(wildcard src/*.lua)
LUAH = $(LUA:.lua=.lua.h)

all: options $(EXE) $(DESTDIR)$(PREF)

options:
	@echo $(EXE) build options:
	@echo "CFLAGS  = $(CFLAGS)"
	@echo "LDFLAGS = $(LDFLAGS)"

%.lua.h: %.lua
	@echo xxd $<
	@xxd -i $< $@

.o:
	@echo $(LD) $@
	@$(LD) -o $@ $< $(LDFLAGS)

.c.o:
	@echo $(CC) $<
	@$(CC) -c -o $@ $< $(CFLAGS)

$(EXE): $(LUAH) $(OBJ)
	@echo $(CC) -o $@
	@$(CC) -o $@ $(OBJ) $(LDFLAGS)

run: all
	./$(EXE)

clean:
	@echo cleaning
	@rm -f $(LUAH) $(OBJ) $(EXE)

dist: clean
	@echo creating dist tarball
	@mkdir -p $(EXE)-$(VERSION)
	@cp -R LICENSE README.md Makefile src/ packs/ $(EXE).1 $(EXE)-$(VERSION)
	@tar -cf $(EXE)-$(VERSION).tar $(EXE)-$(VERSION)
	@gzip $(EXE)-$(VERSION).tar
	@rm -rf $(EXE)-$(VERSION)

install: all
	@echo installing executable file to $(DESTDIR)$(PREFIX)/bin
	@mkdir -p $(DESTDIR)$(PREFIX)/bin
	@cp -f $(EXE) $(DESTDIR)$(PREFIX)/bin
	@chmod 755 $(DESTDIR)$(PREFIX)/bin/$(EXE)
	@echo installing manual page to $(DESTDIR)$(MANPREFIX)/man1
	@mkdir -p $(DESTDIR)$(MANPREFIX)/man1
	@sed "s/VERSION/$(VERSION)/g" < $(EXE).1 > $(DESTDIR)$(MANPREFIX)/man1/$(EXE).1
	@chmod 644 $(DESTDIR)$(MANPREFIX)/man1/$(EXE).1

$(DESTDIR)$(PREF): $(shell find packs | sed "s/\ /\\\ /g")
	@echo installing default pack to $(DESTDIR)$(PREF)/
	@mkdir -p $(DESTDIR)$(PREF)
	@cp -fr packs/ $(DESTDIR)$(PREF)
	@touch $(DESTDIR)$(PREF)

uninstall:
	@echo removing executable file from $(DESTDIR)$(PREFIX)/bin
	@rm -f $(DESTDIR)$(PREFIX)/bin/$(EXE)
	@echo removing manual page from $(DESTDIR)$(MANPREFIX)/man1
	@rm -f $(DESTDIR)$(MANPREFIX)/man1/$(EXE).1

update-man:
	@echo updating man page $(EXE).1
	@cat README.md | sed "s/# $(EXE)/# $(EXE) 1\n\n##NAME\n\n$(EXE) /" | \
		md2man-roff | sed "s/\\[la\]/\</" | sed "s/\\[ra\]/\>/" > $(EXE).1
	@echo \ done

.PHONY: all options run clean dist install uninstall update-man
