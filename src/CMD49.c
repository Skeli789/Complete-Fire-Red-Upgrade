#include "defines.h"
#include "helper_functions.h"

//Add some way to check if Life Orb did damage to either foe
//Make sure there's no choice lock glitch
//Add check to see if AI move prediction was successful. If not, then if the same move is predicted, don't predict that same move again.
//Remove the lines at the bottom?
//Put Soul-Heart somewhere
//Make Red Card its own thing

#define BattleScript_RageIsBuilding (u8*) 0x81D8C48
#define BattleScript_DefrostedViaFireMove (u8*) 0x81D9098
#define BattleScript_FlushMessageBox (u8*) 0x81D96A8
#define BattleScript_Recoil (u8*) 0x81D9243

extern move_t MovesCanUnfreezeTarget[];
extern move_t DanceMoveTable[];
extern move_t TwoToFiveStrikesMoves[];
extern move_t TwoStrikesMoves[];
extern move_t Percent25RecoilMoves[];
extern move_t Percent33RecoilMoves[];
extern move_t Percent50RecoilMoves[];
extern move_t Percent66RecoilMoves[];
extern move_t Percent75RecoilMoves[];
extern move_t Percent100RecoilMoves[];

extern u8 BattleScript_PoisonTouch[];
extern u8 BattleScript_BeakBlastBurn[];
extern u8 BattleScript_Magician[];
extern u8 BattleScript_Moxie[];
extern u8 BattleScript_MindBlownDamage[];
extern u8 BattleScript_LifeOrbDamage[];
extern u8 BattleScript_Pickpocket[];
extern u8 BattleScript_DancerActivated[];
extern u8 BattleScript_MultiHitPrintStrings[];
extern u8 BattleScript_FaintAttacker[];
extern u8 BattleScript_ExplosionAnim[];

extern u32 SpeedCalc(u8 bank);

extern u8* gBattleScriptsForMoveEffects[];

enum
{
	//Add in Thief Effect call
	ATK49_ATTACKER_ABILITIES,
    ATK49_RAGE,
    ATK49_DEFROST,
    ATK49_SYNCHRONIZE_TARGET,
    ATK49_MOVE_END_ABILITIES,
	ATK49_BEAK_BLAST_BURN,
    ATK49_SYNCHRONIZE_ATTACKER,
    ATK49_CHOICE_MOVE,
    /*ATK49_CHANGED_ITEMS,*/
    ATK49_ATTACKER_INVISIBLE,
	ATK49_TARGET_INVISIBLE,
    ATK49_ATTACKER_VISIBLE,
    ATK49_TARGET_VISIBLE,
	ATK49_ITEM_EFFECTS_END_TURN_TARGET,
	ATK49_ITEM_EFFECTS_END_TURN_ATTACKER,
	ATK49_ITEM_EFFECTS_CONTACT_ATTACKER,
	ATK49_ITEM_EFFECTS_CONTACT_TARGET,
	ATK49_STATUS_IMMUNITY_ABILITIES,
    ATK49_SUBSTITUTE,
    ATK49_UPDATE_LAST_MOVES,
    ATK49_MIRROR_MOVE,
	ATK49_MAGICIAN_MOXIE,
	ATK49_MULTI_HIT_MOVES,
    ATK49_NEXT_TARGET,
	ATK49_MOVE_RECOIL,
	ATK49_LIFE_ORB_RECOIL,
	ATK49_RESTORE_ABILITIES,
	ATK49_PICKPOCKET,
	ATK49_DANCER,
	ATK49_END_ZMOVES,
    ATK49_COUNT //28 or 0x1C
};

#define ARG_IN_FUTURE_ATTACK 3

void atk49_moveend(void);
bank_t GetNextMultiTarget(void);

