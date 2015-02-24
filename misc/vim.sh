#!/bin/sh

vim src/ratium.h src/ratium.c \
    -O2 src/map.c src/item.c src/entity.c src/player.c src/inv.c src/data.c

