#include "defines.h"
#include "defines_battle.h"
#include "../include/battle_anim.h"
#include "../include/battle_string_ids.h"
#include "../include/field_weather.h"
#include "../include/random.h"
#include "../include/string_util.h"
#include "../include/window.h"
#include "../include/constants/items.h"

#include "../include/new/ability_battle_effects.h"
#include "../include/new/ability_battle_scripts.h"
#include "../include/new/ability_tables.h"
#include "../include/new/battle_start_turn_start.h"
#include "../include/new/battle_strings.h"
#include "../include/new/battle_script_util.h"
#include "../include/new/battle_util.h"
#include "../include/new/catching.h"
#include "../include/new/cmd49.h"
#include "../include/new/damage_calc.h"
#include "../include/new/dynamax.h"
#include "../include/new/form_change.h"
#include "../include/new/util.h"
#include "../include/new/move_tables.h"
#include "../include/new/text.h"
/*
ability_battle_effects.c
	-functions that introduce or moodify battle effects via abilities or otherwise.
	-includes terrain effects and ability pop-up

tables to edit:
	gAbilityRatings
	gMoldBreakerIgnoredAbilities
	gWeatherContinuesStringIds
	gFlashFireStringIds
*/

extern u8 gStatusConditionString_MentalState[];
extern u8 gStatusConditionString_TauntProblem[];

const s8 gAbilityRatings[ABILITIES_COUNT] =
{
	[ABILITY_ADAPTABILITY] = 8,
	[ABILITY_AFTERMATH] = 5,
	[ABILITY_AERILATE] = 8,
	[ABILITY_AIRLOCK] = 5,
	[ABILITY_ANALYTIC] = 5,
	[ABILITY_ANGERPOINT] = 4,
	[ABILITY_ANTICIPATION] = 2,
	[ABILITY_ARENATRAP] = 9,
	[ABILITY_AROMAVEIL] = 3,
	[ABILITY_AURABREAK] = 3,
	[ABILITY_BADDREAMS] = 4,
	[ABILITY_BATTERY] = 0,
	[ABILITY_BATTLEARMOR] = 2,
	[ABILITY_BATTLEBOND] = 6,
	[ABILITY_BEASTBOOST] = 7,
	[ABILITY_BERSERK] = 5,
	[ABILITY_BIGPECKS] = 1,
	[ABILITY_BLAZE] = 5,
	[ABILITY_BULLETPROOF] = 7,
	[ABILITY_CHEEKPOUCH] = 4,
	[ABILITY_CHLOROPHYLL] = 6,
	[ABILITY_CLEARBODY] = 4,
	[ABILITY_CLOUDNINE] = 5,
	[ABILITY_COLORCHANGE] = 2,
	[ABILITY_COMATOSE] = 6,
	[ABILITY_COMPETITIVE] = 5,
	[ABILITY_COMPOUNDEYES] = 7,
	[ABILITY_CONTRARY] = 8,
	[ABILITY_CORROSION] = 5,
	[ABILITY_CURSEDBODY] = 4,
	[ABILITY_CUTECHARM] = 2,
	[ABILITY_DAMP] = 2,
	[ABILITY_DANCER] = 5,
	[ABILITY_DARKAURA] = 6,
	[ABILITY_DAZZLING] = 5,
	[ABILITY_DEFEATIST] = -1,
	[ABILITY_DEFIANT] = 5,
	[ABILITY_DELTASTREAM] = 10,
	[ABILITY_DESOLATELAND] = 10,
	[ABILITY_DISGUISE] = 8,
	[ABILITY_DOWNLOAD] = 7,
	[ABILITY_DRIZZLE] = 9,
	[ABILITY_DROUGHT] = 9,
	[ABILITY_DRYSKIN] = 6,
	[ABILITY_EARLYBIRD] = 4,
	[ABILITY_EFFECTSPORE] = 4,
	[ABILITY_ELECTRICSURGE] = 8,
	[ABILITY_EMERGENCYEXIT] = 3,
	[ABILITY_FAIRYAURA] = 6,
	[ABILITY_FILTER] = 6,
	[ABILITY_FLAMEBODY] = 4,
	[ABILITY_FLAREBOOST] = 5,
	[ABILITY_FLASHFIRE] = 6,
	[ABILITY_FLOWERGIFT] = 4,
	[ABILITY_FLOWERVEIL] = 0,
	[ABILITY_FLUFFY] = 5,
	[ABILITY_FORECAST] = 6,
	[ABILITY_FOREWARN] = 2,
	[ABILITY_FRIENDGUARD] = 0,
	[ABILITY_FRISK] = 3,
//	[ABILITY_FULLMETALBODY] = 4,
	[ABILITY_FURCOAT] = 7,
	[ABILITY_GALEWINGS] = 6,
	[ABILITY_GALVANIZE] = 8,
	[ABILITY_GLUTTONY] = 3,
	[ABILITY_GOOEY] = 5,
	[ABILITY_GRASSPELT] = 2,
	[ABILITY_GRASSYSURGE] = 8,
	[ABILITY_GUTS] = 6,
	[ABILITY_HARVEST] = 5,
	[ABILITY_HEALER] = 0,
	[ABILITY_HEATPROOF] = 5,
	[ABILITY_HEAVYMETAL] = -1,
	[ABILITY_HONEYGATHER] = 0,
	[ABILITY_HUGEPOWER] = 10,
	[ABILITY_HUSTLE] = 7,
	[ABILITY_HYDRATION] = 4,
	[ABILITY_HYPERCUTTER] = 3,
	[ABILITY_ICEBODY] = 3,
	[ABILITY_ILLUMINATE] = 0,
	[ABILITY_ILLUSION] = 8,
	[ABILITY_IMMUNITY] = 4,
	[ABILITY_IMPOSTER] = 9,
	[ABILITY_INFILTRATOR] = 6,
	[ABILITY_INNARDSOUT] = 5,
	[ABILITY_INNERFOCUS] = 2,
	[ABILITY_INSOMNIA] = 4,
	[ABILITY_INTIMIDATE] = 7,
	[ABILITY_IRONBARBS] = 6,
	[ABILITY_IRONFIST] = 6,
	[ABILITY_JUSTIFIED] = 4,
	[ABILITY_KEENEYE] = 1,
	[ABILITY_KLUTZ] = -1,
	[ABILITY_LEAFGUARD] = 2,
	[ABILITY_LEVITATE] = 7,
	[ABILITY_LIGHTMETAL] = 2,
	[ABILITY_LIGHTNINGROD] = 7,
	[ABILITY_LIMBER] = 3,
	[ABILITY_LIQUIDOOZE] = 3,
	[ABILITY_LIQUIDVOICE] = 5,
	[ABILITY_LONGREACH] = 3,
	[ABILITY_MAGICBOUNCE] = 9,
	[ABILITY_MAGICGUARD] = 9,
	[ABILITY_MAGICIAN] = 3,
	[ABILITY_MAGMAARMOR] = 1,
	[ABILITY_MAGNETPULL] = 9,
	[ABILITY_MARVELSCALE] = 5,
	[ABILITY_MEGALAUNCHER] = 7,
	[ABILITY_MERCILESS] = 4,
	[ABILITY_MINUS] = 0,
	[ABILITY_MISTYSURGE] = 8,
	[ABILITY_MOLDBREAKER] = 7,
	[ABILITY_MOODY] = 10,
	[ABILITY_MOTORDRIVE] = 6,
	[ABILITY_MOXIE] = 7,
	[ABILITY_MULTISCALE] = 8,
	[ABILITY_MULTITYPE] = 8,
	[ABILITY_MUMMY] = 5,
	[ABILITY_NATURALCURE] = 7,
	[ABILITY_NEUROFORCE] = 6,
	[ABILITY_NOGUARD] = 8,
	[ABILITY_NORMALIZE] = -1,
	[ABILITY_OBLIVIOUS] = 2,
	[ABILITY_OVERCOAT] = 5,
	[ABILITY_OVERGROW] = 5,
	[ABILITY_OWNTEMPO] = 3,
	[ABILITY_PARENTALBOND] = 10,
	[ABILITY_PICKUP] = 1,
	[ABILITY_PICKPOCKET] = 3,
	[ABILITY_PIXILATE] = 8,
	[ABILITY_PLUS] = 0,
	[ABILITY_POISONHEAL] = 8,
	[ABILITY_POISONPOINT] = 4,
	[ABILITY_POISONTOUCH] = 4,
	[ABILITY_PORTALPOWER] = 8,
	[ABILITY_POWERCONSTRUCT] = 10,
	[ABILITY_POWEROFALCHEMY] = 0,
	[ABILITY_PRANKSTER] = 8,
	[ABILITY_PRESSURE] = 5,
	[ABILITY_PRIMORDIALSEA] = 10,
	[ABILITY_PRISMARMOR] = 6,
	[ABILITY_PROTEAN] = 8,
	[ABILITY_PSYCHICSURGE] = 8,
//	[ABILITY_PUREPOWER] = 10,
	[ABILITY_QUEENLYMAJESTY] = 6,
	[ABILITY_QUICKFEET] = 5,
	[ABILITY_RAINDISH] = 3,
	[ABILITY_RATTLED] = 3,
	[ABILITY_RECEIVER] = 0,
	[ABILITY_RECKLESS] = 6,
	[ABILITY_REFRIGERATE] = 8,
	[ABILITY_REGENERATOR] = 8,
	[ABILITY_RIVALRY] = 1,
	[ABILITY_RKS_SYSTEM] = 8,
	[ABILITY_ROCKHEAD] = 5,
	[ABILITY_ROUGHSKIN] = 6,
	[ABILITY_RUNAWAY] = 0,
	[ABILITY_SANDFORCE] = 4,
	[ABILITY_SANDRUSH] = 6,
	[ABILITY_SANDSTREAM] = 9,
	[ABILITY_SANDVEIL] = 3,
	[ABILITY_SAPSIPPER] = 7,
	[ABILITY_SCHOOLING] = 6,
	[ABILITY_SCRAPPY] = 6,
	[ABILITY_SERENEGRACE] = 8,
	[ABILITY_SHADOWSHIELD] = 8,
	[ABILITY_SHADOWTAG] = 10,
	[ABILITY_SHEDSKIN] = 7,
	[ABILITY_SHEERFORCE] = 8,
	[ABILITY_SHELLARMOR] = 2,
	[ABILITY_SHIELDDUST] = 5,
	[ABILITY_SHIELDSDOWN] = 6,
	[ABILITY_SIMPLE] = 8,
	[ABILITY_SKILLLINK] = 7,
	[ABILITY_SLOWSTART] = -2,
	[ABILITY_SLUSHRUSH] = 5,
	[ABILITY_SNIPER] = 3,
	[ABILITY_SNOWCLOAK] = 3,
	[ABILITY_SNOWWARNING] = 8,
	[ABILITY_SOLARPOWER] = 3,
	[ABILITY_SOLIDROCK] = 6,
	[ABILITY_SOULHEART] = 7,
	[ABILITY_SOUNDPROOF] = 4,
	[ABILITY_SPEEDBOOST] = 9,
	[ABILITY_STAKEOUT] = 6,
	[ABILITY_STALL] = -1,
	[ABILITY_STAMINA] = 6,
	[ABILITY_STANCECHANGE] = 10,
	[ABILITY_STATIC] = 4,
	[ABILITY_STEADFAST] = 2,
	[ABILITY_STEELWORKER] = 6,
	[ABILITY_STENCH] = 1,
	[ABILITY_STICKYHOLD] = 3,
	[ABILITY_STORMDRAIN] = 7,
	[ABILITY_STRONGJAW] = 6,
	[ABILITY_STURDY] = 6,
	[ABILITY_SUCTIONCUPS] = 2,
	[ABILITY_SUPERLUCK] = 3,
	[ABILITY_SURGESURFER] = 4,
	[ABILITY_SWARM] = 5,
	[ABILITY_SWEETVEIL] = 4,
	[ABILITY_SWIFTSWIM] = 6,
	[ABILITY_SYMBIOSIS] = 0,
	[ABILITY_SYNCHRONIZE] = 4,
	[ABILITY_TANGLEDFEET] = 2,
	[ABILITY_TANGLINGHAIR] = 5,
	[ABILITY_TECHNICIAN] = 8,
	[ABILITY_TELEPATHY] = 0,
	[ABILITY_TERAVOLT] = 7,
	[ABILITY_THICKFAT] = 7,
	[ABILITY_TINTEDLENS] = 7,
	[ABILITY_TORRENT] = 5,
	[ABILITY_TOXICBOOST] = 6,
	[ABILITY_TOUGHCLAWS] = 7,
	[ABILITY_TRACE] = 6,
	[ABILITY_TRIAGE] = 7,
	[ABILITY_TRUANT] = -2,
	[ABILITY_TURBOBLAZE] = 7,
	[ABILITY_UNAWARE] = 6,
	[ABILITY_UNBURDEN] = 7,
	[ABILITY_UNNERVE] = 3,
	[ABILITY_VICTORYSTAR] = 6,
	[ABILITY_VITALSPIRIT] = 4,
	[ABILITY_VOLTABSORB] = 7,
	[ABILITY_WATERABSORB] = 7,
	[ABILITY_WATERBUBBLE] = 8,
	[ABILITY_WATERCOMPACTION] = 4,
	[ABILITY_WATERVEIL] = 4,
	[ABILITY_WEAKARMOR] = 2,
	[ABILITY_WHITESMOKE] = 4,
//	[ABILITY_WIMPOUT] = 3,
	[ABILITY_WONDERGUARD] = 10,
	[ABILITY_WONDERSKIN] = 4,
	[ABILITY_ZENMODE] = -1,
	[ABILITY_INTREPIDSWORD] = 3,
	[ABILITY_DAUNTLESSSHIELD] = 3,
	[ABILITY_BALLFETCH] = 0,
	[ABILITY_COTTONDOWN] = 3,
	[ABILITY_MIRRORARMOR] = 6,
	[ABILITY_GULPMISSLE] = 3,
	[ABILITY_STALWART] = 2, //Also Propellor Tail
	[ABILITY_STEAMENGINE] = 3,
	[ABILITY_PUNKROCK] = 2,
	[ABILITY_SANDSPIT] = 5,
	[ABILITY_ICESCALES] = 7,
	[ABILITY_RIPEN] = 4,
	[ABILITY_ICEFACE] = 4,
	[ABILITY_POWERSPOT] = 2, //UPDATE
	[ABILITY_MIMICRY] = 2,
	[ABILITY_SCREENCLEANER] = 3,
	[ABILITY_NEUTRALIZINGGAS] = 5,
	[ABILITY_HUNGERSWITCH] = 2,
	[ABILITY_PASTELVEIL] = 4,
	[ABILITY_STEELY_SPIRIT] = 2,
	[ABILITY_PERISH_BODY] = -1,
	[ABILITY_WANDERING_SPIRIT] = 2,
	[ABILITY_GORILLATACTICS] = 4,
};

