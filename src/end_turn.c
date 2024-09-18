#include "defines.h"
#include "defines_battle.h"
#include "../include/battle_string_ids.h"
#include "../include/random.h"
#include "../include/constants/items.h"

#include "../include/new/ai_master.h"
#include "../include/new/battle_start_turn_start.h"
#include "../include/new/battle_script_util.h"
#include "../include/new/battle_util.h"
#include "../include/new/damage_calc.h"
#include "../include/new/dynamax.h"
#include "../include/new/end_turn.h"
#include "../include/new/end_turn_battle_scripts.h"
#include "../include/new/form_change.h"
#include "../include/new/general_bs_commands.h"
#include "../include/new/item_battle_scripts.h"
#include "../include/new/move_battle_scripts.h"
#include "../include/new/switching.h"
#include "../include/new/util.h"

/*
end_turn.c
	handles all effects that happen at the end of each turn
*/

enum EndTurnEffects
{
	ET_Order,
	ET_General_Counter_Decrement,
	ET_Weather,
	ET_Weather_Damage_Health_Abilities,
	ET_Future_Sight,
	ET_Wish,
	ET_Block_A,
	ET_Aqua_Ring,
	ET_Ingrain,
	ET_Leech_Seed,
	ET_Item_Effects2,
	ET_Switch_Out_Abilities2,
	ET_Poison,
	ET_Item_Effects3,
	ET_Switch_Out_Abilities3,
	ET_Burn,
	ET_Frostbite,
	ET_Item_Effects4,
	ET_Switch_Out_Abilities4,
	ET_Nightmare,
	ET_Item_Effects5,
	ET_Switch_Out_Abilities5,
	ET_BadThoughts,
	ET_Item_Effects6,
	ET_Switch_Out_Abilities6,
	ET_Curse,
	ET_Item_Effects7,
	ET_Switch_Out_Abilities7,
	ET_Trap_Damage,
	ET_Item_Effects8,
	ET_Switch_Out_Abilities8,
	ET_Splinters,
	ET_Item_Effects9,
	ET_Switch_Out_Abilities9,
	ET_Octolock,
	ET_Taunt_Timer,
	ET_Encore_Timer,
	ET_Disable_Timer,
	ET_Lock_On_Timer,
	ET_Charge_Timer,
	ET_Magnet_Rise_Timer,
	ET_Telekinesis_Timer,
	ET_Heal_Block_Timer,
	ET_Embargo_Timer,
	ET_Yawn,
	ET_Item_Effects10,
	ET_Perish_Song,
	ET_Roost,
	ET_Reflect,
	ET_Light_Screen,
	ET_Safeguard,
	ET_Mist,
	ET_Tailwind_Timer,
	ET_Lucky_Chant_Timer,
	ET_Rainbow_Timer,
	ET_Sea_Of_Fire_Timer,
	ET_Swamp_Timer,
	ET_G_Max_VineLash_Timer,
	ET_G_Max_Wildfire_Timer,
	ET_G_Max_Cannonade_Timer,
	ET_G_Max_Volcalith_Timer,
	ET_Aurora_Veil,
	ET_Trick_Room_Timer,
	ET_Gravity_Timer,
	ET_Water_Sport_Timer,
	ET_Mud_Sport_Timer,
	ET_Wonder_Room_Timer,
	ET_Magic_Room_Timer,
	ET_Seeds,
	ET_Terrain_Timer,
	ET_Block_B,
	ET_Form_Change,
	ET_Reactivate_Overworld_Weather,
	ET_Reactivate_Overworld_Terrain,
	ET_SOS,
	ET_End
};

enum Block_A
{
	ET_Sea_Of_Fire,
	ET_G_Max_VineLash,
	ET_G_Max_Wildfire,
	ET_G_Max_Cannonade,
	ET_G_Max_Volcalith,
	ET_Grassy_Terrain,
	ET_Hydration_ShedSkin_Healer,
	ET_Item_Effects,
	ET_Switch_Out_Abilities,
	MAX_CASES_BLOCK_A,
};

enum Block_B
{
	ET_Uproar,
	ET_SpeedBoost_Moody_BadDreams_SlowStart,
	ET_Orbz,
	ET_Harvest_Pickup,
	MAX_CASES_BLOCK_B,
};

#define TURNBASED_MAX_CASE ET_End

const u16 gSandstormHailContinuesStringIds[] =
{
	STRINGID_SANDSTORMRAGES,
	STRINGID_HAILCONTINUES,
	STRINGID_CUSTOMSTRING,
};

const u16 gSandstormHailDmgStringIds[] =
{
	STRINGID_PKMNBUFFETEDBYSANDSTORM,
	STRINGID_PKMNPELTEDBYHAIL,
	STRINGID_CUSTOMSTRING,
};

