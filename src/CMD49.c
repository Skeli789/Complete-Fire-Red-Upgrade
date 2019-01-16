#include "defines.h"
#include "helper_functions.h"

#define BattleScript_RageIsBuilding (u8*) 0x0
#define BattleScript_DefrostedViaFireMove (u8*) 0x0
#define BattleScript_FlushMessageBox (u8*) 0x0

extern u8 BattleScript_PoisonTouch[];
extern u8 BattleScript_BeakBlastBurn[];
extern u8 BattleScript_Magician[];
extern u8 BattleScript_LifeOrbDamage[];
extern u8 BattleScript_Pickpocket[];

enum
{
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
    ATK49_NEXT_TARGET,
	ATK49_LIFE_ORB_RECOIL,
	ATK49_PICKPOCKET,
	ATK49_RESTORE_ABILITIES,
	ATK49_END_ZMOVES,
    ATK49_COUNT,
};
/*
static void atk49_moveend(void)
{
    s32 i;
    bool32 effect = FALSE;
    u8 moveType = 0;
    u8 holdEffectAtk = 0;
    u16 *choicedMoveAtk = NULL;
    u8 arg1, arg2;
    u16 originallyUsedMove;

    if (gChosenMove == 0xFFFF)
        originallyUsedMove = 0;
    else
        originallyUsedMove = gChosenMove;

    arg1 = gBattlescriptCurrInstr[1];
    arg2 = gBattlescriptCurrInstr[2];

    holdEffectAtk = ITEM_EFFECT(gBankAttacker);

    choicedMoveAtk = &gBattleStruct->choicedMove[gBankAttacker];
    moveType = gBattleStruct->dynamicMoveType;

    do
    {
        switch (gBattleScripting.atk49_state)
        {
		case ATK49_ATTACKER_ABILITIES:
			if (!TOOK_DAMAGE(gBankTarget)
			||  gBattleMons[gBankTarget].hp == 0
			||  MoveBlockedBySubstitute(gCurrentMove, gBankAttacker, gBankTarget)
			{
				gBattleScripting.atk49_state++;
				break;
			}
		
			switch (ABILITY(gBankAtatcker)) {
				case ABILITY_STENCH:
					if (ITEM_EFFECT(gBankAttacker) != ITEM_EFFECT_FLINCH
					&& umodsi(Random(), 100) < 10) 
					{
						gBattleMons[gBankTarget].status2 |= STATUS2_FLINCH;
					}
					break;
				
				case ABILITY_POISONTOUCH:
					if (CheckContact(gCurrentMove, gBankAttacker)
					&& ABILITY(gBankTarget) != ABILITY_SHIELDDUST
					&& CanBePoisoned(gBankTarget)
					&& umodsi(Random(), 100) < 30)
					{
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_PoisonTouch;
						effect = TRUE;
					}
			}
			gBattleScripting.atk49_state++;
            break;
		
        case ATK49_RAGE: // rage check
            if (gBattleMons[gBankTarget].status2 & STATUS2_RAGE
                && gBattleMons[gBankTarget].hp != 0 && gBankAttacker != gBankTarget
                && SIDE(gBankAttacker) != SIDE(gBankTarget)
                && !(gMoveResultFlags & MOVE_RESULT_NO_EFFECT) && TOOK_DAMAGE(gBankTarget)
                && gBattleMoves[gCurrentMove].split != SPLIT_STATUS && gBattleMons[gBankTarget].statStages[STAT_ATK] <= 11)
            {
                gBattleMons[gBankTarget].statStages[STAT_ATK-1]++;
                BattleScriptPushCursor();
                gBattlescriptCurrInstr = BattleScript_RageIsBuilding;
                effect = TRUE;
            }
            gBattleScripting.atk49_state++;
            break;
		
        case ATK49_DEFROST: // defrosting check
            if (gBattleMons[gBankTarget].status1 & STATUS1_FREEZE
                && gBattleMons[gBankTarget].hp && gBankAttacker != gBankTarget
                && gSpecialStatuses[gBankTarget].specialDmg
                && !(gMoveResultFlags & MOVE_RESULT_NO_EFFECT) && (moveType == TYPE_FIRE || gCurrentMove == MOVE_SCALD || gCurrentMove == MOVE_STEAMERUPTION))
            {
                gBattleMons[gBankTarget].status1 &= ~(STATUS1_FREEZE);
                gActiveBattler = gBankTarget;
                EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 4, &gBattleMons[gBankTarget].status1);
                MarkBufferBankForExecution(gActiveBattler);
                BattleScriptPushCursor();
                gBattlescriptCurrInstr = BattleScript_DefrostedViaFireMove;
                effect = TRUE;
            }
            gBattleScripting.atk49_state++;
            break;
			
        case ATK49_SYNCHRONIZE_TARGET: // target synchronize
            if (AbilityBattleEffects(ABILITYEFFECT_SYNCHRONIZE, gBankTarget, 0, 0, 0))
                effect = TRUE;
            gBattleScripting.atk49_state++;
            break;
			
        case ATK49_MOVE_END_ABILITIES: // Such as abilities activating on contact(Poison Spore, Rough Skin, etc.).
            if (AbilityBattleEffects(ABILITYEFFECT_MOVE_END, gBankTarget, 0, 0, 0))
                effect = TRUE;
            gBattleScripting.atk49_state++;
            break;
			
        case ATK49_STATUS_IMMUNITY_ABILITIES: // status immunities
            if (AbilityBattleEffects(ABILITYEFFECT_IMMUNITY, 0, 0, 0, 0))
                effect = TRUE; // it loops through all battlers, so we increment after its done with all battlers
            else
                gBattleScripting.atk49_state++;
            break;
		
		case ATK49_BEAK_BLAST_BURN:
			if (CheckContact(gCurrentMove, gBankAttacker)
			&& TOOK_DAMAGE(gBankTarget)
			&& BeakBlastByte & gBitTable[gBankAttacker]
			&& CanBeBurned(gBankAttacker))
			{
				BattleScriptPushCursor();
                gBattlescriptCurrInstr = BattleScript_BeakBlastBurn;
                effect = TRUE;
			
			}
			gBattleScripting.atk49_state++;
            break;
			
        case ATK49_SYNCHRONIZE_ATTACKER: // attacker synchronize
            if (AbilityBattleEffects(ABILITYEFFECT_ATK_SYNCHRONIZE, gBankAttacker, 0, 0, 0))
                effect = TRUE;
            gBattleScripting.atk49_state++;
            break;
			
        case ATK49_CHOICE_MOVE: // update choice band move
            if (!(gHitMarker & HITMARKER_OBEYS) || holdEffectAtk != HOLD_EFFECT_CHOICE_BAND
                || gChosenMove == MOVE_STRUGGLE || (*choicedMoveAtk != 0 && *choicedMoveAtk != 0xFFFF))
                    goto LOOP;
            if (gChosenMove == MOVE_BATON_PASS && !(gMoveResultFlags & MOVE_RESULT_FAILED))
            {
                gBattleScripting.atk49_state++;
                break;
            }
            *choicedMoveAtk = gChosenMove;
            LOOP:
            {
                for (i = 0; i < MAX_MON_MOVES; i++)
                {
                    if (gBattleMons[gBankAttacker].moves[i] == *choicedMoveAtk)
                        break;
                }
                if (i == MAX_MON_MOVES)
                    *choicedMoveAtk = 0;

                gBattleScripting.atk49_state++;
            }
            break;
			
		//Not really necessary
      /*case ATK49_CHANGED_ITEMS: // changed held items
            for (i = 0; i < gBattlersCount; i++)
            {
                u16* changedItem = &gBattleStruct->changedItems[i];
                if (*changedItem != 0)
                {
                    gBattleMons[i].item = *changedItem;
                    *changedItem = 0;
                }
            }
            gBattleScripting.atk49_state++;
            break; */
