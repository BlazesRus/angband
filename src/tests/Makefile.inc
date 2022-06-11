#
# File: Makefile.inc
#
# Holds dependency information for the test cases.  Expects TESTOBJS to
# be set by whatever includes this (src/tests/Makefile normally).
#

depgen:
	head -n 16 Makefile.inc > Makefile.new
	@for i in $(TESTOBJS:.o=.c); do \
		DIR=`echo ./$$i | sed 's|/[^/]*$$||'`; \
			$(CC) -MM -I. -I.. $$i | \
			sed "s|^\\([^ ]\\)|$$DIR/\\1|" >> Makefile.new; \
	done;

# Dependencies
./artifact/name.o: artifact/name.c unit-test.h unit-test-types.h ../common/z-util.h \
 ../common/h-basic.h unit-test-data.h ../common/angband.h ../common/z-bitflag.h ../common/z-form.h \
 ../common/z-virt.h ../common/z-color.h ../common/z-util.h ../common/z-rand.h ../common/config.h \
 ../client/game-event.h ../common/z-type.h ../server/message.h ../common/list-message.h ../server/player.h \
 ../common/guid.h ../server/obj-properties.h ../common/z-file.h ../common/list-tvals.h \
 ../common/list-object-flags.h ../common/list-kind-flags.h ../common/list-stats.h \
 ../common/list-object-modifiers.h ../server/object.h ../server/z-quark.h ../common/z-dice.h \
 ../common/z-expression.h ../common/list-elements.h ../server/list-origins.h ../common/option.h \
 ../common/list-options.h ../common/list-player-flags.h ../server/init.h ../common/datafile.h \
 ../common/parser.h ../common/list-parser-errors.h ../server/mon-lore.h ../server/z-textblock.h \
 ../server/monster.h ../server/cave.h ../common/list-square-flags.h ../server/list-terrain-flags.h \
 ../server/target.h ../server/mon-predicate.h ../server/mon-timed.h ../common/list-mon-timed.h \
 ../server/mon-blows.h ../common/list-mon-temp-flags.h ../common/list-mon-race-flags.h \
 ../common/list-mon-spells.h ../server/monster.h ../common/obj-tval.h ../server/player.h \
 ../server/player-calcs.h ../server/project.h ../common/source.h ../common/list-projections.h \
 ../server/obj-properties.h ../server/obj-randart.h ../server/list-randart-properties.h \
 ../server/object.h
./cave/scatter.o: cave/scatter.c unit-test.h unit-test-types.h ../common/z-util.h \
 ../common/h-basic.h test-utils.h ../server/cave.h ../common/z-type.h ../common/z-bitflag.h \
 ../common/z-form.h ../common/z-virt.h ../common/list-square-flags.h ../server/list-terrain-flags.h \
 ../server/init.h ../common/z-file.h ../common/z-rand.h ../common/datafile.h ../server/object.h ../server/z-quark.h \
 ../common/z-dice.h ../common/z-expression.h ../server/obj-properties.h ../common/list-tvals.h \
 ../common/list-object-flags.h ../common/list-kind-flags.h ../common/list-stats.h \
 ../common/list-object-modifiers.h ../common/list-elements.h ../server/list-origins.h \
 ../common/parser.h ../common/list-parser-errors.h
./command/lookup.o: command/lookup.c unit-test.h unit-test-types.h ../common/z-util.h \
 ../common/h-basic.h ../server/obj-properties.h ../common/z-file.h ../common/z-bitflag.h ../common/z-form.h \
 ../common/z-virt.h ../common/list-tvals.h ../common/list-object-flags.h ../common/list-kind-flags.h \
 ../common/list-stats.h ../common/list-object-modifiers.h ../server/object.h ../common/z-type.h \
 ../server/z-quark.h ../common/z-dice.h ../common/z-rand.h ../common/z-expression.h \
 ../server/obj-properties.h ../common/list-elements.h ../server/list-origins.h ../server/cmds.h \
 ../server/cave.h ../common/list-square-flags.h ../server/list-terrain-flags.h ../client/cmd-core.h \
 ../server/object.h ../client/ui-keymap.h ../client/ui-event.h ../client/ui-game.h ../client/game-event.h \
 ../client/ui-input.h ../client/ui-event.h ../client/ui-term.h ../common/z-virt.h
./effects/chain.o: effects/chain.c unit-test.h unit-test-types.h ../common/z-util.h \
 ../common/h-basic.h test-utils.h ../server/cave.h ../common/z-type.h ../common/z-bitflag.h \
 ../common/z-form.h ../common/z-virt.h ../common/list-square-flags.h ../server/list-terrain-flags.h \
 ../server/effects.h ../common/source.h ../server/object.h ../server/z-quark.h ../common/z-dice.h \
 ../common/z-rand.h ../common/z-expression.h ../server/obj-properties.h ../common/z-file.h \
 ../common/list-tvals.h ../common/list-object-flags.h ../common/list-kind-flags.h \
 ../common/list-stats.h ../common/list-object-modifiers.h ../common/list-elements.h \
 ../server/list-origins.h ../server/player-attack.h ../client/cmd-core.h ../server/cmds.h ../server/cave.h \
 ../server/list-effects.h ../server/effects-info.h ../server/z-textblock.h ../server/game-world.h \
 ../server/generate.h ../server/monster.h ../server/target.h ../server/mon-predicate.h ../server/mon-timed.h \
 ../common/list-mon-timed.h ../server/mon-blows.h ../server/player.h ../common/guid.h ../common/option.h \
 ../common/list-options.h ../common/list-player-flags.h ../common/list-mon-temp-flags.h \
 ../common/list-mon-race-flags.h ../common/list-mon-spells.h ../server/list-room-flags.h \
 ../server/init.h ../common/datafile.h ../common/parser.h ../common/list-parser-errors.h \
 ../server/mon-make.h ../server/player.h ../server/player-birth.h ../server/player-calcs.h \
 ../server/player-timed.h ../common/list-server/player-timed.h ../common/source.h ../common/z-dice.h
./effects/destruction.o: effects/destruction.c unit-test.h unit-test-types.h \
 ../common/z-util.h ../common/h-basic.h test-utils.h ../server/cave.h ../common/z-type.h \
 ../common/z-bitflag.h ../common/z-form.h ../common/z-virt.h ../common/list-square-flags.h \
 ../server/list-terrain-flags.h ../server/effects.h ../common/source.h ../server/object.h \
 ../server/z-quark.h ../common/z-dice.h ../common/z-rand.h ../common/z-expression.h \
 ../server/obj-properties.h ../common/z-file.h ../common/list-tvals.h ../common/list-object-flags.h \
 ../common/list-kind-flags.h ../common/list-stats.h ../common/list-object-modifiers.h \
 ../common/list-elements.h ../server/list-origins.h ../server/player-attack.h ../client/cmd-core.h \
 ../server/cmds.h ../server/cave.h ../server/list-effects.h ../server/game-world.h ../server/init.h \
 ../common/datafile.h ../common/parser.h ../common/list-parser-errors.h ../server/obj-knowledge.h \
 ../common/angband.h ../common/z-color.h ../common/z-util.h ../common/config.h ../client/game-event.h \
 ../server/message.h ../common/list-message.h ../server/player.h ../common/guid.h ../common/option.h \
 ../common/list-options.h ../common/list-player-flags.h ../server/obj-make.h ../server/obj-pile.h \
 ../server/obj-util.h ../server/player-birth.h ../server/player-util.h
