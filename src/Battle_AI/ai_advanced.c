#include "../defines.h"
#include "../defines_battle.h"
#include "../../include/random.h"
#include "../../include/constants/items.h"

#include "../../include/new/accuracy_calc.h"
#include "../../include/new/ai_advanced.h"
#include "../../include/new/ai_util.h"
#include "../../include/new/ai_master.h"
#include "../../include/new/battle_start_turn_start.h"
#include "../../include/new/battle_script_util.h"
#include "../../include/new/battle_util.h"
#include "../../include/new/damage_calc.h"
#include "../../include/new/dynamax.h"
#include "../../include/new/item.h"
#include "../../include/new/multi.h"
#include "../../include/new/util.h"

/*
ai_advanced.c
	advanced logic for the AI, including move prediction and fight classes
*/

enum FightingClasses
{
	FIGHT_CLASS_NONE,
	FIGHT_CLASS_SWEEPER_KILL,
	FIGHT_CLASS_SWEEPER_SETUP_STATS,
	FIGHT_CLASS_SWEEPER_SETUP_STATUS,
	FIGHT_CLASS_STALL,
	FIGHT_CLASS_TEAM_SUPPORT_BATON_PASS,
	FIGHT_CLASS_TEAM_SUPPORT_CLERIC,
	FIGHT_CLASS_TEAM_SUPPORT_SCREENS,
	FIGHT_CLASS_TEAM_SUPPORT_PHAZING,
	FIGHT_CLASS_ENTRY_HAZARDS,

//For Doubles
	FIGHT_CLASS_DOUBLES_ALL_OUT_ATTACKER,
	FIGHT_CLASS_DOUBLES_SETUP_ATTACKER,
	FIGHT_CLASS_DOUBLES_TRICK_ROOM_ATTACKER,
	FIGHT_CLASS_DOUBLES_TRICK_ROOM_SETUP,
	FIGHT_CLASS_DOUBLES_UTILITY,
	FIGHT_CLASS_DOUBLES_PHAZING,
	FIGHT_CLASS_DOUBLES_TEAM_SUPPORT,
	NUM_FIGHT_CLASSES,
};

//Each doubles spread type uses different damaging moves at different times.
//This table assigns each double class the priority for each of the 12 levels
//of damaging moves.
//Only one move can get one of these damage bonuses at a time. That is why the
//lowest levels all have the same priority. This doesn't necessarily mean use a
//level 4 damaging move at the same rate as level 5 damaging moves. This means
//both level 4 and level 5 damaging moves are considered less priority than what
//comes above them.
//For more info see src/Battle_AI/"Doubles AI Strategy.txt"
u8 gDoublesDamageViabilityMapping[NUM_FIGHT_CLASSES - FIGHT_CLASS_DOUBLES_ALL_OUT_ATTACKER][BEST_DOUBLES_KO_SCORE + 1] =
{
	[FIGHT_CLASS_DOUBLES_ALL_OUT_ATTACKER - FIGHT_CLASS_DOUBLES_ALL_OUT_ATTACKER] =
	{
		[12] = 19,
		[11] = 17,
		[10] = 16,
		[ 9] = 14,
		[ 8] = 13,
		[ 7] = 11,
		[ 6] = 11,
		[ 5] = 10,
		[ 4] = 9,
		[ 3] = 9,
		[ 2] = 9,
		[ 1] = 9,
	},
	[FIGHT_CLASS_DOUBLES_SETUP_ATTACKER - FIGHT_CLASS_DOUBLES_ALL_OUT_ATTACKER] =
	{
		[12] = 19,
		[11] = 16,
		[10] = 15,
		[ 9] = 13,
		[ 8] = 12,
		[ 7] = 11,
		[ 6] = 11,
		[ 5] = 9,
		[ 4] = 4,
		[ 3] = 4,
		[ 2] = 4,
		[ 1] = 4,
	},
	[FIGHT_CLASS_DOUBLES_TRICK_ROOM_ATTACKER - FIGHT_CLASS_DOUBLES_ALL_OUT_ATTACKER] =
	{
		[12] = 18,
		[11] = 16,
		[10] = 15,
		[ 9] = 14,
		[ 8] = 11,
		[ 7] = 10,
		[ 6] = 10,
		[ 5] = 9,
		[ 4] = 3,
		[ 3] = 3,
		[ 2] = 3,
		[ 1] = 3,
	},
	[FIGHT_CLASS_DOUBLES_TRICK_ROOM_SETUP - FIGHT_CLASS_DOUBLES_ALL_OUT_ATTACKER] =
	{
		[12] = 18,
		[11] = 6,
		[10] = 5,
		[ 9] = 4,
		[ 8] = 3,
		[ 7] = 2,
		[ 6] = 2,
		[ 5] = 1,
		[ 4] = 1,
		[ 3] = 1,
		[ 2] = 1,
		[ 1] = 1,
	},
	[FIGHT_CLASS_DOUBLES_UTILITY - FIGHT_CLASS_DOUBLES_ALL_OUT_ATTACKER] =
	{
		[12] = 18,
		[11] = 9,
		[10] = 8,
		[ 9] = 3,
		[ 8] = 2,
		[ 7] = 1,
		[ 6] = 1,
		[ 5] = 1,
		[ 4] = 1,
		[ 3] = 1,
		[ 2] = 1,
		[ 1] = 1,
	},
	[FIGHT_CLASS_DOUBLES_PHAZING - FIGHT_CLASS_DOUBLES_ALL_OUT_ATTACKER] =
	{
		[12] = 18,
		[11] = 8,
		[10] = 7,
		[ 9] = 6,
		[ 8] = 5,
		[ 7] = 4,
		[ 6] = 4,
		[ 5] = 4,
		[ 4] = 4,
		[ 3] = 4,
		[ 2] = 4,
		[ 1] = 4,
	},
	[FIGHT_CLASS_DOUBLES_TEAM_SUPPORT - FIGHT_CLASS_DOUBLES_ALL_OUT_ATTACKER] =
	{
		[12] = 16,
		[11] = 9,
		[10] = 7,
		[ 9] = 2,
		[ 8] = 2,
		[ 7] = 2,
		[ 6] = 2,
		[ 5] = 2,
		[ 4] = 2,
		[ 3] = 2,
		[ 2] = 2,
		[ 1] = 2,
	},
};

//Doubles is now defined as being a non 1v1 Double Battle
#undef IS_DOUBLE_BATTLE
#define IS_DOUBLE_BATTLE (gBattleTypeFlags & BATTLE_TYPE_DOUBLE && ((BATTLER_ALIVE(FOE(bankAtk)) && BATTLER_ALIVE(PARTNER(FOE(bankAtk)))) || BATTLER_ALIVE(PARTNER(bankAtk))))

//This file's functions:
static bool8 BankHoldingUsefulItemToProtectFor(u8 bank);
static bool8 BankHasAbilityUsefulToProtectFor(u8 bankAtk, u8 bankDef);
static bool8 ShouldTryToSetUpStat(u8 bankAtk, u8 bankDef, u16 move, u8 stat, u8 statLimit);
static bool8 ShouldUseSubstitute(u8 bankAtk, u8 bankDef);

bool8 IsClassSweeper(u8 class)
{
	return class == FIGHT_CLASS_SWEEPER_KILL
		|| class == FIGHT_CLASS_SWEEPER_SETUP_STATS
		|| class == FIGHT_CLASS_SWEEPER_SETUP_STATUS;
}

bool8 IsClassKillerSweeper(u8 class)
{
	return class == FIGHT_CLASS_SWEEPER_KILL;
}

bool8 IsClassSetupSweeper(u8 class)
{
	return class == FIGHT_CLASS_SWEEPER_SETUP_STATS;
}

bool8 IsClassStatusSweeper(u8 class)
{
	return class == FIGHT_CLASS_SWEEPER_SETUP_STATUS;
}

bool8 IsClassStall(u8 class)
{
	return class == FIGHT_CLASS_STALL;
}

bool8 IsClassTeamSupport(u8 class)
{
	return class == FIGHT_CLASS_TEAM_SUPPORT_BATON_PASS
		|| class == FIGHT_CLASS_TEAM_SUPPORT_CLERIC
		|| class == FIGHT_CLASS_TEAM_SUPPORT_SCREENS
		|| class == FIGHT_CLASS_TEAM_SUPPORT_PHAZING;
}

bool8 IsClassBatonPass(u8 class)
{
	return class == FIGHT_CLASS_TEAM_SUPPORT_BATON_PASS;
}

bool8 IsClassScreener(u8 class)
{
	return class == FIGHT_CLASS_TEAM_SUPPORT_SCREENS;
}

bool8 IsClassCleric(u8 class)
{
	return class == FIGHT_CLASS_TEAM_SUPPORT_CLERIC;
}

bool8 IsClassPhazer(u8 class)
{
	return class == FIGHT_CLASS_TEAM_SUPPORT_PHAZING
	    || class == FIGHT_CLASS_DOUBLES_PHAZING;
}

bool8 IsClassEntryHazards(u8 class)
{
	return class == FIGHT_CLASS_ENTRY_HAZARDS;
}

bool8 IsClassDoublesAllOutAttacker(u8 class)
{
	return class == FIGHT_CLASS_DOUBLES_ALL_OUT_ATTACKER;
}

bool8 IsClassDoublesSetupAttacker(u8 class)
{
	return class == FIGHT_CLASS_DOUBLES_SETUP_ATTACKER;
}

bool8 IsClassDoublesTrickRoomSetup(u8 class)
{
	return class == FIGHT_CLASS_DOUBLES_TRICK_ROOM_SETUP;
}

bool8 IsClassDoublesTrickRoomer(u8 class)
{
	return class == FIGHT_CLASS_DOUBLES_TRICK_ROOM_ATTACKER
			     || FIGHT_CLASS_DOUBLES_TRICK_ROOM_SETUP;
}

bool8 IsClassDoublesUtility(u8 class)
{
	return class == FIGHT_CLASS_DOUBLES_UTILITY;
}

bool8 IsClassDoublesTeamSupport(u8 class)
{
	return class == FIGHT_CLASS_DOUBLES_TEAM_SUPPORT;
}

bool8 IsClassDoublesSpecific(u8 class)
{
	return class == FIGHT_CLASS_DOUBLES_ALL_OUT_ATTACKER
	    || class == FIGHT_CLASS_DOUBLES_SETUP_ATTACKER
	    || class == FIGHT_CLASS_DOUBLES_TRICK_ROOM_ATTACKER
	    || class == FIGHT_CLASS_DOUBLES_TRICK_ROOM_SETUP
	    || class == FIGHT_CLASS_DOUBLES_UTILITY
	    || class == FIGHT_CLASS_DOUBLES_PHAZING
	    || class == FIGHT_CLASS_DOUBLES_TEAM_SUPPORT;
}

bool8 IsClassDoublesAttacker(u8 class)
{
	return class == FIGHT_CLASS_DOUBLES_TRICK_ROOM_ATTACKER
			     || FIGHT_CLASS_DOUBLES_SETUP_ATTACKER
				 || FIGHT_CLASS_DOUBLES_TRICK_ROOM_ATTACKER;
}

bool8 IsClassDamager(u8 class)
{
	return IsClassSweeper(class) ||  IsClassDoublesAttacker(class);
}

u8 GetBankFightingStyle(u8 bank)
{
	if (gNewBS->ai.fightingStyle[bank] == 0xFF)
		gNewBS->ai.fightingStyle[bank] = PredictBankFightingStyle(bank);

	return gNewBS->ai.fightingStyle[bank];
}

u8 PredictBankFightingStyle(u8 bank)
{
	u8 ability = ABILITY(bank);
	u8 itemEffect = ITEM_EFFECT(bank);

	if (IsDynamaxed(bank))
	{
		u16 moves[MAX_MON_MOVES] = {0};
		for (u8 i = 0; i < MAX_MON_MOVES; ++i)
			moves[i] = GetMaxMove(bank, i);

		return PredictFightingStyle(moves, ability, itemEffect, bank);
	}

	return PredictFightingStyle(gBattleMons[bank].moves, ability, itemEffect, bank);
}

