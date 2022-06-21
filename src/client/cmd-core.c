/*
 * File: cmd-core.c
 * Purpose: Handles the queueing of game commands
 *
 * Copyright (c) 2008-9 Antony Sidwell
 * Copyright (c) 2014 Andi Sidwell
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
#ifndef REDUCEHEADERSPerFile
#include "c-angband.h"

#else
#include "cmd-core.h"
#include "angband.h"
#include "cmds.h"
#include "effects-info.h"
#include "game-input.h"
#include "obj-chest.h"
#include "obj-desc.h"
#include "obj-tval.h"
#include "obj-util.h"
#include "player-attack.h"
#include "player-birth.h"
#include "player-calcs.h"
#include "player-spell.h"
#include "player-timed.h"
#include "player-util.h"
#include "store.h"
#include "target.h"
#endif

/*
 * Command handlers will take a pointer to the command structure
 * so that they can access any arguments supplied.
 */
typedef int (*cmd_handler_fn)(struct command *cmd);


/*
 * A simple list of commands and their handling functions.
 */
struct command_info
{
    cmd_code cmd;
    const char *verb;
    cmd_handler_fn fn;
};


static const struct command_info game_cmds[] =
{
    {CMD_GO_UP, "go up stairs", Send_go_up},
    {CMD_GO_DOWN, "go down stairs", Send_go_down},
    {CMD_TOGGLE_STEALTH, "toggle stealth mode", Send_toggle_stealth},
    {CMD_WALK, "walk", Send_walk},
    {CMD_JUMP, "jump", Send_jump},
    {CMD_INSCRIBE, "inscribe", Send_inscribe},
    {CMD_UNINSCRIBE, "un-inscribe", Send_uninscribe},
    {CMD_TAKEOFF, "take off", Send_take_off},
    {CMD_WIELD, "wear or wield", Send_wield},
    {CMD_DROP, "drop", Send_drop},
    {CMD_BROWSE_SPELL, "browse", textui_spell_browse},
    {CMD_STUDY, "study", Send_gain},
    {CMD_CAST, "cast", cmd_cast},
    {CMD_USE_STAFF, "use", Send_use},
    {CMD_USE_WAND, "aim", Send_aim},
    {CMD_USE_ROD, "zap", Send_zap},
    {CMD_ACTIVATE, "activate", Send_activate},
    {CMD_EAT, "eat", Send_eat},
    {CMD_QUAFF, "quaff", Send_quaff},
    {CMD_READ_SCROLL, "read", Send_read},
    {CMD_REFILL, "refuel with", Send_fill},
    {CMD_USE, "use", Send_use_any},
    {CMD_FIRE, "fire", Send_fire},
    {CMD_THROW, "throw", Send_throw},
    {CMD_PICKUP, "pickup", Send_pickup},
    {CMD_AUTOPICKUP, "autopickup", Send_autopickup},
    {CMD_DISARM, "disarm", Send_disarm},
    {CMD_TUNNEL, "tunnel", Send_tunnel},
    {CMD_OPEN, "open", Send_open},
    {CMD_CLOSE, "close", Send_close},
    {CMD_HOLD, "stay still", Send_hold},
    {CMD_RUN, "run", Send_run},
    {CMD_ALTER, "alter", Send_alter},
    {CMD_STEAL, "steal", Send_steal},
    {CMD_BREATH, "breathe", Send_breath},
    {CMD_PROJECT, "project", cmd_project},
    {CMD_EXAMINE, "examine", Send_observe}
//From Angband
#ifndef DISABLEFeature_CommandEffect
	,{CMD_COMMAND_MONSTER, "make a monster act", do_cmd_mon_command}
#endif
};

#ifdef ENABLEFeature_EnableExtraCMDArgs
const char *cmd_verb(cmd_code cmd)
{
	size_t i;
	for (i = 0; i < N_ELEMENTS(game_cmds); ++i) {
		if (game_cmds[i].cmd == cmd)
			return game_cmds[i].verb;
	}
	return NULL;
}
#endif

/*
 * Return the index of the given command in the command array.
 */
