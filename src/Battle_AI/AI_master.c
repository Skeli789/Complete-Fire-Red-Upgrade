#include "..\\defines.h"
#include "../../include/new/helper_functions.h"
#include "AI_Helper_Functions.h"
#include "../../include/random.h"

extern move_t MoldBreakerMoves[];

extern u8 TypeCalc(u16 move, u8 bankAtk, u8 bankDef, pokemon_t* party_data_atk, bool8 CheckParty);
extern u8 AI_TypeCalc(move_t, u8 bankAtk, pokemon_t* party_data_def);
extern u32 AI_CalcPartyDmg(u8 bankAtk, u8 bankDef, u16 move, pokemon_t* mon);
extern u8 CheckMoveLimitations(u8 bank, u8 unusableMoves, u8 check);
extern u32 GetAIFlagsInBattleFrontier(u8 bank);
extern const struct Evolution* CanMegaEvolve(u8 bank, bool8 CheckUBInstead);

void BattleAI_HandleItemUseBeforeAISetup(u8 defaultScoreMoves);
void BattleAI_SetupAIData(u8 defaultScoreMoves);

void AI_TrySwitchOrUseItem(void);
bool8 ShouldSwitch(void);
bool8 FindMonThatAbsorbsOpponentsMove(void);
bool8 FindMonThatAbsorbsOpponentsMove(void);
bool8 ShouldSwitchIfNaturalCureOrRegenerator(void);
bool8 HasSuperEffectiveMoveAgainstOpponents(bool8 noRng);
bool8 PassOnWish(void);
bool8 SemiInvulnerableTroll(void);
bool8 TheCalcForSemiInvulnerableTroll(u8 bankAtk, u8 flags, bool8 JustCheckLockedMoves);
bool8 CanStopLockedMove(void);
bool8 ShouldSwitchIfWonderGuard(void);
bool8 IsYawned(void);
bool8 FindMonWithFlagsAndSuperEffective(u8 flags, u8 moduloPercent);
u8 GetMostSuitableMonToSwitchInto(void);
u32 WildMonIsSmart(u8 bank);

struct SmartWildMons
{
	u16 species;
	u32 aiFlags;
};

struct SmartWildMons SmartWildAITable[] =
{
	{SPECIES_ARTICUNO, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_ZAPDOS, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_MOLTRES, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_MEWTWO, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_MEW, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_ENTEI, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_RAIKOU, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_SUICUNE, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_LUGIA, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_HO_OH, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_CELEBI, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_REGIROCK, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_REGICE, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_REGISTEEL, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_LATIAS, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_LATIOS, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_KYOGRE, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_GROUDON, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_RAYQUAZA, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_JIRACHI, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_DEOXYS, AI_SCRIPT_CHECK_BAD_MOVE},
	{0xFFFF, 0}
};

void BattleAI_HandleItemUseBeforeAISetup(u8 defaultScoreMoves)
{
    u32 i;
    u8 *data = (u8*)BATTLE_HISTORY;

    for (i = 0; i < sizeof(struct BattleHistory); i++)
        data[i] = 0;

    // Items are allowed to use in ONLY trainer battles.
    if ((gBattleTypeFlags & BATTLE_TYPE_TRAINER)
        && !(gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_SAFARI | BATTLE_TYPE_FRONTIER | BATTLE_TYPE_TWO_OPPONENTS | BATTLE_TYPE_INGAME_PARTNER | BATTLE_TYPE_EREADER_TRAINER))
		&& gTrainerBattleOpponent_A != 0x400)
    {
        for (i = 0; i < 4; i++)
        {
            if (gTrainers[gTrainerBattleOpponent_A].items[i] != 0)
            {
                BATTLE_HISTORY->trainerItems[BATTLE_HISTORY->itemsNo] = gTrainers[gTrainerBattleOpponent_A].items[i];
                BATTLE_HISTORY->itemsNo++;
            }
        }
    }

    BattleAI_SetupAIData(defaultScoreMoves);
}

