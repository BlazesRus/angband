/*
 * File: list-stats.h
 * Purpose: Player stats
 *
 * Changing stat order or making new ones will break savefiles.
 * Each stat has a matching sustain in src/list-object-flags.h, which should
 * be at the same index in that file as the stat in this file (plus one for OF_NONE).
 *
 * Stat properties are defined in lib/gamedata/object_property.txt
 */
#ifndef INCLUDED_LIST_STATS_H
#define INCLUDED_LIST_STATS_H
STAT(STR, "You feel stronger!", "You feel weaker!")
STAT(INT, "You feel smarter!", "You feel more stupid!")
STAT(WIS, "You feel wiser!", "You feel more naive!")
STAT(DEX, "You feel more dextrous!", "You feel clumsier!")
STAT(CON, "You feel healthier!", "You feel sicklier!")
STAT(CHR, "You feel more beautiful!", "You feel more ugly!")

#endif