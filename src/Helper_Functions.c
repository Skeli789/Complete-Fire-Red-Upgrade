#include "defines.h"
#include "defines_battle.h"
#include "../include/random.h"
#include "../include/constants/items.h"
#include "../include/constants/pokedex.h"

#include "../include/new/battle_start_turn_start.h"
#include "../include/new/damage_calc.h"
#include "../include/new/frontier.h"
#include "../include/new/helper_functions.h"
#include "../include/new/item.h"
#include "../include/new/mega.h"
#include "../include/new/move_tables.h"

bool8 CheckTableForMove (move_t move, const u16 table[]) {
	for (u32 i = 0; table[i] != MOVE_TABLES_TERMIN; ++i) {
		if (move == table[i])
			return TRUE;
	}
	return FALSE;
}

bool8 CheckTableForMoveEffect(move_t move, const u8 table[]) {
	for (u32 i = 0; table[i] != MOVE_EFFECT_TABLES_TERMIN; ++i) {
		if (gBattleMoves[move].effect == table[i])
			return TRUE;
	}
	return FALSE;
}

bool8 CheckTableForAbility(u8 ability, const u8 table[]) {
	for (u32 i = 0; table[i] != ABILITY_TABLES_TERMIN; ++i) {
		if (ability == table[i])
			return TRUE;
	}
	return FALSE;
}

bool8 CheckTableForSpecialMoveEffect(u8 effect, const u8 table[]) {
	for (u32 i = 0; table[i] != ABILITY_TABLES_TERMIN; ++i) {
		if (effect == table[i])
			return TRUE;
	}
	return FALSE;
}

bool8 CheckTableForSpecies(u16 species, const u16 table[]) {
	for (u32 i = 0; table[i] != SPECIES_TABLES_TERMIN; ++i) {
		if (species == table[i])
			return TRUE;
	}
	return FALSE;
}

bool8 CheckTableForItem(u16 item, const u16 table[]) {
	for (u32 i = 0; table[i] != ITEM_TABLES_TERMIN; ++i) {
		if (item == table[i])
			return TRUE;
	}
	return FALSE;
}	

ability_t BanksAbility(bank_t bank) {
	if (gStatuses3[bank] & STATUS3_ABILITY_SUPPRESS)
		return ABILITY_NONE;

	return gBattleMons[bank].ability;
}

bank_t GetPartnerBank(bank_t bank) {
	return bank ^ BIT_FLANK;
}

bank_t GetFoeBank(bank_t bank) {
	return (bank ^ BIT_SIDE) & BIT_SIDE;
}

item_effect_t GetBankItemEffect(bank_t bank) {
	if (gBattleMons[bank].ability != ABILITY_KLUTZ && !(gNewBS->EmbargoTimers[bank]) && !gNewBS->MagicRoomTimer)
		return ItemId_GetHoldEffect(gBattleMons[bank].item);
	return 0;
}

bool8 CheckAbilityTargetField(ability_t ability) {
	return (AbilityBattleEffects(ABILITYEFFECT_CHECK_OTHER_SIDE, gBankAttacker, ability, 0, 0));
}

pokemon_t* GetBankPartyData(bank_t bank) {
	u8 index = gBattlerPartyIndexes[bank];
	if (SIDE(bank) == B_SIDE_OPPONENT)
		return &gEnemyParty[index];
	return &gPlayerParty[index];
}

bank_t GetBankFromPartyData(pokemon_t* mon) {

	for (int i = 0; i < gBattlersCount; ++i)
	{
		if (SIDE(i) == B_SIDE_OPPONENT)
		{
			if ((u32) (&gEnemyParty[gBattlerPartyIndexes[i]]) == (u32) mon)
				return i;
		}
	
		if (SIDE(i) == B_SIDE_PLAYER)
		{
			if ((u32) (&gPlayerParty[gBattlerPartyIndexes[i]]) == (u32) mon)
				return i;
		}
	}
	
	return PARTY_SIZE;
}

bool8 CheckGrounding(bank_t bank) {
	if (gStatuses3[gEffectBank] & STATUS3_IN_AIR)
		return IN_AIR;

	if (gNewBS->GravityTimer || GetBankItemEffect(bank) == ITEM_EFFECT_IRON_BALL ||
		(gStatuses3[bank] & (STATUS3_SMACKED_DOWN | STATUS3_ROOTED)))
			return GROUNDED;
			
	else if ((gStatuses3[bank] & (STATUS3_LEVITATING | STATUS3_TELEKINESIS | STATUS3_IN_AIR)) 
		   || ITEM_EFFECT(bank) == ITEM_EFFECT_AIR_BALLOON
		   || gBattleMons[bank].ability == ABILITY_LEVITATE 
		   || gBattleMons[bank].type3 == TYPE_FLYING
		   || gBattleMons[bank].type1 == TYPE_FLYING
		   || gBattleMons[bank].type2 == TYPE_FLYING)
				return IN_AIR;
	
	return GROUNDED;
}

bool8 CheckGroundingFromPartyData(pokemon_t* party) {
	if (gNewBS->GravityTimer 
	|| (ItemId_GetHoldEffect(party->item) == ITEM_EFFECT_IRON_BALL && GetPartyAbility(party) != ABILITY_KLUTZ))
		return GROUNDED;
			
	else if  (GetPartyAbility(party) == ABILITY_LEVITATE 
		|| gBaseStats[party->species].type1 == TYPE_FLYING 
		|| gBaseStats[party->species].type2 == TYPE_FLYING)
			return IN_AIR;
	return GROUNDED;
}

