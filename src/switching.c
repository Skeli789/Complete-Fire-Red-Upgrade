#include "defines.h"
#include "defines_battle.h"
#include "../include/random.h"
#include "../include/party_menu.h"

#include "../include/new/ability_battle_scripts.h"
#include "../include/new/ai_master.h"
#include "../include/new/battle_indicators.h"
#include "../include/new/battle_script_util.h"
#include "../include/new/battle_start_turn_start.h"
#include "../include/new/battle_start_turn_start_battle_scripts.h"
#include "../include/new/battle_util.h"
#include "../include/new/cmd49_battle_scripts.h"
#include "../include/new/damage_calc.h"
#include "../include/new/dynamax.h"
#include "../include/new/form_change.h"
#include "../include/new/frontier.h"
#include "../include/new/util.h"
#include "../include/new/item.h"
#include "../include/new/item_battle_scripts.h"
#include "../include/new/move_battle_scripts.h"
#include "../include/new/mega.h"
#include "../include/new/multi.h"
#include "../include/new/switching.h"
#include "../include/new/trainer_sliding.h"
#include "../include/new/z_move_battle_scripts.h"
/*
switching.c
	handles battle switching logic
*/

enum SwitchInStates
{
	SwitchIn_CamomonsReveal,
	SwitchIn_HealingWish,
	SwitchIn_ZHealingWish,
	SwitchIn_Spikes,
	SwitchIn_StealthRock,
	SwitchIn_Steelsurge,
	SwitchIn_ToxicSpikes,
	SwitchIn_StickyWeb,
	SwitchIn_EmergencyExit,
	SwitchIn_PrimalReversion,
	SwitchIn_Truant,
	SwitchIn_Abilities,
	SwitchIn_Items,
	SwitchIn_AirBalloon,
	SwitchIn_TotemPokemon,
	SwitchIn_TrainerMessage,
	SwitchIn_PreEnd,
	SwitchIn_EjectPack,
	SwitchIn_End,
};

//This file's functions:
static bool8 TryRemovePrimalWeather(u8 bank, u8 ability);
static bool8 TryRemoveNeutralizingGas(u8 ability);
static bool8 TryRemoveUnnerve(u8 bank);
static bool8 TryActivateFlowerGift(u8 leavingBank);
static bool8 TryDoForceSwitchOut(void);
static void sub_80571DC(u8 battlerId, u8 arg1);
static bool8 PPIsMaxed(bank_t);
static u8 GetStealthRockDivisor(void);

void atkE2_switchoutabilities(void)
{
	gActiveBattler = GetBattleBank(T2_READ_8(gBattlescriptCurrInstr + 1));

	switch (ABILITY(gActiveBattler)) {
		case ABILITY_NATURALCURE:
			gBattleMons[gActiveBattler].status1 = 0;
			EmitSetMonData(0, REQUEST_STATUS_BATTLE, gBitTable[gBattleStruct->switchoutPartyIndex[gActiveBattler]], 4, &gBattleMons[gActiveBattler].status1);
			MarkBufferBankForExecution(gActiveBattler);
			break;
		case ABILITY_REGENERATOR:
			gBattleMons[gActiveBattler].hp = MathMin(gBattleMons[gActiveBattler].maxHP, gBattleMons[gActiveBattler].hp + udivsi(gBattleMons[gActiveBattler].maxHP, 3));
			EmitSetMonData(0, REQUEST_HP_BATTLE, gBitTable[gBattleStruct->switchoutPartyIndex[gActiveBattler]], 4, &gBattleMons[gActiveBattler].hp);
			MarkBufferBankForExecution(gActiveBattler);
	}

	SwitchOutFormsRevert(gActiveBattler);

	gBattlescriptCurrInstr += 2;
}

bool8 HandleSpecialSwitchOutAbilities(u8 bank, u8 ability)
{
	return TryRemovePrimalWeather(bank, ability)
		|| TryRemoveNeutralizingGas(ability)
		|| TryRemoveUnnerve(bank)
		|| TryActivateFlowerGift(bank);
}

static bool8 TryRemovePrimalWeather(u8 bank, u8 ability)
{
	int i;
	gBattleStringLoader = NULL;

	switch (ability) {
		case ABILITY_PRIMORDIALSEA:
			if (gBattleWeather & WEATHER_RAIN_PRIMAL)
				gBattleStringLoader = PrimalRainEndString;
			break;
		case ABILITY_DESOLATELAND:
			if (gBattleWeather & WEATHER_SUN_PRIMAL)
				gBattleStringLoader = PrimalSunEndString;
			break;
		case ABILITY_DELTASTREAM:
			if (gBattleWeather & WEATHER_AIR_CURRENT_PRIMAL)
				gBattleStringLoader = PrimalAirCurrentEndString;
	}

	if (gBattleStringLoader != NULL)
	{
		for (i = 0; i < gBattlersCount; ++i)
		{
			if (i == bank) continue;
			if (ABILITY(i) == ability) break;
		}

		if (i == gBattlersCount)
		{
			gBattleWeather = 0;
			gWeatherCounter = 0;
			BattleScriptPushCursor();
			gBattlescriptCurrInstr = BattleScript_PrimalWeatherEnd;
			return TRUE;
		}
	}

	return FALSE;
}

static bool8 TryRemoveNeutralizingGas(u8 ability)
{
	if (ability == ABILITY_NEUTRALIZINGGAS)
	{
		if (!gNewBS->printedNeutralizingGasOverMsg)
		{
			BattleScriptPushCursor();
			gBattleStringLoader = gText_NeutralizingGasEnd;
			gBattlescriptCurrInstr = BattleScript_PrintCustomString;
			gNewBS->printedNeutralizingGasOverMsg = TRUE;
			return TRUE;
		}
	
		for (int i = 0; i < gBattlersCount; ++i)
		{
			u8 bank = gBanksByTurnOrder[i];
		
			if (gNewBS->neutralizingGasBlockedAbilities[bank] != ABILITY_NONE)
			{
				u8 ability = *GetAbilityLocationIgnoreNeutralizingGas(bank) = gNewBS->neutralizingGasBlockedAbilities[bank]; //Restore ability
				gNewBS->neutralizingGasBlockedAbilities[bank] = ABILITY_NONE;
				gNewBS->SlowStartTimers[bank] = 0;
				gDisableStructs[gBankTarget].truantCounter = 0;

				//Some abilities don't reactivate
				switch (ability) {
					case ABILITY_UNNERVE:
						break;
					case ABILITY_IMPOSTER: //Never gets another chance
						gStatuses3[bank] |= STATUS3_SWITCH_IN_ABILITY_DONE;
						break;
					default:
						gStatuses3[bank] &= ~STATUS3_SWITCH_IN_ABILITY_DONE;
				}

				if (AbilityBattleEffects(ABILITYEFFECT_ON_SWITCHIN, bank, 0, 0, 0))
					return TRUE;
			}
		}
	}

	gNewBS->printedNeutralizingGasOverMsg = FALSE; //Reset for next time
	return FALSE;
}

