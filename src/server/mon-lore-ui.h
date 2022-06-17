/*
 * File: mon-lore-ui.h
 * Purpose: Monster memory UI
 */

#if (defined(MBandServer)||defined(HybridClient)) && !defined(MONSTER_LORE_UI_H)
#define MONSTER_LORE_UI_H

extern void lore_description(struct player *p, const struct monster_race *race);
extern void describe_monster(struct player *p, const struct monster_race *race);

#endif /* MONSTER_LORE_UI_H */
