CC = gcc-4.7
CFLAGS = -std=c11 -Wall -Iinclude
LIBS = -lncurses
EXE = ratium
RM = rm -f

SRC = $(wildcard src/*.c)
OBJECTS = $(SRC:.c=.o)

ratium: $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

run: ratium
	./$(EXE)

clean:
	$(RM) $(OBJECTS) $(EXE)

install: ratium
	cp -v $(EXE) /usr/bin/

uninstall:
	$(RM) /usr/bin/$(EXE)

.PHONY: clean install uninstall