const bool8 gMoldBreakerIgnoredAbilities[] =
{
	[ABILITY_BATTLEARMOR] =		TRUE,
	[ABILITY_CLEARBODY] =		TRUE,
	[ABILITY_DAMP] =			TRUE,
	[ABILITY_DRYSKIN] =			TRUE,
	[ABILITY_FILTER] =			TRUE,
	[ABILITY_FLASHFIRE] =		TRUE,
	[ABILITY_FLOWERGIFT] =		TRUE,
	[ABILITY_HEATPROOF] =		TRUE,
	[ABILITY_HYPERCUTTER] =		TRUE,
	[ABILITY_IMMUNITY] =		TRUE,
	[ABILITY_INNERFOCUS] =		TRUE,
	[ABILITY_INSOMNIA] =		TRUE,
	[ABILITY_KEENEYE] =			TRUE,
	[ABILITY_LEAFGUARD] =		TRUE,
	[ABILITY_LEVITATE] =		TRUE,
	[ABILITY_LIGHTNINGROD] =	TRUE,
	[ABILITY_LIMBER] =			TRUE,
	[ABILITY_MAGMAARMOR] =		TRUE,
	[ABILITY_MARVELSCALE] =		TRUE,
	[ABILITY_MOTORDRIVE] =		TRUE,
	[ABILITY_OBLIVIOUS] =		TRUE,
	[ABILITY_OWNTEMPO] =		TRUE,
	[ABILITY_SANDVEIL] =		TRUE,
	[ABILITY_SHELLARMOR] =		TRUE,
	[ABILITY_SHIELDDUST] =		TRUE,
	[ABILITY_SIMPLE] =			TRUE,
	[ABILITY_SNOWCLOAK] =		TRUE,
	[ABILITY_SOLIDROCK] =		TRUE,
	[ABILITY_SOUNDPROOF] =		TRUE,
	[ABILITY_STICKYHOLD] =		TRUE,
	[ABILITY_STORMDRAIN] =		TRUE,
	[ABILITY_STURDY] =			TRUE,
	[ABILITY_SUCTIONCUPS] =		TRUE,
	[ABILITY_TANGLEDFEET] =		TRUE,
	[ABILITY_THICKFAT] =		TRUE,
	[ABILITY_UNAWARE] =			TRUE,
	[ABILITY_VITALSPIRIT] =		TRUE,
	[ABILITY_VOLTABSORB] =		TRUE,
	[ABILITY_WATERABSORB] =		TRUE,
	[ABILITY_WATERVEIL] =		TRUE,
	[ABILITY_WHITESMOKE] =		TRUE,
	[ABILITY_WONDERGUARD] =		TRUE,
	[ABILITY_BIGPECKS] =		TRUE,
	[ABILITY_CONTRARY] =		TRUE,
	[ABILITY_FRIENDGUARD] =		TRUE,
	[ABILITY_HEAVYMETAL] =		TRUE,
	[ABILITY_LIGHTMETAL] =		TRUE,
	[ABILITY_MAGICBOUNCE] =		TRUE,
	[ABILITY_MULTISCALE] =		TRUE,
	[ABILITY_SAPSIPPER] =		TRUE,
	[ABILITY_TELEPATHY] =		TRUE,
	[ABILITY_WONDERSKIN] =		TRUE,
	[ABILITY_AROMAVEIL] =		TRUE,
	[ABILITY_BULLETPROOF] =		TRUE,
	[ABILITY_FLOWERVEIL] =		TRUE,
	[ABILITY_FURCOAT] =			TRUE,
	[ABILITY_OVERCOAT] =		TRUE,
	[ABILITY_SWEETVEIL] =		TRUE,
	[ABILITY_DAZZLING] =		TRUE,
	[ABILITY_DISGUISE] =		TRUE,
	[ABILITY_FLUFFY] =			TRUE,
	[ABILITY_QUEENLYMAJESTY] =	TRUE,
	[ABILITY_WATERBUBBLE] =		TRUE,
	[ABILITY_PORTALPOWER] =		TRUE,
	[ABILITY_MIRRORARMOR] =		TRUE,
	[ABILITY_PUNKROCK] =		TRUE,
	[ABILITY_ICESCALES] =		TRUE,
	[ABILITY_ICEFACE] =			TRUE,
	[ABILITY_PASTELVEIL] =		TRUE,
};

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

const u16 gFlashFireStringIds[] =
{
	STRINGID_PKMNRAISEDFIREPOWERWITH, STRINGID_ITDOESNTAFFECT
};

extern u8 CastformDataTypeChange(u8 bank);
extern void TransformPokemon(u8 bankAtk, u8 bankDef);

