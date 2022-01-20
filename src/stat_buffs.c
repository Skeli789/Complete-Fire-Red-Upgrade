#include "defines.h"
#include "defines_battle.h"
#include "../include/battle_anim.h"
#include "../include/battle_string_ids.h"
#include "../include/string_util.h"
#include "../include/constants/items.h"

#include "../include/new/ability_battle_scripts.h"
#include "../include/new/accuracy_calc.h"
#include "../include/new/battle_strings.h"
#include "../include/new/battle_util.h"
#include "../include/new/stat_buffs.h"
/*
stat_buffs.c
	adjusts stat-related functions to include abilities and effects that change their opteraions (eg. contrary)
*/

extern u8 BattleScript_MistProtected[];
extern u8 DrasticallyString[];
extern u8 SeverelyString[];

static bool8 IsIntimidateActive(void)
{
	return gNewBS->intimidateActive != 0 && !gNewBS->cottonDownActive;
}

static bool8 IsCottonDownActive(void)
{
	return gNewBS->intimidateActive != 0 && gNewBS->cottonDownActive;
}

void atk13_printfromtable(void)
{
	if (gBattleExecBuffer) return;

	u16* ptr = (u16*)T1_READ_PTR(gBattlescriptCurrInstr + 1);
	ptr += gBattleCommunication[MULTISTRING_CHOOSER];
	u16 stringId = *(u16*)ptr;
	PrepareStringBattle(stringId, gBankAttacker);
	gBattlescriptCurrInstr += 5;
	gBattleCommunication[MSG_DISPLAY] = 1;

	if (stringId == STRINGID_PKMNSSTATCHANGED4)
	{
		u8 atkSide = SIDE(gBankAttacker);
		u8 defSide = SIDE(gBankTarget);

		if (IsIntimidateActive()
		|| (IsCottonDownActive() && defSide != atkSide)
		|| (gNewBS->stickyWebActive && SIDE(gSideTimers[defSide].stickyWebBank) != defSide) //Was set by foe and not Court Change
		|| defSide != atkSide)
			DefiantActivation(); //Stat fell from enemy
	}
}

bool8 DefiantActivation(void)
{
	switch (ABILITY(gBankTarget)) {
		case ABILITY_DEFIANT:
			gBattleScripting.statChanger = INCREASE_2 | STAT_STAGE_ATK;
			break;

		case ABILITY_COMPETITIVE:
			gBattleScripting.statChanger = INCREASE_2 | STAT_STAGE_SPATK;
			break;

		case ABILITY_RATTLED:
			if (IsIntimidateActive())
				gBattleScripting.statChanger = INCREASE_1 | STAT_STAGE_SPEED;
			else
				return FALSE;
			break;

		default:
			return FALSE;
	}

	BattleScriptPushCursor();
	gBattlescriptCurrInstr = BattleScript_DefiantCompetitive;
	gBattleScripting.bank = gBankTarget;
	return TRUE;
}

void atk20_jumpifstat(void)
{
	u8 ret = 0;
	u8 bank = GetBankForBattleScript(gBattlescriptCurrInstr[1]);
	u8 value = gBattleMons[bank].statStages[gBattlescriptCurrInstr[3] - 1];
	s8 comparison = gBattlescriptCurrInstr[4];

	if (ABILITY(bank) == ABILITY_CONTRARY)
	{
		comparison -= 12;
		comparison *= - 1;
	}

	switch (T2_READ_8(gBattlescriptCurrInstr + 2)) {
		case CMP_EQUAL:
			if (value == comparison)
				ret++;
			break;
		case CMP_NOT_EQUAL:
			if (value != comparison)
				ret++;
			break;
		case CMP_GREATER_THAN:
			if (value > comparison)
				ret++;
			break;
		case CMP_LESS_THAN:
			if (value < comparison)
				ret++;
			break;
		case CMP_COMMON_BITS:
			if (value & comparison)
				ret++;
			break;
		case CMP_NO_COMMON_BITS:
			if (!(value & comparison))
				ret++;
			break;
	}

	if (ret)
		gBattlescriptCurrInstr = T2_READ_PTR(gBattlescriptCurrInstr + 5);
	else
		gBattlescriptCurrInstr += 9;
}

