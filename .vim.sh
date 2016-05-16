#!/bin/sh

vim src/ratium.c -O2 src/game.c src/map.c src/item.c src/ent.c \
    src/ai.c src/player.c src/data.c src/gfx.c src/util.c \
    src/ratium.h src/map.h src/ent.h src/gfx.h src/util.h \
    -c "bd src/ratium.h src/map.h src/ent.h src/gfx.h src/util.h"

