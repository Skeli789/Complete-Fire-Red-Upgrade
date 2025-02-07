#include "defines.h"
#include "defines_battle.h"
#include "../include/battle_transition.h"
#include "../include/event_object_movement.h"
#include "../include/fieldmap.h"
#include "../include/field_player_avatar.h"
#include "../include/field_weather.h"
#include "../include/script.h"
#include "../include/rtc.h"
#include "../include/text.h"
#include "../include/wild_encounter.h"
#include "../include/random.h"

#include "../include/constants/flags.h"
#include "../include/constants/items.h"
#include "../include/constants/maps.h"
#include "../include/constants/metatile_behaviors.h"
#include "../include/constants/region_map_sections.h"
#include "../include/constants/vars.h"

#include "../include/new/ability_util.h"
#include "../include/new/battle_start_turn_start.h"
#include "../include/new/battle_transition.h"
#include "../include/new/build_pokemon.h"
#include "../include/new/catching.h"
#include "../include/new/daycare.h"
#include "../include/new/dns.h"
#include "../include/new/dynamax.h"
#include "../include/new/overworld.h"
#include "../include/new/roamer.h"
#include "../include/new/util.h"
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

extern const struct WildPokemonHeader* sSavedWildDataDaytimeHeader;
extern u8 sSavedWildDataMapGroup;
extern u8 sSavedWildDataMapNum;

#ifdef IgnoreWildPokemon
extern const u8 SystemScript_PokemonEncounter[];
#endif

extern struct EncounterRate sWildEncounterData;