void atk49_moveend(void) //All the effects that happen after a move is used
{
    int i, j;
	u32 temp;
    u32 effect = FALSE;
    u8 moveType = 0;
    u8 holdEffectAtk = 0;
    u16* choicedMoveAtk = NULL;
    u8 arg1, arg2;
    u16 originallyUsedMove;
	
	u8 bankAtk = gBankAttacker;
	u8 bankDef = gBankTarget;

    if (gChosenMove == -1)
        originallyUsedMove = 0;
    else
        originallyUsedMove = gChosenMove;

    arg1 = gBattlescriptCurrInstr[1];
    arg2 = gBattlescriptCurrInstr[2];

    holdEffectAtk = ITEM_EFFECT(bankAtk);

    choicedMoveAtk = &gBattleStruct->choicedMove[bankAtk];
    moveType = gBattleStruct->dynamicMoveType;
	
	if (gHitMarker & HITMARKER_UNABLE_TO_USE_MOVE)
	{ 	//Cancel the Z-Moves
		gNewBS->ZMoveData->active = FALSE;
		gNewBS->ZMoveData->toBeUsed[gBankAttacker] = 0;
	}

    do
    {
        switch (gBattleScripting->atk49_state)
        {
		case ATK49_ATTACKER_ABILITIES:			
			if (arg1 != ARG_IN_FUTURE_ATTACK
			&& TOOK_DAMAGE(bankDef)
			&&	MOVE_HAD_EFFECT
			&&  gBattleMons[bankDef].hp
			&&  !MoveBlockedBySubstitute(gCurrentMove, bankAtk, bankDef))
			{
				switch (ABILITY(bankAtk)) {
					case ABILITY_STENCH: //Check for Stench is taken care of in King's Rock check
						if (umodsi(Random(), 100) < 10
						&& BankMovedBefore(bankAtk, bankDef)) 
						{
							gBattleMons[bankDef].status2 |= STATUS2_FLINCHED;
						}
						break;
					
					case ABILITY_POISONTOUCH:
						if (CheckContact(gCurrentMove, bankAtk)
						&& ABILITY(bankDef) != ABILITY_SHIELDDUST
						&& CanBePoisoned(bankDef, bankAtk)
						&& umodsi(Random(), 100) < 30)
						{
							BattleScriptPushCursor();
							gBattlescriptCurrInstr = BattleScript_PoisonTouch;
							effect = TRUE;
						}
				}
			}
			gBattleScripting->atk49_state++;
            break;
		
        case ATK49_RAGE: // rage check
            if (gBattleMons[bankDef].status2 & STATUS2_RAGE
                && gBattleMons[bankDef].hp 
				&& bankAtk != bankDef
                && SIDE(bankAtk) != SIDE(bankDef)
                && MOVE_HAD_EFFECT 
				&& TOOK_DAMAGE(bankDef)
                && SPLIT(gCurrentMove) != SPLIT_STATUS 
				&& gBattleMons[bankDef].statStages[STAT_ATK-1] < 12)
            {
                gBattleMons[bankDef].statStages[STAT_ATK-1]++;
                BattleScriptPushCursor();
                gBattlescriptCurrInstr = BattleScript_RageIsBuilding;
                effect = TRUE;
            }
            gBattleScripting->atk49_state++;
            break;
		
        case ATK49_DEFROST: // defrosting check
            if (gBattleMons[bankDef].status1 & STATUS1_FREEZE
            &&  gBattleMons[bankDef].hp 
			&&  bankAtk != bankDef
            &&  MOVE_HAD_EFFECT
            &&  TOOK_DAMAGE(bankDef)
			&& (moveType == TYPE_FIRE || CheckTableForMove(gCurrentMove, MovesCanUnfreezeTarget)))
            {
                gBattleMons[bankDef].status1 &= ~(STATUS1_FREEZE);
                gActiveBattler = bankDef;
                EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 4, &gBattleMons[bankDef].status1);
                MarkBufferBankForExecution(gActiveBattler);
                BattleScriptPushCursor();
                gBattlescriptCurrInstr = BattleScript_DefrostedViaFireMove;
                effect = TRUE;
            }
            gBattleScripting->atk49_state++;
            break;
			
        case ATK49_SYNCHRONIZE_TARGET: // target synchronize
			if (gCurrentMove != MOVE_PSYCHOSHIFT || !MOVE_HAD_EFFECT) //The lazy way of taking care of Psycho Shift Status Transfer->Synchronize->Heal Status
			{
				if (AbilityBattleEffects(ABILITYEFFECT_SYNCHRONIZE, bankDef, 0, 0, 0))
					effect = TRUE;
			}
            gBattleScripting->atk49_state++;
            break;
			
        case ATK49_MOVE_END_ABILITIES: // Such as abilities activating on contact(Poison Spore, Rough Skin, etc.).
            if (AbilityBattleEffects(ABILITYEFFECT_CONTACT, bankDef, 0, 0, 0))
                effect = TRUE;
            gBattleScripting->atk49_state++;
            break;
		
		case ATK49_BEAK_BLAST_BURN:
			if (CheckContact(gCurrentMove, bankAtk)
			&& MOVE_HAD_EFFECT
			&& TOOK_DAMAGE(bankDef)
			&& gNewBS->BeakBlastByte & gBitTable[bankDef]
			&& CanBeBurned(bankAtk))
			{
				BattleScriptPushCursor();
                gBattlescriptCurrInstr = BattleScript_BeakBlastBurn;
                effect = TRUE;
			}
			else
				gBattleScripting->atk49_state++;
            break;
			
        case ATK49_SYNCHRONIZE_ATTACKER: // attacker synchronize
            if (AbilityBattleEffects(ABILITYEFFECT_ATK_SYNCHRONIZE, bankAtk, 0, 0, 0))
                effect = TRUE;
            gBattleScripting->atk49_state++;
            break;
			
        case ATK49_CHOICE_MOVE: // update choice band move
			if (arg1 != ARG_IN_FUTURE_ATTACK)
			{
				if (gHitMarker & HITMARKER_OBEYS)
				{
					if (holdEffectAtk == ITEM_EFFECT_CHOICE_BAND
					&&  gChosenMove != MOVE_STRUGGLE
					&& (*choicedMoveAtk == 0 || *choicedMoveAtk == 0xFFFF))
					{
						if (gChosenMove == MOVE_BATONPASS && !(gMoveResultFlags & MOVE_RESULT_FAILED))
						{
							gBattleScripting->atk49_state++;
							break;
						}
						
						*choicedMoveAtk = gChosenMove;
					}
					else //This should remove the choice lock glitch
						*choicedMoveAtk = 0;
				}
				
				for (i = 0; i < MAX_MON_MOVES; ++i)
				{
					if (gBattleMons[bankAtk].moves[i] == *choicedMoveAtk)
						break;
				}
				if (i == MAX_MON_MOVES)
					*choicedMoveAtk = 0;
			}
            gBattleScripting->atk49_state++;
            break;
			
        case ATK49_ATTACKER_INVISIBLE: // make attacker sprite invisible
            if (gStatuses3[bankAtk] & (STATUS3_SEMI_INVULNERABLE)
            &&  gHitMarker & HITMARKER_NO_ANIMATIONS)
            {
                gActiveBattler = bankAtk;
                EmitSpriteInvisibility(0, TRUE);
                MarkBufferBankForExecution(gActiveBattler);
                gBattleScripting->atk49_state++;
                return;
            }
            gBattleScripting->atk49_state++;
            break;
			
        case ATK49_TARGET_INVISIBLE: // make target sprite invisible
            if (gStatuses3[bankDef] & (STATUS3_SEMI_INVULNERABLE)
            &&  gHitMarker & HITMARKER_NO_ANIMATIONS)
            {
                gActiveBattler = bankDef;
                EmitSpriteInvisibility(0, TRUE);
                MarkBufferBankForExecution(gActiveBattler);
                gBattleScripting->atk49_state++;
                return;
            }
            gBattleScripting->atk49_state++;
            break;
		
        case ATK49_ATTACKER_VISIBLE: // make attacker sprite visible
            if (gMoveResultFlags & MOVE_RESULT_NO_EFFECT
            || !(gStatuses3[bankAtk] & (STATUS3_SEMI_INVULNERABLE))
            || WasUnableToUseMove(bankAtk))
            {
                gActiveBattler = bankAtk;
                EmitSpriteInvisibility(0, FALSE);
                MarkBufferBankForExecution(gActiveBattler);
                gStatuses3[bankAtk] &= ~(STATUS3_SEMI_INVULNERABLE);
                gSpecialStatuses[bankAtk].restoredBankSprite = 1;
                gBattleScripting->atk49_state++;
                return;
            }
            gBattleScripting->atk49_state++;
            break;
		
        case ATK49_TARGET_VISIBLE: // make target sprite visible
            if (!gSpecialStatuses[bankDef].restoredBankSprite 
			&& bankDef < gBattlersCount
            && !(gStatuses3[bankDef] & STATUS3_SEMI_INVULNERABLE))
            {
                gActiveBattler = bankDef;
                EmitSpriteInvisibility(0, FALSE);
                MarkBufferBankForExecution(gActiveBattler);
                gStatuses3[bankDef] &= ~(STATUS3_SEMI_INVULNERABLE);
                gBattleScripting->atk49_state++;
                return;
            }
            gBattleScripting->atk49_state++;
            break;

		case ATK49_ITEM_EFFECTS_END_TURN_TARGET:
			if (ItemBattleEffects(ItemEffects_EndTurn, bankDef, TRUE, FALSE))
                effect = TRUE;
			gBattleScripting->atk49_state++;
			break;
		
		case ATK49_ITEM_EFFECTS_END_TURN_ATTACKER:
			if (ItemBattleEffects(ItemEffects_EndTurn, bankAtk, TRUE, FALSE))
                effect = TRUE;
			gBattleScripting->atk49_state++;
			break;

        case ATK49_ITEM_EFFECTS_CONTACT_ATTACKER:
			if (arg1 != ARG_IN_FUTURE_ATTACK)
			{
				if (ItemBattleEffects(ItemEffects_ContactAttacker, bankAtk, TRUE, FALSE)) //Attacker comes first because it can be switched
					effect = TRUE;														  //out with Red Card
			}
			gBattleScripting->atk49_state++;
			break;
			
        case ATK49_ITEM_EFFECTS_CONTACT_TARGET:
            if (ItemBattleEffects(ItemEffects_ContactTarget, bankDef, TRUE, FALSE))
				effect = TRUE;
			gBattleScripting->atk49_state++;
			break;
			
        case ATK49_STATUS_IMMUNITY_ABILITIES: // status immunities
            if (AbilityBattleEffects(ABILITYEFFECT_IMMUNITY, 0, 0, 0, 0))
                effect = TRUE; // it loops through all battlers, so we increment after its done with all battlers
            else
                gBattleScripting->atk49_state++;
            break;
		
        case ATK49_SUBSTITUTE: // update substitute
            for (i = 0; i < gBattlersCount; i++)
            {
                if (gDisableStructs[i].substituteHP == 0)
                    gBattleMons[i].status2 &= ~(STATUS2_SUBSTITUTE);
            }
            gBattleScripting->atk49_state++;
            break;
		
        case ATK49_UPDATE_LAST_MOVES:
			if (gNewBS->DancerInProgress
			||  arg1 == ARG_IN_FUTURE_ATTACK) 
			{
				gBattleScripting->atk49_state++;
				break;
			}
			
            if (gHitMarker & HITMARKER_SWAP_ATTACKER_TARGET)
            {
                bankAtk = gBankTarget;
                bankDef = gBankAttacker;
                gHitMarker &= ~(HITMARKER_SWAP_ATTACKER_TARGET);
            }
			
            if (gHitMarker & HITMARKER_ATTACKSTRING_PRINTED)
            {
                gLastPrintedMoves[bankAtk] = gChosenMove;
            }
			
            if (!(gAbsentBattlerFlags & gBitTable[bankAtk])
            && !(gBattleStruct->field_91 & gBitTable[bankAtk])
            && gBattleMoves[originallyUsedMove].effect != EFFECT_BATON_PASS)
            {
                if (gHitMarker & HITMARKER_OBEYS)
                {
                    gLastUsedMoves[bankAtk] = gChosenMove;
                    gLastResultingMoves[bankAtk] = gCurrentMove;
                }
                else
                {
                    gLastUsedMoves[bankAtk] = 0xFFFF;
                    gLastResultingMoves[bankAtk] = 0xFFFF;
                }

                if (!(gHitMarker & HITMARKER_FAINTED(bankDef)))
                    gLastHitBy[bankDef] = bankAtk;

                if (gHitMarker & HITMARKER_OBEYS && MOVE_HAD_EFFECT)
                {
                    if (gChosenMove == 0xFFFF)
                    {
                        gLastLandedMoves[bankDef] = gChosenMove;
                    }
                    else
                    {
						if (gNewBS->ZMoveData->active)
							gLastLandedMoves[bankDef] = gChosenMove;
						
						else
							gLastLandedMoves[bankDef] = gCurrentMove;

						gLastHitByType[bankDef] = moveType;
                    }
                }
                else
                {
                    gLastLandedMoves[bankDef] = 0xFFFF;
                }
            }
            gBattleScripting->atk49_state++;
            break;
		
        case ATK49_MIRROR_MOVE:
            if (arg1 != ARG_IN_FUTURE_ATTACK
			&& !(gAbsentBattlerFlags & gBitTable[bankAtk]) 
			&& !(gBattleStruct->field_91 & gBitTable[bankAtk])
            && gBattleMoves[originallyUsedMove].flags & FLAG_MIRROR_MOVE_AFFECTED 
			&& gHitMarker & HITMARKER_OBEYS
            && bankAtk != bankDef 
			&& !(gHitMarker & HITMARKER_FAINTED(bankDef))
            && MOVE_HAD_EFFECT)
            {
                gBattleStruct->lastTakenMove[bankDef] = gChosenMove;
				gBattleStruct->lastTakenMoveFrom[bankDef][bankAtk] = gChosenMove;
            }
			
			if (gNewBS->ZMoveData->active)
            {
                gBattleStruct->lastTakenMove[bankDef] = 0;
				gBattleStruct->lastTakenMoveFrom[bankDef][bankAtk] = 0;
            }
			
            gBattleScripting->atk49_state++;
			SeedHelper[0] = 0; //Reset Seed Helper for Soul Heart
            break;
		
		case ATK49_MAGICIAN_MOXIE:
			switch (ABILITY(bankAtk)) {
				case ABILITY_MAGICIAN:
					if (ITEM(bankAtk) == 0
					&& ITEM(bankDef)
					&& gBattleMons[bankAtk].hp
					&& !MoveBlockedBySubstitute(gCurrentMove, bankAtk, bankDef)
					&& gMultiHitCounter <= 1
					&& TOOK_DAMAGE(bankDef)
					&& MOVE_HAD_EFFECT
					&& CanTransferItem(gBattleMons[bankDef].species, ITEM(bankDef), GetBankPartyData(bankDef))
					&& CanTransferItem(gBattleMons[bankAtk].species, ITEM(bankDef), GetBankPartyData(bankAtk))
					&& (ABILITY(bankDef) != ABILITY_STICKYHOLD || gBattleMons[bankDef].hp == 0))
					{
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_Magician;
						effect = 1;
					}
					break;
					
				case ABILITY_MOXIE:
					if ((arg1 != ARG_IN_FUTURE_ATTACK || gWishFutureKnock->futureSightPartyIndex[bankDef] == gBattlerPartyIndexes[bankAtk])
					&& gBattleMons[bankDef].hp == 0
					&& gBattleMons[bankAtk].hp
					&& TOOK_DAMAGE(bankDef)
					&& MOVE_HAD_EFFECT
					&& STAT_CAN_RISE(gBankAttacker, STAT_STAGE_ATK)
					&& PartyAlive(bankDef))
					{
						PREPARE_STAT_BUFFER(gBattleTextBuff1, STAT_STAGE_ATK);
						
						gEffectBank = gBankAttacker;
						gBattleScripting->bank = gBankAttacker;
						gBattleScripting->statChanger = INCREASE_1 | STAT_STAGE_ATK;
						gBattleScripting->animArg1 = 0xE + STAT_STAGE_ATK;
						gBattleScripting->animArg2 = 0;
						gLastUsedAbility = ABILITY_MOXIE;
						RecordAbilityBattle(gBankAttacker, gLastUsedAbility);
						
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_Moxie;
						effect = 1;
					}
					break;
					
				case ABILITY_BEASTBOOST: ;
					if ((arg1 != ARG_IN_FUTURE_ATTACK || gWishFutureKnock->futureSightPartyIndex[bankDef] == gBattlerPartyIndexes[bankAtk])
					&& gBattleMons[bankDef].hp == 0
					&& gBattleMons[bankAtk].hp
					&& TOOK_DAMAGE(bankDef)
					&& MOVE_HAD_EFFECT
					&& PartyAlive(bankDef))
					{
						u16 temp;
						u16 max;
						u16 stats[STAT_STAGE_SPDEF]; //Create new array to avoid modifying original stats
						
						stats[STAT_STAGE_ATK-1] = gBattleMons[bankAtk].attack;
						stats[STAT_STAGE_DEF-1] = gBattleMons[bankAtk].defense;
						stats[STAT_STAGE_SPATK-2] = gBattleMons[bankAtk].spAttack;
						stats[STAT_STAGE_SPDEF-2] = gBattleMons[bankAtk].spDefense;
						stats[STAT_STAGE_SPEED+1] = gBattleMons[bankAtk].speed;
						
						if (gNewBS->WonderRoomTimer) {
							temp = stats[STAT_STAGE_DEF-1];
							stats[STAT_STAGE_DEF-1] = stats[STAT_STAGE_SPDEF-2]; //-2 b/c shifted left due to speed
							stats[STAT_STAGE_SPDEF-2] = temp;
						}
						
						max = 0;
						for (int i = 1; i < STAT_STAGE_SPDEF; ++i) {
							if (stats[i] > stats[max])
								max = i;
						}
						
						//Get the proper stat stage value
						switch(max) {
							case 0: //Attack
							case 1: //Defense
								max += 1;
								break;
							case 2: //Special Attack
							case 3: //Special Defense
								max += 2;
								break;
							case 4:
								max = STAT_STAGE_SPEED;
						}
						
						if (STAT_CAN_RISE(gBankAttacker, max))
						{
							PREPARE_STAT_BUFFER(gBattleTextBuff1, max);
							
							gEffectBank = gBankAttacker;
							gBattleScripting->bank = gBankAttacker;
							gBattleScripting->statChanger = INCREASE_1 | max;
							gBattleScripting->animArg1 = 0xE + max;
							gBattleScripting->animArg2 = 0;
							gLastUsedAbility = ABILITY_BEASTBOOST;
							RecordAbilityBattle(gBankAttacker, gLastUsedAbility);
							
							BattleScriptPushCursor();
							gBattlescriptCurrInstr = BattleScript_Moxie;
							effect = 1;
						}
					}
			}
            gBattleScripting->atk49_state++;
            break;
		
		case ATK49_MULTI_HIT_MOVES:
			if (arg1 != ARG_IN_FUTURE_ATTACK
			&& !(gMoveResultFlags & MOVE_RESULT_NO_EFFECT)
			&& !(gHitMarker & HITMARKER_UNABLE_TO_USE_MOVE)
			&& gMultiHitCounter)
			{
				++gBattleScripting->multihitString[4];
				if (--gMultiHitCounter == 0)
				{
					if (!gNewBS->ParentalBondOn) 
					{
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_MultiHitPrintStrings;
						effect = 1;
					}
				}
				else
				{
					if (gBattleMons[gBankAttacker].hp
					&& gBattleMons[gBankTarget].hp
					&& (gChosenMove == MOVE_SLEEPTALK || !(gBattleMons[gBankAttacker].status1 & STATUS1_SLEEP))
					&& !(gBattleMons[gBankAttacker].status1 & STATUS1_FREEZE))
					{
						if (gNewBS->ParentalBondOn)
							--gNewBS->ParentalBondOn;
						
						gHitMarker |= (HITMARKER_NO_PPDEDUCT | HITMARKER_NO_ATTACKSTRING);
						gBattleScripting->animTargetsHit = 0;
						gBattleScripting->atk49_state = 0;
						MoveValuesCleanUp();
						BattleScriptPush(gBattleScriptsForMoveEffects[gBattleMoves[gCurrentMove].effect]);
						gBattlescriptCurrInstr = BattleScript_FlushMessageBox;
						return;
					}
					else if (!gNewBS->ParentalBondOn)
					{
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_MultiHitPrintStrings;
						effect = 1;
					}
				}
			}
			
            gBattleScripting->atk49_state++;
			gMultiHitCounter = 0;
			gNewBS->ParentalBondOn = 0;
            break;
		
        case ATK49_NEXT_TARGET: // For moves hitting two opposing Pokemon or whole field.
			gHitMarker |= HITMARKER_NO_ATTACKSTRING;
			if (gNewBS->MoveBounceInProgress)
				++gNewBS->MoveBounceTargetCount;
			else
				++gNewBS->OriginalAttackerTargetCount;

			if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
			{
				if (!(gHitMarker & HITMARKER_UNABLE_TO_USE_MOVE)
				&& !gProtectStructs[bankAtk].chargingTurn)
				{
					if (gBattleMoves[gCurrentMove].target == MOVE_TARGET_BOTH)
					{
						if ((gNewBS->OriginalAttackerTargetCount < 2 && !gNewBS->MoveBounceInProgress)
						|| (gNewBS->MoveBounceTargetCount < 2 && gNewBS->MoveBounceInProgress))
						{ //Get Next Target
							u8 battlerId = PARTNER(bankDef);
							if (gBattleMons[battlerId].hp)
							{
								gBankTarget = battlerId;
								gBattleScripting->atk49_state = 0;
								MoveValuesCleanUp();
								BattleScriptPush(gBattleScriptsForMoveEffects[gBattleMoves[gCurrentMove].effect]);
								gBattlescriptCurrInstr = BattleScript_FlushMessageBox;
								return;
							}
							else if (gNewBS->MoveBounceInProgress)
								goto RESTORE_BOUNCE_ATTACKER;
						}
						else if (gNewBS->MoveBounceInProgress)
						{
						RESTORE_BOUNCE_ATTACKER:
							++gNewBS->OriginalAttackerTargetCount;
							gNewBS->MoveBounceInProgress = 2; //Bounce just finished
							gNewBS->MoveBounceTargetCount = 0;
							u8 battlerId = PARTNER(bankAtk);
							if (gBattleMons[battlerId].hp && gNewBS->OriginalAttackerTargetCount < 2)
							{
								gBankAttacker = gBanksByTurnOrder[gCurrentTurnActionNumber]; //Restore original attacker
								gBankTarget = battlerId; //Attack Bouncer's partner
								gBattleScripting->animTargetsHit = 0;
								gBattleScripting->atk49_state = 0;
								MoveValuesCleanUp();
								BattleScriptPush(gBattleScriptsForMoveEffects[gBattleMoves[gCurrentMove].effect]);
								gBattlescriptCurrInstr = BattleScript_FlushMessageBox;
								return;
							}
						}
					}
					else if (gBattleMoves[gCurrentMove].target == MOVE_TARGET_ALL)
					{
						if (gNewBS->OriginalAttackerTargetCount < 3)
						{ //Get Next Target
							u8 battlerId = GetNextMultiTarget();
							if (battlerId != 0xFF)
							{
								while (GetNextMultiTarget() != 0xFF)
								{
									gBankTarget = battlerId;
									if (gBattleMons[battlerId].hp)
									{
										gBattleScripting->atk49_state = 0;
										MoveValuesCleanUp();
										BattleScriptPush(gBattleScriptsForMoveEffects[gBattleMoves[gCurrentMove].effect]);
										gBattlescriptCurrInstr = BattleScript_FlushMessageBox;
										return;
									}
									++gNewBS->OriginalAttackerTargetCount;
								}
							}
						}
					}
				}
			}
			
			if (gNewBS->MoveBounceInProgress)
				gNewBS->MoveBounceInProgress = 2; //Bounce just finished
			
			gNewBS->MoveBounceTargetCount = 0;			
			gNewBS->OriginalAttackerTargetCount = 0;
            gBattleScripting->atk49_state++;
            break;
		
		case ATK49_MOVE_RECOIL:
			if (arg1 != ARG_IN_FUTURE_ATTACK
			&& gBattleMons[bankAtk].hp
			&&  !(gHitMarker & HITMARKER_UNABLE_TO_USE_MOVE))
			{
				if (gCurrentMove == MOVE_MINDBLOWN
				&& ABILITY(bankAtk) != ABILITY_MAGICGUARD)
				{
					gBattleMoveDamage = MathMax(1, gBattleMons[bankAtk].maxHP / 2);
					
					if (IS_ODD(gBattleMons[bankAtk].maxHP)
					&& gBattleMons[bankAtk].maxHP >= 3)
						++gBattleMoveDamage; //Round up Damage
						
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_MindBlownDamage;
					
					effect = 1;
				}
				else if (gCurrentMove == MOVE_STRUGGLE)
				{
					gBattleMoveDamage = MathMax(1, gBattleMons[bankAtk].maxHP / 4);
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_Recoil;
					effect = 1;
				}
				else if (gBattleMoves[gCurrentMove].effect == EFFECT_EXPLOSION
				&& !ABILITY_PRESENT(ABILITY_DAMP))
				{
					BattleScriptPushCursor();
					
					if (gNewBS->AttackerDidDamageAtLeastOnce)
						gBattlescriptCurrInstr = BattleScript_FaintAttacker;
					else
						gBattlescriptCurrInstr = BattleScript_ExplosionAnim; //The attacker still needs to explode
				
					effect = 1;
				}
				else if (ABILITY(bankAtk) != ABILITY_MAGICGUARD
				&& ABILITY(bankAtk) != ABILITY_ROCKHEAD
				&& gNewBS->AttackerDidDamageAtLeastOnce)
				{
					if (CheckTableForMove(gCurrentMove, Percent25RecoilMoves))
					{
						gBattleMoveDamage = MathMax(1, udivsi(gHpDealt, 4));
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_Recoil;
					}
					else if (CheckTableForMove(gCurrentMove, Percent33RecoilMoves))
					{
						gBattleMoveDamage = MathMax(1, udivsi(gHpDealt, 3));
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_Recoil;
					}
					else if (CheckTableForMove(gCurrentMove, Percent50RecoilMoves))
					{
						gBattleMoveDamage = MathMax(1, udivsi(gHpDealt, 2));
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_Recoil;
					}
					else if (CheckTableForMove(gCurrentMove, Percent66RecoilMoves))
					{
						gBattleMoveDamage = MathMax(1, udivsi(gHpDealt * 2, 3));
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_Recoil;
					}
					else if (CheckTableForMove(gCurrentMove, Percent75RecoilMoves))
					{
						gBattleMoveDamage = MathMax(1, udivsi(gHpDealt * 3, 4));
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_Recoil;
					}
					else if (CheckTableForMove(gCurrentMove, Percent100RecoilMoves))
					{
						gBattleMoveDamage = MathMax(1, gHpDealt);
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_Recoil;
					}
				}
			}
			gBattleScripting->atk49_state++;
			break;
			
		case ATK49_LIFE_ORB_RECOIL:
			if (arg1 != ARG_IN_FUTURE_ATTACK
			&& ITEM_EFFECT(bankAtk) == ITEM_EFFECT_LIFE_ORB
			&& gNewBS->AttackerDidDamageAtLeastOnce
			&& ABILITY(bankAtk) != ABILITY_MAGICGUARD
			&& gBattleMons[bankAtk].hp
			&& !SheerForceCheck())
			{
				gBattleMoveDamage = MathMax(1, udivsi(gBattleMons[bankAtk].maxHP, 10));
				BattleScriptPushCursor();
                gBattlescriptCurrInstr = BattleScript_LifeOrbDamage;
				effect = 1;
			}
			gBattleScripting->atk49_state++;
			break;

		case ATK49_RESTORE_ABILITIES:
			for (i = 0; i < gBattlersCount; ++i)
			{
				if (gNewBS->DisabledMoldBreakerAbilities[i])
				{
					gBattleMons[i].ability = gNewBS->DisabledMoldBreakerAbilities[i];
					gNewBS->DisabledMoldBreakerAbilities[i] = 0;
				}
			}
			gBattleScripting->atk49_state++;
			break;
		
		case ATK49_PICKPOCKET: //Move to before choose new target?
			if (ABILITY(bankDef) == ABILITY_PICKPOCKET
			&& CheckContact(gCurrentMove, bankAtk)
			&& gBattleMons[bankDef].hp
			&& gMultiHitCounter <= 1
			&& ITEM(bankAtk)
			&& ITEM(bankDef) == 0
			&& (ABILITY(bankAtk) != ABILITY_STICKYHOLD || gBattleMons[bankAtk].hp == 0)
			&& !MoveBlockedBySubstitute(gCurrentMove, bankAtk, bankDef)
			&& TOOK_DAMAGE(bankDef)
			&& MOVE_HAD_EFFECT)
			{
				BattleScriptPushCursor();
                gBattlescriptCurrInstr = BattleScript_Pickpocket;
				effect = 1;
			}
			gBattleScripting->atk49_state++;
			break;
			
		case ATK49_END_ZMOVES:
			gNewBS->ZMoveData->active = FALSE;
			gNewBS->ZMoveData->effectApplied = FALSE;
			gNewBS->AttackerDidDamageAtLeastOnce = FALSE;
			gBattleScripting->atk49_state++;
			break;
		
		case ATK49_DANCER:	
			if (gNewBS->DancerBankCount == gBattlersCount)
			{
				gNewBS->DancerInProgress = FALSE;
				gNewBS->DancerByte = 0;
				gNewBS->DancerTurnOrder[0] = 0; //Set the whole array to 0
				gNewBS->DancerTurnOrder[1] = 0;
				gNewBS->DancerTurnOrder[2] = 0;
				gNewBS->DancerTurnOrder[3] = 0;
				gBattleScripting->atk49_state++;
				break;
			}
		
			if (!gNewBS->DancerInProgress
			&& ABILITY_PRESENT(ABILITY_DANCER)
			&& gHitMarker & HITMARKER_ATTACKSTRING_PRINTED
			&& MOVE_HAD_EFFECT
			&& !gNewBS->MoveBounceInProgress
			&& CheckTableForMove(gCurrentMove, DanceMoveTable))
			{
				gNewBS->CurrentTurnAttacker = bankAtk;
				gNewBS->DancerBankCount = 0;
				
				for (i = 0; i < gBattlersCount; ++i)
					gNewBS->DancerTurnOrder[i] = i;
				
				for (i = 0; i < gBattlersCount - 1; ++i) {
					for (j = i + 1; j < gBattlersCount; ++j) {
						if (SpeedCalc(gNewBS->DancerTurnOrder[i]) > SpeedCalc(gNewBS->DancerTurnOrder[j]))
						{ //Slower mon moves first
							temp = gNewBS->DancerTurnOrder[i];
							gNewBS->DancerTurnOrder[i] = gNewBS->DancerTurnOrder[j];
							gNewBS->DancerTurnOrder[j] = temp;
						}
					}
				}
			}
			else if (!gNewBS->DancerInProgress)
			{
				gBattleScripting->atk49_state++;
				break;
			}
				
			u8 bank = gNewBS->DancerTurnOrder[gNewBS->DancerBankCount];
			
			if (ABILITY(bank) == ABILITY_DANCER
			&& !(gAbsentBattlerFlags & gBitTable[bank])
			&& bank != gNewBS->CurrentTurnAttacker)
			{
				gBankAttacker = bank;
				gBankTarget = GetMoveTarget(gCurrentMove, FALSE);
				gBattleScripting->atk49_state = 0;
				gRandomMove = gCurrentMove;
				gBattleScripting->bank = gBankAttacker;
                gBattlescriptCurrInstr = BattleScript_DancerActivated;
				gHitMarker &= ~(HITMARKER_NO_ATTACKSTRING | HITMARKER_ATTACKSTRING_PRINTED);
				gNewBS->AttackerDidDamageAtLeastOnce = FALSE;
				gNewBS->DancerInProgress = TRUE;
				effect = TRUE;
			}
			
			++gNewBS->DancerBankCount;
			break;
			
        case ATK49_COUNT:
			gNewBS->InstructInProgress = FALSE;
            break;
        }
		
		//Should I remove these lines?
        if (arg1 == 1 && effect == FALSE)
            gBattleScripting->atk49_state = ATK49_COUNT;
        if (arg1 == 2 && arg2 == gBattleScripting->atk49_state)
            gBattleScripting->atk49_state = ATK49_COUNT;

    } while (gBattleScripting->atk49_state != ATK49_COUNT && effect == FALSE);

    if (gBattleScripting->atk49_state == ATK49_COUNT && effect == FALSE)
        gBattlescriptCurrInstr += 3;
}

const bank_t gTargetsByBank[4][4] = 
{
	{B_POSITION_OPPONENT_LEFT, B_POSITION_OPPONENT_RIGHT, B_POSITION_PLAYER_RIGHT, 0xFF},	//Bank 0 - Player Left 
	{B_POSITION_PLAYER_LEFT, B_POSITION_PLAYER_RIGHT, B_POSITION_OPPONENT_RIGHT, 0xFF}, 	//Bank 1 - Opponent Left
	{B_POSITION_OPPONENT_LEFT, B_POSITION_OPPONENT_RIGHT, B_POSITION_PLAYER_LEFT, 0xFF}, 	//Bank 2 - Player Right
	{B_POSITION_PLAYER_LEFT, B_POSITION_PLAYER_RIGHT, B_POSITION_OPPONENT_LEFT, 0xFF}  		//Bank 3 - Opponent Right
};

bank_t GetNextMultiTarget(void) 
{
	return gTargetsByBank[gBankAttacker][gNewBS->OriginalAttackerTargetCount];
}