u8 TurnBasedEffects(u16 move, u8 bank, struct Pokemon* monAtk)
{
	int i, j;
	u8 effect = 0;

	if (gBattleTypeFlags & BATTLE_TYPE_SAFARI)
		return 0;

	gHitMarker |= (HITMARKER_GRUDGE | HITMARKER_x20);
	while (gBattleStruct->turnEffectsTracker <= TURNBASED_MAX_CASE)
	{
		u8 sideBank;
		gActiveBattler = gBankAttacker = gBankTarget = gBanksByTurnOrder[gBattleStruct->turnEffectsBank];
		gBattleMoveDamage = 0; //So Emergency Exit works properly

		switch (gBattleStruct->turnEffectsTracker) {
			case ET_Order:
				for (i = 0; i < gBattlersCount; ++i)
				{
					gBanksByTurnOrder[i] = i;
				}
				for (i = 0; i < gBattlersCount - 1; ++i)
				{
					for (j = i + 1; j < gBattlersCount; ++j)
					{
						if (GetWhoStrikesFirst(gBanksByTurnOrder[i], gBanksByTurnOrder[j], TRUE))
							SwapTurnOrder(i, j);
					}
				}
				++gBattleStruct->turnEffectsTracker;
			__attribute__ ((fallthrough));

			case ET_General_Counter_Decrement:
				for (i = 0; i < MAX_BATTLERS_COUNT; ++i)
				{
					if(gNewBS->ThroatChopTimers[i])
						--gNewBS->ThroatChopTimers[i];

					if(gNewBS->StompingTantrumTimers[i])
						--gNewBS->StompingTantrumTimers[i];

					if(gNewBS->GlaiveRushTimers[i])
						--gNewBS->GlaiveRushTimers[i];

					if (gNewBS->StakeoutCounters[i])
						--gNewBS->StakeoutCounters[i];

					if (gNewBS->DestinyBondCounters[i])
						--gNewBS->DestinyBondCounters[i];

					if (gNewBS->ai.switchingCooldown[i])
						--gNewBS->ai.switchingCooldown[i];

					if (gNewBS->ai.typeAbsorbSwitchingCooldown[i])
						--gNewBS->ai.typeAbsorbSwitchingCooldown[i];

					gNewBS->synchronizeTarget[i] = 0;
					gBattleMons[i].status2 &= ~(STATUS2_FLINCHED);
				}

				//These should be cleared earlier on, but just in case they aren't
				gNewBS->PledgeHelper = FALSE;
				gNewBS->PowderByte = 0;
				gNewBS->BeakBlastByte = 0;
				gNewBS->turnOrderLocked = 0;
				gNewBS->playedFocusPunchMessage = 0;
				gNewBS->playedShellTrapMessage = 0;
				gNewBS->ateCustapBerry = 0;
				gNewBS->quickClawCustapIndicator = 0;
				gNewBS->activatedCustapQuickClaw = 0;
				gNewBS->criticalCapture = 0;

				if (gNewBS->RetaliateCounters[0])
					--gNewBS->RetaliateCounters[0];

				if (gNewBS->RetaliateCounters[1])
					--gNewBS->RetaliateCounters[1];

				if (gNewBS->FairyLockTimer)
					--gNewBS->FairyLockTimer;

				if (gNewBS->EchoedVoiceCounter)
					--gNewBS->EchoedVoiceCounter;

				if (gNewBS->EchoedVoiceCounter == 0)
					gNewBS->EchoedVoiceDamageScale = 0;

				if (gNewBS->activateTemperFlare)
					--gNewBS->activateTemperFlare;

				if (GetMonMoveTypeSpecial(monAtk, move) == TYPE_ELECTRIC)
				{
					gNewBS->ElectroCounter[bank] = 0;
				}

				gSideStatuses[B_SIDE_PLAYER] &= ~(SIDE_STATUS_CRAFTY_SHIELD | SIDE_STATUS_MAT_BLOCK | SIDE_STATUS_QUICK_GUARD | SIDE_STATUS_WIDE_GUARD);
				gSideStatuses[B_SIDE_OPPONENT] &= ~(SIDE_STATUS_CRAFTY_SHIELD | SIDE_STATUS_MAT_BLOCK | SIDE_STATUS_QUICK_GUARD | SIDE_STATUS_WIDE_GUARD);

				gNewBS->NoMoreMovingThisTurn = 0;
				gNewBS->fusionFlareUsedPrior = FALSE;
				gNewBS->fusionBoltUsedPrior = FALSE;
				gNewBS->roundUsed = FALSE;

				if (IsRaidBattle() && !BATTLER_ALIVE(BANK_RAID_BOSS)) //Raid mon is dead so skip end turn effects
				{
					gBattleStruct->turnEffectsTracker = ET_End;
					goto END_TURN_SKIP;
				}

				++gBattleStruct->turnEffectsTracker;
			__attribute__ ((fallthrough));

			case ET_Weather:
				if (gBattleWeather & WEATHER_RAIN_ANY)
				{
					if (!(gBattleWeather & WEATHER_RAIN_PERMANENT)
					&& --gWishFutureKnock.weatherDuration == 0)
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

					if (gHitMarker & HITMARKER_NO_ANIMATIONS)
						gBattlescriptCurrInstr = BattleScript_RainContinuesOrEnds;
					else
						gBattlescriptCurrInstr = BattleScript_RainContinuesOrEndsNoString; //Don't need to print string every time - anim is enough

					BattleScriptExecute(gBattlescriptCurrInstr);
					effect++;
				}
				else if (gBattleWeather & WEATHER_SUN_ANY)
				{
					if (!(gBattleWeather & WEATHER_SUN_PERMANENT)
					&& --gWishFutureKnock.weatherDuration == 0)
					{
						gBattleWeather &= ~WEATHER_SUN_ANY;
						gBattlescriptCurrInstr = BattleScript_SunlightFaded;
					}
					else
					{
						if (gHitMarker & HITMARKER_NO_ANIMATIONS)
							gBattlescriptCurrInstr = BattleScript_SunlightContinues;
						else
							gBattlescriptCurrInstr = BattleScript_SunlightContinuesNoString; //Don't need to print string every time - anim is enough
					}

					BattleScriptExecute(gBattlescriptCurrInstr);
					effect++;
				}
				else if (gBattleWeather & WEATHER_SANDSTORM_ANY)
				{
					if (!(gBattleWeather & WEATHER_SANDSTORM_PERMANENT)
					&& --gWishFutureKnock.weatherDuration == 0)
					{
						gBattleWeather &= ~WEATHER_SANDSTORM_ANY;
						gBattlescriptCurrInstr = BattleScript_SandStormHailEnds;
					}
					else
					{
						if (gHitMarker & HITMARKER_NO_ANIMATIONS)
							gBattlescriptCurrInstr = BattleScript_SandstormHailContinues;
						else
							gBattlescriptCurrInstr = BattleScript_SandstormHailContinuesNoString; //Don't need to print string every time - anim is enough
					}

					gBattleScripting.animArg1 = B_ANIM_SANDSTORM_CONTINUES;
					
					if (gBattleWeather & WEATHER_SANDSTORM_PRIMAL)
					{
						gBattleStringLoader = gText_ViciousSandstormContinues;
						gBattleCommunication[MULTISTRING_CHOOSER] = 2;
					}
					else
					{
						gBattleCommunication[MULTISTRING_CHOOSER] = 0;
					}

					BattleScriptExecute(gBattlescriptCurrInstr);
					effect++;
				}
				else if (gBattleWeather & WEATHER_HAIL_ANY)
				{
					if (!(gBattleWeather & WEATHER_HAIL_PERMANENT)
					&& --gWishFutureKnock.weatherDuration == 0)
					{
						gBattleWeather &= ~WEATHER_HAIL_ANY;
						gBattlescriptCurrInstr = BattleScript_SandStormHailEnds;
					}
					else
					{
						if (gHitMarker & HITMARKER_NO_ANIMATIONS)
							gBattlescriptCurrInstr = BattleScript_SandstormHailContinues;
						else
							gBattlescriptCurrInstr = BattleScript_SandstormHailContinuesNoString; //Don't need to print string every time - anim is enough
					}

					gBattleScripting.animArg1 = B_ANIM_HAIL_CONTINUES;
					gBattleCommunication[MULTISTRING_CHOOSER] = 1;
					BattleScriptExecute(gBattlescriptCurrInstr);
					effect++;
				}
				else if (gBattleWeather & WEATHER_AIR_CURRENT_PRIMAL)
				{
					if (gHitMarker & HITMARKER_NO_ANIMATIONS)
						gBattlescriptCurrInstr = BattleScript_MysteriousAirCurrentContinues;
					else
						gBattlescriptCurrInstr = BattleScript_MysteriousAirCurrentContinuesNoString; //Don't need to print string every time - anim is enough

					BattleScriptExecute(gBattlescriptCurrInstr);
					effect++;
				}
				else if (gBattleWeather & WEATHER_FOG_ANY)
				{
					if (!(gBattleWeather & WEATHER_FOG_PERMANENT)
					&& --gWishFutureKnock.weatherDuration == 0)
					{
						gBattleWeather &= ~WEATHER_FOG_ANY;
						gBattlescriptCurrInstr = BattleScript_FogEnded;
					}
					else
					{
						if (gHitMarker & HITMARKER_NO_ANIMATIONS)
							gBattlescriptCurrInstr = BattleScript_FogContinues;
						else
							gBattlescriptCurrInstr = BattleScript_FogContinuesNoString; //Don't need to print string every time - anim is enough
					}

					BattleScriptExecute(gBattlescriptCurrInstr);
					effect++;
				}
				gBattleStruct->turnEffectsBank = gBattlersCount;
				break;

			case ET_Weather_Damage_Health_Abilities:
				if (BATTLER_ALIVE(gActiveBattler))
				{
					gLastUsedAbility = ABILITY(gActiveBattler);
					switch(gLastUsedAbility) {
						case ABILITY_RAINDISH:
						case ABILITY_DRYSKIN:
						case ABILITY_ICEBODY:
						case ABILITY_SOLARPOWER:
						case ABILITY_EVAPORATE:
							if (AbilityBattleEffects(ABILITYEFFECT_ENDTURN, gActiveBattler, 0, 0, 0))
								effect++;
							break;
						case ABILITY_FORECAST:
							if (AbilityBattleEffects(ABILITYEFFECT_ON_SWITCHIN, gActiveBattler, 0, 0, 0))
								effect++;
							break;
					}

					if (!effect)
					{
						if (gBattleWeather & WEATHER_HAIL_ANY)
						{
							if (TakesDamageFromHail(gActiveBattler))
								effect++;
						}
						else if (gBattleWeather & WEATHER_SANDSTORM_ANY)
						{
							if (TakesDamageFromSandstorm(gActiveBattler))
								effect++;
						}

						if (effect)
							BattleScriptExecute(BattleScript_WeatherDamage);
					}
				}
				break;

			case ET_Future_Sight:
				if (gWishFutureKnock.futureSightCounter[gActiveBattler]
				&& --gWishFutureKnock.futureSightCounter[gActiveBattler] == 0
				&& gBattleMons[gActiveBattler].hp)
				{
					if (gWishFutureKnock.futureSightMove[gActiveBattler] == MOVE_FUTURESIGHT)
						gBattleCommunication[MULTISTRING_CHOOSER] = 0;
					else //Doom Desire
						gBattleCommunication[MULTISTRING_CHOOSER] = 1;

					gCurrentMove = gWishFutureKnock.futureSightMove[gActiveBattler];
					gBattleStruct->dynamicMoveType = GetMoveTypeSpecial(gActiveBattler, gCurrentMove);

					gBattleTextBuff1[0] = B_BUFF_PLACEHOLDER_BEGIN;
					gBattleTextBuff1[1] = B_TXT_COPY_VAR_1;
					gBattleTextBuff1[2] = gWishFutureKnock.futureSightMove[gActiveBattler];
					gBattleTextBuff1[3] = gWishFutureKnock.futureSightMove[gActiveBattler] >> 8;
					gBattleTextBuff1[4] = 0xFF;
					gBankAttacker = gWishFutureKnock.futureSightAttacker[gActiveBattler];
					gSpecialStatuses[gBankTarget].moveturnLostHP = 0xFFFF;
					BattleScriptExecute(BattleScript_MonTookFutureAttack);
					effect++;
				}
				break;

			case ET_Wish:
				if(gNewBS->ElectrifyTimers[gActiveBattler]) //Cleared down here because necessary for Future Sight
					--gNewBS->ElectrifyTimers[gActiveBattler];

				if(gNewBS->LaserFocusTimers[gActiveBattler])
					--gNewBS->LaserFocusTimers[gActiveBattler];

				if (gWishFutureKnock.wishCounter[gActiveBattler]
				&& --gWishFutureKnock.wishCounter[gActiveBattler] == 0
				&& gBattleMons[gActiveBattler].hp)
				{
					BattleScriptExecute(BattleScript_WishComesTrue);
					effect++;
				}
				gNewBS->endTurnBlockState = 0; //For later
				break;

			case ET_Block_A:
				switch(gNewBS->endTurnBlockState) {
					case ET_Sea_Of_Fire:
						if (BATTLER_ALIVE(gActiveBattler)
						&&  BankSideHasSeaOfFire(gActiveBattler)
						&&	ABILITY(gActiveBattler) != ABILITY_MAGICGUARD
						&&	!IsOfType(gActiveBattler, TYPE_FIRE))
						{
							gBattleMoveDamage = GetSeaOfFireDamage(gActiveBattler);
							gBattleStringLoader = gText_HurtBySeaOfFire;
							gBattleScripting.animArg1 = B_ANIM_SEA_OF_FIRE;
							BattleScriptExecute(BattleScript_SeaOfFireDamage);
							effect++;
						}
						gNewBS->turnDamageTaken[gActiveBattler] = gBattleMoveDamage; //For Emergency Exit
						break;

					case ET_G_Max_VineLash:
						if (BATTLER_ALIVE(gActiveBattler)
						&&  BankSideHasGMaxVineLash(gActiveBattler)
						&&	ABILITY(gActiveBattler) != ABILITY_MAGICGUARD
						&&	!IsOfType(gActiveBattler, TYPE_GRASS))
						{
							gBattleMoveDamage = GetGMaxVineLashDamage(gActiveBattler);
							gBattleStringLoader = gText_HurtByGMaxVineLash;
							gBattleScripting.animArg1 = B_ANIM_G_MAX_VINE_LASH;
							BattleScriptExecute(BattleScript_SeaOfFireDamage);
							effect++;
						}
						gNewBS->turnDamageTaken[gActiveBattler] = gBattleMoveDamage; //For Emergency Exit
						break;

					case ET_G_Max_Wildfire:
						if (BATTLER_ALIVE(gActiveBattler)
						&&  BankSideHasGMaxWildfire(gActiveBattler)
						&&	ABILITY(gActiveBattler) != ABILITY_MAGICGUARD
						&&	!IsOfType(gActiveBattler, TYPE_FIRE))
						{
							gBattleMoveDamage = GetGMaxWildfireDamage(gActiveBattler);
							gBattleStringLoader = gText_HurtByGMaxWildfire;
							gBattleScripting.animArg1 = B_ANIM_G_MAX_WILDFIRE;
							BattleScriptExecute(BattleScript_SeaOfFireDamage);
							effect++;
						}
						gNewBS->turnDamageTaken[gActiveBattler] = gBattleMoveDamage; //For Emergency Exit
						break;

					case ET_G_Max_Cannonade:
						if (BATTLER_ALIVE(gActiveBattler)
						&&  BankSideHasGMaxCannonade(gActiveBattler)
						&&	ABILITY(gActiveBattler) != ABILITY_MAGICGUARD
						&&	!IsOfType(gActiveBattler, TYPE_WATER))
						{
							gBattleMoveDamage = GetGMaxCannonadeDamage(gActiveBattler);
							gBattleStringLoader = gText_HurtByGMaxCannonade;
							gBattleScripting.animArg1 = B_ANIM_G_MAX_CANNONADE;
							BattleScriptExecute(BattleScript_SeaOfFireDamage);
							effect++;
						}
						gNewBS->turnDamageTaken[gActiveBattler] = gBattleMoveDamage; //For Emergency Exit
						break;

					case ET_G_Max_Volcalith:
						if (BATTLER_ALIVE(gActiveBattler)
						&&  BankSideHasGMaxVolcalith(gActiveBattler)
						&&	ABILITY(gActiveBattler) != ABILITY_MAGICGUARD)
						{
							gBattleMoveDamage = GetGMaxVolcalithDamage(gActiveBattler);
							gBattleStringLoader = gText_HurtByGMaxVolcalith;
							gBattleScripting.animArg1 = B_ANIM_G_MAX_VOLCALITH;
							BattleScriptExecute(BattleScript_SeaOfFireDamage);
							effect++;
						}
						gNewBS->turnDamageTaken[gActiveBattler] = gBattleMoveDamage; //For Emergency Exit
						break;

					case ET_Grassy_Terrain:
						if (gTerrainType == GRASSY_TERRAIN && CheckGrounding(gActiveBattler)
						&&  BATTLER_ALIVE(gActiveBattler)
						&&  !BATTLER_MAX_HP(gActiveBattler)
						&& !IsHealBlocked(gActiveBattler))
						{
							gBattleMoveDamage = -1 * MathMax(1, GetBaseMaxHP(gActiveBattler) / 16);
							BattleScriptExecute(BattleScript_GrassyTerrainHeal);
							effect++;
						}
						break;

					case ET_Item_Effects:
						if (BATTLER_ALIVE(gActiveBattler))
						{
							if (ItemBattleEffects(ItemEffects_EndTurn, gActiveBattler, FALSE, FALSE))
								effect++;
						}
						break;

					case ET_Hydration_ShedSkin_Healer:
						if (BATTLER_ALIVE(gActiveBattler))
						{
							switch(ABILITY(gActiveBattler)) {
								case ABILITY_SHEDSKIN:
								case ABILITY_HYDRATION:
								case ABILITY_HEALER:
								case ABILITY_EMERGENCYEXIT:
									if (AbilityBattleEffects(ABILITYEFFECT_ENDTURN, gActiveBattler, 0, 0, 0))
										effect++;
							}
						}
						break;
				}

				++gNewBS->endTurnBlockState;
				if (gNewBS->endTurnBlockState < MAX_CASES_BLOCK_A)
					goto SkipBankIncrement;

				gNewBS->endTurnBlockState = 0;
				break;

			case ET_Aqua_Ring:
				if (gStatuses3[gActiveBattler] & STATUS3_AQUA_RING
				&& BATTLER_ALIVE(gActiveBattler)
				&& !BATTLER_MAX_HP(gActiveBattler)
				&& !IsHealBlocked(gActiveBattler))
				{
					gBattleMoveDamage = GetBaseMaxHP(gActiveBattler) / 16;
					if (ITEM_EFFECT(gActiveBattler) == ITEM_EFFECT_BIG_ROOT)
						gBattleMoveDamage = (gBattleMoveDamage * 130) / 100;
					if (gBattleMoveDamage == 0)
						gBattleMoveDamage = 1;

					gBattleMoveDamage *= -1;
					BattleScriptExecute(BattleScript_AquaRing);
					effect++;
				}
				break;

			case ET_Ingrain:
				if (gStatuses3[gActiveBattler] & STATUS3_ROOTED
				&& BATTLER_ALIVE(gActiveBattler)
				&& !BATTLER_MAX_HP(gActiveBattler)
				&& !IsHealBlocked(gActiveBattler))
				{
					gBattleMoveDamage = GetBaseMaxHP(gActiveBattler) / 16;
					if (ITEM_EFFECT(gActiveBattler) == ITEM_EFFECT_BIG_ROOT)
						gBattleMoveDamage = (gBattleMoveDamage * 13) / 10;
					if (gBattleMoveDamage == 0)
						gBattleMoveDamage = 1;

					gBattleMoveDamage *= -1;
					BattleScriptExecute(BattleScript_IngrainTurnHeal);
					effect++;
				}
				break;

			case ET_Leech_Seed:
				if (gStatuses3[gActiveBattler] & STATUS3_LEECHSEED
				&& BATTLER_ALIVE(gActiveBattler)
				&& ABILITY(gActiveBattler) != ABILITY_MAGICGUARD
				&& gBattleMons[gStatuses3[gActiveBattler] & STATUS3_LEECHSEED_BATTLER].hp != 0)
				{
					gBankTarget = gStatuses3[gActiveBattler] & STATUS3_LEECHSEED_BATTLER; //funny how the 'target' is actually the bank that receives HP
					gBattleMoveDamage = GetLeechSeedDamage(gActiveBattler);
					gBattleScripting.animArg1 = gBankTarget;
					gBattleScripting.animArg2 = gBankAttacker;
					BattleScriptExecute(BattleScript_LeechSeedTurnDrain);
					effect++;
				}
				gNewBS->turnDamageTaken[gActiveBattler] = gBattleMoveDamage; //For Emergency Exit
				break;

			//One item effect checked is done after each time the Pokemon receives damage
			case ET_Item_Effects2:
			case ET_Item_Effects3:
			case ET_Item_Effects4:
			case ET_Item_Effects5:
			case ET_Item_Effects6:
			case ET_Item_Effects7:
			case ET_Item_Effects8:
			case ET_Item_Effects9:
			case ET_Item_Effects10:
				if (BATTLER_ALIVE(gActiveBattler))
				{
					if (ItemBattleEffects(ItemEffects_EndTurn, gActiveBattler, FALSE, FALSE))
						effect++;
				}
				break;
			
			//One ability checked is done after each time the Pokemon receives damage
			case ET_Switch_Out_Abilities2:
			case ET_Switch_Out_Abilities3:
			case ET_Switch_Out_Abilities4:
			case ET_Switch_Out_Abilities5:
			case ET_Switch_Out_Abilities6:
			case ET_Switch_Out_Abilities7:
			case ET_Switch_Out_Abilities8:
			case ET_Switch_Out_Abilities9:
				if (BATTLER_ALIVE(gActiveBattler))
				{
					switch(ABILITY(gActiveBattler)) {
						case ABILITY_EMERGENCYEXIT:
							if (AbilityBattleEffects(ABILITYEFFECT_ENDTURN, gActiveBattler, 0, 0, 0))
								effect++;
					}
				}
				break;

			case ET_Poison:
				if (BATTLER_ALIVE(gActiveBattler)
				&& ABILITY(gActiveBattler) != ABILITY_MAGICGUARD)
				{
					if (gBattleMons[gActiveBattler].status1 & (STATUS_POISON | STATUS_TOXIC_POISON))
					{
						if (gBattleMons[gActiveBattler].status1 & STATUS_TOXIC_POISON
						&& (gBattleMons[gActiveBattler].status1 & STATUS1_TOXIC_COUNTER) != STATUS1_TOXIC_TURN(15)) //not 16 turns
							gBattleMons[gActiveBattler].status1 += STATUS1_TOXIC_TURN(1);

						gBattleMoveDamage = GetPoisonDamage(gActiveBattler, FALSE);

						if (ABILITY(gActiveBattler) == ABILITY_POISONHEAL)
						{
							if (!BATTLER_MAX_HP(gActiveBattler) && !IsHealBlocked(gActiveBattler))
							{
								gBattleMoveDamage *= -1;
								gBattleScripting.bank = gActiveBattler;
								BattleScriptExecute(BattleScript_PoisonHeal);
							}
						}
						else
						{
							BattleScriptExecute(BattleScript_PoisonTurnDmg);
						}
						++effect;
					}
				}
				gNewBS->turnDamageTaken[gActiveBattler] = gBattleMoveDamage; //For Emergency Exit
				break;

			case ET_Burn:
				if (gBattleMons[gActiveBattler].status1 & STATUS_BURN
				&& BATTLER_ALIVE(gActiveBattler)
				&& ABILITY(gActiveBattler) != ABILITY_MAGICGUARD)
				{
					gBattleMoveDamage = GetBurnDamage(gActiveBattler);
					BattleScriptExecute(BattleScript_BurnTurnDmg);
					effect++;
				}
				gNewBS->turnDamageTaken[gActiveBattler] = gBattleMoveDamage; //For Emergency Exit
				break;

			case ET_Frostbite:
				#ifdef FROSTBITE
				if (gBattleMons[gActiveBattler].status1 & STATUS_FREEZE
				&& BATTLER_ALIVE(gActiveBattler)
				&& ABILITY(gActiveBattler) != ABILITY_MAGICGUARD)
				{
					gBattleMoveDamage = GetFrostbiteDamage(gActiveBattler);
					BattleScriptExecute(BattleScript_FrostbiteTurnDmg);
					effect++;
				}
				#endif
				gNewBS->turnDamageTaken[gActiveBattler] = gBattleMoveDamage; //For Emergency Exit
				break;
				

			case ET_Nightmare:
				if (gBattleMons[gActiveBattler].status2 & STATUS2_NIGHTMARE)
				{
					if (gBattleMons[gActiveBattler].status1 & STATUS_SLEEP
					||  ABILITY(gActiveBattler) == ABILITY_COMATOSE)
					{
						if (BATTLER_ALIVE(gActiveBattler)
						&&  ABILITY(gActiveBattler) != ABILITY_MAGICGUARD)
						{
							gBattleMoveDamage = GetNightmareDamage(gActiveBattler);
							BattleScriptExecute(BattleScript_NightmareTurnDmg);
							effect++;
						}
					}
					else
						gBattleMons[gActiveBattler].status2 &= ~STATUS2_NIGHTMARE;
				}
				gNewBS->turnDamageTaken[gActiveBattler] = gBattleMoveDamage; //For Emergency Exit
				break;

			case ET_BadThoughts:
				if (BATTLER_ALIVE(gActiveBattler) && IsBadThoughtsBattle())
				{
					gBattleMoveDamage = GetBadThoughtsDamage(gActiveBattler);
					if (gBattleMoveDamage != 0)
					{
						BattleScriptExecute(BattleScript_BadThoughtsTurnDmg);
						effect++;
					}
				}
				gNewBS->turnDamageTaken[gActiveBattler] = gBattleMoveDamage; //For Emergency Exit
				break;

			case ET_Curse:
				if (gBattleMons[gActiveBattler].status2 & STATUS2_CURSED
				&&  BATTLER_ALIVE(gActiveBattler)
				&&  ABILITY(gActiveBattler) != ABILITY_MAGICGUARD)
				{
					gBattleMoveDamage = GetCurseDamage(gActiveBattler);
					BattleScriptExecute(BattleScript_CurseTurnDmg);
					effect++;
				}
				gNewBS->turnDamageTaken[gActiveBattler] = gBattleMoveDamage; //For Emergency Exit
				break;

			case ET_Trap_Damage:
				if (gNewBS->brokeFreeMessage & gBitTable[gActiveBattler]
				&& BATTLER_ALIVE(gActiveBattler))
				{
					//Broke free
					gBattleMons[gActiveBattler].status2 &= ~(STATUS2_WRAPPED);
					gNewBS->brokeFreeMessage &= ~(gBitTable[gActiveBattler]);
					gNewBS->sandblastCentiferno[gActiveBattler] = 0;
					gNewBS->SaltcureTimers[gActiveBattler] = 0;

					gBattleTextBuff1[0] = B_BUFF_PLACEHOLDER_BEGIN;
					gBattleTextBuff1[1] = B_TXT_COPY_VAR_1;
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
					bool8 setBrokeFreeMessage = FALSE;
					gBattleMons[gActiveBattler].status2 -= 0x2000;

					if (!(gBattleMons[gActiveBattler].status2 & STATUS2_WRAPPED))
					{
						gBattleMons[gActiveBattler].status2 |= STATUS2_WRAPPED; //Reactivate temporarily
						setBrokeFreeMessage = TRUE; //Will play next turn
					}

					if (ABILITY(gActiveBattler) != ABILITY_MAGICGUARD)
					{
						//Damaged by wrap
						gBattleScripting.animArg1 = gBattleStruct->wrappedMove[gActiveBattler];
						gBattleScripting.animArg2 = gBattleStruct->wrappedMove[gActiveBattler] >> 8;
						gBattleTextBuff1[0] = B_BUFF_PLACEHOLDER_BEGIN;
						gBattleTextBuff1[1] = B_TXT_COPY_VAR_1;
						gBattleTextBuff1[2] = gBattleStruct->wrappedMove[gActiveBattler];
						gBattleTextBuff1[3] = gBattleStruct->wrappedMove[gActiveBattler] >> 8;
						gBattleTextBuff1[4] = EOS;

						gBattlescriptCurrInstr = BattleScript_WrapTurnDmg;
						gBattleMoveDamage = GetTrapDamage(gActiveBattler);

						BattleScriptExecute(gBattlescriptCurrInstr);
						effect++;
					}
	
					//Must be placed afterwards so the trap damage still has an effect on the last turn
					if (setBrokeFreeMessage)
						gNewBS->brokeFreeMessage |= gBitTable[gActiveBattler]; //Will play next turn
				}
				gNewBS->turnDamageTaken[gActiveBattler] = gBattleMoveDamage; //For Emergency Exit
				break;

			case ET_Splinters:
				if (BATTLER_ALIVE(gActiveBattler) && gNewBS->splinterTimer[gActiveBattler] > 0)
				{
					if (--gNewBS->splinterTimer[gActiveBattler] == 0)
					{
						gBattleStringLoader = gText_SplintersEnded;
						BattleScriptExecute(BattleScript_PrintCustomStringEnd2);
						effect++;
					}
					else
					{
						gBattleMoveDamage = GetSplintersDamage(gActiveBattler);
						if (gBattleMoveDamage != 0)
						{
							BattleScriptExecute(BattleScript_SplintersTurnDmg);
							effect++;
						}
					}
				}
				gNewBS->turnDamageTaken[gActiveBattler] = gBattleMoveDamage; //For Emergency Exit
				break;

			case ET_Octolock:
				if (gBattleMons[gActiveBattler].status2 & STATUS2_ESCAPE_PREVENTION
				&& BATTLER_ALIVE(gActiveBattler) && gNewBS->trappedByOctolock)
				{
					gBattleScripting.animArg1 = MOVE_OCTOLOCK & 0xFF;
					gBattleScripting.animArg2 = MOVE_OCTOLOCK >> 8;
					BattleScriptExecute(BattleScript_OctolockTurnDmg);
					effect++;
				}
				break;

			case ET_Taunt_Timer:
				if (gDisableStructs[gActiveBattler].tauntTimer > 0 && --gDisableStructs[gActiveBattler].tauntTimer == 0)
				{
					gBattleScripting.bank = gActiveBattler;
					gBattleStringLoader = gText_TauntWoreOff;
					BattleScriptExecute(BattleScript_PrintCustomStringEnd2);
					effect++;
				}
				break;

			case ET_Encore_Timer:
				if (gDisableStructs[gActiveBattler].encoreTimer && BATTLER_ALIVE(gActiveBattler))
				{
					if (gBattleMons[gActiveBattler].moves[gDisableStructs[gActiveBattler].encoredMovePos] != gDisableStructs[gActiveBattler].encoredMove)
					{  // pokemon does not have the encored move anymore
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

			case ET_Disable_Timer:
				if (gDisableStructs[gActiveBattler].disableTimer1 && gBattleMons[gActiveBattler].hp)
				{
					int movenum;

					for (movenum = 0; movenum < 4; movenum++)
					{
						if (gDisableStructs[gActiveBattler].disabledMove == gBattleMons[gActiveBattler].moves[movenum])
							break;
					}

					if (movenum == 4)
					{ // pokemon does not have the disabled move anymore
						gDisableStructs[gActiveBattler].disabledMove = 0;
						gDisableStructs[gActiveBattler].disableTimer1 = 0;
					}
					else if (--gDisableStructs[gActiveBattler].disableTimer1 == 0)
					{  // disable ends
						gDisableStructs[gActiveBattler].disabledMove = 0;
						BattleScriptExecute(BattleScript_DisabledNoMore);
						effect++;
					}
				}
				break;

			case ET_Lock_On_Timer:
				if (gStatuses3[gActiveBattler] & STATUS3_LOCKON)
					gStatuses3[gActiveBattler] -= 0x8;
				break;

			case ET_Charge_Timer:
				if (gDisableStructs[gActiveBattler].chargeTimer && --gDisableStructs[gActiveBattler].chargeTimer == 0)
					gStatuses3[gActiveBattler] &= ~STATUS3_CHARGED_UP;
				break;

			case ET_Magnet_Rise_Timer:
				if (gNewBS->MagnetRiseTimers[gActiveBattler] > 0
				&& BATTLER_ALIVE(gActiveBattler)
				&& --gNewBS->MagnetRiseTimers[gActiveBattler] == 0)
				{
					BattleScriptExecute(BattleScript_MagnetRiseEnd);
					effect++;
				}
				break;

			case ET_Telekinesis_Timer:
				if (gNewBS->TelekinesisTimers[gActiveBattler] > 0
				&& BATTLER_ALIVE(gActiveBattler)
				&& --gNewBS->TelekinesisTimers[gActiveBattler] == 0)
				{
					BattleScriptExecute(BattleScript_TelekinesisEnd);
					effect++;
				}
				break;

			case ET_Heal_Block_Timer:
				if (gNewBS->HealBlockTimers[gActiveBattler] > 0
				&& BATTLER_ALIVE(gActiveBattler)
				&& --gNewBS->HealBlockTimers[gActiveBattler] == 0)
				{
					BattleScriptExecute(BattleScript_HealBlockEnd);
					effect++;
				}
				break;

			case ET_Embargo_Timer:
				if (gNewBS->EmbargoTimers[gActiveBattler] > 0
				&& BATTLER_ALIVE(gActiveBattler)
				&& --gNewBS->EmbargoTimers[gActiveBattler] == 0)
				{
					BattleScriptExecute(BattleScript_EmbargoEnd);
					effect++;
				}
				break;

			case ET_Yawn:
				if (gStatuses3[gActiveBattler] & STATUS3_YAWN && BATTLER_ALIVE(gActiveBattler))
				{
					gStatuses3[gActiveBattler] -= 0x800;

					if (!(gStatuses3[gActiveBattler] & STATUS3_YAWN))
					{
						if (CanBePutToSleep(gActiveBattler, gActiveBattler, FALSE)) //Falling asleep now
						{
							if (!BATTLER_SEMI_INVULNERABLE(gActiveBattler)) //Semi-Invulnerability is removed when it tries to attack
								CancelMultiTurnMoves(gActiveBattler);
							gBattleMons[gActiveBattler].status1 |= (Random() % 3) + 2;
							EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 4, &gBattleMons[gActiveBattler].status1);
							MarkBufferBankForExecution(gActiveBattler);
							gEffectBank = gActiveBattler;
							BattleScriptExecute(BattleScript_YawnMakesAsleep);
							effect++;
						}
						else if (CheckGrounding(gActiveBattler) && gTerrainType == MISTY_TERRAIN)
						{
							gBattleStringLoader = gText_TargetWrappedInMistyTerrain;
							BattleScriptExecute(BattleScript_PrintCustomStringEnd2);
							effect++;
						}
						else if (CheckGrounding(gActiveBattler) && gTerrainType == ELECTRIC_TERRAIN)
						{
							gBattleStringLoader = gText_TargetWrappedInElectricTerrain;
							BattleScriptExecute(BattleScript_PrintCustomStringEnd2);
							effect++;
						}
					}
				}
				break;

			case ET_Perish_Song:
				if (gStatuses3[gActiveBattler] & STATUS3_PERISH_SONG && BATTLER_ALIVE(gActiveBattler))
				{
					gBattleTextBuff1[0] = B_BUFF_PLACEHOLDER_BEGIN;
					gBattleTextBuff1[1] = B_TXT_BUFF2;
					gBattleTextBuff1[2] = 1;
					gBattleTextBuff1[3] = 1;
					gBattleTextBuff1[4] = gDisableStructs[gActiveBattler].perishSongTimer;
					gBattleTextBuff1[5] = 0xFF;

					if (gDisableStructs[gActiveBattler].perishSongTimer == 0)
					{
						gStatuses3[gActiveBattler] &= ~(STATUS3_PERISH_SONG);
						gBattleMoveDamage = gBattleMons[gActiveBattler].hp;
						gBattlescriptCurrInstr = BattleScript_PerishSongHits;
					}
					else
					{
						gDisableStructs[gActiveBattler].perishSongTimer--;
						gBattlescriptCurrInstr = BattleScript_PerishSongTimerGoesDown;
					}

					BattleScriptExecute(gBattlescriptCurrInstr);
					effect++;
				}
				break;

			case ET_Roost:
				if (gNewBS->RoostCounter & gBitTable[gActiveBattler])
				{
					gNewBS->RoostCounter &= ~(gBitTable[gActiveBattler]);
					if (gBattleMons[gActiveBattler].type1 == TYPE_NORMAL)
						gBattleMons[gActiveBattler].type1 = TYPE_FLYING;
					if (gBattleMons[gActiveBattler].type2 == TYPE_NORMAL)
						gBattleMons[gActiveBattler].type2 = TYPE_FLYING;
					if (gBattleMons[gActiveBattler].type3 == TYPE_ROOSTLESS)
						gBattleMons[gActiveBattler].type3 = TYPE_FLYING;
				}
				else
				{
					if (gBattleMons[gActiveBattler].type1 == TYPE_ROOSTLESS)
						gBattleMons[gActiveBattler].type1 = TYPE_FLYING;
					if (gBattleMons[gActiveBattler].type2 == TYPE_ROOSTLESS)
						gBattleMons[gActiveBattler].type2 = TYPE_FLYING;
					if (gBattleMons[gActiveBattler].type3 == TYPE_ROOSTLESS)
						gBattleMons[gActiveBattler].type3 = TYPE_FLYING;
				}
				break;

//These Timers all fall through to eachother for quicker game speed
			case ET_Reflect:
				if (gBattleStruct->turnEffectsBank < NUM_BATTLE_SIDES)
				{
					sideBank = gBattleStruct->turnEffectsBank;
					if (gSideStatuses[sideBank] & SIDE_STATUS_REFLECT && --gSideTimers[sideBank].reflectTimer == 0)
					{
						gBankAttacker = gBankTarget = gActiveBattler = sideBank;
						gSideStatuses[sideBank] &= ~SIDE_STATUS_REFLECT;
						gBattleStringLoader = gText_SideStatusWoreOff;
						BattleScriptExecute(BattleScript_PrintCustomStringEnd2);
						gBattleTextBuff1[0] = B_BUFF_PLACEHOLDER_BEGIN;
						gBattleTextBuff1[1] = B_TXT_COPY_VAR_1;
						gBattleTextBuff1[2] = MOVE_REFLECT;
						gBattleTextBuff1[3] = MOVE_REFLECT >> 8;
						gBattleTextBuff1[4] = EOS;
						effect++;
					}
					break;
				}

				++gBattleStruct->turnEffectsTracker;
				gBattleStruct->turnEffectsBank = 0;

				__attribute__ ((fallthrough));
			case ET_Light_Screen:
				if (gBattleStruct->turnEffectsBank < NUM_BATTLE_SIDES)
				{
					sideBank = gBattleStruct->turnEffectsBank;
					if (gSideStatuses[sideBank] & SIDE_STATUS_LIGHTSCREEN && --gSideTimers[sideBank].lightscreenTimer == 0)
					{
						gBankAttacker = gBankTarget = gActiveBattler = sideBank;
						gSideStatuses[sideBank] &= ~SIDE_STATUS_LIGHTSCREEN;
						gBattleStringLoader = gText_SideStatusWoreOff;
						BattleScriptExecute(BattleScript_PrintCustomStringEnd2);
						gBattleTextBuff1[0] = B_BUFF_PLACEHOLDER_BEGIN;
						gBattleTextBuff1[1] = B_TXT_COPY_VAR_1;
						gBattleTextBuff1[2] = MOVE_LIGHTSCREEN;
						gBattleTextBuff1[3] = MOVE_LIGHTSCREEN >> 8;
						gBattleTextBuff1[4] = EOS;
						effect++;
					}
					break;
				}

				++gBattleStruct->turnEffectsTracker;
				gBattleStruct->turnEffectsBank = 0;
				
				__attribute__ ((fallthrough));
			case ET_Safeguard:
				if (gBattleStruct->turnEffectsBank < NUM_BATTLE_SIDES)
				{
					sideBank = gBattleStruct->turnEffectsBank;
					if (gSideStatuses[sideBank] & SIDE_STATUS_SAFEGUARD && --gSideTimers[sideBank].safeguardTimer == 0)
					{
						gBankAttacker = gBankTarget = gActiveBattler = sideBank;
						gSideStatuses[sideBank] &= ~SIDE_STATUS_SAFEGUARD;
						BattleScriptExecute(BattleScript_SafeguardEnds);
						effect++;
					}
					break;
				}

				++gBattleStruct->turnEffectsTracker;
				gBattleStruct->turnEffectsBank = 0;

				__attribute__ ((fallthrough));
			case ET_Mist:
				if (gBattleStruct->turnEffectsBank < NUM_BATTLE_SIDES)
				{
					sideBank = gBattleStruct->turnEffectsBank;
					if (gSideTimers[sideBank].mistTimer > 0 && --gSideTimers[sideBank].mistTimer == 0) //Doesn't compare side status because Guard Spec. doesn't set it!
					{
						gBankAttacker = gBankTarget = gActiveBattler = sideBank;
						gSideStatuses[sideBank] &= ~SIDE_STATUS_MIST;
						gBattleStringLoader = gText_SideStatusWoreOff;
						BattleScriptExecute(BattleScript_PrintCustomStringEnd2);
						gBattleTextBuff1[0] = B_BUFF_PLACEHOLDER_BEGIN;
						gBattleTextBuff1[1] = B_TXT_COPY_VAR_1;
						gBattleTextBuff1[2] = MOVE_MIST;
						gBattleTextBuff1[3] = MOVE_MIST >> 8;
						gBattleTextBuff1[4] = EOS;
						effect++;
					}
					break;
				}

				++gBattleStruct->turnEffectsTracker;
				gBattleStruct->turnEffectsBank = 0;

				__attribute__ ((fallthrough));
			case ET_Tailwind_Timer:
				if (gBattleStruct->turnEffectsBank < NUM_BATTLE_SIDES)
				{
					if (gNewBS->TailwindTimers[gBattleStruct->turnEffectsBank]
					&& --gNewBS->TailwindTimers[gBattleStruct->turnEffectsBank] == 0)
					{
						gBankAttacker = gBankTarget = gActiveBattler = gBattleStruct->turnEffectsBank;
						BattleScriptExecute(BattleScript_TailwindEnd);
						effect++;
					}
					break;
				}

				++gBattleStruct->turnEffectsTracker;
				gBattleStruct->turnEffectsBank = 0;

				__attribute__ ((fallthrough));
			case ET_Lucky_Chant_Timer:
				if (gBattleStruct->turnEffectsBank < NUM_BATTLE_SIDES)
				{
					if (gNewBS->LuckyChantTimers[gBattleStruct->turnEffectsBank]
					&& --gNewBS->LuckyChantTimers[gBattleStruct->turnEffectsBank] == 0)
					{
						gBankAttacker = gBankTarget = gActiveBattler = gBattleStruct->turnEffectsBank;
						BattleScriptExecute(BattleScript_LuckyChantEnd);
						effect++;
					}
					break;
				}

				++gBattleStruct->turnEffectsTracker;
				gBattleStruct->turnEffectsBank = 0;

				__attribute__ ((fallthrough));
			case ET_Rainbow_Timer:
				if (gBattleStruct->turnEffectsBank < NUM_BATTLE_SIDES)
				{
					if (gNewBS->RainbowTimers[gBattleStruct->turnEffectsBank] > 0
					&& --gNewBS->RainbowTimers[gBattleStruct->turnEffectsBank] == 0)
					{
						gBankAttacker = gBankTarget = gActiveBattler = gBattleStruct->turnEffectsBank;
						BattleScriptExecute(BattleScript_RainbowEnd);
						effect++;
					}
					break;
				}

				++gBattleStruct->turnEffectsTracker;
				gBattleStruct->turnEffectsBank = 0;

				__attribute__ ((fallthrough));
			case ET_Sea_Of_Fire_Timer:
				if (gBattleStruct->turnEffectsBank < NUM_BATTLE_SIDES)
				{
					if (gNewBS->SeaOfFireTimers[gBattleStruct->turnEffectsBank] > 0
					&& --gNewBS->SeaOfFireTimers[gBattleStruct->turnEffectsBank] == 0)
					{
						gBankAttacker = gBankTarget = gActiveBattler = gBattleStruct->turnEffectsBank;
						BattleScriptExecute(BattleScript_SeaOfFireEnd);
						effect++;
					}
					break;
				}

				++gBattleStruct->turnEffectsTracker;
				gBattleStruct->turnEffectsBank = 0;

				__attribute__ ((fallthrough));
			case ET_Swamp_Timer:
				if (gBattleStruct->turnEffectsBank < NUM_BATTLE_SIDES)
				{
					if (gNewBS->SwampTimers[gBattleStruct->turnEffectsBank] > 0
					&& --gNewBS->SwampTimers[gBattleStruct->turnEffectsBank] == 0)
					{
						gBankAttacker = gBankTarget = gActiveBattler = gBattleStruct->turnEffectsBank;
						BattleScriptExecute(BattleScript_SwampEnd);
						effect++;
					}
					break;
				}

				++gBattleStruct->turnEffectsTracker;
				gBattleStruct->turnEffectsBank = 0;

				__attribute__ ((fallthrough));
			case ET_G_Max_VineLash_Timer:
				if (gBattleStruct->turnEffectsBank < NUM_BATTLE_SIDES)
				{
					if (gNewBS->maxVineLashTimers[gBattleStruct->turnEffectsBank] > 0
					&& --gNewBS->maxVineLashTimers[gBattleStruct->turnEffectsBank] == 0)
					{
						gBankAttacker = gBankTarget = gActiveBattler = gBattleStruct->turnEffectsBank;
						gBattleStringLoader = gText_GMaxVineLashEnded;
						BattleScriptExecute(BattleScript_PrintCustomStringEnd2);
						effect++;
					}
					break;
				}

				++gBattleStruct->turnEffectsTracker;
				gBattleStruct->turnEffectsBank = 0;

				__attribute__ ((fallthrough));
			case ET_G_Max_Wildfire_Timer:
				if (gBattleStruct->turnEffectsBank < NUM_BATTLE_SIDES)
				{
					if (gNewBS->maxWildfireTimers[gBattleStruct->turnEffectsBank] > 0
					&& --gNewBS->maxWildfireTimers[gBattleStruct->turnEffectsBank] == 0)
					{
						gBankAttacker = gBankTarget = gActiveBattler = gBattleStruct->turnEffectsBank;
						gBattleStringLoader = gText_GMaxWildfireEnded;
						BattleScriptExecute(BattleScript_PrintCustomStringEnd2);
						effect++;
					}
					break;
				}

				++gBattleStruct->turnEffectsTracker;
				gBattleStruct->turnEffectsBank = 0;

				__attribute__ ((fallthrough));
			case ET_G_Max_Cannonade_Timer:
				if (gBattleStruct->turnEffectsBank < NUM_BATTLE_SIDES)
				{
					if (gNewBS->maxCannonadeTimers[gBattleStruct->turnEffectsBank] > 0
					&& --gNewBS->maxCannonadeTimers[gBattleStruct->turnEffectsBank] == 0)
					{
						gBankAttacker = gBankTarget = gActiveBattler = gBattleStruct->turnEffectsBank;
						gBattleStringLoader = gText_GMaxCannonadeEnded;
						BattleScriptExecute(BattleScript_PrintCustomStringEnd2);
						effect++;
					}
					break;
				}

				++gBattleStruct->turnEffectsTracker;
				gBattleStruct->turnEffectsBank = 0;

				__attribute__ ((fallthrough));
			case ET_G_Max_Volcalith_Timer:
				if (gBattleStruct->turnEffectsBank < NUM_BATTLE_SIDES)
				{
					if (gNewBS->maxVolcalithTimers[gBattleStruct->turnEffectsBank] > 0
					&& --gNewBS->maxVolcalithTimers[gBattleStruct->turnEffectsBank] == 0)
					{
						gBankAttacker = gBankTarget = gActiveBattler = gBattleStruct->turnEffectsBank;
						gBattleStringLoader = gText_GMaxVolcalithEnded;
						BattleScriptExecute(BattleScript_PrintCustomStringEnd2);
						effect++;
					}
					break;
				}

				++gBattleStruct->turnEffectsTracker;
				gBattleStruct->turnEffectsBank = 0;

				__attribute__ ((fallthrough));
			case ET_Aurora_Veil:
				if (gBattleStruct->turnEffectsBank < NUM_BATTLE_SIDES)
				{
					sideBank = gBattleStruct->turnEffectsBank;
					if (gNewBS->AuroraVeilTimers[sideBank] > 0 && --gNewBS->AuroraVeilTimers[sideBank] == 0)
					{
						gBankAttacker = gBankTarget = gActiveBattler = gBattleStruct->turnEffectsBank;
						gBattleStringLoader = gText_SideStatusWoreOff;
						BattleScriptExecute(BattleScript_PrintCustomStringEnd2);
						gBattleTextBuff1[0] = B_BUFF_PLACEHOLDER_BEGIN;
						gBattleTextBuff1[1] = B_TXT_COPY_VAR_1;
						gBattleTextBuff1[2] = MOVE_AURORAVEIL & 0xFF;
						gBattleTextBuff1[3] = MOVE_AURORAVEIL >> 8;
						gBattleTextBuff1[4] = EOS;
						effect++;
					}
					break;
				}

				++gBattleStruct->turnEffectsTracker;
				gBattleStruct->turnEffectsBank = 0;

				__attribute__ ((fallthrough));
			case ET_Trick_Room_Timer:
				++gBattleStruct->turnEffectsTracker;
				gBattleStruct->turnEffectsBank = 0;
				if (gNewBS->TrickRoomTimer > 0 && --gNewBS->TrickRoomTimer == 0)
				{
					BattleScriptExecute(BattleScript_TrickRoomEnd);
					return ++effect;
				}
				__attribute__ ((fallthrough));
			case ET_Water_Sport_Timer:
				++gBattleStruct->turnEffectsTracker;
				gBattleStruct->turnEffectsBank = 0;
				if (gNewBS->WaterSportTimer > 0 && --gNewBS->WaterSportTimer == 0)
				{
					BattleScriptExecute(BattleScript_WaterSportEnd);
					return ++effect;
				}
				__attribute__ ((fallthrough));
			case ET_Mud_Sport_Timer:
				++gBattleStruct->turnEffectsTracker;
				gBattleStruct->turnEffectsBank = 0;
				if (gNewBS->MudSportTimer > 0 && --gNewBS->MudSportTimer == 0)
				{
					BattleScriptExecute(BattleScript_MudSportEnd);
					return ++effect;
				}
				__attribute__ ((fallthrough));
			case ET_Wonder_Room_Timer:
				++gBattleStruct->turnEffectsTracker;
				gBattleStruct->turnEffectsBank = 0;
				if (gNewBS->WonderRoomTimer > 0 && --gNewBS->WonderRoomTimer == 0)
				{
					BattleScriptExecute(BattleScript_WonderRoomEnd);
					return ++effect;
				}
				__attribute__ ((fallthrough));
			case ET_Magic_Room_Timer:
				++gBattleStruct->turnEffectsTracker;
				gBattleStruct->turnEffectsBank = 0;
				if (gNewBS->MagicRoomTimer > 0 && --gNewBS->MagicRoomTimer == 0)
				{
					BattleScriptExecute(BattleScript_MagicRoomEnd);
					return ++effect;
				}
				__attribute__ ((fallthrough));
			case ET_Gravity_Timer:
				++gBattleStruct->turnEffectsTracker;
				gBattleStruct->turnEffectsBank = 0;
				if (gNewBS->GravityTimer > 0 && --gNewBS->GravityTimer == 0)
				{
					BattleScriptExecute(BattleScript_GravityEnd);
					return ++effect;
				}
				__attribute__ ((fallthrough));
			case ET_Seeds: ;
				if (ITEM_EFFECT(gActiveBattler) == ITEM_EFFECT_SEEDS)
				{
					if (ItemBattleEffects(ItemEffects_SwitchIn, gActiveBattler, FALSE, FALSE))
						++effect;
				}
				break;

			case ET_Terrain_Timer:
				gNewBS->endTurnBlockState = 0; //For later
				++gBattleStruct->turnEffectsTracker;
				gBattleStruct->turnEffectsBank = 0;
				if (gNewBS->TerrainTimer && --gNewBS->TerrainTimer == 0)
				{
					BattleScriptExecute(BattleScript_TerrainEnd);
					return ++effect;
				}
				__attribute__ ((fallthrough));
			case ET_Block_B:
				switch(gNewBS->endTurnBlockState) {
					case ET_Uproar:
						if (gBattleMons[gActiveBattler].status2 & STATUS2_UPROAR
						&& BATTLER_ALIVE(gActiveBattler))
						{
							gBattleMons[gActiveBattler].status2 -= 0x10;  // uproar timer goes down
							if (WasUnableToUseMove(gActiveBattler))
							{
								CancelMultiTurnMoves(gActiveBattler);
								gBattleCommunication[MULTISTRING_CHOOSER] = 1;
							}
							else if (gBattleMons[gActiveBattler].status2 & STATUS2_UPROAR)
							{
								gBattleCommunication[MULTISTRING_CHOOSER] = 0;
								gBattleMons[gActiveBattler].status2 |= STATUS2_MULTIPLETURNS;
							}
							else
							{
								gBattleCommunication[MULTISTRING_CHOOSER] = 1;
								CancelMultiTurnMoves(gActiveBattler);
							}

							BattleScriptExecute(BattleScript_PrintUproarOverTurns);
							++effect;
						}
						break;

					case ET_SpeedBoost_Moody_BadDreams_SlowStart:
						if (BATTLER_ALIVE(gActiveBattler))
						{
							gLastUsedAbility = ABILITY(gActiveBattler);
							switch(gLastUsedAbility) {
								case ABILITY_SPEEDBOOST:
								case ABILITY_TRUANT:
								case ABILITY_MOODY:
								case ABILITY_BADDREAMS:
								case ABILITY_SLOWSTART:
									if (AbilityBattleEffects(ABILITYEFFECT_ENDTURN, gActiveBattler, 0, 0, 0))
										effect++;
									break;
							}
						}
						break;

					case ET_Orbz:
						if (BATTLER_ALIVE(gActiveBattler))
						{
							u8 itemEffect = ITEM_EFFECT(gActiveBattler);

							switch (itemEffect) {
								case ITEM_EFFECT_TOXIC_ORB:
									if (CanBePoisoned(gActiveBattler, gActiveBattler, FALSE))
									{
										gLastUsedItem = ITEM(gActiveBattler);
										RecordItemEffectBattle(gActiveBattler, itemEffect);
										gBattleMons[gActiveBattler].status1 |= STATUS1_TOXIC_POISON;
										EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 4, &gBattleMons[gActiveBattler].status1);
										MarkBufferBankForExecution(gActiveBattler);

										gEffectBank = gActiveBattler;
										BattleScriptExecute(BattleScript_ToxicOrb);
										++effect;
									}
									break;
								case ITEM_EFFECT_FLAME_ORB:
									if (CanBeBurned(gActiveBattler, gActiveBattler, FALSE))
									{
										gLastUsedItem = ITEM(gActiveBattler);
										RecordItemEffectBattle(gActiveBattler, itemEffect);
										gBattleMons[gActiveBattler].status1 |= STATUS1_BURN;
										EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 4, &gBattleMons[gActiveBattler].status1);
										MarkBufferBankForExecution(gActiveBattler);

										gEffectBank = gActiveBattler;
										BattleScriptExecute(BattleScript_FlameOrb);
										++effect;
									}
									break;
								case ITEM_EFFECT_STICKY_BARB:
									if (ABILITY(gActiveBattler) != ABILITY_MAGICGUARD)
									{
										gLastUsedItem = ITEM(gBattleScripting.bank = gActiveBattler);
										RecordItemEffectBattle(gActiveBattler, itemEffect);

										gBattleMoveDamage = MathMax(1, GetBaseMaxHP(gActiveBattler) / 8);
										gNewBS->turnDamageTaken[gActiveBattler] = gBattleMoveDamage; //For Emergency Exit
										BattleScriptExecute(BattleScript_BlackSludgeHurt);
										++effect;
									}
									break;
							}
						}
						break;

					case ET_Harvest_Pickup:
						if (BATTLER_ALIVE(gActiveBattler) && ITEM(gActiveBattler) == ITEM_NONE)
						{
							switch(ABILITY(gActiveBattler)) {
								case ABILITY_HARVEST:
								case ABILITY_PICKUP:
								case ABILITY_BALLFETCH:
									if (AbilityBattleEffects(ABILITYEFFECT_ENDTURN, gActiveBattler, 0, 0, 0))
										effect++;
							}
						}
						break;
				}

				++gNewBS->endTurnBlockState;
				if (gNewBS->endTurnBlockState < MAX_CASES_BLOCK_B)
					goto SkipBankIncrement;
				else
				{
					gNewBS->endTurnBlockState = 0;
					break;
				}

			case ET_Form_Change:
				if (BATTLER_ALIVE(gActiveBattler) && !IS_TRANSFORMED(gActiveBattler))
				{
					struct Pokemon* mon = GetBankPartyData(gActiveBattler);
					u16 species = GetMonData(mon, MON_DATA_SPECIES, NULL);
					u16 newSpecies = SPECIES_NONE;
					u8 ability = ABILITY(gActiveBattler);
					bool8 changedForm = FALSE;
					bool8 reloadType = FALSE;
					bool8 reloadStats = FALSE;
					bool8 reloadAbility = FALSE;
					const u8* battleScript = NULL;

					switch(ability) {
						case ABILITY_FORECAST:
							if (AbilityBattleEffects(ABILITYEFFECT_ENDTURN, gActiveBattler, 0, 0, 0))
								effect++;
							break;

						case ABILITY_ZENMODE:
							if (gBattleMons[gActiveBattler].hp <= gBattleMons[gActiveBattler].maxHP / 2)
							{
								#if (defined SPECIES_DARMANITAN && defined SPECIES_DARMANITANZEN)
								if (species == SPECIES_DARMANITAN)
								{
									newSpecies = SPECIES_DARMANITANZEN;
									changedForm = TRUE;
									reloadType = TRUE;
									reloadStats = TRUE;
									battleScript = BattleScript_TransformedEnd2;
								}
								#endif
								#if (defined SPECIES_DARMANITAN_G && defined SPECIES_DARMANITAN_G_ZEN)
								if (species == SPECIES_DARMANITAN_G)
								{
									newSpecies = SPECIES_DARMANITAN_G_ZEN;
									changedForm = TRUE;
									reloadType = TRUE;
									reloadStats = TRUE;
									battleScript = BattleScript_TransformedEnd2;
								}
								#endif
							}
							else //gBattleMons[gActiveBattler].hp > gBattleMons[gActiveBattler].maxHP / 2
							{
								//Revert if back above half health
								#if (defined SPECIES_DARMANITAN && defined SPECIES_DARMANITANZEN)
								if (species == SPECIES_DARMANITANZEN)
								{
									newSpecies = SPECIES_DARMANITAN;
									changedForm = TRUE;
									reloadType = TRUE;
									reloadStats = TRUE;
									battleScript = BattleScript_TransformedEnd2;
								}
								#endif
								#if (defined SPECIES_DARMANITAN_G && defined SPECIES_DARMANITAN_G_ZEN)
								if (species == SPECIES_DARMANITAN_G_ZEN)
								{
									newSpecies = SPECIES_DARMANITAN_G;
									changedForm = TRUE;
									reloadType = TRUE;
									reloadStats = TRUE;
									battleScript = BattleScript_TransformedEnd2;
								}
								#endif
							}
							break;

						#if (defined SPECIES_ZYGARDE && defined SPECIES_ZYGARDE_10 && defined SPECIES_ZYGARDE_COMPLETE)
						case ABILITY_POWERCONSTRUCT:
							if ((species == SPECIES_ZYGARDE || species == SPECIES_ZYGARDE_10)
							&& gBattleMons[gActiveBattler].hp <= gBattleMons[gActiveBattler].maxHP / 2)
							{
								newSpecies = SPECIES_ZYGARDE_COMPLETE;
								changedForm = TRUE;
								reloadType = TRUE;
								reloadStats = TRUE;
								battleScript = BattleScript_PowerConstruct;
							}
							break;
						#endif

						#if (defined SPECIES_WISHIWASHI && defined SPECIES_WISHIWASHI_S)
						case ABILITY_SCHOOLING:
							if (species == SPECIES_WISHIWASHI && gBattleMons[gActiveBattler].level >= 20
							&& gBattleMons[gActiveBattler].hp > gBattleMons[gActiveBattler].maxHP / 4)
							{
								newSpecies = SPECIES_WISHIWASHI_S;
								changedForm = TRUE;
								reloadStats = TRUE;
								battleScript = BattleScript_StartedSchooling;
							}
							else if (species == SPECIES_WISHIWASHI_S
								&&  (gBattleMons[gActiveBattler].level < 20
								  || gBattleMons[gActiveBattler].hp <= gBattleMons[gActiveBattler].maxHP / 4))
							{
								newSpecies = SPECIES_WISHIWASHI;
								changedForm = TRUE;
								reloadStats = TRUE;
								battleScript = BattleScript_StoppedSchooling;
							}
							break;
						#endif

						#ifdef SPECIES_MINIOR_SHIELD
						case ABILITY_SHIELDSDOWN:
							if (gBattleMons[gActiveBattler].hp <= gBattleMons[gActiveBattler].maxHP / 2
							&& species == SPECIES_MINIOR_SHIELD)
							{
								newSpecies = GetMiniorCoreSpecies(mon);
								changedForm = TRUE;
								reloadStats = TRUE;
								battleScript = BattleScript_ShieldsDownToCore;
							}
							else if (gBattleMons[gActiveBattler].hp > (gBattleMons[gActiveBattler].maxHP / 2)
							&& CheckTableForSpecies(species, gMiniorCores))
							{
								newSpecies = SPECIES_MINIOR_SHIELD;
								changedForm = TRUE;
								reloadStats = TRUE;
								battleScript = BattleScript_ShieldsDownToMeteor;
							}
							break;
						#endif

						#if (defined SPECIES_CHERRIM && defined SPECIES_CHERRIM_SUN)
						case ABILITY_FLOWERGIFT:
							switch (species) {
								case SPECIES_CHERRIM:
									if (WEATHER_HAS_EFFECT
									&& (gBattleWeather & WEATHER_SUN_ANY)
									&& AffectedBySun(gActiveBattler))
									{
										newSpecies = SPECIES_CHERRIM_SUN;
										changedForm = TRUE;
										battleScript = BattleScript_FlowerGiftEnd2;
									}
									break;
								case SPECIES_CHERRIM_SUN:
									if (!WEATHER_HAS_EFFECT
									|| !(gBattleWeather & WEATHER_SUN_ANY)
									//|| ability != ABILITY_FLOWERGIFT //Not neccessary
									|| !AffectedBySun(gActiveBattler))
									{
										newSpecies = SPECIES_CHERRIM;
										changedForm = TRUE;
										battleScript = BattleScript_FlowerGiftEnd2;
									}
									break;
							}
						#endif
							break;

						#if (defined SPECIES_MORPEKO && defined SPECIES_MORPEKO_HANGRY)
						case ABILITY_HUNGERSWITCH:
							if (species == SPECIES_MORPEKO)
							{
								newSpecies = SPECIES_MORPEKO_HANGRY;
								changedForm = TRUE;
								battleScript = BattleScript_FlowerGiftEnd2;
							}
							else if (species == SPECIES_MORPEKO_HANGRY)
							{
								newSpecies = SPECIES_MORPEKO;
								changedForm = TRUE;
								battleScript = BattleScript_FlowerGiftEnd2;
							}
						#endif
							break;
					}

					if (changedForm)
					{
						gBattleScripting.bank = gActiveBattler;
						DoFormChange(gActiveBattler, newSpecies, reloadType, reloadStats, reloadAbility);
						BattleScriptExecute(battleScript);
						++effect;
					}
				}
				break;

			case ET_Reactivate_Overworld_Weather:
				gBattleStruct->turnEffectsBank = gBattlersCount;
				if (gBattleWeather == 0 && AbilityBattleEffects(ABILITYEFFECT_ON_SWITCHIN, 0, 0, 0xFF, 0))
				{
					if (RainCanBeEvaporated() && BankOnFieldHasEvaporate())
					{
						//Prevent rain from returning
						gBattleWeather = 0;
						gWishFutureKnock.weatherDuration = 0;
						gBattlescriptCurrInstr = BattleScript_End3; //Script must still be played because the cursor was pushed
						++gBattleStruct->turnEffectsTracker; //Otherwise it's get stuck in an endless rain loop
					}

					++effect;
					return effect;
				}
				break;

			case ET_Reactivate_Overworld_Terrain:
				gBattleStruct->turnEffectsBank = gBattlersCount;
				if (/*gTerrainType == 0 && */TryActivateOWTerrain()) //Now always overwrites custom terrain
				{
					++effect;
					return effect;
				}
				break;

			case ET_SOS:
				gBattleStruct->turnEffectsBank = gBattlersCount;

				#if (defined FLAG_HOOPA_SOS_BATTLE && defined SPECIES_HOOPA_UNBOUND)
				if (IS_DOUBLE_BATTLE && !(gBattleTypeFlags & BATTLE_TYPE_TRAINER) && FlagGet(FLAG_HOOPA_SOS_BATTLE))
				{
					u8 sosBank = 0xFF;
					u8 foeBank1 = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);
					
					if (SPECIES(foeBank1) == SPECIES_HOOPA_UNBOUND)
						sosBank = PARTNER(foeBank1);
					else if (SPECIES(PARTNER(foeBank1)) == SPECIES_HOOPA_UNBOUND)
						sosBank = foeBank1;

					if (sosBank != 0xFF && BATTLER_ALIVE(foeBank1) && !BATTLER_ALIVE(sosBank)) //Hoopa's alive but partner fainted
					{
						//Restore HP of fainted ally
						gActiveBattler = sosBank;
						u8 monId = (GetBattlerPosition(sosBank) == B_POSITION_OPPONENT_LEFT) ? 0 : 1;
						struct Pokemon* mon = &gEnemyParty[monId];
						gBattleMons[gActiveBattler].hp = GetMonData(mon, MON_DATA_MAX_HP, NULL);
						EmitSetMonData(0, REQUEST_HP_BATTLE, 0, 2, &gBattleMons[gActiveBattler].hp);
						MarkBufferBankForExecution(gActiveBattler);

						//Bring it back in
						PREPARE_SPECIES_BUFFER(gBattleTextBuff1, GetMonData(mon, MON_DATA_SPECIES, NULL));
						gBattleStruct->monToSwitchIntoId[sosBank] = monId;
						gAbsentBattlerFlags &= ~(gBitTable[sosBank]);
						gBankSwitching = sosBank;
						gBattleScripting.bank = foeBank1;
						BattleScriptExecute(BattleScript_HoopaSOS);
						return ++effect;
					}
				}
				#endif
				break;

			case ET_End:
			END_TURN_SKIP:
				gBattleStruct->turnEffectsBank = gBattlersCount;
				gNewBS->endTurnDone = TRUE;
				gNewBS->megaData.state = 0;
				gNewBS->calculatedAIPredictions = FALSE;
				gNewBS->batonPassing = FALSE;
				gNewBS->NoMoreMovingThisTurn = 0; //May be set during end turn Emergency Exit
				gNewBS->ragePowdered = 0;
				gNewBS->dynamaxData.attackAgain = FALSE;
				gNewBS->dynamaxData.repeatedAttacks = 0;
				gNewBS->ai.sideSwitchedThisRound = 0;
				gNewBS->ai.goodToPivot = 0;

				if (gNewBS->IonDelugeTimer) //Cleared down here b/c necessary for future attacks
					--gNewBS->IonDelugeTimer;

				for (int i = 0; i < gBattlersCount; ++i)
				{
					gNewBS->pickupStack[i] = 0xFF;
					gNewBS->leftoverHealingDone[i] = FALSE;
					gNewBS->statRoseThisRound[i] = FALSE;
					gNewBS->statFellThisRound[i] = FALSE;
					gNewBS->turnDamageTaken[i] = 0;
					UpdateQuickClawRandomNumber(i);

					if (gNewBS->metronomeItemBonus[i] > 0)
						--gNewBS->metronomeItemBonus[i];

					if (gNewBS->dynamaxData.timer[i] > 0)
					{
						if (--gNewBS->dynamaxData.timer[i] == 0)
							gNewBS->dynamaxData.timer[i] = -1; //Indicator to revert
					}
				}

				Memset(gNewBS->ai.pivotTo, PARTY_SIZE, sizeof(gNewBS->ai.pivotTo));
				Memset(gBattleStruct->switchoutIndex, PARTY_SIZE, sizeof(gBattleStruct->switchoutIndex)); //Fixes bug where it's not cleared for some reason
				ClearMonCachedDamageCalcs();
				ClearCachedAIData();
		}
		gBattleStruct->turnEffectsBank++;

	SkipBankIncrement:
		if (gBattleStruct->turnEffectsBank >= gBattlersCount)
		{
			gBattleStruct->turnEffectsTracker++;
			gBattleStruct->turnEffectsBank = 0;
		}

		if (effect)
			return effect;
	}

	gHitMarker &= ~(HITMARKER_GRUDGE | HITMARKER_x20);
	return 0;
}

