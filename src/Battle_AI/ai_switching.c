#include "../defines.h"
#include "../defines_battle.h"
#include "../../include/constants/region_map_sections.h"

#include "../../include/new/ai_advanced.h"
#include "../../include/new/ai_util.h"
#include "../../include/new/ai_master.h"
#include "../../include/new/ai_switching.h"
#include "../../include/new/battle_start_turn_start.h"
#include "../../include/new/battle_util.h"
#include "../../include/new/dynamax.h"
#include "../../include/new/end_turn_battle_scripts.h"
#include "../../include/new/general_bs_commands.h"
#include "../../include/new/multi.h"
#include "../../include/new/move_tables.h"
#include "../../include/new/switching.h"
#include "../../include/new/util.h"

/*
ai_switching.c
	AI logic for switching out.
*/

//TODO: Touch up switching logic in doubles

//This file's functions:
static bool8 ShouldSwitchIfOnlyBadMovesLeft(struct Pokemon* party);
static bool8 FindMonThatAbsorbsOpponentsMove(struct Pokemon* party, u8 firstId, u8 lastId);
static bool8 SwitchToBestResistMon(const struct Pokemon* party, bool8 willPivot, u8 foe);
static bool8 ShouldSwitchIfNaturalCureOrRegenerator(struct Pokemon* party);
static bool8 PassOnWish(struct Pokemon* party, u8 firstId, u8 lastId);
static bool8 SemiInvulnerableTroll(void);
static u8 GetBestPartyNumberForSemiInvulnerableLockedMoveCalcs(u8 opposingBattler1, u8 opposingBattler2, bool8 checkLockedMoves);
static bool8 RunAllSemiInvulnerableLockedMoveCalcs(u8 opposingBattler1, u8 opposingBattler2, bool8 checkLockedMoves);
static bool8 TheCalcForSemiInvulnerableTroll(u8 bankAtk, u8 flags, bool8 checkLockedMoves);
static bool8 CanStopLockedMove(void);
static bool8 ShouldSwitchWhenYawned(void);
static bool8 ShouldSwitchWhileAsleep(struct Pokemon* party);
static bool8 IsTakingAnnoyingSecondaryDamage(struct Pokemon* party);
static bool8 ShouldSwitchToAvoidDeath(struct Pokemon* party);
static bool8 ShouldSwitchIfWonderGuard(struct Pokemon* party, u8 firstId, u8 lastId);
static bool8 ShouldSwitchWhenOffensiveStatsAreLow(struct Pokemon* party);
static bool8 ShouldSaveSweeperForLater(struct Pokemon* party);
static void CalcMostSuitableMonSwitchIfNecessary(void);
#ifdef VAR_GAME_DIFFICULTY
static bool8 IsGoodIdeaToDoShiftSwitch(u8 aiBank, u8 foe);
#endif
extern bool8 AISaveSweeperForLaterDifficultyCheck(void);

bool8 ShouldSwitch(struct Pokemon* party, u8 firstId, u8 lastId)
{
	u8 battlerIn1, battlerIn2;
	u8 availableToSwitch;
	u32 i;

	if (IsTrapped(gActiveBattler, TRUE)
	|| gStatuses3[gActiveBattler] & (STATUS3_SKY_DROP_ATTACKER | STATUS3_SKY_DROP_TARGET)) //Can never switch out when in the air
		return FALSE;

	availableToSwitch = 0;
	if (IS_DOUBLE_BATTLE)
	{
		battlerIn1 = gActiveBattler;
		if (gAbsentBattlerFlags & gBitTable[GetBattlerAtPosition(GetBattlerPosition(gActiveBattler) ^ BIT_FLANK)])
			battlerIn2 = gActiveBattler;
		else
			battlerIn2 = GetBattlerAtPosition(GetBattlerPosition(gActiveBattler) ^ BIT_FLANK);
	}
	else
	{
		battlerIn1 = gActiveBattler;
		battlerIn2 = gActiveBattler;
	}

	for (i = firstId; i < lastId; ++i)
	{
		u16 species = GetMonData(&party[i], MON_DATA_SPECIES2, 0);

		if (SPECIES_CANT_BATTLE(species)
		|| 	party[i].hp == 0
		||	i == gBattlerPartyIndexes[battlerIn1]
		||	i == gBattlerPartyIndexes[battlerIn2]
		||	i == gBattleStruct->monToSwitchIntoId[battlerIn1]
		||  i == gBattleStruct->monToSwitchIntoId[battlerIn2])
			continue;

		++availableToSwitch;
	}

	if (availableToSwitch == 0)
		return FALSE;
	if (FindMonThatAbsorbsOpponentsMove(party, firstId, lastId)) //First check if can absorb move
		return TRUE;
	if (PassOnWish(party, firstId, lastId)) //Then try to pass on a wish
		return TRUE;
	if (CanStopLockedMove()) //Then try to stop a locked move
		return TRUE;
	if (ShouldSwitchIfPerishSong()) //Then proceed with the other checks as normal
		return TRUE;
	if (ShouldSwitchIfWonderGuard(party, firstId, lastId))
		return TRUE;
	if (ShouldSwitchIfOnlyBadMovesLeft(party))
		return TRUE;
	if (ShouldSwitchIfNaturalCureOrRegenerator(party))
		return TRUE;
	if (SemiInvulnerableTroll())
		return TRUE;
	if (ShouldSwitchWhenYawned())
		return TRUE;
	if (ShouldSwitchWhileAsleep(party))
		return TRUE;
	if (IsTakingAnnoyingSecondaryDamage(party))
		return TRUE;
	if (ShouldSwitchToAvoidDeath(party))
		return TRUE;
	if (ShouldSwitchWhenOffensiveStatsAreLow(party))
		return TRUE;
	if (ShouldSaveSweeperForLater(party))
		return TRUE;

	return FALSE;
}

void LoadBattlersAndFoes(u8* battlerIn1, u8* battlerIn2, u8* foe1, u8* foe2)
{
	if (IS_DOUBLE_BATTLE)
	{
		*battlerIn1 = gActiveBattler;
		if (gAbsentBattlerFlags & gBitTable[PARTNER(gActiveBattler)])
			*battlerIn2 = gActiveBattler;
		else
			*battlerIn2 = PARTNER(gActiveBattler);

		if (gAbsentBattlerFlags & gBitTable[FOE(gActiveBattler)])
			*foe1 = *foe2 = PARTNER(FOE(gActiveBattler));
		else if (gAbsentBattlerFlags & gBitTable[PARTNER(FOE(gActiveBattler))])
			*foe1 = *foe2 = FOE(gActiveBattler);
		else
		{
			*foe1 = FOE(gActiveBattler);
			*foe2 = PARTNER(FOE(gActiveBattler));
		}
	}
	else
	{
		*battlerIn1 = gActiveBattler;
		*battlerIn2 = gActiveBattler;
		*foe1 = FOE(gActiveBattler);
		*foe2 = *foe1;
	}
}

static void ConfirmAISwitch(u8 monId, bool8 willPivot)
{
	//Switch to a mon that can KO and save your damager for later
	if (!willPivot) //U-Turn/Volt Switch switch on their own
	{
		gBattleStruct->switchoutIndex[SIDE(gActiveBattler)] = monId;
		EmitTwoReturnValues(1, ACTION_SWITCH, 0);
	}
	else
	{
		gNewBS->ai.pivotTo[gActiveBattler] = monId;
		gNewBS->ai.goodToPivot |= gBitTable[gActiveBattler];
	}
}

static bool8 PredictedMoveWontDoTooMuchToMon(u8 activeBattler, struct Pokemon* mon, u8 foe, u8 switchFlags)
{
	u16 defMove = IsValidMovePrediction(foe, activeBattler);

	//Check if a switch in general is safe
	if (switchFlags & SWITCHING_FLAG_FAINTS_FROM_FOE)
		return FALSE; //Don't switch and sack your other mon

	//Check if a switch against the predicted move is safe
	if (defMove == MOVE_NONE || SPLIT(defMove) == SPLIT_STATUS //Predicted move won't do damage
	|| defMove == MOVE_PURSUIT) //Predicted move won't hit the incoming Pokemon
		return TRUE;

	//Check if a switch is worth it at all in Benjamin Butterfree
	if (gBattleTypeFlags & BATTLE_TYPE_BENJAMIN_BUTTERFREE
	&&  gBattleMons[activeBattler].moves[0] == MOVE_NONE) //Mon to be switched out has no moves left
		return FALSE; //Don't switch out and just struggle until you die

	//Check if the switch is a perfect wall
	if (switchFlags & SWITCHING_FLAG_RESIST_ALL_MOVES)
		return TRUE; //Can definitely take multiple hits

	if (gBattleMoves[defMove].effect == EFFECT_SUCKER_PUNCH)
		return TRUE; //Sucker Punch never works on a switch

	//Now run actual damage calc
	u32 predictedDmg = 0;
	u8 monAbility = GetMonAbility(mon);
	if (IsAffectedByDisguse(monAbility, mon->species, CalcMoveSplit(defMove, foe, activeBattler)))
	{
		if (monAbility == ABILITY_DISGUISE) //Disguise only - no Ice Face
			predictedDmg = mon->maxHP / 8; //Loses an 1/8 of max HP when the disguise is busted
	}
	else
		predictedDmg = (defMove == MOVE_NONE) ? 0 : AI_CalcMonDefDmg(foe, activeBattler, defMove, mon, NULL) + GetMonEntryHazardDamage(mon, SIDE(activeBattler));

	if (predictedDmg >= mon->hp)
		return FALSE; //Don't switch and sack your other mon

	if (predictedDmg * 2 < mon->hp)
		return TRUE; //Can take multiple hits

	return predictedDmg * 2 >= mon->maxHP //Will take a sizeable chunk of damage on the switch
		&& HealingMoveInMonMoveset(mon) //But can heal off that damage
		&& SpeedCalcMon(SIDE(activeBattler), mon) > SpeedCalc(foe); //And will get the healing move off first
}

static bool8 PredictedMoveWontKOMon(u8 activeBattler, struct Pokemon* mon, u8 foe)
{
	u16 defMove = IsValidMovePrediction(foe, activeBattler);

	//Check if a switch against the predicted move is safe
	if (defMove == MOVE_NONE || SPLIT(defMove) == SPLIT_STATUS) //Predicted move won't do damage
		return TRUE;

	if (gBattleMoves[defMove].effect == EFFECT_SUCKER_PUNCH)
		return TRUE; //Sucker Punch never works on a switch

	//Now run actual damage calc
	u32 predictedDmg = 0;
	u8 monAbility = GetMonAbility(mon);
	if (IsAffectedByDisguse(monAbility, mon->species, CalcMoveSplit(defMove, foe, activeBattler)))
	{
		if (monAbility == ABILITY_DISGUISE) //Disguise only - no Ice Face
			predictedDmg = mon->maxHP / 8;
	}
	else
		predictedDmg = (defMove == MOVE_NONE) ? 0 : AI_CalcMonDefDmg(foe, activeBattler, defMove, mon, NULL) + GetMonEntryHazardDamage(mon, SIDE(activeBattler));

	if (predictedDmg >= mon->hp)
		return FALSE; //Don't switch and sack your other mon

	return TRUE;
}

static bool8 PredictedMoveKOsSelfDueToContact(u8 activeBattler, struct Pokemon* mon, u8 foe)
{	
	u16 defMove = IsValidMovePrediction(foe, activeBattler);

	if (CheckContact(defMove, foe, activeBattler))
	{
		u32 contactDamage = GetContactDamageMonDef(foe, mon);
		return contactDamage >= gBattleMons[foe].hp;
	}

	return FALSE;
}

static bool8 ShouldSwitchIfOnlyBadMovesLeft(struct Pokemon* party)
{
	u8 battlerIn1, battlerIn2;
	u8 foe1, foe2;
	LoadBattlersAndFoes(&battlerIn1, &battlerIn2, &foe1, &foe2);
	
	if (gNewBS->ai.switchingCooldown[gActiveBattler]) //Just switched in
		return FALSE;

	if (IS_DOUBLE_BATTLE)
	{
		if (gBattleMons[gActiveBattler].status2 & STATUS2_DESTINY_BOND)
			return FALSE; //Don't switch at all because both foes could combine attacks to KO - any number of things could happen

		if ((!BATTLER_ALIVE(foe1) || OnlyBadMovesLeftInMoveset(gActiveBattler, foe1))
		&&  (!BATTLER_ALIVE(foe2) || OnlyBadMovesLeftInMoveset(gActiveBattler, foe2)))
		{
			gBattleStruct->switchoutIndex[SIDE(gActiveBattler)] = PARTY_SIZE;
			EmitTwoReturnValues(1, ACTION_SWITCH, 0);
			return TRUE;
		}
	}
	else
	{
		u16 foePredictedMove = IsValidMovePrediction(foe1, gActiveBattler);

		if (gBattleMons[gActiveBattler].status2 & STATUS2_DESTINY_BOND
		&& MoveKnocksOutXHits(foePredictedMove, foe1, gActiveBattler, 1)
		&& MoveWouldHitFirst(foePredictedMove, foe1, gActiveBattler))
			return FALSE; //Don't swap out if foe is going to activate the Destiny Bond

		if (OnlyBadMovesLeftInMoveset(gActiveBattler, foe1))
		{
			u8 bestMon, secondBestMon, switchFlags, secondBestSwitchFlags;

			//Try to switch out to the best mon
			bestMon = GetMostSuitableMonToSwitchIntoByParty(party);
			switchFlags = GetMostSuitableMonToSwitchIntoFlags();

			if (switchFlags & (SWITCHING_FLAG_OUTSPEEDS | SWITCHING_FLAG_WALLS_FOE | SWITCHING_FLAG_RESIST_ALL_MOVES) //New mon will either go first on continuously take low damage
			&& PredictedMoveWontDoTooMuchToMon(gActiveBattler, &party[bestMon], foe1, switchFlags))
			{
				gBattleStruct->switchoutIndex[SIDE(gActiveBattler)] = PARTY_SIZE;
				EmitTwoReturnValues(1, ACTION_SWITCH, 0);
				return TRUE;
			}

			//Try the second best mon too because staying in with no moves is a bad idea
			secondBestMon = GetSecondMostSuitableMonToSwitchIntoByParty(party);
			secondBestSwitchFlags = GetSecondMostSuitableMonToSwitchIntoFlags();

			if (secondBestMon != PARTY_SIZE
			&& secondBestSwitchFlags & (SWITCHING_FLAG_OUTSPEEDS | SWITCHING_FLAG_WALLS_FOE | SWITCHING_FLAG_RESIST_ALL_MOVES) //New mon will either go first on continuously take low damage
			&& PredictedMoveWontDoTooMuchToMon(gActiveBattler, &party[secondBestMon], foe1, secondBestSwitchFlags))
			{
				gBattleStruct->switchoutIndex[SIDE(gActiveBattler)] = secondBestMon;
				EmitTwoReturnValues(1, ACTION_SWITCH, 0);
				return TRUE;
			}

			//Check more lenient restrictions when you're choice locked
			if (gBattleStruct->choicedMove[gActiveBattler] && CanBeChoiceLocked(gActiveBattler))
			{
				//Try to switch out to the best mon
				if (!(switchFlags & SWITCHING_FLAG_FAINTS_FROM_FOE)
				&& PredictedMoveWontDoTooMuchToMon(gActiveBattler, &party[bestMon], foe1, switchFlags))
				{
					gBattleStruct->switchoutIndex[SIDE(gActiveBattler)] = PARTY_SIZE;
					EmitTwoReturnValues(1, ACTION_SWITCH, 0);
					return TRUE;
				}
			
				//Try to switch out to the second best mon
				if (!(secondBestSwitchFlags & SWITCHING_FLAG_FAINTS_FROM_FOE)
				&& PredictedMoveWontDoTooMuchToMon(gActiveBattler, &party[bestMon], foe1, secondBestSwitchFlags))
				{
					gBattleStruct->switchoutIndex[SIDE(gActiveBattler)] = secondBestMon;
					EmitTwoReturnValues(1, ACTION_SWITCH, 0);
					return TRUE;
				}
			}
		}
	}

	return FALSE;
}

