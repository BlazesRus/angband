/*
 * File: ui-knowledge.c
 * Purpose: Knowledge screen
 *
 * Copyright (c) 2000-2007 Eytan Zweig, Andrew Doull, Pete Mack.
 * Copyright (c) 2010 Peter Denison, Chris Carr.
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

#include "angband.h"
#include "cave.h"
#include "cmds.h"
#include "effects.h"
#include "effects-info.h"
#include "game-input.h"
#include "game-world.h"
#include "grafmode.h"
#include "init.h"
#include "mon-lore.h"
#include "mon-util.h"
#include "monster.h"
#include "obj-desc.h"
#include "obj-ignore.h"
#include "obj-knowledge.h"
#include "obj-info.h"
#include "obj-make.h"
#include "obj-pile.h"
#include "obj-tval.h"
#include "obj-util.h"
#include "object.h"
#include "player-calcs.h"
#include "player-history.h"
#include "player-util.h"
#include "project.h"
#include "store.h"
#include "target.h"
#include "trap.h"
#include "ui-context.h"
#include "ui-equip-cmp.h"
#include "ui-history.h"
#include "ui-knowledge.h"
#include "ui-menu.h"
#include "ui-mon-list.h"
#include "ui-mon-lore.h"
#include "ui-object.h"
#include "ui-obj-list.h"
#include "ui-options.h"
#include "ui-output.h"
#include "ui-prefs.h"
#include "ui-score.h"
#include "ui-store.h"
#include "ui-target.h"
#ifdef SPClient
#include "wizard.h"
#endif
//#include "c-angband.h"


/*
 * The first part of this file contains the knowledge menus. Generic display
 * routines are followed by sections which implement "subclasses" of the
 * abstract classes represented by member_funcs and group_funcs.
 *
 * After the knowledge menus are various knowledge functions - message review;
 * inventory, equipment, monster and object lists; symbol lookup; and the
 * "locate" command which scrolls the screen around the current dungeon level.
 */


/*
 * Knowledge menu utilities
 */


static void do_cmd_knowledge_aux(int hook, const char *header, bool abort)
{
    ui_event ea = EVENT_ABORT;

    /* Set the hook */
    special_line_type = hook;

    /* Set the header */
    my_strcpy(special_line_header[NTERM_WIN_OVERHEAD], header, sizeof(special_line_header[0]));

    /* Call the file perusal */
    if (!peruse_file() && abort) Term_event_push(&ea);
}


/*
 * MONSTERS
 */


/*
 * Display known monsters
 */
static void do_cmd_knowledge_monsters(const char *title, int row)
{
    do_cmd_knowledge_aux(SPECIAL_FILE_KILL, "Monsters", true);
}


/*
 * ARTIFACTS
 */

#ifdef SPClient
/**
 * These are used for all the object sections
 */
static const grouper object_text_order[] =
{
	{TV_RING,			"Ring"			},
	{TV_AMULET,			"Amulet"		},
	{TV_POTION,			"Potion"		},
	{TV_SCROLL,			"Scroll"		},
	{TV_WAND,			"Wand"			},
	{TV_STAFF,			"Staff"			},
	{TV_ROD,			"Rod"			},
 	{TV_FOOD,			"Food"			},
 	{TV_MUSHROOM,		"Mushroom"		},
	{TV_PRAYER_BOOK,	"Priest Book"	},
	{TV_MAGIC_BOOK,		"Magic Book"	},
	{TV_NATURE_BOOK,	"Nature Book"	},
	{TV_SHADOW_BOOK,	"Shadow Book"	},
	{TV_OTHER_BOOK,		"Mystery Book"	},
	{TV_LIGHT,			"Light"			},
	{TV_FLASK,			"Flask"			},
	{TV_SWORD,			"Sword"			},
	{TV_POLEARM,		"Polearm"		},
	{TV_HAFTED,			"Hafted Weapon" },
	{TV_BOW,			"Bow"			},
	{TV_ARROW,			"Ammunition"	},
	{TV_BOLT,			NULL			},
	{TV_SHOT,			NULL			},
	{TV_SHIELD,			"Shield"		},
	{TV_CROWN,			"Crown"			},
	{TV_HELM,			"Helm"			},
	{TV_GLOVES,			"Gloves"		},
	{TV_BOOTS,			"Boots"			},
	{TV_CLOAK,			"Cloak"			},
	{TV_DRAG_ARMOR,		"Dragon Scale Mail" },
	{TV_HARD_ARMOR,		"Hard Armor"	},
	{TV_SOFT_ARMOR,		"Soft Armor"	},
	{TV_DIGGING,		"Digger"		},
	{TV_GOLD,			"Money"			},
	{0,					NULL			}
};

