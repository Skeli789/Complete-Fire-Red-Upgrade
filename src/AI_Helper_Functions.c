#include "defines.h"
#include "helper_functions.h"

extern move_t MinimizeHitTable[];
extern const struct SpecialZMoves SpecialZMoveTable[];

extern s8 PriorityCalc(u8 bank, u8 action, u16 move);
extern s32 BracketCalc(u8 bank);
extern u32 SpeedCalc(u8 bank);
extern u8 CheckMoveLimitations(u8 bank, u8 unusableMoves, u8 check);
extern u32 AI_CalcDmg(u8 bankAtk, u8 bankDef, u16 move);

bool8 CanKillAFoe(u8 bank);
bool8 CanKnockOut(u8 bankAtk, u8 bankDef);
bool8 MoveKnocksOut(u16 move, u8 bankAtk, u8 bankDef);
bool8 IsStrongestMove(u16 currentMove, u8 bankAtk, u8 bankDef);
bool8 MoveWillHit(u16 move, u8 bankAtk, u8 bankDef);
bool8 MoveWouldHitFirst(u16 move, u16 bankAtk, u16 bankDef);
bool8 DamagingMoveInMoveset(u8 bank);
bool8 PhysicalMoveInMoveset(u8 bank);
bool8 SpecialMoveInMoveset(u8 bank);
bool8 MagicCoatableMovesInMoveset(u8 bank);
bool8 GetHealthPercentage(u8 bank);
bool8 TeamFullyHealedMinusBank(u8 bank);
bool8 HasProtectionMoveInMoveset(u8 bank, bool8 AllKinds);
move_t ShouldAIUseZMove(u8 bank, u8 moveIndex, u16 move);

bool8 CanKillAFoe(u8 bank) {
	u8 foe = FOE(bank);
	if (!(gAbsentBattlerFlags & gBitTable[foe]) && CanKnockOut(bank, foe))
		return TRUE;
	
	if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE && !(gAbsentBattlerFlags & gBitTable[PARTNER(foe)]))
		return CanKnockOut(bank, PARTNER(foe));
	
	return FALSE;
}

bool8 CanKnockOut(u8 bankAtk, u8 bankDef)
{
	u16 move;
	int i;

	if (gAbsentBattlerFlags & gBitTable[bankAtk])
		return FALSE;

	u8 moveLimitations = CheckMoveLimitations(bankAtk, 0, 0xFF);
	
	for (i = 0; i < 4; ++i) {
		#ifdef REALLY_SMART_AI
			move = gBattleMons[bankAtk].moves[i];
		#else
			if (SIDE(bankAtk) == B_SIDE_PLAYER && !(gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER))
				move = BATTLE_HISTORY->usedMoves[bankAtk].moves[i];
			else if (SIDE(bankAtk) == B_SIDE_PLAYER && gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER && GetBattlerPosition(bankAtk) == B_POSITION_PLAYER_LEFT)
				move = BATTLE_HISTORY->usedMoves[bankAtk].moves[i];
			else
				move = gBattleMons[bankAtk].moves[i];
		#endif
		
		if (move == MOVE_NONE)
			break;
		
		if (!(gBitTable[i] & moveLimitations)) {
			if (MoveKnocksOut(move, bankAtk, bankDef))
				return TRUE;
		}
	}
	return FALSE;
}

bool8 MoveKnocksOut(u16 move, u8 bankAtk, u8 bankDef) {
	u16 defHP = gBattleMons[bankDef].hp;
	
	if (SPLIT(move) == SPLIT_STATUS)
		return FALSE;
	
	if (gBattleMoves[move].effect == EFFECT_0HKO) {
		if (gBattleMons[bankAtk].level <= gBattleMons[bankDef].level)
			return FALSE;
		if (move == MOVE_SHEERCOLD && IsOfType(bankDef, TYPE_ICE))
			return FALSE;
		return TRUE;
	}
	
	if (gBattleMoves[move].effect == EFFECT_COUNTER || gBattleMoves[move].effect == EFFECT_MIRROR_COAT) //Includes Metal Burst
		return FALSE;
	
	if (gBattleMoves[move].power == 0)
		return FALSE;

	if (AI_CalcDmg(bankAtk, bankDef, move) >= defHP)
		return TRUE;
	
	return FALSE;
}

