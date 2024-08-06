#include "defines.h"
#include "defines_battle.h"
#include "../include/random.h"
#include "../include/party_menu.h"
#include "../include/constants/songs.h"
#include "../include/constants/trainer_classes.h"

#include "../include/new/ability_battle_scripts.h"
#include "../include/new/ability_tables.h"
#include "../include/new/ai_master.h"
#include "../include/new/ai_util.h"
#include "../include/new/battle_indicators.h"
#include "../include/new/battle_script_util.h"
#include "../include/new/battle_start_turn_start.h"
#include "../include/new/battle_start_turn_start_battle_scripts.h"
#include "../include/new/battle_util.h"
#include "../include/new/cmd49_battle_scripts.h"
#include "../include/new/end_battle.h"
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
	SwitchIn_HandleAICooldown,
	SwitchIn_CamomonsReveal,
	SwitchIn_NeutralizingGasRemoveAbility,
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
	SwitchIn_MagnetRiseBattle,
	SwitchIn_PixieBoost,
	SwitchIn_LastPokemonMusic,
	SwitchIn_TrainerMessage,
	SwitchIn_PreEnd,
	SwitchIn_EjectPack,
	SwitchIn_End,
};

//This file's functions:
static bool8 TryRemovePrimalWeather(u8 bank, u8 ability);
static bool8 TryRemoveNeutralizingGas(u8 bank, u8 ability, bool8 leftField);
static bool8 TryRemoveUnnerve(u8 bank);
static bool8 TryActivateFlowerGift(u8 leavingBank);
static bool8 TryDoForceSwitchOut(void);
static void SwitchPartyOrderInGameMulti(u8 bank, u8 monToSwitchIntoId);
static bool8 PPIsMaxed(bank_t);
static u8 GetStealthRockDivisor(void);

void atkE2_switchoutabilities(void)
{
	if (gBattleExecBuffer)
		return;

	gActiveBattler = GetBankForBattleScript(gBattlescriptCurrInstr[1]);

	switch (gNewBS->switchOutAbilitiesState) {
		case 0: //Update current HP if dynamaxed
			if (IsDynamaxed(gActiveBattler))
			{
				gBattleMons[gActiveBattler].hp = GetBaseCurrentHP(gActiveBattler);
				EmitSetMonData(0, REQUEST_HP_BATTLE, 0, 2, &gBattleMons[gActiveBattler].hp);
				MarkBufferBankForExecution(gActiveBattler);
			}
			++gNewBS->switchOutAbilitiesState;
			break;

		case 1: //Update max HP if dynamaxed
			if (IsDynamaxed(gActiveBattler))
			{
				gBattleMons[gActiveBattler].maxHP = GetBaseMaxHP(gActiveBattler);
				EmitSetMonData(0, REQUEST_MAX_HP_BATTLE, 0, 2, &gBattleMons[gActiveBattler].maxHP);
				MarkBufferBankForExecution(gActiveBattler);
				gNewBS->dynamaxData.timer[gActiveBattler] = 0;
			}
			++gNewBS->switchOutAbilitiesState;
			break;

		case 2: //Switch out abilities
			switch (ABILITY(gActiveBattler)) {
				case ABILITY_NATURALCURE:
					gBattleMons[gActiveBattler].status1 = 0;
					EmitSetMonData(0, REQUEST_STATUS_BATTLE, gBitTable[gBattleStruct->switchoutPartyIndex[gActiveBattler]], 4, &gBattleMons[gActiveBattler].status1);
					MarkBufferBankForExecution(gActiveBattler);
					break;
				case ABILITY_REGENERATOR:
					gBattleMons[gActiveBattler].hp = MathMin(gBattleMons[gActiveBattler].maxHP, gBattleMons[gActiveBattler].hp + (gBattleMons[gActiveBattler].maxHP / 3));
					EmitSetMonData(0, REQUEST_HP_BATTLE, gBitTable[gBattleStruct->switchoutPartyIndex[gActiveBattler]], 4, &gBattleMons[gActiveBattler].hp);
					MarkBufferBankForExecution(gActiveBattler);
			}
			++gNewBS->switchOutAbilitiesState;
			break;

		case 3: //Form reversion
			SwitchOutFormsRevert(gActiveBattler);
			++gNewBS->switchOutAbilitiesState;
			break;

		case 4: //Gigantamax reversion
			TryRevertBankGigantamax(gActiveBattler);
			++gNewBS->switchOutAbilitiesState;
			break;

		case 5: //Done
			gNewBS->switchOutAbilitiesState = 0; //Reset for next time
			gBattlescriptCurrInstr += 2;
	}
}

bool8 HandleSpecialSwitchOutAbilities(u8 bank, u8 ability, bool8 leftField)
{
	return TryRemovePrimalWeather(bank, ability)
		|| TryRemoveNeutralizingGas(bank, ability, leftField)
		|| TryRemoveUnnerve(bank)
		|| TryActivateFlowerGift(bank);
}

static bool8 TryRemovePrimalWeather(u8 bank, u8 ability)
{
	int i;
	gBattleStringLoader = NULL;

	switch (ability) {
		case ABILITY_PRIMORDIALSEA:
			if (gBattleWeather & WEATHER_RAIN_PRIMAL
			#ifdef FLAG_PRIMORDIAL_SEA_BATTLE
			&& !FlagGet(FLAG_PRIMORDIAL_SEA_BATTLE) //Should continue to rain even if mon leaves the field
			#endif
			)
				gBattleStringLoader = PrimalRainEndString;
			break;
		case ABILITY_DESOLATELAND:
			if (gBattleWeather & WEATHER_SUN_PRIMAL)
				gBattleStringLoader = PrimalSunEndString;
			break;
		case ABILITY_DELTASTREAM:
			if (gBattleWeather & WEATHER_AIR_CURRENT_PRIMAL
			&& !IsDeltaStreamBattle()) //Should continue to blow even if mon leaves the field
				gBattleStringLoader = PrimalAirCurrentEndString;
			break;
	}

	if (gBattleStringLoader != NULL)
	{
		for (i = 0; i < gBattlersCount; ++i)
		{
			if (i == bank) continue;
			if (BATTLER_ALIVE(i) && ABILITY(i) == ability) break;
		}

		if (i == gBattlersCount)
		{
			gBattleWeather = 0;
			gWishFutureKnock.weatherDuration = 0;
			BattleScriptPushCursor();
			gBattlescriptCurrInstr = BattleScript_PrimalWeatherEnd;
			return TRUE;
		}
	}

	return FALSE;
}

