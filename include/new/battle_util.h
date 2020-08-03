#pragma once

#include "../global.h"
#include "../pokemon.h"

/**
 * \file battle_util.h
 * \brief Contains battle utility functions.
 */

//Exported Functions
u8 GetBankForBattleScript(u8 caseId);
ability_t GetBankAbility(u8 bank);
ability_t GetRecordedAbility(u8 bank);
ability_t CopyAbility(u8 bank);
ability_t* GetAbilityLocation(u8 bank);
ability_t* GetAbilityLocationIgnoreNeutralizingGas(u8 bank);
void RecordAbilityBattle(u8 bank, u8 ability);
void ClearBattlerAbilityHistory(u8 bank);
item_effect_t GetBankItemEffect(u8 bank);
item_effect_t GetMonItemEffect(struct Pokemon* mon);
item_effect_t GetRecordedItemEffect(u8 bank);
void RecordItemEffectBattle(u8 bank, u8 itemEffect);
void ClearBattlerItemEffectHistory(u8 bank);
struct Pokemon* GetBankPartyData(u8 bank);
u16 GetBaseCurrentHP(u8 bank);
u16 GetBaseMaxHP(u8 bank);
u8 GetBankFromPartyData(struct Pokemon* mon);
bool8 CanHitSemiInvulnerableTarget(u8 bankAtk, u8 bankDef, u16 move);
bool8 CheckGrounding(u8 bank);
bool8 NonInvasiveCheckGrounding(u8 bank);
bool8 CheckGroundingFromPartyData(struct Pokemon* mon);
u8 ViableMonCountFromBank(u8 bank);
u8 ViableMonCountFromBankLoadPartyRange(u8 bank);
bool8 CheckContact(u16 move, u8 bank);
bool8 CheckContactByMon(u16 move, struct Pokemon* mon);
bool8 CheckHealingMove(u16 move);
bool8 CheckSoundMove(u16 move);
bool8 SheerForceCheck(void);
bool8 IsOfType(u8 bank, u8 type);
bool8 LiftProtect(u8 bank);
bool8 ProtectsAgainstZMoves(u16 move, u8 bankAtk, u8 bankDef);
bool8 StatsMaxed(u8 bank);
bool8 MainStatsMaxed(u8 bank);
bool8 StatsMinned(u8 bank);
bool8 MainStatsMinned(u8 bank);
bool8 AnyStatGreaterThan(u8 bank, u8 amount);
u8 CountBoosts(u8 bank);
u8 CheckMoveLimitations(u8 bank, u8 unusableMoves, u8 check);
bool8 IsUnusableMove(u16 move, u8 bank, u8 check, u8 pp, u8 ability, u8 holdEffect, u16 choicedMove);
u8 CheckMoveLimitationsFromParty(struct Pokemon* mon, u8 unusableMoves, u8 check);
void CancelMultiTurnMoves(u8 bank);
bool8 IsMoveRedirectedByFollowMe(u16 move, u8 bankAtk, u8 defSide);
bool8 IsMoveRedirectionPrevented(u16 move, u8 atkAbility);
u8 GetMoveTarget(u16 move, u8 useMoveTarget);
bool8 IsBattlerAlive(u8 bank);
struct Pokemon* LoadPartyRange(u8 bank, u8* firstMonId, u8* lastMonId);
bool8 UproarWakeUpCheck(unusedArg u8 bank);
bool8 IsUproarBeingMade(void);
u8 GetIllusionPartyNumber(u8 bank);
struct Pokemon* GetIllusionPartyData(u8 bank);
bool8 BankMovedBefore(u8 bank1, u8 bank2);
bool8 BankMovedBeforeIgnoreSwitch(u8 bank1, u8 bank2);
bool8 IsFirstAttacker(u8 bank);
bool8 CanTransferItem(u16 species, u16 item);
bool8 CanFling(u16 item, u16 species, u8 ability, u8 bankOnSide, u8 embargoTimer);
bool8 SymbiosisCanActivate(u8 giverBank, u8 receiverBank);
bool8 CanKnockOffItem(u8 bank);
bool8 CanKnockOffMonItem(struct Pokemon* mon, u8 side);
bool8 IsAffectedByPowder(u8 bank);
bool8 IsAffectedByPowderByDetails(u8 type1, u8 type2, u8 type3, u8 ability, u8 itemEffect);
bool8 MoveIgnoresSubstitutes(u16 move, u8 atkAbility);
bool8 MoveBlockedBySubstitute(u16 move, u8 bankAtk, u8 bankDef);
bool8 MonMoveBlockedBySubstitute(u16 move, struct Pokemon* monAtk, u8 bankDef);
bool8 IsMockBattle(void);
bool8 IsMoveAffectedByParentalBond(u16 move, u8 bankAtk);
u8 CalcMoveSplit(u8 bank, u16 move);
u8 CalcMoveSplitFromParty(struct Pokemon* mon, u16 move);
u8 FindMovePositionInMoveset(u16 move, u8 bank);
bool8 MoveInMoveset(u16 move, u8 bank);
u8 AttacksThisTurn(u8 bank, u16 move);
bool8 IsZMove(const u16 move);
void ResetVarsForAbilityChange(u8 bank);
void HandleUnburdenBoost(u8 bank);
void AddBankToPickupStack(const u8 bank);
void RemoveBankFromPickupStack(const u8 bank);
u8 GetTopOfPickupStackNotIncludingBank(const u8 bank);
void RemoveScreensFromSide(const u8 side);

