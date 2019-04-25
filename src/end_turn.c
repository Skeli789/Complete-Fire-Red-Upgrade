#include "defines.h"
#include "defines_battle.h"
#include "../include/new/end_turn.h"
#include "../include/new/helper_functions.h"

extern u8 BattleScript_MysteriousAirCurrentContinues[];
extern u8 BattleScript_FogEnded[];
extern u8 BattleScript_FogContinues[];
extern u8 BattleScript_SeaOfFireDamage[];
extern u8 BattleScript_GrassyTerrainHeal[];
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
extern u8 BattleScript_ToxicOrb[];
extern u8 BattleScript_FlameOrb[];
extern u8 BattleScript_TransformedEnd2[];
extern u8 BattleScript_PowerConstruct[];
extern u8 BattleScript_StartedSchooling[];
extern u8 BattleScript_StoppedSchooling[];
extern u8 BattleScript_ShieldsDownToCore[];
extern u8 BattleScript_ShieldsDownToMeteor[];
extern u8 BattleScript_FlowerGift[];
extern u8 BattleScript_RaiseStatsItemEnd2[];
extern u8 BattleScript_HandleFaintedMonDoublesInitial[];
extern u8 BattleScript_HandleFaintedMonDoublesPart2[];
extern u8 BattleScript_HandleFaintedMonDoublesSwitchInEffects[];

extern u8 GetWhoStrikesFirst(bank_t, bank_t, bool8 ignoreMovePriorities);
extern bool8 HasNoMonsToSwitch(u8 battler);

u8 TurnBasedEffects(void);
void ClearBankStatus(bank_t);
u8 CountAliveMonsOnField(void);

