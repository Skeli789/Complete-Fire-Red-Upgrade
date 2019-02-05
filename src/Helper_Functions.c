#include "defines.h"
#include "mega.h"

extern move_t SheerForceTable[];
extern move_t SoundMoveTable[];
extern move_t MoldBreakerMoves[];
extern move_t MovesThatChangePhysicality[];
extern struct BattleMove gBattleMoves[];

extern u8 GetMoveTypeSpecial(bank_t, move_t);
extern s8 PriorityCalc(u8 bank, u8 action, u16 move);

bool8 CheckTableForMove (move_t move, const u16 table[]) {
	for (u32 i = 0; table[i] != MOVE_TABLES_TERMIN; ++i) {
		if (move == table [i])
			return TRUE;
	}
	return FALSE;
}

bool8 CheckTableForMoveEffect (move_t move, const u16 table[]) {
	for (u32 i = 0; table[i] != MOVE_TABLES_TERMIN; ++i) {
		if (gBattleMoves[move].effect == table [i])
			return TRUE;
	}
	return FALSE;
}

bool8 CheckTableForAbility (u8 ability, const u8 table[]) {
	for (u32 i = 0; table[i] != ABILITY_TABLES_TERMIN; ++i) {
		if (ability == table [i])
			return TRUE;
	}
	return FALSE;
}

ability_t BanksAbility(bank_t bank) {
	if (gStatuses3[bank] & STATUS3_ABILITY_SUPPRESS)
		return 0;
	return gBattleMons[bank].ability;
}

bank_t GetPartnerBank(bank_t bank) {
	return bank ^ BIT_FLANK;
}

bank_t GetFoeBank(bank_t bank) {
	return (bank ^ BIT_SIDE) & BIT_SIDE;
}

item_effect_t GetBankItemEffect(bank_t bank) {
	if (gBattleMons[bank].ability != ABILITY_KLUTZ && !(EmbargoTimers[bank]) && !MagicRoomTimer)
		return ItemId_GetHoldEffect(gBattleMons[bank].item);
	return 0;
}

bool8 CheckAbilityTargetField(ability_t ability) {
	return (AbilityBattleEffects(ABILITYEFFECT_CHECK_OTHER_SIDE, gBankAttacker, ability, 0, 0));
}

pokemon_t* GetBankPartyData(bank_t bank) {
	u8 index = gBattlerPartyIndexes[bank];
	if (bank & 1)
		return &gEnemyParty[index];
	return &gPlayerParty[index];
}

enum {IN_AIR, GROUNDED};
bool8 CheckGrounding(bank_t bank) {
	if (GravityTimer || GetBankItemEffect(bank) == ITEM_EFFECT_IRON_BALL ||
		(gStatuses3[bank] & (STATUS3_SMACKED_DOWN | STATUS3_ROOTED)))
			return GROUNDED;
			
	else if ((gStatuses3[bank] & (STATUS3_LEVITATING | STATUS3_TELEKINESIS | STATUS3_IN_AIR)) 
		   || gBattleMons[bank].ability == ABILITY_LEVITATE 
		   || gBattleMons[bank].type3 == TYPE_FLYING
		   || gBattleMons[bank].type1 == TYPE_FLYING
		   || gBattleMons[bank].type2 == TYPE_FLYING)
				return IN_AIR;
	return GROUNDED;
}

bool8 CheckGroundingFromPartyData(pokemon_t* party) {
	if (GravityTimer 
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
        if (pokemon_getattr(&party[i], REQ_SPECIES) == PKMN_NONE) break;
        if (pokemon_getattr(&party[i], REQ_EGG) || pokemon_getattr(&party[i], REQ_CURRHP) == 0) continue;
		
		return 1;
    }
	return 0;
}

