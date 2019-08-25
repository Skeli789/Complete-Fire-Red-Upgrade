#pragma once
#include "../../src/defines.h"
#include "../../src/defines_battle.h"

/**
 * \file helper_functions.h
 * \brief Contains many utility functions that mostly aid in battle, but can also
 *		  help in the overworld.
 */
 
//Exported Functions
bool8 CheckTableForMove(move_t, const u16 table[]);
bool8 CheckTableForMoveEffect(move_t move, const u8 table[]);
bool8 CheckTableForSpecialMoveEffect(u8 effect, const u8 table[]);
bool8 CheckTableForAbility(ability_t, const u8 table[]);
bool8 CheckTableForSpecies(u16 species, const u16 table[]);
bool8 CheckTableForItem(u16 item, const u16 table[]);
ability_t BanksAbility(bank_t);
ability_t GetRecordedAbility(u8 bank);
void RecordAbilityBattle(u8 battlerId, u8 abilityId);
void ClearBattlerAbilityHistory(u8 battlerId);
bank_t GetPartnerBank(bank_t);
bank_t GetFoeBank(bank_t);
item_effect_t GetBankItemEffect(bank_t);
item_effect_t GetRecordedItemEffect(u8 bank);
item_effect_t GetMonItemEffect(struct Pokemon* mon);
void RecordItemEffectBattle(u8 battlerId, u8 itemEffect);
void ClearBattlerItemEffectHistory(u8 battlerId);
bool8 CheckAbilityTargetField(ability_t);
pokemon_t* GetBankPartyData(bank_t);
bank_t GetBankFromPartyData(pokemon_t* mon);
bool8 CheckGrounding(bank_t);
bool8 NonInvasiveCheckGrounding(u8 bank);
bool8 CheckGroundingFromPartyData(pokemon_t* party);
bool8 PartyAlive (bank_t);
u8 ViableMonCount(pokemon_t* partydata);
u8 ViableMonCountFromBank(u8 bank);
u8 ViableMonCountFromBankLoadPartyRange(u8 bank);
bool8 CheckContact(move_t, bank_t);
bool8 CheckContactMove(move_t);
bool8 CheckHealingMove(move_t);
bool8 CheckSoundMove(move_t);
bool8 CheckCurrentMoveSoundMove(void);
bool8 SheerForceCheck(void);
bool8 IsOfType(bank_t, u8 type);
bool8 LiftProtect(bank_t);
bool8 ProtectsAgainstZMoves(u16 move, u8 bankAtk, u8 bankDef);
bool8 StatsMaxed(bank_t);
bool8 MainStatsMaxed(bank_t bank);
bool8 StatsMinned(bank_t);
bool8 AnyStatGreaterThan(bank_t, u8 amount);
u32 MathMax (u32, u32);
u32 MathMin (u32, u32);

u8 GetMoveTarget(u16 move, u8 useMoveTarget);
void HealMon(pokemon_t* poke);
bool8 IsBattlerAlive(u8 bank);
u16 GetNationalPokedexCount(u8 caseID);
bool8 CheckBattleTerrain(u8 caseID);
pokemon_t* LoadPartyRange(u8 bank, u8* FirstMonId, u8* lastMonId);
ability_t CopyAbility(u8 bank);
ability_t* GetAbilityLocation(u8 bank);
bool8 UproarWakeUpCheck(u8 bank);
bool8 IsUproarBeingMade(void);
u8 GetIllusionPartyNumber(u8 bank);
pokemon_t* GetIllusionPartyData(u8 bank);
bool8 BankMovedBefore(u8 bank1, u8 bank2);
bool8 IsFirstAttacker(u8 bank);
bool8 CanFling(ability_t, item_t, pokemon_t*, bank_t, bool8 PartyCheck);
bool8 SymbiosisCanActivate(u8 bank_to_give, u8 bank_to_receive);
bool8 CanTransferItem(species_t, item_t, pokemon_t*);
bool8 CanKnockOffItem(bank_t);
bool8 CanEvolve(pokemon_t*);
bool8 CouldHaveEvolvedViaLevelUp(pokemon_t* mon);
bool8 IsAffectedByPowder(u8 bank);
bool8 IsAffectedByPowderByDetails(u8 type1, u8 type2, u8 type3, u8 ability, u8 itemEffect);
u8 CountBoosts(bank_t);
bool8 MoveIgnoresSubstitutes(u16 move, u8 bankAtk);
bool8 MoveBlockedBySubstitute(u16 move, u8 atkBank, u8 defBank);
bool8 IsMockBattle(void);
u8 CalcMoveSplit(u8 bank, u16 move);
u8 CalcMoveSplitFromParty(pokemon_t* mon, u16 move);
u8 FindMovePositionInMoveset(u16 move, u8 bank);
bool8 MoveInMoveset(u16 move, u8 bank);
u8 FindMovePositionInMonMoveset(u16 move, struct Pokemon* mon);
bool8 MoveInMonMoveset(u16 move, struct Pokemon* mon);
bool8 AllHittingMoveWithTypeInMonMoveset(struct Pokemon* mon, u8 moveType);
u8 AttacksThisTurn(u8 bank, u16 move);
bool8 IsZMove(const u16 move);
void AddBankToPickupStack(const u8 bank);
void RemoveBankFromPickupStack(const u8 bank);
u8 GetTopOfPickupStackNotIncludingBank(const u8 bank);
bool8 IsTrickRoomActive(void);
void ClearBankStatus(bank_t bank);

bool8 DoesSleepClausePrevent(u8 bank);
bool8 CanBeGeneralStatused(u8 bank, bool8 checkFlowerVeil);
bool8 CanBePutToSleep(u8 bank, bool8 checkFlowerVeil);
bool8 CanBePoisoned(u8 bankDef, u8 bankAtk, bool8 checkFlowerVeil);
bool8 CanBeParalyzed(u8 bank, bool8 checkFlowerVeil);
bool8 CanBeBurned(u8 bank, bool8 checkFlowerVeil);
bool8 CanBeFrozen(u8 bank, bool8 checkFlowerVeil);
bool8 CanBeConfused(u8 bank);
bool8 CanPartyMonBeGeneralStatused(pokemon_t* mon);
bool8 CanPartyMonBePutToSleep(pokemon_t* mon);
bool8 CanPartyMonBePoisoned(pokemon_t* mon);
bool8 CanPartyMonBeParalyzed(pokemon_t* mon);
bool8 CanPartyMonBeBurned(pokemon_t* mon);
bool8 CanPartyMonBeFrozen(pokemon_t* mon);

u8 AbilityBattleEffects(u8 caseID, u8 bank, u8 ability, u8 special, u16 moveArg);
u8 ItemBattleEffects(u8 caseID, u8 bank, bool8 moveTurn, bool8 DoPluck);
void EmitDataTransfer(u8 bufferId, void* dst, u16 size, void* data);

//Exported Constants
enum ItemBattleEffectCases
{
	ItemEffects_SwitchIn,
	ItemEffects_EndTurn,
	ItemEffects_ContactTarget,
	ItemEffects_ContactAttacker
};

enum {IN_AIR, GROUNDED};
