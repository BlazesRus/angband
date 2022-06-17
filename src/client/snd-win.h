/*
 * File: snd-win.h
 * Purpose: Windows sound support
 */

#if (defined(MBandClient)||defined(SPClient)||defined(HybridClient)) && !defined(INCLUDED_SND_WIN_H)
#define INCLUDED_SND_WIN_H

extern errr init_sound_win(struct sound_hooks *hooks);

#endif /* INCLUDED_SND_WIN_H */
