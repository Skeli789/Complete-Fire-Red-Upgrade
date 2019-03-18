#include "defines.h"
#include "helper_functions.h"

#define BattleScript_OverworldWeatherStarts (u8*) 0x81D8B1F
#define BattleScript_DrizzleActivates (u8*) 0x81D927F
#define BattleScript_SandstreamActivates (u8*) 0x81D92BF
#define BattleScript_DroughtActivates (u8*) 0x81D9379
#define BattleScript_CastformChange (u8*) 0x81D92F8
#define BattleScript_ShedSkinActivates (u8*) 0x81D92D3
#define BattleScript_SpeedBoostActivates (u8*) 0x81D9293
#define BattleScript_ColorChangeActivates (u8*) 0x81D949C
#define BattleScript_ApplySecondaryEffect (u8*) 0x81D94CB
#define BattleScript_AbilityCuredStatus (u8*) 0x81D94D1

extern u8 BattleScript_NewWeatherAbilityActivates[];
extern u8 BattleScript_IntimidateActivatesEnd3[];
extern u8 BattleScript_TraceActivates[];
extern u8 BattleScript_SwitchInAbilityMsg[];
extern u8 BattleScript_AbilityCuredStatusEnd3[];
extern u8 BattleScript_StartedSchoolingEnd3[];
extern u8 BattleScript_StoppedSchoolingEnd3[];
extern u8 BattleScript_ShieldsDownToCoreEnd3[];
extern u8 BattleScript_ShieldsDownToMeteorEnd3[];
extern u8 BattleScript_TransformedEnd3[];
extern u8 BattleScript_AttackerAbilityStatRaiseEnd3[];
extern u8 BattleScript_ImposterActivates[];
extern u8 BattleScript_TerrainFromAbility[];
extern u8 BattleScript_RainDishActivates[];
extern u8 BattleScript_DrySkinDamage[];
extern u8 BattleScript_SolarPowerDamage[];
extern u8 BattleScript_Healer[];
extern u8 BattleScript_MoodySingleStat[];
extern u8 BattleScript_MoodyRegular[];
extern u8 BattleScript_BadDreams[];
extern u8 BattleScript_Harvest[];
extern u8 BattleScript_Pickup[];
extern u8 BattleScript_PrintCustomStringEnd3[];
extern u8 BattleScript_SoundproofProtected[];
extern u8 BattleScript_FlashFireBoost[];
extern u8 BattleScript_FlashFireBoost_PPLoss[];
extern u8 BattleScript_MonMadeMoveUseless[];
extern u8 BattleScript_MonMadeMoveUseless_PPLoss[];
extern u8 BattleScript_MoveHPDrain[];
extern u8 BattleScript_MoveHPDrain_PPLoss[];
extern u8 BattleScript_MoveStatDrain[];
extern u8 BattleScript_MoveStatDrain_PPLoss[];
extern u8 BattleScript_RoughSkinActivates[];
extern u8 BattleScript_CuteCharmActivates[];
extern u8 BattleScript_TargetAbilityStatRaise[];
extern u8 BattleScript_WeakArmorActivates[];
extern u8 BattleScript_CursedBodyActivates[];
extern u8 BattleScript_MummyActivates[];
extern u8 BattleScript_AngerPointActivates[];
extern u8 BattleScript_GooeyActivates[];
extern u8 BattleScript_IllusionBroken[];
extern u8 BattleScript_SynchronizeActivates_StatusesAttacker[];
extern u8 BattleScript_SynchronizeActivates_StatusesTarget[];

extern u8 gText_FogIsDeep[];
extern u8 gText_SnowWarningActivate[];
extern u8 gText_PrimordialSeaActivate[];
extern u8 gText_DesolateLandActivate[];
extern u8 gText_DeltaStream[];
extern u8 gText_AirLockActivate[];
extern u8 gText_PressureActivate[];
extern u8 gText_MoldBreakerActivate[];
extern u8 gText_TeravoltActivate[];
extern u8 gText_TurboblazeActivate[];
extern u8 gText_SlowStartActivate[];
extern u8 gText_UnnerveActivate[];
extern u8 gText_DarkAuraActivate[];
extern u8 gText_FairyAuraActivate[];
extern u8 gText_AuraBreakActivate[];
extern u8 gText_ComatoseActivate[];
extern u8 gText_AnticipationActivate[];
extern u8 gText_ForewarnActivate[];
extern u8 gText_FriskActivate[];
extern u8 ElectricTerrainSetString[];
extern u8 GrassyTerrainSetString[];
extern u8 MistyTerrainSetString[];
extern u8 PsychicTerrainSetString[];
extern u8 gText_SlowStartEnd[];

extern ability_t TraceBanTable[];
extern move_t BallBombMoveTable[];
extern move_t PowderTable[];

extern u8 gStatusConditionString_MentalState[];
extern u8 gStatusConditionString_TauntProblem[];

const u16 gWeatherContinuesStringIds[] =
{
    STRINGID_ITISRAINING,		//No Weather
	STRINGID_ITISRAINING, 		//Sunnny Weather with Clouds in Water
	STRINGID_ITISRAINING,		//Regular Weather
    STRINGID_ITISRAINING,   	//Rainy Weather
	STRINGID_STARTEDHAIL, 		//Light Snow
	STRINGID_ITISRAINING,		//Thunderstorm
    STRINGID_CUSTOMSTRING, 		//Steady Mist
	STRINGID_STARTEDHAIL, 		//Steady Snowing
	STRINGID_SANDSTORMISRAGING,	//Sandstorm
    STRINGID_CUSTOMSTRING, 		//Mist from Top Right Corner
	STRINGID_CUSTOMSTRING, 		//Dense Bright Mist
	STRINGID_ITISRAINING,		//Cloudy
    STRINGID_SUNLIGHTSTRONG, 	//Drought
	STRINGID_DOWNPOURSTARTED, 	//Downpour
	STRINGID_CUSTOMSTRING, 		//Underwater Mist
	STRINGID_ITISRAINING		//???
};

extern u8 TypeCalc(move_t, u8 bankAtk, u8 bankDef, pokemon_t* party_data_atk, bool8 CheckParty);
extern u8 GetExceptionMoveType(u8 bankAtk, move_t);
extern u8 CastformDataTypeChange(u8 bank);
extern s8 PriorityCalc(u8 bank, u8 action, u16 move);
extern void TransformPokemon(u8 bankAtk, u8 bankDef);

u8 AbilityBattleEffects(u8 caseID, u8 bank, u8 ability, u8 special, u16 moveArg);
u8 CalcMovePowerForForewarn(u16 move);
u8 ActivateWeatherAbility(u16 flags, u16 item, u8 bank, u8* script);
u8 TryActivateTerrainAbility(u8 terrain, u8 anim, u8 bank);
bool8 ImmunityAbilityCheck(u8 bank, u32 status, u8* string);
bool8 AllStatsButOneAreMinned(u8 bank);

