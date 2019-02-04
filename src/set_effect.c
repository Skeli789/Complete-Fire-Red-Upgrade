#include "defines.h"
#include "helper_functions.h"

//Update synchronize effect
//Add Contact Protect effects

extern void DoFormChange(u8 bank, u16 species, u8 ReloadType);

void SetMoveEffect(bool8 primary, u8 certainArg);

/*
u8 MoveEffectsThatIgnoreSubstitute[] = {
MOVE_EFFECT_PAYDAY,
MOVE_EFFECT_RELIC_SONG,
MOVE_EFFECT_ION_DELUGE,
MOVE_EFFECT_FLAME_BURST,
MOVE_EFFECT_LIFT_PROTECT,
MOVE_EFFECT_BREAK_SCREENS,
MOVE_EFFECT_SET_PSYCHIC_TERRAIN,
MOVE_EFFECT_REMOVE_TERRAIN,
MOVE_EFFECT_BURN_UP,
-1
};

u8 ShieldDustIgnoredEffects[] = {
 MOVE_EFFECT_SLEEP,
 MOVE_EFFECT_POISON,
 MOVE_EFFECT_BURN,
 MOVE_EFFECT_FREEZE,
 MOVE_EFFECT_PARALYSIS,
 MOVE_EFFECT_TOXIC,
 MOVE_EFFECT_CONFUSION,
 MOVE_EFFECT_FLINCH,
 MOVE_EFFECT_TRI_ATTACK,
 MOVE_EFFECT_ATK_MINUS_1,
 MOVE_EFFECT_DEF_MINUS_1,
 MOVE_EFFECT_SPD_MINUS_1,
 MOVE_EFFECT_SP_ATK_MINUS_1,
 MOVE_EFFECT_SP_DEF_MINUS_1,
 MOVE_EFFECT_ACC_MINUS_1,
 MOVE_EFFECT_EVS_MINUS_1,
 MOVE_EFFECT_THROAT_CHOP,
 MOVE_EFFECT_PREVENT_ESCAPE,
 -1
};

const u32 sStatusFlagsForMoveEffects[] =
{
    0x00000000,
    STATUS1_SLEEP,
    STATUS1_POISON,
    STATUS1_BURN,
    STATUS1_FREEZE,
    STATUS1_PARALYSIS,
    STATUS1_TOXIC_POISON,
    STATUS2_CONFUSION,
    STATUS2_FLINCHED,
    0x00000000,
    STATUS2_UPROAR,
    0x00000000,
    STATUS2_MULTIPLETURNS,
    STATUS2_WRAPPED,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    STATUS2_RECHARGE,
    0x00000000,
    0x00000000,
    STATUS2_ESCAPE_PREVENTION,
    STATUS2_NIGHTMARE,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    STATUS2_LOCK_CONFUSE,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000
};
*/
void atk15_seteffectwithchance(void) {
    u32 PercentChance;

    if (gBattleMons[gBankAttacker].ability == ABILITY_SERENEGRACE || RainbowTimers[SIDE(gBankAttacker)])
	{
        PercentChance = gBattleMoves[gCurrentMove].secondaryEffectChance * 2;
	}
    else
	{
        PercentChance = gBattleMoves[gCurrentMove].secondaryEffectChance;
	}
	
	if (!SheerForceCheck()) {
		if ((gBattleCommunication[MOVE_EFFECT_BYTE] & 0x80) && !(gMoveResultFlags & MOVE_RESULT_NO_EFFECT))
		{
			gBattleCommunication[MOVE_EFFECT_BYTE] &= 0x7F;
			SetMoveEffect(0, 0x80);
		}
		
		else if (umodsi(Random(), 100) <= PercentChance && gBattleCommunication[MOVE_EFFECT_BYTE] != 0 && !(gMoveResultFlags & MOVE_RESULT_NO_EFFECT))
		{
			if (PercentChance >= 100)
				SetMoveEffect(FALSE, 0x80);
			else
				SetMoveEffect(FALSE, 0);
		}
		
		else
			gBattlescriptCurrInstr++;
	}
	else
		gBattlescriptCurrInstr++;

    gBattleCommunication[MOVE_EFFECT_BYTE] = 0;
    gBattleScripting->multihitMoveEffect = 0;
}