//This file's functions:
static u8 CalcMovePowerForForewarn(u16 move);
static u8 ActivateWeatherAbility(u16 flags, u16 item, u8 bank, u8 animArg, u8 stringIndex, bool8 moveTurn);
static u8 TryActivateTerrainAbility(u8 terrain, u8 anim, u8 bank);
static bool8 ImmunityAbilityCheck(u8 bank, u32 status, u8* string);
static bool8 AllMainStatsButOneAreMinned(u8 bank);

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

	if (gBattleTypeFlags & (BATTLE_TYPE_SAFARI | BATTLE_TYPE_OLD_MAN))
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
			if (SIDE(bank) == B_SIDE_OPPONENT)
				return FALSE; //Ghost's abilities don't activate

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

	if (gNewBS->skipCertainSwitchInAbilities)
	{
		gNewBS->skipCertainSwitchInAbilities = FALSE;

		switch (gLastUsedAbility) {
			case ABILITY_INTIMIDATE:
			case ABILITY_DOWNLOAD:
			case ABILITY_FOREWARN:
			case ABILITY_IMPOSTER:
			case ABILITY_ANTICIPATION:
			case ABILITY_FRISK:
				gStatuses3[bank] |= STATUS3_SWITCH_IN_ABILITY_DONE;
				break;
			case ABILITY_TRACE: //Trace is the only ability that activates after a U-Turn + faint switch-in
				return FALSE;
		}
	}

	switch (caseID)
	{
	case ABILITYEFFECT_ON_SWITCHIN: // 0;
		gBattleScripting.bank = bank;

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
						gBattleScripting.animArg1 = B_ANIM_RAIN_CONTINUES;
						effect++;
					}
					break;
				case WEATHER_RAIN_HEAVY:
					if (!(gBattleWeather & WEATHER_RAIN_ANY))
					{
						gBattleWeather = (WEATHER_RAIN_DOWNPOUR | WEATHER_RAIN_PERMANENT);
						gBattleScripting.animArg1 = B_ANIM_RAIN_CONTINUES;
						effect++;
					}
					break;
				case WEATHER_SANDSTORM:
					if (!(gBattleWeather & WEATHER_SANDSTORM_ANY))
					{
						gBattleWeather = (WEATHER_SANDSTORM_PERMANENT | WEATHER_SANDSTORM_TEMPORARY);
						gBattleScripting.animArg1 = B_ANIM_SANDSTORM_CONTINUES;
						effect++;
					}
					break;
				case WEATHER_DROUGHT:
					if (!(gBattleWeather & WEATHER_SUN_ANY))
					{
						gBattleWeather = (WEATHER_SUN_PERMANENT | WEATHER_SUN_TEMPORARY);
						gBattleScripting.animArg1 = B_ANIM_SUN_CONTINUES;
						effect++;
					}
					break;

			#ifdef HAIL_IN_BATTLE
				case WEATHER_STEADY_SNOW:
					if (!(gBattleWeather & WEATHER_HAIL_ANY))
					{
						gBattleWeather = (WEATHER_HAIL_PERMANENT | WEATHER_HAIL_TEMPORARY);
						gBattleScripting.animArg1 = B_ANIM_HAIL_CONTINUES;
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
					if (!(gBattleWeather & WEATHER_FOG_ANY) && !gNewBS->fogBlownAwayByDefog)
					{
						gBattleStringLoader = gText_FogIsDeep;
						gBattleWeather = (WEATHER_FOG_TEMPORARY | WEATHER_FOG_PERMANENT);
						gBattleScripting.animArg1 = B_ANIM_FOG_CONTINUES;
						effect++;
					}
				#endif
			#endif
			}

			if (effect)
			{
				if (gBattleTypeFlags & BATTLE_TYPE_BATTLE_CIRCUS && gBattleCircusFlags & BATTLE_CIRCUS_WEATHER)
					gBattleWeather |= WEATHER_CIRCUS; //Can't be removed

				gBattleCommunication[MULTISTRING_CHOOSER] = GetCurrentWeather();
				BattleScriptPushCursorAndCallback(BattleScript_OverworldWeatherStarts);
			}
			break;

		case ABILITY_DRIZZLE:
			if (!(gBattleWeather & (WEATHER_RAIN_ANY | WEATHER_PRIMAL_ANY | WEATHER_CIRCUS)))
			{
				effect = ActivateWeatherAbility(WEATHER_RAIN_PERMANENT | WEATHER_RAIN_TEMPORARY,
												ITEM_EFFECT_DAMP_ROCK, bank, B_ANIM_RAIN_CONTINUES, 0, FALSE);
			}

			break;

		case ABILITY_SANDSTREAM:
			if (!(gBattleWeather & (WEATHER_SANDSTORM_ANY | WEATHER_PRIMAL_ANY | WEATHER_CIRCUS)))
			{
				effect = ActivateWeatherAbility(WEATHER_SANDSTORM_PERMANENT | WEATHER_SANDSTORM_TEMPORARY,
												ITEM_EFFECT_SMOOTH_ROCK, bank, B_ANIM_SANDSTORM_CONTINUES, 1, FALSE);
			}

			break;

		case ABILITY_DROUGHT:
			if (!(gBattleWeather & (WEATHER_SUN_ANY | WEATHER_PRIMAL_ANY | WEATHER_CIRCUS)))
			{
				effect = ActivateWeatherAbility(WEATHER_SUN_PERMANENT | WEATHER_SUN_TEMPORARY,
												ITEM_EFFECT_HEAT_ROCK, bank, B_ANIM_SUN_CONTINUES, 2, FALSE);
			}

			break;

		case ABILITY_SNOWWARNING:
			if (!(gBattleWeather & (WEATHER_HAIL_ANY | WEATHER_PRIMAL_ANY | WEATHER_CIRCUS)))
			{
				gBattleScripting.animArg1 = B_ANIM_HAIL_CONTINUES;
				effect = ActivateWeatherAbility(WEATHER_HAIL_PERMANENT | WEATHER_HAIL_TEMPORARY,
												ITEM_EFFECT_ICY_ROCK, bank, B_ANIM_HAIL_CONTINUES, 3, FALSE);
			}

			break;

		case ABILITY_PRIMORDIALSEA:
			if (!(gBattleWeather & (WEATHER_RAIN_PRIMAL | WEATHER_CIRCUS)))
			{
				gBattleWeather = (WEATHER_RAIN_PERMANENT | WEATHER_RAIN_TEMPORARY |  WEATHER_RAIN_PRIMAL);
				gWishFutureKnock.weatherDuration = 0;
				gBattleScripting.animArg1 = B_ANIM_RAIN_CONTINUES;
				gBattleStringLoader = gText_PrimordialSeaActivate;
				gBattleCommunication[MULTISTRING_CHOOSER] = 4;
				BattleScriptPushCursorAndCallback(BattleScript_NewWeatherAbilityActivates);
				effect++;
			}

			break;

		case ABILITY_DESOLATELAND:
			if (!(gBattleWeather & (WEATHER_SUN_PRIMAL | WEATHER_CIRCUS)))
			{
				gBattleWeather = (WEATHER_SUN_PERMANENT | WEATHER_SUN_TEMPORARY |  WEATHER_SUN_PRIMAL);
				gWishFutureKnock.weatherDuration = 0;
				gBattleScripting.animArg1 = B_ANIM_SUN_CONTINUES;
				gBattleStringLoader = gText_DesolateLandActivate;
				gBattleCommunication[MULTISTRING_CHOOSER] = 4;
				BattleScriptPushCursorAndCallback(BattleScript_NewWeatherAbilityActivates);
				effect++;
			}

			break;

		case ABILITY_DELTASTREAM:
			if (!(gBattleWeather & (WEATHER_AIR_CURRENT_PRIMAL | WEATHER_CIRCUS)))
			{
				gBattleWeather = (WEATHER_AIR_CURRENT_PRIMAL);
				gWishFutureKnock.weatherDuration = 0;
				gBattleScripting.animArg1 = B_ANIM_STRONG_WINDS_CONTINUE;
				gBattleStringLoader = gText_DeltaStream;
				gBattleCommunication[MULTISTRING_CHOOSER] = 4;
				BattleScriptPushCursorAndCallback(BattleScript_NewWeatherAbilityActivates);
				effect++;
			}

			break;

		case ABILITY_INTIMIDATE:
			if (!(gBattleMons[FOE(bank)].status2 & STATUS2_SUBSTITUTE) || !(gBattleMons[PARTNER(FOE(bank))].status2 & STATUS2_SUBSTITUTE))
			{
				BattleScriptPushCursorAndCallback(BattleScript_IntimidateActivatesEnd3);
				gBattleStruct->intimidateBank = bank;
				gNewBS->intimidateActive = bank + 1;
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
			if (IS_DOUBLE_BATTLE)
			{
				if (*GetAbilityLocation(target1) != ABILITY_NONE && gBattleMons[target1].hp != 0
				&& *GetAbilityLocation(target2) != ABILITY_NONE && gBattleMons[target2].hp != 0)
				{
					gActiveBattler = GetBattlerAtPosition(((Random() & 1) * 2) | side);
					effect++;
				}
				else if (*GetAbilityLocation(target1) != ABILITY_NONE && gBattleMons[target1].hp != 0)
				{
					gActiveBattler = target1;

					effect++;
				}
				else if (*GetAbilityLocation(target2) != ABILITY_NONE && gBattleMons[target2].hp != 0)
				{
					gActiveBattler = target2;
					effect++;
				}
			}
			else //Single Battle
			{
				if (*GetAbilityLocation(target1) && gBattleMons[target1].hp)
				{
					gActiveBattler = target1;
					effect++;
				}
			}

			if (effect)
			{
				if (!CheckTableForAbility(*GetAbilityLocation(gActiveBattler), gTraceBannedAbilities))
				{
					gBankAttacker = bank;
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
			gBattleStringLoader = gText_AirLockActivate;
			BattleScriptPushCursorAndCallback(BattleScript_AirLock);
			effect++;
			break;

		case ABILITY_PRESSURE:
			gBattleStringLoader = gText_PressureActivate;
			BattleScriptPushCursorAndCallback(BattleScript_SwitchInAbilityMsg);
			effect++;
			break;

		case ABILITY_MOLDBREAKER:
			gBattleStringLoader = gText_MoldBreakerActivate;
			BattleScriptPushCursorAndCallback(BattleScript_SwitchInAbilityMsg);
			effect++;
			break;

		case ABILITY_TERAVOLT:
			gBattleStringLoader = gText_TeravoltActivate;
			BattleScriptPushCursorAndCallback(BattleScript_SwitchInAbilityMsg);
			effect++;
			break;

		case ABILITY_TURBOBLAZE:
			gBattleStringLoader = gText_TurboblazeActivate;
			BattleScriptPushCursorAndCallback(BattleScript_SwitchInAbilityMsg);
			effect++;
			break;

		case ABILITY_SLOWSTART:
			gNewBS->SlowStartTimers[bank] = 5;
			gBattleStringLoader = gText_SlowStartActivate;
			BattleScriptPushCursorAndCallback(BattleScript_SwitchInAbilityMsg);
			effect++;
			break;

		case ABILITY_UNNERVE:
			gBankAttacker = bank;
			gBattleStringLoader = gText_UnnerveActivate;
			BattleScriptPushCursorAndCallback(BattleScript_SwitchInAbilityMsg);
			effect++;
			break;

		case ABILITY_DARKAURA:
			gBattleStringLoader = gText_DarkAuraActivate;
			BattleScriptPushCursorAndCallback(BattleScript_SwitchInAbilityMsg);
			effect++;
			break;

		case ABILITY_FAIRYAURA:
			gBattleStringLoader = gText_FairyAuraActivate;
			BattleScriptPushCursorAndCallback(BattleScript_SwitchInAbilityMsg);
			effect++;
			break;

		case ABILITY_AURABREAK:
			gBattleStringLoader = gText_AuraBreakActivate;
			BattleScriptPushCursorAndCallback(BattleScript_SwitchInAbilityMsg);
			effect++;
			break;

		case ABILITY_COMATOSE:
			gBattleStringLoader = gText_ComatoseActivate;
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

				if (IS_DOUBLE_BATTLE
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
				gBattleStringLoader = gText_AnticipationActivate;
				BattleScriptPushCursorAndCallback(BattleScript_SwitchInAbilityMsg);
			}

			break;

		case ABILITY_FOREWARN: ;
			u16 strongestMove = MOVE_NONE;
			u8 maxPower = 0;
			u8 strongestTarget = FOE(bank);
			u16 power;

			for (i = 0; i < MAX_MON_MOVES; ++i)
			{
				if (gBattleMons[FOE(bank)].hp != 0)
				{
					move = gBattleMons[FOE(bank)].moves[i];
					if (move != MOVE_NONE)
					{
						power = CalcMovePowerForForewarn(move);

						if (strongestMove == MOVE_NONE)
						{
							strongestMove = move;
							maxPower = power;
							strongestTarget = FOE(bank);
						}
						else if (power > maxPower
						|| (power == maxPower && Random() & 1))
						{
							maxPower = power;
							strongestMove = move;
							strongestTarget = FOE(bank);
						}
					}
				}

				if (IS_DOUBLE_BATTLE
				&&  gBattleMons[PARTNER(FOE(bank))].hp)
				{
					move = gBattleMons[PARTNER(FOE(bank))].moves[i];
					if (move != MOVE_NONE)
					{
						power = CalcMovePowerForForewarn(move);

						if (strongestMove == MOVE_NONE)
						{
							strongestMove = move;
							maxPower = power;
							strongestTarget = PARTNER(FOE(bank));
						}
						else if (power > maxPower
						|| (power == maxPower && Random() & 1))
						{
							maxPower = power;
							strongestMove = move;
							strongestTarget = PARTNER(FOE(bank));
						}
					}
				}
			}

			gBankAttacker = bank;
			gEffectBank = strongestTarget;
			PREPARE_MOVE_BUFFER(gBattleTextBuff1, strongestMove);
			gBattleStringLoader = gText_ForewarnActivate;
			BattleScriptPushCursorAndCallback(BattleScript_SwitchInAbilityMsg);
			effect++;
			break;

		case ABILITY_FRISK: ;
			u8 foe = SIDE(bank) ^ BIT_SIDE;
			u8 partner = PARTNER(foe);
			u8 skipFoe = 0xFF;

			//Deal with a Red Card switch-in.
			if (gForceSwitchHelper == Force_Switch_Red_Card)
			{
				skipFoe = gNewBS->originalTargetBackup;

				if (IS_DOUBLE_BATTLE && partner == skipFoe) //The second Frisk target should be skipped
				{
					//Make the skipped target the first one
					u8 temp = partner;
					partner = foe;
					foe = temp;
				}
			}

			if (foe != skipFoe && BATTLER_ALIVE(foe) && ITEM(foe))
			{
				gLastUsedItem = ITEM(foe);
				gBankTarget = foe;
				gBattleScripting.bank = bank;
				gBattleStringLoader = gText_FriskActivate;
				RecordItemEffectBattle(foe, ITEM_EFFECT(foe));
				BattleScriptPushCursorAndCallback(BattleScript_Frisk);
				effect++;
			}
			else if (IS_DOUBLE_BATTLE && BATTLER_ALIVE(partner) && ITEM(partner) != ITEM_NONE)
			{
				gLastUsedItem = ITEM(partner);
				gBankTarget = partner;
				gBattleScripting.bank = bank;
				gBattleStringLoader = gText_FriskActivate;
				RecordItemEffectBattle(partner, ITEM_EFFECT(partner));
				BattleScriptPushCursorAndCallback(BattleScript_SwitchInAbilityMsg);
				effect++;
			}
			break;

		case ABILITY_IMMUNITY:
			effect = ImmunityAbilityCheck(bank, STATUS1_PSN_ANY, gStatusConditionString_Poison);
			break;

		//case ABILITY_PASTELVEIL:
		//	effect = ImmunityAbilityCheck(bank, STATUS1_PSN_ANY, gStatusConditionString_Poison);
		//	break;

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

			if (IS_DOUBLE_BATTLE)
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

			if (STAT_STAGE(bank, statId) < STAT_STAGE_MAX)
			{
				gBankAttacker = bank;
				STAT_STAGE(bank, statId)++;
				gBattleScripting.statChanger = statId | INCREASE_1;
				PREPARE_STAT_BUFFER(gBattleTextBuff1, statId);
				PREPARE_STAT_ROSE(gBattleTextBuff2);
				BattleScriptPushCursorAndCallback(BattleScript_AttackerAbilityStatRaiseEnd3);
				effect++;
			}
			break;

		case ABILITY_IMPOSTER: ;
			u8 transformBank = BATTLE_OPPOSITE(bank);

			if (IsRaidBattle() && SIDE(bank) == B_SIDE_PLAYER)
				transformBank = BANK_RAID_BOSS;

			if (BATTLER_ALIVE(transformBank)
			&& !(gBattleMons[transformBank].status2 & (STATUS2_TRANSFORMED | STATUS2_SUBSTITUTE))
			&& !(gStatuses3[transformBank] & (STATUS3_SEMI_INVULNERABLE | STATUS3_ILLUSION))
			&& !IS_TRANSFORMED(bank)
			&& !(IsRaidBattle() && transformBank == BANK_RAID_BOSS && gNewBS->dynamaxData.raidShieldsUp))
			{
				gBankAttacker = bank;
				gBankTarget = transformBank;
				TransformPokemon(bank, gBankTarget);
				BattleScriptPushCursorAndCallback(BattleScript_ImposterActivates);
				effect++;
			}
			break;

		case ABILITY_SCHOOLING:
			if (!(gBattleMons[bank].status2 & STATUS2_TRANSFORMED))
			{
				if (SPECIES(bank) == SPECIES_WISHIWASHI && gBattleMons[bank].level >= 20
				&&  gBattleMons[bank].hp > (gBattleMons[bank].maxHP / 4))
				{
					DoFormChange(bank, SPECIES_WISHIWASHI_S, FALSE, TRUE, FALSE);
					BattleScriptPushCursorAndCallback(BattleScript_StartedSchoolingEnd3);
					++effect;
				}
				else if (SPECIES(bank) == SPECIES_WISHIWASHI_S
				&& (gBattleMons[bank].level < 20 ||
					gBattleMons[bank].hp <= (gBattleMons[bank].maxHP / 4)))
				{
					DoFormChange(bank, SPECIES_WISHIWASHI, FALSE, TRUE, FALSE);
					BattleScriptPushCursorAndCallback(BattleScript_StoppedSchoolingEnd3);
					++effect;
				}
			}
			break;

		case ABILITY_SHIELDSDOWN:
			if (!(gBattleMons[bank].status2 & STATUS2_TRANSFORMED))
			{
				if (SPECIES(bank) == SPECIES_MINIOR_SHIELD
				&& gBattleMons[bank].hp <= (gBattleMons[bank].maxHP / 2))
				{
					DoFormChange(bank, umodsi(GetBankPartyData(bank)->personality, 7), FALSE, TRUE, FALSE); //Get Minior Colour
					BattleScriptPushCursorAndCallback(BattleScript_ShieldsDownToCoreEnd3);
					++effect;
				}
				else if ((SPECIES(bank) == SPECIES_MINIOR_RED ||
						  SPECIES(bank) == SPECIES_MINIOR_BLUE ||
						  SPECIES(bank) == SPECIES_MINIOR_ORANGE ||
						  SPECIES(bank) == SPECIES_MINIOR_YELLOW ||
						  SPECIES(bank) == SPECIES_MINIOR_INDIGO ||
						  SPECIES(bank) == SPECIES_MINIOR_GREEN ||
						  SPECIES(bank) == SPECIES_MINIOR_VIOLET)
				&& gBattleMons[bank].hp > (gBattleMons[bank].maxHP / 2))
				{
					DoFormChange(bank, SPECIES_MINIOR_SHIELD, FALSE, TRUE, FALSE);
					BattleScriptPushCursorAndCallback(BattleScript_ShieldsDownToMeteorEnd3);
					++effect;
				}
			}
			break;

		case ABILITY_FLOWERGIFT:
			#if (defined SPECIES_CHERRIM && defined SPECIES_CHERRIM_SUN)
			if (!(gBattleMons[bank].status2 & STATUS2_TRANSFORMED))
			{
				switch(SPECIES(bank)) {
					case SPECIES_CHERRIM:
						if (WEATHER_HAS_EFFECT && (gBattleWeather & WEATHER_SUN_ANY)
						&& ITEM_EFFECT(bank) != ITEM_EFFECT_UTILITY_UMBRELLA)
						{
							DoFormChange(bank, SPECIES_CHERRIM_SUN, FALSE, FALSE, FALSE);
							BattleScriptPushCursorAndCallback(BattleScript_TransformedEnd3);
							++effect;
						}
						break;

					case SPECIES_CHERRIM_SUN:
						if (!WEATHER_HAS_EFFECT || !(gBattleWeather & WEATHER_SUN_ANY)
						|| ITEM_EFFECT(bank) == ITEM_EFFECT_UTILITY_UMBRELLA)
						{
							DoFormChange(bank, SPECIES_CHERRIM, FALSE, FALSE, FALSE);
							BattleScriptPushCursorAndCallback(BattleScript_TransformedEnd3);
							++effect;
						}
				}
			}
			#endif
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

		case ABILITY_INTREPIDSWORD:
			if (STAT_STAGE(bank, STAT_STAGE_ATK) < STAT_STAGE_MAX)
			{
				gBankAttacker = bank;
				STAT_STAGE(bank, STAT_STAGE_ATK)++;
				gBattleScripting.statChanger = STAT_STAGE_ATK | INCREASE_1;
				PREPARE_STAT_BUFFER(gBattleTextBuff1, STAT_STAGE_ATK);
				PREPARE_STAT_ROSE(gBattleTextBuff2);
				BattleScriptPushCursorAndCallback(BattleScript_AttackerAbilityStatRaiseEnd3);
				effect++;
			}
			break;

		case ABILITY_DAUNTLESSSHIELD:
			if (STAT_STAGE(bank, STAT_STAGE_DEF) < STAT_STAGE_MAX)
			{
				gBankAttacker = bank;
				STAT_STAGE(bank, STAT_STAGE_DEF)++;
				gBattleScripting.statChanger = STAT_STAGE_DEF | INCREASE_1;
				PREPARE_STAT_BUFFER(gBattleTextBuff1, STAT_STAGE_DEF);
				PREPARE_STAT_ROSE(gBattleTextBuff2);
				BattleScriptPushCursorAndCallback(BattleScript_AttackerAbilityStatRaiseEnd3);
				effect++;
			}
			break;

		case ABILITY_SCREENCLEANER:
			RemoveScreensFromSide(B_SIDE_PLAYER);
			RemoveScreensFromSide(B_SIDE_OPPONENT);
			gBattleStringLoader = gText_ScreenCleanerActivate;
			BattleScriptPushCursorAndCallback(BattleScript_SwitchInAbilityMsg);
			effect++;
			break;

		case ABILITY_MIMICRY: ;
			const u8* script = TryActivateMimicryForBank(bank);
			if (script != NULL)
			{
				BattleScriptPushCursorAndCallback(BattleScript_End3); //Pop back down to end
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = script;
				effect++;
			}
			break;

		case ABILITY_NEUTRALIZINGGAS:
			for (i = 0; i < gBattlersCount; ++i)
			{
				if (!IsAbilitySuppressed(i) //Gastro Acid has higher priority
				&& ABILITY(i) != ABILITY_NONE
				&& !CheckTableForAbility(ABILITY(i), gNeutralizingGasBannedAbilities))
				{
					u8* abilityLoc = GetAbilityLocation(i);
					gNewBS->neutralizingGasBlockedAbilities[i] = *abilityLoc;
					*abilityLoc = 0;
					gNewBS->SlowStartTimers[i] = 0;
				}
			}

			gBattleStringLoader = gText_NeutralizingGasActivate;
			BattleScriptPushCursorAndCallback(BattleScript_NeutralizingGas);
			effect++;
		}

		switch (gLastUsedAbility) { //These abilities should always activate if they can
			case ABILITY_NONE: //So Unnerve activates the first time when Neutralizing Gas leaves the field
			case ABILITY_FORECAST:
			case ABILITY_FLOWERGIFT:
			case ABILITY_TRACE:
			case ABILITYEFFECT_SWITCH_IN_WEATHER:
				break;
			default:
				gStatuses3[bank] |= STATUS3_SWITCH_IN_ABILITY_DONE;
		}

		break;

	case ABILITYEFFECT_ENDTURN: // 1
			if (BATTLER_ALIVE(bank))
			{
				gBankAttacker = gActiveBattler = gBattleScripting.bank = bank;
				switch (gLastUsedAbility)
				{
				case ABILITY_RAINDISH:
					if (WEATHER_HAS_EFFECT && (gBattleWeather & WEATHER_RAIN_ANY)
					&& ITEM_EFFECT(bank) != ITEM_EFFECT_UTILITY_UMBRELLA
					&& !BATTLER_MAX_HP(bank))
					{
						BattleScriptPushCursorAndCallback(BattleScript_RainDishActivates);
						gBattleMoveDamage = MathMax(1, GetBaseMaxHP(bank) / 16);
						gBattleMoveDamage *= -1;
						effect++;
					}
					break;

				case ABILITY_DRYSKIN:
					if (WEATHER_HAS_EFFECT && ITEM_EFFECT(bank) != ITEM_EFFECT_UTILITY_UMBRELLA)
					{
						if (gBattleWeather & WEATHER_RAIN_ANY && !BATTLER_MAX_HP(bank))
						{
							gBattleMoveDamage = MathMax(1, GetBaseMaxHP(bank) / 8);
							gBattleMoveDamage *= -1;
							BattleScriptPushCursorAndCallback(BattleScript_RainDishActivates);
							effect++;
						}
						else if (gBattleWeather & WEATHER_SUN_ANY)
						{
							gBattleMoveDamage = MathMax(1, GetBaseMaxHP(bank) / 8);
							BattleScriptPushCursorAndCallback(BattleScript_DrySkinDamage);
							effect++;
						}
					}
					break;

				case ABILITY_ICEBODY:
					if (WEATHER_HAS_EFFECT && (gBattleWeather & WEATHER_HAIL_ANY) && !BATTLER_MAX_HP(bank))
					{
						gBattleMoveDamage = MathMax(1, GetBaseMaxHP(bank) / 16);
						gBattleMoveDamage *= -1;
						BattleScriptExecute(BattleScript_RainDishActivates);
						effect++;
					}
					break;

				case ABILITY_SOLARPOWER:
					if (WEATHER_HAS_EFFECT && (gBattleWeather & WEATHER_SUN_ANY)
					&& ITEM_EFFECT(bank) != ITEM_EFFECT_UTILITY_UMBRELLA)
					{
						gBattleMoveDamage = MathMax(1, GetBaseMaxHP(bank) / 8);
						BattleScriptExecute(BattleScript_SolarPowerDamage);
						effect++;
					}
					break;

				case ABILITY_SHEDSKIN:
					if ((gBattleMons[bank].status1 & STATUS1_ANY) && Random() % 3 == 0)
					{
						ClearBankStatus(bank);
						BattleScriptPushCursorAndCallback(BattleScript_AbilityCuredStatusEnd3);
						effect++;
					}
					break;

				case ABILITY_HYDRATION:
					if (WEATHER_HAS_EFFECT && (gBattleWeather & WEATHER_RAIN_ANY)
					&& ITEM_EFFECT(bank) != ITEM_EFFECT_UTILITY_UMBRELLA
					&& gBattleMons[bank].status1 & STATUS_ANY)
					{
						ClearBankStatus(bank);
						BattleScriptPushCursorAndCallback(BattleScript_AbilityCuredStatusEnd3);
						effect++;
					}
					break;

				case ABILITY_HEALER:
					if (IS_DOUBLE_BATTLE
					&& BATTLER_ALIVE(PARTNER(bank))
					&& gBattleMons[PARTNER(bank)].status1
					&& Random() % 100 < 30)
					{
						gEffectBank = PARTNER(bank);
						ClearBankStatus(gEffectBank);
						BattleScriptPushCursorAndCallback(BattleScript_Healer);
						effect++;
					}
					break;

				case ABILITY_SPEEDBOOST:
					if (STAT_STAGE(bank, STAT_SPEED) < STAT_STAGE_MAX && gDisableStructs[bank].isFirstTurn != 2)
					{
						gBattleMons[bank].statStages[STAT_SPEED - 1]++;
						gBattleScripting.statChanger = STAT_SPEED | INCREASE_1;
						gBattleScripting.animArg1 = 0x11;
						gBattleScripting.animArg2 = 0;
						PREPARE_STAT_BUFFER(gBattleTextBuff1, STAT_SPEED);
						PREPARE_STAT_ROSE(gBattleTextBuff2);
						BattleScriptPushCursorAndCallback(BattleScript_AttackerAbilityStatRaiseEnd3);
						effect++;
					}
					break;

				case ABILITY_MOODY: ;
					u8 statToRaise = 0;
					u8 statToLower = 0;
					const u8* scriptPtr;

					if (MainStatsMinned(bank))
					{	//Raise One Stat
						statToRaise = RandRange(STAT_ATK, STAT_SPDEF + 1); //Attack, Defense, Sp. Atk, Sp.Def, Speed
						gBattleScripting.statChanger = statToRaise | INCREASE_2;
						scriptPtr = BattleScript_MoodySingleStat;
					}
					else if (MainStatsMaxed(bank))
					{	//Lower One Stat
						statToLower = RandRange(STAT_ATK, STAT_SPDEF + 1); //Attack, Defense, Sp. Atk, Sp.Def, Speed
						gBattleScripting.statChanger = statToLower | DECREASE_1;
						scriptPtr = BattleScript_MoodySingleStat;
					}
					else
					{	//Raise One Stat and Lower Another
						if (!(AllMainStatsButOneAreMinned(bank)))
						{	//At least two non min stats
							do
							{
								statToRaise = RandRange(STAT_ATK, STAT_SPDEF + 1); //Attack, Defense, Sp. Atk, Sp.Def, Speed
							} while (STAT_STAGE(bank, statToRaise) == STAT_STAGE_MAX);
						}
						else
						{	//If all stats but one are at min, then raise one of the min ones so the
							//non min one canned be lowered.
							do
							{
								statToRaise = RandRange(STAT_ATK, STAT_SPDEF + 1); //Attack, Defense, Sp. Atk, Sp.Def, Speed
							} while (STAT_STAGE(bank, statToRaise) != 0);
						}

						do
						{
							statToLower = RandRange(STAT_ATK, STAT_SPDEF + 1); //Attack, Defense, Sp. Atk, Sp.Def, Speed
						} while (statToLower == statToRaise || STAT_STAGE(bank, statToLower) == 0);

						gBattleScripting.statChanger = statToRaise | INCREASE_2;
						gBattleCommunication[MOVE_EFFECT_BYTE] = statToLower; //Save stat to lower in move effect byte
						scriptPtr = BattleScript_MoodyRegular;
					}

					BattleScriptPushCursorAndCallback(scriptPtr);
					effect++;
					break;

				case ABILITY_BADDREAMS:
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
					if (!(gBattleMons[bank].status1 & STATUS1_SLEEP))
						gDisableStructs[bank].truantCounter ^= 1;
					else
						gDisableStructs[bank].truantCounter = 0; //Being asleep resets the Truant counter
					break;

				case ABILITY_HARVEST:
					if (gItems[(SAVED_CONSUMED_ITEMS(bank))].pocket == POCKET_BERRY_POUCH)
					{
						if (WEATHER_HAS_EFFECT && gBattleWeather & WEATHER_SUN_ANY && ITEM_EFFECT(bank) != ITEM_EFFECT_UTILITY_UMBRELLA) //Yeah...that'll never happen
						{
							//100% chance
						}
						else //No Sun
						{
							if (Random() % 100 < 50)
								break;
						}

						BattleScriptPushCursorAndCallback(BattleScript_Harvest);
						++effect;
					}
					break;

				case ABILITY_PICKUP: ;
					u8 itemBank = GetTopOfPickupStackNotIncludingBank(bank);

					if (itemBank != 0xFF)
					{
						RemoveBankFromPickupStack(itemBank);
						if (CONSUMED_ITEMS(itemBank)) //This shouldn't be empty but just in case
						{
							gBattleMons[bank].item = CONSUMED_ITEMS(itemBank);
							EmitSetMonData(0, REQUEST_HELDITEM_BATTLE, 0, 2, &gBattleMons[bank].item);
							MarkBufferBankForExecution(bank);
							gLastUsedItem = CONSUMED_ITEMS(itemBank);
							CONSUMED_ITEMS(itemBank) = 0;
							RecordItemEffectBattle(bank, ITEM_EFFECT(bank));
							BattleScriptPushCursorAndCallback(BattleScript_Pickup);
							++effect;
							break;
						}
					}
					break;

				case ABILITY_SLOWSTART:
					if (gNewBS->SlowStartTimers[bank] > 0 && --gNewBS->SlowStartTimers[bank] == 0)
					{
						gBattleStringLoader = gText_SlowStartEnd;
						BattleScriptPushCursorAndCallback(BattleScript_SwitchInAbilityMsg);
						++effect;
					}
					break;

				case ABILITY_BALLFETCH:
					if (gNewBS->failedThrownPokeBall != ITEM_NONE
					&& SIDE(bank) == B_SIDE_PLAYER
					&& ITEM(bank) == ITEM_NONE)
					{
						gLastUsedItem = ITEM(bank) = gNewBS->failedThrownPokeBall;
						gNewBS->failedThrownPokeBall = ITEM_NONE;

						gActiveBattler = bank;
						EmitSetMonData(0, REQUEST_HELDITEM_BATTLE, 0, 2, &gBattleMons[gActiveBattler].item);
						MarkBufferBankForExecution(gActiveBattler);

						BattleScriptPushCursorAndCallback(BattleScript_BallFetch);
						++effect;
					}
					break;

				case ABILITY_FORECAST:
					effect = CastformDataTypeChange(bank);
					if (effect)
					{
						BattleScriptPushCursorAndCallback(BattleScript_CastformChange);
						gBattleStruct->castformToChangeInto = effect - 1;
					}
					break;
			
				//case ABILITY_WIMPOUT:
				case ABILITY_EMERGENCYEXIT:
					if (!(gStatuses3[bank] & (STATUS3_SKY_DROP_ANY))
					&& gNewBS->turnDamageTaken[bank] > 0
					&& gBattleMons[bank].hp <= gBattleMons[bank].maxHP / 2
					&& gBattleMons[bank].hp + gNewBS->turnDamageTaken[bank] > gBattleMons[bank].maxHP / 2) //Fell from end turn damage
					{
						gBattleScripting.bank = gBankSwitching = bank;
						BattleScriptPushCursorAndCallback(BattleScript_EmergencyExitEnd3);
						effect = 1;
					}

					gNewBS->turnDamageTaken[bank] = 0; //Reset to prevent accidental triggering
				}
			}
			break;

		case ABILITYEFFECT_MOVES_BLOCK: // 2
			if (gBankAttacker != bank) //Can't block against self
			{
				switch (gLastUsedAbility) {
					case ABILITY_SOUNDPROOF:
						if (CheckSoundMove(move))
							effect = 1;
						break;

					case ABILITY_BULLETPROOF:
						if (CheckTableForMove(move, gBallBombMoves))
							effect = 1;
						break;

					case ABILITY_OVERCOAT:
						if (CheckTableForMove(move, gPowderMoves))
							effect = 1;
						break;

					case ABILITY_DAZZLING: //Cannot use
					case ABILITY_QUEENLYMAJESTY: //Cannot use
						if (PriorityCalc(gBankAttacker, ACTION_USE_MOVE, move) > 0)
							effect = 2;
						break;

					case ABILITY_TELEPATHY:
						if (IS_DOUBLE_BATTLE
						&& gBankAttacker == PARTNER(bank))
							effect = 1;
						break;
				}

				if (effect)
				{
					if (gBattleMons[gBankAttacker].status2 & STATUS2_MULTIPLETURNS)
						gHitMarker |= HITMARKER_NO_PPDEDUCT;
					gBattleScripting.bank = bank;

					if (effect == 1)
						gBattlescriptCurrInstr = BattleScript_MonMadeMoveUseless_PPLoss;
					else
						gBattlescriptCurrInstr = BattleScript_AttackerCantUseMove;
				}
			}
			break;

		case ABILITYEFFECT_MOVES_BLOCK_PARTNER: //0x14
			if (gBankAttacker != PARTNER(bank)) //Can't block against partner
			{
				switch (gLastUsedAbility) {
					case ABILITY_DAZZLING:
					case ABILITY_QUEENLYMAJESTY:
						if (PriorityCalc(gBankAttacker, ACTION_USE_MOVE, move) > 0)
							effect = 1;
						break;
				}

				if (effect)
				{
					if (gBattleMons[gBankAttacker].status2 & STATUS2_MULTIPLETURNS)
						gHitMarker |= HITMARKER_NO_PPDEDUCT;
					gBattleScripting.bank = bank;
					gBattlescriptCurrInstr = BattleScript_AttackerCantUseMove;
				}
			}
			break;

		case ABILITYEFFECT_ABSORBING: // 3
			if (move == MOVE_NONE) break;

			u8 statId = 0;
			gBattleScripting.bank = bank;
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
						effect = 3;
					break;
			}

			switch (effect) {
				case 1: // Restore HP ability
					if (BATTLER_MAX_HP(bank) || IsHealBlocked(bank))
					{
						if ((gProtectStructs[gBankAttacker].notFirstStrike))
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

						gBattleMoveDamage = MathMax(1, GetBaseMaxHP(bank) / 4);
						gBattleMoveDamage *= -1;
					}
					break;

				case 2: // Boost Stat ability
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

						gBattleScripting.statChanger = statId | INCREASE_1;
					}
					break;

				case 3: //Flash Fire
					if (!(gBattleResources->flags->flags[bank] & RESOURCE_FLAG_FLASH_FIRE))
						gBattleCommunication[MULTISTRING_CHOOSER] = 0;
					else
						gBattleCommunication[MULTISTRING_CHOOSER] = 1;

					if (gProtectStructs[gBankAttacker].notFirstStrike)
						gBattlescriptCurrInstr = BattleScript_FlashFireBoost;
					else
						gBattlescriptCurrInstr = BattleScript_FlashFireBoost_PPLoss;
			}

			break;
		case ABILITYEFFECT_CONTACT: //After being hit by a move. Not necessarilly contact.
			if (SheerForceCheck() && gLastUsedAbility != ABILITY_ILLUSION) //Sheer Force negates all these abilities
				break;

			gBattleScripting.bank = bank;

			switch (gLastUsedAbility)
			{
			case ABILITY_COLORCHANGE:
				if (MOVE_HAD_EFFECT
				&& TOOK_DAMAGE(bank)
				&& move != MOVE_STRUGGLE
				&& SPLIT(move) != SPLIT_STATUS
				&& !IsOfType(bank, moveType)
				&& BATTLER_ALIVE(bank)
				&& gBankAttacker != bank)
				{
					SET_BATTLER_TYPE(bank, moveType);
					PREPARE_TYPE_BUFFER(gBattleTextBuff1, moveType);
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_AbilityChangedTypeContact;
					effect++;
				}
				break;

			case ABILITY_IRONBARBS:
			case ABILITY_ROUGHSKIN:
				if (MOVE_HAD_EFFECT
				&& TOOK_DAMAGE(bank)
				&& BATTLER_ALIVE(gBankAttacker)
				&& gBankAttacker != bank
				&& ABILITY(gBankAttacker) != ABILITY_MAGICGUARD
				&& CheckContact(move, gBankAttacker))
				{
					gBattleMoveDamage = MathMax(1, GetBaseMaxHP(gBankAttacker) / 8);
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_RoughSkinActivates;
					effect++;
				}
				break;

			case ABILITY_EFFECTSPORE:
				if (MOVE_HAD_EFFECT
				&& TOOK_DAMAGE(bank)
				&& BATTLER_ALIVE(gBankAttacker)
				&& gBankAttacker != bank
				&& CheckContact(move, gBankAttacker)
				&& IsAffectedByPowder(gBankAttacker)
				&& Random() % 10 == 0)
				{
					do
					{
						gBattleCommunication[MOVE_EFFECT_BYTE] = Random() & 3;
					} while (gBattleCommunication[MOVE_EFFECT_BYTE] == 0);

					switch (gBattleCommunication[MOVE_EFFECT_BYTE]) {
						case MOVE_EFFECT_SLEEP:
							if (CanBePutToSleep(gBankAttacker, TRUE))
								++effect;
							break;
						case MOVE_EFFECT_POISON:
							if (CanBePoisoned(gBankAttacker, bank, TRUE))
								++effect;
							break;
						case MOVE_EFFECT_BURN: //Gets changed to Paralysis
							gBattleCommunication[MOVE_EFFECT_BYTE] = MOVE_EFFECT_PARALYSIS;
							if (CanBeParalyzed(gBankAttacker, TRUE))
								++effect;
							break;
					}

					if (effect)
					{
						gBattleCommunication[MOVE_EFFECT_BYTE] |= MOVE_EFFECT_AFFECTS_USER;
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_AbilityApplySecondaryEffect;
						gHitMarker |= HITMARKER_IGNORE_SAFEGUARD; //Safeguard checked earlier
					}
					else
						gBattleCommunication[MOVE_EFFECT_BYTE] = 0;
				}
				break;

			case ABILITY_POISONPOINT:
				if (MOVE_HAD_EFFECT
				&& TOOK_DAMAGE(bank)
				&& BATTLER_ALIVE(gBankAttacker)
				&& gBankAttacker != bank
				&& CheckContact(move, gBankAttacker)
				&& CanBePoisoned(gBankAttacker, gBankTarget, TRUE)
				&& umodsi(Random(), 3) == 0)
				{
					gBattleCommunication[MOVE_EFFECT_BYTE] = MOVE_EFFECT_AFFECTS_USER | MOVE_EFFECT_POISON;
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_AbilityApplySecondaryEffect;
					gHitMarker |= HITMARKER_IGNORE_SAFEGUARD; //Safeguard checked earlier
					effect++;
				}
				break;

			case ABILITY_STATIC:
				if (MOVE_HAD_EFFECT
				&& TOOK_DAMAGE(bank)
				&& BATTLER_ALIVE(gBankAttacker)
				&& gBankAttacker != bank
				&& CheckContact(move, gBankAttacker)
				&& CanBeParalyzed(gBankAttacker, TRUE)
				&& umodsi(Random(), 3) == 0)
				{
					gBattleCommunication[MOVE_EFFECT_BYTE] = MOVE_EFFECT_AFFECTS_USER | MOVE_EFFECT_PARALYSIS;
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_AbilityApplySecondaryEffect;
					gHitMarker |= HITMARKER_IGNORE_SAFEGUARD; //Safeguard checked earlier
					effect++;
				}
				break;

			case ABILITY_FLAMEBODY:
				if (MOVE_HAD_EFFECT
				&& TOOK_DAMAGE(bank)
				&& BATTLER_ALIVE(gBankAttacker)
				&& gBankAttacker != bank
				&& CheckContact(move, gBankAttacker)
				&& CanBeBurned(gBankAttacker, TRUE)
				&& umodsi(Random(), 3) == 0)
				{
					gBattleCommunication[MOVE_EFFECT_BYTE] = MOVE_EFFECT_AFFECTS_USER | MOVE_EFFECT_BURN;
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_AbilityApplySecondaryEffect;
					gHitMarker |= HITMARKER_IGNORE_SAFEGUARD; //Safeguard checked earlier
					effect++;
				}
				break;

			case ABILITY_CUTECHARM:
				if (MOVE_HAD_EFFECT
				&& TOOK_DAMAGE(bank)
				&& BATTLER_ALIVE(gBankAttacker)
				&& BATTLER_ALIVE(bank)
				&& gBankAttacker != bank
				&& CheckContact(move, gBankAttacker)
				&& umodsi(Random(), 3) == 0
				&& ABILITY(gBankAttacker) != ABILITY_OBLIVIOUS
				&& ABILITY(gBankAttacker) != ABILITY_AROMAVEIL
				&& !(IS_DOUBLE_BATTLE && ABILITY(PARTNER(gBankAttacker)) == ABILITY_AROMAVEIL)
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
				&& BATTLER_ALIVE(bank)
				&& gBankAttacker != bank
				&& moveType == TYPE_DARK
				&& gBattleMons[bank].statStages[STAT_ATK - 1] < 12)
				{
					gBattleScripting.statChanger = STAT_ATK | INCREASE_1;
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_TargetAbilityStatRaise;
					effect++;
				}
				break;

			case ABILITY_RATTLED:
				if (MOVE_HAD_EFFECT
				&& TOOK_DAMAGE(bank)
				&& BATTLER_ALIVE(bank)
				&& gBankAttacker != bank
				&& (moveType == TYPE_DARK || moveType == TYPE_BUG || moveType == TYPE_GHOST)
				&& gBattleMons[bank].statStages[STAT_SPEED - 1] < 12)
				{
					gBattleScripting.statChanger = STAT_SPEED | INCREASE_1;
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_TargetAbilityStatRaise;
					effect++;
				}
				break;

			case ABILITY_WEAKARMOR:
				if (MOVE_HAD_EFFECT
				&& TOOK_DAMAGE(bank)
				&& CalcMoveSplit(gBankAttacker, gCurrentMove) == SPLIT_PHYSICAL
				&& BATTLER_ALIVE(bank)
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
				&& BATTLER_ALIVE(gBankAttacker)
				&& gBankAttacker != bank
				&& ABILITY(gBankAttacker) != ABILITY_AROMAVEIL
				&& !(IS_DOUBLE_BATTLE && ABILITY(PARTNER(gBankAttacker)) == ABILITY_AROMAVEIL)
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
				&& BATTLER_ALIVE(gBankAttacker)
				&& gBankAttacker != bank
				&& CheckContact(move, gBankAttacker))
				{
					switch (ABILITY(gBankAttacker)) {
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
						case ABILITY_ICEFACE:
							break;
						default:
							BattleScriptPushCursor();
							gBattlescriptCurrInstr = BattleScript_MummyActivates;
							effect++;
							break;
					}
				}
				break;

			case ABILITY_WANDERING_SPIRIT:
				if (MOVE_HAD_EFFECT
				&& TOOK_DAMAGE(bank)
				&& (BATTLER_ALIVE(gBankAttacker) || BATTLER_ALIVE(gBankTarget))
				&& gBankAttacker != bank
				&& CheckContact(move, gBankAttacker))
				{
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_WanderingSpiritActivates;
					effect++;
					break;
				}
				break;

			case ABILITY_ANGERPOINT:
				if (MOVE_HAD_EFFECT
				&& TOOK_DAMAGE(bank)
				&& gCritMultiplier > BASE_CRIT_MULTIPLIER
				&& BATTLER_ALIVE(bank)
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
				&& BATTLER_ALIVE(gBankAttacker)
				&& gBankAttacker != bank
				&& ABILITY(gBankAttacker) != ABILITY_MAGICGUARD
				&& CheckContact(move, gBankAttacker)
				&& !BATTLER_ALIVE(bank)
				&& !ABILITY_PRESENT(ABILITY_DAMP))
				{
					gBattleMoveDamage = MathMax(1, GetBaseMaxHP(gBankAttacker) / 4);
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_RoughSkinActivates;
					effect++;
				}
				break;

			case ABILITY_BERSERK:
				if (MOVE_HAD_EFFECT
				&& TOOK_DAMAGE(bank)
				&& BATTLER_ALIVE(bank)
				&& gBattleMons[bank].hp < gBattleMons[bank].maxHP / 2
				&& gBattleMons[bank].hp + gHpDealt > gBattleMons[bank].maxHP / 2 //Hp fell below half
				&& gBattleMons[bank].statStages[STAT_SPATK - 1] < 12)
				{
					gBattleScripting.statChanger = STAT_SPATK | INCREASE_1;
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_TargetAbilityStatRaise;
					effect++;
				}
				break;

			case ABILITY_TANGLINGHAIR:
			case ABILITY_GOOEY:
				if (MOVE_HAD_EFFECT
				&& TOOK_DAMAGE(bank)
				&& CheckContact(move, gBankAttacker)
				&& BATTLER_ALIVE(gBankAttacker)
				&& gBankAttacker != bank
				&& (STAT_CAN_FALL(gBankAttacker, STAT_SPD) || ABILITY(gBankAttacker) == ABILITY_MIRRORARMOR))
				{
					gBattleScripting.statChanger = STAT_SPD | DECREASE_1;
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_GooeyActivates;
					effect++;
				}
				break;

			case ABILITY_INNARDSOUT:
				if (MOVE_HAD_EFFECT
				&& TOOK_DAMAGE(bank)
				&& BATTLER_ALIVE(gBankAttacker)
				&& gBankAttacker != bank
				&& ABILITY(gBankAttacker) != ABILITY_MAGICGUARD
				&& !BATTLER_ALIVE(bank))
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
				&& BATTLER_ALIVE(bank)
				&& gBankAttacker != bank
				&& gBattleMons[bank].statStages[STAT_DEF - 1] < 12)
				{
					gBattleScripting.statChanger = STAT_DEF | INCREASE_1;
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_TargetAbilityStatRaise;
					effect++;
				}
				break;

			case ABILITY_WATERCOMPACTION:
				if (MOVE_HAD_EFFECT
				&& TOOK_DAMAGE(bank)
				&& BATTLER_ALIVE(bank)
				&& gBankAttacker != bank
				&& moveType == TYPE_WATER
				&& gBattleMons[bank].statStages[STAT_DEF - 1] < 12)
				{
					gBattleScripting.statChanger = STAT_DEF | INCREASE_2;
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_TargetAbilityStatRaise;
					effect++;
				}
				break;

			case ABILITY_STEAMENGINE:
				if (MOVE_HAD_EFFECT
				&& TOOK_DAMAGE(bank)
				&& BATTLER_ALIVE(bank)
				&& gBankAttacker != bank
				&& (moveType == TYPE_WATER || moveType == TYPE_FIRE)
				&& STAT_STAGE(bank, STAT_SPEED) < STAT_STAGE_MAX)
				{
					gBattleScripting.statChanger = STAT_SPEED | INCREASE_6;
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
					gNewBS->disguisedAs[bank] = 0;
					gActiveBattler = bank;
					EmitDataTransfer(0, &gStatuses3[gActiveBattler], 4, &gStatuses3[gActiveBattler]);
					MarkBufferBankForExecution(gActiveBattler);
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_IllusionBroken;
					effect++;
				}
				break;

			case ABILITY_COTTONDOWN:
				if (MOVE_HAD_EFFECT
				&& TOOK_DAMAGE(bank))
				{
				}
				break;

			case ABILITY_SANDSPIT:
				if (MOVE_HAD_EFFECT
				&& TOOK_DAMAGE(bank)
				&& gBankAttacker != bank
				&& !(gBattleWeather & (WEATHER_SANDSTORM_ANY | WEATHER_PRIMAL_ANY | WEATHER_CIRCUS)))
				{
					effect = ActivateWeatherAbility(WEATHER_SANDSTORM_PERMANENT | WEATHER_SANDSTORM_TEMPORARY,
													ITEM_EFFECT_SMOOTH_ROCK, bank, B_ANIM_SANDSTORM_CONTINUES, 1, TRUE);
				}
				break;

			case ABILITY_PERISH_BODY:
				if (MOVE_HAD_EFFECT
				&& TOOK_DAMAGE(bank)
				&& gBankAttacker != bank
				&& CheckContact(move, gBankAttacker))
				{
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_PerishBody;
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
				case ABILITY_OBLIVIOUS:
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
						gDisableStructs[bank].tauntTimer = 0;
						gBattleMons[bank].status2 &= ~(STATUS2_INFATUATION);
						break;
					}

					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_AbilityCuredStatus;
					gActiveBattler = gBattleScripting.bank = bank;
					EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 4, &gBattleMons[gActiveBattler].status1);
					MarkBufferBankForExecution(gActiveBattler);
					return effect;
				}
			}
			break;
		case ABILITYEFFECT_FORECAST: // 6
			break;
		case ABILITYEFFECT_SYNCHRONIZE: // 7
			if (gLastUsedAbility == ABILITY_SYNCHRONIZE
			&& gNewBS->synchronizeTarget[bank] > 0)
			{
				if (bank != gNewBS->synchronizeTarget[bank] - 1) //Didn't status itself
				{
					gHitMarker &= ~(HITMARKER_SYNCHRONISE_EFFECT);
					gBattleStruct->synchronizeMoveEffect &= ~(MOVE_EFFECT_AFFECTS_USER | MOVE_EFFECT_CERTAIN);
					gBattleCommunication[MOVE_EFFECT_BYTE] = gBattleStruct->synchronizeMoveEffect;
					gBattleScripting.bank = bank;

					gNewBS->backupSynchronizeBanks[0] = gBankAttacker;
					gNewBS->backupSynchronizeBanks[1] = gBankTarget;

					gBankAttacker = bank;
					gBankTarget = gNewBS->synchronizeTarget[bank] - 1;
					gNewBS->synchronizeTarget[bank] = 0;

					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_SynchronizeActivates;
					gHitMarker |= HITMARKER_IGNORE_SAFEGUARD;
					effect++;
				}

				gNewBS->synchronizeTarget[bank] = 0;
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
					effect = IsMudSportActive();
					break;
				case 0xFE:
					effect = IsWaterSportActive();
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

static u8 CalcMovePowerForForewarn(u16 move)
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

const u16 gWeatherAbilityStrings[] =
{
	STRINGID_STARTEDTORAIN, STRINGID_SANDSTORMBREWED, STRINGID_SUNLIGHTGOTBRIGHT, STRINGID_STARTEDHAIL, 0x184
};

static u8 ActivateWeatherAbility(u16 flags, u16 item, u8 bank, u8 animArg, u8 stringIndex, bool8 moveTurn)
{
	#ifdef INFINITE_WEATHER
		gBattleWeather = flags;
		gWishFutureKnock.weatherDuration = 0;
		++item; //So it compiles warning free
	#else
		gBattleWeather = (flags & ~(WEATHER_PERMANENT_ANY)); //Remove any weather permanence
		if (ITEM_EFFECT(bank) == item)
			gWishFutureKnock.weatherDuration = 8;
		else
			gWishFutureKnock.weatherDuration = 5;
	#endif

	gBattleCommunication[MULTISTRING_CHOOSER] = stringIndex;
	gBattleScripting.animArg1 = animArg;

	if (moveTurn)
	{
		BattleScriptPushCursor();
		gBattlescriptCurrInstr = BattleScript_NewWeatherAbilityActivatesCall;
	}
	else
	{
		BattleScriptPushCursorAndCallback(BattleScript_NewWeatherAbilityActivates);
	}

	gBattleScripting.bank = bank;
	return TRUE;
}

static u8 TryActivateTerrainAbility(u8 terrain, u8 anim, u8 bank)
{
	u8 effect = 0;

	if (gBattleTypeFlags & BATTLE_TYPE_BATTLE_CIRCUS && gBattleCircusFlags & BATTLE_CIRCUS_TERRAIN)
		return effect; //Can't be removed

	if (gTerrainType != terrain)
	{
		gTerrainType = terrain;
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
				gBattleStringLoader = ElectricTerrainSetString;
				break;
			case GRASSY_TERRAIN:
				gBattleStringLoader = GrassyTerrainSetString;
				break;
			case MISTY_TERRAIN:
				gBattleStringLoader = MistyTerrainSetString;
				break;
			case PSYCHIC_TERRAIN:
				gBattleStringLoader = PsychicTerrainSetString;
				break;
		}

		gBattleScripting.animArg1 = anim;
		gBattleScripting.bank = bank;
		*((u32*) gSeedHelper) = (u32) gBattleStringLoader; //Backup String
		BattleScriptPushCursorAndCallback(BattleScript_TerrainFromAbility);
		++effect;
	}
	return effect;
}