u8 AbilityBattleEffects(u8 caseID, u8 bank, u8 ability, u8 special, u16 moveArg)
{
	int i;
    u8 effect = 0;
    u16 speciesAtk;
    u16 speciesDef;
    u32 pidAtk;
    u32 pidDef;
    u16 move;
	u8 moveType;
    u8 side;
    u8 target1;
		
    if (gBattleTypeFlags & BATTLE_TYPE_SAFARI)
		return FALSE;
	
	if (gBankAttacker >= gBattlersCount)
		gBankAttacker = bank;

	speciesAtk = gBattleMons[gBankAttacker].species;
	pidAtk = gBattleMons[gBankAttacker].personality;

    speciesDef = gBattleMons[gBankTarget].species;
    pidDef = gBattleMons[gBankTarget].personality;
		
    if (special)
        gLastUsedAbility = special;
    else
        gLastUsedAbility = ABILITY(bank);

    if (moveArg)
        move = moveArg;
    else
        move = gCurrentMove;
	
    moveType = gBattleStruct->dynamicMoveType;
	
	#ifndef NO_GHOST_BATTLES
		if ((gBattleTypeFlags & (BATTLE_TYPE_SCRIPTED_WILD_1 | BATTLE_TYPE_GHOST)) == BATTLE_TYPE_GHOST)
		{
			switch (gLastUsedAbility) {		//All of these abilities either use or make changes to
				case ABILITY_INTIMIDATE:	//the unidentified Ghost. In FR, only Intimidate and
				case ABILITY_TRACE:			//Trace were included in this list. It has thus been
				case ABILITY_DOWNLOAD:		//been expanded to support newer abilities.
				case ABILITY_UNNERVE:
				case ABILITY_ANTICIPATION:
				case ABILITY_FOREWARN:
				case ABILITY_FRISK:
				case ABILITY_IMPOSTER:
					return FALSE;
			}
		}
	#endif

    switch (caseID)
    {
    case ABILITYEFFECT_ON_SWITCHIN: // 0;
		gBankAttacker = gBattleScripting->bank = bank;
		
		if (gStatuses3[bank] & STATUS3_SWITCH_IN_ABILITY_DONE
		&& gLastUsedAbility != ABILITYEFFECT_SWITCH_IN_WEATHER)
			break;

        switch (gLastUsedAbility)
        {
        case ABILITYEFFECT_SWITCH_IN_WEATHER:
            switch (GetCurrentWeather()) {
                case WEATHER_RAIN_LIGHT:
                case WEATHER_RAIN_MED:
                    if (!(gBattleWeather & WEATHER_RAIN_ANY))
                    {
                        gBattleWeather = (WEATHER_RAIN_TEMPORARY | WEATHER_RAIN_PERMANENT);
                        gBattleScripting->animArg1 = B_ANIM_RAIN_CONTINUES;
                        effect++;
                    }
                    break;
                case WEATHER_RAIN_HEAVY:
                    if (!(gBattleWeather & WEATHER_RAIN_ANY))
                    {
                        gBattleWeather = (WEATHER_RAIN_DOWNPOUR | WEATHER_RAIN_PERMANENT);
                        gBattleScripting->animArg1 = B_ANIM_RAIN_CONTINUES;
                        effect++;
                    }
                    break;
                case WEATHER_SANDSTORM:
                    if (!(gBattleWeather & WEATHER_SANDSTORM_ANY))
                    {
                        gBattleWeather = (WEATHER_SANDSTORM_PERMANENT | WEATHER_SANDSTORM_TEMPORARY);
                        gBattleScripting->animArg1 = B_ANIM_SANDSTORM_CONTINUES;
                        effect++;
                    }
                    break;
                case WEATHER_DROUGHT:
                    if (!(gBattleWeather & WEATHER_SUN_ANY))
                    {
                        gBattleWeather = (WEATHER_SUN_PERMANENT | WEATHER_SUN_TEMPORARY);
                        gBattleScripting->animArg1 = B_ANIM_SUN_CONTINUES;
                        effect++;
                    }
                    break;
					
			#ifdef HAIL_IN_BATTLE
				case WEATHER_STEADY_SNOW:
                    if (!(gBattleWeather & WEATHER_HAIL_ANY))
                    {
                        gBattleWeather = (WEATHER_HAIL_PERMANENT | WEATHER_HAIL_TEMPORARY);
                        gBattleScripting->animArg1 = B_ANIM_HAIL_CONTINUES;
                        effect++;
                    }
                    break;
			#endif
					
            #ifdef FOG_IN_BATTLE
				#ifdef FOG_IN_BATTLE_1
				case WEATHER_FOG_1:
				#endif
				#ifdef FOG_IN_BATTLE_2
				case WEATHER_FOG_2:
				#endif
				#ifdef FOG_IN_BATTLE_3
				case WEATHER_FOG_3:
				#endif
					
				#if defined FOG_IN_BATTLE_1 || defined FOG_IN_BATTLE_2 || defined FOG_IN_BATTLE_3 //So dumb people don't cause compilation errors
					if (!(gBattleWeather & WEATHER_FOG_ANY))
					{
						BattleStringLoader = gText_FogIsDeep;
						gBattleWeather = (WEATHER_FOG_TEMPORARY | WEATHER_FOG_PERMANENT);
						gBattleScripting->animArg1 = B_ANIM_FOG_CONTINUES;
						effect++;
					}
				#endif
			#endif
            }
				
            if (effect)
            {
                gBattleCommunication[MULTISTRING_CHOOSER] = GetCurrentWeather();
                BattleScriptPushCursorAndCallback(BattleScript_OverworldWeatherStarts);
            }
            break;
			
        case ABILITY_DRIZZLE:
            if (!(gBattleWeather & (WEATHER_RAIN_ANY | WEATHER_PRIMAL_ANY)))
            {
				effect = ActivateWeatherAbility(WEATHER_RAIN_PERMANENT | WEATHER_RAIN_TEMPORARY, 
												ITEM_EFFECT_DAMP_ROCK, bank, 
												BattleScript_DrizzleActivates);
            }

            break;
			
        case ABILITY_SANDSTREAM:
            if (!(gBattleWeather & (WEATHER_SANDSTORM_ANY | WEATHER_PRIMAL_ANY)))
            {
				effect = ActivateWeatherAbility(WEATHER_SANDSTORM_PERMANENT | WEATHER_SANDSTORM_TEMPORARY, 
												ITEM_EFFECT_SMOOTH_ROCK, bank, 
												BattleScript_SandstreamActivates);
            }
			
            break;
			
        case ABILITY_DROUGHT:
            if (!(gBattleWeather & (WEATHER_SUN_ANY | WEATHER_PRIMAL_ANY)))
            {
				effect = ActivateWeatherAbility(WEATHER_SUN_PERMANENT | WEATHER_SUN_TEMPORARY, 
												ITEM_EFFECT_HEAT_ROCK, bank, 
												BattleScript_DroughtActivates);
            }

            break;
				
		case ABILITY_SNOWWARNING:
            if (!(gBattleWeather & (WEATHER_HAIL_ANY | WEATHER_PRIMAL_ANY)))
            {
				gBattleScripting->animArg1 = B_ANIM_HAIL_CONTINUES;
				BattleStringLoader = gText_SnowWarningActivate;
				effect = ActivateWeatherAbility(WEATHER_HAIL_PERMANENT | WEATHER_HAIL_TEMPORARY, 
												ITEM_EFFECT_ICY_ROCK, bank, 
												BattleScript_NewWeatherAbilityActivates);
            }

            break;

		case ABILITY_PRIMORDIALSEA:
            if (!(gBattleWeather & WEATHER_RAIN_PRIMAL))
            {
				gBattleWeather = (WEATHER_RAIN_PERMANENT | WEATHER_RAIN_TEMPORARY |  WEATHER_RAIN_PRIMAL);
                gWishFutureKnock->weatherDuration = 0;
				gBattleScripting->animArg1 = B_ANIM_RAIN_CONTINUES;
				BattleStringLoader = gText_PrimordialSeaActivate;
				BattleScriptPushCursorAndCallback(BattleScript_NewWeatherAbilityActivates);
                effect++;
            }

            break;
		
		case ABILITY_DESOLATELAND:
            if (!(gBattleWeather & WEATHER_SUN_PRIMAL))
            {
				gBattleWeather = (WEATHER_SUN_PERMANENT | WEATHER_SUN_TEMPORARY |  WEATHER_SUN_PRIMAL);
                gWishFutureKnock->weatherDuration = 0;
				gBattleScripting->animArg1 = B_ANIM_SUN_CONTINUES;
				BattleStringLoader = gText_DesolateLandActivate;
				BattleScriptPushCursorAndCallback(BattleScript_NewWeatherAbilityActivates);
                effect++;
            }

            break;
				
		case ABILITY_DELTASTREAM:
            if (!(gBattleWeather & WEATHER_AIR_CURRENT_PRIMAL))
            {
				gBattleWeather = (WEATHER_AIR_CURRENT_PRIMAL);
                gWishFutureKnock->weatherDuration = 0;
				gBattleScripting->animArg1 = B_ANIM_STRONG_WINDS_CONTINUE;
				BattleStringLoader = gText_DeltaStream;
				BattleScriptPushCursorAndCallback(BattleScript_NewWeatherAbilityActivates);
                effect++;
            }

            break;
			
        case ABILITY_INTIMIDATE:
			if (!(gBattleMons[FOE(bank)].status2 & STATUS2_SUBSTITUTE) || !(gBattleMons[PARTNER(FOE(bank))].status2 & STATUS2_SUBSTITUTE))
			{
				BattleScriptPushCursorAndCallback(BattleScript_IntimidateActivatesEnd3);
				gBattleStruct->intimidateBank = bank;
				effect++;
			}
            break;
		
        case ABILITY_FORECAST:
            effect = CastformDataTypeChange(bank);
            if (effect != 0)
            {
                BattleScriptPushCursorAndCallback(BattleScript_CastformChange);
                gBattleStruct->castformToChangeInto = effect - 1;
            }

            break;
			
        case ABILITY_TRACE: ;
			u8 target2;
            side = (GetBattlerPosition(bank) ^ BIT_SIDE) & BIT_SIDE; // side of the opposing pokemon
            target1 = GetBattlerAtPosition(side);
            target2 = GetBattlerAtPosition(side + BIT_FLANK);
            if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
            {
                if (gBattleMons[target1].ability != 0 && gBattleMons[target1].hp != 0
                && gBattleMons[target2].ability != 0 && gBattleMons[target2].hp != 0)
                {
                    gActiveBattler = GetBattlerAtPosition(((Random() & 1) * 2) | side);
                    effect++;
                }
                else if (gBattleMons[target1].ability != 0 && gBattleMons[target1].hp != 0)
                {
                    gActiveBattler = target1;

                    effect++;
                }
                else if (gBattleMons[target2].ability != 0 && gBattleMons[target2].hp != 0)
                {
                    gActiveBattler = target2;
                    effect++;
                }
            }
            else //Single Battle
            {
                if (gBattleMons[target1].ability && gBattleMons[target1].hp)
                {
                    gActiveBattler = target1;
                    effect++;
                }
            }
			
            if (effect)
            {
				if (!CheckTableForAbility(*GetAbilityLocation(gActiveBattler), TraceBanTable))
				{
					*GetAbilityLocation(bank) = *GetAbilityLocation(gActiveBattler);
                    gLastUsedAbility = *GetAbilityLocation(gActiveBattler);
					BattleScriptPushCursorAndCallback(BattleScript_TraceActivates);

					PREPARE_MON_NICK_WITH_PREFIX_BUFFER(gBattleTextBuff1, gActiveBattler, gBattlerPartyIndexes[gActiveBattler])
					PREPARE_ABILITY_BUFFER(gBattleTextBuff2, gLastUsedAbility)
				}
				else
					--effect;
            }
            break;
			
        case ABILITY_CLOUDNINE:
        case ABILITY_AIRLOCK:
            {
                for (bank = 0; bank < gBattlersCount; ++bank)
                {
                    effect = CastformDataTypeChange(bank);
                    if (effect != 0)
                    {
                        BattleScriptPushCursorAndCallback(BattleScript_CastformChange);
                        gBattleStruct->castformToChangeInto = effect - 1;
                        break;
                    }
                }
				
				BattleStringLoader = gText_AirLockActivate;
	
				BattleScriptPushCursorAndCallback(BattleScript_SwitchInAbilityMsg);
				effect++;
            }
            break;
		
		case ABILITY_PRESSURE:
            BattleStringLoader = gText_PressureActivate;
            
            BattleScriptPushCursorAndCallback(BattleScript_SwitchInAbilityMsg);
            effect++;
            break;
		
		case ABILITY_MOLDBREAKER:
            BattleStringLoader = gText_MoldBreakerActivate;
            BattleScriptPushCursorAndCallback(BattleScript_SwitchInAbilityMsg);
            effect++;
            break;
			
		case ABILITY_TERAVOLT:
            BattleStringLoader = gText_TeravoltActivate;
            BattleScriptPushCursorAndCallback(BattleScript_SwitchInAbilityMsg);
            effect++;
            break;

		case ABILITY_TURBOBLAZE:
            BattleStringLoader = gText_TurboblazeActivate;
            BattleScriptPushCursorAndCallback(BattleScript_SwitchInAbilityMsg);
            effect++;
            break;
			
		case ABILITY_SLOWSTART:
            gNewBS->SlowStartTimers[bank] = 5;
            BattleStringLoader = gText_SlowStartActivate;
            BattleScriptPushCursorAndCallback(BattleScript_SwitchInAbilityMsg);
            effect++;
            break;
		
		case ABILITY_UNNERVE:
			gBankAttacker = bank;
            BattleStringLoader = gText_UnnerveActivate;
            BattleScriptPushCursorAndCallback(BattleScript_SwitchInAbilityMsg);
            effect++;
            break;

		case ABILITY_DARKAURA:
            BattleStringLoader = gText_DarkAuraActivate;
            BattleScriptPushCursorAndCallback(BattleScript_SwitchInAbilityMsg);
            effect++;
            break;

		case ABILITY_FAIRYAURA:
            BattleStringLoader = gText_FairyAuraActivate;
            BattleScriptPushCursorAndCallback(BattleScript_SwitchInAbilityMsg);
            effect++;
            break;
		
		case ABILITY_AURABREAK:
            BattleStringLoader = gText_AuraBreakActivate;
            BattleScriptPushCursorAndCallback(BattleScript_SwitchInAbilityMsg);
            effect++;
            break;
		
		case ABILITY_COMATOSE:
			BattleStringLoader = gText_ComatoseActivate;
            BattleScriptPushCursorAndCallback(BattleScript_SwitchInAbilityMsg);
            effect++;
            break;
			
		case ABILITY_ANTICIPATION:
			for (i = 0; i < MAX_MON_MOVES; ++i)
			{
				if (gBattleMons[FOE(bank)].hp)
				{
					move = gBattleMons[FOE(bank)].moves[i];
						
					if (gBattleMoves[move].effect == EFFECT_0HKO)
					{
						++effect;
						break;
					}
						
					moveType = gBattleMoves[move].type;
					if (move == MOVE_HIDDENPOWER)
						moveType = GetExceptionMoveType(FOE(bank), move);
							
					if (MOVE_RESULT_SUPER_EFFECTIVE &
						TypeCalc(move, FOE(bank), bank, 0, 0))
					{
						++effect;
						break;
					}
				}
					
				if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE
				&&  gBattleMons[PARTNER(FOE(bank))].hp)
				{
					move = gBattleMons[PARTNER(FOE(bank))].moves[i];
						
					if (gBattleMoves[move].effect == EFFECT_0HKO)
					{
						++effect;
						break;
					}
						
					moveType = gBattleMoves[move].type;
					if (move == MOVE_HIDDENPOWER)
						moveType = GetExceptionMoveType(PARTNER(FOE(bank)), move);
							
					if (MOVE_RESULT_SUPER_EFFECTIVE &
						TypeCalc(move, PARTNER(FOE(bank)), bank, 0, 0))
					{
						++effect;
						break;
					}
				}
			}
			
			if (effect)
			{
				BattleStringLoader = gText_AnticipationActivate;
				BattleScriptPushCursorAndCallback(BattleScript_SwitchInAbilityMsg);
            }
			
			break;
		
		case ABILITY_FOREWARN: ;
			u16 strongestMove = 0;
			u8 maxPower = 0;
			u8 strongestTarget = FOE(bank);
			u16 power;
				
			for (i = 0; i < MAX_MON_MOVES; ++i)
			{
				if (gBattleMons[FOE(bank)].hp)
				{
					move = gBattleMons[FOE(bank)].moves[i];
					power = CalcMovePowerForForewarn(move);

					if (power > maxPower
					|| (power == maxPower && Random() & 1))
					{
						maxPower = power;
						strongestMove = move;
						strongestTarget = FOE(bank);
					}	
				}
					
				if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE
				&&  gBattleMons[PARTNER(FOE(bank))].hp)
				{
					move = gBattleMons[PARTNER(FOE(bank))].moves[i];
					power = CalcMovePowerForForewarn(move);

					if (power > maxPower
					|| (power == maxPower && Random() & 1))
					{
						maxPower = power;
						strongestMove = move;
						strongestTarget = PARTNER(FOE(bank));
					}
				}
			}

			gBankAttacker = bank;
			gBattleScripting->bank = strongestTarget;
			PREPARE_MOVE_BUFFER(gBattleTextBuff1, strongestMove);
            BattleStringLoader = gText_ForewarnActivate;
            BattleScriptPushCursorAndCallback(BattleScript_SwitchInAbilityMsg);
            effect++;
            break;
		
		case ABILITY_FRISK: ;
            u8 foe = (SIDE(bank) ^ BIT_SIDE) | (Random() & 1);
			if (ITEM(foe))
			{
				gLastUsedItem = ITEM(foe);
				gBankTarget = foe;
				gBattleScripting->bank = bank;
				BattleStringLoader = gText_FriskActivate; 
				BattleScriptPushCursorAndCallback(BattleScript_SwitchInAbilityMsg);
				effect++;
			}
            break;
		
        case ABILITY_IMMUNITY:
			effect = ImmunityAbilityCheck(bank, STATUS1_PSN_ANY, gStatusConditionString_Poison);
            break;	
				
        case ABILITY_LIMBER:
			effect = ImmunityAbilityCheck(bank, STATUS1_PARALYSIS, gStatusConditionString_Paralysis);
            break;
		
        case ABILITY_INSOMNIA:
        case ABILITY_VITALSPIRIT:
			effect = ImmunityAbilityCheck(bank, STATUS1_SLEEP, gStatusConditionString_Sleep);
			break;

        case ABILITY_WATERVEIL:
		case ABILITY_WATERBUBBLE:
			effect = ImmunityAbilityCheck(bank, STATUS1_BURN, gStatusConditionString_Burn);
			break;

        case ABILITY_MAGMAARMOR:
		    effect = ImmunityAbilityCheck(bank, STATUS1_FREEZE, gStatusConditionString_Ice);
			break;
			
		case ABILITY_DOWNLOAD: ;
            u8 statId;
            u32 opposingBank = BATTLE_OPPOSITE(bank);
				
            u32 opposingDef = gBattleMons[opposingBank].defense;
			APPLY_STAT_MOD(opposingDef, &gBattleMons[opposingBank], opposingDef, STAT_DEF);
            u32 opposingSpDef = gBattleMons[opposingBank].spDefense;
			APPLY_STAT_MOD(opposingDef, &gBattleMons[opposingBank], opposingDef, STAT_SPDEF);
				
			if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
			{
				u32 opposingPartnerDef = 0;
				u32 opposingPartnerSpDef = 0;
				opposingBank = PARTNER(opposingBank);
				if (gBattleMons[opposingBank].hp)
				{
					u32 opposingPartnerDef = gBattleMons[opposingBank].defense;
					APPLY_STAT_MOD(opposingPartnerDef, &gBattleMons[opposingBank], opposingPartnerDef, STAT_DEF);
					u32 opposingPartnerSpDef = gBattleMons[opposingBank].spDefense;
					APPLY_STAT_MOD(opposingPartnerSpDef, &gBattleMons[opposingBank], opposingPartnerSpDef, STAT_SPDEF);
				}
					
				opposingDef += opposingPartnerDef;
				opposingSpDef += opposingPartnerSpDef;
			}

            if (opposingDef < opposingSpDef)
                statId = STAT_ATK;
            else
                statId = STAT_SPATK;

            if (gBattleMons[bank].statStages[statId-1] < 12)
            {
                gBattleMons[bank].statStages[statId-1]++;
                gBattleScripting->statChanger = statId | INCREASE_1;
                PREPARE_STAT_BUFFER(gBattleTextBuff1, statId);
                BattleScriptPushCursorAndCallback(BattleScript_AttackerAbilityStatRaiseEnd3);
                effect++;
            }
            break;
			
        case ABILITY_IMPOSTER:
            if (gBattleMons[BATTLE_OPPOSITE(bank)].hp
            && !(gBattleMons[BATTLE_OPPOSITE(bank)].status2 & (STATUS2_TRANSFORMED | STATUS2_SUBSTITUTE))
			&& !(gStatuses3[BATTLE_OPPOSITE(bank)] & (STATUS3_SEMI_INVULNERABLE | STATUS3_ILLUSION))
            && !(gBattleMons[bank].status2 & STATUS2_TRANSFORMED))
            {
                gBankTarget = BATTLE_OPPOSITE(bank);
				TransformPokemon(bank, gBankTarget);
                BattleScriptPushCursorAndCallback(BattleScript_ImposterActivates);
                effect++;
            }
            break;
		
		case ABILITY_SCHOOLING:
            if (!(gBattleMons[gActiveBattler].status2 & STATUS2_TRANSFORMED))
			{
				if (speciesAtk == PKMN_WISHIWASHI && gBattleMons[gActiveBattler].level >= 20 
				&&  gBattleMons[gActiveBattler].hp > (gBattleMons[gActiveBattler].maxHP / 4)) 
				{
					DoFormChange(bank, PKMN_WISHIWASHI_S, FALSE, TRUE);
					BattleScriptPushCursorAndCallback(BattleScript_StartedSchoolingEnd3);
					++effect;
				}
				else if (speciesAtk == PKMN_WISHIWASHI_S 
				&& (gBattleMons[gActiveBattler].level < 20 || 
					gBattleMons[gActiveBattler].hp <= (gBattleMons[gActiveBattler].maxHP / 4))) 
				{
					DoFormChange(bank, PKMN_WISHIWASHI, FALSE, TRUE);
					BattleScriptPushCursorAndCallback(BattleScript_StoppedSchoolingEnd3);
					++effect;
				}
			}
			break;
							
		case ABILITY_SHIELDSDOWN:
            if (!(gBattleMons[gActiveBattler].status2 & STATUS2_TRANSFORMED))
			{
				if (speciesAtk == PKMN_MINIORSHIELD 
				&& gBattleMons[gActiveBattler].hp <= (gBattleMons[gActiveBattler].maxHP / 2)) 
				{
					DoFormChange(bank, umodsi(GetBankPartyData(bank)->personality, 7), FALSE, TRUE); //Get Minior Colour
					BattleScriptPushCursorAndCallback(BattleScript_ShieldsDownToCoreEnd3);
					++effect;
				}
				else if ((speciesAtk == PKMN_MINIOR_RED ||
						  speciesAtk == PKMN_MINIOR_BLUE ||
					      speciesAtk == PKMN_MINIOR_ORANGE ||
					      speciesAtk == PKMN_MINIOR_YELLOW ||
						  speciesAtk == PKMN_MINIOR_INDIGO ||
						  speciesAtk == PKMN_MINIOR_GREEN ||
						  speciesAtk == PKMN_MINIOR_VIOLET) 
				&& gBattleMons[gActiveBattler].hp > (gBattleMons[gActiveBattler].maxHP / 2)) 
				{
					DoFormChange(bank, PKMN_MINIORSHIELD, FALSE, TRUE);
					BattleScriptPushCursorAndCallback(BattleScript_ShieldsDownToMeteorEnd3);
					++effect;
				}
			}
			break;
		
		case ABILITY_FLOWERGIFT:
            if (!(gBattleMons[gActiveBattler].status2 & STATUS2_TRANSFORMED))
			{
				gBankAttacker = bank;
				switch(speciesAtk) {
					case PKMN_CHERRIM:
						if (WEATHER_HAS_EFFECT && (gBattleWeather & WEATHER_SUN_ANY))
						{
							DoFormChange(bank, PKMN_CHERRIMSUN, FALSE, FALSE);
							BattleScriptPushCursorAndCallback(BattleScript_TransformedEnd3);
							++effect;
						}
						break;
					
					case PKMN_CHERRIMSUN:
						if (!WEATHER_HAS_EFFECT || !(gBattleWeather & WEATHER_SUN_ANY))
						{
							DoFormChange(bank, PKMN_CHERRIM, FALSE, FALSE);
							BattleScriptPushCursorAndCallback(BattleScript_TransformedEnd3);
							++effect;
						}
				}
			}
			break;
			
		case ABILITY_ELECTRICSURGE:
            effect = TryActivateTerrainAbility(ELECTRIC_TERRAIN, B_ANIM_ELECTRIC_SURGE, bank);
			break;
		
		case ABILITY_GRASSYSURGE:
            effect = TryActivateTerrainAbility(GRASSY_TERRAIN, B_ANIM_GRASSY_SURGE, bank);
			break;
		
		case ABILITY_MISTYSURGE:
            effect = TryActivateTerrainAbility(MISTY_TERRAIN, B_ANIM_MISTY_SURGE, bank);
			break;
		
		case ABILITY_PSYCHICSURGE:
            effect = TryActivateTerrainAbility(PSYCHIC_TERRAIN, B_ANIM_PSYCHIC_SURGE, bank);
			break;
		}
		
		switch (gLastUsedAbility) { //These abilities should always activate if they can
			case ABILITY_FORECAST:
			case ABILITY_FLOWERGIFT:
			case ABILITYEFFECT_SWITCH_IN_WEATHER:
				break;
			default:
				gStatuses3[bank] |= STATUS3_SWITCH_IN_ABILITY_DONE;
		}
		
		break;
		
    case ABILITYEFFECT_ENDTURN: // 1
            if (gBattleMons[bank].hp != 0)
            {
                gBankAttacker = gActiveBattler = gBattleScripting->bank = bank;
                switch (gLastUsedAbility)
                {
                case ABILITY_RAINDISH:
                    if (WEATHER_HAS_EFFECT && (gBattleWeather & WEATHER_RAIN_ANY)
                    && gBattleMons[gActiveBattler].hp < gBattleMons[bank].maxHP)
                    {
                        BattleScriptPushCursorAndCallback(BattleScript_RainDishActivates);
                        gBattleMoveDamage = MathMax(1, gBattleMons[bank].maxHP / 16);
                        gBattleMoveDamage *= -1;
                        effect++;
                    }
                    break;
					
				case ABILITY_DRYSKIN:
					if (WEATHER_HAS_EFFECT) 
					{
						if ((gBattleWeather & WEATHER_RAIN_ANY) 
						&& gBattleMons[bank].hp < gBattleMons[bank].maxHP) 
						{
							gBattleMoveDamage = MathMax(1, gBattleMons[bank].maxHP / 8);
							gBattleMoveDamage *= -1;
							BattleScriptPushCursorAndCallback(BattleScript_RainDishActivates);
							effect++;
						}
						else if (gBattleWeather & WEATHER_SUN_ANY) 
						{
							gBattleMoveDamage = MathMax(1, gBattleMons[bank].maxHP / 8);
							BattleScriptPushCursorAndCallback(BattleScript_DrySkinDamage);
							effect++;			
						}
					}
					break;
				
				case ABILITY_ICEBODY:
					if (WEATHER_HAS_EFFECT && (gBattleWeather & WEATHER_HAIL_ANY)
					&& gBattleMons[bank].hp < gBattleMons[bank].maxHP) 
					{
						gBattleMoveDamage = MathMax(1, gBattleMons[bank].maxHP / 16);
						gBattleMoveDamage *= -1;
						BattleScriptExecute(BattleScript_RainDishActivates);
						effect++;
					}
					break;
				
				case ABILITY_SOLARPOWER:
					if (WEATHER_HAS_EFFECT && (gBattleWeather & WEATHER_SUN_ANY) 
					&& gBattleMons[bank].hp) 
					{
						gBattleMoveDamage = MathMax(1, gBattleMons[bank].maxHP / 8);
						BattleScriptExecute(BattleScript_SolarPowerDamage);
						effect++;
					}
					break;
				
                case ABILITY_SHEDSKIN:
                    if ((gBattleMons[bank].status1 & STATUS1_ANY) && (umodsi(Random(), 3)) == 0)
                    {
                        ClearBankStatus(bank);
                        BattleScriptPushCursorAndCallback(BattleScript_ShedSkinActivates);
                        effect++;
                    }
                    break;
								
				case ABILITY_HYDRATION:
					if (WEATHER_HAS_EFFECT && (gBattleWeather & WEATHER_RAIN_ANY)
					&& gBattleMons[bank].status1 & STATUS_ANY) 
					{
						ClearBankStatus(bank);
						BattleScriptPushCursorAndCallback(BattleScript_ShedSkinActivates);
						effect++;
					}
					break;
					
				case ABILITY_HEALER:
					if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE
					&& gBattleMons[PARTNER(bank)].hp
					&& gBattleMons[PARTNER(bank)].status1
					&& umodsi(Random(), 100) < 30) 
					{
						gBattleScripting->bank = PARTNER(bank);
						ClearBankStatus(gBattleScripting->bank);
						BattleScriptPushCursorAndCallback(BattleScript_Healer);
						effect++;
					}
					break;
				
                case ABILITY_SPEEDBOOST:
                    if (gBattleMons[bank].statStages[STAT_SPEED] < STAT_STAGE_MAX && gDisableStructs[bank].isFirstTurn != 2)
                    {
                        gBattleMons[bank].statStages[STAT_SPEED]++;
                        gBattleScripting->animArg1 = 0x11;
                        gBattleScripting->animArg2 = 0;
                        BattleScriptPushCursorAndCallback(BattleScript_SpeedBoostActivates);
                        effect++;
                    }
                    break;
				
				case ABILITY_MOODY: ;
					u8 statToRaise = 0;
					u8 statToLower = 0;
					u8* scriptPtr;
					if (StatsMinned(bank)) 
					{	//Raise One Stat
						statToRaise = umodsi(Random(), BATTLE_STATS_NO-1) + 1;
						gStatChangeByte = statToRaise | INCREASE_2;
						scriptPtr = BattleScript_MoodySingleStat;
					}
					else if (StatsMaxed(bank)) 
					{	//Lower One Stat
						statToLower = umodsi(Random(), BATTLE_STATS_NO-1) + 1;
						gStatChangeByte = statToLower | DECREASE_1;
						scriptPtr = BattleScript_MoodySingleStat;
					}
					else 
					{	//Raise One Stat and Lower Another	
						if (!(AllStatsButOneAreMinned(bank))) 
						{ //At least two non min stats
							do {
								statToRaise = umodsi(Random(), BATTLE_STATS_NO-1) + 1;			
							} while (gBattleMons[bank].statStages[statToRaise - 1] == 12);
										
						}
						else 
						{ //If all stats but one are at min, then raise one of the min ones so the
						  //non min one canned be lowered.
							do {
								statToRaise = umodsi(Random(), BATTLE_STATS_NO-1) + 1;			
							} while (gBattleMons[bank].statStages[statToRaise - 1] != 0);
						}
									
						do {
							statToLower = umodsi(Random(), BATTLE_STATS_NO-1) + 1;			
						} while (statToLower == statToRaise ||
						 gBattleMons[bank].statStages[statToLower - 1] == 0);
									
						gStatChangeByte = statToRaise | INCREASE_2;	
						SeedHelper[0] = statToLower;
						scriptPtr = BattleScript_MoodyRegular;
					}
					BattleScriptPushCursorAndCallback(scriptPtr);
					effect++;
					break;
				
				case (ABILITY_BADDREAMS):
					if (gBattleMons[FOE(bank)].status1 & STATUS_SLEEP
					|| ABILITY(FOE(bank)) == ABILITY_COMATOSE
					|| gBattleMons[PARTNER(FOE(bank))].status1 & STATUS_SLEEP
					|| ABILITY(PARTNER(FOE(bank))) == ABILITY_COMATOSE) 
					{
						BattleScriptPushCursorAndCallback(BattleScript_BadDreams);
						++effect;
					}
					break;
					
                case ABILITY_TRUANT:
                    gDisableStructs[gBankAttacker].truantCounter ^= 1;
                    break;
					
				case ABILITY_HARVEST:
					if (ITEM_POCKET(gNewBS->SavedConsumedItems[gBattlerPartyIndexes[bank]]) == POCKET_BERRY_POUCH) 
					{
						if (!(WEATHER_HAS_EFFECT && (gBattleWeather & WEATHER_SUN_ANY))) 
						{ //No Sun
							if (umodsi(Random(), 100) < 50)
								break;
						}
						BattleScriptPushCursorAndCallback(BattleScript_Harvest);
						++effect;
					}
					break;
							
				case ABILITY_PICKUP:
					for (u8 bank = 0; bank < 4; ++bank) 
					{
						if (bank != gActiveBattler 
						&& !(gNewBS->IncinerateCounters[bank]) 
						&& CONSUMED_ITEMS(bank)) 
						{
							gBattleMons[gActiveBattler].item = CONSUMED_ITEMS(bank);
							EmitSetMonData(0, REQUEST_HELDITEM_BATTLE, 0, 2, &gBattleMons[gActiveBattler].item);
							MarkBufferBankForExecution(gActiveBattler);
							gLastUsedItem = CONSUMED_ITEMS(bank);
							CONSUMED_ITEMS(bank) = 0;
							BattleScriptPushCursorAndCallback(BattleScript_Pickup);
							++effect;
							break;
						}
					}
					break;
				
				case ABILITY_SLOWSTART:
					if (gNewBS->SlowStartTimers[bank] && gBattleMons[bank].hp && --gNewBS->SlowStartTimers[bank] == 0) 
					{
						BattleStringLoader = gText_SlowStartEnd;
						BattleScriptPushCursorAndCallback(BattleScript_PrintCustomStringEnd3);
						++effect;
					}
                }
            }
            break;
		
        case ABILITYEFFECT_MOVES_BLOCK: // 2
			switch (gLastUsedAbility) {
				case ABILITY_SOUNDPROOF:
					if (CheckSoundMove(move))
						effect = 1;
					break;
				
				case ABILITY_BULLETPROOF:
					if (CheckTableForMove(move, BallBombMoveTable))
						effect = 1;
					break;
				
				case ABILITY_OVERCOAT:
					if (CheckTableForMove(move, PowderTable))
						effect = 1;
					break;
				
				case ABILITY_DAZZLING:
				case ABILITY_QUEENLYMAJESTY:
					if (PriorityCalc(gBankAttacker, ACTION_USE_MOVE, move) > 0)
						effect = 1;
					break;
				
				case ABILITY_TELEPATHY:
					if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE
					&& gBankAttacker == PARTNER(bank))
						effect = 1;
					break;
			}
			
			if (effect)
			{
				if (gBattleMons[gBankAttacker].status2 & STATUS2_MULTIPLETURNS)
					gHitMarker |= HITMARKER_NO_PPDEDUCT;
				gBattlescriptCurrInstr = BattleScript_SoundproofProtected;
			}
			break;
		
        case ABILITYEFFECT_ABSORBING: // 3
            if (!move) break;
			u8 statId = 0;
            switch (gLastUsedAbility) {
                case ABILITY_VOLTABSORB:
					if (moveType == TYPE_ELECTRIC)
						effect = 1;
					break;
				
				case ABILITY_WATERABSORB:
				case ABILITY_DRYSKIN:
					if (moveType == TYPE_WATER)
						effect = 1;
					break;
				
				case ABILITY_MOTORDRIVE:
					if (moveType == TYPE_ELECTRIC)
						effect = 2, statId = STAT_SPEED;
					break;
				
				case ABILITY_LIGHTNINGROD:
					if (moveType == TYPE_ELECTRIC)
						effect = 2, statId = STAT_SPATK;
					break;
				
				case ABILITY_STORMDRAIN:
					if (moveType == TYPE_WATER)
						effect = 2, statId = STAT_SPATK;
					break;
				
				case ABILITY_SAPSIPPER:
					if (moveType == TYPE_GRASS)
						effect = 2, statId = STAT_ATK;
					break;
				
                case ABILITY_FLASHFIRE:
                    if (moveType == TYPE_FIRE && !(gBattleMons[bank].status1 & STATUS1_FREEZE))
                    {
                        if (!(gBattleResources->flags->flags[bank] & RESOURCE_FLAG_FLASH_FIRE))
                        {
                            gBattleCommunication[MULTISTRING_CHOOSER] = 0;
                            if (gProtectStructs[gBankAttacker].notFirstStrike)
                                gBattlescriptCurrInstr = BattleScript_FlashFireBoost;
                            else
                                gBattlescriptCurrInstr = BattleScript_FlashFireBoost_PPLoss;

                            gBattleResources->flags->flags[bank] |= RESOURCE_FLAG_FLASH_FIRE;
                            effect = 2;
                        }
                        else
                        {
                            gBattleCommunication[MULTISTRING_CHOOSER] = 1;
                            if (gProtectStructs[gBankAttacker].notFirstStrike)
                                gBattlescriptCurrInstr = BattleScript_FlashFireBoost;
                            else
                                gBattlescriptCurrInstr = BattleScript_FlashFireBoost_PPLoss;

                            effect = 2;
                        }
                    }
                    break;
            }
			
            if (effect == 1) // Restore HP ability.
            {
                if (BATTLER_MAX_HP(bank) || gNewBS->HealBlockTimers[bank])
                {
                    if ((gProtectStructs[gBattlerAttacker].notFirstStrike))
                        gBattlescriptCurrInstr = BattleScript_MonMadeMoveUseless;
                    else
                        gBattlescriptCurrInstr = BattleScript_MonMadeMoveUseless_PPLoss;
                }
                else
                {
                    if (gProtectStructs[gBankAttacker].notFirstStrike)
                        gBattlescriptCurrInstr = BattleScript_MoveHPDrain;
                    else
                        gBattlescriptCurrInstr = BattleScript_MoveHPDrain_PPLoss;

                    gBattleMoveDamage = MathMax(1, gBattleMons[bank].maxHP / 4);
                    gBattleMoveDamage *= -1;
                }
            }
            else if (effect == 2) // Boost Stat ability;
            {
                if (gBattleMons[bank].statStages[statId - 1] >= STAT_STAGE_MAX)
                {
                    if ((gProtectStructs[gBankAttacker].notFirstStrike))
                        gBattlescriptCurrInstr = BattleScript_MonMadeMoveUseless;
                    else
                        gBattlescriptCurrInstr = BattleScript_MonMadeMoveUseless_PPLoss;
                }
                else
                {
                    if (gProtectStructs[gBankAttacker].notFirstStrike)
                        gBattlescriptCurrInstr = BattleScript_MoveStatDrain;
                    else
                        gBattlescriptCurrInstr = BattleScript_MoveStatDrain_PPLoss;

					gBattleScripting->statChanger = statId | INCREASE_1;
                    gBattleMons[bank].statStages[statId - 1]++;
                    PREPARE_STAT_BUFFER(gBattleTextBuff1, statId);
                }
            }
			
            break;
        case ABILITYEFFECT_CONTACT: //After being hit by a move. Not necessarilly contact.
			if (SheerForceCheck() && gLastUsedAbility != ABILITY_ILLUSION) //Sheer Force negates all these abilities
				break;
				
			gBattleScripting->bank = bank;
		
            switch (gLastUsedAbility)
            {
            case ABILITY_COLORCHANGE:
                if (MOVE_HAD_EFFECT
				&& TOOK_DAMAGE(bank)
                && move != MOVE_STRUGGLE
                && SPLIT(move) != SPLIT_STATUS
                && !IsOfType(bank, moveType)
                && gBattleMons[bank].hp
				&& gBankAttacker != bank)
                {
                    SET_BATTLER_TYPE(bank, moveType);
                    PREPARE_TYPE_BUFFER(gBattleTextBuff1, moveType);
                    BattleScriptPushCursor();
                    gBattlescriptCurrInstr = BattleScript_ColorChangeActivates;
                    effect++;
                }
                break;
			
			case ABILITY_IRONBARBS:
            case ABILITY_ROUGHSKIN:
                if (MOVE_HAD_EFFECT
				&& TOOK_DAMAGE(bank)
                && gBattleMons[gBankAttacker].hp
                && gBankAttacker != bank
				&& ABILITY(gBankAttacker) != ABILITY_MAGICGUARD
                && CheckContact(move, gBankAttacker))
                {
                    gBattleMoveDamage = MathMax(1, gBattleMons[gBankAttacker].maxHP / 16);
                    BattleScriptPushCursor();
                    gBattlescriptCurrInstr = BattleScript_RoughSkinActivates;
                    effect++;
                }
                break;
			
            case ABILITY_EFFECTSPORE:
                if (MOVE_HAD_EFFECT
				&& TOOK_DAMAGE(bank)
                && gBattleMons[gBankAttacker].hp
                && gBankAttacker != bank 
                && CheckContact(move, gBankAttacker)
				&& ABILITY(gBankAttacker) != ABILITY_OVERCOAT
				&& ITEM_EFFECT(gBankAttacker) != ITEM_EFFECT_SAFETY_GOGGLES
				&& !IsOfType(gBankAttacker, TYPE_GRASS)
                && umodsi(Random(), 10) == 0)
                {
                    do
                    {
                        gBattleCommunication[MOVE_EFFECT_BYTE] = Random() & 3;
                    } while (gBattleCommunication[MOVE_EFFECT_BYTE] == 0);

                    if (gBattleCommunication[MOVE_EFFECT_BYTE] == MOVE_EFFECT_BURN)
                        gBattleCommunication[MOVE_EFFECT_BYTE] += 2; // 5 MOVE_EFFECT_PARALYSIS

                    gBattleCommunication[MOVE_EFFECT_BYTE] |= MOVE_EFFECT_AFFECTS_USER;
                    BattleScriptPushCursor();
                    gBattlescriptCurrInstr = BattleScript_ApplySecondaryEffect;
                    gHitMarker |= HITMARKER_IGNORE_SAFEGUARD;
                    effect++;
                }
                break;
				
            case ABILITY_POISONPOINT:
                if (MOVE_HAD_EFFECT
				&& TOOK_DAMAGE(bank)
                && gBattleMons[gBankAttacker].hp
                && gBankAttacker != bank 
                && CheckContact(move, gBankAttacker)
				&& CanBePoisoned(gBankAttacker, gBankTarget)
                && umodsi(Random(), 3) == 0)
                {
                    gBattleCommunication[MOVE_EFFECT_BYTE] = MOVE_EFFECT_AFFECTS_USER | MOVE_EFFECT_POISON;
                    BattleScriptPushCursor();
                    gBattlescriptCurrInstr = BattleScript_ApplySecondaryEffect;
                    gHitMarker |= HITMARKER_IGNORE_SAFEGUARD;
                    effect++;
                }
                break;
			
            case ABILITY_STATIC:
                if (MOVE_HAD_EFFECT
				&& TOOK_DAMAGE(bank)
                && gBattleMons[gBankAttacker].hp
                && gBankAttacker != bank 
                && CheckContact(move, gBankAttacker)
				&& CanBeParalyzed(gBankAttacker)
                && umodsi(Random(), 3) == 0)
                {
                    gBattleCommunication[MOVE_EFFECT_BYTE] = MOVE_EFFECT_AFFECTS_USER | MOVE_EFFECT_PARALYSIS;
                    BattleScriptPushCursor();
                    gBattlescriptCurrInstr = BattleScript_ApplySecondaryEffect;
                    gHitMarker |= HITMARKER_IGNORE_SAFEGUARD;
                    effect++;
                }
                break;
			
            case ABILITY_FLAMEBODY:
                if (MOVE_HAD_EFFECT
				&& TOOK_DAMAGE(bank)
                && gBattleMons[gBankAttacker].hp
                && gBankAttacker != bank 
                && CheckContact(move, gBankAttacker)
				&& CanBeBurned(gBankAttacker)
                && umodsi(Random(), 3) == 0)
                {
                    gBattleCommunication[MOVE_EFFECT_BYTE] = MOVE_EFFECT_AFFECTS_USER | MOVE_EFFECT_BURN;
                    BattleScriptPushCursor();
                    gBattlescriptCurrInstr = BattleScript_ApplySecondaryEffect;
                    gHitMarker |= HITMARKER_IGNORE_SAFEGUARD;
                    effect++;
                }
                break;
			
            case ABILITY_CUTECHARM:
                if (MOVE_HAD_EFFECT
				&& TOOK_DAMAGE(bank)
                && gBattleMons[gBankAttacker].hp
				&& gBattleMons[bank].hp
                && gBankAttacker != bank
                && CheckContact(move, gBankAttacker)
                && umodsi(Random(), 3) == 0
                && ABILITY(gBankAttacker) != ABILITY_OBLIVIOUS
				&& ABILITY(gBankAttacker) != ABILITY_AROMAVEIL
				&& !(gBattleTypeFlags & BATTLE_TYPE_DOUBLE && ABILITY(PARTNER(gBankAttacker)) == ABILITY_AROMAVEIL)
                && GetGenderFromSpeciesAndPersonality(speciesAtk, pidAtk) != GetGenderFromSpeciesAndPersonality(speciesDef, pidDef)
                && !(gBattleMons[gBankAttacker].status2 & STATUS2_INFATUATION)
                && GetGenderFromSpeciesAndPersonality(speciesAtk, pidAtk) != MON_GENDERLESS
                && GetGenderFromSpeciesAndPersonality(speciesDef, pidDef) != MON_GENDERLESS)
                {
                    gBattleMons[gBankAttacker].status2 |= STATUS2_INFATUATED_WITH(gBankTarget);
                    BattleScriptPushCursor();
                    gBattlescriptCurrInstr = BattleScript_CuteCharmActivates;
                    effect++;
                }
                break;
				
			case ABILITY_JUSTIFIED:
				if (MOVE_HAD_EFFECT
				&& TOOK_DAMAGE(bank)
				&& gBattleMons[bank].hp
				&& gBankAttacker != bank
				&& moveType == TYPE_DARK
				&& gBattleMons[bank].statStages[STAT_ATK - 1] < 12)
				{
					gBattleScripting->statChanger = STAT_ATK | INCREASE_1;
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_TargetAbilityStatRaise;
					effect++;
				}
				break;	

			case ABILITY_RATTLED:
				if (MOVE_HAD_EFFECT
				&& TOOK_DAMAGE(bank)
				&& gBattleMons[bank].hp
				&& gBankAttacker != bank
				&& (moveType == TYPE_DARK || moveType == TYPE_BUG || moveType == TYPE_GHOST)
				&& gBattleMons[bank].statStages[STAT_SPEED - 1] < 12)
				{
					gBattleScripting->statChanger = STAT_SPEED | INCREASE_1;
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_TargetAbilityStatRaise;
					effect++;
				}
				break;				
			
			case ABILITY_WEAKARMOR:
				if (MOVE_HAD_EFFECT
				&& TOOK_DAMAGE(bank)
				&& gBattleMons[bank].hp
				&& gBankAttacker != bank
				&& (gBattleMons[bank].statStages[STAT_SPEED - 1] < 12 || gBattleMons[bank].statStages[STAT_DEF - 1] > 0))
				{
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_WeakArmorActivates;
					effect++;
				}
				break;
			
			case ABILITY_CURSEDBODY:
				if (MOVE_HAD_EFFECT
				&& TOOK_DAMAGE(bank)
				&& gBattleMons[bank].hp
				&& gBattleMons[gBankAttacker].hp
				&& gBankAttacker != bank
				&& ABILITY(gBankAttacker) != ABILITY_AROMAVEIL
				&& !(gBattleTypeFlags & BATTLE_TYPE_DOUBLE && ABILITY(PARTNER(gBankAttacker)) == ABILITY_AROMAVEIL)
				&& umodsi(Random(), 3) == 0)
				{
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_CursedBodyActivates;
					effect++;
				}
				break;
				
			case ABILITY_MUMMY:
				if (MOVE_HAD_EFFECT
				&& TOOK_DAMAGE(bank)
				&& gBattleMons[bank].hp
				&& gBattleMons[gBankAttacker].hp
				&& gBankAttacker != bank
                && CheckContact(move, gBankAttacker))
				{
					switch (gBattleMons[gBankAttacker].ability) {
						case ABILITY_MUMMY:
						case ABILITY_BATTLEBOND:
						case ABILITY_COMATOSE:
						case ABILITY_DISGUISE:
						case ABILITY_MULTITYPE:
						case ABILITY_POWERCONSTRUCT:
						case ABILITY_RKS_SYSTEM:
						case ABILITY_SCHOOLING:
						case ABILITY_SHIELDSDOWN:
						case ABILITY_STANCECHANGE:
							break;
						default:
							gLastUsedAbility = *GetAbilityLocation(gBankAttacker) = ABILITY_MUMMY;
							RecordAbilityBattle(gBankAttacker, gLastUsedAbility);
							RecordAbilityBattle(bank, gLastUsedAbility);
							BattleScriptPushCursor();
							gBattlescriptCurrInstr = BattleScript_MummyActivates;
							effect++;
							break;
					}
				}
				break;
				
			case ABILITY_ANGERPOINT:
				if (MOVE_HAD_EFFECT
				&& TOOK_DAMAGE(bank)
				&& gCritMultiplier > 100
				&& gBattleMons[bank].hp
				&& gBattleMons[bank].statStages[STAT_ATK - 1] < 12)
				{
					gBattleMons[bank].statStages[STAT_ATK - 1] = 12;
					PREPARE_STAT_BUFFER(gBattleTextBuff1, STAT_ATK);
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_AngerPointActivates;
					effect++;
				}
				break;
				
			case ABILITY_AFTERMATH:
                if (MOVE_HAD_EFFECT
				&& TOOK_DAMAGE(bank)
                && gBattleMons[gBankAttacker].hp
                && gBankAttacker != bank
				&& ABILITY(gBankAttacker) != ABILITY_MAGICGUARD
                && CheckContact(move, gBankAttacker)
				&& gBattleMons[bank].hp == 0
				&& !ABILITY_PRESENT(ABILITY_DAMP))
                {
                    gBattleMoveDamage = MathMax(1, gBattleMons[gBankAttacker].maxHP / 4);
                    BattleScriptPushCursor();
                    gBattlescriptCurrInstr = BattleScript_RoughSkinActivates;
                    effect++;
                }
                break;
			
			case ABILITY_BERSERK:
				if (MOVE_HAD_EFFECT
				&& TOOK_DAMAGE(bank)
				&& gBattleMons[bank].hp
				&& gBattleMons[bank].hp < gBattleMons[bank].maxHP / 2
				&& gBattleMons[bank].hp + gHpDealt > gBattleMons[bank].maxHP / 2 //Hp fell below half
				&& gBattleMons[bank].statStages[STAT_SPATK - 1] < 12)
				{
					gBattleScripting->statChanger = STAT_SPATK | INCREASE_1;
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_TargetAbilityStatRaise;
					effect++;
				}
				break;
			
			case ABILITY_TANGLINGHAIR:
			case ABILITY_GOOEY:
				if (MOVE_HAD_EFFECT
				&& TOOK_DAMAGE(bank)
				&& gBattleMons[gBankAttacker].hp
				&& gBankAttacker != bank
				&& STAT_CAN_FALL(gBankAttacker, STAT_SPD))
				{
					gBattleScripting->statChanger = STAT_SPD | DECREASE_1;
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_GooeyActivates;
					effect++;
				}
				break;
			
			case ABILITY_INNARDSOUT:
                if (MOVE_HAD_EFFECT
				&& TOOK_DAMAGE(bank)
                && gBattleMons[gBankAttacker].hp
                && gBankAttacker != bank
				&& ABILITY(gBankAttacker) != ABILITY_MAGICGUARD
				&& gBattleMons[bank].hp == 0)
                {
                    gBattleMoveDamage = MathMax(1, gHpDealt);
                    BattleScriptPushCursor();
                    gBattlescriptCurrInstr = BattleScript_RoughSkinActivates;
                    effect++;
                }
                break;
			
			case ABILITY_STAMINA:
				if (MOVE_HAD_EFFECT
				&& TOOK_DAMAGE(bank)
				&& gBattleMons[bank].hp
				&& gBankAttacker != bank
				&& gBattleMons[bank].statStages[STAT_DEF - 1] < 12)
				{
					gBattleScripting->statChanger = STAT_DEF | INCREASE_1;
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_TargetAbilityStatRaise;
					effect++;
				}
				break;
		
			case ABILITY_WATERCOMPACTION:
				if (MOVE_HAD_EFFECT
				&& TOOK_DAMAGE(bank)
				&& gBattleMons[bank].hp
				&& gBankAttacker != bank
				&& moveType == TYPE_WATER
				&& gBattleMons[bank].statStages[STAT_DEF - 1] < 12)
				{
					gBattleScripting->statChanger = STAT_DEF | INCREASE_2;
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_TargetAbilityStatRaise;
					effect++;
				}
				break;
				
			case ABILITY_ILLUSION:
				if (MOVE_HAD_EFFECT
				&& TOOK_DAMAGE(bank)
				&& gStatuses3[bank] & STATUS3_ILLUSION)
				{
					gNewBS->IllusionBroken |= gBitTable[bank];
					gStatuses3[bank] &= ~(STATUS3_ILLUSION);
					gActiveBattler = bank;
					EmitDataTransfer(0, &gStatuses3[gActiveBattler], 4, &gStatuses3[gActiveBattler]);
					MarkBufferBankForExecution(gActiveBattler);
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_IllusionBroken;
					effect++;
				}
				break;
            }
            break;
			
        case ABILITYEFFECT_IMMUNITY: // 5
            for (bank = 0; bank < gBattlersCount; bank++)
            {
                switch (ABILITY(bank))
                {
                case ABILITY_IMMUNITY:
                    if (gBattleMons[bank].status1 & (STATUS1_PSN_ANY))
                    {
                        StringCopy(gBattleTextBuff1, gStatusConditionString_Poison);
                        effect = 1;
                    }
                    break;
                case ABILITY_OWNTEMPO:
                    if (gBattleMons[bank].status2 & STATUS2_CONFUSION)
                    {
                        StringCopy(gBattleTextBuff1, gStatusConditionString_Confusion);
                        effect = 2;
                    }
                    break;
                case ABILITY_LIMBER:
                    if (gBattleMons[bank].status1 & STATUS1_PARALYSIS)
                    {
                        StringCopy(gBattleTextBuff1, gStatusConditionString_Paralysis);
                        effect = 1;
                    }
                    break;
                case ABILITY_INSOMNIA:
                case ABILITY_VITALSPIRIT:
                    if (gBattleMons[bank].status1 & STATUS1_SLEEP)
                    {
                        gBattleMons[bank].status2 &= ~(STATUS2_NIGHTMARE);
                        StringCopy(gBattleTextBuff1, gStatusConditionString_Sleep);
                        effect = 1;
                    }
                    break;
                case ABILITY_WATERVEIL:
                    if (gBattleMons[bank].status1 & STATUS1_BURN)
                    {
                        StringCopy(gBattleTextBuff1, gStatusConditionString_Burn);
                        effect = 1;
                    }
                    break;
                case ABILITY_MAGMAARMOR:
                    if (gBattleMons[bank].status1 & STATUS1_FREEZE)
                    {
                        StringCopy(gBattleTextBuff1, gStatusConditionString_Ice);
                        effect = 1;
                    }
                    break;
                case ABILITY_OBLIVIOUS: //TODO Add Taunt
                    if (gBattleMons[bank].status2 & STATUS2_INFATUATION)
                    {
						if (gDisableStructs[bank].tauntTimer)
							StringCopy(gBattleTextBuff1, gStatusConditionString_MentalState);
						else
							StringCopy(gBattleTextBuff1, gStatusConditionString_Love);
                        effect = 3;
                    }
					else if (gDisableStructs[bank].tauntTimer) 
					{
						StringCopy(gBattleTextBuff1, gStatusConditionString_TauntProblem);
						effect = 3;
					}
				}
				
                if (effect)
                {
                    switch (effect)
                    {
                    case 1: // status cleared
                        gBattleMons[bank].status1 = 0;
                        break;
                    case 2: // get rid of confusion
                        gBattleMons[bank].status2 &= ~(STATUS2_CONFUSION);
                        break;
                    case 3: // get rid of infatuation and taunt
						gDisableStructs[gActiveBattler].tauntTimer = 0;
                        gBattleMons[bank].status2 &= ~(STATUS2_INFATUATION);
                        break;
                    }

                    BattleScriptPushCursor();
                    gBattlescriptCurrInstr = BattleScript_AbilityCuredStatus;
                    gActiveBattler = gBattleScripting->bank = bank;
                    EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 4, &gBattleMons[gActiveBattler].status1);
                    MarkBufferBankForExecution(gActiveBattler);
                    return effect;
                }
            }
            break;
        case ABILITYEFFECT_FORECAST: // 6
            for (bank = 0; bank < gBattlersCount; bank++)
            {
                if (ABILITY(bank) == ABILITY_FORECAST)
                {
                    effect = CastformDataTypeChange(bank);
                    if (effect)
                    {
                        BattleScriptPushCursorAndCallback(BattleScript_CastformChange);
                        gBattleScripting->bank = bank;
                        gBattleStruct->castformToChangeInto = effect - 1;
                        return effect;
                    }
                }
            }
            break;
        case ABILITYEFFECT_SYNCHRONIZE: // 7
            if (gLastUsedAbility == ABILITY_SYNCHRONIZE && (gHitMarker & HITMARKER_SYNCHRONISE_EFFECT))
            {
                gHitMarker &= ~(HITMARKER_SYNCHRONISE_EFFECT);
                gBattleStruct->synchronizeMoveEffect &= ~(MOVE_EFFECT_AFFECTS_USER | MOVE_EFFECT_CERTAIN);
                if (gBattleStruct->synchronizeMoveEffect == MOVE_EFFECT_TOXIC)
                    gBattleStruct->synchronizeMoveEffect = MOVE_EFFECT_TOXIC;

                gBattleCommunication[MOVE_EFFECT_BYTE] = gBattleStruct->synchronizeMoveEffect + MOVE_EFFECT_AFFECTS_USER;
                gBattleScripting->bank = gBankTarget;
                BattleScriptPushCursor();
                gBattlescriptCurrInstr = BattleScript_SynchronizeActivates_StatusesAttacker;
                gHitMarker |= HITMARKER_IGNORE_SAFEGUARD;
                effect++;
            }
            break;
        case ABILITYEFFECT_ATK_SYNCHRONIZE: // 8
            if (gLastUsedAbility == ABILITY_SYNCHRONIZE && (gHitMarker & HITMARKER_SYNCHRONISE_EFFECT))
            {
                gHitMarker &= ~(HITMARKER_SYNCHRONISE_EFFECT);
                gBattleStruct->synchronizeMoveEffect &= ~(MOVE_EFFECT_AFFECTS_USER | MOVE_EFFECT_CERTAIN);
                if (gBattleStruct->synchronizeMoveEffect == MOVE_EFFECT_TOXIC)
                    gBattleStruct->synchronizeMoveEffect = MOVE_EFFECT_TOXIC;

                gBattleCommunication[MOVE_EFFECT_BYTE] = gBattleStruct->synchronizeMoveEffect;
                gBattleScripting->bank = gBankAttacker;
                BattleScriptPushCursor();
                gBattlescriptCurrInstr = BattleScript_SynchronizeActivates_StatusesTarget;
                gHitMarker |= HITMARKER_IGNORE_SAFEGUARD;
                effect++;
            }
            break;
        case ABILITYEFFECT_INTIMIDATE1: // 9
            break;
		
        case ABILITYEFFECT_TRACE: // 11
            break;
		
        case ABILITYEFFECT_INTIMIDATE2: // 10
            break;
		
        case ABILITYEFFECT_CHECK_OTHER_SIDE: // 12
            side = SIDE(bank);
            for (i = 0; i < gBattlersCount; i++)
            {
                if (SIDE(i) != side && ABILITY(i) == ability)
                {
                    gLastUsedAbility = ability;
                    effect = i + 1;
                }
            }
            break;
        case ABILITYEFFECT_CHECK_BANK_SIDE: // 13
            side = SIDE(bank);
            for (i = 0; i < gBattlersCount; i++)
            {
                if (SIDE(i) == side && ABILITY(i) == ability)
                {
                    gLastUsedAbility = ability;
                    effect = i + 1;
                }
            }
            break;
        case ABILITYEFFECT_FIELD_SPORT: // 14
            switch (gLastUsedAbility) {
				case 0xFD:
					effect = gNewBS->MudSportTimer;
					break;
				case 0xFE:
					effect = gNewBS->WaterSportTimer;
					break;
				default:
					for (i = 0; i < gBattlersCount; i++)
					{
						if (ABILITY(i) == ability)
						{
							gLastUsedAbility = ability;
							effect = i + 1;
						}
					}
					break;
            }
            break;
        case ABILITYEFFECT_CHECK_ON_FIELD: // 19
            for (i = 0; i < gBattlersCount; i++)
            {
                if (ABILITY(i) == ability && gBattleMons[i].hp)
                {
                    gLastUsedAbility = ability;
                    effect = i + 1;
                }
            }
            break;
        case ABILITYEFFECT_CHECK_FIELD_EXCEPT_BANK: // 15
            for (i = 0; i < gBattlersCount; i++)
            {
                if (ABILITY(i) == ability && i != bank)
                {
                    gLastUsedAbility = ability;
                    effect = i + 1;
                }
            }
            break;
        case ABILITYEFFECT_COUNT_OTHER_SIDE: // 16
            side = SIDE(bank);
            for (i = 0; i < gBattlersCount; i++)
            {
                if (SIDE(i) != side && ABILITY(i) == ability)
                {
                    gLastUsedAbility = ability;
                    effect++;
                }
            }
            break;
        case ABILITYEFFECT_COUNT_BANK_SIDE: // 17
            side = SIDE(bank);
            for (i = 0; i < gBattlersCount; i++)
            {
                if (SIDE(i) == side && ABILITY(i) == ability)
                {
                    gLastUsedAbility = ability;
                    effect++;
                }
            }
            break;
        case ABILITYEFFECT_COUNT_ON_FIELD: // 18
            for (i = 0; i < gBattlersCount; i++)
            {
                if (ABILITY(i) == ability && i != bank)
                {
                    gLastUsedAbility = ability;
                    effect++;
                }
            }
            break;
        }

    if (effect && caseID < ABILITYEFFECT_CHECK_OTHER_SIDE && gLastUsedAbility != 0xFF)
        RecordAbilityBattle(bank, gLastUsedAbility);

    return effect;
}

