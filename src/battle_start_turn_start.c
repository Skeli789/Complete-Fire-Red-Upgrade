#include "defines.h"
#include "defines_battle.h"
#include "../include/battle_transition.h"
#include "../include/battle_setup.h"
#include "../include/event_data.h"
#include "../include/gpu_regs.h"
#include "../include/load_save.h"
#include "../include/random.h"
#include "../include/scanline_effect.h"
#include "../include/constants/pokedex.h"
#include "../include/constants/songs.h"
#include "../include/constants/trainers.h"
#include "../include/constants/trainer_classes.h"

#include "../include/new/ability_battle_scripts.h"
#include "../include/new/ai_master.h"
#include "../include/new/ai_switching.h"
#include "../include/new/ai_util.h"
#include "../include/new/battle_start_turn_start.h"
#include "../include/new/battle_start_turn_start_battle_scripts.h"
#include "../include/new/battle_transition.h"
#include "../include/new/battle_util.h"
#include "../include/new/cmd49.h"
#include "../include/new/damage_calc.h"
#include "../include/new/dexnav.h"
#include "../include/new/dns.h"
#include "../include/new/dynamax.h"
#include "../include/new/form_change.h"
#include "../include/new/frontier.h"
#include "../include/new/general_bs_commands.h"
#include "../include/new/multi.h"
#include "../include/new/mega.h"
#include "../include/new/move_battle_scripts.h"
#include "../include/new/move_tables.h"
#include "../include/new/set_z_effect.h"
#include "../include/new/util.h"

/*
battle_start_turn_start.c
	-handles the logic for determining which pokemon attacks first
	-also handles setting up battle data
*/

enum BattleBeginStates
{
	BTSTART_BACKUP_PARTY_ITEMS,
	BTSTART_GET_TURN_ORDER,
	BTSTART_ACTIVATE_OW_WEATHER,
	BTSTART_ACTIVATE_OW_TERRAIN,
	BTSTART_PRIMAL_REVERSION,
	BTSTART_THIRD_TYPE_REMOVAL,
	BTSTART_RAID_BATTLE_REVEAL,
	BTSTART_DYNAMAX_USABLE_INDICATOR,
	BTSTART_BAD_THOUGHTS_BATTLE,
	BTSTART_TAILWIND_BATTLE,
	BTSTART_MAGNET_RISE_BATTLE,
	BTSTART_TRICK_ROOM_BATTLE,
	BTSTART_WEIGHT_SPEED_BATTLE,
	BTSTART_SHADOW_SHIELD_BATTLE,
	BTSTART_PIXIE_BATTLE,
	BTSTART_PIXIE_BATTLE_BUFFS,
	BTSTART_RAINBOW_BATTLE,
	BTSTART_NEUTRALIZING_GAS,
	BTSTART_SWITCH_IN_ABILITIES,
	BTSTART_SWITCH_IN_ITEMS,
	BTSTART_AIR_BALLOON,
	BTSTART_TOTEM_POKEMON,
	BTSTART_END,
};

enum SpeedWarResults
{
	FirstMon,
	SecondMon,
	SpeedTie,
};

extern void (* const sTurnActionsFuncsTable[])(void);
extern void (* const sEndTurnFuncsTable[])(void);
extern const u16 gClassBasedBattleBGM[];
extern const u16 gWildSpeciesBasedBattleBGM[];
extern const u16 gRandomBattleMusicOptions[];
extern const u8 gRandomBattleMusicOptionsLength;
extern const u16 gWildSpeciesBasedBattleBGMLength;

const u8 gStatStageRatios[][2] =
{
	{10, 40}, // -6
	{10, 35}, // -5
	{10, 30}, // -4
	{10, 25}, // -3
	{10, 20}, // -2
	{10, 15}, // -1
	{10, 10}, //  0
	{15, 10}, //  1
	{20, 10}, //  2
	{25, 10}, //  3
	{30, 10}, //  4
	{35, 10}, //  5
	{40, 10}  //  6
};

//This file's functions:
static void SavePartyItems(void);
static void TryPrepareTotemBoostInBattleSands(void);
static void TrySetupRaidBossRepeatedAttack(u8 turnActionNumber);
static u8 GetWhoStrikesFirstUseLastBracketCalc(u8 bank1, u8 bank2);
static u32 BoostSpeedInWeather(u8 ability, u8 itemEffect, u32 speed);
static u32 BoostSpeedByItemEffect(u8 itemEffect, u8 itemQuality, u16 species, u32 speed, bool8 isDynamaxed);
static void TryClearLevelCapKeptOn(void);

void HandleNewBattleRamClearBeforeBattle(void)
{
	gNewBS = Calloc(sizeof(struct NewBattleStruct));
	Memset(FIRST_NEW_BATTLE_RAM_LOC, 0, (u32) LAST_NEW_BATTLE_RAM_LOC - (u32) FIRST_NEW_BATTLE_RAM_LOC);
	Memset(gBattleBufferA, 0x0, sizeof(gBattleBufferA)); //Clear both battle buffers
	Memset(gBattleBufferB, 0x0, sizeof(gBattleBufferB));
	Memset(gBattleMons, 0x0, sizeof(gBattleMons)); //Clear battle data - can be filled from last double battle and interfere with battle engine
	//Memset((u8*) 0x203C020, 0x0, 0xE0);

	if (IsRaidBattle())
	{
		gBattleTypeFlags |= BATTLE_TYPE_DYNAMAX;
		if (!IsBannedDynamaxBaseSpecies(GetMonData(&gEnemyParty[0], MON_DATA_SPECIES, NULL)))
			gNewBS->dynamaxData.timer[B_POSITION_OPPONENT_LEFT] = -2; //Don't revert
		gNewBS->dynamaxData.backupRaidMonItem = GetMonData(&gEnemyParty[0], MON_DATA_HELD_ITEM, NULL); //For Frontier
	}

	if (gBattleTypeFlags & BATTLE_TYPE_RING_CHALLENGE)
	{
		#ifdef VAR_RING_CHALLENGE_BANNED_TYPE_1
		gNewBS->ringChallengeBannedTypes[0] = VarGet(VAR_RING_CHALLENGE_BANNED_TYPE_1);
		#endif
		#ifdef VAR_RING_CHALLENGE_BANNED_TYPE_2
		gNewBS->ringChallengeBannedTypes[1] = VarGet(VAR_RING_CHALLENGE_BANNED_TYPE_2);
		#endif
		#ifdef VAR_RING_CHALLENGE_BANNED_TYPE_3
		gNewBS->ringChallengeBannedTypes[2] = VarGet(VAR_RING_CHALLENGE_BANNED_TYPE_3);
		#endif
	}

	#ifdef FLAG_BENJAMIN_BUTTERFREE_BATTLE
	if (FlagGet(FLAG_BENJAMIN_BUTTERFREE_BATTLE))
		SavePlayerParty(); //Backup party to be restored after the battle
	#endif

	gNewBS->isTrainerBattle = (gBattleTypeFlags & BATTLE_TYPE_TRAINER) != 0; //Used as part of the anti-catch-Trainer-Pokemon cheat
	FormsRevert(gPlayerParty); //Try to reset all forms before battle
}

void VBlankCB_Battle(void)
{
	#ifndef DEBUG_AI_CHOICES
	if (!(gBattleTypeFlags & BATTLE_TYPE_BATTLE_SANDS)) //Prevent RNG exploit in Battle Sands
		AIRandom(); //Change only AI thinking seed every frame
	#endif
	SetGpuReg(REG_OFFSET_BG0HOFS, gBattle_BG0_X);
	SetGpuReg(REG_OFFSET_BG0VOFS, gBattle_BG0_Y);
	SetGpuReg(REG_OFFSET_BG1HOFS, gBattle_BG1_X);
	SetGpuReg(REG_OFFSET_BG1VOFS, gBattle_BG1_Y);
	SetGpuReg(REG_OFFSET_BG2HOFS, gBattle_BG2_X);
	SetGpuReg(REG_OFFSET_BG2VOFS, gBattle_BG2_Y);
	SetGpuReg(REG_OFFSET_BG3HOFS, gBattle_BG3_X);
	SetGpuReg(REG_OFFSET_BG3VOFS, gBattle_BG3_Y);
	SetGpuReg(REG_OFFSET_WIN0H, gBattle_WIN0H);
	SetGpuReg(REG_OFFSET_WIN0V, gBattle_WIN0V);
	SetGpuReg(REG_OFFSET_WIN1H, gBattle_WIN1H);
	SetGpuReg(REG_OFFSET_WIN1V, gBattle_WIN1V);
	LoadOam();
	ProcessSpriteCopyRequests();
	TransferPlttBuffer();
	ScanlineEffect_InitHBlankDmaTransfer();
}

static void SavePartyItems(void)
{
	for (int i = 0; i < PARTY_SIZE; ++i)
		gNewBS->itemBackup[i] = GetMonData(&gPlayerParty[i], MON_DATA_HELD_ITEM, NULL);
}

static void TryBackupEnemyTeam(void)
{
	#ifdef FLAG_BACKUP_ENEMY_TEAM
	if (FlagGet(FLAG_BACKUP_ENEMY_TEAM))
	{
		gNewBS->foePartyBackup = Calloc(sizeof(struct Pokemon) * PARTY_SIZE);
		Memcpy(gNewBS->foePartyBackup, gEnemyParty, sizeof(struct Pokemon) * PARTY_SIZE);
	}
	#endif
}

static void TryClearLevelCapKeptOn(void)
{
	#if (defined FLAG_HARD_LEVEL_CAP && defined FLAG_KEPT_LEVEL_CAP_ON)
	if (!FlagGet(FLAG_SYS_GAME_CLEAR) //Main game
	&& FlagGet(FLAG_HARD_LEVEL_CAP) //Level Cap is on
	&& FlagGet(FLAG_KEPT_LEVEL_CAP_ON)) //And it hasn't ever been turned off
	{
		u32 i, levelCap;
		
		extern u8 GetCurrentLevelCap(void); //Must be implemented yourself
		for (i = 0, levelCap = GetCurrentLevelCap(); i < PARTY_SIZE; ++i)
		{
			u16 species = GetMonData(&gPlayerParty[i], MON_DATA_SPECIES2, NULL);
			if (species != SPECIES_NONE && species != SPECIES_EGG)
			{
				u8 level = GetMonData(&gPlayerParty[i], MON_DATA_LEVEL, NULL);
				if (level > levelCap)
				{
					FlagClear(FLAG_KEPT_LEVEL_CAP_ON);
					break;
				}
			}
		}
	}
	#endif
}

