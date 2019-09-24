#include "defines.h"
#include "defines_battle.h"
#include "../include/event_object_movement.h"
#include "../include/fieldmap.h"
#include "../include/field_player_avatar.h"
#include "../include/field_weather.h"
#include "../include/script.h"
#include "../include/wild_encounter.h"
#include "../include/random.h"

#include "../include/constants/flags.h"
#include "../include/constants/items.h"
#include "../include/constants/maps.h"
#include "../include/constants/vars.h"

#include "../include/new/battle_start_turn_start.h"
#include "../include/new/build_pokemon.h"
#include "../include/new/dns.h"
#include "../include/new/util.h"
#include "../include/new/roamer.h"
#include "../include/new/wild_encounter.h"
/*
wild_encounter.c
	handles functions related to wild encounter probability and associated features
*/

//Chain Fishing

#define NUM_TANOBY_CHAMBERS 7

struct EncounterRate
{
	u32 randomNum;
	u16 previousMetatileBehavior;
	u16 rateBonus;
	u8 maxNoEncounterStepCounter;
	u8 abilityRateModifier;
	u16 firstMonHeldItem;
};

extern struct EncounterRate gEncounterRate;

extern u8 gUnownDistributionByChamber[NUM_TANOBY_CHAMBERS][12]; //[NUM_ROOMS][NUM_WILD_INDEXES]
extern const struct WildPokemonHeader gWildMonMorningHeaders[];
extern const struct WildPokemonHeader gWildMonEveningHeaders[];
extern const struct WildPokemonHeader gWildMonNightHeaders[];
extern const struct SwarmData gSwarmTable[];
extern const u16 gSwarmTableLength;

extern bool8 CheckAndSetDailyEvent(u16 eventVar, bool8 setDailyEventVar);

//This file's functions
static u8 ChooseWildMonLevel(const struct WildPokemon* wildPokemon);
static const struct WildPokemonHeader* GetCurrentMapWildMonHeader(void);
static const struct WildPokemonHeader* GetCurrentMapWildMonDaytimeHeader(void);
static u8 PickWildMonNature(void);
static bool8 TryGenerateWildMon(const struct WildPokemonInfo* wildMonInfo, u8 area, u8 flags);
static species_t GenerateFishingWildMon(const struct WildPokemonInfo* wildMonInfo, u8 rod);
static bool8 DoWildEncounterRateTest(u32 encounterRate, bool8 ignoreAbility);
static bool8 DoWildEncounterRateDiceRoll(u16 encounterRate);
static bool8 IsAbilityAllowingEncounter(u8 level);
static bool8 TryGetRandomWildMonIndexByType(const struct WildPokemon* wildMon, u8 type, u8 numMon, u8* monIndex);
static bool8 TryGetAbilityInfluencedWildMonIndex(const struct WildPokemon* wildMon, u8 type, u8 ability, u8* monIndex);
static void CreateScriptedWildMon(u16 species, u8 level, u16 item, u16* specialMoves, bool8 firstMon);
static const struct WildPokemonInfo* LoadProperMonsPointer(const struct WildPokemonHeader* header, const u8 type);

#ifdef SCALE_WILD_POKEMON_LEVELS_FLAG
static u8 GetLowestMonLevel(const struct Pokemon* const party);
#endif

static u8 ChooseWildMonLevel(const struct WildPokemon* wildPokemon)
{
	u8 min;
	u8 max;
	u8 range;
	u8 rand;
	u8 fluteBonus;

	#ifdef SCALE_WILD_POKEMON_LEVELS_FLAG
	if (FlagGet(SCALE_WILD_POKEMON_LEVELS_FLAG))
	{
		min = GetLowestMonLevel(gPlayerParty);
		max = GetLowestMonLevel(gPlayerParty);
	}
	else
	#endif
	//Make sure minimum level is less than maximum level
	if (wildPokemon->maxLevel >= wildPokemon->minLevel)
	{
		min = wildPokemon->minLevel;
		max = wildPokemon->maxLevel;
	}
	else
	{
		min = wildPokemon->maxLevel;
		max = wildPokemon->minLevel;
	}
	range = max - min + 1;
	rand = Random() % range;

	if (FlagGet(FLAG_SYS_BLACK_FLUTE))
	{
		fluteBonus = (Random() % 3 + 1);
		max = MathMin(MAX_LEVEL, max + fluteBonus);
		min = MathMin(MAX_LEVEL, min + fluteBonus);
	}
	else if (FlagGet(FLAG_SYS_WHITE_FLUTE))
	{
		fluteBonus = (Random() % 3 + 1);

		if (fluteBonus < max)
			max -= fluteBonus;
		else
			max = 1;

		if (fluteBonus < min)
			min -= fluteBonus;
		else
			min = 1;
	}

	//Check ability for max level mon
	if (!GetMonData(&gPlayerParty[0], MON_DATA_IS_EGG, NULL))
	{
		u8 ability = GetPartyAbility(&gPlayerParty[0]);
		if (ability == ABILITY_HUSTLE || ability == ABILITY_VITALSPIRIT || ability == ABILITY_PRESSURE)
		{
			if (Random() % 2 == 0)
				return max;

			if (rand != 0)
				--rand;
		}
	}

	return min + rand;
}

