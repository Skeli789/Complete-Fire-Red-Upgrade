#include "defines.h"
#include "defines_battle.h"
#include "../include/battle_string_ids.h"
#include "../include/event_data.h"
#include "../include/m4a.h"

#include "../include/constants/game_stat.h"
#include "../include/constants/songs.h"
#include "../include/constants/items.h"

#include "../include/new/battle_strings.h"
#include "../include/new/battle_util.h"
#include "../include/new/dns.h"
#include "../include/new/dynamax.h"
#include "../include/new/exp.h"
#include "../include/new/util.h"

/*
exp.c
	functions that handle the exp share and exp gain
*/

enum
{
	GetExp_Start,
	GetExp_CheckCurrentMonDeserving,
	GetExp_Calculation,
	GetExp_Distribute,
	GetExp_SetStats,
	GetExp_LevelUp,
	GetExp_PrepareLoop,
	GetExp_End,
};

enum
{
	GiveExpBattlePariticpants,
	GiveExpViaExpShare,
};

#define BattleScript_LevelUp (u8*) 0x81D89F5

extern const u16 gBaseExpBySpecies[];
extern u8 String_TeamExpGain[];

//This file's functions:
static u32 ExpCalculator(u32 a, u32 t, u32 b, u32 e, u32 L, u32 Lp, u32 p, u32 f, u32 v, u32 s);
static bool8 WasWholeTeamSentIn(u8 bank, u8 sentIn);
static bool8 SomeoneOnTeamGetsExpFromExpShare(u8 bank, u8 sentIn);
static bool8 MonGetsAffectionBoost(struct Pokemon* mon);
static bool8 IsAffectedByHardLevelCap(struct Pokemon* mon);
static void EmitExpBarUpdate(u8 a, u8 b, u32 c);
static void EmitExpTransferBack(u8 bufferId, u8 b, u8 *c);
static void Task_GiveExpToMon(u8 taskId);
static void sub_80300F4(u8 taskId);
static u32 GetExpToLevel(u8 toLevel, u8 growthRate);
static void MonGainEVs(struct Pokemon *mon, u16 defeatedSpecies);

u8 GetBadgeCount() {
	u8 badgeCount = 0;

	if (FlagGet(FLAG_BADGE01_GET)) {
		badgeCount++;
	}
	if (FlagGet(FLAG_BADGE02_GET)) {
		badgeCount++;
	}
	if (FlagGet(FLAG_BADGE03_GET)) {
		badgeCount++;
	}
	if (FlagGet(FLAG_BADGE04_GET)) {
		badgeCount++;
	}
	if (FlagGet(FLAG_BADGE05_GET)) {
		badgeCount++;
	}
	if (FlagGet(FLAG_BADGE06_GET)) {
		badgeCount++;
	}
	if (FlagGet(FLAG_BADGE07_GET)) {
		badgeCount++;
	}
	if (FlagGet(FLAG_BADGE08_GET)) {
		badgeCount++;
	}

	return badgeCount;
}

u8 GetCurrentLevelCap(void)
{
	u8 badgeCount = GetBadgeCount();

	switch (badgeCount)
	{
		case 0:  return 15; // Before 1st badge
		case 1:  return 20;
		case 2:  return 25;
		case 3:  return 30;
		case 4:  return 35;
		case 5:  return 40;
		case 6:  return 45;
		case 7:  return 50;
		case 8:  return 100; // After all badges
		default: return 100; // Just in case
	}
}