bool8 PartyAlive (u8 bank) {
	pokemon_t* party;
	
	if (GetBattlerSide(bank) == B_SIDE_PLAYER)
		party = gPlayerParty;
	else
		party = gEnemyParty;
	
    for (int i = 0; i < 6; ++i) {
        if (GetMonData(&party[i], REQ_SPECIES, NULL) == SPECIES_NONE) break;
        if (GetMonData(&party[i], REQ_EGG, NULL) || GetMonData(&party[i], REQ_CURRHP, NULL) == 0) continue;
		
		return 1;
    }
	return 0;
}

u8 ViableMonCount(pokemon_t* party) {
	u8 count = 0;
	
    for (int i = 0; i < PARTY_SIZE; ++i) 
	{
        if (party[i].species != SPECIES_NONE
        && !GetMonData(&party[i], REQ_EGG, NULL) 
		&& party[i].hp != 0)
			++count;
    }
	return count;
}

u8 ViableMonCountFromBank(u8 bank) {
	if (SIDE(bank) == B_SIDE_PLAYER)
		return ViableMonCount(gPlayerParty);
	else
		return ViableMonCount(gEnemyParty);
}

u8 ViableMonCountFromBankLoadPartyRange(u8 bank)
{
	u8 count = 0;
	u8 firstMonId, lastMonId;
	struct Pokemon* party = LoadPartyRange(bank, &firstMonId, &lastMonId);

    for (int i = firstMonId; i < lastMonId; ++i) 
	{
        if (party[i].species != SPECIES_NONE
        && !GetMonData(&party[i], REQ_EGG, NULL) 
		&& party[i].hp != 0)
			++count;
    }

	return count;
}

bool8 CheckContact(u16 move, u8 bank) {
	if (!(gBattleMoves[move].flags & FLAG_MAKES_CONTACT) ||
		ITEM_EFFECT(bank) == ITEM_EFFECT_PROTECTIVE_PADS ||
		gBattleMons[bank].ability == ABILITY_LONGREACH)
			return FALSE;
	return TRUE;
}

bool8 CheckContactMove(u16 move) {
	return gBattleMoves[move].flags & FLAG_MAKES_CONTACT;
}

bool8 CheckHealingMove(move_t move) {
	return gBattleMoves[move].flags & FLAG_TRIAGE_AFFECTED;
}

bool8 CheckSoundMove(move_t move) {
	return CheckTableForMove(move, SoundMoveTable);
}

bool8 CheckCurrentMoveSoundMove(void) {
	return (CheckSoundMove(gCurrentMove));
}
	
bool8 SheerForceCheck(void) {
	if (BanksAbility(gBankAttacker) == ABILITY_SHEERFORCE &&
		CheckTableForMove(gCurrentMove, SheerForceTable))
			return TRUE;
	return FALSE;
}

bool8 IsOfType(bank_t bank, u8 type) {
	u8 type1 = gBattleMons[bank].type1;
	u8 type2 = gBattleMons[bank].type2;
	u8 type3 = gBattleMons[bank].type3;
	
	if (!IS_BLANK_TYPE(type1) && type1 == type)
		return TRUE;

	if (!IS_BLANK_TYPE(type2) && type2 == type)
		return TRUE;

	if (!IS_BLANK_TYPE(type3) && type3 == type)
		return TRUE;

	return FALSE;
}

bool8 LiftProtect(bank_t bank) {
	if (gProtectStructs[bank].protected
	|| gProtectStructs[bank].KingsShield
	|| gProtectStructs[bank].SpikyShield
	|| gProtectStructs[bank].BanefulBunker
	|| gSideAffecting[SIDE(bank)] & (SIDE_STATUS_CRAFTY_SHIELD | SIDE_STATUS_MAT_BLOCK | SIDE_STATUS_QUICK_GUARD | SIDE_STATUS_WIDE_GUARD)) 
	{
		gProtectStructs[bank].protected = 0;
		gProtectStructs[bank].KingsShield = 0;
		gProtectStructs[bank].SpikyShield = 0;
		gProtectStructs[bank].BanefulBunker = 0;
		gSideAffecting[SIDE(bank)] &= ~(SIDE_STATUS_CRAFTY_SHIELD | SIDE_STATUS_MAT_BLOCK | SIDE_STATUS_QUICK_GUARD | SIDE_STATUS_WIDE_GUARD);
		return TRUE;
	}
	return FALSE;
}

bool8 ProtectsAgainstZMoves(u16 move, u8 bankAtk, u8 bankDef) {
	if (gProtectStructs[bankDef].protected
	|| gProtectStructs[bankDef].SpikyShield
	|| gProtectStructs[bankDef].BanefulBunker)
		return TRUE;
	
	else if ((gProtectStructs[bankDef].KingsShield || (gSideAffecting[SIDE(bankDef)] & SIDE_STATUS_MAT_BLOCK)) 
		 && SPLIT(move) != SPLIT_STATUS)
			return TRUE;
		
	else if (gSideAffecting[SIDE(bankDef)] & SIDE_STATUS_CRAFTY_SHIELD && bankAtk != bankDef && SPLIT(move) == SPLIT_STATUS)
		return TRUE;
	
	else if (gSideAffecting[SIDE(bankDef)] & (SIDE_STATUS_QUICK_GUARD) && PriorityCalc(bankAtk, ACTION_USE_MOVE, move) > 0)
		return TRUE;
		
	else if (gSideAffecting[SIDE(bankDef)] & (SIDE_STATUS_WIDE_GUARD) 
		&& (gBattleMoves[move].target == MOVE_TARGET_BOTH || gBattleMoves[move].target == MOVE_TARGET_FOES_AND_ALLY))
			return TRUE;
	
	return FALSE;
}

