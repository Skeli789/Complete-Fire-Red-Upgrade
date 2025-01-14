#include "defines.h"
#include "defines_battle.h"
#include "../include/battle_anim.h"
#include "../include/bg.h"
#include "../include/event_data.h"
#include "../include/field_player_avatar.h"
#include "../include/field_specials.h"
#include "../include/item.h"
#include "../include/overworld.h"
#include "../include/random.h"
#include "../include/string_util.h"
#include "../include/constants/game_stat.h"
#include "../include/constants/items.h"
#include "../include/constants/songs.h"

#include "../include/new/battle_util.h"
#include "../include/new/catching.h"
#include "../include/new/dns.h"
#include "../include/new/dynamax.h"
#include "../include/new/end_battle_battle_scripts.h"
#include "../include/new/form_change.h"
#include "../include/new/util.h"
#include "../include/new/mega.h"
#include "../include/new/pokemon_storage_system.h"

/*
catching.c
	handles the catch probability logic, expands pokeballs, etc.
*/

#define gOpenPokeballGfx (u8*) 0x8D022E8

#define BattleScript_TutorialThrow 		((u8*) 0x81D9A88)
#define BattleScript_ShakeBallThrow 	((u8*) 0x81D9A93)
#define BattleScript_TrainerBallBlock 	((u8*) 0x81D9AC1)
#define BattleScript_DodgedBall 		((u8*) 0x81D9AD1)
extern const u8 BattleScript_RaidMonEscapeBall[];

#define sBallCatchBonuses ((u8*) 0x8250892)

extern const struct CompressedSpriteSheet gBallSpriteSheets[];
extern const struct CompressedSpritePalette gBallSpritePalettes[];
extern const struct SpriteTemplate gBallSpriteTemplates[];

#ifdef CAPTURE_EXPERIENCE
	extern u8 BattleScript_SuccessBallThrow[];
#else
	#define BattleScript_SuccessBallThrow ((u8*) 0x81D9A42)
#endif

extern const u8 gText_CantAimAtTwoTargets[];
extern const u8 gText_CantAimAtSemiInvulnerableTarget[];
extern const u8 gText_CantUseAMoveAndABall[];
extern const u8 gText_CantCatchPokemonYet[];
extern const u8 gText_CantCatchPokemonRightNow[];

extern species_t gUltraBeastList[];

//This file's functions:
static u8 GetCatchingBattler(void);
static bool8 CriticalCapture(u32 odds);