///////////////////// GAIN EXPERIENCE //////////////////////
void atk23_getexp(void)
{
	u16 item = gPlayerParty[gBattleStruct->expGetterMonId].item;
	u8 holdEffect = gItems[SanitizeItemId(item)].holdEffect;
	u32 i; // also used as stringId
	u32 sentIn;
	u32 viaExpShare = 0, viaSentIn = 0;
	u32 calculatedExp = 0;

	u16* expGiveType = &gBattleStruct->expValue;

	gBankFainted = GetBankForBattleScript(gBattlescriptCurrInstr[1]);
	sentIn = gSentPokesToOpponent[(gBankFainted & 2) >> 1]; //sentIn = 0 if Bank 1 killed, sentIn = 1 if Bank 3 killed

	switch (gBattleScripting.expStateTracker)
	{
	case GetExp_Start: // check if should receive exp at all

		if (SIDE(gBankFainted) != B_SIDE_OPPONENT
		|| gBattleStruct->givenExpMons & gBitTable[gBattlerPartyIndexes[gBankFainted]]
		|| IsFrontierRaidBattle()
		|| (gBattleTypeFlags &
			 (BATTLE_TYPE_LINK
			  | BATTLE_TYPE_SAFARI
			  | BATTLE_TYPE_EREADER_TRAINER
			  | BATTLE_TYPE_BENJAMIN_BUTTERFREE
			  | BATTLE_TYPE_FRONTIER
			  | BATTLE_TYPE_TRAINER_TOWER)))
		{
			goto END_EXP_GIVE; // goto last case
		}
		else
		{
			gBattleScripting.expStateTracker++;
			gBattleStruct->givenExpMons |= gBitTable[gBattlerPartyIndexes[gBankFainted]]; //The party indices in the opponent's party that have fainted and been given exp for
			gBattleStruct->expGetterMonId = 0;
			gBattleStruct->sentInPokes = sentIn;
			gNewBS->SentInBackup = sentIn;
			gBattleMoveDamage = 0;
			*expGiveType = GiveExpBattlePariticpants; //Start with battle participants
		}
		break;

	case GetExp_CheckCurrentMonDeserving: //Check if Current mon deserves EXP
		if (GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_SPECIES, NULL) == SPECIES_NONE
		||  GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_HP, NULL) == 0
		||  GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_IS_EGG, 0))
		{
			gBattleStruct->sentInPokes >>= 1; //One less pokemon to distribute exp to
			gBattleScripting.expStateTracker = GetExp_PrepareLoop;
			gBattleMoveDamage = 0; // used for exp
			break;
		}
		#ifndef FLAG_EXP_SHARE
		else if (holdEffect != ITEM_EFFECT_EXP_SHARE && !(gBattleStruct->sentInPokes & 1))
		{
			gBattleStruct->sentInPokes >>= 1;
			gBattleScripting.expStateTracker = GetExp_PrepareLoop;
			gBattleMoveDamage = 0; // used for exp
			break;
		}
		#else //New Exp Share
		else if (*expGiveType == GiveExpBattlePariticpants && !(gBattleStruct->sentInPokes & 1)) //Exp Share runs on a different loop
		{
			gBattleStruct->sentInPokes >>= 1;
			gBattleScripting.expStateTracker = GetExp_PrepareLoop;
			gBattleMoveDamage = 0; // used for exp
			break;
		}
		else if (*expGiveType == GiveExpViaExpShare && gBattleStruct->sentInPokes & 1)
		{
			gBattleStruct->sentInPokes >>= 1;
			gBattleScripting.expStateTracker = GetExp_PrepareLoop;
			gBattleMoveDamage = 0; // used for exp
			break;
		}
		#endif

		if (gPlayerParty[gBattleStruct->expGetterMonId].level >= MAX_LEVEL) //Max level mons still gain EVs
		{
			MonGainEVs(&gPlayerParty[gBattleStruct->expGetterMonId], gBattleMons[gBankFainted].species);
			gBattleStruct->sentInPokes >>= 1; //One less pokemon to distribute exp to
			gBattleScripting.expStateTracker = GetExp_PrepareLoop;
			gBattleMoveDamage = 0; // used for exp
			break;
		}

		gBattleScripting.expStateTracker++;
	__attribute__ ((fallthrough));

	case GetExp_Calculation:	; // calculate experience points to redistribute
		u32 trainerBonus, tradeBonus, baseExp, eggBoost, defLevel, pokeLevel, passPower, affection, evolutionBoost, divisor;

		for (viaSentIn = 0, i = 0; i < PARTY_SIZE; i++)
		{
			if (gPlayerParty[i].species == SPECIES_NONE || gPlayerParty[i].hp == 0)
				continue;

			if (gBitTable[i] & sentIn)
				viaSentIn++;
			#ifdef FLAG_EXP_SHARE //In new gens, you either get bonus for being in, or exp share
			else if (FlagGet(FLAG_EXP_SHARE))
				viaExpShare++;
			#endif

			#ifndef FLAG_EXP_SHARE
			if (ItemId_GetHoldEffect(gPlayerParty[i].item) == ITEM_EFFECT_EXP_SHARE)
					viaExpShare++;
			#endif
		}

		if (IsAffectedByHardLevelCap(&gPlayerParty[gBattleStruct->expGetterMonId]))
		{
			calculatedExp = 1; //Doesn't really gain Exp. if above level cap
			goto SKIP_EXP_CALC;
		}

		//Trainer Boost - a
		trainerBonus = 10;
		#ifdef TRAINER_EXP_BOOST
			if (gBattleTypeFlags & BATTLE_TYPE_TRAINER
			#ifdef UNBOUND
			&& FlagGet(FLAG_SYS_GAME_CLEAR) //Too OP before game end
			#endif
			)
				trainerBonus = 15;
		#endif

		//Trade Boost - t
		tradeBonus = 10;
		if (IsTradedMon(&gPlayerParty[gBattleStruct->expGetterMonId]))
			tradeBonus = 15;

		//Base Experience - b
		#ifdef GEN_7_BASE_EXP_YIELD
			baseExp = gBaseExpBySpecies[gBattleMons[gBankFainted].species];
		#else
			baseExp = gBaseStats[gBattleMons[gBankFainted].species].expYield;
		#endif

		//Lucky Egg Boost - e
		eggBoost = 10;
		if (holdEffect == ITEM_EFFECT_LUCKY_EGG)
		{
			eggBoost = 15;

			#ifdef VAR_LUCKY_EGG_LEVEL
			u16 luckyEggLevel = VarGet(VAR_LUCKY_EGG_LEVEL);
			if (luckyEggLevel >= 2)
				eggBoost += (luckyEggLevel - 1) * 5; //Increases by 0.5 for each level above 1
			#endif
		}

		//Level of Fainted Mon - L
		defLevel = gBattleMons[gBankFainted].level;

		//Level of Victorious Mon - Lp
		pokeLevel = gPlayerParty[gBattleStruct->expGetterMonId].level;

		//Pass Power Bonus - Not implemented
		passPower = 1;

		//Affection Boost - f
		affection = 10;
		if (MonGetsAffectionBoost(&gPlayerParty[gBattleStruct->expGetterMonId]))
			affection = 12;

		//Evolution Boost - v
		evolutionBoost = 10;
		if (CouldHaveEvolvedViaLevelUp(&gPlayerParty[gBattleStruct->expGetterMonId]))
			evolutionBoost = 12;

		//Exp Share/Num Battlers Divisor
		#ifndef FLAG_EXP_SHARE
			#ifdef OLD_EXP_SPLIT
				if (viaExpShare) // at least one mon is getting exp via exp share
				{
					if (holdEffect == ITEM_EFFECT_EXP_SHARE)
						calculatedExp += ExpCalculator(trainerBonus, tradeBonus, baseExp, eggBoost, defLevel, pokeLevel, passPower, affection, evolutionBoost, 2 * viaExpShare);
					if (gNewBS->SentInBackup & (1 << gBattleStruct->expGetterMonId))
						calculatedExp += ExpCalculator(trainerBonus, tradeBonus, baseExp, eggBoost, defLevel, pokeLevel, passPower, affection, evolutionBoost, 2 * viaSentIn);
					goto SKIP_EXP_CALC;
				}
				else //No Pokemon holds Exp Share
					divisor = viaSentIn;
			#else
				divisor = 1;
			#endif
		#else //New Exp. Share
			#ifdef OLD_EXP_SPLIT
				if (*expGiveType == GiveExpBattlePariticpants) //In battle recieves full EXP / num battlers
					divisor = viaSentIn;
				else //Otherwise Exp / 2
					divisor = 2 * viaSentIn;
			#else
				if (*expGiveType == GiveExpBattlePariticpants) //In battle recieves full EXP
					divisor = 1;
				else //Otherwise Exp / 2
					divisor = 2;
			#endif
		#endif

		calculatedExp = ExpCalculator(trainerBonus, tradeBonus, baseExp, eggBoost, defLevel, pokeLevel, passPower, affection, evolutionBoost, divisor);

	SKIP_EXP_CALC:
		calculatedExp = MathMax(1, calculatedExp);
		gBattleMoveDamage = calculatedExp;

		gBattleScripting.expStateTracker++;
	__attribute__ ((fallthrough));

	case GetExp_Distribute: // set exp value to the poke in expgetter_id and print message

		// music change in wild battle after fainting a poke
		if (!(gBattleTypeFlags & (BATTLE_TYPE_TRAINER | BATTLE_TYPE_POKE_DUDE))
		&& !gBattleStruct->wildVictorySong)
		{
			if ((IS_DOUBLE_BATTLE && (gBattleMons[0].hp || gBattleMons[2].hp) && gBattleMons[1].hp == 0 && gBattleMons[3].hp == 0)
			|| (!(IS_DOUBLE_BATTLE) && gBattleMons[0].hp && gBattleMons[1].hp == 0))
			{
				BattleStopLowHpSound();

				#ifdef UNBOUND
				if (gBattleTypeFlags & BATTLE_TYPE_SHADOW_WARRIOR)
					PlayBGM(BGM_VICTORY_PLASMA);
				else
				#endif
				if (!IsRaidBattle()) //Music would be played if the player caught the raid boss
					PlayBGM(BGM_VICTORY_WILD_POKE); //Wild PKMN Victory

				gBattleStruct->wildVictorySong++;
				//gAbsentBattlerFlags |= gBitTable[gBankFainted];
			}
		}

		if (gPlayerParty[gBattleStruct->expGetterMonId].hp)
		{
			//Check Boosted String necessary
			if (IsTradedMon(&gPlayerParty[gBattleStruct->expGetterMonId])
			||  CouldHaveEvolvedViaLevelUp(&gPlayerParty[gBattleStruct->expGetterMonId])
			||  holdEffect == ITEM_EFFECT_LUCKY_EGG
		    ||  MonGetsAffectionBoost(&gPlayerParty[gBattleStruct->expGetterMonId]))
			{
				// check if the pokemon doesn't belong to the player
				if (gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER && gBattleStruct->expGetterMonId >= 3)
					i = STRINGID_EMPTYSTRING4;
				else
					i = STRINGID_ABOOSTED;
			}
			else
				i = STRINGID_EMPTYSTRING4;

			//Get exp getter bank
			if (IS_DOUBLE_BATTLE)
			{
				if (gBattlerPartyIndexes[B_POSITION_PLAYER_RIGHT] == gBattleStruct->expGetterMonId && !(gAbsentBattlerFlags & gBitTable[B_POSITION_PLAYER_RIGHT]))
					gBattleStruct->expGetterBank = B_POSITION_PLAYER_RIGHT;
				else
				{
					if (!(gAbsentBattlerFlags & gBitTable[B_POSITION_PLAYER_LEFT]))
						gBattleStruct->expGetterBank = B_POSITION_PLAYER_LEFT;
					else
						gBattleStruct->expGetterBank = B_POSITION_PLAYER_RIGHT;
				}
			}
			else
				gBattleStruct->expGetterBank = B_POSITION_PLAYER_LEFT;

			PREPARE_MON_NICK_WITH_PREFIX_BUFFER(gBattleTextBuff1, gBattleStruct->expGetterBank, gBattleStruct->expGetterMonId)

			// buffer 'gained' or 'gained a boosted'
			PREPARE_STRING_BUFFER(gBattleTextBuff2, i)

			PREPARE_WORD_NUMBER_BUFFER(gBattleTextBuff3, 10, calculatedExp)

			#ifdef FLAG_EXP_SHARE
				if (*expGiveType == GiveExpBattlePariticpants) //Don't print the gained exp string unless it participated in battle
					PrepareStringBattle(STRINGID_PKMNGAINEDEXP, gBattleStruct->expGetterBank);
			#else
				PrepareStringBattle(STRINGID_PKMNGAINEDEXP, gBattleStruct->expGetterBank);
			#endif
			MonGainEVs(&gPlayerParty[gBattleStruct->expGetterMonId], gBattleMons[gBankFainted].species);
		}
		else
		{
			gBattleStruct->sentInPokes >>= 1;
			gBattleScripting.expStateTracker = GetExp_PrepareLoop;
			gBattleMoveDamage = 0; // used for exp
			break;
		}
		gBattleStruct->sentInPokes >>= 1;
		gBattleScripting.expStateTracker++;
	__attribute__ ((fallthrough));

	case GetExp_SetStats: // Set stats and give exp
		if (gBattleExecBuffer) break;

		gBattleBufferB[gBattleStruct->expGetterBank][0] = 0;
		if (gPlayerParty[gBattleStruct->expGetterMonId].hp && gPlayerParty[gBattleStruct->expGetterMonId].level < MAX_LEVEL)
		{
			gBattleResources->statsBeforeLvlUp->hp = gPlayerParty[gBattleStruct->expGetterMonId].maxHP;
			gBattleResources->statsBeforeLvlUp->atk = gPlayerParty[gBattleStruct->expGetterMonId].attack;
			gBattleResources->statsBeforeLvlUp->def = gPlayerParty[gBattleStruct->expGetterMonId].defense;
			gBattleResources->statsBeforeLvlUp->spd = gPlayerParty[gBattleStruct->expGetterMonId].speed;
			gBattleResources->statsBeforeLvlUp->spAtk = gPlayerParty[gBattleStruct->expGetterMonId].spAttack;
			gBattleResources->statsBeforeLvlUp->spDef = gPlayerParty[gBattleStruct->expGetterMonId].spDefense;

			gActiveBattler = gBattleStruct->expGetterBank;
			EmitExpBarUpdate(0, gBattleStruct->expGetterMonId, gBattleMoveDamage);
			MarkBufferBankForExecution(gActiveBattler);
		}
		else
		{
			gBattleScripting.expStateTracker = GetExp_PrepareLoop;
			gBattleMoveDamage = 0; // used for exp
			break;
		}
		gBattleScripting.expStateTracker++;
		break;

	case GetExp_LevelUp: // lvl up if necessary
		if (gBattleExecBuffer) break;

		gActiveBattler = gBattleStruct->expGetterBank;
		if (gBattleBufferB[gActiveBattler][0] == CONTROLLER_CHOSENMONRETURNVALUE
		&&  gBattleBufferB[gActiveBattler][1] == RET_VALUE_LEVELED_UP)
		{
			u8 leveledUpBank = 0xFF;

			if (gBattleTypeFlags & BATTLE_TYPE_TRAINER && gBattlerPartyIndexes[gActiveBattler] == gBattleStruct->expGetterMonId)
				HandleLowHpMusicChange(&gPlayerParty[gBattlerPartyIndexes[gActiveBattler]], gActiveBattler);

			PREPARE_MON_NICK_WITH_PREFIX_BUFFER(gBattleTextBuff1, gActiveBattler, gBattleStruct->expGetterMonId)

			PREPARE_BYTE_NUMBER_BUFFER(gBattleTextBuff2, 3, gPlayerParty[gBattleStruct->expGetterMonId].level)

			BattleScriptPushCursor();
			gLeveledUpInBattle |= gBitTable[gBattleStruct->expGetterMonId];
			gBattlescriptCurrInstr = BattleScript_LevelUp;
			gBattleMoveDamage = (gBattleBufferB[gActiveBattler][2] | (gBattleBufferB[gActiveBattler][3] << 8) | (gBattleBufferB[gActiveBattler][4] << 0x10) | (gBattleBufferB[gActiveBattler][5] << 0x18));
 			AdjustFriendship(&gPlayerParty[gBattleStruct->expGetterMonId], FRIENDSHIP_EVENT_GROW_LEVEL);

			// update battle mon structure after level up
			if (gBattlerPartyIndexes[GetBattlerAtPosition(B_POSITION_PLAYER_LEFT)] == gBattleStruct->expGetterMonId && BATTLER_ALIVE(GetBattlerAtPosition(B_POSITION_PLAYER_LEFT)))
				leveledUpBank = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);
			else if (IS_DOUBLE_BATTLE && gBattlerPartyIndexes[GetBattlerAtPosition(B_POSITION_PLAYER_RIGHT)] == gBattleStruct->expGetterMonId && BATTLER_ALIVE(GetBattlerAtPosition(B_POSITION_PLAYER_RIGHT)))
				leveledUpBank = GetBattlerAtPosition(B_POSITION_PLAYER_RIGHT);

			if (leveledUpBank != 0xFF)
			{
				gBattleMons[leveledUpBank].level = gPlayerParty[gBattleStruct->expGetterMonId].level;
				gBattleMons[leveledUpBank].hp = gPlayerParty[gBattleStruct->expGetterMonId].hp;
				gBattleMons[leveledUpBank].maxHP = gPlayerParty[gBattleStruct->expGetterMonId].maxHP;

				if (!IS_TRANSFORMED(leveledUpBank)) //Don't replace the temp stats
				{
					gBattleMons[leveledUpBank].attack = gPlayerParty[gBattleStruct->expGetterMonId].attack;
					gBattleMons[leveledUpBank].defense = gPlayerParty[gBattleStruct->expGetterMonId].defense;
					gBattleMons[leveledUpBank].speed = gPlayerParty[gBattleStruct->expGetterMonId].speed;
					gBattleMons[leveledUpBank].spAttack = gPlayerParty[gBattleStruct->expGetterMonId].spAttack;
					gBattleMons[leveledUpBank].spDefense = gPlayerParty[gBattleStruct->expGetterMonId].spDefense;
				}
			}
		}
		else
			gBattleMoveDamage = 0;

		gBattleScripting.expStateTracker++;
		break;

	case GetExp_PrepareLoop: // looper increment
		if (gBattleMoveDamage) // there is exp to give, goto case 3 that gives exp
			gBattleScripting.expStateTracker = GetExp_SetStats;
		else
		{
			gBattleStruct->expGetterMonId++;
			if (gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER)
			{
				if (gBattleStruct->expGetterMonId < PARTY_SIZE / 2)
					gBattleScripting.expStateTracker = GetExp_CheckCurrentMonDeserving; // loop again
				else
					gBattleScripting.expStateTracker = GetExp_End; // we're done
			}
			else
			{
				if (gBattleStruct->expGetterMonId < PARTY_SIZE)
					gBattleScripting.expStateTracker = GetExp_CheckCurrentMonDeserving; // loop again
				else
					gBattleScripting.expStateTracker = GetExp_End; // we're done
			}
		}
		break;

	case GetExp_End: // increment instruction
		if (gBattleExecBuffer) break;

		#ifdef FLAG_EXP_SHARE
			if (FlagGet(FLAG_EXP_SHARE) && *expGiveType == GiveExpBattlePariticpants && !WasWholeTeamSentIn(B_POSITION_PLAYER_LEFT, gNewBS->SentInBackup))
			{
				*expGiveType = GiveExpViaExpShare;
				gBattleStruct->expGetterMonId = 0;
				gBattleMoveDamage = 0;
				gBattleStruct->sentInPokes = gNewBS->SentInBackup;
				gBattleScripting.expStateTracker = GetExp_CheckCurrentMonDeserving; // Time for Exp Share loop
				if (SomeoneOnTeamGetsExpFromExpShare(B_POSITION_PLAYER_LEFT, gNewBS->SentInBackup)) //Still give EVs, but don't print message if no Exp gained
				{
					gBattleStringLoader = String_TeamExpGain;
					PrepareStringBattle(0x184, 0);
				}
			}
			else
		#endif
			{
				END_EXP_GIVE:
				gBattleMons[gBankFainted].item = 0;
				gBattleMons[gBankFainted].ability = 0;
				gBattlescriptCurrInstr += 2;
			}
		break;
	}
}