u8 PredictFightingStyle(const u16* const moves, const u8 ability, const u8 itemEffect, const u8 bank)
{
	int i;
	u8 class = FIGHT_CLASS_NONE;
	u8 attackMoveNum = 0;
	u8 entryHazardNum = 0;
	u8 reflectionNum = 0;
	u8 statusMoveNum = 0;
	bool8 boostingMove = FALSE;
	bool8 healingMove = FALSE;
	bool8 auroraVeil = FALSE;

	bool8 isSingleBattle;
	if (bank == 0xFF)
		isSingleBattle = (gBattleTypeFlags & BATTLE_TYPE_DOUBLE) == 0;
	else
		isSingleBattle = !(gBattleTypeFlags & BATTLE_TYPE_DOUBLE && ((BATTLER_ALIVE(FOE(bank)) && BATTLER_ALIVE(PARTNER(FOE(bank)))) || BATTLER_ALIVE(PARTNER(bank))));

	if (isSingleBattle) //AI Single Battle
	{
		for (i = 0; i < MAX_MON_MOVES; ++i)
		{
			u16 move = moves[i];
			u8 moveEffect = gBattleMoves[move].effect;

			if (move == MOVE_NONE)
				continue;

			if (move == MOVE_BATONPASS)
			{
				if (bank != 0xFF)
				{
					if (ViableMonCountFromBank(bank) > 1)
						class = FIGHT_CLASS_TEAM_SUPPORT_BATON_PASS;
					else
						goto MOVE_EFFECT_SWITCH;
				}
				else
					class = FIGHT_CLASS_TEAM_SUPPORT_BATON_PASS;
			}
			else
			{
			MOVE_EFFECT_SWITCH:
				switch (moveEffect) {
					case EFFECT_ROAR:
					case EFFECT_HAZE:
					case EFFECT_REMOVE_TARGET_STAT_CHANGES:
						class = FIGHT_CLASS_TEAM_SUPPORT_PHAZING;
						break;

					case EFFECT_WISH:
					case EFFECT_HEAL_BELL:
						class = FIGHT_CLASS_TEAM_SUPPORT_CLERIC;
						break;

					case EFFECT_REFLECT:
						switch (move) {
							case MOVE_AURORAVEIL:
								if (!gNewBS->AuroraVeilTimers[SIDE(bank)]) //Don't check Hail b/c being a Screener helps set up Hail
									auroraVeil = TRUE; //Same as having two screen moves
								break;
							default:
								if (!(gSideStatuses[SIDE(bank)] & SIDE_STATUS_REFLECT))
									++reflectionNum;
								break;
						}
						break;

					case EFFECT_LIGHT_SCREEN:
						if (!(gSideStatuses[SIDE(bank)] & SIDE_STATUS_LIGHTSCREEN))
							++reflectionNum;
						break;

					case EFFECT_TRAP:
					case EFFECT_MEAN_LOOK:
						class = FIGHT_CLASS_STALL;
						break;

					default:
						if (ability == ABILITY_GORILLATACTICS || itemEffect == ITEM_EFFECT_CHOICE_BAND || itemEffect == ITEM_EFFECT_ASSAULT_VEST)
							class = FIGHT_CLASS_SWEEPER_KILL;
						else if (moveEffect == EFFECT_RESTORE_HP //Not placed above because checked in this order
							  || moveEffect == EFFECT_MORNING_SUN
							  || moveEffect == EFFECT_SWALLOW)
						{
							healingMove = TRUE;
						}
				}
			}

			if (class != FIGHT_CLASS_NONE) break; //Leave loop because we found a class

			if (SPLIT(move) != SPLIT_STATUS)
				++attackMoveNum;

			switch (moveEffect) {
				case EFFECT_ATTACK_UP:
				case EFFECT_DEFENSE_UP:
				case EFFECT_SPEED_UP:
				case EFFECT_SPECIAL_ATTACK_UP:
				case EFFECT_SPECIAL_DEFENSE_UP:
				case EFFECT_ACCURACY_UP:
				case EFFECT_EVASION_UP:
				case EFFECT_ATTACK_UP_2:
				case EFFECT_DEFENSE_UP_2:
				case EFFECT_SPEED_UP_2:
				case EFFECT_SPECIAL_ATTACK_UP_2:
				case EFFECT_SPECIAL_DEFENSE_UP_2:
				case EFFECT_ACCURACY_UP_2:
				case EFFECT_EVASION_UP_2:
				case EFFECT_COSMIC_POWER:
				case EFFECT_DRAGON_DANCE:
				case EFFECT_CALM_MIND:
				case EFFECT_BULK_UP:
					boostingMove = TRUE;
					break;

				case EFFECT_SPIKES:
					//Consider hazards already set up
					switch (move) {
						case MOVE_STEALTHROCK:
							if (gSideTimers[SIDE(FOE(bank))].srAmount == 0)
								++entryHazardNum;
							break;
						case MOVE_TOXICSPIKES:
							if (gSideTimers[SIDE(FOE(bank))].tspikesAmount < 2)
								++entryHazardNum;
							break;
						case MOVE_STICKYWEB:
							if (!gSideTimers[SIDE(FOE(bank))].stickyWeb)
								++entryHazardNum;
							break;
						default: //Spikes
							if (gSideTimers[SIDE(FOE(bank))].spikesAmount < 3)
								++entryHazardNum;
							break;
					}
					break;

				default:
					if (SPLIT(move) == SPLIT_STATUS)
						++statusMoveNum;
			}
		}

		if (class == FIGHT_CLASS_NONE)
		{
			if (reflectionNum >= 2 || auroraVeil)
				class = FIGHT_CLASS_TEAM_SUPPORT_SCREENS;
			else if (entryHazardNum >= 1)
				class = FIGHT_CLASS_ENTRY_HAZARDS;
			else if (attackMoveNum >= 3)
			{
				if (boostingMove)
					class = FIGHT_CLASS_SWEEPER_SETUP_STATS;
				else if (statusMoveNum > 0)
					class = FIGHT_CLASS_SWEEPER_SETUP_STATUS;
				else
					class = FIGHT_CLASS_SWEEPER_KILL;
			}
			else if (attackMoveNum >= 2 && (boostingMove || statusMoveNum > 0))
			{
				//A class should always be assigned here because of the conditions to enter this scope
				if (boostingMove)
					class = FIGHT_CLASS_SWEEPER_SETUP_STATS;
				else if (statusMoveNum > 0)
					class = FIGHT_CLASS_SWEEPER_SETUP_STATUS;
			}
			else if (healingMove)
				class = FIGHT_CLASS_STALL;
			else
				class = FIGHT_CLASS_STALL; //Returns STALL by default
		}
	}
	else //Doubles
	{
		bool8 hasTrickRoom = FALSE;
		bool8 hasPhazing = FALSE;
		bool8 hasRedirection = FALSE;
		//bool8 hasPartnerHelpingMoves = FALSE;
		//bool8 hasSpeedControl = FALSE;
		bool8 hasTeamSupport = FALSE;
		bool8 hasPivot = FALSE;
		bool8 hasPersonalProtect = FALSE;
		//bool8 hasTeamProtect = FALSE;
		bool8 hasFakeOut = FALSE;
		u8 numOffseniveBoostingMoves = 0;

		for (i = 0; i < MAX_MON_MOVES; ++i)
		{
			u16 move = moves[i];
			u8 moveEffect = gBattleMoves[move].effect;

			if (move == MOVE_NONE)
				continue;

			if (move == MOVE_TRICKROOM)
			{
				hasTrickRoom = TRUE;
			}
			else
			{
				switch (moveEffect) {
					case EFFECT_ATTACK_UP:
					case EFFECT_SPEED_UP:
					case EFFECT_SPECIAL_ATTACK_UP:
					case EFFECT_ACCURACY_UP:
					case EFFECT_ATTACK_UP_2:
					case EFFECT_SPEED_UP_2:
					case EFFECT_SPECIAL_ATTACK_UP_2:
					case EFFECT_ACCURACY_UP_2:
					case EFFECT_DRAGON_DANCE:
					case EFFECT_CALM_MIND:
					case EFFECT_BULK_UP:
						if (move == MOVE_ROTOTILLER)
						{
							if (bank == 0xFF || !IsOfType(bank, TYPE_GRASS))
								break; //Don't boost self
						}
						else if (move == MOVE_GEARUP)
						{
							if (ability != ABILITY_PLUS && ability != ABILITY_MINUS)
								break; //Don't boost self
						}

						++numOffseniveBoostingMoves;
						break;

					case EFFECT_ROAR:
					case EFFECT_HAZE:
					case EFFECT_REMOVE_TARGET_STAT_CHANGES:
						hasPhazing = TRUE;
						break;
/*
					case EFFECT_HELPING_HAND:
					case EFFECT_HEAL_TARGET:
					case EFFECT_INSTRUCT_AFTER_YOU_QUASH:
						hasPartnerHelpingMoves = TRUE;
						break;

					case EFFECT_DEFENSE_UP:
						if (move == MOVE_FLOWERSHIELD || move == MOVE_AROMATICMIST)
							hasPartnerHelpingMoves = TRUE;
						break;

					case EFFECT_SPECIAL_ATTACK_UP:
						if (move == MOVE_ROTOTILLER)
							hasPartnerHelpingMoves = TRUE;
						break;

					case EFFECT_EVASION_UP:
						if (move == MOVE_ACUPRESSURE)
							hasPartnerHelpingMoves = TRUE;
						break;

					case EFFECT_SPEED_DOWN_HIT:
						hasSpeedControl = TRUE;
						break;
*/
					case EFFECT_FOLLOW_ME:
						hasRedirection = TRUE;
						break;

					case EFFECT_REFLECT:
					case EFFECT_LIGHT_SCREEN:
					case EFFECT_HEAL_BELL:
					case EFFECT_SAFEGUARD:
					case EFFECT_MIST:
						hasTeamSupport = TRUE;
						break;

					case EFFECT_TEAM_EFFECTS:
						if (move != MOVE_MAGNETRISE)
							hasTeamSupport = TRUE;
						break;

					case EFFECT_BATON_PASS:
						if (move != MOVE_BATONPASS) //U-Turn & Volt Switch
							hasPivot = TRUE;
						break;

					case EFFECT_SUPERPOWER:
						if (itemEffect == ITEM_EFFECT_EJECT_PACK && move != MOVE_HYPERSPACEHOLE)
							hasPivot = TRUE;
						break;

					case EFFECT_FAKE_OUT:
						if (move == MOVE_FAKEOUT)
							hasFakeOut = TRUE;
						break;

					case EFFECT_PROTECT:
						if (move == MOVE_QUICKGUARD || move == MOVE_WIDEGUARD || move == MOVE_CRAFTYSHIELD)
						{
							//hasTeamProtect = TRUE;
						}
						else if (move == MOVE_MATBLOCK)
						{
							/*
							if (bank == 0xFF || gDisableStructs[bank].isFirstTurn)
								hasTeamProtect = TRUE;
							*/
						}
						else if (move != MOVE_ENDURE) //The rest of protects
							hasPersonalProtect = TRUE;
						break;
				}
			}

			if (class != FIGHT_CLASS_NONE) break; //Leave loop because we found a class

			if (SPLIT(move) != SPLIT_STATUS)
				++attackMoveNum;
		}

		if (class == FIGHT_CLASS_NONE)
		{
			if (hasTrickRoom)
			{
				if (attackMoveNum <= 1)
					class = FIGHT_CLASS_DOUBLES_TRICK_ROOM_SETUP;
				else
					class = FIGHT_CLASS_DOUBLES_TRICK_ROOM_ATTACKER;
			}
			else if (hasRedirection
			|| (ability == ABILITY_INTIMIDATE && hasFakeOut)
			|| (hasFakeOut && hasPivot))
			{
				class = FIGHT_CLASS_DOUBLES_UTILITY;
			}
			else if (hasPhazing)
			{
				class = FIGHT_CLASS_DOUBLES_PHAZING;
			}
			else if (attackMoveNum >= MAX_MON_MOVES || (attackMoveNum == 3 && hasPersonalProtect))
			{
				if (itemEffect == ITEM_EFFECT_ROOM_SERVICE || itemEffect == ITEM_EFFECT_IRON_BALL || itemEffect == ITEM_EFFECT_MACHO_BRACE)
					class = FIGHT_CLASS_DOUBLES_TRICK_ROOM_ATTACKER;
				else
					class = FIGHT_CLASS_DOUBLES_ALL_OUT_ATTACKER;
			}
			else if (attackMoveNum + numOffseniveBoostingMoves == 3 || (attackMoveNum + numOffseniveBoostingMoves == 2 && hasPersonalProtect))
			{
				if (itemEffect == ITEM_EFFECT_ROOM_SERVICE || itemEffect == ITEM_EFFECT_IRON_BALL || itemEffect == ITEM_EFFECT_MACHO_BRACE)
					class = FIGHT_CLASS_DOUBLES_TRICK_ROOM_ATTACKER;
				else
					class = FIGHT_CLASS_DOUBLES_SETUP_ATTACKER;
			}
			else if (hasTeamSupport)
			{
				class = FIGHT_CLASS_DOUBLES_TEAM_SUPPORT;
			}
			else
			{
				class = FIGHT_CLASS_DOUBLES_UTILITY; //The default class is Utility
			}
		}
	}

	return class;
}