static int *obj_group_order = NULL;

static void get_artifact_display_name(char *o_name, size_t namelen, int a_idx)
{
	struct object body = OBJECT_NULL, known_body = OBJECT_NULL;
	struct object *obj = &body, *known_obj = &known_body;

	make_fake_artifact(obj, &a_info[a_idx]);
	object_wipe(known_obj);
	object_copy(known_obj, obj);
	obj->known = known_obj;
	object_desc(o_name, namelen, obj,
		ODESC_PREFIX | ODESC_BASE | ODESC_SPOIL, NULL);
	object_wipe(known_obj);
	object_wipe(obj);
}

/**
 * Display an artifact label
 */
static void display_artifact(int col, int row, bool cursor, int oid)
{
	uint8_t attr = curs_attrs[CURS_KNOWN][(int)cursor];
	char o_name[80];

	get_artifact_display_name(o_name, sizeof o_name, oid);

	c_prt(attr, o_name, row, col);
}

/**
 * Look for an artifact
 */
static struct object *find_artifact(struct artifact *artifact)
{
	int y, x, i;
	struct object *obj;

	/* Ground objects */
	for (y = 1; y < cave->height; ++y) {
		for (x = 1; x < cave->width; ++x) {
			struct loc grid = loc(x, y);
			for (obj = square_object(cave, grid); obj; obj = obj->next) {
				if (obj->artifact == artifact) return obj;
			}
		}
	}

	/* Player objects */
	for (obj = player->gear; obj; obj = obj->next) {
		if (obj->artifact == artifact) return obj;
	}

	/* Monster objects */
	for (i = cave_monster_max(cave) - 1; i >= 1; --i) {
		struct monster *mon = cave_monster(cave, i);
		obj = mon ? mon->held_obj : NULL;

		while (obj) {
			if (obj->artifact == artifact) return obj;
			obj = obj->next;
		}
	}

	/* Store objects */
	for (i = 0; i < MAX_STORES; ++i) {
		struct store *s = &stores[i];
		for (obj = s->stock; obj; obj = obj->next) {
			if (obj->artifact == artifact) return obj;
		}
	}

	/* Stored chunk objects */
	for (i = 0; i < chunk_list_max; ++i) {
		struct chunk *c = chunk_list[i];
		int j;
		if (strstr(c->name, "known")) continue;

		/* Ground objects */
		for (y = 1; y < c->height; ++y) {
			for (x = 1; x < c->width; ++x) {
				struct loc grid = loc(x, y);
				for (obj = square_object(c, grid); obj; obj = obj->next) {
					if (obj->artifact == artifact) return obj;
				}
			}
		}

		/* Monster objects */
		for (j = cave_monster_max(c) - 1; j >= 1; --j) {
			struct monster *mon = cave_monster(c, j);
			obj = mon ? mon->held_obj : NULL;

			while (obj) {
				if (obj->artifact == artifact) return obj;
				obj = obj->next;
			}
		}
	}	

	return NULL;
}

/**
 * Show artifact lore
 */
static void desc_art_fake(int a_idx)
{
	struct object *obj, *known_obj = NULL;
	struct object object_body = OBJECT_NULL, known_object_body = OBJECT_NULL;
	bool fake = false;

	char header[120];

	textblock *tb;
	region area = { 0, 0, 0, 0 };

	obj = find_artifact(&a_info[a_idx]);

	/* If it's been lost, make a fake artifact for it */
	if (!obj) {
		fake = true;
		obj = &object_body;
		known_obj = &known_object_body;

		make_fake_artifact(obj, &a_info[a_idx]);
		obj->known = known_obj;
		known_obj->artifact = obj->artifact;
		known_obj->kind = obj->kind;

		/* Check the history entry, to see if it was fully known before it
		 * was lost */
		if (history_is_artifact_known(player, obj->artifact))
			/* Be very careful not to influence anything but this object */
			object_copy(known_obj, obj);
	}

	/* Hack -- Handle stuff */
	handle_stuff(player);

	tb = object_info(obj, OINFO_NONE);
	object_desc(header, sizeof(header), obj,
		ODESC_PREFIX | ODESC_FULL | ODESC_CAPITAL, player);
	if (fake) {
		object_wipe(known_obj);
		object_wipe(obj);
	}

	textui_textblock_show(tb, area, header);
	textblock_free(tb);
}