./effects/earthquake.o: effects/earthquake.c unit-test.h unit-test-types.h \
 ../common/z-util.h ../common/h-basic.h test-utils.h ../server/cave.h ../common/z-type.h \
 ../common/z-bitflag.h ../common/z-form.h ../common/z-virt.h ../common/list-square-flags.h \
 ../server/list-terrain-flags.h ../server/effects.h ../common/source.h ../server/object.h \
 ../server/z-quark.h ../common/z-dice.h ../common/z-rand.h ../common/z-expression.h \
 ../server/obj-properties.h ../common/z-file.h ../common/list-tvals.h ../common/list-object-flags.h \
 ../common/list-kind-flags.h ../common/list-stats.h ../common/list-object-modifiers.h \
 ../common/list-elements.h ../server/list-origins.h ../server/player-attack.h ../client/cmd-core.h \
 ../server/cmds.h ../server/cave.h ../server/list-effects.h ../server/game-world.h ../server/init.h \
 ../common/datafile.h ../common/parser.h ../common/list-parser-errors.h ../server/obj-knowledge.h \
 ../common/angband.h ../common/z-color.h ../common/z-util.h ../common/config.h ../client/game-event.h \
 ../server/message.h ../common/list-message.h ../server/player.h ../common/guid.h ../common/option.h \
 ../common/list-options.h ../common/list-player-flags.h ../server/obj-make.h ../server/obj-pile.h \
 ../server/obj-util.h ../server/player-birth.h ../server/player-util.h
./effects/info.o: effects/info.c unit-test.h unit-test-types.h ../common/z-util.h \
 ../common/h-basic.h test-utils.h ../server/effects.h ../common/source.h ../server/object.h \
 ../common/z-type.h ../server/z-quark.h ../common/z-bitflag.h ../common/z-form.h ../common/z-virt.h \
 ../common/z-dice.h ../common/z-rand.h ../common/z-expression.h ../server/obj-properties.h \
 ../common/z-file.h ../common/list-tvals.h ../common/list-object-flags.h ../common/list-kind-flags.h \
 ../common/list-stats.h ../common/list-object-modifiers.h ../common/list-elements.h \
 ../server/list-origins.h ../server/player-attack.h ../client/cmd-core.h ../server/cmds.h ../server/cave.h \
 ../common/list-square-flags.h ../server/list-terrain-flags.h ../server/list-effects.h \
 ../server/effects-info.h ../server/z-textblock.h ../server/init.h ../common/datafile.h ../common/parser.h \
 ../common/list-parser-errors.h ../common/z-dice.h
./game/basic.o: game/basic.c unit-test.h unit-test-types.h ../common/z-util.h \
 ../common/h-basic.h unit-test-data.h ../common/angband.h ../common/z-bitflag.h ../common/z-form.h \
 ../common/z-virt.h ../common/z-color.h ../common/z-util.h ../common/z-rand.h ../common/config.h \
 ../client/game-event.h ../common/z-type.h ../server/message.h ../common/list-message.h ../server/player.h \
 ../common/guid.h ../server/obj-properties.h ../common/z-file.h ../common/list-tvals.h \
 ../common/list-object-flags.h ../common/list-kind-flags.h ../common/list-stats.h \
 ../common/list-object-modifiers.h ../server/object.h ../server/z-quark.h ../common/z-dice.h \
 ../common/z-expression.h ../common/list-elements.h ../server/list-origins.h ../common/option.h \
 ../common/list-options.h ../common/list-player-flags.h ../server/init.h ../common/datafile.h \
 ../common/parser.h ../common/list-parser-errors.h ../server/mon-lore.h ../server/z-textblock.h \
 ../server/monster.h ../server/cave.h ../common/list-square-flags.h ../server/list-terrain-flags.h \
 ../server/target.h ../server/mon-predicate.h ../server/mon-timed.h ../common/list-mon-timed.h \
 ../server/mon-blows.h ../common/list-mon-temp-flags.h ../common/list-mon-race-flags.h \
 ../common/list-mon-spells.h ../server/monster.h ../common/obj-tval.h ../server/player.h \
 ../server/player-calcs.h ../server/project.h ../common/source.h ../common/list-projections.h \
 test-utils.h ../server/cave.h ../client/cmd-core.h ../client/game-event.h ../server/game-world.h \
 ../server/generate.h ../server/list-room-flags.h ../server/mon-make.h ../server/savefile.h \
 ../server/player-birth.h ../client/cmd-core.h ../server/player-timed.h ../common/list-server/player-timed.h
./game/mage.o: game/mage.c unit-test.h unit-test-types.h ../common/z-util.h \
 ../common/h-basic.h unit-test-data.h ../common/angband.h ../common/z-bitflag.h ../common/z-form.h \
 ../common/z-virt.h ../common/z-color.h ../common/z-util.h ../common/z-rand.h ../common/config.h \
 ../client/game-event.h ../common/z-type.h ../server/message.h ../common/list-message.h ../server/player.h \
 ../common/guid.h ../server/obj-properties.h ../common/z-file.h ../common/list-tvals.h \
 ../common/list-object-flags.h ../common/list-kind-flags.h ../common/list-stats.h \
 ../common/list-object-modifiers.h ../server/object.h ../server/z-quark.h ../common/z-dice.h \
 ../common/z-expression.h ../common/list-elements.h ../server/list-origins.h ../common/option.h \
 ../common/list-options.h ../common/list-player-flags.h ../server/init.h ../common/datafile.h \
 ../common/parser.h ../common/list-parser-errors.h ../server/mon-lore.h ../server/z-textblock.h \
 ../server/monster.h ../server/cave.h ../common/list-square-flags.h ../server/list-terrain-flags.h \
 ../server/target.h ../server/mon-predicate.h ../server/mon-timed.h ../common/list-mon-timed.h \
 ../server/mon-blows.h ../common/list-mon-temp-flags.h ../common/list-mon-race-flags.h \
 ../common/list-mon-spells.h ../server/monster.h ../common/obj-tval.h ../server/player.h \
 ../server/player-calcs.h ../server/project.h ../common/source.h ../common/list-projections.h \
 test-utils.h ../server/cave.h ../client/cmd-core.h ../client/game-event.h ../server/game-world.h \
 ../server/generate.h ../server/list-room-flags.h ../server/mon-make.h ../server/savefile.h \
 ../server/player-birth.h ../client/cmd-core.h ../server/player-timed.h ../common/list-server/player-timed.h
./message/message.o: message/message.c unit-test.h unit-test-types.h ../common/z-util.h \
 ../common/h-basic.h unit-test-data.h ../common/angband.h ../common/z-bitflag.h ../common/z-form.h \
 ../common/z-virt.h ../common/z-color.h ../common/z-util.h ../common/z-rand.h ../common/config.h \
 ../client/game-event.h ../common/z-type.h ../server/message.h ../common/list-message.h ../server/player.h \
 ../common/guid.h ../server/obj-properties.h ../common/z-file.h ../common/list-tvals.h \
 ../common/list-object-flags.h ../common/list-kind-flags.h ../common/list-stats.h \
 ../common/list-object-modifiers.h ../server/object.h ../server/z-quark.h ../common/z-dice.h \
 ../common/z-expression.h ../common/list-elements.h ../server/list-origins.h ../common/option.h \
 ../common/list-options.h ../common/list-player-flags.h ../server/init.h ../common/datafile.h \
 ../common/parser.h ../common/list-parser-errors.h ../server/mon-lore.h ../server/z-textblock.h \
 ../server/monster.h ../server/cave.h ../common/list-square-flags.h ../server/list-terrain-flags.h \
 ../server/target.h ../server/mon-predicate.h ../server/mon-timed.h ../common/list-mon-timed.h \
 ../server/mon-blows.h ../common/list-mon-temp-flags.h ../common/list-mon-race-flags.h \
 ../common/list-mon-spells.h ../server/monster.h ../common/obj-tval.h ../server/player.h \
 ../server/player-calcs.h ../server/project.h ../common/source.h ../common/list-projections.h \
 ../server/message.h ../client/game-event.h ../common/z-color.h ../common/z-virt.h
