#include "defines.h"
#include "defines_battle.h"
#include "../include/battle_string_ids.h"
#include "../include/constants/items.h"
#include "../include/random.h"

#include "../include/new/helper_functions.h"

//Update synchronize effect

#define INCREMENT_RESET_RETURN                  \
{                                               \
    gBattlescriptCurrInstr++;                   \
    gBattleCommunication[MOVE_EFFECT_BYTE] = 0; \
    return;                                     \
}

#define RESET_RETURN                            \
{                                               \
    gBattleCommunication[MOVE_EFFECT_BYTE] = 0; \
    return effect;                              \
}

#define sMoveEffectBS_Ptrs ((u8**) 0x825062C)
#define BattleScript_AllStatsUp (u8*) 0x81D8D55
#define BattleScript_RapidSpinAway (u8*) 0x81D8DF1
#define BattleScript_TargetPRLZHeal (u8*) 0x81D9177
#define BattleScript_AtkDefDown (u8*) 0x81D8F35
#define BattleScript_KnockedOff (u8*) 0x81D8F86
#define BattleScript_StatUp (u8*) 0x81D6BD1
#define BattleScript_StatDown (u8*) 0x81D6C62
#define BattleScript_SAtkDown2 (u8*) 0x81D8FEB
#define ScreensShatteredString (u8*) 0x83FC646

extern u8 BattleScript_PluckEat[];
extern u8 BattleScript_TargetSleepHeal[];
extern u8 BattleScript_TargetBurnHeal[];
extern u8 BattleScript_ItemSteal[];
extern u8 BattleScript_StickyHoldActivatesRet[];
extern u8 BattleScript_PrintCustomString[];

extern u8 IonDelugeShowerString[];
extern u8 TargetStatsResetString[];
extern u8 AbilitySuppressedString[];
extern u8 TerrainEndString[];
extern u8 RoastedBerryString[];
extern u8 gText_TargetWasInfested[];

extern u8 ChangeStatBuffs(s8 statValue, u8 statId, u8 flags, u8* BS_ptr);
extern void BringDownMons(void);

void SetMoveEffect(bool8 primary, u8 certainArg);


u8 MoveEffectsThatIgnoreSubstitute[] = 
{
	MOVE_EFFECT_PAYDAY,
	MOVE_EFFECT_ION_DELUGE,
	MOVE_EFFECT_BREAK_SCREENS,
	MOVE_EFFECT_REMOVE_TERRAIN,
	0xFF,
};

u8 ShieldDustIgnoredEffects[] = 
{
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
	//MOVE_EFFECT_THROAT_CHOP,
	MOVE_EFFECT_PREVENT_ESCAPE,
	0xFF,
};

const u32 sStatusFlagsForMoveEffects[] =
{
	[MOVE_EFFECT_SLEEP] = STATUS1_SLEEP,
	[MOVE_EFFECT_POISON] = STATUS1_POISON,
    [MOVE_EFFECT_BURN] = STATUS1_BURN,
    [MOVE_EFFECT_FREEZE] = STATUS1_FREEZE,
    [MOVE_EFFECT_PARALYSIS] = STATUS1_PARALYSIS,
    [MOVE_EFFECT_TOXIC] = STATUS1_TOXIC_POISON,
    [MOVE_EFFECT_CONFUSION] = STATUS2_CONFUSION,
    [MOVE_EFFECT_FLINCH] = STATUS2_FLINCHED,
	[MOVE_EFFECT_UPROAR] = STATUS2_UPROAR,
	[MOVE_EFFECT_CHARGING] = STATUS2_MULTIPLETURNS,
	[MOVE_EFFECT_WRAP] = STATUS2_WRAPPED,
    [MOVE_EFFECT_RECHARGE] = STATUS2_RECHARGE,
	[MOVE_EFFECT_PREVENT_ESCAPE] = STATUS2_ESCAPE_PREVENTION,
	[MOVE_EFFECT_NIGHTMARE] = STATUS2_NIGHTMARE,
	[MOVE_EFFECT_THRASH] = STATUS2_LOCK_CONFUSE,
 };