static u32 ExpCalculator(u32 a, u32 t, u32 b, u32 e, u32 L, u32 Lp, u32 p, u32 f, u32 v, u32 s) {
	u32 calculatedExp;

	#ifdef FLAT_EXP_FORMULA
		++Lp; //So the variable doesn't remain unused
		calculatedExp = udivsi(a * t * b * e * L * p * v, 10 * 10 * 10 * 10); //Did the calcs, shouldn't overflow (unless Base Exp > 1060)
		calculatedExp = (calculatedExp * f) / 10; //Affection boost
		calculatedExp = udivsi(calculatedExp, 7 * s);

	#else //Scaled Formula Gens 5, 7
		u32 calculatedExpTop, calculatedExpBottom;

		calculatedExp = udivsi(a * b * L, 10 * 5 * s);

		calculatedExpTop = (2 * L + 10) * (2 * L + 10); //* Sqrt(2 * L + 10); //(2 * L + 10)**2.5
		calculatedExpBottom = (L + Lp + 10) * (L + Lp + 10) * Sqrt(L + Lp + 10); //(L + Lp + 10)**2.5

		calculatedExp *= calculatedExpTop;
		calculatedExp = udivsi(calculatedExp, calculatedExpBottom);
		calculatedExp *= Sqrt(2 * L + 10); //Best way to prevent overflow; now Base Exp < 3245 works

		calculatedExp += 1;

		calculatedExp = (udivsi(calculatedExp * t * e * v, 10 * 10 * 10) * p * f) / 10;
	#endif

	if (IsRaidBattle())
		calculatedExp *= 2;

	return MathMin(1640000, calculatedExp);
}