./monster/attack.o: monster/attack.c unit-test.h unit-test-types.h ../common/z-util.h \
 ../common/h-basic.h unit-test-data.h ../common/angband.h ../common/z-bitflag.h ../common/z-form.h \
 ../common/z-virt.h ../common/z-color.h ../common/z-util.h ../common/z-rand.h ../common/config.h \
 ../client/game-event.h ../common/z-type.h ../server/message.h ../common/list-message.h ../server/player.h \
 ../common/guid.h ../server/obj-properties.h ../common/z-file.h ../common/list-tvals.h \
 ../common/list-object-flags.h ../common/list-kind-flags.h ../common/list-stats.h \
 ../common/list-object-modifiers.h ../server/object.h ../server/z-quark.h ../common/z-dice.h \
 ../common/z-expression.h ../common/list-elements.h ../server/list-origins.h ../common/option.h \
 ../common/list-options.h ../common/list-player-flags.h ../server/init.h ../common/datafile.h \
 ../common/parser.h ../common/list-parser-errors.h ../server/mon-lore.h ../server/z-textblock.h \
 ../server/monster.h ../server/cave.h ../common/list-square-flags.h ../server/list-terrain-flags.h \
 ../server/target.h ../server/mon-predicate.h ../server/mon-timed.h ../common/list-mon-timed.h \
 ../server/mon-blows.h ../common/list-mon-temp-flags.h ../common/list-mon-race-flags.h \
 ../common/list-mon-spells.h ../server/monster.h ../common/obj-tval.h ../server/player.h \
 ../server/player-calcs.h ../server/project.h ../common/source.h ../common/list-projections.h \
 ../server/mon-attack.h ../common/option.h ../server/player-timed.h ../common/list-server/player-timed.h \
 ../client/ui-input.h ../client/cmd-core.h ../client/ui-event.h ../client/ui-term.h
./monster/monster.o: monster/monster.c unit-test.h unit-test-types.h ../common/z-util.h \
 ../common/h-basic.h unit-test-data.h ../common/angband.h ../common/z-bitflag.h ../common/z-form.h \
 ../common/z-virt.h ../common/z-color.h ../common/z-util.h ../common/z-rand.h ../common/config.h \
 ../client/game-event.h ../common/z-type.h ../server/message.h ../common/list-message.h ../server/player.h \
 ../common/guid.h ../server/obj-properties.h ../common/z-file.h ../common/list-tvals.h \
 ../common/list-object-flags.h ../common/list-kind-flags.h ../common/list-stats.h \
 ../common/list-object-modifiers.h ../server/object.h ../server/z-quark.h ../common/z-dice.h \
 ../common/z-expression.h ../common/list-elements.h ../server/list-origins.h ../common/option.h \
 ../common/list-options.h ../common/list-player-flags.h ../server/init.h ../common/datafile.h \
 ../common/parser.h ../common/list-parser-errors.h ../server/mon-lore.h ../server/z-textblock.h \
 ../server/monster.h ../server/cave.h ../common/list-square-flags.h ../server/list-terrain-flags.h \
 ../server/target.h ../server/mon-predicate.h ../server/mon-timed.h ../common/list-mon-timed.h \
 ../server/mon-blows.h ../common/list-mon-temp-flags.h ../common/list-mon-race-flags.h \
 ../common/list-mon-spells.h ../server/monster.h ../common/obj-tval.h ../server/player.h \
 ../server/player-calcs.h ../server/project.h ../common/source.h ../common/list-projections.h \
 test-utils.h ../server/mon-util.h ../server/mon-msg.h ../common/list-mon-server/message.h
./object/alloc.o: object/alloc.c unit-test.h unit-test-types.h ../common/z-util.h \
 ../common/h-basic.h unit-test-data.h ../common/angband.h ../common/z-bitflag.h ../common/z-form.h \
 ../common/z-virt.h ../common/z-color.h ../common/z-util.h ../common/z-rand.h ../common/config.h \
 ../client/game-event.h ../common/z-type.h ../server/message.h ../common/list-message.h ../server/player.h \
 ../common/guid.h ../server/obj-properties.h ../common/z-file.h ../common/list-tvals.h \
 ../common/list-object-flags.h ../common/list-kind-flags.h ../common/list-stats.h \
 ../common/list-object-modifiers.h ../server/object.h ../server/z-quark.h ../common/z-dice.h \
 ../common/z-expression.h ../common/list-elements.h ../server/list-origins.h ../common/option.h \
 ../common/list-options.h ../common/list-player-flags.h ../server/init.h ../common/datafile.h \
 ../common/parser.h ../common/list-parser-errors.h ../server/mon-lore.h ../server/z-textblock.h \
 ../server/monster.h ../server/cave.h ../common/list-square-flags.h ../server/list-terrain-flags.h \
 ../server/target.h ../server/mon-predicate.h ../server/mon-timed.h ../common/list-mon-timed.h \
 ../server/mon-blows.h ../common/list-mon-temp-flags.h ../common/list-mon-race-flags.h \
 ../common/list-mon-spells.h ../server/monster.h ../common/obj-tval.h ../server/player.h \
 ../server/player-calcs.h ../server/project.h ../common/source.h ../common/list-projections.h \
 ../server/object.h ../server/obj-make.h ../server/obj-properties.h
./object/attack.o: object/attack.c unit-test.h unit-test-types.h ../common/z-util.h \
 ../common/h-basic.h unit-test-data.h ../common/angband.h ../common/z-bitflag.h ../common/z-form.h \
 ../common/z-virt.h ../common/z-color.h ../common/z-util.h ../common/z-rand.h ../common/config.h \
 ../client/game-event.h ../common/z-type.h ../server/message.h ../common/list-message.h ../server/player.h \
 ../common/guid.h ../server/obj-properties.h ../common/z-file.h ../common/list-tvals.h \
 ../common/list-object-flags.h ../common/list-kind-flags.h ../common/list-stats.h \
 ../common/list-object-modifiers.h ../server/object.h ../server/z-quark.h ../common/z-dice.h \
 ../common/z-expression.h ../common/list-elements.h ../server/list-origins.h ../common/option.h \
 ../common/list-options.h ../common/list-player-flags.h ../server/init.h ../common/datafile.h \
 ../common/parser.h ../common/list-parser-errors.h ../server/mon-lore.h ../server/z-textblock.h \
 ../server/monster.h ../server/cave.h ../common/list-square-flags.h ../server/list-terrain-flags.h \
 ../server/target.h ../server/mon-predicate.h ../server/mon-timed.h ../common/list-mon-timed.h \
 ../server/mon-blows.h ../common/list-mon-temp-flags.h ../common/list-mon-race-flags.h \
 ../common/list-mon-spells.h ../server/monster.h ../common/obj-tval.h ../server/player.h \
 ../server/player-calcs.h ../server/project.h ../common/source.h ../common/list-projections.h \
 ../server/object.h ../server/obj-make.h ../server/player-attack.h ../client/cmd-core.h