u32 GetLeechSeedDamage(u8 bank)
{
	u32 damage = 0;

	if (gStatuses3[bank] & STATUS3_LEECHSEED
	&& ABILITY(bank) != ABILITY_MAGICGUARD)
		damage = MathMax(1, GetBaseMaxHP(bank) / 8);

	return damage;
}

u32 GetPoisonDamage(u8 bank, bool8 aiCalc)
{
	u32 damage = 0;
	u8 ability = ABILITY(bank);

	if (ability != ABILITY_MAGICGUARD
	&& ability != ABILITY_POISONHEAL)
	{
		if (gBattleMons[bank].status1 & STATUS_POISON)
		{
			damage = MathMax(1, GetBaseMaxHP(bank) / 8);
		}
		else if (gBattleMons[bank].status1 & STATUS_TOXIC_POISON)
		{
			u32 status = gBattleMons[bank].status1 & STATUS1_TOXIC_COUNTER;
			if (aiCalc && status < STATUS1_TOXIC_TURN(15))
				status += 0x100; //At the end of the turn it'll do more

			damage = MathMax(1, GetBaseMaxHP(bank) / 16);
			damage *= (status) >> 8;
		}
	}
	else if (ability == ABILITY_POISONHEAL && !aiCalc)
		damage = MathMax(1, GetBaseMaxHP(bank) / 8);

	return damage;
}

