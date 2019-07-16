#include "../defines.h"
#include "../../include/random.h"
#include "../../include/constants/items.h"

#include "../../include/new/ability_tables.h"
#include "../../include/new/accuracy_calc.h"
#include "../../include/new/AI_Helper_Functions.h"
#include "../../include/new/ai_master.h"
#include "../../include/new/battle_start_turn_start.h"
#include "../../include/new/bs_helper_functions.h"
#include "../../include/new/damage_calc.h"
#include "../../include/new/helper_functions.h"
#include "../../include/new/item.h"
#include "../../include/new/general_bs_commands.h"
#include "../../include/new/move_tables.h"

extern move_effect_t SetStatusTable[];
extern move_effect_t StatLowerTable[];
extern move_effect_t ConfusionTable[];
extern species_t gTelekinesisBanList[];

//This file's functions:
static void AI_Flee(void);
static void AI_Watch(void);

u8 AI_Script_Negatives(const u8 bankAtk, const u8 bankDef, const u16 originalMove, const u8 originalViability)
{
	u8 decreased;
	u16 predictedMove = IsValidMovePrediction(bankDef, bankAtk); //The move the target is likely to make against the attacker
	u32 i;
	u8 viability = originalViability;
	
	u16 move = TryReplaceMoveWithZMove(bankAtk, originalMove);

	u16 atkSpecies = SPECIES(bankAtk);
	u16 defSpecies = SPECIES(bankDef);
	u8 atkAbility = ABILITY(bankAtk);
	u8 defAbility = ABILITY(bankDef);
	
	if (!NO_MOLD_BREAKERS(atkAbility, move)
	&& gMoldBreakerIgnoredAbilities[defAbility])
		defAbility = ABILITY_NONE;
	
	u8 atkEffect = ITEM_EFFECT(bankAtk);	//unused
	u8 defEffect = ITEM_EFFECT(bankDef);
	u16 defItem = ITEM(bankDef);
	u16 atkItem = ITEM(bankAtk);

	u8 atkQuality = ITEM_QUALITY(bankAtk);
	//u8 defQuality = ITEM_QUALITY(bankDef);	//unused
	u32 atkStatus1 = gBattleMons[bankAtk].status1;
	u32 defStatus1 = gBattleMons[bankDef].status1;
	u32 atkStatus2 = gBattleMons[bankAtk].status2;
	u32 defStatus2 = gBattleMons[bankDef].status2;
	u32 atkStatus3 = gStatuses3[bankAtk];
	u32 defStatus3 = gStatuses3[bankDef];
	u8 atkGender = GetGenderFromSpeciesAndPersonality(atkSpecies, gBattleMons[bankAtk].personality);
	u8 defGender = GetGenderFromSpeciesAndPersonality(defSpecies, gBattleMons[bankDef].personality);

	u8 moveEffect = gBattleMoves[move].effect;
	u8 moveSplit = CalcMoveSplit(bankAtk, move);
	u8 moveTarget = gBattleMoves[move].target;
	u8 moveType = GetMoveTypeSpecial(bankAtk, move);
	u8 moveFlags = gBattleMoves[move].flags;
	u16 moveAcc = AccuracyCalc(move, bankAtk, bankDef);

	//Load partner data
	u8 bankAtkPartner = (IsDoubleBattle()) ? PARTNER(bankAtk) : bankAtk;
	u8 bankDefPartner = (IsDoubleBattle()) ? PARTNER(bankDef) : bankDef;
	u8 atkPartnerAbility = (IsDoubleBattle()) ? ABILITY(bankAtkPartner) : ABILITY_NONE;
	u8 defPartnerAbility = (IsDoubleBattle()) ? ABILITY(bankDefPartner) : ABILITY_NONE;

	//Load Alternative targets
	u8 foe1, foe2;
	foe1 = FOE(bankAtk);
				
	if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
		foe2 = PARTNER(FOE(bankAtk));
	else
		foe2 = foe1;

	//Affects User Check
	if (moveTarget & MOVE_TARGET_USER || moveTarget & MOVE_TARGET_ALL)
		goto MOVESCR_CHECK_0;
	
	//Ranged Move Check
	if (moveTarget & (MOVE_TARGET_BOTH | MOVE_TARGET_FOES_AND_ALLY))
	{
		if (moveType == TYPE_ELECTRIC && ABILITY_ON_OPPOSING_FIELD(bankAtk, ABILITY_LIGHTNINGROD))
			return viability - 20;
		else if (moveType == TYPE_WATER && ABILITY_ON_OPPOSING_FIELD(bankAtk, ABILITY_STORMDRAIN))
			return viability - 20;
	}
	
	#ifdef AI_TRY_TO_KILL_RATE
		if (AI_THINKING_STRUCT->aiFlags == AI_SCRIPT_CHECK_BAD_MOVE //Only basic AI
		&& Random() % 100 < AI_TRY_TO_KILL_RATE
		&& DamagingMoveInMoveset(bankAtk))
		{
			if (MoveKnocksOutGoesFirstWithBestAccuracy(move, bankAtk, bankDef))
				viability += 7;

			else if (IsStrongestMove(move, bankAtk, bankDef))
				viability += 2;
		}
	#endif

	// Gravity Table Prevention Check
	if (gNewBS->GravityTimer != 0 && CheckTableForMove(move, GravityBanTable))
		return 0; //Can't select this move period

	// Ungrounded check
	if (CheckGrounding(bankDef) == IN_AIR && moveType == TYPE_GROUND)
		return viability = 0;

	// Powder Move Checks (safety goggles, defender has grass type, overcoat, and powder move table)
	if (CheckTableForMove(move, PowderTable) && !IsAffectedByPowder(bankDef))
		viability -= 10; //No return b/c could be reduced further by absorb abilities
		
	//Target Ability Checks
	if (NO_MOLD_BREAKERS(atkAbility, move))
	{
		switch (defAbility) { //Type-specific ability checks - primordial weather handled separately
			
			//Electric
			case ABILITY_VOLTABSORB:
				if (moveType == TYPE_ELECTRIC) // && (moveSplit != SPLIT_STATUS))
					return viability - 20;
				break;
					
			case ABILITY_MOTORDRIVE:
				if (moveType == TYPE_ELECTRIC)
					return viability - 20;
				break;
			
			case ABILITY_LIGHTNINGROD:
				if (moveType == TYPE_ELECTRIC)
					return viability - 20;
				break;
			
			// Water
			case ABILITY_WATERABSORB:
				if (moveType == TYPE_WATER)
					return viability - 20;
				break;
			
			case ABILITY_DRYSKIN:
				if (moveType == TYPE_WATER)
					return viability - 20;
				break;
			
			case ABILITY_STORMDRAIN:
				if (moveType == TYPE_WATER)
					return viability - 20;
				break;

			//case ABILITY_WATERCOMPACTION:
			//	if (moveType == TYPE_WATER)
			//		return viability - 10;
			//	break;

			// Fire	
			case ABILITY_FLASHFIRE:
				if (moveType == TYPE_FIRE)
					return viability - 20;
				break;

			//case ABILITY_HEATPROOF:
			//case ABILITY_WATERBUBBLE: //Handled by damage calc
			//	if (moveType == TYPE_FIRE) // && (moveSplit != SPLIT_STATUS))
			//		return viability - 10;
			//	break;

			// Grass
			case ABILITY_SAPSIPPER:
				if (moveType == TYPE_GRASS)
					return viability - 20;
				break;

			// Dark
			case ABILITY_JUSTIFIED:
				if (moveType == TYPE_DARK) // && (moveSplit != SPLIT_STATUS))
					return viability - 9;
				break;

			//Multiple move types
			case ABILITY_RATTLED:
				if ((moveSplit != SPLIT_STATUS)
				&& (moveType == TYPE_DARK || moveType == TYPE_GHOST || moveType == TYPE_BUG))
					return viability - 9;
				break;
			
			//Move category checks
			case ABILITY_SOUNDPROOF:
				if (CheckSoundMove(move))
					return viability - 10;
				break;
			
			case ABILITY_BULLETPROOF:
				if (CheckTableForMove(move, BallBombMoveTable))
					return viability - 10;
				break;
			
			case ABILITY_DAZZLING:
			case ABILITY_QUEENLYMAJESTY:
				if (PriorityCalc(bankAtk, ACTION_USE_MOVE, move) > 0) //Check if right num
					return viability - 10;
				break;
			
			case ABILITY_AROMAVEIL:
				if (CheckTableForMove(move, AromaVeilTable))
					return viability - 10;
				break;
			
			case ABILITY_SWEETVEIL:
				if (moveEffect == EFFECT_SLEEP || moveEffect == EFFECT_YAWN)
					return viability - 10;
				break;
			
			case ABILITY_FLOWERVEIL:
				if (IsOfType(bankDef, TYPE_GRASS)
				&& (CheckTableForMoveEffect(move, SetStatusTable) || CheckTableForMoveEffect(move, StatLowerTable)))
					return viability - 10;
				break;
			
			case ABILITY_MAGICBOUNCE:
				if (moveFlags & FLAG_MAGIC_COAT_AFFECTED)
					return viability - 20;
				break;
			
			case ABILITY_CONTRARY:
				if (CheckTableForMoveEffect(move, StatLowerTable))
					return viability - 20;
				break;
			
			case ABILITY_CLEARBODY:
			case ABILITY_FULLMETALBODY:
			case ABILITY_WHITESMOKE:
				if (CheckTableForMoveEffect(move, StatLowerTable))
					return viability - 10;
				break;
			
			case ABILITY_HYPERCUTTER:
				if ((moveEffect == EFFECT_ATTACK_DOWN ||  moveEffect == EFFECT_ATTACK_DOWN_2)
				&& move != MOVE_PLAYNICE && move != MOVE_NOBLEROAR && move != MOVE_TEARFULLOOK && move != MOVE_VENOMDRENCH)
					return viability - 10;
				break;
			
			case ABILITY_KEENEYE:
				if (moveEffect == EFFECT_ACCURACY_DOWN
				||  moveEffect == EFFECT_ACCURACY_DOWN_2)
					return viability - 10;
				break;
			
			case ABILITY_BIGPECKS:
				if (moveEffect == EFFECT_DEFENSE_DOWN
				||  moveEffect == EFFECT_DEFENSE_DOWN_2)
					return viability - 10;
				break;
			
			case ABILITY_DEFIANT:
			case ABILITY_COMPETITIVE:
				if (CheckTableForMoveEffect(move, StatLowerTable))
					return viability - 8; //Not 10 b/c move still works, just not recommended
				break;
			
			case ABILITY_COMATOSE:
				if (CheckTableForMoveEffect(move, SetStatusTable))
					return viability - 10;
				break;
				
			case ABILITY_SHIELDSDOWN:
				if (GetBankPartyData(bankDef)->species == SPECIES_MINIOR_SHIELD
				&&  CheckTableForMoveEffect(move, SetStatusTable))
					return viability - 10;
				break;
			
			case ABILITY_WONDERSKIN:
				if (moveSplit == SPLIT_STATUS)
					moveAcc = 50;
				break;
				
			case ABILITY_LEAFGUARD:
				if (WEATHER_HAS_EFFECT && (gBattleWeather & WEATHER_SUN_ANY)
				&& CheckTableForMoveEffect(move, SetStatusTable))
					return viability - 10;
				break;

			#ifndef OLD_PRANKSTER
			case ABILITY_PRANKSTER:
				if (IsOfType(bankDef, TYPE_DARK)
				&& moveSplit == SPLIT_STATUS
				&& !(moveTarget & (MOVE_TARGET_OPPONENTS_FIELD | MOVE_TARGET_USER))) //Directly strikes target
					return viability -= 10;
				break;
			#endif
		}
		
		//Target Partner Ability Check
		if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
		{
			switch (defPartnerAbility) {
				case ABILITY_LIGHTNINGROD:
					if (moveType == TYPE_ELECTRIC)
						return viability - 20;
					break;
				
				case ABILITY_STORMDRAIN:
					if (moveType == TYPE_WATER)
						return viability - 20;
					break;
				
				case ABILITY_SWEETVEIL:
					if (moveEffect == EFFECT_SLEEP || moveEffect == EFFECT_YAWN)
						return viability - 10;
					break;
				
				case ABILITY_FLOWERVEIL:
					if ((IsOfType(bankDef, TYPE_GRASS))
					&& (CheckTableForMoveEffect(move, SetStatusTable) || CheckTableForMoveEffect(move, StatLowerTable)))
						return viability - 10;
					break;
				
				case ABILITY_AROMAVEIL:
					if (CheckTableForMove(move, AromaVeilTable))
						return viability - 10;
					break;
					
				case ABILITY_DAZZLING:
				case ABILITY_QUEENLYMAJESTY:
					if (PriorityCalc(bankAtk, ACTION_USE_MOVE, move) > 0) //Check if right num
						return viability - 10;
					break;		
			}
		}
	}
	
	//Terrain Check
	if (CheckGrounding(bankDef) == GROUNDED)
	{
		switch (TerrainType) {
			case ELECTRIC_TERRAIN:
				if (moveEffect == EFFECT_SLEEP || moveEffect == EFFECT_YAWN)
					return viability - 10;
				break;
			
			case GRASSY_TERRAIN:
				if (move == MOVE_BULLDOZE || move == MOVE_EARTHQUAKE || move == MOVE_MAGNITUDE)
					return viability - 10;
				break;
			
			case MISTY_TERRAIN:
				if (CheckTableForMoveEffect(move, SetStatusTable) || CheckTableForMoveEffect(move, ConfusionTable))
					return viability - 10;
				break;
				
			case PSYCHIC_TERRAIN:
				if (PriorityCalc(bankAtk, ACTION_USE_MOVE, move) > 0)
					return viability - 10;
				break;
		}
	}

	//Throat Chop Check
	if (gNewBS->ThroatChopTimers[bankAtk] && CheckSoundMove(move))
		return 0; //Can't select this move period
			
	//Heal Block Check
	if (gNewBS->HealBlockTimers[bankAtk] && CheckHealingMove(move))
		return 0; //Can't select this move period
		
	//Primal Weather Check
	if (gBattleWeather & WEATHER_SUN_PRIMAL && moveType == TYPE_WATER && moveSplit != SPLIT_STATUS)
		return viability - 20;
	else if (gBattleWeather & WEATHER_RAIN_PRIMAL && moveType == TYPE_FIRE && moveSplit != SPLIT_STATUS)
		return viability - 20;
	
	// Check Move Effects
	MOVESCR_CHECK_0:
	switch (moveEffect) {
		
		case EFFECT_HIT:
			goto AI_STANDARD_DAMAGE;

		case EFFECT_SLEEP:
		AI_CHECK_SLEEP: ;
			if (!CanBePutToSleep(bankDef, TRUE)
			|| (MoveBlockedBySubstitute(move, bankAtk, bankDef)))
				viability -= 10;
			break;

		case EFFECT_ABSORB:
			if (defAbility == ABILITY_LIQUIDOOZE)
				viability -= 6;

			if (move == MOVE_STRENGTHSAP)
			{
				if (defAbility == ABILITY_CONTRARY)
					viability -= 10;
				else if (!STAT_CAN_FALL(bankDef, STAT_STAGE_ATK))
					viability -= 10;
				break;
			}
			break;

		case EFFECT_EXPLOSION:
		#ifdef OKAY_WITH_AI_SUICIDE
			if (NO_MOLD_BREAKERS(atkAbility, move) && ABILITY_PRESENT(ABILITY_DAMP))
				viability -= 10;
			else if (ViableMonCountFromBank(bankDef) > 1	//If the Target only has one PKMN left,
			|| !MoveKnocksOutXHits(move, bankAtk, bankDef, 1))		//and the AI can knock out the target,
				viability -= 4;								//then the AI can use Explosion to win the battle
		#else
			viability -= 10;
		#endif
			break;
		
		case EFFECT_DREAM_EATER:
			if (defAbility != ABILITY_COMATOSE && !(defStatus1 & STATUS1_SLEEP))
				viability -= 10;
			break;
			
		case EFFECT_MIRROR_MOVE: //May cause issues with priority calcs?
			switch (move) {
				case MOVE_COPYCAT:	
					if (MoveWouldHitFirst(move, bankAtk, bankDef))
					{
					COPYCAT_CHECK_LAST_MOVE:
						if (gNewBS->LastUsedMove == MOVE_NONE
						|| gNewBS->LastUsedMove == 0xFFFF
						|| CheckTableForMove(gNewBS->LastUsedMove, CopycatBanTable)
						|| FindMovePositionInMoveset(gNewBS->LastUsedMove, bankAtk) < 4) //If you have the move, use it directly
							viability -= 10;
						else
							return AI_Script_Negatives(bankAtk, bankDef, gNewBS->LastUsedMove, originalViability);
					}
					else
					{
						if (predictedMove == MOVE_NONE)
							goto COPYCAT_CHECK_LAST_MOVE;
						else if (CheckTableForMove(predictedMove, CopycatBanTable)
							 || FindMovePositionInMoveset(predictedMove, bankAtk) < 4)
						{
							viability -= 10;
						}
						else
							return AI_Script_Negatives(bankAtk, bankDef, predictedMove, originalViability);
					}
					break;

				default: //Mirror Move
					if (gBattleStruct->lastTakenMoveFrom[bankAtk][bankDef] != MOVE_NONE)
						return AI_Script_Negatives(bankAtk, bankDef, gBattleStruct->lastTakenMoveFrom[bankAtk][bankDef], originalViability);
					viability -= 10;
			}
			break;
			
		case EFFECT_SPLASH:
			if (!IsTypeZCrystal(atkItem, moveType) || gNewBS->ZMoveData->used[bankAtk])
				viability -= 10;
			break;

		case EFFECT_TELEPORT:
			viability -= 10;
			break;
		
		case EFFECT_ATTACK_UP:
		case EFFECT_ATTACK_UP_2:
			if (atkAbility != ABILITY_CONTRARY)
			{
				switch (move) {
					case MOVE_HONECLAWS:
						if (STAT_STAGE(bankAtk, STAT_STAGE_ATK) >= STAT_STAGE_MAX
						&& (STAT_STAGE(bankAtk, STAT_STAGE_ACC) >= STAT_STAGE_MAX || !PhysicalMoveInMoveset(bankAtk)))
							viability -= 10;
						break;

					default:
						if (STAT_STAGE(bankAtk, STAT_STAGE_ATK) >= STAT_STAGE_MAX || !PhysicalMoveInMoveset(bankAtk))
							viability -= 10;
				}
			}
			else
				viability -= 10;
			break;
		
		case EFFECT_DEFENSE_UP:
		case EFFECT_DEFENSE_UP_2:
		case EFFECT_DEFENSE_CURL:
			switch (move) {
				case MOVE_FLOWERSHIELD:
					if (!IsOfType(bankAtk, TYPE_GRASS)
					&&  !(gBattleTypeFlags & BATTLE_TYPE_DOUBLE && IsOfType(bankAtkPartner, TYPE_GRASS)))
						viability -= 10;
					break;

				case MOVE_MAGNETICFLUX:
					if (atkAbility == ABILITY_PLUS || atkAbility == ABILITY_MINUS)
						goto AI_COSMIC_POWER;

					if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
					{
						if (atkPartnerAbility == ABILITY_PLUS || atkPartnerAbility == ABILITY_MINUS)
						{
							if ((STAT_STAGE(bankAtkPartner, STAT_STAGE_DEF) >= STAT_STAGE_MAX) 
							&&  (STAT_STAGE(bankAtkPartner, STAT_STAGE_SPDEF) >= STAT_STAGE_MAX))
								viability -= 10;
						}
					}
					break;

				case MOVE_AROMATICMIST:
					if (!(gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
					|| gBattleMons[bankAtkPartner].hp == 0
					|| STAT_STAGE(bankAtkPartner, STAT_STAGE_SPDEF) >= STAT_STAGE_MAX)
						viability -= 10;
					break;

				default:
					if (atkAbility == ABILITY_CONTRARY || STAT_STAGE(bankAtk, STAT_STAGE_DEF) >= STAT_STAGE_MAX)
						viability -= 10;
			}
			break;

		case EFFECT_SPEED_UP:
		case EFFECT_SPEED_UP_2:
			if (atkAbility == ABILITY_CONTRARY || STAT_STAGE(bankAtk, STAT_STAGE_SPEED) >= STAT_STAGE_MAX)
				viability -= 10;
			break;
		
		case EFFECT_SPECIAL_ATTACK_UP: 
		case EFFECT_SPECIAL_ATTACK_UP_2:			
			switch(move) {
				case MOVE_GROWTH:
				case MOVE_WORKUP:
				AI_WORK_UP_CHECK: ;
					if (((STAT_STAGE(bankAtk,STAT_STAGE_ATK) >= STAT_STAGE_MAX || !PhysicalMoveInMoveset(bankAtk)) 
					  && (STAT_STAGE(bankAtk, STAT_STAGE_SPATK) >= STAT_STAGE_MAX || !SpecialMoveInMoveset(bankAtk)))
					|| atkAbility == ABILITY_CONTRARY)
						viability -= 10;
					break;
				
				case MOVE_ROTOTILLER:
					if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
					{
						if (!(IsOfType(bankAtk, TYPE_GRASS) 
							  && CheckGrounding(bankAtk) 
							  && atkAbility != ABILITY_CONTRARY 
							  && (STAT_STAGE(bankAtk, STAT_STAGE_ATK) <= STAT_STAGE_MAX || STAT_STAGE(bankAtk, STAT_STAGE_SPATK) <= STAT_STAGE_MAX))
						&&  !(IsOfType(bankAtkPartner, TYPE_GRASS) 
							  && CheckGrounding(bankAtkPartner) 
							  && atkPartnerAbility != ABILITY_CONTRARY
							  && (STAT_STAGE(bankAtkPartner, STAT_STAGE_ATK) <= STAT_STAGE_MAX || STAT_STAGE(bankAtkPartner, STAT_STAGE_SPATK) <= STAT_STAGE_MAX)))
						{
							viability -= 10;
						}
					}
					else if (!IsOfType(bankAtk, TYPE_GRASS) || !CheckGrounding(bankAtk) || atkAbility == ABILITY_CONTRARY)
						viability -= 10;
					break;
					
				case MOVE_GEARUP:
					if (atkAbility == ABILITY_PLUS || atkAbility == ABILITY_MINUS)
						goto AI_WORK_UP_CHECK;
					
					if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
					{
						if (atkPartnerAbility == ABILITY_PLUS || atkPartnerAbility == ABILITY_MINUS)
						{
							if ((STAT_STAGE(bankAtkPartner, STAT_STAGE_ATK) >= STAT_STAGE_MAX || !PhysicalMoveInMoveset(bankAtk)) 
							&&  (STAT_STAGE(bankAtkPartner, STAT_STAGE_SPATK) >= STAT_STAGE_MAX || !SpecialMoveInMoveset(bankAtk)))
								viability -= 10;
						}
					}					
					break;
				
				default:
					if (atkAbility == ABILITY_CONTRARY || STAT_STAGE(bankAtk, STAT_STAGE_SPATK) >= STAT_STAGE_MAX || !SpecialMoveInMoveset(bankAtk))
						viability -= 10;
			}
			break;
			
		case EFFECT_SPECIAL_DEFENSE_UP:
		case EFFECT_SPECIAL_DEFENSE_UP_2:
		AI_SPDEF_RAISE_1: ;
			if (atkAbility == ABILITY_CONTRARY || STAT_STAGE(bankAtk, STAT_STAGE_SPDEF) >= STAT_STAGE_MAX)
				viability -= 10;
			break;
		
		case EFFECT_ACCURACY_UP:
		case EFFECT_ACCURACY_UP_2:
			if (atkAbility == ABILITY_CONTRARY || STAT_STAGE(bankAtk, STAT_STAGE_ACC) >= STAT_STAGE_MAX)
				viability -= 10;
			break;
		
		case EFFECT_EVASION_UP:
		case EFFECT_EVASION_UP_2:
		case EFFECT_MINIMIZE:
			switch (move) {
				case MOVE_ACUPRESSURE:
					if (StatsMaxed(bankDef) || defAbility == ABILITY_CONTRARY)
						viability -= 10;
					break;
					
				default:
					if (atkAbility == ABILITY_CONTRARY || STAT_STAGE(bankAtk, STAT_STAGE_EVASION) >= STAT_STAGE_MAX)
						viability -= 10;
			}
			break;
		
		case EFFECT_ATTACK_DOWN:
		case EFFECT_ATTACK_DOWN_2:
			decreased = FALSE;
			switch (move) {
				case MOVE_VENOMDRENCH:
					if (!(defStatus1 & STATUS_PSN_ANY))
					{
						viability -= 10;
						decreased = TRUE;
						break;
					}
					//Poisoned target
					else if (STAT_STAGE(bankDef, STAT_STAGE_SPEED) == STAT_STAGE_MIN
						 && (STAT_STAGE(bankDef, STAT_STAGE_SPATK) == STAT_STAGE_MIN || !SpecialMoveInMoveset(bankDef))
						 && (STAT_STAGE(bankDef, STAT_STAGE_ATK) == STAT_STAGE_MIN || !PhysicalMoveInMoveset(bankDef)))
					{
						viability -= 10;
						decreased = TRUE;
					}
					break;
					
				case MOVE_PLAYNICE:
				case MOVE_NOBLEROAR:
				case MOVE_TEARFULLOOK:
					if ((STAT_STAGE(bankDef, STAT_STAGE_SPATK) == STAT_STAGE_MIN || !SpecialMoveInMoveset(bankDef))
					&&  (STAT_STAGE(bankDef, STAT_STAGE_ATK) == STAT_STAGE_MIN || !PhysicalMoveInMoveset(bankDef)))
					{
						viability -= 10;
						decreased = TRUE;
					}
					break;
				
				default:
					if (STAT_STAGE(bankDef, STAT_STAGE_ATK) == STAT_STAGE_MIN || !PhysicalMoveInMoveset(bankDef))
					{
						viability -= 10;
						decreased = TRUE;
					}
			}
			if (decreased) break;
			
		AI_SUBSTITUTE_CHECK:
			if (MoveBlockedBySubstitute(move, bankAtk, bankDef))
				viability -= 10;
			break;
			
		case EFFECT_DEFENSE_DOWN: 
		case EFFECT_DEFENSE_DOWN_2: 
			if (STAT_STAGE(bankDef, STAT_STAGE_DEF) == STAT_STAGE_MIN
			|| MoveBlockedBySubstitute(move, bankAtk, bankDef))
				viability -= 10;
			break;
		
		case EFFECT_SPEED_DOWN:
		case EFFECT_SPEED_DOWN_2: 
			if (STAT_STAGE(bankDef, STAT_STAGE_SPEED) == STAT_STAGE_MIN
			|| MoveBlockedBySubstitute(move, bankAtk, bankDef))
				viability -= 10;
			break;
		
		case EFFECT_SPECIAL_ATTACK_DOWN: 
		case EFFECT_SPECIAL_ATTACK_DOWN_2:
			if (move == MOVE_CAPTIVATE
			&& (atkGender == MON_GENDERLESS || defGender == MON_GENDERLESS || atkGender == defGender))
				viability -= 10;
			else if (STAT_STAGE(bankDef, STAT_STAGE_SPATK) == STAT_STAGE_MIN || !SpecialMoveInMoveset(bankDef)
			|| MoveBlockedBySubstitute(move, bankAtk, bankDef))
				viability -= 10;
			break;
		
		case EFFECT_SPECIAL_DEFENSE_DOWN:
		case EFFECT_SPECIAL_DEFENSE_DOWN_2:
			if (STAT_STAGE(bankDef, STAT_STAGE_SPDEF) == STAT_STAGE_MIN
			|| MoveBlockedBySubstitute(move, bankAtk, bankDef))
				viability -= 10;
			break;
		
		case EFFECT_ACCURACY_DOWN: 
		case EFFECT_ACCURACY_DOWN_2:
			if (STAT_STAGE(bankDef, STAT_STAGE_ACC) == STAT_STAGE_MIN
			|| MoveBlockedBySubstitute(move, bankAtk, bankDef))
				viability -= 10;
			break;
		
		case EFFECT_EVASION_DOWN: 
		case EFFECT_EVASION_DOWN_2:
		AI_LOWER_EVASION:
			if (STAT_STAGE(bankDef, STAT_STAGE_EVASION) == STAT_STAGE_MIN
			|| MoveBlockedBySubstitute(move, bankAtk, bankDef))
				viability -= 10;
			break;
		
		case EFFECT_HAZE:
		AI_HAZE_CHECK: ;
			decreased = FALSE;
			//Don't want to reset own high stats
			for (i = 0; i <= BATTLE_STATS_NO-1; ++i)
			{
				if (STAT_STAGE(bankAtk, i) > 6 || STAT_STAGE(bankAtkPartner, i) > 6) 
				{
					viability -= 10;
					decreased = TRUE;
					break;
				}
			}
			if (decreased) 
				break;
			
			//Don't want to reset enemy lowered stats
			for (i = 0; i <= BATTLE_STATS_NO-1; ++i)
			{
				if (STAT_STAGE(bankDef, i) < 6 || STAT_STAGE(bankDefPartner, i) < 6)
				{
					viability -= 10;
					break;
				}
			}
			break;
		
		case EFFECT_BIDE:
			if (!DamagingMoveInMoveset(bankDef)
			||  GetHealthPercentage(bankAtk) < 30 //Close to death
			||  defStatus1 & (STATUS1_SLEEP | STATUS1_FREEZE)) //No point in biding if can't take damage
				viability -= 10;
			break;

		case EFFECT_ROAR:
			switch (move) {
				case MOVE_DRAGONTAIL:
				case MOVE_CIRCLETHROW:
					goto AI_STANDARD_DAMAGE;
				
				default:
					if (ViableMonCountFromBankLoadPartyRange(bankDef) <= 1
					||  defAbility == ABILITY_SUCTIONCUPS)
						viability -= 10;
			}
			break;
			
		case EFFECT_CONVERSION:
			//Check first move type
			if (IsOfType(bankAtk, gBattleMoves[gBattleMons[bankAtk].moves[0]].type))
				viability -= 10;
			break;
		
		case EFFECT_RESTORE_HP:
		case EFFECT_REST:
		case EFFECT_MORNING_SUN:
		AI_RECOVERY:
			switch (move) {
				case MOVE_PURIFY:
					if (!(defStatus1 & STATUS_ANY)
					|| MoveBlockedBySubstitute(move, bankAtk, bankDef)
					|| GetHealthPercentage(bankAtk) == 100)
					{
						viability -= 10;
						break;
					}
					else if (bankDef == bankAtkPartner) 
						break; //Always heal your ally
					else if (GetHealthPercentage(bankAtk) >= 90)
						viability -= 9; //No point in healing, but should at least do it if nothing better
					break;
					
				default:
					if (GetHealthPercentage(bankAtk) == 100)
						viability -= 10;
					else if (GetHealthPercentage(bankAtk) >= 90)
						viability -= 9; //No point in healing, but should at least do it if nothing better
			}
			break;
		
		case EFFECT_POISON:
		case EFFECT_TOXIC:
			if (move == MOVE_TOXICTHREAD
			&& STAT_STAGE(bankDef, STAT_STAGE_SPEED) > STAT_STAGE_MIN
			&& defAbility != ABILITY_CONTRARY)
				break;
		
		AI_POISON_CHECK: ;
			if (!CanBePoisoned(bankDef, bankAtk, TRUE)
			|| MoveBlockedBySubstitute(move, bankAtk, bankDef))
				viability -= 10;
			break;
		
		case EFFECT_LIGHT_SCREEN:
			if (gSideAffecting[SIDE(bankAtk)] & SIDE_STATUS_LIGHTSCREEN)
				viability -= 10;
			break;
		
		case EFFECT_0HKO:
			if (TypeCalc(move, bankAtk, bankDef, 0, FALSE) & (MOVE_RESULT_NO_EFFECT | MOVE_RESULT_MISSED)
			|| (NO_MOLD_BREAKERS(atkAbility, move) && defAbility == ABILITY_STURDY))
				viability -= 10;
			break;
		
		case EFFECT_RECOIL_IF_MISS:
			if (atkAbility == ABILITY_MAGICGUARD)
				goto AI_STANDARD_DAMAGE;
			else if (moveAcc < 75)
				viability -= 6;
			break;
				
		case EFFECT_MIST:
			if (gSideAffecting[SIDE(bankAtk)] & SIDE_STATUS_MIST)
				viability -= 10;
			break;

		case EFFECT_FOCUS_ENERGY:
			if (atkStatus2 & STATUS2_FOCUS_ENERGY)
				viability -= 10;
			break;
		
		case EFFECT_RECOIL:
			if (atkAbility == ABILITY_MAGICGUARD || atkAbility == ABILITY_ROCKHEAD)
				goto AI_STANDARD_DAMAGE;
				
			u32 dmg = AI_CalcDmg(bankAtk, bankDef, move);

			if (CheckTableForMove(move, Percent25RecoilMoves))
				dmg = MathMax(1, dmg / 4);
			else if (CheckTableForMove(move, Percent33RecoilMoves))
				dmg = MathMax(1, dmg / 3);
			else if (CheckTableForMove(move, Percent50RecoilMoves))
				dmg = MathMax(1, dmg / 2);
			else if (CheckTableForMove(move, Percent66RecoilMoves))
				dmg = MathMax(1, (dmg * 2) / 3);
			else if (CheckTableForMove(move, Percent75RecoilMoves))
				dmg = MathMax(1, (dmg * 3) / 4);
			else if (CheckTableForMove(move, Percent100RecoilMoves))
				dmg = MathMax(1, dmg);
			else if (move == MOVE_MINDBLOWN)
				dmg = MathMax(1, gBattleMons[bankAtk].maxHP / 2);
			
			if (dmg > gBattleMons[bankAtk].hp) //Recoil kills attacker
				viability -= 4;
			else
				goto AI_STANDARD_DAMAGE;
			break;
		
		case EFFECT_CONFUSE:
		AI_CONFUSE:
			switch (move) {
				case MOVE_TEETERDANCE: //Check if can affect either target
					if ((defStatus2 & STATUS2_CONFUSION
					  || (NO_MOLD_BREAKERS(atkAbility, move) && defAbility == ABILITY_OWNTEMPO)
					  || (CheckGrounding(bankDef) == GROUNDED && TerrainType == MISTY_TERRAIN)
					  || (MoveBlockedBySubstitute(move, bankAtk, bankDef)))
					&& ((gBattleMons[bankDefPartner].status2 & STATUS2_CONFUSION)
					  || (NO_MOLD_BREAKERS(atkAbility, move) && defPartnerAbility == ABILITY_OWNTEMPO) 
					  || (CheckGrounding(bankDefPartner) == GROUNDED && TerrainType == MISTY_TERRAIN)
					  || (MoveBlockedBySubstitute(move, bankAtk, bankDefPartner))))
					{
						viability -= 10;
					}
					break;
				default:
					if (defStatus2 & STATUS2_CONFUSION 
					|| (NO_MOLD_BREAKERS(atkAbility, move) && defAbility == ABILITY_OWNTEMPO)
					|| (CheckGrounding(bankDef) == GROUNDED && TerrainType == MISTY_TERRAIN)
					|| (MoveBlockedBySubstitute(move, bankAtk, bankDef)))
						viability -= 10;
			}
			break;

		case EFFECT_TRANSFORM:
			if (atkStatus2 & STATUS2_TRANSFORMED
			||  defStatus2 & (STATUS2_TRANSFORMED | STATUS2_SUBSTITUTE)) //Leave out Illusion b/c AI is supposed to be fooled
				viability -= 10;
			break;
			
		case EFFECT_REFLECT:
			switch (move) {
				case MOVE_AURORAVEIL:
					if (gNewBS->AuroraVeilTimers[SIDE(bankAtk)]
					|| !(gBattleWeather & WEATHER_HAIL_ANY))
						viability -= 10;
					break;
				
				default:
					if (gSideAffecting[SIDE(bankAtk)] & SIDE_STATUS_REFLECT)
						viability -= 10;
			}
			break;

		case EFFECT_PARALYZE:
		AI_PARALYZE_CHECK: ;
			if (!CanBeParalyzed(bankDef, TRUE)
			|| MoveBlockedBySubstitute(move, bankAtk, bankDef))
				viability -= 10;
			else if (move != MOVE_GLARE
				&& TypeCalc(move, bankAtk, bankDef, 0, FALSE) & (MOVE_RESULT_NO_EFFECT | MOVE_RESULT_MISSED))
			{
				viability -= 10;
			}
			break;
		
		case EFFECT_RAZOR_WIND:
		case EFFECT_SKULL_BASH:
		case EFFECT_SKY_ATTACK:
			if (atkEffect == ITEM_EFFECT_POWER_HERB)
				goto AI_STANDARD_DAMAGE;
				
			if (CanKnockOut(bankDef, bankAtk) //Attacker can be knocked out
			&&  predictedMove != MOVE_NONE)
				viability -= 4;
			goto AI_STANDARD_DAMAGE;
		
		//Add check for sound move?
		case EFFECT_SUBSTITUTE:
			if (atkStatus2 & STATUS2_SUBSTITUTE
			|| GetHealthPercentage(bankAtk) <= 25)
				viability -= 10;
			else if (ABILITY(FOE(bankAtk)) == ABILITY_INFILTRATOR 
			|| (gBattleTypeFlags & BATTLE_TYPE_DOUBLE && ABILITY(PARTNER(FOE(bankAtk))) == ABILITY_INFILTRATOR))
				viability -= 8;
			break;
		
		case EFFECT_RECHARGE:
			if (atkAbility != ABILITY_TRUANT
			&& MoveKnocksOutXHits(move, bankAtk, bankDef, 1)
			&& CanKnockOutWithoutMove(move, bankAtk, bankDef))
				viability -= 1;
			break;
		
		case EFFECT_SPITE:
		case EFFECT_MIMIC:
			if (MoveWouldHitFirst(move, bankAtk, bankDef))
			{ 	
				if (gLastUsedMoves[bankDef] == MOVE_NONE
				||  gLastUsedMoves[bankDef] == 0xFFFF)
					viability -= 10;
			}
			else if (predictedMove == MOVE_NONE)
				viability -= 10;
			goto AI_SUBSTITUTE_CHECK;
		
		case EFFECT_METRONOME:
			break;
		
		case EFFECT_LEECH_SEED:
			if (IsOfType(bankDef, TYPE_GRASS)
			|| (defStatus3 & STATUS3_LEECHSEED)
			|| (defAbility == ABILITY_LIQUIDOOZE))
				viability -= 10;
			break;
		
		case EFFECT_DISABLE:
			if (gDisableStructs[bankDef].disableTimer1 == 0
			&&  defEffect != ITEM_EFFECT_CURE_ATTRACT)
			{
				if (MoveWouldHitFirst(move, bankAtk, bankDef))
				{ 	
					if (gLastUsedMoves[bankDef] == MOVE_NONE
					|| gLastUsedMoves[bankDef] == 0xFFFF)
						viability -= 10;
				}
				else if (predictedMove == MOVE_NONE)
					viability -= 10;
			}
			else
				viability -= 10;
			break;
			
		//case COUNTER:
			//Should now be handled in damage calc

		case EFFECT_ENCORE:
			if (gDisableStructs[bankDef].encoreTimer == 0
			&&  defEffect != ITEM_EFFECT_CURE_ATTRACT)
			{
				if (MoveWouldHitFirst(move, bankAtk, bankDef))
				{ 	
					if (gLastUsedMoves[bankDef] == MOVE_NONE
					|| gLastUsedMoves[bankDef] == 0xFFFF)
						viability -= 10;
				}
				else if (predictedMove == MOVE_NONE)
					viability -= 10;
			}
			else
				viability -= 10;
			break;
		
		case EFFECT_ENDEAVOR:
		case EFFECT_PAIN_SPLIT:
			if (gBattleMons[bankAtk].hp > (gBattleMons[bankAtk].hp + gBattleMons[bankDef].hp) / 2)
				viability -= 10;
			break;
		
		case EFFECT_SNORE:
		case EFFECT_SLEEP_TALK:
			if (((atkStatus1 & STATUS_SLEEP) == 1 || !(atkStatus1 & STATUS_SLEEP))
			&& atkAbility != ABILITY_COMATOSE)
				viability -= 10;
			break;
		
		case EFFECT_CONVERSION_2:
			if (gNewBS->LastUsedTypes[bankDef] == 0)
				viability -= 10;
			break;
			
		case EFFECT_LOCK_ON:
			switch (move) {
				case MOVE_LASERFOCUS:
					if (gNewBS->LaserFocusTimers[bankAtk] != 0)
						viability -= 10;
					else if (defAbility == ABILITY_SHELLARMOR || defAbility == ABILITY_BATTLEARMOR)
						viability -= 8;
					break;
				
				default: //Lock on
					if (atkStatus3 & STATUS3_LOCKON
					|| atkAbility == ABILITY_NOGUARD
					|| defAbility == ABILITY_NOGUARD)
						viability -= 10;
					break;
			}
			break;
			
		case EFFECT_SKETCH:
			if (gLastUsedMoves[bankDef] == MOVE_NONE)
				viability -= 10;
			break;
		
		case EFFECT_DESTINY_BOND:
			if (gNewBS->DestinyBondCounters[bankAtk] != 0
			|| atkStatus2 & STATUS2_DESTINY_BOND)
				viability -= 10;
			break;
		
		case EFFECT_FALSE_SWIPE:
			if (MoveKnocksOutXHits(move, bankAtk, bankDef, 1)
			&&  CanKnockOutWithoutMove(move, bankAtk, bankDef))
				viability -= 10;
			break;
		
		case EFFECT_HEAL_BELL:
			if (move == MOVE_HEALBELL)
			{
				if (!PartyMemberStatused(bankAtk, TRUE)) //Check Soundproof
					viability -= 10;
			}
			else if (!PartyMemberStatused(bankAtk, FALSE))
				viability -= 10;
			break;

		case EFFECT_MEAN_LOOK:
			switch (move) {
				case MOVE_SPIRITSHACKLE:
				case MOVE_ANCHORSHOT:
					goto AI_STANDARD_DAMAGE;
					
				default: //Mean look
					if (IsTrapped(bankDef, TRUE))
						viability -= 10;
					break;
			}
			break;
		
		case EFFECT_NIGHTMARE:
			if (defStatus2 & STATUS2_NIGHTMARE
			|| !(defStatus1 & STATUS_SLEEP || defAbility == ABILITY_COMATOSE))
				viability -= 10;
			break;
		
		case EFFECT_CURSE:
			if (IsOfType(bankAtk, TYPE_GHOST))
			{
				if (defStatus2 & STATUS2_CURSED)
					viability -= 10;
				else if (GetHealthPercentage(bankAtk) <= 50)
					viability -= 6;
			}
			else //Regular Curse
			{
				if (!STAT_CAN_RISE(bankAtk, STAT_STAGE_ATK)
				&& !STAT_CAN_RISE(bankAtk, STAT_STAGE_DEF)
				&& !STAT_CAN_FALL(bankAtk, STAT_STAGE_SPEED))
					viability -= 10;
			}
			break;
			
		case EFFECT_PROTECT:
			decreased = FALSE;
			switch (move) {
				case MOVE_QUICKGUARD:
				case MOVE_WIDEGUARD:
				case MOVE_CRAFTYSHIELD:
					if (!(gBattleTypeFlags & BATTLE_TYPE_DOUBLE))
					{
						viability -= 10;
						decreased = TRUE;
					}
					break;
				
				case MOVE_MATBLOCK:
					if (!gDisableStructs[bankAtk].isFirstTurn)
					{
						viability -= 10;
						decreased = TRUE;
					}
					break;
					
				case MOVE_ENDURE:
					if (gBattleMons[bankAtk].hp == 1)
					{
						viability -= 10;
						decreased = TRUE;
					}
					break;
			}
			if (decreased)
				break;
			
			if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
			{
				if (gBattleMons[FOE(bankAtk)].status2 & STATUS2_RECHARGE
				&&  gBattleMons[PARTNER(FOE(bankAtk))].status2 & STATUS2_RECHARGE)
				{
					viability -= 10;
					break;
				}
			}
			else //Single Battle
			{
				if (gBattleMons[FOE(bankAtk)].status2 & STATUS2_RECHARGE)
				{
					viability -= 10;
					break;
				}
			}
			
			if (gDisableStructs[bankAtk].protectUses > 0 && Random() % 100 < 50)
				viability -= 6;
			break;

		case EFFECT_SPIKES:
			if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
			{
				if (ViableMonCountFromBank(bankDef) <= 2)
				{
					viability -= 10;
					break;
				}
			}
			else
			{
				if (ViableMonCountFromBank(bankDef) <= 1)
				{
					viability -= 10;
					break;
				}
			}

			switch (move) {
				case MOVE_STEALTHROCK:
					if (gSideTimers[SIDE(bankDef)].srAmount > 0)
						viability -= 10;
					break;
				
				case MOVE_TOXICSPIKES:
					if (gSideTimers[SIDE(bankDef)].tspikesAmount >= 2)
						viability -= 10;
					break;
					
				case MOVE_STICKYWEB:
					if (gSideTimers[SIDE(bankDef)].stickyWeb)
						viability -= 10;
					break;
				
				default: //Spikes
					if (gSideTimers[SIDE(bankDef)].spikesAmount >= 3)
						viability -= 10;
					break;
			}
			break;
			
		case EFFECT_FORESIGHT:
			switch (move) {
				case MOVE_MIRACLEEYE:
					if (defStatus3 & STATUS3_MIRACLE_EYED)
						viability -= 10;
					
					if (STAT_STAGE(bankDef, STAT_STAGE_EVASION) <= 4
					|| !(IsOfType(bankDef, TYPE_DARK)))
						viability -= 9;
					break;
				
				default: //Foresight
					if (defStatus2 & STATUS2_FORESIGHT)
						viability -= 10;
					else if (STAT_STAGE(bankDef, STAT_STAGE_EVASION) <= 4
						|| !(IsOfType(bankDef, TYPE_GHOST)))
					{
						viability -= 9;
					}
					break;
			}
			break;
			
		case EFFECT_PERISH_SONG:
			if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
			{
				if (ViableMonCountFromBank(bankAtk) <= 2
				&&  atkAbility != ABILITY_SOUNDPROOF
				&&  atkPartnerAbility != ABILITY_SOUNDPROOF
				&&  ViableMonCountFromBank(FOE(bankAtk)) >= 3)
					viability -= 10; //Don't wipe your team if you're going to lose
					
				if ((!BATTLER_ALIVE(FOE(bankAtk)) || ABILITY(FOE(bankAtk)) == ABILITY_SOUNDPROOF || gStatuses3[FOE(bankAtk)] & STATUS3_PERISH_SONG)
				&&  (!BATTLER_ALIVE(PARTNER(FOE(bankAtk))) || ABILITY(PARTNER(FOE(bankAtk))) == ABILITY_SOUNDPROOF || gStatuses3[PARTNER(FOE(bankAtk))] & STATUS3_PERISH_SONG))
					viability -= 10; //Both enemies are perish songed
			}
			else
			{
				if (ViableMonCountFromBank(bankAtk) == 1
				&&  atkAbility != ABILITY_SOUNDPROOF
				&&  ViableMonCountFromBank(bankDef) >= 2)
					viability -= 10;
					
				if (gStatuses3[FOE(bankAtk)] & STATUS3_PERISH_SONG || ABILITY(FOE(bankAtk)) == ABILITY_SOUNDPROOF)
					viability -= 10;
			}
			break;
			
		case EFFECT_SANDSTORM:
			if (gBattleWeather & (WEATHER_SANDSTORM_ANY | WEATHER_PRIMAL_ANY))
				viability -= 10;
			break;
			
		case EFFECT_SWAGGER:
			if (bankDef == bankAtkPartner)
			{
				if (STAT_STAGE(bankDef, STAT_STAGE_ATK) > 10 || defAbility == ABILITY_CONTRARY)
					viability -= 10;
			}
			else
				goto AI_CONFUSE;
			break;
		
		case EFFECT_ATTRACT:
			if (defAbility == ABILITY_OBLIVIOUS || (defStatus2 & STATUS2_INFATUATION)
			||  defGender == atkGender || atkGender == MON_GENDERLESS || defGender == MON_GENDERLESS)
				viability -= 10;
			break;

		case EFFECT_SAFEGUARD:
			if (gSideAffecting[SIDE(bankDef)] & SIDE_STATUS_SAFEGUARD)
				viability -= 10;
			break;
		
		case EFFECT_BURN_UP:
			if (!IsOfType(bankAtk, TYPE_FIRE))
				viability -= 10;
			break;
		
		case EFFECT_BATON_PASS:
			if (move == MOVE_UTURN || move == MOVE_VOLTSWITCH)
				goto AI_STANDARD_DAMAGE;

			else if (ViableMonCountFromBank(bankAtk) <= 1)
			{
				viability -= 10;
				break;
			}
			else //Baton pass
			{
				//Check Substitute, Aqua Ring, Magnet Rise, Ingrain, and stats
				if (atkStatus2 & STATUS2_SUBSTITUTE
				|| (atkStatus3 & (STATUS3_ROOTED | STATUS3_AQUA_RING | STATUS3_LEVITATING))
				|| AnyStatIsRaised(bankAtk))
					break;
					
				viability -= 6;
				break;
			}
			break;
			
		case EFFECT_RAPID_SPIN:
			if (move == MOVE_DEFOG)
			{
				if (gSideAffecting[SIDE(bankDef)] & (SIDE_STATUS_REFLECT | SIDE_STATUS_SAFEGUARD | SIDE_STATUS_MIST)
				|| gNewBS->AuroraVeilTimers[SIDE(bankDef)] != 0
				|| gSideAffecting[SIDE(bankAtk)] & SIDE_STATUS_SPIKES) 
					break;
				
				goto AI_LOWER_EVASION;
			}
			else if ((atkStatus2 & STATUS2_WRAPPED) || (atkStatus3 & STATUS3_LEECHSEED))
				goto AI_STANDARD_DAMAGE;
			
			//Spin checks
			if (!(gSideAffecting[SIDE(bankAtk)] & SIDE_STATUS_SPIKES))
				viability -= 6;
			break;
		
		case EFFECT_RAIN_DANCE:
			if (gBattleWeather & (WEATHER_RAIN_ANY | WEATHER_PRIMAL_ANY))
				viability -= 10;
			break;
		
		case EFFECT_SUNNY_DAY:
			if (gBattleWeather & (WEATHER_SUN_ANY | WEATHER_PRIMAL_ANY))
				viability -= 10;
			break;
		
		case EFFECT_BELLY_DRUM:
			if (atkAbility == ABILITY_CONTRARY)
				viability -= 10;
			else if (GetHealthPercentage(bankAtk) <= 50)
				viability -= 10;
			break;
			
		case EFFECT_PSYCH_UP:
			if (move == MOVE_SPECTRALTHIEF)
				goto AI_STANDARD_DAMAGE;
			else
				goto AI_HAZE_CHECK;
			break;
			
		case EFFECT_FUTURE_SIGHT:
			if (gWishFutureKnock->futureSightCounter[bankAtk] != 0)
				viability -= 10;
			else
				goto AI_STANDARD_DAMAGE;
			break;
			
		case EFFECT_SOLARBEAM:
			if (atkEffect == ITEM_EFFECT_POWER_HERB
			|| (WEATHER_HAS_EFFECT && gBattleWeather & WEATHER_SUN_ANY))
				goto AI_STANDARD_DAMAGE;
				
			if (CanKnockOut(bankDef, bankAtk)) //Attacker can be knocked out
				viability -= 4;
				
			goto AI_STANDARD_DAMAGE;
			
		case EFFECT_SEMI_INVULNERABLE: ;
			if (predictedMove != MOVE_NONE
			&& MoveWouldHitFirst(move, bankAtk, bankDef)
			&& gBattleMoves[predictedMove].effect == EFFECT_SEMI_INVULNERABLE)
				viability -= 10; //Don't Fly if opponent is going to fly after you
				
			if (WillFaintFromWeatherSoon(bankAtk)
			&& (move == MOVE_FLY || move == MOVE_BOUNCE))
				viability -= 10; //Attacker will faint while in the air
				
			goto AI_STANDARD_DAMAGE;
			
		case EFFECT_FAKE_OUT:
			if (!gDisableStructs[bankAtk].isFirstTurn)
				viability -= 10;
			else
				goto AI_STANDARD_DAMAGE;
			break;
			
		case EFFECT_STOCKPILE:
			if (gDisableStructs[bankAtk].stockpileCounter >= 3)
				viability -= 10;
			break;
			
		case EFFECT_SPIT_UP:
			if (gDisableStructs[bankAtk].stockpileCounter == 0)
				viability -= 10;
			else
				goto AI_STANDARD_DAMAGE;
			break;
		
		case EFFECT_SWALLOW:
			if (gDisableStructs[bankAtk].stockpileCounter == 0)
				viability -= 10;
			else
				goto AI_RECOVERY;
			break;
		
		case EFFECT_HAIL:
			if (gBattleWeather & (WEATHER_HAIL_ANY | WEATHER_PRIMAL_ANY))
				viability -= 10;
			break;

		case EFFECT_TORMENT:
			if (defStatus2 & STATUS2_TORMENT)
			{
				viability -= 10;
				break;
			}
			if (defEffect == ITEM_EFFECT_CURE_ATTRACT)
				viability -= 6;
			goto AI_SUBSTITUTE_CHECK;
		
		case EFFECT_FLATTER:
			if (bankDef == bankAtkPartner)
			{
				if (STAT_STAGE(bankDef, STAT_STAGE_SPATK) > 10 || defAbility == ABILITY_CONTRARY)
					viability -= 10;
			}
			else
				goto AI_CONFUSE;
			break;
		
		case EFFECT_WILL_O_WISP:
		AI_BURN_CHECK: ;
			if (!CanBeBurned(bankDef, TRUE)
			|| MoveBlockedBySubstitute(move, bankAtk, bankDef))
				viability -= 10;
			break;
		
		case EFFECT_MEMENTO:
			if (ViableMonCountFromBank(bankAtk) <= 1)
			{
				viability -= 10;
				break;
			}
			switch (move) {
				case MOVE_HEALINGWISH:
				case MOVE_LUNARDANCE:
					if (TeamFullyHealedMinusBank(bankAtk))
						viability -= 10;
					break;
				
				case MOVE_FINALGAMBIT:
					//Just the viablemonfromcount check, but not stat check
					break;
				
				default: //Memento
					if (MoveBlockedBySubstitute(move, bankAtk, bankDef))
						viability -= 10;
					else if (STAT_STAGE(bankDef, STAT_STAGE_ATK) == STAT_STAGE_MIN 
						  && STAT_STAGE(bankDef, STAT_STAGE_SPATK) == STAT_STAGE_MIN)
					{
						viability -= 10;
					}
					break;
			}
			break;
		
		case EFFECT_FOCUS_PUNCH: ;
			switch (move) {
				case MOVE_SHELLTRAP: ;
					u16 partnerMove = IsValidMovePrediction(bankDefPartner, bankAtk);

					if (!CheckContact(predictedMove, foe1)
					&&  !CheckContact(partnerMove, foe2))
						viability -= 4; //Probably better not to use it
					break;
					
				case MOVE_BEAKBLAST:
					break;
			
				default:
					if (predictedMove != MOVE_NONE
					&& !MoveBlockedBySubstitute(predictedMove, bankDef, bankAtk)
					&& SPLIT(predictedMove) != SPLIT_STATUS
					&& gBattleMoves[predictedMove].power != 0)
						viability -= 4; //Probably better not to use it
			}
			break;

		case EFFECT_NATURE_POWER:
			return AI_Script_Negatives(bankAtk, bankDef, GetNaturePowerMove(), originalViability);
	
		case EFFECT_CHARGE:
			if (atkStatus3 & STATUS3_CHARGED_UP)
			{
				viability -= 10;
				break;
			}

			if (!MoveTypeInMoveset(bankAtk, TYPE_ELECTRIC))
				goto AI_SPDEF_RAISE_1;
			break;
			
		case EFFECT_TAUNT:
			if (gDisableStructs[bankDef].tauntTimer != 0)
				viability -= 1;
			break;
		
		case EFFECT_FOLLOW_ME:
		case EFFECT_HELPING_HAND:
			if (!(gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
			||  gBattleMons[bankAtkPartner].hp == 0)
				viability -= 10;
			break;

		case EFFECT_TRICK:
			switch (move) {
				case MOVE_BESTOW:
					if (!CanTransferItem(bankAtk, atkItem, GetBankPartyData(bankAtk)))
						viability -= 10;
					break;
					
				default: //Trick
					if ((!CanTransferItem(bankAtk, atkItem, GetBankPartyData(bankAtk))
					  && !CanTransferItem(bankAtk, defItem, GetBankPartyData(bankAtk))
					  && !CanTransferItem(bankDef, atkItem, GetBankPartyData(bankDef))
					  && !CanTransferItem(bankDef, defItem, GetBankPartyData(bankDef)))
					|| (defAbility == ABILITY_STICKYHOLD))
						viability -= 10;
					break;
			}
			break;
			
		case EFFECT_ROLE_PLAY:
			atkAbility = *GetAbilityLocation(bankAtk);
			defAbility = *GetAbilityLocation(bankDef);
		
			if (atkAbility == defAbility
			||  defAbility == ABILITY_NONE
			||  CheckTableForAbility(atkAbility, RolePlayAttackerBanTable)
			||  CheckTableForAbility(defAbility, RolePlayBanTable))
				viability -= 10;
			break;
		
		case EFFECT_WISH:
			if (gWishFutureKnock->wishCounter[bankAtk] != 0)
				viability -= 10;
			break;
		
		case EFFECT_ASSIST: ;
			u8 i, firstMonId, lastMonId;
			struct Pokemon* party = LoadPartyRange(bankAtk, &firstMonId, &lastMonId);
			
			for (i = firstMonId; i < lastMonId; ++i)
			{
				if (party[i].species != SPECIES_NONE
				&& !GetMonData(&party[i], MON_DATA_IS_EGG, NULL)
				&& gBattlerPartyIndexes[i] != gBattlerPartyIndexes[bankAtk])
					break; //At least one other Pokemon on team
			}

			if (i == lastMonId)
				viability -= 10;
			break;
		
		case EFFECT_INGRAIN:
			switch (move) {
				case MOVE_AQUARING:
					if (atkStatus3 & STATUS3_AQUA_RING)
						viability -= 10;
					break;

				default:
					if (atkStatus3 & STATUS3_ROOTED)
						viability -= 10;
			}
			break;

		case EFFECT_SUPERPOWER:
			if (move == MOVE_HYPERSPACEFURY && atkSpecies != SPECIES_HOOPA_UNBOUND)
				viability -= 10;
			break;

		case EFFECT_MAGIC_COAT:
			if (!MagicCoatableMovesInMoveset(bankDef))
				viability -= 10;
			break;
			
		case EFFECT_RECYCLE:
			if (move == MOVE_BELCH)
			{
				if (!(gNewBS->BelchCounters & gBitTable[gBattlerPartyIndexes[bankAtk]]))
					viability -= 10;
				else
					goto AI_STANDARD_DAMAGE;
			}
			else if (gNewBS->SavedConsumedItems[bankAtk] == ITEM_NONE || atkItem != ITEM_NONE)
				viability -= 10;
			break;
		
		case EFFECT_YAWN:
			if (defStatus3 & STATUS3_YAWN)
				viability -= 10;
			else
				goto AI_CHECK_SLEEP;
			break;
		
		case EFFECT_SKILL_SWAP:
			atkAbility = *GetAbilityLocation(bankAtk); //Get actual abilities
			defAbility = *GetAbilityLocation(bankDef);

			switch (move) {
				case MOVE_WORRYSEED:
					if (defAbility == ABILITY_INSOMNIA
					|| CheckTableForAbility(defAbility, WorrySeedGastroAcidBanTable)
					|| MoveBlockedBySubstitute(move, bankAtk, bankDef))
						viability -= 10;
					break;

				case MOVE_GASTROACID:
					if (defStatus3 & STATUS3_ABILITY_SUPPRESS
					||  CheckTableForAbility(defAbility, WorrySeedGastroAcidBanTable)
					||  MoveBlockedBySubstitute(move, bankAtk, bankDef))
						viability -= 10;
					break;

				case MOVE_ENTRAINMENT:
					if (atkAbility == ABILITY_NONE
					||  CheckTableForAbility(atkAbility, EntrainmentBanTableAttacker)
					||  CheckTableForAbility(defAbility, EntrainmentBanTableTarget)
					||  MoveBlockedBySubstitute(move, bankAtk, bankDef))
						viability -= 10;
					else
						goto AI_SUBSTITUTE_CHECK;
					break;

				case MOVE_COREENFORCER:
					goto AI_STANDARD_DAMAGE;

				case MOVE_SIMPLEBEAM:
					if (defAbility == ABILITY_SIMPLE
					||  CheckTableForAbility(defAbility, SimpleBeamBanTable)
					||  MoveBlockedBySubstitute(move, bankAtk, bankDef))
						viability -= 10;
					break;

				default: //Skill Swap
					if (atkAbility == ABILITY_NONE || defAbility == ABILITY_NONE
					|| CheckTableForAbility(atkAbility, SkillSwapBanTable)
					|| CheckTableForAbility(defAbility, SkillSwapBanTable))
						viability -= 10;
			}
			break;
			
		case EFFECT_IMPRISON:
			if (atkStatus3 & STATUS3_IMPRISONED)
				viability -= 10;
			break;
			
		case EFFECT_REFRESH:
			if (!(atkStatus1 & (STATUS1_PSN_ANY | STATUS1_BURN | STATUS1_PARALYSIS)))
			{
				viability -= 10;
				break;
			}
			else if (move == MOVE_PSYCHOSHIFT)
			{
				if (atkStatus1 & STATUS1_PSN_ANY)
					goto AI_POISON_CHECK;
				else if (atkStatus1 & STATUS1_BURN)
					goto AI_BURN_CHECK;
				else if (atkStatus1 & STATUS1_PARALYSIS)
					goto AI_PARALYZE_CHECK;
				else if (atkStatus1 & STATUS1_SLEEP)
					goto AI_CHECK_SLEEP;
				else
					viability -= 10;
			}
			break;
			
		case EFFECT_SNATCH:
			//Check target for any snatchable moves
			if (!HasSnatchableMove(bankDef))
				viability -= 10;
			break;
			
		case EFFECT_MUD_SPORT:
			if (gNewBS->MudSportTimer != 0)
				viability -= 10;
			break;
			
		case EFFECT_TICKLE:
			if (defAbility == ABILITY_CONTRARY)
			{
				viability -= 10;
				break;
			}
			else
			{
				if ((STAT_STAGE(bankDef, STAT_STAGE_ATK) == STAT_STAGE_MIN || !PhysicalMoveInMoveset(bankDef))
				&&  STAT_STAGE(bankDef, STAT_STAGE_DEF) == STAT_STAGE_MIN)
				{
					viability -= 10;
					break;
				}
			}
			goto AI_SUBSTITUTE_CHECK;
			
		case EFFECT_COSMIC_POWER:
			if (atkAbility == ABILITY_CONTRARY)
				viability -= 10;
			else
			{
				AI_COSMIC_POWER:
				if (STAT_STAGE(bankAtk, STAT_STAGE_DEF) >= STAT_STAGE_MAX
				&& STAT_STAGE(bankAtk, STAT_STAGE_SPDEF) >= STAT_STAGE_MAX)
					viability -= 10;
			}
			break;
			
		case EFFECT_BULK_UP:
			if (atkAbility == ABILITY_CONTRARY)
				viability -= 10;
			else
			{
				switch (move) {
					case MOVE_COIL:
						if (STAT_STAGE(bankAtk, STAT_STAGE_ACC) >= STAT_STAGE_MAX
						&& (STAT_STAGE(bankAtk, STAT_STAGE_ATK) >= STAT_STAGE_MAX && !PhysicalMoveInMoveset(bankAtk))
						&&  STAT_STAGE(bankAtk, STAT_STAGE_DEF) >= STAT_STAGE_MAX)
							viability -= 10;
						break;
						
					default:
						if ((STAT_STAGE(bankAtk, STAT_STAGE_ATK) >= STAT_STAGE_MAX && !PhysicalMoveInMoveset(bankAtk))
						&&  STAT_STAGE(bankAtk, STAT_STAGE_DEF) >= STAT_STAGE_MAX)
							viability -= 10;
				}
			}
			break;
			
		case EFFECT_WATER_SPORT:
			if (gNewBS->WaterSportTimer != 0)
				viability -= 10;
			break;
			
		case EFFECT_CALM_MIND:
			if (atkAbility == ABILITY_CONTRARY)
					viability -= 10;
			else
			{
				switch (move) {
					case MOVE_QUIVERDANCE:
					case MOVE_GEOMANCY:
						if (STAT_STAGE(bankAtk, STAT_STAGE_SPEED) >= STAT_STAGE_MAX
						&& (STAT_STAGE(bankAtk, STAT_STAGE_SPATK) >= STAT_STAGE_MAX || !SpecialMoveInMoveset(bankAtk))
						&&  STAT_STAGE(bankAtk, STAT_STAGE_SPDEF) >= STAT_STAGE_MAX)
							viability -= 10;
						break;

					default:
						if ((STAT_STAGE(bankAtk, STAT_STAGE_SPATK) >= STAT_STAGE_MAX || !SpecialMoveInMoveset(bankAtk))
						&&  STAT_STAGE(bankAtk, STAT_STAGE_SPDEF) >= STAT_STAGE_MAX)
							viability -= 10;
				}
			}
			break;
			
		case EFFECT_DRAGON_DANCE:
			switch (move) {
				case MOVE_SHELLSMASH:
					if (atkAbility == ABILITY_CONTRARY)
						goto AI_COSMIC_POWER;
					
					if ((STAT_STAGE(bankAtk, STAT_STAGE_SPATK) >= STAT_STAGE_MAX || !SpecialMoveInMoveset(bankAtk))
					&&  (STAT_STAGE(bankAtk, STAT_STAGE_ATK) >= STAT_STAGE_MAX || !PhysicalMoveInMoveset(bankAtk))
					&&  (STAT_STAGE(bankAtk, STAT_STAGE_SPEED) >= STAT_STAGE_MAX))
						viability -= 10;
					break;
					
				default: //Dragon Dance + Shift Gear
					if (atkAbility == ABILITY_CONTRARY)
						viability -= 10;
					else
					{
						if ((STAT_STAGE(bankAtk, STAT_STAGE_ATK) >= STAT_STAGE_MAX || !PhysicalMoveInMoveset(bankAtk))
						&&  (STAT_STAGE(bankAtk, STAT_STAGE_SPEED) >= STAT_STAGE_MAX))
							viability -= 10;
					}
			}
			break;
		
		case EFFECT_STAT_SWAP_SPLIT:
			if (bankDef == bankAtkPartner)
				break;

			switch (move) {
				case MOVE_POWERTRICK:
					if (gBattleMons[bankAtk].defense >= gBattleMons[bankAtk].attack && !PhysicalMoveInMoveset(bankAtk))
						viability -= 10;
					break;
				
				case MOVE_POWERSWAP: //Don't use if attacker's stat stages are higher than opponents
					if (STAT_STAGE(bankAtk, STAT_STAGE_ATK) >= STAT_STAGE(bankDef, STAT_STAGE_SPATK)
					&&  STAT_STAGE(bankAtk, STAT_STAGE_SPATK) >= STAT_STAGE(bankDef, STAT_STAGE_SPATK))
						viability -= 10;
					else
						goto AI_SUBSTITUTE_CHECK;
					break;
				
				case MOVE_GUARDSWAP: //Don't use if attacker's stat stages are higher than opponents
					if (STAT_STAGE(bankAtk, STAT_STAGE_DEF) >= STAT_STAGE(bankDef, STAT_STAGE_SPDEF)
					&&  STAT_STAGE(bankAtk, STAT_STAGE_SPDEF) >= STAT_STAGE(bankDef, STAT_STAGE_SPDEF))
						viability -= 10;
					else
						goto AI_SUBSTITUTE_CHECK;
					break;
					
				case MOVE_SPEEDSWAP:
					if (gNewBS->TrickRoomTimer)
					{
						if (gBattleMons[bankAtk].speed <= gBattleMons[bankDef].speed)
							viability -= 10;
					}
					else if (gBattleMons[bankAtk].speed >= gBattleMons[bankDef].speed)
						viability -= 10;
					else
						goto AI_SUBSTITUTE_CHECK;
					break;
				
				case MOVE_HEARTSWAP: ;
					u8 attackerPositiveStages = CountBanksPositiveStatStages(bankAtk);
					u8 attackerNegativeStages = CountBanksNegativeStatStages(bankAtk);
					u8 targetPositiveStages = CountBanksPositiveStatStages(bankDef);
					u8 targetNegativeStages = CountBanksNegativeStatStages(bankDef);
					
					if (attackerPositiveStages >= targetPositiveStages
					&&  attackerNegativeStages <= targetNegativeStages)
						viability -= 10;
					else
						goto AI_SUBSTITUTE_CHECK;
					break;
					
				case MOVE_POWERSPLIT: ;
					u8 atkAttack = gBattleMons[bankAtk].attack;
					u8 defAttack = gBattleMons[bankDef].attack;
					u8 atkSpAttack = gBattleMons[bankAtk].spAttack;
					u8 defSpAttack = gBattleMons[bankDef].spAttack;
					
					if (atkAttack + atkSpAttack >= defAttack + defSpAttack) //Combined attacker stats are > than combined target stats
						viability -= 10;
					else
						goto AI_SUBSTITUTE_CHECK;
					break;

				case MOVE_GUARDSPLIT: ;
					u8 atkDefense = gBattleMons[bankAtk].defense;
					u8 defDefense = gBattleMons[bankDef].defense;
					u8 atkSpDefense = gBattleMons[bankAtk].spDefense;
					u8 defSpDefense = gBattleMons[bankDef].spDefense;
					
					if (atkDefense + atkSpDefense >= defDefense + defSpDefense) //Combined attacker stats are > than combined target stats
						viability -= 10;
					else
						goto AI_SUBSTITUTE_CHECK;
					break;
			}
			break;

		case EFFECT_ME_FIRST: ;
			if (move == MOVE_MEFIRST && predictedMove != MOVE_NONE)
			{
				if (!MoveWouldHitFirst(move, bankAtk, bankDef))
					viability -= 10;
				else
					return AI_Script_Negatives(bankAtk, bankDef, predictedMove, originalViability);
			}
			else //Target is predicted to switch most likely
				viability -= 10;
			break;

		case EFFECT_NATURAL_GIFT:
			if (atkAbility == ABILITY_KLUTZ
			|| gNewBS->MagicRoomTimer != 0
			|| GetPocketByItemId(atkItem) != POCKET_BERRIES)
				viability -= 10;
			break;

		case EFFECT_SET_TERRAIN:
			switch (move) {
				case MOVE_ELECTRICTERRAIN:
					if (TerrainType == ELECTRIC_TERRAIN)
						viability -= 10;
					break;
				case MOVE_GRASSYTERRAIN:
					if (TerrainType == GRASSY_TERRAIN)
						viability -= 10;
					break;
				case MOVE_MISTYTERRAIN:
					if (TerrainType == MISTY_TERRAIN)
						viability -= 10;	
					break;
				case MOVE_PSYCHICTERRAIN:
					if (TerrainType == PSYCHIC_TERRAIN)
						viability -= 10;
					break;
			}
			break;

		case EFFECT_PLEDGE:
			if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE
			&&  gBattleMons[bankAtkPartner].hp > 0)
			{
				u16 partnerMove = gChosenMovesByBanks[bankAtkPartner];
				if (partnerMove != MOVE_NONE
				&&  gBattleMoves[partnerMove].effect == EFFECT_PLEDGE
				&&  move != partnerMove) //Different pledge moves
				{
					if (gBattleMons[bankAtkPartner].status1 & (STATUS1_SLEEP | STATUS1_FREEZE)
					&&  gBattleMons[bankAtkPartner].status1 != 1) //Will wake up this turn
						viability -= 10; //Don't use combo move if your partner will cause failure
				}
			}
			break;

		case EFFECT_FIELD_EFFECTS:
			switch (move) {
				case MOVE_TRICKROOM:
					break;

				case MOVE_MAGICROOM:
					if (gNewBS->MagicRoomTimer != 0)
						viability -= 10;
					break;
		
				case MOVE_WONDERROOM:
					if (gNewBS->WonderRoomTimer != 0)
						viability -= 10;
					break;
	
				case MOVE_GRAVITY:
					if (gNewBS->GravityTimer != 0
					&&  !IsOfType(bankAtk, TYPE_FLYING)
					&&  atkEffect != ITEM_EFFECT_AIR_BALLOON) //Should revert Gravity in this case
						viability -= 10;
					break;

				case MOVE_IONDELUGE:
					if (gNewBS->IonDelugeTimer != 0)
						viability -= 10;
					break;

				case MOVE_PLASMAFISTS:
					goto AI_STANDARD_DAMAGE;
			}
			break;
		
		case EFFECT_FLING:
			if (atkItem == ITEM_NONE
			|| !CanTransferItem(atkSpecies, atkItem, GetBankPartyData(bankAtk)))
				viability -= 10;
			goto AI_STANDARD_DAMAGE;

		case EFFECT_ATTACK_BLOCKERS:
			switch (move) {
				case MOVE_EMBARGO:
					if (defAbility == ABILITY_KLUTZ
					|| gNewBS->MagicRoomTimer != 0
					|| gNewBS->EmbargoTimers[bankDef] != 0)
						viability -= 10;
					else
						goto AI_SUBSTITUTE_CHECK;
					break;

				case MOVE_POWDER:
					if (!MoveTypeInMoveset(bankDef, TYPE_FIRE))
						viability -= 10;
					else
						goto AI_SUBSTITUTE_CHECK;
					break;

				case MOVE_TELEKINESIS:
					if (defStatus3 & (STATUS3_TELEKINESIS | STATUS3_ROOTED | STATUS3_SMACKED_DOWN)
					||  gNewBS->GravityTimer != 0
					||  defEffect == ITEM_EFFECT_IRON_BALL
					||  CheckTableForSpecies(defSpecies, gTelekinesisBanList))
						viability -= 10;
					else
						goto AI_SUBSTITUTE_CHECK;
					break;
					
				case MOVE_THROATCHOP:
					goto AI_STANDARD_DAMAGE;
					
				default: //Heal Block
					if (gNewBS->HealBlockTimers[bankDef] != 0)
						viability -= 10;
					else
						goto AI_SUBSTITUTE_CHECK;
					break;
			}
			break;
			
		case EFFECT_TYPE_CHANGES:
			switch (move) {
				case MOVE_SOAK:
					if (gBattleMons[bankDef].type1 == TYPE_WATER
					&&  gBattleMons[bankDef].type2 == TYPE_WATER
					&&  gBattleMons[bankDef].type3 == TYPE_BLANK)
						viability -= 10;
					else
						goto AI_SUBSTITUTE_CHECK;
					break;

				case MOVE_TRICKORTREAT:
					if (gBattleMons[bankDef].type1 == TYPE_GHOST
					||  gBattleMons[bankDef].type2 == TYPE_GHOST
					||  gBattleMons[bankDef].type3 == TYPE_GHOST)
						viability -= 10;
					else
						goto AI_SUBSTITUTE_CHECK;
					break;

				case MOVE_FORESTSCURSE:
					if (gBattleMons[bankDef].type1 == TYPE_GRASS
					||  gBattleMons[bankDef].type2 == TYPE_GRASS
					||  gBattleMons[bankDef].type3 == TYPE_GRASS)
						viability -= 10;
					else
						goto AI_SUBSTITUTE_CHECK;
					break;
			}
			break;
			
		case EFFECT_HEAL_TARGET:
			switch (move) {
				case MOVE_POLLENPUFF:
					if (bankDef != bankAtkPartner)
						goto AI_STANDARD_DAMAGE;

				__attribute__ ((fallthrough));
				default:
					if (bankDef != bankAtkPartner) //Don't heal enemies
						viability -= 10;
					else
					{
						if (BATTLER_MAX_HP(bankDef))
							viability -= 10;
						if (gBattleMons[bankDef].hp > gBattleMons[bankDef].maxHP / 2)
							viability -= 5;
						else
							goto AI_SUBSTITUTE_CHECK;
					}
			}
			break;
			
		case EFFECT_TOPSY_TURVY_ELECTRIFY:
			switch (move) {
				case MOVE_ELECTRIFY:
					if (!MoveWouldHitFirst(move, bankAtk, bankDef)
					||  GetMoveTypeSpecial(bankDef, predictedMove) == TYPE_ELECTRIC) //Move will already be electric type
						viability -= 10;
					else
						goto AI_SUBSTITUTE_CHECK;
					break;

				default: ; //Topsy Turvy
					if (bankDef != bankAtkPartner)
					{
						u8 targetPositiveStages = CountBanksPositiveStatStages(bankDef);
						u8 targetNegativeStages = CountBanksNegativeStatStages(bankDef);
						
						if (targetPositiveStages == 0) //No good stat changes to make bad
							viability -= 10;
						if (targetNegativeStages < targetPositiveStages)
							viability -= 5; //More stages would be made positive than negative
					}
			}
			break;
		
		case EFFECT_FAIRY_LOCK_HAPPY_HOUR:
			switch (move) {
				case MOVE_FAIRYLOCK:
					if (gNewBS->FairyLockTimer != 0)
						viability -= 10;
					break;

				case MOVE_HAPPYHOUR:
					if (!(gBattleTypeFlags & (BATTLE_TYPE_LINK
											| BATTLE_TYPE_EREADER_TRAINER
											| BATTLE_TYPE_FRONTIER
											| BATTLE_TYPE_TRAINER_TOWER))
					|| SIDE(bankAtk) != B_SIDE_PLAYER //Only increase money amount if will benefit player
					|| gNewBS->HappyHourByte != 0) //Already used Happy Hour
					{
						if (!IsTypeZCrystal(atkItem, moveType) || gNewBS->ZMoveData->used[bankAtk])
							viability -= 10;
					}
					break;

				case MOVE_CELEBRATE:
				case MOVE_HOLDHANDS:
					if (!IsTypeZCrystal(atkItem, moveType)
					|| atkQuality != moveType
					|| gNewBS->ZMoveData->used[bankAtk])
						viability -= 10;
					break;
			}
			break;
	
		case EFFECT_INSTRUCT_AFTER_YOU_QUASH:
			switch (move) {
				case MOVE_INSTRUCT: ;
					u16 instructedMove;

					if (!MoveWouldHitFirst(move, bankAtk, bankDef))
						instructedMove = predictedMove;
					else
						instructedMove = gLastPrintedMoves[bankDef];
					
					if (instructedMove == MOVE_NONE
					||  CheckTableForMove(instructedMove, InstructBanList)
					||  CheckTableForMove(instructedMove, MovesThatRequireRecharging)
					||  CheckTableForMove(instructedMove, MovesThatCallOtherMovesTable)
					|| (IsZMove(instructedMove))
					|| (gLockedMoves[bankDef] != 0 && gLockedMoves[bankDef] != 0xFFFF)
					||  gBattleMons[bankDef].status2 & STATUS2_MULTIPLETURNS)
						viability -= 10;
					else if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
					{
						if (bankDef != bankAtkPartner)
							viability -= 10;
					}
					else
					{
						if (gBattleMoves[instructedMove].target & (MOVE_TARGET_SELECTED
																 | MOVE_TARGET_DEPENDS
																 | MOVE_TARGET_RANDOM
																 | MOVE_TARGET_BOTH
																 | MOVE_TARGET_ALL
																 | MOVE_TARGET_OPPONENTS_FIELD)
						&& instructedMove != MOVE_MINDBLOWN)
							viability -= 10; //Don't force the enemy to attack you again unless it can kill itself with Mind Blown
						else if (instructedMove != MOVE_MINDBLOWN)
							viability -= 5; //Do something better
					}
					break;

				case MOVE_QUASH:
					if (!(gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
					|| !MoveWouldHitFirst(move, bankAtk, bankDef))
						viability -= 10;
					break;

				default: //After You
					if (bankDef != bankAtkPartner
					|| !(gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
					|| !MoveWouldHitFirst(move, bankAtk, bankDef))
						viability -= 10;
					break;
			}
			break;

		case EFFECT_SUCKER_PUNCH: ;
			if (predictedMove == MOVE_NONE
			||  SPLIT(predictedMove) == SPLIT_STATUS
			|| !MoveWouldHitFirst(move, bankAtk, bankDef))
				viability -= 10;
			else
				goto AI_STANDARD_DAMAGE;
			break;

		case EFFECT_TEAM_EFFECTS:
			switch (move) {
				case MOVE_TAILWIND:
					if (gNewBS->TailwindTimers[SIDE(bankAtk)] != 0)
						viability -= 10;
					break;
	
				case MOVE_LUCKYCHANT:
					if (gNewBS->LuckyChantTimers[SIDE(bankAtk)] != 0)
						viability -= 10;
					break;

				case MOVE_MAGNETRISE:
					if (gNewBS->GravityTimer != 0
					|| gNewBS->MagnetRiseTimers[bankAtk] != 0
					|| atkEffect == ITEM_EFFECT_IRON_BALL
					|| atkStatus3 & (STATUS3_ROOTED | STATUS3_LEVITATING | STATUS3_SMACKED_DOWN)
					|| CheckGrounding(bankAtk) == IN_AIR)
						viability -= 10;
					break;
			}
			break;
		
		case EFFECT_CAMOUFLAGE:
			if (IsOfType(bankAtk, GetCamouflageType()))
				viability -= 10;
			break;

		case EFFECT_LASTRESORT_SKYDROP:
			switch (move) {
				case MOVE_LASTRESORT:
					if (!CanUseLastResort(bankAtk))
						viability -= 10;
					else
						goto AI_STANDARD_DAMAGE;
					break;
				
			default: //Sky Drop
				if (WillFaintFromWeatherSoon(bankAtk)
				||  MoveBlockedBySubstitute(move, bankAtk, bankDef)
				||  GetActualSpeciesWeight(defSpecies, defAbility, defEffect, bankDef, TRUE) >= 2000) //200.0 kg
					viability -= 10;
				else
					goto AI_STANDARD_DAMAGE;
			}
			break;

		case EFFECT_SYNCHRONOISE:
			//Check holding ring target or is of same type
			if (defEffect == ITEM_EFFECT_RING_TARGET
			|| IsOfType(bankDef, gBattleMons[bankAtk].type1)
			|| IsOfType(bankDef, gBattleMons[bankAtk].type2)
			|| IsOfType(bankDef, gBattleMons[bankAtk].type3))
				goto AI_STANDARD_DAMAGE;
			else
				viability -= 10;
			break;
		
		default:
		AI_STANDARD_DAMAGE: ;
			if (moveSplit != SPLIT_STATUS)
			{
				if (TypeCalc(move, bankAtk, bankDef, 0, FALSE) & (MOVE_RESULT_NO_EFFECT | MOVE_RESULT_MISSED))
					viability -= 10;
			}
			break;
	} //Move effects switch

	//Put Acc check here

	return viability;
}

static void AI_Flee(void)
{
	AI_THINKING_STRUCT->aiAction |= (AI_ACTION_DONE | AI_ACTION_FLEE | AI_ACTION_DO_NOT_ATTACK);
}

u8 AI_Script_Roaming(const u8 bankAtk, const unusedArg u8 bankDef, const unusedArg u16 move, const u8 originalViability)
{
	u8 atkAbility = ABILITY(bankAtk);
	u8 atkEffect = ITEM_EFFECT(bankAtk);

	if (atkAbility == ABILITY_RUNAWAY
	||  atkEffect == ITEM_EFFECT_CAN_ALWAYS_RUN
	||  IsOfType(bankAtk, TYPE_GHOST))
	{
		AI_THINKING_STRUCT->aiAction |= (AI_ACTION_DONE | AI_ACTION_FLEE | AI_ACTION_DO_NOT_ATTACK);
	}
	
	else if (IsTrapped(bankAtk, FALSE))
	{
		return originalViability;
	}
	
	AI_Flee();
	return originalViability;
}

static void AI_Watch(void)
{
	AI_THINKING_STRUCT->aiAction |= (AI_ACTION_DONE | AI_ACTION_WATCH | AI_ACTION_DO_NOT_ATTACK);
}

u8 AI_Script_Safari(const unusedArg u8 bankAtk, const unusedArg u8 bankDef, const unusedArg u16 move, const u8 originalViability)
{
	u8 safariFleeRate = gBattleStruct->safariEscapeFactor * 5; // Safari flee rate, from 0-20.

	if ((u8) (Random() % 100) < safariFleeRate)
	{
		AI_Flee();
	}
	else
	{
		AI_Watch();
	}

	return originalViability;
}

u8 AI_Script_FirstBattle(const unusedArg u8 bankAtk, const unusedArg u8 bankDef, const unusedArg u16 move, const u8 originalViability)
{
	u8 viability = originalViability;
	if (GetHealthPercentage(bankDef) < 20 &&  SPLIT(move) != SPLIT_STATUS)
		viability -= 20; //Only use status moves to let the player win
		
	return viability;
}