void BattleBeginFirstTurn(void)
{
	int i, j;
	u8* state = &(gBattleStruct->switchInAbilitiesCounter);
	u8* bank = &(gBattleStruct->switchInItemsCounter);

	if (!gBattleExecBuffer) //Inlclude Safari Check Here?
	{
		switch(*state) {
			case BTSTART_BACKUP_PARTY_ITEMS:
				#ifdef FLAG_SEMI_SHIFT
				if (FlagGet(FLAG_SEMI_SHIFT))
					gBattleScripting.battleStyle = OPTIONS_BATTLE_STYLE_SEMI_SHIFT;
				#endif
				SavePartyItems();
				TryBackupEnemyTeam();
				TryClearLevelCapKeptOn();
				++*state;
				break;

			case BTSTART_GET_TURN_ORDER:
				gNewBS->skipBankStatAnim = 0xFF;
				for (i = 0; i < gBattlersCount; ++i)
				{
					gBanksByTurnOrder[i] = i;
					ResetBestMonToSwitchInto(i);
				}

				for (i = 0; i < gBattlersCount - 1; ++i)
				{
					for (j = i + 1; j < gBattlersCount; ++j)
					{
						if (GetWhoStrikesFirst(gBanksByTurnOrder[i], gBanksByTurnOrder[j], 1))
							SwapTurnOrder(i, j);
					}
				}
				++*state;
				break;

			case BTSTART_ACTIVATE_OW_WEATHER:
				if (!gBattleStruct->overworldWeatherDone && AbilityBattleEffects(ABILITYEFFECT_ON_SWITCHIN, 0, 0, 0xFF, 0))
				{
					gBattleStruct->overworldWeatherDone = TRUE;
					return;
				}

				++*state;
				break;

			case BTSTART_ACTIVATE_OW_TERRAIN:
				if (TryActivateOWTerrain())
					return;

				++*state;
				*bank = 0;
				break;

			case BTSTART_PRIMAL_REVERSION:
				//Primal Reversion
				if (!(gBattleTypeFlags & (BATTLE_TYPE_OLD_MAN | BATTLE_TYPE_POKE_DUDE))
				&& !IsMegaZMoveBannedBattle())
				{
					for (; *bank < gBattlersCount; ++*bank)
					{
						const u8* script = DoPrimalReversion(gBanksByTurnOrder[*bank], 0);

						if (script != NULL)
						{
							BattleScriptPushCursorAndCallback(script);
							gBankAttacker = gBattleScripting.bank = gBanksByTurnOrder[*bank];
							++*bank;
							return;
						}
					}
				}

				++*state;
				*bank = 0;
				break;

			case BTSTART_THIRD_TYPE_REMOVAL:
				for (; *bank < gBattlersCount; ++*bank)
				{
					gBattleMons[*bank].type3 =  TYPE_BLANK;

					if (AreAbilitiesSuppressed()) //Most likely Circus
					{
						gNewBS->SuppressedAbilities[*bank] = gBattleMons[*bank].ability;
						gBattleMons[*bank].ability = 0;
					}

					if (gBattleTypeFlags & BATTLE_TYPE_CAMOMONS) //The Pokemon takes on the types of its first two moves
					{
						#ifndef NO_GHOST_BATTLES
						if (IS_GHOST_BATTLE && SIDE(*bank) == B_SIDE_OPPONENT)
							continue;
						#endif

						UpdateTypesForCamomons(*bank);
						gBattleScripting.bank = *bank;
						BattleScriptPushCursorAndCallback(BattleScript_CamomonsTypeRevealEnd3);

						u8 type1, type2;
						struct Pokemon* monIllusion = GetIllusionPartyData(*bank);
						if (monIllusion != GetBankPartyData(*bank)) //Under Illusion
						{
							type1 = GetMonType(monIllusion, 0);
							type2 = GetMonType(monIllusion, 1);
						}
						else
						{
							type1 = gBattleMons[*bank].type1;
							type2 = gBattleMons[*bank].type2;
						}

						if (type1 == type2)
							gBattleStringLoader = gText_CamomonsTypeReveal;
						else
							gBattleStringLoader = gText_CamomonsTypeRevealDualType;
						PREPARE_TYPE_BUFFER(gBattleTextBuff1, type1);
						PREPARE_TYPE_BUFFER(gBattleTextBuff2, type2);
						++*bank;
						return;
					}
				}

				*bank = 0;
				++*state;
				break;

			case BTSTART_RAID_BATTLE_REVEAL:
				if (ShouldStartWithRaidShieldsUp() && !gNewBS->dynamaxData.raidShieldsUp) //Can be done outside of Raid Battles now
				{
					//Start with shields for harder Raids
					gNewBS->dynamaxData.raidShieldsUp = TRUE;
					gNewBS->dynamaxData.shieldsDestroyed = 0;
					gBattleScripting.bank = BANK_RAID_BOSS;
					BattleScriptPushCursorAndCallback(BattleScript_RaidShieldsBattleStart);
				}

				if (IsRaidBattle())
				{
					gAbsentBattlerFlags |= gBitTable[B_POSITION_OPPONENT_RIGHT]; //Because it's not there - causes bugs without

					#ifdef FLAG_RAID_BATTLE_NO_FORCE_END
					if (FlagGet(FLAG_RAID_BATTLE_NO_FORCE_END))
					{
						//Don't play the storm animation in a Raid Battle that isn't 10 turns long
					}
					else
					#endif
					{
						gBattleScripting.bank = BANK_RAID_BOSS;
						gBattleStringLoader = gText_RaidBattleReveal;
						if (IsBannedDynamaxBaseSpecies(SPECIES(BANK_RAID_BOSS)))
							BattleScriptPushCursorAndCallback(BattleScript_RaidBattleStart_NoDynamax);
						else
							BattleScriptPushCursorAndCallback(BattleScript_RaidBattleStart);
					}
				}

				++*state;
				break;

			case BTSTART_DYNAMAX_USABLE_INDICATOR:
				#ifdef DYNAMAX_FEATURE
				gBattleScripting.bank = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);
				if (DynamaxEnabled(gBattleScripting.bank))
				{
					gBattleStringLoader = gText_DynamaxUsable;
					BattleScriptPushCursorAndCallback(BattleScript_DynamaxEnergySwirl);
				}
				#endif
				++*state;
				break;

			case BTSTART_BAD_THOUGHTS_BATTLE:
				#ifdef FLAG_BAD_THOUGHTS_BATTLE
				if (FlagGet(FLAG_BAD_THOUGHTS_BATTLE)) //Only print the message when the flag is set, not in Battle Circus
				{
					gBankAttacker = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);
					gBattleStringLoader = gText_BadThoughtsBattleStart;
					BattleScriptPushCursorAndCallback(BattleScript_PrintCustomStringEnd3);
				}
				#endif
				++*state;
				break;

			case BTSTART_TAILWIND_BATTLE:
				#ifdef FLAG_TAILWIND_BATTLE
				if (FlagGet(FLAG_TAILWIND_BATTLE))
				{
					gBattleStringLoader = gText_TailwindBattleStart;
					BattleScriptPushCursorAndCallback(BattleScript_PrintCustomStringEnd3);
				}
				#endif
				++*state;
				break;

			case BTSTART_MAGNET_RISE_BATTLE:
				if (IsMagnetRiseBattle())
				{
					for (; *bank < gBattlersCount; ++*bank)
					{
						if (IsFloatingWithMagnetism(*bank))
						{
							gBankAttacker = gBattleScripting.bank = *bank;
							gBattleStringLoader = gText_MagnetRiseBattleStart;
							BattleScriptPushCursorAndCallback(BattleScript_PrintCustomStringEnd3);
							++*bank;
							return;
						}
					}
				}
				++*state;
				break;

			case BTSTART_TRICK_ROOM_BATTLE:
				#ifdef FLAG_TRICK_ROOM_BATTLE
				if (FlagGet(FLAG_TRICK_ROOM_BATTLE))
				{
					gBattleStringLoader = gText_TrickRoomBattleStart;
					BattleScriptPushCursorAndCallback(BattleScript_PrintCustomStringEnd3);
				}
				#endif
				++*state;
				break;

			case BTSTART_WEIGHT_SPEED_BATTLE:
				#ifdef FLAG_WEIGHT_SPEED_BATTLE
				if (FlagGet(FLAG_WEIGHT_SPEED_BATTLE))
				{
					gBattleStringLoader = gText_WeightSpeedBattleStart;
					BattleScriptPushCursorAndCallback(BattleScript_PrintCustomStringEnd3);
				}
				#endif
				++*state;
				break;

			case BTSTART_SHADOW_SHIELD_BATTLE:
				#ifdef FLAG_SHADOW_SHIELD_BATTLE
				if (FlagGet(FLAG_SHADOW_SHIELD_BATTLE))
				{
					gBattleStringLoader = gText_ShadowShieldBattleStart;
					BattleScriptPushCursorAndCallback(BattleScript_PrintCustomStringEnd3);
				}
				#endif
				++*state;
				break;

			case BTSTART_PIXIE_BATTLE:
				#ifdef FLAG_PIXIE_BATTLE
				if (FlagGet(FLAG_PIXIE_BATTLE)) //Only print the message when the flag is set, not in Battle Circus
				{
					gBattleStringLoader = gText_PixieBattleStart;
					BattleScriptPushCursorAndCallback(BattleScript_PrintCustomStringEnd3);
				}
				#endif
				*bank = 0; //Reset Bank for next loop
				++*state;
				break;

			case BTSTART_PIXIE_BATTLE_BUFFS:
				if (IsPixieBattle())
				{
					for (; *bank < gBattlersCount; ++*bank)
					{
						if (IsAffectedByPixies(*bank))
						{
							if (gBattleMons[*bank].defense <= gBattleMons[*bank].spDefense) //Prioritizes defense when equal
								gBattleScripting.statChanger = STAT_STAGE_DEF | INCREASE_1;
							else
								gBattleScripting.statChanger = STAT_STAGE_SPDEF | INCREASE_1;

							BattleScriptPushCursorAndCallback(BattleScript_PixieBoost);
							gBankAttacker = gBattleScripting.bank = *bank;
							++*bank;
							return;
						}
					}
				}
				++*state;
				break;

			case BTSTART_RAINBOW_BATTLE:
				#ifdef FLAG_RAINBOW_BATTLE
				if (FlagGet(FLAG_RAINBOW_BATTLE))
				{
					gBattleStringLoader = gText_RainbowBattleStart;
					BattleScriptPushCursorAndCallback(BattleScript_PrintCustomStringEnd3);
				}
				#endif
				++*state;
				break;

			case BTSTART_NEUTRALIZING_GAS:
				for (; *bank < gBattlersCount; ++*bank)
				{
					if (ABILITY(gBanksByTurnOrder[*bank]) == ABILITY_NEUTRALIZINGGAS
					&& AbilityBattleEffects(ABILITYEFFECT_ON_SWITCHIN, gBanksByTurnOrder[*bank], 0, 0, 0))
					{
						++*bank;
						return;
					}
				}

				*bank = 0;
				++*state;
				break;

			case BTSTART_SWITCH_IN_ABILITIES:
				for (; *bank < gBattlersCount; ++*bank)
				{
					if (AbilityBattleEffects(ABILITYEFFECT_ON_SWITCHIN, gBanksByTurnOrder[*bank], 0, 0, 0))
					{
						++*bank;
						return;
					}
				}

				*bank = 0; //Reset Bank for next loop
				++*state;
				break;

			case BTSTART_SWITCH_IN_ITEMS:
				for (; *bank < gBattlersCount; ++*bank)
				{
					if (ItemBattleEffects(ItemEffects_SwitchIn, gBanksByTurnOrder[*bank], FALSE, FALSE))
					{
						++*bank;
						return;
					}
				}

				*bank = 0; //Reset Bank for next loop
				++*state;
				break;

			case BTSTART_AIR_BALLOON:
				for (; *bank < gBattlersCount; ++*bank)
				{
					#ifndef NO_GHOST_BATTLES
					if (IS_GHOST_BATTLE && SIDE(gBanksByTurnOrder[*bank]) == B_SIDE_OPPONENT)
						continue;
					#endif

					if (ITEM_EFFECT(gBanksByTurnOrder[*bank]) == ITEM_EFFECT_AIR_BALLOON)
					{
						BattleScriptPushCursorAndCallback(BattleScript_AirBalloonFloat);
						gBankAttacker = gBattleScripting.bank = gBanksByTurnOrder[*bank];
						RecordItemEffectBattle(gBankAttacker, ITEM_EFFECT_AIR_BALLOON);
						++*bank;
						return;
					}
				}

				*bank = 0; //Reset Bank for next loop
				TryPrepareTotemBoostInBattleSands();
				++*state;
				break;

			case BTSTART_TOTEM_POKEMON:
				if (InBattleSands() //The only battle facility to utilize totem boosts
				|| !(gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_FRONTIER | BATTLE_TYPE_EREADER_TRAINER | BATTLE_TYPE_TRAINER_TOWER | BATTLE_TYPE_POKE_DUDE | BATTLE_TYPE_OLD_MAN)))
				{
					for (; *bank < gBattlersCount; ++*bank)
					{
						#ifndef NO_GHOST_BATTLES
						if (IS_GHOST_BATTLE && SIDE(*bank) == B_SIDE_OPPONENT)
							continue;
						#endif

						u8 totemBoostType = CanActivateTotemBoost(*bank);
						if (totemBoostType == TOTEM_SINGLE_BOOST)
						{
							BattleScriptPushCursorAndCallback(BattleScript_Totem);
							gBankAttacker = gBattleScripting.bank = *bank;
							++*bank;
							return;
						}
						else if (totemBoostType == TOTEM_MULTI_BOOST)
						{
							BattleScriptPushCursorAndCallback(BattleScript_TotemMultiBoost);
							gBankAttacker = gBattleScripting.bank = *bank;
							++*bank;
							return;
						}
						else if (totemBoostType == TOTEM_OMNIBOOST) //All main stats
						{
							gBattleScripting.statAnimPlayed = FALSE;
							gBankAttacker = gBattleScripting.bank = *bank;
							GiveOmniboost(gBankAttacker);
							BattleScriptPushCursorAndCallback(BattleScript_TotemOmniboost);
							++*bank;
							return;
						}
					}
				}

				*bank = 0; //Reset Bank for next loop
				++*state;
				break;

			case BTSTART_END:
				for (i = 0; i < MAX_BATTLERS_COUNT; i++)
				{
					gBattleStruct->monToSwitchIntoId[i] = PARTY_SIZE;
					gChosenActionByBank[i] = 0xFF;
					gChosenMovesByBanks[i] = 0;

					if (i < gBattlersCount)
					{
						if (SIDE(i) == B_SIDE_OPPONENT)
							gNewBS->revealedEnemyMons |= gBitTable[gBattlerPartyIndexes[i]]; //Set up base for team preview

						//Prepare switching anti-AI abuse
						gNewBS->ai.previousMonIn[i] = 0xFF;
						gNewBS->ai.secondPreviousMonIn[i] = 0xFF;

						if (IsPlayerInControl(i))
						{
							switch (ABILITY(i))
							{
								//These Abilities are commonly switched out of on the first turn
								case ABILITY_INTIMIDATE:
									gNewBS->ai.switchesInARow[i] = 2; //So the AI gets smart if the player immediately switches out
									break;
							}
						}
					}
				}

				ClearCachedAIData();
				ClearMonCachedDamageCalcs();
				TurnValuesCleanUp(0);
				SpecialStatusesClear();
				gBattleStruct->field_91 = gAbsentBattlerFlags;
				gBattleMainFunc = (void*) (0x8014040 | 1);
				ResetSentPokesToOpponentValue();
				for (i = 0; i < 8; i++)
					gBattleCommunication[i] = 0;

				for (i = 0; i < gBattlersCount; i++)
				{
					gBattleMons[i].status2 &= ~8;
					gNewBS->pickupStack[i] = 0xFF;
					gNewBS->statRoseThisRound[i] = FALSE;
					gNewBS->statFellThisTurn[i] = FALSE;
					gNewBS->statFellThisRound[i] = FALSE;
					UpdateQuickClawRandomNumber(i);
				}

				gBattleStruct->turnEffectsTracker = 0;
				gBattleStruct->turnEffectsBank = 0;
				gBattleStruct->wishPerishSongState = 0;
				gBattleStruct->wishPerishSongBattlerId = 0;
				gBattleScripting.atk49_state = 0;
				gBattleStruct->faintedActionsState = 0;
				gBattleStruct->turncountersTracker = 0;
				gMoveResultFlags = 0;
				gRandomTurnNumber = Random();
				#ifdef DEBUG_AI_CHOICES
				gNewBS->ai.randSeed = Random32(); //So the seed doesn't start at 0
				#endif
				Memset(gNewBS->ai.pivotTo, PARTY_SIZE, sizeof(gNewBS->ai.pivotTo));
				CalculateShellSideArmSplits(); //Only done at the beginning of each turn
				*state = 0;
				#ifdef DEBUG_QUICK_BATTLES
				gBattleOutcome = B_OUTCOME_WON;
				#endif
		}
	}
}