extern u8 sUnownLetterSlots[NUM_TANOBY_CHAMBERS][12]; //[NUM_ROOMS][NUM_WILD_INDEXES]
extern const struct WildPokemonHeader gWildMonMorningHeaders[];
extern const struct WildPokemonHeader gWildMonEveningHeaders[];
extern const struct WildPokemonHeader gWildMonNightHeaders[];
extern const u8 gSwarmOrders[31][24];
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
static bool8 TryGetAbilityInfluencedWildMonIndex(const struct WildPokemon* wildMon, u8 type, u8 ability, u8* monIndex, u8 monsCount);
static void CreateScriptedWildMon(u16 species, u8 level, u16 item, u16* specialMoves, bool8 firstMon);
static const struct WildPokemonInfo* LoadProperMonsPointer(const struct WildPokemonHeader* header, const u8 type);
static void StartRoamerBattle(void);

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

	//#ifdef FLAG_SCALE_WILD_POKEMON_LEVELS
	//if (FlagGet(FLAG_SCALE_WILD_POKEMON_LEVELS))
	if(VarGet(VAR_WILD_LEVEL_SCALING) == 0)
	{
		min = max = GetLowestMonLevel(gPlayerParty);

		#ifdef FLAG_HARD_LEVEL_CAP
		u8 levelCap;
		if (FlagGet(FLAG_HARD_LEVEL_CAP) && max >= (levelCap = GetCurrentLevelCap()))
			min = max = levelCap;
		#endif
	}
	else
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
	if(VarGet(VAR_WILD_LEVEL_SCALING) == 0)
	{
		u16 level = 0;
		for (int i = 0; i < gPlayerPartyCount; i++) {
			level += gPlayerParty[i].level;
		}
		level /= gPlayerPartyCount;
		if(min <= level && max <= level) {
			min = level - 6;
			max = level - 7;
		}
		if (min < 1 || min > 100) {
			min = 2;
		}
		if (max < 1 || max > 100) {
			max = 3;
		}
	}

	#ifdef FLAG_HARD_LEVEL_CAP
	extern u8 GetCurrentLevelCap(void); //Must be implemented yourself
	if (FlagGet(FLAG_HARD_LEVEL_CAP))
	{
		u8 levelCap = GetCurrentLevelCap();
		if (max > levelCap)
			max = levelCap; //Prevent wild Pokemon above the level cap from appearing
	}
	#endif

	if (min > max)
		max = min;

	//Check ability for max level mon
	if (!GetMonData(&gPlayerParty[0], MON_DATA_IS_EGG, NULL))
	{
		u8 ability = GetMonAbility(&gPlayerParty[0]);

		#ifndef ABILITY_VITALSPIRIT
		if (IsVitalSpiritAbility(ability, GetMonData(&gPlayerParty[0], MON_DATA_SPECIES, NULL)))
			ability = ABILITY_PRESSURE;
		#endif

		if (ability == ABILITY_HUSTLE
		#ifdef ABILITY_VITALSPIRIT
		|| ability == ABILITY_VITALSPIRIT
		#endif
		|| ability == ABILITY_PRESSURE)
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

	if (gSaveBlock1->location.mapGroup == sSavedWildDataMapGroup
	&&  gSaveBlock1->location.mapNum == sSavedWildDataMapNum)
	{
		return sSavedWildDataDaytimeHeader;
	}
	else //Cache data for faster data access
	{
		sSavedWildDataMapGroup = gSaveBlock1->location.mapGroup;
		sSavedWildDataMapNum = gSaveBlock1->location.mapNum;
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
			/* A function that returns true if the Tanoby Key flag has been set.
			   If it hasn't, and you're in the ruins, then return false to indicate
			   no Pokemon can be found. */
			if (!CanEncounterUnownInTanobyRuins())
				break; //No Pokemon here					 
			#elif (defined UNBOUND) //For Pokemon Unbound
			if (GetCurrentRegionMapSectionId() == MAPSEC_TOMB_OF_BORRIUS && !FlagGet(FLAG_OPENED_TOMB_OF_BORRIUS_HOLE))
				break; //No Unown yet
			#endif

			sSavedWildDataDaytimeHeader = &gWildMonHeaders[i]; //Cache data for faster data access
			return &gWildMonHeaders[i];
		}
	}

	sSavedWildDataDaytimeHeader = NULL; //No data on this map
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
	&& (Random() % 3) > 0) //2/3 of the time
	{
		u16 leadingMonSpecies = gPlayerParty[0].species;
		u32 leadingMonPersonality = gPlayerParty[0].personality;
		u8 gender = GetGenderFromSpeciesAndPersonality(leadingMonSpecies, leadingMonPersonality);

		if (gender == MON_FEMALE)
			gender = MON_MALE;
		else if (gender == MON_MALE)
			gender = MON_FEMALE;
		else //Genderless
			goto REGULAR_NATURE_CREATION;

		CreateMonWithGenderNatureLetter(&gEnemyParty[enemyMonIndex], species, level, 32, gender, PickWildMonNature(), PickUnownLetter(species, monHeaderIndex));
	}
	else
	{
		REGULAR_NATURE_CREATION:
		CreateMonWithNatureLetter(&gEnemyParty[enemyMonIndex], species, level, 32, PickWildMonNature(), PickUnownLetter(species, monHeaderIndex));
	}

	#ifdef FLAG_HIDDEN_ABILITY
	if (FlagGet(FLAG_HIDDEN_ABILITY))
		gEnemyParty[enemyMonIndex].hiddenAbility = TRUE;
	#endif

	#ifdef FLAG_GIGANTAMAXABLE
	if (FlagGet(FLAG_GIGANTAMAXABLE))
		gEnemyParty[enemyMonIndex].gigantamax = TRUE;
	#endif

	#ifdef FLAG_WILD_CUSTOM_MOVES
	//Custom moves
	if (FlagGet(FLAG_WILD_CUSTOM_MOVES)
	#ifdef FLAG_POKEMON_RANDOMIZER
	&& (!FlagGet(FLAG_POKEMON_RANDOMIZER) //When species are changed, the custom moves no longer make sense
	#ifdef FLAG_TEMP_DISABLE_RANDOMIZER
	|| FlagGet(FLAG_TEMP_DISABLE_RANDOMIZER)
	#endif
	)
	#endif
	)
	{
		u16* moves = (enemyMonIndex == 0) ? &Var8000 : &Var8004;
		for (int i = 0; i < MAX_MON_MOVES; ++i)
		{
			if (moves[i] != 0xFFFF)
				gEnemyParty[enemyMonIndex].moves[i] = moves[i];
		}
	}
	#endif

	//Prevent Uncatchable Shinies (they're mean)
	if (CantCatchBecauseFlag())
	{
		while (IsMonShiny(&gEnemyParty[enemyMonIndex]))
		{
			u32 otId = Random(); //Randomize otId because it's not like the player can catch it anyway
			SetMonData(&gEnemyParty[enemyMonIndex], MON_DATA_OT_ID, &otId);
		}
	}

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

	u32 numEggMoves;
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
		GiveMonNatureAndAbility(mon, GetNature(mon), abilityNum - RAID_ABILITY_1, IsMonShiny(mon), FALSE, FALSE);

	//Try to give a random Egg move
	numEggMoves = GetAllEggMoves(mon, eggMoveBuffer, TRUE);
	if (numEggMoves != 0 && Random() % 100 < eggMoveChance)
	{
		u16 eggMove = eggMoveBuffer[RandRange(0, numEggMoves)];

		if (MoveInMonMoveset(eggMove, mon)) //Try to reroll once if mon already knows move
			eggMove = eggMoveBuffer[RandRange(0, numEggMoves)];

		if (!MoveInMonMoveset(eggMove, mon)
		&& GiveMoveToBoxMon((struct BoxPokemon*) mon, eggMove) == 0xFFFF)
			DeleteFirstMoveAndGiveMoveToBoxMon((struct BoxPokemon*) mon, eggMove);
	}

	//Give perfect IVs based on the number of Raid stars
	GiveMonXPerfectIVs(mon, MathMin(gRaidBattleStars - 1, NUM_STATS)); //Max 5 perfect IVs for 6-star raid
}

