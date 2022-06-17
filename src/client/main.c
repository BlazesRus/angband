/*
 * File: main.c
 * Purpose: Core game initialisation
 *
 * Copyright (c) 1997 Ben Harrison, and others
 * Copyright (c) 2022 MAngband and PWMAngband Developers
 *
 * This work is free software; you can redistribute it and/or modify it
 * under the terms of either:
 *
 * a) the GNU General Public License as published by the Free Software
 *    Foundation, version 2, or
 *
 * b) the "Angband licence":
 *    This software may be copied and distributed for educational, research,
 *    and not for profit purposes provided that this copyright and statement
 *    are included in all such copies.  Other copyrights may also apply.
 */

/*
 * Some machines have a "main()" function in their "main-xxx.c" file,
 * all the others use this file for their "main()" function.
 */


#include "c-angband.h"
#ifdef HybridClient
#include "s-angband.h"
#endif

#ifdef ON_ANDROID
#include <SDL.h>
#endif

#ifdef HybridClient
/* Daily log file */
static int tm_mday = 0;
static ang_file *fp = NULL;
static bool fp_closed = false;
#endif

static errr init_error(void)
{
    return 1;
}


/*
 * List of the available modules in the order they are tried.
 */
static const struct module modules[] =
{
#ifdef USE_SDL
    {"sdl", init_sdl},
#endif

#ifdef USE_SDL2
	{"sdl2", init_sdl2},
#endif

#ifdef USE_GCU
    {"gcu", init_gcu},
#endif

    {"none", init_error}
};


/*
 * A hook for "quit()".
 *
 * Close down, then fall back into "quit()".
 */
static void quit_hook(const char *s)
{
    int j;

    /* Scan windows */
    for (j = ANGBAND_TERM_MAX - 1; j >= 0; j--)
    {
        /* Unused */
        if (!angband_term[j]) continue;

        /* Nuke it */
        term_nuke(angband_term[j]);
    }

    textui_cleanup();
    cleanup_angband();
    close_sound();

#ifdef HybridClient
    /* Close the daily log file */
    if (fp)
    {
        file_close(fp);
        fp_closed = true;
    }
#endif

#ifdef WINDOWS
    /* Cleanup WinSock */
    WSACleanup();
#endif
}

#ifdef HybridClient_TODO//Not actually  at going to use this part at moment
/*
 * Initialize and verify the file paths.
 *
 * Use the configured DEFAULT_*_PATH constants. Be sure to
 * set those properly when building...
 *
 * We must ensure that the path ends with "PATH_SEP" if needed,
 * since the "init_file_paths()" function will simply append the
 * relevant "sub-directory names" to the given path.
 */
static void init_stuff(void)
{
    char configpath[MSG_LEN];
    char libpath[MSG_LEN];
    char datapath[MSG_LEN];
#ifndef BUILDINGWithVS

    /* Use default */
    my_strcpy(configpath, DEFAULT_CONFIG_PATH, sizeof(configpath));
    my_strcpy(libpath, DEFAULT_LIB_PATH, sizeof(libpath));
    my_strcpy(datapath, DEFAULT_DATA_PATH, sizeof(datapath));

    /* Hack -- add a path separator (only if needed) */
    if (!suffix(configpath, PATH_SEP))
        my_strcat(configpath, PATH_SEP, sizeof(configpath));
    if (!suffix(libpath, PATH_SEP))
        my_strcat(libpath, PATH_SEP, sizeof(libpath));
    if (!suffix(datapath, PATH_SEP))
        my_strcat(datapath, PATH_SEP, sizeof(datapath));
#else//Create absolute path
    char path[MSG_LEN];//Based on https://www.delftstack.com/howto/c/get-current-directory-in-c/

    errno = 0;
    if (getcwd(path, MSG_LEN) == NULL) {
        if (errno == ERANGE)
            printf("[ERROR] pathname length exceeds the buffer size\n");
        else
            perror("getcwd");
        exit(EXIT_FAILURE);
    }
#ifdef UsingClangToolset
    strcat(path, "\\");
#else
    strcat(path, PATH_SEP);
#endif
#ifdef _DEBUG
    printf("Current working directory: %s\n", path);
#endif
    //Constructing absolute directory for paths
    #ifdef UsingModernC
        strcpy(configpath, path); strcat(configpath, DEFAULT_CONFIG_PATH);
        strcpy(libpath, path); strcat(libpath, DEFAULT_LIB_PATH);
        strcpy(datapath, path); strcat(datapath, DEFAULT_DATA_PATH);
    #else
        strlcpy(configpath, path, 200); strlcat(configpath, DEFAULT_CONFIG_PATH, 200);
        strlcpy(libpath, path, 200); strlcat(libpath, DEFAULT_LIB_PATH, 200);
        strlcpy(datapath, path, 200); strlcat(datapath, DEFAULT_DATA_PATH, 200);
    #endif
#endif

    /* Initialize */
    init_file_paths(configpath, libpath, datapath);

    /* Create any missing directories */
    create_needed_dirs();
}


/*
 * Translate from ISO Latin-1 characters 128+ to 7-bit ASCII.
 *
 * We use this table to maintain compatibility with systems that cannot
 * display 8-bit characters.  We also use it whenever we wish to suppress
 * accents or ensure that a character is 7-bit.
 */