static bool8 TryRemoveNeutralizingGas(u8 bank, u8 ability, bool8 leftField)
{
	if (ability == ABILITY_NEUTRALIZINGGAS)
	{
		if (!gNewBS->printedNeutralizingGasOverMsg)
		{
			BattleScriptPushCursor();
			gBattleStringLoader = gText_NeutralizingGasEnd;
			gBattlescriptCurrInstr = BattleScript_PrintCustomString;
			gNewBS->printedNeutralizingGasOverMsg = TRUE;
			gNewBS->dontActivateMoldBreakersAnymoreThisTurn = TRUE;
			gNewBS->backupBattlerPosition = gBattlerPositions[bank];
			gBattlerPositions[bank] = 0xFF; //So there are no issues with animations like Drought - will still cause problem in Link Battles
			return TRUE;
		}

		if (leftField)
			gBattleMons[bank].hp = 0; //So Switch-In Abilities like Intimidate don't affect the mon that's now gone

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
				if (IsUnnerveAbility(ability) || ability == ABILITY_IMPOSTER) //Never gets another chance
					gStatuses3[bank] |= STATUS3_SWITCH_IN_ABILITY_DONE;
				else
					gStatuses3[bank] &= ~STATUS3_SWITCH_IN_ABILITY_DONE;

				if (AbilityBattleEffects(ABILITYEFFECT_ON_SWITCHIN, bank, 0, 0, 0))
					return TRUE;
			}
		}
	}

	if (gNewBS->printedNeutralizingGasOverMsg)
	{
		gBattlerPositions[bank] = gNewBS->backupBattlerPosition;
		gNewBS->printedNeutralizingGasOverMsg = FALSE; //Reset for next time
	}

	return FALSE;
}

static bool8 TryRemoveUnnerve(u8 bank)
{
	u8 side = SIDE(bank);
	bool8 ret = FALSE;
	u8 ability = ABILITY(bank);

	if (IsUnnerveAbility(ability))
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

		*GetAbilityLocation(bank) = ability; //Restore Unnerve so loop can continue when we return to this function
	}

	return ret;
}

static bool8 TryActivateFlowerGift(u8 leavingBank)
{
	u32 i = 0;

	if (ABILITY(leavingBank) == ABILITY_CLOUDNINE
	#ifdef ABILITY_AIRLOCK
	|| ABILITY(leavingBank) == ABILITY_AIRLOCK
	#endif
	)
		gBattleMons[leavingBank].ability = ABILITY_NONE; //Remove ability because we can't have these anymore

	for (u8 bank = gBanksByTurnOrder[i]; i < gBattlersCount; ++i, bank = gBanksByTurnOrder[i])
	{
		if (bank == leavingBank)
			continue; //Don't do this form change if you're the bank switching out

		if ((ABILITY(bank) == ABILITY_FLOWERGIFT || ABILITY(bank) == ABILITY_FORECAST)) //Just in case someone with Air Lock/Cloud Nine switches out
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

	if (!gNewBS->handlingFaintSwitching)
		RestoreOriginalAttackerAndTarget(); //I'm not sure if this function is even necessary anymore, but I'd rather not remove it and cause bugs
	gNewBS->skipBankStatAnim = gActiveBattler = GetBankForBattleScript(gBattlescriptCurrInstr[1]);

	if (HandleSpecialSwitchOutAbilities(gActiveBattler, ABILITY(gActiveBattler), TRUE))
		return;

	gNewBS->skipBankStatAnim = 0xFF; //No longer needed
	gActiveBattler = GetBankForBattleScript(gBattlescriptCurrInstr[1]);

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

	gActiveBattler = GetBankForBattleScript(gBattlescriptCurrInstr[1]);
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
	gNewBS->AttackerDidDamageAtLeastOnce = FALSE; //Helps with Throat Spray

	//gNewBS->LastUsedMoves[gActiveBattler] = 0;
	//gNewBS->LastUsedTypes[gActiveBattler] = TYPE_BLANK;

	if (gNewBS->corrodedItems[SIDE(gActiveBattler)] & gBitTable[gBattlerPartyIndexes[gActiveBattler]])
		gBattleMons[gActiveBattler].item = 0;

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
	else
	{
		ClearBehindSubstituteBit(gActiveBattler);

		if (AreAbilitiesSuppressed()) //Most likely circus
		{
			gNewBS->SuppressedAbilities[gActiveBattler] = gBattleMons[gActiveBattler].ability;
			gBattleMons[gActiveBattler].ability = 0;
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

	//Transfer new Illusion status
	if (ABILITY(gActiveBattler) == ABILITY_ILLUSION)
	{
		gStatuses3[gActiveBattler] |= STATUS3_ILLUSION;

		if (GetIllusionPartyData(gActiveBattler) == GetBankPartyData(gActiveBattler)) //No one to disguise as
			gStatuses3[gActiveBattler] &= ~STATUS3_ILLUSION;
	}

	EmitDataTransfer(0, &gStatuses3[gActiveBattler], 4, &gStatuses3[gActiveBattler]); //Necessary to overrite old Illusion data
	MarkBufferBankForExecution(gActiveBattler);

	gBattleScripting.bank = gActiveBattler;
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
	gActiveBattler = GetBankForBattleScript(T2_READ_8(gBattlescriptCurrInstr + 1) & ~(ATK4F_DONT_CHECK_STATUSES));

	if (!(T2_READ_8(gBattlescriptCurrInstr + 1) & ATK4F_DONT_CHECK_STATUSES)
	&& CanBeTrapped(gActiveBattler)
	&& ((gBattleMons[gActiveBattler].status2 & (STATUS2_WRAPPED | STATUS2_ESCAPE_PREVENTION)) || (gStatuses3[gActiveBattler] & STATUS3_ROOTED) || IsFairyLockActive()))
	{
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 2);
	}
	else
	{
		if (!HasMonToSwitchTo(gActiveBattler))
			gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 2);
		else
			gBattlescriptCurrInstr += 6;
	}
}

void atk51_switchhandleorder(void)
{
	int i;
	if (gBattleExecBuffer) return;

	gActiveBattler = GetBankForBattleScript(gBattlescriptCurrInstr[1]);

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
				SwitchPartyOrder(gActiveBattler);
			break;

		case 2:
			gBattleCommunication[0] = gBattleBufferB[gActiveBattler][1];
			gBattleStruct->monToSwitchIntoId[gActiveBattler] = gBattleBufferB[gActiveBattler][1];

			if (gBattleTypeFlags & BATTLE_TYPE_LINK && gBattleTypeFlags & BATTLE_TYPE_MULTI)
			{
				gBattleStruct->battlerPartyOrders[gActiveBattler][0] &= 0xF;
				gBattleStruct->battlerPartyOrders[gActiveBattler][0] |= (gBattleBufferB[gActiveBattler][2] & 0xF0);
				gBattleStruct->battlerPartyOrders[gActiveBattler][1] = gBattleBufferB[gActiveBattler][3];

				gBattleStruct->battlerPartyOrders[gActiveBattler ^ BIT_FLANK][0] &= (0xF0);
				gBattleStruct->battlerPartyOrders[gActiveBattler ^ BIT_FLANK][0] |= (gBattleBufferB[gActiveBattler][2] & 0xF0) >> 4;
				gBattleStruct->battlerPartyOrders[gActiveBattler ^ BIT_FLANK][2] = gBattleBufferB[gActiveBattler][3];
			}
			else if (gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER)
				SwitchPartyOrderInGameMulti(gActiveBattler, gBattleStruct->monToSwitchIntoId[gActiveBattler]);
			else
				SwitchPartyOrder(gActiveBattler);

			PREPARE_SPECIES_BUFFER(gBattleTextBuff1, gBattleMons[gBankAttacker].species)
			PREPARE_MON_NICK_BUFFER(gBattleTextBuff2, gActiveBattler, gBattleBufferB[gActiveBattler][1])
			break;
		}
	gBattlescriptCurrInstr += 3;
}