#define MAP_ALTERING_CAVE ((1 << 8) | 122)

static const struct WildPokemonHeader* GetCurrentMapWildMonHeader(void)
{
	if (gWildDataSwitch != NULL)
	{
		if ((u32) gWildDataSwitch >= 0x8000000) //Real Pointer
			return (const struct WildPokemonHeader*) gWildDataSwitch;
		else
			gWildDataSwitch = NULL;
	}

	#ifdef TIME_ENABLED
		u32 i;

		const struct WildPokemonHeader* headerTable = NULL;

		if (IsNightTime())
			headerTable = gWildMonNightHeaders;
		else if (IsMorning())
			headerTable = gWildMonMorningHeaders;
		else if (IsEvening())
			headerTable = gWildMonEveningHeaders;

		if (headerTable != NULL) //Not Daytime
		{
			for (i = 0; headerTable[i].mapGroup != 0xFF; ++i)
			{
				if (headerTable[i].mapGroup == gSaveBlock1->location.mapGroup
				&&  headerTable[i].mapNum == gSaveBlock1->location.mapNum)
					return &headerTable[i];
			}
		}
	#endif

	return GetCurrentMapWildMonDaytimeHeader();
}

static const struct WildPokemonHeader* GetCurrentMapWildMonDaytimeHeader(void)
{
	u32 i;

	if (gWildDataSwitch != NULL)
	{
		if ((u32) gWildDataSwitch >= 0x8000000) //Real Pointer
			return (const struct WildPokemonHeader*) gWildDataSwitch;
		else
			gWildDataSwitch = NULL;
	}

	for (i = 0; gWildMonHeaders[i].mapGroup != 0xFF; ++i)
	{
		const struct WildPokemonHeader* wildHeader = &gWildMonHeaders[i];

		if (wildHeader->mapGroup == gSaveBlock1->location.mapGroup
		&&  wildHeader->mapNum   == gSaveBlock1->location.mapNum)
		{
			#ifdef ALTERING_CAVE_ENABLED
			if (gSaveBlock1->location.mapGroup == MAP_GROUP(ALTERING_CAVE)
			&&  gSaveBlock1->location.mapNum == MAP_NUM(ALTERING_CAVE))
			{
				u16 alteringCaveId = VarGet(VAR_ALTERING_CAVE_WILD_SET);
				if (alteringCaveId > 8)
					alteringCaveId = 0;

				i += alteringCaveId;
			}
			#endif

			#ifdef TANOBY_RUINS_ENABLED
			if (!CanEncounterUnownInTanobyRuins()) //A function that returns true if the
				return NULL;					 //Tanoby Key flag has been set.
			#endif								 //If it hasn't, and you're in the ruins, then
												 //return false to indicate no Pokemon can be found.
			return &gWildMonHeaders[i];
		}
	}

	return NULL;
}


static u8 PickWildMonNature(void)
{
	//Check Synchronize for a pokemon with the same ability
	if (!GetMonData(&gPlayerParty[0], MON_DATA_IS_EGG, NULL)
	&&  GetPartyAbility(&gPlayerParty[0]) == ABILITY_SYNCHRONIZE
	&&  umodsi(Random(), 2) == 0)
	{
		return umodsi(gPlayerParty[0].personality, 25);
	}

	// random nature
	return umodsi(Random(), 25);
}

