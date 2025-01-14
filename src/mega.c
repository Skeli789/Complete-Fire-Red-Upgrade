#include "defines.h"
#include "defines_battle.h"
#include "../include/link.h"
#include "../include/string_util.h"
#include "../include/constants/items.h"
#include "../include/constants/trainer_classes.h"

#include "../include/new/ability_tables.h"
#include "../include/new/battle_util.h"
#include "../include/new/build_pokemon.h"
#include "../include/new/util.h"
#include "../include/new/form_change.h"
#include "../include/new/frontier.h"
#include "../include/new/item.h"
#include "../include/new/mega.h"
#include "../include/new/mega_battle_scripts.h"
#include "../include/new/move_menu.h"
#include "../include/new/set_z_effect.h"

/*
mega.c
	functions that support mega evolution logic and execution
*/

#define TRAINER_ITEM_COUNT 4

//This file's functions:
static bool8 IsItemKeystone(u16 item);
static item_t FindTrainerKeystone(u16 trainerId);
static item_t FindPlayerKeystone(void);
static item_t FindBankKeystone(u8 bank);

static const item_t sKeystoneTable[] =
{
	ITEM_MEGA_RING,

#ifdef UNBOUND
	ITEM_MEGA_BRACELET,
	ITEM_MEGA_CUFF,
	ITEM_MEGA_CHARM,
	ITEM_MEGA_BONNET,
	ITEM_MEGA_EARING,
	ITEM_MEGA_ANKLET,
	ITEM_MEGA_PENDANT,
#endif
};

const struct Evolution* CanMegaEvolve(unusedArg u8 bank, unusedArg bool8 CheckUBInstead)
{
	#ifndef MEGA_EVOLUTION_FEATURE
		return NULL;
	#else

	if (!CheckUBInstead && !MegaEvolutionEnabled(bank)) //Ultra Burst doesn't need Mega Ring
		return NULL;

	if (IsMegaZMoveBannedBattle()
	|| (!(gBattleTypeFlags & BATTLE_TYPE_TRAINER) && SIDE(bank) == B_SIDE_OPPONENT)) //Wild mons (like Rayquaza) can't Mega Evolve
		return NULL;

	pokemon_t* mon = GetBankPartyData(bank);
	const struct Evolution* evolutions = gEvolutionTable[mon->species];
	int i, j;

	for (i = 0; i < EVOS_PER_MON; ++i)
	{
		if (evolutions[i].method == EVO_NONE) //Most likely end of entries
			break; //Break now to save time
		else if (evolutions[i].method == EVO_MEGA)
		{
			//Ignore reversion information
			if (evolutions[i].param == 0) continue;

			//Check for held item
			if ((!CheckUBInstead && evolutions[i].unknown == MEGA_VARIANT_STANDARD) || (CheckUBInstead && evolutions[i].unknown == MEGA_VARIANT_ULTRA_BURST))
			{
				if (evolutions[i].param == mon->item)
					return &evolutions[i];
			}
			else if (evolutions[i].unknown == MEGA_VARIANT_WISH && !CheckUBInstead
			&& !IsZCrystal(mon->item)) //If Mega Rayquaza holds a Z-Crystal it can't Mega Evolve
			{
				if (!(gBattleTypeFlags & BATTLE_TYPE_FRONTIER)
				|| RayquazaCanMegaEvolveInFrontierBattle())
				{
					//Check learned moves
					for (j = 0; j < MAX_MON_MOVES; ++j)
					{
						if (evolutions[i].param == mon->moves[j])
							return &evolutions[i];
					}
				}
			}
		}
	}

	return NULL;
	#endif
}