static int a_cmp_tval(const void *a, const void *b)
{
	const int a_val = *(const int *)a;
	const int b_val = *(const int *)b;
	const struct artifact *a_a = &a_info[a_val];
	const struct artifact *a_b = &a_info[b_val];

	/* Group by */
	int ta = obj_group_order[a_a->tval];
	int tb = obj_group_order[a_b->tval];
	int c = ta - tb;
	if (c) return c;

	/* Order by */
	c = a_a->sval - a_b->sval;
	if (c) return c;
	return strcmp(a_a->name, a_b->name);
}

static const char *kind_name(int gid)
{
	return object_text_order[gid].name;
}

static int art2gid(int oid)
{
	return obj_group_order[a_info[oid].tval];
}

/**
 * Check if the given artifact idx is something we should "Know" about
 */
static bool artifact_is_known(int a_idx)
{
	struct object *obj;

	if (!a_info[a_idx].name)
		return false;

	if (player->wizard)
		return true;

	if (!is_artifact_created(&a_info[a_idx]))
		return false;

	/* Check all objects to see if it exists but hasn't been IDed */
	obj = find_artifact(&a_info[a_idx]);
	if (obj && !object_is_known_artifact(obj))
		return false;

	return true;
}


/**
 * If 'artifacts' is NULL, it counts the number of known artifacts, otherwise
 * it collects the list of known artifacts into 'artifacts' as well.
 */
static int collect_known_artifacts(int *artifacts, size_t artifacts_len)
{
	int a_count = 0;
	int j;

	if (artifacts)
		assert(artifacts_len >= z_info->a_max);

	for (j = 0; j < z_info->a_max; ++j) {
		/* Artifact doesn't exist */
		if (!a_info[j].name) continue;

		if (OPT(player, cheat_xtra) || artifact_is_known(j)) {
			if (artifacts)
				artifacts[a_count++] = j;
			else
				++a_count;
		}
	}

	return a_count;
}
#endif

/*
 * Display known artifacts
 */
static void do_cmd_knowledge_artifacts(const char *title, int row)
{
    do_cmd_knowledge_aux(SPECIAL_FILE_ARTIFACT, "Artifacts", true);
}


/*
 * EGO ITEMS
 */


/*
 * Display known ego items
 */
static void do_cmd_knowledge_ego_items(const char *title, int row)
{
    do_cmd_knowledge_aux(SPECIAL_FILE_EGO, "Ego Items", true);
}


/*
 * ORDINARY OBJECTS
 */


/*
 * Display known objects
 */
static void do_cmd_knowledge_objects(const char *title, int row)
{
    do_cmd_knowledge_aux(SPECIAL_FILE_OBJECT, "Known Objects", true);
}


/*
 * OBJECT RUNES
 */


/*
 * Display known runes
 */
static void do_cmd_knowledge_runes(const char *title, int row)
{
    do_cmd_knowledge_aux(SPECIAL_FILE_RUNE, "Object Runes", true);
}


/*
 * TERRAIN FEATURES
 */


/*
 * Interact with feature visuals
 */
static void do_cmd_knowledge_features(const char *title, int row)
{
    do_cmd_knowledge_aux(SPECIAL_FILE_FEATURE, "Features", true);
}


/*
 * TRAPS
 */


/*
 * Interact with trap visuals
 */
static void do_cmd_knowledge_traps(const char *title, int row)
{
    do_cmd_knowledge_aux(SPECIAL_FILE_TRAP, "Traps", true);
}


/*
 * Main knowledge menus
 */


/*
 * Display hall of fame
 */
static void do_cmd_knowledge_scores(const char *title, int row)
{
    do_cmd_knowledge_aux(SPECIAL_FILE_SCORES, "Hall of Fame", true);
}


/*
 * Display character history
 */
static void do_cmd_knowledge_history(const char *title, int row)
{
    do_cmd_knowledge_aux(SPECIAL_FILE_HISTORY, "Character History", true);
}


/*
 * Display known uniques
 */
static void do_cmd_knowledge_uniques(const char *title, int row)
{
    do_cmd_knowledge_aux(SPECIAL_FILE_UNIQUE, "Known Uniques", true);
}


/*
 * Display party gear
 */