u8 TurnBasedEffects(void) {
	int i, j;
    u8 effect = 0;
	
	if (gBattleTypeFlags & BATTLE_TYPE_SAFARI)
		return 0;

    gHitMarker |= (HITMARKER_GRUDGE | HITMARKER_x20);
    while (gBattleStruct->turnEffectsTracker <= TURNBASED_MAX_CASE)
    {
		u8 sideBank;
        gActiveBattler = gBankAttacker = gBankTarget = gBanksByTurnOrder[gBattleStruct->turnEffectsBank];
		
        switch (gBattleStruct->turnEffectsTracker) {
			case(ET_Order):
				for (i = 0; i < gBattlersCount; ++i)
				{
					gBanksByTurnOrder[i] = i;
				}
				for (i = 0; i < gBattlersCount - 1; ++i)
				{
					for (j = i + 1; j < gBattlersCount; ++j)
					{
						if (GetWhoStrikesFirst(gBanksByTurnOrder[i], gBanksByTurnOrder[j], 0))
							SwapTurnOrder(i, j);
					}
				}
				++gBattleStruct->turnEffectsTracker;
			__attribute__ ((fallthrough));
			
			case(ET_General_Counter_Decrement):
				for (i = 0; i < 4; ++i) 
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
						
					gBattleMons[i].status2 &= ~(STATUS2_FLINCHED);
				}

				//These should be cleared earlier on, but just in case they aren't
				gNewBS->PowderByte = 0;
				gNewBS->BeakBlastByte = 0;
				gNewBS->playedFocusPunchMessage = 0;
				gNewBS->playedShellTrapMessage = 0;
				gNewBS->CustapQuickClawIndicator = 0;
				gNewBS->criticalCapture = 0;
				
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
					
				gSideAffecting[0] &= ~(SIDE_STATUS_CRAFTY_SHIELD | SIDE_STATUS_MAT_BLOCK | SIDE_STATUS_QUICK_GUARD | SIDE_STATUS_WIDE_GUARD);
				gSideAffecting[1] &= ~(SIDE_STATUS_CRAFTY_SHIELD | SIDE_STATUS_MAT_BLOCK | SIDE_STATUS_QUICK_GUARD | SIDE_STATUS_WIDE_GUARD);
					
				gNewBS->NoMoreMovingThisTurn = 0;
				gNewBS->fusionFlareUsedPrior = FALSE;
				gNewBS->fusionBoltUsedPrior = FALSE;
					
				++gBattleStruct->turnEffectsTracker;
			__attribute__ ((fallthrough));

			case(ET_Rain):
				if (gBattleWeather & WEATHER_RAIN_ANY)
				{
					if (!(gBattleWeather & WEATHER_RAIN_PERMANENT)
					&& --gWishFutureKnock->weatherDuration == 0)
					{
						gBattleWeather &= ~WEATHER_RAIN_ANY;
						gBattleCommunication[MULTISTRING_CHOOSER] = 2;
					}
					else if (gBattleWeather & WEATHER_RAIN_DOWNPOUR)
					{
						gBattleCommunication[MULTISTRING_CHOOSER] = 1;
					}
					else
					{
						gBattleCommunication[MULTISTRING_CHOOSER] = 0;
					}

					BattleScriptExecute(BattleScript_RainContinuesOrEnds);
					effect++;
				}
				++gBattleStruct->turnEffectsTracker;
				gBattleStruct->turnEffectsBank = 0;
				break;
			
			case(ET_Sun):
				if (gBattleWeather & WEATHER_SUN_ANY)
				{
					if (!(gBattleWeather & WEATHER_SUN_PERMANENT) 
					&& --gWishFutureKnock->weatherDuration == 0)
					{
						gBattleWeather &= ~WEATHER_SUN_ANY;
						gBattlescriptCurrInstr = BattleScript_SunlightFaded;
					}
					else
					{
						gBattlescriptCurrInstr = BattleScript_SunlightContinues;
					}
					BattleScriptExecute(gBattlescriptCurrInstr);
					effect++;
				}
				++gBattleStruct->turnEffectsTracker;
				gBattleStruct->turnEffectsBank = 0;
				break;
			
			case(ET_Sandstorm):
				if (gBattleWeather & WEATHER_SANDSTORM_ANY)
				{
					if (!(gBattleWeather & WEATHER_SANDSTORM_PERMANENT) 
					&& --gWishFutureKnock->weatherDuration == 0)
					{
						gBattleWeather &= ~WEATHER_SANDSTORM_ANY;
						gBattlescriptCurrInstr = BattleScript_SandStormHailEnds;
					}
					else
					{
						gBattlescriptCurrInstr = BattleScript_DamagingWeatherContinues;
					}

					gBattleScripting->animArg1 = B_ANIM_SANDSTORM_CONTINUES;
					gBattleCommunication[MULTISTRING_CHOOSER] = 0;
					BattleScriptExecute(gBattlescriptCurrInstr);
					effect++;
				}
				++gBattleStruct->turnEffectsTracker;
				gBattleStruct->turnEffectsBank = 0;
				break;
			
			case(ET_Hail):
				if (gBattleWeather & WEATHER_HAIL_ANY)
				{
					if (!(gBattleWeather & WEATHER_HAIL_PERMANENT)
					&& --gWishFutureKnock->weatherDuration == 0)
					{
						gBattleWeather &= ~WEATHER_HAIL_ANY;
						gBattlescriptCurrInstr = BattleScript_SandStormHailEnds;
					}
					else
					{
						gBattlescriptCurrInstr = BattleScript_DamagingWeatherContinues;
					}

					gBattleScripting->animArg1 = B_ANIM_HAIL_CONTINUES;
					gBattleCommunication[MULTISTRING_CHOOSER] = 1;
					BattleScriptExecute(gBattlescriptCurrInstr);
					effect++;
				}
				++gBattleStruct->turnEffectsTracker;
				gBattleStruct->turnEffectsBank = 0;
				break;
			
			case(ET_Air_Current):
				if (gBattleWeather & WEATHER_AIR_CURRENT_PRIMAL)
				{
					BattleScriptExecute(BattleScript_MysteriousAirCurrentContinues);
					effect++;
				}
				++gBattleStruct->turnEffectsTracker;
				gBattleStruct->turnEffectsBank = 0;
				break;
			
			case(ET_Fog):
				if (gBattleWeather & WEATHER_FOG_ANY)
				{
					if (!(gBattleWeather & WEATHER_FOG_PERMANENT) 
					&& --gWishFutureKnock->weatherDuration == 0)
					{
						gBattleWeather &= ~WEATHER_FOG_ANY;
						gBattlescriptCurrInstr = BattleScript_FogEnded;
					}
					else
					{
						gBattlescriptCurrInstr = BattleScript_FogContinues;
					}
					BattleScriptExecute(gBattlescriptCurrInstr);
					effect++;
				}
				++gBattleStruct->turnEffectsTracker;
				gBattleStruct->turnEffectsBank = 0;
				break;

			case(ET_Weather_Health_Abilities):
				if (gBattleMons[gActiveBattler].hp) 
				{
					gLastUsedAbility = ABILITY(gActiveBattler);
					switch(gLastUsedAbility) {
						case ABILITY_RAINDISH:
						case ABILITY_DRYSKIN:
						case ABILITY_ICEBODY:
						case ABILITY_SOLARPOWER:
							if (AbilityBattleEffects(ABILITYEFFECT_ENDTURN, gActiveBattler, 0, 0, 0))
								effect++;
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
				for (i = 0; i < 4; ++i) //Cleared down here because necessary for Future Sight
				{
					if(gNewBS->ElectrifyTimers[i])
						--gNewBS->ElectrifyTimers[i];
				}
				
                if (gWishFutureKnock->wishCounter[gActiveBattler] 
				&& --gWishFutureKnock->wishCounter[gActiveBattler] == 0 
				&& gBattleMons[gActiveBattler].hp) 
				{
                    BattleScriptExecute(BattleScript_WishComesTrue);
                    effect++;
                }
				gNewBS->blockTracker = 0; //For later
                break;
				
			case(ET_Block_A):
				switch(gNewBS->blockTracker) {
					case(ET_Sea_Of_Fire):
						if (gNewBS->SeaOfFireTimers[SIDE(gActiveBattler)] && 
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
						if (gBattleMons[gActiveBattler].hp)
						{
							if (ItemBattleEffects(ItemEffects_EndTurn, gActiveBattler, FALSE, FALSE))
								effect++;
						}
						break;
						
					case(ET_Hydration_ShedSkin):
						if (gBattleMons[gActiveBattler].hp)
						{
							switch(gBattleMons[gActiveBattler].ability) {
								case ABILITY_SHEDSKIN:
								case ABILITY_HYDRATION:
									if (AbilityBattleEffects(ABILITYEFFECT_ENDTURN, gActiveBattler, 0, 0, 0))
										effect++;
							}
						}
						break;
						
					case(ET_Healer):
						if (gBattleMons[gActiveBattler].hp)
						{
							switch(gBattleMons[gActiveBattler].ability) {
								case ABILITY_HEALER:
									if (AbilityBattleEffects(ABILITYEFFECT_ENDTURN, gActiveBattler, 0, 0, 0))
										effect++;
							}
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
                if (gStatuses3[gActiveBattler] & STATUS3_LEECHSEED
				&& gBattleMons[gStatuses3[gActiveBattler] & STATUS3_LEECHSEED_BATTLER].hp
				&& gBattleMons[gStatuses3[gActiveBattler] & STATUS3_LEECHSEED_BATTLER].ability != ABILITY_MAGICGUARD
				&& gBattleMons[gActiveBattler].hp) 
				{	
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
                if ((gBattleMons[gActiveBattler].status1 & STATUS_POISON) 
				&& gBattleMons[gActiveBattler].hp
				&& ABILITY(gActiveBattler) != ABILITY_MAGICGUARD) 
				{
					gBattleMoveDamage = MathMax(1, gBattleMons[gActiveBattler].maxHP / 8);
					
					if (ABILITY(gActiveBattler) == ABILITY_POISONHEAL) 
					{
						if (!BATTLER_MAX_HP(gActiveBattler) && !gNewBS->HealBlockTimers[gActiveBattler])
						{
							gBattleMoveDamage *= -1;
							gBattleScripting->bank = gActiveBattler;
							BattleScriptExecute(BattleScript_PoisonHeal);
						}
					}
					else
						BattleScriptExecute(BattleScript_PoisonTurnDmg);
					++effect;
                }
                break;
				
			case(ET_Toxic):
                if ((gBattleMons[gActiveBattler].status1 & STATUS_TOXIC_POISON)
				&& gBattleMons[gActiveBattler].hp
				&& ABILITY(gActiveBattler) != ABILITY_MAGICGUARD) 
				{
					gBattleMoveDamage = MathMax(1, gBattleMons[gActiveBattler].maxHP / 16);
							
					if ((gBattleMons[gActiveBattler].status1 & 0xF00) != 0xF00) //not 16 turns
						gBattleMons[gActiveBattler].status1 += 0x100;
							
					gBattleMoveDamage *= (gBattleMons[gActiveBattler].status1 & 0xF00) >> 8;
						
					if (ABILITY(gActiveBattler) == ABILITY_POISONHEAL) 
					{
						if (!BATTLER_MAX_HP(gActiveBattler) && !gNewBS->HealBlockTimers[gActiveBattler])
						{
							gBattleMoveDamage = MathMax(1, gBattleMons[gActiveBattler].maxHP / 8) * -1;
							gBattleScripting->bank = gActiveBattler;
							BattleScriptExecute(BattleScript_PoisonHeal);
						}
					}
					else
						BattleScriptExecute(BattleScript_PoisonTurnDmg);
					effect++;
                }
				break;
				
			case(ET_Burn):
                if ((gBattleMons[gActiveBattler].status1 & STATUS_BURN)			 
				&& gBattleMons[gActiveBattler].hp
				&& gBattleMons[gActiveBattler].ability != ABILITY_MAGICGUARD) 
				{
					if (ABILITY(gActiveBattler) == ABILITY_HEATPROOF) {
						#ifdef OLD_BURN_DAMAGE
							gBattleMoveDamage = MathMax(1, gBattleMons[gActiveBattler].maxHP / 16);
						#else
							gBattleMoveDamage = MathMax(1, gBattleMons[gActiveBattler].maxHP / 32);
						#endif
					}
					else {
						#ifdef OLD_BURN_DAMAGE
							gBattleMoveDamage = MathMax(1, gBattleMons[gActiveBattler].maxHP / 8);
						#else
							gBattleMoveDamage = MathMax(1, gBattleMons[gActiveBattler].maxHP / 16);
						#endif
					}
					BattleScriptExecute(BattleScript_BurnTurnDmg);
					effect++;
                }
                break;
				
			case(ET_Nightmare):
                if (gBattleMons[gActiveBattler].status2 & STATUS2_NIGHTMARE) {
					if (gBattleMons[gActiveBattler].status1 & STATUS_SLEEP) {
						if (gBattleMons[gActiveBattler].hp
						&&  gBattleMons[gActiveBattler].ability != ABILITY_MAGICGUARD) {
								gBattleMoveDamage = MathMax(1, gBattleMons[gActiveBattler].maxHP / 4);
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
						gBattleMoveDamage = MathMax(1, gBattleMons[gActiveBattler].maxHP / 4);
						BattleScriptExecute(BattleScript_CurseTurnDmg);
						effect++;
                }
                break;
				
			case(ET_Trap_Damage):
				if (gNewBS->brokeFreeMessage & gBitTable[gActiveBattler]
				&& gBattleMons[gActiveBattler].hp) 
				{
					//Broke free
					gBattleMons[gActiveBattler].status2 &= ~(STATUS2_WRAPPED);
					gNewBS->brokeFreeMessage &= ~(gBitTable[gActiveBattler]);
					
					gBattleTextBuff1[0] = 0xFD;
					gBattleTextBuff1[1] = 2;
					gBattleTextBuff1[2] = gBattleStruct->wrappedMove[gActiveBattler];
					gBattleTextBuff1[3] = gBattleStruct->wrappedMove[gActiveBattler] >> 8;
					gBattleTextBuff1[4] = EOS;
					gBattlescriptCurrInstr = BattleScript_WrapEnds;
					BattleScriptExecute(gBattlescriptCurrInstr);
					effect++;
				}
                else if ((gBattleMons[gActiveBattler].status2 & STATUS2_WRAPPED)
				&& gBattleMons[gActiveBattler].hp) 
				{
					gBattleMons[gActiveBattler].status2 -= 0x2000;
					
					if (ABILITY(gActiveBattler) != ABILITY_MAGICGUARD)
					{
						//Damaged by wrap
						gBattleScripting->animArg1 = gBattleStruct->wrappedMove[gActiveBattler];
						gBattleScripting->animArg2 = gBattleStruct->wrappedMove[gActiveBattler] >> 8;
						gBattleTextBuff1[0] = 0xFD;
						gBattleTextBuff1[1] = 2;
						gBattleTextBuff1[2] = gBattleStruct->wrappedMove[gActiveBattler];
						gBattleTextBuff1[3] = gBattleStruct->wrappedMove[gActiveBattler] >> 8;
						gBattleTextBuff1[4] = EOS;
						gBattlescriptCurrInstr = BattleScript_WrapTurnDmg;
								
						if (ITEM_EFFECT(gActiveBattler) == ITEM_EFFECT_BINDING_BAND)
							gBattleMoveDamage = MathMax(1, udivsi(gBattleMons[gActiveBattler].maxHP, 6));
						else
							gBattleMoveDamage = MathMax(1, gBattleMons[gActiveBattler].maxHP / 8);

						BattleScriptExecute(gBattlescriptCurrInstr);
						effect++;
					}
					
					if (!(gBattleMons[gActiveBattler].status2 & STATUS2_WRAPPED))
					{
						gBattleMons[gActiveBattler].status2 |= STATUS2_WRAPPED; //Reactivate temporarily
						gNewBS->brokeFreeMessage |= gBitTable[gActiveBattler]; //Will play next turn
					}
				}
                break;
				
			case(ET_Taunt_Timer):
                if (gDisableStructs[gActiveBattler].tauntTimer)
                    gDisableStructs[gActiveBattler].tauntTimer--;
					//Display message?
                break;
				
			case(ET_Encore_Timer):
                if (gDisableStructs[gActiveBattler].encoreTimer && gBattleMons[gActiveBattler].hp) 
				{
                    if (gBattleMons[gActiveBattler].moves[gDisableStructs[gActiveBattler].encoredMovePos] != gDisableStructs[gActiveBattler].encoredMove) {  // pokemon does not have the encored move anymore
                        gDisableStructs[gActiveBattler].encoredMove = 0;
                        gDisableStructs[gActiveBattler].encoreTimer = 0;
                    }
                    else if (--gDisableStructs[gActiveBattler].encoreTimer == 0
						  || gBattleMons[gActiveBattler].pp[gDisableStructs[gActiveBattler].encoredMovePos] == 0) 
					{
						gDisableStructs[gActiveBattler].encoredMove = 0;
						gDisableStructs[gActiveBattler].encoreTimer = 0;
						BattleScriptExecute(BattleScript_EncoredNoMore);
						effect++;
                    }
                }
                break;
				
			case(ET_Disable_Timer):
                if (gDisableStructs[gActiveBattler].disableTimer1 && gBattleMons[gActiveBattler].hp) {
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
				if (gNewBS->MagnetRiseTimers[gActiveBattler] && gBattleMons[gActiveBattler].hp && --gNewBS->MagnetRiseTimers[gActiveBattler] == 0) {
					BattleScriptExecute(BattleScript_MagnetRiseEnd);
					effect++;
				}
				break;
					
			case(ET_Telekinesis_Timer):
				if (gNewBS->TelekinesisTimers[gActiveBattler] && gBattleMons[gActiveBattler].hp && --gNewBS->TelekinesisTimers[gActiveBattler] == 0) {
					BattleScriptExecute(BattleScript_TelekinesisEnd);
					effect++;
				}
				break;
					
			case(ET_Heal_Block_Timer):
				if (gNewBS->HealBlockTimers[gActiveBattler] && gBattleMons[gActiveBattler].hp && --gNewBS->HealBlockTimers[gActiveBattler] == 0) {
					BattleScriptExecute(BattleScript_HealBlockEnd);
					effect++;
				}
				break;
					
			case(ET_Embargo_Timer):
				if (gNewBS->EmbargoTimers[gActiveBattler] && gBattleMons[gActiveBattler].hp && --gNewBS->EmbargoTimers[gActiveBattler] == 0) {
					BattleScriptExecute(BattleScript_EmbargoEnd);
					effect++;
				}
				break;
					
			case(ET_Yawn):
                if (gStatuses3[gActiveBattler] & STATUS3_YAWN && gBattleMons[gActiveBattler].hp) {
                    gStatuses3[gActiveBattler] -= 0x800;
					
                    if (!(gStatuses3[gActiveBattler] & STATUS3_YAWN) && 
						!(gBattleMons[gActiveBattler].status1 & STATUS_ANY) &&
						  gBattleMons[gActiveBattler].species != SPECIES_MINIOR_SHIELD &&
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
                if (gStatuses3[gActiveBattler] & STATUS3_PERISH_SONG && gBattleMons[gActiveBattler].hp) {
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
			case(ET_Seeds): ;
				u8 stat = 0;
	
				switch (TerrainType) {
					case ELECTRIC_TERRAIN:
					case GRASSY_TERRAIN:
						stat = STAT_STAGE_DEF;
						break;
					case MISTY_TERRAIN:
					case PSYCHIC_TERRAIN:
						stat = STAT_STAGE_SPDEF;
				}
	
				if (ITEM_EFFECT(gActiveBattler) == ITEM_EFFECT_SEEDS
				&&  ITEM_QUALITY(gActiveBattler) == TerrainType
				&&  STAT_CAN_RISE(gActiveBattler, stat)
				&& gBattleMons[gActiveBattler].hp)
				{	
					PREPARE_STAT_BUFFER(gBattleTextBuff1, stat);
					gBattleScripting->statChanger = stat | INCREASE_1;
					gBattleScripting->bank = gBankTarget = gEffectBank = gActiveBattler;
					BattleScriptExecute(BattleScript_RaiseStatsItemEnd2);
					++effect;
				}
				break;
				
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
						if (gBattleMons[gActiveBattler].status2 & STATUS2_UPROAR && gBattleMons[gActiveBattler].hp) {
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
							gLastUsedAbility = ABILITY(gActiveBattler);
							switch(gLastUsedAbility) {
							case ABILITY_SPEEDBOOST:
							case ABILITY_TRUANT:
							case ABILITY_MOODY:
							case ABILITY_BADDREAMS:
								if (AbilityBattleEffects(ABILITYEFFECT_ENDTURN, gActiveBattler, 0, 0, 0))
									effect++;
								break;
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
						if (gBattleMons[gActiveBattler].hp 
						&& (!gBattleMons[gActiveBattler].item || ABILITY(gActiveBattler) == ABILITY_SLOWSTART)) 
						{
							switch(ABILITY(gActiveBattler)) {
							case ABILITY_HARVEST:
							case ABILITY_PICKUP:
							case ABILITY_SLOWSTART:
								if (AbilityBattleEffects(ABILITYEFFECT_ENDTURN, gActiveBattler, 0, 0, 0))
									effect++;
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
				if (gBattleMons[gActiveBattler].hp && !(gBattleMons[gActiveBattler].status2 & STATUS2_TRANSFORMED)) 
				{
					pokemon_t* partydata = GetBankPartyData(gActiveBattler);
					bool8 changedform = FALSE;
					u16 species = partydata->species;
					u8 ability = ABILITY(gActiveBattler);
					u16 newspecies = 0;
					bool8 reloadType = FALSE;
					bool8 reloadStats = FALSE;
					u8* battleScript;
					switch(ability) {
						case ABILITY_ZENMODE:
							if (species == SPECIES_DARMANITAN) 
							{
								newspecies = SPECIES_DARMANITANZEN;
								changedform = TRUE;
								reloadType = TRUE;
								reloadStats = TRUE;
								battleScript = BattleScript_TransformedEnd2;
							}
							break;
						
						case ABILITY_POWERCONSTRUCT:
							if (species == SPECIES_ZYGARDE || species == SPECIES_ZYGARDE_10) 
							{
								newspecies = SPECIES_ZYGARDE_COMPLETE;
								reloadType = TRUE;
								reloadStats = TRUE;
								changedform = TRUE;
								battleScript = BattleScript_PowerConstruct;
							}
							break;
						
						case ABILITY_SCHOOLING:
							if (species == SPECIES_WISHIWASHI && gBattleMons[gActiveBattler].level >= 20
							&& gBattleMons[gActiveBattler].hp > gBattleMons[gActiveBattler].maxHP / 4)
							{
								newspecies = SPECIES_WISHIWASHI_S;
								changedform = TRUE;
								reloadStats = TRUE;
								battleScript = BattleScript_StartedSchooling;
							}
							else if (species == SPECIES_WISHIWASHI_S
								&&  (gBattleMons[gActiveBattler].level < 20
								  || gBattleMons[gActiveBattler].hp <= gBattleMons[gActiveBattler].maxHP / 4)) 
							{
								newspecies = SPECIES_WISHIWASHI;
								changedform = TRUE;
								reloadStats = TRUE;
								battleScript = BattleScript_StoppedSchooling;
							}
							break;
							
						case ABILITY_SHIELDSDOWN:
							if (species == SPECIES_MINIOR_SHIELD 
							&& gBattleMons[gActiveBattler].hp <= gBattleMons[gActiveBattler].maxHP / 2)
							{
								newspecies = umodsi(partydata->personality, 7); //Get Minior Colour
								changedform = TRUE;
								reloadStats = TRUE;
								battleScript = BattleScript_ShieldsDownToCore;
							}
							else if ((species == SPECIES_MINIOR_RED
								  || species == SPECIES_MINIOR_BLUE
								  || species == SPECIES_MINIOR_ORANGE
								  || species == SPECIES_MINIOR_YELLOW
								  || species == SPECIES_MINIOR_INDIGO
								  || species == SPECIES_MINIOR_GREEN
								  || species == SPECIES_MINIOR_VIOLET) && gBattleMons[gActiveBattler].hp > (gBattleMons[gActiveBattler].maxHP / 2)) 
							{
								newspecies = SPECIES_MINIOR_SHIELD;
								changedform = TRUE;
								reloadStats = TRUE;
								battleScript = BattleScript_ShieldsDownToMeteor;
							}
							break;
							
						case ABILITY_FLOWERGIFT:
							if (species == SPECIES_CHERRIM_SUN && (!WEATHER_HAS_EFFECT || !(gBattleWeather & WEATHER_SUN_ANY) || ability != ABILITY_FLOWERGIFT)) 
							{
								newspecies = SPECIES_CHERRIM;
								changedform = TRUE;
								battleScript = BattleScript_FlowerGift;
							}
					}
					
					if (changedform) 
					{
						gBattleScripting->bank = gActiveBattler;
						DoFormChange(gActiveBattler, newspecies, reloadType, reloadStats);
						BattleScriptExecute(battleScript);
						++effect;
					}
				}
				break;
						
			case(ET_End):
			    gBattleStruct->turnEffectsBank = gBattlersCount;
				gNewBS->EndTurnDone = TRUE;
				gAbsentBattlerFlags &= ~(gNewBS->AbsentBattlerHelper);
				gNewBS->MegaData->state = 0;
				
				for (int i = 0; i < gBattlersCount; ++i)
					gNewBS->pickupStack[i] = 0xFF;
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

#define FAINTED_ACTIONS_MAX_CASE 9

bool8 HandleFaintedMonActions(void)
{
	int i, j;
	
    if (gBattleTypeFlags & BATTLE_TYPE_SAFARI)
        return FALSE;
	
    do
    {
        switch (gBattleStruct->faintedActionsState) {
			case 0:
				gBattleStruct->faintedActionsBank = 0;
				gBattleStruct->faintedActionsState++;
				for (i = 0; i < gBattlersCount; i++)
				{
					if (gAbsentBattlerFlags & gBitTable[i] && !HasNoMonsToSwitch(i))
						gAbsentBattlerFlags &= ~(gBitTable[i]);
				}
				__attribute__ ((fallthrough));
			
			case 1:
				do
				{
					gBankFainted = gBankTarget = gBattleStruct->faintedActionsBank;
					if (gBattleMons[gBattleStruct->faintedActionsBank].hp == 0
					&& !(gBattleStruct->givenExpMons & gBitTable[gBattlerPartyIndexes[gBattleStruct->faintedActionsBank]])
					&& !(gAbsentBattlerFlags & gBitTable[gBattleStruct->faintedActionsBank]))
					{
						BattleScriptExecute(BattleScript_GiveExp);
						gBattleStruct->faintedActionsState = 2;
						return TRUE;
					}
				} while (++gBattleStruct->faintedActionsBank != gBattlersCount);
				
				gBattleStruct->faintedActionsState = 3;
				break;
			
			case 2:
				sub_8017434(gBankFainted);
				if (++gBattleStruct->faintedActionsBank == gBattlersCount)
					gBattleStruct->faintedActionsState = 3;
				else
					gBattleStruct->faintedActionsState = 1;
				break;
			
			case 3:
				gBattleStruct->faintedActionsBank = 0;
				gBattleStruct->faintedActionsState++;
				__attribute__ ((fallthrough));
			
			case 4:
				do
				{
					gBankFainted = gBankTarget = gBattleStruct->faintedActionsBank;
					if (gBattleMons[gBattleStruct->faintedActionsBank].hp == 0
					&& !(gAbsentBattlerFlags & gBitTable[gBattleStruct->faintedActionsBank]))
					{
						if (gNewBS->EndTurnDone 
						||  ViableMonCountFromBank(gBattleStruct->faintedActionsBank) == 0)
						{
							//Double battle that's not wild
							//This allows the AI to know what you're sending out and react accordingly in regular Single Battles (ie send in a mon strong against you)
							if (gBattleTypeFlags & BATTLE_TYPE_TRAINER && gBattleTypeFlags & (BATTLE_TYPE_DOUBLE | BATTLE_TYPE_FRONTIER | BATTLE_TYPE_LINK))
							{
								gNewBS->handleDoublesSwitchIns |= gBitTable[gBattleStruct->faintedActionsBank];
								BattleScriptExecute(BattleScript_HandleFaintedMonDoublesInitial);
							}
							else
								BattleScriptExecute(BattleScript_HandleFaintedMon);
							
							gBattleStruct->faintedActionsState = 5;
							return TRUE;
						}
						else
						{
							gAbsentBattlerFlags |= gBitTable[gBattleStruct->faintedActionsBank]; //Makes the game realize the target is dead for now, so no attacking it
							gNewBS->AbsentBattlerHelper |= gBitTable[gBattleStruct->faintedActionsBank]; //Record which Pokemon need replacements
						}
					}
				} while (++gBattleStruct->faintedActionsBank < gBattlersCount);
				
				if (gNewBS->handleDoublesSwitchIns)
				{
					gBattleStruct->faintedActionsBank = 0;
					gBattleStruct->faintedActionsState = 6;
				}
				else
					gBattleStruct->faintedActionsState = 8;
				break;
			
			case 5:
				if (++gBattleStruct->faintedActionsBank >= gBattlersCount)
				{
					if (gNewBS->handleDoublesSwitchIns)
					{
						gBattleStruct->faintedActionsBank = 0;
						gBattleStruct->faintedActionsState = 6;
					}
					else
						gBattleStruct->faintedActionsState = 8;
				}
				else
					gBattleStruct->faintedActionsState = 4;
				break;
			
			case 6:
				do
				{
					gBankFainted = gBankTarget = gBattleStruct->faintedActionsBank;
					if (gNewBS->handleDoublesSwitchIns & gBitTable[gBattleStruct->faintedActionsBank])
					{
						++gBattleStruct->faintedActionsBank;
						BattleScriptExecute(BattleScript_HandleFaintedMonDoublesPart2);
						return TRUE;
					}
				} while (++gBattleStruct->faintedActionsBank < gBattlersCount);
				
				//Recalc turn order for switch-in abilities
				for (i = 0; i < gBattlersCount; ++i)
					gBanksByTurnOrder[i] = i;
				
				for (i = 0; i < gBattlersCount - 1; ++i) 
				{
					for (j = i + 1; j < gBattlersCount; ++j) 
					{
						if (GetWhoStrikesFirst(gBanksByTurnOrder[i], gBanksByTurnOrder[j], 1))
							SwapTurnOrder(i, j);
					}
				}
				
				gBattleStruct->faintedActionsBank = 0;
				gBattleStruct->faintedActionsState++;
				__attribute__ ((fallthrough));
			
			case 7:
				do
					{
						gBankFainted = gBankTarget = gBattleStruct->faintedActionsBank;
						if (gNewBS->handleDoublesSwitchIns & gBitTable[gBattleStruct->faintedActionsBank])
						{
							gNewBS->handleDoublesSwitchIns &= ~(gBitTable[gBattleStruct->faintedActionsBank]);
							BattleScriptExecute(BattleScript_HandleFaintedMonDoublesSwitchInEffects);
							return TRUE;
						}
					} while (++gBattleStruct->faintedActionsBank < gBattlersCount);
				gBattleStruct->faintedActionsState++;
				__attribute__ ((fallthrough));
			
			case 8:
				gNewBS->EndTurnDone = FALSE;
				gBattleStruct->faintedActionsState++;
				break;
			
			case FAINTED_ACTIONS_MAX_CASE:
				break;
        }
    } while (gBattleStruct->faintedActionsState != FAINTED_ACTIONS_MAX_CASE);
	
    return FALSE;
}

u8 CountAliveMonsOnField(void) 
{
	u8 count = 0;
	
	for (int i = 0; i < gBattlersCount; ++i)
	{
		if (gBattleMons[i].hp)
			++count;
	}
	return count;
}

void RemoveSwitchInForFaintedBank(void)
{
	gNewBS->handleDoublesSwitchIns &= ~(gBitTable[gBankFainted]);
}