void atkEF_handleballthrow(void)
{
	if (gBattleExecBuffer) return;

	gActiveBattler = gBankAttacker;
	gBankTarget = GetCatchingBattler();

	u8 defLevel = gBattleMons[gBankTarget].level;

	u8 ballType = ItemId_GetType(gLastUsedItem);
	gNewBS->threwBall = TRUE;
	if (ballType != BALL_TYPE_QUICK_BALL //Useless to offer to player after initial use
	|| gBattleResults.battleTurnCounter > 0 //Unless the player explictly chose it after the first turn
	#ifdef FLAG_SANDBOX_MODE
	|| FlagGet(FLAG_SANDBOX_MODE) //Unless all balls have a 100% catch rate
	#endif
	)
		gLastUsedBall = gLastUsedItem;

	if (gNewBS->isTrainerBattle) //Doesn't use BATTLE_TYPE_TRAINER because that's removed by the Catch Trainers Pokemon cheat
	{
		#ifdef FLAG_CATCH_TRAINERS_POKEMON
			if (FlagGet(FLAG_CATCH_TRAINERS_POKEMON))
			{
				EmitBallThrowAnim(0, 4);
				MarkBufferBankForExecution(gActiveBattler);
				gBattlescriptCurrInstr = BattleScript_SuccessBallThrow;
				if (ballType != BALL_TYPE_PARK_BALL)
					SetMonData(GetBankPartyData(gBankTarget), MON_DATA_POKEBALL, &ballType);
				if (CalculatePlayerPartyCount() == 6)
					gBattleCommunication[MULTISTRING_CHOOSER] = 0;
				else
					gBattleCommunication[MULTISTRING_CHOOSER] = 1;
			}
			else
			{
				EmitBallThrowAnim(0, 5);
				MarkBufferBankForExecution(gActiveBattler);
				gBattlescriptCurrInstr = BattleScript_TrainerBallBlock;
			}
		#else
			EmitBallThrowAnim(0, 5);
			MarkBufferBankForExecution(gActiveBattler);
			gBattlescriptCurrInstr = BattleScript_TrainerBallBlock;
		#endif
	}
	else if (gBattleTypeFlags & BATTLE_TYPE_GHOST
	#ifdef FLAG_NO_CATCHING
	|| FlagGet(FLAG_NO_CATCHING)
	#endif
	#ifdef FLAG_NO_CATCHING_AND_RUNNING
	|| FlagGet(FLAG_NO_CATCHING_AND_RUNNING)
	#endif
	)
	{
		EmitBallThrowAnim(0, 6);
		MarkBufferBankForExecution(gActiveBattler);
		gBattlescriptCurrInstr = BattleScript_DodgedBall;
	}
	else if (gBattleTypeFlags & (BATTLE_TYPE_OLD_MAN | BATTLE_TYPE_POKE_DUDE))
	{
		EmitBallThrowAnim(0, 4);
		MarkBufferBankForExecution(gActiveBattler);
		gBattlescriptCurrInstr = BattleScript_TutorialThrow;
	}
	else
	{
		u32 odds = GetBaseBallCatchOdds(ballType, gBankAttacker, gBankTarget);

		#ifndef NO_HARDER_WILD_DOUBLES
		if (!(gBattleTypeFlags & BATTLE_TYPE_TRAINER) && IS_DOUBLE_BATTLE)
		{
			u16 PokesCaught = GetNationalPokedexCount(FLAG_GET_CAUGHT);
			if (PokesCaught < 30)
				odds = (odds * 30) / 100;
			else if (PokesCaught <= 150)
				odds = (odds * 50) / 100;
			else if (PokesCaught <= 300)
				odds = (odds * 70) / 100;
			else if (PokesCaught <= 450)
				odds = (odds * 80) / 100;
			else if (PokesCaught <= 600)
				odds = (odds * 80) / 100;
		}
		#endif

		//Low-Level Modifier - from SwSh
		if (defLevel <= 20)
			odds = (odds * (30 - defLevel)) / 10;

		//Status Modifier
		if (gBattleMons[gBankTarget].status1 & (STATUS_SLEEP
		#ifndef FROSTBITE
		                                      | STATUS_FREEZE
		#endif
		                                       ))
			odds = (odds * 25) / 10;
		if (gBattleMons[gBankTarget].status1 & (STATUS_PSN_ANY | STATUS_BURN | STATUS_PARALYSIS
		#ifdef FROSTBITE
		                                      | STATUS_FREEZE
		#endif
		                                       ))
			odds = (odds * 15) / 10;

		//Difficulty Modifier - from SwSh
		#ifdef SWSH_CATCHING_DIFFICULTY_MODIFIER
		if (!FlagGet(FLAG_SYS_GAME_CLEAR) && gBattleMons[gBankAttacker].level < defLevel)
			odds /= 10;
		#endif

		//Raid Modifier
		if (IsRaidBattle()) //Dynamax Raid Pokemon can be caught easier
			odds *= 4;

		if (ballType != BALL_TYPE_SAFARI_BALL)
		{
			if (ballType == BALL_TYPE_MASTER_BALL)
				gBattleResults.usedMasterBall = 1;

			//This was used for the TV shows in Ruby, but seems kind of pointless in FR.
			//Commenting it out also prevents errors from using poke balls with large indices.
			//else if (gBattleResults.usedBalls[ballType - BALL_TYPE_ULTRA_BALL] < 0xFF)
			//		gBattleResults.usedBalls[ballType - BALL_TYPE_ULTRA_BALL]++;
		}

		if (odds >= 0xFF) //Pokemon is Caught
		{
			EmitBallThrowAnim(0, 4);
			MarkBufferBankForExecution(gActiveBattler);
			gBattlescriptCurrInstr = BattleScript_SuccessBallThrow;

			if (ballType != BALL_TYPE_PARK_BALL || IsRaidBattle())
				SetMonData(&gEnemyParty[gBattlerPartyIndexes[gBankTarget]], MON_DATA_POKEBALL, &ballType);

			if (CalculatePlayerPartyCount() == 6)
				gBattleCommunication[MULTISTRING_CHOOSER] = 0;
			else
				gBattleCommunication[MULTISTRING_CHOOSER] = 1;
		}
		else //Pokemon may be caught, calculate shakes
		{
			u8 shakes, maxShakes;

			if (CriticalCapture(odds))
			{
				maxShakes = 2;  //Critical capture doesn't gauarantee capture
			}
			else
			{
				maxShakes = 4;
			}

			if (ballType == BALL_TYPE_MASTER_BALL
			||	ballType == BALL_TYPE_PARK_BALL
			#ifdef FLAG_ALWAYS_CATCHABLE
			|| FlagGet(FLAG_ALWAYS_CATCHABLE)
			#endif
			#ifdef FLAG_SANDBOX_MODE
			|| FlagGet(FLAG_SANDBOX_MODE)
			#endif
			)
				shakes = maxShakes;
			else
			{
				#ifdef BUTTON_PRESS_INCREASES_CATCH_ODDS
				if (JOY_HELD(A_BUTTON)
				|| JOY_REPT(A_BUTTON)
				|| (JOY_HELD(B_BUTTON) && JOY_HELD(DPAD_DOWN))
				|| (JOY_REPT(B_BUTTON) && JOY_REPT(DPAD_DOWN)))
					odds += (Sqrt(odds * 5)); //Bigger gains for lower numbers
				#endif

				odds = udivsi(0xFFFF0, Sqrt(Sqrt(udivsi(0xFF0000, odds))));
				for (shakes = 0; shakes < maxShakes && Random() < odds; ++shakes) ;
			}

			EmitBallThrowAnim(0, shakes);
			MarkBufferBankForExecution(gActiveBattler);

			if (!gNewBS->firstFailedPokeBallStored)
			{
				gNewBS->firstFailedPokeBallStored = TRUE; //Only once per battle
				gNewBS->failedThrownPokeBall = gLastUsedItem;
			}

			if (shakes >= maxShakes)
			{
				if (gNewBS->criticalCapture)
					gNewBS->criticalCaptureSuccess = TRUE;

				gBattlescriptCurrInstr = BattleScript_SuccessBallThrow;
				if (ballType != BALL_TYPE_PARK_BALL || IsRaidBattle())
					SetMonData(&gEnemyParty[gBattlerPartyIndexes[gBankTarget]], MON_DATA_POKEBALL, &ballType);

				if (CalculatePlayerPartyCount() == 6)
					gBattleCommunication[MULTISTRING_CHOOSER] = 0;
				else
					gBattleCommunication[MULTISTRING_CHOOSER] = 1;
			}
			else if (IsRaidBattle())
			{
				gBattleCommunication[MULTISTRING_CHOOSER] = shakes;
				gBattlescriptCurrInstr = BattleScript_RaidMonEscapeBall;
			}
			else //Rip
			{
				gBattleCommunication[MULTISTRING_CHOOSER] = shakes;
				gBattlescriptCurrInstr = BattleScript_ShakeBallThrow;
			}
		}
	}
}