u8 CalcMovePowerForForewarn(u16 move) 
{
	u8 power;
	
	if (gBattleMoves[move].effect == EFFECT_0HKO)
	{
		power = 160;
	}
	else
	{	
		switch (move) {
			case MOVE_COUNTER:
			case MOVE_METALBURST:
			case MOVE_MIRRORCOAT:
				power = 120;
				break;
			
			case MOVE_CRUSHGRIP:
			case MOVE_DRAGONRAGE:
			case MOVE_ELECTROBALL:
			case MOVE_ENDEAVOR:
			case MOVE_FINALGAMBIT:
			case MOVE_FLAIL:
			case MOVE_FRUSTRATION:
			case MOVE_GRASSKNOT:
			case MOVE_GYROBALL:
			case MOVE_HEATCRASH:
			case MOVE_HEAVYSLAM:
			case MOVE_HIDDENPOWER:
			case MOVE_LOWKICK:
			case MOVE_NATURALGIFT:
			case MOVE_NIGHTSHADE:
			case MOVE_PSYWAVE:
			case MOVE_RETURN:
			case MOVE_REVERSAL:
			case MOVE_SEISMICTOSS:
			case MOVE_SONICBOOM:
			case MOVE_TRUMPCARD:
			case MOVE_WRINGOUT:
				power = 80;
				break;
			
			default:
				power = gBattleMoves[move].power;
		}
	}
	return power;
}