void CreateWildMon(u16 species, u8 level, u8 monHeaderIndex, bool8 purgeParty)
{
	u8 enemyMonIndex = 0;
	bool8 checkCuteCharm = TRUE;

	if (purgeParty)
		ZeroEnemyPartyMons();
	else
		enemyMonIndex = 1;

	switch (gBaseStats[species].genderRatio) {
		case MON_MALE:
		case MON_FEMALE:
		case MON_GENDERLESS:
			checkCuteCharm = FALSE;
			break;
	}

	if (checkCuteCharm
	&& !GetMonData(&gPlayerParty[0], MON_DATA_IS_EGG, NULL)
	&&  GetPartyAbility(&gPlayerParty[0]) == ABILITY_CUTECHARM
	&&  umodsi(Random(), 3))
	{
		u16 leadingMonSpecies = gPlayerParty[0].species;
		u32 leadingMonPersonality = gPlayerParty[0].species;
		u8 gender = GetGenderFromSpeciesAndPersonality(leadingMonSpecies, leadingMonPersonality);

		if (gender == MON_FEMALE)
			gender = MON_MALE;
		else if (gender == MON_MALE)
			gender = MON_FEMALE;
		else //Genderless
			goto REGULAR_NATURE_CREATION;

		CreateMonWithGenderNatureLetter(&gEnemyParty[enemyMonIndex], species, level, 32, gender, PickWildMonNature(), PickUnownLetter(species, monHeaderIndex));
		goto ASSIGN_HIDDEN_ABILITY;
	}
	REGULAR_NATURE_CREATION:

	CreateMonWithNatureLetter(&gEnemyParty[enemyMonIndex], species, level, 32, PickWildMonNature(), PickUnownLetter(species, monHeaderIndex));

	ASSIGN_HIDDEN_ABILITY:
	if (FlagGet(HIDDEN_ABILITY_FLAG))
		gEnemyParty[enemyMonIndex].hiddenAbility = TRUE;

	//Custom moves
	if (FlagSet(WILD_CUSTOM_MOVES_FLAG))
	{
		u16* moves = (enemyMonIndex == 0) ? &Var8000 : &Var8004;
		for (int i = 0; i < MAX_MON_MOVES; ++i)
		{
			if (moves[i] != 0xFFFF)
				gEnemyParty[enemyMonIndex].moves[i] = moves[i];
		}
	}

	//Status Inducers
	TryStatusInducer(&gEnemyParty[enemyMonIndex]);
}

u8 PickUnownLetter(unusedArg u16 species, unusedArg u8 headerIndex)
{
	#ifdef TANOBY_RUINS_ENABLED
		if (species == SPECIES_UNOWN)
		{
			u8 room = gSaveBlock1->location.mapNum - 0x1B;

			if (room >= NUM_TANOBY_CHAMBERS)
				return 0;
			else
				return gUnownDistributionByChamber[room][headerIndex] + 1;
		}
	#endif

	return 0;
}

enum
{
	WILD_AREA_LAND,
	WILD_AREA_WATER,
	WILD_AREA_ROCKS,
	WILD_AREA_FISHING,
};

#define WILD_CHECK_REPEL	0x1
#define WILD_CHECK_KEEN_EYE 0x2

void TryUpdateSwarm(void)
{
	if (CheckAndSetDailyEvent(SWARM_DAILY_EVENT_VAR, TRUE))
	{
		u16 index = Random() % gSwarmTableLength;
		VarSet(SWARM_INDEX_VAR, index);
	}
}

static bool8 TryGenerateSwarmMon(u8 level, u8 wildMonIndex, bool8 purgeParty)
{
	if (gSwarmTableLength == 0)
		return FALSE;

	u8 index = VarGet(SWARM_INDEX_VAR);
	u8 mapName = gSwarmTable[index].mapName;
	u16 species = gSwarmTable[index].species;

	if (mapName == gCurrentMapName
	&&  Random() % 100 < SWARM_CHANCE)
	{
		CreateWildMon(species, level, wildMonIndex, purgeParty);
		return TRUE;
	}

	return FALSE;
}

static bool8 TryGenerateWildMon(const struct WildPokemonInfo* wildMonInfo, u8 area, u8 flags)
{
	u8 wildMonIndex = 0;
	u8 level;

	if (TryGetAbilityInfluencedWildMonIndex(wildMonInfo->wildPokemon, TYPE_STEEL, ABILITY_MAGNETPULL, &wildMonIndex))
		goto SKIP_INDEX_SEARCH;
	if (TryGetAbilityInfluencedWildMonIndex(wildMonInfo->wildPokemon, TYPE_ELECTRIC, ABILITY_STATIC, &wildMonIndex))
		goto SKIP_INDEX_SEARCH;

	switch (area) {
		case WILD_AREA_LAND:
			wildMonIndex = ChooseWildMonIndex_Land();
			break;
		case WILD_AREA_WATER:
			wildMonIndex = ChooseWildMonIndex_WaterRock();
			break;
		case WILD_AREA_ROCKS:
			wildMonIndex = ChooseWildMonIndex_WaterRock();
			break;
	}

SKIP_INDEX_SEARCH:

	level = ChooseWildMonLevel(&wildMonInfo->wildPokemon[wildMonIndex]);

	if (flags & WILD_CHECK_REPEL && !IsWildLevelAllowedByRepel(level))
		return FALSE;

	else if (flags & WILD_CHECK_KEEN_EYE && !IsAbilityAllowingEncounter(level))
		return FALSE;

	else if (!TryGenerateSwarmMon(level, wildMonIndex, TRUE))
		CreateWildMon(wildMonInfo->wildPokemon[wildMonIndex].species, level, wildMonIndex, TRUE);

	if (FlagGet(DOUBLE_WILD_BATTLE_FLAG))
	{
		wildMonIndex = 0;

		if (TryGetAbilityInfluencedWildMonIndex(wildMonInfo->wildPokemon, TYPE_STEEL, ABILITY_MAGNETPULL, &wildMonIndex))
			goto SKIP_INDEX_SEARCH_2;
		if (TryGetAbilityInfluencedWildMonIndex(wildMonInfo->wildPokemon, TYPE_ELECTRIC, ABILITY_STATIC, &wildMonIndex))
			goto SKIP_INDEX_SEARCH_2;

		switch (area) {
			case WILD_AREA_LAND:
				wildMonIndex = ChooseWildMonIndex_Land();
				break;
			case WILD_AREA_WATER:
				wildMonIndex = ChooseWildMonIndex_WaterRock();
				break;
			case WILD_AREA_ROCKS:
				wildMonIndex = ChooseWildMonIndex_WaterRock();
				break;
		}

		SKIP_INDEX_SEARCH_2:
		level = ChooseWildMonLevel(&wildMonInfo->wildPokemon[wildMonIndex]);
		if (!TryGenerateSwarmMon(level, wildMonIndex, FALSE))
			CreateWildMon(wildMonInfo->wildPokemon[wildMonIndex].species, level, wildMonIndex, FALSE);
	}

	return TRUE;
}

