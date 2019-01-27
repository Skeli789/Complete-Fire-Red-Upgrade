//Need to add a blank out for Swallow Stockpile Counter decrement
//Update Haze to not remove type 3
//Update  sMovesForbiddenToCopy, called in Mimic, Metronome, Assist, change 0xFFFF to 0xFEFE
//Update Gen 7 Metronome Choice effect
//Update Disable Timer
//Update Encore Timer
//Sketch update
//Destiny Bond End Turn Update
//Substitute for Heal Bell
//atk7A_jumpifnexttargetvalid, atkC2_selectfirstvalidtarget
//Future sight damage BS
//Update Semi-Invulnerable check in atk45_playanimation, atk46_playanimation2
//Update MoveValuesCleanUp CritMultiplier
//void atk23_getexp(void)
//Hook parental bond into AttackAnimation
//Type Resist Berries, Gems
//Metronome Item during Charging Turn
//Reciever ban list
//Update gCurrMovePos in Instruct
//Move Stomping Tantrum to end command based on TargetsHit
//Update EmitMoveAnimation to include attacker's item
//Fully create TerrainTable

#include "defines.h"
#include "helper_functions.h"
#include "pickup_items.h"
#include "general_bs_commands.h"

#define TEXT_BUFFER_SIDE_STATUS(move, status, side) 			\
{																\
	gSideAffecting[side] &= ~status;							\
    gBattleTextBuff1[0] = 0xFD;									\
    gBattleTextBuff1[1] = 0x10;									\
    gBattleTextBuff1[2] = (move & 0xFF);						\
    gBattleTextBuff1[3] = move >> 8;							\
    gBattleTextBuff1[4] = EOS;									\
	BattleScriptPushCursor();									\
	gBattlescriptCurrInstr = BattleScript_SideStatusWoreOffRet;	\
}

#define BattleScript_Pausex20 (u8*) 0x81D89F1

bool8 UproarWakeUpCheck(bank_t);
u8 CheckMoveLimitations(u8 bank, u8 unusableMoves, u8 check);
item_t ChoosePickupItem(u8 level);
u8 CastformDataTypeChange(u8 bank);

extern void PrepareStringBattle(u16 stringId, u8 bank);
extern u8 TypeCalc(move_t, u8 bankAtk, u8 bankDef, pokemon_t* party_data_atk, bool8 CheckParty);
extern bool8 ProtectAffects(move_t, bank_t, bank_t, u8 set);

extern u8 BattleScript_Gems[]; 
extern u8 BattleScript_Protean[];
extern u8 BattleScript_MimikyuTransform[];
extern u8 BattleScript_HangedOnFocusSash[];
extern u8 BattleScript_EnduredSturdy[];
extern u8 BattleScript_Receiver[];
extern u8 BattleScript_PrimalWeatherEnd[];
extern u8 BattleScript_Symbiosis[];
extern u8 BattleScript_SpikesFreeDefog[];
extern u8 BattleScript_FlowerGift[];

extern u8 MimikyuDisguisedTookDamageString[];
extern u8 StringEnduredHitWithSturdy[];
extern u8 PrimalRainEndString[];
extern u8 PrimalSunEndString[];
extern u8 PrimalAirCurrentEndString[];
extern u8 RemovedEntryHazardsYourSideString[];
extern u8 RemovedEntryHazardsFoeSideString[];
extern u8 PlayerSpikesLayString[];
extern u8 OpponentSpikesLayString[];
extern u8 PlayerStealthRockLayString[];
extern u8 OpponentStealthRockLayString[];
extern u8 PlayerToxicSpikesLayString[];
extern u8 OpponentToxicSpikesLayString[];
extern u8 PlayerStickyWebLayString[];
extern u8 OpponentStickyWebLayString[];

extern move_t SkyBattleBanTable[];
extern move_t GravityBanTable[];
extern move_t SleepTalkBanTable[];
extern move_t MovesThatCallOtherMovesTable[];
extern move_t GemBanTable[];
extern ability_t RolePlayBanTable[];
extern ability_t SkillSwapBanTable[];
extern ability_t ReceiverBanTable[];

extern u8* SwitchClearTableBytes[];
extern u8* SwitchClearTableBits[];
extern u8* gBattleScriptsForMoveEffects[];

void atk02_attackstring(void) {
	u8 moveType = gBattleStruct->dynamicMoveType;
	
    if (gBattleExecBuffer) return;
	
    if (!(gHitMarker & (HITMARKER_NO_ATTACKSTRING | HITMARKER_ATTACKSTRING_PRINTED))) {
		
        PrepareStringBattle(STRINGID_USEDMOVE, gBankAttacker);
		
		gBattlescriptCurrInstr++;
		gBattleCommunication[MSG_DISPLAY] = 0;
		
		if (!DancerByte && gCurrentMove != MOVE_COPYCAT) {
			LastUsedMove = gCurrentMove;
			LastUsedTypes[gBankAttacker] = moveType;
			
			if (!CheckTableForMove(gCurrentMove, MovesThatCallOtherMovesTable)) {
				if (gLastUsedMoves[gBankAttacker] == gCurrentMove)
					MetronomeCounter[gBankAttacker] = MathMin(100, MetronomeCounter[gBankAttacker] + 20);
				else
					MetronomeCounter[gBankAttacker] = 0;	
			}
		}	
		
		if (gCurrentMove != MOVE_STRUGGLE) {
			if (ITEM_EFFECT(gBankAttacker) == ITEM_EFFECT_GEM 
			&&  ITEM_QUALITY(gBankAttacker) == moveType 
			&&  SPLIT(gCurrentMove) != SPLIT_STATUS
			&& !(gMoveResultFlags & (MOVE_RESULT_MISSED | MOVE_RESULT_DOESNT_AFFECT_FOE | MOVE_RESULT_FAILED))
			&& !CheckTableForMove(gCurrentMove, GemBanTable)
			&& !(TypeCalc(gCurrentMove, gBankAttacker, gBankTarget, GetBankPartyData(gBankAttacker), 0) & (MOVE_RESULT_MISSED | MOVE_RESULT_DOESNT_AFFECT_FOE | MOVE_RESULT_FAILED))) 
			{
				gLastUsedItem = ITEM(gBankAttacker);
				GemHelper = TRUE;
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_Gems; 
			}
			
			if (ABILITY(gBankAttacker) == ABILITY_PROTEAN) {
				if (gBattleMons[gBankAttacker].type1 != moveType
				||  gBattleMons[gBankAttacker].type2 != moveType
				|| (gBattleMons[gBankAttacker].type3 != moveType && gBattleMons[gBankAttacker].type3 != TYPE_BLANK)) 
				{
					SET_BATTLER_TYPE(gBankAttacker, moveType);
					PREPARE_TYPE_BUFFER(gBattleTextBuff1, moveType);
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_Protean;
				}
			}
		}
		
		gHitMarker |= HITMARKER_ATTACKSTRING_PRINTED;
    }
	
	else {
		gBattlescriptCurrInstr++;
		gBattleCommunication[MSG_DISPLAY] = 0;
	}
}

void atk03_ppreduce(void) {
    s32 ppToDeduct = 1;

    if (gBattleExecBuffer) return;

    if (!gSpecialStatuses[gBankAttacker].flag20) {
        switch (gBattleMoves[gCurrentMove].target) {
			case MOVE_TARGET_FOES_AND_ALLY:
				ppToDeduct += AbilityBattleEffects(ABILITYEFFECT_COUNT_ON_FIELD, gBankAttacker, ABILITY_PRESSURE, 0, 0);
				break;
			
			case MOVE_TARGET_BOTH:
			case MOVE_TARGET_OPPONENTS_FIELD:
				ppToDeduct += AbilityBattleEffects(ABILITYEFFECT_COUNT_OTHER_SIDE, gBankAttacker, ABILITY_PRESSURE, 0, 0);
				break;
			
			default:
				if (gBankAttacker != gBankTarget && gBattleMons[gBankTarget].ability == ABILITY_PRESSURE)
					ppToDeduct++;
				break;
        }
    }

    if (!(gHitMarker & (HITMARKER_NO_PPDEDUCT | HITMARKER_NO_ATTACKSTRING)) && ParentalBondOn != 1 && !DancerByte && gBattleMons[gBankAttacker].pp[gCurrMovePos]) {
        gProtectStructs[gBankAttacker].notFirstStrike = 1;

        if (gBattleMons[gBankAttacker].pp[gCurrMovePos] > ppToDeduct)
            gBattleMons[gBankAttacker].pp[gCurrMovePos] -= ppToDeduct;
        else
            gBattleMons[gBankAttacker].pp[gCurrMovePos] = 0;

        if (!(gBattleMons[gBankAttacker].status2 & STATUS2_TRANSFORMED)
            && !((gDisableStructs[gBankAttacker].unk18_b) & gBitTable[gCurrMovePos]))
        {
            gActiveBattler = gBankAttacker;
            EmitSetMonData(0, REQUEST_PPMOVE1_BATTLE + gCurrMovePos, 0, 1, &gBattleMons[gBankAttacker].pp[gCurrMovePos]);
            MarkBufferBankForExecution(gBankAttacker);
        }
    }

    gHitMarker &= ~(HITMARKER_NO_PPDEDUCT);
    gBattlescriptCurrInstr++;
}

void atk0B_healthbarupdate(void) {
    if (gBattleExecBuffer) return;

    if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT)) {
        gActiveBattler = GetBattleBank(gBattlescriptCurrInstr[1]);
		
		u8 atkAbility;
		if (gBattlescriptCurrInstr[1] == BS_GET_TARGET)
			atkAbility = ABILITY(gBankAttacker);
		else
			atkAbility = 0;

        if (gBattleMons[gActiveBattler].status2 & STATUS2_SUBSTITUTE 
		&& gDisableStructs[gActiveBattler].substituteHP 
		&& !(gHitMarker & HITMARKER_IGNORE_SUBSTITUTE)
		&& atkAbility != ABILITY_INFILTRATOR) {
            PrepareStringBattle(STRINGID_SUBSTITUTEDAMAGED, gActiveBattler);
		}
		
		else if (ABILITY(gActiveBattler) == ABILITY_DISGUISE 
		&& gBattleMons[gActiveBattler].species == PKMN_MIMIKYU
		&& !(gHitMarker & HITMARKER_IGNORE_SUBSTITUTE)
		&& !(gBattleMons[gActiveBattler].status2 & STATUS2_TRANSFORMED)) {
			BattleStringLoader = MimikyuDisguisedTookDamageString;
			PrepareStringBattle(0x184, gActiveBattler);
		}
			
        else {
            s16 healthValue;
            s32 currDmg = gBattleMoveDamage;
            s32 maxPossibleDmgValue = 0xFFFF; //Ensures that huge damage values don't change sign
            if (currDmg <= maxPossibleDmgValue)
                healthValue = currDmg;
            else
                healthValue = maxPossibleDmgValue;
				
            EmitHealthBarUpdate(0, healthValue);
            MarkBufferBankForExecution(gActiveBattler);

            if (GetBattlerSide(gActiveBattler) == B_SIDE_PLAYER && gBattleMoveDamage > 0)
                gBattleResults->unk5_0 = 1;
        }
    }
    gBattlescriptCurrInstr += 2;
}