u8 ActivateWeatherAbility(u16 flags, u16 item, u8 bank, u8* script)
{
	#ifdef INFINITE_WEATHER
		gBattleWeather = flags;
		gWishFutureKnock->weatherDuration = 0;
		++item; //So it compiles warning free
	#else
		gBattleWeather = flags;
		if (ITEM_EFFECT(bank) == item)
			gWishFutureKnock->weatherDuration = 8;
		else
			gWishFutureKnock->weatherDuration = 5;
	#endif
    BattleScriptPushCursorAndCallback(script);
    gBattleScripting->bank = bank;
    return TRUE;
}
				
u8 TryActivateTerrainAbility(u8 terrain, u8 anim, u8 bank)
{
	u8 effect = 0;
	
	if (TerrainType != terrain)
	{
		TerrainType = terrain;
		#ifdef INFINITE_TERRAIN
			gNewBS->TerrainTimer = 0;
		#else
			if (ITEM_EFFECT(bank) == ITEM_EFFECT_TERRAIN_EXTENDER)
				gNewBS->TerrainTimer = 8;
			else
				gNewBS->TerrainTimer = 5;
		#endif
		
		switch (terrain) {
			case ELECTRIC_TERRAIN:
				BattleStringLoader = ElectricTerrainSetString;
				break;
			case GRASSY_TERRAIN:
				BattleStringLoader = GrassyTerrainSetString;
				break;
			case MISTY_TERRAIN:
				BattleStringLoader = MistyTerrainSetString;
				break;
			case PSYCHIC_TERRAIN:
				BattleStringLoader = PsychicTerrainSetString;
				break;
		}
		
		gBattleScripting->animArg1 = anim;
        gBattleScripting->bank = bank;
		*((u32*) SeedHelper) = (u32) BattleStringLoader; //Backup String
		BattleScriptPushCursorAndCallback(BattleScript_TerrainFromAbility);
		++effect;
	}
	return effect;
}