static species_t GenerateFishingWildMon(const struct WildPokemonInfo* wildMonInfo, u8 rod)
{
	u8 wildMonIndex = ChooseWildMonIndex_Fishing(rod);
	u8 level = ChooseWildMonLevel(&wildMonInfo->wildPokemon[wildMonIndex]);

	CreateWildMon(wildMonInfo->wildPokemon[wildMonIndex].species, level, wildMonIndex, TRUE);

	if (FlagGet(DOUBLE_WILD_BATTLE_FLAG))
	{
		u8 wildMonIndex = ChooseWildMonIndex_Fishing(rod);
		u8 level = ChooseWildMonLevel(&wildMonInfo->wildPokemon[wildMonIndex]);
		CreateWildMon(wildMonInfo->wildPokemon[wildMonIndex].species, level, wildMonIndex, FALSE);
	}

	return wildMonInfo->wildPokemon[wildMonIndex].species;
}

void FishingWildEncounter(u8 rod)
{
	const struct WildPokemonInfo* fishingMonsInfo = LoadProperMonsData(FISHING_MONS_HEADER); //Existence check done prior

	GenerateFishingWildMon(fishingMonsInfo, rod);
	IncrementGameStat(GAME_STAT_FISHING_CAPTURES);
	BattleSetup_StartWildBattle();
}

bool8 DoesCurrentMapHaveFishingMons(void)
{
	const struct WildPokemonInfo* fishingMonsInfo = LoadProperMonsData(FISHING_MONS_HEADER);

	if (fishingMonsInfo == NULL)
		return FALSE;

	return TRUE;
}

static bool8 DoWildEncounterRateTest(u32 encounterRate, bool8 ignoreAbility)
{
	encounterRate *= 16;
	if (TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_MACH_BIKE | PLAYER_AVATAR_FLAG_ACRO_BIKE))
	{
		encounterRate = encounterRate * 80 / 100;
	}

	//encounterRate += (gEncounterRate.rateBonus * 16 / 200);

	gEncounterRate.firstMonHeldItem = gPlayerParty[0].item;
	//ApplyFluteEncounterRateMod(&encounterRate);
	ApplyCleanseTagEncounterRateMod(&encounterRate);

	if (!ignoreAbility && !GetMonData(&gPlayerParty[0], MON_DATA_IS_EGG, NULL))
	{
		u32 ability = GetPartyAbility(&gPlayerParty[0]);

		switch (ability) {
			case ABILITY_WHITESMOKE:
			case ABILITY_STENCH:
			case ABILITY_QUICKFEET:
				encounterRate /= 2;
				break;
			case ABILITY_ARENATRAP:
			case ABILITY_ILLUMINATE:
			case ABILITY_NOGUARD:
			case ABILITY_SWARM:
				encounterRate *= 2;
				break;
			case ABILITY_SANDVEIL:
				if (GetCurrentWeather() == WEATHER_SANDSTORM)
					encounterRate /= 2;
				break;
			case ABILITY_SNOWCLOAK:
				if (GetCurrentWeather() == WEATHER_STEADY_SNOW)
					encounterRate /= 2;
				break;
		}

	}

	if (encounterRate > 2880)
		encounterRate = 2880;

	return DoWildEncounterRateDiceRoll(encounterRate);
}

static bool8 DoWildEncounterRateDiceRoll(u16 encounterRate)
{
	if (Random() % 2880 < encounterRate)
		return TRUE;
	else
		return FALSE;
}