void atk0C_datahpupdate(void) {
    if (gBattleExecBuffer) return;

    if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT)) {
        gActiveBattler = GetBattleBank(gBattlescriptCurrInstr[1]);
		
		u8 atkAbility;
		if (gBattlescriptCurrInstr[1] == BS_GET_TARGET)
			atkAbility = ABILITY(gBankAttacker);
		else
			atkAbility = 0;
		
		//If Substitute Up
        if (gBattleMons[gActiveBattler].status2 & STATUS2_SUBSTITUTE 
		&& gDisableStructs[gActiveBattler].substituteHP 
		&& !(gHitMarker & HITMARKER_IGNORE_SUBSTITUTE) 
		&& atkAbility != ABILITY_INFILTRATOR) {
            if (gDisableStructs[gActiveBattler].substituteHP >= gBattleMoveDamage) {
                if (gSpecialStatuses[gActiveBattler].moveturnLostHP == 0)
                    gSpecialStatuses[gActiveBattler].moveturnLostHP = gBattleMoveDamage;
                gDisableStructs[gActiveBattler].substituteHP -= gBattleMoveDamage;
                gHpDealt = gBattleMoveDamage;
            }
			
            else {
                if (gSpecialStatuses[gActiveBattler].moveturnLostHP == 0)
                    gSpecialStatuses[gActiveBattler].moveturnLostHP = gDisableStructs[gActiveBattler].substituteHP;
                gHpDealt = gDisableStructs[gActiveBattler].substituteHP;
                gDisableStructs[gActiveBattler].substituteHP = 0;
            }
			
            //Check Substitute Fading
            if (gDisableStructs[gActiveBattler].substituteHP == 0) {
                gBattlescriptCurrInstr += 2;
                BattleScriptPushCursor();
                gBattlescriptCurrInstr = BattleScript_SubstituteFade;
                return;
            }
        }
		
		//No Substitute
		else if (ABILITY(gActiveBattler) == ABILITY_DISGUISE 
		&& gBattleMons[gActiveBattler].species == PKMN_MIMIKYU
		&& !(gHitMarker & HITMARKER_IGNORE_SUBSTITUTE)
		&& !(gBattleMons[gActiveBattler].status2 & STATUS2_TRANSFORMED)) {
			if (gSpecialStatuses[gActiveBattler].moveturnLostHP == 0)
				gSpecialStatuses[gActiveBattler].moveturnLostHP = 1;
			gHpDealt = 1;
			
			if (SPLIT(gCurrentMove) == SPLIT_PHYSICAL) {
				gProtectStructs[gActiveBattler].physicalDmg = gHpDealt;
				gSpecialStatuses[gActiveBattler].moveturnLostHP_physical = gHpDealt;
				gProtectStructs[gActiveBattler].physicalBank = gBankAttacker;
				gSpecialStatuses[gActiveBattler].moveturnPhysicalBank = gBankAttacker;
			}
			else if (SPLIT(gCurrentMove) == SPLIT_SPECIAL) {
				gProtectStructs[gActiveBattler].specialDmg = gHpDealt;
				gSpecialStatuses[gActiveBattler].moveturnLostHP_special = gHpDealt;
				gProtectStructs[gActiveBattler].specialBank = gBankAttacker;
				gSpecialStatuses[gActiveBattler].moveturnSpecialBank = gBankAttacker;
			}
			
			DoFormChange(gActiveBattler, PKMN_MIMIKYU_BUSTED, TRUE);
			gBattlescriptCurrInstr += 2;
			BattleScriptPushCursor();
			gBattlescriptCurrInstr = BattleScript_MimikyuTransform;
			return;
		}
		
        else {
            gHitMarker &= ~(HITMARKER_IGNORE_SUBSTITUTE);
            if (gBattleMoveDamage < 0) { // HP goes up
                gBattleMons[gActiveBattler].hp -= gBattleMoveDamage;
                if (gBattleMons[gActiveBattler].hp > gBattleMons[gActiveBattler].maxHP)
                    gBattleMons[gActiveBattler].hp = gBattleMons[gActiveBattler].maxHP;
            }
			
            else { // HP goes down
                if (gHitMarker & HITMARKER_x20)
                    gHitMarker &= ~(HITMARKER_x20);
				
                else {
                    gTakenDmg[gActiveBattler] += gBattleMoveDamage;
                    if (gBattlescriptCurrInstr[1] == BS_GET_TARGET)
                        gTakenDmgBanks[gActiveBattler] = gBankAttacker;
                    else
                        gTakenDmgBanks[gActiveBattler] = gBankTarget;
                }
	
	
                if (gBattleMons[gActiveBattler].hp > gBattleMoveDamage) {
                    gBattleMons[gActiveBattler].hp -= gBattleMoveDamage;
                    gHpDealt = gBattleMoveDamage;
                }
				
                else {
                    gHpDealt = gBattleMons[gActiveBattler].hp;
                    gBattleMons[gActiveBattler].hp = 0;
                }


                if (!gSpecialStatuses[gActiveBattler].moveturnLostHP && !(gHitMarker & HITMARKER_x100000))
                    gSpecialStatuses[gActiveBattler].moveturnLostHP = gHpDealt;

                if (SPLIT(gCurrentMove) == SPLIT_PHYSICAL && !(gHitMarker & HITMARKER_x100000) && gCurrentMove != MOVE_PAINSPLIT) {
                    gProtectStructs[gActiveBattler].physicalDmg = gHpDealt;
                    gSpecialStatuses[gActiveBattler].moveturnLostHP_physical = gHpDealt;
					
                    if (gBattlescriptCurrInstr[1] == BS_GET_TARGET) {
                        gProtectStructs[gActiveBattler].physicalBank = gBankAttacker;
                        gSpecialStatuses[gActiveBattler].moveturnPhysicalBank = gBankAttacker;
                    }
					
                    else {
                        gProtectStructs[gActiveBattler].physicalBank = gBankTarget;
                        gSpecialStatuses[gActiveBattler].moveturnPhysicalBank = gBankTarget;
                    }
                }
				
                else if (SPLIT(gCurrentMove) == SPLIT_SPECIAL && !(gHitMarker & HITMARKER_x100000)) {
                    gProtectStructs[gActiveBattler].specialDmg = gHpDealt;
                    gSpecialStatuses[gActiveBattler].moveturnLostHP_special = gHpDealt;
					
                    if (gBattlescriptCurrInstr[1] == BS_GET_TARGET) {
                        gProtectStructs[gActiveBattler].specialBank = gBankAttacker;
                        gSpecialStatuses[gActiveBattler].moveturnSpecialBank = gBankAttacker;
                    }
					
                    else {
                        gProtectStructs[gActiveBattler].specialBank = gBankTarget;
                        gSpecialStatuses[gActiveBattler].moveturnSpecialBank = gBankTarget;
                    }
                }
            }
            gHitMarker &= ~(HITMARKER_x100000);
            EmitSetMonData(0, REQUEST_HP_BATTLE, 0, 2, &gBattleMons[gActiveBattler].hp);
            MarkBufferBankForExecution(gActiveBattler);
        }
    }
    else
    {
        gActiveBattler = GetBattleBank(gBattlescriptCurrInstr[1]);
        if (gSpecialStatuses[gActiveBattler].moveturnLostHP == 0)
            gSpecialStatuses[gActiveBattler].moveturnLostHP = 0xFFFF;
    }
    gBattlescriptCurrInstr += 2;
}

void atk0D_critmessage(void) {
    if (!gBattleExecBuffer) {
        if (gCritMultiplier > 100 && !(gMoveResultFlags & MOVE_RESULT_NO_EFFECT)) {
            PrepareStringBattle(STRINGID_CRITICALHIT, gBankAttacker);
            gBattleCommunication[MSG_DISPLAY] = 1;
        }
        gBattlescriptCurrInstr++;
    }
}

void atk0F_resultmessage(void) {
    u32 stringId = 0;

    if (gBattleExecBuffer) return;

    if (gMoveResultFlags & MOVE_RESULT_MISSED && (!(gMoveResultFlags & MOVE_RESULT_DOESNT_AFFECT_FOE) || gBattleCommunication[6] > 2)) {
        stringId = gMissStringIds[gBattleCommunication[6]];
        gBattleCommunication[MSG_DISPLAY] = 1;
    }
	
    else {
        gBattleCommunication[MSG_DISPLAY] = 1;
        switch (gMoveResultFlags & (u8)(~(MOVE_RESULT_MISSED))) {
        case MOVE_RESULT_SUPER_EFFECTIVE:
			++TargetsHit;
            stringId = STRINGID_SUPEREFFECTIVE;
            break;
			
        case MOVE_RESULT_NOT_VERY_EFFECTIVE:
			++TargetsHit;
            stringId = STRINGID_NOTVERYEFFECTIVE;
            break;
			
        case MOVE_RESULT_ONE_HIT_KO:
			++TargetsHit;
            stringId = STRINGID_ONEHITKO;
            break;
			
        case MOVE_RESULT_FOE_ENDURED:
			++TargetsHit;
			if (EnduranceHelper == ENDURE_STURDY) {
				stringId = 0x184;
				BattleStringLoader = StringEnduredHitWithSturdy;
				gLastUsedAbility = ABILITY(gBankTarget);
				EnduranceHelper = 0;
				gProtectStructs[gBankTarget].enduredSturdy = 0;
			}
			else
				stringId = STRINGID_PKMNENDUREDHIT;
            break;
			
        case MOVE_RESULT_FAILED:
            stringId = STRINGID_BUTITFAILED;
            break;
			
        case MOVE_RESULT_DOESNT_AFFECT_FOE:
            stringId = STRINGID_ITDOESNTAFFECT;
            break;
			
        case MOVE_RESULT_FOE_HUNG_ON:
			++TargetsHit;
            gLastUsedItem = gBattleMons[gBankTarget].item;
            gStringBank = gBankTarget;
            gMoveResultFlags &= ~(MOVE_RESULT_FOE_ENDURED | MOVE_RESULT_FOE_HUNG_ON);
            BattleScriptPushCursor();
			if (EnduranceHelper == ENDURE_FOCUS_SASH) {
				EnduranceHelper = 0;
				gBattlescriptCurrInstr = BattleScript_HangedOnFocusSash;
			}
			else
				gBattlescriptCurrInstr = BattleScript_HangedOnMsg;
            return;
		
        default:
            if (gMoveResultFlags & MOVE_RESULT_DOESNT_AFFECT_FOE)
                stringId = STRINGID_ITDOESNTAFFECT;
            
            else if (gMoveResultFlags & MOVE_RESULT_ONE_HIT_KO) {
				++TargetsHit;
                gMoveResultFlags &= ~(MOVE_RESULT_ONE_HIT_KO);
                gMoveResultFlags &= ~(MOVE_RESULT_SUPER_EFFECTIVE);
                gMoveResultFlags &= ~(MOVE_RESULT_NOT_VERY_EFFECTIVE);
                BattleScriptPushCursor();
                gBattlescriptCurrInstr = BattleScript_OneHitKOMsg;
                return;
            }
			
            else if (gMoveResultFlags & MOVE_RESULT_FOE_ENDURED) {
				++TargetsHit;
                gMoveResultFlags &= ~(MOVE_RESULT_FOE_ENDURED | MOVE_RESULT_FOE_HUNG_ON);
                BattleScriptPushCursor();
				if (EnduranceHelper == ENDURE_STURDY) {
					gLastUsedAbility = ABILITY(gBankTarget);
					EnduranceHelper = 0;
					gProtectStructs[gBankTarget].enduredSturdy = 0;
					gBattlescriptCurrInstr = BattleScript_EnduredSturdy;
				}
				else
					gBattlescriptCurrInstr = BattleScript_EnduredMsg;
                return;
            }
			
            else if (gMoveResultFlags & MOVE_RESULT_FOE_HUNG_ON) {
				++TargetsHit;
                gLastUsedItem = gBattleMons[gBankTarget].item;
                gStringBank = gBankTarget;
                gMoveResultFlags &= ~(MOVE_RESULT_FOE_ENDURED | MOVE_RESULT_FOE_HUNG_ON);
                BattleScriptPushCursor();
                if (EnduranceHelper == ENDURE_FOCUS_SASH) {
					EnduranceHelper = 0;
					gBattlescriptCurrInstr = BattleScript_HangedOnFocusSash;
				}
				else
					gBattlescriptCurrInstr = BattleScript_HangedOnMsg;
                return;
            }
			
            else if (gMoveResultFlags & MOVE_RESULT_FAILED)  {
                stringId = STRINGID_BUTITFAILED;
            }
			
            else {
				++TargetsHit;
                gBattleCommunication[MSG_DISPLAY] = 0;
			}
        }
    }
	
    if (stringId)
        PrepareStringBattle(stringId, gBankAttacker);

    gBattlescriptCurrInstr++;
}

void atk1B_cleareffectsonfaint(void) {
	int i;
	u8 partner = PARTNER(gActiveBattler);
	
    if (!gBattleExecBuffer) {
		switch (FaintEffectsTracker) {
			case Faint_ClearEffects:
				gActiveBattler = GetBattleBank(gBattlescriptCurrInstr[1]);
				
				gBattleMons[gActiveBattler].status1 = 0;
				EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 0x4, &gBattleMons[gActiveBattler].status1);
				MarkBufferBankForExecution(gActiveBattler);
				UndoEffectsAfterFainting();
				
				for (u8 i = 0; SwitchClearTableBytes[i] != (u8*) 0xFEFEFEFE; ++i)
					SwitchClearTableBytes[i][gActiveBattler] = 0;
				
				for (u8 i = 0; SwitchClearTableBits[i] != (u8*) 0xFEFEFEFE; ++i)
					*(SwitchClearTableBits[i]) &= ~(1 << gActiveBattler);
				
				gProtectStructs[gActiveBattler].KingsShield = 0;	//Necessary because could be sent away with Roar
				gProtectStructs[gActiveBattler].SpikyShield = 0;
				gProtectStructs[gActiveBattler].BanefulBunker = 0;
				gProtectStructs[gActiveBattler].enduredSturdy = 0;
				
				ZMoveData->toBeUsed[gActiveBattler] = 0; //Because you died before you could use the Z-Move
				
				gBattleMons[gActiveBattler].type3 = TYPE_BLANK;
				++FaintEffectsTracker;
			__attribute__ ((fallthrough));
			
			case Faint_ReceiverActivate:
				ReceiverActivated = FALSE;
				u8 partnerAbility = ABILITY(partner);
				
				if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE 
				&& (partnerAbility == ABILITY_RECEIVER || partnerAbility == ABILITY_POWEROFALCHEMY)
				&& !CheckTableForAbility(CopyAbility(gActiveBattler), ReceiverBanTable)) 
				{
					//Put Ability Activation Here Eventually
					gLastUsedAbility = partnerAbility;
					gBattleMons[partner].ability = CopyAbility(gActiveBattler);
					RecordAbilityBattle(partner, gBattleMons[partner].ability);
					gEffectBank = gActiveBattler;
					gBattleScripting->bank = partner;
					ReceiverActivated = TRUE;
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_Receiver;
					++FaintEffectsTracker;
					return;
				}
				++FaintEffectsTracker;
			__attribute__ ((fallthrough));
			
			case Faint_SwitchInAbilities:
				if (ReceiverActivated) {
					if (AbilityBattleEffects(0, partner, 0, 0, 0))
						return;
				}
				++FaintEffectsTracker;
			__attribute__ ((fallthrough));
			
			case Faint_PrimalWeather:	;
				bool8 DoWeatherLoop = FALSE;
				
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
		}
		
		FaintEffectsTracker = 0;
        gBattlescriptCurrInstr += 2;
    }
}

void atk1D_jumpifstatus2(void) {
    u8 bank = GetBattleBank(gBattlescriptCurrInstr[1]);
    u32 flags = T2_READ_32(gBattlescriptCurrInstr + 2);
    void* jump_loc = T2_READ_PTR(gBattlescriptCurrInstr + 6);
	
	if (gBattlescriptCurrInstr[1] == BS_GET_TARGET && ABILITY(gBankAttacker) == ABILITY_INFILTRATOR)
		flags &= ~(STATUS2_SUBSTITUTE);
	
    if (gBattleMons[bank].status2 & flags && gBattleMons[bank].hp)
        gBattlescriptCurrInstr = jump_loc;
    else
        gBattlescriptCurrInstr += 10;
}

