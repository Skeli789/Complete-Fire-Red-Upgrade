#pragma once

#include "../global.h"

/**
 * \file general_bs_commands.h
 * \brief Contains many of the redone commands for battle scripting.
 */

//Exported Functions
void TrySetMissStringForSafetyGoggles(u8 bankDef);
bool8 TryDoBenjaminButterfree(u8 scriptOffset);
s32 GetPsywaveDamage(u8 level, u8 randDamage);
bool8 TrySpitePPReduce(u8 bank, u8 lostPP);
bool8 CanLastMoveNotBeEncored(u8 bank);
bool8 SetSunnyWeather(void);
bool8 SetRainyWeather(void);
bool8 SetHailWeather(void);
bool8 SetSandstormWeather(void);
bool8 SandstormHurts(u8 bank);
bool8 HailHurts(u8 bank);
bool8 TakesDamageFromSandstorm(u8 bank);
bool8 TakesDamageFromHail(u8 bank);
u32 GetSandstormDamage(u8 bank);
u32 GetHailDamage(u8 bank);
void TransformPokemon(u8 bankAtk, u8 bankDef);
u8 CastformDataTypeChange(u8 bank);
bool8 CanSwapItems(u8 bankAtk, u8 bankDef);
u8 GetSecretPowerEffect(void);
u16 GetNaturePowerMove(void);
void RecycleItem(u8 bank);
u8 GetCamouflageType(void);
void BufferAttackerItem(void);
void ResetDoublesSpreadMoveCalcs(void);
void MoveValuesCleanUp(void);

//Exported Constants

enum FaintEffectsStates
{
	Faint_DynamaxHP,
	Faint_ClearEffects,
	Faint_SoulHeart,
	Faint_ReceiverActivate,
	Faint_SwitchInAbilities,
	Faint_PrimalWeather,
	Faint_SkyDrop,
	Faint_RaidBattle,
	Faint_LastPokemonMusic,
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
