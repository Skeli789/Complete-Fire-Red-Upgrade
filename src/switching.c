#include "defines.h"
#include "helper_functions.h"

//Do Illusion hooks

u8* SwitchClearTableBytes[] = {
TelekinesisTimers,
HealBlockTimers,
LaserFocusTimers,
ThroatChopTimers,
StompingTantrumTimers,
MetronomeCounter,
DestinyBondCounters,
SlowStartTimers,
IncinerateCounters,
EmbargoTimers,
IllusionBrokenBytes,
SuppressedAbilities,
(u8*) 0xFEFEFEFE
};

u8* SwitchClearTableBits[] = {
&PowderByte,
&BeakBlastByte,
&UnburdenBoosts,
(u8*) 0xFEFEFEFE
};

enum SwitchInStates {
SwitchIn_HealingWish,
SwitchIn_Spikes,
SwitchIn_StealthRock,
SwitchIn_ToxicSpikes,
SwitchIn_StickyWeb,
SwitchIn_PrimalReversion,
SwitchIn_Truant,
SwitchIn_Abilities,
SwitchIn_Items,
SwitchIn_AirBalloon,
SwitchIn_End
};

extern u8 BattleScript_HealingWishHeal[];
extern u8 BattleScript_LunarDanceHeal[];
extern u8 BattleScript_SpikesHurt[];
extern u8 BattleScript_SRHurt[];
extern u8 BattleScript_TSPoison[];
extern u8 BattleScript_TSHarshPoison[];
extern u8 BattleScript_TSAbsorb[];
extern u8 BattleScript_StickyWebSpeedDrop[];
extern u8 BattleScript_AirBalloonSub[];
extern u8 BattleScript_PrimalWeatherEnd[];
extern u8 BattleScript_SuccessForceOut[];

extern u8 PrimalRainEndString[];
extern u8 PrimalSunEndString[];
extern u8 PrimalAirCurrentEndString[];

extern void TypeDamageModification(u8 atkAbility, u8 bankDef, move_t, u8 move_type, u8* flags);
extern u32 MathMin(u32, u32);
extern void FormRevert(bank_t);
extern u8* DoPrimalReversion(bank_t, u8 caseId);
extern bool8 IsLinkDoubleBattle(void);
extern void SwitchOutFormsRevert(u8 bank);

bool8 TryDoForceSwitchOut(void);
void sub_80571DC(u8 battlerId, u8 arg1);
void SwitchTimerClear(void);
bool8 PPIsMaxed(bank_t);

void atkE2_switchoutabilities(void) {
    gActiveBattler = GetBattleBank(T2_READ_8(gBattlescriptCurrInstr + 1));
	
    switch (ABILITY(gActiveBattler)) {
		case ABILITY_NATURALCURE:
			gBattleMons[gActiveBattler].status1 = 0;
			EmitSetMonData(0, REQUEST_STATUS_BATTLE, gBitTable[gBattleStruct->field_58[gActiveBattler]], 4, &gBattleMons[gActiveBattler].status1);
			MarkBufferBankForExecution(gActiveBattler);
			break;
		case ABILITY_REGENERATOR:
			gBattleMons[gActiveBattler].hp = MathMin(gBattleMons[gActiveBattler].maxHP, gBattleMons[gActiveBattler].hp + udivsi(gBattleMons[gActiveBattler].maxHP, 3));
			EmitSetMonData(0, REQUEST_HP_BATTLE, gBitTable[gBattleStruct->field_58[gActiveBattler]], 4, &gBattleMons[gActiveBattler].hp);
			MarkBufferBankForExecution(gActiveBattler);
    }
	
	SwitchOutFormsRevert(gActiveBattler);
	
    gBattlescriptCurrInstr += 2;
}