/*
		case ATK49_ITEM_EFFECTS_END_TURN_TARGET:
			if (ItemBattleEffects(ItemEffects_EndTurn, gBankTarget, TRUE, FALSE))
                effect = TRUE;
			gBattleScripting.atk49_state++;
			break;
		
		case ATK49_ITEM_EFFECTS_END_TURN_ATTACKER:
			if (ItemBattleEffects(ItemEffects_EndTurn, gBankAttacker, TRUE, FALSE))
                effect = TRUE;
			gBattleScripting.atk49_state++;
			break;

        case ATK49_ITEM_EFFECTS_CONTACT_ATTACKER:
            if (ItemBattleEffects(ItemEffects_ContactAttacker, gBankAttacker, TRUE, FALSE)) //Attacker comes first because it can be switched
				effect = TRUE;																//out with Red Card
			gBattleScripting.atk49_state++;
			break;
			
        case ATK49_ITEM_EFFECTS_CONTACT_TARGET:
            if (ItemBattleEffects(ItemEffects_ContactTarget, gBankTarget, TRUE, FALSE))
				effect = TRUE;
			gBattleScripting.atk49_state++;
			break;
			
        case ATK49_ATTACKER_INVISIBLE: // make attacker sprite invisible
            if (gStatuses3[gBankAttacker] & (STATUS3_SEMI_INVULNERABLE)
                && gHitMarker & HITMARKER_NO_ANIMATIONS)
            {
                gActiveBattler = gBankAttacker;
                EmitSpriteInvisibility(0, TRUE);
                MarkBufferBankForExecution(gActiveBattler);
                gBattleScripting.atk49_state++;
                return;
            }
            gBattleScripting.atk49_state++;
            break;
		
        case ATK49_ATTACKER_VISIBLE: // make attacker sprite visible
            if (gMoveResultFlags & MOVE_RESULT_NO_EFFECT
                || !(gStatuses3[gBankAttacker] & (STATUS3_SEMI_INVULNERABLE))
                || WasUnableToUseMove(gBankAttacker))
            {
                gActiveBattler = gBankAttacker;
                EmitSpriteInvisibility(0, FALSE);
                MarkBufferBankForExecution(gActiveBattler);
                gStatuses3[gBankAttacker] &= ~(STATUS3_SEMI_INVULNERABLE);
                gSpecialStatuses[gBankAttacker].restoredBattlerSprite = 1;
                gBattleScripting.atk49_state++;
                return;
            }
            gBattleScripting.atk49_state++;
            break;
		
        case ATK49_TARGET_VISIBLE: // make target sprite visible
            if (!gSpecialStatuses[gBankTarget].restoredBattlerSprite && gBankTarget < gBattlersCount
                && !(gStatuses3[gBankTarget] & STATUS3_SEMI_INVULNERABLE))
            {
                gActiveBattler = gBankTarget;
                EmitSpriteInvisibility(0, FALSE);
                MarkBufferBankForExecution(gActiveBattler);
                gStatuses3[gBankTarget] &= ~(STATUS3_SEMI_INVULNERABLE);
                gBattleScripting.atk49_state++;
                return;
            }
            gBattleScripting.atk49_state++;
            break;
		
        case ATK49_SUBSTITUTE: // update substitute
            for (i = 0; i < gBattlersCount; i++)
            {
                if (gDisableStructs[i].substituteHP == 0)
                    gBattleMons[i].status2 &= ~(STATUS2_SUBSTITUTE);
            }
            gBattleScripting.atk49_state++;
            break;
		
        case ATK49_UPDATE_LAST_MOVES:
            if (gHitMarker & HITMARKER_SWAP_ATTACKER_TARGET)
            {
                gActiveBattler = gBankAttacker;
                gBankAttacker = gBankTarget;
                gBankTarget = gActiveBattler;
                gHitMarker &= ~(HITMARKER_SWAP_ATTACKER_TARGET);
            }
            if (gHitMarker & HITMARKER_ATTACKSTRING_PRINTED)
            {
                gLastPrintedMoves[gBankAttacker] = gChosenMove;
            }
            if (!(gAbsentBattlerFlags & gBitTable[gBankAttacker])
                && !(gBattleStruct->field_91 & gBitTable[gBankAttacker])
                && gBattleMoves[originallyUsedMove].effect != EFFECT_BATON_PASS)
            {
                if (gHitMarker & HITMARKER_OBEYS)
                {
                    gLastMoves[gBankAttacker] = gChosenMove;
                    gLastResultingMoves[gBankAttacker] = gCurrentMove;
                }
                else
                {
                    gLastMoves[gBankAttacker] = 0xFFFF;
                    gLastResultingMoves[gBankAttacker] = 0xFFFF;
                }

                if (!(gHitMarker & HITMARKER_FAINTED(gBankTarget)))
                    gLastHitBy[gBankTarget] = gBankAttacker;

                if (gHitMarker & HITMARKER_OBEYS && !(gMoveResultFlags & MOVE_RESULT_NO_EFFECT))
                {
                    if (gChosenMove == 0xFFFF)
                    {
                        gLastLandedMoves[gBankTarget] = gChosenMove;
                    }
                    else
                    {
                        gLastLandedMoves[gBankTarget] = gCurrentMove;
                        gLastHitByType[gBankTarget] = moveType;
                    }
                }
                else
                {
                    gLastLandedMoves[gBankTarget] = 0xFFFF;
                }
            }
            gBattleScripting.atk49_state++;
            break;
		
        case ATK49_MIRROR_MOVE: // mirror move
            if (!(gAbsentBattlerFlags & gBitTable[gBankAttacker]) && !(gBattleStruct->field_91 & gBitTable[gBankAttacker])
                && gBattleMoves[originallyUsedMove].flags & FLAG_MIRROR_MOVE_AFFECTED && gHitMarker & HITMARKER_OBEYS
                && gBankAttacker != gBankTarget && !(gHitMarker & HITMARKER_FAINTED(gBankTarget))
                && !(gMoveResultFlags & MOVE_RESULT_NO_EFFECT))
            {
                u8 target, attacker;

                *(gBattleStruct->lastTakenMove + gBankTarget * 2 + 0) = gChosenMove;
                *(gBattleStruct->lastTakenMove + gBankTarget * 2 + 1) = gChosenMove >> 8;

                target = gBankTarget;
                attacker = gBankAttacker;
                *(attacker * 2 + target * 8 + (u8*)(gBattleStruct->lastTakenMoveFrom) + 0) = gChosenMove;

                target = gBankTarget;
                attacker = gBankAttacker;
                *(attacker * 2 + target * 8 + (u8*)(gBattleStruct->lastTakenMoveFrom) + 1) = gChosenMove >> 8;
            }
            gBattleScripting.atk49_state++;
            break;
		
		case ATK49_MAGICIAN:
			if (ABILITY(gBankAttacker) == ABILITY_MAGICIAN
			&& ITEM(gBankAttacker) == 0
			&& ITEM(gBankTarget)
			&& gBattleMons[gBankTarget].hp
			&& !MoveBlockedBySubstitute(gCurrentMove, gBankAttacker, gBankTarget)
			&& gMultiHitCounter <= 1
			&& TOOK_DAMAGE(gBankTarget)
			&& CanTransferItem(gBattleMons[gBankTarget].species, ITEM(gBankTarget), GetBankPartyData(gBankTarget));
			&& CanTransferItem(gBattleMons[gBankAttacker].species, ITEM(gBankTarget), GetBankPartyData(gBankAttacker))
			&& ABILITY(gBankTarget) != ABILITY_STICKYHOLD;
			{
				BattleScriptPushCursor();
                gBattlescriptCurrInstr = BattleScript_Magician;
				effect = 1;
			}
            gBattleScripting.atk49_state++;
            break;
			
        case ATK49_NEXT_TARGET: // For moves hitting two opposing Pokemon.
            if (!(gHitMarker & HITMARKER_UNABLE_TO_USE_MOVE) && gBattleTypeFlags & BATTLE_TYPE_DOUBLE
                && !gProtectStructs[gBankAttacker].chargingTurn && gBattleMoves[gCurrentMove].target == MOVE_TARGET_BOTH
                && !(gHitMarker & HITMARKER_NO_ATTACKSTRING))
            {
                u8 battlerId = GetBattlerAtPosition(BATTLE_PARTNER(GetBattlerPosition(gBankTarget)));
                if (gBattleMons[battlerId].hp != 0)
                {
                    gBankTarget = battlerId;
                    gHitMarker |= HITMARKER_NO_ATTACKSTRING;
                    gBattleScripting.atk49_state = 0;
                    MoveValuesCleanUp();
                    BattleScriptPush(gBattleScriptsForMoveEffects[gBattleMoves[gCurrentMove].effect]);
                    gBattlescriptCurrInstr = BattleScript_FlushMessageBox;
                    return;
                }
                else
                {
                    gHitMarker |= HITMARKER_NO_ATTACKSTRING;
                }
            }
            gBattleScripting.atk49_state++;
            break;
			
		case ATK49_LIFE_ORB_RECOIL:
			if (ITEM_EFFECT(gBankAttacker) == ITEM_EFFECT_LIFE_ORB
			&& gMultiHitCounter <= 1
			&& TOOK_DAMAGE(gBankTarget)
			&& ABILITY(gBankAttacker) != ABILITY_MAGICGUARD
			&& gBattleMons[gBankAttacker].hp
			&& !SheerForceCheck())
			{
				gBattleMoveDamage = MathMax(1, udivsi(gBattleMons[gBankAttacker].maxHP, 10));
				BattleScriptPushCursor();
                gBattlescriptCurrInstr = BattleScript_LifeOrbDamage;
				effect = 1;
			}
			gBattleScripting.atk49_state++;
			break;
		
		case ATK49_PICKPOCKET:
			if (ABILITY(gBankTarget) == ABILITY_PICKPOCKET
			&& CheckContact(gCurrentMove, gBankAttacker)
			&& gBattleMons[gBankTarget].hp
			&& gMultiHitCounter <= 1
			&& ITEM(gBankAttacker)
			&& ITEM(gBankTarget) == 0
			&& ABILITY(gBankAttacker) != ABILITY_STICKYHOLD
			&& !MoveBlockedBySubstitute(gCurrentMove, gBankAttacker, gBankTarget)
			&& TOOK_DAMAGE(gBankTarget))
			{
				BattleScriptPushCursor();
                gBattlescriptCurrInstr = BattleScript_Pickpocket;
				effect = 1;
			}
			gBattleScripting.atk49_state++;
			break;
			
		case ATK49_RESTORE_ABILITIES:
			break;
			
		case ATK49_END_ZMOVES:
			ZMoveData->active = FALSE;
			ZMoveData->effectApplied = FALSE;
			gBattleScripting.atk49_state++;
			break;
		
        case ATK49_COUNT:
            break;
        }
		
        if (arg1 == 1 && effect == FALSE)
            gBattleScripting.atk49_state = ATK49_COUNT;
        if (arg1 == 2 && arg2 == gBattleScripting.atk49_state)
            gBattleScripting.atk49_state = ATK49_COUNT;

    } while (gBattleScripting.atk49_state != ATK49_COUNT && effect == FALSE);

    if (gBattleScripting.atk49_state == ATK49_COUNT && effect == FALSE)
        gBattlescriptCurrInstr += 3;
}*/