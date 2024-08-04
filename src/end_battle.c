#include "defines.h"
#include "defines_battle.h"
#include "../include/event_data.h"
#include "../include/field_weather.h"
#include "../include/item_menu.h"
#include "../include/item_use.h"
#include "../include/load_save.h"
#include "../include/malloc.h"
#include "../include/random.h"
#include "../include/constants/maps.h"
#include "../include/constants/songs.h"
#include "../include/constants/trainer_classes.h"

#include "../include/new/battle_start_turn_start.h"
#include "../include/new/battle_util.h"
#include "../include/new/catching.h"
#include "../include/new/dynamax.h"
#include "../include/new/end_battle.h"
#include "../include/new/end_battle_battle_scripts.h"
#include "../include/new/evolution.h"
#include "../include/new/form_change.h"
#include "../include/new/frontier.h"
#include "../include/new/util.h"
#include "../include/new/mega.h"
#include "../include/new/multi.h"

/*
end_battle.c
	handles all battle termination logic and data resetting/saving
*/

const u16 gEndBattleFlagClearTable[] =
{
#ifdef FLAG_CATCH_TRAINERS_POKEMON
	FLAG_CATCH_TRAINERS_POKEMON,
#endif
#ifdef FLAG_ACTIVATE_TUTORIAL
	FLAG_ACTIVATE_TUTORIAL,
#endif
#ifdef FLAG_INVERSE
	FLAG_INVERSE,
#endif
#ifdef FLAG_SKY_BATTLE
	FLAG_SKY_BATTLE,
#endif
#ifdef FLAG_NO_CATCHING
	FLAG_NO_CATCHING,
#endif
#ifdef FLAG_NO_RUNNING
	FLAG_NO_RUNNING,
#endif
#ifdef FLAG_NO_CATCHING_AND_RUNNING
	FLAG_NO_CATCHING_AND_RUNNING,
#endif
#ifdef FLAG_ALWAYS_CATCHABLE
	FLAG_ALWAYS_CATCHABLE,
#endif
#ifdef FLAG_WILD_CUSTOM_MOVES
	FLAG_WILD_CUSTOM_MOVES,
#endif
#ifdef FLAG_SMART_WILD
	FLAG_SMART_WILD,
#endif
#ifdef FLAG_SCALE_WILD_BOSS_LEVEL
	FLAG_SCALE_WILD_BOSS_LEVEL,
#endif
#ifdef FLAG_DOUBLE_WILD_BATTLE
	FLAG_DOUBLE_WILD_BATTLE,
#endif
#ifdef FLAG_SHINY_CREATION
	FLAG_SHINY_CREATION,
#endif
#ifdef FLAG_CAMOMONS_BATTLE
	FLAG_CAMOMONS_BATTLE,
#endif
#ifdef FLAG_RING_CHALLENGE_BATTLE
	FLAG_RING_CHALLENGE_BATTLE,
#endif
#ifdef FLAG_PRESET_FRONTIER_OPPONENT_TEAM
	FLAG_PRESET_FRONTIER_OPPONENT_TEAM,
#endif
#ifdef FLAG_PRESET_RANDOM_TEAM
	FLAG_PRESET_RANDOM_TEAM,
#endif
#ifdef FLAG_TWO_WILD_POKEMON_ATTACKED_STRING
	FLAG_TWO_WILD_POKEMON_ATTACKED_STRING,
#endif
#ifdef FLAG_WORST_NIGHTMARE_STRING
	FLAG_WORST_NIGHTMARE_STRING,
#endif
#ifdef FLAG_LEGENDARY_APPEARED_STRING
	FLAG_LEGENDARY_APPEARED_STRING,
#endif
#ifdef FLAG_SINGLE_TRAINER_MON_TOTEM_BOOST
	FLAG_SINGLE_TRAINER_MON_TOTEM_BOOST,
#endif
#ifdef FLAG_BAD_THOUGHTS_BATTLE
	FLAG_BAD_THOUGHTS_BATTLE,
#endif
#ifdef FLAG_TAILWIND_BATTLE
	FLAG_TAILWIND_BATTLE,
#endif
#ifdef FLAG_DELTA_STREAM_BATTLE
	FLAG_DELTA_STREAM_BATTLE,
#endif
#ifdef FLAG_MAGNET_RISE_BATTLE
	FLAG_MAGNET_RISE_BATTLE,
#endif
#ifdef FLAG_TRICK_ROOM_BATTLE
	FLAG_TRICK_ROOM_BATTLE,
#endif
#ifdef FLAG_WEIGHT_SPEED_BATTLE
	FLAG_WEIGHT_SPEED_BATTLE,
#endif
#ifdef FLAG_PRIMORDIAL_SEA_BATTLE
	FLAG_PRIMORDIAL_SEA_BATTLE,
#endif
#ifdef FLAG_VICIOUS_SANDSTORM_BATTLE
	FLAG_VICIOUS_SANDSTORM_BATTLE,
#endif
#ifdef FLAG_SHADOW_SHIELD_BATTLE
	FLAG_SHADOW_SHIELD_BATTLE,
#endif
#ifdef FLAG_PIXIE_BATTLE
	FLAG_PIXIE_BATTLE,
#endif
#ifdef FLAG_RAINBOW_BATTLE
	FLAG_RAINBOW_BATTLE,
#endif
#ifdef FLAG_DYNAMAX_BATTLE
	FLAG_DYNAMAX_BATTLE,
#endif
#ifdef FLAG_RAID_BATTLE
	FLAG_RAID_BATTLE,
#endif
#ifdef FLAG_RAID_BATTLE_NO_FORCE_END
	FLAG_RAID_BATTLE_NO_FORCE_END,
#endif
#ifdef FLAG_START_WITH_RAID_SHIELDS
	FLAG_START_WITH_RAID_SHIELDS,
#endif
#ifdef FLAG_KEEP_CONSUMABLE_ITEMS
	FLAG_KEEP_CONSUMABLE_ITEMS,
#endif
#ifdef FLAG_HIDDEN_ABILITY
	FLAG_HIDDEN_ABILITY,
#endif
#ifdef FLAG_GIGANTAMAXABLE
	FLAG_GIGANTAMAXABLE,
#endif
#ifdef FLAG_TEMP_DISABLE_RANDOMIZER
	FLAG_TEMP_DISABLE_RANDOMIZER,
#endif
#ifdef FLAG_AI_CONTROL_BATTLE
	FLAG_AI_CONTROL_BATTLE,
#endif
#ifdef FLAG_USE_HALF_PARTNER_TEAM
	FLAG_USE_HALF_PARTNER_TEAM,
#endif
#ifdef FLAG_BATTLE_YOURSELF
	FLAG_BATTLE_YOURSELF,
#endif
#ifdef FLAG_DISPLAY_OPPOSING_FOR_WILD
	FLAG_DISPLAY_OPPOSING_FOR_WILD,
#endif
#ifdef FLAG_HOOPA_SOS_BATTLE
	FLAG_HOOPA_SOS_BATTLE,
#endif
	FLAG_TAG_BATTLE,
	FLAG_TWO_OPPONENTS,
};