u32 GetBurnDamage(u8 bank)
{
	u32 damage = 0;
	u8 ability = ABILITY(bank);

	if (gBattleMons[bank].status1 & STATUS_BURN
	&& ability != ABILITY_MAGICGUARD)
	{
		if (ability == ABILITY_HEATPROOF)
		{
			#ifdef OLD_BURN_DAMAGE
				damage = MathMax(1, GetBaseMaxHP(bank) / 16);
			#else
				damage = MathMax(1, GetBaseMaxHP(bank) / 32);
			#endif
		}
		else
		{
			#ifdef OLD_BURN_DAMAGE
				damage = MathMax(1, GetBaseMaxHP(bank) / 8);
			#else
				damage = MathMax(1, GetBaseMaxHP(bank) / 16);
			#endif
		}
	}

	return damage;
}

u32 GetFrostbiteDamage(unusedArg u8 bank)
{
	u32 damage = 0;

	#ifdef FROSTBITE
	u8 ability = ABILITY(bank);

	if (gBattleMons[bank].status1 & STATUS_FREEZE
	&& ability != ABILITY_MAGICGUARD)
	{
		#ifdef OLD_BURN_DAMAGE
			damage = MathMax(1, GetBaseMaxHP(bank) / 8);
		#else
			damage = MathMax(1, GetBaseMaxHP(bank) / 16);
		#endif
	}
	#endif

	return damage;
}