static bool8 TryRemoveUnnerve(u8 bank)
{
	u8 side = SIDE(bank);
	bool8 ret = FALSE;

	if (ABILITY(bank) == ABILITY_UNNERVE)
	{
		*GetAbilityLocation(bank) = ABILITY_NONE; //Temporarily remove Unnerve so Berries can activate

		for (int i = 0; i < gBattlersCount; ++i)
		{
			u8 bank = gBanksByTurnOrder[i];
			if (SIDE(bank) == side) //Ignore Berries on the side of the Unnerver
				continue;

			if (IsBerry(ITEM(bank)))
			{
				if (ItemBattleEffects(ItemEffects_EndTurn, bank, TRUE, FALSE))
				{
					ret = TRUE;
					break;
				}
			}
		}

		*GetAbilityLocation(bank) = ABILITY_UNNERVE; //Restore Unnerve so loop can continue when we return to this function
	}
	
	return ret;
}

static bool8 TryActivateFlowerGift(u8 leavingBank)
{
	u32 i = 0;

	if (ABILITY(leavingBank) == ABILITY_AIRLOCK
	||  ABILITY(leavingBank) == ABILITY_CLOUDNINE)
		gBattleMons[leavingBank].ability = ABILITY_NONE; //Remove ability because we can't have these anymore

	for (u8 bank = gBanksByTurnOrder[i]; i < gBattlersCount; ++i, bank = gBanksByTurnOrder[i])
	{
		if ((ABILITY(bank) == ABILITY_FLOWERGIFT ||  ABILITY(bank) == ABILITY_FORECAST)) //Just in case someone with Air Lock/Cloud Nine switches out
		{
			gStatuses3[bank] &= ~STATUS3_SWITCH_IN_ABILITY_DONE;

			if (AbilityBattleEffects(ABILITYEFFECT_ON_SWITCHIN, bank, 0, 0, 0))
				return TRUE;
		}
	}

	return FALSE;
}

void atk61_drawpartystatussummary(void)
{
	int i;
	pokemon_t* party;
	struct HpAndStatus hpStatus[6];
	if (gBattleExecBuffer)
		return;

	RestoreOriginalAttackerAndTarget();
	gNewBS->skipBankStatAnim = gActiveBattler = GetBattleBank(gBattlescriptCurrInstr[1]);

	if (HandleSpecialSwitchOutAbilities(gActiveBattler, ABILITY(gActiveBattler)))
		return;
	
	gNewBS->skipBankStatAnim = 0xFF; //No longer needed
	gActiveBattler = GetBattleBank(gBattlescriptCurrInstr[1]);

	if (SIDE(gActiveBattler) == 0)
		party = gPlayerParty;
	else
		party = gEnemyParty;

	for (i = 0; i < 6; ++i)
	{
		if (party[i].species == SPECIES_NONE || party[i].species == SPECIES_EGG)
		{
			hpStatus[i].hp = 0xFFFF;
			hpStatus[i].status = 0;
		}
		else
		{
			hpStatus[i].hp = party[i].hp;
			hpStatus[i].status = party[i].condition;
		}
	}
	EmitDrawPartyStatusSummary(0, hpStatus, 1);
	MarkBufferBankForExecution(gActiveBattler);
	gBattlescriptCurrInstr += 2;
}

void atk4D_switchindataupdate(void)
{
	struct BattlePokemon oldData;
	u32 i;
	u8* monData;

	if (gBattleExecBuffer) return;

	gActiveBattler = GetBattleBank(gBattlescriptCurrInstr[1]);
	oldData = gBattleMons[gActiveBattler];
	monData = (u8*)(&gBattleMons[gActiveBattler]);

	for (i = 0; i < sizeof(struct BattlePokemon); ++i)
		monData[i] = gBattleBufferB[gActiveBattler][4 + i];

	if (gBattleTypeFlags & BATTLE_TYPE_CAMOMONS) //The Pokemon takes on the types of its first two moves
	{
		UpdateTypesForCamomons(gActiveBattler);
	}
	else
	{
		gBattleMons[gActiveBattler].type1 = gBaseStats[gBattleMons[gActiveBattler].species].type1;
		gBattleMons[gActiveBattler].type2 = gBaseStats[gBattleMons[gActiveBattler].species].type2;
	}

	gBattleMons[gActiveBattler].ability = GetMonAbility(GetBankPartyData(gActiveBattler));

	CONSUMED_ITEMS(gActiveBattler) = 0;
	gNewBS->StakeoutCounters[gActiveBattler] = 1;

	ClearSwitchBytes(gActiveBattler);
	ClearSwitchBits(gActiveBattler);

	//gNewBS->LastUsedMoves[gActiveBattler] = 0;
	//gNewBS->LastUsedTypes[gActiveBattler] = TYPE_BLANK;

	//Former Knock Off Check was here

	if (gCurrentMove == MOVE_BATONPASS)
	{
		for (i = 0; i < BATTLE_STATS_NO-1; ++i)
			gBattleMons[gActiveBattler].statStages[i] = oldData.statStages[i];

		gBattleMons[gActiveBattler].status2 = oldData.status2;

		//Gastro Acid Passing
		if (IsAbilitySuppressed(gActiveBattler))
		{
			gNewBS->SuppressedAbilities[gActiveBattler] = gBattleMons[gActiveBattler].ability;
			gBattleMons[gActiveBattler].ability = 0;
		}

		//Power Trick Passing
		if (gStatuses3[gActiveBattler] & STATUS3_POWER_TRICK)
		{
			u8 attack = gBattleMons[gActiveBattler].attack;
			u8 defense = gBattleMons[gActiveBattler].defense;
			gBattleMons[gActiveBattler].attack = defense;
			gBattleMons[gActiveBattler].defense = attack;
		}
	}

	if (!(gStatuses3[gActiveBattler] & STATUS3_LEVITATING))
		gNewBS->MagnetRiseTimers[gActiveBattler] = 0;

	{
		u32 backupStatus2[gBattlersCount];
		for (i = 0; i < gBattlersCount; ++i)
			backupStatus2[i] = gBattleMons[i].status2;
		SwitchInClearSetData();
		TryReactivateCentifernoSandblast(backupStatus2);
	}

	if (ABILITY(gActiveBattler) == ABILITY_ILLUSION)
	{
		gStatuses3[gActiveBattler] |= STATUS3_ILLUSION;

		if (GetIllusionPartyData(gActiveBattler) != GetBankPartyData(gActiveBattler))
		{
			EmitDataTransfer(0, &gStatuses3[gActiveBattler], 4, &gStatuses3[gActiveBattler]);
			MarkBufferBankForExecution(gActiveBattler);
		}
		else
			gStatuses3[gActiveBattler] &= ~STATUS3_ILLUSION;
	}

	gBattleScripting->bank = gActiveBattler;

	PREPARE_MON_NICK_BUFFER(gBattleTextBuff1, gActiveBattler, GetIllusionPartyNumber(gActiveBattler));

	gBattleMons[gActiveBattler].type3 = TYPE_BLANK;

	gBattlescriptCurrInstr += 2;
}