u8 ViableMonCount(pokemon_t* party) {
	u8 count = 0;
	
    for (int i = 0; i < 6; ++i) {
        if (pokemon_getattr(&party[i], REQ_SPECIES) == PKMN_NONE
        || pokemon_getattr(&party[i], REQ_EGG) 
		|| pokemon_getattr(&party[i], REQ_CURRHP) == 0) 
			continue;
		else
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
	if (gBattleMons[bank].type1 == type ||
		gBattleMons[bank].type2 == type ||
		gBattleMons[bank].type3 == type)
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
        if (gSideTimers[defSide].followmeTimer && gBattleMons[gSideTimers[defSide].followmeTarget].hp)
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
			
			if(NO_MOLD_BREAKERS(ABILITY(gBankAttacker)) && !CheckTableForMove(move, MoldBreakerMoves)) {
				u8 moveType = GetMoveTypeSpecial(bankAtk, move);
				if ((moveType == TYPE_ELECTRIC
				&& AbilityBattleEffects(ABILITYEFFECT_COUNT_OTHER_SIDE, bankAtk, ABILITY_LIGHTNINGROD, 0, 0)
				&& gBattleMons[bankDef].ability != ABILITY_LIGHTNINGROD)
				||
					(moveType == TYPE_WATER
				&& AbilityBattleEffects(ABILITYEFFECT_COUNT_OTHER_SIDE, bankAtk, ABILITY_STORMDRAIN, 0, 0)
				&& gBattleMons[bankDef].ability != ABILITY_STORMDRAIN)) {
					bankDef ^= BIT_FLANK;
					RecordAbilityBattle(bankDef, gBattleMons[bankDef].ability);
					gSpecialStatuses[bankDef].lightningRodRedirected = 1;
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
    u16 i;

    for (i = 0; i < NATIONAL_DEX_COUNT; i++) {
        switch (caseID) {
        case FLAG_GET_SEEN:
            if (GetSetPokedexFlag(i + 1, FLAG_GET_SEEN))
                count++;
            break;
		
        case FLAG_GET_CAUGHT:
            if (GetSetPokedexFlag(i + 1, FLAG_GET_CAUGHT))
                count++;
            break;
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
	
	if (GetBattlerSide(bank) == B_SIDE_OPPONENT) {
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
            if (GetBattlerSide(bank) == B_SIDE_PLAYER) {
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
		return SuppressedAbilities[bank];
	else
		return gBattleMons[bank].ability;
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
pokemon_t* GetIllusionPartyData(u8 bank) {
	int i;
	u8 firstMonId, lastMonId;
	
	if (!(gStatuses3[bank] & STATUS3_ILLUSION))
		return GetBankPartyData(bank);
	
	//Wild Pokemon can't diguise themselves
	if ((!(gBattleTypeFlags & BATTLE_TYPE_TRAINER) && SIDE(bank) == B_SIDE_OPPONENT))
		return GetBankPartyData(bank);

	pokemon_t* party = LoadPartyRange(bank, &firstMonId, &lastMonId);
	
	for (i = lastMonId; i > firstMonId; --i) { //Loop through party in reverse order
		if (i == gBattlerPartyIndexes[bank])
			return GetBankPartyData(bank);
	
		if (GetMonData(&party[i], MON_DATA_SPECIES, 0) == PKMN_NONE
		||  GetMonData(&party[i], MON_DATA_HP, 0) == 0)
			continue;
		
		return &party[i];
	}
	
	return GetBankPartyData(bank);
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
	item_effect_t effect = gItems[SanitizeItemId(item)].holdEffect;
	u8 quality = ItemId_GetHoldEffectParam(item);
	evolution* evolutions = (evolution*) ((u32) evolution_table + party_data->species * sizeof(evolution) * (evos_per_poke + 1));
	int i;
	
	switch (effect) {
		case ITEM_EFFECT_Z_CRYSTAL:
			return FALSE;
			
		case ITEM_EFFECT_GRISEOUS_ORB:
			if (species == PKMN_GIRATINA || species == PKMN_GIRATINA_ORIGIN)
				return FALSE;
			break;
			
		case ITEM_EFFECT_PLATE:
			if (SpeciesToNationalPokedexNum(species) == PKDX_ARCEUS)
				return FALSE;
			break;
			
		case ITEM_EFFECT_MEMORY:
			if (SpeciesToNationalPokedexNum(species) == PKDX_SILVALLY)
				return FALSE;
			break;
			
		case ITEM_EFFECT_DRIVE:
			if (SpeciesToNationalPokedexNum(species) == PKDX_GENESECT)
				return FALSE;
			break;

		case ITEM_EFFECT_MEGA_STONE:
			for (i = 0; i <= evos_per_poke; ++i) {
				if (evolutions[i].type == MEGA_EVOLUTION && evolutions[i].argument == item)
					return FALSE;
			}
			break;
		
		case ITEM_EFFECT_PRIMAL_ORB:
			if ((quality == QUALITY_BLUE_ORB && (species == PKMN_KYOGRE  || species == PKMN_KYOGRE_PRIMAL))
			||  (quality == QUALITY_RED_ORB &&  (species == PKMN_GROUDON || species == PKMN_KYOGRE_PRIMAL)))
				return FALSE;
	}
	return TRUE;
}
//TODO: Put Gems (or not) and Berry reduction in flash bs command

//Make sure the input bank is any bank on the specific mon's side
bool8 CanFling(u8 ability, u16 item, pokemon_t* party_data, u8 bank, bool8 PartyCheck) {
	if (ability == ABILITY_KLUTZ
	|| MagicRoomTimer
	|| (!PartyCheck && EmbargoTimers[bank])
	|| !CanTransferItem(party_data->species, item, party_data)
	|| gItems[SanitizeItemId(item)].holdEffect == ITEM_EFFECT_PRIMAL_ORB
	|| IsMail(item)
	|| (GetPocketByItemId(item) == POCKET_BERRY_POUCH && AbilityBattleEffects(ABILITYEFFECT_CHECK_OTHER_SIDE, bank, ABILITY_UNNERVE, 0, 0))
	|| GetPocketByItemId(item) == POCKET_POKE_BALLS
	|| gItems[SanitizeItemId(item)].holdEffect == ITEM_EFFECT_GEM
	|| item == ITEM_ABILITY_CAPSULE)
		return FALSE;
	return TRUE;
}

bool8 SymbiosisCanActivate(u8 bank_to_give, u8 bank_to_receive) {
	item_t item = gBattleMons[bank_to_give].item;
	
	if (!(gBattleTypeFlags & BATTLE_TYPE_DOUBLE) 
	||  ABILITY(bank_to_give) != ABILITY_SYMBIOSIS
	||  ITEM(bank_to_receive) != 0
	|| !CanTransferItem(gBattleMons[bank_to_give].species, item, GetBankPartyData(bank_to_give)) 
	|| !CanTransferItem(gBattleMons[bank_to_receive].species, item, GetBankPartyData(bank_to_receive)))
		return FALSE;
	return TRUE;
}

//Sticky Hold also, but the boost ignores it
bool8 CanKnockOffItem(bank_t bank) {
	item_t item = gBattleMons[bank].item;
	if (!CanTransferItem(gBattleMons[bank].species, item, GetBankPartyData(bank))
	|| IsMail(item))
		return FALSE;
	return TRUE;
}

bool8 CanEvolve(pokemon_t* party_data) {
	int i;
	evolution* evolutions = (evolution*) ((u32) evolution_table + party_data->species * sizeof(evolution) * (evos_per_poke + 1));
	
	for (i = 0; i <= evos_per_poke; ++i) {
		if (evolutions[i].type != MEGA_EVOLUTION && evolutions[i].type != 0)
			return TRUE;
	}
	return FALSE;
}

bool8 CouldHaveEvolvedViaLevelUp(pokemon_t* mon) {
	int i;
	evolution* evolutions = (evolution*) ((u32) evolution_table + mon->species * sizeof(evolution) * (evos_per_poke + 1));
	
	for (i = 0; i <= evos_per_poke; ++i) {
		if (evolutions[i].type == EVO_LEVEL && mon->level >= evolutions[i].argument)
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
	
	return SPLIT(move);
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

bool8 CanBeGeneralStatused(u8 bank) {
	if (gBattleMons[bank].species == PKMN_MINIORSHIELD)
		return FALSE;
	
	switch (ABILITY(bank)) {
		case ABILITY_COMATOSE:
			return FALSE;
		
		case ABILITY_LEAFGUARD:
			if (WEATHER_HAS_EFFECT && gBattleWeather & WEATHER_SUN_ANY)
				return FALSE;
			break;
			
		case ABILITY_FLOWERVEIL:
			if (IsOfType(bank, TYPE_GRASS))
				return FALSE;
	}
	
	if (ABILITY(PARTNER(bank)) == ABILITY_FLOWERVEIL && IsOfType(bank, TYPE_GRASS))
		return FALSE;
	
	if (TerrainType == MISTY_TERRAIN && CheckGrounding(bank))
		return FALSE;

	return TRUE;
}


bool8 CanBePutToSleep(u8 bank) {
	if (!CanBeGeneralStatused(bank))
		return FALSE;
	
	switch (ABILITY(bank)) {
		case ABILITY_INSOMNIA:
		case ABILITY_VITALSPIRIT:
		case ABILITY_SWEETVEIL:
			return FALSE;
	}
	
	if (TerrainType == ELECTRIC_TERRAIN && CheckGrounding(bank))
		return FALSE;

	return TRUE;
}

bool8 CanBePoisoned(u8 bankDef, u8 bankAtk) {
	if (!CanBeGeneralStatused(bankDef))
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

bool8 CanBeParalyzed(u8 bank) {
	if (!CanBeGeneralStatused(bank))
		return FALSE;
	
	if (IsOfType(bank, TYPE_ELECTRIC))
		return FALSE;
		
	switch (ABILITY(bank)) {
		case ABILITY_LIMBER:
			return FALSE;
	}
	return TRUE;
}

bool8 CanBeBurned(u8 bank) {
	if (!CanBeGeneralStatused(bank))
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

bool8 CanBeFrozen(u8 bank) {
	if (!CanBeGeneralStatused(bank))
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

bool8 CanPartyMonBeGeneralStatused(pokemon_t* mon) {
	if (mon->species == PKMN_MINIORSHIELD)
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