//This file's functions:
static void TryRestoreEnemyTeam(void);
static void NaturalCureHeal(void);
static void RestoreNonConsumableItems(void);
static void RevertDynamax(void);
static void RecalcAllStats(void);
static void BringBackTheDead(void);
static void EndPartnerBattlePartyRestore(void);
static void EndSkyBattlePartyRestore(void);
static void EndBenjaminButterfreeBattleRestore(void);
static void EndBattleFlagClear(void);
static void HealPokemonInFrontier(void);

void HandleEndTurn_BattleWon(void)
{
	gCurrentActionFuncId = 0;

	if (gBattleTypeFlags & BATTLE_TYPE_LINK)
	{
		//gSpecialVar_Result = gBattleOutcome;
		gBattleTextBuff1[0] = gBattleOutcome;
		gBankAttacker = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);
		gBattlescriptCurrInstr = BattleScript_LinkBattleWonOrLost;
		gBattleOutcome &= ~(B_OUTCOME_LINK_BATTLE_RAN);
	}
	else if (gBattleTypeFlags & BATTLE_TYPE_TRAINER
		  && gBattleTypeFlags & (BATTLE_TYPE_FRONTIER | BATTLE_TYPE_EREADER_TRAINER | BATTLE_TYPE_TRAINER_TOWER))
	{
		BattleStopLowHpSound();
		gBattlescriptCurrInstr = BattleScript_Victory;

		PlayBGM(BGM_VICTORY_TRAINER_BATTLE);
	}
	else if (gBattleTypeFlags & BATTLE_TYPE_TRAINER && !(gBattleTypeFlags & BATTLE_TYPE_LINK))
	{
		BattleStopLowHpSound();
		gBattlescriptCurrInstr = BattleScript_Victory;

		u16 id = gTrainerBattleOpponent_A;
		u8 specialMus = FALSE;
		u8 loop = FALSE;

		if (IsFrontierTrainerId(id))
		{
			PlayBGM(BGM_VICTORY_TRAINER_BATTLE);
			goto SKIP_MUSIC_SELECTION;
		}

	VICTORY_MUSIC_SELECTION:
		switch (gTrainers[id].trainerClass) {
		#ifndef UNBOUND //Change this part
			case CLASS_LEADER:
			case CLASS_ELITE_4:
			case CLASS_CHAMPION:
				PlayBGM(BGM_VICTORY_SPECIAL);
				specialMus = TRUE;
				break;
			default:
				PlayBGM(BGM_VICTORY_TRAINER_BATTLE);
				break;

		#else //For Pokemon Unbound
			case CLASS_CHAMPION:
				PlayBGM(BGM_VICTORY_CHAMPION);
				specialMus = TRUE;
				break;
			case CLASS_ELITE_4:
				PlayBGM(BGM_VICTORY_ELITE_4);
				specialMus = TRUE;
				break;
			case CLASS_LEADER:
			case CLASS_FRONTIER_BRAIN:
			case CLASS_SUCCESSOR:
				PlayBGM(BGM_VICTORY_GYM);
				specialMus = TRUE;
				break;
			case CLASS_SHADOW:
			case CLASS_SHADOW_ADMIN:
			case CLASS_BOSS:
				PlayBGM(BGM_VICTORY_PLASMA);
				specialMus = TRUE;
				break;
			case CLASS_LOR_ADMIN:
				if (VarGet(VAR_MAIN_STORY) <= 0x25) //MAIN_STORY_LEFT_CUBE
				{
					PlayBGM(BGM_VICTORY_PLASMA); //Ivory is still a Shadow Admin at this point
					specialMus = TRUE;
					break;
				}
				//Fallthrough
			case CLASS_LOR:
			case CLASS_LOR_LEADER:
			case CLASS_AGENT:
				PlayBGM(BGM_VICTORY_GALACTIC);
				specialMus = TRUE;
				break;
			default:
				PlayBGM(BGM_VICTORY_TRAINER_BATTLE);
				break;
		#endif
		}

		#ifdef UNBOUND
		if (MAP_IS(POKEMON_LEAGUE_CHAMP_ROOM))
		{
			PlayBGM(BGM_VICTORY_CHAMPION);
			specialMus = TRUE;
		}
		#endif

	SKIP_MUSIC_SELECTION:
		if (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS && !specialMus && !loop)
		{
			id = VarGet(VAR_SECOND_OPPONENT);
			loop = TRUE;
			goto VICTORY_MUSIC_SELECTION;
		}
	}
	else //Wild - Music played in Exp.C
	{
		gBattlescriptCurrInstr = BattleScript_PayDayMoneyAndPickUpItems;
	}

	gSpecialVar_LastResult = 0;
	gBattleMainFunc = HandleEndTurn_FinishBattle;
}