void atk1F_jumpifsideaffecting(void) {
    u8 side;
    u16 flags = T2_READ_16(gBattlescriptCurrInstr + 2);
    void* jump_loc = T2_READ_PTR(gBattlescriptCurrInstr + 4);
	
    if (gBattlescriptCurrInstr[1] == 1)
        side = GetBattlerPosition(gBankAttacker) & 1;
	
    else {
        side = GetBattlerPosition(gBankTarget) & 1;
		if (ABILITY(gBankAttacker) == ABILITY_INFILTRATOR)
			side &= ~(SIDE_STATUS_SAFEGUARD | SIDE_STATUS_MIST);
	}

    if (gSideAffecting[side] & flags)
        gBattlescriptCurrInstr = jump_loc;
    else
        gBattlescriptCurrInstr += 8;
}

void atk22_jumpiftype(void) //u8 bank, u8 type, *ptr
{
    u8 bank = GetBattleBank(gBattlescriptCurrInstr[1]);
    u8 type = gBattlescriptCurrInstr[2];
    void* jump_loc = T2_READ_PTR(gBattlescriptCurrInstr + 3);

    if (IsOfType(bank, type))
        gBattlescriptCurrInstr = jump_loc;
    else
        gBattlescriptCurrInstr += 7;
}

void atk40_jumpifaffectedbyprotect(void)
{
    if (ProtectAffects(gCurrentMove, gBankAttacker, gBankTarget, FALSE))
    {
        gMoveResultFlags |= MOVE_RESULT_MISSED;
        JumpIfMoveFailed(5, 0);
        gBattleCommunication[6] = 1;
    }
    else
        gBattlescriptCurrInstr += 5;
}

void atk42_jumpiftype2(void) //u8 bank, u8 type, *ptr
{
    u8 bank = GetBattleBank(gBattlescriptCurrInstr[1]);

    if (IsOfType(bank, gBattlescriptCurrInstr[2]))
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 3);
    else
        gBattlescriptCurrInstr += 7;
}

void atk45_playanimation(void)
{
    const u16* argumentPtr;

    gActiveBattler = GetBattleBank(gBattlescriptCurrInstr[1]);
    argumentPtr = T2_READ_PTR(gBattlescriptCurrInstr + 3);

    if (gBattlescriptCurrInstr[2] == B_ANIM_STATS_CHANGE
    || 	gBattlescriptCurrInstr[2] == B_ANIM_SNATCH_MOVE
    || 	gBattlescriptCurrInstr[2] == B_ANIM_SUBSTITUTE_FADE
	|| 	gBattlescriptCurrInstr[2] == B_ANIM_TRANSFORM
	|| 	gBattlescriptCurrInstr[2] == B_ANIM_WISHIWASHI_FISH
	|| 	gBattlescriptCurrInstr[2] == B_ANIM_ZYGARDE_CELL_SWIRL
	|| 	gBattlescriptCurrInstr[2] == B_ANIM_BLUE_PRIMAL_REVERSION
	|| 	gBattlescriptCurrInstr[2] == B_ANIM_RED_PRIMAL_REVERSION
	|| 	gBattlescriptCurrInstr[2] == B_ANIM_TRANSFORM
	|| 	gBattlescriptCurrInstr[2] == B_ANIM_ZMOVE_ACTIVATE
	|| 	gBattlescriptCurrInstr[2] == B_ANIM_MEGA_EVOLUTION
	|| 	gBattlescriptCurrInstr[2] == B_ANIM_ULTRA_BURST)
    {
        EmitBattleAnimation(0, gBattlescriptCurrInstr[2], *argumentPtr);
        MarkBufferBankForExecution(gActiveBattler);
        gBattlescriptCurrInstr += 7;
    }
    else if (gHitMarker & HITMARKER_NO_ANIMATIONS)
    {
        BattleScriptPush(gBattlescriptCurrInstr + 7);
        gBattlescriptCurrInstr = BattleScript_Pausex20;
    }
    else if (gBattlescriptCurrInstr[2] == B_ANIM_RAIN_CONTINUES
          || gBattlescriptCurrInstr[2] == B_ANIM_SUN_CONTINUES
          || gBattlescriptCurrInstr[2] == B_ANIM_SANDSTORM_CONTINUES
          || gBattlescriptCurrInstr[2] == B_ANIM_HAIL_CONTINUES
	      || gBattlescriptCurrInstr[2] == B_ANIM_DELTA_STREAM
	      || gBattlescriptCurrInstr[2] == B_ANIM_FOG_CONTINUES)
    {
        EmitBattleAnimation(0, gBattlescriptCurrInstr[2], *argumentPtr);
        MarkBufferBankForExecution(gActiveBattler);
        gBattlescriptCurrInstr += 7;
    }
    else if (gStatuses3[gActiveBattler] & STATUS3_SEMI_INVULNERABLE)
    {
        gBattlescriptCurrInstr += 7;
    }
    else
    {
        EmitBattleAnimation(0, gBattlescriptCurrInstr[2], *argumentPtr);
        MarkBufferBankForExecution(gActiveBattler);
        gBattlescriptCurrInstr += 7;
    }
}

void atk46_playanimation2(void) // animation Id is stored in the first pointer
{
    const u16* argumentPtr;
    const u8* animationIdPtr;

    gActiveBattler = GetBattleBank(gBattlescriptCurrInstr[1]);
    animationIdPtr = T2_READ_PTR(gBattlescriptCurrInstr + 2);
    argumentPtr = T2_READ_PTR(gBattlescriptCurrInstr + 6);

    if (*animationIdPtr == B_ANIM_STATS_CHANGE
    || 	*animationIdPtr == B_ANIM_SNATCH_MOVE
    || 	*animationIdPtr == B_ANIM_SUBSTITUTE_FADE
	|| 	*animationIdPtr == B_ANIM_TRANSFORM
	|| 	*animationIdPtr == B_ANIM_WISHIWASHI_FISH
	|| 	*animationIdPtr == B_ANIM_ZYGARDE_CELL_SWIRL
	|| 	*animationIdPtr == B_ANIM_BLUE_PRIMAL_REVERSION
	|| 	*animationIdPtr == B_ANIM_RED_PRIMAL_REVERSION
	|| 	*animationIdPtr == B_ANIM_TRANSFORM
	|| 	*animationIdPtr == B_ANIM_ZMOVE_ACTIVATE
	|| 	*animationIdPtr == B_ANIM_MEGA_EVOLUTION
	|| 	*animationIdPtr == B_ANIM_ULTRA_BURST)
    {
        EmitBattleAnimation(0, *animationIdPtr, *argumentPtr);
        MarkBufferBankForExecution(gActiveBattler);
        gBattlescriptCurrInstr += 10;
    }
    else if (gHitMarker & HITMARKER_NO_ANIMATIONS)
    {
        gBattlescriptCurrInstr += 10;
    }
    else if (*animationIdPtr == B_ANIM_RAIN_CONTINUES
             || *animationIdPtr == B_ANIM_SUN_CONTINUES
             || *animationIdPtr == B_ANIM_SANDSTORM_CONTINUES
             || *animationIdPtr == B_ANIM_HAIL_CONTINUES
			 || *animationIdPtr == B_ANIM_DELTA_STREAM
			 || *animationIdPtr == B_ANIM_FOG_CONTINUES)
    {
        EmitBattleAnimation(0, *animationIdPtr, *argumentPtr);
        MarkBufferBankForExecution(gActiveBattler);
        gBattlescriptCurrInstr += 10;
    }
    else if (gStatuses3[gActiveBattler] & STATUS3_SEMI_INVULNERABLE)
    {
        gBattlescriptCurrInstr += 10;
    }
    else
    {
        EmitBattleAnimation(0, *animationIdPtr, *argumentPtr);
        MarkBufferBankForExecution(gActiveBattler);
        gBattlescriptCurrInstr += 10;
    }
}

void atk64_statusanimation(void) {
    if (gBattleExecBuffer) return;
	
    gActiveBattler = GetBattleBank(gBattlescriptCurrInstr[1]);
    
	if (!(gStatuses3[gActiveBattler] & STATUS3_SEMI_INVULNERABLE) && gDisableStructs[gActiveBattler].substituteHP == 0 && !(gHitMarker & HITMARKER_NO_ANIMATIONS)) {
            EmitStatusAnimation(0, 0, gBattleMons[gActiveBattler].status1);
            MarkBufferBankForExecution(gActiveBattler);
    }
    gBattlescriptCurrInstr += 2;
}

void atk66_chosenstatusanimation(void) {
    if (gBattleExecBuffer) return;
	
    u32 status;
    gActiveBattler = GetBattleBank(gBattlescriptCurrInstr[1]);
    status = T1_READ_32(gBattlescriptCurrInstr + 3);
    
	if (!(gStatuses3[gActiveBattler] & STATUS3_SEMI_INVULNERABLE) && gDisableStructs[gActiveBattler].substituteHP == 0 && !(gHitMarker & HITMARKER_NO_ANIMATIONS)) {
            EmitStatusAnimation(0, gBattlescriptCurrInstr[2], status);
            MarkBufferBankForExecution(gActiveBattler);
    }
    gBattlescriptCurrInstr += 7;
}

void atk6A_removeitem(void) {
    u8 bank = gActiveBattler = GetBattleBank(gBattlescriptCurrInstr[1]);
	u8 oldItemEffect = ITEM_EFFECT(bank);
	
	if (!IncinerateCounters[bank]) {
		CONSUMED_ITEMS(bank) = gBattleMons[bank].item;
		SAVED_CONSUMED_ITEMS(bank) = gBattleMons[bank].item;
		UnburdenBoosts |= 1 << bank;
	}
	
    gBattleMons[bank].item = 0;
    EmitSetMonData(0, REQUEST_HELDITEM_BATTLE, 0, 2, &gBattleMons[bank].item);
    MarkBufferBankForExecution(bank);
    gBattlescriptCurrInstr += 2;
	
	u8 partner = PARTNER(bank);
	u16 partnerItem = gBattleMons[partner].item;
	
	if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE 
	&& NoSymbiosisByte == FALSE
	&& ABILITY(partner) == ABILITY_SYMBIOSIS
	&& (!IncinerateCounters[bank] || oldItemEffect == ITEM_EFFECT_AIR_BALLOON) //Air Balloons can't be restored by Recycle, but they trigger Symbiosis
	&& !(gWishFutureKnock->knockedOffPokes[GetBattlerSide(bank)] & gBitTable[gBattlerPartyIndexes[bank]])
	&& gBattleMons[partner].item
	&& CanTransferItem(gBattleMons[bank].species, partnerItem, GetBankPartyData(bank))
	&& CanTransferItem(gBattleMons[partner].species, partnerItem, GetBankPartyData(bank))) {
		gBattleMons[bank].item = partnerItem;
		gBattleMons[partner].item = 0;
		
		EmitSetMonData(0, REQUEST_HELDITEM_BATTLE, 0, 2, &gBattleMons[bank].item);
		MarkBufferBankForExecution(bank);
		
		EmitSetMonData(0, REQUEST_HELDITEM_BATTLE, 0, 2, &gBattleMons[partner].item);
		MarkBufferBankForExecution(partner);
		
		gEffectBank = bank;
		gBattleScripting->bank = partner;
		
		BattleScriptPushCursor();
		gBattlescriptCurrInstr = BattleScript_Symbiosis;
	}
	
	NoSymbiosisByte = FALSE;
	IncinerateCounters[bank] = 0;
	gWishFutureKnock->knockedOffPokes[GetBattlerSide(bank)] &= ~(gBitTable[gBattlerPartyIndexes[bank]]);
}

void atk70_recordlastability(void) {
    gActiveBattler = GetBattleBank(gBattlescriptCurrInstr[1]);
    RecordAbilityBattle(gActiveBattler, gLastUsedAbility);
    gBattlescriptCurrInstr += 2;
}