static int cmd_idx(cmd_code code)
{
    size_t i;

    for (i = 0; i < N_ELEMENTS(game_cmds); i++)
    {
        if (game_cmds[i].cmd == code) return i;
    }

    return CMD_ARG_NOT_PRESENT;
}

#ifdef ENABLEFeature_EnableExtraCMDArgs
/**
 * ------------------------------------------------------------------------
 * The command queue.
 * ------------------------------------------------------------------------ */

#define CMD_QUEUE_SIZE 20
#define prev_cmd_idx(idx) ((idx + CMD_QUEUE_SIZE - 1) % CMD_QUEUE_SIZE)

static int cmd_head = 0;
static int cmd_tail = 0;
static struct command cmd_queue[CMD_QUEUE_SIZE];

static bool repeat_prev_allowed = false;
static bool repeating = false;

/**
 * Insert the given command into the command queue.
 */
errr cmdq_push_copy(struct command *cmd)
{
	/* If queue full, return error */
	if (cmd_head + 1 == cmd_tail) return 1;
	if (cmd_head + 1 == CMD_QUEUE_SIZE && cmd_tail == 0) return 1;

	/* Insert command into queue. */
	if (cmd->code != CMD_REPEAT) {
		cmd_queue[cmd_head] = *cmd;
	} else if (!repeat_prev_allowed) {
		return 1;
	} else {
		int idx_to_repeat = cmdq_find_repeat_target_index();

		if (idx_to_repeat < 0) {
			return 1;
		}
		/*
		 * If we're repeating a command, we duplicate the previous
		 * command in the next command "slot".
		 */
		cmd_queue[cmd_head] = cmd_queue[idx_to_repeat];
	}

	/* Advance point in queue, wrapping around at the end */
	cmd_head++;
	if (cmd_head == CMD_QUEUE_SIZE) cmd_head = 0;

	return 0;	
}
#endif

/*
 * Process a game command from the UI or the command queue and carry out
 * whatever actions go along with it.
 */
void process_command(cmd_code ctx)
{
    struct command cmd;

    /* If we've got a command to process, do it. */
#ifndef DISABLEFeature_CommandEffect
	/* Hack - command a monster */
	int idx = cmd_idx(player->timed[TMD_COMMAND] ?
		CMD_COMMAND_MONSTER : cmd->code);
#else
    int idx = cmd_idx(ctx);
#endif

    if (idx == -1) return;

    memset(&cmd, 0, sizeof(cmd));
    cmd.code = ctx;

    /* Process the command */
    if (game_cmds[idx].fn) game_cmds[idx].fn(&cmd);
}

#ifdef ENABLEFeature_EnableExtraCMDArgs
/**
 * Process a game command from the UI or the command queue and carry out
 * whatever actions go along with it.
 */
static void process_commandV2(cmd_context ctx, struct command *cmd)
{
	int oldrepeats = cmd->nrepeats;
#ifndef DISABLEFeature_CommandEffect
	/* Hack - command a monster */
	int idx = cmd_idx(player->timed[TMD_COMMAND] ?
		CMD_COMMAND_MONSTER : cmd->code);
#else
    int idx = cmd_idx(ctx);
#endif

	/* Reset so that when selecting items, we look in the default location */
	player->upkeep->command_wrk = 0;

	if (idx == -1) return;

	/* Command repetition */
	if (game_cmds[idx].repeat_allowed) {
		/* Auto-repeat only if there isn't already a repeat length. */
		if (game_cmds[idx].auto_repeat_n > 0 && cmd->nrepeats == 0)
			cmd_set_repeat(game_cmds[idx].auto_repeat_n);
	} else {
		cmd->nrepeats = 0;
		repeating = false;
	}

	/* The command gets to unset this if it isn't appropriate for
	 * the user to repeat it. */
	repeat_prev_allowed = true;

	cmd->context = ctx;

	/* Actually execute the command function */
	if (game_cmds[idx].fn) {
		/* Occasional attack instead for bloodlust-affected characters */
		if (randint0(200) < player->timed[TMD_BLOODLUST]) {
			if (player_attack_random_monster(player)) return;
		}
		game_cmds[idx].fn(cmd);
	}

	/* If the command hasn't changed nrepeats, count this execution. */
	if (cmd->nrepeats > 0 && oldrepeats == cmd_get_nrepeats())
		cmd_set_repeat(oldrepeats - 1);
}