void BattleAI_SetupAIData(u8 defaultScoreMoves)
{
    u32 i;
    u8* data = (u8*) AI_THINKING_STRUCT;
    u8 moveLimitations;

    // Clear AI data.
    for (i = 0; i < sizeof(struct AI_ThinkingStruct); ++i)
        data[i] = 0;

    // Conditional score reset, unlike Ruby.
    for (i = 0; i < MAX_MON_MOVES; ++i)
    {
        if (defaultScoreMoves & 1)
            AI_THINKING_STRUCT->score[i] = 100;
        else
            AI_THINKING_STRUCT->score[i] = 0;

        defaultScoreMoves >>= 1;
    }

    moveLimitations = CheckMoveLimitations(gActiveBattler, 0, 0xFF);

    // Ignore moves that aren't possible to use.
    for (i = 0; i < MAX_MON_MOVES; i++)
    {
        if (gBitTable[i] & moveLimitations)
            AI_THINKING_STRUCT->score[i] = 0;

        AI_THINKING_STRUCT->simulatedRNG[i] = 100 - umodsi(Random(), 16);
    }

    gBattleResources->AI_ScriptsStack->size = 0;
    gBankAttacker = gActiveBattler;

    // Decide a random target battlerId in doubles.
    if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
    {
        gBankTarget = (Random() & BIT_FLANK) + (GetBattlerSide(gActiveBattler) ^ BIT_SIDE);
        if (gAbsentBattlerFlags & gBitTable[gBankTarget])
            gBankTarget ^= BIT_FLANK;
    }
    // There's only one choice in single battles.
    else
        gBankTarget = gBankAttacker ^ BIT_SIDE;

    // Choose proper trainer ai scripts.
    if (gBattleTypeFlags & BATTLE_TYPE_SAFARI)
        AI_THINKING_STRUCT->aiFlags = AI_SCRIPT_SAFARI;
    else if (gBattleTypeFlags & BATTLE_TYPE_ROAMER)
        AI_THINKING_STRUCT->aiFlags = AI_SCRIPT_ROAMING | WildMonIsSmart(gActiveBattler);
    else if (gBattleTypeFlags & BATTLE_TYPE_OAK_TUTORIAL)
        AI_THINKING_STRUCT->aiFlags = AI_SCRIPT_FIRST_BATTLE;
	else if (gBattleTypeFlags & (BATTLE_TYPE_FRONTIER))
		AI_THINKING_STRUCT->aiFlags = GetAIFlagsInBattleFrontier(gActiveBattler);
    else if (gBattleTypeFlags & (BATTLE_TYPE_EREADER_TRAINER | BATTLE_TYPE_TRAINER_TOWER) && gTrainerBattleOpponent_A != 0x400)
        AI_THINKING_STRUCT->aiFlags = AI_SCRIPT_CHECK_BAD_MOVE | AI_SCRIPT_CHECK_VIABILITY | AI_SCRIPT_TRY_TO_FAINT;
    else if (gBattleTypeFlags & BATTLE_TYPE_SCRIPTED_WILD_2) //No idea how these two work
		AI_THINKING_STRUCT->aiFlags = AI_SCRIPT_CHECK_BAD_MOVE;
	else if (gBattleTypeFlags & BATTLE_TYPE_SCRIPTED_WILD_3) 
		AI_THINKING_STRUCT->aiFlags = AI_SCRIPT_CHECK_BAD_MOVE | AI_SCRIPT_CHECK_VIABILITY | AI_SCRIPT_TRY_TO_FAINT;
	else if (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS)
        AI_THINKING_STRUCT->aiFlags = gTrainers[gTrainerBattleOpponent_A].aiFlags | gTrainers[VarGet(SECOND_OPPONENT_VAR)].aiFlags;
    else
       AI_THINKING_STRUCT->aiFlags = gTrainers[gTrainerBattleOpponent_A].aiFlags;

    if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
        AI_THINKING_STRUCT->aiFlags |= AI_SCRIPT_DOUBLE_BATTLE; // act smart in doubles and don't attack your partner
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void AI_TrySwitchOrUseItem(void)
{
    struct Pokemon* party;
    u8 battlerIn1, battlerIn2;
    u8 firstId, lastId;

	party = LoadPartyRange(gActiveBattler, &firstId, &lastId);

    if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)
    {
        if (ShouldSwitch()) //0x8039A80
        {
            if (gBattleStruct->switchoutIndex[SIDE(gActiveBattler)] == PARTY_SIZE)
            {
                u8 monToSwitchId = GetMostSuitableMonToSwitchInto();
                if (monToSwitchId == PARTY_SIZE)
                {
                    if (!(gBattleTypeFlags & BATTLE_TYPE_DOUBLE))
                    {
                        battlerIn1 = gActiveBattler;
                        battlerIn2 = battlerIn1;
                    }
                    else
                    {
                        battlerIn1 = gActiveBattler;
						if (gAbsentBattlerFlags & gBitTable[PARTNER(gActiveBattler)])
							battlerIn2 = battlerIn1;
						else
							battlerIn2 = PARTNER(battlerIn1);
                    }

                    for (monToSwitchId = firstId; monToSwitchId < lastId; ++monToSwitchId)
                    {
                        if (party[monToSwitchId].hp == 0
						||  GetMonData(&party[monToSwitchId], MON_DATA_IS_EGG, 0)
						||  monToSwitchId == gBattlerPartyIndexes[battlerIn1]
						||	monToSwitchId == gBattlerPartyIndexes[battlerIn2]
						||	monToSwitchId == gBattleStruct->monToSwitchIntoId[battlerIn1]
						||	monToSwitchId == gBattleStruct->monToSwitchIntoId[battlerIn2])
                            continue;
						
                        break;
                    }
                }

                gBattleStruct->switchoutIndex[SIDE(gActiveBattler)] = monToSwitchId;
            }

            gBattleStruct->monToSwitchIntoId[gActiveBattler] = gBattleStruct->switchoutIndex[SIDE(gActiveBattler)];				
            return;
        }
        else if (ShouldUseItem()) //0x803A1F4
            return;
    }

    EmitTwoReturnValues(1, ACTION_USE_MOVE, (gActiveBattler ^ BIT_SIDE) << 8);
}