./object/util.o: object/util.c unit-test.h unit-test-types.h ../common/z-util.h \
 ../common/h-basic.h unit-test-data.h ../common/angband.h ../common/z-bitflag.h ../common/z-form.h \
 ../common/z-virt.h ../common/z-color.h ../common/z-util.h ../common/z-rand.h ../common/config.h \
 ../client/game-event.h ../common/z-type.h ../server/message.h ../common/list-message.h ../server/player.h \
 ../common/guid.h ../server/obj-properties.h ../common/z-file.h ../common/list-tvals.h \
 ../common/list-object-flags.h ../common/list-kind-flags.h ../common/list-stats.h \
 ../common/list-object-modifiers.h ../server/object.h ../server/z-quark.h ../common/z-dice.h \
 ../common/z-expression.h ../common/list-elements.h ../server/list-origins.h ../common/option.h \
 ../common/list-options.h ../common/list-player-flags.h ../server/init.h ../common/datafile.h \
 ../common/parser.h ../common/list-parser-errors.h ../server/mon-lore.h ../server/z-textblock.h \
 ../server/monster.h ../server/cave.h ../common/list-square-flags.h ../server/list-terrain-flags.h \
 ../server/target.h ../server/mon-predicate.h ../server/mon-timed.h ../common/list-mon-timed.h \
 ../server/mon-blows.h ../common/list-mon-temp-flags.h ../common/list-mon-race-flags.h \
 ../common/list-mon-spells.h ../server/monster.h ../common/obj-tval.h ../server/player.h \
 ../server/player-calcs.h ../server/project.h ../common/source.h ../common/list-projections.h \
 ../server/object.h ../server/obj-make.h ../server/obj-pile.h ../server/obj-util.h
./object/pile.o: object/pile.c unit-test.h unit-test-types.h ../common/z-util.h \
 ../common/h-basic.h unit-test-data.h ../common/angband.h ../common/z-bitflag.h ../common/z-form.h \
 ../common/z-virt.h ../common/z-color.h ../common/z-util.h ../common/z-rand.h ../common/config.h \
 ../client/game-event.h ../common/z-type.h ../server/message.h ../common/list-message.h ../server/player.h \
 ../common/guid.h ../server/obj-properties.h ../common/z-file.h ../common/list-tvals.h \
 ../common/list-object-flags.h ../common/list-kind-flags.h ../common/list-stats.h \
 ../common/list-object-modifiers.h ../server/object.h ../server/z-quark.h ../common/z-dice.h \
 ../common/z-expression.h ../common/list-elements.h ../server/list-origins.h ../common/option.h \
 ../common/list-options.h ../common/list-player-flags.h ../server/init.h ../common/datafile.h \
 ../common/parser.h ../common/list-parser-errors.h ../server/mon-lore.h ../server/z-textblock.h \
 ../server/monster.h ../server/cave.h ../common/list-square-flags.h ../server/list-terrain-flags.h \
 ../server/target.h ../server/mon-predicate.h ../server/mon-timed.h ../common/list-mon-timed.h \
 ../server/mon-blows.h ../common/list-mon-temp-flags.h ../common/list-mon-race-flags.h \
 ../common/list-mon-spells.h ../server/monster.h ../common/obj-tval.h ../server/player.h \
 ../server/player-calcs.h ../server/project.h ../common/source.h ../common/list-projections.h \
 ../server/object.h ../server/obj-pile.h
./object/slays.o: object/slays.c unit-test.h unit-test-types.h ../common/z-util.h \
 ../common/h-basic.h unit-test-data.h ../common/angband.h ../common/z-bitflag.h ../common/z-form.h \
 ../common/z-virt.h ../common/z-color.h ../common/z-util.h ../common/z-rand.h ../common/config.h \
 ../client/game-event.h ../common/z-type.h ../server/message.h ../common/list-message.h ../server/player.h \
 ../common/guid.h ../server/obj-properties.h ../common/z-file.h ../common/list-tvals.h \
 ../common/list-object-flags.h ../common/list-kind-flags.h ../common/list-stats.h \
 ../common/list-object-modifiers.h ../server/object.h ../server/z-quark.h ../common/z-dice.h \
 ../common/z-expression.h ../common/list-elements.h ../server/list-origins.h ../common/option.h \
 ../common/list-options.h ../common/list-player-flags.h ../server/init.h ../common/datafile.h \
 ../common/parser.h ../common/list-parser-errors.h ../server/mon-lore.h ../server/z-textblock.h \
 ../server/monster.h ../server/cave.h ../common/list-square-flags.h ../server/list-terrain-flags.h \
 ../server/target.h ../server/mon-predicate.h ../server/mon-timed.h ../common/list-mon-timed.h \
 ../server/mon-blows.h ../common/list-mon-temp-flags.h ../common/list-mon-race-flags.h \
 ../common/list-mon-spells.h ../server/monster.h ../common/obj-tval.h ../server/player.h \
 ../server/player-calcs.h ../server/project.h ../common/source.h ../common/list-projections.h \
 test-utils.h ../server/mon-spell.h ../server/obj-slays.h ../server/player-birth.h \
 ../client/cmd-core.h ../server/player-timed.h ../common/list-server/player-timed.h ../common/z-color.h \
 ../common/z-virt.h
./parse/a-info.o: parse/a-info.c unit-test.h unit-test-types.h ../common/z-util.h \
 ../common/h-basic.h unit-test-data.h ../common/angband.h ../common/z-bitflag.h ../common/z-form.h \
 ../common/z-virt.h ../common/z-color.h ../common/z-util.h ../common/z-rand.h ../common/config.h \
 ../client/game-event.h ../common/z-type.h ../server/message.h ../common/list-message.h ../server/player.h \
 ../common/guid.h ../server/obj-properties.h ../common/z-file.h ../common/list-tvals.h \
 ../common/list-object-flags.h ../common/list-kind-flags.h ../common/list-stats.h \
 ../common/list-object-modifiers.h ../server/object.h ../server/z-quark.h ../common/z-dice.h \
 ../common/z-expression.h ../common/list-elements.h ../server/list-origins.h ../common/option.h \
 ../common/list-options.h ../common/list-player-flags.h ../server/init.h ../common/datafile.h \
 ../common/parser.h ../common/list-parser-errors.h ../server/mon-lore.h ../server/z-textblock.h \
 ../server/monster.h ../server/cave.h ../common/list-square-flags.h ../server/list-terrain-flags.h \
 ../server/target.h ../server/mon-predicate.h ../server/mon-timed.h ../common/list-mon-timed.h \
 ../server/mon-blows.h ../common/list-mon-temp-flags.h ../common/list-mon-race-flags.h \
 ../common/list-mon-spells.h ../server/monster.h ../common/obj-tval.h ../server/player.h \
 ../server/player-calcs.h ../server/project.h ../common/source.h ../common/list-projections.h \
 ../server/effects.h ../server/player-attack.h ../client/cmd-core.h ../server/cmds.h \
 ../server/list-effects.h ../server/obj-util.h ../server/object.h
./parse/c-info.o: parse/c-info.c unit-test.h unit-test-types.h ../common/z-util.h \
 ../common/h-basic.h ../server/init.h ../common/z-bitflag.h ../common/z-form.h ../common/z-virt.h \
 ../common/z-file.h ../common/z-rand.h ../common/datafile.h ../server/object.h ../common/z-type.h \
 ../server/z-quark.h ../common/z-dice.h ../common/z-expression.h ../server/obj-properties.h \
 ../common/list-tvals.h ../common/list-object-flags.h ../common/list-kind-flags.h \
 ../common/list-stats.h ../common/list-object-modifiers.h ../common/list-elements.h \
 ../server/list-origins.h ../common/parser.h ../common/list-parser-errors.h \
 ../server/obj-properties.h ../server/object.h ../common/obj-tval.h ../server/obj-util.h ../server/player.h \
 ../common/guid.h ../common/option.h ../common/list-options.h ../common/list-player-flags.h