bool8 ShouldTrap(u8 bankAtk, u8 bankDef, u16 move, u8 class)
{
	if (WillFaintFromSecondaryDamage(bankAtk))
		return FALSE;

	if (IsClassStall(class))
	{
		if (MoveWouldHitFirst(move, bankAtk, bankDef)) //Attacker goes first
		{
			if (!CanKnockOut(bankDef, bankAtk)) //Enemy can't kill attacker
				return TRUE;
		}
		else //Opponent Goes First
		{
			if (!CanKnockOut(bankDef, bankAtk)
			&&  !Can2HKO(bankDef, bankAtk))
				return TRUE;
		}
	}

	return FALSE;
}

u16 GetAmountToRecoverBy(u8 bankAtk, u8 bankDef, u16 move)
{
	if (move == 0xFFFF) //Item use
		return gBattleMons[bankAtk].maxHP - gBattleMons[bankAtk].hp; //Assume restores full health

	if (IsHealBlocked(bankAtk))
		return 0;

	u16 amountToRecover = 0;
	u16 curHp = GetBaseCurrentHP(bankAtk);
	u16 maxHp = GetBaseMaxHP(bankAtk);
	u8 itemEffect = ITEM_EFFECT(bankAtk);

	switch (gBattleMoves[move].effect) {
		case EFFECT_RESTORE_HP:
			if (move == MOVE_LIFEDEW)
				amountToRecover = MathMax(1, maxHp / 4);
			else
				amountToRecover = MathMax(1, maxHp / 2);
			break;

		case EFFECT_MORNING_SUN:
			switch (move) {
				case MOVE_SHOREUP:
					if (gBattleWeather & WEATHER_SANDSTORM_ANY && WEATHER_HAS_EFFECT)
						amountToRecover = maxHp;
					else
						amountToRecover = MathMax(1, maxHp / 2);
					break;

				default:
					if (gBattleWeather & WEATHER_SUN_ANY && WEATHER_HAS_EFFECT && itemEffect != ITEM_EFFECT_UTILITY_UMBRELLA)
						amountToRecover = maxHp;
					else if (gBattleWeather && WEATHER_HAS_EFFECT && itemEffect != ITEM_EFFECT_UTILITY_UMBRELLA) //Not sunny
						amountToRecover = MathMax(1, maxHp / 4);
					else
						amountToRecover = MathMax(1, maxHp / 2);
					break;
			}
			break;

		case EFFECT_SWALLOW:
			switch (gDisableStructs[bankAtk].stockpileCounter) {
				case 1:
					amountToRecover = MathMax(1, maxHp / 4);
					break;
				case 2:
					amountToRecover = MathMax(1, maxHp / 2);
					break;
				case 3:
					amountToRecover = maxHp;
					break;
			}
			break;

		case EFFECT_REST:
			amountToRecover = maxHp;
			break;

		case EFFECT_ABSORB:
		case EFFECT_DREAM_EATER: ;
			if (move == MOVE_STRENGTHSAP)
			{
				amountToRecover = CalcStrengthSapHealAmount(bankAtk, bankDef) * -1;
			}
			else
			{
				u16 predictedDmg = GetFinalAIMoveDamage(move, bankAtk, bankDef, 1, NULL);

				if (move == MOVE_OBLIVIONWING || move == MOVE_DRAININGKISS)
					amountToRecover = MathMax(1, (75 * predictedDmg) / 100);
				else
					amountToRecover = MathMax(1, predictedDmg / 2);

				if (ITEM_EFFECT(bankAtk) == ITEM_EFFECT_BIG_ROOT)
					amountToRecover = (130 * predictedDmg) / 100;
			}
			break;

		case EFFECT_PAIN_SPLIT: ;
			u16 finalHp = MathMax(1, (curHp + GetBaseCurrentHP(bankDef)) / 2);

			if (finalHp > curHp)
				amountToRecover = finalHp - curHp;
			break;

		case EFFECT_PROTECT: ;
			u8 ability = ABILITY(bankAtk);
			u8 itemEffect = ITEM_EFFECT(bankAtk);

			//maxHp = baseMaxHP;

			if (gStatuses3[bankAtk] & (STATUS3_ROOTED | STATUS3_AQUA_RING))
			{
				u16 hp = MathMax(1, maxHp / 16);
				if (itemEffect == ITEM_EFFECT_BIG_ROOT)
					amountToRecover += (130 * hp) / 100;
			}

			for (int i = 0; i < gBattlersCount; ++i)
			{
				if (gStatuses3[i] & STATUS3_LEECHSEED
				&&  ABILITY(i) != ABILITY_MAGICGUARD
				&& (gStatuses3[i] & STATUS3_LEECHSEED_BATTLER) == bankAtk)
				{
					amountToRecover += MathMax(1, GetBaseMaxHP(i) / 8);
				}
			}

			if (itemEffect == ITEM_EFFECT_LEFTOVERS
			|| (itemEffect == ITEM_EFFECT_BLACK_SLUDGE && IsOfType(bankAtk, TYPE_POISON)))
			{
				amountToRecover += MathMax(1, maxHp / 16);
			}

			if (gWishFutureKnock.wishCounter[bankAtk] > 0)
			{
				amountToRecover += MathMax(1, maxHp / 2);
			}

			if (gBattleWeather & WEATHER_RAIN_ANY && WEATHER_HAS_EFFECT && ITEM_EFFECT(bankAtk) != ITEM_EFFECT_UTILITY_UMBRELLA)
			{
				if (ability == ABILITY_RAINDISH)
					amountToRecover += MathMax(1, maxHp / 16);
				else if (ability == ABILITY_DRYSKIN)
					amountToRecover += MathMax(1, maxHp / 8);
			}

			if (gBattleWeather & WEATHER_HAIL_ANY && WEATHER_HAS_EFFECT)
			{
				if (ability == ABILITY_ICEBODY)
					amountToRecover += MathMax(1, maxHp / 16);
			}

			if (gBattleMons[bankAtk].status1 & STATUS1_PSN_ANY)
			{
				if (ability == ABILITY_POISONHEAL)
					amountToRecover += MathMax(1, maxHp / 8);
			}

			if (gTerrainType == GRASSY_TERRAIN && CheckGrounding(bankAtk) == GROUNDED)
			{
				amountToRecover += MathMax(1, maxHp / 16);
			}
	}

	return MathMin(amountToRecover, gBattleMons[bankAtk].maxHP - gBattleMons[bankAtk].hp);
}

bool8 ShouldRecover(u8 bankAtk, u8 bankDef, u16 move)
{
	u32 healAmount = GetAmountToRecoverBy(bankAtk, bankDef, move);

	//if (IS_SINGLE_BATTLE)
	//{
		if (move == 0xFFFF || MoveWouldHitFirst(move, bankAtk, bankDef)) //Using item or attacker goes first
		{
			if (CanKnockOut(bankDef, bankAtk)
			&& !CanKnockOutAfterHealing(bankDef, bankAtk, healAmount, 1))
				return TRUE;
		}
		else //Opponent Goes First
		{
			if (!CanKnockOut(bankDef, bankAtk)) //Enemy can't kill attacker
			{
				if (Can2HKO(bankDef, bankAtk)
				&& !CanKnockOutAfterHealing(bankDef, bankAtk, healAmount, 2))
					return TRUE;
			}
		}
	//}
	//else {}

	return FALSE;
}

static bool8 BankHoldingUsefulItemToProtectFor(u8 bank)
{
	u8 ability = ABILITY(bank);
	u8 itemEffect = ITEM_EFFECT(bank);

	if (itemEffect == ITEM_EFFECT_TOXIC_ORB
	&& CanBePoisoned(bank, bank, FALSE)
	&& (ability == ABILITY_POISONHEAL
	 || ability == ABILITY_TOXICBOOST
	 || ability == ABILITY_QUICKFEET
	 || ability == ABILITY_MAGICGUARD
	 || MoveInMoveset(MOVE_FACADE, bank)))
		return TRUE;

	if (itemEffect == ITEM_EFFECT_FLAME_ORB
	&& CanBeBurned(bank, FALSE)
	&& (ability == ABILITY_GUTS
	 || ability == ABILITY_FLAREBOOST
	 || ability == ABILITY_MAGICGUARD
	 || MoveInMoveset(MOVE_FACADE, bank)))
		return TRUE;

	return FALSE;
}

static bool8 BankHasAbilityUsefulToProtectFor(u8 bankAtk, u8 bankDef)
{
	u8 ability = ABILITY(bankAtk);

	switch (ability) {
		case ABILITY_MOODY:
			if (!StatsMaxed(bankAtk))
				return TRUE;
			break;

		case ABILITY_SPEEDBOOST:
			if (SpeedCalc(bankAtk) <= SpeedCalc(bankDef)
			&&  STAT_STAGE(bankAtk, STAT_STAGE_SPEED) < STAT_STAGE_MAX)
				return TRUE;
			break;
	}

	return FALSE;
}

enum ProtectQueries ShouldProtect(u8 bankAtk, u8 bankDef, u16 move)
{
	u16 predictedMove = IsValidMovePrediction(bankDef, bankAtk);
	u8 predictedMoveEffect = gBattleMoves[predictedMove].effect;
	u8 defAbility = ABILITY(bankDef);
	bool8 isAtkDynamaxed = IsDynamaxed(bankAtk);

	if (WillFaintFromSecondaryDamage(bankAtk)
	&&  defAbility != ABILITY_MOXIE
	&&  defAbility != ABILITY_BEASTBOOST)
		return FALSE; //Don't protect if you're going to faint after protecting and foe can't get boosts from your KO

	if (IsBankIncapacitated(bankDef))
		return FALSE; //Don't Protect against an opponent that isn't going to do anything

	if ((!isAtkDynamaxed && BankHoldingUsefulItemToProtectFor(bankAtk))
	||  (!isAtkDynamaxed && BankHasAbilityUsefulToProtectFor(bankAtk, bankDef))
	||  (IsDynamaxed(bankDef) && (!IsRaidBattle() || bankDef != BANK_RAID_BOSS) && !IsDynamaxed(bankAtk) && !ShouldAIDynamax(bankAtk, bankDef) && SPLIT(predictedMove) != SPLIT_STATUS) //Foe is going to attack with a Max Move and ai won't be dynamaxed
	||  predictedMoveEffect == EFFECT_EXPLOSION
	|| (predictedMoveEffect == EFFECT_SEMI_INVULNERABLE && BATTLER_SEMI_INVULNERABLE(bankDef) //Foe coming down
	   && gBattleMoves[predictedMove].flags & FLAG_PROTECT_AFFECTED))
	{
		return USE_PROTECT;
	}