static bool8 ImmunityAbilityCheck(u8 bank, u32 status, u8* string)
{

	if (gBattleMons[bank].status1 & status)
	{
		StringCopy(gBattleTextBuff1, string);
		gBattleMons[bank].status1 = 0;
		gBattleMons[bank].status2 &= ~(STATUS2_NIGHTMARE);
		BattleScriptPushCursorAndCallback(BattleScript_AbilityCuredStatusEnd3);
		gBattleScripting.bank = bank;
		gActiveBattler = bank;
		EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 4, &gBattleMons[gActiveBattler].status1);
		MarkBufferBankForExecution(gActiveBattler);
		return TRUE;
	}
	return FALSE;
}

static bool8 AllMainStatsButOneAreMinned(bank_t bank)
{
	for (u8 i = STAT_ATK, counter = 0; i <= STAT_SPDEF; ++i) //No Acc of Evsn
	{
		if (STAT_STAGE(bank, i) > 0)
		{
			if (++counter > 1)
				return FALSE;
		}
	}

	return TRUE;
}

void LoadProperAbilityBattleData(void)
{
	gBattleMons[gActiveBattler].ability = GetMonAbility(GetBankPartyData(gActiveBattler));
}

void ApplyFlashFireBoost(void)
{
	gBattleResources->flags->flags[gBattleScripting.bank] |= RESOURCE_FLAG_FLASH_FIRE;
}