./parse/e-info.o: parse/e-info.c unit-test.h unit-test-types.h ../common/z-util.h \
 ../common/h-basic.h unit-test-data.h ../common/angband.h ../common/z-bitflag.h ../common/z-form.h \
 ../common/z-virt.h ../common/z-color.h ../common/z-util.h ../common/z-rand.h ../common/config.h \
 ../client/game-event.h ../common/z-type.h ../server/message.h ../common/list-message.h ../server/player.h \
 ../common/guid.h ../server/obj-properties.h ../common/z-file.h ../common/list-tvals.h \
 ../common/list-object-flags.h ../common/list-kind-flags.h ../common/list-stats.h \
 ../common/list-object-modifiers.h ../server/object.h ../server/z-quark.h ../common/z-dice.h \
 ../common/z-expression.h ../common/list-elements.h ../server/list-origins.h ../common/option.h \
 ../common/list-options.h ../common/list-player-flags.h ../server/init.h ../common/datafile.h \
 ../common/parser.h ../common/list-parser-errors.h ../server/mon-lore.h ../server/z-textblock.h \
 ../server/monster.h ../server/cave.h ../common/list-square-flags.h ../server/list-terrain-flags.h \
 ../server/target.h ../server/mon-predicate.h ../server/mon-timed.h ../common/list-mon-timed.h \
 ../server/mon-blows.h ../common/list-mon-temp-flags.h ../common/list-mon-race-flags.h \
 ../common/list-mon-spells.h ../server/monster.h ../common/obj-tval.h ../server/player.h \
 ../server/player-calcs.h ../server/project.h ../common/source.h ../common/list-projections.h \
 ../server/object.h
./parse/f-info.o: parse/f-info.c unit-test.h unit-test-types.h ../common/z-util.h \
 ../common/h-basic.h unit-test-data.h ../common/angband.h ../common/z-bitflag.h ../common/z-form.h \
 ../common/z-virt.h ../common/z-color.h ../common/z-util.h ../common/z-rand.h ../common/config.h \
 ../client/game-event.h ../common/z-type.h ../server/message.h ../common/list-message.h ../server/player.h \
 ../common/guid.h ../server/obj-properties.h ../common/z-file.h ../common/list-tvals.h \
 ../common/list-object-flags.h ../common/list-kind-flags.h ../common/list-stats.h \
 ../common/list-object-modifiers.h ../server/object.h ../server/z-quark.h ../common/z-dice.h \
 ../common/z-expression.h ../common/list-elements.h ../server/list-origins.h ../common/option.h \
 ../common/list-options.h ../common/list-player-flags.h ../server/init.h ../common/datafile.h \
 ../common/parser.h ../common/list-parser-errors.h ../server/mon-lore.h ../server/z-textblock.h \
 ../server/monster.h ../server/cave.h ../common/list-square-flags.h ../server/list-terrain-flags.h \
 ../server/target.h ../server/mon-predicate.h ../server/mon-timed.h ../common/list-mon-timed.h \
 ../server/mon-blows.h ../common/list-mon-temp-flags.h ../common/list-mon-race-flags.h \
 ../common/list-mon-spells.h ../server/monster.h ../common/obj-tval.h ../server/player.h \
 ../server/player-calcs.h ../server/project.h ../common/source.h ../common/list-projections.h
./parse/flavor.o: parse/flavor.c unit-test.h unit-test-types.h ../common/z-util.h \
 ../common/h-basic.h ../server/init.h ../common/z-bitflag.h ../common/z-form.h ../common/z-virt.h \
 ../common/z-file.h ../common/z-rand.h ../common/datafile.h ../server/object.h ../common/z-type.h \
 ../server/z-quark.h ../common/z-dice.h ../common/z-expression.h ../server/obj-properties.h \
 ../common/list-tvals.h ../common/list-object-flags.h ../common/list-kind-flags.h \
 ../common/list-stats.h ../common/list-object-modifiers.h ../common/list-elements.h \
 ../server/list-origins.h ../common/parser.h ../common/list-parser-errors.h ../common/obj-tval.h \
 ../server/obj-properties.h ../server/object.h ../common/z-color.h
./parse/graphics.o: parse/graphics.c unit-test.h unit-test-types.h ../common/z-util.h \
 ../common/h-basic.h test-utils.h ../client/game-event.h ../common/z-type.h ../server/init.h \
 ../common/z-bitflag.h ../common/z-form.h ../common/z-virt.h ../common/z-file.h ../common/z-rand.h \
 ../common/datafile.h ../server/object.h ../server/z-quark.h ../common/z-dice.h ../common/z-expression.h \
 ../server/obj-properties.h ../common/list-tvals.h ../common/list-object-flags.h \
 ../common/list-kind-flags.h ../common/list-stats.h ../common/list-object-modifiers.h \
 ../common/list-elements.h ../server/list-origins.h ../common/parser.h ../common/list-parser-errors.h \
 ../server/message.h ../common/list-message.h ../client/grafmode.h ../server/player.h ../common/guid.h \
 ../common/option.h ../common/list-options.h ../common/list-player-flags.h ../server/player-birth.h \
 ../client/cmd-core.h ../client/ui-prefs.h ../server/cave.h ../common/list-square-flags.h \
 ../server/list-terrain-flags.h ../client/ui-keymap.h ../client/ui-term.h ../client/ui-event.h \
 ../server/mon-util.h ../server/monster.h ../server/target.h ../server/mon-predicate.h ../server/mon-timed.h \
 ../common/list-mon-timed.h ../server/mon-blows.h ../server/player.h ../common/list-mon-temp-flags.h \
 ../common/list-mon-race-flags.h ../common/list-mon-spells.h ../server/mon-msg.h \
 ../common/list-mon-server/message.h ../common/obj-tval.h
./parse/h-info.o: parse/h-info.c unit-test.h unit-test-types.h ../common/z-util.h \
 ../common/h-basic.h ../server/init.h ../common/z-bitflag.h ../common/z-form.h ../common/z-virt.h \
 ../common/z-file.h ../common/z-rand.h ../common/datafile.h ../server/object.h ../common/z-type.h \
 ../server/z-quark.h ../common/z-dice.h ../common/z-expression.h ../server/obj-properties.h \
 ../common/list-tvals.h ../common/list-object-flags.h ../common/list-kind-flags.h \
 ../common/list-stats.h ../common/list-object-modifiers.h ../common/list-elements.h \
 ../server/list-origins.h ../common/parser.h ../common/list-parser-errors.h ../server/player.h \
 ../common/guid.h ../common/option.h ../common/list-options.h ../common/list-player-flags.h
./parse/k-info.o: parse/k-info.c unit-test.h unit-test-types.h ../common/z-util.h \
 ../common/h-basic.h unit-test-data.h ../common/angband.h ../common/z-bitflag.h ../common/z-form.h \
 ../common/z-virt.h ../common/z-color.h ../common/z-util.h ../common/z-rand.h ../common/config.h \
 ../client/game-event.h ../common/z-type.h ../server/message.h ../common/list-message.h ../server/player.h \
 ../common/guid.h ../server/obj-properties.h ../common/z-file.h ../common/list-tvals.h \
 ../common/list-object-flags.h ../common/list-kind-flags.h ../common/list-stats.h \
 ../common/list-object-modifiers.h ../server/object.h ../server/z-quark.h ../common/z-dice.h \
 ../common/z-expression.h ../common/list-elements.h ../server/list-origins.h ../common/option.h \
 ../common/list-options.h ../common/list-player-flags.h ../server/init.h ../common/datafile.h \
 ../common/parser.h ../common/list-parser-errors.h ../server/mon-lore.h ../server/z-textblock.h \
 ../server/monster.h ../server/cave.h ../common/list-square-flags.h ../server/list-terrain-flags.h \
 ../server/target.h ../server/mon-predicate.h ../server/mon-timed.h ../common/list-mon-timed.h \
 ../server/mon-blows.h ../common/list-mon-temp-flags.h ../common/list-mon-race-flags.h \
 ../common/list-mon-spells.h ../server/monster.h ../common/obj-tval.h ../server/player.h \
 ../server/player-calcs.h ../server/project.h ../common/source.h ../common/list-projections.h
