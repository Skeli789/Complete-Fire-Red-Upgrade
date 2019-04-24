#pragma once
#include "../../src/defines.h"
#include "../../src/defines_battle.h"

extern bool8 CheckTableForMove(move_t, const u16 table[]);
extern bool8 CheckTableForMoveEffect(move_t move, const u8 table[]);
extern bool8 CheckTableForSpecialMoveEffect(u8 effect, const u8 table[]);
extern bool8 CheckTableForAbility(ability_t, const u8 table[]);
extern ability_t BanksAbility(bank_t);
extern bank_t GetPartnerBank(bank_t);
extern bank_t GetFoeBank(bank_t);
extern item_effect_t GetBankItemEffect(bank_t);
extern bool8 CheckAbilityTargetField(ability_t);
extern pokemon_t* GetBankPartyData(bank_t);
extern bank_t GetBankFromPartyData(pokemon_t* mon);
extern bool8 CheckGrounding(bank_t);
extern bool8 CheckGroundingFromPartyData(pokemon_t* party);
extern bool8 PartyAlive (bank_t);
extern u8 ViableMonCount(pokemon_t* partydata);
extern u8 ViableMonCountFromBank(u8 bank);
extern bool8 CheckContact(move_t, bank_t);
extern bool8 CheckContactMove(move_t);
extern bool8 CheckHealingMove(move_t);
extern bool8 CheckSoundMove(move_t);
extern bool8 CheckCurrentMoveSoundMove(void);
extern bool8 SheerForceCheck(void);
extern bool8 IsOfType(bank_t, u8 type);
extern bool8 LiftProtect(bank_t);
extern bool8 ProtectsAgainstZMoves(u16 move, u8 bankAtk, u8 bankDef);
extern bool8 StatsMaxed(bank_t);
extern bool8 StatsMinned(bank_t);
extern u32 MathMax (u32, u32);
extern u32 MathMin (u32, u32);

extern u8 GetMoveTarget(u16 move, u8 useMoveTarget);
extern void HealMon(pokemon_t* poke);
extern bool8 IsBattlerAlive(u8 bank);
extern u16 GetNationalPokedexCount(u8 caseID);
extern bool8 CheckBattleTerrain(u8 caseID);
extern pokemon_t* LoadPartyRange(u8 bank, u8* FirstMonId, u8* lastMonId);
extern ability_t CopyAbility(u8 bank);
extern ability_t* GetAbilityLocation(u8 bank);
extern bool8 UproarWakeUpCheck(u8 bank);
extern bool8 IsUproarBeingMade(void);
extern void DoFormChange(u8 bank, u16 species, bool8 ReloadType, bool8 ReloadStats);
extern pokemon_t* GetIllusionPartyData(u8 bank);
extern bool8 BankMovedBefore(u8 bank1, u8 bank2);
extern bool8 IsFirstAttacker(u8 bank);
extern bool8 CanFling(ability_t, item_t, pokemon_t*, bank_t, bool8 PartyCheck);
extern bool8 SymbiosisCanActivate(u8 bank_to_give, u8 bank_to_receive);
extern bool8 CanTransferItem(species_t, item_t, pokemon_t*);
extern bool8 CanKnockOffItem(bank_t);
extern bool8 CanEvolve(pokemon_t*);
extern bool8 CouldHaveEvolvedViaLevelUp(pokemon_t* mon);
extern u8 CountBoosts(bank_t);
extern bool8 MoveBlockedBySubstitute(u16 move, u8 atkBank, u8 defBank);
extern bool8 IsMockBattle(void);
extern u8 CalcMoveSplit(u8 bank, u16 move);
extern u8 CalcMoveSplitFromParty(pokemon_t* mon, u16 move);
extern u8 FindMovePositionInMoveset(u16 move, u8 bank);
extern u8 AttacksThisTurn(u8 bank, u16 move);
extern bool8 IsZMove(const u16 move);
extern void AddBankToPickupStack(const u8 bank);
extern void RemoveBankFromPickupStack(const u8 bank);
extern u8 GetTopOfPickupStackNotIncludingBank(const u8 bank);
extern void ClearBankStatus(bank_t bank);

extern bool8 CanBeGeneralStatused(u8 bank);
extern bool8 CanBePutToSleep(u8 bank);
extern bool8 CanBePoisoned(u8 bankDef, u8 bankAtk);
extern bool8 CanBeParalyzed(u8 bank);
extern bool8 CanBeBurned(u8 bank);
extern bool8 CanBeFrozen(u8 bank);
extern bool8 CanBeConfused(u8 bank);
extern bool8 CanPartyMonBeGeneralStatused(pokemon_t* mon);
extern bool8 CanPartyMonBePutToSleep(pokemon_t* mon);
extern bool8 CanPartyMonBePoisoned(pokemon_t* mon);
extern bool8 CanPartyMonBeParalyzed(pokemon_t* mon);
extern bool8 CanPartyMonBeBurned(pokemon_t* mon);
extern bool8 CanPartyMonBeFrozen(pokemon_t* mon);

extern u8 AbilityBattleEffects(u8 caseID, u8 bank, u8 ability, u8 special, u16 moveArg);
extern u8 ItemBattleEffects(u8 caseID, u8 bank, bool8 moveTurn, bool8 DoPluck);
extern void EmitDataTransfer(u8 bufferId, void* dst, u16 size, void* data);

enum ItemBattleEffectCases {
ItemEffects_SwitchIn,
ItemEffects_EndTurn,
ItemEffects_ContactTarget,
ItemEffects_ContactAttacker
};
