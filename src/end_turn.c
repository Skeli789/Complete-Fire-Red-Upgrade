#include "defines.h"
#include "end_turn.h"
#include "helper_functions.h"

//Slow start

extern u8 BattleScript_RainDishActivates[];
extern u8 BattleScript_DrySkinDamage[];
extern u8 BattleScript_SolarPowerDamage[];
extern u8 BattleScript_SeaOfFireDamage[];
extern u8 BattleScript_GrassyTerrainHeal[];
extern u8 BattleScript_Healer[];
extern u8 BattleScript_AquaRing[];
extern u8 BattleScript_PoisonHeal[];
extern u8 BattleScript_MagnetRiseEnd[];
extern u8 BattleScript_TelekinesisEnd[];
extern u8 BattleScript_HealBlockEnd[];
extern u8 BattleScript_EmbargoEnd[];
extern u8 BattleScript_AuroraVeilEnd[];
extern u8 BattleScript_TailwindEnd[];
extern u8 BattleScript_LuckyChantEnd[];
extern u8 BattleScript_RainbowEnd[];
extern u8 BattleScript_SeaOfFireEnd[];
extern u8 BattleScript_SwampEnd[];
extern u8 BattleScript_TrickRoomEnd[];
extern u8 BattleScript_WaterSportEnd[];
extern u8 BattleScript_MudSportEnd[];
extern u8 BattleScript_WonderRoomEnd[];
extern u8 BattleScript_MagicRoomEnd[];
extern u8 BattleScript_GravityEnd[];
extern u8 BattleScript_TerrainEnd[];
extern u8 BattleScript_MoodySingleStat[];
extern u8 BattleScript_MoodyRegular[];
extern u8 BattleScript_BadDreams[];
extern u8 BattleScript_ToxicOrb[];
extern u8 BattleScript_FlameOrb[];
extern u8 BattleScript_Harvest[];
extern u8 BattleScript_Pickup[];
extern u8 BattleScript_ZenMode[];
extern u8 BattleScript_PowerConstruct[];
extern u8 BattleScript_StartedSchooling[];
extern u8 BattleScript_StoppedSchooling[];
extern u8 BattleScript_ShieldsDownToCore[];
extern u8 BattleScript_ShieldsDownToMeteor[];
extern u8 BattleScript_FlowerGift[];

u8 TurnBasedEffects(void);
void ClearBankStatus(bank_t);
bool8 AllStatsButOneAreMinned(bank_t);

