#pragma once

extern const u8* gBattleScriptsForMoveEffects[];

//Scripts
#define BattleScript_FlushMessageBox (u8*) 0x81D96A8
#define BattleScript_MoveFaint (u8*) 0x81D6947
#define BattleScript_MoveEnd (u8*) 0x81D694E
#define BattleScript_ButItDoesntAffect (u8*) 0x81D7E04
#define BattleScript_Pausex20 (u8*) 0x81D89F1
#define BattleScript_MoveMissedPause (u8*) 0x81D695E
#define BattleScript_HitFromCritCalc (u8*) 0x81D6930
#define BattleScript_AlreadyAtFullHp (u8*) 0x81D7DD1
#define BattleScript_PresentHealTarget (u8*) 0x81D7DB7
#define BattleScript_DestinyBondTakesLife (u8*) 0x81D8C6C
#define BattleScript_GrudgeTakesPp (u8*) 0x81D8FA3
#define BattleScript_RestCantSleep (u8*) 0x81D6EB2
#define BattleScript_ResultMessage (u8*) 0x81D6942

#define BattleScript_SubstituteFade (u8*)  0x81D9526
#define BattleScript_HangedOnMsg (u8*) 0x81D9641
#define BattleScript_OneHitKOMsg (u8*) 0x81D8FE4
#define BattleScript_PrintPayDayMoneyString (u8*) 0x81D911D
#define BattleScript_WrapFree (u8*) 0x81D8DF3
#define BattleScript_LeechSeedFree (u8*) 0x81D8E04
#define BattleScript_SpikesFree (u8*) 0x81D8E0B

#define BattleScript_AllStatsUp (u8*) 0x81D8D55
#define BattleScript_RapidSpinAway (u8*) 0x81D8DF1
#define BattleScript_TargetPRLZHeal (u8*) 0x81D9177
#define BattleScript_AtkDefDown (u8*) 0x81D8F35
#define BattleScript_KnockedOff (u8*) 0x81D8F86
#define BattleScript_StatUp (u8*) 0x81D6BD1
#define BattleScript_StatDown (u8*) 0x81D6C62
#define BattleScript_SAtkDown2 (u8*) 0x81D8FEB
#define ScreensShatteredString (u8*) 0x83FC646

extern u8 BattleScript_PauseResultMessage[];
extern u8 BattleScript_DarkVoidFail[];
extern u8 BattleScript_MagneticFluxStatBoost[];
extern u8 BattleScript_GearUpStatBoost[];
extern u8 BattleScript_SapSipperAromatherapy[];
extern u8 BattleScript_DefogAdditionalEffects[];
extern u8 BattleScript_PledgeCombined[];
extern u8 BattleScript_PledgeReady[];
extern u8 BattleScript_PrintCustomString[];

extern u8 BattleScript_NoHealTargetAfterHealBlock[];
extern u8 BattleScript_FaintAttacker[];
extern u8 BattleScript_FaintTarget[];
extern u8 BattleScript_FaintScriptingBank[];
extern u8 BattleScript_ButItFailedAttackstring[];
extern u8 BattleScript_EnduredMsg[];
extern u8 BattleScript_SideStatusWoreOffRet[];

extern u8 BattleScript_TargetSleepHeal[];
extern u8 BattleScript_TargetBurnHeal[];
extern u8 BattleScript_ItemSteal[];
extern u8 BattleScript_AbilityWasSuppressed[];