//Illusion Updates////////////////////////////////////////////////////////////////////////////////////
pokemon_t* UpdateNickForIllusion(pokemon_t* mon)
{
	u8 bank = GetBankFromPartyData(mon);

	if (bank >= 6) //Safety Measure
		return mon;

	return GetIllusionPartyData(bank);
}

u8 UpdatePokeBallForIllusion(u8 bank)
{
	if (IsDynamaxed(bank))
		return BALL_TYPE_DYNAMAX_BALL;

	return GetMonData(GetIllusionPartyData(bank), MON_DATA_POKEBALL, NULL);
}

species_t TryUpdateIllusionAltitude(u8 bank)
{
	if (gBattleMons[bank].status2 & STATUS2_TRANSFORMED)
		return SPECIES_NONE;

	return GetIllusionPartyData(bank)->species;
}

//Ability Pop-Up//////////////////////////////////////////////////////////////////////////////////////
//Credit to the Emerald Battle Engine Upgrade 2.0 by DizzyEgg for the original code.

// for sprite
#define tOriginalX	  data[0]
#define tHide		   data[1]
#define tFrames		 data[2]
#define tRightToLeft	data[3]
#define tBattlerId	  data[4]
#define tIsMain		 data[5]

// for task
#define tSpriteId1	  data[6]
#define tSpriteId2	  data[7]

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
	FillWindowPixelBuffer(*windowId, PIXEL_FILL(color1));

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
	int i;
	u8 lastChar;
	u8* textPtr;
	u8 monName[POKEMON_NAME_LENGTH + 3] = {0};
	u8* nick = GetIllusionPartyData(battlerId)->nickname;

	for (i = 0; i < POKEMON_NAME_LENGTH; ++i)
	{
		monName[i] = nick[i];

		if (nick[i] == 0xFF || i + 1 == POKEMON_NAME_LENGTH) //End of string
			break;
	}

	textPtr = monName + i + 1;

	if (*(textPtr - 1) == 0xFF)
		--textPtr;

	lastChar = *(textPtr - 1);

	//Make the string say "[NAME]'s" instead of "[NAME]"
	textPtr[0] = 0xB4; //'
	++textPtr;
	if (lastChar != PC_S && lastChar != PC_s) //Proper grammar for names ending in "s"
	{
		textPtr[0] = 0xE7; //s
		++textPtr;
	}
	textPtr[0] = EOS;

	PrintOnAbilityPopUp((const u8*) monName,
						(void*)(OBJ_VRAM0) + (gSprites[spriteId1].oam.tileNum * 32),
						(void*)(OBJ_VRAM0) + (gSprites[spriteId2].oam.tileNum * 32),
						5, 0,
						0,
						2, 7, 1);
}