u32 GetBaseBallCatchOdds(u8 ballType, u8 bankAtk, u8 bankDef)
{
	u8 catchRate;
	u8 ballMultiplier = 0;
	u16 atkSpecies = gBattleMons[bankAtk].species;
	u16 defSpecies = GetMonData(GetBankPartyData(bankDef), MON_DATA_SPECIES, NULL); //Can't use gBattleMons because of things like Transform
	u8 atkLevel = gBattleMons[bankAtk].level;
	u8 defLevel = gBattleMons[bankDef].level;

	if (ballType == BALL_TYPE_SAFARI_BALL)
		catchRate = (gBattleStruct->safariCatchFactor * 1275) % 100;
	else
		catchRate = gBaseStats[GetMonData(GetBankPartyData(bankDef), MON_DATA_SPECIES, NULL)].catchRate; //Uses party data b/c Transform update Gen 5+

	if (ballType >= BALL_TYPE_NET_BALL)
	{
		switch (ballType) {
			case BALL_TYPE_NET_BALL:
				if (IsOfType(bankDef, TYPE_WATER) || IsOfType(bankDef, TYPE_BUG))
					ballMultiplier = 35;
				else
					ballMultiplier = 10;
				break;

			case BALL_TYPE_DIVE_BALL:
				if (GetCurrentMapType() == MAP_TYPE_UNDERWATER
				|| gFishingByte || TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_SURFING))
					ballMultiplier = 35;
				else
					ballMultiplier = 10;
				break;

			case BALL_TYPE_NEST_BALL:
				if (gBattleMons[bankDef].level < 31)
					ballMultiplier = 41 - defLevel; //At lowest 11
				else
					ballMultiplier = 10;
				break;

			case BALL_TYPE_REPEAT_BALL:
				if (GetSetPokedexFlag(SpeciesToNationalPokedexNum(defSpecies), FLAG_GET_CAUGHT))
					ballMultiplier = 35;
				else
					ballMultiplier = 10;
				break;

			case BALL_TYPE_TIMER_BALL:
				ballMultiplier = MathMin(40, 10 + gBattleResults.battleTurnCounter * 3);
				break;

			case BALL_TYPE_LUXURY_BALL:
			case BALL_TYPE_PREMIER_BALL:
			case BALL_TYPE_CHERISH_BALL:
			case BALL_TYPE_FRIEND_BALL:
			case BALL_TYPE_HEAL_BALL:
				ballMultiplier = 10;
				break;

			case BALL_TYPE_LEVEL_BALL:
				if (atkLevel <= defLevel)
					ballMultiplier = 10;
				else if (atkLevel > defLevel && atkLevel < defLevel * 2)
					ballMultiplier = 20;
				else if (atkLevel >= defLevel * 2 && atkLevel < defLevel * 4)
					ballMultiplier = 40;
				else
					ballMultiplier = 80;
				break;

			case BALL_TYPE_LURE_BALL:
				if (gFishingByte)
					ballMultiplier = 40;
				else
					ballMultiplier = 10;
				break;

			case BALL_TYPE_MOON_BALL: ;
				switch (defSpecies) {
					default:
						ballMultiplier = 10;
						break;
					#ifdef SPECIES_NIDORAN_F
					case SPECIES_NIDORAN_F:
					#endif
					#ifdef SPECIES_NIDORINA
					case SPECIES_NIDORINA:
					#endif
					#ifdef SPECIES_NIDOQUEEN
					case SPECIES_NIDOQUEEN:
					#endif
					#ifdef SPECIES_NIDORAN_M
					case SPECIES_NIDORAN_M:
					#endif
					#ifdef SPECIES_NIDORINO
					case SPECIES_NIDORINO:
					#endif
					#ifdef SPECIES_NIDOKING
					case SPECIES_NIDOKING:
					#endif
					#ifdef SPECIES_CLEFFA
					case SPECIES_CLEFFA:
					#endif
					#ifdef SPECIES_CLEFAIRY
					case SPECIES_CLEFAIRY:
					#endif
					#ifdef SPECIES_CLEFABLE
					case SPECIES_CLEFABLE:
					#endif
					#ifdef SPECIES_IGGLYBUFF
					case SPECIES_IGGLYBUFF:
					#endif
					#ifdef SPECIES_JIGGLYPUFF
					case SPECIES_JIGGLYPUFF:
					#endif
					#ifdef SPECIES_WIGGLYTUFF
					case SPECIES_WIGGLYTUFF:
					#endif
					#ifdef SPECIES_SKITTY
					case SPECIES_SKITTY:
					#endif
					#ifdef SPECIES_DELCATTY
					case SPECIES_DELCATTY:
					#endif
					#ifdef SPECIES_MUNNA
					case SPECIES_MUNNA:
					#endif
					#ifdef SPECIES_MUSHARNA
					case SPECIES_MUSHARNA:
					#endif
						ballMultiplier = 40;
						break;
				}

				break;

			case BALL_TYPE_LOVE_BALL:
				ballMultiplier = 10;
				if (SpeciesToNationalPokedexNum(atkSpecies) == SpeciesToNationalPokedexNum(defSpecies))
				{
					u8 atkGender = GetGenderFromSpeciesAndPersonality(atkSpecies, gBattleMons[bankAtk].personality);
					u8 defGender = GetGenderFromSpeciesAndPersonality(defSpecies, gBattleMons[bankDef].personality);

					if (atkGender != 0xFF && defGender != 0xFF && atkGender != defGender)
						ballMultiplier = 80;
				}
				break;

			//Heavy Ball modifies the catch rate itself, not the multiplier
			case BALL_TYPE_HEAVY_BALL:	;
				ballMultiplier = 10;
				//Apparently abilities or Float Stone have no affect here
				u16 defWeight = GetPokedexHeightWeight(SpeciesToNationalPokedexNum(defSpecies), PKDX_GET_WEIGHT);
				u8 oldCatchRate;

				if (defWeight < 1000)
				{
					oldCatchRate = catchRate;
					catchRate -= 20;
					if (catchRate > oldCatchRate)
						catchRate = 1; //New in USUM
				}
				else if (defWeight >= 2000 && defWeight < 3000)
					catchRate += 20;
				else if (defWeight > 3000)
					catchRate += 30;
				break;

			case BALL_TYPE_FAST_BALL:
				if (gBaseStats[defSpecies].baseSpeed >= 100)
					ballMultiplier = 40;
				else
					ballMultiplier = 10;
				break;

			case BALL_TYPE_SPORT_BALL:
				ballMultiplier = 15;
				break;

			case BALL_TYPE_DUSK_BALL:
				if (GetCurrentMapType() == MAP_TYPE_UNDERGROUND)
					ballMultiplier = DUSK_BALL_MULTIPLIER;
				#ifdef TIME_ENABLED
				else if (IsNightTime())
					ballMultiplier = DUSK_BALL_MULTIPLIER;
				#endif
				else
					ballMultiplier = 10;
				break;

			case BALL_TYPE_QUICK_BALL:
				if (gBattleResults.battleTurnCounter == 0)
					ballMultiplier = 50;
				else
					ballMultiplier = 10;
				break;

			case BALL_TYPE_DREAM_BALL:
				if (gBattleMons[bankDef].status1 & STATUS1_SLEEP || ABILITY(bankDef) == ABILITY_COMATOSE)
					ballMultiplier = 40;
				else
					ballMultiplier = 10;
				break;

			case BALL_TYPE_BEAST_BALL:
				if (CheckTableForSpecies(defSpecies, gUltraBeastList))
					ballMultiplier = 50;
				else
					ballMultiplier = 1;
				break;
		}
	}
	else
		ballMultiplier = sBallCatchBonuses[ballType - BALL_TYPE_ULTRA_BALL];

	#ifdef VAR_CATCH_RATE_BONUS
	if (VarGet(VAR_CATCH_RATE_BONUS) > 0)
		ballMultiplier += (ballMultiplier * VarGet(VAR_CATCH_RATE_BONUS)) / 100; //Percent increase - Eg. Var is set to 10, and multiplier is 5x, then final multiplier will be 5.5 (55)
	#endif

	if (CheckTableForSpecies(defSpecies, gUltraBeastList) && ballType != BALL_TYPE_BEAST_BALL)
		ballMultiplier = 1; //All balls except for Beast Ball have a hard time catching Ultra Beasts

	u32 odds = (((catchRate * ballMultiplier) / 10) * (gBattleMons[bankDef].maxHP * 3 - gBattleMons[bankDef].hp * 2)) / (3 * gBattleMons[bankDef].maxHP);
	return odds;
}