void atk77_setprotect(void) {
    bool8 not_last_turn = TRUE;
    u16 last_move = gLastUsedMoves3[gBankAttacker];
	u16 rate = 0xFFFF;
	u16 divisor = 1;
	int i;
	
	switch (last_move) {
		case MOVE_PROTECT:
		case MOVE_DETECT:
		case MOVE_ENDURE:
		case MOVE_KINGSSHIELD:
		case MOVE_SPIKYSHIELD:
		case MOVE_BANEFULBUNKER:
		case MOVE_CRAFTYSHIELD:
		case MOVE_MATBLOCK:
		case MOVE_QUICKGUARD:
		case MOVE_WIDEGUARD:
			break;
		default:
			gDisableStructs[gBankAttacker].protectUses = 0;
	}
	
    if (gCurrentTurnActionNumber == (gBattlersCount - 1)) {
        not_last_turn = FALSE;
	}
	
	if (gCurrentMove == MOVE_WIDEGUARD || gCurrentMove == MOVE_QUICKGUARD || gCurrentMove == MOVE_CRAFTYSHIELD || gCurrentMove ==MOVE_MATBLOCK)
		divisor = 1; //These moves increase the fail rate, but can't fail themselves
	else {
		for (i = 0; i < gDisableStructs[gBankAttacker].protectUses && i <= 6; ++i)
			divisor *= 3;
	}
	
    if (udivsi(rate, divisor) >= Random() && not_last_turn) {
	
        if (gBattleMoves[gCurrentMove].effect == EFFECT_PROTECT) {
			u8 atkSide = GetBattlerSide(gBankAttacker);
			switch (gCurrentMove) {
				case MOVE_KINGSSHIELD:
					gProtectStructs[gBankAttacker].KingsShield = 1;
					break;
				
				case MOVE_SPIKYSHIELD:
					gProtectStructs[gBankAttacker].SpikyShield = 1;
					break;
				
				case MOVE_BANEFULBUNKER:
					gProtectStructs[gBankAttacker].BanefulBunker = 1;
					break;
				
				case MOVE_CRAFTYSHIELD:
					gSideAffecting[atkSide] |= SIDE_STATUS_CRAFTY_SHIELD;
					break;
				
				case MOVE_MATBLOCK:
					if (!gDisableStructs[gBankAttacker].isFirstTurn)
						goto PROTECT_FAILED;
					
					gSideAffecting[atkSide] |= SIDE_STATUS_MAT_BLOCK;
					break;
				
				case MOVE_QUICKGUARD:
					gSideAffecting[atkSide] |= SIDE_STATUS_QUICK_GUARD;
					break;
				
				case MOVE_WIDEGUARD:
					gSideAffecting[atkSide] |= SIDE_STATUS_WIDE_GUARD;
					break;
				
				default:
					gProtectStructs[gBankAttacker].protected = 1;
			}
            gBattleCommunication[MULTISTRING_CHOOSER] = 0;
        }
		
        else if (gBattleMoves[gCurrentMove].effect == EFFECT_ENDURE) {
            gProtectStructs[gBankAttacker].endured = 1;
            gBattleCommunication[MULTISTRING_CHOOSER] = 1;
        }
		
        ++gDisableStructs[gBankAttacker].protectUses;
    }
	
    else {
	PROTECT_FAILED:
        gDisableStructs[gBankAttacker].protectUses = 0;
        gBattleCommunication[MULTISTRING_CHOOSER] = 2;
        gMoveResultFlags |= MOVE_RESULT_MISSED;
    }

    gBattlescriptCurrInstr++;
}

void atk7A_jumpifnexttargetvalid(void) {
    u8* jump_loc = T1_READ_PTR(gBattlescriptCurrInstr + 1);

    if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE) {

		if (gBankAttacker == GetBattlerAtPosition(B_POSITION_PLAYER_RIGHT)
		&& gBankTarget == GetBattlerAtPosition(B_POSITION_PLAYER_LEFT)) {
			gBattlescriptCurrInstr += 5;
			return;
		}
		
        for (gBankTarget++; gBankTarget < gBattlersCount; gBankTarget++) {
            if (gBankTarget == gBankAttacker && !(gBattleMoves[gCurrentMove].target & MOVE_TARGET_USER))
                continue;
            if (!(gAbsentBattlerFlags & gBitTable[gBankTarget]))
                break;
        }
		
        if (gBankTarget >= gBattlersCount)
			if (gBankAttacker == GetBattlerAtPosition(B_POSITION_PLAYER_RIGHT)
			&&  !(gAbsentBattlerFlags & gBitTable[GetBattlerAtPosition(B_POSITION_PLAYER_LEFT)]))
			{
				gBankTarget = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);
				gBattlescriptCurrInstr = jump_loc;
			}
			else
				gBattlescriptCurrInstr += 5;
        else
			gBattlescriptCurrInstr = jump_loc;
    }
    else
        gBattlescriptCurrInstr += 5;
}

void atk7D_setrain(void) {
    if (gBattleWeather & (WEATHER_RAIN_ANY | WEATHER_PRIMAL_ANY)) {
        gMoveResultFlags |= MOVE_RESULT_MISSED;
        gBattleCommunication[MULTISTRING_CHOOSER] = 2;
    }
    else {
        gBattleWeather = WEATHER_RAIN_TEMPORARY;
        gBattleCommunication[MULTISTRING_CHOOSER] = 0;
		
		if (ITEM_EFFECT(gBankAttacker) == ITEM_EFFECT_DAMP_ROCK)
			gWishFutureKnock->weatherDuration = 8;
		else
			gWishFutureKnock->weatherDuration = 5;
    }
    gBattlescriptCurrInstr++;
}

void atk7E_setreflect(void) {
    if (gSideAffecting[SIDE(gBankAttacker)] & SIDE_STATUS_REFLECT) {
        gMoveResultFlags |= MOVE_RESULT_MISSED;
        gBattleCommunication[MULTISTRING_CHOOSER] = 0;
    }
    else {
        gSideAffecting[SIDE(gBankAttacker)] |= SIDE_STATUS_REFLECT;
		
		if (ITEM_EFFECT(gBankAttacker) == ITEM_EFFECT_LIGHT_CLAY)
			gSideTimers[SIDE(gBankAttacker)].reflectTimer = 8;
		else
			gSideTimers[SIDE(gBankAttacker)].reflectTimer = 5;
		
        if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE && CountAliveMons(1) == 2)
            gBattleCommunication[MULTISTRING_CHOOSER] = 2;
        else
            gBattleCommunication[MULTISTRING_CHOOSER] = 1;
    }
    gBattlescriptCurrInstr++;
}

void atk7F_setseeded(void) {
    if ((gMoveResultFlags & MOVE_RESULT_NO_EFFECT) || (gStatuses3[gBankTarget] & STATUS3_LEECHSEED)) {
        gMoveResultFlags |= MOVE_RESULT_MISSED;
        gBattleCommunication[MULTISTRING_CHOOSER] = 1;
    }
	
    else if (IsOfType(gBankTarget, TYPE_GRASS)) {
        gMoveResultFlags |= MOVE_RESULT_MISSED;
        gBattleCommunication[MULTISTRING_CHOOSER] = 2;
    }
	
    else {
        gStatuses3[gBankTarget] |= gBankAttacker;
        gStatuses3[gBankTarget] |= STATUS3_LEECHSEED;
        gBattleCommunication[MULTISTRING_CHOOSER] = 0;
    }

    gBattlescriptCurrInstr++;
}

void atk81_trysetrest(void) {
    u8* fail_loc = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    gActiveBattler = gBankTarget = gBankAttacker;
    gBattleMoveDamage = gBattleMons[gBankTarget].maxHP * (-1);
	
    if (gBattleMons[gBankTarget].hp == gBattleMons[gBankTarget].maxHP)
        gBattlescriptCurrInstr = fail_loc;
	
	else if (!CanBePutToSleep(gBankTarget) && !(gBattleMons[gBankTarget].status1 & STATUS_SLEEP))
		gBattlescriptCurrInstr = fail_loc;
	
    else {
        if (gBattleMons[gBankTarget].status1 & STATUS_SLEEP)
            gBattleCommunication[MULTISTRING_CHOOSER] = 0;
        else
			gBattleCommunication[MULTISTRING_CHOOSER] = 1;

        gBattleMons[gBankTarget].status1 = 3;
        EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 4, &gBattleMons[gActiveBattler].status1);
        MarkBufferBankForExecution(gActiveBattler);
        gBattlescriptCurrInstr += 5;
    }
}

void atk84_jumpifcantmakeasleep(void) {
	u8 bankDef = gBankTarget;
	u8 defPartner = PARTNER(bankDef);
	u8 defAbility = ABILITY(bankDef);
	u8 defPartnerAbility = ABILITY(defPartner);
	
    u8* jump_loc = T1_READ_PTR(gBattlescriptCurrInstr + 1);
	
    if (UproarWakeUpCheck(bankDef) || gBattleMons[bankDef].species == PKMN_MINIORSHIELD)
        gBattlescriptCurrInstr = jump_loc;
	
    else if (defAbility == ABILITY_INSOMNIA || defAbility == ABILITY_VITALSPIRIT || defAbility == ABILITY_COMATOSE || defAbility == ABILITY_SWEETVEIL
	|| (defAbility == ABILITY_LEAFGUARD && WEATHER_HAS_EFFECT && gBattleWeather & WEATHER_SUN_ANY)
	|| (defAbility == ABILITY_FLOWERVEIL && IsOfType(bankDef, TYPE_GRASS) && gCurrentMove != MOVE_REST)) {
        gLastUsedAbility = defAbility;
        gBattleCommunication[MULTISTRING_CHOOSER] = 2;
        gBattlescriptCurrInstr = jump_loc;
        RecordAbilityBattle(bankDef, gLastUsedAbility);
    }
	
	else if (defPartnerAbility == ABILITY_SWEETVEIL 
	|| (defPartnerAbility == ABILITY_FLOWERVEIL && IsOfType(bankDef, TYPE_GRASS) && gCurrentMove != MOVE_REST)) {
		gLastUsedAbility = defPartnerAbility;
		gBattlescriptCurrInstr = jump_loc;
		RecordAbilityBattle(defPartner, gLastUsedAbility);
	}
	
	else if (CheckGrounding(bankDef) && (TerrainType == ELECTRIC_TERRAIN || TerrainType == MISTY_TERRAIN))
		gBattlescriptCurrInstr = jump_loc;
		
    else
        gBattlescriptCurrInstr += 5;
}

//This function used to calculate damage, but now all it does is check if Spit Up can be used
void atk86_stockpiletobasedamage(void) {
    u8* jump_loc = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    if (gDisableStructs[gBankAttacker].stockpileCounter == 0)
        gBattlescriptCurrInstr = jump_loc;

    else 
        gBattlescriptCurrInstr += 5;
}

void atk88_negativedamage(void) {	
	if (gCurrentMove == MOVE_OBLIVIONWING)
		gBattleMoveDamage = udivsi(75 * gHpDealt, 100);
	else
		gBattleMoveDamage = (gHpDealt / 2);
	
	if (ITEM_EFFECT(gBankAttacker) == ITEM_EFFECT_BIGROOT)
		gBattleMoveDamage = udivsi(130 * gHpDealt, 100);
	
	gBattleMoveDamage *= -1;
	
    if (gBattleMoveDamage == 0)
        gBattleMoveDamage = -1;
	
    gBattlescriptCurrInstr++;
}

void atk8D_setmultihitcounter(void) {
    if (gBattlescriptCurrInstr[1])
        gMultiHitCounter = gBattlescriptCurrInstr[1];
	
    else if (ABILITY(gBankAttacker) == ABILITY_SKILLLINK)
		gMultiHitCounter = 5;
	
	else {
        gMultiHitCounter = Random() & 3;
        if (gMultiHitCounter > 1)
            gMultiHitCounter = (Random() & 3) + 2;
        else
            gMultiHitCounter += 2;
    }
    gBattlescriptCurrInstr += 2;
}

void atk90_tryconversiontypechange(void) {
	u8* fail_script = T1_READ_PTR(gBattlescriptCurrInstr + 1);
	
	if (gCurrentMove != MOVE_REFLECTTYPE) { //Conversion
		u8 moveType = gBattleMoves[gBattleMons[gBankAttacker].moves[0]].type;
		
		if (IS_BLANK_TYPE(moveType))
			gBattlescriptCurrInstr = fail_script;
		
		else if (
			moveType == gBattleMons[gBankAttacker].type1
		||  moveType == gBattleMons[gBankAttacker].type2
		||  moveType == gBattleMons[gBankAttacker].type3)
			gBattlescriptCurrInstr = fail_script;
		
		else {
			gBattleMons[gBankAttacker].type1 = moveType;
			gBattleMons[gBankAttacker].type2 = moveType;
			gBattleMons[gBankAttacker].type3 = TYPE_BLANK;
		
			PREPARE_TYPE_BUFFER(gBattleTextBuff1, moveType);

			gBattlescriptCurrInstr += 5;
		}
	}
	
	else { //Reflect Type
		u8 defType1 = gBattleMons[gBankTarget].type1;
		u8 defType2 = gBattleMons[gBankTarget].type2;
		u8 defType3 = gBattleMons[gBankTarget].type3;
		
		//If target has no type
		if((IS_BLANK_TYPE(defType1))
		&& (IS_BLANK_TYPE(defType2))
		&& (IS_BLANK_TYPE(defType3)))
			gBattlescriptCurrInstr = fail_script;
			
		else {
			//If target has no main types, but has a third type
			if ((IS_BLANK_TYPE(defType1))
			&&  (IS_BLANK_TYPE(defType2))
			&& !(IS_BLANK_TYPE(defType3))) {
				defType1 = TYPE_NORMAL;
				defType2 = TYPE_NORMAL;
			}
			
			else { //Target Has Main Type
				if (IS_BLANK_TYPE(defType1))
					defType1 = defType2;
				else if (IS_BLANK_TYPE(defType2))
					defType2 = defType1;
				
				if (IS_BLANK_TYPE(defType3)) //Just in case it has a burned out Fire type
					defType3 = TYPE_BLANK;
			}
			
			gBattleMons[gBankAttacker].type1 = defType1;
			gBattleMons[gBankAttacker].type2 = defType2;
			gBattleMons[gBankAttacker].type3 = defType3;
			
			gBattlescriptCurrInstr += 5;
		}
	}
}

void atk91_givepaydaymoney(void) {
	int i;
	u32 money = 0;
	
    if (!(gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_FRONTIER | BATTLE_TYPE_TRAINER_TOWER))) {
		
		for (i = 0; i < 6; ++i)
			money += (gPlayerParty[i].level * 5) * PayDayByPartyIndices[i];
		
		money *= gBattleStruct->moneyMultiplier;
		money = MathMin(money, 0xFFFF);
		
		if (money) {
			AddMoney(&gSaveBlock1->money, money);
			PREPARE_HWORD_NUMBER_BUFFER(gBattleTextBuff1, 5, money);
			BattleScriptPush(gBattlescriptCurrInstr + 1);
			gBattlescriptCurrInstr = BattleScript_PrintPayDayMoneyString;
			return;
		}
    }
	gBattlescriptCurrInstr++;
}