void atk52_switchineffects(void)
{
	if (gBattleExecBuffer)
		return;

	u32 i;
	u8 arg = T2_READ_8(gBattlescriptCurrInstr + 1);
	if (arg == BS_GET_SCRIPTING_BANK)
		gBattleScripting.bank = gNewBS->SentInBackup; //Restore scripting backup b/c can get changed
	
	gActiveBattler = GetBankForBattleScript(arg);
	UpdateSentPokesToOpponentValue(gActiveBattler);
	gHitMarker &= ~(HITMARKER_FAINTED(gActiveBattler));
	gSpecialStatuses[gActiveBattler].flag40 = 0;
	u8 ability = ABILITY(gActiveBattler);

	if (gBattleMons[gActiveBattler].hp == 0)
		goto SWITCH_IN_END;

	if (!(gSideStatuses[SIDE(gActiveBattler)] & SIDE_STATUS_SPIKES) //Skip the entry hazards if there are none
	&& gNewBS->switchInEffectsState >= SwitchIn_Spikes
	&& gNewBS->switchInEffectsState <= SwitchIn_StickyWeb)
		gNewBS->switchInEffectsState = SwitchIn_PrimalReversion;

	switch (gNewBS->switchInEffectsState) {
		case SwitchIn_HandleAICooldown:
			if (SIDE(gActiveBattler) == B_SIDE_PLAYER) //Player switched in a Pokemon
			{
				//If the player switches out their Pokemon, allow the AI to immediately switch out if it wants to
				if (gNewBS->ai.switchingCooldown[GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT)] != 3)
				{
					gNewBS->ai.switchingCooldown[GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT)] = 0;
					gNewBS->ai.typeAbsorbSwitchingCooldown[GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT)] = 0;
				}
				else
					gNewBS->ai.switchingCooldown[GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT)] = 1; //AI just decided not to shift switch

				if (IS_DOUBLE_BATTLE)
				{
					gNewBS->ai.switchingCooldown[GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT)] = 0;
					gNewBS->ai.typeAbsorbSwitchingCooldown[GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT)] = 0;
				}

				if (!(gNewBS->ai.sideSwitchedThisRound & gBitTable[SIDE(FOE(gActiveBattler))])) //There was no change on the other side of the field
				{
					++gNewBS->ai.switchesInARow[gActiveBattler];
					if (ABILITY(gActiveBattler) == ABILITY_INTIMIDATE) //Don't allow Intimidate cheesing
						++gNewBS->ai.switchesInARow[gActiveBattler];
				}
			}
			else //A foe just switched in
			{
				u8 bank = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);
				gNewBS->ai.switchingCooldown[bank] = 0;
				gNewBS->ai.typeAbsorbSwitchingCooldown[bank] = 0;
				gNewBS->ai.switchesInARow[bank] = 0; //Using for helping treat AI abuse
				gNewBS->ai.previousMonIn[bank] = 0xFF;
				gNewBS->ai.secondPreviousMonIn[bank] = 0xFF;
				
				if (IS_DOUBLE_BATTLE)
				{
					bank = GetBattlerAtPosition(B_POSITION_PLAYER_RIGHT);
					gNewBS->ai.switchingCooldown[bank] = 0;
					gNewBS->ai.typeAbsorbSwitchingCooldown[bank] = 0;
					gNewBS->ai.switchesInARow[bank] = 0;
					gNewBS->ai.previousMonIn[bank] = 0xFF;
					gNewBS->ai.secondPreviousMonIn[bank] = 0xFF;
				}

				gNewBS->revealedEnemyMons |= gBitTable[gBattlerPartyIndexes[gActiveBattler]]; //Add to team preview
			}

			gNewBS->ai.pivotTo[FOE(gActiveBattler)] = PARTY_SIZE; //Throw the old pivot out the window if the mon to be pivoted against has changed
			if (IS_DOUBLE_BATTLE)
				gNewBS->ai.pivotTo[PARTNER(FOE(gActiveBattler))] = PARTY_SIZE;
			++gNewBS->switchInEffectsState;
			//Fallthrough

		case SwitchIn_CamomonsReveal:
			if (gBattleTypeFlags & BATTLE_TYPE_CAMOMONS)
			{
				gBattleScripting.bank = gActiveBattler;
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_CamomonsTypeRevealRet;

				u8 type1, type2;
				struct Pokemon* monIllusion = GetIllusionPartyData(gActiveBattler);
				if (monIllusion != GetBankPartyData(gActiveBattler)) //Under Illusion
				{
					type1 = GetMonType(monIllusion, 0);
					type2 = GetMonType(monIllusion, 1);
				}
				else
				{
					type1 = gBattleMons[gActiveBattler].type1;
					type2 = gBattleMons[gActiveBattler].type2;
				}

				if (type1 == type2)
					gBattleStringLoader = gText_CamomonsTypeReveal;
				else
					gBattleStringLoader = gText_CamomonsTypeRevealDualType;

				PREPARE_TYPE_BUFFER(gBattleTextBuff1, type1);
				PREPARE_TYPE_BUFFER(gBattleTextBuff2, type2);
			}
			++gNewBS->switchInEffectsState;
			break;

		case SwitchIn_NeutralizingGasRemoveAbility:
			if (!IsAbilitySuppressed(gActiveBattler) //Gastro Acid has higher priority
			&& ABILITY(gActiveBattler) != ABILITY_NONE
			&& !gSpecialAbilityFlags[ABILITY(gActiveBattler)].gNeutralizingGasBannedAbilities
			&& AbilityBattleEffects(ABILITYEFFECT_CHECK_FIELD_EXCEPT_BANK, gActiveBattler, ABILITY_NEUTRALIZINGGAS, 0, 0))
			{
				u8* abilityLoc = GetAbilityLocation(gActiveBattler);
				gNewBS->neutralizingGasBlockedAbilities[gActiveBattler] = *abilityLoc;
				*abilityLoc = 0;
				gNewBS->SlowStartTimers[gActiveBattler] = 0;
			}
			++gNewBS->switchInEffectsState;
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
					MarkBufferBankForExecution(gActiveBattler);
					gBattleScripting.bank = gActiveBattler;
					gBankAttacker = gActiveBattler;
					++gNewBS->switchInEffectsState;
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
				MarkBufferBankForExecution(gActiveBattler);

				//PP Restored in Battle Script

				gBattleScripting.bank = gActiveBattler;
				gBankAttacker = gActiveBattler;
				++gNewBS->switchInEffectsState;
				return;
			}
			++gNewBS->switchInEffectsState;
		__attribute__ ((fallthrough));

		case SwitchIn_ZHealingWish:
			if (gBattleMons[gActiveBattler].hp != gBattleMons[gActiveBattler].maxHP && gNewBS->zMoveData.healReplacement)
			{
				gNewBS->zMoveData.healReplacement = FALSE;
				gBattleMoveDamage = -1 * (gBattleMons[gActiveBattler].maxHP);
				gBattleScripting.bank = gActiveBattler;
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_HealReplacementZMove;
				return;
			}
			else
				gNewBS->zMoveData.healReplacement = FALSE;

			gNewBS->DamageTaken[gActiveBattler] = 0;
			++gNewBS->switchInEffectsState;
		__attribute__ ((fallthrough));

		case SwitchIn_Spikes:
			if (CheckGrounding(gActiveBattler)
			&& gSideTimers[SIDE(gActiveBattler)].spikesAmount > 0
			&& ability != ABILITY_MAGICGUARD
			&& IsAffectedByHazards(gActiveBattler))
			{
				gBattleMoveDamage = CalcSpikesDamage(gActiveBattler);
				gNewBS->DamageTaken[gActiveBattler] += gBattleMoveDamage;

				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_SpikesHurt;
				gSideStatuses[SIDE(gActiveBattler)] |= SIDE_STATUS_SPIKES_DAMAGED;
				gBattleScripting.bank = gActiveBattler;
				gBankTarget = gActiveBattler;
				//gBankAttacker = FOE(gActiveBattler); //For EXP
				++gNewBS->switchInEffectsState;
				return;
			}
			++gNewBS->switchInEffectsState;
		__attribute__ ((fallthrough));

		case SwitchIn_StealthRock:
			if (gSideTimers[SIDE(gActiveBattler)].srAmount > 0
			&& ability != ABILITY_MAGICGUARD
			&& IsAffectedByHazards(gActiveBattler))
			{
				gBattleMoveDamage = CalcStealthRockDamage(gActiveBattler);
				gNewBS->DamageTaken[gActiveBattler] += gBattleMoveDamage;

				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_SRHurt;
				gSideStatuses[SIDE(gActiveBattler)] |= SIDE_STATUS_SPIKES_DAMAGED;
				gBattleScripting.bank = gActiveBattler;
				gBankTarget = gActiveBattler;
				//gBankAttacker = FOE(gActiveBattler); //For EXP
				++gNewBS->switchInEffectsState;
				return;
			}
			++gNewBS->switchInEffectsState;
		__attribute__ ((fallthrough));

		case SwitchIn_Steelsurge:
			if (gSideTimers[SIDE(gActiveBattler)].steelsurge > 0
			&& ability != ABILITY_MAGICGUARD
			&& IsAffectedByHazards(gActiveBattler))
			{
				gBattleMoveDamage = CalcSteelsurgeDamage(gActiveBattler);
				gNewBS->DamageTaken[gActiveBattler] += gBattleMoveDamage;

				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_SteelsurgeHurt;
				gSideStatuses[SIDE(gActiveBattler)] |= SIDE_STATUS_SPIKES_DAMAGED;
				gBankTarget = gBattleScripting.bank = gActiveBattler;
				//gBankAttacker = FOE(gActiveBattler); //For EXP
				++gNewBS->switchInEffectsState;
				return;
			}
			++gNewBS->switchInEffectsState;
		__attribute__ ((fallthrough));

		case SwitchIn_ToxicSpikes:
			if (gSideTimers[SIDE(gActiveBattler)].tspikesAmount > 0
			&& CheckGrounding(gActiveBattler))
			{
				if (IsOfType(gActiveBattler, TYPE_POISON))
				{
					gSideTimers[SIDE(gActiveBattler)].tspikesAmount = 0;
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_TSAbsorb;
				}
				else if (IsAffectedByHazards(gActiveBattler) //Pokemon with this item can still remove T-Spikes
				&& CanBePoisoned(gActiveBattler, 0xFF, TRUE) //Must include check here, otherwise Corrosion can activate (which it shouldn't)
				&& !BankSideHasSafeguard(gActiveBattler))
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

					gSideStatuses[SIDE(gActiveBattler)] |= SIDE_STATUS_SPIKES_DAMAGED;
				}

				gBattleScripting.bank = gActiveBattler;
				gBankTarget = gActiveBattler;
				//gBankAttacker = FOE(gActiveBattler); //For EXP
				++gNewBS->switchInEffectsState;
				return;
			}
			++gNewBS->switchInEffectsState;
		__attribute__ ((fallthrough));

		case SwitchIn_StickyWeb:
			if (gSideTimers[SIDE(gActiveBattler)].stickyWeb
			&&  CheckGrounding(gActiveBattler)
			&& IsAffectedByHazards(gActiveBattler))
			{
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_StickyWebSpeedDrop;
				gSideStatuses[SIDE(gActiveBattler)] |= SIDE_STATUS_SPIKES_DAMAGED;
				gBattleScripting.bank = gActiveBattler;
				gBankTarget = gActiveBattler;
				//gBankAttacker = FOE(gActiveBattler); //For EXP
				++gNewBS->switchInEffectsState;
				return;
			}
			++gNewBS->switchInEffectsState;
		__attribute__ ((fallthrough));

		case SwitchIn_EmergencyExit:
			if (ABILITY(gActiveBattler) == ABILITY_EMERGENCYEXIT
			/*||  ABILITY(gActiveBattler) == ABILITY_WIMPOUT*/)
			{
				if (gBattleMons[gActiveBattler].hp > 0
				&&  gBattleMons[gActiveBattler].hp <= gBattleMons[gActiveBattler].maxHP / 2
				&&  gBattleMons[gActiveBattler].hp + gNewBS->DamageTaken[gActiveBattler] > gBattleMons[gActiveBattler].maxHP / 2)
				{
					gBattleScripting.bank = gBankSwitching = gActiveBattler;
					BattleScriptPush(gBattlescriptCurrInstr + 2);
					gBattlescriptCurrInstr = BattleScript_EmergencyExit;
					return;
				}
			}
			++gNewBS->switchInEffectsState;
		__attribute__ ((fallthrough));

		case SwitchIn_PrimalReversion:	;
			const u8* script = DoPrimalReversion(gActiveBattler, 1);
			if (!IsMegaZMoveBannedBattle() && script != NULL)
			{
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = script;
				gBattleScripting.bank = gActiveBattler;
				gBankAttacker = gActiveBattler;
				++gNewBS->switchInEffectsState;
				return;
			}
			++gNewBS->switchInEffectsState;
		__attribute__ ((fallthrough));

		case SwitchIn_Truant:
			if (ABILITY(gActiveBattler) == ABILITY_TRUANT)
				gDisableStructs[gActiveBattler].truantCounter ^= 1;
			++gNewBS->switchInEffectsState;
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

			++gNewBS->switchInEffectsState;
		__attribute__ ((fallthrough));

		case SwitchIn_Items:
			if (ItemBattleEffects(ItemEffects_SwitchIn, gActiveBattler, TRUE, FALSE))
				return;

			if (ItemBattleEffects(ItemEffects_EndTurn, gActiveBattler, TRUE, FALSE))
				return;
			++gNewBS->switchInEffectsState;
		__attribute__ ((fallthrough));

		case SwitchIn_AirBalloon:
			if (gActiveBattler < gBattlersCount && ITEM_EFFECT(gActiveBattler) == ITEM_EFFECT_AIR_BALLOON)
			{
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_AirBalloonSub;
				gBattleScripting.bank = gActiveBattler;
				gBankAttacker = gActiveBattler;
				++gNewBS->switchInEffectsState;
				return;
			}
			++gNewBS->switchInEffectsState;
		__attribute__ ((fallthrough));

		case SwitchIn_TotemPokemon: ;
			u8 totemBoostType = CanActivateTotemBoost(gActiveBattler);

			if (totemBoostType == TOTEM_SINGLE_BOOST)
			{
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_TotemRet;
				gBankAttacker = gBattleScripting.bank = gActiveBattler;
				gBattleScripting.statAnimPlayed = FALSE;
				++gNewBS->switchInEffectsState;
				return;
			}
			else if (totemBoostType == TOTEM_MULTI_BOOST)
			{
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_TotemMultiBoostRet;
				gBankAttacker = gBattleScripting.bank = gActiveBattler;
				gBattleScripting.statAnimPlayed = FALSE;
				++gNewBS->switchInEffectsState;
				return;
			}
			else if (totemBoostType == TOTEM_OMNIBOOST)
			{
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_TotemOmniboostRet;
				gBankAttacker = gBattleScripting.bank = gActiveBattler;
				gBattleScripting.statAnimPlayed = FALSE;
				GiveOmniboost(gBankAttacker);
				++gNewBS->switchInEffectsState;
				return;
			}
			++gNewBS->switchInEffectsState;
		__attribute__ ((fallthrough));

		case SwitchIn_MagnetRiseBattle:
			if (IsFloatingWithMagnetism(gActiveBattler)
			&& CheckGrounding(gActiveBattler) == IN_AIR)
			{
				BattleScriptPushCursor();
				gBattleStringLoader = gText_MagnetRiseBattleStart;
				gBattlescriptCurrInstr = BattleScript_PrintCustomString;
				gBankAttacker = gBattleScripting.bank = gActiveBattler;
				++gNewBS->switchInEffectsState;
				return;
			}
			++gNewBS->switchInEffectsState;
		__attribute__ ((fallthrough));

		case SwitchIn_PixieBoost:
			if (IsPixieBattle())
			{
				if (IsAffectedByPixies(gActiveBattler))
				{
					if (gBattleMons[gActiveBattler].defense < gBattleMons[gActiveBattler].spDefense)
						gBattleScripting.statChanger = STAT_STAGE_DEF | INCREASE_1;
					else
						gBattleScripting.statChanger = STAT_STAGE_SPDEF | INCREASE_1;

					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_PixieBoostRet;
					gBankAttacker = gBattleScripting.bank = gActiveBattler;
					++gNewBS->switchInEffectsState;
					return;
				}
			}
			++gNewBS->switchInEffectsState;
		__attribute__ ((fallthrough));

		case SwitchIn_LastPokemonMusic:
			++gNewBS->switchInEffectsState;
			#ifdef BGM_BATTLE_GYM_LEADER_LAST_POKEMON
			if (gBattleTypeFlags & BATTLE_TYPE_TRAINER
			&& !(gBattleTypeFlags & (BATTLE_TYPE_DOUBLE | BATTLE_TYPE_LINK | BATTLE_TYPE_FRONTIER | BATTLE_TYPE_TRAINER_TOWER))
			&& gTrainers[gTrainerBattleOpponent_A].trainerClass == CLASS_LEADER
			&& SIDE(gActiveBattler) == B_SIDE_OPPONENT //So player accepting offer to switching out doesn't trigger this yet
			&& ViableMonCount(gEnemyParty) <= 1)
			{
				PlayBGM(BGM_BATTLE_GYM_LEADER_LAST_POKEMON);
			}
			#endif
		__attribute__ ((fallthrough));

		case SwitchIn_TrainerMessage:
			++gNewBS->switchInEffectsState;
			if (ShouldDoTrainerSlide(gActiveBattler, gTrainerBattleOpponent_A, TRAINER_SLIDE_LAST_SWITCHIN))
			{
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_TrainerSlideMsgRet;
				return;
			}
		__attribute__ ((fallthrough));

		case SwitchIn_PreEnd:
		SWITCH_IN_END:
			gSideStatuses[SIDE(gActiveBattler)] &= ~(SIDE_STATUS_SPIKES_DAMAGED);

			for (i = 0; i < gBattlersCount; ++i)
			{
				if (gBanksByTurnOrder[i] == gActiveBattler)
					gActionsByTurnOrder[i] = ACTION_FINISHED;
			}

			for (i = 0; i < gBattlersCount; i++)
				gBattleStruct->hpOnSwitchout[SIDE(i)] = gBattleMons[i].hp;

			if (T2_READ_8(gBattlescriptCurrInstr + 1) == 5)
			{
				u32 hitmarkerFaintBits = gHitMarker >> 0x1C;
				++gBankFainted;
				while (1)
				{
					if (hitmarkerFaintBits & gBitTable[gBankFainted] && !(gAbsentBattlerFlags & gBitTable[gBankFainted]))
						break;
					if (gBankFainted >= gBattlersCount)
						break;
					++gBankFainted;
				}
			}
			++gNewBS->switchInEffectsState;
		__attribute__ ((fallthrough));

		case SwitchIn_EjectPack:
			gNewBS->switchInEffectsState = 0;
			gBattlescriptCurrInstr += 2;

			for (i = 0; i < gBattlersCount; ++i)
			{
				if ((ITEM_EFFECT(i) == ITEM_EFFECT_EJECT_PACK || ITEM_EFFECT(i) == ITEM_EFFECT_RESTORE_STATS)
				&& ItemBattleEffects(ItemEffects_SwitchIn, i, TRUE, FALSE))  //Try to trigger White Herbs or Eject Packs after Intimidate
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
	gActiveBattler = gBattleScripting.bank;

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
	u8 bankDef, firstMonId, lastMonId;
	u8 battler1PartyId = 0;
	u8 battler2PartyId = 0;
	u8 monsCount = 0;
	u8 validMons = 0;
	u8 minNeeded = 1;
	struct Pokemon* party = NULL;
	
	bankDef = GetBankForBattleScript(gBattlescriptCurrInstr[1]);

	if (gBattleTypeFlags & (BATTLE_TYPE_TRAINER | BATTLE_TYPE_DOUBLE)) //Trainer Battle or Wild Double Battle
	{
		party = LoadPartyRange(bankDef, &firstMonId, &lastMonId);

		if ((gBattleTypeFlags & BATTLE_TYPE_LINK && gBattleTypeFlags & BATTLE_TYPE_MULTI)
		||  (gBattleTypeFlags & BATTLE_TYPE_LINK && gBattleTypeFlags & BATTLE_TYPE_FRONTIER && SIDE(bankDef) == B_SIDE_PLAYER)
		||  (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS && SIDE(bankDef) == B_SIDE_OPPONENT)
		||  (gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER && SIDE(bankDef) == B_SIDE_PLAYER))
		{
			monsCount = 3;
			battler1PartyId = gBattlerPartyIndexes[bankDef];
			battler2PartyId = gBattlerPartyIndexes[PARTNER(bankDef)];
		}
		else if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
		{
			monsCount = 6;
			battler1PartyId = gBattlerPartyIndexes[bankDef];
			battler2PartyId = gBattlerPartyIndexes[PARTNER(bankDef)];
		}
		else //Single Battle
		{
			monsCount = 6;
			battler1PartyId = gBattlerPartyIndexes[bankDef]; // there is only one pokemon out in single battles
			battler2PartyId = gBattlerPartyIndexes[bankDef];
		}

		for (i = firstMonId; i < lastMonId; ++i)
		{
			if (i != battler1PartyId
			&&  i != battler2PartyId
			&& MON_CAN_BATTLE(&party[i]))
				validMons++;
		}

		if (validMons < minNeeded)
		{
			gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 3);
		}
		else
		{
			if (TryDoForceSwitchOut())
			{
				do
				{
					i = Random() % monsCount;
					i += firstMonId;
				}
				while (i == battler1PartyId
					|| i == battler2PartyId
					|| !MON_CAN_BATTLE(&party[i]));

				gBattleStruct->monToSwitchIntoId[bankDef] = i;

				if (!IsLinkDoubleBattle() && !IsTagBattle())
					SwitchPartyOrder(bankDef);
				else if (gBattleTypeFlags & BATTLE_TYPE_LINK && gBattleTypeFlags & (BATTLE_TYPE_MULTI | BATTLE_TYPE_FRONTIER))
				{
					SwitchPartyOrderLinkMulti(bankDef, i, 0);
					SwitchPartyOrderLinkMulti(PARTNER(bankDef), i, 1);
				}
				else if (IsTagBattle())
					SwitchPartyOrderInGameMulti(bankDef, i);
			}
		}
	}
	else //Regular Wild Battle
		TryDoForceSwitchOut();
}

