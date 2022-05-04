CMAKE_MINIMUM_REQUIRED(VERSION 3.5 FATAL_ERROR)
PROJECT(Angband LANGUAGES C)
SET(OUR_EXECUTABLE_NAME ${PROJECT_NAME})

# Append the custom modules to the search path.
LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/SinglePlayerSrc/cmake/modules")

OPTION(SUPPORT_GCU_FRONTEND "Support for GCU front end." OFF)
OPTION(SUPPORT_NCURSES_FRONTEND "Synonym for SUPPORT_GCU_FRONTEND for backwards compatibility." OFF)
OPTION(SUPPORT_SDL_FRONTEND "Support for SDL front end." OFF)
OPTION(SUPPORT_SDL2_FRONTEND "Support for SDL2 front end." OFF)
OPTION(SUPPORT_X11_FRONTEND "Support for X11 Frontend." OFF)
OPTION(SUPPORT_SDL_SOUND "Support for sound with SDL." OFF)
OPTION(SUPPORT_SDL2_SOUND "Support for sound with SDL2." OFF)
IF(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    SET(SPOIL_DEFAULT OFF)
ELSE()
    SET(SPOIL_DEFAULT ON)
ENDIF()
OPTION(SUPPORT_SPOIL_FRONTEND "Support for spoiler front end." ${SPOIL_DEFAULT})
OPTION(SUPPORT_STATS_FRONTEND "Support for statistics front end; requires sqlite3 development library." OFF)
OPTION(SUPPORT_TEST_FRONTEND "Support for test front end." OFF)
OPTION(SUPPORT_WINDOWS_FRONTEND "Support for windows front end." OFF)
OPTION(SUPPORT_STATS_BACKEND "Enable backend support for statistics and related debugging commands.  Implied by SUPPORT_STATS_FRONTEND." OFF)

# By default, generate a self-contained build left where the build was run.
# If not using the Windows front end, the executable will have hardwired
# relative paths to the game's data, and when run, the working directory should
# be the location of executable.  The other options are to build for a shared
# installation with a setgid executable that manages the save and high score
# files or to build for a read-only installation where all the variable
# persistent state is stored in the user's directories.  Those two other
# options use paths from GNUInstallDirs as the basis for where stuff is
# installed.  Because paths are configured into the executable, it is not
# possible to set the destination dir when running make (i.e. with DESTDIR).
# One can set the paths when running cmake.  To set the path prefix, set
# CMAKE_INSTALL_PREFIX.  To customize where the pieces go beneath that prefix,
# set CMAKE_INSTALL_BINDIR, CMAKE_INSTALL_DATAROOTDIR, CMAKE_INSTALL_SYSCONFDIR,
# or CMAKE_INSTALL_SHAREDSTATEDIR.  Use the INSTALL_GROUP_ID cache variable
# (a string either holding the name or number of the group) as the source for
# the group ID to use for the setgid executable.  Note that with SHARED_INSTALL,
# running "make install" will always require superuser privileges because
# the owner of some of the installed files and directories is set to
# root:${INSTALL_GROUP_ID}.
OPTION(SC_INSTALL "Generate a self-contained build that could be left as is or moved elsewhere.  Conflicts with SHARED_INSTALL and READONLY_INSTALL." OFF)
OPTION(SHARED_INSTALL "Install for shared use with a setgid executable.  Conflicts with SC_INSTALL, READONLY_INSTALL, and SUPPORT_WINOOWS_FRONTEND." OFF)
SET(INSTALL_GROUP_ID "games" CACHE STRING "Group ID (either name or number) to use if SHARED_INSTALL is on.")
OPTION(READONLY_INSTALL "Install for shared use with variable persistent data stored in the user's directories.  Conflicts with SC_INSTALL, SHARED_INSTALL, and SUPPORT_WINDOWS_FRONTEND." OFF)
INCLUDE(GNUInstallDirs)

# Allow building the documentation if sphinx-build is available.
INCLUDE(CMakeDependentOption)
find_package(Sphinx)
CMAKE_DEPENDENT_OPTION(BUILD_DOC "Build the documentation" OFF "Sphinx_FOUND" OFF)
SET(DOC_HTML_THEME "" CACHE STRING "If set and not an empty string, it is the builtin Sphinx HTML theme to use in place of the default theme in conf.py (currently the better theme).")

IF ((SUPPORT_NCURSES_FRONTEND) AND (NOT SUPPORT_GCU_FRONTEND))
    SET(SUPPORT_GCU_FRONTEND ON)
ENDIF()
IF((SUPPORT_STATS_FRONTEND) AND (NOT SUPPORT_STATS_BACKEND))
    SET(SUPPORT_STATS_BACKEND ON)
ENDIF()
# If none of the graphical front ends will be configured, configure the one for
# Windows if that's the target plaform or the X11 one for anything else.
IF((NOT SUPPORT_GCU_FRONTEND) AND (NOT SUPPORT_SDL_FRONTEND) AND (NOT SUPPORT_SDL2_FRONTEND) AND (NOT SUPPORT_WINDOWS_FRONTEND) AND (NOT SUPPORT_X11_FRONTEND))
    IF(CMAKE_SYSTEM_NAME STREQUAL "Windows")
        SET(SUPPORT_WINDOWS_FRONTEND ON)
    ELSE()
        SET(SUPPORT_X11_FRONTEND ON)
    ENDIF()
ENDIF()
# Can not have support for SDL and SDL2 at the same time.
IF (SUPPORT_SDL2_FRONTEND OR SUPPORT_SDL2_SOUND)
    IF (SUPPORT_SDL_FRONTEND OR SUPPORT_SDL_SOUND)
        MESSAGE(FATAL_ERROR "Can not configure with support for both SDL and SDL2")
    ENDIF()
ENDIF()
IF(SUPPORT_WINDOWS_FRONTEND)
    # The Windows front end bypasses main.c so can't use any of the other front
    # ends when it is used.  For somewhat similar reasons, disable SDL or SDL2
    # sound with the Windows front end.
    IF(SUPPORT_GCU_FRONTEND)
        MESSAGE(WARNING "Disabling GCU front end because Windows front end is enabled")
        SET(SUPPORT_GCU_FRONTEND OFF)
    ENDIF()
    IF(SUPPORT_SDL_FRONTEND)
        MESSAGE(WARNING "Disabling SDL front end because Windows front end is enabled")
        SET(SUPPORT_SDL_FRONTEND OFF)
    ENDIF()
    IF(SUPPORT_SDL2_FRONTEND)
        MESSAGE(WARNING "Disabling SDL2 front end because Windows front end is enabled")
        SET(SUPPORT_SDL2_FRONTEND OFF)
    ENDIF()
    IF(SUPPORT_SPOIL_FRONTEND)
        MESSAGE(WARNING "Disabling spoiler front end because Windows front end is enabled")
        SET(SUPPORT_SPOIL_FRONTEND OFF)
    ENDIF()
    IF(SUPPORT_STATS_FRONTEND)
        MESSAGE(WARNING "Disabling statistics front end because Windows front end is enabled")
        SET(SUPPORT_STATS_FRONTEND OFF)
    ENDIF()
    IF(SUPPORT_TEST_FRONTEND)
        MESSAGE(WARNING "Disabling test front end because Windows front end is enabled")
        SET(SUPPORT_TEST_FRONTEND OFF)
    ENDIF()
    IF(SUPPORT_X11_FRONTEND)
        MESSAGE(WARNING "Disabling X11 front end because Windows front end is enabled")
        SET(SUPPORT_X11_FRONTEND OFF)
    ENDIF()
    IF(SUPPORT_SDL_SOUND)
        MESSAGE(WARNING "Disabling SDL sound because Windows front end is enabled")
        SET(SUPPORT_SDL_SOUND OFF)
    ENDIF()
    IF(SUPPORT_SDL2_SOUND)
        MESSAGE(WARNING "Disabling SDL2 sound because Windows front end is enabled")
        SET(SUPPORT_SDL2_SOUND OFF)
    ENDIF()
    # Set up the path for the source of the resource file.
    STRING(TOLOWER ${OUR_EXECUTABLE_NAME} OUR_WINDOWS_RC_BASE)
    STRING(CONCAT SinglePlayerSrc/win/ ${OUR_WINDOWS_RC_BASE} .rc OUR_WINDOWS_RC)
ENDIF()

# If none of SC_INSTALL, SHARED_INSTALL, and READONLY_INSTALL are set, use
# SC_INSTALL.
IF((NOT SC_INSTALL) AND (NOT SHARED_INSTALL) AND (NOT READONLY_INSTALL))
    SET(SC_INSTALL ON)
ENDIF()
IF(SHARED_INSTALL)
    IF(SC_INSTALL)
        MESSAGE(FATAL_ERROR "Conflicting options, SHARED_INSTALL and SC_INSTALL, enabled")
    ENDIF()
    IF(READONLY_INSTALL)
        MESSAGE(FATAL_ERROR "Conflicting options, SHARED_INSTALL and READONLY_INSTALL, enabled")
    ENDIF()
    IF(SUPPORT_WINDOWS_FRONTEND)
        MESSAGE(FATAL_ERROR "Conflicting options, SHARED_INSTALL and SUPPORT_WINDOWS_FRONTEND, enabled")
    ENDIF()
ENDIF()
IF(READONLY_INSTALL)
    IF(SC_INSTALL)
        MESSAGE(FATAL_ERROR "Conflicting options, READONLY_INSTALL and SC_INSTALL, enabled")
    ENDIF()
    IF(SUPPORT_WINDOWS_FRONTEND)
        MESSAGE(FATAL_ERROR "Conflicting options, READONLY_INSTALL and SUPPORT_WINDOWS_FRONTEND, enabled")
    ENDIF()
ENDIF()

ADD_LIBRARY(OurCoreLib OBJECT
        SinglePlayerSrc/buildid.c
        SinglePlayerSrc/cave-map.c
        SinglePlayerSrc/cave-square.c
        SinglePlayerSrc/cave-view.c
        SinglePlayerSrc/cave.c
        SinglePlayerSrc/cmd-cave.c
        SinglePlayerSrc/cmd-core.c
        SinglePlayerSrc/cmd-misc.c
        SinglePlayerSrc/cmd-obj.c
        SinglePlayerSrc/cmd-pickup.c
        SinglePlayerSrc/cmd-spoil.c
        SinglePlayerSrc/cmd-wizard.c
        SinglePlayerSrc/datafile.c
        SinglePlayerSrc/debug.c
        SinglePlayerSrc/effect-handler-attack.c
        SinglePlayerSrc/effect-handler-general.c
        SinglePlayerSrc/effects.c
        SinglePlayerSrc/effects-info.c
        SinglePlayerSrc/game-event.c
        SinglePlayerSrc/game-input.c
        SinglePlayerSrc/game-world.c
        SinglePlayerSrc/gen-cave.c
        SinglePlayerSrc/gen-chunk.c
        SinglePlayerSrc/gen-monster.c
        SinglePlayerSrc/gen-room.c
        SinglePlayerSrc/gen-util.c
        SinglePlayerSrc/generate.c
        SinglePlayerSrc/grafmode.c
        SinglePlayerSrc/guid.c
        SinglePlayerSrc/init.c
        SinglePlayerSrc/load.c
        SinglePlayerSrc/message.c
        SinglePlayerSrc/mon-attack.c
        SinglePlayerSrc/mon-blows.c
        SinglePlayerSrc/mon-desc.c
        SinglePlayerSrc/mon-group.c
        SinglePlayerSrc/mon-init.c
        SinglePlayerSrc/mon-list.c
        SinglePlayerSrc/mon-lore.c
        SinglePlayerSrc/mon-make.c
        SinglePlayerSrc/mon-move.c
        SinglePlayerSrc/mon-msg.c
        SinglePlayerSrc/mon-predicate.c
        SinglePlayerSrc/mon-spell.c
        SinglePlayerSrc/mon-summon.c
        SinglePlayerSrc/mon-timed.c
        SinglePlayerSrc/mon-util.c
        SinglePlayerSrc/obj-chest.c
        SinglePlayerSrc/obj-curse.c
        SinglePlayerSrc/obj-desc.c
        SinglePlayerSrc/obj-gear.c
        SinglePlayerSrc/obj-ignore.c
        SinglePlayerSrc/obj-info.c
        SinglePlayerSrc/obj-init.c
        SinglePlayerSrc/obj-knowledge.c
        SinglePlayerSrc/obj-list.c
        SinglePlayerSrc/obj-make.c
        SinglePlayerSrc/obj-pile.c
        SinglePlayerSrc/obj-power.c
        SinglePlayerSrc/obj-properties.c
        SinglePlayerSrc/obj-randart.c
        SinglePlayerSrc/obj-slays.c
        SinglePlayerSrc/obj-tval.c
        SinglePlayerSrc/obj-util.c
        SinglePlayerSrc/option.c
        SinglePlayerSrc/parser.c
        SinglePlayerSrc/player-attack.c
        SinglePlayerSrc/player-birth.c
        SinglePlayerSrc/player-calcs.c
        SinglePlayerSrc/player-class.c
        SinglePlayerSrc/player-history.c
        SinglePlayerSrc/player-path.c
        SinglePlayerSrc/player-properties.c
        SinglePlayerSrc/player-quest.c
        SinglePlayerSrc/player-race.c
        SinglePlayerSrc/player-spell.c
        SinglePlayerSrc/player-timed.c
        SinglePlayerSrc/player-util.c
        SinglePlayerSrc/player.c
        SinglePlayerSrc/project-feat.c
        SinglePlayerSrc/project-mon.c
        SinglePlayerSrc/project-obj.c
        SinglePlayerSrc/project-player.c
        SinglePlayerSrc/project.c
        SinglePlayerSrc/randname.c
        SinglePlayerSrc/save.c
        SinglePlayerSrc/save-charoutput.c
        SinglePlayerSrc/savefile.c
        SinglePlayerSrc/score.c
        SinglePlayerSrc/sound-core.c
        SinglePlayerSrc/source.c
        SinglePlayerSrc/store.c
        SinglePlayerSrc/target.c
        SinglePlayerSrc/trap.c
        SinglePlayerSrc/ui-birth.c
        SinglePlayerSrc/ui-command.c
        SinglePlayerSrc/ui-context.c
        SinglePlayerSrc/ui-curse.c
        SinglePlayerSrc/ui-death.c
        SinglePlayerSrc/ui-display.c
        SinglePlayerSrc/ui-effect.c
        SinglePlayerSrc/ui-entry-combiner.c
        SinglePlayerSrc/ui-entry-renderers.c
        SinglePlayerSrc/ui-entry.c
        SinglePlayerSrc/ui-equip-cmp.c
        SinglePlayerSrc/ui-event.c
        SinglePlayerSrc/ui-game.c
        SinglePlayerSrc/ui-help.c
        SinglePlayerSrc/ui-history.c
        SinglePlayerSrc/ui-init.c
        SinglePlayerSrc/ui-input.c
        SinglePlayerSrc/ui-keymap.c
        SinglePlayerSrc/ui-knowledge.c
        SinglePlayerSrc/ui-map.c
        SinglePlayerSrc/ui-menu.c
        SinglePlayerSrc/ui-mon-list.c
        SinglePlayerSrc/ui-mon-lore.c
        SinglePlayerSrc/ui-obj-list.c
        SinglePlayerSrc/ui-object.c
        SinglePlayerSrc/ui-options.c
        SinglePlayerSrc/ui-output.c
        SinglePlayerSrc/ui-player-properties.c
        SinglePlayerSrc/ui-player.c
        SinglePlayerSrc/ui-prefs.c
        SinglePlayerSrc/ui-score.c
        SinglePlayerSrc/ui-signals.c
        SinglePlayerSrc/ui-spell.c
        SinglePlayerSrc/ui-spoil.c
        SinglePlayerSrc/ui-store.c
        SinglePlayerSrc/ui-target.c
        SinglePlayerSrc/ui-term.c
        SinglePlayerSrc/ui-visuals.c
        SinglePlayerSrc/ui-wizard.c
        SinglePlayerSrc/wiz-debug.c
        SinglePlayerSrc/wiz-spoil.c
        SinglePlayerSrc/wiz-stats.c
        SinglePlayerSrc/z-bitflag.c
        SinglePlayerSrc/z-color.c
        SinglePlayerSrc/z-dice.c
        SinglePlayerSrc/z-expression.c
        SinglePlayerSrc/z-file.c
        SinglePlayerSrc/z-form.c
        SinglePlayerSrc/z-quark.c
        SinglePlayerSrc/z-queue.c
        SinglePlayerSrc/z-rand.c
        SinglePlayerSrc/z-set.c
        SinglePlayerSrc/z-textblock.c
        SinglePlayerSrc/z-type.c
        SinglePlayerSrc/z-util.c
        SinglePlayerSrc/z-virt.c
)

SET_TARGET_PROPERTIES(OurCoreLib PROPERTIES C_STANDARD 99)
SET(ANGBAND_CORE_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src")
SET(ANGBAND_CORE_LINK_LIBRARIES "")
FIND_LIBRARY(MATH_LIBRARY m)
IF(MATH_LIBRARY)
    LIST(APPEND ANGBAND_CORE_LINK_LIBRARIES ${MATH_LIBRARY})
ENDIF()

IF(SUPPORT_SDL_SOUND OR SUPPORT_SDL2_SOUND)
    ADD_LIBRARY(OurSoundSupportLib OBJECT
            SinglePlayerSrc/snd-sdl.c
    )
    SET_TARGET_PROPERTIES(OurSoundSupportLib PROPERTIES C_STANDARD 99)
    SET(SOUND_SUPPORT_LIB OurSoundSupportLib)
ELSE()
    SET(SOUND_SUPPORT_LIB "")
ENDIF()

ADD_EXECUTABLE(OurExecutable
        $<TARGET_OBJECTS:OurCoreLib>
        $<$<BOOL:${SOUND_SUPPORT_LIB}>:$<TARGET_OBJECTS:${SOUND_SUPPORT_LIB}>>
        $<$<BOOL:${SUPPORT_GCU_FRONTEND}>:SinglePlayerSrc/main-gcu.c>
        $<$<BOOL:${SUPPORT_SDL_FRONTEND}>:SinglePlayerSrc/main-sdl.c>
        $<$<BOOL:${SUPPORT_SDL2_FRONTEND}>:SinglePlayerSrc/main-sdl2.c>
        $<$<BOOL:${SUPPORT_WINDOWS_FRONTEND}>:${OUR_WINDOWS_RC}>
        $<$<BOOL:${SUPPORT_WINDOWS_FRONTEND}>:SinglePlayerSrc/main-win.c>
        $<$<BOOL:${SUPPORT_WINDOWS_FRONTEND}>:SinglePlayerSrc/win/readdib.c>
        $<$<BOOL:${SUPPORT_WINDOWS_FRONTEND}>:SinglePlayerSrc/win/readpng.c>
        $<$<BOOL:${SUPPORT_WINDOWS_FRONTEND}>:SinglePlayerSrc/win/scrnshot.c>
        $<$<BOOL:${SUPPORT_WINDOWS_FRONTEND}>:SinglePlayerSrc/win/win-layout.c>
        $<$<BOOL:${SUPPORT_X11_FRONTEND}>:SinglePlayerSrc/main-x11.c>
        $<$<BOOL:${SUPPORT_SPOIL_FRONTEND}>:SinglePlayerSrc/main-spoil.c>
        $<$<BOOL:${SUPPORT_STATS_FRONTEND}>:SinglePlayerSrc/main-stats.c>
        $<$<BOOL:${SUPPORT_STATS_FRONTEND}>:SinglePlayerSrc/stats/db.c>
        $<$<BOOL:${SUPPORT_TEST_FRONTEND}>:SinglePlayerSrc/main-test.c>
        $<$<NOT:$<BOOL:${SUPPORT_WINDOWS_FRONTEND}>>:SinglePlayerSrc/main.c>
)

SET_TARGET_PROPERTIES(OurExecutable PROPERTIES
    C_STANDARD 99
    OUTPUT_NAME ${OUR_EXECUTABLE_NAME}
)
TARGET_INCLUDE_DIRECTORIES(OurExecutable PRIVATE
    ${ANGBAND_CORE_INCLUDE_DIRS}
)
TARGET_LINK_LIBRARIES(OurExecutable PRIVATE ${ANGBAND_CORE_LINK_LIBRARIES})

IF (SUPPORT_X11_FRONTEND)
    INCLUDE(SinglePlayerSrc/cmake/macros/X11_Frontend.cmake)
    CONFIGURE_X11_FRONTEND(OurExecutable)
ENDIF()
IF(SUPPORT_GCU_FRONTEND)
    INCLUDE(SinglePlayerSrc/cmake/macros/GCU_Frontend.cmake)
    CONFIGURE_GCU_FRONTEND(OurExecutable)
ENDIF()
IF(SUPPORT_SDL_FRONTEND)
    INCLUDE(SinglePlayerSrc/cmake/macros/SDL_Frontend.cmake)
    CONFIGURE_SDL_FRONTEND(OurExecutable)
ELSEIF(SUPPORT_SDL2_FRONTEND)
    INCLUDE(SinglePlayerSrc/cmake/macros/SDL2_Frontend.cmake)
    CONFIGURE_SDL2_FRONTEND(OurExecutable)
ENDIF()
IF(SUPPORT_WINDOWS_FRONTEND)
    INCLUDE(SinglePlayerSrc/cmake/macros/WINDOWS_Frontend.cmake)
    CONFIGURE_WINDOWS_FRONTEND(OurExecutable NO)
    CONFIGURE_WINDOWS_FRONTEND(OurCoreLib YES)
ENDIF()

IF(SUPPORT_SDL_SOUND)
    INCLUDE(SinglePlayerSrc/cmake/macros/SDL_Sound.cmake)
    CONFIGURE_SDL_SOUND(OurExecutable NO)
    CONFIGURE_SDL_SOUND(OurCoreLib YES)
    CONFIGURE_SDL_SOUND(${SOUND_SUPPORT_LIB} NO)
ENDIF()

IF (SUPPORT_SDL2_SOUND)
    INCLUDE(SinglePlayerSrc/cmake/macros/SDL2_Sound.cmake)
    CONFIGURE_SDL2_SOUND(OurExecutable NO)
    CONFIGURE_SDL2_SOUND(OurCoreLib YES)
    CONFIGURE_SDL2_SOUND(${SOUND_SUPPORT_LIB} NO)
ENDIF()

IF (SUPPORT_SPOIL_FRONTEND)

    INCLUDE(SinglePlayerSrc/cmake/macros/SPOIL_Frontend.cmake)
    CONFIGURE_SPOIL_FRONTEND(OurExecutable)

ENDIF()

IF(SUPPORT_STATS_FRONTEND)
    INCLUDE(SinglePlayerSrc/cmake/macros/STATS_Frontend.cmake)
    CONFIGURE_STATS_FRONTEND(OurExecutable)
ENDIF()

IF(SUPPORT_STATS_BACKEND)
    INCLUDE(SinglePlayerSrc/cmake/macros/STATS_Backend.cmake)
    IF(SUPPORT_STATS_FRONTEND)
        CONFIGURE_STATS_BACKEND(OurExecutable)
    ENDIF()
    CONFIGURE_STATS_BACKEND(OurCoreLib)
ENDIF()

IF(SUPPORT_TEST_FRONTEND)
    INCLUDE(SinglePlayerSrc/cmake/macros/TEST_Frontend.cmake)
    CONFIGURE_TEST_FRONTEND(OurExecutable)
ENDIF()

# Set the build ID.
IF(SUPPORT_WINDOWS_FRONTEND)
    # Just check for the version file left in a snapshot.  If not in a snapshot,
    # don't bother to set the build ID.
    IF(EXISTS ./version)
        EXECUTE_PROCESS(COMMAND type version WORKING_DIRECTORY . OUTPUT_VARIABLE ANGBAND_BUILD_ID)
        IF(ANGBAND_BUILD_ID MATCHES "^[ \t\n\r]*$")
            SET(ANGBAND_BUILD_ID "")
        ENDIF()
    ELSE()
        SET(ANGBAND_BUILD_ID "")
    ENDIF()
ELSE()
    # Assumes a Unix-like environment.
    EXECUTE_PROCESS(COMMAND "${CMAKE_SOURCE_DIR}/scripts/version.sh" WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/scripts" OUTPUT_VARIABLE ANGBAND_BUILD_ID)
    IF(ANGBAND_BUILD_ID MATCHES "^[ \t\n\r]*$")
        SET(ANGBAND_BUILD_ID "")
    ENDIF()
ENDIF()
IF(NOT (ANGBAND_BUILD_ID STREQUAL ""))
    SET(ANGBAND_BUILD_ID_OPTION "BUILD_ID=${ANGBAND_BUILD_ID}")
ELSE()
    SET(ANGBAND_BUILD_ID_OPTION "")
ENDIF()
TARGET_COMPILE_DEFINITIONS(OurExecutable PRIVATE "${ANGBAND_BUILD_ID_OPTION}")
TARGET_COMPILE_DEFINITIONS(OurCoreLib PRIVATE "${ANGBAND_BUILD_ID_OPTION}")

# Set up to generate documentation if requested.
IF(BUILD_DOC)
    INCLUDE(SinglePlayerSrc/cmake/modules/SphinxDocument.cmake)

    # Use a custom target to rewrite some of the configuration information
    # for sphinx-build.  Need to bring over the rest of the configuration
    # information from the source directories because of that.
    SET(_OUTDIR "${CMAKE_CURRENT_BINARY_DIR}/manual-conf")
    ADD_CUSTOM_TARGET(
        OurManualConf ALL
        "${CMAKE_COMMAND}" "-E" "make_directory" "${_OUTDIR}"
        COMMAND "${CMAKE_COMMAND}" "-E" "copy_directory"
        "${CMAKE_CURRENT_SOURCE_DIR}/docs/_static"
        "${_OUTDIR}/_static"
        COMMAND "${CMAKE_COMMAND}" "-E" "copy_directory"
        "${CMAKE_CURRENT_SOURCE_DIR}/docs/_templates"
        "${_OUTDIR}/_templates"
        VERBATIM
    )
    SET_TARGET_PROPERTIES(OurManualConf PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${_OUTDIR}")
    SET(_SCRIPT "SinglePlayerSrc/cmake/scripts/rewrite.cmake")
    SET(_IN "docs/conf.py")
    SET(_OUT "conf.py")
    ADD_CUSTOM_COMMAND(
        TARGET OurManualConf POST_BUILD
        COMMAND "${CMAKE_COMMAND}"
        "-P" "${CMAKE_CURRENT_SOURCE_DIR}/${_SCRIPT}" --
        "${_OUTDIR}/${_OUT}"
        "${CMAKE_CURRENT_SOURCE_DIR}/${_IN}"
        "DOC_VERSION" "${ANGBAND_BUILD_ID}"
        "DOC_HTML_THEME" "${DOC_HTML_THEME}"
        DEPENDS "${_IN}" "${_SCRIPT}"
        BYPRODUCTS "${_OUTDIR}/${_OUT}"
        VERBATIM
    )

    # Add the target to generate the documentation.
    ADD_SPHINX_DOCUMENT(
        OurManual
        SOURCE_DIR docs
        CONF_TARGET OurManualConf
        OUTPUT_FORMATS html
        OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/manual-output"
        ALL
        BUILD_FAIL_NO_SPHINX
    )

    IF(SC_INSTALL)
        # Add another target to transfer the built documentation to the
        # self-contained installation.  Don't do that as an install target
        # since everything else will be put in place by the build process
        # without an install step.
        SPHINX_DOCUMENT_GET_OUTPUT_DIRECTORY(_DOC OurManual)
        SPHINX_DOCUMENT_GET_OUTPUT_FORMATS(_FORMATS OurManual)
        IF(_DOC STREQUAL "_DOC-NOTFOUND")
            MESSAGE(FATAL_ERROR "logic error:  Sphinx output directory not set for OurManual target")
        ENDIF()
        IF(_FORMATS STREQUAL "_FORMATS-NOTFOUND")
            MESSAGE(FATAL_ERROR "logic error:  Sphinx output formats not set for OurManual target")
        ENDIF()
        IF(NOT (_DOC MATCHES "/$"))
            SET(_DOC "${_DOC}/")
        ENDIF()
        SET(_OUTDIR "${CMAKE_CURRENT_BINARY_DIR}/docs")
        UNSET(_COMMANDS)
        FOREACH(_X ${_FORMATS})
            # Strip out the format-specific subdirectory when transferring
            # so there's less nesting.  Works okay with only HTML output;
            # will there be name conflicts if multiple formats are used?
            LIST(
                APPEND
                _COMMANDS
                "COMMAND" "${CMAKE_COMMAND}" "-E" "copy_directory"
                "${_DOC}${_X}/"
                "${_OUTDIR}"
            )
        ENDFOREACH()
        ADD_CUSTOM_TARGET(
            OurManualTransfer ALL
            "${CMAKE_COMMAND}" "-E" "make_directory" "${_OUTDIR}"
            ${_COMMANDS}
            VERBATIM
        )
        ADD_DEPENDENCIES(OurManualTransfer OurManual)
    ENDIF()
ENDIF()

SET(EXECUTABLE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
SET(DIR_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
SET(STRICT_VARDATADIR_PERMISSIONS ${DIR_PERMISIONS})
SET(LOOSE_VARDATADIR_PERMISSIONS ${DIR_PERMISIONS})
SET(DATA_PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ)

IF(SHARED_INSTALL)
    INCLUDE(SinglePlayerSrc/cmake/macros/GID_Handling.cmake)
    CONFIGURE_GID_HANDLING(OurExecutable)
    CONFIGURE_GID_HANDLING(OurCoreLib)
    TARGET_COMPILE_DEFINITIONS(OurExecutable PRIVATE -D SETGID)
    TARGET_COMPILE_DEFINITIONS(OurCoreLib PRIVATE -D SETGID)
    SET(EXECUTABLE_PERMISSIONS ${EXECUTABLE_PERMISSIONS} SETGID)
    # Override so the world has no access (read access for directory listing
    # would be okay but leads to confusing messages, for instance from ls,
    # when search access is not allowed; so disallow read access as well) and
    # group can do anything.  Means the world has to use the elevated
    # privileges of the setgid executatble to do stuff.
    SET(
        STRICT_VARDATADIR_PERMISSIONS
        OWNER_READ OWNER_WRITE OWNER_EXECUTE
        GROUP_READ GROUP_WRITE GROUP_EXECUTE
    )
    # As above, but allow the world read and search privileges.
    SET(
        LOOSE_VARDATADIR_PERMISSIONS
        OWNER_READ OWNER_WRITE OWNER_EXECUTE
        GROUP_READ GROUP_WRITE GROUP_EXECUTE
        WORLD_READ WORLD_EXECUTE
    )
ENDIF()

IF(READONLY_INSTALL)
    TARGET_COMPILE_DEFINITIONS(OurExecutable PRIVATE -D USE_PRIVATE_PATHS)
    TARGET_COMPILE_DEFINITIONS(OurCoreLib PRIVATE -D USE_PRIVATE_PATHS)
ENDIF()

# This is the executable to be used for the end-to-end tests; those will
# be run with the current working direcctory set to CMAKE_CURRENT_BINARY_DIR.
# Builds using an installation step will override this.
SET(TEST_EXECUTABLE ./${OUR_EXECUTABLE_NAME})

IF((SHARED_INSTALL) OR (READONLY_INSTALL))
    # When hardwiring paths, assume lib/gamedata from the source directories
    # is in DEFAULT_LIB_PATH rather than DEFAULT_CONFIG_PATH.  If not set,
    # have to change the installation of lib/gamedata so it's parent is the
    # same as lib/customize.
    TARGET_COMPILE_DEFINITIONS(OurExecutable PRIVATE -D GAMEDATA_IN_LIB)
    TARGET_COMPILE_DEFINITIONS(OurCoreLib PRIVATE -D GAMEDATA_IN_LIB)
    # The paths set here should end in a path separator and must match up with
    # the paths passed to INSTALL().
    SET(ANGBAND_CONFIG_PATH ${CMAKE_INSTALL_FULL_SYSCONFDIR}/${PROJECT_NAME}/)
    SET(ANGBAND_LIB_PATH ${CMAKE_INSTALL_FULL_DATAROOTDIR}/${PROJECT_NAME}/)
    SET(ANGBAND_DATA_PATH ${CMAKE_INSTALL_FULL_SHAREDSTATEDIR}/${PROJECT_NAME}/)
    TARGET_COMPILE_DEFINITIONS(OurExecutable PRIVATE -D DEFAULT_CONFIG_PATH="${ANGBAND_CONFIG_PATH}")
    TARGET_COMPILE_DEFINITIONS(OurExecutable PRIVATE -D DEFAULT_LIB_PATH="${ANGBAND_LIB_PATH}")
    # This is always passed to init_file_paths() but is ignored internally when
    # the USE_PRIVATE_PATHS preprocessor macro is set (i.e. for READONLY_INSTALL
    # builds).
    TARGET_COMPILE_DEFINITIONS(OurExecutable PRIVATE -D DEFAULT_DATA_PATH="${ANGBAND_DATA_PATH}")

    # Set up for installation.
    INSTALL(TARGETS OurExecutable RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} PERMISSIONS ${EXECUTABLE_PERMISSIONS})
    SET(INSTALLED_EXECUTABLE ${CMAKE_INSTALL_FULL_BINDIR}/${OUR_EXECUTABLE_NAME})
    SET(TEST_EXECUTABLE ${INSTALLED_EXECUTABLE})
    SET(ANGBAND_CHANGE_OWNER ${INSTALLED_EXECUTABLE})
    SET(ANGBAND_DIRS gamedata help screens)
    IF((SUPPORT_SDL_FRONTEND) OR (SUPPORT_SDL2_FRONTEND) OR (SUPPORT_WINDOWS_FRONTEND))
        SET(ANGBAND_DIRS ${ANGBAND_DIRS} fonts icons tiles)
    ENDIF()
    IF((SUPPORT_WINDOWS_FRONTEND) OR (SUPPORT_SDL_SOUND) OR (SUPPORT_SDL2_SOUND))
        SET(ANGBAND_DIRS ${ANGBAND_DIRS} sounds)
    ENDIF()
    FOREACH(ANGBAND_DIR ${ANGBAND_DIRS})
        SET(ANGBAND_DEST ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME})
        INSTALL(DIRECTORY lib/${ANGBAND_DIR} DESTINATION ${ANGBAND_DEST} FILE_PERMISSIONS ${DATA_PERMISSIONS} DIRECTORY_PERMISSIONS ${DIR_PERMISSIONS} PATTERN Makefile EXCLUDE PATTERN .deps EXCLUDE)
    ENDFOREACH()
    FOREACH(ANGBAND_DIR customize)
        SET(ANGBAND_DEST ${CMAKE_INSTALL_SYSCONFDIR}/${PROJECT_NAME})
        INSTALL(DIRECTORY lib/${ANGBAND_DIR} DESTINATION ${ANGBAND_DEST} FILE_PERMISSIONS ${DATA_PERMISSIONS} DIRECTORY_PERMISSIONS ${DIR_PERMISSIONS} PATTERN Makefile EXCLUDE PATTERN .deps EXCLUDE)
    ENDFOREACH()
    IF(SHARED_INSTALL)
        FOREACH(ANGBAND_DIR panic save scores)
            SET(ANGBAND_DEST ${CMAKE_INSTALL_SHAREDSTATEDIR}/${PROJECT_NAME})
            IF(NOT (ANGBAND_DIR STREQUAL archive))
                INSTALL(DIRECTORY lib/user/${ANGBAND_DIR} DESTINATION ${ANGBAND_DEST} FILE_PERMISSIONS ${DATA_PERMISSIONS} DIRECTORY_PERMISSIONS ${STRICT_VARDATADIR_PERMISSIONS} PATTERN Makefile EXCLUDE PATTERN .deps EXCLUDE)
            ELSE()
                INSTALL(DIRECTORY lib/user/${ANGBAND_DIR} DESTINATION ${ANGBAND_DEST} FILE_PERMISSIONS ${DATA_PERMISSIONS} DIRECTORY_PERMISSIONS ${LOOSE_VARDATADIR_PERMISSIONS} PATTERN Makefile EXCLUDE PATTERN .deps EXCLUDE)
            ENDIF()
            SET(ANGBAND_CHANGE_OWNER ${ANGBAND_CHANGE_OWNER} ${CMAKE_INSTALL_FULL_SHAREDSTATEDIR}/${PROJECT_NAME}/${ANGBAND_DIR})
        ENDFOREACH()
        INSTALL(CODE "EXECUTE_PROCESS(COMMAND chown root:${INSTALL_GROUP_ID} ${ANGBAND_CHANGE_OWNER} RESULT_VARIABLE ANGBAND_RESULT)\nIF(NOT ANGBAND_RESULT EQUAL 0)\nMESSAGE(FATAL_ERROR \"Changing ownership to root:${INSTALL_GROUP_ID} on ${ANGBAND_CHANGE_OWNER} failed\")\nENDIF()")
        # Since chown can reset the setgid bit that CMake set when installing,
        # turn that bit back on.
        INSTALL(CODE "EXECUTE_PROCESS(COMMAND chmod g+s ${INSTALLED_EXECUTABLE} RESULT_VARIABLE ANGBAND_RESULT)\nIF(NOT ANGBAND_RESULT EQUAL 0)\nMESSAGE(FATAL ERROR \"Resetting setgid bit on ${INSTALLED_EXECUTABLE} failed\")\nENDIF()")
    ENDIF()

    IF(BUILD_DOC)
        # Install the built documentation.
        SPHINX_DOCUMENT_GET_OUTPUT_DIRECTORY(_DOC OurManual)
        SPHINX_DOCUMENT_GET_OUTPUT_FORMATS(_FORMATS OurManual)
        IF(_DOC STREQUAL "_DOC-NOTFOUND")
            MESSAGE(FATAL_ERROR "logic error:  Sphinx output directory not set for OurManual target")
        ENDIF()
        IF(_FORMATS STREQUAL "_FORMATS-NOTFOUND")
            MESSAGE(FATAL_ERROR "logic error:  Sphinx output formats not set for OurManual target")
        ENDIF()
        IF(NOT (_DOC MATCHES "/$"))
            SET(_DOC "${_DOC}/")
        ENDIF()
        FOREACH(_X ${_FORMATS})
            # Remove the format subdirectory when installing for one less
            # level of nesting.  Works okay with just HTML output; may have
            # conflicting names with multiple output types.
            INSTALL(
                DIRECTORY "${_DOC}${_X}/"
                DESTINATION "${CMAKE_INSTALL_DOCDIR}"
                FILE_PERMISSIONS ${DATA_PERMISSIONS}
                DIRECTORY_PERMISSIONS ${DIR_PERMISSIONS}
                PATTERN .buildinfo  EXCLUDE
            )
        ENDFOREACH()
    ENDIF()
ENDIF()

IF(SC_INSTALL)
    SET(ANGBAND_CONFIG_PATH ./lib/)
    SET(ANGBAND_LIB_PATH ./lib/)
    # Would use lib/user here to match up with the preexisting structure from
    # the source directory, but previous CMake builds used this (because of
    # the default setting in config.h).  So, to be compatible if people
    # rebuild on an existing copy, use this.  When there's backward
    # incompatible changes to the save files, could change this since
    # compatiblity with something already on disk will be less of a concern.
    SET(ANGBAND_DATA_PATH ./lib/)
    # The Windows front end doesn't access these so don't set them in that case.
    IF(NOT SUPPORT_WINDOWS_FRONTEND)
        TARGET_COMPILE_DEFINITIONS(OurExecutable PRIVATE -D DEFAULT_CONFIG_PATH="${ANGBAND_CONFIG_PATH}")
        TARGET_COMPILE_DEFINITIONS(OurExecutable PRIVATE -D DEFAULT_LIB_PATH="${ANGBAND_LIB_PATH}")
        TARGET_COMPILE_DEFINITIONS(OurExecutable PRIVATE -D DEFAULT_DATA_PATH="${ANGBAND_DATA_PATH}")
    ENDIF()
    # Provide an install rule which copies the self-contained form to another
    # location.
    INSTALL(TARGETS OurExecutable DESTINATION . PERMISSIONS ${EXECUTABLE_PERMISSIONS})
    IF(SUPPORT_WINDOWS_FRONTEND)
        # DATA_PERMISSIONS matches the permissions those files have in the
        # source tree, but should the installed permissions be
        # EXECUTABLE_PERMISSIONS?
        INSTALL(zlib1.dll DESTINATION . PERMISSIONS ${DATA_PERMISSIONS})
        INSTALL(libpng12.dll DESTINATION . PERMISSIONS ${DATA_PERMISSIONS})
    ENDIF()
    INSTALL(DIRECTORY lib/ DESTINATION lib)
    IF(BUILD_DOC)
        INSTALL(DIRECTORY docs/ DESTINATION docs)
    ENDIF()

    # Set up a target to transfer the files in lib/ at build time.
    ADD_CUSTOM_TARGET(TransferLib ALL)
    ADD_CUSTOM_COMMAND(
        TARGET TransferLib
        PRE_BUILD
        COMMAND "${CMAKE_COMMAND}" "-P"
        "${CMAKE_CURRENT_SOURCE_DIR}/SinglePlayerSrc/cmake/scripts/copy_with_exclude.cmake"
        "--"
        "${CMAKE_CURRENT_BINARY_DIR}"
        "${CMAKE_CURRENT_SOURCE_DIR}/lib"
        "Makefile"
        ".deps"
        COMMENT "Copying needed files"
        VERBATIM
    )

    IF(SUPPORT_WINDOWS_FRONTEND)
        # Transfer the packaged DLLs so there in the same directory as the
        # executable.
        FILE(COPY ${CMAKE_CURRENT_SOURCE_DIR}/SinglePlayerSrc/win/dll/zlib1.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
        FILE(COPY ${CMAKE_CURRENT_SOURCE_DIR}/SinglePlayerSrc/win/dll/libpng12.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
    ENDIF()
ENDIF()

# Set up tests.  For now, don't bother on Windows since haven't worked out
# replacements for the scripts used (SinglePlayerSrc/tests/run-tests and
# SinglePlayerSrc/tests/list-tests).
IF(SUPPORT_WINDOWS_FRONTEND)
    RETURN()
ENDIF()

# First copy some scripts and, as necessary, test case data from the source
# tree.
FILE(COPY ${CMAKE_CURRENT_SOURCE_DIR}/SinglePlayerSrc/tests/run-tests
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/unittests/)
IF(SUPPORT_TEST_FRONTEND)
    FILE(COPY ${CMAKE_CURRENT_SOURCE_DIR}/tests DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
ENDIF()
FILE(COPY ${CMAKE_CURRENT_SOURCE_DIR}/run-tests
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
FILE(COPY ${CMAKE_CURRENT_SOURCE_DIR}/tests/run-test
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/tests)

# Set up target for running all tests.  Those (from tests in the source tree)
# using the test front end will be run directly.  The unit tests will be
# handled through a dependency.  Note that when SHARED_INSTALL or
# READONLY_INSTALL is set, the tests can only be successfully run after
# installing.  In the SHARED_INSTALL case, some unit tests will fail even
# after installation is done because the test case executables are not setgid.
ADD_CUSTOM_TARGET(alltests
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/run-tests ${TEST_EXECUTABLE}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
ADD_CUSTOM_TARGET(allunittests
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/unittests/run-tests
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
IF(SUPPORT_TEST_FRONTEND)
    ADD_DEPENDENCIES(alltests OurExecutable)
    IF(SC_INSTALL)
        ADD_DEPENDENCIES(alltests TransferLib)
    ENDIF()
ENDIF()
ADD_DEPENDENCIES(alltests allunittests)
IF(SC_INSTALL)
    ADD_DEPENDENCIES(allunittests TransferLib)
ENDIF()

# Set up an object library with the shared code for unit tests.
ADD_LIBRARY(OurUnitTestLib OBJECT EXCLUDE_FROM_ALL
        SinglePlayerSrc/tests/test-utils.c
        SinglePlayerSrc/tests/unit-test.c
)
SET_TARGET_PROPERTIES(OurUnitTestLib PROPERTIES C_STANDARD 99)
SET(ANGBAND_UNIT_TEST_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/SinglePlayerSrc/tests")
TARGET_INCLUDE_DIRECTORIES(OurUnitTestLib PRIVATE
    ${ANGBAND_CORE_INCLUDE_DIRS}
)
TARGET_COMPILE_DEFINITIONS(OurUnitTestLib PRIVATE "${ANGBAND_BUILD_ID_OPTION}")
# Use the same definitions for the hardwired paths as were used (or would
# be used if it wasn't the Windows front end) for the executable.
TARGET_COMPILE_DEFINITIONS(OurUnitTestLib PRIVATE -D DEFAULT_CONFIG_PATH="${ANGBAND_CONFIG_PATH}")
TARGET_COMPILE_DEFINITIONS(OurUnitTestLib PRIVATE -D DEFAULT_LIB_PATH="${ANGBAND_LIB_PATH}")
TARGET_COMPILE_DEFINITIONS(OurUnitTestLib PRIVATE -D DEFAULT_DATA_PATH="${ANGBAND_DATA_PATH}")

# Set up targets for exercising the unit tests (from SinglePlayerSrc/tests in the source
# tree).
EXECUTE_PROCESS(COMMAND "${CMAKE_CURRENT_SOURCE_DIR}/SinglePlayerSrc/tests/list-tests" -d
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/SinglePlayerSrc/tests"
    OUTPUT_VARIABLE ANGBAND_TEST_DIRS
)
STRING(REGEX REPLACE "[ \t\n\r]" ";" ANGBAND_TEST_DIR_LIST ${ANGBAND_TEST_DIRS})
FOREACH(ANGBAND_TEST_DIR ${ANGBAND_TEST_DIR_LIST})
    FILE(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/unittests/${ANGBAND_TEST_DIR}")
ENDFOREACH()
EXECUTE_PROCESS(COMMAND "${CMAKE_CURRENT_SOURCE_DIR}/SinglePlayerSrc/tests/list-tests"
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/SinglePlayerSrc/tests"
    OUTPUT_VARIABLE ANGBAND_TEST_CASES
)
STRING(REGEX REPLACE "[ \t\n\r]" ";" ANGBAND_TEST_CASE_LIST ${ANGBAND_TEST_CASES})
SET(ANGBAND_TEST_CASE_INDEX 1)
FOREACH(ANGBAND_TEST_CASE ${ANGBAND_TEST_CASE_LIST})
    SET(ANGBAND_TEST_CASE_NAME "unittest${ANGBAND_TEST_CASE_INDEX}")
    SET(ANGBAND_TEST_CASE_PATH "unittests/${ANGBAND_TEST_CASE}")
    ADD_EXECUTABLE(${ANGBAND_TEST_CASE_NAME} EXCLUDE_FROM_ALL
            "SinglePlayerSrc/tests/${ANGBAND_TEST_CASE}.c"
            $<TARGET_OBJECTS:OurUnitTestLib>
            $<TARGET_OBJECTS:OurCoreLib>
            $<$<BOOL:${SOUND_SUPPORT_LIB}>:$<TARGET_OBJECTS:${SOUND_SUPPORT_LIB}>>
    )
    SET_TARGET_PROPERTIES(${ANGBAND_TEST_CASE_NAME} PROPERTIES
        C_STANDARD 99 OUTPUT_NAME "${ANGBAND_TEST_CASE_PATH}")
    TARGET_INCLUDE_DIRECTORIES(${ANGBAND_TEST_CASE_NAME} PRIVATE
        ${ANGBAND_CORE_INCLUDE_DIRS}
        ${ANGBAND_UNIT_TEST_INCLUDE_DIRS}
    )
    TARGET_COMPILE_DEFINITIONS(${ANGBAND_TEST_CASE_NAME} PRIVATE "${ANGBAND_BUILD_ID_OPTION}")
    TARGET_LINK_LIBRARIES(${ANGBAND_TEST_CASE_NAME} PRIVATE
        ${ANGBAND_CORE_LINK_LIBRARIES}
    )
    IF(SUPPORT_SDL_SOUND)
        CONFIGURE_SDL_SOUND(${ANGBAND_TEST_CASE_NAME} NO)
    ENDIF()
    IF(SUPPORT_SDL2_SOUND)
        CONFIGURE_SDL2_SOUND(${ANGBAND_TEST_CASE_NAME} NO)
    ENDIF()
    ADD_DEPENDENCIES(allunittests ${ANGBAND_TEST_CASE_NAME})
    MATH(EXPR ANGBAND_TEST_CASE_INDEX "${ANGBAND_TEST_CASE_INDEX} + 1")
ENDFOREACH()
