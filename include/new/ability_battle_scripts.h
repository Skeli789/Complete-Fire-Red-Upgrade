#pragma once

#include "../global.h"

/**
 * \file move_battle_scripts.h
 * \brief Contains declarations for battle scripts of abilities.
 */

extern const u8 BattleScript_OverworldWeatherStarts[];
extern const u8 BattleScript_NewWeatherAbilityActivates[];
extern const u8 BattleScript_NewWeatherAbilityActivatesCall[];
extern const u8 BattleScript_AirLock[];
extern const u8 BattleScript_WeatherAbilityBlockedByPrimalWeather[];
extern const u8 BattleScript_WeatherAbilityBlockedByPrimalWeatherRet[];
extern const u8 BattleScript_WeatherAbilityBlockedByEvaporate[];
extern const u8 BattleScript_EvaporateOnSwitchIn[];
extern const u8 BattleScript_IntimidateActivatesEnd3[];
extern const u8 BattleScript_TraceActivates[];
extern const u8 BattleScript_Frisk[];
extern const u8 BattleScript_FriskEnd[];
extern const u8 BattleScript_PastelVeil[];
extern const u8 BattleScript_SwitchInAbilityMsg[];
extern const u8 BattleScript_End3[];
extern const u8 BattleScript_AbilityCuredStatus[];
extern const u8 BattleScript_AbilityCuredStatusEnd3[];
extern const u8 BattleScript_StartedSchoolingEnd3[];
extern const u8 BattleScript_StoppedSchoolingEnd3[];
extern const u8 BattleScript_ShieldsDownToCoreEnd3[];
extern const u8 BattleScript_ShieldsDownToMeteorEnd3[];
extern const u8 BattleScript_TransformedEnd3[];
extern const u8 BattleScript_AttackerAbilityStatRaiseEnd3[];
extern const u8 BattleScript_NeutralizingGas[];
extern const u8 BattleScript_ImposterActivates[];
extern const u8 BattleScript_TerrainFromAbility[];
extern const u8 BattleScript_RainDishActivates[];
extern const u8 BattleScript_DrySkinDamage[];
extern const u8 BattleScript_SolarPowerDamage[];
extern const u8 BattleScript_Healer[];
extern const u8 BattleScript_MoodySingleStat[];
extern const u8 BattleScript_MoodyRegular[];
extern const u8 BattleScript_BadDreams[];
extern const u8 BattleScript_Harvest[];
extern const u8 BattleScript_Pickup[];
extern const u8 BattleScript_BallFetch[];
extern const u8 BattleScript_EmergencyExitEnd3[];
extern const u8 BattleScript_PrintCustomStringEnd3[];
extern const u8 BattleScript_AttackerCantUseMove[];
extern const u8 BattleScript_FlashFireBoost[];
extern const u8 BattleScript_FlashFireBoost_PPLoss[];
extern const u8 BattleScript_MonMadeMoveUseless[];
extern const u8 BattleScript_MonMadeMoveUseless_PPLoss[];
extern const u8 BattleScript_MoveHPDrain[];
extern const u8 BattleScript_MoveHPDrain_PPLoss[];
extern const u8 BattleScript_MoveStatDrain[];
extern const u8 BattleScript_MoveStatDrain_PPLoss[];
extern const u8 BattleScript_AbilityApplySecondaryEffect[];
extern const u8 BattleScript_AbilityChangedTypeContact[];
extern const u8 BattleScript_RoughSkinActivates[];
extern const u8 BattleScript_CuteCharmActivates[];
extern const u8 BattleScript_TargetAbilityStatRaise[];
extern const u8 BattleScript_WeakArmorActivates[];
extern const u8 BattleScript_CursedBodyActivates[];
extern const u8 BattleScript_MummyActivates[];
extern const u8 BattleScript_WanderingSpiritActivates[];
extern const u8 BattleScript_AngerPointActivates[];
extern const u8 BattleScript_GooeyActivates[];
extern const u8 BattleScript_IllusionBroken[];
extern const u8 BattleScript_SynchronizeActivates[];
extern const u8 BattleScript_ObliviousPrevents[];
extern const u8 BattleScript_CastformChange[];
extern const u8 BattleScript_CottonDownActivates[];
extern const u8 BattleScript_PerishBody[];
extern const u8 BattleScript_PerishBodyReturn[];

