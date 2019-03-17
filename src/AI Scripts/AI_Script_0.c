//Add ,move to NO_MOLD_BREAKERS

/*

#include "..\\defines.h"
#include "..\\AI_Helper_Functions.h"
#include "..\\Helper_Functions.h"

enum {IN_AIR, GROUNDED};

u8 AI_Script_Negatives(u8 bankAtk, u8 bankDef, u16 move) {
RESTART_AI_SCRIPT_0:
	u8 viability = 100;
	u8 decreased;
	// int i;
	
	u8 bankAtkPartner = PARTNER(bankAtk);
	u8 bankDefPartner = PARTNER(bankDef);
	u16 atkSpecies = SPECIES(bankAtk);
	u16 defSpecies = SPECIES(bankDef);
	u8 atkAbility = ABILITY(bankAtk);
	u8 atkPartnerAbility = ABILITY(bankAtkPartner);
	u8 defPartnerAbility = ABILITY(bankDefPartner);
	u8 defAbility = ABILITY(bankDef);
	u8 atkEffect = ITEM_EFFECT(bankAtk);
	u8 defEffect = ITEM_EFFECT(bankDef);
	u8 atkQuality = ITEM_QUALITY(bankAtk);
	u8 defQuality = ITEM_QUALITY(bankDef);
	u32 atkStatus1 = gBattleMons[bankAtk].status1;
	u32 defStatus1 = gBattleMons[bankDef].status1;
	u32 atkStatus2 = gBattleMons[bankAtk].status2;
	u32 defStatus2 = gBattleMons[bankDef].status2;
	u32 atkStatus3 = gStatuses3[bankAtk];
	u32 defStatus3 = gStatuses3[bankDef];
	
	//u8 moveEffect = MOVE_EFFECT(move);
	u8 moveEffect = gBattleMoves[move].effect;
	u8 moveSplit = SPLIT(move);
	//u8 moveRange = RANGE(move);
	u8 moveTarget = gBattleMoves[move].target;
	u8 moveType = GetMoveTypeSpecial(bankAtk, move);
	//u8 moveFlags = MOVE_FLAGS(move);
	u8 moveFlags = gBattleMoves[move].flags;
	//u8 moveAcc = MOVE_ACC(move); //Do something with stat buffs and Unaware/Keen Eye
	u8 moveAcc = gBattleMoves[move].accuracy;
	
	//Affects User Check
	//if (moveRange & AFFECTS_USER || moveRange & AFFECTS_FIELD)
	if (moveTarget & MOVE_TARGET_USER || moveTarget & MOVE_TARGET_ALL)
		goto MOVESCR_CHECK_0;
	
	//Ranged Move Check
	//if (moveRange & HIT_ALL) {
	if (moveTarget & MOVE_TARGET_FOES_AND_ALLY) {
		//if (moveType == TYPE_ELECTRIC && ABILITY_OTHER_SIDE(bankAtk, ABILITY_LIGHTNINGROD))
		if (moveType == TYPE_ELECTRIC && (ABILITY(SIDE(bankAtk) ^ BIT_SIDE) == ABILITY_LIGHTNINGROD))
			return (viability - 20);
		//else if (moveTYPE == TYPE_WATER && ABILITY_OTHER_SIDE(bankAtk, ABILITY_STORMDRAIN))
		else if (moveType == TYPE_WATER && (ABILITY(SIDE(bankAtk) ^ BIT_SIDE) == ABILITY_STORMDRAIN))
			return (viability - 20);
	}
	
	#ifdef AI_TRY_TO_KILL
		if (DamagingMoveInMoveset(bankAtk)) {
			if (MoveKnocksOut(move, bankAtk, bankDef)
			&& MoveWouldHitFirst(move, bankAtk, bankDef))
				viability += 7;

			else if (IsStrongestMove(move, bankAtk, bankDef))
				viability += 2;
		}
	#endif
	// Gravity Table Prevention Check
	if ((gNewBS->GravityTimer > 0) && CheckMoveInTable(move, GravityBanTable))
		return 0; //Can't select this move period

	// Ungrounded check (need air balloon in CheckGrounding)
	if ((CheckGrounding(bankDef) == IN_AIR) && (moveType==TYPE_GROUND)) {
		return viability = 0;
	}
		
	// Powder Move Checks (safety goggles, defender has grass type, overcoat, and powder move table)
	if ((defEffect == ITEM_EFFECT_SAFETY_GOGGLES || IsOfType(bankDef, TYPE_GRASS || defAbility == ABILITY_OVERCOAT))
	&& CheckMoveInTable(move, PowderTable))
		viability -= 10; //No return b/c could be reduced further by absorb abilities
		
	//Target Ability Checks
	if (NO_MOLD_BREAKERS(bankAtk) || !CheckMoveInTable(move, MoldBreakerMovesTable)) {
		switch (defAbility) {
			
			// type-specific ability checks - primordial weather handled separately
			// Electric
			case ABILITY_VOLTABSORB:
				if ((moveType == TYPE_ELECTRIC)) // && (moveSplit != SPLIT_STATUS))
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
			
			case ABILITY_WATERCOMPACTION:
				if (moveType == TYPE_WATER)		// defense-related condition?
					return viability - 10;
				break;
			
			// Fire			
			case ABILITY_WATERBUBBLE:
				if ((moveType == TYPE_FIRE)) // && (moveSplit != SPLIT_STATUS))
					return viability - 20;
				break;
				
			case ABILITY_FLASHFIRE:
				if (moveType == TYPE_FIRE)
					return viability - 20;
				break;
				
			case ABILITY_HEATPROOF:
				if ((moveType == TYPE_FIRE)) // && (moveSplit != SPLIT_STATUS))
					return viability - 10;
				break;
			
			// Grass
			case ABILITY_SAPSIPPER:
				if (moveType == TYPE_GRASS)
					return viability - 20;
				break;
				
			// Dark
			case ABILITY_JUSTIFIED:
				if ((moveType == TYPE_DARK)) // && (moveSplit != SPLIT_STATUS))
					return viability - 10;
				break;
				
				
			// multiple move types
			case ABILITY_THICKFAT:
				if (moveType == TYPE_FIRE || moveType == TYPE_ICE)
					return viability - 10;
				break;
				
			case ABILITY_RATTLED:
				if ((moveSplit != TYPE_STATUS) && 
					(moveType == TYPE_DARK || moveType == TYPE_GHOST || moveType == TYPE_BUG))
					return viability - 10;
				break;
			
			// move category checks
			case ABILITY_SOUNDPROOF:
				if (CheckSoundMove(move))
					return viability - 10;
				break;
			
			case ABILITY_BULLETPROOF:
				if (CheckMoveInTable(move, BombTable))
					return viability - 10;
				break;
			
			case ABILITY_DAZZLING:
			case ABILITY_QUEENLYMAJESTY:
				if (CheckMovePriority(move, bankAtk) > 0) //Check if right num
					return viability - 10;
				break;
			
			case ABILITY_AROMAVEIL:
				if (CheckMoveInTable(move, AromaVeilTable))
					return viability - 10;
				break;
			
			case ABILITY_SWEETVEIL:
				if (moveEffect == EFFECT_SLEEP || moveEffect == EFFECT_SET_YAWN)
					return viability - 10;
				break;
			
			case ABILITY_FLOWERVEIL:
				if (IsOfType(bankDef, TYPE_GRASS)
				&& (CheckTableForMoveEffect(move, SetStatusTable) || CheckTableForMoveEffect(move, StatLowerTable))
					return viability - 10;
				break;
			
			case ABILITY_MAGICBOUNCE:
				if (moveFlags & FLAG_REFLECTED_BY_MAGIC_COAT)
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
				if (moveEffect == MOVE_EFFECT_ATK_MINUS_1
				||  moveEffect == MOVE_EFFECT_ATK_MINUS_2)
					return (viability - 10);
				break;
			
			case ABILITY_KEENEYE:
				if (moveEffect == MOVE_EFFECT_ACC_MINUS_1
				||  moveEffect == MOVE_EFFECT_ACC_MINUS_2)
					return (viability - 10);
				break;
			
			case ABILITY_BIGPECKS:
				if (moveEffect == MOVE_EFFECT_DEF_MINUS_1
				||  moveEffect == MOVE_EFFECT_DEF_MINUS_2)
					return (viability - 10);
				break;
			
			case ABILITY_DEFIANT:
			case ABILITY_COMPETITIVE:
				if (CheckTableForMoveEffect(move, StatLowerTable))
					return (viability - 8); //Not -10 b/c move still works, just not recommended
				break;
			
			case ABILITY_COMATOSE:
				if (CheckTableForMoveEffect(move, SetStatusTable))
					return (viability - 10);
				break;
			
			case ABILITY_WONDERSKIN:
				if (moveSplit == SPLIT_STATUS) {
					moveAcc = 50;
					return (viability - 8);
				}
				break;
				
			case ABILITY_LEAFGUARD:
				if (WEATHER_HAS_EFFECT && (gBattleWeather & WEATHER_SUN_ANY)
				&& CheckTableForMoveEffect(move, SetStatusTable))
					return (viability - 10);
		}
		
		//Target Partner Ability Check
		if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE) {
			switch (defPartnerAbility) {
				case ABILITY_LIGHTNINGROD:
					if (moveType == TYPE_ELECTRIC)
						return (viability - 20);
					break;
				
				case ABILITY_STORMDRAIN:
					if (moveType == TYPE_WATER)
						return (viability - 20);
					break;
				
				case ABILITY_SWEETVEIL:
					if (moveEffect == EFFECT_SLEEP || moveEffect == EFFECT_SET_YAWN)
						return (viability - 10);
					break;
				
				case ABILITY_FLOWERVEIL:
					if ((IsOfType(bankDef, TYPE_GRASS))
					&& (CheckTableForMoveEffect(move, SetStatusTable) || CheckTableForMoveEffect(move, StatLowerTable)))
						return (viability - 10);
					break;
				
				case ABILITY_AROMAVEIL:
					if (CheckMoveInTable(move, AromaVeilTable))
						return (viability - 10);
					break;
				
				case ABILITY_MAGICBOUNCE:
					if ((moveFlags & FLAG_REFLECTED_BY_MAGIC_COAT)
					&& (moveRange & AFFECTS_TARGET_SIDE))
						return (viability - 20);
			} // switch
		} // if double battle
	}
	
	//Terrain Check
	if (CheckGrounding(bankDef) == GROUNDED) {
		switch (TerrainType) {
			case ELECTRIC_TERRAIN:
				if (moveEffect == EFFECT_SLEEP || moveEffect == EFFECT_SET_YAWN)
					return (viability - 10);
				break;
			
			case GRASSY_TERRAIN:
				if ((move == MOVE_BULLDOZE) || (move == MOVE_EARTHQUAKE) || (move == MOVE_MAGNITUDE))
					return (viability - 10);
				break;
			
			case MISTY_TERRAIN:
				if (CheckTableForMoveEffect(move, SetStatusTable) || CheckTableForMoveEffect(move, ConfusionTable))
					return (viability - 10);
				break;
				
			case PSYCHIC_TERRAIN:
				if (CheckMovePriority(move, bankAtk) > 0) //Check if right num
					return (viability - 10);
				break;
		}
	}
		
	//Minior Check
	if (defSpecies == PKMN_MINIORSHIELD
	&& CheckTableForMoveEffect(move, SetStatusTable))
		return viability - 10;
		
	//Throat Chop Check
	if (gNewBS->ThroatChopTimers[bankAtk] && CheckSoundMove(move))
		return 0; //Can't select this move period
			
	//Heal Block Check
	if (gNewBS->HealBlockTimers[bankAtk] && CheckHealingMove(move))
		return 0; //Can't select this move period
		
	//Primal Weather Check
	if (gBattleWeather & WEATHER_SUN_PRIMAL && moveType == TYPE_WATER && !(moveSplit == SPLIT_STATUS))
		return viability - 20;
	else if (gBattleWeather & WEATHER_RAIN_PRIMAL && moveType == TYPE_FIRE && !(moveSplit == SPLIT_STATUS))
		return viability - 20;
	
	// Check Move Effects
	MOVESCR_CHECK_0:
	switch (moveEffect) {
		case EFFECT_SLEEP:
		AI_CHECK_SLEEP:
			if (!CanBePutToSleep(bankDef, bankAtk)
			|| (!CanBeGeneralStatused(bankDef))
			|| (MoveBlockedBySubstitute(move, bankAtk, bankDef)))
				viability -= 10;
			break;
			
		case EFFECT_ABSORB:
			if (abilityDef == ABILITY_LIQUIDOOZE)
				viability -= 6;
			if (move == MOVE_STRENGTHSAP && gBattleMons[bankDef].statstages[STAT_STAGE_ATK-1] == 0)
				viability -= 10;
			break;
			
		case EFFECT_EXPLOSION:
		#ifdef OKAY_WITH_AI_SUICIDE
			if (NO_MOLD_BREAKERS(bankAtk) && ABILITY_PRESENT(ABILITY_DAMP))
				viability -= 10;
			else if (!(ViableMonCountFromBank(bankAtk) == 1) //If the AI has one PKMN left,
			&& (ViableMonCountFromBank(bankDef) == 1)		 //and the Target only has one PKMN left,
			&& (MoveKnocksOut(move, bankAtk, bankDef)))		 //and the AI can knock out the target,
				viability -= 4;								 //then the AI can use Explosion to win the battle
		#else
			viability -= 10;
		#endif
			break;
		
		case EFFECT_DREAM_EATER:
			if (!(defAbility == ABILITY_COMATOSE || defStatus1 & STATUS_SLEEP))
				viability -= 10;
			break;
			
		case EFFECT_MIRROR_MOVE: //May cause issues with priority calcs?
			if (lastTakenMoveFrom[bankAtk][bankDef]) {
				move = lastTakenMoveFrom[bankAtk][bankDef];
				goto RESTART_AI_SCRIPT_0;
			}
		
		case EFFECT_SPLASH:
		case EFFECT_TELEPORT:
			viability -= 10;
			break;
		
		case EFFECT_ATTACK_UP:
		case EFFECT_ATTACK_UP_2:
			if (StatMaxed(bankAtk, STAT_STAGE_ATK))
				viability -= 10;
			break;
		
		case EFFECT_DEFENSE_UP:
		case EFFECT_DEFENSE_UP_2:
		case EFFECT_DEFENSE_CURL:
			switch (move) {
				case MOVE_FLOWERSHIELD:
					if (!IsOfType(bankAtk, TYPE_GRASS)
					&&  !(gBattleTypeFlags & BATTLE_TYPE_DOUBLE && IsOfType(bankAtkPartner), TYPE_GRASS))
						viability -= 10;
					break;
				
				case MOVE_MAGNETIC_FLUX:
					if (abilityAtk == ABILITY_PLUS || ability_Atk == ABILITY_MINUS) {
						if (StatMaxed(bankAtk, STAT_STAGE_DEF) && StatMaxed(bankAtk, STAT_STAGE_SPDEF))
							viability -= 10;
					}
					else if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE
					&& (defPartnerAbility == ABILITY_PLUS || defPartnerAbility == ABILITY_MINUS)) {
						if (StatMaxed(bankAtkPartner, STAT_STAGE_DEF) && StatMaxed(bankAtkPartner, STAT_STAGE_SPDEF))
							viability -= 10;
					}
					break;
				
				case MOVE_AROMATIC_MIST:
					if (!(gBattleTypeFlags & BATTLE_TYPE_DOUBLE) || StatMaxed(bankAtkPartner, STAT_STAGE_SPDEF))
						viability -= 10;
					break;
				
				default:
					if (StatMaxed(bankAtk, STAT_STAGE_DEF))
						viability -= 10;
			}
			break;
		
		case EFFECT_SPEED_UP:
		case EFFECT_SPEED_UP_2:
			if (StatMaxed(bankAtk, STAT_STAGE_SPD))
				viability -= 10;
			break;
		
		case EFFECT_SPECIAL_ATTACK_UP: 
		case EFFECT_SPECIAL_ATTACK_UP_2:			
			switch(move) {
				case MOVE_WORKUP:
					if (StatMaxed(bankAtk, STAT_STAGE_ATK) 
					&&  StatMaxed(bankAtk, STAT_STAGE_SPATK))
						viability -= 10;
					break;
				
				case MOVE_ROTOTILLER:
					if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE) {
						if (!(IsOfType(bankAtk, TYPE_GRASS) && CheckGrounding(bankAtk))
						&&  !(IsOfType(bankAtkPartner, TYPE_GRASS) && CheckGrounding(bankAtkPartner)))
							viability -= 10;
					}
					else if (!(IsOfType(bankAtk, TYPE_GRASS) && CheckGrounding(bankAtk)))
						viability -= 10;
					break;
					
				case MOVE_GEARUP:
					if (abilityAtk == ABILITY_PLUS || ability_Atk == ABILITY_MINUS) {
						if (StatMaxed(bankAtk, STAT_STAGE_ATK) && StatMaxed(bankAtk, STAT_STAGE_SPATK))
							viability -= 10;
					}
					else if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE
					&& (defPartnerAbility == ABILITY_PLUS || defPartnerAbility == ABILITY_MINUS)) {
						if (StatMaxed(bankAtkPartner, STAT_STAGE_ATK) && StatMaxed(bankAtkPartner, STAT_STAGE_SPATK))
							viability -= 10;
					}
					break;
				
				default:
					if (StatMaxed(bankAtk, STAT_STAGE_SPATK))
						viability -= 10;
			}
			break;
			
		case EFFECT_SPECIAL_DEFENSE_UP:
		case EFFECT_SPECIAL_DEFENSE_UP_2:
		AI_SPDEF_RAISE_1:
			if (StatMaxed(bankAtk, STAT_STAGE_SPDEF))
				viability -= 10;
			break;
		
		case EFFECT_ACCURACY_UP:
		case EFFECT_ACCURACY_UP_2:
			if (StatMaxed(bankAtk, STAT_STAGE_ACC))
				viability -= 10;
			break;
		
		case EFFECT_EVASION_UP:
		case EFFECT_EVASION_UP_2:
		case EFFECT_MINIMIZE:
			switch (move) {
				case MOVE_ACUPRESSURE:
					if (StatsMaxed(bankDef))
						viability -= 10;
					break;
					
				default:
					if (StatMaxed(bankAtk, STAT_STAGE_EVSN))
						viability -= 10;
			}
			break;
		
		case EFFECT_ATTACK_DOWN:
		case EFFECT_ATTACK_DOWN_2:
			decreased = FALSE;
			switch (move) {
				case MOVE_VENOMDRENCH:
					if (!(defStatus1 & STATUS_PSN_ANY)) {
						viability -= 10;
						decreased = TRUE;
						break;
					}
					else if (!StatMinned(bankDef, STAT_STAGE_SPD))
						break;
					else
						break;
				case MOVE_PLAYNICE:
				case MOVE_NOBLEROAR:
				case MOVE_TEARFULOOK:
					if (StatMinned(bankDef, STAT_STAGE_ATK)
					&&  StatMinned(bankDef, STAT_STAGE_SPATK)) {
						viability -= 10;
						decreased = TRUE;
					}
					break;
				
				default:
					if (StatMinned(bankDef, STAT_STAGE_ATK)) {
						viability -= 10;
						decreased = TRUE;
					}
			}
			if (decreased)
				break;
			goto AI_STANDARD_DAMAGE;
			
		case EFFECT_DEFENSE_DOWN: 
		case EFFECT_DEFENSE_DOWN_2: 
			if (StatMinned(bankDef, STAT_STAGE_DEF))
				viability -= 10;
			else if (MoveBlockedBySubstitute(move, bankAtk, bankDef))
				viability -= 10;
			break;
		
		case EFFECT_SPEED_DOWN:
		case EFFECT_SPEED_DOWN_2: 
			if (StatMinned(bankDef, STAT_STAGE_SPD))
				viability -= 10;
			else if (MoveBlockedBySubstitute(move, bankAtk, bankDef))
				viability -= 10;
			break;
		
		case EFFECT_SPECIAL_ATTACK_DOWN: 
		case EFFECT_SPECIAL_ATTACK_DOWN_2:
			if (StatMinned(bankDef, STAT_STAGE_SPATK))
				viability -= 10;
			else if ((move == MOVE_CAPTIVATE)
			&& (gBattleMons[bankAtk].gender == GENDER_UNKNOWN)
			||  (gBattleMons[bankDef].gender == GENDER_UNKNOWN)
			||  (gBattleMons[bankAtk].gender == gBattleMons[bankDef].gender))
				viability -= 10;
			else if (MoveBlockedBySubstitute(move, bankAtk, bankDef))
				viability -= 10;
			break;
		
		case EFFECT_SPECIAL_DEFENSE_DOWN:
		case EFFECT_SPECIAL_DEFENSE_DOWN_2:
			if (StatMinned(bankDef, STAT_STAGE_SPDEF))
				viability -= 10;
			else if (MoveBlockedBySubstitute(move, bankAtk, bankDef))
				viability -= 10;
			break;
		
		case EFFECT_ACCURACY_DOWN: 
		case EFFECT_ACCURACY_DOWN_2:
			if (StatMinned(bankDef, STAT_STAGE_ACC))
				viability -= 10;
			else if (MoveBlockedBySubstitute(move, bankAtk, bankDef))
				viability -= 10;
			break;
		
		case EFFECT_EVASION_DOWN: 
		case EFFECT_EVASION_DOWN_2: 
			if (StatMinned(bankDef, STAT_STAGE_EVSN))
				viability -= 10;
			else if (MoveBlockedBySubstitute(move, bankAtk, bankDef))
				viability -= 10;
			break;
		
		case EFFECT_HAZE:
		AI_HAZE_CHECK:
			decreased = FALSE;
			for (i = 0; i <= BATTLE_STATS_NO - 1; ++i) {
				if (gBattleMons[bankAtk].statStages[i] > 6) {
					viability -= 10;
					decreased = TRUE;
					break;
				}
			}
			if (decreased) 
				break;
			
			for (i = 0; i <= BATTLE_STATS_NO - 1; ++i) {
				if (gBattleMons[bankDef].statStages[i] < 6) {
					viability -= 10;
					decreased = TRUE;
					break;
				}
			}
			break;
			
		case EFFECT_ROAR:
			switch (move) {
				case MOVE_DRAGONTAIL:
				case MOVE_CIRCLETHROW:
					goto AI_STANDARD_DAMAGE;
				
				default:
					if ((ViableMonCountFromBank(bankDef) <= 1)
					||  defAbility == ABILITY_SUCTIONCUPS)
						viability -= 10;
			}
			break;
			
		case EFFECT_CONVERSION:
			if (IsOfType(bankAtk, gBattleMoves[gBattleMons[bankAtk].move1].type))
				viability -= 10;
			break;
		
		case EFFECT_RESTORE_HP:
		case EFFECT_REST:
		case EFFECT_MORNING_SUN:
		case EFFECT_SYNTHESIS:
		case EFFECT_MOONLIGHT:
		case EFFECT_SOFTBOILED:
		AI_RECOVERY:
			switch (move) {
				case MOVE_PURIFY:
					if (!(defStatus1 & STATUS_ANY)
					|| MoveBlockedBySubstitute(move, bankAtk, bankDef)
					|| GetHealthPercentage(bankAtk) == 100) {
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
			switch (move) {
				case MOVE_TOXICTHREAD:
					if (!StatMinned(bankDef, STAT_STAGE_SPD)
					&& !MoveBlockedBySubstitute(move, bankAtk, bankDef))
						break;
				default:
					If (!CanBePoisoned(bankDef, bankAtk)
					|| MoveBlockedBySubstitute(move, bankAtk, bankDef))
						viability -= 10;
			}
			break;
		
		case EFFECT_LIGHT_SCREEN:
			if (gSideStatuses[bankAtk & 1] & SIDE_STATUS_LIGHT_SCREEN)
				viability -= 10;
			break;
		
		case EFFECT_0HKO:
			if (TypeCalc(move, bankAtk, bankDef, 0, FALSE) & (MOVE_RESULT_NO_EFFECT | MOVE_RESULT_MISSED)
			|| (NO_MOLD_BREAKERS(bankAtk) && defAbility == ABILITY_STURDY))
				viability -= 10;
			break;
		
		case EFFECT_RECOIL_IF_MISS:
			if (atkAbility == ABILITY_MAGICGUARD)
				goto AI_STANDARD_DAMAGE;
			else if (moveAcc < 75)
				viability -= 6;
			break;
				
		case EFFECT_MIST:
			if (gSideStatuses[bankAtk & 1] & SIDE_STATUS_MIST)
				viability -= 10;
			break;
		
		case EFFECT_RECOIL:
			if (atkAbility == ABILITY_MAGICGUARD || atkAbility == ABILITY_ROCKHEAD)
				goto AI_STANDARD_DAMAGE;
			else if (GetHealthPercentage(bankAtk) < 10)
				viability -= 4;
			break;
			
		case EFFECT_FOCUS_ENERGY:
			if (defStatus2 & STATUS2_FOCUS_ENERGY)
				viability -= 10;
			break;
		
		case EFFECT_CONFUSE:
		AI_CONFUSE:
			if (defStatus2 & STATUS2_CONFUSE 
			|| (NO_MOLD_BREAKERS(bankAtk) && defAbility == ABILITY_OWNTEMPO))
				viability -= 10;
			else if (defEffect == ITEM_EFFECT_MENTAL_HERB)
				viability -= 6;
			break;

		case EFFECT_TRANSFORM:
			if (atkStatus2 & STATUS2_TRANSFORMED
			||  defStatus2 & (STATUS2_TRANSFORMED | STATUS2_SUBSTITUTE))
				viability -= 10;
			
		case EFFECT_REFLECT:
			switch (move) {
				case MOVE_AURORAVEIL:
					if (gNewBS->AuroraVeilTimers[bankAtk & 1]
					|| !(gBattleWeather & WEATHER_HAIL_ANY))
						viability -= 10;
					break;
				
				default:
					if (gSideStatuses[bankAtk & 1] & SIDE_STATUS_REFLECT)
						viability -= 10;
			}
			break;
			
		//case EFFECT_PARALYZE_HIT:
		case EFFECT_PARALYZE:
		AI_PARALYZE_CHECK:
			if (!CanBeParalyzed(bankDef, bankAtk)
			|| MoveBlockedBySubstitute(move, bankAtk, bankDef))
				viability -= 10;
			else if (move == MOVE_THUNDERWAVE
				&& TypeCalc(move, bankAtk, bankDef, 0, FALSE) & (RESULT_NO_EFFECT | RESULT_MISSED))
					viability -= 10;
			break;
			
		case EFFECT_SUBSTITUTE:
			if (defStatus2 & STATUS2_SUBSTITUTE
			|| GetHealthPercentage(bankAtk) <= 25)
				viability -= 10;
			else if (ABILITY(GetFoeBank(bankAtk)) == ABILITY_INFILTRATOR 
			|| (gBattleTypeFlags & BATTLE_TYPE_DOUBLE && ABILITY(PARTNER(GetFoeBank(bankAtk))) == ABILITY_INFILTRATOR))
				viability -= 8;
			break;
		
		case EFFECT_SPITE:
		case EFFECT_MIMIC:
			if (gNewBS->LastUsedMove[bankDef] == 0)
				viabilility -= 10;
			break;
		
		case EFFECT_METRONOME:
			break;
		
		case EFFECT_LEECH_SEED:
			switch (move) {
				case MOVE_AQUARING:
					if (atkStatus3 & STATUS3_AQUA_RING)
						viability -= 10;
					break;
				
				default: //Leech Seed
					if (IsOfType(bankDef, TYPE_GRASS)
					|| (defStatus3 & STATUS3_SEEDED)
					|| (defAbility == ABILITY_LIQUIDOOZE))
						break;
			}
			break;
		
		case EFFECT_DISABLE:
			if (gDisableStructs[bankDef].disableTimer1)
				viability -= 10;			
			if (defEffect == ITEM_EFFECT_MENTAL_HERB)
				viability -= 6;
			break;
			
		case EFFECT_COUNTER:
			switch (move) {
				case MOVE_METALBURST:
					if (MoveWouldHitFirst(move, bankAtk, bankDef)
					|| !DamagingMoveInMoveset(bankDef))
						viability -= 10;
					break;
				
				default: //Counter
					if (!PhysicalMoveInMoveset(bankDef)
					|| GetHealthPercentage(bankAtk) < 20)
						viability -= 10;
			}
			break;
		
		case EFFECT_ENCORE:
			//Check Encore Timer
			if (gDisableStructs[gActiveBattler].encoreTimer != 0)
				viability -= 10;
			break;
			
			//Check Last Used Move
			if (defEffect == ITEM_EFFECT_MENTAL_HERB)
				viability -= 6;
			break;
		
		case EFFECT_ENDEAVOR:
		case EFFECT_PAIN_SPLIT:
			if (gBattleMons[bankAtk].hp > (gBattleMons[bankAtk].hp + gBattleMons[bankDef].hp)/2)
				viability -= 10;
			break;
		
		case EFFECT_SNORE:
		case EFFECT_SLEEP_TALK:
			if (!(defStatus1 & STATUS_SLEEP) && defAbility != ABILITY_COMATOSE)
				viability -= 10;
			break;
		
		case EFFECT_CONVERSION_2:
			if (gNewBS->LastUsedTypes[bankDef] == 0)
				viabilility -= 10;
			break;
			
		case EFFECT_LOCK_ON:
			switch (move) {
				case MOVE_LASERFOCUS:
					if (gNewBS->LaserFocusTimers[bankAtk])
						viability -= 10;
					else if (defAbility == ABILITY_SHELLARMOR || defAbility == ABILITY_BATTLEARMOR)
						viability -= 8;
					break;
				
				default:	// lock on
					if (atkStatus3 & STATUS3_LOCKED_ON
					|| atkAbility == ABILITY_NOGUARD
					|| defAbility == ABILITY_NOGUARD)
						viability -= 10;
					break;
			}
			break;
		
		case EFFECT_DESTINY_BOND:
			if (gLastUsedMoves[bankAtk] == move
			|| atkStatus2 & STATUS2_DESTINY_BOND)
				viability -= 10;
			break;
		
		case EFFECT_FALSE_SWIPE:
			if (MoveKnocksOut(move, bankAtk, bankDef))
				viability -= 10;
			break;
		
		case EFFECT_HEAL_BELL:
			if (!PartyMemberStatused(bankAtk))
				viability -= 10;
			break;
			
		case EFFECT_MEAN_LOOK:
			switch (move) {
				case MOVE_SPIRITSHACKLE:
				case MOVE_ANCHORSHOT:
					goto AI_STANDARD_DAMAGE;
					
				default: // mean look
					if (defStatus2 & 0x400E000 
					|| defEffect == ITEM_EFFECT_SHED_SHELL
					|| IsOfType(bankDef, TYPE_GHOST)
					|| atkAbility == ABILITY_SHADOWTAG
					|| (atkAbility == ABILITY_ARENATRAP && CheckGrounding(bankDef))
					|| (atkAbility == ABILITY_MAGNETPULL && IsOfType(bankDef, TYPE_STEEL)))
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
			if (IsOfType(bankAtk, TYPE_GHOST)) {
				if (defStatus2 & STATUS2_CURSE)
					viability -= 10;
				else if (GetHealthPercentage(bankAtk) <= 50)
					viability -= 6;
			}
			else if (atkAbility == ABILITY_CONTRARY || (StatMaxed(bankAtk, STAT_STAGE_ATK) && StatMaxed(bankAtk, STAT_STAGE_DEF)))
				viability -= 10;
			break;
			
		case EFFECT_PROTECT:
			decreased = FALSE;
			switch (move) {
				case MOVE_QUICKGUARD:
				case MOVE_WIDEGUARD:
				case MOVE_CRAFTYSHIELD:
					if (!(gBattleTypeFlags & BATTLE_TYPE_DOUBLE)) {
						viability -= 10;
						decreased = TRUE;
					}
					break;
				
				case MOVE_MATBLOCK:
					if (!gDisableStructs[gBankAttacker].isFirstTurn) {
						viability -= 10;
						decreased = TRUE;
					}
			}
			if (decreased)
				break;
			
			if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE) {
				if (gBattleMons[GetFoeBank(bankAtk)].status2 & STATUS2_RECHARGE
				&&  gBattleMons[PARTNER(GetFoeBank(bankAtk))].status2 & STATUS2_RECHARGE) {
					viability -= 10;
					break;
				}
			}
			else { //Single Battle
				if (gBattleMons[GetFoeBank(bankAtk)].status2 & STATUS2_RECHARGE) {
					viability -= 10;
					break;
				}
			}
			
			if ((GetProtectActivity(bankAtk) > 0) && (umodsi(Random(), 100) < 50))
				viability -= 6;
			break;
			
		//Fix up!
		case EFFECT_SPIKES:
			if (ViableMonCountFromBank(bankDef) <= 1) {
				viability -= 10;
				break;
			}
			switch (move) {
				case MOVE_STEALTHROCK:
					if (something[bankDef & 1].SRLay)
						viability -= 10;
					break;
				
				case MOVE_TOXICSPIKES:
					if (something[bankDef & 1].tsLayer >= 2)
						viability -= 10;
					break;
					
				case MOVE_STICKWEB:
					if (something[bankDef & 1].SWLay)
						viability -= 10;
					break;
				
				default: //Spikes
					if (something[bankDef & 1].spikesLayer >= 3)
						viability -= 10;
					break;
			}
			break;
			
		case EFFECT_FORESIGHT:
			switch (move) {
				case MOVE_MIRACLEEYE:
					if (defStatus3 & STATUS3_MIRACLE_EYED)
						viability -= 10;
					
					if (gBattleMons[bankDef].statstages[STAT_EVASION] <= 6
					&& !IsOfType(bankDef, TYPE_DARK))
						viability -= 9;
					break;
				
				default: //Foresight
					if (defStatus2 & STATUS2_FORESIGHT)
						viability -= 10;
					
					if (gBattleMons[bankDef].statstages[STAT_EVASION] <= 6
					&& !IsOfType(bankDef, TYPE_GHOST))
						viability -= 9;
			}
			break;
			
		case EFFECT_PERISH_SONG:
			if ((gStatuses3[GetFoeBank(bankAtk)] & STATUS3_PERISH_SONG)
			&& !(gBattleTypeFlags & BATTLE_TYPE_DOUBLE && !(gStatuses3[PARTNER(GetFoeBank(bankAtk))] & STATUS3_PERISH_SONG)))
				viability -= 10;
			break;
			
		case EFFECT_SANDSTORM:
			if (gBattleWeather & (WEATHER_SANDSTORM_ANY | WEATHER_PRIMAL))
				viability -= 10;
			break;
		
		case EFFECT_ENDURE:
			if (gBattleMons[bankAtk].hp == 1)
				viability -= 10;
			else if ((GetProtectActivity(bankAtk) > 0) && (umodsi(Random(), 100) < 50))
				viability -= 6;
			break;
			
		case EFFECT_SWAGGER:
			if (bankDef == bankAtkPartner)
			&& StatMaxed(bankDef, STAT_STAGE_ATK))
				viability -= 10;
			else
				goto AI_CONFUSE;
			break;
		
		case EFFECT_ATTRACT: 
			if (defAbility == ABILITY_OBLIVIOUS || (defStatus2 & STATUS2_INFATUATION))
				viability -= 20;
			else if (GetGenderFromSpeciesAndPersonality(defSpecies, gBattleMons[bankDef].personality) == 
			GetGenderFromSpeciesAndPersonality(atkSpecies, gBattleMons[bankAtk].personality))
				viability -= 10;
			break;

		case EFFECT_SAFEGUARD:
			if (gSideAffecting[SIDE(bankDef)] & SIDE_STATUS_SAFEGUARD)
				viability -= 10;
			break;
		
		case EFFECT_BURNUP:
			if (!IsOfType(bankAtk, TYPE_FIRE))
				viability = 0;
			break;
		
		case EFFECT_BATON_PASS:
			if (move == MOVE_UTURN || move == MOVE_VOLTSWITCH)
				break;
			else if (ViableMonCountFromBank(bankDef) <= 1)
				viability -= 10;
				break;
			else {	// baton pass
				// check aqua ring, magnet rise, ingrain
				if (!(gStatuses3[bankAtk] & (STATUS3_ROOTED | STATUS3_AQUA_RING | STATUS3_LEVITATING)))
					viability -= 6;
				else if (!(gStatuses2[bankAtk] & STATUS2_SUBSTITUTE))
					viability -= 6;
				viabilility = aiAllStatChecks(viabilility, bankAtk, 6);
				break;
			}
			break;
			
		case EFFECT_RAPID_SPIN:
			if (move == MOVE_DEFOG) {
				if (defAbility == ABILITY_KEENEYE
				|| defAbility == ABILITY_CLEARBODY
				|| defAbility == ABILITY_FULLMETALBODY
				|| defAbility == ABILITY_WHITESMOKE)
					viability -= 10;
				else if ( gBattleMons[bankDef].statStages[STAT_STAGE_ACC] == 0)
					viability -= 10;
				else if (gSideAffecting[SIDE(bankDef)] & (SIDE_REFLECT | SIDE_SAFEGUARD | SIDE_MIST)
					goto AI_STANDARD_DAMAGE;
				else if (gNewBS->AuroraVeilTimers[bankDef])
					goto AI_STANDARD_DAMAGE;
			}
			else if ((gStatuses3[bankAtk] & STATUS3_SEEDED) || (gStatuses2[bankAtk] & STATUS2_WRAPPED))
				goto AI_STANDARD_DAMAGE;
			
			// spin checks
			if (gSideAffecting[SIDE(bankAtk)] & SIDE_STATUS_SPIKES)
				viability -= 6;
			break;
		
		
		case EFFECT_RAIN_DANCE:
			if (gBattleWeather & (WEATHER_RAIN_ANY | WEATHER_PRIMAL))
				viability -= 10;
			break;
		
		case EFFECT_SUNNY_DAY:
			if (gBattleWeather & (WEATHER_SUN_ANY | WEATHER_PRIMAL))
				viability -= 10;
			break;
		
		case EFFECT_BELLY_DRUM:
			switch (move) {
				case MOVE_MINDBLOWN:
					if (GetHealthPercentage(bankAtk) <= 50)
						viability -= 4;
					goto AI_STANDARD_DAMAGE
					
				default: //Belly Drum
					if (StatMaxed(bankAtk, STAT_STAGE_ATK)
					|| GetHealthPercentage(bankAtk) <= 50)
						viability -= 10;
			}
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
			
		case EFFECT_FAKE_OUT:
			if (!gDisableStructs[gBankAttacker].isFirstTurn)
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
			if (gBattleWeather & (WEATHER_HAIL_ANY | WEATHER_PRIMAL))
				viability -= 10;
			break;

		case EFFECT_TORMENT:
			if (defStatus2 & STATUS2_TORMENT) {
				viability -= 10;
				break;
			}
			if (defEffect == ITEM_EFFECT_MENTAL_HERB)
				viability -= 6;
			break;
		
		case EFFECT_FLATTER:
			if ((bankDef == bankAtkPartner)
			&& StatMaxed(bankDef, STAT_STAGE_SPATK))
				viability -= 10;
			else
				goto AI_CONFUSE;
			break;
		
		case EFFECT_WILL_O_WISP:
		AI_BURN_CHECK:
			if (!CanBeBurned(bankDef, bankAtk)
			|| MoveBlockedBySubstitute(move, bankAtk, bankDef))
				viability -= 10;
			break;
		
		case EFFECT_MEMENTO:
			if (ViableMonCountFromBank(bankAtk) <= 1) {
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
					// just the viablemonfromcount check, but not stat check
					break;
				
				default: //Memento
					if ((StatMinned(bankDef, STAT_STAGE_ATK) 
					&&  StatMinned(bankDef, STAT_STAGE_SPATK))
					&&  !MoveBlockedBySubstitute(move, bankAtk, bankDef))
						viability -= 10;
			}
			break;
		
		case EFFECT_CHARGE:
			if (atkStatus3 & STATUS3_CHARGED_UP) {
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
		AI_HELPING_HAND_CHECK:
			if (!(gBattleTypeFlags & BATTLE_TYPE_DOUBLE))
				viability -= 10;
			break;
		
		case EFFECT_TRICK:
			switch (move) {
				case MOVE_BESTOW:
					if (!CanTransferItem(bankAtk, atkItem))
						viability -= 10;
					break;
					
				default: //Trick
					if (!CanTransferItem(bankAtk, atkItem) 
						|| (!CanTransferItem(bankAtk, defItem))
						|| (defAbility == ABILITY_STICKYHOLD))
						viability -= 10;
					break;
			}
			break;
			
		case EFFECT_ROLE_PLAY:
			if (atkAbility == defAbility
				|| (CheckTableForAbility(defAbility, RolePlayBanTable)))
				viability -= 10;
			break;
		
		case EFFECT_WISH:
			if (gWishFutureKnock->wishCounter[bankAtk] != 0)
				viability -= 10;
			break;
		
		case EFFECT_ASSIST:
			if (CountPokemonOnTeam(bankAtk) == 1)
				viability -= 10;
			break;
		
		case EFFECT_INGRAIN:
			if (atkStatus3 & STATUS3_ROOTED)
				viability -= 10;
			break;

		case EFFECT_MAGIC_COAT:
			if (!MagicCoatableMovesInMoveset(bankDef))
				viability -= 10;
			break;
			
		case EFFECT_RECYCLE:
			if (move == MOVE_BELCH)
				if (!(gNewBS->BelchCounters & gBitTable[gBattlerPartyIndexes[bankAtk]]))
					viabilility -= 10;
				else
					goto AI_STANDARD_DAMAGE;
				break;
			else if (!gNewBS->SavedConsumedItems[bankAtk] || atkItem != 0)
				viability -= 10;
			break;
		
		case EFFECT_YAWN:
			if (defStatus3 & STATUS3_YAWN)
				viability -= 10;
			else
				goto AI_CHECK_SLEEP;
			break;
		
		case EFFECT_SKILL_SWAP:
			if (CheckTableForAbility(defAbility, WorrySeedGastroAcidBanTable) && (move != MOVE_COREENFORCER)) {
				viability -= 10;
				break;
			}
			switch (move) {
				case MOVE_WORRYSEED:
					if (defAbility == ABILITY_INSOMNIA)						
						viability -= 10;
					else if (MoveBlockedBySubstitute(move, bankAtk, bankDef))
						viability -= 10;
					break;
				case MOVE_GASTROACID:
					if (defAbility == 0)
						viability -= 10;
					else if (MoveBlockedBySubstitute(move, bankAtk, bankDef))
						viability -= 10;
					break;
				case MOVE_ENTRAINMENT:
					if (CheckTableForAbility(atkAbility, EntrainmentBanTableAttacker))
						viability -= 10;
					else
						goto AI_SUBSTITUTE_CHECK;
					break;
				case MOVE_COREENFORCER:
					goto AI_STANDARD_DAMAGE;
				case MOVE_SIMPLEBEAM:
					if (defAbility == ABILITY_SIMPLE)
						viability -= 10;
					break;
				default:	// skill swap
					if (CheckTableForAbility(atkAbility, SkillSwapBanTable)
						|| CheckTableForAbility(defAbility, SkillSwapBanTable))
						viability -= 10;
					break;
			}
		case EFFECT_IMPRISON:
			if (atkStatus3 & STATUS3_IMPRISONED)
				viabilility -= 10;
			break;
			
		case EFFECT_REFRESH:
			if (atkStatus1 & (STATUS_POISON | STATUS_BAD_POISON | STATUS_BURN | STATUS_PARALYSIS))
				viability -= 10;
			else if (move == MOVE_PSYCHOSHIFT) {
				if (atkStatus1 & (STATUS_POISON & STATUS_BAD_POISON))
					goto AI_POISON_CHECK;
				else if (atkStatus1 & STATUS_BURN)
					goto AI_BURN_CHECK;
				else if (atkStatus1 & STATUS_PARALYSIS)
					goto AI_PARALYZE_CHECK;
				else
					viability -= 10;
			}
			break;
			
		case EFFECT_SNATCH:
			// check target for any snatchable moves
			u8 i = 0;
			bool isSnatchable = FALSE;
			while (isSnatchable == FALSE) {
				isSnatchable = (gBattleMoves(gBattleMons[bankDef].moves[i]).flags & FLAG_SNATCH_AFFECTED);
				if (i == 3) {
					viabilility -= 20;	// all target moves un-snatchable
					break;
				}
				i++;
			}
			break;
			
		case EFFECT_DOUBLE_EDGE:
			if (atkAbility == ABILITY_ROCKHEAD || atkAbility == ABILITY_MAGICGUARD)
				goto AI_STANDARD_DAMAGE;
			// check < 10% hp
			else if (GetHealthPercentage(bankAtk) < 10)
				viabilility -= 4;
			else
				goto AI_STANDARD_DAMAGE;
			break; 
			
		case EFFECT_TEETER_DANCE:
			if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE) {
				// check if adjacent banks for falling asleep
				if (CanBePutToSleep(bankAtkPartner))
					viabilility -= 4;
				if (CanBePutToSleep(bankDef) == FALSE)
					viabilility -= 6;
				if (CanBePutToSleep(bankDefPartner) == FALSE)
					viabilility -= 6;
				break;
			}
			else
				goto AI_CHECK_SLEEP;
			break;
			
		case EFFECT_MUD_SPORT:
			if (gNewBS->MudSportTimer != 0)
				viability -= 10;
			break;
			
		case EFFECT_TICKLE:
			// lower viability if atk or def buffs = 0
			if (gBattleMons[bankDef].statstages[STAT_ATK] == 0
				|| gBattleMons[bankDef].statstages[STAT_DEF] == 0)
				viability -= 10;
			break;
			
		case EFFECT_COSMIC_POWER:
		AI_COSMIC_POWER:
			// lower viability if def/spdef maxed
			if (gBattleMons[bankAtk].statstages[STAT_DEF] >= 12
				|| gBattleMons[bankAtk].statstages[STAT_SPDEF] >= 12)
				viability -= 10;
			break;
			
		case EFFECT_BULK_UP:
			if (move == MOVE_COIL && gBattleMons[bankAtk].statstages[STAT_ACC] >= 12)
				viability -= 10;
			if (gBattleMons[bankAtk].statstages[STAT_ATK] >= 12
				|| gBattleMons[bankAtk].statstages[STAT_DEF] >= 12)
				viability -= 10;
			break;
			
		case EFFECT_WATER_SPORT:
			if (gNewBS->WaterSportTimer != 0)
				viability -= 10;
			break;
			
		case EFFECT_CALM_MIND:
			if ((move == MOVE_QUIVERDANCE || move == MOVE_GEOMANCY) && 
				(gBattleMons[bankAtk].statstages[STAT_SPEED] >= 12))
				viability -= 10;
			if (gBattleMons[bankAtk].statstages[STAT_SPATK] >= 12
				|| gBattleMons[bankAtk].statstages[STAT_SPDEF] >= 12)
				viability -= 10;
			break;
			
		case EFFECT_DRAGON_DANCE:
			if (move == MOVE_SHELLSMASH) {
				if (atkAbility == ABILITY_CONTRARY)
					goto AI_COSMIC_POWER;
				else if (gBattleMons[bankAtk].statstages[STAT_SPATK] >= 12)
					viability -= 10;
			}
			// dragon dance
			if (gBattleMons[bankAtk].statstages[STAT_ATK] >= 12 || 
				gBattleMons[bankAtk].statstages[STAT_SPEED] >= 12)
				viability -= 10;			
			break;
			
		case EFFECT_StoredPower:
			if (move == MOVE_PUNISHMENT)
				viabilility = aiAllStatChecks(viabilility, bankDef, 6);
			else
				viabilility = aiAllStatChecks(viabilility, bankAtk, 6);		
			break;
			
		case EFFECT_FrostBreath:
			if (defAbility == ABILITY_SHELLARMOR || defAbility == ABILITY_BATTLEARMOR)
				viabilility -= 4;
			break;
			
		case EFFECT_ME_FIRST:
			if (move == MOVE_MEFIRST) {
				if (MoveWouldHitFirst(move, bankAtk, bankDef))
					viabilility -= 10;
				else
					goto AI_SUBSTITUTE_CHECK;
			}
			else if (move == MOVE_TRUMPCARD)
				goto AI_STANDARD_DAMAGE;
			else if ((defAbility == ABILITY_KLUTZ)
				|| (gNewBS->MagicRoomTimer != 0)
				|| (GetPocketByItemId(atkItem) != POCKET_BERRIES))
				viabilility -= 10;
			break;
			
		case EFFECT_ClearSmog: 
			u8 i;
			for (i = 0; i <= 6; i++) {
				if (gBattleMons[bankDef].statStages[i] > 6)
					goto AI_STANDARD_DAMAGE;
			}
			break;
		case EFFECT_SpeedBalls:
			if (move == MOVE_ELECTROBALL && MoveWouldHitFirst(move, bankAtk, bankDef))
					viabilility -= 6;
			else if (move == MOVE_GYROBALL && MoveWouldHitFirst(move, bankDef, bankAtk))
					viabilility -= 6;					
			goto AI_STANDARD_DAMAGE;
			
		case EFFECT_SET_TERRAIN:
			switch (move) {
				case MOVE_ELECTRICTERRAIN:
					if (gBattleTerrain == ELECTRIC_TERRAIN)
						viabilility -= 10;
				case MOVE_GRASSYTERRAIN:
					if (gBattleTerrain == GRASSY_TERRAIN)
						viabilility -= 10;				
				case MOVE_MISTYTERRAIN:
					if (gBattleTerrain == MISTY_TERRAIN)
						viabilility -= 10;				
				case MOVE_PSYCHICTERRAIN:
					if (gBattleTerrain == PSYCHIC_TERRAIN)
						viabilility -= 10;
			}
			break;
						
		case EFFECT_FIELD_EFFECTS:
			switch (move) {
				case MOVE_TRICKROOM:
					if (MoveWouldHitFirst(move, bankAtk, bankDef))
						viabilility -= 10;
					break;
				case MOVE_MAGICROOM:
					if (gNewBS->MagicRoomTimer != 0)
						viabilility -= 10;
					break;				
				case MOVE_WONDERROOM:
					if (gNewBS->WonderRoomTimer != 0)
						viabilility -= 10;
					break;				
				case MOVE_GRAVITY:
					if (gNewBS->GravityTimer != 0)
						viabilility -= 10;
					break;
				case MOVE_IONDELUGE:
					goto AI_HELPING_HAND_CHECK;
				case MOVE_PLASMAFISTS:
					goto AI_STANDARD_DAMAGE;
			}
			break;
			
		case EFFECT_FLING:
			if !(CanTransferItem(atkSpecies, atkItem, GetBankPartyData(bankAtk)))
				viabilility -= 10;
			break;
					
		case EFFECT_ATTACK_BLOCKERS:
			switch (move) {
				case MOVE_HEALBLOCK:
					if (gNewBS->HealBlockTimers[bankDef] != 0)
						viabilility -= 10;
					break;
				case MOVE_EMBARGO:
					if ((defAbility == ABILITY_KLUTZ) 
						|| gNewBS->MagicRoomTimer != 0
						|| gNewBS->EmbargoTimers[bankDef] != 0)
						viabilility -= 10;
					goto AI_SUBSTITUTE_CHECK;
				//case MOVE_POWDER:
					//break;
				case MOVE_TELEKINESIS:
					if ((defStatus3 & (STATUS3_ROOTED | STATUS3_SMACKED_DOWN | STATUS3_TELEKINESIS))
						|| (gNewBS->GravityTimer != 0)
						|| (GetBankItemEffect(bankDef) == ITEM_EFFECT_IRON_BALL)
						||  (defSpecies == PKMN_DIGLETT)
						||  (defSpecies == PKMN_DUGTRIO)
						||  (defSpecies == PKMN_DIGLETT_A)
						||  (defSpecies == PKMN_DUGTRIO_A)
						||  (defSpecies == PKMN_SANDYGAST)
						||  (defSpecies == PKMN_PALOSSAND)
						||  (defSpecies == PKMN_GENGAR_MEGA))
						viabilility -= 10;
					goto AI_SUBSTITUTE_CHECK;
					
				case MOVE_THROATCHOP:
					goto AI_STANDARD_DAMAGE;
			}
			break;
			
		case EFFECT_TYPE_CHANGES:
			switch (move) {
				case MOVE_SOAK:
					if (gBattleMons[bankDef].type1 == TYPE_WATER || gBattleMons[bankDef].type2 == TYPE_WATER)
						viabilility -= 10;
					goto AI_SUBSTITUTE_CHECK;
				case MOVE_TRICKORTREAT:
					if (gBattleMons[bankDef].type1 == TYPE_GHOST
					|| gBattleMons[bankDef].type2 == TYPE_GHOST
					|| gBattleMons[bankDef].type3 == TYPE_GHOST)
						viabilility -= 10;
					goto AI_SUBSTITUTE_CHECK;
				case MOVE_FORESTSCURSE:
					if (gBattleMons[bankDef].type1 == TYPE_GRASS
					|| gBattleMons[bankDef].type2 == TYPE_GRASS
					|| gBattleMons[bankDef].type3 == TYPE_GRASS)
						viabilility -= 10;
					goto AI_SUBSTITUTE_CHECK;
			}
			break;
			
		case EFFECT_HEAL_TARGET:
			if (move == MOVE_POLLENPUFF)
				goto AI_STANDARD_DAMAGE;
			else if (gBattleMons[bankDef].hp > (gBattleMons[bankDef].maxHP/2))
				viabilility -= 10;
			goto AI_SUBSTITUTE_CHECK;
			
		case EFFECT_TOPSY_TURVY_ELECTRIFY:
			if (move == MOVE_ELECTRIFY && MoveWouldHitFirst(move, bankDef, bankAtk))
				viabilility -= 10;
			goto AI_SUBSTITUTE_CHECK;
				
		case EFFECT_TEAM_EFFECTS:
			switch (move) {
				case MOVE_TAILWIND:
					if (gNewBS->TailwindTimers[SIDE(bankAtk)] != 0)
						viabilility -= 10;
					break;				
				case MOVE_LUCKYCHANT:
					if (gNewBS->LuckyChantTimers[SIDE(bankAtk)] != 0)
						viabilility -= 10;
					break;
				case MOVE_MAGNETRISE:
					if ((atkStatus3 & (STATUS3_ROOTED | STATUS3_LEVITATING | STATUS3_SMACKED_DOWN)) // 0x600400
					|| (gNewBS->GravityTimer > 0)
					|| (GetBankItemEffect(bankAtk) == ITEM_EFFECT_IRON_BALL)
					|| (gNewBS->MagnetRiseTimers[bankAtk] != 0)
					|| (defAbility == ABILITY_LEVITATE))
						viabilility -= 10;
					break;
			}
			break;
			
		case EFFECT_Synchronoise:
			// check holding ring target or is of same type
			if (GetBankItemEffect(bankAtk) == ITEM_EFFECT_RING_TARGET
				|| IsOfType(bankDef, gBattleMons[bankAtk].type1)
				|| IsOfType(bankDef, gBattleMons[bankAtk].type2)
				|| IsOfType(bankDef, gBattleMons[bankAtk].type3))
				break;
			else
				viabilility -= 10;
			break;
		
		default:
		AI_STANDARD_DAMAGE:
			if (moveSplit != SPLIT_STATUS) {
				if (TypeCalc(move, bankAtk, bankDef, 0, FALSE) & (MOVE_RESULT_NO_EFFECT | MOVE_RESULT_MISSED))
					viability -= 10;
			}
			break;
	} // move effects switch
	return viabilility;
	
	// other switch breakout
	AI_SUBSTITUTE_CHECK:
	if (MoveBlockedBySubstitute(move, bankAtk, bankDef))
		viability -= 10;
	return viability;
}

*/