u8 TurnBasedEffects(void) {
    u8 effect = 0;

    gHitMarker |= (HITMARKER_GRUDGE | HITMARKER_x20);
    while (!(gBattleTypeFlags & BATTLE_TYPE_SAFARI) && gBattleStruct->turnEffectsTracker <= TURNBASED_MAX_CASE)
    {
		u8 sideBank;
        gActiveBattler = gBankAttacker = gBankTarget = gBanksByTurnOrder[gBattleStruct->turnEffectsBank];
        if (!(gAbsentBattlerFlags & gBitTable[gActiveBattler])) {
            switch (gBattleStruct->turnEffectsTracker) { 
			
			case(ET_General_Counter_Decrement):
				for (u8 i = 0; i < 4; ++i) 
				{
					if(gNewBS->LaserFocusTimers[i])
						--gNewBS->LaserFocusTimers[i];
					
					if(gNewBS->ThroatChopTimers[i])
						--gNewBS->ThroatChopTimers[i];
						
					if(gNewBS->StompingTantrumTimers[i])
						--gNewBS->StompingTantrumTimers[i];
						
					if (gNewBS->StakeoutCounters[i])
						--gNewBS->StakeoutCounters[i];
						
					if (gNewBS->DestinyBondCounters[i])
						--gNewBS->DestinyBondCounters[i];
						
					if(gNewBS->ElectrifyTimers[i])
						--gNewBS->ElectrifyTimers[i];
				}

				for (u8 i = 0; i < 4; ++i)
				{ //These should be cleared earlier on, but just in case they aren't
					gNewBS->PowderByte = 0;
					gNewBS->BeakBlastByte = 0;
					gNewBS->CustapQuickClawIndicator = 0;
				}
				
				if (gNewBS->RetaliateCounters[0])
					--gNewBS->RetaliateCounters[0];
					
				if (gNewBS->RetaliateCounters[1])
					--gNewBS->RetaliateCounters[1];
					
				if (gNewBS->IonDelugeTimer)
					--gNewBS->IonDelugeTimer;
					
				if (gNewBS->FairyLockTimer)
					--gNewBS->FairyLockTimer;

				if (gNewBS->EchoedVoiceCounter)
					--gNewBS->EchoedVoiceCounter;
					
				if (gNewBS->EchoedVoiceCounter == 0)
					gNewBS->EchoedVoiceDamageScale = 0;
				
				break;
				
			case(ET_Weather_Health_Abilities):
				if (gBattleMons[gActiveBattler].hp) 
				{
					gLastUsedAbility = gBattleMons[gActiveBattler].ability;
					switch(gLastUsedAbility) {
						case ABILITY_RAINDISH:
							if (AbilityBattleEffects(ABILITYEFFECT_ENDTURN, gActiveBattler, 0, 0, 0))
								effect++;
							break;
						case ABILITY_DRYSKIN:
							if (WEATHER_HAS_EFFECT) {
								if ((gBattleWeather & WEATHER_RAIN_ANY) 
								&& gBattleMons[gActiveBattler].hp < gBattleMons[gActiveBattler].maxHP) 
								{
									gBattleMoveDamage = MathMax(1, gBattleMons[gActiveBattler].maxHP / 8);
									gBattleMoveDamage *= -1;
									BattleScriptExecute(BattleScript_RainDishActivates);
									effect++;
								}
								else if (gBattleWeather & WEATHER_SUN_ANY) 
								{
									gBattleMoveDamage = MathMax(1, gBattleMons[gActiveBattler].maxHP / 8);
									BattleScriptExecute(BattleScript_DrySkinDamage);
									effect++;			
								}
							}
							break;
						case ABILITY_ICEBODY:
							if (WEATHER_HAS_EFFECT && (gBattleWeather & WEATHER_HAIL_ANY)
							&& gBattleMons[gActiveBattler].hp < gBattleMons[gActiveBattler].maxHP) 
							{
								gBattleMoveDamage = MathMax(1, gBattleMons[gActiveBattler].maxHP / 16);
								gBattleMoveDamage *= -1;
								BattleScriptExecute(BattleScript_RainDishActivates);
								effect++;
							}
							break;
						case ABILITY_SOLARPOWER:
							if (WEATHER_HAS_EFFECT && (gBattleWeather & WEATHER_SUN_ANY) 
							&& gBattleMons[gActiveBattler].hp) 
							{
								gBattleMoveDamage = MathMax(1, gBattleMons[gActiveBattler].maxHP / 8);
								BattleScriptExecute(BattleScript_SolarPowerDamage);
								effect++;
							}
						}
				}
				break;
			
			case(ET_Future_Sight):
                if (gWishFutureKnock->futureSightCounter[gActiveBattler] 
				&& --gWishFutureKnock->futureSightCounter[gActiveBattler] == 0 
				&& gBattleMons[gActiveBattler].hp) 
				{
                    if (gWishFutureKnock->futureSightMove[gActiveBattler] == MOVE_FUTURESIGHT)
                        gBattleCommunication[MULTISTRING_CHOOSER] = 0;
                    else //Doom Desire
                        gBattleCommunication[MULTISTRING_CHOOSER] = 1;
						
					gCurrentMove = gWishFutureKnock->futureSightMove[gActiveBattler];
					
                    gBattleTextBuff1[0] = 0xFD;
                    gBattleTextBuff1[1] = 2;
                    gBattleTextBuff1[2] = gWishFutureKnock->futureSightMove[gActiveBattler];
                    gBattleTextBuff1[3] = gWishFutureKnock->futureSightMove[gActiveBattler] >> 8;
                    gBattleTextBuff1[4] = 0xFF;
                    gBankAttacker = gWishFutureKnock->futureSightAttacker[gActiveBattler];
                    gSpecialStatuses[gBankTarget].moveturnLostHP = 0xFFFF;
                    BattleScriptExecute(BattleScript_MonTookFutureAttack);
					effect++;
                }
				break;
	
			case(ET_Wish):
                if (gWishFutureKnock->wishCounter[gActiveBattler] && --gWishFutureKnock->wishCounter[gActiveBattler] == 0 && gBattleMons[gActiveBattler].hp) {
                    BattleScriptExecute(BattleScript_WishComesTrue);
                    effect++;
                }
				gNewBS->blockTracker = 0; //For later
                break;
				
			case(ET_Thrash):
                if (gBattleMons[gActiveBattler].status2 & STATUS2_LOCK_CONFUSE)
                {
                    gBattleMons[gActiveBattler].status2 -= 0x400;
                    if (!(gBattleMons[gActiveBattler].status2 & STATUS2_LOCK_CONFUSE)
                     && (gBattleMons[gActiveBattler].status2 & STATUS2_MULTIPLETURNS))
                    {
                        gBattleMons[gActiveBattler].status2 &= ~(STATUS2_MULTIPLETURNS);
                        if (!(gBattleMons[gActiveBattler].status2 & STATUS2_CONFUSION))
                        {
                            gBattleCommunication[MOVE_EFFECT_BYTE] = 0x47;
                            SetMoveEffect(1, 0);
                            if (gBattleMons[gActiveBattler].status2 & STATUS2_CONFUSION)
                                BattleScriptExecute(BattleScript_ThrashConfuses);
                            effect++;
                        }
                    }
					else if (WasUnableToUseMove(gActiveBattler))
						CancelMultiTurnMoves(gActiveBattler);
                }
				break;
				
			case(ET_Block_A):
				switch(gNewBS->blockTracker) {
					case(ET_Sea_Of_Fire):
						if (gNewBS->SeaOfFireTimers[gActiveBattler] && 
						    gBattleMons[gActiveBattler].ability != ABILITY_MAGICGUARD &&
							gBattleMons[gActiveBattler].hp &&
							gBattleMons[gActiveBattler].type1 != TYPE_FIRE &&
							gBattleMons[gActiveBattler].type2 != TYPE_FIRE &&
							gBattleMons[gActiveBattler].type3 != TYPE_FIRE) {
								BattleScriptExecute(BattleScript_SeaOfFireDamage);
								effect++;
							}
						break;
						
					case(ET_Grassy_Terrain):
						if (TerrainType == GRASSY_TERRAIN && CheckGrounding(gActiveBattler) &&
							gBattleMons[gActiveBattler].hp < gBattleMons[gActiveBattler].maxHP &&
						    gBattleMons[gActiveBattler].hp && !gNewBS->HealBlockTimers[gActiveBattler]) {
								BattleScriptExecute(BattleScript_GrassyTerrainHeal);
								effect++;
							}
						break;
						
					case(ET_Item_Effects):
						if (ItemBattleEffects(ItemEffects_EndTurn, gActiveBattler, FALSE, FALSE))
							effect++;
						break;
						
					case(ET_Hydration_ShedSkin):
						switch(gBattleMons[gActiveBattler].ability) {
						case ABILITY_SHEDSKIN:
							if (AbilityBattleEffects(ABILITYEFFECT_ENDTURN, gActiveBattler, 0, 0, 0))
								effect++;
							break;
						case ABILITY_HYDRATION:
							if (WEATHER_HAS_EFFECT && (gBattleWeather & WEATHER_RAIN_ANY) &&
								gBattleMons[gActiveBattler].status1 & STATUS_ANY) {
									ClearBankStatus(gActiveBattler);
									BattleScriptPushCursorAndCallback(BattleScript_ShedSkinActivates);
									gBattleScripting->bank = gActiveBattler;
									MarkBufferBankForExecution(gActiveBattler);
									effect++;
							}
						}
						break;
						
					case(ET_Healer):
						if (gBattleMons[gActiveBattler].ability == ABILITY_HEALER &&
							gBattleTypeFlags & BATTLE_TYPE_DOUBLE &&
							gBattleMons[PARTNER(gActiveBattler)].hp &&
							gBattleMons[PARTNER(gActiveBattler)].status1 &&
							umodsi(Random(), 100) < 30) {
								gBankTarget = PARTNER(gActiveBattler);
								ClearBankStatus(gBankTarget);
								BattleScriptExecute(BattleScript_Healer);
								effect++;
							}
				}
				
				++gNewBS->blockTracker;
				if (gNewBS->blockTracker < MAX_CASES_BLOCK_A)
					goto SkipBankIncrement;
				else {
					gNewBS->blockTracker = 0;
					break;
				}
					
			case(ET_Aqua_Ring):
				if ((gStatuses3[gActiveBattler] & STATUS3_AQUA_RING) &&
					 gBattleMons[gActiveBattler].hp < gBattleMons[gActiveBattler].maxHP &&
					 gBattleMons[gActiveBattler].hp && !gNewBS->HealBlockTimers[gActiveBattler]) {
					 
						gBattleMoveDamage = gBattleMons[gActiveBattler].maxHP / 16;
						if (GetBankItemEffect(gActiveBattler) == ITEM_EFFECT_BIGROOT)
							gBattleMoveDamage = udivsi((gBattleMoveDamage * 130), 100);
						if (gBattleMoveDamage == 0)
							gBattleMoveDamage = 1;
						gBattleMoveDamage *= -1;
						BattleScriptExecute(BattleScript_AquaRing);
						effect++;
					}
				break;
				
			case(ET_Ingrain):
                if ((gStatuses3[gActiveBattler] & STATUS3_ROOTED) &&
                     gBattleMons[gActiveBattler].hp < gBattleMons[gActiveBattler].maxHP &&
                     gBattleMons[gActiveBattler].hp && !gNewBS->HealBlockTimers[gActiveBattler]) {
					 
						gBattleMoveDamage = gBattleMons[gActiveBattler].maxHP / 16;
						if (GetBankItemEffect(gActiveBattler) == ITEM_EFFECT_BIGROOT)
							gBattleMoveDamage = udivsi((gBattleMoveDamage * 130), 100);
						if (gBattleMoveDamage == 0)
							gBattleMoveDamage = 1;
						gBattleMoveDamage *= -1;
						BattleScriptExecute(BattleScript_IngrainTurnHeal);
						effect++;
                }
				break;
				
			case(ET_Leech_Seed): //TO-DO, Leech Seed + Big Root
                if (gStatuses3[gActiveBattler] & STATUS3_LEECHSEED && 
					gBattleMons[gStatuses3[gActiveBattler] & STATUS3_LEECHSEED_BATTLER].hp &&
					gBattleMons[gStatuses3[gActiveBattler] & STATUS3_LEECHSEED_BATTLER].ability != ABILITY_MAGICGUARD &&
					gBattleMons[gActiveBattler].hp) {
					
                    gBankTarget = gStatuses3[gActiveBattler] & STATUS3_LEECHSEED_BATTLER; //funny how the 'target' is actually the bank that receives HP
                    gBattleMoveDamage = gBattleMons[gActiveBattler].maxHP / 8;
                    if (gBattleMoveDamage == 0)
                        gBattleMoveDamage = 1;
                    gBattleScripting->animArg1 = gBankTarget;
                    gBattleScripting->animArg2 = gBankAttacker;
                    BattleScriptExecute(BattleScript_LeechSeedTurnDrain);
                    effect++;
				}
				break;
				
			case(ET_Poison):
                if ((gBattleMons[gActiveBattler].status1 & STATUS_POISON) && 
					 gBattleMons[gActiveBattler].hp && 
					 gBattleMons[gActiveBattler].ability != ABILITY_MAGICGUARD) {
						gBattleMoveDamage = gBattleMons[gActiveBattler].maxHP / 8;
						if (gBattleMoveDamage == 0)
							gBattleMoveDamage = 1;
							
						if (gBattleMons[gActiveBattler].ability == ABILITY_POISONHEAL &&
						    gBattleMons[gActiveBattler].hp < gBattleMons[gActiveBattler].maxHP) {
								gBattleMoveDamage *= -1;
								BattleScriptExecute(BattleScript_PoisonHeal);
						}
						else
							BattleScriptExecute(BattleScript_PoisonTurnDmg);
						effect++;
                }
                break;
				
			case(ET_Toxic):
                if ((gBattleMons[gActiveBattler].status1 & STATUS_TOXIC_POISON) && 
					 gBattleMons[gActiveBattler].hp && 
					 gBattleMons[gActiveBattler].ability != ABILITY_MAGICGUARD) {
						gBattleMoveDamage = gBattleMons[gActiveBattler].maxHP / 16;
						if (gBattleMoveDamage == 0)
							gBattleMoveDamage = 1;
							
						if ((gBattleMons[gActiveBattler].status1 & 0xF00) != 0xF00) //not 16 turns
							gBattleMons[gActiveBattler].status1 += 0x100;
							
						gBattleMoveDamage *= (gBattleMons[gActiveBattler].status1 & 0xF00) >> 8;
						
						if (gBattleMons[gActiveBattler].ability == ABILITY_POISONHEAL) {
							gBattleMoveDamage *= -1;
							BattleScriptExecute(BattleScript_PoisonHeal);
						}
						else
							BattleScriptExecute(BattleScript_PoisonTurnDmg);
						effect++;
                }
				break;
				
			case(ET_Burn):
                if ((gBattleMons[gActiveBattler].status1 & STATUS_BURN) &&				 
					 gBattleMons[gActiveBattler].hp && 
					 gBattleMons[gActiveBattler].ability != ABILITY_MAGICGUARD) {
						 if (ABILITY(gActiveBattler) == ABILITY_HEATPROOF) {
							#ifdef OLD_BURN_DAMAGE
								gBattleMoveDamage = gBattleMons[gActiveBattler].maxHP / 16;
							#else
								gBattleMoveDamage = gBattleMons[gActiveBattler].maxHP / 32;
							#endif
						 }
						 else {
							#ifdef OLD_BURN_DAMAGE
								gBattleMoveDamage = gBattleMons[gActiveBattler].maxHP / 8;
							#else
								gBattleMoveDamage = gBattleMons[gActiveBattler].maxHP / 16;
							#endif
						}
						if (gBattleMoveDamage == 0)
							gBattleMoveDamage = 1;
						BattleScriptExecute(BattleScript_BurnTurnDmg);
						effect++;
                }
                break;
				
			case(ET_Nightmare):
                if (gBattleMons[gActiveBattler].status2 & STATUS2_NIGHTMARE) {
					if (gBattleMons[gActiveBattler].status2 & STATUS_SLEEP) {
						if (gBattleMons[gActiveBattler].hp && 
							gBattleMons[gActiveBattler].ability != ABILITY_MAGICGUARD) {
								gBattleMoveDamage = gBattleMons[gActiveBattler].maxHP / 4;
								if (gBattleMoveDamage == 0)
									gBattleMoveDamage = 1;
								BattleScriptExecute(BattleScript_NightmareTurnDmg);
								effect++;
						}
					}
					else
						gBattleMons[gActiveBattler].status2 &= ~STATUS2_NIGHTMARE;
                }
                break;
				
			case(ET_Curse):
                if ((gBattleMons[gActiveBattler].status2 & STATUS2_CURSED) && 
					 gBattleMons[gActiveBattler].hp && 
					 gBattleMons[gActiveBattler].ability != ABILITY_MAGICGUARD) {
						gBattleMoveDamage = gBattleMons[gActiveBattler].maxHP / 4;
						if (gBattleMoveDamage == 0)
							gBattleMoveDamage = 1;
						BattleScriptExecute(BattleScript_CurseTurnDmg);
						effect++;
                }
                break;
				
			case(ET_Trap_Damage):
                if ((gBattleMons[gActiveBattler].status2 & STATUS2_WRAPPED) && 
					 gBattleMons[gActiveBattler].hp && 
					 gBattleMons[gActiveBattler].ability != ABILITY_MAGICGUARD) {
						gBattleMons[gActiveBattler].status2 -= 0x2000;
						if (gBattleMons[gActiveBattler].status2 & STATUS2_WRAPPED) {  // damaged by wrap
							gBattleScripting->animArg1 = gBattleStruct->wrappedMove[gActiveBattler * 2];
							gBattleScripting->animArg2 = gBattleStruct->wrappedMove[1 + gActiveBattler * 2];
							gBattleTextBuff1[0] = 0xFD;
							gBattleTextBuff1[1] = 2;
							gBattleTextBuff1[2] = gBattleStruct->wrappedMove[gActiveBattler * 2];
							gBattleTextBuff1[3] = gBattleStruct->wrappedMove[gActiveBattler * 2 + 1];
							gBattleTextBuff1[4] = EOS;
							gBattlescriptCurrInstr = BattleScript_WrapTurnDmg;
							
							if (ITEM_EFFECT(gActiveBattler) == ITEM_EFFECT_BINDING_BAND)
								gBattleMoveDamage = udivsi(gBattleMons[gActiveBattler].maxHP, 6);
							else
								gBattleMoveDamage = gBattleMons[gActiveBattler].maxHP / 8;
							
							if (gBattleMoveDamage == 0)
								gBattleMoveDamage = 1;
						}
						else { // broke free
							gBattleTextBuff1[0] = 0xFD;
							gBattleTextBuff1[1] = 2;
							gBattleTextBuff1[2] = gBattleStruct->wrappedMove[gActiveBattler * 2];
							gBattleTextBuff1[3] = gBattleStruct->wrappedMove[gActiveBattler * 2 + 1];
							gBattleTextBuff1[4] = EOS;
							gBattlescriptCurrInstr = BattleScript_WrapEnds;
						}
						BattleScriptExecute(gBattlescriptCurrInstr);
						effect++;
					}
                break;
				
			case(ET_Taunt_Timer):
                if (gDisableStructs[gActiveBattler].tauntTimer)
                    gDisableStructs[gActiveBattler].tauntTimer--;
					//Display message?
                break;
				
			case(ET_Encore_Timer):
                if (gDisableStructs[gActiveBattler].encoreTimer) {
                    if (gBattleMons[gActiveBattler].moves[gDisableStructs[gActiveBattler].encoredMovePos] != gDisableStructs[gActiveBattler].encoredMove) {  // pokemon does not have the encored move anymore
                        gDisableStructs[gActiveBattler].encoredMove = 0;
                        gDisableStructs[gActiveBattler].encoreTimer = 0;
                    }
                    else if (--gDisableStructs[gActiveBattler].encoreTimer == 0 ||
							   gBattleMons[gActiveBattler].pp[gDisableStructs[gActiveBattler].encoredMovePos] == 0) {
									gDisableStructs[gActiveBattler].encoredMove = 0;
									gDisableStructs[gActiveBattler].encoreTimer = 0;
									BattleScriptExecute(BattleScript_EncoredNoMore);
									effect++;
                    }
                }
                break;
				
			case(ET_Disable_Timer):
                if (gDisableStructs[gActiveBattler].disableTimer1) {
                    int movenum;
					
                    for (movenum = 0; movenum < 4; movenum++) {
                        if (gDisableStructs[gActiveBattler].disabledMove == gBattleMons[gActiveBattler].moves[movenum])
                            break;
                    }
					
                    if (movenum == 4) { // pokemon does not have the disabled move anymore
                        gDisableStructs[gActiveBattler].disabledMove = 0;
                        gDisableStructs[gActiveBattler].disableTimer1 = 0;
                    }
                    else if (--gDisableStructs[gActiveBattler].disableTimer1 == 0) {  // disable ends
                        gDisableStructs[gActiveBattler].disabledMove = 0;
                        BattleScriptExecute(BattleScript_DisabledNoMore);
                        effect++;
                    }
                }
                break;
			
			case(ET_Lock_On_Timer):
                if (gStatuses3[gActiveBattler] & STATUS3_LOCKON)
                    gStatuses3[gActiveBattler] -= 0x8;
                break;
				
			case(ET_Charge_Timer):
                if (gDisableStructs[gActiveBattler].chargeTimer && --gDisableStructs[gActiveBattler].chargeTimer == 0)
                    gStatuses3[gActiveBattler] &= ~STATUS3_CHARGED_UP;
                break;
				
			case(ET_Magnet_Rise_Timer):
				if (gNewBS->MagnetRiseTimers[gActiveBattler] && --gNewBS->MagnetRiseTimers[gActiveBattler] == 0) {
					BattleScriptExecute(BattleScript_MagnetRiseEnd);
					effect++;
				}
				break;
					
			case(ET_Telekinesis_Timer):
				if (gNewBS->TelekinesisTimers[gActiveBattler] && --gNewBS->TelekinesisTimers[gActiveBattler] == 0) {
					BattleScriptExecute(BattleScript_TelekinesisEnd);
					effect++;
				}
				break;
					
			case(ET_Heal_Block_Timer):
				if (gNewBS->HealBlockTimers[gActiveBattler] && --gNewBS->HealBlockTimers[gActiveBattler] == 0) {
					BattleScriptExecute(BattleScript_HealBlockEnd);
					effect++;
				}
				break;
					
			case(ET_Embargo_Timer):
				if (gNewBS->EmbargoTimers[gActiveBattler] && --gNewBS->EmbargoTimers[gActiveBattler] == 0) {
					BattleScriptExecute(BattleScript_EmbargoEnd);
					effect++;
				}
				break;
					
			case(ET_Yawn):
                if (gStatuses3[gActiveBattler] & STATUS3_YAWN) {
                    gStatuses3[gActiveBattler] -= 0x800;
					
                    if (!(gStatuses3[gActiveBattler] & STATUS3_YAWN) && 
						!(gBattleMons[gActiveBattler].status1 & STATUS_ANY) &&
						  gBattleMons[gActiveBattler].species != PKMN_MINIORSHIELD &&
                          gBattleMons[gActiveBattler].ability != ABILITY_VITALSPIRIT &&
                          gBattleMons[gActiveBattler].ability != ABILITY_INSOMNIA && 
						  gBattleMons[gActiveBattler].ability != ABILITY_COMATOSE &&
						  !(gBattleMons[gActiveBattler].ability == ABILITY_LEAFGUARD && WEATHER_HAS_EFFECT && (gBattleWeather & WEATHER_SUN_ANY)) &&
						  !(CheckAbilityTargetField(ABILITY_SWEETVEIL)) &&
						  !((TerrainType == ELECTRIC_TERRAIN || TerrainType == MISTY_TERRAIN) && CheckGrounding(gActiveBattler)) &&
						  !(UproarWakeUpCheck(gActiveBattler)))
						  
                    {
                        CancelMultiTurnMoves(gActiveBattler);
                        gBattleMons[gActiveBattler].status1 |= (Random() & 3) + 2;
                        EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 4, &gBattleMons[gActiveBattler].status1);
                        MarkBufferBankForExecution(gActiveBattler);
                        gEffectBank = gActiveBattler;
                        BattleScriptExecute(BattleScript_YawnMakesAsleep);
                        effect++;
                    }
                }
                break;
			
			case(ET_Perish_Song):
                if (gStatuses3[gActiveBattler] & STATUS3_PERISH_SONG) {
                    gBattleTextBuff1[0] = 0xFD;
                    gBattleTextBuff1[1] = 1;
                    gBattleTextBuff1[2] = 1;
                    gBattleTextBuff1[3] = 1;
                    gBattleTextBuff1[4] = gDisableStructs[gActiveBattler].perishSongTimer;
                    gBattleTextBuff1[5] = 0xFF;
					
                    if (gDisableStructs[gActiveBattler].perishSongTimer == 0) {
                        gStatuses3[gActiveBattler] &= ~(STATUS3_PERISH_SONG);
                        gBattleMoveDamage = gBattleMons[gActiveBattler].hp;
                        gBattlescriptCurrInstr = BattleScript_PerishSongHits;
                    }
                    else {
                        gDisableStructs[gActiveBattler].perishSongTimer--;
                        gBattlescriptCurrInstr = BattleScript_PerishSongTimerGoesDown;
                    }
					
                    BattleScriptExecute(gBattlescriptCurrInstr);
                    effect++;
                }
				break;
			
			case(ET_Roost):
				if (gNewBS->RoostCounter & (1 << gActiveBattler)) {
					gNewBS->RoostCounter &= ~(1 << gActiveBattler);
					if (gBattleMons[gActiveBattler].type1 == TYPE_NORMAL)
						gBattleMons[gActiveBattler].type1 = TYPE_FLYING;
					if (gBattleMons[gActiveBattler].type2 == TYPE_NORMAL)
						gBattleMons[gActiveBattler].type2 = TYPE_FLYING;
					if (gBattleMons[gActiveBattler].type3 == TYPE_ROOSTLESS)
						gBattleMons[gActiveBattler].type3 = TYPE_FLYING;
				}
				else {
					if (gBattleMons[gActiveBattler].type1 == TYPE_ROOSTLESS)
						gBattleMons[gActiveBattler].type1 = TYPE_FLYING;
					if (gBattleMons[gActiveBattler].type2 == TYPE_ROOSTLESS)
						gBattleMons[gActiveBattler].type2 = TYPE_FLYING;
					if (gBattleMons[gActiveBattler].type3 == TYPE_ROOSTLESS)
						gBattleMons[gActiveBattler].type3 = TYPE_FLYING;
				}
				break;

//These Timers all fall through to eachother for quicker game speed
			case(ET_Reflect): 
				if (gBattleStruct->turnEffectsBank < 2) {
					sideBank = gBattleStruct->turnEffectsBank;
					if ((gSideAffecting[sideBank] & SIDE_STATUS_REFLECT) && --gSideTimers[sideBank].reflectTimer == 0) {
						gBankAttacker = gBankTarget = gActiveBattler = sideBank;
						gSideAffecting[sideBank] &= ~SIDE_STATUS_REFLECT;
						BattleScriptExecute(BattleScript_SideStatusWoreOff);
                        gBattleTextBuff1[0] = 0xFD;
                        gBattleTextBuff1[1] = 2;
                        gBattleTextBuff1[2] = MOVE_REFLECT;
                        gBattleTextBuff1[3] = MOVE_REFLECT >> 8;
                        gBattleTextBuff1[4] = EOS;
						effect++;
					}
					break;
				}
				else {
					++gBattleStruct->turnEffectsTracker;
					gBattleStruct->turnEffectsBank = 0;
				}
				__attribute__ ((fallthrough));
			case(ET_Light_Screen):
				if (gBattleStruct->turnEffectsBank < 2) {
					sideBank = gBattleStruct->turnEffectsBank;
					if ((gSideAffecting[sideBank] & SIDE_STATUS_LIGHTSCREEN) && --gSideTimers[sideBank].lightscreenTimer == 0) {
						gBankAttacker = gBankTarget = gActiveBattler = sideBank;
						gSideAffecting[sideBank] &= ~SIDE_STATUS_LIGHTSCREEN;
						BattleScriptExecute(BattleScript_SideStatusWoreOff);
                        gBattleTextBuff1[0] = 0xFD;
                        gBattleTextBuff1[1] = 2;
                        gBattleTextBuff1[2] = MOVE_LIGHTSCREEN;
                        gBattleTextBuff1[3] = MOVE_LIGHTSCREEN >> 8;
                        gBattleTextBuff1[4] = EOS;
						effect++;
					}
					break;
				}
				else {
					++gBattleStruct->turnEffectsTracker;
					gBattleStruct->turnEffectsBank = 0;
				}
				__attribute__ ((fallthrough));
			case(ET_Aurora_Veil):
				if (gBattleStruct->turnEffectsBank < 2) {
					if (gNewBS->AuroraVeilTimers[gBattleStruct->turnEffectsBank] && --gNewBS->AuroraVeilTimers[gBattleStruct->turnEffectsBank] == 0) {
						gBankAttacker = gBankTarget = gActiveBattler = gBattleStruct->turnEffectsBank;
						BattleScriptExecute(BattleScript_AuroraVeilEnd);
						effect++;
					}
					break;
				}
				else {
					++gBattleStruct->turnEffectsTracker;
					gBattleStruct->turnEffectsBank = 0;
				}
				__attribute__ ((fallthrough));
			case(ET_Safeguard):
				if (gBattleStruct->turnEffectsBank < 2) {
					sideBank = gBattleStruct->turnEffectsBank;
					if ((gSideAffecting[sideBank] & SIDE_STATUS_SAFEGUARD) && --gSideTimers[sideBank].safeguardTimer == 0) {
						gBankAttacker = gBankTarget = gActiveBattler = sideBank;
						gSideAffecting[sideBank] &= ~SIDE_STATUS_SAFEGUARD;
						BattleScriptExecute(BattleScript_SafeguardEnds);
						effect++;
					}
					break;
				}
				else {
					++gBattleStruct->turnEffectsTracker;
					gBattleStruct->turnEffectsBank = 0;
				}
				__attribute__ ((fallthrough));
			case(ET_Mist):
				if (gBattleStruct->turnEffectsBank < 2) {
					sideBank = gBattleStruct->turnEffectsBank;
					if ((gSideAffecting[sideBank] & SIDE_STATUS_MIST) && --gSideTimers[sideBank].mistTimer == 0) {
						gBankAttacker = gBankTarget = gActiveBattler = sideBank;
						gSideAffecting[sideBank] &= ~SIDE_STATUS_MIST;
						BattleScriptExecute(BattleScript_SideStatusWoreOff);
                        gBattleTextBuff1[0] = 0xFD;
                        gBattleTextBuff1[1] = 2;
                        gBattleTextBuff1[2] = MOVE_MIST;
                        gBattleTextBuff1[3] = MOVE_MIST >> 8;
                        gBattleTextBuff1[4] = EOS;
						effect++;
					}
					break;
				}
				else {
					++gBattleStruct->turnEffectsTracker;
					gBattleStruct->turnEffectsBank = 0;
				}
				__attribute__ ((fallthrough));
			case(ET_Tailwind_Timer):
				if (gBattleStruct->turnEffectsBank < 2) {
					if (gNewBS->TailwindTimers[gBattleStruct->turnEffectsBank] && --gNewBS->TailwindTimers[gBattleStruct->turnEffectsBank] == 0) {
						gBankAttacker = gBankTarget = gActiveBattler = gBattleStruct->turnEffectsBank;
						BattleScriptExecute(BattleScript_TailwindEnd);
						effect++;
					}
					break;
				}
				else {
					++gBattleStruct->turnEffectsTracker;
					gBattleStruct->turnEffectsBank = 0;
				}
				__attribute__ ((fallthrough));
			case(ET_Lucky_Chant_Timer):
				if (gBattleStruct->turnEffectsBank < 2) {
					if (gNewBS->LuckyChantTimers[gBattleStruct->turnEffectsBank] && --gNewBS->LuckyChantTimers[gBattleStruct->turnEffectsBank] == 0) {
						gBankAttacker = gBankTarget = gActiveBattler = gBattleStruct->turnEffectsBank;
						BattleScriptExecute(BattleScript_LuckyChantEnd);
						effect++;
					}
					break;
				}
				else {
					++gBattleStruct->turnEffectsTracker;
					gBattleStruct->turnEffectsBank = 0;
				}
				__attribute__ ((fallthrough));
			case(ET_Rainbow_Timer):
				if (gBattleStruct->turnEffectsBank < 2) {
					if (gNewBS->RainbowTimers[gBattleStruct->turnEffectsBank] && --gNewBS->RainbowTimers[gBattleStruct->turnEffectsBank] == 0) {
						gBankAttacker = gBankTarget = gActiveBattler = gBattleStruct->turnEffectsBank;
						BattleScriptExecute(BattleScript_RainbowEnd);
						effect++;
					}
					break;
				}
				else {
					++gBattleStruct->turnEffectsTracker;
					gBattleStruct->turnEffectsBank = 0;
				}
				__attribute__ ((fallthrough));
			case(ET_Sea_Of_Fire_Timer):
				if (gBattleStruct->turnEffectsBank < 2) {
					if (gNewBS->SeaOfFireTimers[gBattleStruct->turnEffectsBank] && --gNewBS->SeaOfFireTimers[gBattleStruct->turnEffectsBank] == 0) {
						gBankAttacker = gBankTarget = gActiveBattler = gBattleStruct->turnEffectsBank;
						BattleScriptExecute(BattleScript_SeaOfFireEnd);
						effect++;
					}
					break;
				}
				else {
					++gBattleStruct->turnEffectsTracker;
					gBattleStruct->turnEffectsBank = 0;
				}
				__attribute__ ((fallthrough));
			case(ET_Swamp_Timer):
				if (gBattleStruct->turnEffectsBank < 2) {
					if (gNewBS->SwampTimers[gBattleStruct->turnEffectsBank] && --gNewBS->SwampTimers[gBattleStruct->turnEffectsBank] == 0) {
						gBankAttacker = gBankTarget = gActiveBattler = gBattleStruct->turnEffectsBank;
						BattleScriptExecute(BattleScript_SwampEnd);
						effect++;
					}
					break;
				}
				else {
					++gBattleStruct->turnEffectsTracker;
					gBattleStruct->turnEffectsBank = 0;
				}
				__attribute__ ((fallthrough));
			case(ET_Trick_Room_Timer):
				++gBattleStruct->turnEffectsTracker;
				gBattleStruct->turnEffectsBank = 0;
				if (gNewBS->TrickRoomTimer && --gNewBS->TrickRoomTimer == 0) {
					BattleScriptExecute(BattleScript_TrickRoomEnd);
					return ++effect;
				}
				__attribute__ ((fallthrough));
			case(ET_Water_Sport_Timer):
				++gBattleStruct->turnEffectsTracker;
				gBattleStruct->turnEffectsBank = 0;
				if (gNewBS->WaterSportTimer && --gNewBS->WaterSportTimer == 0) {
					BattleScriptExecute(BattleScript_WaterSportEnd);
					return ++effect;
				}
				__attribute__ ((fallthrough));
			case(ET_Mud_Sport_Timer):
				++gBattleStruct->turnEffectsTracker;
				gBattleStruct->turnEffectsBank = 0;
				if (gNewBS->MudSportTimer && --gNewBS->MudSportTimer == 0) {
					BattleScriptExecute(BattleScript_MudSportEnd);
					return ++effect;
				}
				__attribute__ ((fallthrough));
			case(ET_Wonder_Room_Timer):
				++gBattleStruct->turnEffectsTracker;
				gBattleStruct->turnEffectsBank = 0;
				if (gNewBS->WonderRoomTimer && --gNewBS->WonderRoomTimer == 0) {
					BattleScriptExecute(BattleScript_WonderRoomEnd);
					return ++effect;
				}
				__attribute__ ((fallthrough));
			case(ET_Magic_Room_Timer):
				++gBattleStruct->turnEffectsTracker;
				gBattleStruct->turnEffectsBank = 0;
				if (gNewBS->MagicRoomTimer && --gNewBS->MagicRoomTimer == 0) {
					BattleScriptExecute(BattleScript_MagicRoomEnd);
					return ++effect;
				}
				__attribute__ ((fallthrough));
			case(ET_Gravity_Timer):
				++gBattleStruct->turnEffectsTracker;
				gBattleStruct->turnEffectsBank = 0;
				if (gNewBS->GravityTimer && --gNewBS->GravityTimer == 0) {
					BattleScriptExecute(BattleScript_GravityEnd);
					return ++effect;
				}
				__attribute__ ((fallthrough));
			case(ET_Terrain_Timer):
				gNewBS->blockTracker = 0; //For later
				++gBattleStruct->turnEffectsTracker;
				gBattleStruct->turnEffectsBank = 0;
				if (gNewBS->TerrainTimer && --gNewBS->TerrainTimer == 0) {
					BattleScriptExecute(BattleScript_TerrainEnd);
					return ++effect;
				}
				__attribute__ ((fallthrough));
			case(ET_Block_B):
				switch(gNewBS->blockTracker) {
				
					case(ET_Uproar):
						if (gBattleMons[gActiveBattler].status2 & STATUS2_UPROAR) {
							gBattleMons[gActiveBattler].status2 -= 0x10;  // uproar timer goes down
							if (WasUnableToUseMove(gActiveBattler)) {
								CancelMultiTurnMoves(gActiveBattler);
								gBattleCommunication[MULTISTRING_CHOOSER] = 1;
							}
							else if (gBattleMons[gActiveBattler].status2 & STATUS2_UPROAR) {
								gBattleCommunication[MULTISTRING_CHOOSER] = 0;
								gBattleMons[gActiveBattler].status2 |= STATUS2_MULTIPLETURNS;
							}
							else {
								gBattleCommunication[MULTISTRING_CHOOSER] = 1;
								CancelMultiTurnMoves(gActiveBattler);
							}
							BattleScriptExecute(BattleScript_PrintUproarOverTurns);
							++effect;
						}
						break;
						
					case(ET_SpeedBoost_Moody_BadDreams):
						if (gBattleMons[gActiveBattler].hp) {
							gLastUsedAbility = gBattleMons[gActiveBattler].ability;
							switch(gLastUsedAbility) {
							case ABILITY_SPEEDBOOST:
								if (AbilityBattleEffects(ABILITYEFFECT_ENDTURN, gActiveBattler, 0, 0, 0))
									effect++;
								break;
							case (ABILITY_MOODY): ;
								u8 stat_to_raise = 0;
								u8 stat_to_lower = 0;
								u8* scriptPtr;
								if (StatsMinned(gActiveBattler)) {
								//Raise One Stat
									stat_to_raise = umodsi(Random(), BATTLE_STATS_NO-1) + 1;
									gStatChangeByte = stat_to_raise | INCREASE_2;
									scriptPtr = BattleScript_MoodySingleStat;
								}
								else if (StatsMaxed(gActiveBattler)) {
								//Lower One Stat
									stat_to_lower = umodsi(Random(), BATTLE_STATS_NO-1) + 1;
									gStatChangeByte = stat_to_lower | DECREASE_1;
									scriptPtr = BattleScript_MoodySingleStat;
								}
								else {
								//Raise One Stat and Lower Another
								
									if (!(AllStatsButOneAreMinned(gActiveBattler))) { //At least two non min stats
										do {
											stat_to_raise = umodsi(Random(), BATTLE_STATS_NO-1) + 1;			
										} while (gBattleMons[gActiveBattler].statStages[stat_to_raise - 1] == 12);
										
									}
									else { //If all stats but one are at min, then raise one of the min ones so the
										   //non min one canned be lowered.
										do {
											stat_to_raise = umodsi(Random(), BATTLE_STATS_NO-1) + 1;			
										} while (gBattleMons[gActiveBattler].statStages[stat_to_raise - 1] != 0);
									}
									
									do {
										stat_to_lower = umodsi(Random(), BATTLE_STATS_NO-1) + 1;			
									} while (stat_to_lower == stat_to_raise ||
											 gBattleMons[gActiveBattler].statStages[stat_to_lower - 1] == 0);
									
									gStatChangeByte = stat_to_raise | INCREASE_2;	
									gNewBS->MoodyLowerStat = stat_to_lower;
									scriptPtr = BattleScript_MoodyRegular;
								}
								BattleScriptExecute(scriptPtr);
								effect++;
								break;
							
							case (ABILITY_BADDREAMS):
								if (gBattleMons[GetFoeBank(gActiveBattler)].status1 & STATUS_SLEEP ||
									gBattleMons[GetFoeBank(gActiveBattler)].ability == ABILITY_COMATOSE ||
									gBattleMons[PARTNER(GetFoeBank(gActiveBattler))].status1 & STATUS_SLEEP ||
									gBattleMons[PARTNER(GetFoeBank(gActiveBattler))].ability == ABILITY_COMATOSE) {
										BattleScriptExecute(BattleScript_BadDreams);
										++effect;
									}
							}
						}
						break;
						
					case(ET_Orbz):
						if (gBattleMons[gActiveBattler].hp && !(gBattleMons[gActiveBattler].status1)) {
							u8 item_effect = ITEM_EFFECT(gActiveBattler);
							if (item_effect == ITEM_EFFECT_TOXIC_ORB
							&& CanBePoisoned(gActiveBattler, gActiveBattler)) {
								gLastUsedItem = ITEM(gActiveBattler);
								RecordItemBattle(gActiveBattler, item_effect);
								BattleScriptExecute(BattleScript_ToxicOrb);
								++effect;
							}
							else if (item_effect == ITEM_EFFECT_FLAME_ORB
							&& CanBeBurned(gActiveBattler)) {
								gLastUsedItem = ITEM(gActiveBattler);
								RecordItemBattle(gActiveBattler, item_effect);
								BattleScriptExecute(BattleScript_FlameOrb);
								++effect;
							}
						}
						break;
						
					case(ET_Harvest_Pickup):
						if (gBattleMons[gActiveBattler].hp && !(gBattleMons[gActiveBattler].item)) {
							switch(gBattleMons[gActiveBattler].ability) {
							case ABILITY_HARVEST:
								if (GetPocketByItemId(gNewBS->SavedConsumedItems[gBattlerPartyIndexes[gActiveBattler]]) == POCKET_BERRY_POUCH) {
									if (!(WEATHER_HAS_EFFECT && (gBattleWeather & WEATHER_SUN_ANY))) { //No Sun
										if (umodsi(Random(), 100) < 50)
											break;
									}
									BattleScriptExecute(BattleScript_Harvest);
									++effect;
								}
								break;
							case ABILITY_PICKUP:
								for (u8 bank = 0; bank < 4; ++bank) {
									if (bank != gActiveBattler && !(gNewBS->IncinerateCounters[bank]) && CONSUMED_ITEMS(bank)) {
										gBattleMons[gActiveBattler].item = CONSUMED_ITEMS(bank);
										(GetBankPartyData(gActiveBattler))->item = CONSUMED_ITEMS(bank);
										gLastUsedItem = CONSUMED_ITEMS(bank);
										CONSUMED_ITEMS(bank) = 0;
										BattleScriptExecute(BattleScript_Pickup);
										++effect;
										break;
									}
								}
							}
						}
						break;
					
				}
				
				++gNewBS->blockTracker;
				if (gNewBS->blockTracker < MAX_CASES_BLOCK_B)
					goto SkipBankIncrement;
				else {
					gNewBS->blockTracker = 0;
					break;
				}
					
			case(ET_Form_Change):
				if (gBattleMons[gActiveBattler].hp && !(gBattleMons[gActiveBattler].status2 & STATUS2_TRANSFORMED)) {
					pokemon_t* partydata = GetBankPartyData(gActiveBattler);
					bool8 changedform = FALSE;
					u16 species = partydata->species;
					u8 ability = gBattleMons[gActiveBattler].ability;
					u16 newspecies = 0;
					u8* battle_script;
					switch(ability) {
						
						case ABILITY_ZENMODE:
							if (species == PKMN_DARMANITAN) {
								newspecies = PKMN_DARMANITANZEN;
								gBattleMons[gActiveBattler].type1 = TYPE_FIRE;
								gBattleMons[gActiveBattler].type2 = TYPE_PSYCHIC;
								gBattleMons[gActiveBattler].type3 = TYPE_BLANK;
								changedform = TRUE;
								battle_script = BattleScript_ZenMode;
							}
							break;
						
						case ABILITY_POWERCONSTRUCT:
							if (species == PKMN_ZYGARDE || species == PKMN_ZYGARDE_10) {
								newspecies = PKMN_ZYGARDE_COMPLETE;
								gBattleMons[gActiveBattler].type1 = TYPE_DRAGON;
								gBattleMons[gActiveBattler].type2 = TYPE_GROUND;
								gBattleMons[gActiveBattler].type3 = TYPE_BLANK;
								changedform = TRUE;
								battle_script = BattleScript_PowerConstruct;
							}
							break;
						
						case ABILITY_SCHOOLING:
							if (species == PKMN_WISHIWASHI && gBattleMons[gActiveBattler].level >= 20 &&
							    gBattleMons[gActiveBattler].hp > (gBattleMons[gActiveBattler].maxHP / 4)) {
									newspecies = PKMN_WISHIWASHI_S;
									changedform = TRUE;
									battle_script = BattleScript_StartedSchooling;
							}
							else if (species == PKMN_WISHIWASHI_S && 
									(gBattleMons[gActiveBattler].level < 20 || 
									 gBattleMons[gActiveBattler].hp <= (gBattleMons[gActiveBattler].maxHP / 4))) {
										newspecies = PKMN_WISHIWASHI;
										changedform = TRUE;
										battle_script = BattleScript_StoppedSchooling;
							}
							break;
							
						case ABILITY_SHIELDSDOWN:
							if (species == PKMN_MINIORSHIELD && gBattleMons[gActiveBattler].hp <= (gBattleMons[gActiveBattler].maxHP / 2)) {
									newspecies = umodsi(partydata->personality, 7); //Get Minior Colour
									changedform = TRUE;
									battle_script = BattleScript_ShieldsDownToCore;
								}
							else if ((species == PKMN_MINIOR_RED ||
									 species == PKMN_MINIOR_BLUE ||
									 species == PKMN_MINIOR_ORANGE ||
									 species == PKMN_MINIOR_YELLOW ||
									 species == PKMN_MINIOR_INDIGO ||
									 species == PKMN_MINIOR_GREEN ||
									 species == PKMN_MINIOR_VIOLET) && gBattleMons[gActiveBattler].hp > (gBattleMons[gActiveBattler].maxHP / 2)) {
											newspecies = PKMN_MINIORSHIELD;
											changedform = TRUE;
											battle_script = BattleScript_ShieldsDownToMeteor;
							}
							break;
							
						case ABILITY_FLOWERGIFT:
							if (species == PKMN_CHERRIMSUN && (!WEATHER_HAS_EFFECT || !(gBattleWeather & WEATHER_SUN_ANY) || ability != ABILITY_FLOWERGIFT)) {
								newspecies = PKMN_CHERRIM;
								battle_script = BattleScript_FlowerGift;
							}
			
						if (changedform) {
							partydata->species = newspecies;
							gBattleMons[gActiveBattler].species = newspecies;
							partydata->backupSpecies = species;
							CalculateMonStats(partydata);
							gBattleMons[gActiveBattler].attack = partydata->attack;
							gBattleMons[gActiveBattler].defense = partydata->defense;
							gBattleMons[gActiveBattler].speed = partydata->speed;
							gBattleMons[gActiveBattler].spAttack = partydata->spAttack;
							gBattleMons[gActiveBattler].spDefense = partydata->spDefense;
							gBattleMons[gActiveBattler].hp = partydata->hp;
							gBattleMons[gActiveBattler].maxHP = partydata->maxHP;
							BattleScriptExecute(battle_script);
							++effect;
						}
					}
				}
				break;
						
			case(ET_End):
			    gBattleStruct->turnEffectsBank = gBattlersCount;
				gNewBS->EndTurnDone = 1;
				gAbsentBattlerFlags &= ~(gNewBS->AbsentBattlerHelper);
				gNewBS->MegaData->state = 0;
			}
		}
		gBattleStruct->turnEffectsBank++;
		
		SkipBankIncrement:
			if (gBattleStruct->turnEffectsBank >= gBattlersCount) {
				gBattleStruct->turnEffectsTracker++;
				gBattleStruct->turnEffectsBank = 0;
			}
			if (effect)
				return effect;
	}
	
    gHitMarker &= ~(HITMARKER_GRUDGE | HITMARKER_x20);
    return 0;
}