static void PrintAbilityOnAbilityPopUp(u32 ability, u8 spriteId1, u8 spriteId2)
{
	const u8* abilityName = GetAbilityName(ability);

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
+ ((((y) - ((y / 8) * 8))) * 4)					\
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
	{PIXEL_COORDS_TO_OFFSET(0, 47), 8},
	{PIXEL_COORDS_TO_OFFSET(0, 48), 8},
	{PIXEL_COORDS_TO_OFFSET(8, 45), 8},
	{PIXEL_COORDS_TO_OFFSET(8, 46), 8},
	{PIXEL_COORDS_TO_OFFSET(8, 47), 8},
	{PIXEL_COORDS_TO_OFFSET(8, 48), 8},
	{PIXEL_COORDS_TO_OFFSET(16, 45), 8},
	{PIXEL_COORDS_TO_OFFSET(16, 46), 8},
	{PIXEL_COORDS_TO_OFFSET(16, 47), 8},
	{PIXEL_COORDS_TO_OFFSET(16, 48), 8},
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
	u8 ability = gAbilityPopUpHelper; //Preceded by transfer of proper ability

	LoadSpriteSheet((const struct SpriteSheet*) &gBattleAnimPicTable[ANIM_TAG_ABILITY_POP_UP - ANIM_SPRITES_START]);
	LoadSpritePalette((const struct SpritePalette*) &gBattleAnimPaletteTable[ANIM_TAG_ABILITY_POP_UP - ANIM_SPRITES_START]);

	gNewBS->activeAbilityPopUps |= gBitTable[gBattleAnimAttacker];
	battlerPosition = GetBattlerPosition(gBattleAnimAttacker);

	if (IS_DOUBLE_BATTLE)
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

	gNewBS->abilityPopUpIds[gBattleAnimAttacker][0] = spriteId1;
	gNewBS->abilityPopUpIds[gBattleAnimAttacker][1] = spriteId2;

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
			if ((!sprite->tRightToLeft && (sprite->pos1.x += 4) >= sprite->tOriginalX + ABILITY_POP_UP_POS_X_SLIDE + 16)
			||   (sprite->tRightToLeft && (sprite->pos1.x -= 4) <= sprite->tOriginalX - ABILITY_POP_UP_POS_X_SLIDE - 16))
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
		u8 bank = gSprites[gTasks[taskId].tSpriteId1].tBattlerId;

		gNewBS->abilityPopUpIds[bank][0] = 0;
		gNewBS->abilityPopUpIds[bank][1] = 0;
		FreeSpriteTilesByTag(ANIM_TAG_ABILITY_POP_UP);
		FreeSpritePaletteByTag(ANIM_TAG_ABILITY_POP_UP);
		DestroyTask(taskId);
	}
}