void atk61_drawpartystatussummary(void) {
    int i;
    pokemon_t* party;
	bool8 DoWeatherLoop = FALSE;
    struct HpAndStatus hpStatus[6];
    if (gBattleExecBuffer)
        return;

    gActiveBattler = GetBattleBank(T2_READ_8(gBattlescriptCurrInstr + 1));

    switch (ABILITY(gActiveBattler)) {	
		case ABILITY_PRIMORDIALSEA:
			if (gBattleWeather & WEATHER_RAIN_PRIMAL) {
				BattleStringLoader = PrimalRainEndString;
				DoWeatherLoop = TRUE;
			}
			break;
		case ABILITY_DESOLATELAND:
			if (gBattleWeather & WEATHER_SUN_PRIMAL) {
				BattleStringLoader = PrimalSunEndString;
				DoWeatherLoop = TRUE;
			}
			break;
		case ABILITY_DELTASTREAM:
			if (gBattleWeather & WEATHER_AIR_CURRENT_PRIMAL) {
				BattleStringLoader = PrimalAirCurrentEndString;
				DoWeatherLoop = TRUE;
			}
	}
	
	if (DoWeatherLoop) {
		
		for (i = 0; i < gBattlersCount; ++i) {
			if (i == gActiveBattler) continue;
			if (ABILITY(i) == ABILITY(gActiveBattler)) break;
		}
		
		if (i == gBattlersCount) {
			gBattleWeather = 0;
			gWeatherCounter = 0;
			PrimalWeatherLoc = 0; //Remove this line Eventually
			BattleScriptPushCursor();
			gBattlescriptCurrInstr = BattleScript_PrimalWeatherEnd;
			return;
		}
	}
	
    if (GetBattlerSide(gActiveBattler) == 0)
        party = gPlayerParty;
    else
        party = gEnemyParty;

    for (i = 0; i < 6; ++i) {
        if (party[i].species == PKMN_NONE || party[i].species == PKMN_EGG) {
            hpStatus[i].hp = 0xFFFF;
            hpStatus[i].status = 0;
        }
        else {
            hpStatus[i].hp = party[i].hp;
            hpStatus[i].status = party[i].condition;
        }
    }
    EmitDrawPartyStatusSummary(0, hpStatus, 1);
    MarkBufferBankForExecution(gActiveBattler);
    gBattlescriptCurrInstr += 2;
}

void atk4D_switchindataupdate(void) {
    struct BattlePokemon oldData;
    u32 i;
    u8* monData;

    if (gBattleExecBuffer) return;

    gActiveBattler = GetBattleBank(gBattlescriptCurrInstr[1]);
    oldData = gBattleMons[gActiveBattler];
    monData = (u8*)(&gBattleMons[gActiveBattler]);

    for (i = 0; i < sizeof(struct BattlePokemon); ++i)
        monData[i] = gBattleBufferB[gActiveBattler][4 + i];

    gBattleMons[gActiveBattler].type1 = gBaseStats[gBattleMons[gActiveBattler].species].type1;
    gBattleMons[gActiveBattler].type2 = gBaseStats[gBattleMons[gActiveBattler].species].type2;
    gBattleMons[gActiveBattler].ability = GetPartyAbility(GetBankPartyData(gActiveBattler));
	
	CONSUMED_ITEMS(gActiveBattler) = 0;
	StakeoutCounters[gActiveBattler] = 1;
	
	for (u8 i = 0; SwitchClearTableBytes[i] != (u8*) 0xFEFEFEFE; ++i)
		SwitchClearTableBytes[i][gActiveBattler] = 0;
		
	for (u8 i = 0; SwitchClearTableBits[i] != (u8*) 0xFEFEFEFE; ++i)
		*(SwitchClearTableBits[i]) &= ~(1 << gActiveBattler);
	
	//LastUsedMoves[gActiveBattler] = 0;
	//LastUsedTypes[gActiveBattler] = TYPE_BLANK;
	
    //Former Knock Off Check was here

    if (gCurrentMove == MOVE_BATONPASS)
    {
        for (i = 0; i < BATTLE_STATS_NO-1; ++i)
            gBattleMons[gActiveBattler].statStages[i] = oldData.statStages[i];
		
        gBattleMons[gActiveBattler].status2 = oldData.status2;
		
		//Gastro Acid Passing
		if (gStatuses3[gActiveBattler] & STATUS3_ABILITY_SUPPRESS) 
		{
			SuppressedAbilities[gActiveBattler] = gBattleMons[gActiveBattler].ability;
			gBattleMons[gActiveBattler].ability = 0;
		}
		
		//Power Trick Passing
		if (gStatuses3[gActiveBattler] & STATUS3_POWER_TRICK) 
		{
			u8 attack = gBattleMons[gActiveBattler].attack;
			u8 defense = gBattleMons[gActiveBattler].defense;
			gBattleMons[gActiveBattler].attack = defense;
			gBattleMons[gActiveBattler].defense = attack;
		}
		
		if (!(gStatuses3[gActiveBattler] & STATUS3_LEVITATING))
			MagnetRiseTimers[gActiveBattler] = 0;
    }
	
	if (ABILITY(gActiveBattler) == ABILITY_ILLUSION && GetIllusionPartyData(gActiveBattler) != GetBankPartyData(gActiveBattler))
		gStatuses3[gActiveBattler] |= STATUS3_ILLUSION;
	
    SwitchInClearSetData();

    gBattleScripting->bank = gActiveBattler;

    PREPARE_MON_NICK_BUFFER(gBattleTextBuff1, gActiveBattler, gBattlerPartyIndexes[gActiveBattler]);
	
	gBattleMons[gActiveBattler].type3 = TYPE_BLANK;

    gBattlescriptCurrInstr += 2;
}