./parse/lore.o: parse/lore.c unit-test.h unit-test-types.h ../common/z-util.h \
 ../common/h-basic.h test-utils.h ../server/init.h ../common/z-bitflag.h ../common/z-form.h \
 ../common/z-virt.h ../common/z-file.h ../common/z-rand.h ../common/datafile.h ../server/object.h \
 ../common/z-type.h ../server/z-quark.h ../common/z-dice.h ../common/z-expression.h \
 ../server/obj-properties.h ../common/list-tvals.h ../common/list-object-flags.h \
 ../common/list-kind-flags.h ../common/list-stats.h ../common/list-object-modifiers.h \
 ../common/list-elements.h ../server/list-origins.h ../common/parser.h ../common/list-parser-errors.h \
 ../mon-server/init.h
./parse/names.o: parse/names.c unit-test.h unit-test-types.h ../common/z-util.h \
 ../common/h-basic.h ../server/init.h ../common/z-bitflag.h ../common/z-form.h ../common/z-virt.h \
 ../common/z-file.h ../common/z-rand.h ../common/datafile.h ../server/object.h ../common/z-type.h \
 ../server/z-quark.h ../common/z-dice.h ../common/z-expression.h ../server/obj-properties.h \
 ../common/list-tvals.h ../common/list-object-flags.h ../common/list-kind-flags.h \
 ../common/list-stats.h ../common/list-object-modifiers.h ../common/list-elements.h \
 ../server/list-origins.h ../common/parser.h ../common/list-parser-errors.h ../common/randname.h
./parse/p-info.o: parse/p-info.c unit-test.h unit-test-types.h ../common/z-util.h \
 ../common/h-basic.h ../server/init.h ../common/z-bitflag.h ../common/z-form.h ../common/z-virt.h \
 ../common/z-file.h ../common/z-rand.h ../common/datafile.h ../server/object.h ../common/z-type.h \
 ../server/z-quark.h ../common/z-dice.h ../common/z-expression.h ../server/obj-properties.h \
 ../common/list-tvals.h ../common/list-object-flags.h ../common/list-kind-flags.h \
 ../common/list-stats.h ../common/list-object-modifiers.h ../common/list-elements.h \
 ../server/list-origins.h ../common/parser.h ../common/list-parser-errors.h ../server/player.h \
 ../common/guid.h ../common/option.h ../common/list-options.h ../common/list-player-flags.h
./parse/parse.o: parse/parse.c unit-test.h unit-test-types.h ../common/z-util.h \
 ../common/h-basic.h ../common/parser.h ../common/z-bitflag.h ../common/z-form.h ../common/z-virt.h \
 ../common/z-dice.h ../common/z-rand.h ../common/z-expression.h ../common/list-parser-errors.h
./parse/r-info.o: parse/r-info.c unit-test.h unit-test-types.h ../common/z-util.h \
 ../common/h-basic.h unit-test-data.h ../common/angband.h ../common/z-bitflag.h ../common/z-form.h \
 ../common/z-virt.h ../common/z-color.h ../common/z-util.h ../common/z-rand.h ../common/config.h \
 ../client/game-event.h ../common/z-type.h ../server/message.h ../common/list-message.h ../server/player.h \
 ../common/guid.h ../server/obj-properties.h ../common/z-file.h ../common/list-tvals.h \
 ../common/list-object-flags.h ../common/list-kind-flags.h ../common/list-stats.h \
 ../common/list-object-modifiers.h ../server/object.h ../server/z-quark.h ../common/z-dice.h \
 ../common/z-expression.h ../common/list-elements.h ../server/list-origins.h ../common/option.h \
 ../common/list-options.h ../common/list-player-flags.h ../server/init.h ../common/datafile.h \
 ../common/parser.h ../common/list-parser-errors.h ../server/mon-lore.h ../server/z-textblock.h \
 ../server/monster.h ../server/cave.h ../common/list-square-flags.h ../server/list-terrain-flags.h \
 ../server/target.h ../server/mon-predicate.h ../server/mon-timed.h ../common/list-mon-timed.h \
 ../server/mon-blows.h ../common/list-mon-temp-flags.h ../common/list-mon-race-flags.h \
 ../common/list-mon-spells.h ../server/monster.h ../common/obj-tval.h ../server/player.h \
 ../server/player-calcs.h ../server/project.h ../common/source.h ../common/list-projections.h
./parse/readstore.o: parse/readstore.c unit-test.h unit-test-types.h ../common/z-util.h \
 ../common/h-basic.h ../server/object.h ../common/z-type.h ../server/z-quark.h ../common/z-bitflag.h \
 ../common/z-form.h ../common/z-virt.h ../common/z-dice.h ../common/z-rand.h ../common/z-expression.h \
 ../server/obj-properties.h ../common/z-file.h ../common/list-tvals.h ../common/list-object-flags.h \
 ../common/list-kind-flags.h ../common/list-stats.h ../common/list-object-modifiers.h \
 ../common/list-elements.h ../server/list-origins.h ../server/store.h ../server/cave.h \
 ../common/list-square-flags.h ../server/list-terrain-flags.h ../client/cmd-core.h ../server/object.h \
 ../common/datafile.h ../common/parser.h ../common/list-parser-errors.h ../server/init.h
./parse/v-info.o: parse/v-info.c unit-test.h unit-test-types.h ../common/z-util.h \
 ../common/h-basic.h ../server/init.h ../common/z-bitflag.h ../common/z-form.h ../common/z-virt.h \
 ../common/z-file.h ../common/z-rand.h ../common/datafile.h ../server/object.h ../common/z-type.h \
 ../server/z-quark.h ../common/z-dice.h ../common/z-expression.h ../server/obj-properties.h \
 ../common/list-tvals.h ../common/list-object-flags.h ../common/list-kind-flags.h \
 ../common/list-stats.h ../common/list-object-modifiers.h ../common/list-elements.h \
 ../server/list-origins.h ../common/parser.h ../common/list-parser-errors.h ../server/cave.h \
 ../common/list-square-flags.h ../server/list-terrain-flags.h ../server/generate.h \
 ../server/monster.h ../server/cave.h ../server/target.h ../server/mon-predicate.h ../server/mon-timed.h \
 ../common/list-mon-timed.h ../server/mon-blows.h ../server/player.h ../common/guid.h ../common/option.h \
 ../common/list-options.h ../common/list-player-flags.h ../common/list-mon-temp-flags.h \
 ../common/list-mon-race-flags.h ../common/list-mon-spells.h ../server/list-room-flags.h
