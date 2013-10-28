/*
 * File: obj-power.c
 * Purpose: calculation of object power
 *
 * Copyright (c) 2001 Chris Carr, Chris Robertson
 * Revised in 2009 by Chris Carr, Peter Denison
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
#include "object/tvalsval.h"
#include "init.h"
#include "effects.h"

/*
 * Constants for the power algorithm:
 * - ammo damage for calculating launcher power
 * (the current values assume normal (non-seeker) ammo enchanted to +9)
 * - launcher multipliers for calculating ammo power
 * (these are halved in the algorithm)
 * - fudge factor for extra damage from rings etc. (used if extra blows)
 * - assumed damage for ring brands
 * - base power for light sources (additional power for NFUEL is added later)
 * - base power for jewelry
 * - base power for armour items (for halving acid damage)
 * - power per point of damage
 * - power per point of +to_hit
 * - power per point of base AC
 * - power per point of +to_ac
 * (these four are all halved in the algorithm)
 * - assumed max blows
 * - fudge factor for rescaling missile power 
 * (shots are currently set to be 1.25x as powerful as blows)
 * - inhibiting values for +blows/might/shots/immunities (max is one less)
 * - power per unit pval for each pval ability (except speed)
 * (there is an extra term for multiple pval bonuses)
 * - additional power for full rbase set (on top of arithmetic progression)
 * - additional power for full sustain set (ditto, excludes susCHR)
 */
#define AVG_SLING_AMMO_DAMAGE  10
#define AVG_BOW_AMMO_DAMAGE    11
#define AVG_XBOW_AMMO_DAMAGE   12
#define AVG_SLING_MULT          4 /* i.e. 2 */
#define AVG_BOW_MULT            5 /* i.e. 2.5 */
#define AVG_XBOW_MULT           7 /* i.e. 3.5 */
#define AVG_LAUNCHER_DMG	9
#define MELEE_DAMAGE_BOOST     10
#define RING_BRAND_DMG	       30 /* fudge to boost off-weapon brand power */
#define BASE_LIGHT_POWER         6
#define BASE_JEWELRY_POWER	4
#define BASE_ARMOUR_POWER	2
#define DAMAGE_POWER            4 /* i.e. 2 */
#define TO_HIT_POWER            2 /* i.e. 1 */
#define BASE_AC_POWER           3 /* i.e. 1.5 */
#define TO_AC_POWER             2 /* i.e. 1 */
#define MAX_BLOWS               5
#define BOW_RESCALER            4
#define INHIBIT_BLOWS           4
#define INHIBIT_MIGHT           4
#define INHIBIT_SHOTS           4
#define IMMUNITY_POWER         25 /* for each immunity after the first */
#define INHIBIT_IMMUNITIES      4
#define STR_POWER	        9
#define INT_POWER	        5
#define WIS_POWER	        5
#define DEX_POWER		6
#define CON_POWER	       12
#define CHR_POWER		2
#define STEALTH_POWER		8
#define SEARCH_POWER		2
#define INFRA_POWER		4
#define TUNN_POWER		2
#define RBASE_POWER		5 
#define SUST_POWER		5

/*
 * Table giving speed power ratings
 * We go up to +20 here, but in practice it will never get there
 */
static s16b speed_power[21] =
	{0, 10, 21, 33, 46, 60, 75, 91, 108, 126, 145,
	163, 180, 196, 211, 225, 238, 250, 261, 271, 280};

/*
 * Boost ratings for combinations of ability bonuses
 * We go up to +24 here - anything higher is inhibited
 * N.B. Not all stats count equally towards this total
 */
static s16b ability_power[25] =
	{0, 0, 0, 0, 0, 0, 0, 2, 4, 6, 8,
	12, 16, 20, 24, 30, 36, 42, 48, 56, 64,
	74, 84, 96, 110};

/*
 * Calculate the rating for a given slay combination
 */
