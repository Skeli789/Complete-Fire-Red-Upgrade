#ifdef TRAINERS_WITH_EVS

const struct TrainersWithEvs gTrainersWithEvsSpreads[] =
{
[0] = {0}, //Empty Spread
[1] =
	{ //Physical Sweeper: Power Based
		.nature = NATURE_ADAMANT,
		.ivs = 31,
		.hpEv = 6,
		.atkEv = 252,
		.spAtkEv = 252,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_Random_1_2,
	},
[2] =
	{ //Physical Sweeper: Speed Based
		.nature = NATURE_JOLLY,
		.ivs = 31,
		.hpEv = 6,
		.atkEv = 252,
		.spAtkEv = 252,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_Random_1_2,
	},
[3] =
	{ //Special Sweeper: Power Based
		.nature = NATURE_MODEST,
		.ivs = 31,
		.hpEv = 6,
		.spAtkEv = 252,
		.spDefEv = 252,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_Random_1_2,
	},
[4] =
	{ //Special Sweeper: Speed Based
		.nature = NATURE_TIMID,
		.ivs = 31,
		.hpEv = 6,
		.spAtkEv = 252,
		.spDefEv = 252,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_Random_1_2,
	},
[5] =
	{ //Bulky Physical Attacker 
		.nature = NATURE_ADAMANT,
		.ivs = 31,
		.hpEv = 252,
		.atkEv = 252,
		.spAtkEv = 6,
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
		.spdEv = 252,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_Random_1_2,
	},
[8] =
	{ //Bulky Special Attacker 
		.nature = NATURE_MODEST,
		.ivs = 31,
		.hpEv = 252,
		.spAtkEv = 6,
		.spDefEv = 252,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_Random_1_2,
	},
[9] =
	{ //Bulky Special Attacker 
		.nature = NATURE_MODEST,
		.ivs = 31,
		.hpEv = 6,
		.defEv = 252,
		.spDefEv = 252,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_Random_1_2,
	},
[10] =
	{ //Bulky Special Attacker 
		.nature = NATURE_MODEST,
		.ivs = 31,
		.hpEv = 6,
		.spDefEv = 252,
		.spdEv = 252,
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
	{ //Special Wall 
		.nature = NATURE_CALM,
		.ivs = 31,
		.hpEv = 252,
		.spdEv = 252,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_Random_1_2,
	},
[14] =
	{ //Special Wall 
		.nature = NATURE_CAREFUL,
		.ivs = 31,
		.hpEv = 252,
		.spdEv = 252,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_Random_1_2,
	},
[15] =
	{ //Mixed Attacker: Attack Based, Less Defense
		.nature = NATURE_LONELY,
		.ivs = 31,
		.hpEv = 6,
		.atkEv = 128,
		.spAtkEv = 252,
		.spDefEv = 128,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_Random_1_2,
	},
[16] =
	{ //Mixed Attacker: Attack Based, Less Sp. Defense
		.nature = NATURE_NAUGHTY,
		.ivs = 31,
		.hpEv = 6,
		.atkEv = 128,
		.spAtkEv = 252,
		.spDefEv = 128,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_Random_1_2,
	},
[17] =
	{ //Mixed Attacker: Sp. Attack Based, Less Defense
		.nature = NATURE_MILD,
		.ivs = 31,
		.hpEv = 6,
		.atkEv = 128,
		.spAtkEv = 252,
		.spDefEv = 128,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_Random_1_2,
	},
[18] =
	{ //Mixed Attacker: Sp. Attack Based, Less Sp. Defense
		.nature = NATURE_RASH,
		.ivs = 31,
		.hpEv = 6,
		.atkEv = 128,
		.spAtkEv = 252,
		.spDefEv = 128,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_Random_1_2,
	},
[19] =
	{ //Mixed Attacker: Speed Based, Less Defense
		.nature = NATURE_HASTY,
		.ivs = 31,
		.hpEv = 6,
		.atkEv = 128,
		.spAtkEv = 252,
		.spDefEv = 128,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_Random_1_2,
	},
[20] =
	{ //Mixed Attacker: Speed Based, Less Sp. Defense
		.nature = NATURE_NAIVE,
		.ivs = 31,
		.hpEv = 6,
		.atkEv = 128,
		.spAtkEv = 252,
		.spDefEv = 128,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_Random_1_2,
	},
[21] =
	{ //Mixed Wall: Defense Based, Less Attack
		.nature = NATURE_BOLD,
		.ivs = 31,
		.hpEv = 252,
		.defEv = 128,
		.spAtkEv = 4,
		.spdEv = 128,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_Random_1_2,
	},
[22] =
	{ //Mixed Wall: Defense Based, Less Sp. Attack
		.nature = NATURE_IMPISH,
		.ivs = 31,
		.hpEv = 252,
		.defEv = 128,
		.spAtkEv = 4,
		.spdEv = 128,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_Random_1_2,
	},
[23] =
	{ //Mixed Wall: Sp. Defense Based, Less Attack
		.nature = NATURE_CALM,
		.ivs = 31,
		.hpEv = 252,
		.defEv = 128,
		.spAtkEv = 4,
		.spdEv = 128,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_Random_1_2,
	},
[24] =
	{ //Mixed Wall: Sp. Defense Based, Less Sp. Attack
		.nature = NATURE_CAREFUL,
		.ivs = 31,
		.hpEv = 252,
		.defEv = 128,
		.spAtkEv = 4,
		.spdEv = 128,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_Random_1_2,
	},
[25] =
	{ //Fast Wall: HP Based, Less Attack
		.nature = NATURE_TIMID,
		.ivs = 31,
		.hpEv = 252,
		.spAtkEv = 252,
		.spDefEv = 4,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_Random_1_2,
	},
[26] =
	{ //Fast Wall: HP Based, Less Sp. Attack
		.nature = NATURE_JOLLY,
		.ivs = 31,
		.hpEv = 252,
		.atkEv = 4,
		.spAtkEv = 252,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_Random_1_2,
	},
[27] =
	{ //Fast Wall: Defense Based, Less Attack
		.nature = NATURE_TIMID,
		.ivs = 31,
		.hpEv = 4,
		.defEv = 252,
		.spAtkEv = 252,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_Random_1_2,
	},
[28] =
	{ //Fast Wall: Defense Based, Less Sp. Attack
		.nature = NATURE_JOLLY,
		.ivs = 31,
		.hpEv = 4,
		.defEv = 252,
		.spAtkEv = 252,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_Random_1_2,
	},
[29] =
	{ //Fast Wall: Sp. Defense Based, Less Attack
		.nature = NATURE_TIMID,
		.ivs = 31,
		.hpEv = 4,
		.spAtkEv = 252,
		.spdEv = 252,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_Random_1_2,
	},
[30] =
	{ //Fast Wall: Sp. Defense Based, Less Sp. Attack
		.nature = NATURE_JOLLY,
		.ivs = 31,
		.hpEv = 4,
		.spAtkEv = 252,
		.spdEv = 252,
		.ball = TRAINER_EV_CLASS_BALL,
		.ability = Ability_Random_1_2,
	},
};
#endif