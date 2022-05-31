#include "defines.h"
#include "defines_battle.h"
#include "../include/battle_string_ids.h"
#include "../include/constants/items.h"
#include "../include/random.h"

#include "../include/new/ability_battle_scripts.h"
#include "../include/new/battle_util.h"
#include "../include/new/battle_script_util.h"
#include "../include/new/dynamax.h"
#include "../include/new/item.h"
#include "../include/new/move_battle_scripts.h"
#include "../include/new/set_effect.h"
#include "../include/new/stat_buffs.h"
#include "../include/new/util.h"

/*
set_effect.c
	handles move effects

tables:
	sMoveEffectsThatIgnoreSubstitute
	sShieldDustIgnoredEffects
	sStatusFlagsForMoveEffects
	gTrappingMoves
	gWrappedStringIds

*/

#define INCREMENT_RESET_RETURN				  \
{											   \
	gBattlescriptCurrInstr++;				   \
	gBattleCommunication[MOVE_EFFECT_BYTE] = 0; \
	return;									 \
}

#define RESET_RETURN							\
{											   \
	gBattleCommunication[MOVE_EFFECT_BYTE] = 0; \
	return effect;							  \
}

#define sMoveEffectBS_Ptrs ((u8**) 0x825062C)

static const u8 sMoveEffectsThatIgnoreSubstitute[] =
{
	MOVE_EFFECT_PAYDAY,
	MOVE_EFFECT_ION_DELUGE,
	MOVE_EFFECT_BREAK_SCREENS,
	MOVE_EFFECT_REMOVE_TERRAIN,
	MOVE_EFFECT_EAT_BERRY,
	0xFF,
};

static const u8 sShieldDustIgnoredEffects[] =
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

static const u32 sStatusFlagsForMoveEffects[] =
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
	MOVE_BIND, MOVE_WRAP, MOVE_FIRESPIN, MOVE_CLAMP, MOVE_WHIRLPOOL, MOVE_SANDTOMB, MOVE_MAGMASTORM, MOVE_INFESTATION, MOVE_SNAPTRAP, MOVE_OCTOLOCK, MOVE_THUNDERCAGE, 0xFFFF
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

void atk15_seteffectwithchance(void)
{
	u32 percentChance = gBattleMoves[gCurrentMove].secondaryEffectChance;

	if (ABILITY(gBankAttacker) == ABILITY_SERENEGRACE || BankHasRainbow(gBankAttacker))
	{
		percentChance *= 2;

		if ((ABILITY(gBankAttacker) == ABILITY_SERENEGRACE && BankHasRainbow(gBankAttacker)) //Has both
		&& (gBattleCommunication[MOVE_EFFECT_BYTE] & 0x3F) != MOVE_EFFECT_FLINCH) //Doesn't stack for flinches
			percentChance *= 2;
	}

	#ifdef FROSTBITE
	if (gBattleWeather & WEATHER_HAIL_ANY && gBattleCommunication[MOVE_EFFECT_BYTE] == MOVE_EFFECT_FREEZE && WEATHER_HAS_EFFECT)
		percentChance *= 2;
	#endif

	if (!SheerForceCheck() || (gBattleCommunication[MOVE_EFFECT_BYTE] & 0x3F) == MOVE_EFFECT_RAPIDSPIN)
	{
		if ((gBattleCommunication[MOVE_EFFECT_BYTE] & MOVE_EFFECT_CERTAIN) && MOVE_HAD_EFFECT)
		{
			gBattleCommunication[MOVE_EFFECT_BYTE] &= 0x7F;
			SetMoveEffect(FALSE, MOVE_EFFECT_CERTAIN);
		}
		else if (Random() % 100 <= percentChance && gBattleCommunication[MOVE_EFFECT_BYTE] != 0 && MOVE_HAD_EFFECT)
		{
			SetMoveEffect(FALSE, 0);
		}
		else
			gBattlescriptCurrInstr++;
	}
	else
		gBattlescriptCurrInstr++;

	gBattleCommunication[MOVE_EFFECT_BYTE] = 0;
	gBattleScripting.multihitMoveEffect = 0;
}