bool8 ShouldSwitch(void)
{
    u8 battlerIn1, battlerIn2;
    u8 firstId, lastId;
	u8 availableToSwitch;
    struct Pokemon *party;
    int i;

	if (!IsOfType(gActiveBattler, TYPE_GHOST) && ITEM_EFFECT(gActiveBattler) != ITEM_EFFECT_SHED_SHELL) {
		if (gBattleMons[gActiveBattler].status2 & (STATUS2_WRAPPED | STATUS2_ESCAPE_PREVENTION))
			return FALSE;
		if (gStatuses3[gActiveBattler] & STATUS3_ROOTED)
			return FALSE;
		if (ABILITY_ON_OPPOSING_FIELD(gActiveBattler, ABILITY_SHADOWTAG) && ABILITY(gActiveBattler) != ABILITY_SHADOWTAG)
			return FALSE;
		if (ABILITY_ON_OPPOSING_FIELD(gActiveBattler, ABILITY_ARENATRAP) && CheckGrounding(gActiveBattler))
			return FALSE;
		if (ABILITY_ON_FIELD2(ABILITY_MAGNETPULL) && IsOfType(gActiveBattler, TYPE_STEEL))
			return FALSE;
		if (gNewBS->FairyLockTimer)
			return FALSE;
	}
	if (gStatuses3[gActiveBattler] & STATUS3_SKY_DROP_TARGET)
		return FALSE;
	
    availableToSwitch = 0;
    if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
    {
        battlerIn1 = gActiveBattler;
        if (gAbsentBattlerFlags & gBitTable[GetBattlerAtPosition(GetBattlerPosition(gActiveBattler) ^ BIT_FLANK)])
            battlerIn2 = gActiveBattler;
        else
            battlerIn2 = GetBattlerAtPosition(GetBattlerPosition(gActiveBattler) ^ BIT_FLANK);
    }
    else
    {
        battlerIn1 = gActiveBattler;
        battlerIn2 = gActiveBattler;
    }

    party = LoadPartyRange(gActiveBattler, &firstId, &lastId);

    for (i = firstId; i < lastId; ++i)
    {
        if (party[i].hp == 0
        ||	GetMonData(&party[i], MON_DATA_SPECIES2, 0) == SPECIES_NONE
        || 	GetMonData(&party[i], MON_DATA_IS_EGG, 0)
        ||	i == gBattlerPartyIndexes[battlerIn1]
        ||	i == gBattlerPartyIndexes[battlerIn2]
        ||	i == gBattleStruct->monToSwitchIntoId[battlerIn1]
        ||  i == gBattleStruct->monToSwitchIntoId[battlerIn2])
            continue;

        ++availableToSwitch;
    }
	
    if (availableToSwitch == 0)
        return FALSE;
    if (ShouldSwitchIfPerishSong())
        return TRUE;
    if (ShouldSwitchIfWonderGuard())
        return TRUE;
    if (FindMonThatAbsorbsOpponentsMove())
        return TRUE;
    if (ShouldSwitchIfNaturalCureOrRegenerator())
      { return TRUE; }
	if (PassOnWish())
		return TRUE;
	if (SemiInvulnerableTroll())
		return TRUE;
	if (CanStopLockedMove())
		return TRUE;
	if (IsYawned())
		return TRUE;
    if (CanKillAFoe(gActiveBattler))
        return FALSE;
    if (AreStatsRaised())
        return FALSE;
    if (FindMonWithFlagsAndSuperEffective(MOVE_RESULT_DOESNT_AFFECT_FOE, 1)
    ||  FindMonWithFlagsAndSuperEffective(MOVE_RESULT_NOT_VERY_EFFECTIVE, 2))
        return TRUE;

    return FALSE;
}

bool8 FindMonThatAbsorbsOpponentsMove(void)
{
    u8 battlerIn1, battlerIn2;
    u8 absorbingTypeAbility1, absorbingTypeAbility2, absorbingTypeAbility3;
    u8 firstId, lastId;
    struct Pokemon *party;
    int i;

    if ((CanKillAFoe(gActiveBattler) && umodsi(Random(), 2) != 0)
    || 	gLastLandedMoves[gActiveBattler] == 0
    ||	gLastLandedMoves[gActiveBattler] == 0xFFFF
    ||	gBattleMoves[gLastLandedMoves[gActiveBattler]].power == 0)
        return FALSE;

    if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
    {
        battlerIn1 = gActiveBattler;
        if (gAbsentBattlerFlags & gBitTable[PARTNER(gActiveBattler)])
            battlerIn2 = gActiveBattler;
        else
            battlerIn2 = PARTNER(gActiveBattler);
    }
    else
    {
        battlerIn1 = gActiveBattler;
        battlerIn2 = gActiveBattler;
    }

	switch (gNewBS->LastUsedTypes[gLastHitBy[gActiveBattler]]) {
		case TYPE_FIRE:
			absorbingTypeAbility1 = ABILITY_FLASHFIRE;
			absorbingTypeAbility2 = ABILITY_FLASHFIRE;
			absorbingTypeAbility3 = ABILITY_FLASHFIRE;
			break;
		case TYPE_ELECTRIC:
			absorbingTypeAbility1 = ABILITY_VOLTABSORB;
			absorbingTypeAbility2 = ABILITY_LIGHTNINGROD;
			absorbingTypeAbility3 = ABILITY_MOTORDRIVE;
			break;
		case TYPE_WATER:
			absorbingTypeAbility1 = ABILITY_WATERABSORB;
			absorbingTypeAbility2 = ABILITY_DRYSKIN;
			absorbingTypeAbility3 = ABILITY_STORMDRAIN;
			break;
		case TYPE_GRASS:
			absorbingTypeAbility1 = ABILITY_SAPSIPPER;
			absorbingTypeAbility2 = ABILITY_SAPSIPPER;
			absorbingTypeAbility3 = ABILITY_SAPSIPPER;
			break;
		default:
			return FALSE;
	}
	
	if (ABILITY(gActiveBattler) == absorbingTypeAbility1
	||  ABILITY(gActiveBattler) == absorbingTypeAbility2
	||  ABILITY(gActiveBattler) == absorbingTypeAbility3)
		return FALSE;

	party = LoadPartyRange(gActiveBattler, &firstId, &lastId);

    for (i = firstId; i < lastId; i++)
    {
        u16 species = party[i].species;
        u8 monAbility = GetPartyAbility(&party[i]);

        if (party[i].hp == 0
        ||  species == SPECIES_NONE
        ||	GetMonData(&party[i], MON_DATA_IS_EGG, 0)
        ||	i == gBattlerPartyIndexes[battlerIn1]
        ||	i == gBattlerPartyIndexes[battlerIn2]
        ||	i == gBattleStruct->monToSwitchIntoId[battlerIn1]
        ||	i == gBattleStruct->monToSwitchIntoId[battlerIn2])
            continue;

        if (monAbility == absorbingTypeAbility1
		||  monAbility == absorbingTypeAbility2
		||  monAbility == absorbingTypeAbility3)
        {
            // we found a mon.
            gBattleStruct->switchoutIndex[SIDE(gActiveBattler)] = i;
            EmitTwoReturnValues(1, ACTION_SWITCH, 0);
            return TRUE;
        }
    }

    return FALSE;
}