const u16 gTrappingMoves[] =
{
    MOVE_BIND, MOVE_WRAP, MOVE_FIRESPIN, MOVE_CLAMP, MOVE_WHIRLPOOL, MOVE_SANDTOMB, MOVE_MAGMASTORM, MOVE_INFESTATION, 0xFFFF
};

const u16 gWrappedStringIds[] =
{
    STRINGID_PKMNSQUEEZEDBYBIND, 
	STRINGID_PKMNWRAPPEDBY, 
	STRINGID_PKMNTRAPPEDINVORTEX,
    STRINGID_PKMNCLAMPED, 
	STRINGID_PKMNTRAPPEDINVORTEX, 
	STRINGID_PKMNTRAPPEDBYSANDTOMB,
	STRINGID_PKMNTRAPPEDINVORTEX,
	0x184,
};

void atk15_seteffectwithchance(void) {
    u32 PercentChance;
	
	if (CheckSoundMove(gCurrentMove) || ABILITY(gBankAttacker) == ABILITY_INFILTRATOR)
		gHitMarker |= HITMARKER_IGNORE_SUBSTITUTE;

    if (gBattleMons[gBankAttacker].ability == ABILITY_SERENEGRACE || gNewBS->RainbowTimers[SIDE(gBankAttacker)])
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
			{
				SetMoveEffect(FALSE, 0x80);
			}
			else
			{
				SetMoveEffect(FALSE, 0);
			}
		}
		
		else
			gBattlescriptCurrInstr++;
	}
	else
		gBattlescriptCurrInstr++;

    gBattleCommunication[MOVE_EFFECT_BYTE] = 0;
    gBattleScripting->multihitMoveEffect = 0;
}


