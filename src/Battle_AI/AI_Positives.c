#include "../defines.h"
#include "../../include/random.h"
#include "../../include/constants/items.h"

#include "../../include/new/ability_tables.h"
#include "../../include/new/accuracy_calc.h"
#include "../../include/new/AI_advanced.h"
#include "../../include/new/AI_Helper_Functions.h"
#include "../../include/new/battle_start_turn_start.h"
#include "../../include/new/damage_calc.h"
#include "../../include/new/general_bs_commands.h"
#include "../../include/new/Helper_Functions.h"
#include "../../include/new/item.h"
#include "../../include/new/move_tables.h"

extern const struct NaturalGiftStruct gNaturalGiftTable[];
extern const struct FlingStruct gFlingTable[];

u8 AI_Script_Positives(const u8 bankAtk, const u8 bankDef, const u16 originalMove, const u8 originalViability)
{
	//Put check to call special script for attacking partner
	u32 i, j;
	u16 predictedMove = IsValidMovePrediction(bankDef, bankAtk); //The move the target is likely to make against the attacker
	u8 class = PredictBankFightingStyle(bankAtk);
	s16 viability = originalViability;

	//Get relevant params
	u16 move = TryReplaceMoveWithZMove(bankAtk, originalMove);	
	u8 moveEffect = gBattleMoves[move].effect;
	u8 moveType = GetMoveTypeSpecial(bankAtk, move);

	u8 bankAtkPartner = PARTNER(bankAtk);
	u8 bankDefPartner = PARTNER(bankDef);
	
	u16 partnerMove = (gChosenMovesByBanks[bankAtkPartner] != MOVE_NONE) ? gChosenMovesByBanks[bankAtkPartner] : IsValidMovePrediction(bankAtkPartner, FOE(bankAtk));
	
	u16 atkSpecies = SPECIES(bankAtk);
	//u16 defSpecies = SPECIES(bankDef);
	
	u16 atkItem = ITEM(bankAtk);
	u16 defItem = ITEM(bankDef);
	u8 atkItemEffect = ITEM_EFFECT(bankAtk);
	u8 defItemEffect = ITEM_EFFECT(bankDef);
	u8 atkPartnerItemEffect = ITEM_EFFECT(bankAtkPartner);
	
	u8 atkItemQuality = ITEM_QUALITY(bankAtk);
	
	//u8 atkItemPocket = ITEM_POCKET(bankAtk);
	//u8 atkPartnerItemPocket = ITEM_POCKET(bankAtkPartner);
	//u8 defItemPocket = ITEM_POCKET(bankDef);
	//u8 defPartnerItemPocket = ITEM_POCKET(bankDefPartner);
	
	u8 atkAbility = GetAIAbility(bankAtk, bankDef, move);
	u8 defAbility = GetAIAbility(bankDef, bankAtk, predictedMove);
	u8 atkPartnerAbility = ABILITY(bankAtkPartner);
	//u8 defPartnerAbility = ABILITY(bankDefPartner);

	if (!NO_MOLD_BREAKERS(atkAbility, move)
	&& gMoldBreakerIgnoredAbilities[defAbility])
		defAbility = ABILITY_NONE;

	u32 atkStatus1 = gBattleMons[bankAtk].status1;
	u32 atkStatus2 = gBattleMons[bankAtk].status2;
	u32 atkStatus3 = gStatuses3[bankAtk];
	u32 defStatus1 = gBattleMons[bankDef].status1;
	u32 defStatus2 = gBattleMons[bankDef].status2;
	u32 defStatus3 = gStatuses3[bankDef];
	
	//u32 defPartnerStatus2 = gBattleMons[bankDefPartner].status2;
	
	u16 atkAttack = gBattleMons[bankAtk].attack;
	u16 atkDefense = gBattleMons[bankAtk].defense;
	u16 atkSpeed = gBattleMons[bankAtk].speed;
	u16 atkSpAtk = gBattleMons[bankAtk].spAttack;
	u16 atkSpDef = gBattleMons[bankAtk].spDefense;
	
	u16 defAttack = gBattleMons[bankDef].attack;
	u16 defDefense = gBattleMons[bankDef].defense;
	u16 defSpeed = gBattleMons[bankDef].speed;
	u16 defSpAtk = gBattleMons[bankDef].spAttack;
	u16 defSpDef = gBattleMons[bankDef].spDefense;	
	
	u8 moveSplit = CalcMoveSplit(bankAtk, move);

	//Load Alternative targets
	u8 foe1, foe2;
	foe1 = FOE(bankAtk);
				
	if (IS_DOUBLE_BATTLE)
		foe2 = PARTNER(FOE(bankAtk));
	else
		foe2 = foe1;

	switch (moveEffect) {
		case EFFECT_HIT:
			// to do
			break;
		
		case EFFECT_SLEEP:
		case EFFECT_YAWN:
		AI_SLEEP_CHECKS:
			if (defItemEffect != ITEM_EFFECT_CURE_SLP
			&& defItemEffect != ITEM_EFFECT_CURE_STATUS
			&& defAbility != ABILITY_EARLYBIRD
			&& defAbility != ABILITY_SHEDSKIN)
				INCREASE_STATUS_VIABILITY(3);
			break;
			
		case EFFECT_ABSORB:
		case EFFECT_DREAM_EATER:
		AI_DRAIN_HP_CHECK: ;
			if (ShouldRecover(bankAtk, bankDef, move))
			{
				if (IsClassStall(class))
					INCREASE_VIABILITY(6);
				else
					INCREASE_VIABILITY(3); //Past strongest move
			}
			break;

		case EFFECT_PARALYZE_HIT:
			if (CalcSecondaryEffectChance(bankAtk, move) >= 75)
				goto AI_PARALYZE_CHECKS;
			break;

		case EFFECT_EXPLOSION:
			if (predictedMove != MOVE_NONE //If foe isn't going to attack, don't kill yourself now
			&&  gBattleMoves[predictedMove].effect != EFFECT_PROTECT)
			{
				if (atkItemEffect == ITEM_EFFECT_GEM
				&& atkItemQuality == moveType) //AI Was meant to explode
				{	
					INCREASE_VIABILITY(4); //Counteract the negative check
				}
				else if (atkItemEffect == ITEM_EFFECT_CUSTAP_BERRY
				&& GetHealthPercentage(bankAtk) <= 25)
				{
					INCREASE_VIABILITY(4); //Counteract the negative check
				}
			}
			break;

		case EFFECT_MIRROR_MOVE: //May cause issues with priority calcs?
			switch (move) {
				case MOVE_COPYCAT:	
					if (MoveWouldHitFirst(move, bankAtk, bankDef))
					{
					COPYCAT_CHECK_LAST_MOVE:
						if (gNewBS->LastUsedMove != MOVE_NONE
						&& gNewBS->LastUsedMove != 0xFFFF
						&& !CheckTableForMove(gNewBS->LastUsedMove, CopycatBanTable)
						&& !MoveInMoveset(gNewBS->LastUsedMove, bankAtk)) //If you have the move, use it directly
							return AI_Script_Positives(bankAtk, bankDef, gNewBS->LastUsedMove, originalViability);
					}
					else
					{
						if (predictedMove == MOVE_NONE)
							goto COPYCAT_CHECK_LAST_MOVE;
						else if (!CheckTableForMove(predictedMove, CopycatBanTable)
							 &&  !MoveInMoveset(predictedMove, bankAtk))
						{
							return AI_Script_Positives(bankAtk, bankDef, predictedMove, originalViability);
						}						
					}
					break;

				default: //Mirror Move
					if (gBattleStruct->lastTakenMoveFrom[bankAtk][bankDef] != MOVE_NONE)
						return AI_Script_Positives(bankAtk, bankDef, gBattleStruct->lastTakenMoveFrom[bankAtk][bankDef], originalViability);
			}
			break;
			
		//Increased stat effects
		case EFFECT_ATTACK_UP:
		case EFFECT_ATTACK_UP_2:
			switch (move) {
				case MOVE_HONECLAWS:	
					if (STAT_STAGE(bankAtk,STAT_STAGE_ATK) >= 8)
						goto AI_ACCURACY_PLUS;
					break;

				default:
				AI_ATTACK_PLUS:
					if (PhysicalMoveInMoveset(bankAtk) && atkAbility != ABILITY_CONTRARY)
						INCREASE_STAT_VIABILITY(STAT_STAGE_ATK, 8, 2);
					break;
			}
			break;
			
		case EFFECT_DEFENSE_UP:
		case EFFECT_DEFENSE_UP_2:
		AI_DEFENSE_PLUS:
		/*
			switch (move) {
				case MOVE_FLOWERSHIELD:
					if (IsClassCleric(class)
					&& IsOfType(bankAtkPartner, TYPE_GRASS)
					&& atkPartnerAbility != ABILITY_CONTRARY
					&& STAT_STAGE(bankAtkPartner, STAT_STAGE_DEF) < 12
					&& BATTLER_ALIVE(bankAtkPartner))
					{
						viability += 5;
					}
					break;
					
				case MOVE_MAGNETICFLUX:
					if (IsClassCleric(class)
					&& (atkPartnerAbility == ABILITY_PLUS || atkPartnerAbility == ABILITY_MINUS)
					&& (STAT_STAGE(bankAtkPartner, STAT_STAGE_DEF) < 12 || STAT_STAGE(bankAtkPartner, STAT_STAGE_SPDEF) < 12))
					&& BATTLER_ALIVE(bankAtkPartner)
					{
						viability += 5;
					}
					else if (BankLikelyToUseMoveSplit(bankDef) == SPLIT_PHYSICAL)
						break;
					
					goto AI_SPECIAL_DEFENSE_PLUS;
					
				case MOVE_AROMATICMIST:
					if (IsClassCleric(class)
					&& STAT_STAGE(bankAtkPartner, STAT_STAGE_SPDEF) < 12
					&& BATTLER_ALIVE(bankAtkPartner)
					&& atkPartnerAbility != ABILITY_CONTRARY)
						viability += 5;
					break;
			}*/

			//Continue defense check
			if (BankLikelyToUseMoveSplit(bankDef, class) == SPLIT_PHYSICAL && atkAbility != ABILITY_CONTRARY)
				INCREASE_STAT_VIABILITY(STAT_STAGE_DEF, 10, 1);
			break;	
			
		case EFFECT_SPEED_UP:
		case EFFECT_SPEED_UP_2:
		AI_SPEED_PLUS:
			if (atkAbility != ABILITY_CONTRARY)
				INCREASE_STAT_VIABILITY(STAT_STAGE_SPEED, 8, 3);
			break;
			
		case EFFECT_SPECIAL_ATTACK_UP:
		case EFFECT_SPECIAL_ATTACK_UP_2:
			switch (move)
			{
				case MOVE_GROWTH:
				case MOVE_WORKUP:
					if (PhysicalMoveInMoveset(bankAtk))
						goto AI_ATTACK_PLUS;
					else if (SpecialMoveInMoveset(bankAtk))
						goto AI_SPECIAL_ATTACK_PLUS;
					break;
				/*
				case MOVE_ROTOTILLER:
					if (IsOfType(bankAtkPartner,TYPE_GRASS))
						viability += 5;
					else if (PhysicalMoveInMoveset(bankAtk))
						goto AI_ATTACK_PLUS;
					else if (SpecialMoveInMoveset(bankAtk))
						break;
					goto FUNCTION_RETURN;

				case MOVE_GEARUP:
					if (atkPartnerAbility == ABILITY_PLUS || atkPartnerAbility == ABILITY_MINUS)
					{
						viability += 5;
						goto FUNCTION_RETURN;
					}
					else if (PhysicalMoveInMoveset(bankAtk))
						goto AI_ATTACK_PLUS;
					else if (SpecialMoveInMoveset(bankAtk))
						break;
					goto FUNCTION_RETURN;
				*/

				default:
				AI_SPECIAL_ATTACK_PLUS:
					if (SpecialMoveInMoveset(bankAtk) && atkAbility != ABILITY_CONTRARY)
						INCREASE_STAT_VIABILITY(STAT_STAGE_SPATK, 8, 2);
					break;
			}
			break;
			
		case EFFECT_SPECIAL_DEFENSE_UP:
		case EFFECT_SPECIAL_DEFENSE_UP_2:
		AI_SPECIAL_DEFENSE_PLUS: ;
			if (BankLikelyToUseMoveSplit(bankDef, class) == SPLIT_SPECIAL && atkAbility != ABILITY_CONTRARY)
				INCREASE_STAT_VIABILITY(STAT_STAGE_SPDEF, 10, 1);
			break;
			
		case EFFECT_ACCURACY_UP:
		case EFFECT_ACCURACY_UP_2:
		case EFFECT_MINIMIZE:
		AI_ACCURACY_PLUS: ;
			if (MoveInMovesetWithAccuracyLessThan(bankAtk, foe1, 90, TRUE) && defAbility != ABILITY_CONTRARY)
				INCREASE_STAT_VIABILITY(STAT_STAGE_ACC, STAT_STAGE_MAX, 2);
			break;
			
		case EFFECT_EVASION_UP:
		case EFFECT_EVASION_UP_2:
			if (atkAbility != ABILITY_CONTRARY)
				INCREASE_STAT_VIABILITY(STAT_STAGE_EVASION, STAT_STAGE_MAX, 4);
			break;
			
		//Decreased stat effects 
		case EFFECT_ATTACK_DOWN:
		case EFFECT_ATTACK_DOWN_2:
		AI_ATTACK_MINUS:
			if (!MoveWouldHitFirst(move, bankAtk, bankDef) && CanKnockOut(bankAtk, bankDef))
				break; //Don't bother lowering stats if can kill enemy.
			if (STAT_STAGE(bankDef, STAT_STAGE_ATK) > 4 && PhysicalMoveInMoveset(bankDef) && defAbility != ABILITY_CONTRARY)
				INCREASE_STATUS_VIABILITY(1);
			break;
			
		case EFFECT_DEFENSE_DOWN:
		case EFFECT_DEFENSE_DOWN_2:
		AI_DEFENSE_MINUS:
			if (!MoveWouldHitFirst(move, bankAtk, bankDef) && CanKnockOut(bankAtk, bankDef))
				break; //Don't bother lowering stats if can kill enemy.
			if (STAT_STAGE(bankDef, STAT_STAGE_DEF) > 4 && PhysicalMoveInMoveset(bankAtk) && defAbility != ABILITY_CONTRARY)
				INCREASE_STATUS_VIABILITY(1);
			break;
			
		case EFFECT_SPEED_DOWN:
		case EFFECT_SPEED_DOWN_2:
		AI_SPEED_MINUS:
			if (!MoveWouldHitFirst(move, bankAtk, bankDef) && CanKnockOut(bankAtk, bankDef))
				break; //Don't bother lowering stats if can kill enemy.
			if (SpeedCalc(bankAtk) <= SpeedCalc(bankDef) && defAbility != ABILITY_CONTRARY)
				INCREASE_STATUS_VIABILITY(2);
			break;
			
		case EFFECT_SPECIAL_ATTACK_DOWN:
		case EFFECT_SPECIAL_ATTACK_DOWN_2:
		AI_SPECIAL_ATTACK_MINUS:
			if (!MoveWouldHitFirst(move, bankAtk, bankDef) && CanKnockOut(bankAtk, bankDef))
				break; //Don't bother lowering stats if can kill enemy.
			if (STAT_STAGE(bankDef, STAT_STAGE_SPATK) > 4 && SpecialMoveInMoveset(bankDef) && defAbility != ABILITY_CONTRARY)
				INCREASE_STATUS_VIABILITY(1);
			break;
			
		case EFFECT_SPECIAL_DEFENSE_DOWN:
		case EFFECT_SPECIAL_DEFENSE_DOWN_2:
		AI_SPECIAL_DEFENSE_MINUS:
			if (!MoveWouldHitFirst(move, bankAtk, bankDef) && CanKnockOut(bankAtk, bankDef))
				break; //Don't bother lowering stats if can kill enemy.
			if (STAT_STAGE(bankDef, STAT_STAGE_SPDEF) > 4 && SpecialMoveInMoveset(bankAtk) && defAbility != ABILITY_CONTRARY)
				INCREASE_STATUS_VIABILITY(1);
			break;
			
		case EFFECT_ACCURACY_DOWN:
		case EFFECT_ACCURACY_DOWN_2:
		AI_ACCURACY_MINUS:
			if (!MoveWouldHitFirst(move, bankAtk, bankDef) && CanKnockOut(bankAtk, bankDef))
				break; //Don't bother lowering stats if can kill enemy.
			if (defAbility != ABILITY_CONTRARY)
				INCREASE_STATUS_VIABILITY(1);
			break;
			
		case EFFECT_EVASION_DOWN:
		case EFFECT_EVASION_DOWN_2:
		AI_EVASION_MINUS:
			if ((STAT_STAGE(bankDef, STAT_STAGE_EVASION) > 6 || MoveInMovesetWithAccuracyLessThan(bankAtk, foe1, 90, TRUE))
			&& defAbility != ABILITY_CONTRARY)
				INCREASE_STATUS_VIABILITY(2);
			break;

		case EFFECT_HAZE:
			if (!IS_DOUBLE_BATTLE)
			{
				if (ShouldPhaze(bankAtk, foe1, move, class))
					INCREASE_VIABILITY(8);
			}
			else
			{
				if (ShouldPhaze(bankAtk, foe1, move, class)
				||  ShouldPhaze(bankAtk, foe2, move, class))
					INCREASE_VIABILITY(8);
			}			
			break;
		/*	
		case EFFECT_BIDE:
			if (DamagingMoveInMoveset(bankAtk))
				break;
			else if (IsClassSweeper(class))
				viability += 3;
			break;*/

		case EFFECT_ROAR:
			if (ShouldPhaze(bankAtk, bankDef, move, class))
				INCREASE_VIABILITY(8);
			break;

		case EFFECT_MULTI_HIT:
		case EFFECT_TRIPLE_KICK:
		case EFFECT_DOUBLE_HIT:
			if (IsClassSweeper(class)
			&& !IsStrongestMove(move, bankAtk, bankDef, FALSE)
			&& (MoveBlockedBySubstitute(move, bankAtk, bankDef) //Attack has to hit substitute to break it
			 || atkItemEffect == ITEM_EFFECT_FLINCH))
				INCREASE_VIABILITY(3); //Move past strongest move
			break;
			
		case EFFECT_CONVERSION:
			if (!IsOfType(bankAtk, gBattleMoves[gBattleMons[bankAtk].moves[0]].type)
			&& IsClassSweeper(class))
				INCREASE_STATUS_VIABILITY(1);
			break;
			
		case EFFECT_FLINCH_HIT:
		AI_FLINCH_CHECKS:
			if (defAbility == ABILITY_INNERFOCUS
			||  MoveBlockedBySubstitute(move, bankAtk, bankDef)
			||  !MoveWouldHitFirst(move, bankAtk, bankDef))
				break;
			else if (atkAbility == ABILITY_SERENEGRACE
			|| defStatus1 & STATUS1_PARALYSIS
			|| defStatus2 & (STATUS2_INFATUATION | STATUS2_CONFUSION))
				INCREASE_STATUS_VIABILITY(3);
			break;
			
		case EFFECT_RESTORE_HP:
		case EFFECT_MORNING_SUN:
		case EFFECT_SWALLOW:
		AI_RECOVER:
			if (BATTLER_ALIVE(foe1) && ShouldRecover(bankAtk, foe1, move))
			{
				if (IsClassStall(class))
					INCREASE_VIABILITY(4);
				else
					INCREASE_STATUS_VIABILITY(3);
			}
			else if (IS_DOUBLE_BATTLE)
			{
				if (BATTLER_ALIVE(foe2)
				&& ShouldRecover(bankAtk, foe2, move))
				{
					if (IsClassStall(class))
						INCREASE_VIABILITY(4);
					else
						INCREASE_STATUS_VIABILITY(3);
				}
			}
			break;

		case EFFECT_TOXIC:
		case EFFECT_POISON:
		AI_POISON_CHECKS:
			if (!CanBePoisoned(bankDef, bankAtk, TRUE)
			||  defItemEffect == ITEM_EFFECT_CURE_PSN
			||  defItemEffect == ITEM_EFFECT_CURE_STATUS
			||  defAbility == ABILITY_SHEDSKIN 
			||  defAbility == ABILITY_TOXICBOOST
			||  defAbility == ABILITY_POISONHEAL
			||  defAbility == ABILITY_MAGICGUARD
			||  atkAbility == ABILITY_POISONTOUCH)
				break;
			else if (MoveInMoveset(MOVE_VENOSHOCK, bankAtk)
			|| MoveInMoveset(MOVE_HEX, bankAtk)
			|| MoveInMoveset(MOVE_VENOMDRENCH, bankAtk)
			|| atkAbility == ABILITY_MERCILESS)
				INCREASE_STATUS_VIABILITY(2);
			else
				INCREASE_STATUS_VIABILITY(1); //AI enjoys poisoning
			break;
			
		case EFFECT_LIGHT_SCREEN:
			if (ShouldSetUpScreens(bankAtk, bankDef, move))
				INCREASE_VIABILITY(7);
			break;
			
		case EFFECT_REST:
			if (!(CanBePutToSleep(bankAtk, FALSE)))
				break;
			else if (atkItemEffect == ITEM_EFFECT_CURE_SLP 
			|| atkItemEffect == ITEM_EFFECT_CURE_STATUS
			|| MoveEffectInMoveset(EFFECT_SLEEP_TALK, bankAtk) 
			|| MoveEffectInMoveset(EFFECT_SNORE, bankAtk)
			|| atkAbility == ABILITY_SHEDSKIN
			|| atkAbility == ABILITY_EARLYBIRD
			|| ((gBattleWeather & WEATHER_RAIN_ANY && gWishFutureKnock->weatherDuration > 1) && atkAbility == ABILITY_HYDRATION))
			{
				if (BATTLER_ALIVE(foe1) && ShouldRecover(bankAtk, foe1, move))
				{
					INCREASE_STATUS_VIABILITY(1);
				}
				else if (IS_DOUBLE_BATTLE)
				{
					if (BATTLER_ALIVE(foe2)
					&& ShouldRecover(bankAtk, foe2, move))
					{
						INCREASE_STATUS_VIABILITY(1);
					}
				}
			}
			else
				goto AI_RECOVER;

			break;

		case EFFECT_TRAP:
		AI_TRAP:
			if (MoveInMoveset(MOVE_RAPIDSPIN, bankDef)
			||  IsOfType(bankDef, TYPE_GHOST)
			||  defStatus2 & (STATUS2_WRAPPED))
			{
				break; //Just a regular attacking move now
			}
			else if (ShouldTrap(bankAtk, bankDef, move, class))
				INCREASE_VIABILITY(8);
			break;
		
		case EFFECT_MIST:
			if (IsClassScreener(class)
			/*&&  EnemiesHaveMovesWhichCanLowerStatsOrIntimidate(foe1, foe2)*/)
				INCREASE_VIABILITY(6);
			break;
		
		case EFFECT_FOCUS_ENERGY:
		AI_FOCUS_ENERGY:
			if (atkAbility == ABILITY_SUPERLUCK
			|| atkAbility == ABILITY_SNIPER
			|| atkItemEffect == ITEM_EFFECT_SCOPE_LENS)
				INCREASE_STATUS_VIABILITY(2);
			break;

		case EFFECT_CONFUSE:
		case EFFECT_FLATTER:
		AI_CONFUSE_CHECK:
			if (CanBeConfused(bankDef))
			{
				if (defStatus1 & STATUS1_PARALYSIS
				|| defStatus2 & (STATUS2_INFATUATION)
				|| (atkAbility == ABILITY_SERENEGRACE && FlinchingMoveInMoveset(bankAtk)))
					INCREASE_STATUS_VIABILITY(2);
				else
					INCREASE_STATUS_VIABILITY(1);
			}
			/* Automatically runs the above check for target's partner too
			else if (IS_DOUBLE_BATTLE
			&& gBattleMoves[move].target & MOVE_TARGET_BOTH && CanBeConfused(bankDefPartner))
			{
				if (defPartnerStatus1 & STATUS1_PARALYSIS
				|| defPartnerStatus2 & (STATUS2_INFATUATION)
				|| (atkAbility == ABILITY_SERENEGRACE && FlinchingMoveInMoveset(bankAtk)))
					INCREASE_STATUS_VIABILITY(2);
				else
					INCREASE_STATUS_VIABILITY(1);
			}*/
			break;
		
		case EFFECT_REFLECT:
			switch (move) {
				case MOVE_AURORAVEIL:
					if (ShouldSetUpScreens(bankAtk, bankDef, move))
					{
						if (IsClassScreener(class))
							INCREASE_VIABILITY(8);
						else
							INCREASE_STATUS_VIABILITY(2);
					}
					break;

				default:
					if (ShouldSetUpScreens(bankAtk, bankDef, move))
					{
						if (IsClassScreener(class))
							INCREASE_VIABILITY(7);
						else
							INCREASE_STATUS_VIABILITY(2);
					}
			}
			break;
			
		case EFFECT_PARALYZE:
		AI_PARALYZE_CHECKS:
			if (CanBeParalyzed(bankDef, TRUE))
			{
				if (defItemEffect == ITEM_EFFECT_CURE_PAR
				|| defItemEffect == ITEM_EFFECT_CURE_STATUS
				|| defAbility == ABILITY_SHEDSKIN)
					break;
					
				else if (MoveInMoveset(MOVE_HEX, bankAtk)
				|| FlinchingMoveInMoveset(bankAtk)
				|| defStatus2 & (STATUS2_CONFUSION | STATUS2_INFATUATION))
					INCREASE_STATUS_VIABILITY(2);
				else
					INCREASE_STATUS_VIABILITY(1);
			}
			break;

		case EFFECT_ATTACK_DOWN_HIT:
			if (CalcSecondaryEffectChance(bankAtk, move) >= 50)
				goto AI_ATTACK_MINUS;
			break;

		case EFFECT_DEFENSE_DOWN_HIT:
			if (CalcSecondaryEffectChance(bankAtk, move) >= 50)
				goto AI_DEFENSE_MINUS;
			break;

		case EFFECT_SPEED_DOWN_HIT:
			if (CalcSecondaryEffectChance(bankAtk, move) >= 50)
				goto AI_SPEED_MINUS;
			break;

		case EFFECT_SPECIAL_ATTACK_DOWN_HIT:
			if (CalcSecondaryEffectChance(bankAtk, move) >= 50)
				goto AI_SPECIAL_ATTACK_MINUS;
			break;

		case EFFECT_SPECIAL_DEFENSE_DOWN_HIT:
			if (CalcSecondaryEffectChance(bankAtk, move) >= 50)
				goto AI_SPECIAL_DEFENSE_MINUS;
			break;

		case EFFECT_ACCURACY_DOWN_HIT:
			if (CalcSecondaryEffectChance(bankAtk, move) >= 50)
				goto AI_ACCURACY_MINUS;
			break;

		case EFFECT_EVASION_DOWN_HIT:
			if (CalcSecondaryEffectChance(bankAtk, move) >= 50)
				goto AI_EVASION_MINUS;
			break;
			
		case EFFECT_CONFUSE_HIT:
			if (CalcSecondaryEffectChance(bankAtk, move) >= 75
			&&  MoveWillHit(move, bankAtk, bankDef))
				goto AI_CONFUSE_CHECK;
			break;

		case EFFECT_SUBSTITUTE:
			IncreaseSubstituteViability(&viability, class, bankAtk, foe1);
			break;

		case EFFECT_MIMIC:
			if (MoveWouldHitFirst(move, bankAtk, bankDef))
			{ 	
				if (gLastUsedMoves[bankDef] != MOVE_NONE
				&& gLastUsedMoves[bankDef] != 0xFFFF)
				{
					if (!CanKnockOut(bankDef, bankAtk)
					&& MoveKnocksOutXHits(gLastUsedMoves[bankDef], bankAtk, bankDef, 1))
						INCREASE_STATUS_VIABILITY(2);
					else if (MoveEffectInMoveset(EFFECT_IMPRISON, bankAtk))
						INCREASE_STATUS_VIABILITY(1);
				}

			}
			else if (predictedMove != MOVE_NONE)
			{
				if (!Can2HKO(bankDef, bankAtk)
				&& MoveKnocksOutXHits(predictedMove, bankAtk, bankDef, 1)) //Attacker can kill with mimicked move
					INCREASE_STATUS_VIABILITY(1);
				else if (MoveEffectInMoveset(EFFECT_IMPRISON, bankAtk))
					INCREASE_STATUS_VIABILITY(1);
			}
			break;

		case EFFECT_LEECH_SEED:
			if (IsOfType(bankDef, TYPE_GRASS)
			|| defStatus3 & STATUS3_LEECHSEED
			|| MoveInMoveset(MOVE_RAPIDSPIN, bankDef)
			|| defAbility == ABILITY_LIQUIDOOZE
			|| defAbility == ABILITY_MAGICGUARD)
				break;
			else
				INCREASE_STATUS_VIABILITY(3);
			break;
		
		case EFFECT_SPLASH:
			if (IsTypeZCrystal(atkItem, moveType) && !gNewBS->ZMoveData->used[bankAtk])
				INCREASE_VIABILITY(9); //Z-Splash!
			break;

		case EFFECT_DISABLE:
			if (gDisableStructs[bankDef].disableTimer1 == 0
			&&  defItemEffect != ITEM_EFFECT_CURE_ATTRACT)
			{
				if (MoveWouldHitFirst(move, bankAtk, bankDef))
				{ 	
					if (gLastUsedMoves[bankDef] != MOVE_NONE
					&& gLastUsedMoves[bankDef] != 0xFFFF)
					{
						if (gLastUsedMoves[bankDef] == predictedMove)
							INCREASE_STATUS_VIABILITY(3);
						else if (MoveKnocksOutXHits(gLastUsedMoves[bankDef], bankDef, bankAtk, 1))
							INCREASE_STATUS_VIABILITY(2); //Disable move that can kill attacker
					}
				}
				else if (predictedMove != MOVE_NONE && SPLIT(predictedMove) == SPLIT_STATUS)
					INCREASE_STATUS_VIABILITY(1); //Disable annoying status moves
			}
			break;

		case EFFECT_ENCORE:
			if (gDisableStructs[bankDef].encoreTimer == 0
			&&  defItemEffect != ITEM_EFFECT_CURE_ATTRACT)
			{
				if (MoveWouldHitFirst(move, bankAtk, bankDef))
				{ 	
					if (gLastUsedMoves[bankDef] != MOVE_NONE
					&& gLastUsedMoves[bankDef] != 0xFFFF
					&& (SPLIT(gLastUsedMoves[bankDef]) == SPLIT_STATUS || AI_SpecialTypeCalc(gLastUsedMoves[bankDef], bankDef, bankAtk) & MOVE_RESULT_DOESNT_AFFECT_FOE))
						INCREASE_STATUS_VIABILITY(3); //Lock into status moves
				}
				else if (predictedMove != MOVE_NONE && SPLIT(predictedMove) == SPLIT_STATUS)
					INCREASE_STATUS_VIABILITY(3); //Lock into status moves
			}
			break;

		case EFFECT_PAIN_SPLIT: ;
			u16 newHealth = (gBattleMons[bankAtk].hp + gBattleMons[bankDef].hp) / 2;
			u16 healthBenchmark = (gBattleMons[bankAtk].hp * 12) / 10;
			
			if (newHealth > healthBenchmark)
			{
				goto AI_DRAIN_HP_CHECK;
			}
			
			break;

		case EFFECT_SNORE:
		case EFFECT_SLEEP_TALK:
			if (atkStatus1 & STATUS1_SLEEP)
				INCREASE_VIABILITY(10);
			break;
			
		case EFFECT_LOCK_ON:
			switch (move) {
				case MOVE_LASERFOCUS:
					goto AI_FOCUS_ENERGY;
				
				default:	
					if (MoveEffectInMoveset(EFFECT_0HKO, bankAtk))
						INCREASE_STATUS_VIABILITY(2);
			}
			break;
		
		case EFFECT_SPEED_UP_1_HIT:
			if (CalcSecondaryEffectChance(bankAtk, move) >= 75)
				goto AI_SPEED_PLUS;
			break;

		case EFFECT_DESTINY_BOND:
			if (MoveWouldHitFirst(move, bankAtk, bankDef)
			&& (CanKnockOut(foe1, bankAtk) || CanKnockOut(foe2, bankAtk)))
				INCREASE_STATUS_VIABILITY(3);
			break;
			
		case EFFECT_SPITE: ;
			u8 moveSlot = FindMovePositionInMoveset(predictedMove, bankDef);

			if (MoveWouldHitFirst(move, bankAtk, bankDef)
			&&  predictedMove != MOVE_NONE
			&&  moveSlot < 4
			&&  gBattleMons[bankDef].pp[moveSlot] <= 4)
				INCREASE_STATUS_VIABILITY(3);
			break;
			
		case EFFECT_HEAL_BELL:
			if (ShouldUseWishAromatherapy(bankAtk, bankDef, move, class))
				INCREASE_VIABILITY(7);
			break;
		
		case EFFECT_THIEF:
			if (atkItem == ITEM_NONE
			&&  defItem != ITEM_NONE
			&& CanTransferItem(bankDef, defItem, GetBankPartyData(bankDef))
			&& CanTransferItem(bankAtk, defItem, GetBankPartyData(bankAtk))
			&& !MoveInMoveset(MOVE_ACROBATICS, bankAtk)
			&& defAbility != ABILITY_STICKYHOLD
			&& IsClassSweeper(class))
			{
				switch (ItemId_GetHoldEffect(ITEM(bankDef))) {
					case ITEM_EFFECT_CHOICE_BAND: ;
						if (ITEM_QUALITY(bankDef) == QUALITY_CHOICE_SCARF
						|| MoveSplitInMoveset(ITEM_QUALITY(bankDef), bankAtk))
							INCREASE_STATUS_VIABILITY(2);
						break;
	
					case ITEM_EFFECT_TOXIC_ORB:
						if (CanBePoisoned(bankAtk, bankAtk, FALSE)
						&& (atkAbility == ABILITY_POISONHEAL
						 || atkAbility == ABILITY_TOXICBOOST
						 || atkAbility == ABILITY_QUICKFEET
						 || atkAbility == ABILITY_MAGICGUARD
						 || MoveInMoveset(MOVE_FACADE, bankAtk)))
							INCREASE_STATUS_VIABILITY(2);
						break;
					
					case ITEM_EFFECT_FLAME_ORB:
						if (CanBeBurned(bankAtk, FALSE)
						&& (atkAbility == ABILITY_GUTS
						 || atkAbility == ABILITY_FLAREBOOST
						 || atkAbility == ABILITY_MAGICGUARD
						 || MoveInMoveset(MOVE_FACADE, bankAtk)))
							INCREASE_STATUS_VIABILITY(2);
						break;
					
					case ITEM_EFFECT_BLACK_SLUDGE:
						if (IsOfType(bankAtk, TYPE_POISON))
							INCREASE_STATUS_VIABILITY(2);
						break;
					
					case ITEM_EFFECT_IRON_BALL:
						if (MoveInMoveset(MOVE_FLING, bankAtk))
							INCREASE_STATUS_VIABILITY(2);
						break;
					
					case ITEM_EFFECT_LAGGING_TAIL:
					case ITEM_EFFECT_STICKY_BARB:
						break;
						
					default:
						INCREASE_VIABILITY(1);
				}
			}
			break;

		case EFFECT_MEAN_LOOK:
			if (IsTrapped(bankDef, TRUE))
				break; //Already trapped
			
			goto AI_TRAP;
			
		case EFFECT_NIGHTMARE:
			if (defAbility == ABILITY_MAGICGUARD)
				break;
			if (defAbility == ABILITY_COMATOSE || defStatus1 & STATUS1_SLEEP)
				INCREASE_STATUS_VIABILITY(3);
			break;
		
		case EFFECT_CURSE:
			if (IsOfType(bankAtk, TYPE_GHOST))
			{
				if (IsTrapped(bankDef, TRUE))
					INCREASE_STATUS_VIABILITY(3);
				else
					INCREASE_STATUS_VIABILITY(1);
				break;
			}
			else
			{
				if (atkAbility == ABILITY_CONTRARY || defAbility == ABILITY_MAGICGUARD)
					break;
				else if (MoveInMoveset(MOVE_GYROBALL, bankAtk))
					INCREASE_STAT_VIABILITY(STAT_STAGE_ATK, STAT_STAGE_MAX, 4); //Go until Gyro Ball can kill in 1 hit
				else if (STAT_STAGE(bankAtk, STAT_STAGE_SPEED) < 3)
					break;
				else if (STAT_STAGE(bankAtk, STAT_STAGE_ATK) < 8)
					INCREASE_STAT_VIABILITY(STAT_STAGE_ATK, 8, 2);
				else if (STAT_STAGE(bankAtk, STAT_STAGE_DEF) < 8)
					INCREASE_STAT_VIABILITY(STAT_STAGE_DEF, 8, 1);
				break;
			}
			break;
			
		case EFFECT_PROTECT: ;
			u16 firstOpponentMove = IsValidMovePrediction(foe1, bankAtk);
			u16 secondOpponentMove = IsValidMovePrediction(foe2, bankAtk);

			switch (move)
			{
				case MOVE_QUICKGUARD:
					if (IS_DOUBLE_BATTLE)
					{
						if (firstOpponentMove != MOVE_NONE
						&& PriorityCalc(foe1, ACTION_USE_MOVE, firstOpponentMove) > 0)
							INCREASE_STATUS_VIABILITY(3);
						else if (secondOpponentMove != MOVE_NONE
						&& PriorityCalc(foe1, ACTION_USE_MOVE, secondOpponentMove) > 0)
							INCREASE_STATUS_VIABILITY(3);
					}
					break;

				case MOVE_WIDEGUARD:
					if (IS_DOUBLE_BATTLE)
					{
						if (firstOpponentMove != MOVE_NONE
						&& gBattleMoves[firstOpponentMove].target & (MOVE_TARGET_ALL | MOVE_TARGET_BOTH))
							INCREASE_STATUS_VIABILITY(3);
						else if (secondOpponentMove != MOVE_NONE
						&& gBattleMoves[secondOpponentMove].target & (MOVE_TARGET_ALL | MOVE_TARGET_BOTH))
							INCREASE_STATUS_VIABILITY(3);
					}
					break;

				case MOVE_CRAFTYSHIELD:
					if (IS_DOUBLE_BATTLE)
					{
						if (firstOpponentMove != MOVE_NONE
						&& SPLIT(firstOpponentMove) == SPLIT_STATUS && !(gBattleMoves[firstOpponentMove].target & MOVE_TARGET_USER))
							INCREASE_STATUS_VIABILITY(3);
						else if (secondOpponentMove != MOVE_NONE
						&& SPLIT(secondOpponentMove) == SPLIT_STATUS && !(gBattleMoves[secondOpponentMove].target & MOVE_TARGET_USER))
							INCREASE_STATUS_VIABILITY(3);
					}
					break;
				
				case MOVE_MATBLOCK:
					if (IS_DOUBLE_BATTLE)
					{
						if (gDisableStructs[bankAtk].isFirstTurn)
						{
							if (firstOpponentMove != MOVE_NONE
							&& SPLIT(firstOpponentMove) != SPLIT_STATUS && !(gBattleMoves[firstOpponentMove].target & MOVE_TARGET_USER))
								INCREASE_STATUS_VIABILITY(3);
							else if (secondOpponentMove != MOVE_NONE
							&& SPLIT(secondOpponentMove) != SPLIT_STATUS && !(gBattleMoves[secondOpponentMove].target & MOVE_TARGET_USER))
								INCREASE_STATUS_VIABILITY(3);
						}
					}
					__attribute__ ((fallthrough));
					
				case MOVE_ENDURE:
					if (CanKnockOut(bankDef, bankAtk))
					{
						if (MoveEffectInMoveset(EFFECT_FLAIL, bankAtk)
						||  MoveEffectInMoveset(EFFECT_ENDEAVOR, bankAtk))
							INCREASE_STATUS_VIABILITY(3);
							
						if (IsPinchBerryItemEffect(atkItemEffect))
							INCREASE_STATUS_VIABILITY(3);
					}
					break;

				default:
					if (ShouldProtect(bankAtk, foe1, move) == USE_PROTECT)
					{
						if (IsClassStall(class))
							INCREASE_VIABILITY(3);
						else
							INCREASE_STATUS_VIABILITY(3);
					}
					else if (IS_DOUBLE_BATTLE
					&& ShouldProtect(bankAtk, foe2, move) == USE_PROTECT)
					{
						if (IsClassStall(class))
							INCREASE_VIABILITY(3);
						else
							INCREASE_STATUS_VIABILITY(3);
					}
			}
			break;

		case EFFECT_SPIKES: ;
			u8 firstId, lastId;
			struct Pokemon* atkParty = LoadPartyRange(bankAtk, &firstId, &lastId);
			struct Pokemon* defParty = LoadPartyRange(bankDef, &firstId, &lastId);
					
			switch (move) {
				case MOVE_STICKYWEB:

					for (i = 0; i < PARTY_SIZE; ++i) //Loop through attacker party
					{
						for (j = 0; j < PARTY_SIZE; ++j) //Loop through target party
						{
							if (GetMonData(&atkParty[i], MON_DATA_HP, NULL) > 0
							&&  GetMonData(&defParty[j], MON_DATA_HP, NULL) > 0
							&&  GetMonData(&atkParty[i], MON_DATA_SPECIES, NULL) != SPECIES_NONE
							&&  GetMonData(&defParty[j], MON_DATA_SPECIES, NULL) != SPECIES_NONE
							&&  !GetMonData(&atkParty[i], MON_DATA_IS_EGG, NULL)
							&&  !GetMonData(&defParty[j], MON_DATA_IS_EGG, NULL)
							&&  j != gBattlerPartyIndexes[foe1]
							&&  j != gBattlerPartyIndexes[foe2]
							&&  SpeedCalcForParty(SIDE(bankAtk), &atkParty[i]) < SpeedCalcForParty(SIDE(bankDef), &defParty[j]))
							{
								IncreaseEntryHazardsViability(&viability, class, bankAtk, bankDef, move);
								goto END_ENTRY_HAZARDS;
							}
						}
					}
					break;
					
				case MOVE_STEALTHROCK:
					IncreaseEntryHazardsViability(&viability, class, bankAtk, bankDef, move);
					break;
					
				case MOVE_TOXICSPIKES:
					for (i = 0; i < PARTY_SIZE; ++i)
					{
						if (GetMonData(&defParty[i], MON_DATA_HP, NULL) > 0
						&&  i != gBattlerPartyIndexes[foe1]
						&&  i != gBattlerPartyIndexes[foe2]
						&&  CanPartyMonBePoisoned(&defParty[i]))
						{
							IncreaseEntryHazardsViability(&viability, class, bankAtk, bankDef, move);
							break;
						}
					}
					break;
					
				default: //Spikes
					IncreaseEntryHazardsViability(&viability, class, bankAtk, bankDef, move);
					break;
			}
			
		END_ENTRY_HAZARDS:
			break;
			
		case EFFECT_FORESIGHT:
			if (move == MOVE_MIRACLEEYE)
			{
				if (STAT_STAGE(bankDef, STAT_STAGE_EVASION) > 6
				|| (IsOfType(bankDef, TYPE_DARK) && (MoveTypeInMoveset(TYPE_PSYCHIC, bankAtk))))
					INCREASE_STATUS_VIABILITY(2);
				break;
			}
			else
			{
				if (atkAbility == ABILITY_SCRAPPY)
					break;
				else if (STAT_STAGE(bankDef, STAT_STAGE_EVASION) > 6
				|| (IsOfType(bankDef, TYPE_GHOST)
				 && (MoveTypeInMoveset(TYPE_NORMAL, bankAtk)
				  || MoveTypeInMoveset(TYPE_FIGHTING, bankAtk))))
					INCREASE_STATUS_VIABILITY(2);
				break;
			}
			break;
			
		case EFFECT_PERISH_SONG:
			if (IsTrapped(bankDef, TRUE))
				INCREASE_STATUS_VIABILITY(3);
			break;
			
		case EFFECT_SANDSTORM:
			if (atkAbility == ABILITY_SANDVEIL
			|| atkAbility == ABILITY_SANDRUSH
			|| atkAbility == ABILITY_SANDFORCE
			|| atkAbility == ABILITY_SANDFORCE
			|| atkAbility == ABILITY_OVERCOAT
			|| atkAbility == ABILITY_MAGICGUARD
			|| atkItemEffect == ITEM_EFFECT_SAFETY_GOGGLES
			|| IsOfType(bankAtk, TYPE_ROCK)
			|| IsOfType(bankAtk, TYPE_STEEL) 
			|| IsOfType(bankAtk, TYPE_GROUND)
			|| MoveInMoveset(MOVE_SHOREUP, bankAtk)
			|| MoveInMoveset(MOVE_WEATHERBALL, bankAtk)
			|| atkItemEffect == ITEM_EFFECT_SMOOTH_ROCK)
				IncreaseStatusViability(&viability, class, 2, bankAtk, foe1);

			//Partner Checks
			else if (IS_DOUBLE_BATTLE)
			{
				if (atkPartnerAbility == ABILITY_SANDVEIL
				|| atkPartnerAbility == ABILITY_SANDRUSH
				|| atkPartnerAbility == ABILITY_SANDFORCE
				|| atkPartnerAbility == ABILITY_OVERCOAT
				|| atkPartnerAbility == ABILITY_MAGICGUARD
				|| atkPartnerItemEffect == ITEM_EFFECT_SAFETY_GOGGLES
				|| IsOfType(bankAtkPartner, TYPE_ROCK)
				|| IsOfType(bankAtkPartner, TYPE_STEEL) 
				|| IsOfType(bankAtkPartner, TYPE_GROUND)
				|| MoveInMoveset(MOVE_SHOREUP, bankAtkPartner)
				|| MoveInMoveset(MOVE_WEATHERBALL, bankAtkPartner))
					IncreaseStatusViability(&viability, class, 2, bankAtk, foe2);
			}
			break;
		
		case EFFECT_ROLLOUT:
			if (IsClassSweeper(class)
			&& atkStatus2 & STATUS2_DEFENSE_CURL)
				INCREASE_VIABILITY(8);
			break;
			
		case EFFECT_SWAGGER:
			if (MoveInMoveset(MOVE_FOULPLAY, bankAtk))
				INCREASE_STATUS_VIABILITY(2);
			else
				goto AI_CONFUSE_CHECK;
			break;
			
		case EFFECT_FURY_CUTTER:
			if (IsClassSweeper(class)
			&& atkItemEffect == ITEM_EFFECT_METRONOME)
				INCREASE_VIABILITY(3); //Past strongest move
			break;
			
		case EFFECT_ATTRACT:
			if (atkStatus1 & STATUS1_ANY
			|| atkStatus2 & STATUS2_CONFUSION
			|| IsTrapped(bankDef, TRUE))
				INCREASE_STATUS_VIABILITY(2);
			else
				INCREASE_STATUS_VIABILITY(1);
			break;
			
		case EFFECT_BATON_PASS:
			switch (move)
			{
				case MOVE_UTURN:
				case MOVE_VOLTSWITCH:
					if (ViableMonCountFromBank(bankAtk) <= 1)
						break;
					else if (gWishFutureKnock->wishCounter[bankAtk] > 0
					&& ShouldUseWishAromatherapy(bankAtk, bankDef, MOVE_WISH, class))
						INCREASE_VIABILITY(7);
					else if (atkItemEffect == ITEM_EFFECT_CHOICE_BAND)
						INCREASE_VIABILITY(3); //Past strongest move
					break;
				
				case MOVE_BATONPASS:
					if (IsClassBatonPass(class))
						INCREASE_VIABILITY(3);
					break;
			}
			break;
		
		case EFFECT_PURSUIT:
			if (IsClassSweeper(class))
			{
				if (IsPredictedToSwitch(bankDef, bankAtk))
					INCREASE_VIABILITY(3);
				else if (IsPredictedToUsePursuitableMove(bankDef, bankAtk) && !MoveWouldHitFirst(move, bankAtk, bankDef)) //Pursuit against fast U-Turn
					INCREASE_VIABILITY(3);
			}
			break;				
		
		case EFFECT_RAPID_SPIN:
			if (gSideAffecting[SIDE(bankAtk)] & SIDE_STATUS_SPIKES)
				INCREASE_STATUS_VIABILITY(3);
			else if (move == MOVE_DEFOG)
			{
				if (gSideAffecting[SIDE(bankDef)] & 
					(SIDE_STATUS_REFLECT | SIDE_STATUS_LIGHTSCREEN | SIDE_STATUS_SAFEGUARD | SIDE_STATUS_MIST)
				|| gNewBS->AuroraVeilTimers[SIDE(bankDef)] != 0)
					INCREASE_STATUS_VIABILITY(3);
				else if (!(gSideAffecting[SIDE(bankDef)] & SIDE_STATUS_SPIKES)) //Don't blow away hazards if you set them up
				{
					if (IS_DOUBLE_BATTLE)
					{
						if (gBattleMoves[partnerMove].effect == EFFECT_SPIKES //Partner is going to set up hazards
						&& !MoveWouldHitBeforeOtherMove(move, bankAtk, partnerMove, bankAtkPartner)) //Partner is going to set up before the potential Defog
							break; //Don't use Defog if partner is going to set up hazards
					}

					goto AI_EVASION_MINUS; //If no other reason to use Defog, use it for the Evasion lowering
				}
			}
			else //Rapid Spin
			{
				if (atkStatus3 & STATUS3_LEECHSEED
				|| atkStatus2 & STATUS2_WRAPPED)
					INCREASE_STATUS_VIABILITY(3);
			}
			break;
			
		case EFFECT_RAIN_DANCE:
			if (atkAbility == ABILITY_SWIFTSWIM 
			|| atkAbility == ABILITY_FORECAST 
			|| atkAbility == ABILITY_HYDRATION
			|| atkAbility == ABILITY_RAINDISH
			|| atkAbility == ABILITY_DRYSKIN
			|| MoveEffectInMoveset(EFFECT_THUNDER, bankAtk) //Includes Hurricane
			|| MoveEffectInMoveset(EFFECT_MORNING_SUN, foe1)
			|| MoveInMoveset(MOVE_WEATHERBALL, bankAtk)
			|| MoveTypeInMoveset(TYPE_WATER, bankAtk)
			|| MoveTypeInMoveset(TYPE_FIRE, foe1)
			|| atkItemEffect == ITEM_EFFECT_DAMP_ROCK)
				IncreaseStatusViability(&viability, class, 2, bankAtk, foe1);
			
			//Partner checks
			else if (IS_DOUBLE_BATTLE)
			{
				if (MoveEffectInMoveset(EFFECT_THUNDER, bankAtkPartner)
				|| MoveInMoveset(MOVE_WEATHERBALL, bankAtkPartner)
				|| atkPartnerItemEffect == ITEM_EFFECT_DAMP_ROCK
				|| atkPartnerAbility == ABILITY_SWIFTSWIM
				|| atkPartnerAbility == ABILITY_FORECAST
				|| atkPartnerAbility == ABILITY_HYDRATION
				|| atkPartnerAbility == ABILITY_RAINDISH
				|| atkPartnerAbility == ABILITY_DRYSKIN
				|| MoveEffectInMoveset(EFFECT_THUNDER, bankAtkPartner) //Includes Hurricane
				|| MoveEffectInMoveset(EFFECT_MORNING_SUN, foe2)
				|| MoveInMoveset(MOVE_WEATHERBALL, bankAtkPartner)
				|| MoveTypeInMoveset(TYPE_WATER, bankAtkPartner)
				|| MoveTypeInMoveset(TYPE_FIRE, foe2))
				{
					IncreaseStatusViability(&viability, class, 2, bankAtk, foe2);
				}
			}
			break;
			
		case EFFECT_SUNNY_DAY:
			if (atkAbility == ABILITY_CHLOROPHYLL
			|| atkAbility == ABILITY_FLOWERGIFT
			|| atkAbility == ABILITY_FORECAST
			|| atkAbility == ABILITY_LEAFGUARD
			|| atkAbility == ABILITY_SOLARPOWER
			|| atkAbility == ABILITY_HARVEST
			|| MoveEffectInMoveset(EFFECT_SOLARBEAM, bankAtk)
			|| MoveEffectInMoveset(EFFECT_MORNING_SUN, bankAtk)
			|| MoveEffectInMoveset(EFFECT_THUNDER, foe1)
			|| MoveInMoveset(MOVE_WEATHERBALL, bankAtk)
			|| MoveInMoveset(MOVE_GROWTH, bankAtk)
			|| MoveTypeInMoveset(TYPE_FIRE, bankAtk)
			|| MoveTypeInMoveset(TYPE_WATER, foe1)
			|| atkItemEffect == ITEM_EFFECT_HEAT_ROCK)
				IncreaseStatusViability(&viability, class, 2, bankAtk, foe1);
			
			// check double battle, partner
			else if (IS_DOUBLE_BATTLE)
			{
				if (atkPartnerAbility == ABILITY_CHLOROPHYLL
				|| atkPartnerAbility == ABILITY_FLOWERGIFT
				|| atkPartnerAbility == ABILITY_FORECAST
				|| atkPartnerAbility == ABILITY_LEAFGUARD
				|| atkPartnerAbility == ABILITY_SOLARPOWER
				|| atkPartnerAbility == ABILITY_HARVEST
				|| MoveEffectInMoveset(EFFECT_SOLARBEAM, bankAtkPartner)
				|| MoveEffectInMoveset(EFFECT_MORNING_SUN, bankAtkPartner)
				|| MoveEffectInMoveset(EFFECT_THUNDER, foe2)
				|| MoveInMoveset(MOVE_WEATHERBALL, bankAtkPartner)
				|| MoveInMoveset(MOVE_GROWTH, bankAtkPartner)
				|| MoveTypeInMoveset(TYPE_FIRE, bankAtkPartner)
				|| MoveTypeInMoveset(TYPE_WATER, foe2))
					IncreaseStatusViability(&viability, class, 2, bankAtk, foe2);
			}
			break;
			
		case EFFECT_ATTACK_UP_HIT:
			if (CalcSecondaryEffectChance(bankAtk, move) >= 75)
				goto AI_ATTACK_PLUS;
			else if (move == MOVE_FELLSTINGER)
			{
				if (MoveKnocksOutXHits(move, bankAtk, bankDef, 1))
					INCREASE_VIABILITY(3); //Past strongest move
				break;
			}
			break;
			
		case EFFECT_BELLY_DRUM:
			if (PhysicalMoveInMoveset(bankAtk) && atkAbility != ABILITY_CONTRARY)
			{
				if (IsTypeZCrystal(atkItem, moveType))
					INCREASE_STAT_VIABILITY(STAT_STAGE_ATK, STAT_STAGE_MAX, 5);
				else
					INCREASE_STAT_VIABILITY(STAT_STAGE_ATK, 8, 2);
			}
			break;
			
		case EFFECT_PSYCH_UP:
			if (move != MOVE_SPECTRALTHIEF)
			{
				//Copy positive stat changes
				for (i = STAT_STAGE_ATK; i < BATTLE_STATS_NO; ++i)
				{
					if (STAT_STAGE(bankDef, i) > STAT_STAGE(bankAtk, i))
					{
						if (i == STAT_STAGE_ATK && (PhysicalMoveInMoveset(bankAtk)))
						{
							INCREASE_STATUS_VIABILITY(1);
							break;
						}
						else if (i == STAT_STAGE_SPATK && (SpecialMoveInMoveset(bankAtk)))
						{
							INCREASE_STATUS_VIABILITY(1);
							break;
						}
						else if (i == STAT_STAGE_ACC || i == STAT_STAGE_EVASION || i == STAT_STAGE_SPEED)
						{
							INCREASE_STATUS_VIABILITY(1);
							break;
						}
						else if (IsClassStall(class)) //Defense and Sp. Defense
						{
							INCREASE_STATUS_VIABILITY(1);
							break;
						}
					}
				}
			}
			break;
			
		case EFFECT_SEMI_INVULNERABLE:
			if (IsClassSweeper(class))
			{
				if (MoveWouldHitFirst(move, bankAtk, bankDef))
				{
					if (gBattleMoves[predictedMove].effect == EFFECT_EXPLOSION
					||  gBattleMoves[predictedMove].effect == EFFECT_PROTECT)
						INCREASE_VIABILITY(3); //Past strongest move
				}
				else if (gBattleMoves[predictedMove].effect == EFFECT_SEMI_INVULNERABLE
				&& !BATTLER_SEMI_INVULNERABLE(bankDef))
					INCREASE_VIABILITY(3); //Past strongest move
			}
			break;
		
		case EFFECT_DEFENSE_CURL:
			if (MoveEffectInMoveset(EFFECT_ROLLOUT, bankAtk) && !(atkStatus2 & STATUS2_DEFENSE_CURL))
				INCREASE_STAT_VIABILITY(STAT_STAGE_DEF, STAT_STAGE_MAX, 5);
			else
				goto AI_DEFENSE_PLUS;
			break;
			
		case EFFECT_FAKE_OUT:
			if (gDisableStructs[bankAtk].isFirstTurn)
				INCREASE_VIABILITY(8);
			break;
			
		case EFFECT_STOCKPILE:
			if (MoveEffectInMoveset(EFFECT_SWALLOW, bankAtk)
			||  MoveEffectInMoveset(EFFECT_SPIT_UP, bankAtk))
				INCREASE_STATUS_VIABILITY(2);
			else
				goto AI_COSMIC_POWER;
			break;
			
		case EFFECT_HAIL:
			if (IsClassScreener(class)
			&& MoveInMoveset(MOVE_AURORAVEIL, bankAtk))
				INCREASE_VIABILITY(8);
			
			else if (atkAbility == ABILITY_SNOWCLOAK
			|| atkAbility == ABILITY_ICEBODY
			|| atkAbility == ABILITY_FORECAST
			|| atkAbility == ABILITY_SLUSHRUSH
			|| atkAbility == ABILITY_MAGICGUARD
			|| atkAbility == ABILITY_OVERCOAT
			|| MoveInMoveset(MOVE_BLIZZARD, bankAtk)
			|| MoveInMoveset(MOVE_AURORAVEIL, bankAtk)
			|| MoveInMoveset(MOVE_WEATHERBALL, bankAtk)
			|| MoveEffectInMoveset(EFFECT_MORNING_SUN, bankDef)
			|| atkItemEffect == ITEM_EFFECT_ICY_ROCK)
				IncreaseStatusViability(&viability, class, 2, bankAtk, foe1);
			
			//Partner checks
			else if (IS_DOUBLE_BATTLE)
			{
				if (atkPartnerAbility == ABILITY_SNOWCLOAK
				|| atkPartnerAbility == ABILITY_ICEBODY
				|| atkPartnerAbility == ABILITY_FORECAST
				|| atkPartnerAbility == ABILITY_SLUSHRUSH
				|| atkPartnerAbility == ABILITY_MAGICGUARD
				|| atkPartnerAbility == ABILITY_OVERCOAT
				|| MoveInMoveset(MOVE_BLIZZARD, bankAtkPartner)
				|| MoveInMoveset(MOVE_AURORAVEIL, bankAtkPartner)
				|| MoveInMoveset(MOVE_WEATHERBALL, bankAtkPartner)
				|| MoveEffectInMoveset(EFFECT_MORNING_SUN, bankDefPartner))
					IncreaseStatusViability(&viability, class, 2, bankAtk, foe2);
				break;
			}
			break;
		
		case EFFECT_TORMENT:
			INCREASE_STATUS_VIABILITY(0);
			break;
	
		case EFFECT_WILL_O_WISP:
		AI_BURN_CHECKS:
			if (CanBeBurned(bankDef, TRUE))
			{
				if (defItemEffect == ITEM_EFFECT_CURE_BRN
				|| defItemEffect == ITEM_EFFECT_CURE_STATUS
				|| defAbility == ABILITY_SHEDSKIN
				|| defAbility == ABILITY_FLAREBOOST
				|| defAbility == ABILITY_MAGICGUARD
				|| defAbility == ABILITY_MAGICGUARD)
					break;
					
				else if (MoveInMoveset(MOVE_HEX, bankAtk)
				|| MoveInMoveset(MOVE_HEX, bankAtkPartner)
				|| PhysicalMoveInMoveset(bankDef))
					INCREASE_STATUS_VIABILITY(2);
				else
					INCREASE_STATUS_VIABILITY(1);
			}

			break;
		
		case EFFECT_MEMENTO:
			//TODO
			break;
		
		
		case EFFECT_FOLLOW_ME:
			if (IS_DOUBLE_BATTLE)
			{
				INCREASE_STATUS_VIABILITY(3);
			}
			break;
			
		case EFFECT_NATURE_POWER:
			return AI_Script_Positives(bankAtk, bankDef, GetNaturePowerMove(), originalViability);

		case EFFECT_CHARGE:
			for (i = 0; i < MAX_MON_MOVES; ++i)
			{
				if (GetMoveTypeSpecial(bankAtk, gBattleMons[bankAtk].moves[i]) == TYPE_ELECTRIC
				&&  SPLIT(gBattleMons[bankAtk].moves[i]) != SPLIT_STATUS)
					INCREASE_STATUS_VIABILITY(2);			
			}
			
			goto AI_SPECIAL_DEFENSE_PLUS;
		
		case EFFECT_TAUNT:
			if (SPLIT(predictedMove) == SPLIT_STATUS)
				INCREASE_STATUS_VIABILITY(3);
			else if (StatusMoveInMoveset(bankDef))
				INCREASE_STATUS_VIABILITY(2);
			break;

		case EFFECT_HELPING_HAND:
			if (IS_DOUBLE_BATTLE)
			{
				u16 partnerMove = IsValidMovePrediction(bankAtkPartner, foe1);
				
				if (partnerMove != MOVE_NONE)
				{
					if (SPLIT(partnerMove) != SPLIT_STATUS)
					{
						INCREASE_STATUS_VIABILITY(1);
						break;
					}
				}
				
				partnerMove = IsValidMovePrediction(bankAtkPartner, foe2);
				
				if (partnerMove != MOVE_NONE)
				{
					if (SPLIT(partnerMove) != SPLIT_STATUS)
					{
						INCREASE_STATUS_VIABILITY(1);
						break;
					}
				}				
			}
			break;
			
		case EFFECT_TRICK: //+ Bestwo
			switch (atkItemEffect) {
				case ITEM_EFFECT_CHOICE_BAND: ;
					if (atkItemQuality == QUALITY_CHOICE_SCARF
					|| !MoveSplitInMoveset(atkItemQuality, bankDef)) //Target doesn't have beneficial move for choice item
						INCREASE_STATUS_VIABILITY(2);
					break;
	
				case ITEM_EFFECT_TOXIC_ORB:
					if (CanBePoisoned(bankDef, bankDef, FALSE)
					&& !(atkAbility == ABILITY_POISONHEAL
					 || atkAbility == ABILITY_TOXICBOOST
					 || atkAbility == ABILITY_QUICKFEET
					 || atkAbility == ABILITY_MAGICGUARD
					 || MoveInMoveset(MOVE_FACADE, bankAtk))
					&& !(defAbility == ABILITY_POISONHEAL
					 || defAbility == ABILITY_TOXICBOOST
					 || defAbility == ABILITY_QUICKFEET
					 || defAbility == ABILITY_MAGICGUARD
					 || MoveInMoveset(MOVE_FACADE, bankDef)))
						INCREASE_STATUS_VIABILITY(2);
					break;
					
				case ITEM_EFFECT_FLAME_ORB:
					if (CanBeBurned(bankDef, FALSE)
					&& !(atkAbility == ABILITY_GUTS
					 || atkAbility == ABILITY_FLAREBOOST
					 || atkAbility == ABILITY_MAGICGUARD
					 || MoveInMoveset(MOVE_FACADE, bankAtk))
					&& !(defAbility == ABILITY_GUTS
					 || defAbility == ABILITY_FLAREBOOST
					 || defAbility == ABILITY_MAGICGUARD
					 || MoveInMoveset(MOVE_FACADE, bankDef)))
						INCREASE_STATUS_VIABILITY(2);
					break;
					
				case ITEM_EFFECT_BLACK_SLUDGE:
					if (!IsOfType(bankDef, TYPE_POISON))
						INCREASE_STATUS_VIABILITY(3);
					break;
					
				case ITEM_EFFECT_IRON_BALL:
					if (!MoveInMoveset(MOVE_FLING, bankDef) || CheckGrounding(bankDef) != GROUNDED)
						INCREASE_STATUS_VIABILITY(2);
					break;
					
				case ITEM_EFFECT_LAGGING_TAIL:
				case ITEM_EFFECT_STICKY_BARB:
					INCREASE_STATUS_VIABILITY(3);
					break;
						
				default:
					if (move != MOVE_BESTOW && atkItem == ITEM_NONE)
					{
						switch (defItemEffect) {
							case ITEM_EFFECT_CHOICE_BAND:
								break;
				
							case ITEM_EFFECT_TOXIC_ORB:
								if (CanBePoisoned(bankAtk, bankAtk, FALSE)
								&& (atkAbility == ABILITY_POISONHEAL
								 || atkAbility == ABILITY_TOXICBOOST
								 || atkAbility == ABILITY_QUICKFEET
								 || atkAbility == ABILITY_MAGICGUARD
								 || MoveInMoveset(MOVE_FACADE, bankAtk)))
									INCREASE_STATUS_VIABILITY(2);
								break;
								
							case ITEM_EFFECT_FLAME_ORB:
								if (CanBeBurned(bankAtk, FALSE)
								&& (atkAbility == ABILITY_GUTS
								 || atkAbility == ABILITY_FLAREBOOST
								 || atkAbility == ABILITY_MAGICGUARD
								 || MoveInMoveset(MOVE_FACADE, bankAtk)))
									INCREASE_STATUS_VIABILITY(2);
								break;
								
							case ITEM_EFFECT_BLACK_SLUDGE:
								if (IsOfType(bankAtk, TYPE_POISON))
									INCREASE_STATUS_VIABILITY(3);
								break;
								
							case ITEM_EFFECT_IRON_BALL:
								if (MoveInMoveset(MOVE_FLING, bankAtk))
									INCREASE_STATUS_VIABILITY(2);
								break;
								
							case ITEM_EFFECT_LAGGING_TAIL:
							case ITEM_EFFECT_STICKY_BARB:
								break;
								
							default:
								INCREASE_STATUS_VIABILITY(1);
						}
					}
			}
			break;
		
		case EFFECT_ROLE_PLAY:
			//To do
			break;

		case EFFECT_WISH:
			if (ShouldUseWishAromatherapy(bankAtk, bankDef, move, class))
				INCREASE_VIABILITY(8);
			break;

		case EFFECT_INGRAIN: //+ Aqua Ring
			if (atkItemEffect == ITEM_EFFECT_BIG_ROOT)
				INCREASE_STATUS_VIABILITY(2);
			else
				INCREASE_STATUS_VIABILITY(1);
			break;
		
		case EFFECT_MAGIC_COAT:
			if (SPLIT(predictedMove) == SPLIT_STATUS && gBattleMoves[predictedMove].target & (MOVE_TARGET_SELECTED | MOVE_TARGET_OPPONENTS_FIELD | MOVE_TARGET_BOTH))
				INCREASE_STATUS_VIABILITY(3);
			break;

		case EFFECT_RECYCLE:
			//If saved item exists and bank not holding anything
			if (SAVED_CONSUMED_ITEMS(bankAtk) != 0 && atkItem == 0)		
				INCREASE_STATUS_VIABILITY(1);
			break;
		
		case EFFECT_BRICK_BREAK:
			if (IsClassSweeper(class))
			{
				if (gSideAffecting[SIDE(bankDef)] & 
					(SIDE_STATUS_REFLECT | SIDE_STATUS_LIGHTSCREEN)
				|| gNewBS->AuroraVeilTimers[SIDE(bankDef)] > 0)
					INCREASE_VIABILITY(3); //Increase past strongest move
			}
			break;

		case EFFECT_KNOCK_OFF:
			if (CanKnockOffItem(bankDef))
			{
				switch (defItemEffect) {
					case ITEM_EFFECT_IRON_BALL:
					case ITEM_EFFECT_LAGGING_TAIL:
					case ITEM_EFFECT_STICKY_BARB:
						break;
					
					default:
						INCREASE_VIABILITY(3); //Increase past strongest move
				}
			}
			break;
		
		case EFFECT_SKILL_SWAP:
			switch (move) {
				case MOVE_WORRYSEED:
				case MOVE_GASTROACID:
				case MOVE_SIMPLEBEAM:
					if (gAbilityRatings[defAbility] >= 5)
						INCREASE_STATUS_VIABILITY(2);
					break;

				case MOVE_ENTRAINMENT:
					if (gAbilityRatings[defAbility] >= 5
					||  gAbilityRatings[atkAbility] <= 0)
						INCREASE_STATUS_VIABILITY(2);
					break;

				case MOVE_SKILLSWAP:
					if (gAbilityRatings[defAbility] > gAbilityRatings[atkAbility])
						INCREASE_STATUS_VIABILITY(1);
					break;
			}
			break;

		case EFFECT_IMPRISON:
			if (MoveInMoveset(predictedMove, bankAtk)) //The attacker knows the move the opponent is about to use
				INCREASE_STATUS_VIABILITY(3);
			else
				INCREASE_STATUS_VIABILITY(1);
			break;

		case EFFECT_REFRESH:
			switch (move) {
				case MOVE_PSYCHOSHIFT:
					if (atkStatus1 & STATUS1_PSN_ANY)
						goto AI_POISON_CHECKS;
					else if (atkStatus1 & STATUS1_BURN)
						goto AI_BURN_CHECKS;
					else if (atkStatus1 & STATUS1_PARALYSIS)
						goto AI_PARALYZE_CHECKS;
					else if (atkStatus1 & STATUS1_SLEEP)
						goto AI_SLEEP_CHECKS;
					break;

				case MOVE_REFRESH:
					if (atkStatus1 & STATUS1_ANY)
						INCREASE_STATUS_VIABILITY(3);
					break;
			}
			break;
		
		case EFFECT_GRUDGE:
			break;

		case EFFECT_SNATCH:
			if (gBattleMoves[predictedMove].flags & FLAG_SNATCH_AFFECTED)
				INCREASE_STATUS_VIABILITY(3); //Steal move
			break;
		
		case EFFECT_MUD_SPORT:
			if (!MoveTypeInMoveset(TYPE_ELECTRIC, bankAtk)
			&& (MoveTypeInMoveset(TYPE_ELECTRIC, foe1) || MoveTypeInMoveset(TYPE_ELECTRIC, foe2)))
				INCREASE_STATUS_VIABILITY(1);
			break;
		
		case EFFECT_TICKLE:
			if (STAT_STAGE(bankDef, STAT_STAGE_DEF) > 4 && PhysicalMoveInMoveset(bankAtk) && defAbility != ABILITY_CONTRARY)
				goto AI_DEFENSE_MINUS;
			else
				goto AI_ATTACK_MINUS;
			break;
			
		case EFFECT_COSMIC_POWER:
		AI_COSMIC_POWER: ;
			if (atkAbility != ABILITY_CONTRARY)
			{
				if (STAT_STAGE(bankAtk, STAT_STAGE_DEF) < 10 || IsClassBatonPass(class))
					goto AI_DEFENSE_PLUS;
				else
					goto AI_SPECIAL_DEFENSE_PLUS;
			}
			break;
			
		case EFFECT_BULK_UP:
			if (atkAbility != ABILITY_CONTRARY)
			{
				if (STAT_STAGE(bankAtk, STAT_STAGE_ATK) < 8 || IsClassBatonPass(class))
					goto AI_ATTACK_PLUS;
				else
					goto AI_DEFENSE_PLUS;
			}
			break;

		case EFFECT_WATER_SPORT:
			if (!MoveTypeInMoveset(TYPE_FIRE, bankAtk)
			&& (MoveTypeInMoveset(TYPE_FIRE, foe1) || MoveTypeInMoveset(TYPE_FIRE, foe2)))
				INCREASE_STATUS_VIABILITY(1);
			break;
			
		case EFFECT_CALM_MIND:
			if (atkAbility != ABILITY_CONTRARY)
			{
				switch (move) {
					case MOVE_GEOMANCY:
						if (atkItemEffect == ITEM_EFFECT_POWER_HERB)
							INCREASE_STAT_VIABILITY(STAT_STAGE_SPEED, STAT_STAGE_MAX, 3);
						__attribute__ ((fallthrough));

					case MOVE_QUIVERDANCE:
						if (SpeedCalc(bankAtk) <= SpeedCalc(bankDef) || IsClassBatonPass(class))
							goto AI_SPEED_PLUS;
						__attribute__ ((fallthrough));

					default:
						if ((IsClassBatonPass(class) && STAT_STAGE(bankAtk, STAT_STAGE_SPATK) < 4)
						|| STAT_STAGE(bankAtk, STAT_STAGE_SPATK) < 8)
							goto AI_SPECIAL_ATTACK_PLUS;
						else
							goto AI_SPECIAL_DEFENSE_PLUS;
				}					
			}
			break;

		case EFFECT_DRAGON_DANCE:
			if (atkAbility != ABILITY_CONTRARY)
			{
				switch (move) {
					case MOVE_SHELLSMASH:
						if (atkItemEffect == ITEM_EFFECT_POWER_HERB)
							INCREASE_STAT_VIABILITY(STAT_STAGE_SPEED, STAT_STAGE_MAX, 3);
						else if (SpeedCalc(bankAtk) <= SpeedCalc(bankDef) || IsClassBatonPass(class))
							goto AI_SPEED_PLUS;
						else if ((IsClassBatonPass(class) && STAT_STAGE(bankAtk, STAT_STAGE_SPATK) < 10)
						|| (STAT_STAGE(bankAtk, STAT_STAGE_SPATK) < 8 && (SpecialMoveInMoveset(bankAtk))))
							goto AI_SPECIAL_ATTACK_PLUS;
						else
							goto AI_ATTACK_PLUS;
						__attribute__ ((fallthrough));

					default: //Dragon Dance + Shift Gear
						if (SpeedCalc(bankAtk) <= SpeedCalc(bankDef) || IsClassBatonPass(class))
							goto AI_SPEED_PLUS;
						else
							goto AI_ATTACK_PLUS;
						break;
				}
			}
			break;

		case EFFECT_STAT_SWAP_SPLIT:
			switch (move)
			{
				case MOVE_GUARDSWAP: //Prevent's getting caught in an infinite loop
					if (STAT_STAGE(bankDef, STAT_STAGE_DEF) > STAT_STAGE(bankAtk, STAT_STAGE_DEF)
					&&  STAT_STAGE(bankDef, STAT_STAGE_SPDEF) >= STAT_STAGE(bankAtk, STAT_STAGE_SPDEF))
						INCREASE_STATUS_VIABILITY(1);
					else if (STAT_STAGE(bankDef, STAT_STAGE_SPDEF) > STAT_STAGE(bankAtk, STAT_STAGE_SPDEF)
					&&  STAT_STAGE(bankDef, STAT_STAGE_DEF) >= STAT_STAGE(bankAtk, STAT_STAGE_DEF))
						INCREASE_STATUS_VIABILITY(1);
					break;
					
				case MOVE_POWERSWAP:
					if (STAT_STAGE(bankDef, STAT_STAGE_ATK) > STAT_STAGE(bankAtk, STAT_STAGE_ATK)
					&&  STAT_STAGE(bankDef, STAT_STAGE_SPATK) >= STAT_STAGE(bankAtk, STAT_STAGE_SPATK))
						INCREASE_STATUS_VIABILITY(1);
					else if (STAT_STAGE(bankDef, STAT_STAGE_SPATK) > STAT_STAGE(bankAtk, STAT_STAGE_SPATK)
					&&  STAT_STAGE(bankDef, STAT_STAGE_ATK) >= STAT_STAGE(bankAtk, STAT_STAGE_ATK))
						INCREASE_STATUS_VIABILITY(1);
					break;
					
				case MOVE_POWERTRICK:
					if (!(atkStatus3 & STATUS3_POWER_TRICK))
					{
						if (atkDefense > atkAttack && PhysicalMoveInMoveset(bankAtk))
							INCREASE_STATUS_VIABILITY(2);
						break;
					}
					break;
					
				case MOVE_HEARTSWAP: ;
					bool8 hasHigherStat = FALSE;

					//Only use if all target stats are >= attacker stats to prevent infinite loop
					for (i = STAT_STAGE_ATK; i < BATTLE_STATS_NO; ++i)
					{
						if (STAT_STAGE(bankDef, i) < STAT_STAGE(bankAtk, i))
							break;
						
						if (STAT_STAGE(bankDef, i) > STAT_STAGE(bankAtk, i))
							hasHigherStat = TRUE;
					}
					
					if (hasHigherStat && i == BATTLE_STATS_NO)
						INCREASE_STATUS_VIABILITY(1);
					break;
					
				case MOVE_SPEEDSWAP:
					if (defSpeed > atkSpeed)
						INCREASE_STATUS_VIABILITY(3);
					break;
					
				case MOVE_GUARDSPLIT: ;
					u16 newDefense = (atkDefense + defDefense) / 2;
					u16 newSpDef = (atkSpDef + defSpDef) / 2;
					
					if ((newDefense > atkDefense && newSpDef >= atkSpDef)
					|| (newSpDef > atkSpDef && newDefense >= atkDefense))
						INCREASE_STATUS_VIABILITY(1);
					break;
					
				case MOVE_POWERSPLIT: ;
					u16 newAttack = (atkAttack + defAttack) / 2;
					u16 newSpAtk = (atkSpAtk + defSpAtk) / 2;
					
					if ((newAttack > atkAttack && newSpAtk >= atkSpAtk)
					|| (newSpAtk > atkSpAtk && newAttack >= atkAttack))
						INCREASE_STATUS_VIABILITY(1);
					break;
			}
			break;

		case EFFECT_EAT_BERRY:
			switch (move)
			{
				case MOVE_BUGBITE:
				case MOVE_PLUCK:		
					if (defStatus2 & STATUS2_SUBSTITUTE
					||  defAbility == ABILITY_STICKYHOLD)
						break;
					else if (IsBerry(defItem))
						INCREASE_VIABILITY(3); //Increase past strongest move
					break;
					
				case MOVE_INCINERATE:
					if (defStatus2 & STATUS2_SUBSTITUTE
					||  defAbility == ABILITY_STICKYHOLD)
						break;
					else if (IsBerry(defItem) || IsGem(defItem))
						INCREASE_VIABILITY(3); //Increase past strongest move
					break;
			}
			break;

		case EFFECT_SMACK_DOWN:
			if (CheckGrounding(bankDef) != GROUNDED)
				INCREASE_VIABILITY(3); //Increase past strongest move
			break;
			
		case EFFECT_REMOVE_TARGET_STAT_CHANGES:
			if (ShouldPhaze(bankAtk, bankDef, move, class))
				INCREASE_VIABILITY(8);
			break;
			
		case EFFECT_RELIC_SONG:
			if (atkSpecies == SPECIES_MELOETTA && defDefense < defSpDef) //Change to pirouette if can do more damage
				INCREASE_VIABILITY(3); //Increase past strongest move
			else if (atkSpecies == SPECIES_MELOETTA_PIROUETTE && defSpDef < defDefense) //Change to Aria if can do more damage
				INCREASE_VIABILITY(3); //Increase past strongest move
//			else
//				goto AI_SLEEP_CHECKS;
			break;
			
		case EFFECT_SET_TERRAIN:
			INCREASE_STATUS_VIABILITY(2);
		break;
			
		case EFFECT_PLEDGE:
			if (IS_DOUBLE_BATTLE)
			{
				if (MoveEffectInMoveset(EFFECT_PLEDGE, bankAtkPartner))
					INCREASE_VIABILITY(3); //Increase past strongest move if partner might use pledge move
			}
			break;
			
		case EFFECT_FIELD_EFFECTS:
			switch (move) {
				case MOVE_TRICKROOM:
					if (!gNewBS->TrickRoomTimer
					&& GetTeamSpeedAverage(bankAtk) < GetTeamSpeedAverage(foe1))
						INCREASE_STATUS_VIABILITY(3);
					else if (gNewBS->TrickRoomTimer
					&& GetTeamSpeedAverage(bankAtk) >= GetTeamSpeedAverage(foe1))
						INCREASE_STATUS_VIABILITY(3);
					break;

				case MOVE_MAGICROOM:
					INCREASE_STATUS_VIABILITY(1);
					break;
		
				case MOVE_WONDERROOM:
					if ((PhysicalMoveInMoveset(foe1) && atkDefense < atkSpDef)
					||  (SpecialMoveInMoveset(foe1) && atkSpDef < atkDefense))
						INCREASE_STATUS_VIABILITY(2);
					break;
	
				case MOVE_GRAVITY:
					if (gNewBS->GravityTimer == 0)
						INCREASE_STATUS_VIABILITY(2);
					break;

				case MOVE_IONDELUGE:
					if ((atkAbility == ABILITY_VOLTABSORB
					||  atkAbility == ABILITY_MOTORDRIVE
					||  atkAbility == ABILITY_LIGHTNINGROD)
					 && GetMoveTypeSpecial(foe1, predictedMove) == TYPE_NORMAL)
						INCREASE_STATUS_VIABILITY(2);
					else if (IS_DOUBLE_BATTLE
					&&  (GetMoveTypeSpecial(foe1, predictedMove) == TYPE_NORMAL || GetMoveTypeSpecial(foe2, predictedMove) == TYPE_NORMAL)
					&&  (atkPartnerAbility == ABILITY_VOLTABSORB
					  || atkPartnerAbility == ABILITY_MOTORDRIVE
					  || atkPartnerAbility == ABILITY_LIGHTNINGROD))
						INCREASE_STATUS_VIABILITY(2);
					break;
			}
			break;
			
		case EFFECT_FLING: ;
			u8 effect = gFlingTable[atkItem].effect;
			
			switch (effect) {
				case MOVE_EFFECT_BURN:
					goto AI_BURN_CHECKS;
				
				case MOVE_EFFECT_FLINCH:
					goto AI_FLINCH_CHECKS;
				
				case MOVE_EFFECT_PARALYSIS:
					goto AI_PARALYZE_CHECKS;
					
				case MOVE_EFFECT_POISON:
				case MOVE_EFFECT_TOXIC:
					goto AI_POISON_CHECKS;
			}
			break;
			
		case EFFECT_FEINT:
			if (gBattleMoves[predictedMove].effect == EFFECT_PROTECT)
				INCREASE_VIABILITY(3);
			break;
		
		case EFFECT_ATTACK_BLOCKERS:
			switch (move) {
				case MOVE_EMBARGO:
					if (defItemEffect != ITEM_EFFECT_NONE)
						INCREASE_STATUS_VIABILITY(1);
					break;

				case MOVE_POWDER:
					if (SPLIT(predictedMove) != SPLIT_STATUS
					&& GetMoveTypeSpecial(bankDef, predictedMove) == TYPE_FIRE)
						INCREASE_STATUS_VIABILITY(3);
					break;

				case MOVE_TELEKINESIS:
					if (MoveInMovesetWithAccuracyLessThan(bankAtk, bankDef, 90, FALSE))
						INCREASE_STATUS_VIABILITY(1);
					break;
					
				case MOVE_THROATCHOP:
					if (MoveWouldHitFirst(move, bankAtk, bankDef) && CheckSoundMove(predictedMove))
						INCREASE_STATUS_VIABILITY(3);
					else if (IsClassSweeper(class) && SoundMoveInMoveset(bankDef))
						INCREASE_VIABILITY(3); //Past strongest move	
					break;
					
				default: //Heal Block
					if (MoveWouldHitFirst(move, bankAtk, bankDef) && IsMovePredictionHealingMove(bankDef, bankAtk))
						INCREASE_STATUS_VIABILITY(3); //Try to cancel move
					
					else if (HealingMoveInMoveset(bankDef)
					|| defItemEffect == ITEM_EFFECT_LEFTOVERS
					|| (defItemEffect == ITEM_EFFECT_BLACK_SLUDGE && IsOfType(bankDef, TYPE_POISON)))
						INCREASE_STATUS_VIABILITY(2);
			}
			break;
			
		case EFFECT_TYPE_CHANGES:
			switch (move) {
				case MOVE_SOAK:
					if (IsClassSweeper(class)
					&& (MoveTypeInMoveset(bankAtk, TYPE_ELECTRIC) || MoveTypeInMoveset(bankAtk, TYPE_GRASS)))
						INCREASE_STATUS_VIABILITY(2); //Get some super effective moves
					break;

				case MOVE_TRICKORTREAT:
				case MOVE_FORESTSCURSE:
					if (defAbility == ABILITY_WONDERGUARD)
						INCREASE_STATUS_VIABILITY(2); //Give it more weaknesses
					break;
			}
			break;
			
		//case EFFECT_HEAL_TARGET:
			//Nothing to see here
			//break;
			
		case EFFECT_TOPSY_TURVY_ELECTRIFY:
			switch (move) {
				case MOVE_ELECTRIFY:
					if (GetMoveTypeSpecial(bankDef, predictedMove) == TYPE_NORMAL
					&& (atkAbility == ABILITY_VOLTABSORB
					 || atkAbility == ABILITY_MOTORDRIVE
					 || atkAbility == ABILITY_LIGHTNINGROD))
					{
						INCREASE_STATUS_VIABILITY(3);
					}
					break;

				default: ; //Topsy Turvy
					u8 targetPositiveStages = CountBanksPositiveStatStages(bankDef);
					u8 targetNegativeStages = CountBanksNegativeStatStages(bankDef);
						
					if (targetPositiveStages > targetNegativeStages)
						INCREASE_STATUS_VIABILITY(1);
			}
			break;
			
		case EFFECT_FAIRY_LOCK_HAPPY_HOUR:
			switch (move) {
				case MOVE_FAIRYLOCK:
					if (!IsTrapped(foe1, TRUE) || !IsTrapped(foe2, TRUE))
					{
						if (ShouldTrap(bankAtk, bankDef, move, class))
							INCREASE_VIABILITY(8);
					}
					break;

				case MOVE_CELEBRATE:
				case MOVE_HOLDHANDS:
				case MOVE_HAPPYHOUR:
					if (IsTypeZCrystal(atkItem, moveType) && !gNewBS->ZMoveData->used[bankAtk])
						INCREASE_VIABILITY(9); //Z-Happy Hour! / Z-Celebrate
					break;
			}
			break;
			
		case EFFECT_INSTRUCT_AFTER_YOU_QUASH:
			if (IS_DOUBLE_BATTLE)
			{
				switch (move) {
					case MOVE_QUASH:
						INCREASE_STATUS_VIABILITY(1);
				}
			}
		break;
			
		case EFFECT_TEAM_EFFECTS:
			switch (move)
			{
				case MOVE_TAILWIND:
					if (IsClassTeamSupport(class))
						INCREASE_STATUS_VIABILITY(2);
					else
						INCREASE_STATUS_VIABILITY(1);
					break;
					
				case MOVE_LUCKYCHANT:
					INCREASE_STATUS_VIABILITY(1);
					break;
					
				case MOVE_MAGNETRISE:
					if (!IS_DOUBLE_BATTLE)
					{
						if (CheckGrounding(bankAtk) == GROUNDED
						&& DamagingMoveTypeInMoveset(foe1, TYPE_GROUND)
						&& !(AI_SpecialTypeCalc(MOVE_EARTHQUAKE, foe1, bankAtk) & (MOVE_RESULT_DOESNT_AFFECT_FOE | MOVE_RESULT_NOT_VERY_EFFECTIVE))) //Doesn't resist ground move
						{
							if (MoveWouldHitFirst(move, bankAtk, bankDef) //Attacker goes first
							&&  GetMoveTypeSpecial(foe1, predictedMove) == TYPE_GROUND)
							{
								INCREASE_STATUS_VIABILITY(3); //Cause the enemy's move to fail
							}
							else //Opponent Goes First
							{
								if (GetMoveTypeSpecial(foe1, predictedMove) == TYPE_GROUND)
									INCREASE_STATUS_VIABILITY(2); //If he's using it now, he'll probably use it again later
								else
									INCREASE_STATUS_VIABILITY(1);
							}
						}
					}
					else //Double Battle
					{
						if (CheckGrounding(bankAtk) == GROUNDED
						&& !(AI_SpecialTypeCalc(MOVE_EARTHQUAKE, foe1, bankAtk) & (MOVE_RESULT_DOESNT_AFFECT_FOE | MOVE_RESULT_NOT_VERY_EFFECTIVE)) //Doesn't resist ground move
						&& (DamagingMoveTypeInMoveset(foe1, TYPE_GROUND)
						 || DamagingMoveTypeInMoveset(foe2, TYPE_GROUND)
						 || DamagingAllHitMoveTypeInMoveset(bankAtkPartner, TYPE_GROUND)))
						{
							INCREASE_STATUS_VIABILITY(2); //Fix more later
						}
					}
					break;
			}
			break;
			
		case EFFECT_CAMOUFLAGE:
			if (MoveWouldHitFirst(move, bankAtk, bankDef)
			&& SPLIT(move) != SPLIT_STATUS
			&& !(AI_SpecialTypeCalc(predictedMove, bankDef, bankAtk) & MOVE_RESULT_DOESNT_AFFECT_FOE))
				INCREASE_STATUS_VIABILITY(1);
			break;
			
		case EFFECT_FLAMEBURST:
			if (IS_DOUBLE_BATTLE)
			{
				if (IsClassSweeper(class)
				&& BATTLER_ALIVE(bankDefPartner)
				&& GetHealthPercentage(bankDefPartner) < 12
				&& ABILITY(bankDefPartner) != ABILITY_MAGICGUARD
				&& !IsOfType(bankDefPartner, TYPE_FIRE))
					INCREASE_VIABILITY(3); //Past strongest move to wipe out other foe
			}
			break;
			
		case EFFECT_LASTRESORT_SKYDROP:
			if (move == MOVE_SKYDROP)
			{
				if (IsClassSweeper(class)
				&& IsTakingSecondaryDamage(bankDef))
					INCREASE_VIABILITY(3); //Past strongest move
			}
			break;	
	}

	if (moveSplit != SPLIT_STATUS)
	{
		if (!IS_DOUBLE_BATTLE)
		{
			//Every spread type has the same viability increases for these two
			if (MoveKnocksOutGoesFirstWithBestAccuracy(move, bankAtk, bankDef))
			{
				//If the AI's best killing move has a low accuracy, then
				//try to make it's chance of hitting higher.
				if (AccuracyCalc(move, bankAtk, bankDef) >= 70
				|| !MoveThatCanHelpAttacksHitInMoveset(bankAtk)
				|| CanKnockOut(bankDef, bankAtk)) //Just use the move if you'll die anyways
					INCREASE_VIABILITY(9);
			}
			else if (IsClassSweeper(class)
			&& MoveKnocksOutXHits(predictedMove, bankDef, bankAtk, 1) //Foe can kill attacker
			&& StrongestMoveGoesFirst(move, bankAtk, bankDef)) //Use fastest move
			{
				INCREASE_VIABILITY(9);
			}
			else if (IsStrongestMove(move, bankAtk, bankDef, FALSE))
			{
				//If the attacker is slower than the target and the target is going to die
				//anyways, then do something else and let it die.
				if (MoveWouldHitFirst(move, bankAtk, bankDef)
				|| !WillFaintFromSecondaryDamage(bankDef)
				|| IsMovePredictionHealingMove(bankDef, bankAtk)
				|| atkAbility == ABILITY_MOXIE
				|| atkAbility == ABILITY_BEASTBOOST)
					INCREASE_VIABILITY(2);
			}
		}
		else //Double Battle
		{
			INCREASE_VIABILITY(GetDoubleKillingScore(move, bankAtk, bankDef));
		}
	}

	return MathMin(viability, 255);
}
