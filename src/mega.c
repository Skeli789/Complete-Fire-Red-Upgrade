#include "defines.h"
#include "helper_functions.h"
#include "mega.h"

#define ITEM_MEGA_RING 0x161
#define TRAINER_ITEM_COUNT 4

extern u8 BattleScript_Primal[];
extern u8 BattleScript_PrimalSub[];
extern u8 BattleScript_MegaEvolution[];
extern u8 BattleScript_MegaWish[];
extern u8 BattleScript_UltraBurst[];

const struct Evolution* CanMegaEvolve(u8 bank, bool8 CheckUBInstead);
u8* DoMegaEvolution(u8 bank);
u8* DoPrimalReversion(u8 bank, u8 caseId);
void MegaRevert(void);
bool8 IsItemKeystone(u16 item);
item_t FindTrainerKeystone(u16 trainerId);
item_t FindPlayerKeystone(void);
item_t FindBankKeystone(u8 bank);
bool8 MegaEvolutionEnabled(u8 bank);
bool8 BankMegaEvolved(u8 bank, bool8 checkUB);
u8* GetTrainerName(u8 bank);

item_t KeystoneTable[] = {
    ITEM_MEGA_RING,
};

const struct Evolution* CanMegaEvolve(u8 bank, bool8 CheckUBInstead) {
	const struct Evolution* evolutions = gEvolutionTable[gBattleMons[bank].species];
	int i, j;
	
	for (i = 0; i < EVOS_PER_MON; ++i) {
		if (evolutions[i].method == EVO_MEGA) {
			// Ignore reversion information
			if (evolutions[i].param == 0) continue;
			
			// Check for held item
			if ((!CheckUBInstead && evolutions[i].unknown == MEGA_VARIANT_STANDARD) || (CheckUBInstead && evolutions[i].unknown == MEGA_VARIANT_ULTRA_BURST)) {
				if (evolutions[i].param == gBattleMons[bank].item)
					return &evolutions[i];	
			} 
			else if (evolutions[i].unknown == MEGA_VARIANT_WISH) {
			// Check learned moves
				for (j = 0; j < 4; ++j) {
					if (GetBankPartyData(bank)->moves[j] == evolutions[i].param) //Check party moves b/c Mimic can't activate Dragon Ascent
						return &evolutions[i];
				}
			}
		}
	}
	// NULL
	return 0;
}

u8* DoMegaEvolution(u8 bank) {
	const struct Evolution* evolutions = CanMegaEvolve(bank, FALSE);
	
	if (evolutions == NULL) //Check Ultra Burst if no Mega
		evolutions = CanMegaEvolve(bank, TRUE);
	
	if (evolutions != NULL) {
		u16 species = gBattleMons[bank].species;
		DoFormChange(bank, evolutions->targetSpecies, TRUE);
		gBattleMons[bank].ability = GetPartyAbility(GetBankPartyData(bank));
		gBattleScripting->bank = bank;
		gLastUsedItem = gBattleMons[bank].item;
		
		//FD 00's FD 16 FE is reacting to FD 04's FD 01!
		PREPARE_SPECIES_BUFFER(gBattleTextBuff1, species);
		PREPARE_ITEM_BUFFER(gBattleTextBuff2, FindBankKeystone(bank));
		StringCopy(gStringVar3, GetTrainerName(bank));
		
		if (evolutions->unknown == MEGA_VARIANT_WISH)
			return BattleScript_MegaWish;
		else if (evolutions->unknown == MEGA_VARIANT_ULTRA_BURST)
			return BattleScript_UltraBurst;
		else
			return BattleScript_MegaEvolution;
	}
	else
		return NULL;
}


u8* DoPrimalReversion(u8 bank, u8 caseId) {
	pokemon_t* partydata = GetBankPartyData(bank);
	const struct Evolution* evolutions = gEvolutionTable[partydata->species];

	for (u8 i = 0; i < EVOS_PER_MON; ++i) {
		u16 item = GetMonData(&partydata[i], MON_DATA_HELD_ITEM, 0);
		if (evolutions[i].method == EVO_MEGA && evolutions[i].unknown == MEGA_VARIANT_PRIMAL && evolutions[i].param == item) {
			DoFormChange(bank, evolutions[i].targetSpecies, TRUE);
			gBattleMons[bank].ability = GetPartyAbility(partydata);
			
			switch (caseId) {
				case 0:
					return BattleScript_Primal;
				default:
					return BattleScript_PrimalSub;
			}
		}
	}
	return NULL;
}

void MegaRevert(void) {
}

bool8 IsItemKeystone(u16 item)
{
    for (u8 i = 0; i < sizeof(KeystoneTable) / sizeof(item_t); ++i) {
        if (item == KeystoneTable[i]) {
            return TRUE;
        }
    }
    return FALSE;
}

item_t FindTrainerKeystone(u16 trainerId)
{
	if (gBattleTypeFlags & (BATTLE_TYPE_FRONTIER | BATTLE_TYPE_LINK))
		return ITEM_MEGA_RING;
		
    for (u8 i = 0; i < TRAINER_ITEM_COUNT; ++i) {
        if (IsItemKeystone(gTrainers[trainerId].items[i])) {
            return gTrainers[trainerId].items[i];
        }
    }

    return ITEM_NONE;
}


item_t FindPlayerKeystone(void)
{
    for (u8 i = 0; i < sizeof(KeystoneTable) / sizeof(item_t); ++i) {
        if (CheckBagHasItem(KeystoneTable[i], 1)) {
            return KeystoneTable[i];
        }
    }

    return ITEM_NONE;
}

