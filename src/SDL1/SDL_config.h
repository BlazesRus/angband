/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2012 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

    Sam Lantinga
    slouken@libsdl.org
*/

#ifndef SDL_config_h_
#define SDL_config_h_

#include "SDL_platform.h"

/**
 *  \file SDL_config.h
 */

/* Add any platform that doesn't build using the configure system. */
#if defined(__SYMBIAN32__)
#include "SDL_config_symbian.h"  /* must be before win32! */
#elif defined(__WIN32__)
#include "SDL_config_win32.h"
#elif defined(__MACOS__)
#include "SDL_config_macos.h"
#elif defined(__MACOSX__)
#include "SDL_config_macosx.h"
#elif defined(__OS2__)
#include "SDL_config_os2.h"
#elif defined(__DREAMCAST__)
#include "SDL_config_dreamcast.h"
#else
/* This is a minimal configuration just to get SDL running on new platforms. */
#include "SDL_config_minimal.h"
#endif /* platform config */

#ifdef USING_GENERATED_CONFIG_H
#error Wrong SDL_config.h, check your include path?
#endif

#endif /* SDL_config_h_ */
