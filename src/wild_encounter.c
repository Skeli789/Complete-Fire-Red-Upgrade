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
#include "../include/constants/metatile_behaviors.h"
#include "../include/constants/vars.h"

#include "../include/new/battle_start_turn_start.h"
#include "../include/new/build_pokemon.h"
#include "../include/new/daycare.h"
#include "../include/new/dns.h"
#include "../include/new/dynamax.h"
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
	u32 rngState;
	u16 prevMetatileBehavior;
	u16 encounterRateBuff;
	u8 stepsSinceLastEncounter;
	u8 abilityEffect;
	u16 leadMonHeldItem;
};

extern struct EncounterRate sWildEncounterData;

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

#ifdef FLAG_SCALE_WILD_POKEMON_LEVELS
static u8 GetLowestMonLevel(const struct Pokemon* const party);
#endif

static u8 ChooseWildMonLevel(const struct WildPokemon* wildPokemon)
{
	u8 min;
	u8 max;
	u8 range;
	u8 rand;
	u8 fluteBonus;

	#ifdef FLAG_SCALE_WILD_POKEMON_LEVELS
	if (FlagGet(FLAG_SCALE_WILD_POKEMON_LEVELS))
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

    switch (GetFluteEncounterRateModType()) {
		case 2: //Black Flute
			fluteBonus = (Random() % 3 + 1);
			max = MathMin(MAX_LEVEL, max + fluteBonus);
			min = MathMin(MAX_LEVEL, min + fluteBonus);
			break;
		case 1: //White Flute
			fluteBonus = (Random() % 3 + 1);

			if (fluteBonus < max)
				max -= fluteBonus;
			else
				max = 1;

			if (fluteBonus < min)
				min -= fluteBonus;
			else
				min = 1;
			break;
	}

	//Check ability for max level mon
	if (!GetMonData(&gPlayerParty[0], MON_DATA_IS_EGG, NULL))
	{
		u8 ability = GetMonAbility(&gPlayerParty[0]);
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
	&&  GetMonAbility(&gPlayerParty[0]) == ABILITY_SYNCHRONIZE
/*	&&  umodsi(Random(), 2) == 0*/) //Guaranteed Gen 8+
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
	&&  GetMonAbility(&gPlayerParty[0]) == ABILITY_CUTECHARM
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
	if (FlagGet(FLAG_HIDDEN_ABILITY))
		gEnemyParty[enemyMonIndex].hiddenAbility = TRUE;

	#ifdef FLAG_WILD_CUSTOM_MOVES
	//Custom moves
	if (FlagSet(FLAG_WILD_CUSTOM_MOVES))
	{
		u16* moves = (enemyMonIndex == 0) ? &Var8000 : &Var8004;
		for (int i = 0; i < MAX_MON_MOVES; ++i)
		{
			if (moves[i] != 0xFFFF)
				gEnemyParty[enemyMonIndex].moves[i] = moves[i];
		}
	}
	#endif

	//Status Inducers
	TryStatusInducer(&gEnemyParty[enemyMonIndex]);
}