void HandleEndTurn_BattleLost(void)
{
	gCurrentActionFuncId = 0;

	if (gBattleTypeFlags & BATTLE_TYPE_LINK)
	{
		if (gBattleTypeFlags & BATTLE_TYPE_FRONTIER)
		{
			if (gBattleOutcome & B_OUTCOME_LINK_BATTLE_RAN)
			{
				gBattlescriptCurrInstr = BattleScript_PrintPlayerForfeitedLinkBattle;
				gBattleOutcome &= ~(B_OUTCOME_LINK_BATTLE_RAN);
			}
			else
			{
				gBattlescriptCurrInstr = BattleScript_LostMultiBattleTower;
				gBattleOutcome &= ~(B_OUTCOME_LINK_BATTLE_RAN);
			}
		}
		else
		{
			gBattleTextBuff1[0] = gBattleOutcome;
			gBankAttacker = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);
			gBattlescriptCurrInstr = BattleScript_LinkBattleWonOrLost;
			gBattleOutcome &= ~(B_OUTCOME_LINK_BATTLE_RAN);
			EndOfBattleThings();
		}
	}
	else if (gBattleTypeFlags & BATTLE_TYPE_FRONTIER)
	{
		if (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS)
			gBattlescriptCurrInstr = BattleScript_LostMultiBattleTower;
		else
			gBattlescriptCurrInstr = BattleScript_LostBattleTower;

		gBattleOutcome &= ~(B_OUTCOME_RAN);
		EndOfBattleThings();
	}
	else if (gBattleTypeFlags & BATTLE_TYPE_TRAINER && sTrainerBattleMode == TRAINER_BATTLE_OAK_TUTORIAL)
	{
		if (sTrainerBattleOakTutorialHelper & 1)
			gBattleCommunication[MULTISTRING_CHOOSER] = 1;
		else
			gBattleCommunication[MULTISTRING_CHOOSER] = 2;

		gBankAttacker = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);
		gBattlescriptCurrInstr = BattleScript_LocalBattleLost;
	}
	else if (IsRaidBattle() && FlagGet(FLAG_BATTLE_FACILITY))
	{
		gBattlescriptCurrInstr = BattleScript_LoseFrontierRaidBattle;
		EndOfBattleThings();
	}
	else
	{
		gBattleCommunication[MULTISTRING_CHOOSER] = 0;
		gBattlescriptCurrInstr = BattleScript_LocalBattleLost;
	}

	gSpecialVar_LastResult = 1;
	gBattleMainFunc = HandleEndTurn_FinishBattle;
}

