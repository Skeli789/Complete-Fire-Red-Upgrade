#include "../defines.h"
#include "../../include/random.h"
#include "../../include/constants/items.h"

#include "../../include/new/AI_Helper_Functions.h"
#include "../../include/new/damage_calc.h"
#include "../../include/new/helper_functions.h"

extern const struct NaturalGiftStruct gNaturalGiftTable[];

/*
u8 AI_Script_Positives(const u8 bankAtk, const u8 bankDef, const u16 move, const u8 originalViability)
{
	u32 i;
	u16 predictedMove = IsValidMovePrediction(bankDef, bankAtk); //The move the target is likely to make against the attacker
	u8 viability = originalViability;
	
	struct FightingStyle fightStyle = PredictBankFightingStyle(bankAtk, style);
	
	// get relevant params
	u8 moveEffect = gBattleMoves[move].effect;
	u8 moveType = GetMoveTypeSpecial(bankAtk, move);
	
	u8 bankAtkPartner = PARTNER(bankAtk);
	u8 bankDefPartner = PARTNER(bankDef);
	
	u16 atkSpecies = SPECIES(bankAtk);
	u16 defSpecies = SPECIES(bankDef);
	
	u16 atkItem = ITEM(bankAtk);
	u16 defItem = ITEM(bankDef);
	u8 atkItemEffect = ITEM_EFFECT(bankAtk);
	u8 defItemEffect = ITEM_EFFECT(bankDef);
	u8 atkPartnerItemEffect = ITEM_EFFECT(bankAtkPartner);
	
	u8 atkItemQuality = ITEM_QUALITY(bankAtk);
	
	u8 atkItemPocket = ITEM_POCKET(bankAtk);
	u8 atkPartnerItemPocket = ITEM_POCKET(bankAtkPartner);
	u8 defItemPocket = ITEM_POCKET(bankDef);
	u8 defPartnerItemPocket = ITEM_POCKET(bankDefPartner);
	
	u8 atkAbility = ABILITY(bankAtk);
	u8 defAbility = ABILITY(bankDef);
	u8 atkPartnerAbility = ABILITY(bankAtkPartner);
	u8 defPartnerAbility = ABILITY(bankDefPartner);
	
	u32 atkStatus1 = gBattleMons[bankAtk].status1;
	u32 atkStatus2 = gBattleMons[bankAtk].status2;
	u32 atkStatus3 = gStatuses3[bankAtk];
	u32 defStatus1 = gBattleMons[bankDef].status1;
	u32 defStatus2 = gBattleMons[bankDef].status2;
	u32 defStatus3 = gStatuses3[bankDef];
	
	u32 defPartnerStatus2 = gBattleMons[bankDefPartner].status2;
	
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
	
	u8 moveSplit = SPLIT(move);

	//Alternative targets
	u8 foe1, foe2;
	foe1 = FOE(bankAtk);
				
	if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
		foe2 = PARTNER(FOE(bankAtk))
	else
		foe2 = foe1;
				
	switch (moveEffect) {
		case EFFECT_HIT:
			// to do
			break;
		
		case EFFECT_SLEEP:
		case EFFECT_YAWN:
			if (defItemEffect == ITEM_EFFECT_CURE_SLP
			|| defItemEffect == ITEM_EFFECT_CURE_STATUS
			|| defAbility == ABILITY_EARLYBIRD
			|| defAbility == ABILITY_SHEDSKIN)
				break;
			else
				viability += 6;
			break;
			
		case EFFECT_ABSORB:
		AI_DRAIN_HP_CHECK: ;
			if (!CanFightStyleRecover(fightStyle) //Attacker has other means of recovery
			&& (atkItemEffect == ITEM_EFFECT_BIG_ROOT|| GetHealthPercentage(bankAtk) <= 50))
				viability += 5;
			break;
		
		case EFFECT_PARALYZE_HIT:
			if (gBattleMoves[move].secondaryEffectChance >= 75)
				goto AI_PARALYZE;
		
		case EFFECT_EXPLOSION:
			if (predictedMove != MOVE_NONE) //If foe isn't going to attack, don't kill yourself now
			{
				if (atkItemEffect == ITEM_EFFECT_GEM
				&& ITEM_QUALITY(bankAtk) == moveType) //AI Was meant to explode
				{	
					viability += 6;
				}
				else if (atkItemEffect == ITEM_EFFECT_CUSTAP_BERRY
				&& GetHealthPercentage(bankAtk) <= 25)
				{
					viability += 6;
				}
			}
			break;
			
		case EFFECT_DREAM_EATER:
			if (defAbility == ABILITY_COMATOSE
			||  defStatus1 & STATUS1_SLEEP)
				goto AI_DRAIN_HP_CHECK;
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
							break;
						else
							return AI_Script_Positives(bankAtk, bankDef, gNewBS->LastUsedMove, originalViability);
					}
					else
					{
						if (predictedMove == MOVE_NONE)
							goto COPYCAT_CHECK_LAST_MOVE;
						else if (CheckTableForMove(predictedMove, CopycatBanTable)
						     || FindMovePositionInMoveset(predictedMove, bankAtk) < 4)
						{
							break;
						}
						else
							return AI_Script_Positives(bankAtk, bankDef, predictedMove, originalViability);
					}
					break;

				default: //Mirror Move
					if (gBattleStruct->lastTakenMoveFrom[bankAtk][bankDef] != MOVE_NONE)
						return AI_Script_Positives(bankAtk, bankDef, gBattleStruct->lastTakenMoveFrom[bankAtk][bankDef], originalViability);
			}
			break;
			
		// increased stat effects
		case EFFECT_ATTACK_UP:
		case EFFECT_ATTACK_UP_2:
			switch (move) {
				case MOVE_HONECLAWS:
					if (STAT_STAGE(bankAtk,STAT_STAGE_ATK) >= 8)
						goto AI_ACCURACY_PLUS;
					else if (PhysicalMoveInMoveset(bankAtk)
					&& IsFightStyleSweeper(fightStyle)
					&& (!CanKnockOut(bankDef, bankAtk) || predictedMove == MOVE_NONE))
						viability += 6;
					goto AI_ACCURACY_PLUS;
					
			default:
			AI_ATTACK_PLUS:
				if (STAT_STAGE(bankAtk,STAT_STAGE_ATK) >= 8)
					break;
				else if (PhysicalMoveInMoveset(bankAtk)
				&& IsFightStyleSweeper(fightStyle)
				&& (!CanKnockOut(bankDef, bankAtk) || predictedMove == MOVE_NONE)
				&& atkAbility != ABILITY_CONTRARY)
					viability += 6;
				break;
			}
			break;
			
		case EFFECT_DEFENSE_UP:
		case EFFECT_DEFENSE_UP_2:
		AI_DEFENSE_PLUS:
			switch (move) {
				case MOVE_FLOWERSHIELD:
					if (IsFightStyleTeamHelper(fightStyle)
					&& IsOfType(bankAtkPartner, TYPE_GRASS)
					&& atkPartnerAbility != ABILITY_CONTRARY
					&& STAT_STAGE(bankAtkPartner, STAT_STAGE_DEF) < 12
					&& BATTLER_ALIVE(bankAtkPartner))
					{
						viability += 5;
					}
					break;
					
				case MOVE_MAGNETICFLUX:
					if (IsFightStyleTeamHelper(fightStyle)
					&& (atkPartnerAbility == ABILITY_PLUS || atkPartnerAbility == ABILITY_MINUS)
					&& (STAT_STAGE(bankAtkPartner, STAT_STAGE_DEF) < 12 || STAT_STAGE(bankAtkPartner, STAT_STAGE_SPDEF) < 12))
					&& BATTLER_ALIVE(bankAtkPartner)
					{
						viability += 5;
					}
					else if (BankLikelyToUseMoveSplit(bankDef) == SPLIT_PHYSICAL)
						break;
					
					goto AI_SPDEF_PLUS;
					
				case MOVE_AROMATICMIST:
					if (IsFightStyleTeamHelper(fightStyle)
					&& STAT_STAGE(bankAtkPartner, STAT_STAGE_SPDEF) < 12
					&& BATTLER_ALIVE(bankAtkPartner)
					&& atkPartnerAbility != ABILITY_CONTRARY)
						viability += 5;
					break;
			}

			//Continue defense check
			if (IsFightingStyleWall(fightStyle)
			&& STAT_STAGE(bankAtk, STAT_STAGE_DEF) < 12)
			&& BankLikelyToUseMoveSplit(bankDef) == SPLIT_PHYSICAL
			&& atkAbility != ABILITY_CONTRARY)
				viability += 5;
			break;	
			
		case EFFECT_SPEED_UP:
		case EFFECT_SPEED_UP_2:
		AI_SPEED_PLUS:
			if (IsFightStyleSpeedy(fightStyle)
			&& !MoveWouldHitFirst(move, bankAtk, bankDef))
			&& atkAbility != ABILITY_CONTRARY)
				viability += 6;
			break;
			
		case EFFECT_SPECIAL_ATTACK_UP:
		case EFFECT_SPECIAL_ATTACK_UP_2:
			switch (move)
			{
				case MOVE_GROWTH
				case MOVE_WORKUP:
					if (PhysicalMoveInMoveset(bankAtk))
						goto AI_ATTACK_PLUS;
					else if (SpecialMoveInMoveset(bankAtk))
						goto AI_SP_ATTACK_PLUS;
					break;
					
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
					
				default:
				AI_SP_ATTACK_PLUS:
					if (STAT_STAGE(bankAtk, STAT_STAGE_SPATK) >= 8)
						break;
					else if (SpecialMoveInMoveset(bankAtk)
					&& IsFightStyleSweeper(fightStyle)
					&& (!CanKnockOut(bankDef, bankAtk) || predictedMove == MOVE_NONE)
					&& atkAbility != ABILITY_CONTRARY)
						viability += 6;
					break;
			}
			break;
			
		case EFFECT_SPECIAL_DEFENSE_UP:
		case EFFECT_SPECIAL_DEFENSE_UP_2:
		AI_SPDEF_PLUS: ;
			if (IsFightingStyleWall(fightStyle)
			&& STAT_STAGE(bankAtk, STAT_STAGE_SPDEF) < 12)
			&& BankLikelyToUseMoveSplit(bankDef) == SPLIT_SPECIAL
			&& atkAbility != ABILITY_CONTRARY)
				viability += 5;
			break;
			
		case EFFECT_ACCURACY_UP:
		case EFFECT_ACCURACY_UP_2:
		case EFFECT_MINIMIZE:
		AI_ACCURACY_PLUS: ;
			if (MoveInMovesetWithAccuracyLessThan(bankAtk, 90))
			&& defAbility != ABILITY_CONTRARY)
				viability += 5;
			break;
			
		case EFFECT_EVASION_UP:
		case EFFECT_EVASION_UP_2:
			if (atkAbility != ABILITY_CONTRARY)
				viability += 6;
			break;
			
		// decreased stat effects 
		case EFFECT_ATTACK_DOWN:
		case EFFECT_ATTACK_DOWN_2:
			if (STAT_STAGE(bankDef,STAT_STAGE_ATK) < 5)
				break;
			else if (STAT_STAGE(bankDef,STAT_STAGE_ATK) > STAT_STAGE(bankDef,STAT_STAGE_SPATK) && defAbility != ABILITY_CONTRARY)
				viability += 3;
			break;
			
		case EFFECT_DEFENSE_DOWN:
		case EFFECT_DEFENSE_DOWN_2:
		AI_DEFENSE_MINUS:
			if (STAT_STAGE(bankDef,STAT_STAGE_DEF) < 5)
				break;
			else if (PhysicalMoveInMoveset(bankAtk) && defAbility != ABILITY_CONTRARY)
				viability += 3;
			break;
			
		case EFFECT_SPEED_DOWN:
		case EFFECT_SPEED_DOWN_2:
			if (MoveWouldHitFirst(move, bankAtk, bankDef))
				break;
			else if (defAbility != ABILITY_CONTRARY)
				viability += 5;
			break;
			
		case EFFECT_SPECIAL_ATTACK_DOWN:
		case EFFECT_SPECIAL_ATTACK_DOWN_2:
			if (STAT_STAGE(bankDef, STAT_STAGE_SPATK) < 5)
				break;
			else if (STAT_STAGE(bankDef, STAT_STAGE_SPATK) > STAT_STAGE(bankDef, STAT_STAGE_ATK) && defAbility != ABILITY_CONTRARY)
				viability += 3;
			break;
			
		case EFFECT_SPECIAL_DEFENSE_DOWN:
		case EFFECT_SPECIAL_DEFENSE_DOWN_2:
			if (STAT_STAGE(bankDef, STAT_STAGE_SPDEF) < 5)
				break;
			else if (SpecialMoveInMoveset(bankAtk) && defAbility != ABILITY_CONTRARY)
				viability += 3;
			break;
			
		case EFFECT_ACCURACY_DOWN:
		case EFFECT_ACCURACY_DOWN_2:
			if (defAbility != ABILITY_CONTRARY)
				viability += 3;
			break;
			
		case EFFECT_EVASION_DOWN:
		case EFFECT_EVASION_DOWN_2:
			if (STAT_STAGE(bankDef,STAT_STAGE_EVASION) < 5)
				break;
			else if (defAbility != ABILITY_CONTRARY)
				viability += 3;
			break;

		case EFFECT_HAZE:
			//If any stat buffs are below 6, increase viability
			for (i = 0; i < BATTLE_STATS_NO - 1; ++i)
			{
				if (STAT_STAGE(bankAtk, i) < 6)
				{	
					viability += 5;
					break;
				}
			}
			
			if (IsFightStylesHazer(fightStyle))
			{
				for (i = 0; i < BATTLE_STATS_NO - 1; ++i)
				{
					if ((BATTLER_ALIVE(foe1) && STAT_STAGE(foe1, i) > 6)
					||  (BATTLER_ALIVE(foe2) && STAT_STAGE(foe2, i) > 6))
					{	
						viability += 5;
						break;
					}
				}
			}
			
			break;
			
		case EFFECT_BIDE:
			if (DamagingMoveInMoveset(bankAtk))
				break;
			else if (IsFightStyleSweeper(fighStyle))
				viability += 3;
			break;
			
		case EFFECT_RAMPAGE:
			if (IsFightStyleSweeper(fightStyle)
			&& !IsStrongestMove(move, bankAtk, bankDef)
			&& !TeamHasMonResistantToMove(bankDef, move)
			&& (atkItemEffect == ITEM_EFFECT_CURE_CONFUSION
				|| atkItemEffect == ITEM_EFFECT_CURE_STATUS
				|| atkAbility == ABILITY_OWNTEMPO))
			{
				viability += 5;
			}	
			break;
		
		case EFFECT_ROAR:
			if (IsFightStyleForceSwitcher(fightStyle)
			&&  !CanKnockOut(bankAtk, bankDef))
			{
				if (EntryHazardsAreUpOnBanksSide(bankDef))
					viability += 5;
				
				else
				{
					for (i = 0; i < BATTLE_STATS_NO - 1; ++i)
					{
						if (STAT_STAGE(foe1, i) > 6)
						{	
							viability += 5;
							break;
						}
					}
				}
			}

		case EFFECT_MULTI_HIT:
			if (defStatus2 & STATUS2_SUBSTITUTE
			||  atkItemEffect == ITEM_EFFECT_FLINCH)
				viability += 5;
			break;
			
		case EFFECT_CONVERSION:
			if (IsOfType(bankAtk, gBattleMoves[gBattleMons[bankAtk].moves[0]].type)
			&& IsFightStyleSweeper(fightStyle))
				viability += 5;
			break;
			
		case EFFECT_FLINCH_HIT:
			if (defAbility == ABILITY_INNERFOCUS)
			||  defStatus2 & STATUS2_SUBSTITUTE)
			||  !MoveWouldHitFirst(move, bankAtk, bankDef))
				break;
			else if (atkAbility == ABILITY_SERENEGRACE)
			|| defStatus1 & STATUS1_PARALYSIS)
			|| defStatus2 & (STATUS2_INFATUATION | STATUS2_CONFUSION))
				viability += 5;
			break;
			
		case EFFECT_RESTORE_HP:
		case EFFECT_MORNING_SUN:
		case EFFECT_SWALLOW:
		case EFFECT_WISH:
			if (!(IsFightStyleSweeper(fightStyle) && CanKillAFoe(bankAtk))
			|| IsFightStyleWall)
			{
				if (MoveWouldHitFirst(move, bankAtk, foe1)
				&& MoveWouldHitFirst(move, bankAtk, foe1)
				&& GetHealthPercentage(bankAtk) <= 50)
					viability += 5;
				else if (GetHealthPercentage(bankAtk) <= 65
				&& (PredictedMoveCanCauseDamage(foe1, bankAtk) || PredictedMoveCanCauseDamage(foe2, bankAtk)))
					viability += 5;
			}
			break;
			
		case EFFECT_TOXIC:
		case EFFECT_POISON:
			if (!CanBePoisoned(bankDef, bankAtk, TRUE)
			||  defItemEffect == ITEM_EFFECT_CURE_PSN
			||  defItemEffect == ITEM_EFFECT_CURE_STATUS
			||  defAbility == ABILITY_SHEDSKIN 
			||  defAbility == ABILITY_TOXICBOOST
			||  defAbility == ABILITY_POISONHEAL
			||  atkAbility == ABILITY_POISONTOUCH)
				break;
			else if (MoveInMoveset(MOVE_VENOSHOCK, bankAtk)
			|| MoveInMoveset(MOVE_HEX, bankAtk)
			|| MoveInMoveset(MOVE_VENOMDRENCH, bankAtk)
			|| atkAbility == ABILITY_MERCILESS)
				viability += 6;
			else if (IsFightStyleStatuser(fightStyle))
				viability += 5;	//AI enjoys poisoning
			break;
			
		case EFFECT_LIGHT_SCREEN:
			if (!(gSideAffecting[SIDE(bankAtk)] & SIDE_STATUS_LIGHTSCREEN)
			&& (IsFightStyleTeamHelper(fightStyle) || !CanKillAFoe(bankAtk))
			&& (SpecialMoveInMoveset(foe1) || SpecialMoveInMoveset(foe2)))
				viability += 5;
			break;
			
		case EFFECT_REST:
			if (!(CanBePutToSleep(bankAtk, FALSE)
			|| GetHealthPercentage(bankAtk) > 50
			|| (IsFightStyleSweeper(fightStyle) && CanKillAFoe(bankAtk)))
				break;
			else if (atkItemEffect == ITEM_EFFECT_CURE_SLP 
			|| atkItemEffect == ITEM_EFFECT_CURE_STATUS
			|| MoveEffectInMoveset(EFFECT_SLEEPTALK, bankAtk) 
			|| MoveEffectInMoveset(EFFECT_SNORE, bankAtk)
			|| atkAbility == ABILITY_SHEDSKIN)
				viability += 5;
			else if ((gBattleWeather & WEATHER_RAIN_ANY) && atkAbility == ABILITY_HYDRATION)
				viability += 4;
			else if (atkAbility == ABILITY_EARLYBIRD)
				viability += 3;
			break;

		case EFFECT_TRAP:
			if (MoveInMoveset(MOVE_RAPIDSPIN, bankDef)
			||  IsOfType(bankDef, TYPE_GHOST)
			||  defStatus2 & (STATUS2_WRAPPED))
			{
				break; //Just a regular attacking move now
			}
			else if (atkItemEffect == ITEM_EFFECT_BINDING_BAND
			|| atkItemEffect == ITEM_EFFECT_GRIP_CLAW)
				viability += 5;
			else
				goto AI_TRAP_TARGET_CHECK;	//Check other conditions to trap target
			break;

		case EFFECT_TRIPLE_KICK:
		case EFFECT_DOUBLE_HIT:
			if (defStatus2 & STATUS2_SUBSTITUTE)
				viability += 5;
			break;
		
		case EFFECT_MIST:
			if (IsFightStyleTeamHelper(fightStyle)
			&&  EnemiesHaveMovesWhichCanLowerStatsOrIntimidate(foe1, foe2))
				viability += 5;
		
		case EFFECT_FOCUS_ENERGY:
		AI_FOCUS_ENERGY:
			if (atkAbility == ABILITY_SUPERLUCK
			|| atkAbility == ABILITY_SNIPER)
			|| atkItemEffect == ITEM_EFFECT_SCOPE_LENS)
				viability += 6;
			break;

		case EFFECT_CONFUSE:
		AI_CONFUSE_CHECK:
			if (CanBeConfused(bankDef) || (move == MOVE_TEETERDANCE && CanBeConfused(bankDefPartner)))
			{
				if (defStatus1 & STATUS1_PARALYSIS
				|| defStatus2 & (STATUS2_INFATUATION)
				|| (atkAbility == ABILITY_SERENEGRACE && FlinchingMoveInMoveset(bankAtk)))
				{
					viability += 6;
					break;
				}
				else if (IsFightStyleStatuser(fightStyle))
				{
					viability += 5;
					break;
				}
			}
			
			if (gBattleMoves[move].target & MOVE_TARGET_BOTH && CanBeConfused(bankDefPartner))
			{
				if (defPartnerStatus1 & STATUS1_PARALYSIS
				|| defPartnerStatus2 & (STATUS2_INFATUATION)
				|| (atkAbility == ABILITY_SERENEGRACE && FlinchingMoveInMoveset(bankAtk)))
					viability += 6;
				else if (IsFightStyleStatuser(fightStyle))
					viability += 5;
			}
			break;
		
		case EFFECT_REFLECT:
			switch (move) {
				case MOVE_AURORAVEIL:
					if (gNewBS->AuroraVeilTimers[SIDE(bankAtk)] != 0 && gBattleWeather & WEATHER_HAIL_ANY)
					&& (IsFightStyleTeamHelper(fightStyle) || !CanKillAFoe(bankAtk))
						viability += 5;
					break;

				default:
					if (!(gSideAffecting[SIDE(bankAtk)] & SIDE_STATUS_REFLECT)
					&& (IsFightStyleTeamHelper(fightStyle) || !CanKillAFoe(bankAtk))
					&& (PhysicalMoveInMoveset(foe1) || PhysicalMoveInMoveset(foe2)))
						viability += 5;
			}
			break;
			
		case EFFECT_PARALYZE:
		AI_PARALYZE:
			if (CanBeParalyzed(bankDef, TRUE))
			{
				if (defItemEffect == ITEM_EFFECT_CURE_PAR
				|| defItemEffect == ITEM_EFFECT_CURE_STATUS)
				|| defAbility == ABILITY_SHEDSKIN)
					break;
				else if (MoveInMoveset(MOVE_HEX, bankAtk)
				|| FlinchingMoveInMoveset(bankAtk)
				|| defStatus2 & (STATUS2_CONFUSION | STATUS2_INFATUATION))
					viability += 6;
				else if (IsFightStyleStatuser(fightStyle))
					viability += 5;
			}
			break;

		case EFFECT_ATTACK_DOWN_HIT:
		case EFFECT_DEFENSE_DOWN_HIT:
		case EFFECT_SPEED_DOWN_HIT:
		case EFFECT_SPECIAL_ATTACK_DOWN_HIT:
		case EFFECT_SPECIAL_DEFENSE_DOWN_HIT:
		case EFFECT_ACCURACY_DOWN_HIT:
		case EFFECT_EVASION_DOWN_HIT:
			if (gBattleMoves[move].secondaryEffectChance >= 50
			&&  DoesFightStyleLowerStats(fightStyle))
				viability += 3;
			break;
			
		case EFFECT_CONFUSE_HIT:
			if (gBattleMoves[move].secondaryEffectChance >= 75)
				goto AI_CONFUSE;

		case EFFECT_SUBSTITUTE:
			if (!IS_BEHIND_SUBSTITUTE(bankAtk)
			&& (GetHealthPercentage(bankAtk) > 50 || (!IsDoubleBattle() && IsPredictedToSwitch(foe1)))
				viability += 6;
			break;

		case EFFECT_MIMIC:
			if (MoveWouldHitFirst(move, bankAtk, bankDef))
			{ 	
				if (gLastUsedMoves[bankDef] != MOVE_NONE
				&& gLastUsedMoves[bankDef] != 0xFFFF
				&& MoveKnocksOut(gLastUsedMoves[bankDef], bankAtk, bankDef))
					viability += 5;
			}
			else if (predictedMove != MOVE_NONE
			&& MoveKnocksOut(predictedMove, bankAtk, bankDef)) //Attacker can kill with mimicked move
				viability += 5;
		
		case EFFECT_LEECH_SEED:
			if (IsOfType(bankDef, TYPE_GRASS)
			|| defStatus3 & STATUS3_LEECHSEED
			|| MoveInMoveset(MOVE_RAPIDSPIN, bankDef))
			|| defAbility == ABILITY_LIQUIDOOZE)
				break;
			else
				viability += 6;
			break;
		
		case EFFECT_SPLASH:
			if (IsZCrystal(atkItem) && ITEM_QUALITY(atkItem) == moveType
			&&  !IsSpecialZCrystal(atkItem))
				viability += 9; //Z-Splash!
				
		case EFFECT_DISABLE:
			if (gDisableStructs[bankDef].disableTimer1 == 0
			&&  defEffect != ITEM_EFFECT_CURE_ATTRACT)
			{
				if (MoveWouldHitFirst(move, bankAtk, bankDef))
				{ 	
					if (gLastUsedMoves[bankDef] != MOVE_NONE
					&& gLastUsedMoves[bankDef] != 0xFFFF
					&& MoveKnocksOut(gLastUsedMoves[bankDef], bankDef, bankAtk))
						viability += 5; //Disable move that can kill attacker
				}
				else if (predictedMove != MOVE_NONE
				&& SPLIT(predictedMove) == SPLIT_STATUS)
					viability += 4; //Disable annoying status moves
			}
			break;

		case EFFECT_ENCORE:
			if (gDisableStructs[bankDef].encoreTimer == 0
			&&  defEffect != ITEM_EFFECT_CURE_ATTRACT)
			{
				if (MoveWouldHitFirst(move, bankAtk, bankDef))
				{ 	
					if (gLastUsedMoves[bankDef] != MOVE_NONE
					&& gLastUsedMoves[bankDef] != 0xFFFF
					&& SPLIT(gLastUsedMoves[bankDef]) == SPLIT_STATUS)
						viability += 5; //Lock into status moves
				}
				else if (predictedMove != MOVE_NONE
				&& SPLIT(predictedMove) == SPLIT_STATUS)
					viability += 5; //Lock into status moves
			}
			break;

		case EFFECT_PAIN_SPLIT: ;
			//20 % health difference
			defPercent = (GetHealthPercentage(bankDef)) < 20 ? 0 : GetHealthPercentage(bankDef);
			
			if (GetHealthPercentage(bankAtk) < defPercent)
				goto AI_RECOVERY;
			break;

		case EFFECT_SNORE:
		case EFFECT_SLEEP_TALK:
			if (atkStatus1 & STATUS1_SLEEP)
				viability += 8;
			break;
			
		case EFFECT_LOCK_ON:
			switch (move) {
				case MOVE_LASERFOCUS:
					goto AI_FOCUS_ENERGY;
				
				default:	
					if (MoveEffectInMoveset(EFFECT_0HKO, bankAtk))
						viability += 6;
			}
			break;
		
		case EFFECT_SPEED_UP_1_HIT:
			if (gBattleMoves[move].secondaryEffectChance >= 75)
				goto AI_SPEED_PLUS;
		
		case EFFECT_DESTINY_BOND:
			if (MoveWouldHitFirst(move, bankAtk, bankDef)
			&& (CanKnockOut(foe1, bankAtk) || CanKnockOut(foe2, bankAtk)))
				viability += 7;
			break;
			
		case EFFECT_SPITE:
			if (MoveWouldHitFirst(move, bankAtk, bankDef)
			&&  predictedMove != MOVE_NONE
			&&  gBattleMons[bankDef].pp[FindMoveSlotInMoveset(predictedMove)] <= 4)
				viability += 5;
			break;
			
		case EFFECT_HEAL_BELL:
			if (IsFightStyleTeamHelper())
			{
				if (move == MOVE_HEALBELL)
				{
					if (PartyMemberStatused(bankAtk, TRUE)) //Check Soundproof
						viability += 5;
				}
				else if (PartyMemberStatused(bankAtk, FALSE))
					viability += 5;
			}
			break;
		
		case EFFECT_THIEF:
			if (atkItem == ITEM_NONE
			&& CanTransferItem(bankDef, defItem, GetBankPartyData(bankDef))
			&& CanTransferItem(bankAtk, defItem, GetBankPartyData(bankAtk))
			&& !MoveInMoveset(MOVE_ACROBATICS, bankAtk)
			&& defAbility != ABILITY_STICKYHOLD)
			{
				switch (defItemEffect) {
					case ITEM_EFFECT_CHOICE_BAND: ;
						if (ITEM_QUALITY(bankDef) == SPLIT_STATUS //Choice scard
						|| MoveSplitInMoveset(ITEM_QUALITY(bankDef), bankAtk)
							viability += 4;
						break;

					case ITEM_EFFECT_TOXIC_ORB:
						if (atkAbility == ABILITY_POISONHEAL
						||  atkAbility == ABILITY_TOXICBOOST)
							viability += 4;
						break;
					
					case ITEM_EFFECT_FLAME_ORB:
						if (atkAbility == ABILITY_FLAREBOOST)
							viability += 4;
					
					case ITEM_EFFECT_BLACK_SLUDGE:
						if (IsOfType(bankAtk, TYPE_POISON))
							viability += 4;
						break;
					
					case ITEM_EFFECT_IRON_BALL:
						if (MoveInMoveset(MOVE_FLING, bankAtk))
							viability += 4;
						break;
					
					case ITEM_EFFECT_LAGGING_TAIL:
					case ITEM_EFFECT_STICKY_BARB:
						break;
						
					default:
						viability += 2;
				}
			}
		
		case EFFECT_QUICK_ATTACK:
			if (SpeedCalc(bankAtk) < SpeedCalc(bankDef)
			&& CanKnockOut(bankDef, bankAtk)
			&& MoveWouldGoFirst(bankAtk, bankDef)) //Includes quick attack priority
				viability += 7;
			break;
			
		case EFFECT_MEAN_LOOK:
		AI_TRAP_TARGET_CHECK:
			if (defStatus2 & STATUS2_ESCAPE_PREVENTION
			|| (atkAbility == ABILITY_SHADOWTAG && defAbility != ABILITY_SHADOWTAG)
			|| (atkAbility == ABILITY_ARENATRAP && CheckGrounding(bankDef) == GROUNDED)
			|| (atkAbility == ABILITY_MAGNETPULL && IsOfType(bankDef, TYPE_STEEL))
			||  defStatus3 & (STATUS3_ROOTED))
				break; //Already trapped
			
			if (defStatus1 & STATUS1_ANY	//Better to trap statused target
			|| defStatus2 & (STATUS2_INFATUATION)
			|| defStatus3 & (STATUS3_LEECHSEED))
			{
				viability += 5;
				break;
			}

			//Better to trap target with low stats
			for (i = 0; i < BATTLE_STATS_NO - 1; ++i)
			{
				if (STAT_STAGE(bankDef, i) < 6)
				{
					if (!(gBattleTypeFlags & BATTLE_TYPE_DOUBLE)) //Single battle
					{
						if (i == STAT_STAGE_DEF && (PhysicalMoveInMoveset(bankAtk)))
						{
							viability += 5;
							break;
						}
						else if (i == STAT_STAGE_SPDEF && (SpecialMoveInMoveset(bankAtk)))
						{
							viability += 5;
							break;
						}
					}
					else	// double battle
					{
						if ((i == STAT_STAGE_DEF) &&
						(PhysicalMoveInMoveset(bankAtk) || PhysicalMoveInMoveset(bankAtkPartner)))
						{
							viability += 5;
							break;
						}
						else if ((i == STAT_STAGE_SPDEF) &&
						(SpecialMoveInMoveset(bankAtk) || SpecialMoveInMoveset(bankAtkPartner)))
						{
							viability += 5;
							break;
						}
					}
					break;
				}
			}
			break;
			
		case EFFECT_NIGHTMARE:
			if (defAbility == ABILITY_COMATOSE)
				viability += 5;
			else if (defStatus1 & STATUS1_SLEEP)
				viability += 5;
			break;
		
		case EFFECT_CURSE:
			if (IsOfType(bankAtk, TYPE_GHOST))
			{
				if (atkAbility == ABILITY_SHADOWTAG || atkAbility == ABILITY_ARENATRAP)
					viability += 5;
				else if (atkStatus2 & (STATUS2_ESCAPE_PREVENTION | STATUS2_WRAPPED))
					viability += 5;
				break;
			}
			else
			{
				if (atkAbility == ABILITY_CONTRARY)
					break;
				else if (FindMovePositionInMoveset(MOVE_GYROBALL, bankAtk) != 4)
					viability += 5;
				else if (STAT_STAGE(bankAtk, STAT_STAGE_SPEED) < 3)
					break;
				else if (STAT_STAGE(bankAtk, STAT_STAGE_ATK) < 8)
					viability += 5;
				else if (STAT_STAGE(bankAtk, STAT_STAGE_DEF) < 8)
					viability += 5;
				break;
			}
			break;
			
		case EFFECT_PROTECT:
			switch (move)
			{
				case MOVE_QUICKGUARD:
					if (defAbility == ABILITY_GALEWINGS
					|| defAbility == ABILITY_PRANKSTER)
						viability += 5;
					else if (atkPartnerAbility == ABILITY_GALEWINGS
					|| atkPartnerAbility == ABILITY_PRANKSTER)
						viability += 5;
					else if (MoveEffectInMoveset(EFFECT_QUICK_ATTACK, bankDef)
					|| MoveEffectInMoveset(EFFECT_QUICK_ATTACK, bankAtkPartner))
						viability += 5;
					goto FUNCTION_RETURN;
					
				case MOVE_WIDEGUARD:
					// check if attack targets multiple foes (opponent side or all)
					for (i = 0; i <= 3; ++i)
					{
						if (gBattleMoves[gBattleMons[bankDef].moves[i]].target & (MOVE_TARGET_ALL | MOVE_TARGET_OPPONENTS_FIELD | MOVE_TARGET_BOTH))
							viability += 5;
						else if (gBattleMoves[gBattleMons[bankDefPartner].moves[i]].target 
						& (MOVE_TARGET_ALL | MOVE_TARGET_OPPONENTS_FIELD | MOVE_TARGET_BOTH))
							viability += 5;
						goto FUNCTION_RETURN;
					}
					goto FUNCTION_RETURN;
					
				case MOVE_CRAFTYSHIELD:
				AI_DOUBLEBATTLE_CHECK: ;
					if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
						viability += 5;
					goto FUNCTION_RETURN;
				
				case MOVE_MATBLOCK:
					if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
					{
						if (gDisableStructs[bankAtk].isFirstTurn)
							viability += 7;
						break;
					}
					goto FUNCTION_RETURN;
				
				default:
					if (atkAbility == ABILITY_POISONHEAL && (atkStatus1 & STATUS1_PSN_ANY))
						goto AI_PROTECT_LOW_HEALTH_CHECK;
					if (move == MOVE_SPIKYSHIELD)
					{
						if (PhysicalMoveInMoveset(bankDef))
							goto AI_CHECK_PROTECT_CHANCE;
						else
							goto AI_RANDOM_DOUBLES_PROTECT;
					}
					else if (move == MOVE_BANEFULBUNKER)
					{
						if (defStatus1 & STATUS1_ANY)
							goto AI_RANDOM_DOUBLES_PROTECT;
						else if (PhysicalMoveInMoveset(bankDef))
							goto AI_CHECK_PROTECT_CHANCE;
						goto AI_RANDOM_DOUBLES_PROTECT;
					}
					else if (atkStatus1 & (STATUS1_PSN_ANY | STATUS1_BURN))
						goto AI_CHECK_PROTECT_CHANCE;
					else if (atkStatus1 & (STATUS1_SLEEP | STATUS1_FREEZE))
						goto FUNCTION_RETURN;
					else if (atkItemEffect == ITEM_EFFECT_LEFTOVERS || atkItemEffect == ITEM_EFFECT_BLACK_SLUDGE)
						goto AI_PROTECT_LOW_HEALTH_CHECK;
					else
						goto AI_RANDOM_DOUBLES_PROTECT;
					break;
			} // switch move
		AI_PROTECT_LOW_HEALTH_CHECK: ;
			if (GetHealthPercentage(bankAtk) > 35)
				goto AI_RANDOM_DOUBLES_PROTECT;
		AI_CHECK_PROTECT_CHANCE: ;
			if (gDisableStructs[bankAtk].protectUses > 0)
				break;
			viability += 5;
			break;
		AI_RANDOM_DOUBLES_PROTECT:
			if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE && (umodsi(Random(), 100) < 50))
				goto AI_CHECK_PROTECT_CHANCE;
			break;
			
		// entry hazards - fix?
		case EFFECT_SPIKES:
			if (MoveWouldHitFirst(move, bankAtk, bankDef))
				viability += 6;
			else if (GetHealthPercentage(bankAtk) > 20)
				viability += 6;
			break;
			
		case EFFECT_FORESIGHT:
			if (move == MOVE_MIRACLEEYE)
			{
				if (IsOfType(bankDef, TYPE_DARK))
					viability += 4;
				break;
			}
			else
			{
				if (atkAbility == ABILITY_SCRAPPY)
					break;
				else if (STAT_STAGE(bankDef, STAT_STAGE_EVASION) > 6)
					viability += 4;
				else if (IsOfType(bankDef, TYPE_GHOST))
					viability += 4;
				break;
			}
			break;
			
		case EFFECT_PERISH_SONG:
			if (atkAbility == ABILITY_SHADOWTAG || atkAbility == ABILITY_ARENATRAP)
				viability += 5;
			else if (atkStatus2 & (STATUS2_ESCAPE_PREVENTION | STATUS2_WRAPPED))
				viability += 5;
			break;
			
		case EFFECT_SANDSTORM:
			if (atkAbility == ABILITY_SANDVEIL || atkAbility == ABILITY_SANDRUSH || atkAbility == ABILITY_SANDFORCE)
				viability += 6;
			else if ((IsOfType(bankAtk, TYPE_ROCK)) || (IsOfType(bankAtk, TYPE_STEEL)) || (IsOfType(bankAtk, TYPE_GROUND)))
				viability += 6;
			else if (FindMovePositionInMoveset(MOVE_SHOREUP, bankAtk) != 4)
				viability += 6;
			else if (FindMovePositionInMoveset(MOVE_WEATHERBALL, bankAtk) != 4)
				viability += 6;
			else if (atkItemEffect == ITEM_EFFECT_SMOOTH_ROCK)
				viability += 6;
			else if (atkPartnerAbility == ABILITY_SANDVEIL || atkPartnerAbility == ABILITY_SANDRUSH || atkPartnerAbility == ABILITY_SANDFORCE)
				viability += 6;
			else if ((IsOfType(bankAtkPartner, TYPE_ROCK)) || (IsOfType(bankAtkPartner, TYPE_STEEL)) || (IsOfType(bankAtkPartner, TYPE_GROUND)))
				viability += 6;
			break;
			
		case EFFECT_ROLLOUT:
			if (atkStatus2 & STATUS2_DEFENSE_CURL)
				viability += 5;
			break;
			
		case EFFECT_SWAGGER:
			if (FindMovePositionInMoveset(MOVE_FOULPLAY, bankAtk) != 4)
				viability += 5;
			else
				goto AI_CONFUSE_CHECK;
			break;
			
		case EFFECT_FURY_CUTTER:
			if (atkItemEffect == ITEM_EFFECT_METRONOME)
				viability += 5;
			break;
			
		case EFFECT_ATTRACT:
			if (atkStatus1 & STATUS1_ANY)
				viability += 5;
			else if (atkStatus2 & STATUS2_CONFUSION)
				viability += 5;
			break;
			
		case EFFECT_BATON_PASS:
			switch (move)
			{
				case MOVE_UTURN:
				case MOVE_VOLTSWITCH:
					if (ViableMonCountFromBank(bankAtk) <= 1)
						break;
					else if (atkItemEffect == ITEM_EFFECT_CHOICE_BAND)
						viability += 5;
					goto FUNCTION_CHECK_RESULT;
				
				case MOVE_BATONPASS:
					// pass on boosted stats
					for (i = 0; i <= BATTLE_STATS_NO-1; ++i)
					{
						if (STAT_STAGE(bankAtk, i) > 6)
						{
							viability += 6;
							break;
						}
					}
					// pass on good statuses
					if (atkStatus3 & (STATUS3_ROOTED | STATUS3_AQUA_RING | STATUS3_LEVITATING))
						viability += 6;
					else if (atkStatus2 & STATUS2_SUBSTITUTE)
						viability += 6;
					break;
			}
			break;
			
		case EFFECT_RAPID_SPIN:
			if (move == MOVE_DEFOG)
			{
				if (gSideAffecting[SIDE(bankDef)] & 
				(SIDE_STATUS_REFLECT | SIDE_STATUS_LIGHTSCREEN | SIDE_STATUS_SAFEGUARD | SIDE_STATUS_MIST))
					viability += 6;
				else if (gNewBS->AuroraVeilTimers[SIDE(bankDef)] != 0)
					viability += 6;
				goto AI_RAPIDSPIN_CHECK;
			}
			else
			{
				if (atkStatus3 & STATUS3_LEECHSEED)
					viability += 6;
				else if (atkStatus2 & STATUS2_WRAPPED)
					viability += 6;
			}
		AI_RAPIDSPIN_CHECK: ;
			if (gSideAffecting[SIDE(bankAtk)] & SIDE_STATUS_SPIKES)
				viability += 6;
			break;
			
		case EFFECT_RAIN_DANCE:
			if (atkAbility == ABILITY_SWIFTSWIM 
			|| atkAbility == ABILITY_FORECAST 
			|| atkAbility == ABILITY_HYDRATION
			|| atkAbility == ABILITY_RAINDISH
			|| atkAbility == ABILITY_DRYSKIN)
				viability += 6;
			else if (MoveEffectInMoveset(EFFECT_THUNDER, bankAtk))
				viability += 6;
			else if (FindMovePositionInMoveset(MOVE_WEATHERBALL, bankAtk) != 4)
				viability += 6;
			else if (atkItemEffect == ITEM_EFFECT_DAMP_ROCK)
				viability += 6;
			
			// double battle, partner checks
			if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
			{
				if (MoveEffectInMoveset(EFFECT_THUNDER, bankAtkPartner))
					viability += 6;
				else if (FindMovePositionInMoveset(MOVE_WEATHERBALL, bankAtkPartner) != 4)
					viability += 6;
				else if (atkPartnerItemEffect == ITEM_EFFECT_DAMP_ROCK)
					viability += 6;
				else if (atkPartnerAbility == ABILITY_SWIFTSWIM
				|| atkPartnerAbility == ABILITY_FORECAST
				|| atkPartnerAbility == ABILITY_HYDRATION
				|| atkPartnerAbility == ABILITY_RAINDISH
				|| atkPartnerAbility == ABILITY_DRYSKIN)
					viability += 6;
				break;
			}
			break;
			
		case EFFECT_SUNNY_DAY:
			if (atkAbility == ABILITY_CHLOROPHYLL
			|| atkAbility == ABILITY_FLOWERGIFT
			|| atkAbility == ABILITY_FORECAST
			|| atkAbility == ABILITY_LEAFGUARD
			|| atkAbility == ABILITY_SOLARPOWER
			|| atkAbility == ABILITY_HARVEST)
				viability += 6;
			else if (MoveEffectInMoveset(EFFECT_SOLARBEAM, bankAtk))
				viability += 6;
			else if (FindMovePositionInMoveset(MOVE_WEATHERBALL, bankAtk) != 4)
				viability += 6;
			else if (atkItemEffect == ITEM_EFFECT_HEAT_ROCK)
				viability += 6;
			
			// check double battle, partner
			if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
			{
				if (atkPartnerAbility == ABILITY_CHLOROPHYLL
				|| atkPartnerAbility == ABILITY_FLOWERGIFT
				|| atkPartnerAbility == ABILITY_FORECAST
				|| atkPartnerAbility == ABILITY_LEAFGUARD
				|| atkPartnerAbility == ABILITY_SOLARPOWER
				|| atkPartnerAbility == ABILITY_HARVEST)
					viability += 6;
				else if (MoveEffectInMoveset(EFFECT_SOLARBEAM, bankAtkPartner))
					viability += 6;
				else if (FindMovePositionInMoveset(MOVE_WEATHERBALL, bankAtkPartner) != 4)
					viability += 6;
				else if (atkPartnerItemEffect == ITEM_EFFECT_HEAT_ROCK)
					viability += 6;
				break;
			}
			break;
			
		case EFFECT_ATTACK_UP_HIT:
			if (move == MOVE_POWERUPPUNCH)
				goto AI_ATTACK_PLUS;
			else if (move == MOVE_FELLSTINGER)
			{
				if (MoveKnocksOut(move, bankAtk, bankDef))
					viability += 7;
				break;
			}
			break;
			
		case EFFECT_BELLY_DRUM:
			if (move == MOVE_MINDBLOWN)
				break;
			else if (atkAbility != ABILITY_CONTRARY)
				viability += 6;
			break;
			
		case EFFECT_PSYCH_UP:
			if (move == MOVE_SPECTRALTHIEF)
				goto FUNCTION_CHECK_RESULT;
			else
			{
				// copy positive stat changes
				for (i = 0; i <= BATTLE_STATS_NO-1; ++i)
				{
					if (STAT_STAGE(bankDef, i) > 6)
					{
						if (i == STAT_STAGE_ATK && (PhysicalMoveInMoveset(bankAtk)))
						{
							viability += 3;
							break;
						}
						else if (i == STAT_STAGE_SPATK && (SpecialMoveInMoveset(bankAtk)))
						{
							viability += 3;
							break;
						}
						else if (i == STAT_STAGE_ACC)
						{
							viability += 3;
							break;
						}
					}
				}
			}
			break;
			
		case EFFECT_TWISTER:
		case EFFECT_GUST:
		case EFFECT_THUNDER:
			if (MoveWouldHitFirst(move, bankAtk, bankDef))
				break;
			else if (!(defStatus3 & STATUS3_IN_AIR))
				break;
			else if (defStatus3 & STATUS3_DISAPPEARED)
				break;
			else
				viability += 8;
			break;
		
		case EFFECT_MAGNITUDE:
		case EFFECT_EARTHQUAKE:
			if (MoveWouldHitFirst(move, bankAtk, bankDef))
				break;
			else if (!(defStatus3 & STATUS3_UNDERGROUND))
				break;
			else
				viability += 8;
			break;
			
		case EFFECT_SOLARBEAM:
			if (gBattleWeather & WEATHER_SUN_ANY)
				viability += 5;
			else
				goto AI_CHARGEUP_CHECK;
			break;
			
		case EFFECT_DEFENSE_CURL:
			if (MoveEffectInMoveset(EFFECT_ROLLOUT, bankAtk))
				viability += 6;
			else
				goto AI_DEFENSE_PLUS;
			break;
			
		case EFFECT_FAKE_OUT:
			if (gDisableStructs[bankAtk].isFirstTurn)
				viability += 8;
			break;
			
		case EFFECT_STOCKPILE:
			if (MoveEffectInMoveset(EFFECT_SWALLOW, bankAtk))
				viability += 5;
			else if (MoveEffectInMoveset(EFFECT_SPIT_UP, bankAtk))
				viability += 5;
			else
				goto AI_COSMIC_POWER;
			break;
			
		case EFFECT_HAIL:
			if (atkAbility == ABILITY_SNOWCLOAK
			|| atkAbility == ABILITY_ICEBODY
			|| atkAbility == ABILITY_FORECAST
			|| atkAbility == ABILITY_SLUSHRUSH)
				viability += 6;
			else if (FindMovePositionInMoveset(MOVE_BLIZZARD, bankAtk) != 4)
				viability += 6;
			else if (FindMovePositionInMoveset(MOVE_AURORAVEIL, bankAtk) != 4)
				viability += 6;
			else if (FindMovePositionInMoveset(MOVE_WEATHERBALL, bankAtk) != 4)
				viability += 6;
			else if (atkItemEffect == ITEM_EFFECT_ICY_ROCK)
				viability += 6;
			
			// check double battle, partner checks
			if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
			{
				if (atkPartnerAbility == ABILITY_SNOWCLOAK
				|| atkPartnerAbility == ABILITY_ICEBODY
				|| atkPartnerAbility == ABILITY_FORECAST
				|| atkPartnerAbility == ABILITY_SLUSHRUSH)		
					viability += 6;
				else if (FindMovePositionInMoveset(MOVE_BLIZZARD, bankAtkPartner) != 4)
					viability += 6;
				else if (FindMovePositionInMoveset(MOVE_AURORAVEIL, bankAtkPartner) != 4)
					viability += 6;
				else if (FindMovePositionInMoveset(MOVE_WEATHERBALL, bankAtkPartner) != 4)
					viability += 6;
				else if (atkPartnerItemEffect == ITEM_EFFECT_ICY_ROCK)
					viability += 6;
				break;
			}
			break;
			
		case EFFECT_WILL_O_WISP:
			if (defItemEffect == ITEM_EFFECT_CURE_BRN || defItemEffect == ITEM_EFFECT_CURE_STATUS)
				break;
			else if (defAbility == ABILITY_SHEDSKIN || defAbility == ABILITY_FLAREBOOST)
				break;
			else if (FindMovePositionInMoveset(MOVE_HEX, bankAtk) != 4)
				viability += 6;
			else if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
			{
				if (FindMovePositionInMoveset(MOVE_HEX, bankAtkPartner) != 4)
					viability += 6;
				break;
			}
			else if (STAT_STAGE(bankDef, STAT_STAGE_ATK) > STAT_STAGE(bankDef, STAT_STAGE_SPATK))
				viability += 6;
			else if (CanBeBurned(bankDef, TRUE))
				viability += 5;
			break;
			
		case EFFECT_FOLLOW_ME:
		case EFFECT_HELPING_HAND:
			goto AI_DOUBLEBATTLE_CHECK;
			break;
			
		case EFFECT_TAUNT:
			if (StatusMoveInMoveset(bankDef))
				viability += 6;
			break;
			
		case EFFECT_TRICK:
			if ((CanTransferItem(atkSpecies, atkItem, GetBankPartyData(bankAtk)))
			&& (CanTransferItem(defSpecies, defItem, GetBankPartyData(bankDef))))
				viability += 6;
			break;
			
		case EFFECT_INGRAIN:
			// checks related to trapping self?
			if (atkItemEffect == ITEM_EFFECT_BIG_ROOT)
				viability += 7;
			else
				viability += 6;
			break;
			
		case EFFECT_RECYCLE:
			// if saved item exists and bank not holding anything
			if ((SAVED_CONSUMED_ITEMS(bankAtk) != 0) && (atkItem == 0))			
				viability += 5;
			break;
		
		case EFFECT_BRICK_BREAK:
			if (gSideAffecting[SIDE(bankDef)] & 
			(SIDE_STATUS_REFLECT | SIDE_STATUS_LIGHTSCREEN | SIDE_STATUS_SAFEGUARD | SIDE_STATUS_MIST))
				viability += 6;
			else if (gNewBS->AuroraVeilTimers[SIDE(bankDef)] != 0)
				viability += 6;
			break;
						
		case EFFECT_REFRESH:
			if (atkStatus1 & STATUS1_ANY)
				viability += 5;
			break;
			
		case EFFECT_KNOCK_OFF:
			if (CanKnockOffItem(bankDef))
				viability += 5;
			break;
			
		case EFFECT_TICKLE:
			if (STAT_STAGE(bankDef, STAT_STAGE_ATK) < 5)
				goto AI_DEFENSE_MINUS;
			else if (STAT_STAGE(bankDef, STAT_STAGE_ATK) > STAT_STAGE(bankDef, STAT_STAGE_SPATK) && defAbility != ABILITY_CONTRARY)
				viability += 3;
			else
				goto AI_DEFENSE_MINUS;
			break;
			
		case EFFECT_COSMIC_POWER:
		AI_COSMIC_POWER: ;
			if (STAT_STAGE(bankAtk, STAT_STAGE_DEF) > 7)
				goto AI_SPDEF_PLUS;
			else if (STAT_STAGE(bankAtk, STAT_STAGE_ATK) > STAT_STAGE(bankAtk, STAT_STAGE_SPATK) && atkAbility != ABILITY_CONTRARY)
				viability += 5;
			else
				goto AI_SPDEF_PLUS;
			break;
			
		case EFFECT_BULK_UP:
			if (STAT_STAGE(bankAtk, STAT_STAGE_ATK) > 7)
				goto AI_DEFENSE_PLUS;
			else if (PhysicalMoveInMoveset(bankAtk) && atkAbility != ABILITY_CONTRARY)
				viability += 6;
			else
				goto AI_DEFENSE_PLUS;
			break;
			
		case EFFECT_CALM_MIND:
			if (move == MOVE_QUIVERDANCE)
			{
				if (MoveWouldHitFirst(move, bankAtk, bankDef))
					goto AI_CALM_MIND_CHECK;
				else if (atkAbility != ABILITY_CONTRARY)
					viability += 6;
				break;
			}
			else if (move == MOVE_GEOMANCY)
			{
				if (atkItemEffect == ITEM_EFFECT_POWER_HERB && atkAbility != ABILITY_CONTRARY)
					viability += 6;
				break;
			}
			
		AI_CALM_MIND_CHECK:
			if (STAT_STAGE(bankAtk, STAT_STAGE_SPATK) > 7)
				goto AI_SPDEF_PLUS;
			else if (SpecialMoveInMoveset(bankAtk) && atkAbility != ABILITY_CONTRARY)
				viability += 6;
			else
				goto AI_SPDEF_PLUS;
			break;
			
		case EFFECT_DRAGON_DANCE:
			if (atkAbility == ABILITY_CONTRARY)
				break;
			if (move == MOVE_SHELLSMASH)
			{
				if (atkItem == ITEM_EFFECT_POWER_HERB)
				{
					viability += 6;
					break;
				}
				else if (STAT_STAGE(bankAtk, STAT_STAGE_SPATK) <= 6 && (SpecialMoveInMoveset(bankAtk)))
				{
					viability += 6;
					break;
				}
			}
			// dragon dance
			if (STAT_STAGE(bankAtk, STAT_STAGE_ATK) > 7)
				goto AI_SPEED_PLUS;
			else if (PhysicalMoveInMoveset(bankAtk))
				viability += 6;
			goto AI_SPEED_PLUS;
			
			
		case EFFECT_STAT_SWAP_SPLT:
			switch (move)
			{
				case MOVE_GUARDSWAP:
					if (defStatus3 & STATUS3_SEMI_INVULNERABLE)
						break;
					else
					{
						if (STAT_STAGE(bankDef, STAT_STAGE_DEF) > STAT_STAGE(bankAtk, STAT_STAGE_DEF))
							viability += 5;
						else if (STAT_STAGE(bankDef, STAT_STAGE_SPDEF) > STAT_STAGE(bankAtk, STAT_STAGE_SPDEF))
							viability += 5;
						else
							break;
					}
					break;
					
				case MOVE_POWERSWAP:
					if (defStatus3 & STATUS3_SEMI_INVULNERABLE)
						break;
					else
					{
						if (STAT_STAGE(bankDef, STAT_STAGE_ATK) > STAT_STAGE(bankAtk, STAT_STAGE_ATK))
							viability += 5;
						else if (STAT_STAGE(bankDef, STAT_STAGE_SPATK) > STAT_STAGE(bankAtk, STAT_STAGE_SPATK))
							viability += 5;
						else
							break;
					}
					break;
					
				case MOVE_POWERTRICK:
					if (atkStatus3 & STATUS3_POWER_TRICK)
					{
						// power trick in effect
						break;
						
					}
					else
					{
						if ((atkDefense > atkAttack) && PhysicalMoveInMoveset(bankAtk))
							viability += 5;
						break;
					}
					break;
					
				case MOVE_HEARTSWAP:
					if (defStatus3 & STATUS3_SEMI_INVULNERABLE)
						break;
					else
					{
						for (i=0; i <= BATTLE_STATS_NO-1; ++i)
						{
							if (STAT_STAGE(bankDef, i) > STAT_STAGE(bankAtk, i))
							{
								if (i == STAT_STAGE_ATK && (PhysicalMoveInMoveset(bankAtk) || PhysicalMoveInMoveset(bankDef)))
									viability += 5;
								else if (i == STAT_STAGE_SPATK && (SpecialMoveInMoveset(bankAtk) || SpecialMoveInMoveset(bankDef)))
									viability += 5;
								else if (i == STAT_STAGE_SPEED || i == STAT_STAGE_ACC)
									viability += 5;
								break;
							}
							
						}
						
					}
					break;
					
				case MOVE_SPEEDSWAP:
					if (defStatus3 & STATUS3_SEMI_INVULNERABLE)
						break;
					else
					{
						if (defSpeed > atkSpeed)
							viability += 5;
					}
					break;
					
				case MOVE_GUARDSPLIT:
					if (defStatus3 & STATUS3_SEMI_INVULNERABLE)
						break;
					else
					{
						if (defDefense > atkDefense || defSpDef > atkSpDef)
							viability += 5;
					}
					break;
					
				case MOVE_POWERSPLIT:
					if (defStatus3 & STATUS3_SEMI_INVULNERABLE)
						break;
					else
					{
						if (defAttack > atkAttack || defSpAtk > atkSpAtk)
							viability += 5;
					}
					break;
			}
			break;
						
		case EFFECT_EAT_BERRY:
			switch (move)
			{
				case MOVE_BUGBITE:
				case MOVE_PLUCK:		
					if (defStatus2 & STATUS2_SUBSTITUTE)
						break;
					else if (defAbility == ABILITY_STICKYHOLD)
						break;
					else if (defItemPocket == POCKET_BERRY_POUCH)
						viability += 5;
					break;
					
				case MOVE_INCINERATE:
					if (atkPartnerItemPocket == POCKET_BERRY_POUCH && atkPartnerAbility != ABILITY_STICKYHOLD)
						break;	//don't destory ally item
					else if (defItemPocket == POCKET_BERRY_POUCH && defAbility != ABILITY_STICKYHOLD)
						viability += 5;
					else if (defPartnerItemPocket == POCKET_BERRY_POUCH && defPartnerAbility != ABILITY_STICKYHOLD)
						viability += 5;
					break;
			}
			break;
			
		case EFFECT_NATURAL_GIFT:
			if (atkItemPocket == POCKET_BERRY_POUCH)
			{
				for (i = 0; NaturalGiftTable[i].berry != ITEM_TABLES_TERMIN; ++i)
				{
					if (NaturalGiftTable[i].berry == atkItem)
					{
						if (IsOfType(bankDef, NaturalGiftTable[i].type))
							break;
						viability += 5;
						break;
					}
				}
			}
			break;
			
		case EFFECT_SMACK_DOWN:
			if (CheckGrounding(bankDef) == GROUNDED)
				break;
			else if (defStatus3 & STATUS3_TELEKINESIS)
				break;
			else if (defStatus3 & STATUS3_LEVITATING)
				viability += 5;	// remove magnet rise good
			break;
			
		case EFFECT_REMOVE_TARGET_STAT_CHANGES:
			// clear smog
			if (defStatus3 & STATUS3_SEMI_INVULNERABLE)
				break;
			else if (defStatus2 & STATUS2_SUBSTITUTE)
				break;
			for (i = 0; i <= BATTLE_STATS_NO-1; ++i)
			{
				if (STAT_STAGE(bankDef, i) > 6)
				{
					viability += 5;
					break;
				}
			}
			break;
			
		case EFFECT_RELIC_SONG:
			if (CanBePutToSleep(bankDef, TRUE))
				viability += 5;
			break;
			
		//case EFFECT_SET_TERRAIN:
			// handled in negative checks
		//	break;
			
		case EFFECT_PLEDGE:
			if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
			{
				if (MoveEffectInMoveset(EFFECT_PLEDGE, bankAtkPartner))
					viability += 2;	// small extra boost if partner might use pledge move
			}
			break;
			
		//case EFFECT_FIELD_EFFECTS:
			// handled in negative checks
		//	break;
			
		//case EFFECT_FLING:
			// handled in negative checks
		//	break;
			
		case EFFECT_FEINT:
			// lower chance of using protect each successive time
			if (move == MOVE_ENDURE)
			{
				if (MoveEffectInMoveset(EFFECT_FLAIL, bankAtk) && GetHealthPercentage(bankAtk) > 25)
					viability += 5;
			}
			else
			{
				if (gDisableStructs[bankAtk].protectUses > 0)
					break;
				else if (MoveWouldHitFirst(move, bankAtk, bankDef))
					break;
				else if (HasProtectionMoveInMoveset(bankDef, TRUE))
					viability += 5;
			}
			break;
		
		//case EFFECT_ATTACK_BLOCKERS:
			// handled in negative checks

			
		//case EFFECT_TYPE_CHANGES:
			//handled in negative checks
		//	break;
			
		case EFFECT_HEAL_TARGET:
			if (bankDef != bankAtkPartner)
			{
				if (move == MOVE_POLLENPUFF)
					goto FUNCTION_CHECK_RESULT;
				break;
			}
			else if ((defStatus2 & STATUS2_SUBSTITUTE) || (defStatus3 & STATUS3_SEMI_INVULNERABLE))
				break;
			else if (gNewBS->HealBlockTimers[bankDef] != 0 || gNewBS->HealBlockTimers[bankAtk] != 0)
				break;
			else
			{
				switch (move)
				{
					case MOVE_HEALPULSE:
						if (GetHealthPercentage(bankDef) <= 50)
							viability += 5;
						break;
						
					case MOVE_POLLENPUFF:
						if (defAbility == ABILITY_BULLETPROOF)
							break;
						else if (GetHealthPercentage(bankDef) <= 50)
							viability += 5;
						break;
												
					case MOVE_FLORALHEALING:
						if (gBattleTerrain == GRASSY_TERRAIN)
							viability += 6;
						else
							viability += 5;
						break;
				}
				break;
			}
			break;
			
		case EFFECT_TOPSY_TURVY_ELECTRIFY:
			// electrify handled in negative checks
			if (move == MOVE_TOPSYTURVY)
			{
				if (defStatus3 & STATUS3_SEMI_INVULNERABLE)
					break;
				u8 boostCounter = 0;
				for (i = 0; i <= BATTLE_STATS_NO-1; ++i)
				{
					if (STAT_STAGE(bankDef, i) > 6)
					{
						boostCounter++;
						if (i == STAT_STAGE_ATK && PhysicalMoveInMoveset(bankDef))
						{
							viability += 5;
							break;
						}
						else if (i == STAT_STAGE_SPATK && SpecialMoveInMoveset(bankDef))
						{
							viability += 5;
							break;
						}
						else if (i == STAT_STAGE_ACC)
						{
							viability += 5;
							break;
						}
						else if (boostCounter >= 3)
						{						
							// extra viability if a bunch of stats are heightened regardless of type ?
							viability += 5;
							break;
						}
					}
				}
			}
			break;
			
		case EFFECT_FAIRY_LOCK_HAPPY_HOUR:
			// happy hour no benefit
			if (move == MOVE_FAIRYLOCK)
			{
				goto AI_TRAP_TARGET_CHECK;
			}
			break;
			
		//case EFFECT_INSTRUCT_AFTER_YOU_QUASH:
			// handled in negative checks
		//	break;
			
		case EFFECT_TEAM_EFFECTS:
			switch (move)
			{
				case MOVE_TAILWIND:
					if (gNewBS->TailwindTimers[SIDE(bankAtk)] != 0)
						break;
					viability += 5;	// double speed is pretty sweet
					break;
					
				case MOVE_LUCKYCHANT:
					if (defAbility == ABILITY_SNIPER || defAbility == ABILITY_SUPERLUCK || (defStatus2 & STATUS2_FOCUS_ENERGY))
						viability += 5;
					else if (defPartnerAbility == ABILITY_SNIPER || defPartnerAbility == ABILITY_SUPERLUCK || (defPartnerStatus2 & STATUS2_FOCUS_ENERGY))
						viability += 5;
					break;
					
				case MOVE_MAGNETRISE:
					if (gNewBS->GravityTimer != 0)
						break;
					else if (atkItemEffect == ITEM_EFFECT_IRON_BALL)
						break;
					else if (atkStatus3 & (STATUS3_SMACKED_DOWN | STATUS3_ROOTED))
						break;
					else if (MoveEffectInMoveset(EFFECT_SMACK_DOWN, bankDef) || MoveEffectInMoveset(EFFECT_SMACK_DOWN, bankDefPartner))
						break;
					else if (gSideAffecting[SIDE(bankAtk)] & SIDE_STATUS_SPIKES)
						viability += 6;
					else
						viability += 4;	// raised up pretty cool
			}
			break;
			
		//case EFFECT_CAMOUFLAGE:
		//	break;
			
		//case EFFECT_FLAMEBURST:
		//	break;
			
		//case EFFECT_LASTRESORT_SKYDROP:
		//	break;
			
					
	} // move effect switch
	FUNCTION_CHECK_RESULT: ;
	if (moveSplit != SPLIT_STATUS)
	{
		if (TypeCalc(move, bankAtk, bankDef, 0, FALSE) & (MOVE_RESULT_SUPER_EFFECTIVE))
			viability += 3;
		else if (IsStrongestMove(move, bankAtk, bankDef))
			viability += 1;
	}
	FUNCTION_RETURN:
	return viability;
}

*/