static void do_cmd_knowledge_gear(const char *title, int row)
{
    do_cmd_knowledge_aux(SPECIAL_FILE_GEAR, "Party Gear", true);
}


/*
 * Display owned houses
 */
static void do_cmd_knowledge_houses(const char *title, int row)
{
    do_cmd_knowledge_aux(SPECIAL_FILE_HOUSES, "Owned Houses", true);
}


/*
 * Display visited dungeons and towns
 */
static void do_cmd_knowledge_dungeons(const char *title, int row)
{
    do_cmd_knowledge_aux(SPECIAL_FILE_DUNGEONS, "Visited Dungeons and Towns", true);
}


/*
 * Definition of the "player knowledge" menu.
 */
static menu_action knowledge_actions[] =
{
    {0, 0, "Display object knowledge", do_cmd_knowledge_objects},
    {0, 0, "Display rune knowledge", do_cmd_knowledge_runes},
    {0, 0, "Display artifact knowledge", do_cmd_knowledge_artifacts},
    {0, 0, "Display ego item knowledge", do_cmd_knowledge_ego_items},
    {0, 0, "Display monster knowledge", do_cmd_knowledge_monsters},
    {0, 0, "Display feature knowledge", do_cmd_knowledge_features},
    {0, 0, "Display trap knowledge", do_cmd_knowledge_traps},
    {0, 0, "Display hall of fame", do_cmd_knowledge_scores},
    {0, 0, "Display character history", do_cmd_knowledge_history},
    {0, 0, "Display known uniques", do_cmd_knowledge_uniques},
    {0, 0, "Display party gear", do_cmd_knowledge_gear},
    {0, 0, "Display owned houses", do_cmd_knowledge_houses},
    {0, 0, "Display visited dungeons and towns", do_cmd_knowledge_dungeons}
};


static struct menu knowledge_menu;


/*
 * Display the "player knowledge" menu.
 */
void textui_browse_knowledge(void)
{
    struct menu *menu;
    region knowledge_region = {0, 0, -1, 15};

    /* Initialize the knowledge menu */
    menu = &knowledge_menu;
    menu_init(menu, MN_SKIN_SCROLL, menu_find_iter(MN_ITER_ACTIONS));
    menu_setpriv(menu, N_ELEMENTS(knowledge_actions), knowledge_actions);

    menu->title = "Display current knowledge";
    menu->selections = all_letters_nohjkl;

    screen_save();
    menu_layout(menu, &knowledge_region);

    clear_from(0);
    menu_select(menu, 0, false);

    screen_load(true);
}


/*
 * Other knowledge functions
 */


void do_cmd_players(void)
{
    do_cmd_knowledge_aux(SPECIAL_FILE_PLAYER, "Players", false);
}


/*
 * Recall the most recent message
 */
void do_cmd_message_one(void)
{
    uint8_t color = message_color(0);
    uint16_t count = message_count(0);
    const char *str = message_str(0);
    const char *msg;

    if (count <= 1) msg = str;
    else msg = format("%s <%dx>", str, count);

    /* Hack -- re-color message from string template */
    message_color_hack(msg, &color);

    /* Recall one message XXX XXX XXX */
    c_prt(color, format( "> %s", msg), 0, 0);
}


/*
 * Show previous messages to the user
 *
 * The screen format uses line 0 and 23 for headers and prompts,
 * skips line 1 and 22, and uses line 2 thru 21 for old messages.
 *
 * This command shows you which commands you are viewing, and allows
 * you to "search" for strings in the recall.
 *
 * Note that messages may be longer than NORMAL_WID characters, but they are
 * displayed using "infinite" length, with a special sub-command to
 * "slide" the virtual display to the left or right.
 *
 * Attempt to only highlight the matching portions of the string.
 */