/**
 * Get the next game command from the queue and process it.
 */
bool cmdq_pop(cmd_context c)
{
	struct command *cmd;

	/* If we're repeating, just pull the last command again. */
	if (repeating) {
		cmd = &cmd_queue[prev_cmd_idx(cmd_tail)];
	} else if (cmd_head != cmd_tail) {
		/* If we have a command ready, set it. */
		cmd = &cmd_queue[cmd_tail++];
		if (cmd_tail == CMD_QUEUE_SIZE)
			cmd_tail = 0;
	} else {
		/* Failure to get a command. */
		return false;
	}

	/* Now process it */
	process_command(c, cmd);
	return true;
}

/**
 * Inserts a command in the queue to be carried out, with the given
 * number of repeats.
 */
errr cmdq_push_repeat(cmd_code c, int nrepeats)
{
	struct command cmd = {
		.context = CTX_INIT,
		.code = CMD_NULL,
		.nrepeats = 0,
		.is_background_command = false,
		.arg = { { 0 } }
	};

	if (cmd_idx(c) == -1)
		return 1;

	cmd.code = c;
	cmd.nrepeats = nrepeats;

	return cmdq_push_copy(&cmd);
}

/**
 * Inserts a command in the queue to be carried out. 
 */
errr cmdq_push(cmd_code c)
{
	return cmdq_push_repeat(c, 0);
}


/**
 * Shorthand to execute all commands in the queue right now, no waiting
 * for input.
 */
void cmdq_execute(cmd_context ctx)
{
	while (cmdq_pop(ctx)) ;
}

/**
 * Remove all commands from the queue.
 */
void cmdq_flush(void)
{
	cmd_tail = cmd_head;
}

/**
 * ------------------------------------------------------------------------
 * Handling of repeated commands
 * ------------------------------------------------------------------------ */

/**
 * Remove any pending repeats from the current command.
 */
void cmd_cancel_repeat(void)
{
	struct command *cmd = &cmd_queue[prev_cmd_idx(cmd_tail)];

	if (cmd->nrepeats || repeating) {
		/* Cancel */
		cmd->nrepeats = 0;
		repeating = false;

		/* Redraw the state (later) */
		player->upkeep->redraw |= (PR_STATE);
	}
}

/**
 * Update the number of repeats pending for the current command.
 */
void cmd_set_repeat(int nrepeats)
{
	struct command *cmd = &cmd_queue[prev_cmd_idx(cmd_tail)];

	cmd->nrepeats = nrepeats;
	if (nrepeats) repeating = true;
	else repeating = false;

	/* Redraw the state (later) */
	player->upkeep->redraw |= (PR_STATE);
}

/**
 * Return the number of repeats pending for the current command.
 */
int cmd_get_nrepeats(void)
{
	struct command *cmd = &cmd_queue[prev_cmd_idx(cmd_tail)];
	return cmd->nrepeats;
}

/**
 * Do not allow the current command to be repeated by the user using the
 * "repeat last command" command.
 */
void cmd_disable_repeat(void)
{
	repeat_prev_allowed = false;
}

/**
 * Do not allow the current command to be repeated by the user using the
 * "repeat last command" command if that command used an item from the floor.
 */
void cmd_disable_repeat_floor_item(void)
{
	int cmd_prev;

	/*
	 * Repeat already disallowed so skip further checks (avoids access
	 * to dangling object references in the command structures).
	 */
	if (!repeat_prev_allowed) return;

	cmd_prev = cmd_head - 1;
	if (cmd_prev < 0) cmd_prev = CMD_QUEUE_SIZE - 1;
	if (cmd_queue[cmd_prev].code != CMD_NULL) {
		struct command *cmd = &cmd_queue[cmd_prev];
		int i = 0;

		while (1) {
			if (i >= CMD_MAX_ARGS) {
				break;
			}
			if (cmd->arg[i].type == arg_ITEM
					&& cmd->arg[i].data.obj
					&& (cmd->arg[i].data.obj->grid.x != 0
					|| cmd->arg[i].data.obj->grid.y != 0)) {
				repeat_prev_allowed = false;
				break;
			}
			++i;
		}
	}
}
#endif