static u8 GetCatchingBattler(void)
{
	u8 battler;

	if (IS_DOUBLE_BATTLE && GetBankPartyData(GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT))->hp > 0)
		battler = GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT);
	else
		battler = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);

	if (battler >= gBattlersCount || SIDE(battler) == B_SIDE_PLAYER)
		battler = B_POSITION_OPPONENT_LEFT;

	return battler;
}

#ifdef CRITICAL_CAPTURE
struct CriticalCaptureOdds
{
	u16 numCaught; //Less than or equal to this number
	u16 oddsMultiplier; //x10
};

static const struct CriticalCaptureOdds sCriticalCaptureSpeciesCounts[] =
{
	{30,      0}, //x0.0
	{150,     5}, //x0.5
	{300,    10}, //x1.0
	{450,    15}, //x1.5
	{600,    20}, //x2.0
	{0xFFFF, 25}, //x2.5 default
};
#endif

static bool8 CriticalCapture(unusedArg u32 odds)
{
	if (IsRaidBattle())
		return FALSE; //Critical Captures can't happen in Raids

	#ifdef CRITICAL_CAPTURE
	//Adjust Original Catch Odds - X
	u8 baseOdds = MathMin(odds, 255);

	//Pokedex Modifier - P
	u8 pokedexMultiplier = 0;
	u16 speciesCaught = GetNationalPokedexCount(FLAG_GET_CAUGHT);

	for (u32 i = 0; i < NELEMS(sCriticalCaptureSpeciesCounts); ++i)
	{
		if (speciesCaught <= sCriticalCaptureSpeciesCounts[i].numCaught)
		{
			pokedexMultiplier = sCriticalCaptureSpeciesCounts[i].oddsMultiplier;
			break;
		}
	}

	//Catching Charm Modifier - Ch
	u8 catchingCharmModifier = 1;

	#ifdef ITEM_CATCHING_CHARM
	if (CheckBagHasItem(ITEM_CATCHING_CHARM, 1))
		catchingCharmModifier = 2;
	#endif

	//Final Calculation
	odds = ((baseOdds * pokedexMultiplier * catchingCharmModifier) / 10) / 6;

	gNewBS->criticalCaptureSuccess = FALSE;
	if (Random() % 0x100 < odds) //0 to 255 inclusive
		return gNewBS->criticalCapture = TRUE;
	#endif

	return gNewBS->criticalCapture = FALSE;
}