bool8 TryActivateOWTerrain(void)
{
	bool8 effect = FALSE;
	u8 owTerrain = VarGet(VAR_TERRAIN);

	if (gBattleTypeFlags & BATTLE_TYPE_BATTLE_CIRCUS)
	{
		//Can have at most one of these set at a time
		switch (gBattleCircusFlags & BATTLE_CIRCUS_TERRAIN) {
			case BATTLE_CIRCUS_ELECTRIC_TERRAIN:
				owTerrain = ELECTRIC_TERRAIN;
				break;
			case BATTLE_CIRCUS_GRASSY_TERRAIN:
				owTerrain = GRASSY_TERRAIN;
				break;
			case BATTLE_CIRCUS_MISTY_TERRAIN:
				owTerrain = MISTY_TERRAIN;
				break;
			case BATTLE_CIRCUS_PSYCHIC_TERRAIN:
				owTerrain = PSYCHIC_TERRAIN;
				break;
		}
	}

	if (owTerrain != 0 && gTerrainType != owTerrain && !gNewBS->terrainForcefullyRemoved)
	{
		switch (owTerrain) {
			case ELECTRIC_TERRAIN:
				BattleScriptPushCursorAndCallback(BattleScript_ElectricTerrainBattleBegin);
				effect = TRUE;
				break;
			case GRASSY_TERRAIN:
				BattleScriptPushCursorAndCallback(BattleScript_GrassyTerrainBattleBegin);
				effect = TRUE;
				break;
			case MISTY_TERRAIN:
				BattleScriptPushCursorAndCallback(BattleScript_MistyTerrainBattleBegin);
				effect = TRUE;
				break;
			case PSYCHIC_TERRAIN:
				BattleScriptPushCursorAndCallback(BattleScript_PsychicTerrainBattleBegin);
				effect = TRUE;
		}

		if (effect)
		{
			gTerrainType = owTerrain;
			gNewBS->TerrainTimer = 0; //Reset any timers that may have been set before
		}
	}

	return effect;
}

u8 GetTotemStat(u8 bank, bool8 multiBoost)
{
	if (multiBoost)
		bank = PARTNER(bank);

	return VarGet(VAR_TOTEM + bank) & 0x7;
}

u8 GetTotemRaiseAmount(u8 bank, bool8 multiBoost)
{
	if (multiBoost)
		bank = PARTNER(bank);

	return VarGet(VAR_TOTEM + bank) & ~(0xF);
}

s8 TotemRaiseAmountToStatMod(u8 raiseAmount)
{
	if (raiseAmount >= INCREASE_1 && raiseAmount <= INCREASE_6)
		return raiseAmount >> 4; //1 to 6

	if (raiseAmount >= DECREASE_1 && raiseAmount <= DECREASE_6)
	{
		u8 temp = raiseAmount >> 4; //9 to 14
		temp *= -1; //-9 to -14
		temp += 8; //-1 to -6
		return temp;
	}

	return 0;
}

u8 CanActivateTotemBoost(u8 bank)
{
	u16 val = VarGet(VAR_TOTEM + bank);
	u16 stat = GetTotemStat(bank, FALSE);

	if (bank < gBattlersCount && stat != 0)
	{
		if (val == 0xFFFF) //Omniboost
		{
			if (InBattleSands()
			#ifdef FLAG_SINGLE_TRAINER_MON_TOTEM_BOOST
			|| FlagGet(FLAG_SINGLE_TRAINER_MON_TOTEM_BOOST)
			#endif
			)
				VarSet(VAR_TOTEM + bank, 0); //Only first Pokemon gets boost in battle sands

			return TOTEM_OMNIBOOST;
		}

		u8 raiseAmount = GetTotemRaiseAmount(bank, FALSE);
		if (stat <= STAT_STAGE_EVASION
		&& ((raiseAmount >= INCREASE_1 && raiseAmount <= INCREASE_6)
		 || (raiseAmount >= DECREASE_1 && raiseAmount <= DECREASE_6)))
		{
			gBattleScripting.statChanger = stat | raiseAmount;

			if (InBattleSands()
			#ifdef FLAG_SINGLE_TRAINER_MON_TOTEM_BOOST
			|| FlagGet(FLAG_SINGLE_TRAINER_MON_TOTEM_BOOST)
			#endif
			)
				VarSet(VAR_TOTEM + bank, 0); //Only first Pokemon gets boost in battle sands

			if (IS_SINGLE_BATTLE && VarGet(VAR_TOTEM + PARTNER(bank)) != 0) //Second stat is stored in partner's var
				return TOTEM_MULTI_BOOST;

			return TOTEM_SINGLE_BOOST;
		}
	}

	return TOTEM_NO_BOOST;
}

static void TryPrepareTotemBoostInBattleSands(void)
{
	if (InBattleSands())
	{
		u8 playerId = 0;
		u8 enemyId = 1;
		u8 playerStat = RandRange(STAT_STAGE_ATK, STAT_STAGE_ACC + 1); //No Evasion boost
		u8 enemyStat = RandRange(STAT_STAGE_ATK, STAT_STAGE_ACC + 1);
		u8 increaseMax, increase;

		if (IS_DOUBLE_BATTLE)
		{
			playerId |= (Random() & BIT_FLANK);
			enemyId |= (Random() & BIT_FLANK);
		}

		//The farther the "player" gets, the higher chance a stat will be raised more than 1
		u8 currStreak = GetCurrentBattleTowerStreak();
		if (currStreak < 35)
			increaseMax = 1;
		else if (currStreak < 50)
			increaseMax = 2;
		else if (currStreak < 65)
			increaseMax = 3;
		else if (currStreak < 80)
			increaseMax = 4;
		else if (currStreak < 100)
			increaseMax = 5;
		else
			increaseMax = 6;

		//Makes it so Contrary has no effect on the stat boost
		u8 contraryShiftPlayer = (ABILITY(playerId) == ABILITY_CONTRARY) ? 0x80 : 0;
		u8 contraryShiftEnemy = (ABILITY(enemyId) == ABILITY_CONTRARY) ? 0x80 : 0;

		increase = (Random() % increaseMax) + 1; //Player and enemy get the same amount of boost
		VarSet(VAR_TOTEM + playerId, playerStat | (increase * 0x10 + contraryShiftPlayer));
		VarSet(VAR_TOTEM + enemyId, enemyStat | (increase * 0x10 + contraryShiftEnemy));
	}
}

void CleanUpExtraTurnValues(void)
{
	gNewBS->NoMoreMovingThisTurn = 0;
	gNewBS->OriginalAttackerTargetCount = 0;
	gNewBS->ParentalBondOn = FALSE;
	gNewBS->DancerInProgress = FALSE;
	gNewBS->MoveBounceInProgress = FALSE;
	gNewBS->zMoveData.active = FALSE;
	gNewBS->dynamaxData.active = FALSE;
	gNewBS->batonPassing = FALSE;
	gNewBS->devolveForgotMove = 0; //Here and no in end turn because Pokemon can faint and devolve because of hazards after the end turn switch in
}

void SetActionsAndBanksTurnOrder(void)
{
	s32 turnOrderId = 0;
	s32 i, j;

	if (gBattleTypeFlags & BATTLE_TYPE_SAFARI)
	{
		for (gActiveBattler = 0; gActiveBattler < gBattlersCount; ++gActiveBattler)
		{
			gActionsByTurnOrder[turnOrderId] = gChosenActionByBank[gActiveBattler];
			gBanksByTurnOrder[turnOrderId] = gActiveBattler;
			++turnOrderId;
		}
	}
	else
	{
		if (gBattleTypeFlags & BATTLE_TYPE_LINK)
		{
			for (gActiveBattler = 0; gActiveBattler < gBattlersCount; ++gActiveBattler)
			{
				if (gChosenActionByBank[gActiveBattler] == ACTION_RUN)
				{
					turnOrderId = 5;
					break;
				}
			}
		}
		else
		{
			if (gChosenActionByBank[0] == ACTION_RUN)
			{
				gActiveBattler = 0;
				turnOrderId = 5;
			}
			else if (gChosenActionByBank[0] == ACTION_USE_ITEM)
			{
				gNewBS->playerItemUsedCount = MathMin(gNewBS->playerItemUsedCount + 1, 0xFF);
			}

			if (gChosenActionByBank[2] == ACTION_RUN)
			{
				gActiveBattler = 2;
				turnOrderId = 5;
			}
			else if (gChosenActionByBank[2] == ACTION_USE_ITEM && !IsTagBattle())
			{
				gNewBS->playerItemUsedCount = MathMin(gNewBS->playerItemUsedCount + 1, 0xFF);
			}
		}

		if (turnOrderId == 5)
		{
			gActionsByTurnOrder[0] = gChosenActionByBank[gActiveBattler];
			gBanksByTurnOrder[0] = gActiveBattler;
			turnOrderId = 1;
			for (i = 0; i < gBattlersCount; i++)
			{
				if (i != gActiveBattler)
				{
					gActionsByTurnOrder[turnOrderId] = gChosenActionByBank[i];
					gBanksByTurnOrder[turnOrderId] = i;
					++turnOrderId;
				}
			}
			gBattleMainFunc = CheckFocusPunch_ClearVarsBeforeTurnStarts;
			gBattleStruct->focusPunchBank = 0;
			return;
		}
		else
		{
			//First come switches
			for (gActiveBattler = 0; gActiveBattler < gBattlersCount; ++gActiveBattler)
			{
				if (gChosenActionByBank[gActiveBattler] == ACTION_SWITCH)
				{
					gActionsByTurnOrder[turnOrderId] = gChosenActionByBank[gActiveBattler];
					gBanksByTurnOrder[turnOrderId] = gActiveBattler;
					++turnOrderId;
				}
			}

			//Then AI uses items
			for (gActiveBattler = 0; gActiveBattler < gBattlersCount; ++gActiveBattler)
			{
				if (gChosenActionByBank[gActiveBattler] == ACTION_USE_ITEM)
				{
					gNewBS->ai.switchesInARow[gActiveBattler] = 0; //Wipe since didn't switch this turn
					gNewBS->ai.previousMonIn[gActiveBattler] = 0xFF;
					gActionsByTurnOrder[turnOrderId] = gChosenActionByBank[gActiveBattler];
					gBanksByTurnOrder[turnOrderId] = gActiveBattler;
					++turnOrderId;
				}
			}

			//Then everything else
			for (gActiveBattler = 0; gActiveBattler < gBattlersCount; gActiveBattler++)
			{
				if (gChosenActionByBank[gActiveBattler] != ACTION_USE_ITEM && gChosenActionByBank[gActiveBattler] != ACTION_SWITCH)
				{
					gNewBS->ai.switchesInARow[gActiveBattler] = 0; //Wipe since didn't switch this turn
					gNewBS->ai.previousMonIn[gActiveBattler] = 0xFF;
					gActionsByTurnOrder[turnOrderId] = gChosenActionByBank[gActiveBattler];
					gBanksByTurnOrder[turnOrderId] = gActiveBattler;
					++turnOrderId;
				}
			}

			for (i = 0; i < gBattlersCount - 1; i++)
			{
				for (j = i + 1; j < gBattlersCount; j++)
				{
					u8 bank1 = gBanksByTurnOrder[i];
					u8 bank2 = gBanksByTurnOrder[j];
					if (gActionsByTurnOrder[i] != ACTION_USE_ITEM
						&& gActionsByTurnOrder[j] != ACTION_USE_ITEM
						&& gActionsByTurnOrder[i] != ACTION_SWITCH
						&& gActionsByTurnOrder[j] != ACTION_SWITCH)
					{
						if (GetWhoStrikesFirst(bank1, bank2, FALSE))
							SwapTurnOrder(i, j);
					}
				}
			}
		}
	}

	gBattleMainFunc = CheckFocusPunch_ClearVarsBeforeTurnStarts;
	gBattleStruct->focusPunchBank = 0;
	gCurrentMove = 0;
}