#ifdef UNBOUND
#define WILD_UNOWN(letter) (CHAR_##letter - CHAR_A)
static const u8 sUnboundUnownLetterSlots[][12] =
{
	{WILD_UNOWN(A), WILD_UNOWN(B), WILD_UNOWN(C), WILD_UNOWN(C), WILD_UNOWN(D), WILD_UNOWN(D), WILD_UNOWN(E), WILD_UNOWN(E), WILD_UNOWN(E), WILD_UNOWN(E), WILD_UNOWN(E), WILD_UNOWN(E)},
	{WILD_UNOWN(F), WILD_UNOWN(G), WILD_UNOWN(H), WILD_UNOWN(H), WILD_UNOWN(I), WILD_UNOWN(I), WILD_UNOWN(J), WILD_UNOWN(J), WILD_UNOWN(J), WILD_UNOWN(J), WILD_UNOWN(J), WILD_UNOWN(J)},
	{WILD_UNOWN(K), WILD_UNOWN(L), WILD_UNOWN(M), WILD_UNOWN(M), WILD_UNOWN(N), WILD_UNOWN(N), WILD_UNOWN(O), WILD_UNOWN(O), WILD_UNOWN(O), WILD_UNOWN(O), WILD_UNOWN(O), WILD_UNOWN(O)},
	{}, //Alternate map footer
	{}, //Alternate map footer
	{WILD_UNOWN(P), WILD_UNOWN(Q), WILD_UNOWN(R), WILD_UNOWN(R), WILD_UNOWN(S), WILD_UNOWN(S), WILD_UNOWN(T), WILD_UNOWN(T), WILD_UNOWN(T), WILD_UNOWN(T), WILD_UNOWN(T), WILD_UNOWN(T)},
	{WILD_UNOWN(U), WILD_UNOWN(V), WILD_UNOWN(W), WILD_UNOWN(W), WILD_UNOWN(X), WILD_UNOWN(X), WILD_UNOWN(Y), WILD_UNOWN(Y), WILD_UNOWN(Y), WILD_UNOWN(Y), WILD_UNOWN(Y), WILD_UNOWN(T)},
	{WILD_UNOWN(Z), WILD_UNOWN(Z), WILD_UNOWN(Z), WILD_UNOWN(Z), WILD_UNOWN(Z), WILD_UNOWN(Z), WILD_UNOWN(Z), WILD_UNOWN(Z), WILD_UNOWN(Z), WILD_UNOWN(Z), 26, 27}, //Last two are ! and ?
};
#endif

