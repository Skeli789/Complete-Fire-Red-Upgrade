#include "defines.h"
#include "defines_battle.h"
#include "../include/battle_setup.h"
#include "../include/event_data.h"
#include "../include/pokemon.h"
#include "../include/pokemon_storage_system.h"
#include "../include/random.h"
#include "../include/script.h"
#include "../include/string_util.h"
#include "../include/wild_encounter.h"
#include "../include/constants/event_objects.h"
#include "../include/constants/items.h"
#include "../include/constants/maps.h"
#include "../include/constants/pokedex.h"
#include "../include/constants/pokemon.h"
#include "../include/constants/region_map_sections.h"
#include "../include/constants/species.h"
#include "../include/constants/trainer_classes.h"
#include "../include/constants/trainers.h"
#include "../include/constants/tutors.h"

#include "../include/new/ability_tables.h"
#include "../include/new/ai_advanced.h"
#include "../include/new/battle_util.h"
#include "../include/new/build_pokemon.h"
#include "../include/new/build_pokemon_2.h"
#include "../include/new/catching.h"
#include "../include/new/damage_calc.h"
#include "../include/new/dexnav.h"
#include "../include/new/dynamax.h"
#include "../include/new/form_change.h"
#include "../include/new/frontier.h"
#include "../include/new/item.h"
#include "../include/new/learn_move.h"
#include "../include/new/mega.h"
#include "../include/new/move_tables.h"
#include "../include/new/multi.h"
#include "../include/new/pokemon_storage_system.h"
#include "../include/new/species_tables.h"
#include "../include/new/util.h"

#include "Tables/battle_tower_spreads.h"
#include "Tables/raid_encounters.h"
#include "Tables/raid_partners.h"
#include "Tables/trainers_with_evs_table.h"

/*
build_pokemon.c
	modifies the data that is set for generated pokemon, eg. for battle tower/frontier team generation and others.
*/

#define TOTAL_SPREADS ARRAY_COUNT(gFrontierSpreads)
#define TOTAL_LEGENDARY_SPREADS ARRAY_COUNT(gFrontierLegendarySpreads)
#define TOTAL_ARCEUS_SPREADS ARRAY_COUNT(gArceusSpreads)
#define TOTAL_PIKACHU_SPREADS ARRAY_COUNT(gPikachuSpreads)
#define TOTAL_WORMADAM_SPREADS ARRAY_COUNT(gWormadamSpreads)
#define TOTAL_ROTOM_SPREADS ARRAY_COUNT(gRotomSpreads)
#define TOTAL_ORICORIO_SPREADS ARRAY_COUNT(gOricorioSpreads)
#define TOTAL_LITTLE_CUP_SPREADS ARRAY_COUNT(gLittleCupSpreads)
#define TOTAL_MIDDLE_CUP_SPREADS ARRAY_COUNT(gMiddleCupSpreads)

enum
{
	HAZARDS_SETUP,
	BATON_PASSER,
	SCREENER,
	CLERIC,
	FAST_MON,
	BAD_ABILITY,
	NORMAL_IMMUNITY,
	POISON_IMMUNITY,
	GROUND_IMMUNITY,
	FIRE_IMMUNITY,
	WATER_IMMUNITY,
	GRASS_IMMUNITY,
	ELECTRIC_IMMUNITY,
	SOUND_IMMUNITY,
	JUSTIFIED_BOOSTED,

	//Field setters go at the end
	ELECTRIC_TERRAIN_SETTER,
	PSYCHIC_TERRAIN_SETTER,
	RAIN_SETTER,
	HAIL_SETTER,
	NUM_INDEX_CHECKS
};

#define PARTY_INDEX_FIELD_SETTERS_START ELECTRIC_TERRAIN_SETTER

struct TeamBuilder
{
	u16 speciesArray[PARTY_SIZE];
	u16 itemArray[PARTY_SIZE];
	bool8 speciesOnTeam[NATIONAL_DEX_COUNT];
	bool8 moveOnTeam[MOVES_COUNT];
	bool8 abilityOnTeam[ABILITIES_COUNT];
	bool8 itemEffectOnTeam[ITEM_EFFECT_COUNT];
	u8 weaknesses[NUMBER_OF_MON_TYPES];
	bool8 moveTypeOnPlayerTeam[NUMBER_OF_MON_TYPES];
	const struct BattleTowerSpread* spreads[PARTY_SIZE];
	u8 spreadClasses[PARTY_SIZE];
	u8 partyIndex[NUM_INDEX_CHECKS];
	u8 tier;
	u8 battleType;
	u8 monsCount;
	u8 numStalls;
	u8 numChoiceItems;
	u8 numMegas;
	u16 trainerId;
};

struct Immunity
{
	u8 index;
	u8 type;
};

static struct Immunity sImmunities[] =
{
	{NORMAL_IMMUNITY, TYPE_NORMAL},
	{POISON_IMMUNITY, TYPE_POISON},
	{GROUND_IMMUNITY, TYPE_GROUND},
	{FIRE_IMMUNITY, TYPE_FIRE},
	{WATER_IMMUNITY, TYPE_WATER},
	{GRASS_IMMUNITY, TYPE_GRASS},
	{ELECTRIC_IMMUNITY, TYPE_ELECTRIC},
};

extern const u8 gClassPokeBalls[NUM_TRAINER_CLASSES];
extern const species_t gRandomizerSpeciesBanList[];
extern const species_t gSetPerfectXIvList[];
extern const species_t gDeerlingForms[];
extern const species_t gSawsbuckForms[];
extern const species_t gVivillonForms[];
extern const species_t gFurfrouForms[];
extern const species_t gFlabebeForms[];
extern const species_t gFloetteForms[];
extern const species_t gFlorgesForms[];
extern const species_t gPikachuCapForms[];
extern const species_t gAlcremieForms[];
extern const u8 gNumDeerlingForms;
extern const u8 gNumSawsbuckForms;
extern const u8 gNumVivillonForms;
extern const u8 gNumFlabebeForms;
extern const u8 gNumFloetteForms;
extern const u8 gNumFlorgesForms;
extern const u8 gNumFurfrouForms;
extern const u8 gNumPikachuCapForms;
extern const u8 gNumAlcremieForms;

extern bool8 sp051_CanTeamParticipateInSkyBattle(void);
extern bool8 CanMonParticipateInASkyBattle(struct Pokemon* mon);

//This file's functions:
static void TryGiveMonOnlyMetronome(struct Pokemon* mon);
static u8 CreateNPCTrainerParty(struct Pokemon* const party, const u16 trainerNum, const bool8 firstTrainer, const bool8 side);
static u8 GetTrainerMonGender(struct Trainer* trainer);
static u8 GetTrainerMonMovePPBonus(void);
static u8 GetTrainerMonMovePP(u16 move, u8 index);
#if (defined SCALED_TRAINERS && !defined  DEBUG_NO_LEVEL_SCALING)
static u8 GetPlayerBiasedAverageLevel(u8 maxLevel);
static bool8 DoesTrainerBattleModeAllowMonEvolution(void);
static bool8 IsPseudoBossTrainerPartyForLevelScaling(u8 trainerPartyFlags);
#endif
static bool8 IsBossTrainerClassForLevelScaling(u16 trainerId);
static void ModifySpeciesAndLevelForGenericBattle(u16* species, u8* level, u8 minEnemyTeamLevel, u8 highestPlayerTeamLevel, u8 averagePlayerTeamLevel, u8 trainerClass, unusedArg u8 partySize, bool8 shouldEvolve);
static void ModifySpeciesAndLevelForBossBattle(unusedArg u16* species, unusedArg u8* level, unusedArg u8 maxEnemyTeamLevel, unusedArg u8 maxPlayerTeamLevel, unusedArg bool8 shouldEvolve);
#ifdef VAR_GAME_DIFFICULTY
static void GiveMon2BestBaseStatEVs(struct Pokemon* mon);
#endif
static u16 TryReplaceNormalTrainerSpecies(u16 species, unusedArg u16 trainerId);
static u8 BuildFrontierParty(struct Pokemon* const party, const u16 trainerNum, const u8 tier, const bool8 firstTrainer, const bool8 forPlayer, const u8 side);
static void BuildFrontierMultiParty(u8 multiId);
static void BuildRaidMultiParty(void);
static void CreateFrontierMon(struct Pokemon* mon, const u8 level, const struct BattleTowerSpread* spread, const u16 trainerId, const u8 trainerNum, const u8 trainerGender, const bool8 forPlayer);
static void TryFixMiniorForm(struct Pokemon* mon);
static u8 ConvertFrontierAbilityNumToAbility(const u8 abilityNum, const u16 species);
static bool8 BaseStatsTotalGEAlreadyOnTeam(const u16 toCheck, const u8 partySize, u16* speciesArray);
static bool8 SpeciesAlreadyOnTeam(const u16 species, const u8 partySize, const species_t* const speciesArray);
static bool8 ItemAlreadyOnTeam(const u16 item, const u8 partySize, const item_t* const itemArray);
static void AdjustTypesForMegas(const u16 species, const u16 item, u8* const type1, u8* const type2);
static bool8 TeamNotAllSameType(const u16 species, const u16 item, const u8 partySize, const species_t* const speciesArray, const item_t* const itemArray);
static bool8 TooManyLegendariesOnGSCupTeam(const u16 species, const u8 partySize, const species_t* const speciesArray);
static bool8 IsPokemonBannedBasedOnStreak(u16 species, u16 item, u16* speciesArray, u8 monsCount, u16 trainerId, u8 tier, bool8 forPlayer);
static bool8 TeamDoesntHaveSynergy(const struct BattleTowerSpread* const spread, const struct TeamBuilder* const builder, bool8 forPlayer);
static void AddPlayerMoveTypesToBuilder(struct TeamBuilder* builder, u8 monsCount);
static void UpdateBuilderAfterSpread(struct TeamBuilder* builder, const struct BattleTowerSpread* spread, u16 species, u8 ability, u16 item, u8 itemEffect, u32 partyId);
static bool8 CareAboutTeamWeaknessesInTier(u8 tier);
static bool8 IsSpreadWeakToType(u8 moveType, u8 defType1, u8 defType2, u8 ability);
static u16 GivePlayerFrontierMonGivenSpecies(const u16 species, const struct BattleTowerSpread* const spreadTable, const u16 spreadCount);
static const struct BattleTowerSpread* GetSpreadBySpecies(const u16 species, const struct BattleTowerSpread* const spreads, const u16 spreadCount);
static void TryGetSpecialSpeciesSpreadTable(u16 species, const struct BattleTowerSpread** table, u16* spreadCount);
static const struct BattleTowerSpread* TryAdjustSpreadForSpecies(const struct BattleTowerSpread* originalSpread);
static u16 TryAdjustAestheticSpecies(u16 species);
static void SwapMons(struct Pokemon* party, u8 i, u8 j);
static void PostProcessTeam(struct Pokemon* party, struct TeamBuilder* builder);
static void TryShuffleMovesForCamomons(struct Pokemon* party, u8 tier, u16 trainerId);
static u8 GetPartyIdFromPartyData(struct Pokemon* mon);
static u8 GetHighestMonLevel(const struct Pokemon* const party);
static void CheckShinyMon(struct Pokemon* mon);
#ifdef UNBOUND
extern u8 GetEVSpreadNumForUnboundRivalChallenge(struct Pokemon* mon, u32 aiFlags, u8 trainerClass);
extern void TryGiveSpecialTrainerHiddenPower(u16 trainerId, struct Pokemon* mon);
extern void TryGiveSpecialTrainerStatusCondition(u16 trainerId, struct Pokemon* mon);
extern u8 GetCurrentLevelCap(void); //Must be implemented yourself
#endif

#ifdef OPEN_WORLD_TRAINERS

extern const u8 gOpenWorldLevelRanges[NUM_BADGE_OPTIONS][2];
extern const species_t gGeneralTrainerSpreads[NUM_TRAINER_CLASSES][NUM_BADGE_OPTIONS][NUM_MONS_PER_BADGE];

static u8 GetOpenWorldTrainerMonAmount(void);
static u8 GetOpenWorldSpeciesIndex(u32 nameHash, u8 i);
static u8 GetOpenWorldSpeciesLevel(u32 nameHash, u8 i);
u8 GetOpenWorldBadgeCount(void);

#endif

void BuildTrainerPartySetup(void)
{
	u32 i;
	u8 towerTier = VarGet(VAR_BATTLE_FACILITY_TIER);
	gDontFadeWhite = FALSE; //Set if mugshot battle transition was used

	//Build opponent's team
	if (gBattleTypeFlags & BATTLE_TYPE_LINK && gBattleTypeFlags & BATTLE_TYPE_FRONTIER)
	{
		BuildFrontierParty(&gEnemyParty[0], gTrainerBattleOpponent_A, towerTier, TRUE, FALSE, B_SIDE_OPPONENT);
		BuildFrontierParty(&gEnemyParty[3], VarGet(VAR_SECOND_OPPONENT), towerTier, FALSE, FALSE, B_SIDE_OPPONENT);
	}
	else if (gBattleTypeFlags & BATTLE_TYPE_FRONTIER)
	{
		if (!(gBattleTypeFlags & BATTLE_TYPE_RING_CHALLENGE)
		#ifdef FLAG_PRESET_FRONTIER_OPPONENT_TEAM
		&& !FlagGet(FLAG_PRESET_FRONTIER_OPPONENT_TEAM)
		#endif
		/*&& !IsBattleFactoryBattle()*/) //Team should be pregenerated in these facilities
		{
			if (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS)
			{
				BuildFrontierParty(&gEnemyParty[0], gTrainerBattleOpponent_A, towerTier, TRUE, FALSE, B_SIDE_OPPONENT);
				BuildFrontierParty(&gEnemyParty[3], SECOND_OPPONENT, towerTier, FALSE, FALSE, B_SIDE_OPPONENT);
			}
			else
				BuildFrontierParty(&gEnemyParty[0], gTrainerBattleOpponent_A, towerTier, TRUE, FALSE, B_SIDE_OPPONENT);

			if (IsRandomBattleTowerBattle()
			#ifdef FLAG_PRESET_RANDOM_TEAM
			&& !FlagGet(FLAG_PRESET_RANDOM_TEAM)
			#endif
			)
				BuildFrontierParty(gPlayerParty, 0, towerTier, TRUE, TRUE + 1, B_SIDE_PLAYER);
		}
	}
	else if (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS)
	{
		CreateNPCTrainerParty(&gEnemyParty[0], gTrainerBattleOpponent_A, TRUE, B_SIDE_OPPONENT);
		CreateNPCTrainerParty(&gEnemyParty[3], SECOND_OPPONENT, FALSE, B_SIDE_OPPONENT);
	}
	else if (!(gBattleTypeFlags & BATTLE_TYPE_LINK))
	{
		if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)
		{
			#ifdef FLAG_BATTLE_YOURSELF
			if (FlagGet(FLAG_BATTLE_YOURSELF))
			{
				u32 k;
				ZeroEnemyPartyMons();
				
				for (i = 0, k = 0; i < PARTY_SIZE; ++i)
				{
					u16 species = GetMonData(&gPlayerParty[i], MON_DATA_SPECIES2, NULL);
					if (species != SPECIES_NONE && species != SPECIES_EGG) //Prevent Eggs from getting in
					{
						gEnemyParty[k] = gPlayerParty[i];
						HealMon(&gEnemyParty[k++]);
					}
				}

				#ifdef FLAG_DOUBLE_BATTLE
				if (FlagGet(FLAG_DOUBLE_BATTLE) && k >= 2 && ViableMonCount(gPlayerParty) >= 2)
					gBattleTypeFlags |= BATTLE_TYPE_DOUBLE;
				#endif
			}
			else
			#endif
				CreateNPCTrainerParty(&gEnemyParty[0], gTrainerBattleOpponent_A, TRUE, B_SIDE_OPPONENT);
		}
		else if (!(gBattleTypeFlags & (BATTLE_TYPE_POKE_DUDE | BATTLE_TYPE_SCRIPTED_WILD_1)))
			SetWildMonHeldItem();

		if (IsRaidBattle())
		{
			SetWildMonHeldItem();

			if (GetMonData(&gEnemyParty[0], MON_DATA_SPECIES, NULL) != SPECIES_SHEDINJA)
			{
				gEnemyParty[0].maxHP *= GetRaidBattleHPBoost();
				gEnemyParty[0].hp *= GetRaidBattleHPBoost();
			}
		}
	}

	//Format player's team for a Sky Battle
	#ifdef FLAG_SKY_BATTLE
	if (FlagGet(FLAG_SKY_BATTLE))
	{
		if (sp051_CanTeamParticipateInSkyBattle())
		{
			ExtensionState.skyBattlePartyBackup = Calloc(sizeof(struct Pokemon) * PARTY_SIZE);

			if (ExtensionState.skyBattlePartyBackup != NULL)
			{
				u32 j, counter;
				Memset(gSelectedOrderFromParty, 0, sizeof(u8) * PARTY_SIZE); //Clear old data that might interfere

				for (i = 0, j = 0, counter = 0; i < PARTY_SIZE; ++i)
				{
					if (!CanMonParticipateInASkyBattle(&gPlayerParty[i]))
					{
						ExtensionState.skyBattlePartyBackup[counter++] = gPlayerParty[i];
						Memset(&gPlayerParty[i], 0x0, sizeof(struct Pokemon));
					}
					else
					{
						gSelectedOrderFromParty[j++] = i + 1;
					}
				}

				gSelectedOrderFromParty[j] = 0;
				CompactPartySlots();
			}
		}
	}
	#endif

	bool8 useHalfPartnerTeam = FALSE;
	#ifdef FLAG_USE_HALF_PARTNER_TEAM
	useHalfPartnerTeam = FlagGet(FLAG_USE_HALF_PARTNER_TEAM); //Partner's team still replaces second half of team but you have control
	#endif

	//Build multi partner's team
	if (((gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER) || useHalfPartnerTeam)
	&& (ViableMonCount(gEnemyParty) > 1 || IsRaidBattle() || useHalfPartnerTeam))
	{
		if (gBattleTypeFlags & BATTLE_TYPE_FRONTIER)
		{
			if (IsRandomBattleTowerBattle()
			|| GetMonData(&gPlayerParty[3], MON_DATA_SPECIES, NULL) == SPECIES_NONE) //Ideally the partner's Pokemon should be prepared beforehand. This is a backup measure
				BuildFrontierParty(&gPlayerParty[3], VarGet(VAR_PARTNER), towerTier, 3, FALSE, B_SIDE_PLAYER);
		}
		else
		{
			u32 k;
			ExtensionState.partyBackup = Calloc(sizeof(struct Pokemon) * PARTY_SIZE);
			if (ExtensionState.partyBackup != NULL)
			{
				if (gSelectedOrderFromParty[0] == 0) //Special 0x2F was not used
				{
					//Choose first three viable Pokemon on team to fight
					for (i = 0, k = 0; i < PARTY_SIZE && k < 3; ++i)
					{
						u16 species = GetMonData(&gPlayerParty[i], MON_DATA_SPECIES2, NULL);
						if (species != SPECIES_NONE && species != SPECIES_EGG
						&& GetMonData(&gPlayerParty[i], MON_DATA_HP, NULL) > 0)
							gSelectedOrderFromParty[k++] = i + 1;
					}
				}

				u8 counter = 0;
				u8 mon1 = gSelectedOrderFromParty[0];
				u8 mon2 = gSelectedOrderFromParty[1];
				u8 mon3 = gSelectedOrderFromParty[2];
				for (i = 0; i < PARTY_SIZE; ++i)
				{
					if (i + 1 != mon1 && i + 1 != mon2 && i + 1 != mon3) //Don't backup selected mons
						Memcpy(&((struct Pokemon*) ExtensionState.partyBackup)[counter++], &gPlayerParty[i], sizeof(struct Pokemon));
				}

				ReducePartyToThree(); //Well... sometimes can be less than 3
				Memset(&gPlayerParty[3], 0x0, sizeof(struct Pokemon) * 3);

				if (IsRaidBattle() && VarGet(VAR_PARTNER) == RAID_BATTLE_MULTI_TRAINER_TID)
					BuildRaidMultiParty();
				else
					CreateNPCTrainerParty(&gPlayerParty[3], VarGet(VAR_PARTNER), FALSE, B_SIDE_PLAYER);

				if (useHalfPartnerTeam)
					CompactPartySlots();
			}
		}
	}

	//Try swapping a Pokemon in the Battle Circus
	if (gBattleTypeFlags & BATTLE_TYPE_BATTLE_CIRCUS && gBattleCircusFlags & BATTLE_CIRCUS_TRADE_MON)
	{
		//Swap a random Pokemon on each side of the field
		u8 playerMonId, enemyMonId;
		struct Pokemon temp;

		do
		{
			playerMonId = Random() % PARTY_SIZE;
		} while (GetMonData(&gPlayerParty[playerMonId], MON_DATA_SPECIES, NULL) == SPECIES_NONE);

		do
		{
			enemyMonId = Random() % PARTY_SIZE;
		} while (GetMonData(&gEnemyParty[enemyMonId], MON_DATA_SPECIES, NULL) == SPECIES_NONE);
		
		temp = gPlayerParty[playerMonId];
		gPlayerParty[playerMonId] = gEnemyParty[enemyMonId];
		gEnemyParty[enemyMonId] = temp;
	}

	//Give every Pokemon only Metronome in a Metronome battle
	if ((gBattleTypeFlags & BATTLE_TYPE_FRONTIER && VarGet(VAR_BATTLE_FACILITY_TIER) == BATTLE_FACILITY_METRONOME)
	#ifdef FLAG_METRONOME_BATTLE
	|| FlagGet(FLAG_METRONOME_BATTLE)
	#endif
	)
	{
		for (i = 0; i < PARTY_SIZE; ++i)
		{
			TryGiveMonOnlyMetronome(&gPlayerParty[i]);
			TryGiveMonOnlyMetronome(&gEnemyParty[i]);
		}
	}

	//Try change Zacian and Zamazenta's form
	TryCrownZacianZamazenta(gPlayerParty);
	TryCrownZacianZamazenta(gEnemyParty);

	if (ViableMonCount(gEnemyParty) <= 1 && !IsRaidBattle() && !(gBattleTypeFlags & BATTLE_TYPE_LINK)) //Error prevention
		gBattleTypeFlags &= ~(BATTLE_TYPE_INGAME_PARTNER | BATTLE_TYPE_TWO_OPPONENTS | BATTLE_TYPE_DOUBLE);
}

static void TryGiveMonOnlyMetronome(struct Pokemon* mon)
{
	u32 j;
	u16 species = GetMonData(mon, MON_DATA_SPECIES2, NULL);
	
	if (species != SPECIES_NONE && species != SPECIES_EGG)
	{
		u16 item = ITEM_LEPPA_BERRY; //Makes it so Metronome has even more PP mwahaha
		u16 move = MOVE_METRONOME;
		u8 bonus = 3; //Only needs to be 3 because move index is force set at 0 (next line)
		u8 pp = CalculatePPWithBonus(MOVE_METRONOME, bonus, 0);
		SetMonData(mon, MON_DATA_MOVE1, &move);
		SetMonData(mon, MON_DATA_PP1, &pp);
		SetMonData(mon, MON_DATA_PP_BONUSES, &bonus);
		
		for (j = 1, move = MOVE_NONE, pp = 0; j < MAX_MON_MOVES; ++j)
		{
			SetMonData(mon, MON_DATA_MOVE1 + j, &move);
			SetMonData(mon, MON_DATA_PP1 + j, &pp);
		}

		SetMonData(mon, MON_DATA_HELD_ITEM, &item);
	}
}