extern const u8 BattleScript_AbilityChangedType[];
extern const u8 BattleScript_DisguiseTookDamage[];
extern const u8 BattleScript_DisguiseTransform[];
extern const u8 BattleScript_IceFaceTookDamage[];
extern const u8 BattleScript_IceFaceTransform[];
extern const u8 BattleScript_EnduredSturdy[];
extern const u8 BattleScript_Receiver[];
extern const u8 BattleScript_PrimalWeatherEnd[];
extern const u8 BattleScript_Symbiosis[];
extern const u8 BattleScript_FlowerGift[];
extern const u8 BattleScript_SoulHeart[];
extern const u8 BattleScript_IllusionBrokenFaint[];
extern const u8 BattleScript_TeamProtectedByFlowerVeil[];
extern const u8 BattleScript_TeamProtectedBySweetVeil[];
extern const u8 BattleScript_TeamProtectedByPastelVeil[];
extern const u8 BattleScript_TargetStayedAwakeUsingAbility[];
extern const u8 BattleScript_ProtectedByAbility[];
extern const u8 BattleScript_SturdyPreventsOHKO[];
extern const u8 BattleScript_StickyHoldActivates[];
extern const u8 BattleScript_StickyHoldActivatesRet[];
extern const u8 BattleScript_AvoidedMoveWithAbility[];
extern const u8 BattleScript_MimicryTransformed[];
extern const u8 BattleScript_MimicryReturnedToNormal[];
extern const u8 BattleScript_CramorantCatchPrey[];
extern const u8 BattleScript_CramorantSpitPrey[];

extern const u8 BattleScript_AbilityNoStatLoss[];
extern const u8 BattleScript_PartnerAbilityNoStatLoss[];
extern const u8 BattleScript_AbilityNoSpecificStatLoss[];
extern const u8 BattleScript_MirrorArmorReflectsStatLoss[];
extern const u8 BattleScript_MirrorArmorReflectsIntimidate[];
extern const u8 BattleScript_DefiantCompetitive[];
extern const u8 BattleScript_TryRemoveIllusion[];

extern const u8 BattleScript_PrimalWeatherEnd[];

extern const u8 BattleScript_AbilityPopUp[];
extern const u8 BattleScript_AbilityPopUpRevert[];

//Strings
extern const u8 gText_ItIsHailing[];
extern const u8 gText_FogIsDeep[];
extern const u8 gText_ViciousSandstormBrewed[];
extern const u8 gText_SnowWarningActivate[];
extern const u8 gText_PrimordialSeaActivate[];
extern const u8 gText_DesolateLandActivate[];
extern const u8 gText_DeltaStream[];
extern const u8 gText_AirLockActivate[];
extern const u8 gText_PressureActivate[];
extern const u8 gText_MoldBreakerActivate[];
extern const u8 gText_TeravoltActivate[];
extern const u8 gText_TurboblazeActivate[];
extern const u8 gText_SlowStartActivate[];
extern const u8 gText_UnnerveActivate[];
extern const u8 gText_DarkAuraActivate[];
extern const u8 gText_FairyAuraActivate[];
extern const u8 gText_AuraBreakActivate[];
extern const u8 gText_ComatoseActivate[];
extern const u8 gText_AnticipationActivate[];
extern const u8 gText_ForewarnActivate[];
extern const u8 gText_FriskActivate[];
extern const u8 gText_ScreenCleanerActivate[];
extern const u8 gText_NeutralizingGasActivate[];
extern const u8 gText_NeutralizingGasEnd[];
extern const u8 gText_CuriousMedicineActivate[];
extern const u8 ElectricTerrainSetString[];
extern const u8 GrassyTerrainSetString[];
extern const u8 MistyTerrainSetString[];
extern const u8 PsychicTerrainSetString[];
extern const u8 gText_SlowStartEnd[];
extern const u8 PrimalRainEndString[];
extern const u8 PrimalSunEndString[];
extern const u8 PrimalAirCurrentEndString[];
extern const u8 gText_BeadsofRuin[];
extern const u8 gText_TabletsofRuin[];
extern const u8 gText_SwordofRuin[];
extern const u8 gText_VesselofRuin[];
extern const u8 gText_CostarActivate[];
extern const u8 gText_GoodAsGoldActivate[];

//Gen 9 Ability Scripts
extern const u8 BattleScript_AngerShellActivates[];
extern const u8 BattleScript_CudChew[];
extern const u8 BattleScript_ElectromorphosisActivates[];
extern const u8 BattleScript_GuardDogActivates[];
extern const u8 BattleScript_LingeringAromaActivates[];