void ClearBankStatus(u8 bank);
bool8 DoesSleepClausePrevent(u8 bank);
bool8 CanBeGeneralStatused(u8 bank, bool8 checkFlowerVeil);
bool8 CanBePutToSleep(u8 bank, bool8 checkFlowerVeil);
bool8 CanBeYawned(u8 bank);
bool8 CanBePoisoned(u8 bankDef, u8 bankAtk, bool8 checkFlowerVeil);
bool8 CanBeParalyzed(u8 bank, bool8 checkFlowerVeil);
bool8 CanBeBurned(u8 bank, bool8 checkFlowerVeil);
bool8 CanBeFrozen(u8 bank, bool8 checkFlowerVeil);
bool8 CanBeConfused(u8 bank, u8 checkSafeguard);
bool8 CanBeTormented(u8 bank);
bool8 CanBeInfatuated(u8 bankDef, u8 bankAtk);

bool8 IsTrickRoomActive(void);
bool8 IsMagicRoomActive(void);
bool8 IsWonderRoomActive(void);
bool8 IsGravityActive(void);
bool8 IsIonDelugeActive(void);
bool8 IsFairyLockActive(void);
bool8 IsMudSportActive(void);
bool8 IsWaterSportActive(void);
bool8 IsInverseBattle(void);
bool8 BankSideHasSafeguard(u8 bank);
bool8 BankSideHasMist(u8 bank);
bool8 BankSideHasSeaOfFire(u8 bank);
bool8 BankSideHasRainbow(u8 bank);
bool8 BankSideHasSwamp(u8 bank);
bool8 SideHasSwamp(u8 side);
bool8 BankSideHasGMaxVineLash(u8 bank);
bool8 BankSideHasGMaxWildfire(u8 bank);
bool8 BankSideHasGMaxCannonade(u8 bank);
bool8 BankSideHasGMaxVolcalith(u8 bank);
bool8 IsConfused(u8 bank);
bool8 IsTaunted(u8 bank);
bool8 IsTormented(u8 bank);
bool8 IsHealBlocked(u8 bank);
bool8 CantUseSoundMoves(u8 bank);
bool8 IsLaserFocused(u8 bank);
bool8 IsAbilitySuppressed(u8 bank);
bool8 CantScoreACrit(u8 bank, struct Pokemon* mon);

u8 AbilityBattleEffects(u8 caseID, u8 bank, u8 ability, u8 special, u16 moveArg);
u8 ItemBattleEffects(u8 caseID, u8 bank, bool8 moveTurn, bool8 DoPluck);
void EmitDataTransfer(u8 bufferId, void* dst, u16 size, void* data);

//Functions Hooked In
void ClearTemporarySpeciesSpriteData(u8 bank, bool8 dontClearSubstitute);
u16 TryFixDynamaxTransformSpecies(u8 bank, u16 species);

//Exported Constants
enum {IN_AIR, GROUNDED};

enum ItemBattleEffectCases
{
	ItemEffects_SwitchIn,
	ItemEffects_EndTurn,
	ItemEffects_ContactTarget,
	ItemEffects_ContactAttacker
};

#define MOVE_LIMITATION_ZEROMOVE    (1 << 0)
#define MOVE_LIMITATION_PP          (1 << 1)
#define MOVE_LIMITATION_DISABLED    (1 << 2)
#define MOVE_LIMITATION_TORMENTED   (1 << 3)
#define MOVE_LIMITATION_TAUNT       (1 << 4)
#define MOVE_LIMITATION_IMPRISION   (1 << 5)
#define MOVE_LIMITATION_CHOICE		(1 << 6)
#define MOVE_LIMITATION_ENCORE		(1 << 7)