/*
Don't switch to mon if:
1. Mon has been switched to for type absorbing before (75% / 25% not to switch).
2. It will faint from entry hazards (100%)
3. The mon to switch to has an HP absorption Ability and will be switched in at full health, and the mon out can take a couple hits (100% in Singles).
*/
static bool8 TypeAbosorbingSwitchAbilityCheck(struct Pokemon* mon, u8 monId, u16 predictedMove, u8 absorbingTypeAbility1, u8 absorbingTypeAbility2, u8 absorbingTypeAbility3)
{
	u8 side = SIDE(gActiveBattler);

	//Check mon has been switched to for type absorbing before
	if (gNewBS->ai.didTypeAbsorbSwitchToMonBefore[side] & gBitTable[monId]) //Used this mon for a type switch before
	{
		u8 noSwitchChance = (gNewBS->ai.switchingCooldown[gActiveBattler]) ? 75 : 25; //Just switched in (not type absorb) 75% chance not to switch, otherwise 25% chance

		if (AIRandom() % 100 < noSwitchChance)
			return FALSE; //Only allow switching to this mon again for a type absorbing some of the time
	}

	u8 monAbility = GetMonAbilityAfterTrace(mon, FOE(gActiveBattler));
	monAbility = TryReplaceImposterAbility(monAbility, gActiveBattler);

	if (monAbility == absorbingTypeAbility1
	||  monAbility == absorbingTypeAbility2
	||  monAbility == absorbingTypeAbility3)
	{
		if (!WillFaintFromEntryHazards(mon, side)) //Theres a point to switching in this mon
		{
			u8 foe = FOE(gActiveBattler);

			//Check HP Absorption Switch
			if (IS_SINGLE_BATTLE //Still good to do these kinds of switches in Doubles
			&& IsHPAbsorptionAbility(monAbility) //Ability restores HP when hit
			&& mon->hp == mon->maxHP //Mon is already at max HP
			&& GetMonEntryHazardDamage(mon, side) == 0) //And won't take any damage switching in
			{
				if (gNewBS->ai.switchingCooldown[gActiveBattler]) //AI just switched out
					return FALSE; //Don't double switch when there's no point

				u32 dmg = GetFinalAIMoveDamage(predictedMove, foe, gActiveBattler, 2, NULL);

				if (dmg < gBattleMons[gActiveBattler].hp) //Move doesn't 3HKO - the mon out already isn't going to take a lot of damage from the attack
					return FALSE; //No point of switching to absorbing this attack
			}

			//We found a mon.
			if (IsHPAbsorptionAbility(GetMonAbility(GetBankPartyData(gActiveBattler)))) //The mon being switch out has a type absorption Ability
				gNewBS->ai.didTypeAbsorbSwitchToMonBefore[side] |= gBitTable[gBattlerPartyIndexes[gActiveBattler]]; //Act as if it was switched into to absorb an attack before

			gNewBS->ai.didTypeAbsorbSwitchToMonBefore[side] |= gBitTable[monId];
			gNewBS->ai.typeAbsorbSwitchingCooldown[gActiveBattler] = 2; //Allow switching again for type absorbers on the turn after the next
			gBattleStruct->switchoutIndex[side] = monId;
			EmitTwoReturnValues(1, ACTION_SWITCH, 0);
			return TRUE;
		}
	}

	return FALSE;
}

static bool8 FindMonThatAbsorbsOpponentsMove(struct Pokemon* party, u8 firstId, u8 lastId)
{
	u8 battlerIn1, battlerIn2;
	u8 foe1, foe2;
	u16 predictedMove1, predictedMove2;
	u8 absorbingTypeAbility1, absorbingTypeAbility2, absorbingTypeAbility3;

	LoadBattlersAndFoes(&battlerIn1, &battlerIn2, &foe1, &foe2);

	predictedMove1 = IsValidMovePrediction(foe1, gActiveBattler);
	predictedMove2 = IsValidMovePrediction(foe2, gActiveBattler);

	if (gNewBS->ai.typeAbsorbSwitchingCooldown[gActiveBattler])
		return FALSE; //Just switch in a mon to absorb an attack so don't allow an infinite loop

	if (STAT_STAGE(gActiveBattler, STAT_STAGE_EVASION) >= 6 + 3)
		return FALSE; //Invested in Evasion so don't switch

	if ((predictedMove1 == MOVE_NONE && predictedMove2 == MOVE_NONE) //No predicted move
	|| (SPLIT(predictedMove1) == SPLIT_STATUS && SPLIT(predictedMove2) == SPLIT_STATUS)) //Predicted move is status move
		return FALSE; //Don't bother running checks

	if (IS_SINGLE_BATTLE)
	{
		u8 foeClass = PredictBankFightingStyle(foe1);

		if (!IsClassDamager(foeClass)) //Enemy is just trying to stall out
		{
			if (gNewBS->ai.switchingCooldown[gActiveBattler]) //Just switched in
				return FALSE;

			if (AIRandom() & 1)
				return FALSE; //Only switch 50% of the time
		}

		if (!MoveWouldHitFirst(predictedMove1, foe1, gActiveBattler)) //AI goes first
		{
			if (CanKnockOut(gActiveBattler, foe1)) //And AI can knock out the foe
				return FALSE; //Just KO the opponent and don't worry about switching out
		}
		else //AI goes second
		{
			if (!CanKnockOut(foe1, gActiveBattler) //The enemy can't KO the AI first
			&&   CanKnockOut(gActiveBattler, foe1) //And the AI can KO the foe
			&&   AnyStatGreaterThan(gActiveBattler, 6 + 0)) //And the AI has any stat boost +7 or more
				return FALSE; //Just KO the opponent and don't worry about switching out if you're boosted up
		}

		if (!CanKnockOut(foe1, gActiveBattler) //The enemy can't KO the AI
		&& AnyStatGreaterThan(gActiveBattler, 6 + 1)) //AI is invested in stat boosts +8 or more
			return FALSE;

		if (IS_BEHIND_SUBSTITUTE(gActiveBattler) //AI is behind a Substitute
		&& !DamagingMoveThaCanBreakThroughSubstituteInMoveset(foe1, gActiveBattler)) //And the foe can't hit it through the Substitute
			return FALSE; //Make use of your substitute before switching

		//Check faulty previous prediction
		if (!gDisableStructs[gActiveBattler].isFirstTurn //AI mon has had at least a turn in battle so far
		&& !gDisableStructs[foe1].isFirstTurn //Player mon has had at least a turn in battle so far
		&& gNewBS->ai.previousMovePredictions[foe1][gActiveBattler] == predictedMove1 //Player was predicted to do the same thing last turn
		&& gLastUsedMoves[foe1] != predictedMove1) //But the player didn't do it
			return FALSE; //The player isn't doing what's expected
	}
	else //Double Battle
	{
		if (gNewBS->ai.switchingCooldown[gActiveBattler]) //Just switched in
			return FALSE;

		u16 bestMove1 = gNewBS->ai.bestDoublesKillingMoves[gActiveBattler][foe1];
		u16 bestMove2 = gNewBS->ai.bestDoublesKillingMoves[gActiveBattler][foe2];
		if (GetDoubleKillingScore(bestMove1, gActiveBattler, foe1) >= BEST_DOUBLES_KO_SCORE - 2 //10: Hit 2 Foes, KO 1 Foe/Strongest Move 2 Foes
		||  GetDoubleKillingScore(bestMove2, gActiveBattler, foe2) >= BEST_DOUBLES_KO_SCORE - 2) //10: Hit 2 Foes, KO 1 Foe/Strongest Move 2 Foes
			return FALSE; //Don't switch if this mon can do some major damage to the enemy side

		if (AnyStatGreaterThan(gActiveBattler, 6 + 1) //AI is invested in stat boosts +8 or more
		&& (GetDoubleKillingScore(bestMove1, gActiveBattler, foe1) >= BEST_DOUBLES_KO_SCORE / 2 //6: Hit 1 Foe, KO 1 Foe
		 || GetDoubleKillingScore(bestMove2, gActiveBattler, foe2) >= BEST_DOUBLES_KO_SCORE / 2)) //6: Hit 1 Foe, KO 1 Foe
			return FALSE;
	}

	//Now run the actual type checks
	u8 moveType;
	if (predictedMove1 != MOVE_NONE && SPLIT(predictedMove1) != SPLIT_STATUS) //Foe 1 is going to use a damaging move
	{
		if (IS_DOUBLE_BATTLE
		&& predictedMove2 != MOVE_NONE && SPLIT(predictedMove2) != SPLIT_STATUS
		&& AIRandom() & 1)
		{
			//If both foes are going to use a damaging move, pick one at random to check
			foe1 = foe2;
			predictedMove1 = predictedMove2;
		}
	}
	else
	{
		foe1 = foe2;
		predictedMove1 = predictedMove2;
	}

	moveType = GetMoveTypeSpecial(foe1, predictedMove1);
	if (IS_MOLD_BREAKER(ABILITY(foe1), predictedMove1))
		return FALSE; //Can't absorb move if the Ability is ignored

	switch (moveType) {
		case TYPE_FIRE:
			absorbingTypeAbility1 = ABILITY_FLASHFIRE;
			absorbingTypeAbility2 = ABILITY_FLASHFIRE;
			absorbingTypeAbility3 = ABILITY_FLASHFIRE;
			break;
		case TYPE_ELECTRIC:
			absorbingTypeAbility1 = 0;
			absorbingTypeAbility2 = 0;
			absorbingTypeAbility3 = 0;
			if (!SpeciesHasEarthEater(SPECIES(gBankTarget))){
				absorbingTypeAbility1 = ABILITY_VOLTABSORB;
				absorbingTypeAbility2 = ABILITY_LIGHTNINGROD;
				absorbingTypeAbility3 = ABILITY_MOTORDRIVE;
				}
			break;
		case TYPE_WATER:
			absorbingTypeAbility1 = ABILITY_WATERABSORB;
			absorbingTypeAbility2 = ABILITY_DRYSKIN;
			absorbingTypeAbility3 = ABILITY_STORMDRAIN;
			break;
		case TYPE_GRASS:
			absorbingTypeAbility1 = ABILITY_SAPSIPPER;
			absorbingTypeAbility2 = ABILITY_SAPSIPPER;
			absorbingTypeAbility3 = ABILITY_SAPSIPPER;
			break;
		case TYPE_GROUND:
			absorbingTypeAbility1 = 0;
			absorbingTypeAbility2 = 0;
			absorbingTypeAbility3 = 0;
			if (SpeciesHasEarthEater(SPECIES(gBankTarget))){
				absorbingTypeAbility1 = ABILITY_VOLTABSORB;
				absorbingTypeAbility2 = ABILITY_VOLTABSORB;
				absorbingTypeAbility3 = ABILITY_VOLTABSORB;
				}
			break;
		default:
			return FALSE;
	}

	u8 atkAbility = GetPredictedAIAbility(gActiveBattler, foe1);
	if (atkAbility == absorbingTypeAbility1
	||  atkAbility == absorbingTypeAbility2
	||  atkAbility == absorbingTypeAbility3)
		return FALSE;

	//Give priority to the mons likely to be switched into first
	u8 bestMonId = GetMostSuitableMonToSwitchIntoByParty(party);

	//Check best mon to switch into
	if (bestMonId != gBattleStruct->monToSwitchIntoId[battlerIn1]
	&&  bestMonId != gBattleStruct->monToSwitchIntoId[battlerIn2]
	&&  TypeAbosorbingSwitchAbilityCheck(&party[bestMonId], bestMonId, predictedMove1,
										 absorbingTypeAbility1, absorbingTypeAbility2, absorbingTypeAbility3))
		return TRUE;

	//Check second best mon to switch into
	u8 secondBestMonId = GetSecondMostSuitableMonToSwitchIntoByParty(party);
	if (secondBestMonId != PARTY_SIZE
	&& secondBestMonId != gBattleStruct->monToSwitchIntoId[battlerIn1]
	&& secondBestMonId != gBattleStruct->monToSwitchIntoId[battlerIn2]
	&& TypeAbosorbingSwitchAbilityCheck(&party[secondBestMonId], secondBestMonId, predictedMove1,
										 absorbingTypeAbility1, absorbingTypeAbility2, absorbingTypeAbility3))
		return TRUE;

	//Check the rest of the party
	for (u32 i = firstId; i < lastId; ++i)
	{
		u16 species = party[i].species;

		if (party[i].hp == 0
		||  species == SPECIES_NONE
		||	GetMonData(&party[i], MON_DATA_IS_EGG, 0)
		||  i == bestMonId //Handled above
		||  i == secondBestMonId //Handled above
		||	i == gBattlerPartyIndexes[battlerIn1]
		||	i == gBattlerPartyIndexes[battlerIn2]
		||	i == gBattleStruct->monToSwitchIntoId[battlerIn1]
		||	i == gBattleStruct->monToSwitchIntoId[battlerIn2])
			continue;

		if (TypeAbosorbingSwitchAbilityCheck(&party[i], i, predictedMove1, absorbingTypeAbility1, absorbingTypeAbility2, absorbingTypeAbility3))
			return TRUE;
	}

	return FALSE;
}

static bool8 SwitchToBestResistMonHelper(bool8 willPivot, const struct Pokemon* party, u8 monId, u8 switchFlags, u8 foe)
{
	if (monId >= PARTY_SIZE)
		return FALSE;

	if (switchFlags & SWITCHING_FLAG_RESIST_ALL_MOVES //New mon resists all moves
	//&& AIRandom() % 100 < 75 //75 % chance of taking the switch
	/*&& switchFlags & SWITCHING_FLAG_KO_FOE*/ //And can KO foe
	|| (foe != 0xFF //Actually check what move the foe is using
	 && PredictedMoveKOsSelfDueToContact(gActiveBattler, (struct Pokemon*) &party[monId], foe) //The switch in will KO the foe when it takes contact damage from the switched-in mon
	 && PredictedMoveWontKOMon(gActiveBattler, (struct Pokemon*) &party[monId], foe))) //And the switchd-in mon won't faint due to the contact hit
	{
		ConfirmAISwitch(monId, willPivot);
		return TRUE;
	}

	return FALSE;
}

static bool8 SwitchToBestResistMon(const struct Pokemon* party, bool8 willPivot, u8 foe)
{
	//Check best switching option
	u8 bestMonId = GetMostSuitableMonToSwitchIntoByParty(party);
	u8 switchFlags = GetMostSuitableMonToSwitchIntoFlags();
	if (SwitchToBestResistMonHelper(willPivot, party, bestMonId, switchFlags, foe))
		return TRUE;

	//Check second best switching option
	u8 secondBestMonId = GetSecondMostSuitableMonToSwitchIntoByParty(party);
	switchFlags = GetSecondMostSuitableMonToSwitchIntoFlags();
	if (SwitchToBestResistMonHelper(willPivot, party, secondBestMonId, switchFlags, foe))
		return TRUE;

	//Check the rest of the switching options
	u8 side = SIDE(gActiveBattler);
	u8 opposingBattler1, opposingBattler2;
	u8 battlerIn1, battlerIn2;
	LoadBattlersAndFoes(&battlerIn1, &battlerIn2, &opposingBattler1, &opposingBattler2);

	for (u32 i = 0; i < PARTY_SIZE; ++i)
	{
		if (i == bestMonId
		|| i == secondBestMonId //Skip the mons already checked
		|| i == gBattlerPartyIndexes[battlerIn1]
		|| i == gBattlerPartyIndexes[battlerIn2]
		|| i == gBattleStruct->monToSwitchIntoId[battlerIn1]
		|| i == gBattleStruct->monToSwitchIntoId[battlerIn2])
			continue;

		if (SwitchToBestResistMonHelper(willPivot, party, i, gNewBS->ai.monIdToSwitchIntoFlags[side][i], foe))
			return TRUE;
	}

	return FALSE;
}

