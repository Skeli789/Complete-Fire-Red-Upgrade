#include "defines.h"
#include "helper_functions.h"

//Add some way to check if Life Orb did damage to either foe
//Make sure there's no choice lock glitch
//Add check to see if AI move prediction was successful. If not, then if the same move is predicted, don't predict that same move again.
//Remove the lines at the bottom?

#define BattleScript_RageIsBuilding (u8*) 0x81D8C48
#define BattleScript_DefrostedViaFireMove (u8*) 0x81D9098
#define BattleScript_FlushMessageBox (u8*) 0x81D96A8

extern move_t MovesCanUnfreezeTarget[];
extern move_t DanceMoveTable[];
extern move_t TwoToFiveStrikesMoves[];
extern move_t TwoStrikesMoves[];

extern u8 BattleScript_PoisonTouch[];
extern u8 BattleScript_BeakBlastBurn[];
extern u8 BattleScript_Magician[];
extern u8 BattleScript_LifeOrbDamage[];
extern u8 BattleScript_Pickpocket[];
extern u8 BattleScript_DancerActivated[];
extern u8 BattleScript_MultiHitPrintStrings[];

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
    ATK49_STATUS_IMMUNITY_ABILITIES,
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
    ATK49_SUBSTITUTE,
    ATK49_UPDATE_LAST_MOVES,
    ATK49_MIRROR_MOVE,
	ATK49_MAGICIAN,
	ATK49_MULTI_HIT_MOVES,
    ATK49_NEXT_TARGET,
	ATK49_LIFE_ORB_RECOIL,
	ATK49_RESTORE_ABILITIES,
	ATK49_PICKPOCKET,
	ATK49_DANCER,
	ATK49_END_ZMOVES,
    ATK49_COUNT //27 or 0x1B
};

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

    do
    {
        switch (gBattleScripting->atk49_state)
        {
		case ATK49_ATTACKER_ABILITIES:			
			if (TOOK_DAMAGE(bankDef)
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
			
        case ATK49_STATUS_IMMUNITY_ABILITIES: // status immunities
            if (AbilityBattleEffects(ABILITYEFFECT_IMMUNITY, 0, 0, 0, 0))
                effect = TRUE; // it loops through all battlers, so we increment after its done with all battlers
            else
                gBattleScripting->atk49_state++;
            break;
		
		case ATK49_BEAK_BLAST_BURN:
			if (CheckContact(gCurrentMove, bankAtk)
			&& MOVE_HAD_EFFECT
			&& TOOK_DAMAGE(bankDef)
			&& BeakBlastByte & gBitTable[bankDef]
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
            if (ItemBattleEffects(ItemEffects_ContactAttacker, bankAtk, TRUE, FALSE)) //Attacker comes first because it can be switched
				effect = TRUE;														  //out with Red Card
			gBattleScripting->atk49_state++;
			break;
			
        case ATK49_ITEM_EFFECTS_CONTACT_TARGET:
            if (ItemBattleEffects(ItemEffects_ContactTarget, bankDef, TRUE, FALSE))
				effect = TRUE;
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
		
        case ATK49_SUBSTITUTE: // update substitute
            for (i = 0; i < gBattlersCount; i++)
            {
                if (gDisableStructs[i].substituteHP == 0)
                    gBattleMons[i].status2 &= ~(STATUS2_SUBSTITUTE);
            }
            gBattleScripting->atk49_state++;
            break;
		
        case ATK49_UPDATE_LAST_MOVES:
			if (DancerInProgress) {
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
						if (ZMoveData->active)
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
            if (!(gAbsentBattlerFlags & gBitTable[bankAtk]) 
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
			
			if (ZMoveData->active)
            {
                gBattleStruct->lastTakenMove[bankDef] = 0;
				gBattleStruct->lastTakenMoveFrom[bankDef][bankAtk] = 0;
            }
			
            gBattleScripting->atk49_state++;
            break;
		
		case ATK49_MAGICIAN:
			if (ABILITY(bankAtk) == ABILITY_MAGICIAN
			&& ITEM(bankAtk) == 0
			&& ITEM(bankDef)
			&& gBattleMons[bankAtk].hp
			&& !MoveBlockedBySubstitute(gCurrentMove, bankAtk, bankDef)
			&& gMultiHitCounter <= 1
			&& TOOK_DAMAGE(bankDef)
			&& CanTransferItem(gBattleMons[bankDef].species, ITEM(bankDef), GetBankPartyData(bankDef))
			&& CanTransferItem(gBattleMons[bankAtk].species, ITEM(bankDef), GetBankPartyData(bankAtk))
			&& (ABILITY(bankDef) != ABILITY_STICKYHOLD || gBattleMons[bankDef].hp == 0))
			{
				BattleScriptPushCursor();
                gBattlescriptCurrInstr = BattleScript_Magician;
				effect = 1;
			}
            gBattleScripting->atk49_state++;
            break;
		
		case ATK49_MULTI_HIT_MOVES:
			if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT) && gMultiHitCounter)
			{
				++gBattleScripting->multihitString[4];
				if (--gMultiHitCounter == 0)
				{
					if (!ParentalBondOn) 
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
						if (ParentalBondOn)
							--ParentalBondOn;
						
						gHitMarker |= (HITMARKER_NO_PPDEDUCT | HITMARKER_NO_ATTACKSTRING);
						gBattleScripting->animTargetsHit = 0;
						gBattleScripting->atk49_state = 0;
						MoveValuesCleanUp();
						BattleScriptPush(gBattleScriptsForMoveEffects[gBattleMoves[gCurrentMove].effect]);
						gBattlescriptCurrInstr = BattleScript_FlushMessageBox;
						return;
					}
					else if (!ParentalBondOn)
					{
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_MultiHitPrintStrings;
						effect = 1;
					}
				}
			}
			
            gBattleScripting->atk49_state++;
			gMultiHitCounter = 0;
			ParentalBondOn = 0;
            break;
		
        case ATK49_NEXT_TARGET: // For moves hitting two opposing Pokemon.
			gHitMarker |= HITMARKER_NO_ATTACKSTRING;
			if (MoveBounceInProgress)
				++MoveBounceTargetCount;
			else
				++OriginalAttackerTargetCount;

			if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
			{
				if (!(gHitMarker & HITMARKER_UNABLE_TO_USE_MOVE)
				&& !gProtectStructs[bankAtk].chargingTurn)
				{
					if (gBattleMoves[gCurrentMove].target == MOVE_TARGET_BOTH)
					{
						if ((OriginalAttackerTargetCount < 2 && !MoveBounceInProgress)
						|| (MoveBounceTargetCount < 2 && MoveBounceInProgress))
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
							else if (MoveBounceInProgress)
								goto RESTORE_BOUNCE_ATTACKER;
						}
						else if (MoveBounceInProgress)
						{
						RESTORE_BOUNCE_ATTACKER:
							++OriginalAttackerTargetCount;
							MoveBounceInProgress = 2; //Bounce just finished
							MoveBounceTargetCount = 0;
							u8 battlerId = PARTNER(bankAtk);
							if (gBattleMons[battlerId].hp && OriginalAttackerTargetCount < 2)
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
					/*else if (gBattleMoves[gCurrentMove].target == MOVE_TARGET_ALL) //TO DO
					{
						if ((OriginalAttackerTargetCount < 3 && !MoveBounceInProgress)
						|| (MoveBounceTargetCount < 2 && MoveBounceInProgress))
						{ //Get Next Target
							u8 battlerId = GetNextMultiTarget();
							if (battlerId != -1)
							{
								while (GetNextMultiTarget() != -1)
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
								}
							}
							else if (MoveBounceInProgress)
								goto RESTORE_BOUNCE_ATTACKER;
						}
						else if (MoveBounceInProgress)
						{
						RESTORE_BOUNCE_ATTACKER:
							++OriginalAttackerTargetCount;
							MoveBounceInProgress = 2; //Bounce just finished
							MoveBounceTargetCount = 0;
							u8 battlerId = GetNextMultiTarget();
							if (gBattleMons[battlerId].hp && OriginalAttackerTargetCount < 3)
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
					}*/
				}
			}
			
			if (MoveBounceInProgress)
				MoveBounceInProgress = 2; //Bounce just finished
			
			MoveBounceTargetCount = 0;			
			OriginalAttackerTargetCount = 0;
            gBattleScripting->atk49_state++;
            break;
			
		case ATK49_LIFE_ORB_RECOIL:
			if (ITEM_EFFECT(bankAtk) == ITEM_EFFECT_LIFE_ORB
			&& gMultiHitCounter <= 1
			&& AttackerDidDamageAtLeastOnce
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
				if (DisabledMoldBreakerAbilities[i])
				{
					gBattleMons[i].ability = DisabledMoldBreakerAbilities[i];
					DisabledMoldBreakerAbilities[i] = 0;
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
			ZMoveData->active = FALSE;
			ZMoveData->effectApplied = FALSE;
			AttackerDidDamageAtLeastOnce = FALSE;
			gBattleScripting->atk49_state++;
			break;
		
		case ATK49_DANCER:	
			if (DancerBankCount == gBattlersCount)
			{
				DancerInProgress = FALSE;
				DancerByte = 0;
				*((u32*) DancerTurnOrder) = 0; //Set the whole array to 0
				gBattleScripting->atk49_state++;
				break;
			}
		
			if (!DancerInProgress
			&& ABILITY_PRESENT(ABILITY_DANCER)
			&& gHitMarker & HITMARKER_ATTACKSTRING_PRINTED //Should it be (!HITMARKER_NO_ANIMATIONS)?
			&& MOVE_HAD_EFFECT
			&& !MoveBounceInProgress
			&& CheckTableForMove(gCurrentMove, DanceMoveTable))
			{
				CurrentTurnAttacker = bankAtk;
				DancerBankCount = 0;
				
				for (i = 0; i < gBattlersCount; ++i)
					DancerTurnOrder[i] = i;
				
				for (i = 0; i < gBattlersCount - 1; ++i) {
					for (j = i + 1; j < gBattlersCount; ++j) {
						if (SpeedCalc(DancerTurnOrder[i]) > SpeedCalc(DancerTurnOrder[j]))
						{ //Slower mon moves first
							temp = DancerTurnOrder[i];
							DancerTurnOrder[i] = DancerTurnOrder[j];
							DancerTurnOrder[j] = temp;
						}
					}
				}
			}
			else if (!DancerInProgress)
			{
				gBattleScripting->atk49_state++;
				break;
			}
				
			u8 bank = DancerTurnOrder[DancerBankCount];
			
			if (ABILITY(bank) == ABILITY_DANCER
			&& !(gAbsentBattlerFlags & gBitTable[bank])
			&& bank != CurrentTurnAttacker)
			{
				gBankAttacker = bank;
				gBankTarget = GetMoveTarget(gCurrentMove, FALSE);
				gBattleScripting->atk49_state = 0;
				gRandomMove = gCurrentMove;
				gBattleScripting->bank = gBankAttacker;
                gBattlescriptCurrInstr = BattleScript_DancerActivated;
				gHitMarker &= ~(HITMARKER_NO_ANIMATIONS | HITMARKER_NO_ATTACKSTRING | HITMARKER_ATTACKSTRING_PRINTED);
				AttackerDidDamageAtLeastOnce = FALSE;
				DancerInProgress = TRUE;
				effect = TRUE;
			}
			
			++DancerBankCount;
			break;
			
        case ATK49_COUNT:
			InstructInProgress = FALSE;
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