bool8 ShouldSwitchIfNaturalCureOrRegenerator(void)
{
	switch (ABILITY(gActiveBattler)) {
		case ABILITY_NATURALCURE:
			if (gBattleMons[gActiveBattler].status1 & (STATUS1_SLEEP | STATUS1_FREEZE))
				break;
			if (gBattleMons[gActiveBattler].hp < gBattleMons[gActiveBattler].maxHP / 2)
				return FALSE;
			break;
		//Try switch if less than half health
		case ABILITY_REGENERATOR:
			if (gBattleMons[gActiveBattler].hp > gBattleMons[gActiveBattler].maxHP / 2)
				return FALSE;
			break;
		
		default:
			return FALSE;
	}

    if (FindMonWithFlagsAndSuperEffective(MOVE_RESULT_DOESNT_AFFECT_FOE, 1))
        return TRUE;
    if (FindMonWithFlagsAndSuperEffective(MOVE_RESULT_NOT_VERY_EFFECTIVE, 1))
        return TRUE;

    gBattleStruct->switchoutIndex[SIDE(gActiveBattler)] = PARTY_SIZE;
    EmitTwoReturnValues(1, ACTION_SWITCH, 0);
    return TRUE;
}

bool8 HasSuperEffectiveMoveAgainstOpponents(bool8 noRng) {
    u8 opposingBattler;
    int i;
    u8 moveFlags;
    u16 move;

    opposingBattler = FOE(gActiveBattler);
    if (!(gAbsentBattlerFlags & gBitTable[opposingBattler])) {
	
        for (i = 0; i < 4; i++) {
            move = gBattleMons[gActiveBattler].moves[i];
            if (move == MOVE_NONE || SPLIT(move) == SPLIT_STATUS)
                continue;

            moveFlags = TypeCalc(move, gActiveBattler, opposingBattler, 0, FALSE);
            
			if (moveFlags & MOVE_RESULT_SUPER_EFFECTIVE) {
                if (noRng)
                    return TRUE;
                if (umodsi(Random(), 10) != 0)
                    return TRUE;
            }
        }
    }

    if (!(gBattleTypeFlags & BATTLE_TYPE_DOUBLE))
        return FALSE;

    opposingBattler = PARTNER(FOE(gActiveBattler));
    if (!(gAbsentBattlerFlags & gBitTable[opposingBattler])) {
        
		for (i = 0; i < 4; ++i) {
            move = gBattleMons[gActiveBattler].moves[i];
            if (move == MOVE_NONE || SPLIT(move) == SPLIT_STATUS)
                continue;

            moveFlags = TypeCalc(move, gActiveBattler, opposingBattler, 0, FALSE);
			
            if (moveFlags & MOVE_RESULT_SUPER_EFFECTIVE) {
                if (noRng)
                    return TRUE;
                if (umodsi(Random(), 10) != 0)
                    return TRUE;
            }
        }
    }

    return FALSE;
}

bool8 PassOnWish(void) {
	pokemon_t* party;
	u8 firstId, lastId;
	u8 bank = gActiveBattler;
	u8 battlerIn1, battlerIn2;
	u8 opposingBattler1, opposingBattler2;
	int i;
	
    if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
    {
		opposingBattler1 = FOE(bank);
		opposingBattler2 = PARTNER(opposingBattler1);
        battlerIn1 = gActiveBattler;
        if (gAbsentBattlerFlags & gBitTable[PARTNER(gActiveBattler)])
            battlerIn2 = gActiveBattler;
        else
            battlerIn2 = PARTNER(gActiveBattler);
    }
    else
    {
		opposingBattler1 = FOE(bank);
		opposingBattler2 = opposingBattler1;
        battlerIn1 = gActiveBattler;
        battlerIn2 = gActiveBattler;
    }
		
	if (gWishFutureKnock->wishCounter[bank]) {
		
		//Don't switch if your health is less than half and you can survive an opponent's hit
		if (gBattleMons[bank].hp < gBattleMons[bank].maxHP / 2
		&& ((!CanKnockOut(opposingBattler1, bank) && !(gBattleTypeFlags & BATTLE_TYPE_DOUBLE && CanKnockOut(opposingBattler2, bank))) || HasProtectionMoveInMoveset(bank, FALSE)))
			return FALSE;
			
		party = LoadPartyRange(gActiveBattler, &firstId, &lastId);
		
		for (i = firstId; i < lastId; ++i) {
			if (party[i].hp == 0
			||  party[i].species == SPECIES_NONE
			||	GetMonData(&party[i], MON_DATA_IS_EGG, 0)
			||	i == gBattlerPartyIndexes[battlerIn1]
			||	i == gBattlerPartyIndexes[battlerIn2]
			||	i == gBattleStruct->monToSwitchIntoId[battlerIn1]
			||	i == gBattleStruct->monToSwitchIntoId[battlerIn2])
				continue;

			if (party[i].hp < party[i].maxHP / 2) {
				gBattleStruct->switchoutIndex[SIDE(bank)] = i;
                EmitTwoReturnValues(1, ACTION_SWITCH, 0);
				return TRUE;
			}
		}
	}
	return FALSE;
}