//Strings
extern u8 CraftyShieldProtectedString[];
extern u8 MatBlockProtectedString[];
extern u8 QuickGuardProtectedString[];
extern u8 WideGuardProtectedString[];
extern u8 PowerTrickString[];
extern u8 PowerSwapString[];
extern u8 GuardSwapString[];
extern u8 SpeedSwapString[];
extern u8 HeartSwapString[];
extern u8 PowerSplitString[];
extern u8 GuardSplitString[];
extern u8 SwampString[];
extern u8 SeaOfFireString[];
extern u8 RainbowString[];
extern u8 TrickRoomEndString[];
extern u8 WonderRoomEndString[];
extern u8 MagicRoomEndString[];
extern u8 GravityEndString[];
extern u8 TrickRoomSetString[];
extern u8 WonderRoomSetString[];
extern u8 MagicRoomSetString[];
extern u8 GravitySetString[];
extern u8 IonDelugeShowerString[];
extern u8 NoMoreAirborneFromGravityString[];
extern u8 Bank0AWasBroughtDownString[];
extern u8 TargetTransformedIntoType[];
extern u8 ThirdTypeAddedString[];
extern u8 FairyLockString[];
extern u8 HappyHourString[];
extern u8 CelebrateString[];
extern u8 HoldHandsString[];
extern u8 TailwindSetString[];
extern u8 LuckyChantSetString[];
extern u8 WorrySeedString[];
extern u8 AbilitySuppressedString[];
extern u8 EntrainmentString[];
extern u8 SimpleBeamString[];
extern u8 gText_SetAuroraVeil[];
extern u8 FreedFromSkyDropString[];
extern u8 StringEnduredHitWithSturdy[];
extern u8 PrimalRainEndString[];
extern u8 PrimalSunEndString[];
extern u8 PrimalAirCurrentEndString[];
extern u8 RemovedEntryHazardsString[];
extern u8 RemovedEntryHazardsTargetSideString[];
extern u8 SpikesLayString[];
extern u8 StealthRockLayString[];
extern u8 ToxicSpikesLayString[];
extern u8 StickyWebLayString[];
extern u8 CraftyShieldSetString[];
extern u8 MatBlockSetString[];
extern u8 QuickGuardSetString[];
extern u8 WideGuardSetString[];
extern u8 gText_ScreenRaisedStat[];
extern u8 RemovedEntryHazardsString[];
extern u8 SpikesLayString[];
extern u8 StealthRockLayString[];
extern u8 ToxicSpikesLayString[];
extern u8 StickyWebLayString[];

extern u8 ElectricTerrainSetString[];
extern u8 GrassyTerrainSetString[];
extern u8 MistyTerrainSetString[];
extern u8 PsychicTerrainSetString[];

#define gText_TargetAlreadyAsleep (u8*) 0x83FB57C
extern u8 gText_TeamProtectedBySafeguard[];
extern u8 gText_TargetAlreadyHasStatusCondition[];
extern u8 gText_TargetWrappedInMistyTerrain[];
extern u8 gText_TargetWrappedInElectricTerrain[];
extern u8 gText_FlowerVeilProtects[];
extern u8 gText_SweetVeilProtects[];
extern u8 gText_SleepClausePrevents[];
#define gText_CantFallAsleepDuringUproar (u8*) 0x83FBDC4

#define gText_TargetAlreadyParalyzed (u8*) 0x83FB78A
#define gText_TargetAlreadyBurned (u8*) 0x83FB67B
#define gText_TargetAlreadyPoisoned (u8*) 0x83FB5F8

extern u8 IonDelugeShowerString[];
extern u8 TargetStatsResetString[];
extern u8 TerrainEndString[];
extern u8 RoastedBerryString[];
extern u8 gText_TargetWasInfested[];

extern u8 BattleScript_HealingWishHeal[];
extern u8 BattleScript_LunarDanceHeal[];
extern u8 BattleScript_EntryHazardsHurtReturn[];
extern u8 BattleScript_SpikesHurt[];
extern u8 BattleScript_SRHurt[];
extern u8 BattleScript_TSPoison[];
extern u8 BattleScript_TSHarshPoison[];
extern u8 BattleScript_TSAbsorb[];
extern u8 BattleScript_StickyWebSpeedDrop[];
extern u8 BattleScript_SuccessForceOut[];
extern u8 BattleScript_TrainerSlideMsgRet[];