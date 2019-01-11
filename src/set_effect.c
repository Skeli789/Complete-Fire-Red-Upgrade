#include "defines.h"
#include "helper_functions.h"

void SetMoveEffect(bool8 primary, u8 certainArg);

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
				SetMoveEffect(0, 0x80);
			else
				SetMoveEffect(0, 0);
		}
		
		else
			gBattlescriptCurrInstr++;
	}
	else
		gBattlescriptCurrInstr++;

    gBattleCommunication[MOVE_EFFECT_BYTE] = 0;
    gBattleScripting->multihitMoveEffect = 0;
}

/*void SetMoveEffect(bool8 primary, u8 certainArg)
{
    #define EffectAffectsUser 0x40
    register u8 certain asm("r5") = certainArg;
    register bool32 StatusChanged asm("r10") = 0;
	register int AffectsUser asm("r6") = 0; //0x40 otherwise
    bool32 NoSunCanFreeze = 1;

    if (gBattleCommunication[MOVE_EFFECT_BYTE] & EffectAffectsUser)
    {
        gEffectBank = gBankAttacker; //bank that effects get applied on
        gBattleCommunication[MOVE_EFFECT_BYTE] &= ~(EffectAffectsUser);
        AffectsUser = EffectAffectsUser;
        gBattleStruct->scriptingActive = gBankTarget; //theoretically the attacker
    }
    else
    {
        gEffectBank = gBankTarget;
        gBattleStruct->scriptingActive = gBankAttacker;
    }

    if (gBattleMons[gEffectBank].ability == ABILITY_SHIELD_DUST && !(gHitMarker & HITMARKER_IGNORE_SAFEGUARD) &&
        !primary && gBattleCommunication[MOVE_EFFECT_BYTE] <= 9)
        {gBattlescriptCurrInstr++; return;}

    if (gSideAffecting[GetBattlerPosition(gEffectBank) & 1] & SIDE_STATUS_SAFEGUARD && !(gHitMarker & HITMARKER_IGNORE_SAFEGUARD) &&
        !primary && gBattleCommunication[MOVE_EFFECT_BYTE] <= 7)
        {gBattlescriptCurrInstr++; return;}

    //make sure at least ONE HP except payday and thief
    if (gBattleMons[gEffectBank].hp == 0 && gBattleCommunication[MOVE_EFFECT_BYTE] != 0xB && gBattleCommunication[MOVE_EFFECT_BYTE] != 0x1F)
        {gBattlescriptCurrInstr++; return;}

    if (gBattleMons[gEffectBank].status2 & STATUS2_SUBSTITUTE && AffectsUser != EffectAffectsUser)
        {gBattlescriptCurrInstr++; return;}

    if (gBattleCommunication[MOVE_EFFECT_BYTE] <= 6) //status change
    {
        switch (sStatusFlagsForMoveEffects[gBattleCommunication[MOVE_EFFECT_BYTE]])
        {
        case STATUS_SLEEP:
            //check active uproar
            if (gBattleMons[gEffectBank].ability != ABILITY_SOUNDPROOF)
            {
                for (gActiveBattler = 0; gActiveBattler < gBattlersCount && !(gBattleMons[gActiveBattler].status2 & STATUS2_UPROAR); gActiveBattler++) {}
            }
            else
                gActiveBattler = gBattlersCount;
            if (gBattleMons[gEffectBank].status1) {break;}
            if (gActiveBattler != gBattlersCount) {break;} //nice way of checking uproar...
            if (gBattleMons[gEffectBank].ability == ABILITY_VITAL_SPIRIT) {break;}
            if (gBattleMons[gEffectBank].ability == ABILITY_INSOMNIA) {break;}

            CancelMultiTurnMoves(gEffectBank);
            StatusChanged = 1;
            break;
        case STATUS_POISON:
            if (gBattleMons[gEffectBank].ability == ABILITY_IMMUNITY && (primary == 1 || certain == 0x80))
            {
                gLastUsedAbility = ABILITY_IMMUNITY;
                RecordAbilityBattle(gEffectBank, ABILITY_IMMUNITY);
                BattleScriptPush(gBattlescriptCurrInstr + 1);
            //_0801E664:
                gBattlescriptCurrInstr = BattleScript_PSNPrevention;
                if (gHitMarker & HITMARKER_IGNORE_SAFEGUARD)
                {
                    gBattleCommunication[MULTISTRING_CHOOSER] = 1;
                    gHitMarker &= ~(HITMARKER_IGNORE_SAFEGUARD);
                    return;
                }
                else
                    {gBattleCommunication[MULTISTRING_CHOOSER] = 0; return;}
            }
            if ((gBattleMons[gEffectBank].type1 == TYPE_POISON || gBattleMons[gEffectBank].type2 == TYPE_POISON || gBattleMons[gEffectBank].type1 == TYPE_STEEL || gBattleMons[gEffectBank].type2 == TYPE_STEEL)
                && !(gHitMarker & HITMARKER_IGNORE_SAFEGUARD) && (primary == 1 || certain == 0x80))
            {
                BattleScriptPush(gBattlescriptCurrInstr + 1);
                gBattlescriptCurrInstr = BattleScript_PSNPrevention;
                gBattleCommunication[MULTISTRING_CHOOSER] = 2;
                return;
            }
            if (gBattleMons[gEffectBank].type1 == TYPE_POISON) {break;}
            if (gBattleMons[gEffectBank].type2 == TYPE_POISON) {break;}
            if (gBattleMons[gEffectBank].type1 == TYPE_STEEL) {break;}
            if (gBattleMons[gEffectBank].type2 == TYPE_STEEL) {break;}
            if (gBattleMons[gEffectBank].status1) {break;}
            if (gBattleMons[gEffectBank].ability == ABILITY_IMMUNITY) {break;}

            StatusChanged = 1;
            break;
        case STATUS_BURN:
            if (gBattleMons[gEffectBank].ability == ABILITY_WATER_VEIL && (primary == 1 || certain == 0x80))
            {
                gLastUsedAbility = ABILITY_WATER_VEIL;
                RecordAbilityBattle(gEffectBank, ABILITY_WATER_VEIL);
                BattleScriptPush(gBattlescriptCurrInstr + 1);
            //_0801E664:
                gBattlescriptCurrInstr = BattleScript_BRNPrevention;
                if (gHitMarker & HITMARKER_IGNORE_SAFEGUARD)
                {
                    gBattleCommunication[MULTISTRING_CHOOSER] = 1;
                    gHitMarker &= ~(HITMARKER_IGNORE_SAFEGUARD);
                    return;
                }
                else
                    {gBattleCommunication[MULTISTRING_CHOOSER] = 0; return;}
            }
            if ((gBattleMons[gEffectBank].type1 == TYPE_FIRE || gBattleMons[gEffectBank].type2 == TYPE_FIRE)
                && !(gHitMarker & HITMARKER_IGNORE_SAFEGUARD) && (primary == 1 || certain == 0x80))
            {
                BattleScriptPush(gBattlescriptCurrInstr + 1);
                gBattlescriptCurrInstr = BattleScript_BRNPrevention;
                gBattleCommunication[MULTISTRING_CHOOSER] = 2;
                return;
            }
            if (gBattleMons[gEffectBank].type1 == TYPE_FIRE) {break;}
            if (gBattleMons[gEffectBank].type2 == TYPE_FIRE) {break;}
            if (gBattleMons[gEffectBank].ability == ABILITY_WATER_VEIL) {break;}
            if (gBattleMons[gEffectBank].status1 == 0) {break;}
            StatusChanged = 1;
            break;
        case STATUS_FREEZE:
            if (WEATHER_HAS_EFFECT && gBattleWeather & WEATHER_SUN_ANY) {NoSunCanFreeze = 0;}
            if (gBattleMons[gEffectBank].type1 == TYPE_ICE) {break;}
            if (gBattleMons[gEffectBank].type2 == TYPE_ICE) {break;}
            if (gBattleMons[gEffectBank].status1) {break;}
            if (NoSunCanFreeze == 0) {break;}
            if (gBattleMons[gEffectBank].ability == ABILITY_MAGMA_ARMOR) {break;}

            CancelMultiTurnMoves(gEffectBank);
            StatusChanged = 1;
            break;
        case STATUS_PARALYSIS:
            if (gBattleMons[gEffectBank].ability == ABILITY_LIMBER)
            {
                if ((primary == 1 || certain == 0x80))
                {
                    gLastUsedAbility = ABILITY_LIMBER;
                    RecordAbilityBattle(gEffectBank, ABILITY_LIMBER);
                    BattleScriptPush(gBattlescriptCurrInstr + 1);
                //_0801E664:
                    gBattlescriptCurrInstr = BattleScript_PRLZPrevention;
                    if (gHitMarker & HITMARKER_IGNORE_SAFEGUARD)
                    {
                        gBattleCommunication[MULTISTRING_CHOOSER] = 1;
                        gHitMarker &= ~(HITMARKER_IGNORE_SAFEGUARD);
                        return;
                    }
                    else
                        {gBattleCommunication[MULTISTRING_CHOOSER] = 0; return;}
                }
                else {break;}
            }
            if (gBattleMons[gEffectBank].status1) {break;}
            StatusChanged = 1;
            break;
        case STATUS_TOXIC_POISON:
            if (gBattleMons[gEffectBank].ability == ABILITY_IMMUNITY && (primary == 1 || certain == 0x80))
            {
                gLastUsedAbility = ABILITY_IMMUNITY;
                RecordAbilityBattle(gEffectBank, ABILITY_IMMUNITY);
                BattleScriptPush(gBattlescriptCurrInstr + 1);
            //_0801E664:
                gBattlescriptCurrInstr = BattleScript_PSNPrevention;
                if (gHitMarker & HITMARKER_IGNORE_SAFEGUARD)
                {
                    gBattleCommunication[MULTISTRING_CHOOSER] = 1;
                    gHitMarker &= ~(HITMARKER_IGNORE_SAFEGUARD);
                    return;
                }
                else
                    {gBattleCommunication[MULTISTRING_CHOOSER] = 0; return;}
            }
            if ((gBattleMons[gEffectBank].type1 == TYPE_POISON || gBattleMons[gEffectBank].type2 == TYPE_POISON || gBattleMons[gEffectBank].type1 == TYPE_STEEL || gBattleMons[gEffectBank].type2 == TYPE_STEEL)
                && !(gHitMarker & HITMARKER_IGNORE_SAFEGUARD) && (primary == 1 || certain == 0x80))
            {
                BattleScriptPush(gBattlescriptCurrInstr + 1);
                gBattlescriptCurrInstr = BattleScript_PSNPrevention;
                gBattleCommunication[MULTISTRING_CHOOSER] = 2;
                return;
            }
            if (gBattleMons[gEffectBank].status1) {break;}
            if (gBattleMons[gEffectBank].type1 != TYPE_POISON &&
                gBattleMons[gEffectBank].type2 != TYPE_POISON &&
                gBattleMons[gEffectBank].type1 != TYPE_STEEL &&
                gBattleMons[gEffectBank].type2 != TYPE_STEEL)
            {
                if (gBattleMons[gEffectBank].ability == ABILITY_IMMUNITY) {break;}
                gBattleMons[gEffectBank].status1 &= ~(0x9); //This gets (correctly) optimized out...
                StatusChanged = 1;
                break;
            }
            else
                gMoveResultFlags |= MOVE_RESULT_DOESNT_AFFECT_FOE;
            break;
        }
        if (StatusChanged == 1)
        {
            BattleScriptPush(gBattlescriptCurrInstr + 1);
            if (sStatusFlagsForMoveEffects[gBattleCommunication[MOVE_EFFECT_BYTE]] == STATUS_SLEEP)
                gBattleMons[gEffectBank].status1 |= ((Random() & 3) + 2);
            else
                gBattleMons[gEffectBank].status1 |= sStatusFlagsForMoveEffects[gBattleCommunication[MOVE_EFFECT_BYTE]];
            gBattlescriptCurrInstr = gMoveEffectBS_Ptrs[gBattleCommunication[MOVE_EFFECT_BYTE]];
            gActiveBattler = gEffectBank;
            EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 4, &gBattleMons[gEffectBank].status1);
            MarkBufferBankForExecution(gActiveBattler);
            if (gHitMarker & HITMARKER_IGNORE_SAFEGUARD)
            {
                gBattleCommunication[MULTISTRING_CHOOSER] = 1;
                gHitMarker &= ~(HITMARKER_IGNORE_SAFEGUARD);
            }
            else
                gBattleCommunication[MULTISTRING_CHOOSER] = 0;
            if (gBattleCommunication[MOVE_EFFECT_BYTE] == 2 || gBattleCommunication[MOVE_EFFECT_BYTE] == 6 || gBattleCommunication[MOVE_EFFECT_BYTE] == 5 || gBattleCommunication[MOVE_EFFECT_BYTE] == 3)
            {
                gBattleStruct->synchroniseEffect = gBattleCommunication[MOVE_EFFECT_BYTE];
                gHitMarker |= HITMARKER_SYNCHRONISE_EFFECT;
            }
            return;
        }
        else if (StatusChanged == 0)
            {gBattlescriptCurrInstr++; return;}
    }
    else
    {
        if (gBattleMons[gEffectBank].status2 & sStatusFlagsForMoveEffects[gBattleCommunication[MOVE_EFFECT_BYTE]])
        {
            gBattlescriptCurrInstr++;
            return;
        }
        switch (sStatusFlagsForMoveEffects[gBattleCommunication[MOVE_EFFECT_BYTE]])
        {
        case 7: //confusion
            if (gBattleMons[gEffectBank].ability == ABILITY_OWN_TEMPO)
                {gBattlescriptCurrInstr++; return;}
            if (gBattleMons[gEffectBank].status2 & STATUS2_CONFUSION)
                {gBattlescriptCurrInstr++; return;}
            gBattleMons[gEffectBank].status2 |= (((Random()) % 0x4)) + 2;
            BattleScriptPush(gBattlescriptCurrInstr + 1);
            gBattlescriptCurrInstr = gMoveEffectBS_Ptrs[gBattleCommunication[MOVE_EFFECT_BYTE]];
            break;
        case 8: //flinch
            if (gBattleMons[gEffectBank].ability == ABILITY_INNER_FOCUS)
            {
                if (primary == 1 || certain == 0x80)
                {
                    gLastUsedAbility = ABILITY_INNER_FOCUS;
                    RecordAbilityBattle(gEffectBank, ABILITY_INNER_FOCUS);
                    gBattlescriptCurrInstr = BattleScript_FlinchPrevention;
                    return;
                }
                else
                    {gBattlescriptCurrInstr++; return;}
            }
            else
            {
                if (BankGetTurnOrder(gEffectBank) > gCurrentTurnActionNumber)
                    gBattleMons[gEffectBank].status2 |= sStatusFlagsForMoveEffects[gBattleCommunication[MOVE_EFFECT_BYTE]];
                gBattlescriptCurrInstr++; return;
            }
            break;
        case 10: //uproar
            if (gBattleMons[gEffectBank].status2 & STATUS2_UPROAR)
                {gBattlescriptCurrInstr++; return;}
            gBattleMons[gEffectBank].status2 |= STATUS2_MULTIPLETURNS;
            gLockedMoves[gEffectBank] = gCurrentMove;
            gBattleMons[gEffectBank].status2 |= ((Random() & 3) + 2) << 4;
            BattleScriptPush(gBattlescriptCurrInstr + 1);
            gBattlescriptCurrInstr = gMoveEffectBS_Ptrs[gBattleCommunication[MOVE_EFFECT_BYTE]];
            break;
        case 11: //pay day
            if (!(GetBattlerPosition(gBankAttacker) & 1))
            {
                u16 PayDay = gPaydayMoney;
                gPaydayMoney += (gBattleMons[gBankAttacker].level * 5);
                if (PayDay > gPaydayMoney)
                    gPaydayMoney = 0xFFFF;
            }
            BattleScriptPush(gBattlescriptCurrInstr + 1);
            gBattlescriptCurrInstr = gMoveEffectBS_Ptrs[gBattleCommunication[MOVE_EFFECT_BYTE]];
            break;
        case 9: //tri attack
            if (gBattleMons[gEffectBank].status1)
                {gBattlescriptCurrInstr++; return;}
            gBattleCommunication[MOVE_EFFECT_BYTE] = Random() % 3 + 3;
            SetMoveEffect(0, 0);
            break;
        case 12: //charging move
            gBattleMons[gEffectBank].status2 |= STATUS2_MULTIPLETURNS;
            gLockedMoves[gEffectBank] = gCurrentMove;
            gProtectStructs[gEffectBank].chargingTurn = 1;
            gBattlescriptCurrInstr++;
            break;
        case 13: //wrap
            if (gBattleMons[gEffectBank].status2 & STATUS2_WRAPPED)
                {gBattlescriptCurrInstr++; return;}
            gBattleMons[gEffectBank].status2 |= ((Random() & 3) + 2) << 0xD;
            gBattleStruct->wrappedMove[gEffectBank*2] = (u8)gCurrentMove;
            (1 + gBattleStruct->wrappedMove)[gEffectBank*2] = gCurrentMove >> 8; //don't ask.
            gBattleStruct->wrappedBy[gEffectBank] = gBankAttacker;
            BattleScriptPush(gBattlescriptCurrInstr + 1);
            gBattlescriptCurrInstr = gMoveEffectBS_Ptrs[gBattleCommunication[MOVE_EFFECT_BYTE]];
            gBattleCommunication[MULTISTRING_CHOOSER] = 0;
            while (gBattleCommunication[MULTISTRING_CHOOSER] <= 4
             && gCurrentMove != gTrappingMoves[gBattleCommunication[MULTISTRING_CHOOSER]])
                gBattleCommunication[MULTISTRING_CHOOSER]++;
            break;
        case 14: //25% recoil
            gBattleMoveDamage = (gHpDealt) / 4;
            if (gBattleMoveDamage == 0)
                gBattleMoveDamage = 1;
            BattleScriptPush(gBattlescriptCurrInstr + 1);
            gBattlescriptCurrInstr = gMoveEffectBS_Ptrs[gBattleCommunication[MOVE_EFFECT_BYTE]];
            break;
        case 15 ... 21: //stat + 1
            if (ChangeStatBuffs(0x10, gBattleCommunication[MOVE_EFFECT_BYTE] + 0xF2, certain, 0)) {gBattlescriptCurrInstr++;}
            else
            {
                gBattleStruct->animArg1 = gBattleCommunication[MOVE_EFFECT_BYTE] & 0x3F; //TODO: the arg ptr is wrong by one
                gBattleStruct->animArg2 = 0;
                BattleScriptPush(gBattlescriptCurrInstr + 1);
                gBattlescriptCurrInstr = BattleScript_StatUp;
            }
            break;
        case 22 ... 28: //stat - 1
            if (ChangeStatBuffs(~(0x6f), gBattleCommunication[MOVE_EFFECT_BYTE] + 0xEB, certain, 0)) {gBattlescriptCurrInstr++;} //TODO: negation doesnt work correctly
            else
            {
                gBattleStruct->animArg1 = gBattleCommunication[MOVE_EFFECT_BYTE] & 0x3F;
                gBattleStruct->animArg2 = 0;
                BattleScriptPush(gBattlescriptCurrInstr + 1);
                gBattlescriptCurrInstr = BattleScript_StatDown;
            }
            break;
        case 39 ... 45: //stat + 2
            if (ChangeStatBuffs(0x20, gBattleCommunication[MOVE_EFFECT_BYTE] + 0xDA, certain, 0)) {gBattlescriptCurrInstr++;}
            else
            {
                gBattleStruct->animArg1 = gBattleCommunication[MOVE_EFFECT_BYTE] & 0x3F;
                gBattleStruct->animArg2 = 0;
                BattleScriptPush(gBattlescriptCurrInstr + 1);
                gBattlescriptCurrInstr = BattleScript_StatUp;
            }
            break;
        case 46 ... 52: //stat - 2
            if (ChangeStatBuffs(~(0x5f), gBattleCommunication[MOVE_EFFECT_BYTE] + 0xD3, certain, 0)) {gBattlescriptCurrInstr++;}
            else
            {
                gBattleStruct->animArg1 = gBattleCommunication[MOVE_EFFECT_BYTE] & 0x3F;
                gBattleStruct->animArg2 = 0;
                BattleScriptPush(gBattlescriptCurrInstr + 1);
                gBattlescriptCurrInstr = BattleScript_StatDown;
            }
            break;
        case 29: //recharge
            gBattleMons[gEffectBank].status2 |= STATUS2_RECHARGE;
            gDisableStructs[gEffectBank].rechargeCounter = 2;
            gLockedMoves[gEffectBank] = gCurrentMove;
            gBattlescriptCurrInstr++;
            break;
        case 30: //rage
            gBattleMons[gBankAttacker].status2 |= STATUS2_RAGE;
            gBattlescriptCurrInstr++;
            break;
        case 31: //item steal
            {
                u8 side = GetBattlerSide(gBankAttacker);
                if (GetBattlerSide(gBankAttacker) == 1 && !(gBattleTypeFlags & (BATTLE_TYPE_EREADER_TRAINER | BATTLE_TYPE_TRAINER_TOWER | BATTLE_TYPE_FRONTIER | BATTLE_TYPE_LINK)) && gTrainerBattleOpponent != 0x400)
                        {gBattlescriptCurrInstr++; return;}
                if (!(gBattleTypeFlags & (BATTLE_TYPE_EREADER_TRAINER | BATTLE_TYPE_TRAINER_TOWER | BATTLE_TYPE_FRONTIER | BATTLE_TYPE_LINK)) && gTrainerBattleOpponent != 0x400 && (gWishFutureKnock.knockedOffPokes[side] & gBitTable[gBattlerPartyIndexes[gBankAttacker]]))
                        {gBattlescriptCurrInstr++; return;}
                if (gBattleMons[gBankTarget].item && gBattleMons[gBankTarget].ability == ABILITY_STICKY_HOLD)
                {
                    BattleScriptPushCursor();
                    gBattlescriptCurrInstr = BattleScript_NoItemSteal;
                    gLastUsedAbility = gBattleMons[gBankTarget].ability;
                    RecordAbilityBattle(gBankTarget, gLastUsedAbility);
                    return;
                }
                if (gBattleMons[gBankAttacker].item)
                    {gBattlescriptCurrInstr++; return;}
                if (gBattleMons[gBankTarget].item == ITEM_ENIGMA_BERRY)
                    {gBattlescriptCurrInstr++; return;}
                if (gBattleMons[gBankTarget].item == 0)
                    {gBattlescriptCurrInstr++; return;}

				gLastUsedItem = gBattleMons[gBankTarget].item;
                USED_HELD_ITEM(bank) = gLastUsedItem;
                gBattleMons[gBankTarget].item = 0;

                gActiveBattler = gBankAttacker;
                EmitSetMonData(0, REQUEST_HELDITEM_BATTLE, 0, 2, &gLastUsedItem);
                MarkBufferBankForExecution(gBankAttacker);

                gActiveBattler = gBankTarget;
                EmitSetMonData(0, REQUEST_HELDITEM_BATTLE, 0, 2, &gBattleMons[gBankTarget].item);
                MarkBufferBankForExecution(gBankTarget);

                BattleScriptPush(gBattlescriptCurrInstr + 1);
                gBattlescriptCurrInstr = BattleScript_ItemSteal;

				CHOICED_MOVE(gBankTarget) = 0;
            }
            break;
        case 32: //escape prevention
            gBattleMons[gBankTarget].status2 |= STATUS2_RECHARGE;
            gDisableStructs[gBankTarget].bankPreventingEscape = gBankAttacker;
            gBattlescriptCurrInstr++;
            break;
        case 33: //nightmare
            gBattleMons[gBankTarget].status2 |= STATUS2_NIGHTMARE;
            gBattlescriptCurrInstr++;
            break;
        case 34: //ancientpower
            BattleScriptPush(gBattlescriptCurrInstr + 1);
            gBattlescriptCurrInstr = BattleScript_AllStatsUp;
            return;
        case 35: //break free rapidspin
            BattleScriptPush(gBattlescriptCurrInstr + 1);
            gBattlescriptCurrInstr = BattleScript_RapidSpinAway;
            return;
        case 36: //paralysis removal
            if (gBattleMons[gBankTarget].status1 & STATUS_PARALYSIS)
            {
                gBattleMons[gBankTarget].status1 &= ~(STATUS_PARALYSIS);
                gActiveBattler = gBankTarget;
                EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 4, &gBattleMons[gBankTarget].status1);
                MarkBufferBankForExecution(gActiveBattler);
                BattleScriptPush(gBattlescriptCurrInstr + 1);
                gBattlescriptCurrInstr = BattleScript_TargetPRLZHeal;
            }
            else
                {gBattlescriptCurrInstr++; return;}
            break;
        case 37: //superpower
            BattleScriptPush(gBattlescriptCurrInstr + 1);
            gBattlescriptCurrInstr = BattleScript_AtkDefDown;
            return;
        case 38: //33% recoil
            gBattleMoveDamage = gHpDealt / 3;
            if (gBattleMoveDamage == 0)
                gBattleMoveDamage = 1;
            BattleScriptPush(gBattlescriptCurrInstr + 1);
            gBattlescriptCurrInstr = gMoveEffectBS_Ptrs[gBattleCommunication[MOVE_EFFECT_BYTE]];
            break;
        case 53: //thrash
            if (!(gBattleMons[gEffectBank].status2 & STATUS2_LOCK_CONFUSE))
            {
                gBattleMons[gEffectBank].status2 |= STATUS2_MULTIPLETURNS;
                gLockedMoves[gEffectBank] = gCurrentMove;
                gBattleMons[gEffectBank].status2 |= (((Random() & 1) + 2) << 0xA);
            }
            else
                {gBattlescriptCurrInstr++; return;}
            break;
        case 54: //knock off
            if (gBattleMons[gEffectBank].ability == ABILITY_STICKY_HOLD)
            {
                if (gBattleMons[gEffectBank].item == 0)
                    {gBattlescriptCurrInstr++; return;}
                gLastUsedAbility = ABILITY_STICKY_HOLD;
                gBattlescriptCurrInstr = BattleScript_NoItemSteal;
                RecordAbilityBattle(gEffectBank, ABILITY_STICKY_HOLD);
                return;
            }
            if (gBattleMons[gEffectBank].item == 0)
                    {gBattlescriptCurrInstr++; return;}
            else
            {
                u8 side = GetBattlerSide(gEffectBank);
                gLastUsedItem = gBattleMons[gEffectBank].item;
                gBattleMons[gEffectBank].item = 0;
                gWishFutureKnock.knockedOffPokes[side] |= gBitTable[gBattlerPartyIndexes[gEffectBank]];
                BattleScriptPush(gBattlescriptCurrInstr + 1);
                gBattlescriptCurrInstr = BattleScript_KnockedOff;

                CHOICED_MOVE(gEffectBank) = 0;
            }
            break;
        case 59: //overheat
            BattleScriptPush(gBattlescriptCurrInstr + 1);
            gBattlescriptCurrInstr = BattleScript_SAtkDown2;
            return;
        }
    }
}
*/