void atk4F_jumpifcantswitch(void) {
    int i;
    u8 firstMonId, lastMonId;
    struct Pokemon *party;

    gActiveBattler = GetBattleBank(T2_READ_8(gBattlescriptCurrInstr + 1) & ~(ATK4F_DONT_CHECK_STATUSES));

    if (!(T2_READ_8(gBattlescriptCurrInstr + 1) & ATK4F_DONT_CHECK_STATUSES)
	&& !IsOfType(gActiveBattler, TYPE_GHOST)
	&& ITEM_EFFECT(gActiveBattler) != ITEM_EFFECT_SHED_SHELL
    && ((gBattleMons[gActiveBattler].status2 & (STATUS2_WRAPPED | STATUS2_ESCAPE_PREVENTION)) || (gStatuses3[gActiveBattler] & STATUS3_ROOTED) || FairyLockTimer))
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 2);
		
    else if (gBattleTypeFlags & (BATTLE_TYPE_INGAME_PARTNER | BATTLE_TYPE_MULTI)
	|| (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS && gBattleTypeFlags & BATTLE_TYPE_TRAINER && GetBattlerSide(gActiveBattler) == B_SIDE_OPPONENT)) 
	{
		
		party = LoadPartyRange(gActiveBattler, &firstMonId, &lastMonId);
		
        for (i = firstMonId; i < lastMonId; ++i) {
            if (GetMonData(&party[i], MON_DATA_SPECIES, 0) != PKMN_NONE
             && !GetMonData(&party[i], MON_DATA_IS_EGG, 0)
             && GetMonData(&party[i], MON_DATA_HP, 0) != 0
             && gBattlerPartyIndexes[gActiveBattler] != i)
                break;
        }

        if (i == lastMonId)
            gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 2);
        else
            gBattlescriptCurrInstr += 6;
    }
	
    else //Single Battle // Regular Double Battle // or Wild Double Battle
    {
        u8 battlerIn1, battlerIn2;

        if (GetBattlerSide(gActiveBattler) == B_SIDE_OPPONENT)
        {
            battlerIn1 = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);

            if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
                battlerIn2 = GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT);
            else
                battlerIn2 = battlerIn1;

            party = gEnemyParty;
        }
        else
        {
            battlerIn1 = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);

            if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
                battlerIn2 = GetBattlerAtPosition(B_POSITION_PLAYER_RIGHT);
            else
                battlerIn2 = battlerIn1;

            party = gPlayerParty;
        }

        for (i = 0; i < PARTY_SIZE; i++) {
            if (GetMonData(&party[i], MON_DATA_HP, 0) != 0
             && GetMonData(&party[i], MON_DATA_SPECIES, 0) != PKMN_NONE
             && !GetMonData(&party[i], MON_DATA_IS_EGG, 0)
             && i != gBattlerPartyIndexes[battlerIn1] && i != gBattlerPartyIndexes[battlerIn2])
                break;
        }

        if (i == 6)
            gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 2);
        else
            gBattlescriptCurrInstr += 6;
    }
}