bool8 IsStrongestMove(u16 currentMove, u8 bankAtk, u8 bankDef) {
	u16 move;
	u16 strongestMove = gBattleMons[bankAtk].moves[0];
	s32 highestDamage = -1;
	int i;
	
	u8 moveLimitations = CheckMoveLimitations(bankAtk, 0, 0xFF);
	
	for (i = 0; i < 4; ++i) {
		move = gBattleMons[bankAtk].moves[i];
		if (move == MOVE_NONE)
			break;
		
		if (!(gBitTable[i] & moveLimitations)) {
			
			if (SPLIT(move) == SPLIT_STATUS)
				continue;
			
			if (gBattleMoves[move].power == 0)
				continue;
			
			if (gBattleMoves[move].effect == EFFECT_COUNTER || gBattleMoves[move].effect == EFFECT_MIRROR_COAT) //Includes Metal Burst
				continue;
	
			else if (gBattleMoves[move].effect == EFFECT_0HKO) {
				if (gBattleMons[bankAtk].level <= gBattleMons[bankDef].level)
					continue;
				if (move == MOVE_SHEERCOLD && IsOfType(bankDef, TYPE_ICE))
					continue;
				if (MoveWillHit(move, bankAtk, bankDef)) {
					strongestMove = move;
					highestDamage = 0x7FFFFFFF;
					
					if (currentMove == strongestMove)
						return TRUE;
					//Keep going because user may have another 0HKO move to be checked
				}
			}
			
			else if (AI_CalcDmg(bankAtk, bankDef, move) > (u32) highestDamage) 
			{
				highestDamage = AI_CalcDmg(bankAtk, bankDef, move);
				strongestMove = move;
			}
		}
	}
	
	if (strongestMove == currentMove)
		return TRUE;
	
	return FALSE;
}

bool8 MoveWillHit(u16 move, u8 bankAtk, u8 bankDef) {
	u8 moveEffect = gBattleMoves[move].effect;
	
	#ifdef REALLY_SMART_AI
		u8 defAbility = BATTLE_HISTORY->abilities[bankDef];
	#else
		u8 defAbility = ABILITY(bankDef);
	#endif
	
	if (ABILITY(bankAtk) == ABILITY_NOGUARD 
	||  defAbility == ABILITY_NOGUARD
	||  (gStatuses3[bankDef] & STATUS3_ALWAYS_HITS && gDisableStructs[bankDef].bankWithSureHit == bankAtk))
		return TRUE;
	
	if (((gStatuses3[bankDef] & (STATUS3_IN_AIR | STATUS3_SKY_DROP_ATTACKER | STATUS3_SKY_DROP_TARGET)) && moveEffect != EFFECT_THUNDER && moveEffect != EFFECT_TWISTER) //Includes Hurricane
    ||  ((gStatuses3[bankDef] & STATUS3_UNDERGROUND) && moveEffect != EFFECT_EARTHQUAKE && move != EFFECT_MAGNITUDE)
    ||  ((gStatuses3[bankDef] & STATUS3_UNDERWATER) && move != MOVE_SURF)
    ||   (gStatuses3[bankDef] & STATUS3_DISAPPEARED))
		return FALSE;
		
	if ((move == MOVE_TOXIC && IsOfType(bankAtk, TYPE_POISON))
	||  (CheckTableForMove(move, MinimizeHitTable) && gStatuses3[bankDef] & STATUS3_MINIMIZED)
	|| ((gStatuses3[bankDef] & STATUS3_TELEKINESIS) && gBattleMoves[move].effect != EFFECT_0HKO)
	||  gBattleMoves[move].accuracy == 0
	|| (WEATHER_HAS_EFFECT && (gBattleWeather & WEATHER_RAIN_ANY) && gBattleMoves[move].effect == EFFECT_THUNDER))
		return TRUE;
	
	return FALSE;
}

bool8 MoveWouldHitFirst(u16 move, u16 bankAtk, u16 bankDef) {
	u32 temp;
	u32 bankAtkSpeed, bankDefSpeed;

//Priority Calc
	if (PriorityCalc(bankAtk, TRUE, move) > 0)
		return TRUE;
	
//BracketCalc
	if (BracketCalc(bankAtk) > BracketCalc(bankDef))
		return TRUE;
	
//SpeedCalc
	bankAtkSpeed = SpeedCalc(bankAtk);
	bankDefSpeed = SpeedCalc(bankDef);
	if (TrickRoomTimer) {
		temp = bankDefSpeed;
		bankDefSpeed = bankAtkSpeed;
		bankAtkSpeed = temp;
	}
	
	if (bankAtkSpeed > bankDefSpeed)
		return TRUE;
		
	return FALSE;
}

