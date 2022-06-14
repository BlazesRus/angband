/*
 * File: buildid.h
 * Purpose: Version strings
 */
#ifndef BUILDID
#define BUILDID
/*
 * Name of this Angband variant
 */
#define VERSION_NAME    "Tangaria/Tangband"

extern bool beta_version(void);
extern uint16_t current_version(void);
extern uint16_t min_version(void);
extern char *version_build(const char *label, bool build);
#endif /* BUILDID */