void atk51_switchhandleorder(void) {
    int i;
    if (gBattleExecBuffer) return;

    gActiveBattler = GetBattleBank(T2_READ_8(gBattlescriptCurrInstr + 1));

    switch (T2_READ_8(gBattlescriptCurrInstr + 2)) {
		case 0:
			for (i = 0; i < gBattlersCount; ++i) {
				if (gBattleBufferB[i][0] == 0x22)
					gBattleStruct->monToSwitchIntoId[i] = gBattleBufferB[i][1];
			}
			break;
		
		case 1:
			if (!(gBattleTypeFlags & BATTLE_TYPE_MULTI))
				sub_8013F6C(gActiveBattler);
			break;

		case 2:
			gBattleCommunication[0] = gBattleBufferB[gActiveBattler][1];
			gBattleStruct->monToSwitchIntoId[gActiveBattler] = gBattleBufferB[gActiveBattler][1];

			if (gBattleTypeFlags & BATTLE_TYPE_LINK && gBattleTypeFlags & BATTLE_TYPE_MULTI)
			{
				gBattleStruct->field_60[gActiveBattler][0] &= 0xF;
				gBattleStruct->field_60[gActiveBattler][0] |= (gBattleBufferB[gActiveBattler][2] & 0xF0);
				gBattleStruct->field_60[gActiveBattler][1] = gBattleBufferB[gActiveBattler][3];

				gBattleStruct->field_60[gActiveBattler ^ BIT_FLANK][0] &= (0xF0);
				gBattleStruct->field_60[gActiveBattler ^ BIT_FLANK][0] |= (gBattleBufferB[gActiveBattler][2] & 0xF0) >> 4;
				gBattleStruct->field_60[gActiveBattler ^ BIT_FLANK][2] = gBattleBufferB[gActiveBattler][3];
			}
			else if (gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER)
				sub_80571DC(gActiveBattler, gBattleStruct->monToSwitchIntoId[gActiveBattler]);
				
			else
				sub_8013F6C(gActiveBattler);

			PREPARE_SPECIES_BUFFER(gBattleTextBuff1, gBattleMons[gBattlerAttacker].species)
			PREPARE_MON_NICK_BUFFER(gBattleTextBuff2, gActiveBattler, gBattleBufferB[gActiveBattler][1])
			break;
		}
    gBattlescriptCurrInstr += 3;
}

