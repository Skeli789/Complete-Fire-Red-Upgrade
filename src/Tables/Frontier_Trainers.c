#include "..\\defines.h"
#include "trainer_details.h"
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

#ifdef UNBOUND
const struct BattleTowerTrainer gTowerTrainers[] = 
{
	{ 
	.owNum = 			OW_YOUNGSTER,
	.trainerClass = 	CLASS_YOUNGSTER,
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
	}
};

/*Male Names*/
  u8 Name_Billy[] = { PC_B, PC_i, PC_l, PC_l, PC_y, PC_END };
  u8 Name_Joel[] = { PC_J, PC_o, PC_e, PC_l, PC_END };
  u8 Name_Randy[] = { PC_R, PC_a, PC_n, PC_d, PC_y, PC_END };
  u8 Name_Benny[] = { PC_B, PC_e, PC_n, PC_n, PC_y, PC_END };
  u8 Name_Dexter[] = { PC_D, PC_e, PC_x, PC_t, PC_e, PC_r, PC_END };

/*Female Names*/
  u8 Name_Sarah[] = { PC_S, PC_a, PC_r, PC_a, PC_h, PC_END };
  u8 Name_Yenta[] = { PC_Y, PC_e, PC_n, PC_t, PC_a, PC_END };
  u8 Name_Sakura[] = { PC_S, PC_a, PC_k, PC_u, PC_r, PC_a, PC_END };
  u8 Name_Brittany[] = { PC_B, PC_r, PC_i, PC_t, PC_t, PC_a, PC_n, PC_y, PC_END };
  u8 Name_Rachel[] = { PC_R, PC_a, PC_c, PC_h, PC_e, PC_l, PC_END };
 
 u8* MaleFrontierNamesTable[] = {
 Name_Billy,
 Name_Joel,
 Name_Randy,
 Name_Benny,
 Name_Dexter,
};
 
u8* FemaleFrontierNamesTable[] = {
 Name_Sarah,
 Name_Yenta,
 Name_Sakura,
 Name_Brittany,
 Name_Rachel,
};

#else 
const struct BattleTowerTrainer gTowerTrainers[] = {};
u8* MaleFrontierNamesTable[] = {};
u8* FemaleFrontierNamesTable[] = {};
#endif