	if (IS_SINGLE_BATTLE)
	{
		u32 healAmount = GetAmountToRecoverBy(bankAtk, bankDef, move);

		if (CanKnockOut(bankDef, bankAtk)) //Foe can kill
		{
			if (!isAtkDynamaxed && !CanKnockOutAfterHealing(bankDef, bankAtk, healAmount, 1))
				return USE_PROTECT;
			else if (!isAtkDynamaxed && IsTakingSecondaryDamage(bankDef))
				return USE_PROTECT;
			else
				return USE_STATUS_THEN_PROTECT;
		}
		else if (Can2HKO(bankDef, bankAtk))
		{
			if (!isAtkDynamaxed && !CanKnockOutAfterHealing(bankDef, bankAtk, healAmount, 2))
				return USE_PROTECT;
			else if (!isAtkDynamaxed && IsTakingSecondaryDamage(bankDef))
				return USE_PROTECT;
			else
				return USE_STATUS_THEN_PROTECT;
		}
	}
	else //Double Battle
	{
		u8 partner = PARTNER(bankAtk);

		if (BATTLER_ALIVE(partner))
		{
			u16 partnerMove = GetAIChosenMove(partner, bankDef);

			if (ABILITY(bankAtk) != ABILITY_TELEPATHY)
			{

				if (partnerMove != MOVE_NONE
				&& !isAtkDynamaxed
				&&  gBattleMoves[partnerMove].target & MOVE_TARGET_ALL
				&&  ABILITY(bankAtk) != ABILITY_TELEPATHY
				&& !(AI_SpecialTypeCalc(partnerMove, partner, bankAtk) & MOVE_RESULT_NO_EFFECT))
				{
					return PROTECT_FROM_ALLIES; //Protect if partner is going to use a move that damages the whole field
				}
			}

			if (!isAtkDynamaxed && GetDoubleKillingScore(partnerMove, partner, bankDef) >= BEST_DOUBLES_KO_SCORE - 1)
			{
				return PROTECT_FROM_FOES; //Partner has this covered
			}
		}
	}

	return DONT_PROTECT;
}

bool8 ShouldPhaze(u8 bankAtk, u8 bankDef, u16 move, u8 class)
{
	if (IsDynamaxed(bankDef))
		return FALSE; //These Pokemon can't be forced out.

	if (IsClassPhazer(class))
	{
		if (IS_SINGLE_BATTLE)
		{
			if (MoveWouldHitFirst(move, bankAtk, bankDef)) //Attacker goes first
			{
				if (CanKnockOut(bankDef, bankAtk)) //Foe can kill
				{
					if (AnyUsefulStatIsRaised(bankDef))
						return TRUE;
				}
				else
					goto ROAR_CHECK;
			}
			else //Opponent Goes First
			{
				if (Can2HKO(bankDef, bankAtk))
				{
					if (AnyUsefulStatIsRaised(bankDef))
						return TRUE;
				}
				else
				{
				ROAR_CHECK:
					switch (gBattleMoves[move].effect)
					{
						case EFFECT_ROAR:
							if (gSideTimers[SIDE(bankDef)].srAmount > 0 //Has some hurtful hazard
							||  gSideTimers[SIDE(bankDef)].tspikesAmount >= 1
							||  gSideTimers[SIDE(bankDef)].spikesAmount >= 1
							||  gSideTimers[SIDE(bankDef)].steelsurge > 0
							||  AnyUsefulStatIsRaised(bankDef))
								return TRUE;
							break;
						default:
							if (AnyUsefulStatIsRaised(bankDef))
								return TRUE;
					}
				}
			}
		}
		else //Double Battle
		{
			if (!CanKnockOut(bankAtk, bankDef)) //KO if you can
			{
				switch (gBattleMoves[move].effect)
				{
					case EFFECT_ROAR:
						if (gSideTimers[SIDE(bankDef)].srAmount > 0 //Has some hurtful hazard
						||  gSideTimers[SIDE(bankDef)].tspikesAmount >= 1
						||  gSideTimers[SIDE(bankDef)].spikesAmount >= 1
						||  AnyUsefulStatIsRaised(bankDef))
							return TRUE;
						break;
					case EFFECT_HAZE:
						if ((BATTLER_ALIVE(PARTNER(bankAtk)) && AnyUsefulStatIsRaised(PARTNER(bankAtk))))
							return FALSE; //Don't Haze if partner will lose benefits
						break;
					default:
						if (AnyUsefulStatIsRaised(bankDef))
							return TRUE;
				}
			}
		}
	}

	return FALSE;
}

bool8 ShouldUseWishAromatherapy(u8 bankAtk, u8 bankDef, u16 move, u8 class)
{
	int i;
	u8 firstId, lastId;
	struct Pokemon* party;
	bool8 hasStatus = FALSE;
	bool8 needHealing = FALSE;

	party = LoadPartyRange(bankAtk, &firstId, &lastId);

	if (ViableMonCountFromBank(bankAtk) <= 1
	&& (CanKnockOut(bankDef, bankAtk) || WillFaintFromSecondaryDamage(bankAtk)))
		return FALSE; //Don't heal if last mon and will faint after getting KOd

	for (i = 0; i < PARTY_SIZE; ++i)
	{
		u16 curHp = GetMonData(&party[i], MON_DATA_HP, NULL);
		u16 maxHp = GetMonData(&party[i], MON_DATA_MAX_HP, NULL);

		if (!GetMonData(&party[i], MON_DATA_IS_EGG, NULL) && curHp > 0)
		{
			if ((curHp * 100) / maxHp < 65 //Less than 65% health remaining
			&&  i >= firstId && i < lastId) //Can only switch to mon on your team
			{
				needHealing = TRUE;
			}

			if (GetMonData(&party[i], MON_DATA_STATUS, NULL) != STATUS1_NONE)
			{
				if (move != MOVE_HEALBELL || GetMonAbility(&party[i]) != ABILITY_SOUNDPROOF)
					hasStatus = TRUE;
			}
		}
	}

	if (IS_SINGLE_BATTLE && IsClassCleric(class))
	{
		switch (gBattleMoves[move].effect) {
			case EFFECT_WISH:
				if (needHealing)
					return TRUE;
				break;

			case EFFECT_HEAL_BELL:
				if (hasStatus)
					return TRUE;
		}
	}
	else if (IS_DOUBLE_BATTLE)
	{
		switch (gBattleMoves[move].effect) {
			case EFFECT_WISH:
				return ShouldRecover(bankAtk, bankDef, move); //Switch recovery isn't good idea in doubles

			case EFFECT_HEAL_BELL:
				if (hasStatus)
					return TRUE;
		}
	}

	return FALSE;
}

bool8 MoveSplitOnTeam(u8 bank, u8 split)
{
	u8 firstId, lastId;
	struct Pokemon* party = LoadPartyRange(bank, &firstId, &lastId);

	for (int i = 0; i < PARTY_SIZE; ++i)
	{
		for (int j = 0; j < MAX_MON_MOVES; ++j)
		{
			bool8 alive = GetMonData(&party[i], MON_DATA_HP, NULL) != 0;
			u16 move = GetMonData(&party[i], MON_DATA_MOVE1 + j, NULL);
			u16 pp = GetMonData(&party[i], MON_DATA_PP1 + j, NULL);

			if (alive && pp > 0 && move != MOVE_NONE)
			{
				if (CalcMoveSplitFromParty(&party[i], move) == split)
					return TRUE;
			}
		}
	}

	return FALSE;
}

bool8 ShouldSetUpScreens(u8 bankAtk, u8 bankDef, u16 move)
{
	if (!gNewBS->AuroraVeilTimers[SIDE(bankAtk)])
	{
		if (move == MOVE_AURORAVEIL
		&& gBattleWeather & WEATHER_HAIL_ANY
		&& !((gSideStatuses[bankAtk] & (SIDE_STATUS_REFLECT | SIDE_STATUS_LIGHTSCREEN)) == (SIDE_STATUS_REFLECT | SIDE_STATUS_LIGHTSCREEN)))
			return TRUE;
		else
		{
			bool8 defPhysicalMoveInMoveset = MoveSplitInMoveset(bankDef, SPLIT_PHYSICAL);
			bool8 defSpecialMoveInMoveset = MoveSplitInMoveset(bankDef, SPLIT_SPECIAL);

			switch (gBattleMoves[move].effect) {
				case EFFECT_REFLECT:
					if (MoveSplitOnTeam(bankDef, SPLIT_PHYSICAL))
					{
						if (!defPhysicalMoveInMoveset && !defSpecialMoveInMoveset)
							return TRUE; //Target has no attacking moves so no point in doing Light Screen check

						if (defPhysicalMoveInMoveset
						|| !MoveInMoveset(MOVE_LIGHTSCREEN, bankAtk) || !defSpecialMoveInMoveset)
							return TRUE;
					}
					break;

				case EFFECT_LIGHT_SCREEN:
					if (MoveSplitOnTeam(bankDef, SPLIT_SPECIAL))
					{
						if (!defPhysicalMoveInMoveset && !defSpecialMoveInMoveset)
							return TRUE; //Target has no attacking moves so no point in doing Light Screen check

						if (defSpecialMoveInMoveset
						|| !MoveInMoveset(MOVE_REFLECT, bankAtk) || !defPhysicalMoveInMoveset)
							return TRUE;
					}
					break;
			}
		}
	}

	return FALSE;
}

bool8 ShouldUseFakeOut(u8 bankAtk, u8 bankDef)
{
	if (gDisableStructs[bankAtk].isFirstTurn
	&& IsUsefulToFlinchTarget(bankDef)
	&& ABILITY(bankDef) != ABILITY_INNERFOCUS
	&& !MoveBlockedBySubstitute(MOVE_FAKEOUT, bankAtk, bankDef))
	{
		if (IS_DOUBLE_BATTLE)
		{
			if (!CanMovePredictionProtectAgainstMove(bankDef, bankAtk, MOVE_FAKEOUT))
			{
				if (ITEM_EFFECT(bankAtk) == ITEM_EFFECT_CHOICE_BAND  //Don't lock the attacker into Fake Out
				&& ViableMonCountFromBank(bankAtk) <= 2) 	 		 //if they can't switch out afterwards.
				{
					if (ViableMonCountFromBank(bankDef) == 1
					&&  MoveKnocksOutXHits(MOVE_FAKEOUT, bankAtk, bankDef, 1))
						return TRUE; //If the opponent also only has one Pokemon left, then kill it to win the battle
				}
				else
					return TRUE;
			}
		}
		else //Single Battle
		{
			if (ITEM_EFFECT(bankAtk) == ITEM_EFFECT_CHOICE_BAND  //Don't lock the attacker into Fake Out
			&& ViableMonCountFromBank(bankAtk) <= 1) 			 //if they can't switch out afterwards.
			{
				if (ViableMonCountFromBank(bankDef) == 1
				&&  MoveKnocksOutXHits(MOVE_FAKEOUT, bankAtk, bankDef, 1))
					return TRUE; //If the opponent also only has one Pokemon left, then kill it to win the battle
			}
			else
				return TRUE;
		}
	}

	return FALSE;
}