void HandleEndTurn_RanFromBattle(void)
{
	gCurrentActionFuncId = 0;

	if (gBattleTypeFlags & BATTLE_TYPE_FRONTIER && gBattleTypeFlags & BATTLE_TYPE_TRAINER)
	{
		gBattlescriptCurrInstr = BattleScript_PrintPlayerForfeited;
		gBattleOutcome = B_OUTCOME_LOST;
	}
	else if (RAID_BATTLE_END)
	{
		gActiveBattler = BANK_RAID_BOSS;
		EmitSpriteInvisibility(0, TRUE);
		MarkBufferBankForExecution(gActiveBattler);

		gBattlescriptCurrInstr = BattleScript_RaidMonRanAway;
		gBattleOutcome = B_OUTCOME_WON;
	}
	else
	{
		switch (gProtectStructs[gBankAttacker].fleeFlag) {
			case 1:
				gBattlescriptCurrInstr = BattleScript_SmokeBallEscape; //0x81D8901
				break;
			case 2:
				gBattleScripting.bank = gBankAttacker;
				gBattlescriptCurrInstr = BattleScript_RanAwayUsingMonAbility; //0x81D890F
				break;
			default:
				gBattlescriptCurrInstr = BattleScript_GotAwaySafely; //0x81D8916
				break;
		}
	}

	gBattleMainFunc = HandleEndTurn_FinishBattle;
}

#define STATE_BEFORE_ACTION_CHOSEN 0

bool8 HandleRunActionFrontier(void)
{
	if (gBattleBufferB[gActiveBattler][1] == ACTION_RUN)
	{
		if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)
		{
			if (gBattleTypeFlags & BATTLE_TYPE_FRONTIER)
			{
				BattleScriptExecute(BattleScript_AskIfWantsToForfeitMatch);
				gBattleCommunication[gActiveBattler] = STATE_BEFORE_ACTION_CHOSEN;
				return TRUE;
			}
		}
		else if (!CantCatchBecauseFlag() //Wild battle where mon can be caught
		&& !IsRunningFromBattleImpossible()) //Can run right now
		{
			if ((GetMonData(&gEnemyParty[0], MON_DATA_HP, 0) != 0 && IsMonShiny(&gEnemyParty[0]))
			|| (IS_DOUBLE_BATTLE && GetMonData(&gEnemyParty[1], MON_DATA_HP, 0) != 0 && IsMonShiny(&gEnemyParty[1]))
			#ifdef FLAG_LEGENDARY_APPEARED_STRING
			|| FlagGet(FLAG_LEGENDARY_APPEARED_STRING)
			#endif
			)
			{
				BattleScriptExecute(BattleScript_ConfirmRunAway);
				gBattleCommunication[gActiveBattler] = STATE_BEFORE_ACTION_CHOSEN;
				return TRUE;
			}
		}
	}

	return FALSE;
}

#define ABILITY_PREVENTING_ESCAPE 2

bool8 AreAllKindsOfRunningPrevented(void)
{
	if (IsRaidBattle() && !RAID_BATTLE_END)
		return TRUE;
	#ifdef FLAG_NO_RUNNING
	else if (FlagGet(FLAG_NO_RUNNING))
		return TRUE;
	#endif
	#ifdef FLAG_NO_CATCHING_AND_RUNNING
	else if (FlagGet(FLAG_NO_CATCHING_AND_RUNNING))
		return TRUE;
	#endif

	return FALSE;
}