void atk48_playstatchangeanimation(void)
{
	u32 currStat = 0;
	u16 statAnimId = 0;
	s32 changeableStatsCount = 0;
	u8 statsToCheck = 0;
	u8 flags = gBattlescriptCurrInstr[3];

	gActiveBattler = GetBankForBattleScript(T2_READ_8(gBattlescriptCurrInstr + 1));
	statsToCheck = T2_READ_8(gBattlescriptCurrInstr + 2);

	if (ABILITY(gActiveBattler) == ABILITY_SIMPLE)
		flags |= ATK48_STAT_BY_TWO;

	if (flags & ATK48_STAT_NEGATIVE) // goes down
	{
		if (ABILITY(gActiveBattler) == ABILITY_CONTRARY)
			goto STAT_ANIM_UP;

	STAT_ANIM_DOWN:	;
		s16 startingStatAnimId;
		u8 ability = ABILITY(gActiveBattler);

		if (flags & ATK48_STAT_BY_TWO)
			startingStatAnimId = STAT_ANIM_MINUS2 - 1;
		else
			startingStatAnimId = STAT_ANIM_MINUS1 - 1;

		while (statsToCheck != 0) {

			if (statsToCheck & 1)
			{
				if (flags & ATK48_DONT_CHECK_LOWER)
				{
					if (gBattleMons[gActiveBattler].statStages[currStat - 1] > 0)
					{
						statAnimId = startingStatAnimId + currStat;
						changeableStatsCount++;
					}
				}
				else if (!CanStatNotBeLowered(currStat, gActiveBattler, (gBattlescriptCurrInstr[1] == BS_GET_TARGET) ? gBankAttacker : gActiveBattler, ability)
					&& !(AbilityBlocksIntimidate(ability) && IsIntimidateActive())
					&& !(ability == ABILITY_MIRRORARMOR && gBankAttacker != gBankTarget && gActiveBattler == gBankTarget))
				{
					if (STAT_STAGE(gActiveBattler, currStat) > STAT_STAGE_MIN)
					{
						statAnimId = startingStatAnimId + currStat;
						changeableStatsCount++;
					}
				}
			}
			statsToCheck >>= 1, ++currStat;
		}

		if (changeableStatsCount > 1) // more than one stat, so the color is gray
		{
			if (flags & ATK48_STAT_BY_TWO)
				statAnimId = STAT_ANIM_MULTIPLE_MINUS2;
			else
				statAnimId = STAT_ANIM_MULTIPLE_MINUS1;
		}
	}

	else // goes up
	{
		if (ABILITY(gActiveBattler) == ABILITY_CONTRARY)
			goto STAT_ANIM_DOWN;

	STAT_ANIM_UP:	;
		s16 startingStatAnimId;
		if (flags & ATK48_STAT_BY_TWO)
			startingStatAnimId = STAT_ANIM_PLUS2 - 1;
		else
			startingStatAnimId = STAT_ANIM_PLUS1 - 1;

		while (statsToCheck != 0)
		{
			if (statsToCheck & 1 && gBattleMons[gActiveBattler].statStages[currStat - 1] < 12)
			{
				statAnimId = startingStatAnimId + currStat;
				changeableStatsCount++;
			}
			statsToCheck >>= 1, ++currStat;
		}

		if (changeableStatsCount > 1) // more than one stat, so the color is gray
		{
			if (flags & ATK48_STAT_BY_TWO)
				statAnimId = STAT_ANIM_MULTIPLE_PLUS2;
			else
				statAnimId = STAT_ANIM_MULTIPLE_PLUS1;
		}
	}

	if (gNewBS->SpectralThiefActive)
	{
		if (ABILITY(gActiveBattler) == ABILITY_CONTRARY)
		{
			statAnimId = STAT_ANIM_MULTIPLE_MINUS1; //Invert stat changes
			changeableStatsCount = 0xFF;
		}
		else
		{
			statAnimId = STAT_ANIM_MULTIPLE_PLUS1;
			changeableStatsCount = 0xFF;
		}
	}
	else if (gNewBS->totemOmniboostActive)
	{
		statAnimId = STAT_ANIM_MULTIPLE_PLUS1;
		changeableStatsCount = 0xFF;
	}

	if ((T2_READ_8(gBattlescriptCurrInstr + 3) & ATK48_ONLY_MULTIPLE && changeableStatsCount < 2)
	|| (T2_READ_8(gBattlescriptCurrInstr + 3) & ATK48_ONLY_TRIPLE && changeableStatsCount < 3))
	{
		gBattlescriptCurrInstr += 4;
	}
	else if (changeableStatsCount != 0 && !gBattleScripting.statAnimPlayed)
	{
		EmitBattleAnimation(0, B_ANIM_STATS_CHANGE, statAnimId);
		MarkBufferBankForExecution(gActiveBattler);
		if ((T2_READ_8(gBattlescriptCurrInstr + 3) & ATK48_ONLY_MULTIPLE && changeableStatsCount > 1)
		||  (T2_READ_8(gBattlescriptCurrInstr + 3) & ATK48_ONLY_TRIPLE && changeableStatsCount > 2))
			gBattleScripting.statAnimPlayed = TRUE;
		gBattlescriptCurrInstr += 4;
	}
	else
	{
		gBattlescriptCurrInstr += 4;
	}
}