bool8 StatsMaxed(bank_t bank) {
	for (u8 i = 0; i < BATTLE_STATS_NO-1; ++i) {
		if (gBattleMons[bank].statStages[i] < 12)
			return FALSE;
	}
	return TRUE;
}

bool8 StatsMinned(bank_t bank) {
	for (u8 i = 0; i < BATTLE_STATS_NO-1; ++i) {
		if (gBattleMons[bank].statStages[i] > 0)
			return FALSE;
	}
	return TRUE;
}

u32 MathMax (u32 num1, u32 num2) {
	if (num1 > num2)
		return num1;
	return num2;
}

u32 MathMin (u32 num1, u32 num2) {
	if (num1 < num2)
		return num1;
	return num2;
}

u8 GetMoveTarget(u16 move, u8 useMoveTarget) {
	u8 bankAtk = gBankAttacker;
    u8 bankDef = 0;
    u8 moveTarget;
    u8 atkSide, defSide;
	u8 chosen = FALSE;

    if (useMoveTarget)
        moveTarget = useMoveTarget - 1;
    else
        moveTarget = gBattleMoves[move].target;

    switch (moveTarget) {
    case 0:
        defSide = SIDE(bankAtk) ^ BIT_SIDE;
        if (gSideTimers[defSide].followmeTimer && gBattleMons[gSideTimers[defSide].followmeTarget].hp && move != MOVE_SKYDROP)
            bankDef = gSideTimers[defSide].followmeTarget;
		
        else {
			if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE) {
				if (gBattleMons[defSide].hp == 0 && gBattleMons[defSide ^ BIT_FLANK].hp == 0) {
					bankDef = defSide; //Both targets are dead so either target on opposing side works
					chosen = TRUE;
				}
			}
			
			else if (gBattleMons[defSide].hp == 0) {
				bankDef = defSide; //Target is dead so no choice but to hit it
				chosen = TRUE;
			}
			
			if (!chosen) {
				atkSide = SIDE(bankAtk);
				do {
					bankDef = umodsi(Random(), gBattlersCount);
				} while (bankDef == bankAtk || atkSide == SIDE(bankDef) || gAbsentBattlerFlags & gBitTable[bankDef]);
			}
			
			if (NO_MOLD_BREAKERS(ABILITY(bankAtk), move)) 
			{
				u8 moveType = GetMoveTypeSpecial(bankAtk, move);
				switch (moveType) {
					case TYPE_WATER:
						if (ABILITY(bankDef) != ABILITY_STORMDRAIN) 
						{
							if (ABILITY(SIDE(bankAtk) ^ BIT_SIDE) == ABILITY_STORMDRAIN)
							{
								bankDef = SIDE(bankAtk) ^ BIT_SIDE;
								RecordAbilityBattle(bankDef, ABILITY_STORMDRAIN);
								gSpecialStatuses[bankDef].lightningRodRedirected = 1;
							}
							else if (ABILITY(PARTNER(SIDE(bankAtk) ^ BIT_SIDE)) == ABILITY_STORMDRAIN)
							{
								bankDef = PARTNER(SIDE(bankAtk) ^ BIT_SIDE);
								RecordAbilityBattle(bankDef, ABILITY_STORMDRAIN);
								gSpecialStatuses[bankDef].lightningRodRedirected = 1;
							}
							else if (ABILITY(PARTNER(bankAtk)) == ABILITY_STORMDRAIN)
							{
								bankDef = PARTNER(bankAtk);
								RecordAbilityBattle(bankDef, ABILITY_STORMDRAIN);
								gSpecialStatuses[bankDef].lightningRodRedirected = 1;
							}
						}
						break;
						
					case TYPE_ELECTRIC:
						if (ABILITY(bankDef) != ABILITY_LIGHTNINGROD) 
						{
							if (ABILITY(SIDE(bankAtk) ^ BIT_SIDE) == ABILITY_LIGHTNINGROD)
							{
								bankDef = SIDE(bankAtk) ^ BIT_SIDE;
								RecordAbilityBattle(bankDef, ABILITY_LIGHTNINGROD);
								gSpecialStatuses[bankDef].lightningRodRedirected = 1;
							}
							else if (ABILITY(PARTNER(SIDE(bankAtk) ^ BIT_SIDE)) == ABILITY_LIGHTNINGROD)
							{
								bankDef = PARTNER(SIDE(bankAtk) ^ BIT_SIDE);
								RecordAbilityBattle(bankDef, ABILITY_LIGHTNINGROD);
								gSpecialStatuses[bankDef].lightningRodRedirected = 1;
							}
							else if (ABILITY(PARTNER(bankAtk)) == ABILITY_LIGHTNINGROD)
							{
								bankDef = PARTNER(bankAtk);
								RecordAbilityBattle(bankDef, ABILITY_LIGHTNINGROD);
								gSpecialStatuses[bankDef].lightningRodRedirected = 1;
							}
						}
				}
			}
        }
        break;
	
    case 1:
    case 8:
    case 32:
    case 64:
        bankDef = GetBattlerAtPosition((GetBattlerPosition(bankAtk) & BIT_SIDE) ^ BIT_SIDE);
        if (gAbsentBattlerFlags & gBitTable[bankDef])
            bankDef ^= BIT_FLANK;
        break;
	
    case 4:
        defSide = SIDE(bankAtk) ^ BIT_SIDE;
        if (gSideTimers[defSide].followmeTimer && gBattleMons[gSideTimers[defSide].followmeTarget].hp)
            bankDef = gSideTimers[defSide].followmeTarget;
		
        else if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE && moveTarget & 4) {
            if (SIDE(bankAtk) == B_SIDE_PLAYER) {
                if (Random() & 1)
                    bankDef = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);
                else
                    bankDef = GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT);
            }
            else
            {
                if (Random() & 1)
                    bankDef = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);
                else
                    bankDef = GetBattlerAtPosition(B_POSITION_PLAYER_RIGHT);
            }
            if (gAbsentBattlerFlags & gBitTable[bankDef])
                bankDef ^= 2;
        }
        else
            bankDef = GetBattlerAtPosition((GetBattlerPosition(bankAtk) & BIT_SIDE) ^ BIT_SIDE);
        break;
	
    case 2:
		//Put something for Acupressure here
    case 16:
        bankDef = bankAtk;
        break;
    }
	
	if (!gNewBS->DancerInProgress && !gNewBS->InstructInProgress)
		gBattleStruct->moveTarget[bankAtk] = bankDef;
	
    return bankDef;
}

