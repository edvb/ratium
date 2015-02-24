include config.mk

EXE = ratium
SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)

all: options ratium

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

ratium: $(OBJ)
	@echo CC -o $@
	@$(CC) -o $@ $(OBJ) $(LDFLAGS)

run: all
	./$(EXE)

clean:
	@echo cleaning
	@rm -f $(OBJ) $(EXE)

dist: clean
	@echo creating dist tarball
	@mkdir -p $(EXE)-$(VERSION)
	@cp -R Makefile config.mk src/ data/ misc/ \
		$(EXE).1 $(EXE)-$(VERSION)
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

uninstall:
	@echo removing executable file from $(DESTDIR)$(PREFIX)/bin
	@rm -f $(DESTDIR)$(PREFIX)/bin/$(EXE)
	@echo removing manual page from $(DESTDIR)$(MANPREFIX)/man1
	@rm -f $(DESTDIR)$(MANPREFIX)/man1/$(EXE).1

.PHONY: all options run clean install uninstall