void do_cmd_messages(void)
{
    ui_event ke;
    bool more = true;
    int i, j, n, q;
    int wid, hgt;
    char shower[NORMAL_WID] = "";

    /* Total messages */
    n = messages_num();

    /* Start on first message */
    i = 0;

    /* Start at leftmost edge */
    q = 0;

    /* Get size */
    Term_get_size(&wid, &hgt);

    /* Enter "icky" mode */
    topline_icky = true;

    /* Save screen */
    screen_save();

    /* Process requests until done */
    while (more)
    {
        /* Clear screen */
        Term_clear();

        /* Dump messages */
        for (j = 0; (j < hgt - 4) && (i + j < n); j++)
        {
            const char *msg;
            const char *str = message_str(i + j);
            uint8_t attr = message_color(i + j);
            uint16_t count = message_count(i + j);

            if (count <= 1) msg = str;
            else msg = format("%s <%dx>", str, count);

            /* Hack -- re-color message from string template */
            message_color_hack(msg, &attr);

            /* Apply horizontal scroll */
            msg = ((int)strlen(msg) >= q)? (msg + q): "";

            /* Dump the messages, bottom to top */
            Term_putstr(0, hgt - 3 - j, -1, attr, msg);

            /* Highlight "shower" */
            if (strlen(shower))
            {
                str = msg;

                /* Display matches */
                while ((str = my_stristr(str, shower)) != NULL)
                {
                    int len = strlen(shower);

                    /* Display the match */
                    Term_putstr(str - msg, hgt - 3 - j, len, COLOUR_YELLOW, str);

                    /* Advance */
                    str += len;
                }
            }
        }

        /* Display header */
        prt(format("Message recall (%d-%d of %d), offset %d", i, i + j - 1, n, q), 0, 0);

        /* Display prompt (not very informative) */
        if (strlen(shower))
            prt("[Movement keys to navigate, '-' for next, '=' to find]", hgt - 1, 0);
        else
            prt("[Movement keys to navigate, '=' to find, or ESCAPE to exit]", hgt - 1, 0);

        /* Get a command */
        ke = inkey_ex();

        /* Exit on Escape */
        if (is_exit(ke)) break;

        else if (ke.type == EVT_KBRD)
        {
            switch (ke.key.code)
            {
                /* Find text */
                case '=':
                {
                    int res;

                    /* Get the string to find */
                    prt("Find: ", hgt - 1, 0);
                    res = askfor_ex(shower, sizeof(shower), NULL, false);
                    if (res == 1) more = false;
                    else if (!res)
                    {
                        /* Set to find */
                        ke.key.code = '-';
                    }

                    break;
                }

                /* Scroll left */
                case ARROW_LEFT:
                case '4':
                case 'h':
                {
                    q = ((q >= wid / 2)? (q - wid / 2): 0);
                    break;
                }

                /* Scroll right */
                case ARROW_RIGHT:
                case '6':
                case 'l':
                {
                    q = q + wid / 2;
                    break;
                }

                /* Recall 1 older message */
                case ARROW_UP:
                case '8':
                case 'k':
                {
                    if (i + 1 < n) i += 1;
                    break;
                }

                /* Recall 1 newer message */
                case ARROW_DOWN:
                case '2':
                case 'j':
                case KC_ENTER:
                {
                    i = ((i >= 1)? (i - 1): 0);
                    break;
                }

                /* Recall 20 older messages */
                case KC_PGUP:
                case 'p':
                case ' ':
                {
                    if (i + 20 < n) i += 20;
                    break;
                }

                /* Recall 20 newer messages */
                case KC_PGDOWN:
                case 'n':
                {
                    i = ((i >= 20)? (i - 20): 0);
                    break;
                }
            }

            /* Find the next item */
            if ((ke.key.code == '-') && strlen(shower))
            {
                int16_t z;

                /* Scan messages */
                for (z = i + 1; z < n; z++)
                {
                    /* Search for it */
                    if (my_stristr(message_str(z), shower))
                    {
                        /* New location */
                        i = z;

                        /* Done */
                        break;
                    }
                }
            }
        }
    }

    /* Load screen */
    screen_load(true);

    /* Leave "icky" mode */
    topline_icky = false;
}


/*
 * Display inventory
 */
void do_cmd_inven(void)
{
    struct object *obj = NULL;
    int ret = 3;
    int mode = USE_EQUIP | USE_INVEN | USE_QUIVER | USE_FLOOR;

    if (!player->upkeep->inven[0])
    {
        c_msg_print("You have nothing in your inventory.");
        return;
    }

    /* No context menu while shopping */
    if (store_ctx)
    {
        /* Save the screen */
        screen_save();

        /* Display the inventory */
        show_inven(OLIST_WINDOW | OLIST_WEIGHT | OLIST_QUIVER, NULL);

        inkey_ex();

        /* Restore the screen */
        screen_load(true);

        return;
    }

    /* Start in "inventory" mode */
    mode |= (START_INVEN | SHOW_QUIVER);

    /* Loop this menu until an object context menu says differently */
    while (ret == 3)
    {
        /* Get an item to use a context command on (Display the inventory) */
        if (get_item(&obj, "Select Item: ", NULL, CMD_NULL, NULL, mode))
        {
            if (obj)
            {
                /* Track the object */
                Send_track_object(obj->oidx);

                while ((ret = context_menu_object(obj)) == 2);
            }
        }
        else
            ret = -1;
    }
}