static bool8 WasWholeTeamSentIn(u8 bank, u8 sentIn)
{
	u8 start, end;
	int i;

	struct Pokemon* party = LoadPartyRange(bank, &start, &end);

	for (i = start; i < end; ++i) {
		if (party[i].species == 0)
			return TRUE;

		if (party[i].hp == 0 || GetMonData(&party[i], MON_DATA_IS_EGG, 0) /*|| party[i].level >= MAX_LEVEL*/) //Pokemon can still gain EVs at Max Level so don't check this
			continue;

		if (!(sentIn & gBitTable[i]))
			return FALSE;
	}

	return TRUE;
}

static bool8 SomeoneOnTeamGetsExpFromExpShare(u8 bank, u8 sentIn)
{
	u8 start, end;
	int i;

	struct Pokemon* party = LoadPartyRange(bank, &start, &end);

	for (i = start; i < end; ++i)
	{
		if (party[i].species == 0)
			continue;

		if (party[i].hp == 0 || GetMonData(&party[i], MON_DATA_IS_EGG, 0) || party[i].level >= MAX_LEVEL)
			continue;

		if (!(sentIn & gBitTable[i]))
			return TRUE;
	}

	return FALSE;
}

static bool8 MonGetsAffectionBoost(struct Pokemon* mon)
{
	if (GetMonData(mon, MON_DATA_FRIENDSHIP, NULL) >= 220)
	{
		#ifdef EXP_AFFECTION_BOOST
			#ifdef UNBOUND
			if (FlagGet(FLAG_SYS_GAME_CLEAR)) //Too OP before game end
			#endif
				return TRUE;
		#endif
	}

	return FALSE;
}