void ClearBankStatus(bank_t bank) {
	if (gBattleMons[bank].status1 & (STATUS_POISON | STATUS_TOXIC_POISON))
		StringCopy(gBattleTextBuff1, gStatusConditionString_Poison);
	else if (gBattleMons[bank].status1 & STATUS_SLEEP)
		StringCopy(gBattleTextBuff1, gStatusConditionString_Sleep);
	else if (gBattleMons[bank].status1 & STATUS_PARALYSIS)
		StringCopy(gBattleTextBuff1, gStatusConditionString_Paralysis);
	else if (gBattleMons[bank].status1 & STATUS_BURN)
		StringCopy(gBattleTextBuff1, gStatusConditionString_Burn);
	else if (gBattleMons[bank].status1 & STATUS_FREEZE)
		StringCopy(gBattleTextBuff1, gStatusConditionString_Ice);
	gBattleMons[bank].status1 = 0;
	gBattleMons[bank].status2 &= ~(STATUS2_NIGHTMARE);
	EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 4, &gBattleMons[bank].status1);
}

bool8 AllStatsButOneAreMinned(bank_t bank) {
	u8 counter = 0;
	for (u8 i = 0; i < BATTLE_STATS_NO-1; ++i) {
		if (gBattleMons[bank].statStages[i] > 0) {
			++counter;
			if (counter > 1)
				return FALSE;
		}
	}
	return TRUE;
}