u32 GetNightmareDamage(u8 bank)
{
	u32 damage = 0;

	if (gBattleMons[bank].status2 & STATUS2_NIGHTMARE
	&& gBattleMons[bank].status1 & STATUS_SLEEP
	&& ABILITY(bank) != ABILITY_MAGICGUARD)
	{
		damage = MathMax(1, GetBaseMaxHP(bank) / 4);
	}

	return damage;
}

u32 GetCurseDamage(u8 bank)
{
	u32 damage = 0;

	if (gBattleMons[bank].status2 & STATUS2_CURSED
	&& ABILITY(bank) != ABILITY_MAGICGUARD)
	{
		damage = MathMax(1, GetBaseMaxHP(bank) / 4);
	}

	return damage;
}

u32 GetTrapDamage(u8 bank)
{
	u32 damage = 0;

	if (gBattleMons[bank].status2 & STATUS2_WRAPPED
	&& !(gNewBS->brokeFreeMessage & gBitTable[bank]) //Trapping isn't about to end
	&& ABILITY(bank) != ABILITY_MAGICGUARD)
	{
		if (gNewBS->SaltcureTimers[gActiveBattler] && (IsOfType(bank, TYPE_WATER) || IsOfType(bank, TYPE_STEEL)))
			damage = MathMax(1, GetBaseMaxHP(bank) / 4);
		else if ((gNewBS->sandblastCentiferno[gActiveBattler] & 2) //Trapped by this move and user held Binding Band
		|| ITEM_EFFECT(gBattleStruct->wrappedBy[bank]) == ITEM_EFFECT_BINDING_BAND)
			damage = MathMax(1, GetBaseMaxHP(bank) / 6);
		else
			damage = MathMax(1, GetBaseMaxHP(bank) / 8);
	}

	return damage;
}