void HealMon(pokemon_t* poke) {

    //Restore HP.
	poke->hp = poke->maxHP;
    
	//Restore PP.
	MonRestorePP(poke);

    //Restore Status.
    poke->condition = 0;
}

bool8 IsBattlerAlive(u8 bank) {
    if (gBattleMons[bank].hp == 0
	||	bank >= gBattlersCount
	||	gAbsentBattlerFlags & gBitTable[bank]) {
        return FALSE;
	}
	return TRUE;
}

u16 GetNationalPokedexCount(u8 caseID) {
    u16 count = 0;
    u32 i;
	u8 byte;

	u8* flags;

    switch (caseID) {
        case FLAG_GET_SEEN:
            flags = (u8*) SEEN_DEX_FLAGS;
            break;
		
        default: //case FLAG_GET_CAUGHT:
            flags = (u8*) CAUGHT_DEX_FLAGS;
            break;
    }
	
    for (i = 0; i <= (NATIONAL_DEX_COUNT - 1) / 8; ++i) //8 Pokemon per byte
	{
		byte = flags[i];
		
		while (byte != 0)
		{
			if (byte & 1)
				++count;
			
			byte >>= 1;
		}
    }
    return count;
}

/*TODO
bool8 CheckBattleTerrain(u8 caseID) {
	return TRUE;
}
*/

pokemon_t* LoadPartyRange(u8 bank, u8* FirstMonId, u8* lastMonId) {
	pokemon_t* party;
	
	if (SIDE(bank) == B_SIDE_OPPONENT) {
        party = gEnemyParty;
	}
    else {
        party = gPlayerParty;
	}

	if (gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER && SIDE(bank) == B_SIDE_PLAYER) {

        if (bank & BIT_FLANK) 
			*FirstMonId = 3;
		else 
			*FirstMonId = 0;
		
		*lastMonId = *FirstMonId + 3;
    }
	
    else if (gBattleTypeFlags & BATTLE_TYPE_MULTI) {
		
		//Two Human Trainers vs Two AI Trainers
        if (gBattleTypeFlags & BATTLE_TYPE_TOWER_LINK_MULTI)
		{
            if (SIDE(bank) == B_SIDE_PLAYER) {
                *FirstMonId = 0;
                if (GetLinkTrainerFlankId(GetBattlerMultiplayerId(bank)) == TRUE)
                    *FirstMonId = 3;
            }
			
            else { //B_SIDE_OPPONENT
                if (bank == B_POSITION_OPPONENT_LEFT)
                    *FirstMonId = 0;
                else
                    *FirstMonId = 3;
            }
        }
		
		//Two Human Trainers vs Two Human Trainers
        else
		{
            *FirstMonId = 0;
            if (GetLinkTrainerFlankId(GetBattlerMultiplayerId(bank)) == TRUE)
                *FirstMonId = 3;
        }
		
		*lastMonId = *FirstMonId + 3;
    }
	
    else if (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS && SIDE(bank) == B_SIDE_OPPONENT)
    {
        *FirstMonId = 0;
        if (GetBattlerPosition(bank) == B_POSITION_OPPONENT_RIGHT)
            *FirstMonId = 3;

		*lastMonId = *FirstMonId + 3;
    }
	
	
    else //Single Battle // Regular Double Battle // Regular Double Against Two Trainers + PlayerSide // Wild Double Battle
    {
		*FirstMonId = 0;
		*lastMonId = PARTY_SIZE;
    }
	
	return party;
}

ability_t CopyAbility(u8 bank) {
	if (gStatuses3[bank] & STATUS3_ABILITY_SUPPRESS)
		return gNewBS->SuppressedAbilities[bank];
	else if (gNewBS->DisabledMoldBreakerAbilities[bank])
		return gNewBS->DisabledMoldBreakerAbilities[bank];
	else
		return gBattleMons[bank].ability;
}

ability_t* GetAbilityLocation(u8 bank) {
	if (gStatuses3[bank] & STATUS3_ABILITY_SUPPRESS)
		return &gNewBS->SuppressedAbilities[bank];
	else if (gNewBS->DisabledMoldBreakerAbilities[bank])
		return &gNewBS->DisabledMoldBreakerAbilities[bank];
	else
		return &gBattleMons[bank].ability;
}


//Check Function
bool8 UproarWakeUpCheck(u8 bank) {
    int i;
	bank += 1; //Useless statement
	
    for (i = 0; i < gBattlersCount; ++i) {
        if (gBattleMons[i].status2 & STATUS2_UPROAR) {
			gBattleScripting->bank = i;
			if (gBankTarget == 0xFF)
				gBankTarget = i;
			else if (gBankTarget == i)
				gBattleCommunication[MULTISTRING_CHOOSER] = 0;
			else
				gBattleCommunication[MULTISTRING_CHOOSER] = 1;
			break;
		}
    }
	
    if (i == gBattlersCount)
        return FALSE;
    else
        return TRUE;
}

