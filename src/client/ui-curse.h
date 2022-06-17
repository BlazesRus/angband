/*
 * File: ui-curse.h
 * Purpose: Curse selection menu
 */

#if (defined(MBandClient)||defined(SPClient)||defined(HybridClient)) && !defined(UI_CURSE_H)
#define UI_CURSE_H

extern bool textui_get_curse(int *choice, struct object *obj, char *dice_string);

#endif