void atk52_switchineffects(void) {
    int i;
	u8 spikesDmg;

    gActiveBattler = GetBattleBank(T2_READ_8(gBattlescriptCurrInstr + 1));
    sub_80174B8(gActiveBattler);
    gHitMarker &= ~(HITMARKER_FAINTED(gActiveBattler));
    gSpecialStatuses[gActiveBattler].flag40 = 0;
	u8 ability = ABILITY(gActiveBattler);
	
	if(gBattleMons[gActiveBattler].hp == 0)
		goto SWITCH_IN_END;
	
	if (!(gSideAffecting[GetBattlerSide(gActiveBattler)] & SIDE_STATUS_SPIKES) 
	&& SwitchInEffectsTracker >= SwitchIn_Spikes 
	&& SwitchInEffectsTracker <= SwitchIn_StickyWeb)
		SwitchInEffectsTracker = SwitchIn_PrimalReversion;
	
	switch (SwitchInEffectsTracker) {
		case SwitchIn_HealingWish:
			if (gBattleMons[gActiveBattler].hp != gBattleMons[gActiveBattler].maxHP
			|| gBattleMons[gActiveBattler].status1) {
				if (HealingWishLoc & (1 << gActiveBattler)) {
					HealingWishLoc &= ~(1 << gActiveBattler);
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_HealingWishHeal;
					gBattleMoveDamage = -1 * (gBattleMons[gActiveBattler].maxHP);
					gBattleMons[gActiveBattler].status1 = 0;
					EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 4, &gBattleMons[gActiveBattler].status1);
					gBattleScripting->bank = gActiveBattler;
					gBankAttacker = gActiveBattler;
					++SwitchInEffectsTracker;
					return;
				}
				else if (HealingWishLoc & (1 << gActiveBattler << 4)) {
					goto LUNAR_DANCE_ACTIVATE;
				}
			}
			else if (HealingWishLoc & (1 << gActiveBattler << 4) && !PPIsMaxed(gActiveBattler)) {
			LUNAR_DANCE_ACTIVATE:
				HealingWishLoc &= ~(1 << gActiveBattler << 4);
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_LunarDanceHeal;
				
				gBattleMoveDamage = -1 * (gBattleMons[gActiveBattler].maxHP);
				gBattleMons[gActiveBattler].status1 = 0;
				EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 4, &gBattleMons[gActiveBattler].status1);
				
				//Restore PP Only in Lunar Dance
				for (int i = 0; i < 4; ++i) {
					if (gBattleMons[gActiveBattler].moves[i] == 0) 
						break;
					
					gBattleMons[gActiveBattler].pp[i] = CalculatePPWithBonus(gBattleMons[gActiveBattler].moves[i], gBattleMons[gActiveBattler].ppBonuses, i);
					EmitSetMonData(0, REQUEST_PPMOVE1_BATTLE + i, 0, 1, &gBattleMons[gActiveBattler].pp[i]);
				}
				
				gBattleScripting->bank = gActiveBattler;
				gBankAttacker = gActiveBattler;
				++SwitchInEffectsTracker;
				return;
			}
			++SwitchInEffectsTracker;
		__attribute__ ((fallthrough));	
		
		case SwitchIn_Spikes:
			if (CheckGrounding(gActiveBattler) && ability != ABILITY_MAGICGUARD && gSideTimers[GetBattlerSide(gActiveBattler)].spikesAmount) {
				spikesDmg = (5 - gSideTimers[GetBattlerSide(gActiveBattler)].spikesAmount) * 2;
				gSideAffecting[GetBattlerSide(gActiveBattler)] |= SIDE_STATUS_SPIKES_DAMAGED;
				gBattleMoveDamage = MathMax(1, udivsi(gBattleMons[gActiveBattler].maxHP, spikesDmg));

				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_SpikesHurt;								
				gSideAffecting[GetBattlerSide(gActiveBattler)] |= SIDE_STATUS_SPIKES_DAMAGED;
				gBattleScripting->bank = gActiveBattler;
				gBankTarget = gActiveBattler;
				++SwitchInEffectsTracker;
				return;
			}
			++SwitchInEffectsTracker;
		__attribute__ ((fallthrough));	
		
		case SwitchIn_StealthRock:
			if (ability != ABILITY_MAGICGUARD && gSideTimers[GetBattlerSide(gActiveBattler)].srAmount) {
				u8 divisor = 8;
				gBattleMoveDamage = 40;
				u8 flags;
				TypeDamageModification(0, gActiveBattler, MOVE_STEALTHROCK, TYPE_ROCK, &flags);
				switch (gBattleMoveDamage) {
					case 5:
						divisor = 64;
						break;
					case 10:
						divisor = 32;
						break;
					case 20:
						divisor = 16;
						break;
					case 40:
						divisor = 8;
						break;
					case 80:
						divisor = 4;
						break;
					case 160:
						divisor = 2;
						break;
					case 320:
						divisor = 1;
				}
				gBattleMoveDamage = MathMax(1, udivsi(gBattleMons[gActiveBattler].maxHP, divisor));

				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_SRHurt;								
				gSideAffecting[GetBattlerSide(gActiveBattler)] |= SIDE_STATUS_SPIKES_DAMAGED;
				gBattleScripting->bank = gActiveBattler;
				gBankTarget = gActiveBattler;
				++SwitchInEffectsTracker;
				return;
			}
			++SwitchInEffectsTracker;
		__attribute__ ((fallthrough));	
		
		case SwitchIn_ToxicSpikes:
			if (CheckGrounding(gActiveBattler) && gSideTimers[GetBattlerSide(gActiveBattler)].tspikesAmount) {
				
				if (gBattleMons[gActiveBattler].type1 == TYPE_POISON 
				||  gBattleMons[gActiveBattler].type2 == TYPE_POISON 
				||  gBattleMons[gActiveBattler].type3 == TYPE_POISON) {
					gSideTimers[GetBattlerSide(gActiveBattler)].tspikesAmount = 0;
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_TSAbsorb;
				}
				else if (gSideTimers[GetBattlerSide(gActiveBattler)].tspikesAmount == 1) {
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_TSPoison;
				}
				else {
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_TSHarshPoison;
				}
				gSideAffecting[GetBattlerSide(gActiveBattler)] |= SIDE_STATUS_SPIKES_DAMAGED;
				gBattleScripting->bank = gActiveBattler;
				gBankTarget = gActiveBattler;
				++SwitchInEffectsTracker;
				return;
			}
			++SwitchInEffectsTracker;
		__attribute__ ((fallthrough));	
		
		case SwitchIn_StickyWeb:
			if (CheckGrounding(gActiveBattler) && gSideTimers[GetBattlerSide(gActiveBattler)].stickyWeb) {
					
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_StickyWebSpeedDrop;
					gSideAffecting[GetBattlerSide(gActiveBattler)] |= SIDE_STATUS_SPIKES_DAMAGED;
					gBattleScripting->bank = gActiveBattler;
					gBankTarget = gActiveBattler;
					++SwitchInEffectsTracker;
					return;
			}
			++SwitchInEffectsTracker;
		__attribute__ ((fallthrough));	
		
		case SwitchIn_PrimalReversion:	;
			u8* script = DoPrimalReversion(gActiveBattler, 1);
			if(script) {
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = script;
				gBattleScripting->bank = gActiveBattler;
				gBattlerAttacker = gActiveBattler;
				++SwitchInEffectsTracker;
				return;
			}
			++SwitchInEffectsTracker;
		__attribute__ ((fallthrough));	
		
		case SwitchIn_Truant:
			if (ABILITY(gActiveBattler) == ABILITY_TRUANT)
				gDisableStructs[gActiveBattler].truantCounter = 1;
			++SwitchInEffectsTracker;
		__attribute__ ((fallthrough));	
		
		case SwitchIn_Abilities:
			if (AbilityBattleEffects(0, gActiveBattler, 0, 0, 0))
				return;
			++SwitchInEffectsTracker;
		__attribute__ ((fallthrough));
		
		case SwitchIn_Items:
			if (ItemBattleEffects(ItemEffects_SwitchIn, gActiveBattler, FALSE, FALSE))
				return;
			++SwitchInEffectsTracker;
		__attribute__ ((fallthrough));
		
		case SwitchIn_AirBalloon:
			if (ITEM_EFFECT(gActiveBattler) == ITEM_EFFECT_AIR_BALLOON) {
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_AirBalloonSub;
				gBattleScripting->bank = gActiveBattler;
				gBattlerAttacker = gActiveBattler;
				++SwitchInEffectsTracker;
				return;
			}
			++SwitchInEffectsTracker;
		__attribute__ ((fallthrough));
	
		case SwitchIn_End:
		SWITCH_IN_END:
            gSideAffecting[GetBattlerSide(gActiveBattler)] &= ~(SIDE_STATUS_SPIKES_DAMAGED);

            for (i = 0; i < gBattlersCount; ++i) {
                if (gBanksByTurnOrder[i] == gActiveBattler)
                    gActionsByTurnOrder[i] = 0xC;
            }

            for (i = 0; i < gBattlersCount; i++) {
                gBattleStruct->hpOnSwitchout[GetBattlerSide(i)] = gBattleMons[i].hp;
            }

            if (T2_READ_8(gBattlescriptCurrInstr + 1) == 5) {
                u32 hitmark = gHitMarker >> 0x1C;
                ++gBank1;
                while (1)
                {
                    if (hitmark & gBitTable[gBank1] && !(gAbsentBattlerFlags & gBitTable[gBank1]))
                        break;
                    if (gBank1 >= gBattlersCount)
                        break;
                    ++gBank1;
                }
            }
			
			SwitchInEffectsTracker = 0;
            gBattlescriptCurrInstr += 2;
    }
}