enum MegaStates
{
	Mega_Check,
	Mega_CalcTurnOrder,
	Mega_SwitchInAbilities,
	Mega_Intimidate,
	Mega_End
};

void RunTurnActionsFunctions(void)
{
	int i, j;
	u8 effect, savedActionFuncId;
	u8* megaBank = &(gNewBS->megaData.activeBank);

	if (gBattleOutcome != 0)
		gCurrentActionFuncId = ACTION_FINISHED;
	
	savedActionFuncId = gCurrentActionFuncId;

	if (!gNewBS->activatedCustapQuickClaw)
	{
		for (i = 0; i < gBattlersCount; ++i) //Loop through all battlers and play Quick Claw anim for each
		{
			u8 bank = gBanksByTurnOrder[i];
			u8 action = gActionsByTurnOrder[i];

			if (gNewBS->quickDrawIndicator & gBitTable[bank])
			{
				gNewBS->quickDrawIndicator &= ~(gBitTable[bank]);
				gNewBS->quickClawCustapIndicator &= ~(gBitTable[bank]); //One or the other

				if (action == ACTION_USE_ITEM || action == ACTION_SWITCH || action == ACTION_RUN)
					continue;

				gBattleScripting.bank = bank;
				BattleScriptExecute(BattleScript_QuickDraw);
				gCurrentActionFuncId = savedActionFuncId;
				return;
			}
			else if (gNewBS->quickClawCustapIndicator & gBitTable[bank])
			{
				gNewBS->quickClawCustapIndicator &= ~(gBitTable[bank]);
				gNewBS->quickDrawIndicator &= ~(gBitTable[bank]); //One or the other

				if (action == ACTION_USE_ITEM)
					continue;
				else if (ITEM_EFFECT(bank) == ITEM_EFFECT_CUSTAP_BERRY
				&& (action == ACTION_USE_ITEM || action == ACTION_SWITCH || action == ACTION_RUN)) //Only Quick Claw activates on the switch
					continue;

				gBattleScripting.bank = bank;
				gLastUsedItem = ITEM(bank);
				if (ITEM_EFFECT(bank) != ITEM_EFFECT_CUSTAP_BERRY)
					RecordItemEffectBattle(bank, ITEM_EFFECT(bank));
				else
					gNewBS->ateCustapBerry |= gBitTable[bank];

				BattleScriptExecute(BattleScript_QuickClaw);
				gCurrentActionFuncId = savedActionFuncId;
				return;
			}
		}
	}

	gNewBS->activatedCustapQuickClaw = TRUE; //So the animation only plays once

	//Try to Mega Evolve/Ultra Burst Pokemon
	switch (gNewBS->megaData.state) {
		case Mega_Check:
			for (i = *megaBank; i < gBattlersCount; ++i, ++*megaBank)
			{
				u8 bank = gActiveBattler = gBanksByTurnOrder[i];
				if (gNewBS->megaData.chosen[bank]
				&& !gNewBS->megaData.done[bank]
				&& !DoesZMoveUsageStopMegaEvolution(bank)
				&& (gCurrentActionFuncId == ACTION_USE_MOVE
				 || (gCurrentActionFuncId == ACTION_SWITCH && gChosenMovesByBanks[bank] == MOVE_PURSUIT)))
				{
					const u8* script = DoMegaEvolution(bank);
					if (script != NULL)
					{
						if (!(gBattleTypeFlags & BATTLE_TYPE_MEGA_BRAWL)) //As many mons can Mega Evolve as you want
							gNewBS->megaData.done[bank] = TRUE;

						gNewBS->megaData.chosen[bank] = 0;
						gNewBS->megaData.megaEvoInProgress = TRUE;
						gNewBS->megaData.script = script;
						if (!(gBattleTypeFlags & (BATTLE_TYPE_INGAME_PARTNER | BATTLE_TYPE_MULTI | BATTLE_TYPE_MEGA_BRAWL))
						&& SIDE(bank) == B_SIDE_PLAYER)
						{
							gNewBS->megaData.chosen[PARTNER(bank)] = 0;
							gNewBS->megaData.done[PARTNER(bank)] = TRUE;
						}
						else if (!(gBattleTypeFlags & (BATTLE_TYPE_TWO_OPPONENTS | BATTLE_TYPE_MULTI | BATTLE_TYPE_MEGA_BRAWL))
						&& SIDE(bank) == B_SIDE_OPPONENT)
						{
							gNewBS->megaData.chosen[PARTNER(bank)] = 0;
							gNewBS->megaData.done[PARTNER(bank)] = TRUE;
						}
						RecordItemEffectBattle(bank, ITEM_EFFECT_MEGA_STONE);
						BattleScriptExecute(gNewBS->megaData.script);
						gCurrentActionFuncId = savedActionFuncId;
						return;
					}
				}
				else if (gNewBS->ultraData.chosen[bank] && !gNewBS->ultraData.done[bank])
				{
					const u8* script = DoMegaEvolution(bank);
					if (script != NULL)
					{
						if (!(gBattleTypeFlags & BATTLE_TYPE_MEGA_BRAWL)) //As many mons can Mega Evolve as you want
							gNewBS->ultraData.done[bank] = TRUE;

						gNewBS->ultraData.chosen[bank] = 0;
						gNewBS->megaData.megaEvoInProgress = TRUE;
						gNewBS->megaData.script = script;
						if (!(gBattleTypeFlags & (BATTLE_TYPE_INGAME_PARTNER | BATTLE_TYPE_MULTI | BATTLE_TYPE_MEGA_BRAWL))
						&& SIDE(bank) == B_SIDE_PLAYER)
						{
							gNewBS->ultraData.chosen[PARTNER(bank)] = 0;
							gNewBS->ultraData.done[PARTNER(bank)] = TRUE;
						}
						else if (!(gBattleTypeFlags & (BATTLE_TYPE_TWO_OPPONENTS | BATTLE_TYPE_MULTI | BATTLE_TYPE_MEGA_BRAWL))
						&& SIDE(bank) == B_SIDE_OPPONENT)
						{
							gNewBS->ultraData.chosen[PARTNER(bank)] = 0;
							gNewBS->ultraData.done[PARTNER(bank)] = TRUE;
						}
						RecordItemEffectBattle(bank, ITEM_EFFECT_MEGA_STONE);
						BattleScriptExecute(gNewBS->megaData.script);
						gCurrentActionFuncId = savedActionFuncId;
						return;
					}
				}
			}
			if (gNewBS->megaData.megaEvoInProgress)
				++gNewBS->megaData.state;
			else
				gNewBS->megaData.state = Mega_End;
			return;

		case Mega_CalcTurnOrder:
			for (i = 0; i < gBattlersCount - 1; ++i)
			{
				for (j = i + 1; j < gBattlersCount; ++j)
				{
					u8 bank1 = gBanksByTurnOrder[i];
					u8 bank2 = gBanksByTurnOrder[j];
					if (gActionsByTurnOrder[i] != ACTION_USE_ITEM
						&& gActionsByTurnOrder[j] != ACTION_USE_ITEM
						&& gActionsByTurnOrder[i] != ACTION_SWITCH
						&& gActionsByTurnOrder[j] != ACTION_SWITCH
						&& gActionsByTurnOrder[i] != ACTION_FINISHED
						&& gActionsByTurnOrder[j] != ACTION_FINISHED)
					{
						if (GetWhoStrikesFirst(bank1, bank2, FALSE))
							SwapTurnOrder(i, j);
					}
				}
			}
			*megaBank = 0; //Reset the bank for the next loop
			++gNewBS->megaData.state;
			return;

		case Mega_SwitchInAbilities:
			while (*megaBank < gBattlersCount) {
				if (BATTLER_ALIVE(gBanksByTurnOrder[*megaBank])
				&& AbilityBattleEffects(ABILITYEFFECT_ON_SWITCHIN, gBanksByTurnOrder[*megaBank], 0, 0, 0))
					++effect;
				++*megaBank;

				if (effect) return;
			}
			*megaBank = 0;
			++gNewBS->megaData.state;
			return;

		case Mega_Intimidate:
			gNewBS->megaData.script = 0;
			gNewBS->megaData.state = Mega_End;
			gNewBS->megaData.activeBank = 0;
			gNewBS->megaData.megaEvoInProgress = FALSE;
			//Fallthrough
		case Mega_End:
			if (gCurrentActionFuncId != ACTION_USE_MOVE && gCurrentActionFuncId != ACTION_RUN_BATTLESCRIPT) //Necessary because of Mega Evolving before Pursuit
				gNewBS->megaData.state = 0; //Reset since not everyone may have had a chance to Mega Evolve
	}

	*megaBank = 0;

	if (gCurrentActionFuncId == ACTION_USE_MOVE)
	{
		//Try to Dynamax/Gigantamax Pokemon
		for (i = 0; i < gBattlersCount; ++i)
		{
			u8 bank = gActiveBattler = gBanksByTurnOrder[i];

			if (gNewBS->dynamaxData.toBeUsed[bank]
			&& !gNewBS->dynamaxData.used[bank]
			&& !DoesZMoveUsageStopMegaEvolution(bank)) //Same for Dynamax
			{
				const u8* script = GetDynamaxScript(bank);
				if (script != NULL)
				{
					gNewBS->dynamaxData.toBeUsed[bank] = FALSE;
					gNewBS->dynamaxData.used[bank] = TRUE;
					gNewBS->dynamaxData.timer[bank] = 3; //Dynamax lasts for 3 turns
					gNewBS->dynamaxData.partyIndex[SIDE(bank)] = gBitTable[gBattlerPartyIndexes[bank]];

					if (SIDE(bank) == B_SIDE_PLAYER)
					{
						gNewBS->dynamaxData.toBeUsed[PARTNER(bank)] = FALSE;
						gNewBS->dynamaxData.used[PARTNER(bank)] = TRUE;
					}
					else
					{
						gNewBS->dynamaxData.toBeUsed[PARTNER(bank)] = FALSE;
						gNewBS->dynamaxData.used[PARTNER(bank)] = TRUE;
					}

					BattleScriptExecute(script);
					gCurrentActionFuncId = savedActionFuncId;

					if (IS_BEHIND_SUBSTITUTE(bank))
					{
						BattleScriptPushCursor();
						gBankTarget = bank;
						gBattleMons[gActiveBattler].status2 &= ~(STATUS2_SUBSTITUTE);
						gDisableStructs[gActiveBattler].substituteHP = 0;
						gBattlescriptCurrInstr = BattleScript_SubstituteFade;
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_FlushMessageBox;
					}
					return;
				}
			}
		}
	
		while (gBattleStruct->focusPunchBank < gBattlersCount)
		{
			gActiveBattler = gBanksByTurnOrder[gBattleStruct->focusPunchBank];
			++gBattleStruct->focusPunchBank;
			u16 chosenMove = gActionsByTurnOrder[gActiveBattler] == ACTION_USE_MOVE ? gChosenMovesByBanks[gActiveBattler] : MOVE_NONE;
			if ((chosenMove == MOVE_FOCUSPUNCH || chosenMove == MOVE_BEAKBLAST || chosenMove == MOVE_SHELLTRAP)
			&& !(gBattleMons[gActiveBattler].status1 & STATUS1_SLEEP)
			&& !(gDisableStructs[gActiveBattler].truantCounter)
			&& !(gProtectStructs[gActiveBattler].onlyStruggle)
			&& !gNewBS->zMoveData.toBeUsed[gActiveBattler]
			&& !IsDynamaxed(gActiveBattler))
			{
				gBankAttacker = gBattleScripting.bank = gActiveBattler;
				if (chosenMove == MOVE_BEAKBLAST && !(gNewBS->BeakBlastByte & gBitTable[gActiveBattler]))
				{
					gNewBS->BeakBlastByte |= gBitTable[gActiveBattler];
					BattleScriptExecute(BattleScript_BeakBlastSetUp);
				}
				else if (chosenMove == MOVE_SHELLTRAP && !(gNewBS->playedShellTrapMessage & gBitTable[gActiveBattler]))
				{
					gNewBS->playedShellTrapMessage |= gBitTable[gActiveBattler];
					BattleScriptExecute(BattleScript_ShellTrapSetUp);
				}
				else if (!(gNewBS->playedFocusPunchMessage & gBitTable[gActiveBattler]))
				{
					gNewBS->playedFocusPunchMessage |= gBitTable[gActiveBattler];
					BattleScriptExecute(BattleScript_FocusPunchSetUp);
				}
				return;
			}
		}
	}

	gBattleStruct->savedTurnActionNumber = gCurrentTurnActionNumber;
	sTurnActionsFuncsTable[gCurrentActionFuncId]();

	TrySetupRaidBossRepeatedAttack(savedActionFuncId);

	if (gCurrentTurnActionNumber >= gBattlersCount) // everyone did their actions, turn finished
	{
		gHitMarker &= ~(HITMARKER_NON_ATTACK_DMG);
		gBattleMainFunc = sEndTurnFuncsTable[gBattleOutcome & 0x7F];
	}
	else
	{
		if (gBattleStruct->savedTurnActionNumber != gCurrentTurnActionNumber) // action turn has been done, clear hitmarker bits for another bank
		{
			gHitMarker &= ~(HITMARKER_NO_ATTACKSTRING);
			gHitMarker &= ~(HITMARKER_UNABLE_TO_USE_MOVE);
		}
	}
}