void atk92_setlightscreen(void) {
    if (gSideAffecting[SIDE(gBankAttacker)] & SIDE_STATUS_LIGHTSCREEN) {
        gMoveResultFlags |= MOVE_RESULT_MISSED;
        gBattleCommunication[MULTISTRING_CHOOSER] = 0;
    }
	
    else {
        gSideAffecting[SIDE(gBankAttacker)] |= SIDE_STATUS_LIGHTSCREEN;
		
		if (ITEM_EFFECT(gBankAttacker) == ITEM_EFFECT_LIGHT_CLAY)
			gSideTimers[SIDE(gBankAttacker)].lightscreenTimer = 8;
		else
			gSideTimers[SIDE(gBankAttacker)].lightscreenTimer = 5;
		
        if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE && CountAliveMons(1) == 2)
            gBattleCommunication[MULTISTRING_CHOOSER] = 4;
        else
            gBattleCommunication[MULTISTRING_CHOOSER] = 3;
    }
    gBattlescriptCurrInstr++;
}

void atk93_tryKO(void) {
	u8 bankAtk = gBankAttacker;
	u8 bankDef = gBankTarget;
	
	u8 atkAbility = ABILITY(bankAtk);
	u8 defAbility = ABILITY(bankDef);
	u8 defEffect = ITEM_EFFECT(bankDef);
    u8 defQuality = ITEM_QUALITY(bankDef);
	u8 mystery = ItemId_GetMystery2(ITEM(bankDef));
	
    gStringBank = bankDef;

    if (atkAbility == ABILITY_STURDY) {
        gMoveResultFlags |= MOVE_RESULT_MISSED;
        gLastUsedAbility = ABILITY_STURDY;
        gBattlescriptCurrInstr = BattleScript_SturdyPreventsOHKO;
        RecordAbilityBattle(bankDef, ABILITY_STURDY);
    }
	
    else {
		u16 chance = FALSE;
		u8 baseAcc;
		#ifdef NO_SHEER_COLD_NERF
			baseAcc = gBattleMoves[gCurrentMove].accuracy;
		#else
			if (gCurrentMove == MOVE_SHEERCOLD && !IsOfType(bankAtk, TYPE_ICE))
				baseAcc = 20;
			else
				baseAcc = gBattleMoves[gCurrentMove].accuracy;
		#endif
		  
		if (gBattleMons[bankAtk].level >= gBattleMons[bankDef].level) {
			if (((gStatuses3[bankDef] & STATUS3_ALWAYS_HITS && gDisableStructs[bankDef].bankWithSureHit == bankAtk)
			|| atkAbility == ABILITY_NOGUARD || defAbility == ABILITY_NOGUARD))
				chance = TRUE;
			
			#ifndef NO_SHEER_COLD_NERF
			else if (gCurrentMove == MOVE_SHEERCOLD && IsOfType(bankDef, TYPE_ICE))
				chance = FALSE;
			#endif
				
			else {
				chance = baseAcc + (gBattleMons[bankAtk].level - gBattleMons[bankDef].level);
				if (umodsi(Random(), 100) + 1 < chance)
					chance = TRUE;
				else
					chance = FALSE;
			}
		}
		else
			chance = FALSE;
		
        if (chance) {
		    if (defEffect == ITEM_EFFECT_FOCUS_BAND && !mystery && umodsi(Random(), 100) < defQuality) {
				RecordItemBattle(bankDef, defEffect);
				gSpecialStatuses[bankDef].focusBanded = 1;
			}
			//Focus Sash
			else if (defEffect == ITEM_EFFECT_FOCUS_BAND && mystery && (gBattleMons[bankDef].hp == gBattleMons[bankDef].maxHP)) {
				RecordItemBattle(bankDef, defEffect);
				gSpecialStatuses[bankDef].focusBanded = 1;
				EnduranceHelper = ENDURE_FOCUS_SASH;
			}
			
            if (gProtectStructs[bankDef].endured) {
                gBattleMoveDamage = gBattleMons[bankDef].hp - 1;
                gMoveResultFlags |= MOVE_RESULT_FOE_ENDURED;
            }
            else if (gSpecialStatuses[bankDef].focusBanded) {
                gBattleMoveDamage = gBattleMons[bankDef].hp - 1;
                gMoveResultFlags |= MOVE_RESULT_FOE_HUNG_ON;
                gLastUsedItem = gBattleMons[bankDef].item;
            }
            else {
                gBattleMoveDamage = gBattleMons[bankDef].hp;
                gMoveResultFlags |= MOVE_RESULT_ONE_HIT_KO;
            }
            gBattlescriptCurrInstr += 5;
        }
		
        else {
            gMoveResultFlags |= MOVE_RESULT_MISSED;
			#ifdef NO_SHEER_COLD_NERF
				if (gBattleMons[bankAtk].level >= gBattleMons[bankDef].level)
					gBattleCommunication[MULTISTRING_CHOOSER] = 0;
				else
					gBattleCommunication[MULTISTRING_CHOOSER] = 1;
			#else
				if (gBattleMons[bankAtk].level < gBattleMons[bankDef].level
				|| (gCurrentMove == MOVE_SHEERCOLD && IsOfType(bankDef, TYPE_ICE)))
					gBattleCommunication[MULTISTRING_CHOOSER] = 1;
				else
					gBattleCommunication[MULTISTRING_CHOOSER] = 0;
			#endif
			
            gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
        }
    }
}

void atk94_damagetohalftargethp(void) { //Super Fang
	switch (gCurrentMove) {
		case MOVE_GUARDIAN_OF_ALOLA:
			gBattleMoveDamage = udivsi(gBattleMons[gBankTarget].hp * 75, 100);
			if (ProtectsAgainstZMoves(gCurrentMove, gBankAttacker, gBankTarget))
				gBattleMoveDamage = udivsi(gBattleMoveDamage * 25, 100);
			break;
			
		default:
			gBattleMoveDamage = gBattleMons[gBankTarget].hp / 2;
	}
	
    if (gBattleMoveDamage == 0)
        gBattleMoveDamage = 1;
    gBattlescriptCurrInstr++;
}

void atk95_setsandstorm(void) {
    if (gBattleWeather & (WEATHER_SANDSTORM_ANY | WEATHER_PRIMAL_ANY)) {
        gMoveResultFlags |= MOVE_RESULT_MISSED;
        gBattleCommunication[MULTISTRING_CHOOSER] = 2;
    }
	
    else {
        gBattleWeather = WEATHER_SANDSTORM_TEMPORARY;
        gBattleCommunication[MULTISTRING_CHOOSER] = 3;
		
		if (ITEM_EFFECT(gBankAttacker) == ITEM_EFFECT_SMOOTH_ROCK)
			gWishFutureKnock->weatherDuration = 8;
		else
			gWishFutureKnock->weatherDuration = 5;
    }
    gBattlescriptCurrInstr++;
}

void atk96_weatherdamage(void) {
	u8 bank = gBankAttacker;
	u8 ability = ABILITY(bank);
	u8 effect = ITEM_EFFECT(bank);
	
	if (gAbsentBattlerFlags & gBitTable[bank])
        gBattleMoveDamage = 0;
		
	else if (WEATHER_HAS_EFFECT
		&& 	ability != ABILITY_MAGICGUARD
		&&	ability != ABILITY_OVERCOAT
		&&	effect  != ITEM_EFFECT_SAFETY_GOGGLES
		&& !(gStatuses3[bank] & (STATUS3_UNDERGROUND | STATUS3_UNDERWATER | STATUS3_DISAPPEARED)))
	{
	
        if (gBattleWeather & WEATHER_SANDSTORM_ANY) {
            if (!IsOfType(bank, TYPE_ROCK) && !IsOfType(bank, TYPE_GROUND) && !IsOfType(bank, TYPE_STEEL)
             && ability != ABILITY_SANDVEIL && ability != ABILITY_SANDRUSH && ability != ABILITY_SANDFORCE)
                gBattleMoveDamage = MathMax(1, gBattleMons[bank].maxHP / 16);
            else
                gBattleMoveDamage = 0;
        }
		
        if (gBattleWeather & WEATHER_HAIL_ANY) {
            if (!IsOfType(bank, TYPE_ICE) && ability != ABILITY_ICEBODY && ability != ABILITY_SNOWCLOAK && ability != ABILITY_SLUSHRUSH)
				gBattleMoveDamage = MathMax(1, gBattleMons[bank].maxHP / 16);
            else
                gBattleMoveDamage = 0;
        }
    }
	
    else
        gBattleMoveDamage = 0;

    gBattlescriptCurrInstr++;
}

void atkA0_psywavedamageeffect(void)
{
    s32 randDamage = umodsi(Random(), 100);
	
    gBattleMoveDamage = MathMax(1, udivsi(gBattleMons[gBankAttacker].level * (randDamage + 50), 100));
    ++gBattlescriptCurrInstr;
}

void atkA1_counterdamagecalculator(void) {
    u8 atkSide = SIDE(gBankAttacker);
    u8 defSide = SIDE(gProtectStructs[gBankAttacker].physicalBank);
	
    if (gProtectStructs[gBankAttacker].physicalDmg && atkSide != defSide && gBattleMons[gProtectStructs[gBankAttacker].physicalBank].hp) {
	
		if (!gSpecialStatuses[gBankAttacker].moveturnLostHP)
			gBattleMoveDamage = 1; //Suffered physical damage, but negated damage
		else
			gBattleMoveDamage = gProtectStructs[gBankAttacker].physicalDmg * 2;
		
        if (gSideTimers[defSide].followmeTimer && gBattleMons[gSideTimers[defSide].followmeTarget].hp)
            gBankTarget = gSideTimers[defSide].followmeTarget;
        else
            gBankTarget = gProtectStructs[gBankAttacker].physicalBank;
        gBattlescriptCurrInstr += 5;
    }
	
    else {
        gSpecialStatuses[gBankAttacker].flag20 = 1;
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    }
}

void atkA2_mirrorcoatdamagecalculator(void) {
    u8 atkSide = SIDE(gBankAttacker);
    u8 defSide = SIDE(gProtectStructs[gBankAttacker].specialBank);
	
    if (gProtectStructs[gBankAttacker].specialDmg && atkSide != defSide && gBattleMons[gProtectStructs[gBankAttacker].specialBank].hp) {
	
		if (!gSpecialStatuses[gBankAttacker].moveturnLostHP)
			gBattleMoveDamage = 1; //Suffered special damage, but negated damage
		else
			gBattleMoveDamage = gProtectStructs[gBankAttacker].specialDmg * 2;
		
        if (gSideTimers[defSide].followmeTimer && gBattleMons[gSideTimers[defSide].followmeTarget].hp)
            gBankTarget = gSideTimers[defSide].followmeTarget;
        else
            gBankTarget = gProtectStructs[gBankAttacker].specialBank;
        gBattlescriptCurrInstr += 5;
    }
	
    else {
        gSpecialStatuses[gBankAttacker].flag20 = 1;
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    }
}

void atkA6_settypetorandomresistance(void) { //Conversion 2
	u8 bankAtk = gBankAttacker;
	u8 bankDef = gBankTarget;
	u32 i, j, rands;

    if (gLastUsedMoves[bankDef] == 0 || gLastUsedMoves[bankDef] == 0xFFFF) {
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
		return;
	}

    for (rands = 0; rands < 1000; ++rands) {
		
        while (((i = (Random() & 0x7F)) > udivsi(sizeof(gTypeEffectiveness), 3)));

        i *= 3;
		
		#ifdef INVERSE_BATTLES
			if (FlagGet(INVERSE_FLAG)) {
				if (TYPE_EFFECT_ATK_TYPE(i) == LastUsedTypes[bankDef]
				&& TYPE_EFFECT_MULTIPLIER(i) >= TYPE_MUL_SUPER_EFFECTIVE
				&& !IsOfType(bankAtk, TYPE_EFFECT_DEF_TYPE(i)))
				{
					SET_BATTLER_TYPE(bankAtk, TYPE_EFFECT_DEF_TYPE(i));
					PREPARE_TYPE_BUFFER(gBattleTextBuff1, TYPE_EFFECT_DEF_TYPE(i));

					gBattlescriptCurrInstr += 5;
					return;
				}
			}
			
			else {
				if (TYPE_EFFECT_ATK_TYPE(i) == LastUsedTypes[bankDef]
				&& TYPE_EFFECT_MULTIPLIER(i) <= TYPE_MUL_NOT_EFFECTIVE
				&& !IsOfType(bankAtk, TYPE_EFFECT_DEF_TYPE(i)))
				{
					SET_BATTLER_TYPE(bankAtk, TYPE_EFFECT_DEF_TYPE(i));
					PREPARE_TYPE_BUFFER(gBattleTextBuff1, TYPE_EFFECT_DEF_TYPE(i));

					gBattlescriptCurrInstr += 5;
					return;
				}
			}
		#else
			if (TYPE_EFFECT_ATK_TYPE(i) == LastUsedTypes[bankDef]
			&& TYPE_EFFECT_MULTIPLIER(i) <= TYPE_MUL_NOT_EFFECTIVE
			&& !IsOfType(bankAtk, TYPE_EFFECT_DEF_TYPE(i)))
			{
				SET_BATTLER_TYPE(bankAtk, TYPE_EFFECT_DEF_TYPE(i));
				PREPARE_TYPE_BUFFER(gBattleTextBuff1, TYPE_EFFECT_DEF_TYPE(i));

				gBattlescriptCurrInstr += 5;
				return;
			}
		#endif
    }

    for (j = 0, rands = 0; rands < sizeof(gTypeEffectiveness); j += 3, rands += 3) {
        switch (TYPE_EFFECT_ATK_TYPE(j)) {
			case TYPE_ENDTABLE:
			case TYPE_FORESIGHT:
				break;
			default:
			#ifdef INVERSE_BATTLES
			if (FlagGet(INVERSE_FLAG)) {
				if (TYPE_EFFECT_ATK_TYPE(j) == LastUsedTypes[bankDef]
				&& TYPE_EFFECT_MULTIPLIER(j) >= TYPE_MUL_SUPER_EFFECTIVE
				&& !IsOfType(bankAtk, TYPE_EFFECT_DEF_TYPE(i)))
				{
					SET_BATTLER_TYPE(bankAtk, TYPE_EFFECT_DEF_TYPE(rands));
					PREPARE_TYPE_BUFFER(gBattleTextBuff1, TYPE_EFFECT_DEF_TYPE(rands))

					gBattlescriptCurrInstr += 5;
					return;
				}
			}
			
			else {
				if (TYPE_EFFECT_ATK_TYPE(j) == LastUsedTypes[bankDef]
				&& TYPE_EFFECT_MULTIPLIER(j) <= TYPE_MUL_NOT_EFFECTIVE
				&& !IsOfType(bankAtk, TYPE_EFFECT_DEF_TYPE(i)))
				{
					SET_BATTLER_TYPE(bankAtk, TYPE_EFFECT_DEF_TYPE(rands));
					PREPARE_TYPE_BUFFER(gBattleTextBuff1, TYPE_EFFECT_DEF_TYPE(rands))

					gBattlescriptCurrInstr += 5;
					return;
				}
			}
		#else
			if (TYPE_EFFECT_ATK_TYPE(j) == LastUsedTypes[bankDef]
			&& TYPE_EFFECT_MULTIPLIER(j) <= TYPE_MUL_NOT_EFFECTIVE
			&& !IsOfType(bankAtk, TYPE_EFFECT_DEF_TYPE(i)))
			{
				SET_BATTLER_TYPE(bankAtk, TYPE_EFFECT_DEF_TYPE(rands));
				PREPARE_TYPE_BUFFER(gBattleTextBuff1, TYPE_EFFECT_DEF_TYPE(rands))

				gBattlescriptCurrInstr += 5;
				return;
			}
		#endif
				
        }
    }

    gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
}

