#include "../defines.h"
#include "../defines_battle.h"
#include "../../include/new/dynamax.h"
#include "../../include/new/z_move_effects.h"

/*
battle_moves.c
	Set up attack data and dynamax powers for each move
*/

const struct BattleMove gBattleMoves[] =
{
	[MOVE_NONE] =
	{
		.effect = EFFECT_HIT,
		.power = 40,
		.type = TYPE_MYSTERY,
		.accuracy = 100,
		.pp = 35,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_POUND] =
	{
		.effect = EFFECT_HIT,
		.power = 40,
		.type = TYPE_NORMAL,
		.accuracy = 100,
		.pp = 35,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_KARATECHOP] =
	{
		.effect = EFFECT_HIGH_CRITICAL,
		.power = 50,
		.type = TYPE_FIGHTING,
		.accuracy = 100,
		.pp = 25,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_DOUBLESLAP] =
	{
		.effect = EFFECT_MULTI_HIT,
		.power = 15,
		.type = TYPE_NORMAL,
		.accuracy = 85,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_COMETPUNCH] =
	{
		.effect = EFFECT_MULTI_HIT,
		.power = 18,
		.type = TYPE_NORMAL,
		.accuracy = 85,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_MEGAPUNCH] =
	{
		.effect = EFFECT_HIT,
		.power = 80,
		.type = TYPE_NORMAL,
		.accuracy = 85,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 160,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_PAYDAY] =
	{
		.effect = EFFECT_PAY_DAY,
		.power = 40,
		.type = TYPE_NORMAL,
		.accuracy = 100,
		.pp = 20,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_FIREPUNCH] =
	{
		.effect = EFFECT_BURN_HIT,
		.power = 75,
		.type = TYPE_FIRE,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 10,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 140,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_ICEPUNCH] =
	{
		.effect = EFFECT_FREEZE_HIT,
		.power = 75,
		.type = TYPE_ICE,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 10,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 140,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_THUNDERPUNCH] =
	{
		.effect = EFFECT_PARALYZE_HIT,
		.power = 75,
		.type = TYPE_ELECTRIC,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 10,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 140,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_SCRATCH] =
	{
		.effect = EFFECT_HIT,
		.power = 40,
		.type = TYPE_NORMAL,
		.accuracy = 100,
		.pp = 35,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_VICEGRIP] =
	{
		.effect = EFFECT_HIT,
		.power = 55,
		.type = TYPE_NORMAL,
		.accuracy = 100,
		.pp = 30,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_GUILLOTINE] =
	{
		.effect = EFFECT_0HKO,
		.power = 1,
		.type = TYPE_NORMAL,
		.accuracy = 30,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 180,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_RAZORWIND] =
	{
		.effect = EFFECT_RAZOR_WIND,
		.power = 80,
		.type = TYPE_NORMAL,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_BOTH,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 160,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_SWORDSDANCE] =
	{
		.effect = EFFECT_ATTACK_UP_2,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 0,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = FLAG_SNATCH_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_RESET_STATS
	},

	[MOVE_CUT] =
	{
		.effect = EFFECT_HIT,
		.power = 50,
		.type = TYPE_NORMAL,
		.accuracy = 95,
		.pp = 30,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_GUST] =
	{
		.effect = EFFECT_GUST,
		.power = 40,
		.type = TYPE_FLYING,
		.accuracy = 100,
		.pp = 35,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_WINGATTACK] =
	{
		.effect = EFFECT_HIT,
		.power = 60,
		.type = TYPE_FLYING,
		.accuracy = 100,
		.pp = 35,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 120,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_WHIRLWIND] =
	{
		.effect = EFFECT_ROAR,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 0,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 250,
		.flags = FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_SPDEF_UP_1
	},

	[MOVE_FLY] =
	{
		.effect = EFFECT_SEMI_INVULNERABLE,
		#ifdef UNBOUND
		.power = 100,
		#else
		.power = 90,
		#endif
		.type = TYPE_FLYING,
		.accuracy = 95,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 175,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_BIND] =
	{
		.effect = EFFECT_TRAP,
		.power = 15,
		.type = TYPE_NORMAL,
		.accuracy = 85,
		.pp = 20,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_SLAM] =
	{
		.effect = EFFECT_HIT,
		.power = 80,
		.type = TYPE_NORMAL,
		.accuracy = 75,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 160,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_VINEWHIP] =
	{
		.effect = EFFECT_HIT,
		.power = 45,
		.type = TYPE_GRASS,
		.accuracy = 100,
		.pp = 25,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_STOMP] =
	{
		.effect = EFFECT_FLINCH_HIT,
		.power = 65,
		.type = TYPE_NORMAL,
		.accuracy = 100,
		.pp = 20,
		.secondaryEffectChance = 30,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 120,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_DOUBLEKICK] =
	{
		.effect = EFFECT_DOUBLE_HIT,
		.power = 30,
		.type = TYPE_FIGHTING,
		.accuracy = 100,
		.pp = 30,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_MEGAKICK] =
	{
		.effect = EFFECT_HIT,
		.power = 120,
		.type = TYPE_NORMAL,
		.accuracy = 75,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 190,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_JUMPKICK] =
	{
		.effect = EFFECT_RECOIL_IF_MISS,
		.power = 100,
		.type = TYPE_FIGHTING,
		.accuracy = 95,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 180,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_ROLLINGKICK] =
	{
		.effect = EFFECT_FLINCH_HIT,
		.power = 60,
		.type = TYPE_FIGHTING,
		.accuracy = 85,
		.pp = 15,
		.secondaryEffectChance = 30,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 120,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_SANDATTACK] =
	{
		.effect = EFFECT_ACCURACY_DOWN,
		.power = 0,
		.type = TYPE_GROUND,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_EVSN_UP_1
	},

	[MOVE_HEADBUTT] =
	{
		.effect = EFFECT_FLINCH_HIT,
		.power = 70,
		.type = TYPE_NORMAL,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 30,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 140,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_HORNATTACK] =
	{
		.effect = EFFECT_HIT,
		.power = 65,
		.type = TYPE_NORMAL,
		.accuracy = 100,
		.pp = 25,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 120,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_FURYATTACK] =
	{
		.effect = EFFECT_MULTI_HIT,
		.power = 15,
		.type = TYPE_NORMAL,
		.accuracy = 85,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_HORNDRILL] =
	{
		.effect = EFFECT_0HKO,
		.power = 1,
		.type = TYPE_NORMAL,
		.accuracy = 30,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 180,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_TACKLE] =
	{
		.effect = EFFECT_HIT,
		.power = 40,
		.type = TYPE_NORMAL,
		.accuracy = 100,
		.pp = 35,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_BODYSLAM] =
	{
		.effect = EFFECT_PARALYZE_HIT,
		.power = 85,
		.type = TYPE_NORMAL,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 30,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 160,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_WRAP] =
	{
		.effect = EFFECT_TRAP,
		.power = 15,
		.type = TYPE_NORMAL,
		.accuracy = 90,
		.pp = 20,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_TAKEDOWN] =
	{
		.effect = EFFECT_RECOIL,
		.power = 90,
		.type = TYPE_NORMAL,
		.accuracy = 85,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 175,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_THRASH] =
	{
		.effect = EFFECT_RAMPAGE,
		.power = 120,
		.type = TYPE_NORMAL,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_RANDOM,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 190,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_DOUBLEEDGE] =
	{
		.effect = EFFECT_RECOIL,
		.power = 120,
		.type = TYPE_NORMAL,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 190,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_TAILWHIP] =
	{
		.effect = EFFECT_DEFENSE_DOWN,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 100,
		.pp = 30,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_BOTH,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_ATK_UP_1
	},

	[MOVE_POISONSTING] =
	{
		.effect = EFFECT_POISON_HIT,
		.power = 15,
		.type = TYPE_POISON,
		.accuracy = 100,
		.pp = 35,
		.secondaryEffectChance = 30,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_TWINEEDLE] =
	{
		.effect = EFFECT_POISON_HIT,
		.power = 25,
		.type = TYPE_BUG,
		.accuracy = 100,
		.pp = 20,
		.secondaryEffectChance = 20,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_PINMISSILE] =
	{
		.effect = EFFECT_MULTI_HIT,
		.power = 25,
		.type = TYPE_BUG,
		.accuracy = 95,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_LEER] =
	{
		.effect = EFFECT_DEFENSE_DOWN,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 100,
		.pp = 30,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_BOTH,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_ATK_UP_1
	},

	[MOVE_BITE] =
	{
		.effect = EFFECT_FLINCH_HIT,
		.power = 60,
		.type = TYPE_DARK,
		.accuracy = 100,
		.pp = 25,
		.secondaryEffectChance = 30,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 120,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_GROWL] =
	{
		.effect = EFFECT_ATTACK_DOWN,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 100,
		.pp = 40,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_BOTH,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_DEF_UP_1
	},

	[MOVE_ROAR] =
	{
		.effect = EFFECT_ROAR,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 0,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 250,
		.flags = FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_DEF_UP_1
	},

	[MOVE_SING] =
	{
		.effect = EFFECT_SLEEP,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 55,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_SPD_UP_1
	},

	[MOVE_SUPERSONIC] =
	{
		.effect = EFFECT_CONFUSE,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 55,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_SPD_UP_1
	},

	[MOVE_SONICBOOM] =
	{
		.effect = EFFECT_SONICBOOM,
		.power = 1,
		.type = TYPE_NORMAL,
		.accuracy = 90,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_DISABLE] =
	{
		.effect = EFFECT_DISABLE,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 100,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_RESET_STATS
	},

	[MOVE_ACID] =
	{
		.effect = EFFECT_SPECIAL_DEFENSE_DOWN_HIT,
		.power = 40,
		.type = TYPE_POISON,
		.accuracy = 100,
		.pp = 30,
		.secondaryEffectChance = 10,
		.target = MOVE_TARGET_BOTH,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_EMBER] =
	{
		.effect = EFFECT_BURN_HIT,
		.power = 40,
		.type = TYPE_FIRE,
		.accuracy = 100,
		.pp = 25,
		.secondaryEffectChance = 10,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_FLAMETHROWER] =
	{
		.effect = EFFECT_BURN_HIT,
		#ifdef GEN_6_POWER_NERFS
		.power = 90,
		#else
		.power = 95,
		#endif
		.type = TYPE_FIRE,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 10,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 175,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_MIST] =
	{
		.effect = EFFECT_MIST,
		.power = 0,
		.type = TYPE_ICE,
		.accuracy = 0,
		.pp = 30,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = FLAG_SNATCH_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_RECOVER_HP
	},

	[MOVE_WATERGUN] =
	{
		.effect = EFFECT_HIT,
		.power = 40,
		.type = TYPE_WATER,
		.accuracy = 100,
		.pp = 25,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_HYDROPUMP] =
	{
		.effect = EFFECT_HIT,
		#ifdef GEN_6_POWER_NERFS
		.power = 110,
		#else
		.power = 120,
		#endif
		.type = TYPE_WATER,
		.accuracy = 80,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 190,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_SURF] =
	{
		.effect = EFFECT_HIT,
		#ifdef GEN_6_POWER_NERFS
		.power = 90,
		#else
		.power = 95,
		#endif
		.type = TYPE_WATER,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_FOES_AND_ALLY,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 175,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_ICEBEAM] =
	{
		.effect = EFFECT_FREEZE_HIT,
		#ifdef GEN_6_POWER_NERFS
		.power = 90,
		#else
		.power = 95,
		#endif
		.type = TYPE_ICE,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 10,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 175,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_BLIZZARD] =
	{
		.effect = EFFECT_FREEZE_HIT,
		#ifdef GEN_6_POWER_NERFS
		.power = 110,
		#else
		.power = 120,
		#endif
		.type = TYPE_ICE,
		.accuracy = 70,
		.pp = 5,
		.secondaryEffectChance = 10,
		.target = MOVE_TARGET_BOTH,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 190,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_PSYBEAM] =
	{
		.effect = EFFECT_CONFUSE_HIT,
		.power = 65,
		.type = TYPE_PSYCHIC,
		.accuracy = 100,
		.pp = 20,
		.secondaryEffectChance = 10,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 120,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_BUBBLEBEAM] =
	{
		.effect = EFFECT_SPEED_DOWN_HIT,
		.power = 65,
		.type = TYPE_WATER,
		.accuracy = 100,
		.pp = 20,
		.secondaryEffectChance = 10,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 120,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_AURORABEAM] =
	{
		.effect = EFFECT_ATTACK_DOWN_HIT,
		.power = 65,
		.type = TYPE_ICE,
		.accuracy = 100,
		.pp = 20,
		.secondaryEffectChance = 10,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 120,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_HYPERBEAM] =
	{
		.effect = EFFECT_RECHARGE,
		.power = 150,
		.type = TYPE_NORMAL,
		.accuracy = 90,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 200,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_PECK] =
	{
		.effect = EFFECT_HIT,
		.power = 35,
		.type = TYPE_FLYING,
		.accuracy = 100,
		.pp = 35,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_DRILLPECK] =
	{
		.effect = EFFECT_HIT,
		.power = 80,
		.type = TYPE_FLYING,
		.accuracy = 100,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 160,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_SUBMISSION] =
	{
		.effect = EFFECT_RECOIL,
		.power = 80,
		.type = TYPE_FIGHTING,
		.accuracy = 80,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 160,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_LOWKICK] =
	{
		.effect = EFFECT_HIT,
		.power = 1,
		.type = TYPE_FIGHTING,
		.accuracy = 100,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 160,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_COUNTER] =
	{
		.effect = EFFECT_COUNTER,
		.power = 1,
		.type = TYPE_FIGHTING,
		.accuracy = 100,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_DEPENDS,
		.priority = 251,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_SEISMICTOSS] =
	{
		.effect = EFFECT_LEVEL_DAMAGE,
		.power = 1,
		.type = TYPE_FIGHTING,
		.accuracy = 100,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_STRENGTH] =
	{
		.effect = EFFECT_HIT,
		.power = 80,
		.type = TYPE_NORMAL,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 160,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_ABSORB] =
	{
		.effect = EFFECT_ABSORB,
		.power = 25,
		.type = TYPE_GRASS,
		.accuracy = 100,
		.pp = 25,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_TRIAGE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_MEGADRAIN] =
	{
		.effect = EFFECT_ABSORB,
		.power = 40,
		.type = TYPE_GRASS,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_TRIAGE_AFFECTED,
		.z_move_power = 120,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_LEECHSEED] =
	{
		.effect = EFFECT_LEECH_SEED,
		.power = 0,
		.type = TYPE_GRASS,
		.accuracy = 90,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_RESET_STATS
	},

	[MOVE_GROWTH] =
	{
		.effect = EFFECT_ATK_SPATK_UP,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 0,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = FLAG_SNATCH_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_SPATK_UP_1
	},

	[MOVE_RAZORLEAF] =
	{
		.effect = EFFECT_HIGH_CRITICAL,
		.power = 55,
		.type = TYPE_GRASS,
		.accuracy = 95,
		.pp = 25,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_BOTH,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_SOLARBEAM] =
	{
		.effect = EFFECT_SOLARBEAM,
		.power = 120,
		.type = TYPE_GRASS,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 190,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_POISONPOWDER] =
	{
		.effect = EFFECT_POISON,
		.power = 0,
		.type = TYPE_POISON,
		.accuracy = 75,
		.pp = 35,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_DEF_UP_1
	},

	[MOVE_STUNSPORE] =
	{
		.effect = EFFECT_PARALYZE,
		.power = 0,
		.type = TYPE_GRASS,
		.accuracy = 75,
		.pp = 30,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_SPDEF_UP_1
	},

	[MOVE_SLEEPPOWDER] =
	{
		.effect = EFFECT_SLEEP,
		.power = 0,
		.type = TYPE_GRASS,
		.accuracy = 75,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_SPD_UP_1
	},

	[MOVE_PETALDANCE] =
	{
		.effect = EFFECT_RAMPAGE,
		.power = 120,
		.type = TYPE_GRASS,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_RANDOM,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 190,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_STRINGSHOT] =
	{
		.effect = EFFECT_SPEED_DOWN_2,
		.power = 0,
		.type = TYPE_BUG,
		.accuracy = 95,
		.pp = 40,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_BOTH,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_SPD_UP_1
	},

	[MOVE_DRAGONRAGE] =
	{
		.effect = EFFECT_DRAGON_RAGE,
		.power = 1,
		.type = TYPE_DRAGON,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_FIRESPIN] =
	{
		.effect = EFFECT_TRAP,
		.power = 35,
		.type = TYPE_FIRE,
		.accuracy = 85,
		.pp = 15,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_THUNDERSHOCK] =
	{
		.effect = EFFECT_PARALYZE_HIT,
		.power = 40,
		.type = TYPE_ELECTRIC,
		.accuracy = 100,
		.pp = 30,
		.secondaryEffectChance = 10,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_THUNDERBOLT] =
	{
		.effect = EFFECT_PARALYZE_HIT,
		#ifdef GEN_6_POWER_NERFS
		.power = 90,
		#else
		.power = 95,
		#endif
		.type = TYPE_ELECTRIC,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 10,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 175,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_THUNDERWAVE] =
	{
		.effect = EFFECT_PARALYZE,
		.power = 0,
		.type = TYPE_ELECTRIC,
		.accuracy = 90,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_SPDEF_UP_1
	},

	[MOVE_THUNDER] =
	{
		.effect = EFFECT_THUNDER,
		#ifdef GEN_6_POWER_NERFS
		.power = 110,
		#else
		.power = 120,
		#endif
		.type = TYPE_ELECTRIC,
		.accuracy = 70,
		.pp = 10,
		.secondaryEffectChance = 30,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 190,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_ROCKTHROW] =
	{
		.effect = EFFECT_HIT,
		.power = 50,
		.type = TYPE_ROCK,
		.accuracy = 90,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_EARTHQUAKE] =
	{
		.effect = EFFECT_HIT,
		.power = 100,
		.type = TYPE_GROUND,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_FOES_AND_ALLY,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 180,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_FISSURE] =
	{
		.effect = EFFECT_0HKO,
		.power = 1,
		.type = TYPE_GROUND,
		.accuracy = 30,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 180,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_DIG] =
	{
		.effect = EFFECT_SEMI_INVULNERABLE,
		.power = 80,
		.type = TYPE_GROUND,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 160,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_TOXIC] =
	{
		.effect = EFFECT_TOXIC,
		.power = 0,
		.type = TYPE_POISON,
		.accuracy = 90,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_DEF_UP_1
	},

	[MOVE_CONFUSION] =
	{
		.effect = EFFECT_CONFUSE_HIT,
		.power = 50,
		.type = TYPE_PSYCHIC,
		.accuracy = 100,
		.pp = 25,
		.secondaryEffectChance = 10,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_PSYCHIC] =
	{
		.effect = EFFECT_SPECIAL_DEFENSE_DOWN_HIT,
		.power = 90,
		.type = TYPE_PSYCHIC,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 10,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 175,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_HYPNOSIS] =
	{
		.effect = EFFECT_SLEEP,
		.power = 0,
		.type = TYPE_PSYCHIC,
		.accuracy = 60,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_SPD_UP_1
	},

	[MOVE_MEDITATE] =
	{
		.effect = EFFECT_ATTACK_UP,
		.power = 0,
		.type = TYPE_PSYCHIC,
		.accuracy = 0,
		.pp = 40,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = FLAG_SNATCH_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_ATK_UP_1
	},

	[MOVE_AGILITY] =
	{
		.effect = EFFECT_SPEED_UP_2,
		.power = 0,
		.type = TYPE_PSYCHIC,
		.accuracy = 0,
		.pp = 30,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = FLAG_SNATCH_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_RESET_STATS
	},

	[MOVE_QUICKATTACK] =
	{
		.effect = EFFECT_QUICK_ATTACK,
		.power = 40,
		.type = TYPE_NORMAL,
		.accuracy = 100,
		.pp = 30,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 1,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_RAGE] =
	{
		.effect = EFFECT_RAGE,
		.power = 20,
		.type = TYPE_NORMAL,
		.accuracy = 100,
		.pp = 20,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_TELEPORT] =
	{
		.effect = EFFECT_TELEPORT,
		.power = 0,
		.type = TYPE_PSYCHIC,
		.accuracy = 0,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 250,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_RECOVER_HP
	},

	[MOVE_NIGHTSHADE] =
	{
		.effect = EFFECT_LEVEL_DAMAGE,
		.power = 1,
		.type = TYPE_GHOST,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_MIMIC] =
	{
		.effect = EFFECT_MIMIC,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_ACC_UP_1
	},

	[MOVE_SCREECH] =
	{
		.effect = EFFECT_DEFENSE_DOWN_2,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 85,
		.pp = 40,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_ATK_UP_1
	},

	[MOVE_DOUBLETEAM] =
	{
		.effect = EFFECT_EVASION_UP,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 0,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = FLAG_SNATCH_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_RESET_STATS
	},

	[MOVE_RECOVER] =
	{
		.effect = EFFECT_RESTORE_HP,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = FLAG_SNATCH_AFFECTED | FLAG_TRIAGE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_RESET_STATS
	},

	[MOVE_HARDEN] =
	{
		.effect = EFFECT_DEFENSE_UP,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 0,
		.pp = 30,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = FLAG_SNATCH_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_DEF_UP_1
	},

	[MOVE_MINIMIZE] =
	{
		.effect = EFFECT_MINIMIZE,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = FLAG_SNATCH_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_RESET_STATS
	},

	[MOVE_SMOKESCREEN] =
	{
		.effect = EFFECT_ACCURACY_DOWN,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 100,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_EVSN_UP_1
	},

	[MOVE_CONFUSERAY] =
	{
		.effect = EFFECT_CONFUSE,
		.power = 0,
		.type = TYPE_GHOST,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_SPATK_UP_1
	},

	[MOVE_WITHDRAW] =
	{
		.effect = EFFECT_DEFENSE_UP,
		.power = 0,
		.type = TYPE_WATER,
		.accuracy = 0,
		.pp = 40,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = FLAG_SNATCH_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_DEF_UP_1
	},

	[MOVE_DEFENSECURL] =
	{
		.effect = EFFECT_DEFENSE_CURL,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 0,
		.pp = 40,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = FLAG_SNATCH_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_ACC_UP_1
	},

	[MOVE_BARRIER] =
	{
		.effect = EFFECT_DEFENSE_UP_2,
		.power = 0,
		.type = TYPE_PSYCHIC,
		.accuracy = 0,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = FLAG_SNATCH_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_RESET_STATS
	},

	[MOVE_LIGHTSCREEN] =
	{
		.effect = EFFECT_LIGHT_SCREEN,
		.power = 0,
		.type = TYPE_PSYCHIC,
		.accuracy = 0,
		.pp = 30,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = FLAG_SNATCH_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_SPDEF_UP_1
	},

	[MOVE_HAZE] =
	{
		.effect = EFFECT_HAZE,
		.power = 0,
		.type = TYPE_ICE,
		.accuracy = 0,
		.pp = 30,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_RECOVER_HP
	},

	[MOVE_REFLECT] =
	{
		.effect = EFFECT_REFLECT,
		.power = 0,
		.type = TYPE_PSYCHIC,
		.accuracy = 0,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = FLAG_SNATCH_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_DEF_UP_1
	},

	[MOVE_FOCUSENERGY] =
	{
		.effect = EFFECT_FOCUS_ENERGY,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 0,
		.pp = 30,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = FLAG_SNATCH_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_ACC_UP_1
	},

	[MOVE_BIDE] =
	{
		.effect = EFFECT_BIDE,
		.power = 1,
		.type = TYPE_NORMAL,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 1,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_METRONOME] =
	{
		.effect = EFFECT_METRONOME,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_DEPENDS,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = 0
	},

	[MOVE_MIRRORMOVE] =
	{
		.effect = EFFECT_MIRROR_MOVE,
		.power = 0,
		.type = TYPE_FLYING,
		.accuracy = 0,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_ATK_UP_2
	},

	[MOVE_SELFDESTRUCT] =
	{
		.effect = EFFECT_EXPLOSION,
		.power = 200,
		.type = TYPE_NORMAL,
		.accuracy = 100,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_FOES_AND_ALLY,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 200,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_EGGBOMB] =
	{
		.effect = EFFECT_HIT,
		.power = 100,
		.type = TYPE_NORMAL,
		.accuracy = 75,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 180,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_LICK] =
	{
		.effect = EFFECT_PARALYZE_HIT,
		.power = 30,
		.type = TYPE_GHOST,
		.accuracy = 100,
		.pp = 30,
		.secondaryEffectChance = 30,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_SMOG] =
	{
		.effect = EFFECT_POISON_HIT,
		.power = 30,
		.type = TYPE_POISON,
		#ifdef UNBOUND
		.accuracy = 100,
		#else
		.accuracy = 70,
		#endif
		.pp = 20,
		.secondaryEffectChance = 40,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_SLUDGE] =
	{
		.effect = EFFECT_POISON_HIT,
		.power = 65,
		.type = TYPE_POISON,
		.accuracy = 100,
		.pp = 20,
		.secondaryEffectChance = 30,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 120,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_BONECLUB] =
	{
		.effect = EFFECT_FLINCH_HIT,
		.power = 65,
		.type = TYPE_GROUND,
		.accuracy = 85,
		.pp = 20,
		.secondaryEffectChance = 10,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 120,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_FIREBLAST] =
	{
		.effect = EFFECT_BURN_HIT,
		#ifdef GEN_6_POWER_NERFS
		.power = 110,
		#else
		.power = 120,
		#endif
		.type = TYPE_FIRE,
		.accuracy = 85,
		.pp = 5,
		.secondaryEffectChance = 10,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 190,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_WATERFALL] =
	{
		.effect = EFFECT_FLINCH_HIT,
		.power = 80,
		.type = TYPE_WATER,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 20,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 160,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_CLAMP] =
	{
		.effect = EFFECT_TRAP,
		.power = 35,
		.type = TYPE_WATER,
		.accuracy = 85,
		.pp = 15,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_SWIFT] =
	{
		.effect = EFFECT_ALWAYS_HIT,
		.power = 60,
		.type = TYPE_NORMAL,
		.accuracy = 0,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_BOTH,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 120,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_SKULLBASH] =
	{
		.effect = EFFECT_SKULL_BASH,
		.power = 130,
		.type = TYPE_NORMAL,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 195,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_SPIKECANNON] =
	{
		.effect = EFFECT_MULTI_HIT,
		.power = 20,
		.type = TYPE_NORMAL,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_CONSTRICT] =
	{
		.effect = EFFECT_SPEED_DOWN_HIT,
		.power = 10,
		.type = TYPE_NORMAL,
		.accuracy = 100,
		.pp = 35,
		.secondaryEffectChance = 10,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_AMNESIA] =
	{
		.effect = EFFECT_SPECIAL_DEFENSE_UP_2,
		.power = 0,
		.type = TYPE_PSYCHIC,
		.accuracy = 0,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = FLAG_SNATCH_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_RESET_STATS
	},

	[MOVE_KINESIS] =
	{
		.effect = EFFECT_ACCURACY_DOWN,
		.power = 0,
		.type = TYPE_PSYCHIC,
		.accuracy = 80,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_EVSN_UP_1
	},

	[MOVE_SOFTBOILED] =
	{
		.effect = EFFECT_RESTORE_HP,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = FLAG_SNATCH_AFFECTED | FLAG_TRIAGE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_RESET_STATS
	},

	[MOVE_HIGHJUMPKICK] =
	{
		.effect = EFFECT_RECOIL_IF_MISS,
		.power = 130,
		.type = TYPE_FIGHTING,
		.accuracy = 90,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 195,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_GLARE] =
	{
		.effect = EFFECT_PARALYZE,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 100,
		.pp = 30,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_SPDEF_UP_1
	},

	[MOVE_DREAMEATER] =
	{
		.effect = EFFECT_DREAM_EATER,
		.power = 100,
		.type = TYPE_PSYCHIC,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_TRIAGE_AFFECTED,
		.z_move_power = 180,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_POISONGAS] =
	{
		.effect = EFFECT_POISON,
		.power = 0,
		.type = TYPE_POISON,
		.accuracy = 90,
		.pp = 40,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_BOTH,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_DEF_UP_1
	},

	[MOVE_BARRAGE] =
	{
		.effect = EFFECT_MULTI_HIT,
		.power = 15,
		.type = TYPE_NORMAL,
		.accuracy = 85,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_LEECHLIFE] =
	{
		.effect = EFFECT_ABSORB,
		#ifdef BUFFED_LEECH_LIFE
		.power = 80,
		#else
		.power = 20,
		#endif
		.type = TYPE_BUG,
		.accuracy = 100,
		#ifdef BUFFED_LEECH_LIFE
		.pp = 10,
		#else
		.pp = 15,
		#endif
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_TRIAGE_AFFECTED,
		#ifdef BUFFED_LEECH_LIFE
		.z_move_power = 160,
		#else
		.z_move_power = 100,
		#endif
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_LOVELYKISS] =
	{
		.effect = EFFECT_SLEEP,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 75,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_SPD_UP_1
	},

	[MOVE_SKYATTACK] =
	{
		.effect = EFFECT_SKY_ATTACK,
		.power = 140,
		.type = TYPE_FLYING,
		.accuracy = 90,
		.pp = 5,
		.secondaryEffectChance = 30,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 200,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_TRANSFORM] =
	{
		.effect = EFFECT_TRANSFORM,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_RECOVER_HP
	},

	[MOVE_BUBBLE] =
	{
		.effect = EFFECT_SPEED_DOWN_HIT,
		.power = 40,
		.type = TYPE_WATER,
		.accuracy = 100,
		.pp = 30,
		.secondaryEffectChance = 10,
		.target = MOVE_TARGET_BOTH,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_DIZZYPUNCH] =
	{
		.effect = EFFECT_CONFUSE_HIT,
		.power = 70,
		.type = TYPE_NORMAL,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 20,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 140,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_SPORE] =
	{
		.effect = EFFECT_SLEEP,
		.power = 0,
		.type = TYPE_GRASS,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_RESET_STATS
	},

	[MOVE_FLASH] =
	{
		.effect = EFFECT_ACCURACY_DOWN,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 100,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_EVSN_UP_1
	},

	[MOVE_PSYWAVE] =
	{
		.effect = EFFECT_PSYWAVE,
		.power = 1,
		.type = TYPE_PSYCHIC,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_SPLASH] =
	{
		.effect = EFFECT_SPLASH,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 0,
		.pp = 40,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_ATK_UP_3
	},

	[MOVE_ACIDARMOR] =
	{
		.effect = EFFECT_DEFENSE_UP_2,
		.power = 0,
		.type = TYPE_POISON,
		.accuracy = 0,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = FLAG_SNATCH_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_RESET_STATS
	},

	[MOVE_CRABHAMMER] =
	{
		.effect = EFFECT_HIGH_CRITICAL,
		.power = 100,
		.type = TYPE_WATER,
		.accuracy = 90,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 180,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_EXPLOSION] =
	{
		.effect = EFFECT_EXPLOSION,
		.power = 250,
		.type = TYPE_NORMAL,
		.accuracy = 100,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_FOES_AND_ALLY,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 200,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_FURYSWIPES] =
	{
		.effect = EFFECT_MULTI_HIT,
		.power = 18,
		.type = TYPE_NORMAL,
		.accuracy = 80,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_BONEMERANG] =
	{
		.effect = EFFECT_DOUBLE_HIT,
		.power = 50,
		.type = TYPE_GROUND,
		.accuracy = 90,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_REST] =
	{
		.effect = EFFECT_REST,
		.power = 0,
		.type = TYPE_PSYCHIC,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = FLAG_SNATCH_AFFECTED | FLAG_TRIAGE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_RESET_STATS
	},

	[MOVE_ROCKSLIDE] =
	{
		.effect = EFFECT_FLINCH_HIT,
		.power = 75,
		.type = TYPE_ROCK,
		.accuracy = 90,
		.pp = 10,
		.secondaryEffectChance = 30,
		.target = MOVE_TARGET_BOTH,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 140,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_HYPERFANG] =
	{
		.effect = EFFECT_FLINCH_HIT,
		.power = 80,
		.type = TYPE_NORMAL,
		.accuracy = 90,
		.pp = 15,
		.secondaryEffectChance = 10,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 160,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_SHARPEN] =
	{
		.effect = EFFECT_ATTACK_UP,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 0,
		.pp = 30,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = FLAG_SNATCH_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_ATK_UP_1
	},

	[MOVE_CONVERSION] =
	{
		.effect = EFFECT_CONVERSION,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 0,
		.pp = 30,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = FLAG_SNATCH_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_ALL_STATS_UP_1
	},

	[MOVE_TRIATTACK] =
	{
		.effect = EFFECT_TRI_ATTACK,
		.power = 80,
		.type = TYPE_NORMAL,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 20,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 160,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_SUPERFANG] =
	{
		.effect = EFFECT_SUPER_FANG,
		.power = 1,
		.type = TYPE_NORMAL,
		.accuracy = 90,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_SLASH] =
	{
		.effect = EFFECT_HIGH_CRITICAL,
		.power = 70,
		.type = TYPE_NORMAL,
		.accuracy = 100,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 140,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_SUBSTITUTE] =
	{
		.effect = EFFECT_SUBSTITUTE,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = FLAG_SNATCH_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_RESET_STATS
	},

	[MOVE_STRUGGLE] =
	{
		.effect = EFFECT_RECOIL,
		.power = 50,
		.type = TYPE_MYSTERY,
		.accuracy = 0,
		.pp = 1,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_RANDOM,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED,
		.z_move_power = 1,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_SKETCH] =
	{
		.effect = EFFECT_SKETCH,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 0,
		.pp = 1,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_ALL_STATS_UP_1
	},

	[MOVE_TRIPLEKICK] =
	{
		.effect = EFFECT_TRIPLE_KICK,
		.power = 10,
		.type = TYPE_FIGHTING,
		.accuracy = 90,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_THIEF] =
	{
		.effect = EFFECT_THIEF,
		.power = 60,
		.type = TYPE_DARK,
		.accuracy = 100,
		.pp = 25,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 120,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_SPIDERWEB] =
	{
		.effect = EFFECT_MEAN_LOOK,
		.power = 0,
		.type = TYPE_BUG,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_DEF_UP_1
	},

	[MOVE_MINDREADER] =
	{
		.effect = EFFECT_LOCK_ON,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 0,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_SPATK_UP_1
	},

	[MOVE_NIGHTMARE] =
	{
		.effect = EFFECT_NIGHTMARE,
		.power = 0,
		.type = TYPE_GHOST,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_SPATK_UP_1
	},

	[MOVE_FLAMEWHEEL] =
	{
		.effect = EFFECT_BURN_HIT,
		.power = 60,
		.type = TYPE_FIRE,
		.accuracy = 100,
		.pp = 25,
		.secondaryEffectChance = 10,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 120,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_SNORE] =
	{
		.effect = EFFECT_SNORE,
		#ifdef UNBOUND
		.power = 80,
		#else
		.power = 50,
		#endif
		.type = TYPE_NORMAL,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 30,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_CURSE] =
	{
		.effect = EFFECT_CURSE,
		.power = 0,
		.type = TYPE_GHOST,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_CURSE
	},

	[MOVE_FLAIL] =
	{
		.effect = EFFECT_FLAIL,
		.power = 1,
		.type = TYPE_NORMAL,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 160,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_CONVERSION2] =
	{
		.effect = EFFECT_CONVERSION_2,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 0,
		.pp = 30,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_RECOVER_HP
	},

	[MOVE_AEROBLAST] =
	{
		.effect = EFFECT_HIGH_CRITICAL,
		.power = 100,
		.type = TYPE_FLYING,
		.accuracy = 95,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 180,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_COTTONSPORE] =
	{
		.effect = EFFECT_SPEED_DOWN_2,
		.power = 0,
		.type = TYPE_GRASS,
		.accuracy = 100,
		.pp = 40,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_BOTH,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_RESET_STATS
	},

	[MOVE_REVERSAL] =
	{
		.effect = EFFECT_FLAIL,
		.power = 1,
		.type = TYPE_FIGHTING,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 160,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_SPITE] =
	{
		.effect = EFFECT_SPITE,
		.power = 0,
		.type = TYPE_GHOST,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_RECOVER_HP
	},

	[MOVE_POWDERSNOW] =
	{
		.effect = EFFECT_FREEZE_HIT,
		.power = 40,
		.type = TYPE_ICE,
		.accuracy = 100,
		.pp = 25,
		.secondaryEffectChance = 10,
		.target = MOVE_TARGET_BOTH,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_PROTECT] =
	{
		.effect = EFFECT_PROTECT,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 4,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_RESET_STATS
	},

	[MOVE_MACHPUNCH] =
	{
		.effect = EFFECT_QUICK_ATTACK,
		.power = 40,
		.type = TYPE_FIGHTING,
		.accuracy = 100,
		.pp = 30,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 1,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_SCARYFACE] =
	{
		.effect = EFFECT_SPEED_DOWN_2,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_SPD_UP_1
	},

	[MOVE_FEINTATTACK] =
	{
		.effect = EFFECT_ALWAYS_HIT,
		.power = 60,
		.type = TYPE_DARK,
		.accuracy = 0,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 120,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_SWEETKISS] =
	{
		.effect = EFFECT_CONFUSE,
		.power = 0,
		.type = TYPE_FAIRY,
		.accuracy = 75,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_SPATK_UP_1
	},

	[MOVE_BELLYDRUM] =
	{
		.effect = EFFECT_BELLY_DRUM,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = FLAG_SNATCH_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_RECOVER_HP
	},

	[MOVE_SLUDGEBOMB] =
	{
		.effect = EFFECT_POISON_HIT,
		.power = 90,
		.type = TYPE_POISON,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 30,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 175,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_MUDSLAP] =
	{
		.effect = EFFECT_ACCURACY_DOWN_HIT,
		.power = 20,
		.type = TYPE_GROUND,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_OCTAZOOKA] =
	{
		.effect = EFFECT_ACCURACY_DOWN_HIT,
		.power = 65,
		.type = TYPE_WATER,
		.accuracy = 85,
		.pp = 10,
		.secondaryEffectChance = 50,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 120,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_SPIKES] =
	{
		.effect = EFFECT_SPIKES,
		.power = 0,
		.type = TYPE_GROUND,
		.accuracy = 0,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_OPPONENTS_FIELD,
		.priority = 0,
		.flags = FLAG_MAGIC_COAT_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_DEF_UP_1
	},

	[MOVE_ZAPCANNON] =
	{
		.effect = EFFECT_PARALYZE_HIT,
		.power = 120,
		.type = TYPE_ELECTRIC,
		.accuracy = 50,
		.pp = 5,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 190,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_FORESIGHT] =
	{
		.effect = EFFECT_FORESIGHT,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 0,
		.pp = 40,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_BOOST_CRITS
	},

	[MOVE_DESTINYBOND] =
	{
		.effect = EFFECT_DESTINY_BOND,
		.power = 0,
		.type = TYPE_GHOST,
		.accuracy = 0,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_FOLLOW_ME
	},

	[MOVE_PERISHSONG] =
	{
		.effect = EFFECT_PERISH_SONG,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 0,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_RESET_STATS
	},

	[MOVE_ICYWIND] =
	{
		.effect = EFFECT_SPEED_DOWN_HIT,
		.power = 55,
		.type = TYPE_ICE,
		.accuracy = 95,
		.pp = 15,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_BOTH,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_DETECT] =
	{
		.effect = EFFECT_PROTECT,
		.power = 0,
		.type = TYPE_FIGHTING,
		.accuracy = 0,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 4,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_EVSN_UP_1
	},

	[MOVE_BONERUSH] =
	{
		.effect = EFFECT_MULTI_HIT,
		.power = 25,
		.type = TYPE_GROUND,
		.accuracy = 90,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_LOCKON] =
	{
		.effect = EFFECT_LOCK_ON,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 0,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_SPD_UP_1
	},

	[MOVE_OUTRAGE] =
	{
		.effect = EFFECT_RAMPAGE,
		.power = 120,
		.type = TYPE_DRAGON,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_RANDOM,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 190,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_SANDSTORM] =
	{
		.effect = EFFECT_SANDSTORM,
		.power = 0,
		.type = TYPE_ROCK,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_SPD_UP_1
	},

	[MOVE_GIGADRAIN] =
	{
		.effect = EFFECT_ABSORB,
		.power = 75,
		.type = TYPE_GRASS,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_TRIAGE_AFFECTED,
		.z_move_power = 140,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_ENDURE] =
	{
		.effect = EFFECT_PROTECT,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 4,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_RESET_STATS
	},

	[MOVE_CHARM] =
	{
		.effect = EFFECT_ATTACK_DOWN_2,
		.power = 0,
		.type = TYPE_FAIRY,
		.accuracy = 100,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_DEF_UP_1
	},

	[MOVE_ROLLOUT] =
	{
		.effect = EFFECT_ROLLOUT,
		.power = 30,
		.type = TYPE_ROCK,
		.accuracy = 90,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_FALSESWIPE] =
	{
		.effect = EFFECT_FALSE_SWIPE,
		.power = 40,
		.type = TYPE_NORMAL,
		.accuracy = 100,
		.pp = 40,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_SWAGGER] =
	{
		.effect = EFFECT_SWAGGER,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 85,
		.pp = 15,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_RESET_STATS
	},

	[MOVE_MILKDRINK] =
	{
		.effect = EFFECT_RESTORE_HP,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = FLAG_SNATCH_AFFECTED | FLAG_TRIAGE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_RESET_STATS
	},

	[MOVE_SPARK] =
	{
		.effect = EFFECT_PARALYZE_HIT,
		.power = 65,
		.type = TYPE_ELECTRIC,
		.accuracy = 100,
		.pp = 20,
		.secondaryEffectChance = 30,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 120,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_FURYCUTTER] =
	{
		.effect = EFFECT_FURY_CUTTER,
		.power = 40,
		.type = TYPE_BUG,
		.accuracy = 95,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_STEELWING] =
	{
		.effect = EFFECT_DEFENSE_UP_HIT,
		.power = 70,
		.type = TYPE_STEEL,
		.accuracy = 90,
		.pp = 25,
		.secondaryEffectChance = 10,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 140,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_MEANLOOK] =
	{
		.effect = EFFECT_MEAN_LOOK,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 0,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_SPDEF_UP_1
	},

	[MOVE_ATTRACT] =
	{
		.effect = EFFECT_ATTRACT,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_RESET_STATS
	},

	[MOVE_SLEEPTALK] =
	{
		.effect = EFFECT_SLEEP_TALK,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_DEPENDS,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_BOOST_CRITS
	},

	[MOVE_HEALBELL] =
	{
		.effect = EFFECT_HEAL_BELL,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 0,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = FLAG_SNATCH_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_RECOVER_HP
	},

	[MOVE_RETURN] =
	{
		.effect = EFFECT_HIT,
		.power = 1,
		.type = TYPE_NORMAL,
		.accuracy = 100,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 160,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_PRESENT] =
	{
		.effect = EFFECT_PRESENT,
		.power = 1,
		.type = TYPE_NORMAL,
		.accuracy = 90,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_FRUSTRATION] =
	{
		.effect = EFFECT_HIT,
		.power = 1,
		.type = TYPE_NORMAL,
		.accuracy = 100,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 160,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_SAFEGUARD] =
	{
		.effect = EFFECT_SAFEGUARD,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 0,
		.pp = 25,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = FLAG_SNATCH_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_SPD_UP_1
	},

	[MOVE_PAINSPLIT] =
	{
		.effect = EFFECT_PAIN_SPLIT,
		.power = 1,
		.type = TYPE_NORMAL,
		.accuracy = 0,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_DEF_UP_1
	},

	[MOVE_SACREDFIRE] =
	{
		.effect = EFFECT_BURN_HIT,
		.power = 100,
		.type = TYPE_FIRE,
		.accuracy = 95,
		.pp = 5,
		.secondaryEffectChance = 50,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 180,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_MAGNITUDE] =
	{
		.effect = EFFECT_MAGNITUDE,
		.power = 1,
		.type = TYPE_GROUND,
		.accuracy = 100,
		.pp = 30,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_FOES_AND_ALLY,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 140,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_DYNAMICPUNCH] =
	{
		.effect = EFFECT_CONFUSE_HIT,
		.power = 100,
		.type = TYPE_FIGHTING,
		.accuracy = 50,
		.pp = 5,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 180,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_MEGAHORN] =
	{
		.effect = EFFECT_HIT,
		.power = 120,
		.type = TYPE_BUG,
		.accuracy = 85,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 190,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_DRAGONBREATH] =
	{
		.effect = EFFECT_PARALYZE_HIT,
		.power = 60,
		.type = TYPE_DRAGON,
		.accuracy = 100,
		.pp = 20,
		.secondaryEffectChance = 30,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 120,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_BATONPASS] =
	{
		.effect = EFFECT_BATON_PASS,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 0,
		.pp = 40,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_RESET_STATS
	},

	[MOVE_ENCORE] =
	{
		.effect = EFFECT_ENCORE,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 100,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_SPD_UP_1
	},

	[MOVE_PURSUIT] =
	{
		.effect = EFFECT_PURSUIT,
		.power = 40,
		.type = TYPE_DARK,
		.accuracy = 100,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_RAPIDSPIN] =
	{
		.effect = EFFECT_RAPID_SPIN,
		.power = 50,
		.type = TYPE_NORMAL,
		.accuracy = 100,
		.pp = 40,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_SWEETSCENT] =
	{
		.effect = EFFECT_EVASION_DOWN_2,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 100,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_BOTH,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_ACC_UP_1
	},

	[MOVE_IRONTAIL] =
	{
		.effect = EFFECT_DEFENSE_DOWN_HIT,
		.power = 100,
		.type = TYPE_STEEL,
		.accuracy = 75,
		.pp = 15,
		.secondaryEffectChance = 30,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 180,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_METALCLAW] =
	{
		.effect = EFFECT_ATTACK_UP_HIT,
		.power = 50,
		.type = TYPE_STEEL,
		.accuracy = 95,
		.pp = 35,
		.secondaryEffectChance = 10,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_VITALTHROW] =
	{
		.effect = EFFECT_ALWAYS_HIT,
		.power = 70,
		.type = TYPE_FIGHTING,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 255,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 140,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_MORNINGSUN] =
	{
		.effect = EFFECT_MORNING_SUN,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 0,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = FLAG_SNATCH_AFFECTED | FLAG_TRIAGE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_RESET_STATS
	},

	[MOVE_SYNTHESIS] =
	{
		.effect = EFFECT_MORNING_SUN,
		.power = 0,
		.type = TYPE_GRASS,
		.accuracy = 0,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = FLAG_SNATCH_AFFECTED | FLAG_TRIAGE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_RESET_STATS
	},

	[MOVE_MOONLIGHT] =
	{
		.effect = EFFECT_MORNING_SUN,
		.power = 0,
		.type = TYPE_FAIRY,
		.accuracy = 0,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = FLAG_SNATCH_AFFECTED | FLAG_TRIAGE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_RESET_STATS
	},

	[MOVE_HIDDENPOWER] =
	{
		.effect = EFFECT_HIT,
		.power = 60,
		.type = TYPE_NORMAL,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 120,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_CROSSCHOP] =
	{
		.effect = EFFECT_HIGH_CRITICAL,
		.power = 100,
		.type = TYPE_FIGHTING,
		.accuracy = 80,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 180,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_TWISTER] =
	{
		.effect = EFFECT_TWISTER,
		.power = 40,
		.type = TYPE_DRAGON,
		.accuracy = 100,
		.pp = 20,
		.secondaryEffectChance = 20,
		.target = MOVE_TARGET_BOTH,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_RAINDANCE] =
	{
		.effect = EFFECT_RAIN_DANCE,
		.power = 0,
		.type = TYPE_WATER,
		.accuracy = 0,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_SPD_UP_1
	},

	[MOVE_SUNNYDAY] =
	{
		.effect = EFFECT_SUNNY_DAY,
		.power = 0,
		.type = TYPE_FIRE,
		.accuracy = 0,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_SPD_UP_1
	},

	[MOVE_CRUNCH] =
	{
		.effect = EFFECT_DEFENSE_DOWN_HIT,
		.power = 80,
		.type = TYPE_DARK,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 20,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 160,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_MIRRORCOAT] =
	{
		.effect = EFFECT_MIRROR_COAT,
		.power = 1,
		.type = TYPE_PSYCHIC,
		.accuracy = 100,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_DEPENDS,
		.priority = 251,
		.flags = FLAG_PROTECT_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_PSYCHUP] =
	{
		.effect = EFFECT_PSYCH_UP,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_RECOVER_HP
	},

	[MOVE_EXTREMESPEED] =
	{
		.effect = EFFECT_QUICK_ATTACK,
		.power = 80,
		.type = TYPE_NORMAL,
		.accuracy = 100,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 2,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 160,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_ANCIENTPOWER] =
	{
		.effect = EFFECT_ALL_STATS_UP_HIT,
		.power = 60,
		.type = TYPE_ROCK,
		.accuracy = 100,
		.pp = 5,
		.secondaryEffectChance = 10,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 120,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_SHADOWBALL] =
	{
		.effect = EFFECT_SPECIAL_DEFENSE_DOWN_HIT,
		.power = 80,
		.type = TYPE_GHOST,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 20,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 160,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_FUTURESIGHT] =
	{
		.effect = EFFECT_FUTURE_SIGHT,
		.power = 120,
		.type = TYPE_PSYCHIC,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.z_move_power = 190,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_ROCKSMASH] =
	{
		.effect = EFFECT_DEFENSE_DOWN_HIT,
		.power = 40,
		.type = TYPE_FIGHTING,
		.accuracy = 100,
		.pp = 15,
		#ifdef UNBOUND
		.secondaryEffectChance = 100,
		#else
		.secondaryEffectChance = 50,
		#endif
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_WHIRLPOOL] =
	{
		.effect = EFFECT_TRAP,
		.power = 35,
		.type = TYPE_WATER,
		.accuracy = 85,
		.pp = 15,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_BEATUP] =
	{
		.effect = EFFECT_BEAT_UP,
		.power = 1,
		.type = TYPE_DARK,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_FAKEOUT] =
	{
		.effect = EFFECT_FAKE_OUT,
		.power = 40,
		.type = TYPE_NORMAL,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 3,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_UPROAR] =
	{
		.effect = EFFECT_UPROAR,
		.power = 90,
		.type = TYPE_NORMAL,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_RANDOM,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 175,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_STOCKPILE] =
	{
		.effect = EFFECT_STOCKPILE,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 0,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = FLAG_SNATCH_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_RECOVER_HP
	},

	[MOVE_SPITUP] =
	{
		.effect = EFFECT_SPIT_UP,
		.power = 1,
		.type = TYPE_NORMAL,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_SWALLOW] =
	{
		.effect = EFFECT_SWALLOW,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = FLAG_SNATCH_AFFECTED | FLAG_TRIAGE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_RESET_STATS
	},

	[MOVE_HEATWAVE] =
	{
		.effect = EFFECT_BURN_HIT,
		#ifdef GEN_6_POWER_NERFS
		.power = 95,
		#else
		.power = 100,
		#endif
		.type = TYPE_FIRE,
		.accuracy = 90,
		.pp = 10,
		.secondaryEffectChance = 10,
		.target = MOVE_TARGET_BOTH,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 180,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_HAIL] =
	{
		.effect = EFFECT_HAIL,
		.power = 0,
		.type = TYPE_ICE,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_SPD_UP_1
	},

	[MOVE_TORMENT] =
	{
		.effect = EFFECT_TORMENT,
		.power = 0,
		.type = TYPE_DARK,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_DEF_UP_1
	},

	[MOVE_FLATTER] =
	{
		.effect = EFFECT_FLATTER,
		.power = 0,
		.type = TYPE_DARK,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_SPDEF_UP_1
	},

	[MOVE_WILLOWISP] =
	{
		.effect = EFFECT_WILL_O_WISP,
		.power = 0,
		.type = TYPE_FIRE,
		.accuracy = 85,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_ATK_UP_1
	},

	[MOVE_MEMENTO] =
	{
		.effect = EFFECT_MEMENTO,
		.power = 0,
		.type = TYPE_DARK,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_RESTORE_REPLACEMENT_HP
	},

	[MOVE_FACADE] =
	{
		.effect = EFFECT_HIT,
		.power = 70,
		.type = TYPE_NORMAL,
		.accuracy = 100,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 140,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_FOCUSPUNCH] =
	{
		.effect = EFFECT_FOCUS_PUNCH,
		.power = 150,
		.type = TYPE_FIGHTING,
		.accuracy = 100,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 253,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED,
		.z_move_power = 200,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_SMELLINGSALTS] =
	{
		.effect = EFFECT_SMELLINGSALT,
		.power = 70,
		.type = TYPE_NORMAL,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 140,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_FOLLOWME] =
	{
		.effect = EFFECT_FOLLOW_ME,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 0,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 2,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_RESET_STATS
	},

	[MOVE_NATUREPOWER] =
	{
		.effect = EFFECT_NATURE_POWER,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 0,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = 0
	},

	[MOVE_CHARGE] =
	{
		.effect = EFFECT_CHARGE,
		.power = 0,
		.type = TYPE_ELECTRIC,
		.accuracy = 0,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = FLAG_SNATCH_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_SPDEF_UP_1
	},

	[MOVE_TAUNT] =
	{
		.effect = EFFECT_TAUNT,
		.power = 0,
		.type = TYPE_DARK,
		.accuracy = 100,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_ATK_UP_1
	},

	[MOVE_HELPINGHAND] =
	{
		.effect = EFFECT_HELPING_HAND,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 0,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 5,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_RESET_STATS
	},

	[MOVE_TRICK] =
	{
		.effect = EFFECT_TRICK,
		.power = 0,
		.type = TYPE_PSYCHIC,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_SPD_UP_2
	},

	[MOVE_ROLEPLAY] =
	{
		.effect = EFFECT_ROLE_PLAY,
		.power = 0,
		.type = TYPE_PSYCHIC,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_SPD_UP_1
	},

	[MOVE_WISH] =
	{
		.effect = EFFECT_WISH,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = FLAG_SNATCH_AFFECTED | FLAG_TRIAGE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_SPDEF_UP_1
	},

	[MOVE_ASSIST] =
	{
		.effect = EFFECT_ASSIST,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 0,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_DEPENDS,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = 0
	},

	[MOVE_INGRAIN] =
	{
		.effect = EFFECT_INGRAIN,
		.power = 0,
		.type = TYPE_GRASS,
		.accuracy = 0,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = FLAG_SNATCH_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_SPDEF_UP_1
	},

	[MOVE_SUPERPOWER] =
	{
		.effect = EFFECT_SUPERPOWER,
		.power = 120,
		.type = TYPE_FIGHTING,
		.accuracy = 100,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 190,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_MAGICCOAT] =
	{
		.effect = EFFECT_MAGIC_COAT,
		.power = 0,
		.type = TYPE_PSYCHIC,
		.accuracy = 0,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_DEPENDS,
		.priority = 4,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_SPDEF_UP_2
	},

	[MOVE_RECYCLE] =
	{
		.effect = EFFECT_RECYCLE,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = FLAG_SNATCH_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_SPD_UP_2
	},

	[MOVE_REVENGE] =
	{
		.effect = EFFECT_HIT,
		.power = 60,
		.type = TYPE_FIGHTING,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 252,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 120,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_BRICKBREAK] =
	{
		.effect = EFFECT_BRICK_BREAK,
		.power = 75,
		.type = TYPE_FIGHTING,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 140,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_YAWN] =
	{
		.effect = EFFECT_YAWN,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_SPD_UP_1
	},

	[MOVE_KNOCKOFF] =
	{
		.effect = EFFECT_KNOCK_OFF,
		.power = 65,
		.type = TYPE_DARK,
		.accuracy = 100,
		.pp = 20,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 120,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_ENDEAVOR] =
	{
		.effect = EFFECT_ENDEAVOR,
		.power = 1,
		.type = TYPE_NORMAL,
		.accuracy = 100,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 160,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_ERUPTION] =
	{
		.effect = EFFECT_HIT,
		.power = 150,
		.type = TYPE_FIRE,
		.accuracy = 100,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_BOTH,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 200,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_SKILLSWAP] =
	{
		.effect = EFFECT_SKILL_SWAP,
		.power = 0,
		.type = TYPE_PSYCHIC,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_SPD_UP_1
	},

	[MOVE_IMPRISON] =
	{
		.effect = EFFECT_IMPRISON,
		.power = 0,
		.type = TYPE_PSYCHIC,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = FLAG_SNATCH_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_SPDEF_UP_2
	},

	[MOVE_REFRESH] =
	{
		.effect = EFFECT_REFRESH,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 0,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = FLAG_SNATCH_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_RECOVER_HP
	},

	[MOVE_GRUDGE] =
	{
		.effect = EFFECT_GRUDGE,
		.power = 0,
		.type = TYPE_GHOST,
		.accuracy = 0,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_FOLLOW_ME
	},

	[MOVE_SNATCH] =
	{
		.effect = EFFECT_SNATCH,
		.power = 0,
		.type = TYPE_DARK,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_DEPENDS,
		.priority = 4,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_SPD_UP_2
	},

	[MOVE_SECRETPOWER] =
	{
		.effect = EFFECT_SECRET_POWER,
		.power = 70,
		.type = TYPE_NORMAL,
		.accuracy = 100,
		.pp = 20,
		.secondaryEffectChance = 30,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 140,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_DIVE] =
	{
		.effect = EFFECT_SEMI_INVULNERABLE,
		#ifdef UNBOUND
		.power = 100,
		#else
		.power = 80,
		#endif
		.type = TYPE_WATER,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 160,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_ARMTHRUST] =
	{
		.effect = EFFECT_MULTI_HIT,
		.power = 15,
		.type = TYPE_FIGHTING,
		.accuracy = 100,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_CAMOUFLAGE] =
	{
		.effect = EFFECT_CAMOUFLAGE,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 0,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = FLAG_SNATCH_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_EVSN_UP_1
	},

	[MOVE_TAILGLOW] =
	{
		.effect = EFFECT_SPECIAL_ATTACK_UP_2,
		.power = 0,
		.type = TYPE_BUG,
		.accuracy = 0,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = FLAG_SNATCH_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_RESET_STATS
	},

	[MOVE_LUSTERPURGE] =
	{
		.effect = EFFECT_SPECIAL_DEFENSE_DOWN_HIT,
		.power = 70,
		.type = TYPE_PSYCHIC,
		.accuracy = 100,
		.pp = 5,
		.secondaryEffectChance = 50,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 160,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_MISTBALL] =
	{
		.effect = EFFECT_SPECIAL_ATTACK_DOWN_HIT,
		.power = 70,
		.type = TYPE_PSYCHIC,
		.accuracy = 100,
		.pp = 5,
		.secondaryEffectChance = 50,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 140,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_FEATHERDANCE] =
	{
		.effect = EFFECT_ATTACK_DOWN_2,
		.power = 0,
		.type = TYPE_FLYING,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_DEF_UP_1
	},

	[MOVE_TEETERDANCE] =
	{
		.effect = EFFECT_CONFUSE,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 100,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_FOES_AND_ALLY,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_SPATK_UP_1
	},

	[MOVE_BLAZEKICK] =
	{
		.effect = EFFECT_BURN_HIT,
		.power = 85,
		.type = TYPE_FIRE,
		.accuracy = 90,
		.pp = 10,
		.secondaryEffectChance = 10,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 160,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_MUDSPORT] =
	{
		.effect = EFFECT_MUD_SPORT,
		.power = 0,
		.type = TYPE_GROUND,
		.accuracy = 0,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_SPDEF_UP_1
	},

	[MOVE_ICEBALL] =
	{
		.effect = EFFECT_ROLLOUT,
		.power = 30,
		.type = TYPE_ICE,
		.accuracy = 90,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_NEEDLEARM] =
	{
		.effect = EFFECT_FLINCH_HIT,
		.power = 60,
		.type = TYPE_GRASS,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 30,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 120,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_SLACKOFF] =
	{
		.effect = EFFECT_RESTORE_HP,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = FLAG_SNATCH_AFFECTED | FLAG_TRIAGE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_RESET_STATS
	},

	[MOVE_HYPERVOICE] =
	{
		.effect = EFFECT_HIT,
		.power = 90,
		.type = TYPE_NORMAL,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_BOTH,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 175,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_POISONFANG] =
	{
		.effect = EFFECT_BAD_POISON_HIT,
		.power = 50,
		.type = TYPE_POISON,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 50,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_CRUSHCLAW] =
	{
		.effect = EFFECT_DEFENSE_DOWN_HIT,
		.power = 75,
		.type = TYPE_NORMAL,
		.accuracy = 95,
		.pp = 10,
		.secondaryEffectChance = 50,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 140,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_BLASTBURN] =
	{
		.effect = EFFECT_RECHARGE,
		.power = 150,
		.type = TYPE_FIRE,
		.accuracy = 90,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 200,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_HYDROCANNON] =
	{
		.effect = EFFECT_RECHARGE,
		.power = 150,
		.type = TYPE_WATER,
		.accuracy = 90,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 200,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_METEORMASH] =
	{
		.effect = EFFECT_ATTACK_UP_HIT,
		#ifdef GEN_6_POWER_NERFS
		.power = 90,
		#else
		.power = 100,
		#endif
		.type = TYPE_STEEL,
		.accuracy = 90,
		.pp = 10,
		.secondaryEffectChance = 20,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 180,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_ASTONISH] =
	{
		.effect = EFFECT_FLINCH_HIT,
		.power = 30,
		.type = TYPE_GHOST,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 30,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_WEATHERBALL] =
	{
		.effect = EFFECT_HIT,
		.power = 50,
		.type = TYPE_NORMAL,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 160,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_AROMATHERAPY] =
	{
		.effect = EFFECT_HEAL_BELL,
		.power = 0,
		.type = TYPE_GRASS,
		.accuracy = 0,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = FLAG_SNATCH_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_RECOVER_HP
	},

	[MOVE_FAKETEARS] =
	{
		.effect = EFFECT_SPECIAL_DEFENSE_DOWN_2,
		.power = 0,
		.type = TYPE_DARK,
		.accuracy = 100,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_SPATK_UP_1
	},

	[MOVE_AIRCUTTER] =
	{
		.effect = EFFECT_HIGH_CRITICAL,
		.power = 60,
		.type = TYPE_FLYING,
		.accuracy = 95,
		.pp = 25,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_BOTH,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 120,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_OVERHEAT] =
	{
		.effect = EFFECT_OVERHEAT,
		#ifdef GEN_6_POWER_NERFS
		.power = 130,
		#else
		.power = 140,
		#endif
		.type = TYPE_FIRE,
		.accuracy = 90,
		.pp = 5,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 200,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_ODORSLEUTH] =
	{
		.effect = EFFECT_FORESIGHT,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 0,
		.pp = 40,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_ATK_UP_1
	},

	[MOVE_ROCKTOMB] =
	{
		.effect = EFFECT_SPEED_DOWN_HIT,
		.power = 60,
		.type = TYPE_ROCK,
		.accuracy = 95,
		.pp = 15,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 120,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_SILVERWIND] =
	{
		.effect = EFFECT_ALL_STATS_UP_HIT,
		.power = 60,
		.type = TYPE_BUG,
		.accuracy = 100,
		.pp = 5,
		.secondaryEffectChance = 10,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 120,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_METALSOUND] =
	{
		.effect = EFFECT_SPECIAL_DEFENSE_DOWN_2,
		.power = 0,
		.type = TYPE_STEEL,
		.accuracy = 85,
		.pp = 40,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_SPATK_UP_1
	},

	[MOVE_GRASSWHISTLE] =
	{
		.effect = EFFECT_SLEEP,
		.power = 0,
		.type = TYPE_GRASS,
		.accuracy = 55,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_SPD_UP_1
	},

	[MOVE_TICKLE] =
	{
		.effect = EFFECT_TICKLE,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 100,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_DEF_UP_1
	},

	[MOVE_COSMICPOWER] =
	{
		.effect = EFFECT_COSMIC_POWER,
		.power = 0,
		.type = TYPE_PSYCHIC,
		.accuracy = 0,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = FLAG_SNATCH_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_SPDEF_UP_1
	},

	[MOVE_WATERSPOUT] =
	{
		.effect = EFFECT_HIT,
		.power = 150,
		.type = TYPE_WATER,
		.accuracy = 100,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_BOTH,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 200,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_SIGNALBEAM] =
	{
		.effect = EFFECT_CONFUSE_HIT,
		.power = 75,
		.type = TYPE_BUG,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 10,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 140,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_SHADOWPUNCH] =
	{
		.effect = EFFECT_ALWAYS_HIT,
		.power = 60,
		.type = TYPE_GHOST,
		.accuracy = 0,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 140,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_EXTRASENSORY] =
	{
		.effect = EFFECT_FLINCH_HIT,
		.power = 80,
		.type = TYPE_PSYCHIC,
		.accuracy = 100,
		.pp = 20,
		.secondaryEffectChance = 10,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 160,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_SKYUPPERCUT] =
	{
		.effect = EFFECT_HIT,
		.power = 85,
		.type = TYPE_FIGHTING,
		.accuracy = 90,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 160,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_SANDTOMB] =
	{
		.effect = EFFECT_TRAP,
		.power = 35,
		.type = TYPE_GROUND,
		.accuracy = 85,
		.pp = 15,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_SHEERCOLD] =
	{
		.effect = EFFECT_0HKO,
		.power = 1,
		.type = TYPE_ICE,
		.accuracy = 30,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 180,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_MUDDYWATER] =
	{
		.effect = EFFECT_ACCURACY_DOWN_HIT,
		#ifdef GEN_6_POWER_NERFS
		.power = 90,
		#else
		.power = 95,
		#endif
		.type = TYPE_WATER,
		.accuracy = 85,
		.pp = 10,
		.secondaryEffectChance = 30,
		.target = MOVE_TARGET_BOTH,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 175,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_BULLETSEED] =
	{
		.effect = EFFECT_MULTI_HIT,
		.power = 25,
		.type = TYPE_GRASS,
		.accuracy = 100,
		.pp = 30,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_AERIALACE] =
	{
		.effect = EFFECT_ALWAYS_HIT,
		.power = 60,
		.type = TYPE_FLYING,
		.accuracy = 0,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 120,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_ICICLESPEAR] =
	{
		.effect = EFFECT_MULTI_HIT,
		.power = 25,
		.type = TYPE_ICE,
		.accuracy = 100,
		.pp = 30,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_IRONDEFENSE] =
	{
		.effect = EFFECT_DEFENSE_UP_2,
		.power = 0,
		.type = TYPE_STEEL,
		.accuracy = 0,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = FLAG_SNATCH_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_RESET_STATS
	},

	[MOVE_BLOCK] =
	{
		.effect = EFFECT_MEAN_LOOK,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 0,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_DEF_UP_1
	},

	[MOVE_HOWL] =
	{
		.effect = EFFECT_ATTACK_UP,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 0,
		.pp = 40,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = FLAG_SNATCH_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_ATK_UP_1
	},

	[MOVE_DRAGONCLAW] =
	{
		.effect = EFFECT_HIT,
		.power = 80,
		.type = TYPE_DRAGON,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 160,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_FRENZYPLANT] =
	{
		.effect = EFFECT_RECHARGE,
		.power = 150,
		.type = TYPE_GRASS,
		.accuracy = 90,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 200,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_BULKUP] =
	{
		.effect = EFFECT_BULK_UP,
		.power = 0,
		.type = TYPE_FIGHTING,
		.accuracy = 0,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = FLAG_SNATCH_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_ATK_UP_1
	},

	[MOVE_BOUNCE] =
	{
		.effect = EFFECT_SEMI_INVULNERABLE,
		.power = 85,
		.type = TYPE_FLYING,
		.accuracy = 85,
		.pp = 5,
		.secondaryEffectChance = 30,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 160,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_MUDSHOT] =
	{
		.effect = EFFECT_SPEED_DOWN_HIT,
		.power = 55,
		.type = TYPE_GROUND,
		.accuracy = 95,
		.pp = 15,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_POISONTAIL] =
	{
		.effect = EFFECT_POISON_HIT,
		.power = 50,
		.type = TYPE_POISON,
		.accuracy = 100,
		.pp = 25,
		.secondaryEffectChance = 10,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_COVET] =
	{
		.effect = EFFECT_THIEF,
		.power = 60,
		.type = TYPE_NORMAL,
		.accuracy = 100,
		.pp = 25,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 120,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_VOLTTACKLE] =
	{
		.effect = EFFECT_PARALYZE_HIT,
		.power = 120,
		.type = TYPE_ELECTRIC,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 10,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 190,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_MAGICALLEAF] =
	{
		.effect = EFFECT_ALWAYS_HIT,
		.power = 60,
		.type = TYPE_GRASS,
		.accuracy = 0,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 120,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_WATERSPORT] =
	{
		.effect = EFFECT_WATER_SPORT,
		.power = 0,
		.type = TYPE_WATER,
		.accuracy = 0,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_SPDEF_UP_1
	},

	[MOVE_CALMMIND] =
	{
		.effect = EFFECT_CALM_MIND,
		.power = 0,
		.type = TYPE_PSYCHIC,
		.accuracy = 0,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = FLAG_SNATCH_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_RESET_STATS
	},

	[MOVE_LEAFBLADE] =
	{
		.effect = EFFECT_HIGH_CRITICAL,
		.power = 90,
		.type = TYPE_GRASS,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 175,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_DRAGONDANCE] =
	{
		.effect = EFFECT_DRAGON_DANCE,
		.power = 0,
		.type = TYPE_DRAGON,
		.accuracy = 0,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = FLAG_SNATCH_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_RESET_STATS
	},

	[MOVE_ROCKBLAST] =
	{
		.effect = EFFECT_MULTI_HIT,
		.power = 25,
		.type = TYPE_ROCK,
		.accuracy = 90,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_SHOCKWAVE] =
	{
		.effect = EFFECT_ALWAYS_HIT,
		.power = 60,
		.type = TYPE_ELECTRIC,
		.accuracy = 0,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 120,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_WATERPULSE] =
	{
		.effect = EFFECT_CONFUSE_HIT,
		.power = 60,
		.type = TYPE_WATER,
		.accuracy = 100,
		.pp = 20,
		.secondaryEffectChance = 20,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 120,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_DOOMDESIRE] =
	{
		.effect = EFFECT_FUTURE_SIGHT,
		.power = 140,
		.type = TYPE_STEEL,
		.accuracy = 100,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.z_move_power = 200,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_PSYCHOBOOST] =
	{
		.effect = EFFECT_OVERHEAT,
		.power = 140,
		.type = TYPE_PSYCHIC,
		.accuracy = 90,
		.pp = 5,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 200,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_LEECHFANG] =
	{
		.effect = EFFECT_ABSORB,
		.power = 80,
		.type = TYPE_BUG,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_TRIAGE_AFFECTED,
		.z_move_power = 160,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_AIRSLASH] =
	{
		.effect = EFFECT_FLINCH_HIT,
		.power = 75,
		.type = TYPE_FLYING,
		.accuracy = 95,
		.pp = 15,
		.secondaryEffectChance = 30,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 140,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_AQUAJET] =
	{
		.effect = EFFECT_QUICK_ATTACK,
		.power = 40,
		.type = TYPE_WATER,
		.accuracy = 100,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 1,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_AQUATAIL] =
	{
		.effect = EFFECT_HIT,
		.power = 90,
		.type = TYPE_WATER,
		.accuracy = 90,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 175,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_AURASPHERE] =
	{
		.effect = EFFECT_ALWAYS_HIT,
		#ifdef GEN_6_POWER_NERFS
		.power = 80,
		#else
		.power = 90,
		#endif
		.type = TYPE_FIGHTING,
		.accuracy = 0,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 175,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_AVALANCHE] =
	{
		.effect = EFFECT_HIT,
		.power = 60,
		.type = TYPE_ICE,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 252,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 120,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_BRAVEBIRD] =
	{
		.effect = EFFECT_RECOIL,
		.power = 120,
		.type = TYPE_FLYING,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 190,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_BUGBUZZ] =
	{
		.effect = EFFECT_SPECIAL_DEFENSE_DOWN_HIT,
		.power = 90,
		.type = TYPE_BUG,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 10,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 175,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_BULLETPUNCH] =
	{
		.effect = EFFECT_QUICK_ATTACK,
		.power = 40,
		.type = TYPE_STEEL,
		.accuracy = 100,
		.pp = 30,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 1,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_CHARGEBEAM] =
	{
		.effect = EFFECT_SPECIAL_ATTACK_UP_HIT,
		.power = 50,
		.type = TYPE_ELECTRIC,
		.accuracy = 90,
		.pp = 10,
		.secondaryEffectChance = 70,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_CLOSECOMBAT] =
	{
		.effect = EFFECT_SUPERPOWER,
		.power = 120,
		.type = TYPE_FIGHTING,
		.accuracy = 100,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 190,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_CROSSPOISON] =
	{
		.effect = EFFECT_POISON_HIT,
		.power = 70,
		.type = TYPE_POISON,
		.accuracy = 100,
		.pp = 20,
		.secondaryEffectChance = 10,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 140,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_DARKPULSE] =
	{
		.effect = EFFECT_FLINCH_HIT,
		.power = 80,
		.type = TYPE_DARK,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 20,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 160,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_DISCHARGE] =
	{
		.effect = EFFECT_PARALYZE_HIT,
		.power = 80,
		.type = TYPE_ELECTRIC,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 30,
		.target = MOVE_TARGET_FOES_AND_ALLY,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 160,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_DRACOMETEOR] =
	{
		.effect = EFFECT_OVERHEAT,
		#ifdef GEN_6_POWER_NERFS
		.power = 130,
		#else
		.power = 140,
		#endif
		.type = TYPE_DRAGON,
		.accuracy = 90,
		.pp = 5,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 200,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_DRAGONPULSE] =
	{
		.effect = EFFECT_HIT,
		#ifdef GEN_6_POWER_NERFS
		.power = 85,
		#else
		.power = 90,
		#endif
		.type = TYPE_DRAGON,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 175,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_DRAINPUNCH] =
	{
		.effect = EFFECT_ABSORB,
		.power = 75,
		.type = TYPE_FIGHTING,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_TRIAGE_AFFECTED,
		.z_move_power = 140,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_EARTHPOWER] =
	{
		.effect = EFFECT_SPECIAL_DEFENSE_DOWN_HIT,
		.power = 90,
		.type = TYPE_GROUND,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 10,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 175,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_ENERGYBALL] =
	{
		.effect = EFFECT_SPECIAL_DEFENSE_DOWN_HIT,
		.power = 90,
		.type = TYPE_GRASS,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 10,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 175,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_FOCUSBLAST] =
	{
		.effect = EFFECT_SPECIAL_DEFENSE_DOWN_HIT,
		.power = 120,
		.type = TYPE_FIGHTING,
		.accuracy = 70,
		.pp = 5,
		.secondaryEffectChance = 10,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 190,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_FORCEPALM] =
	{
		.effect = EFFECT_PARALYZE_HIT,
		.power = 60,
		.type = TYPE_FIGHTING,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 30,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 120,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_GIGAIMPACT] =
	{
		.effect = EFFECT_RECHARGE,
		.power = 150,
		.type = TYPE_NORMAL,
		.accuracy = 90,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 200,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_GRASSKNOT] =
	{
		.effect = EFFECT_HIT,
		.power = 1,
		.type = TYPE_GRASS,
		.accuracy = 100,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 160,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_GUNKSHOT] =
	{
		.effect = EFFECT_POISON_HIT,
		.power = 120,
		.type = TYPE_POISON,
		.accuracy = 80,
		.pp = 5,
		.secondaryEffectChance = 30,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 190,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_HAMMERARM] =
	{
		.effect = EFFECT_SUPERPOWER,
		.power = 100,
		.type = TYPE_FIGHTING,
		.accuracy = 90,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 180,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_HEADSMASH] =
	{
		.effect = EFFECT_RECOIL,
		.power = 150,
		.type = TYPE_ROCK,
		.accuracy = 80,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 200,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_ICESHARD] =
	{
		.effect = EFFECT_QUICK_ATTACK,
		.power = 40,
		.type = TYPE_ICE,
		.accuracy = 100,
		.pp = 30,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 1,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_IRONHEAD] =
	{
		.effect = EFFECT_FLINCH_HIT,
		.power = 80,
		.type = TYPE_STEEL,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 30,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 160,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_LAVAPLUME] =
	{
		.effect = EFFECT_BURN_HIT,
		.power = 80,
		.type = TYPE_FIRE,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 30,
		.target = MOVE_TARGET_FOES_AND_ALLY,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 160,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_MAGNETBOMB] =
	{
		.effect = EFFECT_ALWAYS_HIT,
		.power = 60,
		.type = TYPE_STEEL,
		.accuracy = 0,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 120,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_MUDBOMB] =
	{
		.effect = EFFECT_ACCURACY_DOWN_HIT,
		.power = 65,
		.type = TYPE_GROUND,
		.accuracy = 85,
		.pp = 10,
		.secondaryEffectChance = 30,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 120,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_NASTYPLOT] =
	{
		.effect = EFFECT_SPECIAL_ATTACK_UP_2,
		.power = 0,
		.type = TYPE_DARK,
		.accuracy = 0,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = FLAG_SNATCH_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_RESET_STATS
	},

	[MOVE_NIGHTSLASH] =
	{
		.effect = EFFECT_HIGH_CRITICAL,
		.power = 70,
		.type = TYPE_DARK,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 140,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_OMINOUSWIND] =
	{
		.effect = EFFECT_ALL_STATS_UP_HIT,
		.power = 60,
		.type = TYPE_GHOST,
		.accuracy = 100,
		.pp = 5,
		.secondaryEffectChance = 10,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 120,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_POWERGEM] =
	{
		.effect = EFFECT_HIT,
		.power = 80,
		.type = TYPE_ROCK,
		.accuracy = 100,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 160,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_POWERWHIP] =
	{
		.effect = EFFECT_HIT,
		.power = 120,
		.type = TYPE_GRASS,
		.accuracy = 85,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 190,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_PSYCHOCUT] =
	{
		.effect = EFFECT_HIGH_CRITICAL,
		.power = 70,
		.type = TYPE_PSYCHIC,
		.accuracy = 100,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 140,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_ROCKCLIMB] =
	{
		.effect = EFFECT_CONFUSE_HIT,
		.power = 90,
		.type = TYPE_NORMAL,
		.accuracy = 85,
		.pp = 20,
		.secondaryEffectChance = 20,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 175,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_ROCKPOLISH] =
	{
		.effect = EFFECT_SPEED_UP_2,
		.power = 0,
		.type = TYPE_ROCK,
		.accuracy = 0,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = FLAG_SNATCH_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_RESET_STATS
	},

	[MOVE_ROCKWRECKER] =
	{
		.effect = EFFECT_RECHARGE,
		.power = 150,
		.type = TYPE_ROCK,
		.accuracy = 90,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 200,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_ROOST] =
	{
		.effect = EFFECT_RESTORE_HP,
		.power = 0,
		.type = TYPE_FLYING,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = FLAG_SNATCH_AFFECTED | FLAG_TRIAGE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_RESET_STATS
	},

	[MOVE_SEEDBOMB] =
	{
		.effect = EFFECT_HIT,
		.power = 80,
		.type = TYPE_GRASS,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 160,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_SHADOWCLAW] =
	{
		.effect = EFFECT_HIGH_CRITICAL,
		.power = 70,
		.type = TYPE_GHOST,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 140,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_SHADOWSNEAK] =
	{
		.effect = EFFECT_QUICK_ATTACK,
		.power = 40,
		.type = TYPE_GHOST,
		.accuracy = 100,
		.pp = 30,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 1,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_VACUUMWAVE] =
	{
		.effect = EFFECT_QUICK_ATTACK,
		.power = 40,
		.type = TYPE_FIGHTING,
		.accuracy = 100,
		.pp = 30,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 1,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_XSCISSOR] =
	{
		.effect = EFFECT_HIT,
		.power = 80,
		.type = TYPE_BUG,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 160,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_ZENHEADBUTT] =
	{
		.effect = EFFECT_FLINCH_HIT,
		.power = 80,
		.type = TYPE_PSYCHIC,
		.accuracy = 90,
		.pp = 15,
		.secondaryEffectChance = 20,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 160,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_SWITCHEROO] =
	{
		.effect = EFFECT_TRICK,
		.power = 0,
		.type = TYPE_DARK,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_SPD_UP_2
	},

	[MOVE_DRILLRUN] =
	{
		.effect = EFFECT_HIGH_CRITICAL,
		.power = 80,
		.type = TYPE_GROUND,
		.accuracy = 95,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 160,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_BULLDOZE] =
	{
		.effect = EFFECT_SPEED_DOWN_HIT,
		.power = 60,
		.type = TYPE_GROUND,
		.accuracy = 100,
		.pp = 20,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_FOES_AND_ALLY,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 120,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_ELECTROWEB] =
	{
		.effect = EFFECT_SPEED_DOWN_HIT,
		.power = 55,
		.type = TYPE_ELECTRIC,
		.accuracy = 95,
		.pp = 15,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_BOTH,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_FLAMECHARGE] =
	{
		.effect = EFFECT_SPEED_UP_1_HIT,
		.power = 50,
		.type = TYPE_FIRE,
		.accuracy = 100,
		.pp = 20,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_FROSTBREATH] =
	{
		.effect = EFFECT_HIT,
		.power = 60,
		.type = TYPE_ICE,
		.accuracy = 90,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 120,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_HEARTSTAMP] =
	{
		.effect = EFFECT_FLINCH_HIT,
		.power = 60,
		.type = TYPE_PSYCHIC,
		.accuracy = 100,
		.pp = 25,
		.secondaryEffectChance = 30,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 120,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_HONECLAWS] =
	{
		.effect = EFFECT_ATK_ACC_UP,
		.power = 0,
		.type = TYPE_DARK,
		.accuracy = 0,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = FLAG_SNATCH_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_ATK_UP_1
	},

	[MOVE_HORNLEECH] =
	{
		.effect = EFFECT_ABSORB,
		.power = 75,
		.type = TYPE_GRASS,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_TRIAGE_AFFECTED,
		.z_move_power = 140,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_COIL] =
	{
		.effect = EFFECT_BULK_UP,
		.power = 0,
		.type = TYPE_POISON,
		.accuracy = 0,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = FLAG_SNATCH_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_RESET_STATS
	},

	[MOVE_HURRICANE] =
	{
		.effect = EFFECT_THUNDER,
		#ifdef GEN_6_POWER_NERFS
		.power = 110,
		#else
		.power = 120,
		#endif
		.type = TYPE_FLYING,
		.accuracy = 70,
		.pp = 10,
		.secondaryEffectChance = 30,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 190,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_ICICLECRASH] =
	{
		.effect = EFFECT_FLINCH_HIT,
		.power = 85,
		.type = TYPE_ICE,
		.accuracy = 90,
		.pp = 10,
		.secondaryEffectChance = 30,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 160,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_WORKUP] =
	{
		.effect = EFFECT_ATK_SPATK_UP,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 0,
		.pp = 30,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = FLAG_SNATCH_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_ATK_UP_1
	},

	[MOVE_QUIVERDANCE] =
	{
		.effect = EFFECT_CALM_MIND,
		.power = 0,
		.type = TYPE_BUG,
		.accuracy = 0,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = FLAG_SNATCH_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_RESET_STATS
	},

	[MOVE_LEAFTORNADO] =
	{
		.effect = EFFECT_ACCURACY_DOWN_HIT,
		.power = 65,
		.type = TYPE_GRASS,
		.accuracy = 90,
		.pp = 10,
		.secondaryEffectChance = 30,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 120,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_LOWSWEEP] =
	{
		.effect = EFFECT_SPEED_DOWN_HIT,
		.power = 65,
		.type = TYPE_FIGHTING,
		.accuracy = 100,
		.pp = 20,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 120,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_SNARL] =
	{
		.effect = EFFECT_SPECIAL_ATTACK_DOWN_HIT,
		.power = 55,
		.type = TYPE_DARK,
		.accuracy = 95,
		.pp = 15,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_BOTH,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_STRUGGLEBUG] =
	{
		.effect = EFFECT_SPECIAL_ATTACK_DOWN_HIT,
		.power = 50,
		.type = TYPE_BUG,
		.accuracy = 100,
		.pp = 20,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_BOTH,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_STEAMROLLER] =
	{
		.effect = EFFECT_FLINCH_HIT,
		.power = 65,
		.type = TYPE_BUG,
		.accuracy = 100,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 120,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_STORMTHROW] =
	{
		.effect = EFFECT_HIT,
		.power = 60,
		.type = TYPE_FIGHTING,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 120,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_VENOSHOCK] =
	{
		.effect = EFFECT_HIT, //Also double damage on poisoned foes
		.power = 65,
		.type = TYPE_POISON,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 120,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_WILDCHARGE] =
	{
		.effect = EFFECT_RECOIL,
		.power = 90,
		.type = TYPE_ELECTRIC,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 175,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_POISONJAB] =
	{
		.effect = EFFECT_POISON_HIT,
		.power = 80,
		.type = TYPE_POISON,
		.accuracy = 100,
		.pp = 20,
		.secondaryEffectChance = 30,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 160,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_ACIDSPRAY] =
	{
		.effect = EFFECT_SPECIAL_DEFENSE_DOWN_2_HIT,
		.power = 40,
		.type = TYPE_POISON,
		.accuracy = 100,
		.pp = 20,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_FIERYDANCE] =
	{
		.effect = EFFECT_SPECIAL_ATTACK_UP_HIT,
		.power = 80,
		.type = TYPE_FIRE,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 50,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 160,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_CLEARSMOG] =
	{
		.effect = EFFECT_REMOVE_TARGET_STAT_CHANGES,
		.power = 50,
		.type = TYPE_POISON,
		.accuracy = 0,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_LEAFSTORM] =
	{
		.effect = EFFECT_OVERHEAT,
		#ifdef GEN_6_POWER_NERFS
		.power = 130,
		#else
		.power = 140,
		#endif
		.type = TYPE_GRASS,
		.accuracy = 90,
		.pp = 5,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 200,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_STONEEDGE] =
	{
		.effect = EFFECT_HIGH_CRITICAL,
		.power = 100,
		.type = TYPE_ROCK,
		.accuracy = 80,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 180,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_PAYBACK] =
	{
		.effect = EFFECT_HIT,
		.power = 50,
		.type = TYPE_DARK,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_FIREFANG] =
	{
		.effect = EFFECT_FLINCH_HIT,
		.power = 65,
		.type = TYPE_FIRE,
		.accuracy = 95,
		.pp = 15,
		.secondaryEffectChance = 10,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 120,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_ICEFANG] =
	{
		.effect = EFFECT_FLINCH_HIT,
		.power = 65,
		.type = TYPE_ICE,
		.accuracy = 95,
		.pp = 15,
		.secondaryEffectChance = 10,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 120,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_THUNDERFANG] =
	{
		.effect = EFFECT_FLINCH_HIT,
		.power = 65,
		.type = TYPE_ELECTRIC,
		.accuracy = 95,
		.pp = 15,
		.secondaryEffectChance = 10,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 120,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_FLAREBLITZ] =
	{
		.effect = EFFECT_BURN_HIT,
		.power = 120,
		.type = TYPE_FIRE,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 10,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 190,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_INFERNO] =
	{
		.effect = EFFECT_BURN_HIT,
		.power = 100,
		.type = TYPE_FIRE,
		.accuracy = 50,
		.pp = 5,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 180,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_HEX] =
	{
		.effect = EFFECT_HIT,
		.power = 65,
		.type = TYPE_GHOST,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 160,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_DUALCHOP] =
	{
		.effect = EFFECT_DOUBLE_HIT,
		.power = 40,
		.type = TYPE_DRAGON,
		.accuracy = 90,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_DOUBLEHIT] =
	{
		.effect = EFFECT_DOUBLE_HIT,
		.power = 35,
		.type = TYPE_NORMAL,
		.accuracy = 90,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_COTTONGUARD] =
	{
		.effect = EFFECT_DEFENSE_UP_2,
		.power = 0,
		.type = TYPE_GRASS,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = FLAG_SNATCH_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_RESET_STATS
	},

	[MOVE_ACROBATICS] =
	{
		.effect = EFFECT_HIT,
		.power = 55,
		.type = TYPE_FLYING,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_FLAMEBURST] =
	{
		.effect = EFFECT_FLAMEBURST,
		.power = 70,
		.type = TYPE_FIRE,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 140,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_UTURN] =
	{
		.effect = EFFECT_BATON_PASS,
		.power = 70,
		.type = TYPE_BUG,
		.accuracy = 100,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 140,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_VOLTSWITCH] =
	{
		.effect = EFFECT_BATON_PASS,
		.power = 70,
		.type = TYPE_ELECTRIC,
		.accuracy = 100,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 140,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_DRAGONTAIL] =
	{
		.effect = EFFECT_ROAR,
		.power = 60,
		.type = TYPE_DRAGON,
		.accuracy = 90,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 250,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 120,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_CIRCLETHROW] =
	{
		.effect = EFFECT_ROAR,
		.power = 60,
		.type = TYPE_FIGHTING,
		.accuracy = 90,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 250,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 120,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_MIRRORSHOT] =
	{
		.effect = EFFECT_ACCURACY_DOWN_HIT,
		.power = 65,
		.type = TYPE_STEEL,
		.accuracy = 85,
		.pp = 10,
		.secondaryEffectChance = 30,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 120,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_WOODHAMMER] =
	{
		.effect = EFFECT_RECOIL,
		.power = 120,
		.type = TYPE_GRASS,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 190,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_HEALPULSE] =
	{
		.effect = EFFECT_HEAL_TARGET,
		.power = 0,
		.type = TYPE_PSYCHIC,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_TRIAGE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_RESET_STATS
	},

	[MOVE_FLASHCANNON] =
	{
		.effect = EFFECT_SPECIAL_DEFENSE_DOWN_HIT,
		.power = 80,
		.type = TYPE_STEEL,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 10,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 160,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_BUGBITE] =
	{
		.effect = EFFECT_EAT_BERRY,
		.power = 60,
		.type = TYPE_BUG,
		.accuracy = 100,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 120,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_PLUCK] =
	{
		.effect = EFFECT_EAT_BERRY,
		.power = 60,
		.type = TYPE_FLYING,
		.accuracy = 100,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 120,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_SHELLSMASH] =
	{
		.effect = EFFECT_DRAGON_DANCE,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 0,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = FLAG_SNATCH_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_RESET_STATS
	},

	[MOVE_RAGEPOWDER] =
	{
		.effect = EFFECT_FOLLOW_ME,
		.power = 0,
		.type = TYPE_BUG,
		.accuracy = 0,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 2,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_RESET_STATS
	},

	[MOVE_WAKEUPSLAP] =
	{
		.effect = EFFECT_SMELLINGSALT,
		.power = 70,
		.type = TYPE_FIGHTING,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 140,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_RAZORSHELL] =
	{
		.effect = EFFECT_DEFENSE_DOWN_HIT,
		.power = 75,
		.type = TYPE_WATER,
		.accuracy = 95,
		.pp = 10,
		.secondaryEffectChance = 50,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 140,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_COPYCAT] =
	{
		.effect = EFFECT_MIRROR_MOVE,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 0,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_DEPENDS,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_ACC_UP_1
	},

	[MOVE_MIRACLEEYE] =
	{
		.effect = EFFECT_FORESIGHT,
		.power = 0,
		.type = TYPE_PSYCHIC,
		.accuracy = 0,
		.pp = 40,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_SPATK_UP_1
	},

	[MOVE_SUCKERPUNCH] =
	{
		.effect = EFFECT_SUCKER_PUNCH,
		#ifdef GEN_7_POWER_NERFS
		.power = 70,
		#else
		.power = 80,
		#endif
		.type = TYPE_DARK,
		.accuracy = 100,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 1,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 160,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_SCALD] =
	{
		.effect = EFFECT_BURN_HIT,
		.power = 80,
		.type = TYPE_WATER,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 30,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 160,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_BRINE] =
	{
		.effect = EFFECT_HIT,
		.power = 65,
		.type = TYPE_WATER,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 120,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_FAIRYWIND] =
	{
		.effect = EFFECT_HIT,
		.power = 40,
		.type = TYPE_FAIRY,
		.accuracy = 100,
		.pp = 30,
		.secondaryEffectChance = 30,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_BABYDOLLEYES] =
	{
		.effect = EFFECT_ATTACK_DOWN,
		.power = 0,
		.type = TYPE_FAIRY,
		.accuracy = 100,
		.pp = 30,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 1,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_DEF_UP_1
	},

	[MOVE_PLAYROUGH] =
	{
		.effect = EFFECT_ATTACK_DOWN_HIT,
		.power = 90,
		.type = TYPE_FAIRY,
		.accuracy = 90,
		.pp = 10,
		.secondaryEffectChance = 10,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 175,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_MOONBLAST] =
	{
		.effect = EFFECT_SPECIAL_ATTACK_DOWN_HIT,
		.power = 95,
		.type = TYPE_FAIRY,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 30,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 175,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_DRAININGKISS] =
	{
		.effect = EFFECT_ABSORB,
		.power = 50,
		.type = TYPE_FAIRY,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_TRIAGE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_DAZZLINGGLEAM] =
	{
		.effect = EFFECT_HIT,
		.power = 80,
		.type = TYPE_FAIRY,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_BOTH,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 160,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_DISARMINGVOICE] =
	{
		.effect = EFFECT_ALWAYS_HIT,
		.power = 40,
		.type = TYPE_FAIRY,
		.accuracy = 0,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_BOTH,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_STEALTHROCK] =
	{
		.effect = EFFECT_SPIKES,
		.power = 0,
		.type = TYPE_ROCK,
		.accuracy = 0,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_OPPONENTS_FIELD,
		.priority = 0,
		.flags = FLAG_MAGIC_COAT_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_DEF_UP_1
	},

	[MOVE_TOXICSPIKES] =
	{
		.effect = EFFECT_SPIKES,
		.power = 0,
		.type = TYPE_POISON,
		.accuracy = 0,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_OPPONENTS_FIELD,
		.priority = 0,
		.flags = FLAG_MAGIC_COAT_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_DEF_UP_1
	},

	[MOVE_HEALINGWISH] =
	{
		.effect = EFFECT_MEMENTO,
		.power = 0,
		.type = TYPE_PSYCHIC,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = FLAG_SNATCH_AFFECTED | FLAG_TRIAGE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = 0
	},

	[MOVE_LUNARDANCE] =
	{
		.effect = EFFECT_MEMENTO,
		.power = 0,
		.type = TYPE_PSYCHIC,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = FLAG_SNATCH_AFFECTED | FLAG_TRIAGE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = 0
	},

	[MOVE_TAILWIND] =
	{
		.effect = EFFECT_TEAM_EFFECTS,
		.power = 0,
		.type = TYPE_FLYING,
		.accuracy = 0,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = FLAG_SNATCH_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_BOOST_CRITS
	},

	[MOVE_BOOMBURST] =
	{
		.effect = EFFECT_HIT,
		.power = 140,
		.type = TYPE_NORMAL,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_FOES_AND_ALLY,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 200,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_INCINERATE] =
	{
		.effect = EFFECT_EAT_BERRY,
		.power = 60,
		.type = TYPE_FIRE,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_BOTH,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 120,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_WORRYSEED] =
	{
		.effect = EFFECT_SKILL_SWAP,
		.power = 0,
		.type = TYPE_GRASS,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_SPD_UP_1
	},

	[MOVE_GASTROACID] =
	{
		.effect = EFFECT_SKILL_SWAP,
		.power = 0,
		.type = TYPE_POISON,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_SPD_UP_1
	},

	[MOVE_GEOMANCY] =
	{
		.effect = EFFECT_CALM_MIND,
		.power = 0,
		.type = TYPE_FAIRY,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_ALL_STATS_UP_1
	},

	[MOVE_FLOWERSHIELD] =
	{
		.effect = EFFECT_DEFENSE_UP,
		.power = 0,
		.type = TYPE_FAIRY,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_FOES_AND_ALLY,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_DEF_UP_1
	},

	[MOVE_ROTOTILLER] =
	{
		.effect = EFFECT_ATK_SPATK_UP,
		.power = 0,
		.type = TYPE_GROUND,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_FOES_AND_ALLY,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_ATK_UP_1
	},

	[MOVE_SHIFTGEAR] =
	{
		.effect = EFFECT_DRAGON_DANCE,
		.power = 0,
		.type = TYPE_STEEL,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = FLAG_SNATCH_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_RESET_STATS
	},

	[MOVE_STICKYWEB] =
	{
		.effect = EFFECT_SPIKES,
		.power = 0,
		.type = TYPE_BUG,
		.accuracy = 0,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_OPPONENTS_FIELD,
		.priority = 0,
		.flags = FLAG_MAGIC_COAT_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_SPD_UP_1
	},

	[MOVE_STOREDPOWER] =
	{
		.effect = EFFECT_HIT,
		.power = 20,
		.type = TYPE_PSYCHIC,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 160,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_ACUPRESSURE] =
	{
		.effect = EFFECT_EVASION_UP,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 0,
		.pp = 30,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER_OR_PARTNER,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_BOOST_CRITS
	},

	[MOVE_PUNISHMENT] =
	{
		.effect = EFFECT_HIT,
		.power = 60, //Starts at 60 so best to show it to the player
		.type = TYPE_DARK,
		.accuracy = 100,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 160,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_ASSURANCE] =
	{
		.effect = EFFECT_HIT,
		.power = 60,
		.type = TYPE_DARK,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 120,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_DRAGONRUSH] =
	{
		.effect = EFFECT_FLINCH_HIT,
		.power = 100,
		.type = TYPE_DRAGON,
		.accuracy = 75,
		.pp = 10,
		.secondaryEffectChance = 30,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 180,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_DARKVOID] =
	{
		.effect = EFFECT_SLEEP,
		.power = 0,
		.type = TYPE_DARK,
		#ifdef DARK_VOID_ACC_NERF
		.accuracy = 50,
		#else
		.accuracy = 80,
		#endif
		.pp = 10,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_BOTH,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_RESET_STATS
	},

	[MOVE_FEINT] =
	{
		.effect = EFFECT_FEINT,
		#ifndef UNBOUND
		.power = 30,
		#else //For Pokemon Unbound
		.power = 50,
		#endif
		.type = TYPE_NORMAL,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 2,
		.flags = FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_PHANTOMFORCE] =
	{
		.effect = EFFECT_SEMI_INVULNERABLE,
		.power = 90,
		.type = TYPE_GHOST,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 175,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_PETALBLIZZARD] =
	{
		.effect = EFFECT_HIT,
		.power = 90,
		.type = TYPE_GRASS,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_FOES_AND_ALLY,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 175,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_PLAYNICE] =
	{
		.effect = EFFECT_PLAY_NICE,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 0,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_DEF_UP_1
	},

	[MOVE_ROAROFTIME] =
	{
		.effect = EFFECT_RECHARGE,
		.power = 150,
		.type = TYPE_DRAGON,
		.accuracy = 90,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 200,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_SPACIALREND] =
	{
		.effect = EFFECT_HIGH_CRITICAL,
		.power = 100,
		.type = TYPE_DRAGON,
		.accuracy = 95,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 180,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_MAGMASTORM] =
	{
		.effect = EFFECT_TRAP,
		#ifdef GEN_6_POWER_NERFS
		.power = 100,
		#else
		.power = 120,
		#endif
		.type = TYPE_FIRE,
		.accuracy = 75,
		.pp = 5,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 180,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_FINALGAMBIT] =
	{
		.effect = EFFECT_MEMENTO,
		.power = 1,
		.type = TYPE_FIGHTING,
		.accuracy = 100,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED,
		.z_move_power = 180,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_SHADOWFORCE] =
	{
		.effect = EFFECT_SEMI_INVULNERABLE,
		.power = 120,
		.type = TYPE_GHOST,
		.accuracy = 100,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 190,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_NUZZLE] =
	{
		.effect = EFFECT_PARALYZE_HIT,
		.power = 20,
		.type = TYPE_ELECTRIC,
		.accuracy = 100,
		.pp = 20,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_RETALIATE] =
	{
		.effect = EFFECT_HIT,
		.power = 70,
		.type = TYPE_NORMAL,
		.accuracy = 100,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 140,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_STEELYHIT] =
	{
		.effect = EFFECT_HIT,
		.power = 40,
		.type = TYPE_STEEL,
		.accuracy = 100,
		.pp = 35,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_JUDGMENT] =
	{
		.effect = EFFECT_HIT,
		.power = 100,
		.type = TYPE_NORMAL,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 180,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_FREEZEDRY] =
	{
		.effect = EFFECT_FREEZE_HIT,
		.power = 70,
		.type = TYPE_ICE,
		.accuracy = 100,
		.pp = 20,
		#ifndef FROSTBITE
		.secondaryEffectChance = 10,
		#else
		.secondaryEffectChance = 30,
		#endif
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 140,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_PSYSHOCK] =
	{
		.effect = EFFECT_HIT,
		.power = 80,
		.type = TYPE_PSYCHIC,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 160,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_ROUND] =
	{
		.effect = EFFECT_FURY_CUTTER,
		.power = 60,
		.type = TYPE_NORMAL,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 120,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_TAILSLAP] =
	{
		.effect = EFFECT_MULTI_HIT,
		.power = 25,
		.type = TYPE_NORMAL,
		.accuracy = 85,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_GEARGRIND] =
	{
		.effect = EFFECT_DOUBLE_HIT,
		.power = 50,
		.type = TYPE_STEEL,
		.accuracy = 85,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 180,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_NATURALGIFT] =
	{
		.effect = EFFECT_NATURAL_GIFT,
		.power = 1,
		.type = TYPE_NORMAL,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 160,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_CHIPAWAY] =
	{
		.effect = EFFECT_HIT,
		.power = 70,
		.type = TYPE_NORMAL,
		.accuracy = 100,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 140,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_SMACKDOWN] =
	{
		.effect = EFFECT_SMACK_DOWN,
		.power = 50,
		.type = TYPE_ROCK,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_HYPERSPACEHOLE] =
	{
		.effect = EFFECT_FEINT,
		.power = 80,
		.type = TYPE_PSYCHIC,
		.accuracy = 0,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 160,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_GYROBALL] =
	{
		.effect = EFFECT_HIT,
		.power = 1,
		.type = TYPE_STEEL,
		.accuracy = 100,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 160,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_HYPERSPACEFURY] =
	{
		.effect = EFFECT_SUPERPOWER,
		.power = 100,
		.type = TYPE_DARK,
		.accuracy = 0,
		.pp = 5,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 180,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_ATTACKORDER] =
	{
		.effect = EFFECT_HIGH_CRITICAL,
		.power = 90,
		.type = TYPE_BUG,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 175,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_DEFENDORDER] =
	{
		.effect = EFFECT_COSMIC_POWER,
		.power = 0,
		.type = TYPE_BUG,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = FLAG_SNATCH_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_DEF_UP_1
	},

	[MOVE_HEALORDER] =
	{
		.effect = EFFECT_RESTORE_HP,
		.power = 0,
		.type = TYPE_BUG,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = FLAG_SNATCH_AFFECTED | FLAG_TRIAGE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_RESET_STATS
	},

	[MOVE_CAPTIVATE] =
	{
		.effect = EFFECT_SPECIAL_ATTACK_DOWN_2,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 100,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_BOTH,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_SPDEF_UP_2
	},

	[MOVE_DEFOG] =
	{
		.effect = EFFECT_RAPID_SPIN,
		.power = 0,
		.type = TYPE_FLYING,
		.accuracy = 0,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_ACC_UP_1
	},

	[MOVE_ORIGINPULSE] =
	{
		.effect = EFFECT_HIT,
		#ifdef GEN_6_POWER_NERFS
		.power = 110,
		#else
		.power = 120,
		#endif
		.type = TYPE_WATER,
		.accuracy = 85,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_BOTH,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 190,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_PRECIPICEBLADES] =
	{
		.effect = EFFECT_HIT,
		.power = 120,
		.type = TYPE_GROUND,
		.accuracy = 85,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_BOTH,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 190,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_GUARDSWAP] =
	{
		.effect = EFFECT_STAT_SWAP_SPLIT,
		.power = 0,
		.type = TYPE_PSYCHIC,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_SPD_UP_1
	},

	[MOVE_STEAMERUPTION] =
	{
		.effect = EFFECT_BURN_HIT,
		.power = 120,
		.type = TYPE_WATER,
		.accuracy = 95,
		.pp = 5,
		.secondaryEffectChance = 30,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 190,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_METALBURST] =
	{
		.effect = EFFECT_COUNTER,
		.power = 1,
		.type = TYPE_STEEL,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_DEPENDS,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_MEFIRST] =
	{
		.effect = EFFECT_ME_FIRST,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 0,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_SPD_UP_2
	},

	[MOVE_POWERSWAP] =
	{
		.effect = EFFECT_STAT_SWAP_SPLIT,
		.power = 0,
		.type = TYPE_PSYCHIC,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_SPD_UP_1
	},

	[MOVE_POWERTRICK] =
	{
		.effect = EFFECT_STAT_SWAP_SPLIT,
		.power = 0,
		.type = TYPE_PSYCHIC,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = FLAG_SNATCH_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_ATK_UP_1
	},

	[MOVE_PSYCHOSHIFT] =
	{
		.effect = EFFECT_REFRESH,
		.power = 0,
		.type = TYPE_PSYCHIC,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_SPATK_UP_2
	},

	[MOVE_WRINGOUT] =
	{
		.effect = EFFECT_HIT,
		.power = 1,
		.type = TYPE_NORMAL,
		.accuracy = 100,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 190,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_KINGSSHIELD] =
	{
		.effect = EFFECT_PROTECT,
		.power = 0,
		.type = TYPE_STEEL,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 4,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_RESET_STATS
	},

	[MOVE_PSYSTRIKE] =
	{
		.effect = EFFECT_HIT,
		.power = 100,
		.type = TYPE_PSYCHIC,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 180,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_AUTOTOMIZE] =
	{
		.effect = EFFECT_SPEED_UP_2,
		.power = 0,
		.type = TYPE_STEEL,
		.accuracy = 0,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = FLAG_SNATCH_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_RESET_STATS
	},

	[MOVE_DIAMONDSTORM] =
	{
		.effect = EFFECT_DEFENSE_UP_2_HIT,
		.power = 100,
		.type = TYPE_ROCK,
		.accuracy = 95,
		.pp = 5,
		.secondaryEffectChance = 50,
		.target = MOVE_TARGET_BOTH,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 180,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_SLUDGEWAVE] =
	{
		.effect = EFFECT_POISON_HIT,
		.power = 95,
		.type = TYPE_POISON,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 10,
		.target = MOVE_TARGET_FOES_AND_ALLY,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 175,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_HEAVYSLAM] =
	{
		.effect = EFFECT_HIT,
		.power = 1,
		.type = TYPE_STEEL,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 160,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_SYNCHRONOISE] =
	{
		.effect = EFFECT_HIT,
		.power = 120,
		.type = TYPE_PSYCHIC,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_FOES_AND_ALLY,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 190,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_ELECTROBALL] =
	{
		.effect = EFFECT_HIT,
		.power = 1,
		.type = TYPE_ELECTRIC,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 160,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_FOULPLAY] =
	{
		.effect = EFFECT_HIT,
		.power = 95,
		.type = TYPE_DARK,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 175,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_ENTRAINMENT] =
	{
		.effect = EFFECT_SKILL_SWAP,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_SPDEF_UP_1
	},

	[MOVE_NIGHTDAZE] =
	{
		.effect = EFFECT_ACCURACY_DOWN_HIT,
		.power = 85,
		.type = TYPE_DARK,
		.accuracy = 95,
		.pp = 10,
		.secondaryEffectChance = 40,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 160,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_ECHOEDVOICE] =
	{
		.effect = EFFECT_FURY_CUTTER,
		.power = 40,
		.type = TYPE_NORMAL,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_LANDSWRATH] =
	{
		.effect = EFFECT_HIT,
		.power = 90,
		.type = TYPE_GROUND,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_BOTH,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 175,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_OBLIVIONWING] =
	{
		.effect = EFFECT_ABSORB,
		.power = 80,
		.type = TYPE_FLYING,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_TRIAGE_AFFECTED,
		.z_move_power = 160,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_HEARTSWAP] =
	{
		.effect = EFFECT_STAT_SWAP_SPLIT,
		.power = 0,
		.type = TYPE_PSYCHIC,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_BOOST_CRITS
	},

	[MOVE_CRUSHGRIP] =
	{
		.effect = EFFECT_HIT,
		.power = 1,
		.type = TYPE_NORMAL,
		.accuracy = 100,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 190,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_SACREDSWORD] =
	{
		.effect = EFFECT_HIT,
		.power = 90,
		.type = TYPE_FIGHTING,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 175,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_HEATCRASH] =
	{
		.effect = EFFECT_HIT,
		.power = 1,
		.type = TYPE_FIRE,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 160,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_HEADCHARGE] =
	{
		.effect = EFFECT_RECOIL,
		.power = 120,
		.type = TYPE_NORMAL,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 190,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_TECHNOBLAST] =
	{
		.effect = EFFECT_HIT,
		.power = 120,
		.type = TYPE_NORMAL,
		.accuracy = 100,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 190,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_RELICSONG] =
	{
		.effect = EFFECT_RELIC_SONG,
		.power = 75,
		.type = TYPE_NORMAL,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 10,
		.target = MOVE_TARGET_BOTH,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 140,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_SECRETSWORD] =
	{
		.effect = EFFECT_HIT,
		.power = 85,
		.type = TYPE_FIGHTING,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 160,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_GLACIATE] =
	{
		.effect = EFFECT_SPEED_DOWN_HIT,
		#ifdef UNBOUND
		.power = 75,
		#else
		.power = 65,
		#endif
		.type = TYPE_ICE,
		#ifdef UNBOUND
		.accuracy = 100,
		#else
		.accuracy = 95,
		#endif
		.pp = 10,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_BOTH,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 120,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_BOLTSTRIKE] =
	{
		.effect = EFFECT_PARALYZE_HIT,
		.power = 130,
		.type = TYPE_ELECTRIC,
		.accuracy = 85,
		.pp = 5,
		.secondaryEffectChance = 20,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 195,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_BLUEFLARE] =
	{
		.effect = EFFECT_BURN_HIT,
		.power = 130,
		.type = TYPE_FIRE,
		.accuracy = 85,
		.pp = 5,
		.secondaryEffectChance = 20,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 195,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_FREEZESHOCK] =
	{
		.effect = EFFECT_SKY_ATTACK,
		.power = 140,
		.type = TYPE_ICE,
		.accuracy = 90,
		.pp = 5,
		.secondaryEffectChance = 30,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 200,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_ICEBURN] =
	{
		.effect = EFFECT_SKY_ATTACK,
		.power = 140,
		.type = TYPE_ICE,
		.accuracy = 90,
		.pp = 5,
		.secondaryEffectChance = 30,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 200,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_VCREATE] =
	{
		.effect = EFFECT_SUPERPOWER,
		.power = 180,
		.type = TYPE_FIRE,
		.accuracy = 95,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 220,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_FUSIONFLARE] =
	{
		.effect = EFFECT_HIT,
		.power = 100,
		.type = TYPE_FIRE,
		.accuracy = 100,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 180,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_FUSIONBOLT] =
	{
		.effect = EFFECT_HIT,
		.power = 100,
		.type = TYPE_ELECTRIC,
		.accuracy = 100,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 180,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_FELLSTINGER] =
	{
		.effect = EFFECT_ATTACK_UP_HIT,
		.power = 50,
		.type = TYPE_BUG,
		.accuracy = 100,
		.pp = 25,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_NOBLEROAR] =
	{
		.effect = EFFECT_PLAY_NICE,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 100,
		.pp = 30,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_DEF_UP_1
	},

	[MOVE_DRAGONASCENT] =
	{
		.effect = EFFECT_SUPERPOWER,
		.power = 120,
		.type = TYPE_FLYING,
		.accuracy = 100,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 190,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_PARTINGSHOT] =
	{
		.effect = EFFECT_BATON_PASS,
		.power = 0,
		.type = TYPE_DARK,
		.accuracy = 100,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_RESTORE_REPLACEMENT_HP
	},

	[MOVE_TOPSYTURVY] =
	{
		.effect = EFFECT_TOPSY_TURVY_ELECTRIFY,
		.power = 0,
		.type = TYPE_DARK,
		.accuracy = 0,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_ATK_UP_1
	},

	[MOVE_COREENFORCER] =
	{
		.effect = EFFECT_SKILL_SWAP,
		.power = 100,
		.type = TYPE_DRAGON,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_BOTH,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 140,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_CHATTER] =
	{
		.effect = EFFECT_CONFUSE_HIT,
		.power = 65,
		.type = TYPE_FLYING,
		.accuracy = 100,
		.pp = 20,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 120,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_CONFIDE] =
	{
		.effect = EFFECT_SPECIAL_ATTACK_DOWN,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 0,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_SPDEF_UP_1
	},

	[MOVE_WATERSHURIKEN] =
	{
		.effect = EFFECT_MULTI_HIT,
		.power = 15,
		.type = TYPE_WATER,
		.accuracy = 100,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 1,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_MYSTICALFIRE] =
	{
		.effect = EFFECT_SPECIAL_ATTACK_DOWN_HIT,
		.power = 75,
		.type = TYPE_FIRE,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 140,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_SPIKYSHIELD] =
	{
		.effect = EFFECT_PROTECT,
		.power = 0,
		.type = TYPE_GRASS,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 4,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_DEF_UP_1
	},

	[MOVE_EERIEIMPULSE] =
	{
		.effect = EFFECT_SPECIAL_ATTACK_DOWN_2,
		.power = 0,
		.type = TYPE_ELECTRIC,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_SPDEF_UP_1
	},

	[MOVE_HOLDBACK] =
	{
		.effect = EFFECT_FALSE_SWIPE,
		.power = 40,
		.type = TYPE_NORMAL,
		.accuracy = 100,
		.pp = 40,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_INFESTATION] =
	{
		.effect = EFFECT_TRAP,
		.power = 20,
		.type = TYPE_BUG,
		.accuracy = 100,
		.pp = 20,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_POWERUPPUNCH] =
	{
		.effect = EFFECT_ATTACK_UP_HIT,
		.power = 40,
		.type = TYPE_FIGHTING,
		.accuracy = 100,
		.pp = 20,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_THOUSANDARROWS] =
	{
		.effect = EFFECT_SMACK_DOWN,
		.power = 90,
		.type = TYPE_GROUND,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_BOTH,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 175,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_THOUSANDWAVES] =
	{
		.effect = EFFECT_MEAN_LOOK,
		.power = 90,
		.type = TYPE_GROUND,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_BOTH,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 175,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_PARABOLICCHARGE] =
	{
		.effect = EFFECT_ABSORB,
		.power = 65,
		.type = TYPE_ELECTRIC,
		.accuracy = 100,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_FOES_AND_ALLY,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_TRIAGE_AFFECTED,
		.z_move_power = 120,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_SEARINGSHOT] =
	{
		.effect = EFFECT_BURN_HIT,
		.power = 100,
		.type = TYPE_FIRE,
		.accuracy = 100,
		.pp = 5,
		.secondaryEffectChance = 30,
		.target = MOVE_TARGET_FOES_AND_ALLY,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 180,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_SEEDFLARE] =
	{
		.effect = EFFECT_SPECIAL_DEFENSE_DOWN_2_HIT,
		.power = 120,
		.type = TYPE_GRASS,
		.accuracy = 85,
		.pp = 5,
		.secondaryEffectChance = 40,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 190,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_ACCELEROCK] =
	{
		.effect = EFFECT_QUICK_ATTACK,
		.power = 40,
		.type = TYPE_ROCK,
		.accuracy = 100,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 1,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_ANCHORSHOT] =
	{
		.effect = EFFECT_MEAN_LOOK,
		.power = 80,
		.type = TYPE_STEEL,
		.accuracy = 100,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 160,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_AURORAVEIL] =
	{
		.effect = EFFECT_REFLECT,
		.power = 0,
		.type = TYPE_ICE,
		.accuracy = 0,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = FLAG_SNATCH_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_SPD_UP_1
	},

	[MOVE_BANEFULBUNKER] =
	{
		.effect = EFFECT_PROTECT,
		.power = 0,
		.type = TYPE_POISON,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 4,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_DEF_UP_1
	},

	[MOVE_BEAKBLAST] =
	{
		.effect = EFFECT_FOCUS_PUNCH,
		.power = 100,
		.type = TYPE_FLYING,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 253,
		.flags = FLAG_PROTECT_AFFECTED,
		.z_move_power = 180,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_BRUTALSWING] =
	{
		.effect = EFFECT_HIT,
		.power = 60,
		.type = TYPE_DARK,
		.accuracy = 100,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_FOES_AND_ALLY,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 120,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_BURNUP] =
	{
		.effect = EFFECT_BURN_UP,
		#ifdef GEN_6_POWER_NERFS
		.power = 130,
		#else
		.power = 140,
		#endif
		.type = TYPE_FIRE,
		.accuracy = 100,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 200,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_CLANGINGSCALES] =
	{
		.effect = EFFECT_SUPERPOWER,
		#ifdef GEN_6_POWER_NERFS
		.power = 110,
		#else
		.power = 120,
		#endif
		.type = TYPE_DRAGON,
		.accuracy = 100,
		.pp = 5,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_BOTH,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 190,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_DARKESTLARIAT] =
	{
		.effect = EFFECT_HIT,
		.power = 85,
		.type = TYPE_DARK,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 160,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_DRAGONHAMMER] =
	{
		.effect = EFFECT_HIT,
		.power = 90,
		.type = TYPE_DRAGON,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 175,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_FIRELASH] =
	{
		.effect = EFFECT_DEFENSE_DOWN_HIT,
		.power = 80,
		.type = TYPE_FIRE,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 160,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_FIRSTIMPRESSION] =
	{
		.effect = EFFECT_FAKE_OUT,
		.power = 90,
		.type = TYPE_BUG,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 2,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 175,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_FLEURCANNON] =
	{
		.effect = EFFECT_OVERHEAT,
		#ifdef GEN_6_POWER_NERFS
		.power = 130,
		#else
		.power = 140,
		#endif
		.type = TYPE_FAIRY,
		.accuracy = 90,
		.pp = 5,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 200,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_FLORALHEALING] =
	{
		.effect = EFFECT_HEAL_TARGET,
		.power = 0,
		.type = TYPE_FAIRY,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_TRIAGE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_RESET_STATS
	},

	[MOVE_GEARUP] =
	{
		.effect = EFFECT_ATK_SPATK_UP,
		.power = 0,
		.type = TYPE_STEEL,
		.accuracy = 0,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_FOES_AND_ALLY,
		.priority = 0,
		.flags = FLAG_SNATCH_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_SPATK_UP_1
	},

	[MOVE_HIGHHORSEPOWER] =
	{
		.effect = EFFECT_HIT,
		.power = 95,
		.type = TYPE_GROUND,
		.accuracy = 95,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 175,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_ICEHAMMER] =
	{
		.effect = EFFECT_SUPERPOWER,
		.power = 100,
		.type = TYPE_ICE,
		.accuracy = 90,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 180,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_LASERFOCUS] =
	{
		.effect = EFFECT_LOCK_ON,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 0,
		.pp = 30,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = FLAG_SNATCH_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_ATK_UP_1
	},

	[MOVE_LEAFAGE] =
	{
		.effect = EFFECT_HIT,
		.power = 40,
		.type = TYPE_GRASS,
		.accuracy = 100,
		.pp = 40,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_LIQUIDATION] =
	{
		.effect = EFFECT_DEFENSE_DOWN_HIT,
		.power = 85,
		.type = TYPE_WATER,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 20,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 160,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_LUNGE] =
	{
		.effect = EFFECT_ATTACK_DOWN_HIT,
		.power = 80,
		.type = TYPE_BUG,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 160,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_MOONGEISTBEAM] =
	{
		.effect = EFFECT_HIT,
		.power = 100,
		.type = TYPE_GHOST,
		.accuracy = 100,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 180,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_MULTIATTACK] =
	{
		.effect = EFFECT_HIT,
		.power = 120,
		.type = TYPE_NORMAL,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 175,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_NATURESMADNESS] =
	{
		.effect = EFFECT_SUPER_FANG,
		.power = 1,
		.type = TYPE_FAIRY,
		.accuracy = 90,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_POLLENPUFF] =
	{
		.effect = EFFECT_HEAL_TARGET,
		.power = 90,
		.type = TYPE_BUG,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 175,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_POWERTRIP] =
	{
		.effect = EFFECT_HIT,
		.power = 20,
		.type = TYPE_DARK,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 160,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_PRISMATICLASER] =
	{
		.effect = EFFECT_RECHARGE,
		.power = 160,
		.type = TYPE_PSYCHIC,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 200,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_PSYCHICFANGS] =
	{
		.effect = EFFECT_BRICK_BREAK,
		.power = 85,
		.type = TYPE_PSYCHIC,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 160,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_PURIFY] =
	{
		.effect = EFFECT_RESTORE_HP,
		.power = 0,
		.type = TYPE_POISON,
		.accuracy = 0,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_TRIAGE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_ALL_STATS_UP_1
	},

	[MOVE_REVELATIONDANCE] =
	{
		.effect = EFFECT_HIT,
		.power = 90,
		.type = TYPE_NORMAL,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 175,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_SHADOWBONE] =
	{
		.effect = EFFECT_DEFENSE_DOWN_HIT,
		.power = 85,
		.type = TYPE_GHOST,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 20,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 160,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_SHELLTRAP] =
	{
		.effect = EFFECT_FOCUS_PUNCH,
		.power = 150,
		.type = TYPE_FIRE,
		.accuracy = 100,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_BOTH,
		.priority = 253,
		.flags = FLAG_PROTECT_AFFECTED,
		.z_move_power = 200,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_SHOREUP] =
	{
		.effect = EFFECT_MORNING_SUN,
		.power = 0,
		.type = TYPE_GROUND,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = FLAG_SNATCH_AFFECTED | FLAG_TRIAGE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_RESET_STATS
	},

	[MOVE_SMARTSTRIKE] =
	{
		.effect = EFFECT_ALWAYS_HIT,
		.power = 70,
		.type = TYPE_STEEL,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 140,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_SOLARBLADE] =
	{
		.effect = EFFECT_SOLARBEAM,
		.power = 125,
		.type = TYPE_GRASS,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 190,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_SPARKLINGARIA] =
	{
		.effect = EFFECT_SMELLINGSALT,
		.power = 90,
		.type = TYPE_WATER,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_FOES_AND_ALLY,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 175,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_SPECTRALTHIEF] =
	{
		.effect = EFFECT_PSYCH_UP,
		.power = 90,
		.type = TYPE_GHOST,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 175,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_SPEEDSWAP] =
	{
		.effect = EFFECT_STAT_SWAP_SPLIT,
		.power = 0,
		.type = TYPE_PSYCHIC,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_SPD_UP_1
	},

	[MOVE_SPIRITSHACKLE] =
	{
		.effect = EFFECT_MEAN_LOOK,
		.power = 80,
		.type = TYPE_GHOST,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 160,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_SPOTLIGHT] =
	{
		.effect = EFFECT_FOLLOW_ME,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 0,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 3,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_SPDEF_UP_1
	},

	[MOVE_STOMPINGTANTRUM] =
	{
		.effect = EFFECT_HIT,
		.power = 75,
		.type = TYPE_GROUND,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 140,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_STRENGTHSAP] =
	{
		.effect = EFFECT_ABSORB,
		.power = 0,
		.type = TYPE_GRASS,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_TRIAGE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_DEF_UP_1
	},

	[MOVE_SUNSTEELSTRIKE] =
	{
		.effect = EFFECT_HIT,
		.power = 100,
		.type = TYPE_STEEL,
		.accuracy = 100,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 180,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_TEARFULLOOK] =
	{
		.effect = EFFECT_PLAY_NICE,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 0,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_DEF_UP_1
	},

	[MOVE_THROATCHOP] =
	{
		.effect = EFFECT_ATTACK_BLOCKERS,
		.power = 80,
		.type = TYPE_DARK,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 160,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_TOXICTHREAD] =
	{
		.effect = EFFECT_POISON,
		.power = 0,
		.type = TYPE_POISON,
		.accuracy = 100,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_SPD_UP_1
	},

	[MOVE_TROPKICK] =
	{
		.effect = EFFECT_ATTACK_DOWN_HIT,
		.power = 70,
		.type = TYPE_GRASS,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 140,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_ZINGZAP] =
	{
		.effect = EFFECT_FLINCH_HIT,
		.power = 80,
		.type = TYPE_ELECTRIC,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 30,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 160,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_ELECTRICTERRAIN] =
	{
		.effect = EFFECT_SET_TERRAIN,
		.power = 0,
		.type = TYPE_ELECTRIC,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_SPD_UP_1
	},

	[MOVE_GRASSYTERRAIN] =
	{
		.effect = EFFECT_SET_TERRAIN,
		.power = 0,
		.type = TYPE_GRASS,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_DEF_UP_1
	},

	[MOVE_MISTYTERRAIN] =
	{
		.effect = EFFECT_SET_TERRAIN,
		.power = 0,
		.type = TYPE_FAIRY,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_SPDEF_UP_1
	},

	[MOVE_PSYCHICTERRAIN] =
	{
		.effect = EFFECT_SET_TERRAIN,
		.power = 0,
		.type = TYPE_PSYCHIC,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_SPATK_UP_1
	},

	[MOVE_GRASSPLEDGE] =
	{
		.effect = EFFECT_PLEDGE,
		.power = 80,
		.type = TYPE_GRASS,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 160,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_FIREPLEDGE] =
	{
		.effect = EFFECT_PLEDGE,
		.power = 80,
		.type = TYPE_FIRE,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 160,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_WATERPLEDGE] =
	{
		.effect = EFFECT_PLEDGE,
		.power = 80,
		.type = TYPE_WATER,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 160,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_TRICKROOM] =
	{
		.effect = EFFECT_FIELD_EFFECTS,
		.power = 0,
		.type = TYPE_PSYCHIC,
		.accuracy = 0,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 249,
		.flags = FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_ACC_UP_1
	},

	[MOVE_MAGICROOM] =
	{
		.effect = EFFECT_FIELD_EFFECTS,
		.power = 0,
		.type = TYPE_PSYCHIC,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_SPDEF_UP_1
	},

	[MOVE_WONDERROOM] =
	{
		.effect = EFFECT_FIELD_EFFECTS,
		.power = 0,
		.type = TYPE_PSYCHIC,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_SPDEF_UP_1
	},

	[MOVE_CRAFTYSHIELD] =
	{
		.effect = EFFECT_PROTECT,
		.power = 0,
		.type = TYPE_FAIRY,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 3,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_SPDEF_UP_1
	},

	[MOVE_MATBLOCK] =
	{
		.effect = EFFECT_PROTECT,
		.power = 0,
		.type = TYPE_FIGHTING,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = FLAG_SNATCH_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_DEF_UP_1
	},

	[MOVE_QUICKGUARD] =
	{
		.effect = EFFECT_PROTECT,
		.power = 0,
		.type = TYPE_FIGHTING,
		.accuracy = 0,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 3,
		.flags = FLAG_SNATCH_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_DEF_UP_1
	},

	[MOVE_WIDEGUARD] =
	{
		.effect = EFFECT_PROTECT,
		.power = 0,
		.type = TYPE_ROCK,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 3,
		.flags = FLAG_SNATCH_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_DEF_UP_1
	},

	[MOVE_AQUARING] =
	{
		.effect = EFFECT_INGRAIN,
		.power = 0,
		.type = TYPE_WATER,
		.accuracy = 0,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = FLAG_SNATCH_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_DEF_UP_1
	},

	[MOVE_AROMATICMIST] =
	{
		.effect = EFFECT_SPECIAL_DEFENSE_UP,
		.power = 0,
		.type = TYPE_FAIRY,
		.accuracy = 0,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_SPDEF_UP_2
	},

	[MOVE_BELCH] =
	{
		.effect = EFFECT_RECYCLE,
		.power = 120,
		.type = TYPE_POISON,
		.accuracy = 90,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED,
		.z_move_power = 190,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_BESTOW] =
	{
		.effect = EFFECT_TRICK,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 0,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_SPD_UP_2
	},

	[MOVE_FLING] =
	{
		.effect = EFFECT_FLING,
		.power = 1,
		.type = TYPE_DARK,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_FLYINGPRESS] =
	{
		.effect = EFFECT_HIT,
		.power = 100,
		.type = TYPE_FIGHTING,
		.accuracy = 95,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 170,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_GRAVITY] =
	{
		.effect = EFFECT_FIELD_EFFECTS,
		.power = 0,
		.type = TYPE_PSYCHIC,
		.accuracy = 0,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_FOES_AND_ALLY,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_SPATK_UP_1
	},

	[MOVE_GUARDSPLIT] =
	{
		.effect = EFFECT_STAT_SWAP_SPLIT,
		.power = 0,
		.type = TYPE_PSYCHIC,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_SPD_UP_1
	},

	[MOVE_HEALBLOCK] =
	{
		.effect = EFFECT_ATTACK_BLOCKERS,
		.power = 0,
		.type = TYPE_PSYCHIC,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_BOTH,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_SPATK_UP_2
	},

	[MOVE_IONDELUGE] =
	{
		.effect = EFFECT_FIELD_EFFECTS,
		.power = 0,
		.type = TYPE_ELECTRIC,
		.accuracy = 0,
		.pp = 25,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 1,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_SPATK_UP_1
	},

	[MOVE_LUCKYCHANT] =
	{
		.effect = EFFECT_TEAM_EFFECTS,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 0,
		.pp = 30,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = FLAG_SNATCH_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_EVSN_UP_1
	},

	[MOVE_MAGNETRISE] =
	{
		.effect = EFFECT_TEAM_EFFECTS,
		.power = 0,
		.type = TYPE_ELECTRIC,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = FLAG_SNATCH_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_EVSN_UP_1
	},

	[MOVE_MAGNETICFLUX] =
	{
		.effect = EFFECT_COSMIC_POWER,
		.power = 0,
		.type = TYPE_ELECTRIC,
		.accuracy = 0,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_FOES_AND_ALLY,
		.priority = 0,
		.flags = FLAG_SNATCH_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_SPDEF_UP_1
	},

	[MOVE_POWDER] =
	{
		.effect = EFFECT_ATTACK_BLOCKERS,
		.power = 0,
		.type = TYPE_BUG,
		.accuracy = 100,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 1,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_SPDEF_UP_2
	},

	[MOVE_POWERSPLIT] =
	{
		.effect = EFFECT_STAT_SWAP_SPLIT,
		.power = 0,
		.type = TYPE_PSYCHIC,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_SPD_UP_1
	},

	[MOVE_REFLECTTYPE] =
	{
		.effect = EFFECT_CONVERSION,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 0,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_SPATK_UP_1
	},

	[MOVE_SIMPLEBEAM] =
	{
		.effect = EFFECT_SKILL_SWAP,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_SPATK_UP_1
	},

	[MOVE_SOAK] =
	{
		.effect = EFFECT_TYPE_CHANGES,
		.power = 0,
		.type = TYPE_WATER,
		.accuracy = 100,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_SPATK_UP_1
	},

	[MOVE_TELEKINESIS] =
	{
		.effect = EFFECT_ATTACK_BLOCKERS,
		.power = 0,
		.type = TYPE_PSYCHIC,
		.accuracy = 0,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_SPATK_UP_1
	},

	[MOVE_TRICKORTREAT] =
	{
		.effect = EFFECT_TYPE_CHANGES,
		.power = 0,
		.type = TYPE_GHOST,
		.accuracy = 100,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_ALL_STATS_UP_1
	},

	[MOVE_VENOMDRENCH] =
	{
		.effect = EFFECT_VENOM_DRENCH,
		.power = 0,
		.type = TYPE_POISON,
		.accuracy = 100,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_BOTH,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_DEF_UP_1
	},

	[MOVE_FORESTSCURSE] =
	{
		.effect = EFFECT_TYPE_CHANGES,
		.power = 0,
		.type = TYPE_GRASS,
		.accuracy = 100,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = 0
	},

	[MOVE_MINDBLOWN] =
	{
		.effect = EFFECT_HIT,
		.power = 150,
		.type = TYPE_FIRE,
		.accuracy = 100,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_FOES_AND_ALLY,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 200,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_PHOTONGEYSER] =
	{
		.effect = EFFECT_HIT,
		.power = 100,
		.type = TYPE_PSYCHIC,
		.accuracy = 100,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 180,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_PLASMAFISTS] =
	{
		.effect = EFFECT_FIELD_EFFECTS,
		.power = 100,
		.type = TYPE_ELECTRIC,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 180,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_ELECTRIFY] =
	{
		.effect = EFFECT_TOPSY_TURVY_ELECTRIFY,
		.power = 0,
		.type = TYPE_ELECTRIC,
		.accuracy = 0,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_SPATK_UP_1
	},

	[MOVE_EMBARGO] =
	{
		.effect = EFFECT_ATTACK_BLOCKERS,
		.power = 0,
		.type = TYPE_DARK,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_SPATK_UP_1
	},

	[MOVE_FAIRYLOCK] =
	{
		.effect = EFFECT_FAIRY_LOCK_HAPPY_HOUR,
		.power = 0,
		.type = TYPE_FAIRY,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_DEF_UP_1
	},

	[MOVE_LIGHTOFRUIN] =
	{
		.effect = EFFECT_RECOIL,
		.power = 140,
		.type = TYPE_FAIRY,
		.accuracy = 90,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 200,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_TRUMPCARD] =
	{
		.effect = EFFECT_HIT,
		.power = 1,
		.type = TYPE_NORMAL,
		.accuracy = 0,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 160,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_AFTERYOU] =
	{
		.effect = EFFECT_INSTRUCT_AFTER_YOU_QUASH,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 0,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_SPD_UP_1
	},

	[MOVE_INSTRUCT] =
	{
		.effect = EFFECT_INSTRUCT_AFTER_YOU_QUASH,
		.power = 0,
		.type = TYPE_PSYCHIC,
		.accuracy = 0,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_SPATK_UP_1
	},

	[MOVE_QUASH] =
	{
		.effect = EFFECT_INSTRUCT_AFTER_YOU_QUASH,
		.power = 0,
		.type = TYPE_DARK,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_SPD_UP_1
	},

	[MOVE_HAPPYHOUR] =
	{
		.effect = EFFECT_FAIRY_LOCK_HAPPY_HOUR,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 0,
		.pp = 30,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_ALL_STATS_UP_1
	},

	[MOVE_DOUBLEIRONBASH] =
	{
		.effect = EFFECT_FLINCH_HIT,
		.power = 60,
		.type = TYPE_STEEL,
		.accuracy = 100,
		.pp = 5,
		.secondaryEffectChance = 30,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 120,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_LASTRESORT] =
	{
		.effect = EFFECT_LAST_RESORT,
		.power = 140,
		.type = TYPE_NORMAL,
		.accuracy = 100,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 200,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_SKYDROP] =
	{
		.effect = EFFECT_SKY_DROP,
		.power = 60,
		.type = TYPE_FLYING,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 120,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_CELEBRATE] =
	{
		.effect = EFFECT_FAIRY_LOCK_HAPPY_HOUR,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 0,
		.pp = 40,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_ALL_STATS_UP_1
	},

	[MOVE_HOLDHANDS] =
	{
		.effect = EFFECT_FAIRY_LOCK_HAPPY_HOUR,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 0,
		.pp = 40,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_ALL_STATS_UP_1
	},

	[MOVE_DYNAMAXCANNON] =
	{
		.effect = 0,
		.power = 100,
		.type = TYPE_DRAGON,
		.accuracy = 100,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED,
		.split = SPLIT_SPECIAL,
		.z_move_power = 180,
		.z_move_effect = 0,
	},

	[MOVE_SNIPESHOT] =
	{
		.effect = EFFECT_IGNORE_REDIRECTION,
		.power = 80,
		.type = TYPE_WATER,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.split = SPLIT_SPECIAL,
		.z_move_power = 160,
		.z_move_effect = 0,
	},

	[MOVE_JAWLOCK] =
	{
		.effect = EFFECT_MEAN_LOOK,
		.power = 80,
		.type = TYPE_DARK,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.split = SPLIT_PHYSICAL,
		.z_move_power = 160,
		.z_move_effect = 0,
	},

	[MOVE_STUFFCHEEKS] =
	{
		.effect = EFFECT_DEFENSE_UP_2,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = FLAG_SNATCH_AFFECTED,
		.split = SPLIT_STATUS,
		.z_move_power = 0,
		.z_move_effect = Z_EFFECT_RESET_STATS,
	},

	[MOVE_NORETREAT] =
	{
		.effect = EFFECT_EXTREME_EVOBOOST,
		.power = 0,
		.type = TYPE_FIGHTING,
		.accuracy = 0,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = FLAG_SNATCH_AFFECTED,
		.split = SPLIT_STATUS,
		.z_move_power = 0,
		.z_move_effect = Z_EFFECT_RESET_STATS,
	},

	[MOVE_TARSHOT] =
	{
		.effect = EFFECT_SPEED_DOWN,
		.power = 0,
		.type = TYPE_ROCK,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.split = SPLIT_STATUS,
		.z_move_power = 0,
		.z_move_effect = Z_EFFECT_SPD_UP_1,
	},

	[MOVE_MAGICPOWDER] =
	{
		.effect = EFFECT_TYPE_CHANGES,
		.power = 0,
		.type = TYPE_PSYCHIC,
		.accuracy = 100,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.split = SPLIT_STATUS,
		.z_move_power = 0,
		.z_move_effect = Z_EFFECT_SPATK_UP_1,
	},

	[MOVE_DRAGONDARTS] =
	{
		.effect = 0,
		.power = 50,
		.type = TYPE_DRAGON,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.split = SPLIT_PHYSICAL,
		.z_move_power = 100,
		.z_move_effect = 0,
	},

	[MOVE_TEATIME] =
	{	//ANIM TODO
		.effect = EFFECT_TEATIME, //EFFECT TODO
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_STATUS,
		.z_move_power = 0,
		.z_move_effect = Z_EFFECT_ALL_STATS_UP_1,
	},

	[MOVE_OCTOLOCK] =
	{
		.effect = EFFECT_MEAN_LOOK,
		.power = 0,
		.type = TYPE_FIGHTING,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.split = SPLIT_STATUS,
		.z_move_power = 0,
		.z_move_effect = Z_EFFECT_ATK_UP_1,
	},

	[MOVE_BOLTBEAK] =
	{
		.effect = 0,
		.power = 85,
		.type = TYPE_ELECTRIC,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.split = SPLIT_PHYSICAL,
		.z_move_power = 160,
		.z_move_effect = 0,
	},

	[MOVE_FISHIOUSREND] =
	{
		.effect = 0,
		.power = 85,
		.type = TYPE_WATER,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.split = SPLIT_PHYSICAL,
		.z_move_power = 160,
		.z_move_effect = 0,
	},

	[MOVE_COURTCHANGE] =
	{
		.effect = EFFECT_FIELD_EFFECTS,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = FLAG_MIRROR_MOVE_AFFECTED,
		.split = SPLIT_STATUS,
		.z_move_power = 0,
		.z_move_effect = Z_EFFECT_SPD_UP_1,
	},

	[MOVE_CLANGOROUSSOUL] =
	{
		.effect = EFFECT_EXTREME_EVOBOOST,
		.power = 0,
		.type = TYPE_DRAGON,
		.accuracy = 100,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = FLAG_SNATCH_AFFECTED,
		.split = SPLIT_STATUS,
		.z_move_power = 0,
		.z_move_effect = Z_EFFECT_RESET_STATS,
	},

	[MOVE_BODYPRESS] =
	{
		.effect = 0,
		.power = 80,
		.type = TYPE_FIGHTING,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.split = SPLIT_PHYSICAL,
		.z_move_power = 160,
		.z_move_effect = 0,
	},

	[MOVE_DECORATE] =
	{
		.effect = EFFECT_HELPING_HAND,
		.power = 0,
		.type = TYPE_FAIRY,
		.accuracy = 0,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_STATUS,
		.z_move_power = 0,
		.z_move_effect = Z_EFFECT_RESET_STATS,
	},

	[MOVE_DRUMBEATING] =
	{
		.effect = EFFECT_SPEED_DOWN_HIT,
		.power = 80,
		.type = TYPE_GRASS,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.split = SPLIT_PHYSICAL,
		.z_move_power = 160,
		.z_move_effect = 0,
	},

	[MOVE_SNAPTRAP] =
	{
		.effect = EFFECT_TRAP,
		.power = 35,
		.type = TYPE_GRASS,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.split = SPLIT_PHYSICAL,
		.z_move_power = 100,
		.z_move_effect = 0,
	},

	[MOVE_PYROBALL] =
	{
		.effect = EFFECT_BURN_HIT,
		.power = 120,
		.type = TYPE_FIRE,
		.accuracy = 90,
		.pp = 5,
		.secondaryEffectChance = 10,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.split = SPLIT_PHYSICAL,
		.z_move_power = 190,
		.z_move_effect = 0,
	},

	[MOVE_BEHEMOTHBLADE] =
	{
		.effect = 0,
		.power = 100,
		.type = TYPE_STEEL,
		.accuracy = 100,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.split = SPLIT_PHYSICAL,
		.z_move_power = 180,
		.z_move_effect = 0,
	},

	[MOVE_BEHEMOTHBASH] =
	{
		.effect = 0,
		.power = 100,
		.type = TYPE_STEEL,
		.accuracy = 100,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.split = SPLIT_PHYSICAL,
		.z_move_power = 180,
		.z_move_effect = 0,
	},

	[MOVE_AURAWHEEL] =
	{
		.effect = EFFECT_SPEED_UP_1_HIT,
		.power = 110,
		.type = TYPE_ELECTRIC,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.split = SPLIT_PHYSICAL,
		.z_move_power = 185,
		.z_move_effect = 0,
	},

	[MOVE_BREAKINGSWIPE] =
	{
		.effect = EFFECT_ATTACK_DOWN_HIT,
		.power = 60,
		.type = TYPE_DRAGON,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_BOTH,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.split = SPLIT_PHYSICAL,
		.z_move_power = 120,
		.z_move_effect = 0,
	},

	[MOVE_BRANCHPOKE] =
	{
		.effect = 0,
		.power = 40,
		.type = TYPE_GRASS,
		.accuracy = 100,
		.pp = 40,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.split = SPLIT_PHYSICAL,
		.z_move_power = 100,
		.z_move_effect = 0,
	},

	[MOVE_OVERDRIVE] =
	{
		.effect = 0,
		.power = 80,
		.type = TYPE_ELECTRIC,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_BOTH,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.split = SPLIT_SPECIAL,
		.z_move_power = 160,
		.z_move_effect = 0,
	},

	[MOVE_APPLEACID] =
	{
		.effect = EFFECT_SPECIAL_DEFENSE_DOWN_HIT,
		.power = 80,
		.type = TYPE_GRASS,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.split = SPLIT_SPECIAL,
		.z_move_power = 160,
		.z_move_effect = 0,
	},

	[MOVE_GRAVAPPLE] =
	{
		.effect = EFFECT_DEFENSE_DOWN_HIT,
		.power = 80,
		.type = TYPE_GRASS,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.split = SPLIT_PHYSICAL,
		.z_move_power = 160,
		.z_move_effect = 0,
	},

	[MOVE_SPIRITBREAK] =
	{
		.effect = EFFECT_SPECIAL_ATTACK_DOWN_HIT,
		.power = 75,
		.type = TYPE_FAIRY,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.split = SPLIT_PHYSICAL,
		.z_move_power = 140,
		.z_move_effect = 0,
	},

	[MOVE_STRANGESTEAM] =
	{
		.effect = EFFECT_CONFUSE_HIT,
		.power = 90,
		.type = TYPE_FAIRY,
		.accuracy = 95,
		.pp = 10,
		.secondaryEffectChance = 20,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.split = SPLIT_SPECIAL,
		.z_move_power = 175,
		.z_move_effect = 0,
	},

	[MOVE_LIFEDEW] =
	{
		.effect = EFFECT_RESTORE_HP,
		.power = 0,
		.type = TYPE_WATER,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = FLAG_SNATCH_AFFECTED | FLAG_TRIAGE_AFFECTED,
		.split = SPLIT_STATUS,
		.z_move_power = 0,
		.z_move_effect = Z_EFFECT_RESET_STATS,
	},

	[MOVE_OBSTRUCT] =
	{
		.effect = EFFECT_PROTECT,
		.power = 0,
		.type = TYPE_DARK,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 4,
		.flags = 0,
		.split = SPLIT_STATUS,
		.z_move_power = 0,
		.z_move_effect = Z_EFFECT_DEF_UP_1,
	},

	[MOVE_FALSESURRENDER] =
	{
		.effect = 0,
		.power = 80,
		.type = TYPE_DARK,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.split = SPLIT_PHYSICAL,
		.z_move_power = 160,
		.z_move_effect = 0,
	},

	[MOVE_METEORASSAULT] =
	{
		.effect = EFFECT_RECHARGE,
		.power = 150,
		.type = TYPE_FIGHTING,
		.accuracy = 100,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.split = SPLIT_PHYSICAL,
		.z_move_power = 200,
		.z_move_effect = 0,
	},

	[MOVE_ETERNABEAM] =
	{
		.effect = EFFECT_RECHARGE,
		.power = 160,
		.type = TYPE_DRAGON,
		.accuracy = 90,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.split = SPLIT_SPECIAL,
		.z_move_power = 200,
		.z_move_effect = 0,
	},

	[MOVE_STEELBEAM] =
	{
		.effect = 0,
		.power = 140,
		.type = TYPE_STEEL,
		.accuracy = 95,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.split = SPLIT_SPECIAL,
		.z_move_power = 200,
		.z_move_effect = 0,
	},

	[MOVE_EXPANDINGFORCE] =
	{
		.effect = EFFECT_HIT,
		.power = 80,
		.type = TYPE_PSYCHIC,
		.accuracy = 100,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 160,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_STEELROLLER] =
	{
		.effect = EFFECT_DAMAGE_SET_TERRAIN,
		.power = 130,
		.type = TYPE_STEEL,
		.accuracy = 100,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 195,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_SCALESHOT] =
	{
		.effect = EFFECT_MULTI_HIT,
		.power = 25,
		.type = TYPE_DRAGON,
		.accuracy = 90,
		.pp = 25,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_METEORBEAM] =
	{
		.effect = EFFECT_SKULL_BASH,
		.power = 120,
		.type = TYPE_ROCK,
		.accuracy = 90,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 190,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_SHELLSIDEARM] =
	{
		.effect = EFFECT_HIT,
		.power = 90,
		.type = TYPE_POISON,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 10,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 175,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_MISTYEXPLOSION] =
	{
		.effect = EFFECT_EXPLOSION,
		.power = 100,
		.type = TYPE_FAIRY,
		.accuracy = 100,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_FOES_AND_ALLY,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 180,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_GRASSYGLIDE] =
	{
		.effect = EFFECT_HIT,
		.power = 70,
		.type = TYPE_GRASS,
		.accuracy = 100,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 140,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_RISINGVOLTAGE] =
	{
		.effect = EFFECT_HIT,
		.power = 70,
		.type = TYPE_ELECTRIC,
		.accuracy = 100,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 140,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_TERRAINPULSE] =
	{
		.effect = EFFECT_HIT,
		.power = 50,
		.type = TYPE_NORMAL,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 160,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_SKITTERSMACK] =
	{
		.effect = EFFECT_SPECIAL_ATTACK_DOWN_HIT,
		.power = 70,
		.type = TYPE_BUG,
		.accuracy = 90,
		.pp = 10,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 140,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_BURNINGJEALOUSY] =
	{
		.effect = EFFECT_BURN_HIT,
		.power = 70,
		.type = TYPE_FIRE,
		.accuracy = 100,
		.pp = 5,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_BOTH,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 140,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_LASHOUT] =
	{
		.effect = EFFECT_HIT,
		.power = 75,
		.type = TYPE_DARK,
		.accuracy = 100,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 140,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_POLTERGEIST] =
	{
		.effect = EFFECT_POLTERGEIST,
		.power = 110,
		.type = TYPE_GHOST,
		.accuracy = 90,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 185,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_CORROSIVEGAS] =
	{
		.effect = EFFECT_KNOCK_OFF,
		.power = 0,
		.type = TYPE_POISON,
		.accuracy = 100,
		.pp = 40,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MIRROR_MOVE_AFFECTED | FLAG_MAGIC_COAT_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = 0
	},

	[MOVE_COACHING] =
	{
		.effect = EFFECT_HELPING_HAND,
		.power = 0,
		.type = TYPE_FIGHTING,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_RESET_STATS
	},

	[MOVE_FLIPTURN] =
	{
		.effect = EFFECT_BATON_PASS,
		.power = 60,
		.type = TYPE_WATER,
		.accuracy = 100,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 120,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_TRIPLEAXEL] =
	{
		.effect = EFFECT_TRIPLE_KICK,
		.power = 20,
		.type = TYPE_ICE,
		.accuracy = 90,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_DUALWINGBEAT] =
	{
		.effect = EFFECT_DOUBLE_HIT,
		.power = 40,
		.type = TYPE_FLYING,
		.accuracy = 90,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_SCORCHINGSANDS] =
	{
		.effect = EFFECT_BURN_HIT,
		.power = 70,
		.type = TYPE_GROUND,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 30,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 140,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_JUNGLEHEALING] =
	{
		.effect = EFFECT_RESTORE_HP,
		.power = 0,
		.type = TYPE_GRASS,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = FLAG_TRIAGE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_RESET_STATS
	},

	[MOVE_WICKEDBLOW] =
	{
		.effect = EFFECT_HIT,
		.power = 80,
		.type = TYPE_DARK,
		.accuracy = 100,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 160,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_SURGINGSTRIKES] =
	{
		.effect = EFFECT_MULTI_HIT,
		.power = 25,
		.type = TYPE_WATER,
		.accuracy = 100,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_FREEZINGGLARE] =
	{
		.effect = EFFECT_FREEZE_HIT,
		.power = 90,
		.type = TYPE_PSYCHIC,
		.accuracy = 100,
		.pp = 10,
		#ifndef FROSTBITE
		.secondaryEffectChance = 10,
		#else
		.secondaryEffectChance = 20,
		#endif
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 175,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_THUNDEROUSKICK] =
	{
		.effect = EFFECT_DEFENSE_DOWN_HIT,
		.power = 90,
		.type = TYPE_FIGHTING,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 175,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_FIERYWRATH] =
	{
		.effect = EFFECT_FLINCH_HIT,
		.power = 90,
		.type = TYPE_DARK,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 20,
		.target = MOVE_TARGET_BOTH,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 175,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_EERIESPELL] =
	{
		.effect = EFFECT_SPITE,
		.power = 80,
		.type = TYPE_PSYCHIC,
		.accuracy = 90,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 160,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},
	
	[MOVE_THUNDERCAGE] =
	{
		.effect = EFFECT_TRAP,
		.power = 80,
		.type = TYPE_ELECTRIC,
		.accuracy = 90,
		.pp = 15,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 160,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_DRAGONENERGY] =
	{
		.effect = EFFECT_HIT,
		.power = 150,
		.type = TYPE_DRAGON,
		.accuracy = 100,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_BOTH,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 200,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_ASTRALBARRAGE] =
	{	//ANIM TODO
		.effect = EFFECT_HIT,
		.power = 120,
		.type = TYPE_GHOST,
		.accuracy = 100,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_BOTH,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 190,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_GLACIALLANCE] =
	{
		.effect = EFFECT_HIT,
		.power = 130,
		.type = TYPE_ICE,
		.accuracy = 100,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_BOTH,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 195,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_DIRECLAW] =
	{
		.effect = EFFECT_TRI_ATTACK,
		.power = 60,
		.type = TYPE_POISON,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 50,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 120,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_PSYSHIELDBASH] =
	{
		.effect = EFFECT_DEFENSE_UP_HIT,
		.power = 70,
		.type = TYPE_PSYCHIC,
		.accuracy = 90,
		.pp = 10,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 140,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_POWERSHIFT] =
	{
		.effect = EFFECT_STAT_SWAP_SPLIT,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = FLAG_SNATCH_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_ATK_UP_1
	},

	[MOVE_STONEAXE] =
	{
		.effect = EFFECT_SPLINTER,
		#ifdef ACTUAL_PLA_MOVE_POWERS
		.power = 65,
		.z_move_power = 120,
		#else
		.power = 80,
		.z_move_power = 160,
		#endif
		.type = TYPE_ROCK,
		.accuracy = 90,
		.pp = 15,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_MYSTICALPOWER] =
	{
		.effect = EFFECT_HIGHER_OFFENSES_DEFENSES_UP_HIT, //Atk/Sp. Atk up, or Def./Sp.Def up
		.power = 70,
		.type = TYPE_PSYCHIC,
		.accuracy = 90,
		.pp = 10,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 140,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_RAGINGFURY] =
	{
		.effect = EFFECT_RAMPAGE,
		#ifdef ACTUAL_PLA_MOVE_POWERS
		.power = 90,
		.z_move_power = 175,
		#else
		.power = 120, //Other rampage moves also had power reduced to 90 in PLA
		.z_move_power = 190,
		#endif
		.type = TYPE_FIRE,
		.accuracy = 85,
		.pp = 10,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_RANDOM,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_WAVECRASH] =
	{
		.effect = EFFECT_HIT,
		.power = 75,
		.type = TYPE_WATER,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 1, //"Raises user's action speed" is said about PLA's equivalent to Priority moves
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 140,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_CHLOROBLAST] =
	{
		.effect = EFFECT_HIT, //Has the same recoil as Mind Blown and Steel Beam
		#ifdef ACTUAL_PLA_MOVE_POWERS
		.power = 120,
		.z_move_power = 190,
		#else
		.power = 150,
		.z_move_power = 200,
		#endif
		.type = TYPE_GRASS,
		.accuracy = 95,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_MOUNTAINGALE] =
	{
		.effect = EFFECT_FLINCH_HIT,
		#ifdef ACTUAL_PLA_MOVE_POWERS
		.power = 100,
		.z_move_power = 180,
		#else
		.power = 110,
		.z_move_power = 185,
		#endif
		.type = TYPE_ICE,
		.accuracy = 85,
		.pp = 5,
		.secondaryEffectChance = 30,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_VICTORYDANCE] =
	{
		.effect = EFFECT_BULK_UP,
		.power = 0,
		.type = TYPE_FIGHTING,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = FLAG_SNATCH_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_RESET_STATS
	},

	[MOVE_HEADLONGRUSH] =
	{
		.effect = EFFECT_SUPERPOWER, //-Def, -Sp. Def
		#ifdef ACTUAL_PLA_MOVE_POWERS
		.power = 100,
		.z_move_power = 180,
		#else
		.power = 120,
		.z_move_power = 190,
		#endif
		.type = TYPE_GROUND,
		.accuracy = 100,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_BARBBARRAGE] =
	{
		.effect = EFFECT_POISON_HIT, //Also double damage on poisoned foes
		#ifdef ACTUAL_PLA_MOVE_POWERS
		.power = 60,
		.z_move_power = 120,
		#else
		.power = 75,
		.z_move_power = 140,
		#endif
		.type = TYPE_POISON,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 30,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_ESPERWING] =
	{
		.effect = EFFECT_HIT,
		.power = 75,
		.type = TYPE_PSYCHIC,
		.accuracy = 90,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 1, //"Raises user's action speed" is said about PLA's equivalent to Priority moves
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 140,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_BITTERMALICE] =
	{
		.effect = EFFECT_FREEZE_HIT, //Also double damage on frozen foes
		.power = 60,
		.type = TYPE_GHOST,
		.accuracy = 100,
		.pp = 15,
		#ifndef FROSTBITE
		.secondaryEffectChance = 20, //30% Freeze chance is way too OP
		#else
		.secondaryEffectChance = 30,
		#endif
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 120,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_SHELTER] =
	{
		.effect = EFFECT_DEF_SPD_UP,
		.power = 0,
		.type = TYPE_STEEL,
		.accuracy = 0,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = FLAG_SNATCH_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_SPDEF_UP_1
	},

	[MOVE_TRIPLEARROWS] =
	{
		.effect = EFFECT_DEFENSE_DOWN_HIT, //Also raises crit chance
		#ifdef ACTUAL_PLA_MOVE_POWERS
		.power = 50,
		.z_move_power = 100,
		#else
		.power = 60,
		.z_move_power = 120,
		#endif
		.type = TYPE_FIGHTING,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_INFERNALPARADE] =
	{
		.effect = EFFECT_BURN_HIT, //Also double damage on burned foes
		#ifdef ACTUAL_PLA_MOVE_POWERS
		.power = 60,
		.z_move_power = 120,
		#else
		.power = 75,
		.z_move_power = 140,
		#endif
		.type = TYPE_GHOST,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 30,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_CEASELESSEDGE] =
	{
		.effect = EFFECT_SPLINTER,
		#ifdef ACTUAL_PLA_MOVE_POWERS
		.power = 65,
		.z_move_power = 120,
		#else
		.power = 80,
		.z_move_power = 160,
		#endif
		.type = TYPE_DARK,
		.accuracy = 90,
		.pp = 15,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_BLEAKWINDSTORM] =
	{
		.effect = EFFECT_FREEZE_HIT,
		#ifdef ACTUAL_PLA_MOVE_POWERS
		.power = 95,
		.z_move_power = 175,
		#else
		.power = 105,
		.z_move_power = 180,
		#endif
		.type = TYPE_FLYING,
		.accuracy = 80,
		.pp = 5,
		#ifndef FROSTBITE
		.secondaryEffectChance = 20, //30% Freeze chance is way too OP
		#else
		.secondaryEffectChance = 30,
		#endif
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_WILDBOLTSTORM] =
	{
		.effect = EFFECT_PARALYZE_HIT,
		#ifdef ACTUAL_PLA_MOVE_POWERS
		.power = 95,
		.z_move_power = 175,
		#else
		.power = 105,
		.z_move_power = 180,
		#endif
		.type = TYPE_ELECTRIC,
		.accuracy = 80,
		.pp = 5,
		.secondaryEffectChance = 30,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_SANDSEARSTORM] =
	{
		.effect = EFFECT_BURN_HIT,
		#ifdef ACTUAL_PLA_MOVE_POWERS
		.power = 95,
		.z_move_power = 175,
		#else
		.power = 105,
		.z_move_power = 180,
		#endif
		.type = TYPE_GROUND,
		.accuracy = 80,
		.pp = 5,
		.secondaryEffectChance = 30,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_SPRINGTIDESTORM] =
	{
		.effect = EFFECT_SPRINGTIDE_STORM, //Omniboost for Incarnate, Lower foe Sp. Def for Therian
		#ifdef ACTUAL_PLA_MOVE_POWERS
		.power = 95,
		.z_move_power = 175,
		#else
		.power = 105,
		.z_move_power = 180,
		#endif
		.type = TYPE_FAIRY,
		.accuracy = 80,
		.pp = 5,
		.secondaryEffectChance = 30,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_LUNARBLESSING] =
	{
		.effect = EFFECT_RESTORE_HP,
		.power = 0,
		.type = TYPE_PSYCHIC,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = FLAG_TRIAGE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_RESET_STATS
	},

	[MOVE_TAKEHEART] =
	{
		.effect = EFFECT_CALM_MIND, //Also heals status conditions
		.power = 0,
		.type = TYPE_PSYCHIC,
		.accuracy = 0,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = FLAG_SNATCH_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_RECOVER_HP
	},
	//Z-Moves//
	[MOVE_BREAKNECK_BLITZ_P] =
	{
		.effect = EFFECT_HIT,
		.power = 1,
		.type = TYPE_NORMAL,
		.accuracy = 0,
		.pp = 1,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_BREAKNECK_BLITZ_S] =
	{
		.effect = EFFECT_HIT,
		.power = 1,
		.type = TYPE_NORMAL,
		.accuracy = 0,
		.pp = 1,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_ALL_OUT_PUMMELING_P] =
	{
		.effect = EFFECT_HIT,
		.power = 1,
		.type = TYPE_FIGHTING,
		.accuracy = 0,
		.pp = 1,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_ALL_OUT_PUMMELING_S] =
	{
		.effect = EFFECT_HIT,
		.power = 1,
		.type = TYPE_FIGHTING,
		.accuracy = 0,
		.pp = 1,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_SUPERSONIC_SKYSTRIKE_P] =
	{
		.effect = EFFECT_HIT,
		.power = 1,
		.type = TYPE_FLYING,
		.accuracy = 0,
		.pp = 1,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_SUPERSONIC_SKYSTRIKE_S] =
	{
		.effect = EFFECT_HIT,
		.power = 1,
		.type = TYPE_FLYING,
		.accuracy = 0,
		.pp = 1,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_ACID_DOWNPOUR_P] =
	{
		.effect = EFFECT_HIT,
		.power = 1,
		.type = TYPE_POISON,
		.accuracy = 0,
		.pp = 1,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_ACID_DOWNPOUR_S] =
	{
		.effect = EFFECT_HIT,
		.power = 1,
		.type = TYPE_POISON,
		.accuracy = 0,
		.pp = 1,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_TECTONIC_RAGE_P] =
	{
		.effect = EFFECT_HIT,
		.power = 1,
		.type = TYPE_GROUND,
		.accuracy = 0,
		.pp = 1,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_TECTONIC_RAGE_S] =
	{
		.effect = EFFECT_HIT,
		.power = 1,
		.type = TYPE_GROUND,
		.accuracy = 0,
		.pp = 1,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_CONTINENTAL_CRUSH_P] =
	{
		.effect = EFFECT_HIT,
		.power = 1,
		.type = TYPE_ROCK,
		.accuracy = 0,
		.pp = 1,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_CONTINENTAL_CRUSH_S] =
	{
		.effect = EFFECT_HIT,
		.power = 1,
		.type = TYPE_ROCK,
		.accuracy = 0,
		.pp = 1,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_SAVAGE_SPIN_OUT_P] =
	{
		.effect = EFFECT_HIT,
		.power = 1,
		.type = TYPE_BUG,
		.accuracy = 0,
		.pp = 1,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_SAVAGE_SPIN_OUT_S] =
	{
		.effect = EFFECT_HIT,
		.power = 1,
		.type = TYPE_BUG,
		.accuracy = 0,
		.pp = 1,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_NEVER_ENDING_NIGHTMARE_P] =
	{
		.effect = EFFECT_HIT,
		.power = 1,
		.type = TYPE_GHOST,
		.accuracy = 0,
		.pp = 1,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_NEVER_ENDING_NIGHTMARE_S] =
	{
		.effect = EFFECT_HIT,
		.power = 1,
		.type = TYPE_GHOST,
		.accuracy = 0,
		.pp = 1,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_CORKSCREW_CRASH_P] =
	{
		.effect = EFFECT_HIT,
		.power = 1,
		.type = TYPE_STEEL,
		.accuracy = 0,
		.pp = 1,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_CORKSCREW_CRASH_S] =
	{
		.effect = EFFECT_HIT,
		.power = 1,
		.type = TYPE_STEEL,
		.accuracy = 0,
		.pp = 1,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_INFERNO_OVERDRIVE_P] =
	{
		.effect = EFFECT_HIT,
		.power = 1,
		.type = TYPE_FIRE,
		.accuracy = 0,
		.pp = 1,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_INFERNO_OVERDRIVE_S] =
	{
		.effect = EFFECT_HIT,
		.power = 1,
		.type = TYPE_FIRE,
		.accuracy = 0,
		.pp = 1,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_HYDRO_VORTEX_P] =
	{
		.effect = EFFECT_HIT,
		.power = 1,
		.type = TYPE_WATER,
		.accuracy = 0,
		.pp = 1,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_HYDRO_VORTEX_S] =
	{
		.effect = EFFECT_HIT,
		.power = 1,
		.type = TYPE_WATER,
		.accuracy = 0,
		.pp = 1,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_BLOOM_DOOM_P] =
	{
		.effect = EFFECT_HIT,
		.power = 1,
		.type = TYPE_GRASS,
		.accuracy = 0,
		.pp = 1,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_BLOOM_DOOM_S] =
	{
		.effect = EFFECT_HIT,
		.power = 1,
		.type = TYPE_GRASS,
		.accuracy = 0,
		.pp = 1,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_GIGAVOLT_HAVOC_P] =
	{
		.effect = EFFECT_HIT,
		.power = 1,
		.type = TYPE_ELECTRIC,
		.accuracy = 0,
		.pp = 1,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_GIGAVOLT_HAVOC_S] =
	{
		.effect = EFFECT_HIT,
		.power = 1,
		.type = TYPE_ELECTRIC,
		.accuracy = 0,
		.pp = 1,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_SHATTERED_PSYCHE_P] =
	{
		.effect = EFFECT_HIT,
		.power = 1,
		.type = TYPE_PSYCHIC,
		.accuracy = 0,
		.pp = 1,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_SHATTERED_PSYCHE_S] =
	{
		.effect = EFFECT_HIT,
		.power = 1,
		.type = TYPE_PSYCHIC,
		.accuracy = 0,
		.pp = 1,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_SUBZERO_SLAMMER_P] =
	{
		.effect = EFFECT_HIT,
		.power = 1,
		.type = TYPE_ICE,
		.accuracy = 0,
		.pp = 1,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_SUBZERO_SLAMMER_S] =
	{
		.effect = EFFECT_HIT,
		.power = 1,
		.type = TYPE_ICE,
		.accuracy = 0,
		.pp = 1,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_DEVASTATING_DRAKE_P] =
	{
		.effect = EFFECT_HIT,
		.power = 1,
		.type = TYPE_DRAGON,
		.accuracy = 0,
		.pp = 1,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_DEVASTATING_DRAKE_S] =
	{
		.effect = EFFECT_HIT,
		.power = 1,
		.type = TYPE_DRAGON,
		.accuracy = 0,
		.pp = 1,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_BLACK_HOLE_ECLIPSE_P] =
	{
		.effect = EFFECT_HIT,
		.power = 1,
		.type = TYPE_DARK,
		.accuracy = 0,
		.pp = 1,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_BLACK_HOLE_ECLIPSE_S] =
	{
		.effect = EFFECT_HIT,
		.power = 1,
		.type = TYPE_DARK,
		.accuracy = 0,
		.pp = 1,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_TWINKLE_TACKLE_P] =
	{
		.effect = EFFECT_HIT,
		.power = 1,
		.type = TYPE_FAIRY,
		.accuracy = 0,
		.pp = 1,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_TWINKLE_TACKLE_S] =
	{
		.effect = EFFECT_HIT,
		.power = 1,
		.type = TYPE_FAIRY,
		.accuracy = 0,
		.pp = 1,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_CATASTROPIKA] =
	{
		.effect = EFFECT_HIT,
		.power = 210,
		.type = TYPE_ELECTRIC,
		.accuracy = 0,
		.pp = 1,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_10000000_VOLT_THUNDERBOLT] =
	{
		.effect = EFFECT_HIGH_CRITICAL,
		.power = 195,
		.type = TYPE_ELECTRIC,
		.accuracy = 0,
		.pp = 1,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_STOKED_SPARKSURFER] =
	{
		.effect = EFFECT_PARALYZE_HIT,
		.power = 175,
		.type = TYPE_ELECTRIC,
		.accuracy = 0,
		.pp = 1,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_EXTREME_EVOBOOST] =
	{
		.effect = EFFECT_EXTREME_EVOBOOST,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 0,
		.pp = 1,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = 0
	},

	[MOVE_PULVERIZING_PANCAKE] =
	{
		.effect = EFFECT_HIT,
		.power = 210,
		.type = TYPE_NORMAL,
		.accuracy = 0,
		.pp = 1,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_GENESIS_SUPERNOVA] =
	{
		.effect = EFFECT_DAMAGE_SET_TERRAIN,
		.power = 185,
		.type = TYPE_PSYCHIC,
		.accuracy = 0,
		.pp = 1,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_SINISTER_ARROW_RAID] =
	{
		.effect = EFFECT_HIT,
		.power = 180,
		.type = TYPE_GHOST,
		.accuracy = 0,
		.pp = 1,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_MALICIOUS_MOONSAULT] =
	{
		.effect = EFFECT_HIT,
		.power = 180,
		.type = TYPE_DARK,
		.accuracy = 0,
		.pp = 1,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_OCEANIC_OPERETTA] =
	{
		.effect = EFFECT_HIT,
		.power = 195,
		.type = TYPE_WATER,
		.accuracy = 0,
		.pp = 1,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_SPLINTERED_STORMSHARDS] =
	{
		.effect = EFFECT_DAMAGE_SET_TERRAIN, //"Sets" the terrain to NONE
		.power = 190,
		.type = TYPE_ROCK,
		.accuracy = 0,
		.pp = 1,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_LETS_SNUGGLE_FOREVER] =
	{
		.effect = EFFECT_HIT,
		.power = 190,
		.type = TYPE_FAIRY,
		.accuracy = 0,
		.pp = 1,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_CLANGOROUS_SOULBLAZE] =
	{
		.effect = EFFECT_ALL_STATS_UP_HIT,
		.power = 185,
		.type = TYPE_DRAGON,
		.accuracy = 0,
		.pp = 1,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_BOTH,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_GUARDIAN_OF_ALOLA] =
	{
		.effect = EFFECT_SUPER_FANG,
		.power = 1,
		.type = TYPE_FAIRY,
		.accuracy = 0,
		.pp = 1,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_SEARING_SUNRAZE_SMASH] =
	{
		.effect = EFFECT_HIT,
		.power = 200,
		.type = TYPE_STEEL,
		.accuracy = 0,
		.pp = 1,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_MENACING_MOONRAZE_MAELSTROM] =
	{
		.effect = EFFECT_HIT,
		.power = 200,
		.type = TYPE_GHOST,
		.accuracy = 0,
		.pp = 1,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_LIGHT_THAT_BURNS_THE_SKY] =
	{
		.effect = EFFECT_HIT,
		.power = 200,
		.type = TYPE_PSYCHIC,
		.accuracy = 0,
		.pp = 1,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_SOUL_STEALING_7_STAR_STRIKE] =
	{
		.effect = EFFECT_HIT,
		.power = 195,
		.type = TYPE_GHOST,
		.accuracy = 0,
		.pp = 1,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

#ifdef DYNAMAX_FEATURE
	[MOVE_MAX_GUARD] =
	{
		.effect = EFFECT_PROTECT,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 4,
		.flags = 0,
		.split = SPLIT_STATUS,
		.z_move_power = 0,
		.z_move_effect = 0,
	},

	[MOVE_MAX_FLARE_P] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 1,
		.type = TYPE_FIRE,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_PHYSICAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_SUN,
	},

	[MOVE_MAX_FLARE_S] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 1,
		.type = TYPE_FIRE,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_SPECIAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_SUN,
	},

	[MOVE_MAX_FLUTTERBY_P] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 1,
		.type = TYPE_BUG,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_PHYSICAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_LOWER_SP_ATK,
	},

	[MOVE_MAX_FLUTTERBY_S] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 1,
		.type = TYPE_BUG,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_SPECIAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_LOWER_SP_ATK,
	},

	[MOVE_MAX_LIGHTNING_P] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 1,
		.type = TYPE_ELECTRIC,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_PHYSICAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_ELECTRIC_TERRAIN,
	},

	[MOVE_MAX_LIGHTNING_S] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 1,
		.type = TYPE_ELECTRIC,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_SPECIAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_ELECTRIC_TERRAIN,
	},

	[MOVE_MAX_STRIKE_P] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 1,
		.type = TYPE_NORMAL,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_PHYSICAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_LOWER_SPEED,
	},

	[MOVE_MAX_STRIKE_S] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 1,
		.type = TYPE_NORMAL,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_SPECIAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_LOWER_SPEED,
	},

	[MOVE_MAX_KNUCKLE_P] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 1,
		.type = TYPE_FIGHTING,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_PHYSICAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_RAISE_TEAM_ATTACK,
	},

	[MOVE_MAX_KNUCKLE_S] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 1,
		.type = TYPE_FIGHTING,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_SPECIAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_RAISE_TEAM_ATTACK,
	},

	[MOVE_MAX_PHANTASM_P] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 1,
		.type = TYPE_GHOST,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_PHYSICAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_LOWER_DEFENSE,
	},

	[MOVE_MAX_PHANTASM_S] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 1,
		.type = TYPE_GHOST,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_SPECIAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_LOWER_DEFENSE,
	},

	[MOVE_MAX_HAILSTORM_P] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 1,
		.type = TYPE_ICE,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_PHYSICAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_HAIL,
	},

	[MOVE_MAX_HAILSTORM_S] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 1,
		.type = TYPE_ICE,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_SPECIAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_HAIL,
	},

	[MOVE_MAX_OOZE_P] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 1,
		.type = TYPE_POISON,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_PHYSICAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_RAISE_TEAM_SP_ATK,
	},

	[MOVE_MAX_OOZE_S] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 1,
		.type = TYPE_POISON,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_SPECIAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_RAISE_TEAM_SP_ATK,
	},

	[MOVE_MAX_GEYSER_P] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 1,
		.type = TYPE_WATER,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_PHYSICAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_RAIN,
	},

	[MOVE_MAX_GEYSER_S] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 1,
		.type = TYPE_WATER,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_SPECIAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_RAIN,
	},

	[MOVE_MAX_AIRSTREAM_P] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 1,
		.type = TYPE_FLYING,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_PHYSICAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_RAISE_TEAM_SPEED,
	},

	[MOVE_MAX_AIRSTREAM_S] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 1,
		.type = TYPE_FLYING,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_SPECIAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_RAISE_TEAM_SPEED,
	},

	[MOVE_MAX_STARFALL_P] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 1,
		.type = TYPE_FAIRY,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_PHYSICAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_MISTY_TERRAIN,
	},

	[MOVE_MAX_STARFALL_S] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 1,
		.type = TYPE_FAIRY,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_SPECIAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_MISTY_TERRAIN,
	},

	[MOVE_MAX_WYRMWIND_P] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 1,
		.type = TYPE_DRAGON,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_PHYSICAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_LOWER_ATTACK,
	},

	[MOVE_MAX_WYRMWIND_S] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 1,
		.type = TYPE_DRAGON,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_SPECIAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_LOWER_ATTACK,
	},

	[MOVE_MAX_MINDSTORM_P] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 1,
		.type = TYPE_PSYCHIC,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_PHYSICAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_PSYCHIC_TERRAIN,
	},

	[MOVE_MAX_MINDSTORM_S] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 1,
		.type = TYPE_PSYCHIC,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_SPECIAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_PSYCHIC_TERRAIN,
	},

	[MOVE_MAX_ROCKFALL_P] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 1,
		.type = TYPE_ROCK,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_PHYSICAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_SANDSTORM,
	},

	[MOVE_MAX_ROCKFALL_S] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 1,
		.type = TYPE_ROCK,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_SPECIAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_SANDSTORM,
	},

	[MOVE_MAX_QUAKE_P] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 1,
		.type = TYPE_GROUND,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_PHYSICAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_RAISE_TEAM_SP_DEF,
	},

	[MOVE_MAX_QUAKE_S] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 1,
		.type = TYPE_GROUND,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_SPECIAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_RAISE_TEAM_SP_DEF,
	},

	[MOVE_MAX_DARKNESS_P] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 1,
		.type = TYPE_DARK,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_PHYSICAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_LOWER_SP_DEF,
	},

	[MOVE_MAX_DARKNESS_S] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 1,
		.type = TYPE_DARK,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_SPECIAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_LOWER_SP_DEF,
	},

	[MOVE_MAX_OVERGROWTH_P] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 10,
		.type = TYPE_GRASS,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_PHYSICAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_GRASSY_TERRAIN,
	},

	[MOVE_MAX_OVERGROWTH_S] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 10,
		.type = TYPE_GRASS,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_SPECIAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_GRASSY_TERRAIN,
	},

	[MOVE_MAX_STEELSPIKE_P] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 10,
		.type = TYPE_STEEL,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_PHYSICAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_RAISE_TEAM_DEFENSE,
	},

	[MOVE_MAX_STEELSPIKE_S] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 10,
		.type = TYPE_STEEL,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_SPECIAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_RAISE_TEAM_DEFENSE,
	},

	[MOVE_G_MAX_VINE_LASH_P] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 10,
		.type = TYPE_GRASS,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_PHYSICAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_VINE_LASH,
	},
	
	[MOVE_G_MAX_VINE_LASH_S] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 10,
		.type = TYPE_GRASS,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_SPECIAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_VINE_LASH,
	},
	
	[MOVE_G_MAX_WILDFIRE_P] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 10,
		.type = TYPE_FIRE,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_PHYSICAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_WILDFIRE,
	},

	[MOVE_G_MAX_WILDFIRE_S] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 10,
		.type = TYPE_FIRE,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_SPECIAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_WILDFIRE,
	},
	
	[MOVE_G_MAX_CANNONADE_P] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 10,
		.type = TYPE_WATER,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_PHYSICAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_CANNONADE,
	},
	
	[MOVE_G_MAX_CANNONADE_S] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 10,
		.type = TYPE_WATER,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_SPECIAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_CANNONADE,
	},
	
	[MOVE_G_MAX_BEFUDDLE_P] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 10,
		.type = TYPE_BUG,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_PHYSICAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_EFFECT_SPORE_FOES,
	},

	[MOVE_G_MAX_BEFUDDLE_S] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 10,
		.type = TYPE_BUG,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_SPECIAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_EFFECT_SPORE_FOES,
	},

	[MOVE_G_MAX_VOLT_CRASH_P] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 10,
		.type = TYPE_ELECTRIC,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_PHYSICAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_PARALYZE_FOES,
	},

	[MOVE_G_MAX_VOLT_CRASH_S] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 10,
		.type = TYPE_ELECTRIC,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_SPECIAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_PARALYZE_FOES,
	},

	[MOVE_G_MAX_GOLD_RUSH_P] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 10,
		.type = TYPE_NORMAL,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_PHYSICAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_CONFUSE_FOES_PAY_DAY,
	},

	[MOVE_G_MAX_GOLD_RUSH_S] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 10,
		.type = TYPE_NORMAL,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_SPECIAL,
		.z_move_power = 100,
		.z_move_effect = MAX_EFFECT_CONFUSE_FOES_PAY_DAY,
	},

	[MOVE_G_MAX_CHI_STRIKE_P] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 10,
		.type = TYPE_FIGHTING,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_PHYSICAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_CRIT_PLUS,
	},

	[MOVE_G_MAX_CHI_STRIKE_S] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 10,
		.type = TYPE_FIGHTING,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_SPECIAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_CRIT_PLUS,
	},

	[MOVE_G_MAX_TERROR_P] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 10,
		.type = TYPE_GHOST,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_PHYSICAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_MEAN_LOOK,
	},

	[MOVE_G_MAX_TERROR_S] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 10,
		.type = TYPE_GHOST,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_SPECIAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_MEAN_LOOK,
	},

	[MOVE_G_MAX_FOAM_BURST_P] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 10,
		.type = TYPE_WATER,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_PHYSICAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_LOWER_SPEED_2_FOES,
	},

	[MOVE_G_MAX_FOAM_BURST_S] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 10,
		.type = TYPE_WATER,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_SPECIAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_LOWER_SPEED_2_FOES,
	},

	[MOVE_G_MAX_RESONANCE_P] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 10,
		.type = TYPE_ICE,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_PHYSICAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_AURORA_VEIL,
	},

	[MOVE_G_MAX_RESONANCE_S] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 10,
		.type = TYPE_ICE,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_SPECIAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_AURORA_VEIL,
	},

	[MOVE_G_MAX_CUDDLE_P] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 10,
		.type = TYPE_NORMAL,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_PHYSICAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_INFATUATE_FOES,
	},

	[MOVE_G_MAX_CUDDLE_S] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 10,
		.type = TYPE_NORMAL,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_SPECIAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_INFATUATE_FOES,
	},

	[MOVE_G_MAX_REPLENISH_P] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 10,
		.type = TYPE_NORMAL,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_PHYSICAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_RECYCLE_BERRIES,
	},

	[MOVE_G_MAX_REPLENISH_S] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 10,
		.type = TYPE_NORMAL,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_SPECIAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_RECYCLE_BERRIES,
	},

	[MOVE_G_MAX_MALODOR_P] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 10,
		.type = TYPE_POISON,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_PHYSICAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_POISON_FOES,
	},

	[MOVE_G_MAX_MALODOR_S] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 10,
		.type = TYPE_POISON,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_SPECIAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_POISON_FOES,
	},

	[MOVE_G_MAX_MELTDOWN_P] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 10,
		.type = TYPE_STEEL,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_PHYSICAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_TORMENT_FOES,
	},

	[MOVE_G_MAX_MELTDOWN_S] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 10,
		.type = TYPE_STEEL,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_SPECIAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_TORMENT_FOES,
	},
	
	[MOVE_G_MAX_DRUM_SOLO_P] =
	{	//ANIM TODO
		.effect = EFFECT_MAX_MOVE,
		.power = 10,
		.type = TYPE_GRASS,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_PHYSICAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_IGNORE_ABILITY,
	},
	
	[MOVE_G_MAX_DRUM_SOLO_S] =
	{	//ANIM TODO
		.effect = EFFECT_MAX_MOVE,
		.power = 10,
		.type = TYPE_GRASS,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_SPECIAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_IGNORE_ABILITY,
	},
	
	[MOVE_G_MAX_FIREBALL_P] =
	{	//ANIM TODO
		.effect = EFFECT_MAX_MOVE,
		.power = 10,
		.type = TYPE_FIRE,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_PHYSICAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_IGNORE_ABILITY,
	},
	
	[MOVE_G_MAX_FIREBALL_S] =
	{	//ANIM TODO
		.effect = EFFECT_MAX_MOVE,
		.power = 10,
		.type = TYPE_FIRE,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_SPECIAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_IGNORE_ABILITY,
	},
	
	[MOVE_G_MAX_HYDROSNIPE_P] =
	{	//ANIM TODO
		.effect = EFFECT_MAX_MOVE,
		.power = 10,
		.type = TYPE_WATER,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_PHYSICAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_IGNORE_ABILITY,
	},
	
	[MOVE_G_MAX_HYDROSNIPE_S] =
	{	//ANIM TODO
		.effect = EFFECT_MAX_MOVE,
		.power = 10,
		.type = TYPE_WATER,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_SPECIAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_IGNORE_ABILITY,
	},
	
	[MOVE_G_MAX_WIND_RAGE_P] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 10,
		.type = TYPE_FLYING,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_PHYSICAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_DEFOG,
	},

	[MOVE_G_MAX_WIND_RAGE_S] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 10,
		.type = TYPE_FLYING,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_SPECIAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_DEFOG,
	},

	[MOVE_G_MAX_GRAVITAS_P] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 10,
		.type = TYPE_PSYCHIC,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_PHYSICAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_GRAVITY,
	},

	[MOVE_G_MAX_GRAVITAS_S] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 10,
		.type = TYPE_PSYCHIC,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_SPECIAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_GRAVITY,
	},
	
	[MOVE_G_MAX_STONESURGE_P] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 10,
		.type = TYPE_WATER,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_PHYSICAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_STEALTH_ROCK,
	},

	[MOVE_G_MAX_STONESURGE_S] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 10,
		.type = TYPE_WATER,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_SPECIAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_STEALTH_ROCK,
	},

	[MOVE_G_MAX_VOLCALITH_P] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 10,
		.type = TYPE_ROCK,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_PHYSICAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_VOLCAITH_FOES,
	},

	[MOVE_G_MAX_VOLCALITH_S] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 10,
		.type = TYPE_ROCK,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_SPECIAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_VOLCAITH_FOES,
	},

	[MOVE_G_MAX_TARTNESS_P] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 10,
		.type = TYPE_GRASS,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_PHYSICAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_LOWER_EVASIVENESS_FOES,
	},

	[MOVE_G_MAX_TARTNESS_S] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 10,
		.type = TYPE_GRASS,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_SPECIAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_LOWER_EVASIVENESS_FOES,
	},

	[MOVE_G_MAX_SWEETNESS_P] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 10,
		.type = TYPE_GRASS,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_PHYSICAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_AROMATHERAPY,
	},

	[MOVE_G_MAX_SWEETNESS_S] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 10,
		.type = TYPE_GRASS,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_SPECIAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_AROMATHERAPY,
	},
	
	[MOVE_G_MAX_SANDBLAST_P] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 10,
		.type = TYPE_GROUND,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_PHYSICAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_SANDBLAST_FOES,
	},

	[MOVE_G_MAX_SANDBLAST_S] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 10,
		.type = TYPE_GROUND,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_SPECIAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_SANDBLAST_FOES,
	},
	
	[MOVE_G_MAX_STUN_SHOCK_P] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 10,
		.type = TYPE_ELECTRIC,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_PHYSICAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_POISON_PARALYZE_FOES,
	},

	[MOVE_G_MAX_STUN_SHOCK_S] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 10,
		.type = TYPE_ELECTRIC,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_SPECIAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_POISON_PARALYZE_FOES,
	},

	[MOVE_G_MAX_CENTIFERNO_P] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 10,
		.type = TYPE_FIRE,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_PHYSICAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_FIRE_SPIN_FOES,
	},

	[MOVE_G_MAX_CENTIFERNO_S] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 10,
		.type = TYPE_FIRE,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_SPECIAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_FIRE_SPIN_FOES,
	},
	
	[MOVE_G_MAX_SMITE_P] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 10,
		.type = TYPE_FAIRY,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_PHYSICAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_CONFUSE_FOES,
	},
	
	[MOVE_G_MAX_SMITE_S] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 10,
		.type = TYPE_FAIRY,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_SPECIAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_CONFUSE_FOES,
	},


	[MOVE_G_MAX_SNOOZE_P] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 10,
		.type = TYPE_DARK,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_PHYSICAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_YAWN_FOE,
	},

	[MOVE_G_MAX_SNOOZE_S] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 10,
		.type = TYPE_DARK,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_SPECIAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_YAWN_FOE,
	},

	[MOVE_G_MAX_FINALE_P] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 10,
		.type = TYPE_FAIRY,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_PHYSICAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_HEAL_TEAM,
	},

	[MOVE_G_MAX_FINALE_S] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 10,
		.type = TYPE_FAIRY,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_SPECIAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_HEAL_TEAM,
	},

	[MOVE_G_MAX_STEELSURGE_P] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 10,
		.type = TYPE_STEEL,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_PHYSICAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_STEELSURGE,
	},

	[MOVE_G_MAX_STEELSURGE_S] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 10,
		.type = TYPE_STEEL,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_SPECIAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_STEELSURGE,
	},
	
	[MOVE_G_MAX_DEPLETION_P] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 10,
		.type = TYPE_DRAGON,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_PHYSICAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_SPITE,
	},

	[MOVE_G_MAX_DEPLETION_S] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 10,
		.type = TYPE_DRAGON,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_SPECIAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_SPITE,
	},	

	[MOVE_G_MAX_ONE_BLOW_P] =
	{
		.effect = EFFECT_MAX_MOVE,
		.power = 10,
		.type = TYPE_DARK,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_PHYSICAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_BYPASS_PROTECT,
	},
	
	[MOVE_G_MAX_ONE_BLOW_S] =
	{	//ANIM TODO
		.effect = EFFECT_MAX_MOVE,
		.power = 10,
		.type = TYPE_DARK,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_SPECIAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_BYPASS_PROTECT,
	},
	
	[MOVE_G_MAX_RAPID_FLOW_P] =
	{	//ANIM TODO
		.effect = EFFECT_MAX_MOVE,
		.power = 10,
		.type = TYPE_WATER,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_PHYSICAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_BYPASS_PROTECT,
	},
	
	[MOVE_G_MAX_RAPID_FLOW_S] =
	{	//ANIM TODO
		.effect = EFFECT_MAX_MOVE,
		.power = 10,
		.type = TYPE_WATER,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.split = SPLIT_SPECIAL,
		.z_move_power = 0,
		.z_move_effect = MAX_EFFECT_BYPASS_PROTECT,
	},
	//Gen 9 Moves
	[MOVE_AQUACUTTER] =
    {	
		.effect = EFFECT_HIGH_CRITICAL,
		.power = 70,
		.type = TYPE_WATER,
		.accuracy = 100,
		.pp = 20,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.split = SPLIT_PHYSICAL,
		.z_move_power = 140,
		.z_move_effect = 0,
	},

	[MOVE_AQUASTEP] =
    {	
		.effect = EFFECT_SPEED_UP_1_HIT,
		.power = 80,
		.type = TYPE_WATER,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.split = SPLIT_PHYSICAL,
		.z_move_power = 160,
		.z_move_effect = 0,
	},

	[MOVE_ARMORCANNON] =
    {	
		.effect = EFFECT_SUPERPOWER,
		.power = 120,
		.type = TYPE_FIRE,
		.accuracy = 100,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.split = SPLIT_SPECIAL,
		.z_move_power = 200,
		.z_move_effect = 0,
	},

	[MOVE_AXEKICK] =
    {	
		.effect = EFFECT_RECOIL_IF_MISS,
		.power = 120,
		.type = TYPE_FIGHTING,
		.accuracy = 90,
		.pp = 10,
		.secondaryEffectChance = 30,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.split = SPLIT_PHYSICAL,
		.z_move_power = 255,
		.z_move_effect = 0,
	},

	[MOVE_BITTERBLADE] =
    {	
		.effect = EFFECT_ABSORB,
		.power = 90,
		.type = TYPE_FIRE,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.split = SPLIT_PHYSICAL,
		.z_move_power = 160,
		.z_move_effect = 0,
	},

	[MOVE_BLAZINGTORQUE] =
	{
		.effect = EFFECT_BURN_HIT,
		.power = 80,
		.type = TYPE_FIRE,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 30,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 160,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_CHILLINGWATER] =
    {	
		.effect = EFFECT_ATTACK_DOWN_HIT,
		.power = 50,
		.type = TYPE_WATER,
		.accuracy = 100,
		.pp = 20,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.split = SPLIT_SPECIAL,
		.z_move_power = 100,
		.z_move_effect = 0,
	},

	[MOVE_CHILLYRECEPTION] =
	{
		.effect = EFFECT_HAIL,
		.power = 0,
		.type = TYPE_ICE,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_RECOVER_HP
	},

	[MOVE_COLLISIONCOURSE] =
    {	
		.effect = EFFECT_HIT,
		.power = 100,
		.type = TYPE_FIGHTING,
		.accuracy = 100,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.split = SPLIT_PHYSICAL,
		.z_move_power = 250,
		.z_move_effect = 0,
	},

	[MOVE_COMBATTORQUE] =
	{
		.effect = EFFECT_PARALYZE_HIT,
		.power = 100,
		.type = TYPE_FIGHTING,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 30,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 160,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_COMEUPPANCE] =
	{
		.effect = EFFECT_COUNTER,
		.power = 1,
		.type = TYPE_DARK,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_DEPENDS,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 120,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_DOODLE] =
	{
		.effect = EFFECT_SKILL_SWAP,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_SPDEF_UP_1
	},

	[MOVE_DOUBLESHOCK] =
    {	
		.effect = EFFECT_BURN_UP,
		.power = 120,
		.type = TYPE_ELECTRIC,
		.accuracy = 100,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.split = SPLIT_PHYSICAL,
		.z_move_power = 200,
		.z_move_effect = 0,
	},

	[MOVE_ELECTRODRIFT] =
    {	
		.effect = EFFECT_HIT,
		.power = 100,
		.type = TYPE_ELECTRIC,
		.accuracy = 100,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.split = SPLIT_SPECIAL,
		.z_move_power = 250,
		.z_move_effect = 0,
	},

	[MOVE_FILLETAWAY] =
    {	
		.effect = EFFECT_DRAGON_DANCE,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = FLAG_SNATCH_AFFECTED,
		.split = SPLIT_STATUS,
		.z_move_power = 0,
		.z_move_effect = Z_EFFECT_RESET_STATS,
	},

	[MOVE_FLOWERTRICK] =
    {	
		.effect = EFFECT_HIT,
		.power = 70,
		.type = TYPE_GRASS,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.split = SPLIT_PHYSICAL,
		.z_move_power = 160,
		.z_move_effect = 0,
	},

	[MOVE_GIGATONHAMMER] =
    {	
		.effect = EFFECT_LAST_RESORT,
		.power = 160,
		.type = TYPE_STEEL,
		.accuracy = 100,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.split = SPLIT_PHYSICAL,
		.z_move_power = 250,
		.z_move_effect = 0,
	},

	[MOVE_GLAIVERUSH] =
    {	
		.effect = EFFECT_GLAIVE_RUSH,
		.power = 120,
		.type = TYPE_DRAGON,
		.accuracy = 100,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.split = SPLIT_PHYSICAL,
		.z_move_power = 250,
		.z_move_effect = 0,
	},

	[MOVE_HYDROSTEAM] =
	{
		.effect = EFFECT_HIT,
		.power = 80,
		.type = TYPE_WATER,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 130,
		.split = SPLIT_SPECIAL,
		.z_move_effect = Z_EFFECT_SPATK_UP_1
	},

	[MOVE_HYPERDRILL] =
    {	
		.effect = EFFECT_HIT,
		.power = 100,
		.type = TYPE_NORMAL,
		.accuracy = 100,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_MIRROR_MOVE_AFFECTED,
		.split = SPLIT_PHYSICAL,
		.z_move_power = 180,
		.z_move_effect = 0,
	},

	[MOVE_ICESPINNER] =
    {	
		.effect = EFFECT_DAMAGE_SET_TERRAIN,
		.power = 80,
		.type = TYPE_ICE,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.split = SPLIT_PHYSICAL,
		.z_move_power = 180,
		.z_move_effect = 0,
	},

	[MOVE_JETPUNCH] =
    {	
		.effect = EFFECT_HIT,
		.power = 60,
		.type = TYPE_WATER,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 1,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.split = SPLIT_PHYSICAL,
		.z_move_power = 130,
		.z_move_effect = 0,
	},

	[MOVE_KOWTOWCLEAVE] =
    {	
		.effect = EFFECT_ALWAYS_HIT,
		.power = 85,
		.type = TYPE_DARK,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.split = SPLIT_PHYSICAL,
		.z_move_power = 180,
		.z_move_effect = 0,
	},

	[MOVE_LASTRESPECTS] =
    {	
		.effect = EFFECT_HIT,
		.power = 50,
		.type = TYPE_GHOST,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.split = SPLIT_PHYSICAL,
		.z_move_power = 200,
		.z_move_effect = 0,
	},

	[MOVE_LUMINACRASH] =
    {	
		.effect = EFFECT_SPECIAL_DEFENSE_DOWN_2_HIT,
		.power = 80,
		.type = TYPE_PSYCHIC,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.split = SPLIT_SPECIAL,
		.z_move_power = 120,
		.z_move_effect = 0,
	},

	[MOVE_MAGICALTORQUE] =
	{
		.effect = EFFECT_CONFUSE_HIT,
		.power = 100,
		.type = TYPE_FAIRY,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 30,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 160,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_MAKEITRAIN] =
    {	
		.effect = EFFECT_SUPERPOWER,
		.power = 120,
		.type = TYPE_STEEL,
		.accuracy = 100,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_OPPONENTS_FIELD,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.split = SPLIT_SPECIAL,
		.z_move_power = 220,
		.z_move_effect = 0,
	},

	[MOVE_MORTALSPIN] =
    {	
		.effect = EFFECT_RAPID_SPIN,
		.power = 30,
		.type = TYPE_POISON,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.split = SPLIT_PHYSICAL,
		.z_move_power = 80,
		.z_move_effect = 0,
	},

	[MOVE_NOXIOUSTORQUE] =
	{
		.effect = EFFECT_POISON_HIT,
		.power = 100,
		.type = TYPE_POISON,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 30,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 160,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_ORDERUP] =
	{
		.effect = EFFECT_ATTACK_UP_HIT,
		.power = 80,
		.type = TYPE_DRAGON,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 120,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_POPULATIONBOMB] =
    {	
		.effect = EFFECT_MULTI_HIT,
		.power = 20,
		.type = TYPE_NORMAL,
		.accuracy = 90,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT |FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.split = SPLIT_PHYSICAL,
		.z_move_power = 180,
		.z_move_effect = 0,
	},

	[MOVE_POUNCE] =
    {	
		.effect = EFFECT_SPEED_DOWN_HIT,
		.power = 50,
		.type = TYPE_BUG,
		.accuracy = 100,
		.pp = 20,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.split = SPLIT_PHYSICAL,
		.z_move_power = 100,
		.z_move_effect = 0,
	},

	[MOVE_PSYBLADE] =
	{
		.effect = EFFECT_HIT,
		.power = 80,
		.type = TYPE_PSYCHIC,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 130,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = Z_EFFECT_ATK_UP_1,
	},

	[MOVE_RAGEFIST] =
    {	
		.effect = EFFECT_HIT,
		.power = 50,
		.type = TYPE_GHOST,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.split = SPLIT_PHYSICAL,
		.z_move_power = 180,
		.z_move_effect = 0,
	},

	[MOVE_RAGINGBULL] =
    {	
		.effect = EFFECT_BRICK_BREAK,
		.power = 90,
		.type = TYPE_NORMAL,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.split = SPLIT_PHYSICAL,
		.z_move_power = 200,
		.z_move_effect = 0,
	},

	[MOVE_REVIVALBLESSING] =
	{
		.effect = EFFECT_HEAL_BELL,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 0,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = FLAG_SNATCH_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_RECOVER_HP
	},

	[MOVE_RUINATION] =
    {	
		.effect = EFFECT_SUPER_FANG,
		.power = 1,
		.type = TYPE_DARK,
		.accuracy = 90,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.split = SPLIT_SPECIAL,
		.z_move_power = 160,
		.z_move_effect = 0,
	},

	[MOVE_SALTCURE] =
    {	
		.effect = EFFECT_SALTCURE,
		.power = 40,
		.type = TYPE_ROCK,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.split = SPLIT_PHYSICAL,
		.z_move_power = 80,
		.z_move_effect = 0,
	},

	[MOVE_SHEDTAIL] =
    {	
		.effect = EFFECT_SUBSTITUTE,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = FLAG_SNATCH_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_RESET_STATS
	},

	[MOVE_SILKTRAP] =
    {	
		.effect = EFFECT_PROTECT,
		.power = 0,
		.type = TYPE_BUG,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 4,
		.flags = 0,
		.split = SPLIT_STATUS,
		.z_move_power = 0,
		.z_move_effect = Z_EFFECT_RESET_STATS,
	},

	[MOVE_SNOWSCAPE] =
	{
		.effect = EFFECT_HAIL,
		.power = 0,
		.type = TYPE_ICE,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_SPD_UP_1
	},

	[MOVE_SPICYEXTRACT] =
    {	
		.effect = EFFECT_DEFENSE_DOWN_2,
		.power = 0,
		.type = TYPE_GRASS,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.split = SPLIT_STATUS,
		.z_move_power = 0,
		.z_move_effect = Z_EFFECT_SPATK_UP_2,
	},

	[MOVE_SPINOUT] =
    {	
		.effect = EFFECT_SUPERPOWER,
		.power = 100,
		.type = TYPE_STEEL,
		.accuracy = 100,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.split = SPLIT_PHYSICAL,
		.z_move_power = 150,
		.z_move_effect = 0,
	},

	[MOVE_TERABLAST] =
    {	
		.effect = EFFECT_HIT,
		.power = 80,
		.type = TYPE_NORMAL,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.split = SPLIT_SPECIAL,
		.z_move_power = 130,
		.z_move_effect = 0,
	},

	[MOVE_TIDYUP] =
    {	
		.effect = EFFECT_RAPID_SPIN,
		.power = 0,
		.type = TYPE_NORMAL,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_ATK_UP_1,
	},

	[MOVE_TORCHSONG] =
    {	
		.effect = EFFECT_SPECIAL_ATTACK_UP_HIT,
		.power = 80,
		.type = TYPE_FIRE,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.split = SPLIT_SPECIAL,
		.z_move_power = 120,
		.z_move_effect = 0,
	},

	[MOVE_TRAILBLAZE] =
    {	
		.effect = EFFECT_SPEED_UP_1_HIT,
		.power = 50,
		.type = TYPE_GRASS,
		.accuracy = 100,
		.pp = 20,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.split = SPLIT_PHYSICAL,
		.z_move_power = 90,
		.z_move_effect = 0,
	},

	[MOVE_TRIPLEDIVE] =
    {	
		.effect = EFFECT_TRIPLE_KICK,
		.power = 30,
		.type = TYPE_WATER,
		.accuracy = 95,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.split = SPLIT_PHYSICAL,
		.z_move_power = 120,
		.z_move_effect = 0,
	},

	[MOVE_TWINBEAM] =
    {	
		.effect = EFFECT_DOUBLE_HIT,
		.power = 40,
		.type = TYPE_PSYCHIC,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.split = SPLIT_SPECIAL,
		.z_move_power = 120,
		.z_move_effect = 0,
	},

	[MOVE_WICKEDTORQUE] =
	{
		.effect = EFFECT_RELIC_SONG,
		.power = 100,
		.type = TYPE_DARK,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 10,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 160,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_BLOODMOON] =
    {	
		.effect = EFFECT_HIT,
		.power = 140,
		.type = TYPE_NORMAL,
		.accuracy = 100,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.split = SPLIT_SPECIAL,
		.z_move_power = 250,
		.z_move_effect = 0,
	},

	[MOVE_UPPERHAND] =
	{
		.effect = EFFECT_FLINCH_HIT,
		.power = 65,
		.type = TYPE_FIGHTING,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 3,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 100,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_MATCHAGOTCHA] =
	{
		.effect = EFFECT_BURN_HIT,
		.power = 80,
		.type = TYPE_GRASS,
		.accuracy = 90,
		.pp = 15,
		.secondaryEffectChance = 30,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_TRIAGE_AFFECTED,
		.z_move_power = 140,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_SYRUPBOMB] =
	{
		.effect = EFFECT_SPEED_DOWN_HIT,
		.power = 60,
		.type = TYPE_GRASS,
		.accuracy = 85,
		.pp = 15,
		.secondaryEffectChance = 100,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_TRIAGE_AFFECTED,
		.z_move_power = 120,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_IVYCUDGEL] =
	{
		.effect = EFFECT_HIGH_CRITICAL,
		.power = 100,
		.type = TYPE_GRASS,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 180,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_ELECTROSHOT] =
	{
		.effect = EFFECT_SKULL_BASH,
		.power = 130,
		.type = TYPE_ELECTRIC,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 190,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_TERASTARSTORM] =
	{
		.effect = EFFECT_HIT,
		.power = 120,
		.type = TYPE_NORMAL,
		.accuracy = 100,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 180,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_FICKLEBEAM] =
	{
		.effect = EFFECT_HIT,
		.power = 80,
		.type = TYPE_DRAGON,
		.accuracy = 100,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 140,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_BURNINGBULWARK] =
	{
		.effect = EFFECT_PROTECT,
		.power = 0,
		.type = TYPE_FIRE,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 4,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_SPDEF_UP_1
	},

	[MOVE_MIGHTYCLEAVE] =
	{
		.effect = EFFECT_HIT,
		.power = 95,
		.type = TYPE_ROCK,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 3,
		.flags = FLAG_MAKES_CONTACT | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 130,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_TACHYONCUTTER] =
	{
		.effect = EFFECT_DOUBLE_HIT,
		.power = 50,
		.type = TYPE_DRAGON,
		.accuracy = 0,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 140,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_DRAGONCHEER] =
	{
		.effect = EFFECT_HELPING_HAND,
		.power = 0,
		.type = TYPE_DRAGON,
		.accuracy = 0,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_USER,
		.priority = 0,
		.flags = 0,
		.z_move_power = 0,
		.split = SPLIT_STATUS,
		.z_move_effect = Z_EFFECT_RESET_STATS
	},

	[MOVE_MALIGNANTCHAIN] =
	{
		.effect = EFFECT_POISON_HIT,
		.power = 100,
		.type = TYPE_POISON,
		.accuracy = 100,
		.pp = 20,
		.secondaryEffectChance = 50,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 160,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_THUNDERCLAP] =
	{
		.effect = EFFECT_SUCKER_PUNCH,
		.power = 70,
		.type = TYPE_ELECTRIC,
		.accuracy = 100,
		.pp = 5,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 1,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 160,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_HARDPRESS] =
	{
		.effect = EFFECT_HIT,
		.power = 1,
		.type = TYPE_STEEL,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 30,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 160,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_ALLURINGVOICE] =
	{
		.effect = EFFECT_CONFUSE_HIT,
		.power = 80,
		.type = TYPE_FAIRY,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_FOES_AND_ALLY,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 175,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	},

	[MOVE_TEMPERFLARE] =
	{
		.effect = EFFECT_HIT,
		.power = 75,
		.type = TYPE_FIRE,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 160,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_SUPERCELLSLAM] =
	{
		.effect = EFFECT_RECOIL_IF_MISS,
		.power = 100,
		.type = TYPE_ELECTRIC,
		.accuracy = 100,
		.pp = 15,
		.secondaryEffectChance = 0,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 165,
		.split = SPLIT_PHYSICAL,
		.z_move_effect = 0
	},

	[MOVE_PSYCHICNOISE] =
	{
		.effect = EFFECT_ATTACK_BLOCKERS,
		.power = 90,
		.type = TYPE_PSYCHIC,
		.accuracy = 100,
		.pp = 10,
		.secondaryEffectChance = 10,
		.target = MOVE_TARGET_SELECTED,
		.priority = 0,
		.flags = FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
		.z_move_power = 175,
		.split = SPLIT_SPECIAL,
		.z_move_effect = 0
	}