bool8 ImmunityAbilityCheck(u8 bank, u32 status, u8* string)
{
	
	if (gBattleMons[bank].status1 & status)
	{
        StringCopy(gBattleTextBuff1, string);
		gBattleMons[bank].status1 = 0;
		gBattleMons[bank].status2 &= ~(STATUS2_NIGHTMARE);
		BattleScriptPushCursorAndCallback(BattleScript_AbilityCuredStatusEnd3);
		gBattleScripting->bank = bank;
        gActiveBattler = bank;
        EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 4, &gBattleMons[gActiveBattler].status1);
        MarkBufferBankForExecution(gActiveBattler);
        return TRUE;
    }
	return FALSE;
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

//Illusion Updates////////////////////////////////////////////////////////////////////////////////////
extern u8 ItemIdToBallId(u16 ballItem);
extern item_t BallIdToItemId(u8 ballId);
extern void LoadBallGfx(u8 ballId);

pokemon_t* UpdateNickForIllusion(pokemon_t* mon)
{
	u8 bank = GetBankFromPartyData(mon);
	
	if (bank >= 6) //Safety Measure
		return mon;
	
	return GetIllusionPartyData(bank);
}

u8 UpdatePokeBallForIllusion(u8 bank)
{
	return GetMonData(GetIllusionPartyData(bank), MON_DATA_POKEBALL, NULL);
}

