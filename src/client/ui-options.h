/*
 * File: ui-options.h
 * Purpose: Text UI options handling code (everything accessible from '=')
 */
#if (defined(MBandClient)||defined(SPClient)||defined(HybridClient)) && !defined(INCLUDED_UI_OPTIONS_H)
#define INCLUDED_UI_OPTIONS_H

#include "obj-ignore.h"

struct ego_desc
{
    int16_t e_idx;
    uint16_t itype;
    const char *short_name;
};

extern int ego_item_name(char *buf, size_t buf_size, struct ego_desc *desc);
extern void do_cmd_options_birth(void);
extern void do_cmd_options(void);
extern void cleanup_options(void);
extern const char *ignore_name_for_type(ignore_type_t type);
extern const char *quality_name_for_value(uint8_t value);
extern bool ignore_tval(int tval);
extern void do_cmd_keymaps_shortcut(void);

#endif /* INCLUDED_UI_OPTIONS_H */