bool8 SemiInvulnerableTroll(void) {
	u8 opposingBattler1, opposingBattler2;

	if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE) {
		opposingBattler1 = FOE(gActiveBattler);
		opposingBattler2 = PARTNER(opposingBattler1);
	}
	else {
		opposingBattler1 = FOE(gActiveBattler);
		opposingBattler2 = opposingBattler1;
	}
	
	if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE) {
		if (!(gStatuses3[opposingBattler1] & STATUS3_SEMI_INVULNERABLE) && !(gStatuses3[opposingBattler2] & STATUS3_SEMI_INVULNERABLE))
			return FALSE;
		#ifndef REALLY_SMART_AI
			if (ViableMonCountFromBank(gActiveBattler) > 1)
				return FALSE; //AI doesn't know which mon is being targeted if there's more than one on the field
		#endif
	}
	else if (!(gStatuses3[opposingBattler1] & STATUS3_SEMI_INVULNERABLE))
		return FALSE;
	
	if (TheCalcForSemiInvulnerableTroll(opposingBattler1, MOVE_RESULT_DOESNT_AFFECT_FOE, FALSE))
		return TRUE;
	if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE && TheCalcForSemiInvulnerableTroll(opposingBattler2, MOVE_RESULT_DOESNT_AFFECT_FOE, FALSE))
		return TRUE;
	if (TheCalcForSemiInvulnerableTroll(opposingBattler1, MOVE_RESULT_NOT_VERY_EFFECTIVE, FALSE))
		return TRUE;
	if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE && TheCalcForSemiInvulnerableTroll(opposingBattler2, MOVE_RESULT_NOT_VERY_EFFECTIVE, FALSE))
		return TRUE;
	
	return FALSE;
}

bool8 TheCalcForSemiInvulnerableTroll(u8 bankAtk, u8 flags, bool8 JustCheckLockedMoves) {
	u8 firstId, lastId;
	u8 battlerIn1, battlerIn2;
	int i;

    if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
    {
        battlerIn1 = gActiveBattler;
        if (gAbsentBattlerFlags & gBitTable[PARTNER(gActiveBattler)])
            battlerIn2 = gActiveBattler;
        else
            battlerIn2 = PARTNER(gActiveBattler);
    }
    else
    {
        battlerIn1 = gActiveBattler;
        battlerIn2 = gActiveBattler;
    }
	
	pokemon_t* party = LoadPartyRange(gActiveBattler, &firstId, &lastId);
	
	if (((gStatuses3[bankAtk] & STATUS3_SEMI_INVULNERABLE) || (JustCheckLockedMoves && gLockedMoves[bankAtk])) 
	&&    gBattleStruct->moveTarget[bankAtk] == gActiveBattler) {
		if (TypeCalc(gLockedMoves[bankAtk], bankAtk, gActiveBattler, 0, FALSE) & flags)
			return FALSE;
			
		for (i = firstId; i < lastId; ++i) {
			if (party[i].hp == 0
			||  party[i].species == SPECIES_NONE
			||	GetMonData(&party[i], MON_DATA_IS_EGG, 0)
			||	i == gBattlerPartyIndexes[battlerIn1]
			||	i == gBattlerPartyIndexes[battlerIn2]
			||	i == gBattleStruct->monToSwitchIntoId[battlerIn1]
			||	i == gBattleStruct->monToSwitchIntoId[battlerIn2])
				continue;

			if (AI_TypeCalc(gLockedMoves[bankAtk], bankAtk, &party[i]) & flags) {
				gBattleStruct->switchoutIndex[SIDE(gActiveBattler)] = i;
                EmitTwoReturnValues(1, ACTION_SWITCH, 0);
				return TRUE;
			}
		}
	}
	return FALSE;
}

bool8 CanStopLockedMove(void) {
	u8 opposingBattler1, opposingBattler2;

	if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE) {
		opposingBattler1 = FOE(gActiveBattler);
		opposingBattler2 = PARTNER(opposingBattler1);
	}
	else {
		opposingBattler1 = FOE(gActiveBattler);
		opposingBattler2 = opposingBattler1;
	}
	
	if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE) {
		if (!(gLockedMoves[opposingBattler1] && SPLIT(gLockedMoves[opposingBattler1]) != SPLIT_STATUS) && !(gLockedMoves[opposingBattler2] && SPLIT(gLockedMoves[opposingBattler2]) != SPLIT_STATUS))
			return FALSE;
		#ifndef REALLY_SMART_AI
			if (ViableMonCountFromBank(gActiveBattler) > 1)
				return FALSE; //AI doesn't know which mon is being targeted if there's more than one on the field
		#endif
	}
	else if (!(gLockedMoves[opposingBattler1] && SPLIT(gLockedMoves[opposingBattler1]) != SPLIT_STATUS))
		return FALSE;
		
	if (TheCalcForSemiInvulnerableTroll(opposingBattler1, MOVE_RESULT_DOESNT_AFFECT_FOE, TRUE))
		return TRUE;
	if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE && TheCalcForSemiInvulnerableTroll(opposingBattler2, MOVE_RESULT_DOESNT_AFFECT_FOE, TRUE))
		return TRUE;
	if (TheCalcForSemiInvulnerableTroll(opposingBattler1, MOVE_RESULT_NOT_VERY_EFFECTIVE, TRUE))
		return TRUE;
	if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE && TheCalcForSemiInvulnerableTroll(opposingBattler2, MOVE_RESULT_NOT_VERY_EFFECTIVE, TRUE))
		return TRUE;
	
	return FALSE;
}

