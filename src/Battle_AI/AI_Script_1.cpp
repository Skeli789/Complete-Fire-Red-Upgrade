#include "..\\defines.h"
#include "AI_Helper_Functions.h"
#include "..\\Helper_Functions.h"



u8 AI_Script_Positives(u8 bankAtk, u8 bankDef, u16 move, u8 viability) {
	//u8 viability = 100;
	
	// get relevant params
	u8 moveEffect = gBattleMoves[move].effect;
	u8 bankAtkPartner = PARTNER(bankAtk);
	u8 bankDefPartner = PARTNER(bankDef);
	
	u16 atkItem = ITEM(bankAtk);
	u16 defItem = ITEM(bankDef);
	u8 atkItemEffect = ITEM_EFFECT(bankAtk);
	u8 defItemEffect = ITEM_EFFECT(bankDef);
	u8 atkItemQuality = ITEM_QUALITY(bankAtk);
	
	u8 atkAbility = ABILITY(bankAtk);
	u8 defAbility = ABILITY(bankDef);
	
	u32 atkStatus1 = gBattleMons[bankAtk].status1;
	u32 atkStatus2 = gBattleMons[bankAtk].status2;
	u32 atkStatus3 = gBattleMons[bankAtk].status3;
	u32 defStatus1 = gBattleMons[bankDef].status1;
	u32 defStatus2 = gBattleMons[bankDef].status2;
	u32 defStatus3 = gBattleMons[bankDef].status3;
	
	
	
	switch (moveEffect) {
		case EFFECT_HIT:
			// to do
			break;
		
		case EFFECT_SLEEP:
		case EFFECT_YAWN:
			if (defItemEffect == ITEM_EFFECT_CURE_SLP || defItemEffect == ITEM_EFFECT_CURE_STATUS)
				break;
			else if (defAbility == ABILITY_EARLYBIRD || defAbility == ABILITY_SHEDSKIN)
				break;
			else
				viability += 6;
			break;
			
		case EFFECT_ABSORB:
		AI_DRAIN_HP_CHECK: ;
			if (atkItemEffect == ITEM_EFFECT_BIGROOT || GetHealthPercentage(bankAtk) <= 50)
				viability += 5;
			break;
			
		case EFFECT_EXPLOSION:
		case EFFECT_MEMENTO:
			if (atkItemEffect == ITEM_EFFECT_GEM)
			{
				if (atkItemQuality == 0)
					viability += 6;
				break;
			}
			else if (atkItemEffect == ITEM_EFFECT_CUSTAP_BERRY)
			{
				if (GetHealthPercentage(bankAtk) <= 25)
					viability += 6;
				break;
			}
			break;
			
		case EFFECT_DREAM_EATER:
			if (defAbility == ABILITY_COMATOSE)
				viability += 5;
			else if (defStatus1 & STATUS1_SLEEP)
				viability += 5;
			goto AI_DRAIN_HP_CHECK;
			
		// increased stat effects
		case EFFECT_ATTACK_UP:
		case EFFECT_ATTACK_UP_2:
		AI_ATTACK_PLUS: ;
			if (move == MOVE_HONECLAWS)
			{
				if (STAT_STAGE(bankAtk,STAT_STAGE_ATK) > 7)
					goto AI_ACCURACY_PLUS;
				else if (PhysicalMoveInMoveset(bankAtk))
					viability += 6;
				goto AI_ACCURACY_PLUS;
			}
			else
			{
				if (STAT_STAGE(bankAtk,STAT_STAGE_ATK) > 7)
					break;
				else if (PhysicalMoveInMoveset(bankAtk))
					viability += 6;
				break;
			}
			break;
			
		case EFFECT_DEFENSE_UP:
		case EFFECT_DEFENSE_UP_2:
		AI_DEFENSE_PLUS: ;
			switch (move)
			{
				case MOVE_FLOWERSHIELD:
					if (IsOfType(bankAtkPartner, TYPE_GRASS))
					{
						viability += 5;
						break;
					}
					break;
					
				case MOVE_MAGNETICFLUX:
					if (ABILITY(bankAtkPartner) == ABILITY_PLUS || ABILITY(bankAtkPartner) == ABILITY_MINUS)
					{
						viability += 5;
						break;
					}
					else if (STAT_STAGE(bankDef,STAT_STAGE_ATK) > STAT_STAGE(bankDef,STAT_STAGE_SPATK))
						break;
					else
						goto AI_SPDEF_PLUS;
					
				case MOVE_AROMATICMIST:
					viability += 5;
					break;
			}
			// continue defense check
			if (STAT_STAGE(bankAtk,STAT_STAGE_DEF) > 7)
				break;
			else if (STAT_STAGE(bankDef,STAT_STAGE_ATK) > STAT_STAGE(bankDef,STAT_STAGE_SPATK) && atkAbility != ABILITY_CONTRARY)
				viability += 5;
			break;	
			
		case EFFECT_SPEED_UP:
		case EFFECT_SPEED_UP_2:
		AI_SPEED_PLUS: ;
			if (MoveWouldHitFirst(move, bankAtk, bankDef))
				break;
			else if (atkAbility != ABILITY_CONTRARY)
				viability += 6;
			break;
			
		case EFFECT_SPECIAL_ATTACK_UP:
		case EFFECT_SPECIAL_ATTACK_UP_2:
		AI_SPATK_PLUS: ;
			switch (move)
			{
				case MOVE_WORKUP:
					if (PhysicalMoveInMoveset(bankAtk))
						goto AI_ATTACK_PLUS;
					else if (SpecialMoveInMoveset(bankAtk))
						break;
					goto FUNCTION_RETURN;
					
				case MOVE_ROTOTILLER:
					if (IsOfType(bankAtkPartner,TYPE_GRASS))
						viability += 5;
					else if (PhysicalMoveInMoveset(bankAtk))
						goto AI_ATTACK_PLUS;
					else if (SpecialMoveInMoveset(bankAtk))
						break;
					goto FUNCTION_RETURN;
					
				case MOVE_GEARUP:
					if (ABILITY(bankAtkPartner) == ABILITY_PLUS || ABILITY(bankAtkPartner) == ABILITY_MINUS)
					{
						viability += 5;
						goto FUNCTION_RETURN;
					}
					else if (PhysicalMoveInMoveset(bankAtk))
						goto AI_ATTACK_PLUS;
					else if (SpecialMoveInMoveset(bankAtk))
						break;
					goto FUNCTION_RETURN;
			}
			// continue spatk plus check
			if (STAT_STAGE(bankAtk,STAT_STAGE_SPATK) > 7)
				break;
			else if (SpecialMoveInMoveset(bankAtk) && atkAbility != ABILITY_CONTRARY)
				viability += 6;
			break;
			
		case EFFECT_SPECIAL_DEFENSE_UP:
		case EFFECT_SPECIAL_DEFENSE_UP_2:
		AI_SPDEF_PLUS: ;
			if (STAT_STAGE(bankAtk,STAT_STAGE_SPDEF) > 7)
				break;
			else if (STAT_STAGE(bankDef,STAT_STAGE_SPATK) > STAT_STAGE(bankDef,STAT_STAGE_ATK) && atkAbility != ABILITY_CONTRARY)
				viability += 5;
			break;
			
		case EFFECT_ACCURACY_UP:
		case EFFECT_ACCURACY_UP_2:
		case EFFECT_MINIMIZE:
		AI_ACCURACY_PLUS: ;
			if (STAT_STAGE(bankAtk,STAT_STAGE_ACC) < 6 && atkAbility != ABILITY_CONTRARY)
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
			
		case EFFECT_ROAR:
		case EFFECT_HAZE:
			// to do
			break;
			
		case EFFECT_BIDE:
			// to do
			break;
			
		case EFFECT_RAMPAGE:
			// to do
			break;
			
		case EFFECT_MULTI_HIT:
			// to do
			break;
			
		case EFFECT_FLINCH_HIT:
			// to do
			break;
			
		case EFFECT_RESTORE_HP:
		case EFFECT_MORNING_SUN:
		case EFFECT_SYNTHESIS:
		case EFFECT_MOONLIGHT:
		case EFFECT_SOFTBOILED:
		case EFFECT_SWALLOW:
		case EFFECT_WISH:
			// to do
			break;
			
		case EFFECT_TOXIC:
		case EFFECT_POISON:
			// to do
			break;
			
		case EFFECT_LIGHT_SCREEN:
			// to do
			break;
			
		case EFFECT_REST:
			// to do
			break;
			
		case EFFECT_0HKO:
			// to do
			break;
		
		case EFFECT_SUPER_FANG:
			// to do
			break;
			
		case EFFECT_HIGH_CRITICAL:
			// to do
			break;
			
		case EFFECT_DOUBLE_HIT:
			// to do
			break;
			
		case EFFECT_FOCUS_ENERGY:
			// to do
			break;
			
		case EFFECT_RECOIL:
			// to do
			break;
			
		case EFFECT_CONFUSE:
		case EFFECT_FLATTER:
		case EFFECT_TEETER_DANCE:
			// to do
			break;
			
		case EFFECT_REFLECT:
			// to do
			break;
			
		case EFFECT_PARALYZE:
			// to do
			break;
			
		case EFFECT_SKY_ATTACK:
		case EFFECT_SKULL_BASH:
		case EFFECT_SEMI_INVULNERABLE:
			// to do
			break;
			
		case EFFECT_SUBSTITUTE:
			// to do
			break;
			
		case EFFECT_RECHARGE:
			// to do
			break;
			
		case EFFECT_LEECH_SEED:
			// to do
			break;
			
		case EFFECT_ENCORE:
			// to do
			break;
			
		case EFFECT_SNORE:
		case EFFECT_SLEEP_TALK:
			// to do
			break;
			
		case EFFECT_LOCK_ON:
			// to do
			break;
		
		case EFFECT_DESTINY_BOND:
		case EFFECT_FLAIL:
			// to do
			break;
			
		case EFFECT_HEAL_BELL:
			// to do
			break;
			
		case EFFECT_QUICK_ATTACK:
			// to do
			break;
			
		case EFFECT_MEAN_LOOK:
			// to do
			break;
			
		case EFFECT_NIGHTMARE:
			// to do
			break;
		
		case EFFECT_CURSE:
			// to do
			break;
			
		case EFFECT_PROTECT:
			// to do
			break;
			
		// entry hazards
		case EFFECT_SPIKES:
			// to do
			break;
			
		case EFFECT_FORESIGHT:
			// to do
			break;
			
		case EFFECT_PERISH_SONG;
			// todo
			break;
			
		case EFFECT_SANDSTORM:
			// todo 
			break;
			
		case EFFECT_ENDURE:
			// todo
			break;
		
		case EFFECT_ROLLOUT:
			// to do
			break;
			
		case EFFECT_SWAGGER:
			// to do
			break;
			
		case EFFECT_FURY_CUTTER:
			// to do
			break;
			
		case EFFECT_ATTRACT:
			// to do
			break;
			
		case EFFECT_BATON_PASS:
			// todo
			break;
			
		case EFFECT_RAPID_SPIN:
			//todo
			break;
			
		case EFFECT_RAIN_DANCE:
			//todo
			break;
			
		case EFFECT_SUNNY_DAY:
			//todo
			break;
			
		case EFFECT_ATTACK_UP_HIT:
			//todo
			break;
			
		case EFFECT_BELLY_DRUM:
			//todo
			break;
			
		case EFFECT_PSYCH_UP:
			//todo
			break;
			
		case EFFECT_TWISTER:
		case EFFECT_GUST:
		case EFFECT_THUNDER:
			//todo
			break;
		
		case EFFECT_MAGNITUDE:
		case EFFECT_EARTHQUAKE:
			//todo
			break;
			
		case EFFECT_FLINCH_HIT_2:
			//todo
			break;
			
		case EFFECT_SOLARBEAM:
			//todo
			break;
			
		case EFFECT_DEFENSE_CURL:
			//todo`
			break;
			
		case EFFECT_FAKE_OUT:
			//todo
			break;
			
		case EFFECT_STOCKPILE:
			//todo
			break;
			
		case EFFECT_HAIL:
			//todo
			break;
			
		case EFFECT_WILL_O_WISP:
			//todo
			break;
			
		case EFFECT_FOLLOW_ME:
		case EFFECT_HELPING_HAND:
			//todo
			break;
			
		case EFFECT_TAUNT:
			//todo
			break;
			
		case EFFECT_TRICK:
			//todo
			break;
			
		case EFFECT_INGRAIN:
			//todo
			break;
			
		case EFFECT_RECYCLE:
		case EFFECT_BRICK_BREAK:
			//todo
			break;
			
		case EFFECT_ENDEAVOR:	//flail?
			//todo
			break;
			
		case EFFECT_REFRESH:
			//todo
			break;
			
		case EFFECT_TICKLE:
			//todo
			break;
			
		case EFFECT_COSMIC_POWER:
			//todo
			break;
			
		case EFFECT_BULK_UP:
			//todo;
			break;
			
		case EFFECT_CALM_MIND:
			//todo;
			break;
			
		case EFFECT_DRAGON_DANCE:
			if (move == MOVE_SHELLSMASH)
			{
				if (atkItem == ITEM_EFFECT_WHITE_HERB)
					viability += 6;
				else if (gBattleMons[bankAtk].statStages[STAT_SPECIAL_ATTACK-1] <= 6 && (SpecialMoveInMoveset(bankAtk)))
					viability += 6;
			}
			if (gBattleMons[bankAtk].statStages[STAT_ATTACK-1] > 7)
				goto AI_SPEED_PLUS;
			else if (PhysicalMoveInMoveset(bankAtk))
				viability += 6;
			goto AI_SPEED_PLUS;
			
		
		// other effects ???
		case EFFECT_PLEDGE:
			//todo - if partner going to use same pledge move
			break;
		
		
	}
	FUNCTION_RETURN: ;
	return viability;
}