static bool8 IsAffectedByHardLevelCap(unusedArg struct Pokemon* mon)
{
	#ifdef FLAG_HARD_LEVEL_CAP
	extern u8 GetCurrentLevelCap(void); //Must be implemented yourself
	if (FlagGet(FLAG_HARD_LEVEL_CAP))
	{
		if (GetMonData(mon, MON_DATA_LEVEL, NULL) >= GetCurrentLevelCap())
			return TRUE;
	}
	#endif

	return FALSE;
}

static void EmitExpBarUpdate(u8 a, u8 b, u32 c) //Changed the u16 to a u32 to allow for more exp gain
{
	gBattleBuffersTransferData[0] = 25;
	gBattleBuffersTransferData[1] = b;
	gBattleBuffersTransferData[2] = c;
	gBattleBuffersTransferData[3] = (c & 0xFF00)	 >> 0x08;
	gBattleBuffersTransferData[4] = (c & 0xFF0000)   >> 0x10;
	gBattleBuffersTransferData[5] = (c & 0xFF000000) >> 0x18;
	PrepareBufferDataTransfer(a, gBattleBuffersTransferData, 6);
}

static void EmitExpTransferBack(u8 bufferId, u8 b, u8 *c)
{
	s32 i;

	gBattleBuffersTransferData[0] = CONTROLLER_CHOSENMONRETURNVALUE;
	gBattleBuffersTransferData[1] = b;
	for (i = 0; i < 4; i++)
		gBattleBuffersTransferData[2 + i] = c[i];
	PrepareBufferDataTransfer(bufferId, gBattleBuffersTransferData, 6);
}