bool8 IsYawned(void) {
	if (ABILITY(gActiveBattler) != ABILITY_NATURALCURE && gStatuses3[gActiveBattler] & STATUS3_YAWN && gBattleMons[gActiveBattler].hp > gBattleMons[gActiveBattler].maxHP / 4) {
		if (FindMonWithFlagsAndSuperEffective(MOVE_RESULT_DOESNT_AFFECT_FOE, 1))
			return TRUE;
		if (FindMonWithFlagsAndSuperEffective(MOVE_RESULT_NOT_VERY_EFFECTIVE, 1))
			return TRUE;

		gBattleStruct->switchoutIndex[SIDE(gActiveBattler)] = PARTY_SIZE;
		EmitTwoReturnValues(1, ACTION_SWITCH, 0);
		return TRUE;
	}
	return FALSE;
}

bool8 FindMonWithFlagsAndSuperEffective(u8 flags, u8 moduloPercent) {
    u8 battlerIn1, battlerIn2;
    int i, j;
	u8 start, end;
    u16 move;
    u8 moveFlags;

    if (gLastLandedMoves[gActiveBattler] == 0)
        return FALSE;
    if (gLastLandedMoves[gActiveBattler] == 0xFFFF)
        return FALSE;
    if (gLastHitBy[gActiveBattler] == 0xFF)
        return FALSE;
    if (gBattleMoves[gLastLandedMoves[gActiveBattler]].power == 0)
        return FALSE;

    if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
    {
        battlerIn1 = gActiveBattler;
        if (gAbsentBattlerFlags & gBitTable[PARTNER(gActiveBattler)])
            battlerIn2 = gActiveBattler;
        else
            battlerIn2 = PARTNER(gActiveBattler);
    }
    else
    {
        battlerIn1 = gActiveBattler;
        battlerIn2 = gActiveBattler;
    }

	pokemon_t* party = LoadPartyRange(gActiveBattler, &start, &end);


//Party Search			
    for (i = start; i < end; ++i) {
        if (party[i].hp == 0)
            continue;
        if (party[i].species == SPECIES_NONE)
            continue;
        if (GetMonData(&party[i], MON_DATA_IS_EGG, 0))
            continue;
        if (i == gBattlerPartyIndexes[battlerIn1])
            continue;
        if (i == gBattlerPartyIndexes[battlerIn2])
            continue;
        if (i == gBattleStruct->monToSwitchIntoId[battlerIn1])
            continue;
        if (i == gBattleStruct->monToSwitchIntoId[battlerIn2])
            continue;
					
        moveFlags = AI_TypeCalc(gLastLandedMoves[gActiveBattler], gLastHitBy[gActiveBattler], &party[i]);
		
        if (moveFlags & flags) {
            battlerIn1 = gLastHitBy[gActiveBattler];

            for (j = 0; j < 4; j++) {
                move = party[i].moves[j];
                if (move == 0 || SPLIT(move) == SPLIT_STATUS)
                    continue;
							
                moveFlags = TypeCalc(move, gActiveBattler, battlerIn1, &party[i], TRUE);
				
                if (moveFlags & MOVE_RESULT_SUPER_EFFECTIVE && umodsi(Random(), moduloPercent) == 0) {
                    gBattleStruct->switchoutIndex[SIDE(gActiveBattler)] = i; //There was a GetBattlerPosition here but its a pretty useless function
                    EmitTwoReturnValues(1, ACTION_SWITCH, 0);
                    return TRUE;
                }
            }
        }
    }

    return FALSE;
}

