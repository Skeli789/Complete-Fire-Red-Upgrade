#include "..\\defines.h"

struct BattleMove gBattleMoves[] = {
{				//MOVE_NONE
/*effect*/			EFFECT_HIT,
/*power*/			50,
/*type*/			TYPE_NORMAL,
/*accuracy*/		100,
/*pp*/				35,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			0,
/*z_move_power*/	100,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_POUND
/*effect*/			EFFECT_HIT,
/*power*/			40,
/*type*/			TYPE_NORMAL,
/*accuracy*/		100,
/*pp*/				35,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_KARATECHOP
/*effect*/			EFFECT_HIGH_CRITICAL,
/*power*/			50,
/*type*/			TYPE_FIGHTING,
/*accuracy*/		100,
/*pp*/				25,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_DOUBLESLAP
/*effect*/			EFFECT_MULTI_HIT,
/*power*/			15,
/*type*/			TYPE_NORMAL,
/*accuracy*/		85,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_COMETPUNCH
/*effect*/			EFFECT_MULTI_HIT,
/*power*/			18,
/*type*/			TYPE_NORMAL,
/*accuracy*/		85,
/*pp*/				15,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_MEGAPUNCH
/*effect*/			EFFECT_HIT,
/*power*/			80,
/*type*/			TYPE_NORMAL,
/*accuracy*/		85,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	160,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_PAYDAY
/*effect*/			EFFECT_PAY_DAY,
/*power*/			40,
/*type*/			TYPE_NORMAL,
/*accuracy*/		100,
/*pp*/				20,
/*effectChance*/	100,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_FIREPUNCH
/*effect*/			EFFECT_BURN_HIT,
/*power*/			75,
/*type*/			TYPE_FIRE,
/*accuracy*/		100,
/*pp*/				15,
/*effectChance*/	10,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	140,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_ICEPUNCH
/*effect*/			EFFECT_FREEZE_HIT,
/*power*/			75,
/*type*/			TYPE_ICE,
/*accuracy*/		100,
/*pp*/				15,
/*effectChance*/	10,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	140,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_THUNDERPUNCH
/*effect*/			EFFECT_PARALYZE_HIT,
/*power*/			75,
/*type*/			TYPE_ELECTRIC,
/*accuracy*/		100,
/*pp*/				15,
/*effectChance*/	10,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	140,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_SCRATCH
/*effect*/			EFFECT_HIT,
/*power*/			40,
/*type*/			TYPE_NORMAL,
/*accuracy*/		100,
/*pp*/				35,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_VICEGRIP
/*effect*/			EFFECT_HIT,
/*power*/			55,
/*type*/			TYPE_NORMAL,
/*accuracy*/		100,
/*pp*/				30,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_GUILLOTINE
/*effect*/			EFFECT_0HKO,
/*power*/			1,
/*type*/			TYPE_NORMAL,
/*accuracy*/		30,
/*pp*/				5,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_RAZORWIND
/*effect*/			EFFECT_RAZOR_WIND,
/*power*/			80,
/*type*/			TYPE_NORMAL,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_BOTH,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	160,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_SWORDSDANCE
/*effect*/			EFFECT_ATTACK_UP_2,
/*power*/			0,
/*type*/			TYPE_NORMAL,
/*accuracy*/		0,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			FLAG_SNATCH_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	1
},

{				//MOVE_CUT
/*effect*/			EFFECT_HIT,
/*power*/			50,
/*type*/			TYPE_NORMAL,
/*accuracy*/		95,
/*pp*/				30,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_GUST
/*effect*/			EFFECT_GUST,
/*power*/			40,
/*type*/			TYPE_FLYING,
/*accuracy*/		100,
/*pp*/				35,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_WINGATTACK
/*effect*/			EFFECT_HIT,
/*power*/			60,
/*type*/			TYPE_FLYING,
/*accuracy*/		100,
/*pp*/				35,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	120,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_WHIRLWIND
/*effect*/			EFFECT_ROAR,
/*power*/			0,
/*type*/			TYPE_NORMAL,
/*accuracy*/		0,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		250,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_FLY
/*effect*/			EFFECT_SEMI_INVULNERABLE,
/*power*/			90,
/*type*/			TYPE_FLYING,
/*accuracy*/		95,
/*pp*/				15,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	175,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_BIND
/*effect*/			EFFECT_TRAP,
/*power*/			15,
/*type*/			TYPE_NORMAL,
/*accuracy*/		85,
/*pp*/				20,
/*effectChance*/	100,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_SLAM
/*effect*/			EFFECT_HIT,
/*power*/			80,
/*type*/			TYPE_NORMAL,
/*accuracy*/		75,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	160,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_VINEWHIP
/*effect*/			EFFECT_HIT,
/*power*/			45,
/*type*/			TYPE_GRASS,
/*accuracy*/		100,
/*pp*/				25,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_STOMP
/*effect*/			EFFECT_FLINCH_HIT_2,
/*power*/			65,
/*type*/			TYPE_NORMAL,
/*accuracy*/		100,
/*pp*/				20,
/*effectChance*/	30,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	120,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_DOUBLEKICK
/*effect*/			EFFECT_DOUBLE_HIT,
/*power*/			30,
/*type*/			TYPE_FIGHTING,
/*accuracy*/		100,
/*pp*/				30,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_MEGAKICK
/*effect*/			EFFECT_HIT,
/*power*/			120,
/*type*/			TYPE_NORMAL,
/*accuracy*/		75,
/*pp*/				5,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	190,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_JUMPKICK
/*effect*/			EFFECT_RECOIL_IF_MISS,
/*power*/			100,
/*type*/			TYPE_FIGHTING,
/*accuracy*/		95,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	180,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_ROLLINGKICK
/*effect*/			EFFECT_FLINCH_HIT,
/*power*/			60,
/*type*/			TYPE_FIGHTING,
/*accuracy*/		85,
/*pp*/				15,
/*effectChance*/	30,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	120,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_SANDATTACK
/*effect*/			EFFECT_ACCURACY_DOWN,
/*power*/			0,
/*type*/			TYPE_GROUND,
/*accuracy*/		100,
/*pp*/				15,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_HEADBUTT
/*effect*/			EFFECT_FLINCH_HIT,
/*power*/			70,
/*type*/			TYPE_NORMAL,
/*accuracy*/		100,
/*pp*/				15,
/*effectChance*/	30,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	140,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_HORNATTACK
/*effect*/			EFFECT_HIT,
/*power*/			65,
/*type*/			TYPE_NORMAL,
/*accuracy*/		100,
/*pp*/				25,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	120,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_FURYATTACK
/*effect*/			EFFECT_MULTI_HIT,
/*power*/			1,
/*type*/			TYPE_NORMAL,
/*accuracy*/		85,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_HORNDRILL
/*effect*/			EFFECT_0HKO,
/*power*/			1,
/*type*/			TYPE_NORMAL,
/*accuracy*/		30,
/*pp*/				5,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_TACKLE
/*effect*/			EFFECT_HIT,
/*power*/			40,
/*type*/			TYPE_NORMAL,
/*accuracy*/		100,
/*pp*/				35,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_BODYSLAM
/*effect*/			EFFECT_FLINCH_HIT_2,
/*power*/			85,
/*type*/			TYPE_NORMAL,
/*accuracy*/		100,
/*pp*/				15,
/*effectChance*/	30,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	160,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_WRAP
/*effect*/			EFFECT_TRAP,
/*power*/			15,
/*type*/			TYPE_NORMAL,
/*accuracy*/		90,
/*pp*/				20,
/*effectChance*/	100,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_TAKEDOWN
/*effect*/			EFFECT_RECOIL,
/*power*/			90,
/*type*/			TYPE_NORMAL,
/*accuracy*/		85,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	175,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_THRASH
/*effect*/			EFFECT_RAMPAGE,
/*power*/			120,
/*type*/			TYPE_NORMAL,
/*accuracy*/		100,
/*pp*/				20,
/*effectChance*/	100,
/*target*/			MOVE_TARGET_RANDOM,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	190,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_DOUBLEEDGE
/*effect*/			EFFECT_DOUBLE_EDGE,
/*power*/			120,
/*type*/			TYPE_NORMAL,
/*accuracy*/		100,
/*pp*/				15,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	190,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_TAILWHIP
/*effect*/			EFFECT_DEFENSE_DOWN,
/*power*/			0,
/*type*/			TYPE_NORMAL,
/*accuracy*/		100,
/*pp*/				30,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_BOTH,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_POISONSTING
/*effect*/			EFFECT_POISON_HIT,
/*power*/			15,
/*type*/			TYPE_POISON,
/*accuracy*/		100,
/*pp*/				35,
/*effectChance*/	30,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_TWINEEDLE
/*effect*/			EFFECT_TWINEEDLE,
/*power*/			25,
/*type*/			TYPE_BUG,
/*accuracy*/		100,
/*pp*/				20,
/*effectChance*/	20,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_PINMISSILE
/*effect*/			EFFECT_MULTI_HIT,
/*power*/			25,
/*type*/			TYPE_BUG,
/*accuracy*/		95,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_LEER
/*effect*/			EFFECT_DEFENSE_DOWN,
/*power*/			0,
/*type*/			TYPE_NORMAL,
/*accuracy*/		100,
/*pp*/				30,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_BOTH,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_BITE
/*effect*/			EFFECT_FLINCH_HIT,
/*power*/			60,
/*type*/			TYPE_DARK,
/*accuracy*/		100,
/*pp*/				25,
/*effectChance*/	10,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	120,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_GROWL
/*effect*/			EFFECT_ATTACK_DOWN,
/*power*/			0,
/*type*/			TYPE_NORMAL,
/*accuracy*/		100,
/*pp*/				40,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_BOTH,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_ROAR
/*effect*/			EFFECT_ROAR,
/*power*/			0,
/*type*/			TYPE_NORMAL,
/*accuracy*/		0,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		250,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_SING
/*effect*/			EFFECT_SLEEP,
/*power*/			0,
/*type*/			TYPE_NORMAL,
/*accuracy*/		55,
/*pp*/				15,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_SUPERSONIC
/*effect*/			EFFECT_CONFUSE,
/*power*/			0,
/*type*/			TYPE_NORMAL,
/*accuracy*/		55,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_SONICBOOM
/*effect*/			EFFECT_SONICBOOM,
/*power*/			1,
/*type*/			TYPE_NORMAL,
/*accuracy*/		90,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_DISABLE
/*effect*/			EFFECT_DISABLE,
/*power*/			0,
/*type*/			TYPE_NORMAL,
/*accuracy*/		100,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_ACID
/*effect*/			EFFECT_SPECIAL_DEFENSE_DOWN_HIT,
/*power*/			50,
/*type*/			TYPE_POISON,
/*accuracy*/		100,
/*pp*/				30,
/*effectChance*/	10,
/*target*/			MOVE_TARGET_BOTH,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_EMBER
/*effect*/			EFFECT_BURN_HIT,
/*power*/			40,
/*type*/			TYPE_FIRE,
/*accuracy*/		100,
/*pp*/				25,
/*effectChance*/	10,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_FLAMETHROWER
/*effect*/			EFFECT_BURN_HIT,
/*power*/			95,
/*type*/			TYPE_FIRE,
/*accuracy*/		100,
/*pp*/				15,
/*effectChance*/	10,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	175,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_MIST
/*effect*/			EFFECT_MIST,
/*power*/			0,
/*type*/			TYPE_ICE,
/*accuracy*/		0,
/*pp*/				30,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			FLAG_SNATCH_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_WATERGUN
/*effect*/			EFFECT_HIT,
/*power*/			40,
/*type*/			TYPE_WATER,
/*accuracy*/		100,
/*pp*/				25,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_HYDROPUMP
/*effect*/			EFFECT_HIT,
/*power*/			120,
/*type*/			TYPE_WATER,
/*accuracy*/		80,
/*pp*/				5,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	190,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_SURF
/*effect*/			EFFECT_HIT,
/*power*/			95,
/*type*/			TYPE_WATER,
/*accuracy*/		100,
/*pp*/				15,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_FOES_AND_ALLY,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	175,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_ICEBEAM
/*effect*/			EFFECT_FREEZE_HIT,
/*power*/			95,
/*type*/			TYPE_ICE,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	10,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	175,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_BLIZZARD
/*effect*/			EFFECT_FREEZE_HIT,
/*power*/			120,
/*type*/			TYPE_ICE,
/*accuracy*/		70,
/*pp*/				5,
/*effectChance*/	10,
/*target*/			MOVE_TARGET_BOTH,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	190,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_PSYBEAM
/*effect*/			EFFECT_CONFUSE_HIT,
/*power*/			65,
/*type*/			TYPE_PSYCHIC,
/*accuracy*/		100,
/*pp*/				20,
/*effectChance*/	10,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	120,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_BUBBLEBEAM
/*effect*/			EFFECT_SPEED_DOWN_HIT,
/*power*/			65,
/*type*/			TYPE_WATER,
/*accuracy*/		100,
/*pp*/				20,
/*effectChance*/	10,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	120,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_AURORABEAM
/*effect*/			EFFECT_ATTACK_DOWN_HIT,
/*power*/			65,
/*type*/			TYPE_ICE,
/*accuracy*/		100,
/*pp*/				20,
/*effectChance*/	10,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	120,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_HYPERBEAM
/*effect*/			EFFECT_RECHARGE,
/*power*/			150,
/*type*/			TYPE_NORMAL,
/*accuracy*/		90,
/*pp*/				5,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	200,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_PECK
/*effect*/			EFFECT_HIT,
/*power*/			35,
/*type*/			TYPE_FLYING,
/*accuracy*/		100,
/*pp*/				35,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_DRILLPECK
/*effect*/			EFFECT_HIT,
/*power*/			80,
/*type*/			TYPE_FLYING,
/*accuracy*/		100,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	160,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_SUBMISSION
/*effect*/			EFFECT_RECOIL,
/*power*/			80,
/*type*/			TYPE_FIGHTING,
/*accuracy*/		80,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	160,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_LOWKICK
/*effect*/			EFFECT_LOW_KICK,
/*power*/			1,
/*type*/			TYPE_FIGHTING,
/*accuracy*/		100,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_COUNTER
/*effect*/			EFFECT_COUNTER,
/*power*/			1,
/*type*/			TYPE_FIGHTING,
/*accuracy*/		100,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_DEPENDS,
/*priority*/		251,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_SEISMICTOSS
/*effect*/			EFFECT_LEVEL_DAMAGE,
/*power*/			1,
/*type*/			TYPE_FIGHTING,
/*accuracy*/		100,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_STRENGTH
/*effect*/			EFFECT_HIT,
/*power*/			80,
/*type*/			TYPE_NORMAL,
/*accuracy*/		100,
/*pp*/				15,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	160,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_ABSORB
/*effect*/			EFFECT_ABSORB,
/*power*/			20,
/*type*/			TYPE_GRASS,
/*accuracy*/		100,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_TRIAGE_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_MEGADRAIN
/*effect*/			EFFECT_ABSORB,
/*power*/			40,
/*type*/			TYPE_GRASS,
/*accuracy*/		100,
/*pp*/				15,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_TRIAGE_AFFECTED,
/*z_move_power*/	120,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_LEECHSEED
/*effect*/			EFFECT_LEECH_SEED,
/*power*/			0,
/*type*/			TYPE_GRASS,
/*accuracy*/		90,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_GROWTH
/*effect*/			EFFECT_SPECIAL_ATTACK_UP,
/*power*/			0,
/*type*/			TYPE_NORMAL,
/*accuracy*/		0,
/*pp*/				40,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			FLAG_SNATCH_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_RAZORLEAF
/*effect*/			EFFECT_HIGH_CRITICAL,
/*power*/			55,
/*type*/			TYPE_GRASS,
/*accuracy*/		95,
/*pp*/				25,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_BOTH,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_SOLARBEAM
/*effect*/			EFFECT_SOLARBEAM,
/*power*/			120,
/*type*/			TYPE_GRASS,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	190,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_POISONPOWDER
/*effect*/			EFFECT_POISON,
/*power*/			0,
/*type*/			TYPE_POISON,
/*accuracy*/		75,
/*pp*/				35,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_STUNSPORE
/*effect*/			EFFECT_PARALYZE,
/*power*/			0,
/*type*/			TYPE_GRASS,
/*accuracy*/		75,
/*pp*/				30,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_SLEEPPOWDER
/*effect*/			EFFECT_SLEEP,
/*power*/			0,
/*type*/			TYPE_GRASS,
/*accuracy*/		75,
/*pp*/				15,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_PETALDANCE
/*effect*/			EFFECT_RAMPAGE,
/*power*/			120,
/*type*/			TYPE_GRASS,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	100,
/*target*/			MOVE_TARGET_RANDOM,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	190,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_STRINGSHOT
/*effect*/			EFFECT_SPEED_DOWN,
/*power*/			0,
/*type*/			TYPE_BUG,
/*accuracy*/		95,
/*pp*/				40,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_BOTH,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_DRAGONRAGE
/*effect*/			EFFECT_DRAGON_RAGE,
/*power*/			1,
/*type*/			TYPE_DRAGON,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_FIRESPIN
/*effect*/			EFFECT_TRAP,
/*power*/			35,
/*type*/			TYPE_FIRE,
/*accuracy*/		85,
/*pp*/				15,
/*effectChance*/	100,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_THUNDERSHOCK
/*effect*/			EFFECT_PARALYZE_HIT,
/*power*/			40,
/*type*/			TYPE_ELECTRIC,
/*accuracy*/		100,
/*pp*/				30,
/*effectChance*/	10,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_THUNDERBOLT
/*effect*/			EFFECT_PARALYZE_HIT,
/*power*/			95,
/*type*/			TYPE_ELECTRIC,
/*accuracy*/		100,
/*pp*/				15,
/*effectChance*/	10,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	175,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_THUNDERWAVE
/*effect*/			EFFECT_PARALYZE,
/*power*/			0,
/*type*/			TYPE_ELECTRIC,
/*accuracy*/		100,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_THUNDER
/*effect*/			EFFECT_THUNDER,
/*power*/			120,
/*type*/			TYPE_ELECTRIC,
/*accuracy*/		70,
/*pp*/				10,
/*effectChance*/	30,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	190,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_ROCKTHROW
/*effect*/			EFFECT_HIT,
/*power*/			50,
/*type*/			TYPE_ROCK,
/*accuracy*/		90,
/*pp*/				15,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_EARTHQUAKE
/*effect*/			EFFECT_EARTHQUAKE,
/*power*/			100,
/*type*/			TYPE_GROUND,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_FOES_AND_ALLY,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	180,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_FISSURE
/*effect*/			EFFECT_0HKO,
/*power*/			1,
/*type*/			TYPE_GROUND,
/*accuracy*/		30,
/*pp*/				5,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_DIG
/*effect*/			EFFECT_SEMI_INVULNERABLE,
/*power*/			80,
/*type*/			TYPE_GROUND,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	160,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_TOXIC
/*effect*/			EFFECT_TOXIC,
/*power*/			0,
/*type*/			TYPE_POISON,
/*accuracy*/		90,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_CONFUSION
/*effect*/			EFFECT_CONFUSE_HIT,
/*power*/			50,
/*type*/			TYPE_PSYCHIC,
/*accuracy*/		100,
/*pp*/				25,
/*effectChance*/	10,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_PSYCHIC
/*effect*/			EFFECT_SPECIAL_DEFENSE_DOWN_HIT,
/*power*/			90,
/*type*/			TYPE_PSYCHIC,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	10,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	175,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_HYPNOSIS
/*effect*/			EFFECT_SLEEP,
/*power*/			0,
/*type*/			TYPE_PSYCHIC,
/*accuracy*/		100,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_MEDITATE
/*effect*/			EFFECT_ATTACK_UP,
/*power*/			0,
/*type*/			TYPE_PSYCHIC,
/*accuracy*/		0,
/*pp*/				40,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			FLAG_SNATCH_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_AGILITY
/*effect*/			EFFECT_SPEED_UP_2,
/*power*/			0,
/*type*/			TYPE_PSYCHIC,
/*accuracy*/		0,
/*pp*/				30,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			FLAG_SNATCH_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_QUICKATTACK
/*effect*/			EFFECT_QUICK_ATTACK,
/*power*/			40,
/*type*/			TYPE_NORMAL,
/*accuracy*/		100,
/*pp*/				30,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		1,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_RAGE
/*effect*/			EFFECT_RAGE,
/*power*/			20,
/*type*/			TYPE_NORMAL,
/*accuracy*/		100,
/*pp*/				20,
/*effectChance*/	100,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_TELEPORT
/*effect*/			EFFECT_TELEPORT,
/*power*/			0,
/*type*/			TYPE_PSYCHIC,
/*accuracy*/		0,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			0,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_NIGHTSHADE
/*effect*/			EFFECT_LEVEL_DAMAGE,
/*power*/			1,
/*type*/			TYPE_GHOST,
/*accuracy*/		100,
/*pp*/				15,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_MIMIC
/*effect*/			EFFECT_MIMIC,
/*power*/			0,
/*type*/			TYPE_NORMAL,
/*accuracy*/		0,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_SCREECH
/*effect*/			EFFECT_DEFENSE_DOWN_2,
/*power*/			0,
/*type*/			TYPE_NORMAL,
/*accuracy*/		85,
/*pp*/				40,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_DOUBLETEAM
/*effect*/			EFFECT_EVASION_UP,
/*power*/			0,
/*type*/			TYPE_NORMAL,
/*accuracy*/		0,
/*pp*/				15,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			FLAG_SNATCH_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_RECOVER
/*effect*/			EFFECT_RESTORE_HP,
/*power*/			0,
/*type*/			TYPE_NORMAL,
/*accuracy*/		0,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			FLAG_SNATCH_AFFECTED | FLAG_TRIAGE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	26
},

{				//MOVE_HARDEN
/*effect*/			EFFECT_DEFENSE_UP,
/*power*/			0,
/*type*/			TYPE_NORMAL,
/*accuracy*/		0,
/*pp*/				30,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			FLAG_SNATCH_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_MINIMIZE
/*effect*/			EFFECT_MINIMIZE,
/*power*/			0,
/*type*/			TYPE_NORMAL,
/*accuracy*/		0,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			FLAG_SNATCH_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_SMOKESCREEN
/*effect*/			EFFECT_ACCURACY_DOWN,
/*power*/			0,
/*type*/			TYPE_NORMAL,
/*accuracy*/		100,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_CONFUSERAY
/*effect*/			EFFECT_CONFUSE,
/*power*/			0,
/*type*/			TYPE_GHOST,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_WITHDRAW
/*effect*/			EFFECT_DEFENSE_UP,
/*power*/			0,
/*type*/			TYPE_WATER,
/*accuracy*/		0,
/*pp*/				40,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			FLAG_SNATCH_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_DEFENSECURL
/*effect*/			EFFECT_DEFENSE_CURL,
/*power*/			0,
/*type*/			TYPE_NORMAL,
/*accuracy*/		0,
/*pp*/				40,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			FLAG_SNATCH_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_BARRIER
/*effect*/			EFFECT_DEFENSE_UP_2,
/*power*/			0,
/*type*/			TYPE_PSYCHIC,
/*accuracy*/		0,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			FLAG_SNATCH_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	1
},

{				//MOVE_LIGHTSCREEN
/*effect*/			EFFECT_LIGHT_SCREEN,
/*power*/			0,
/*type*/			TYPE_PSYCHIC,
/*accuracy*/		0,
/*pp*/				30,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			FLAG_SNATCH_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_HAZE
/*effect*/			EFFECT_HAZE,
/*power*/			0,
/*type*/			TYPE_ICE,
/*accuracy*/		0,
/*pp*/				30,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			0,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_REFLECT
/*effect*/			EFFECT_REFLECT,
/*power*/			0,
/*type*/			TYPE_PSYCHIC,
/*accuracy*/		0,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			FLAG_SNATCH_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_FOCUSENERGY
/*effect*/			EFFECT_FOCUS_ENERGY,
/*power*/			0,
/*type*/			TYPE_NORMAL,
/*accuracy*/		0,
/*pp*/				30,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			FLAG_SNATCH_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_BIDE
/*effect*/			EFFECT_BIDE,
/*power*/			1,
/*type*/			TYPE_NORMAL,
/*accuracy*/		0,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		1,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_METRONOME
/*effect*/			EFFECT_METRONOME,
/*power*/			0,
/*type*/			TYPE_NORMAL,
/*accuracy*/		0,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_DEPENDS,
/*priority*/		0,
/*flags*/			0,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_MIRRORMOVE
/*effect*/			EFFECT_MIRROR_MOVE,
/*power*/			0,
/*type*/			TYPE_FLYING,
/*accuracy*/		0,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_DEPENDS,
/*priority*/		0,
/*flags*/			0,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_SELFDESTRUCT
/*effect*/			EFFECT_EXPLOSION,
/*power*/			200,
/*type*/			TYPE_NORMAL,
/*accuracy*/		100,
/*pp*/				5,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_FOES_AND_ALLY,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	200,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_EGGBOMB
/*effect*/			EFFECT_HIT,
/*power*/			100,
/*type*/			TYPE_NORMAL,
/*accuracy*/		75,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	180,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_LICK
/*effect*/			EFFECT_PARALYZE_HIT,
/*power*/			30,
/*type*/			TYPE_GHOST,
/*accuracy*/		100,
/*pp*/				30,
/*effectChance*/	30,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_SMOG
/*effect*/			EFFECT_POISON_HIT,
/*power*/			30,
/*type*/			TYPE_POISON,
/*accuracy*/		70,
/*pp*/				20,
/*effectChance*/	40,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_SLUDGE
/*effect*/			EFFECT_POISON_HIT,
/*power*/			65,
/*type*/			TYPE_POISON,
/*accuracy*/		100,
/*pp*/				20,
/*effectChance*/	30,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	120,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_BONECLUB
/*effect*/			EFFECT_FLINCH_HIT,
/*power*/			65,
/*type*/			TYPE_GROUND,
/*accuracy*/		85,
/*pp*/				20,
/*effectChance*/	10,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	120,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_FIREBLAST
/*effect*/			EFFECT_BURN_HIT,
/*power*/			120,
/*type*/			TYPE_FIRE,
/*accuracy*/		85,
/*pp*/				5,
/*effectChance*/	10,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	190,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_WATERFALL
/*effect*/			EFFECT_FLINCH_HIT,
/*power*/			80,
/*type*/			TYPE_WATER,
/*accuracy*/		100,
/*pp*/				15,
/*effectChance*/	20,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	160,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_CLAMP
/*effect*/			EFFECT_TRAP,
/*power*/			35,
/*type*/			TYPE_WATER,
/*accuracy*/		85,
/*pp*/				10,
/*effectChance*/	100,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_SWIFT
/*effect*/			EFFECT_ALWAYS_HIT,
/*power*/			60,
/*type*/			TYPE_NORMAL,
/*accuracy*/		0,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_BOTH,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	120,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_SKULLBASH
/*effect*/			EFFECT_SKULL_BASH,
/*power*/			130,
/*type*/			TYPE_NORMAL,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	195,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_SPIKECANNON
/*effect*/			EFFECT_MULTI_HIT,
/*power*/			20,
/*type*/			TYPE_NORMAL,
/*accuracy*/		100,
/*pp*/				15,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_CONSTRICT
/*effect*/			EFFECT_SPEED_DOWN_HIT,
/*power*/			10,
/*type*/			TYPE_NORMAL,
/*accuracy*/		100,
/*pp*/				35,
/*effectChance*/	10,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_AMNESIA
/*effect*/			EFFECT_SPECIAL_DEFENSE_UP_2,
/*power*/			0,
/*type*/			TYPE_PSYCHIC,
/*accuracy*/		0,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			FLAG_SNATCH_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_KINESIS
/*effect*/			EFFECT_ACCURACY_DOWN,
/*power*/			0,
/*type*/			TYPE_PSYCHIC,
/*accuracy*/		80,
/*pp*/				15,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_SOFTBOILED
/*effect*/			EFFECT_SOFTBOILED,
/*power*/			0,
/*type*/			TYPE_NORMAL,
/*accuracy*/		0,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			FLAG_SNATCH_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_TRIAGE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_HIGHJUMPKICK
/*effect*/			EFFECT_RECOIL_IF_MISS,
/*power*/			130,
/*type*/			TYPE_FIGHTING,
/*accuracy*/		90,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	195,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_GLARE
/*effect*/			EFFECT_PARALYZE,
/*power*/			0,
/*type*/			TYPE_NORMAL,
/*accuracy*/		100,
/*pp*/				30,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_DREAMEATER
/*effect*/			EFFECT_DREAM_EATER,
/*power*/			100,
/*type*/			TYPE_PSYCHIC,
/*accuracy*/		100,
/*pp*/				15,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_TRIAGE_AFFECTED,
/*z_move_power*/	180,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_POISONGAS
/*effect*/			EFFECT_POISON,
/*power*/			0,
/*type*/			TYPE_POISON,
/*accuracy*/		90,
/*pp*/				40,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_BOTH,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_BARRAGE
/*effect*/			EFFECT_MULTI_HIT,
/*power*/			15,
/*type*/			TYPE_NORMAL,
/*accuracy*/		85,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_LEECHLIFE
/*effect*/			EFFECT_ABSORB,
/*power*/			20,
/*type*/			TYPE_BUG,
/*accuracy*/		100,
/*pp*/				15,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_TRIAGE_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_LOVELYKISS
/*effect*/			EFFECT_SLEEP,
/*power*/			0,
/*type*/			TYPE_NORMAL,
/*accuracy*/		75,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_SKYATTACK
/*effect*/			EFFECT_SKY_ATTACK,
/*power*/			140,
/*type*/			TYPE_FLYING,
/*accuracy*/		90,
/*pp*/				5,
/*effectChance*/	30,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	200,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_TRANSFORM
/*effect*/			EFFECT_TRANSFORM,
/*power*/			0,
/*type*/			TYPE_NORMAL,
/*accuracy*/		0,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			0,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_BUBBLE
/*effect*/			EFFECT_SPEED_DOWN_HIT,
/*power*/			40,
/*type*/			TYPE_WATER,
/*accuracy*/		100,
/*pp*/				30,
/*effectChance*/	10,
/*target*/			MOVE_TARGET_BOTH,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_DIZZYPUNCH
/*effect*/			EFFECT_CONFUSE_HIT,
/*power*/			70,
/*type*/			TYPE_NORMAL,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	20,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	140,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_SPORE
/*effect*/			EFFECT_SLEEP,
/*power*/			0,
/*type*/			TYPE_GRASS,
/*accuracy*/		100,
/*pp*/				15,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_FLASH
/*effect*/			EFFECT_ACCURACY_DOWN,
/*power*/			0,
/*type*/			TYPE_NORMAL,
/*accuracy*/		70,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_PSYWAVE
/*effect*/			EFFECT_PSYWAVE,
/*power*/			1,
/*type*/			TYPE_PSYCHIC,
/*accuracy*/		100,
/*pp*/				15,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_SPLASH
/*effect*/			EFFECT_SPLASH,
/*power*/			0,
/*type*/			TYPE_NORMAL,
/*accuracy*/		0,
/*pp*/				40,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			0,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_ACIDARMOR
/*effect*/			EFFECT_DEFENSE_UP_2,
/*power*/			0,
/*type*/			TYPE_POISON,
/*accuracy*/		0,
/*pp*/				40,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			FLAG_SNATCH_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_CRABHAMMER
/*effect*/			EFFECT_HIGH_CRITICAL,
/*power*/			100,
/*type*/			TYPE_WATER,
/*accuracy*/		90,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	180,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_EXPLOSION
/*effect*/			EFFECT_EXPLOSION,
/*power*/			250,
/*type*/			TYPE_NORMAL,
/*accuracy*/		100,
/*pp*/				5,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_FOES_AND_ALLY,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	200,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_FURYSWIPES
/*effect*/			EFFECT_MULTI_HIT,
/*power*/			18,
/*type*/			TYPE_NORMAL,
/*accuracy*/		80,
/*pp*/				15,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_BONEMERANG
/*effect*/			EFFECT_DOUBLE_HIT,
/*power*/			50,
/*type*/			TYPE_GROUND,
/*accuracy*/		90,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_REST
/*effect*/			EFFECT_REST,
/*power*/			0,
/*type*/			TYPE_PSYCHIC,
/*accuracy*/		0,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			FLAG_SNATCH_AFFECTED | FLAG_TRIAGE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_ROCKSLIDE
/*effect*/			EFFECT_FLINCH_HIT,
/*power*/			75,
/*type*/			TYPE_ROCK,
/*accuracy*/		90,
/*pp*/				10,
/*effectChance*/	30,
/*target*/			MOVE_TARGET_BOTH,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	140,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_HYPERFANG
/*effect*/			EFFECT_FLINCH_HIT,
/*power*/			80,
/*type*/			TYPE_NORMAL,
/*accuracy*/		90,
/*pp*/				15,
/*effectChance*/	10,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	160,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_SHARPEN
/*effect*/			EFFECT_ATTACK_UP,
/*power*/			0,
/*type*/			TYPE_NORMAL,
/*accuracy*/		0,
/*pp*/				30,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			FLAG_SNATCH_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_CONVERSION
/*effect*/			EFFECT_CONVERSION,
/*power*/			0,
/*type*/			TYPE_NORMAL,
/*accuracy*/		0,
/*pp*/				30,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			FLAG_SNATCH_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_TRIATTACK
/*effect*/			EFFECT_TRI_ATTACK,
/*power*/			80,
/*type*/			TYPE_NORMAL,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	20,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	160,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_SUPERFANG
/*effect*/			EFFECT_SUPER_FANG,
/*power*/			1,
/*type*/			TYPE_NORMAL,
/*accuracy*/		90,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_SLASH
/*effect*/			EFFECT_HIGH_CRITICAL,
/*power*/			70,
/*type*/			TYPE_NORMAL,
/*accuracy*/		100,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	140,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_SUBSTITUTE
/*effect*/			EFFECT_SUBSTITUTE,
/*power*/			0,
/*type*/			TYPE_NORMAL,
/*accuracy*/		0,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			FLAG_SNATCH_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_STRUGGLE
/*effect*/			EFFECT_RECOIL,
/*power*/			50,
/*type*/			TYPE_NORMAL,
/*accuracy*/		0,
/*pp*/				1,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	1,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_SKETCH
/*effect*/			EFFECT_SKETCH,
/*power*/			0,
/*type*/			TYPE_NORMAL,
/*accuracy*/		0,
/*pp*/				1,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			0,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_TRIPLEKICK
/*effect*/			EFFECT_TRIPLE_KICK,
/*power*/			10,
/*type*/			TYPE_FIGHTING,
/*accuracy*/		90,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_THIEF
/*effect*/			EFFECT_THIEF,
/*power*/			60,
/*type*/			TYPE_DARK,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	100,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	120,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_SPIDERWEB
/*effect*/			EFFECT_MEAN_LOOK,
/*power*/			0,
/*type*/			TYPE_BUG,
/*accuracy*/		0,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_MINDREADER
/*effect*/			EFFECT_LOCK_ON,
/*power*/			0,
/*type*/			TYPE_NORMAL,
/*accuracy*/		0,
/*pp*/				5,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_NIGHTMARE
/*effect*/			EFFECT_NIGHTMARE,
/*power*/			0,
/*type*/			TYPE_GHOST,
/*accuracy*/		0,
/*pp*/				15,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_FLAMEWHEEL
/*effect*/			EFFECT_THAW_HIT,
/*power*/			60,
/*type*/			TYPE_FIRE,
/*accuracy*/		100,
/*pp*/				25,
/*effectChance*/	10,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	120,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_SNORE
/*effect*/			EFFECT_SNORE,
/*power*/			50,
/*type*/			TYPE_NORMAL,
/*accuracy*/		100,
/*pp*/				15,
/*effectChance*/	30,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_CURSE
/*effect*/			EFFECT_CURSE,
/*power*/			0,
/*type*/			TYPE_GHOST,
/*accuracy*/		0,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			0,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_FLAIL
/*effect*/			EFFECT_FLAIL,
/*power*/			1,
/*type*/			TYPE_NORMAL,
/*accuracy*/		100,
/*pp*/				15,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_CONVERSION2
/*effect*/			EFFECT_CONVERSION_2,
/*power*/			0,
/*type*/			TYPE_NORMAL,
/*accuracy*/		0,
/*pp*/				30,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			0,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_AEROBLAST
/*effect*/			EFFECT_HIGH_CRITICAL,
/*power*/			100,
/*type*/			TYPE_FLYING,
/*accuracy*/		95,
/*pp*/				5,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	180,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_COTTONSPORE
/*effect*/			EFFECT_SPEED_DOWN_2,
/*power*/			0,
/*type*/			TYPE_GRASS,
/*accuracy*/		85,
/*pp*/				40,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_BOTH,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_REVERSAL
/*effect*/			EFFECT_FLAIL,
/*power*/			1,
/*type*/			TYPE_FIGHTING,
/*accuracy*/		100,
/*pp*/				15,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_SPITE
/*effect*/			EFFECT_SPITE,
/*power*/			0,
/*type*/			TYPE_GHOST,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_POWDERSNOW
/*effect*/			EFFECT_FREEZE_HIT,
/*power*/			40,
/*type*/			TYPE_ICE,
/*accuracy*/		100,
/*pp*/				25,
/*effectChance*/	10,
/*target*/			MOVE_TARGET_BOTH,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_PROTECT
/*effect*/			EFFECT_PROTECT,
/*power*/			0,
/*type*/			TYPE_NORMAL,
/*accuracy*/		0,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		4,
/*flags*/			0,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_MACHPUNCH
/*effect*/			EFFECT_QUICK_ATTACK,
/*power*/			40,
/*type*/			TYPE_FIGHTING,
/*accuracy*/		100,
/*pp*/				30,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		1,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_SCARYFACE
/*effect*/			EFFECT_SPEED_DOWN_2,
/*power*/			0,
/*type*/			TYPE_NORMAL,
/*accuracy*/		90,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_FEINTATTACK
/*effect*/			EFFECT_ALWAYS_HIT,
/*power*/			60,
/*type*/			TYPE_DARK,
/*accuracy*/		0,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	120,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_SWEETKISS
/*effect*/			EFFECT_CONFUSE,
/*power*/			0,
/*type*/			TYPE_FAIRY,
/*accuracy*/		75,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_BELLYDRUM
/*effect*/			EFFECT_BELLY_DRUM,
/*power*/			0,
/*type*/			TYPE_NORMAL,
/*accuracy*/		0,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			FLAG_SNATCH_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_SLUDGEBOMB
/*effect*/			EFFECT_POISON_HIT,
/*power*/			90,
/*type*/			TYPE_POISON,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	30,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	175,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_MUDSLAP
/*effect*/			EFFECT_ACCURACY_DOWN_HIT,
/*power*/			35,
/*type*/			TYPE_GROUND,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	100,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_OCTAZOOKA
/*effect*/			EFFECT_ACCURACY_DOWN_HIT,
/*power*/			65,
/*type*/			TYPE_WATER,
/*accuracy*/		85,
/*pp*/				10,
/*effectChance*/	50,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	120,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_SPIKES
/*effect*/			EFFECT_SPIKES,
/*power*/			0,
/*type*/			TYPE_GROUND,
/*accuracy*/		0,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_OPPONENTS_FIELD,
/*priority*/		0,
/*flags*/			FLAG_MAGIC_COAT_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_ZAPCANNON
/*effect*/			EFFECT_PARALYZE_HIT,
/*power*/			120,
/*type*/			TYPE_ELECTRIC,
/*accuracy*/		50,
/*pp*/				5,
/*effectChance*/	100,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	190,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_FORESIGHT
/*effect*/			EFFECT_FORESIGHT,
/*power*/			0,
/*type*/			TYPE_NORMAL,
/*accuracy*/		0,
/*pp*/				40,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_DESTINYBOND
/*effect*/			EFFECT_DESTINY_BOND,
/*power*/			0,
/*type*/			TYPE_GHOST,
/*accuracy*/		0,
/*pp*/				5,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			0,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_PERISHSONG
/*effect*/			EFFECT_PERISH_SONG,
/*power*/			0,
/*type*/			TYPE_NORMAL,
/*accuracy*/		0,
/*pp*/				5,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			0,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_ICYWIND
/*effect*/			EFFECT_SPEED_DOWN_HIT,
/*power*/			55,
/*type*/			TYPE_ICE,
/*accuracy*/		95,
/*pp*/				15,
/*effectChance*/	100,
/*target*/			MOVE_TARGET_BOTH,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_DETECT
/*effect*/			EFFECT_PROTECT,
/*power*/			0,
/*type*/			TYPE_FIGHTING,
/*accuracy*/		0,
/*pp*/				5,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		4,
/*flags*/			0,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_BONERUSH
/*effect*/			EFFECT_MULTI_HIT,
/*power*/			25,
/*type*/			TYPE_GROUND,
/*accuracy*/		80,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_LOCKON
/*effect*/			EFFECT_LOCK_ON,
/*power*/			0,
/*type*/			TYPE_NORMAL,
/*accuracy*/		0,
/*pp*/				5,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_OUTRAGE
/*effect*/			EFFECT_RAMPAGE,
/*power*/			120,
/*type*/			TYPE_DRAGON,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	100,
/*target*/			MOVE_TARGET_RANDOM,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	190,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_SANDSTORM
/*effect*/			EFFECT_SANDSTORM,
/*power*/			0,
/*type*/			TYPE_ROCK,
/*accuracy*/		0,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			0,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_GIGADRAIN
/*effect*/			EFFECT_ABSORB,
/*power*/			75,
/*type*/			TYPE_GRASS,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_TRIAGE_AFFECTED,
/*z_move_power*/	140,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_ENDURE
/*effect*/			EFFECT_ENDURE,
/*power*/			0,
/*type*/			TYPE_NORMAL,
/*accuracy*/		0,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		4,
/*flags*/			0,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_CHARM
/*effect*/			EFFECT_ATTACK_DOWN_2,
/*power*/			0,
/*type*/			TYPE_FAIRY,
/*accuracy*/		100,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_ROLLOUT
/*effect*/			EFFECT_ROLLOUT,
/*power*/			30,
/*type*/			TYPE_ROCK,
/*accuracy*/		90,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_FALSESWIPE
/*effect*/			EFFECT_FALSE_SWIPE,
/*power*/			40,
/*type*/			TYPE_NORMAL,
/*accuracy*/		100,
/*pp*/				40,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_SWAGGER
/*effect*/			EFFECT_SWAGGER,
/*power*/			0,
/*type*/			TYPE_NORMAL,
/*accuracy*/		85,
/*pp*/				15,
/*effectChance*/	100,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_MILKDRINK
/*effect*/			EFFECT_SOFTBOILED,
/*power*/			0,
/*type*/			TYPE_NORMAL,
/*accuracy*/		0,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_SNATCH_AFFECTED | FLAG_TRIAGE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_SPARK
/*effect*/			EFFECT_PARALYZE_HIT,
/*power*/			65,
/*type*/			TYPE_ELECTRIC,
/*accuracy*/		100,
/*pp*/				20,
/*effectChance*/	30,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	120,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_FURYCUTTER
/*effect*/			EFFECT_FURY_CUTTER,
/*power*/			40,
/*type*/			TYPE_BUG,
/*accuracy*/		95,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_STEELWING
/*effect*/			EFFECT_DEFENSE_UP_HIT,
/*power*/			70,
/*type*/			TYPE_STEEL,
/*accuracy*/		90,
/*pp*/				25,
/*effectChance*/	10,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	140,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_MEANLOOK
/*effect*/			EFFECT_MEAN_LOOK,
/*power*/			0,
/*type*/			TYPE_NORMAL,
/*accuracy*/		0,
/*pp*/				5,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_ATTRACT
/*effect*/			EFFECT_ATTRACT,
/*power*/			0,
/*type*/			TYPE_NORMAL,
/*accuracy*/		100,
/*pp*/				15,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_SLEEPTALK
/*effect*/			EFFECT_SLEEP_TALK,
/*power*/			0,
/*type*/			TYPE_NORMAL,
/*accuracy*/		0,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_DEPENDS,
/*priority*/		0,
/*flags*/			0,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_HEALBELL
/*effect*/			EFFECT_HEAL_BELL,
/*power*/			0,
/*type*/			TYPE_NORMAL,
/*accuracy*/		0,
/*pp*/				5,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			FLAG_SNATCH_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_RETURN
/*effect*/			EFFECT_RETURN,
/*power*/			1,
/*type*/			TYPE_NORMAL,
/*accuracy*/		100,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_PRESENT
/*effect*/			EFFECT_PRESENT,
/*power*/			1,
/*type*/			TYPE_NORMAL,
/*accuracy*/		90,
/*pp*/				15,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_FRUSTRATION
/*effect*/			EFFECT_FRUSTRATION,
/*power*/			1,
/*type*/			TYPE_NORMAL,
/*accuracy*/		100,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_SAFEGUARD
/*effect*/			EFFECT_SAFEGUARD,
/*power*/			0,
/*type*/			TYPE_NORMAL,
/*accuracy*/		0,
/*pp*/				25,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			FLAG_SNATCH_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_PAINSPLIT
/*effect*/			EFFECT_PAIN_SPLIT,
/*power*/			0,
/*type*/			TYPE_NORMAL,
/*accuracy*/		0,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_SACREDFIRE
/*effect*/			EFFECT_THAW_HIT,
/*power*/			100,
/*type*/			TYPE_FIRE,
/*accuracy*/		95,
/*pp*/				5,
/*effectChance*/	50,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	180,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_MAGNITUDE
/*effect*/			EFFECT_MAGNITUDE,
/*power*/			1,
/*type*/			TYPE_GROUND,
/*accuracy*/		100,
/*pp*/				30,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_FOES_AND_ALLY,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_DYNAMICPUNCH
/*effect*/			EFFECT_CONFUSE_HIT,
/*power*/			100,
/*type*/			TYPE_FIGHTING,
/*accuracy*/		50,
/*pp*/				5,
/*effectChance*/	100,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	180,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_MEGAHORN
/*effect*/			EFFECT_HIT,
/*power*/			120,
/*type*/			TYPE_BUG,
/*accuracy*/		85,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	190,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_DRAGONBREATH
/*effect*/			EFFECT_PARALYZE_HIT,
/*power*/			60,
/*type*/			TYPE_DRAGON,
/*accuracy*/		100,
/*pp*/				20,
/*effectChance*/	30,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	120,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_BATONPASS
/*effect*/			EFFECT_BATON_PASS,
/*power*/			0,
/*type*/			TYPE_NORMAL,
/*accuracy*/		0,
/*pp*/				40,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			0,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_ENCORE
/*effect*/			EFFECT_ENCORE,
/*power*/			0,
/*type*/			TYPE_NORMAL,
/*accuracy*/		100,
/*pp*/				5,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_PURSUIT
/*effect*/			EFFECT_PURSUIT,
/*power*/			40,
/*type*/			TYPE_DARK,
/*accuracy*/		100,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_RAPIDSPIN
/*effect*/			EFFECT_RAPID_SPIN,
/*power*/			20,
/*type*/			TYPE_NORMAL,
/*accuracy*/		100,
/*pp*/				40,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_SWEETSCENT
/*effect*/			EFFECT_EVASION_DOWN,
/*power*/			0,
/*type*/			TYPE_NORMAL,
/*accuracy*/		100,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_BOTH,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_IRONTAIL
/*effect*/			EFFECT_DEFENSE_DOWN_HIT,
/*power*/			100,
/*type*/			TYPE_STEEL,
/*accuracy*/		75,
/*pp*/				15,
/*effectChance*/	30,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	180,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_METALCLAW
/*effect*/			EFFECT_ATTACK_UP_HIT,
/*power*/			50,
/*type*/			TYPE_STEEL,
/*accuracy*/		95,
/*pp*/				35,
/*effectChance*/	10,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_VITALTHROW
/*effect*/			EFFECT_VITAL_THROW,
/*power*/			70,
/*type*/			TYPE_FIGHTING,
/*accuracy*/		0,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		255,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	140,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_MORNINGSUN
/*effect*/			EFFECT_MORNING_SUN,
/*power*/			0,
/*type*/			TYPE_NORMAL,
/*accuracy*/		0,
/*pp*/				5,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			FLAG_SNATCH_AFFECTED | FLAG_TRIAGE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_SYNTHESIS
/*effect*/			EFFECT_SYNTHESIS,
/*power*/			0,
/*type*/			TYPE_GRASS,
/*accuracy*/		0,
/*pp*/				5,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			FLAG_SNATCH_AFFECTED | FLAG_TRIAGE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_MOONLIGHT
/*effect*/			EFFECT_MOONLIGHT,
/*power*/			0,
/*type*/			TYPE_FAIRY,
/*accuracy*/		0,
/*pp*/				5,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			FLAG_SNATCH_AFFECTED | FLAG_TRIAGE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_HIDDENPOWER
/*effect*/			EFFECT_HIDDEN_POWER,
/*power*/			70,
/*type*/			TYPE_NORMAL,
/*accuracy*/		100,
/*pp*/				15,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	140,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_CROSSCHOP
/*effect*/			EFFECT_HIGH_CRITICAL,
/*power*/			100,
/*type*/			TYPE_FIGHTING,
/*accuracy*/		80,
/*pp*/				5,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	180,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_TWISTER
/*effect*/			EFFECT_TWISTER,
/*power*/			40,
/*type*/			TYPE_DRAGON,
/*accuracy*/		100,
/*pp*/				20,
/*effectChance*/	20,
/*target*/			MOVE_TARGET_BOTH,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_RAINDANCE
/*effect*/			EFFECT_RAIN_DANCE,
/*power*/			0,
/*type*/			TYPE_WATER,
/*accuracy*/		0,
/*pp*/				5,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			0,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_SUNNYDAY
/*effect*/			EFFECT_SUNNY_DAY,
/*power*/			0,
/*type*/			TYPE_FIRE,
/*accuracy*/		0,
/*pp*/				5,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			0,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_CRUNCH
/*effect*/			EFFECT_DEFENSE_DOWN_HIT,
/*power*/			80,
/*type*/			TYPE_DARK,
/*accuracy*/		100,
/*pp*/				15,
/*effectChance*/	20,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	160,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_MIRRORCOAT
/*effect*/			EFFECT_MIRROR_COAT,
/*power*/			1,
/*type*/			TYPE_PSYCHIC,
/*accuracy*/		100,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_DEPENDS,
/*priority*/		251,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_PSYCHUP
/*effect*/			EFFECT_PSYCH_UP,
/*power*/			0,
/*type*/			TYPE_NORMAL,
/*accuracy*/		0,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_SNATCH_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_EXTREMESPEED
/*effect*/			EFFECT_QUICK_ATTACK,
/*power*/			80,
/*type*/			TYPE_NORMAL,
/*accuracy*/		100,
/*pp*/				5,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		2,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	160,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_ANCIENTPOWER
/*effect*/			EFFECT_ALL_STATS_UP_HIT,
/*power*/			60,
/*type*/			TYPE_ROCK,
/*accuracy*/		100,
/*pp*/				5,
/*effectChance*/	10,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	120,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_SHADOWBALL
/*effect*/			EFFECT_SPECIAL_DEFENSE_DOWN_HIT,
/*power*/			80,
/*type*/			TYPE_GHOST,
/*accuracy*/		100,
/*pp*/				15,
/*effectChance*/	20,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	160,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_FUTURESIGHT
/*effect*/			EFFECT_FUTURE_SIGHT,
/*power*/			120,
/*type*/			TYPE_PSYCHIC,
/*accuracy*/		100,
/*pp*/				15,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			0,
/*z_move_power*/	190,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_ROCKSMASH
/*effect*/			EFFECT_DEFENSE_DOWN_HIT,
/*power*/			40,
/*type*/			TYPE_FIGHTING,
/*accuracy*/		100,
/*pp*/				15,
/*effectChance*/	50,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_WHIRLPOOL
/*effect*/			EFFECT_TRAP,
/*power*/			35,
/*type*/			TYPE_WATER,
/*accuracy*/		85,
/*pp*/				15,
/*effectChance*/	100,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_BEATUP
/*effect*/			EFFECT_BEAT_UP,
/*power*/			0,
/*type*/			TYPE_DARK,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_FAKEOUT
/*effect*/			EFFECT_FAKE_OUT,
/*power*/			40,
/*type*/			TYPE_NORMAL,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		3,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_UPROAR
/*effect*/			EFFECT_UPROAR,
/*power*/			90,
/*type*/			TYPE_NORMAL,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	100,
/*target*/			MOVE_TARGET_RANDOM,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	175,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_STOCKPILE
/*effect*/			EFFECT_STOCKPILE,
/*power*/			0,
/*type*/			TYPE_NORMAL,
/*accuracy*/		0,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			FLAG_SNATCH_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_SPITUP
/*effect*/			EFFECT_SPIT_UP,
/*power*/			100,
/*type*/			TYPE_NORMAL,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	180,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_SWALLOW
/*effect*/			EFFECT_SWALLOW,
/*power*/			0,
/*type*/			TYPE_NORMAL,
/*accuracy*/		0,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			FLAG_SNATCH_AFFECTED | FLAG_TRIAGE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_HEATWAVE
/*effect*/			EFFECT_BURN_HIT,
/*power*/			100,
/*type*/			TYPE_FIRE,
/*accuracy*/		90,
/*pp*/				10,
/*effectChance*/	10,
/*target*/			MOVE_TARGET_BOTH,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	180,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_HAIL
/*effect*/			EFFECT_HAIL,
/*power*/			0,
/*type*/			TYPE_ICE,
/*accuracy*/		0,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_TORMENT
/*effect*/			EFFECT_TORMENT,
/*power*/			0,
/*type*/			TYPE_DARK,
/*accuracy*/		100,
/*pp*/				15,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_FLATTER
/*effect*/			EFFECT_FLATTER,
/*power*/			0,
/*type*/			TYPE_DARK,
/*accuracy*/		100,
/*pp*/				15,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_WILLOWISP
/*effect*/			EFFECT_WILL_O_WISP,
/*power*/			0,
/*type*/			TYPE_FIRE,
/*accuracy*/		85,
/*pp*/				15,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_MEMENTO
/*effect*/			EFFECT_MEMENTO,
/*power*/			0,
/*type*/			TYPE_DARK,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	7
},

{				//MOVE_FACADE
/*effect*/			EFFECT_FACADE,
/*power*/			70,
/*type*/			TYPE_NORMAL,
/*accuracy*/		100,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	140,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_FOCUSPUNCH
/*effect*/			EFFECT_FOCUS_PUNCH,
/*power*/			150,
/*type*/			TYPE_FIGHTING,
/*accuracy*/		100,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		253,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED,
/*z_move_power*/	200,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_SMELLINGSALTS
/*effect*/			EFFECT_SMELLINGSALT,
/*power*/			70,
/*type*/			TYPE_NORMAL,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	140,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_FOLLOWME
/*effect*/			EFFECT_FOLLOW_ME,
/*power*/			0,
/*type*/			TYPE_NORMAL,
/*accuracy*/		100,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		2,
/*flags*/			0,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_NATUREPOWER
/*effect*/			EFFECT_NATURE_POWER,
/*power*/			0,
/*type*/			TYPE_NORMAL,
/*accuracy*/		0,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_DEPENDS,
/*priority*/		0,
/*flags*/			0,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_CHARGE
/*effect*/			EFFECT_CHARGE,
/*power*/			0,
/*type*/			TYPE_ELECTRIC,
/*accuracy*/		100,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			FLAG_SNATCH_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_TAUNT
/*effect*/			EFFECT_TAUNT,
/*power*/			0,
/*type*/			TYPE_DARK,
/*accuracy*/		100,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_HELPINGHAND
/*effect*/			EFFECT_HELPING_HAND,
/*power*/			0,
/*type*/			TYPE_NORMAL,
/*accuracy*/		100,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		5,
/*flags*/			0,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_TRICK
/*effect*/			EFFECT_TRICK,
/*power*/			0,
/*type*/			TYPE_PSYCHIC,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_ROLEPLAY
/*effect*/			EFFECT_ROLE_PLAY,
/*power*/			0,
/*type*/			TYPE_PSYCHIC,
/*accuracy*/		0,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			0,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_WISH
/*effect*/			EFFECT_WISH,
/*power*/			0,
/*type*/			TYPE_NORMAL,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_SNATCH_AFFECTED | FLAG_TRIAGE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_ASSIST
/*effect*/			EFFECT_ASSIST,
/*power*/			0,
/*type*/			TYPE_NORMAL,
/*accuracy*/		100,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_DEPENDS,
/*priority*/		0,
/*flags*/			0,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_INGRAIN
/*effect*/			EFFECT_INGRAIN,
/*power*/			0,
/*type*/			TYPE_GRASS,
/*accuracy*/		100,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			FLAG_SNATCH_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_SUPERPOWER
/*effect*/			EFFECT_SUPERPOWER,
/*power*/			120,
/*type*/			TYPE_FIGHTING,
/*accuracy*/		100,
/*pp*/				5,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	190,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_MAGICCOAT
/*effect*/			EFFECT_MAGIC_COAT,
/*power*/			0,
/*type*/			TYPE_PSYCHIC,
/*accuracy*/		100,
/*pp*/				15,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_DEPENDS,
/*priority*/		4,
/*flags*/			0,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_RECYCLE
/*effect*/			EFFECT_RECYCLE,
/*power*/			0,
/*type*/			TYPE_NORMAL,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			FLAG_SNATCH_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_REVENGE
/*effect*/			EFFECT_REVENGE,
/*power*/			60,
/*type*/			TYPE_FIGHTING,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		252,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	120,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_BRICKBREAK
/*effect*/			EFFECT_BRICK_BREAK,
/*power*/			75,
/*type*/			TYPE_FIGHTING,
/*accuracy*/		100,
/*pp*/				15,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	140,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_YAWN
/*effect*/			EFFECT_YAWN,
/*power*/			0,
/*type*/			TYPE_NORMAL,
/*accuracy*/		0,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_KNOCKOFF
/*effect*/			EFFECT_KNOCK_OFF,
/*power*/			65,
/*type*/			TYPE_DARK,
/*accuracy*/		100,
/*pp*/				20,
/*effectChance*/	100,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	120,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_ENDEAVOR
/*effect*/			EFFECT_ENDEAVOR,
/*power*/			1,
/*type*/			TYPE_NORMAL,
/*accuracy*/		100,
/*pp*/				5,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_ERUPTION
/*effect*/			EFFECT_ERUPTION,
/*power*/			150,
/*type*/			TYPE_FIRE,
/*accuracy*/		100,
/*pp*/				5,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_BOTH,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	200,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_SKILLSWAP
/*effect*/			EFFECT_SKILL_SWAP,
/*power*/			0,
/*type*/			TYPE_PSYCHIC,
/*accuracy*/		0,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_IMPRISON
/*effect*/			EFFECT_IMPRISON,
/*power*/			0,
/*type*/			TYPE_PSYCHIC,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_SNATCH_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_REFRESH
/*effect*/			EFFECT_REFRESH,
/*power*/			0,
/*type*/			TYPE_NORMAL,
/*accuracy*/		100,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			FLAG_SNATCH_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_GRUDGE
/*effect*/			EFFECT_GRUDGE,
/*power*/			0,
/*type*/			TYPE_GHOST,
/*accuracy*/		100,
/*pp*/				5,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_SNATCH
/*effect*/			EFFECT_SNATCH,
/*power*/			0,
/*type*/			TYPE_DARK,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_DEPENDS,
/*priority*/		4,
/*flags*/			0,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_SECRETPOWER
/*effect*/			EFFECT_SECRET_POWER,
/*power*/			70,
/*type*/			TYPE_NORMAL,
/*accuracy*/		100,
/*pp*/				20,
/*effectChance*/	30,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	140,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_DIVE
/*effect*/			EFFECT_SEMI_INVULNERABLE,
/*power*/			80,
/*type*/			TYPE_WATER,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	160,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_ARMTHRUST
/*effect*/			EFFECT_MULTI_HIT,
/*power*/			15,
/*type*/			TYPE_FIGHTING,
/*accuracy*/		100,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_CAMOUFLAGE
/*effect*/			240,
/*power*/			0,
/*type*/			TYPE_NORMAL,
/*accuracy*/		100,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			FLAG_SNATCH_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_TAILGLOW
/*effect*/			EFFECT_SPECIAL_ATTACK_UP_2,
/*power*/			0,
/*type*/			TYPE_BUG,
/*accuracy*/		0,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			FLAG_SNATCH_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_LUSTERPURGE
/*effect*/			EFFECT_SPECIAL_DEFENSE_DOWN_HIT,
/*power*/			80,
/*type*/			TYPE_PSYCHIC,
/*accuracy*/		100,
/*pp*/				5,
/*effectChance*/	50,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	160,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_MISTBALL
/*effect*/			EFFECT_SPECIAL_ATTACK_DOWN_HIT,
/*power*/			70,
/*type*/			TYPE_PSYCHIC,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	50,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	140,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_FEATHERDANCE
/*effect*/			EFFECT_ATTACK_DOWN_2,
/*power*/			0,
/*type*/			TYPE_FLYING,
/*accuracy*/		100,
/*pp*/				15,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_TEETERDANCE
/*effect*/			EFFECT_TEETER_DANCE,
/*power*/			0,
/*type*/			TYPE_NORMAL,
/*accuracy*/		100,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_FOES_AND_ALLY,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_BLAZEKICK
/*effect*/			EFFECT_BLAZE_KICK,
/*power*/			85,
/*type*/			TYPE_FIRE,
/*accuracy*/		95,
/*pp*/				10,
/*effectChance*/	10,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	160,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_MUDSPORT
/*effect*/			EFFECT_MUD_SPORT,
/*power*/			0,
/*type*/			TYPE_GROUND,
/*accuracy*/		100,
/*pp*/				15,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			0,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_ICEBALL
/*effect*/			EFFECT_ROLLOUT,
/*power*/			30,
/*type*/			TYPE_ICE,
/*accuracy*/		90,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_NEEDLEARM
/*effect*/			EFFECT_FLINCH_HIT,
/*power*/			60,
/*type*/			TYPE_GRASS,
/*accuracy*/		100,
/*pp*/				15,
/*effectChance*/	30,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	120,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_SLACKOFF
/*effect*/			EFFECT_RESTORE_HP,
/*power*/			0,
/*type*/			TYPE_NORMAL,
/*accuracy*/		0,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			FLAG_SNATCH_AFFECTED | FLAG_TRIAGE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_HYPERVOICE
/*effect*/			EFFECT_HIT,
/*power*/			90,
/*type*/			TYPE_NORMAL,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_BOTH,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	175,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_POISONFANG
/*effect*/			EFFECT_POISON_FANG,
/*power*/			50,
/*type*/			TYPE_POISON,
/*accuracy*/		100,
/*pp*/				15,
/*effectChance*/	50,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_CRUSHCLAW
/*effect*/			EFFECT_DEFENSE_DOWN_HIT,
/*power*/			75,
/*type*/			TYPE_NORMAL,
/*accuracy*/		95,
/*pp*/				10,
/*effectChance*/	50,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	140,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_BLASTBURN
/*effect*/			EFFECT_RECHARGE,
/*power*/			150,
/*type*/			TYPE_FIRE,
/*accuracy*/		90,
/*pp*/				5,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	200,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_HYDROCANNON
/*effect*/			EFFECT_RECHARGE,
/*power*/			150,
/*type*/			TYPE_WATER,
/*accuracy*/		90,
/*pp*/				5,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	200,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_METEORMASH
/*effect*/			EFFECT_ATTACK_UP_HIT,
/*power*/			100,
/*type*/			TYPE_STEEL,
/*accuracy*/		90,
/*pp*/				10,
/*effectChance*/	20,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	180,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_ASTONISH
/*effect*/			EFFECT_FLINCH_HIT,
/*power*/			30,
/*type*/			TYPE_GHOST,
/*accuracy*/		100,
/*pp*/				15,
/*effectChance*/	30,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_WEATHERBALL
/*effect*/			EFFECT_WEATHER_BALL,
/*power*/			50,
/*type*/			TYPE_NORMAL,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	160,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_AROMATHERAPY
/*effect*/			EFFECT_HEAL_BELL,
/*power*/			0,
/*type*/			TYPE_GRASS,
/*accuracy*/		0,
/*pp*/				5,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			FLAG_SNATCH_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_FAKETEARS
/*effect*/			EFFECT_SPECIAL_DEFENSE_DOWN_2,
/*power*/			0,
/*type*/			TYPE_DARK,
/*accuracy*/		100,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_AIRCUTTER
/*effect*/			EFFECT_HIGH_CRITICAL,
/*power*/			60,
/*type*/			TYPE_FLYING,
/*accuracy*/		95,
/*pp*/				25,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_BOTH,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	120,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_OVERHEAT
/*effect*/			EFFECT_OVERHEAT,
/*power*/			140,
/*type*/			TYPE_FIRE,
/*accuracy*/		90,
/*pp*/				5,
/*effectChance*/	100,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	200,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_ODORSLEUTH
/*effect*/			EFFECT_FORESIGHT,
/*power*/			0,
/*type*/			TYPE_NORMAL,
/*accuracy*/		0,
/*pp*/				40,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_ROCKTOMB
/*effect*/			EFFECT_SPEED_DOWN_HIT,
/*power*/			60,
/*type*/			TYPE_ROCK,
/*accuracy*/		95,
/*pp*/				15,
/*effectChance*/	100,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	120,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_SILVERWIND
/*effect*/			EFFECT_ALL_STATS_UP_HIT,
/*power*/			60,
/*type*/			TYPE_BUG,
/*accuracy*/		100,
/*pp*/				5,
/*effectChance*/	10,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	120,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_METALSOUND
/*effect*/			EFFECT_SPECIAL_DEFENSE_DOWN_2,
/*power*/			0,
/*type*/			TYPE_STEEL,
/*accuracy*/		85,
/*pp*/				40,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_GRASSWHISTLE
/*effect*/			EFFECT_SLEEP,
/*power*/			0,
/*type*/			TYPE_GRASS,
/*accuracy*/		55,
/*pp*/				15,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_TICKLE
/*effect*/			EFFECT_TICKLE,
/*power*/			0,
/*type*/			TYPE_NORMAL,
/*accuracy*/		100,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_COSMICPOWER
/*effect*/			EFFECT_COSMIC_POWER,
/*power*/			0,
/*type*/			TYPE_PSYCHIC,
/*accuracy*/		0,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			FLAG_SNATCH_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_WATERSPOUT
/*effect*/			EFFECT_ERUPTION,
/*power*/			150,
/*type*/			TYPE_WATER,
/*accuracy*/		100,
/*pp*/				5,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_BOTH,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	200,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_SIGNALBEAM
/*effect*/			EFFECT_CONFUSE_HIT,
/*power*/			75,
/*type*/			TYPE_BUG,
/*accuracy*/		100,
/*pp*/				15,
/*effectChance*/	10,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	140,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_SHADOWPUNCH
/*effect*/			EFFECT_ALWAYS_HIT,
/*power*/			75,
/*type*/			TYPE_GHOST,
/*accuracy*/		0,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	140,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_EXTRASENSORY
/*effect*/			EFFECT_FLINCH_HIT,
/*power*/			80,
/*type*/			TYPE_PSYCHIC,
/*accuracy*/		100,
/*pp*/				20,
/*effectChance*/	10,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	160,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_SKYUPPERCUT
/*effect*/			EFFECT_SKY_UPPERCUT,
/*power*/			85,
/*type*/			TYPE_FIGHTING,
/*accuracy*/		90,
/*pp*/				15,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	160,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_SANDTOMB
/*effect*/			EFFECT_TRAP,
/*power*/			35,
/*type*/			TYPE_GROUND,
/*accuracy*/		85,
/*pp*/				15,
/*effectChance*/	100,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_SHEERCOLD
/*effect*/			EFFECT_0HKO,
/*power*/			1,
/*type*/			TYPE_ICE,
/*accuracy*/		30,
/*pp*/				5,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_MUDDYWATER
/*effect*/			EFFECT_ACCURACY_DOWN_HIT,
/*power*/			95,
/*type*/			TYPE_WATER,
/*accuracy*/		85,
/*pp*/				10,
/*effectChance*/	30,
/*target*/			MOVE_TARGET_BOTH,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	175,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_BULLETSEED
/*effect*/			EFFECT_MULTI_HIT,
/*power*/			25,
/*type*/			TYPE_GRASS,
/*accuracy*/		100,
/*pp*/				30,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_AERIALACE
/*effect*/			EFFECT_ALWAYS_HIT,
/*power*/			60,
/*type*/			TYPE_FLYING,
/*accuracy*/		0,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	120,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_ICICLESPEAR
/*effect*/			EFFECT_MULTI_HIT,
/*power*/			25,
/*type*/			TYPE_ICE,
/*accuracy*/		100,
/*pp*/				30,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_IRONDEFENSE
/*effect*/			EFFECT_DEFENSE_UP_2,
/*power*/			0,
/*type*/			TYPE_STEEL,
/*accuracy*/		0,
/*pp*/				15,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			FLAG_SNATCH_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_BLOCK
/*effect*/			EFFECT_MEAN_LOOK,
/*power*/			0,
/*type*/			TYPE_NORMAL,
/*accuracy*/		0,
/*pp*/				5,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_HOWL
/*effect*/			EFFECT_ATTACK_UP,
/*power*/			0,
/*type*/			TYPE_NORMAL,
/*accuracy*/		0,
/*pp*/				40,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			FLAG_SNATCH_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_DRAGONCLAW
/*effect*/			EFFECT_HIT,
/*power*/			80,
/*type*/			TYPE_DRAGON,
/*accuracy*/		100,
/*pp*/				15,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	160,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_FRENZYPLANT
/*effect*/			EFFECT_RECHARGE,
/*power*/			150,
/*type*/			TYPE_GRASS,
/*accuracy*/		90,
/*pp*/				5,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	200,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_BULKUP
/*effect*/			EFFECT_BULK_UP,
/*power*/			0,
/*type*/			TYPE_FIGHTING,
/*accuracy*/		0,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			FLAG_SNATCH_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_BOUNCE
/*effect*/			EFFECT_SEMI_INVULNERABLE,
/*power*/			85,
/*type*/			TYPE_FLYING,
/*accuracy*/		85,
/*pp*/				5,
/*effectChance*/	30,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	160,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_MUDSHOT
/*effect*/			EFFECT_SPEED_DOWN_HIT,
/*power*/			55,
/*type*/			TYPE_GROUND,
/*accuracy*/		95,
/*pp*/				15,
/*effectChance*/	100,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_POISONTAIL
/*effect*/			EFFECT_POISON_TAIL,
/*power*/			50,
/*type*/			TYPE_POISON,
/*accuracy*/		100,
/*pp*/				25,
/*effectChance*/	10,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_COVET
/*effect*/			EFFECT_THIEF,
/*power*/			60,
/*type*/			TYPE_NORMAL,
/*accuracy*/		100,
/*pp*/				40,
/*effectChance*/	100,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	120,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_VOLTTACKLE
/*effect*/			EFFECT_DOUBLE_EDGE,
/*power*/			120,
/*type*/			TYPE_ELECTRIC,
/*accuracy*/		100,
/*pp*/				15,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	190,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_MAGICALLEAF
/*effect*/			EFFECT_ALWAYS_HIT,
/*power*/			60,
/*type*/			TYPE_GRASS,
/*accuracy*/		0,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	120,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_WATERSPORT
/*effect*/			EFFECT_WATER_SPORT,
/*power*/			0,
/*type*/			TYPE_WATER,
/*accuracy*/		100,
/*pp*/				15,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			0,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_CALMMIND
/*effect*/			EFFECT_CALM_MIND,
/*power*/			0,
/*type*/			TYPE_PSYCHIC,
/*accuracy*/		0,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			FLAG_SNATCH_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_LEAFBLADE
/*effect*/			EFFECT_HIGH_CRITICAL,
/*power*/			90,
/*type*/			TYPE_GRASS,
/*accuracy*/		100,
/*pp*/				15,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	175,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_DRAGONDANCE
/*effect*/			EFFECT_DRAGON_DANCE,
/*power*/			0,
/*type*/			TYPE_DRAGON,
/*accuracy*/		0,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			FLAG_SNATCH_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_ROCKBLAST
/*effect*/			EFFECT_MULTI_HIT,
/*power*/			25,
/*type*/			TYPE_ROCK,
/*accuracy*/		80,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_SHOCKWAVE
/*effect*/			EFFECT_ALWAYS_HIT,
/*power*/			60,
/*type*/			TYPE_ELECTRIC,
/*accuracy*/		0,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	120,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_WATERPULSE
/*effect*/			EFFECT_CONFUSE_HIT,
/*power*/			60,
/*type*/			TYPE_WATER,
/*accuracy*/		100,
/*pp*/				20,
/*effectChance*/	20,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	120,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_DOOMDESIRE
/*effect*/			EFFECT_FUTURE_SIGHT,
/*power*/			140,
/*type*/			TYPE_STEEL,
/*accuracy*/		100,
/*pp*/				5,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			0,
/*z_move_power*/	200,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_PSYCHOBOOST
/*effect*/			EFFECT_OVERHEAT,
/*power*/			140,
/*type*/			TYPE_PSYCHIC,
/*accuracy*/		90,
/*pp*/				5,
/*effectChance*/	100,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	200,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_LEECHFANG
/*effect*/			EFFECT_ABSORB,
/*power*/			80,
/*type*/			TYPE_BUG,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_TRIAGE_AFFECTED,
/*z_move_power*/	160,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_AIRSLASH
/*effect*/			EFFECT_FLINCH_HIT,
/*power*/			75,
/*type*/			TYPE_FLYING,
/*accuracy*/		95,
/*pp*/				15,
/*effectChance*/	30,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	140,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_AQUAJET
/*effect*/			EFFECT_QUICK_ATTACK,
/*power*/			40,
/*type*/			TYPE_WATER,
/*accuracy*/		100,
/*pp*/				30,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		1,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_AQUATAIL
/*effect*/			EFFECT_HIT,
/*power*/			90,
/*type*/			TYPE_WATER,
/*accuracy*/		90,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	175,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_AURASPHERE
/*effect*/			EFFECT_ALWAYS_HIT,
/*power*/			90,
/*type*/			TYPE_FIGHTING,
/*accuracy*/		0,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	175,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_AVALANCHE
/*effect*/			EFFECT_REVENGE,
/*power*/			60,
/*type*/			TYPE_ICE,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		252,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	120,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_BRAVEBIRD
/*effect*/			EFFECT_DOUBLE_EDGE,
/*power*/			120,
/*type*/			TYPE_FLYING,
/*accuracy*/		100,
/*pp*/				15,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	190,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_BUGBUZZ
/*effect*/			EFFECT_SPECIAL_DEFENSE_DOWN_HIT,
/*power*/			90,
/*type*/			TYPE_BUG,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	10,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	175,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_BULLETPUNCH
/*effect*/			EFFECT_QUICK_ATTACK,
/*power*/			40,
/*type*/			TYPE_STEEL,
/*accuracy*/		100,
/*pp*/				30,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		1,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_CHARGEBEAM
/*effect*/			EFFECT_SPECIAL_DEFENSE_DOWN_HIT,
/*power*/			50,
/*type*/			TYPE_ELECTRIC,
/*accuracy*/		90,
/*pp*/				10,
/*effectChance*/	70,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_CLOSECOMBAT
/*effect*/			EFFECT_SUPERPOWER,
/*power*/			120,
/*type*/			TYPE_FIGHTING,
/*accuracy*/		100,
/*pp*/				5,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	190,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_CROSSPOISON
/*effect*/			EFFECT_POISON_TAIL,
/*power*/			70,
/*type*/			TYPE_POISON,
/*accuracy*/		100,
/*pp*/				20,
/*effectChance*/	10,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	140,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_DARKPULSE
/*effect*/			EFFECT_FLINCH_HIT,
/*power*/			80,
/*type*/			TYPE_DARK,
/*accuracy*/		100,
/*pp*/				15,
/*effectChance*/	20,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	160,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_DISCHARGE
/*effect*/			EFFECT_PARALYZE_HIT,
/*power*/			80,
/*type*/			TYPE_ELECTRIC,
/*accuracy*/		100,
/*pp*/				15,
/*effectChance*/	30,
/*target*/			MOVE_TARGET_FOES_AND_ALLY,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	160,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_DRACOMETEOR
/*effect*/			EFFECT_OVERHEAT,
/*power*/			140,
/*type*/			TYPE_DRAGON,
/*accuracy*/		90,
/*pp*/				5,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	200,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_DRAGONPULSE
/*effect*/			EFFECT_HIT,
/*power*/			90,
/*type*/			TYPE_DRAGON,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	175,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_DRAINPUNCH
/*effect*/			EFFECT_ABSORB,
/*power*/			75,
/*type*/			TYPE_FIGHTING,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED | FLAG_TRIAGE_AFFECTED,
/*z_move_power*/	140,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_EARTHPOWER
/*effect*/			EFFECT_SPECIAL_DEFENSE_DOWN_HIT,
/*power*/			90,
/*type*/			TYPE_GROUND,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	10,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	175,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_ENERGYBALL
/*effect*/			EFFECT_SPECIAL_DEFENSE_DOWN_HIT,
/*power*/			90,
/*type*/			TYPE_GRASS,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	10,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	175,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_FOCUSBLAST
/*effect*/			EFFECT_SPECIAL_DEFENSE_DOWN_HIT,
/*power*/			120,
/*type*/			TYPE_FIGHTING,
/*accuracy*/		70,
/*pp*/				5,
/*effectChance*/	10,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	190,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_FORCEPALM
/*effect*/			EFFECT_PARALYZE_HIT,
/*power*/			60,
/*type*/			TYPE_FIGHTING,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	30,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	120,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_GIGAIMPACT
/*effect*/			EFFECT_RECHARGE,
/*power*/			150,
/*type*/			TYPE_NORMAL,
/*accuracy*/		90,
/*pp*/				5,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	200,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_GRASSKNOT
/*effect*/			EFFECT_LOW_KICK,
/*power*/			1,
/*type*/			TYPE_GRASS,
/*accuracy*/		100,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_GUNKSHOT
/*effect*/			EFFECT_POISON_HIT,
/*power*/			120,
/*type*/			TYPE_POISON,
/*accuracy*/		80,
/*pp*/				5,
/*effectChance*/	30,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	190,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_HAMMERARM
/*effect*/			EFFECT_SUPERPOWER,
/*power*/			100,
/*type*/			TYPE_FIGHTING,
/*accuracy*/		90,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	180,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_HEADSMASH
/*effect*/			EFFECT_DOUBLE_EDGE,
/*power*/			150,
/*type*/			TYPE_ROCK,
/*accuracy*/		80,
/*pp*/				5,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	200,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_ICESHARD
/*effect*/			EFFECT_QUICK_ATTACK,
/*power*/			40,
/*type*/			TYPE_ICE,
/*accuracy*/		100,
/*pp*/				30,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		1,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_IRONHEAD
/*effect*/			EFFECT_FLINCH_HIT,
/*power*/			80,
/*type*/			TYPE_STEEL,
/*accuracy*/		100,
/*pp*/				15,
/*effectChance*/	30,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	160,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_LAVAPLUME
/*effect*/			EFFECT_BURN_HIT,
/*power*/			80,
/*type*/			TYPE_FIRE,
/*accuracy*/		100,
/*pp*/				15,
/*effectChance*/	30,
/*target*/			MOVE_TARGET_FOES_AND_ALLY,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	160,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_MAGNETBOMB
/*effect*/			EFFECT_ALWAYS_HIT,
/*power*/			60,
/*type*/			TYPE_STEEL,
/*accuracy*/		0,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	120,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_MUDBOMB
/*effect*/			EFFECT_ACCURACY_DOWN_HIT,
/*power*/			65,
/*type*/			TYPE_GROUND,
/*accuracy*/		85,
/*pp*/				10,
/*effectChance*/	30,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	120,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_NASTYPLOT
/*effect*/			EFFECT_SPECIAL_ATTACK_UP_2,
/*power*/			0,
/*type*/			TYPE_DARK,
/*accuracy*/		0,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			FLAG_SNATCH_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_NIGHTSLASH
/*effect*/			EFFECT_HIGH_CRITICAL,
/*power*/			70,
/*type*/			TYPE_DARK,
/*accuracy*/		100,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	140,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_OMINOUSWIND
/*effect*/			EFFECT_ALL_STATS_UP_HIT,
/*power*/			60,
/*type*/			TYPE_GHOST,
/*accuracy*/		100,
/*pp*/				5,
/*effectChance*/	10,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	120,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_POWERGEM
/*effect*/			EFFECT_HIT,
/*power*/			80,
/*type*/			TYPE_ROCK,
/*accuracy*/		100,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	160,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_POWERWHIP
/*effect*/			EFFECT_HIT,
/*power*/			120,
/*type*/			TYPE_GRASS,
/*accuracy*/		85,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	190,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_PSYCHOCUT
/*effect*/			EFFECT_HIGH_CRITICAL,
/*power*/			70,
/*type*/			TYPE_PSYCHIC,
/*accuracy*/		100,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	140,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_ROCKCLIMB
/*effect*/			EFFECT_CONFUSE_HIT,
/*power*/			90,
/*type*/			TYPE_NORMAL,
/*accuracy*/		85,
/*pp*/				20,
/*effectChance*/	20,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	175,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_ROCKPOLISH
/*effect*/			EFFECT_SPEED_UP_2,
/*power*/			0,
/*type*/			TYPE_ROCK,
/*accuracy*/		0,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			FLAG_SNATCH_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_ROCKWRECKER
/*effect*/			EFFECT_RECHARGE,
/*power*/			150,
/*type*/			TYPE_ROCK,
/*accuracy*/		90,
/*pp*/				5,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	200,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_ROOST
/*effect*/			EFFECT_RESTORE_HP,
/*power*/			0,
/*type*/			TYPE_FLYING,
/*accuracy*/		0,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			FLAG_SNATCH_AFFECTED | FLAG_TRIAGE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_SEEDBOMB
/*effect*/			EFFECT_HIT,
/*power*/			80,
/*type*/			TYPE_GRASS,
/*accuracy*/		100,
/*pp*/				15,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	160,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_SHADOWCLAW
/*effect*/			EFFECT_HIGH_CRITICAL,
/*power*/			70,
/*type*/			TYPE_GHOST,
/*accuracy*/		100,
/*pp*/				15,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	140,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_SHADOWSNEAK
/*effect*/			EFFECT_QUICK_ATTACK,
/*power*/			40,
/*type*/			TYPE_GHOST,
/*accuracy*/		100,
/*pp*/				30,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		1,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_VACUUMWAVE
/*effect*/			EFFECT_QUICK_ATTACK,
/*power*/			40,
/*type*/			TYPE_FIGHTING,
/*accuracy*/		100,
/*pp*/				30,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		1,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_XSCISSOR
/*effect*/			EFFECT_HIT,
/*power*/			80,
/*type*/			TYPE_BUG,
/*accuracy*/		100,
/*pp*/				15,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	160,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_ZENHEADBUTT
/*effect*/			EFFECT_FLINCH_HIT,
/*power*/			80,
/*type*/			TYPE_PSYCHIC,
/*accuracy*/		100,
/*pp*/				15,
/*effectChance*/	20,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	160,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_SWITCHEROO
/*effect*/			EFFECT_TRICK,
/*power*/			0,
/*type*/			TYPE_DARK,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_DRILLRUN
/*effect*/			EFFECT_HIGH_CRITICAL,
/*power*/			80,
/*type*/			TYPE_GROUND,
/*accuracy*/		95,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	160,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_BULLDOZE
/*effect*/			EFFECT_SPEED_DOWN_HIT,
/*power*/			60,
/*type*/			TYPE_GROUND,
/*accuracy*/		100,
/*pp*/				20,
/*effectChance*/	100,
/*target*/			MOVE_TARGET_FOES_AND_ALLY,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	120,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_ELECTROWEB
/*effect*/			EFFECT_SPEED_DOWN_HIT,
/*power*/			55,
/*type*/			TYPE_ELECTRIC,
/*accuracy*/		95,
/*pp*/				15,
/*effectChance*/	100,
/*target*/			MOVE_TARGET_BOTH,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_FLAMECHARGE
/*effect*/			EFFECT_SPEED_DOWN_HIT,
/*power*/			50,
/*type*/			TYPE_FIRE,
/*accuracy*/		100,
/*pp*/				20,
/*effectChance*/	100,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_FROSTBREATH
/*effect*/			EFFECT_ALWAYS_CRIT,
/*power*/			60,
/*type*/			TYPE_ICE,
/*accuracy*/		90,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	120,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_HEARTSTAMP
/*effect*/			EFFECT_FLINCH_HIT,
/*power*/			60,
/*type*/			TYPE_PSYCHIC,
/*accuracy*/		100,
/*pp*/				25,
/*effectChance*/	30,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	120,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_HONECLAWS
/*effect*/			EFFECT_ATTACK_UP,
/*power*/			0,
/*type*/			TYPE_DARK,
/*accuracy*/		0,
/*pp*/				15,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			FLAG_SNATCH_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_HORNLEECH
/*effect*/			EFFECT_ABSORB,
/*power*/			75,
/*type*/			TYPE_GRASS,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED | FLAG_TRIAGE_AFFECTED,
/*z_move_power*/	140,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_COIL
/*effect*/			EFFECT_BULK_UP,
/*power*/			0,
/*type*/			TYPE_POISON,
/*accuracy*/		0,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			FLAG_SNATCH_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_HURRICANE
/*effect*/			EFFECT_THUNDER,
/*power*/			120,
/*type*/			TYPE_FLYING,
/*accuracy*/		70,
/*pp*/				10,
/*effectChance*/	30,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	190,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_ICICLECRASH
/*effect*/			EFFECT_FLINCH_HIT,
/*power*/			85,
/*type*/			TYPE_ICE,
/*accuracy*/		90,
/*pp*/				10,
/*effectChance*/	30,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	160,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_WORKUP
/*effect*/			EFFECT_SPECIAL_ATTACK_UP,
/*power*/			0,
/*type*/			TYPE_NORMAL,
/*accuracy*/		0,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			FLAG_SNATCH_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_QUIVERDANCE
/*effect*/			EFFECT_CALM_MIND,
/*power*/			0,
/*type*/			TYPE_BUG,
/*accuracy*/		0,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			FLAG_SNATCH_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_LEAFTORNADO
/*effect*/			EFFECT_ACCURACY_DOWN_HIT,
/*power*/			65,
/*type*/			TYPE_GRASS,
/*accuracy*/		90,
/*pp*/				10,
/*effectChance*/	30,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	120,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_LOWSWEEP
/*effect*/			EFFECT_SPEED_DOWN_HIT,
/*power*/			60,
/*type*/			TYPE_FIGHTING,
/*accuracy*/		100,
/*pp*/				20,
/*effectChance*/	100,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	120,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_SNARL
/*effect*/			EFFECT_SPECIAL_ATTACK_DOWN_HIT,
/*power*/			55,
/*type*/			TYPE_DARK,
/*accuracy*/		95,
/*pp*/				10,
/*effectChance*/	100,
/*target*/			MOVE_TARGET_BOTH,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_STRUGGLEBUG
/*effect*/			EFFECT_SPECIAL_ATTACK_DOWN_HIT,
/*power*/			50,
/*type*/			TYPE_BUG,
/*accuracy*/		100,
/*pp*/				20,
/*effectChance*/	100,
/*target*/			MOVE_TARGET_BOTH,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_STEAMROLLER
/*effect*/			EFFECT_FLINCH_HIT_2,
/*power*/			65,
/*type*/			TYPE_BUG,
/*accuracy*/		100,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	120,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_STORMTHROW
/*effect*/			EFFECT_ALWAYS_CRIT,
/*power*/			60,
/*type*/			TYPE_FIGHTING,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	120,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_VENOSHOCK
/*effect*/			EFFECT_SMELLINGSALT,
/*power*/			65,
/*type*/			TYPE_POISON,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	120,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_WILDCHARGE
/*effect*/			EFFECT_RECOIL,
/*power*/			90,
/*type*/			TYPE_ELECTRIC,
/*accuracy*/		100,
/*pp*/				15,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	175,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_POISONJAB
/*effect*/			EFFECT_POISON_HIT,
/*power*/			80,
/*type*/			TYPE_POISON,
/*accuracy*/		100,
/*pp*/				20,
/*effectChance*/	30,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	160,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_ACIDSPRAY
/*effect*/			EFFECT_SPECIAL_DEFENSE_DOWN_HIT,
/*power*/			40,
/*type*/			TYPE_POISON,
/*accuracy*/		100,
/*pp*/				20,
/*effectChance*/	100,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_FIERYDANCE
/*effect*/			EFFECT_SPECIAL_DEFENSE_DOWN_HIT,
/*power*/			80,
/*type*/			TYPE_FIRE,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	50,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	160,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_CLEARSMOG
/*effect*/			222,
/*power*/			50,
/*type*/			TYPE_POISON,
/*accuracy*/		0,
/*pp*/				15,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_LEAFSTORM
/*effect*/			EFFECT_OVERHEAT,
/*power*/			140,
/*type*/			TYPE_GRASS,
/*accuracy*/		90,
/*pp*/				5,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	200,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_STONEEDGE
/*effect*/			EFFECT_HIGH_CRITICAL,
/*power*/			100,
/*type*/			TYPE_ROCK,
/*accuracy*/		80,
/*pp*/				5,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	180,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_PAYBACK
/*effect*/			EFFECT_REVENGE,
/*power*/			50,
/*type*/			TYPE_DARK,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_FIREFANG
/*effect*/			EFFECT_FLINCH_HIT,
/*power*/			65,
/*type*/			TYPE_FIRE,
/*accuracy*/		95,
/*pp*/				15,
/*effectChance*/	10,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	120,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_ICEFANG
/*effect*/			EFFECT_FLINCH_HIT,
/*power*/			65,
/*type*/			TYPE_ICE,
/*accuracy*/		95,
/*pp*/				15,
/*effectChance*/	10,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	120,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_THUNDERFANG
/*effect*/			EFFECT_FLINCH_HIT,
/*power*/			65,
/*type*/			TYPE_ELECTRIC,
/*accuracy*/		95,
/*pp*/				15,
/*effectChance*/	10,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	120,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_FLAREBLITZ
/*effect*/			EFFECT_THAW_HIT,
/*power*/			120,
/*type*/			TYPE_FIRE,
/*accuracy*/		100,
/*pp*/				15,
/*effectChance*/	10,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	190,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_INFERNO
/*effect*/			EFFECT_BURN_HIT,
/*power*/			100,
/*type*/			TYPE_FIRE,
/*accuracy*/		50,
/*pp*/				5,
/*effectChance*/	100,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	180,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_HEX
/*effect*/			EFFECT_SMELLINGSALT,
/*power*/			65,
/*type*/			TYPE_GHOST,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	160,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_DUALCHOP
/*effect*/			EFFECT_DOUBLE_HIT,
/*power*/			40,
/*type*/			TYPE_DRAGON,
/*accuracy*/		90,
/*pp*/				15,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_DOUBLEHIT
/*effect*/			EFFECT_DOUBLE_HIT,
/*power*/			35,
/*type*/			TYPE_NORMAL,
/*accuracy*/		90,
/*pp*/				15,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_COTTONGUARD
/*effect*/			EFFECT_DEFENSE_UP_2,
/*power*/			0,
/*type*/			TYPE_GRASS,
/*accuracy*/		0,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			FLAG_SNATCH_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_ACROBATICS
/*effect*/			230,
/*power*/			55,
/*type*/			TYPE_FLYING,
/*accuracy*/		100,
/*pp*/				15,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_FLAMEBURST
/*effect*/			241,
/*power*/			70,
/*type*/			TYPE_FIRE,
/*accuracy*/		100,
/*pp*/				15,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	140,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_UTURN
/*effect*/			EFFECT_BATON_PASS,
/*power*/			70,
/*type*/			TYPE_BUG,
/*accuracy*/		100,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	140,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_VOLTSWITCH
/*effect*/			EFFECT_BATON_PASS,
/*power*/			70,
/*type*/			TYPE_ELECTRIC,
/*accuracy*/		100,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	140,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_DRAGONTAIL
/*effect*/			EFFECT_ROAR,
/*power*/			60,
/*type*/			TYPE_DRAGON,
/*accuracy*/		90,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		250,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	120,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_CIRCLETHROW
/*effect*/			EFFECT_ROAR,
/*power*/			60,
/*type*/			TYPE_FIGHTING,
/*accuracy*/		90,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		250,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	120,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_MIRRORSHOT
/*effect*/			EFFECT_ACCURACY_DOWN_HIT,
/*power*/			65,
/*type*/			TYPE_STEEL,
/*accuracy*/		85,
/*pp*/				10,
/*effectChance*/	30,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	120,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_WOODHAMMER
/*effect*/			EFFECT_DOUBLE_EDGE,
/*power*/			120,
/*type*/			TYPE_GRASS,
/*accuracy*/		100,
/*pp*/				15,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	190,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_HEALPULSE
/*effect*/			233,
/*power*/			0,
/*type*/			TYPE_PSYCHIC,
/*accuracy*/		0,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_TRIAGE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_FLASHCANNON
/*effect*/			EFFECT_SPECIAL_DEFENSE_DOWN_HIT,
/*power*/			80,
/*type*/			TYPE_STEEL,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	10,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	160,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_BUGBITE
/*effect*/			219,
/*power*/			60,
/*type*/			TYPE_BUG,
/*accuracy*/		100,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	120,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_PLUCK
/*effect*/			219,
/*power*/			60,
/*type*/			TYPE_FLYING,
/*accuracy*/		100,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	120,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_SHELLSMASH
/*effect*/			EFFECT_DRAGON_DANCE,
/*power*/			0,
/*type*/			TYPE_NORMAL,
/*accuracy*/		0,
/*pp*/				15,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			FLAG_SNATCH_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_RAGEPOWDER
/*effect*/			EFFECT_FOLLOW_ME,
/*power*/			0,
/*type*/			TYPE_BUG,
/*accuracy*/		100,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		2,
/*flags*/			0,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_WAKEUPSLAP
/*effect*/			EFFECT_SMELLINGSALT,
/*power*/			70,
/*type*/			TYPE_FIGHTING,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	100,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	140,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_RAZORSHELL
/*effect*/			EFFECT_DEFENSE_DOWN_HIT,
/*power*/			75,
/*type*/			TYPE_WATER,
/*accuracy*/		95,
/*pp*/				10,
/*effectChance*/	50,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	140,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_COPYCAT
/*effect*/			EFFECT_MIRROR_MOVE,
/*power*/			0,
/*type*/			TYPE_NORMAL,
/*accuracy*/		0,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_DEPENDS,
/*priority*/		0,
/*flags*/			0,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_MIRACLEEYE
/*effect*/			EFFECT_FORESIGHT,
/*power*/			0,
/*type*/			TYPE_PSYCHIC,
/*accuracy*/		0,
/*pp*/				40,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_SUCKERPUNCH
/*effect*/			237,
/*power*/			80,
/*type*/			TYPE_DARK,
/*accuracy*/		100,
/*pp*/				5,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		1,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	160,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_SCALD
/*effect*/			EFFECT_THAW_HIT,
/*power*/			80,
/*type*/			TYPE_WATER,
/*accuracy*/		100,
/*pp*/				15,
/*effectChance*/	30,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	160,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_BRINE
/*effect*/			238,
/*power*/			65,
/*type*/			TYPE_WATER,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	100,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	120,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_FAIRYWIND
/*effect*/			EFFECT_HIT,
/*power*/			40,
/*type*/			TYPE_FAIRY,
/*accuracy*/		100,
/*pp*/				15,
/*effectChance*/	30,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_BABYDOLLEYES
/*effect*/			EFFECT_ATTACK_DOWN,
/*power*/			0,
/*type*/			TYPE_FAIRY,
/*accuracy*/		100,
/*pp*/				30,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		1,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_PLAYROUGH
/*effect*/			EFFECT_ATTACK_DOWN_HIT,
/*power*/			90,
/*type*/			TYPE_FAIRY,
/*accuracy*/		90,
/*pp*/				10,
/*effectChance*/	10,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	175,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_MOONBLAST
/*effect*/			EFFECT_SPECIAL_ATTACK_DOWN_HIT,
/*power*/			95,
/*type*/			TYPE_FAIRY,
/*accuracy*/		100,
/*pp*/				40,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	175,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_DRAININGKISS
/*effect*/			EFFECT_ABSORB,
/*power*/			50,
/*type*/			TYPE_FAIRY,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED | FLAG_TRIAGE_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_DAZZLINGGLEAM
/*effect*/			EFFECT_HIT,
/*power*/			80,
/*type*/			TYPE_FAIRY,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_BOTH,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	160,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_DISARMINGVOICE
/*effect*/			EFFECT_ALWAYS_HIT,
/*power*/			40,
/*type*/			TYPE_FAIRY,
/*accuracy*/		0,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_BOTH,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_STEALTHROCK
/*effect*/			EFFECT_SPIKES,
/*power*/			0,
/*type*/			TYPE_ROCK,
/*accuracy*/		0,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_OPPONENTS_FIELD,
/*priority*/		0,
/*flags*/			FLAG_MAGIC_COAT_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_TOXICSPIKES
/*effect*/			EFFECT_SPIKES,
/*power*/			0,
/*type*/			TYPE_POISON,
/*accuracy*/		0,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_OPPONENTS_FIELD,
/*priority*/		0,
/*flags*/			FLAG_MAGIC_COAT_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_HEALINGWISH
/*effect*/			EFFECT_MEMENTO,
/*power*/			0,
/*type*/			TYPE_PSYCHIC,
/*accuracy*/		0,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			FLAG_SNATCH_AFFECTED | FLAG_TRIAGE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_LUNARDANCE
/*effect*/			EFFECT_MEMENTO,
/*power*/			0,
/*type*/			TYPE_PSYCHIC,
/*accuracy*/		0,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			FLAG_SNATCH_AFFECTED | FLAG_TRIAGE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_TAILWIND
/*effect*/			239,
/*power*/			0,
/*type*/			TYPE_FLYING,
/*accuracy*/		0,
/*pp*/				15,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			FLAG_SNATCH_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_BOOMBURST
/*effect*/			EFFECT_UNUSED_6E,
/*power*/			140,
/*type*/			TYPE_NORMAL,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_FOES_AND_ALLY,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	200,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_INCINERATE
/*effect*/			219,
/*power*/			60,
/*type*/			TYPE_FIRE,
/*accuracy*/		100,
/*pp*/				15,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_BOTH,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	120,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_WORRYSEED
/*effect*/			EFFECT_SKILL_SWAP,
/*power*/			0,
/*type*/			TYPE_GRASS,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_GASTROACID
/*effect*/			EFFECT_SKILL_SWAP,
/*power*/			0,
/*type*/			TYPE_POISON,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_GEOMANCY
/*effect*/			EFFECT_CALM_MIND,
/*power*/			0,
/*type*/			TYPE_FAIRY,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_FLOWERSHIELD
/*effect*/			EFFECT_DEFENSE_UP,
/*power*/			0,
/*type*/			TYPE_FAIRY,
/*accuracy*/		0,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_FOES_AND_ALLY,
/*priority*/		0,
/*flags*/			0,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_ROTOTILLER
/*effect*/			EFFECT_SPECIAL_ATTACK_UP,
/*power*/			0,
/*type*/			TYPE_GROUND,
/*accuracy*/		0,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_FOES_AND_ALLY,
/*priority*/		0,
/*flags*/			0,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_SHIFTGEAR
/*effect*/			EFFECT_DRAGON_DANCE,
/*power*/			0,
/*type*/			TYPE_STEEL,
/*accuracy*/		0,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			FLAG_SNATCH_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_STICKYWEB
/*effect*/			EFFECT_SPIKES,
/*power*/			0,
/*type*/			TYPE_BUG,
/*accuracy*/		0,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_OPPONENTS_FIELD,
/*priority*/		0,
/*flags*/			FLAG_MAGIC_COAT_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_STOREDPOWER
/*effect*/			214,
/*power*/			20,
/*type*/			TYPE_PSYCHIC,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_ACUPRESSURE
/*effect*/			EFFECT_EVASION_UP,
/*power*/			0,
/*type*/			TYPE_NORMAL,
/*accuracy*/		0,
/*pp*/				30,
/*effectChance*/	0,
/*target*/			2,
/*priority*/		0,
/*flags*/			FLAG_SNATCH_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_PUNISHMENT
/*effect*/			214,
/*power*/			1,
/*type*/			TYPE_DARK,
/*accuracy*/		100,
/*pp*/				5,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_ASSURANCE
/*effect*/			EFFECT_PURSUIT,
/*power*/			60,
/*type*/			TYPE_DARK,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	120,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_DRAGONRUSH
/*effect*/			EFFECT_FLINCH_HIT_2,
/*power*/			100,
/*type*/			TYPE_DRAGON,
/*accuracy*/		80,
/*pp*/				10,
/*effectChance*/	30,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	180,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_DARKVOID
/*effect*/			EFFECT_SLEEP,
/*power*/			0,
/*type*/			TYPE_DARK,
/*accuracy*/		80,
/*pp*/				10,
/*effectChance*/	100,
/*target*/			MOVE_TARGET_BOTH,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_FEINT
/*effect*/			230,
/*power*/			50,
/*type*/			TYPE_NORMAL,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		2,
/*flags*/			FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_PHANTOMFORCE
/*effect*/			EFFECT_SEMI_INVULNERABLE,
/*power*/			90,
/*type*/			TYPE_GHOST,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	175,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_PETALBLIZZARD
/*effect*/			EFFECT_UNUSED_6E,
/*power*/			90,
/*type*/			TYPE_GRASS,
/*accuracy*/		100,
/*pp*/				15,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_FOES_AND_ALLY,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	175,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_PLAYNICE
/*effect*/			EFFECT_ATTACK_DOWN,
/*power*/			0,
/*type*/			TYPE_NORMAL,
/*accuracy*/		0,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAGIC_COAT_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_ROAROFTIME
/*effect*/			EFFECT_RECHARGE,
/*power*/			150,
/*type*/			TYPE_DRAGON,
/*accuracy*/		90,
/*pp*/				5,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	200,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_SPACIALREND
/*effect*/			EFFECT_HIGH_CRITICAL,
/*power*/			100,
/*type*/			TYPE_DRAGON,
/*accuracy*/		95,
/*pp*/				5,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	180,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_MAGMASTORM
/*effect*/			EFFECT_TRAP,
/*power*/			100,
/*type*/			TYPE_FIRE,
/*accuracy*/		100,
/*pp*/				5,
/*effectChance*/	100,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	180,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_FINALGAMBIT
/*effect*/			EFFECT_MEMENTO,
/*power*/			1,
/*type*/			TYPE_FIGHTING,
/*accuracy*/		100,
/*pp*/				5,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_SHADOWFORCE
/*effect*/			EFFECT_SEMI_INVULNERABLE,
/*power*/			120,
/*type*/			TYPE_GHOST,
/*accuracy*/		100,
/*pp*/				5,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	190,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_NUZZLE
/*effect*/			EFFECT_PARALYZE_HIT,
/*power*/			20,
/*type*/			TYPE_ELECTRIC,
/*accuracy*/		100,
/*pp*/				20,
/*effectChance*/	100,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_RETALIATE
/*effect*/			215,
/*power*/			70,
/*type*/			TYPE_NORMAL,
/*accuracy*/		100,
/*pp*/				5,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	140,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_STEELYHIT
/*effect*/			EFFECT_HIT,
/*power*/			40,
/*type*/			TYPE_STEEL,
/*accuracy*/		100,
/*pp*/				35,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_JUDGMENT
/*effect*/			215,
/*power*/			100,
/*type*/			TYPE_NORMAL,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	180,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_FREEZEDRY
/*effect*/			EFFECT_FREEZE_HIT,
/*power*/			70,
/*type*/			TYPE_ICE,
/*accuracy*/		100,
/*pp*/				20,
/*effectChance*/	10,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	140,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_PSYSHOCK
/*effect*/			EFFECT_SPATK_PHYSICAL_DAMAGE,
/*power*/			80,
/*type*/			TYPE_PSYCHIC,
/*accuracy*/		0,
/*pp*/				10,
/*effectChance*/	100,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	160,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_ROUND
/*effect*/			EFFECT_FURY_CUTTER,
/*power*/			60,
/*type*/			TYPE_NORMAL,
/*accuracy*/		100,
/*pp*/				15,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	120,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_TAILSLAP
/*effect*/			EFFECT_MULTI_HIT,
/*power*/			25,
/*type*/			TYPE_NORMAL,
/*accuracy*/		85,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_GEARGRIND
/*effect*/			EFFECT_DOUBLE_HIT,
/*power*/			50,
/*type*/			TYPE_STEEL,
/*accuracy*/		100,
/*pp*/				15,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	180,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_NATURALGIFT
/*effect*/			220,
/*power*/			1,
/*type*/			TYPE_NORMAL,
/*accuracy*/		100,
/*pp*/				15,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_CHIPAWAY
/*effect*/			EFFECT_IGNORE_STAT_CHANGES,
/*power*/			70,
/*type*/			TYPE_NORMAL,
/*accuracy*/		100,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	140,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_SMACKDOWN
/*effect*/			221,
/*power*/			50,
/*type*/			TYPE_ROCK,
/*accuracy*/		100,
/*pp*/				15,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_HYPERSPACEHOLE
/*effect*/			EFFECT_SUPERPOWER,
/*power*/			80,
/*type*/			TYPE_PSYCHIC,
/*accuracy*/		0,
/*pp*/				5,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	160,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_GYROBALL
/*effect*/			225,
/*power*/			1,
/*type*/			TYPE_STEEL,
/*accuracy*/		100,
/*pp*/				5,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_HYPERSPACEFURY
/*effect*/			EFFECT_SUPERPOWER,
/*power*/			100,
/*type*/			TYPE_DARK,
/*accuracy*/		0,
/*pp*/				5,
/*effectChance*/	100,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	180,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_ATTACKORDER
/*effect*/			EFFECT_HIGH_CRITICAL,
/*power*/			90,
/*type*/			TYPE_BUG,
/*accuracy*/		100,
/*pp*/				15,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	175,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_DEFENDORDER
/*effect*/			EFFECT_COSMIC_POWER,
/*power*/			0,
/*type*/			TYPE_BUG,
/*accuracy*/		0,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			FLAG_SNATCH_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_HEALORDER
/*effect*/			EFFECT_RESTORE_HP,
/*power*/			0,
/*type*/			TYPE_BUG,
/*accuracy*/		0,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			FLAG_SNATCH_AFFECTED | FLAG_TRIAGE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_CAPTIVATE
/*effect*/			EFFECT_SPECIAL_ATTACK_DOWN_2,
/*power*/			0,
/*type*/			TYPE_NORMAL,
/*accuracy*/		100,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_BOTH,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_DEFOG
/*effect*/			EFFECT_RAPID_SPIN,
/*power*/			0,
/*type*/			TYPE_FLYING,
/*accuracy*/		0,
/*pp*/				15,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_ORIGINPULSE
/*effect*/			EFFECT_HIT,
/*power*/			120,
/*type*/			TYPE_WATER,
/*accuracy*/		85,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_BOTH,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	190,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_PRECIPICEBLADES
/*effect*/			EFFECT_HIT,
/*power*/			120,
/*type*/			TYPE_GROUND,
/*accuracy*/		85,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_BOTH,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	190,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_GUARDSWAP
/*effect*/			EFFECT_CAMOUFLAGE,
/*power*/			0,
/*type*/			TYPE_PSYCHIC,
/*accuracy*/		0,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_STEAMERUPTION
/*effect*/			EFFECT_THAW_HIT,
/*power*/			120,
/*type*/			TYPE_WATER,
/*accuracy*/		95,
/*pp*/				5,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	190,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_METALBURST
/*effect*/			EFFECT_COUNTER,
/*power*/			1,
/*type*/			TYPE_STEEL,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_MEFIRST
/*effect*/			220,
/*power*/			0,
/*type*/			TYPE_NORMAL,
/*accuracy*/		0,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_POWERSWAP
/*effect*/			EFFECT_CAMOUFLAGE,
/*power*/			0,
/*type*/			TYPE_PSYCHIC,
/*accuracy*/		0,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_POWERTRICK
/*effect*/			EFFECT_CAMOUFLAGE,
/*power*/			0,
/*type*/			TYPE_PSYCHIC,
/*accuracy*/		0,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			FLAG_SNATCH_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_PSYCHOSHIFT
/*effect*/			EFFECT_REFRESH,
/*power*/			0,
/*type*/			TYPE_PSYCHIC,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_WRINGOUT
/*effect*/			EFFECT_ERUPTION,
/*power*/			1,
/*type*/			TYPE_NORMAL,
/*accuracy*/		100,
/*pp*/				5,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_KINGSSHIELD
/*effect*/			EFFECT_PROTECT,
/*power*/			0,
/*type*/			TYPE_STEEL,
/*accuracy*/		0,
/*pp*/				15,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		4,
/*flags*/			0,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_PSYSTRIKE
/*effect*/			EFFECT_SPATK_PHYSICAL_DAMAGE,
/*power*/			100,
/*type*/			TYPE_PSYCHIC,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	180,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_AUTOTOMIZE
/*effect*/			EFFECT_SPEED_UP_2,
/*power*/			0,
/*type*/			TYPE_STEEL,
/*accuracy*/		0,
/*pp*/				15,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			FLAG_SNATCH_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_DIAMONDSTORM
/*effect*/			EFFECT_DEFENSE_UP_HIT,
/*power*/			100,
/*type*/			TYPE_ROCK,
/*accuracy*/		95,
/*pp*/				5,
/*effectChance*/	50,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	180,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_SLUDGEWAVE
/*effect*/			EFFECT_POISON_HIT,
/*power*/			95,
/*type*/			TYPE_POISON,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	10,
/*target*/			MOVE_TARGET_FOES_AND_ALLY,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	175,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_HEAVYSLAM
/*effect*/			EFFECT_FLINCH_HIT_2,
/*power*/			1,
/*type*/			TYPE_STEEL,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_SYNCHRONOISE
/*effect*/			254,
/*power*/			120,
/*type*/			TYPE_PSYCHIC,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_FOES_AND_ALLY,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	190,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_ELECTROBALL
/*effect*/			225,
/*power*/			1,
/*type*/			TYPE_ELECTRIC,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_FOULPLAY
/*effect*/			224,
/*power*/			95,
/*type*/			TYPE_DARK,
/*accuracy*/		100,
/*pp*/				15,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	175,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_ENTRAINMENT
/*effect*/			EFFECT_SKILL_SWAP,
/*power*/			0,
/*type*/			TYPE_NORMAL,
/*accuracy*/		100,
/*pp*/				15,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_NIGHTDAZE
/*effect*/			EFFECT_ACCURACY_DOWN_HIT,
/*power*/			85,
/*type*/			TYPE_DARK,
/*accuracy*/		95,
/*pp*/				10,
/*effectChance*/	40,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	160,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_ECHOEDVOICE
/*effect*/			EFFECT_FURY_CUTTER,
/*power*/			40,
/*type*/			TYPE_NORMAL,
/*accuracy*/		100,
/*pp*/				15,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_LANDSWRATH
/*effect*/			EFFECT_HIT,
/*power*/			90,
/*type*/			TYPE_GROUND,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_BOTH,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	175,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_OBLIVIONWING
/*effect*/			EFFECT_ABSORB,
/*power*/			80,
/*type*/			TYPE_FLYING,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED | FLAG_TRIAGE_AFFECTED,
/*z_move_power*/	160,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_HEARTSWAP
/*effect*/			EFFECT_CAMOUFLAGE,
/*power*/			0,
/*type*/			TYPE_PSYCHIC,
/*accuracy*/		0,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_CRUSHGRIP
/*effect*/			EFFECT_ERUPTION,
/*power*/			1,
/*type*/			TYPE_NORMAL,
/*accuracy*/		100,
/*pp*/				5,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_SACREDSWORD
/*effect*/			EFFECT_IGNORE_STAT_CHANGES,
/*power*/			90,
/*type*/			TYPE_FIGHTING,
/*accuracy*/		100,
/*pp*/				15,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	175,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_HEATCRASH
/*effect*/			EFFECT_FLINCH_HIT_2,
/*power*/			80,
/*type*/			TYPE_FIRE,
/*accuracy*/		100,
/*pp*/				35,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	160,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_HEADCHARGE
/*effect*/			EFFECT_RECOIL,
/*power*/			120,
/*type*/			TYPE_NORMAL,
/*accuracy*/		100,
/*pp*/				15,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	190,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_TECHNOBLAST
/*effect*/			215,
/*power*/			120,
/*type*/			TYPE_NORMAL,
/*accuracy*/		100,
/*pp*/				5,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	190,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_RELICSONG
/*effect*/			223,
/*power*/			75,
/*type*/			TYPE_NORMAL,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	10,
/*target*/			MOVE_TARGET_BOTH,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	140,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_SECRETSWORD
/*effect*/			EFFECT_SPATK_PHYSICAL_DAMAGE,
/*power*/			85,
/*type*/			TYPE_FIGHTING,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	160,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_GLACIATE
/*effect*/			EFFECT_SPEED_DOWN_HIT,
/*power*/			65,
/*type*/			TYPE_ICE,
/*accuracy*/		95,
/*pp*/				10,
/*effectChance*/	100,
/*target*/			MOVE_TARGET_BOTH,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	120,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_BOLTSTRIKE
/*effect*/			EFFECT_PARALYZE_HIT,
/*power*/			130,
/*type*/			TYPE_ELECTRIC,
/*accuracy*/		85,
/*pp*/				5,
/*effectChance*/	20,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	195,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_BLUEFLARE
/*effect*/			EFFECT_BURN_HIT,
/*power*/			130,
/*type*/			TYPE_FIRE,
/*accuracy*/		85,
/*pp*/				5,
/*effectChance*/	20,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	195,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_FREEZESHOCK
/*effect*/			EFFECT_SKY_ATTACK,
/*power*/			140,
/*type*/			TYPE_ICE,
/*accuracy*/		90,
/*pp*/				5,
/*effectChance*/	30,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	200,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_ICEBURN
/*effect*/			EFFECT_SKY_ATTACK,
/*power*/			140,
/*type*/			TYPE_ICE,
/*accuracy*/		90,
/*pp*/				5,
/*effectChance*/	30,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	200,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_VCREATE
/*effect*/			EFFECT_SUPERPOWER,
/*power*/			180,
/*type*/			TYPE_FIRE,
/*accuracy*/		95,
/*pp*/				5,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	220,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_FUSIONFLARE
/*effect*/			EFFECT_THAW_HIT,
/*power*/			100,
/*type*/			TYPE_FIRE,
/*accuracy*/		100,
/*pp*/				5,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	180,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_FUSIONBOLT
/*effect*/			223,
/*power*/			100,
/*type*/			TYPE_ELECTRIC,
/*accuracy*/		100,
/*pp*/				5,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	180,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_FELLSTINGER
/*effect*/			EFFECT_ATTACK_UP_HIT,
/*power*/			50,
/*type*/			TYPE_BUG,
/*accuracy*/		100,
/*pp*/				25,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_NOBLEROAR
/*effect*/			EFFECT_ATTACK_DOWN,
/*power*/			0,
/*type*/			TYPE_NORMAL,
/*accuracy*/		0,
/*pp*/				30,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_DRAGONASCENT
/*effect*/			EFFECT_SUPERPOWER,
/*power*/			120,
/*type*/			TYPE_FLYING,
/*accuracy*/		100,
/*pp*/				5,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	190,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_PARTINGSHOT
/*effect*/			EFFECT_BATON_PASS,
/*power*/			0,
/*type*/			TYPE_DARK,
/*accuracy*/		100,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_TOPSYTURVY
/*effect*/			234,
/*power*/			0,
/*type*/			TYPE_DARK,
/*accuracy*/		0,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_COREENFORCER
/*effect*/			EFFECT_SKILL_SWAP,
/*power*/			100,
/*type*/			TYPE_DRAGON,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_BOTH,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	140,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_CHATTER
/*effect*/			EFFECT_CONFUSE_HIT,
/*power*/			65,
/*type*/			TYPE_FLYING,
/*accuracy*/		100,
/*pp*/				20,
/*effectChance*/	100,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	120,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_CONFIDE
/*effect*/			EFFECT_SPECIAL_ATTACK_DOWN,
/*power*/			0,
/*type*/			TYPE_NORMAL,
/*accuracy*/		0,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			0,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_WATERSHURIKEN
/*effect*/			EFFECT_MULTI_HIT,
/*power*/			15,
/*type*/			TYPE_WATER,
/*accuracy*/		100,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		1,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_MYSTICALFIRE
/*effect*/			EFFECT_SPECIAL_ATTACK_DOWN_HIT,
/*power*/			75,
/*type*/			TYPE_FIRE,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	100,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	140,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_SPIKYSHIELD
/*effect*/			EFFECT_PROTECT,
/*power*/			0,
/*type*/			TYPE_GRASS,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		4,
/*flags*/			0,
/*z_move_power*/	0,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_EERIEIMPULSE
/*effect*/			EFFECT_SPECIAL_ATTACK_DOWN_2,
/*power*/			0,
/*type*/			TYPE_ELECTRIC,
/*accuracy*/		100,
/*pp*/				15,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_HOLDBACK
/*effect*/			EFFECT_FALSE_SWIPE,
/*power*/			40,
/*type*/			TYPE_NORMAL,
/*accuracy*/		100,
/*pp*/				40,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_INFESTATION
/*effect*/			EFFECT_TRAP,
/*power*/			20,
/*type*/			TYPE_BUG,
/*accuracy*/		100,
/*pp*/				20,
/*effectChance*/	100,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_POWERUPPUNCH
/*effect*/			EFFECT_ATTACK_UP_HIT,
/*power*/			40,
/*type*/			TYPE_FIGHTING,
/*accuracy*/		100,
/*pp*/				20,
/*effectChance*/	100,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_THOUSANDARROWS
/*effect*/			221,
/*power*/			90,
/*type*/			TYPE_GROUND,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	175,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_THOUSANDWAVES
/*effect*/			EFFECT_MEAN_LOOK,
/*power*/			90,
/*type*/			TYPE_GROUND,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	175,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_PARABOLICCHARGE
/*effect*/			EFFECT_ABSORB,
/*power*/			65,
/*type*/			TYPE_ELECTRIC,
/*accuracy*/		100,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_FOES_AND_ALLY,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED | FLAG_TRIAGE_AFFECTED,
/*z_move_power*/	120,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_SEARINGSHOT
/*effect*/			EFFECT_BURN_HIT,
/*power*/			100,
/*type*/			TYPE_FIRE,
/*accuracy*/		100,
/*pp*/				5,
/*effectChance*/	30,
/*target*/			MOVE_TARGET_FOES_AND_ALLY,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	180,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_SEEDFLARE
/*effect*/			EFFECT_SPECIAL_DEFENSE_DOWN_HIT,
/*power*/			120,
/*type*/			TYPE_GRASS,
/*accuracy*/		85,
/*pp*/				5,
/*effectChance*/	40,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	190,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_ACCELEROCK
/*effect*/			EFFECT_QUICK_ATTACK,
/*power*/			40,
/*type*/			TYPE_ROCK,
/*accuracy*/		100,
/*pp*/				35,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		1,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_ANCHORSHOT
/*effect*/			EFFECT_MEAN_LOOK,
/*power*/			80,
/*type*/			TYPE_STEEL,
/*accuracy*/		100,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	160,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_AURORAVEIL
/*effect*/			EFFECT_REFLECT,
/*power*/			0,
/*type*/			TYPE_ICE,
/*accuracy*/		0,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			FLAG_SNATCH_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_BANEFULBUNKER
/*effect*/			EFFECT_PROTECT,
/*power*/			0,
/*type*/			TYPE_POISON,
/*accuracy*/		0,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		4,
/*flags*/			0,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_BEAKBLAST
/*effect*/			EFFECT_FOCUS_PUNCH,
/*power*/			100,
/*type*/			TYPE_FLYING,
/*accuracy*/		100,
/*pp*/				15,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		253,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	180,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_BRUTALSWING
/*effect*/			EFFECT_UNUSED_6E,
/*power*/			60,
/*type*/			TYPE_DARK,
/*accuracy*/		100,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_FOES_AND_ALLY,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	120,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_BURNUP
/*effect*/			EFFECT_THAW_HIT,
/*power*/			140,
/*type*/			TYPE_FIRE,
/*accuracy*/		100,
/*pp*/				5,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	200,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_CLANGINGSCALES
/*effect*/			EFFECT_SUPERPOWER,
/*power*/			120,
/*type*/			TYPE_DRAGON,
/*accuracy*/		100,
/*pp*/				5,
/*effectChance*/	100,
/*target*/			MOVE_TARGET_BOTH,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	190,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_DARKESTLARIAT
/*effect*/			EFFECT_IGNORE_STAT_CHANGES,
/*power*/			85,
/*type*/			TYPE_DARK,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	160,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_DRAGONHAMMER
/*effect*/			EFFECT_HIT,
/*power*/			90,
/*type*/			TYPE_DRAGON,
/*accuracy*/		100,
/*pp*/				15,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	175,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_FIRELASH
/*effect*/			EFFECT_DEFENSE_DOWN_HIT,
/*power*/			80,
/*type*/			TYPE_FIRE,
/*accuracy*/		100,
/*pp*/				15,
/*effectChance*/	100,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	160,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_FIRSTIMPRESSION
/*effect*/			EFFECT_FAKE_OUT,
/*power*/			95,
/*type*/			TYPE_BUG,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		2,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	175,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_FLEURCANNON
/*effect*/			EFFECT_OVERHEAT,
/*power*/			140,
/*type*/			TYPE_FAIRY,
/*accuracy*/		90,
/*pp*/				5,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	200,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_FLORALHEALING
/*effect*/			233,
/*power*/			0,
/*type*/			TYPE_FAIRY,
/*accuracy*/		0,
/*pp*/				10,
/*effectChance*/	100,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_TRIAGE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_GEARUP
/*effect*/			EFFECT_SPECIAL_ATTACK_UP,
/*power*/			0,
/*type*/			TYPE_STEEL,
/*accuracy*/		0,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_FOES_AND_ALLY,
/*priority*/		0,
/*flags*/			FLAG_SNATCH_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_HIGHHORSEPOWER
/*effect*/			EFFECT_HIT,
/*power*/			95,
/*type*/			TYPE_GROUND,
/*accuracy*/		95,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	175,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_ICEHAMMER
/*effect*/			EFFECT_SUPERPOWER,
/*power*/			100,
/*type*/			TYPE_ICE,
/*accuracy*/		90,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	180,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_LASERFOCUS
/*effect*/			EFFECT_LOCK_ON,
/*power*/			0,
/*type*/			TYPE_NORMAL,
/*accuracy*/		0,
/*pp*/				30,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_SNATCH_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_LEAFAGE
/*effect*/			EFFECT_HIT,
/*power*/			40,
/*type*/			TYPE_GRASS,
/*accuracy*/		100,
/*pp*/				40,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_LIQUIDATION
/*effect*/			EFFECT_DEFENSE_DOWN_HIT,
/*power*/			85,
/*type*/			TYPE_WATER,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	20,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	160,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_LUNGE
/*effect*/			EFFECT_ATTACK_DOWN_HIT,
/*power*/			80,
/*type*/			TYPE_BUG,
/*accuracy*/		100,
/*pp*/				15,
/*effectChance*/	100,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	160,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_MOONGEISTBEAM
/*effect*/			EFFECT_HIT,
/*power*/			100,
/*type*/			TYPE_GHOST,
/*accuracy*/		100,
/*pp*/				5,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	180,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_MULTIATTACK
/*effect*/			215,
/*power*/			90,
/*type*/			TYPE_NORMAL,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	175,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_NATURESMADNESS
/*effect*/			EFFECT_SUPER_FANG,
/*power*/			0,
/*type*/			TYPE_FAIRY,
/*accuracy*/		90,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_POLLENPUFF
/*effect*/			233,
/*power*/			90,
/*type*/			TYPE_BUG,
/*accuracy*/		100,
/*pp*/				15,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	175,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_POWERTRIP
/*effect*/			214,
/*power*/			20,
/*type*/			TYPE_DARK,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	100,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_PRISMATICLASER
/*effect*/			EFFECT_RECHARGE,
/*power*/			160,
/*type*/			TYPE_PSYCHIC,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	200,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_PSYCHICFANGS
/*effect*/			EFFECT_BRICK_BREAK,
/*power*/			85,
/*type*/			TYPE_PSYCHIC,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	160,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_PURIFY
/*effect*/			EFFECT_RESTORE_HP,
/*power*/			0,
/*type*/			TYPE_POISON,
/*accuracy*/		0,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_TRIAGE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_REVELATIONDANCE
/*effect*/			215,
/*power*/			90,
/*type*/			TYPE_NORMAL,
/*accuracy*/		100,
/*pp*/				15,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	175,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_SHADOWBONE
/*effect*/			EFFECT_DEFENSE_DOWN_HIT,
/*power*/			85,
/*type*/			TYPE_GHOST,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	20,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	160,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_SHELLTRAP
/*effect*/			EFFECT_FOCUS_PUNCH,
/*power*/			150,
/*type*/			TYPE_FIRE,
/*accuracy*/		100,
/*pp*/				5,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_BOTH,
/*priority*/		253,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	200,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_SHOREUP
/*effect*/			EFFECT_SYNTHESIS,
/*power*/			0,
/*type*/			TYPE_GROUND,
/*accuracy*/		0,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			FLAG_SNATCH_AFFECTED | FLAG_TRIAGE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_SMARTSTRIKE
/*effect*/			EFFECT_ALWAYS_HIT,
/*power*/			70,
/*type*/			TYPE_STEEL,
/*accuracy*/		0,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	140,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_SOLARBLADE
/*effect*/			EFFECT_SOLARBEAM,
/*power*/			125,
/*type*/			TYPE_GRASS,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	190,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_SPARKLINGARIA
/*effect*/			EFFECT_SMELLINGSALT,
/*power*/			90,
/*type*/			TYPE_WATER,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_FOES_AND_ALLY,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	175,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_SPECTRALTHIEF
/*effect*/			EFFECT_PSYCH_UP,
/*power*/			90,
/*type*/			TYPE_GHOST,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	175,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_SPEEDSWAP
/*effect*/			EFFECT_CAMOUFLAGE,
/*power*/			0,
/*type*/			TYPE_PSYCHIC,
/*accuracy*/		0,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_SPIRITSHACKLE
/*effect*/			EFFECT_MEAN_LOOK,
/*power*/			80,
/*type*/			TYPE_GHOST,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	160,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_SPOTLIGHT
/*effect*/			EFFECT_FOLLOW_ME,
/*power*/			0,
/*type*/			TYPE_NORMAL,
/*accuracy*/		0,
/*pp*/				15,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		3,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_STOMPINGTANTRUM
/*effect*/			224,
/*power*/			75,
/*type*/			TYPE_GROUND,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	140,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_STRENGTHSAP
/*effect*/			EFFECT_ABSORB,
/*power*/			0,
/*type*/			TYPE_GRASS,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_TRIAGE_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_SUNSTEELSTRIKE
/*effect*/			EFFECT_HIT,
/*power*/			100,
/*type*/			TYPE_STEEL,
/*accuracy*/		100,
/*pp*/				5,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	180,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_TEARFULLOOK
/*effect*/			EFFECT_ATTACK_DOWN,
/*power*/			0,
/*type*/			TYPE_NORMAL,
/*accuracy*/		0,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAGIC_COAT_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_THROATCHOP
/*effect*/			231,
/*power*/			80,
/*type*/			TYPE_DARK,
/*accuracy*/		100,
/*pp*/				15,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	160,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_TOXICTHREAD
/*effect*/			EFFECT_POISON,
/*power*/			0,
/*type*/			TYPE_POISON,
/*accuracy*/		100,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_TROPKICK
/*effect*/			EFFECT_ATTACK_DOWN_HIT,
/*power*/			70,
/*type*/			TYPE_GRASS,
/*accuracy*/		100,
/*pp*/				15,
/*effectChance*/	100,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	140,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_ZINGZAP
/*effect*/			EFFECT_FLINCH_HIT,
/*power*/			80,
/*type*/			TYPE_ELECTRIC,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	30,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED,
/*z_move_power*/	160,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_ELECTRICTERRAIN
/*effect*/			226,
/*power*/			0,
/*type*/			TYPE_ELECTRIC,
/*accuracy*/		0,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			0,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_GRASSYTERRAIN
/*effect*/			226,
/*power*/			0,
/*type*/			TYPE_GRASS,
/*accuracy*/		0,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			0,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_MISTYTERRAIN
/*effect*/			226,
/*power*/			0,
/*type*/			TYPE_FAIRY,
/*accuracy*/		0,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			0,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_PSYCHICTERRAIN
/*effect*/			226,
/*power*/			0,
/*type*/			TYPE_PSYCHIC,
/*accuracy*/		0,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			0,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_GRASSPLEDGE
/*effect*/			227,
/*power*/			80,
/*type*/			TYPE_GRASS,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	160,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_FIREPLEDGE
/*effect*/			227,
/*power*/			80,
/*type*/			TYPE_FIRE,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	160,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_WATERPLEDGE
/*effect*/			227,
/*power*/			80,
/*type*/			TYPE_WATER,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	160,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_TRICKROOM
/*effect*/			228,
/*power*/			0,
/*type*/			TYPE_PSYCHIC,
/*accuracy*/		0,
/*pp*/				5,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		249,
/*flags*/			0,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_MAGICROOM
/*effect*/			228,
/*power*/			0,
/*type*/			TYPE_PSYCHIC,
/*accuracy*/		0,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			0,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_WONDERROOM
/*effect*/			228,
/*power*/			0,
/*type*/			TYPE_PSYCHIC,
/*accuracy*/		0,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		249,
/*flags*/			0,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_CRAFTYSHIELD
/*effect*/			EFFECT_PROTECT,
/*power*/			0,
/*type*/			TYPE_FAIRY,
/*accuracy*/		0,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		3,
/*flags*/			0,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_MATBLOCK
/*effect*/			EFFECT_PROTECT,
/*power*/			0,
/*type*/			TYPE_FIGHTING,
/*accuracy*/		0,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			FLAG_SNATCH_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_QUICKGUARD
/*effect*/			EFFECT_PROTECT,
/*power*/			0,
/*type*/			TYPE_FIGHTING,
/*accuracy*/		0,
/*pp*/				15,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		3,
/*flags*/			FLAG_SNATCH_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_WIDEGUARD
/*effect*/			EFFECT_PROTECT,
/*power*/			0,
/*type*/			TYPE_FIGHTING,
/*accuracy*/		0,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		3,
/*flags*/			FLAG_SNATCH_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_AQUARING
/*effect*/			EFFECT_LEECH_SEED,
/*power*/			0,
/*type*/			TYPE_WATER,
/*accuracy*/		0,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			FLAG_SNATCH_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_AROMATICMIST
/*effect*/			EFFECT_DEFENSE_UP,
/*power*/			0,
/*type*/			TYPE_FAIRY,
/*accuracy*/		0,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			0,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_BELCH
/*effect*/			EFFECT_RECYCLE,
/*power*/			120,
/*type*/			TYPE_POISON,
/*accuracy*/		90,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED,
/*z_move_power*/	190,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_BESTOW
/*effect*/			EFFECT_TRICK,
/*power*/			0,
/*type*/			TYPE_NORMAL,
/*accuracy*/		0,
/*pp*/				15,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			0,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_FLING
/*effect*/			229,
/*power*/			0,
/*type*/			TYPE_DARK,
/*accuracy*/		100,
/*pp*/				10,
/*effectChance*/	100,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_FLYINGPRESS
/*effect*/			EFFECT_FLINCH_HIT_2,
/*power*/			100,
/*type*/			TYPE_FIGHTING,
/*accuracy*/		95,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	170,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_GRAVITY
/*effect*/			228,
/*power*/			0,
/*type*/			TYPE_PSYCHIC,
/*accuracy*/		0,
/*pp*/				5,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_FOES_AND_ALLY,
/*priority*/		0,
/*flags*/			0,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_GUARDSPLIT
/*effect*/			EFFECT_CAMOUFLAGE,
/*power*/			0,
/*type*/			TYPE_PSYCHIC,
/*accuracy*/		0,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_HEALBLOCK
/*effect*/			231,
/*power*/			0,
/*type*/			TYPE_PSYCHIC,
/*accuracy*/		100,
/*pp*/				15,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_BOTH,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_IONDELUGE
/*effect*/			228,
/*power*/			0,
/*type*/			TYPE_ELECTRIC,
/*accuracy*/		0,
/*pp*/				25,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		1,
/*flags*/			0,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_LUCKYCHANT
/*effect*/			239,
/*power*/			0,
/*type*/			TYPE_NORMAL,
/*accuracy*/		0,
/*pp*/				30,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			FLAG_SNATCH_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_MAGNETRISE
/*effect*/			239,
/*power*/			0,
/*type*/			TYPE_ELECTRIC,
/*accuracy*/		0,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			FLAG_SNATCH_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_MAGNETICFLUX
/*effect*/			EFFECT_DEFENSE_UP,
/*power*/			0,
/*type*/			TYPE_ELECTRIC,
/*accuracy*/		0,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_FOES_AND_ALLY,
/*priority*/		0,
/*flags*/			FLAG_SNATCH_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_POWDER
/*effect*/			231,
/*power*/			0,
/*type*/			TYPE_BUG,
/*accuracy*/		0,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		1,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_POWERSPLIT
/*effect*/			EFFECT_CAMOUFLAGE,
/*power*/			0,
/*type*/			TYPE_PSYCHIC,
/*accuracy*/		0,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_REFLECTTYPE
/*effect*/			EFFECT_CONVERSION,
/*power*/			0,
/*type*/			TYPE_NORMAL,
/*accuracy*/		0,
/*pp*/				15,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_SIMPLEBEAM
/*effect*/			EFFECT_SKILL_SWAP,
/*power*/			0,
/*type*/			TYPE_NORMAL,
/*accuracy*/		100,
/*pp*/				15,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_SOAK
/*effect*/			232,
/*power*/			0,
/*type*/			TYPE_WATER,
/*accuracy*/		100,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_TELEKINESIS
/*effect*/			231,
/*power*/			0,
/*type*/			TYPE_PSYCHIC,
/*accuracy*/		0,
/*pp*/				15,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_TRICKORTREAT
/*effect*/			232,
/*power*/			0,
/*type*/			TYPE_GHOST,
/*accuracy*/		100,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_VENOMDRENCH
/*effect*/			EFFECT_ATTACK_DOWN,
/*power*/			0,
/*type*/			TYPE_POISON,
/*accuracy*/		100,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_BOTH,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_FORESTSCURSE
/*effect*/			232,
/*power*/			0,
/*type*/			TYPE_GRASS,
/*accuracy*/		100,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_MINDBLOWN
/*effect*/			EFFECT_BELLY_DRUM,
/*power*/			150,
/*type*/			TYPE_FIRE,
/*accuracy*/		100,
/*pp*/				5,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_FOES_AND_ALLY,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	200,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_PHOTONGEYSER
/*effect*/			EFFECT_HIT,
/*power*/			100,
/*type*/			TYPE_PSYCHIC,
/*accuracy*/		100,
/*pp*/				5,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	180,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_PLASMAFISTS
/*effect*/			228,
/*power*/			100,
/*type*/			TYPE_ELECTRIC,
/*accuracy*/		100,
/*pp*/				15,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	180,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_ELECTRIFY
/*effect*/			234,
/*power*/			0,
/*type*/			TYPE_ELECTRIC,
/*accuracy*/		0,
/*pp*/				20,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_EMBARGO
/*effect*/			231,
/*power*/			0,
/*type*/			TYPE_DARK,
/*accuracy*/		100,
/*pp*/				15,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_MAGIC_COAT_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_FAIRYLOCK
/*effect*/			235,
/*power*/			0,
/*type*/			TYPE_FAIRY,
/*accuracy*/		0,
/*pp*/				10,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			0,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_LIGHTOFRUIN
/*effect*/			EFFECT_DOUBLE_EDGE,
/*power*/			140,
/*type*/			TYPE_FAIRY,
/*accuracy*/		90,
/*pp*/				5,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	200,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_TRUMPCARD
/*effect*/			220,
/*power*/			0,
/*type*/			TYPE_NORMAL,
/*accuracy*/		0,
/*pp*/				5,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_AFTERYOU
/*effect*/			236,
/*power*/			0,
/*type*/			TYPE_NORMAL,
/*accuracy*/		0,
/*pp*/				15,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			0,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_INSTRUCT
/*effect*/			236,
/*power*/			0,
/*type*/			TYPE_PSYCHIC,
/*accuracy*/		0,
/*pp*/				15,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_QUASH
/*effect*/			236,
/*power*/			0,
/*type*/			TYPE_DARK,
/*accuracy*/		100,
/*pp*/				15,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_PROTECT_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_HAPPYHOUR
/*effect*/			235,
/*power*/			0,
/*type*/			TYPE_NORMAL,
/*accuracy*/		0,
/*pp*/				30,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_USER,
/*priority*/		0,
/*flags*/			0,
/*z_move_power*/	0,
/*split*/			SPLIT_STATUS,
/*z_move_effect*/	0
},

{				//MOVE_DOUBLEIRONBASH
/*effect*/			EFFECT_FLINCH_HIT,
/*power*/			0,
/*type*/			TYPE_STEEL,
/*accuracy*/		100,
/*pp*/				0,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_MAKES_CONTACT | FLAG_PROTECT_AFFECTED | FLAG_MIRROR_MOVE_AFFECTED | FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_LASTRESORT
/*effect*/			0,
/*power*/			1,
/*type*/			0,
/*accuracy*/		0,
/*pp*/				1,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_SKYDROP
/*effect*/			0,
/*power*/			1,
/*type*/			0,
/*accuracy*/		0,
/*pp*/				1,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_CELEBRATE
/*effect*/			0,
/*power*/			1,
/*type*/			0,
/*accuracy*/		0,
/*pp*/				1,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_HOLDHANDS
/*effect*/			0,
/*power*/			1,
/*type*/			0,
/*accuracy*/		0,
/*pp*/				1,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_BREAKNECK_BLITZ_P
/*effect*/			0,
/*power*/			1,
/*type*/			TYPE_NORMAL,
/*accuracy*/		0,
/*pp*/				1,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_BREAKNECK_BLITZ_S
/*effect*/			0,
/*power*/			1,
/*type*/			TYPE_NORMAL,
/*accuracy*/		0,
/*pp*/				1,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_ALL_OUT_PUMMELING_P
/*effect*/			0,
/*power*/			1,
/*type*/			TYPE_FIGHTING,
/*accuracy*/		0,
/*pp*/				1,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_ALL_OUT_PUMMELING_S
/*effect*/			0,
/*power*/			1,
/*type*/			TYPE_FIGHTING,
/*accuracy*/		0,
/*pp*/				1,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_SUPERSONIC_SKYSTRIKE_P
/*effect*/			0,
/*power*/			1,
/*type*/			TYPE_FLYING,
/*accuracy*/		0,
/*pp*/				1,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_SUPERSONIC_SKYSTRIKE_S
/*effect*/			0,
/*power*/			1,
/*type*/			TYPE_FLYING,
/*accuracy*/		0,
/*pp*/				1,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_ACID_DOWNPOUR_P
/*effect*/			0,
/*power*/			1,
/*type*/			TYPE_POISON,
/*accuracy*/		0,
/*pp*/				1,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_ACID_DOWNPOUR_S
/*effect*/			0,
/*power*/			1,
/*type*/			TYPE_POISON,
/*accuracy*/		0,
/*pp*/				1,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_TECTONIC_RAGE_P
/*effect*/			0,
/*power*/			1,
/*type*/			TYPE_GROUND,
/*accuracy*/		0,
/*pp*/				1,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_TECTONIC_RAGE_S
/*effect*/			0,
/*power*/			1,
/*type*/			TYPE_GROUND,
/*accuracy*/		0,
/*pp*/				1,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_CONTINENTAL_CRUSH_P
/*effect*/			0,
/*power*/			1,
/*type*/			TYPE_ROCK,
/*accuracy*/		0,
/*pp*/				1,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_CONTINENTAL_CRUSH_S
/*effect*/			0,
/*power*/			1,
/*type*/			TYPE_ROCK,
/*accuracy*/		0,
/*pp*/				1,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_SAVAGE_SPIN_OUT_P
/*effect*/			0,
/*power*/			1,
/*type*/			TYPE_BUG,
/*accuracy*/		0,
/*pp*/				1,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_SAVAGE_SPIN_OUT_S
/*effect*/			0,
/*power*/			1,
/*type*/			TYPE_BUG,
/*accuracy*/		0,
/*pp*/				1,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_NEVER_ENDING_NIGHTMARE_P
/*effect*/			0,
/*power*/			1,
/*type*/			TYPE_GHOST,
/*accuracy*/		0,
/*pp*/				1,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_NEVER_ENDING_NIGHTMARE_S
/*effect*/			0,
/*power*/			1,
/*type*/			TYPE_GHOST,
/*accuracy*/		0,
/*pp*/				1,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_CORKSCREW_CRASH_P
/*effect*/			0,
/*power*/			1,
/*type*/			TYPE_STEEL,
/*accuracy*/		0,
/*pp*/				1,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_CORKSCREW_CRASH_S
/*effect*/			0,
/*power*/			1,
/*type*/			TYPE_STEEL,
/*accuracy*/		0,
/*pp*/				1,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_INFERNO_OVERDRIVE_P
/*effect*/			0,
/*power*/			1,
/*type*/			TYPE_FIRE,
/*accuracy*/		0,
/*pp*/				1,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_INFERNO_OVERDRIVE_S
/*effect*/			0,
/*power*/			1,
/*type*/			TYPE_FIRE,
/*accuracy*/		0,
/*pp*/				1,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_HYDRO_VORTEX_P
/*effect*/			0,
/*power*/			1,
/*type*/			TYPE_WATER,
/*accuracy*/		0,
/*pp*/				1,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_HYDRO_VORTEX_S
/*effect*/			0,
/*power*/			1,
/*type*/			TYPE_WATER,
/*accuracy*/		0,
/*pp*/				1,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_BLOOM_DOOM_P
/*effect*/			0,
/*power*/			1,
/*type*/			TYPE_GRASS,
/*accuracy*/		0,
/*pp*/				1,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_BLOOM_DOOM_S
/*effect*/			0,
/*power*/			1,
/*type*/			TYPE_GRASS,
/*accuracy*/		0,
/*pp*/				1,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_GIGAVOLT_HAVOC_P
/*effect*/			0,
/*power*/			1,
/*type*/			TYPE_ELECTRIC,
/*accuracy*/		0,
/*pp*/				1,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_GIGAVOLT_HAVOC_S
/*effect*/			0,
/*power*/			1,
/*type*/			TYPE_ELECTRIC,
/*accuracy*/		0,
/*pp*/				1,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_SHATTERED_PSYCHE_P
/*effect*/			0,
/*power*/			1,
/*type*/			TYPE_PSYCHIC,
/*accuracy*/		0,
/*pp*/				1,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_SHATTERED_PSYCHE_S
/*effect*/			0,
/*power*/			1,
/*type*/			TYPE_PSYCHIC,
/*accuracy*/		0,
/*pp*/				1,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_SUBZERO_SLAMMER_P
/*effect*/			0,
/*power*/			1,
/*type*/			TYPE_ICE,
/*accuracy*/		0,
/*pp*/				1,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_SUBZERO_SLAMMER_S
/*effect*/			0,
/*power*/			1,
/*type*/			TYPE_ICE,
/*accuracy*/		0,
/*pp*/				1,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_DEVASTATING_DRAKE_P
/*effect*/			0,
/*power*/			1,
/*type*/			TYPE_DRAGON,
/*accuracy*/		0,
/*pp*/				1,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_DEVASTATING_DRAKE_S
/*effect*/			0,
/*power*/			1,
/*type*/			TYPE_DRAGON,
/*accuracy*/		0,
/*pp*/				1,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_BLACK_HOLE_ECLIPSE_P
/*effect*/			0,
/*power*/			1,
/*type*/			TYPE_DARK,
/*accuracy*/		0,
/*pp*/				1,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_BLACK_HOLE_ECLIPSE_S
/*effect*/			0,
/*power*/			1,
/*type*/			TYPE_DARK,
/*accuracy*/		0,
/*pp*/				1,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_SPECIAL,
/*z_move_effect*/	0
},

{				//MOVE_TWINKLE_TACKLE_P
/*effect*/			0,
/*power*/			1,
/*type*/			0,
/*accuracy*/		0,
/*pp*/				1,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_TWINKLE_TACKLE_S
/*effect*/			0,
/*power*/			1,
/*type*/			0,
/*accuracy*/		0,
/*pp*/				1,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_CATASTROPIKA
/*effect*/			0,
/*power*/			1,
/*type*/			0,
/*accuracy*/		0,
/*pp*/				1,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_10000000_VOLT_THUNDERBOLT
/*effect*/			0,
/*power*/			1,
/*type*/			0,
/*accuracy*/		0,
/*pp*/				1,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_STOKED_SPARKSURFER
/*effect*/			0,
/*power*/			1,
/*type*/			0,
/*accuracy*/		0,
/*pp*/				1,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_EXTREME_EVOBOOST
/*effect*/			0,
/*power*/			1,
/*type*/			0,
/*accuracy*/		0,
/*pp*/				1,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_PULVERIZING_PANCAKE
/*effect*/			0,
/*power*/			1,
/*type*/			0,
/*accuracy*/		0,
/*pp*/				1,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_GENESIS_SUPERNOVA
/*effect*/			0,
/*power*/			1,
/*type*/			0,
/*accuracy*/		0,
/*pp*/				1,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_SINISTER_ARROW_RAID
/*effect*/			0,
/*power*/			1,
/*type*/			0,
/*accuracy*/		0,
/*pp*/				1,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_MALICIOUS_MOONSAULT
/*effect*/			0,
/*power*/			1,
/*type*/			0,
/*accuracy*/		0,
/*pp*/				1,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_OCEANIC_OPERETTA
/*effect*/			0,
/*power*/			1,
/*type*/			0,
/*accuracy*/		0,
/*pp*/				1,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_SPLINTERED_STORMSHARDS
/*effect*/			0,
/*power*/			1,
/*type*/			0,
/*accuracy*/		0,
/*pp*/				1,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_LETS_SNUGGLE_FOREVER
/*effect*/			0,
/*power*/			1,
/*type*/			0,
/*accuracy*/		0,
/*pp*/				1,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_CLANGOROUS_SOULBLAZE
/*effect*/			0,
/*power*/			1,
/*type*/			0,
/*accuracy*/		0,
/*pp*/				1,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_GUARDIAN_OF_ALOLA
/*effect*/			0,
/*power*/			1,
/*type*/			0,
/*accuracy*/		0,
/*pp*/				1,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_SEARING_SUNRAZE_SMASH
/*effect*/			0,
/*power*/			1,
/*type*/			0,
/*accuracy*/		0,
/*pp*/				1,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_MENACING_MOONRAZE_MAELSTROM
/*effect*/			0,
/*power*/			1,
/*type*/			0,
/*accuracy*/		0,
/*pp*/				1,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_LIGHT_THAT_BURNS_THE_SKY
/*effect*/			0,
/*power*/			1,
/*type*/			0,
/*accuracy*/		0,
/*pp*/				1,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
},

{				//MOVE_SOUL_STEALING_7_STAR_STRIKE
/*effect*/			0,
/*power*/			1,
/*type*/			0,
/*accuracy*/		0,
/*pp*/				1,
/*effectChance*/	0,
/*target*/			MOVE_TARGET_SELECTED,
/*priority*/		0,
/*flags*/			FLAG_KINGS_ROCK_AFFECTED,
/*z_move_power*/	0,
/*split*/			SPLIT_PHYSICAL,
/*z_move_effect*/	0
}
};