void AnimTask_DestroyAbilityPopUp(u8 taskId)
{
	gSprites[gNewBS->abilityPopUpIds[gBattleAnimAttacker][0]].tFrames = 0;
	gSprites[gNewBS->abilityPopUpIds[gBattleAnimAttacker][1]].tFrames = 0;
	DestroyAnimVisualTask(taskId);
}

void TransferAbilityPopUpHelper(void)
{
	if (gBattleExecBuffer)
	{
		gBattlescriptCurrInstr -= 5;
		return;
	}

	TransferAbilityPopUp(gBattleScripting.bank, CopyAbility(gBattleScripting.bank));
	gLastUsedAbility = gAbilityPopUpHelper;
}

void TransferAbilityPopUpHelperAsTrace(void)
{
	if (gBattleExecBuffer)
	{
		gBattlescriptCurrInstr -= 5;
		return;
	}

	TransferAbilityPopUp(gBattleScripting.bank, ABILITY_TRACE);
}

void TransferAbilityPopUpHelperAsImposter(void)
{
	if (gBattleExecBuffer)
	{
		gBattlescriptCurrInstr -= 5;
		return;
	}

	TransferAbilityPopUp(gBattleScripting.bank, ABILITY_IMPOSTER);
}

void TransferAbilityPopUpHelperAsWanderingSpirit(void)
{
	if (gBattleExecBuffer)
	{
		gBattlescriptCurrInstr -= 5;
		return;
	}

	TransferAbilityPopUp(gBattleScripting.bank, ABILITY_WANDERING_SPIRIT);
}