bool8 ShouldSwitchIfWonderGuard(void) {
	u8 battlerIn1, battlerIn2;
    u8 opposingBattler;
    u8 moveFlags;
    int i, j;
	u8 start, end;
	opposingBattler = FOE(gActiveBattler);

    if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
        return FALSE;

    if (ABILITY(opposingBattler) != ABILITY_WONDERGUARD)
        { return FALSE; } //Curly braces are here becuase dumb compiler was giving me errors without them
		
	if (ABILITY(gActiveBattler) == ABILITY_MOLDBREAKER
	||  ABILITY(gActiveBattler) == ABILITY_TERAVOLT
	||  ABILITY(gActiveBattler) == ABILITY_TURBOBLAZE)
		return FALSE;
	
	//Check Mega Evolving into a mon with Mold Breaker
	const struct Evolution* evos = CanMegaEvolve(gActiveBattler, FALSE);
	if (evos != NULL)
	{
		u16 megaSpecies = evos->targetSpecies;
		u8 megaAbility = gBaseStats[megaSpecies].ability1; //Megas can only have 1 ability
		
		if (megaAbility == ABILITY_MOLDBREAKER
		||  megaAbility == ABILITY_TERAVOLT
		||  megaAbility == ABILITY_TURBOBLAZE)
			return FALSE;
	}

    //Check if pokemon has a super effective move or Mold Breaker Move
    for (i = 0; i < 4; i++) {
        u16 move = gBattleMons[gActiveBattler].moves[i];
        if (move == MOVE_NONE)
            continue;
			
		if (CheckTableForMove(move, MoldBreakerMoves))
			return FALSE;
			
		if (SPLIT(move) == SPLIT_STATUS)
			continue;
					
        moveFlags = TypeCalc(move, gActiveBattler, opposingBattler, 0, 0);
        if (moveFlags & MOVE_RESULT_SUPER_EFFECTIVE)
            return FALSE;
    }

    if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
    {
        battlerIn1 = gActiveBattler;
        if (gAbsentBattlerFlags & gBitTable[PARTNER(gActiveBattler)])
            battlerIn2 = gActiveBattler;
        else
            battlerIn2 = PARTNER(gActiveBattler);
    }
    else
    {
        battlerIn1 = gActiveBattler;
        battlerIn2 = gActiveBattler;
    }
	
	pokemon_t* party = LoadPartyRange(gActiveBattler, &start, &end);
	
    // find a pokemon in the party that has a super effective move
    for (i = start; i < end; i++) {
        if (party[i].hp == 0
            || party[i].species == SPECIES_NONE
            || GetMonData(&party[i], MON_DATA_IS_EGG, 0)
            || i == gBattlerPartyIndexes[battlerIn1]
			|| i == gBattlerPartyIndexes[battlerIn2]
			|| i == gBattleStruct->monToSwitchIntoId[battlerIn1]
            || i == gBattleStruct->monToSwitchIntoId[battlerIn2])
            continue;

        for (j = 0; j < 4; j++) {
            u16 move = party[i].moves[j];
            if (move == MOVE_NONE || SPLIT(move) == SPLIT_STATUS)
                continue;

            moveFlags = TypeCalc(move, gActiveBattler, opposingBattler, &party[i], TRUE);
            if (moveFlags & MOVE_RESULT_SUPER_EFFECTIVE) {
                // we found a mon
                gBattleStruct->switchoutIndex[SIDE(gActiveBattler)] = i;
                EmitTwoReturnValues(1, ACTION_SWITCH, 0);
                return TRUE;
            }
        }
    }

    return FALSE; // at this point there is not a single pokemon in the party that has a super effective move against a pokemon with wonder guard
}

u8 GetMostSuitableMonToSwitchInto(void)
{
    u8 opposingBattler;
    u32 bestDmg;
    u8 bestMonId;
    u8 battlerIn1, battlerIn2;
    u8 firstId, lastId; // + 1
    pokemon_t* party;
    int i, j;
    u8 invalidMons;
    u16 move;

    if (gBattleStruct->monToSwitchIntoId[gActiveBattler] != PARTY_SIZE)
        return gBattleStruct->monToSwitchIntoId[gActiveBattler];

    if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
    {
        battlerIn1 = gActiveBattler;
        if (gAbsentBattlerFlags & gBitTable[PARTNER(gActiveBattler)])
            battlerIn2 = battlerIn1;
        else
            battlerIn2 = PARTNER(battlerIn1);
		
		opposingBattler = (Random() & BIT_FLANK) ^ SIDE(battlerIn1);
			
        if (gAbsentBattlerFlags & gBitTable[opposingBattler])
            opposingBattler ^= BIT_FLANK;
    }
    else
    {
		opposingBattler = GetBattlerAtPosition(GetBattlerPosition(gActiveBattler) ^ BIT_SIDE);
        battlerIn1 = gActiveBattler;
        battlerIn2 = gActiveBattler;
    }
	
	party = LoadPartyRange(gActiveBattler, &firstId, & lastId);
	
	//Check if point even running calcs
	u8 count = 0, potentialMon = 0;
	for (i = firstId; i < lastId; ++i) {
		if (party[i].species != SPECIES_NONE
		&& !GetMonData(&party[i], MON_DATA_IS_EGG, 0)
        && party[i].hp
        && gBattlerPartyIndexes[battlerIn1] != i
        && gBattlerPartyIndexes[battlerIn2] != i
		&& i != gBattleStruct->monToSwitchIntoId[battlerIn1]
        && i != gBattleStruct->monToSwitchIntoId[battlerIn2])
		{
			++count;
			potentialMon = i;
		}
	}
	
	if (count == 0)
		return PARTY_SIZE;
	else if (count == 1) //Only one poke remaining
		return potentialMon;
	
    invalidMons = 0;

    while (invalidMons != 0x3F) // All mons are invalid.
    {
        bestDmg = 0;
        bestMonId = PARTY_SIZE;
		
        // Find the mon whose type is the most suitable offensively.
        for (i = firstId; i < lastId; i++)
        {
            u16 species = party[i].species;
            if (species != SPECIES_NONE
                && party[i].hp != 0
                && !(gBitTable[i] & invalidMons)
                && gBattlerPartyIndexes[battlerIn1] != i
                && gBattlerPartyIndexes[battlerIn2] != i
                && i != gBattleStruct->monToSwitchIntoId[battlerIn1]
                && i != gBattleStruct->monToSwitchIntoId[battlerIn2])
            {
                u8 type1 = gBaseStats[species].type1;
                u8 type2 = gBaseStats[species].type2;
                u8 typeDmg = 10;
                ModulateByTypeEffectiveness(gBattleMons[opposingBattler].type1, type1, type2, &typeDmg);
                ModulateByTypeEffectiveness(gBattleMons[opposingBattler].type2, type1, type2, &typeDmg);
				ModulateByTypeEffectiveness(gBattleMons[opposingBattler].type3, type1, type2, &typeDmg);
                if (bestDmg < typeDmg)
                {
                    bestDmg = typeDmg;
                    bestMonId = i;
                }
            }
            else
            {
                invalidMons |= gBitTable[i];
            }
        }

        // Ok, we know the mon has the right typing but does it have at least one super effective move?
        if (bestMonId != PARTY_SIZE)
        {
            for (i = 0; i < MAX_MON_MOVES; i++)
            {
                move = GetMonData(&party[bestMonId], MON_DATA_MOVE1 + i, 0);
                if (move != MOVE_NONE 
				&& SPLIT(move) != SPLIT_STATUS 
				&& TypeCalc(move, gActiveBattler, opposingBattler, &party[bestMonId], TRUE) & MOVE_RESULT_SUPER_EFFECTIVE)
                    break;
            }

            if (i != MAX_MON_MOVES)
                return bestMonId; // Has both the typing and at least one super effective move.

            invalidMons |= gBitTable[bestMonId]; // Sorry buddy, we want something better.
        }
        else
        {
            invalidMons = 0x3F; // No viable mon to switch.
        }
    }

    gMoveResultFlags = 0;
    bestDmg = 0;
    bestMonId = 6;

    // If we couldn't find the best mon in terms of typing, find the one that deals most damage.
    for (i = firstId; i < lastId; i++)
    {
        if (party[i].species == SPECIES_NONE
        || party[i].hp == 0
        || gBattlerPartyIndexes[battlerIn1] == i
        || gBattlerPartyIndexes[battlerIn2] == i
        || i == gBattleStruct->monToSwitchIntoId[battlerIn1]
        || i == gBattleStruct->monToSwitchIntoId[battlerIn2])
            continue;

        for (j = 0; j < MAX_MON_MOVES; j++)
        {
            move = GetMonData(&party[i], MON_DATA_MOVE1 + j, 0);
            gBattleMoveDamage = 0;
            if (move != MOVE_NONE && SPLIT(move) != SPLIT_STATUS)
            {	
				gBattleMoveDamage = AI_CalcPartyDmg(gActiveBattler, opposingBattler, move, &party[i]);
				TypeCalc(move, gActiveBattler, opposingBattler, &party[i], TRUE);
            }
            if (bestDmg < (u32) gBattleMoveDamage)
            {
                bestDmg = gBattleMoveDamage;
                bestMonId = i;
            }
        }
    }

    return bestMonId;
}

