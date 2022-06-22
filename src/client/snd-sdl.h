/*
 * File: snd-sdl.h
 * Purpose: SDL sound support
 */

#if !defined(INCLUDED_SND_SDL_H) && (defined(SOUND_SDL)||defined(SOUND_SDL2))
#define INCLUDED_SND_SDL_H

extern errr init_sound_sdl(struct sound_hooks *hooks);

#endif /* INCLUDED_SND_SDL_H */