void sp117_CreateRaidMon(void)
{
	if (FlagGet(FLAG_BATTLE_FACILITY)) //Battle Frontier Demo
	{
		VarSet(VAR_BATTLE_FACILITY_POKE_LEVEL, gRaidBattleLevel);
		CreateFrontierRaidMon(gRaidBattleSpecies);
		return;
	}

	u32 i, numEggMoves;
	u16 eggMoveBuffer[EGG_MOVES_ARRAY_COUNT];

	u8 eggMoveChance = GetRaidEggMoveChance();
	u8 abilityNum = GetRaidSpeciesAbilityNum(gRaidBattleSpecies);
	struct Pokemon* mon = &gEnemyParty[0];

	if (abilityNum == RAID_ABILITY_HIDDEN
	|| (abilityNum == RAID_ABILITY_RANDOM_ALL && Random() & 1)) //50% chance of hidden ability
		FlagSet(FLAG_HIDDEN_ABILITY);
	else
		FlagClear(FLAG_HIDDEN_ABILITY);

	#ifdef FLAG_WILD_CUSTOM_MOVES
	FlagClear(FLAG_WILD_CUSTOM_MOVES); //We'll set custom moves later
	#endif

	CreateWildMon(gRaidBattleSpecies, gRaidBattleLevel, 0, TRUE);

	if (abilityNum == RAID_ABILITY_1 || abilityNum == RAID_ABILITY_2)
		GiveMonNatureAndAbility(mon, GetNature(mon), abilityNum - RAID_ABILITY_1, IsMonShiny(mon));

	numEggMoves = GetAllEggMoves(&gEnemyParty[0], eggMoveBuffer, TRUE);
	for (i = 0; i < MAX_MON_MOVES; ++i)
	{
		if (numEggMoves != 0 && Random() % 100 < eggMoveChance)
		{
			u16 eggMove = eggMoveBuffer[RandRange(0, numEggMoves)];

			if (MoveInMonMoveset(eggMove, mon)) //Try to reroll once if mon already knows move
				eggMove = eggMoveBuffer[RandRange(0, numEggMoves)];

			if (!MoveInMonMoveset(eggMove, mon) && GiveMoveToBoxMon((struct BoxPokemon*) mon, eggMove) == 0xFFFF)
				DeleteFirstMoveAndGiveMoveToBoxMon((struct BoxPokemon*) mon, eggMove);
		}
	}

	//Give perfect IVs based on the number of Raid stars
	u8 numPerfectStats = 0;
	u8 perfect = 31;
	bool8 perfectStats[NUM_STATS] = {0};

	while (numPerfectStats < MathMin(gRaidBattleStars, NUM_STATS)) //Error prevention
	{
		u8 statId = Random() % NUM_STATS;
		if (!perfectStats[statId]) //Must be unique
		{
			perfectStats[statId] = TRUE;
			++numPerfectStats;
			SetMonData(&gPlayerParty[0], MON_DATA_HP_IV + statId, &perfect);
		}
	}
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

static void ClearDailyEventFlags(void)
{
	#ifdef FLAG_DAILY_EVENTS_START
	for (u32 i = FLAG_DAILY_EVENTS_START; i < FLAG_DAILY_EVENTS_START + 0x100; ++i)
		FlagClear(i);
	#endif
}

void TryUpdateSwarm(void)
{
	u32 backupVar = VarGet(VAR_SWARM_DAILY_EVENT) | (VarGet(VAR_SWARM_DAILY_EVENT + 1) << 16);

	if (CheckAndSetDailyEvent(VAR_SWARM_DAILY_EVENT, TRUE))
	{
		u16 index = Random() % gSwarmTableLength;
		VarSet(VAR_SWARM_INDEX, index);

		u32 daysSince = GetDaysSinceTimeInValue(backupVar);
		UpdatePartyPokerusTime(daysSince);
		ClearDailyEventFlags();
		SetGameStat(GAME_STAT_CAUGHT_TODAY, 0);
		SetGameStat(GAME_STAT_EXP_EARNED_TODAY, 0);
	}
}

static bool8 TryGenerateSwarmMon(u8 level, u8 wildMonIndex, bool8 purgeParty)
{
	if (gSwarmTableLength == 0)
		return FALSE;

	u8 index = VarGet(VAR_SWARM_INDEX);
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
	if (TryGetAbilityInfluencedWildMonIndex(wildMonInfo->wildPokemon, TYPE_ELECTRIC, ABILITY_LIGHTNINGROD, &wildMonIndex))
		goto SKIP_INDEX_SEARCH;
	if (TryGetAbilityInfluencedWildMonIndex(wildMonInfo->wildPokemon, TYPE_FIRE, ABILITY_FLASHFIRE, &wildMonIndex))
		goto SKIP_INDEX_SEARCH;
	if (TryGetAbilityInfluencedWildMonIndex(wildMonInfo->wildPokemon, TYPE_GRASS, ABILITY_HARVEST, &wildMonIndex))
		goto SKIP_INDEX_SEARCH;
	if (TryGetAbilityInfluencedWildMonIndex(wildMonInfo->wildPokemon, TYPE_WATER, ABILITY_STORMDRAIN, &wildMonIndex))
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

	#ifdef FLAG_DOUBLE_WILD_BATTLE
	if (FlagGet(FLAG_DOUBLE_WILD_BATTLE))
	{
		wildMonIndex = 0;

		if (TryGetAbilityInfluencedWildMonIndex(wildMonInfo->wildPokemon, TYPE_STEEL, ABILITY_MAGNETPULL, &wildMonIndex))
			goto SKIP_INDEX_SEARCH_2;
		if (TryGetAbilityInfluencedWildMonIndex(wildMonInfo->wildPokemon, TYPE_ELECTRIC, ABILITY_STATIC, &wildMonIndex))
			goto SKIP_INDEX_SEARCH_2;
		if (TryGetAbilityInfluencedWildMonIndex(wildMonInfo->wildPokemon, TYPE_ELECTRIC, ABILITY_LIGHTNINGROD, &wildMonIndex))
			goto SKIP_INDEX_SEARCH_2;
		if (TryGetAbilityInfluencedWildMonIndex(wildMonInfo->wildPokemon, TYPE_FIRE, ABILITY_FLASHFIRE, &wildMonIndex))
			goto SKIP_INDEX_SEARCH_2;
		if (TryGetAbilityInfluencedWildMonIndex(wildMonInfo->wildPokemon, TYPE_GRASS, ABILITY_HARVEST, &wildMonIndex))
			goto SKIP_INDEX_SEARCH_2;
		if (TryGetAbilityInfluencedWildMonIndex(wildMonInfo->wildPokemon, TYPE_WATER, ABILITY_STORMDRAIN, &wildMonIndex))
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
	#endif

	return TRUE;
}

static species_t GenerateFishingWildMon(const struct WildPokemonInfo* wildMonInfo, u8 rod)
{
	u8 wildMonIndex = ChooseWildMonIndex_Fishing(rod);
	u8 level = ChooseWildMonLevel(&wildMonInfo->wildPokemon[wildMonIndex]);

	CreateWildMon(wildMonInfo->wildPokemon[wildMonIndex].species, level, wildMonIndex, TRUE);

	#ifdef FLAG_DOUBLE_WILD_BATTLE
	if (FlagGet(FLAG_DOUBLE_WILD_BATTLE))
	{
		u8 wildMonIndex = ChooseWildMonIndex_Fishing(rod);
		u8 level = ChooseWildMonLevel(&wildMonInfo->wildPokemon[wildMonIndex]);
		CreateWildMon(wildMonInfo->wildPokemon[wildMonIndex].species, level, wildMonIndex, FALSE);
	}
	#endif

	return wildMonInfo->wildPokemon[wildMonIndex].species;
}

void FishingWildEncounter(u8 rod)
{
	const struct WildPokemonInfo* fishingMonsInfo = LoadProperMonsData(FISHING_MONS_HEADER); //Existence check done prior

	gFishingByte = TRUE;
	GenerateFishingWildMon(fishingMonsInfo, rod);

	u16 currSpecies = GetMonData(&gEnemyParty[0], MON_DATA_SPECIES, NULL);
	if (currSpecies == gLastFishingSpecies)
		gFishingStreak = MathMin(gFishingStreak + 1, 0xFF); //Increment chain
	else
		gFishingStreak = 0; //Reset chain

	gLastFishingSpecies = currSpecies;
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

bool8 DoesFishBite(void)
{
	if (DoesCurrentMapHaveFishingMons())
	{
		u8 chance = 50; //Default 50% chance of biting

		if (!GetMonData(&gPlayerParty[0], MON_DATA_IS_EGG, NULL))
		{
			u8 ability = GetMonAbility(&gPlayerParty[0]);
			if (ability == ABILITY_SUCTIONCUPS || ability  == ABILITY_STICKYHOLD)
				chance = 85; //85% chance with abilities
		}

		if (Random() % 100 < chance)
			return TRUE;
	}

	gFishingStreak = 0; //End fishing chain
	return FALSE;
}

static bool8 DoWildEncounterRateTest(u32 encounterRate, bool8 ignoreAbility)
{
	encounterRate *= 16;
	if (TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_BIKE))
		encounterRate = encounterRate * 80 / 100;

	encounterRate += sWildEncounterData.encounterRateBuff * 16 / 200;
	//ApplyFluteEncounterRateMod(&encounterRate);
	ApplyCleanseTagEncounterRateMod(&encounterRate);

	if (!ignoreAbility)
	{
        switch (sWildEncounterData.abilityEffect) {
			case 1:
				encounterRate /= 2;
				break;
			case 2:
				encounterRate *= 2;
				break;
        }
	}

	if (encounterRate > 1600)
		encounterRate = 1600;

	return DoWildEncounterRateDiceRoll(encounterRate);
}

static bool8 DoWildEncounterRateDiceRoll(u16 encounterRate)
{
	if (Random() % 1600 < encounterRate)
		return TRUE;
	else
		return FALSE;
}

u8 GetAbilityEncounterRateModType(void)
{
    sWildEncounterData.abilityEffect = 0;

    if (!GetMonData(&gPlayerParty[0], MON_DATA_IS_EGG, NULL))
    {
        u8 ability = GetMonAbility(&gPlayerParty[0]);

		switch (ability) {
			case ABILITY_WHITESMOKE:
			case ABILITY_STENCH:
			case ABILITY_QUICKFEET:
			case ABILITY_INFILTRATOR:
				sWildEncounterData.abilityEffect = 1;
				break;
			case ABILITY_ARENATRAP:
			case ABILITY_ILLUMINATE:
			case ABILITY_NOGUARD:
			case ABILITY_SWARM:
				sWildEncounterData.abilityEffect = 2;
				break;
			case ABILITY_SANDVEIL:
				if (GetCurrentWeather() == WEATHER_SANDSTORM)
					sWildEncounterData.abilityEffect = 1;
				break;
			case ABILITY_SNOWCLOAK:
				if (GetCurrentWeather() == WEATHER_STEADY_SNOW)
					sWildEncounterData.abilityEffect = 1;
				break;
		}
    }

    return sWildEncounterData.abilityEffect;
}

bool8 StandardWildEncounter(const u32 currMetaTileBehavior, const u16 previousMetaTileBehavior)
{
	struct Roamer* roamer;
	bool8 clearDoubleFlag = FALSE;
	const u8 lowerByte = GetMetatileAttributeFromRawMetatileBehavior(currMetaTileBehavior, METATILE_ATTRIBUTE_ENCOUNTER_TYPE);

	if (sWildEncountersDisabled == TRUE)
		return FALSE;

	const struct WildPokemonInfo* landMonsInfo = LoadProperMonsData(LAND_MONS_HEADER);
	const struct WildPokemonInfo* waterMonsInfo = LoadProperMonsData(WATER_MONS_HEADER);

	#ifdef FLAG_NO_RANDOM_WILD_ENCOUNTERS
	if (FlagGet(FLAG_NO_RANDOM_WILD_ENCOUNTERS))
		return FALSE;
	else
	#endif
	{
		if (lowerByte & TILE_FLAG_ENCOUNTER_TILE)
		{
			if (landMonsInfo == NULL)
				return FALSE;
			else if (GetMetatileAttributeFromRawMetatileBehavior(currMetaTileBehavior, METATILE_ATTRIBUTE_BEHAVIOR) != previousMetaTileBehavior
			&& !DoGlobalWildEncounterDiceRoll())
				return FALSE;
			else if (DoWildEncounterRateTest(landMonsInfo->encounterRate, FALSE) != TRUE)
			{
				AddToWildEncounterRateBuff(landMonsInfo->encounterRate);
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
				#ifdef FLAG_DOUBLE_WILD_BATTLE
				if (!FlagGet(FLAG_DOUBLE_WILD_BATTLE) //Flag hasn't already been set by user
				&&  ViableMonCount(gPlayerParty) >= 2
				&&  (lowerByte & TILE_FLAG_WILD_DOUBLE)
				&&  Random() % 100 < WILD_DOUBLE_RANDOM_CHANCE)
				{
					FlagSet(FLAG_DOUBLE_WILD_BATTLE);
					clearDoubleFlag = TRUE;
				}
				#endif

				if (TryGenerateWildMon(landMonsInfo, WILD_AREA_LAND, WILD_CHECK_REPEL | WILD_CHECK_KEEN_EYE) == TRUE)
				{
					BattleSetup_StartWildBattle();
					return TRUE;
				}

				#ifdef FLAG_DOUBLE_WILD_BATTLE
				if (clearDoubleFlag)
					FlagClear(FLAG_DOUBLE_WILD_BATTLE); //Battle didn't start so restart the flag
				#endif

				AddToWildEncounterRateBuff(landMonsInfo->encounterRate);
			}
		}
		else if (lowerByte & TILE_FLAG_SURFABLE
			 &&  (TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_SURFING) || (currMetaTileBehavior & 0xFF) == MB_CAVE)) //For alternate grass data on route
		{
			if (waterMonsInfo == NULL)
			   return FALSE;
			else if (previousMetaTileBehavior != GetMetatileAttributeFromRawMetatileBehavior(currMetaTileBehavior, METATILE_ATTRIBUTE_BEHAVIOR)
			&& !DoGlobalWildEncounterDiceRoll())
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
				#ifdef FLAG_DOUBLE_WILD_BATTLE
				if (!FlagGet(FLAG_DOUBLE_WILD_BATTLE) //Flag hasn't already been set by user
				&&  ViableMonCount(gPlayerParty) >= 2
				&&  lowerByte & TILE_FLAG_WILD_DOUBLE
				&&  Random() % 100 < WILD_DOUBLE_RANDOM_CHANCE)
				{
					FlagSet(FLAG_DOUBLE_WILD_BATTLE);
					clearDoubleFlag = TRUE;
				}
				#endif

				if (TryGenerateWildMon(waterMonsInfo, WILD_AREA_WATER, WILD_CHECK_REPEL | WILD_CHECK_KEEN_EYE) == TRUE)
				{
					BattleSetup_StartWildBattle();
					return TRUE;
				}

				#ifdef FLAG_DOUBLE_WILD_BATTLE
				if (clearDoubleFlag)
					FlagClear(FLAG_DOUBLE_WILD_BATTLE); //Battle didn't start so restart the flag
				#endif

				return FALSE;
			}
		}
	}

	return FALSE;
}