/*
 * Argument setting/getting generics
 */


/*
 * Set an argument of name 'arg' to data 'data'
 */
static void cmd_set_arg(struct command *cmd, const char *name, enum cmd_arg_type type,
    union cmd_arg_data data)
{
    size_t i;
    int first_empty = -1;
    int idx = -1;

    assert(name);
    assert(name[0]);

    /* Find an arg that either... */
    for (i = 0; i < CMD_MAX_ARGS; i++)
    {
        struct cmd_arg *arg = &cmd->arg[i];

        if (!arg->name[0] && first_empty == -1)
            first_empty = i;

        if (streq(arg->name, name))
        {
            idx = i;
            break;
        }
    }

    assert(first_empty != -1 || idx != -1);

    if (idx == -1)
        idx = first_empty;

    cmd->arg[idx].type = type;
    cmd->arg[idx].data = data;
    my_strcpy(cmd->arg[idx].name, name, sizeof(cmd->arg[0].name));
}


/*
 * Get an argument with name 'arg'
 */
static int cmd_get_arg(struct command *cmd, const char *arg, enum cmd_arg_type type,
    union cmd_arg_data *data)
{
    size_t i;

    for (i = 0; i < CMD_MAX_ARGS; i++)
    {
        if (streq(cmd->arg[i].name, arg))
        {
            if (cmd->arg[i].type != type)
                return CMD_ARG_WRONG_TYPE;

            *data = cmd->arg[i].data;
            return CMD_OK;
        }
    }

    return CMD_ARG_NOT_PRESENT;
}

#ifdef ENABLEFeature_EnableExtraCMDArgs
/**
 * ------------------------------------------------------------------------
 * 'Choice' type
 * ------------------------------------------------------------------------ */

/**
 * XXX This type is a hack. The only places that use this are:
 * - resting
 * - birth choices
 * - store items
 * - spells
 * - selecting an effect for an item that activates for an EF_SELECT effect
 *   (dragon's breath wands or potions, dragon armor that has multiple breath
 *   types)
 * - several debugging commands for integer or boolean arguments that did not
 *   seem to be a good match for 'number' arguments
 *
 * Each of these should have its own type, which will allow for proper
 * validity checking of data.
 */

/**
 * Set arg 'n' to 'choice'
 */
void cmd_set_arg_choice(struct command *cmd, const char *arg, int choice)
{
	union cmd_arg_data data;
	data.choice = choice;
	cmd_set_arg(cmd, arg, arg_CHOICE, data);
}

/**
 * Retrive an argument 'n' if it's a choice
 */
int cmd_get_arg_choice(struct command *cmd, const char *arg, int *choice)
{
	union cmd_arg_data data;
	int err;

	if ((err = cmd_get_arg(cmd, arg, arg_CHOICE, &data)) == CMD_OK)
		*choice = data.choice;

	return err;
}


/**
 * Get a spell from the user, trying the command first but then prompting
 *
 * \param cmd is the command to use.
 * \param arg is the name of the command's argument that stores the spell's
 * index.
 * \param p is the player.
 * \param spell is dereferenced and set to the index of the spell selected.
 * \param verb is the string describing the action for which the spell is
 * requested.  It is typically "cast" or "study".
 * \param book_filter is the function (if any) to test that an object is
 * appropriate for use as a spellbook by the player.
 * \param book_error is the message to display if no valid book is available.
 * If NULL, no message will be displayed.
 * \param spell_filter is the function to call to test if a spell is a valid
 * selection for the request.
 * \param spell_error is the message to display if no valid spell is available.
 * If NULL, no message will be displayed.
 * \return CMD_OK if a valid spell index was assigned to *spell or
 * CMD_ARG_ABORTED if no valid spell index was assigned to *spell.
 */