#define tExpTask_monId      data[0]
#define tExpTask_battler    data[2]
#define tExpTask_gainedExp1 data[3]
#define tExpTask_gainedExp2 data[4]
#define tExpTask_frames     data[10]
static void UpdateDailyExpGameStat(u32 gainedExp)
{
	if (!IsTimeInVarInFuture(VAR_SWARM_DAILY_EVENT))
	{
		u32 totalExpGainedToday = GetGameStat(GAME_STAT_EXP_EARNED_TODAY);
		u32 remainingPossibleInGameStat = 0xFFFFFFFF - totalExpGainedToday;

		if (gainedExp > remainingPossibleInGameStat)		
			SetGameStat(GAME_STAT_EXP_EARNED_TODAY, 0xFFFFFFFF);
		else
			SetGameStat(GAME_STAT_EXP_EARNED_TODAY, totalExpGainedToday + gainedExp);
	}
	else
	{
		SetGameStat(GAME_STAT_EXP_EARNED_TODAY, 0);
	}
}

void PlayerHandleExpBarUpdate(void)
{
	u8 monId = gBattleBufferA[gActiveBattler][1];

	if (gPlayerParty[monId].level >= MAX_LEVEL)
	{
		PlayerBufferExecCompleted();
	}
	else
	{
		s32 gainedExp;
		u8 taskId;

		load_gfxc_health_bar(1);
		gainedExp = T1_READ_32(&gBattleBufferA[gActiveBattler][2]);
		UpdateDailyExpGameStat(gainedExp);

		taskId = CreateTask(Task_GiveExpToMon, 10);
		gTasks[taskId].tExpTask_monId = monId;
		gTasks[taskId].tExpTask_battler = gActiveBattler;
		gTasks[taskId].tExpTask_gainedExp1 = gainedExp;
		gTasks[taskId].tExpTask_gainedExp2 = gainedExp >> 0x10;

		gBattlerControllerFuncs[gActiveBattler] = (void*) (0x802E310 | 1);
	}
}