bool8 IsUproarBeingMade(void) {
	for (int i = 0; i < gBattlersCount; ++i) {
		if (gBattleMons[i].status2 & STATUS2_UPROAR)
			return TRUE;
	}
	return FALSE;
}

//Change to loop through battle modified party indexes
u8 GetIllusionPartyNumber(u8 bank)
{
	int i;
	u8 firstMonId, lastMonId;
	
	if (gStatuses3[bank] & STATUS3_ILLUSION)
	{
		//Wild Pokemon can't diguise themselves
		if (!(gBattleTypeFlags & BATTLE_TYPE_TRAINER) && SIDE(bank) == B_SIDE_OPPONENT)
			return gBattlerPartyIndexes[bank];

		pokemon_t* party = LoadPartyRange(bank, &firstMonId, &lastMonId);
		
		for (i = lastMonId - 1; i >= firstMonId; --i) //Loop through party in reverse order
		{ 
			if (i == gBattlerPartyIndexes[bank]) //Finsihed checking mons after
				return gBattlerPartyIndexes[bank];
		
			if (party[i].species == SPECIES_NONE ||  party[i].hp == 0)
				continue;
			
			return i;
		}
	}
	
	return gBattlerPartyIndexes[bank];
}

pokemon_t* GetIllusionPartyData(u8 bank) 
{
	u8 firstMonId, lastMonId;
	pokemon_t* party = LoadPartyRange(bank, &firstMonId, &lastMonId);
	
	return &party[GetIllusionPartyNumber(bank)];
}

bool8 BankMovedBefore(u8 bank1, u8 bank2) {
	for (u8 i = 0; i < gBattlersCount; ++i) {
		if (gBanksByTurnOrder[i] == bank1)
			return TRUE;
		else if (gBanksByTurnOrder[i] == bank2)
			return FALSE;
	}
	return FALSE; //Should never be reached
}

bool8 IsFirstAttacker(u8 bank) {
	for (u8 i = 0; i < gBattlersCount; ++i) {
		if (gActionsByTurnOrder[i] == ACTION_USE_ITEM
		||  gActionsByTurnOrder[i] == ACTION_SWITCH)
			continue;
	
		return gBanksByTurnOrder[i] == bank;
	}
	return FALSE; //Should never be reached
}

bool8 CanTransferItem(u16 species, u16 item, pokemon_t* party_data) {
	u16 dexNum = SpeciesToNationalPokedexNum(species);
	u8 effect = ItemId_GetHoldEffect(item);
	u8 quality = ItemId_GetHoldEffectParam(item);
	const struct Evolution* evolutions = gEvolutionTable[party_data->species];
	int i;
	
	if (item == ITEM_NONE || IsMail(item))
		return FALSE;
	
	switch (effect) {
		case ITEM_EFFECT_Z_CRYSTAL:
			return FALSE;
			
		case ITEM_EFFECT_GRISEOUS_ORB:
			if (dexNum == NATIONAL_DEX_GIRATINA)
				return FALSE;
			break;
			
		case ITEM_EFFECT_PLATE:
			if (dexNum == NATIONAL_DEX_ARCEUS)
				return FALSE;
			break;
			
		case ITEM_EFFECT_MEMORY:
			if (dexNum == NATIONAL_DEX_SILVALLY)
				return FALSE;
			break;
			
		case ITEM_EFFECT_DRIVE:
			if (dexNum == NATIONAL_DEX_GENESECT)
				return FALSE;
			break;

		case ITEM_EFFECT_MEGA_STONE:
			for (i = 0; i < EVOS_PER_MON; ++i) 
			{
				if ((evolutions[i].method == MEGA_EVOLUTION && evolutions[i].param == item) //Can Mega Evolve
				||  (evolutions[i].method == MEGA_EVOLUTION && evolutions[i].param == 0)) //Is Mega
					return FALSE;
			}
			break;
		
		case ITEM_EFFECT_PRIMAL_ORB:
			if ((quality == QUALITY_BLUE_ORB && dexNum == NATIONAL_DEX_KYOGRE)
			||  (quality == QUALITY_RED_ORB && dexNum == NATIONAL_DEX_GROUDON))
				return FALSE;
	}

	return TRUE;
}

//Make sure the input bank is any bank on the specific mon's side
bool8 CanFling(u8 ability, u16 item, struct Pokemon* mon, u8 bank, bool8 partyCheck)
{
	u8 itemEffect = ItemId_GetHoldEffect(item);

	if (ability == ABILITY_KLUTZ
	|| gNewBS->MagicRoomTimer
	|| (!partyCheck && gNewBS->EmbargoTimers[bank])
	|| !CanTransferItem(mon->species, item, mon)
	|| itemEffect == ITEM_EFFECT_PRIMAL_ORB
	|| itemEffect == ITEM_EFFECT_GEM
	|| itemEffect == ITEM_EFFECT_ABILITY_CAPSULE
	|| (IsBerry(item) && AbilityBattleEffects(ABILITYEFFECT_CHECK_OTHER_SIDE, bank, ABILITY_UNNERVE, 0, 0))
	|| GetPocketByItemId(item) == POCKET_POKE_BALLS)
		return FALSE;

	return TRUE;
}

