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
	
	//Powder Check
	if ((defEffect == ITEM_EFFECT_SAFETY_GOGGLES || IsOfType(bankDef, TYPE_GRASS || defAbility == ABILITY_OVERCOAT))
	&& CheckMoveInTable(move, PowderTable))
		viability -= 10; //No return b/c could be reduced further by absorb abilities
	
	//Target Ability Check
	if (NO_MOLD_BREAKERS(bankAtk) || !CheckMoveInTable(move, MoldBreakerMovesTable)) {
		switch (defAbility) {
			case ABILITY_VOLTABSORB:
				if (moveType == TYPE_ELECTRIC)
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
			
			case ABILITY_FLASHFIRE:
				if (moveType == TYPE_FIRE)
					return viability - 20;
				break;
			
			case ABILITY_SAPSIPPER:
				if (moveType == TYPE_GRASS)
					return viability - 20;
				break;
			
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
				if (moveEffect == EFFECT_SET_SLEEP || moveEffect == EFFECT_SET_YAWN)
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
			
			case ABILITY_JUSTIFIED:
				if (moveType == TYPE_DARK)
					return viability - 10;
				break;
			
			case ABILITY_WONDERSKIN:
				if (moveSplit == SPLIT_STATUS)
					moveAcc = 50;
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
					if (moveEffect == EFFECT_SET_SLEEP || moveEffect == EFFECT_SET_YAWN)
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
	if (CheckGrounding(bankDef) {
		switch (TerrainType) {
			case ELECTRIC_TERRAIN:
				if (moveEffect == EFFECT_SET_SLEEP || moveEffect == EFFECT_SET_YAWN)
					return viability - 10;
				break;
				
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
			
	//Gravity Check
	if (gNewBS->GravityTimer && CheckMoveInTable(move, GravityTable))
		return 0; //Can't select this move period
			
	//Throat Chop Check
	if (gNewBS->ThroatChopTimers[bankAtk] && CheckSoundMove(move))
		return 0; //Can't select this move period
			
	//Heal Block Check
	if (gNewBS->HealBlockTimers[bankAtk] && CheckHealingMove(move))
		return 0; //Can't select this move period
		
	//Primal Weather Check
	if (gBattleWeather & WEATHER_SUN_PRIMAL && moveType == TYPE_WATER)
		return viability - 10;
	else if (gBattleWeather & WEATHER_RAIN_PRIMAL && moveType == TYPE_FIRE)
		return viability - 10;
	
	MOVESCR_CHECK_0:
	switch (moveEffect) {
		case EFFECT_SetSleep:
		AI_SET_SLEEP:
			if (!CanBePutToSleep(bankDef, bankAtk)
			|| !CanBeGeneralStatused(bankDef)
			|| MoveBlockedBySubstitute(move, bankAtk, bankDef))
				viability -= 10;
			break;
			
		case EFFECT_DrainHP:
			if (abilityDef == ABILITY_LIQUIDOOZE)
				viability -= 6;
			if (move == MOVE_STRENGTHSAP && gBattleMons[bankDef].statstages[STAT_STAGE_ATK-1] == 0)
				viability -= 10;
			break;
			
		case EFFECT_Explosion:
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
		
		case EFFECT_DreamEater:
			if (!(defAbility == ABILITY_COMATOSE || defStatus1 & STATUS_SLEEP))
				viability -= 10;
			break;
			
		case EFFECT_MirrorMove: //May cause issues with priority calcs
			if (lastTakenMoveFrom[bankAtk][bankDef]) {
				move = lastTakenMoveFrom[bankAtk][bankDef];
				goto RESTART_AI_SCRIPT_0;
			}
		
		case EFFECT_Splash:
		case EFFECT_Teleport:
			viability -= 10;
			break;
		
		case EFFECT_UserAtk+1:
		case EFFECT_UserAtk+2:
			if (StatMaxed(bankAtk, STAT_STAGE_ATK))
				viability -= 10;
			break;
		
		case EFFECT_UserDef+1:
		case EFFECT_UserDef+2:
		case EFFECT_DefenseCurl:
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
		
		case EFFECT_UserSpd+1:
		case EFFECT_UserSpd+2:
			if (StatMaxed(bankAtk, STAT_STAGE_SPD))
				viability -= 10;
			break;
		
		case EFFECT_UserSpAtk+1: AI_Minus_10_SpAtk_Max
		case EFFECT_UserSpAtk+2: AI_Minus_10_SpAtk_Max
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
			
		case EFFECT_UserSpDef+1:
		case EFFECT_UserSpDef+2:
		AI_SPDEF_RAISE_1:
			if (StatMaxed(bankAtk, STAT_STAGE_SPDEF))
				viability -= 10;
			break;
		
		case EFFECT_UserAcc+1:
		case EFFECT_UserAcc+2:
			if (StatMaxed(bankAtk, STAT_STAGE_ACC))
				viability -= 10;
			break;
		
		case EFFECT_UserEvsn+1:
		case EFFECT_UserEvsn+2:
		case EFFECT_Minimize:
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
		
		case EFFECT_TargetAtk-1: AI_Minus_10_Atk_Min
		case EFFECT_TargetAtk-2: AI_Minus_10_Atk_Min
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
			
		case EFFECT_TargetDef-1: AI_Minus_10_Def_Min
		case EFFECT_TargetDef-2: AI_Minus_10_Def_Min
			if (StatMinned(bankDef, STAT_STAGE_DEF))
				viability -= 10;
			else if (MoveBlockedBySubstitute(move, bankAtk, bankDef))
				viability -= 10;
			break;
		
		case EFFECT_TargetSpd-1: AI_Minus_10_Spd_Min
		case EFFECT_TargetSpd-2: AI_Minus_10_Spd_Min
			if (StatMinned(bankDef, STAT_STAGE_SPD))
				viability -= 10;
			else if (MoveBlockedBySubstitute(move, bankAtk, bankDef))
				viability -= 10;
			break;
		
		case EFFECT_TargetSpAtk-1: AI_Minus_10_SpAtk_Min
		case EFFECT_TargetSpAtk-2: AI_Minus_10_SpAtk_Min
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
		
		case EFFECT_TargetSpDef-1: AI_Minus_10_SpDef_Min
		case EFFECT_TargetSpDef-2: AI_Minus_10_SpDef_Min
			if (StatMinned(bankDef, STAT_STAGE_SPDEF))
				viability -= 10;
			else if (MoveBlockedBySubstitute(move, bankAtk, bankDef))
				viability -= 10;
			break;
		
		case EFFECT_TargetAcc-1: AI_Minus_10_Acc_Min
		case EFFECT_TargetAcc-2: AI_Minus_10_Acc_Min
			if (StatMinned(bankDef, STAT_STAGE_ACC))
				viability -= 10;
			else if (MoveBlockedBySubstitute(move, bankAtk, bankDef))
				viability -= 10;
			break;
		
		case EFFECT_TargetEvsn-1: AI_Minus_10_Evsn_Min
		case EFFECT_TargetEvsn-2: AI_Minus_10_Evsn_Min
			if (StatMinned(bankDef, STAT_STAGE_EVSN))
				viability -= 10;
			else if (MoveBlockedBySubstitute(move, bankAtk, bankDef))
				viability -= 10;
			break;
		
		case EFFECT_Haze: //Clear smog here?
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
			
		case EFFECT_Roar:
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
			
		case EFFECT_Conversion:
			if (IsOfType(bankAtk, gBattleMoves[gBattleMons[bankAtk].move1].type))
				viability -= 10;
			break;
		
		case EFFECT_Recovery:
		case EFFECT_Rest:
		case EFFECT_MorningSun:
		case EFFECT_Synthesis:
		case EFFECT_Moonlight:
		case EFFECT_Softboiled:
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
		
		case EFFECT_PoisonTarget:
		case EFFECT_ToxicPoisonTarget:
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
		
		case EFFECT_LightScreen:
			if (gSideStatuses[bankAtk & 1] & SIDE_STATUS_LIGHT_SCREEN)
				viability -= 10;
			break;
		
		case EFFECT_0HKO:
			if (TypeCalc(move, bankAtk, bankDef...) & (RESULT_NO_EFFECT | RESULT_MISSED)
			|| (NO_MOLD_BREAKERS(bankAtk) && defAbility == ABILITY_STURDY)
				viability -= 10;
			break;
		
		case EFFECT_JumpKick:
			if (atkAbility == ABILITY_MAGICGUARD)
				goto AI_STANDARD_DAMAGE;
			if (moveAcc < 75)
				viability -= 6;
			break;
				
		case EFFECT_Mist:
			if (gSideStatuses[bankAtk & 1] & SIDE_STATUS_MIST)
				viability -= 10;
			break;
		
		case EFFECT_1/4Recoil:
			if (atkAbility == ABILITY_MAGICGUARD || atkAbility == ABILITY_ROCKHEAD)
				goto AI_STANDARD_DAMAGE;
			if (GetHealthPercentage(bankAtk) < 10)
				viability -= 4;
			break;
			
		case EFFECT_FocusEnergy:
			if (defStatus2 & STATUS2_FOCUS_ENERGY)
				viability -= 10;
			break;
		
		case EFFECT_ConfuseTarget:
		AI_CONFUSE:
			if (defStatus2 & STATUS2_CONFUSE 
			|| (NO_MOLD_BREAKERS(bankAtk) && defAbility == ABILITY_OWNTEMPO)
				viability -= 10;
			else if (defEffect == ITEM_EFFECT_MENTAL_HERB)
				viability -= 6;
			break;

		case EFFECT_Transform:
			if (atkStatus2 & STATUS2_TRANSFORMED
			||  defStatus2 & (STATUS2_TRANSFORMED | STATUS2_SUBSTITUTE))
				viability -= 10;
			
		case EFFECT_Reflect: AI_Reflect
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
			
		case EFFECT_ParalyzeTarget:
			if (!CanBeParalyzed(bankDef, bankAtk)
			|| MoveBlockedBySubstitute(move, bankAtk, bankDef))
				viability -= 10;
			else {
				if (move == MOVE_THUNDERWAVE
				&& TypeCalc(move, bankAtk, bankDef...) & (RESULT_NO_EFFECT | RESULT_MISSED))
					viability -= 10;
			}
			break;
			
		case EFFECT_Substitute:
			if (defStatus2 & STATUS2_SUBSTITUTE
			|| GetHealthPercentage(bankAtk) <= 25)
				viability -= 10;
			
			else if (ABILITY(GetFoeBank(bankAtk)) == ABILITY_INFILTRATOR 
			|| (gBattleTypeFlags & BATTLE_TYPE_DOUBLE && ABILITY(PARTNER(GetFoeBank(bankAtk))) == ABILITY_INFILTRATOR)
				viability -= 8; //Not really much point in making a sub if the foe is gonna bust right through it
			break;
			
		case EFFECT_Mimic:
			//Put some last used move logic here
			break;
		
		case EFFECT_Metronome:
			break;
		
		case EFFECT_LeechSeed:
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
		
		case EFFECT_Disable:
			//Check Disable Timer
			//Check Last Used Move
			if (defEffect == ITEM_EFFECT_MENTAL_HERB)
				viability -= 6;
			break;
			
		case EFFECT_Counter:
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
		
		case EFFECT_Encore:
			//Check Encore Timer
			//Check Last Used Move
			if (defEffect == ITEM_EFFECT_MENTAL_HERB)
				viability -= 6;
			break;
			
		case EFFECT_PainSplit:
			if (gBattleMons[bankAtk].hp > (gBattleMons[bankAtk].hp + gBattleMons[bankDef].hp) / 2)
				viability -= 10;
			break;
		
		case EFFECT_Snore:
		case EFFECT_SleepTalk:
			if (!(defStatus1 & STATUS_SLEEP) && defAbility != ABILITY_COMATOSE)
				viability -= 10;
			break;
		
		case EFFECT_Conversion2:
			// To do
		case EFFECT_LockOn:
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
		
		case EFFECT_DestinyBond:
			if (gLastUsedMoves[bankAtk] == move
			|| atkStatus2 & STATUS2_DESTINY_BOND)
				viability -= 10;
			break;
		
		case EFFECT_Spite:
			//Spite Logic
			break;
		
		case EFFECT_FalseSwipe:
			if (MoveKnocksOut(move, bankAtk, bankDef))
				viability -= 10;
			break;
		
		case EFFECT_Aromatherapy:
			if (!PartyMemberStatused(bankAtk))
				viability -= 10;
			break;
		
		case EFFECT_MeanLook:
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
		
		case EFFECT_Nightmare:
			if (defStatus2 & STATUS2_NIGHTMARE
			|| !(defStatus1 & STATUS_SLEEP || defAbility == ABILITY_COMATOSE)) 
				viability -= 10;
			break;
		
		case EFFECT_Curse:
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
			
		case EFFECT_Protect:
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
		case EFFECT_EntryHazards:
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
			
		case EFFECT_Foresight:
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
			
		case EFFECT_PerishSong:
			if (gStatuses3[GetFoeBank(bankAtk)] & STATUS3_PERISH_SONG)
			&& !(gBattleTypeFlags & BATTLE_TYPE_DOUBLE && !(gStatuses3[PARTNER(GetFoeBank(bankAtk))] & STATUS3_PERISH_SONG)))
				viability -= 10;
			break;
			
		case EFFECT_Sandstorm:
			if (gBattleWeather & (WEATHER_SANDSTORM_ANY | WEATHER_PRIMAL))
				viability -= 10;
			break;
		
		case EFFECT_Endure:
			if (gBattleMons[bankAtk].hp == 1) {
				viability -= 10;
				break;
			}
			
			if (GetProtectActivity(bankAtk) > 0) {
				if (umodsi(Random(), 100) < 50)
					viability -= 6;
			}
			break;
			
		case EFFECT_Swagger:
			if (bankDef == bankAtkPartner)
			&& StatMaxed(bankDef, STAT_STAGE_ATK))
				viability -= 10;
			else
				goto AI_CONFUSE;
			break;
		
		case EFFECT_Attract: AI_Attract

		case EFFECT_Safeguard: AI_Safeguard
		case EFFECT_BatonPass: AI_BatonPass
		case EFFECT_RapidSpin: AI_RapidSpin
		

		case EFFECT_Rain:
			if (gBattleWeather & (WEATHER_RAIN_ANY | WEATHER_PRIMAL))
				viability -= 10;
			break;
		
		case EFFECT_Sun:
			if (gBattleWeather & (WEATHER_SUN_ANY | WEATHER_PRIMAL))
				viability -= 10;
			break;
		
		case EFFECT_BellyDrum:
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
			
		case EFFECT_PsychUp:
			switch (move) {
				case MOVE_SPECTRALTHIEF:
					goto AI_STANDARD_DAMAGE;
					
				default: //Psych Up
					goto HAZE_CHECK;
			}
			break;
			
		case EFFECT_FutureAttack:
			if (/*Check Future Sight Timer*/)
				viability -= 10;
			else
				goto AI_STANDARD_DAMAGE;
			break;
			
		case EFFECT_FakeOut: AI_FakeOut
			if (/*Not First Turn*/)
				viability -= 10;
			else
				goto AI_STANDARD_DAMAGE;
			break;
			
		case EFFECT_Stockpile:
			if (/*StockpileCounter(bankAtk) >= 3*/)
				viability -= 10;
			break;
		
		case EFFECT_Swallow:
			if (/*StockpileCounter(bankAtk) == 0*/)
				viability -= 10;
			else
				goto AI_RECOVERY;
			break;
			
		case EFFECT_SpitUp:
			if (/*StockpileCounter(bankAtk) == 0*/)
				viability -= 10;
			else
				goto AI_STANDARD_DAMAGE;
			break;
		
		case EFFECT_Hail:
			if (gBattleWeather & (WEATHER_HAIL_ANY | WEATHER_PRIMAL))
				viability -= 10;
			break;

		case EFFECT_Torment:
			if (defStatus2 & STATUS2_TORMENT) {
				viability -= 10;
				break;
			}
			if (defEffect == ITEM_EFFECT_MENTAL_HERB)
				viability -= 6;
			break;
		
		case EFFECT_Flatter:
			if (bankDef == bankAtkPartner)
			&& StatMaxed(bankDef, STAT_STAGE_SPATK))
				viability -= 10;
			else
				goto AI_CONFUSE;
			break;
		
		case EFFECT_SetBurn:
			If (!CanBeBurned(bankDef, bankAtk)
			|| MoveBlockedBySubstitute(move, bankAtk, bankDef))
				viability -= 10;
			break;
		
		case EFFECT_Memento:
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
		
		case EFFECT_Charge:
			if (atkStatus3 & STATUS3_CHARGED_UP) {
				viability -= 10;
				break;
			}
			if (!MoveTypeInMoveset(bankAtk, TYPE_ELECTRIC))
				goto AI_SPDEF_RAISE_1;
			break;
			
		case EFFECT_Taunt:
			if (/*TauntTimer(bankDef)*/)
				viability -= 1;
			break;
		
		case EFFECT_FollowMe:
		case EFFECT_HelpingHand:
			if (!(gBattleTypeFlags & BATTLE_TYPE_DOUBLE))
				viability -= 10;
			break;
		
		case EFFECT_Trick:
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
			
		case EFFECT_RolePlay:
			if (atkAbility == defAbility
			|| CheckTableForAbility(defAbility, RolePlayBanList)
			|| CheckTableForAbility(defAbility, BadAbilities))
				viability -= 10;
			break;
		
		case EFFECT_Wish:
			(/*If WishCounter(bankAtk) != 0*/)
				viability -= 10;
			break;
		
		case EFFECT_Assist:
			if (CountPokemonOnTeam(bankAtk) == 1)
				viability -= 10;
			break;
		
		case EFFECT_Ingrain:
			if (atkStatus3 & STATUS3_ROOTED)
				viability -= 10;
			break;

		case EFFECT_MagicCoat:
			if (!MagicCoatableMovesInMoveset(bankDef))
				viability -= 10;
			break;
			
		case EFFECT_Recycle:
			if (!gNewBS->SavedConsumedItems[bankAtk] || atkItem != 0)
				viability -= 10;
			break;
		
		case EFFECT_SetYawn:
			if (defStatus3 & STATUS3_DROWSY)
				viability -= 10;
			else
				goto AI_SET_SLEEP;
			break;
			
		case EFFECT_Endeavor: AI_PainSplit
		case EFFECT_SkillSwap: AI_SkillSwap
		case EFFECT_Imprison: AI_Imprison
		case EFFECT_Refresh: AI_Refresh
		case EFFECT_Grudge: AI_BatonPass
		case EFFECT_Snatch: Return
		case EFFECT_1/3Recoil: AI_Recoil
		case EFFECT_TeeterDance: AI_TeeterDance
		case EFFECT_MudSport: AI_MudSport
		case EFFECT_Tickle: AI_Tickle
		case EFFECT_CosmicPower: AI_CosmicPower
		case EFFECT_BulkUp: AI_BulkUp
		case EFFECT_WaterSport: AI_WaterSport
		case EFFECT_CalmMind: AI_CalmMind
		case EFFECT_DragonDance: AI_DragonDance

		case EFFECT_StatSwapperSplitter: Return
		case EFFECT_StoredPower: AI_StoredPower
		case EFFECT_FrostBreath: AI_FrostBreath
		case EFFECT_NaturalGift_MeFirst: AI_NaturalGift_MeFirst
		case EFFECT_ClearSmog: AI_ClearSmog
		case EFFECT_SpeedBalls: AI_Speed_Balls
		case EFFECT_Terrain: AI_Terrain
		case EFFECT_FieldEffects: AI_FieldEffects
		case EFFECT_Fling: AI_Fling
		case EFFECT_AttackBlockers: AI_AttackBlockers
		case EFFECT_TypeChangers: AI_TypeChangers
		case EFFECT_HealTarget: AI_HealTarget
		case EFFECT_TopsyTurvyElectrify: AI_TopsyTurvyElectrify
		case EFFECT_TeamEffects: AI_TeamEffects
		case EFFECT_Synchronoise: AI_Synchronoise
		
		default:
		AI_STANDARD_DAMAGE:
			if (moveSplit != SPLIT_STATUS) {
				if (TypeCalc(move, bankAtk, bankDef...) & (RESULT_NO_EFFECT | RESULT_MISSED))
					viability -= 10;
			}
	}
	return viability;
}