static void Task_GiveExpToMon(u8 taskId)
{
	u8 monId = gTasks[taskId].tExpTask_monId;
	u8 bank = gTasks[taskId].tExpTask_battler;
	s32 gainedExp = (u16) gTasks[taskId].tExpTask_gainedExp1 | ((u16) gTasks[taskId].tExpTask_gainedExp2 << 0x10);

	struct Pokemon* mon = &gPlayerParty[monId];
	u16 species = GetMonData(mon, MON_DATA_SPECIES, NULL);
	u8 level = GetMonData(mon, MON_DATA_LEVEL, NULL);
	u32 currExp = GetMonData(mon, MON_DATA_EXP, NULL);
	u32 nextLvlExp = GetExpToLevel(level + 1, gBaseStats[species].growthRate);

	if (IS_DOUBLE_BATTLE || monId != gBattlerPartyIndexes[bank]) //Give exp without moving the expbar.
	{
		if (currExp + gainedExp >= nextLvlExp)
		{
			u8 savedActiveBattler = gActiveBattler;

			SetMonData(mon, MON_DATA_EXP, &nextLvlExp);
			CalculateMonStats(mon);
			if (monId == gBattlerPartyIndexes[bank]) //Pokemon in battle
				TryBoostDynamaxHPAfterLevelUp(bank);
			gainedExp -= (nextLvlExp - currExp);

			if (IsAffectedByHardLevelCap(mon))
				gainedExp = 0; //Don't gain any more Exp.

			gActiveBattler = bank;
			EmitExpTransferBack(1, RET_VALUE_LEVELED_UP, (u8*) (&gainedExp)); //Used to be EmitTwoReturnValues, but Cmd34 allows for more data transfer
			gActiveBattler = savedActiveBattler;

			if (IS_DOUBLE_BATTLE && (monId == gBattlerPartyIndexes[bank] || monId == gBattlerPartyIndexes[bank ^ BIT_FLANK]))
				gTasks[taskId].func = Task_LaunchLvlUpAnim;
			else
				gTasks[taskId].func = DestroyExpTaskAndCompleteOnInactiveTextPrinter;
		}
		else
		{
			currExp += gainedExp;
			SetMonData(mon, MON_DATA_EXP, &currExp);
			gBattlerControllerFuncs[bank] = CompleteOnInactiveTextPrinter;
			DestroyTask(taskId);
		}
	}
	else //Single Battle
	{
		u32 currLvlExp = GetExpToLevel(level, gBaseStats[species].growthRate);
		u32 totalExpToNextLvl = nextLvlExp - currLvlExp;

		PlaySE(SE_EXP);
		SetBattleBarStruct(bank, gHealthboxSpriteIds[bank], totalExpToNextLvl, currExp - currLvlExp, -gainedExp); //sub_8043D84 in Ruby
		gTasks[taskId].func = sub_80300F4;
	}
}

static void sub_80300F4(u8 taskId)
{
	if (gTasks[taskId].data[10] < 13)
	{
		gTasks[taskId].data[10]++;
	}
	else
	{
		u8 monId = gTasks[taskId].tExpTask_monId;
		u8 bank = gTasks[taskId].tExpTask_battler;
		s32 gainedExp = (u16) gTasks[taskId].tExpTask_gainedExp1 | ((u16) gTasks[taskId].tExpTask_gainedExp2 << 0x10);
		s32 newExpPoints = MoveBattleBar(bank, gHealthboxSpriteIds[bank], EXP_BAR, 0);

		SetHealthboxSpriteVisible(gHealthboxSpriteIds[bank]);
		if (newExpPoints == -1)
		{
			struct Pokemon* mon = &gPlayerParty[monId];
			u16 species = GetMonData(mon, MON_DATA_SPECIES, NULL);
			u8 level = GetMonData(mon, MON_DATA_LEVEL, NULL);
			u32 currExp = GetMonData(mon, MON_DATA_EXP, NULL);
			u32 nextLvlExp = GetExpToLevel(level + 1, gBaseStats[species].growthRate);

			m4aSongNumStop(SE_EXP);

			if (currExp + gainedExp >= nextLvlExp)
			{
				u8 savedActiveBattler = gActiveBattler;

				SetMonData(mon, MON_DATA_EXP, &nextLvlExp);
				CalculateMonStats(mon);
				TryBoostDynamaxHPAfterLevelUp(bank);
				gainedExp -= (nextLvlExp - currExp);

				if (IsAffectedByHardLevelCap(mon))
					gainedExp = 0; //Don't gain any more Exp.

				gActiveBattler = bank;
				EmitExpTransferBack(1, RET_VALUE_LEVELED_UP, (u8*) &gainedExp);

				gActiveBattler = savedActiveBattler;
				gTasks[taskId].func = Task_LaunchLvlUpAnim;
			}
			else
			{
				currExp += gainedExp;
				SetMonData(mon, MON_DATA_EXP, &currExp);
				gBattlerControllerFuncs[bank] = CompleteOnInactiveTextPrinter;
				DestroyTask(taskId);
			}
		}
	}
}