int cmd_get_spell(struct command *cmd, const char *arg, struct player *p,
		int *spell,
		const char *verb, item_tester book_filter,
		const char *book_error,
		bool (*spell_filter)(const struct player *p, int spell),
		const char *spell_error)
{
	struct object *book;

	/* See if we've been provided with this one */
	if (cmd_get_arg_choice(cmd, arg, spell) == CMD_OK) {
		/* Ensure it passes the filter */
		if (!spell_filter || spell_filter(p, *spell) == true)
			return CMD_OK;
	}

	/* See if we've been given a book to look at */
	if (cmd_get_arg_item(cmd, "book", &book) == CMD_OK) {
		*spell = get_spell_from_book(p, verb, book, spell_error,
			spell_filter);
	} else {
		*spell = get_spell(p, verb, book_filter, cmd->code, book_error,
			spell_filter, spell_error, &book);
	}

	if (*spell >= 0) {
		cmd_set_arg_item(cmd, "book", book);
		cmd_set_arg_choice(cmd, arg, *spell);
		return CMD_OK;
	}

	return CMD_ARG_ABORTED;
}

/**
 * Choose an effect from a list, first try the command but then prompt
 * \param cmd is the command to use.
 * \param arg is the name of the argument to consult in the command
 * \param choice When the return value is CMD_OK, *choice will be set to the
 * index in the list for the selected effect or to -2 if the user selected the
 * random option enabled by allow_random.
 * \param prompt Is the text for the prompt displayed when querying the user.
 * May be NULL to use a default prompt.
 * \param effect points to the first effect in the linked list of effects.
 * \param count is the number of effects from which to choose.  If count is -1,
 * all the effects in the list will be used.
 * \param allow_random when true, present the user an additional option which
 * will choose one of the effects at random; when false, only present the
 * options that correspond to the effects in the list.
 * \return CMD_OK if *choice was updated with a valid selection; otherwise
 * return CMD_ARG_ABORTED.
 */
int cmd_get_effect_from_list(struct command *cmd, const char *arg, int *choice,
	const char *prompt, struct effect *effect, int count,
	bool allow_random)
{
	int selection;

	if (count == -1) {
		struct effect *cursor = effect;

		count = 0;
		while (cursor) {
			++count;
			cursor = effect_next(cursor);
		}
	}

	if (cmd_get_arg_choice(cmd, arg, &selection) != CMD_OK ||
			((selection != -2 || !allow_random) &&
			(selection < 0 || selection >= count))) {
		/* It isn't in the command or is invalid; prompt. */
		selection = get_effect_from_list(prompt, effect, count,
			allow_random);
	}
	if ((selection == -2 && allow_random) ||
			(selection >= 0 && selection < count)) {
		/* Record the selection in the command. */
		cmd_set_arg_choice(cmd, arg, selection);
		*choice = selection;
		return CMD_OK;
	}
	return CMD_ARG_ABORTED;
}
#endif

/*
 * Strings
 */


/*
 * Set arg 'n' to given string
 */
void cmd_set_arg_string(struct command *cmd, const char *arg, const char *str)
{
    union cmd_arg_data data;

    data.string = string_make(str);
    cmd_set_arg(cmd, arg, arg_STRING, data);
}


/*
 * Retrieve arg 'n' if a string
 */
int cmd_get_arg_string(struct command *cmd, const char *arg, const char **str)
{
    union cmd_arg_data data;
    int err;

    if ((err = cmd_get_arg(cmd, arg, arg_STRING, &data)) == CMD_OK)
        *str = data.string;

    return err;
}


/*
 * Get a string, first from the command or failing that prompt the user
 */