bool8 IsCriticalCapture(void)
{
	return gNewBS->criticalCapture;
}

bool8 IsCriticalCaptureSuccess(void)
{
	return gNewBS->criticalCaptureSuccess;
}

static bool8 GetFreeSlotInPartyForMon(void)
{
	u32 i = 0;

	while (i < PARTY_SIZE && gPlayerParty[i].species != SPECIES_NONE)
		++i;

	if (i >= PARTY_SIZE
	|| (gMain.inBattle && gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER)) //Always send Pokemon in multi battles to the PC because of overwritten team
		return PARTY_SIZE;

	return i;
}

u8 GiveMonToPlayer(struct Pokemon* mon) //Hook in
{
	TryFormRevert(mon);
	TryRevertMega(mon);
	TryRevertGigantamax(mon);

	SetMonData(mon, MON_DATA_OT_NAME, gSaveBlock2->playerName);
	SetMonData(mon, MON_DATA_OT_GENDER, &gSaveBlock2->playerGender);
	SetMonData(mon, MON_DATA_OT_ID, gSaveBlock2->playerTrainerId);

	u8 freeSlot = GetFreeSlotInPartyForMon();
	if (freeSlot >= PARTY_SIZE) //Can't add mon
	{
		TryRevertOriginFormes(mon, TRUE);
		return SendMonToPC(mon);
	}

	CopyMon(&gPlayerParty[freeSlot], mon, sizeof(struct Pokemon));
	gPlayerPartyCount = freeSlot + 1;
	return MON_GIVEN_TO_PARTY;
}

void ApplyBallSpecialEffect(void)
{
	struct Pokemon* mon = LoadTargetPartyData();
	u8 ballType = ItemId_GetType(gLastUsedItem);

	if (ballType == BALL_TYPE_HEAL_BALL)
		HealMon(mon);
	else if (ballType == BALL_TYPE_FRIEND_BALL)
		mon->friendship = 200;
	#ifdef UNBOUND
	else if (ballType == BALL_TYPE_DREAM_BALL)
		mon->hiddenAbility = TRUE;
	#endif
}

#ifdef UNBOUND
static void FixOverflownPP(struct Pokemon* mon)
{
	u32 moveIndex;
	u32 ppBonuses = GetMonData(mon, MON_DATA_PP_BONUSES, NULL);

	for (moveIndex = 0; moveIndex < MAX_MON_MOVES; ++moveIndex)
	{
		u16 move = GetMonData(mon, MON_DATA_MOVE1 + moveIndex, NULL);
		u8 totalPP = CalculatePPWithBonus(move, ppBonuses, moveIndex);

		if (GetMonData(mon, MON_DATA_PP1 + moveIndex, NULL) > totalPP) //Overflown
			SetMonData(mon, MON_DATA_PP1 + moveIndex, &totalPP); //Adjust PP
	}
}
#endif

void atkF0_givecaughtmon(void)
{
	struct Pokemon* mon = LoadTargetPartyData();

	if (IsRaidBattle() && FlagGet(FLAG_BATTLE_FACILITY) && gNewBS->dynamaxData.backupRaidMonItem != ITEM_NONE)
	{
		SetMonData(mon, MON_DATA_HELD_ITEM, &gNewBS->dynamaxData.backupRaidMonItem);
		gNewBS->dynamaxData.backupRaidMonItem = ITEM_NONE;
	}

	#ifdef UNBOUND
	u32 none = 0;
	SetMonData(mon, MON_DATA_PP_BONUSES, &none); //In case it was set for a boss battle
	FixOverflownPP(mon);
	#endif

	if (GiveMonToPlayer(mon) != MON_GIVEN_TO_PARTY)
	{
		if (!ShouldShowBoxWasFullMessage())
		{
			gBattleCommunication[MULTISTRING_CHOOSER] = 0;
			StringCopy(gStringVar1, GetBoxNamePtr(VarGet(VAR_PC_BOX_TO_SEND_MON)));
			GetMonData(mon, MON_DATA_NICKNAME, gStringVar2);
		}
		else
		{
			StringCopy(gStringVar1, GetBoxNamePtr(VarGet(VAR_PC_BOX_TO_SEND_MON))); //Box the mon was sent to
			GetMonData(mon, MON_DATA_NICKNAME, gStringVar2);
			StringCopy(gStringVar3, GetBoxNamePtr(GetPCBoxToSendMon())); //Box the mon was going to be sent to
			gBattleCommunication[MULTISTRING_CHOOSER] = 2;
		}

		if (FlagGet(FLAG_SYS_NOT_SOMEONES_PC))
			++gBattleCommunication[MULTISTRING_CHOOSER];
	}

	if (!IsTimeInVarInFuture(VAR_SWARM_DAILY_EVENT))
		IncrementGameStat(GAME_STAT_CAUGHT_TODAY);
	else
		SetGameStat(GAME_STAT_CAUGHT_TODAY, 0);

	gBattleResults.caughtMonSpecies = SPECIES(gBankTarget);
	GetMonData(mon, MON_DATA_NICKNAME, gBattleResults.caughtMonNick);
	++gBattlescriptCurrInstr;
}