item_t FindBankKeystone(u8 bank)
{
	if (gBattleTypeFlags & BATTLE_TYPE_LINK)
		return ITEM_MEGA_RING; //You can always Mega Evolve in a link battle
		
	#ifdef DEBUG_MEGA
		if (bank + 1)
			return ITEM_MEGA_RING;
	#endif
		
    if (SIDE(bank) == SIDE_OPPONENT) {
		if (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS) {
			if (GetBattlerPosition(bank) == B_POSITION_OPPONENT_LEFT)
				return FindTrainerKeystone(gTrainerBattleOpponent_A);
			else
				return FindTrainerKeystone(SECOND_OPPONENT);
		}
		else
			return FindTrainerKeystone(gTrainerBattleOpponent_A);
	}
	else {
		if (gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER) {
			if (GetBattlerPosition(bank) == B_POSITION_PLAYER_RIGHT)
				return FindTrainerKeystone(VarGet(PARTNER_VAR));
			else
				return FindPlayerKeystone();
		}
		else
			return FindPlayerKeystone();
	}
}

bool8 MegaEvolutionEnabled(u8 bank) {
	if (FindBankKeystone(bank) == ITEM_NONE) {
	#ifdef DEBUG_MEGA
		return TRUE;
	#else
		return FALSE;
	#endif
	}
	else
		return TRUE;
}

bool8 BankMegaEvolved(u8 bank, bool8 checkUB) {
	if ((SIDE(bank) == B_SIDE_PLAYER && gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER)
	||  (SIDE(bank) == B_SIDE_OPPONENT && gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS))
	{
		if (checkUB) {
			if (gNewBS->UltraData->done[bank])
				return TRUE;
			else
				return FALSE;
		}
		else {	
			if (gNewBS->MegaData->done[bank])
				return TRUE;
			else
				return FALSE;
		}
	}
	
	if (checkUB) {
		if (gNewBS->UltraData->done[bank] || gNewBS->UltraData->done[PARTNER(bank)])
			return TRUE;
	}
	else {
		if (gNewBS->MegaData->done[bank] || (gNewBS->MegaData->done[PARTNER(bank)]))
			return TRUE;
	}
	
	return FALSE;
}

u8* GetTrainerName(u8 bank) {
	u16 trainerId = 0xFFFF;
	u8 multiplayerId = GetMultiplayerId();
	
	u16 linkPartner = GetBattlerMultiplayerId(BATTLE_PARTNER(gLinkPlayers[multiplayerId].id));
	u16 linkOpponent1 = GetBattlerMultiplayerId(BATTLE_OPPOSITE(gLinkPlayers[multiplayerId].id));
	u16 linkOpponent2 = GetBattlerMultiplayerId(BATTLE_PARTNER(BATTLE_OPPOSITE(gLinkPlayers[multiplayerId].id)));
	
	switch (GetBattlerPosition(bank)) {
		case B_POSITION_OPPONENT_LEFT:
			if (gBattleTypeFlags & BATTLE_TYPE_LINK)
				trainerId = linkOpponent1;
			else
				trainerId = gTrainerBattleOpponent_A;
			break;
		
		case B_POSITION_PLAYER_RIGHT:
			if (gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER)
				trainerId = VarGet(PARTNER_VAR);
			else if (gBattleTypeFlags & BATTLE_TYPE_LINK && gBattleTypeFlags & BATTLE_TYPE_MULTI)
				trainerId = linkPartner;
			break;
		
		case B_POSITION_OPPONENT_RIGHT:
			if (gBattleTypeFlags & BATTLE_TYPE_LINK && gBattleTypeFlags & BATTLE_TYPE_MULTI)
				trainerId = linkOpponent2;
			else if (gBattleTypeFlags & BATTLE_TYPE_LINK)
				trainerId = linkOpponent1;
			else if (!(gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS))
				trainerId = gTrainerBattleOpponent_A;
			else
				trainerId = SECOND_OPPONENT;
			break;
	}
	
	if (trainerId == 0xFFFF) {
		if (gBattleTypeFlags & BATTLE_TYPE_LINK)
			return gLinkPlayers[multiplayerId].name;
		else
			return gSaveBlock2->playerName;
	}
	
	else if (gBattleTypeFlags & BATTLE_TYPE_LINK)
		return gLinkPlayers[trainerId].name;
		
	else {
	#ifdef UNBOUND
		u8 class = gTrainers[trainerId].trainerClass;
		if (class == 0x51 || class == 0x59)
			return GetExpandedPlaceholder(ExpandPlaceholder_RivalName);
		else
			return gTrainers[trainerId].trainerName;
	#elif defined OVERWRITE_RIVAL
		u8 class = gTrainers[trainerId].trainerClass;
		if (class == 0x51 || class == 0x59 || class == 0x5A)
			return GetExpandedPlaceholder(ExpandPlaceholder_RivalName);
		else
			return gTrainers[trainerId].trainerName;
	#else
		return gTrainers[trainerId].trainerName;
	#endif
	}
}

void MegaRetrieveData(void) {
	gNewBS->MegaData->chosen[gActiveBattler] |= gBattleBufferB[gActiveBattler][4];
	gNewBS->UltraData->chosen[gActiveBattler] |= gBattleBufferB[gActiveBattler][5];
	gNewBS->ZMoveData->toBeUsed[gActiveBattler] |= gBattleBufferB[gActiveBattler][6];
}