void atk89_statbuffchange(void)
{
	u8* jump_loc = T1_READ_PTR(gBattlescriptCurrInstr + 2);
	if (ChangeStatBuffs(gBattleScripting.statChanger & 0xF0, gBattleScripting.statChanger & 0xF, T2_READ_8(gBattlescriptCurrInstr + 1), jump_loc) == 0)
		gBattlescriptCurrInstr += 6;
}

u8 ChangeStatBuffs(s8 statValue, u8 statId, u8 flags, const u8* BS_ptr)
{
	bool8 certain = FALSE;
	bool8 affectsUser = FALSE;
	bool8 notProtectAffected = FALSE;
	u32 index;

	if (flags & MOVE_EFFECT_AFFECTS_USER)
	{
		gActiveBattler = gBankAttacker;
		affectsUser = TRUE;
	}
	else
		gActiveBattler = gBankTarget;

	u8 ability = ABILITY(gActiveBattler);

	flags &= ~(MOVE_EFFECT_AFFECTS_USER);

	if (flags & MOVE_EFFECT_CERTAIN)
		certain++;
	flags &= ~(MOVE_EFFECT_CERTAIN);

	if (flags & STAT_CHANGE_NOT_PROTECT_AFFECTED)
		notProtectAffected++;
	flags &= ~(STAT_CHANGE_NOT_PROTECT_AFFECTED);

	PREPARE_STAT_BUFFER(gBattleTextBuff1, statId)

	if ((statValue << 0x18) < 0) // stat decrease
	{
		if (ABILITY(gActiveBattler) == ABILITY_CONTRARY && !gNewBS->zMoveData.runningZEffect)
			goto RAISE_STAT_BUFF;

	LOWER_STAT_BUFF:
		if (BankSideHasMist(gActiveBattler)
		&& !certain
		&& gCurrentMove != MOVE_CURSE
		&& !BypassesScreens(ABILITY(gBankAttacker)))
		{
			if (flags == STAT_CHANGE_BS_PTR)
			{
				if (gSpecialStatuses[gActiveBattler].statLowered)
				{
					gBattlescriptCurrInstr = BS_ptr;
				}
				else
				{
					BattleScriptPush(BS_ptr);
					gBattleScripting.bank = gActiveBattler;
					gBattlescriptCurrInstr = BattleScript_MistProtected;
					gSpecialStatuses[gActiveBattler].statLowered = 1;
				}
			}
			return STAT_CHANGE_DIDNT_WORK;
		}

		else if (gCurrentMove != MOVE_CURSE
			  && notProtectAffected != TRUE
			  && JumpIfMoveAffectedByProtect(gCurrentMove, gBankAttacker, gBankTarget, TRUE))
		{
			gBattlescriptCurrInstr = BattleScript_ButItFailed;
			return STAT_CHANGE_DIDNT_WORK;
		}

		else if ((IsClearBodyAbility(ability)
			  || (ability == ABILITY_FLOWERVEIL && IsOfType(gActiveBattler, TYPE_GRASS)))
		&& !certain && gCurrentMove != MOVE_CURSE)
		{
			if (flags == STAT_CHANGE_BS_PTR)
			{
				if (gSpecialStatuses[gActiveBattler].statLowered)
				{
					gBattlescriptCurrInstr = BS_ptr;
				}
				else
				{
					BattleScriptPush(BS_ptr);
					gBattleScripting.bank = gActiveBattler;
					gBattleCommunication[0] = gActiveBattler;
					gBattlescriptCurrInstr = BattleScript_AbilityNoStatLoss;
					gLastUsedAbility = gBattleMons[gActiveBattler].ability;
					RecordAbilityBattle(gActiveBattler, gLastUsedAbility);
					gSpecialStatuses[gActiveBattler].statLowered = 1;
				}
			}
			return STAT_CHANGE_DIDNT_WORK;
		}

		else if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE
		&& IsOfType(gActiveBattler, TYPE_GRASS)
		&& ABILITY(PARTNER(gActiveBattler)) == ABILITY_FLOWERVEIL
		&& !certain && gCurrentMove != MOVE_CURSE)
		{
			if (flags == STAT_CHANGE_BS_PTR)
			{
				if (gSpecialStatuses[gActiveBattler].statLowered)
				{
					gBattlescriptCurrInstr = BS_ptr;
				}
				else
				{
					BattleScriptPush(BS_ptr);
					gBattleScripting.bank = PARTNER(gActiveBattler);
					gBattleCommunication[0] = gActiveBattler;
					gBattlescriptCurrInstr = BattleScript_PartnerAbilityNoStatLoss;
					gSpecialStatuses[gActiveBattler].statLowered = 1;
				}
			}
			return STAT_CHANGE_DIDNT_WORK;
		}

		else if (!certain
		&& (AbilityPreventsLoweringStat(ability, statId) || (IsIntimidateActive() && AbilityBlocksIntimidate(ability))))
		{
			if (flags == STAT_CHANGE_BS_PTR)
			{
				BattleScriptPush(BS_ptr);
				gBattleScripting.bank = gActiveBattler;
				gBattlescriptCurrInstr = BattleScript_AbilityNoSpecificStatLoss;
			}
			return STAT_CHANGE_DIDNT_WORK;
		}
		else if (ability == ABILITY_MIRRORARMOR && (IsIntimidateActive() || IsCottonDownActive()) && !certain)
		{
			if (flags == STAT_CHANGE_BS_PTR)
			{
				BattleScriptPush(BS_ptr);
				gBattleScripting.bank = gActiveBattler;
				gBattlescriptCurrInstr = BattleScript_MirrorArmorReflectsIntimidate;
			}
			return STAT_CHANGE_DIDNT_WORK;
		}
		else if (ability == ABILITY_MIRRORARMOR && !affectsUser && gBankAttacker != gBankTarget && gActiveBattler == gBankTarget)
		{
			if (flags == STAT_CHANGE_BS_PTR)
			{
				BattleScriptPush(BS_ptr);
				gBattleScripting.bank = gActiveBattler;
				gBattlescriptCurrInstr = BattleScript_MirrorArmorReflectsStatLoss;
			}
			return STAT_CHANGE_DIDNT_WORK;
		}
		else if ((ability == ABILITY_SHIELDDUST || SheerForceCheck()) && !(gHitMarker & HITMARKER_IGNORE_SUBSTITUTE) && flags == 0)
		{
			return STAT_CHANGE_DIDNT_WORK;
		}
		else // try to decrease
		{
			statValue = -GET_STAT_BUFF_VALUE(statValue);

			if (ability == ABILITY_SIMPLE && !gNewBS->zMoveData.runningZEffect)
				statValue *= 2;

			gBattleTextBuff2[0] = B_BUFF_PLACEHOLDER_BEGIN;
			index = 1;
			if (statValue == -2)
			{
				gBattleTextBuff2[1] = B_BUFF_STRING;
				gBattleTextBuff2[2] = STRINGID_STATHARSHLY;
				gBattleTextBuff2[3] = STRINGID_STATHARSHLY >> 8;
				index = 4;
			}
			else if (statValue <= -3)
			{
				gBattleTextBuff2[1] = B_BUFF_STRING;
				gBattleTextBuff2[2] = 0x85;
				gBattleTextBuff2[3] = 0x1;
				index = 4;
			}
			if (!gNewBS->zMoveData.runningZEffect) {
				gBattleTextBuff2[index] = B_BUFF_STRING;
				index++;
				gBattleTextBuff2[index] = STRINGID_STATFELL;
				index++;
				gBattleTextBuff2[index] = STRINGID_STATFELL >> 8;
				index++;
			}
			gBattleTextBuff2[index] = B_BUFF_EOS;

			if (STAT_STAGE(gActiveBattler, statId) == 0)
				gBattleCommunication[MULTISTRING_CHOOSER] = 2;
			else
				gBattleCommunication[MULTISTRING_CHOOSER] = (gBankTarget == gActiveBattler);

			gNewBS->statFellThisTurn[gActiveBattler] = TRUE;
			gNewBS->statFellThisRound[gActiveBattler] = TRUE;
		}
	}

	//Stat Increase
	else
	{
		if (ABILITY(gActiveBattler) == ABILITY_CONTRARY && !gNewBS->zMoveData.runningZEffect)
			goto LOWER_STAT_BUFF;

	RAISE_STAT_BUFF:
		statValue = GET_STAT_BUFF_VALUE(statValue);

		if (ability == ABILITY_SIMPLE && !gNewBS->zMoveData.runningZEffect)
			statValue *= 2;

		gBattleTextBuff2[0] = B_BUFF_PLACEHOLDER_BEGIN;
		index = 1;
		if (statValue == 2)
		{
			gBattleTextBuff2[1] = B_BUFF_STRING;
			gBattleTextBuff2[2] = STRINGID_STATSHARPLY;
			gBattleTextBuff2[3] = STRINGID_STATSHARPLY >> 8;
			index = 4;
		}
		if (statValue >= 3)
		{
			gBattleTextBuff2[1] = B_BUFF_STRING;
			gBattleTextBuff2[2] = 0x86;
			gBattleTextBuff2[3] = 0x1;
			index = 4;
		}
		if (!gNewBS->zMoveData.runningZEffect) {
			gBattleTextBuff2[index] = B_BUFF_STRING;
			index++;
			gBattleTextBuff2[index] = STRINGID_STATROSE;
			index++;
			gBattleTextBuff2[index] = STRINGID_STATROSE >> 8;
			index++;
		}
		gBattleTextBuff2[index] = B_BUFF_EOS;

		if (gBattleMons[gActiveBattler].statStages[statId - 1] == STAT_STAGE_MAX)
			gBattleCommunication[MULTISTRING_CHOOSER] = 2;
		else
			gBattleCommunication[MULTISTRING_CHOOSER] = (gBankTarget == gActiveBattler);

		gNewBS->statRoseThisRound[gActiveBattler] = TRUE;
	}

	gBattleMons[gActiveBattler].statStages[statId - 1] += statValue;
	if (gBattleMons[gActiveBattler].statStages[statId - 1] < 0)
		gBattleMons[gActiveBattler].statStages[statId - 1] = 0;
	if (gBattleMons[gActiveBattler].statStages[statId - 1] > STAT_STAGE_MAX)
		gBattleMons[gActiveBattler].statStages[statId - 1] = STAT_STAGE_MAX;

	if (gBattleCommunication[MULTISTRING_CHOOSER] == 2 && flags & STAT_CHANGE_BS_PTR)
		gMoveResultFlags |= MOVE_RESULT_MISSED;

	if (gBattleCommunication[MULTISTRING_CHOOSER] == 2 && !(flags & STAT_CHANGE_BS_PTR))
		return STAT_CHANGE_DIDNT_WORK;

	return STAT_CHANGE_WORKED;
}