void WipeYesNoBattleBoxes(void)
{
	HandleBattleWindow(0x17, 0x8, 0x1D, 0xD, WINDOW_CLEAR);
	CopyBgTilemapBufferToVram(0); //Wipe any yes/no boxes that may still be present
}

void HandleTryTakeItemFromCaughtMonInput(void)
{
	if (gMain.newKeys & DPAD_UP && gBattleCommunication[CURSOR_POSITION] != 0)
	{
		PlaySE(SE_SELECT);
		BattleDestroyYesNoCursorAt(gBattleCommunication[CURSOR_POSITION]);
		gBattleCommunication[CURSOR_POSITION] = 0;
		BattleCreateYesNoCursorAt(0);
	}
	else if (gMain.newKeys & DPAD_DOWN && gBattleCommunication[CURSOR_POSITION] == 0)
	{
		PlaySE(SE_SELECT);
		BattleDestroyYesNoCursorAt(gBattleCommunication[CURSOR_POSITION]);
		gBattleCommunication[CURSOR_POSITION] = 1;
		BattleCreateYesNoCursorAt(1);
	}
	else if (gMain.newKeys & A_BUTTON)
	{
		PlaySE(SE_SELECT);

		if (gBattleCommunication[CURSOR_POSITION] != 0)
			goto SELECTED_NO;

		gBattleCommunication[CURSOR_POSITION] = 0; //So the game doesn't crash when selecting a move
		return;
	}
	else if (gMain.newKeys & B_BUTTON)
	{
		SELECTED_NO:
		PlaySE(SE_SELECT);
		BattleScriptPop();
		gBattleCommunication[CURSOR_POSITION] = 0; //So the game doesn't crash when selecting a move
		HandleBattleWindow(0x17, 0x8, 0x1D, 0xD, WINDOW_CLEAR);
		CopyBgTilemapBufferToVram(0);
	}

	gBattlescriptCurrInstr -= 5;
}

void TakeItemFromCaughtMon(void)
{
	struct Pokemon* mon = LoadTargetPartyData();

	//Add the item to the bag
	u16 item = GetMonData(mon, MON_DATA_HELD_ITEM, NULL);
	AddBagItem(item, 1);

	//Remove the item from the mon
	item = ITEM_NONE;
	SetMonData(mon, MON_DATA_HELD_ITEM, &item);

	gBattleStringLoader =
	#ifdef UNBOUND
		gText_TookCaughtItemToCube;
	#else
		gText_TookCaughtItemToBag;
	#endif
}

void atkF1_trysetcaughtmondexflags(void)
{
	struct Pokemon* mon = LoadTargetPartyData();

	#ifdef TAKE_WILD_MON_ITEM_ON_CAPTURE
	if (!gNewBS->triedToTakeWildItem)
	{
		gNewBS->triedToTakeWildItem = TRUE; //Only execute this code once
		u16 item = GetMonData(mon, MON_DATA_HELD_ITEM, NULL);

		if (item != ITEM_NONE && !IsFrontierRaidBattle() && CheckBagHasSpace(item, 1))
		{
			gLastUsedItem = GetMonData(mon, MON_DATA_HELD_ITEM, NULL);
			PREPARE_MON_NICK_BUFFER(gBattleTextBuff1, gBankTarget, gBattlerPartyIndexes[gBankTarget]);
			BattleScriptPushCursor();

			#ifdef VAR_TAKE_WILD_ITEM
			u8 choice = VarGet(VAR_TAKE_WILD_ITEM);
			if (choice == OPTIONS_ALWAYS_TAKE_WILD_ITEM)
			{
				gBattlescriptCurrInstr = BattleScript_TakeWildMonItem;
				return;
			}
			else if (choice != OPTIONS_NEVER_TAKE_WILD_ITEM) //Prompt
			#endif
			{
				gBattlescriptCurrInstr = BattleScript_TryTakeWildMonItem;
				return;
			}
		}
	}
	#endif

	TryRevertMega(mon); //Megas aren't set in the habitat table
	TryRevertGigantamax(mon); //Gigantamaxes aren't set in the habitat table
	TryFormRevert(mon);

	u16 species = GetMonData(mon, MON_DATA_SPECIES, NULL);
	u32 personality = GetMonData(mon, MON_DATA_PERSONALITY, NULL);

	if (GetSetPokedexFlag(SpeciesToNationalPokedexNum(species), FLAG_GET_CAUGHT))
	{
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
	}
	else
	{
		HandleSetPokedexFlag(SpeciesToNationalPokedexNum(species), FLAG_SET_CAUGHT, personality);
		gBattlescriptCurrInstr += 5;
	}
}