u32 WildMonIsSmart(u8 bank) {
	#ifdef WILD_ALWAYS_SMART
		bank++; //So no compiler errors
		return TRUE;
	#else
		u16 species = gBattleMons[bank].species;
		for (u32 i = 0; SmartWildAITable[i].species != 0xFFFF; ++i) {
			if (SmartWildAITable[i].species == species)
				return SmartWildAITable[i].aiFlags;
		}
		return FALSE;
	#endif
}

/*
u32 AIAnalyzeMoveset(u8 bank) {
	u8* moveArray = gBattleMons[bank].moves;
	u16 move;
	u8 moveEffect;
	u8 damagingMoves, statusyMoves, hazardMoves, healingMoves, statBoostingMoves, supportMoves;
	u32 aiFlags;
	int i;
	
	for (i = 0; i < MAX_MON_MOVES && moveArray[i] != 0 && moveArray[i] < MOVES_COUNT; ++i) 
	{
		move = moveArray[i];
		moveEffect = gBattleMoves[move].effect;
		
		if (moveEffect == EFFECT_SPIKES)
			aiFlags |= AI_SCRIPT_SET_HAZARDS;
			
		switch (moveEffect) {
			case EFFECT_SLEEP:
			case EFFECT_TOXIC:
			case EFFECT_POISON:
			case EFFECT_PARALYZE:
			case EFFECT_LEECH_SEED:
			case EFFECT_ATTRACT:
			case EFFECT_WILL_O_WISP:
				aiFlags |= AI_SCRIPT_SET_STATUS;
		}
		
		switch (moveEffect) {
			case EFFECT_ABSORB
			case EFFECT_RESTORE_HP:
			case EFFECT_REST:
			case EFFECT_LEECH_SEED:
			case EFFECT_MORNING_SUN:
			case EFFECT_SOFTBOILED:
			case EFFECT_WISH:
			case EFFECT_INGRAIN:
				aiFlags |= AI_SCRIPT_HEAL;
		}
		
		if (gBattleMoveFlags & BATTLE_TYPE_DOUBLE) {
			switch (moveEffect) {
				case EFFECT_HELPING_HAND:
				case EFFECT_HEAL_TARGET: //Includes Pollen Puff
					aiFlags |= AI_SCRIPT_HELP_PARTNER;
			}
		}
			case EFFECT_ATTACK_UP:
			case EFFECT_DEFENSE_UP:
			case EFFECT_SPEED_UP:
			case EFFECT_SPECIAL_ATTACK_UP:
			case EFFECT_SPECIAL_DEFENSE_UP:
			case EFFECT_ACCURACY_UP:
			case EFFECT_EVASION_UP:
				

			
		}
	}
}
*/