#define MOVE_LIMITATION_ZEROMOVE    (1 << 0)
#define MOVE_LIMITATION_PP          (1 << 1)
#define MOVE_LIMITATION_DISABLED    (1 << 2)
#define MOVE_LIMITATION_TORMENTED   (1 << 3)
#define MOVE_LIMITATION_TAUNT       (1 << 4)
#define MOVE_LIMITATION_IMPRISION   (1 << 5)
#define MOVE_LIMITATION_CHOICE		(1 << 6)

void atkA9_trychoosesleeptalkmove(void) {
    u8 unusable_moves = 0;
    int i;

    for (i = 0; i < 4; ++i) {
        if (CheckTableForMove(gBattleMons[gBankAttacker].moves[i], SleepTalkBanTable))
            unusable_moves |= gBitTable[i];
    }
	
    unusable_moves = CheckMoveLimitations(gBankAttacker, unusable_moves, 0xFF ^ MOVE_LIMITATION_PP ^ MOVE_LIMITATION_DISABLED ^ MOVE_LIMITATION_CHOICE);
    if (unusable_moves == 0xF) //all 4 moves cannot be chosen
        gBattlescriptCurrInstr += 5;
	
    else { //at least one move can be chosen
        u32 random_pos;
        do {
            random_pos = Random() & 3;
        } while ((gBitTable[random_pos] & unusable_moves));

        gRandomMove = gBattleMons[gBankAttacker].moves[random_pos];
        gCurrMovePos = random_pos;
        gHitMarker &= ~(HITMARKER_ATTACKSTRING_PRINTED);
        gBankTarget = GetMoveTarget(gRandomMove, 0);
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
		gHitMarker |= HITMARKER_NO_PPDEDUCT;
    }
}

u8 CheckMoveLimitations(u8 bank, u8 unusableMoves, u8 check) {
    u8 holdEffect = ITEM_EFFECT(bank);
    u16 choicedMove = CHOICED_MOVE(bank);
    int i;
	
    gStringBank = bank;
    for (i = 0; i < 4; i++) {
		u16 move = gBattleMons[bank].moves[i];
        if (move == 0 && check & MOVE_LIMITATION_ZEROMOVE)
            unusableMoves |= gBitTable[i];
        else if (gBattleMons[bank].pp[i] == 0 && check & MOVE_LIMITATION_PP)
            unusableMoves |= gBitTable[i];
        else if (move == gDisableStructs[bank].disabledMove && check & MOVE_LIMITATION_DISABLED)
            unusableMoves |= gBitTable[i];
        else if (move == gLastUsedMoves[bank] && check & MOVE_LIMITATION_TORMENTED && gBattleMons[bank].status2 & STATUS2_TORMENT)
            unusableMoves |= gBitTable[i];
        else if (gDisableStructs[bank].tauntTimer1 && check & MOVE_LIMITATION_TAUNT && SPLIT(move) == SPLIT_STATUS)
            unusableMoves |= gBitTable[i];
        else if (IsImprisoned(bank, move) && check & MOVE_LIMITATION_IMPRISION)
            unusableMoves |= gBitTable[i];
        else if (gDisableStructs[bank].encoreTimer1 && gDisableStructs[bank].encoredMove != move)
            unusableMoves |= gBitTable[i];
        else if (holdEffect == ITEM_EFFECT_CHOICE_BAND && choicedMove != 0 && choicedMove != 0xFFFF && choicedMove != move && check & MOVE_LIMITATION_CHOICE)
            unusableMoves |= gBitTable[i];
		
		else if (holdEffect == ITEM_EFFECT_ASSAULT_VEST && SPLIT(move) == SPLIT_STATUS)
			unusableMoves |= gBitTable[i];
		else if (FlagGet(SKY_BATTLE_FLAG) && CheckTableForMove(move, SkyBattleBanTable))
			unusableMoves |= gBitTable[i];
		else if (GravityTimer && CheckTableForMove(move, GravityBanTable))
			unusableMoves |= gBitTable[i];
		else if (ThroatChopTimers[bank] && CheckSoundMove(move))
			unusableMoves |= gBitTable[i];
		else if (HealBlockTimers[bank] && CheckHealingMove(move))
			unusableMoves |= gBitTable[i];
    }
    return unusableMoves;
}

void atkAA_setdestinybond(void) {
	if (DestinyBondCounters[gBankAttacker])
		gBattlescriptCurrInstr = BattleScript_Failed + 2;
		
	else {
		DestinyBondCounters[gBankAttacker] = 2;
		gBattleMons[gBankAttacker].status2 |= STATUS2_DESTINY_BOND;
		gBattlescriptCurrInstr++;
	}
}

void atkAD_tryspiteppreduce(void) {
    if (gLastUsedMoves[gBankTarget] != 0 
	&&  gLastUsedMoves[gBankTarget] != 0xFFFF 
	&& (!(gStatuses3[gBankTarget] & STATUS3_SEMI_INVULNERABLE) || ABILITY(gBankAttacker) == ABILITY_NOGUARD || ABILITY(gBankTarget) == ABILITY_NOGUARD)) {
        int i;
        for (i = 0; i < 4; i++)
        {
            if (gLastUsedMoves[gBankTarget] == gBattleMons[gBankTarget].moves[i])
                break;
        }
		
        if (i != 4 && gBattleMons[gBankTarget].pp[i] > 0) {
            s32 lost_pp = MathMin(gBattleMons[gBankTarget].pp[i], 4);

            gBattleTextBuff1[0] = 0xFD;
            gBattleTextBuff1[1] = 2;
            gBattleTextBuff1[2] = gLastUsedMoves[gBankTarget];
            gBattleTextBuff1[3] = gLastUsedMoves[gBankTarget] >> 8;
            gBattleTextBuff1[4] = 0xFF;
            ConvertIntToDecimalStringN(gBattleTextBuff2, lost_pp, 0, 1);
            gBattleTextBuff2[0] = 0xFD;
            gBattleTextBuff2[1] = 1;
            gBattleTextBuff2[2] = 1;
            gBattleTextBuff2[3] = 1;
            gBattleTextBuff2[4] = lost_pp;
            gBattleTextBuff2[5] = 0xFF;

            gBattleMons[gBankTarget].pp[i] -= lost_pp;
            gActiveBattler = gBankTarget;
            if (!(gDisableStructs[gActiveBattler].unk18_b & gBitTable[i])
                && !(gBattleMons[gActiveBattler].status2 & STATUS2_TRANSFORMED))
            {
                EmitSetMonData(0, REQUEST_PPMOVE1_BATTLE + i, 0, 1, &gBattleMons[gActiveBattler].pp[i]);
                MarkBufferBankForExecution(gActiveBattler);
            }
            gBattlescriptCurrInstr += 5;
            if (gBattleMons[gBankTarget].pp[i] == 0)
                CancelMultiTurnMoves(gBankTarget);
            return;
        }
    }
    gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
}

void atkAF_cursetarget(void) {
    if (gBattleMons[gBankTarget].status2 & STATUS2_CURSED 
	|| (gStatuses3[gBankTarget] & STATUS3_SEMI_INVULNERABLE && ABILITY(gBankAttacker) != ABILITY_NOGUARD && ABILITY(gBankTarget) != ABILITY_NOGUARD))
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);

    else {
        gBattleMons[gBankTarget].status2 |= STATUS2_CURSED;
        gBattleMoveDamage = MathMax(1, gBattleMons[gBankAttacker].maxHP / 2);
        gBattlescriptCurrInstr += 5;
    }
}

void atkB0_trysetspikes(void) {
	u8 atkSide = SIDE(gBankAttacker);
    u8 defSide = atkSide ^ BIT_SIDE;
	u8 stringcase = 0xFF;
	
	switch (gCurrentMove) {
		case MOVE_STEALTHROCK:
			if (gSideTimers[defSide].srAmount) {
				gSpecialStatuses[gBankAttacker].flag20 = 1;
				gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
			}
			else {
				gSideAffecting[defSide] |= SIDE_STATUS_SPIKES;
				gSideTimers[defSide].srAmount = 1;
				gBattlescriptCurrInstr += 5;
				stringcase = 2;
			}
			break;
		
		case MOVE_TOXICSPIKES:
			if (gSideTimers[defSide].tspikesAmount >= 2) {
				gSpecialStatuses[gBankAttacker].flag20 = 1;
				gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
			}
			else {
				gSideAffecting[defSide] |= SIDE_STATUS_SPIKES;
				gSideTimers[defSide].tspikesAmount++;
				gBattlescriptCurrInstr += 5;
				stringcase = 4;
			}
			break;
		
		case MOVE_STICKYWEB:
			if (gSideTimers[defSide].stickyWeb) {
				gSpecialStatuses[gBankAttacker].flag20 = 1;
				gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
			}
			else {
				gSideAffecting[defSide] |= SIDE_STATUS_SPIKES;
				gSideTimers[defSide].stickyWeb = 1;
				gBattlescriptCurrInstr += 5;
				stringcase = 6;
			}
			break;
		
		default:
			if (gSideTimers[defSide].spikesAmount >= 3) {
				gSpecialStatuses[gBankAttacker].flag20 = 1;
				gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
			}
			else {
				gSideAffecting[defSide] |= SIDE_STATUS_SPIKES;
				gSideTimers[defSide].spikesAmount++;
				gBattlescriptCurrInstr += 5;
				stringcase = 0;
			}							
	}
	
	if (stringcase != 0xFF)
		BattleStringLoader = EntryHazardsStrings[stringcase + atkSide];
}

//In this function, the target attacks the attacker
void atkBA_jumpifnopursuitswitchdmg(void) {
	int i;
	
	switch (gMultiHitCounter) {
		case 1:
			if (SIDE(gBankAttacker) == B_SIDE_PLAYER)
				gBankTarget = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);
			else
				gBankTarget = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);
			break;
		
		default:
			if (SIDE(gBankAttacker) == B_SIDE_PLAYER)
				gBankTarget = GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT);
			else
				gBankTarget = GetBattlerAtPosition(B_POSITION_PLAYER_RIGHT);
	}


    if (gActionForBanks[gBankTarget] == ACTION_USE_MOVE
	//&& gBankAttacker == gBattleStruct->moveTarget[gBankTarget] //Used pre Gen 4
	&& !(gBattleMons[gBankTarget].status1 & (STATUS_SLEEP | STATUS_FREEZE))
    && gBattleMons[gBankAttacker].hp 
	&& !gDisableStructs[gBankTarget].truantCounter 
	&& gChosenMovesByBanks[gBankTarget] == MOVE_PURSUIT) {
        for (i = 0; i < gBattlersCount; i++) {
            if (gBanksByTurnOrder[i] == gBankTarget)
                gActionsByTurnOrder[i] = 11;
        }
		
        gCurrentMove = MOVE_PURSUIT;
        gBattlescriptCurrInstr += 5;
        gBattleScripting->animTurn = 1;
        gHitMarker &= ~(HITMARKER_ATTACKSTRING_PRINTED);
    }
    else
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
}