void CreateCapturedMonDexPic(u16 species)
{
	CreateMonPicSprite_HandleDeoxys(species, gBattleMons[gBankTarget].otId,
									gBattleMons[gBankTarget].personality,
									TRUE, 120, 64, 0, 0xFFFF);
}

u8 ItemIdToBallId(u16 ballItem)
{
	if (ballItem == 0xFFFF)
		return BALL_TYPE_DYNAMAX_BALL;

	return ItemId_GetType(ballItem);
}

void PlayerHandleSuccessBallThrowAnim(void)
{
	u8 animNum = (gNewBS->criticalCapture) ? B_ANIM_CRITICAL_CAPTURE_THROW : B_ANIM_BALL_THROW;
	gBattleSpritesDataPtr->animationData->ballThrowCaseId = BALL_3_SHAKES_SUCCESS;

	gDoingBattleAnim = TRUE;
	InitAndLaunchSpecialAnimation(gActiveBattler, gActiveBattler, GetCatchingBattler(), animNum);
	gBattlerControllerFuncs[gActiveBattler] = CompleteOnSpecialAnimDone;
}

void PlayerHandleBallThrowAnim(void)
{
	u8 animNum = (gNewBS->criticalCapture) ? B_ANIM_CRITICAL_CAPTURE_THROW : B_ANIM_BALL_THROW;
	u8 ballThrowCaseId = gBattleBufferA[gActiveBattler][1];

	gBattleSpritesDataPtr->animationData->ballThrowCaseId = ballThrowCaseId;
	gDoingBattleAnim = TRUE;
	InitAndLaunchSpecialAnimation(gActiveBattler, gActiveBattler, GetCatchingBattler(), animNum);
	gBattlerControllerFuncs[gActiveBattler] = CompleteOnSpecialAnimDone;
}

void AnimTask_LoadBallGfx(u8 taskId)
{
	u8 ballId = ItemIdToBallId(gLastUsedItem);
	LoadBallGfx(ballId);
	DestroyAnimVisualTask(taskId);
}

void __attribute__((long_call)) FreeBallGfx(u8 ballId);
void AnimTask_FreeBallGfx(u8 taskId)
{
	u8 ballId = ItemIdToBallId(gLastUsedItem);
	FreeBallGfx(ballId);
	DestroyAnimVisualTask(taskId);
}

void AnimTask_ThrowBall(u8 taskId)
{
	u8 ballId;
	u8 spriteId;

	ballId = ItemIdToBallId(gLastUsedItem);
	spriteId = CreateSprite(&gBallSpriteTemplates[ballId], 32, 80, 29);
	gSprites[spriteId].data[0] = 34;

	gBattleAnimTarget = GetCatchingBattler();

	gSprites[spriteId].data[1] = GetBattlerSpriteCoord(gBattleAnimTarget, 0);
	gSprites[spriteId].data[2] = GetBattlerSpriteCoord(gBattleAnimTarget, 1) - 16;
	gSprites[spriteId].callback = (void*) 0x80EF8C1;
	gBattleSpritesDataPtr->animationData->wildMonInvisible = gSprites[gBattlerSpriteIds[gBattleAnimTarget]].invisible;
	gTasks[taskId].data[0] = spriteId;
	gTasks[taskId].func = (void*) 0x80EF699;
}

void LoadBallGfx(u8 ballId)
{
//	u16 var;
	if (GetSpriteTileStartByTag(gBallSpriteSheets[ballId].tag) == 0xFFFF)
	{
		LoadCompressedSpriteSheetUsingHeap(&gBallSpriteSheets[ballId]);
		LoadCompressedSpritePaletteUsingHeap(&gBallSpritePalettes[ballId]);
	}
	/*switch (ballId) {
		case BALL_TYPE_MASTER_BALL:
		case BALL_TYPE_ULTRA_BALL:
		case BALL_TYPE_GREAT_BALL:
		case BALL_TYPE_POKE_BALL:
		case BALL_TYPE_SAFARI_BALL:
		case BALL_TYPE_NET_BALL:
		case BALL_TYPE_NEST_BALL:
		case BALL_TYPE_REPEAT_BALL:
		case BALL_TYPE_TIMER_BALL:
			var = GetSpriteTileStartByTag(gBallSpriteSheets[ballId].tag);
			LZDecompressVram(gOpenPokeballGfx, (void*)(VRAM + 0x10100 + var * 32));
			break;
	}*/
}

u16 GetBattlerPokeballItemId(u8 bank)
{
	u8 ballId;
	
	if (IsDynamaxed(bank))
		ballId = BALL_TYPE_DYNAMAX_BALL;
	else
		ballId = GetMonData(GetIllusionPartyData(bank), MON_DATA_POKEBALL, 0);

	return ballId;
}

static bool8 CantCatchBecauseRaid(void)
{
	return IsRaidBattle() && !RAID_BATTLE_END;
}

bool8 CantCatchBecauseFlag(void)
{
	return FlagGet(FLAG_NO_CATCHING) || FlagGet(FLAG_NO_CATCHING_AND_RUNNING);
}

static bool8 CantCatchBecauseTwoAlive(void)
{
	return BATTLER_ALIVE(GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT))
		&& BATTLER_ALIVE(GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT));
}