static bool8 ShouldSwitchIfNaturalCureOrRegenerator(struct Pokemon* party)
{
	u8 battlerIn1, battlerIn2;
	u8 foe1, foe2;
	u16 aiMovePrediction;
	u16 foe1MovePrediction, foe2MovePrediction;

	LoadBattlersAndFoes(&battlerIn1, &battlerIn2, &foe1, &foe2);

	switch (ABILITY(gActiveBattler)) {
		case ABILITY_NATURALCURE:
			if (WillTakeSignificantDamageFromEntryHazards(gActiveBattler, 4))
				return FALSE; //Don't switch out if you'll take a lot of damage of switch in

			//Don't switch out if you're poisoned and just going to get poisoned again on switch in
			if (gSideTimers[SIDE(gActiveBattler)].tspikesAmount > 0
			&& gBattleMons[gActiveBattler].status1 & STATUS_PSN_ANY
			&& IsMonAffectedByHazards(GetBankPartyData(gActiveBattler))
			&& !IsOfType(gActiveBattler, TYPE_POISON)
			&& CheckGrounding(gActiveBattler))
			{
				//Check if Pokemon that will be sent out will negate Toxic Spikes
				u8 bestMonId;
				bestMonId = GetMostSuitableMonToSwitchIntoByParty(party);

				if (!IsMonOfType(&party[bestMonId], TYPE_POISON) || !CheckMonGrounding(&party[bestMonId])) //Can't negate Toxic Spikes
					return FALSE;
			}

			if (gBattleMons[gActiveBattler].status1 & (STATUS1_SLEEP
			#ifndef FROSTBITE
			| STATUS1_FREEZE
			#endif
			))
				break;
			if (gBattleMons[gActiveBattler].status1 //Has regular status and over half health
			&& gBattleMons[gActiveBattler].hp >= gBattleMons[gActiveBattler].maxHP / 2
			&& !IsDynamaxed(gActiveBattler))
				break;
			return FALSE;

		//Try switch if less than half health, enemy can kill, and mon can't kill enemy first
		case ABILITY_REGENERATOR:
			if (gBattleMons[gActiveBattler].hp > gBattleMons[gActiveBattler].maxHP / 2
			|| IsDynamaxed(gActiveBattler))
				return FALSE;

			if (WillTakeSignificantDamageFromEntryHazards(gActiveBattler, 3))
				return FALSE; //Don't switch out if you'll lose more than you gain by switching out here

			foe1MovePrediction = IsValidMovePrediction(foe1, gActiveBattler);
			foe2MovePrediction = IsValidMovePrediction(foe2, gActiveBattler);

			if ((BATTLER_ALIVE(foe1) && foe1MovePrediction != MOVE_NONE && MoveKnocksOutXHits(foe1MovePrediction, foe1, gActiveBattler, 1)) //Foe can kill AI
			|| (IsDoubleBattle() && BATTLER_ALIVE(foe2) && foe2MovePrediction != MOVE_NONE && MoveKnocksOutXHits(foe2MovePrediction, foe2, gActiveBattler, 1)))
			{
				if (BATTLER_ALIVE(foe1))
				{
					aiMovePrediction = IsValidMovePrediction(gActiveBattler, foe1);
					if (aiMovePrediction != MOVE_NONE && MoveWouldHitFirst(aiMovePrediction, gActiveBattler, foe1) && MoveKnocksOutXHits(aiMovePrediction, gActiveBattler, foe1, 1))
						return FALSE; //Don't switch if can knock out enemy first or enemy can't kill
					else
						break;
				}

				if (IsDoubleBattle() && BATTLER_ALIVE(foe2))
				{
					aiMovePrediction = IsValidMovePrediction(gActiveBattler, foe2);
					if (aiMovePrediction != MOVE_NONE && MoveWouldHitFirst(aiMovePrediction, gActiveBattler, foe2) && MoveKnocksOutXHits(aiMovePrediction, gActiveBattler, foe2, 1))
						return FALSE; //Don't switch if can knock out enemy first or enemy can't kill
					else
						break;
				}
			}

			return FALSE;

		default:
			return FALSE;
	}

	if (SwitchToBestResistMon(party, FALSE, 0xFF))
		return TRUE; //Mon has already been found

	struct Pokemon* mostSuitableMon = &party[GetMostSuitableMonToSwitchIntoByParty(party)]; //Mon to be switched to
	if (IS_SINGLE_BATTLE && !PredictedMoveWontDoTooMuchToMon(gActiveBattler, mostSuitableMon, foe1, GetMostSuitableMonToSwitchIntoFlags())) //Move would do a lot to switched in mon
	{
		if (PredictedMoveWontKOMon(gActiveBattler, mostSuitableMon, foe1))
		{
			if (AIRandom() & 1) //Switch only 50% of the time
				return FALSE;
		}
		else //Attack would KO the mon about to switch in
			return FALSE; //Don't sack your best mon just to save this one
	}

	gBattleStruct->switchoutIndex[SIDE(gActiveBattler)] = PARTY_SIZE;
	EmitTwoReturnValues(1, ACTION_SWITCH, 0);
	return TRUE;
}

static bool8 PassOnWishSwitchingCheck(struct Pokemon* party, u8 monId)
{
	if (party[monId].hp < party[monId].maxHP / 2
	&& !WillFaintFromEntryHazards(&party[monId], SIDE(gActiveBattler))
	&& (IS_DOUBLE_BATTLE
	 || PredictedMoveWontKOMon(gActiveBattler, &party[monId], FOE(gActiveBattler))))
	{
		gBattleStruct->switchoutIndex[SIDE(gActiveBattler)] = monId;
		EmitTwoReturnValues(1, ACTION_SWITCH, 0);
		return TRUE;
	}

	return FALSE;
}

static bool8 PassOnWish(struct Pokemon* party, u8 firstId, u8 lastId)
{
	u8 opposingBattler1, opposingBattler2;
	u8 battlerIn1, battlerIn2;
	LoadBattlersAndFoes(&battlerIn1, &battlerIn2, &opposingBattler1, &opposingBattler2);

	if (gWishFutureKnock.wishCounter[gActiveBattler])
	{
		//Don't switch if your health is less than half and you can survive an opponent's hit
		if (gBattleMons[gActiveBattler].hp < gBattleMons[gActiveBattler].maxHP / 2 //Health is less than half
		&& ((!CanKnockOut(opposingBattler1, gActiveBattler) && !(IS_DOUBLE_BATTLE && CanKnockOut(opposingBattler2, gActiveBattler))) //Both foes can't knock out
		  || HasProtectionMoveInMoveset(gActiveBattler, CHECK_REGULAR_PROTECTION | CHECK_MAT_BLOCK))) //Or you can protect during the wish
			return FALSE;

		//Prioritize the best mons to switch into first
		u8 bestMonId = GetMostSuitableMonToSwitchIntoByParty(party);

		//Check best mon to switch into
		if (bestMonId != gBattleStruct->monToSwitchIntoId[battlerIn1]
		&&  bestMonId != gBattleStruct->monToSwitchIntoId[battlerIn2]
		&&  PassOnWishSwitchingCheck(party, bestMonId))
			return TRUE;

		//Check second best mon to switch into
		u8 secondBestMonId = GetSecondMostSuitableMonToSwitchIntoByParty(party);
		if (secondBestMonId != PARTY_SIZE
		&& secondBestMonId != gBattleStruct->monToSwitchIntoId[battlerIn1]
		&& secondBestMonId != gBattleStruct->monToSwitchIntoId[battlerIn2]
		&& PassOnWishSwitchingCheck(party, secondBestMonId))
			return TRUE;

		for (u32 i = firstId; i < lastId; ++i)
		{
			if (party[i].hp == 0
			||  party[i].species == SPECIES_NONE
			||	GetMonData(&party[i], MON_DATA_IS_EGG, 0)
			||	i == gBattlerPartyIndexes[battlerIn1]
			||	i == gBattlerPartyIndexes[battlerIn2]
			||	i == gBattleStruct->monToSwitchIntoId[battlerIn1]
			||	i == gBattleStruct->monToSwitchIntoId[battlerIn2]
			||  i == bestMonId //Already checked
			||  i == secondBestMonId) //Already checked
				continue;

			if (PassOnWishSwitchingCheck(party, i))
				return TRUE;
		}
	}

	return FALSE;
}

static bool8 SemiInvulnerableTroll(void)
{
	u8 opposingBattler1, opposingBattler2;

	if (IS_DOUBLE_BATTLE)
	{
		opposingBattler1 = FOE(gActiveBattler);
		opposingBattler2 = PARTNER(opposingBattler1);
	}
	else
	{
		opposingBattler1 = FOE(gActiveBattler);
		opposingBattler2 = opposingBattler1;
	}

	if (IS_DOUBLE_BATTLE)
	{
		if (!(gStatuses3[opposingBattler1] & STATUS3_SEMI_INVULNERABLE) && !(gStatuses3[opposingBattler2] & STATUS3_SEMI_INVULNERABLE))
			return FALSE;
		#ifndef REALLY_SMART_AI
			if (ViableMonCountFromBank(gActiveBattler) > 1)
				return FALSE; //AI doesn't know which mon is being targeted if there's more than one on the field
		#endif
	}
	else if (!(gStatuses3[opposingBattler1] & STATUS3_SEMI_INVULNERABLE))
		return FALSE;

	if (IsDynamaxed(gActiveBattler) && gNewBS->dynamaxData.timer[gActiveBattler] > 1) //Going to be Dynamaxed after this turn
		return FALSE;

	if (RunAllSemiInvulnerableLockedMoveCalcs(opposingBattler1, opposingBattler2, FALSE))
		return TRUE;

	return FALSE;
}

static u8 GetBestPartyNumberForSemiInvulnerableLockedMoveCalcs(u8 opposingBattler1, u8 opposingBattler2, bool8 checkLockedMoves)
{
	u8 option1 = TheCalcForSemiInvulnerableTroll(opposingBattler1, MOVE_RESULT_DOESNT_AFFECT_FOE, checkLockedMoves);
	if (option1 != PARTY_SIZE)
	{
		return option1;
	}

	if (IS_DOUBLE_BATTLE)
	{
		u8 option2 = TheCalcForSemiInvulnerableTroll(opposingBattler2, MOVE_RESULT_DOESNT_AFFECT_FOE, checkLockedMoves);
		if (option2 != PARTY_SIZE)
		{
			return option2;
		}
	}

	u8 option3 = TheCalcForSemiInvulnerableTroll(opposingBattler1, MOVE_RESULT_NOT_VERY_EFFECTIVE, checkLockedMoves);
	if (option3 != PARTY_SIZE)
	{
		return option3;
	}

	if (IS_DOUBLE_BATTLE)
	{
		u8 option4 = TheCalcForSemiInvulnerableTroll(opposingBattler2, MOVE_RESULT_NOT_VERY_EFFECTIVE, checkLockedMoves);
		if (option4 != PARTY_SIZE)
		{
			return option4;
		}
	}

	return PARTY_SIZE;
}

static bool8 RunAllSemiInvulnerableLockedMoveCalcs(u8 opposingBattler1, u8 opposingBattler2, bool8 checkLockedMoves)
{
	u8 option = GetBestPartyNumberForSemiInvulnerableLockedMoveCalcs(opposingBattler1, opposingBattler2, checkLockedMoves);
	if (option != PARTY_SIZE)
	{
		gBattleStruct->switchoutIndex[SIDE(gActiveBattler)] = option;
		EmitTwoReturnValues(1, ACTION_SWITCH, 0);
	}

	return FALSE;
}

static u8 TheCalcForSemiInvulnerableTroll(u8 bankAtk, u8 flags, bool8 checkLockedMoves)
{
	u8 firstId, lastId;
	u8 battlerIn1, battlerIn2;
	int i;

	if (IS_DOUBLE_BATTLE)
	{
		battlerIn1 = gActiveBattler;
		if (gAbsentBattlerFlags & gBitTable[PARTNER(gActiveBattler)])
			battlerIn2 = gActiveBattler;
		else
			battlerIn2 = PARTNER(gActiveBattler);
	}
	else
	{
		battlerIn1 = gActiveBattler;
		battlerIn2 = gActiveBattler;
	}

	struct Pokemon* party = LoadPartyRange(gActiveBattler, &firstId, &lastId);

	if (((gStatuses3[bankAtk] & STATUS3_SEMI_INVULNERABLE) || (checkLockedMoves && gLockedMoves[bankAtk] != MOVE_NONE))
	&&	gBattleStruct->moveTarget[bankAtk] == gActiveBattler)
	{
		u8 newFlags = AI_SpecialTypeCalc(gLockedMoves[bankAtk], bankAtk, gActiveBattler);
		if (BATTLER_ALIVE(gActiveBattler)) //Not end turn switch
		{
			if (newFlags & flags) //Target already has these type flags so why switch?
				return PARTY_SIZE;
		}

		for (i = firstId; i < lastId; ++i)
		{
			if (party[i].hp == 0
			||  party[i].species == SPECIES_NONE
			||	GetMonData(&party[i], MON_DATA_IS_EGG, 0)
			||	i == gBattlerPartyIndexes[battlerIn1]
			||	i == gBattlerPartyIndexes[battlerIn2]
			||	i == gBattleStruct->monToSwitchIntoId[battlerIn1]
			||	i == gBattleStruct->monToSwitchIntoId[battlerIn2])
				continue;

			if (AI_TypeCalc(gLockedMoves[bankAtk], bankAtk, gActiveBattler, &party[i]) & flags)
			{
				return i;
			}
		}
	}

	return PARTY_SIZE;
}

static bool8 CanStopLockedMove(void)
{
	u8 opposingBattler1, opposingBattler2;

	if (IS_DOUBLE_BATTLE)
	{
		opposingBattler1 = FOE(gActiveBattler);
		opposingBattler2 = PARTNER(opposingBattler1);
	}
	else
	{
		opposingBattler1 = FOE(gActiveBattler);
		opposingBattler2 = opposingBattler1;
	}

	if (IS_DOUBLE_BATTLE)
	{
		if (!(gLockedMoves[opposingBattler1] && SPLIT(gLockedMoves[opposingBattler1]) != SPLIT_STATUS) && !(gLockedMoves[opposingBattler2] && SPLIT(gLockedMoves[opposingBattler2]) != SPLIT_STATUS))
			return FALSE;
		#ifndef REALLY_SMART_AI
			if (ViableMonCountFromBank(gActiveBattler) > 1)
				return FALSE; //AI doesn't know which mon is being targeted if there's more than one on the field
		#endif
	}
	else if (!(gLockedMoves[opposingBattler1] && SPLIT(gLockedMoves[opposingBattler1]) != SPLIT_STATUS))
		return FALSE;

	if (IsDynamaxed(gActiveBattler) && gNewBS->dynamaxData.timer[gActiveBattler] > 1) //Going to be Dynamaxed after this turn
		return FALSE;

	if (RunAllSemiInvulnerableLockedMoveCalcs(opposingBattler1, opposingBattler2, TRUE))
		return TRUE;

	return FALSE;
}