//Ability Pop-Up//////////////////////////////////////////////////////////////////////////////////////
//Credit to the Emerald Battle Engine Upgrade 2.0 by DizzyEgg for the original code.

// for sprite
#define tOriginalX      data[0]
#define tHide           data[1]
#define tFrames         data[2]
#define tRightToLeft    data[3]
#define tBattlerId      data[4]
#define tIsMain         data[5]

// for task
#define tSpriteId1      data[6]
#define tSpriteId2      data[7]

extern const struct CompressedSpriteSheet gBattleAnimPicTable[];
extern const struct CompressedSpritePalette gBattleAnimPaletteTable[];
extern const u8 Ability_Pop_UpTiles[64 * 64 / 2];

static void SpriteCb_AbilityPopUp(struct Sprite *sprite);
static void Task_FreeAbilityPopUpGfx(u8 taskId);

static const struct OamData sOamData_AbilityPopUp =
{
    .y = 0,
    .affineMode = 0,
    .objMode = 0,
    .mosaic = 0,
    .bpp = 0,
    .shape = ST_OAM_H_RECTANGLE,
    .x = 0,
    .matrixNum = 0,
    .size = 3,
    .tileNum = 0,
    .priority = 0,
    .paletteNum = 0,
    .affineParam = 0,
};