bool8 ShouldPivot(u8 bankAtk, u8 bankDef, u16 move, u8 class)
{
	bool8 hasUsefulStatBoost = (AnyUsefulOffseniveStatIsRaised(bankAtk) && IsClassDamager(class))
							|| STAT_STAGE(bankDef, STAT_STAGE_EVASION) >= 6 + 4; //Significant boost in evasion for any class

	u8 backupBattler = gActiveBattler;
	gActiveBattler = bankAtk;
	u8 switchScore = GetMostSuitableMonToSwitchIntoScore();
	gActiveBattler = backupBattler;

	u8 defAbility = ABILITY(bankDef);

	if (WillTakeSignificantDamageFromEntryHazards(bankAtk, 4)) //Don't switch out if you'll do a quarter or more damage to yourself on switch in
		return DONT_PIVOT;

	if (IS_SINGLE_BATTLE)
	{
		if (!BankHasMonToSwitchTo(bankAtk))
			return CAN_TRY_PIVOT; //Can't switch

		if (IsPredictedToSwitch(bankDef, bankAtk) && !hasUsefulStatBoost)
			return PIVOT; //Try pivoting so you can switch to a better matchup to counter your new opponent

		if (MoveWouldHitFirst(move, bankAtk, bankDef)) //Attacker goes first
		{
			if (!CanKnockOutWithoutMove(move, bankAtk, bankDef, TRUE)) //Can't KO foe otherwise
			{
				if (CanKnockOut(bankAtk, bankDef))
				{
					//If this clears, it's because this move is the only KO move. As such,
					//it'll be trated like one in the best damaging move calc later.
					return CAN_TRY_PIVOT;
				}
				else if (IsClassDamager(class) && Can2HKO(bankAtk, bankDef)) //Two more hits KOs the foe
				{
					if (CanKnockOut(bankDef, bankAtk)) //Won't get the chance to 2HKO
						return PIVOT;

					if (IsClassDamager(class)
					&& SPLIT(move) != SPLIT_STATUS //Damaging move
					&& (switchScore >= SWITCHING_INCREASE_RESIST_ALL_MOVES + SWITCHING_INCREASE_KO_FOE //Or remove hazards
					 || (BATTLER_MAX_HP(bankDef)
					    && (IsBankHoldingFocusSash(bankDef) || defAbility == ABILITY_STURDY || defAbility == ABILITY_MULTISCALE || defAbility == ABILITY_SHADOWSHIELD)))) //Pivot to break the sash/sturdy/multiscale
						return PIVOT;
				}
				else if (!hasUsefulStatBoost) //3HKO+ the foe
				{
					if (IsClassDamager(class)
					&& BATTLER_MAX_HP(bankDef)
					&& SPLIT(move) != SPLIT_STATUS //Damaging move
					&& (IsBankHoldingFocusSash(bankDef) || defAbility == ABILITY_STURDY || defAbility == ABILITY_MULTISCALE || defAbility == ABILITY_SHADOWSHIELD)) //Pivot to break the sash/sturdy/multiscale
						return PIVOT;

					if (IsClassDamager(class) && switchScore >= SWITCHING_INCREASE_WALLS_FOE)
						return PIVOT;

					if (gSideStatuses[bankAtk] & SIDE_STATUS_SPIKES && switchScore >= SWITCHING_INCREASE_CAN_REMOVE_HAZARDS)
							return PIVOT;

					if (WillFaintFromSecondaryDamage(bankAtk) && switchScore >= SWITCHING_INCREASE_WALLS_FOE)
						return PIVOT;

					if (IsClassDamager(class) && switchScore >= SWITCHING_INCREASE_HAS_SUPER_EFFECTIVE_MOVE)
					{
						bool8 physMoveInMoveset = PhysicalMoveInMoveset(bankAtk);
						bool8 specMoveInMoveset = SpecialMoveInMoveset(bankAtk);

						//Pivot if attacking stats are bad
						if (physMoveInMoveset && !specMoveInMoveset)
						{
							if (STAT_STAGE_ATK < 6)
								return PIVOT;
						}
						else if (!physMoveInMoveset && specMoveInMoveset)
						{
							if (STAT_STAGE_SPATK < 6)
								return PIVOT;
						}
						else if (physMoveInMoveset && specMoveInMoveset)
						{
							if (STAT_STAGE_ATK < 6 && STAT_STAGE_SPATK < 6)
								return PIVOT;
						}

						return CAN_TRY_PIVOT;
					}
				}
			}
		}
		else //Opponent Goes First
		{
			if (CanKnockOut(bankDef, bankAtk))
			{
				if (gBattleMoves[move].effect == EFFECT_TELEPORT)
					return DONT_PIVOT; //If you're going to faint because you'll go second, use a different move
				else
					return CAN_TRY_PIVOT; //You're probably going to faint anyways so if for some reason you don't, better switch
			}
			else if (Can2HKO(bankDef, bankAtk)) //Foe can 2HKO AI
			{
				if (CanKnockOut(bankAtk, bankDef))
				{
					if (!CanKnockOutWithoutMove(move, bankAtk, bankDef, TRUE))
						return CAN_TRY_PIVOT; //Use this move to KO if you must
				}
				else //Can't KO the foe
				{
					if (IsClassDamager(class) && switchScore >= SWITCHING_INCREASE_WALLS_FOE)
						return PIVOT;
				}
			}
			else //Foe can 3HKO+ AI
			{
				if (CanKnockOut(bankAtk, bankDef))
				{
					if (!CanKnockOutWithoutMove(move, bankAtk, bankDef, TRUE) //This is the only move that can KO
					&&  !hasUsefulStatBoost) //You're not wasting a valuable stat boost
					{
						return CAN_TRY_PIVOT;
					}
				}
				else if (Can2HKO(bankAtk, bankDef))
				{
					if (IsClassDamager(class)
					&& SPLIT(move) != SPLIT_STATUS //Damaging move
					&& (switchScore >= SWITCHING_INCREASE_RESIST_ALL_MOVES + SWITCHING_INCREASE_KO_FOE //remove hazards
					 || (IsBankHoldingFocusSash(bankDef) && BATTLER_MAX_HP(bankDef)))) //Pivot to break the sash
						return PIVOT;

					//Otherwise try to get foe hp down to where the CanKnockOut check is reached
				}
				else
				{
					if (IsClassDamager(class) && switchScore >= SWITCHING_INCREASE_KO_FOE)
						return PIVOT; //Only switch if way better matchup

					if (!hasUsefulStatBoost)
					{
						if (gSideStatuses[bankAtk] & SIDE_STATUS_SPIKES && switchScore >= SWITCHING_INCREASE_CAN_REMOVE_HAZARDS)
							return PIVOT;

						if (WillFaintFromSecondaryDamage(bankAtk) && switchScore >= SWITCHING_INCREASE_HAS_SUPER_EFFECTIVE_MOVE)
							return PIVOT;

						if (IsClassDamager(class) && switchScore >= SWITCHING_INCREASE_HAS_SUPER_EFFECTIVE_MOVE)
						{
							bool8 physMoveInMoveset = PhysicalMoveInMoveset(bankAtk);
							bool8 specMoveInMoveset = SpecialMoveInMoveset(bankAtk);

							//Pivot if attacking stats are bad
							if (physMoveInMoveset && !specMoveInMoveset)
							{
								if (STAT_STAGE_ATK < 6)
									return PIVOT;
							}
							else if (!physMoveInMoveset && specMoveInMoveset)
							{
								if (STAT_STAGE_SPATK < 6)
									return PIVOT;
							}
							else if (physMoveInMoveset && specMoveInMoveset)
							{
								if (STAT_STAGE_ATK < 6 && STAT_STAGE_SPATK < 6)
									return PIVOT;
							}
						}

						return CAN_TRY_PIVOT; //You're going to hit second so it can't hurt to Pivot. But do what does the most damage
					}
				}
			}
		}
	}

	return DONT_PIVOT;
}


u8 BankLikelyToUseMoveSplit(u8 bank, u8 class)
{
	if (PhysicalMoveInMoveset(bank))
	{
		if (IsClassSweeper(class))
		{
			if (gBattleMons[bank].attack > gBattleMons[bank].spAttack)
				return SPLIT_PHYSICAL;
		}
	}
	else if (SpecialMoveInMoveset(bank))
	{
		if (IsClassSweeper(class))
		{
			if (gBattleMons[bank].spAttack > gBattleMons[bank].attack)
				return SPLIT_SPECIAL;
		}
	}

	return SPLIT_STATUS;
}

void IncreaseStatusViability(s16* originalViability, u8 class, u8 boost, u8 bankAtk, u8 bankDef)
{
	s16 viability = *originalViability;

	switch (class) {
		case FIGHT_CLASS_SWEEPER_KILL:
			break;

		case FIGHT_CLASS_SWEEPER_SETUP_STATS:
			if (!Can2HKO(bankDef, bankAtk))
				INCREASE_VIABILITY(3);
			break;

		case FIGHT_CLASS_SWEEPER_SETUP_STATUS:
			INCREASE_VIABILITY(4 + boost);
			break;

		case FIGHT_CLASS_STALL:
			INCREASE_VIABILITY(4 + boost);
			break;

		case FIGHT_CLASS_TEAM_SUPPORT_BATON_PASS:
			if (boost >= 3)
				INCREASE_VIABILITY(1);
			break;

		case FIGHT_CLASS_TEAM_SUPPORT_CLERIC:
			INCREASE_VIABILITY(3 + boost);
			break;

		case FIGHT_CLASS_TEAM_SUPPORT_SCREENS:
			INCREASE_VIABILITY(2 + boost);
			break;

		case FIGHT_CLASS_TEAM_SUPPORT_PHAZING:
			INCREASE_VIABILITY(4 + boost);
			break;

		case FIGHT_CLASS_ENTRY_HAZARDS:
			INCREASE_VIABILITY(3);
			break;

		case FIGHT_CLASS_DOUBLES_ALL_OUT_ATTACKER:
			break;

		case FIGHT_CLASS_DOUBLES_SETUP_ATTACKER:
			INCREASE_VIABILITY(4 + boost);
			break;

		case FIGHT_CLASS_DOUBLES_TRICK_ROOM_ATTACKER:
			INCREASE_VIABILITY(4 + boost);
			break;

		case FIGHT_CLASS_DOUBLES_TRICK_ROOM_SETUP:
			INCREASE_VIABILITY(7 + boost);
			break;

		case FIGHT_CLASS_DOUBLES_UTILITY:
			INCREASE_VIABILITY(3 + boost);
			break;

		case FIGHT_CLASS_DOUBLES_PHAZING:
			INCREASE_VIABILITY(8 + boost);
			break;

		case FIGHT_CLASS_DOUBLES_TEAM_SUPPORT:
			INCREASE_VIABILITY(2 + boost);
			break;
	}

	*originalViability = MathMin(viability, 255);
}

static bool8 ShouldTryToSetUpStat(u8 bankAtk, u8 bankDef, u16 move, u8 stat, u8 statLimit)
{
	if (ABILITY(bankDef) == ABILITY_UNAWARE
	&& !MoveInMoveset(MOVE_STOREDPOWER, bankAtk)
	&& !MoveInMoveset(MOVE_POWERTRIP, bankAtk))
		return FALSE; //Don't set up if foe has Unaware

	if (WillFaintFromSecondaryDamage(bankAtk))
		return FALSE; //Don't set up if you're going to die

	if (IS_SINGLE_BATTLE)
	{
		if (MoveWouldHitFirst(move, bankAtk, bankDef)) //Attacker goes first
		{
			if (CanKnockOut(bankDef, bankAtk))
			{
				return FALSE; //Don't set up if enemy can KO you
			}
			else
			{
				if (BATTLER_SEMI_INVULNERABLE(bankDef))
					return TRUE; //Can't hit target anyways

				if (STAT_STAGE(bankAtk, stat) >= statLimit)
					return FALSE;
			}

			return TRUE;
		}
		else //Opponent Goes First
		{
			if (IsMovePredictionSemiInvulnerable(bankDef, bankAtk))
				return TRUE;

			if (stat == STAT_STAGE_SPEED && STAT_STAGE(bankAtk, stat) < statLimit)
				return TRUE; //Opponent goes first now, but maybe boosting speed will make you faster

			if (!Can2HKO(bankDef, bankAtk) && STAT_STAGE(bankAtk, stat) < statLimit)
				return TRUE;

			return FALSE;
		}
	}
	else //Double Battle
	{
		u8 foe1, foe2;
		foe1 = FOE(bankAtk);

		if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
			foe2 = PARTNER(FOE(bankAtk));
		else
			foe2 = foe1;

		//Don't set up if either foe on the field can KO
		if (BATTLER_ALIVE(foe1))
		{
			if (MoveWouldHitFirst(move, bankAtk, foe1)) //Attacker goes first
			{
				if (CanKnockOut(foe1, bankAtk) || STAT_STAGE(bankAtk, stat) >= statLimit)
					return FALSE;
			}
			else //Opponent Goes First
			{
				if (Can2HKO(foe1, bankAtk) || STAT_STAGE(bankAtk, stat) >= statLimit)
					return FALSE;
			}
		}

		if (BATTLER_ALIVE(foe2))
		{
			if (MoveWouldHitFirst(move, bankAtk, foe2)) //Attacker goes first
			{
				if (CanKnockOut(foe2, bankAtk) || STAT_STAGE(bankAtk, stat) >= statLimit)
					return FALSE;
			}
			else //Opponent Goes First
			{
				if (Can2HKO(foe2, bankAtk) || STAT_STAGE(bankAtk, stat) >= statLimit)
					return FALSE;
			}
		}

		return TRUE;
	}
}

