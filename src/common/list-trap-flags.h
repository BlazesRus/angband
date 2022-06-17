/*
 * File: list-trap-flags.h
 * Purpose: Trap properties
 *
 * Adjusting these flags does not break savefiles. Flags below start from 1
 * on line 13, so a flag's sequence number is its line number minus 12.
 */
#ifndef INCLUDED_LIST_TRAP_FLAGS_H
#define INCLUDED_LIST_TRAP_FLAGS_H

/* symbol descr */
TRF(NONE, "")
TRF(GLYPH, "Is a glyph")
TRF(TRAP, "Is a player trap")
TRF(VISIBLE, "Is visible")
TRF(INVISIBLE, "Is invisible")
TRF(FLOOR, "Can be set on a floor")
TRF(DOWN, "Takes the player down a level")
TRF(PIT, "Moves the player onto the trap")
TRF(ONETIME, "Disappears after being activated")
TRF(MAGICAL, "Has magical activation (absence of this flag means physical)")
TRF(SAVE_THROW, "Allows a save from all effects by standard saving throw")
TRF(SAVE_ARMOR, "Allows a save from all effects due to AC")
TRF(LOCK, "Is a door lock")
TRF(DELAY, "Has a delayed effect")
TRF(FEATHER_TRAP, "You can't avoid trap as you can not fly")
TRF(FEATHER_SAVE, "Allows a save if flying")

#endif