static bool8 CantCatchBecauseSemiInvulnerable(u8 pos)
{
	return BATTLER_SEMI_INVULNERABLE(GetBattlerAtPosition(pos)) != 0;
}

static bool8 CantCatchBecauseSemiInvulnerableDoubles(void)
{
	return (BATTLER_ALIVE(GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT)) && CantCatchBecauseSemiInvulnerable(B_POSITION_OPPONENT_LEFT))
		|| (BATTLER_ALIVE(GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT)) && CantCatchBecauseSemiInvulnerable(B_POSITION_OPPONENT_RIGHT));
}

static bool8 CantCatchBecauseSecondMon(void)
{
	return BATTLER_ALIVE(GetBattlerAtPosition(B_POSITION_PLAYER_LEFT))
		&& gChosenActionByBank[GetBattlerAtPosition(B_POSITION_PLAYER_LEFT)] != ACTION_USE_ITEM
		&& gChosenActionByBank[GetBattlerAtPosition(B_POSITION_PLAYER_LEFT)] != ACTION_NOTHING_FAINTED;
}

bool8 DoubleWildPokeBallItemUseFix(u8 taskId)
{
	bool8 effect = FALSE;

	if (CantCatchBecauseRaid())
	{
		DisplayItemMessageInBag(taskId, 2, gText_CantCatchPokemonYet, Task_ReturnToBagFromContextMenu);
		effect = TRUE;
	}
	else if (CantCatchBecauseFlag())
	{
		DisplayItemMessageInBag(taskId, 2, gText_CantCatchPokemonRightNow, Task_ReturnToBagFromContextMenu);
		effect = TRUE;
	}
	else if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
	{
		if (CantCatchBecauseTwoAlive())
		{
			DisplayItemMessageInBag(taskId, 2, gText_CantAimAtTwoTargets, Task_ReturnToBagFromContextMenu);
			effect = TRUE;
		}
		else if (CantCatchBecauseSemiInvulnerableDoubles())
		{
			DisplayItemMessageInBag(taskId, 2, gText_CantAimAtSemiInvulnerableTarget, Task_ReturnToBagFromContextMenu);
			effect = TRUE;
		}
		else if (CantCatchBecauseSecondMon())
		{
			DisplayItemMessageInBag(taskId, 2, gText_CantUseAMoveAndABall, Task_ReturnToBagFromContextMenu);
			effect = TRUE;
		}
	}
	else if (CantCatchBecauseSemiInvulnerable(B_POSITION_OPPONENT_LEFT))
	{
		DisplayItemMessageInBag(taskId, 2, gText_CantAimAtSemiInvulnerableTarget, Task_ReturnToBagFromContextMenu);
		effect = TRUE;
	}

	return effect;
}

bool8 CantCatchPokemonRightNow(void)
{
	if (CantCatchBecauseRaid() || CantCatchBecauseFlag())
		return TRUE;
	
	if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
	{
		if (CantCatchBecauseTwoAlive()
		|| CantCatchBecauseSemiInvulnerableDoubles()
		|| (IS_DOUBLE_BATTLE
		 && BATTLER_ALIVE(GetBattlerAtPosition(B_POSITION_PLAYER_LEFT))
		 && gActiveBattler == GetBattlerAtPosition(B_POSITION_PLAYER_RIGHT))) //Only Pokemon on left can throw ball
			return TRUE;
	}
	else if (CantCatchBecauseSemiInvulnerable(B_POSITION_OPPONENT_LEFT))
		return TRUE;

	return FALSE;
}

struct Pokemon* LoadTargetPartyData(void)
{
	return GetBankPartyData(gBankTarget);
}

u16 LoadTargetPokedexSpecies(void)
{
	u16 species = GetMonData(LoadTargetPartyData(), MON_DATA_SPECIES, NULL);

	#ifdef  SPECIES_MINIOR_SHIELD
	if (IsMinior(species))
		return SPECIES_MINIOR_SHIELD;
	#endif

	#if (defined NATIONAL_POKEDEX_PIKACHU && defined SPECIES_PIKACHU)
	if (SpeciesToNationalPokedexNum(species) == NATIONAL_POKEDEX_PIKACHU)
		return SPECIES_PIKACHU;
	#endif

	return species;
}

#define BattleScript_BallThrow (const u8*) 0x81D9A14
#define BattleScript_SafariBallThrow (const u8*) 0x81D9A3C
bool8 TryGetPokeBallBattleScript(void)
{
	if (GetPocketByItemId(gLastUsedItem) == POCKET_POKE_BALLS)
	{
		u8 ballType = ItemId_GetType(gLastUsedItem);
		if (ballType == BALL_TYPE_SAFARI_BALL)
			gBattlescriptCurrInstr = BattleScript_SafariBallThrow;
		else
			gBattlescriptCurrInstr = BattleScript_BallThrow;

		return TRUE;
	}

	return FALSE;
}

const struct SpriteTemplate* OakIntroPokeballFix(void)
{
	LoadCompressedSpriteSheetUsingHeap(&gBallSpriteSheets[BALL_TYPE_POKE_BALL]);
	LoadCompressedSpritePaletteUsingHeap(&gBallSpritePalettes[BALL_TYPE_POKE_BALL]);
	return &gBallSpriteTemplates[BALL_TYPE_POKE_BALL];
}