void SetMoveEffect(bool8 primary, u8 certain)
{
    bool8 statusChanged = FALSE;
    u8 affectsUser = 0; // 0x40 otherwise

    if (gBattleCommunication[MOVE_EFFECT_BYTE] & MOVE_EFFECT_AFFECTS_USER)
    {
        gEffectBank = gBankAttacker; // battlerId that effects get applied on
        gBattleCommunication[MOVE_EFFECT_BYTE] &= ~(MOVE_EFFECT_AFFECTS_USER);
        affectsUser = MOVE_EFFECT_AFFECTS_USER;
        gBattleScripting->bank = gBankAttacker;
    }
    else
    {
        gEffectBank = gBankTarget;
        gBattleScripting->bank = gBankAttacker;
    }
	
    if (ABILITY(gEffectBank) == ABILITY_SHIELDDUST 
	&& !(gHitMarker & HITMARKER_IGNORE_SAFEGUARD) //Used by contact abilities and synchronize
    && !primary 
	&& CheckTableForSpecialMoveEffect(gBattleCommunication[MOVE_EFFECT_BYTE], ShieldDustIgnoredEffects))
	{
        ++gBattlescriptCurrInstr;
		goto CLEAR_MOVE_EFFECT_BYTE;
	}
	
    if (gSideAffecting[SIDE(gEffectBank)] & SIDE_STATUS_SAFEGUARD 
	&& !(gHitMarker & HITMARKER_IGNORE_SAFEGUARD)
	&& ABILITY(gBankAttacker) != ABILITY_INFILTRATOR
	&& gEffectBank != gBankAttacker
    && !primary
	&& certain != MOVE_EFFECT_CERTAIN
	&& gBattleCommunication[MOVE_EFFECT_BYTE] <= MOVE_EFFECT_CONFUSION)
	{
        ++gBattlescriptCurrInstr;
		goto CLEAR_MOVE_EFFECT_BYTE;
	}
	
	if (IsOfType(gEffectBank, TYPE_GRASS)
	&& (ABILITY(gEffectBank) == ABILITY_FLOWERVEIL || (gBattleTypeFlags & BATTLE_TYPE_DOUBLE && ABILITY(PARTNER(gEffectBank)) == ABILITY_FLOWERVEIL))
	&& gEffectBank != gBankAttacker
	&& gBattleCommunication[MOVE_EFFECT_BYTE] <= MOVE_EFFECT_CONFUSION)
	{
        ++gBattlescriptCurrInstr;
		goto CLEAR_MOVE_EFFECT_BYTE;
	}

	if (CheckTableForSpecialMoveEffect(gBattleCommunication[MOVE_EFFECT_BYTE], MoveEffectsThatIgnoreSubstitute))
		goto SKIP_SUBSTITUTE_CHECK;
	
    if (gBattleMons[gEffectBank].hp == 0)
	{
        ++gBattlescriptCurrInstr;
		goto CLEAR_MOVE_EFFECT_BYTE;
	}

    if (gBattleMons[gEffectBank].status2 & STATUS2_SUBSTITUTE 
	&& affectsUser != MOVE_EFFECT_AFFECTS_USER
	&& !(gHitMarker & HITMARKER_IGNORE_SUBSTITUTE))
	{
        ++gBattlescriptCurrInstr;
		goto CLEAR_MOVE_EFFECT_BYTE;
	}
		
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

            /*if (gHitMarker & HITMARKER_IGNORE_SAFEGUARD)
            {
                gBattleCommunication[MULTISTRING_CHOOSER] = 1;
                gHitMarker &= ~(HITMARKER_IGNORE_SAFEGUARD);
            }
            else
            {*/
                gBattleCommunication[MULTISTRING_CHOOSER] = 0;
            //}
			
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
            switch (gBattleCommunication[MOVE_EFFECT_BYTE])
            {
            case MOVE_EFFECT_CONFUSION:
                if (ABILITY(gEffectBank)== ABILITY_OWNTEMPO
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
                if (ABILITY(gEffectBank) == ABILITY_INNERFOCUS)
					gBattlescriptCurrInstr++;
                else
                {
                    if (gCurrentTurnActionNumber < GetBattlerTurnOrderNum(gEffectBank))
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
				if (!gNewBS->secondaryEffectApplied)
				{
					gNewBS->secondaryEffectApplied = TRUE;
					if (SIDE(gBankAttacker) == B_SIDE_PLAYER)
					{
						gNewBS->PayDayByPartyIndices[gBattlerPartyIndexes[gBankAttacker]]++;
					}
					BattleScriptPush(gBattlescriptCurrInstr + 1);
					gBattlescriptCurrInstr = sMoveEffectBS_Ptrs[gBattleCommunication[MOVE_EFFECT_BYTE]];
				}
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
                if (gBattleMons[gEffectBank].status2 & STATUS2_WRAPPED || gNewBS->secondaryEffectApplied)
                {
                    gBattlescriptCurrInstr++;
                }
                else
                {
					gNewBS->secondaryEffectApplied = TRUE;
					
					if (ITEM_EFFECT(gBankAttacker) == ITEM_EFFECT_GRIP_CLAW)
						gBattleMons[gEffectBank].status2 |= (7 << 0xD);
					else
						gBattleMons[gEffectBank].status2 |= ((Random() & 1) + 4) << 0xD;

                    gBattleStruct->wrappedMove[gEffectBank] = gCurrentMove;
                    gBattleStruct->wrappedBy[gEffectBank] = gBankAttacker;

                    BattleScriptPush(gBattlescriptCurrInstr + 1);
                    gBattlescriptCurrInstr = sMoveEffectBS_Ptrs[gBattleCommunication[MOVE_EFFECT_BYTE]];

                    for (gBattleCommunication[MULTISTRING_CHOOSER] = 0; ; gBattleCommunication[MULTISTRING_CHOOSER]++)
                    {
                        if (gBattleCommunication[MULTISTRING_CHOOSER] > 6)
                            break;
                        if (gTrappingMoves[gBattleCommunication[MULTISTRING_CHOOSER]] == gCurrentMove)
                            break;
                    }
					
					if (gCurrentMove == MOVE_INFESTATION)
						BattleStringLoader = gText_TargetWasInfested;
                }
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
                    gBattleScripting->animArg1 = gBattleCommunication[MOVE_EFFECT_BYTE] & ~(MOVE_EFFECT_AFFECTS_USER | MOVE_EFFECT_CERTAIN);
                    gBattleScripting->animArg2 = 0;
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
                    gBattleScripting->animArg1 = gBattleCommunication[MOVE_EFFECT_BYTE] & ~(MOVE_EFFECT_AFFECTS_USER | MOVE_EFFECT_CERTAIN);
                    gBattleScripting->animArg2 = 0;
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
                    gBattleScripting->animArg1 = gBattleCommunication[MOVE_EFFECT_BYTE] & ~(MOVE_EFFECT_AFFECTS_USER | MOVE_EFFECT_CERTAIN);
                    gBattleScripting->animArg2 = 0;
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
                    gBattleScripting->animArg1 = gBattleCommunication[MOVE_EFFECT_BYTE] & ~(MOVE_EFFECT_AFFECTS_USER | MOVE_EFFECT_CERTAIN);
                    gBattleScripting->animArg2 = 0;
                    BattleScriptPush(gBattlescriptCurrInstr + 1);
                    gBattlescriptCurrInstr = BattleScript_StatDown;
                }
                break;
			
			/*
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
                    gBattleScripting->animArg1 = gBattleCommunication[MOVE_EFFECT_BYTE] & ~(MOVE_EFFECT_AFFECTS_USER | MOVE_EFFECT_CERTAIN);
                    gBattleScripting->animArg2 = 0;
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
                    gBattleScripting->animArg1 = gBattleCommunication[MOVE_EFFECT_BYTE] & ~(MOVE_EFFECT_AFFECTS_USER | MOVE_EFFECT_CERTAIN);
                    gBattleScripting->animArg2 = 0;
                    BattleScriptPush(gBattlescriptCurrInstr + 1);
                    gBattlescriptCurrInstr = BattleScript_StatDown;
                }
                break;
			*/
			
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
			
            case MOVE_EFFECT_PREVENT_ESCAPE:
				if (!IsOfType(gEffectBank, TYPE_GHOST))
				{
					gBattleMons[gEffectBank].status2 |= STATUS2_ESCAPE_PREVENTION;
					gDisableStructs[gEffectBank].bankPreventingEscape = gBankAttacker;
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
			
            case MOVE_EFFECT_ALL_STATS_UP:
				if (gCurrentMove != MOVE_CLANGOROUS_SOULBLAZE || !gNewBS->secondaryEffectApplied) //Hits two targets but only gets effect once
				{
					gNewBS->secondaryEffectApplied = TRUE;
					BattleScriptPush(gBattlescriptCurrInstr + 1);
					gBattlescriptCurrInstr = BattleScript_AllStatsUp;
				}
                break;
			
            case MOVE_EFFECT_RAPIDSPIN:
                BattleScriptPush(gBattlescriptCurrInstr + 1);
                gBattlescriptCurrInstr = BattleScript_RapidSpinAway;
                break;
			
            case MOVE_EFFECT_ATK_DEF_DOWN: // SuperPower
                BattleScriptPush(gBattlescriptCurrInstr + 1);
                gBattlescriptCurrInstr = BattleScript_AtkDefDown;
                break;
			
            case MOVE_EFFECT_THRASH:
				if (gNewBS->DancerInProgress
				|| gBattleMons[gEffectBank].status2 & STATUS2_LOCK_CONFUSE)
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
			
            case MOVE_EFFECT_SP_ATK_TWO_DOWN: // Overheat
                BattleScriptPush(gBattlescriptCurrInstr + 1);
                gBattlescriptCurrInstr = BattleScript_SAtkDown2;
                break;
			
			case MOVE_EFFECT_BURN_BERRY:
				if ((ITEM_POCKET(gEffectBank) == POCKET_BERRY_POUCH || ITEM_EFFECT(gEffectBank) == ITEM_EFFECT_GEM)
				&&  ABILITY(gEffectBank) != ABILITY_STICKYHOLD)
				{
					gNewBS->IncinerateCounters[gEffectBank] = TRUE;
					
					gLastUsedItem = gBattleMons[gEffectBank].item;
					gBattleMons[gEffectBank].item = 0;
					
					gActiveBattler = gEffectBank;
                    EmitSetMonData(0, REQUEST_HELDITEM_BATTLE, 0, 2, &gBattleMons[gActiveBattler].item);
                    MarkBufferBankForExecution(gActiveBattler);

                    BattleScriptPush(gBattlescriptCurrInstr + 1);
					BattleStringLoader = RoastedBerryString;
					gBattlescriptCurrInstr = BattleScript_PrintCustomString;
				}
				else
					gBattlescriptCurrInstr++;
				
				break;
				
			case MOVE_EFFECT_RESET_STAT_CHANGES:
				for (int i = 0; i < BATTLE_STATS_NO-1; ++i)
					gBattleMons[gEffectBank].statStages[i] = 6;
				
				BattleScriptPush(gBattlescriptCurrInstr + 1);
				BattleStringLoader = TargetStatsResetString;
				gBattlescriptCurrInstr = BattleScript_PrintCustomString;
				break;
				
			case MOVE_EFFECT_SUPPRESS_ABILITY:
				if (!gNewBS->secondaryEffectApplied
				&& !(gStatuses3[gEffectBank] & STATUS3_ABILITY_SUPPRESS)
				&&  GetBattlerTurnOrderNum(gEffectBank) < gCurrentTurnActionNumber) //Target moved before attacker
				{
					gNewBS->secondaryEffectApplied = TRUE;
					u8* defAbilityLoc;
					defAbilityLoc = GetAbilityLocation(gBankTarget);
						
					gStatuses3[gEffectBank] |= STATUS3_ABILITY_SUPPRESS;
					gNewBS->SuppressedAbilities[gEffectBank] = *defAbilityLoc;
					*defAbilityLoc = 0;
					
					BattleScriptPush(gBattlescriptCurrInstr + 1);
					BattleStringLoader = AbilitySuppressedString;
					gBattlescriptCurrInstr = BattleScript_PrintCustomString;
				}
				else
					gBattlescriptCurrInstr++;
				
				break;
				
			case MOVE_EFFECT_BREAK_SCREENS: ;
				u8 side = SIDE(gBankTarget);
				if (gSideTimers[side].reflectTimer 
				||  gSideTimers[side].lightscreenTimer 
				||  gSideAffecting[side] & SIDE_STATUS_REFLECT
				||  gSideAffecting[side] & SIDE_STATUS_LIGHTSCREEN
				||  gNewBS->AuroraVeilTimers[side]) 
				{
					gSideAffecting[side] &= ~(SIDE_STATUS_REFLECT);
					gSideAffecting[side] &= ~(SIDE_STATUS_LIGHTSCREEN);
					gSideTimers[side].reflectTimer = 0;
					gSideTimers[side].lightscreenTimer = 0;
					gNewBS->AuroraVeilTimers[side] = 0;
					BattleScriptPush(gBattlescriptCurrInstr + 1);
					BattleStringLoader = ScreensShatteredString;
					gBattlescriptCurrInstr = BattleScript_PrintCustomString;
				}
				else
					gBattlescriptCurrInstr++;
				break;
			
			case MOVE_EFFECT_REMOVE_TERRAIN:
				if (TerrainType)
				{
					TerrainType = 0;
					gNewBS->TerrainTimer = 0;
					BattleScriptPush(gBattlescriptCurrInstr + 1);
					BattleStringLoader = TerrainEndString;
					gBattlescriptCurrInstr = BattleScript_PrintCustomString;
					
					gActiveBattler = 0;
					EmitDataTransfer(0, &TerrainType, 1, &TerrainType);
					MarkBufferBankForExecution(gActiveBattler);
				}
				else
					gBattlescriptCurrInstr++;
				break;
			
			default:
				gBattlescriptCurrInstr++;
			}
		}
    }

CLEAR_MOVE_EFFECT_BYTE:
	switch (gBattleCommunication[MOVE_EFFECT_BYTE]) { //Handled later
		case MOVE_EFFECT_STEAL_ITEM:
		case MOVE_EFFECT_KNOCK_OFF:
		case MOVE_EFFECT_REMOVE_PARALYSIS:
		case MOVE_EFFECT_BRING_DOWN:
		case MOVE_EFFECT_ION_DELUGE:
			gNewBS->backupMoveEffect = gBattleCommunication[MOVE_EFFECT_BYTE]; 
	}
	
	gBattleCommunication[MOVE_EFFECT_BYTE] = 0;
}