static u32 GetExpToLevel(u8 toLevel, u8 growthRate)
{
	return gExperienceTables[growthRate][toLevel];
}

u32 GetSpeciesExpToLevel(u16 species, u8 toLevel)
{
	return GetExpToLevel(toLevel, gBaseStats[species].growthRate);
}

//////////////////// POWER ITEMS ////////////////////////////////
static void MonGainEVs(struct Pokemon *mon, u16 defeatedSpecies)
{
	u16 evIncrease;
	u16 heldItem = GetMonData(mon, MON_DATA_HELD_ITEM, NULL);
	u8 holdEffect = ItemId_GetHoldEffect(heldItem);
	u8 itemQuality = ItemId_GetHoldEffectParam(heldItem);
	u8 pkrsMultiplier = (CheckPartyHasHadPokerus(mon, 0)) ? 2 : 1;

	if (GetMonEVCount(mon) >= MAX_TOTAL_EVS)
		return;

	for (u8 stat = 0; stat < NUM_STATS; ++stat, evIncrease = 0)
	{
		if (GetMonData(mon, MON_DATA_HP_EV + stat, NULL) >= EV_CAP)
			continue;

		//Get EV yield
		switch (stat)
		{
			case STAT_HP:
				evIncrease = gBaseStats[defeatedSpecies].evYield_HP;
				break;
			case STAT_ATK:
				evIncrease = gBaseStats[defeatedSpecies].evYield_Attack;
				break;
			case STAT_DEF:
				evIncrease = gBaseStats[defeatedSpecies].evYield_Defense;
				break;
			case STAT_SPEED:
				evIncrease = gBaseStats[defeatedSpecies].evYield_Speed;
				break;
			case STAT_SPATK:
				evIncrease = gBaseStats[defeatedSpecies].evYield_SpAttack;
				break;
			case STAT_SPDEF:
				evIncrease = gBaseStats[defeatedSpecies].evYield_SpDefense;
				break;
		}

		//Pokerus
		evIncrease *= pkrsMultiplier;

		//Check Macho Brace
		if (holdEffect == ITEM_EFFECT_MACHO_BRACE && itemQuality == QUALITY_MACHO_BRACE)
		{
			#ifdef VAR_MACHO_BRACE_LEVEL
			evIncrease *= MathMin(max(2, VarGet(VAR_MACHO_BRACE_LEVEL)), EV_CAP);
			#else
			evIncrease *= 2;
			#endif
		}

		AddEVs(mon, stat, evIncrease);

		//Check Power Item
		if (holdEffect == ITEM_EFFECT_MACHO_BRACE && itemQuality > 0 && itemQuality - 1 == stat)
		{
			evIncrease = POWER_ITEM_EV_YIELD * pkrsMultiplier;

			#ifdef VAR_POWER_ITEM_LEVEL
			u8 powerItemLevel = VarGet(VAR_POWER_ITEM_LEVEL);
			if (powerItemLevel >= 2) //At least doubling
				evIncrease *= MathMin(powerItemLevel, EV_CAP);
			#endif

			AddEVs(mon, stat, evIncrease); //Power items always add to requested stat
		}
	}
}

bool8 AddEVs(struct Pokemon* mon, u8 statId, u16 numToAdd)
{
	u8 currentEv = GetMonData(mon, MON_DATA_HP_EV + statId, NULL);
	u16 totalEvs = GetMonEVCount(mon);

	if (totalEvs < MAX_TOTAL_EVS
	&&  currentEv < EV_CAP) //The EV can be added
	{
		if (totalEvs + numToAdd > MAX_TOTAL_EVS)
			numToAdd = MAX_TOTAL_EVS - totalEvs; //Lower amount

		if (currentEv + numToAdd > EV_CAP)
			numToAdd = EV_CAP - currentEv; //Lower amount

		currentEv += numToAdd;
		SetMonData(mon, MON_DATA_HP_EV + statId, &currentEv);

		if (numToAdd > 0)
			return TRUE; //Evs successfully added
	}

	return FALSE; //No EVs were added
}
