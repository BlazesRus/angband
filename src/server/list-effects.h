/*
 * File: list-effects.h
 * Purpose: List of effects
 */
#ifndef INCLUDED_LIST_effects_H
#define INCLUDED_LIST_effects_H

/*
 * name: effect code
 * aim: does the effect require aiming?
 * info: info label for spells
 * args: how many arguments the description takes
 * info flags: flags for object description
 * description: text of description
 */

/* name  aim  info  args  info flags  description */
EFFECT(ACQUIRE, false, NULL, 1, EFINFO_CONST, "creates %s good item(s) nearby")
EFFECT(ALTER, true, NULL, 1, EFINFO_BOLT, "creates a line which %s")
EFFECT(ALTER_REALITY, false, NULL, 0, EFINFO_NONE, "creates a new dungeon level")
EFFECT(ARC, true, NULL, 2, EFINFO_BREATH, "produces a cone of %s for %s points of damage")
EFFECT(BALL, true, "dam", 3, EFINFO_BALL, "shoots a radius-{%d} %s ball that inflicts %s points of damage at the centre")
EFFECT(BALL_OBVIOUS, true, NULL, 3, EFINFO_BALL, "shoots a radius-{%d} %s ball that inflicts %s points of damage")
EFFECT(BANISH, false, NULL, 0, EFINFO_NONE, "removes all non-unique monsters represented by a chosen symbol from the level, inflicting {1d4} points of damage for every monster removed")
EFFECT(BATTY, false, NULL, 0, EFINFO_NONE, "turns you into a fruit bat")
EFFECT(BEAM, true, "dam", 2, EFINFO_BOLTD, "shoots a beam of %s that inflicts %s points of damage")
EFFECT(BEAM_OBVIOUS, true, NULL, 2, EFINFO_BOLTD, "shoots a beam of %s that inflicts %s points of damage")
EFFECT(BIZARRE, true, NULL, 0, EFINFO_NONE, "does bizarre things")
EFFECT(BLAST, false, "dam", 3, EFINFO_BALL, "creates a radius-{%d} %s ball around you that inflicts %s points of damage")
EFFECT(BLAST_OBVIOUS, false, NULL, 3, EFINFO_BALL, "creates a radius-{%d} %s ball around you that inflicts %s points of damage")
EFFECT(BOLT, true, "dam", 2, EFINFO_BOLTD, "shoots a bolt of %s that inflicts %s points of damage")
EFFECT(BOLT_AWARE, true, NULL, 1, EFINFO_BOLT, "creates a bolt which %s affected by the bolt")
EFFECT(BOLT_OR_BEAM, true, "dam", 2, EFINFO_BOLTD, "shoots a bolt or beam of %s that inflicts %s points of damage")
EFFECT(BOLT_STATUS, true, NULL, 1, EFINFO_BOLT, "produces a bolt that %s the first monster in its path")
EFFECT(BOLT_STATUS_DAM, true, "dam", 2, EFINFO_BOLTD, "casts a bolt which %s, dealing %s damage")
EFFECT(BOW_BRAND, false, "dam", 0, EFINFO_NONE, "makes your missiles explode")
EFFECT(BOW_BRAND_SHOT, false, "dam", 0, EFINFO_NONE, "brands your missiles")
EFFECT(BRAND_AMMO, false, NULL, 0, EFINFO_NONE, "brands a stack of ammunition")
EFFECT(BRAND_WEAPON, false, NULL, 0, EFINFO_NONE, "brands your wielded melee weapon")
EFFECT(BREATH, true, NULL, 2, EFINFO_BREATH, "allows you to breathe %s for %s points of damage")
EFFECT(CLEAR_VALUE, false, NULL, 0, EFINFO_NONE, NULL)
EFFECT(CLOAK_CHANGT, false, "dur", 0, EFINFO_NONE, "disguises you as another player's class")
EFFECT(COOKIE, false, NULL, 0, EFINFO_NONE, "displays a random hint")
EFFECT(CREATE_ARROWS, false, NULL, 0, EFINFO_NONE, "uses a staff to create a stack of arrows")
EFFECT(SALVAGE, false, NULL, 0, EFINFO_NONE, "salvages items into valuables")
EFFECT(ALCHEMY, false, NULL, 0, EFINFO_NONE, "brew potions from reagents")
EFFECT(CRAFT, false, NULL, 0, EFINFO_NONE, "recraft equipment")
EFFECT(CREATE_HOUSE, false, NULL, 0, EFINFO_NONE, "allows you to create a house from House Foundation Stones")
EFFECT(CREATE_POISON, false, NULL, 0, EFINFO_NONE, "creates potions of poison from any potion")
EFFECT(CREATE_HOLY_WATER, false, NULL, 0, EFINFO_NONE, "creates holy water from any potion")
EFFECT(CREATE_STAIRS, false, NULL, 0, EFINFO_NONE, "creates a staircase beneath your feet")
EFFECT(CREATE_TREES, false, NULL, 0, EFINFO_NONE, "creates trees next to you")
EFFECT(CREATE_WALLS, false, NULL, 0, EFINFO_NONE, "creates a wall next to you")
EFFECT(CRUNCH, false, NULL, 0, EFINFO_NONE, "crunches")
EFFECT(CURE, false, NULL, 1, EFINFO_CURE, "cures %s")
EFFECT(CURSE, true, "dam", 0, EFINFO_NONE, "damages a monster directly")
EFFECT(CURSE_ARMOR, false, NULL, 0, EFINFO_NONE, "curses your worn armor")
EFFECT(CURSE_WEAPON, false, NULL, 0, EFINFO_NONE, "curses your wielded melee weapon")
EFFECT(DAMAGE, false, "hurt", 1, EFINFO_DICE, "damages the player for %s points of damage")
EFFECT(DARKEN_AREA, false, NULL, 0, EFINFO_NONE, "darkens a radius-{3} area (plus the entire room if you are currently in one)")
EFFECT(DARKEN_LEVEL, false, NULL, 0, EFINFO_NONE, "completely darkens up and magically maps the current level")
EFFECT(DEEP_DESCENT, false, NULL, 0, EFINFO_NONE, "teleports you up to five dungeon levels lower than the lowest point you have reached so far")
EFFECT(DESTRUCTION, false, NULL, 1, EFINFO_QUAKE, "destroys a radius-%s circular area around you and blinds you for {10+1d10} turns")
EFFECT(DETECT_ALL_MONSTERS, false, NULL, 0, EFINFO_NONE, "detects all creatures on the level")
EFFECT(DETECT_DOORS, false, NULL, 0, EFINFO_NONE, "detects all doors in the immediate area")
EFFECT(DETECT_EVIL, false, NULL, 0, EFINFO_NONE, "detects all evil monsters in the immediate area")
EFFECT(DETECT_FEARFUL_MONSTERS, false, NULL, 0, EFINFO_NONE, "detects all fearful monsters in the immediate area")
EFFECT(DETECT_ANIMALS, false, NULL, 0, EFINFO_NONE, "detects all animals in the immediate area")
EFFECT(DETECT_GOLD, false, NULL, 0, EFINFO_NONE, "detects gold nearby")
EFFECT(DETECT_INVISIBLE_MONSTERS, false, NULL, 0, EFINFO_NONE, "detects all invisible monsters in the immediate area")
EFFECT(DETECT_LIVING_MONSTERS, false, NULL, 0, EFINFO_NONE, "detects living creatures nearby")
EFFECT(DETECT_MONSTERS, false, NULL, 0, EFINFO_NONE, "detects all monsters in the immediate area")
EFFECT(DETECT_NONEVIL, false, NULL, 0, EFINFO_NONE, "detects all non-evil monsters in the immediate area")
EFFECT(DETECT_SOUL, false, NULL, 0, EFINFO_NONE, "detects all creatures with a spirit in the immediate area")
EFFECT(DETECT_STAIRS, false, NULL, 0, EFINFO_NONE, "detects all stairs in the immediate area")
EFFECT(DETECT_TRAPS, false, NULL, 0, EFINFO_NONE, "detects all traps in the immediate area")
EFFECT(DETECT_TREASURES, false, NULL, 0, EFINFO_NONE, "detects all treasure and objects in the immediate area")
EFFECT(DETECT_VISIBLE_MONSTERS, false, NULL, 0, EFINFO_NONE, "detects visible creatures nearby")
EFFECT(DETONATE, false, NULL, 0, EFINFO_NONE, "makes all summoned jellies and vortices explode")
EFFECT(DISENCHANT, false, NULL, 0, EFINFO_NONE, "disenchants one of your wielded items")
EFFECT(DRAIN_LIGHT, false, NULL, 0, EFINFO_NONE, "drains your light source")
EFFECT(SPEND_MANA, false, NULL, 0, EFINFO_NONE, "spends some mana")
EFFECT(DRAIN_MANA, false, NULL, 0, EFINFO_NONE, "drains some mana")
EFFECT(DRAIN_STAT, false, NULL, 1, EFINFO_STAT, "reduces your %s")
EFFECT(EARTHQUAKE, true, NULL, 1, EFINFO_QUAKE, "causes a radius-%s earthquake")
EFFECT(ENCHANT, false, NULL, 2, EFINFO_ENCHANT, "attempts to magically enhance %s %s time(s)")
EFFECT(ELEM_BRAND, false, "x", 0, EFINFO_NONE, "brands your wielded melee weapon with some elements")
EFFECT(GAIN_EXP, false, NULL, 1, EFINFO_CONST, "grants (up to) %s experience points")
EFFECT(GRANITE, false, NULL, 0, EFINFO_NONE, "causes a granite wall to fall behind you")
EFFECT(GAIN_STAT, false, NULL, 1, EFINFO_STAT, "permanently increases %s")
EFFECT(HEAL_HP, false, "heal", 2, EFINFO_HEAL, "cures %s points of damage%s")
EFFECT(IDENTIFY, false, NULL, 0, EFINFO_NONE, "reveals a single unknown property of a selected item")
EFFECT(LASH, true, NULL, 2, EFINFO_LASH, "fires a beam of %s length %d, dealing damage determined by blows")
EFFECT(LIGHT_AREA, false, NULL, 0, EFINFO_NONE, "lights up the surrounding area")
EFFECT(LIGHT_LEVEL, false, NULL, 0, EFINFO_NONE, "completely lights up and magically maps the current level")
EFFECT(LINE, true, "dam", 2, EFINFO_BOLTD, "creates a line of %s, inflicting %s points of damage to susceptible monsters")
EFFECT(LOSE_EXP, false, NULL, 0, EFINFO_NONE, "drains {25%%} of your experience total")
EFFECT(LOSE_RANDOM_STAT, false, NULL, 1, EFINFO_STAT, "reduces a stat other than %s")
EFFECT(MAP_AREA, false, NULL, 0, EFINFO_NONE, "maps out a portion of the level around you")
EFFECT(MAP_WILD, false, NULL, 0, EFINFO_NONE, "reveals the location of a random wilderness area")
EFFECT(MASS_BANISH, false, NULL, 0, EFINFO_NONE, "removes all non-unique monsters within {20} squares of you, inflicting {1d3} points of damage for every monster removed")
EFFECT(MELEE_BLOWS, true, NULL, 0, EFINFO_NONE, "strikes blows against an adjacent monster")
EFFECT(MIND_VISION, false, NULL, 0, EFINFO_NONE, "allows you to infiltrate other players' minds")
EFFECT(MON_HEAL_HP, false, NULL, 0, EFINFO_NONE, "heals monster hitpoints")
EFFECT(MON_HEAL_KIN, false, NULL, 0, EFINFO_NONE, "heals fellow monster hitpoints")
EFFECT(MON_TIMED_INC, false, NULL, 2, EFINFO_TIMED, "increases monster %s by %s turns")
EFFECT(NOURISH, false, NULL, 3, EFINFO_FOOD, "%s for %s turns (%s percent)")
EFFECT(POLY_RACE, false, NULL, 0, EFINFO_NONE, "attempts to polymorph you into the corresponding monster")
EFFECT(PROBE, false, NULL, 0, EFINFO_NONE, "probes all monsters within line of sight")
EFFECT(PROJECT, false, NULL, 0, EFINFO_NONE, NULL)
EFFECT(PROJECT_LOS, false, NULL, 1, EFINFO_SEEN, "%s all monsters within line of sight")
EFFECT(PROJECT_LOS_AWARE, false, "dam", 1, EFINFO_SEEN, "%s within line of sight")
EFFECT(RANDOM, false, NULL, 0, EFINFO_NONE, "randomly ")
EFFECT(READ_MINDS, false, NULL, 0, EFINFO_NONE, "maps the area around recently detected monsters")
EFFECT(RECALL, false, "delay", 0, EFINFO_NONE, "takes you (after a short while) from the dungeon to the surface or from the surface to the deepest level you have visited in the dungeon")
EFFECT(RECHARGE, false, "power", 0, EFINFO_NONE, "tries to recharge a wand or staff, destroying the wand or staff on failure")
EFFECT(REMOVE_CURSE, false, NULL, 1, EFINFO_DICE, "attempts to remove a curse from an item with power %s")
EFFECT(RESILIENCE, false, NULL, 0, EFINFO_NONE, "increases the lifespan of all controlled monsters")
EFFECT(RESTORE_EXP, false, NULL, 0, EFINFO_NONE, "restores experience to maximum")
EFFECT(RESTORE_MANA, false, NULL, 1, EFINFO_MANA, "restores %s mana points")
EFFECT(RESTORE_STAT, false, NULL, 1, EFINFO_STAT, "restores %s")
EFFECT(RESURRECT, false, NULL, 0, EFINFO_NONE, "brings back to life a nearby ghost character")
EFFECT(RUBBLE, false, NULL, 0, EFINFO_NONE, "causes rubble to fall around you")
EFFECT(GLYPH, false, NULL, 1, EFINFO_NONE, "inscribes a glyph beneath you")
EFFECT(SAFE_GUARD, false, NULL, 0, EFINFO_NONE, "inscribes a circle of glyphs of warding around you")
EFFECT(SET_VALUE, false, NULL, 0, EFINFO_NONE, NULL)
EFFECT(SHORT_BEAM, true, "dam", 3, EFINFO_SHORT, "produces a beam of %s with length %d that inflicts %s points of damage")
EFFECT(SPOT, false, "dam", 3, EFINFO_BALL, "creates a radius-{%d} %s ball around you that inflicts %s points of damage")
EFFECT(STAR, false, "dam", 2, EFINFO_BOLTD, "fires a line of %s in all eight cardinal directions, inflicting %s points of damage on light-sensitive creatures caught in one of the lines")
EFFECT(STAR_BALL, false, "dam", 3, EFINFO_BALL, "shoots a radius-{%d} %s ball that inflicts %s points of damage in all eight cardinal directions")
EFFECT(STRIKE, true, "dam", 3, EFINFO_BALL, "shoots a radius-{%d} %s ball that inflicts %s points of damage at the centre")
EFFECT(SUMMON, false, NULL, 2, EFINFO_SUMM, "summons %s %s")
EFFECT(SWARM, true, "dam", 3, EFINFO_BALL, "fires a series of radius-{%d} %s balls each inflicting %s points of damage")
EFFECT(SWEEP, false, NULL, 0, EFINFO_NONE, "strikes blows against all adjacent monsters")
EFFECT(TAP_DEVICE, false, NULL, 0, EFINFO_NONE, "tries to drain charges from a wand or staff")
EFFECT(TAP_UNLIFE, true, "dam", 1, EFINFO_DICE, "drains %s mana from the closest undead monster, damaging it")
EFFECT(TELE_OBJECT, false, NULL, 0, EFINFO_NONE, "allows you to send objects to other players")
EFFECT(TELEPORT, false, "range", 1, EFINFO_TELE, "teleports you randomly up to %s squares away")
EFFECT(TELEPORT_LEVEL, false, NULL, 0, EFINFO_NONE, "teleports you one level up or down (chosen at random)")
EFFECT(TELEPORT_TO, true, "range", 0, EFINFO_NONE, "teleports toward a target")
EFFECT(TIMED_DEC, false, NULL, 2, EFINFO_TIMED, "reduces length of %s by %s turns")
EFFECT(TIMED_INC, false, "dur", 2, EFINFO_TIMED, "grants or extends %s for %s turns")
EFFECT(TIMED_INC_NO_RES, false, NULL, 2, EFINFO_TIMED, "extends %s for %s turns (unresistable)")
EFFECT(TIMED_SET, false, "dur", 2, EFINFO_TIMED, "induces %s for %s turns")
EFFECT(TOUCH, false, NULL, 1, EFINFO_TOUCH, "%s around you")
EFFECT(TOUCH_AWARE, false, NULL, 1, EFINFO_TOUCH, "%s around you")
EFFECT(UNDEAD_FORM, false, NULL, 0, EFINFO_NONE, "turns you into a ghost")
EFFECT(UPDATE_STUFF, false, NULL, 0, EFINFO_NONE, NULL)
EFFECT(WAKE, false, NULL, 0, EFINFO_NONE, "awakens all nearby sleeping monsters")
EFFECT(WEB, false, NULL, 0, EFINFO_NONE, "creates a web")
EFFECT(WIPE_AREA, false, NULL, 0, EFINFO_NONE, "wipes everything around you")
EFFECT(WONDER, true, NULL, 0, EFINFO_NONE, "creates random and unpredictable effects")
EFFECT(LOSE_STAT, false, NULL, 1, EFINFO_STAT, "permanently decreases %s")
EFFECT(WEB_SPIDER, false, NULL, 0, EFINFO_NONE, "creates a web")
EFFECT(FORGET_LEVEL, false, NULL, 0, EFINFO_NONE, "completely forgets the current level")
#endif