bool8 SetMoveEffect2(void)
{
	u8 affectsUser;
	bool8 effect = FALSE;

    if (gBattleCommunication[MOVE_EFFECT_BYTE] & MOVE_EFFECT_AFFECTS_USER)
    {
        gEffectBank = gBankAttacker; // battlerId that effects get applied on
        gBattleCommunication[MOVE_EFFECT_BYTE] &= ~(MOVE_EFFECT_AFFECTS_USER);
        affectsUser = MOVE_EFFECT_AFFECTS_USER;
        gBattleScripting->bank = gBankAttacker;
    }
    else
    {
        gEffectBank = gBankTarget;
        gBattleScripting->bank = gBankAttacker;
    }

	if (CheckTableForSpecialMoveEffect(gBattleCommunication[MOVE_EFFECT_BYTE], MoveEffectsThatIgnoreSubstitute))
		goto SKIP_SUBSTITUTE_CHECK;	
	
    if (gBattleMons[gEffectBank].hp == 0
    && gBattleCommunication[MOVE_EFFECT_BYTE] != MOVE_EFFECT_STEAL_ITEM)
        RESET_RETURN
	
    if (gBattleMons[gEffectBank].status2 & STATUS2_SUBSTITUTE 
	&& affectsUser != MOVE_EFFECT_AFFECTS_USER
	&& !(gHitMarker & HITMARKER_IGNORE_SUBSTITUTE))
        RESET_RETURN
	
	SKIP_SUBSTITUTE_CHECK:
    switch (gBattleCommunication[MOVE_EFFECT_BYTE]) {
        case MOVE_EFFECT_STEAL_ITEM:
            if (!(gBattleTypeFlags & BATTLE_TYPE_TRAINER) && SIDE(gBankAttacker) == B_SIDE_OPPONENT) //Wild mons can steal items
            {
                break;
            }	
			else if (ITEM(gEffectBank) == 0
			||  ITEM(gBankAttacker)
			||  !CanTransferItem(gBattleMons[gEffectBank].species, ITEM(gEffectBank), GetBankPartyData(gEffectBank))
			||  !CanTransferItem(gBattleMons[gBankAttacker].species, ITEM(gEffectBank), GetBankPartyData(gBankAttacker))
			||	gBattleMons[gBankAttacker].hp == 0)
            {
                break;
            }
            else if (gBattleMons[gEffectBank].ability == ABILITY_STICKYHOLD && gBattleMons[gEffectBank].hp != 0)
            {
                BattleScriptPushCursor();
                gBattlescriptCurrInstr = BattleScript_StickyHoldActivatesRet;

                gLastUsedAbility = gBattleMons[gEffectBank].ability;
                RecordAbilityBattle(gEffectBank, gLastUsedAbility);
				effect = TRUE;
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

                BattleScriptPushCursor();
                gBattlescriptCurrInstr = BattleScript_ItemSteal;

                gBattleStruct->choicedMove[gEffectBank] = 0;
				effect = TRUE;
            }
            break;
			
        case MOVE_EFFECT_KNOCK_OFF:
            if (!(gBattleTypeFlags & BATTLE_TYPE_TRAINER) && SIDE(gBankAttacker) == B_SIDE_OPPONENT) //Wild mons can't knock off items
            {
                break;
            }
			else if (ITEM(gEffectBank) == 0
			||  !CanTransferItem(gBattleMons[gEffectBank].species, ITEM(gEffectBank), GetBankPartyData(gEffectBank))
			||	gBattleMons[gBankAttacker].hp == 0)
            {
                break;
            }
            else if (ABILITY(gEffectBank) == ABILITY_STICKYHOLD && gBattleMons[gEffectBank].hp)
            {
                gLastUsedAbility = ABILITY_STICKYHOLD;
                gBattlescriptCurrInstr = BattleScript_StickyHoldActivatesRet;
                RecordAbilityBattle(gEffectBank, gLastUsedAbility);
				effect = TRUE;
            }
            else
            {
                gLastUsedItem = gBattleMons[gEffectBank].item;
                gBattleMons[gEffectBank].item = 0;

                gActiveBattler = gEffectBank;
                EmitSetMonData(0, REQUEST_HELDITEM_BATTLE, 0, 2, &gBattleMons[gActiveBattler].item);
                MarkBufferBankForExecution(gActiveBattler);

                BattleScriptPushCursor();
                gBattlescriptCurrInstr = BattleScript_KnockedOff;

                gBattleStruct->choicedMove[gEffectBank] = 0;
				effect = TRUE;
            }
            break;

        case MOVE_EFFECT_REMOVE_PARALYSIS: // Smelling salts
			switch (gCurrentMove) {
				case MOVE_SMELLINGSALTS:
					if (gBattleMons[gEffectBank].status1 & STATUS1_PARALYSIS)
					{
						gBattleMons[gEffectBank].status1 &= ~(STATUS1_PARALYSIS);

						gActiveBattler = gEffectBank;
						EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 4, &gBattleMons[gActiveBattler].status1);
						MarkBufferBankForExecution(gActiveBattler);

						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_TargetPRLZHeal;
						effect = TRUE;
					}
					break;
						
				case MOVE_WAKEUPSLAP:
					if (gBattleMons[gEffectBank].status1 & STATUS1_SLEEP)
					{
						gBattleMons[gEffectBank].status1 &= ~(STATUS1_SLEEP);

						gActiveBattler = gEffectBank;
						EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 4, &gBattleMons[gActiveBattler].status1);
						MarkBufferBankForExecution(gActiveBattler);

						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_TargetSleepHeal;
						effect = TRUE;
					}
					break;
						
				case MOVE_SPARKLINGARIA:
					if (gBattleMons[gEffectBank].status1 & STATUS1_BURN)
					{
						gBattleMons[gEffectBank].status1 &= ~(STATUS1_BURN);

						gActiveBattler = gEffectBank;
						EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 4, &gBattleMons[gActiveBattler].status1);
						MarkBufferBankForExecution(gActiveBattler);

						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_TargetBurnHeal;
						effect = TRUE;
					}
					break;
			}
			break;
		
		case MOVE_EFFECT_BRING_DOWN:
			if (gStatuses3[gEffectBank] & STATUS3_IN_AIR)
				goto SMACK_TGT_DOWN;
		
			if (gStatuses3[gEffectBank] & (STATUS3_SKY_DROP_ATTACKER | STATUS3_SKY_DROP_TARGET | STATUS3_ROOTED | STATUS3_SMACKED_DOWN)
			||  ITEM_EFFECT(gEffectBank) == ITEM_EFFECT_IRON_BALL
			||  gNewBS->GravityTimer)
			{
				break;
			}

			if (IsOfType(gEffectBank, TYPE_FLYING)
			||  ABILITY(gEffectBank) == ABILITY_LEVITATE
			||  (gStatuses3[gEffectBank] & (STATUS3_IN_AIR | STATUS3_LEVITATING | STATUS3_TELEKINESIS))
			||  (ITEM_EFFECT(gEffectBank) == ITEM_EFFECT_AIR_BALLOON))
			{ 
			SMACK_TGT_DOWN:
				gStatuses3[gEffectBank] |= STATUS3_SMACKED_DOWN;
				gNewBS->targetsToBringDown |= gBitTable[gEffectBank];
				gBattlescriptCurrInstr += 1;
				BringDownMons();
				gBattlescriptCurrInstr = BattleScript_PrintCustomString;
				effect = TRUE;
			}
					
			break;
		
		case MOVE_EFFECT_ION_DELUGE:
			gNewBS->IonDelugeTimer = 1;
			BattleScriptPushCursor();
			BattleStringLoader = IonDelugeShowerString;
			gBattlescriptCurrInstr = BattleScript_PrintCustomString;
			effect = TRUE;
			break;
    }

    gBattleCommunication[MOVE_EFFECT_BYTE] = 0;
	return effect;
}
