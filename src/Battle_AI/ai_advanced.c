#include "../defines.h"
#include "../defines_battle.h"
#include "../../include/random.h"
#include "../../include/constants/items.h"

#include "../../include/new/accuracy_calc.h"
#include "../../include/new/ai_advanced.h"
#include "../../include/new/ai_master.h"
#include "../../include/new/ai_util.h"
#include "../../include/new/ai_switching.h"
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
	FIGHT_CLASS_SWEEPER_SETUP_SCREENS,
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
	FIGHT_CLASS_DOUBLES_TOTAL_TEAM_SUPPORT, //Only different from above is that this one won't get Helping Handed
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
	[FIGHT_CLASS_DOUBLES_TOTAL_TEAM_SUPPORT - FIGHT_CLASS_DOUBLES_ALL_OUT_ATTACKER] =
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
	return class == FIGHT_CLASS_TEAM_SUPPORT_SCREENS || class == FIGHT_CLASS_SWEEPER_SETUP_SCREENS;
}

bool8 IsClassSupportScreener(u8 class)
{
	return IsClassScreener(class) && IsClassTeamSupport(class);
}

bool8 IsClassCleric(u8 class)
{
	return class == FIGHT_CLASS_TEAM_SUPPORT_CLERIC;
}

bool8 IsClassDoublesPhazer(u8 class)
{
	return class == FIGHT_CLASS_DOUBLES_PHAZING;
}

bool8 IsClassPhazer(u8 class)
{
	return class == FIGHT_CLASS_TEAM_SUPPORT_PHAZING
		|| IsClassDoublesPhazer(class);
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
		|| class == FIGHT_CLASS_DOUBLES_TRICK_ROOM_SETUP;
}

bool8 IsClassDoublesUtility(u8 class)
{
	return class == FIGHT_CLASS_DOUBLES_UTILITY;
}

bool8 IsClassDoublesTeamSupport(u8 class)
{
	return class == FIGHT_CLASS_DOUBLES_TEAM_SUPPORT || class == FIGHT_CLASS_DOUBLES_TOTAL_TEAM_SUPPORT;
}

bool8 IsClassDoublesTotalTeamSupport(u8 class)
{
	return class == FIGHT_CLASS_DOUBLES_TOTAL_TEAM_SUPPORT;
}

bool8 IsClassDoublesSpecific(u8 class)
{
	return class == FIGHT_CLASS_DOUBLES_ALL_OUT_ATTACKER
		|| class == FIGHT_CLASS_DOUBLES_SETUP_ATTACKER
		|| class == FIGHT_CLASS_DOUBLES_TRICK_ROOM_ATTACKER
		|| class == FIGHT_CLASS_DOUBLES_TRICK_ROOM_SETUP
		|| class == FIGHT_CLASS_DOUBLES_UTILITY
		|| class == FIGHT_CLASS_DOUBLES_PHAZING
		|| class == FIGHT_CLASS_DOUBLES_TEAM_SUPPORT
		|| class == FIGHT_CLASS_DOUBLES_TOTAL_TEAM_SUPPORT;
}

bool8 IsClassDoublesAttacker(u8 class)
{
	return class == FIGHT_CLASS_DOUBLES_ALL_OUT_ATTACKER
		|| class == FIGHT_CLASS_DOUBLES_TRICK_ROOM_ATTACKER
		|| class == FIGHT_CLASS_DOUBLES_SETUP_ATTACKER;
}

bool8 IsClassDamager(u8 class)
{
	return IsClassSweeper(class) || IsClassDoublesAttacker(class)
		|| class == FIGHT_CLASS_SWEEPER_SETUP_SCREENS;
}