void HandleAction_UseMove(void)
{
	u8 side;
	u8 moveType;
	int i, j;

	if (!gNewBS->PledgeHelper) //Don't recalculate during pledge execution
	{
		//Recalculate turn order before each attack
		for (i = gCurrentTurnActionNumber; i < gBattlersCount - 1; ++i)
		{
			for (j = i + 1; j < gBattlersCount; ++j)
			{
				u8 bank1 = gBanksByTurnOrder[i];
				u8 bank2 = gBanksByTurnOrder[j];
				if (gActionsByTurnOrder[i] != ACTION_USE_ITEM
					&& gActionsByTurnOrder[j] != ACTION_USE_ITEM
					&& gActionsByTurnOrder[i] != ACTION_SWITCH
					&& gActionsByTurnOrder[j] != ACTION_SWITCH
					&& gActionsByTurnOrder[i] != ACTION_FINISHED
					&& gActionsByTurnOrder[j] != ACTION_FINISHED
					&& !(gNewBS->turnOrderLocked & gBitTable[bank1])
					&& !(gNewBS->turnOrderLocked & gBitTable[bank2]))
				{
					if (GetWhoStrikesFirstUseLastBracketCalc(bank1, bank2))
						SwapTurnOrder(i, j);
				}
			}
		}
	}

	gBankAttacker = gBanksByTurnOrder[gCurrentTurnActionNumber];

	if (gBattleStruct->field_91 & gBitTable[gBankAttacker])
	{
		gCurrentActionFuncId = ACTION_FINISHED;
		return;
	}

	gHpDealt = 0;
	gCritMultiplier = BASE_CRIT_MULTIPLIER;
	gBattleScripting.dmgMultiplier = 1;
	gBattleStruct->atkCancellerTracker = 0;
	gMoveResultFlags = 0;
	gMultiHitCounter = 0;
	gNewBS->OriginalAttackerTargetCount = 0;
	gNewBS->ParentalBondOn = FALSE;
	gNewBS->DancerInProgress = FALSE;
	gNewBS->MoveBounceInProgress = FALSE;
	gNewBS->breakDisguiseSpecialDmg = FALSE;
	gNewBS->dontActivateMoldBreakersAnymoreThisTurn = FALSE;
	gNewBS->printedStrongWindsWeakenedAttack = FALSE;
	gNewBS->cramorantTransformed = FALSE;
	gNewBS->zMoveData.active = FALSE;
	gNewBS->batonPassing = FALSE;
	gNewBS->dynamaxData.nullifiedStats = FALSE;
	gNewBS->dynamaxData.attackAgain = FALSE;
	gBattleCommunication[MOVE_EFFECT_BYTE] = 0; //Remove secondary effects
	gBattleCommunication[6] = 0;
	gCurrMovePos = gChosenMovePos = gBattleStruct->chosenMovePositions[gBankAttacker];

	gNewBS->totalDamageGiven = 0;
	gNewBS->selfInflictedDamage = 0;
	gNewBS->enduredDamage = 0;
	gNewBS->lessThanHalfHPBeforeShellBell = FALSE;
	ResetDoublesSpreadMoveCalcs(); //Clear spread move things

	for (int i = 0; i < MAX_BATTLERS_COUNT; ++i)
	{
		gNewBS->DamageTaken[i] = 0;
		gNewBS->turnDamageTaken[i] = 0;
		gNewBS->criticalMultiplier[i] = 0;
		gNewBS->ResultFlags[i] = 0;
		gNewBS->missStringId[i] = 0;
		gNewBS->noResultString[i] = 0;
		gNewBS->statFellThisTurn[i] = FALSE;
	}

	if (IsRaidBattle())
		gNewBS->dynamaxData.turnStartHP = gBattleMons[BANK_RAID_BOSS].hp;

//Get Move to be Used
	if (gProtectStructs[gBankAttacker].onlyStruggle)
	{
		gProtectStructs[gBankAttacker].onlyStruggle = 0;
		gCurrentMove = gChosenMove = MOVE_STRUGGLE;
		gHitMarker |= HITMARKER_NO_PPDEDUCT;
		gBattleStruct->moveTarget[gBankAttacker] = GetMoveTarget(MOVE_STRUGGLE, 0);
	}
	else if (gBattleMons[gBankAttacker].status2 & STATUS2_RECHARGE)
	{
		gChosenMovesByBanks[gBankAttacker] = gCurrentMove = gChosenMove = gLockedMoves[gBankAttacker];
	}
	else if (gBattleMons[gBankAttacker].status2 & STATUS2_MULTIPLETURNS)
	{
		gChosenMovesByBanks[gBankAttacker] = gCurrentMove = gChosenMove = gLockedMoves[gBankAttacker];
	}
	// Encore forces you to use the same move
	else if (gDisableStructs[gBankAttacker].encoredMove != MOVE_NONE
		  && gDisableStructs[gBankAttacker].encoredMove == gBattleMons[gBankAttacker].moves[gDisableStructs[gBankAttacker].encoredMovePos]
		  && !gNewBS->zMoveData.toBeUsed[gBankAttacker] //If a Z-Move was chosen, it can still be used
		  && !gNewBS->dynamaxData.active)
	{
		gChosenMove = gBattleMons[gBankAttacker].moves[gCurrMovePos];
		if (gChosenMove != gDisableStructs[gBankAttacker].encoredMove) //The encored move wasn't chosen
			gBattleStruct->moveTarget[gBankAttacker] = GetMoveTarget(gDisableStructs[gBankAttacker].encoredMove, 0); //Get correct target

		gCurrentMove = gChosenMove = gDisableStructs[gBankAttacker].encoredMove;
		gCurrMovePos = gChosenMovePos = gDisableStructs[gBankAttacker].encoredMovePos;
	}
	// Check if the encored move wasn't overwritten
	else if (gDisableStructs[gBankAttacker].encoredMove != MOVE_NONE
		  && gDisableStructs[gBankAttacker].encoredMove != gBattleMons[gBankAttacker].moves[gDisableStructs[gBankAttacker].encoredMovePos]
		  && !gNewBS->zMoveData.toBeUsed[gBankAttacker] //If a Z-Move was chosen, it can still be used
		  && !gNewBS->dynamaxData.active)
	{
		gCurrMovePos = gChosenMovePos = gDisableStructs[gBankAttacker].encoredMovePos;
		gCurrentMove = gChosenMove = gBattleMons[gBankAttacker].moves[gCurrMovePos];
		gDisableStructs[gBankAttacker].encoredMove = MOVE_NONE;
		gDisableStructs[gBankAttacker].encoredMovePos = 0;
		gDisableStructs[gBankAttacker].encoreTimer = 0;
		gBattleStruct->moveTarget[gBankAttacker] = GetMoveTarget(gCurrentMove, 0);
	}
	else if (gBattleMons[gBankAttacker].moves[gCurrMovePos] != gChosenMovesByBanks[gBankAttacker])
	{
		gCurrentMove = gChosenMove = gBattleMons[gBankAttacker].moves[gCurrMovePos];
		gBattleStruct->moveTarget[gBankAttacker] = GetMoveTarget(gCurrentMove, 0);
	}
	else
	{
		gCurrentMove = gChosenMove = gBattleMons[gBankAttacker].moves[gCurrMovePos];
	}

	if (gBattleMons[gBankAttacker].hp)
	{
		if (SIDE(gBankAttacker) == B_SIDE_PLAYER)
			gBattleResults.lastUsedMovePlayer = gCurrentMove;
		else
			gBattleResults.lastUsedMoveOpponent = gCurrentMove;
	}

	if (gNewBS->zMoveData.toBeUsed[gBankAttacker]/* && !gNewBS->zMoveData.used[gBankAttacker]*/)
	{
		gNewBS->zMoveData.active = TRUE;

		if (SPLIT(gCurrentMove) != SPLIT_STATUS)
		{
			u16 zmove = GetSpecialZMove(gCurrentMove, SPECIES(gBankAttacker), ITEM(gBankAttacker));
			if (zmove != MOVE_NONE && zmove != 0xFFFF) //There's a special Z-Move
				gCurrentMove = zmove;
			else if (zmove != 0xFFFF) //This check is needed b/c in Benjamin Butterfree you can select a special Z-Move but then lose it before it activates
				gCurrentMove = GetTypeBasedZMove(gBattleMons[gBankAttacker].moves[gCurrMovePos], gBankAttacker);
			else
			{
				gNewBS->zMoveData.active = FALSE;
				gNewBS->zMoveData.toBeUsed[gBankAttacker] = FALSE;
			}
		}
		else
		{
			//This check is needed b/c in Benjamin Butterfree you can select a special Z-Move but then lose it before it activates
			if (GetSpecialZMove(gCurrentMove, SPECIES(gBankAttacker), ITEM(gBankAttacker)) == 0xFFFF)
			{
				gNewBS->zMoveData.active = FALSE;
				gNewBS->zMoveData.toBeUsed[gBankAttacker] = FALSE;
			}
		}
	}
	else if (IsDynamaxed(gBankAttacker))
	{
		if (IsRaidBattle() && gBankAttacker == BANK_RAID_BOSS)
		{
			u8 split = SPLIT(gCurrentMove);
			bool8 isBannedMove = gSpecialMoveFlags[gCurrentMove].gRaidBattleBannedRaidMonMoves
							  || gSpecialMoveFlags[gCurrentMove].gRaidBattleBannedMoves
							  || gBattleMoves[gCurrentMove].effect == EFFECT_BIDE //Bide should always be executed as Max Strike
							  || IsUnusableMove(gCurrentMove, gBankAttacker, 0xFF, 1, ABILITY(gBankAttacker), ITEM_EFFECT(gBankAttacker), CHOICED_MOVE(gBankAttacker));

			if (isBannedMove && split != SPLIT_STATUS) //Use banned status move - don't use Max Guard
			{
				if (gBattleMoves[gCurrentMove].effect == EFFECT_BIDE)
					gBattleStruct->moveTarget[gBankAttacker] = GetMoveTarget(GetMaxMove(gBankAttacker, gCurrMovePos), FALSE); //Fix self-targeting

				goto TURN_MOVE_INTO_MAX_MOVE;
			}
			else if (IsRaidBossUsingRegularMove(gBankAttacker, gCurrentMove))
			{
				//Samll chance to use regular damaging move
				//Raid wild Pokemon shouldn't be using Max Guard
			}
			else
				goto TURN_MOVE_INTO_MAX_MOVE;
		}
		else if (gCurrentMove != MOVE_STRUGGLE)
		{
			TURN_MOVE_INTO_MAX_MOVE:
			gNewBS->dynamaxData.active = TRUE;
			gCurrentMove = GetMaxMove(gBankAttacker, gCurrMovePos);
			if (gCurrentMove == MOVE_MAX_GUARD)
				gBattleStruct->moveTarget[gBankAttacker] = gBankAttacker; //Fix target to self
		}
	}

	gBattleStruct->dynamicMoveType = GetMoveTypeSpecial(gBankAttacker, gCurrentMove);
	moveType = gBattleStruct->dynamicMoveType;

	TryChangeMoveTargetToCounterPlayerProtectCheese();

//Get Move Target
	u8 atkAbility = ABILITY(gBankAttacker);
	u8 moveTarget = GetBaseMoveTarget(gCurrentMove, gBankAttacker);
	side = SIDE(gBankAttacker) ^ BIT_SIDE;
	bank_t selectedTarget = gBattleStruct->moveTarget[gBankAttacker];

	if ((moveTarget == MOVE_TARGET_SELECTED || moveTarget == MOVE_TARGET_RANDOM)
	&& IsMoveRedirectedByFollowMe(gCurrentMove, gBankAttacker, side))
	{
		gBankTarget = gSideTimers[side].followmeTarget;
	}
	else if (IS_DOUBLE_BATTLE
		  && gSideTimers[side].followmeTimer == 0
		  && !IsMoveRedirectionPrevented(gCurrentMove, atkAbility)
		  && (SPLIT(gCurrentMove) != SPLIT_STATUS || moveTarget != MOVE_TARGET_USER)
		  && !(moveTarget & (MOVE_TARGET_ALL | MOVE_TARGET_BOTH))
		  && NO_MOLD_BREAKERS(ABILITY(gBankAttacker), gCurrentMove))
	{ //Try Ability Redirection
		switch (moveType) {
			case TYPE_WATER:
				if (ABILITY(selectedTarget) != ABILITY_STORMDRAIN)
				{
					if (ABILITY(SIDE(gBankAttacker) ^ BIT_SIDE) == ABILITY_STORMDRAIN)
					{
						gBankTarget = SIDE(gBankAttacker) ^ BIT_SIDE;
						gSpecialStatuses[gBankTarget].lightningRodRedirected = 1;
					}
					else if (ABILITY(PARTNER(SIDE(gBankAttacker) ^ BIT_SIDE)) == ABILITY_STORMDRAIN)
					{
						gBankTarget = PARTNER(SIDE(gBankAttacker) ^ BIT_SIDE);
						gSpecialStatuses[gBankTarget].lightningRodRedirected = 1;
					}
					else if (ABILITY(PARTNER(gBankAttacker)) == ABILITY_STORMDRAIN)
					{
						gBankTarget = PARTNER(gBankAttacker);
						gSpecialStatuses[gBankTarget].lightningRodRedirected = 1;
					}
				}

				break;

			case TYPE_ELECTRIC:
				if (ABILITY(selectedTarget) != ABILITY_LIGHTNINGROD)
				{
					if (ABILITY(SIDE(gBankAttacker) ^ BIT_SIDE) == ABILITY_LIGHTNINGROD)
					{
						gBankTarget = SIDE(gBankAttacker) ^ BIT_SIDE;
						gSpecialStatuses[gBankTarget].lightningRodRedirected = 1;
					}
					else if (ABILITY(PARTNER(SIDE(gBankAttacker) ^ BIT_SIDE)) == ABILITY_LIGHTNINGROD)
					{
						gBankTarget = PARTNER(SIDE(gBankAttacker) ^ BIT_SIDE);
						gSpecialStatuses[gBankTarget].lightningRodRedirected = 1;
					}
					else if (ABILITY(PARTNER(gBankAttacker)) == ABILITY_LIGHTNINGROD)
					{
						gBankTarget = PARTNER(gBankAttacker);
						gSpecialStatuses[gBankTarget].lightningRodRedirected = 1;
					}
				}
				break;
		}

		if (!gSpecialStatuses[gBankTarget].lightningRodRedirected)
		{
			if (moveTarget & MOVE_TARGET_RANDOM
			&& !IsAnyMaxMove(gCurrentMove))
				goto CHOOSE_RANDOM_TARGET_DOUBLES;
			else
				goto CHOOSE_REGULAR_TARGET_DOUBLES;
		}
	}
	else if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE
		  && moveTarget & MOVE_TARGET_RANDOM
		  && !IsAnyMaxMove(gCurrentMove))
	{
	CHOOSE_RANDOM_TARGET_DOUBLES:
		if (SIDE(gBankAttacker) == B_SIDE_PLAYER)
		{
			if (IsRaidBattle() || Random() & 1)
				gBankTarget = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);
			else
				gBankTarget = GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT);
		}
		else
		{
			if (Random() & 1)
				gBankTarget = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);
			else
				gBankTarget = GetBattlerAtPosition(B_POSITION_PLAYER_RIGHT);
		}

		if (gAbsentBattlerFlags & gBitTable[gBankTarget]
		&& SIDE(gBankAttacker) != SIDE(gBankTarget))
		{
			gBankTarget = GetBattlerAtPosition(PARTNER(gBankTarget));
		}
	}
	else if (IS_DOUBLE_BATTLE && moveTarget & MOVE_TARGET_ALL)
	{
		DetermineFirstMultiTarget();
	}
	else if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
	{
	CHOOSE_REGULAR_TARGET_DOUBLES:
		gBankTarget = selectedTarget;
		if (gAbsentBattlerFlags & gBitTable[gBankTarget])
		{
			if (gBattleMoves[gCurrentMove].target & MOVE_TARGET_USER_OR_PARTNER) //Acupressure
			{
				gBankTarget = gBankAttacker; //Redirect to user instead
			}
			else if (SIDE(gBankAttacker) != SIDE(gBankTarget))
			{
				gBankTarget = PARTNER(gBankTarget);
			}
			else //Targeted Partner
			{
				if (gCurrentMove == MOVE_HEALPULSE || gCurrentMove == MOVE_INSTRUCT)
				{
					//Never redirect Heal Pulse or Instruct to the enemy
					goto FAIL_NO_TARGET;
				}
				else
				{
					gBankTarget = GetBattlerAtPosition(GetBattlerPosition(gBankAttacker) ^ BIT_SIDE);
					if (gAbsentBattlerFlags & gBitTable[gBankTarget])
						gBankTarget = PARTNER(gBankTarget);
				}
			}
		}
	}
	else
		gBankTarget = selectedTarget;

	// choose battlescript
	if (gStatuses3[gBankAttacker] & STATUS3_SKY_DROP_ATTACKER
	&& !BATTLER_ALIVE(gNewBS->skyDropAttackersTarget[gBankAttacker]))
	{
		gStatuses3[gBankAttacker] &= ~(STATUS3_SKY_DROP_ATTACKER | STATUS3_SKY_DROP_TARGET | STATUS3_IN_AIR);
		gNewBS->skyDropTargetsAttacker[gBankTarget] = 0;
		gNewBS->skyDropAttackersTarget[gBankAttacker] = 0;
		gBattlescriptCurrInstr = BattleScript_NoTargetMoveFailed;
	}
	else if (!BATTLER_ALIVE(gBankTarget)
	&& AttacksThisTurn(gBankAttacker, gCurrentMove) == 2 //Not charging move
	&& gBattleMoves[gCurrentMove].effect != EFFECT_EXPLOSION //Exploding moves still KO the attacker
	&& !(moveTarget & MOVE_TARGET_OPPONENTS_FIELD) //Moves like Stealth Rock can still be used
	&& !(SPLIT(gCurrentMove) == SPLIT_STATUS && moveTarget & MOVE_TARGET_DEPENDS)) //Status moves like Metronome can still be used
	{
		FAIL_NO_TARGET:
		CancelMultiTurnMoves(gBankAttacker);
		gBattlescriptCurrInstr = BattleScript_NoTargetMoveFailed;
	}
	else
		gBattlescriptCurrInstr = gBattleScriptsForMoveEffects[gBattleMoves[gCurrentMove].effect];

	gCurrentActionFuncId = ACTION_RUN_BATTLESCRIPT;
}