void IncreaseStatViability(s16* originalViability, u8 class, u8 boost, u8 bankAtk, u8 bankDef, u16 move, u8 stat, u8 statLimit)
{
	s16 viability = *originalViability;

	switch (class) {
		case FIGHT_CLASS_SWEEPER_KILL:
			break;

		case FIGHT_CLASS_SWEEPER_SETUP_STATS:
			if (ShouldTryToSetUpStat(bankAtk, bankDef, move, stat, statLimit))
				INCREASE_VIABILITY(3 + boost);
			break;

		case FIGHT_CLASS_SWEEPER_SETUP_STATUS:
			if (ShouldTryToSetUpStat(bankAtk, bankDef, move, stat, statLimit))
				INCREASE_STATUS_VIABILITY(1); //Treat like a low-priority status move
			break;

		case FIGHT_CLASS_STALL:
			if (ShouldTryToSetUpStat(bankAtk, bankDef, move, stat, statLimit))
				INCREASE_STATUS_VIABILITY(1); //Treat like a low-priority status move
			break;

		case FIGHT_CLASS_TEAM_SUPPORT_BATON_PASS:
			switch (stat) {
				//Offsensive Stat Booster
				case STAT_STAGE_ATK:
				case STAT_STAGE_SPATK:
				case STAT_STAGE_SPEED:
				case STAT_STAGE_ACC:
					if (STAT_STAGE(bankAtk, stat) < 6 + 4)
						INCREASE_VIABILITY(5);
					break;

				case STAT_STAGE_DEF:
				case STAT_STAGE_SPDEF:
					if (STAT_STAGE(bankAtk, stat) < 6 + 4)
						INCREASE_VIABILITY(4);
					break;

				//Evasiveness Stat Booster
				case STAT_STAGE_EVASION:
					if (STAT_STAGE(bankAtk, stat) < STAT_STAGE_MAX)
						INCREASE_VIABILITY(6);
					break;
			}
			break;

		case FIGHT_CLASS_TEAM_SUPPORT_CLERIC:
			if (ShouldTryToSetUpStat(bankAtk, bankDef, move, stat, statLimit))
				INCREASE_STATUS_VIABILITY(1); //Treat like a low-priority status move
			break;

		case FIGHT_CLASS_TEAM_SUPPORT_SCREENS:
			if (ShouldTryToSetUpStat(bankAtk, bankDef, move, stat, statLimit))
				INCREASE_STATUS_VIABILITY(1); //Treat like a low-priority status move
			break;

		case FIGHT_CLASS_TEAM_SUPPORT_PHAZING:
			if (!MoveEffectInMoveset(EFFECT_HAZE, bankAtk) //Don't boost stats if can haze them away
			&&  ShouldTryToSetUpStat(bankAtk, bankDef, move, stat, statLimit))
				INCREASE_STATUS_VIABILITY(1); //Treat like a low-priority status move
			break;

		case FIGHT_CLASS_ENTRY_HAZARDS:
			if (ShouldTryToSetUpStat(bankAtk, bankDef, move, stat, statLimit))
				INCREASE_STATUS_VIABILITY(1); //Treat like a low-priority status move
			break;

		case FIGHT_CLASS_DOUBLES_ALL_OUT_ATTACKER:
			break;

		case FIGHT_CLASS_DOUBLES_SETUP_ATTACKER:
			switch (stat) {
				//Offsensive Stat Booster
				case STAT_STAGE_ATK:
				case STAT_STAGE_SPATK:
				case STAT_STAGE_SPEED:
				case STAT_STAGE_ACC:
				case STAT_STAGE_EVASION:
					if (STAT_STAGE(bankAtk, stat) < 6 + 1) //Max 1 Boost
						INCREASE_VIABILITY(17);
					break;
			}
			break;

		case FIGHT_CLASS_DOUBLES_TRICK_ROOM_ATTACKER:
			if (ShouldTryToSetUpStat(bankAtk, bankDef, move, stat, statLimit))
				INCREASE_STATUS_VIABILITY(1); //Treat like a low-priority status move
			break;

		case FIGHT_CLASS_DOUBLES_TRICK_ROOM_SETUP:
			if (ShouldTryToSetUpStat(bankAtk, bankDef, move, stat, statLimit))
				INCREASE_STATUS_VIABILITY(1); //Treat like a low-priority status move
			break;

		case FIGHT_CLASS_DOUBLES_UTILITY:
			if (ShouldTryToSetUpStat(bankAtk, bankDef, move, stat, statLimit))
				INCREASE_STATUS_VIABILITY(1); //Treat like a low-priority status move
			break;

		case FIGHT_CLASS_DOUBLES_PHAZING:
			if (ShouldTryToSetUpStat(bankAtk, bankDef, move, stat, statLimit))
				INCREASE_STATUS_VIABILITY(1); //Treat like a low-priority status move
			break;

		case FIGHT_CLASS_DOUBLES_TEAM_SUPPORT:
			if (ShouldTryToSetUpStat(bankAtk, bankDef, move, stat, statLimit))
				INCREASE_STATUS_VIABILITY(1); //Treat like a low-priority status move
			break;
	}

	*originalViability = MathMin(viability, 255);
}

static bool8 ShouldUseSubstitute(u8 bankAtk, u8 bankDef)
{
	u16 defPrediction = IsValidMovePrediction(bankDef, bankAtk);

	if (IsBankIncapacitated(bankDef))
		return TRUE;

	if (defPrediction != MOVE_NONE)
	{
		if (MoveWouldHitFirst(MOVE_SUBSTITUTE, bankAtk, bankDef)) //Attacker goes first
		{
			if (GetFinalAIMoveDamage(defPrediction, bankDef, bankAtk, 1, NULL) < MathMax(1, gBattleMons[bankAtk].maxHP / 4))
				return TRUE;
		}
		else //Substitute would go second
		{
			if (IsPredictedToSwitch(bankDef, bankAtk))
				return TRUE;

			s32 hp = gBattleMons[bankAtk].hp - GetFinalAIMoveDamage(defPrediction, bankDef, bankAtk, 1, NULL);
			if (hp > gBattleMons[bankAtk].maxHP / 4) //After being struck a substitute can still be made
				return TRUE;
		}
	}

	return FALSE;
}

void IncreaseSleepViability(s16* originalViability, u8 class, u8 bankAtk, u8 bankDef, u16 move)
{
	s16 viability = *originalViability;

	if (BadIdeaToPutToSleep(bankDef, bankAtk))
		return;

	bool8 spreadSleep = gBattleMoves[move].target & (MOVE_TARGET_BOTH | MOVE_TARGET_ALL)
						 && IS_DOUBLE_BATTLE
						 && BATTLER_ALIVE(PARTNER(bankDef))
						 && !MoveBlockedBySubstitute(move, bankAtk, PARTNER(bankDef))
						 && !BadIdeaToPutToSleep(PARTNER(bankDef), bankAtk);

	switch (class) {
		case FIGHT_CLASS_SWEEPER_KILL:
			break;

		case FIGHT_CLASS_SWEEPER_SETUP_STATS:
			if (AccuracyCalc(move, bankAtk, bankDef) >= 80) //Decent chance of hitting
				INCREASE_STATUS_VIABILITY(9);
			else
				INCREASE_STATUS_VIABILITY(3);
			break;

		case FIGHT_CLASS_SWEEPER_SETUP_STATUS:
			if (AccuracyCalc(move, bankAtk, bankDef) >= 80) //Decent chance of hitting
				INCREASE_STATUS_VIABILITY(8);
			else
				INCREASE_STATUS_VIABILITY(3);
			break;

		case FIGHT_CLASS_STALL:
			INCREASE_STATUS_VIABILITY(3);
			break;

		case FIGHT_CLASS_TEAM_SUPPORT_BATON_PASS:
			if (AccuracyCalc(move, bankAtk, bankDef) >= 80) //Decent chance of hitting
				INCREASE_STATUS_VIABILITY(9);
			else
				INCREASE_STATUS_VIABILITY(3);
			break;

		case FIGHT_CLASS_TEAM_SUPPORT_CLERIC:
			if (AccuracyCalc(move, bankAtk, bankDef) >= 80) //Decent chance of hitting
				INCREASE_STATUS_VIABILITY(7);
			else
				INCREASE_STATUS_VIABILITY(3);
			break;

		case FIGHT_CLASS_TEAM_SUPPORT_SCREENS:
			if (AccuracyCalc(move, bankAtk, bankDef) >= 80) //Decent chance of hitting
				INCREASE_STATUS_VIABILITY(8);
			else
				INCREASE_STATUS_VIABILITY(3);
			break;

		case FIGHT_CLASS_TEAM_SUPPORT_PHAZING:
			if (AccuracyCalc(move, bankAtk, bankDef) >= 80) //Decent chance of hitting
				INCREASE_STATUS_VIABILITY(9);
			else
				INCREASE_STATUS_VIABILITY(3);
			break;

		case FIGHT_CLASS_ENTRY_HAZARDS:
			if (AccuracyCalc(move, bankAtk, bankDef) >= 80) //Decent chance of hitting
				INCREASE_STATUS_VIABILITY(8);
			else
				INCREASE_STATUS_VIABILITY(3);
			break;

		case FIGHT_CLASS_DOUBLES_ALL_OUT_ATTACKER:
			INCREASE_STATUS_VIABILITY(3);
			break;

		case FIGHT_CLASS_DOUBLES_SETUP_ATTACKER:
			if (spreadSleep)
				INCREASE_VIABILITY(18);
			else
				INCREASE_STATUS_VIABILITY(3);
			break;

		case FIGHT_CLASS_DOUBLES_TRICK_ROOM_ATTACKER:
			if (spreadSleep)
				INCREASE_VIABILITY(16);
			else
				INCREASE_STATUS_VIABILITY(3);
			break;

		case FIGHT_CLASS_DOUBLES_TRICK_ROOM_SETUP:
			if (spreadSleep)
				INCREASE_VIABILITY(16);
			else
				INCREASE_STATUS_VIABILITY(3);
			break;

		case FIGHT_CLASS_DOUBLES_UTILITY:
			INCREASE_VIABILITY(17);
			break;

		case FIGHT_CLASS_DOUBLES_PHAZING:
			if (spreadSleep)
				INCREASE_VIABILITY(17);
			else
				INCREASE_STATUS_VIABILITY(3);
			break;

		case FIGHT_CLASS_DOUBLES_TEAM_SUPPORT:
			if (spreadSleep)
				INCREASE_VIABILITY(19);
			else
				INCREASE_STATUS_VIABILITY(3);
			break;
	}

	*originalViability = MathMin(viability, 255);
}