static bool8 ShouldSwitchWhenYawned(void)
{
	u8 itemEffect;

	if (gStatuses3[gActiveBattler] & STATUS3_YAWN
	&& ABILITY(gActiveBattler) != ABILITY_NATURALCURE //Can switch out when asleep to cure
	&& (itemEffect = ITEM_EFFECT(gActiveBattler)) != ITEM_EFFECT_CURE_SLP //Will wake up as soon as it falls asleep
	&& itemEffect != ITEM_EFFECT_CURE_STATUS
	&& (!IsDynamaxed(gActiveBattler) || AIRandom() & 1) //50% chance to switch out if Dynamaxed
	&& gBattleMons[gActiveBattler].hp > gBattleMons[gActiveBattler].maxHP / 4 //Don't bother saving a mon with less than 25% of HP
	&& CanBePutToSleep(gActiveBattler, gActiveBattler, FALSE)) //Could have been yawned and then afflicted with another status condition
	{
		u8 i;
		u8 battlerIn1, battlerIn2;
		u8 foe1, foe2;
		LoadBattlersAndFoes(&battlerIn1, &battlerIn2, &foe1, &foe2);

		//Don't switch if one foe left and you can KO it
		if (ViableMonCountFromBank(foe1) <= 1)
		{
			if (BATTLER_ALIVE(foe1))
			{
				if (CanKnockOut(gActiveBattler, foe1))
					return FALSE;
			}

			if (BATTLER_ALIVE(foe2))
			{
				if (CanKnockOut(gActiveBattler, foe2))
					return FALSE;
			}
		}

		//Don't switch if there's an enemy being trapped while taking secondary damage
		u8 activeSide = SIDE(gActiveBattler);
		for (i = 0; i < gBattlersCount; ++i)
		{
			if (SIDE(i) != activeSide)
			{
				if (IsTrapped(i, TRUE) && IsTakingSecondaryDamage(i, TRUE))
					return FALSE;
			}
		}

		//Don't switch if invested in Evasion and it'll hold up
		if (STAT_STAGE(gActiveBattler, STAT_STAGE_EVASION) >= 6 + 3
		&& !ABILITY_ON_OPPOSING_FIELD(gActiveBattler, ABILITY_UNAWARE)
		&& !ABILITY_ON_OPPOSING_FIELD(gActiveBattler, ABILITY_KEENEYE))
			return FALSE;

		//Don't switch if you can fight through the sleep
		u8 ability = ABILITY(gActiveBattler);
		if (/*itemEffect == ITEM_EFFECT_CURE_SLP //Checked above
		|| itemEffect == ITEM_EFFECT_CURE_STATUS
		||*/ ability == ABILITY_EARLYBIRD
		|| ability == ABILITY_SHEDSKIN
		|| MoveEffectInMoveset(EFFECT_SNORE, gActiveBattler)
		|| MoveEffectInMoveset(EFFECT_SLEEP_TALK, gActiveBattler)
		|| (MoveInMoveset(MOVE_ELECTRICTERRAIN, gActiveBattler) && IsAffectedByElectricTerrain(gActiveBattler))
		|| (MoveInMoveset(MOVE_MISTYTERRAIN, gActiveBattler) && CheckGrounding(gActiveBattler)))
			return FALSE;

		//Check if can do major damage instead of switching
		if (IS_SINGLE_BATTLE)
		{
			u16 foePrediction = IsValidMovePrediction(foe1, gActiveBattler);

			if (!IsPredictedToSwitch(foe1, gActiveBattler) //Enemy isn't going to switch
			&&  gBattleMoves[foePrediction].effect != EFFECT_PROTECT //Enemy isn't going to protect
			&& (gBattleMoves[foePrediction].effect != EFFECT_SEMI_INVULNERABLE || !MoveWouldHitFirst(foePrediction, foe1, gActiveBattler))) //Enemy isn't going to dodge attack first with Fly etc.
			{
				if (!MoveWouldHitFirst(foePrediction, foe2, gActiveBattler)) //AI goes first
				{
					if (CanKnockOut(gActiveBattler, foe1))
						return FALSE; //You're going to fall asleep, but at least take down the enemy with you
				}
				else
				{
					if (!CanKnockOut(foe1, gActiveBattler) //The enemy can't KO you first
					&&  CanKnockOut(gActiveBattler, foe1))
						return FALSE; //You're going to fall asleep, but at least take down the enemy with you
				}
			}
		}
		else //Double Battle
		{
			u16 bestMove1 = gNewBS->ai.bestDoublesKillingMoves[gActiveBattler][foe1];
			u16 bestMove2 = gNewBS->ai.bestDoublesKillingMoves[gActiveBattler][foe2];
			if (GetDoubleKillingScore(bestMove1, gActiveBattler, foe1) >= BEST_DOUBLES_KO_SCORE
			||  GetDoubleKillingScore(bestMove2, gActiveBattler, foe2) >= BEST_DOUBLES_KO_SCORE)
				return FALSE; //Don't switch if this mon can do some major damage to the enemy side
		}

		if (IsSleepClauseInEffect(gActiveBattler) //AI can only have one Pokemon to sleep
		&& !DoesSleepClausePrevent(gActiveBattler)) //And AI mon will actually fall asleep
		{
			u8 switchFlags = GetMostSuitableMonToSwitchIntoFlags();
			u8 wantedFlags = (SWITCHING_FLAG_KO_FOE | SWITCHING_FLAG_OUTSPEEDS); //We don't want a slow resist mon here because the player could just keep spamming yawn to force switches

			if ((switchFlags & wantedFlags) != wantedFlags) //Best switching mon can't just come in and KO the yawner
				return FALSE; //Let this mon fall asleep and deal with the consequences later
		}

		gBattleStruct->switchoutIndex[SIDE(gActiveBattler)] = PARTY_SIZE;
		EmitTwoReturnValues(1, ACTION_SWITCH, 0);
		return TRUE;
	}

	return FALSE;
}

static bool8 ShouldSwitchWhileAsleep(struct Pokemon* party)
{
	if (gNewBS->ai.switchingCooldown[gActiveBattler]) //Just switched in
		return FALSE;

	if (IsBankAsleep(gActiveBattler)
	&& AIRandom() & 1 //50 % chance of switching out here
	&& (!IsDynamaxed(gActiveBattler) //Not Dynamaxed
	 || (s8) (gBattleMons[gActiveBattler].status1 & STATUS1_SLEEP) > gNewBS->dynamaxData.timer[gActiveBattler])) //Or will wake up after the Dynamax ends
	{
		u8 ability = ABILITY(gActiveBattler);

		if (STAT_STAGE(gActiveBattler, STAT_STAGE_ATK) >= 8
		||  STAT_STAGE(gActiveBattler, STAT_STAGE_SPATK) >= 8
		||  STAT_STAGE(gActiveBattler, STAT_STAGE_SPEED) >= 8
		||  STAT_STAGE(gActiveBattler, STAT_STAGE_EVASION) >= 9)
		{
			if (AIRandom() & 1) //50 % chance of not switching out here
				return FALSE; //Lower chance of switching out if AI has some good stat boosted
		}

		if (ability == ABILITY_SHEDSKIN
		|| ability == ABILITY_EARLYBIRD
		|| (ability == ABILITY_HYDRATION && gBattleWeather & WEATHER_RAIN_ANY && gWishFutureKnock.weatherDuration != 1)
		|| (IS_DOUBLE_BATTLE && BATTLER_ALIVE(PARTNER(gActiveBattler)) && ABILITY(PARTNER(gActiveBattler)) == ABILITY_HEALER)
		|| MoveEffectInMoveset(EFFECT_SLEEP_TALK, gActiveBattler)
		|| MoveEffectInMoveset(EFFECT_SNORE, gActiveBattler)
		|| MoveEffectInMoveset(EFFECT_REST, gActiveBattler) //You probably put yourself to sleep
		|| (IsClassStall(GetBankFightingStyle(gActiveBattler)) && IsTrapped(FOE(gActiveBattler), TRUE))) //You trapped the opponent in with you
			return FALSE; //Don't switch

		if (IS_SINGLE_BATTLE)
		{
			u8 bestMonId = GetMostSuitableMonToSwitchIntoByParty(party);
			u8 switchFlags = GetMostSuitableMonToSwitchIntoFlags();

			if (party[bestMonId].condition & STATUS1_SLEEP && party[bestMonId].condition > 1)
				return FALSE; //Don't switch into a mon that's still asleep

			if (!PredictedMoveWontDoTooMuchToMon(gActiveBattler, &party[bestMonId], FOE(gActiveBattler), switchFlags)) //Move will do a lot
				return FALSE; //Could lose the new mon switched in
		}

		gBattleStruct->switchoutIndex[SIDE(gActiveBattler)] = PARTY_SIZE;
		EmitTwoReturnValues(1, ACTION_SWITCH, 0);
		return TRUE;
	}

	return FALSE;
}

static bool8 AnnoyingSecondaryDamageSwitchCheck(u8 monId, u8 switchFlags, struct Pokemon* party, bool8 urgent)
{
	u8 foe = FOE(gActiveBattler);
	bool8 goodToSwitch = FALSE;

	if (monId >= PARTY_SIZE)
		return FALSE;

	if (urgent)
		goodToSwitch = TRUE;
	else if (switchFlags & SWITCHING_FLAG_OUTSPEEDS //New mon will go first
	&& ((switchFlags & SWITCHING_FLAG_KO_FOE) //Can KO 
	 || (switchFlags & SWITCHING_FLAG_CAN_2HKO && !(switchFlags & SWITCHING_FLAG_FAINTS_FROM_FOE))) //Or 2HKO and survive a hit
	&& (IS_DOUBLE_BATTLE
	 || PredictedMoveWontDoTooMuchToMon(gActiveBattler, &party[monId], foe, switchFlags))) //And won't take a lot of damage on the switch in
		goodToSwitch = TRUE;
	else if (switchFlags & (SWITCHING_FLAG_RESIST_ALL_MOVES | SWITCHING_FLAG_WALLS_FOE)) //Just walls in general
		goodToSwitch = TRUE;
	else
	{
		//New mon might die on the switch in, so determine if it's worth it to sack the mon
		if (BATTLER_ALIVE(foe) && Can2HKO(gActiveBattler, foe)) //This mon could KO a foe in a couple of turns
			goodToSwitch = TRUE;

		if (IS_DOUBLE_BATTLE)
		{
			foe = PARTNER(foe);
			if (BATTLER_ALIVE(foe) && Can2HKO(gActiveBattler, foe)) //This mon could KO a foe in a couple of turns
				goodToSwitch = TRUE;
		}
	}

	if (goodToSwitch)
	{
		gBattleStruct->switchoutIndex[SIDE(gActiveBattler)] = monId;
		EmitTwoReturnValues(1, ACTION_SWITCH, 0);
		return TRUE;
	}

	return FALSE;
}

static bool8 IsTakingAnnoyingSecondaryDamage(struct Pokemon* party)
{
	u8 ability = GetPredictedAIAbility(gActiveBattler, FOE(gActiveBattler));

	if (ability != ABILITY_MAGICGUARD
	&& !CanKillAFoe(gActiveBattler)
	&& !IsDynamaxed(gActiveBattler)
	&& AI_THINKING_STRUCT->aiFlags > AI_SCRIPT_CHECK_BAD_MOVE) //Has smart AI
	{
		bool8 trySwitch = FALSE;
		bool8 urgent = FALSE;

		if (gStatuses3[gActiveBattler] & STATUS3_LEECHSEED && (AIRandom() & 3) == 0 //25% chance to switch out when seeded
		&& !AnyUsefulOffseniveStatIsRaised(gActiveBattler) //Unless invested in offensive stat buffs
		&& !(IS_SINGLE_BATTLE && CanKnockOut(gActiveBattler, FOE(gActiveBattler)))) //Or can KO the foe and not suffer the seed damage
		{
			trySwitch = TRUE;
		}
		else if (((gBattleMons[gActiveBattler].status1 & STATUS1_SLEEP) > 1 && gBattleMons[gActiveBattler].status2 & STATUS2_NIGHTMARE)
		|| (gBattleMons[gActiveBattler].status2 & STATUS2_CURSED)
		|| ((gBattleMons[gActiveBattler].status1 & STATUS1_TOXIC_COUNTER) > 0x600 && ability != ABILITY_POISONHEAL) //Been sitting with toxic for 6 turns
		|| (gBattleMons[gActiveBattler].status1 & STATUS1_PSN_ANY && ability != ABILITY_POISONHEAL
		 && GetMonAbility(GetBankPartyData(gActiveBattler)) == ABILITY_POISONHEAL)) //Had Poison Heal but lost it
		{
			trySwitch = TRUE;
			urgent = TRUE;
		}

		if (trySwitch && !WillTakeSignificantDamageFromEntryHazards(gActiveBattler, 4)) //Don't switch out if you'll take a lot of damage on switch in
		{
			if (AnnoyingSecondaryDamageSwitchCheck(GetMostSuitableMonToSwitchIntoByParty(party), GetMostSuitableMonToSwitchIntoFlags(), party, urgent) //Try to switch to the first best mon
			|| AnnoyingSecondaryDamageSwitchCheck(GetSecondMostSuitableMonToSwitchIntoByParty(party), GetSecondMostSuitableMonToSwitchIntoFlags(), party, urgent)) //Try to switch to the second best mon
				return TRUE;
		}
	}

	return FALSE;
}

static bool8 ShouldSwitchToAvoidDeathHelper(struct Pokemon* party, u8 bankDef, u16 defMove, u8 monId, u8 switchFlags)
{
	if (monId >= PARTY_SIZE)
		return FALSE;

	struct Pokemon* mon = &party[monId];

	if
	//OPTION A
		((switchFlags & (SWITCHING_FLAG_OUTSPEEDS | SWITCHING_FLAG_WALLS_FOE | SWITCHING_FLAG_RESIST_ALL_MOVES) //Will either not waste the switch or can wall
		&& (AI_TypeCalc(defMove, bankDef, gActiveBattler, mon) & MOVE_RESULT_NO_EFFECT)) //Move will have no effect on switched in mon

	//OPTION B
	||  (!(gBattleTypeFlags & BATTLE_TYPE_BENJAMIN_BUTTERFREE) //Death is only a figment of the imagination in this format
		&& ((switchFlags & (SWITCHING_FLAG_WALLS_FOE | SWITCHING_FLAG_RESIST_ALL_MOVES)) //Walls foe
		 || (switchFlags & SWITCHING_FLAG_KO_FOE && switchFlags & SWITCHING_FLAG_OUTSPEEDS)) //Or can go first and KO
		&& PredictedMoveWontDoTooMuchToMon(gActiveBattler, mon, bankDef, switchFlags)) //Move will affect but not do too much damage

	//OPTION C
	|| (PredictedMoveKOsSelfDueToContact(gActiveBattler, mon, bankDef) //The switch in will KO the foe when it takes contact damage from the switched-in mon
	 && PredictedMoveWontKOMon(gActiveBattler, mon, bankDef))) //The switched-in mon can survive the hit
	{
		gBattleStruct->switchoutIndex[SIDE(gActiveBattler)] = monId;
		EmitTwoReturnValues(1, ACTION_SWITCH, 0);
		return TRUE;
	}

	return FALSE;
}

static bool8 ShouldSwitchToAvoidDeath(struct Pokemon* party)
{
	if (gNewBS->ai.switchingCooldown[gActiveBattler]) //Just switched in
		return FALSE;

	if (gBattleMons[gActiveBattler].status2 & STATUS2_DESTINY_BOND)
		return FALSE; //It's honestly better to die

	if (IS_SINGLE_BATTLE
	&& AI_THINKING_STRUCT->aiFlags > AI_SCRIPT_CHECK_BAD_MOVE) //Has smart AI
	{
		u8 bankDef = FOE(gActiveBattler);
		u16 atkMove = IsValidMovePrediction(gActiveBattler, bankDef);
		u16 defMove = IsValidMovePrediction(bankDef, gActiveBattler);

		if (gBattleMons[gActiveBattler].status1 & STATUS1_PARALYSIS
		&& gBattleMons[gActiveBattler].hp < gBattleMons[gActiveBattler].maxHP / 3)
		{
			u8 ability = ABILITY(gActiveBattler);
			if (ability != ABILITY_QUICKFEET && ability != ABILITY_NATURALCURE)
				return FALSE; //Don't switch out a paralyzed Pokemon that'll probably be KO'd when it switches back in
		}

		if ((SPECIES(gActiveBattler) == SPECIES_AEGISLASH || SPECIES(gActiveBattler) == SPECIES_AEGISLASH_BLADE) //Mon to be switched is Aegislash
		&& gDisableStructs[gActiveBattler].protectUses == 0 //And it didn't use King's Shield last
		&& defMove != MOVE_NONE //Aegislash would be hit
		&& CheckContact(defMove, bankDef, gActiveBattler) //With a contact move
		&& RealPhysicalMoveInMoveset(bankDef) //That's probably physical
		&& ABILITY(gActiveBattler) == ABILITY_STANCECHANGE
		&& MoveInMoveset(MOVE_KINGSSHIELD, gActiveBattler))
			return FALSE; //Don't switch and use King's Shield instead

		if (defMove != MOVE_NONE //Foe going to attack
		&& (atkMove == MOVE_NONE || !MoveWouldHitFirst(atkMove, gActiveBattler, bankDef)) //Attacker wouldn't go first
		&& (!IS_BEHIND_SUBSTITUTE(gActiveBattler) || !MoveBlockedBySubstitute(defMove, bankDef, gActiveBattler))
		&&  MoveKnocksOutXHits(defMove, bankDef, gActiveBattler, 1) //Foe will kill
		&& !WillTakeSignificantDamageFromEntryHazards(gActiveBattler, 3) //33% health loss
		&& (GetHealthPercentage(gActiveBattler) > 20 //Don't switch out mons that are super close to death
		 || GetMostSuitableMonToSwitchIntoScore() >= SWITCHING_SCORE_MAX) //Unless its a really good switch
		 /*|| TODO: Damager and faster than majority of living enemy team*/)
		{
			u8 bestMonId = GetMostSuitableMonToSwitchIntoByParty(party);
			u8 secondBestMonId = GetSecondMostSuitableMonToSwitchIntoByParty(party);
			if (ShouldSwitchToAvoidDeathHelper(party, bankDef, defMove, bestMonId, GetMostSuitableMonToSwitchIntoFlags())
			||  ShouldSwitchToAvoidDeathHelper(party, bankDef, defMove, secondBestMonId, GetSecondMostSuitableMonToSwitchIntoFlags()))
				return TRUE;

			u8 side = SIDE(gActiveBattler);
			u8 battlerIn1, battlerIn2;
			u8 foe1, foe2;
			LoadBattlersAndFoes(&battlerIn1, &battlerIn2, &foe1, &foe2);

			for (u32 i = 0; i < PARTY_SIZE; ++i) //Run a check on the whole party now
			{
				u16 species = GetMonData(&party[i], MON_DATA_SPECIES2, 0);

				if (SPECIES_CANT_BATTLE(species)
				|| 	party[i].hp == 0
				||  i == bestMonId
				||  i == secondBestMonId //Already checked these
				||	i == gBattlerPartyIndexes[battlerIn1]
				||	i == gBattlerPartyIndexes[battlerIn2]
				||	i == gBattleStruct->monToSwitchIntoId[battlerIn1]
				||  i == gBattleStruct->monToSwitchIntoId[battlerIn2])
					continue;

				if (ShouldSwitchToAvoidDeathHelper(party, bankDef, defMove, i, gNewBS->ai.monIdToSwitchIntoFlags[side][i]))
					return TRUE;
			}
		}
	}

	return FALSE;
}