//Ripped from PokeEmerald
void atk8F_forcerandomswitch(void) {
    int i;
    s32 battler1PartyId = 0;
    s32 battler2PartyId = 0;
    u8 lastMonId = 0; // + 1
    u8 firstMonId = 0;
    s32 monsCount = 0;
    struct Pokemon *party = NULL;
    s32 validMons = 0;
    s32 minNeeded = 0;

    if (gBattleTypeFlags & (BATTLE_TYPE_TRAINER | BATTLE_TYPE_DOUBLE)) //Trainer Battle or Wild Double Battle
    {
        party = LoadPartyRange(gBankTarget, &firstMonId, &lastMonId);

        if (   (gBattleTypeFlags & BATTLE_TYPE_BATTLE_TOWER && gBattleTypeFlags & BATTLE_TYPE_LINK)
            || (gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER)
			|| (gBattleTypeFlags & BATTLE_TYPE_MULTI && gBattleTypeFlags & BATTLE_TYPE_LINK))
        {
            monsCount = 3;
            minNeeded = 1;
            battler2PartyId = gBattlerPartyIndexes[gBankTarget];
            battler1PartyId = gBattlerPartyIndexes[gBankTarget ^ BIT_FLANK];
        }
		
        else if (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS)
        {
            if (GetBattlerSide(gBankTarget) == B_SIDE_PLAYER)
            {
                monsCount = 6;
                minNeeded = 2; // since there are two opponents, it has to be a double battle
            }
            else //If Double Wild Battle, the following values will not be used
            {
                monsCount = 3;
                minNeeded = 1;
            }
            battler2PartyId = gBattlerPartyIndexes[gBankTarget];
            battler1PartyId = gBattlerPartyIndexes[gBankTarget ^ BIT_FLANK];
        }
		
        else if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
        {
            monsCount = 6;
            minNeeded = 2;
            battler2PartyId = gBattlerPartyIndexes[gBankTarget];
            battler1PartyId = gBattlerPartyIndexes[gBankTarget ^ BIT_FLANK];
        }
		
        else
        {
            monsCount = 6;
            minNeeded = 1;
            battler2PartyId = gBattlerPartyIndexes[gBankTarget]; // there is only one pokemon out in single battles
            battler1PartyId = gBattlerPartyIndexes[gBankTarget];
        }

        for (i = firstMonId; i < lastMonId; i++)
        {
            if (GetMonData(&party[i], MON_DATA_SPECIES, 0) != PKMN_NONE
            && !GetMonData(&party[i], MON_DATA_IS_EGG, 0)
            &&  GetMonData(&party[i], MON_DATA_HP, 0) != 0)
                validMons++;
        }

        if (validMons <= minNeeded)
            gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
        
        else
        {
            if (TryDoForceSwitchOut())
            {
                do
                {
                    i = umodsi(Random(), monsCount);
                    i += firstMonId;
                }
                while (   i == battler1PartyId
                       || i == battler2PartyId
                       || GetMonData(&party[i], MON_DATA_SPECIES, 0) == PKMN_NONE
                       || GetMonData(&party[i], MON_DATA_IS_EGG, 0) != FALSE
                       || GetMonData(&party[i], MON_DATA_HP, 0) == 0);
            }
            gBattleStruct->monToSwitchIntoId[gBankTarget] = i;

            if (!IsLinkDoubleBattle())
                sub_8013F6C(gBankTarget);

            if ((gBattleTypeFlags & BATTLE_TYPE_LINK && gBattleTypeFlags & BATTLE_TYPE_BATTLE_TOWER)
                || (gBattleTypeFlags & BATTLE_TYPE_LINK && gBattleTypeFlags & BATTLE_TYPE_MULTI))
            {
                sub_8127EC4(gBankTarget, i, 0);
                sub_8127EC4(gBankTarget ^ BIT_FLANK, i, 1);
            }

            if (gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER)
                sub_80571DC(gBankTarget, i);
        }
    }
    else //Regular Wild Battle
        TryDoForceSwitchOut();
}

