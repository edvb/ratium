# ratium - extendible top-down pixel adventure game

## SYNOPSIS

`ratium` [--help] [--version]

## DESCRIPTION

Ratium is a dumb little game where you play as an adventurer and have to
survive in a pixel world by defending yourself from various enemies, such as
the super rat. Collect items such as spam, swords, and arrows to aid you in
your battles and exploration.

## COMMANDS

| Key | Function            |
| --- | ------------------- |
| h   | Move left           |
| j   | Move down           |
| k   | Move up             |
| l   | Move left           |
| e   | Open inventory menu |
| r   | Action key          |
| t   | Drop key            |
| ESC | Quit                |

## ENTITIES

| Name        | Description                                  | Heath | Attack | Drops |
| ----------- | ---------------------------------            | ----- | ------ | ----- |
| Rat         | A small pest                                 | 2     | 1      | Meat  |
| Super Rat   | A rat you don't want to mess with            | 4     | 2      | Meat  |
| Gnu         | A harmless animal that just wants to be free | 6     | 1      | Meat  |
| Cow         | "moo"                                        | 2     | None   | Meat  |
| King Arthur | King of the Britons                          | 10    | None   | Gold  |
| Knight      | A humble warrior                             | 10    | None   | Sword |
| Peasant     | A worthless low life                         | 10    | None   | None  |

## ITEMS

| Item   | Description                                   |
| ------ | --------------------------------------------- |
| Gold   | Increase score                                |
| Spam   | Eat to restore 3 heath                        |
| Sword  | Hold to deal more 2 damage                    |
| Shield | Hold to block 4 damage                        |
| Bow    | Hold to load then fire arrows up to 20 blocks |
| Arrow  | Fire with bow to deal 5 damage                |
| Meat   | Eat to restore 1 heath                        |

## AUTHOR

Ed van Bruggen <edvb54@gmail.com>

## SEE ALSO

View source code at: <https://gitlab.com/edvb/ratium>

See screenshots, videos, and binary download at: <http://edvb.itch.io/ratium>

## LICENSE

GPL v3 License
