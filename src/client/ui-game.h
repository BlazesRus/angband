/*
 * File: ui-game.h
 * Purpose: Game management for the traditional text UI
 */

#if (defined(MBandClient)||defined(SPClient)||defined(HybridClient)) && !defined(INCLUDED_UI_GAME_H)
#define INCLUDED_UI_GAME_H

#include "cmd-core.h"
#include "game-event.h"

extern void cmd_init(void);
extern unsigned char cmd_lookup_key(cmd_code lookup_cmd, int mode);
extern unsigned char cmd_lookup_key_unktrl(cmd_code lookup_cmd, int mode);
extern cmd_code cmd_lookup(unsigned char key, int mode);
extern void textui_process_command(void);

#endif /* INCLUDED_UI_GAME_H */