void TryReactivateCentifernoSandblast(u32* status2)
{
	for (int i = 0; i < gBattlersCount; ++i)
	{
		//Check if special Fire Spin or Sand Tomb were undone by the switch, and they reactivate them
		if (gNewBS->sandblastCentiferno[i])
			gBattleMons[i].status2 |= (status2[i] & STATUS2_WRAPPED);
	}
}

void atk4F_jumpifcantswitch(void)
{
	int i;
	u8 firstMonId, lastMonId;
	u8 battlerIn1, battlerIn2;
	u8 foe1, foe2;

	gActiveBattler = GetBattleBank(T2_READ_8(gBattlescriptCurrInstr + 1) & ~(ATK4F_DONT_CHECK_STATUSES));
	LoadBattlersAndFoes(&battlerIn1, &battlerIn2, &foe1, &foe2);
	struct Pokemon* party = LoadPartyRange(gActiveBattler, &firstMonId, &lastMonId);

	if (!(T2_READ_8(gBattlescriptCurrInstr + 1) & ATK4F_DONT_CHECK_STATUSES)
	&& !IsOfType(gActiveBattler, TYPE_GHOST)
	&& ITEM_EFFECT(gActiveBattler) != ITEM_EFFECT_SHED_SHELL
	&& ((gBattleMons[gActiveBattler].status2 & (STATUS2_WRAPPED | STATUS2_ESCAPE_PREVENTION)) || (gStatuses3[gActiveBattler] & STATUS3_ROOTED) || IsFairyLockActive()))
	{
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 2);
	}
	else
	{
		for (i = firstMonId; i < lastMonId; ++i)
		{
			if (GetMonData(&party[i], MON_DATA_HP, 0) != 0
			&& GetMonData(&party[i], MON_DATA_SPECIES, 0) != SPECIES_NONE
			&& !GetMonData(&party[i], MON_DATA_IS_EGG, 0)
			&& i != gBattlerPartyIndexes[battlerIn1]
			&& i != gBattlerPartyIndexes[battlerIn2])
				break;
		}

		if (i == lastMonId)
			gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 2);
		else
			gBattlescriptCurrInstr += 6;
	}
}

void atk51_switchhandleorder(void)
{
	int i;
	if (gBattleExecBuffer) return;

	gActiveBattler = GetBattleBank(gBattlescriptCurrInstr[1]);

	switch (gBattlescriptCurrInstr[2]) {
		case 0:
			for (i = 0; i < gBattlersCount; ++i)
			{
				if (gBattleBufferB[i][0] == CONTROLLER_CHOSENMONRETURNVALUE)
					gBattleStruct->monToSwitchIntoId[i] = gBattleBufferB[i][1];
			}
			break;

		case 1:
			if (!(gBattleTypeFlags & BATTLE_TYPE_MULTI))
				sub_8013F6C(gActiveBattler);
			break;

		case 2:
			gBattleCommunication[0] = gBattleBufferB[gActiveBattler][1];
			gBattleStruct->monToSwitchIntoId[gActiveBattler] = gBattleBufferB[gActiveBattler][1];

			if (gBattleTypeFlags & BATTLE_TYPE_LINK && gBattleTypeFlags & BATTLE_TYPE_MULTI)
			{
				gBattleStruct->field_60[gActiveBattler][0] &= 0xF;
				gBattleStruct->field_60[gActiveBattler][0] |= (gBattleBufferB[gActiveBattler][2] & 0xF0);
				gBattleStruct->field_60[gActiveBattler][1] = gBattleBufferB[gActiveBattler][3];

				gBattleStruct->field_60[gActiveBattler ^ BIT_FLANK][0] &= (0xF0);
				gBattleStruct->field_60[gActiveBattler ^ BIT_FLANK][0] |= (gBattleBufferB[gActiveBattler][2] & 0xF0) >> 4;
				gBattleStruct->field_60[gActiveBattler ^ BIT_FLANK][2] = gBattleBufferB[gActiveBattler][3];
			}
			else if (gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER)
				sub_80571DC(gActiveBattler, gBattleStruct->monToSwitchIntoId[gActiveBattler]);

			else
				sub_8013F6C(gActiveBattler);

			PREPARE_SPECIES_BUFFER(gBattleTextBuff1, gBattleMons[gBankAttacker].species)
			PREPARE_MON_NICK_BUFFER(gBattleTextBuff2, gActiveBattler, gBattleBufferB[gActiveBattler][1])
			break;
		}
	gBattlescriptCurrInstr += 3;
}