static s32b slay_power(const object_type *o_ptr, int verbose, ang_file* log_file,
	u32b f[OBJ_FLAG_N])
{
	u32b s_index = 0;
	s32b sv = 0;
	int i;
	int mult;
	const slay_t *s_ptr;

	/* Combine the slay bytes into an index value */
	s_index = f[0] & TR0_ALL_SLAYS;

	/* Look in the cache to see if we know this one yet */
	for (i = 0; slay_cache[i].flags; i++)
	{
		if (slay_cache[i].flags == s_index) sv = slay_cache[i].value;
	}

	/* we know the value of 0 (no slays) is cached at the end of the array */
	if (s_index == 0) sv = slay_cache[N_ELEMENTS(slay_cache)].value;

	/* If it's cached, return its value */
	if (sv) 
	{
		LOG_PRINT("Slay cache hit\n");
		return sv;
	}
	
	/*
	 * Otherwise we need to calculate the expected average multiplier
	 * for this combination (multiplied by the total number of
	 * monsters, which we'll divide out later).
	 */
	for (i = 0; i < z_info->r_max; i++)
	{
		monster_race *r_ptr = &r_info[i];
		mult = 1;

		/*
		 * Do the following in ascending order so that the best
		 * multiple is retained
		 */
		for (s_ptr = slay_table; s_ptr->slay_flag; s_ptr++)
		{
			if ((f[0] & s_ptr->slay_flag) &&
				((r_ptr->flags[2] & s_ptr->monster_flag) || 
				(s_ptr->brand && !(r_ptr->flags[2] & 
				s_ptr->resist_flag))))
			{
			    mult = s_ptr->mult;
			}
		}

		/* Add the multiple to sv */
		sv += mult * r_ptr->power;
	}

	/*
	 * To get the expected damage for this weapon, multiply the
	 * average damage from base dice by sv, and divide by the
	 * total number of monsters.
	 */
	if (verbose)
	{
		/* Write info about the slay combination and multiplier */
		file_putf(log_file,"Slay multiplier for:");

		if (f[0] & TR0_SLAY_EVIL) file_putf(log_file,"Evl ");
		if (f[0] & TR0_KILL_DRAGON) file_putf(log_file,"XDr ");
		if (f[0] & TR0_KILL_DEMON) file_putf(log_file,"XDm ");
		if (f[0] & TR0_KILL_UNDEAD) file_putf(log_file,"XUn ");
		if (f[0] & TR0_SLAY_ANIMAL) file_putf(log_file,"Ani ");
		if (f[0] & TR0_SLAY_UNDEAD) file_putf(log_file,"Und ");
		if (f[0] & TR0_SLAY_DRAGON) file_putf(log_file,"Drg ");
		if (f[0] & TR0_SLAY_DEMON) file_putf(log_file,"Dmn ");
		if (f[0] & TR0_SLAY_TROLL) file_putf(log_file,"Tro ");
		if (f[0] & TR0_SLAY_ORC) file_putf(log_file,"Orc ");
		if (f[0] & TR0_SLAY_GIANT) file_putf(log_file,"Gia ");

		if (f[0] & TR0_BRAND_ACID) file_putf(log_file,"Acd ");
		if (f[0] & TR0_BRAND_ELEC) file_putf(log_file,"Elc ");
		if (f[0] & TR0_BRAND_FIRE) file_putf(log_file,"Fir ");
		if (f[0] & TR0_BRAND_COLD) file_putf(log_file,"Cld ");
		if (f[0] & TR0_BRAND_POIS) file_putf(log_file,"Poi ");

		file_putf(log_file,"sv is: %d\n", sv);
		file_putf(log_file," and t_m_p is: %d \n", tot_mon_power);

		file_putf(log_file,"times 1000 is: %d\n", (1000 * sv) / tot_mon_power);
	}

	/* Add to the cache */
     /*	LOG_PRINT1("s_index is %d\n", s_index); */
	for (i = 0; slay_cache[i].flags; i++)
	{
	     /*	LOG_PRINT2("i is %d and flag is %d\n", i, slay_cache[i].flags); */
		if (slay_cache[i].flags == s_index) slay_cache[i].value = sv;
	}
	/* Ensure we cache the value of 0 (no slays) */
	if (s_index == 0) slay_cache[N_ELEMENTS(slay_cache)].value = sv;

	LOG_PRINT("Added to slay cache\n");

	return sv;
}

/*
 * Calculate the multiplier we'll get with a given bow type.
 * Note that this relies on the multiplier being the 2nd digit of the bow's 
 * sval. We assume that sval has already been checked for legitimacy before 
 * we get here.
 */
static int bow_multiplier(int sval)
{
	int mult = 0;
	mult = sval - 10 * (sval / 10);
	return mult;
}


/*
 * Evaluate the object's overall power level.
 */