bool8 StandardWildEncounter(const u16 currMetaTileBehavior, const u16 previousMetaTileBehavior)
{
	struct Roamer* roamer;
	bool8 clearDoubleFlag = FALSE;
	const u8 lowerByte = MetatileBehavior_GetLowerBytes(currMetaTileBehavior, 4);

	if (sWildEncountersDisabled == TRUE)
		return FALSE;

	const struct WildPokemonInfo* landMonsInfo = LoadProperMonsData(LAND_MONS_HEADER);
	const struct WildPokemonInfo* waterMonsInfo = LoadProperMonsData(WATER_MONS_HEADER);

	if (FlagGet(NO_RANDOM_WILD_ENCOUNTERS_FLAG))
		return FALSE;
	else
	{
		if (lowerByte & TILE_FLAG_ENCOUNTER_TILE)
		{
			if (landMonsInfo == NULL)
				return FALSE;
			else if (MetatileBehavior_GetLowerBytes(currMetaTileBehavior, 0) != previousMetaTileBehavior && !DoGlobalWildEncounterDiceRoll())
				return FALSE;
			else if (DoWildEncounterRateTest(landMonsInfo->encounterRate, FALSE) != TRUE)
			{
				IncrementEncounterProbabilityBonus(landMonsInfo->encounterRate);
				return FALSE;
			}

			if (TryStartRoamerEncounter(ENCOUNTER_TYPE_LAND))
			{
				roamer = &gRoamers[gLastSelectedRoamer];
				if (!IsWildLevelAllowedByRepel(roamer->level))
					return FALSE;

				BattleSetup_StartRoamerBattle();
				return TRUE;
			}
			else
			{
				//Try a regular wild land encounter
				if (!FlagGet(DOUBLE_WILD_BATTLE_FLAG) //Flag hasn't already been set by user
				&&  ViableMonCount(gPlayerParty) >= 2
				&&  (lowerByte & TILE_FLAG_WILD_DOUBLE)
				&&  Random() % 100 < WILD_DOUBLE_RANDOM_CHANCE)
				{
					FlagSet(DOUBLE_WILD_BATTLE_FLAG);
					clearDoubleFlag = TRUE;
				}

				if (TryGenerateWildMon(landMonsInfo, WILD_AREA_LAND, WILD_CHECK_REPEL | WILD_CHECK_KEEN_EYE) == TRUE)
				{
					BattleSetup_StartWildBattle();
					return TRUE;
				}

				if (clearDoubleFlag)
					FlagClear(DOUBLE_WILD_BATTLE_FLAG); //Battle didn't start so restart the flag

				IncrementEncounterProbabilityBonus(landMonsInfo->encounterRate);
			}
		}
		else if (lowerByte & TILE_FLAG_SURFABLE
			 &&  TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_SURFING))
		{
			if (waterMonsInfo == NULL)
			   return FALSE;
			else if (previousMetaTileBehavior != MetatileBehavior_GetLowerBytes(currMetaTileBehavior, 0) && !DoGlobalWildEncounterDiceRoll())
				return FALSE;
			else if (DoWildEncounterRateTest(waterMonsInfo->encounterRate, FALSE) != TRUE)
				return FALSE;

			if (TryStartRoamerEncounter(ENCOUNTER_TYPE_WATER) == TRUE)
			{
				roamer = &gRoamers[gLastSelectedRoamer];
				if (!IsWildLevelAllowedByRepel(roamer->level))
					return FALSE;

				BattleSetup_StartRoamerBattle();
				return TRUE;
			}
			else // try a regular surfing encounter
			{
				if (!FlagGet(DOUBLE_WILD_BATTLE_FLAG) //Flag hasn't already been set by user
				&&  ViableMonCount(gPlayerParty) >= 2
				&&  lowerByte & TILE_FLAG_WILD_DOUBLE
				&&  Random() % 100 < WILD_DOUBLE_RANDOM_CHANCE)
				{
					FlagSet(DOUBLE_WILD_BATTLE_FLAG);
					clearDoubleFlag = TRUE;
				}

				if (TryGenerateWildMon(waterMonsInfo, WILD_AREA_WATER, WILD_CHECK_REPEL | WILD_CHECK_KEEN_EYE) == TRUE)
				{
					BattleSetup_StartWildBattle();
					return TRUE;
				}

				if (clearDoubleFlag)
					FlagClear(DOUBLE_WILD_BATTLE_FLAG); //Battle didn't start so restart the flag

				return FALSE;
			}
		}
	}

	return FALSE;
}

bool8 CheckStandardWildEncounter(u16 metatileBehavior)
{
	if (StandardWildEncounter(metatileBehavior, gEncounterRate.previousMetatileBehavior) == TRUE)
	{
		gEncounterRate.rateBonus = 0;
		gEncounterRate.maxNoEncounterStepCounter = 0;
		gEncounterRate.previousMetatileBehavior = MetatileBehavior_GetLowerBytes(metatileBehavior, 0);
		return TRUE;
	}

	gEncounterRate.previousMetatileBehavior = MetatileBehavior_GetLowerBytes(metatileBehavior, 0);
	return FALSE;
}