bool8 IsClassGoodToTaunt(u8 class)
{
	return class == FIGHT_CLASS_STALL
		|| class == FIGHT_CLASS_SWEEPER_SETUP_SCREENS
		|| class == FIGHT_CLASS_TEAM_SUPPORT_BATON_PASS
		|| class == FIGHT_CLASS_TEAM_SUPPORT_CLERIC
		|| class == FIGHT_CLASS_TEAM_SUPPORT_SCREENS
		|| class == FIGHT_CLASS_ENTRY_HAZARDS
		|| class == FIGHT_CLASS_DOUBLES_TRICK_ROOM_SETUP
		|| class == FIGHT_CLASS_DOUBLES_UTILITY
		|| class == FIGHT_CLASS_DOUBLES_TEAM_SUPPORT
		|| class == FIGHT_CLASS_DOUBLES_TOTAL_TEAM_SUPPORT;
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
		bool8 isRaidBoss = IsRaidBattle() && bank == BANK_RAID_BOSS;
		u16 moves[MAX_MON_MOVES] = {0};

		for (u8 i = 0; i < MAX_MON_MOVES; ++i)
		{
			if (isRaidBoss && SPLIT(gBattleMons[bank].moves[i]) == SPLIT_STATUS)
				moves[i] = gBattleMons[bank].moves[i]; //Raid Boss can't use Max Guard
			else
				moves[i] = GetMaxMove(bank, i);
		}

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
	bool8 phazingMove = FALSE;

	bool8 isSingleBattle;
	if (bank == 0xFF)
		isSingleBattle = (gBattleTypeFlags & BATTLE_TYPE_DOUBLE) == 0;
	else
		isSingleBattle = !(gBattleTypeFlags & BATTLE_TYPE_DOUBLE && ((BATTLER_ALIVE(FOE(bank)) && BATTLER_ALIVE(PARTNER(FOE(bank)))) || BATTLER_ALIVE(PARTNER(bank))));

	if (isSingleBattle) //AI Single Battle
	{
		bool8 leechSeed = FALSE;
		bool8 protectionMove = FALSE;

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
					if (ViableMonCountFromBankLoadPartyRange(bank) > 1)
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
				switch (moveEffect)
				{
					case EFFECT_ROAR:
						phazingMove = TRUE; //Could be Roar support for breaking setups
						break;

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
								++reflectionNum;
								break;
						}
						break;

					case EFFECT_LIGHT_SCREEN:
						++reflectionNum;
						break;

					case EFFECT_LEECH_SEED:
						leechSeed = TRUE;
						break;
					
					case EFFECT_PROTECT:
						protectionMove = TRUE;
						break;

					case EFFECT_TRAP:
					case EFFECT_MEAN_LOOK:
						class = FIGHT_CLASS_STALL;
						break;

					default:
						if (IsChoiceItemEffectOrAbility(itemEffect, ability) || itemEffect == ITEM_EFFECT_ASSAULT_VEST)
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

			switch (moveEffect)
			{
				case EFFECT_ATTACK_UP:
				case EFFECT_DEFENSE_UP:
				case EFFECT_SPEED_UP:
				case EFFECT_SPECIAL_ATTACK_UP:
				case EFFECT_SPECIAL_DEFENSE_UP:
				case EFFECT_ACCURACY_UP:
				case EFFECT_EVASION_UP:
				case EFFECT_ATK_SPATK_UP:
				case EFFECT_ATK_ACC_UP:
				case EFFECT_DEF_SPD_UP:
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
			{
				if (attackMoveNum >= 2)
					class = FIGHT_CLASS_SWEEPER_SETUP_SCREENS;
				else
					class = FIGHT_CLASS_TEAM_SUPPORT_SCREENS;
			}
			else if (entryHazardNum >= 1)
			{
				if (phazingMove)
					class = FIGHT_CLASS_TEAM_SUPPORT_PHAZING;
				else
					class = FIGHT_CLASS_ENTRY_HAZARDS;
			}
			else if (attackMoveNum >= 3)
			{
				if (boostingMove)
					class = FIGHT_CLASS_SWEEPER_SETUP_STATS;
				else if (statusMoveNum > 0)
					class = FIGHT_CLASS_SWEEPER_SETUP_STATUS;
				else if (phazingMove) //Goal is to phaze out when stats are up
					class = FIGHT_CLASS_SWEEPER_SETUP_STATUS;
				else
					class = FIGHT_CLASS_SWEEPER_KILL;
			}
			else if (leechSeed && protectionMove)
				class = FIGHT_CLASS_STALL;
			else if (attackMoveNum >= 2 && (boostingMove || statusMoveNum > 0 || phazingMove))
			{
				//A class should always be assigned here because of the conditions to enter this scope
				if (boostingMove)
					class = FIGHT_CLASS_SWEEPER_SETUP_STATS;
				else if (statusMoveNum > 0 || phazingMove)
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
		u8 numOffensiveBoostingMoves = 0;

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
					case EFFECT_ATK_ACC_UP:
						++numOffensiveBoostingMoves;
						break;

					case EFFECT_ATK_SPATK_UP:
						if (move == MOVE_ROTOTILLER)
						{
							if (bank == 0xFF || !IsOfType(bank, TYPE_GRASS) || !CheckGrounding(bank))
								break; //Don't boost self
						}
						else if (move == MOVE_GEARUP)
						{
							if (!IsPlusMinusAbility(ability))
								break; //Don't boost self
						}

						++numOffensiveBoostingMoves;
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

			if (SPLIT(move) != SPLIT_STATUS
			&& move != MOVE_NUZZLE //Really just meant for the Paralysis
			&& move != MOVE_FAKEOUT) //Really just for the flinch
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
				if (attackMoveNum == 3 || (attackMoveNum == 2 && hasPersonalProtect))
					class = FIGHT_CLASS_DOUBLES_SETUP_ATTACKER;
				else
					class = FIGHT_CLASS_DOUBLES_PHAZING;
			}
			else if (attackMoveNum >= MAX_MON_MOVES || (attackMoveNum == 3 && hasPersonalProtect))
			{
				if (itemEffect == ITEM_EFFECT_ROOM_SERVICE || itemEffect == ITEM_EFFECT_IRON_BALL || itemEffect == ITEM_EFFECT_MACHO_BRACE)
					class = FIGHT_CLASS_DOUBLES_TRICK_ROOM_ATTACKER;
				else
					class = FIGHT_CLASS_DOUBLES_ALL_OUT_ATTACKER;
			}
			else if (attackMoveNum + numOffensiveBoostingMoves >= 3 || (attackMoveNum + numOffensiveBoostingMoves == 2 && hasPersonalProtect))
			{
				if (itemEffect == ITEM_EFFECT_ROOM_SERVICE || itemEffect == ITEM_EFFECT_IRON_BALL || itemEffect == ITEM_EFFECT_MACHO_BRACE)
					class = FIGHT_CLASS_DOUBLES_TRICK_ROOM_ATTACKER;
				else
					class = FIGHT_CLASS_DOUBLES_SETUP_ATTACKER;
			}
			else if (hasTeamSupport)
			{
				if (attackMoveNum <= 1) //Clearly wasn't meant for attacking
					class = FIGHT_CLASS_DOUBLES_TOTAL_TEAM_SUPPORT;
				else
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


bool8 ShouldPrioritizeKOingFoesDoubles(u8 bank)
{
	//Handles what happens when two targets have the same move score.
	//Should the AI use the move that KOs more targets, or should it pick one at random?

	if (SIDE(bank) == B_SIDE_PLAYER) //Partners should always try to KO as much as they can
		return TRUE;

	if (AI_THINKING_STRUCT->aiFlags <= AI_SCRIPT_CHECK_BAD_MOVE) //Dumber AI should always go for the kill
		return TRUE;

	if (IsClassDoublesAttacker(GetBankFightingStyle(bank)))
		return TRUE; //Hyper offense mons should always go for the kill

	return FALSE;
}

bool8 ShouldPrioritizeMostDamageDoubles(u8 bank)
{
	//Handles what happens when two targets have the same move score.
	//Should the AI go for the one that it does a higher damage percentage to, or should it pick one at random?

	if (SIDE(bank) == B_SIDE_PLAYER) //Partners should always try to KO as much as they can
		return TRUE;

	if (AI_THINKING_STRUCT->aiFlags <= AI_SCRIPT_SEMI_SMART)
		return TRUE;

	if (IsClassDoublesAttacker(GetBankFightingStyle(bank)))
		return (AIRandom() % 4) == 0; //25% chance this turn - can get too predictable otherwise

	return FALSE;
}

bool8 ShouldPrioritizeDangerousTarget(u8 bank)
{
	//Handles what happens when two targets have the same move score and both take over 50% of their max HP in damage.
	//Should the AI go for the one that is more dangerous to it, or should is pick one at random?

	if (SIDE(bank) == B_SIDE_PLAYER) //Partners should always go for their threats.
		return TRUE;

	if (AI_THINKING_STRUCT->aiFlags <= AI_SCRIPT_SEMI_SMART)
		return FALSE; //Semi-smart Trainers shouldn't care

	if (IsClassDoublesAttacker(GetBankFightingStyle(bank)))
		return TRUE;

	return FALSE;
}

bool8 ShouldTrap(u8 bankAtk, u8 bankDef, u16 move, u8 class)
{
	if (WillFaintFromSecondaryDamage(bankAtk))
		return FALSE;

	if (IsClassStall(class))
	{
		if (!HasMonToSwitchTo(bankDef))
			return FALSE;

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

	switch (gBattleMoves[move].effect) {
		case EFFECT_RESTORE_HP:
			if (move == MOVE_LIFEDEW || move == MOVE_JUNGLEHEALING)
				amountToRecover = MathMax(1, maxHp / 4);
			else
				amountToRecover = MathMax(1, maxHp / 2);
			break;

		case EFFECT_MORNING_SUN:
			if (gBattleWeather == 0 || gBattleWeather & WEATHER_AIR_CURRENT_PRIMAL || !WEATHER_HAS_EFFECT)
			{
				amountToRecover = MathMax(1, maxHp / 2);
				break;
			}

			switch (move) {
				case MOVE_SHOREUP:
					if (gBattleWeather & WEATHER_SANDSTORM_ANY)
						amountToRecover = maxHp;
					else
						amountToRecover = MathMax(1, maxHp / 2);
					break;

				default:
					if (gBattleWeather & WEATHER_SUN_ANY)
					{
						if (AffectedBySun(bankAtk))
							amountToRecover = maxHp;
						else
							amountToRecover = MathMax(1, maxHp / 2);
					}
					else if (gBattleWeather != 0) //Weather other than sunny
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

		case EFFECT_DREAM_EATER:
			if (ABILITY(bankDef) != ABILITY_COMATOSE && !(gBattleMons[bankDef].status1 & STATUS1_SLEEP))
				break; //Move will fail
			//Fallthrough
		case EFFECT_ABSORB:
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
					amountToRecover = (13 * predictedDmg) / 10;
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

			if (gStatuses3[bankAtk] & STATUS3_ROOTED)
			{
				u16 hp = MathMax(1, maxHp / 16);
				if (itemEffect == ITEM_EFFECT_BIG_ROOT)
					amountToRecover += (130 * hp) / 100;
			}

			if (gStatuses3[bankAtk] & STATUS3_AQUA_RING)
			{
				u16 hp = MathMax(1, maxHp / 16);
				if (itemEffect == ITEM_EFFECT_BIG_ROOT)
					amountToRecover += (130 * hp) / 100;
			}

			for (u8 i = 0; i < gBattlersCount; ++i)
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

			if (gBattleWeather & WEATHER_RAIN_ANY && WEATHER_HAS_EFFECT && AffectedByRain(bankAtk))
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

			if (GetSecondaryEffectDamage(bankAtk) >= amountToRecover)
				amountToRecover = 0; //The AI wouldn't actually recover any HP
	}

	return MathMin(amountToRecover, gBattleMons[bankAtk].maxHP - 1); //Can recover at most up to Max HP - 1
}

bool8 ShouldRecover(u8 bankAtk, u8 bankDef, u16 move)
{
	u32 healAmount = GetAmountToRecoverBy(bankAtk, bankDef, move);

	//if (IS_SINGLE_BATTLE)
	//{
		if (move == 0xFFFF || MoveWouldHitFirst(move, bankAtk, bankDef)) //Using item or attacker goes first
		{
			if (CanKnockOut(bankDef, bankAtk)
			&& !CanKnockOutAfterHealing(bankDef, bankAtk, healAmount, 1, FALSE))
				return TRUE;

			if (Can2HKO(bankDef, bankAtk)
			&& !IsBankIncapacitated(bankDef)
			&& healAmount > gBattleMons[bankAtk].hp //Will get back more damage than the foe would do
			&& (AI_THINKING_STRUCT->simulatedRNG[1] & 1) == 0) //Heal 50% of the time
				return TRUE;				
		}
		else //Opponent Goes First
		{
			if (!CanKnockOut(bankDef, bankAtk)) //Enemy can't kill attacker
			{
				if (!IsBankIncapacitated(bankDef)
				&& Can2HKO(bankDef, bankAtk)
				&& !CanKnockOutAfterHealing(bankDef, bankAtk, healAmount, 1, TRUE)) //Deal one hit first since it's slower
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
	&& CanBeBurned(bank, bank, FALSE)
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

	if (WillFaintFromSecondaryDamage(bankAtk) && !IsMoxieAbility(defAbility))
		return FALSE; //Don't protect if you're going to faint after protecting and foe can't get boosts from your KO

	if (IsBankIncapacitated(bankDef))
		return FALSE; //Don't Protect against an opponent that isn't going to do anything

	if ((!isAtkDynamaxed && BankHoldingUsefulItemToProtectFor(bankAtk))
	||  (!isAtkDynamaxed && BankHasAbilityUsefulToProtectFor(bankAtk, bankDef))
	||  (!isAtkDynamaxed && WillFaintFromSecondaryDamage(bankDef)) //Don't protect if you're Dynamaxed because Max Moves can have beneficial side effects
	||  (!isAtkDynamaxed && IsTrapped(bankDef, TRUE) && IsTakingSecondaryDamage(bankDef, FALSE)) //Not like the foe will be able to go anywhere
	||  (IsDynamaxed(bankDef) && (!IsRaidBattle() || bankDef != BANK_RAID_BOSS) && !IsDynamaxed(bankAtk) && !ShouldAIDynamax(bankAtk) && SPLIT(predictedMove) != SPLIT_STATUS) //Foe is going to attack with a Max Move and ai won't be dynamaxed
	||  predictedMoveEffect == EFFECT_EXPLOSION
	|| (predictedMoveEffect == EFFECT_SEMI_INVULNERABLE && BATTLER_SEMI_INVULNERABLE(bankDef) //Foe coming down
	   && gBattleMoves[predictedMove].flags & FLAG_PROTECT_AFFECTED))
	{
		return USE_PROTECT;
	}

	if (gStatuses3[bankDef] & STATUS3_YAWN //Foe is about to fall asleep
	&& !CanKnockOut(bankAtk, bankDef) //Foe can't be KOd this turn
	&& CanKnockOut(bankDef, bankAtk) //But foe can KO attacker this turn
	&& ViableMonCountFromBank(bankAtk) == 1) //And attacker is the last mon
		return USE_PROTECT; //Protect and force the opponent to sleep

	if (IS_SINGLE_BATTLE)
	{
		u32 healAmount = GetAmountToRecoverBy(bankAtk, bankDef, move);

		if (CanKnockOut(bankDef, bankAtk)) //Foe can kill
		{
			if (!isAtkDynamaxed && !CanKnockOutAfterHealing(bankDef, bankAtk, healAmount, 1, FALSE))
				return USE_PROTECT;
			else if (!isAtkDynamaxed && IsTakingSecondaryDamage(bankDef, FALSE))
				return USE_PROTECT;
			else
				return USE_STATUS_THEN_PROTECT;
		}
		else if (Can2HKO(bankDef, bankAtk))
		{
			if (!isAtkDynamaxed && !CanKnockOutAfterHealing(bankDef, bankAtk, healAmount, 2, FALSE))
				return USE_PROTECT;
			else if (!isAtkDynamaxed && IsTakingSecondaryDamage(bankDef, FALSE))
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
				&&  ABILITY(bankAtk) != ABILITY_TELEPATHY
				&&  GetBaseMoveTarget(partnerMove, partner) & MOVE_TARGET_ALL //Partner using move that hits everyone
				&& !(AI_SpecialTypeCalc(partnerMove, partner, bankAtk) & MOVE_RESULT_NO_EFFECT) //It affects this mon
				&& !IsDamagingMoveUnusable(partnerMove, partner, bankAtk)) //And it will work against this mon (considers Volt Absorb, Storm Drain, etc.)
				{
					return PROTECT_FROM_ALLIES; //Protect if partner is going to use a move that damages the whole field
				}
			}

			if (!isAtkDynamaxed && GetDoubleKillingScore(partnerMove, partner, bankDef) >= BEST_DOUBLES_KO_SCORE - 1)
			{
				return PROTECT_FROM_FOES; //Partner has this covered
			}
		}

		#if (defined FLAG_HOOPA_SOS_BATTLE && defined SPECIES_HOOPA_UNBOUND)
		if (FlagGet(FLAG_HOOPA_SOS_BATTLE) && !(gBattleTypeFlags & BATTLE_TYPE_TRAINER) && SPECIES(bankAtk) == SPECIES_HOOPA_UNBOUND)
			return PROTECT_FROM_FOES; //Higher protect chance so partner can be respawned more often
		#endif
	}

	return DONT_PROTECT;
}

bool8 ShouldPhaze(u8 bankAtk, u8 bankDef, u16 move, u8 class)
{
	if (IsDynamaxed(bankDef))
		return FALSE; //These Pokemon can't be forced out.

	if (CanKnockOutWithoutMove(move, bankAtk, bankDef, TRUE))
		return FALSE; //KO if you can

	if (IS_SINGLE_BATTLE)
	{
		if (CanKnockOut(bankAtk, bankDef))
			return FALSE; //KO if you can

		if (gStatuses3[bankDef] & STATUS3_PERISH_SONG)
		{
			if (gDisableStructs[bankDef].perishSongTimer == 1)
				return FALSE; //They're going to faint this turn if they don't switch, so why bother
			else if (gDisableStructs[bankDef].perishSongTimer == 2) //Will faint at the end of next turn
			{
				if (!MoveWouldHitFirst(move, bankAtk, bankDef))
					return FALSE; //Why waste time when they'll probably switch at the beginning of next turn
			}
		}

		switch (gBattleMoves[move].effect)
		{
			case EFFECT_ROAR:
				if (IsClassPhazer(class)) //So Roar supporters don't spam this
				{
					if (gSideTimers[SIDE(bankDef)].srAmount > 0 //Has some hurtful hazard
					||  gSideTimers[SIDE(bankDef)].tspikesAmount >= 1
					||  gSideTimers[SIDE(bankDef)].spikesAmount >= 1
					||  gSideTimers[SIDE(bankDef)].steelsurge > 0)
						return TRUE;
				}
				//Fallthrough
			default:
				if (AnyUsefulStatIsRaised(bankDef))
					return TRUE;
				break;
		}
	}
	else //Double Battle
	{
		switch (gBattleMoves[move].effect)
		{
			case EFFECT_ROAR:
				if (IsClassPhazer(class))
				{
					if (gSideTimers[SIDE(bankDef)].srAmount > 0 //Has some hurtful hazard
					||  gSideTimers[SIDE(bankDef)].tspikesAmount >= 1
					||  gSideTimers[SIDE(bankDef)].spikesAmount >= 1
					||  AnyUsefulStatIsRaised(bankDef))
						return TRUE;
				}
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

	return FALSE;
}

bool8 ShouldUseWishAromatherapy(u8 bankAtk, u8 bankDef, u16 move, u8 class)
{
	u32 i;
	u8 firstId, lastId;
	struct Pokemon* party;
	bool8 hasStatus = FALSE;
	bool8 needHealing = FALSE;
	party = LoadPartyRange(bankAtk, &firstId, &lastId);

	//Check if last attacking mon should even bother
	if (gBattleMoves[move].effect == EFFECT_WISH && ViableMonCountFromBank(bankAtk) <= 1)
	{
		//Don't bother using Wish if last mon and will faint before getting healed
		if ((Can2HKO(bankDef, bankAtk) //Foe would KO before Wish would take effect at the end of next turn
		 && !HasProtectionMoveInMoveset(bankAtk, CHECK_REGULAR_PROTECTION)) //And there's no protecting against that attack
		|| WillFaintFromSecondaryDamage(bankAtk))
			return FALSE;
	}

	//Check if fighting last foe mon
	if (CanKnockOut(bankAtk, bankDef)
	&& ViableMonCountFromBank(bankDef) <= 1)
		return FALSE; //Don't waste a turn wishing and just KO the enemy

	//Check if move will have effect
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
		switch (gBattleMoves[move].effect)
		{
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
		switch (gBattleMoves[move].effect)
		{
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
				if (CalcMoveSplitFromParty(move, &party[i]) == split)
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
		if (gBattleWeather & WEATHER_HAIL_ANY
		&& (move == MOVE_AURORAVEIL || move == MOVE_G_MAX_RESONANCE_P || move == MOVE_G_MAX_RESONANCE_S)
		&& !((gSideStatuses[bankAtk] & (SIDE_STATUS_REFLECT | SIDE_STATUS_LIGHTSCREEN)) == (SIDE_STATUS_REFLECT | SIDE_STATUS_LIGHTSCREEN)))
			return TRUE;
		else
		{
			u16 defPrediction = IsValidMovePrediction(bankDef, bankAtk); //Move foe is probably going to use
			
			//Check movesets of both enemies to determine which move is best to prioritize
			bool8 defPhysicalMoveInMoveset = RealPhysicalMoveInMoveset(bankDef) || (IS_DOUBLE_BATTLE && BATTLER_ALIVE(PARTNER(bankDef)) && RealPhysicalMoveInMoveset(PARTNER(bankDef)));
			bool8 defSpecialMoveInMoveset = SpecialMoveInMoveset(bankDef) || (IS_DOUBLE_BATTLE && BATTLER_ALIVE(PARTNER(bankDef)) && SpecialMoveInMoveset(PARTNER(bankDef)));

			switch (gBattleMoves[move].effect) {
				case EFFECT_REFLECT:
					if (defPhysicalMoveInMoveset || MoveSplitOnTeam(bankDef, SPLIT_PHYSICAL)) //Team could be hit by a physical move at some point
					{
						bool8 hasLightScreen = !(gSideStatuses[bankAtk] & SIDE_STATUS_LIGHTSCREEN) //Light Screen isn't already up
											&& MoveInMovesetAndUsable(MOVE_LIGHTSCREEN, bankAtk); //Attacker could also use Light Screen

						if (!defPhysicalMoveInMoveset && !defSpecialMoveInMoveset) //Target has no attacking moves
							return TRUE; //But opposing team does so just use Reflect now

						if (defPhysicalMoveInMoveset && defSpecialMoveInMoveset //Foe has both physical and special moves
						&& hasLightScreen //Attacker could also use Light Screen instead
						&& CalcMoveSplit(defPrediction, bankDef, bankAtk) == SPLIT_SPECIAL) //Foe is probably going to hit with a special move
							return FALSE; //Use Light Screen to survive longer

						if (defPhysicalMoveInMoveset //Foe just has a physical move
						|| !hasLightScreen || !defSpecialMoveInMoveset) //No chance of using Light Screen
							return TRUE; //Use Reflect
					}
					break;

				case EFFECT_LIGHT_SCREEN:
					if (defSpecialMoveInMoveset || MoveSplitOnTeam(bankDef, SPLIT_SPECIAL))
					{
						bool8 hasReflect = !(gSideStatuses[bankAtk] & SIDE_STATUS_REFLECT) //Reflect isn't already up
											&& MoveInMovesetAndUsable(MOVE_REFLECT, bankAtk); //Attacker could also use Reflect

						if (!defPhysicalMoveInMoveset && !defSpecialMoveInMoveset) //Target has no attacking moves
							return TRUE; //But opposing team does so just use Light Screen now

						if (defPhysicalMoveInMoveset && defSpecialMoveInMoveset //Foe has both physical and special moves
						&& hasReflect //Attacker could also use Reflect instead
						&& CalcMoveSplit(defPrediction, bankDef, bankAtk) == SPLIT_PHYSICAL) //Foe is probably going to hit with a physcial move
							return FALSE; //Use Reflect to survive longer

						if (defSpecialMoveInMoveset //Foe just has a special move
						|| !hasReflect || !defPhysicalMoveInMoveset) //No chance of using Reflect
							return TRUE; //Use Light Screen
					}
					break;
			}
		}
	}

	return FALSE;
}

bool8 ShouldUseFakeOut(u8 bankAtk, u8 bankDef, u8 defAbility)
{
	if (gDisableStructs[bankAtk].isFirstTurn
	&& IsUsefulToFlinchTarget(bankDef)
	&& CanBeFlinched(bankDef, bankAtk, defAbility, MOVE_FAKEOUT))
	{
		if (IS_DOUBLE_BATTLE)
		{
			if (!CanMovePredictionProtectAgainstMove(bankDef, bankAtk, MOVE_FAKEOUT))
			{
				if (CanBeChoiceLocked(bankAtk)           //Don't lock the attacker into Fake Out
				&& ViableMonCountFromBankLoadPartyRange(bankAtk) <= 2) //if they can't switch out afterwards.
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
			if (CanBeChoiceLocked(bankAtk)           //Don't lock the attacker into Fake Out
			&& ViableMonCountFromBankLoadPartyRange(bankAtk) <= 1) //if they can't switch out afterwards.
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
	if (gNewBS->ai.goodToPivot & gBitTable[bankAtk]) //Set in switching checks
		return PIVOT_IMMEDIATELY;

	bool8 hasUsefulStatBoost = (AnyUsefulOffseniveStatIsRaised(bankAtk) && IsClassDamager(class))
							|| STAT_STAGE(bankDef, STAT_STAGE_EVASION) >= 6 + 4; //Significant boost in evasion for any class

	u8 backupBattler = gActiveBattler;
	gActiveBattler = bankAtk;
	u8 switchFlags = GetMostSuitableMonToSwitchIntoFlags();
	gActiveBattler = backupBattler;

	u8 atkAbility = ABILITY(bankAtk);
	u8 defAbility = ABILITY(bankDef);

	if (WillTakeSignificantDamageFromEntryHazards(bankAtk, 4)) //Don't switch out if you'll do a quarter or more damage to yourself on switch in
		return DONT_PIVOT;

	if (IS_SINGLE_BATTLE)
	{
		if (!HasMonToSwitchTo(bankAtk))
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
					&& SPLIT(move) != SPLIT_STATUS //Damaging pivot move
					&& ((switchFlags & SWITCHING_FLAG_RESIST_ALL_MOVES && switchFlags & (SWITCHING_FLAG_KO_FOE | SWITCHING_FLAG_CAN_REMOVE_HAZARDS)) //Resists all moves and can KO or remove hazards
					 || IsAffectedByFocusSash(bankDef)
					 || IsAffectedBySturdy(defAbility, bankDef)
					 || IsDamageHalvedDueToFullHP(bankDef, defAbility, move, atkAbility))) //Pivot to break the sash/sturdy/multiscale
						return PIVOT;
				}
				else if (!hasUsefulStatBoost) //3HKO+ the foe
				{
					if (IsClassDamager(class)
					&& SPLIT(move) != SPLIT_STATUS //Damaging pivot move
					&& (IsAffectedByFocusSash(bankDef)
					 || IsAffectedBySturdy(defAbility, bankDef)
					 || IsDamageHalvedDueToFullHP(bankDef, defAbility, move, atkAbility))) //Pivot to break the sash/sturdy/multiscale
						return PIVOT;

					if (switchFlags & (SWITCHING_FLAG_WALLS_FOE | SWITCHING_FLAG_RESIST_ALL_MOVES)) //Switched in mon won't take too much damage
					{
						if (gSideStatuses[bankAtk] & SIDE_STATUS_SPIKES && switchFlags & SWITCHING_FLAG_CAN_REMOVE_HAZARDS)
							return PIVOT;

						if (WillFaintFromSecondaryDamage(bankAtk))
							return PIVOT;

						if (IsClassDamager(class))
						{
							bool8 physMoveInMoveset = RealPhysicalMoveInMoveset(bankAtk);
							bool8 specMoveInMoveset = SpecialMoveInMoveset(bankAtk);

							//Pivot if attacking stats are bad
							if (physMoveInMoveset && !specMoveInMoveset)
							{
								if (STAT_STAGE(bankAtk, STAT_STAGE_ATK) <= OFFENSIVE_STAT_MIN_NUM)
									return PIVOT;
							}
							else if (!physMoveInMoveset && specMoveInMoveset)
							{
								if (STAT_STAGE(bankAtk, STAT_STAGE_SPATK) <= OFFENSIVE_STAT_MIN_NUM)
									return PIVOT;
							}
							else if (physMoveInMoveset && specMoveInMoveset)
							{
								if (STAT_STAGE_ATK <= OFFENSIVE_STAT_MIN_NUM && STAT_STAGE(bankAtk, STAT_STAGE_SPATK) <= OFFENSIVE_STAT_MIN_NUM)
									return PIVOT;
							}

							return CAN_TRY_PIVOT;
						}
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
					if (IsClassDamager(class)
					&& switchFlags & SWITCHING_FLAG_KO_FOE //New mon can KO the foe
					&& switchFlags & (SWITCHING_FLAG_OUTSPEEDS | SWITCHING_FLAG_WALLS_FOE | SWITCHING_FLAG_RESIST_ALL_MOVES)) //New mon will go first or survive a hit
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
					if (IsClassDamager(class) && SPLIT(move) != SPLIT_STATUS) //Damaging move
					{
						if ((switchFlags & SWITCHING_FLAG_KO_FOE && switchFlags & (SWITCHING_FLAG_OUTSPEEDS | SWITCHING_FLAG_WALLS_FOE | SWITCHING_FLAG_RESIST_ALL_MOVES)) //New mon will go first and KO or survive a hit
						 || (switchFlags & SWITCHING_FLAG_RESIST_ALL_MOVES && switchFlags & SWITCHING_FLAG_CAN_REMOVE_HAZARDS) //Resists all moves and can remove hazards
						 || IsAffectedByFocusSash(bankDef)
						 || IsAffectedBySturdy(defAbility, bankDef)
						 || IsDamageHalvedDueToFullHP(bankDef, defAbility, move, atkAbility)) //Pivot to break the sash
							return PIVOT;
					}

					//Otherwise try to get foe hp down to where the CanKnockOut check is reached
				}
				else
				{
					if (IsClassDamager(class)
					&& switchFlags & SWITCHING_FLAG_KO_FOE
					&& switchFlags & SWITCHING_FLAG_OUTSPEEDS)
						return PIVOT; //Only switch if way better matchup

					if (!hasUsefulStatBoost
					&& (switchFlags & (SWITCHING_FLAG_OUTSPEEDS | SWITCHING_FLAG_WALLS_FOE | SWITCHING_FLAG_RESIST_ALL_MOVES))) //New mon will either go first or be able to take a hit
					{
						if (gSideStatuses[bankAtk] & SIDE_STATUS_SPIKES && switchFlags & SWITCHING_FLAG_CAN_REMOVE_HAZARDS)
							return PIVOT;

						if (WillFaintFromSecondaryDamage(bankAtk))
							return PIVOT;

						if (IsClassDamager(class))
						{
							bool8 physMoveInMoveset = RealPhysicalMoveInMoveset(bankAtk);
							bool8 specMoveInMoveset = SpecialMoveInMoveset(bankAtk);

							//Pivot if attacking stats are bad
							if (physMoveInMoveset && !specMoveInMoveset)
							{
								if (STAT_STAGE(bankAtk, STAT_STAGE_ATK) <= OFFENSIVE_STAT_MIN_NUM)
									return PIVOT;
							}
							else if (!physMoveInMoveset && specMoveInMoveset)
							{
								if (STAT_STAGE(bankAtk, STAT_STAGE_SPATK) <= OFFENSIVE_STAT_MIN_NUM)
									return PIVOT;
							}
							else if (physMoveInMoveset && specMoveInMoveset)
							{
								if (STAT_STAGE_ATK <= OFFENSIVE_STAT_MIN_NUM && STAT_STAGE(bankAtk, STAT_STAGE_SPATK) <= OFFENSIVE_STAT_MIN_NUM)
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

static bool8 GoodIdeaToSwapVanillaTimers(u8 timerAtk, u8 timerDef, bool8 wantToKeep)
{
	if (wantToKeep)
		return timerAtk == 0 && timerDef != 0; //Steal it if foe has it but attacker doesn't
	else //Want to get rid of
		return timerDef == 0 && timerAtk != 0; //Give it away if attacker has it but foe doesn't
}

static bool8 GoodIdeaToSwapTimers(u8* timers, u8 sideAtk, u8 sideDef, bool8 wantToKeep)
{
	return GoodIdeaToSwapVanillaTimers(timers[sideAtk], timers[sideDef], wantToKeep);
}

bool8 ShouldCourtChange(u8 bankAtk, u8 bankDef)
{
	#define GOOD_IDEA(timer, wantToKeep) (GoodIdeaToSwapTimers(gNewBS->timer, sideAtk, sideDef, wantToKeep))
	#define GOOD_IDEA_VANILLA(timer, wantToKeep) (GoodIdeaToSwapVanillaTimers(gSideTimers[sideAtk].timer, gSideTimers[sideDef].timer, wantToKeep))

	u8 sideAtk = SIDE(bankAtk);
	u8 sideDef = SIDE(bankDef);

	return GOOD_IDEA(SeaOfFireTimers, FALSE)
		|| GOOD_IDEA(SwampTimers, FALSE)
		|| GOOD_IDEA(RainbowTimers, TRUE)
		|| GOOD_IDEA(LuckyChantTimers, TRUE)
		|| GOOD_IDEA(TailwindTimers, TRUE)
		|| GOOD_IDEA(AuroraVeilTimers, TRUE)
		|| GOOD_IDEA(maxVineLashTimers, FALSE)
		|| GOOD_IDEA(maxWildfireTimers, FALSE)
		|| GOOD_IDEA(maxCannonadeTimers, FALSE)
		|| GOOD_IDEA(maxVolcalithTimers, FALSE)
		|| GOOD_IDEA_VANILLA(reflectTimer, TRUE)
		|| GOOD_IDEA_VANILLA(lightscreenTimer, TRUE)
		|| GOOD_IDEA_VANILLA(safeguardTimer, TRUE)
		|| GOOD_IDEA_VANILLA(mistTimer, TRUE)
		|| GOOD_IDEA_VANILLA(spikesAmount, FALSE)
		|| GOOD_IDEA_VANILLA(srAmount, FALSE)
		|| GOOD_IDEA_VANILLA(stickyWeb, FALSE)
		|| GOOD_IDEA_VANILLA(steelsurge, FALSE);

	#undef GOOD_IDEA
	#undef GOOD_IDEA_VANILLA
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
			INCREASE_VIABILITY(3 + boost);
			break;

		case FIGHT_CLASS_TEAM_SUPPORT_BATON_PASS:
			if (boost >= 3)
				INCREASE_VIABILITY(1);
			break;

		case FIGHT_CLASS_TEAM_SUPPORT_CLERIC:
			INCREASE_VIABILITY(3 + boost);
			break;

		case FIGHT_CLASS_TEAM_SUPPORT_SCREENS:
		case FIGHT_CLASS_SWEEPER_SETUP_SCREENS:
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
		case FIGHT_CLASS_DOUBLES_TOTAL_TEAM_SUPPORT:
			INCREASE_VIABILITY(2 + boost);
			break;
	}

	*originalViability = MathMin(viability, 255);
}

static bool8 ShouldTryToSetUpStat(u8 bankAtk, u8 bankDef, u16 move, u8 stat, u8 statLimit)
{
	if (stat == 0xFF) //Steal boosts with Spectral Thief
		return !(AI_SpecialTypeCalc(move, bankAtk, bankDef) & MOVE_RESULT_NO_EFFECT); //Spectral Thief only steals stats when the move affects

	if (ABILITY(bankDef) == ABILITY_UNAWARE
	&& !MoveInMovesetAndUsable(MOVE_STOREDPOWER, bankAtk)
	&& !MoveInMovesetAndUsable(MOVE_POWERTRIP, bankAtk)
	&& !(stat == STAT_STAGE_DEF && MoveInMovesetAndUsable(MOVE_BODYPRESS, bankAtk)))
		return FALSE; //Don't set up if foe has Unaware

	if (WillFaintFromSecondaryDamage(bankAtk))
		return FALSE; //Don't set up if you're going to die
	else if (stat == 0xFE) //All stats
		return TRUE;

	if (IS_SINGLE_BATTLE)
	{
		if (MoveWouldHitFirst(move, bankAtk, bankDef)) //Setup move would go first
		{
			if (CanKnockOutWithChipDamage(bankDef, bankAtk))
				return FALSE; //Don't set up if enemy can KO you or get you down to 1 HP and you'll die from chip damage
			else if (BATTLER_SEMI_INVULNERABLE(bankDef))
				return TRUE; //Can't hit target anyways
		}
		else //Setup move would go second
		{
			if (IsMovePredictionSemiInvulnerable(bankDef, bankAtk))
				return TRUE; //Might as well if the enemy is going to hide first

			u16 foePrediction = IsValidMovePrediction(bankDef, bankAtk);
			if (stat == STAT_STAGE_SPEED && STAT_STAGE(bankAtk, stat) < statLimit //Trying to raise Speed
			&& !MoveKnocksOutXHits(foePrediction, bankDef, bankAtk, 1) //Opponent won't KO with its next move
			&& WillBeFasterAfterMoveSpeedBuff(bankAtk, bankDef, move))
				return TRUE; //Opponent goes first now,	but boosting speed will make you faster

			if (Can2HKO(bankDef, bankAtk)) //Foe can 2HKO AI
			{
				if (!CanKnockOut(bankDef, bankAtk)) //Will actually take 2 hits to KO
				{
					if (stat == STAT_STAGE_DEF)
					{
						if (CalcMoveSplit(GetStrongestMove(bankDef, bankAtk), bankDef, bankAtk) == SPLIT_PHYSICAL)
							return TRUE; //Maybe increasing defenses will reduce the chance of a 2HKO
					}
					else if (stat == STAT_STAGE_SPDEF)
					{
						if (CalcMoveSplit(GetStrongestMove(bankDef, bankAtk), bankDef, bankAtk) == SPLIT_SPECIAL)
							return TRUE; //Maybe increasing defenses will reduce the chance of a 2HKO
					}
				}

				return FALSE; //Don't set up now if you'll just die on the next turn
			}
		}

		if (STAT_STAGE(bankAtk, stat) >= statLimit)
			return FALSE;

		if ((stat == STAT_STAGE_ATK || stat == STAT_STAGE_SPATK) //Trying to boost offensive stat
		&& EnduresAHitFromFullHealth(bankDef, ABILITY(bankDef), ABILITY(bankAtk)) //The foe is going to live a hit with Sturdy/Focus Sash no matter how many times the AI sets up
		&& !MultiHitMoveWithSplitInMovesetThatAffects(bankAtk, bankDef, (stat == STAT_STAGE_ATK) ? SPLIT_PHYSICAL : SPLIT_SPECIAL) //The offensive boost will never be able to make a multi-hit move OHKO
		&& !IsBankIncapacitated(bankDef)) //The foe will be able to attack this turn
			return FALSE; //Prioritize breaking the endurance since you'll never be able to OHKO

		return TRUE;
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

static bool8 ShouldMaxStatBuffLimit(u8 bankAtk, u8 bankDef, u16 move, u8 stat)
{
	if (IS_SINGLE_BATTLE)
	{
		//Set up on non-threatening foe fighting style
		u8 defFightingStyle = GetBankFightingStyle(bankDef);
		if ((IsClassStall(defFightingStyle) || IsClassTeamSupport(defFightingStyle)) //Generally non-threatening
		&& (gLastUsedMoves[bankDef] == MOVE_NONE || gLastUsedMoves[bankDef] == 0xFFFF || SPLIT(gLastUsedMoves[bankDef]) == SPLIT_STATUS)) //Didn't try to damage last turn
		{
			if (stat == STAT_STAGE_ATK || stat == STAT_STAGE_SPATK)
				return TRUE; //Take advantage and set up more on the non-threatening mon
		}

		//Set up on foe predicted to switch out
		if (IsPredictedToSwitch(bankDef, bankAtk))
			return TRUE;
	}

	//Set up when Raid Boss
	if (IsRaidBattle()
	&& bankAtk == BANK_RAID_BOSS
	&& SPLIT(move) == SPLIT_STATUS //Inherent setup move, and not side effect
	&& GetRaidBattleRepeatedAttackChance(bankAtk) >= 50 //50% chance of attacking again
	&& (HasRaidShields(bankAtk) || GetNextRaidShieldHP(bankAtk) > 0)) //Shields need to be broken before the Raid Boss will faint
		return TRUE; //Raid Boss should use the fact that it'll set up multiple times to its advantage

	return FALSE;
}

void IncreaseStatViability(s16* originalViability, u8 class, u8 boost, u8 bankAtk, u8 bankDef, u16 move, u8 stat, u8 statLimit)
{
	s16 viability = *originalViability;

	if (statLimit < STAT_STAGE_MAX && ShouldMaxStatBuffLimit(bankAtk, bankDef, move, stat))
		statLimit = STAT_STAGE_MAX; //Try to boost until stats can't go any higher

	switch (class) {
		case FIGHT_CLASS_SWEEPER_KILL:
			if (stat == 0xFF //Spectral Thief
			|| ShouldTryToSetUpStat(bankAtk, bankDef, move, stat, statLimit))
				INCREASE_VIABILITY(3 + boost);
			else if (SPLIT(move) != SPLIT_STATUS
			&& (stat == STAT_STAGE_ATK //For moves like Power-Up Punch
			 || stat == STAT_STAGE_SPATK))
				INCREASE_VIABILITY(1); //Slight boost, but not much since it's not a good idea
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
					if (ShouldTryToSetUpStat(bankAtk, bankDef, move, stat, 6 + 4))
						INCREASE_VIABILITY(5);
					break;

				case STAT_STAGE_DEF:
				case STAT_STAGE_SPDEF:
					if (ShouldTryToSetUpStat(bankAtk, bankDef, move, stat, 6 + 4))
						INCREASE_VIABILITY(4);
					break;

				//Evasiveness Stat Booster
				case STAT_STAGE_EVASION:
					if (ShouldTryToSetUpStat(bankAtk, bankDef, move, stat, STAT_STAGE_MAX))
						INCREASE_VIABILITY(6);
					break;
			}
			break;

		case FIGHT_CLASS_TEAM_SUPPORT_CLERIC:
			if (ShouldTryToSetUpStat(bankAtk, bankDef, move, stat, statLimit))
				INCREASE_STATUS_VIABILITY(1); //Treat like a low-priority status move
			break;

		case FIGHT_CLASS_TEAM_SUPPORT_SCREENS:
		case FIGHT_CLASS_SWEEPER_SETUP_SCREENS:
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
			if (stat == 0xFE && ShouldTryToSetUpStat(bankAtk, bankDef, move, stat, statLimit)) //All
				INCREASE_VIABILITY(16);
			break;

		case FIGHT_CLASS_DOUBLES_SETUP_ATTACKER:
			switch (stat) {
				//Offsensive Stat Booster
				case STAT_STAGE_ATK:
				case STAT_STAGE_SPATK:
				case STAT_STAGE_SPEED:
				case STAT_STAGE_ACC:
				case STAT_STAGE_EVASION:
				case 0xFE: //All
				case 0xFF: //Spectral Thief
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
		case FIGHT_CLASS_DOUBLES_TOTAL_TEAM_SUPPORT:
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

	bool8 spreadSleep = IS_DOUBLE_BATTLE
					 && BATTLER_ALIVE(PARTNER(bankDef))
					 && GetBaseMoveTarget(move, bankAtk) & (MOVE_TARGET_BOTH | MOVE_TARGET_ALL)
					 && !MoveBlockedBySubstitute(move, bankAtk, PARTNER(bankDef))
					 && !BadIdeaToPutToSleep(PARTNER(bankDef), bankAtk);

	//Check if Yawn shouldn't be used
	if (gBattleMoves[move].effect == EFFECT_YAWN)
	{
		if (CanKnockOut(bankAtk, bankDef) //Could KO the foe this turn
		&& !IsMovePredictionHealingMove(bankDef, bankAtk) //And the foe isn't going to try to heal
		&& !IsPredictedToSwitch(bankDef, bankAtk)) //And the foe's going to stay in
			return; //Just KO the foe, don't waste the turn

		if (ABILITY(bankDef) != ABILITY_TRUANT
		&& ViableMonCountFromBank(bankAtk) == 1 //Yawner is the last mon
		&& !HasProtectionMoveInMoveset(bankAtk, CHECK_REGULAR_PROTECTION)) //Can't protect from the follow up attack after Yawn
		{
			if (MoveWouldHitFirst(move, bankAtk, bankDef)) //Yawn would go first
			{
				if (CanKnockOut(bankDef, bankAtk)) //Opponent can KO right after being hit with Yawn
					return; //Don't waste your last turn alive using Yawn

				if (Can2HKO(bankDef, bankAtk) && !HealingMoveInMoveset(bankAtk)) //Opponent can KO before Yawn takes effect and attacker can't stall until it does
					return; //Don't waste your second last turn alive using Yawn
			}
			else //Yawn would go second
			{
				if (Can2HKO(bankDef, bankAtk)) //Opponent can KO right after being hit with Yawn
					return; //Don't waste your last turn alive using Yawn
			}
		}
	}

	switch (class) {
		case FIGHT_CLASS_SWEEPER_KILL:
			break;

		case FIGHT_CLASS_SWEEPER_SETUP_STATS:
			if (AccuracyCalc(move, bankAtk, bankDef) >= 80) //Decent chance of hitting
				INCREASE_VIABILITY(9);
			else
				INCREASE_STATUS_VIABILITY(3);
			break;

		case FIGHT_CLASS_SWEEPER_SETUP_STATUS:
			if (AccuracyCalc(move, bankAtk, bankDef) >= 80) //Decent chance of hitting
				INCREASE_VIABILITY(8);
			else
				INCREASE_STATUS_VIABILITY(3);
			break;

		case FIGHT_CLASS_STALL:
			INCREASE_STATUS_VIABILITY(3);
			break;

		case FIGHT_CLASS_TEAM_SUPPORT_BATON_PASS:
			if (AccuracyCalc(move, bankAtk, bankDef) >= 80) //Decent chance of hitting
				INCREASE_VIABILITY(9);
			else
				INCREASE_STATUS_VIABILITY(3);
			break;

		case FIGHT_CLASS_TEAM_SUPPORT_CLERIC:
			if (AccuracyCalc(move, bankAtk, bankDef) >= 80) //Decent chance of hitting
				INCREASE_VIABILITY(7);
			else
				INCREASE_STATUS_VIABILITY(3);
			break;

		case FIGHT_CLASS_TEAM_SUPPORT_SCREENS:
		case FIGHT_CLASS_SWEEPER_SETUP_SCREENS:
			if (AccuracyCalc(move, bankAtk, bankDef) >= 80) //Decent chance of hitting
				INCREASE_VIABILITY(8);
			else
				INCREASE_STATUS_VIABILITY(3);
			break;

		case FIGHT_CLASS_TEAM_SUPPORT_PHAZING:
			if (AccuracyCalc(move, bankAtk, bankDef) >= 80) //Decent chance of hitting
				INCREASE_VIABILITY(9);
			else
				INCREASE_STATUS_VIABILITY(3);
			break;

		case FIGHT_CLASS_ENTRY_HAZARDS:
			if (AccuracyCalc(move, bankAtk, bankDef) >= 80) //Decent chance of hitting
				INCREASE_VIABILITY(8);
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
		case FIGHT_CLASS_DOUBLES_TOTAL_TEAM_SUPPORT:
			if (spreadSleep)
				INCREASE_VIABILITY(19);
			else
				INCREASE_STATUS_VIABILITY(3);
			break;
	}

	*originalViability = MathMin(viability, 255);
}

void IncreaseFreezeViability(s16* originalViability, u8 class, u8 bankAtk, u8 bankDef)
{
	s16 viability = *originalViability;

	if (BadIdeaToFreeze(bankDef, bankAtk))
		return;

	switch (class) {
		case FIGHT_CLASS_SWEEPER_KILL:
			break;

		case FIGHT_CLASS_SWEEPER_SETUP_STATS:
			INCREASE_VIABILITY(9);
			break;

		case FIGHT_CLASS_SWEEPER_SETUP_STATUS:
			INCREASE_VIABILITY(8);
			break;

		case FIGHT_CLASS_STALL:
			INCREASE_VIABILITY(8);
			break;

		case FIGHT_CLASS_TEAM_SUPPORT_BATON_PASS:
			INCREASE_VIABILITY(9);
			break;

		case FIGHT_CLASS_TEAM_SUPPORT_CLERIC:
			INCREASE_VIABILITY(7);
			break;

		case FIGHT_CLASS_TEAM_SUPPORT_SCREENS:
		case FIGHT_CLASS_SWEEPER_SETUP_SCREENS:
			INCREASE_VIABILITY(8);
			break;

		case FIGHT_CLASS_TEAM_SUPPORT_PHAZING:
			INCREASE_VIABILITY(9);
			break;

		case FIGHT_CLASS_ENTRY_HAZARDS:
			INCREASE_VIABILITY(8);
			break;

		case FIGHT_CLASS_DOUBLES_ALL_OUT_ATTACKER:
			INCREASE_VIABILITY(17);
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
			INCREASE_VIABILITY(17);
			break;

		case FIGHT_CLASS_DOUBLES_PHAZING:
			INCREASE_VIABILITY(17);
			break;

		case FIGHT_CLASS_DOUBLES_TEAM_SUPPORT:
		case FIGHT_CLASS_DOUBLES_TOTAL_TEAM_SUPPORT:
			INCREASE_VIABILITY(19);
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
			case FIGHT_CLASS_SWEEPER_SETUP_SCREENS:
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
			case FIGHT_CLASS_DOUBLES_TOTAL_TEAM_SUPPORT:
				if (ShouldUseSubstitute(bankAtk, bankDef))
					INCREASE_STATUS_VIABILITY(1); //Treat like a low-priority status move
				break;
		}
	}

	*originalViability = MathMin(viability, 255);
}

static bool8 IsWorthSettingHazards(u8 bankDef)
{
	u8 monsLeft = ViableMonCountFromBankLoadPartyRange(bankDef);
	
	if (monsLeft == 2) //Foe only two mons left
		return FALSE; //Don't bother setting hazards

	if (monsLeft == 3 //Opponent only has three Pokemon alive on team
	&& gBattleTypeFlags & BATTLE_TYPE_FRONTIER //And it's a Frontier battle (so likely 3v3)
	&& Random() % 100 < 66) //Don't use hazards 66% of the time 
		return FALSE;

	return TRUE;
}

void IncreaseEntryHazardsViability(s16* originalViability, u8 class, u8 bankAtk, u8 bankDef, u16 move)
{
	s16 viability = *originalViability;

	if (HasUsedMoveWithEffect(bankDef, EFFECT_MAGIC_COAT))
		return; //Don't use Hazards if the player is going to try to cheese with Magic Coat spam

	switch (class) {
		case FIGHT_CLASS_SWEEPER_KILL:
			break;

		case FIGHT_CLASS_SWEEPER_SETUP_STATS:
			if (!IsWorthSettingHazards(bankDef))
				break; //Don't bother
			else
				INCREASE_STATUS_VIABILITY(2); //Treat like a middle-priority status move
			break;

		case FIGHT_CLASS_SWEEPER_SETUP_STATUS:
			if (!IsWorthSettingHazards(bankDef))
				break; //Don't bother
			else
				INCREASE_STATUS_VIABILITY(2); //Treat like a middle-priority status move
			break;

		case FIGHT_CLASS_STALL:
			if (!IsWorthSettingHazards(bankDef))
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
			if (!IsWorthSettingHazards(bankDef))
				INCREASE_STATUS_VIABILITY(1); //Treat like a low-priority status move
			else
				INCREASE_STATUS_VIABILITY(2); //Treat like a middle-priority status move
			break;

		case FIGHT_CLASS_TEAM_SUPPORT_SCREENS:
		case FIGHT_CLASS_SWEEPER_SETUP_SCREENS:
			if (!IsWorthSettingHazards(bankDef))
				INCREASE_STATUS_VIABILITY(1); //Treat like a low-priority status move
			else
				INCREASE_STATUS_VIABILITY(2); //Treat like a middle-priority status move
			break;

		case FIGHT_CLASS_TEAM_SUPPORT_PHAZING:
			if (((move == MOVE_SPIKES && gSideTimers[SIDE(bankDef)].spikesAmount > 0) //Trying to set up more Spikes
			  || (move == MOVE_TOXICSPIKES && gSideTimers[SIDE(bankDef)].tspikesAmount > 0)) //or trying to set up more Toxic Spikes
			&& ((!gSideTimers[SIDE(bankDef)].srAmount && MoveInMoveset(MOVE_STEALTHROCK, bankAtk)) //But can set up Stealth Rock too
			 || (!gSideTimers[SIDE(bankDef)].stickyWeb && MoveInMoveset(MOVE_STICKYWEB, bankAtk)))) //Or can set up Sticky Web too
			{
				//Use less of the time to allow SR to get set up
				if (ViableMonCountFromBankLoadPartyRange(bankDef) == 2) //Only two mons left
					INCREASE_STATUS_VIABILITY(1); //Treat like a low-priority status move
				else
					INCREASE_STATUS_VIABILITY(2);
			}
			else if (ViableMonCountFromBankLoadPartyRange(bankDef) == 2) //Only two mons left
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
		case FIGHT_CLASS_DOUBLES_TOTAL_TEAM_SUPPORT:
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
	u8 decrement = 0;
	s16 viability = *originalViability;

	if (move == MOVE_FAKEOUT && IS_DOUBLE_BATTLE)
	{
		u8 partner = PARTNER(bankAtk);
		u16 partnerMove = GetAIChosenMove(partner, bankDef);

		if (BATTLER_ALIVE(partner)
		&&  partnerMove == MOVE_FAKEOUT
		&&  gBattleStruct->moveTarget[partner] == bankDef)
			return; //No benefit to using Fake Out twice on the same opponent

		//Try to hit the more threatening target with Fake Out
		u8 bankAtkPartner = PARTNER(bankAtk);
		u8 bankDefPartner = PARTNER(bankDef);

		if (BATTLER_ALIVE(bankDef) && BATTLER_ALIVE(bankDefPartner) //2 opponents
		&& !(BATTLER_ALIVE(partner) && partnerMove == MOVE_FAKEOUT && gBattleStruct->moveTarget[partner] == bankDefPartner)) //And partner isn't targeting second opponent
		{
			//Number of AI mons potentional target could KO
			u8 knockOutCount1 = CanKnockOut(bankDef, bankAtk) ? 1 : 0;
			   knockOutCount1 += BATTLER_ALIVE(bankAtkPartner) && CanKnockOut(bankDef, bankAtkPartner) ? 1 : 0;

			//Number of AI mons potential target could KO
			u8 knockOutCount2 = CanKnockOut(bankDefPartner, bankAtk) ? 1 : 0;
			   knockOutCount2 += BATTLER_ALIVE(bankAtkPartner) && CanKnockOut(bankDefPartner, bankAtkPartner) ? 1 : 0;

			if (knockOutCount2 > knockOutCount1 //Target's partner is more threatening
			&& !(AI_SpecialTypeCalc(move, bankAtk, bankDef) & MOVE_RESULT_NO_EFFECT)) //And it can be hit with Fake Out
				decrement = 1; //Make it slightly less of a good option to hit this target with Fake Out
		}
	}

	switch (class) {
		case FIGHT_CLASS_DOUBLES_ALL_OUT_ATTACKER:
			INCREASE_VIABILITY(15 - decrement);
			break;

		case FIGHT_CLASS_DOUBLES_SETUP_ATTACKER:
			INCREASE_VIABILITY(15 - decrement);
			break;

		case FIGHT_CLASS_DOUBLES_TRICK_ROOM_ATTACKER:
			if (!IsTrickRoomActive()) //Don't burn a Trick Room turn to use Fake Out
				INCREASE_VIABILITY(17 - decrement);
			break;

		case FIGHT_CLASS_DOUBLES_TRICK_ROOM_SETUP:
			INCREASE_VIABILITY(17 - decrement);
			break;

		case FIGHT_CLASS_DOUBLES_UTILITY:
			INCREASE_VIABILITY(19 - decrement);
			break;

		case FIGHT_CLASS_DOUBLES_PHAZING:
			INCREASE_VIABILITY(19 - decrement);
			break;

		case FIGHT_CLASS_DOUBLES_TEAM_SUPPORT:
		case FIGHT_CLASS_DOUBLES_TOTAL_TEAM_SUPPORT:
			INCREASE_VIABILITY(17 - decrement);
			break;

		default:
			INCREASE_VIABILITY(8 - decrement);
	}

	*originalViability = MathMin(viability, 255);
}

void IncreasePivotViability(s16* originalViability, u8 class, u8 bankAtk, unusedArg u8 bankDef, u8 pivotType)
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

	if (pivotType == PIVOT_IMMEDIATELY) //Set by the switching logic to pivot instead of actually switching
	{
		if (IsClassDoublesSpecific(pivotType))
			INCREASE_VIABILITY(19);
		else
			INCREASE_VIABILITY(9);
	}
	else
	{
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
			case FIGHT_CLASS_SWEEPER_SETUP_SCREENS:
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
			case FIGHT_CLASS_DOUBLES_TOTAL_TEAM_SUPPORT:
				INCREASE_VIABILITY(15);
				break;
		}
	}

	*originalViability = MathMin(viability, 255);
}

static bool8 BetterToKOLastFoeMon(u8 bankAtk, u8 bankDef) //Prerequisite being bankAtk is slower than bankDef, and bankAtk's considered move KOs bankDef
{
	if ((CanKnockOut(bankDef, bankAtk) || Can2HKO(bankDef, bankAtk)) //Foe will KO soon so attacker will be able to get max 1 attack off before then
	&& ViableMonCountFromBank(bankDef) <= 1) //But it's also the foe's last mon
		return TRUE;

	return FALSE;
}

//Only for single battles
void IncreaseViabilityForSlowKOMove(s16* originalViability, u8 class, u8 bankAtk, u8 bankDef)
{
	s16 viability = *originalViability;

	if (class == 0xFF) //Dumb AI
		class = FIGHT_CLASS_SWEEPER_KILL;

	switch (class) {
		case FIGHT_CLASS_SWEEPER_KILL:
			INCREASE_VIABILITY(8);
			break;

		case FIGHT_CLASS_SWEEPER_SETUP_STATS:
			if (BetterToKOLastFoeMon(bankAtk, bankDef))
				INCREASE_VIABILITY(8);
			else
				INCREASE_VIABILITY(6);
			break;

		case FIGHT_CLASS_SWEEPER_SETUP_STATUS:
			INCREASE_VIABILITY(8);
			break;

		case FIGHT_CLASS_STALL:
			if (BetterToKOLastFoeMon(bankAtk, bankDef))
				INCREASE_VIABILITY(9); //Assumed it's the only damaging move and no priority move will also have a 9
			else
				INCREASE_VIABILITY(8);
			break;

		case FIGHT_CLASS_TEAM_SUPPORT_BATON_PASS:
			if (BetterToKOLastFoeMon(bankAtk, bankDef))
				INCREASE_VIABILITY(9); //Assumed it's the only damaging move and no priority move will also have a 9
			else if (!Can2HKO(bankDef, bankAtk))
				INCREASE_VIABILITY(3);
			else
				INCREASE_VIABILITY(8);
			break;

		case FIGHT_CLASS_TEAM_SUPPORT_CLERIC:
			if (BetterToKOLastFoeMon(bankAtk, bankDef))
				INCREASE_VIABILITY(9); //Assumed it's the only damaging move and no priority move will also have a 9
			else if (!Can2HKO(bankDef, bankAtk))
				INCREASE_VIABILITY(6);
			else
				INCREASE_VIABILITY(7);
			break;

		case FIGHT_CLASS_TEAM_SUPPORT_SCREENS:
		case FIGHT_CLASS_SWEEPER_SETUP_SCREENS:
			if (BetterToKOLastFoeMon(bankAtk, bankDef))
				INCREASE_VIABILITY(9); //Assumed it's the only damaging move and no priority move will also have a 9
			else if (!Can2HKO(bankDef, bankAtk))
				INCREASE_VIABILITY(6); //Get the screens up first
			else
				INCREASE_VIABILITY(9); //KO now before you die
			break;

		case FIGHT_CLASS_TEAM_SUPPORT_PHAZING:
			if (BetterToKOLastFoeMon(bankAtk, bankDef))
				INCREASE_VIABILITY(9); //Assumed it's the only damaging move and no priority move will also have a 9
			else
				INCREASE_VIABILITY(8);
			break;

		case FIGHT_CLASS_ENTRY_HAZARDS:
			if (BetterToKOLastFoeMon(bankAtk, bankDef))
				INCREASE_VIABILITY(9); //Assumed it's the only damaging move and no priority move will also have a 9
			else if (!Can2HKO(bankDef, bankAtk))
				INCREASE_VIABILITY(3);
			else
				INCREASE_VIABILITY(6);
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
			if (IsTrapped(bankDef, TRUE))
				INCREASE_VIABILITY(7); //Not like the foe can go anywhere
			else
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
		case FIGHT_CLASS_DOUBLES_TOTAL_TEAM_SUPPORT:
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
		case FIGHT_CLASS_DOUBLES_TOTAL_TEAM_SUPPORT:
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
		case FIGHT_CLASS_DOUBLES_TOTAL_TEAM_SUPPORT:
			INCREASE_VIABILITY(12);
			break;
	}

	*originalViability = MathMin(viability, 255);
}

void IncreaseTailwindViability(s16* originalViability, u8 class, u8 bankAtk, u8 bankDef)
{
	s16 viability = *originalViability;

	switch (class) {
		case FIGHT_CLASS_SWEEPER_SETUP_SCREENS:
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
		case FIGHT_CLASS_DOUBLES_TOTAL_TEAM_SUPPORT:
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
		case FIGHT_CLASS_DOUBLES_TOTAL_TEAM_SUPPORT:
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

	if (gChosenActionByBank[partner] == ACTION_USE_ITEM) //Item is being used on partner
	{
		u16 item = gBattleBufferB[partner][1] | (gBattleBufferB[partner][2] << 8);
		u8 itemType = GetAI_ItemType(item, gItemEffectTable[item - ITEM_POTION]);

		if (itemType == AI_ITEM_FULL_RESTORE || itemType == AI_ITEM_HEAL_HP)
			return; //Don't heal with move when item is already being used to heal
	}

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
		case FIGHT_CLASS_DOUBLES_TOTAL_TEAM_SUPPORT:
			INCREASE_VIABILITY(13);
			break;
	}

	*originalViability = MathMin(viability, 255);
}

void IncreasePsychUpViability(s16* originalViability, u8 class, u8 bankAtk, u8 bankDef)
{
	if (GoodIdeaToSwapStatStages(bankAtk, bankDef))
	{
		u8 statsToCheck[] = {STAT_STAGE_ATK, STAT_STAGE_SPATK, STAT_STAGE_SPEED,
							 STAT_STAGE_ACC, STAT_STAGE_EVASION, STAT_STAGE_DEF,
							 STAT_STAGE_SPDEF}; //Priotitize checking offensive stats first
		s16 viability = *originalViability;
		u16 move = MOVE_PSYCHUP; //Needed for INCREASE_STAT_VIABILITY

		//Increase the viability by the stat that's going to be raised
		for (u32 i = 0; i < NELEMS(statsToCheck); ++i)
		{
			u8 stat = statsToCheck[i];
			u8 defStatStage = STAT_STAGE(bankDef, stat);
			u8 atkStatStage =  STAT_STAGE(bankAtk, stat);
			s8 statStageDifference = defStatStage - atkStatStage;
		
			if (statStageDifference > 0) //Partner's stat is higher
			{
				if (stat == STAT_STAGE_ATK && RealPhysicalMoveInMoveset(bankAtk))
				{
					INCREASE_STAT_VIABILITY(viability, 8, statStageDifference);
					break;
				}
				else if (stat == STAT_STAGE_SPATK && SpecialMoveInMoveset(bankAtk))
				{
					INCREASE_STAT_VIABILITY(stat, 8, statStageDifference);
					break;
				}
				else if (stat == STAT_STAGE_ACC || stat == STAT_STAGE_EVASION || stat == STAT_STAGE_SPEED)
				{
					INCREASE_STAT_VIABILITY(stat, 8, statStageDifference);
					break;
				}
				else if (IsClassStall(class)) //Defense and Sp. Defense
				{
					INCREASE_STAT_VIABILITY(stat, 8, statStageDifference);
					break;
				}
			}
		}
		
		*originalViability = MathMin(viability, 255);
	}
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
			case FIGHT_CLASS_DOUBLES_TOTAL_TEAM_SUPPORT:
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