u32 GetSeaOfFireDamage(u8 bank)
{
	u32 damage = 0;

	if (BankSideHasSeaOfFire(bank)
	&& !IsOfType(bank, TYPE_FIRE)
	&& ABILITY(bank) != ABILITY_MAGICGUARD)
	{
		damage = MathMax(1, GetBaseMaxHP(bank) / 8);
	}

	return damage;
}

u32 GetBadDreamsDamage(u8 bank) //Not actually used in calculation - mainly used for AI
{
	u32 damage = 0;

	if (gBattleMons[bank].status1 & STATUS1_SLEEP
	&& ABILITY(bank) != ABILITY_MAGICGUARD)
	{
		damage = MathMax(1, GetBaseMaxHP(bank) / 8);
		damage *= AbilityBattleEffects(ABILITYEFFECT_COUNT_OTHER_SIDE, bank, ABILITY_BADDREAMS, 0, 0);
	}

	return damage;
}

u32 GetSplintersDamage(u8 bank)
{
	u32 damage = 0;

	if (gNewBS->splinterTimer[bank] > 0
	&& ABILITY(bank) != ABILITY_MAGICGUARD)
		damage = SplintersDamageCalc(gNewBS->splinterAttackerBank[bank], bank, gNewBS->splinterMove[bank]);

	return damage;
}