void RockSmashWildEncounter(void)
{
	const struct WildPokemonInfo* wildPokemonInfo = LoadProperMonsData(ROCK_SMASH_MONS_HEADER);

	if (wildPokemonInfo == NULL)
		gSpecialVar_LastResult = FALSE;
	else if (DoWildEncounterRateTest(wildPokemonInfo->encounterRate, 1) == TRUE
		  && TryGenerateWildMon(wildPokemonInfo, 2, WILD_CHECK_REPEL | WILD_CHECK_KEEN_EYE) == TRUE)
	{
		BattleSetup_StartWildBattle();
		gSpecialVar_LastResult = TRUE;
	}
	else
		gSpecialVar_LastResult = FALSE;
}

bool8 SweetScentWildEncounter(void)
{
	s16 x, y;
	const struct WildPokemonInfo* landMonsInfo = LoadProperMonsData(LAND_MONS_HEADER);
	const struct WildPokemonInfo* waterMonsInfo = LoadProperMonsData(WATER_MONS_HEADER);

	#ifdef SWEET_SCENT_ONLY_IN_CLEAR_WEATHER
		switch (GetCurrentWeather()) {
			case WEATHER_NONE:
			case WEATHER_CLOUDS:
			case WEATHER_SUNNY:
			case WEATHER_SHADE:
			case WEATHER_DROUGHT:
				break;
			default:
				return FALSE;
		}
	#endif

	PlayerGetDestCoords(&x, &y);
	if (MapGridGetMetatileField(x, y, 4) & TILE_FLAG_ENCOUNTER_TILE)
	{
		if (landMonsInfo == NULL)
			return FALSE;

		if (TryStartRoamerEncounter(ENCOUNTER_TYPE_LAND) == TRUE)
		{
			BattleSetup_StartRoamerBattle();
			return TRUE;
		}

		TryGenerateWildMon(landMonsInfo, WILD_AREA_LAND, 0);

		BattleSetup_StartWildBattle();
		return TRUE;
	}
	else if (MapGridGetMetatileField(x, y, 4) & TILE_FLAG_SURFABLE)
	{
		if (waterMonsInfo == NULL)
			return FALSE;

		if (TryStartRoamerEncounter(ENCOUNTER_TYPE_WATER) == TRUE)
		{
			BattleSetup_StartRoamerBattle();
			return TRUE;
		}

		TryGenerateWildMon(waterMonsInfo, WILD_AREA_WATER, 0);
		BattleSetup_StartWildBattle();
		return TRUE;
	}

	return FALSE;
}

bool8 StartRandomWildEncounter(bool8 waterMon)
{
	const struct WildPokemonInfo* landMonsInfo = LoadProperMonsData(LAND_MONS_HEADER);
	const struct WildPokemonInfo* waterMonsInfo = LoadProperMonsData(WATER_MONS_HEADER);

	if (!waterMon)
	{
		if (landMonsInfo == NULL)
			return FALSE;

		if (TryStartRoamerEncounter(ENCOUNTER_TYPE_LAND) == TRUE)
		{
			BattleSetup_StartRoamerBattle();
			return TRUE;
		}

		TryGenerateWildMon(landMonsInfo, WILD_AREA_LAND, 0);

		BattleSetup_StartWildBattle();
		return TRUE;
	}
	else
	{
		if (waterMonsInfo == NULL)
			return FALSE;

		if (TryStartRoamerEncounter(ENCOUNTER_TYPE_WATER) == TRUE)
		{
			BattleSetup_StartRoamerBattle();
			return TRUE;
		}

		TryGenerateWildMon(waterMonsInfo, WILD_AREA_WATER, 0);
		BattleSetup_StartWildBattle();
		return TRUE;
	}

	return FALSE;
}

static bool8 IsAbilityAllowingEncounter(u8 level)
{
	u8 ability;

	if (GetMonData(&gPlayerParty[0], MON_DATA_IS_EGG, NULL))
		return TRUE;

	ability = GetPartyAbility(&gPlayerParty[0]);
	if (ability == ABILITY_KEENEYE || ability == ABILITY_INTIMIDATE)
	{
		u8 playerMonLevel = gPlayerParty[0].level;
		if (playerMonLevel > 5 && level <= playerMonLevel - 5 && !umodsi(Random(), 2))
			return FALSE;
	}

	return TRUE;
}

static bool8 TryGetRandomWildMonIndexByType(const struct WildPokemon* wildMon, u8 type, u8 numMon, u8* monIndex)
{
	u8 validIndexes[numMon]; // variable length array, an interesting feature
	u8 i, validMonCount;

	for (i = 0; i < numMon; i++)
		validIndexes[i] = 0;

	for (validMonCount = 0, i = 0; i < numMon; i++)
	{
		if (gBaseStats[wildMon[i].species].type1 == type || gBaseStats[wildMon[i].species].type2 == type)
			validIndexes[validMonCount++] = i;
	}

	if (validMonCount == 0 || validMonCount == numMon)
		return FALSE;

	*monIndex = validIndexes[umodsi(Random(), validMonCount)];
	return TRUE;
}