static const char seven_bit_translation[128] =
{
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
    'A', 'A', 'A', 'A', 'A', 'A', ' ', 'C',
    'E', 'E', 'E', 'E', 'I', 'I', 'I', 'I',
    'D', 'N', 'O', 'O', 'O', 'O', 'O', ' ',
    'O', 'U', 'U', 'U', 'U', 'Y', ' ', ' ',
    'a', 'a', 'a', 'a', 'a', 'a', ' ', 'c',
    'e', 'e', 'e', 'e', 'i', 'i', 'i', 'i',
    'o', 'n', 'o', 'o', 'o', 'o', 'o', ' ',
    'o', 'u', 'u', 'u', 'u', 'y', ' ', 'y'
};


/*
 * Server logging hook.
 * We should be cautious, as we may be called from a signal handler in a panic.
 */
static void server_log(const char *str)
{
    char buf[16];
    time_t t;
    struct tm *local;
    char path[MSG_LEN];
    char file[30];
    char ascii[MSG_LEN];
    char *s;

    /* Grab the time */
    time(&t);
    local = localtime(&t);
    strftime(buf, 16, "%d%m%y %H%M%S", local);

    /* Translate into ASCII */
    my_strcpy(ascii, str, sizeof(ascii));
    for (s = ascii; *s; s++)
    {
        if (*s < 0) *s = seven_bit_translation[128 + *s];
    }

    /* Output the message timestamped */
    fprintf(stderr, "%s %s\n", buf, ascii);

    /* Paranoia */
    if (fp_closed) return;

    /* Open the daily log file */
    if (tm_mday != local->tm_mday)
    {
        tm_mday = local->tm_mday;

        /* Close the daily log file */
        if (fp) file_close(fp);

        /* Open a new daily log file */
        strftime(file, 30, "pwmangband%d%m%y.log", local);
        path_build(path, sizeof(path), ANGBAND_DIR_SCORES, file);
        fp = file_open(path, MODE_APPEND, FTYPE_TEXT);
        if (fp == NULL)
        {
            printf("Unable to open %s for writing!\n", path);
            return;
        }
    }

    /* Output the message to the daily log file */
    file_putf(fp, "%s %s\n", buf, str);
    file_flush(fp);
}


static void show_version(void)
{
    printf("PWMAngband Server(BlazesRus Branch)%s\n", version_build(NULL, true));
    puts("Copyright (c) 2007-2022 MAngband and PWMAngband Project Team");

    /* Actually abort the process */
    quit(NULL);
}
#endif


static void read_credentials(void)
{
#ifdef WINDOWS
    char buffer[20] = {'\0'};
    DWORD bufferLen = sizeof(buffer);

    /* Initial defaults */
    my_strcpy(nick, "PLAYER", sizeof(nick));
    my_strcpy(pass, "passwd", sizeof(pass));
    my_strcpy(real_name, "PLAYER", sizeof(real_name));

    /* Get user name from Windows machine! */
    if (GetUserName(buffer, &bufferLen))
    {
        /* Cut */
        buffer[MAX_NAME_LEN] = '\0';

        /* Copy to real name */
        my_strcpy(real_name, buffer, sizeof(real_name));
    }
#else
    /* Initial defaults */
    my_strcpy(nick, "PLAYER", sizeof(nick));
    my_strcpy(pass, "passwd", sizeof(pass));
    my_strcpy(real_name, "PLAYER", sizeof(real_name));
#endif
}


/*
 * Simple "main" function for multiple platforms.
 */
int main(int argc, char *argv[])
{
    bool done = false;
#ifdef WINDOWS
    WSADATA wsadata;
#endif
    int i;

    /* Save the program name */
    argv0 = argv[0];

    /* Save command-line arguments */
    clia_init(argc, (const char**)argv);

#ifdef WINDOWS
    /* Initialize WinSock */
    WSAStartup(MAKEWORD(1, 1), &wsadata);
#endif

    memset(&Setup, 0, sizeof(Setup));

    /* Client Config-file */
    conf_init(NULL);

    /* Setup the file paths */
    init_stuff();

    /* Install "quit" hook */
    quit_aux = quit_hook;

    /* Try the modules in the order specified by modules[] */
    for (i = 0; i < (int)N_ELEMENTS(modules); i++)
    {
        if (0 == modules[i].init())
        {
            ANGBAND_SYS = modules[i].name;
            done = true;
            break;
        }
    }

    /* Make sure we have a display! */
    if (!done) quit("Unable to prepare any 'display module'!");

    /* Attempt to read default name/real name from OS */
    read_credentials();

    /* Get the meta address */
    my_strcpy(meta_address, conf_get_string("MAngband", "meta_address", "mangband.org"),
        sizeof(meta_address));
    meta_port = conf_get_int("MAngband", "meta_port", 8802);

    turn_off_numlock();

    /* Create save default config */
    conf_default_save();

    /* Initialize RNG */
    Rand_init();

    /* Initialize everything, contact the server, and start the loop */
    client_init(true);

#ifdef HybridClient_TODO//Not actually  at going to use this part at moment
    /* Load the mangband.cfg options */
    load_server_cfg();

    /* Initialize the basics */
    init_angband();

    /* Play the game */
    play_game();
#endif

    /* Quit */
    quit(NULL);

    /* Exit */
    return 0;
}