void DetermineFirstMultiTarget(void)
{
	while ((gBankTarget = GetNextMultiTarget()) != 0xFF && gBattleMons[gBankTarget].hp == 0)
	{
		++gNewBS->OriginalAttackerTargetCount;
	}

	if (gBankTarget == 0xFF) //No targets left
		gBankTarget = FOE(gBankAttacker); //Doesn't matter who, as long as not attacker
}

static void TrySetupRaidBossRepeatedAttack(u8 actionFuncId)
{
	if (IsRaidBattle() && gNewBS->dynamaxData.attackAgain && gNewBS->dynamaxData.repeatedAttacks < 2 && actionFuncId == ACTION_FINISHED)
	{
		u8 i, moveLimitations, viableMoves;
		gNewBS->dynamaxData.attackAgain = FALSE;

		gBankAttacker = gBanksByTurnOrder[gCurrentTurnActionNumber - 1]; //Get original attacker

		if (gBankAttacker != BANK_RAID_BOSS //Just in case the player KOs the partner and sets the bit
		|| CountAliveMonsInBattle(BATTLE_ALIVE_DEF_SIDE, gBankAttacker, FOE(gBankAttacker)) == 0) //Don't attack again if no one left to hit
			return;

		moveLimitations = CheckMoveLimitations(gBankAttacker, 0, 0xFF);

		for (i = 0, viableMoves = 0; i < MAX_MON_MOVES; ++i)
		{
			if (gBattleMons[gBankAttacker].moves[i] == MOVE_NONE)
				continue;

			if (!(gBitTable[i] & moveLimitations)) //Move is usable
			{
				++viableMoves;
				break; //We only need at least one viable
			}
		}

		if (viableMoves == 0)
			return; //Don't force another move if the raid boss can't use one

		++gNewBS->dynamaxData.repeatedAttacks;
		gCurrentActionFuncId = gActionsByTurnOrder[--gCurrentTurnActionNumber] = ACTION_USE_MOVE;

		PickRaidBossRepeatedMove(moveLimitations);

		gHitMarker &= ~(HITMARKER_NO_ATTACKSTRING);
		gHitMarker &= ~(HITMARKER_UNABLE_TO_USE_MOVE);
	}
}

static u16 GetRandomBattleBGM(void)
{
	u16 song;

	do //Assumes table has legit music
	{
		song = gRandomBattleMusicOptions[Random() % gRandomBattleMusicOptionsLength];
	} while (song == 0); //Song is not legit

	return song;
}

