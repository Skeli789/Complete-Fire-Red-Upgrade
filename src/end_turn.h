#pragma once
#include "defines.h"

#define EOS 0xFF

enum EndTurnEffects {
ET_General_Counter_Decrement,	//Works
ET_Weather_Health_Abilities,
ET_Future_Sight,
ET_Wish,
ET_Thrash,
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

u8* ByBankCounterTable[] = {
DisabledMoldBreakerAbilities,
LaserFocusTimers,
ThroatChopTimers,
StompingTantrumTimers,
StakeoutCounters,
DestinyBondCounters,
ElectrifyTimers,
(u8*) 0xFEFEFEFE //End
};

u8* BytesCounterTable[] = {
RetaliateCounters,
RetaliateCounters + 1, 
&EchoedVoiceCounter,
(u8*) 0x203D87E, //Fusion Flare
(u8*) 0x203D87F, //Fusion Bolt
//&EnduranceHelper, //Endurance
&IonDelugeTimer, //Ion Deluge
&FairyLockTimer, //Fairy Lock
(u8*) 0xFEFEFEFE //End
};

u8* BitsCounterTable[] = {
&PowderByte,
&BeakBlastByte,
&CustapQuickClawIndicator,
(u8*) 0xFEFEFEFE
};