bool8 TryDoForceSwitchOut(void) {
	if (gBattleTypeFlags & BATTLE_TYPE_TRAINER) {
		gBattleStruct->field_58[gBankTarget] = gBattlerPartyIndexes[gBankTarget];
		gBattlescriptCurrInstr = BattleScript_SuccessForceOut;
		return TRUE;
	}
	
	//If Wild Battle
	else if (gBattleMons[gBankAttacker].level < gBattleMons[gBankTarget].level) {
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
        return FALSE;
	}
	
	//Roar always fails in Wild Double Battles if used by the player
	else if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE && GetBattlerSide(gBankAttacker) == B_SIDE_PLAYER) {
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
        return FALSE;
	}

	gBattleStruct->field_58[gBankTarget] = gBattlerPartyIndexes[gBankTarget];
	gBattlescriptCurrInstr = BattleScript_SuccessForceOut;
	return TRUE;
}

void sub_80571DC(u8 battlerId, u8 arg1) { //0x8013F6C in FR
    if (GetBattlerSide(battlerId) != B_SIDE_OPPONENT)
    {
        s32 i;

        // gBattleStruct->field_60[0][i]

        for (i = 0; i < 3; i++)
            gUnknown_0203B0DC[i] = *(0 * 3 + i + (u8*)(gBattleStruct->field_60));

        sub_8127FF4(pokemon_order_func(gBattlerPartyIndexes[battlerId]), pokemon_order_func(arg1)); //In Emerald: sub_81B8FB0

        for (i = 0; i < 3; i++)
            *(0 * 3 + i + (u8*)(gBattleStruct->field_60)) = gUnknown_0203B0DC[i];
    }
}

