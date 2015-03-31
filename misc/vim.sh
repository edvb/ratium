#!/bin/sh

vim src/ratium.h src/ratium.c \
    -O2 src/map.c src/item.c src/ent.h src/ent.c src/ai.c src/player.c \
    src/data.c