extern void SortItemsInBag(u8 pocket, u8 type);
void sp067_GenerateRandomBattleTowerTeam(void)
{
	u8 tier, i;
	struct Pokemon backupMon;

	if (Var8001 == 2)
	{
		u8 numEnemyMons = PARTY_SIZE / 2;

		if (IsFrontierMulti(VarGet(VAR_BATTLE_FACILITY_BATTLE_TYPE)))
		{
			if (VarGet(VAR_BATTLE_FACILITY_POKE_NUM) == 2)
			{
				numEnemyMons = 4; //Use both full enemy teams

				if (GetMonData(&gEnemyParty[2], MON_DATA_SPECIES, NULL) == SPECIES_NONE)
					gEnemyParty[2] = gEnemyParty[4]; //Move up last mon on second team since teams are staggared
			}
			//else
				//Otherwise the standard 3
		}

		//Splice enemy's team into player's team
		Memcpy(&gPlayerParty[PARTY_SIZE - numEnemyMons], gEnemyParty, sizeof(struct Pokemon) * numEnemyMons);
		CompactPartySlots();
		#ifdef FLAG_PRESET_RANDOM_TEAM
		FlagSet(FLAG_PRESET_RANDOM_TEAM);
		#endif
		return;
	}

	switch (Var8000) {
		case 0:
		default:
			tier = BATTLE_FACILITY_STANDARD;
			break;

		case 1: //Legendary Pokemon
			tier = BATTLE_FACILITY_UBER;
			break;

		case 2: //Little Cup
			tier = BATTLE_FACILITY_LITTLE_CUP;
			break;

		case 3: //Middle Cup
			tier = BATTLE_FACILITY_MIDDLE_CUP;
			break;

		case 4:
			tier = VarGet(VAR_BATTLE_FACILITY_TIER);
			break;
	}

	if (Var8001 == 1) //Keep team lead from previous battle
		backupMon = gPlayerParty[0];

	#ifdef FLAG_PRESET_RANDOM_TEAM
	FlagSet(FLAG_PRESET_RANDOM_TEAM);
	#endif
	VarSet(VAR_BATTLE_FACILITY_TIER, tier);
	BuildFrontierParty(gPlayerParty, 0, tier, TRUE, TRUE, B_SIDE_PLAYER);

	if (Var8001) //Keep team lead from previous battle
	{
		u16 backupSpecies = GetMonData(&backupMon, MON_DATA_SPECIES, NULL);
		for (i = 0; i < PARTY_SIZE; ++i)
		{
			if (GetMonData(&gPlayerParty[i], MON_DATA_SPECIES, NULL) == backupSpecies)
				break; //Replace this mon because duplicate species should never be offered
		}

		if (i >= PARTY_SIZE) //Duplicate species wasn't found
			i = 0; //Just replace first mon

		gPlayerParty[i] = backupMon;
		SwapMons(gPlayerParty, 0, i);
	}

	/*for (u32 i = TOTAL_SPREADS / 2; i < TOTAL_SPREADS; ++i)
	{
		struct Pokemon mon;
		const struct BattleTowerSpread* spread = TryAdjustSpreadForSpecies(&gFrontierSpreads[i]);
		*((u16*) 0x2023D6C) = spread->species;
		CreateFrontierMon(&mon, 50, spread, 0, 0, 0, TRUE);
		GiveMonToPlayer(&mon);
	}*/

	/*for (int i = 0; i < ITEMS_COUNT; ++i)
	{
		const u8* name = ItemId_GetName(i);
		if (name[0] != 0xAC && name[0] != 0xFF) //'?', ' '
			AddBagItem(i, 1);
	}

	SortItemsInBag(0, 1);*/

	/*for (u32 i = 0; i <= SPECIES_STUNFISK_G; ++i)
	{
		if (gSpecialSpeciesFlags[i].randomizerBan)
			continue;

		if (!CanSpeciesEvolve(i))
		{
			struct Pokemon mon;
			CreateMon(&mon, i, 100, 31, FALSE, 0, OT_ID_PLAYER_ID, 0);
			GiveMonToPlayer(&mon);
		}
	}*/
}

//@Details: Adds a Pokemon with the given species from the requested spreads to
//			the player's party. If no space, then adds it to the PC.
//@Inputs:
//		Var8000: Species
//		Var8001: Spread Type
//@Returns: If the Pokemon was added or not.
u16 sp068_GivePlayerFrontierMonGivenSpecies(void)
{
	u16 species = Var8000;

	u16 spreadCount = 0;
	const struct BattleTowerSpread* spreads = NULL;
	TryGetSpecialSpeciesSpreadTable(species, &spreads, &spreadCount);

	if (spreads == NULL) //No special table like Arceus or Pikachu was found
	{
		switch (Var8001) {
			case 0:
			default:
				spreadCount = TOTAL_SPREADS;
				spreads = gFrontierSpreads;
				break;
			case 1:
				spreadCount = TOTAL_LITTLE_CUP_SPREADS;
				spreads = gLittleCupSpreads;
				break;
			case 2:
				spreadCount = TOTAL_MIDDLE_CUP_SPREADS;
				spreads = gMiddleCupSpreads;
				break;
			case 3:
				spreadCount = TOTAL_LEGENDARY_SPREADS;
				spreads = gFrontierLegendarySpreads;
				break;
			case 4: //Any Spread
				spreadCount = TOTAL_SPREADS;
				spreads = gFrontierSpreads;
				u16 val = GivePlayerFrontierMonGivenSpecies(species, spreads, spreadCount);

				if (val != 0xFFFF)
					return val;

				spreadCount = TOTAL_LEGENDARY_SPREADS;
				spreads = gFrontierLegendarySpreads;
				val = GivePlayerFrontierMonGivenSpecies(species, spreads, spreadCount);

				if (val != 0xFFFF)
					return val;

				spreadCount = TOTAL_MIDDLE_CUP_SPREADS;
				spreads = gMiddleCupSpreads;
				val = GivePlayerFrontierMonGivenSpecies(species, spreads, spreadCount);

				if (val != 0xFFFF)
					return val;

				spreadCount = TOTAL_LITTLE_CUP_SPREADS;
				spreads = gLittleCupSpreads;
				break;
		}
	}

	return GivePlayerFrontierMonGivenSpecies(species, spreads, spreadCount);
}

//@Details: Add a random Pokemon battleable in the given tier.
//@Inputs:
//		Var8000: Tier
//		Var8001: Spread Type
//@Returns: If the Pokemon was added or not.
u16 sp069_GivePlayerRandomFrontierMonByTier(void)
{
	return GiveRandomFrontierMonByTier(B_SIDE_PLAYER, Var8000, Var8001);
}

//@Details: Creates the opposing team for a Frontier battle in the overworld.
//@Inputs:
//		Var8000: Trainer Id
void sp0E7_CreateFrontierOpponentTeamBeforeBattle(void)
{
	BuildFrontierParty(&gEnemyParty[0], Var8000, VarGet(VAR_BATTLE_FACILITY_TIER), TRUE, FALSE, B_SIDE_OPPONENT);
}

u16 GiveRandomFrontierMonByTier(u8 side, u8 tier, u16 spreadType)
{
	u8 level;
	u16 spreadCount;
	struct Pokemon mon;
	const struct BattleTowerSpread* spread;
	const struct BattleTowerSpread* spreads;

	switch (spreadType) {
		case 0:
		default:
			level = 50;
			spreadCount = TOTAL_SPREADS;
			spreads = gFrontierSpreads;
			tier = BATTLE_FACILITY_STANDARD;
			break;

		case 1: //Legendary Pokemon
			level = 50;
			spreadCount = TOTAL_LEGENDARY_SPREADS;
			spreads = gFrontierLegendarySpreads;
			tier = BATTLE_FACILITY_NO_RESTRICTIONS;
			break;

		case 2: //Little Cup
			level = 5;
			spreadCount = TOTAL_LITTLE_CUP_SPREADS;
			spreads = gLittleCupSpreads;
			tier = BATTLE_FACILITY_LITTLE_CUP;
			break;

		case 3: //Middle Cup
			level = 50;
			spreadCount = TOTAL_MIDDLE_CUP_SPREADS;
			spreads = gMiddleCupSpreads;
			tier = BATTLE_FACILITY_MIDDLE_CUP;
			break;
	}

	do
	{
		spread = TryAdjustSpreadForSpecies(&spreads[Random() % spreadCount]);
	} while (IsPokemonBannedBasedOnStreak(spread->species, spread->item, NULL, 0, 0, tier, TRUE)
		  || PokemonTierBan(spread->species, spread->item, spread, NULL, tier, CHECK_BATTLE_TOWER_SPREADS));

	CreateFrontierMon(&mon, level, spread, 0, 0, 0, TRUE);

	if (side == B_SIDE_PLAYER)
	{
		SetMonPokedexFlags(&mon);
		return GiveMonToPlayer(&mon);
	}
	else
	{
		gEnemyParty[0] = mon;
		return TRUE;
	}
}

u16 sp06A_GivePlayerFrontierMonByLoadedSpread(void)
{
	struct Pokemon mon;
	struct BattleTowerSpread* spread = (struct BattleTowerSpread*) gLoadPointer;

	CreateFrontierMon(&mon, Var8000, spread, 0, 0, 0, TRUE);

	#ifdef FLAG_POKEMON_RANDOMIZER
	if (FlagGet(FLAG_POKEMON_RANDOMIZER))
	{
		Memset(mon.moves, 0, sizeof(mon.moves)); //Wipe custom moves
		GiveBoxMonInitialMoveset((struct BoxPokemon*) &mon);
	}
	#endif

	#ifdef FLAG_POKEMON_LEARNSET_RANDOMIZER
	if (FlagGet(FLAG_POKEMON_LEARNSET_RANDOMIZER))
	{
		Memset(mon.moves, 0, sizeof(mon.moves)); //Wipe custom moves
		GiveBoxMonInitialMoveset((struct BoxPokemon*) &mon); //Of randomized moves
	}
	#endif

	SetMonPokedexFlags(&mon);
	return GiveMonToPlayer(&mon);
}

//@Details: Temporily replaces the player's team with the the team of the given multi
//			battle trainer to allow the player to choose which Pokemon they want the
//			partner to use.
//@Inputs:
//		Var8000: Given Id.
void sp06B_ReplacePlayerTeamWithMultiTrainerTeam(void)
{
	VarSet(VAR_FACILITY_TRAINER_ID_PARTNER, Var8000); //Due to the dumb way I set up CreateFrontierMon this is necessary for trainer data to load properly
	BackupPartyToTempTeam(0, 3);
	BuildFrontierMultiParty(Var8000);
}