void atkBB_setsunny(void)
{
    if (gBattleWeather & (WEATHER_SUN_ANY | WEATHER_PRIMAL_ANY)) {
        gMoveResultFlags |= MOVE_RESULT_MISSED;
        gBattleCommunication[MULTISTRING_CHOOSER] = 2;
    }
	
    else {
        gBattleWeather = WEATHER_SUN_TEMPORARY;
        gBattleCommunication[MULTISTRING_CHOOSER] = 4;

		if (ITEM_EFFECT(gBankAttacker) == ITEM_EFFECT_HEAT_ROCK)
			gWishFutureKnock->weatherDuration = 8;
		else
			gWishFutureKnock->weatherDuration = 5;
    }
    gBattlescriptCurrInstr++;
}

void atkBE_rapidspinfree(void) {
	u8 bankAtk = gBankAttacker;
	u8 sideAtk = SIDE(bankAtk);
	u8 sideDef = SIDE(gBankTarget);
	
	if (gCurrentMove != MOVE_DEFOG) {
		if (gBattleMons[bankAtk].status2 & STATUS2_WRAPPED) {
			gBattleMons[bankAtk].status2 &= ~(STATUS2_WRAPPED);
			gBankTarget = gBattleStruct->wrappedBy[bankAtk];
			gBattleTextBuff1[0] = 0xFD;
			gBattleTextBuff1[1] = 2;
			gBattleTextBuff1[2] = gBattleStruct->wrappedMove[bankAtk * 2];
			gBattleTextBuff1[3] = gBattleStruct->wrappedMove[bankAtk * 2 + 1];
			gBattleTextBuff1[4] = 0xFF;
			BattleScriptPushCursor();
			gBattlescriptCurrInstr = BattleScript_WrapFree;
		}
		
		else if (gStatuses3[bankAtk] & STATUS3_LEECHSEED) {
			gStatuses3[bankAtk] &= ~(STATUS3_LEECHSEED);
			gStatuses3[bankAtk] &= ~(STATUS3_LEECHSEED_BATTLER);
			BattleScriptPushCursor();
			gBattlescriptCurrInstr = BattleScript_LeechSeedFree;
		}
		
		else if (gSideAffecting[sideAtk] & SIDE_STATUS_SPIKES) {
			gSideAffecting[sideAtk] &= ~(SIDE_STATUS_SPIKES);
			gSideTimers[sideAtk].spikesAmount = 0;
			gSideTimers[sideAtk].tspikesAmount = 0;
			gSideTimers[sideAtk].srAmount = 0;
			gSideTimers[sideAtk].stickyWeb = 0;
			BattleScriptPushCursor();
			gBattlescriptCurrInstr = BattleScript_SpikesFree;
		}
		
		else
			gBattlescriptCurrInstr++;
	}
	
	else { //Defog
		if (gSideAffecting[sideAtk] & SIDE_STATUS_SPIKES) {
			gSideAffecting[sideAtk] &= ~(SIDE_STATUS_SPIKES);
			gSideTimers[sideAtk].spikesAmount = 0;
			gSideTimers[sideAtk].tspikesAmount = 0;
			gSideTimers[sideAtk].srAmount = 0;
			gSideTimers[sideAtk].stickyWeb = 0;
			BattleScriptPushCursor();
			gBattlescriptCurrInstr = BattleScript_SpikesFreeDefog;
			BattleStringLoader = EntryHazardDefogRemovalStrings[sideAtk];
		}
		
		else if (gSideAffecting[sideDef] & SIDE_STATUS_SPIKES) {
			gSideAffecting[sideDef] &= ~(SIDE_STATUS_SPIKES);
			gSideTimers[sideDef].spikesAmount = 0;
			gSideTimers[sideDef].tspikesAmount = 0;
			gSideTimers[sideDef].srAmount = 0;
			gSideTimers[sideDef].stickyWeb = 0;
			BattleScriptPushCursor();
			gBattlescriptCurrInstr = BattleScript_SpikesFreeDefog;
			BattleStringLoader = EntryHazardDefogRemovalStrings[sideDef];
		}
		
		else if (gSideAffecting[sideDef] & (SIDE_STATUS_REFLECT)) {
			gSideTimers[sideDef].reflectTimer = 0;
			TEXT_BUFFER_SIDE_STATUS(MOVE_REFLECT, SIDE_STATUS_REFLECT, sideDef);
		}
		
		else if (gSideAffecting[sideDef] & (SIDE_STATUS_LIGHTSCREEN)) {
			gSideTimers[sideDef].lightscreenTimer = 0;
			TEXT_BUFFER_SIDE_STATUS(MOVE_LIGHTSCREEN, SIDE_STATUS_LIGHTSCREEN, sideDef);
		}
		
		else if (gSideAffecting[sideDef] & (SIDE_STATUS_SAFEGUARD)) {
			gSideTimers[sideDef].safeguardTimer = 0;
			TEXT_BUFFER_SIDE_STATUS(MOVE_SAFEGUARD, SIDE_STATUS_SAFEGUARD, sideDef);
		}
		
		else if (gSideAffecting[sideDef] & (SIDE_STATUS_MIST)) {
			gSideTimers[sideDef].mistTimer = 0;
			TEXT_BUFFER_SIDE_STATUS(MOVE_MIST, SIDE_STATUS_MIST, sideDef);
		}
		
		else if (AuroraVeilTimers[sideDef]) {
			AuroraVeilTimers[sideDef] = 0;
			TEXT_BUFFER_SIDE_STATUS(MOVE_AURORAVEIL, 0, sideDef);
		}
		
		else
			gBattlescriptCurrInstr++;
	}
}

void atkC0_recoverbasedonsunlight(void) {
    gBankTarget = gBankAttacker;
    if (gBattleMons[gBankAttacker].hp < gBattleMons[gBankAttacker].maxHP) {
	
		if (!gBattleWeather || !WEATHER_HAS_EFFECT)
			gBattleMoveDamage = gBattleMons[gBankAttacker].maxHP / 2;
		
		else if (gCurrentMove == MOVE_SHOREUP && gBattleWeather & WEATHER_SANDSTORM_ANY)
			gBattleMoveDamage = udivsi(20 * gBattleMons[gBankAttacker].maxHP, 30);
			
		else if (gBattleWeather & WEATHER_SUN_ANY)
			gBattleMoveDamage = udivsi(20 * gBattleMons[gBankAttacker].maxHP, 30);
		
		else //Not Sunny Weather
			gBattleMoveDamage = gBattleMons[gBankAttacker].maxHP / 4;
		
		gBattleMoveDamage = MathMax(1, gBattleMoveDamage);
        gBattleMoveDamage *= -1;
        gBattlescriptCurrInstr += 5;
    }
	
    else
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
}

void atkC2_selectfirstvalidtarget(void) {
    for (gBankTarget = 0; gBankTarget < gBattlersCount; ++gBankTarget) {
        if (gBankTarget == gBankAttacker) continue;
		
        if (!(gAbsentBattlerFlags & gBitTable[gBankTarget])) break;
    }
	
	//Stops certain animations from acting like they only target the partner
	if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE
	&&  gBankAttacker == GetBattlerAtPosition(B_POSITION_PLAYER_RIGHT)
	&&  gBankTarget == GetBattlerAtPosition(B_POSITION_PLAYER_LEFT)) 
	{
		if (IsBattlerAlive(GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT)))
			gBankTarget = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);
		else if (IsBattlerAlive(GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT)))
			gBankTarget = GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT);
	}
	
    gBattlescriptCurrInstr++;
}

void atkC3_trysetfutureattack(void) {
    if (gWishFutureKnock->futureSightCounter[gBankTarget] != 0)
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);

    else
    {
        gWishFutureKnock->futureSightMove[gBankTarget] = gCurrentMove;
        gWishFutureKnock->futureSightAttacker[gBankTarget] = gBankAttacker;
        gWishFutureKnock->futureSightCounter[gBankTarget] = 3;

        if (gCurrentMove == MOVE_DOOMDESIRE)
            gBattleCommunication[MULTISTRING_CHOOSER] = 1;
        else
            gBattleCommunication[MULTISTRING_CHOOSER] = 0;

        gBattlescriptCurrInstr += 5;
    }
}

void atkC5_setsemiinvulnerablebit(void) {
    switch (gCurrentMove) {
		case MOVE_FLY:
		case MOVE_BOUNCE:
			gStatuses3[gBankAttacker] |= STATUS3_IN_AIR;
			break;
		
		case MOVE_DIG:
			gStatuses3[gBankAttacker] |= STATUS3_UNDERGROUND;
			break;
		
		case MOVE_DIVE:
			gStatuses3[gBankAttacker] |= STATUS3_UNDERWATER;
			break;
		
		case MOVE_SHADOWFORCE:
		case MOVE_PHANTOMFORCE:
			gStatuses3[gBankAttacker] |= STATUS3_DISAPPEARED;
			break;
		
		case MOVE_SKYDROP:
			gStatuses3[gBankAttacker] |= STATUS3_SKY_DROP_ATTACKER;
			gStatuses3[gBankTarget] |= STATUS3_SKY_DROP_TARGET;
    }
    gBattlescriptCurrInstr++;
}

void atkC6_clearsemiinvulnerablebit(void) {
    switch (gCurrentMove) {
		case MOVE_FLY:
		case MOVE_BOUNCE:
			gStatuses3[gBankAttacker] &= ~STATUS3_IN_AIR;
			break;
		case MOVE_DIG:
			gStatuses3[gBankAttacker] &= ~STATUS3_UNDERGROUND;
			break;
		case MOVE_DIVE:
			gStatuses3[gBankAttacker] &= ~STATUS3_UNDERWATER;
			break;
		case MOVE_SHADOWFORCE:
		case MOVE_PHANTOMFORCE:
			gStatuses3[gBankAttacker] &= ~STATUS3_DISAPPEARED;
			break;
		case MOVE_SKYDROP:
			gStatuses3[gBankAttacker] &= ~STATUS3_SKY_DROP_ATTACKER;
			gStatuses3[gBankTarget] &= ~STATUS3_SKY_DROP_TARGET;
			break;
    }
    gBattlescriptCurrInstr++;
}

void atkC8_sethail(void) {
    if (gBattleWeather & WEATHER_HAIL_ANY) {
        gMoveResultFlags |= MOVE_RESULT_MISSED;
        gBattleCommunication[MULTISTRING_CHOOSER] = 2;
    }
	
    else {
        gBattleWeather = WEATHER_HAIL_ANY;
        gBattleCommunication[MULTISTRING_CHOOSER] = 5;
		
        if (ITEM_EFFECT(gBankAttacker) == ITEM_EFFECT_ICY_ROCK)
			gWishFutureKnock->weatherDuration = 8;
		else
			gWishFutureKnock->weatherDuration = 5;
    }
    gBattlescriptCurrInstr++;
}

void atkCC_callterrainattack(void) { //nature power
    gHitMarker &= ~(HITMARKER_ATTACKSTRING_PRINTED);
	
	switch (TerrainType) {
		case ELECTRIC_TERRAIN:
			gCurrentMove = TerrainTable[0].naturePowerMove;
			break;
		
		case GRASSY_TERRAIN:
			gCurrentMove = TerrainTable[1].naturePowerMove;
			break;
	
		case MISTY_TERRAIN:
			gCurrentMove = TerrainTable[2].naturePowerMove;
			break;

		case PSYCHIC_TERRAIN:
			gCurrentMove = TerrainTable[3].naturePowerMove;
			break;
		
		default:
			gCurrentMove = TerrainTable[gBattleTerrain + 4].naturePowerMove;
	}
	
    gBankTarget = GetMoveTarget(gCurrentMove, 0);
    BattleScriptPush(gBattleScriptsForMoveEffects[gBattleMoves[gCurrentMove].effect]);
    gBattlescriptCurrInstr++;
}

void atkD2_tryswapitems(void) { //Trick
    // Wild Pokemon can't swap items with player
    if (SIDE(gBankAttacker) == B_SIDE_OPPONENT && !(gBattleTypeFlags & (BATTLE_TYPE_TRAINER)))
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
	
    else {
/* Should no longer be neccessary since Knock off now removes items
        // you can't swap items if they were knocked off in regular battles
        if (!(gBattleTypeFlags & (BATTLE_TYPE_LINK
                             | BATTLE_TYPE_EREADER_TRAINER
                             | BATTLE_TYPE_FRONTIER
                             | BATTLE_TYPE_SECRET_BASE
                             | BATTLE_TYPE_x2000000))
            && (gWishFutureKnock->knockedOffMons[sideAtk] & gBitTable[gBattlerPartyIndexes[gBankAttacker]]
                || gWishFutureKnock->knockedOffMons[sideDef] & gBitTable[gBattlerPartyIndexes[gBankTarget]]))
        {
            gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
        }
*/

        // can't swap if two pokemon don't have an item
        // or if either of them is a forbidden item
        if ((gBattleMons[gBankAttacker].item == 0 && gBattleMons[gBankTarget].item == 0)
        || !CanTransferItem(gBattleMons[gBankAttacker].species, gBattleMons[gBankAttacker].item, GetBankPartyData(gBankAttacker))
        || !CanTransferItem(gBattleMons[gBankAttacker].species, gBattleMons[gBankTarget].item, GetBankPartyData(gBankAttacker))
        || !CanTransferItem(gBattleMons[gBankTarget].species, gBattleMons[gBankTarget].item, GetBankPartyData(gBankTarget))
        || !CanTransferItem(gBattleMons[gBankTarget].species, gBattleMons[gBankAttacker].item, GetBankPartyData(gBankTarget)))
            gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);

        // check if ability prevents swapping
        else if (ABILITY(gBankTarget) == ABILITY_STICKYHOLD)  {
            gBattlescriptCurrInstr = BattleScript_StickyHoldActivates;
            gLastUsedAbility = gBattleMons[gBankTarget].ability;
            RecordAbilityBattle(gBankTarget, gLastUsedAbility);
        }
		
        // took a while, but all checks passed and items can be safely swapped
        else {
            u16 oldItemAtk, *newItemAtk;

            newItemAtk = &gBattleStruct->changedItems[gBankAttacker];
            oldItemAtk = gBattleMons[gBankAttacker].item;
            *newItemAtk = gBattleMons[gBankTarget].item;

            gBattleMons[gBankAttacker].item = 0;
            gBattleMons[gBankTarget].item = oldItemAtk;

            gActiveBattler = gBankAttacker;
            EmitSetMonData(0, REQUEST_HELDITEM_BATTLE, 0, 2, newItemAtk);
            MarkBufferBankForExecution(gBankAttacker);

            gActiveBattler = gBankTarget;
            EmitSetMonData(0, REQUEST_HELDITEM_BATTLE, 0, 2, &gBattleMons[gBankTarget].item);
            MarkBufferBankForExecution(gBankTarget);
			
			gBattleStruct->choicedMove[gBankTarget] = 0;
            gBattleStruct->choicedMove[gBankTarget] = 0;

            gBattleStruct->choicedMove[gBankAttacker] = 0;
            gBattleStruct->choicedMove[gBankAttacker] = 0;

            gBattlescriptCurrInstr += 5;

            PREPARE_ITEM_BUFFER(gBattleTextBuff1, *newItemAtk);
            PREPARE_ITEM_BUFFER(gBattleTextBuff2, oldItemAtk);

            if (oldItemAtk != 0 && *newItemAtk != 0)
                gBattleCommunication[MULTISTRING_CHOOSER] = 2; // attacker's item -> <- target's item
            else if (oldItemAtk == 0 && *newItemAtk != 0)
                gBattleCommunication[MULTISTRING_CHOOSER] = 0; // nothing -> <- target's item
            else
                gBattleCommunication[MULTISTRING_CHOOSER] = 1; // attacker's item -> <- nothing
        }
    }
}