#endif
};

#ifdef DYNAMAX_FEATURE
const u8 gDynamaxMovePowers[MOVES_COUNT] =
{
	[MOVE_POUND] = 90,
	[MOVE_KARATECHOP] = 75,
	[MOVE_DOUBLESLAP] = 90,
	[MOVE_COMETPUNCH] = 100,
	[MOVE_MEGAPUNCH] = 130,
	[MOVE_PAYDAY] = 90,
	[MOVE_FIREPUNCH] = 130,
	[MOVE_ICEPUNCH] = 130,
	[MOVE_THUNDERPUNCH] = 130,
	[MOVE_SCRATCH] = 90,
	[MOVE_VICEGRIP] = 110,
	[MOVE_GUILLOTINE] = 130,
	[MOVE_RAZORWIND] = 130,
	[MOVE_CUT] = 100,
	[MOVE_GUST] = 90,
	[MOVE_WINGATTACK] = 110,
	[MOVE_FLY] = 130,
	[MOVE_BIND] = 90,
	[MOVE_SLAM] = 130,
	[MOVE_VINEWHIP] = 100,
	[MOVE_STOMP] = 120,
	[MOVE_DOUBLEKICK] = 80,
	[MOVE_MEGAKICK] = 140,
	[MOVE_JUMPKICK] = 90,
	[MOVE_ROLLINGKICK] = 80,
	[MOVE_HEADBUTT] = 120,
	[MOVE_HORNATTACK] = 120,
	[MOVE_FURYATTACK] = 90,
	[MOVE_HORNDRILL] = 130,
	[MOVE_TACKLE] = 90,
	[MOVE_BODYSLAM] = 130,
	[MOVE_WRAP] = 90,
	[MOVE_TAKEDOWN] = 130,
	[MOVE_THRASH] = 140,
	[MOVE_DOUBLEEDGE] = 140,
	[MOVE_POISONSTING] = 70,
	[MOVE_TWINEEDLE] = 100,
	[MOVE_PINMISSILE] = 130,
	[MOVE_BITE] = 110,
	[MOVE_SONICBOOM] = 100,
	[MOVE_ACID] = 70,
	[MOVE_EMBER] = 90,
	[MOVE_FLAMETHROWER] = 130,
	[MOVE_WATERGUN] = 90,
	[MOVE_HYDROPUMP] = 140,
	[MOVE_SURF] = 130,
	[MOVE_ICEBEAM] = 130,
	[MOVE_BLIZZARD] = 140,
	[MOVE_PSYBEAM] = 120,
	[MOVE_BUBBLEBEAM] = 120,
	[MOVE_AURORABEAM] = 120,
	[MOVE_HYPERBEAM] = 150,
	[MOVE_PECK] = 90,
	[MOVE_DRILLPECK] = 130,
	[MOVE_SUBMISSION] = 90,
	[MOVE_LOWKICK] = 100,
	[MOVE_COUNTER] = 75,
	[MOVE_SEISMICTOSS] = 75,
	[MOVE_STRENGTH] = 130,
	[MOVE_ABSORB] = 90,
	[MOVE_MEGADRAIN] = 90,
	[MOVE_RAZORLEAF] = 110,
	[MOVE_SOLARBEAM] = 140,
	[MOVE_PETALDANCE] = 140,
	[MOVE_DRAGONRAGE] = 100,
	[MOVE_FIRESPIN] = 90,
	[MOVE_THUNDERSHOCK] = 90,
	[MOVE_THUNDERBOLT] = 130,
	[MOVE_THUNDER] = 140,
	[MOVE_ROCKTHROW] = 100,
	[MOVE_EARTHQUAKE] = 130,
	[MOVE_FISSURE] = 130,
	[MOVE_DIG] = 130,
	[MOVE_CONFUSION] = 100,
	[MOVE_PSYCHIC] = 130,
	[MOVE_QUICKATTACK] = 90,
	[MOVE_RAGE] = 90,
	[MOVE_NIGHTSHADE] = 100,
	[MOVE_BIDE] = 100,
	[MOVE_SELFDESTRUCT] = 150,
	[MOVE_EGGBOMB] = 130,
	[MOVE_LICK] = 90,
	[MOVE_SMOG] = 70,
	[MOVE_SLUDGE] = 85,
	[MOVE_BONECLUB] = 120,
	[MOVE_FIREBLAST] = 140,
	[MOVE_WATERFALL] = 130,
	[MOVE_CLAMP] = 90,
	[MOVE_SWIFT] = 110,
	[MOVE_SKULLBASH] = 140,
	[MOVE_SPIKECANNON] = 120,
	[MOVE_CONSTRICT] = 90,
	[MOVE_HIGHJUMPKICK] = 95,
	[MOVE_DREAMEATER] = 130,
	[MOVE_BARRAGE] = 90,
	[MOVE_LEECHFANG] = 130,
	#ifdef BUFFED_LEECH_LIFE
	[MOVE_LEECHLIFE] = 130,
	#else
	[MOVE_LEECHLIFE] = 90,
	#endif
	[MOVE_SKYATTACK] = 140,
	[MOVE_BUBBLE] = 90,
	[MOVE_DIZZYPUNCH] = 120,
	[MOVE_PSYWAVE] = 100,
	[MOVE_CRABHAMMER] = 130,
	[MOVE_EXPLOSION] = 150,
	[MOVE_FURYSWIPES] = 100,
	[MOVE_BONEMERANG] = 130,
	[MOVE_ROCKSLIDE] = 130,
	[MOVE_HYPERFANG] = 130,
	[MOVE_TRIATTACK] = 130,
	[MOVE_SUPERFANG] = 100,
	[MOVE_SLASH] = 120,
	[MOVE_STRUGGLE] = 1,
	[MOVE_TRIPLEKICK] = 80,
	[MOVE_THIEF] = 110,
	[MOVE_FLAMEWHEEL] = 110,
	[MOVE_SNORE] = 100,
	[MOVE_FLAIL] = 130,
	[MOVE_AEROBLAST] = 130,
	[MOVE_REVERSAL] = 100,
	[MOVE_POWDERSNOW] = 90,
	[MOVE_MACHPUNCH] = 70,
	[MOVE_FEINTATTACK] = 110,
	[MOVE_SLUDGEBOMB] = 90,
	[MOVE_MUDSLAP] = 90,
	[MOVE_OCTAZOOKA] = 120,
	[MOVE_ZAPCANNON] = 140,
	[MOVE_ICYWIND] = 110,
	[MOVE_BONERUSH] = 130,
	[MOVE_OUTRAGE] = 140,
	[MOVE_GIGADRAIN] = 130,
	[MOVE_ROLLOUT] = 90,
	[MOVE_FALSESWIPE] = 90,
	[MOVE_SPARK] = 120,
	[MOVE_FURYCUTTER] = 90,
	[MOVE_STEELWING] = 120,
	[MOVE_RETURN] = 130,
	[MOVE_PRESENT] = 100,
	[MOVE_FRUSTRATION] = 130,
	[MOVE_SACREDFIRE] = 130,
	[MOVE_MAGNITUDE] = 140,
	[MOVE_DYNAMICPUNCH] = 90,
	[MOVE_MEGAHORN] = 140,
	[MOVE_DRAGONBREATH] = 110,
	[MOVE_PURSUIT] = 90,
	[MOVE_RAPIDSPIN] = 100,
	[MOVE_IRONTAIL] = 130,
	[MOVE_METALCLAW] = 100,
	[MOVE_VITALTHROW] = 85,
	[MOVE_HIDDENPOWER] = 110,
	[MOVE_CROSSCHOP] = 90,
	[MOVE_TWISTER] = 90,
	[MOVE_CRUNCH] = 130,
	[MOVE_MIRRORCOAT] = 100,
	[MOVE_EXTREMESPEED] = 130,
	[MOVE_ANCIENTPOWER] = 110,
	[MOVE_SHADOWBALL] = 130,
	[MOVE_FUTURESIGHT] = 140,
	[MOVE_ROCKSMASH] = 70,
	[MOVE_WHIRLPOOL] = 90,
	[MOVE_BEATUP] = 100,
	[MOVE_FAKEOUT] = 90,
	[MOVE_UPROAR] = 130,
	[MOVE_SPITUP] = 100,
	[MOVE_HEATWAVE] = 130,
	[MOVE_FACADE] = 120,
	[MOVE_FOCUSPUNCH] = 100,
	[MOVE_SMELLINGSALTS] = 120,
	[MOVE_SUPERPOWER] = 95,
	[MOVE_REVENGE] = 80,
	[MOVE_BRICKBREAK] = 90,
	[MOVE_KNOCKOFF] = 120,
	[MOVE_ENDEAVOR] = 130,
	[MOVE_ERUPTION] = 150,
	[MOVE_SECRETPOWER] = 120,
	[MOVE_DIVE] = 130,
	[MOVE_ARMTHRUST] = 70,
	[MOVE_LUSTERPURGE] = 120,
	[MOVE_MISTBALL] = 120,
	[MOVE_BLAZEKICK] = 130,
	[MOVE_ICEBALL] = 90,
	[MOVE_NEEDLEARM] = 110,
	[MOVE_HYPERVOICE] = 130,
	[MOVE_POISONFANG] = 75,
	[MOVE_CRUSHCLAW] = 130,
	[MOVE_BLASTBURN] = 150,
	[MOVE_HYDROCANNON] = 150,
	[MOVE_METEORMASH] = 130,
	[MOVE_ASTONISH] = 90,
	[MOVE_WEATHERBALL] = 130,
	[MOVE_AIRCUTTER] = 110,
	[MOVE_OVERHEAT] = 140,
	[MOVE_ROCKTOMB] = 110,
	[MOVE_SILVERWIND] = 110,
	[MOVE_WATERSPOUT] = 150,
	[MOVE_SIGNALBEAM] = 130,
	[MOVE_SHADOWPUNCH] = 110,
	[MOVE_EXTRASENSORY] = 130,
	[MOVE_SKYUPPERCUT] = 90,
	[MOVE_SANDTOMB] = 90,
	[MOVE_SHEERCOLD] = 130,
	[MOVE_MUDDYWATER] = 130,
	[MOVE_BULLETSEED] = 130,
	[MOVE_AERIALACE] = 110,
	[MOVE_ICICLESPEAR] = 130,
	[MOVE_DRAGONCLAW] = 130,
	[MOVE_FRENZYPLANT] = 150,
	[MOVE_BOUNCE] = 130,
	[MOVE_MUDSHOT] = 110,
	[MOVE_POISONTAIL] = 75,
	[MOVE_COVET] = 110,
	[MOVE_VOLTTACKLE] = 140,
	[MOVE_MAGICALLEAF] = 110,
	[MOVE_LEAFBLADE] = 130,
	[MOVE_ROCKBLAST] = 130,
	[MOVE_SHOCKWAVE] = 110,
	[MOVE_WATERPULSE] = 110,
	[MOVE_DOOMDESIRE] = 140,
	[MOVE_PSYCHOBOOST] = 140,
	[MOVE_WAKEUPSLAP] = 85,
	[MOVE_HAMMERARM] = 90,
	[MOVE_GYROBALL] = 130,
	[MOVE_BRINE] = 120,
	[MOVE_NATURALGIFT] = 130,
	[MOVE_FEINT] = 90,
	[MOVE_PLUCK] = 110,
	[MOVE_METALBURST] = 100,
	[MOVE_UTURN] = 120,
	[MOVE_CLOSECOMBAT] = 95,
	[MOVE_PAYBACK] = 100,
	[MOVE_ASSURANCE] = 110,
	[MOVE_FLING] = 100,
	[MOVE_TRUMPCARD] = 130,
	[MOVE_WRINGOUT] = 140,
	[MOVE_PUNISHMENT] = 130,
	[MOVE_LASTRESORT] = 140,
	[MOVE_SUCKERPUNCH] = 120,
	[MOVE_FLAREBLITZ] = 140,
	[MOVE_FORCEPALM] = 80,
	[MOVE_AURASPHERE] = 90,
	[MOVE_POISONJAB] = 90,
	[MOVE_DARKPULSE] = 130,
	[MOVE_NIGHTSLASH] = 120,
	[MOVE_AQUATAIL] = 130,
	[MOVE_SEEDBOMB] = 130,
	[MOVE_AIRSLASH] = 130,
	[MOVE_XSCISSOR] = 130,
	[MOVE_BUGBUZZ] = 130,
	[MOVE_DRAGONPULSE] = 130,
	[MOVE_DRAGONRUSH] = 130,
	[MOVE_POWERGEM] = 130,
	[MOVE_DRAINPUNCH] = 90,
	[MOVE_VACUUMWAVE] = 70,
	[MOVE_FOCUSBLAST] = 95,
	[MOVE_ENERGYBALL] = 130,
	[MOVE_BRAVEBIRD] = 140,
	[MOVE_EARTHPOWER] = 130,
	[MOVE_GIGAIMPACT] = 150,
	[MOVE_BULLETPUNCH] = 90,
	[MOVE_AVALANCHE] = 110,
	[MOVE_ICESHARD] = 90,
	[MOVE_SHADOWCLAW] = 120,
	[MOVE_THUNDERFANG] = 120,
	[MOVE_ICEFANG] = 120,
	[MOVE_FIREFANG] = 120,
	[MOVE_SHADOWSNEAK] = 90,
	[MOVE_MUDBOMB] = 120,
	[MOVE_PSYCHOCUT] = 120,
	[MOVE_ZENHEADBUTT] = 130,
	[MOVE_MIRRORSHOT] = 120,
	[MOVE_FLASHCANNON] = 130,
	[MOVE_ROCKCLIMB] = 130,
	[MOVE_DRACOMETEOR] = 140,
	[MOVE_DISCHARGE] = 130,
	[MOVE_LAVAPLUME] = 130,
	[MOVE_LEAFSTORM] = 140,
	[MOVE_POWERWHIP] = 140,
	[MOVE_ROCKWRECKER] = 150,
	[MOVE_CROSSPOISON] = 85,
	[MOVE_GUNKSHOT] = 95,
	[MOVE_IRONHEAD] = 130,
	[MOVE_MAGNETBOMB] = 110,
	[MOVE_STONEEDGE] = 130,
	[MOVE_GRASSKNOT] = 130,
	[MOVE_CHATTER] = 120,
	[MOVE_JUDGMENT] = 130,
	[MOVE_BUGBITE] = 110,
	[MOVE_CHARGEBEAM] = 100,
	[MOVE_WOODHAMMER] = 140,
	[MOVE_AQUAJET] = 90,
	[MOVE_ATTACKORDER] = 130,
	[MOVE_HEADSMASH] = 150,
	[MOVE_DOUBLEHIT] = 120,
	[MOVE_ROAROFTIME] = 150,
	[MOVE_SPACIALREND] = 130,
	[MOVE_CRUSHGRIP] = 140,
	[MOVE_MAGMASTORM] = 130,
	[MOVE_SEEDFLARE] = 140,
	[MOVE_OMINOUSWIND] = 110,
	[MOVE_SHADOWFORCE] = 140,
	[MOVE_PSYSHOCK] = 130,
	[MOVE_VENOSHOCK] = 85,
	[MOVE_SMACKDOWN] = 100,
	[MOVE_STORMTHROW] = 80,
	[MOVE_FLAMEBURST] = 120,
	[MOVE_SLUDGEWAVE] = 90,
	[MOVE_HEAVYSLAM] = 130,
	[MOVE_SYNCHRONOISE] = 140,
	[MOVE_ELECTROBALL] = 130,
	[MOVE_FLAMECHARGE] = 100,
	[MOVE_LOWSWEEP] = 85,
	[MOVE_ACIDSPRAY] = 70,
	[MOVE_FOULPLAY] = 130,
	[MOVE_ROUND] = 110,
	[MOVE_ECHOEDVOICE] = 90,
	[MOVE_CHIPAWAY] = 120,
	[MOVE_CLEARSMOG] = 75,
	[MOVE_STOREDPOWER] = 130,
	[MOVE_SCALD] = 130,
	[MOVE_HEX] = 120,
	[MOVE_SKYDROP] = 110,
	[MOVE_CIRCLETHROW] = 80,
	[MOVE_INCINERATE] = 110,
	[MOVE_ACROBATICS] = 110,
	[MOVE_RETALIATE] = 120,
	[MOVE_STEELYHIT] = 90,
	[MOVE_FINALGAMBIT] = 100,
	[MOVE_INFERNO] = 130,
	[MOVE_WATERPLEDGE] = 130,
	[MOVE_FIREPLEDGE] = 130,
	[MOVE_GRASSPLEDGE] = 130,
	[MOVE_VOLTSWITCH] = 120,
	[MOVE_STRUGGLEBUG] = 100,
	[MOVE_BULLDOZE] = 110,
	[MOVE_FROSTBREATH] = 110,
	[MOVE_DRAGONTAIL] = 110,
	[MOVE_ELECTROWEB] = 110,
	[MOVE_WILDCHARGE] = 130,
	[MOVE_DRILLRUN] = 130,
	[MOVE_DUALCHOP] = 130,
	[MOVE_HEARTSTAMP] = 110,
	[MOVE_HORNLEECH] = 130,
	[MOVE_SACREDSWORD] = 90,
	[MOVE_RAZORSHELL] = 130,
	[MOVE_HEATCRASH] = 130,
	[MOVE_LEAFTORNADO] = 120,
	[MOVE_STEAMROLLER] = 120,
	[MOVE_NIGHTDAZE] = 130,
	[MOVE_PSYSTRIKE] = 130,
	[MOVE_TAILSLAP] = 130,
	[MOVE_HURRICANE] = 140,
	[MOVE_HEADCHARGE] = 140,
	[MOVE_GEARGRIND] = 130,
	[MOVE_SEARINGSHOT] = 130,
	[MOVE_TECHNOBLAST] = 140,
	[MOVE_RELICSONG] = 130,
	[MOVE_SECRETSWORD] = 90,
	[MOVE_GLACIATE] = 120,
	[MOVE_BOLTSTRIKE] = 140,
	[MOVE_BLUEFLARE] = 140,
	[MOVE_FIERYDANCE] = 130,
	[MOVE_FREEZESHOCK] = 140,
	[MOVE_ICEBURN] = 140,
	[MOVE_SNARL] = 110,
	[MOVE_ICICLECRASH] = 130,
	[MOVE_VCREATE] = 150,
	[MOVE_FUSIONFLARE] = 130,
	[MOVE_FUSIONBOLT] = 130,
	[MOVE_FLYINGPRESS] = 90,
	[MOVE_BELCH] = 95,
	[MOVE_FELLSTINGER] = 100,
	[MOVE_PHANTOMFORCE] = 130,
	[MOVE_PARABOLICCHARGE] = 120,
	[MOVE_PETALBLIZZARD] = 130,
	[MOVE_FREEZEDRY] = 120,
	[MOVE_DISARMINGVOICE] = 90,
	[MOVE_DRAININGKISS] = 100,
	[MOVE_PLAYROUGH] = 130,
	[MOVE_FAIRYWIND] = 90,
	[MOVE_MOONBLAST] = 130,
	[MOVE_BOOMBURST] = 140,
	[MOVE_DIAMONDSTORM] = 130,
	[MOVE_STEAMERUPTION] = 140,
	[MOVE_HYPERSPACEHOLE] = 130,
	[MOVE_WATERSHURIKEN] = 90,
	[MOVE_MYSTICALFIRE] = 130,
	[MOVE_DAZZLINGGLEAM] = 130,
	[MOVE_NUZZLE] = 90,
	[MOVE_HOLDBACK] = 90,
	[MOVE_INFESTATION] = 90,
	[MOVE_POWERUPPUNCH] = 70,
	[MOVE_OBLIVIONWING] = 130,
	[MOVE_THOUSANDARROWS] = 130,
	[MOVE_THOUSANDWAVES] = 130,
	[MOVE_LANDSWRATH] = 130,
	[MOVE_LIGHTOFRUIN] = 140,
	[MOVE_ORIGINPULSE] = 140,
	[MOVE_PRECIPICEBLADES] = 140,
	[MOVE_DRAGONASCENT] = 140,
	[MOVE_HYPERSPACEFURY] = 130,
	[MOVE_FIRSTIMPRESSION] = 130,
	[MOVE_SPIRITSHACKLE] = 130,
	[MOVE_DARKESTLARIAT] = 130,
	[MOVE_SPARKLINGARIA] = 130,
	[MOVE_ICEHAMMER] = 130,
	[MOVE_HIGHHORSEPOWER] = 130,
	[MOVE_SOLARBLADE] = 140,
	[MOVE_LEAFAGE] = 90,
	[MOVE_THROATCHOP] = 130,
	[MOVE_POLLENPUFF] = 130,
	[MOVE_ANCHORSHOT] = 130,
	[MOVE_LUNGE] = 130,
	[MOVE_FIRELASH] = 130,
	[MOVE_POWERTRIP] = 130,
	[MOVE_BURNUP] = 140,
	[MOVE_SMARTSTRIKE] = 120,
	[MOVE_REVELATIONDANCE] = 130,
	[MOVE_COREENFORCER] = 130,
	[MOVE_TROPKICK] = 120,
	[MOVE_BEAKBLAST] = 130,
	[MOVE_CLANGINGSCALES] = 140,
	[MOVE_DRAGONHAMMER] = 130,
	[MOVE_BRUTALSWING] = 110,
	[MOVE_SHELLTRAP] = 150,
	[MOVE_FLEURCANNON] = 140,
	[MOVE_PSYCHICFANGS] = 130,
	[MOVE_STOMPINGTANTRUM] = 130,
	[MOVE_SHADOWBONE] = 130,
	[MOVE_ACCELEROCK] = 90,
	[MOVE_LIQUIDATION] = 130,
	[MOVE_PRISMATICLASER] = 150,
	[MOVE_SPECTRALTHIEF] = 130,
	[MOVE_SUNSTEELSTRIKE] = 130,
	[MOVE_MOONGEISTBEAM] = 130,
	[MOVE_ZINGZAP] = 130,
	[MOVE_NATURESMADNESS] = 100,
	[MOVE_MULTIATTACK] = 95,
	[MOVE_MINDBLOWN] = 150,
	[MOVE_PLASMAFISTS] = 130,
	[MOVE_PHOTONGEYSER] = 130,
	[MOVE_DOUBLEIRONBASH] = 140,
	[MOVE_DYNAMAXCANNON] = 140,
	[MOVE_SNIPESHOT] = 130,
	[MOVE_JAWLOCK] = 130,
	[MOVE_DRAGONDARTS] = 130,
	[MOVE_BOLTBEAK] = 130,
	[MOVE_FISHIOUSREND] = 130,
	[MOVE_BODYPRESS] = 90,
	[MOVE_DRUMBEATING] = 130,
	[MOVE_SNAPTRAP] = 90,
	[MOVE_PYROBALL] = 140,
	[MOVE_BEHEMOTHBLADE] = 130,
	[MOVE_BEHEMOTHBASH] = 130,
	[MOVE_AURAWHEEL] = 140,
	[MOVE_BREAKINGSWIPE] = 110,
	[MOVE_BRANCHPOKE] = 90,
	[MOVE_OVERDRIVE] = 130,
	[MOVE_APPLEACID] = 130,
	[MOVE_GRAVAPPLE] = 130,
	[MOVE_SPIRITBREAK] = 130,
	[MOVE_STRANGESTEAM] = 130,
	[MOVE_FALSESURRENDER] = 130,
	[MOVE_METEORASSAULT] = 100,
	[MOVE_ETERNABEAM] = 150,
	[MOVE_STEELBEAM] = 140,
	[MOVE_EXPANDINGFORCE] = 130,
	[MOVE_STEELROLLER] = 140,
	[MOVE_SCALESHOT] = 130,
	[MOVE_METEORBEAM] = 140,
	[MOVE_SHELLSIDEARM] = 90,
	[MOVE_MISTYEXPLOSION] = 130,
	[MOVE_GRASSYGLIDE] = 120,
	[MOVE_RISINGVOLTAGE] = 140,
	[MOVE_TERRAINPULSE] = 130,
	[MOVE_SKITTERSMACK] = 120,
	[MOVE_BURNINGJEALOUSY] = 120,
	[MOVE_LASHOUT] = 130,
	[MOVE_POLTERGEIST] = 140,
	[MOVE_FLIPTURN] = 110,
	[MOVE_TRIPLEAXEL] = 140,
	[MOVE_DUALWINGBEAT] = 130,
	[MOVE_SCORCHINGSANDS] = 120,
	[MOVE_WICKEDBLOW] = 130,
	[MOVE_SURGINGSTRIKES] = 130,
	[MOVE_FREEZINGGLARE] = 130,
	[MOVE_THUNDEROUSKICK] = 90,
	[MOVE_FIERYWRATH] = 130,
	[MOVE_EERIESPELL] = 130,
	[MOVE_THUNDERCAGE] = 130,
	[MOVE_DRAGONENERGY] = 150,
	[MOVE_ASTRALBARRAGE] = 140,
	[MOVE_GLACIALLANCE] = 140,
	[MOVE_DIRECLAW] = 110,
	[MOVE_PSYSHIELDBASH] = 120,
	[MOVE_MYSTICALPOWER] = 120,
	#ifdef ACTUAL_PLA_MOVE_POWERS
	[MOVE_RAGINGFURY] = 130,
	[MOVE_CHLOROBLAST] = 140,
	[MOVE_MOUNTAINGALE] = 130,
	[MOVE_HEADLONGRUSH] = 130,
	[MOVE_STONEAXE] = 120,
	[MOVE_TRIPLEARROWS] = 75,
	[MOVE_INFERNALPARADE] = 110,
	[MOVE_CEASELESSEDGE] = 120,
	[MOVE_BARBBARRAGE] = 80,
	#else
	[MOVE_RAGINGFURY] = 140,
	[MOVE_CHLOROBLAST] = 150,
	[MOVE_MOUNTAINGALE] = 140,
	[MOVE_HEADLONGRUSH] = 140,
	[MOVE_STONEAXE] = 130,
	[MOVE_TRIPLEARROWS] = 80,
	[MOVE_INFERNALPARADE] = 130,
	[MOVE_CEASELESSEDGE] = 130,
	[MOVE_BARBBARRAGE] = 90,
	#endif
	[MOVE_WAVECRASH] = 130,
	[MOVE_ESPERWING] = 130,
	[MOVE_BITTERMALICE] = 110,
	[MOVE_BLEAKWINDSTORM] = 130,
	[MOVE_WILDBOLTSTORM] = 130,
	[MOVE_SANDSEARSTORM] = 130,
	[MOVE_SPRINGTIDESTORM] = 130,
//Gen 9 Moves
	[MOVE_AQUACUTTER] = 90,
    [MOVE_AQUASTEP] = 100,
    [MOVE_ARMORCANNON] = 160,
    [MOVE_AXEKICK] = 160,
    [MOVE_BITTERBLADE] = 110,
    [MOVE_BLAZINGTORQUE] = 120,
    [MOVE_CHILLINGWATER] = 100,
    [MOVE_COLLISIONCOURSE] = 140,
    [MOVE_COMBATTORQUE] = 140,
    [MOVE_COMEUPPANCE] = 90,
    [MOVE_DOUBLESHOCK] = 160,
    [MOVE_ELECTRODRIFT] = 140,
    [MOVE_FLOWERTRICK] = 120,
    [MOVE_GIGATONHAMMER] = 180,
    [MOVE_GLAIVERUSH] = 160,
    [MOVE_HYDROSTEAM] = 100,
    [MOVE_HYPERDRILL] = 120,
    [MOVE_ICESPINNER] = 120,
    [MOVE_JETPUNCH] = 90,
    [MOVE_KOWTOWCLEAVE] = 120,
    [MOVE_LASTRESPECTS] = 90,
    [MOVE_LUMINACRASH] = 100,
    [MOVE_MAGICALTORQUE] = 120,
    [MOVE_MAKEITRAIN] = 160,
    [MOVE_MORTALSPIN] = 80,
    [MOVE_NOXIOUSTORQUE] = 120,
    [MOVE_ORDERUP] = 120,
    [MOVE_POPULATIONBOMB] = 100,
    [MOVE_POUNCE] = 110,
    [MOVE_PSYBLADE] = 120,
    [MOVE_RAGEFIST] = 90,
    [MOVE_RAGINGBULL] = 120,
    [MOVE_RUINATION] = 100,
    [MOVE_SALTCURE] = 90,
    [MOVE_SPINOUT] = 140,
    [MOVE_TERABLAST] = 120,
    [MOVE_TORCHSONG] = 110,
    [MOVE_TRAILBLAZE] = 90,
    [MOVE_TRIPLEDIVE] = 120,
    [MOVE_TWINBEAM] = 120,
    [MOVE_WICKEDTORQUE] = 120,
    [MOVE_BLOODMOON] = 180,
    [MOVE_UPPERHAND] = 100,
    [MOVE_MATCHAGOTCHA] = 120,
    [MOVE_SYRUPBOMB] = 100,
    [MOVE_IVYCUDGEL] = 140,
    [MOVE_ELECTROSHOT] = 160,
    [MOVE_TERASTARSTORM] = 120,
    [MOVE_FICKLEBEAM] = 120,
    [MOVE_MIGHTYCLEAVE] = 120,
    [MOVE_TACHYONCUTTER] = 100,
    [MOVE_MALIGNANTCHAIN] = 120,
    [MOVE_THUNDERCLAP] = 110,
    [MOVE_HARDPRESS] = 120,
    [MOVE_ALLURINGVOICE] = 120,
    [MOVE_TEMPERFLARE] = 120,
    [MOVE_SUPERCELLSLAM] = 140,
    [MOVE_PSYCHICNOISE] = 120,
};
#endif
