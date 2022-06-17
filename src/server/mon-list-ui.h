/*
 * File: mon-list-ui.h
 * Purpose: Monster list UI.
 */

#if (defined(MBandServer)||defined(HybridClient)) && !defined(MONSTER_LIST_UI_H)
#define MONSTER_LIST_UI_H

extern void monster_list_show_subwindow(struct player *p, int height, int width);
extern void monster_list_show_interactive(struct player *p, int height, int width);

#endif /* MONSTER_LIST_UI_H */