u8 IsRunningFromBattleImpossible(void)
{
	u8 itemEffect;
	u8 side;
	int i;

	itemEffect = ITEM_EFFECT(gActiveBattler);
	gStringBank = gActiveBattler;
	gBattleCommunication[MULTISTRING_CHOOSER] = 0;

	if (gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_FRONTIER))
		return FALSE;
	else if (RAID_BATTLE_END)
		return FALSE;
	else if (AreAllKindsOfRunningPrevented())
		return TRUE;
	else if (itemEffect == ITEM_EFFECT_CAN_ALWAYS_RUN)
		return FALSE;
	else if (gBattleMons[gActiveBattler].ability == ABILITY_RUNAWAY)
		return FALSE;
	else if (IsOfType(gActiveBattler, TYPE_GHOST))
		return FALSE;

	side = SIDE(gActiveBattler);

	for (i = 0; i < gBattlersCount; i++)
	{
		if (side != SIDE(i))
		{
			u8 ability = ABILITY(i);
			if (IsTrappedByAbility(gActiveBattler, ability))
			{
				gBattleScripting.bank = i;
				gLastUsedAbility = ABILITY(i);
				gBattleCommunication[MULTISTRING_CHOOSER] = ABILITY_PREVENTING_ESCAPE;
				return ABILITY_PREVENTING_ESCAPE;
			}
		}
	}

	if ((gBattleMons[gActiveBattler].status2 & (STATUS2_ESCAPE_PREVENTION | STATUS2_WRAPPED))
	|| (gStatuses3[gActiveBattler] & STATUS3_SKY_DROP_TARGET))
	{
		gBattleCommunication[MULTISTRING_CHOOSER] = 0;
		return TRUE;
	}

	if (!gNewBS->TeleportBit)
	{
		if (gStatuses3[gActiveBattler] & STATUS3_ROOTED)
		{
			gBattleCommunication[MULTISTRING_CHOOSER] = 0;
			return 1;
		}
	}
	else
		gNewBS->TeleportBit = FALSE;

	if (IsFairyLockActive())
	{
		gBattleCommunication[MULTISTRING_CHOOSER] = 0;
		return TRUE;
	}

	if (gBattleTypeFlags & BATTLE_TYPE_OAK_TUTORIAL)
	{
		gBattleCommunication[MULTISTRING_CHOOSER] = 1;
		return TRUE;
	}

	return 0;
}

bool8 TryRunFromBattle(u8 bank)
{
	bool8 effect = FALSE;
	u8 itemEffect;
	u8 speedVar;

	itemEffect = ITEM_EFFECT(bank);
	gStringBank = bank;

	if (AreAllKindsOfRunningPrevented())
	{
		return FALSE;
	}
	else if (IsRaidBattle() && RAID_BATTLE_END)
	{
		++effect; //So you can always run at the end of a Raid
	}
	else if (IsOfType(bank, TYPE_GHOST))
	{
		++effect;
	}
	else if (itemEffect == ITEM_EFFECT_CAN_ALWAYS_RUN)
	{
		gLastUsedItem = ITEM(bank);
		gProtectStructs[bank].fleeFlag = 1;
		++effect;
	}
	else if (ABILITY(bank) == ABILITY_RUNAWAY)
	{
		gLastUsedAbility = ABILITY_RUNAWAY;
		gProtectStructs[bank].fleeFlag = 2;
		++effect;
	}
	#ifndef NO_GHOST_BATTLES
	else if ((gBattleTypeFlags & (BATTLE_TYPE_SCRIPTED_WILD_1 | BATTLE_TYPE_GHOST)) == BATTLE_TYPE_GHOST)
	{
		if (SIDE(bank) == B_SIDE_PLAYER)
			++effect;
	}
	#endif
	else if (gBattleTypeFlags & (BATTLE_TYPE_FRONTIER | BATTLE_TYPE_TRAINER_TOWER) && gBattleTypeFlags & BATTLE_TYPE_TRAINER)
	{
		++effect;
	}
	#ifdef QUICK_CLAW_PROCING_ALWAYS_ALLOWS_FLEEING
	else if (ITEM_EFFECT(bank) == ITEM_EFFECT_QUICK_CLAW && QuickClawActivatesThisTurn(bank))
	{
		++effect;
	}
	#endif
	else
	{
		if (!(gBattleTypeFlags & BATTLE_TYPE_DOUBLE))
		{
		SINGLE_FLEE_CALC:
			if (gBattleMons[bank].speed < gBattleMons[BATTLE_OPPOSITE(bank)].speed)
			{
				speedVar = udivsi((gBattleMons[bank].speed * 128), (gBattleMons[BATTLE_OPPOSITE(bank)].speed)) + (gBattleStruct->runTries * 30);
				if (speedVar > (Random() & 0xFF))
					++effect;
			}
			else // same speed or faster
			{
				++effect;
			}
		}
		else //Wild Double
		{
			if (gBattleMons[BATTLE_OPPOSITE(bank)].hp)
				goto SINGLE_FLEE_CALC;

			if (gBattleMons[bank].speed < gBattleMons[PARTNER(BATTLE_OPPOSITE(bank))].speed)
			{
				speedVar = udivsi((gBattleMons[bank].speed * 128), (gBattleMons[PARTNER(BATTLE_OPPOSITE(bank))].speed)) + (gBattleStruct->runTries * 30);
				if (speedVar > (Random() & 0xFF))
					++effect;
			}
			else // same speed or faster
			{
				++effect;
			}
		}

		gBattleStruct->runTries++;
	}

	if (effect)
	{
		gCurrentTurnActionNumber = gBattlersCount;
		gBattleOutcome = B_OUTCOME_RAN;
	}

	return effect;
}