static const union AnimCmd sSpriteAnim_AbilityPopUp1[] =
{
    ANIMCMD_FRAME(0, 0),
    ANIMCMD_END
};

static const union AnimCmd *const sSpriteAnimTable_AbilityPopUp1[] =
{
    sSpriteAnim_AbilityPopUp1
};

static const struct SpriteTemplate sSpriteTemplate_AbilityPopUp1 =
{
    .tileTag = ANIM_TAG_ABILITY_POP_UP,
    .paletteTag = ANIM_TAG_ABILITY_POP_UP,
    .oam = &sOamData_AbilityPopUp,
    .anims = sSpriteAnimTable_AbilityPopUp1,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCb_AbilityPopUp
};

static const union AnimCmd sSpriteAnim_AbilityPopUp2[] =
{
    ANIMCMD_FRAME(32, 0),
    ANIMCMD_END
};

static const union AnimCmd *const sSpriteAnimTable_AbilityPopUp2[] =
{
    sSpriteAnim_AbilityPopUp2
};

static const struct SpriteTemplate sSpriteTemplate_AbilityPopUp2 =
{
    .tileTag = ANIM_TAG_ABILITY_POP_UP,
    .paletteTag = ANIM_TAG_ABILITY_POP_UP,
    .oam = &sOamData_AbilityPopUp,
    .anims = sSpriteAnimTable_AbilityPopUp2,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCb_AbilityPopUp
};

#define ABILITY_POP_UP_POS_X_DIFF 64
#define ABILITY_POP_UP_POS_X_SLIDE 68

static const s16 sAbilityPopUpCoordsDoubles[MAX_BATTLERS_COUNT][2] =
{
    {29, 80}, // player left
    {186, 19}, // opponent left
    {29, 97}, // player right
    {186, 36}, // opponent right
};

static const s16 sAbilityPopUpCoordsSingles[MAX_BATTLERS_COUNT][2] =
{
    {29, 97}, // player
    {186, 57}, // opponent
};

static u8* AddTextPrinterAndCreateWindowOnAbilityPopUp(const u8* str, u32 x, u32 y, u32 color1, u32 color2, u32 color3, u32* windowId)
{
    u8 color[3] = {color1, color2, color3};
    struct WindowTemplate winTemplate = {0};
    winTemplate.width = 8;
    winTemplate.height = 2;

    *windowId = AddWindow(&winTemplate);
    FillWindowPixelBuffer(*windowId, (color1 << 4) | (color1));

    AddTextPrinterParameterized4(*windowId, 0, x, y, 0, 0, color, -1, str);
    return (u8*)(GetWindowAttribute(*windowId, WINDOW_TILE_DATA));
}

static void TextIntoAbilityPopUp(void *dest, u8 *windowTileData, s32 arg2, bool32 arg3)
{
    CpuCopy32(windowTileData + 256, dest + 256, arg2 * 32);
    if (arg2 > 0)
    {
        do
        {
            if (arg3)
                CpuCopy32(windowTileData + 16, dest + 16, 16);
            else
                CpuCopy32(windowTileData + 20, dest + 20, 12);
            dest += 32, windowTileData += 32;
            arg2--;
        } while (arg2 != 0);
    }
}

#define MAX_CHARS_PRINTED 12

static void PrintOnAbilityPopUp(const u8* str, u8* spriteTileData1, u8* spriteTileData2, u32 x1, u32 x2, u32 y, u32 color1, u32 color2, u32 color3)
{
    u32 windowId, i;
    u8 *windowTileData;
    u8 text1[MAX_CHARS_PRINTED];
    u8 text2[MAX_CHARS_PRINTED];

    for (i = 0; i < MAX_CHARS_PRINTED; i++)
    {
        text1[i] = str[i];
        if (text1[i] == EOS)
            break;
    }
    text1[i] = EOS;

    windowTileData = AddTextPrinterAndCreateWindowOnAbilityPopUp(text1, x1, y, color1, color2, color3, &windowId);
    TextIntoAbilityPopUp(spriteTileData1, windowTileData, 8, (y == 0));
    RemoveWindow(windowId);

    if (i == MAX_CHARS_PRINTED)
    {
        for (i = 0; i < MAX_CHARS_PRINTED; i++)
        {
            text2[i] = str[MAX_CHARS_PRINTED + i];
            if (text2[i] == EOS)
                break;
        }
        text2[i] = EOS;

        windowTileData = AddTextPrinterAndCreateWindowOnAbilityPopUp(text2, x2, y, color1, color2, color3, &windowId);
        TextIntoAbilityPopUp(spriteTileData2, windowTileData, 3, (y == 0));
        RemoveWindow(windowId);
    }
}

static void PrintBattlerOnAbilityPopUp(u8 battlerId, u8 spriteId1, u8 spriteId2)
{
	u8* textPtr = StringCopy(gDisplayedStringBattle, GetIllusionPartyData(battlerId)->nickname);
	
	//Make the string say "[NAME]'s" instead of "[NAME]"
	textPtr[0] = 0xB4; //'
	++textPtr;
	textPtr[0] = 0xE7; //s
	++textPtr;
	textPtr[0] = EOS;

    PrintOnAbilityPopUp((const u8*) gDisplayedStringBattle,
                        (void*)(OBJ_VRAM0) + (gSprites[spriteId1].oam.tileNum * 32),
                        (void*)(OBJ_VRAM0) + (gSprites[spriteId2].oam.tileNum * 32),
                        5, 0,
                        0,
                        2, 7, 1);
}

static void PrintAbilityOnAbilityPopUp(u32 ability, u8 spriteId1, u8 spriteId2)
{
	const u8* abilityName = gAbilityNames[ability];
	
	if (abilityName[3] == 0x8) //Expanded Ability Name
		abilityName = T2_READ_PTR(abilityName);

    PrintOnAbilityPopUp(abilityName,
                        (void*)(OBJ_VRAM0) + (gSprites[spriteId1].oam.tileNum * 32) + 256,
                        (void*)(OBJ_VRAM0) + (gSprites[spriteId2].oam.tileNum * 32) + 256,
                        5, 1,
                        4,
                        7, 9, 1);
}

#define PIXEL_COORDS_TO_OFFSET(x, y)(			\
/*Add tiles by X*/								\
((y / 8) * 32 * 8)								\
/*Add tiles by X*/								\
+ ((x / 8) * 32)								\
/*Add pixels by Y*/								\
+ ((((y) - ((y / 8) * 8))) * 4)				    \
/*Add pixels by X*/								\
+ ((((x) - ((x / 8) * 8)) / 2)))