s32b object_power(const object_type* o_ptr, int verbose, ang_file *log_file,
	bool known)
{
	s32b p = 0;
	object_kind *k_ptr;
	int immunities = 0;
	int misc = 0;
	int lowres = 0;
	int highres = 0;
	int sustains = 0;
	int extra_stat_bonus = 0;
	int i;
	u32b f[OBJ_FLAG_N];

	/* Extract the flags */
	if (known) 
	{
		LOG_PRINT("Object is known\n");
		object_flags(o_ptr, f);
	}		
	else 
	{
		LOG_PRINT("Object is not fully known\n");
		object_flags_known(o_ptr, f);
	}

	if (verbose)
	{
		for (i = 0; i < OBJ_FLAG_N; i++)
		{
			LOG_PRINT2("Object flags[%d] is %x\n", i, f[i]);
		}
	}

	k_ptr = &k_info[o_ptr->k_idx];

	/* Evaluate certain abilities based on type of object. */
	switch (o_ptr->tval)
	{
		case TV_BOW:
		{
			int mult;

			/*
			 * Damage multiplier for bows should be weighted less than that
			 * for melee weapons, since players typically get fewer shots
			 * than hits (note, however, that the multipliers are applied
			 * afterwards in the bow calculation, not before as for melee
			 * weapons, which tends to bring these numbers back into line).
			 * ToDo: rework evaluation of negative pvals
			 */

			p += (o_ptr->to_d * DAMAGE_POWER / 2);
			LOG_PRINT1("Adding power from to_dam, total is %d\n", p);

			/*
			 * Add the average damage of fully enchanted (good) ammo for this
			 * weapon.  Could make this dynamic based on k_info if desired.
			 * ToDo: precisely that.
			 */

			if (o_ptr->sval == SV_SLING)
			{
				p += (AVG_SLING_AMMO_DAMAGE * DAMAGE_POWER / 2);
			}
			else if (o_ptr->sval == SV_SHORT_BOW ||
				o_ptr->sval == SV_LONG_BOW)
			{
				p += (AVG_BOW_AMMO_DAMAGE * DAMAGE_POWER / 2);
			}
			else if (o_ptr->sval == SV_LIGHT_XBOW ||
				o_ptr->sval == SV_HEAVY_XBOW)
			{
				p += (AVG_XBOW_AMMO_DAMAGE * DAMAGE_POWER / 2);
			}

			LOG_PRINT1("Adding power from ammo, total is %d\n", p);

			mult = bow_multiplier(o_ptr->sval);

			LOG_PRINT1("Base multiplier for this weapon is %d\n", mult);

			if ((f[0] & TR0_MIGHT) && (known ||
				object_pval_is_visible(o_ptr)))
			{
				if (o_ptr->pval >= INHIBIT_MIGHT || o_ptr->pval < 0)
				{
					p += INHIBIT_POWER;	/* inhibit */
					mult = 1;	/* don't overflow */
					LOG_PRINT("INHIBITING - too much extra might\n");
				}
				else
				{
					mult += o_ptr->pval;
				}
				LOG_PRINT1("Extra might multiple is %d\n", mult);
			}
			p *= mult;
			LOG_PRINT2("Multiplying power by %d, total is %d\n", mult, p);

			if ((f[0] & TR0_SHOTS) && (known ||
				object_pval_is_visible(o_ptr)))
			{
				LOG_PRINT1("Extra shots: %d\n", o_ptr->pval);

				if (o_ptr->pval >= INHIBIT_SHOTS || o_ptr->pval < 0)
				{
					p += INHIBIT_POWER;	/* inhibit */
					LOG_PRINT("INHIBITING - too many extra shots\n");
				}
				else if (o_ptr->pval > 0)
				{
					p = (p * (1 + o_ptr->pval));
					LOG_PRINT2("Multiplying power by 1 + %d, total is %d\n",
						o_ptr->pval, p);
				}
			}

			/* Apply the correct slay multiplier */
			p = (p * slay_power(o_ptr, verbose, log_file, f)) / tot_mon_power;
			LOG_PRINT1("Adjusted for slay power, total is %d\n", p);

			if (o_ptr->weight < k_ptr->weight)
			{
				p++;
				LOG_PRINT("Incrementing power by one for low weight\n");
			}

			/*
			 * Correction to match ratings to melee damage ratings.
			 * We multiply all missile weapons by 1.5 in order to compare damage.
			 * (CR 11/20/01 - changed this to 1.25).
			 * Melee weapons assume 5 attacks per turn, so we must also divide
			 * by 5 to get equal ratings. 1.25 / 5 = 0.25
			 */
			p = sign(p) * (ABS(p) / BOW_RESCALER);
			LOG_PRINT1("Rescaling bow power, total is %d\n", p);

			p += sign(o_ptr->to_h) * (ABS(o_ptr->to_h) * TO_HIT_POWER / 2);
			LOG_PRINT1("Adding power from to_hit, total is %d\n", p);

			break;
		}
		case TV_SHOT:
		case TV_ARROW:
		case TV_BOLT:
		case TV_DIGGING:
		case TV_HAFTED:
		case TV_POLEARM:
		case TV_SWORD:
		{
			p += (o_ptr->dd * (o_ptr->ds + 1) * DAMAGE_POWER / 4);
			LOG_PRINT1("Adding power for dam dice, total is %d\n", p);

			/* Apply the correct slay multiplier */
			p = (p * slay_power(o_ptr, verbose, log_file, f)) / tot_mon_power;
			LOG_PRINT1("Adjusted for slay power, total is %d\n", p);

			p += (o_ptr->to_d * DAMAGE_POWER / 2);
			LOG_PRINT1("Adding power for to_dam, total is %d\n", p);

			if ((f[0] & TR0_BLOWS) && (known ||
				object_pval_is_visible(o_ptr)))
			{
				LOG_PRINT1("Extra blows: %d\n", o_ptr->pval);
				if (o_ptr->pval >= INHIBIT_BLOWS || o_ptr->pval < 0)
				{
					p += INHIBIT_POWER;	/* inhibit */
					LOG_PRINT("INHIBITING, too many extra blows or a negative number\n");
				}
				else if (o_ptr->pval > 0)
				{
					p = sign(p) * ((ABS(p) * (MAX_BLOWS + o_ptr->pval)) 
						/ MAX_BLOWS);
					/* Add an extra amount per extra blow to account for damage rings */
					p += (MELEE_DAMAGE_BOOST * o_ptr->pval * DAMAGE_POWER / (2 * MAX_BLOWS));
					LOG_PRINT1("Adding power for blows, total is %d\n", p);
				}
			}

			/* add launcher bonus for ego ammo, and multiply */
			if (o_ptr->tval == TV_SHOT)
			{
				if (o_ptr->name2) p += (AVG_LAUNCHER_DMG * DAMAGE_POWER / 2);
				p = p * AVG_SLING_MULT / (2 * BOW_RESCALER);
			}
			if (o_ptr->tval == TV_ARROW)
			{
				if (o_ptr->name2) p += (AVG_LAUNCHER_DMG * DAMAGE_POWER / 2);
				p = p * AVG_BOW_MULT / (2 * BOW_RESCALER);
			}
			if (o_ptr->tval == TV_BOLT)
			{
				if (o_ptr->name2) p += (AVG_LAUNCHER_DMG * DAMAGE_POWER / 2);
				p = p * AVG_XBOW_MULT / (2 * BOW_RESCALER);
			}
			LOG_PRINT1("After multiplying ammo and rescaling, power is %d\n", p);
			
			p += sign(o_ptr->to_h) * (ABS(o_ptr->to_h) * TO_HIT_POWER / 2);
			LOG_PRINT1("Adding power for to hit, total is %d\n", p);

			/* Remember, weight is in 0.1 lb. units. */
			if (o_ptr->weight < k_ptr->weight)
			{
				p += (k_ptr->weight - o_ptr->weight) / 20; 
				LOG_PRINT1("Adding power for low weight, total is %d\n", p);
			}

			break;
		}
		case TV_BOOTS:
		case TV_GLOVES:
		case TV_HELM:
		case TV_CROWN:
		case TV_SHIELD:
		case TV_CLOAK:
		case TV_SOFT_ARMOR:
		case TV_HARD_ARMOR:
		case TV_DRAG_ARMOR:
		{
			p += BASE_ARMOUR_POWER;
			LOG_PRINT1("Armour item, base power is %d\n", p);

			p += sign(o_ptr->ac) * ((ABS(o_ptr->ac) * BASE_AC_POWER) / 2);
			LOG_PRINT1("Adding power for base AC value, total is %d\n", p);

			p += sign(o_ptr->to_h) * (ABS(o_ptr->to_h) * TO_HIT_POWER);
			LOG_PRINT1("Adding power for to_hit, total is %d\n", p);

			p += o_ptr->to_d * DAMAGE_POWER;
			LOG_PRINT1("Adding power for to_dam, total is %d\n", p);

			/* Apply the correct brand/slay multiplier */
			p += (((2 * (o_ptr->to_d + RING_BRAND_DMG)
				* slay_power(o_ptr, verbose, log_file, f))
				/ tot_mon_power) - (2 * (o_ptr->to_d + RING_BRAND_DMG)));
			LOG_PRINT1("Adjusted for brand/slay power, total is %d\n", p);

			/* Add power for extra blows */
			if ((f[0] & TR0_BLOWS) && (known ||
				object_pval_is_visible(o_ptr)))
			{
				LOG_PRINT1("Extra blows: %d\n", o_ptr->pval);
				if (o_ptr->pval >= INHIBIT_BLOWS || o_ptr->pval < 0)
				{
					p += INHIBIT_POWER;	/* inhibit */
					LOG_PRINT("INHIBITING, too many extra blows or a negative number\n");
				}
				else if (o_ptr->pval > 0)
				{
					/* We assume a 3d5 weapon with +10 damage elsewhere - gives ~16 power per extra blow */
					p += ((MELEE_DAMAGE_BOOST + RING_BRAND_DMG + o_ptr->to_d) * o_ptr->pval * DAMAGE_POWER / MAX_BLOWS);
					LOG_PRINT1("Adding power for extra blows, total is %d\n", p);
				}
			}

			/* Add power for extra shots */
			if ((f[0] & TR0_SHOTS) && (known ||
				object_pval_is_visible(o_ptr)))
			{
				LOG_PRINT1("Extra shots: %d\n", o_ptr->pval);
				if (o_ptr->pval >= INHIBIT_SHOTS || o_ptr->pval < 0)
				{
					p += INHIBIT_POWER;	/* inhibit */
					LOG_PRINT("INHIBITING - too many extra shots\n");
				}
				else if (o_ptr->pval > 0)
				{
					/* We assume a x2.5 +9dam launcher with +9 1d4 ammo - gives ~50 power per extra shot */
					p += ((AVG_BOW_AMMO_DAMAGE + AVG_LAUNCHER_DMG) * AVG_BOW_MULT * o_ptr->pval * DAMAGE_POWER / (2 * BOW_RESCALER));
					LOG_PRINT1("Adding power for extra shots - total is %d\n", p);
				}
			}

			if (o_ptr->weight < k_ptr->weight)
			{
				p += (k_ptr->weight - o_ptr->weight) / 10;
				LOG_PRINT1("Adding power for low weight, total is %d\n", p);
			}
			break;
		}
		case TV_LIGHT:
		{
			p += BASE_LIGHT_POWER;
			LOG_PRINT("Light source, adding base power\n");

			p += sign(o_ptr->to_h) * (ABS(o_ptr->to_h) * TO_HIT_POWER);
			LOG_PRINT1("Adding power for to_hit, total is %d\n", p);

			p += o_ptr->to_d * DAMAGE_POWER;
			LOG_PRINT1("Adding power for to_dam, total is %d\n", p);

			/* Apply the correct brand/slay multiplier */
			p += (((2 * (o_ptr->to_d + RING_BRAND_DMG)
				* slay_power(o_ptr, verbose, log_file, f))
				/ tot_mon_power) - (2 * (o_ptr->to_d + RING_BRAND_DMG)));
			LOG_PRINT1("Adjusted for brand/slay power, total is %d\n", p);

			/* Add power for extra blows */
			if ((f[0] & TR0_BLOWS) && (known ||
				object_pval_is_visible(o_ptr)))
			{
				LOG_PRINT1("Extra blows: %d\n", o_ptr->pval);
				if (o_ptr->pval >= INHIBIT_BLOWS || o_ptr->pval < 0)
				{
					p += INHIBIT_POWER;	/* inhibit */
					LOG_PRINT("INHIBITING, too many extra blows or a negative number\n");
				}
				else if (o_ptr->pval > 0)
				{
					/* We assume a 3d5 weapon with +10 damage elsewhere - gives ~16 power per extra blow */
					p += ((MELEE_DAMAGE_BOOST + RING_BRAND_DMG + o_ptr->to_d) * o_ptr->pval * DAMAGE_POWER / MAX_BLOWS);
					LOG_PRINT1("Adding power for extra blows, total is %d\n", p);
				}
			}

			/* Add power for extra shots */
			if ((f[0] & TR0_SHOTS) && (known ||
				object_pval_is_visible(o_ptr)))
			{
				LOG_PRINT1("Extra shots: %d\n", o_ptr->pval);
				if (o_ptr->pval >= INHIBIT_SHOTS || o_ptr->pval < 0)
				{
					p += INHIBIT_POWER;	/* inhibit */
					LOG_PRINT("INHIBITING - too many extra shots\n");
				}
				else if (o_ptr->pval > 0)
				{
					/* We assume a x2.5 +9dam launcher with +9 1d4 ammo - gives ~25 power per extra shot */
					p += ((AVG_BOW_AMMO_DAMAGE + AVG_LAUNCHER_DMG) * AVG_BOW_MULT * o_ptr->pval * DAMAGE_POWER / (2 * BOW_RESCALER));
					LOG_PRINT1("Adding power for extra shots - total is %d\n", p);
				}
			}

			/* 
			 * Big boost for extra light radius 
			 * n.b. Another few points are added below 
			 */
			if (f[2] & TR2_LIGHT) p += 30;

			break;
		}
		case TV_RING:
		case TV_AMULET:
		{
			p += BASE_JEWELRY_POWER;
			LOG_PRINT("Jewellery - adding base power\n");

			p += sign(o_ptr->to_h) * (ABS(o_ptr->to_h) * TO_HIT_POWER);
			LOG_PRINT1("Adding power for to_hit, total is %d\n", p);

			p += o_ptr->to_d * DAMAGE_POWER;
			LOG_PRINT1("Adding power for to_dam, total is %d\n", p);

			/* Apply the correct brand/slay multiplier */
			p += (((2 * (o_ptr->to_d + RING_BRAND_DMG)
				* slay_power(o_ptr, verbose, log_file, f))
				/ tot_mon_power) - (2 * (o_ptr->to_d + RING_BRAND_DMG)));
			LOG_PRINT1("Adjusted for brand/slay power, total is %d\n", p);

			/* Add power for extra blows */
			if ((f[0] & TR0_BLOWS) && (known ||
				object_pval_is_visible(o_ptr)))
			{
				LOG_PRINT1("Extra blows: %d\n", o_ptr->pval);
				if (o_ptr->pval >= INHIBIT_BLOWS || o_ptr->pval < 0)
				{
					p += INHIBIT_POWER;	/* inhibit */
					LOG_PRINT("INHIBITING, too many extra blows or a negative number\n");
				}
				else if (o_ptr->pval > 0)
				{
					/* We assume a 3d5 weapon with +10 damage elsewhere - gives ~16 power per extra blow */
					p += ((MELEE_DAMAGE_BOOST + RING_BRAND_DMG + o_ptr->to_d) * o_ptr->pval * DAMAGE_POWER / MAX_BLOWS);
					LOG_PRINT1("Adding power for extra blows, total is %d\n", p);
				}
			}

			/* Add power for extra shots */
			if ((f[0] & TR0_SHOTS) && (known ||
				object_pval_is_visible(o_ptr)))
			{
				LOG_PRINT1("Extra shots: %d\n", o_ptr->pval);
				if (o_ptr->pval >= INHIBIT_SHOTS || o_ptr->pval < 0)
				{
					p += INHIBIT_POWER;	/* inhibit */
					LOG_PRINT("INHIBITING - too many extra shots\n");
				}
				else if (o_ptr->pval > 0)
				{
					/* We assume a x2.5 +9dam launcher with +9 1d4 ammo - gives ~25 power per extra shot */
					p += ((AVG_BOW_AMMO_DAMAGE + AVG_LAUNCHER_DMG) * AVG_BOW_MULT * o_ptr->pval * DAMAGE_POWER / (2 * BOW_RESCALER));
					LOG_PRINT1("Adding power for extra shots - total is %d\n", p);
				}
			}

			break;
		}
	}

	/* Other abilities are evaluated independent of the object type. */
	p += sign(o_ptr->to_a) * (ABS(o_ptr->to_a) * TO_AC_POWER / 2);
	LOG_PRINT2("Adding power for to_ac of %d, total is %d\n", o_ptr->to_a, p);

	if (o_ptr->to_a > HIGH_TO_AC)
	{
		p += ((o_ptr->to_a - (HIGH_TO_AC - 1)) * TO_AC_POWER / 2);
		LOG_PRINT1("Adding power for high to_ac value, total is %d\n", p);
	}
	if (o_ptr->to_a > VERYHIGH_TO_AC)
	{
		p += ((o_ptr->to_a - (VERYHIGH_TO_AC -1)) * TO_AC_POWER / 2);
		LOG_PRINT1("Adding power for very high to_ac value, total is %d\n", p);
	}
	if (o_ptr->to_a >= INHIBIT_AC)
	{
		p += INHIBIT_POWER;	/* inhibit */
		LOG_PRINT("INHIBITING: AC bonus too high\n");
	}

	if ((o_ptr->pval > 0) && (known || object_pval_is_visible(o_ptr)))
	{
		if (f[0] & TR0_STR)
		{
			p += STR_POWER * o_ptr->pval;
			LOG_PRINT2("Adding power for STR bonus %d, total is %d\n", o_ptr->pval, p);
		}
		if (f[0] & TR0_INT)
		{
			p += INT_POWER * o_ptr->pval;
			LOG_PRINT2("Adding power for INT bonus %d, total is %d\n", o_ptr->pval, p);
		}
		if (f[0] & TR0_WIS)
		{
			p += WIS_POWER * o_ptr->pval;
			LOG_PRINT2("Adding power for WIS bonus %d, total is %d\n", o_ptr->pval, p);
		}
		if (f[0] & TR0_DEX)
		{
			p += DEX_POWER * o_ptr->pval;
			LOG_PRINT2("Adding power for DEX bonus %d, total is %d\n", o_ptr->pval, p);
		}
		if (f[0] & TR0_CON)
		{
			p += CON_POWER * o_ptr->pval;
			LOG_PRINT2("Adding power for CON bonus %d, total is %d\n", o_ptr->pval, p);
		}
		if (f[0] & TR0_STEALTH)
		{
			p += STEALTH_POWER * o_ptr->pval;
			LOG_PRINT2("Adding power for Stealth bonus %d, total is %d\n", o_ptr->pval, p);
		}
		if (f[0] & TR0_SEARCH)
		{
			p += SEARCH_POWER * o_ptr->pval;
			LOG_PRINT2("Adding power for searching bonus %d, total is %d\n", o_ptr->pval , p);
		}
		/* Add extra power term if there are a lot of ability bonuses */
		if (o_ptr->pval > 0)
		{
			extra_stat_bonus += ( (f[0] & TR0_STR) ? 1 * o_ptr->pval: 0);
			extra_stat_bonus += ( (f[0] & TR0_INT) ? 3 * o_ptr->pval / 4: 0);
			extra_stat_bonus += ( (f[0] & TR0_WIS) ? 3 * o_ptr->pval / 4: 0);
			extra_stat_bonus += ( (f[0] & TR0_DEX) ? 1 * o_ptr->pval: 0);
			extra_stat_bonus += ( (f[0] & TR0_CON) ? 1 * o_ptr->pval: 0);
			extra_stat_bonus += ( (f[0] & TR0_CHR) ? 0 * o_ptr->pval: 0);
			extra_stat_bonus += ( (f[0] & TR0_STEALTH) ? 1 * o_ptr->pval: 0);
			extra_stat_bonus += ( (f[0] & TR0_INFRA) ? 0 * o_ptr->pval: 0);
			extra_stat_bonus += ( (f[0] & TR0_TUNNEL) ? 0 * o_ptr->pval: 0);
			extra_stat_bonus += ( (f[0] & TR0_SEARCH) ? 0 * o_ptr->pval: 0);
			extra_stat_bonus += ( (f[0] & TR0_SPEED) ? 0 * o_ptr->pval: 0);

			if (o_ptr->tval == TV_BOW)
			{
				extra_stat_bonus += ( (f[0] & TR0_MIGHT) ? 5 * o_ptr->pval / 2: 0);
				extra_stat_bonus += ( (f[0] & TR0_SHOTS) ? 3 * o_ptr->pval: 0);
			}
			else if ( (o_ptr->tval == TV_DIGGING) || (o_ptr->tval == TV_HAFTED) ||
			          (o_ptr->tval == TV_POLEARM) || (o_ptr->tval == TV_SWORD) )
			{
				extra_stat_bonus += ( (f[0] & TR0_BLOWS) ? 3 * o_ptr->pval: 0);
			}

			if (extra_stat_bonus > 24)
			{
				/* Inhibit */
				LOG_PRINT1("Inhibiting!  (Total ability bonus of %d is too high)\n", extra_stat_bonus);
				p += INHIBIT_POWER;
			}
			else
			{
				p += ability_power[extra_stat_bonus];
				LOG_PRINT2("Adding power for combination of %d, total is %d\n", ability_power[extra_stat_bonus], p);
			}
		}

	}
	else if ((o_ptr->pval < 0) && (known || object_pval_is_visible(o_ptr)))
	{
		if (f[0] & TR0_STR) p += 4 * o_ptr->pval;
		if (f[0] & TR0_INT) p += 2 * o_ptr->pval;
		if (f[0] & TR0_WIS) p += 2 * o_ptr->pval;
		if (f[0] & TR0_DEX) p += 3 * o_ptr->pval;
		if (f[0] & TR0_CON) p += 4 * o_ptr->pval;
		if (f[0] & TR0_STEALTH) p += o_ptr->pval;
		LOG_PRINT1("Subtracting power for negative ability values, total is %d\n", p);
	}

	if (known || object_pval_is_visible(o_ptr))
	{
		if (f[0] & TR0_CHR)
		{
			p += CHR_POWER * o_ptr->pval;
			LOG_PRINT2("Adding power for CHR bonus/penalty %d, total is %d\n", o_ptr->pval, p);
		}
		if (f[0] & TR0_INFRA)
		{
			p += INFRA_POWER * o_ptr->pval;
			LOG_PRINT2("Adding power for infra bonus/penalty %d, total is %d\n", o_ptr->pval, p);
		}
		if (f[0] & TR0_TUNNEL)
		{
			p += TUNN_POWER * o_ptr->pval;
			LOG_PRINT2("Adding power for tunnelling bonus/penalty %d, total is %d\n", o_ptr->pval, p);
		}
		if (f[0] & TR0_SPEED)
		{
			p += sign(o_ptr->pval) * speed_power[ABS(o_ptr->pval)];
			LOG_PRINT2("Adding power for speed bonus/penalty %d, total is %d\n", o_ptr->pval, p);
		}
	}
	
#define ADD_POWER1(string, val, flag, flgnum) \
	if (f[flgnum] & flag) { \
		p += (val); \
		LOG_PRINT1("Adding power for " string ", total is %d\n", p); \
	}

#define ADD_POWER2(string, val, flag, flgnum, extra) \
	if (f[flgnum] & flag) { \
		p += (val); \
		extra; \
		LOG_PRINT1("Adding power for " string ", total is %d\n", p); \
	}

	ADD_POWER2("sustain STR",         9, TR1_SUST_STR, 1, sustains++);
	ADD_POWER2("sustain INT",         4, TR1_SUST_INT, 1, sustains++);
	ADD_POWER2("sustain WIS",         4, TR1_SUST_WIS, 1, sustains++);
	ADD_POWER2("sustain DEX",         7, TR1_SUST_DEX, 1, sustains++);
	ADD_POWER2("sustain CON",         8, TR1_SUST_CON, 1, sustains++);
	ADD_POWER1("sustain CHR",         1, TR1_SUST_CHR, 1);

	for (i = 2; i <= sustains; i++)
	{
		p += i;
		LOG_PRINT1("Adding power for multiple sustains, total is %d\n", p);
		if (i == 5)
		{
			p += SUST_POWER;
			LOG_PRINT1("Adding power for full set of sustains, total is %d\n", p);
		}
	}
	
	ADD_POWER2("acid immunity",      38, TR1_IM_ACID,  1, immunities++);
	ADD_POWER2("elec immunity",      35, TR1_IM_ELEC,  1, immunities++);
	ADD_POWER2("fire immunity",      40, TR1_IM_FIRE,  1, immunities++);
	ADD_POWER2("cold immunity",      37, TR1_IM_COLD,  1, immunities++);

	for (i = 2; i <= immunities; i++)
	{
		p += IMMUNITY_POWER;
		LOG_PRINT1("Adding power for multiple immunities, total is %d\n", p);
		if (i >= INHIBIT_IMMUNITIES)
		{
			p += INHIBIT_POWER;             /* inhibit */
			LOG_PRINT("INHIBITING: Too many immunities\n");
		}
	}

	ADD_POWER2("free action",           14, TR2_FREE_ACT,    2, misc++);
	ADD_POWER2("hold life",             12, TR2_HOLD_LIFE,   2, misc++);
	ADD_POWER1("feather fall",           1, TR2_FEATHER,     2);
	ADD_POWER2("permanent light",        3, TR2_LIGHT,       2, misc++);
	ADD_POWER2("see invisible",         10, TR2_SEE_INVIS,   2, misc++);
	ADD_POWER2("telepathy",             70, TR2_TELEPATHY,   2, misc++);
	ADD_POWER2("slow digestion",         2, TR2_SLOW_DIGEST, 2, misc++);
	ADD_POWER2("resist acid",            5, TR1_RES_ACID,    1, lowres++);
	ADD_POWER2("resist elec",            6, TR1_RES_ELEC,    1, lowres++);
	ADD_POWER2("resist fire",            6, TR1_RES_FIRE,    1, lowres++);
	ADD_POWER2("resist cold",            6, TR1_RES_COLD,    1, lowres++);
	ADD_POWER2("resist poison",         28, TR1_RES_POIS,    1, highres++);
	ADD_POWER2("resist fear",            6, TR1_RES_FEAR,    1, highres++);
	ADD_POWER2("resist light",           6, TR1_RES_LIGHT,   1, highres++);
	ADD_POWER2("resist dark",           16, TR1_RES_DARK,    1, highres++);
	ADD_POWER2("resist blindness",      16, TR1_RES_BLIND,   1, highres++);
	ADD_POWER2("resist confusion",      24, TR1_RES_CONFU,   1, highres++);
	ADD_POWER2("resist sound",          14, TR1_RES_SOUND,   1, highres++);
	ADD_POWER2("resist shards",          8, TR1_RES_SHARD,   1, highres++);
	ADD_POWER2("resist nexus",          15, TR1_RES_NEXUS,   1, highres++);
	ADD_POWER2("resist nether",         20, TR1_RES_NETHR,   1, highres++);
	ADD_POWER2("resist chaos",          20, TR1_RES_CHAOS,   1, highres++);
	ADD_POWER2("resist disenchantment", 20, TR1_RES_DISEN,   1, highres++);
	ADD_POWER2("regeneration",           9, TR2_REGEN,       2, misc++);
	ADD_POWER1("blessed",                1, TR2_BLESSED,     2);
	ADD_POWER1("no fuel",                5, TR2_NO_FUEL,     2);

	for (i = 2; i <= misc; i++)
	{
		p += i;
		LOG_PRINT1("Adding power for multiple misc abilities, total is %d\n", p);
	}

	for (i = 2; i <= lowres; i++)
	{
		p += i;
		LOG_PRINT1("Adding power for multiple low resists, total is %d\n", p);
		if (i == 4)
		{
			p += RBASE_POWER;
			LOG_PRINT1("Adding power for full rbase set, total is %d\n", p);
		}
	}

	for (i = 2; i <= highres; i++)
	{
		p += (i * 2);
		LOG_PRINT1("Adding power for multiple high resists, total is %d\n", p);
	}

	/* Note: the following code is irrelevant until curses are reworked */
	if (f[2] & TR2_TELEPORT)
	{
		p -= 1;
		LOG_PRINT1("Subtracting power for teleportation, total is %d\n", p);
	}
	if (f[2] & TR2_DRAIN_EXP)
	{
		p -= 1;
		LOG_PRINT1("Subtracting power for drain experience, total is %d\n", p);
	}
	if (f[2] & TR2_AGGRAVATE)
	{
		p -= 1;
		LOG_PRINT1("Subtracting power for aggravation, total is %d\n", p);
	}
	if (f[2] & TR2_LIGHT_CURSE)
	{
		p -= 1;
		LOG_PRINT1("Subtracting power for light curse, total is %d\n", p);
	}
	if (f[2] & TR2_HEAVY_CURSE)
	{
		p -= 1;
		LOG_PRINT1("Subtracting power for heavy curse, total is %d\n", p);
	}

	/*	if (f[2] & TR2_PERMA_CURSE) p -= 40; */

	/* add power for effect */
	if (known || object_effect_is_known(o_ptr))
	{
		if (o_ptr->name1 && a_info[o_ptr->name1].effect)
		{
			p += effect_power(a_info[o_ptr->name1].effect);
			LOG_PRINT1("Adding power for artifact activation, total is %d\n", p);
		}
		else
		{
			p += effect_power(k_info[o_ptr->k_idx].effect);
			LOG_PRINT1("Adding power for item activation, total is %d\n", p);
		}
	}

	/* add tiny amounts for ignore flags */
	if (f[2] & TR2_IGNORE_ACID) p++;
	if (f[2] & TR2_IGNORE_FIRE) p++;
	if (f[2] & TR2_IGNORE_COLD) p++;
	if (f[2] & TR2_IGNORE_ELEC) p++;

	LOG_PRINT1("After ignore flags, FINAL POWER IS %d\n", p);

	return (p);
}