//Assumes Wish Evolution isn't important
species_t GetMegaSpecies(unusedArg u16 species, unusedArg u16 item, unusedArg const u16* moves)
{
	#ifndef MEGA_EVOLUTION_FEATURE
		return SPECIES_NONE;
	#else

	const struct Evolution* evolutions = gEvolutionTable[species];
	u32 i, j;

	for (i = 0; i < EVOS_PER_MON; ++i)
	{
		if (evolutions[i].method == EVO_NONE) //Most likely end of entries
			break; //Break now to save time
		else if (evolutions[i].method == EVO_MEGA)
		{
			//Ignore reversion information
			if (evolutions[i].param == 0) continue;

			//Check for held item
			if (evolutions[i].unknown == MEGA_VARIANT_STANDARD || evolutions[i].unknown == MEGA_VARIANT_ULTRA_BURST)
			{
				if (evolutions[i].param == item)
					return evolutions[i].targetSpecies;
			}
			else if (evolutions[i].unknown == MEGA_VARIANT_WISH && moves != NULL)
			{
				for (j = 0; j < MAX_MON_MOVES; ++j)
				{
					if (evolutions[i].param == moves[j])
						return evolutions[i].targetSpecies;
				}
			}
		}
	}

	return SPECIES_NONE;
	#endif
}

species_t GetPrimalSpecies(unusedArg u16 species, unusedArg u16 item)
{
	#ifndef MEGA_EVOLUTION_FEATURE
		return SPECIES_NONE;
	#else

	u32 i;
	const struct Evolution* evolutions = gEvolutionTable[species];

	for (i = 0; i < EVOS_PER_MON; ++i)
	{
		if (evolutions[i].method == EVO_NONE) //Most likely end of entries
			break; //Break now to save time
		else if ((evolutions[i].method == MEGA_EVOLUTION && evolutions[i].unknown == MEGA_VARIANT_PRIMAL && evolutions[i].param == item)) //Can Primal Evolve
			return evolutions[i].targetSpecies;
	}

	return SPECIES_NONE;
	#endif
}

ability_t GetBankMegaFormAbility(u8 megaBank, u8 foe)
{
	u8 ability = ABILITY_NONE;
	const struct Evolution* evos;

	if (!IsAbilitySuppressed(megaBank))
	{
		evos = CanMegaEvolve(megaBank, FALSE);
		if (evos != NULL)
			ability = GetAbility1(evos->targetSpecies); //Megas can only have 1 ability
		else
		{
			//Check Ultra Burst
			evos = CanMegaEvolve(megaBank, TRUE);
			if (evos != NULL)
				ability = GetAbility1(evos->targetSpecies); //Ultra Necrozma only has 1 ability
		}

		if (ability == ABILITY_TRACE && IS_SINGLE_BATTLE)
		{
			u8 foeAbility = *GetAbilityLocation(foe);
			if (!gSpecialAbilityFlags[foeAbility].gTraceBannedAbilities)
				ability = foeAbility; //What the Ability will become
		}
	}

	return ability;
}