bool8 DamagingMoveInMoveset(u8 bank) {
	u16 move;
	u8 moveLimitations = CheckMoveLimitations(bank, 0, 0xFF);
	
	for (int i = 0; i < 4; ++i) {
		#ifdef REALLY_SMART_AI
			move = gBattleMons[bank].moves[i];
		#else
			if (SIDE(bank) == B_SIDE_PLAYER && !(gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER))
				move = BATTLE_HISTORY->usedMoves[bank].moves[i];
			else if (SIDE(bank) == B_SIDE_PLAYER && gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER && GetBattlerPosition(bank) == B_POSITION_PLAYER_LEFT)
				move = BATTLE_HISTORY->usedMoves[bank].moves[i];
			else
				move = gBattleMons[bank].moves[i];
		#endif
		
		if (move == MOVE_NONE)
			break;
		
		if (!(gBitTable[i] & moveLimitations)) {
			if (SPLIT(move) != SPLIT_STATUS
			&& gBattleMoves[move].power != 0
			&& gBattleMoves[move].effect != EFFECT_COUNTER
			&& gBattleMoves[move].effect != EFFECT_MIRROR_COAT)
				return TRUE;
		}
	}
	return FALSE;
}

bool8 PhysicalMoveInMoveset(u8 bank) {
	u16 move;
	u8 moveLimitations = CheckMoveLimitations(bank, 0, 0xFF);
	
	for (int i = 0; i < 4; ++i) {
		#ifdef REALLY_SMART_AI
			move = gBattleMons[bank].moves[i];
		#else
			if (SIDE(bank) == B_SIDE_PLAYER && !(gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER))
				move = BATTLE_HISTORY->usedMoves[bank].moves[i];
			else if (SIDE(bank) == B_SIDE_PLAYER && gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER && GetBattlerPosition(bank) == B_POSITION_PLAYER_LEFT)
				move = BATTLE_HISTORY->usedMoves[bank].moves[i];
			else
				move = gBattleMons[bank].moves[i];
		#endif
		
		if (move == MOVE_NONE)
			break;

		if (!(gBitTable[i] & moveLimitations)) {
			if (SPLIT(move) == SPLIT_PHYSICAL
			&& gBattleMoves[move].power != 0
			&& gBattleMoves[move].effect != EFFECT_COUNTER)
				return TRUE;
		}
	}
	return FALSE;
}

bool8 SpecialMoveInMoveset(u8 bank) {
	u16 move;
	u8 moveLimitations = CheckMoveLimitations(bank, 0, 0xFF);
	
	for (int i = 0; i < 4; ++i) {
		#ifdef REALLY_SMART_AI
			move = gBattleMons[bank].moves[i];
		#else
			if (SIDE(bank) == B_SIDE_PLAYER && !(gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER))
				move = BATTLE_HISTORY->usedMoves[bank].moves[i];
			else if (SIDE(bank) == B_SIDE_PLAYER && gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER && GetBattlerPosition(bank) == B_POSITION_PLAYER_LEFT)
				move = BATTLE_HISTORY->usedMoves[bank].moves[i];
			else
				move = gBattleMons[bank].moves[i];
		#endif
		
		if (move == MOVE_NONE)
			break;
		
		if (!(gBitTable[i] & moveLimitations)) {
			if (SPLIT(move) == SPLIT_SPECIAL
			&& gBattleMoves[move].power != 0
			&& gBattleMoves[move].effect != EFFECT_MIRROR_COAT)
				return TRUE;
		}
	}
	return FALSE;
}