void IncreaseSubstituteViability(s16* originalViability, u8 class, u8 bankAtk, u8 bankDef)
{
	s16 viability = *originalViability;

	if (!IS_BEHIND_SUBSTITUTE(bankAtk))
	{
		switch (class) {
			case FIGHT_CLASS_SWEEPER_KILL:
				break;

			case FIGHT_CLASS_SWEEPER_SETUP_STATS:
				if (ShouldUseSubstitute(bankAtk, bankDef))
					INCREASE_VIABILITY(8);
				break;

			case FIGHT_CLASS_SWEEPER_SETUP_STATUS:
				if (ShouldUseSubstitute(bankAtk, bankDef))
					INCREASE_STATUS_VIABILITY(2);
				break;

			case FIGHT_CLASS_STALL:
				if (ShouldUseSubstitute(bankAtk, bankDef))
					INCREASE_STATUS_VIABILITY(2);
				break;

			case FIGHT_CLASS_TEAM_SUPPORT_BATON_PASS:
				if (ShouldUseSubstitute(bankAtk, bankDef))
					IncreaseViability(&viability, 4);
				break;

			case FIGHT_CLASS_TEAM_SUPPORT_CLERIC:
				if (ShouldUseSubstitute(bankAtk, bankDef))
					INCREASE_STATUS_VIABILITY(1);
				break;

			case FIGHT_CLASS_TEAM_SUPPORT_SCREENS:
				if (ShouldUseSubstitute(bankAtk, bankDef))
					INCREASE_STATUS_VIABILITY(1);
				break;

			case FIGHT_CLASS_TEAM_SUPPORT_PHAZING:
				if (ShouldUseSubstitute(bankAtk, bankDef))
					INCREASE_STATUS_VIABILITY(1);
				break;

			case FIGHT_CLASS_ENTRY_HAZARDS:
				if (ShouldUseSubstitute(bankAtk, bankDef))
					INCREASE_STATUS_VIABILITY(1);
				break;

			case FIGHT_CLASS_DOUBLES_ALL_OUT_ATTACKER:
				break;

			case FIGHT_CLASS_DOUBLES_SETUP_ATTACKER:
				if (ShouldUseSubstitute(bankAtk, bankDef))
					INCREASE_STATUS_VIABILITY(1); //Treat like a low-priority status move
				break;

			case FIGHT_CLASS_DOUBLES_TRICK_ROOM_ATTACKER:
				if (ShouldUseSubstitute(bankAtk, bankDef))
					INCREASE_STATUS_VIABILITY(1); //Treat like a low-priority status move
				break;

			case FIGHT_CLASS_DOUBLES_TRICK_ROOM_SETUP:
				if (ShouldUseSubstitute(bankAtk, bankDef))
					INCREASE_STATUS_VIABILITY(1); //Treat like a low-priority status move
				break;

			case FIGHT_CLASS_DOUBLES_UTILITY:
				if (ShouldUseSubstitute(bankAtk, bankDef))
					INCREASE_STATUS_VIABILITY(1); //Treat like a low-priority status move
				break;

			case FIGHT_CLASS_DOUBLES_PHAZING:
				if (ShouldUseSubstitute(bankAtk, bankDef))
					INCREASE_STATUS_VIABILITY(1); //Treat like a low-priority status move
				break;

			case FIGHT_CLASS_DOUBLES_TEAM_SUPPORT:
				if (ShouldUseSubstitute(bankAtk, bankDef))
					INCREASE_STATUS_VIABILITY(1); //Treat like a low-priority status move
				break;
		}
	}

	*originalViability = MathMin(viability, 255);
}

void IncreaseEntryHazardsViability(s16* originalViability, u8 class, u8 bankAtk, u8 bankDef, u16 move)
{
	s16 viability = *originalViability;

	switch (class) {
		case FIGHT_CLASS_SWEEPER_KILL:
			break;

		case FIGHT_CLASS_SWEEPER_SETUP_STATS:
			if (ViableMonCountFromBank(bankDef) == 2) //Only two mons left
				break; //Don't bother
			else
				INCREASE_STATUS_VIABILITY(2); //Treat like a middle-priority status move
			break;

		case FIGHT_CLASS_SWEEPER_SETUP_STATUS:
			if (ViableMonCountFromBank(bankDef) == 2) //Only two mons left
				break; //Don't bother
			else
				INCREASE_STATUS_VIABILITY(2); //Treat like a middle-priority status move
			break;

		case FIGHT_CLASS_STALL:
			if (ViableMonCountFromBank(bankDef) == 2) //Only two mons left
				INCREASE_STATUS_VIABILITY(1); //Treat like a low-priority status move
			else
				INCREASE_STATUS_VIABILITY(2); //Treat like a middle-priority status move
			break;

		case FIGHT_CLASS_TEAM_SUPPORT_BATON_PASS:
			if (gBattleMoves[move].effect == EFFECT_SPIKES) //Entry Hazards
			{
				if (MoveWouldHitFirst(move, bankAtk, bankDef))
				{
					if (!CanKnockOut(bankDef, bankAtk) && !WillFaintFromSecondaryDamage(bankAtk)) //Opponent can't kill before user can Baton Pass
						INCREASE_VIABILITY(4); //Right more important than Baton Pass
				}
				else
				{
					if (!Can2HKO(bankDef, bankAtk)) //Opponent can't kill before user can Baton Pass
						INCREASE_VIABILITY(4); //Right more important than Baton Pass
				}
			}
			else
				INCREASE_STATUS_VIABILITY(2); //Treat like a middle-priority status move
			break;

		case FIGHT_CLASS_TEAM_SUPPORT_CLERIC:
			if (ViableMonCountFromBank(bankDef) == 2) //Only two mons left
				INCREASE_STATUS_VIABILITY(1); //Treat like a low-priority status move
			else
				INCREASE_STATUS_VIABILITY(2); //Treat like a middle-priority status move
			break;

		case FIGHT_CLASS_TEAM_SUPPORT_SCREENS:
			if (ViableMonCountFromBank(bankDef) == 2) //Only two mons left
				INCREASE_STATUS_VIABILITY(1); //Treat like a low-priority status move
			else
				INCREASE_STATUS_VIABILITY(2); //Treat like a middle-priority status move
			break;

		case FIGHT_CLASS_TEAM_SUPPORT_PHAZING:
			if (ViableMonCountFromBank(bankDef) == 2) //Only two mons left
				INCREASE_STATUS_VIABILITY(2); //Treat like a middle-priority status move
			else
				INCREASE_STATUS_VIABILITY(3);
			break;

		case FIGHT_CLASS_ENTRY_HAZARDS:
			switch (move) {
				case MOVE_STICKYWEB:
					INCREASE_VIABILITY(7);
					break;
				case MOVE_STEALTHROCK:
					INCREASE_VIABILITY(6);
					break;
				case MOVE_TOXICSPIKES:
					INCREASE_VIABILITY(5);
					break;
				default:
					INCREASE_VIABILITY(4);
					break;
			}
			break;


		case FIGHT_CLASS_DOUBLES_ALL_OUT_ATTACKER:
			break;

		case FIGHT_CLASS_DOUBLES_SETUP_ATTACKER:
			if (BankSideHasTwoTrainers(bankDef))
				INCREASE_STATUS_VIABILITY(1); //Treat like a low-priority status move in multis
			else
				INCREASE_STATUS_VIABILITY(2); //Treat like a middle-priority status move
			break;

		case FIGHT_CLASS_DOUBLES_TRICK_ROOM_ATTACKER:
			if (BankSideHasTwoTrainers(bankDef))
				INCREASE_STATUS_VIABILITY(1); //Treat like a low-priority status move in multis
			else
			{
				if (move == MOVE_STEALTHROCK)
					INCREASE_VIABILITY(13);
				else
					INCREASE_STATUS_VIABILITY(2); //Treat like a middle-priority status move
			}
			break;

		case FIGHT_CLASS_DOUBLES_TRICK_ROOM_SETUP:
			if (BankSideHasTwoTrainers(bankDef))
				INCREASE_STATUS_VIABILITY(1); //Treat like a low-priority status move in multis
			else
				INCREASE_STATUS_VIABILITY(2); //Treat like a middle-priority status move
			break;

		case FIGHT_CLASS_DOUBLES_UTILITY:
			if (BankSideHasTwoTrainers(bankDef))
				INCREASE_STATUS_VIABILITY(1); //Treat like a low-priority status move in multis
			else
				INCREASE_STATUS_VIABILITY(2); //Treat like a middle-priority status move
			break;

		case FIGHT_CLASS_DOUBLES_PHAZING:
			if (BankSideHasTwoTrainers(bankDef))
				INCREASE_STATUS_VIABILITY(1); //Treat like a low-priority status move in multis
			else
				INCREASE_STATUS_VIABILITY(2); //Treat like a middle-priority status move
			break;

		case FIGHT_CLASS_DOUBLES_TEAM_SUPPORT:
			if (BankSideHasTwoTrainers(bankDef))
				INCREASE_STATUS_VIABILITY(1); //Treat like a low-priority status move in multis
			else
				INCREASE_STATUS_VIABILITY(2); //Treat like a middle-priority status move
			break;
	}

	*originalViability = MathMin(viability, 255);
}

void IncreaseFakeOutViability(s16* originalViability, u8 class, u8 bankAtk, u8 bankDef, u16 move)
{
	s16 viability = *originalViability;

	if (move == MOVE_FAKEOUT && IS_DOUBLE_BATTLE)
	{
		u8 partner = PARTNER(bankAtk);
		u16 partnerMove = GetAIChosenMove(partner, bankDef);

		if (BATTLER_ALIVE(partner)
		&&  partnerMove == MOVE_FAKEOUT
		&&  gBattleStruct->moveTarget[partner] == bankDef)
			return; //No benefit to using Fake Out twice on the same opponent
	}

	switch (class) {
		case FIGHT_CLASS_DOUBLES_ALL_OUT_ATTACKER:
			break; //Only use if good damaging move

		case FIGHT_CLASS_DOUBLES_SETUP_ATTACKER:
			break; //Only use if good damaging move

		case FIGHT_CLASS_DOUBLES_TRICK_ROOM_ATTACKER:
			if (!IsTrickRoomActive()) //Don't burn a Trick Room turn to use Fake Out
				INCREASE_VIABILITY(17);
			break;

		case FIGHT_CLASS_DOUBLES_TRICK_ROOM_SETUP:
			INCREASE_VIABILITY(17);
			break;

		case FIGHT_CLASS_DOUBLES_UTILITY:
			INCREASE_VIABILITY(19);
			break;

		case FIGHT_CLASS_DOUBLES_PHAZING:
			INCREASE_VIABILITY(19);
			break;

		case FIGHT_CLASS_DOUBLES_TEAM_SUPPORT:
			INCREASE_VIABILITY(17);
			break;

		default:
			INCREASE_VIABILITY(8);
	}

	*originalViability = MathMin(viability, 255);
}

void IncreasePivotViability(s16* originalViability, u8 class, u8 bankAtk, unusedArg u8 bankDef)
{
	s16 viability = *originalViability;

	if (IS_SINGLE_BATTLE)
	{
		if (class == 0xFF) //Dumb AI
			class = FIGHT_CLASS_SWEEPER_KILL;
	}
	else //Double Battle
	{
		if (class == 0xFF) //Dumb AI
			class = FIGHT_CLASS_DOUBLES_ALL_OUT_ATTACKER;
	}

	switch (class) {
		case FIGHT_CLASS_SWEEPER_KILL:
			INCREASE_VIABILITY(3);
			break;

		case FIGHT_CLASS_SWEEPER_SETUP_STATS:
			INCREASE_VIABILITY(9);
			break;

		case FIGHT_CLASS_SWEEPER_SETUP_STATUS:
			INCREASE_VIABILITY(9);
			break;

		case FIGHT_CLASS_STALL:
			INCREASE_VIABILITY(9);
			break;

		case FIGHT_CLASS_TEAM_SUPPORT_BATON_PASS:
			INCREASE_VIABILITY(9);
			break;

		case FIGHT_CLASS_TEAM_SUPPORT_CLERIC:
			INCREASE_VIABILITY(7);
			break;

		case FIGHT_CLASS_TEAM_SUPPORT_SCREENS:
			INCREASE_VIABILITY(7);
			break;

		case FIGHT_CLASS_TEAM_SUPPORT_PHAZING:
			INCREASE_VIABILITY(4);
			break;

		case FIGHT_CLASS_ENTRY_HAZARDS:
			INCREASE_VIABILITY(3);
			break;

		case FIGHT_CLASS_DOUBLES_ALL_OUT_ATTACKER:
			INCREASE_VIABILITY(18);
			break;

		case FIGHT_CLASS_DOUBLES_SETUP_ATTACKER:
			INCREASE_VIABILITY(18);
			break;

		case FIGHT_CLASS_DOUBLES_TRICK_ROOM_ATTACKER:
			INCREASE_VIABILITY(16);
			break;

		case FIGHT_CLASS_DOUBLES_TRICK_ROOM_SETUP:
			INCREASE_VIABILITY(16);
			break;

		case FIGHT_CLASS_DOUBLES_UTILITY:
			INCREASE_VIABILITY(16);
			break;

		case FIGHT_CLASS_DOUBLES_PHAZING:
			INCREASE_VIABILITY(14);
			break;

		case FIGHT_CLASS_DOUBLES_TEAM_SUPPORT:
			INCREASE_VIABILITY(15);
			break;
	}

	*originalViability = MathMin(viability, 255);
}