static bool8 ShouldSwitchIfWonderGuard(struct Pokemon* party, u8 firstId, u8 lastId)
{
	u8 battlerIn1;
	u8 opposingBattler;
	u8 moveFlags;
	u32 i, j;
	opposingBattler = FOE(gActiveBattler);

	if (IS_DOUBLE_BATTLE)
		return FALSE;

	if (ABILITY(opposingBattler) != ABILITY_WONDERGUARD)
		return FALSE;

	if (WillFaintFromSecondaryDamage(opposingBattler))
		return FALSE;

	//Check if pokemon has a super effective move, Mold Breaker Move, or move that can hurt Shedinja
	u8 bankAtk = gActiveBattler;
	u8 bankDef = opposingBattler;
	u8 moveLimitations = CheckMoveLimitations(bankAtk, 0, AdjustMoveLimitationFlagsForAI(bankAtk));
	for (i = 0; i < MAX_MON_MOVES; ++i)
	{
		u16 move = GetBattleMonMove(gActiveBattler, i);
		if (move == MOVE_NONE)
			break;

		if (!(gBitTable[i] & moveLimitations))
		{
			if (gSpecialMoveFlags[move].gMoldBreakerMoves)
				return FALSE;

			if (SPLIT(move) != SPLIT_STATUS)
			{
				u8 atkAbility = GetAIAbility(bankAtk, bankDef, move);
				if (IsMoldBreakerAbility(atkAbility))
					return FALSE;

				moveFlags = AI_SpecialTypeCalc(move, bankAtk, bankDef);
				if (moveFlags & MOVE_RESULT_SUPER_EFFECTIVE)
					return FALSE;
			}
			else if (!MoveBlockedBySubstitute(move, bankAtk, bankDef))
			{
				switch (gBattleMoves[move].effect) {
					case EFFECT_SLEEP:
					case EFFECT_YAWN:
						if (CanBePutToSleep(bankDef, bankAtk, TRUE)) //Don't use CanBeYawned since we want the sleep to actually stick
							return FALSE;
						break;
					case EFFECT_ROAR:
						if (HasMonToSwitchTo(bankDef))
							return FALSE;
						break;
					case EFFECT_TOXIC:
					case EFFECT_POISON:
					PSN_CHECK:
						if (CanBePoisoned(bankDef, bankAtk, TRUE))
							return FALSE;
						break;
					case EFFECT_WILL_O_WISP:
					BRN_CHECK:
						if (CanBeBurned(bankDef, bankAtk, TRUE))
							return FALSE;
						break;
					case EFFECT_CONFUSE:
					case EFFECT_SWAGGER:
					case EFFECT_FLATTER:
						if (CanBeConfused(bankDef, bankAtk, TRUE))
							return FALSE;
						break;
					case EFFECT_PARALYZE:
						if (CanBeParalyzed(bankDef, bankAtk, TRUE))
							return FALSE;
						break;
					case EFFECT_LEECH_SEED:
						if (!IsOfType(bankDef, TYPE_GRASS))
							return FALSE;
						break;
					case EFFECT_NIGHTMARE:
						if (gBattleMons[bankDef].status1 & STATUS_SLEEP)
							return FALSE;
						break;
					case EFFECT_CURSE:
						if (IsOfType(bankAtk, TYPE_GHOST))
							return FALSE;
						break;
					case EFFECT_SPIKES:
						switch (move) {
							case MOVE_STEALTHROCK:
								if (gSideTimers[SIDE(bankDef)].srAmount == 0)
									return FALSE;
								break;

							case MOVE_TOXICSPIKES:
								if (gSideTimers[SIDE(bankDef)].tspikesAmount < 2)
									return FALSE;
								break;

							case MOVE_STICKYWEB:
								if (gSideTimers[SIDE(bankDef)].stickyWeb == 0)
									return FALSE;
								break;

							default: //Spikes
								if (gSideTimers[SIDE(bankDef)].spikesAmount < 3)
									return FALSE;
								break;
						}
						break;
					case EFFECT_PERISH_SONG:
						if (!(gStatuses3[bankDef] & STATUS3_PERISH_SONG))
							return FALSE;
						break;
					case EFFECT_SANDSTORM:
						if (SandstormHurts(bankDef))
							return FALSE;
						break;
					case EFFECT_HAIL:
						if (HailHurts(bankDef))
							return FALSE;
						break;
					case EFFECT_BATON_PASS:
					case EFFECT_TELEPORT:
						return FALSE;
					case EFFECT_MEMENTO:
						if (SPLIT(move) == SPLIT_STATUS)
							return FALSE;
						break;
					case EFFECT_TRICK:
						if (move == MOVE_TRICK)
						{
							u8 atkSpecies = SPECIES(bankAtk);
							u8 defSpecies = SPECIES(bankDef);

							if (CanTransferItem(atkSpecies, ITEM(bankAtk))
							&& CanTransferItem(atkSpecies, ITEM(bankDef))
							&& CanTransferItem(defSpecies, ITEM(bankAtk))
							&& CanTransferItem(defSpecies, ITEM(bankDef))
							&& !(gNewBS->corrodedItems[SIDE(bankAtk)] & gBitTable[gBattlerPartyIndexes[bankAtk]])
							&& !(gNewBS->corrodedItems[SIDE(bankDef)] & gBitTable[gBattlerPartyIndexes[bankDef]]))
							{
								switch (ITEM_EFFECT(bankAtk)) {
									case ITEM_EFFECT_TOXIC_ORB:
										goto PSN_CHECK;
									case ITEM_EFFECT_FLAME_ORB:
										goto BRN_CHECK;
									case ITEM_EFFECT_BLACK_SLUDGE:
										if (!IsOfType(bankDef, TYPE_POISON))
											return FALSE;
										break;
									case ITEM_EFFECT_STICKY_BARB:
										return FALSE;
								}
							}
						}
						break;
					case EFFECT_WISH:
						if (gWishFutureKnock.wishCounter[bankAtk] == 0)
							return FALSE;
						break;
					case EFFECT_SKILL_SWAP:
						if (move != MOVE_SKILLSWAP)
							return FALSE;
						break;

					case EFFECT_TYPE_CHANGES:
						switch (move) {
							case MOVE_TRICKORTREAT:
								if (!IsOfType(bankDef, TYPE_GHOST))
									return FALSE;
								break;
							case MOVE_FORESTSCURSE:
								if (!IsOfType(bankDef, TYPE_GRASS))
									return FALSE;
								break;
						}
						break;
					case EFFECT_TEAM_EFFECTS:
						switch (move) {
							case MOVE_TAILWIND:
								if (BankSideHasTailwind(bankAtk))
									return FALSE;
								break;
							case MOVE_LUCKYCHANT:
								if (gNewBS->LuckyChantTimers[SIDE(bankAtk)] == 0)
									return FALSE;
								break;
						}
						break;
				}
			}
		}
	}


	battlerIn1 = gActiveBattler;

	//Find a pokemon in the party that has a super effective move
	for (i = firstId; i < lastId; ++i)
	{
		if (party[i].hp == 0
			|| party[i].species == SPECIES_NONE
			|| GetMonData(&party[i], MON_DATA_IS_EGG, 0)
			|| i == gBattlerPartyIndexes[battlerIn1]
			|| i == gBattleStruct->monToSwitchIntoId[battlerIn1])
			continue;

		for (j = 0; j < MAX_MON_MOVES; j++) {
			u16 move = party[i].moves[j];
			if (move == MOVE_NONE || SPLIT(move) == SPLIT_STATUS)
				continue;

			moveFlags = TypeCalc(move, gActiveBattler, opposingBattler, &party[i]);
			if (moveFlags & MOVE_RESULT_SUPER_EFFECTIVE)
			{
				// we found a mon
				gBattleStruct->switchoutIndex[SIDE(gActiveBattler)] = i;
				EmitTwoReturnValues(1, ACTION_SWITCH, 0);
				return TRUE;
			}
		}
	}

	return FALSE; // at this point there is not a single pokemon in the party that has a super effective move against a pokemon with wonder guard
}

static bool8 ShouldSwitchWhenOffensiveStatsAreLow(struct Pokemon* party)
{
	//-3 in offensive stat
	//Sweeper class
	//Only has moves of physicality
	//Can't knock out
	//Minimal damage switch
	//Add something about pivoting

	if (gNewBS->ai.switchingCooldown[gActiveBattler]) //Just switched in
		return FALSE;

	u8 class = GetBankFightingStyle(gActiveBattler);

	if (!IsDynamaxed(gActiveBattler) //Don't waste the Dynamax
	&& IsClassDamager(class) //Role is to dish out as much damage as possible
	&& (STAT_STAGE(gActiveBattler, STAT_STAGE_ATK) <= OFFENSIVE_STAT_MIN_NUM
	 || STAT_STAGE(gActiveBattler, STAT_STAGE_SPATK) <= OFFENSIVE_STAT_MIN_NUM
	 || STAT_STAGE(gActiveBattler, STAT_STAGE_ACC) <= OFFENSIVE_STAT_MIN_NUM)) //Has at least one bad offensive stat (placed here to save runtime)
	{
		u8 i;
		u8 battlerIn1, battlerIn2;
		u8 foe1, foe2;
		bool8 hasPhysicalMove, hasSpecialMove;
		u8 moveLimitations = CheckMoveLimitations(gActiveBattler, 0, 0xFF);
		LoadBattlersAndFoes(&battlerIn1, &battlerIn2, &foe1, &foe2);

		if (IsValidMovePrediction(gActiveBattler, foe1) == MOVE_HEARTSWAP)
			return FALSE; //Don't switch if going to trade the bad stats away
		else if (IS_DOUBLE_BATTLE && IsValidMovePrediction(gActiveBattler, foe2) == MOVE_HEARTSWAP)
			return FALSE; //Don't switch if going to trade the bad stats away

		//Check physical and special moves
		for (i = 0, hasPhysicalMove = FALSE, hasSpecialMove = FALSE; i < MAX_MON_MOVES; ++i)
		{
			u16 move = GetBattleMonMove(gActiveBattler, i);
			if (move == MOVE_NONE)
				break;

			if (!(gBitTable[i] & moveLimitations) //Can use move
			&& gBattleMoves[move].power != 0) //Move actually does damage
			{
				u8 split = CalcMoveSplit(move, gActiveBattler, foe1);

				if (split == SPLIT_PHYSICAL)
				{
					if (gBattleMoves[move].effect != EFFECT_COUNTER && move != MOVE_BODYPRESS && move != MOVE_FOULPLAY)
						hasPhysicalMove = TRUE;
				}
				else if (split == SPLIT_SPECIAL)
				{
					if (gBattleMoves[move].effect != EFFECT_MIRROR_COAT)
						hasSpecialMove = TRUE;
				}
			}
		}

		if (STAT_STAGE(gActiveBattler, STAT_STAGE_ATK) <= OFFENSIVE_STAT_MIN_NUM //Attack is at -3 or less
		&& hasPhysicalMove && !hasSpecialMove) //Only has physical moves
		{
			//Cleared check
		}
		else if (STAT_STAGE(gActiveBattler, STAT_STAGE_SPATK) <= OFFENSIVE_STAT_MIN_NUM //Sp. Atk is at -3 or less
		&& hasSpecialMove && !hasPhysicalMove) //Only has special moves
		{
			//Cleared check
		}
		else if (STAT_STAGE(gActiveBattler, STAT_STAGE_ATK) <= OFFENSIVE_STAT_MIN_NUM //Attack is at -3 or less
		&& STAT_STAGE(gActiveBattler, STAT_STAGE_SPATK) <= OFFENSIVE_STAT_MIN_NUM //Sp. Atk is also at -3 or less
		&& hasPhysicalMove && hasSpecialMove) //Has both kinds of moves
		{
			//Cleared check
		}
		else if (STAT_STAGE(gActiveBattler, STAT_STAGE_ACC) <= OFFENSIVE_STAT_MIN_NUM //Accuracy is at -3 or less
		&& AllMovesInMovesetWithAccuracyLessThan(gActiveBattler, foe1, 70, TRUE) //Can't use any move with Acc 70 or more
		&& (!IS_DOUBLE_BATTLE || AllMovesInMovesetWithAccuracyLessThan(gActiveBattler, foe2, 70, TRUE)) //And can't use any move with Acc 70 or more against second foe in doubles
		&& !IsClassStall(class)) //Doesn't really matter if stall mons can hit the foe as long as they can stall out
		{
			//Cleared check
		}
		else //Didn't clear stat stage check
			return FALSE; //Switch not worth it

		bool8 willPivot = !IS_DOUBLE_BATTLE && FastPivotingMoveInMovesetThatAffects(gActiveBattler, foe1);

		if ((!CanKnockOut(foe1, gActiveBattler) && !(IS_DOUBLE_BATTLE && CanKnockOut(foe2, gActiveBattler))) //Can't knock out only (or both) foe
		|| STAT_STAGE(gActiveBattler, STAT_STAGE_ACC) <= OFFENSIVE_STAT_MIN_NUM) //Or just has really bad accuracy
		{
			u8 bestMonId = GetMostSuitableMonToSwitchIntoByParty(party);
			u8 switchFlags = GetMostSuitableMonToSwitchIntoFlags();

			if (switchFlags & (SWITCHING_FLAG_WALLS_FOE | SWITCHING_FLAG_RESIST_ALL_MOVES) //New mon can take a hit
			|| (IS_SINGLE_BATTLE && switchFlags & SWITCHING_FLAG_OUTSPEEDS && PredictedMoveWontDoTooMuchToMon(gActiveBattler, &party[bestMonId], foe1, switchFlags))) //Can take at least the next hit and follow up
			{
				ConfirmAISwitch(bestMonId, willPivot);
				return TRUE;
			}
		}
	}

	return FALSE;
}

static bool8 ShouldSaveSweeperForLaterHelper(struct Pokemon* party, u8 bestMonId, u8 switchFlags, u8 foe, bool8 willPivot)
{
	if (bestMonId >= PARTY_SIZE)
		return FALSE;

	u8 wantedFlags = (SWITCHING_FLAG_OUTSPEEDS | SWITCHING_FLAG_KO_FOE);
	struct Pokemon* mon = &party[bestMonId];

	if ((wantedFlags == (switchFlags & wantedFlags) //New mon will go first and KO
	&& PredictedMoveWontDoTooMuchToMon(gActiveBattler, mon, foe, switchFlags)) //And it can take the predicted hit
	|| (PredictedMoveKOsSelfDueToContact(gActiveBattler, mon, foe) //The switch in will KO the foe when it takes contact damage from the switched-in mon
	 && PredictedMoveWontKOMon(gActiveBattler, mon, foe))) //The switched-in mon can survive the hit
	{
		ConfirmAISwitch(bestMonId, willPivot);
		return TRUE;
	}

	return FALSE;
}

static bool8 ShouldSaveChoiceSweeper(u8 monId, u8 switchFlags, struct Pokemon* party)
{
	if (monId >= PARTY_SIZE)
		return FALSE;

	u8 wantedFlags = (SWITCHING_FLAG_OUTSPEEDS | SWITCHING_FLAG_KO_FOE);

	//Check if should switch to the best mon
	if (switchFlags & SWITCHING_FLAG_FAINTS_FROM_FOE) //The mon faints from the current foe
	{
		if (!(switchFlags & wantedFlags)) //And it couldn't outspeed and KO if the current mon was sacked
		{
			SWITCH_TO_MON:
			//Switch to mon and save choice sweeper
			gBattleStruct->switchoutIndex[SIDE(gActiveBattler)] = monId; //Sack the mon"
			EmitTwoReturnValues(1, ACTION_SWITCH, 0);
			return TRUE;
		}
	}
	else if (PredictedMoveWontDoTooMuchToMon(gActiveBattler, &party[monId], FOE(gActiveBattler), switchFlags))
		goto SWITCH_TO_MON;

	return FALSE;
}