bool8 MagicCoatableMovesInMoveset(u8 bank) {
	u16 move;
	u8 moveLimitations = CheckMoveLimitations(bank, 0, 0xFF);
	
	for (int i = 0; i < 4; ++i) {
		#ifdef REALLY_SMART_AI
			move = gBattleMons[bank].moves[i];
		#else
			if (SIDE(bank) == B_SIDE_PLAYER && !(gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER))
				move = BATTLE_HISTORY->usedMoves[bank].moves[i];
			else if (SIDE(bank) == B_SIDE_PLAYER && gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER && GetBattlerPosition(bank) == B_POSITION_PLAYER_LEFT)
				move = BATTLE_HISTORY->usedMoves[bank].moves[i];
			else
				move = gBattleMons[bank].moves[i];
		#endif
		
		if (move == MOVE_NONE)
			break;
		
		if (!(gBitTable[i] & moveLimitations)) {
			if (gBattleMoves[move].flags & FLAG_MAGIC_COAT_AFFECTED)
				return TRUE;
		}
	}
	return FALSE;
}

bool8 GetHealthPercentage(u8 bank) {
	return udivsi(gBattleMons[bank].hp * 100, gBattleMons[bank].maxHP);
}

bool8 TeamFullyHealedMinusBank(u8 bank) {
	u8 firstId, lastId;
	
	pokemon_t* party = LoadPartyRange(bank, &firstId, &lastId);
	
    for (int i = firstId; i < lastId; ++i) {
        if (pokemon_getattr(&party[i], REQ_SPECIES) == PKMN_NONE
        || pokemon_getattr(&party[i], REQ_EGG)
		|| i == gBattlerPartyIndexes[bank])
			continue;
			
		if (party[i].hp < party[i].maxHP)
			return FALSE;
    }
	return TRUE;
}

bool8 HasProtectionMoveInMoveset(u8 bank, bool8 AllKinds) {
	u16 move;
	u8 moveLimitations = CheckMoveLimitations(bank, 0, 0xFF);
	
	for (int i = 0; i < 4; ++i) {
		#ifdef REALLY_SMART_AI
			move = gBattleMons[bank].moves[i];
		#else
			if (SIDE(bank) == B_SIDE_PLAYER && !(gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER))
				move = BATTLE_HISTORY->usedMoves[bank].moves[i];
			else if (SIDE(bank) == B_SIDE_PLAYER && gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER && GetBattlerPosition(bank) == B_POSITION_PLAYER_LEFT)
				move = BATTLE_HISTORY->usedMoves[bank].moves[i];
			else
				move = gBattleMons[bank].moves[i];
		#endif
		
		if (move == MOVE_NONE)
			break;
		
		if (!(gBitTable[i] & moveLimitations)) {
			if (gBattleMoves[gBattleMons[bank].moves[i]].effect == EFFECT_PROTECT) {
				switch (move) {
					case MOVE_PROTECT:
					case MOVE_SPIKYSHIELD:
					case MOVE_KINGSSHIELD:
						return TRUE;
					
					case MOVE_QUICKGUARD:
					case MOVE_WIDEGUARD:
					case MOVE_CRAFTYSHIELD:
					case MOVE_MATBLOCK:
						if (AllKinds)
							return TRUE;
				}
			}
		}
	}
	return FALSE;
}

u16 ShouldAIUseZMove(u8 bank, u8 moveIndex, u16 move) {
	int i;
	if (move == 0)
		move = gBattleMons[bank].moves[moveIndex];
		
	if (MegaData->partyIndex[SIDE(bank)] & gBitTable[gBattlerPartyIndexes[bank]])
		return FALSE;
	
	if (gItems[SanitizeItemId(gBattleMons[bank].item)].holdEffect == ITEM_EFFECT_Z_CRYSTAL) {
		for (i = 0; SpecialZMoveTable[i].species != 0xFFFF; ++i) {
			if (SpecialZMoveTable[i].species == gBattleMons[bank].species
			&&	SpecialZMoveTable[i].item == gBattleMons[bank].item
			&&  SpecialZMoveTable[i].move == move)
				return SpecialZMoveTable[i].move;
		}
		
		if (gBattleMoves[move].type == ITEM_QUALITY(bank)) {
			if (SPLIT(move) == SPLIT_STATUS)
				return 0xFFFF;
			else if (gBattleMoves[move].type < TYPE_FIRE)
				return MOVE_BREAKNECK_BLITZ_P + (gBattleMoves[move].type * 2) + SPLIT(move);
			else
				return MOVE_BREAKNECK_BLITZ_P + ((gBattleMoves[move].type - 1) * 2) + SPLIT(move);
		}
	}
	return FALSE;
}