void atk52_switchineffects(void)
{
	int i;
	u8 arg = T2_READ_8(gBattlescriptCurrInstr + 1);
	if (arg == BS_GET_SCRIPTING_BANK)
		gBattleScripting->bank = gNewBS->SentInBackup; //Restore scripting backup b/c can get changed

	gActiveBattler = GetBattleBank(arg);
	sub_80174B8(gActiveBattler);
	gHitMarker &= ~(HITMARKER_FAINTED(gActiveBattler));
	gSpecialStatuses[gActiveBattler].flag40 = 0;
	u8 ability = ABILITY(gActiveBattler);
	u8 itemEffect = ITEM_EFFECT(gActiveBattler);

	if (gBattleMons[gActiveBattler].hp == 0)
		goto SWITCH_IN_END;

	if (!(gSideAffecting[SIDE(gActiveBattler)] & SIDE_STATUS_SPIKES) //Skip the entry hazards if there are none
	&& gNewBS->SwitchInEffectsTracker >= SwitchIn_Spikes
	&& gNewBS->SwitchInEffectsTracker <= SwitchIn_StickyWeb)
		gNewBS->SwitchInEffectsTracker = SwitchIn_PrimalReversion;

	switch (gNewBS->SwitchInEffectsTracker) {
		case SwitchIn_CamomonsReveal:
			if (gBattleTypeFlags & BATTLE_TYPE_CAMOMONS)
			{
				gBattleScripting->bank = gActiveBattler;
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_CamomonsTypeRevealRet;

				if (gBattleMons[gActiveBattler].type1 == gBattleMons[gActiveBattler].type2)
					gBattleStringLoader = gText_CamomonsTypeReveal;
				else
					gBattleStringLoader = gText_CamomonsTypeRevealDualType;
				PREPARE_TYPE_BUFFER(gBattleTextBuff1, gBattleMons[gActiveBattler].type1);
				PREPARE_TYPE_BUFFER(gBattleTextBuff2, gBattleMons[gActiveBattler].type2);
			}
			++gNewBS->SwitchInEffectsTracker;
			break;

		case SwitchIn_HealingWish:
			if (gBattleMons[gActiveBattler].hp != gBattleMons[gActiveBattler].maxHP
			|| gBattleMons[gActiveBattler].status1 != STATUS1_NONE)
			{
				if (gNewBS->HealingWishLoc & gBitTable[GetBattlerPosition(gActiveBattler)])
				{
					gNewBS->HealingWishLoc &= ~gBitTable[GetBattlerPosition(gActiveBattler)];
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_HealingWishHeal;
					gBattleMoveDamage = -1 * (gBattleMons[gActiveBattler].maxHP);
					gBattleMons[gActiveBattler].status1 = 0;
					EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 4, &gBattleMons[gActiveBattler].status1);
					gBattleScripting->bank = gActiveBattler;
					gBankAttacker = gActiveBattler;
					++gNewBS->SwitchInEffectsTracker;
					return;
				}
				else if (gNewBS->HealingWishLoc & (gBitTable[GetBattlerPosition(gActiveBattler)] << 4))
				{
					goto LUNAR_DANCE_ACTIVATE;
				}
			}
			else if (gNewBS->HealingWishLoc & (gBitTable[GetBattlerPosition(gActiveBattler)] << 4) && !PPIsMaxed(gActiveBattler))
			{
			LUNAR_DANCE_ACTIVATE:
				gNewBS->HealingWishLoc &= ~(gBitTable[GetBattlerPosition(gActiveBattler)] << 4);
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_LunarDanceHeal;

				gBattleMoveDamage = -1 * (gBattleMons[gActiveBattler].maxHP);
				gBattleMons[gActiveBattler].status1 = 0;
				EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 4, &gBattleMons[gActiveBattler].status1);

				//PP Restored in Battle Script

				gBattleScripting->bank = gActiveBattler;
				gBankAttacker = gActiveBattler;
				++gNewBS->SwitchInEffectsTracker;
				return;
			}
			++gNewBS->SwitchInEffectsTracker;
		__attribute__ ((fallthrough));

		case SwitchIn_ZHealingWish:
			if (gBattleMons[gActiveBattler].hp != gBattleMons[gActiveBattler].maxHP && gNewBS->ZMoveData->healReplacement)
			{
				gNewBS->ZMoveData->healReplacement = FALSE;
				gBattleMoveDamage = -1 * (gBattleMons[gActiveBattler].maxHP);
				gBattleScripting->bank = gActiveBattler;
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_HealReplacementZMove;
				return;
			}
			else
				gNewBS->ZMoveData->healReplacement = FALSE;

			gNewBS->DamageTaken[gActiveBattler] = 0;
			++gNewBS->SwitchInEffectsTracker;
		__attribute__ ((fallthrough));

		case SwitchIn_Spikes:
			if (CheckGrounding(gActiveBattler)
			&& gSideTimers[SIDE(gActiveBattler)].spikesAmount > 0
			&& ability != ABILITY_MAGICGUARD
			&& itemEffect != ITEM_EFFECT_HEAVY_DUTY_BOOTS)
			{
				gBattleMoveDamage = CalcSpikesDamage(gActiveBattler);
				gNewBS->DamageTaken[gActiveBattler] += gBattleMoveDamage;

				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_SpikesHurt;
				gSideAffecting[SIDE(gActiveBattler)] |= SIDE_STATUS_SPIKES_DAMAGED;
				gBattleScripting->bank = gActiveBattler;
				gBankTarget = gActiveBattler;
				//gBankAttacker = FOE(gActiveBattler); //For EXP
				++gNewBS->SwitchInEffectsTracker;
				return;
			}
			++gNewBS->SwitchInEffectsTracker;
		__attribute__ ((fallthrough));

		case SwitchIn_StealthRock:
			if (gSideTimers[SIDE(gActiveBattler)].srAmount > 0
			&& ability != ABILITY_MAGICGUARD
			&& itemEffect != ITEM_EFFECT_HEAVY_DUTY_BOOTS)
			{
				gBattleMoveDamage = CalcStealthRockDamage(gActiveBattler);
				gNewBS->DamageTaken[gActiveBattler] += gBattleMoveDamage;

				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_SRHurt;
				gSideAffecting[SIDE(gActiveBattler)] |= SIDE_STATUS_SPIKES_DAMAGED;
				gBattleScripting->bank = gActiveBattler;
				gBankTarget = gActiveBattler;
				//gBankAttacker = FOE(gActiveBattler); //For EXP
				++gNewBS->SwitchInEffectsTracker;
				return;
			}
			++gNewBS->SwitchInEffectsTracker;
		__attribute__ ((fallthrough));
		
		case SwitchIn_Steelsurge:
			if (gSideTimers[SIDE(gActiveBattler)].steelsurge > 0
			&& ability != ABILITY_MAGICGUARD
			&& itemEffect != ITEM_EFFECT_HEAVY_DUTY_BOOTS)
			{
				gBattleMoveDamage = CalcSteelsurgeDamage(gActiveBattler);
				gNewBS->DamageTaken[gActiveBattler] += gBattleMoveDamage;

				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_SteelsurgeHurt;
				gSideAffecting[SIDE(gActiveBattler)] |= SIDE_STATUS_SPIKES_DAMAGED;
				gBattleScripting->bank = gActiveBattler;
				gBankTarget = gActiveBattler;
				//gBankAttacker = FOE(gActiveBattler); //For EXP
				++gNewBS->SwitchInEffectsTracker;
				return;
			}
			++gNewBS->SwitchInEffectsTracker;
		__attribute__ ((fallthrough));

		case SwitchIn_ToxicSpikes:
			if (gSideTimers[SIDE(gActiveBattler)].tspikesAmount > 0
			&& CheckGrounding(gActiveBattler))
			{
				if (gBattleMons[gActiveBattler].type1 == TYPE_POISON
				||  gBattleMons[gActiveBattler].type2 == TYPE_POISON
				||  gBattleMons[gActiveBattler].type3 == TYPE_POISON)
				{
					gSideTimers[SIDE(gActiveBattler)].tspikesAmount = 0;
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_TSAbsorb;
				}
				else if (itemEffect != ITEM_EFFECT_HEAVY_DUTY_BOOTS) //Pokemon with this item can still remove T-Spikes
				{
					if (gSideTimers[SIDE(gActiveBattler)].tspikesAmount == 1)
					{
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_TSPoison;
					}
					else
					{
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_TSHarshPoison;
					}

					gSideAffecting[SIDE(gActiveBattler)] |= SIDE_STATUS_SPIKES_DAMAGED;
				}

				gBattleScripting->bank = gActiveBattler;
				gBankTarget = gActiveBattler;
				//gBankAttacker = FOE(gActiveBattler); //For EXP
				++gNewBS->SwitchInEffectsTracker;
				return;
			}
			++gNewBS->SwitchInEffectsTracker;
		__attribute__ ((fallthrough));

		case SwitchIn_StickyWeb:
			if (gSideTimers[SIDE(gActiveBattler)].stickyWeb
			&&  CheckGrounding(gActiveBattler)
			&& itemEffect != ITEM_EFFECT_HEAVY_DUTY_BOOTS)
			{
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_StickyWebSpeedDrop;
				gSideAffecting[SIDE(gActiveBattler)] |= SIDE_STATUS_SPIKES_DAMAGED;
				gBattleScripting->bank = gActiveBattler;
				gBankTarget = gActiveBattler;
				//gBankAttacker = FOE(gActiveBattler); //For EXP
				++gNewBS->SwitchInEffectsTracker;
				return;
			}
			++gNewBS->SwitchInEffectsTracker;
		__attribute__ ((fallthrough));

		case SwitchIn_EmergencyExit:
			if (ABILITY(gActiveBattler) == ABILITY_EMERGENCYEXIT
			/*||  ABILITY(gActiveBattler) == ABILITY_WIMPOUT*/)
			{
				if (gBattleMons[gActiveBattler].hp > 0
				&&  gBattleMons[gActiveBattler].hp <= gBattleMons[gActiveBattler].maxHP / 2
				&&  gBattleMons[gActiveBattler].hp + gNewBS->DamageTaken[gActiveBattler] > gBattleMons[gActiveBattler].maxHP / 2)
				{
					BattleScriptPush(gBattlescriptCurrInstr + 2);
					gBattlescriptCurrInstr = BattleScript_EmergencyExit;
					return;
				}
			}
			++gNewBS->SwitchInEffectsTracker;
		__attribute__ ((fallthrough));

		case SwitchIn_PrimalReversion:	;
			const u8* script = DoPrimalReversion(gActiveBattler, 1);
			if(script)
			{
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = script;
				gBattleScripting->bank = gActiveBattler;
				gBankAttacker = gActiveBattler;
				++gNewBS->SwitchInEffectsTracker;
				return;
			}
			++gNewBS->SwitchInEffectsTracker;
		__attribute__ ((fallthrough));

		case SwitchIn_Truant:
			if (ABILITY(gActiveBattler) == ABILITY_TRUANT)
				gDisableStructs[gActiveBattler].truantCounter ^= 1;
			++gNewBS->SwitchInEffectsTracker;
		__attribute__ ((fallthrough));

		case SwitchIn_Abilities:
			for (i = 0; i < gBattlersCount; ++i)
			{
				if (i != gActiveBattler
				&& ABILITY(i) == ABILITY_TRACE
				&& AbilityBattleEffects(ABILITYEFFECT_ON_SWITCHIN, i, 0, 0, 0))
					return;
			}

			if (AbilityBattleEffects(ABILITYEFFECT_ON_SWITCHIN, gActiveBattler, 0, 0, 0))
				return;

			++gNewBS->SwitchInEffectsTracker;
		__attribute__ ((fallthrough));

		case SwitchIn_Items:
			if (ItemBattleEffects(ItemEffects_SwitchIn, gActiveBattler, TRUE, FALSE))
				return;

			if (ItemBattleEffects(ItemEffects_EndTurn, gActiveBattler, TRUE, FALSE))
				return;
			++gNewBS->SwitchInEffectsTracker;
		__attribute__ ((fallthrough));

		case SwitchIn_AirBalloon:
			if (gActiveBattler < gBattlersCount && ITEM_EFFECT(gActiveBattler) == ITEM_EFFECT_AIR_BALLOON)
			{
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_AirBalloonSub;
				gBattleScripting->bank = gActiveBattler;
				gBankAttacker = gActiveBattler;
				++gNewBS->SwitchInEffectsTracker;
				return;
			}
			++gNewBS->SwitchInEffectsTracker;
		__attribute__ ((fallthrough));

		case SwitchIn_TotemPokemon:
			if (CanActivateTotemBoost(gActiveBattler))
			{
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_TotemRet;
				gBankAttacker = gBattleScripting->bank = gActiveBattler;
				++gNewBS->SwitchInEffectsTracker;
				return;
			}
			++gNewBS->SwitchInEffectsTracker;
		__attribute__ ((fallthrough));

		case SwitchIn_TrainerMessage:
			++gNewBS->SwitchInEffectsTracker;
			if (ShouldDoTrainerSlide(gActiveBattler, gTrainerBattleOpponent_A, TRAINER_SLIDE_LAST_SWITCHIN))
			{
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_TrainerSlideMsgRet;
				return;
			}
		__attribute__ ((fallthrough));

		case SwitchIn_PreEnd:
		SWITCH_IN_END:
			gSideAffecting[SIDE(gActiveBattler)] &= ~(SIDE_STATUS_SPIKES_DAMAGED);

			for (i = 0; i < gBattlersCount; ++i)
			{
				if (gBanksByTurnOrder[i] == gActiveBattler)
					gActionsByTurnOrder[i] = ACTION_FINISHED;
			}

			for (i = 0; i < gBattlersCount; i++)
				gBattleStruct->hpOnSwitchout[SIDE(i)] = gBattleMons[i].hp;

			if (T2_READ_8(gBattlescriptCurrInstr + 1) == 5)
			{
				u32 hitmark = gHitMarker >> 0x1C;
				++gBank1;
				while (1)
				{
					if (hitmark & gBitTable[gBank1] && !(gAbsentBattlerFlags & gBitTable[gBank1]))
						break;
					if (gBank1 >= gBattlersCount)
						break;
					++gBank1;
				}
			}
			++gNewBS->SwitchInEffectsTracker;
		__attribute__ ((fallthrough));

		case SwitchIn_EjectPack:
			gNewBS->SwitchInEffectsTracker = 0;
			gBattlescriptCurrInstr += 2;

			for (i = 0; i < gBattlersCount; ++i)
			{
				if (ITEM_EFFECT(i) == ITEM_EFFECT_EJECT_PACK && ItemBattleEffects(ItemEffects_SwitchIn, i, TRUE, FALSE))  //Try to trigger Eject Packs after Intimidate
					return;
			}
		__attribute__ ((fallthrough));

		case SwitchIn_End:
			break;
	}
}