u8 CanStatNotBeLowered(u8 statId, u8 bankDef, u8 bankAtk, u8 defAbility)
{
	if (!BATTLER_ALIVE(bankDef))
		return STAT_FAINTED; 

	if (defAbility == ABILITY_CONTRARY)
	{
		if (STAT_STAGE(bankDef, statId) >= STAT_STAGE_MAX)
			return STAT_AT_MAX;

		return STAT_CAN_BE_LOWERED;		
	}

	if (STAT_STAGE(bankDef, statId) == STAT_STAGE_MIN)
		return STAT_AT_MIN;
	else if (BankSideHasMist(bankDef) && bankDef != bankAtk && !BypassesScreens(ABILITY(bankAtk)))
		return STAT_PROTECTED_BY_MIST;

	if (IsClearBodyAbility(defAbility)
	|| (defAbility == ABILITY_FLOWERVEIL && IsOfType(bankDef, TYPE_GRASS)))
		return STAT_PROTECTED_BY_GENERAL_ABILITY;
	else if (ABILITY(PARTNER(bankDef)) == ABILITY_FLOWERVEIL && IsOfType(bankDef, TYPE_GRASS))
		return STAT_PROTECTED_BY_PARTNER_ABILITY;
	else if (AbilityPreventsLoweringStat(defAbility, statId))
		return STAT_PROTECTED_BY_SPECIFIC_ABILITY;

	return STAT_CAN_BE_LOWERED;
}