bool8 SymbiosisCanActivate(u8 giverBank, u8 receiverBank) {
	u16 item = gBattleMons[giverBank].item;
	
	if (!(gBattleTypeFlags & BATTLE_TYPE_DOUBLE) 
	||  ABILITY(giverBank) != ABILITY_SYMBIOSIS
	||  ITEM(receiverBank) != 0
	|| !CanTransferItem(gBattleMons[giverBank].species, item, GetBankPartyData(giverBank)) 
	|| !CanTransferItem(gBattleMons[receiverBank].species, item, GetBankPartyData(receiverBank)))
		return FALSE;
	return TRUE;
}

//Sticky Hold also, but the boost ignores it
bool8 CanKnockOffItem(u8 bank)
{
	if (!CanTransferItem(SPECIES(bank), ITEM(bank), GetBankPartyData(bank)))
		return FALSE;

	return TRUE;
}

bool8 CanEvolve(pokemon_t* mon) {
	int i;
	const struct Evolution* evolutions = gEvolutionTable[mon->species];
	
	for (i = 0; i < EVOS_PER_MON; ++i) 
	{
		if (evolutions[i].method != MEGA_EVOLUTION && evolutions[i].method != 0)
			return TRUE;
	}
	return FALSE;
}

bool8 CouldHaveEvolvedViaLevelUp(pokemon_t* mon) {
	int i;
	const struct Evolution* evolutions = gEvolutionTable[mon->species];
	
	for (i = 0; i < EVOS_PER_MON; ++i) 
	{
		if (evolutions[i].method == EVO_LEVEL && mon->level >= evolutions[i].param)
			return TRUE;
	}
	return FALSE;
}

u8 CountBoosts(bank_t bank) {
	u8 sum = 0;
	for (u8 i = 0; i < BATTLE_STATS_NO-1; ++i) {
		if (gBattleMons[bank].statStages[i] > 6)
			sum += gBattleMons[bank].statStages[i] - 6;
	}
	return sum;
}

bool8 MoveBlockedBySubstitute(u16 move, u8 atkBank, u8 defBank) {
	if (!(gBattleMons[defBank].status2 & STATUS2_SUBSTITUTE)
	|| CheckSoundMove(move)
	|| ABILITY(atkBank) == ABILITY_INFILTRATOR)
		return FALSE;
	
	return TRUE;
}

bool8 IsMockBattle(void) {
	if (gBattleTypeFlags & BATTLE_TYPE_MOCK_BATTLE)
		return TRUE;
	return FALSE;
}

u8 CalcMoveSplit(u8 bank, u16 move) {
	if (CheckTableForMove(move, MovesThatChangePhysicality)
	&&  SPLIT(move) != SPLIT_STATUS)
	{
		u32 attack = gBattleMons[bank].attack;
		u32 spAttack = gBattleMons[bank].spAttack;

		attack = attack * gStatStageRatios[gBattleMons[bank].statStages[STAT_STAGE_ATK-1]][0];
		attack = udivsi(attack, gStatStageRatios[gBattleMons[bank].statStages[STAT_STAGE_ATK-1]][1]);
		
		spAttack = spAttack * gStatStageRatios[gBattleMons[bank].statStages[STAT_STAGE_SPATK-1]][0];
		spAttack = udivsi(spAttack, gStatStageRatios[gBattleMons[bank].statStages[STAT_STAGE_SPATK-1]][1]);
		
		if (spAttack >= attack)
			return SPLIT_SPECIAL;
		else
			return SPLIT_PHYSICAL;
	}
	
	#ifdef OLD_MOVE_SPLIT
		if (gBattleMoves[move].type < TYPE_FIRE)
			return SPLIT_PHYSICAL;
		else
			return SPLIT_SPECIAL;
	#else
		return SPLIT(move);
	#endif
}

u8 CalcMoveSplitFromParty(pokemon_t* mon, u16 move) {
	if (CheckTableForMove(move, MovesThatChangePhysicality))
	{	
		if (mon->spAttack >= mon->attack)
			return SPLIT_SPECIAL;
		else
			return SPLIT_PHYSICAL;
	}

	return SPLIT(move);
}

u8 FindMovePositionInMoveset(u16 move, u8 bank) {
	int i;
	for (i = 0; i < MAX_MON_MOVES; ++i)
	{
		if (gBattleMons[bank].moves[i] == move)
			break;
	}
	return i;
}

bool8 MoveInMoveset(u16 move, u8 bank)
{
	return FindMovePositionInMoveset(move, bank) < MAX_MON_MOVES;
}

u8 AttacksThisTurn(u8 bank, u16 move) // Note: returns 1 if it's a charging turn, otherwise 2
{
    // first argument is unused
	if (ITEM_EFFECT(bank) == ITEM_EFFECT_POWER_HERB)
		return 2;
	
    if (gBattleMoves[move].effect == EFFECT_SOLARBEAM && (gBattleWeather & WEATHER_SUN_ANY))
        return 2;

    if (gBattleMoves[move].effect == EFFECT_SKULL_BASH
    ||  gBattleMoves[move].effect == EFFECT_RAZOR_WIND
    ||  gBattleMoves[move].effect == EFFECT_SKY_ATTACK
    ||  gBattleMoves[move].effect == EFFECT_SOLARBEAM
    ||  gBattleMoves[move].effect == EFFECT_SEMI_INVULNERABLE
    ||  gBattleMoves[move].effect == EFFECT_BIDE
	||	move == MOVE_GEOMANCY
	||  move == MOVE_SKYDROP)
    {
        if (gBattleMons[bank].status2 & STATUS2_MULTIPLETURNS)
            return 2;
		else
			return 1; //About to initiate attack.
    }
	
    return 2;
}

