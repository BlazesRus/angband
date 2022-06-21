/*
 * File: mon-lore.h
 * Purpose: Monster memory code.
 */

#ifndef MONSTER_LORE_H
#define MONSTER_LORE_H

#ifdef EnableExtraHeaderIncludes
#include "z-textblock.h"
#include "monster.h"
#endif

#ifdef ENABLEFeature_EnableExtraMonsterLore
/**
 * Monster "lore" information
 */
typedef struct monster_lore
{
	int ridx;			/* Index of monster race */

	uint16_t sights;		/* Count sightings of this monster */
	uint16_t deaths;		/* Count deaths from this monster */

	uint16_t pkills;		/* Count monsters killed in this life */
	uint16_t thefts;		/* Count objects stolen in this life */
	uint16_t tkills;		/* Count monsters killed in all lives */

	uint8_t wake;			/* Number of times woken up (?) */
	uint8_t ignore;			/* Number of times ignored (?) */

	uint8_t drop_gold;		/* Max number of gold dropped at once */
	uint8_t drop_item;		/* Max number of item dropped at once */

	uint8_t cast_innate;		/* Max number of innate spells seen */
	uint8_t cast_spell;		/* Max number of other spells seen */

	struct monster_blow *blows; /* Knowledge of blows */

	bitflag flags[RF_SIZE]; /* Observed racial flags - a 1 indicates
	                         * the flag (or lack thereof) is known to
	                         * the player */
	bitflag spell_flags[RSF_SIZE];  /* Observed racial spell flags */

	struct monster_drop *drops;
    struct monster_friends *friends;
	struct monster_friends_base *friends_base;
	struct monster_mimic *mimic_kinds;

	/* Derived known fields, put here for simplicity */
	bool all_known;
	bool *blow_known;
	bool armour_known;
	bool drop_known;
	bool sleep_known;
	bool spell_freq_known;
	bool innate_freq_known;
} monster_lore;

/**
 * Array[z_info->r_max] of monster lore
 */
extern struct monster_lore *l_list;

bool lore_save(const char *name);
#endif

extern void lore_learn_spell_if_has(struct monster_lore *lore, const struct monster_race *race,
    int flag);
extern void lore_learn_spell_if_visible(struct monster_lore *lore, bool visible, int flag);
extern void lore_learn_flag_if_visible(struct monster_lore *lore, bool visible, int flag);
extern void lore_update(const struct monster_race *race, struct monster_lore *lore);
extern void lore_do_probe(struct player *p, struct monster *mon);
extern bool lore_is_fully_known(struct player *p, const struct monster_race *race);
extern void lore_treasure(struct player *p, struct monster *mon, int num_item, int num_gold);
extern void monster_flags_known(const struct monster_race *race, const struct monster_lore *lore,
    bitflag flags[RF_SIZE]);
extern void lore_append_kills(struct player *p, const struct monster_race *race,
    const struct monster_lore *lore, const bitflag known_flags[RF_SIZE]);
extern void lore_append_flavor(struct player *p, const struct monster_race *race);
extern void lore_append_movement(struct player *p, const struct monster_race *race,
    const struct monster_lore *lore, bitflag known_flags[RF_SIZE]);
extern void lore_append_toughness(struct player *p, const struct monster_race *race,
    const struct monster_lore *lore, bitflag known_flags[RF_SIZE]);
extern void lore_append_exp(struct player *p, const struct monster_race *race,
    const struct monster_lore *lore, bitflag known_flags[RF_SIZE]);
extern void lore_append_drop(struct player *p, const struct monster_race *race,
    const struct monster_lore *lore, bitflag known_flags[RF_SIZE]);
extern void lore_append_abilities(struct player *p, const struct monster_race *race,
    const struct monster_lore *lore, bitflag known_flags[RF_SIZE]);
extern void lore_append_awareness(struct player *p, const struct monster_race *race,
    const struct monster_lore *lore, bitflag known_flags[RF_SIZE]);
extern void lore_append_friends(struct player *p, const struct monster_race *race,
    const struct monster_lore *lore, bitflag known_flags[RF_SIZE]);
extern void lore_append_spells(struct player *p, const struct monster_race *race,
    const struct monster_lore *lore, bitflag known_flags[RF_SIZE]);
extern void lore_append_attack(struct player *p, const struct monster_race *race,
    const struct monster_lore *lore, bitflag known_flags[RF_SIZE]);
extern void get_global_lore(struct player *p, const struct monster_race *race,
    struct monster_lore* lore);
extern struct monster_lore *get_lore(struct player *p, const struct monster_race *race);

#endif /* MONSTER_LORE_H */