u32 GetBadThoughtsDamage(u8 bank)
{
	u32 damage = 0;

	u8 divisor;
	u8 ability = ABILITY(bank);

	if (IsBadThoughtsBattle()
	&& !IsOfType(bank, TYPE_DARK)
	&& !IsOfType(bank, TYPE_GHOST)
	&& !IsOfType(bank, TYPE_PSYCHIC)
	&& ability != ABILITY_MAGICGUARD
	&& ability != ABILITY_OBLIVIOUS
	&& ability != ABILITY_UNAWARE)
	{
		if (gBattleTypeFlags & BATTLE_TYPE_FRONTIER)
		{
			divisor = 12; //1/12 of HP in Battle Circus
		}
		else
		{
			#ifdef VAR_GAME_DIFFICULTY
			switch (VarGet(VAR_GAME_DIFFICULTY)) {
				case OPTIONS_EASY_DIFFICULTY:
				case OPTIONS_NORMAL_DIFFICULTY:
					divisor = 16; //1/16 of HP
					break;
				case OPTIONS_HARD_DIFFICULTY:
					divisor = 12; //1/12 of HP
					break;
				case OPTIONS_EXPERT_DIFFICULTY:
				default:
					divisor = 7; //1/7 of HP
					break;
			}
			#else
				divisor = 16; //1/16 of HP
			#endif
		}

		damage = MathMax(1, GetBaseMaxHP(bank) / divisor);
	}

	return damage;
}

