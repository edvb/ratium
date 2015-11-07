# NAME ratium - A Dumb Little NCurses ASCII Game

## SYNOPSIS

ratium [--help] [--version]

## DESCRIPTION

ratium  is a dumb little game where you are a '@' and have to survive in an
ASCII world by defending yourself from various enemies, like the super rat.
Collect items such as spam, swords, and haggises to aid you in your battles.

## COMMANDS

To quit the game press ESC, you can also press ctrl-z to suspend the game.

### MOVEMENT

y k u

h . l

b j n

### PLAYER ACTIONS

 * **i**: Open/close inventory menu.
 * **o**: Action key.
 * **p**: drop key.

## ENTITIES
 * Rat (r)
   * A hostile creature.
   * Attack: 1
   * Heath:  2
   * Drops rat meat on death.
 * Super Rat (R)
   * A hostile creature.
   * Attack: 2
   * Heath:  4
   * Drops rat meat on death.
 * Gnu (G)
   * A peaceful animal.
   * Heath:  6
   * Drops gnu meat on death.
 * Cow (c)
   * A peaceful animal.
   * Heath:  2
   * Drops beef on death.
 * King Arthur (@)
   * A NPC which will talk to you.
   * Heath:  10
   * Drops gold on death.

## Items
 * Gold ($)
   * Increase score
 * Spam (=)
   * Eat to restore 2 heath
 * Haggis (o)
   * Eat to restore 10 heath
 * Sword (/)
   * Hold to deal more damage
 * Shield (0)
   * Hold to block damage
 * Bow ())
   * Hold to fire arrows
 * Arrow (|)
   * Used by bow to deal 5 damage
 * Rat Meat (%)
   * Eat to remove 1 heath
 * Gnu Meat (%)
   * Eat to restore 4 heath
 * Beef (%)
   * Eat to restore 22 heath

## Author

Written by ED van Bruggen.

## See Also

View source code at: <https://gitlab.com/edvb/ratium> or see screenshots, videos
and binary download at: <http://edvb.itch.io/ratium>