u16 GetMUS_ForBattle(void)
{
	u16 song;

	if (gBattleTypeFlags & BATTLE_TYPE_LINK)
	{
		song = VarGet(VAR_BATTLE_FACILITY_SONG_OVERRIDE); //Check custom song
		if (song == BGM_RANDOM_BATTLE_MUSIC)
			song = GetRandomBattleBGM();

		if (song != 0)
			return song;

		#ifdef UNBOUND
			return BGM_BATTLE_BORRIUS_TRAINER;
		#else
			return BGM_BATTLE_RSE_TRAINER;
		#endif
	}

	if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)
	{
		u8 trainerClass;

		if (gBattleTypeFlags & BATTLE_TYPE_FRONTIER || IsFrontierTrainerId(gTrainerBattleOpponent_A))
		{
			//If either trainer is special, try to load their music
			song = TryGetSpecialFrontierTrainerMusic(gTrainerBattleOpponent_A, BATTLE_FACILITY_TRAINER_A);
			if (song != 0)
				return song;

			if (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS)
			{
				song = TryGetSpecialFrontierTrainerMusic(SECOND_OPPONENT, BATTLE_FACILITY_TRAINER_B);
				if (song != 0)
					return song;
			}

			//Then try to load class based music for either trainer
			trainerClass = GetFrontierTrainerClassId(gTrainerBattleOpponent_A, BATTLE_FACILITY_TRAINER_A);

			if (gClassBasedBattleBGM[trainerClass])
				return gClassBasedBattleBGM[trainerClass];

			if (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS)
			{
				trainerClass = GetFrontierTrainerClassId(SECOND_OPPONENT, BATTLE_FACILITY_TRAINER_B);

				if (gClassBasedBattleBGM[trainerClass])
					return gClassBasedBattleBGM[trainerClass];
			}

			if (gBattleTypeFlags & BATTLE_TYPE_FRONTIER)
			{
				//Then try loading the song override only in the actual Frontier
				song = VarGet(VAR_BATTLE_FACILITY_SONG_OVERRIDE);
				if (song == BGM_RANDOM_BATTLE_MUSIC)
					song = GetRandomBattleBGM();

				if (song != 0)
					return song;
			}
		}
		else
		{
			trainerClass = gTrainers[gTrainerBattleOpponent_A].trainerClass;

			if (gClassBasedBattleBGM[trainerClass])
				return gClassBasedBattleBGM[trainerClass];
			
			#ifdef UNBOUND
			if (gTrainerBattleOpponent_A == TRAINER_NEX)
				return BGM_BATTLE_SINNOH_TRAINER;
			#endif

			if (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS)
			{
				trainerClass = gTrainers[SECOND_OPPONENT].trainerClass;
				if (gClassBasedBattleBGM[trainerClass])
					return gClassBasedBattleBGM[trainerClass];
			}
		}

		#ifdef VAR_TRAINER_BGM_OVERRIDE
			song = VarGet(VAR_TRAINER_BGM_OVERRIDE);
			if (song != 0)
				return song;
		#endif

		#ifdef UNBOUND
			return BGM_BATTLE_BORRIUS_TRAINER;
		#else
			return BGM_BATTLE_TRAINER;
		#endif
	}

	u16 species = gEnemyParty[0].species;

	if (species < gWildSpeciesBasedBattleBGMLength
	&& gWildSpeciesBasedBattleBGM[species] != 0)
		return gWildSpeciesBasedBattleBGM[species];

	if (FlagGet(FLAG_DOUBLE_WILD_BATTLE)
	&& gEnemyParty[1].species != SPECIES_NONE
	&& gEnemyParty[1].species < gWildSpeciesBasedBattleBGMLength
	&& gWildSpeciesBasedBattleBGM[gEnemyParty[1].species] != 0)
	{
		return gWildSpeciesBasedBattleBGM[gEnemyParty[1].species];
	}

	#ifdef UNBOUND
		if (IsRaidBattle())
			return BGM_BATTLE_RAID_BOSS;

		#ifdef VAR_WILD_BGM_OVERRIDE
			song = VarGet(VAR_WILD_BGM_OVERRIDE);
			if (song != 0)
				return song;
		#endif

		u16 dexNum = SpeciesToNationalPokedexNum(species);
		if (dexNum >= NATIONAL_DEX_GROOKEY && dexNum < NATIONAL_DEX_CALYREX)
			return BGM_BATTLE_GALAR_WILD;

		return BGM_BATTLE_BORRIUS_WILD;
	#else
		if (IsRaidBattle())
			return BGM_BATTLE_LEGENDARY_BIRDS;

		#ifdef VAR_WILD_BGM_OVERRIDE
			song = VarGet(VAR_WILD_BGM_OVERRIDE);
			if (song != 0)
				return song;
		#endif

		return BGM_BATTLE_WILD;
	#endif
}

u16 LoadProperMusicForLinkBattles(void)
{
	gBattleTypeFlags |= BATTLE_TYPE_LINK;
	return GetMUS_ForBattle();
}

extern const u8 sBattleTransitionTable_Trainer[][2];
u8 GetTrainerBattleTransition(void)
{
	u8 minPartyCount, transitionType, enemyLevel, playerLevel;

	if (gTrainerBattleOpponent_A == TRAINER_SECRET_BASE)
		return B_TRANSITION_CHAMPION;

	#ifdef FR_PRE_BATTLE_MUGSHOT_STYLE
	if (gTrainers[gTrainerBattleOpponent_A].trainerClass == CLASS_CHAMPION)
		return B_TRANSITION_CHAMPION;

	if (gTrainers[gTrainerBattleOpponent_A].trainerClass == CLASS_ELITE_FOUR)
	{
		VarSet(VAR_PRE_BATTLE_MUGSHOT_STYLE, MUGSHOT_TWO_BARS);
		VarSet(VAR_PRE_BATTLE_MUGSHOT_SPRITE, MUGSHOT_PLAYER);

		if (gTrainerBattleOpponent_A == TRAINER_LORELEI
		||  gTrainerBattleOpponent_A == TRAINER_LORELEI_REMATCH)
			return B_TRANSITION_LORELEI;
		if (gTrainerBattleOpponent_A == TRAINER_BRUNO
		||  gTrainerBattleOpponent_A == TRAINER_BRUNO_REMATCH)
			return B_TRANSITION_BRUNO;
		if (gTrainerBattleOpponent_A == TRAINER_AGATHA
		||  gTrainerBattleOpponent_A == TRAINER_AGATHA_REMATCH)
			return B_TRANSITION_AGATHA;
		if (gTrainerBattleOpponent_A == TRAINER_LANCE
		||  gTrainerBattleOpponent_A == TRAINER_LANCE_REMATCH)
			return B_TRANSITION_LANCE;

		return B_TRANSITION_CHAMPION;
	}
	#endif

	#ifdef TUTORIAL_BATTLES
	if (Var8000 == 0xFEFE && sTrainerEventObjectLocalId >= 0x100)
		return B_TRANSITION_CHAMPION;
	#else
	if (sTrainerEventObjectLocalId >= 0x100) //Used for mugshots
		return B_TRANSITION_CHAMPION;
	#endif

	#ifdef VAR_BATTLE_TRANSITION_LOGO
	u16 transitionLogo = VarGet(VAR_BATTLE_TRANSITION_LOGO);
	if (transitionLogo == 0) //No preset logo
	{
		u8 trainerClass = GetFrontierTrainerClassId(gTrainerBattleOpponent_A, 0);

		for (u32 i = 0; i < gNumBattleTransitionLogos; ++i)
		{
			if (gBattleTransitionLogos[i].trainerClass != 0 //These logos must be set manually
			&& gBattleTransitionLogos[i].trainerClass == trainerClass)
			{
				transitionLogo = i;
				VarSet(VAR_BATTLE_TRANSITION_LOGO, transitionLogo); //Prep for later
				return B_TRANSITION_CUSTOM_LOGO;
			}
		}
	}
	else
		return B_TRANSITION_CUSTOM_LOGO;
	#endif

	if ((gTrainers[gTrainerBattleOpponent_A].doubleBattle == TRUE
	#ifdef FLAG_DOUBLE_BATTLE
	|| FlagGet(FLAG_DOUBLE_BATTLE)
	#endif
	) && ViableMonCount(gPlayerParty) >= 2)
		minPartyCount = 2; // double battles always at least have 2 pokemon.
	else
		minPartyCount = 1;

	transitionType = GetBattleTransitionTypeByMap();
	enemyLevel = GetSumOfEnemyPartyLevel(gTrainerBattleOpponent_A, minPartyCount);
	playerLevel = GetSumOfPlayerPartyLevel(minPartyCount); //Really only gets the sum of the at most the first two Pokemon on the player's team

	if (enemyLevel < playerLevel) //The player's first (or first two) Pokemon's level(s) is (are) >= than the opponent's first (or first two) Pokemon's level(s)
		return sBattleTransitionTable_Trainer[transitionType][0];
	else
		return sBattleTransitionTable_Trainer[transitionType][1];
}

// Determines which of the two given mons will strike first in a battle.
// Returns:
// 0 = first mon moves first
// 1 = second mon moves first
// 2 = second mon moves first because it won a 50/50 roll
u8 GetWhoStrikesFirst(u8 bank1, u8 bank2, bool8 ignoreMovePriorities)
{
	s8 bank1Priority, bank2Priority;
	s32 bank1Bracket, bank2Bracket;
	u32 bank1Spd, bank2Spd;

//Priority Calc
	if (!ignoreMovePriorities)
	{
		u16 move1 = ReplaceWithZMoveRuntime(bank1, gBattleMons[bank1].moves[gBattleStruct->chosenMovePositions[bank1]]);
		u16 move2 = ReplaceWithZMoveRuntime(bank2, gBattleMons[bank2].moves[gBattleStruct->chosenMovePositions[bank2]]);
	
		bank1Priority = PriorityCalc(bank1, gChosenActionByBank[bank1], move1);
		bank2Priority = PriorityCalc(bank2, gChosenActionByBank[bank2], move2);
		if (bank1Priority > bank2Priority)
			return FirstMon;
		else if (bank1Priority < bank2Priority)
			return SecondMon;

		bank1Bracket = gNewBS->lastBracketCalc[bank1] = BracketCalc(bank1, gChosenActionByBank[bank1], move1);
		bank2Bracket = gNewBS->lastBracketCalc[bank2] = BracketCalc(bank2, gChosenActionByBank[bank2], move2);
	}
	else
	{
		//Bracket Calc
		bank1Bracket = gNewBS->lastBracketCalc[bank1] = BracketCalc(bank1, 0, MOVE_NONE);
		bank2Bracket = gNewBS->lastBracketCalc[bank2] = BracketCalc(bank2, 0, MOVE_NONE);
	}

	if (bank1Bracket > bank2Bracket)
		return FirstMon;
	else if (bank1Bracket < bank2Bracket)
		return SecondMon;

//Speed Calc
	bank1Spd = SpeedCalc(bank1);
	bank2Spd = SpeedCalc(bank2);
	u32 temp;
	if (IsTrickRoomActive())
	{
		temp = bank2Spd;
		bank2Spd = bank1Spd;
		bank1Spd = temp;
	}

	if (bank1Spd > bank2Spd)
		return FirstMon;
	else if (bank1Spd < bank2Spd)
		return SecondMon;

	if (Random() & 1)
		return SpeedTie; //Second mon goes first because it won the speed tie

	return FirstMon;
}

static u8 GetWhoStrikesFirstUseLastBracketCalc(u8 bank1, u8 bank2)
{
	s8 bank1Priority, bank2Priority;
	s8 bank1Bracket, bank2Bracket;
	u32 bank1Spd, bank2Spd;

	//Priority Calc
	bank1Priority = PriorityCalc(bank1, gChosenActionByBank[bank1], ReplaceWithZMoveRuntime(bank1, gBattleMons[bank1].moves[gBattleStruct->chosenMovePositions[bank1]]));
	bank2Priority = PriorityCalc(bank2, gChosenActionByBank[bank2], ReplaceWithZMoveRuntime(bank2, gBattleMons[bank2].moves[gBattleStruct->chosenMovePositions[bank2]]));
	if (bank1Priority > bank2Priority)
		return FirstMon;
	else if (bank1Priority < bank2Priority)
		return SecondMon;

	//BracketCalc
	bank1Bracket = gNewBS->lastBracketCalc[bank1];
	bank2Bracket = gNewBS->lastBracketCalc[bank2];
	if (bank1Bracket > bank2Bracket)
		return FirstMon;
	else if (bank1Bracket < bank2Bracket)
		return SecondMon;

	//SpeedCalc
	bank1Spd = SpeedCalc(bank1);
	bank2Spd = SpeedCalc(bank2);
	u32 temp;
	if (IsTrickRoomActive())
	{
		temp = bank2Spd;
		bank2Spd = bank1Spd;
		bank1Spd = temp;
	}

	if (bank1Spd > bank2Spd)
		return FirstMon;
	else if (bank1Spd < bank2Spd)
		return SecondMon;

	return SpeedTie;
}

