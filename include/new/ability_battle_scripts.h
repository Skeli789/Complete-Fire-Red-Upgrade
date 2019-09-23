#pragma once

#include "../global.h"

/**
 * \file move_battle_scripts.h
 * \brief Contains declarations for battle scripts of abilities.
 */

#define BattleScript_OverworldWeatherStarts (u8*) 0x81D8B1F

extern u8 BattleScript_NewWeatherAbilityActivates[];
extern u8 BattleScript_AirLock[];
extern u8 BattleScript_IntimidateActivatesEnd3[];
extern u8 BattleScript_TraceActivates[];
extern u8 BattleScript_Frisk[];
extern u8 BattleScript_FriskEnd[];
extern u8 BattleScript_SwitchInAbilityMsg[];
extern u8 BattleScript_AbilityCuredStatus[];
extern u8 BattleScript_AbilityCuredStatusEnd3[];
extern u8 BattleScript_StartedSchoolingEnd3[];
extern u8 BattleScript_StoppedSchoolingEnd3[];
extern u8 BattleScript_ShieldsDownToCoreEnd3[];
extern u8 BattleScript_ShieldsDownToMeteorEnd3[];
extern u8 BattleScript_TransformedEnd3[];
extern u8 BattleScript_AttackerAbilityStatRaiseEnd3[];
extern u8 BattleScript_ImposterActivates[];
extern u8 BattleScript_TerrainFromAbility[];
extern u8 BattleScript_RainDishActivates[];
extern u8 BattleScript_DrySkinDamage[];
extern u8 BattleScript_SolarPowerDamage[];
extern u8 BattleScript_Healer[];
extern u8 BattleScript_MoodySingleStat[];
extern u8 BattleScript_MoodyRegular[];
extern u8 BattleScript_BadDreams[];
extern u8 BattleScript_Harvest[];
extern u8 BattleScript_Pickup[];
extern u8 BattleScript_PrintCustomStringEnd3[];
extern u8 BattleScript_AttackerCantUseMove[];
extern u8 BattleScript_FlashFireBoost[];
extern u8 BattleScript_FlashFireBoost_PPLoss[];
extern u8 BattleScript_MonMadeMoveUseless[];
extern u8 BattleScript_MonMadeMoveUseless_PPLoss[];
extern u8 BattleScript_MoveHPDrain[];
extern u8 BattleScript_MoveHPDrain_PPLoss[];
extern u8 BattleScript_MoveStatDrain[];
extern u8 BattleScript_MoveStatDrain_PPLoss[];
extern u8 BattleScript_AbilityApplySecondaryEffect[];
extern u8 BattleScript_AbilityChangedTypeContact[];
extern u8 BattleScript_RoughSkinActivates[];
extern u8 BattleScript_CuteCharmActivates[];
extern u8 BattleScript_TargetAbilityStatRaise[];
extern u8 BattleScript_WeakArmorActivates[];
extern u8 BattleScript_CursedBodyActivates[];
extern u8 BattleScript_MummyActivates[];
extern u8 BattleScript_AngerPointActivates[];
extern u8 BattleScript_GooeyActivates[];
extern u8 BattleScript_IllusionBroken[];
extern u8 BattleScript_SynchronizeActivates[];
extern u8 BattleScript_ObliviousPrevents[];
extern u8 BattleScript_CastformChange[];

extern u8 BattleScript_AbilityChangedType[];
extern u8 BattleScript_MimikyuTookDamage[];
extern u8 BattleScript_MimikyuTransform[];
extern u8 BattleScript_EnduredSturdy[];
extern u8 BattleScript_Receiver[];
extern u8 BattleScript_PrimalWeatherEnd[];
extern u8 BattleScript_Symbiosis[];
extern u8 BattleScript_FlowerGift[];
extern u8 BattleScript_SoulHeart[];
extern u8 BattleScript_IllusionBrokenFaint[];
extern u8 BattleScript_TeamProtectedByFlowerVeil[];
extern u8 BattleScript_TeamProtectedBySweetVeil[];
extern u8 BattleScript_TargetStayedAwakeUsingAbility[];
extern u8 BattleScript_ProtectedByAbility[];
extern u8 BattleScript_SturdyPreventsOHKO[];
extern u8 BattleScript_StickyHoldActivates[];
extern u8 BattleScript_StickyHoldActivatesRet[];
extern u8 BattleScript_AvoidedMoveWithAbility[];

extern u8 BattleScript_AbilityNoStatLoss[];
extern u8 BattleScript_AbilityNoSpecificStatLoss[];
extern u8 BattleScript_DefiantCompetitive[];

extern u8 BattleScript_PrimalWeatherEnd[];

extern u8 BattleScript_AbilityPopUpRevert[];

//Strings
extern u8 gText_FogIsDeep[];
extern u8 gText_SnowWarningActivate[];
extern u8 gText_PrimordialSeaActivate[];
extern u8 gText_DesolateLandActivate[];
extern u8 gText_DeltaStream[];
extern u8 gText_AirLockActivate[];
extern u8 gText_PressureActivate[];
extern u8 gText_MoldBreakerActivate[];
extern u8 gText_TeravoltActivate[];
extern u8 gText_TurboblazeActivate[];
extern u8 gText_SlowStartActivate[];
extern u8 gText_UnnerveActivate[];
extern u8 gText_DarkAuraActivate[];
extern u8 gText_FairyAuraActivate[];
extern u8 gText_AuraBreakActivate[];
extern u8 gText_ComatoseActivate[];
extern u8 gText_AnticipationActivate[];
extern u8 gText_ForewarnActivate[];
extern u8 gText_FriskActivate[];
extern u8 ElectricTerrainSetString[];
extern u8 GrassyTerrainSetString[];
extern u8 MistyTerrainSetString[];
extern u8 PsychicTerrainSetString[];
extern u8 gText_SlowStartEnd[];
extern u8 PrimalRainEndString[];
extern u8 PrimalSunEndString[];
extern u8 PrimalAirCurrentEndString[];