//Returns the number of Pokemon
static u8 CreateNPCTrainerParty(struct Pokemon* const party, const u16 trainerId, const bool8 firstTrainer, const bool8 side)
{
	u32 i, j, nameHash;
	unusedArg u8 monsCount, baseIV, setMonGender, trainerNameLengthOddness, minPartyLevel, maxPartyLevel,
	   modifiedAveragePlayerLevel, highestPlayerLevel, canEvolveMon, canEvolveMonBackup, levelScaling, setCustomMoves;
	struct Trainer* trainer;
	u32 otid = 0;
	u8 otIdType = OT_ID_RANDOM_NO_SHINY;

	if (trainerId == TRAINER_SECRET_BASE)
		return 0;
	else if (IsFrontierTrainerId(trainerId))
		return BuildFrontierParty(party, trainerId, VarGet(VAR_BATTLE_FACILITY_TIER), firstTrainer, FALSE, side);

	//Check if can build team
	if (((gBattleTypeFlags & (BATTLE_TYPE_TRAINER | BATTLE_TYPE_EREADER_TRAINER | BATTLE_TYPE_TRAINER_TOWER)) == BATTLE_TYPE_TRAINER)
	||   (gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER))
	{
		if (firstTrainer && side == B_SIDE_OPPONENT)
			ZeroEnemyPartyMons();

		//Set up necessary data
		trainer = &gTrainers[trainerId];

		//Choose Trainer IVs
		#ifdef VAR_GAME_DIFFICULTY
		u8 gameDifficulty = VarGet(VAR_GAME_DIFFICULTY);
		if (gameDifficulty >= OPTIONS_EXPERT_DIFFICULTY && side != B_SIDE_PLAYER) //Not partner
			baseIV = 31;
		else
		#endif
		{
			baseIV = MathMin(gBaseIVsByTrainerClass[trainer->trainerClass], 31);
			if (baseIV == 0) //No data in the table
				baseIV = STANDARD_IV;
		}

		//Choose Trainer Pokemon genders
		setMonGender = GetTrainerMonGender(trainer);
		if (!firstTrainer && side == B_SIDE_PLAYER && trainer->encounterMusic > 0) //Multi partner with preset Id
		{
			otid = gFrontierMultiBattleTrainers[trainer->encounterMusic - 1].otId;
			otIdType = OT_ID_PRESET;
			setMonGender = trainer->gender; //So all Pokemon have the same gender every time
		}

		//Get party size
		if (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS && side == B_SIDE_OPPONENT)
		{
			#ifdef OPEN_WORLD_TRAINERS
			if ((firstTrainer && gTrainerBattleOpponent_A < DYNAMIC_TRAINER_LIMIT)
			||  (!firstTrainer && VarGet(VAR_SECOND_OPPONENT) < DYNAMIC_TRAINER_LIMIT))
			{
				u8 openWorldAmount = GetOpenWorldTrainerMonAmount();
				if (openWorldAmount > 3)
					monsCount = 3;
				else
					monsCount = openWorldAmount;
			}
			else
			#endif
			{
				if (trainer->partySize > 3)
					monsCount = 3;
				else
					monsCount = trainer->partySize;
			}
		}
		else if (gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER && side == B_SIDE_PLAYER)
		{
			if (trainer->partySize > 3)
				monsCount = 3;
			else
				monsCount = trainer->partySize;
		}
		else
		{
			#ifdef OPEN_WORLD_TRAINERS
			if (gTrainerBattleOpponent_A < DYNAMIC_TRAINER_LIMIT)
			{
				monsCount = GetOpenWorldTrainerMonAmount();

				if (trainer->doubleBattle)
					monsCount = MathMax(monsCount, 2);
			}
			else
			#endif
				monsCount = trainer->partySize;
		}

		//Get details for level scaling
		#if (defined SCALED_TRAINERS && !defined DEBUG_NO_LEVEL_SCALING)
		#ifdef VAR_GAME_DIFFICULTY
		levelScaling = gameDifficulty != OPTIONS_EASY_DIFFICULTY //Don't scale Trainers on easy mode
					|| GetCurrentRegionMapSectionId() == MAPSEC_POKEMON_LEAGUE //Unless you're facing the final bosses
					#ifdef UNBOUND
					|| trainerId == 0x223 //Auburn Waterway Hiker needs to scale to confirm you're prepared to face the wild mons below
					#endif
					|| (!(gBattleTypeFlags & BATTLE_TYPE_TRAINER) && FlagGet(FLAG_SCALE_WILD_BOSS_LEVEL)); //Scale the partner up against a scaled wild boss
		#else
		levelScaling = TRUE;
		#endif

		minPartyLevel = MAX_LEVEL;
		maxPartyLevel = 0;
		highestPlayerLevel = GetHighestMonLevel(gPlayerParty);
		modifiedAveragePlayerLevel = GetPlayerBiasedAverageLevel(highestPlayerLevel);
		canEvolveMon = side == B_SIDE_OPPONENT //Partners can't evolve their Pokemon
					&& DoesTrainerBattleModeAllowMonEvolution();

		if (side == B_SIDE_OPPONENT || !firstTrainer) //Only worth calculating if the Trainer is the enemy or the partner
		{
			for (i = 0; i < monsCount; ++i)
			{
				switch (trainer->partyFlags) {
					case 0:
						minPartyLevel = (trainer->party.NoItemDefaultMoves[i].lvl < minPartyLevel) ? trainer->party.NoItemDefaultMoves[i].lvl : minPartyLevel;
						maxPartyLevel = (trainer->party.NoItemDefaultMoves[i].lvl > maxPartyLevel) ? trainer->party.NoItemDefaultMoves[i].lvl : maxPartyLevel;
						break;
					case PARTY_FLAG_CUSTOM_MOVES:
						minPartyLevel = (trainer->party.NoItemCustomMoves[i].lvl < minPartyLevel) ? trainer->party.NoItemCustomMoves[i].lvl : minPartyLevel;
						maxPartyLevel = (trainer->party.NoItemCustomMoves[i].lvl > maxPartyLevel) ? trainer->party.NoItemCustomMoves[i].lvl : maxPartyLevel;
						break;
					case PARTY_FLAG_HAS_ITEM:
						minPartyLevel = (trainer->party.ItemDefaultMoves[i].lvl < minPartyLevel) ? trainer->party.ItemDefaultMoves[i].lvl : minPartyLevel;
						maxPartyLevel = (trainer->party.ItemDefaultMoves[i].lvl > maxPartyLevel) ? trainer->party.ItemDefaultMoves[i].lvl : maxPartyLevel;
						break;
					case PARTY_FLAG_CUSTOM_MOVES | PARTY_FLAG_HAS_ITEM:
						minPartyLevel = (trainer->party.ItemCustomMoves[i].lvl < minPartyLevel) ? trainer->party.ItemCustomMoves[i].lvl : minPartyLevel;
						maxPartyLevel = (trainer->party.ItemCustomMoves[i].lvl > maxPartyLevel) ? trainer->party.ItemCustomMoves[i].lvl : maxPartyLevel;
						break;
				}
			}
		}
		#else //No level scaling
		levelScaling = FALSE;
		minPartyLevel = MAX_LEVEL;
		maxPartyLevel = 0;
		modifiedAveragePlayerLevel = 0;
		highestPlayerLevel = 0;
		canEvolveMon = FALSE;
		#endif

		#ifdef FLAG_POKEMON_RANDOMIZER
		setCustomMoves = FlagGet(FLAG_BATTLE_FACILITY) //Don't set custom moves when the species wouldn't be randomized normally
					|| !FlagGet(FLAG_POKEMON_RANDOMIZER) //Or when species aren't randomized
					#ifdef FLAG_TEMP_DISABLE_RANDOMIZER
					|| FlagGet(FLAG_TEMP_DISABLE_RANDOMIZER) //Unless the randomizer is disabled
					#endif
					;
		#else
		setCustomMoves = TRUE;
		#endif

		//Create each Pokemon
		for (i = 0, trainerNameLengthOddness = StringLength(trainer->trainerName) & 1, nameHash = 0; i < monsCount; ++i)
		{
			u32 personalityValue;
			u8 genderOffset = 0x80;
			struct Pokemon* mon = &party[i];

			if (setMonGender == 1)
			{
				genderOffset = 0x78; //Female
				personalityValue = genderOffset;
				goto SKIP_SET_RANDOM_PERSONALITY;
			}
			else if (setMonGender == 0)
			{
				genderOffset = 0x88; //Male
				personalityValue = genderOffset;
				goto SKIP_SET_RANDOM_PERSONALITY;
			}
			else if ((i + 1) % 3 == 0) //Every third Pokemon
			{
				if (trainerNameLengthOddness == 0) //If trainer name length is even
					genderOffset = 0x78; //Female
				else
					genderOffset = 0x88; //Male
			}
			else
			{
				if (trainerNameLengthOddness == 0) //If trainer name length is even
					genderOffset = 0x88; //Male
				else
					genderOffset = 0x78; //Female
			}

			personalityValue = genderOffset ^ StringLength(trainer->trainerName); //"Randomize" ability

			SKIP_SET_RANDOM_PERSONALITY:
			for (j = 0; trainer->trainerName[j] != EOS; ++j)
				nameHash += trainer->trainerName[j];

			#ifdef OPEN_WORLD_TRAINERS
			u8 openWorldSpeciesIndex = GetOpenWorldSpeciesIndex(nameHash, i);
			u8 openWorldLevel = GetOpenWorldSpeciesLevel(nameHash, i);

			if (gTrainerBattleOpponent_A < DYNAMIC_TRAINER_LIMIT)
			{
				u16 speciesToCreate = gGeneralTrainerSpreads[trainer->trainerClass][GetOpenWorldBadgeCount()][openWorldSpeciesIndex];

				if (FlagGet(FLAG_SCALE_TRAINER_LEVELS) || (gBattleTypeFlags & BATTLE_TYPE_TRAINER_TOWER))
					openWorldLevel = GetHighestMonLevel(gPlayerParty);

				CreateMon(mon, speciesToCreate, openWorldLevel, STANDARD_IV, TRUE, personalityValue, otIdType, otid);
			}
			else
			#endif
			{
				switch (trainer->partyFlags) {
					case 0:
						MAKE_POKEMON(trainer->party.NoItemDefaultMoves);
						break;

					case PARTY_FLAG_CUSTOM_MOVES:
						MAKE_POKEMON(trainer->party.NoItemCustomMoves);
						if (setCustomMoves)
							SET_MOVES(trainer->party.NoItemCustomMoves);
						break;

					case PARTY_FLAG_HAS_ITEM: ;
						#if (defined SCALED_TRAINERS && !defined  DEBUG_NO_LEVEL_SCALING)
						canEvolveMonBackup = canEvolveMon;
						if (trainer->party.ItemDefaultMoves[i].heldItem == ITEM_EVIOLITE)
							canEvolveMon = FALSE; //Don't try to evolve mon holding Eviolite
						MAKE_POKEMON(trainer->party.ItemDefaultMoves);
						if (trainer->party.ItemDefaultMoves[i].heldItem == ITEM_EVIOLITE)
							canEvolveMon = canEvolveMonBackup; //Restore original option for rest of team
						#else
						MAKE_POKEMON(trainer->party.ItemDefaultMoves);
						#endif
						SetMonData(mon, MON_DATA_HELD_ITEM, &trainer->party.ItemDefaultMoves[i].heldItem);
						break;

					case PARTY_FLAG_CUSTOM_MOVES | PARTY_FLAG_HAS_ITEM:
						#if (defined SCALED_TRAINERS && !defined  DEBUG_NO_LEVEL_SCALING)
						canEvolveMonBackup = canEvolveMon;
						if (trainer->party.ItemCustomMoves[i].heldItem == ITEM_EVIOLITE)
							canEvolveMon = FALSE; //Don't try to evolve mon holding Eviolite
						MAKE_POKEMON(trainer->party.ItemCustomMoves);
						if (trainer->party.ItemCustomMoves[i].heldItem == ITEM_EVIOLITE)
							canEvolveMon = canEvolveMonBackup; //Restore original option for rest of team
						#else
						MAKE_POKEMON(trainer->party.ItemCustomMoves);
						#endif

						if (setCustomMoves)
							SET_MOVES(trainer->party.ItemCustomMoves);
						SetMonData(mon, MON_DATA_HELD_ITEM, &trainer->party.ItemCustomMoves[i].heldItem);
						break;
				}
			}

			//Assign Trainer information to mon
			u8 otGender = trainer->gender;
			const u8* name = TryGetRivalNameByTrainerClass(gTrainers[trainerId].trainerClass);
			if (name == NULL) //Not Rival or Rival name isn't tied to Trainer class
				SetMonData(mon, MON_DATA_OT_NAME, &trainer->trainerName);
			else
				SetMonData(mon, MON_DATA_OT_NAME, name);
			SetMonData(mon, MON_DATA_OT_GENDER, &otGender);

			//Give custom Poke Ball
			#ifdef TRAINER_CLASS_POKE_BALLS
			SetMonData(mon, MON_DATA_POKEBALL, &gClassPokeBalls[trainer->trainerClass]);
			#endif

			//Try Evolve Randomized Mon
			#if (defined FLAG_POKEMON_RANDOMIZER && defined FLAG_TEMP_DISABLE_RANDOMIZER && defined VAR_GAME_DIFFICULTY)
			if (FlagGet(FLAG_POKEMON_RANDOMIZER) && !FlagGet(FLAG_TEMP_DISABLE_RANDOMIZER) && gameDifficulty != OPTIONS_EASY_DIFFICULTY) //Allow Trainers to grow naturally
			{
				if (EvolveSpeciesByLevel(&mon->species, mon->level))
				{
					//Fix nickname in case it changed after evolving
					u8 speciesName[POKEMON_NAME_LENGTH + 1];
					GetSpeciesName(speciesName, mon->species);
					SetMonData(mon, MON_DATA_NICKNAME, speciesName);
				}
			}
			#endif

			//Give EVs
			#ifdef TRAINERS_WITH_EVS
			u8 spreadNum = (gTrainers[trainerId].partyFlags & PARTY_FLAG_CUSTOM_MOVES) ? trainer->party.NoItemCustomMoves[i].iv : trainer->party.NoItemDefaultMoves[i].iv;

			#ifdef UNBOUND
			if ((gTrainers[trainerId].trainerClass == CLASS_RIVAL && gameDifficulty >= OPTIONS_HARD_DIFFICULTY)
			 || (gTrainers[trainerId].trainerClass == CLASS_RIVAL_2 && gameDifficulty == OPTIONS_HARD_DIFFICULTY)) //Not for Insane
				spreadNum = GetEVSpreadNumForUnboundRivalChallenge(mon, trainer->aiFlags, gTrainers[trainerId].trainerClass);
			#endif

			if (spreadNum != 0
			&& spreadNum < NELEMS(gTrainersWithEvsSpreads) //Valid id
			#ifndef UNBOUND
			&& gTrainers[trainerId].partyFlags == (PARTY_FLAG_CUSTOM_MOVES | PARTY_FLAG_HAS_ITEM)
			&& trainer->aiFlags > 1
			#endif
			)
			{
				const struct TrainersWithEvs* spread = &gTrainersWithEvsSpreads[spreadNum];

				#ifdef VAR_GAME_DIFFICULTY
				if (gameDifficulty != OPTIONS_EASY_DIFFICULTY)
				#endif
				{
					SET_EVS(spread);
					if (spread->ivs != 0) //Otherwise use default class values
						SET_IVS_SINGLE_VALUE(MathMin(31, spread->ivs));
				}

				if (MoveInMonMoveset(MOVE_TRICKROOM, mon)
				|| MoveInMonMoveset(MOVE_GYROBALL, mon)
				#ifdef FLAG_TRICK_ROOM_BATTLE
				|| FlagGet(FLAG_TRICK_ROOM_BATTLE)
				#endif
				)
				{
					//Set all speed IVs to 0 in a forced Trick Room battle
					u32 zero = 0;
					SetMonData(mon, MON_DATA_SPEED_IV, &zero);
				}

				u8 ballType;
				switch(spread->ball) {
					case TRAINER_EV_CLASS_BALL:
					#ifdef TRAINER_CLASS_POKE_BALLS
						ballType = gClassPokeBalls[trainer->trainerClass];
					#else
						ballType = BALL_TYPE_POKE_BALL;
					#endif
						break;
					case TRAINER_EV_RANDOM_BALL:
						ballType = Random() % NUM_BALLS; //Set Random Ball
						break;
					default:
						ballType = MathMin(spread->ball, LAST_BALL_INDEX);
				}

				SetMonData(mon, MON_DATA_POKEBALL, &ballType);

				switch(spread->ability) {
					case Ability_Hidden:
					TRAINER_WITH_EV_GIVE_HIDDEN_ABILITY:
						GiveMonNatureAndAbility(mon, spread->nature, 0xFF, FALSE, TRUE, FALSE); //Give Hidden Ability
						break;
					case Ability_1:
					case Ability_2:
						GiveMonNatureAndAbility(mon, spread->nature, MathMin(1, spread->ability - 1), FALSE, TRUE, FALSE);
						break;
					case Ability_Random_1_2:
					TRAINER_WITH_EV_GIVE_RANDOM_ABILITY:
						GiveMonNatureAndAbility(mon, spread->nature, 0xFF, FALSE, TRUE, FALSE);
						mon->hiddenAbility = FALSE; //Set by setting abilityNum to 0xFF (which is done to save time since Ability doesn't matter)
						break;
					case Ability_RandomAll: ;
						u8 random = Random() % 3;

						if (random == 2)
							goto TRAINER_WITH_EV_GIVE_HIDDEN_ABILITY;

						goto TRAINER_WITH_EV_GIVE_RANDOM_ABILITY;
				}

				#ifdef UNBOUND
				TryGiveSpecialTrainerHiddenPower(trainerId, mon);
				#endif
			}
			#endif

			#ifdef VAR_GAME_DIFFICULTY
			extern bool8 ShouldGiveTrainerMonBestStatsMaxEVs(u8 trainerClass);
			extern bool8 ShouldGiveTrainerMonMaxFriendship(u8 trainerClass);
			if (ShouldGiveTrainerMonBestStatsMaxEVs(trainer->trainerClass))
			{
				if (GetMonEVCount(mon) == 0) //Has no EVs already
					GiveMon2BestBaseStatEVs(mon);
			}

			if (ShouldGiveTrainerMonMaxFriendship(trainer->trainerClass))
			{
				if (MoveInMonMoveset(MOVE_FRUSTRATION, mon))
					mon->friendship = 0; //Give Frustration max power
				else
					mon->friendship = 255; //Max friendship
			}
			#endif

			//Fix Minior
			if (IsMinior(mon->species))
			{
				u16 correctMiniorForm = GetMiniorCoreFromPersonality(mon->personality);
				SetMonData(mon, MON_DATA_SPECIES, &correctMiniorForm); //Prevents problems with it changing forms after lowering its shields
			}

			//Caluate stats and set to full health
			CalculateMonStatsNew(mon);
			HealMon(mon);

			//Status Inducers
			TryStatusInducer(mon);
			#ifdef UNBOUND
			TryGiveSpecialTrainerStatusCondition(trainerId, mon);
			#endif

			//Fix Partner Met Locations
			if (side == B_SIDE_PLAYER) //Partner
			{
				u8 metLoc = 0; //Unknown location
				SetMonData(&gPlayerParty[i + 3], MON_DATA_MET_LOCATION, &metLoc); //So they don't the current area
			}
		}

		//Set Double battle type if necessary
		if (trainer->doubleBattle
		#ifdef FLAG_DOUBLE_BATTLE
		|| FlagGet(FLAG_DOUBLE_BATTLE)
		#endif
		)
		{
			#ifdef OPEN_WORLD_TRAINERS
			if ((GetOpenWorldTrainerMonAmount() > 1 || trainer->doubleBattle)
			#else
			if (trainer->partySize > 1
			#endif
			&& ViableMonCount(gPlayerParty) >= 2) //Double battles will not happen if the player only has 1 mon that can fight or if the foe only has 1 mon
			{
				gBattleTypeFlags |= BATTLE_TYPE_DOUBLE;
			}
		}
	}
	else
		monsCount = 1;

	return monsCount;
}

static u8 GetTrainerMonGender(struct Trainer* trainer)
{
	switch (trainer->trainerClass)
	{
		case CLASS_TEAM_ROCKET: //Otherwise all Grunts have the same genders
		case CLASS_LEADER:
		case CLASS_ELITE_4:
		case CLASS_CHAMPION:
		case CLASS_RIVAL:
		case CLASS_RIVAL_2:
		case CLASS_BOSS:
		#ifdef UNBOUND
		case CLASS_LOR:
		case CLASS_SUCCESSOR:
		case CLASS_SHADOW_ADMIN:
		case CLASS_LOR_ADMIN:
		case CLASS_LOR_LEADER:
		case CLASS_AGENT:
		#endif
			return trainer->gender; //These Trainer classes always match the gender of the Trainer
		default:
			return 0xFF; //Randomly assign gender based on hash
	}
}

static u8 GetTrainerMonMovePPBonus(void)
{
	#ifdef VAR_GAME_DIFFICULTY
	if (VarGet(VAR_GAME_DIFFICULTY) >= OPTIONS_EXPERT_DIFFICULTY)
		return 0xFF; //Max PP
	#endif

	return 0;
}

static u8 GetTrainerMonMovePP(u16 move, u8 index)
{
	return CalculatePPWithBonus(move, GetTrainerMonMovePPBonus(), index);
}

//These next few functions are related to scaling a Trainer's team dynamically based the player's strength
#if (defined SCALED_TRAINERS && !defined DEBUG_NO_LEVEL_SCALING)
struct LevelScaler
{
	u8 minLevel;
	u8 startScalingAtLevel; //Usually highest level of next Gym Leader + 1
};

static const struct LevelScaler sLevelScales[] =
{
	[0] = {0, 19},
	[1] = {15, 26},
	[2] = {20, 32},
	[3] = {25, 36},
	[4] = {30, 45},
	[5] = {38, 54},
	[6] = {47, 58},
	[7] = {51, 63},
	[8] = {58, 70},
	[9] = {70,  0},
};

static u8 GetPlayerBiasedAverageLevel(u8 maxLevel)
{
	u32 i, sum, count;
	
	for (i = 0, sum = 0, count = 0; i < PARTY_SIZE; ++i)
	{
		u16 species = GetMonData(&gPlayerParty[i], MON_DATA_SPECIES2, NULL);
	
		if (species != SPECIES_NONE && species != SPECIES_EGG) //Viable mon
		{
			u8 level = GetMonData(&gPlayerParty[i], MON_DATA_LEVEL, NULL);
			
			if (maxLevel - level <= 5) //This level is within 5 levels of the max
			{
				sum += level;
				++count;
			}
		}
	}

	return sum / count;
}

static bool8 DoesTrainerBattleModeAllowMonEvolution(void)
{
	return sTrainerBattleMode == TRAINER_BATTLE_SINGLE_SCALED
		|| sTrainerBattleMode == TRAINER_BATTLE_DOUBLE_SCALED
		|| sTrainerBattleMode == TRAINER_BATTLE_SINGLE_NO_INTRO_TEXT_SCALED
		#ifdef UNBOUND
		|| sTrainerBattleMode == TRAINER_BATTLE_REMATCH
		|| sTrainerBattleMode == TRAINER_BATTLE_REMATCH_DOUBLE
		|| sTrainerBattleMode == TRAINER_BATTLE_REMATCH_TWO_OPPONENTS
		#endif
		;
}

static bool8 IsPseudoBossTrainerPartyForLevelScaling(u8 trainerPartyFlags)
{
	#ifdef VAR_GAME_DIFFICULTY
	if (VarGet(VAR_GAME_DIFFICULTY) == OPTIONS_EASY_DIFFICULTY)
		return FALSE; //No pseudo bosses in easy mode
	#endif

	//If the Trainer has custom moves, then they must be important
	switch (trainerPartyFlags) {
		case PARTY_FLAG_CUSTOM_MOVES:
		case PARTY_FLAG_CUSTOM_MOVES | PARTY_FLAG_HAS_ITEM:
			return TRUE;
	}

	return FALSE;
}

#endif

static bool8 IsBossTrainerClassForLevelScaling(u16 trainerId)
{
	#ifdef VAR_GAME_DIFFICULTY
	if (VarGet(VAR_GAME_DIFFICULTY) == OPTIONS_EASY_DIFFICULTY)
		return FALSE; //No bosses in easy mode
	#endif

	switch (gTrainers[trainerId].trainerClass) {
		case CLASS_LEADER:
		case CLASS_ELITE_4:
		case CLASS_CHAMPION:
		case CLASS_RIVAL_2:
		case CLASS_BOSS:
		#ifdef UNBOUND
		case CLASS_SUCCESSOR:
		case CLASS_LOR_LEADER:
		#endif
			return TRUE;
	}

	return FALSE;
}

static void ModifySpeciesAndLevelForGenericBattle(unusedArg u16* species, unusedArg u8* level, unusedArg u8 minEnemyTeamLevel, unusedArg u8 highestPlayerTeamLevel,
                                                  unusedArg u8 averagePlayerTeamLevel, unusedArg u8 trainerPartyFlags, unusedArg u8 partySize, unusedArg bool8 shouldEvolve)
{
	#if (defined SCALED_TRAINERS && !defined  DEBUG_NO_LEVEL_SCALING)
	u8 minEnemyLevel, startScalingAtLevel, prevStartScalingAtLevel, levelRange, newLevel, badgeCount, levelSubtractor;
	bool8 levelChangedForEvolution = FALSE;

	badgeCount = GetOpenWorldBadgeCount();
	minEnemyLevel = sLevelScales[badgeCount].minLevel;
	startScalingAtLevel = sLevelScales[badgeCount].startScalingAtLevel;
	prevStartScalingAtLevel = (badgeCount == 0) ? 0 : sLevelScales[badgeCount - 1].startScalingAtLevel;
	levelRange = *level - minEnemyTeamLevel; //The offset in the team
	newLevel = minEnemyLevel + levelRange;
	
	if (IsPseudoBossTrainerPartyForLevelScaling(trainerPartyFlags)
	#ifdef VAR_GAME_DIFFICULTY //Scale partners well in wild boss battles on easy
	|| (VarGet(VAR_GAME_DIFFICULTY) == OPTIONS_EASY_DIFFICULTY && (!(gBattleTypeFlags & BATTLE_TYPE_TRAINER) && FlagGet(FLAG_SCALE_WILD_BOSS_LEVEL)))
	#endif
	)
	{
		levelSubtractor = 0; //Allow pseudo bosses to be closer to the player's average level (and maybe even surpass their max)
	}
	else
	{
		#ifdef VAR_GAME_DIFFICULTY
		if  (VarGet(VAR_GAME_DIFFICULTY) >= OPTIONS_EXPERT_DIFFICULTY)
			levelSubtractor = 2; //In the hardest mode, Trainers scale closer to your average level
		else
		{
			//The more Pokemon on the enemy team the lower the levels scale (helps maintain curve)
			switch (partySize)
			{
				default:
				case 1:
					levelSubtractor = 3;
					break;
				case 2:
					levelSubtractor = 5;
					break;
				case 3:
					levelSubtractor = 6;
					break;
				case 4:
					levelSubtractor = 7;
					break;
				case 5:
				case 6:
					levelSubtractor = 8;
					break;
			}
		}

		#else
		levelSubtractor = 6;
		#endif
	}

	if (newLevel > *level) //Trainer is weaker than they should be based on badge count
	{
		*level = MathMin(newLevel, MAX_LEVEL);
		levelChangedForEvolution = TRUE;
	}

	if (highestPlayerTeamLevel >= startScalingAtLevel //Strongest is stronger than Gym Leader would be normally
	|| IsPseudoBossTrainerPartyForLevelScaling(trainerPartyFlags) //Fighting a pseudo boss on Difficult
	#ifdef VAR_GAME_DIFFICULTY
	|| VarGet(VAR_GAME_DIFFICULTY) >= OPTIONS_HARD_DIFFICULTY //Or the game is on on a harder setting
	#endif
	)
	{
		//So scale normal enemies based on the average team level
		newLevel = averagePlayerTeamLevel + levelRange; //Highest is used to enter into this section, but average is used to calculate the new level. Ensures better consistency while not scaling the enemies too fast if you want to train your whole team
		if (newLevel >= levelSubtractor)
			newLevel -= levelSubtractor;
		else
			newLevel = 1;
		newLevel = MathMin(newLevel, MAX_LEVEL);
		if (*level < newLevel)
		{
			*level = newLevel;
			if (IsPseudoBossTrainerPartyForLevelScaling(trainerPartyFlags)
			#ifdef VAR_GAME_DIFFICULTY
			|| VarGet(VAR_GAME_DIFFICULTY) >= OPTIONS_EXPERT_DIFFICULTY //Or the game is on on a harder setting
			#endif
			)
				levelChangedForEvolution = TRUE; //Always evolve Pseudobosses or regular trainers on Insane
		}
	}
	else if (highestPlayerTeamLevel >= prevStartScalingAtLevel) //Team is stronger than prev Gym Leader
	{
		//So scale normal enemies based on the previous Gym's start scaling level
		//This section is most likely never going to be used
		newLevel = MathMin(prevStartScalingAtLevel + levelRange - levelSubtractor, MAX_LEVEL);
		if (prevStartScalingAtLevel > 0 && *level < newLevel)
			*level = newLevel;
	}

	if (levelChangedForEvolution && shouldEvolve)
		EvolveSpeciesByLevel(species, *level);
	#endif
}

static void ModifySpeciesAndLevelForBossBattle(unusedArg u16* species, unusedArg u8* level, unusedArg u8 maxEnemyTeamLevel, unusedArg u8 maxPlayerTeamLevel, unusedArg bool8 canEvolve)
{
	#if (defined SCALED_TRAINERS && !defined  DEBUG_NO_LEVEL_SCALING)
	u8 levelRange, newLevel;

	if (maxPlayerTeamLevel >= MAX_LEVEL)
		newLevel = maxPlayerTeamLevel; //Just give full team of level 100 since AI can't overlevel
	else
	{
		levelRange = maxEnemyTeamLevel - *level; //The offset in the team from the strongest mon
		newLevel = (maxPlayerTeamLevel - levelRange) + 1; //Boss battles always have a Pokemon 1 level higher than the player's strongest mon
	}

	if (*level < newLevel)
	{
		*level = newLevel;
		if (canEvolve)
			EvolveSpeciesByLevel(species, *level);
	}
	#endif
}

u8 GetScaledWildBossLevel(u8 level)
{
	#if (defined SCALED_TRAINERS && !defined DEBUG_NO_LEVEL_SCALING)
	u8 newLevel;

	#ifdef VAR_GAME_DIFFICULTY
	if (VarGet(VAR_GAME_DIFFICULTY) == OPTIONS_EASY_DIFFICULTY)
	{
		newLevel = GetPlayerBiasedAverageLevel(GetHighestMonLevel(gPlayerParty));
		if (newLevel >= 3)
			newLevel -= 2; //2 levels below biased average level
		else
			newLevel = 1;
	}
	else
	#endif
	{
		//Scale directly to biased average team level + 1 - allows chance of being stronger than team if all the same level
		newLevel = GetPlayerBiasedAverageLevel(GetHighestMonLevel(gPlayerParty)) + 1;
	}

	if (level < newLevel)
		level = newLevel;

	if (level > MAX_LEVEL)
		level = MAX_LEVEL;

	#ifdef FLAG_HARD_LEVEL_CAP
	u8 levelCap;
	if (FlagGet(FLAG_HARD_LEVEL_CAP)
	&& !CantCatchBecauseFlag() //Catchable mon
	&& level >= (levelCap = GetCurrentLevelCap()))
		level = levelCap; //Make sure catchable boss is within level cap
	#endif

	#endif

	return level;
}

#ifdef VAR_GAME_DIFFICULTY
static void GiveMon2BestBaseStatEVs(struct Pokemon* mon)
{
	//Assign random Trainers max EVs in their two best base stats
	u8 statId, bestStat1, bestStat2;
	u16 species = GetMonData(mon, MON_DATA_SPECIES, NULL);

	//Set initial best stats
	if (GetVisualBaseStat(STAT_HP, species) > GetVisualBaseStat(STAT_ATK, species))
	{
		bestStat1 = STAT_HP;
		bestStat2 = STAT_ATK;
	}
	else
	{
		bestStat1 = STAT_ATK;
		bestStat2 = STAT_HP;
	}

	//Find best of rest of stats
	for (statId = STAT_DEF; statId <= STAT_SPDEF; ++statId)
	{
		u16 stat = GetVisualBaseStat(statId, species);

		if (stat > GetVisualBaseStat(bestStat1, species))
		{
			bestStat2 = bestStat1; //Shift down
			bestStat1 = statId; //New best stat
		}
		else if (stat > GetVisualBaseStat(bestStat2, species))
		{
			bestStat2 = statId; //New second best stat
		}
	}

	u8 maxEV = EV_CAP;
	SetMonData(mon, MON_DATA_HP_EV + bestStat1, &maxEV);
	SetMonData(mon, MON_DATA_HP_EV + bestStat2, &maxEV);
}
#endif

static u16 TryReplaceNormalTrainerSpecies(u16 species, unusedArg u16 trainerId)
{
	#ifdef UNBOUND
	extern u16 TryReplaceUnboundNormalTrainerSpecies(u16 species, u16 trainerId);
	return TryReplaceUnboundNormalTrainerSpecies(species, trainerId);
	#else
	return species;
	#endif
}

//Returns the number of Pokemon
static u8 BuildFrontierParty(struct Pokemon* const party, const u16 trainerId, const u8 tier, const bool8 firstTrainer, const bool8 forPlayer, const u8 side)
{
	u32 i;
	u8 monsCount;
	u8 rand;

	u8 trainerGender = 0;
	u8 battleTowerPokeNum = VarGet(VAR_BATTLE_FACILITY_POKE_NUM);
	u8 battleType = VarGet(VAR_BATTLE_FACILITY_BATTLE_TYPE);
	u8 level = GetBattleFacilityLevel(tier);
	u16 tableId = VarGet(VAR_FACILITY_TRAINER_ID + (firstTrainer ^ 1));

	if (!forPlayer)
	{
		if (trainerId == TRAINER_SECRET_BASE)
			return 0;
		else if (!IsFrontierTrainerId(trainerId))
			return CreateNPCTrainerParty(party, trainerId, firstTrainer, side);
	}

	//Two of the three variables here hold garbage data which is never called.
	const struct BattleTowerTrainer* trainer = &gTowerTrainers[tableId];
	const struct SpecialBattleFrontierTrainer* specialTrainer = (trainerId == FRONTIER_BRAIN_TID) ? &gFrontierBrains[tableId] : &gSpecialTowerTrainers[tableId];
	const struct MultiBattleTowerTrainer* multiPartner = &gFrontierMultiBattleTrainers[tableId];

	switch (trainerId) {
		case BATTLE_TOWER_TID:
			trainerGender = trainer->gender;
			break;
		case BATTLE_TOWER_SPECIAL_TID:
		case FRONTIER_BRAIN_TID:
			trainerGender = specialTrainer->gender;
			break;
		case BATTLE_FACILITY_MULTI_TRAINER_TID:
			trainerGender = multiPartner->gender;
			break;
	}

	if (forPlayer)
		ZeroPlayerPartyMons();
	else if (firstTrainer && firstTrainer != 3) //Clear if not multi partner
		ZeroEnemyPartyMons();

	if (forPlayer == TRUE) //Excludes random battles
		monsCount = PARTY_SIZE;
	else if (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS && battleTowerPokeNum > 3 && side == B_SIDE_OPPONENT)
		monsCount = 3;
	else if (gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER && battleTowerPokeNum > 3 && side == B_SIDE_PLAYER)
		monsCount = 3;
	else
		monsCount = MathMax(1, MathMin(PARTY_SIZE, battleTowerPokeNum));

	if (IsFrontierMulti(battleType) && trainerId == FRONTIER_BRAIN_TID)
		monsCount = MathMin(PARTY_SIZE, monsCount * 2); //Frontier brains fight alone so double the amount of pokes they can use in Multi battles

	struct TeamBuilder* builder = Calloc(sizeof(struct TeamBuilder));
	builder->tier = tier;
	builder->battleType = battleType;
	builder->monsCount = monsCount;
	builder->trainerId = trainerId;
	Memset(builder->partyIndex, 0xFF, sizeof(builder->partyIndex));

	if (!forPlayer
	&& !IsRandomBattleTowerBattle() //Player has a random team so ignore it
	&& CareAboutTeamWeaknessesInTier(tier) //Don't waste time unless this is relevant
	&& !IsFrontierMulti(battleType)) //Can't do this in Multi battles because player partner's team is not in yet
		AddPlayerMoveTypesToBuilder(builder, monsCount);

	for (i = 0; i < monsCount; ++i)
	{
		bool8 loop = TRUE;
		u16 species, dexNum, item;
		u8 ability, itemEffect;
		const struct BattleTowerSpread* spread = NULL;

		do
		{
			switch (trainerId) {
				case BATTLE_TOWER_SPECIAL_TID:
				case FRONTIER_BRAIN_TID:
					if (tier == BATTLE_FACILITY_MEGA_BRAWL)
					{	//Force these trainers to have at least X amount of Mega Pokemon
						if (monsCount < PARTY_SIZE)
						{
							if (builder->numMegas < 2 && i + (2 - builder->numMegas) >= monsCount)
								goto GENERIC_RANDOM_SPREADS; //Force at least two megas
						}
						else //6v6
						{
							if (builder->numMegas < 3 && i + (3 - builder->numMegas) >= monsCount)
								goto GENERIC_RANDOM_SPREADS; //Force at least three megas
						}
					}

					switch (tier) {
						case BATTLE_FACILITY_UBER:
						case BATTLE_FACILITY_NO_RESTRICTIONS:
						case BATTLE_FACILITY_UBER_CAMOMONS:
						SPECIAL_TRAINER_LEGENDARY_SPREADS:
							if (specialTrainer->legendarySpreads != NULL)
								spread = &specialTrainer->legendarySpreads[Random() % specialTrainer->legSpreadSize];
							else
								goto REGULAR_UBERS_SPREADS;
							break;
						case BATTLE_FACILITY_LITTLE_CUP:
						case BATTLE_FACILITY_LC_CAMOMONS:
						SPECIAL_TRAINER_LITTLE_SPREADS:
							if (specialTrainer->littleCupSpreads != NULL)
								spread = &specialTrainer->littleCupSpreads[Random() % specialTrainer->lcSpreadSize];
							else
								goto REGULAR_LC_SPREADS;
							break;
						case BATTLE_FACILITY_MIDDLE_CUP:
						case BATTLE_FACILITY_MC_CAMOMONS:
							if (IsFrontierSingles(battleType))
							{
								SPECIAL_TRAINER_MIDDLE_SPREADS:
								if (specialTrainer->middleCupSpreads != NULL)
									spread = &specialTrainer->middleCupSpreads[Random() % specialTrainer->mcSpreadSize];
								else
									goto REGULAR_MC_SPREADS;
							}
							else //Doubles - GS Cup
							{
								if ((Random() & 1) == 0) //50% chance of pulling legendary
									goto SPECIAL_TRAINER_LEGENDARY_SPREADS;

								goto SPECIAL_TRAINER_REGULAR_SPREADS;
							}
							break;
						case BATTLE_FACILITY_SCALEMONS: ;
							if (trainerId == FRONTIER_BRAIN_TID && BATTLE_FACILITY_NUM == IN_BATTLE_MINE)
								goto SPECIAL_TRAINER_LITTLE_SPREADS;

							rand = Random() & 7;
							switch (rand) {
								case 0: //High prevalence of baby spreads b/c they
								case 1: //get insane stats in Scalemons
								case 2:
								case 3:
									goto SPECIAL_TRAINER_LITTLE_SPREADS;
								case 4:
								case 5:
									goto SPECIAL_TRAINER_MIDDLE_SPREADS;
							}
							goto SPECIAL_TRAINER_REGULAR_SPREADS;
						case BATTLE_FACILITY_350_CUP: ;
						SPECIAL_TRAINER_350_SPREADS:
							if (trainerId == FRONTIER_BRAIN_TID && BATTLE_FACILITY_NUM == IN_BATTLE_MINE)
								goto SPECIAL_TRAINER_LITTLE_SPREADS;

							rand = Random() & 3;
							switch (rand) {
								case 0:
								case 1:
									if (specialTrainer->littleCupSpreads != NULL)
										spread = &specialTrainer->littleCupSpreads[Random() % specialTrainer->lcSpreadSize];
									else
										spread = &gLittleCupSpreads[Random() % TOTAL_LITTLE_CUP_SPREADS];

									u16 bst = GetBaseStatsTotal(spread->species);
									if (bst > 350 || bst < 250)
										goto SPECIAL_TRAINER_350_SPREADS; //Reroll if doesn't have viable stats
									break;
								case 2:
									goto SPECIAL_TRAINER_LEGENDARY_SPREADS;
								default:
									goto SPECIAL_TRAINER_REGULAR_SPREADS;
							}
							break;
						case BATTLE_FACILITY_AVERAGE_MONS: ;
							if (trainerId == FRONTIER_BRAIN_TID && BATTLE_FACILITY_NUM == IN_BATTLE_MINE)
								goto SPECIAL_TRAINER_LITTLE_SPREADS;

							rand = Random() & 3;
							switch (rand) {
								case 0:
									goto SPECIAL_TRAINER_LITTLE_SPREADS;
								case 1:
									goto SPECIAL_TRAINER_MIDDLE_SPREADS;
								case 2:
									goto SPECIAL_TRAINER_LEGENDARY_SPREADS;
							}
							goto SPECIAL_TRAINER_REGULAR_SPREADS;
						default:
						SPECIAL_TRAINER_REGULAR_SPREADS:
							if (specialTrainer->regularSpreads != NULL)
								spread = &specialTrainer->regularSpreads[Random() % specialTrainer->regSpreadSize]; //Special trainers have preset spreads.
							else
								goto REGULAR_SPREADS;
					}
					break;
				case BATTLE_FACILITY_MULTI_TRAINER_TID: //This will only get called if the Player's party was not set up properly beforehand
					if (IsRandomBattleTowerBattle())
						goto GENERIC_RANDOM_SPREADS; //Generate random Pokemon for the partner in random battles

					switch (tier) {
						case BATTLE_FACILITY_UBER:
						case BATTLE_FACILITY_NO_RESTRICTIONS:
						case BATTLE_FACILITY_UBER_CAMOMONS:
						MULTI_PARTNER_LEGENDARY_SPREADS:
							if (multiPartner->legendarySpreads != NULL)
								spread = &multiPartner->legendarySpreads[Random() % multiPartner->legSpreadSize];
							else
								goto REGULAR_UBERS_SPREADS;
							break;
						case BATTLE_FACILITY_LITTLE_CUP:
						case BATTLE_FACILITY_LC_CAMOMONS:
						MULTI_PARTNER_LITTLE_SPREADS:
							if (multiPartner->littleCupSpreads != NULL)
								spread = &multiPartner->littleCupSpreads[Random() % multiPartner->lcSpreadSize];
							else
								goto REGULAR_LC_SPREADS;
							break;
						case BATTLE_FACILITY_MIDDLE_CUP:
						case BATTLE_FACILITY_MC_CAMOMONS:
							if (IsFrontierSingles(battleType))
							{
								goto REGULAR_MC_SPREADS; //Middle Cup doesn't exist in Multi Battles so this is error prevention for something
							}
							else //Doubles - GS Cup
							{
								if ((Random() & 1) == 0) //50% chance of pulling legendary
									goto MULTI_PARTNER_LEGENDARY_SPREADS;

								goto MULTI_PARTNER_REGULAR_SPREADS;
							}
							break;
						case BATTLE_FACILITY_SCALEMONS: ;
							rand = Random() & 7;
							switch (rand) {
								case 0: //High prevalence of baby spreads b/c they
								case 1: //get insane stats in Scalemons
								case 2:
								case 3:
								case 4:
									goto MULTI_PARTNER_LITTLE_SPREADS;
							}
							goto MULTI_PARTNER_REGULAR_SPREADS;
						case BATTLE_FACILITY_350_CUP: ;
						MULTI_PARTNER_350_SPREADS:
							rand = Random() & 3;
							switch (rand) {
								case 0:
								case 1:
									if (multiPartner->littleCupSpreads != NULL)
										spread = &multiPartner->littleCupSpreads[Random() % multiPartner->lcSpreadSize];
									else
										spread = &gLittleCupSpreads[Random() % TOTAL_LITTLE_CUP_SPREADS];

									u16 bst = GetBaseStatsTotal(spread->species);
									if (bst > 350 || bst < 250)
										goto MULTI_PARTNER_350_SPREADS; //Reroll if doesn't have viable stats
									break;
								case 2:
									goto MULTI_PARTNER_LEGENDARY_SPREADS;
								default:
									goto MULTI_PARTNER_REGULAR_SPREADS;
							}
							break;
						case BATTLE_FACILITY_AVERAGE_MONS: ;
							rand = Random() & 3;
							switch (rand) {
								case 0:
								case 1:
									goto MULTI_PARTNER_LITTLE_SPREADS;
								case 2:
									goto MULTI_PARTNER_LEGENDARY_SPREADS;
							}
							goto MULTI_PARTNER_REGULAR_SPREADS;
						default:
						MULTI_PARTNER_REGULAR_SPREADS:
							if (multiPartner->regularSpreads != NULL)
								spread = &multiPartner->regularSpreads[Random() % multiPartner->regSpreadSize]; //Multi trainers have preset spreads.
							else
								goto REGULAR_SPREADS;
					}
					break;
				case BATTLE_TOWER_TID:
				GENERIC_RANDOM_SPREADS:
					switch (tier) {
						case BATTLE_FACILITY_UBER:
						case BATTLE_FACILITY_NO_RESTRICTIONS:
						case BATTLE_FACILITY_UBER_CAMOMONS:
							SPECIAL_UBERS_SPREADS:
							if (Random() % 100 < 75) //75% chance per mon of being non-legend good for Ubers (in reality a lot lower because standard spreads are much bigger)
							{
								spread = &gFrontierSpreads[Random() % TOTAL_SPREADS];
								if (!gSpecialSpeciesFlags[spread->species].goodForUbers //Not a Pokemon that would do well in Ubers
								&& !CheckTableForItem(spread->item, gSmogonOU_ItemBanList) //Doesn't have an item like Ubers Mega Stones
								&& spread->item != ITEM_MAWILITE) //Not banned in OU but still good in Ubers
								{
									continue;
								}
							}
							else
							{
								REGULAR_UBERS_SPREADS:
								spread = &gFrontierLegendarySpreads[Random() % TOTAL_LEGENDARY_SPREADS];
								if (gSpecialSpeciesFlags[spread->species].badForUbers) //Don't allow some bad Legendaries like Shaymin-Land and Necrozma-Base
									continue;
							}
							break;
						case BATTLE_FACILITY_LITTLE_CUP:
						case BATTLE_FACILITY_LC_CAMOMONS:
						REGULAR_LC_SPREADS:
							spread = &gLittleCupSpreads[Random() % TOTAL_LITTLE_CUP_SPREADS];
							break;
						case BATTLE_FACILITY_MIDDLE_CUP:
						case BATTLE_FACILITY_MC_CAMOMONS:
							if (!IsFrontierSingles(battleType)) //Doubles - GS Cup
							{
								if ((Random() & 1) == 0)
								{
									REGULAR_LEGENDARY_SPREADS: //Unlike REGULAR_UBERS_SPREADS, this includes bad legends
									spread = &gFrontierLegendarySpreads[Random() % TOTAL_LEGENDARY_SPREADS];
								}
								else
									goto REGULAR_SPREADS;
							}
							else
							{
								REGULAR_MC_SPREADS:
								spread = &gMiddleCupSpreads[Random() % TOTAL_MIDDLE_CUP_SPREADS];
							}
							break;
						case BATTLE_FACILITY_OU:
						case BATTLE_FACILITY_NATIONAL_DEX_OU:
						case BATTLE_FACILITY_MONOTYPE:
						case BATTLE_FACILITY_CAMOMONS:
						case BATTLE_FACILITY_METRONOME:
						case BATTLE_FACILITY_UU:
						case BATTLE_FACILITY_RU:
						case BATTLE_FACILITY_NU:
							//25% chance of trying to use a legend allowed in these tiers
							if ((Random() & 3) == 0)
								goto REGULAR_LEGENDARY_SPREADS;

							goto REGULAR_SPREADS;
						case BATTLE_FACILITY_SCALEMONS: ;
							rand = Random() & 7;
							switch (rand) {
								case 0: //High prevalence of baby spreads b/c they
								case 1: //get insane stats in Scalemons
								case 2:
								case 3:
									goto REGULAR_LC_SPREADS;
								case 4:
								case 5:
									goto REGULAR_MC_SPREADS;
							}
							goto REGULAR_SPREADS;
						case BATTLE_FACILITY_350_CUP: ;
						REGULAR_350_SPREADS:
							rand = Random() & 3;
							switch (rand) {
								case 0:
								case 1:
									spread = &gLittleCupSpreads[Random() % TOTAL_LITTLE_CUP_SPREADS];

									u16 bst = GetBaseStatsTotal(spread->species);
									if (bst > 350 || bst < 250)
										goto REGULAR_350_SPREADS; //Reroll if doesn't have viable stats
									break;
								case 2:
									goto REGULAR_LEGENDARY_SPREADS;
								default:
									goto REGULAR_SPREADS;
							}
							break;
						case BATTLE_FACILITY_AVERAGE_MONS: ;
							rand = Random() & 3;
							switch (rand) {
								case 0:
									goto REGULAR_LC_SPREADS;
								case 1:
									goto REGULAR_MC_SPREADS;
								case 2:
									goto REGULAR_LEGENDARY_SPREADS;
							}
							goto REGULAR_SPREADS;
						case BATTLE_FACILITY_STANDARD:
						case BATTLE_FACILITY_DYNAMAX_STANDARD:
							if (trainerId != BATTLE_TOWER_TID) //Multi partner team
								goto REGULAR_SPREADS;

							u16 streak = GetCurrentBattleTowerStreak();
							if (streak < 2)
							{
								spread = &gLittleCupSpreads[Random() % TOTAL_LITTLE_CUP_SPREADS]; //Load Little Cup spreads for first two battles to make them easier
								break;
							}
							else if (streak < 5)
							{
								spread = &gMiddleCupSpreads[Random() % TOTAL_MIDDLE_CUP_SPREADS]; //Load Middle Cup spreads for battles 3-5 to make them easier
								break;
							}
							__attribute__ ((fallthrough));
						case BATTLE_FACILITY_BENJAMIN_BUTTERFREE: //Don't use legends even though you can
						case BATTLE_FACILITY_MEGA_BRAWL:
						default:
						REGULAR_SPREADS:
							spread = &gFrontierSpreads[Random() % TOTAL_SPREADS];
							break;
					}

					spread = TryAdjustSpreadForSpecies(spread); //Update Arceus
					break;

				default: //forPlayer
					switch (tier) {
						case BATTLE_FACILITY_UBER:
						case BATTLE_FACILITY_NO_RESTRICTIONS:
						case BATTLE_FACILITY_UBER_CAMOMONS:
							goto SPECIAL_UBERS_SPREADS;
						case BATTLE_FACILITY_LITTLE_CUP:
						case BATTLE_FACILITY_LC_CAMOMONS:
							goto REGULAR_LC_SPREADS;
						case BATTLE_FACILITY_MIDDLE_CUP:
						case BATTLE_FACILITY_MC_CAMOMONS:
							if (!IsFrontierSingles(battleType)) //Doubles - GS Cup
							{
								if ((Random() & 1) == 0)
									goto REGULAR_LEGENDARY_SPREADS;

								goto REGULAR_SPREADS;
							}
							goto REGULAR_MC_SPREADS;
						case BATTLE_FACILITY_OU:
						case BATTLE_FACILITY_NATIONAL_DEX_OU:
						case BATTLE_FACILITY_MONOTYPE:
						case BATTLE_FACILITY_CAMOMONS:
						case BATTLE_FACILITY_METRONOME:
						case BATTLE_FACILITY_UU:
						case BATTLE_FACILITY_RU:
						case BATTLE_FACILITY_NU:
							//25% chance of trying to use a legend allowed in these tiers
							if ((Random() & 3) == 0)
								goto REGULAR_LEGENDARY_SPREADS;

							goto REGULAR_SPREADS;
						case BATTLE_FACILITY_SCALEMONS: ;
							rand = Random() & 7;
							switch (rand) {
								case 0: //High prevalence of baby spreads b/c they
								case 1: //get insane stats in Scalemons
								case 2:
								case 3:
									goto REGULAR_LC_SPREADS;
								case 4:
								case 5:
									goto REGULAR_MC_SPREADS;
							}
							goto REGULAR_SPREADS;
						case BATTLE_FACILITY_350_CUP: ;
							goto REGULAR_350_SPREADS;
						case BATTLE_FACILITY_AVERAGE_MONS: ;
							rand = Random() & 3;
							switch (rand) {
								case 0:
									goto REGULAR_LC_SPREADS;
								case 1:
									goto REGULAR_MC_SPREADS;
								case 2:
									goto REGULAR_LEGENDARY_SPREADS;
							}
							goto REGULAR_SPREADS;

						default:
							goto REGULAR_SPREADS;
					}
			}

			species = spread->species;
			dexNum = SpeciesToNationalPokedexNum(species);
			item = spread->item;
			ability = (gMain.inBattle && gBattleTypeFlags & BATTLE_TYPE_BATTLE_CIRCUS && gBattleCircusFlags & BATTLE_CIRCUS_ABILITY_SUPPRESSION) ? 0
					: ConvertFrontierAbilityNumToAbility(spread->ability, species);
			itemEffect = (ability == ABILITY_KLUTZ
					  || (gMain.inBattle && gBattleTypeFlags & BATTLE_TYPE_BATTLE_CIRCUS && gBattleCircusFlags & BATTLE_CIRCUS_MAGIC_ROOM)) ? 0 : ItemId_GetHoldEffect(item);

			if (IsFrontierSingles(battleType))
			{
				if (!spread->forSingles) //Certain spreads are only for double battles
					continue;
			}
			else //Double Battle
			{
				if (!spread->forDoubles) //Certain spreads are only for single battles
					continue;
			}

			if (tier == BATTLE_FACILITY_MEGA_BRAWL && !IsMegaStone(item))
			{
				//Force trainers to have at least X amount of Mega Pokemon
				if (trainerId == BATTLE_TOWER_SPECIAL_TID || trainerId == FRONTIER_BRAIN_TID)
				{
					if (monsCount < 6)
					{
						if (IsFrontierMulti(battleType) && monsCount < 3)
						{
							if (i + 1 >= monsCount && builder->numMegas == 0)
								continue; //Force at least one mega
						}
						else
						{
							if (builder->numMegas < 2 && i + (2 - builder->numMegas) >= monsCount)
								continue; //Force at least two megas
						}
					}
					else //6v6
					{
						if (builder->numMegas < 3 && i + (3 - builder->numMegas) >= monsCount)
							continue; //Force at least three megas
					}
				}
				else if (trainerId == BATTLE_TOWER_TID || forPlayer)
				{
					if (monsCount < 6)
					{
						if (i + 1 >= monsCount && builder->numMegas == 0)
							continue; //Force at least one mega
					}
					else //6v6
					{
						if (builder->numMegas < 2 && i + (2 - builder->numMegas) >= monsCount)
							continue; //Force at least two megas
					}
				}
			}

			if (ability == ABILITY_WONDERGUARD && IsRandomBattleTowerBattle())
				continue; //Don't allow Wonder Guard to appear when you have no control over the Pokemon you get

			#ifdef FLAG_GEN_8_PLACED_IN_GAME
			if (species >= SPECIES_GROOKEY && species < NUM_SPECIES_GEN_8
			&& !FlagGet(FLAG_GEN_8_PLACED_IN_GAME))
				continue; //Only allow Gen 8 if they've been unlocked
			#endif

			//Prevent duplicate species and items
			//Only allow one Mega Stone & Z-Crystal per team
			if (!IsPokemonBannedBasedOnStreak(species, item, builder->speciesArray, monsCount, trainerId, tier, forPlayer)
			&& (!builder->speciesOnTeam[dexNum] || tier == BATTLE_FACILITY_NO_RESTRICTIONS)
			&& !(ItemAlreadyOnTeam(item, monsCount, builder->itemArray) && DuplicateItemsAreBannedInTier(builder->tier, builder->battleType))
			&& (tier == BATTLE_FACILITY_MEGA_BRAWL || itemEffect != ITEM_EFFECT_MEGA_STONE || item == ITEM_ULTRANECROZIUM_Z || !builder->itemEffectOnTeam[ITEM_EFFECT_MEGA_STONE])
			&& ((itemEffect != ITEM_EFFECT_Z_CRYSTAL && item != ITEM_ULTRANECROZIUM_Z) || !builder->itemEffectOnTeam[ITEM_EFFECT_Z_CRYSTAL])
			&& !PokemonTierBan(species, item, spread, NULL, tier, CHECK_BATTLE_TOWER_SPREADS)
			&& !(tier == BATTLE_FACILITY_MONOTYPE && TeamNotAllSameType(species, item, monsCount, builder->speciesArray, builder->itemArray))
			&& !(tier == BATTLE_FACILITY_GS_CUP && !IsFrontierSingles(battleType) && TooManyLegendariesOnGSCupTeam(species, monsCount, builder->speciesArray))
			&& !((trainerId == BATTLE_TOWER_TID || forPlayer || (trainerId == BATTLE_FACILITY_MULTI_TRAINER_TID && IsRandomBattleTowerBattle())) && TeamDoesntHaveSynergy(spread, builder, forPlayer)))
			{
				loop = FALSE;
			}
		} while (loop);

		UpdateBuilderAfterSpread(builder, spread, species, ability, item, itemEffect, i);
		CreateFrontierMon(&party[i], level, spread, trainerId, firstTrainer ^ 1, trainerGender, forPlayer);
	}

	PostProcessTeam(party, builder);
	Free(builder);

	if (!forPlayer) //Probably best to put these checks somewhere else
	{
		if (IsFrontierDoubles(battleType) || IsFrontierMulti(battleType))
			gBattleTypeFlags |= BATTLE_TYPE_DOUBLE;
	}

	return monsCount;
}

static void BuildFrontierMultiParty(u8 multiId)
{
	int i;
	u8 numRegMonsOnTeam = 0;
	u8 tier = VarGet(VAR_BATTLE_FACILITY_TIER);
	u8 level = GetBattleFacilityLevel(tier);
	const struct BattleTowerSpread* spread = NULL;
	const struct MultiBattleTowerTrainer* multiPartner = &gFrontierMultiBattleTrainers[multiId];
	u8 idOnTeam[multiPartner->regSpreadSize];
	u8 metLoc = 0; //Unknown location

	//Clear Values
	for (i = 0; i < multiPartner->regSpreadSize; ++i)
		idOnTeam[i] = FALSE;

	ZeroPlayerPartyMons();

	//Build Team
	for (int i = 0; i < PARTY_SIZE; ++i)
	{
		switch (tier) {
			case BATTLE_FACILITY_UBER:
			case BATTLE_FACILITY_NO_RESTRICTIONS:
			case BATTLE_FACILITY_UBER_CAMOMONS:
				if (multiPartner->legendarySpreads != NULL && i < multiPartner->legSpreadSize)
					spread = &multiPartner->legendarySpreads[i];
				else
					return; //No Pokemon data to load
				break;
			case BATTLE_FACILITY_LITTLE_CUP:
			case BATTLE_FACILITY_LC_CAMOMONS:
				if (multiPartner->littleCupSpreads != NULL && i < multiPartner->lcSpreadSize)
					spread = &multiPartner->littleCupSpreads[i];
				else
					return; //No Pokemon data to load
				break;
			case BATTLE_FACILITY_MIDDLE_CUP:
			case BATTLE_FACILITY_MC_CAMOMONS:
				if (IsGSCupBattle()) //Middle Cup doesn't exist in Multi Battles; just GS Cup
				{
					if (i < 2 && multiPartner->legendarySpreads != NULL && i < multiPartner->legSpreadSize)
						spread = &multiPartner->legendarySpreads[i]; //Pull the first 2 Pokemon from the legendary spreads
					else
					{
						if (multiPartner->regularSpreads != NULL && numRegMonsOnTeam < multiPartner->regSpreadSize)
						{
							u8 id;
							do
							{
								id = Random() % multiPartner->regSpreadSize;
							} while (idOnTeam[id]);

							idOnTeam[id] = TRUE;
							++numRegMonsOnTeam;
							spread = &multiPartner->regularSpreads[id];
						}
						else
							return; //No Pokemon data to load
					}
				}
				break;
			default:
				if (multiPartner->regularSpreads != NULL && i < multiPartner->regSpreadSize)
				{
					#ifdef UNBOUND
					if (multiPartner->trainerClass == CLASS_RIVAL && i >= 4)
					{
						#define VAR_RIVAL_CHOSEN_STARTER 0x5012
						extern const struct BattleTowerSpread gMultiTowerSpread_RivalV2[];
						extern const struct BattleTowerSpread gMultiTowerSpread_RivalV3[];
						switch (VarGet(VAR_RIVAL_CHOSEN_STARTER)) {
							case 2: //Chose Larvitar
								spread = &gMultiTowerSpread_RivalV3[i - 4]; //Rival has Metagross & Haxorus
								break;
							case 3: //Chose Beldum
								spread = &multiPartner->regularSpreads[i]; //Rival has Garchomp & Aggron
								break;
							default: //Chose Gible
								spread = &gMultiTowerSpread_RivalV2[i - 4]; //Rival has Tyranitar & Haxorus
						}
					}
					else
					#endif
						spread = &multiPartner->regularSpreads[i];
				}
				else
					return; //No Pokemon data to load
		}

		CreateFrontierMon(&gPlayerParty[i], level, spread, BATTLE_FACILITY_MULTI_TRAINER_TID, 2, multiPartner->gender, FALSE);
		SetMonData(&gPlayerParty[i], MON_DATA_MET_LOCATION, &metLoc); //So they don't say "Battle Frontier"
	}

	TryShuffleMovesForCamomons(gPlayerParty, tier, BATTLE_FACILITY_MULTI_TRAINER_TID);
}

const struct BattleTowerSpread* GetRaidMultiSpread(u8 multiId, u8 index, u8 numStars)
{
	const struct BattleTowerSpread* spread;
	const struct MultiRaidTrainer* multiPartner = &gRaidPartners[multiId];

	#ifdef UNBOUND
	if (index == 0 && multiPartner->trainerClass == CLASS_RIVAL)
	{
		switch (VarGet(VAR_RIVAL_CHOSEN_STARTER)) {
			case 2: //Chose Larvitar
				spread = &multiPartner->spreads[numStars][4]; //Rival has Metagross
				break;
			case 3: //Chose Beldum
				spread = &multiPartner->spreads[numStars][0]; //Rival has Garchomp
				break;
			default: //Chose Gible
				spread = &multiPartner->spreads[numStars][3]; //Rival has Tyranitar
		}
	}
	else
	#endif
		spread = &multiPartner->spreads[numStars][index];

	return spread;
}

static void BuildRaidMultiParty(void)
{
	int i;
	u8 metLoc = 0;
	u8 numStars = gRaidBattleStars;
	u8 multiId = VarGet(VAR_FACILITY_TRAINER_ID_PARTNER);

	//Build Team
	for (i = 0; i < PARTY_SIZE / 2 && i < gRaidPartners[multiId].spreadSizes[numStars]; ++i)
	{
		const struct BattleTowerSpread* spread = GetRaidMultiSpread(multiId, i, numStars);
		CreateFrontierMon(&gPlayerParty[i + 3], GetRandomRaidLevel(), spread, RAID_BATTLE_MULTI_TRAINER_TID, 2, gRaidPartners[multiId].gender, FALSE);
		SetMonData(&gPlayerParty[i + 3], MON_DATA_MET_LOCATION, &metLoc); //So they don't say "Battle Frontier"

		if (!FlagGet(FLAG_BATTLE_FACILITY)
		#ifdef FLAG_POKEMON_RANDOMIZER
		&& FlagGet(FLAG_POKEMON_RANDOMIZER) //Don't set custom moves when species has been randomized
		#endif
		#ifdef FLAG_TEMP_DISABLE_RANDOMIZER
		&& !FlagGet(FLAG_TEMP_DISABLE_RANDOMIZER) //Unless the species has been temporarily disabled
		#endif
		)
		{
			Memset(gPlayerParty[i + 3].moves, 0, sizeof(gPlayerParty[i + 3].moves));
			GiveBoxMonInitialMoveset((void*) &gPlayerParty[i + 3]); //Give the randomized Pokemon moves it would normally have
		}
	}
}

static void CreateFrontierMon(struct Pokemon* mon, const u8 level, const struct BattleTowerSpread* spread, const u16 trainerId, const u8 trainerNum, const u8 trainerGender, const bool8 forPlayer)
{
	int i, j;
	u16 species = spread->species;
	u32 otId;

	#ifdef FLAG_POKEMON_RANDOMIZER
	if (!FlagGet(FLAG_POKEMON_RANDOMIZER) //Don't try to customize a species that'll change anyway
	|| FlagGet(FLAG_BATTLE_FACILITY)
	#ifdef FLAG_TEMP_DISABLE_RANDOMIZER
	|| FlagGet(FLAG_TEMP_DISABLE_RANDOMIZER)
	#endif
	)
	#endif
		species = TryAdjustAestheticSpecies(species);

	if (trainerId == BATTLE_FACILITY_MULTI_TRAINER_TID)
		otId = gFrontierMultiBattleTrainers[VarGet(VAR_FACILITY_TRAINER_ID_PARTNER)].otId;
	else if (trainerId == RAID_BATTLE_MULTI_TRAINER_TID)
		otId = gRaidPartners[VarGet(VAR_FACILITY_TRAINER_ID_PARTNER)].otId;
	else
		otId = Random32();

	if (forPlayer)
	{
		CreateMon(mon, species, level, 0, TRUE, 0, OT_ID_PLAYER_ID, 0);
	}
	else
	{
		CreateMon(mon, species, level, 0, TRUE, 0, OT_ID_PRESET, otId);

		CopyFrontierTrainerName(gStringVar1, trainerId, trainerNum);
		SetMonData(mon, MON_DATA_OT_NAME, gStringVar1);
		SetMonData(mon, MON_DATA_OT_GENDER, &trainerGender);
	}

/*
	#ifdef UNBOUND
		mon->metLocation = MAPSEC_BATTLE_FRONTIER;
	#else
		mon->metLocation = MAPSEC_TRAINER_TOWER;
	#endif
*/
	mon->metLevel = level;
	mon->obedient = TRUE;
	mon->friendship = 255;

	i = 0;
	struct Pokemon* party = mon;

	SET_IVS(spread);
	SET_EVS(spread);

	if (spread->ability > FRONTIER_ABILITY_HIDDEN)
	{
		GiveMonNatureAndAbility(mon, spread->nature, spread->ability - 1, spread->shiny, FALSE, FALSE);
	}
	else //Hidden Ability
	{
		GiveMonNatureAndAbility(mon, spread->nature, 0xFF, spread->shiny, FALSE, FALSE);
	}

	for (j = 0; j < MAX_MON_MOVES; j++)
	{
		mon->moves[j] = spread->moves[j];
		if (mon->moves[j] != MOVE_NONE)
			mon->ppBonuses |= (3 << (j * 2)); //Max PP
		mon->pp[j] = CalculatePPWithBonus(spread->moves[j], mon->ppBonuses, j);
	}

	SetMonData(mon, MON_DATA_HELD_ITEM, &spread->item);

	u8 ballType;
	if (spread->ball != BALL_TYPE_RANDOM)
		ballType = MathMin(LAST_BALL_INDEX, spread->ball);
	else
		ballType = umodsi(Random(), NUM_BALLS);
	SetMonData(mon, MON_DATA_POKEBALL, &ballType);

	if (spread->gigantamax)
		mon->gigantamax = TRUE;

	TryFormRevert(mon); //To fix Minior forms
	CalculateMonStatsNew(mon);
	HealMon(mon);
}

static void TryFixMiniorForm(struct Pokemon* mon)
{
	u16 species = GetMonData(mon, MON_DATA_SPECIES2, NULL);
	if (species != SPECIES_NONE && species != SPECIES_EGG)
	{
		u16 dexNum = SpeciesToNationalPokedexNum(species);
		if (dexNum == NATIONAL_DEX_MINIOR)
			TryFormRevert(mon); //To fix form
	}
}

u16 GenerateWildMonHeldItem(u16 species, u8 bonus)
{
	u16 rnd = Random() % 100;
	u16 var1 = 45;
	u16 var2 = 95;

	if (gBaseStats[species].item1 == gBaseStats[species].item2 && gBaseStats[species].item1 != ITEM_NONE)
		return gBaseStats[species].item1; //100% chance

	if (!GetMonData(&gPlayerParty[0], MON_DATA_IS_EGG, NULL)
	&& AbilityIncreasesWildItemChance(GetMonAbility(&gPlayerParty[0]))) //Increased chance of finding an item
	{
		var1 = 20;
		var2 = 80;
	}

	if (bonus < var1)
		var1 -= bonus;
	else
		var1 = 0; //Guaranteed at least one item

	if (bonus < var2)
		var2 -= bonus;
	else
		var2 = 0; //Guaranteed the second item

	if (rnd < var1)
		return ITEM_NONE;

	if (rnd < var2)
		return gBaseStats[species].item1;

	return gBaseStats[species].item2;
}

void SetWildMonHeldItem(void)
{
	if (!(gBattleTypeFlags & (BATTLE_TYPE_POKE_DUDE | BATTLE_TYPE_SCRIPTED_WILD_1 | BATTLE_TYPE_TRAINER))
	&& !gDexNavStartedBattle) //Items would be set earlier
	{
		for (u8 i = 0; i < 2; ++i) //Two possible wild opponents
		{
			if (i > 0 && !IS_DOUBLE_BATTLE)
				break;

			u16 species = GetMonData(&gEnemyParty[i], MON_DATA_SPECIES, NULL);
			u16 item = GenerateWildMonHeldItem(species, 0);
			
			if (item != ITEM_NONE)
				SetMonData(&gEnemyParty[i], MON_DATA_HELD_ITEM, &item);
		}
	}
}

void GiveMonNatureAndAbility(struct Pokemon* mon, u8 nature, u8 abilityNum, bool8 forceShiny, bool8 keepGender, bool8 keepLetterCore)
{
	u32 personality = GetMonData(mon, MON_DATA_PERSONALITY, NULL);
	u16 species  = GetMonData(mon, MON_DATA_SPECIES, NULL);
	u32 otId = GetMonData(mon, MON_DATA_OT_ID, NULL);
	u16 sid = HIHALF(otId);
	u16 tid = LOHALF(otId);
	u8 gender = GetGenderFromSpeciesAndPersonality(species, personality);
	u8 letter = GetUnownLetterFromPersonality(personality);
	bool8 isMinior = IsMinior(species);
	u8 miniorCore = GetMiniorCoreFromPersonality(personality);

	if (abilityNum == 0xFF) //Hidden Ability
		mon->hiddenAbility = TRUE;
	else
		abilityNum = MathMin(1, abilityNum); //Either 0 or 1

	do
	{
		personality = Random32();
		if (forceShiny)
		{
			u8 shinyRange = Random() % SHINY_ODDS;
			personality = (((shinyRange ^ (sid ^ tid)) ^ LOHALF(personality)) << 16) | LOHALF(personality);
		}

		if (abilityNum != 0xFF)
		{
			personality &= ~(1);
			personality |= abilityNum; 
		}
	} while (GetNatureFromPersonality(personality) != nature
	|| (keepGender && GetGenderFromSpeciesAndPersonality(species, personality) != gender)
	|| (!forceShiny && IsShinyOtIdPersonality(otId, personality)) //Prevent NPCs from accidentally getting shinies
	|| (keepLetterCore && species == SPECIES_UNOWN && GetUnownLetterFromPersonality(personality) != letter) //Make sure the Unown letter doesn't change
	|| (keepLetterCore && isMinior && GetMiniorCoreFromPersonality(personality) != miniorCore)); //Make sure the Minior core doesn't change

	mon->personality = personality;
}

void GiveMonXPerfectIVs(struct Pokemon* mon, u8 totalPerfectStats)
{
	//Pick potential ivs to set to 31
	u8 i, numPerfectStats;
	u8 perfect = 31;
	bool8 perfectStats[NUM_STATS] = {0};

	for (i = 0, numPerfectStats = 0; i < NUM_STATS; ++i) //Count how many stats are already perfect
	{
		if (GetMonData(mon, MON_DATA_HP_IV + i, NULL) >= 31)
		{
			perfectStats[i] = TRUE;
			++numPerfectStats;
		}
	}

	while (numPerfectStats < totalPerfectStats) //Assign the rest of the prefect stats
	{
		u8 statId = Random() % NUM_STATS;
		if (!perfectStats[statId]) //Must be unique
		{
			perfectStats[statId] = TRUE;
			++numPerfectStats;
			SetMonData(mon, MON_DATA_HP_IV + statId, &perfect);
		}
	}
}

static u8 ConvertFrontierAbilityNumToAbility(const u8 abilityNum, const u16 species)
{
	u8 ability = ABILITY_NONE;

	switch (abilityNum) {
		case FRONTIER_ABILITY_1:
			ability = GetAbility1(species);
			break;
		case FRONTIER_ABILITY_2:
			ability = GetAbility2(species);
			break;
		case FRONTIER_ABILITY_HIDDEN:
			ability = GetHiddenAbility(species);
			break;
	}

	if (ability == ABILITY_NONE)
		ability = GetAbility1(species);

	return ability;
}

static bool8 BaseStatsTotalGEAlreadyOnTeam(const u16 toCheck, const u8 partySize, u16* speciesArray)
{
	for (int i = 0; i < partySize; ++i)
	{
		if (GetBaseStatsTotal(speciesArray[i]) >= toCheck)
			return TRUE;
	}

	return FALSE;
}

static bool8 SpeciesAlreadyOnTeam(const u16 species, const u8 partySize, const species_t* const speciesArray)
{
	for (int i = 0; i < partySize; ++i)
	{
		if (SpeciesToNationalPokedexNum(speciesArray[i]) == SpeciesToNationalPokedexNum(species))
			return TRUE;
	}

	return FALSE;
}

static bool8 ItemAlreadyOnTeam(const u16 item, const u8 partySize, const item_t* const itemArray)
{
	if (item == ITEM_NONE) return FALSE;

	for (int i = 0; i < partySize; ++i)
	{
		if (itemArray[i] == item)
			return TRUE;
	}

	return FALSE;
}

static void AdjustTypesForMegas(const u16 species, const u16 item, u8* const type1, u8* const type2)
{
	u16 megaSpecies = GetMegaSpecies(species, item, NULL);
	if (megaSpecies != SPECIES_NONE)
	{
		u8 megaType1 = gBaseStats[megaSpecies].type1;
		u8 megaType2 = gBaseStats[megaSpecies].type2;

		//Remove any types not shared with Mega
		if (*type1 != megaType1
		&&  *type1 != megaType2)
			*type1 = TYPE_BLANK;

		if (*type2 != megaType1
		&&  *type2 != megaType2)
			*type2 = TYPE_BLANK;
	}
}

//Assumes speciesArray only has Pokemon of the same type
static bool8 TeamNotAllSameType(const u16 species, const u16 item, const u8 partySize, const species_t* const speciesArray, const item_t* const itemArray)
{
	u8 typeOnTeam[NUMBER_OF_MON_TYPES] = {FALSE};

	u8 type1 = gBaseStats[species].type1;
	u8 type2 = gBaseStats[species].type2;
	AdjustTypesForMegas(species, item, &type1, &type2);

	//if (type1 != TYPE_GHOST && type2 != TYPE_GHOST) //For debugging
	//	return TRUE;

	for (int i = 0; i < partySize; ++i)
	{
		if (speciesArray[i] == SPECIES_NONE)
			break;

		u8 checkType1 = gBaseStats[speciesArray[i]].type1;
		u8 checkType2 = gBaseStats[speciesArray[i]].type2;
		AdjustTypesForMegas(speciesArray[i], itemArray[i], &checkType1, &checkType2);

		if (i == 0)
		{
			typeOnTeam[checkType1] = TRUE;
			typeOnTeam[checkType2] = TRUE;
		}
		else
		{
			if (!typeOnTeam[checkType1])
				checkType1 = checkType2;
			else if (!typeOnTeam[checkType2])
				checkType2 = checkType1;

			for (int j = 0; j < NUMBER_OF_MON_TYPES; ++j)
			{
				if (typeOnTeam[j]
				&&  j != checkType1 //First Pokemon had type that second or third etc. did not,
				&&  j != checkType2) //then type no longer on team
					typeOnTeam[j] = FALSE;
			}
		}

		if ((type1 != checkType1 || type1 == TYPE_BLANK || checkType1 == TYPE_BLANK)
		&&  (type1 != checkType2 || type1 == TYPE_BLANK || checkType2 == TYPE_BLANK)
		&&  (type2 != checkType1 || type2 == TYPE_BLANK || checkType1 == TYPE_BLANK)
		&&  (type2 != checkType2 || type2 == TYPE_BLANK || checkType2 == TYPE_BLANK))
			return TRUE;
	}

	return FALSE;
}

static bool8 TooManyLegendariesOnGSCupTeam(const u16 species, const u8 partySize, const species_t* const speciesArray)
{
	u8 legendCount = 0;
	bool8 isMulti = IsFrontierMulti(VarGet(VAR_BATTLE_FACILITY_BATTLE_TYPE));

	if (!gSpecialSpeciesFlags[species].gsCupLegendaries)
		return FALSE; //Allowed normally so we don't care

	for (int i = 0; i < partySize; ++i)
	{
		if (speciesArray[i] == SPECIES_NONE)
			continue;

		if (gSpecialSpeciesFlags[speciesArray[i]].gsCupLegendaries)
			++legendCount;

		if (isMulti && legendCount >= 1) //1 legendary per multi trainer
			return TRUE;

		if (legendCount >= 2) //2 Legendaries allowed at most
			return TRUE;
	}

	return FALSE;
}

static bool8 IsPokemonBannedBasedOnStreak(u16 species, u16 item, u16* speciesArray, u8 monsCount, u16 trainerId, u8 tier, bool8 forPlayer)
{
	if (!(gBattleTypeFlags & BATTLE_TYPE_FRONTIER))
		return FALSE; //There are no streaks outside of the Frontier

	u16 streak = GetCurrentBattleTowerStreak();
	bool8 megasZMovesBannedInTier = AreMegasZMovesBannedInTier(tier) || BATTLE_FACILITY_NUM == IN_RING_CHALLENGE;

	if (!forPlayer && trainerId == BATTLE_TOWER_TID && IsStandardTier(tier))
	{
		if (megasZMovesBannedInTier
		&& (IsZCrystal(item) || IsMegaStone(item))) //Don't give the AI Pokemon with bad items
			return TRUE;

		//Battles get more difficult the higher the streak.
		if (streak < 10)
		{
			if (IsMegaStone(item)
			||  IsZCrystal(item)
			||  GetBaseStatsTotal(species) >= 520)
				return TRUE;
		}
		else if (streak < 20)
		{
			if (IsMegaStone(item)
			||  GetBaseStatsTotal(species) >= 540)
				return TRUE;
		}
		else if (streak < 30)
		{
			if (GetBaseStatsTotal(species) >= 570)
				return TRUE;
		}
		else if (streak < 50)
		{
			if (GetBaseStatsTotal(species) >= 570
			&& BaseStatsTotalGEAlreadyOnTeam(570, monsCount, speciesArray))
				return TRUE;
		}
	}
	else if ((forPlayer || trainerId == BATTLE_FACILITY_MULTI_TRAINER_TID)
	&& IsStandardTier(tier))
	{
		streak = GetMaxBattleTowerStreakForTier(tier);

		if (megasZMovesBannedInTier
		&& (IsZCrystal(item) || IsMegaStone(item))) //Don't give the player Pokemon with bad items
			return TRUE;

		//Better Pokemon are given to the player the better the streak
		if (streak < 10)
		{
			if (IsMegaStone(item)
			||  IsZCrystal(item)
			||  GetBaseStatsTotal(species) >= 540)
				return TRUE;
		}
		else if (streak < 20)
		{
			if (IsMegaStone(item)
			||  GetBaseStatsTotal(species) >= 550)
				return TRUE;
		}
		else if (streak < 30)
		{
			if (GetBaseStatsTotal(species) >= 570)
				return TRUE;
		}
	}
	else if (trainerId == BATTLE_TOWER_SPECIAL_TID)
	{
		if (megasZMovesBannedInTier && IsMegaStone(item)) //Don't give special Trainers Pokemon with bad items
			return TRUE;

		if (streak < 20 && IsStandardTier(tier))
		{
			if (IsMegaStone(item)) //Special trainers aren't allowed to Mega Evolve
				return TRUE;	   //before the player has beaten Palmer in the 20th battle.
		}
	}
	else if (trainerId == FRONTIER_BRAIN_TID)
	{
		if (BATTLE_FACILITY_NUM == IN_RING_CHALLENGE)
			return IsZCrystal(item) || IsMegaStone(item); //Don't give the frontier Brain Pokemon with bad items
	}

	return FALSE;
}

static bool8 TeamDoesntHaveSynergy(const struct BattleTowerSpread* const spread, const struct TeamBuilder* const builder, bool8 forPlayer)
{
	int i;

	u8 ability = (gMain.inBattle && gBattleTypeFlags & BATTLE_TYPE_BATTLE_CIRCUS && gBattleCircusFlags & BATTLE_CIRCUS_ABILITY_SUPPRESSION) ? 0
			   : ConvertFrontierAbilityNumToAbility(spread->ability, spread->species);
	u8 itemEffect = (ability == ABILITY_KLUTZ
				 || (gMain.inBattle && gBattleTypeFlags & BATTLE_TYPE_BATTLE_CIRCUS && gBattleCircusFlags & BATTLE_CIRCUS_MAGIC_ROOM)) ? 0 : ItemId_GetHoldEffect(spread->item);
	u8 battleType = builder->battleType;

	bool8 hasTailwinder = builder->moveOnTeam[MOVE_TAILWIND];
	bool8 hasTrickRoomer = builder->moveOnTeam[MOVE_TRICKROOM];
	bool8 hasRainSetter = builder->abilityOnTeam[ABILITY_DRIZZLE] || builder->moveOnTeam[MOVE_RAINDANCE];
	bool8 hasSunSetter = builder->abilityOnTeam[ABILITY_DROUGHT] || builder->moveOnTeam[MOVE_SUNNYDAY];
	bool8 hasSandSetter = builder->abilityOnTeam[ABILITY_SANDSTREAM] || builder->moveOnTeam[MOVE_SANDSTORM];
	bool8 hasHailSetter = builder->abilityOnTeam[ABILITY_SNOWWARNING] || builder->moveOnTeam[MOVE_HAIL];
	bool8 hasElectricTerrainSetter = builder->abilityOnTeam[ABILITY_ELECTRICSURGE] || builder->moveOnTeam[MOVE_ELECTRICTERRAIN];
	bool8 hasPsychicTerrainSetter = builder->abilityOnTeam[ABILITY_PSYCHICSURGE] || builder->moveOnTeam[MOVE_PSYCHICTERRAIN];
	bool8 hasWonderGuard = builder->abilityOnTeam[ABILITY_WONDERGUARD];
	bool8 hasJustified = builder->abilityOnTeam[ABILITY_JUSTIFIED];

	if (builder->partyIndex[BAD_ABILITY] != 0xFF && gAbilityRatings[ability] < 0)
		return TRUE; //Only stick at most 1 Pokemon with a sucky ability on a team

	if (builder->tier != BATTLE_FACILITY_MONOTYPE && hasTrickRoomer && spread->spdEv >= 20)
		return TRUE; //Don't stick a fast Pokemon on a Trick Room team

	switch (spread->specificTeamType) {
		case DOUBLES_SUN_TEAM:
			if (!hasSunSetter)
				return TRUE; //These Pokemon need a sun setter on the team to function
			break;
		case DOUBLES_SAND_TEAM:
			if (!hasSandSetter)
				return TRUE; //These Pokemon need a sand setter on the team to function
			break;
		case DOUBLES_RAIN_TEAM:
			if (!hasRainSetter)
				return TRUE; //These Pokemon need a rain setter on the team to function
			break;
		case DOUBLES_HAIL_TEAM:
			if (!hasHailSetter) //These Pokemon need a hail setter on the team to function
				return TRUE;
			break;
		case DOUBLES_ELECTRIC_TERRAIN_TEAM:
			if (!hasElectricTerrainSetter) //These Pokemon need Electric Terrain to function
				return TRUE;
			break;
		case DOUBLES_PSYCHIC_TERRAIN_TEAM:
			if (!hasPsychicTerrainSetter) //These Pokemon need Psychic Terrain to function
				return TRUE;
			break;
		case DOUBLES_TRICK_ROOM_TEAM:
			if (!hasTrickRoomer) //These Pokemon need a Trick Room setter on the team to function
				return TRUE;
			break;
		case DOUBLES_TAILWIND_TEAM:
			if (!hasTailwinder)
				return TRUE; //These Pokemon need a Tailwind setter on the team to function
			break;
		case DOUBLES_HYPER_OFFENSE_TEAM:
			break;
		case DOUBLES_JUSTIFIED_TEAM:
			if (!hasJustified)
				return TRUE; //These Pokemon need a pokemon with Justified to boost
			break;
	}

	for (i = 0; i < MAX_MON_MOVES; ++i)
	{
		if (spread->moves[i] == MOVE_TAILWIND && hasTrickRoomer)
			return TRUE;

		if (spread->moves[i] == MOVE_TRICKROOM)
		{
			if (hasTailwinder)
				return TRUE;

			if (builder->tier != BATTLE_FACILITY_MONOTYPE && builder->partyIndex[FAST_MON] != 0xFF)
				return TRUE;
		}
	}

	//Team should have max 1 weather type
	switch (ability) {
		case ABILITY_DRIZZLE:
			if (hasSunSetter || hasSandSetter || hasHailSetter)
				return TRUE;
			break;

		case ABILITY_DROUGHT:
			if (hasRainSetter || hasSandSetter || hasHailSetter)
				return TRUE;
			break;

		case ABILITY_SANDSTREAM:
			if (hasSunSetter || hasRainSetter || hasHailSetter || hasWonderGuard)
				return TRUE;
			break;

		case ABILITY_SNOWWARNING:
			if (hasSunSetter || hasRainSetter || hasSandSetter || hasWonderGuard)
				return TRUE;
			break;

		case ABILITY_WONDERGUARD:
			if (hasSandSetter || hasHailSetter) //Weather abilities like these knock of Shedinja
				return TRUE;
			break;
	}

	u8 maxWeaknesses;
	u8 class = PredictFightingStyle(spread->moves, ability, itemEffect, 0xFF);
	if (builder->monsCount <= 4)
	{
		maxWeaknesses = 1; //Don't allow duplicate weaknesses on team

		if (!forPlayer
		&& !IsFrontierMulti(battleType) //Teams are built seperately, so even though maxWeaknesses is 1, there may be up to 2 with the other team factored in
		&& gBattleTypeFlags & BATTLE_TYPE_FRONTIER) //Streaks are only in Frontier
		{
			u16 streak = GetCurrentBattleTowerStreak();

			if (IsStandardTier(builder->tier))
			{
				if (streak < 20)
					maxWeaknesses = 2; //Make teams slightly easier for early standard battles
			}
			else if (IS_SINGLE_BATTLE) //3v3 only
			{
				if (streak > 50)
					maxWeaknesses = (Random() & 3) == 0 ? 0 : 1; //Slightly more likely to be Pokemon that have no weaknesses to your team
			}
		}

		if (IS_SINGLE_BATTLE) //3v3
		{
			if (IsClassStall(class) || IsClassCleric(class))
			{
				if (builder->numStalls >= 1
				||  builder->partyIndex[CLERIC] != 0xFF)
					return TRUE; //Max one stall/cleric Pokemon per 3v3/4v4 Team
			}
			else if (IsClassBatonPass(class) || IsClassEntryHazards(class) || IsClassScreener(class))
			{
				if (builder->partyIndex[BATON_PASSER] != 0xFF
				||  builder->partyIndex[HAZARDS_SETUP] != 0xFF
				||  builder->partyIndex[SCREENER] != 0xFF)
					return TRUE; //Baton Pass/Hazards/Screener should only be used as leads in 3v3/4v4
			}
		}
		else //4v4 Double Battle
		{
			if (IsChoiceItemEffectOrAbility(itemEffect, ability) && builder->numChoiceItems >= 1)
				return TRUE; //Max one choiced Pokemon per 4v4 doubles team

			if (IsClassDoublesUtility(class) || IsClassDoublesTeamSupport(class))
			{
				if (builder->numStalls >= 2) //Stalls here refers to utility/team support
					return TRUE; //Max two utility/team support Pokemon per 4v4 Team
			}
		}
	}
	else //5-6 Pokemon
	{
		maxWeaknesses = 2; //A well-designed team has at most 2 weaknesses of each type

		if (IS_SINGLE_BATTLE)
		{
			if (IsClassStall(class))
			{
				if (builder->tier == BATTLE_FACILITY_MONOTYPE
				|| (IsMiddleCupTier(builder->tier) && IsFrontierSingles(battleType)))
				{
					//Let more stally Pokemon in Middle Cup and Monotype
					if (builder->numStalls >= 4
					|| (builder->numStalls == 3 && builder->partyIndex[CLERIC] != 0xFF))
						return TRUE; //Max two stall/one stall + one cleric Pokemon per 6v6 Team
				}
				else
				{
					if (builder->numStalls >= 2
					|| (builder->numStalls == 1 && builder->partyIndex[CLERIC] != 0xFF))
						return TRUE; //Max two stall/one stall + one cleric Pokemon per 6v6 Team
				}
			}
			else if (IsClassCleric(class))
			{
				if (builder->partyIndex[CLERIC] != 0xFF
				||  builder->numStalls >= 2)
					return TRUE; //Max one cleric Pokemon per 6v6 Team
			}
			else if (IsClassBatonPass(class))
			{
				if (builder->partyIndex[BATON_PASSER] != 0xFF)
					return TRUE; //Max one Baton Pass Pokemon per 6v6 Team
			}
			else if (IsClassEntryHazards(class))
			{
				if (builder->partyIndex[HAZARDS_SETUP] != 0xFF)
					return TRUE; //Max one hazards Pokemon per 6v6 Team
			}
			else if (IsClassScreener(class))
			{
				if (builder->partyIndex[SCREENER] != 0xFF)
					return TRUE; //Max one screener Pokemon per 6v6 Team
			}
		}
		else //Double Battle
		{
			if (IsChoiceItemEffectOrAbility(itemEffect, ability) && builder->numChoiceItems >= 2)
				return TRUE; //Max two choiced Pokemon per 6v6 doubles team

			if (IsClassDoublesUtility(class) || IsClassDoublesTeamSupport(class))
			{
				if (builder->numStalls >= 3) //Stalls here refers to utility/team support
					return TRUE; //Max three utility/team support Pokemon per 6v6 Team
			}
		}
	}

	//Confirm not too many weaknesses
	if (CareAboutTeamWeaknessesInTier(builder->tier)
	&& itemEffect != ITEM_EFFECT_WEAKNESS_POLICY) //Good to have more weaknesses for this item
	{
		u8 moveType;
		u8 defType1 = gBaseStats[spread->species].type1; //Camomons not relevant here since it'll never make it to this point
		u8 defType2 = gBaseStats[spread->species].type2;
		for (moveType = 0; moveType < NUMBER_OF_MON_TYPES; ++moveType)
		{
			if (moveType > TYPE_DARK && moveType < TYPE_FAIRY)
				moveType = TYPE_FAIRY; //Skip right to fairy

			if ((builder->moveTypeOnPlayerTeam[moveType] //If the player doesn't have this move type on their team, who cares?
			 || IsFrontierMulti(battleType) || forPlayer || IsRandomBattleTowerBattle()) //In these cases all move types are considered
			&& builder->weaknesses[moveType] >= maxWeaknesses
			&& IsSpreadWeakToType(moveType, defType1, defType2, ability))
				return TRUE; //Too many weaknesses
		}
	}

	return FALSE;
}


static void AddPlayerMoveTypesToBuilder(struct TeamBuilder* builder, u8 monsCount)
{
	u32 i, j;

	for (i = 0; i < monsCount; ++i)
	{
		u16 species = GetMonData(&gPlayerParty[i], MON_DATA_SPECIES, NULL);
		if (species != SPECIES_NONE && species != SPECIES_EGG)
		{
			for (j = 0; j < MAX_MON_MOVES; ++j)
			{
				u16 move = GetMonData(&gPlayerParty[i], MON_DATA_MOVE1 + j, NULL);

				switch (move)
				{
					case MOVE_NONE:
						break;
					case MOVE_WEATHERBALL:
						//Account for all Weather Ball types
						builder->moveTypeOnPlayerTeam[gBattleMoves[move].type] = TRUE;
						builder->moveTypeOnPlayerTeam[TYPE_FIRE] = TRUE;
						builder->moveTypeOnPlayerTeam[TYPE_WATER] = TRUE;
						builder->moveTypeOnPlayerTeam[TYPE_ROCK] = TRUE;
						builder->moveTypeOnPlayerTeam[TYPE_ICE] = TRUE;
						break;
					case MOVE_TERRAINPULSE:
						//Account for all Terrain Pulse types
						builder->moveTypeOnPlayerTeam[gBattleMoves[move].type] = TRUE;
						builder->moveTypeOnPlayerTeam[TYPE_ELECTRIC] = TRUE;
						builder->moveTypeOnPlayerTeam[TYPE_GRASS] = TRUE;
						builder->moveTypeOnPlayerTeam[TYPE_FAIRY] = TRUE;
						builder->moveTypeOnPlayerTeam[TYPE_PSYCHIC] = TRUE;
						break;
					default:
						if (SPLIT(move) != SPLIT_STATUS && !CheckTableForMovesEffect(move, gMoveEffectsThatIgnoreWeaknessResistance))
						{
							u8 moveType = GetMonMoveTypeSpecial(&gPlayerParty[i], move);
							builder->moveTypeOnPlayerTeam[moveType] = TRUE;
						}
						break;
				}
			}
		}
	}
}

static void UpdateBuilderAfterSpread(struct TeamBuilder* builder, const struct BattleTowerSpread* spread, u16 species, u8 ability, u16 item, u8 itemEffect, u32 partyId)
{
	u32 j;
	u8 class = PredictFightingStyle(spread->moves, ability, itemEffect, 0xFF);

	builder->spreads[partyId] = spread;
	builder->speciesArray[partyId] = species;
	builder->itemArray[partyId] = item;
	builder->abilityOnTeam[ability] = TRUE;

	if (item == ITEM_ULTRANECROZIUM_Z)
		builder->itemEffectOnTeam[ITEM_EFFECT_Z_CRYSTAL] = TRUE; //Treat like a Z-Crystal
	else
		builder->itemEffectOnTeam[itemEffect] = TRUE;

	if (IsChoiceItemEffectOrAbility(itemEffect, ability))
		++builder->numChoiceItems;

	if (IsMegaStone(item))
		++builder->numMegas;

	builder->speciesOnTeam[SpeciesToNationalPokedexNum(species)] = TRUE;
	for (j = 0; j < MAX_MON_MOVES; ++j)
	{
		builder->moveOnTeam[spread->moves[j]] = TRUE;

		switch (spread->moves[j]) {
			case MOVE_ELECTRICTERRAIN:
				builder->partyIndex[ELECTRIC_TERRAIN_SETTER] = partyId;
				break;

			case MOVE_PSYCHICTERRAIN:
				builder->partyIndex[PSYCHIC_TERRAIN_SETTER] = partyId;
				break;

			//Rain setter and Hail setter are used for Thunder and Blizzard, so it's better to only rely on weather Abilities for those
		}
	}

	if (spread->spdEv >= 20)
		builder->partyIndex[FAST_MON] = partyId;

	if (gAbilityRatings[ability] < 0)
		builder->partyIndex[BAD_ABILITY] = partyId;

	if (IsClassStall(class) || IsClassDoublesUtility(class) || IsClassDoublesTeamSupport(class))
		++builder->numStalls;
	else if (IsClassBatonPass(class))
		builder->partyIndex[BATON_PASSER] = partyId;
	else if (IsClassScreener(class))
		builder->partyIndex[SCREENER] = partyId;
	else if (IsClassCleric(class))
		builder->partyIndex[CLERIC] = partyId;
	else if (IsClassEntryHazards(class))
		builder->partyIndex[HAZARDS_SETUP] = partyId;

	//Abilities Always
	switch (ability) {
		case ABILITY_DRIZZLE:
			builder->partyIndex[RAIN_SETTER] = partyId;
			break;

		case ABILITY_SNOWWARNING:
			builder->partyIndex[HAIL_SETTER] = partyId;
			break;

		case ABILITY_ELECTRICSURGE:
			builder->partyIndex[ELECTRIC_TERRAIN_SETTER] = partyId;
			break;

		case ABILITY_PSYCHICSURGE:
			builder->partyIndex[PSYCHIC_TERRAIN_SETTER] = partyId;
			break;
	}

	u8 defType1 = (gBattleTypeFlags & BATTLE_TYPE_CAMOMONS) ? GetCamomonsTypeBySpread(spread, 0) : gBaseStats[species].type1;
	u8 defType2 = (gBattleTypeFlags & BATTLE_TYPE_CAMOMONS) ? GetCamomonsTypeBySpread(spread, 1) : gBaseStats[species].type2;
	if (!IsFrontierSingles(builder->battleType)) //Doubles or Multi
	{
		switch (ability) {
			case ABILITY_VOLTABSORB:
			case ABILITY_MOTORDRIVE:
			case ABILITY_LIGHTNINGROD:
				builder->partyIndex[ELECTRIC_IMMUNITY] = partyId;
				break;

			case ABILITY_WATERABSORB:
			case ABILITY_DRYSKIN:
			case ABILITY_STORMDRAIN:
				builder->partyIndex[WATER_IMMUNITY] = partyId;
				break;

			case ABILITY_FLASHFIRE:
				builder->partyIndex[FIRE_IMMUNITY] = partyId;
				break;

			case ABILITY_SAPSIPPER:
				builder->partyIndex[GRASS_IMMUNITY] = partyId;
				break;

			case ABILITY_LEVITATE:
				if (itemEffect != ITEM_EFFECT_IRON_BALL)
					builder->partyIndex[GROUND_IMMUNITY] = partyId;
				break;

			case ABILITY_SOUNDPROOF:
				builder->partyIndex[SOUND_IMMUNITY] = partyId;
				break;

			case ABILITY_JUSTIFIED:
				builder->partyIndex[JUSTIFIED_BOOSTED] = partyId;
				break;
		}

		u8 typeDmg, j;

		//Update mons with immunities
		for (j = 0; j < ARRAY_COUNT(sImmunities); ++j)
		{
			typeDmg = 10;
			ModulateByTypeEffectiveness(sImmunities[j].type, defType1, defType2, &typeDmg);
			if (typeDmg == 0 && builder->partyIndex[sImmunities[j].index] == 0xFF)
				builder->partyIndex[sImmunities[j].index] = partyId;
		}
	}
	
	//Update mon weaknesses
	if (CareAboutTeamWeaknessesInTier(builder->tier) && species != SPECIES_DITTO) //Ditto transforms anyway
	{
		u8 moveType;
		for (moveType = 0; moveType < NUMBER_OF_MON_TYPES; ++moveType)
		{
			if (moveType > TYPE_DARK && moveType < TYPE_FAIRY)
				moveType = TYPE_FAIRY; //Skip right to fairy

			if (IsSpreadWeakToType(moveType, defType1, defType2, ability))
				++builder->weaknesses[moveType];
		}
	}
}

static bool8 CareAboutTeamWeaknessesInTier(u8 tier)
{
	return tier != BATTLE_FACILITY_MONOTYPE //Too hard to avoid duplicate weaknesses
		&& tier != BATTLE_FACILITY_METRONOME //Legit random anyway
		&& !IsCamomonsTier(tier); //Weaknesses get shuffled anyway
}

static bool8 IsSpreadWeakToType(u8 moveType, u8 defType1, u8 defType2, u8 ability)
{
	u8 typeDmg = 10;
	ModulateByTypeEffectiveness(moveType, defType1, defType2, &typeDmg);

	switch (ability)
	{
		case ABILITY_LEVITATE:
			if (moveType == TYPE_GROUND)
				typeDmg = 0; //This assumes there's no Iron Ball on the spread
			break;
		case ABILITY_THICKFAT:
			if (moveType == TYPE_FIRE || moveType == TYPE_ICE)
				typeDmg /= 2;
			break;
		case ABILITY_HEATPROOF:
		case ABILITY_DRIZZLE:
			if (moveType == TYPE_FIRE)
				typeDmg /= 2;
			break;
		case ABILITY_FLUFFY:
			if (moveType == TYPE_FIRE)
				typeDmg *= 2;
			break;
		case ABILITY_DROUGHT:
			if (moveType == TYPE_WATER)
				typeDmg /= 2;
			break;
		case ABILITY_VOLTABSORB:
		case ABILITY_MOTORDRIVE:
		case ABILITY_LIGHTNINGROD:
			if (moveType == TYPE_ELECTRIC)
				typeDmg = 0;
			break;
		case ABILITY_WATERABSORB:
		case ABILITY_DRYSKIN:
		case ABILITY_STORMDRAIN:
		case ABILITY_DESOLATELAND:
			if (moveType == TYPE_WATER)
				typeDmg = 0;
			break;
		case ABILITY_FLASHFIRE:
			if (moveType == TYPE_FIRE)
				typeDmg = 0;
			break;
		case ABILITY_SAPSIPPER:
			if (moveType == TYPE_GRASS)
				typeDmg = 0;
			break;
	}

	return typeDmg >= 20; //Super effective
}

static u16 GivePlayerFrontierMonGivenSpecies(const u16 species, const struct BattleTowerSpread* const spreadTable, const u16 spreadCount)
{
	struct Pokemon mon;
	const struct BattleTowerSpread* spread = GetSpreadBySpecies(species, spreadTable, spreadCount);

	if (spread == NULL)
		return 0xFFFF;

	spread = TryAdjustSpreadForSpecies(spread); //Update Arceus
	CreateFrontierMon(&mon, 50, spread, 0, 0, 0, TRUE);
	SetMonPokedexFlags(&mon);
	return GiveMonToPlayer(&mon);
}

void CreateFrontierRaidMon(u16 originalSpecies)
{
	u16 species;
	struct Pokemon mon;
	const struct BattleTowerSpread* spreadPtr = (const struct BattleTowerSpread*) gPokeBackupPtr;

	//spreadPtr = GetSpreadBySpecies(SPECIES_RAYQUAZA, gFrontierLegendarySpreads, NELEMS(gFrontierLegendarySpreads));
	if (spreadPtr == NULL)
		return;

	spreadPtr = TryAdjustSpreadForSpecies(spreadPtr); //Update Arceus
	struct BattleTowerSpread spread = *spreadPtr;

	if (IsGigantamaxSpecies(originalSpecies)) //Player was shown Gigantamaxed form
		spread.species = originalSpecies; //Update with Gigantamax form
	else
	{
		species = GetMegaSpecies(spread.species, spread.item, spread.moves); //Try Mega Evolve the mon
		if (species != SPECIES_NONE)
			spread.species = species; //Update with Mega Species
	}

	CreateFrontierMon(&mon, 50, &spread, 0, 0, 0, TRUE);

	#ifdef SPECIES_ETERNATUS_ETERNAMAX
	if (originalSpecies == SPECIES_ETERNATUS_ETERNAMAX) //Reverted to SPECIES_ETERNATUS in CreateFrontierMon so set down here
	{
		mon.species = SPECIES_ETERNATUS_ETERNAMAX;
		CalculateMonStatsNew(&mon);
		HealMon(&mon);
	}
	#endif

	ZeroEnemyPartyMons();
	gEnemyParty[0] = mon;
	gPokeBackupPtr = NULL;
}

static const struct BattleTowerSpread* GetSpreadBySpecies(const u16 species, const struct BattleTowerSpread* const spreads, const u16 spreadCount)
{
	u32 i;

	for (i = 0; i < spreadCount; ++i)
	{
		if (spreads[i].species == species)
			break;
	}

	if (i == spreadCount)
		return NULL; //Species not found

	u8 offset = Random() % 5; //Max number of possible spreads for a given Pokemon

	while (spreads[i + offset].species != species) //Overshot
		offset = Random() % 5;

	return &spreads[i + offset];
}

static void TryGetSpecialSpeciesSpreadTable(u16 species, const struct BattleTowerSpread** table, u16* spreadCount)
{
	switch (SpeciesToNationalPokedexNum(species))
	{
		#ifdef NATIONAL_DEX_PIKACHU
		case NATIONAL_DEX_PIKACHU:
			*table = gPikachuSpreads; //Sooo many different forms of Pikachu
			*spreadCount = TOTAL_PIKACHU_SPREADS;
			break;
		#endif
		#ifdef NATIONAL_DEX_WORMADAM
		case NATIONAL_DEX_WORMADAM:
			*table = gWormadamSpreads;
			*spreadCount = TOTAL_WORMADAM_SPREADS;
			break;
		#endif
		#ifdef NATIONAL_DEX_ROTOM
		case NATIONAL_DEX_ROTOM:
			*table = gRotomSpreads;
			*spreadCount = TOTAL_ROTOM_SPREADS;
			break;
		#endif
		#ifdef NATIONAL_DEX_ARCEUS
		case NATIONAL_DEX_ARCEUS:
			*table = gArceusSpreads;
			*spreadCount = TOTAL_ARCEUS_SPREADS;
			break;
		#endif
		#ifdef NATIONAL_DEX_ORICORIO
		case NATIONAL_DEX_ORICORIO:
			*table = gOricorioSpreads;
			*spreadCount = TOTAL_ORICORIO_SPREADS;
			break;
		#endif
	}
}

static const struct BattleTowerSpread* TryAdjustSpreadForSpecies(const struct BattleTowerSpread* originalSpread)
{
	u16 species = originalSpread->species;

	const struct BattleTowerSpread* spreadTable = NULL;
	u16 spreadCount = 0;	
	TryGetSpecialSpeciesSpreadTable(species, &spreadTable, &spreadCount);

	if (spreadTable == NULL)
		return originalSpread;
	
	return &spreadTable[Random() % spreadCount];
}

static u16 TryAdjustAestheticSpecies(u16 species)
{
	u16 nationalDexNum = SpeciesToNationalPokedexNum(species);

	switch (nationalDexNum) {
		#ifdef NATIONAL_DEX_SHELLOS
		case NATIONAL_DEX_SHELLOS:
			if ((Random() & 1) == 0)
				species = SPECIES_SHELLOS;
			else
				species = SPECIES_SHELLOS_EAST;
			break;
		#endif
		#ifdef NATIONAL_DEX_GASTRODON
		case NATIONAL_DEX_GASTRODON:
			if ((Random() & 1) == 0)
				species = SPECIES_GASTRODON;
			else
				species = SPECIES_GASTRODON_EAST;
			break;
		#endif
		#ifdef NATIONAL_DEX_DEERLING
		case NATIONAL_DEX_DEERLING:
			species = gDeerlingForms[Random() % gNumDeerlingForms];
			break;
		#endif
		#ifdef NATIONAL_DEX_SAWSBUCK
		case NATIONAL_DEX_SAWSBUCK:
			species = gSawsbuckForms[Random() % gNumSawsbuckForms];
			break;
		#endif
		#ifdef NATIONAL_DEX_VIVILLON
		case NATIONAL_DEX_VIVILLON:
			species = gVivillonForms[Random() % gNumVivillonForms];
			break;
		#endif
		#ifdef NATIONAL_DEX_FLABEBE
		case NATIONAL_DEX_FLABEBE:
			species = gFlabebeForms[Random() % gNumFlabebeForms];
			break;
		#endif
		#ifdef NATIONAL_DEX_FLOETTE
		case NATIONAL_DEX_FLOETTE:
			#ifdef SPECIES_FLOETTE_ETERNAL
			if (species != SPECIES_FLOETTE_ETERNAL) //Floette Eternal gets its own spreads
			#endif
				species = gFloetteForms[Random() % gNumFloetteForms];
			break;
		#endif
		#ifdef NATIONAL_DEX_FLORGES
		case NATIONAL_DEX_FLORGES:
			species = gFlorgesForms[Random() % gNumFlorgesForms];
			break;
		#endif
		#ifdef NATIONAL_DEX_FURFROU
		case NATIONAL_DEX_FURFROU:
			species = gFurfrouForms[Random() % gNumFurfrouForms];
			break;
		#endif
		#ifdef NATIONAL_DEX_ALCREMIE
		case NATIONAL_DEX_ALCREMIE:
			species = gAlcremieForms[Random() % gNumAlcremieForms];
			break;
		#endif
		default:
			#ifdef SPECIES_PIKACHU_CAP_ORIGINAL
			if (species == SPECIES_PIKACHU_CAP_ORIGINAL)
				species = gPikachuCapForms[Random() % gNumPikachuCapForms];
			#endif
			break;
	}

	return species;
}

static void SwapMons(struct Pokemon* party, u8 i, u8 j)
{
	struct Pokemon tempMon;

	Memcpy(&tempMon, &party[i], sizeof(struct Pokemon));
	Memcpy(&party[i], &party[j], sizeof(struct Pokemon));
	Memcpy(&party[j], &tempMon, sizeof(struct Pokemon));
}

struct DoubleReplacementMoves
{
	u16 oldMove;
	u16 replacementMove;
	u8 learnType;
	u16 other;
	u8 replaceIfNoImmunity;
	u8 replaceIfImmunity;
};

enum
{
	LEARN_TYPE_LEVEL_UP,
	LEARN_TYPE_TM,
	LEARN_TYPE_TUTOR,
};

#ifdef UNBOUND

static struct DoubleReplacementMoves sDoubleSpreadReplacementMoves[] =
{
	{MOVE_OUTRAGE, MOVE_DRAGONCLAW, LEARN_TYPE_TM, ITEM_TM02_DRAGON_CLAW, 0, 0},
	{MOVE_SUBSTITUTE, MOVE_PROTECT, LEARN_TYPE_TM, ITEM_TM17_PROTECT, 0, 0},
	{MOVE_THRASH, MOVE_RETURN, LEARN_TYPE_TM, ITEM_TM27_RETURN, 0, 0},
	{MOVE_STONEEDGE, MOVE_ROCKSLIDE, LEARN_TYPE_TM, ITEM_TM80_ROCK_SLIDE, 0, 0},
	{MOVE_MOONBLAST, MOVE_DAZZLINGGLEAM, LEARN_TYPE_TM, ITEM_TM102_DAZZLING_GLEAM, 0, 0},
	{MOVE_FLAMETHROWER, MOVE_HEATWAVE, LEARN_TYPE_TUTOR, TUTOR50_HEAT_WAVE, 0, 0},
	{MOVE_FIREBLAST, MOVE_HEATWAVE, LEARN_TYPE_TUTOR, TUTOR50_HEAT_WAVE, 0, 0},

	//Replace Move 1 with Move 2 if immunity isn't found on team
	{MOVE_SLUDGEWAVE, MOVE_SLUDGEBOMB, LEARN_TYPE_TM, ITEM_TM36_SLUDGE_BOMB, POISON_IMMUNITY, 0},
	{MOVE_BOOMBURST, MOVE_HYPERVOICE, LEARN_TYPE_TUTOR, TUTOR51_HYPER_VOICE, SOUND_IMMUNITY, 0},
	{MOVE_SURF, MOVE_MUDDYWATER, LEARN_TYPE_TUTOR, TUTOR111_MUDDY_WATER, WATER_IMMUNITY, 0},
	{MOVE_EARTHQUAKE, MOVE_HIGHHORSEPOWER, LEARN_TYPE_TUTOR, TUTOR109_HIGH_HORSEPOWER, GROUND_IMMUNITY, 0},

	//Replace Move 1 with Move 2 if weather or terrain setter is found on team
	{MOVE_THUNDERBOLT, MOVE_RISINGVOLTAGE, LEARN_TYPE_TUTOR, TUTOR134_RISING_VOLTAGE, 0, ELECTRIC_TERRAIN_SETTER},
	{MOVE_PSYCHIC, MOVE_EXPANDINGFORCE, LEARN_TYPE_TUTOR, TUTOR128_EXPANDING_FORCE, 0, PSYCHIC_TERRAIN_SETTER},
	{MOVE_PSYSHOCK, MOVE_EXPANDINGFORCE, LEARN_TYPE_TUTOR, TUTOR128_EXPANDING_FORCE, 0, PSYCHIC_TERRAIN_SETTER},
	{MOVE_EXTRASENSORY, MOVE_EXPANDINGFORCE, LEARN_TYPE_TUTOR, TUTOR128_EXPANDING_FORCE, 0, PSYCHIC_TERRAIN_SETTER},
	{MOVE_THUNDERBOLT, MOVE_THUNDER, LEARN_TYPE_TM, ITEM_TM25_THUNDER, 0, RAIN_SETTER},
	{MOVE_AIRSLASH, MOVE_HURRICANE, LEARN_TYPE_TUTOR, TUTOR122_HURRICANE, 0, RAIN_SETTER},
	{MOVE_ICEBEAM, MOVE_BLIZZARD, LEARN_TYPE_TM, ITEM_TM14_BLIZZARD, 0, HAIL_SETTER},

	//Replace Move 1 with Move 2 if immunity is found on team
	{MOVE_THUNDERBOLT, MOVE_DISCHARGE, LEARN_TYPE_LEVEL_UP, 0, 0, ELECTRIC_IMMUNITY},
	{MOVE_HYPERVOICE, MOVE_BOOMBURST, LEARN_TYPE_LEVEL_UP, 0, 0, SOUND_IMMUNITY},
};

#endif

static void PostProcessTeam(struct Pokemon* party, struct TeamBuilder* builder)
{
	u32 i, j;

	u8 tailwindTRIndex = 0xFF;
	u8 hazardsIndex = 0xFF;
	u8 screensIndex = 0xFF;
	u8 weatherIndex = 0xFF;
	u8 terrainIndex = 0xFF;
	u8 defiantIndex = 0xFF;
	u8 intimidateIndex = 0xFF;
	u8 sleepIndex = 0xFF;
	u8 yawnIndex = 0xFF;
	u8 illusionIndex = 0xFF;
	u8 followMeIndex = 0xFF;

	for (i = 0; i < builder->monsCount; ++i)
	{
		if (builder->spreads[i] == NULL)
			break;

		for (j = 0; j < MAX_MON_MOVES; ++j)
		{
			u16 move = builder->spreads[i]->moves[j];
			u8 moveEffect = gBattleMoves[move].effect;

			if (move == MOVE_TAILWIND || move == MOVE_TRICKROOM)
				tailwindTRIndex = i;

			switch (moveEffect) {
				case EFFECT_SPIKES:
					hazardsIndex = i;
					break;
				case EFFECT_REFLECT:
				case EFFECT_LIGHT_SCREEN:
					screensIndex = i;
					break;
				case EFFECT_RAIN_DANCE:
				case EFFECT_SUNNY_DAY:
				case EFFECT_SANDSTORM:
				case EFFECT_HAIL:
					weatherIndex = i;
					break;
				case EFFECT_SET_TERRAIN:
					terrainIndex = i;
					break;
				case EFFECT_SLEEP:
					sleepIndex = i;
					break;
				case EFFECT_YAWN:
					yawnIndex = i;
					break;
				case EFFECT_FOLLOW_ME:
					followMeIndex = i;
					break;
			}
		}

		switch (ConvertFrontierAbilityNumToAbility(builder->spreads[i]->ability, builder->spreads[i]->species)) {
			case ABILITY_DRIZZLE:
			case ABILITY_DROUGHT:
			case ABILITY_SANDSTREAM:
			case ABILITY_SNOWWARNING:
				weatherIndex = i;
				break;
			case ABILITY_ELECTRICSURGE:
			case ABILITY_GRASSYSURGE:
			case ABILITY_MISTYSURGE:
			case ABILITY_PSYCHICSURGE:
				terrainIndex = i;
				break;
			case ABILITY_DEFIANT:
			case ABILITY_COMPETITIVE:
			case ABILITY_CONTRARY:
				defiantIndex = i;
				break;
			case ABILITY_INTIMIDATE:
				intimidateIndex = i;
				break;
			case ABILITY_ILLUSION:
				illusionIndex = i;
				break;
		}

		//Try to replace healing items depending on tier
		u16 item;
		switch (ItemId_GetHoldEffect(builder->spreads[i]->item)) {
			case ITEM_EFFECT_RESTORE_HP:
			case ITEM_EFFECT_ENIGMA_BERRY:
				if (!DuplicateItemsAreBannedInTier(builder->tier, builder->battleType)) //Only replace items if no worry of duplicate items
				{
					switch (builder->spreads[i]->item) {
						case ITEM_ORAN_BERRY:
						case ITEM_BERRY_JUICE:
							item = ITEM_SITRUS_BERRY;
							if (GetMonData(&party[i], MON_DATA_MAX_HP, NULL) / 4 > ItemId_GetHoldEffectParam(item))
								SetMonData(&party[i], MON_DATA_HELD_ITEM, &item);
							break;
						default: //Sitrus Berry + Enigma Berry
							item = ITEM_BERRY_JUICE;
							if (GetMonData(&party[i], MON_DATA_MAX_HP, NULL) / 4 < ItemId_GetHoldEffectParam(item))
								SetMonData(&party[i], MON_DATA_HELD_ITEM, &item);
							break;
					}
				}
				break;
			case ITEM_EFFECT_CONFUSE_SPICY:
			case ITEM_EFFECT_CONFUSE_DRY:
			case ITEM_EFFECT_CONFUSE_SWEET:
			case ITEM_EFFECT_CONFUSE_BITTER:
			case ITEM_EFFECT_CONFUSE_SOUR:
				if (!DuplicateItemsAreBannedInTier(builder->tier, builder->battleType))
				{
					item = ITEM_BERRY_JUICE;
					if (GetMonData(&party[i], MON_DATA_MAX_HP, NULL) / 4 < ItemId_GetHoldEffectParam(item))
						SetMonData(&party[i], MON_DATA_HELD_ITEM, &item);
					break;
				}
		}
	}

	//Shuffle moves in camomons
	TryShuffleMovesForCamomons(party, builder->tier, builder->trainerId);

	//Try change lead
	if (IsFrontierSingles(builder->battleType))
	{
		if (builder->partyIndex[HAZARDS_SETUP] != 0xFF)
		{
			if (builder->partyIndex[HAZARDS_SETUP] != 0) //Entry Hazards aren't already at lead
				SwapMons(party, 0, builder->partyIndex[HAZARDS_SETUP]);
		}
		else if (builder->partyIndex[BATON_PASSER] != 0xFF)
		{
			if (builder->partyIndex[BATON_PASSER] != 0) //Baton Passer isn't already at lead
				SwapMons(party, 0, builder->partyIndex[BATON_PASSER]);
		}
		else //Try to stick a mon with Tailwind/Trick Room or entry hazards at the front
		{
			if (tailwindTRIndex != 0xFF)
				SwapMons(party, 0, tailwindTRIndex);
			else if (hazardsIndex != 0xFF)
				SwapMons(party, 0, hazardsIndex);
			else if (builder->partyIndex[SCREENER] != 0xFF)
			{
				if (builder->partyIndex[SCREENER] != 0) //Screener isn't already at lead
					SwapMons(party, 0, builder->partyIndex[SCREENER]);
			}
			else if (screensIndex != 0xFF)
				SwapMons(party, 0, screensIndex);
			else if (terrainIndex != 0xFF)
				SwapMons(party, 0, terrainIndex);
			else if (weatherIndex != 0xFF)
				SwapMons(party, 0, weatherIndex);
			else if (defiantIndex != 0xFF && party == gEnemyParty && GetMonAbility(&gPlayerParty[0]) == ABILITY_INTIMIDATE)
				SwapMons(party, 0, defiantIndex); //Stick Pokemon with Defiant/Competitive up front to absorb the Intimidate
			else if (sleepIndex != 0xFF)
				SwapMons(party, 0, sleepIndex);
			else if (yawnIndex != 0xFF)
				SwapMons(party, 0, yawnIndex);
			else if (illusionIndex != 0xFF)
				SwapMons(party, 0, illusionIndex);
		}
	}
	else //Doubles or Multi
	{
		for (i = 0; i < builder->monsCount; ++i)
		{
			if (builder->spreads[i]->modifyMovesDoubles)
			{
				#ifdef UNBOUND
					u32 k;
					u16 levelUpMoves[MAX_LEARNABLE_MOVES] = {MOVE_NONE};
					GetLevelUpMovesBySpecies(GetMonData(&party[i], MON_DATA_SPECIES, NULL), levelUpMoves);

					for (j = 0; j < ARRAY_COUNT(sDoubleSpreadReplacementMoves); ++j)
					{
						u16 oldMove = sDoubleSpreadReplacementMoves[j].oldMove;
						u16 newMove = sDoubleSpreadReplacementMoves[j].replacementMove;
						u8 pos = FindMovePositionInMonMoveset(oldMove, &party[i]);
						u8 newPP = CalculatePPWithBonus(newMove, 0xFF, pos);

						if (pos < MAX_MON_MOVES && !MoveInMonMoveset(newMove, &party[i]))
						{
							//Don't replace the move if the immunity is found on the team
							//Eg. Earthquake -> High Horsepower if no GROUND_IMMUNITY
							u8 immunity = sDoubleSpreadReplacementMoves[j].replaceIfNoImmunity;
							if (immunity != 0 //Entry in the table
							&& builder->partyIndex[immunity] != 0xFF //This immunity is on the team (Eg. Flying-type on team)
							&& builder->partyIndex[immunity] != i //This mon isn't the mon with the immunity (Eg. Flying-type is someone else)
							&& !IsFrontierMulti(builder->battleType)) //This mon could be together with the immune mon (Eg. This mon could be with the Flying mon)
								continue; //Don't replace move (Eg. The Earthquaker could be paired with a Flying-type)

							//Only replace the move if the immunity is found on the team
							//Eg. Thunderbolt -> Discharge if ELECTRIC_IMMUNITY
							immunity = sDoubleSpreadReplacementMoves[j].replaceIfImmunity;
							if (immunity != 0 //Entry in the table
							&& (builder->partyIndex[immunity] == 0xFF //This immunity isn't on the team (No Volt Absorber on team)
							|| builder->partyIndex[immunity] == i //This mon is the mon with the immunity (Eg. Volt Absorber is this mon)
							|| (IsFrontierMulti(builder->battleType) //This mon couldn't be together with the immune mon
							 && sDoubleSpreadReplacementMoves[j].replaceIfImmunity < PARTY_INDEX_FIELD_SETTERS_START))) //And the immunity is an immunity and not something like PSYCHIC_TERRAIN_SETTER
								continue;

							switch (sDoubleSpreadReplacementMoves[j].learnType) {
								case LEARN_TYPE_TM:
									if (CanMonLearnTMTutor(&party[i], sDoubleSpreadReplacementMoves[j].other, 0) == CAN_LEARN_MOVE)
									{
										SetMonData(&party[i], MON_DATA_MOVE1 + pos, &newMove);
										SetMonData(&party[i], MON_DATA_PP1 + pos, &newPP);
									}
									break;

								case LEARN_TYPE_TUTOR:
									if (CanMonLearnTMTutor(&party[i], 0, sDoubleSpreadReplacementMoves[j].other) == CAN_LEARN_MOVE)
									{
										SetMonData(&party[i], MON_DATA_MOVE1 + pos, &newMove);
										SetMonData(&party[i], MON_DATA_PP1 + pos, &newPP);
									}
									break;

								case LEARN_TYPE_LEVEL_UP:
									for (k = 0; k < MAX_LEARNABLE_MOVES && levelUpMoves[k] != MOVE_NONE; ++k)
									{
										if (levelUpMoves[k] == newMove)
										{
											SetMonData(&party[i], MON_DATA_MOVE1 + pos, &newMove);
											SetMonData(&party[i], MON_DATA_PP1 + pos, &newPP);
											break;
										}
									}
									break;
							}
						}
					}
				#endif
			}
		}

		for (i = 0; i < ARRAY_COUNT(sImmunities); ++i)
		{
			u8 partyIndex = builder->partyIndex[sImmunities[i].index];
			if (partyIndex != 0xFF)
			{
				if (partyIndex != 0 && partyIndex != 1)
				{
					if (AllHittingMoveWithTypeInMonMoveset(&party[0], sImmunities[i].type))
					{
						SwapMons(party, 1, partyIndex); //Replace the second Pokemon
						break;
					}
					else if (AllHittingMoveWithTypeInMonMoveset(&party[1], sImmunities[i].type))
					{
						SwapMons(party, 0, partyIndex); //Replace the first Pokemon
						break;
					}
				}
			}
		}

		#define INDEX_CHECK(partyId) (index < 2 && partyId > index && partyId != 0xFF)

		if (i == ARRAY_COUNT(sImmunities)) //The two Pokemon up front aren't meant to work off of each other
		{
			u8 index = 0;

			if (INDEX_CHECK(builder->partyIndex[HAZARDS_SETUP]))
				SwapMons(party, index++, builder->partyIndex[HAZARDS_SETUP]);

			if (INDEX_CHECK(tailwindTRIndex))
				SwapMons(party, index++, tailwindTRIndex);

			if (INDEX_CHECK(builder->partyIndex[SCREENER]))
			{
				if (builder->partyIndex[SCREENER] != 0) //Screener isn't already at lead
					SwapMons(party, index++, builder->partyIndex[SCREENER]);
			}
			else if (INDEX_CHECK(screensIndex))
				SwapMons(party, index++, screensIndex);

			if (INDEX_CHECK(terrainIndex))
				SwapMons(party, index++, terrainIndex);

			if (INDEX_CHECK(weatherIndex))
				SwapMons(party, index++, weatherIndex);

			if (INDEX_CHECK(intimidateIndex))
				SwapMons(party, index++, intimidateIndex);

			if (INDEX_CHECK(followMeIndex))
				SwapMons(party, index++, followMeIndex);

			if (INDEX_CHECK(defiantIndex) && party == gEnemyParty
			&& (GetMonAbility(&gPlayerParty[0]) == ABILITY_INTIMIDATE || GetMonAbility(&gPlayerParty[1]) == ABILITY_INTIMIDATE))
				SwapMons(party, index++, defiantIndex); //Stick Pokemon with Defiant/Competitive up front to absorb the Intimidate

			if (INDEX_CHECK(sleepIndex))
				SwapMons(party, index++, sleepIndex);

			if (INDEX_CHECK(yawnIndex))
				SwapMons(party, index++, yawnIndex);

			if (INDEX_CHECK(illusionIndex))
				SwapMons(party, index++, illusionIndex);

			if (INDEX_CHECK(hazardsIndex))
				SwapMons(party, index++, hazardsIndex);
		}
	}

	//Try change last mon
	if (builder->monsCount >= 3 && GetMonAbility(&party[builder->monsCount - 1]) == ABILITY_ILLUSION)
	{
		bool8 onePartyMonOnField = IsFrontierSingles(builder->battleType) || IsFrontierMulti(builder->battleType); //Only one mon from the specific team is on the field at a time
		for (i = onePartyMonOnField ? 1 : 2; i < (u32) (builder->monsCount - 1); ++i) //-1 because we already know the last mon has Illusion and wants to leave that slot
		{
			if (GetMonAbility(&party[i]) != ABILITY_ILLUSION)
			{
				SwapMons(party, i, builder->monsCount - 1); //Move Illusion mon as close to the front as possible without putting it on the field
				break;
			}
		}
	}
}

static void TryShuffleMovesForCamomons(struct Pokemon* party, u8 tier, u16 trainerId)
{
	u32 i, j;

	if (IsCamomonsTier(tier) && trainerId != FRONTIER_BRAIN_TID)
	{
		for (i = 0; i < PARTY_SIZE; ++i)
		{
			if (GetMonData(&party[i], MON_DATA_SPECIES, NULL) == SPECIES_NONE
			||  GetMonData(&party[i], MON_DATA_HP, NULL) == 0)
				continue;

			for (j = 0; j < 30; ++j) //Shuffle 30 times
			{
				u8 index1 = Random() & 3;
				u8 index2 = Random() & 3;

				u16 move1 = party[i].moves[index1];
				u16 move2 = party[i].moves[index2];
				u8 pp1 = party[i].pp[index1];
				u8 pp2 = party[i].pp[index2];

				if (move1 == MOVE_NONE || move2 == MOVE_NONE)
					continue;

				party[i].moves[index1] = move2;
				party[i].moves[index2] = move1;
				party[i].pp[index1] = pp2;
				party[i].pp[index2] = pp1;
			}
		}
	}
}

bool8 IsMonAllowedInBattleTower(struct Pokemon* mon)
{
	if (GetMonData(mon, MON_DATA_IS_EGG, NULL))
		return FALSE;

	if (FlagGet(FLAG_BATTLE_FACILITY))
	{
		u16 species = mon->species;
		u16 item = mon->item;
		u16 tier = VarGet(VAR_BATTLE_FACILITY_TIER);

		if (PokemonTierBan(species, item, NULL, mon, tier, CHECK_PARTY_OFFSET))
			return FALSE;

		if (tier != BATTLE_FACILITY_NO_RESTRICTIONS) //Free for all has no duplicate species or item restriction
		{
			u8 partySize = 0;
			u8 partyId = GetPartyIdFromPartyData(mon);
			u16 speciesArray[PARTY_SIZE] = {0};
			u16 itemArray[PARTY_SIZE] = {0};

			for (int i = 0; gSelectedOrderFromParty[i] != 0 && i < 6; ++i, ++partySize)
			{
				if (gSelectedOrderFromParty[i] - 1 != partyId) //You can't ban yourself
				{
					speciesArray[i] = gPlayerParty[gSelectedOrderFromParty[i] - 1].species;
					itemArray[i] = gPlayerParty[gSelectedOrderFromParty[i] - 1].item;
				}
			}

			if (SpeciesAlreadyOnTeam(mon->species, partySize, speciesArray)
			|| (DuplicateItemsAreBannedInTier(tier, VarGet(VAR_BATTLE_FACILITY_BATTLE_TYPE)) && ItemAlreadyOnTeam(mon->item, partySize, itemArray))
			|| (tier == BATTLE_FACILITY_MONOTYPE && TeamNotAllSameType(mon->species, mon->item, partySize, speciesArray, itemArray))
			|| (tier == BATTLE_FACILITY_GS_CUP && !IsFrontierSingles(VarGet(VAR_BATTLE_FACILITY_BATTLE_TYPE)) && TooManyLegendariesOnGSCupTeam(mon->species, partySize, speciesArray)))
				return FALSE;
		}
	}
	else if (GetMonData(mon, MON_DATA_HP, NULL) == 0) //Regular multi battle probably
		return FALSE;

	return TRUE;
}

static u8 GetPartyIdFromPartyData(struct Pokemon* mon)
{
	u8 id;
	for (id = 0; mon != gPlayerParty; --mon, ++id);

	return id;
}

static u8 GetHighestMonLevel(const struct Pokemon* const party)
{
	u8 max = GetMonData(&party[0], MON_DATA_LEVEL, NULL);

	for (int i = 1; i < PARTY_SIZE; ++i)
	{
		u8 level;
		u16 species = GetMonData(&party[i], MON_DATA_SPECIES2, NULL);
	
		if (max == MAX_LEVEL || species == SPECIES_NONE)
			return max;

		if (species == SPECIES_EGG)
			continue;

		level = GetMonData(&party[i], MON_DATA_LEVEL, NULL);
		if (level > max)
			max = level;
	}

	return max;
}

u8 GetMonPokeBall(struct PokemonSubstruct0* data)
{
	return data->pokeball;
}

void SetMonPokeBall(struct PokemonSubstruct0* data, u8 ballId)
{
	data->pokeball = ballId;
}

void SetMonHeldItem(struct PokemonSubstruct0* data, u16 item, struct Pokemon* mon)
{
	data->heldItem = item;
	HoldItemFormChange(mon, item);
}

#ifdef OPEN_WORLD_TRAINERS

static u8 GetOpenWorldTrainerMonAmount(void)
{
	switch (GetOpenWorldBadgeCount()) {
		case 0:
			return 1;
		case 1:
		case 2:
			return 2;
		case 3:
		case 4:
			return 3;
		case 5:
		case 6:
			return 4;
		case 7:
		case 8:
			return 5;
		default:
			return 6;
	}
}

static u8 GetOpenWorldSpeciesIndex(u32 nameHash, u8 i)
{
	return ((nameHash + 2 * i) ^ T1_READ_32(gSaveBlock2->playerTrainerId)) % 10;
}

static u8 GetOpenWorldSpeciesLevel(u32 nameHash, u8 i)
{
	u8 badgeCount = GetOpenWorldBadgeCount();

	u8 max = MathMax(gOpenWorldLevelRanges[badgeCount][0], gOpenWorldLevelRanges[badgeCount][1]); //Prevent incorrect order errors
	u8 min = MathMin(gOpenWorldLevelRanges[badgeCount][0], gOpenWorldLevelRanges[badgeCount][1]);
	u8 range = (max - min) + 1;

	return min + ((nameHash + 7 * i) ^ T1_READ_32(gSaveBlock2->playerTrainerId)) % range;
}

#endif

u8 GetOpenWorldBadgeCount(void)
{
	u8 badgeCount = 0;

	if (FlagGet(FLAG_SYS_GAME_CLEAR)) //0x82C
		return 9;

	if (FlagGet(FLAG_BADGE08_GET))
		++badgeCount;
	if (FlagGet(FLAG_BADGE07_GET))
		++badgeCount;
	if (FlagGet(FLAG_BADGE06_GET))
		++badgeCount;
	if (FlagGet(FLAG_BADGE05_GET))
		++badgeCount;
	if (FlagGet(FLAG_BADGE04_GET))
		++badgeCount;
	if (FlagGet(FLAG_BADGE03_GET))
		++badgeCount;
	if (FlagGet(FLAG_BADGE02_GET))
		++badgeCount;
	if (FlagGet(FLAG_BADGE01_GET))
		++badgeCount;

	return badgeCount;
}

//unused1 is used to hook in so don't use it for anything
u8 ScriptGiveMon(u16 species, u8 level, u16 item, unusedArg u32 unused1, unusedArg u32 customGivePokemon, unusedArg u8 ballType)
{
	u8 sentToPc;
	struct Pokemon mon;

	CreateMon(&mon, species, level, 32, 0, 0, 0, 0);
	SetMonData(&mon, MON_DATA_HELD_ITEM, &item);

	#ifdef GIVEPOKEMON_BALL_HACK
	if (ballType != 0)
		mon.pokeball = ballType;
	#endif

	#ifdef FLAG_HIDDEN_ABILITY
	if (FlagGet(FLAG_HIDDEN_ABILITY))
	{
		mon.hiddenAbility = TRUE;
		FlagClear(FLAG_HIDDEN_ABILITY);
	}
	#endif

	#ifdef FLAG_GIGANTAMAXABLE
	if (FlagGet(FLAG_GIGANTAMAXABLE))
	{
		mon.gigantamax = TRUE;
		FlagClear(FLAG_GIGANTAMAXABLE);
	}
	#endif

	#ifdef GIVEPOKEMON_CUSTOM_HACK
	if (customGivePokemon != 0)
	{
		u8 i;
		u16* moves = &Var8000; //-0x8003
		u8 nature = Var8004;
		bool8 shiny = Var8005;
		u16* ivs = &Var8006; //-0x800B

		for (i = 0; i < MAX_MON_MOVES; ++i)
		{
			if (moves[i] < MOVES_COUNT)
				SetMonData(&mon, MON_DATA_MOVE1 + i, &moves[i]);
		}

		for (i = 0; i < NUM_STATS; ++i)
			SetMonData(&mon, MON_DATA_HP_IV + i, &ivs[i]);

		if (nature >= NUM_NATURES)
		{
			#ifdef SYNCHRONIZE_GIFT_POKEMON
			if (GetMonAbility(&gPlayerParty[0]) == ABILITY_SYNCHRONIZE) //Lead has Synchronize
				nature = GetNature(&gPlayerParty[0]); //Match lead
			else
			#endif
				nature = Random() % NUM_NATURES;
		}

		GiveMonNatureAndAbility(&mon, nature, GetMonData(&mon, MON_DATA_PERSONALITY, NULL) & 1, shiny ? TRUE : IsMonShiny(&mon), FALSE, FALSE);
		CalculateMonStats(&mon);
	}
	else
	#endif
	{
		#ifdef SYNCHRONIZE_GIFT_POKEMON
		if (GetMonAbility(&gPlayerParty[0]) == ABILITY_SYNCHRONIZE) //Lead has Synchronize
		{
			u8 nature = GetNature(&gPlayerParty[0]); //Match lead
			GiveMonNatureAndAbility(&mon, nature, GetMonData(&mon, MON_DATA_PERSONALITY, NULL) & 1, IsMonShiny(&mon), FALSE, FALSE);
			CalculateMonStats(&mon);
		}
		#endif
	}

	HealMon(&mon);
	sentToPc = GiveMonToPlayer(&mon);

	switch(sentToPc) {
		case 0:
		case 1:
			SetMonPokedexFlags(&mon);
			break;
	}

	return sentToPc;
}

static void CheckShinyMon(struct Pokemon* mon)
{
	bool8 forceShiny = FALSE;
	u32 otId = GetMonData(mon, MON_DATA_OT_ID, NULL);

	#ifdef FLAG_SHINY_CREATION
	if (FlagGet(FLAG_SHINY_CREATION))
	{
		forceShiny = TRUE;
	}
	else
	#endif
	{
		#ifdef ITEM_SHINY_CHARM
		if (CheckBagHasItem(ITEM_SHINY_CHARM, 1) > 0)
		{
			//Try an extra 2 times to generate shiny personality
			if (IsShinyOtIdPersonality(otId, Random32())
			||  IsShinyOtIdPersonality(otId, Random32()))
				forceShiny = TRUE;
		}
		#endif

		if (!forceShiny && gFishingByte) //Currently fishing
		{
			u32 i, numTries; //Tries an extra 2 times per Pokemon in the streak up to 40 times

			for (i = 0, numTries = 2 * MathMin(gFishingStreak, 20); i < numTries; ++i)
			{
				if (IsShinyOtIdPersonality(otId, Random32()))
				{
					forceShiny = TRUE;
					break;
				}
			}
		}
	}

	if (forceShiny)
		ForceMonShiny(mon);
}

void ForceMonShiny(struct Pokemon* mon)
{
	if (IsMonShiny(mon))
		return;

	u32 trainerId = GetMonData(mon, MON_DATA_OT_ID, NULL);
	u16 sid = HIHALF(trainerId);
	u16 tid = LOHALF(trainerId);

	u16 species = GetMonData(mon, MON_DATA_SPECIES, NULL);
	u32 personality = GetMonData(mon, MON_DATA_PERSONALITY, NULL);
	u8 ability = personality & 1;
	u8 nature = GetNatureFromPersonality(personality);
	bool8 abilityMatters = !mon->hiddenAbility;
	u8 gender = GetGenderFromSpeciesAndPersonality(species, personality);
	u8 letter = GetUnownLetterFromPersonality(personality);
	bool8 isMinior = IsMinior(species);
	u16 miniorCore = GetMiniorCoreFromPersonality(personality);

	do
	{
		personality = Random32();

		u8 shinyRange = Random() % SHINY_ODDS;
		personality = (((shinyRange ^ (sid ^ tid)) ^ LOHALF(personality)) << 16) | LOHALF(personality);

		if (abilityMatters)
		{
			personality &= ~(1);
			personality |= ability; //Either 0 or 1
		}

	} while (GetNatureFromPersonality(personality) != nature || GetGenderFromSpeciesAndPersonality(species, personality) != gender
	|| (species == SPECIES_UNOWN && GetUnownLetterFromPersonality(personality) != letter)
	|| (isMinior && GetMiniorCoreFromPersonality(personality) != miniorCore));

	SetMonData(mon, MON_DATA_PERSONALITY, &personality);
	CalculateMonStats(mon);
}

void TryRandomizeSpecies(unusedArg u16* species)
{
	#ifdef FLAG_POKEMON_RANDOMIZER
	u32 speciesCount = NUM_SPECIES_RANDOMIZER;

	#ifdef FLAG_GEN_8_PLACED_IN_GAME
	if (FlagGet(FLAG_GEN_8_PLACED_IN_GAME))
		speciesCount = NUM_SPECIES_GEN_8;
	#endif

	if (FlagGet(FLAG_POKEMON_RANDOMIZER) && !FlagGet(FLAG_BATTLE_FACILITY)
	#ifdef FLAG_TEMP_DISABLE_RANDOMIZER
	&& !FlagGet(FLAG_TEMP_DISABLE_RANDOMIZER)
	#endif
	&& *species != SPECIES_NONE && *species != SPECIES_ZYGARDE_CELL && *species < NUM_SPECIES)
	{
		u16 newSpecies;
		u32 id = T1_READ_32(gSaveBlock2->playerTrainerId);
		u16 startAt = (id & 0xFFFF) % (u32) speciesCount;
		u16 xorVal = (id >> 16) % (u32) 0x400; //Only set the bits likely to be in the species
		u32 numAttempts = 0;

		newSpecies = *species + startAt;
		if (newSpecies >= speciesCount)
		{
			u16 overflow = newSpecies - (speciesCount - 2);
			newSpecies = overflow;
		}

		newSpecies ^= xorVal;
		newSpecies %= (u32) speciesCount; //Prevent overflow
		
		while (gSpecialSpeciesFlags[newSpecies].randomizerBan && numAttempts < 100)
		{
			newSpecies *= xorVal;
			newSpecies %= (u32) speciesCount;
			++numAttempts;
		}

		if (numAttempts >= 100)
			newSpecies = SPECIES_DITTO;

		*species = newSpecies;
	}
	#endif
}

u16 IsSpeciesBannedFromRandomizer(u16 species) //Exported
{
	return gSpecialSpeciesFlags[species].randomizerBan;
}

u16 GetRandomizedSpecies(u16 species)
{
	TryRandomizeSpecies(&species);
	return species;
}

void CreateBoxMon(struct BoxPokemon* boxMon, u16 species, u8 level, u8 fixedIV, bool8 hasFixedPersonality, u32 fixedPersonality, u8 otIdType, u32 fixedOtId)
{
	u32 i, personality, value;
	u8 speciesName[POKEMON_NAME_LENGTH + 1];

	TryRandomizeSpecies(&species);

	ZeroBoxMonData(boxMon);
	if (hasFixedPersonality)
		personality = fixedPersonality;
	else
		personality = Random32();

	SetBoxMonData(boxMon, MON_DATA_PERSONALITY, &personality);

	//Determine original trainer ID
	if (otIdType == OT_ID_RANDOM_NO_SHINY) //Pokemon cannot be shiny
	{
		do
		{
			value = Random32();
		} while (IsShinyOtIdPersonality(value, personality));
	}
	else if (otIdType == OT_ID_PRESET) //Pokemon has a preset OT ID
		value = fixedOtId;
	else //Player is the OT
		value = T1_READ_32(gSaveBlock2->playerTrainerId);

	SetBoxMonData(boxMon, MON_DATA_OT_ID, &value);

	GetSpeciesName(speciesName, species);
	SetBoxMonData(boxMon, MON_DATA_NICKNAME, speciesName);
	SetBoxMonData(boxMon, MON_DATA_LANGUAGE, &gGameLanguage);
	SetBoxMonData(boxMon, MON_DATA_OT_NAME, gSaveBlock2->playerName);
	SetBoxMonData(boxMon, MON_DATA_SPECIES, &species);
	SetBoxMonData(boxMon, MON_DATA_EXP, &gExperienceTables[gBaseStats[species].growthRate][level]);
	SetBoxMonData(boxMon, MON_DATA_FRIENDSHIP, &gBaseStats[species].friendship);
	value = GetCurrentRegionMapSectionId();
	SetBoxMonData(boxMon, MON_DATA_MET_LOCATION, &value);
	SetBoxMonData(boxMon, MON_DATA_MET_LEVEL, &level);
	SetBoxMonData(boxMon, MON_DATA_MET_GAME, &gGameVersion);
	value = BALL_TYPE_POKE_BALL;
	SetBoxMonData(boxMon, MON_DATA_POKEBALL, &value);
	SetBoxMonData(boxMon, MON_DATA_OT_GENDER, &gSaveBlock2->playerGender);

	if (IsGigantamaxSpecies(species))
		boxMon->substruct3.gigantamax = TRUE;

	if (fixedIV < 32)
	{
		for (i = 0; i < NUM_STATS; ++i)
			SetBoxMonData(boxMon, MON_DATA_HP_IV + i, &fixedIV);
	}
	else
	{
		u32 iv;
		u8 numPerfectStats = 0;
		bool8 perfectStats[NUM_STATS] = {0};

		//HP, Attack, Defense
		value = Random();

		//HP IV
		iv = value & 0x1F;
		SetBoxMonData(boxMon, MON_DATA_HP_IV, &iv);
		if (iv >= 31)
		{
			++numPerfectStats;
			perfectStats[STAT_HP] = TRUE;
		}

		//Attack IV
		iv = (value & 0x3E0) >> 5;
		SetBoxMonData(boxMon, MON_DATA_ATK_IV, &iv);
		if (iv >= 31)
		{
			++numPerfectStats;
			perfectStats[STAT_ATK] = TRUE;
		}

		//Defense IV
		iv = (value & 0x7C00) >> 10;
		SetBoxMonData(boxMon, MON_DATA_DEF_IV, &iv);
		if (iv >= 31)
		{
			++numPerfectStats;
			perfectStats[STAT_DEF] = TRUE;
		}

		//Speed, Sp. Atk, Sp. Def
		value = Random();

		//Speed IV
		iv = value & 0x1F;
		SetBoxMonData(boxMon, MON_DATA_SPEED_IV, &iv);
		if (iv >= 31)
		{
			++numPerfectStats;
			perfectStats[STAT_SPEED] = TRUE;
		}

		//Sp. Atk IV
		iv = (value & 0x3E0) >> 5;
		SetBoxMonData(boxMon, MON_DATA_SPATK_IV, &iv);
		if (iv >= 31)
		{
			++numPerfectStats;
			perfectStats[STAT_SPATK] = TRUE;
		}

		//Sp. Def IV
		iv = (value & 0x7C00) >> 10;
		SetBoxMonData(boxMon, MON_DATA_SPDEF_IV, &iv);
		if (iv >= 31)
		{
			++numPerfectStats;
			perfectStats[STAT_SPDEF] = TRUE;
		}

		#ifdef CREATE_WITH_X_PERFECT_IVS
		{
			if (gSpecialSpeciesFlags[species].setPerfectXIVs)
			{
				u8 perfect = 31;
				u8 perfectStatCount = MathMin(CREATE_WITH_X_PERFECT_IVS, NUM_STATS); //Error prevention

				while (numPerfectStats < perfectStatCount) //Harder to get more than 3 perfect since perfect stats from before are already taken into account
				{
					u8 statId = Random() % NUM_STATS;
					if (!perfectStats[statId]) //Must be unique
					{
						perfectStats[statId] = TRUE;
						++numPerfectStats;
						SetBoxMonData(boxMon, MON_DATA_HP_IV + statId, &perfect);
					}
				}
			}
		}
		#endif
	}

	((struct Pokemon*) boxMon)->hiddenAbility = FALSE; //Set base hidden ability to 0
	SetBoxMonData(boxMon, MON_DATA_PERSONALITY, &personality);

	if (otIdType != OT_ID_RANDOM_NO_SHINY) //Pokemon can be shiny
		CheckShinyMon((struct Pokemon*) boxMon); //Activate Shiny Charm or fishing chain

	GiveBoxMonInitialMoveset(boxMon);
	TrySetCorrectToxtricityForm(boxMon);
}

void CreateMonWithNatureLetter(struct Pokemon* mon, u16 species, u8 level, u8 fixedIV, u8 nature, u8 letter)
{
	u32 personality;
	letter -= 1;

	#ifdef SPECIES_UNOWN
	if ((u8)(letter) < 28 && species == SPECIES_UNOWN)
	{
		u16 actualLetter;

		do
		{
			personality = Random32();
			actualLetter = GetUnownLetterFromPersonality(personality);
		}
		while (nature != GetNatureFromPersonality(personality) || actualLetter != letter);

		CreateMon(mon, species, level, fixedIV, TRUE, personality, OT_ID_PLAYER_ID, 0);
	}
	else
	#endif
	{
		CreateMonWithNature(mon, species, level, 32, nature);
	}
}

void TryStatusInducer(unusedArg struct Pokemon* mon)
{
	#ifdef VAR_STATUS_INDUCER
	if (VarGet(VAR_STATUS_INDUCER))
	{
		u8 status = VarGet(VAR_STATUS_INDUCER) & 0xFF; //Lowest byte is status
		mon->condition = status;
	}
	#endif
}

bool8 GetAlternateHasSpecies(struct BoxPokemon* mon)
{
	return GetBoxMonData(mon, MON_DATA_SPECIES, NULL) != SPECIES_NONE && !GetBoxMonData(mon, MON_DATA_SANITY_IS_BAD_EGG, NULL);
}

#define CALC_STAT(base, iv, ev, statIndex, field)			\
{															\
	u32 n = (((2 * base + iv + ev / 4) * level) / 100) + 5; \
	n = ModifyStatByNature(nature, n, statIndex);			\
	SetMonData(mon, field, &n);								\
}

void CalculateMonStatsNew(struct Pokemon *mon)
{
	u32 i;
	u16 newMaxHP;
	u8 ivs[NUM_STATS] = {0};
	u8 evs[NUM_STATS] = {0};

	for (i = STAT_HP; i < NUM_STATS; ++i)
	{
		ivs[i] = GetMonData(mon, MON_DATA_HP_IV + i, NULL);
		evs[i] = GetMonData(mon, MON_DATA_HP_EV + i, NULL);
	}

	u16 species = GetMonData(mon, MON_DATA_SPECIES, NULL);
	u16 currentHP = GetMonData(mon, MON_DATA_HP, NULL);
	u16 oldMaxHP = GetMonData(mon, MON_DATA_MAX_HP, NULL);
	u8 level = GetLevelFromMonExp(mon);
	u32 baseStatTotal = GetBaseStatsTotal(species);
	u8 baseHP;

	if (IsAverageMonsBattle()) //All base stats are 100
	{
		baseHP = 100;
	}
	else
	{
		baseHP = gBaseStats[species].baseHP;

		if (Is350CupBattle() && baseStatTotal <= 350)
			baseHP *= 2;
	}

	SetMonData(mon, MON_DATA_LEVEL, &level);

	#ifdef SPECIES_SHEDINJA
	if (species == SPECIES_SHEDINJA)
	{
		newMaxHP = 1;
	}
	else
	#endif
	{
		u32 n = 2 * baseHP + ivs[STAT_HP];
		newMaxHP = MathMin((((n + evs[STAT_HP] / 4) * level) / 100) + level + 10, 0xFFFF);
	}

	gBattleScripting.field_23 = newMaxHP - oldMaxHP;
	if (gBattleScripting.field_23 == 0)
		gBattleScripting.field_23 = 1;

	SetMonData(mon, MON_DATA_MAX_HP, &newMaxHP);

	u8 nature = GetNature(mon);
	if (IsScaleMonsBattle() && IsSpeciesAffectedByScalemons(species))
	{
		for (i = STAT_ATK; i < NUM_STATS; ++i) //HP doesn't change in Scalemons
		{
			u16 base = ((u8*) (&gBaseStats[species].baseHP))[i];
			base = MathMin((base * (600 - baseHP)) / (baseStatTotal - baseHP), 255); //Max 255
			CALC_STAT(base, ivs[i], evs[i], i, MON_DATA_ATK + (i - 1));
		}
	}
	else if (IsAverageMonsBattle())
	{
		for (i = STAT_ATK; i < NUM_STATS; ++i) //HP doesn't change in Scalemons
		{
			u16 base = 100;
			CALC_STAT(base, ivs[i], evs[i], i, MON_DATA_ATK + (i - 1));
		}
	}
	else if (Is350CupBattle() && baseStatTotal <= 350)
	{
		for (i = STAT_ATK; i < NUM_STATS; ++i) //HP doesn't change in Scalemons
		{
			u16 base = ((u8*) (&gBaseStats[species].baseHP))[i] * 2;
			CALC_STAT(base, ivs[i], evs[i], i, MON_DATA_ATK + (i - 1));
		}
	}
	else
	{
		for (i = STAT_ATK; i < NUM_STATS; ++i)
		{
			u16 base = ((u8*) (&gBaseStats[species].baseHP))[i];
			CALC_STAT(base, ivs[i], evs[i], i, MON_DATA_ATK + (i - 1));
		}
	}

	#ifdef SPECIES_SHEDINJA
	if (species == SPECIES_SHEDINJA)
	{
		if (currentHP != 0 || oldMaxHP == 0)
			currentHP = 1;
		else
			return;
	}
	else
	#endif
	{
		if (currentHP == 0 && oldMaxHP == 0)
			currentHP = newMaxHP;
		else if (currentHP != 0 && newMaxHP >= oldMaxHP) //To prevent garbage data after Dynamax form change
			currentHP += newMaxHP - oldMaxHP;
		else
			return;

		if (!gMain.inBattle && currentHP > newMaxHP)
			currentHP = newMaxHP;
	}

	SetMonData(mon, MON_DATA_HP, &currentHP);
}

u8 GetMonAbility(const struct Pokemon* mon)
{
	u8 ability;
	u16 species = GetMonData(mon, MON_DATA_SPECIES, NULL);

	if (mon->hiddenAbility && gBaseStats[species].hiddenAbility != ABILITY_NONE)
		return GetHiddenAbility(species);

	u32 personality = GetMonData(mon, MON_DATA_PERSONALITY, NULL);
	if ((personality & 1) == 0 || gBaseStats[species].ability2 == ABILITY_NONE)
		ability = GetAbility1(species);
	else
		ability = GetAbility2(species);

	return ability;
}

void SetMonExpWithMaxLevelCheck(struct Pokemon *mon, u16 species, unusedArg u8 unused, u32 data)
{
    if (data > gExperienceTables[gBaseStats[species].growthRate][MAX_LEVEL])
    {
        data = gExperienceTables[gBaseStats[species].growthRate][MAX_LEVEL];
        SetMonData(mon, MON_DATA_EXP, &data);
    }
}

void RandomlyGivePartyPokerus(struct Pokemon *party)
{
	u16 rnd = Random();
	if (rnd == 0x4000 || rnd == 0x8000 || rnd == 0xC000)
	{
		struct Pokemon *mon;

		do
		{
			do
			{
				rnd = Random() % PARTY_SIZE;
				mon = &party[rnd];
			}
			while (!GetMonData(mon, MON_DATA_SPECIES, 0));
		}
		while (GetMonData(mon, MON_DATA_IS_EGG, 0));

		if (!(CheckPartyHasHadPokerus(party, gBitTable[rnd])))
		{
			u8 rnd2;

			do
			{
				rnd2 = Random();
			}
			while ((rnd2 & 0x7) == 0);

			if (rnd2 & 0xF0)
				rnd2 &= 0x7;

			rnd2 |= (rnd2 << 4);
			rnd2 &= 0xF3;
			rnd2++;

			SetMonData(&party[rnd], MON_DATA_POKERUS, &rnd2);
		}
	}
}

void UpdatePartyPokerusTime(u16 days)
{
    int i;
    for (i = 0; i < PARTY_SIZE; i++)
    {
        if (GetMonData(&gPlayerParty[i], MON_DATA_SPECIES, NULL))
        {
            u8 pokerus = GetMonData(&gPlayerParty[i], MON_DATA_POKERUS, NULL);
            if (pokerus & 0xF)
            {
                if ((pokerus & 0xF) < days || days > 4)
                    pokerus &= 0xF0;
                else
                    pokerus -= days;

                if (pokerus == 0)
                    pokerus = 0x10;

                SetMonData(&gPlayerParty[i], MON_DATA_POKERUS, &pokerus);
            }
        }
    }
}

void PartySpreadPokerus(struct Pokemon *party)
{
	if ((Random() % 3) == 0)
	{
		int i;
		for (i = 0; i < PARTY_SIZE; i++)
		{
			if (GetMonData(&party[i], MON_DATA_SPECIES, NULL))
			{
				u8 pokerus = GetMonData(&party[i], MON_DATA_POKERUS, NULL);
				u8 curPokerus = pokerus;
				if (pokerus)
				{
					if (pokerus & 0xF)
					{
						// Spread to adjacent party members.
						if (i != 0 && !(GetMonData(&party[i - 1], MON_DATA_POKERUS, NULL) & 0xF0))
							SetMonData(&party[i - 1], MON_DATA_POKERUS, &curPokerus);
						if (i != (PARTY_SIZE - 1) && !(GetMonData(&party[i + 1], MON_DATA_POKERUS, NULL) & 0xF0))
						{
							SetMonData(&party[i + 1], MON_DATA_POKERUS, &curPokerus);
							i++;
						}
					}
				}
			}
		}
	}
}