int cmd_get_string(struct command *cmd, const char *arg, const char **str, const char *initial,
    const char *title, const char *prompt)
{
    char tmp[NORMAL_WID] = "";

    if (cmd_get_arg_string(cmd, arg, str) == CMD_OK)
        return CMD_OK;

    /* Introduce */
    if (title) c_msg_print(title);

    /* Prompt properly */
    if (initial)
        my_strcpy(tmp, initial, sizeof(tmp));

    if (get_string(prompt, tmp, sizeof(tmp)))
    {
        cmd_set_arg_string(cmd, arg, tmp);
        if (cmd_get_arg_string(cmd, arg, str) == CMD_OK)
            return CMD_OK;
    }

    return CMD_ARG_ABORTED;
}


/*
 * Numbers, quantities
 */


/*
 * Set argument 'n' to 'number'
 */
void cmd_set_arg_number(struct command *cmd, const char *arg, int amt)
{
    union cmd_arg_data data;

    data.number = amt;
    cmd_set_arg(cmd, arg, arg_NUMBER, data);
}


/*
 * Get argument 'n' as a number
 */
int cmd_get_arg_number(struct command *cmd, const char *arg, int *amt)
{
    union cmd_arg_data data;
    int err;

    if ((err = cmd_get_arg(cmd, arg, arg_NUMBER, &data)) == CMD_OK)
        *amt = data.number;

    return err;
}


/*
 * Get argument 'n' as a number; failing that, prompt for input
 */
int cmd_get_quantity(struct command *cmd, const char *arg, int *amt, int max, bool plural)
{
    if (cmd_get_arg_number(cmd, arg, amt) == CMD_OK)
        return CMD_OK;

    *amt = 1;

    if (!plural || (max > 1))
    {
        *amt = get_quantity(NULL, max);
        if (*amt > 0)
        {
            cmd_set_arg_number(cmd, arg, *amt);
            return CMD_OK;
        }

        return CMD_ARG_ABORTED;
    }

    return CMD_OK;
}

#ifdef ENABLEFeature_EnableExtraCMDArgs
/**
 * ------------------------------------------------------------------------
 * Directions
 * ------------------------------------------------------------------------ */

/**
 * Set arg 'n' to given direction
 */
void cmd_set_arg_direction(struct command *cmd, const char *arg, int dir)
{
	union cmd_arg_data data;
	data.direction = dir;
	cmd_set_arg(cmd, arg, arg_DIRECTION, data);
}

/**
 * Retrieve arg 'n' if a direction
 */
int cmd_get_arg_direction(struct command *cmd, const char *arg, int *dir)
{
	union cmd_arg_data data;
	int err;

	if ((err = cmd_get_arg(cmd, arg, arg_DIRECTION, &data)) == CMD_OK)
		*dir = data.direction;

	return err;
}

/**
 * Get a direction, first from command or prompt otherwise
 */
int cmd_get_direction(struct command *cmd, const char *arg, int *dir,
					  bool allow_5)
{
	if (cmd_get_arg_direction(cmd, arg, dir) == CMD_OK) {
		/* Validity check */
		if (*dir != DIR_NONE)
			return CMD_OK;
	}

	/* We need to do extra work */
	if (get_rep_dir(dir, allow_5)) {
		cmd_set_arg_direction(cmd, arg, *dir);
		return CMD_OK;
	}

	cmd_cancel_repeat();
	return CMD_ARG_ABORTED;
}

/**
 * ------------------------------------------------------------------------
 * Targets
 * ------------------------------------------------------------------------ */

/**
 * XXX Should this be unified with the arg_DIRECTION type?
 *
 * XXX Should we abolish DIR_TARGET and instead pass a struct target which
 * contains all relevant info?
 */

/**
 * Set arg 'n' to target
 */
void cmd_set_arg_target(struct command *cmd, const char *arg, int target)
{
	union cmd_arg_data data;
	data.direction = target;
	cmd_set_arg(cmd, arg, arg_TARGET, data);
}

/**
 * Retrieve arg 'n' if it's a target
 */
int cmd_get_arg_target(struct command *cmd, const char *arg, int *target)
{
	union cmd_arg_data data;
	int err;

	if ((err = cmd_get_arg(cmd, arg, arg_TARGET, &data)) == CMD_OK)
		*target = data.direction;

	return err;
}