void atkD3_trycopyability(void) { //Role Play
	u8* atkAbilityLoc;
	u8 atkAbility, defAbility;
	
	//Get correct location of ability
	if (gStatuses3[gBankAttacker] & STATUS3_ABILITY_SUPPRESS)
		atkAbilityLoc = &(SuppressedAbilities[gBankAttacker]);
	else
		atkAbilityLoc = &(gBattleMons[gBankAttacker].ability);
	
	if (gStatuses3[gBankTarget] & STATUS3_ABILITY_SUPPRESS)
		defAbility = SuppressedAbilities[gBankTarget];
	else
		defAbility = gBattleMons[gBankTarget].ability;
	
	atkAbility = *atkAbilityLoc;
	
	if (atkAbility == defAbility || defAbility == 0 || CheckTableForAbility(defAbility, RolePlayBanTable))
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
		
	else {
		*atkAbilityLoc = defAbility;
        gLastUsedAbility = defAbility;
        gBattlescriptCurrInstr += 5;
    }
}

void atkDA_tryswapabilities(void) { //Skill Swap
	u8* atkAbilityLoc, *defAbilityLoc;
	u8 atkAbility, defAbility;
	
	//Get correct location of ability
	if (gStatuses3[gBankAttacker] & STATUS3_ABILITY_SUPPRESS)
		atkAbilityLoc = &(SuppressedAbilities[gBankAttacker]);
	else
		atkAbilityLoc = &(gBattleMons[gBankAttacker].ability);
	
	if (gStatuses3[gBankTarget] & STATUS3_ABILITY_SUPPRESS)
		defAbilityLoc = &(SuppressedAbilities[gBankTarget]);
	else
		defAbilityLoc = &(gBattleMons[gBankTarget].ability);
		
	atkAbility = *atkAbilityLoc;
	defAbility = *defAbilityLoc;
	
	if (atkAbility == 0 || defAbility == 0 
	|| CheckTableForAbility(atkAbility, SkillSwapBanTable) || CheckTableForAbility(defAbility, SkillSwapBanTable)
	|| gMoveResultFlags & MOVE_RESULT_NO_EFFECT)
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
	
    else {
        *atkAbilityLoc = defAbility;
        *defAbilityLoc = atkAbility;
        gBattlescriptCurrInstr += 5;
    }
}

void atkDB_tryimprision(void) {
    if ((gStatuses3[gBankAttacker] & STATUS3_IMPRISONED))
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
		
    else {
        PressurePPLoseOnUsingImprision(gBankAttacker);
        gStatuses3[gBankAttacker] |= STATUS3_IMPRISONED;
        gBattlescriptCurrInstr += 5;
    }
}

void atkE4_getsecretpowereffect(void) {
	switch (TerrainType) {
		case ELECTRIC_TERRAIN:
			gBattleCommunication[MOVE_EFFECT_BYTE] = TerrainTable[0].secretPowerEffect;
			break;
		
		case GRASSY_TERRAIN:
			gBattleCommunication[MOVE_EFFECT_BYTE] = TerrainTable[1].secretPowerEffect;
			break;
	
		case MISTY_TERRAIN:
			gBattleCommunication[MOVE_EFFECT_BYTE] = TerrainTable[2].secretPowerEffect;
			break;

		case PSYCHIC_TERRAIN:
			gBattleCommunication[MOVE_EFFECT_BYTE] = TerrainTable[3].secretPowerEffect;
			break;
		
		default:
			gBattleCommunication[MOVE_EFFECT_BYTE] = TerrainTable[gBattleTerrain + 4].secretPowerEffect;
	}
    gBattlescriptCurrInstr++;
}

void atkE5_pickupitemcalculation(void) {
	u16 item = 0;
	u8 chance = 0; 
	
    for (int i = 0; i < 6; ++i) {
        
        if (gPlayerParty[i].species == PKMN_NONE) break;
		if (gPlayerParty[i].item) continue;
        if (GetMonData(&gPlayerParty[i], MON_DATA_IS_EGG, 0)) continue;

		u8 level = gPlayerParty[i].level;
		u8 randomness = umodsi(Random(), 100);
		
		switch (GetPartyAbility(&gPlayerParty[i])) {
			case ABILITY_PICKUP:
				chance = 10; // ~10% chance of pickup to activate
				item = ChoosePickupItem(level);
				break;
			
			case ABILITY_HONEYGATHER:
				chance = 5 + 5 * udivsi((level - 1), 10);
				item = ITEM_HONEY;
		}
		
		if (randomness < chance)
			gPlayerParty[i].item = item;
    }
    ++gBattlescriptCurrInstr;
}

item_t ChoosePickupItem(u8 level) {
    u32 row_num = (level - 1) / 10;
    item_t* common_row = pickup_common_items + row_num;
    item_t* rare_row = pickup_rare_items + row_num;
    
    u16 k = Random();
    
    for (u32 i = 0; i < PU_COMMON_PER_ROW; ++i)
        if (k < pickup_common_item_ceilings[i])
            return common_row[i];
            
    for (u32 i = 0; i < PU_RARE_PER_ROW; ++i)
        if (k < pickup_rare_item_ceilings[i])
            return rare_row[i];
            
    // will never be reached
    return ITEM_NONE;
}

void atkE7_trycastformdatachange(void) {
    u8 form = FALSE;
	u8 bank = gBattleScripting->bank;
	
    gBattlescriptCurrInstr++;
	
	switch (gBattleMons[bank].species) { //Not ability b/c you can lose ability
		case PKMN_CASTFORM:
			form = CastformDataTypeChange(bank);
			if (form) {
				BattleScriptPushCursorAndCallback(BattleScript_CastformChange);
				gBattleStruct->castformToChangeInto = form - 1;
			}
			return;
			
		case PKMN_CHERRIM:
			if (ABILITY(bank) == ABILITY_FLOWERGIFT && !IS_TRANSFORMED(bank)
			&& WEATHER_HAS_EFFECT && gBattleWeather & WEATHER_SUN_ANY) {
				DoFormChange(bank, PKMN_CHERRIMSUN, FALSE);
				form = TRUE;
			}
			break;
		
		case PKMN_CHERRIMSUN:
			if (ABILITY(bank) != ABILITY_FLOWERGIFT
			|| !WEATHER_HAS_EFFECT || !(gBattleWeather & WEATHER_SUN_ANY)) {
				DoFormChange(bank, PKMN_CHERRIM, FALSE);
				form = TRUE;
			}
	}
	
	BattleScriptPushCursorAndCallback(BattleScript_FlowerGift);
}

u8 CastformDataTypeChange(u8 bank) {
    u8 formChange = 0;
    if (gBattleMons[bank].species != PKMN_CASTFORM || gBattleMons[bank].hp == 0)
        return CASTFORM_NO_CHANGE;
		
    if ((!WEATHER_HAS_EFFECT || ABILITY(bank) != ABILITY_FORECAST) && gBattleMonForms[bank] != CASTFORM_NORMAL) {
        SET_BATTLER_TYPE(bank, TYPE_NORMAL);
        return CASTFORM_TO_NORMAL;
    }
	
	if (WEATHER_HAS_EFFECT && ABILITY(bank) == ABILITY_FORECAST) {
	
		if (!(gBattleWeather & (WEATHER_RAIN_ANY | WEATHER_SUN_ANY | WEATHER_HAIL_ANY)) && gBattleMonForms[bank] != CASTFORM_NORMAL) {
			SET_BATTLER_TYPE(bank, TYPE_NORMAL);
			formChange = CASTFORM_TO_NORMAL;
		}
		
		else if (gBattleWeather & WEATHER_SUN_ANY &&gBattleMonForms[bank] != CASTFORM_SUN) {
			SET_BATTLER_TYPE(bank, TYPE_FIRE);
			formChange = CASTFORM_TO_FIRE;
		}
		
		else if (gBattleWeather & WEATHER_RAIN_ANY && gBattleMonForms[bank] != CASTFORM_RAIN) {
			SET_BATTLER_TYPE(bank, TYPE_WATER);
			formChange = CASTFORM_TO_WATER;
		}
		
		else if (gBattleWeather & WEATHER_HAIL_ANY && gBattleMonForms[bank] != CASTFORM_HAIL) {
			SET_BATTLER_TYPE(bank, TYPE_ICE);
			formChange = CASTFORM_TO_ICE;
		}
		
		return formChange;
	}
    return CASTFORM_NO_CHANGE;
}

void atkE8_settypebasedhalvers(void) { //water/mud sport 
    bool8 worked = FALSE;
	
    if (gBattleMoves[gCurrentMove].effect == EFFECT_MUD_SPORT)
    {
        if (!MudSportTimer) {
            MudSportTimer = 5;
            gBattleCommunication[MULTISTRING_CHOOSER] = 0;
            worked = TRUE;
        }
    }
    else { //Water Sport
        if (!WaterSportTimer) {
            WaterSportTimer = 5;
            gBattleCommunication[MULTISTRING_CHOOSER] = 1;
            worked = TRUE;
        }
    }
	
    if (worked)
        gBattlescriptCurrInstr += 5;
    else
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
}

void atkEA_tryrecycleitem(void) {
    gActiveBattler = gBankAttacker;
	u16 item = SAVED_CONSUMED_ITEMS(gActiveBattler);
	
    if (item && gBattleMons[gActiveBattler].item == 0) {
        gLastUsedItem = item;
        SAVED_CONSUMED_ITEMS(gActiveBattler) = 0;
		CONSUMED_ITEMS(gActiveBattler) = 0;	//Remove the temporary item storage
        gBattleMons[gActiveBattler].item = gLastUsedItem;
        EmitSetMonData(0, REQUEST_HELDITEM_BATTLE, 0, 2, &gBattleMons[gActiveBattler].item);
        MarkBufferBankForExecution(gActiveBattler);
        gBattlescriptCurrInstr += 5;
    }
    else
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
}

void atkEB_settypetoterrain(void) {
	u8 type;

	switch (TerrainType) {
		case ELECTRIC_TERRAIN:
			type = TerrainTable[0].camouflageType;
			break;
		
		case GRASSY_TERRAIN:
			type = TerrainTable[1].camouflageType;
			break;
	
		case MISTY_TERRAIN:
			type = TerrainTable[2].camouflageType;
			break;

		case PSYCHIC_TERRAIN:
			type = TerrainTable[3].camouflageType;
			break;
		
		default:
			type = TerrainTable[gBattleTerrain + 4].camouflageType;
	}
	
    if (gBattleMons[gBankAttacker].type1 != type 
	&&  gBattleMons[gBankAttacker].type2 != type 
	&&  gBattleMons[gBankAttacker].type3 != type)
    {
        gBattleMons[gBankAttacker].type1 = type;
        gBattleMons[gBankAttacker].type2 = type;
		gBattleMons[gBankAttacker].type3 = TYPE_BLANK;
        PREPARE_TYPE_BUFFER(gBattleTextBuff1, type);
        gBattlescriptCurrInstr += 5;
    }
    else
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
}

void atkEE_removelightscreenreflect(void) { //Brick Break
    u8 side = SIDE(gBankTarget);
    if (gSideTimers[side].reflectTimer || gSideTimers[side].lightscreenTimer || AuroraVeilTimers[side]) {
        gSideAffecting[side] &= ~(SIDE_STATUS_REFLECT);
        gSideAffecting[side] &= ~(SIDE_STATUS_LIGHTSCREEN);
        gSideTimers[side].reflectTimer = 0;
        gSideTimers[side].lightscreenTimer = 0;
		AuroraVeilTimers[side] = 0;
        gBattleScripting->animTurn = 1;
        gBattleScripting->animTargetsHit = 1;
    }
    else {
        gBattleScripting->animTurn = 0;
        gBattleScripting->animTargetsHit = 0;
    }
    gBattlescriptCurrInstr++;
}