/*
 * Display equipment
 */
void do_cmd_equip(void)
{
    struct object *obj = NULL;
    int ret = 3;
    int mode = USE_EQUIP | USE_INVEN | USE_QUIVER | USE_FLOOR;

    if (!player->upkeep->equip_cnt)
    {
        c_msg_print("You are not wielding or wearing anything.");
        return;
    }

    /* No context menu while shopping */
    if (store_ctx)
    {
        /* Save the screen */
        screen_save();

        /* Display the equipment */
        show_equip(OLIST_WINDOW | OLIST_WEIGHT | OLIST_SEMPTY, NULL);

        inkey_ex();

        /* Restore the screen */
        screen_load(true);

        return;
    }

    /* Start in "equipment" mode */
    mode |= (START_EQUIP | SHOW_EMPTY);

    /* Loop this menu until an object context menu says differently */
    while (ret == 3)
    {
        /* Get an item to use a context command on (Display the equipment) */
        if (get_item(&obj, "Select Item: ", NULL, CMD_NULL, NULL, mode))
        {
            if (obj)
            {
                /* Track the object */
                Send_track_object(obj->oidx);

                while ((ret = context_menu_object(obj)) == 2);
            }
        }
        else
            ret = -1;
    }
}


/*
 * Display quiver
 */
void do_cmd_quiver(void)
{
    struct object *obj = NULL;
    int ret = 3;
    int mode = USE_EQUIP | USE_INVEN | USE_QUIVER | USE_FLOOR;

    if (!player->upkeep->quiver_cnt)
    {
        c_msg_print("You have nothing in your quiver.");
        return;
    }

    /* No context menu while shopping */
    if (store_ctx)
    {
        /* Save the screen */
        screen_save();

        /* Display the quiver */
        show_quiver(OLIST_WINDOW, NULL);

        inkey_ex();

        /* Restore the screen */
        screen_load(true);

        return;
    }

    /* Start in "quiver" mode */
    mode |= (START_QUIVER);

    /* Loop this menu until an object context menu says differently */
    while (ret == 3)
    {
        /* Get an item to use a context command on (Display the quiver) */
        if (get_item(&obj, "Select Item: ", NULL, CMD_NULL, NULL, mode))
        {
            if (obj)
            {
                /* Track the object */
                Send_track_object(obj->oidx);

                while ((ret = context_menu_object(obj)) == 2);
            }
        }
        else
            ret = -1;
    }
}


/*
 * Look command
 */
void do_cmd_look(void)
{
    cmd_target_interactive(TARGET_LOOK);
}


/*
 * Allow the player to examine other sectors on the map
 */
void do_cmd_locate(void)
{
    ui_event ke;

    target_icky_screen = true;

    /* Initialize */
    Send_locate(5);

    /* Show panels until done */
    while (1)
    {
        /* Assume no direction */
        int dir = 0;

        /* Get a direction */
        while (!dir)
        {
            /* Get a command (or Cancel) */
            ke = inkey_ex();

            /* Check for cancel */
            if (is_exit(ke)) break;

            /* Extract direction */
            dir = target_dir(ke.key);

            /* Error */
            if (!dir) bell("Illegal direction for locate!");
        }

        /* No direction */
        if (!dir) break;

        /* Send the command */
        Send_locate(dir);
    }

    /* Done */
    Send_locate(0);

    /* Clear */
    c_msg_print(NULL);

    target_icky_screen = false;
    if (full_icky_screen) Term_redraw();
}


/*
 * Identify a character, allow recall of monsters
 */
void do_cmd_query_symbol(void)
{
    char buf[NORMAL_WID];

    /* Get a name or abort */
    buf[0] = '\0';
    if (!get_string("Enter (partial) monster name: ", buf, sizeof(buf))) return;

    Send_symbol(buf);
}


/*
 * Centers the map on the player
 */
void do_cmd_center_map(void)
{
    Send_center_map();
}


/*
 * Display the main-screen monster list.
 */
void do_cmd_monlist(void)
{
    Send_monlist();
}


/*
 * Display the main-screen item list.
 */
void do_cmd_itemlist(void)
{
    Send_objlist();
}