static bool8 ShouldSaveSweeperForLater(struct Pokemon* party)
{
	//The smart AI should switch out if it's trying to sweep and
	//has a better mon out back than the one already on the field.
	//This is the partner to ShouldSwitchToAvoidDeath, as that function
	//only works on mons that are slower.

	if (gNewBS->ai.switchingCooldown[gActiveBattler]) //Just switched in
		return FALSE;

	u8 ability;
	u8 foe = FOE(gActiveBattler);
	u16 foeMovePrediction = IsValidMovePrediction(foe, gActiveBattler);
	u16 randVal;

	if (IS_SINGLE_BATTLE //Not good for Doubles
	&& AI_THINKING_STRUCT->aiFlags > AI_SCRIPT_CHECK_BAD_MOVE //Has smart AI
	#ifdef UNBOUND
	&& AISaveSweeperForLaterDifficultyCheck() //And allowed to use this logic on the current Difficulty
	#endif
	&& !IsDynamaxed(gActiveBattler) //Don't waste the Dynamax
	#ifndef FROSTBITE
	&& !(gBattleMons[gActiveBattler].status1 & STATUS1_FREEZE) //Better to not try to save frozen target
	#endif
	&& (!(gBattleMons[gActiveBattler].status1 & STATUS1_PARALYSIS) //Better to not try to save paralyzed target
	 || ((ability = ABILITY(gActiveBattler)) == ABILITY_QUICKFEET || ability == ABILITY_GUTS)) //Unless it has Quick Feet or Guts
	&& IsClassDamager(GetBankFightingStyle(gActiveBattler)) //Role is to dish out as much damage as possible
	&&
	(
		//OPTION A:
		!CanKnockOut(gActiveBattler, foe) //But this powerhouse can't KO right now

		//OPTION B:
		|| (foeMovePrediction == MOVE_FAKEOUT //The AI can KO but the foe will go first with Fake Out
	     && CanBeFlinched(gActiveBattler, foe,
		                  IsTargetAbilityIgnored(ABILITY(gActiveBattler), ABILITY(foe), foeMovePrediction) ? ABILITY_NONE : ABILITY(gActiveBattler),
		                  foeMovePrediction) //Fake Out will cause a flinch
	     && GetFinalAIMoveDamage(foeMovePrediction, foe, gActiveBattler, 1, NULL) >= gBattleMons[gActiveBattler].maxHP / 2) //And deal a ton of damage
	)
	&& (!IS_BEHIND_SUBSTITUTE(gActiveBattler) || DamagingMoveThaCanBreakThroughSubstituteInMoveset(foe, gActiveBattler)) //It's not behind a Substitute
	&& !WillTakeSignificantDamageFromEntryHazards(gActiveBattler, 4) //Will take less than 25% damage on reentry
	&& !(IsTrapped(foe, TRUE) && IsTakingSecondaryDamage(foe, TRUE)) //This mon isn't keeping the foe locked in taking damage
	&&
	(
		//OPTION A:
		(CanKnockOut(foe, gActiveBattler) //Foe is likely slower and will KO
		 && !CanHealFirstToPreventKnockOut(gActiveBattler, foe)) //And the AI can't just heal to avoid the KO

		//OPTION B:
		|| (!AnyUsefulOffseniveStatIsRaised(gActiveBattler) //It isn't invested in useful buffs
		 && STAT_STAGE(gActiveBattler, STAT_STAGE_EVASION) < 6 + 3 //Including +3 Evasion
		 && !(IsTakingSecondaryDamage(foe, TRUE) && IsTrapped(foe, TRUE)) //And it isn't necessary to stay in so the foe will take trap damage
		 && !OffensiveSetupMoveInMoveset(gActiveBattler, foe) //It can't set up stats either
		 &&
		 (
			//OPTION B1:
			(GetMostSuitableMonToSwitchIntoFlags() & SWITCHING_FLAG_KO_FOE) //The new mon can KO (helps against PP stallers)

			//OPTION B2:
			|| ((randVal = AIRandom()) & 1 //Only perform this switch 50% of the time to throw off the player
			 && (!Can2HKO(gActiveBattler, foe) || (randVal & 2)) //75% of the time only switch if this mon can't KO the foe in 2 turns (the other 25% of the time switch anyway)
			 && !ResistsAllMoves(foe, gActiveBattler) //It doesn't already resist all of the foe's moves (Since a mon that resists all moves will be chosen, don't get into an infinite loop if this mon already does).
			 && !IsStrongestMoveHPDrainingMove(gActiveBattler, foe)) //And it can't chip away at the foe's HP by healing off damage it takes
		))
	))
	{
		u16 movePrediction = IsValidMovePrediction(gActiveBattler, foe);
		if ((movePrediction == MOVE_FAKEOUT && CanFlinch(foe, ABILITY(foe))) //Assume it was chosen because Flinching would work
		|| gBattleMoves[movePrediction].effect == EFFECT_PROTECT)
			return FALSE; //Use the move and then switch

		bool8 willPivot = FastPivotingMoveInMovesetThatAffects(gActiveBattler, foe); //U-Turn/Volt Switch switch on their own

		if (CanKnockOut(foe, gActiveBattler)) //Only in case where foe can KO AI mon (better than ShouldSwitchToAvoidDeath because this also accounts for slower foes)
		{
			//Confirm AI's Destiny Bond isn't active and won't be active
			if (MoveWouldHitFirst(movePrediction, gActiveBattler, foe))
			{
				if (gBattleMoves[movePrediction].effect == EFFECT_DESTINY_BOND)
					return FALSE; //Take the L and bring the foe down with you
			}
			else //Slower
			{
				if (gBattleMons[gActiveBattler].status2 & STATUS2_DESTINY_BOND) //Destiny Bond already active
					return FALSE; //Take the L and bring the foe down with you
			}

			//Try to switch out to the best mon
			u8 bestMonId, secondBestMonId, switchFlags, secondBestSwitchFlags;

			//Check best mon can come in and KO the foe
			bestMonId = GetMostSuitableMonToSwitchIntoByParty(party);
			switchFlags = GetMostSuitableMonToSwitchIntoFlags();
			if (ShouldSaveSweeperForLaterHelper(party, bestMonId, switchFlags, foe, willPivot))
				return TRUE;

			//Check second best mon can come in and KO the foe
			secondBestMonId = GetSecondMostSuitableMonToSwitchIntoByParty(party);
			secondBestSwitchFlags = GetSecondMostSuitableMonToSwitchIntoFlags();
			if (ShouldSaveSweeperForLaterHelper(party, secondBestMonId, secondBestSwitchFlags, foe, willPivot))
				return TRUE;

			//Check if it's worth breaking a Choice Lock
			if (CHOICED_MOVE(gActiveBattler) != MOVE_NONE //The AI is choice locked
			&& CanBeChoiceLocked(gActiveBattler)) //And the choice lock is legit
			{
				u16 backupChoiceMove = CHOICED_MOVE(gActiveBattler); //Backup the choice move
				CHOICED_MOVE(gActiveBattler) = MOVE_NONE; //Wipe the move locked into so it can compare other moves
				s16 strongestMove = CalcStrongestMove(gActiveBattler, foe, FALSE); //Get the strongest move assuming any move could be chosen
				CHOICED_MOVE(gActiveBattler) = backupChoiceMove; //Restore the old choiced move

				if (GetFinalAIMoveDamage(strongestMove, gActiveBattler, foe, 1, NULL) >= gBattleMons[foe].hp //The strongest move could KO the current foe
				&& MoveWouldHitFirst(strongestMove, gActiveBattler, foe)) //And it would hit first against the move the foe is likely to use
				{
					//Switch the AI out becuase when it switches back in it can KO the current foe

					if (ShouldSaveChoiceSweeper(bestMonId, switchFlags, party) //Handles the emit in the function
					||  ShouldSaveChoiceSweeper(secondBestMonId, secondBestSwitchFlags, party))
						return TRUE;

					//Sack the current mon and the next mon that comes in will do clean up
				}
			}
		}

		if (SwitchToBestResistMon(party, willPivot, foe))
			return TRUE;
	}

	return FALSE;
}

/*
In order:
70: KO Foe + Resist/Immune to All Moves + Revenge Kill + Outspeeds
62: KO Foe + Resist/Immune to All Moves + Outspeeds
56: KO Foe + Resist/Immune to All Moves + Revenge Kill
55: KO Foe + Outspeeds + Revenge Kill + Walls Foe 
53: KO Foe + Outspeeds + Revenge Kill
52: KO Foe + Outspeeds + Revenge Kill + Weak to Move
48: KO Foe + Resist/Immune to All Moves
45: KO Foe + Outspeeds
41: KO Foe + Revenge Kill + Walls Foe
39: KO Foe + Revenge Kill
38: KO Foe + Revenge Kill + Weak to Move
33: KO Foe + Walls Foe
 || Resist/Immune to All Moves + Outspeeds + 2HKO Foe
31: KO Foe
 || Resist/Immune to All Moves + Outspeeds
30: KO Foe + Weak to Move
19: Resist/Immune to All Moves + 2HKO Foe
18: Outspeeds + 2HKO Foe + Walls Foe
17: Resist/Immune to All Moves
16: Outspeeds + 2HKO Foe
15: Outspeeds + 2HKO Foe + Weak to Move
14: Outspeeds
04: Walls Foe + 2HKO Foe
02: Walls Foe
 || 2HKO Foe
*/

//Add logic about switching in a partner to resist spread move in doubles
u8 GetMostSuitableMonToSwitchInto(void)
{
	u8 firstId, lastId;
	struct Pokemon* party = LoadPartyRange(gActiveBattler, &firstId, & lastId);
	return GetMostSuitableMonToSwitchIntoByParty(party);
}

u8 GetMostSuitableMonToSwitchIntoByParty(const struct Pokemon* party)
{
	u8 battlerIn1, battlerIn2;
	u8 foe1, foe2;
	LoadBattlersAndFoes(&battlerIn1, &battlerIn2, &foe1, &foe2);

	CalcMostSuitableMonSwitchIfNecessary();
	u8 option1 = gNewBS->ai.bestMonIdToSwitchInto[gActiveBattler][0];
	u8 option2 = gNewBS->ai.bestMonIdToSwitchInto[gActiveBattler][1];

	if (option1 == PARTY_SIZE
	||  GetMonData(&party[option1], MON_DATA_HP, NULL) == 0) //Best mon is dead
	{
		CalcMostSuitableMonToSwitchInto();
		gNewBS->ai.calculatedAISwitchings[gActiveBattler] = TRUE;

		if (!BankSideHasTwoTrainers(gActiveBattler))
			gNewBS->ai.calculatedAISwitchings[PARTNER(gActiveBattler)] = TRUE;
	}

	if (option1 == gBattleStruct->monToSwitchIntoId[battlerIn1]
	||  option1 == gBattleStruct->monToSwitchIntoId[battlerIn2])
		return option2;

	return option1;
}

u8 GetSecondMostSuitableMonToSwitchInto(void)
{
	u8 firstId, lastId;
	struct Pokemon* party = LoadPartyRange(gActiveBattler, &firstId, & lastId);
	return GetSecondMostSuitableMonToSwitchIntoByParty(party);
}

u8 GetSecondMostSuitableMonToSwitchIntoByParty(const struct Pokemon* party)
{
	u8 battlerIn1, battlerIn2;
	u8 foe1, foe2;
	LoadBattlersAndFoes(&battlerIn1, &battlerIn2, &foe1, &foe2);

	CalcMostSuitableMonSwitchIfNecessary();
	u8 option2 = gNewBS->ai.bestMonIdToSwitchInto[gActiveBattler][1];

	if (option2 == PARTY_SIZE
	|| option2 == gBattleStruct->monToSwitchIntoId[battlerIn1] //Second best mon 2 has already been chosen
	|| option2 == gBattleStruct->monToSwitchIntoId[battlerIn2]
	|| GetMonData(&party[option2], MON_DATA_HP, NULL) == 0) //Second best mon is dead
		return PARTY_SIZE;

	return option2;
}

s16 GetMostSuitableMonToSwitchIntoScore(void)
{
	u8 battlerIn1, battlerIn2;
	u8 foe1, foe2;
	LoadBattlersAndFoes(&battlerIn1, &battlerIn2, &foe1, &foe2);

	CalcMostSuitableMonSwitchIfNecessary();
	u8 option1 = gNewBS->ai.bestMonIdToSwitchInto[gActiveBattler][0];

	if (option1 == gBattleStruct->monToSwitchIntoId[battlerIn1]
	||  option1 == gBattleStruct->monToSwitchIntoId[battlerIn2])
		return gNewBS->ai.bestMonIdToSwitchIntoScores[gActiveBattler][1];

	return gNewBS->ai.bestMonIdToSwitchIntoScores[gActiveBattler][0];
}

u8 GetMostSuitableMonToSwitchIntoFlags(void)
{
	u8 battlerIn1, battlerIn2;
	u8 foe1, foe2;
	LoadBattlersAndFoes(&battlerIn1, &battlerIn2, &foe1, &foe2);

	CalcMostSuitableMonSwitchIfNecessary();
	u8 option1 = gNewBS->ai.bestMonIdToSwitchInto[gActiveBattler][0];

	if (option1 == gBattleStruct->monToSwitchIntoId[battlerIn1]
	||  option1 == gBattleStruct->monToSwitchIntoId[battlerIn2])
		return gNewBS->ai.bestMonIdToSwitchIntoFlags[gActiveBattler][1];

	return gNewBS->ai.bestMonIdToSwitchIntoFlags[gActiveBattler][0];
}

u8 GetSecondMostSuitableMonToSwitchIntoFlags(void)
{
	u8 battlerIn1, battlerIn2;
	u8 foe1, foe2;
	LoadBattlersAndFoes(&battlerIn1, &battlerIn2, &foe1, &foe2);

	CalcMostSuitableMonSwitchIfNecessary();
	u8 option2 = gNewBS->ai.bestMonIdToSwitchInto[gActiveBattler][1];

	if (option2 == gBattleStruct->monToSwitchIntoId[battlerIn1]
	||  option2 == gBattleStruct->monToSwitchIntoId[battlerIn2])
		return 0;

	return gNewBS->ai.bestMonIdToSwitchIntoFlags[gActiveBattler][1];
}

static void CalcMostSuitableMonSwitchIfNecessary(void)
{
	//Calc switching only if necessary
	if (!gNewBS->ai.calculatedAISwitchings[gActiveBattler] && BATTLER_ALIVE(gActiveBattler)) //So Multi Battles still work properly
	{
		CalcMostSuitableMonToSwitchInto();
		gNewBS->ai.calculatedAISwitchings[gActiveBattler] = TRUE;

		if (!BankSideHasTwoTrainers(gActiveBattler))
			gNewBS->ai.calculatedAISwitchings[PARTNER(gActiveBattler)] = TRUE;
	}
}

