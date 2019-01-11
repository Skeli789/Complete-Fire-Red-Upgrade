#include "..\\defines.h"
#include "..\\build_pokemon.h"
#include "..\\catching.h"

enum
{
Ability_Hidden,
Ability_1,
Ability_2,
};

struct TrainersWithEvs TrainersWithEvsTable[TRAINERS_WITH_EVS_TABLE_SIZE] = {
{ 0 },	//0 - Empty Spread

{ //1 - Physical Sweeper: Power Based
	.nature = NATURE_ADAMANT,
	.ivs = 31,
	.hpEv = 6,
	.atkEv = 252,
	.spdEv = 252,
	.ball = BALL_TYPE_ULTRA_BALL,
	.ability = Ability_1,
},

};

