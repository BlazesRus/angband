/*
 * File: ui-death.h
 * Purpose: Handle the UI bits that happen after the character dies.
 */

#if (defined(MBandClient)||defined(SPClient)||defined(HybridClient)) && !defined(INCLUDED_UI_DEATH_H)
#define INCLUDED_UI_DEATH_H

extern void print_tomb(void);
extern void display_winner(void);

#endif /* INCLUDED_UI_DEATH_H */