bool8 TryStandardWildEncounter(u32 currMetatileBehavior)
{
	if (!HandleWildEncounterCooldown(currMetatileBehavior))
    {
        sWildEncounterData.prevMetatileBehavior = GetMetatileAttributeFromRawMetatileBehavior(currMetatileBehavior, METATILE_ATTRIBUTE_BEHAVIOR);
        return FALSE;
    }
	else if (StandardWildEncounter(currMetatileBehavior, sWildEncounterData.prevMetatileBehavior) == TRUE)
	{
		sWildEncounterData.encounterRateBuff = 0;
		sWildEncounterData.stepsSinceLastEncounter = 0;
		sWildEncounterData.prevMetatileBehavior = GetMetatileAttributeFromRawMetatileBehavior(currMetatileBehavior, METATILE_ATTRIBUTE_BEHAVIOR);
		return TRUE;
	}
	else
	{
		sWildEncounterData.prevMetatileBehavior = GetMetatileAttributeFromRawMetatileBehavior(currMetatileBehavior, METATILE_ATTRIBUTE_BEHAVIOR);
		return FALSE;
	}
}

u8 GetMapBaseEncounterCooldown(u8 lowerByte)
{
    const struct WildPokemonInfo* landMonsInfo = LoadProperMonsData(LAND_MONS_HEADER);
	const struct WildPokemonInfo* waterMonsInfo = LoadProperMonsData(WATER_MONS_HEADER);

    if (lowerByte & TILE_FLAG_ENCOUNTER_TILE)
    {
        if (landMonsInfo == NULL) //No land data on this map
            return 0xFF;
        if (landMonsInfo->encounterRate >= 80)
            return 0;
        if (landMonsInfo->encounterRate < 10)
            return 8;

        return 8 - (landMonsInfo->encounterRate / 10);
    }

    if (lowerByte & TILE_FLAG_SURFABLE)
    {
        if (waterMonsInfo == NULL) //No water data on this map
            return 0xFF;
        if (waterMonsInfo->encounterRate >= 80)
            return 0;
        if (waterMonsInfo->encounterRate < 10)
            return 8;

        return 8 - (waterMonsInfo->encounterRate / 10);
    }

    return 0xFF;
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

	ability = GetMonAbility(&gPlayerParty[0]);
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
	else if (GetMonAbility(&gPlayerParty[0]) != ability)
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

	#ifdef FLAG_DOUBLE_WILD_BATTLE
	if (FlagGet(FLAG_DOUBLE_WILD_BATTLE))
	{
		gBattleTypeFlags |= BATTLE_TYPE_DOUBLE;

		if (FlagGet(FLAG_TAG_BATTLE))
			gBattleTypeFlags |=  BATTLE_TYPE_INGAME_PARTNER;
	}
	#endif

	CreateBattleStartTask(GetWildBattleTransition(), GetMUS_ForBattle());
	IncrementGameStat(GAME_STAT_TOTAL_BATTLES);
	IncrementGameStat(GAME_STAT_WILD_BATTLES);
}

