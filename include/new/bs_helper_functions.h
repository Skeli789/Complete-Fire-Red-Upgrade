#pragma once

#include "../global.h"

/**
 * \file bs_helper_functions.h
 * \brief Contains functions which help out with battle scripting and are generally
 *		  called with the battle scripting "callasm" command.
 */
 
//Exported Functions
void CheckIfDarkVoidShouldFail(void);
void SetTargetPartner(void);
bool8 CheckCraftyShield(u8 bank);
void LiftProtectTarget(void);
void IncreaseNimbleCounter(void);
void MagnetFluxLooper(void);
void MagnetFluxLooper(void);
void ModifyGrowthInSun(void);
void AcupressureFunc(void);
void SetStatSwapSplit(void);
void ResetTargetStats(void);
void ToggleSpectralThiefByte(void);
void CheeckPouchFunc(void);
void SetUnburdenBoostTarget(void);
void MoldBreakerRemoveAbilitiesOnForceSwitchIn(void);
void MoldBreakerRestoreAbilitiesOnForceSwitchIn(void);
void TrainerSlideOut(void);
void SetAuroraVeil(void);
void MetalBurstDamageCalculator(void);
void TryActivatePartnerSapSipper(void);
void RoundBSFunction(void);
void EchoedVoiceFunc(void);
void DefogHelperFunc(void);
void ClearBeakBlastBit(void);
void BelchFunction(void);
void StrengthSapFunc(void);
void PlayAttackAnimationForExplosion(void);
void CopycatFunc(void);
void SetRoostFunc(void);
void CaptivateFunc(void);
void MeFirstFunc(void);
void LoadPledgeScript(void);
void SetPledgeEffect(void);
void DoFieldEffect(void);
void BringDownMons(void);
void TryFling(void);
void CheckTelekinesisFail(void);
void ChangeTargetTypeFunc(void);
void HealTargetFunc(void);
void TopsyTurvyFunc(void);
void DoFairyLockHappyHourFunc(void);
bool8 TargetMovesRightAfterAttacker(void);
void AfterYouFunc(void);
void QuashFunc(void);
void TryExecuteInstruct(void);
void InitiateInstruct(void);
void TrySetMagnetRise(void);
void TailwindLuckyChantFunc(void);
void FlameBurstFunc(void);
void ModifyPostStockpileBoostDecrement(void);
void RemoveStockpileBoosts(void);
void SetHealingWishLunarDanceFunc(void);
void FinalGambitDamageCalc(void);
void AbilityChangeBSFunc(void);
void LoadStatustoPsychoShiftTransfer(void);
void PluckBerryEat(void);
void BurnUpFunc(void);
void SeedLooper(void);
void LastResortFunc(void);
void SynchronoiseFunc(void);
void MakeScriptingBankInvisible(void);
void TransferTerrainData(void);
void TransferIllusionBroken(void);
void CycleScriptingBankHealthBetween0And1(void);
void SetTeleportBit(void);
void TryLaunchShellTrap(void);
void TrySuckerPunch(void);
void DoProteanTypeChange(void);
void HarvestActivateBerry(void);
void TryManipulateDamageForLeechSeedBigRoot(void);
void DisplayForfeitYesNoBox(void);
void HandleForfeitYesNoBox(void);
