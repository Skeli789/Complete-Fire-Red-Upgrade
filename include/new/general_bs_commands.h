#pragma once

#include "../global.h"

/**
 * \file general_bs_commands.h
 * \brief Contains many of the redone commands for battle scripting.
 */
 
//Exported Functions
s32 GetPsywaveDamage(u8 randDamage);
u8 CheckMoveLimitations(u8 bank, u8 unusableMoves, u8 check);
u8 CheckMoveLimitationsFromParty(struct Pokemon* mon, u8 unusableMoves, u8 check);
bool8 SandstormHurts(u8 bank);
bool8 HailHurts(u8 bank);
bool8 TakesDamageFromSandstorm(u8 bank);
bool8 TakesDamageFromHail(u8 bank);
u32 GetSandstormDamage(u8 bank);
u32 GetHailDamage(u8 bank);
void TransformPokemon(u8 bankAtk, u8 bankDef);
u8 CastformDataTypeChange(u8 bank);
u16 GetNaturePowerMove(void);
u8 GetCamouflageType(void);
void BufferAttackerItem(void);
void MoveValuesCleanUp(void);

//Exported Constants
#define MOVE_LIMITATION_ZEROMOVE    (1 << 0)
#define MOVE_LIMITATION_PP          (1 << 1)
#define MOVE_LIMITATION_DISABLED    (1 << 2)
#define MOVE_LIMITATION_TORMENTED   (1 << 3)
#define MOVE_LIMITATION_TAUNT       (1 << 4)
#define MOVE_LIMITATION_IMPRISION   (1 << 5)
#define MOVE_LIMITATION_CHOICE		(1 << 6)
#define MOVE_LIMITATION_ENCORE		(1 << 7)

enum FaintEffectsStates
{
	Faint_ClearEffects,
	Faint_SoulHeart,
	Faint_ReceiverActivate,
	Faint_SwitchInAbilities,
	Faint_PrimalWeather,
	Faint_FormsRevert,
	Faint_FormsStats,
	Faint_FormsHP,
};

enum
{
	CASTFORM_NO_CHANGE, //0
	CASTFORM_TO_NORMAL, //1
	CASTFORM_TO_FIRE,   //2
	CASTFORM_TO_WATER,  //3
	CASTFORM_TO_ICE,    //4
};


enum CastformForms
{
	CASTFORM_NORMAL,
	CASTFORM_SUN,
	CASTFORM_RAIN,
	CASTFORM_HAIL
};