void BattleUseFunc_PokeDoll(u8 taskId)
{
	if (!(gBattleTypeFlags & BATTLE_TYPE_TRAINER)
	&& !AreAllKindsOfRunningPrevented())
	{
		RemoveUsedItem();
		DisplayItemMessageInBag(taskId, 2, gStringVar4, ItemMenu_StartFadeToExitCallback);
	}
	else
		PrintNotTheTimeToUseThat(taskId, 0);
}

void EndOfBattleThings(void)
{
	if (gNewBS != NULL) //Hasn't been cleared yet
	{
		TryRestoreEnemyTeam();
		NaturalCureHeal();
		RestoreNonConsumableItems();
		RevertDynamax();
		FormsRevert(gPlayerParty);
		MegaRevert(gPlayerParty);
		GigantamaxRevert(gPlayerParty);
		UpdateBurmy();
		EndPartnerBattlePartyRestore();
		EndSkyBattlePartyRestore();
		EndBenjaminButterfreeBattleRestore();
		RecalcAllStats();
		BringBackTheDead();
		EndBattleFlagClear();
		HealPokemonInFrontier();
		gTerrainType = 0; //Reset now b/c normal reset is after BG is loaded
		CalculatePlayerPartyCount(); //Party size can change after multi battle is over

		#ifdef UNBOUND
		u8 weather = GetCurrentWeather();
		if (InBattleSands())
		{
			if (weather != WEATHER_NONE
			&& weather != WEATHER_CLOUDS
			&& weather != WEATHER_SUNNY)
				SetSav1Weather(WEATHER_RAIN_LIGHT); //Reset weather in Battle Sands
		}
		else if (gBattleTypeFlags & BATTLE_TYPE_BATTLE_CIRCUS)
		{
			SetSav1Weather(WEATHER_NONE);
		}
		#endif
	}
}

static void TryRestoreEnemyTeam(void)
{
	#ifdef FLAG_BACKUP_ENEMY_TEAM
	if (FlagGet(FLAG_BACKUP_ENEMY_TEAM))
	{
		Memcpy(gEnemyParty, gNewBS->foePartyBackup, sizeof(struct Pokemon) * PARTY_SIZE);
		Free(gNewBS->foePartyBackup);
	}
	#endif
}

static void NaturalCureHeal(void)
{
	for (int bank = 0; bank < gBattlersCount; ++bank)
	{
		if (ABILITY(bank) == ABILITY_NATURALCURE && gBattleMons[bank].status1)
			GetBankPartyData(bank)->condition = 0;
	}
}

static void RestoreNonConsumableItems(void)
{
	u16 none = ITEM_NONE;
	u16* items = gNewBS->itemBackup;
	#ifdef FLAG_KEEP_CONSUMABLE_ITEMS
	bool8 keepConsumables = FlagGet(FLAG_KEEP_CONSUMABLE_ITEMS)
			#ifdef FLAG_SANDBOX_MODE
			|| (FlagGet(FLAG_SANDBOX_MODE) && gBattleTypeFlags & BATTLE_TYPE_TRAINER) //All Trainer battles
			#endif
			;
	#else
	bool8 keepConsumables = FALSE;
	#endif

	if (gBattleTypeFlags & BATTLE_TYPE_TRAINER || IsRaidBattle())
	{
		for (int i = 0; i < PARTY_SIZE; ++i)
		{
			if (gBattleTypeFlags & BATTLE_TYPE_FRONTIER
			||  keepConsumables
			||  items[i] == ITEM_NONE
			||  !IsConsumable(items[i]))
			{
				SetMonData(&gPlayerParty[i], MON_DATA_HELD_ITEM, &items[i]);
			}
			else if (gPlayerParty[i].item != items[i] //The player consumed their item, and then picked up another one
			&& IsConsumable(items[i]))
			{
				SetMonData(&gPlayerParty[i], MON_DATA_HELD_ITEM, &none);
			}
		}
	}
}