bool8 PPIsMaxed(u8 bank) {
	for (int i = 0; i < 4; ++i) {
		if (gBattleMons[bank].moves[i] == 0) break;
		
		if (gBattleMons[bank].pp[i] < CalculatePPWithBonus(gBattleMons[bank].moves[i], gBattleMons[bank].ppBonuses, i))
			return FALSE;
	}
	return TRUE;
}

bool8 HasNoMonsToSwitch(u8 battler)
{	
    pokemon_t* party;
    u8 firstMonId, lastMonId, battlerIn1, battlerIn2;
    int i;

    if (!(gBattleTypeFlags & BATTLE_TYPE_DOUBLE)) {
        return FALSE;
	}

	party = LoadPartyRange(battler, &firstMonId, &lastMonId);
	
	battlerIn1 = battler;
    if (gAbsentBattlerFlags & gBitTable[GetBattlerAtPosition(GetBattlerPosition(battler) ^ BIT_FLANK)])
        battlerIn2 = battler;
    else
        battlerIn2 = GetBattlerAtPosition(GetBattlerPosition(battler) ^ BIT_FLANK);
			
    for (i = firstMonId; i < lastMonId; ++i)
    {
        if (GetMonData(&party[i], MON_DATA_HP, 0) != 0
        && GetMonData(&party[i], MON_DATA_SPECIES, 0) != PKMN_NONE
        && GetMonData(&party[i], MON_DATA_SPECIES2, 0) != PKMN_EGG
		&& !GetMonData(&party[i], MON_DATA_IS_EGG, 0)
		&& i != gBattlerPartyIndexes[battlerIn1] 
		&& i != gBattlerPartyIndexes[battlerIn2])
            break;
    }
    return (i == lastMonId);	
}
