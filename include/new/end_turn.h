#pragma once
#include "../../src/defines.h"
#include "../../src/defines_battle.h"

#define EOS 0xFF

enum EndTurnEffects {
ET_Order,
ET_General_Counter_Decrement,	//Works
ET_Rain,
ET_Sun,
ET_Sandstorm,
ET_Hail,
ET_Air_Current,
ET_Fog,
ET_Weather_Health_Abilities,
ET_Future_Sight,
ET_Wish,
ET_Block_A,
ET_Aqua_Ring,
ET_Ingrain,
ET_Leech_Seed,
ET_Poison,
ET_Toxic,						//Works
ET_Burn,						//Works
ET_Nightmare,
ET_Curse,
ET_Trap_Damage,					//TWorks
ET_Taunt_Timer,
ET_Encore_Timer,
ET_Disable_Timer,
ET_Lock_On_Timer,
ET_Charge_Timer,
ET_Magnet_Rise_Timer,			//Works
ET_Telekinesis_Timer,			//Works
ET_Heal_Block_Timer,			//Works
ET_Embargo_Timer,				//Works
ET_Yawn,
ET_Perish_Song,
ET_Roost,
ET_Reflect,
ET_Light_Screen,
ET_Aurora_Veil,
ET_Safeguard,
ET_Mist,
ET_Tailwind_Timer,				//Works
ET_Lucky_Chant_Timer,			//Works
ET_Rainbow_Timer,				//Works
ET_Sea_Of_Fire_Timer,			//Works
ET_Swamp_Timer,					//Works
ET_Trick_Room_Timer,			//Works
ET_Water_Sport_Timer,			//Works
ET_Mud_Sport_Timer,				//Works
ET_Wonder_Room_Timer,			//Works
ET_Magic_Room_Timer,			//Works
ET_Gravity_Timer,				//Works
ET_Seeds,
ET_Terrain_Timer,				//Works
ET_Block_B,
ET_Form_Change,
ET_End
};

enum Block_A {
ET_Sea_Of_Fire,					//Works
ET_Grassy_Terrain,				//Works
ET_Hydration_ShedSkin,			//Works
ET_Item_Effects,
ET_Healer						//Works
};
#define MAX_CASES_BLOCK_A 5

enum Block_B {
ET_Uproar,
ET_SpeedBoost_Moody_BadDreams,
ET_Orbz,						//Works
ET_Harvest_Pickup,
};
#define MAX_CASES_BLOCK_B 4

#define TURNBASED_MAX_CASE ET_End

#define BattleScript_RainContinuesOrEnds (u8*) 0x81D8A64
#define BattleScript_SunlightFaded (u8*) 0x81D8B18
#define BattleScript_SunlightContinues (u8*) 0x81D8B0A
#define BattleScript_SandStormHailEnds (u8*) 0x81D8B01
#define BattleScript_DamagingWeatherContinues (u8*) 0x81D8A7F
#define BattleScript_MonTookFutureAttack ((u8*) 0x81D8E12)
#define BattleScript_WishComesTrue ((u8*) 0x81D8ED5)
#define BattleScript_ShedSkinActivates ((u8*) 0x81D92D3) 
#define BattleScript_IngrainTurnHeal ((u8*) 0x81D8F0C)
#define BattleScript_LeechSeedTurnDrain ((u8*) 0x81D8B4D)
#define BattleScript_PoisonTurnDmg ((u8*) 0x81D9053)
#define BattleScript_BurnTurnDmg ((u8*) 0x81D9075)
#define BattleScript_NightmareTurnDmg ((u8*) 0x81D9155)
#define BattleScript_CurseTurnDmg ((u8*) 0x81D9166)
#define BattleScript_WrapTurnDmg ((u8*) 0x81D9124)
#define BattleScript_WrapEnds ((u8*) 0x81D9136)
#define BattleScript_EncoredNoMore ((u8*) 0x81D8C65)
#define BattleScript_DisabledNoMore ((u8*) 0x81D8C5E)
#define BattleScript_YawnMakesAsleep ((u8*) 0x81D91C7)
#define BattleScript_PerishSongHits ((u8*) 0x81D8D33)
#define BattleScript_PerishSongTimerGoesDown ((u8*) 0x81D8D4E)
#define BattleScript_SideStatusWoreOff ((u8*) 0x81D8B32)
#define BattleScript_SafeguardEnds ((u8*) 0x81D8B43)
#define BattleScript_PrintUproarOverTurns ((u8*) 0x81D90BC)
#define BattleScript_GiveExp ((u8*) 0x81D8694)
#define BattleScript_HandleFaintedMon ((u8*) 0x81D869D)