const u8* DoMegaEvolution(u8 bank)
{
	struct Pokemon* mon = GetBankPartyData(bank);
	const struct Evolution* evolutions = CanMegaEvolve(bank, FALSE);

	if (evolutions == NULL) //Check Ultra Burst if no Mega
		evolutions = CanMegaEvolve(bank, TRUE);

	if (evolutions != NULL)
	{
		gNewBS->backupAbility = ABILITY(bank); //So Abilities like Delta Stream & Neutralizing Gas are removed properly

		DoFormChange(bank, evolutions->targetSpecies, TRUE, TRUE, TRUE);

		gBattleScripting.bank = bank;
		gLastUsedItem = mon->item;

		//[BUFFER][00]'s [LAST_ITEM]\nis reacting to [PLAYER_NAME]'s [BUFFER][01]!
		PREPARE_MON_NICK_BUFFER(gBattleTextBuff1, bank, gBattlerPartyIndexes[bank]);
		PREPARE_ITEM_BUFFER(gBattleTextBuff2, FindBankKeystone(bank));
		PREPARE_SPECIES_BUFFER(gBattleTextBuff3, SPECIES(bank));

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


const u8* DoPrimalReversion(u8 bank, u8 caseId)
{
	pokemon_t* mon = GetBankPartyData(bank);
	const struct Evolution* evolutions = gEvolutionTable[mon->species];
	u16 item = mon->item;

	if (item != ITEM_NONE)
	{
		for (u8 i = 0; i < EVOS_PER_MON; ++i)
		{
			if (evolutions[i].method == EVO_MEGA && evolutions[i].unknown == MEGA_VARIANT_PRIMAL && evolutions[i].param == item)
			{
				DoFormChange(bank, evolutions[i].targetSpecies, TRUE, TRUE, TRUE);

				switch (caseId) {
					case 0:
						return BattleScript_Primal;
					default:
						return BattleScript_PrimalSub;
				}
			}
		}
	}

	return NULL;
}

//In theory, this function will do nothing as the regular forms revert should
//should take care of the reversion. This is to prevent bugs if the player
//gives themselves a Mega or Primal to start the battle.
void MegaRevert(struct Pokemon* party)
{
	int i;

	for (i = 0; i < PARTY_SIZE; ++i)
		TryRevertMega(&party[i]);
}

void TryRevertMega(struct Pokemon* mon)
{
	const struct Evolution* evolutions = gEvolutionTable[mon->species];

	for (u8 i = 0; i < EVOS_PER_MON; ++i)
	{
		if (evolutions[i].method == EVO_NONE) //Most likely end of entries
			break; //Break now to save time
		else if (evolutions[i].method == EVO_MEGA && evolutions[i].param == 0)
		{
			mon->species = evolutions[i].targetSpecies;
			CalculateMonStats(mon);
		}
	}
}

static bool8 IsItemKeystone(u16 item)
{
	for (u8 i = 0; i < ARRAY_COUNT(sKeystoneTable); ++i)
	{
		if (item == sKeystoneTable[i])
			return TRUE;
	}
	return FALSE;
}

static item_t FindTrainerKeystone(u16 trainerId)
{
	if (gBattleTypeFlags & (BATTLE_TYPE_FRONTIER | BATTLE_TYPE_LINK) || IsFrontierTrainerId(trainerId))
		return ITEM_MEGA_RING;

	for (u8 i = 0; i < TRAINER_ITEM_COUNT; ++i)
	{
		if (IsItemKeystone(gTrainers[trainerId].items[i]))
			return gTrainers[trainerId].items[i];
	}

	return ITEM_NONE;
}

static item_t FindPlayerKeystone(void)
{
	#ifdef VAR_KEYSTONE
		u16 keystone = VarGet(VAR_KEYSTONE);
		if (keystone != ITEM_NONE)
			return keystone;
	#else
		for (u8 i = 0; i < ARRAY_COUNT(sKeystoneTable); ++i)
		{
			if (CheckBagHasItem(sKeystoneTable[i], 1))
				return sKeystoneTable[i];
		}
	#endif

	#ifdef DEBUG_MEGA
		return ITEM_MEGA_RING; //Give player Mega Ring if they have none
	#endif

	if (gBattleTypeFlags & (BATTLE_TYPE_FRONTIER | BATTLE_TYPE_LINK))
		return ITEM_MEGA_RING;

	return ITEM_NONE;
}

static item_t FindBankKeystone(u8 bank)
{
	if (gBattleTypeFlags & BATTLE_TYPE_LINK)
		return ITEM_MEGA_RING; //You can always Mega Evolve in a link battle

	#ifdef DEBUG_MEGA
		if (bank + 1)
			return ITEM_MEGA_RING;
	#endif

	if (SIDE(bank) == SIDE_OPPONENT)
	{
		if (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS)
		{
			if (GetBattlerPosition(bank) == B_POSITION_OPPONENT_LEFT)
				return FindTrainerKeystone(gTrainerBattleOpponent_A);
			else
				return FindTrainerKeystone(SECOND_OPPONENT);
		}
		else
			return FindTrainerKeystone(gTrainerBattleOpponent_A);
	}
	else //SIDE_PLAYER
	{
		if (gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER)
		{
			if (GetBattlerPosition(bank) == B_POSITION_PLAYER_RIGHT)
				return FindTrainerKeystone(VarGet(VAR_PARTNER));
			else
				return FindPlayerKeystone();
		}
		else
			return FindPlayerKeystone();
	}
}

bool8 MegaEvolutionEnabled(u8 bank)
{
	if (gBattleTypeFlags & (BATTLE_TYPE_LINK))
		return TRUE;

	if (FindBankKeystone(bank) == ITEM_NONE)
	{
		#ifdef DEBUG_MEGA
			return TRUE;
		#else
			return FALSE;
		#endif
	}
	else
		return TRUE;
}

bool8 BankMegaEvolved(u8 bank, bool8 checkUB)
{
	if ((SIDE(bank) == B_SIDE_PLAYER && gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER)
	||  (SIDE(bank) == B_SIDE_OPPONENT && gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS))
	{
		if (checkUB)
		{
			if (gNewBS->ultraData.done[bank])
				return TRUE;
			else
				return FALSE;
		}
		else
		{
			if (gNewBS->megaData.done[bank])
				return TRUE;
			else
				return FALSE;
		}
	}

	if (checkUB)
	{
		if (gNewBS->ultraData.done[bank] || gNewBS->ultraData.done[PARTNER(bank)])
			return TRUE;
	}
	else
	{
		if (gNewBS->megaData.done[bank] || (gNewBS->megaData.done[PARTNER(bank)]))
			return TRUE;
	}

	return FALSE;
}

bool8 IsMegaSpecies(u16 species)
{
	const struct Evolution* evolutions = gEvolutionTable[species];

	for (u8 i = 0; i < EVOS_PER_MON; ++i)
	{
		if (evolutions[i].method == EVO_NONE) //Most likely end of entries
			break; //Break now to save time
		else if (evolutions[i].method == EVO_MEGA
		&& (evolutions[i].unknown == MEGA_VARIANT_STANDARD || evolutions[i].unknown == MEGA_VARIANT_WISH)
		&& evolutions[i].param == 0)
			return TRUE;
	}

	return FALSE;
}

//No better way to check for these next two
bool8 IsBluePrimalSpecies(unusedArg u16 species)
{
	#ifdef SPECIES_KYOGRE_PRIMAL
	return species == SPECIES_KYOGRE_PRIMAL;
	#else
	return FALSE;
	#endif
}

bool8 IsRedPrimalSpecies(unusedArg u16 species)
{
	#ifdef SPECIES_GROUDON_PRIMAL
	return species == SPECIES_GROUDON_PRIMAL;
	#else
	return FALSE;
	#endif
}

bool8 IsPrimalSpecies(u16 species)
{
	return IsBluePrimalSpecies(species) || IsRedPrimalSpecies(species);
}

bool8 IsMega(u8 bank)
{
	return(IsMegaSpecies(GetBankPartyData(bank)->species));
}

bool8 IsBluePrimal(u8 bank)
{
	return IsBluePrimalSpecies(GetBankPartyData(bank)->species);
}

bool8 IsRedPrimal(u8 bank)
{
	return IsRedPrimalSpecies(GetBankPartyData(bank)->species);
}

bool8 IsUltraNecrozma(u8 bank)
{
	return IsUltraNecrozmaSpecies(GetBankPartyData(bank)->species);
}

bool8 IsUltraNecrozmaSpecies(u16 species)
{
	const struct Evolution* evolutions = gEvolutionTable[species];

	for (u8 i = 0; i < EVOS_PER_MON; ++i)
	{
		if (evolutions[i].method == EVO_NONE) //Most likely end of entries
			break; //Break now to save time
		else if (evolutions[i].method == EVO_MEGA && evolutions[i].unknown == MEGA_VARIANT_ULTRA_BURST && evolutions[i].param == 0)
			return TRUE;
	}

	return FALSE;
}

bool8 HasMegaSymbol(u8 bank)
{
	return IsMega(bank) || IsBluePrimal(bank) || IsRedPrimal(bank) || IsUltraNecrozma(bank);
}

const u8* GetTrainerName(u8 bank)
{
	u8 battlerNum = 0;
	u16 trainerId = 0xFFFF;
	u8 multiplayerId = GetMultiplayerId();

	u16 linkPartner = GetBattlerMultiplayerId(BATTLE_PARTNER(gLinkPlayers[multiplayerId].id));
	u16 linkOpponent1 = GetBattlerMultiplayerId(BATTLE_OPPOSITE(gLinkPlayers[multiplayerId].id));
	u16 linkOpponent2 = GetBattlerMultiplayerId(BATTLE_PARTNER(BATTLE_OPPOSITE(gLinkPlayers[multiplayerId].id)));

	switch (GetBattlerPosition(bank)) {
		case B_POSITION_PLAYER_LEFT:
			if (IsAIControlledBattle())
			{
				if (InBattleSands())
				{
					trainerId = BATTLE_FACILITY_MULTI_TRAINER_TID;
					battlerNum = 2; //Name stored in partner var
				}
				else
				{
					trainerId = VarGet(VAR_PARTNER);
				}
			}
			break;

		case B_POSITION_OPPONENT_LEFT:
			if (gBattleTypeFlags & BATTLE_TYPE_LINK)
				trainerId = linkOpponent1;
			else
				trainerId = gTrainerBattleOpponent_A;
			break;

		case B_POSITION_PLAYER_RIGHT:
			battlerNum = 2;
			if (gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER)
			{
				trainerId = VarGet(VAR_PARTNER);
			}
			else if (gBattleTypeFlags & BATTLE_TYPE_LINK && gBattleTypeFlags & BATTLE_TYPE_MULTI)
				trainerId = linkPartner;
			else if (IsAIControlledBattle())
			{
				if (InBattleSands())
				{
					trainerId = BATTLE_FACILITY_MULTI_TRAINER_TID;
					battlerNum = 2; //Name stored in partner var
				}
				else
				{
					trainerId = VarGet(VAR_PARTNER);
				}
			}
			else
				battlerNum = 0;
			break;

		case B_POSITION_OPPONENT_RIGHT:
			battlerNum = 1;
			if (gBattleTypeFlags & BATTLE_TYPE_LINK && gBattleTypeFlags & BATTLE_TYPE_MULTI)
				trainerId = linkOpponent2;
			else if (gBattleTypeFlags & BATTLE_TYPE_LINK)
				trainerId = linkOpponent1;
			else if (!(gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS))
			{
				trainerId = gTrainerBattleOpponent_A;
				battlerNum = 0;
			}
			else
				trainerId = SECOND_OPPONENT;
			break;
	}

	if (trainerId == 0xFFFF)
	{
		if (gBattleTypeFlags & BATTLE_TYPE_LINK)
			return gLinkPlayers[multiplayerId].name;
		else
			return gSaveBlock2->playerName;
	}
	else if (gBattleTypeFlags & BATTLE_TYPE_LINK)
	{
		return gLinkPlayers[trainerId].name;
	}
	else
	{
		u8 class = gTrainers[trainerId].trainerClass;
		u8* name = NULL;

		if (trainerId < RAID_BATTLE_MULTI_TRAINER_TID)
			name = TryGetRivalNameByTrainerClass(class);

		if (name == NULL)
		{
			if (gBattleTypeFlags & BATTLE_TYPE_FRONTIER
			|| IsFrontierTrainerId(trainerId))
				return GetFrontierTrainerName(trainerId, battlerNum);

			return gTrainers[trainerId].trainerName;
		}

		return name;
	}
}

u8* TryGetRivalNameByTrainerClass(unusedArg u8 class)
{
	#ifdef UNBOUND
		if (class == CLASS_RIVAL|| class == CLASS_RIVAL_2)
			return GetExpandedPlaceholder(ExpandPlaceholder_RivalName);
		else
	#elif defined OVERWRITE_RIVAL
		if (class == CLASS_RIVAL || class == CLASS_RIVAL_2 || class == CLASS_CHAMPION)
			return GetExpandedPlaceholder(ExpandPlaceholder_RivalName);
		else
	#endif
			return NULL;
}

void MegaRetrieveData(void)
{
	if (gBattleTypeFlags & (BATTLE_TYPE_POKE_DUDE | BATTLE_TYPE_OLD_MAN)) //Poke Dude's controller wasn't updated so these need to be manually set to 0
	{
		gNewBS->megaData.chosen[gActiveBattler] = 0;
		gNewBS->ultraData.chosen[gActiveBattler] = 0;
		gNewBS->zMoveData.toBeUsed[gActiveBattler] = 0;
		gNewBS->dynamaxData.toBeUsed[gActiveBattler] = 0;
	}
	else
	{
		gNewBS->megaData.chosen[gActiveBattler] |= gBattleBufferB[gActiveBattler][4];
		gNewBS->ultraData.chosen[gActiveBattler] |= gBattleBufferB[gActiveBattler][5];
		gNewBS->zMoveData.toBeUsed[gActiveBattler] |= gBattleBufferB[gActiveBattler][6];
		gNewBS->dynamaxData.toBeUsed[gActiveBattler] |= gBattleBufferB[gActiveBattler][7];
	}
}