static void RevertDynamax(void)
{
	for (int i = 0; i < gBattlersCount; ++i)
		EndBattleDynamaxRevert(i);
}

static void RecalcAllStats(void)
{
	for (int i = 0; i < PARTY_SIZE; ++i)
	{
		u16 species = GetMonData(&gPlayerParty[i], MON_DATA_SPECIES2, NULL);
		if (species != SPECIES_NONE && species != SPECIES_EGG)
			CalculateMonStats(&gPlayerParty[i]); //Only recalc Pokemon that need to be recalced
	}
}

static void BringBackTheDead(void)
{
	//Used after Multi Battles that you lost, but your partner won
	if (ViableMonCount(gPlayerParty) == 0)
	{
		for (int i = 0; i < PARTY_SIZE; ++i)
		{
			u16 species = GetMonData(&gPlayerParty[i], MON_DATA_SPECIES2, NULL);
			if (species != SPECIES_NONE && species != SPECIES_EGG)
			{
				gPlayerParty[i].hp = 1;
				break;
			}
		}
	}
}

static void EndPartnerBattlePartyRestore(void)
{
	int i;
	u8 counter = 0;
	struct Pokemon* backup = ExtensionState.partyBackup;

	if (ExtensionState.partyBackup != NULL)
	{
		if (gSelectedOrderFromParty[0] == 0)
		{ 	//Special 0x2F was not used
			Memcpy(&gPlayerParty[3], ExtensionState.partyBackup, sizeof(struct Pokemon) * 3);
		}
		else
		{ 	//Special 0x2F was used
			struct Pokemon foughtMons[3];
			u8 newLeveledUpInBattle = 0;
			Memcpy(foughtMons, gPlayerParty, sizeof(struct Pokemon) * 3);
			Memset(gPlayerParty, 0x0, sizeof(struct Pokemon) * PARTY_SIZE);

			for (i = 0; i < 3; ++i)
			{
				if (gSelectedOrderFromParty[i] != 0) //Pokemon was chosen for battle
				{
					u8 correctSlotId = gSelectedOrderFromParty[i] - 1;
					Memcpy(&gPlayerParty[correctSlotId], &foughtMons[i], sizeof(struct Pokemon));
					if (gLeveledUpInBattle & gBitTable[i])
						newLeveledUpInBattle |= gBitTable[correctSlotId]; //Move to correct index
				}
			}

			//Pokemon that leveled up may move around after battle.
			//This is necessary so proper evolutions can happen after battle.
			gLeveledUpInBattle = newLeveledUpInBattle;

			for (i = 0; i < PARTY_SIZE; ++i)
			{
				if (!FlagGet(FLAG_BATTLE_FACILITY))
					gSelectedOrderFromParty[i] = 0; //Reset for next battle

				if (GetMonData(&gPlayerParty[i], MON_DATA_SPECIES, NULL) == SPECIES_NONE)
					Memcpy(&gPlayerParty[i], &backup[counter++], sizeof(struct Pokemon));
			}
		}

		Free(ExtensionState.partyBackup);
	}
}

static void EndSkyBattlePartyRestore(void)
{
	u32 i;
	u8 counter = 0;
	struct Pokemon* backup = ExtensionState.skyBattlePartyBackup;

	if (backup != NULL)
	{
		u8 newLeveledUpInBattle = 0;
		struct Pokemon tempTeam[PARTY_SIZE] = {0};
		bool8 slotFilled[PARTY_SIZE] = {FALSE};

		for (i = 0; i < PARTY_SIZE; ++i)
		{
			if (gSelectedOrderFromParty[i] != 0)
			{
				u8 correctSlotId = gSelectedOrderFromParty[i] - 1;
				tempTeam[correctSlotId] = gPlayerParty[i];
				slotFilled[correctSlotId] = TRUE;
				gSelectedOrderFromParty[i] = 0;

				if (gLeveledUpInBattle & gBitTable[i])
					newLeveledUpInBattle |= gBitTable[correctSlotId]; //Move to correct index
			}
		}

		gLeveledUpInBattle = newLeveledUpInBattle; //Same logic as in above function

		for (i = 0; i < PARTY_SIZE; ++i)
		{
			if (!slotFilled[i])
			{
				tempTeam[i] = backup[counter++];
				slotFilled[i] = TRUE;
			}
		}

		Memcpy(gPlayerParty, tempTeam, sizeof(struct Pokemon) * PARTY_SIZE);
		Free(backup);
	}
}