./parse/z-info.o: parse/z-info.c unit-test.h unit-test-types.h ../common/z-util.h \
 ../common/h-basic.h unit-test-data.h ../common/angband.h ../common/z-bitflag.h ../common/z-form.h \
 ../common/z-virt.h ../common/z-color.h ../common/z-util.h ../common/z-rand.h ../common/config.h \
 ../client/game-event.h ../common/z-type.h ../server/message.h ../common/list-message.h ../server/player.h \
 ../common/guid.h ../server/obj-properties.h ../common/z-file.h ../common/list-tvals.h \
 ../common/list-object-flags.h ../common/list-kind-flags.h ../common/list-stats.h \
 ../common/list-object-modifiers.h ../server/object.h ../server/z-quark.h ../common/z-dice.h \
 ../common/z-expression.h ../common/list-elements.h ../server/list-origins.h ../common/option.h \
 ../common/list-options.h ../common/list-player-flags.h ../server/init.h ../common/datafile.h \
 ../common/parser.h ../common/list-parser-errors.h ../server/mon-lore.h ../server/z-textblock.h \
 ../server/monster.h ../server/cave.h ../common/list-square-flags.h ../server/list-terrain-flags.h \
 ../server/target.h ../server/mon-predicate.h ../server/mon-timed.h ../common/list-mon-timed.h \
 ../server/mon-blows.h ../common/list-mon-temp-flags.h ../common/list-mon-race-flags.h \
 ../common/list-mon-spells.h ../server/monster.h ../common/obj-tval.h ../server/player.h \
 ../server/player-calcs.h ../server/project.h ../common/source.h ../common/list-projections.h
./player/birth.o: player/birth.c unit-test.h unit-test-types.h ../common/z-util.h \
 ../common/h-basic.h unit-test-data.h ../common/angband.h ../common/z-bitflag.h ../common/z-form.h \
 ../common/z-virt.h ../common/z-color.h ../common/z-util.h ../common/z-rand.h ../common/config.h \
 ../client/game-event.h ../common/z-type.h ../server/message.h ../common/list-message.h ../server/player.h \
 ../common/guid.h ../server/obj-properties.h ../common/z-file.h ../common/list-tvals.h \
 ../common/list-object-flags.h ../common/list-kind-flags.h ../common/list-stats.h \
 ../common/list-object-modifiers.h ../server/object.h ../server/z-quark.h ../common/z-dice.h \
 ../common/z-expression.h ../common/list-elements.h ../server/list-origins.h ../common/option.h \
 ../common/list-options.h ../common/list-player-flags.h ../server/init.h ../common/datafile.h \
 ../common/parser.h ../common/list-parser-errors.h ../server/mon-lore.h ../server/z-textblock.h \
 ../server/monster.h ../server/cave.h ../common/list-square-flags.h ../server/list-terrain-flags.h \
 ../server/target.h ../server/mon-predicate.h ../server/mon-timed.h ../common/list-mon-timed.h \
 ../server/mon-blows.h ../common/list-mon-temp-flags.h ../common/list-mon-race-flags.h \
 ../common/list-mon-spells.h ../server/monster.h ../common/obj-tval.h ../server/player.h \
 ../server/player-calcs.h ../server/project.h ../common/source.h ../common/list-projections.h \
 ../server/player-birth.h ../client/cmd-core.h ../server/player-quest.h
./player/calc-inventory.o: player/calc-inventory.c unit-test.h unit-test-types.h \
 ../common/z-util.h ../common/h-basic.h test-utils.h ../server/cave.h ../common/z-type.h \
 ../common/z-bitflag.h ../common/z-form.h ../common/z-virt.h ../common/list-square-flags.h \
 ../server/list-terrain-flags.h ../server/game-world.h ../server/cave.h ../server/generate.h \
 ../server/monster.h ../common/z-rand.h ../server/target.h ../server/mon-predicate.h ../server/mon-timed.h \
 ../common/list-mon-timed.h ../server/mon-blows.h ../server/player.h ../common/guid.h \
 ../server/obj-properties.h ../common/z-file.h ../common/list-tvals.h ../common/list-object-flags.h \
 ../common/list-kind-flags.h ../common/list-stats.h ../common/list-object-modifiers.h \
 ../server/object.h ../server/z-quark.h ../common/z-dice.h ../common/z-expression.h \
 ../common/list-elements.h ../server/list-origins.h ../common/option.h ../common/list-options.h \
 ../common/list-player-flags.h ../common/list-mon-temp-flags.h ../common/list-mon-race-flags.h \
 ../common/list-mon-spells.h ../server/list-room-flags.h ../server/init.h ../common/datafile.h \
 ../common/parser.h ../common/list-parser-errors.h ../server/mon-make.h ../server/obj-gear.h \
 ../common/list-equip-slots.h ../server/obj-knowledge.h ../common/angband.h ../common/z-color.h \
 ../common/z-util.h ../common/config.h ../client/game-event.h ../server/message.h ../common/list-message.h \
 ../server/obj-make.h ../server/obj-pile.h ../server/obj-properties.h ../common/obj-tval.h \
 ../server/obj-util.h ../server/player-birth.h ../client/cmd-core.h ../server/player-calcs.h \
 ../server/z-quark.h
./player/combine-pack.o: player/combine-pack.c unit-test.h unit-test-types.h \
 ../common/z-util.h ../common/h-basic.h test-utils.h ../server/cave.h ../common/z-type.h \
 ../common/z-bitflag.h ../common/z-form.h ../common/z-virt.h ../common/list-square-flags.h \
 ../server/list-terrain-flags.h ../server/game-world.h ../server/cave.h ../server/generate.h \
 ../server/monster.h ../common/z-rand.h ../server/target.h ../server/mon-predicate.h ../server/mon-timed.h \
 ../common/list-mon-timed.h ../server/mon-blows.h ../server/player.h ../common/guid.h \
 ../server/obj-properties.h ../common/z-file.h ../common/list-tvals.h ../common/list-object-flags.h \
 ../common/list-kind-flags.h ../common/list-stats.h ../common/list-object-modifiers.h \
 ../server/object.h ../server/z-quark.h ../common/z-dice.h ../common/z-expression.h \
 ../common/list-elements.h ../server/list-origins.h ../common/option.h ../common/list-options.h \
 ../common/list-player-flags.h ../common/list-mon-temp-flags.h ../common/list-mon-race-flags.h \
 ../common/list-mon-spells.h ../server/list-room-flags.h ../server/init.h ../common/datafile.h \
 ../common/parser.h ../common/list-parser-errors.h ../server/mon-make.h ../server/obj-gear.h \
 ../common/list-equip-slots.h ../server/obj-knowledge.h ../common/angband.h ../common/z-color.h \
 ../common/z-util.h ../common/config.h ../client/game-event.h ../server/message.h ../common/list-message.h \
 ../server/obj-make.h ../server/obj-pile.h ../common/obj-tval.h ../server/obj-util.h \
 ../server/player-birth.h ../client/cmd-core.h ../server/player-calcs.h ../server/z-quark.h
./player/history.o: player/history.c ../server/player.h ../common/guid.h ../server/obj-properties.h \
 ../common/z-file.h ../common/h-basic.h ../common/z-bitflag.h ../common/z-form.h ../common/z-virt.h \
 ../common/list-tvals.h ../common/list-object-flags.h ../common/list-kind-flags.h \
 ../common/list-stats.h ../common/list-object-modifiers.h ../server/object.h ../common/z-type.h \
 ../server/z-quark.h ../common/z-dice.h ../common/z-rand.h ../common/z-expression.h \
 ../common/list-elements.h ../server/list-origins.h ../common/option.h ../common/list-options.h \
 ../common/list-player-flags.h ../server/player-birth.h ../client/cmd-core.h unit-test.h \
 unit-test-types.h ../common/z-util.h
