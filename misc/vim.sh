#!/bin/sh

vim src/ratium.h src/main.c \
    -O2 src/maps.h src/map.c src/item.c src/ent.h src/ent.c src/ai.c src/player.c \
    src/data.c