static void EndBenjaminButterfreeBattleRestore(void)
{
	#ifdef FLAG_BENJAMIN_BUTTERFREE_BATTLE
	if (FlagGet(FLAG_BENJAMIN_BUTTERFREE_BATTLE))
		LoadPlayerParty(); //Backed up before the battle
	#endif
}

static void EndBattleFlagClear(void)
{
	u32 i;

	for (i = 0; i < ARRAY_COUNT(gEndBattleFlagClearTable); ++i)
		FlagClear(gEndBattleFlagClearTable[i]);

	#ifdef VAR_STATUS_INDUCER
	u16 inducer = VarGet(VAR_STATUS_INDUCER);
	if (inducer & 0xFF00) //Temporary status inducer
	{
		u8 status = inducer & 0xFF;
		u8 amount = ((inducer & 0xFF00) >> 8) - 1; //Subtract num battles by 1

		if (amount == 0) //Time's up
			VarSet(VAR_STATUS_INDUCER, 0);
		else
			VarSet(VAR_STATUS_INDUCER, status | (amount << 8));
	}
	#endif

	//Handle Sirfetch'd Evolution
	gScored3CritsInBattle = 0;
	for (i = 0; i < PARTY_SIZE; ++i)
	{
		if (gNewBS->criticalHitsThisBattle[i] >= 3)
		{
			gScored3CritsInBattle |= gBitTable[i];
			if (EvolvesViaScoring3Crits(&gPlayerParty[i]))
				gLeveledUpInBattle |= gBitTable[i];
		}
	}

	//Reset Totem Vars
	VarSet(VAR_TOTEM + 0, 0);	//Bank B_POSITION_PLAYER_LEFT's Stat
	VarSet(VAR_TOTEM + 1, 0);	//Bank B_POSITION_OPPONENT_LEFT's Stat
	VarSet(VAR_TOTEM + 2, 0);	//Bank B_POSITION_PLAYER_RIGHT's Stat
	VarSet(VAR_TOTEM + 3, 0);	//Bank B_POSITION_OPPONENT_RIGHT's Stat

	VarSet(VAR_TERRAIN, 0);
	VarSet(VAR_BATTLE_FACILITY_TRAINER1_NAME, 0xFFFF);
	VarSet(VAR_BATTLE_FACILITY_TRAINER2_NAME, 0xFFFF);
	#ifdef VAR_BATTLE_TRANSITION_LOGO
	VarSet(VAR_BATTLE_TRANSITION_LOGO, 0);
	#endif
	gFishingByte = FALSE;
	FREE_AND_SET_NULL(gNewBS);

	//Handle DexNav Chain
	if (gDexNavStartedBattle
	&& (gBattleOutcome == B_OUTCOME_WON || gBattleOutcome == B_OUTCOME_CAUGHT))
	{
		if (gCurrentDexNavChain == 0)
			gCurrentDexNavChain = 2; //Next battle would be battle 2
		else if (gCurrentDexNavChain < 100)
			++gCurrentDexNavChain;
		else
			gCurrentDexNavChain = 1; //Restart from 1 (101 % 100 = 1)
	}
	else
		gCurrentDexNavChain = 0;
	gDexNavStartedBattle = FALSE;

	u16 backup = gTrainerBattleOpponent_B;
	Memset(&ExtensionState, 0x0, sizeof(struct BattleExtensionState));
	gTrainerBattleOpponent_B = backup;
	gBattleCircusFlags = 0;
	Memset(gStatuses3, 0, sizeof(gStatuses3));
}

static void HealPokemonInFrontier(void)
{
	//This helps with the issue of multi battle teams in the Battle Tower are not healed
	if (gBattleTypeFlags & BATTLE_TYPE_FRONTIER)
	{
		for (int i = 0; i < PARTY_SIZE; ++i)
		{
			struct Pokemon* mon = &gPlayerParty[i];

			if (GetMonData(mon, MON_DATA_SPECIES, NULL) != SPECIES_NONE)
				HealMon(mon);
		}
	}
}

bool8 IsConsumable(u16 item)
{
	u8 effect = gItems[SanitizeItemId(item)].holdEffect;

	for (u32 i = 0; gConsumableItemEffects[i] != 0xFF; ++i)
	{
		if (effect == gConsumableItemEffects[i])
			return TRUE;
	}

	return FALSE;
}