./player/inven-carry-num.o: player/inven-carry-num.c test-utils.h unit-test.h \
 unit-test-types.h ../common/z-util.h ../common/h-basic.h ../server/init.h ../common/z-bitflag.h \
 ../common/z-form.h ../common/z-virt.h ../common/z-file.h ../common/z-rand.h ../common/datafile.h \
 ../server/object.h ../common/z-type.h ../server/z-quark.h ../common/z-dice.h ../common/z-expression.h \
 ../server/obj-properties.h ../common/list-tvals.h ../common/list-object-flags.h \
 ../common/list-kind-flags.h ../common/list-stats.h ../common/list-object-modifiers.h \
 ../common/list-elements.h ../server/list-origins.h ../common/parser.h ../common/list-parser-errors.h \
 ../server/obj-gear.h ../server/player.h ../common/guid.h ../common/option.h ../common/list-options.h \
 ../common/list-player-flags.h ../common/list-equip-slots.h ../server/obj-knowledge.h \
 ../common/angband.h ../common/z-color.h ../common/z-util.h ../common/config.h ../client/game-event.h \
 ../server/message.h ../common/list-message.h ../server/obj-make.h ../server/cave.h \
 ../common/list-square-flags.h ../server/list-terrain-flags.h ../server/obj-pile.h \
 ../server/obj-util.h ../server/player-birth.h ../client/cmd-core.h ../server/player-calcs.h
./player/inven-wield.o: player/inven-wield.c unit-test.h unit-test-types.h \
 ../common/z-util.h ../common/h-basic.h test-utils.h ../server/cave.h ../common/z-type.h \
 ../common/z-bitflag.h ../common/z-form.h ../common/z-virt.h ../common/list-square-flags.h \
 ../server/list-terrain-flags.h ../server/effects.h ../common/source.h ../server/object.h \
 ../server/z-quark.h ../common/z-dice.h ../common/z-rand.h ../common/z-expression.h \
 ../server/obj-properties.h ../common/z-file.h ../common/list-tvals.h ../common/list-object-flags.h \
 ../common/list-kind-flags.h ../common/list-stats.h ../common/list-object-modifiers.h \
 ../common/list-elements.h ../server/list-origins.h ../server/player-attack.h ../client/cmd-core.h \
 ../server/cmds.h ../server/cave.h ../server/list-effects.h ../server/game-world.h ../server/generate.h \
 ../server/monster.h ../server/target.h ../server/mon-predicate.h ../server/mon-timed.h \
 ../common/list-mon-timed.h ../server/mon-blows.h ../server/player.h ../common/guid.h ../common/option.h \
 ../common/list-options.h ../common/list-player-flags.h ../common/list-mon-temp-flags.h \
 ../common/list-mon-race-flags.h ../common/list-mon-spells.h ../server/list-room-flags.h \
 ../server/init.h ../common/datafile.h ../common/parser.h ../common/list-parser-errors.h \
 ../server/mon-make.h ../server/obj-curse.h ../server/obj-gear.h ../common/list-equip-slots.h \
 ../server/obj-knowledge.h ../common/angband.h ../common/z-color.h ../common/z-util.h ../common/config.h \
 ../client/game-event.h ../server/message.h ../common/list-message.h ../server/obj-make.h \
 ../server/obj-pile.h ../common/obj-tval.h ../server/obj-util.h ../server/player-birth.h ../server/z-quark.h
./player/pathfind.o: player/pathfind.c unit-test.h unit-test-types.h ../common/z-util.h \
 ../common/h-basic.h ../server/cave.h ../common/z-type.h ../common/z-bitflag.h ../common/z-form.h \
 ../common/z-virt.h ../common/list-square-flags.h ../server/list-terrain-flags.h \
 ../server/player-path.h
./player/playerstat.o: player/playerstat.c unit-test.h unit-test-types.h \
 ../common/z-util.h ../common/h-basic.h unit-test-data.h ../common/angband.h ../common/z-bitflag.h \
 ../common/z-form.h ../common/z-virt.h ../common/z-color.h ../common/z-util.h ../common/z-rand.h ../common/config.h \
 ../client/game-event.h ../common/z-type.h ../server/message.h ../common/list-message.h ../server/player.h \
 ../common/guid.h ../server/obj-properties.h ../common/z-file.h ../common/list-tvals.h \
 ../common/list-object-flags.h ../common/list-kind-flags.h ../common/list-stats.h \
 ../common/list-object-modifiers.h ../server/object.h ../server/z-quark.h ../common/z-dice.h \
 ../common/z-expression.h ../common/list-elements.h ../server/list-origins.h ../common/option.h \
 ../common/list-options.h ../common/list-player-flags.h ../server/init.h ../common/datafile.h \
 ../common/parser.h ../common/list-parser-errors.h ../server/mon-lore.h ../server/z-textblock.h \
 ../server/monster.h ../server/cave.h ../common/list-square-flags.h ../server/list-terrain-flags.h \
 ../server/target.h ../server/mon-predicate.h ../server/mon-timed.h ../common/list-mon-timed.h \
 ../server/mon-blows.h ../common/list-mon-temp-flags.h ../common/list-mon-race-flags.h \
 ../common/list-mon-spells.h ../server/monster.h ../common/obj-tval.h ../server/player.h \
 ../server/player-calcs.h ../server/project.h ../common/source.h ../common/list-projections.h \
 ../server/player-birth.h ../client/cmd-core.h
./trivial/trivial.o: trivial/trivial.c unit-test.h unit-test-types.h ../common/z-util.h \
 ../common/h-basic.h
./z-dice/dice.o: z-dice/dice.c unit-test.h unit-test-types.h ../common/z-util.h \
 ../common/h-basic.h ../common/z-dice.h ../common/z-rand.h ../common/z-expression.h
./z-expression/expression.o: z-expression/expression.c unit-test.h unit-test-types.h \
 ../common/z-util.h ../common/h-basic.h ../common/z-expression.h
./z-quark/quark.o: z-quark/quark.c unit-test.h unit-test-types.h ../common/z-util.h \
 ../common/h-basic.h ../server/z-quark.h
./z-textblock/textblock.o: z-textblock/textblock.c unit-test.h unit-test-types.h \
 ../common/z-util.h ../common/h-basic.h ../common/z-color.h ../server/z-textblock.h ../common/z-file.h
./z-util/util.o: z-util/util.c unit-test.h unit-test-types.h ../common/z-util.h \
 ../common/h-basic.h ../server/z-quark.h
./z-virt/mem.o: z-virt/mem.c unit-test.h unit-test-types.h ../common/z-util.h \
 ../common/h-basic.h ../common/z-virt.h
./z-virt/string.o: z-virt/string.c unit-test.h unit-test-types.h ../common/z-util.h \
 ../common/h-basic.h ../common/z-virt.h
./test-utils.o: test-utils.c ../common/h-basic.h ../common/config.h ../server/init.h \
 ../common/h-basic.h ../common/z-bitflag.h ../common/z-form.h ../common/z-virt.h ../common/z-file.h \
 ../common/z-rand.h ../common/datafile.h ../server/object.h ../common/z-type.h ../server/z-quark.h \
 ../common/z-dice.h ../common/z-expression.h ../server/obj-properties.h ../common/list-tvals.h \
 ../common/list-object-flags.h ../common/list-kind-flags.h ../common/list-stats.h \
 ../common/list-object-modifiers.h ../common/list-elements.h ../server/list-origins.h \
 ../common/parser.h ../common/list-parser-errors.h test-utils.h ../common/z-util.h
./unit-test.o: unit-test.c unit-test.h unit-test-types.h ../common/z-util.h \
 ../common/h-basic.h