void TransferAbilityPopUp(u8 bank, u8 ability)
{
	gActiveBattler = bank;
	gAbilityPopUpHelper = ability;

	EmitDataTransfer(0, &gAbilityPopUpHelper, 1, &gAbilityPopUpHelper);
	MarkBufferBankForExecution(gActiveBattler);
}

void TryRemoveIntimidateAbilityPopUp(void)
{
	if (gNewBS->intimidateActive > 0)
	{
		BattleScriptPushCursor();
		gBattlescriptCurrInstr = BattleScript_AbilityPopUpRevert - 5;
		gBattleScripting.bank = gNewBS->intimidateActive - 1;
		gNewBS->intimidateActive -= 10; //Intimidate is still active but pop-up is hidden
	}
}

void TryHideActiveAbilityPopUps(void)
{
	for (u8 i = 0; i < gBattlersCount; ++i)
	{
		if (gNewBS->activeAbilityPopUps & gBitTable[i])
		{
			BattleScriptPushCursor();
			gBattlescriptCurrInstr = BattleScript_AbilityPopUpRevert - 5;
			gBattleScripting.bank = i;
		}
	}
}

void RemoveIntimidateActive(void)
{
	gNewBS->intimidateActive = 0;
}

void TryReactiveIntimidatePopUp(void)
{
	if (gNewBS->intimidateActive < 0) //Pop-up was hidden due to ability like Clear Body
	{
		BattleScriptPushCursor();
		gBattlescriptCurrInstr = BattleScript_AbilityPopUp - 5;
		gNewBS->intimidateActive += 10;
		gBattleScripting.bank = gNewBS->intimidateActive - 1;
	}
}

void LoadIntimidateBankIntoTarget(void)
{
	if (gNewBS->intimidateActive > 0)
		gBankTarget = gNewBS->intimidateActive - 1;
	else if (gNewBS->intimidateActive < 0)
		gBankTarget = (gNewBS->intimidateActive + 10) - 1;
}

//Switch-in abilities that affect a knocked out target
//after using U-Turn/Volt Switch are delayed until a
//new Pokemon is sent out.
void SetSkipCertainSwitchInAbilities(void)
{
	if (BATTLER_ALIVE(FOE(gBankAttacker)))
		return;

	if (IS_DOUBLE_BATTLE && BATTLER_ALIVE(PARTNER(FOE(gBankAttacker))))
		return;

	//Only set the bit if no enemies are alive on the field
	gNewBS->skipCertainSwitchInAbilities = TRUE;
}
