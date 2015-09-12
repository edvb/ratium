# NAME

ratium - A Dumb Little NCurses ASCII Game

## SYNOPSIS

ratium [--help] [--version]

## DESCRIPTION

ratium  is a dumb little game where you are a '@' and have to survive in an
ASCII world by defending yourself from various enemies, like the super rat.
Collect items such as spam, swords, and haggises to aid you in your battles.

## COMMANDS

To quit the game press ESC, you can also press ctrl-z to suspend the game.

### MOVEMENT

 * **h**: Move player left.
 * **j**: Move player down.
 * **k**: Move player up.
 * **l**: Move player right.

### PLAYER ACTIONS

 * **g**: Get item under player.
 * **o**: Open/close door.
 * **i**: Open inventory menu.

### INVENTORY
 * **i**: Exit from inventory menu.
 * **g**: Use item, depending on item type you can eat it or equip it in hand.
 * **d**: Drop item onto map.

## ENTITIES
 * Rat (r)
   * A hostile creature which deals 1 damage and has 2 heath.
   * Drops rat meat when it dies.
 * Super Rat (R)
   * A hostile creature which deals 2 damage and has 4 heath.
   * Drops rat meat when it dies.
 * Gnu (G)
   * A peaceful animal with 6 heath.
   * Drops gnu meat when it dies