bool8 CanStatBeLowered(u8 statId, u8 bankDef, u8 bankAtk, u8 defAbility)
{
	return CanStatNotBeLowered(statId, bankDef, bankAtk, defAbility) == STAT_CAN_BE_LOWERED;
}

bool8 NewXSpecialBoost(u16 item, u8 boostAmount)
{
	u8 stat;
	bool8 retVal = TRUE; //Didn't work by default

	if (item == ITEM_X_SP_DEF) //Not in Gen 3
		stat = STAT_STAGE_SPDEF;
	else
		stat = STAT_STAGE_SPATK;

	if (STAT_STAGE(gActiveBattler, stat) < STAT_STAGE_MAX)
	{
		STAT_STAGE(gActiveBattler, stat) += boostAmount;
		if (STAT_STAGE(gActiveBattler, stat) > STAT_STAGE_MAX)
			STAT_STAGE(gActiveBattler, stat) = STAT_STAGE_MAX;
		retVal = FALSE;
		
		if (stat == STAT_STAGE_SPDEF)
			gNewBS->usedXSpDef = TRUE;
		else
			gNewBS->usedXSpDef = FALSE;
	}

	return retVal;
}

extern const u8 gXItemEffectToStatId[];
void PrepareStringForUsingXItem(u32 stat)
{
	u8 index = 4;

	stat = gXItemEffectToStatId[stat];
	if (stat == STAT_SPATK && gNewBS->usedXSpDef)
		stat = STAT_SPDEF;

    gBankTarget = gBattlerInMenuId;
    StringCopy(gBattleTextBuff1, gStatNamesTable[stat]);
   
	gBattleTextBuff2[0] = B_BUFF_PLACEHOLDER_BEGIN;
	gBattleTextBuff2[1] = B_BUFF_STRING;
	gBattleTextBuff2[2] = STRINGID_STATSHARPLY;
	gBattleTextBuff2[3] = STRINGID_STATSHARPLY >> 8;
	gBattleTextBuff2[index++] = B_BUFF_STRING;
	gBattleTextBuff2[index++] = STRINGID_STATROSE;
	gBattleTextBuff2[index++] = STRINGID_STATROSE >> 8;
	gBattleTextBuff2[index] = B_BUFF_EOS;

    BattleStringExpandPlaceholdersToDisplayedString((void*) 0x83FCB6A); //gText_PkmnsStatChanged2
}

void PrepareStringForAIUsingXItem(void)
{
	u8 index = 4;

	PREPARE_STAT_BUFFER(gBattleTextBuff1, STAT_ATK);
	gBattleTextBuff2[0] = B_BUFF_PLACEHOLDER_BEGIN;
	gBattleTextBuff2[1] = B_BUFF_STRING;
	gBattleTextBuff2[2] = STRINGID_STATSHARPLY;
	gBattleTextBuff2[3] = STRINGID_STATSHARPLY >> 8;
	gBattleTextBuff2[index++] = B_BUFF_STRING;
	gBattleTextBuff2[index++] = STRINGID_STATROSE;
	gBattleTextBuff2[index++] = STRINGID_STATROSE >> 8;
	gBattleTextBuff2[index] = B_BUFF_EOS;

	while (!(gBattleStruct->AI_itemFlags[gBankAttacker >> 1] & 1))
	{
		gBattleStruct->AI_itemFlags[gBankAttacker >> 1] >>= 1;
		++gBattleTextBuff1[2];
	}

	gBattleScripting.animArg1 = gBattleTextBuff1[2] + STAT_ANIM_PLUS2 - 1;
	gBattleScripting.animArg2 = 0;
}