static bool8 TryDoForceSwitchOut(void)
{
	u8 bankDef = GetBankForBattleScript(gBattlescriptCurrInstr[1]);
	u8 bankAtk = GetBankForBattleScript(gBattlescriptCurrInstr[2]);

	if (IsDynamaxed(bankDef)) //Can't force out a Dynamaxed mon
	{
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 3);
		return FALSE;
	}

	if (!(gBattleTypeFlags & BATTLE_TYPE_TRAINER)) //Wild
	{
		if (IS_DOUBLE_BATTLE)
		{
			if (SIDE(bankDef) == B_SIDE_OPPONENT)
			{
				//Roar always fails in Wild Double Battles if used on a wild mon
				gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 3);
				return FALSE;
			}
		}
		else //Single Battle
		{
			if (AreAllKindsOfRunningPrevented() || IsRaidBattle())
			{
				//Roar always fails if running is impossible
				gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 3);
				return FALSE;
			}
			else if (gBattleMons[bankAtk].level < gBattleMons[bankDef].level)
			{
				//Pokemon using Roar must be of higher level to end the battle in a Single battle
				gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 3);
				return FALSE;
			}
		}
	}

	gBankSwitching = bankDef;
	gBattleStruct->switchoutPartyIndex[bankDef] = gBattlerPartyIndexes[bankDef];
	gBattlescriptCurrInstr = BattleScript_SuccessForceOut;
	return TRUE;
}