void sp138_StartLegendaryBattle(void)
{
	ScriptContext2_Enable();
	gMain.savedCallback = CB2_EndScriptedWildBattle_2;

	gBattleTypeFlags = BATTLE_TYPE_SCRIPTED_WILD_1 | BATTLE_TYPE_SCRIPTED_WILD_3;

	#ifdef FLAG_DOUBLE_WILD_BATTLE
	if (FlagGet(FLAG_DOUBLE_WILD_BATTLE)
	&& gEnemyParty[1].species != SPECIES_NONE
	&& ViableMonCount(gPlayerParty) > 1) //At least two alive Pokemon
	{
		gBattleTypeFlags |= BATTLE_TYPE_DOUBLE;

		if (FlagGet(FLAG_TAG_BATTLE))
			gBattleTypeFlags |=  BATTLE_TYPE_INGAME_PARTNER;
	}
	#endif

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

void sp118_StartRaidBattle(void)
{
	HealPlayerParty();

	ScriptContext2_Enable();
	gMain.savedCallback = CB2_EndScriptedWildBattle_2;

	gBattleTypeFlags = BATTLE_TYPE_SCRIPTED_WILD_1 | BATTLE_TYPE_SCRIPTED_WILD_3 | BATTLE_TYPE_DYNAMAX;

	#ifdef FLAG_RAID_BATTLE
	FlagSet(FLAG_RAID_BATTLE);
	#endif

	if (FlagGet(FLAG_TAG_BATTLE))
		gBattleTypeFlags |= (BATTLE_TYPE_DOUBLE | BATTLE_TYPE_INGAME_PARTNER);

	if (FlagGet(FLAG_BATTLE_FACILITY))
	{
		VarSet(VAR_BATTLE_FACILITY_BATTLE_TYPE, 0); //So battle type doesn't interfere with anything
		VarSet(VAR_BATTLE_FACILITY_TIER, 0); //So tier doesn't interfere with anything
	}

	CreateBattleStartTask(0, GetMUS_ForBattle());
	IncrementGameStat(GAME_STAT_TOTAL_BATTLES);
	IncrementGameStat(GAME_STAT_WILD_BATTLES);
	IncrementGameStat(GAME_STAT_RAID_BATTLES);
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
	#ifdef FLAG_DOUBLE_WILD_BATTLE
	if (species == 0xFFFF) //Wild Double
	{
		species = ScriptReadByte(ctx); //Skip null values
		species = ScriptReadHalfword(ctx); //Skip null values
		species = ScriptReadByte(ctx); //Skip null values
		FlagSet(FLAG_DOUBLE_WILD_BATTLE);

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
	#endif
	{
		#ifdef FLAG_DOUBLE_WILD_BATTLE
		FlagClear(FLAG_DOUBLE_WILD_BATTLE); //Singular mon
		#endif
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

	#ifdef FLAG_WILD_CUSTOM_MOVES
	if (FlagGet(FLAG_WILD_CUSTOM_MOVES))
	{
		moves = firstMon ? moves : &moves[4];
		for (int i = 0; i < MAX_MON_MOVES; ++i)
		{
			if (moves[i] != 0xFFFF)
				gEnemyParty[index].moves[i] = moves[i];
		}
	}
	#endif

	if (FlagGet(FLAG_HIDDEN_ABILITY))
		gEnemyParty[index].hiddenAbility = TRUE;
}

void TrySetWildDoubleBattleTypeScripted()
{
	#ifdef FLAG_DOUBLE_WILD_BATTLE
	if (FlagGet(FLAG_DOUBLE_WILD_BATTLE))
		gBattleTypeFlags |= BATTLE_TYPE_DOUBLE;
	#endif
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

#ifdef FLAG_SCALE_WILD_POKEMON_LEVELS
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
