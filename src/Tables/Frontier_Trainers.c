#include "..\\defines.h"
#include "trainer_details.h"
#include "frontier_trainer_names.h"
#include "..\\frontier.h"
#include "..\\text.h"

/*struct BattleTowerTrainer
{
    u16 owNum;
	u8 trainerClass;
	u8 trainerSprite;
	u8 gender;
	u8* preBattleText;
	u8* playerWinText;
	u8* playerLoseText;
};*/

extern u8 FrontierText_Youngster_PreBattle_1[];
extern u8 FrontierText_Youngster_PlayerWin_1[];
extern u8 FrontierText_Youngster_PlayerLose_1[];

#ifdef UNBOUND
const struct BattleTowerTrainer gTowerTrainers[] = 
{
	{ 
	.owNum = 			OW_YOUNGSTER,
	.trainerClass = 	CLASS_YOUNGSTER,
	.trainerSprite = 	TRAINER_SPRITE_YOUNGSTER,
	.gender = 			MALE,
	.preBattleText = 	FrontierText_Youngster_PreBattle_1,
	.playerWinText = 	FrontierText_Youngster_PlayerWin_1,
	.playerLoseText = 	FrontierText_Youngster_PlayerLose_1,
	},
  
	{ 
	.owNum = 			OW_YOUNGSTER,
	.trainerClass =		CLASS_YOUNGSTER,
	.trainerSprite = 	TRAINER_SPRITE_YOUNGSTER,
	.gender = 			MALE,
	.preBattleText = 	(u8*) 0x8AAAAAA,
	.playerWinText = 	(u8*) 0x8AAAAAA,
	.playerLoseText = 	(u8*) 0x8AAAAAA,
	},
	
	{ 
	.owNum = 			OW_YOUNGSTER,
	.trainerClass =		CLASS_YOUNGSTER,
	.trainerSprite = 	TRAINER_SPRITE_YOUNGSTER,
	.gender = 			MALE,
	.preBattleText = 	(u8*) 0x8AAAAAA,
	.playerWinText = 	(u8*) 0x8AAAAAA,
	.playerLoseText = 	(u8*) 0x8AAAAAA,
	},
	
	{ 
	.owNum = 			OW_LASS,
	.trainerClass =		CLASS_LASS,
	.trainerSprite = 	TRAINER_SPRITE_LASS,
	.gender = 			FEMALE,
	.preBattleText = 	(u8*) 0x8AAAAAA,
	.playerWinText = 	(u8*) 0x8AAAAAA,
	.playerLoseText = 	(u8*) 0x8AAAAAA,
	},
};

#else 
const struct BattleTowerTrainer gTowerTrainers[] = {};
u8* MaleFrontierNamesTable[] = {};
u8* FemaleFrontierNamesTable[] = {};
#endif

