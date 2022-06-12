/**
 * File: angband.h
 * Purpose: Main Angband header file
 *
 * Copyright (c) 1997 Ben Harrison, James E. Wilson, Robert A. Koeneke
 *
 * This software may be copied and distributed for educational, research,
 * and not for profit purposes provided that this copyright and statement
 * are included in all such copies.  Other copyrights may also apply.
 */

#ifndef INCLUDED_ANGBAND_H
#define INCLUDED_ANGBAND_H

/*
 * Include the low-level includes
 */
#include "h-basic.h"

#ifndef SPClient
#include "h-quark.h"
#include "option.h"
#endif
/*
 * Include the mid-level includes
 */
#include "z-bitflag.h"
#include "z-color.h"

#ifndef SPClient
#include "z-file.h"
#endif

#include "z-form.h"

#ifndef SPClient
#include "z-msg.h"
#include "z-rand.h"
#include "z-set.h"
#include "z-spells.h"
#include "z-type.h"
#endif

#include "z-util.h"
#include "z-virt.h"

#ifndef SPClient
#include "z-expression.h"
#include "z-dice.h"
#endif

/*
 * Include the high-level includes
 */
#ifndef SPClient
#include "z-defines.h"

#include "buildid.h"
#endif

#include "config.h"

#ifndef SPClient
#include "datafile.h"
#include "guid.h"
#include "md5.h"
#include "source.h"
#include "parser.h"
#include "obj-common.h"
#include "trap-common.h"
#include "player-state.h"
#include "mon-common.h"
#include "player-common.h"
#include "display.h"
#include "obj-gear-common.h"
#include "obj-tval.h"
#include "player-common-calcs.h"
#include "randname.h"
#include "store-types.h"
#include "util.h"

#else

#include "message.h"
#include "player.h"

#endif

/***** Some older copyright messages follow below *****/

/*
 * Note that these copyright messages apply to an ancient version
 * of Angband, as in, from pre-2.4.frog-knows days, and thus the
 * references to version numbers may be rather misleading...
 */

/*
 * UNIX ANGBAND Version 5.0
 */

/* Original copyright message follows. */

/*
 * ANGBAND Version 4.8	COPYRIGHT (c) Robert Alan Koeneke
 *
 *	 I lovingly dedicate this game to hackers and adventurers
 *	 everywhere...
 *
 *	 Designer and Programmer:
 *		Robert Alan Koeneke
 *		University of Oklahoma
 *
 *	 Assistant Programmer:
 *		Jimmey Wayne Todd
 *		University of Oklahoma
 *
 *	 Assistant Programmer:
 *		Gary D. McAdoo
 *		University of Oklahoma
 *
 *	 UNIX Port:
 *		James E. Wilson
 *		UC Berkeley
 *		wilson@ernie.Berkeley.EDU
 *		ucbvax!ucbernie!wilson
 */

/*
 *	 ANGBAND may be copied and modified freely as long as the above
 *	 credits are retained.	No one who-so-ever may sell or market
 *	 this software in any form without the expressed written consent
 *	 of the author Robert Alan Koeneke.
 */
 
/* Horrible hack -- should we link lib math instead? */
#ifndef min
# define min(A, B) (A < B ? A : B)
#endif
#ifndef max
# define max(A, B) (A > B ? A : B)
#endif

#endif