u32 GetGMaxVineLashDamage(u8 bank)
{
	u32 damage = 0;

	if (BankSideHasGMaxVineLash(bank)
	&& !IsOfType(bank, TYPE_GRASS)
	&& ABILITY(bank) != ABILITY_MAGICGUARD)
	{
		damage = MathMax(1, GetBaseMaxHP(bank) / 6);
	}

	return damage;
}

u32 GetGMaxWildfireDamage(u8 bank)
{
	u32 damage = 0;

	if (BankSideHasGMaxWildfire(bank)
	&& !IsOfType(bank, TYPE_FIRE)
	&& ABILITY(bank) != ABILITY_MAGICGUARD)
	{
		damage = MathMax(1, GetBaseMaxHP(bank) / 6);
	}

	return damage;
}

u32 GetGMaxCannonadeDamage(u8 bank)
{
	u32 damage = 0;

	if (BankSideHasGMaxCannonade(bank)
	&& !IsOfType(bank, TYPE_WATER)
	&& ABILITY(bank) != ABILITY_MAGICGUARD)
	{
		damage = MathMax(1, GetBaseMaxHP(bank) / 6);
	}

	return damage;
}

u32 GetGMaxVolcalithDamage(u8 bank)
{
	u32 damage = 0;

	if (BankSideHasGMaxVolcalith(bank)
	&& ABILITY(bank) != ABILITY_MAGICGUARD)
	{
		damage = MathMax(1, GetBaseMaxHP(bank) / 6);
	}

	return damage;
}

u32 GetSaltCureDamage(u8 bank)
{
	u32 damage = 0;

	if (gBattleMons[bank].status2 & STATUS2_WRAPPED
	&& !(gNewBS->brokeFreeMessage & gBitTable[bank]) //Trapping isn't about to end
	&& ABILITY(bank) != ABILITY_MAGICGUARD)
	{
		if (IsOfType(bank, TYPE_WATER)
		||  IsOfType(bank, TYPE_STEEL))
			damage = MathMax(1, GetBaseMaxHP(bank) / 4);
		else
			damage = MathMax(1, GetBaseMaxHP(bank) / 8);
	}

	return damage;
}

#define FAINTED_ACTIONS_MAX_CASE 11

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
				OpponentSwitchInResetSentPokesToOpponentValue(gBankFainted);
				if (++gBattleStruct->faintedActionsBank >= gBattlersCount)
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
					BackupSwitchingBank();
					if (gBattleMons[gBattleStruct->faintedActionsBank].hp == 0)
					{
						if (IsCatchableRaidBattle()
						&& gBattleStruct->faintedActionsBank == BANK_RAID_BOSS)
							continue; //Don't end the battle if the raid mon is KOed

						if (RAID_BATTLE_END
						&& SIDE(gBattleStruct->faintedActionsBank) == B_SIDE_PLAYER
						&& ViableMonCountFromBank(GetBattlerAtPosition(B_POSITION_PLAYER_LEFT)) >= 1)
							continue; //Don't end the raid battle if the player is KOd but partner is still alive to help catch mon

						if (!(gAbsentBattlerFlags & gBitTable[gBattleStruct->faintedActionsBank])) //Bank was just emptied
						{
							gAbsentBattlerFlags |= gBitTable[gBattleStruct->faintedActionsBank]; //Makes the game realize the target is dead for now, so no attacking it
							gNewBS->AbsentBattlerHelper |= gBitTable[gBattleStruct->faintedActionsBank]; //Record which Pokemon need replacements
						}

						if (gNewBS->AbsentBattlerHelper & gBitTable[gBattleStruct->faintedActionsBank])
						{
							if (gNewBS->endTurnDone
							||  ViableMonCountFromBank(gBattleStruct->faintedActionsBank) == 0)
							{
								gAbsentBattlerFlags &= ~(gBitTable[gBattleStruct->faintedActionsBank]);

								//Not wild battle
								//In a double or frontier battle, the player is not asked if they want to switch Pokemon
								if (gBattleTypeFlags & BATTLE_TYPE_TRAINER && gBattleTypeFlags & (BATTLE_TYPE_DOUBLE | BATTLE_TYPE_FRONTIER | BATTLE_TYPE_LINK))
								{
									gNewBS->handleSetSwitchIns |= gBitTable[gBattleStruct->faintedActionsBank];
									BattleScriptExecute(BattleScript_HandleFaintedMonDoublesInitial);
								}
								else
								{
									BattleScriptExecute(BattleScript_HandleFaintedMonSingles);
									gNewBS->doSwitchInEffects |= gBitTable[gBattleStruct->faintedActionsBank];
									gNewBS->ai.switchingCooldown[gBattleStruct->faintedActionsBank] = 1; //AI shouldn't switch out again until after the next time an end turn is reached
									gNewBS->handlingFaintSwitching = TRUE;

									for (i = 0; i < gBattlersCount; ++i)
									{
										if (gNewBS->AbsentBattlerHelper & gBitTable[i]
										&&  !(gNewBS->doSwitchInEffects & gBitTable[i])
										&&  HITMARKER_FAINTED(i))
										{
											if (++gBattleStruct->faintedActionsBank >= gBattlersCount)
												gBattleStruct->faintedActionsState = 7;
											return TRUE; //Still more Pokemon that need to switch in (eg after Explosion)
										}
									}

									if (gBattlersCount & 1) //Odd number
										gAbsentBattlerFlags |= gBitTable[gBattlersCount]; //So switching doesn't crash in Raids

									if (++gBattleStruct->faintedActionsBank >= gBattlersCount)
										gBattleStruct->faintedActionsState = 7;
									return TRUE;
								}

								gBattleStruct->faintedActionsState = 5;
								return TRUE;
							}
						}
					}
				} while (++gBattleStruct->faintedActionsBank < gBattlersCount);

				if (gNewBS->handleSetSwitchIns)
				{
					gBattleStruct->faintedActionsBank = 0;
					gBattleStruct->faintedActionsState = 6;
				}
				else if (gNewBS->doSwitchInEffects)
					gBattleStruct->faintedActionsState = 7;
				else
					gBattleStruct->faintedActionsState = FAINTED_ACTIONS_MAX_CASE - 1;
				break;

			case 5:
				if (++gBattleStruct->faintedActionsBank >= gBattlersCount)
				{
					if (gNewBS->handleSetSwitchIns)
					{
						gBattleStruct->faintedActionsBank = 0;
						gBattleStruct->faintedActionsState = 6;
					}
					else if (gNewBS->doSwitchInEffects)
						gBattleStruct->faintedActionsState = 7;
					else
						gBattleStruct->faintedActionsState = FAINTED_ACTIONS_MAX_CASE - 1;
				}
				else
					gBattleStruct->faintedActionsState = 4;
				break;

			case 6:
				do
				{
					gBankFainted = gBankTarget = gBattleStruct->faintedActionsBank;
					BackupSwitchingBank();
					if (gNewBS->handleSetSwitchIns & gBitTable[gBattleStruct->faintedActionsBank])
					{
						++gBattleStruct->faintedActionsBank;
						gAbsentBattlerFlags &= ~(gBitTable[gBattleStruct->faintedActionsBank]);
						gNewBS->ai.switchingCooldown[gBankFainted] = 1; //AI shouldn't switch out again until after the next time an end turn is reached
						BattleScriptExecute(BattleScript_HandleFaintedMonDoublesPart2);
						return TRUE;
					}
				} while (++gBattleStruct->faintedActionsBank < gBattlersCount);
				__attribute__ ((fallthrough));

			case 7:
				//Recalc turn order for switch-in abilities
				for (i = 0; i < gBattlersCount; ++i)
				{
					if (gNewBS->doSwitchInEffects & gBitTable[i])
						gNewBS->handleSetSwitchIns |= gBitTable[i];

					if (!BATTLER_ALIVE(i))
						gAbsentBattlerFlags |= gBitTable[i];

					gBanksByTurnOrder[i] = i;
				}
				gNewBS->doSwitchInEffects = FALSE;

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

			case 8:
				do
				{
					gBankFainted = gBankTarget = gBanksByTurnOrder[gBattleStruct->faintedActionsBank];
					BackupSwitchingBank();
					if (gNewBS->handleSetSwitchIns & gBitTable[gBankFainted])
					{
						if (ABILITY(gBankFainted) == ABILITY_TRUANT)
							gDisableStructs[gBankFainted].truantCounter = 1; //So it gets unset during the switch in effects

						gNewBS->handleSetSwitchIns &= ~(gBitTable[gBankFainted]);
						BattleScriptExecute(BattleScript_HandleFaintedMonDoublesSwitchInEffects);
						return TRUE;
					}
				} while (++gBattleStruct->faintedActionsBank < gBattlersCount);
				gBattleStruct->faintedActionsState++;
				__attribute__ ((fallthrough));

			case 9:
				if (gNewBS->restartEndTurnSwitching) //Died from entry hazards
				{
					gNewBS->restartEndTurnSwitching = FALSE;
					gBattleStruct->faintedActionsState = 0;
				}
				else
					gBattleStruct->faintedActionsState++;
				break;

			case 10:
				gNewBS->endTurnDone = FALSE;
				gBattleStruct->faintedActionsState++;
				break;

			case FAINTED_ACTIONS_MAX_CASE:
				gNewBS->handlingFaintSwitching = FALSE;
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
	gNewBS->handleSetSwitchIns &= ~(gBitTable[gBankFainted]);
}

bool8 IsInMiddleOfEndTurnSwitchIn(u8 bank)
{
	if (gNewBS->handleSetSwitchIns & gBitTable[bank])
		return TRUE;

	return FALSE;
}

bool8 HandleDynamaxOnTurnEnd(void)
{
	u8 bank;

	if (IsRaidBattle()
	#ifdef FLAG_RAID_BATTLE_NO_FORCE_END
	&& !FlagGet(FLAG_RAID_BATTLE_NO_FORCE_END) //These battles can't be force ended
	&& BATTLER_ALIVE(BANK_RAID_BOSS) //Don't force out if battle is over
	#endif
	&& gBattleResults.battleTurnCounter + 1 >= 10) //10 Turns have passed
	{
		gBattleStringLoader = gText_RaidBattleKO4;

		if (BATTLER_ALIVE(GetBattlerAtPosition(B_POSITION_PLAYER_LEFT)))
			gBattleScripting.bank = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);
		else
			gBattleScripting.bank = GetBattlerAtPosition(B_POSITION_PLAYER_RIGHT);
		BattleScriptExecute(BattleScript_LoseRaidBattle);
		gBattleResults.battleTurnCounter -= 1; //To prevent infinite loop
		return TRUE;
	}

	if (!RAID_BATTLE_END)
	{
		for (bank = 0; bank < gBattlersCount; ++bank)
		{
			if (gNewBS->dynamaxData.timer[bank] == -1) //Revert
			{
				TryRevertBankGigantamax(bank);
				gNewBS->dynamaxData.timer[bank] = 0;
				gBattleScripting.bank = bank;
				BattleScriptExecute(BattleScript_DynamaxEnd);
				return TRUE;
			}
		}
	}

	CalculateShellSideArmSplits(); //Only done at the beginning of each turn
	return FALSE;
}