static void SwitchPartyOrderInGameMulti(u8 bank, u8 monToSwitchIntoId)
{
	if (SIDE(bank) != B_SIDE_OPPONENT)
	{
		s32 i;

		for (i = 0; i < PARTY_SIZE / 2; i++)
			gBattlePartyCurrentOrder[i] = gBattleStruct->battlerPartyOrders[0][i];

		SwitchPartyMonSlots(GetBattlePartyIdFromPartyId(gBattlerPartyIndexes[bank]), GetBattlePartyIdFromPartyId(monToSwitchIntoId));

		for (i = 0; i < PARTY_SIZE / 2; i++)
			gBattleStruct->battlerPartyOrders[0][i] = gBattlePartyCurrentOrder[i];
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
	gNewBS->GlaiveRushTimers[bank] = 0;
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
	gNewBS->zMoveData.toBeUsed[bank] = 0; //Force switch or fainted before Z-Move could be used
	gNewBS->chiStrikeCritBoosts[bank] = 0;
	gNewBS->sandblastCentiferno[bank] = 0;
	gNewBS->SaltcureTimers[bank] = 0;
	gNewBS->disguisedAs[bank] = 0;
	gNewBS->powerShifted[bank] = 0;
	gNewBS->splinterTimer[bank] = 0;
	gNewBS->splinterAttackerBank[bank] = 0;
	gNewBS->splinterAttackerMonId[bank] = 0;
	gNewBS->splinterMove[bank] = 0;
	gNewBS->tookAbilityFrom[bank] = 0;

	gProtectStructs[bank].KingsShield = 0;	//Necessary because could be sent away with Roar
	gProtectStructs[bank].SpikyShield = 0;
	gProtectStructs[bank].BanefulBunker = 0;
	gProtectStructs[bank].obstruct = 0;
	gProtectStructs[bank].enduredSturdy = 0;
	gProtectStructs[bank].BurningBulwark = 0;
	
	DestroyMegaIndicator(bank);
	WipeOldDeperateAttemptRecord(bank);
	ClearBattlerAbilityHistory(bank);
	ClearBattlerItemEffectHistory(bank);
	ClearBattlerMoveHistory(bank);
	ClearMovePredictionsOnBank(bank);
}

void ClearSwitchBits(u8 bank)
{
	gNewBS->PowderByte &= ~(gBitTable[bank]);
	gNewBS->turnOrderLocked &= ~(gBitTable[bank]);
	gNewBS->BeakBlastByte &= ~(gBitTable[bank]);
	gNewBS->tarShotBits &= ~(gBitTable[bank]);
	gNewBS->trappedByOctolock &= ~(gBitTable[bank]);
	gNewBS->trappedByNoRetreat &= ~(gBitTable[bank]);
	gNewBS->UnburdenBoosts &= ~(gBitTable[bank]);
	gNewBS->IllusionBroken &= ~(gBitTable[bank]);
	gNewBS->brokeFreeMessage &= ~(gBitTable[bank]);
	gNewBS->ateCustapBerry &= ~(gBitTable[bank]);
	gNewBS->quickClawCustapIndicator &= ~(gBitTable[bank]);
	gNewBS->devolveForgotMove &= ~(gBitTable[bank]);
}

void PartyMenuSwitchingUpdate(void)
{
	int i;

	gBattleStruct->switchoutPartyIndex[gActiveBattler] = gBattlerPartyIndexes[gActiveBattler];

	if (gStatuses3[gActiveBattler] & STATUS3_SKY_DROP_TARGET) //Being Ghost doesn't get you out of this
		goto TRAPPED;
	else if (!CanBeTrapped(gActiveBattler))
		goto SKIP_SWITCH_BLOCKING_CHECK;
	else if ((gBattleMons[gActiveBattler].status2 & (STATUS2_WRAPPED | STATUS2_ESCAPE_PREVENTION))
	|| (gStatuses3[gActiveBattler] & STATUS3_ROOTED)
	|| IsFairyLockActive())
	{
		TRAPPED:
		EmitChoosePokemon(0, PARTY_CANT_SWITCH, PARTY_SIZE, ABILITY_NONE, gBattleStruct->battlerPartyOrders[gActiveBattler]);
	}
	else if (((i = ABILITY_ON_OPPOSING_FIELD(gActiveBattler, ABILITY_SHADOWTAG)) && IsTrappedByAbility(gActiveBattler, ABILITY_SHADOWTAG))
		 ||  ((i = ABILITY_ON_OPPOSING_FIELD(gActiveBattler, ABILITY_ARENATRAP)) && IsTrappedByAbility(gActiveBattler, ABILITY_ARENATRAP))
		 ||  ((i = ABILITY_ON_OPPOSING_FIELD(gActiveBattler, ABILITY_MAGNETPULL)) && IsTrappedByAbility(gActiveBattler, ABILITY_MAGNETPULL)))
	{
		EmitChoosePokemon(0, ((i - 1) << 4) | PARTY_ABILITY_PREVENTS, 6, gLastUsedAbility, gBattleStruct->battlerPartyOrders[gActiveBattler]);
	}
	else
	{
		SKIP_SWITCH_BLOCKING_CHECK:
		if (GetBattlerPosition(gActiveBattler) == B_POSITION_PLAYER_RIGHT
		&& gChosenActionByBank[GetBattlerAtPosition(B_POSITION_PLAYER_LEFT)] == ACTION_SWITCH)
			EmitChoosePokemon(0, PARTY_CHOOSE_MON, gBattleStruct->monToSwitchIntoId[0], ABILITY_NONE, gBattleStruct->battlerPartyOrders[gActiveBattler]);
		else if (GetBattlerPosition(gActiveBattler) == B_POSITION_OPPONENT_RIGHT
		&& gChosenActionByBank[GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT)] == ACTION_SWITCH)
			EmitChoosePokemon(0, PARTY_CHOOSE_MON, gBattleStruct->monToSwitchIntoId[1], ABILITY_NONE, gBattleStruct->battlerPartyOrders[gActiveBattler]);
		else
			EmitChoosePokemon(0, PARTY_CHOOSE_MON, PARTY_SIZE, ABILITY_NONE, gBattleStruct->battlerPartyOrders[gActiveBattler]);
	}

	MarkBufferBankForExecution(gActiveBattler);
}

