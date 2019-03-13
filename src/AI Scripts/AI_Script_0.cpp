//Add ,move to NO_MOLD_BREAKERS

#include "defines.h"
#include "ai_script_functions.h"
#include "helper_functions.h"

u8 AI_Script_Negatives(u8 bankAtk, u8 bankDef, u16 move) {
RESTART_AI_SCRIPT_0:
	u8 viability = 100;
	u8 decreased;
	int i;
	
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
	
	u8 moveEffect = MOVE_EFFECT(move);
	u8 moveSplit = SPLIT(move);
	u8 moveRange = RANGE(move);
	u8 moveType = GetMoveType(bankAtk, move);
	u8 moveFlags = MOVE_FLAGS(move);
	u8 moveAcc = MOVE_ACC(move); //Do something with stat buffs and Unaware/Keen Eye
	
	//Affects User Check
	if (moveRange & AFFECTS_USER || moveRange & AFFECTS_FIELD)
		goto MOVESCR_CHECK_0;
	
	//Ranged Move Check
	if (moveRange & HIT_ALL) {
		if (moveType == TYPE_ELECTRIC && ABILITY_OTHER_SIDE(bankAtk, ABILITY_LIGHTNINGROD))
			return viability - 20;
		else if (moveTYPE == TYPE_WATER && ABILITY_OTHER_SIDE(bankAtk, ABILITY_STORMDRAIN))
			return viability - 20;
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
	// Not Very Effective check
	

	// Gravity Table Prevention Check
	if (gNewBS->GravityTimer && CheckMoveInTable(move, GravityTable))
		return 0; //Can't select this move period

	// Ungrounded check (need air balloon in CheckGrounding)
	if ( (CheckGrounding(bankDef) == IN_AIR) && (moveType==TYPE_GROUND) ) {
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
				if ( (moveType == TYPE_ELECTRIC) && !(moveSplit == SPLIT_STATUS) )
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
				if ( (moveType == TYPE_FIRE) && !(moveSplit == SPLIT_STATUS) )
					return viability - 20;
				break;
				
			case ABILITY_FLASHFIRE:
				if (moveType == TYPE_FIRE)
					return viability - 20;
				break;
				
			case ABILITY_HEATPROOF:
				if ( (moveType == TYPE_FIRE) && !(moveSplit == SPLIT_STATUS) )
					return viability - 10;
				break;
			
			// Grass
			case ABILITY_SAPSIPPER:
				if (moveType == TYPE_GRASS)
					return viability - 20;
				break;
				
			// Dark
			case ABILITY_JUSTIFIED:
				if ((moveType == TYPE_DARK) && !(moveSplit == SPLIT_STATUS) )
					return viability - 10;
				break;
				
				
			// multiple move types
			case ABILITY_THICKFAT:
				if (moveType == TYPE_FIRE || moveType == TYPE_ICE )
					return viability - 10;
				break;
				
			case ABILITY_RATTLED:
				if ( !(moveSplit == TYPE_STATUS) && (moveType == TYPE_DARK) || (moveType == TYPE_GHOST) || (moveType == TYPE_BUG) )
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
				&& (CheckMoveEffectInTable(move, SetStatusTable) || CheckMoveEffectInTable(move, StatLowerTable))
					return viability - 10;
				break;
			
			case ABILITY_MAGICBOUNCE:
				if (moveFlags & FLAG_REFLECTED_BY_MAGIC_COAT)
					return viability - 20;
				break;
			
			case ABILITY_CONTRARY:
				if (CheckMoveEffectInTable(move, StatLowerTable))
					return viability - 20;
				break;
			
			case ABILITY_CLEARBODY:
			case ABILITY_FULLMETALBODY:
			case ABILITY_WHITESMOKE:
				if (CheckMoveEffectInTable(move, StatLowerTable))
					return viability - 10;
				break;
			
			case ABILITY_HYPERCUTTER:
				if (moveEffect == EFFECT_LOWER_TGT_ATK_1
				||  moveEffect == EFFECT_LOWER_TGT_ATK_2)
					return viability - 10;
				break;
			
			case ABILITY_KEENEYE:
				if (moveEffect == EFFECT_LOWER_TGT_ACC_1
				||  moveEffect == EFFECT_LOWER_TGT_ACC_2)
					return viability - 10;
				break;
			
			case ABILITY_BIGPECKS:
				if (moveEffect == EFFECT_LOWER_TGT_DEF_1
				||  moveEffect == EFFECT_LOWER_TGT_DEF_2)
					return viability - 10;
				break;
			
			case ABILITY_DEFIANT:
			case ABILITY_COMPETITIVE:
				if (CheckMoveEffectInTable(move, StatLowerTable))
					return viability - 8; //Not -10 b/c move still works, just not recommended
				break;
			
			case ABILITY_COMATOSE:
				if (CheckMoveEffectInTable(move, SetStatusTable))
					return viability - 10;
				break;
			
			case ABILITY_WONDERSKIN:
				if (moveSplit == SPLIT_STATUS)
					//moveAcc = 50;
					return viability - 8;
				break;
				
			case ABILITY_LEAFGUARD:
				if (WEATHER_HASS_EFFECT && (gBattleWeather & WEATHER_SUN_ANY)
				&& CheckMoveEffectInTable(move, SetStatusTable))
					return viability - 10;
		}
		
		//Target Partner Ability Check
		if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE) {
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
					if (moveEffect == EFFECT_SLEEP || moveEffect == EFFECT_SET_YAWN)
						return viability - 10;
					break;
				
				case ABILITY_FLOWERVEIL:
					if (IsOfType(bankDef, TYPE_GRASS)
					&& (CheckMoveEffectInTable(move, SetStatusTable) || CheckMoveEffectInTable(move, StatLowerTable))
						return viability - 10;
					break;
				
				case ABILITY_AROMAVEIL:
					if (CheckMoveInTable(move, AromaVeilTable))
						return viability - 10;
					break;
				
				case ABILITY_MAGICBOUNCE:
					if (moveFlags & FLAG_REFLECTED_BY_MAGIC_COAT
					&& moveRange & AFFECTS_TARGET_SIDE)
						return viability - 20;
			}
		}
	}
	
	//Terrain Check
	if (CheckGrounding(bankDef) == GROUNDED) {
		switch (TerrainType) {
			case ELECTRIC_TERRAIN:
				if (moveEffect == EFFECT_SLEEP || moveEffect == EFFECT_SET_YAWN)
					return viability - 10;
				break;
			
			case GRASSY_TERRAIN:
				if ( (move == MOVE_BULLDOZE) || (move == MOVE_EARTHQUAKE) || (move == MOVE_MAGNITUDE) )
					return viability - 10;
			
			case MISTY_TERRAIN:
				if (CheckMoveEffectInTable(move, SetStatusTable) || CheckMoveEffectInTable(move, ConfusionTable))
					return viability - 10;
				break;
				
			case PSYCHIC_TERRAIN:
				if (CheckMovePriority(move, bankAtk) > 0) //Check if right num
					return viability - 10;
		}
	}
		
	//Minior Check
	if (defSpecies == PKMN_MINIORSHIELD
	&& CheckMoveEffectInTable(move, SetStatusTable))
		return viability - 10;
		
	//Throat Chop Check
	if (gNewBS->ThroatChopTimers[bankAtk] && CheckSoundMove(move))
		return 0; //Can't select this move period
			
	//Heal Block Check
	if (gNewBS->HealBlockTimers[bankAtk] && CheckHealingMove(move))
		return 0; //Can't select this move period
		
	//Primal Weather Check
	if (gBattleWeather & WEATHER_SUN_PRIMAL && moveType == TYPE_WATER && !(moveSplit == SPLIT_STATUS) )
		return viability - 20;
	else if (gBattleWeather & WEATHER_RAIN_PRIMAL && moveType == TYPE_FIRE && !(moveSplit == SPLIT_STATUS) )
		return viability - 20;
	
	// Check Move Effects
	MOVESCR_CHECK_0:
	switch (moveEffect) {
		case EFFECT_SLEEP:
		AI_SET_SLEEP:
			if (!CanBePutToSleep(bankDef, bankAtk)
			|| !CanBeGeneralStatused(bankDef)
			|| MoveBlockedBySubstitute(move, bankAtk, bankDef))
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
			else if (!(ViableMonCountFromBank(bankAtk) == 1 //If the AI has one PKMN left,
			&& ViableMonCountFromBank(bankDef) == 1		 //and the Target only has one PKMN left,
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
			
		case EFFECT_MIRROR_MOVE: //May cause issues with priority calcs
			if (lastTakenMoveFrom[bankAtk][bankDef]) {
				move = lastTakenMoveFrom[bankAtk][bankDef];
				goto RESTART_AI_SCRIPT_0;
			}
		
		// useless effects
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
		AI_RAISE_SPATK:			
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
		AI_ATTACK_MINUS:
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
					
					//Wimplicit fallthrough
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
			if (decreased) break;
			
			if (MoveBlockedBySubstitute(move, bankAtk, bankDef))
				viability -= 10;
			break;
			
		case EFFECT_DEFENSE_DOWN: 
		case EFFECT_DEFENSE_DOWN_2: 
		AI_DEFENSE_MINUS:		
			if (StatMinned(bankDef, STAT_STAGE_DEF))
				viability -= 10;
			else if (MoveBlockedBySubstitute(move, bankAtk, bankDef))
				viability -= 10;
			break;
		
		case EFFECT_SPEED_DOWN:
		case EFFECT_SPEED_DOWN_2: 
		AI_SPEED_MINUS:	
			if (StatMinned(bankDef, STAT_STAGE_SPD))
				viability -= 10;
			else if (MoveBlockedBySubstitute(move, bankAtk, bankDef))
				viability -= 10;
			break;
		
		case EFFECT_SPECIAL_ATTACK_DOWN: 
		case EFFECT_SPECIAL_ATTACK_DOWN_2:
		AI_SPATK_MINUS:		
			if (StatMinned(bankDef, STAT_STAGE_SPATK))
				viability -= 10;
				
			else if (move == MOVE_CAPTIVATE)
				if (gBattleMons[bankAtk].gender == GENDER_UNKNOWN
				||  gBattleMons[bankDef].gender == GENDER_UNKNOWN
				||  gBattleMons[bankAtk].gender == gBattleMons[bankDef].gender)
					viability -= 10;
			
			else if (MoveBlockedBySubstitute(move, bankAtk, bankDef))
				viability -= 10;
			break;
		
		case EFFECT_SPECIAL_DEFENSE_DOWN:
		case EFFECT_SPECIAL_DEFENSE_DOWN_2:
		AI_SPDEF_MINUS:	
			if (StatMinned(bankDef, STAT_STAGE_SPDEF))
				viability -= 10;
			else if (MoveBlockedBySubstitute(move, bankAtk, bankDef))
				viability -= 10;
			break;
		
		case EFFECT_ACCURACY_DOWN: 
		case EFFECT_ACCURACY_DOWN_2:
		AI_ACCURACY_MINUS:
			if (StatMinned(bankDef, STAT_STAGE_ACC))
				viability -= 10;
			else if (MoveBlockedBySubstitute(move, bankAtk, bankDef))
				viability -= 10;
			break;
		
		case EFFECT_EVASION_DOWN: 
			// TO DO
			goto AI_Minus_10_Evsn_Min
		case EFFECT_EVASION_DOWN_2: 
			AI_Minus_10_Evsn_Min
			
			if (StatMinned(bankDef, STAT_STAGE_EVSN))
				viability -= 10;
			else if (MoveBlockedBySubstitute(move, bankAtk, bankDef))
				viability -= 10;
			break;
		
		case EFFECT_HAZE: //Clear smog here?
		HAZE_CHECK:
			decreased = FALSE;
			for (i = 0; i < NUM_STAT_STAGES-1; ++i) {
				if (gBattleMons[bankAtk].statstages[i] > 6)
					viability -= 10;
					decreased = TRUE;
					break;
			}
			if (decreased) break;
			
			for (i = 0; i < NUM_STAT_STAGES-1; ++i) {
				if (gBattleMons[bankDef].statstages[i] < 6)
					viability -= 10;
					decreased = TRUE;
					break;
			}
			break;
			
		case EFFECT_ROAR:
			switch (move) {
				case MOVE_DRAGONTAIL:
				case MOVE_CIRCLETHROW:
					goto AI_STANDARD_DAMAGE;
				
				default:
					if (ViableMonCountFromBank(bankDef) <= 1
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
					
					if (bankDef == bankAtkPartner) break; //Always heal your ally
					
					if (GetHealthPercentage(bankAtk) >= 90)
						viability -= 9; //No point in healing, but should at least do it if nothing better
					break;
					
				default:
					if (GetHealthPercentage(bankAtk) == 100)
						viability -= 10;
					else if (GetHealthPercentage(bankAtk) >= 90)
						viability -= 9; //No point in healing, but should at least do it if nothing better
			}
			break;
		
		//case EFFECT_POISON_HIT:
		case EFFECT_POISON:
		//case EFFECT_POISON_FANG:
		//case EFFECT_POISON_TAIL:
		case EFFECT_TOXIC:
			switch (move) {
				case MOVE_TOXICTHREAD:
					if (!StatMinned(bankDef, STAT_STAGE_SPD)
					&& !MoveBlockedBySubstitute(move, bankAtk, bankDef)) 
						break;
					
					//wimplicit fallthrough;
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
			if (TypeCalc(move, bankAtk, bankDef, 0, FALSE) & (RESULT_NO_EFFECT | RESULT_MISSED)
			|| (NO_MOLD_BREAKERS(bankAtk) && defAbility == ABILITY_STURDY)
				viability -= 10;
			break;
		
		case EFFECT_RECOIL_IF_MISS:
			if (atkAbility == ABILITY_MAGICGUARD)
				goto AI_STANDARD_DAMAGE;
			if (moveAcc < 75)
				viability -= 6;
			break;
				
		case EFFECT_MIST:
			if (gSideStatuses[bankAtk & 1] & SIDE_STATUS_MIST)
				viability -= 10;
			break;
		
		case EFFECT_RECOIL:
			if (atkAbility == ABILITY_MAGICGUARD || atkAbility == ABILITY_ROCKHEAD)
				goto AI_STANDARD_DAMAGE;
			if (GetHealthPercentage(bankAtk) < 10)
				viability -= 4;
			break;
			
		case EFFECT_FOCUS_ENERGY:
			if (defStatus2 & STATUS2_FOCUS_ENERGY)
				viability -= 10;
			break;
		
		case EFFECT_CONFUSE:
		AI_CONFUSE:
			if (defStatus2 & STATUS2_CONFUSE 
			|| (NO_MOLD_BREAKERS(bankAtk) && defAbility == ABILITY_OWNTEMPO)
				viability -= 10;
			else if (defEffect == ITEM_EFFECT_MENTAL_HERB)
				viability -= 6;
			break;

		case EFFECT_TRANSFORM:
			if (atkStatus2 & STATUS2_TRANSFORMED
			||  defStatus2 & (STATUS2_TRANSFORMED | STATUS2_SUBSTITUTE))
				viability -= 10;
			
		case EFFECT_REFLECT: 
		AI_REFLECT:
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
			if (!CanBeParalyzed(bankDef, bankAtk)
			|| MoveBlockedBySubstitute(move, bankAtk, bankDef))
				viability -= 10;
			else {
				if (move == MOVE_THUNDERWAVE
				&& TypeCalc(move, bankAtk, bankDef, 0, FALSE ) & (RESULT_NO_EFFECT | RESULT_MISSED))
					viability -= 10;
			}
			break;
			
		case EFFECT_SUBSTITUTE:
			if (defStatus2 & STATUS2_SUBSTITUTE
			|| GetHealthPercentage(bankAtk) <= 25)
				viability -= 10;
			
			else if (ABILITY(GetFoeBank(bankAtk)) == ABILITY_INFILTRATOR 
			|| (gBattleTypeFlags & BATTLE_TYPE_DOUBLE && ABILITY(PARTNER(GetFoeBank(bankAtk))) == ABILITY_INFILTRATOR)
				viability -= 8; //Not really much point in making a sub if the foe is gonna bust right through it
			break;
			
		case EFFECT_MIMIC:
			//Put some last used move logic here
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
					|| defStatus3 & 0x4
					|| defAbility == ABILITY_LIQUIDOOZE)
						break;
			}
			break;
		
		case EFFECT_DISABLE:
			//Check Disable Timer
			if ( gDisableStructs[bankDef].disableTimer1 )
				viability -= 10;			
			//Check Last Used Move ??
			
			//Check Mental Herb
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
		
		case EFFECT_ENCORE:
			//Check Encore Timer
			if (gDisableStructs[gActiveBattler].encoreTimer)
				viability -= 10;
			break;
			
			//Check Last Used Move
			if (defEffect == ITEM_EFFECT_MENTAL_HERB)
				viability -= 6;
			break;
			
		case EFFECT_PAIN_SPLIT:
			if (gBattleMons[bankAtk].hp > (gBattleMons[bankAtk].hp + gBattleMons[bankDef].hp) / 2)
				viability -= 10;
			break;
		
		case EFFECT_SNORE:
		case EFFECT_SLEEP_TALK:
			if (!(defStatus1 & STATUS_SLEEP) && defAbility != ABILITY_COMATOSE)
				viability -= 10;
			break;
		
		case EFFECT_CONVERSION_2:
			// To do
			
		case EFFECT_LOCK_ON:
			switch (move) {
				case MOVE_LASERFOCUS:
					if (gNewBS->LaserFocusTimers[bankAtk])
						viability -= 10;
					else if (defAbility == ABILITY_SHELLARMOR || defAbility == ABILITY_BATTLEARMOR)
						viability -= 8;
					break;
				
				default:
					if (atkStatus3 & STATUS3_LOCKED_ON
					|| atkAbility == ABILITY_NOGUARD
					|| defAbility == ABILITY_NOGUARD)
						viability -= 10;
			}
		
		case EFFECT_DESTINY_BOND:
			if (gLastUsedMoves[bankAtk] == move
			|| atkStatus2 & STATUS2_DESTINY_BOND)
				viability -= 10;
			break;
		
		case EFFECT_FLAIL:
			// check HP percentage
			
			break;
		
		case EFFECT_SPITE:
			//Spite Logic
			
			break;
		
		case EFFECT_FALSE_SWIPE:
			if (MoveKnocksOut(move, bankAtk, bankDef))
				viability -= 10;
			break;
		
		case EFFECT_HEAL_BELL:
			if (!PartyMemberStatused(bankAtk))
				viability -= 10;
			break;
			
		case EFFECT_THIEF:
			// check if holding an item
			break;
		
		case EFFECT_MEAN_LOOK:
			switch (move) {
				case MOVE_SPIRITSHACKLE:
				case MOVE_ANCHORSHOT:
					goto AI_STANDARD_DAMAGE;
					
				default:
					if (defStatus2 & 0x400E000 
					|| defEffect == ITEM_EFFECT_SHED_SHELL
					|| IsOfType(bankDef, TYPE_GHOST)
					|| atkAbility == ABILITY_SHADOWTAG
					|| (atkAbility == ABILITY_ARENATRAP && CheckGrounding(bankDef))
					|| (atkAbility == ABILITY_MAGNETPULL && IsOfType(bankDef, TYPE_STEEL)))
						viability -= 10;
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
			else {
				if (atkAbility == ABILITY_CONTRARY
				|| (StatMaxed(bankAtk, STAT_STAGE_ATK) && StatMaxed(bankAtk, STAT_STAGE_DEF))
					viability -= 10;
			}
			
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
					if (!FirstTurn) { //Replace with something legit
						viability -= 10;
						decreased = TRUE;
					}
			}
			if (decreased) break;
			
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
			
			if (GetProtectActivity(bankAtk) > 0) {
				if (umodsi(Random(), 100) < 50)
					viability -= 6;
			}
		
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
					
					if (gBattleMons[bankDef].statstages[STAT_STAGE_EVSN-1] <= 6
					&& !IsOfType(bankDef, TYPE_DARK)
						viability -= 9;
					break;
				
				default: //Foresight
					if (defStatus2 & STATUS2_FORESIGHT)
						viability -= 10;
					
					if (gBattleMons[bankDef].statstages[STAT_STAGE_EVSN-1] <= 6
					&& !IsOfType(bankDef, TYPE_GHOST)
						viability -= 9;
			}
			break;
			
		case EFFECT_PERISH_SONG:
			if (gStatuses3[GetFoeBank(bankAtk)] & STATUS3_PERISH_SONG)
			&& !(gBattleTypeFlags & BATTLE_TYPE_DOUBLE && !(gStatuses3[PARTNER(GetFoeBank(bankAtk))] & STATUS3_PERISH_SONG)))
				viability -= 10;
			break;
			
		case EFFECT_SANDSTORM:
			if (gBattleWeather & (WEATHER_SANDSTORM_ANY | WEATHER_PRIMAL))
				viability -= 10;
			break;
		
		case EFFECT_ENDURE:
			if (gBattleMons[bankAtk].hp == 1) {
				viability -= 10;
				break;
			}
			
			if (GetProtectActivity(bankAtk) > 0) {
				if (umodsi(Random(), 100) < 50)
					viability -= 6;
			}
			break;
			
		case EFFECT_SWAGGER:
			if (bankDef == bankAtkPartner)
			&& StatMaxed(bankDef, STAT_STAGE_ATK))
				viability -= 10;
			else
				goto AI_CONFUSE;
			break;
		
		case EFFECT_ATTRACT: 
		AI_ATTRACT:
			if (defAbility == ABILITY_OBLIVIOUS || (defStatus2 & STATUS2_INFATUATION) )
				viability -= 20;
			else if (GetGenderFromSpeciesAndPersonality(defSpecies, gBattleMons[bankDef].personality) == 
				GetGenderFromSpeciesAndPersonality(atkSpecies, gBattleMons[bankAtk].personality) )
				viability -= 10;
			break;

		case EFFECT_SAFEGUARD:
		AI_SAFEGUARD:
			if (gSideAffecting[SIDE(bankDef)] & SIDE_STATUS_SAFEGUARD)
				viability -= 10;
			break;
		
		case EFFECT_BURNUP:
			if !(IsOfType(bankAtk, TYPE_FIRE) )
				viability = 0;
			break;
			
		case EFFECT_BATON_PASS:
		AI_BATON_PASS:
			if (move == MOVE_UTURN || move == MOVE_VOLTSWITCH)
				break;
			else if (ViableMonCountFromBank(bankDef) <= 1)
				viability -= 10;
				break;
			else {	// baton pass
				// check aqua ring, magnet rise, ingrain
				if !( gStatuses3[bankAtk] & (STATUS3_ROOTED || STATUS3_AQUA_RING || STATUS3_LEVITATING) ) {
					viability -= 6;
				else if !( gStatuses2[bankAtk] & STATUS2_SUBSTITUTE )
					viability -= 6;
				// check stats
				u8 statVal = 6;
				u8 statId = 0;
				// if any stat decreased, less viability for move
				while (statVal <= 6) {
					statVal = gBattleMons[bankAtk].statStages[statId];
					if (statId == 6)
						break;
					statId++;
				}
			}
			break;
			
		case EFFECT_RAPID_SPIN:
		AI_RAPID_SPIN:
			if (move == MOVE_DEFOG) {
				if (defAbility == ABILITY_KEENEYE
					|| defAbility == ABILITY_CLEARBODY
					|| defAbility == ABILITY_FULLMETALBODY
					|| defAbility == ABILITY_WHITESMOKE)
					viability -= 10;
				else if (  gBattleMons[bankDef].statStages[STAT_STAGE_ACC] == 0 )
					viability -= 10;
				else if ( gSideAffecting[SIDE(bankDef)] & (SIDE_REFLECT || SIDE_SAFEGUARD || SIDE_MIST)
					goto AI_STANDARD_DAMAGE;
				else if ( gNewBS->AuroraVeilTimers[bankDef] )
					goto AI_STANDARD_DAMAGE;
			}
			else {
				if ( (gStatuses3[bankAtk] & STATUS3_SEEDED) || (gStatuses2[bankAtk] & STATUS2_WRAPPED) )
					goto AI_STANDARD_DAMAGE;
			}
			// spin checks
			if ( gSideAffecting[SIDE(bankAtk)] & SIDE_STATUS_SPIKES )
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
			switch (move) {
				case MOVE_SPECTRALTHIEF:
					goto AI_STANDARD_DAMAGE;
					
				default: //Psych Up
					goto HAZE_CHECK;
			}
			break;
			
		case EFFECT_FUTURE_SIGHT:
			// check if future sight already set
			if (gWishFutureKnock->futureSightCounter[bankAtk])
				viability -= 10;
			else
				goto AI_STANDARD_DAMAGE;
			break;
			
		case EFFECT_FAKE_OUT:
		AI_FAKEOUT:
			if !(gDisableStructs[gBankAttacker].isFirstTurn)
				viability -= 10;
			else
				goto AI_STANDARD_DAMAGE;
			break;
			
		case EFFECT_STOCKPILE:
			if (/*StockpileCounter(bankAtk) >= 3*/)
				viability -= 10;
			break;
			
		case EFFECT_SPIT_UP:
			if (/*StockpileCounter(bankAtk) == 0*/)
				viability -= 10;
			else
				goto AI_STANDARD_DAMAGE;
			break;
		
		case EFFECT_SWALLOW:
			if (/*StockpileCounter(bankAtk) == 0*/)
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
			if (bankDef == bankAtkPartner)
			&& StatMaxed(bankDef, STAT_STAGE_SPATK))
				viability -= 10;
			else
				goto AI_CONFUSE;
			break;
		
		case EFFECT_WILL_O_WISP:
			If (!CanBeBurned(bankDef, bankAtk)
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
					// Some Logics
					break;
				
				default: //Memento
					if (StatMinned(bankDef, STAT_STAGE_ATK) 
					&&  StatMinned(bankDef, STAT_STAGE_SPATK))
					&&  !MoveBlockedBySubstitute(move, bankAtk, bankDef)
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
			if (/*TauntTimer(bankDef)*/)
				viability -= 1;
			break;
		
		case EFFECT_FOLLOW_ME:
		case EFFECT_HELPING_HAND:
			if (!(gBattleTypeFlags & BATTLE_TYPE_DOUBLE))
				viability -= 10;
			break;
		
		case EFFECT_TRICK:
			switch (move) {
				case MOVE_BESTOW:
					if (!CanTranferItem(bankAtk, atkItem))
						viability -= 10;
					break;
					
				default: //Trick
					if (!CanTranferItem(bankAtk, atkItem) || !CanTranferItem(bankAtk, defItem)
					|| defAbility == ABILITY_STICKYHOLD)
						viability -= 10;
					break;
			}
			break;
			
		case EFFECT_ROLE_PLAY:
			if (atkAbility == defAbility
			|| CheckTableForAbility(defAbility, RolePlayBanList)
			|| CheckTableForAbility(defAbility, BadAbilities))
				viability -= 10;
			break;
		
		case EFFECT_WISH:
			(/*If WishCounter(bankAtk) != 0*/)
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
			if (!gNewBS->SavedConsumedItems[bankAtk] || atkItem != 0)
				viability -= 10;
			break;
		
		case EFFECT_YAWN:
			if (defStatus3 & STATUS3_DROWSY)
				viability -= 10;
			else
				goto AI_SET_SLEEP;
			break;
			
		case EFFECT_ENDEAVOR: 
		AI_PAINSPLIT:
			// to do
		
		case EFFECT_ERUPTION:
			// to do
			// flail AI
			
		case EFFECT_SKILL_SWAP:
		AI_SKILLSWAP:
			// to do
		case EFFECT_IMPRISON:
		AI_IMPRISON:
			// to do
		case EFFECT_REFRESH:
		AI_REFRESH:
			// to do
		case EFFECT_GRUDGE:
		AI_BATON_PASS:
			// to do
		case EFFECT_SNATCH:
			break;
		case EFFECT_DOUBLE_EDGE:
		AI_RECOIL:
			// to do
		case EFFECT_TEETER_DANCE:
		AI_TEETER_DANCE:
			// to do
		case EFFECT_MUD_SPORT:
		AI_MUD_SPORT:
			// to do
		case EFFECT_TICKLE:
		AI_TICKLE:
			// to do
		case EFFECT_COSMIC_POWER:
		AI_COSMIC_POWER:
			// to do
		case EFFECT_BULK_UP:
		AI_BULK_UP:
			// to do
		case EFFECT_WATER_SPORT:
		AI_WATER_SPORT:
			// to do
		case EFFECT_CALM_MIND:
		AI_CALM_MIND:
			// to do
		case EFFECT_DRAGON_DANCE:
		AI_DRAGON_DANCE:
			// to do

		case EFFECT_STAT_SWAP_SPLT:
			break;
		case EFFECT_StoredPower: 
		AI_STOREDPOWER:
			// to do
		case EFFECT_FrostBreath:
		AI_FROSTBREATH:
			// to do
		case EFFECT_ME_FIRST:
		AI_NATURALGIFT_MEFIRST:
			// to do
		case EFFECT_ClearSmog: 
		AI_CLEARSMOG:
			// to do
		case EFFECT_SpeedBalls:		// to do
		AI_SPEED_BALLS:
			// to do
		case EFFECT_SET_TERRAIN:
		AI_TERRAIN:
			// to do
		case EFFECT_PLEDGE:
			// to do - check ally move for same pledge
		case EFFECT_FIELD_EFFECTS:
		AI_FIELDEFFECTS:
			// to do 
		case EFFECT_FLING:
		AI_FLING:
			// to do
		case EFFECT_ATTACK_BLOCKERS:
		AI_ATTACKBLOCKERS:
			// to do
		case EFFECT_TYPE_CHANGES:
		AI_TYPECHANGERS:
			// to do
		case EFFECT_HEAL_TARGET:
		AI_HEALTARGET:
			// to do 
		case EFFECT_TOPSY_TURVY_ELECTRIFY:
		AI_TOPSYTURVYELECTRIFY:
			// to do
		case EFFECT_TEAM_EFFECTS:
		AI_TEAMEFFECTS:
			// to do
		case EFFECT_Synchronoise:	// to do
		AI_SYNCHRONOISE:
			// to do
		
		default:
		AI_STANDARD_DAMAGE:
			if (moveSplit != SPLIT_STATUS) {
				if (TypeCalc(move, bankAtk, bankDef, 0, FALSE) & (RESULT_NO_EFFECT | RESULT_MISSED))
					viability -= 10;
			}
	}
	return viability;
}