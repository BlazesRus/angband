/*
 * File: player-ui.h
 * Purpose: Character screens and dumps
 */

#if (defined(MBandServer)||defined(HybridClient)) && !defined(PLAYER_UI_H)
#define PLAYER_UI_H

extern bool dump_save(struct player *p, const char *path, bool server);

#endif /* PLAYER_UI_H */