void IncreaseFoeProtectionViability(s16* originalViability, u8 class, u8 bankAtk, u8 bankDef)
{
	s16 viability = *originalViability;

	if (class == 0xFF) //Dumb AI
		class = FIGHT_CLASS_DOUBLES_ALL_OUT_ATTACKER;

	switch (class) {
		case FIGHT_CLASS_STALL:
			INCREASE_VIABILITY(3);
			break;

		case FIGHT_CLASS_DOUBLES_ALL_OUT_ATTACKER:
			INCREASE_VIABILITY(12);
			break;

		case FIGHT_CLASS_DOUBLES_SETUP_ATTACKER:
			INCREASE_VIABILITY(10);
			break;

		case FIGHT_CLASS_DOUBLES_TRICK_ROOM_ATTACKER:
			INCREASE_VIABILITY(4);
			break;

		case FIGHT_CLASS_DOUBLES_TRICK_ROOM_SETUP:
			INCREASE_VIABILITY(7);
			break;

		case FIGHT_CLASS_DOUBLES_UTILITY:
			INCREASE_VIABILITY(10);
			break;

		case FIGHT_CLASS_DOUBLES_PHAZING:
			INCREASE_VIABILITY(13);
			break;

		case FIGHT_CLASS_DOUBLES_TEAM_SUPPORT:
			INCREASE_VIABILITY(1);
			break;

		default:
			INCREASE_STATUS_VIABILITY(3);
	}

	*originalViability = MathMin(viability, 255);
}

void IncreaseAllyProtectionViability(s16* originalViability, u8 class)
{
	s16 viability = *originalViability;

	if (class == 0xFF) //Dumb AI
		class = FIGHT_CLASS_DOUBLES_ALL_OUT_ATTACKER;

	switch (class) {
		case FIGHT_CLASS_DOUBLES_ALL_OUT_ATTACKER:
			INCREASE_VIABILITY(15);
			break;

		case FIGHT_CLASS_DOUBLES_SETUP_ATTACKER:
			INCREASE_VIABILITY(14);
			break;

		case FIGHT_CLASS_DOUBLES_TRICK_ROOM_ATTACKER:
			INCREASE_VIABILITY(12);
			break;

		case FIGHT_CLASS_DOUBLES_TRICK_ROOM_SETUP:
			INCREASE_VIABILITY(14);
			break;

		case FIGHT_CLASS_DOUBLES_UTILITY:
			INCREASE_VIABILITY(14);
			break;

		case FIGHT_CLASS_DOUBLES_PHAZING:
			INCREASE_VIABILITY(14);
			break;

		case FIGHT_CLASS_DOUBLES_TEAM_SUPPORT:
			INCREASE_VIABILITY(14);
			break;
	}

	*originalViability = MathMin(viability, 255);
}

void IncreaseTeamProtectionViability(s16* originalViability, u8 class)
{
	s16 viability = *originalViability;

	switch (class) {
		case FIGHT_CLASS_DOUBLES_ALL_OUT_ATTACKER:
			INCREASE_VIABILITY(8);
			break;

		case FIGHT_CLASS_DOUBLES_SETUP_ATTACKER:
			INCREASE_VIABILITY(3);
			break;

		case FIGHT_CLASS_DOUBLES_TRICK_ROOM_ATTACKER:
			INCREASE_VIABILITY(3);
			break;

		case FIGHT_CLASS_DOUBLES_TRICK_ROOM_SETUP:
			INCREASE_VIABILITY(13);
			break;

		case FIGHT_CLASS_DOUBLES_UTILITY:
			INCREASE_VIABILITY(12);
			break;

		case FIGHT_CLASS_DOUBLES_PHAZING:
			INCREASE_VIABILITY(3);
			break;

		case FIGHT_CLASS_DOUBLES_TEAM_SUPPORT:
			INCREASE_VIABILITY(12);
			break;
	}

	*originalViability = MathMin(viability, 255);
}

void IncreaseTailwindViability(s16* originalViability, u8 class, u8 bankAtk, u8 bankDef)
{
	s16 viability = *originalViability;

	switch (class) {
		case FIGHT_CLASS_TEAM_SUPPORT_BATON_PASS:
		case FIGHT_CLASS_TEAM_SUPPORT_CLERIC:
		case FIGHT_CLASS_TEAM_SUPPORT_SCREENS:
		case FIGHT_CLASS_TEAM_SUPPORT_PHAZING:
			INCREASE_STATUS_VIABILITY(2);
			break;

		case FIGHT_CLASS_DOUBLES_SETUP_ATTACKER:
			if (SpeedCalc(bankAtk) < SpeedCalc(bankDef))
				INCREASE_VIABILITY(18);
			break;

		case FIGHT_CLASS_DOUBLES_UTILITY:
			INCREASE_VIABILITY(11);
			break;

		case FIGHT_CLASS_DOUBLES_TEAM_SUPPORT:
			INCREASE_VIABILITY(18);
			break;

		default:
			if (IsClassDoublesSpecific(class))
				INCREASE_STATUS_VIABILITY(3);
			else
				INCREASE_STATUS_VIABILITY(1);
	}

	*originalViability = MathMin(viability, 255);
}

//Also used for things like After You & raising partner stats
void IncreaseHelpingHandViability(s16* originalViability, u8 class)
{
	s16 viability = *originalViability;

	switch (class) {
		case FIGHT_CLASS_DOUBLES_ALL_OUT_ATTACKER:
			INCREASE_VIABILITY(6); //Really just for raising partner's stats
			break;

		case FIGHT_CLASS_DOUBLES_SETUP_ATTACKER:
			INCREASE_VIABILITY(1);
			break;

		case FIGHT_CLASS_DOUBLES_TRICK_ROOM_ATTACKER:
			INCREASE_VIABILITY(1);
			break;

		case FIGHT_CLASS_DOUBLES_TRICK_ROOM_SETUP:
			INCREASE_VIABILITY(15);
			break;

		case FIGHT_CLASS_DOUBLES_UTILITY:
			INCREASE_VIABILITY(13);
			break;

		case FIGHT_CLASS_DOUBLES_PHAZING:
			INCREASE_VIABILITY(1);
			break;

		case FIGHT_CLASS_DOUBLES_TEAM_SUPPORT:
			INCREASE_VIABILITY(10);
			break;
	}

	*originalViability = MathMin(viability, 255);
}

void IncreaseHealPartnerViability(s16* originalViability, u8 class, u8 partner)
{
	s16 viability = *originalViability;

	if (BATTLER_ALIVE(partner)
	&& gBattleMons[partner].hp > ((gBattleMons[partner].maxHP * 2) / 3)) //Only try heal if 2/3 or less HP
		return;

	switch (class) {
		case FIGHT_CLASS_DOUBLES_ALL_OUT_ATTACKER:
			INCREASE_VIABILITY(7); //Heal through abilities that absorb damage
			break;

		case FIGHT_CLASS_DOUBLES_SETUP_ATTACKER:
			INCREASE_VIABILITY(2);
			break;

		case FIGHT_CLASS_DOUBLES_TRICK_ROOM_ATTACKER:
			INCREASE_VIABILITY(2);
			break;

		case FIGHT_CLASS_DOUBLES_TRICK_ROOM_SETUP:
			INCREASE_VIABILITY(12);
			break;

		case FIGHT_CLASS_DOUBLES_UTILITY:
			INCREASE_VIABILITY(15);
			break;

		case FIGHT_CLASS_DOUBLES_PHAZING:
			INCREASE_VIABILITY(2);
			break;

		case FIGHT_CLASS_DOUBLES_TEAM_SUPPORT:
			INCREASE_VIABILITY(13);
			break;
	}

	*originalViability = MathMin(viability, 255);
}

bool8 IncreaseViabilityForSpeedControl(s16* originalViability, u8 class, u8 bankAtk, u8 bankDef)
{
	s16 viability = *originalViability;
	u8 bankAtkPartner = PARTNER(bankAtk);
	bool8 ret = FALSE;

	if (!IsTrickRoomActive() //Don't try to lower speed while Trick Room is up
	&&  (SpeedCalc(bankDef) >= SpeedCalc(bankAtk)
	 || (BATTLER_ALIVE(bankAtkPartner) && SpeedCalc(bankDef) >= SpeedCalc(bankAtkPartner))))
	{
		switch (class) {
			case FIGHT_CLASS_DOUBLES_ALL_OUT_ATTACKER:
			case FIGHT_CLASS_DOUBLES_SETUP_ATTACKER:
			case FIGHT_CLASS_DOUBLES_TRICK_ROOM_ATTACKER:
			case FIGHT_CLASS_DOUBLES_TRICK_ROOM_SETUP:
				break;

			case FIGHT_CLASS_DOUBLES_UTILITY:
				INCREASE_VIABILITY(11);
				ret = TRUE;
				break;

			case FIGHT_CLASS_DOUBLES_TEAM_SUPPORT:
				INCREASE_VIABILITY(11);
				ret = TRUE;
				break;
		}
	}

	*originalViability = MathMin(viability, 255);
	return ret;
}

void IncreaseDoublesDamageViability(s16* originalViability, u8 class, u8 bankAtk, u8 bankDef, u16 move)
{
	s16 viability = *originalViability;

	if (class == 0xFF) //Dumb AI
		class = FIGHT_CLASS_DOUBLES_ALL_OUT_ATTACKER;

	INCREASE_VIABILITY(gDoublesDamageViabilityMapping[class - FIGHT_CLASS_DOUBLES_ALL_OUT_ATTACKER][GetDoubleKillingScore(move, bankAtk, bankDef)]);
	*originalViability = MathMin(viability, 255);
}

void IncreaseDoublesDamageViabilityToScore(s16* originalViability, u8 class, u8 score, u8 bankAtk, u8 bankDef)
{
	s16 viability = *originalViability;
	u8 bankAtkPartner = PARTNER(bankAtk);

	if (gChosenMovesByBanks[bankAtkPartner] != MOVE_NONE) //Check partner's target
	{
		u16 partnerMove = GetAIChosenMove(bankAtkPartner, bankDef);
		u16 partnerTarget = gBattleStruct->moveTarget[bankAtkPartner];

		if (bankDef == partnerTarget											//Partner is targeting same target
		&& partnerTarget != bankAtkPartner										//and not targeting itself
		&& MoveKnocksOutXHits(partnerMove, bankAtkPartner, partnerTarget, 1))	//and can KO it with its chosen move.
		{
			INCREASE_VIABILITY(gDoublesDamageViabilityMapping[class - FIGHT_CLASS_DOUBLES_ALL_OUT_ATTACKER][1]); //Minimal score
			*originalViability = MathMin(viability, 255);
			return;
		}
	}

	INCREASE_VIABILITY(gDoublesDamageViabilityMapping[class - FIGHT_CLASS_DOUBLES_ALL_OUT_ATTACKER][score]);
	*originalViability = MathMin(viability, 255);
}
