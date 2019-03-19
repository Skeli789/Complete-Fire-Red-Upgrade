#include "..\\defines.h"
#include "AI_Helper_Functions.h"
#include "..\\Helper_Functions.h"

/*

u8 AI_Script_Positives(u8 bankAtk, u8 bankDef, u16 move, u8 viability) {
	//u8 viability = 100;
	// get relevant params
	u8 moveEffect = gBattleMoves[move].effect;
	u8 atkItemEffect = ITEM_EFFECT(bankAtk);
	
	
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
			// to do
			break;
			
		case EFFECT_EXPLOSION:
		case EFFECT_MEMENTO:
			// to do
			break;
			
		case EFFECT_DREAM_EATER:
			// to do
			break;
			
		// increased stat effects
		case EFFECT_ATTACK_UP:
		case EFFECT_ATTACK_UP_2:
		AI_ATTACK_PLUS: ;
			// to do
			break;
			
		case EFFECT_DEFENSE_UP:
		case EFFECT_DEFENSE_UP_2:
		AI_DEFENSE_PLUS: ;
			// to do
			break;
			
		case EFFECT_SPEED_UP:
		case EFFECT_SPEED_UP_2:
		AI_SPEED_PLUS: ;
			// to do
			break;
			
		case EFFECT_SPECIAL_ATTACK_UP:
		case EFFECT_SPECIAL_ATTACK_UP_2:
			// to do
			break;
			
		case EFFECT_SPECIAL_DEFENSE_UP:
		case EFFECT_SPECIAL_DEFENSE_UP_2:
			// to do
			break;
			
		case EFFECT_ACCURACY_UP:
		case EFFECT_ACCURACY_UP_2:
		case EFFECT_MINIMIZE:
			// to do 
			break;
			
		case EFFECT_EVASION_UP:
		case EFFECT_EVASION_UP_2:
			// to do 
			break;
			
		// decreased stat effects 
		case EFFECT_ATTACK_DOWN:
		case EFFECT_ATTACK_DOWN_2:
			// to do
			break;
			
		case EFFECT_DEFENSE_DOWN:
		case EFFECT_DEFENSE_DOWN_2:
			// to do
			break;
			
		case EFFECT_SPEED_DOWN:
		case EFFECT_SPEED_DOWN_2:
			// to do
			break;
			
		case EFFECT_SPECIAL_ATTACK_DOWN:
		case EFFECT_SPECIAL_ATTACK_DOWN_2:
			// to do
			break;
			
		case EFFECT_SPECIAL_DEFENSE_DOWN:
		case EFFECT_SPECIAL_DEFENSE_DOWN_2:
			// to do
			break;
			
		case EFFECT_ACCURACY_DOWN:
		case EFFECT_ACCURACY_DOWN_2:
			// to do
			break;
			
		case EFFECT_EVASION_DOWN:
		case EFFECT_EVASION_DOWN_2:
			// to do
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
	
	return viability;
}

*/