bool8 IsZMove(const u16 move)
{
	return move >= FIRST_Z_MOVE && move <= LAST_Z_MOVE;
}

void AddBankToPickupStack(const u8 bank)
{
	int i, j;
	u8 newStack[4];
	
	for (i = 0, j = 0; i < gBattlersCount; ++i)
	{
		if (gNewBS->pickupStack[i] != bank && gNewBS->pickupStack[i] != 0xFF)
			newStack[j++] = gNewBS->pickupStack[i];
	}
	
	newStack[j++] = bank;
	
	while (j < gBattlersCount)
		newStack[j++] = 0xFF;
		
	for (i = 0; i < gBattlersCount; ++i)
		gNewBS->pickupStack[i] = newStack[i];
}

void RemoveBankFromPickupStack(const u8 bank)
{
	int i, j;
	u8 newStack[4];
	
	for (i = 0, j = 0; i < gBattlersCount; ++i)
	{
		if (gNewBS->pickupStack[i] != bank && gNewBS->pickupStack[i] != 0xFF)
			newStack[j++] = gNewBS->pickupStack[i];
	}
	
	while (j < gBattlersCount)
		newStack[j++] = 0xFF;
		
	for (i = 0; i < gBattlersCount; ++i)
		gNewBS->pickupStack[i] = newStack[i];
}

u8 GetTopOfPickupStackNotIncludingBank(const u8 bank)
{
	int i;
	
	for (i = 0; i < gBattlersCount; ++i)
	{
		if (gNewBS->pickupStack[i] == 0xFF)
			break;
	}
	
	if (i == 0 //Stack is empty
	|| (i == 1 && gNewBS->pickupStack[0] == bank)) //Stack only contains ignored bank
		return 0xFF;
	
	if (gNewBS->pickupStack[i - 1] == bank)
		return gNewBS->pickupStack[i - 2];
	
	return gNewBS->pickupStack[i - 1];
}

void ClearBankStatus(u8 bank)
{
	if (gBattleMons[bank].status1 & (STATUS_POISON | STATUS_TOXIC_POISON))
		StringCopy(gBattleTextBuff1, gStatusConditionString_Poison);
	else if (gBattleMons[bank].status1 & STATUS_SLEEP)
		StringCopy(gBattleTextBuff1, gStatusConditionString_Sleep);
	else if (gBattleMons[bank].status1 & STATUS_PARALYSIS)
		StringCopy(gBattleTextBuff1, gStatusConditionString_Paralysis);
	else if (gBattleMons[bank].status1 & STATUS_BURN)
		StringCopy(gBattleTextBuff1, gStatusConditionString_Burn);
	else if (gBattleMons[bank].status1 & STATUS_FREEZE)
		StringCopy(gBattleTextBuff1, gStatusConditionString_Ice);
	gBattleMons[bank].status1 = 0;
	gBattleMons[bank].status2 &= ~(STATUS2_NIGHTMARE);
	gActiveBattler = bank;
	EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 4, &gBattleMons[bank].status1);
	MarkBufferBankForExecution(gActiveBattler);
}

bool8 DoesSleepClausePrevent(u8 bank)
{
	if (gBattleTypeFlags & BATTLE_TYPE_FRONTIER)
	{
		switch (VarGet(BATTLE_TOWER_TIER)) {
			case BATTLE_TOWER_OU:
			case BATTLE_TOWER_UBER:
			case BATTLE_TOWER_LITTLE_CUP: ;
				u8 firstId, lastId;
				struct Pokemon* party = LoadPartyRange(bank, &firstId, &lastId);
				
				for (int i = 0; i < PARTY_SIZE; ++i)
				{
					if (GetMonData(&party[i], MON_DATA_HP, NULL) != 0
					&& !GetMonData(&party[i], MON_DATA_IS_EGG, NULL)
					&& GetMonData(&party[i], MON_DATA_STATUS, NULL) & STATUS_SLEEP) //Someone on team is already asleep
						return TRUE;
				}
		}
	}
	
	return FALSE;
}

bool8 CanBeGeneralStatused(u8 bank, bool8 checkFlowerVeil)
{
	if (ABILITY(bank) == ABILITY_SHIELDSDOWN
	&&  GetBankPartyData(bank)->species == SPECIES_MINIOR_SHIELD) //Prevents Ditto from getting this benefit
		return FALSE;
	
	switch (ABILITY(bank)) {
		case ABILITY_COMATOSE:
			return FALSE;
		
		case ABILITY_LEAFGUARD:
			if (WEATHER_HAS_EFFECT && gBattleWeather & WEATHER_SUN_ANY)
				return FALSE;
			break;
			
		case ABILITY_FLOWERVEIL:
			if (checkFlowerVeil && IsOfType(bank, TYPE_GRASS))
				return FALSE;
	}
	
	if (checkFlowerVeil && ABILITY(PARTNER(bank)) == ABILITY_FLOWERVEIL && IsOfType(bank, TYPE_GRASS))
		return FALSE;
	
	if (TerrainType == MISTY_TERRAIN && CheckGrounding(bank))
		return FALSE;
		
	if (gBattleMons[bank].status1 != STATUS1_NONE)
		return FALSE;

	return TRUE;
}

bool8 CanBePutToSleep(u8 bank, bool8 checkFlowerVeil)
{
	if (!CanBeGeneralStatused(bank, checkFlowerVeil))
		return FALSE;
	
	switch (ABILITY(bank)) {
		case ABILITY_INSOMNIA:
		case ABILITY_VITALSPIRIT:
		case ABILITY_SWEETVEIL:
			return FALSE;
	}
	
	if (TerrainType == ELECTRIC_TERRAIN && CheckGrounding(bank))
		return FALSE;
		
	if (DoesSleepClausePrevent(bank))
		return FALSE;	
		
	return TRUE;
}

