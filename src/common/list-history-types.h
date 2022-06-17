/*
 * File: list-history-types.h
 * Purpose: History message types
 */
#ifndef INCLUDED_LIST_HISTORY_TYPES_H
#define INCLUDED_LIST_HISTORY_TYPES_H
HIST(NONE, "")
HIST(PLAYER_BIRTH, "Player was born")
HIST(ARTIFACT_UNKNOWN, "Player has generated an artifact")
HIST(ARTIFACT_KNOWN, "Player has found an artifact")
HIST(ARTIFACT_LOST, "Player had an artifact and lost it")
HIST(PLAYER_DEATH, "Player has been slain")
HIST(SLAY_UNIQUE, "Player has slain a unique monster")
HIST(HELP_UNIQUE, "Player helped to slay a unique monster")
HIST(PLAYER_REVIVE, "Player has been revived")
HIST(GAIN_LEVEL, "Player gained a level")
//From Angband
HIST(USER_INPUT,		"User-added note")
HIST(SAVEFILE_IMPORT,	"Added when an older version savefile is imported")
HIST(GENERIC,			"Anything else not covered here (unused)")

#endif