u8 CalcMostSuitableMonToSwitchInto(void)
{
	//u8 opposingBattler;
	u32 i, j, k;
	u8 bestMonId, secondBestMonId;
	u8 battlerIn1, battlerIn2;
	u8 foe, foe1, foe2;
	u8 firstId, lastId; // + 1
	u16 move;
	struct Pokemon* party;

	u8 lastValidMon = PARTY_SIZE;
	u8 secondLastValidMon = PARTY_SIZE;
	s16 scores[PARTY_SIZE] = {0};
	u8 flags[PARTY_SIZE] = {0};
	bool8 canNegateToxicSpikes[PARTY_SIZE] = {FALSE};
	bool8 canRemoveHazards[PARTY_SIZE] = {FALSE};

	if (gBattleStruct->monToSwitchIntoId[gActiveBattler] != PARTY_SIZE)
		return gBattleStruct->monToSwitchIntoId[gActiveBattler];

	LoadBattlersAndFoes(&battlerIn1, &battlerIn2, &foe1, &foe2);
	party = LoadPartyRange(gActiveBattler, &firstId, & lastId);

	//Check if point even running calcs
	u8 count = 0;
	for (i = firstId; i < lastId; ++i)
	{
		if (party[i].species != SPECIES_NONE
		&& !GetMonData(&party[i], MON_DATA_IS_EGG, NULL)
		&& party[i].hp > 0
		&& gBattlerPartyIndexes[battlerIn1] != i
		&& gBattlerPartyIndexes[battlerIn2] != i
		&& i != gBattleStruct->monToSwitchIntoId[battlerIn1]
		&& i != gBattleStruct->monToSwitchIntoId[battlerIn2])
		{
			++count;
		}
	}

	if (count == 0)
	{
		ResetBestMonToSwitchInto(gActiveBattler);
		return PARTY_SIZE;
	}

	//Try to counter a locked move
	/*u8 option = GetBestPartyNumberForSemiInvulnerableLockedMoveCalcs(opposingBattler, opposingBattler, TRUE);
	if (option != PARTY_SIZE)
		return option;*/

	//Find the mon who is most suitable
	bestMonId = PARTY_SIZE;
	secondBestMonId = PARTY_SIZE;
	for (i = firstId; i < lastId; ++i)
	{
		struct Pokemon* consideredMon = &party[i];

		if (consideredMon->species != SPECIES_NONE
		&& consideredMon->hp > 0
		&& !GetMonData(consideredMon, MON_DATA_IS_EGG, NULL)
		&& (!(consideredMon->condition & (STATUS1_SLEEP
		#ifndef FROSTBITE
		                                | STATUS1_FREEZE
		#endif
		                                 )) || consideredMon->condition == 1) //Not asleep or is about to wake up
		&& i != gBattlerPartyIndexes[battlerIn1]
		&& i != gBattlerPartyIndexes[battlerIn2]
		&& i != gBattleStruct->monToSwitchIntoId[battlerIn1]
		&& i != gBattleStruct->monToSwitchIntoId[battlerIn2])
		{
			u8 foes[] = {foe1, foe2};
			u8 moveLimitations = CheckMoveLimitationsFromParty(consideredMon, 0, 0xFF);
			u8 ability = GetMonAbilityAfterTrace(consideredMon, foe1);
			ability = TryReplaceImposterAbility(ability, gActiveBattler);
			u32 speed = SpeedCalcMon(SIDE(gActiveBattler), consideredMon);
			bool8 isOneFoeOnField = IS_SINGLE_BATTLE || ViableMonCountFromBank(foe1) == 1;
			secondLastValidMon = lastValidMon;
			lastValidMon = i;
			canNegateToxicSpikes[i] = IsMonOfType(consideredMon, TYPE_POISON) && CheckMonGrounding(consideredMon);

			if (WillFaintFromEntryHazards(consideredMon, SIDE(gActiveBattler)))
				continue; //Don't switch in the mon if it'll faint on reentry

			struct DamageCalc damageData = {0};
			damageData.bankAtk = gActiveBattler;
			damageData.monAtk = consideredMon;
			PopulateDamageCalcStructWithBaseAttackerData(&damageData);
			s32 passiveRecovery = GetMonPassiveRecovery(consideredMon, gActiveBattler);
			u16 wishRecovery = GetWishHPRecovery(gActiveBattler, FALSE);

			for (j = 0; j < gBattlersCount / 2; ++j) //Loop through all enemies on field
			{
				foe = foes[j];

				if (BATTLER_ALIVE(foe)
				&& (j == 0 || foes[0] != foes[j])) //Don't check same opponent twice
				{
					u8 typeEffectiveness = 0;
					bool8 isWeakToMove = FALSE;
					bool8 faintsFromMove = FALSE;
					bool8 isNormalEffectiveness = 0; //Records how many moves the foe has that can do normal damage
					//bool8 isNotVeryEffective = 0; //Records how many moves the foe has that can do minimal damage

					damageData.bankDef = foe;
					PopulateDamageCalcStructWithBaseDefenderData(&damageData);

					//Check Speed Capabilities
					if (speed >= SpeedCalc(foe)) //The considered mon is faster than the enemy
					{
						scores[i] += SWITCHING_INCREASE_OUTSPEEDS;
						flags[i] |= SWITCHING_FLAG_OUTSPEEDS;
					}

					//Check Offensive Capabilities
					if (CanKnockOutFromParty(consideredMon, foe, &damageData))
					{
						scores[i] += SWITCHING_INCREASE_KO_FOE;
						flags[i] |= SWITCHING_FLAG_KO_FOE;

						if (IsMoxieAbility(ability)
						|| (IsTrappedByAbility(foe, ability))) //Basically guaranteed kill
						{
							scores[i] += SWITCHING_INCREASE_REVENGE_KILL;
							flags[i] |= SWITCHING_FLAG_REVENGE_KILL;
						}
						else //No revenge killing Ability, so check moves
						{
							u8 numUsableMoves = 0;
							u16 moves[MAX_MON_MOVES];
							bool8 isPriority[MAX_MON_MOVES];
	
							//First build list of moves with priority moves at the front
							for (k = 0; k < MAX_MON_MOVES; ++k)
							{
								s8 movePriority;

								if (gBitTable[k] & moveLimitations)
									continue;

								if (damageData.atkImposter)
								{
									move = GetBattleMonMove(damageData.atkImposterBank, k);
									movePriority = PriorityCalc(damageData.atkImposterBank, ACTION_USE_MOVE, move);
								}
								else
								{
									move = GetMonData(consideredMon, MON_DATA_MOVE1 + k, 0);
									movePriority = PriorityCalcMon(consideredMon, move);
								}

								if (movePriority > 0) //Move has priority
								{
									//Add move at the beginning of the list
									for (u8 m = numUsableMoves; m > 0; --m)
									{
										moves[m] = moves[m - 1]; //Shift moves down
										isPriority[m] = isPriority[m - 1];
									}

									moves[0] = move;
									isPriority[0] = TRUE;
									numUsableMoves++;
								}
								else //Not priority move
								{
									//Tack move on to the end of the list
									isPriority[numUsableMoves] = FALSE;
									moves[numUsableMoves++] = move;
								}
							}

							//Then go through the moves
							for (k = 0; k < numUsableMoves; ++k)
							{
								move = moves[k];

								if (gBattleMoves[move].effect == EFFECT_RAPID_SPIN //Includes Defog
								&&  gSideStatuses[SIDE(gActiveBattler)] & SIDE_STATUS_SPIKES)
								{
									if (IS_SINGLE_BATTLE) //Single Battle
										canRemoveHazards[i] = ViableMonCountFromBank(gActiveBattler) >= 2; //There's a point in removing the hazards
									else //Double Battle
										canRemoveHazards[i] = ViableMonCountFromBank(gActiveBattler) >= 3; //There's a point in removing the hazards
								}

								if (SPLIT(move) != SPLIT_STATUS
								&& isPriority[k] > 0
								&& MoveKnocksOutFromParty(move, consideredMon, foe, &damageData)) //Priority move that KOs
								{
									scores[i] += SWITCHING_INCREASE_REVENGE_KILL;
									flags[i] |= SWITCHING_FLAG_REVENGE_KILL;

									if (!(flags[i] & SWITCHING_FLAG_OUTSPEEDS)) //Doesn't already get the boost for outspeeding
									{
										//Make the AI think it outspeeds so it prioritizes this mon
										scores[i] += SWITCHING_INCREASE_OUTSPEEDS;
										flags[i] |= SWITCHING_FLAG_OUTSPEEDS;
									}

									break;
								}
								else if (move == MOVE_FELLSTINGER //Gets an attack boost when a foe is KOd
								|| move == MOVE_PURSUIT) //KOs the foe before it can switch out
								{
									if (MoveKnocksOutFromParty(move, consideredMon, foe, &damageData))
									{
										scores[i] += SWITCHING_INCREASE_REVENGE_KILL;
										flags[i] |= SWITCHING_FLAG_REVENGE_KILL;
										break;
									}
								}
							}
						}
					}
					else //This mon can't KO the foe
					{
						bool8 hasUsableMove = FALSE;

						for (k = 0; k < MAX_MON_MOVES; ++k)
						{
							if (gBitTable[k] & moveLimitations)
								continue;

							if (damageData.atkImposter)
								move = GetBattleMonMove(damageData.atkImposterBank, k);
							else
								move = GetMonData(consideredMon, MON_DATA_MOVE1 + k, 0);

							hasUsableMove = TRUE;

							if (gBattleMoves[move].effect == EFFECT_RAPID_SPIN //Includes Defog
							&&  gSideStatuses[SIDE(gActiveBattler)] & SIDE_STATUS_SPIKES)
							{
								if (!IS_DOUBLE_BATTLE) //Single Battle
									canRemoveHazards[i] = ViableMonCountFromBank(gActiveBattler) >= 2; //There's a point in removing the hazards
								else //Double Battle
									canRemoveHazards[i] = ViableMonCountFromBank(gActiveBattler) >= 3; //There's a point in removing the hazards
							}

							if (move != MOVE_NONE
							&& SPLIT(move) != SPLIT_STATUS
							//&& TypeCalc(move, gActiveBattler, foe, consideredMon) & MOVE_RESULT_SUPER_EFFECTIVE)
							&& gNewBS->ai.monMaxDamage[SIDE(gActiveBattler)][i][foe] >= gBattleMons[foe].hp / 2) //Move can 2HKO
							{
								scores[i] += SWITCHING_INCREASE_CAN_2HKO; //Only checked if can't KO
								flags[i] |= SWITCHING_FLAG_CAN_2HKO;
								break;
							}
						}

						if (!hasUsableMove)
						{
							scores[i] = -1; //Bad idea to switch to this mon
							goto CHECK_NEXT_MON;
						}
					}

					//Check Defensive Capabilities
					u16 moves[MAX_MON_MOVES];
					bool8 isPriority[MAX_MON_MOVES];
					u8 numUsableMoves = 0;
					bool8 physMoveInMoveset = FALSE;
					bool8 specMoveInMoveset = FALSE;
					u8 foeMoveLimitations = CheckMoveLimitations(foe, 0, 0xFF);
					struct DamageCalc foeDamageData = {0};
					foeDamageData.bankAtk = foe;
					foeDamageData.bankDef = gActiveBattler; //For the side
					foeDamageData.monDef = consideredMon;
					PopulateDamageCalcStructWithBaseAttackerData(&foeDamageData);
					PopulateDamageCalcStructWithBaseDefenderData(&foeDamageData);

					s32 hpOnSwitchIn = consideredMon->hp - GetMonEntryHazardDamage(consideredMon, SIDE(gActiveBattler));
					if (hpOnSwitchIn < 0)
						hpOnSwitchIn = 0;

					foeDamageData.defHP = hpOnSwitchIn; //Adjust for hazard damage

					for (k = 0; k < MAX_MON_MOVES; ++k) //Get a list of all usable moves with priority moves at the front
					{
						move = GetBattleMonMove(foe, k);

						if (move == MOVE_NONE)
							break; //End of moveset

						if (gBattleMons[foe].status2 & STATUS2_MULTIPLETURNS
						&&  MoveInMoveset(gLockedMoves[foe], foe)
						&&  move != gLockedMoves[foe])
							continue; //Skip non-locked moves

						if (!(gBitTable[k] & foeMoveLimitations))
						{
							u8 split = CalcMoveSplit(move, foe, foe);
							if (split == SPLIT_PHYSICAL)
								physMoveInMoveset = TRUE;
							else if (split == SPLIT_SPECIAL)
								specMoveInMoveset = TRUE;
							else
								continue; //Skip status moves

							if (PriorityCalc(foe, ACTION_USE_MOVE, move) > 0) //Move has priority
							{
								//Add move at the beginning of the list
								for (u8 m = numUsableMoves; m > 0; --m)
								{
									moves[m] = moves[m - 1]; //Shift moves down
									isPriority[m] = isPriority[m - 1];
								}

								moves[0] = move;
								isPriority[0] = TRUE;
								numUsableMoves++;
							}
							else //Not priority move
							{
								//Tack move on to the end of the list
								isPriority[numUsableMoves] = FALSE;
								moves[numUsableMoves++] = move;
							}
						}
					}

					for (k = 0; k < numUsableMoves; ++k) //Check how much damage each move does
					{
						move = moves[k];

						if (isOneFoeOnField && AI_THINKING_STRUCT->aiFlags > AI_SCRIPT_CHECK_BAD_MOVE) //Only smart AI gets this calc to not slow down regular Trainers
						{
							u32 firstHitDmg, otherHitsDmg;
							firstHitDmg = otherHitsDmg = AI_CalcMonDefDmg(foe, gActiveBattler, move, consideredMon, &foeDamageData); //VERY SLOW

							//Check if KO in one shot
							if (IsAffectedByDisguse(foeDamageData.defAbility, foeDamageData.defSpecies, foeDamageData.moveSplit))
							{
								if (foeDamageData.defAbility == ABILITY_DISGUISE) //Disguise only - not Ice Face
									firstHitDmg = foeDamageData.defMaxHP / 8;
								else
									firstHitDmg = 0;
							}
							else if (firstHitDmg >= (u16) hpOnSwitchIn) //TODO eventually: factor in secondary damage (eg. weather, poison)
							{
								faintsFromMove = TRUE;

								if (flags[i] & SWITCHING_FLAG_OUTSPEEDS //This mon would normally go first
								&& isPriority[k] > 0 //But the foe has a move that KOs and has priority
								&& (gTerrainType != PSYCHIC_TERRAIN || !CheckMonGrounding(consideredMon))) //And the mon can be hit by priority
									flags[i] &= ~SWITCHING_FLAG_OUTSPEEDS; //Then remove the outspeed flag so the AI doesn't think it can outspeed
									//Potential UB: AI mon outspeed BECAUSE of a priority move it has. If it also outspeeds normally, this shouldn't matter since its priority move will still go first.
									//TODO fix this

								break; //Only need 1 check for this to pass - priority moves are sorted first so a break here won't cause problems
							}

							//Check how weak to move this mon is
							if (IsMonDamageHalvedDueToFullHP(consideredMon, foeDamageData.defAbility, move, foeDamageData.atkAbility))	
								otherHitsDmg *= 2; //Only factor in Multiscale for first hit

							s32 adjustedHP = MathMin(hpOnSwitchIn + wishRecovery, consideredMon->maxHP); //Factor in Wish up to max HP if possible
							if (adjustedHP + passiveRecovery <= (s32) (firstHitDmg + otherHitsDmg)) //Move could 2HKO new mon
							{
								isWeakToMove = TRUE;
							}
							else if (adjustedHP + (passiveRecovery * 2) <= (s32) (firstHitDmg + otherHitsDmg * 2)) //Move could 3HKO mon
							{
								++isNormalEffectiveness;
							}
							//else //Move does less than a third of HP remaining
								//++isNotVeryEffective;
						}
						else //In Doubles or regular Trainers use type matchups because too slow otherwise
						{
							typeEffectiveness = AI_TypeCalc(move, foe, gActiveBattler, consideredMon);

							if (typeEffectiveness & MOVE_RESULT_SUPER_EFFECTIVE)
							{
								isWeakToMove = TRUE;
								break; //Only need 1 check for this to pass
							}
							else if (typeEffectiveness == 0)
								++isNormalEffectiveness;
							//else
								//++isNotVeryEffective; //By default it either resists or is immune
						}
					}

					if (faintsFromMove) //At least one move KOs the considered mon
					{
						if (IS_DOUBLE_BATTLE)
							goto WEAK_TO_MOVE_DECREMENT;

						if (!(flags[i] & SWITCHING_FLAG_OUTSPEEDS)) //Mon doesn't outspeed foe
						{
							flags[i] |= SWITCHING_FLAG_FAINTS_FROM_FOE;

							if (scores[i] >= SWITCHING_DECREASE_FAINTS_FROM_FOE)
								scores[i] -= SWITCHING_DECREASE_FAINTS_FROM_FOE;
							else
								scores[i] = 0;
						}
						else if (!(flags[i] & SWITCHING_FLAG_KO_FOE)) //Mon outspeeds foe but wouldn't KO
						{
							if (scores[i] >= SWITCHING_DECREASE_FAINTS_FROM_FOE_BUT_OUTSPEEDS)
								scores[i] -= SWITCHING_DECREASE_FAINTS_FROM_FOE_BUT_OUTSPEEDS;
							else
								scores[i] = 0;
						}
						else //Outspeeds and KOs
							goto WEAK_TO_MOVE_DECREMENT;
					}
					else if (isWeakToMove) //At least one move does a good chunk of damage to the considered mon
					{
						WEAK_TO_MOVE_DECREMENT:
						if (scores[i] >= SWITCHING_DECREASE_WEAK_TO_MOVE)
							scores[i] -= SWITCHING_DECREASE_WEAK_TO_MOVE;
						else
							scores[i] = 0;
					}
					else if (!isNormalEffectiveness) //Foe has no Super-Effective or Normal-Effectiveness moves
					{
						scores[i] += SWITCHING_INCREASE_RESIST_ALL_MOVES;
						flags[i] |= SWITCHING_FLAG_RESIST_ALL_MOVES;
					}
					else
					{
						bool8 cantWall = FALSE;
						u32 attack, spAttack;
						APPLY_STAT_MOD(attack, &gBattleMons[foe], gBattleMons[foe].attack, STAT_STAGE_ATK);
						APPLY_STAT_MOD(spAttack, &gBattleMons[foe], gBattleMons[foe].spAttack, STAT_STAGE_SPATK);

						if (physMoveInMoveset && GetMonData(consideredMon, MON_DATA_DEF, NULL) <= attack)
							cantWall = TRUE;
						else if (specMoveInMoveset && GetMonData(consideredMon, MON_DATA_SPDEF, NULL) <= spAttack)
							cantWall = TRUE;

						if (!cantWall)
						{
							scores[i] += SWITCHING_INCREASE_WALLS_FOE;
							flags[i] |= SWITCHING_FLAG_WALLS_FOE;
						}
					}
				}
			}

			if (scores[i] >= SWITCHING_SCORE_MAX * (isOneFoeOnField ? 1 : 2) //Adjust when multiple foes are on the field
			&& canRemoveHazards[i]) //This mon is perfect
			{
				secondBestMonId = bestMonId; //Move down the old best mon
				bestMonId = i; //This is the best mon
			}
			else if (bestMonId == PARTY_SIZE //No best mon yet
			|| scores[i] > scores[bestMonId]
			|| (scores[i] == scores[bestMonId] //Same score but
			 && flags[bestMonId] & SWITCHING_FLAG_FAINTS_FROM_FOE //Best mon faints from foe
			 && !(flags[i] & SWITCHING_FLAG_FAINTS_FROM_FOE)) //This one isn't KOd immediately
			|| (scores[i] == scores[bestMonId]
			 && !(flags[bestMonId] & SWITCHING_FLAG_FAINTS_FROM_FOE)
			 && !(flags[i] & SWITCHING_FLAG_FAINTS_FROM_FOE)
			 && (AIRandom() % 100 < 50))) //50% chance when having similar scores and neither can be KOd
			{
				secondBestMonId = bestMonId; //If no best mon will still be PARTY_SIZE
				bestMonId = i;
			}
			else if (secondBestMonId == PARTY_SIZE //No second best mon yet
			|| scores[i] > scores[secondBestMonId] //This mon isn't the best but it's better than the current second best
			|| (scores[i] == scores[secondBestMonId] //Same score but
			 && flags[secondBestMonId] & SWITCHING_FLAG_FAINTS_FROM_FOE //Second best mon faints from foe
			 && !(flags[i] & SWITCHING_FLAG_FAINTS_FROM_FOE)) //Same score but one isn't KOd immediately
			|| (scores[i] == scores[secondBestMonId]
			 && !(flags[secondBestMonId] & SWITCHING_FLAG_FAINTS_FROM_FOE)
			 && !(flags[i] & SWITCHING_FLAG_FAINTS_FROM_FOE)
			 && (AIRandom() % 100 < 50))) //50% chance when having similar scores and neither can be KOd
				secondBestMonId = i;
		}

		CHECK_NEXT_MON: ;
	}

	Memcpy(gNewBS->ai.monIdToSwitchIntoScores[SIDE(gActiveBattler)], scores, sizeof(gNewBS->ai.monIdToSwitchIntoScores[SIDE(gActiveBattler)]));
	Memcpy(gNewBS->ai.monIdToSwitchIntoFlags[SIDE(gActiveBattler)], flags, sizeof(gNewBS->ai.monIdToSwitchIntoFlags[SIDE(gActiveBattler)]));

	if (bestMonId != PARTY_SIZE)
	{
		if (!(flags[bestMonId] & SWITCHING_FLAG_RESIST_ALL_MOVES) //Best mon doesn't resist all moves
		&& (flags[bestMonId] & (SWITCHING_FLAG_KO_FOE | SWITCHING_FLAG_OUTSPEEDS)) != (SWITCHING_FLAG_KO_FOE | SWITCHING_FLAG_OUTSPEEDS)) //And doesn't outspeed and KO
		{
			bool8 tSpikesActive = gSideTimers[SIDE(gActiveBattler)].tspikesAmount > 0;
			for (i = firstId; i < lastId; ++i)
			{
				if ((canRemoveHazards[i] && !(flags[i] & SWITCHING_FLAG_FAINTS_FROM_FOE)) //Mon can stay alive and remove hazards
				|| (tSpikesActive && canNegateToxicSpikes[i]))
				{
					if (IS_DOUBLE_BATTLE)
					{
						gNewBS->ai.bestMonIdToSwitchIntoScores[gActiveBattler][0] = SWITCHING_INCREASE_CAN_REMOVE_HAZARDS * 2;
						gNewBS->ai.bestMonIdToSwitchIntoFlags[gActiveBattler][0] = SWITCHING_FLAG_CAN_REMOVE_HAZARDS;

						if (!BankSideHasTwoTrainers(gActiveBattler))
						{
							gNewBS->ai.bestMonIdToSwitchIntoScores[PARTNER(gActiveBattler)][0] = SWITCHING_INCREASE_CAN_REMOVE_HAZARDS * 2;
							gNewBS->ai.bestMonIdToSwitchIntoFlags[PARTNER(gActiveBattler)][0] = SWITCHING_FLAG_CAN_REMOVE_HAZARDS;
						}
					}
					else
					{
						gNewBS->ai.bestMonIdToSwitchIntoScores[gActiveBattler][0] = SWITCHING_INCREASE_CAN_REMOVE_HAZARDS;
						gNewBS->ai.bestMonIdToSwitchIntoFlags[gActiveBattler][0] = SWITCHING_FLAG_CAN_REMOVE_HAZARDS;
					}

					gNewBS->ai.bestMonIdToSwitchInto[gActiveBattler][0] = i;
					if (!BankSideHasTwoTrainers(gActiveBattler))
						gNewBS->ai.bestMonIdToSwitchInto[PARTNER(gActiveBattler)][0] = i;

					secondBestMonId = bestMonId;
					bestMonId = i; //New best mon is mon that can remove hazard
					break;
				}
			}
		}

		gNewBS->ai.bestMonIdToSwitchInto[gActiveBattler][0] = bestMonId;
		gNewBS->ai.bestMonIdToSwitchInto[gActiveBattler][1] = secondBestMonId;
		gNewBS->ai.bestMonIdToSwitchIntoScores[gActiveBattler][0] = scores[bestMonId];
		gNewBS->ai.bestMonIdToSwitchIntoScores[gActiveBattler][1] = scores[secondBestMonId];
		gNewBS->ai.bestMonIdToSwitchIntoFlags[gActiveBattler][0] = flags[bestMonId];
		gNewBS->ai.bestMonIdToSwitchIntoFlags[gActiveBattler][1] = flags[secondBestMonId];

		if (IS_DOUBLE_BATTLE && !BankSideHasTwoTrainers(gActiveBattler)) //Store data for second mon too
		{
			gNewBS->ai.bestMonIdToSwitchInto[PARTNER(gActiveBattler)][0] = bestMonId;
			gNewBS->ai.bestMonIdToSwitchInto[PARTNER(gActiveBattler)][1] = secondBestMonId;
			gNewBS->ai.bestMonIdToSwitchIntoScores[PARTNER(gActiveBattler)][0] = scores[bestMonId];
			gNewBS->ai.bestMonIdToSwitchIntoScores[PARTNER(gActiveBattler)][1] = scores[secondBestMonId];
			gNewBS->ai.bestMonIdToSwitchIntoFlags[PARTNER(gActiveBattler)][0] = flags[bestMonId];
			gNewBS->ai.bestMonIdToSwitchIntoFlags[PARTNER(gActiveBattler)][1] = flags[secondBestMonId];
		}

		return bestMonId;
	}

	//If for some reason we weren't able to find a mon above,
	//pick the last checked available mon now.
	gNewBS->ai.bestMonIdToSwitchIntoScores[gActiveBattler][0] = 0;
	gNewBS->ai.bestMonIdToSwitchIntoFlags[gActiveBattler][0] = 0;
	gNewBS->ai.bestMonIdToSwitchInto[gActiveBattler][0] = lastValidMon;
	gNewBS->ai.bestMonIdToSwitchIntoScores[gActiveBattler][1] = 0;
	gNewBS->ai.bestMonIdToSwitchIntoFlags[gActiveBattler][1] = 0;
	gNewBS->ai.bestMonIdToSwitchInto[gActiveBattler][1] = secondLastValidMon;

	if (!BankSideHasTwoTrainers(gActiveBattler)) //Store data for second mon too
	{
		gNewBS->ai.bestMonIdToSwitchIntoScores[PARTNER(gActiveBattler)][0] = 0;
		gNewBS->ai.bestMonIdToSwitchIntoFlags[PARTNER(gActiveBattler)][0] = 0;
		gNewBS->ai.bestMonIdToSwitchInto[PARTNER(gActiveBattler)][0] = lastValidMon;
		gNewBS->ai.bestMonIdToSwitchIntoScores[PARTNER(gActiveBattler)][1] = 0;
		gNewBS->ai.bestMonIdToSwitchIntoFlags[PARTNER(gActiveBattler)][1] = 0;
		gNewBS->ai.bestMonIdToSwitchInto[PARTNER(gActiveBattler)][1] = secondLastValidMon;
	}

	return lastValidMon; //If lastValidMon is still PARTY_SIZE when reaching here, there is a bigger problem
}