s8 PriorityCalc(u8 bank, u8 action, u16 move)
{
	u8 priority = 0;

	if (!BATTLER_ALIVE(bank))
		return 0;

	if (action == ACTION_USE_MOVE)
	{
		if (gProtectStructs[bank].onlyStruggle)
			move = MOVE_STRUGGLE;

		priority = gBattleMoves[move].priority;

		if (move != MOVE_BIDE && gBattleMons[bank].status2 & STATUS2_BIDE)
			priority = 1;


		#ifdef FLAG_WEIGHT_SPEED_BATTLE
		if (!FlagGet(FLAG_WEIGHT_SPEED_BATTLE))
		#endif
		{
			switch (ABILITY(bank))
			{
				case ABILITY_PRANKSTER:
					if (SPLIT(move) == SPLIT_STATUS)
						++priority;
					break;

				case ABILITY_GALEWINGS: ;
					u16 species = GetProperAbilityPopUpSpecies(bank);
					if (SpeciesHasGrassDash(species))
					{
						if (GetMoveTypeSpecial(bank, move) == TYPE_GRASS)
							++priority;
					}
					else if (SpeciesHasSlipperyTail(species))
					{
						if (gSpecialMoveFlags[move].gTailMoves)
							++priority;
					}
					else if (GetMoveTypeSpecial(bank, move) == TYPE_FLYING
					#ifndef OLD_GALE_WINGS
					&& BATTLER_MAX_HP(bank)
					#endif
					) //Gale Wings
					{
						++priority;
					}
					break;

				case ABILITY_TRIAGE:
					if (gBattleMoves[move].flags & FLAG_TRIAGE_AFFECTED)
						priority += 3;
					break;
			}
		}

		if (move == MOVE_GRASSYGLIDE && gTerrainType == GRASSY_TERRAIN && CheckGrounding(bank))
			++priority;
	}

	return priority;
}

s8 PriorityCalcMon(struct Pokemon* mon, u16 move)
{
	u8 priority = gBattleMoves[move].priority;

	#ifdef FLAG_WEIGHT_SPEED_BATTLE
	if (!FlagGet(FLAG_WEIGHT_SPEED_BATTLE))
	#endif
	{
		switch (GetMonAbility(mon)) {
			case ABILITY_PRANKSTER:
				if (SPLIT(move) == SPLIT_STATUS)
					++priority;
				break;

			case ABILITY_GALEWINGS:
				if (SpeciesHasGrassDash(mon->species))
				{
					if (GetMonMoveTypeSpecial(mon, move) == TYPE_GRASS)
						++priority;
				}
				else if (SpeciesHasSlipperyTail(mon->species))
				{
					if (gSpecialMoveFlags[move].gTailMoves)
						++priority;
				}
				else if (GetMonMoveTypeSpecial(mon, move) == TYPE_FLYING
				#ifndef OLD_GALE_WINGS
				&& GetMonData(mon, MON_DATA_HP, NULL) == GetMonData(mon, MON_DATA_MAX_HP, NULL)
				#endif
				)
				{
					++priority;
				}
				break;

			case ABILITY_TRIAGE:
				if (gBattleMoves[move].flags & FLAG_TRIAGE_AFFECTED)
					priority += 3;
				break;
		}
	}

	return priority;
}

bool8 QuickClawActivatesThisTurn(u8 bank)
{
	return gNewBS->quickClawRandomNumber[bank] < ITEM_QUALITY(bank);
}

s32 BracketCalc(u8 bank, u8 action, u16 move)
{
	u8 itemEffect = ITEM_EFFECT(bank);
	u8 ability = ABILITY(bank);

	gNewBS->quickClawCustapIndicator &= ~(gBitTable[bank]); //Reset the Quick Claw counter just in case
	gNewBS->quickDrawIndicator &= ~(gBitTable[bank]); //Reset the Quick Claw counter just in case
	if (BATTLER_ALIVE(bank))
	{
		if (gNewBS->ateCustapBerry & gBitTable[bank]) //Already ate the Berry
			return 1;
		else
		{
			if (ability == ABILITY_QUICKDRAW
			&& gNewBS->quickDrawRandomNumber[bank] < 30 //30% chance - activates before items
			&& action == ACTION_USE_MOVE
			&& SPLIT(move) != SPLIT_STATUS) //Only damaging moves
			{
				gNewBS->quickDrawIndicator |= gBitTable[bank];
				return 1;
			}

			switch (itemEffect) {
				case ITEM_EFFECT_QUICK_CLAW:
					if (QuickClawActivatesThisTurn(bank))
					{
						gNewBS->quickClawCustapIndicator |= gBitTable[bank];
						return 1;
					}
					break;

				case ITEM_EFFECT_CUSTAP_BERRY:
					if (PINCH_BERRY_CHECK(bank) && !UnnerveOnOpposingField(bank))
					{
						gNewBS->quickClawCustapIndicator |= gBitTable[bank];
						return 1;
					}
					break;

				case ITEM_EFFECT_LAGGING_TAIL:
					return -2;
			}
		}

		if (ability == ABILITY_STALL && !(SpeciesHasBeadsofRuin(SPECIES(bank))
										|| SpeciesHasSwordofRuin(SPECIES(bank))
										|| SpeciesHasTabletsofRuin(SPECIES(bank))
										|| SpeciesHasVesselofRuin(SPECIES(bank))))
			return -1;
		
		if (SPLIT(gCurrentMove) == SPLIT_STATUS && ability == ABILITY_MOLDBREAKER && SpeciesHasMyceliumMight(SPECIES(bank)))
			return -2;
	}

	return 0;
}

static u32 BoostSpeedInWeather(u8 ability, u8 itemEffect, u32 speed)
{
	if (WEATHER_HAS_EFFECT) {
		switch (ability) {
			case ABILITY_SWIFTSWIM:
				if (gBattleWeather & WEATHER_RAIN_ANY && !ItemEffectIgnoresSunAndRain(itemEffect))
					speed *= 2;
				break;
			case ABILITY_CHLOROPHYLL:
				if (gBattleWeather & WEATHER_SUN_ANY && !ItemEffectIgnoresSunAndRain(itemEffect))
					speed *= 2;
				break;
			case ABILITY_SANDRUSH:
				if (gBattleWeather & WEATHER_SANDSTORM_ANY)
					speed *= 2;
				break;
			case ABILITY_SLUSHRUSH:
				if (gBattleWeather & WEATHER_HAIL_ANY)
					speed *= 2;
				break;
		}
	}

	return speed;
}

static u32 BoostSpeedByItemEffect(u8 itemEffect, u8 itemQuality, u16 species, u32 speed, bool8 isDynamaxed)
{
	switch (itemEffect) {
		case ITEM_EFFECT_MACHO_BRACE:
			speed /= 2;
			break;
		case ITEM_EFFECT_CHOICE_BAND:
			if (!isDynamaxed && itemQuality == QUALITY_CHOICE_SCARF)
				speed = (speed * 15) / 10;
			break;
		case ITEM_EFFECT_IRON_BALL:
			speed /= 2;
			break;
		case ITEM_EFFECT_QUICK_POWDER:
			if (species == SPECIES_DITTO)
				speed *= 2;
	}

	return speed;
}

u32 SpeedCalc(u8 bank)
{
	u32 speed, rawSpeed;

	if (!BATTLER_ALIVE(bank))
		return 0;

	u8 ability = ABILITY(bank);
	u8 itemEffect = ITEM_EFFECT(bank);
	u8 itemQuality = ITEM_QUALITY(bank);

	//Calculate adjusted speed stat
	#ifdef FLAG_WEIGHT_SPEED_BATTLE
	if (FlagGet(FLAG_WEIGHT_SPEED_BATTLE))
		rawSpeed = GetActualSpeciesWeight(SPECIES(bank), ability, itemEffect, bank, TRUE);
	else
	#endif
		rawSpeed = gBattleMons[bank].speed;

	speed = (rawSpeed * gStatStageRatios[gBattleMons[bank].statStages[STAT_STAGE_SPEED-1]][0]) / gStatStageRatios[gBattleMons[bank].statStages[STAT_STAGE_SPEED-1]][1];

	//Check for abilities that alter speed
	speed = BoostSpeedInWeather(ability, itemEffect, speed);

	switch (ability) {
		case ABILITY_UNBURDEN:
			if (gNewBS->UnburdenBoosts & gBitTable[bank] && ITEM(bank) == ITEM_NONE)
				speed *= 2;
			break;
		case ABILITY_SLOWSTART:
			if (gNewBS->SlowStartTimers[bank])
				speed /= 2;
			break;
		case ABILITY_SURGESURFER:
			if (gTerrainType == ELECTRIC_TERRAIN)
				speed *= 2;
			break;
	}

	speed = BoostSpeedByItemEffect(itemEffect, itemQuality, SPECIES(bank), speed, IsDynamaxed(bank));

	if (BankHasTailwind(bank))
		speed *= 2;
	if (BankSideHasSwamp(bank))
		speed /= 4;

	#ifdef BADGE_BOOSTS
		if (!(gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_TRAINER_TOWER | BATTLE_TYPE_FRONTIER | BATTLE_TYPE_EREADER_TRAINER))
		&& FlagGet(FLAG_BADGE03_GET)
		&& gBattleTypeFlags & BATTLE_TYPE_TRAINER
		&& SIDE(bank) == B_SIDE_PLAYER
		&& gTrainerBattleOpponent_A != 0x400)
			speed = (speed * 11) / 10; //1.1x
	#endif

	if (gBattleMons[bank].status1 & STATUS_ANY && ability == ABILITY_QUICKFEET)
	{
		speed = (speed * 15) / 10; //1.5x
	}
	else if (gBattleMons[bank].status1 & STATUS_PARALYSIS)
	{
		#ifndef OLD_PARALYSIS_SPD_DROP
			speed /= 2;
		#else
			speed /= 4;
		#endif
	}

	return speed;
}

u32 SpeedCalcMon(u8 side, struct Pokemon* mon) //Used for the AI
{
	if (GetMonData(mon, MON_DATA_IS_EGG, NULL))
		return 0;

	u32 speed;
	u8 ability = GetMonAbilityAfterTrace(mon, FOE(side));
	u8 itemEffect = (ability != ABILITY_KLUTZ) ? ItemId_GetHoldEffect(mon->item) : 0;
	u8 itemQuality = ItemId_GetHoldEffectParam(mon->item);
	u8 statVal = 6;

	#ifdef FLAG_WEIGHT_SPEED_BATTLE
	if (FlagGet(FLAG_WEIGHT_SPEED_BATTLE))
		speed = GetActualSpeciesWeight(mon->species, ability, itemEffect, 0, FALSE);
	else
	#endif
		speed = mon->speed;

	//Calculate adjusted speed stat if Sticky Web is present
	if (gSideTimers[side].stickyWeb
	&& IsMonAffectedByHazards(mon)
	&& ability != ABILITY_CLEARBODY
	&& ITEM_EFFECT(gActiveBattler) != ITEM_EFFECT_CLEAR_AMULET
	&& CheckMonGrounding(mon))
	{
		if (ability == ABILITY_CONTRARY) //Gets a speed boost
			statVal += 1;
		else if (ability == ABILITY_SIMPLE) //Gets extra speed decrement
			statVal -= 2;
		else
			statVal -= 1;
	}

	if (IsTrickRoomActive() && itemEffect == ITEM_EFFECT_ROOM_SERVICE)
		statVal -= 1;

	speed = (speed * gStatStageRatios[statVal][0]) / gStatStageRatios[statVal][1];

	//Check for abilities that alter speed
	speed = BoostSpeedInWeather(ability, itemEffect, speed);

	switch (ability) {
		case ABILITY_SLOWSTART:
			speed /= 2;
			break;
		case ABILITY_SURGESURFER:
			if (gTerrainType == ELECTRIC_TERRAIN)
				speed *= 2;
			break;
	}

	//Check for items that alter speed
	speed = BoostSpeedByItemEffect(itemEffect, itemQuality, mon->species, speed, FALSE);

	//Check other things that alter speed
	if (MonHasTailwind(mon, side))
		speed *= 2;
	if (SideHasSwamp(side))
		speed /= 4;

	if (mon->condition & STATUS_ANY && ability == ABILITY_QUICKFEET)
		speed *= 2;
	else if (mon->condition & STATUS_PARALYSIS)
	{
		#ifndef OLD_PARALYSIS_SPD_DROP
			speed /= 2;
		#else
			speed /= 4;
		#endif
	}

	return speed;
}