static bool8 TryGetAbilityInfluencedWildMonIndex(const struct WildPokemon* wildMon, u8 type, u8 ability, u8* monIndex)
{
	if (GetMonData(&gPlayerParty[0], MON_DATA_IS_EGG, NULL))
		return FALSE;
	else if (GetPartyAbility(&gPlayerParty[0]) != ability)
		return FALSE;
	else if (umodsi(Random(), 2) != 0)
		return FALSE;

	return TryGetRandomWildMonIndexByType(wildMon, type, LAND_WILD_COUNT, monIndex);
}

void DoStandardWildBattle(void)
{
	ScriptContext2_Enable();
	FreezeEventObjects();
	sub_805C780();
	gMain.savedCallback = CB2_EndWildBattle;
	gBattleTypeFlags = 0;

	if (FlagGet(DOUBLE_WILD_BATTLE_FLAG))
	{
		gBattleTypeFlags |= BATTLE_TYPE_DOUBLE;

		if (FlagGet(TAG_BATTLE_FLAG))
		{
			gBattleTypeFlags |=  BATTLE_TYPE_INGAME_PARTNER;
		}
	}

	CreateBattleStartTask(GetWildBattleTransition(), GetMUS_ForBattle());
	IncrementGameStat(GAME_STAT_TOTAL_BATTLES);
	IncrementGameStat(GAME_STAT_WILD_BATTLES);
}

void sp138_StartLegendaryBattle(void)
{
	ScriptContext2_Enable();
	gMain.savedCallback = CB2_EndScriptedWildBattle_2;

	gBattleTypeFlags = BATTLE_TYPE_SCRIPTED_WILD_1 | BATTLE_TYPE_SCRIPTED_WILD_3;

	if (FlagGet(DOUBLE_WILD_BATTLE_FLAG)
	&& gEnemyParty[1].species != SPECIES_NONE
	&& ViableMonCount(gPlayerParty) > 1) //At least two alive Pokemon
		gBattleTypeFlags |= BATTLE_TYPE_DOUBLE;

	CreateBattleStartTask(0, GetMUS_ForBattle());
	IncrementGameStat(GAME_STAT_TOTAL_BATTLES);
	IncrementGameStat(GAME_STAT_WILD_BATTLES);
}

#define gText_Ghost (const u8*) 0x841D148
void sp156_StartGhostBattle(void)
{
	#ifndef NO_GHOST_BATTLES
	ScriptContext2_Enable();
	gMain.savedCallback = CB2_EndScriptedWildBattle;

	gBattleTypeFlags = BATTLE_TYPE_GHOST;
	if (CheckBagHasItem(ITEM_SILPH_SCOPE, 1))
		gBattleTypeFlags |= BATTLE_TYPE_SCRIPTED_WILD_1;

	if (Var8004 == 0 || Var8004 >= NUM_SPECIES) //Default is Marowak
		CreateMonWithGenderNatureLetter(&gEnemyParty[0], SPECIES_MAROWAK, 30, 31, MON_FEMALE, NATURE_SERIOUS, 0);
	else
		CreateWildMon(Var8004, Var8005, 0, TRUE);

	SetMonData(&gEnemyParty[0], MON_DATA_NICKNAME, gText_Ghost);
	SetMonData(&gEnemyParty[0], MON_DATA_HELD_ITEM, &Var8006);

	CreateBattleStartTask(0, GetMUS_ForBattle());
	IncrementGameStat(GAME_STAT_TOTAL_BATTLES);
	IncrementGameStat(GAME_STAT_WILD_BATTLES);
	#endif
}

//setwildbattle SPECIES LEVEL ITEM
//setwildbattle 0xFFFF 0x0 0x0 0x0 SPECIES_1 LEVEL_1 ITEM_1 0x0 SPECIES_2 LEVEL2 ITEM_2
bool8 ScrCmd_setwildbattle(struct ScriptContext* ctx)
{
	u16 species, item;
	u8 level;
	u16 specialMoves[MAX_MON_MOVES * 2];
	int i;

	for (i = 0; i < MAX_MON_MOVES * 2; ++i)
		specialMoves[i] = (&Var8000)[i];

	species = ScriptReadHalfword(ctx);
	if (species == 0xFFFF) //Wild Double
	{
		species = ScriptReadByte(ctx); //Skip null values
		species = ScriptReadHalfword(ctx); //Skip null values
		species = ScriptReadByte(ctx); //Skip null values
		FlagSet(DOUBLE_WILD_BATTLE_FLAG);

		for (i = 0; i < 2; ++i)
		{
			species = ScriptReadHalfword(ctx);
			level = ScriptReadByte(ctx);
			item = ScriptReadHalfword(ctx);
			CreateScriptedWildMon(species, level, item, specialMoves, i == 0);

			if (i == 0)
				species = ScriptReadByte(ctx); //Skip null values
		}
	}
	else
	{
		FlagClear(DOUBLE_WILD_BATTLE_FLAG); //Singular mon
		level = ScriptReadByte(ctx);
		item = ScriptReadHalfword(ctx);
		CreateScriptedWildMon(species, level, item, specialMoves, TRUE);
	}

	return FALSE;
}