void RestorePPLunarDance(void)
{
	u8 i, maxPP;
	gActiveBattler = gBattleScripting->bank;

	if (gBattleExecBuffer)
	{
		gBattlescriptCurrInstr -= 5;
		return;
	}

	for (i = 0; i < MAX_MON_MOVES; ++i)
	{
		maxPP = 0;
		if (gBattleMons[gActiveBattler].moves[i] != MOVE_NONE)
		{
			maxPP = CalculatePPWithBonus(gBattleMons[gActiveBattler].moves[i], gBattleMons[gActiveBattler].ppBonuses, i);
			if (IS_TRANSFORMED(gActiveBattler) && maxPP > 5)
				maxPP = 5; //Can't restore past 5 PP if transformed	
		}

		if (gBattleMons[gActiveBattler].pp[i] != maxPP)
		{
			gBattleMons[gActiveBattler].pp[i] = maxPP;
			EmitSetMonData(0, REQUEST_PPMOVE1_BATTLE + i, 0, 1, &gBattleMons[gActiveBattler].pp[i]); //Restore PP, one move at a time
			MarkBufferBankForExecution(gActiveBattler);
			gBattlescriptCurrInstr -= 5;
			return;
		}
	}
}

//Ripped from PokeEmerald
void atk8F_forcerandomswitch(void)
{
	int i;
	s32 battler1PartyId = 0;
	s32 battler2PartyId = 0;
	u8 lastMonId = 0; // + 1
	u8 firstMonId = 0;
	s32 monsCount = 0;
	struct Pokemon* party = NULL;
	s32 validMons = 0;
	s32 minNeeded = 1;

	if (gBattleTypeFlags & (BATTLE_TYPE_TRAINER | BATTLE_TYPE_DOUBLE)) //Trainer Battle or Wild Double Battle
	{
		party = LoadPartyRange(gBankTarget, &firstMonId, &lastMonId);

		if ((gBattleTypeFlags & BATTLE_TYPE_FRONTIER && gBattleTypeFlags & BATTLE_TYPE_LINK)
		||  (gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER)
		||  (gBattleTypeFlags & BATTLE_TYPE_MULTI && gBattleTypeFlags & BATTLE_TYPE_LINK))
		{
			monsCount = 3;
			battler2PartyId = gBattlerPartyIndexes[gBankTarget];
			battler1PartyId = gBattlerPartyIndexes[gBankTarget ^ BIT_FLANK];
		}
		else if (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS)
		{
			if (SIDE(gBankTarget) == B_SIDE_PLAYER)
			{
				if (gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER)
				{
					monsCount = 3;
				}
				else //Player Vs. 2 Trainers
				{
					monsCount = 6;
				}
			}
			else //B_OPPONENT_SIDE - Player Vs. 2 Trainers
			{
				monsCount = 3;
			}
			battler2PartyId = gBattlerPartyIndexes[gBankTarget];
			battler1PartyId = gBattlerPartyIndexes[gBankTarget ^ BIT_FLANK];
		}
		else if (gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER)
		{
			if (SIDE(gBankTarget) == B_SIDE_PLAYER)
			{
				monsCount = 3;
			}
			else
			{
				monsCount = 6;
			}
			battler2PartyId = gBattlerPartyIndexes[gBankTarget];
			battler1PartyId = gBattlerPartyIndexes[gBankTarget ^ BIT_FLANK];
		}
		else if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
		{
			monsCount = 6;
			battler2PartyId = gBattlerPartyIndexes[gBankTarget];
			battler1PartyId = gBattlerPartyIndexes[gBankTarget ^ BIT_FLANK];
		}
		else //Single Battle
		{
			monsCount = 6;
			battler2PartyId = gBattlerPartyIndexes[gBankTarget]; // there is only one pokemon out in single battles
			battler1PartyId = gBattlerPartyIndexes[gBankTarget];
		}

		for (i = firstMonId; i < lastMonId; i++)
		{
			if (GetMonData(&party[i], MON_DATA_SPECIES, 0) != SPECIES_NONE
			&& !GetMonData(&party[i], MON_DATA_IS_EGG, 0)
			&&  GetMonData(&party[i], MON_DATA_HP, 0) != 0
			&& i != battler1PartyId
			&& i != battler2PartyId)
				validMons++;
		}

		if (validMons < minNeeded)
		{
			gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
		}
		else
		{
			if (TryDoForceSwitchOut())
			{
				do
				{
					i = umodsi(Random(), monsCount);
					i += firstMonId;
				}
				while (i == battler1PartyId
					|| i == battler2PartyId
					|| GetMonData(&party[i], MON_DATA_SPECIES, 0) == SPECIES_NONE
					|| GetMonData(&party[i], MON_DATA_IS_EGG, 0) != FALSE
					|| GetMonData(&party[i], MON_DATA_HP, 0) == 0);
			}
			gBattleStruct->monToSwitchIntoId[gBankTarget] = i;

			if (!IsLinkDoubleBattle() && !IsTagBattle())
				sub_8013F6C(gBankTarget);

			if ((gBattleTypeFlags & BATTLE_TYPE_LINK && gBattleTypeFlags & BATTLE_TYPE_FRONTIER)
				|| (gBattleTypeFlags & BATTLE_TYPE_LINK && gBattleTypeFlags & BATTLE_TYPE_MULTI))
			{
				sub_8127EC4(gBankTarget, i, 0);
				sub_8127EC4(gBankTarget ^ BIT_FLANK, i, 1);
			}

			if (IsTagBattle())
				sub_80571DC(gBankTarget, i);
		}
	}
	else //Regular Wild Battle
		TryDoForceSwitchOut();
}