u32 CalcSpikesDamage(u8 bank)
{
	if (!IsAffectedByHazards(bank))
		return 0;

	u32 dmg = (5 - gSideTimers[SIDE(bank)].spikesAmount) * 2;
	return MathMax(1, gBattleMons[bank].maxHP / dmg);
}

u32 CalcSpikesDamagePartyMon(struct Pokemon* mon, u8 side)
{
	if (!IsMonAffectedByHazards(mon))
		return 0;

	u32 dmg = (5 - gSideTimers[side].spikesAmount) * 2;
	return MathMax(1, GetMonData(mon, MON_DATA_MAX_HP, NULL) / dmg);
}

u32 CalcStealthRockDamage(u8 bank)
{
	u8 flags;
	u8 divisor = 8;
	gBattleMoveDamage = 40;

	TypeDamageModification(0, bank, MOVE_STEALTHROCK, TYPE_ROCK, &flags);
	divisor = GetStealthRockDivisor();

	return MathMax(1, gBattleMons[bank].maxHP / divisor);
}

u32 GetStealthRockDamage(u8 bank)
{
	if (!IsAffectedByHazards(bank))
		return 0;

	return CalcStealthRockDamage(bank);
}

u32 CalcMonStealthRockDamage(struct Pokemon* mon)
{
	u8 flags;
	u8 divisor = 8;
	gBattleMoveDamage = 40;

	TypeDamageModificationPartyMon(0, mon, MOVE_STEALTHROCK, TYPE_ROCK, &flags);
	divisor = GetStealthRockDivisor();

	return MathMax(1, GetMonData(mon, MON_DATA_MAX_HP, NULL) / divisor);
}

