#ifdef TRAINERS_WITH_EVS
/*
trainers_with_evs_table.h
	defining the EV/IV/ability/ball type to be assigned to trainer pokemon
*/

const struct TrainersWithEvs gTrainersWithEvsSpreads[] =
{
	[0] = {0}, //Empty Spread
	[1] =
	{ //Physical Sweeper: Power Based
		.nature = NATURE_ADAMANT,
		.ivs = 31,
		.hpEv = 6,
		.atkEv = 252,
		.spdEv = 252,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_Random_1_2,
	},
	[2] =
	{ //Physical Sweeper: Speed Based
		.nature = NATURE_JOLLY,
		.ivs = 31,
		.hpEv = 6,
		.atkEv = 252,
		.spdEv = 252,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_Random_1_2,
	},
	[3] =
	{ //Special Sweeper: Power Based
		.nature = NATURE_MODEST,
		.ivs = 31,
		.hpEv = 6,
		.spdEv = 252,
		.spAtkEv = 252,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_Random_1_2,
	},
	[4] =
	{ //Special Sweeper: Speed Based
		.nature = NATURE_TIMID,
		.ivs = 31,
		.hpEv = 6,
		.spdEv = 252,
		.spAtkEv = 252,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_Random_1_2,
	},
	[5] =
	{ //Bulky Physical Attacker 
		.nature = NATURE_ADAMANT,
		.ivs = 31,
		.hpEv = 252,
		.atkEv = 252,
		.spdEv = 6,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_Random_1_2,
	},
	[6] =
	{ //Bulky Physical Attacker 
		.nature = NATURE_ADAMANT,
		.ivs = 31,
		.hpEv = 6,
		.atkEv = 252,
		.defEv = 252,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_Random_1_2,
	},
	[7] =
	{ //Bulky Physical Attacker 
		.nature = NATURE_ADAMANT,
		.ivs = 31,
		.hpEv = 6,
		.atkEv = 252,
		.spDefEv = 252,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_Random_1_2,
	},
	[8] =
	{ //Bulky Special Attacker 
		.nature = NATURE_MODEST,
		.ivs = 31,
		.hpEv = 252,
		.spdEv = 6,
		.spAtkEv = 252,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_Random_1_2,
	},
	[9] =
	{ //Bulky Special Attacker 
		.nature = NATURE_MODEST,
		.ivs = 31,
		.hpEv = 6,
		.defEv = 252,
		.spAtkEv = 252,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_Random_1_2,
	},
	[10] =
	{ //Bulky Special Attacker 
		.nature = NATURE_MODEST,
		.ivs = 31,
		.hpEv = 6,
		.spAtkEv = 252,
		.spDefEv = 252,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_Random_1_2,
	},
	[11] =
	{ //Physical Wall 
		.nature = NATURE_BOLD,
		.ivs = 31,
		.hpEv = 252,
		.defEv = 252,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_Random_1_2,
	},
	[12] =
	{ //Physical Wall 
		.nature = NATURE_IMPISH,
		.ivs = 31,
		.hpEv = 252,
		.defEv = 252,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_Random_1_2,
	},
	[13] =
	{ //Special Wall: Less Attack
		.nature = NATURE_CALM,
		.ivs = 31,
		.hpEv = 252,
		.spDefEv = 252,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_Random_1_2,
	},
	[14] =
	{ //Special Wall: Less Special Attack
		.nature = NATURE_CAREFUL,
		.ivs = 31,
		.hpEv = 252,
		.spDefEv = 252,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_Random_1_2,
	},
	[15] =
	{ //Mixed Attacker: Attack Based, Less Defense
		.nature = NATURE_LONELY,
		.ivs = 31,
		.hpEv = 6,
		.atkEv = 128,
		.spdEv = 252,
		.spAtkEv = 128,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_Random_1_2,
	},
	[16] =
	{ //Mixed Attacker: Attack Based, Less Sp. Defense
		.nature = NATURE_NAUGHTY,
		.ivs = 31,
		.hpEv = 6,
		.atkEv = 128,
		.spdEv = 252,
		.spAtkEv = 128,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_Random_1_2,
	},
	[17] =
	{ //Mixed Attacker: Sp. Attack Based, Less Defense
		.nature = NATURE_MILD,
		.ivs = 31,
		.hpEv = 6,
		.atkEv = 128,
		.spdEv = 252,
		.spAtkEv = 128,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_Random_1_2,
	},
	[18] =
	{ //Mixed Attacker: Sp. Attack Based, Less Sp. Defense
		.nature = NATURE_RASH,
		.ivs = 31,
		.hpEv = 6,
		.atkEv = 128,
		.spdEv = 252,
		.spAtkEv = 128,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_Random_1_2,
	},
	[19] =
	{ //Mixed Attacker: Speed Based, Less Defense
		.nature = NATURE_HASTY,
		.ivs = 31,
		.hpEv = 6,
		.atkEv = 128,
		.spdEv = 252,
		.spAtkEv = 128,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_Random_1_2,
	},
	[20] =
	{ //Mixed Attacker: Speed Based, Less Sp. Defense
		.nature = NATURE_NAIVE,
		.ivs = 31,
		.hpEv = 6,
		.atkEv = 128,
		.spdEv = 252,
		.spAtkEv = 128,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_Random_1_2,
	},
	[21] =
	{ //Mixed Wall: Defense Based, Less Attack
		.nature = NATURE_BOLD,
		.ivs = 31,
		.hpEv = 252,
		.defEv = 128,
		.spdEv = 4,
		.spDefEv = 128,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_Random_1_2,
	},
	[22] =
	{ //Mixed Wall: Defense Based, Less Sp. Attack
		.nature = NATURE_IMPISH,
		.ivs = 31,
		.hpEv = 252,
		.defEv = 128,
		.spdEv = 4,
		.spDefEv = 128,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_Random_1_2,
	},
	[23] =
	{ //Mixed Wall: Sp. Defense Based, Less Attack
		.nature = NATURE_CALM,
		.ivs = 31,
		.hpEv = 252,
		.defEv = 128,
		.spdEv = 4,
		.spDefEv = 128,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_Random_1_2,
	},
	[24] =
	{ //Mixed Wall: Sp. Defense Based, Less Sp. Attack
		.nature = NATURE_CAREFUL,
		.ivs = 31,
		.hpEv = 252,
		.defEv = 128,
		.spdEv = 4,
		.spDefEv = 128,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_Random_1_2,
	},
	[25] =
	{ //Fast Wall: HP Based, Less Attack
		.nature = NATURE_TIMID,
		.ivs = 31,
		.hpEv = 252,
		.spdEv = 252,
		.spAtkEv = 4,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_Random_1_2,
	},
	[26] =
	{ //Fast Wall: HP Based, Less Sp. Attack
		.nature = NATURE_JOLLY,
		.ivs = 31,
		.hpEv = 252,
		.atkEv = 4,
		.spdEv = 252,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_Random_1_2,
	},
	[27] =
	{ //Fast Wall: Defense Based, Less Attack
		.nature = NATURE_TIMID,
		.ivs = 31,
		.hpEv = 4,
		.defEv = 252,
		.spdEv = 252,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_Random_1_2,
	},
	[28] =
	{ //Fast Wall: Defense Based, Less Sp. Attack
		.nature = NATURE_JOLLY,
		.ivs = 31,
		.hpEv = 4,
		.defEv = 252,
		.spdEv = 252,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_Random_1_2,
	},
	[29] =
	{ //Fast Wall: Sp. Defense Based, Less Attack
		.nature = NATURE_TIMID,
		.ivs = 31,
		.hpEv = 4,
		.spdEv = 252,
		.spDefEv = 252,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_Random_1_2,
	},
	[30] =
	{ //Fast Wall: Sp. Defense Based, Less Sp. Attack
		.nature = NATURE_JOLLY,
		.ivs = 31,
		.hpEv = 4,
		.spdEv = 252,
		.spDefEv = 252,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_Random_1_2,
	},
	[31] =
	{ //Physical Sweeper: Power Based - Ability_1
		.nature = NATURE_ADAMANT,
		.ivs = 31,
		.hpEv = 6,
		.atkEv = 252,
		.spdEv = 252,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_1,
	},
	[32] =
	{ //Physical Sweeper: Speed Based - Ability_1
		.nature = NATURE_JOLLY,
		.ivs = 31,
		.hpEv = 6,
		.atkEv = 252,
		.spdEv = 252,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_1,
	},
	[33] =
	{ //Special Sweeper: Power Based - Ability_1
		.nature = NATURE_MODEST,
		.ivs = 31,
		.hpEv = 6,
		.spdEv = 252,
		.spAtkEv = 252,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_1,
	},
	[34] =
	{ //Special Sweeper: Speed Based - Ability_1
		.nature = NATURE_TIMID,
		.ivs = 31,
		.hpEv = 6,
		.spdEv = 252,
		.spAtkEv = 252,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_1,
	},
	[35] =
	{ //Bulky Physical Attacker - Ability_1
		.nature = NATURE_ADAMANT,
		.ivs = 31,
		.hpEv = 252,
		.atkEv = 252,
		.spdEv = 6,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_1,
	},
	[36] =
	{ //Bulky Physical Attacker - Ability_1
		.nature = NATURE_ADAMANT,
		.ivs = 31,
		.hpEv = 6,
		.atkEv = 252,
		.defEv = 252,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_1,
	},
	[37] =
	{ //Bulky Physical Attacker - Ability_1
		.nature = NATURE_ADAMANT,
		.ivs = 31,
		.hpEv = 6,
		.atkEv = 252,
		.spDefEv = 252,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_1,
	},
	[38] =
	{ //Bulky Special Attacker - Ability_1
		.nature = NATURE_MODEST,
		.ivs = 31,
		.hpEv = 252,
		.spdEv = 6,
		.spAtkEv = 252,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_1,
	},
	[39] =
	{ //Bulky Special Attacker - Ability_1
		.nature = NATURE_MODEST,
		.ivs = 31,
		.hpEv = 6,
		.defEv = 252,
		.spAtkEv = 252,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_1,
	},
	[40] =
	{ //Bulky Special Attacker - Ability_1
		.nature = NATURE_MODEST,
		.ivs = 31,
		.hpEv = 6,
		.spAtkEv = 252,
		.spDefEv = 252,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_1,
	},
	[41] =
	{ //Physical Wall - Ability_1
		.nature = NATURE_BOLD,
		.ivs = 31,
		.hpEv = 252,
		.defEv = 252,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_1,
	},
	[42] =
	{ //Physical Wall - Ability_1
		.nature = NATURE_IMPISH,
		.ivs = 31,
		.hpEv = 252,
		.defEv = 252,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_1,
	},
	[43] =
	{ //Special Wall - Ability_1
		.nature = NATURE_CALM,
		.ivs = 31,
		.hpEv = 252,
		.spDefEv = 252,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_1,
	},
	[44] =
	{ //Special Wall - Ability_1
		.nature = NATURE_CAREFUL,
		.ivs = 31,
		.hpEv = 252,
		.spDefEv = 252,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_1,
	},
	[45] =
	{ //Mixed Attacker: Attack Based, Less Defense - Ability_1
		.nature = NATURE_LONELY,
		.ivs = 31,
		.hpEv = 6,
		.atkEv = 128,
		.spdEv = 252,
		.spAtkEv = 128,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_1,
	},
	[46] =
	{ //Mixed Attacker: Attack Based, Less Sp. Defense - Ability_1
		.nature = NATURE_NAUGHTY,
		.ivs = 31,
		.hpEv = 6,
		.atkEv = 128,
		.spdEv = 252,
		.spAtkEv = 128,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_1,
	},
	[47] =
	{ //Mixed Attacker: Sp. Attack Based, Less Defense - Ability_1
		.nature = NATURE_MILD,
		.ivs = 31,
		.hpEv = 6,
		.atkEv = 128,
		.spdEv = 252,
		.spAtkEv = 128,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_1,
	},
	[48] =
	{ //Mixed Attacker: Sp. Attack Based, Less Sp. Defense - Ability_1
		.nature = NATURE_RASH,
		.ivs = 31,
		.hpEv = 6,
		.atkEv = 128,
		.spdEv = 252,
		.spAtkEv = 128,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_1,
	},
	[49] =
	{ //Mixed Attacker: Speed Based, Less Defense - Ability_1
		.nature = NATURE_HASTY,
		.ivs = 31,
		.hpEv = 6,
		.atkEv = 128,
		.spdEv = 252,
		.spAtkEv = 128,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_1,
	},
	[50] =
	{ //Mixed Attacker: Speed Based, Less Sp. Defense - Ability_1
		.nature = NATURE_NAIVE,
		.ivs = 31,
		.hpEv = 6,
		.atkEv = 128,
		.spdEv = 252,
		.spAtkEv = 128,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_1,
	},
	[51] =
	{ //Mixed Wall: Defense Based, Less Attack - Ability_1
		.nature = NATURE_BOLD,
		.ivs = 31,
		.hpEv = 252,
		.defEv = 128,
		.spdEv = 4,
		.spDefEv = 128,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_1,
	},
	[52] =
	{ //Mixed Wall: Defense Based, Less Sp. Attack - Ability_1
		.nature = NATURE_IMPISH,
		.ivs = 31,
		.hpEv = 252,
		.defEv = 128,
		.spdEv = 4,
		.spDefEv = 128,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_1,
	},
	[53] =
	{ //Mixed Wall: Sp. Defense Based, Less Attack - Ability_1
		.nature = NATURE_CALM,
		.ivs = 31,
		.hpEv = 252,
		.defEv = 128,
		.spdEv = 4,
		.spDefEv = 128,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_1,
	},
	[54] =
	{ //Mixed Wall: Sp. Defense Based, Less Sp. Attack - Ability_1
		.nature = NATURE_CAREFUL,
		.ivs = 31,
		.hpEv = 252,
		.defEv = 128,
		.spdEv = 4,
		.spDefEv = 128,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_1,
	},
	[55] =
	{ //Fast Wall: HP Based, Less Attack - Ability_1
		.nature = NATURE_TIMID,
		.ivs = 31,
		.hpEv = 252,
		.spdEv = 252,
		.spAtkEv = 4,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_1,
	},
	[56] =
	{ //Fast Wall: HP Based, Less Sp. Attack - Ability_1
		.nature = NATURE_JOLLY,
		.ivs = 31,
		.hpEv = 252,
		.atkEv = 4,
		.spdEv = 252,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_1,
	},
	[57] =
	{ //Fast Wall: Defense Based, Less Attack - Ability_1
		.nature = NATURE_TIMID,
		.ivs = 31,
		.hpEv = 4,
		.defEv = 252,
		.spdEv = 252,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_1,
	},
	[58] =
	{ //Fast Wall: Defense Based, Less Sp. Attack - Ability_1
		.nature = NATURE_JOLLY,
		.ivs = 31,
		.hpEv = 4,
		.defEv = 252,
		.spdEv = 252,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_1,
	},
	[59] =
	{ //Fast Wall: Sp. Defense Based, Less Attack - Ability_1
		.nature = NATURE_TIMID,
		.ivs = 31,
		.hpEv = 4,
		.spdEv = 252,
		.spDefEv = 252,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_1,
	},
	[60] =
	{ //Fast Wall: Sp. Defense Based, Less Sp. Attack - Ability_1
		.nature = NATURE_JOLLY,
		.ivs = 31,
		.hpEv = 4,
		.spdEv = 252,
		.spDefEv = 252,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_1,
	},
	[61] =
	{ //Physical Sweeper: Power Based - Ability_2
		.nature = NATURE_ADAMANT,
		.ivs = 31,
		.hpEv = 6,
		.atkEv = 252,
		.spdEv = 252,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_2,
	},
	[62] =
	{ //Physical Sweeper: Speed Based - Ability_2
		.nature = NATURE_JOLLY,
		.ivs = 31,
		.hpEv = 6,
		.atkEv = 252,
		.spdEv = 252,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_2,
	},
	[63] =
	{ //Special Sweeper: Power Based - Ability_2
		.nature = NATURE_MODEST,
		.ivs = 31,
		.hpEv = 6,
		.spdEv = 252,
		.spAtkEv = 252,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_2,
	},
	[64] =
	{ //Special Sweeper: Speed Based - Ability_2
		.nature = NATURE_TIMID,
		.ivs = 31,
		.hpEv = 6,
		.spdEv = 252,
		.spAtkEv = 252,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_2,
	},
	[65] =
	{ //Bulky Physical Attacker - Ability_2
		.nature = NATURE_ADAMANT,
		.ivs = 31,
		.hpEv = 252,
		.atkEv = 252,
		.spdEv = 6,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_2,
	},
	[66] =
	{ //Bulky Physical Attacker - Ability_2
		.nature = NATURE_ADAMANT,
		.ivs = 31,
		.hpEv = 6,
		.atkEv = 252,
		.defEv = 252,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_2,
	},
	[67] =
	{ //Bulky Physical Attacker - Ability_2
		.nature = NATURE_ADAMANT,
		.ivs = 31,
		.hpEv = 6,
		.atkEv = 252,
		.spDefEv = 252,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_2,
	},
	[68] =
	{ //Bulky Special Attacker - Ability_2
		.nature = NATURE_MODEST,
		.ivs = 31,
		.hpEv = 252,
		.spdEv = 6,
		.spAtkEv = 252,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_2,
	},
	[69] =
	{ //Bulky Special Attacker - Ability_2
		.nature = NATURE_MODEST,
		.ivs = 31,
		.hpEv = 6,
		.defEv = 252,
		.spAtkEv = 252,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_2,
	},
	[70] =
	{ //Bulky Special Attacker - Ability_2
		.nature = NATURE_MODEST,
		.ivs = 31,
		.hpEv = 6,
		.spAtkEv = 252,
		.spDefEv = 252,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_2,
	},
	[71] =
	{ //Physical Wall - Ability_2
		.nature = NATURE_BOLD,
		.ivs = 31,
		.hpEv = 252,
		.defEv = 252,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_2,
	},
	[72] =
	{ //Physical Wall - Ability_2
		.nature = NATURE_IMPISH,
		.ivs = 31,
		.hpEv = 252,
		.defEv = 252,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_2,
	},
	[73] =
	{ //Special Wall - Ability_2
		.nature = NATURE_CALM,
		.ivs = 31,
		.hpEv = 252,
		.spDefEv = 252,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_2,
	},
	[74] =
	{ //Special Wall - Ability_2
		.nature = NATURE_CAREFUL,
		.ivs = 31,
		.hpEv = 252,
		.spDefEv = 252,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_2,
	},
	[75] =
	{ //Mixed Attacker: Attack Based, Less Defense - Ability_2
		.nature = NATURE_LONELY,
		.ivs = 31,
		.hpEv = 6,
		.atkEv = 128,
		.spdEv = 252,
		.spAtkEv = 128,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_2,
	},
	[76] =
	{ //Mixed Attacker: Attack Based, Less Sp. Defense - Ability_2
		.nature = NATURE_NAUGHTY,
		.ivs = 31,
		.hpEv = 6,
		.atkEv = 128,
		.spdEv = 252,
		.spAtkEv = 128,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_2,
	},
	[77] =
	{ //Mixed Attacker: Sp. Attack Based, Less Defense - Ability_2
		.nature = NATURE_MILD,
		.ivs = 31,
		.hpEv = 6,
		.atkEv = 128,
		.spdEv = 252,
		.spAtkEv = 128,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_2,
	},
	[78] =
	{ //Mixed Attacker: Sp. Attack Based, Less Sp. Defense - Ability_2
		.nature = NATURE_RASH,
		.ivs = 31,
		.hpEv = 6,
		.atkEv = 128,
		.spdEv = 252,
		.spAtkEv = 128,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_2,
	},
	[79] =
	{ //Mixed Attacker: Speed Based, Less Defense - Ability_2
		.nature = NATURE_HASTY,
		.ivs = 31,
		.hpEv = 6,
		.atkEv = 128,
		.spdEv = 252,
		.spAtkEv = 128,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_2,
	},
	[80] =
	{ //Mixed Attacker: Speed Based, Less Sp. Defense - Ability_2
		.nature = NATURE_NAIVE,
		.ivs = 31,
		.hpEv = 6,
		.atkEv = 128,
		.spdEv = 252,
		.spAtkEv = 128,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_2,
	},
	[81] =
	{ //Mixed Wall: Defense Based, Less Attack - Ability_2
		.nature = NATURE_BOLD,
		.ivs = 31,
		.hpEv = 252,
		.defEv = 128,
		.spdEv = 4,
		.spDefEv = 128,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_2,
	},
	[82] =
	{ //Mixed Wall: Defense Based, Less Sp. Attack - Ability_2
		.nature = NATURE_IMPISH,
		.ivs = 31,
		.hpEv = 252,
		.defEv = 128,
		.spdEv = 4,
		.spDefEv = 128,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_2,
	},
	[83] =
	{ //Mixed Wall: Sp. Defense Based, Less Attack - Ability_2
		.nature = NATURE_CALM,
		.ivs = 31,
		.hpEv = 252,
		.defEv = 128,
		.spdEv = 4,
		.spDefEv = 128,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_2,
	},
	[84] =
	{ //Mixed Wall: Sp. Defense Based, Less Sp. Attack - Ability_2
		.nature = NATURE_CAREFUL,
		.ivs = 31,
		.hpEv = 252,
		.defEv = 128,
		.spdEv = 4,
		.spDefEv = 128,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_2,
	},
	[85] =
	{ //Fast Wall: HP Based, Less Attack - Ability_2
		.nature = NATURE_TIMID,
		.ivs = 31,
		.hpEv = 252,
		.spdEv = 252,
		.spAtkEv = 4,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_2,
	},
	[86] =
	{ //Fast Wall: HP Based, Less Sp. Attack - Ability_2
		.nature = NATURE_JOLLY,
		.ivs = 31,
		.hpEv = 252,
		.atkEv = 4,
		.spdEv = 252,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_2,
	},
	[87] =
	{ //Fast Wall: Defense Based, Less Attack - Ability_2
		.nature = NATURE_TIMID,
		.ivs = 31,
		.hpEv = 4,
		.defEv = 252,
		.spdEv = 252,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_2,
	},
	[88] =
	{ //Fast Wall: Defense Based, Less Sp. Attack - Ability_2
		.nature = NATURE_JOLLY,
		.ivs = 31,
		.hpEv = 4,
		.defEv = 252,
		.spdEv = 252,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_2,
	},
	[89] =
	{ //Fast Wall: Sp. Defense Based, Less Attack - Ability_2
		.nature = NATURE_TIMID,
		.ivs = 31,
		.hpEv = 4,
		.spdEv = 252,
		.spDefEv = 252,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_2,
	},
	[90] =
	{ //Fast Wall: Sp. Defense Based, Less Sp. Attack - Ability_2
		.nature = NATURE_JOLLY,
		.ivs = 31,
		.hpEv = 4,
		.spdEv = 252,
		.spDefEv = 252,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_2,
	},
	[91] =
	{ //Physical Sweeper: Power Based - Ability_Hidden
		.nature = NATURE_ADAMANT,
		.ivs = 31,
		.hpEv = 6,
		.atkEv = 252,
		.spdEv = 252,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_Hidden,
	},
	[92] =
	{ //Physical Sweeper: Speed Based - Ability_Hidden
		.nature = NATURE_JOLLY,
		.ivs = 31,
		.hpEv = 6,
		.atkEv = 252,
		.spdEv = 252,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_Hidden,
	},
	[93] =
	{ //Special Sweeper: Power Based - Ability_Hidden
		.nature = NATURE_MODEST,
		.ivs = 31,
		.hpEv = 6,
		.spdEv = 252,
		.spAtkEv = 252,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_Hidden,
	},
	[94] =
	{ //Special Sweeper: Speed Based - Ability_Hidden
		.nature = NATURE_TIMID,
		.ivs = 31,
		.hpEv = 6,
		.spdEv = 252,
		.spAtkEv = 252,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_Hidden,
	},
	[95] =
	{ //Bulky Physical Attacker - Ability_Hidden
		.nature = NATURE_ADAMANT,
		.ivs = 31,
		.hpEv = 252,
		.atkEv = 252,
		.spdEv = 6,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_Hidden,
	},
	[96] =
	{ //Bulky Physical Attacker - Ability_Hidden
		.nature = NATURE_ADAMANT,
		.ivs = 31,
		.hpEv = 6,
		.atkEv = 252,
		.defEv = 252,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_Hidden,
	},
	[97] =
	{ //Bulky Physical Attacker - Ability_Hidden
		.nature = NATURE_ADAMANT,
		.ivs = 31,
		.hpEv = 6,
		.atkEv = 252,
		.spDefEv = 252,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_Hidden,
	},
	[98] =
	{ //Bulky Special Attacker - Ability_Hidden
		.nature = NATURE_MODEST,
		.ivs = 31,
		.hpEv = 252,
		.spdEv = 6,
		.spAtkEv = 252,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_Hidden,
	},
	[99] =
	{ //Bulky Special Attacker - Ability_Hidden
		.nature = NATURE_MODEST,
		.ivs = 31,
		.hpEv = 6,
		.defEv = 252,
		.spAtkEv = 252,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_Hidden,
	},
	[100] =
	{ //Bulky Special Attacker - Ability_Hidden
		.nature = NATURE_MODEST,
		.ivs = 31,
		.hpEv = 6,
		.spAtkEv = 252,
		.spDefEv = 252,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_Hidden,
	},
	[101] =
	{ //Physical Wall - Ability_Hidden
		.nature = NATURE_BOLD,
		.ivs = 31,
		.hpEv = 252,
		.defEv = 252,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_Hidden,
	},
	[102] =
	{ //Physical Wall - Ability_Hidden
		.nature = NATURE_IMPISH,
		.ivs = 31,
		.hpEv = 252,
		.defEv = 252,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_Hidden,
	},
	[103] =
	{ //Special Wall - Ability_Hidden
		.nature = NATURE_CALM,
		.ivs = 31,
		.hpEv = 252,
		.spDefEv = 252,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_Hidden,
	},
	[104] =
	{ //Special Wall - Ability_Hidden
		.nature = NATURE_CAREFUL,
		.ivs = 31,
		.hpEv = 252,
		.spDefEv = 252,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_Hidden,
	},
	[105] =
	{ //Mixed Attacker: Attack Based, Less Defense - Ability_Hidden
		.nature = NATURE_LONELY,
		.ivs = 31,
		.hpEv = 6,
		.atkEv = 128,
		.spdEv = 252,
		.spAtkEv = 128,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_Hidden,
	},
	[106] =
	{ //Mixed Attacker: Attack Based, Less Sp. Defense - Ability_Hidden
		.nature = NATURE_NAUGHTY,
		.ivs = 31,
		.hpEv = 6,
		.atkEv = 128,
		.spdEv = 252,
		.spAtkEv = 128,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_Hidden,
	},
	[107] =
	{ //Mixed Attacker: Sp. Attack Based, Less Defense - Ability_Hidden
		.nature = NATURE_MILD,
		.ivs = 31,
		.hpEv = 6,
		.atkEv = 128,
		.spdEv = 252,
		.spAtkEv = 128,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_Hidden,
	},
	[108] =
	{ //Mixed Attacker: Sp. Attack Based, Less Sp. Defense - Ability_Hidden
		.nature = NATURE_RASH,
		.ivs = 31,
		.hpEv = 6,
		.atkEv = 128,
		.spdEv = 252,
		.spAtkEv = 128,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_Hidden,
	},
	[109] =
	{ //Mixed Attacker: Speed Based, Less Defense - Ability_Hidden
		.nature = NATURE_HASTY,
		.ivs = 31,
		.hpEv = 6,
		.atkEv = 128,
		.spdEv = 252,
		.spAtkEv = 128,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_Hidden,
	},
	[110] =
	{ //Mixed Attacker: Speed Based, Less Sp. Defense - Ability_Hidden
		.nature = NATURE_NAIVE,
		.ivs = 31,
		.hpEv = 6,
		.atkEv = 128,
		.spdEv = 252,
		.spAtkEv = 128,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_Hidden,
	},
	[111] =
	{ //Mixed Wall: Defense Based, Less Attack - Ability_Hidden
		.nature = NATURE_BOLD,
		.ivs = 31,
		.hpEv = 252,
		.defEv = 128,
		.spdEv = 4,
		.spDefEv = 128,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_Hidden,
	},
	[112] =
	{ //Mixed Wall: Defense Based, Less Sp. Attack - Ability_Hidden
		.nature = NATURE_IMPISH,
		.ivs = 31,
		.hpEv = 252,
		.defEv = 128,
		.spdEv = 4,
		.spDefEv = 128,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_Hidden,
	},
	[113] =
	{ //Mixed Wall: Sp. Defense Based, Less Attack - Ability_Hidden
		.nature = NATURE_CALM,
		.ivs = 31,
		.hpEv = 252,
		.defEv = 128,
		.spdEv = 4,
		.spDefEv = 128,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_Hidden,
	},
	[114] =
	{ //Mixed Wall: Sp. Defense Based, Less Sp. Attack - Ability_Hidden
		.nature = NATURE_CAREFUL,
		.ivs = 31,
		.hpEv = 252,
		.defEv = 128,
		.spdEv = 4,
		.spDefEv = 128,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_Hidden,
	},
	[115] =
	{ //Fast Wall: HP Based, Less Attack - Ability_Hidden
		.nature = NATURE_TIMID,
		.ivs = 31,
		.hpEv = 252,
		.spdEv = 252,
		.spAtkEv = 4,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_Hidden,
	},
	[116] =
	{ //Fast Wall: HP Based, Less Sp. Attack - Ability_Hidden
		.nature = NATURE_JOLLY,
		.ivs = 31,
		.hpEv = 252,
		.atkEv = 4,
		.spdEv = 252,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_Hidden,
	},
	[117] =
	{ //Fast Wall: Defense Based, Less Attack - Ability_Hidden
		.nature = NATURE_TIMID,
		.ivs = 31,
		.hpEv = 4,
		.defEv = 252,
		.spdEv = 252,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_Hidden,
	},
	[118] =
	{ //Fast Wall: Defense Based, Less Sp. Attack - Ability_Hidden
		.nature = NATURE_JOLLY,
		.ivs = 31,
		.hpEv = 4,
		.defEv = 252,
		.spdEv = 252,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_Hidden,
	},
	[119] =
	{ //Fast Wall: Sp. Defense Based, Less Attack - Ability_Hidden
		.nature = NATURE_TIMID,
		.ivs = 31,
		.hpEv = 4,
		.spdEv = 252,
		.spDefEv = 252,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_Hidden,
	},
	[120] =
	{ //Fast Wall: Sp. Defense Based, Less Sp. Attack - Ability_Hidden
		.nature = NATURE_JOLLY,
		.ivs = 31,
		.hpEv = 4,
		.spdEv = 252,
		.spDefEv = 252,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_Hidden,
	},
};
#endif