static void CreateScriptedWildMon(u16 species, u8 level, u16 item, u16* moves, bool8 firstMon)
{
	u8 index = firstMon ? 0 : 1;

	if (firstMon)
		ZeroEnemyPartyMons();

	CreateMon(&gEnemyParty[index], species, level, 0x20, 0, 0, 0, 0);
	if (item)
		SetMonData(&gEnemyParty[index], MON_DATA_HELD_ITEM, &item);

	if (FlagGet(WILD_CUSTOM_MOVES_FLAG))
	{
		moves = firstMon ? moves : &moves[4];
		for (int i = 0; i < MAX_MON_MOVES; ++i)
		{
			if (moves[i] != 0xFFFF)
				gEnemyParty[index].moves[i] = moves[i];
		}
	}

	if (FlagGet(HIDDEN_ABILITY_FLAG))
		gEnemyParty[index].hiddenAbility = TRUE;
}

void TrySetWildDoubleBattleTypeScripted()
{
	if (FlagGet(DOUBLE_WILD_BATTLE_FLAG))
		gBattleTypeFlags |= BATTLE_TYPE_DOUBLE;
}

species_t GetLocalWildMon(bool8* isWaterMon)
{
	const struct WildPokemonInfo* landMonsInfo = LoadProperMonsData(LAND_MONS_HEADER);
	const struct WildPokemonInfo* waterMonsInfo = LoadProperMonsData(WATER_MONS_HEADER);
	*isWaterMon = FALSE;

	// Neither
	if (landMonsInfo == NULL && waterMonsInfo == NULL)
		return SPECIES_NONE;
	// Land Pokemon
	else if (landMonsInfo != NULL && waterMonsInfo == NULL)
		return landMonsInfo->wildPokemon[ChooseWildMonIndex_Land()].species;
	// Water Pokemon
	else if (landMonsInfo == NULL && waterMonsInfo != NULL)
	{
		*isWaterMon = TRUE;
		return waterMonsInfo->wildPokemon[ChooseWildMonIndex_WaterRock()].species;
	}

	// Either land or water Pokemon
	if ((Random() % 100) < 80)
	{
		return landMonsInfo->wildPokemon[ChooseWildMonIndex_Land()].species;
	}
	else
	{
		*isWaterMon = TRUE;
		return waterMonsInfo->wildPokemon[ChooseWildMonIndex_WaterRock()].species;
	}
}

u16 GetLocalWaterMon(void)
{
	const struct WildPokemonInfo* waterMonsInfo = LoadProperMonsData(WATER_MONS_HEADER);

	if (waterMonsInfo)
		return waterMonsInfo->wildPokemon[ChooseWildMonIndex_WaterRock()].species;

	return SPECIES_NONE;
}

const struct WildPokemonInfo* LoadProperMonsData(u8 type)
{
	const struct WildPokemonInfo* monsInfo;
	const struct WildPokemonHeader* header = GetCurrentMapWildMonHeader();

	if (header == NULL) //No data whatsoever on this map
		return NULL;

	monsInfo = LoadProperMonsPointer(header, type);

	if (monsInfo == NULL)
	{
		#ifdef TIME_ENABLED
			header = GetCurrentMapWildMonDaytimeHeader();
			monsInfo = LoadProperMonsPointer(header, type);
			if (header == NULL || monsInfo == NULL)
		#endif
				return NULL;
	}

	return monsInfo;
}

static const struct WildPokemonInfo* LoadProperMonsPointer(const struct WildPokemonHeader* header, const u8 type)
{
	switch (type) {
		case LAND_MONS_HEADER:
			return header->landMonsInfo;
		case WATER_MONS_HEADER:
			return header->waterMonsInfo;
		case FISHING_MONS_HEADER:
			return header->fishingMonsInfo;
		case ROCK_SMASH_MONS_HEADER:
			return header->rockSmashMonsInfo;
		default:
			return NULL;
	}
}

#ifdef SCALE_WILD_POKEMON_LEVELS_FLAG
static u8 GetLowestMonLevel(const struct Pokemon* const party)
{
	u8 min = party[0].level;

	for (int i = 1; i < PARTY_SIZE; ++i)
	{
		if (min == 1
		||  party[i].species == SPECIES_NONE)
			return min;

		if (GetMonData(&party[i], MON_DATA_IS_EGG, NULL))
			continue;

		if (party[i].level < min)
			min = party[i].level;
	}

	return min;
}
#endif