void SetMoveEffect(bool8 primary, u8 certain)
{
	bool8 statusChanged = FALSE;
	u8 affectsUser = 0; // 0x40 otherwise
	u8 flags = 0;
	bool8 mirrorArmorReflected = ABILITY(gBankTarget) == ABILITY_MIRRORARMOR;

	if (gBattleCommunication[MOVE_EFFECT_BYTE] & MOVE_EFFECT_AFFECTS_USER)
	{
		gEffectBank = gBankAttacker; // battlerId that effects get applied on
		gBattleCommunication[MOVE_EFFECT_BYTE] &= ~(MOVE_EFFECT_AFFECTS_USER);
		affectsUser = MOVE_EFFECT_AFFECTS_USER;
		gBattleScripting.bank = gBankAttacker;
	}
	else
	{
		gEffectBank = gBankTarget;
		gBattleScripting.bank = gBankAttacker;
	}

	if (ABILITY(gEffectBank) == ABILITY_SHIELDDUST
	&& !(gHitMarker & HITMARKER_IGNORE_SAFEGUARD) //Used by contact abilities and synchronize
	&& !primary
	&& CheckTableForSpecialMoveEffect(gBattleCommunication[MOVE_EFFECT_BYTE], sShieldDustIgnoredEffects))
	{
		++gBattlescriptCurrInstr;
		goto CLEAR_MOVE_EFFECT_BYTE;
	}

	if (BankSideHasSafeguard(gEffectBank)
	&& !(gHitMarker & HITMARKER_IGNORE_SAFEGUARD)
	&& !BypassesScreens(ABILITY(gBankAttacker))
	&& gEffectBank != gBankAttacker
	&& !primary
	&& certain != MOVE_EFFECT_CERTAIN
	&& gBattleCommunication[MOVE_EFFECT_BYTE] <= MOVE_EFFECT_CONFUSION)
	{
		++gBattlescriptCurrInstr;
		goto CLEAR_MOVE_EFFECT_BYTE;
	}

	if (IsOfType(gEffectBank, TYPE_GRASS)
	&& (ABILITY(gEffectBank) == ABILITY_FLOWERVEIL || (IS_DOUBLE_BATTLE && ABILITY(PARTNER(gEffectBank)) == ABILITY_FLOWERVEIL))
	&& gEffectBank != gBankAttacker
	&& (gBattleCommunication[MOVE_EFFECT_BYTE] <= MOVE_EFFECT_TOXIC //Note: Confusion not stopped by Flower Veil
	 || (gBattleCommunication[MOVE_EFFECT_BYTE] >= MOVE_EFFECT_ATK_MINUS_1 && gBattleCommunication[MOVE_EFFECT_BYTE] <= MOVE_EFFECT_EVS_MINUS_1)
	 || (gBattleCommunication[MOVE_EFFECT_BYTE] >= MOVE_EFFECT_ATK_MINUS_2 && gBattleCommunication[MOVE_EFFECT_BYTE] <= MOVE_EFFECT_EVS_MINUS_2)))
	{
		++gBattlescriptCurrInstr;
		goto CLEAR_MOVE_EFFECT_BYTE;
	}

	if (CheckTableForSpecialMoveEffect(gBattleCommunication[MOVE_EFFECT_BYTE], sMoveEffectsThatIgnoreSubstitute))
		goto SKIP_SUBSTITUTE_CHECK;

	if (!BATTLER_ALIVE(gEffectBank))
	{
		++gBattlescriptCurrInstr;
		goto CLEAR_MOVE_EFFECT_BYTE;
	}

	if (MoveBlockedBySubstitute(gCurrentMove, gBattleScripting.bank, gEffectBank)
	&& affectsUser != MOVE_EFFECT_AFFECTS_USER
	&& !(gHitMarker & HITMARKER_IGNORE_SUBSTITUTE))
	{
		++gBattlescriptCurrInstr;
		goto CLEAR_MOVE_EFFECT_BYTE;
	}

	if (HasRaidShields(gEffectBank)
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
				if (CanBePutToSleep(gEffectBank, gBankAttacker, FALSE)) //Flower Veil & Safeguard checked earlier
				{
					CancelMultiTurnMoves(gEffectBank);
					statusChanged = TRUE;
				}
			break;

			case STATUS1_POISON:
			case STATUS1_TOXIC_POISON:
				if (CanBePoisoned(gEffectBank, gBankAttacker, FALSE)) //Flower Veil & Safeguard checked earlier
					statusChanged = TRUE;
				break;

			case STATUS1_BURN:
				if (CanBeBurned(gEffectBank, gBankAttacker, FALSE)) //Flower Veil & Safeguard checked earlier
					statusChanged = TRUE;
				break;

			case STATUS1_FREEZE:
				if (CanBeFrozen(gEffectBank, gBankAttacker, FALSE)) //Flower Veil & Safeguard checked earlier
				{
					CancelMultiTurnMoves(gEffectBank);
					statusChanged = TRUE;
				}
				break;

			case STATUS1_PARALYSIS:
				if (CanBeParalyzed(gEffectBank, gBankAttacker, FALSE)) //Flower Veil & Safeguard checked earlier
					statusChanged = TRUE;
				break;
		}

		if (statusChanged)
		{
			BattleScriptPush(gBattlescriptCurrInstr + 1);

			if (gBattleCommunication[MOVE_EFFECT_BYTE] == MOVE_EFFECT_TOXIC && IsDynamaxed(gEffectBank))
				gBattleCommunication[MOVE_EFFECT_BYTE] = MOVE_EFFECT_POISON; //Toxic becomes regular poison on a Dynamaxed opponent

			if (gBattleCommunication[MOVE_EFFECT_BYTE] == MOVE_EFFECT_SLEEP)
				gBattleMons[gEffectBank].status1 |= ((Random() % 3) + 2);
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

			if (gBattleCommunication[MOVE_EFFECT_BYTE] == MOVE_EFFECT_POISON
			|| gBattleCommunication[MOVE_EFFECT_BYTE] == MOVE_EFFECT_TOXIC
			|| gBattleCommunication[MOVE_EFFECT_BYTE] == MOVE_EFFECT_PARALYSIS
			|| gBattleCommunication[MOVE_EFFECT_BYTE] == MOVE_EFFECT_BURN
			|| gBattleCommunication[MOVE_EFFECT_BYTE] == MOVE_EFFECT_TOXIC)
			{
				if (gPoisonedBy != 0x2 && gPoisonedBy != 0x3) //Not Toxic Spikes or Toxic Orb / Flame Orb
				{
					gBattleStruct->synchronizeMoveEffect = gBattleCommunication[MOVE_EFFECT_BYTE];
					gNewBS->synchronizeTarget[gEffectBank] = gBankAttacker + 1;
				}
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
		if (gBattleCommunication[MOVE_EFFECT_BYTE] < NELEMS(sStatusFlagsForMoveEffects) //Error prevention
		&& gBattleMons[gEffectBank].status2 & sStatusFlagsForMoveEffects[gBattleCommunication[MOVE_EFFECT_BYTE]])
		{
			gBattlescriptCurrInstr++;
		}
		else
		{
			switch (gBattleCommunication[MOVE_EFFECT_BYTE])
			{
			case MOVE_EFFECT_CONFUSION:
				if (CanBeConfused(gEffectBank, gBankAttacker, FALSE)) //Safeguard checked earlier
				{
					gBattleMons[gEffectBank].status2 |= (umodsi(Random(), 4)) + 2;

					BattleScriptPush(gBattlescriptCurrInstr + 1);
					gBattlescriptCurrInstr = sMoveEffectBS_Ptrs[gBattleCommunication[MOVE_EFFECT_BYTE]];
				}
				else
					gBattlescriptCurrInstr++;
				break;

			case MOVE_EFFECT_FLINCH:
				if (!CanFlinch(gEffectBank, ABILITY(gEffectBank)))
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
						if (IsAnyMaxMove(gCurrentMove) && gBattleMoves[gCurrentMove].z_move_effect == MAX_EFFECT_CONFUSE_FOES_PAY_DAY)
							gNewBS->maxGoldrushMoney += (100 * ++gNewBS->maxGoldrushUses * gBattleMons[gBankAttacker].level);
						else
							gNewBS->PayDayByPartyIndices[gBattlerPartyIndexes[gBankAttacker]]++;
					}
					BattleScriptPush(gBattlescriptCurrInstr + 1);
					gBattlescriptCurrInstr = sMoveEffectBS_Ptrs[gBattleCommunication[MOVE_EFFECT_BYTE]];
				}
				break;

			case MOVE_EFFECT_TRI_ATTACK:
				if (gBattleMons[gEffectBank].status1 != 0)
				{
					gBattlescriptCurrInstr++;
				}
				else
				{
					gBattleCommunication[MOVE_EFFECT_BYTE] = (Random() % 3);

					if (gCurrentMove == MOVE_DIRECLAW)
					{
						gBattleCommunication[MOVE_EFFECT_BYTE] += MOVE_EFFECT_SLEEP; //Sleep, Poison, Paralysis
						
						if (gBattleCommunication[MOVE_EFFECT_BYTE] == MOVE_EFFECT_BURN)
							gBattleCommunication[MOVE_EFFECT_BYTE] = MOVE_EFFECT_PARALYSIS; //Paralyzes, doesn't burn
					}
					else
						gBattleCommunication[MOVE_EFFECT_BYTE] += MOVE_EFFECT_BURN; //Burn, Freeze, Paralysis

					SetMoveEffect(FALSE, 0);
				}
				break;

			case MOVE_EFFECT_CHARGING:
				gBattleMons[gEffectBank].status2 |= STATUS2_MULTIPLETURNS;
				gLockedMoves[gEffectBank] = gCurrentMove;
				gProtectStructs[gEffectBank].chargingTurn = 1;
				gNewBS->metronomeItemBonus[gEffectBank] = 2;
				gBattlescriptCurrInstr++;
				break;

			case MOVE_EFFECT_WRAP:
				if (gBattleMons[gEffectBank].status2 & STATUS2_WRAPPED || gNewBS->secondaryEffectApplied)
				{
					gBattlescriptCurrInstr++;
				}
				else
				{
					u16 trappingMove = gCurrentMove;
					gNewBS->secondaryEffectApplied = TRUE;

					if (ITEM_EFFECT(gBankAttacker) == ITEM_EFFECT_GRIP_CLAW)
						gBattleMons[gEffectBank].status2 |= (7 << 0xD);
					else
						gBattleMons[gEffectBank].status2 |= ((Random() & 1) + 4) << 0xD;

					//Turn G-Max Moves into their corresponding trapping moves
					switch (trappingMove) {
						case MOVE_G_MAX_CENTIFERNO_P:
						case MOVE_G_MAX_CENTIFERNO_S:
							trappingMove = MOVE_FIRESPIN;
							break;
						case MOVE_G_MAX_SANDBLAST_P:
						case MOVE_G_MAX_SANDBLAST_S:
							trappingMove = MOVE_SANDTOMB;
							break;
					}

					gBattleStruct->wrappedMove[gEffectBank] = trappingMove;
					gBattleStruct->wrappedBy[gEffectBank] = gBankAttacker;

					BattleScriptPush(gBattlescriptCurrInstr + 1);
					gBattlescriptCurrInstr = sMoveEffectBS_Ptrs[gBattleCommunication[MOVE_EFFECT_BYTE]];

					for (gBattleCommunication[MULTISTRING_CHOOSER] = 0; ; gBattleCommunication[MULTISTRING_CHOOSER]++)
					{
						if (gBattleCommunication[MULTISTRING_CHOOSER] > 6)
							break;
						if (gTrappingMoves[gBattleCommunication[MULTISTRING_CHOOSER]] == trappingMove)
							break;
					}

					switch (gCurrentMove) {
						case MOVE_INFESTATION:
							gBattleStringLoader = gText_TargetWasInfested;
							break;
						case MOVE_SNAPTRAP:
							gBattleStringLoader = gText_TargetWasCaughtInSnapTrap;
							break;
						case MOVE_THUNDERCAGE:
							gBattleStringLoader = gText_TargetWasTrappedByAttacker;
							break;
						case MOVE_G_MAX_CENTIFERNO_P:
						case MOVE_G_MAX_CENTIFERNO_S:
						case MOVE_G_MAX_SANDBLAST_P:
						case MOVE_G_MAX_SANDBLAST_S:
							gNewBS->sandblastCentiferno[gEffectBank] = 1; //This condition doesn't get removed when the attacker leaves the field
							if (ITEM_EFFECT(gBankAttacker) == ITEM_EFFECT_GRIP_CLAW)
								gNewBS->sandblastCentiferno[gEffectBank] |= 2;
							break;
					}
				}
				break;

			case MOVE_EFFECT_ATK_PLUS_1:
			case MOVE_EFFECT_DEF_PLUS_1:
			case MOVE_EFFECT_SPD_PLUS_1:
			case MOVE_EFFECT_SP_ATK_PLUS_1:
			case MOVE_EFFECT_SP_DEF_PLUS_1:
			case MOVE_EFFECT_ACC_PLUS_1:
			case MOVE_EFFECT_EVS_PLUS_1:
				if (gNewBS->statBuffEffectNotProtectAffected)
				{
					flags |= STAT_CHANGE_NOT_PROTECT_AFFECTED;
					gNewBS->statBuffEffectNotProtectAffected = FALSE;
				}

				if (ChangeStatBuffs(SET_STAT_BUFF_VALUE(1),
									gBattleCommunication[MOVE_EFFECT_BYTE] - MOVE_EFFECT_ATK_PLUS_1 + 1,
									affectsUser | certain, 0))
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
				flags = affectsUser | certain;
				if (mirrorArmorReflected && !affectsUser)
				{
					gBattleScripting.statChanger = DECREASE_1 | (gBattleCommunication[MOVE_EFFECT_BYTE] - MOVE_EFFECT_ATK_MINUS_1 + 1);
					flags |= STAT_CHANGE_BS_PTR;
				}
				
				if (gNewBS->statBuffEffectNotProtectAffected)
				{
					flags |= STAT_CHANGE_NOT_PROTECT_AFFECTED;
					gNewBS->statBuffEffectNotProtectAffected = FALSE;
				}

				if (ChangeStatBuffs(SET_STAT_BUFF_VALUE(1) | STAT_BUFF_NEGATIVE,
									gBattleCommunication[MOVE_EFFECT_BYTE] - MOVE_EFFECT_ATK_MINUS_1 + 1,
									flags, gBattlescriptCurrInstr + 1))
				{
					if (!mirrorArmorReflected)
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
				if (gNewBS->statBuffEffectNotProtectAffected)
				{
					flags |= STAT_CHANGE_NOT_PROTECT_AFFECTED;
					gNewBS->statBuffEffectNotProtectAffected = FALSE;
				}

				if (ChangeStatBuffs(SET_STAT_BUFF_VALUE(2),
									gBattleCommunication[MOVE_EFFECT_BYTE] - MOVE_EFFECT_ATK_PLUS_2 + 1,
									affectsUser | certain, 0))
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
				flags = affectsUser | certain;
				if (mirrorArmorReflected && !affectsUser)
				{
					gBattleScripting.statChanger = DECREASE_2 | (gBattleCommunication[MOVE_EFFECT_BYTE] - MOVE_EFFECT_ATK_MINUS_2 + 1);
					flags |= STAT_CHANGE_BS_PTR;
				}

				if (gNewBS->statBuffEffectNotProtectAffected)
				{
					flags |= STAT_CHANGE_NOT_PROTECT_AFFECTED;
					gNewBS->statBuffEffectNotProtectAffected = FALSE;
				}

				if (ChangeStatBuffs(SET_STAT_BUFF_VALUE(2) | STAT_BUFF_NEGATIVE,
									gBattleCommunication[MOVE_EFFECT_BYTE] - MOVE_EFFECT_ATK_MINUS_2 + 1,
									flags, gBattlescriptCurrInstr + 1))
				{
					if (!mirrorArmorReflected)
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
				if (!IsOfType(gEffectBank, TYPE_GHOST) && !(gBattleMons[gEffectBank].status2 & STATUS2_ESCAPE_PREVENTION))
				{
					gBattleMons[gEffectBank].status2 |= STATUS2_ESCAPE_PREVENTION;
					gDisableStructs[gEffectBank].bankPreventingEscape = gBankAttacker;

					if (gCurrentMove == MOVE_OCTOLOCK)
						gNewBS->trappedByOctolock |= gBitTable[gEffectBank];
					else if (gCurrentMove == MOVE_NORETREAT)
						gNewBS->trappedByNoRetreat |= gBitTable[gEffectBank];
					else
						gNewBS->trappedByOctolock &= ~(gBitTable[gEffectBank]);
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
					if (ViableMonCountFromBank(FOE(gEffectBank)) == 0) //End of battle so don't bother
						gBattlescriptCurrInstr++;
					else
					{
						gNewBS->secondaryEffectApplied = TRUE;
						BattleScriptPush(gBattlescriptCurrInstr + 1);
						gBattlescriptCurrInstr = BattleScript_AllStatsUp;
					}
				}
				else
					gBattlescriptCurrInstr++;
				break;

			case MOVE_EFFECT_HIGHER_OFFENSES_DEFENSES_UP_1:
				if (ViableMonCountFromBank(FOE(gEffectBank)) == 0) //End of battle so don't bother
					gBattlescriptCurrInstr++;
				else
				{
					BattleScriptPush(gBattlescriptCurrInstr + 1);
					gBattlescriptCurrInstr = BattleScript_HigherOffensesDefensesUp;
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
				if ((IsBerry(ITEM(gEffectBank)) || ITEM_EFFECT(gEffectBank) == ITEM_EFFECT_GEM)
				&&  ABILITY(gEffectBank) != ABILITY_STICKYHOLD)
				{
					gNewBS->IncinerateCounters[gEffectBank] = TRUE;

					gLastUsedItem = gBattleMons[gEffectBank].item;
					gBattleMons[gEffectBank].item = 0;
					HandleUnburdenBoost(gEffectBank); //Give target Unburden boost

					gActiveBattler = gEffectBank;
					EmitSetMonData(0, REQUEST_HELDITEM_BATTLE, 0, 2, &gBattleMons[gActiveBattler].item);
					MarkBufferBankForExecution(gActiveBattler);

					BattleScriptPush(gBattlescriptCurrInstr + 1);
					gBattleStringLoader = RoastedBerryString;
					gBattlescriptCurrInstr = BattleScript_PrintCustomString;
				}
				else
					gBattlescriptCurrInstr++;

				break;

			extern const u16 gBannedBattleEatBerries[];
			case MOVE_EFFECT_EAT_BERRY: //For Stuff Cheeks
				if (IsBerry(ITEM(gEffectBank))
				&& !CheckTableForItem(ITEM(gEffectBank), gBannedBattleEatBerries)
				&& BATTLER_ALIVE(gEffectBank)
				&& gEffectBank == gBankAttacker) //Don't do Pluck here
				{
					gLastUsedItem = ITEM(gEffectBank);
					if (!ItemBattleEffects(ItemEffects_EndTurn, gBankAttacker, TRUE, TRUE))
					{
						//The Berry didn't activate an effect
						BattleScriptPush(gBattlescriptCurrInstr + 1);
						gBattlescriptCurrInstr = BattleScript_EatEffectBankBerry;
					}
				}
				break;

			case MOVE_EFFECT_RESET_STAT_CHANGES:
				for (int i = 0; i < BATTLE_STATS_NO-1; ++i)
					gBattleMons[gEffectBank].statStages[i] = 6;

				BattleScriptPush(gBattlescriptCurrInstr + 1);
				gBattleStringLoader = TargetStatsResetString;
				gBattlescriptCurrInstr = BattleScript_PrintCustomString;
				break;

			case MOVE_EFFECT_SUPPRESS_ABILITY:
				if (!gNewBS->secondaryEffectApplied
				&& !IsAbilitySuppressed(gEffectBank)
				&&  GetBattlerTurnOrderNum(gEffectBank) < gCurrentTurnActionNumber) //Target moved before attacker
				{
					gNewBS->secondaryEffectApplied = TRUE;
					u8* defAbilityLoc;
					defAbilityLoc = GetAbilityLocation(gBankTarget);

					gStatuses3[gEffectBank] |= STATUS3_ABILITY_SUPPRESS;
					gNewBS->SuppressedAbilities[gEffectBank] = *defAbilityLoc;
					*defAbilityLoc = 0;

					gNewBS->backupAbility = gNewBS->SuppressedAbilities[gEffectBank];

					gBattleScripting.bank = gEffectBank;
					BattleScriptPush(gBattlescriptCurrInstr + 1);
					gBattlescriptCurrInstr = BattleScript_AbilityWasSuppressed;
				}
				else
					gBattlescriptCurrInstr++;

				break;

			case MOVE_EFFECT_BREAK_SCREENS: ;
				u8 side = SIDE(gBankTarget);
				if (gSideTimers[side].reflectTimer
				||  gSideTimers[side].lightscreenTimer
				||  gSideStatuses[side] & SIDE_STATUS_REFLECT
				||  gSideStatuses[side] & SIDE_STATUS_LIGHTSCREEN
				||  gNewBS->AuroraVeilTimers[side])
				{
					RemoveScreensFromSide(side);
					BattleScriptPush(gBattlescriptCurrInstr + 1);
					gBattleStringLoader = ScreensShatteredString;
					gBattlescriptCurrInstr = BattleScript_PrintCustomString;
				}
				else
					gBattlescriptCurrInstr++;
				break;

			case MOVE_EFFECT_REMOVE_TERRAIN:
				if (gBattleTypeFlags & BATTLE_TYPE_BATTLE_CIRCUS && gBattleCircusFlags & BATTLE_CIRCUS_TERRAIN)
				{
					gBattlescriptCurrInstr++;
					break; //Can't be removed
				}

				if (gTerrainType)
				{
					gTerrainType = 0;
					gNewBS->TerrainTimer = 0;
					BattleScriptPush(gBattlescriptCurrInstr + 1);
					gBattleStringLoader = TerrainEndString;
					gBattlescriptCurrInstr = BattleScript_PrintCustomString;

					gActiveBattler = GetFirstAliveActiveBattler();
					EmitDataTransfer(0, &gTerrainType, 1, &gTerrainType);
					MarkBufferBankForExecution(gActiveBattler);
				}
				else
					gBattlescriptCurrInstr++;
				break;

			case MOVE_EFFECT_SPLINTERS:
				if (gNewBS->splinterTimer[gEffectBank] == 0)
				{
					gNewBS->splinterTimer[gEffectBank] = 4; //3 turns of splinters
					gNewBS->splinterAttackerBank[gEffectBank] = gBankAttacker;
					gNewBS->splinterAttackerMonId[gEffectBank] = gBattlerPartyIndexes[gBankAttacker];
					gNewBS->splinterMove[gEffectBank] = gCurrentMove;
					BattleScriptPush(gBattlescriptCurrInstr + 1);
					gBattleStringLoader = gText_EffectBankAfflictedBySplinters;
					gBattlescriptCurrInstr = BattleScript_PrintCustomString;	
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
	bool8 affectsUser = FALSE;
	bool8 effect = FALSE;

	if (gBattleCommunication[MOVE_EFFECT_BYTE] & MOVE_EFFECT_AFFECTS_USER)
	{
		gEffectBank = gBankAttacker; // battlerId that effects get applied on
		gBattleCommunication[MOVE_EFFECT_BYTE] &= ~(MOVE_EFFECT_AFFECTS_USER);
		affectsUser = TRUE;
		gBattleScripting.bank = gBankAttacker;
	}
	else
	{
		gEffectBank = gBankTarget;
		gBattleScripting.bank = gBankAttacker;
	}

	if (CheckTableForSpecialMoveEffect(gBattleCommunication[MOVE_EFFECT_BYTE], sMoveEffectsThatIgnoreSubstitute))
		goto SKIP_SUBSTITUTE_CHECK;

	if (gBattleMons[gEffectBank].hp == 0
	&& gBattleCommunication[MOVE_EFFECT_BYTE] != MOVE_EFFECT_STEAL_ITEM)
	{
		#ifdef PICK_UP_KNOCKED_OFF_ITEMS
		if (gBattleCommunication[MOVE_EFFECT_BYTE] == MOVE_EFFECT_KNOCK_OFF
		&& !(gBattleTypeFlags & BATTLE_TYPE_TRAINER)
		&& SIDE(gEffectBank) == B_SIDE_OPPONENT
		#ifdef FLAG_KEEP_CONSUMABLE_ITEMS
		&& !FlagGet(FLAG_KEEP_CONSUMABLE_ITEMS) //Can't keep these items
		#endif
		)
		{
			//Allow knocking off wild item even if KOd
		}
		else
		#endif
			RESET_RETURN
	}

	if (MoveBlockedBySubstitute(gCurrentMove, gBattleScripting.bank, gEffectBank)
	&& !affectsUser
	&& !(gHitMarker & HITMARKER_IGNORE_SUBSTITUTE))
		RESET_RETURN

	if (HasRaidShields(gEffectBank)
	&& !affectsUser
	&& !(gHitMarker & HITMARKER_IGNORE_SUBSTITUTE))
		RESET_RETURN

	SKIP_SUBSTITUTE_CHECK:
	switch (gBattleCommunication[MOVE_EFFECT_BYTE]) {
		case MOVE_EFFECT_STEAL_ITEM:
			if (!(gBattleTypeFlags & BATTLE_TYPE_TRAINER) && SIDE(gBankAttacker) == B_SIDE_OPPONENT) //Wild mons can steal items
			{
				break;
			}
			else if (!BATTLER_ALIVE(gBankAttacker)
			|| ITEM(gEffectBank) == ITEM_NONE
			|| ITEM(gBankAttacker) != ITEM_NONE
			|| !CanTransferItem(SPECIES(gEffectBank), ITEM(gEffectBank))
			|| !CanTransferItem(SPECIES(gBankAttacker), ITEM(gEffectBank))
			|| (gNewBS->corrodedItems[SIDE(gBankAttacker)] & gBitTable[gBattlerPartyIndexes[gBankAttacker]]))
			{
				break;
			}
			else if (ABILITY(gEffectBank) == ABILITY_STICKYHOLD && gBattleMons[gEffectBank].hp != 0)
			{
				//Handled in CMD49
				/*
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_StickyHoldActivatesRet;

				gLastUsedAbility = ABILITY(gEffectBank);
				RecordAbilityBattle(gEffectBank, gLastUsedAbility);
				effect = TRUE;
				*/
				break;
			}
			else
			{
				gLastUsedItem = gBattleMons[gEffectBank].item;
				gBattleMons[gEffectBank].item = 0;
				gBattleMons[gBankAttacker].item = gLastUsedItem;
				HandleUnburdenBoost(gEffectBank); //Give target Unburden boost
				HandleUnburdenBoost(gBankAttacker); //Remove attacker's Unburden boost

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
			if (!CanKnockOffItem(gEffectBank)
			||	!BATTLER_ALIVE(gBankAttacker))
			{
				break;
			}
			else if (ABILITY(gEffectBank) == ABILITY_STICKYHOLD && gBattleMons[gEffectBank].hp)
			{
				//Handled in CMD49
				/*
				gLastUsedAbility = ABILITY_STICKYHOLD;
				gBattlescriptCurrInstr = BattleScript_StickyHoldActivatesRet;
				gBattleScripting.bank = gEffectBank;
				effect = TRUE;
				*/
				break;
			}
			else
			{
				gLastUsedItem = gBattleMons[gEffectBank].item;
				gBattleMons[gEffectBank].item = 0;
				HandleUnburdenBoost(gEffectBank); //Give target Unburden boost

				gActiveBattler = gEffectBank;
				EmitSetMonData(0, REQUEST_HELDITEM_BATTLE, 0, 2, &gBattleMons[gActiveBattler].item);
				MarkBufferBankForExecution(gActiveBattler);

				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_KnockedOff;

				gBattleStruct->choicedMove[gEffectBank] = 0;

				#ifdef PICK_UP_KNOCKED_OFF_ITEMS
				if (!(gBattleTypeFlags & BATTLE_TYPE_TRAINER)
				&& SIDE(gEffectBank) == B_SIDE_OPPONENT //Wild Pokemon's item
				#ifdef FLAG_KEEP_CONSUMABLE_ITEMS
				&& !FlagGet(FLAG_KEEP_CONSUMABLE_ITEMS) //Can't keep these items
				#endif
				)
					gNewBS->knockedOffWildItem = gLastUsedItem;
				#endif

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
			||  IsGravityActive())
			{
				break;
			}

			if (IsOfType(gEffectBank, TYPE_FLYING)
			||  ABILITY(gEffectBank) == ABILITY_LEVITATE
			|| (gStatuses3[gEffectBank] & (STATUS3_IN_AIR | STATUS3_LEVITATING | STATUS3_TELEKINESIS))
			|| (ITEM_EFFECT(gEffectBank) == ITEM_EFFECT_AIR_BALLOON)
			||  IsFloatingWithMagnetism(gEffectBank))
			{
			SMACK_TGT_DOWN:
				gStatuses3[gEffectBank] |= STATUS3_SMACKED_DOWN;
				gNewBS->targetsToBringDown |= gBitTable[gEffectBank];
				BringDownMons();
				gBattlescriptCurrInstr = BattleScript_PrintCustomString;
				effect = TRUE;
			}
			break;

		case MOVE_EFFECT_ION_DELUGE:
			if (!IsIonDelugeActive())
				gNewBS->IonDelugeTimer = 1;
			BattleScriptPushCursor();
			gBattleStringLoader = IonDelugeShowerString;
			gBattlescriptCurrInstr = BattleScript_PrintCustomString;
			effect = TRUE;
			break;
	}

	gBattleCommunication[MOVE_EFFECT_BYTE] = 0;
	return effect;
}