/*
void SetMoveEffect(bool8 primary, u8 certain)
{
    bool8 statusChanged = FALSE;
    u8 affectsUser = 0; // 0x40 otherwise
    bool8 noSunCanFreeze = TRUE;

    if (gBattleCommunication[MOVE_EFFECT_BYTE] & MOVE_EFFECT_AFFECTS_USER)
    {
        gEffectBank = gBankAttacker; // battlerId that effects get applied on
        gBattleCommunication[MOVE_EFFECT_BYTE] &= ~(MOVE_EFFECT_AFFECTS_USER);
        affectsUser = MOVE_EFFECT_AFFECTS_USER;
        gBattleScripting->bank = gBankTarget; //Technically the attacker
    }
    else
    {
        gEffectBank = gBankTarget;
        gBattleScripting->bank = gBankAttacker;
    }

    if (gBattleMons[gEffectBank].ability == ABILITY_SHIELD_DUST 
	&& !(gHitMarker & HITMARKER_IGNORE_SAFEGUARD) //Used by contact abilities and synchronize
    && !primary 
	&& CheckTableForSpecialMoveEffect(gBattleCommunication[MOVE_EFFECT_BYTE], ShieldDustIgnoredEffects))
        INCREMENT_RESET_RETURN

    if (gSideStatuses[SIDE(gEffectBank)] & SIDE_STATUS_SAFEGUARD 
	&& !(gHitMarker & HITMARKER_IGNORE_SAFEGUARD)
	&& !(ABILITY(gBankAttacker) == ABILITY_INFILTRATOR)
	&& gEffectBank != gBankAttacker
    && !primary
	&& certain != MOVE_EFFECT_CERTAIN
	&& gBattleCommunication[MOVE_EFFECT_BYTE] <= MOVE_EFFECT_CONFUSION)
        INCREMENT_RESET_RETURN

	for (i = 0; MoveEffectsThatIgnoreSubstitute[i] != -1; ++i) {
		if (MoveEffectsThatIgnoreSubstitute[i] == gBattleCommunication[MOVE_EFFECT_BYTE])
			goto SKIP_SUBSTITUTE_CHECK
	}
	
    if (gBattleMons[gEffectBank].hp == 0
    && gBattleCommunication[MOVE_EFFECT_BYTE] != MOVE_EFFECT_STEAL_ITEM)
        INCREMENT_RESET_RETURN

    if (gBattleMons[gEffectBank].status2 & STATUS2_SUBSTITUTE 
	&& affectsUser != MOVE_EFFECT_AFFECTS_USER
	&& ABILITY(gBankAttacker) != ABILITY_INFILTRATOR)
        INCREMENT_RESET_RETURN
		
	SKIP_SUBSTITUTE_CHECK:
    if (gBattleCommunication[MOVE_EFFECT_BYTE] <= 6) // status change
    {
        switch (sStatusFlagsForMoveEffects[gBattleCommunication[MOVE_EFFECT_BYTE]]) {
			case STATUS1_SLEEP:
				// check active uproar
				if (CanBePutToSleep(gEffectBank))
				{
					CancelMultiTurnMoves(gEffectBank);
					statusChanged = TRUE;
				}
			break;
			
			case STATUS1_POISON:
			case STATUS1_TOXIC_POISON:
				if (CanBePoisoned(gEffectBank, gBankAttacker))
					statusChanged = TRUE;
				break;
				
			case STATUS1_BURN:
				if (CanBeBurned(gEffectBank))
					statusChanged = TRUE;
				break;
			
			case STATUS1_FREEZE:
				if (CanBeFrozen(gEffectBank))
				{
					CancelMultiTurnMoves(gEffectBank);
					statusChanged = TRUE;
				}
				break;
			
			case STATUS1_PARALYSIS:
				if (CanBeParalyzed(gEffectBank))
					statusChanged = TRUE;
				break;
        }
		
        if (statusChanged)
        {
            BattleScriptPush(gBattlescriptCurrInstr + 1);

            if (gBattleCommunication[MOVE_EFFECT_BYTE] == MOVE_EFFECT_SLEEP)
                gBattleMons[gEffectBank].status1 |= ((Random() & 3) + 2);
            else
                gBattleMons[gEffectBank].status1 |= sStatusFlagsForMoveEffects[gBattleCommunication[MOVE_EFFECT_BYTE]];

            gBattlescriptCurrInstr = sMoveEffectBS_Ptrs[gBattleCommunication[MOVE_EFFECT_BYTE]];

            gActiveBattler = gEffectBank;
            EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 4, &gBattleMons[gEffectBank].status1);
            MarkBufferBankForExecution(gActiveBattler);

            if (gHitMarker & HITMARKER_IGNORE_SAFEGUARD)
            {
                gBattleCommunication[MULTISTRING_CHOOSER] = 1;
                gHitMarker &= ~(HITMARKER_IGNORE_SAFEGUARD);
            }
            else
            {
                gBattleCommunication[MULTISTRING_CHOOSER] = 0;
            }
			
            return;
        }
        else
        {
            gBattleCommunication[MOVE_EFFECT_BYTE] = 0;
            gBattlescriptCurrInstr++;
            return;
        }
        return;
    }
	
    else
    {
        if (gBattleMons[gEffectBank].status2 & sStatusFlagsForMoveEffects[gBattleCommunication[MOVE_EFFECT_BYTE]])
        {
            gBattlescriptCurrInstr++;
        }
        else
        {
            u8 side;
            switch (gBattleCommunication[MOVE_EFFECT_BYTE])
            {
            case MOVE_EFFECT_CONFUSION:
                if (gBattleMons[gEffectBank].ability == ABILITY_OWN_TEMPO
                ||  gBattleMons[gEffectBank].status2 & STATUS2_CONFUSION
				||	(CheckGrounding(gEffectBank) && TerrainType == MISTY_TERRAIN))
                {
                    gBattlescriptCurrInstr++;
                }
                else
                {
                    gBattleMons[gEffectBank].status2 |= (umodsi(Random(), 4)) + 2;

                    BattleScriptPush(gBattlescriptCurrInstr + 1);
                    gBattlescriptCurrInstr = sMoveEffectBS_Ptrs[gBattleCommunication[MOVE_EFFECT_BYTE]];
                }
                break;
			
            case MOVE_EFFECT_FLINCH:
                if (gBattleMons[gEffectBank].ability == ABILITY_INNER_FOCUS)
					gBattlescriptCurrInstr++;
                else
                {
                    if (GetBattlerTurnOrderNum(gEffectBank) > gCurrentTurnActionNumber)
                        gBattleMons[gEffectBank].status2 |= sStatusFlagsForMoveEffects[gBattleCommunication[MOVE_EFFECT_BYTE]];
                    gBattlescriptCurrInstr++;
                }
                break;
			
            case MOVE_EFFECT_UPROAR:
                if (!(gBattleMons[gEffectBank].status2 & STATUS2_UPROAR))
                {
                    gBattleMons[gEffectBank].status2 |= STATUS2_MULTIPLETURNS;
                    gLockedMoves[gEffectBank] = gCurrentMove;
                    gBattleMons[gEffectBank].status2 |= ((Random() & 3) + 2) << 4;

                    BattleScriptPush(gBattlescriptCurrInstr + 1);
                    gBattlescriptCurrInstr = sMoveEffectBS_Ptrs[gBattleCommunication[MOVE_EFFECT_BYTE]];
                }
                else
                    gBattlescriptCurrInstr++;
					
                break;

            case MOVE_EFFECT_PAYDAY:
                if (SIDE(gBankAttacker) == B_SIDE_PLAYER)
                {
                    PayDayByPartyIndices[gBattlerPartyIndexes[gEffectBank]];
                }
                BattleScriptPush(gBattlescriptCurrInstr + 1);
                gBattlescriptCurrInstr = sMoveEffectBS_Ptrs[gBattleCommunication[MOVE_EFFECT_BYTE]];
                break;
			
            case MOVE_EFFECT_TRI_ATTACK:
                if (gBattleMons[gEffectBank].status1)
                {
                    gBattlescriptCurrInstr++;
                }
                else
                {
                    gBattleCommunication[MOVE_EFFECT_BYTE] = umodsi(Random(), 3) + 3;
                    SetMoveEffect(FALSE, 0);
                }
                break;
			
            case MOVE_EFFECT_CHARGING:
                gBattleMons[gEffectBank].status2 |= STATUS2_MULTIPLETURNS;
                gLockedMoves[gEffectBank] = gCurrentMove;
                gProtectStructs[gEffectBank].chargingTurn = 1;
                gBattlescriptCurrInstr++;
                break;
			
            case MOVE_EFFECT_WRAP:
                if (gBattleMons[gEffectBank].status2 & STATUS2_WRAPPED)
                {
                    gBattlescriptCurrInstr++;
                }
                else
                {
                    gBattleMons[gEffectBank].status2 |= ((Random() & 3) + 3) << 0xD;

                    gBattleStruct->wrappedMove[gEffectBank][0] = gCurrentMove;
                    gBattleStruct->wrappedMove[gEffectBank][1] = gCurrentMove >> 8;
                    gBattleStruct->wrappedBy[gEffectBank] = gBankAttacker;

                    BattleScriptPush(gBattlescriptCurrInstr + 1);
                    gBattlescriptCurrInstr = sMoveEffectBS_Ptrs[gBattleCommunication[MOVE_EFFECT_BYTE]];

                    for (gBattleCommunication[MULTISTRING_CHOOSER] = 0; ; gBattleCommunication[MULTISTRING_CHOOSER]++)
                    {
                        if (gBattleCommunication[MULTISTRING_CHOOSER] > 4)
                            break;
                        if (gTrappingMoves[gBattleCommunication[MULTISTRING_CHOOSER]] == gCurrentMove)
                            break;
                    }
                }
                break;
			
            case MOVE_EFFECT_RECOIL_25: // 25% recoil
                gBattleMoveDamage = MathMax(1, (gHpDealt) / 4);
                BattleScriptPush(gBattlescriptCurrInstr + 1);
                gBattlescriptCurrInstr = sMoveEffectBS_Ptrs[gBattleCommunication[MOVE_EFFECT_BYTE]];
                break;
			
            case MOVE_EFFECT_ATK_PLUS_1:
            case MOVE_EFFECT_DEF_PLUS_1:
            case MOVE_EFFECT_SPD_PLUS_1:
            case MOVE_EFFECT_SP_ATK_PLUS_1:
            case MOVE_EFFECT_SP_DEF_PLUS_1:
            case MOVE_EFFECT_ACC_PLUS_1:
            case MOVE_EFFECT_EVS_PLUS_1:
                if (ChangeStatBuffs(SET_STAT_BUFF_VALUE(1),
                                    gBattleCommunication[MOVE_EFFECT_BYTE] - MOVE_EFFECT_ATK_PLUS_1 + 1,
                                    affectsUser, 0))
                {
                    gBattlescriptCurrInstr++;
                }
                else
                {
                    gBattleScripting.animArg1 = gBattleCommunication[MOVE_EFFECT_BYTE] & ~(MOVE_EFFECT_AFFECTS_USER | MOVE_EFFECT_CERTAIN);
                    gBattleScripting.animArg2 = 0;
                    BattleScriptPush(gBattlescriptCurrInstr + 1);
                    gBattlescriptCurrInstr = BattleScript_StatUp;
                }
                break;
			
            case MOVE_EFFECT_ATK_MINUS_1:
            case MOVE_EFFECT_DEF_MINUS_1:
            case MOVE_EFFECT_SPD_MINUS_1:
            case MOVE_EFFECT_SP_ATK_MINUS_1:
            case MOVE_EFFECT_SP_DEF_MINUS_1:
            case MOVE_EFFECT_ACC_MINUS_1:
            case MOVE_EFFECT_EVS_MINUS_1:
                if (ChangeStatBuffs(SET_STAT_BUFF_VALUE(1) | STAT_BUFF_NEGATIVE,
                                    gBattleCommunication[MOVE_EFFECT_BYTE] - MOVE_EFFECT_ATK_MINUS_1 + 1,
                                     affectsUser, 0))
                {
                    gBattlescriptCurrInstr++;
                }
                else
                {
                    gBattleScripting.animArg1 = gBattleCommunication[MOVE_EFFECT_BYTE] & ~(MOVE_EFFECT_AFFECTS_USER | MOVE_EFFECT_CERTAIN);
                    gBattleScripting.animArg2 = 0;
                    BattleScriptPush(gBattlescriptCurrInstr + 1);
                    gBattlescriptCurrInstr = BattleScript_StatDown;
                }
                break;
				
            case MOVE_EFFECT_ATK_PLUS_2:
            case MOVE_EFFECT_DEF_PLUS_2:
            case MOVE_EFFECT_SPD_PLUS_2:
            case MOVE_EFFECT_SP_ATK_PLUS_2:
            case MOVE_EFFECT_SP_DEF_PLUS_2:
            case MOVE_EFFECT_ACC_PLUS_2:
            case MOVE_EFFECT_EVS_PLUS_2:
                if (ChangeStatBuffs(SET_STAT_BUFF_VALUE(2),
                                    gBattleCommunication[MOVE_EFFECT_BYTE] - MOVE_EFFECT_ATK_PLUS_2 + 1,
                                    affectsUser, 0))
                {
                    gBattlescriptCurrInstr++;
                }
                else
                {
                    gBattleScripting.animArg1 = gBattleCommunication[MOVE_EFFECT_BYTE] & ~(MOVE_EFFECT_AFFECTS_USER | MOVE_EFFECT_CERTAIN);
                    gBattleScripting.animArg2 = 0;
                    BattleScriptPush(gBattlescriptCurrInstr + 1);
                    gBattlescriptCurrInstr = BattleScript_StatUp;
                }
                break;
			
            case MOVE_EFFECT_ATK_MINUS_2:
            case MOVE_EFFECT_DEF_MINUS_2:
            case MOVE_EFFECT_SPD_MINUS_2:
            case MOVE_EFFECT_SP_ATK_MINUS_2:
            case MOVE_EFFECT_SP_DEF_MINUS_2:
            case MOVE_EFFECT_ACC_MINUS_2:
            case MOVE_EFFECT_EVS_MINUS_2:
                if (ChangeStatBuffs(SET_STAT_BUFF_VALUE(2) | STAT_BUFF_NEGATIVE,
                                    gBattleCommunication[MOVE_EFFECT_BYTE] - MOVE_EFFECT_ATK_MINUS_2 + 1,
                                    affectsUser, 0))
                {
                    gBattlescriptCurrInstr++;
                }
                else
                {
                    gBattleScripting.animArg1 = gBattleCommunication[MOVE_EFFECT_BYTE] & ~(MOVE_EFFECT_AFFECTS_USER | MOVE_EFFECT_CERTAIN);
                    gBattleScripting.animArg2 = 0;
                    BattleScriptPush(gBattlescriptCurrInstr + 1);
                    gBattlescriptCurrInstr = BattleScript_StatDown;
                }
                break;
				
			case MOVE_EFFECT_ATK_PLUS_3:
            case MOVE_EFFECT_DEF_PLUS_3:
            case MOVE_EFFECT_SPD_PLUS_3:
            case MOVE_EFFECT_SP_ATK_PLUS_3:
            case MOVE_EFFECT_SP_DEF_PLUS_3:
            case MOVE_EFFECT_ACC_PLUS_3:
            case MOVE_EFFECT_EVS_PLUS_3:
                if (ChangeStatBuffs(SET_STAT_BUFF_VALUE(3),
                                    gBattleCommunication[MOVE_EFFECT_BYTE] - MOVE_EFFECT_ATK_PLUS_3 + 1,
                                    affectsUser, 0))
                {
                    gBattlescriptCurrInstr++;
                }
                else
                {
                    gBattleScripting.animArg1 = gBattleCommunication[MOVE_EFFECT_BYTE] & ~(MOVE_EFFECT_AFFECTS_USER | MOVE_EFFECT_CERTAIN);
                    gBattleScripting.animArg2 = 0;
                    BattleScriptPush(gBattlescriptCurrInstr + 1);
                    gBattlescriptCurrInstr = BattleScript_StatUp;
                }
                break;
			
            case MOVE_EFFECT_ATK_MINUS_3:
            case MOVE_EFFECT_DEF_MINUS_3:
            case MOVE_EFFECT_SPD_MINUS_3:
            case MOVE_EFFECT_SP_ATK_MINUS_3:
            case MOVE_EFFECT_SP_DEF_MINUS_3:
            case MOVE_EFFECT_ACC_MINUS_3:
            case MOVE_EFFECT_EVS_MINUS_3:
                if (ChangeStatBuffs(SET_STAT_BUFF_VALUE(3) | STAT_BUFF_NEGATIVE,
                                    gBattleCommunication[MOVE_EFFECT_BYTE] - MOVE_EFFECT_ATK_MINUS_3 + 1,
                                    affectsUser, 0))
                {
                    gBattlescriptCurrInstr++;
                }
                else
                {
                    gBattleScripting.animArg1 = gBattleCommunication[MOVE_EFFECT_BYTE] & ~(MOVE_EFFECT_AFFECTS_USER | MOVE_EFFECT_CERTAIN);
                    gBattleScripting.animArg2 = 0;
                    BattleScriptPush(gBattlescriptCurrInstr + 1);
                    gBattlescriptCurrInstr = BattleScript_StatDown;
                }
                break;
			
            case MOVE_EFFECT_RECHARGE:
                gBattleMons[gEffectBank].status2 |= STATUS2_RECHARGE;
                gDisableStructs[gEffectBank].rechargeTimer = 2;
                gLockedMoves[gEffectBank] = gCurrentMove;
                gBattlescriptCurrInstr++;
                break;
			
            case MOVE_EFFECT_RAGE:
                gBattleMons[gBankAttacker].status2 |= STATUS2_RAGE;
                gBattlescriptCurrInstr++;
                break;
			
            case MOVE_EFFECT_STEAL_ITEM: //Move to CMD49
                if (!(gBattleTypeFlags & BATTLE_TYPE_TRAINER) && SIDE(gBankAttacker) == B_SIDE_OPPONENT) //Wild mons can steal items
                {
                    gBattlescriptCurrInstr++;
                    break;
                }
					
				if (ITEM(gEffectBank) == 0
				||  ITEM(gBankAttacker)
				||  !CanTransferItem(gBattleMons[gEffectBank].species, ITEM(gEffectBank), GetBankPartyData(gEffectBank))
				||  !CanTransferItem(gBattleMons[gBankAttacker].species, ITEM(gEffectBank), GetBankPartyData(gBankAttacker))
				||	gBattleMons[gBankAttacker].hp == 0)
                {
                    gBattlescriptCurrInstr++;
                    break;
                }
					
                else if (gBattleMons[gEffectBank].ability == ABILITY_STICKY_HOLD && gBattleMons[gEffectBank].hp != 0)
                {
                    BattleScriptPushCursor();
                    gBattlescriptCurrInstr = BattleScript_NoItemSteal;

                    gLastUsedAbility = gBattleMons[gEffectBank].ability;
                    RecordAbilityBattle(gEffectBank, gLastUsedAbility);
                }
					
                else
                {
                    gLastUsedItem = gBattleMons[gEffectBank].item;
                    gBattleMons[gEffectBank].item = 0;
					gBattleMons[gBankAttacker].item = gLastUsedItem;

                    gActiveBattler = gBankAttacker;
                    EmitSetMonData(0, REQUEST_HELDITEM_BATTLE, 0, 2, &gLastUsedItem);
                    MarkBufferBankForExecution(gActiveBattler);

                    gActiveBattler = gEffectBank;
                    EmitSetMonData(0, REQUEST_HELDITEM_BATTLE, 0, 2, &gBattleMons[gActiveBattler].item);
                    MarkBufferBankForExecution(gActiveBattler);

                    BattleScriptPush(gBattlescriptCurrInstr + 1);
                    gBattlescriptCurrInstr = BattleScript_ItemSteal;

                    gBattleStruct->choicedMove[gEffectBank] = 0;
                }
                break;
			
            case MOVE_EFFECT_PREVENT_ESCAPE:
				if (!IsOfType(gEffectBank, TYPE_GHOST)
				{
					gBattleMons[gEffectBank].status2 |= STATUS2_ESCAPE_PREVENTION;
					gDisableStructs[gEffectBank]->battlerPreventingEscape = gBankAttacker;
					gBattlescriptCurrInstr++;
				}
                break;
			
            case MOVE_EFFECT_NIGHTMARE:
				if (gBattleMons[gEffectBank].status1 & STATUS1_SLEEP)
				{
					gBattleMons[gEffectBank].status2 |= STATUS2_NIGHTMARE;
					gBattlescriptCurrInstr++;
				}
                break;
			
            case MOVE_EFFECT_ALL_STATS_UP: ///Make case for Clangorous SoulBlaze b/c it hits two targets
                BattleScriptPush(gBattlescriptCurrInstr + 1);
                gBattlescriptCurrInstr = BattleScript_AllStatsUp;
                break;
			
            case MOVE_EFFECT_RAPIDSPIN:
                BattleScriptPush(gBattlescriptCurrInstr + 1);
                gBattlescriptCurrInstr = BattleScript_RapidSpinAway;
                break;
			
            case MOVE_EFFECT_REMOVE_PARALYSIS: // Smelling salts
                if (!(gBattleMons[gEffectBank].status1 & STATUS1_PARALYSIS))
                {
                    gBattlescriptCurrInstr++;
                }
                else
                {
                    gBattleMons[gEffectBank].status1 &= ~(STATUS1_PARALYSIS);

                    gActiveBattler = gEffectBank;
                    EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 4, &gBattleMons[gActiveBattler].status1);
                    MarkBufferBankForExecution(gActiveBattler);

                    BattleScriptPush(gBattlescriptCurrInstr + 1);
                    gBattlescriptCurrInstr = BattleScript_TargetPRLZHeal;
                }
                break;
				
			case MOVE_EFFECT_REMOVE_SLEEP: // Wake-Up-Slap
                if (!(gBattleMons[gEffectBank].status1 & STATUS1_SLEEP))
                {
                    gBattlescriptCurrInstr++;
                }
                else
                {
                    gBattleMons[gEffectBank].status1 &= ~(STATUS1_SLEEP);

                    gActiveBattler = gEffectBank;
                    EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 4, &gBattleMons[gActiveBattler].status1);
                    MarkBufferBankForExecution(gActiveBattler);

                    BattleScriptPush(gBattlescriptCurrInstr + 1);
                    gBattlescriptCurrInstr = BattleScript_TargetSleepHeal;
                }
                break;

            case MOVE_EFFECT_REMOVE_BURN: // Sparkling Aria
                if (!(gBattleMons[gEffectBank].status1 & STATUS1_BURN))
                {
                    gBattlescriptCurrInstr++;
                }
                else
                {
                    gBattleMons[gEffectBank].status1 &= ~(STATUS1_BURN);

                    gActiveBattler = gEffectBank;
                    EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 4, &gBattleMons[gActiveBattler].status1);
                    MarkBufferBankForExecution(gActiveBattler);

                    BattleScriptPush(gBattlescriptCurrInstr + 1);
                    gBattlescriptCurrInstr = BattleScript_TargetBurnHeal;
                }
                break;
			
            case MOVE_EFFECT_ATK_DEF_DOWN: // SuperPower
                BattleScriptPush(gBattlescriptCurrInstr + 1);
                gBattlescriptCurrInstr = BattleScript_AtkDefDown;
                break;
			
            case MOVE_EFFECT_RECOIL_33: // Double Edge
                gBattleMoveDamage = MathMax(1, udivsi(gHpDealt, 3));
                BattleScriptPush(gBattlescriptCurrInstr + 1);
                gBattlescriptCurrInstr = sMoveEffectBS_Ptrs[gBattleCommunication[MOVE_EFFECT_BYTE]];
                break;
				
			case MOVE_EFFECT_RECOIL_50: //Head Smash
                gBattleMoveDamage = MathMax(1, udivsi(gHpDealt, 2));
                BattleScriptPush(gBattlescriptCurrInstr + 1);
                gBattlescriptCurrInstr = sMoveEffectBS_Ptrs[MOVE_EFFECT_RECOIL_33];
                break;
			
            case MOVE_EFFECT_THRASH:
                if (gBattleMons[gEffectBank].status2 & STATUS2_LOCK_CONFUSE)
                {
                    gBattlescriptCurrInstr++;
                }
                else
                {
                    gBattleMons[gEffectBank].status2 |= STATUS2_MULTIPLETURNS;
                    gLockedMoves[gEffectBank] = gCurrentMove;
                    gBattleMons[gEffectBank].status2 |= (((Random() & 1) + 2) << 0xA);
                }
                break;
			
            case MOVE_EFFECT_KNOCK_OFF: //Call from CMD49
                if (!(gBattleTypeFlags & BATTLE_TYPE_TRAINER) && SIDE(gBankAttacker) == B_SIDE_OPPONENT) //Wild mons can't knock off items
                {
                    gBattlescriptCurrInstr++;
                    break;
                }
					
				else if (ITEM(gEffectBank) == 0
				||  !CanTransferItem(gBattleMons[gEffectBank].species, ITEM(gEffectBank), GetBankPartyData(gEffectBank))
				||	gBattleMons[gBankAttacker].hp == 0)
                {
                    gBattlescriptCurrInstr++;
                    break;
                }
				
                else if (ABILITY(gEffectBank) == ABILITY_STICKY_HOLD && gBattleMons[gEffectBank].hp)
                {
                    gLastUsedAbility = ABILITY_STICKY_HOLD;
                    gBattlescriptCurrInstr = BattleScript_StickyHoldActivates;
                    RecordAbilityBattle(gEffectBank, gLastUsedAbility);
					break;
                }
				
                else
                {
                    gLastUsedItem = gBattleMons[gEffectBank].item;
                    gBattleMons[gEffectBank].item = 0;

                    gActiveBattler = gEffectBank;
                    EmitSetMonData(0, REQUEST_HELDITEM_BATTLE, 0, 2, &gBattleMons[gActiveBattler].item);
                    MarkBufferBankForExecution(gActiveBattler);

                    BattleScriptPush(gBattlescriptCurrInstr + 1);
                    gBattlescriptCurrInstr = BattleScript_KnockedOff;

                    gBattleStruct->choicedMove[gEffectBank] = 0;
                }
                break;
			
            case MOVE_EFFECT_SP_ATK_TWO_DOWN: // Overheat
                BattleScriptPush(gBattlescriptCurrInstr + 1);
                gBattlescriptCurrInstr = BattleScript_SAtkDown2;
                break;
            }
			
			case MOVE_EFFECT_BURN_BERRY:
				if ((ITEM_POCKET(gEffectBank) == POCKET_BERRY_POUCH || EFFECT(gEffectBank) == ITEM_EFFECT_GEMS)
				&&  ABILITY(gEffectBank) != ABILITY_STICKYHOLD)
				{
					IncinerateCounters[gEffectBank] = TRUE;
					
					gLastUsedItem = gBattleMons[gEffectBank].item;
					gBattleMons[gEffectBank].item = 0;
					
					gActiveBattler = gEffectBank;
                    EmitSetMonData(0, REQUEST_HELDITEM_BATTLE, 0, 2, &gBattleMons[gActiveBattler].item);
                    MarkBufferBankForExecution(gActiveBattler);

                    BattleScriptPush(gBattlescriptCurrInstr + 1);
                    gBattlescriptCurrInstr = BattleScript_BerryBurned;
				}
				else
					gBattlescriptCurrInstr++;
				
				break;

			case MOVE_EFFECT_EAT_BERRY: //Call from CMD49 after Contact Items but Before End Turn items
				if (ITEM_POCKET(gEffectBank) == POCKET_BERRY_POUCH
				&&  ABILITY(gEffectBank) != ABILITY_STICKYHOLD)
				{
					gLastUsedItem = gBattleMons[gEffectBank].item;
					gBattleMons[gEffectBank].item = 0;
					
					gActiveBattler = gEffectBank;
                    EmitSetMonData(0, REQUEST_HELDITEM_BATTLE, 0, 2, &gBattleMons[gActiveBattler].item);
                    MarkBufferBankForExecution(gActiveBattler);

                    BattleScriptPush(gBattlescriptCurrInstr + 1);
                    gBattlescriptCurrInstr = BattleScript_PluckEat;
				}
				else
					gBattlescriptCurrInstr++;
			
			case MOVE_EFFECT_BRING_DOWN:	;
				bool8 smackedDown = FALSE;
				
				if (gStatuses3[gEffectBank] & (STATUS3_SKY_DROP_ATTACKER | STATUS3_SKY_DROP_TARGET | STATUS3_ROOTED)
				||  EFFECT(gEffectBank) == ITEM_EFFECT_IRON_BALL
				||  GravityTimer)
				{
					gBattlescriptCurrInstr++;
					break;
				}

				if (IsOfType(gEffectBank, TYPE_FLYING)
				||  ABILITY(gEffectBank) == ABILITY_LEVITATE
				||  (gStatuses3[gEffectBank] & (STATUS3_LEVITATING | STATUSE3_TELEKINESIS)))
				{
					gStatuses3[gEffectBank] &= ~(STATUS3_LEVITATING | STATUSE3_TELEKINESIS);
					gStatuses3[gEffectBank] |= STATUS3_SMACKED_DOWN;
					smackedDown = TRUE;
				}
				
				if (gStatuses3[gEffectBank] & STATUS3_IN_AIR)
				{
					CancelMultiTurnMoves(gEffectBank);
					gStatuses3[gEffectBank] ^= STATUS3_IN_AIR;
					gStatuses3[gEffectBank] |= STATUS3_SMACKED_DOWN;
					NoMoreMovingThisTurn |= gBitTable[gEffectBank];
					
					gActiveBattler = gEffectBank;
					EmitSpriteInvisibility(0, FALSE);
					MarkBufferBankForExecution(gActiveBattler);
					gSpecialStatuses[gEffectBank].restoredBattlerSprite = 1;
					
					smackedDown = TRUE;
				}
				
				if (smackedDown)
				{
					BattleScriptPush(gBattlescriptCurrInstr + 1);
					gBattleScripting->bank = gEffectBank;
                    gBattlescriptCurrInstr = BattleScript_BroughtDown;
				}
				else
					gBattlescriptCurrInstr++;
				break;
			
			case MOVE_EFFECT_RELIC_SONG:
				switch (SPECIES(gEffectBank)) {
					case PKMN_MELOETTA:
						DoFormChange(gEffectBank, PKMN_MELOETTA_PIROUTTE, TRUE);
						BattleScriptPush(gBattlescriptCurrInstr + 1);
						gBattlescriptCurrInstr = BattleScript_MeloettaTransformed;
						break;
					case PKMN_MELOETTA_PIROUTTE:
						DoFormChange(gEffectBank, PKMN_MELOETTA, TRUE);
						BattleScriptPush(gBattlescriptCurrInstr + 1);
						gBattlescriptCurrInstr = BattleScript_MeloettaTransformed;
						break;
					default:
						gBattlescriptCurrInstr++;
						break;
				}
				break;
			
			case MOVE_EFFECT_ION_DELUGE:
				IonDelugeTimer = 1;
				BattleScriptPush(gBattlescriptCurrInstr + 1);
                gBattlescriptCurrInstr = BattleScript_IonShower;
				break;
			
			case MOVE_EFFECT_THROAT_CHOP:
				ThroatChopTimers[gEffectBank] = 2;
				BattleScriptPush(gBattlescriptCurrInstr + 1);
                gBattlescriptCurrInstr = BattleScript_ThroatChopEffect;
				break;
			
			case MOVE_EFFECT_FLAME_BURST:
				if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE
				&&  ABILITY(PARTNER(gEffectBank)) != ABILITY_MAGICGUARD)
				{
					gBattleMoveDamage = MathMax(1, gBattleMons[PARTNER(gEffectBank)].maxHP / 16);
					gBattleScripting->bank = PARTNER(gEffectBank);
					BattleScriptPush(gBattlescriptCurrInstr + 1);
					gBattlescriptCurrInstr = BattleScript_BurstingFlame;
				}
				else
					gBattlescriptCurrInstr++;
				
				break;
				
			case MOVE_EFFECT_RESET_STAT_CHANGES:
				for (i = 0; i < BATTLE_STATS_NO-1; ++i)
					gBattleMons[gEffectBank].statStages[i] = 6;
				
				BattleScriptPush(gBattlescriptCurrInstr + 1);
				gBattlescriptCurrInstr = BattleScript_TargetStatsReset;
				break;
				
			case MOVE_EFFECT_LIFT_PROTECT:
				if (LiftProtect(gEffectBank)
				{
					BattleScriptPush(gBattlescriptCurrInstr + 1);
					gBattlescriptCurrInstr = BattleScript_LiftProtect;
				}
				else
					gBattlescriptCurrInstr++;
				
				break;
				
			case MOVE_EFFECT_SUPPRESS_ABILITY:	
				if (!(gStatuses3[gEffectBank) & STATUS3_ABILITY_SUPPRESSED)
				&&  BankMovedBefore(gEffectBank, gBankAttacker))
				{
					u8* atkAbilityLoc, *defAbilityLoc;
				
					//Get correct location of ability
					if (DisabledMoldBreakerAbilities[gEffectBank])
						atkAbilityLoc = &(DisabledMoldBreakerAbilities[gEffectBank]);
					else
						atkAbilityLoc = &(gBattleMons[gBankAttacker].ability);
						
					gStatuses3[gEffectBank) |= STATUS3_ABILITY_SUPPRESSED;
					SuppressedAbilities[gEffectBank] = *atkAbilityLoc;
					*atkAbilityLoc = 0;
					
					BattleScriptPush(gBattlescriptCurrInstr + 1);
					gBattlescriptCurrInstr = BattleScript_SuppressAbility;
				}
				else
					gBattlescriptCurrInstr++;
				
				break;
				
			case MOVE_EFFECT_BREAK_SCREENS: ;
				u8 side = SIDE(gBankTarget);
				if (gSideTimers[side].reflectTimer 
				||  gSideTimers[side].lightscreenTimer 
				||  AuroraVeilTimers[side]) 
				{
					gSideAffecting[side] &= ~(SIDE_STATUS_REFLECT);
					gSideAffecting[side] &= ~(SIDE_STATUS_LIGHTSCREEN);
					gSideTimers[side].reflectTimer = 0;
					gSideTimers[side].lightscreenTimer = 0;
					AuroraVeilTimers[side] = 0;
					BattleScriptPush(gBattlescriptCurrInstr + );
					gBattlescriptCurrInstr = BattleScript_ScreensShattered;
				}
				else
					gBattlescriptCurrInstr++;
					
			case MOVE_EFFECT_SET_PSYCHIC_TERRAIN:
				if (TerrainType != PSYCHIC_TERRAIN)
				{
					TerrainType = PSYCHIC_TERRAIN;
					TerrainCounter = 5;
					BattleScriptPush(gBattlescriptCurrInstr + 1);
					gBattlescriptCurrInstr = BattleScript_SetPsychicTerrain;
				}
				else
					gBattlescriptCurrInstr++;
					
			case MOVE_EFFECT_REMOVE_TERRAIN:
				if (TerrainType)
				{
					TerrainType = 0;
					TerrainCounter = 0;
					BattleScriptPush(gBattlescriptCurrInstr + 1);
					gBattlescriptCurrInstr = BattleScript_RemoveTerrain;
				}
				else
					gBattlescriptCurrInstr++;
				
			case MOVE_EFFECT_BURN_UP:
				break;
    }

    gBattleCommunication[MOVE_EFFECT_BYTE] = 0;
}*/