u32 GetStealthRockDamagePartyMon(struct Pokemon* mon)
{
	if (!IsMonAffectedByHazards(mon))
		return 0;

	return CalcMonStealthRockDamage(mon);
}

u32 CalcSteelsurgeDamage(u8 bank)
{
	u8 flags;
	u8 divisor = 8;
	gBattleMoveDamage = 40;

	if (!IsAffectedByHazards(bank))
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

	if (!IsMonAffectedByHazards(mon))
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

u32 GetMonEntryHazardDamage(struct Pokemon* mon, u8 side)
{
	u32 dmg = 0;

	if (gSideStatuses[side] & SIDE_STATUS_SPIKES
	&& GetMonAbility(mon) != ABILITY_MAGICGUARD
	&& IsMonAffectedByHazards(mon)) //Has Klutz or not holding boots
	{
		if (gSideTimers[side].srAmount > 0)
			dmg += CalcMonStealthRockDamage(mon);

		if (gSideTimers[side].steelsurge > 0)
			dmg += CalcSteelsurgeDamagePartyMon(mon);

		if (gSideTimers[side].spikesAmount > 0)
			dmg += CalcSpikesDamagePartyMon(mon, side);
	}

	return dmg;
}

bool8 WillFaintFromEntryHazards(struct Pokemon* mon, u8 side)
{
	return GetMonEntryHazardDamage(mon, side) >= mon->hp;
}