bool8 CanBePoisoned(u8 bankDef, u8 bankAtk, bool8 checkFlowerVeil) {
	if (!CanBeGeneralStatused(bankDef, checkFlowerVeil))
		return FALSE;
	
	switch (ABILITY(bankDef)) {
		case ABILITY_IMMUNITY:
			return FALSE;
	}
	
	if (ABILITY(bankAtk) != ABILITY_CORROSION) {
		if (IsOfType(bankDef, TYPE_POISON) || IsOfType(bankDef, TYPE_STEEL))
			return FALSE;
	}

	return TRUE;
}

bool8 CanBeParalyzed(u8 bank, bool8 checkFlowerVeil) {
	if (!CanBeGeneralStatused(bank, checkFlowerVeil))
		return FALSE;
	
	if (IsOfType(bank, TYPE_ELECTRIC))
		return FALSE;
		
	switch (ABILITY(bank)) {
		case ABILITY_LIMBER:
			return FALSE;
	}
	return TRUE;
}

bool8 CanBeBurned(u8 bank, bool8 checkFlowerVeil) {
	if (!CanBeGeneralStatused(bank, checkFlowerVeil))
		return FALSE;
	
	if (IsOfType(bank, TYPE_FIRE))
		return FALSE;
		
	switch (ABILITY(bank)) {
		case ABILITY_WATERVEIL:
		case ABILITY_WATERBUBBLE:
			return FALSE;
	}
	return TRUE;
}

bool8 CanBeFrozen(u8 bank, bool8 checkFlowerVeil) {
	if (!CanBeGeneralStatused(bank, checkFlowerVeil))
		return FALSE;

	if (IsOfType(bank, TYPE_ICE))
		return FALSE;
		
	switch (ABILITY(bank)) {
		case ABILITY_MAGMAARMOR:
			return FALSE;
	}
	
	if (WEATHER_HAS_EFFECT && gBattleWeather & WEATHER_SUN_ANY)
		return FALSE;
	
	return TRUE;
}

bool8 CanBeConfused(u8 bank) {
	if (gBattleMons[bank].status2 & STATUS2_CONFUSION)
		return FALSE;

	if (TerrainType == MISTY_TERRAIN)
		return FALSE;
		
	if (ABILITY(bank) == ABILITY_OWNTEMPO)
		return FALSE;

	return TRUE;	
}

bool8 CanPartyMonBeGeneralStatused(pokemon_t* mon) {
	if (mon->species == SPECIES_MINIOR_SHIELD)
		return FALSE;
	
	switch (GetPartyAbility(mon)) {
		case ABILITY_COMATOSE:
			return FALSE;
			
		case ABILITY_FLOWERVEIL:
			if (gBaseStats[mon->species].type1 == TYPE_GRASS
			||  gBaseStats[mon->species].type2 == TYPE_GRASS)
				return FALSE;
	}
	
	return TRUE;
}

bool8 CanPartyMonBePutToSleep(pokemon_t* mon) {
	if (!CanPartyMonBeGeneralStatused(mon))
		return FALSE;
	
	switch (GetPartyAbility(mon)) {
		case ABILITY_INSOMNIA:
		case ABILITY_VITALSPIRIT:
		case ABILITY_SWEETVEIL:
			return FALSE;
	}
	
	return TRUE;
}

bool8 CanPartyMonBePoisoned(pokemon_t* mon) {
	if (!CanPartyMonBeGeneralStatused(mon))
		return FALSE;
	
	switch (GetPartyAbility(mon)) {
		case ABILITY_IMMUNITY:
			return FALSE;
	}
	
	if (gBaseStats[mon->species].type1 == TYPE_POISON
	||  gBaseStats[mon->species].type2 == TYPE_POISON
	||  gBaseStats[mon->species].type1 == TYPE_STEEL
	||  gBaseStats[mon->species].type2 == TYPE_STEEL)
		return FALSE;

	return TRUE;
}

bool8 CanPartyMonBeParalyzed(pokemon_t* mon) {
	if (!CanPartyMonBeGeneralStatused(mon))
		return FALSE;

	switch (GetPartyAbility(mon)) {
		case ABILITY_LIMBER:
			return FALSE;
	}
	
	if (gBaseStats[mon->species].type1 == TYPE_ELECTRIC
	||  gBaseStats[mon->species].type2 == TYPE_ELECTRIC)
		return FALSE;
		
	return TRUE;
}

bool8 CanPartyMonBeBurned(pokemon_t* mon) {
	if (!CanPartyMonBeGeneralStatused(mon))
		return FALSE;
		
	switch (GetPartyAbility(mon)) {
		case ABILITY_WATERVEIL:
		case ABILITY_WATERBUBBLE:
			return FALSE;
	}
	
	if (gBaseStats[mon->species].type1 == TYPE_FIRE
	||  gBaseStats[mon->species].type2 == TYPE_FIRE)
		return FALSE;
	
	return TRUE;
}

bool8 CanPartyMonBeFrozen(pokemon_t* mon) {
	if (!CanPartyMonBeGeneralStatused(mon))
		return FALSE;

	switch (GetPartyAbility(mon)) {
		case ABILITY_MAGMAARMOR:
			return FALSE;
	}

	if (gBaseStats[mon->species].type1 == TYPE_ICE
	||  gBaseStats[mon->species].type2 == TYPE_ICE)
		return FALSE;
		
	return TRUE;
}