u8 PickUnownLetter(unusedArg u16 species, unusedArg u8 headerIndex)
{
	#ifdef TANOBY_RUINS_ENABLED
		if (species == SPECIES_UNOWN)
		{
			u8 room = gSaveBlock1->location.mapNum - 0x1B;

			if (room >= NUM_TANOBY_CHAMBERS)
				return 0;
			else
				return sUnownLetterSlots[room][headerIndex] + 1;
		}
	#elif (defined UNBOUND)
		if (species == SPECIES_UNOWN)
		{
			u8 room = gSaveBlock1->location.mapNum - MAP_NUM(TOMB_OF_BORRIUS_1F);

			if (room >= NELEMS(sUnboundUnownLetterSlots))
				return 0;
			else
				return sUnboundUnownLetterSlots[room][headerIndex] + 1;
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

#ifdef TIME_ENABLED
static void ClearDailyEventFlags(void)
{
	#ifdef FLAG_DAILY_EVENTS_START
	for (u32 i = FLAG_DAILY_EVENTS_START; i < FLAG_DAILY_EVENTS_START + 0x100; ++i)
		FlagClear(i);
	#endif

	#ifdef UNBOUND
	ClearAllRaidBattleFlags();
	#endif
}
#endif

#ifdef TIME_ENABLED
static void Task_UpdateDailyValues(u8 taskId)
{
	if (--gTasks[taskId].data[0] == 0) //Decrement the timer
	{
		DirectClockUpdate();
		if (gClock.day == gTasks[taskId].data[1]
		&& gClock.month == gTasks[taskId].data[2]
		&& gClock.year == gTasks[taskId].data[3]) //The date change was true and the RTC didn't just glitch out momentarily
		{
			u32 backupVar = VarGet(VAR_SWARM_DAILY_EVENT) | (VarGet(VAR_SWARM_DAILY_EVENT + 1) << 16);

			CheckAndSetDailyEvent(VAR_SWARM_DAILY_EVENT, TRUE); //Update the value in the var

			#ifdef SWARM_CHANGE_HOURLY
			VarSet(VAR_SWARM_INDEX, 0xFFFF); //Reset override daily
			#else
			u16 index = Random() % gSwarmTableLength;
			VarSet(VAR_SWARM_INDEX, index);
			#endif

			#ifdef VAR_RAID_PARTNER_RANDOM_NUM
			VarSet(VAR_RAID_PARTNER_RANDOM_NUM, Random()); //Changes daily to help vary the partners
			#endif

			u32 daysSince = GetDaysSinceTimeInValue(backupVar);
			UpdatePartyPokerusTime(daysSince);
			ClearDailyEventFlags();
			SetGameStat(GAME_STAT_CAUGHT_TODAY, 0);
			SetGameStat(GAME_STAT_EXP_EARNED_TODAY, 0);
		}

		DestroyTask(taskId);
	}
}
#endif

void TryUpdateSwarm(void)
{
	#ifdef TIME_ENABLED //Otherwise causes lags
	if (CheckAndSetDailyEvent(VAR_SWARM_DAILY_EVENT, FALSE) //Just check if different, don't update yet
	&& !FuncIsActiveTask(Task_UpdateDailyValues))
	{
		u8 taskId = CreateTask(Task_UpdateDailyValues, 0xFF);
		if (taskId != 0xFF)
		{
			//Save the current time in the task.
			//If the date when 0.25 seconds is up is different, destroy the task and try again.
			//This helps alleviate the issue of momentarily flash cart glitches where the date
			//is changed forward in time.
			DirectClockUpdate();
			gTasks[taskId].data[0] = 0x10; //Timer - 0.25 Seconds
			gTasks[taskId].data[1] = gClock.day;
			gTasks[taskId].data[2] = gClock.month;
			gTasks[taskId].data[3] = gClock.year;
		}
	}
	#endif
}

u8 GetCurrentSwarmIndex(void)
{
	if (gSwarmTableLength == 0)
		return 0xFF;

	if (gMapHeader.mapType == MAP_TYPE_UNDERWATER) //No swarms underwater
		return 0xFF;

	#ifdef SWARM_CHANGE_HOURLY
	u8 index;

	if (VarGet(VAR_SWARM_INDEX) < gSwarmTableLength)
	{
		index = VarGet(VAR_SWARM_INDEX); //Override
	}
	else if (gSwarmTableLength == 24) //24 different species: 1 for each hour
	{
		index = gSwarmOrders[gClock.day - 1][gClock.hour];
	}
	else
	{
		u8 dayOfWeek = (gClock.dayOfWeek == 0) ? 8 : gClock.dayOfWeek;
		u8 hour = (gClock.hour == 0) ? 12 : gClock.hour / 2; //Change every two hours
		u8 day = (gClock.day == 0) ? 32 : gClock.day;
		u8 month = (gClock.month == 0) ? 13 : gClock.month;
		u32 val = ((hour * (day + month)) + ((hour * (day + month)) ^ dayOfWeek)) ^ T1_READ_32(gSaveBlock2->playerTrainerId);
		index = val % gSwarmTableLength;
	}
	#else
	u8 index = VarGet(VAR_SWARM_INDEX);
	#endif

	return index;
}

bool8 IsValidSwarmIndex(u8 index)
{
	return index < gSwarmTableLength;
}

static bool8 TryGenerateSwarmMon(u8 level, u8 wildMonIndex, bool8 purgeParty)
{
	if (gSwarmTableLength == 0)
		return FALSE;

	u8 index = GetCurrentSwarmIndex();
	if (IsValidSwarmIndex(index))
	{
		u8 mapName = gSwarmTable[index].mapName;
		u16 species = gSwarmTable[index].species;

		if (mapName == GetCurrentRegionMapSectionId()
		&& Random() % 100 < SWARM_CHANCE)
		{
			CreateWildMon(species, level, wildMonIndex, purgeParty);
			return TRUE;
		}
	}

	return FALSE;
}

static bool8 TryGenerateWildMon(const struct WildPokemonInfo* wildMonInfo, u8 area, u8 flags)
{
	u8 level;
	u8 wildMonIndex = 0;
	u8 monsCount = 0;

	if (area == WILD_AREA_LAND)
		monsCount = LAND_WILD_COUNT;
	else if (area == WILD_AREA_WATER)
		monsCount = WATER_WILD_COUNT;
	
	if (monsCount != 0)
	{
		if (TryGetAbilityInfluencedWildMonIndex(wildMonInfo->wildPokemon, TYPE_STEEL, ABILITY_MAGNETPULL, &wildMonIndex, monsCount))
			goto SKIP_INDEX_SEARCH;
		if (TryGetAbilityInfluencedWildMonIndex(wildMonInfo->wildPokemon, TYPE_ELECTRIC, ABILITY_STATIC, &wildMonIndex, monsCount))
			goto SKIP_INDEX_SEARCH;
		if (TryGetAbilityInfluencedWildMonIndex(wildMonInfo->wildPokemon, TYPE_ELECTRIC, ABILITY_LIGHTNINGROD, &wildMonIndex, monsCount))
			goto SKIP_INDEX_SEARCH;
		if (TryGetAbilityInfluencedWildMonIndex(wildMonInfo->wildPokemon, TYPE_FIRE, ABILITY_FLASHFIRE, &wildMonIndex, monsCount))
			goto SKIP_INDEX_SEARCH;
		if (TryGetAbilityInfluencedWildMonIndex(wildMonInfo->wildPokemon, TYPE_GRASS, ABILITY_HARVEST, &wildMonIndex, monsCount))
			goto SKIP_INDEX_SEARCH;
		if (TryGetAbilityInfluencedWildMonIndex(wildMonInfo->wildPokemon, TYPE_WATER, ABILITY_STORMDRAIN, &wildMonIndex, monsCount))
			goto SKIP_INDEX_SEARCH;
	}

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

	else if (area != WILD_AREA_LAND)
		return FALSE;

	#ifdef IgnoreWildPokemon
	else if (!TryGenerateSwarmMon(level, wildMonIndex, TRUE))
	{
		CreateWildMon(wildMonInfo->wildPokemon[wildMonIndex].species, level, wildMonIndex, TRUE);
		if (FlagGet(FLAG_ENABLE_WILD_PMN_PREBATTLE_SCREEN))
		{
			Var8001 = wildMonInfo->wildPokemon[wildMonIndex].species;
			Var8000 = 0x7500;
			Var8002 = 0x030A;
			Var8003 = 0xFF03;
			Var8004 = 0x098;
			FlagSet(FLAG_WILD_POKEMON_PREBATTLE_SCREEN);
			ScriptContext1_SetupScript(SystemScript_PokemonEncounter); 
			return FALSE;
		} 
		else
  	{
		   CreateWildMon(wildMonInfo->wildPokemon[wildMonIndex].species, level, wildMonIndex, TRUE);
  	}
	}
	#else
	else if (!TryGenerateSwarmMon(level, wildMonIndex, TRUE))
	{
		CreateWildMon(wildMonInfo->wildPokemon[wildMonIndex].species, level, wildMonIndex, TRUE);
	}
	#endif


	#ifdef FLAG_DOUBLE_WILD_BATTLE
	if (FlagGet(FLAG_DOUBLE_WILD_BATTLE))
	{
		wildMonIndex = 0;

		if (monsCount != 0)
		{
			if (TryGetAbilityInfluencedWildMonIndex(wildMonInfo->wildPokemon, TYPE_STEEL, ABILITY_MAGNETPULL, &wildMonIndex, monsCount))
				goto SKIP_INDEX_SEARCH_2;
			if (TryGetAbilityInfluencedWildMonIndex(wildMonInfo->wildPokemon, TYPE_ELECTRIC, ABILITY_STATIC, &wildMonIndex, monsCount))
				goto SKIP_INDEX_SEARCH_2;
			if (TryGetAbilityInfluencedWildMonIndex(wildMonInfo->wildPokemon, TYPE_ELECTRIC, ABILITY_LIGHTNINGROD, &wildMonIndex, monsCount))
				goto SKIP_INDEX_SEARCH_2;
			if (TryGetAbilityInfluencedWildMonIndex(wildMonInfo->wildPokemon, TYPE_FIRE, ABILITY_FLASHFIRE, &wildMonIndex, monsCount))
				goto SKIP_INDEX_SEARCH_2;
			if (TryGetAbilityInfluencedWildMonIndex(wildMonInfo->wildPokemon, TYPE_GRASS, ABILITY_HARVEST, &wildMonIndex, monsCount))
				goto SKIP_INDEX_SEARCH_2;
			if (TryGetAbilityInfluencedWildMonIndex(wildMonInfo->wildPokemon, TYPE_WATER, ABILITY_STORMDRAIN, &wildMonIndex, monsCount))
				goto SKIP_INDEX_SEARCH_2;
		}

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
		if (area != WILD_AREA_LAND || !TryGenerateSwarmMon(level, wildMonIndex, FALSE))
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
		encounterRate = (encounterRate * BIKE_ENCOUNTER_PERCENT) / 100;

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
		#ifndef ABILITY_WHITESMOKE
		if (IsWhiteSmokeAbility(ability, GetMonData(&gPlayerParty[0], MON_DATA_SPECIES, NULL)))
			ability = ABILITY_STENCH;
		#endif
	
		switch (ability) {
			#ifdef ABILITY_WHITESMOKE
			case ABILITY_WHITESMOKE:
			#endif
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
	const struct WildPokemonInfo* landMonsInfo, *waterMonsInfo;
	struct Roamer* roamer;
	bool8 clearDoubleFlag = FALSE;
	const u8 lowerByte = GetMetatileAttributeFromRawMetatileBehavior(currMetaTileBehavior, METATILE_ATTRIBUTE_ENCOUNTER_TYPE);

	if (sWildEncountersDisabled == TRUE)
		return FALSE;

	#ifdef FLAG_NO_RANDOM_WILD_ENCOUNTERS
	if (FlagGet(FLAG_NO_RANDOM_WILD_ENCOUNTERS))
		return FALSE;
	#endif

	if (lowerByte & TILE_FLAG_ENCOUNTER_TILE)
	{
		if (GetMetatileAttributeFromRawMetatileBehavior(currMetaTileBehavior, METATILE_ATTRIBUTE_BEHAVIOR) != previousMetaTileBehavior
		&& !DoGlobalWildEncounterDiceRoll())
			return FALSE;

		landMonsInfo = LoadProperMonsData(LAND_MONS_HEADER);

		if (landMonsInfo == NULL)
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

			StartRoamerBattle();
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
		if (previousMetaTileBehavior != GetMetatileAttributeFromRawMetatileBehavior(currMetaTileBehavior, METATILE_ATTRIBUTE_BEHAVIOR)
		&& !DoGlobalWildEncounterDiceRoll())
			return FALSE;

		waterMonsInfo = LoadProperMonsData(WATER_MONS_HEADER);

		if (waterMonsInfo == NULL)
		   return FALSE;
		else if (DoWildEncounterRateTest(waterMonsInfo->encounterRate, FALSE) != TRUE)
			return FALSE;

		if (TryStartRoamerEncounter(ENCOUNTER_TYPE_WATER) == TRUE)
		{
			roamer = &gRoamers[gLastSelectedRoamer];
			if (!IsWildLevelAllowedByRepel(roamer->level))
				return FALSE;

			StartRoamerBattle();
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

void HeadbuttWildEncounter(void)
{
	const struct WildPokemonInfo* wildPokemonInfo = LoadProperMonsData(ROCK_SMASH_MONS_HEADER);

	if (wildPokemonInfo == NULL)
		gSpecialVar_LastResult = FALSE;
	else if (DoWildEncounterRateTest(wildPokemonInfo->encounterRate, 1) == TRUE
		  && TryGenerateWildMon(wildPokemonInfo, 2, 0) == TRUE)
	{
		BattleSetup_StartWildBattle();
		gSpecialVar_LastResult = TRUE;
	}
	else
		gSpecialVar_LastResult = FALSE;
}

static void TrySetDoubleSweetScentBattle(void)
{
	#ifdef SWEET_SCENT_WILD_DOUBLE_BATTLES
	if (!FlagGet(FLAG_DOUBLE_WILD_BATTLE) //Flag hasn't already been set
	&& Random() % 100 < WILD_DOUBLE_RANDOM_CHANCE
	&& ViableMonCount(gPlayerParty) >= 2) //Player has two Pokeon that can battle on their own
		FlagSet(FLAG_DOUBLE_WILD_BATTLE); //Sweet Scent can trigger a wild double battle
	#endif
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
	if (MapGridGetMetatileField(x, y, METATILE_ATTRIBUTE_ENCOUNTER_TYPE) & TILE_FLAG_ENCOUNTER_TILE)
	{
		if (landMonsInfo == NULL)
			return FALSE;

		if (TryStartRoamerEncounter(ENCOUNTER_TYPE_LAND) == TRUE)
		{
			StartRoamerBattle();
			return TRUE;
		}

		TrySetDoubleSweetScentBattle();
		TryGenerateWildMon(landMonsInfo, WILD_AREA_LAND, 0);
		BattleSetup_StartWildBattle();
		return TRUE;
	}
	else if (MapGridGetMetatileField(x, y, METATILE_ATTRIBUTE_ENCOUNTER_TYPE) & TILE_FLAG_SURFABLE)
	{
		if (waterMonsInfo == NULL)
			return FALSE;

		if (TryStartRoamerEncounter(ENCOUNTER_TYPE_WATER) == TRUE)
		{
			StartRoamerBattle();
			return TRUE;
		}

		TrySetDoubleSweetScentBattle();
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
			StartRoamerBattle();
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
			StartRoamerBattle();
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

static bool8 TryGetAbilityInfluencedWildMonIndex(const struct WildPokemon* wildMon, u8 type, u8 ability, u8* monIndex, u8 monsCount)
{
	if (GetMonData(&gPlayerParty[0], MON_DATA_IS_EGG, NULL))
		return FALSE;
	else if (GetMonAbility(&gPlayerParty[0]) != ability)
		return FALSE;
	else if (umodsi(Random(), 2) != 0)
		return FALSE;

	return TryGetRandomWildMonIndexByType(wildMon, type, monsCount, monIndex);
}

void DoStandardWildBattle(void)
{
	ScriptContext2_Enable();
	FreezeEventObjects();
	StopPlayerAvatar();
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

	#ifdef FLAG_AI_CONTROL_BATTLE
	if (FlagGet(FLAG_AI_CONTROL_BATTLE))
		gBattleTypeFlags |= BATTLE_TYPE_MOCK_BATTLE;
	#endif

	CreateBattleStartTask(GetWildBattleTransition(), GetMUS_ForBattle());
	IncrementGameStat(GAME_STAT_TOTAL_BATTLES);
	IncrementGameStat(GAME_STAT_WILD_BATTLES);
}

static void StartRoamerBattle(void)
{
	ScriptContext2_Enable();
	FreezeEventObjects();
	StopPlayerAvatar();
	gMain.savedCallback = CB2_EndWildBattle;
	gBattleTypeFlags = BATTLE_TYPE_ROAMER;
	CreateBattleStartTask(GetWildBattleTransition(), GetMUS_ForBattle());
	IncrementGameStat(GAME_STAT_TOTAL_BATTLES);
	IncrementGameStat(GAME_STAT_WILD_BATTLES);
}

void sp138_StartLegendaryBattle(void)
{
	u8 transition = B_TRANSITION_BLUR; //Default
	ScriptContext2_Enable();
	gMain.savedCallback = CB2_EndScriptedWildBattle_2;

	gBattleTypeFlags = BATTLE_TYPE_SCRIPTED_WILD_1 | BATTLE_TYPE_LEGENDARY_FRLG;

	#ifdef FLAG_DOUBLE_WILD_BATTLE
	if (FlagGet(FLAG_DOUBLE_WILD_BATTLE)
	&& gEnemyParty[1].species != SPECIES_NONE
	&& (FlagGet(FLAG_TAG_BATTLE) || ViableMonCount(gPlayerParty) > 1)) //At least two alive Pokemon
	{
		gBattleTypeFlags |= BATTLE_TYPE_DOUBLE;

		if (FlagGet(FLAG_TAG_BATTLE))
			gBattleTypeFlags |=  BATTLE_TYPE_INGAME_PARTNER;
	}
	#endif

	#ifdef FLAG_AI_CONTROL_BATTLE
	if (FlagGet(FLAG_AI_CONTROL_BATTLE))
		gBattleTypeFlags |= BATTLE_TYPE_MOCK_BATTLE;
	#endif

	#ifdef VAR_BATTLE_TRANSITION_LOGO
	u16 transitionLogo = VarGet(VAR_BATTLE_TRANSITION_LOGO);
	if (transitionLogo != 0)
		transition = B_TRANSITION_CUSTOM_LOGO;
	#endif

	CreateBattleStartTask(transition, GetMUS_ForBattle());
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
	u8 transition = 0;

	if (FlagGet(FLAG_BATTLE_FACILITY)) //Only heal in battle facilities
		HealPlayerParty();

	ScriptContext2_Enable();
	gMain.savedCallback = CB2_EndScriptedWildBattle_2;

	gBattleTypeFlags = BATTLE_TYPE_SCRIPTED_WILD_1 | BATTLE_TYPE_DYNAMAX;

	#ifdef FLAG_RAID_BATTLE
	FlagSet(FLAG_RAID_BATTLE);
	#endif

	if (FlagGet(FLAG_DOUBLE_WILD_BATTLE) && ViableMonCount(gPlayerParty) >= 2)
		gBattleTypeFlags |= BATTLE_TYPE_DOUBLE;

	if (FlagGet(FLAG_TAG_BATTLE))
		gBattleTypeFlags |= (BATTLE_TYPE_DOUBLE | BATTLE_TYPE_INGAME_PARTNER);

	if (FlagGet(FLAG_BATTLE_FACILITY))
	{
		VarSet(VAR_BATTLE_FACILITY_BATTLE_TYPE, 0); //So battle type doesn't interfere with anything
		VarSet(VAR_BATTLE_FACILITY_TIER, 0); //So tier doesn't interfere with anything
	}

	#ifdef VAR_BATTLE_TRANSITION_LOGO
	for (u32 i = 0; i < gNumBattleTransitionLogos; ++i)
	{
		if (gBattleTransitionLogos[i].trainerClass == 0xFF) //Dynamax logo
		{
			u16 transitionLogo = i;
			VarSet(VAR_BATTLE_TRANSITION_LOGO, transitionLogo); //Prep for later
			transition =  B_TRANSITION_CUSTOM_LOGO;
		}
	}
	#endif

	CreateBattleStartTask(transition, GetMUS_ForBattle());
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
			species = VarGet(ScriptReadHalfword(ctx));
			level = ScriptReadByte(ctx);
			item = ScriptReadHalfword(ctx);

			#ifdef FLAG_SCALE_WILD_BOSS_LEVEL
			if (FlagGet(FLAG_SCALE_WILD_BOSS_LEVEL))
				level = GetScaledWildBossLevel(level);
			#endif

			CreateScriptedWildMon(species, level, item, specialMoves, i == 0);

			if (i == 0)
				species = ScriptReadByte(ctx); //Skip null values
		}
	}
	else
	#endif
	{
		species = VarGet(species);
		#ifdef FLAG_DOUBLE_WILD_BATTLE
		FlagClear(FLAG_DOUBLE_WILD_BATTLE); //Singular mon
		#endif
		level = ScriptReadByte(ctx);

		#ifdef FLAG_SCALE_WILD_BOSS_LEVEL
		if (FlagGet(FLAG_SCALE_WILD_BOSS_LEVEL))
			level = GetScaledWildBossLevel(level);
		#endif

		item = ScriptReadHalfword(ctx);
		CreateScriptedWildMon(species, level, item, specialMoves, TRUE);
	}

	return FALSE;
}

static void CreateScriptedWildMon(u16 species, u8 level, u16 item, u16* moves, bool8 firstMon)
{
	u8 index = firstMon ? 0 : 1;
	bool8 customMoves = FlagGet(FLAG_WILD_CUSTOM_MOVES); //Backup flag status

	if (firstMon)
		ZeroEnemyPartyMons();

	FlagClear(FLAG_WILD_CUSTOM_MOVES); //Make sure custom moves aren't set in next function
	CreateWildMon(species, level, 0, firstMon);
	if (customMoves)
		FlagSet(FLAG_WILD_CUSTOM_MOVES); //Reset custom move state if necessary

	if (item == 0xFFFF) //Default held item
		SetWildMonHeldItem();
	else if (item != ITEM_NONE)
		SetMonData(&gEnemyParty[index], MON_DATA_HELD_ITEM, &item);

	#ifdef FLAG_WILD_CUSTOM_MOVES
	if (FlagGet(FLAG_WILD_CUSTOM_MOVES))
	{
		u8 ppBonus = 0;

		#ifdef VAR_GAME_DIFFICULTY
		if (VarGet(VAR_GAME_DIFFICULTY) >= OPTIONS_EXPERT_DIFFICULTY)
			ppBonus = 0xFF; //Max PP on all moves
		#endif

		SetMonData(&gEnemyParty[index], MON_DATA_PP_BONUSES, &ppBonus);

		#ifdef FLAG_POKEMON_RANDOMIZER
		if (!FlagGet(FLAG_POKEMON_RANDOMIZER) //When species are changed, the custom moves no longer make sense
		#ifdef FLAG_TEMP_DISABLE_RANDOMIZER
		|| FlagGet(FLAG_TEMP_DISABLE_RANDOMIZER)
		#endif
		)
		#endif
		{
			moves = firstMon ? moves : &moves[4];
			for (int i = 0; i < MAX_MON_MOVES; ++i)
			{
				if (moves[i] != 0xFFFF)
					gEnemyParty[index].moves[i] = moves[i];

				gEnemyParty[index].pp[i] = CalculatePPWithBonus(gEnemyParty[index].moves[i], ppBonus, i);
			}
		}
	}
	#endif

	if (FlagGet(FLAG_HIDDEN_ABILITY))
		gEnemyParty[index].hiddenAbility = TRUE;

	#ifdef UNBOUND
	if (species == SPECIES_SHADOW_WARRIOR)
	{
		//Shadow Warriors have preset natures and can't be shiny
		u32 shadowWarriorPersonalities[] = {0xCC94DC29, 0xEB9752E1}; //Either Adamant or Jolly
		u32 shadowWarriorOtId = 0x0;

		SetMonData(&gEnemyParty[index], MON_DATA_PERSONALITY, &shadowWarriorPersonalities[Random() & 1]); //Randomly set one of the above natures
		SetMonData(&gEnemyParty[index], MON_DATA_OT_ID, &shadowWarriorOtId);

		if (VarGet(VAR_GAME_DIFFICULTY) == OPTIONS_EXPERT_DIFFICULTY)
			gEnemyParty[index].hiddenAbility = TRUE; //Give it Wonder Guard
	}
	#endif
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

	#ifdef UNBOUND
	if (GetCurrentRegionMapSectionId() == MAPSEC_DISTORTION_WORLD && FlagGet(FLAG_HIDE_GIRATINA))
		return SPECIES_NONE; //No cries if Giratina isn't around
	#endif

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