static bool8 TryDoForceSwitchOut(void)
{
	if (IsDynamaxed(gBankTarget)) //Can't force out a Dynamaxed mon
	{
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
		return FALSE;
	}

	if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)
	{
		gBattleStruct->switchoutPartyIndex[gBankTarget] = gBattlerPartyIndexes[gBankTarget];
		gBattlescriptCurrInstr = BattleScript_SuccessForceOut;
		return TRUE;
	}

	//If Wild Battle
	else if (gBattleMons[gBankAttacker].level < gBattleMons[gBankTarget].level)
	{
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
		return FALSE;
	}

	//Roar always fails in Wild Double Battles if used by the player
	else if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE && SIDE(gBankAttacker) == B_SIDE_PLAYER)
	{
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
		return FALSE;
	}

	gBattleStruct->switchoutPartyIndex[gBankTarget] = gBattlerPartyIndexes[gBankTarget];
	gBattlescriptCurrInstr = BattleScript_SuccessForceOut;
	return TRUE;
}

static void sub_80571DC(u8 battlerId, u8 arg1)
{ //0x8013F6C in FR
	if (SIDE(battlerId) != B_SIDE_OPPONENT)
	{
		s32 i;

		// gBattleStruct->field_60[0][i]

		for (i = 0; i < 3; i++)
			gUnknown_0203B0DC[i] = gBattleStruct->field_60[0][i];

		sub_8127FF4(pokemon_order_func(gBattlerPartyIndexes[battlerId]), pokemon_order_func(arg1)); //In Emerald: sub_81B8FB0

		for (i = 0; i < 3; i++)
			gBattleStruct->field_60[0][i] = gUnknown_0203B0DC[i];
	}
}

