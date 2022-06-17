/*
 * File: list-origins.h
 * Purpose: List of object origins
 */
#ifndef INCLUDED_LIST_origins_H
#define INCLUDED_LIST_origins_H
ORIGIN(NONE, -1, "")
ORIGIN(FLOOR, 1, "Found lying on the floor %s.")
ORIGIN(CHEST, 1, "Taken from a chest found %s.")
ORIGIN(SPECIAL, 1, "Found lying on the floor of a special room %s.")
ORIGIN(PIT, 1, "Found lying on the floor in a pit %s.")
ORIGIN(VAULT, 1, "Found lying on the floor in a vault %s.")
ORIGIN(LABYRINTH, 1, "Found lying on the floor of a labyrinth %s.")
ORIGIN(CAVERN, 1, "Found lying on the floor of a cavern %s.")
ORIGIN(RUBBLE, 1, "Found under some rubble %s.")
ORIGIN(MIXED, -1, "")
ORIGIN(DROP, 2, "Dropped by %s %s.")
ORIGIN(DROP_SPECIAL, 2, "Dropped by %s %s.")
ORIGIN(DROP_PIT, 2, "Dropped by %s %s.")
ORIGIN(DROP_VAULT, 2, "Dropped by %s %s.")
ORIGIN(ACQUIRE, 1, "Conjured forth by magic %s.")
ORIGIN(STORE, 1, "Bought from a store %s.")
ORIGIN(STOLEN, -1, "")
ORIGIN(BIRTH, 1, "An inheritance from %s family.")
ORIGIN(CHEAT, 0, "Created by debug option.")
ORIGIN(DROP_BREED, 2, "Dropped by %s %s.")
ORIGIN(DROP_SUMMON, 2, "Dropped by %s %s.")
ORIGIN(DROP_UNKNOWN, 1, "Dropped by an unknown monster %s.")
ORIGIN(DROP_POLY, 2, "Dropped by %s %s.")
ORIGIN(DROP_MIMIC, 2, "Dropped by %s %s.")
ORIGIN(WORTHLESS, 1, "Conjured forth by magic %s.")
ORIGIN(PLAYER, 1, "Purchased from a player %s.")
ORIGIN(FOUNTAIN, 1, "Obtained from a fountain %s.")
ORIGIN(WILD_DWELLING, 1, "Found in an abandoned house %s.")

#endif