/**
 * Get a target, first from command or prompt otherwise
 */
int cmd_get_target(struct command *cmd, const char *arg, int *target)
{
	if (cmd_get_arg_target(cmd, arg, target) == CMD_OK) {
		if (*target != DIR_UNKNOWN &&
				(*target != DIR_TARGET || target_okay()))
			return CMD_OK;
	}

	if (get_aim_dir(target)) {
		cmd_set_arg_target(cmd, arg, *target);
		return CMD_OK;
	}

	return CMD_ARG_ABORTED;
}

/**
 * ------------------------------------------------------------------------
 * Points
 * ------------------------------------------------------------------------ */

/**
 * Set argument 'n' to point grid
 */
void cmd_set_arg_point(struct command *cmd, const char *arg, struct loc grid)
{
	union cmd_arg_data data;
	data.point = grid;
	cmd_set_arg(cmd, arg, arg_POINT, data);
}

/**
 * Retrieve argument 'n' if it's a point
 */
int cmd_get_arg_point(struct command *cmd, const char *arg, struct loc *grid)
{
	union cmd_arg_data data;
	int err;

	if ((err = cmd_get_arg(cmd, arg, arg_POINT, &data)) == CMD_OK) {
		*grid = data.point;
	}

	return err;
}
#endif

/*
 * Item arguments
 */


/*
 * Set argument 'n' to 'item'
 */
void cmd_set_arg_item(struct command *cmd, const char *arg, struct object *obj)
{
    union cmd_arg_data data;

    data.obj = obj;
    cmd_set_arg(cmd, arg, arg_ITEM, data);
}


/*
 * Retrieve argument 'n' as an item
 */
int cmd_get_arg_item(struct command *cmd, const char *arg, struct object **obj)
{
    union cmd_arg_data data;
    int err;

    if ((err = cmd_get_arg(cmd, arg, arg_ITEM, &data)) == CMD_OK)
        *obj = data.obj;

    return err;
}


/*
 * Get an item, first from the command or try the UI otherwise
 */
int cmd_get_item(struct command *cmd, const char *arg, struct object **obj, const char *prompt,
    const char *reject, item_tester filter, int mode)
{
    if (cmd_get_arg_item(cmd, arg, obj) == CMD_OK)
        return CMD_OK;

#ifdef ENABLEFeature_RestrictShapechangedPlayerInv
	/* Shapechanged players can only access the floor */
	if (player_is_shapechanged(player)) {
		mode &= ~(USE_EQUIP | USE_INVEN | USE_QUIVER);
	}
#endif

    if (get_item(obj, prompt, reject, cmd->code, filter, mode))
    {
        cmd_set_arg_item(cmd, arg, *obj);
        return CMD_OK;
    }

    return CMD_ARG_ABORTED;
}


/*
 * Targets
 */


/*
 * Set arg 'n' to target
 */
void cmd_set_arg_target(struct command *cmd, const char *arg, int target)
{
    union cmd_arg_data data;

    data.direction = target;
    cmd_set_arg(cmd, arg, arg_DIRECTION, data);
}


/*
 * Retrieve arg 'n' if it's a target
 */
int cmd_get_arg_target(struct command *cmd, const char *arg, int *target)
{
    union cmd_arg_data data;
    int err;

    if ((err = cmd_get_arg(cmd, arg, arg_DIRECTION, &data)) == CMD_OK)
        *target = data.direction;

    return err;
}


/*
 * Get a target, first from command or prompt otherwise
 */
int cmd_get_target(struct command *cmd, const char *arg, int *target)
{
    if (cmd_get_arg_target(cmd, arg, target) == CMD_OK)
        return CMD_OK;

    if (*target == DIR_SKIP)
    {
        *target = DIR_UNKNOWN;
        cmd_set_arg_target(cmd, arg, *target);
        return CMD_OK;
    }

    if (get_aim_dir(target))
    {
        cmd_set_arg_target(cmd, arg, *target);
        return CMD_OK;
    }

    return CMD_ARG_ABORTED;
}