static bool8 PPIsMaxed(u8 bank)
{
	for (int i = 0; i < 4; ++i)
	{
		if (gBattleMons[bank].moves[i] == MOVE_NONE) break;

		if (gBattleMons[bank].pp[i] < CalculatePPWithBonus(gBattleMons[bank].moves[i], gBattleMons[bank].ppBonuses, i))
			return FALSE;
	}
	return TRUE;
}

bool8 HasNoMonsToSwitch(u8 battler)
{
	pokemon_t* party;
	u8 firstMonId, lastMonId, battlerIn1, battlerIn2;
	int i;

	if (!(gBattleTypeFlags & BATTLE_TYPE_DOUBLE))
		return FALSE;

	party = LoadPartyRange(battler, &firstMonId, &lastMonId);

	battlerIn1 = battler;
	if (gAbsentBattlerFlags & gBitTable[GetBattlerAtPosition(GetBattlerPosition(battler) ^ BIT_FLANK)])
		battlerIn2 = battler;
	else
		battlerIn2 = GetBattlerAtPosition(GetBattlerPosition(battler) ^ BIT_FLANK);

	for (i = firstMonId; i < lastMonId; ++i)
	{
		if (GetMonData(&party[i], MON_DATA_HP, 0) != 0
		&& GetMonData(&party[i], MON_DATA_SPECIES, 0) != SPECIES_NONE
		&& GetMonData(&party[i], MON_DATA_SPECIES2, 0) != SPECIES_EGG
		&& !GetMonData(&party[i], MON_DATA_IS_EGG, 0)
		&& i != gBattlerPartyIndexes[battlerIn1]
		&& i != gBattlerPartyIndexes[battlerIn2])
			break;
	}

	return (i == lastMonId);
}

void ClearSwitchBytes(u8 bank)
{
	gNewBS->TelekinesisTimers[bank] = 0;
	gNewBS->HealBlockTimers[bank] = 0;
	gNewBS->LaserFocusTimers[bank] = 0;
	gNewBS->ThroatChopTimers[bank] = 0;
	gNewBS->StompingTantrumTimers[bank] = 0;
	gNewBS->MetronomeCounter[bank] = 0;
	gNewBS->DestinyBondCounters[bank] = 0;
	gNewBS->SlowStartTimers[bank] = 0;
	gNewBS->IncinerateCounters[bank] = 0;
	gNewBS->EmbargoTimers[bank] = 0;
	gNewBS->DisabledMoldBreakerAbilities[bank] = 0;
	gNewBS->SuppressedAbilities[bank] = 0;
	gNewBS->neutralizingGasBlockedAbilities[bank] = 0;
	gNewBS->lastTargeted[bank] = 0;
	gNewBS->usedMoveIndices[bank] = 0;
	gNewBS->synchronizeTarget[bank] = 0;
	gNewBS->statFellThisTurn[bank] = FALSE;
	gNewBS->dynamaxData.timer[bank] = 0;
	gNewBS->sandblastCentiferno[bank] = 0;
	DestroyMegaIndicator(bank);
	ClearBattlerAbilityHistory(bank);
	ClearBattlerItemEffectHistory(bank);
}

void ClearSwitchBits(u8 bank)
{
	gNewBS->PowderByte &= ~(gBitTable[bank]);
	gNewBS->quashed &= ~(gBitTable[bank]);
	gNewBS->BeakBlastByte &= ~(gBitTable[bank]);
	gNewBS->tarShotBits &= ~(gBitTable[bank]);
	gNewBS->trappedByOctolock &= ~(gBitTable[bank]);
	gNewBS->trappedByNoRetreat &= ~(gBitTable[bank]);
	gNewBS->UnburdenBoosts &= ~(gBitTable[bank]);
	gNewBS->IllusionBroken &= ~(gBitTable[bank]);
	gNewBS->brokeFreeMessage &= ~(gBitTable[bank]);
	gNewBS->CustapQuickClawIndicator &= ~(gBitTable[bank]);
	gNewBS->devolveForgotMove &= ~(gBitTable[bank]);
}