static const u16 sOverwrittenPixelsTable[][2] =
{
	{PIXEL_COORDS_TO_OFFSET(0, 0), 5},
	{PIXEL_COORDS_TO_OFFSET(0, 1), 5},
	{PIXEL_COORDS_TO_OFFSET(0, 2), 5},
	{PIXEL_COORDS_TO_OFFSET(0, 3), 5},
	{PIXEL_COORDS_TO_OFFSET(0, 4), 5},
	{PIXEL_COORDS_TO_OFFSET(0, 5), 5},
	{PIXEL_COORDS_TO_OFFSET(0, 6), 5},
	{PIXEL_COORDS_TO_OFFSET(0, 7), 3},
	{PIXEL_COORDS_TO_OFFSET(0, 8), 3},
	{PIXEL_COORDS_TO_OFFSET(0, 9), 3},
	{PIXEL_COORDS_TO_OFFSET(0, 10), 3},
	{PIXEL_COORDS_TO_OFFSET(0, 11), 3},
	{PIXEL_COORDS_TO_OFFSET(0, 12), 3},
	{PIXEL_COORDS_TO_OFFSET(0, 13), 8},

	{PIXEL_COORDS_TO_OFFSET(8, 13), 8},
	{PIXEL_COORDS_TO_OFFSET(16, 13), 8},
	{PIXEL_COORDS_TO_OFFSET(24, 13), 8},
	{PIXEL_COORDS_TO_OFFSET(32, 13), 8},
	{PIXEL_COORDS_TO_OFFSET(40, 13), 8},
	{PIXEL_COORDS_TO_OFFSET(48, 13), 8},
	{PIXEL_COORDS_TO_OFFSET(56, 13), 8},

    {PIXEL_COORDS_TO_OFFSET(0, 14), 8},
	{PIXEL_COORDS_TO_OFFSET(8, 14), 8},
	{PIXEL_COORDS_TO_OFFSET(16, 14), 8},
	{PIXEL_COORDS_TO_OFFSET(24, 14), 8},
	{PIXEL_COORDS_TO_OFFSET(32, 14), 8},
	{PIXEL_COORDS_TO_OFFSET(40, 14), 8},
	{PIXEL_COORDS_TO_OFFSET(48, 14), 8},
	{PIXEL_COORDS_TO_OFFSET(56, 14), 8},
	
	{PIXEL_COORDS_TO_OFFSET(0, 15), 3},
	{PIXEL_COORDS_TO_OFFSET(0, 16), 3},
	{PIXEL_COORDS_TO_OFFSET(0, 17), 3},
	{PIXEL_COORDS_TO_OFFSET(0, 18), 3},
	{PIXEL_COORDS_TO_OFFSET(0, 19), 3},
	{PIXEL_COORDS_TO_OFFSET(0, 20), 3},
	{PIXEL_COORDS_TO_OFFSET(0, 21), 3},
	{PIXEL_COORDS_TO_OFFSET(0, 22), 3},
	{PIXEL_COORDS_TO_OFFSET(0, 23), 3},
	{PIXEL_COORDS_TO_OFFSET(0, 24), 3},
	{PIXEL_COORDS_TO_OFFSET(0, 25), 3},
	{PIXEL_COORDS_TO_OFFSET(0, 26), 3},
	
//Second Row Of Image
	{PIXEL_COORDS_TO_OFFSET(0, 45), 8},
	{PIXEL_COORDS_TO_OFFSET(0, 46), 8},
	{PIXEL_COORDS_TO_OFFSET(8, 45), 8},
	{PIXEL_COORDS_TO_OFFSET(8, 46), 8},
	{PIXEL_COORDS_TO_OFFSET(16, 45), 8},
	{PIXEL_COORDS_TO_OFFSET(16, 46), 8},
};

static inline void CopyPixels(u8 *dest, const u8 *src, u32 pixelCount)
{
	u32 i = 0;

	if (pixelCount & 1)
	{
		while (pixelCount != 0)
		{
			dest[i] &= ~(0xF);
			dest[i] |= (src[i] & 0xF);
			if (--pixelCount != 0)
			{
				dest[i] &= ~(0xF0);
				dest[i] |= (src[i] & 0xF0);
				pixelCount--;
			}
			i++;
		}
	}
	else
	{
		for (i = 0; i < pixelCount / 2; i++)
			dest[i] = src[i];
	}
}

static void RestoreOverwrittenPixels(u8 *tiles)
{
	u32 i;
	u8 *buffer = Malloc(sizeof(Ability_Pop_UpTiles) * 2);

	CpuCopy32(tiles, buffer, sizeof(Ability_Pop_UpTiles));

	for (i = 0; i < ARRAY_COUNT(sOverwrittenPixelsTable); i++)
	{
		CopyPixels(buffer + sOverwrittenPixelsTable[i][0],
				   Ability_Pop_UpTiles + sOverwrittenPixelsTable[i][0],
				   sOverwrittenPixelsTable[i][1]);
	}

	CpuCopy32(buffer, tiles, sizeof(Ability_Pop_UpTiles));
	Free(buffer);
}

void AnimTask_LoadAbilityPopUp(u8 taskId)
{
    const s16 (*coords)[2];
    u8 spriteId1, spriteId2, battlerPosition, destroyerTaskId;
	u8 ability = AbilityPopUpHelper; //Preceded by transfer of proper ability

	LoadSpriteSheet((const struct SpriteSheet*) &gBattleAnimPicTable[ANIM_TAG_ABILITY_POP_UP - ANIM_SPRITES_START]);
    LoadSpritePalette((const struct SpritePalette*) &gBattleAnimPaletteTable[ANIM_TAG_ABILITY_POP_UP - ANIM_SPRITES_START]);
		
    gNewBS->activeAbilityPopUps |= gBitTable[gBattleAnimAttacker];
    battlerPosition = GetBattlerPosition(gBattleAnimAttacker);

    if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
        coords = sAbilityPopUpCoordsDoubles;
    else
        coords = sAbilityPopUpCoordsSingles;

    if ((battlerPosition & BIT_SIDE) == B_SIDE_PLAYER)
    {
        spriteId1 = CreateSprite(&sSpriteTemplate_AbilityPopUp1,
                                coords[battlerPosition][0] - ABILITY_POP_UP_POS_X_SLIDE,
                                coords[battlerPosition][1], 0);
        spriteId2 = CreateSprite(&sSpriteTemplate_AbilityPopUp2,
                                coords[battlerPosition][0] - ABILITY_POP_UP_POS_X_SLIDE + ABILITY_POP_UP_POS_X_DIFF,
                                coords[battlerPosition][1], 0);

        gSprites[spriteId1].tOriginalX = coords[battlerPosition][0];
        gSprites[spriteId2].tOriginalX = coords[battlerPosition][0] + ABILITY_POP_UP_POS_X_DIFF;

        gSprites[spriteId1].tRightToLeft = TRUE;
        gSprites[spriteId2].tRightToLeft = TRUE;
    }
    else
    {
        spriteId1 = CreateSprite(&sSpriteTemplate_AbilityPopUp1,
                                coords[battlerPosition][0] + ABILITY_POP_UP_POS_X_SLIDE,
                                coords[battlerPosition][1], 0);
        spriteId2 = CreateSprite(&sSpriteTemplate_AbilityPopUp2,
                                coords[battlerPosition][0] + ABILITY_POP_UP_POS_X_SLIDE + ABILITY_POP_UP_POS_X_DIFF,
                                coords[battlerPosition][1], 0);

        gSprites[spriteId1].tOriginalX = coords[battlerPosition][0];
        gSprites[spriteId2].tOriginalX = coords[battlerPosition][0] + ABILITY_POP_UP_POS_X_DIFF;

        gSprites[spriteId1].tRightToLeft = FALSE;
        gSprites[spriteId2].tRightToLeft = FALSE;
    }

	gNewBS->abilityPopUpIds[0] = spriteId1;
	gNewBS->abilityPopUpIds[1] = spriteId2;
	
    destroyerTaskId = CreateTask(Task_FreeAbilityPopUpGfx, 5);
    gTasks[destroyerTaskId].tSpriteId1 = spriteId1;
    gTasks[destroyerTaskId].tSpriteId2 = spriteId2;

    gSprites[spriteId1].tIsMain = TRUE;
    gSprites[spriteId1].tBattlerId = gBattleAnimAttacker;
    gSprites[spriteId2].tBattlerId = gBattleAnimAttacker;

    StartSpriteAnim(&gSprites[spriteId1], 0);
    StartSpriteAnim(&gSprites[spriteId2], 0);


    PrintBattlerOnAbilityPopUp(gBattleAnimAttacker, spriteId1, spriteId2);
    PrintAbilityOnAbilityPopUp(ability, spriteId1, spriteId2);
    RestoreOverwrittenPixels((void*)(OBJ_VRAM0) + (gSprites[spriteId1].oam.tileNum * 32));
	
	DestroyAnimVisualTask(taskId);
}

static void SpriteCb_AbilityPopUp(struct Sprite* sprite)
{
    if (!sprite->tHide) // Show
    {
        if (sprite->tIsMain && ++sprite->tFrames == 4)
            PlaySE(0x80); //Also could be 0xBA
        if ((!sprite->tRightToLeft && (sprite->pos1.x -= 4) <= sprite->tOriginalX)
        ||   (sprite->tRightToLeft && (sprite->pos1.x += 4) >= sprite->tOriginalX))
        {
            sprite->pos1.x = sprite->tOriginalX;
            sprite->tHide = TRUE;
            sprite->tFrames = 1;
        }
    }
    else // Hide
    {
        if (sprite->tFrames == 0)
        {
            if ((!sprite->tRightToLeft && (sprite->pos1.x += 4) >= sprite->tOriginalX + ABILITY_POP_UP_POS_X_SLIDE)
            ||   (sprite->tRightToLeft && (sprite->pos1.x -= 4) <= sprite->tOriginalX - ABILITY_POP_UP_POS_X_SLIDE))
            {
                gNewBS->activeAbilityPopUps &= ~(gBitTable[sprite->tBattlerId]);
                DestroySprite(sprite);
            }
        }
    }
}

static void Task_FreeAbilityPopUpGfx(u8 taskId)
{
    if (!gSprites[gTasks[taskId].tSpriteId1].inUse
        && !gSprites[gTasks[taskId].tSpriteId2].inUse
        && !gNewBS->activeAbilityPopUps)
    {
		gNewBS->abilityPopUpIds[0] = 0;
		gNewBS->abilityPopUpIds[1] = 0;
        FreeSpriteTilesByTag(ANIM_TAG_ABILITY_POP_UP);
        FreeSpritePaletteByTag(ANIM_TAG_ABILITY_POP_UP);
        DestroyTask(taskId);
    }
}

void AnimTask_DestroyAbilityPopUp(u8 taskId)
{
	gSprites[gNewBS->abilityPopUpIds[0]].tFrames = 0;
	gSprites[gNewBS->abilityPopUpIds[1]].tFrames = 0;
	DestroyAnimVisualTask(taskId);
}

void TransferAbilityPopUpHelper(void)
{
	if (gBattleExecBuffer)
	{
		gBattlescriptCurrInstr -= 5;
		return;
	}
	
	gActiveBattler = gBattleScripting->bank;
	AbilityPopUpHelper = CopyAbility(gActiveBattler);
	
	EmitDataTransfer(0, &AbilityPopUpHelper, 1, &AbilityPopUpHelper);
	MarkBufferBankForExecution(gActiveBattler);
}

void TransferAbilityPopUpHelperAsTrace(void)
{
	if (gBattleExecBuffer)
	{
		gBattlescriptCurrInstr -= 5;
		return;
	}
	
	gActiveBattler = gBattleScripting->bank;
	AbilityPopUpHelper = ABILITY_TRACE;
	
	EmitDataTransfer(0, &AbilityPopUpHelper, 1, &AbilityPopUpHelper);
	MarkBufferBankForExecution(gActiveBattler);
}