static void ResetBestMonToSwitchIntoData(u8 bank)
{
	gNewBS->ai.bestMonIdToSwitchIntoScores[bank][0] = 0;
	gNewBS->ai.bestMonIdToSwitchIntoFlags[bank][0] = 0;
	gNewBS->ai.bestMonIdToSwitchInto[bank][0] = PARTY_SIZE;
	gNewBS->ai.bestMonIdToSwitchIntoScores[bank][1] = 0;
	gNewBS->ai.bestMonIdToSwitchIntoFlags[bank][1] = 0;
	gNewBS->ai.bestMonIdToSwitchInto[bank][1] = PARTY_SIZE;
	Memset(gNewBS->ai.monIdToSwitchIntoScores[SIDE(bank)], 0, sizeof(gNewBS->ai.monIdToSwitchIntoScores[SIDE(bank)]));
	Memset(gNewBS->ai.monIdToSwitchIntoFlags[SIDE(bank)], 0, sizeof(gNewBS->ai.monIdToSwitchIntoFlags[SIDE(bank)]));
}

void ResetBestMonToSwitchInto(u8 bank)
{
	ResetBestMonToSwitchIntoData(bank);

	if (!BankSideHasTwoTrainers(bank)) //Wipe data for second mon too
		ResetBestMonToSwitchIntoData(PARTNER(bank));
}

static void RemoveBestMonToSwitchIntoData(u8 bank)
{
	//secondBestMonId -> bestMonId
	gNewBS->ai.bestMonIdToSwitchIntoScores[bank][0] = gNewBS->ai.bestMonIdToSwitchIntoScores[bank][1];
	gNewBS->ai.bestMonIdToSwitchIntoFlags[bank][0] = gNewBS->ai.bestMonIdToSwitchIntoFlags[bank][1];
	gNewBS->ai.bestMonIdToSwitchInto[bank][0] = gNewBS->ai.bestMonIdToSwitchInto[bank][1];

	gNewBS->ai.bestMonIdToSwitchIntoScores[bank][1] = 0;
	gNewBS->ai.bestMonIdToSwitchIntoFlags[bank][1] = 0;
	gNewBS->ai.bestMonIdToSwitchInto[bank][1] = PARTY_SIZE;
}

void RemoveBestMonToSwitchInto(u8 bank)
{
	RemoveBestMonToSwitchIntoData(bank);

	if (!BankSideHasTwoTrainers(bank)) //Wipe data for second mon too
		RemoveBestMonToSwitchIntoData(PARTNER(bank));
}

#ifdef VAR_GAME_DIFFICULTY
static bool8 IsGoodIdeaToDoShiftSwitch(u8 aiBank, u8 playerBank)
{
	u8 switchFlags = GetMostSuitableMonToSwitchIntoFlags();

	if (!WillTakeSignificantDamageFromEntryHazards(aiBank, 2)) //50% health loss
	{
		if (OnlyBadMovesLeftInMoveset(aiBank, playerBank)) //AI mon has nothing good against this new foe
			return TRUE;

		if (!CanKnockOut(aiBank, playerBank)) //Current mon out can't KO new mon being switched in
		{
			if (switchFlags & SWITCHING_FLAG_KO_FOE && switchFlags & (SWITCHING_FLAG_OUTSPEEDS | SWITCHING_FLAG_RESIST_ALL_MOVES))
				return TRUE;
		}
		
		if (!Can2HKO(aiBank, playerBank)) //Current mon out can't 2HKO new mon being switched in
		{
			if (switchFlags & SWITCHING_INCREASE_CAN_2HKO //AI can 2HKO with new mon
			&& switchFlags & SWITCHING_FLAG_OUTSPEEDS //New AI mon would go before new player mon
			&& !(switchFlags & SWITCHING_FLAG_FAINTS_FROM_FOE)) //Can live a hit and fire back with a 2HKO
				return TRUE;
		}

		if (SpeedCalc(aiBank) < SpeedCalc(playerBank) //New foe will outspeed AI mon currently on field
		&& CanKnockOut(playerBank, aiBank) //And KO the AI mon currently on field
		&& !PriorityMoveInMoveset(aiBank)) //And the AI can't use a priority move to hit before that happens
		{
			if (switchFlags & SWITCHING_FLAG_KO_FOE && switchFlags & (SWITCHING_FLAG_OUTSPEEDS | SWITCHING_FLAG_RESIST_ALL_MOVES | SWITCHING_FLAG_WALLS_FOE))
				return TRUE; //New AI mon can KO and either outspeeds or walls

			if (switchFlags & SWITCHING_FLAG_RESIST_ALL_MOVES)
				return TRUE; //New AI mon can't KO but can resist all moves

			if (switchFlags & SWITCHING_INCREASE_CAN_2HKO
			&& switchFlags & SWITCHING_FLAG_OUTSPEEDS
			&& !(switchFlags & SWITCHING_FLAG_FAINTS_FROM_FOE))
				return TRUE; //New AI mon can live to 2HKO
		}
	}

	return FALSE; //Don't switch
}
#endif

void ShouldDoAIShiftSwitch(void)
{
	#ifdef VAR_GAME_DIFFICULTY
	if (gBattleTypeFlags & BATTLE_TYPE_TRAINER
	&& !IS_DOUBLE_BATTLE
	&& BATTLER_ALIVE(GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT)) //AI has a mon that can be switched out
	&& gBankFainted != GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT) //Handles case of both player and AI mons fainting at same time (eg. due to Explosion)
	&& ((gBattleScripting.battleStyle == OPTIONS_BATTLE_STYLE_SHIFT && VarGet(VAR_GAME_DIFFICULTY) >= OPTIONS_HARD_DIFFICULTY)
	 || (gBattleScripting.battleStyle == OPTIONS_BATTLE_STYLE_SEMI_SHIFT && VarGet(VAR_GAME_DIFFICULTY) >= OPTIONS_EXPERT_DIFFICULTY)
	#ifdef UNBOUND
	 || GetCurrentRegionMapSectionId() == MAPSEC_SS_MARINE
	#endif
	))
	{
		gActiveBattler = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);
		u8 foe = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);

		CalculateAIPredictions();
		if (IsGoodIdeaToDoShiftSwitch(gActiveBattler, foe))
		{
			ClearCachedAIData();
			return; //Continue in script
		}
		else
			gNewBS->ai.switchingCooldown[gActiveBattler] = 3; //AI just decided not to switch out
	}
	#endif

	gBattlescriptCurrInstr = BattleScript_FaintedMonChooseAnotherRejoin - 5;
}