void PartyMenuSwitchingUpdate(void)
{
	int i;

	if (IsOfType(gActiveBattler, TYPE_GHOST)
	||  ITEM_EFFECT(gActiveBattler) == ITEM_EFFECT_SHED_SHELL)
		goto SKIP_SWITCH_BLOCKING_CHECK;

	gBattleStruct->switchoutPartyIndex[gActiveBattler] = gBattlerPartyIndexes[gActiveBattler];
	if ((gBattleMons[gActiveBattler].status2 & (STATUS2_WRAPPED | STATUS2_ESCAPE_PREVENTION))
	|| (gStatuses3[gActiveBattler] & (STATUS3_ROOTED | STATUS3_SKY_DROP_TARGET))
	|| IsFairyLockActive())
	{
		EmitChoosePokemon(0, PARTY_CANT_SWITCH, 6, ABILITY_NONE, gBattleStruct->field_60[gActiveBattler]);
	}
	else if (((i = ABILITY_ON_OPPOSING_FIELD(gActiveBattler, ABILITY_SHADOWTAG)) && ABILITY(gActiveBattler) != ABILITY_SHADOWTAG)
		 ||  ((i = ABILITY_ON_OPPOSING_FIELD(gActiveBattler, ABILITY_ARENATRAP)) && CheckGrounding(gActiveBattler))
		 ||  ((i = AbilityBattleEffects(ABILITYEFFECT_CHECK_FIELD_EXCEPT_BANK, gActiveBattler, ABILITY_MAGNETPULL, 0, 0))
				 && IsOfType(gActiveBattler, TYPE_STEEL)))
	{
		EmitChoosePokemon(0, ((i - 1) << 4) | PARTY_ABILITY_PREVENTS, 6, gLastUsedAbility, gBattleStruct->field_60[gActiveBattler]);
	}
	else
	{
	SKIP_SWITCH_BLOCKING_CHECK:
		if (gActiveBattler == B_POSITION_PLAYER_RIGHT && gActionForBanks[0] == ACTION_SWITCH)
			EmitChoosePokemon(0, PARTY_CHOOSE_MON, gBattleStruct->monToSwitchIntoId[0], ABILITY_NONE, gBattleStruct->field_60[gActiveBattler]);
		else if (gActiveBattler == B_POSITION_OPPONENT_RIGHT && gActionForBanks[1] == ACTION_SWITCH)
			EmitChoosePokemon(0, PARTY_CHOOSE_MON, gBattleStruct->monToSwitchIntoId[1], ABILITY_NONE, gBattleStruct->field_60[gActiveBattler]);
		else
			EmitChoosePokemon(0, PARTY_CHOOSE_MON, 6, ABILITY_NONE, gBattleStruct->field_60[gActiveBattler]);
	}
	MarkBufferBankForExecution(gActiveBattler);
}

u32 CalcSpikesDamage(u8 bank)
{
	if (ITEM_EFFECT(bank) == ITEM_EFFECT_HEAVY_DUTY_BOOTS)
		return 0;

	u32 dmg = (5 - gSideTimers[SIDE(bank)].spikesAmount) * 2;
	return MathMax(1, gBattleMons[bank].maxHP / dmg);
}

u32 CalcSpikesDamagePartyMon(struct Pokemon* mon, u8 side)
{
	if (GetMonItemEffect(mon) == ITEM_EFFECT_HEAVY_DUTY_BOOTS)
		return 0;

	u32 dmg = (5 - gSideTimers[side].spikesAmount) * 2;
	return MathMax(1, GetMonData(mon, MON_DATA_MAX_HP, NULL) / dmg);
}

u32 CalcStealthRockDamage(u8 bank)
{
	u8 flags;
	u8 divisor = 8;
	gBattleMoveDamage = 40;

	if (ITEM_EFFECT(bank) == ITEM_EFFECT_HEAVY_DUTY_BOOTS)
		return 0;

	TypeDamageModification(0, bank, MOVE_STEALTHROCK, TYPE_ROCK, &flags);
	divisor = GetStealthRockDivisor();

	return MathMax(1, gBattleMons[bank].maxHP / divisor);
}

u32 CalcStealthRockDamagePartyMon(struct Pokemon* mon)
{
	u8 flags;
	u8 divisor = 8;
	gBattleMoveDamage = 40;

	if (GetMonItemEffect(mon) == ITEM_EFFECT_HEAVY_DUTY_BOOTS)
		return 0;

	TypeDamageModificationPartyMon(0, mon, MOVE_STEALTHROCK, TYPE_ROCK, &flags);
	divisor = GetStealthRockDivisor();

	return MathMax(1, GetMonData(mon, MON_DATA_MAX_HP, NULL) / divisor);
}

u32 CalcSteelsurgeDamage(u8 bank)
{
	u8 flags;
	u8 divisor = 8;
	gBattleMoveDamage = 40;

	if (ITEM_EFFECT(bank) == ITEM_EFFECT_HEAVY_DUTY_BOOTS)
		return 0;

	TypeDamageModification(0, bank, MOVE_IRONHEAD, TYPE_STEEL, &flags);
	divisor = GetStealthRockDivisor();

	return MathMax(1, gBattleMons[bank].maxHP / divisor);
}

u32 CalcSteelsurgeDamagePartyMon(struct Pokemon* mon)
{
	u8 flags;
	u8 divisor = 8;
	gBattleMoveDamage = 40;

	if (GetMonItemEffect(mon) == ITEM_EFFECT_HEAVY_DUTY_BOOTS)
		return 0;

	TypeDamageModificationPartyMon(0, mon, MOVE_IRONHEAD, TYPE_STEEL, &flags);
	divisor = GetStealthRockDivisor();

	return MathMax(1, GetMonData(mon, MON_DATA_MAX_HP, NULL) / divisor);
}

static u8 GetStealthRockDivisor(void)
{
	u8 divisor = 1;

	switch (gBattleMoveDamage) {
		case 5:
			divisor = 64;
			break;
		case 10:
			divisor = 32;
			break;
		case 20:
			divisor = 16;
			break;
		case 40:
			divisor = 8;
			break;
		case 80:
			divisor = 4;
			break;
		case 160:
			divisor = 2;
			break;
		case 320:
			divisor = 1;
	}

	return divisor;
}

bool8 WillFaintFromEntryHazards(struct Pokemon* mon, u8 side)
{
	u16 hp = GetMonData(mon, MON_DATA_HP, NULL);
	u32 dmg = 0;

	if (gSideAffecting[side] & SIDE_STATUS_SPIKES
	&& GetMonAbility(mon) != ABILITY_MAGICGUARD
	&& ItemId_GetHoldEffect(GetMonData(mon, MON_DATA_HELD_ITEM, NULL)) != ITEM_EFFECT_HEAVY_DUTY_BOOTS)
	{
		if (gSideTimers[side].srAmount > 0)
			dmg += CalcStealthRockDamagePartyMon(mon);

		if (gSideTimers[side].steelsurge > 0)
			dmg += CalcSteelsurgeDamagePartyMon(mon);

		if (gSideTimers[side].spikesAmount > 0)
			dmg += CalcSpikesDamagePartyMon(mon, side);

		if (dmg >= hp)
			return TRUE;
	}

	return FALSE;
}
