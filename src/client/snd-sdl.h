/*
 * File: snd-sdl.h
 * Purpose: SDL sound support
 */

#if (defined(MBandClient)||defined(SPClient)||defined(HybridClient)) && (defined(SOUND_SDL) || defined(SOUND_SDL2)) && !defined(INCLUDED_SND_SDL_H)
#define INCLUDED_SND_SDL_H

extern errr init_sound_sdl(struct sound_hooks *hooks);

#endif /* INCLUDED_SND_SDL_H */
