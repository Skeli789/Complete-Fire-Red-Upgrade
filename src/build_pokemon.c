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
#include "../include/new/terastallization.h"
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
static u8 GetTrainerMonGender(const struct Trainer* trainer);
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
static void SetEVSpread(struct Pokemon* mon, u8 hp, u8 atk, u8 def, u8 spa, u8 spdef, u8 spd);
static void SetIVSpread(struct Pokemon* mon, u8 hp, u8 atk, u8 def, u8 spa, u8 spdef, u8 spd);
static void SetAbilityFromEnum(struct Pokemon* mon, u8 abilityNum, u8 natureNum);

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

	#ifdef MID_BATTLE_EVO
	gPlayerDoesNotWantToEvolveLeft = FALSE;
    gPlayerDoesNotWantToEvolveRight = FALSE;
	#endif
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

	const struct Trainer* trainer;
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
			if ((firstTrainer && gTrainerBattleOpponent_A < DYNAMIC_TRAINER_LIMIT && class != CLASS_RIVAL && class != CLASS_RIVAL_2)
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
			u8 class = trainer->trainerClass;
			if (gTrainerBattleOpponent_A < DYNAMIC_TRAINER_LIMIT && class != CLASS_RIVAL && class != CLASS_RIVAL_2)
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
			u8 class = trainer->trainerClass;
			u8 openWorldSpeciesIndex = GetOpenWorldSpeciesIndex(nameHash, i);
			u8 openWorldLevel = GetOpenWorldSpeciesLevel(nameHash, i);

			if (gTrainerBattleOpponent_A < DYNAMIC_TRAINER_LIMIT && class != CLASS_RIVAL && class != CLASS_RIVAL_2)
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
						SetAbilityFromEnum(&party[i], trainer->party.ItemCustomMoves[i].ability, trainer->party.ItemCustomMoves[i].nature);
						SetMonData(mon, MON_DATA_HELD_ITEM, &trainer->party.ItemCustomMoves[i].heldItem);
						SetEVSpread(&party[i],
							trainer->party.ItemCustomMoves[i].evSpread[0],
							trainer->party.ItemCustomMoves[i].evSpread[1],
							trainer->party.ItemCustomMoves[i].evSpread[2],
							trainer->party.ItemCustomMoves[i].evSpread[3],
							trainer->party.ItemCustomMoves[i].evSpread[4],
							trainer->party.ItemCustomMoves[i].evSpread[5]
						);
						SetIVSpread(&party[i],
							trainer->party.ItemCustomMoves[i].ivSpread[0],
							trainer->party.ItemCustomMoves[i].ivSpread[1],
							trainer->party.ItemCustomMoves[i].ivSpread[2],
							trainer->party.ItemCustomMoves[i].ivSpread[3],
							trainer->party.ItemCustomMoves[i].ivSpread[4],
							trainer->party.ItemCustomMoves[i].ivSpread[5]
						);
						if (trainer->partyFlags & PARTY_FLAG_CUSTOM_MOVES) {
    						mon->teraType = trainer->party.ItemCustomMoves[i].teraType;
						}
						break;
				}

				// Assign random teraType based on original types
				u16 species = GetMonData(mon, MON_DATA_SPECIES, NULL);
				u8 type1 = gBaseStats[species].type1;
				u8 type2 = gBaseStats[species].type2;
				u8 randomValue = Random() % 100;

				// 2% chance to get a random teraType
				if (randomValue < 2)
					mon->teraType = GetRandomTeraType();

				// Otherwise, get a random one from the original typing
				else
				{
					if (type1 == type2 || type2 == TYPE_MYSTERY || type2 == TYPE_BLANK)
						mon->teraType = type1;
					else
						mon->teraType = (Random() & 1) ? type1 : type2;				
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
						GiveMonNatureAndAbility(mon, spread->nature, 0xFF, spread->shiny, TRUE, FALSE); //Give Hidden Ability
						break;
					case Ability_1:
					case Ability_2:
						GiveMonNatureAndAbility(mon, spread->nature, MathMin(1, spread->ability - 1), spread->shiny, TRUE, FALSE);
						break;
					case Ability_Random_1_2:
					TRAINER_WITH_EV_GIVE_RANDOM_ABILITY:
						GiveMonNatureAndAbility(mon, spread->nature, 0xFF, spread->shiny, TRUE, FALSE);
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

				// Try assigning teraType
				if (spread->teraType == TERA_TYPE_RANDOM_ALL)
					mon->teraType = GetRandomTeraType();
				else if (spread->teraType != TERA_TYPE_RANDOM) // We skip 0xFF because it has already been assigned prior
					mon->teraType = spread->teraType; // Set teraType to designated value
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

static u8 GetTrainerMonGender(const struct Trainer* trainer)
{
	switch (trainer->trainerClass)
	{
		case CLASS_TEAM_ROCKET: //Otherwise all Grunts have the same genders
		case CLASS_LEADER:
		case CLASS_ELITE_FOUR:
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

bool8 ShouldGiveTrainerMonBestStatsMaxEVs(u8 trainerClass)
{
#ifdef VAR_GAME_DIFFICULTY
    u8 difficulty = VarGet(VAR_GAME_DIFFICULTY);

    // Only apply on Hard and above
    if (difficulty < OPTIONS_HARD_DIFFICULTY)
        return FALSE;

    switch (trainerClass)
    {
        case CLASS_LEADER:
        case CLASS_ELITE_FOUR:
        case CLASS_CHAMPION:
        case CLASS_RIVAL:
        case CLASS_RIVAL_2:
        case CLASS_BOSS:
        #ifdef UNBOUND
        case CLASS_SUCCESSOR:
        case CLASS_LOR:
        case CLASS_LOR_ADMIN:
        case CLASS_LOR_LEADER:
        #endif
            return TRUE;
        default:
            return FALSE;
    }
#else
    return FALSE; // No difficulty system = safe default
#endif
}

bool8 ShouldGiveTrainerMonMaxFriendship(u8 trainerClass)
{
#ifdef VAR_GAME_DIFFICULTY
    u8 difficulty = VarGet(VAR_GAME_DIFFICULTY);

    // Only apply on Hard and above
    if (difficulty < OPTIONS_HARD_DIFFICULTY)
        return FALSE;

    switch (trainerClass)
    {
        case CLASS_CHAMPION:
        case CLASS_RIVAL:
        case CLASS_RIVAL_2:
        case CLASS_LEADER:
        case CLASS_ELITE_FOUR:
            return TRUE;
        default:
            return FALSE;
    }
#else
    return FALSE; // No difficulty system = safe default
#endif
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
		case CLASS_ELITE_FOUR:
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
				if (SpeciesHasEarthEater(SPECIES(gBankTarget)))
					builder->partyIndex[GROUND_IMMUNITY] = partyId;
				else
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
			if ((moveType == TYPE_ELECTRIC && !SpeciesHasEarthEater(SPECIES(gBankTarget)))
			|| (moveType == TYPE_GROUND && SpeciesHasEarthEater(SPECIES(gBankTarget))))
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
		case 9:
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
#ifdef OPEN_WORLD_TRAINERS
// Trainers will be based on these levels only if OPEN_WORLD_TRAINERS
const u8 gOpenWorldLevelRanges[NUM_BADGE_OPTIONS][2] = {
    { 5,  8},   // 0 badges
    { 9, 13},   // 1 badge
    {14, 18},   // 2 badges
    {19, 25},   // 3 badges
    {26, 33},   // 4 badges
    {34, 40},   // 5 badges
    {41, 48},   // 6 badges
    {49, 55},   // 7 badges
    {56, 65},   // 8 badges
    {66, 75},   // 9 badges (Post-League)
};
// Trainers will be based on badge count and class based on this, only edit if you're trying making it OPEN_WORLD
const species_t gGeneralTrainerSpreads[NUM_TRAINER_CLASSES][NUM_BADGE_OPTIONS][NUM_MONS_PER_BADGE] = {
    [CLASS_PKMN_TRAINER_1] = {
        [0] = {SPECIES_DUDUNSPARCE_THREE, SPECIES_LECHONK, SPECIES_SQUAWKABILLY_YELLOW, SPECIES_OINKOLOGNE, SPECIES_PATRAT, SPECIES_DEERLING_WINTER, SPECIES_ZORUA_H, SPECIES_TAILLOW, SPECIES_LILLIPUP, SPECIES_RATTATA_A},
        [1] = {SPECIES_GRAFAIAI, SPECIES_EGG, SPECIES_OINKOLOGNE_FEMALE, SPECIES_YUNGOOS, SPECIES_BIDOOF, SPECIES_SENTRET, SPECIES_PIDOVE, SPECIES_DEERLING_AUTUMN, SPECIES_RATTATA_A, SPECIES_TERAPAGOS_TERASTAL},
        [2] = {SPECIES_PIKIPEK, SPECIES_DUDUNSPARCE, SPECIES_DEERLING, SPECIES_DEERLING_WINTER, SPECIES_RATTATA_A, SPECIES_OINKOLOGNE_FEMALE, SPECIES_ARBOLIVA, SPECIES_PIDGEY, SPECIES_SQUAWKABILLY_BLUE, SPECIES_YUNGOOS},
        [3] = {SPECIES_TYPE_NULL, SPECIES_SMEARGLE, SPECIES_AIPOM, SPECIES_EEVEE, SPECIES_CASTFORM, SPECIES_VIGOROTH, SPECIES_PORYGON, SPECIES_LINOONE_G, SPECIES_RUFFLET, SPECIES_DUNSPARCE},
        [4] = {SPECIES_RATICATE, SPECIES_TRANQUILL, SPECIES_WOOLOO, SPECIES_SWABLU, SPECIES_PIDGEOTTO, SPECIES_CASTFORM, SPECIES_WATCHOG, SPECIES_LOUDRED, SPECIES_LINOONE_G, SPECIES_FARFETCHD},
        [5] = {SPECIES_STUFFUL, SPECIES_FARFETCHD, SPECIES_DUNSPARCE, SPECIES_KECLEON, SPECIES_LITLEO, SPECIES_RATICATE, SPECIES_BUNEARY, SPECIES_SWABLU, SPECIES_LINOONE_G, SPECIES_HERDIER},
        [6] = {SPECIES_AUDINO, SPECIES_SNORLAX, SPECIES_CHANSEY, SPECIES_INDEEDEE, SPECIES_DUBWOOL, SPECIES_AUDINO_MEGA, SPECIES_GREEDENT, SPECIES_PORYGON2, SPECIES_WYRDEER, SPECIES_PORYGON_Z},
        [7] = {SPECIES_STANTLER, SPECIES_PYROAR_FEMALE, SPECIES_KOMALA, SPECIES_MELOETTA_PIROUETTE, SPECIES_FURFROU_DIAMOND, SPECIES_CHANSEY, SPECIES_FURFROU_LA_REINE, SPECIES_OBSTAGOON, SPECIES_MILTANK, SPECIES_UNFEZANT_F},
        [8] = {SPECIES_CHANSEY, SPECIES_TAUROS, SPECIES_SLAKING, SPECIES_STOUTLAND, SPECIES_FURFROU_STAR, SPECIES_BRAVIARY, SPECIES_BEWEAR, SPECIES_FURFROU_KABUKI, SPECIES_FURFROU_DEBUTANTE, SPECIES_PYROAR},
        [9] = {SPECIES_STARAPTOR, SPECIES_FURFROU_KABUKI, SPECIES_OBSTAGOON, SPECIES_HELIOLISK, SPECIES_UNFEZANT_F, SPECIES_SNORLAX_GIGA, SPECIES_BEWEAR, SPECIES_KANGASKHAN, SPECIES_PORYGON2, SPECIES_AMBIPOM},
    },
    [CLASS_PKMN_TRAINER_2] = {
        [0] = {SPECIES_SKWOVET, SPECIES_SQUAWKABILLY_BLUE, SPECIES_MEOWTH_GIGA, SPECIES_SQUAWKABILLY_YELLOW, SPECIES_SQUAWKABILLY, SPECIES_FARIGIRAF, SPECIES_SMOLIV, SPECIES_HOOTHOOT, SPECIES_EGG, SPECIES_TANDEMAUS},
        [1] = {SPECIES_YUNGOOS, SPECIES_TAILLOW, SPECIES_EGG, SPECIES_SKWOVET, SPECIES_SQUAWKABILLY_YELLOW, SPECIES_URSALUNA_BLOODMOON, SPECIES_PIDOVE, SPECIES_SKITTY, SPECIES_LECHONK, SPECIES_DEERLING_AUTUMN},
        [2] = {SPECIES_LECHONK, SPECIES_IGGLYBUFF, SPECIES_HOOTHOOT, SPECIES_ZIGZAGOON_G, SPECIES_RATTATA, SPECIES_PIDGEY, SPECIES_LILLIPUP, SPECIES_SHROODLE, SPECIES_PIDOVE, SPECIES_DEERLING_SUMMER},
        [3] = {SPECIES_LITLEO, SPECIES_EEVEE, SPECIES_DODUO, SPECIES_PORYGON, SPECIES_FARFETCHD, SPECIES_PIDGEOTTO, SPECIES_AIPOM, SPECIES_TEDDIURSA, SPECIES_PERSIAN, SPECIES_RATICATE},
        [4] = {SPECIES_LOUDRED, SPECIES_PERSIAN, SPECIES_SPINDA, SPECIES_SLAKOTH, SPECIES_TEDDIURSA, SPECIES_SWABLU, SPECIES_DELCATTY, SPECIES_EEVEE, SPECIES_WIGGLYTUFF, SPECIES_TRUMBEAK},
        [5] = {SPECIES_TRANQUILL, SPECIES_CHATOT, SPECIES_SLAKOTH, SPECIES_RATICATE, SPECIES_JIGGLYPUFF, SPECIES_VIGOROTH, SPECIES_BIBAREL, SPECIES_SPINDA, SPECIES_TRUMBEAK, SPECIES_MUNCHLAX},
        [6] = {SPECIES_WYRDEER, SPECIES_LOPUNNY, SPECIES_BOUFFALANT, SPECIES_FURFROU_DEBUTANTE, SPECIES_INDEEDEE, SPECIES_FURFROU_DIAMOND, SPECIES_INDEEDEE_FEMALE, SPECIES_GREEDENT, SPECIES_SAWSBUCK_WINTER, SPECIES_FURFROU_STAR},
        [7] = {SPECIES_MELOETTA_PIROUETTE, SPECIES_UNFEZANT_F, SPECIES_PYROAR_FEMALE, SPECIES_SILVALLY, SPECIES_FURFROU_PHAROAH, SPECIES_AMBIPOM, SPECIES_SAWSBUCK_WINTER, SPECIES_GREEDENT, SPECIES_MILTANK, SPECIES_SAWSBUCK_AUTUMN},
        [8] = {SPECIES_PIDGEOT, SPECIES_INDEEDEE_FEMALE, SPECIES_PORYGON2, SPECIES_SNORLAX, SPECIES_ORANGURU, SPECIES_ZOROARK_H, SPECIES_PYROAR, SPECIES_FURFROU_STAR, SPECIES_FURFROU, SPECIES_SAWSBUCK_SUMMER},
        [9] = {SPECIES_FEAROW, SPECIES_CINCCINO, SPECIES_HAPPINY, SPECIES_SNORLAX_GIGA, SPECIES_KOMALA, SPECIES_BEWEAR, SPECIES_DODRIO, SPECIES_TAUROS, SPECIES_FURFROU_DANDY, SPECIES_PORYGON_Z},
    },
    [CLASS_AQUA_LEADER] = {
        [0] = {SPECIES_HOOTHOOT, SPECIES_OINKOLOGNE_FEMALE, SPECIES_PIDGEY, SPECIES_OINKOLOGNE, SPECIES_SHROODLE, SPECIES_HELIOPTILE, SPECIES_MINCCINO, SPECIES_TERAPAGOS, SPECIES_AZURILL, SPECIES_PIKIPEK},
        [1] = {SPECIES_CYCLIZAR, SPECIES_SQUAWKABILLY_WHITE, SPECIES_SENTRET, SPECIES_MAUSHOLD_FOUR, SPECIES_SPEAROW, SPECIES_ZIGZAGOON, SPECIES_HELIOPTILE, SPECIES_MEOWTH, SPECIES_OINKOLOGNE_FEMALE, SPECIES_PIKIPEK},
        [2] = {SPECIES_DUDUNSPARCE_THREE, SPECIES_ZIGZAGOON, SPECIES_SQUAWKABILLY_BLUE, SPECIES_DEERLING, SPECIES_DEERLING_WINTER, SPECIES_RATTATA, SPECIES_LECHONK, SPECIES_MAUSHOLD_FOUR, SPECIES_DITTO, SPECIES_FLETCHLING},
        [3] = {SPECIES_EEVEE, SPECIES_DIGGERSBY, SPECIES_CASTFORM, SPECIES_RATICATE, SPECIES_SLAKOTH, SPECIES_WOOLOO, SPECIES_BUNEARY, SPECIES_WIGGLYTUFF, SPECIES_GIRAFARIG, SPECIES_GLAMEOW},
        [4] = {SPECIES_SPINDA, SPECIES_TRANQUILL, SPECIES_FURRET, SPECIES_SWABLU, SPECIES_EEVEE, SPECIES_JIGGLYPUFF, SPECIES_STARAVIA, SPECIES_LOUDRED, SPECIES_CHATOT, SPECIES_LINOONE_G},
        [5] = {SPECIES_VIGOROTH, SPECIES_AIPOM, SPECIES_SWABLU, SPECIES_TRUMBEAK, SPECIES_LOUDRED, SPECIES_EEVEE, SPECIES_SLAKOTH, SPECIES_SMEARGLE, SPECIES_JIGGLYPUFF, SPECIES_MUNCHLAX},
        [6] = {SPECIES_SILVALLY, SPECIES_INDEEDEE, SPECIES_UNFEZANT, SPECIES_MILTANK, SPECIES_ARCEUS, SPECIES_TOUCANNON, SPECIES_STOUTLAND, SPECIES_PORYGON2, SPECIES_FURFROU_STAR, SPECIES_FURFROU_LA_REINE},
        [7] = {SPECIES_ARCEUS, SPECIES_MILTANK, SPECIES_PORYGON_Z, SPECIES_AUDINO, SPECIES_STARAPTOR, SPECIES_ZOROARK_H, SPECIES_BOUFFALANT, SPECIES_UNFEZANT, SPECIES_URSARING, SPECIES_FURFROU_MATRON},
        [8] = {SPECIES_FURFROU_HEART, SPECIES_FURFROU_STAR, SPECIES_FURFROU_DANDY, SPECIES_PURUGLY, SPECIES_SAWSBUCK_SUMMER, SPECIES_MELOETTA_PIROUETTE, SPECIES_SAWSBUCK_AUTUMN, SPECIES_FURFROU, SPECIES_LOPUNNY, SPECIES_FURFROU_LA_REINE},
        [9] = {SPECIES_MELOETTA, SPECIES_SNORLAX, SPECIES_BRAVIARY, SPECIES_LICKILICKY, SPECIES_INDEEDEE, SPECIES_WYRDEER, SPECIES_ORANGURU, SPECIES_FURFROU_DEBUTANTE, SPECIES_AUDINO_MEGA, SPECIES_URSALUNA},
    },
    [CLASS_TEAM_AQUA] = {
        [0] = {SPECIES_SMOLIV, SPECIES_SKWOVET, SPECIES_TERAPAGOS, SPECIES_BUNNELBY, SPECIES_SQUAWKABILLY_YELLOW, SPECIES_DEERLING_AUTUMN, SPECIES_HOOTHOOT, SPECIES_DEERLING, SPECIES_FARIGIRAF, SPECIES_CYCLIZAR},
        [1] = {SPECIES_MINCCINO, SPECIES_TERAPAGOS_STELLAR, SPECIES_TAILLOW, SPECIES_MAUSHOLD, SPECIES_GRAFAIAI, SPECIES_HELIOPTILE, SPECIES_PATRAT, SPECIES_HOOTHOOT, SPECIES_ZIGZAGOON, SPECIES_PIDGEY},
        [2] = {SPECIES_TERAPAGOS_TERASTAL, SPECIES_DITTO, SPECIES_MAUSHOLD_FOUR, SPECIES_TERAPAGOS, SPECIES_BIDOOF, SPECIES_BUNNELBY, SPECIES_SQUAWKABILLY_BLUE, SPECIES_OINKOLOGNE, SPECIES_SENTRET, SPECIES_IGGLYBUFF},
        [3] = {SPECIES_PORYGON, SPECIES_LOUDRED, SPECIES_VIGOROTH, SPECIES_FURRET, SPECIES_CASTFORM, SPECIES_DIGGERSBY, SPECIES_KECLEON, SPECIES_PERSIAN, SPECIES_DUNSPARCE, SPECIES_GLAMEOW},
        [4] = {SPECIES_FURRET, SPECIES_LINOONE, SPECIES_DELCATTY, SPECIES_FARFETCHD, SPECIES_TRANQUILL, SPECIES_VIGOROTH, SPECIES_SPINDA, SPECIES_RUFFLET, SPECIES_PORYGON, SPECIES_TYPE_NULL},
        [5] = {SPECIES_VIGOROTH, SPECIES_STARAVIA, SPECIES_TRANQUILL, SPECIES_FARFETCHD, SPECIES_DIGGERSBY, SPECIES_LINOONE_G, SPECIES_TRUMBEAK, SPECIES_SMEARGLE, SPECIES_KECLEON, SPECIES_TEDDIURSA},
        [6] = {SPECIES_LOPUNNY, SPECIES_SLAKING, SPECIES_STARAPTOR, SPECIES_OBSTAGOON, SPECIES_FURFROU_PHAROAH, SPECIES_BOUFFALANT, SPECIES_FURFROU_HEART, SPECIES_SAWSBUCK_WINTER, SPECIES_PURUGLY, SPECIES_BLISSEY},
        [7] = {SPECIES_INDEEDEE_FEMALE, SPECIES_BOUFFALANT, SPECIES_CHANSEY, SPECIES_PORYGON_Z, SPECIES_PIDGEOT, SPECIES_WYRDEER, SPECIES_UNFEZANT_F, SPECIES_SAWSBUCK_AUTUMN, SPECIES_STARAPTOR, SPECIES_KANGASKHAN},
        [8] = {SPECIES_PURUGLY, SPECIES_BOUFFALANT, SPECIES_SAWSBUCK_AUTUMN, SPECIES_FURFROU_MATRON, SPECIES_SNORLAX_GIGA, SPECIES_BEWEAR, SPECIES_PORYGON_Z, SPECIES_SLAKING, SPECIES_PORYGON2, SPECIES_ARCEUS},
        [9] = {SPECIES_FURFROU_LA_REINE, SPECIES_AUDINO_MEGA, SPECIES_BLISSEY, SPECIES_MELOETTA_PIROUETTE, SPECIES_SAWSBUCK_WINTER, SPECIES_PIDGEOT_MEGA, SPECIES_MELOETTA, SPECIES_KANGASKHAN_MEGA, SPECIES_ZANGOOSE, SPECIES_FURFROU_PHAROAH},
    },
    [CLASS_AROMA_LADY_RS] = {
        [0] = {SPECIES_SHROODLE, SPECIES_WHISMUR, SPECIES_GRAFAIAI, SPECIES_DITTO, SPECIES_URSALUNA_BLOODMOON, SPECIES_DOLLIV, SPECIES_HELIOPTILE, SPECIES_SMOLIV, SPECIES_PIDGEY, SPECIES_MEOWTH},
        [1] = {SPECIES_ARBOLIVA, SPECIES_DEERLING_SUMMER, SPECIES_DEERLING_WINTER, SPECIES_IGGLYBUFF, SPECIES_PIKIPEK, SPECIES_MEOWTH_GIGA, SPECIES_PATRAT, SPECIES_FLETCHLING, SPECIES_DITTO, SPECIES_MAUSHOLD},
        [2] = {SPECIES_CYCLIZAR, SPECIES_URSALUNA_BLOODMOON, SPECIES_BIDOOF, SPECIES_HOOTHOOT, SPECIES_TERAPAGOS_TERASTAL, SPECIES_SKWOVET, SPECIES_SQUAWKABILLY_YELLOW, SPECIES_IGGLYBUFF, SPECIES_TERAPAGOS, SPECIES_ZORUA_H},
        [3] = {SPECIES_STUFFUL, SPECIES_LINOONE_G, SPECIES_WATCHOG, SPECIES_CASTFORM, SPECIES_LICKITUNG, SPECIES_VIGOROTH, SPECIES_PORYGON, SPECIES_DODUO, SPECIES_BIBAREL, SPECIES_AIPOM},
        [4] = {SPECIES_FARFETCHD, SPECIES_GUMSHOOS, SPECIES_FURRET, SPECIES_SWABLU, SPECIES_DIGGERSBY, SPECIES_DODUO, SPECIES_CHATOT, SPECIES_JIGGLYPUFF, SPECIES_STUFFUL, SPECIES_KECLEON},
        [5] = {SPECIES_SLAKOTH, SPECIES_HERDIER, SPECIES_WOOLOO, SPECIES_CASTFORM, SPECIES_STUFFUL, SPECIES_TYPE_NULL, SPECIES_TEDDIURSA, SPECIES_RUFFLET, SPECIES_CHATOT, SPECIES_BIBAREL},
        [6] = {SPECIES_AUDINO, SPECIES_REGIGIGAS, SPECIES_CINCCINO, SPECIES_AUDINO_MEGA, SPECIES_URSARING, SPECIES_LOPUNNY_MEGA, SPECIES_FURFROU, SPECIES_FURFROU_MATRON, SPECIES_SAWSBUCK_WINTER, SPECIES_SAWSBUCK},
        [7] = {SPECIES_PYROAR, SPECIES_FEAROW, SPECIES_SAWSBUCK_AUTUMN, SPECIES_FURFROU_PHAROAH, SPECIES_OBSTAGOON, SPECIES_HELIOLISK, SPECIES_INDEEDEE, SPECIES_AUDINO, SPECIES_UNFEZANT_F, SPECIES_ZANGOOSE},
        [8] = {SPECIES_ORANGURU, SPECIES_CINCCINO, SPECIES_FURFROU_HEART, SPECIES_KANGASKHAN_MEGA, SPECIES_BLISSEY, SPECIES_BOUFFALANT, SPECIES_ZOROARK_H, SPECIES_DODRIO, SPECIES_HAPPINY, SPECIES_STOUTLAND},
        [9] = {SPECIES_DRAMPA, SPECIES_LOPUNNY_MEGA, SPECIES_FURFROU_HEART, SPECIES_HAPPINY, SPECIES_REGIGIGAS, SPECIES_BOUFFALANT, SPECIES_FURFROU, SPECIES_ZANGOOSE, SPECIES_AUDINO_MEGA, SPECIES_AMBIPOM},
    },
    [CLASS_RUIN_MANIAC_RS] = {
        [0] = {SPECIES_FLETCHLING, SPECIES_PIKIPEK, SPECIES_MEOWTH, SPECIES_TERAPAGOS_STELLAR, SPECIES_MINCCINO, SPECIES_PATRAT, SPECIES_ZIGZAGOON_G, SPECIES_GRAFAIAI, SPECIES_MAUSHOLD_FOUR, SPECIES_DEERLING_SUMMER},
        [1] = {SPECIES_BIDOOF, SPECIES_CYCLIZAR, SPECIES_AZURILL, SPECIES_YUNGOOS, SPECIES_ZORUA_H, SPECIES_DEERLING_SUMMER, SPECIES_PIKIPEK, SPECIES_GRAFAIAI, SPECIES_SHROODLE, SPECIES_RATTATA_A},
        [2] = {SPECIES_CYCLIZAR, SPECIES_DITTO, SPECIES_DOLLIV, SPECIES_MINCCINO, SPECIES_BUNNELBY, SPECIES_LECHONK, SPECIES_IGGLYBUFF, SPECIES_SMOLIV, SPECIES_SQUAWKABILLY_YELLOW, SPECIES_TAILLOW},
        [3] = {SPECIES_DELCATTY, SPECIES_AIPOM, SPECIES_DUNSPARCE, SPECIES_TRANQUILL, SPECIES_FURRET, SPECIES_HERDIER, SPECIES_BUNEARY, SPECIES_RATICATE_A, SPECIES_LINOONE, SPECIES_TEDDIURSA},
        [4] = {SPECIES_CHATOT, SPECIES_EEVEE, SPECIES_LITLEO, SPECIES_WATCHOG, SPECIES_STARAVIA, SPECIES_JIGGLYPUFF, SPECIES_HERDIER, SPECIES_TEDDIURSA, SPECIES_MUNCHLAX, SPECIES_DUNSPARCE},
        [5] = {SPECIES_HERDIER, SPECIES_WOOLOO, SPECIES_LICKITUNG, SPECIES_FURRET, SPECIES_TRANQUILL, SPECIES_SMEARGLE, SPECIES_CASTFORM, SPECIES_SPINDA, SPECIES_WATCHOG, SPECIES_RATICATE},
        [6] = {SPECIES_AUDINO_MEGA, SPECIES_PYROAR, SPECIES_ZANGOOSE, SPECIES_AUDINO, SPECIES_BLISSEY, SPECIES_PORYGON2, SPECIES_PIDGEOT_MEGA, SPECIES_DUBWOOL, SPECIES_UNFEZANT_F, SPECIES_LOPUNNY_MEGA},
        [7] = {SPECIES_FURFROU_HEART, SPECIES_FURFROU_DANDY, SPECIES_AMBIPOM, SPECIES_DODRIO, SPECIES_FURFROU_KABUKI, SPECIES_PURUGLY, SPECIES_ZOROARK_H, SPECIES_MELOETTA_PIROUETTE, SPECIES_FURFROU, SPECIES_DUBWOOL},
        [8] = {SPECIES_UNFEZANT, SPECIES_HELIOLISK, SPECIES_FURFROU_PHAROAH, SPECIES_SAWSBUCK_AUTUMN, SPECIES_SILVALLY, SPECIES_PYROAR, SPECIES_INDEEDEE, SPECIES_ZOROARK_H, SPECIES_FURFROU_DANDY, SPECIES_SAWSBUCK},
        [9] = {SPECIES_LICKILICKY, SPECIES_FURFROU, SPECIES_KANGASKHAN_MEGA, SPECIES_FURFROU_DANDY, SPECIES_HELIOLISK, SPECIES_SAWSBUCK, SPECIES_FEAROW, SPECIES_KANGASKHAN, SPECIES_FURFROU_LA_REINE, SPECIES_REGIGIGAS},
    },
    [CLASS_INTERVIEWER] = {
        [0] = {SPECIES_SPEAROW, SPECIES_OINKOLOGNE_FEMALE, SPECIES_TERAPAGOS_TERASTAL, SPECIES_BIDOOF, SPECIES_DUDUNSPARCE, SPECIES_DEERLING_AUTUMN, SPECIES_WHISMUR, SPECIES_PATRAT, SPECIES_SHROODLE, SPECIES_EGG},
        [1] = {SPECIES_DEERLING_AUTUMN, SPECIES_TANDEMAUS, SPECIES_SQUAWKABILLY_WHITE, SPECIES_DOLLIV, SPECIES_SMOLIV, SPECIES_SKWOVET, SPECIES_LECHONK, SPECIES_ZIGZAGOON_G, SPECIES_MINCCINO, SPECIES_SQUAWKABILLY},
        [2] = {SPECIES_DEERLING, SPECIES_ZORUA_H, SPECIES_ZIGZAGOON_G, SPECIES_SQUAWKABILLY_WHITE, SPECIES_TERAPAGOS, SPECIES_RATTATA, SPECIES_EGG, SPECIES_SKWOVET, SPECIES_DEERLING_WINTER, SPECIES_RATTATA_A},
        [3] = {SPECIES_TEDDIURSA, SPECIES_DIGGERSBY, SPECIES_DUNSPARCE, SPECIES_LOUDRED, SPECIES_WATCHOG, SPECIES_EEVEE_GIGA, SPECIES_JIGGLYPUFF, SPECIES_RATICATE, SPECIES_GIRAFARIG, SPECIES_DELCATTY},
        [4] = {SPECIES_FURRET, SPECIES_DELCATTY, SPECIES_RATICATE, SPECIES_STUFFUL, SPECIES_WOOLOO, SPECIES_JIGGLYPUFF, SPECIES_EEVEE_GIGA, SPECIES_LICKITUNG, SPECIES_GLAMEOW, SPECIES_LOUDRED},
        [5] = {SPECIES_RATICATE, SPECIES_EEVEE, SPECIES_CASTFORM, SPECIES_DIGGERSBY, SPECIES_VIGOROTH, SPECIES_LITLEO, SPECIES_LINOONE, SPECIES_STUFFUL, SPECIES_DELCATTY, SPECIES_DUNSPARCE},
        [6] = {SPECIES_BEWEAR, SPECIES_KOMALA, SPECIES_FEAROW, SPECIES_FURFROU_KABUKI, SPECIES_FURFROU_DANDY, SPECIES_BRAVIARY, SPECIES_LOPUNNY_MEGA, SPECIES_CHANSEY, SPECIES_HAPPINY, SPECIES_PYROAR_FEMALE},
        [7] = {SPECIES_HAPPINY, SPECIES_GREEDENT, SPECIES_PIDGEOT, SPECIES_CHANSEY, SPECIES_EXPLOUD, SPECIES_STANTLER, SPECIES_ARCEUS, SPECIES_INDEEDEE_FEMALE, SPECIES_DRAMPA, SPECIES_KANGASKHAN_MEGA},
        [8] = {SPECIES_TOUCANNON, SPECIES_DUBWOOL, SPECIES_HELIOLISK, SPECIES_STARAPTOR, SPECIES_BOUFFALANT, SPECIES_SNORLAX_GIGA, SPECIES_MILTANK, SPECIES_HAPPINY, SPECIES_SAWSBUCK_AUTUMN, SPECIES_UNFEZANT},
        [9] = {SPECIES_BRAVIARY, SPECIES_HAPPINY, SPECIES_DRAMPA, SPECIES_ARCEUS, SPECIES_UNFEZANT_F, SPECIES_FURFROU_LA_REINE, SPECIES_HELIOLISK, SPECIES_FURFROU_DIAMOND, SPECIES_STANTLER, SPECIES_SNORLAX},
    },
    [CLASS_TUBER_RS_1] = {
        [0] = {SPECIES_FRILLISH_F, SPECIES_SURSKIT, SPECIES_OSHAWOTT, SPECIES_ARROKUDA, SPECIES_DONDOZO, SPECIES_WALKING_WAKE, SPECIES_CHEWTLE, SPECIES_WIMPOD, SPECIES_SOBBLE, SPECIES_TATSUGIRI},
        [1] = {SPECIES_POPPLIO, SPECIES_WALKING_WAKE, SPECIES_DEWPIDER, SPECIES_TATSUGIRI, SPECIES_MAGIKARP, SPECIES_FROAKIE, SPECIES_QUAXWELL, SPECIES_PIPLUP, SPECIES_CLAUNCHER, SPECIES_WOOPER},
        [2] = {SPECIES_POPPLIO, SPECIES_VELUZA, SPECIES_QUAXLY, SPECIES_DUCKLETT, SPECIES_SKRELP, SPECIES_IRON_BUNDLE, SPECIES_FROAKIE, SPECIES_ARROKUDA, SPECIES_BINACLE, SPECIES_OSHAWOTT},
        [3] = {SPECIES_WISHIWASHI_S, SPECIES_SPHEAL, SPECIES_WAILMER, SPECIES_OMANYTE, SPECIES_LOTAD, SPECIES_FROGADIER, SPECIES_BASCULIN_RED, SPECIES_ARAQUANID, SPECIES_CLAMPERL, SPECIES_CARVANHA},
        [4] = {SPECIES_LOTAD, SPECIES_SLOWPOKE, SPECIES_MAREANIE, SPECIES_MANTYKE, SPECIES_PANPOUR, SPECIES_QWILFISH, SPECIES_POLIWHIRL, SPECIES_TENTACOOL, SPECIES_BIBAREL, SPECIES_WAILMER},
        [5] = {SPECIES_SHELLDER, SPECIES_PALPITOAD, SPECIES_SEEL, SPECIES_ROTOM_WASH, SPECIES_WISHIWASHI, SPECIES_WAILMER, SPECIES_BASCULIN_H, SPECIES_OMANYTE, SPECIES_QWILFISH, SPECIES_BIBAREL},
        [6] = {SPECIES_KINGLER, SPECIES_CRAMORANT_GORGING, SPECIES_PHIONE, SPECIES_LAPRAS, SPECIES_GYARADOS_MEGA, SPECIES_EMPOLEON, SPECIES_JELLICENT_F, SPECIES_GASTRODON, SPECIES_PELIPPER, SPECIES_SEAKING},
        [7] = {SPECIES_VOLCANION, SPECIES_CRAMORANT_GORGING, SPECIES_BLASTOISE_GIGA, SPECIES_WAILORD, SPECIES_LAPRAS_GIGA, SPECIES_HUNTAIL, SPECIES_FLOATZEL, SPECIES_ALOMOMOLA, SPECIES_CRAWDAUNT, SPECIES_LAPRAS},
        [8] = {SPECIES_AZUMARILL, SPECIES_CARRACOSTA, SPECIES_POLITOED, SPECIES_GASTRODON, SPECIES_ARCTOVISH, SPECIES_SAMUROTT_H, SPECIES_PALKIA_ORIGIN, SPECIES_VAPOREON, SPECIES_KELDEO_RESOLUTE, SPECIES_SWANNA},
        [9] = {SPECIES_OMASTAR, SPECIES_KABUTOPS, SPECIES_PALKIA, SPECIES_DEWGONG, SPECIES_KINGLER, SPECIES_RELICANTH, SPECIES_SWAMPERT_MEGA, SPECIES_DREDNAW_GIGA, SPECIES_LAPRAS_GIGA, SPECIES_WALREIN},
    },
    [CLASS_TUBER_RS_2] = {
        [0] = {SPECIES_WIMPOD, SPECIES_QUAXLY, SPECIES_BINACLE, SPECIES_WUGTRIO, SPECIES_SURSKIT, SPECIES_FRILLISH_F, SPECIES_FRILLISH, SPECIES_MARILL, SPECIES_SHELLOS_EAST, SPECIES_FROAKIE},
        [1] = {SPECIES_IRON_BUNDLE, SPECIES_WIMPOD, SPECIES_SQUIRTLE, SPECIES_FRILLISH_F, SPECIES_DEWPIDER, SPECIES_VELUZA, SPECIES_WALKING_WAKE, SPECIES_QUAQUAVAL, SPECIES_WOOPER, SPECIES_MAGIKARP},
        [2] = {SPECIES_TATSUGIRI_RED, SPECIES_QUAXWELL, SPECIES_MUDKIP, SPECIES_BINACLE, SPECIES_SURSKIT, SPECIES_ARROKUDA, SPECIES_CLAUNCHER, SPECIES_SOBBLE, SPECIES_WIMPOD, SPECIES_PIPLUP},
        [3] = {SPECIES_FROGADIER, SPECIES_BRIONNE, SPECIES_WAILMER, SPECIES_TENTACOOL, SPECIES_SLOWPOKE, SPECIES_FINNEON, SPECIES_BASCULIN_H, SPECIES_SEALEO, SPECIES_MAREANIE, SPECIES_BASCULIN_BLUE},
        [4] = {SPECIES_HORSEA, SPECIES_PRINPLUP, SPECIES_SHELLDER, SPECIES_BARBOACH, SPECIES_WISHIWASHI, SPECIES_CLAMPERL, SPECIES_DRIZZILE, SPECIES_PANPOUR, SPECIES_ROTOM_WASH, SPECIES_TIRTOUGA},
        [5] = {SPECIES_OMANYTE, SPECIES_SHELLOS, SPECIES_SEEL, SPECIES_WAILMER, SPECIES_QUAGSIRE, SPECIES_SILVALLY_WATER, SPECIES_BASCULIN_BLUE, SPECIES_SPHEAL, SPECIES_WARTORTLE, SPECIES_GOLDEEN},
        [6] = {SPECIES_INTELEON_GIGA, SPECIES_KYOGRE_PRIMAL, SPECIES_SHARPEDO, SPECIES_SUICUNE, SPECIES_FERALIGATR, SPECIES_SHARPEDO_MEGA, SPECIES_BLASTOISE_MEGA, SPECIES_HUNTAIL, SPECIES_BARBARACLE, SPECIES_LANTURN},
        [7] = {SPECIES_MANTINE, SPECIES_SEISMITOAD, SPECIES_KYOGRE_PRIMAL, SPECIES_POLITOED, SPECIES_ALOMOMOLA, SPECIES_TENTACRUEL, SPECIES_CRAMORANT_GULPING, SPECIES_OMASTAR, SPECIES_POLIWRATH, SPECIES_KINGDRA},
        [8] = {SPECIES_CRAMORANT, SPECIES_MANTINE, SPECIES_KYOGRE, SPECIES_DREDNAW, SPECIES_BLASTOISE_MEGA, SPECIES_LUDICOLO, SPECIES_PELIPPER, SPECIES_CARRACOSTA, SPECIES_POLIWRATH, SPECIES_CLOYSTER},
        [9] = {SPECIES_SLOWKING, SPECIES_SLOWBRO_MEGA, SPECIES_GYARADOS_MEGA, SPECIES_CRAMORANT_GULPING, SPECIES_KELDEO_RESOLUTE, SPECIES_CRAMORANT_GORGING, SPECIES_INTELEON_GIGA, SPECIES_STARMIE, SPECIES_PALKIA, SPECIES_SWAMPERT},
    },
    [CLASS_COOLTRAINER_RS] = {
        [0] = {SPECIES_BIDOOF, SPECIES_STARLY, SPECIES_SKWOVET, SPECIES_PIDGEY, SPECIES_LECHONK, SPECIES_MAUSHOLD, SPECIES_SQUAWKABILLY_WHITE, SPECIES_ZIGZAGOON_G, SPECIES_DEERLING, SPECIES_DEERLING_SUMMER},
        [1] = {SPECIES_GRAFAIAI, SPECIES_ZORUA_H, SPECIES_MEOWTH, SPECIES_SPEAROW, SPECIES_RATTATA_A, SPECIES_SMOLIV, SPECIES_AZURILL, SPECIES_STARLY, SPECIES_DEERLING_SUMMER, SPECIES_PIDGEY},
        [2] = {SPECIES_YUNGOOS, SPECIES_DITTO, SPECIES_SHROODLE, SPECIES_URSALUNA_BLOODMOON, SPECIES_PIDGEY, SPECIES_SKITTY, SPECIES_IGGLYBUFF, SPECIES_ZIGZAGOON, SPECIES_TERAPAGOS_STELLAR, SPECIES_ZORUA_H},
        [3] = {SPECIES_BUNEARY, SPECIES_GLAMEOW, SPECIES_WATCHOG, SPECIES_JIGGLYPUFF, SPECIES_STUFFUL, SPECIES_VIGOROTH, SPECIES_PORYGON, SPECIES_STARAVIA, SPECIES_DIGGERSBY, SPECIES_TYPE_NULL},
        [4] = {SPECIES_TEDDIURSA, SPECIES_GLAMEOW, SPECIES_EEVEE_GIGA, SPECIES_BUNEARY, SPECIES_CASTFORM, SPECIES_STARAVIA, SPECIES_LINOONE_G, SPECIES_LICKITUNG, SPECIES_JIGGLYPUFF, SPECIES_VIGOROTH},
        [5] = {SPECIES_LINOONE_G, SPECIES_JIGGLYPUFF, SPECIES_KECLEON, SPECIES_RATICATE, SPECIES_TRUMBEAK, SPECIES_EEVEE, SPECIES_STUFFUL, SPECIES_STARAVIA, SPECIES_VIGOROTH, SPECIES_DUNSPARCE},
        [6] = {SPECIES_MILTANK, SPECIES_INDEEDEE, SPECIES_FURFROU_MATRON, SPECIES_CHANSEY, SPECIES_TOUCANNON, SPECIES_FURFROU_DANDY, SPECIES_SAWSBUCK, SPECIES_SWELLOW, SPECIES_FURFROU_HEART, SPECIES_MELOETTA_PIROUETTE},
        [7] = {SPECIES_PURUGLY, SPECIES_HELIOLISK, SPECIES_AUDINO_MEGA, SPECIES_SNORLAX, SPECIES_SAWSBUCK_SUMMER, SPECIES_SAWSBUCK_WINTER, SPECIES_SWELLOW, SPECIES_STARAPTOR, SPECIES_EXPLOUD, SPECIES_DUBWOOL},
        [8] = {SPECIES_PURUGLY, SPECIES_BLISSEY, SPECIES_FURFROU_MATRON, SPECIES_BRAVIARY, SPECIES_PIDGEOT_MEGA, SPECIES_FURFROU_KABUKI, SPECIES_REGIGIGAS, SPECIES_URSALUNA, SPECIES_SWELLOW, SPECIES_KOMALA},
        [9] = {SPECIES_HAPPINY, SPECIES_SAWSBUCK, SPECIES_ARCEUS, SPECIES_FURFROU_MATRON, SPECIES_ORANGURU, SPECIES_STANTLER, SPECIES_EXPLOUD, SPECIES_PORYGON2, SPECIES_FURFROU_DEBUTANTE, SPECIES_INDEEDEE_FEMALE},
    },
    [CLASS_HEX_MANIAC] = {
        [0] = {SPECIES_ANNIHILAPE, SPECIES_COSMOG, SPECIES_SCREAM_TAIL, SPECIES_WYNAUT, SPECIES_UNOWN_U, SPECIES_UNOWN_I, SPECIES_PUMPKABOO_M, SPECIES_IRON_BOULDER, SPECIES_GIMMIGHOUL_ROAMING, SPECIES_SINISTCHA},
        [1] = {SPECIES_MUNKIDORI, SPECIES_GOTHITA, SPECIES_UNOWN_V, SPECIES_PUMPKABOO_M, SPECIES_UNOWN_X, SPECIES_LITWICK, SPECIES_GREAVARD, SPECIES_UNOWN_S, SPECIES_UNOWN_C, SPECIES_IRON_CROWN},
        [2] = {SPECIES_UNOWN_D, SPECIES_GIMMIGHOUL, SPECIES_SINISTEA_CHIPPED, SPECIES_CERULEDGE, SPECIES_UNOWN_J, SPECIES_UNOWN_H, SPECIES_BALTOY, SPECIES_PECHARUNT, SPECIES_UNOWN_Z, SPECIES_IRON_LEAVES},
        [3] = {SPECIES_SABLEYE_MEGA, SPECIES_DOTTLER, SPECIES_SMOOCHUM, SPECIES_MUNNA, SPECIES_PSYDUCK, SPECIES_HATTREM, SPECIES_SWOOBAT, SPECIES_EXEGGCUTE_A, SPECIES_SHUPPET, SPECIES_SILVALLY_GHOST},
        [4] = {SPECIES_SHEDINJA, SPECIES_CHIMECHO, SPECIES_CHINGLING, SPECIES_SABLEYE_MEGA, SPECIES_MIME_JR_G, SPECIES_SANDYGAST, SPECIES_BELDUM, SPECIES_SILVALLY_GHOST, SPECIES_PIKACHU_PHD, SPECIES_DUOSION},
        [5] = {SPECIES_MEDITITE, SPECIES_PIKACHU_PHD, SPECIES_EXEGGCUTE_A, SPECIES_MUNNA, SPECIES_SMOOCHUM, SPECIES_SILVALLY_GHOST, SPECIES_ROTOM, SPECIES_LAMPENT, SPECIES_MIME_JR, SPECIES_DROWZEE},
        [6] = {SPECIES_GOLURK, SPECIES_INDEEDEE_FEMALE, SPECIES_GOLDUCK, SPECIES_POLTEAGEIST_CHIPPED, SPECIES_GARDEVOIR_MEGA, SPECIES_MISMAGIUS, SPECIES_CHANDELURE, SPECIES_GRUMPIG, SPECIES_BRAVIARY_H, SPECIES_MEWTWO_MEGA_Y},
        [7] = {SPECIES_GOURGEIST_L, SPECIES_TYPHLOSION_H, SPECIES_XATU, SPECIES_GRUMPIG, SPECIES_ZOROARK_H, SPECIES_GARDEVOIR_MEGA, SPECIES_SLOWKING, SPECIES_ALAKAZAM, SPECIES_ORBEETLE, SPECIES_NECROZMA_DUSK_MANE},
        [8] = {SPECIES_SIGILYPH, SPECIES_JELLICENT_F, SPECIES_BANETTE_MEGA, SPECIES_TREVENANT, SPECIES_GRUMPIG, SPECIES_SLOWBRO, SPECIES_PONYTA_G, SPECIES_MALAMAR, SPECIES_DOUBLADE, SPECIES_SOLROCK},
        [9] = {SPECIES_ESPEON, SPECIES_FROSLASS, SPECIES_RUNERIGUS, SPECIES_HATTERENE_GIGA, SPECIES_DEOXYS_SPEED, SPECIES_TAPU_LELE, SPECIES_ORBEETLE, SPECIES_XATU, SPECIES_MIMIKYU_BUSTED, SPECIES_MEOWSTIC_FEMALE},
    },
    [CLASS_LADY_RS] = {
        [0] = {SPECIES_SENTRET, SPECIES_AZURILL, SPECIES_TERAPAGOS, SPECIES_OINKOLOGNE_FEMALE, SPECIES_ZIGZAGOON_G, SPECIES_MAUSHOLD_FOUR, SPECIES_WHISMUR, SPECIES_TANDEMAUS, SPECIES_STARLY, SPECIES_DITTO},
        [1] = {SPECIES_PIDGEY, SPECIES_PIDOVE, SPECIES_TERAPAGOS_TERASTAL, SPECIES_GRAFAIAI, SPECIES_SPEAROW, SPECIES_FLETCHLING, SPECIES_ZORUA_H, SPECIES_HELIOPTILE, SPECIES_FARIGIRAF, SPECIES_TERAPAGOS_STELLAR},
        [2] = {SPECIES_SKITTY, SPECIES_RATTATA_A, SPECIES_TERAPAGOS_STELLAR, SPECIES_DOLLIV, SPECIES_PIKIPEK, SPECIES_SKWOVET, SPECIES_HELIOPTILE, SPECIES_GRAFAIAI, SPECIES_DEERLING_SUMMER, SPECIES_OINKOLOGNE_FEMALE},
        [3] = {SPECIES_GLAMEOW, SPECIES_LOUDRED, SPECIES_TRANQUILL, SPECIES_CASTFORM, SPECIES_LINOONE_G, SPECIES_BIBAREL, SPECIES_LITLEO, SPECIES_MUNCHLAX, SPECIES_RATICATE, SPECIES_EEVEE_GIGA},
        [4] = {SPECIES_EEVEE_GIGA, SPECIES_CHATOT, SPECIES_WOOLOO, SPECIES_STARAVIA, SPECIES_JIGGLYPUFF, SPECIES_TEDDIURSA, SPECIES_GIRAFARIG, SPECIES_RUFFLET, SPECIES_GLAMEOW, SPECIES_GUMSHOOS},
        [5] = {SPECIES_DELCATTY, SPECIES_KECLEON, SPECIES_LINOONE_G, SPECIES_STARAVIA, SPECIES_STUFFUL, SPECIES_VIGOROTH, SPECIES_DODUO, SPECIES_CASTFORM, SPECIES_RATICATE_A, SPECIES_PIDGEOTTO},
        [6] = {SPECIES_MILTANK, SPECIES_FURFROU_DEBUTANTE, SPECIES_SAWSBUCK, SPECIES_AUDINO_MEGA, SPECIES_DUBWOOL, SPECIES_STARAPTOR, SPECIES_AMBIPOM, SPECIES_SAWSBUCK_SUMMER, SPECIES_UNFEZANT_F, SPECIES_LOPUNNY},
        [7] = {SPECIES_SAWSBUCK, SPECIES_INDEEDEE_FEMALE, SPECIES_OBSTAGOON, SPECIES_CHANSEY, SPECIES_FURFROU, SPECIES_HAPPINY, SPECIES_FURFROU_LA_REINE, SPECIES_STANTLER, SPECIES_BOUFFALANT, SPECIES_PORYGON_Z},
        [8] = {SPECIES_STANTLER, SPECIES_UNFEZANT, SPECIES_PYROAR, SPECIES_GREEDENT, SPECIES_BRAVIARY, SPECIES_BLISSEY, SPECIES_BEWEAR, SPECIES_EXPLOUD, SPECIES_HELIOLISK, SPECIES_AUDINO_MEGA},
        [9] = {SPECIES_PORYGON2, SPECIES_FURFROU_STAR, SPECIES_KANGASKHAN_MEGA, SPECIES_FURFROU_LA_REINE, SPECIES_DODRIO, SPECIES_INDEEDEE, SPECIES_INDEEDEE_FEMALE, SPECIES_AUDINO, SPECIES_HELIOLISK, SPECIES_SNORLAX},
    },
    [CLASS_BEAUTY_RS] = {
        [0] = {SPECIES_SENTRET, SPECIES_FARIGIRAF, SPECIES_SQUAWKABILLY_YELLOW, SPECIES_DOLLIV, SPECIES_SQUAWKABILLY, SPECIES_BUNNELBY, SPECIES_DEERLING_WINTER, SPECIES_DUDUNSPARCE, SPECIES_MEOWTH, SPECIES_YUNGOOS},
        [1] = {SPECIES_DITTO, SPECIES_HELIOPTILE, SPECIES_DEERLING_SUMMER, SPECIES_MEOWTH, SPECIES_SPEAROW, SPECIES_DUDUNSPARCE_THREE, SPECIES_LECHONK, SPECIES_TAILLOW, SPECIES_ZIGZAGOON_G, SPECIES_TERAPAGOS_TERASTAL},
        [2] = {SPECIES_BIDOOF, SPECIES_YUNGOOS, SPECIES_SQUAWKABILLY, SPECIES_MEOWTH, SPECIES_OINKOLOGNE_FEMALE, SPECIES_PIDGEY, SPECIES_URSALUNA_BLOODMOON, SPECIES_MINCCINO, SPECIES_MAUSHOLD_FOUR, SPECIES_DEERLING_AUTUMN},
        [3] = {SPECIES_RATICATE, SPECIES_SPINDA, SPECIES_DUNSPARCE, SPECIES_WOOLOO, SPECIES_LINOONE, SPECIES_PORYGON, SPECIES_STUFFUL, SPECIES_GUMSHOOS, SPECIES_SWABLU, SPECIES_RATICATE_A},
        [4] = {SPECIES_LICKITUNG, SPECIES_SMEARGLE, SPECIES_SPINDA, SPECIES_BUNEARY, SPECIES_LINOONE_G, SPECIES_EEVEE_GIGA, SPECIES_GLAMEOW, SPECIES_FARFETCHD, SPECIES_JIGGLYPUFF, SPECIES_KECLEON},
        [5] = {SPECIES_RUFFLET, SPECIES_PORYGON, SPECIES_TRANQUILL, SPECIES_FARFETCHD, SPECIES_STARAVIA, SPECIES_GIRAFARIG, SPECIES_HERDIER, SPECIES_DIGGERSBY, SPECIES_RATICATE_A, SPECIES_VIGOROTH},
        [6] = {SPECIES_TAUROS, SPECIES_SNORLAX_GIGA, SPECIES_INDEEDEE, SPECIES_LICKILICKY, SPECIES_ORANGURU, SPECIES_KOMALA, SPECIES_FURFROU_KABUKI, SPECIES_REGIGIGAS, SPECIES_AUDINO, SPECIES_AUDINO_MEGA},
        [7] = {SPECIES_AMBIPOM, SPECIES_SAWSBUCK_WINTER, SPECIES_ZANGOOSE, SPECIES_KOMALA, SPECIES_FURFROU_HEART, SPECIES_FURFROU_STAR, SPECIES_FEAROW, SPECIES_LOPUNNY_MEGA, SPECIES_INDEEDEE, SPECIES_PIDGEOT_MEGA},
        [8] = {SPECIES_SAWSBUCK_AUTUMN, SPECIES_INDEEDEE, SPECIES_EXPLOUD, SPECIES_KANGASKHAN, SPECIES_FURFROU_KABUKI, SPECIES_SNORLAX_GIGA, SPECIES_PYROAR, SPECIES_FURFROU_DIAMOND, SPECIES_FURFROU_LA_REINE, SPECIES_AMBIPOM},
        [9] = {SPECIES_PYROAR_FEMALE, SPECIES_PORYGON2, SPECIES_ZOROARK_H, SPECIES_CINCCINO, SPECIES_FURFROU_PHAROAH, SPECIES_SAWSBUCK_WINTER, SPECIES_BOUFFALANT, SPECIES_FEAROW, SPECIES_FURFROU, SPECIES_LOPUNNY_MEGA},
    },
    [CLASS_RICH_BOY] = {
        [0] = {SPECIES_PIDGEY, SPECIES_ZIGZAGOON, SPECIES_DUDUNSPARCE, SPECIES_ZIGZAGOON_G, SPECIES_MEOWTH, SPECIES_IRON_JUGULIS, SPECIES_TING_LU, SPECIES_DEINO, SPECIES_TERAPAGOS_TERASTAL, SPECIES_OINKOLOGNE_FEMALE},
        [1] = {SPECIES_MABOSSTIFF, SPECIES_MEOWTH_A, SPECIES_MAUSHOLD, SPECIES_ZIGZAGOON, SPECIES_TING_LU, SPECIES_TANDEMAUS, SPECIES_PIKIPEK, SPECIES_KINGAMBIT, SPECIES_RATTATA, SPECIES_ZIGZAGOON_G},
        [2] = {SPECIES_HELIOPTILE, SPECIES_IMPIDIMP, SPECIES_SKITTY, SPECIES_BOMBIRDIER, SPECIES_MEOWTH_A, SPECIES_CHIEN_PAO, SPECIES_WHISMUR, SPECIES_BRUTE_BONNET, SPECIES_SQUAWKABILLY_WHITE, SPECIES_GRAFAIAI},
        [3] = {SPECIES_KECLEON, SPECIES_MIGHTYENA, SPECIES_FURRET, SPECIES_QWILFISH_H, SPECIES_PERSIAN, SPECIES_BIBAREL, SPECIES_GLAMEOW, SPECIES_PIDGEOTTO, SPECIES_STARAVIA, SPECIES_GRIMER_A},
        [4] = {SPECIES_GLAMEOW, SPECIES_DODUO, SPECIES_VULLABY, SPECIES_DIGGERSBY, SPECIES_ARBOK, SPECIES_GRIMER_A, SPECIES_MORGREM, SPECIES_PERSIAN_A, SPECIES_LITLEO, SPECIES_ZWEILOUS},
        [5] = {SPECIES_LOUDRED, SPECIES_HERDIER, SPECIES_CASTFORM, SPECIES_BIBAREL, SPECIES_WOOLOO, SPECIES_VULLABY, SPECIES_LINOONE_G, SPECIES_GRIMER_A, SPECIES_SNEASEL, SPECIES_HOUNDOUR},
        [6] = {SPECIES_GUZZLORD, SPECIES_FURFROU_MATRON, SPECIES_ORANGURU, SPECIES_TYRANITAR, SPECIES_MORPEKO_HANGRY, SPECIES_CHANSEY, SPECIES_ZOROARK, SPECIES_PANGORO, SPECIES_DRAMPA, SPECIES_KROOKODILE},
        [7] = {SPECIES_MANDIBUZZ, SPECIES_AMBIPOM, SPECIES_GUZZLORD, SPECIES_STANTLER, SPECIES_AUDINO_MEGA, SPECIES_ASHGRENINJA, SPECIES_SCRAFTY, SPECIES_ZOROARK_H, SPECIES_PIDGEOT_MEGA, SPECIES_WYRDEER},
        [8] = {SPECIES_STOUTLAND, SPECIES_FURFROU_PHAROAH, SPECIES_TOUCANNON, SPECIES_PYROAR, SPECIES_SAWSBUCK_AUTUMN, SPECIES_MELOETTA_PIROUETTE, SPECIES_SILVALLY, SPECIES_DARKRAI, SPECIES_KANGASKHAN_MEGA, SPECIES_ZANGOOSE},
        [9] = {SPECIES_ABSOL_MEGA, SPECIES_KOMALA, SPECIES_ZARUDE_DADA, SPECIES_SLAKING, SPECIES_MALAMAR, SPECIES_INDEEDEE_FEMALE, SPECIES_GYARADOS_MEGA, SPECIES_SAWSBUCK_AUTUMN, SPECIES_HAPPINY, SPECIES_TYRANITAR},
    },
    [CLASS_POKEMANIAC_RS] = {
        [0] = {SPECIES_DEERLING_WINTER, SPECIES_TERAPAGOS_TERASTAL, SPECIES_OINKOLOGNE_FEMALE, SPECIES_DEERLING, SPECIES_SPEAROW, SPECIES_HELIOPTILE, SPECIES_DITTO, SPECIES_PIDGEY, SPECIES_SQUAWKABILLY, SPECIES_GRAFAIAI},
        [1] = {SPECIES_MINCCINO, SPECIES_OINKOLOGNE_FEMALE, SPECIES_YUNGOOS, SPECIES_ARBOLIVA, SPECIES_SQUAWKABILLY_BLUE, SPECIES_SENTRET, SPECIES_BIDOOF, SPECIES_RATTATA, SPECIES_WHISMUR, SPECIES_BUNNELBY},
        [2] = {SPECIES_PATRAT, SPECIES_GRAFAIAI, SPECIES_DEERLING_SUMMER, SPECIES_SPEAROW, SPECIES_HOOTHOOT, SPECIES_WHISMUR, SPECIES_LILLIPUP, SPECIES_TERAPAGOS_STELLAR, SPECIES_TANDEMAUS, SPECIES_IGGLYBUFF},
        [3] = {SPECIES_DUNSPARCE, SPECIES_AIPOM, SPECIES_SMEARGLE, SPECIES_KECLEON, SPECIES_WIGGLYTUFF, SPECIES_HERDIER, SPECIES_PIDGEOTTO, SPECIES_LICKITUNG, SPECIES_WATCHOG, SPECIES_SLAKOTH},
        [4] = {SPECIES_SPINDA, SPECIES_RATICATE, SPECIES_WATCHOG, SPECIES_DUNSPARCE, SPECIES_VIGOROTH, SPECIES_LITLEO, SPECIES_RATICATE_A, SPECIES_SWABLU, SPECIES_STARAVIA, SPECIES_MUNCHLAX},
        [5] = {SPECIES_WATCHOG, SPECIES_AIPOM, SPECIES_LINOONE_G, SPECIES_TYPE_NULL, SPECIES_DUNSPARCE, SPECIES_DODUO, SPECIES_KECLEON, SPECIES_LOUDRED, SPECIES_SWABLU, SPECIES_BUNEARY},
        [6] = {SPECIES_CINCCINO, SPECIES_STANTLER, SPECIES_BRAVIARY, SPECIES_EXPLOUD, SPECIES_PYROAR, SPECIES_SAWSBUCK, SPECIES_BOUFFALANT, SPECIES_REGIGIGAS, SPECIES_URSARING, SPECIES_KANGASKHAN},
        [7] = {SPECIES_STARAPTOR, SPECIES_CINCCINO, SPECIES_SAWSBUCK_AUTUMN, SPECIES_WYRDEER, SPECIES_STOUTLAND, SPECIES_FURFROU, SPECIES_FURFROU_STAR, SPECIES_FURFROU_KABUKI, SPECIES_FURFROU_LA_REINE, SPECIES_BRAVIARY},
        [8] = {SPECIES_FURFROU, SPECIES_FURFROU_LA_REINE, SPECIES_SAWSBUCK_SUMMER, SPECIES_GREEDENT, SPECIES_STARAPTOR, SPECIES_KANGASKHAN, SPECIES_INDEEDEE_FEMALE, SPECIES_BRAVIARY, SPECIES_TAUROS, SPECIES_EXPLOUD},
        [9] = {SPECIES_PORYGON_Z, SPECIES_PIDGEOT_MEGA, SPECIES_SNORLAX_GIGA, SPECIES_SAWSBUCK_WINTER, SPECIES_SAWSBUCK_SUMMER, SPECIES_SLAKING, SPECIES_STANTLER, SPECIES_MILTANK, SPECIES_WYRDEER, SPECIES_DRAMPA},
    },
    [CLASS_PKMN_TRAINER_RSE] = {
        [0] = {SPECIES_DUDUNSPARCE_THREE, SPECIES_TERAPAGOS_STELLAR, SPECIES_TERAPAGOS, SPECIES_YUNGOOS, SPECIES_BIDOOF, SPECIES_CYCLIZAR, SPECIES_MAUSHOLD_FOUR, SPECIES_DEERLING_WINTER, SPECIES_IGGLYBUFF, SPECIES_LILLIPUP},
        [1] = {SPECIES_ZIGZAGOON_G, SPECIES_TERAPAGOS_STELLAR, SPECIES_LILLIPUP, SPECIES_AZURILL, SPECIES_YUNGOOS, SPECIES_MEOWTH, SPECIES_GRAFAIAI, SPECIES_ZIGZAGOON, SPECIES_RATTATA_A, SPECIES_DEERLING_AUTUMN},
        [2] = {SPECIES_LILLIPUP, SPECIES_TANDEMAUS, SPECIES_FARIGIRAF, SPECIES_CYCLIZAR, SPECIES_WHISMUR, SPECIES_PATRAT, SPECIES_ZIGZAGOON, SPECIES_HOOTHOOT, SPECIES_DEERLING_AUTUMN, SPECIES_MAUSHOLD},
        [3] = {SPECIES_SMEARGLE, SPECIES_BUNEARY, SPECIES_KECLEON, SPECIES_CHATOT, SPECIES_BIBAREL, SPECIES_LINOONE, SPECIES_FARFETCHD, SPECIES_DUNSPARCE, SPECIES_STARAVIA, SPECIES_RATICATE_A},
        [4] = {SPECIES_LICKITUNG, SPECIES_DUNSPARCE, SPECIES_TEDDIURSA, SPECIES_GLAMEOW, SPECIES_GUMSHOOS, SPECIES_KECLEON, SPECIES_STUFFUL, SPECIES_HERDIER, SPECIES_JIGGLYPUFF, SPECIES_EEVEE_GIGA},
        [5] = {SPECIES_MUNCHLAX, SPECIES_BUNEARY, SPECIES_LINOONE_G, SPECIES_JIGGLYPUFF, SPECIES_LICKITUNG, SPECIES_RATICATE, SPECIES_LINOONE, SPECIES_EEVEE, SPECIES_AIPOM, SPECIES_PERSIAN},
        [6] = {SPECIES_UNFEZANT, SPECIES_SLAKING, SPECIES_MELOETTA, SPECIES_DUBWOOL, SPECIES_INDEEDEE, SPECIES_WYRDEER, SPECIES_REGIGIGAS, SPECIES_LOPUNNY_MEGA, SPECIES_BRAVIARY, SPECIES_FURFROU_PHAROAH},
        [7] = {SPECIES_LOPUNNY, SPECIES_PIDGEOT_MEGA, SPECIES_DODRIO, SPECIES_FURFROU, SPECIES_SAWSBUCK, SPECIES_FEAROW, SPECIES_ZOROARK_H, SPECIES_UNFEZANT, SPECIES_DUBWOOL, SPECIES_OBSTAGOON},
        [8] = {SPECIES_HAPPINY, SPECIES_FURFROU_MATRON, SPECIES_PORYGON_Z, SPECIES_SILVALLY, SPECIES_SLAKING, SPECIES_PIDGEOT_MEGA, SPECIES_TOUCANNON, SPECIES_CINCCINO, SPECIES_MELOETTA, SPECIES_DODRIO},
        [9] = {SPECIES_FURFROU_PHAROAH, SPECIES_SNORLAX_GIGA, SPECIES_SAWSBUCK_AUTUMN, SPECIES_HELIOLISK, SPECIES_STANTLER, SPECIES_SWELLOW, SPECIES_OBSTAGOON, SPECIES_FURFROU_DIAMOND, SPECIES_BLISSEY, SPECIES_SNORLAX},
    },
    [CLASS_BLACK_BELT_RS] = {
        [0] = {SPECIES_HOOTHOOT, SPECIES_RATTATA_A, SPECIES_STARLY, SPECIES_SHROODLE, SPECIES_LILLIPUP, SPECIES_BIDOOF, SPECIES_TAILLOW, SPECIES_SENTRET, SPECIES_DEERLING_AUTUMN, SPECIES_SQUAWKABILLY_WHITE},
        [1] = {SPECIES_TERAPAGOS_TERASTAL, SPECIES_RATTATA, SPECIES_SQUAWKABILLY_WHITE, SPECIES_IGGLYBUFF, SPECIES_DOLLIV, SPECIES_PIKIPEK, SPECIES_MAUSHOLD_FOUR, SPECIES_ARBOLIVA, SPECIES_MAUSHOLD, SPECIES_LILLIPUP},
        [2] = {SPECIES_SKWOVET, SPECIES_MINCCINO, SPECIES_BUNNELBY, SPECIES_YUNGOOS, SPECIES_SQUAWKABILLY_WHITE, SPECIES_SQUAWKABILLY_YELLOW, SPECIES_TERAPAGOS_TERASTAL, SPECIES_MEOWTH_GIGA, SPECIES_SKITTY, SPECIES_AZURILL},
        [3] = {SPECIES_SLAKOTH, SPECIES_LICKITUNG, SPECIES_JIGGLYPUFF, SPECIES_LOUDRED, SPECIES_CASTFORM, SPECIES_TRUMBEAK, SPECIES_STARAVIA, SPECIES_GIRAFARIG, SPECIES_RUFFLET, SPECIES_CHATOT},
        [4] = {SPECIES_KECLEON, SPECIES_PERSIAN, SPECIES_TRANQUILL, SPECIES_RATICATE, SPECIES_GIRAFARIG, SPECIES_TEDDIURSA, SPECIES_LINOONE_G, SPECIES_CHATOT, SPECIES_PIDGEOTTO, SPECIES_RUFFLET},
        [5] = {SPECIES_BIBAREL, SPECIES_GUMSHOOS, SPECIES_LINOONE, SPECIES_WIGGLYTUFF, SPECIES_TYPE_NULL, SPECIES_DIGGERSBY, SPECIES_TRANQUILL, SPECIES_RATICATE, SPECIES_RATICATE_A, SPECIES_GIRAFARIG},
        [6] = {SPECIES_INDEEDEE, SPECIES_FEAROW, SPECIES_AUDINO, SPECIES_SNORLAX, SPECIES_STOUTLAND, SPECIES_ARCEUS, SPECIES_TOUCANNON, SPECIES_PORYGON_Z, SPECIES_REGIGIGAS, SPECIES_UNFEZANT},
        [7] = {SPECIES_MILTANK, SPECIES_STOUTLAND, SPECIES_URSALUNA, SPECIES_SLAKING, SPECIES_FURFROU_DEBUTANTE, SPECIES_PORYGON2, SPECIES_SILVALLY, SPECIES_UNFEZANT, SPECIES_KOMALA, SPECIES_FURFROU_DANDY},
        [8] = {SPECIES_AUDINO_MEGA, SPECIES_ARCEUS, SPECIES_DUBWOOL, SPECIES_BRAVIARY, SPECIES_FURFROU_MATRON, SPECIES_AMBIPOM, SPECIES_OBSTAGOON, SPECIES_FURFROU_HEART, SPECIES_FURFROU_PHAROAH, SPECIES_FEAROW},
        [9] = {SPECIES_FURFROU_DEBUTANTE, SPECIES_LICKILICKY, SPECIES_GREEDENT, SPECIES_MILTANK, SPECIES_LOPUNNY_MEGA, SPECIES_KANGASKHAN, SPECIES_HAPPINY, SPECIES_SAWSBUCK_SUMMER, SPECIES_TAUROS, SPECIES_CHANSEY},
    },
    [CLASS_GUITARIST_RS] = {
        [0] = {SPECIES_EGG, SPECIES_OINKOLOGNE_FEMALE, SPECIES_DEERLING_AUTUMN, SPECIES_RATTATA, SPECIES_MINCCINO, SPECIES_TAILLOW, SPECIES_TERAPAGOS_STELLAR, SPECIES_ZORUA_H, SPECIES_CYCLIZAR, SPECIES_WHISMUR},
        [1] = {SPECIES_SKITTY, SPECIES_MAUSHOLD, SPECIES_OINKOLOGNE_FEMALE, SPECIES_DUDUNSPARCE_THREE, SPECIES_RATTATA_A, SPECIES_PIDGEY, SPECIES_HOOTHOOT, SPECIES_BUNNELBY, SPECIES_PIKIPEK, SPECIES_SHROODLE},
        [2] = {SPECIES_LECHONK, SPECIES_DEERLING_AUTUMN, SPECIES_PIDGEY, SPECIES_HOOTHOOT, SPECIES_AZURILL, SPECIES_SQUAWKABILLY, SPECIES_MAUSHOLD, SPECIES_SQUAWKABILLY_BLUE, SPECIES_DITTO, SPECIES_SKITTY},
        [3] = {SPECIES_BUNEARY, SPECIES_DELCATTY, SPECIES_WATCHOG, SPECIES_STUFFUL, SPECIES_DODUO, SPECIES_WOOLOO, SPECIES_BIBAREL, SPECIES_PORYGON, SPECIES_PERSIAN, SPECIES_TRUMBEAK},
        [4] = {SPECIES_BUNEARY, SPECIES_KECLEON, SPECIES_LITLEO, SPECIES_WIGGLYTUFF, SPECIES_STUFFUL, SPECIES_LOUDRED, SPECIES_SPINDA, SPECIES_RATICATE_A, SPECIES_EEVEE_GIGA, SPECIES_PORYGON},
        [5] = {SPECIES_LINOONE, SPECIES_DIGGERSBY, SPECIES_DUNSPARCE, SPECIES_TYPE_NULL, SPECIES_STUFFUL, SPECIES_CHATOT, SPECIES_PIDGEOTTO, SPECIES_BUNEARY, SPECIES_SMEARGLE, SPECIES_PORYGON},
        [6] = {SPECIES_STARAPTOR, SPECIES_PORYGON2, SPECIES_ARCEUS, SPECIES_STOUTLAND, SPECIES_FURFROU_HEART, SPECIES_FURFROU_LA_REINE, SPECIES_CHANSEY, SPECIES_SILVALLY, SPECIES_PORYGON_Z, SPECIES_HAPPINY},
        [7] = {SPECIES_FURFROU_LA_REINE, SPECIES_SWELLOW, SPECIES_SNORLAX, SPECIES_LOPUNNY, SPECIES_SAWSBUCK_AUTUMN, SPECIES_PYROAR_FEMALE, SPECIES_BRAVIARY, SPECIES_BEWEAR, SPECIES_PORYGON_Z, SPECIES_INDEEDEE_FEMALE},
        [8] = {SPECIES_FURFROU_MATRON, SPECIES_FURFROU_DEBUTANTE, SPECIES_WYRDEER, SPECIES_HAPPINY, SPECIES_FURFROU_LA_REINE, SPECIES_UNFEZANT_F, SPECIES_FURFROU_STAR, SPECIES_EXPLOUD, SPECIES_PYROAR, SPECIES_MILTANK},
        [9] = {SPECIES_FURFROU_DANDY, SPECIES_TOUCANNON, SPECIES_SAWSBUCK_AUTUMN, SPECIES_DUBWOOL, SPECIES_FURFROU_PHAROAH, SPECIES_BLISSEY, SPECIES_ORANGURU, SPECIES_FEAROW, SPECIES_AUDINO, SPECIES_ARCEUS},
    },
    [CLASS_KINDLER] = {
        [0] = {SPECIES_TORCHIC, SPECIES_CHIMCHAR, SPECIES_IRON_MOTH, SPECIES_TAUROS_BLAZE_P, SPECIES_SCORBUNNY, SPECIES_TEPIG, SPECIES_CERULEDGE, SPECIES_GOUGING_FIRE, SPECIES_LITTEN, SPECIES_OGERPON_HEARTHFLAME_MASK},
        [1] = {SPECIES_CERULEDGE, SPECIES_CHARMANDER, SPECIES_CHARCADET, SPECIES_VULPIX, SPECIES_TAUROS_BLAZE_P, SPECIES_GOUGING_FIRE, SPECIES_TEPIG, SPECIES_CROCALOR, SPECIES_ARMAROUGE, SPECIES_SKELEDIRGE},
        [2] = {SPECIES_CERULEDGE, SPECIES_SKELEDIRGE, SPECIES_SCORBUNNY, SPECIES_CROCALOR, SPECIES_IRON_MOTH, SPECIES_LITWICK, SPECIES_TAUROS_BLAZE_P, SPECIES_FUECOCO, SPECIES_ARMAROUGE, SPECIES_CHIMCHAR},
        [3] = {SPECIES_GROWLITHE, SPECIES_SILVALLY_FIRE, SPECIES_SLUGMA, SPECIES_LAMPENT, SPECIES_TORRACAT, SPECIES_COMBUSKEN, SPECIES_GROWLITHE_H, SPECIES_RABOOT, SPECIES_CARKOL, SPECIES_NUMEL},
        [4] = {SPECIES_MAROWAK_A, SPECIES_SUNFLORA, SPECIES_SLUGMA, SPECIES_LARVESTA, SPECIES_FLETCHINDER, SPECIES_BRAIXEN, SPECIES_HOUNDOUR, SPECIES_LITLEO, SPECIES_PANSEAR, SPECIES_SALANDIT},
        [5] = {SPECIES_FLETCHINDER, SPECIES_GROWLITHE, SPECIES_QUILAVA, SPECIES_SILVALLY_FIRE, SPECIES_LITLEO, SPECIES_TORRACAT, SPECIES_COMBUSKEN, SPECIES_SLUGMA, SPECIES_ROTOM_HEAT, SPECIES_GROWLITHE_H},
        [6] = {SPECIES_BLAZIKEN_MEGA, SPECIES_CAMERUPT, SPECIES_COALOSSAL_GIGA, SPECIES_GROUDON_PRIMAL, SPECIES_CHARIZARD_MEGA_X, SPECIES_DARMANITAN, SPECIES_TALONFLAME, SPECIES_ENTEI, SPECIES_TYPHLOSION_H, SPECIES_CHANDELURE},
        [7] = {SPECIES_DARMANITAN, SPECIES_SALAZZLE, SPECIES_DELPHOX, SPECIES_HOUNDOOM, SPECIES_PONYTA, SPECIES_PYROAR, SPECIES_RESHIRAM, SPECIES_SIMISEAR, SPECIES_BLAZIKEN, SPECIES_ORICORIO},
        [8] = {SPECIES_COALOSSAL, SPECIES_MAGCARGO, SPECIES_TORKOAL, SPECIES_ARCEUS_FIRE, SPECIES_HEATMOR, SPECIES_CENTISKORCH, SPECIES_PYROAR, SPECIES_ARCANINE_H, SPECIES_ENTEI, SPECIES_CHARIZARD_MEGA_Y},
        [9] = {SPECIES_BLAZIKEN, SPECIES_TURTONATOR, SPECIES_TALONFLAME, SPECIES_BLAZIKEN_MEGA, SPECIES_HOUNDOOM, SPECIES_FLAREON, SPECIES_MAGMAR, SPECIES_NINETALES, SPECIES_COALOSSAL_GIGA, SPECIES_MOLTRES},
    },
    [CLASS_CAMPER_RS] = {
        [0] = {SPECIES_DEERLING_AUTUMN, SPECIES_RATTATA_A, SPECIES_AZURILL, SPECIES_ARBOLIVA, SPECIES_TANDEMAUS, SPECIES_TAILLOW, SPECIES_OINKOLOGNE, SPECIES_URSALUNA_BLOODMOON, SPECIES_FARIGIRAF, SPECIES_SKITTY},
        [1] = {SPECIES_WHISMUR, SPECIES_PIKIPEK, SPECIES_SMOLIV, SPECIES_ARBOLIVA, SPECIES_RATTATA, SPECIES_URSALUNA_BLOODMOON, SPECIES_HELIOPTILE, SPECIES_DUDUNSPARCE, SPECIES_PATRAT, SPECIES_DEERLING},
        [2] = {SPECIES_AZURILL, SPECIES_EGG, SPECIES_TAILLOW, SPECIES_PATRAT, SPECIES_YUNGOOS, SPECIES_DEERLING_WINTER, SPECIES_SMOLIV, SPECIES_OINKOLOGNE_FEMALE, SPECIES_TANDEMAUS, SPECIES_TERAPAGOS},
        [3] = {SPECIES_TEDDIURSA, SPECIES_EEVEE, SPECIES_RUFFLET, SPECIES_STUFFUL, SPECIES_PORYGON, SPECIES_KECLEON, SPECIES_GUMSHOOS, SPECIES_PERSIAN, SPECIES_VIGOROTH, SPECIES_FURRET},
        [4] = {SPECIES_WIGGLYTUFF, SPECIES_BUNEARY, SPECIES_LICKITUNG, SPECIES_LINOONE_G, SPECIES_DUNSPARCE, SPECIES_TRANQUILL, SPECIES_FURRET, SPECIES_PORYGON, SPECIES_LITLEO, SPECIES_JIGGLYPUFF},
        [5] = {SPECIES_PERSIAN, SPECIES_KECLEON, SPECIES_PIDGEOTTO, SPECIES_CHATOT, SPECIES_WATCHOG, SPECIES_DODUO, SPECIES_GUMSHOOS, SPECIES_TEDDIURSA, SPECIES_DELCATTY, SPECIES_LOUDRED},
        [6] = {SPECIES_PYROAR, SPECIES_PYROAR_FEMALE, SPECIES_FURFROU_LA_REINE, SPECIES_PIDGEOT, SPECIES_BOUFFALANT, SPECIES_PIDGEOT_MEGA, SPECIES_STARAPTOR, SPECIES_EXPLOUD, SPECIES_LICKILICKY, SPECIES_SAWSBUCK},
        [7] = {SPECIES_SAWSBUCK_WINTER, SPECIES_REGIGIGAS, SPECIES_PIDGEOT, SPECIES_FURFROU_DANDY, SPECIES_STOUTLAND, SPECIES_ZANGOOSE, SPECIES_STARAPTOR, SPECIES_BRAVIARY, SPECIES_FURFROU_DIAMOND, SPECIES_LOPUNNY},
        [8] = {SPECIES_DRAMPA, SPECIES_SWELLOW, SPECIES_SAWSBUCK_AUTUMN, SPECIES_BRAVIARY, SPECIES_SAWSBUCK_WINTER, SPECIES_REGIGIGAS, SPECIES_SAWSBUCK, SPECIES_SAWSBUCK_SUMMER, SPECIES_LICKILICKY, SPECIES_SNORLAX},
        [9] = {SPECIES_DRAMPA, SPECIES_SAWSBUCK_WINTER, SPECIES_TOUCANNON, SPECIES_FURFROU, SPECIES_BEWEAR, SPECIES_FURFROU_DEBUTANTE, SPECIES_SAWSBUCK_SUMMER, SPECIES_FURFROU_LA_REINE, SPECIES_MILTANK, SPECIES_PYROAR},
    },
    [CLASS_BUG_MANIAC] = {
        [0] = {SPECIES_BURMY_TRASH, SPECIES_VENIPEDE, SPECIES_DWEBBLE, SPECIES_RABSCA, SPECIES_WURMPLE, SPECIES_SNOM, SPECIES_BURMY, SPECIES_BURMY_SANDY, SPECIES_SHELMET, SPECIES_SCATTERBUG},
        [1] = {SPECIES_RELLOR, SPECIES_JOLTIK, SPECIES_SCATTERBUG, SPECIES_SPIDOPS, SPECIES_PINECO, SPECIES_TAROUNTULA, SPECIES_BLIPBUG, SPECIES_SHELMET, SPECIES_GRUBBIN, SPECIES_DEWPIDER},
        [2] = {SPECIES_WIMPOD, SPECIES_SPIDOPS, SPECIES_LEDYBA, SPECIES_KRICKETOT, SPECIES_DWEBBLE, SPECIES_JOLTIK, SPECIES_SPINARAK, SPECIES_NINCADA, SPECIES_WURMPLE, SPECIES_SNOM},
        [3] = {SPECIES_SHEDINJA, SPECIES_YANMA, SPECIES_RIBOMBEE, SPECIES_ANORITH, SPECIES_CUTIEFLY, SPECIES_PARAS, SPECIES_ARAQUANID, SPECIES_SHUCKLE, SPECIES_FORRETRESS, SPECIES_WORMADAM_TRASH},
        [4] = {SPECIES_SEWADDLE, SPECIES_LARVESTA, SPECIES_LEDIAN, SPECIES_CASCOON, SPECIES_VENOMOTH, SPECIES_METAPOD, SPECIES_ARIADOS, SPECIES_SILVALLY_BUG, SPECIES_SHEDINJA, SPECIES_CHARJABUG},
        [5] = {SPECIES_VIBRAVA, SPECIES_SWADLOON, SPECIES_ARAQUANID, SPECIES_LARVESTA, SPECIES_SKORUPI, SPECIES_RIBOMBEE, SPECIES_SPEWPA, SPECIES_ILLUMISE, SPECIES_WHIRLIPEDE, SPECIES_KAKUNA},
        [6] = {SPECIES_BEEDRILL_MEGA, SPECIES_CENTISKORCH_GIGA, SPECIES_VIVILLON_CONTINENTAL, SPECIES_BUTTERFREE_GIGA, SPECIES_HERACROSS, SPECIES_FROSMOTH, SPECIES_ARMALDO, SPECIES_VIVILLON_GARDEN, SPECIES_VIVILLON_ELEGANT, SPECIES_BEEDRILL},
        [7] = {SPECIES_ORBEETLE, SPECIES_VIVILLON_SAVANNA, SPECIES_VIVILLON_TUNDRA, SPECIES_VIVILLON_GARDEN, SPECIES_KLEAVOR, SPECIES_ESCAVALIER, SPECIES_VIVILLON_ARCHIPELAGO, SPECIES_VIVILLON_SANDSTORM, SPECIES_VIVILLON_RIVER, SPECIES_VIVILLON_MONSOON},
        [8] = {SPECIES_YANMEGA, SPECIES_NINJASK, SPECIES_VIVILLON_POKEBALL, SPECIES_BEEDRILL_MEGA, SPECIES_MOTHIM, SPECIES_GENESECT_CHILL, SPECIES_VIVILLON_CONTINENTAL, SPECIES_VESPIQUEN, SPECIES_LEAVANNY, SPECIES_SCYTHER},
        [9] = {SPECIES_VIVILLON_HIGH_PLAINS, SPECIES_BEEDRILL_MEGA, SPECIES_ARCEUS_BUG, SPECIES_KLEAVOR, SPECIES_SCIZOR_MEGA, SPECIES_ACCELGOR, SPECIES_CRUSTLE, SPECIES_SCYTHER, SPECIES_VESPIQUEN, SPECIES_BUZZWOLE},
    },
    [CLASS_PSYCHIC_RS] = {
        [0] = {SPECIES_DOLLIV, SPECIES_TANDEMAUS, SPECIES_FLETCHLING, SPECIES_EGG, SPECIES_HELIOPTILE, SPECIES_LECHONK, SPECIES_SQUAWKABILLY_YELLOW, SPECIES_WHISMUR, SPECIES_RATTATA, SPECIES_MAUSHOLD},
        [1] = {SPECIES_MAUSHOLD, SPECIES_PIKIPEK, SPECIES_SQUAWKABILLY, SPECIES_LECHONK, SPECIES_STARLY, SPECIES_TANDEMAUS, SPECIES_MINCCINO, SPECIES_DEERLING_WINTER, SPECIES_DEERLING, SPECIES_DEERLING_AUTUMN},
        [2] = {SPECIES_AZURILL, SPECIES_RATTATA, SPECIES_BUNNELBY, SPECIES_ZIGZAGOON_G, SPECIES_PATRAT, SPECIES_SQUAWKABILLY_WHITE, SPECIES_HELIOPTILE, SPECIES_SQUAWKABILLY_YELLOW, SPECIES_DEERLING_AUTUMN, SPECIES_MINCCINO},
        [3] = {SPECIES_DUNSPARCE, SPECIES_SWABLU, SPECIES_DODUO, SPECIES_JIGGLYPUFF, SPECIES_FURRET, SPECIES_CASTFORM, SPECIES_WATCHOG, SPECIES_GLAMEOW, SPECIES_VIGOROTH, SPECIES_RATICATE_A},
        [4] = {SPECIES_WIGGLYTUFF, SPECIES_PERSIAN, SPECIES_SPINDA, SPECIES_BUNEARY, SPECIES_DIGGERSBY, SPECIES_JIGGLYPUFF, SPECIES_STARAVIA, SPECIES_TRANQUILL, SPECIES_CHATOT, SPECIES_LINOONE},
        [5] = {SPECIES_LOUDRED, SPECIES_RATICATE_A, SPECIES_GIRAFARIG, SPECIES_PIDGEOTTO, SPECIES_CASTFORM, SPECIES_PERSIAN, SPECIES_RUFFLET, SPECIES_TEDDIURSA, SPECIES_VIGOROTH, SPECIES_TYPE_NULL},
        [6] = {SPECIES_PIDGEOT, SPECIES_PYROAR, SPECIES_OBSTAGOON, SPECIES_FURFROU_STAR, SPECIES_PORYGON_Z, SPECIES_UNFEZANT, SPECIES_ZOROARK_H, SPECIES_SNORLAX_GIGA, SPECIES_SAWSBUCK_AUTUMN, SPECIES_LOPUNNY_MEGA},
        [7] = {SPECIES_SILVALLY, SPECIES_INDEEDEE_FEMALE, SPECIES_URSARING, SPECIES_FURFROU_LA_REINE, SPECIES_CHANSEY, SPECIES_LOPUNNY_MEGA, SPECIES_ZOROARK_H, SPECIES_EXPLOUD, SPECIES_PYROAR, SPECIES_DRAMPA},
        [8] = {SPECIES_PIDGEOT, SPECIES_DRAMPA, SPECIES_FURFROU_DIAMOND, SPECIES_AMBIPOM, SPECIES_BEWEAR, SPECIES_OBSTAGOON, SPECIES_FURFROU_DEBUTANTE, SPECIES_SILVALLY, SPECIES_UNFEZANT, SPECIES_URSARING},
        [9] = {SPECIES_KOMALA, SPECIES_TOUCANNON, SPECIES_URSARING, SPECIES_BLISSEY, SPECIES_OBSTAGOON, SPECIES_FURFROU_HEART, SPECIES_LOPUNNY_MEGA, SPECIES_DUBWOOL, SPECIES_HELIOLISK, SPECIES_SAWSBUCK_SUMMER},
    },
    [CLASS_GENTLEMAN_RS] = {
        [0] = {SPECIES_GRAFAIAI, SPECIES_DUDUNSPARCE, SPECIES_SQUAWKABILLY, SPECIES_ZIGZAGOON, SPECIES_PIDOVE, SPECIES_SENTRET, SPECIES_SQUAWKABILLY_YELLOW, SPECIES_OINKOLOGNE, SPECIES_DEERLING_SUMMER, SPECIES_YUNGOOS},
        [1] = {SPECIES_LILLIPUP, SPECIES_DEERLING_AUTUMN, SPECIES_DUDUNSPARCE, SPECIES_TERAPAGOS, SPECIES_ZORUA_H, SPECIES_FLETCHLING, SPECIES_IGGLYBUFF, SPECIES_HELIOPTILE, SPECIES_DEERLING_SUMMER, SPECIES_TAILLOW},
        [2] = {SPECIES_OINKOLOGNE_FEMALE, SPECIES_ARBOLIVA, SPECIES_PIKIPEK, SPECIES_SQUAWKABILLY_BLUE, SPECIES_EGG, SPECIES_FARIGIRAF, SPECIES_OINKOLOGNE, SPECIES_SKITTY, SPECIES_SKWOVET, SPECIES_DEERLING_AUTUMN},
        [3] = {SPECIES_TEDDIURSA, SPECIES_WIGGLYTUFF, SPECIES_PIDGEOTTO, SPECIES_STUFFUL, SPECIES_EEVEE_GIGA, SPECIES_AIPOM, SPECIES_PERSIAN, SPECIES_DODUO, SPECIES_KECLEON, SPECIES_CHATOT},
        [4] = {SPECIES_BIBAREL, SPECIES_SLAKOTH, SPECIES_STARAVIA, SPECIES_TRUMBEAK, SPECIES_RATICATE, SPECIES_DIGGERSBY, SPECIES_FARFETCHD, SPECIES_LICKITUNG, SPECIES_HERDIER, SPECIES_PORYGON},
        [5] = {SPECIES_TEDDIURSA, SPECIES_SPINDA, SPECIES_AIPOM, SPECIES_GUMSHOOS, SPECIES_FURRET, SPECIES_RUFFLET, SPECIES_GLAMEOW, SPECIES_PERSIAN, SPECIES_LITLEO, SPECIES_PIDGEOTTO},
        [6] = {SPECIES_URSALUNA, SPECIES_MELOETTA_PIROUETTE, SPECIES_STOUTLAND, SPECIES_PYROAR, SPECIES_LICKILICKY, SPECIES_BLISSEY, SPECIES_SNORLAX_GIGA, SPECIES_DUBWOOL, SPECIES_HELIOLISK, SPECIES_FURFROU_KABUKI},
        [7] = {SPECIES_FURFROU_HEART, SPECIES_LOPUNNY_MEGA, SPECIES_PORYGON_Z, SPECIES_INDEEDEE_FEMALE, SPECIES_ZOROARK_H, SPECIES_CINCCINO, SPECIES_SAWSBUCK_SUMMER, SPECIES_KANGASKHAN_MEGA, SPECIES_FURFROU_KABUKI, SPECIES_SNORLAX},
        [8] = {SPECIES_SAWSBUCK, SPECIES_CHANSEY, SPECIES_FURFROU_DEBUTANTE, SPECIES_AMBIPOM, SPECIES_UNFEZANT, SPECIES_URSARING, SPECIES_DRAMPA, SPECIES_MILTANK, SPECIES_BOUFFALANT, SPECIES_FURFROU_DANDY},
        [9] = {SPECIES_TAUROS, SPECIES_SAWSBUCK_AUTUMN, SPECIES_SWELLOW, SPECIES_SILVALLY, SPECIES_STANTLER, SPECIES_PYROAR_FEMALE, SPECIES_PORYGON2, SPECIES_FEAROW, SPECIES_DRAMPA, SPECIES_ARCEUS},
    },
    [CLASS_ELITE_FOUR_RS] = {
        [0] = {SPECIES_SMOLIV, SPECIES_EGG, SPECIES_PATRAT, SPECIES_HOOTHOOT, SPECIES_DEERLING, SPECIES_DOLLIV, SPECIES_SPEAROW, SPECIES_MINCCINO, SPECIES_ZIGZAGOON_G, SPECIES_SKITTY},
        [1] = {SPECIES_MAUSHOLD, SPECIES_URSALUNA_BLOODMOON, SPECIES_SPEAROW, SPECIES_GRAFAIAI, SPECIES_STARLY, SPECIES_ZIGZAGOON, SPECIES_SQUAWKABILLY_BLUE, SPECIES_SKWOVET, SPECIES_DEERLING, SPECIES_SQUAWKABILLY_WHITE},
        [2] = {SPECIES_MINCCINO, SPECIES_DEERLING, SPECIES_ZIGZAGOON, SPECIES_PIDGEY, SPECIES_LILLIPUP, SPECIES_RATTATA_A, SPECIES_PIKIPEK, SPECIES_AZURILL, SPECIES_ARBOLIVA, SPECIES_DEERLING_SUMMER},
        [3] = {SPECIES_STARAVIA, SPECIES_LITLEO, SPECIES_DUNSPARCE, SPECIES_TYPE_NULL, SPECIES_RATICATE, SPECIES_CASTFORM, SPECIES_LOUDRED, SPECIES_BIBAREL, SPECIES_MUNCHLAX, SPECIES_PIDGEOTTO},
        [4] = {SPECIES_DIGGERSBY, SPECIES_PERSIAN, SPECIES_DODUO, SPECIES_LICKITUNG, SPECIES_TRUMBEAK, SPECIES_FURRET, SPECIES_FARFETCHD, SPECIES_HERDIER, SPECIES_PORYGON, SPECIES_SLAKOTH},
        [5] = {SPECIES_DODUO, SPECIES_TRUMBEAK, SPECIES_PORYGON, SPECIES_LOUDRED, SPECIES_SLAKOTH, SPECIES_CASTFORM, SPECIES_CHATOT, SPECIES_WOOLOO, SPECIES_BUNEARY, SPECIES_EEVEE},
        [6] = {SPECIES_SAWSBUCK_AUTUMN, SPECIES_FURFROU_HEART, SPECIES_MELOETTA_PIROUETTE, SPECIES_SWELLOW, SPECIES_KANGASKHAN, SPECIES_SLAKING, SPECIES_GREEDENT, SPECIES_LOPUNNY, SPECIES_WYRDEER, SPECIES_LICKILICKY},
        [7] = {SPECIES_SAWSBUCK_SUMMER, SPECIES_STARAPTOR, SPECIES_PORYGON_Z, SPECIES_WYRDEER, SPECIES_URSARING, SPECIES_SILVALLY, SPECIES_ARCEUS, SPECIES_SNORLAX, SPECIES_FURFROU, SPECIES_TOUCANNON},
        [8] = {SPECIES_FURFROU_KABUKI, SPECIES_PORYGON2, SPECIES_ORANGURU, SPECIES_ZOROARK_H, SPECIES_ARCEUS, SPECIES_STANTLER, SPECIES_STARAPTOR, SPECIES_SNORLAX_GIGA, SPECIES_SAWSBUCK_SUMMER, SPECIES_UNFEZANT_F},
        [9] = {SPECIES_BRAVIARY, SPECIES_FURFROU_PHAROAH, SPECIES_UNFEZANT_F, SPECIES_FURFROU_DANDY, SPECIES_DUBWOOL, SPECIES_URSALUNA, SPECIES_KOMALA, SPECIES_MILTANK, SPECIES_SNORLAX_GIGA, SPECIES_REGIGIGAS},
    },
    [CLASS_LEADER_RS] = {
        [0] = {SPECIES_AZURILL, SPECIES_IGGLYBUFF, SPECIES_SMOLIV, SPECIES_ZIGZAGOON_G, SPECIES_DEERLING_AUTUMN, SPECIES_ZIGZAGOON, SPECIES_URSALUNA_BLOODMOON, SPECIES_MAUSHOLD, SPECIES_ARBOLIVA, SPECIES_TANDEMAUS},
        [1] = {SPECIES_MEOWTH_GIGA, SPECIES_GRAFAIAI, SPECIES_ARBOLIVA, SPECIES_SKITTY, SPECIES_PIDOVE, SPECIES_BUNNELBY, SPECIES_DITTO, SPECIES_DUDUNSPARCE_THREE, SPECIES_EGG, SPECIES_DEERLING_SUMMER},
        [2] = {SPECIES_SQUAWKABILLY_WHITE, SPECIES_HELIOPTILE, SPECIES_AZURILL, SPECIES_DOLLIV, SPECIES_DUDUNSPARCE_THREE, SPECIES_MEOWTH_GIGA, SPECIES_PIDGEY, SPECIES_TAILLOW, SPECIES_BUNNELBY, SPECIES_GRAFAIAI},
        [3] = {SPECIES_GLAMEOW, SPECIES_WIGGLYTUFF, SPECIES_PERSIAN, SPECIES_SPINDA, SPECIES_TRANQUILL, SPECIES_RUFFLET, SPECIES_STUFFUL, SPECIES_AIPOM, SPECIES_BIBAREL, SPECIES_JIGGLYPUFF},
        [4] = {SPECIES_MUNCHLAX, SPECIES_GIRAFARIG, SPECIES_AIPOM, SPECIES_RATICATE, SPECIES_VIGOROTH, SPECIES_BUNEARY, SPECIES_PORYGON, SPECIES_KECLEON, SPECIES_WATCHOG, SPECIES_BIBAREL},
        [5] = {SPECIES_WOOLOO, SPECIES_EEVEE, SPECIES_JIGGLYPUFF, SPECIES_TEDDIURSA, SPECIES_LICKITUNG, SPECIES_HERDIER, SPECIES_TRANQUILL, SPECIES_FURRET, SPECIES_RATICATE_A, SPECIES_DELCATTY},
        [6] = {SPECIES_MILTANK, SPECIES_SAWSBUCK, SPECIES_EXPLOUD, SPECIES_TOUCANNON, SPECIES_URSALUNA, SPECIES_FURFROU, SPECIES_BOUFFALANT, SPECIES_KOMALA, SPECIES_SNORLAX, SPECIES_LOPUNNY},
        [7] = {SPECIES_SILVALLY, SPECIES_PORYGON_Z, SPECIES_SNORLAX_GIGA, SPECIES_SAWSBUCK, SPECIES_KANGASKHAN_MEGA, SPECIES_BLISSEY, SPECIES_DODRIO, SPECIES_BEWEAR, SPECIES_SAWSBUCK_WINTER, SPECIES_CHANSEY},
        [8] = {SPECIES_FURFROU_KABUKI, SPECIES_INDEEDEE_FEMALE, SPECIES_STOUTLAND, SPECIES_GREEDENT, SPECIES_MELOETTA, SPECIES_SNORLAX_GIGA, SPECIES_DUBWOOL, SPECIES_ZOROARK_H, SPECIES_EXPLOUD, SPECIES_DRAMPA},
        [9] = {SPECIES_AMBIPOM, SPECIES_HELIOLISK, SPECIES_OBSTAGOON, SPECIES_SAWSBUCK, SPECIES_LOPUNNY, SPECIES_FURFROU_STAR, SPECIES_CINCCINO, SPECIES_UNFEZANT, SPECIES_FURFROU, SPECIES_PIDGEOT},
    },
    [CLASS_SCHOOL_KID] = {
        [0] = {SPECIES_SKWOVET, SPECIES_PICHU_SPIKY, SPECIES_OINKOLOGNE_FEMALE, SPECIES_IGGLYBUFF, SPECIES_LECHONK, SPECIES_PATRAT, SPECIES_SQUAWKABILLY, SPECIES_RAGING_BOLT, SPECIES_ZIGZAGOON_G, SPECIES_CYCLIZAR},
        [1] = {SPECIES_YAMPER, SPECIES_TAILLOW, SPECIES_WHISMUR, SPECIES_MEOWTH_GIGA, SPECIES_URSALUNA_BLOODMOON, SPECIES_SQUAWKABILLY_YELLOW, SPECIES_TOXEL, SPECIES_LECHONK, SPECIES_PAWMOT, SPECIES_TANDEMAUS},
        [2] = {SPECIES_PATRAT, SPECIES_TANDEMAUS, SPECIES_AZURILL, SPECIES_MAUSHOLD, SPECIES_FARIGIRAF, SPECIES_DUDUNSPARCE, SPECIES_DEERLING_WINTER, SPECIES_MINCCINO, SPECIES_TERAPAGOS_TERASTAL, SPECIES_LECHONK},
        [3] = {SPECIES_LUXIO, SPECIES_TOGEDEMARU, SPECIES_PACHIRISU, SPECIES_CHINCHOU, SPECIES_LICKITUNG, SPECIES_CHATOT, SPECIES_DELCATTY, SPECIES_PIKACHU_COSPLAY, SPECIES_EELEKTRIK, SPECIES_GEODUDE_A},
        [4] = {SPECIES_RAICHU_A, SPECIES_LUXIO, SPECIES_LOUDRED, SPECIES_DIGGERSBY, SPECIES_PIDGEOTTO, SPECIES_ROTOM_HEAT, SPECIES_SLAKOTH, SPECIES_DODUO, SPECIES_RUFFLET, SPECIES_FARFETCHD},
        [5] = {SPECIES_GLAMEOW, SPECIES_GIRAFARIG, SPECIES_PIKACHU_CAP_ALOLA, SPECIES_LITLEO, SPECIES_PIKACHU_CAP_ORIGINAL, SPECIES_CASTFORM, SPECIES_SMEARGLE, SPECIES_EELEKTRIK, SPECIES_DIGGERSBY, SPECIES_MUNCHLAX},
        [6] = {SPECIES_EMOLGA, SPECIES_GOLEM_A, SPECIES_PYROAR_FEMALE, SPECIES_MORPEKO, SPECIES_FURFROU_DIAMOND, SPECIES_TAPU_KOKO, SPECIES_GREEDENT, SPECIES_PURUGLY, SPECIES_CINCCINO, SPECIES_DRAMPA},
        [7] = {SPECIES_FURFROU_KABUKI, SPECIES_SNORLAX_GIGA, SPECIES_UNFEZANT_F, SPECIES_URSALUNA, SPECIES_AMBIPOM, SPECIES_FEAROW, SPECIES_LANTURN, SPECIES_MORPEKO, SPECIES_MAGNEZONE, SPECIES_STANTLER},
        [8] = {SPECIES_TOXTRICITY_GIGA, SPECIES_MORPEKO_HANGRY, SPECIES_JOLTEON, SPECIES_FURFROU_LA_REINE, SPECIES_ARCEUS_ELECTRIC, SPECIES_PURUGLY, SPECIES_FURFROU_DEBUTANTE, SPECIES_SAWSBUCK_SUMMER, SPECIES_AMBIPOM, SPECIES_WYRDEER},
        [9] = {SPECIES_FURFROU_DEBUTANTE, SPECIES_STUNFISK, SPECIES_KANGASKHAN_MEGA, SPECIES_AMBIPOM, SPECIES_CINCCINO, SPECIES_AMPHAROS, SPECIES_DUBWOOL, SPECIES_SAWSBUCK, SPECIES_ELECTIVIRE, SPECIES_THUNDURUS_THERIAN},
    },
    [CLASS_SR_AND_JR] = {
        [0] = {SPECIES_DOLLIV, SPECIES_TERAPAGOS_STELLAR, SPECIES_PATRAT, SPECIES_URSALUNA_BLOODMOON, SPECIES_TANDEMAUS, SPECIES_ZIGZAGOON, SPECIES_DUDUNSPARCE_THREE, SPECIES_TERAPAGOS, SPECIES_BIDOOF, SPECIES_FARIGIRAF},
        [1] = {SPECIES_HELIOPTILE, SPECIES_DUDUNSPARCE, SPECIES_SMOLIV, SPECIES_TERAPAGOS_STELLAR, SPECIES_ZORUA_H, SPECIES_FLETCHLING, SPECIES_FARIGIRAF, SPECIES_URSALUNA_BLOODMOON, SPECIES_OINKOLOGNE_FEMALE, SPECIES_MEOWTH_GIGA},
        [2] = {SPECIES_IGGLYBUFF, SPECIES_FARIGIRAF, SPECIES_SPEAROW, SPECIES_WHISMUR, SPECIES_HOOTHOOT, SPECIES_SENTRET, SPECIES_DEERLING_SUMMER, SPECIES_RATTATA_A, SPECIES_DOLLIV, SPECIES_OINKOLOGNE_FEMALE},
        [3] = {SPECIES_LICKITUNG, SPECIES_SMEARGLE, SPECIES_WOOLOO, SPECIES_DUNSPARCE, SPECIES_TYPE_NULL, SPECIES_FARFETCHD, SPECIES_SWABLU, SPECIES_JIGGLYPUFF, SPECIES_LINOONE_G, SPECIES_TRUMBEAK},
        [4] = {SPECIES_TEDDIURSA, SPECIES_STUFFUL, SPECIES_DUNSPARCE, SPECIES_LINOONE, SPECIES_CHATOT, SPECIES_RATICATE, SPECIES_EEVEE, SPECIES_RATICATE_A, SPECIES_LITLEO, SPECIES_FURRET},
        [5] = {SPECIES_SLAKOTH, SPECIES_FARFETCHD, SPECIES_DUNSPARCE, SPECIES_EEVEE, SPECIES_LOUDRED, SPECIES_RATICATE, SPECIES_TRANQUILL, SPECIES_TEDDIURSA, SPECIES_AIPOM, SPECIES_DIGGERSBY},
        [6] = {SPECIES_SAWSBUCK_AUTUMN, SPECIES_SAWSBUCK_SUMMER, SPECIES_SAWSBUCK_WINTER, SPECIES_INDEEDEE, SPECIES_FURFROU_DANDY, SPECIES_AUDINO_MEGA, SPECIES_KANGASKHAN_MEGA, SPECIES_TOUCANNON, SPECIES_ZOROARK_H, SPECIES_FURFROU_LA_REINE},
        [7] = {SPECIES_STARAPTOR, SPECIES_LOPUNNY, SPECIES_FURFROU_DANDY, SPECIES_TOUCANNON, SPECIES_URSARING, SPECIES_FURFROU_DEBUTANTE, SPECIES_FURFROU, SPECIES_SNORLAX_GIGA, SPECIES_CHANSEY, SPECIES_TAUROS},
        [8] = {SPECIES_PYROAR, SPECIES_EXPLOUD, SPECIES_BOUFFALANT, SPECIES_STANTLER, SPECIES_ORANGURU, SPECIES_HAPPINY, SPECIES_URSALUNA, SPECIES_FURFROU_DANDY, SPECIES_OBSTAGOON, SPECIES_LOPUNNY},
        [9] = {SPECIES_FURFROU_STAR, SPECIES_MELOETTA_PIROUETTE, SPECIES_PYROAR_FEMALE, SPECIES_DODRIO, SPECIES_BLISSEY, SPECIES_FURFROU_LA_REINE, SPECIES_HAPPINY, SPECIES_SAWSBUCK_SUMMER, SPECIES_URSALUNA, SPECIES_FEAROW},
    },
    [CLASS_POKEFAN] = {
        [0] = {SPECIES_PATRAT, SPECIES_RATTATA_A, SPECIES_ZIGZAGOON, SPECIES_FARIGIRAF, SPECIES_DEERLING, SPECIES_DITTO, SPECIES_PIKIPEK, SPECIES_ZIGZAGOON_G, SPECIES_SHROODLE, SPECIES_CYCLIZAR},
        [1] = {SPECIES_DOLLIV, SPECIES_MEOWTH, SPECIES_DEERLING_AUTUMN, SPECIES_FLETCHLING, SPECIES_DITTO, SPECIES_TAILLOW, SPECIES_LILLIPUP, SPECIES_OINKOLOGNE, SPECIES_MINCCINO, SPECIES_BUNNELBY},
        [2] = {SPECIES_PIDOVE, SPECIES_ZIGZAGOON_G, SPECIES_SQUAWKABILLY_YELLOW, SPECIES_YUNGOOS, SPECIES_FARIGIRAF, SPECIES_TERAPAGOS_TERASTAL, SPECIES_IGGLYBUFF, SPECIES_AZURILL, SPECIES_DEERLING, SPECIES_FLETCHLING},
        [3] = {SPECIES_LINOONE_G, SPECIES_STUFFUL, SPECIES_EEVEE_GIGA, SPECIES_VIGOROTH, SPECIES_BUNEARY, SPECIES_MUNCHLAX, SPECIES_WATCHOG, SPECIES_TRANQUILL, SPECIES_RUFFLET, SPECIES_JIGGLYPUFF},
        [4] = {SPECIES_SPINDA, SPECIES_WATCHOG, SPECIES_LOUDRED, SPECIES_WIGGLYTUFF, SPECIES_RATICATE, SPECIES_GIRAFARIG, SPECIES_TEDDIURSA, SPECIES_RATICATE_A, SPECIES_STARAVIA, SPECIES_GUMSHOOS},
        [5] = {SPECIES_SMEARGLE, SPECIES_EEVEE, SPECIES_STUFFUL, SPECIES_TEDDIURSA, SPECIES_GLAMEOW, SPECIES_LINOONE_G, SPECIES_RATICATE, SPECIES_JIGGLYPUFF, SPECIES_TRUMBEAK, SPECIES_STARAVIA},
        [6] = {SPECIES_PORYGON2, SPECIES_INDEEDEE, SPECIES_FURFROU_MATRON, SPECIES_LICKILICKY, SPECIES_BEWEAR, SPECIES_FURFROU, SPECIES_UNFEZANT_F, SPECIES_URSALUNA, SPECIES_CHANSEY, SPECIES_PIDGEOT_MEGA},
        [7] = {SPECIES_PORYGON_Z, SPECIES_PURUGLY, SPECIES_MILTANK, SPECIES_SAWSBUCK_SUMMER, SPECIES_LOPUNNY_MEGA, SPECIES_HAPPINY, SPECIES_FURFROU_DEBUTANTE, SPECIES_SLAKING, SPECIES_UNFEZANT_F, SPECIES_FURFROU_HEART},
        [8] = {SPECIES_DODRIO, SPECIES_CHANSEY, SPECIES_SWELLOW, SPECIES_BLISSEY, SPECIES_CINCCINO, SPECIES_HAPPINY, SPECIES_PORYGON_Z, SPECIES_ZOROARK_H, SPECIES_PIDGEOT, SPECIES_FURFROU_DIAMOND},
        [9] = {SPECIES_ARCEUS, SPECIES_FURFROU_KABUKI, SPECIES_MELOETTA_PIROUETTE, SPECIES_FURFROU_DEBUTANTE, SPECIES_INDEEDEE_FEMALE, SPECIES_KANGASKHAN_MEGA, SPECIES_SNORLAX, SPECIES_SAWSBUCK_AUTUMN, SPECIES_HAPPINY, SPECIES_KANGASKHAN},
    },
    [CLASS_EXPERT] = {
        [0] = {SPECIES_UNOWN_Z, SPECIES_COSMOG, SPECIES_UNOWN_T, SPECIES_WYNAUT, SPECIES_ELGYEM, SPECIES_OKIDOGI, SPECIES_KORAIDON, SPECIES_UNOWN_D, SPECIES_INKAY, SPECIES_FARIGIRAF},
        [1] = {SPECIES_UNOWN_Y, SPECIES_UNOWN_K, SPECIES_IRON_VALIANT, SPECIES_CLOBBOPUS, SPECIES_ARMAROUGE, SPECIES_PAWMO, SPECIES_TAUROS_BLAZE_P, SPECIES_GREAT_TUSK, SPECIES_UNOWN_M, SPECIES_TAUROS_P},
        [2] = {SPECIES_GOTHITA, SPECIES_KORAIDON, SPECIES_BALTOY, SPECIES_ANNIHILAPE, SPECIES_UNOWN_O, SPECIES_SOLOSIS, SPECIES_INKAY, SPECIES_UNOWN_EXCLAMATION, SPECIES_UNOWN_X, SPECIES_IRON_HANDS},
        [3] = {SPECIES_FARFETCHD_G, SPECIES_SPOINK, SPECIES_SILVALLY_FIGHT, SPECIES_MUNNA, SPECIES_SNEASEL_H, SPECIES_SCRAGGY, SPECIES_RIOLU, SPECIES_SWOOBAT, SPECIES_COSMOEM, SPECIES_RALTS},
        [4] = {SPECIES_RIOLU, SPECIES_PSYDUCK, SPECIES_SNEASEL_H, SPECIES_SLOWPOKE, SPECIES_MONFERNO, SPECIES_CHINGLING, SPECIES_HATTREM, SPECIES_PIKACHU_PHD, SPECIES_DROWZEE, SPECIES_JYNX},
        [5] = {SPECIES_CRABRAWLER, SPECIES_SILVALLY_FIGHT, SPECIES_STUFFUL, SPECIES_PANCHAM, SPECIES_MAKUHITA, SPECIES_COSMOEM, SPECIES_TYROGUE, SPECIES_SNEASLER, SPECIES_RALTS, SPECIES_PIKACHU_LIBRE},
        [6] = {SPECIES_URSHIFU_SINGLE, SPECIES_MEDICHAM, SPECIES_MESPRIT, SPECIES_GALLADE_MEGA, SPECIES_MEOWSTIC, SPECIES_ZAMAZENTA, SPECIES_NOCTOWL, SPECIES_ESPEON, SPECIES_URSHIFU_RAPID_GIGA, SPECIES_BEWEAR},
        [7] = {SPECIES_HOOPA_UNBOUND, SPECIES_MIENSHAO, SPECIES_EXEGGUTOR, SPECIES_BEARTIC, SPECIES_SOLROCK, SPECIES_PONYTA_G, SPECIES_GARDEVOIR_MEGA, SPECIES_HOOPA, SPECIES_DARMANITANZEN, SPECIES_CALYREX_SHADOW_RIDER},
        [8] = {SPECIES_SAMUROTT, SPECIES_NECROZMA_ULTRA, SPECIES_BEARTIC, SPECIES_SLOWKING_G, SPECIES_ZAPDOS_G, SPECIES_MEWTWO_MEGA_Y, SPECIES_HAWLUCHA, SPECIES_DEOXYS_DEFENSE, SPECIES_BEWEAR, SPECIES_GRAPPLOCT},
        [9] = {SPECIES_BUZZWOLE, SPECIES_LATIOS, SPECIES_MEOWSTIC, SPECIES_WYRDEER, SPECIES_MEWTWO_MEGA_Y, SPECIES_EXEGGUTOR, SPECIES_NECROZMA_DUSK_MANE, SPECIES_MACHAMP_GIGA, SPECIES_URSHIFU_SINGLE, SPECIES_HOOPA},
    },
    [CLASS_YOUNGSTER_RS] = {
        [0] = {SPECIES_MINCCINO, SPECIES_ZIGZAGOON_G, SPECIES_TERAPAGOS, SPECIES_URSALUNA_BLOODMOON, SPECIES_BUNNELBY, SPECIES_SENTRET, SPECIES_FLETCHLING, SPECIES_GRAFAIAI, SPECIES_SQUAWKABILLY, SPECIES_LECHONK},
        [1] = {SPECIES_SQUAWKABILLY_YELLOW, SPECIES_MAUSHOLD_FOUR, SPECIES_RATTATA, SPECIES_BUNNELBY, SPECIES_MAUSHOLD, SPECIES_LECHONK, SPECIES_DUDUNSPARCE, SPECIES_SPEAROW, SPECIES_MEOWTH_GIGA, SPECIES_BIDOOF},
        [2] = {SPECIES_HELIOPTILE, SPECIES_DUDUNSPARCE, SPECIES_DUDUNSPARCE_THREE, SPECIES_GRAFAIAI, SPECIES_MEOWTH, SPECIES_EGG, SPECIES_DEERLING_AUTUMN, SPECIES_SQUAWKABILLY_YELLOW, SPECIES_TERAPAGOS, SPECIES_BIDOOF},
        [3] = {SPECIES_LOUDRED, SPECIES_EEVEE, SPECIES_SPINDA, SPECIES_GIRAFARIG, SPECIES_SLAKOTH, SPECIES_TYPE_NULL, SPECIES_BIBAREL, SPECIES_GLAMEOW, SPECIES_HERDIER, SPECIES_EEVEE_GIGA},
        [4] = {SPECIES_TRUMBEAK, SPECIES_BIBAREL, SPECIES_JIGGLYPUFF, SPECIES_BUNEARY, SPECIES_LINOONE, SPECIES_TYPE_NULL, SPECIES_DUNSPARCE, SPECIES_KECLEON, SPECIES_STUFFUL, SPECIES_LICKITUNG},
        [5] = {SPECIES_WOOLOO, SPECIES_BUNEARY, SPECIES_RATICATE_A, SPECIES_PORYGON, SPECIES_CASTFORM, SPECIES_AIPOM, SPECIES_SLAKOTH, SPECIES_SMEARGLE, SPECIES_EEVEE_GIGA, SPECIES_TRUMBEAK},
        [6] = {SPECIES_INDEEDEE, SPECIES_OBSTAGOON, SPECIES_SAWSBUCK_SUMMER, SPECIES_FURFROU_DANDY, SPECIES_KOMALA, SPECIES_STARAPTOR, SPECIES_TAUROS, SPECIES_BEWEAR, SPECIES_DODRIO, SPECIES_STOUTLAND},
        [7] = {SPECIES_FURFROU_HEART, SPECIES_FURFROU_STAR, SPECIES_KANGASKHAN_MEGA, SPECIES_WYRDEER, SPECIES_SWELLOW, SPECIES_MILTANK, SPECIES_LOPUNNY_MEGA, SPECIES_LICKILICKY, SPECIES_SNORLAX, SPECIES_SAWSBUCK_SUMMER},
        [8] = {SPECIES_INDEEDEE, SPECIES_TOUCANNON, SPECIES_KANGASKHAN, SPECIES_FURFROU_LA_REINE, SPECIES_SAWSBUCK, SPECIES_FURFROU_PHAROAH, SPECIES_LOPUNNY_MEGA, SPECIES_OBSTAGOON, SPECIES_ARCEUS, SPECIES_STARAPTOR},
        [9] = {SPECIES_CINCCINO, SPECIES_DODRIO, SPECIES_AMBIPOM, SPECIES_INDEEDEE, SPECIES_FURFROU_DANDY, SPECIES_KANGASKHAN_MEGA, SPECIES_LICKILICKY, SPECIES_ARCEUS, SPECIES_INDEEDEE_FEMALE, SPECIES_AUDINO_MEGA},
    },
    [CLASS_CHAMPION_RS] = {
        [0] = {SPECIES_SMOLIV, SPECIES_AZURILL, SPECIES_WHISMUR, SPECIES_EGG, SPECIES_TANDEMAUS, SPECIES_DUDUNSPARCE, SPECIES_DUDUNSPARCE_THREE, SPECIES_TAILLOW, SPECIES_MEOWTH_GIGA, SPECIES_OINKOLOGNE},
        [1] = {SPECIES_TERAPAGOS_TERASTAL, SPECIES_DUDUNSPARCE_THREE, SPECIES_SKWOVET, SPECIES_LILLIPUP, SPECIES_ZIGZAGOON_G, SPECIES_SPEAROW, SPECIES_URSALUNA_BLOODMOON, SPECIES_CYCLIZAR, SPECIES_RATTATA, SPECIES_SQUAWKABILLY_WHITE},
        [2] = {SPECIES_HELIOPTILE, SPECIES_OINKOLOGNE_FEMALE, SPECIES_DUDUNSPARCE_THREE, SPECIES_EGG, SPECIES_GRAFAIAI, SPECIES_IGGLYBUFF, SPECIES_RATTATA_A, SPECIES_MEOWTH, SPECIES_DEERLING, SPECIES_OINKOLOGNE},
        [3] = {SPECIES_RATICATE_A, SPECIES_SMEARGLE, SPECIES_TEDDIURSA, SPECIES_TYPE_NULL, SPECIES_FURRET, SPECIES_CASTFORM, SPECIES_RUFFLET, SPECIES_FARFETCHD, SPECIES_KECLEON, SPECIES_LITLEO},
        [4] = {SPECIES_GLAMEOW, SPECIES_HERDIER, SPECIES_PIDGEOTTO, SPECIES_EEVEE_GIGA, SPECIES_PORYGON, SPECIES_TEDDIURSA, SPECIES_SMEARGLE, SPECIES_LOUDRED, SPECIES_CASTFORM, SPECIES_WIGGLYTUFF},
        [5] = {SPECIES_RATICATE, SPECIES_BUNEARY, SPECIES_PIDGEOTTO, SPECIES_STARAVIA, SPECIES_EEVEE_GIGA, SPECIES_HERDIER, SPECIES_EEVEE, SPECIES_WIGGLYTUFF, SPECIES_CHATOT, SPECIES_SPINDA},
        [6] = {SPECIES_FURFROU_DEBUTANTE, SPECIES_DRAMPA, SPECIES_AUDINO, SPECIES_URSALUNA, SPECIES_SAWSBUCK_AUTUMN, SPECIES_WYRDEER, SPECIES_HAPPINY, SPECIES_SAWSBUCK_SUMMER, SPECIES_FURFROU_PHAROAH, SPECIES_ZOROARK_H},
        [7] = {SPECIES_SLAKING, SPECIES_FURFROU_LA_REINE, SPECIES_INDEEDEE, SPECIES_PURUGLY, SPECIES_URSARING, SPECIES_LICKILICKY, SPECIES_MELOETTA_PIROUETTE, SPECIES_DODRIO, SPECIES_SAWSBUCK, SPECIES_PORYGON_Z},
        [8] = {SPECIES_PORYGON_Z, SPECIES_PYROAR_FEMALE, SPECIES_ORANGURU, SPECIES_SAWSBUCK_SUMMER, SPECIES_UNFEZANT, SPECIES_STANTLER, SPECIES_INDEEDEE_FEMALE, SPECIES_BRAVIARY, SPECIES_OBSTAGOON, SPECIES_TAUROS},
        [9] = {SPECIES_HAPPINY, SPECIES_PIDGEOT, SPECIES_FURFROU_DANDY, SPECIES_STOUTLAND, SPECIES_BRAVIARY, SPECIES_MELOETTA, SPECIES_WYRDEER, SPECIES_INDEEDEE, SPECIES_PYROAR_FEMALE, SPECIES_AUDINO_MEGA},
    },
    [CLASS_FISHERMAN_RS] = {
        [0] = {SPECIES_SHROODLE, SPECIES_GRAFAIAI, SPECIES_FARIGIRAF, SPECIES_TERAPAGOS_STELLAR, SPECIES_MEOWTH_GIGA, SPECIES_SKITTY, SPECIES_URSALUNA_BLOODMOON, SPECIES_DEERLING_SUMMER, SPECIES_TERAPAGOS_TERASTAL, SPECIES_SENTRET},
        [1] = {SPECIES_OINKOLOGNE_FEMALE, SPECIES_ZORUA_H, SPECIES_SENTRET, SPECIES_SKWOVET, SPECIES_RATTATA, SPECIES_EGG, SPECIES_HELIOPTILE, SPECIES_LILLIPUP, SPECIES_HOOTHOOT, SPECIES_DUDUNSPARCE},
        [2] = {SPECIES_BIDOOF, SPECIES_URSALUNA_BLOODMOON, SPECIES_TAILLOW, SPECIES_MINCCINO, SPECIES_WHISMUR, SPECIES_ARBOLIVA, SPECIES_DITTO, SPECIES_OINKOLOGNE, SPECIES_FARIGIRAF, SPECIES_IGGLYBUFF},
        [3] = {SPECIES_GIRAFARIG, SPECIES_GLAMEOW, SPECIES_TYPE_NULL, SPECIES_DUNSPARCE, SPECIES_RATICATE_A, SPECIES_RATICATE, SPECIES_TRANQUILL, SPECIES_STARAVIA, SPECIES_SWABLU, SPECIES_SLAKOTH},
        [4] = {SPECIES_STUFFUL, SPECIES_DODUO, SPECIES_GIRAFARIG, SPECIES_BUNEARY, SPECIES_SLAKOTH, SPECIES_DELCATTY, SPECIES_PIDGEOTTO, SPECIES_KECLEON, SPECIES_RUFFLET, SPECIES_TRUMBEAK},
        [5] = {SPECIES_LINOONE_G, SPECIES_STARAVIA, SPECIES_DIGGERSBY, SPECIES_FARFETCHD, SPECIES_LITLEO, SPECIES_DUNSPARCE, SPECIES_RATICATE, SPECIES_RUFFLET, SPECIES_SMEARGLE, SPECIES_GLAMEOW},
        [6] = {SPECIES_PORYGON2, SPECIES_ZANGOOSE, SPECIES_KOMALA, SPECIES_SAWSBUCK_SUMMER, SPECIES_SNORLAX, SPECIES_PIDGEOT_MEGA, SPECIES_TAUROS, SPECIES_STANTLER, SPECIES_SAWSBUCK, SPECIES_HELIOLISK},
        [7] = {SPECIES_PORYGON2, SPECIES_FURFROU_DIAMOND, SPECIES_TOUCANNON, SPECIES_FURFROU, SPECIES_SAWSBUCK_AUTUMN, SPECIES_WYRDEER, SPECIES_URSALUNA, SPECIES_ZANGOOSE, SPECIES_FURFROU_STAR, SPECIES_AUDINO},
        [8] = {SPECIES_PIDGEOT_MEGA, SPECIES_PORYGON_Z, SPECIES_FURFROU_DANDY, SPECIES_REGIGIGAS, SPECIES_TAUROS, SPECIES_INDEEDEE, SPECIES_ZOROARK_H, SPECIES_PIDGEOT, SPECIES_LICKILICKY, SPECIES_MELOETTA_PIROUETTE},
        [9] = {SPECIES_REGIGIGAS, SPECIES_FURFROU_MATRON, SPECIES_DRAMPA, SPECIES_SAWSBUCK_AUTUMN, SPECIES_SILVALLY, SPECIES_LOPUNNY_MEGA, SPECIES_STARAPTOR, SPECIES_INDEEDEE, SPECIES_MELOETTA, SPECIES_SNORLAX},
    },
    [CLASS_TRIATHLETE] = {
        [0] = {SPECIES_TAUROS_P, SPECIES_YAMPER, SPECIES_TADBULB, SPECIES_TAUROS_AQUA_P, SPECIES_BELLIBOLT, SPECIES_KILOWATTREL, SPECIES_MIRAIDON, SPECIES_SLITHER_WING, SPECIES_PICHU, SPECIES_IRON_THORNS},
        [1] = {SPECIES_FLAMIGO, SPECIES_ANNIHILAPE, SPECIES_TYNAMO, SPECIES_PAWMOT, SPECIES_JOLTIK, SPECIES_MAREEP, SPECIES_SHINX, SPECIES_PAWMI, SPECIES_SANDY_SHOCKS, SPECIES_HELIOPTILE},
        [2] = {SPECIES_CLOBBOPUS, SPECIES_PICHU_SPIKY, SPECIES_TAUROS_AQUA_P, SPECIES_TOXEL, SPECIES_KORAIDON, SPECIES_KILOWATTREL, SPECIES_PAWMI, SPECIES_PAWMO, SPECIES_OKIDOGI, SPECIES_KUBFU},
        [3] = {SPECIES_PIKACHU_GIGA, SPECIES_PIGNITE, SPECIES_MANKEY, SPECIES_PIKACHU_CAP_UNOVA, SPECIES_RAICHU_A, SPECIES_VOLTORB, SPECIES_ELECTRODE_H, SPECIES_TIMBURR, SPECIES_PACHIRISU, SPECIES_PIKACHU_FLYING},
        [4] = {SPECIES_TOGEDEMARU, SPECIES_ELECTRIKE, SPECIES_MONFERNO, SPECIES_GRAVELER_A, SPECIES_FLAAFFY, SPECIES_SNEASLER, SPECIES_ROTOM, SPECIES_MACHOP, SPECIES_PIKACHU_BELLE, SPECIES_PIKACHU_GIGA},
        [5] = {SPECIES_PIKACHU, SPECIES_GRAVELER_A, SPECIES_RAICHU, SPECIES_MINUN, SPECIES_ROTOM_FROST, SPECIES_EELEKTRIK, SPECIES_PIKACHU_FLYING, SPECIES_PIKACHU_CAP_HOENN, SPECIES_MEDITITE, SPECIES_PIKACHU_CAP_ALOLA},
        [6] = {SPECIES_RAIKOU, SPECIES_VIKAVOLT, SPECIES_TAPU_KOKO, SPECIES_ZAMAZENTA, SPECIES_TOXTRICITY_GIGA, SPECIES_KELDEO, SPECIES_EELEKTROSS, SPECIES_ELECTRODE, SPECIES_ZERAORA, SPECIES_THROH},
        [7] = {SPECIES_LUCARIO_MEGA, SPECIES_INFERNAPE, SPECIES_THROH, SPECIES_BLAZIKEN_MEGA, SPECIES_MEWTWO_MEGA_X, SPECIES_PHEROMOSA, SPECIES_GRAPPLOCT, SPECIES_MACHAMP, SPECIES_BRELOOM, SPECIES_URSHIFU_RAPID_GIGA},
        [8] = {SPECIES_ARCTOZOLT, SPECIES_BUZZWOLE, SPECIES_INFERNAPE, SPECIES_PANGORO, SPECIES_BLAZIKEN, SPECIES_THROH, SPECIES_URSHIFU_SINGLE, SPECIES_LUXRAY, SPECIES_MARSHADOW, SPECIES_VIKAVOLT},
        [9] = {SPECIES_TOXTRICITY_LOW_KEY_GIGA, SPECIES_MANECTRIC_MEGA, SPECIES_ARCEUS_FIGHT, SPECIES_LUCARIO, SPECIES_SAWK, SPECIES_CHESNAUGHT, SPECIES_URSHIFU_SINGLE, SPECIES_THROH, SPECIES_URSHIFU_RAPID, SPECIES_KELDEO},
    },
    [CLASS_DRAGON_TAMER] = {
        [0] = {SPECIES_GOUGING_FIRE, SPECIES_MIRAIDON, SPECIES_ARCTIBAX, SPECIES_NOIBAT, SPECIES_FRIGIBAX, SPECIES_GIBLE, SPECIES_TATSUGIRI_RED, SPECIES_BAXCALIBUR, SPECIES_HYDRAPPLE, SPECIES_AXEW},
        [1] = {SPECIES_ROARING_MOON, SPECIES_HYDRAPPLE, SPECIES_BAXCALIBUR, SPECIES_ARCTIBAX, SPECIES_APPLIN, SPECIES_GOOMY, SPECIES_DRATINI, SPECIES_FRIGIBAX, SPECIES_RAGING_BOLT, SPECIES_CYCLIZAR},
        [2] = {SPECIES_ARCTIBAX, SPECIES_DREEPY, SPECIES_RAGING_BOLT, SPECIES_GOUGING_FIRE, SPECIES_AXEW, SPECIES_NOIBAT, SPECIES_CYCLIZAR, SPECIES_DEINO, SPECIES_MIRAIDON, SPECIES_KORAIDON},
        [3] = {SPECIES_ZWEILOUS, SPECIES_BAGON, SPECIES_TYRUNT, SPECIES_SHELGON, SPECIES_HAKAMO_O, SPECIES_GABITE, SPECIES_VIBRAVA, SPECIES_SILVALLY_DRAGON, SPECIES_FRAXURE, SPECIES_JANGMO_O},
        [4] = {SPECIES_BAGON, SPECIES_GABITE, SPECIES_JANGMO_O, SPECIES_DRAKLOAK, SPECIES_FRAXURE, SPECIES_HAKAMO_O, SPECIES_SILVALLY_DRAGON, SPECIES_ZWEILOUS, SPECIES_TYRUNT, SPECIES_DRAGONAIR},
        [5] = {SPECIES_HAKAMO_O, SPECIES_FRAXURE, SPECIES_DRAKLOAK, SPECIES_JANGMO_O, SPECIES_VIBRAVA, SPECIES_SHELGON, SPECIES_DRAGONAIR, SPECIES_GABITE, SPECIES_TYRUNT, SPECIES_ZWEILOUS},
        [6] = {SPECIES_DIALGA, SPECIES_RAYQUAZA_MEGA, SPECIES_HYDREIGON, SPECIES_GUZZLORD, SPECIES_DRUDDIGON, SPECIES_ETERNATUS_ETERNAMAX, SPECIES_DURALUDON, SPECIES_GIRATINA_ORIGIN, SPECIES_PALKIA, SPECIES_LATIAS_MEGA},
        [7] = {SPECIES_PALKIA, SPECIES_DRAGAPULT, SPECIES_ALTARIA_MEGA, SPECIES_FLAPPLE_GIGA, SPECIES_LATIOS, SPECIES_TURTONATOR, SPECIES_ARCEUS_DRAGON, SPECIES_GOODRA_H, SPECIES_DRACOZOLT, SPECIES_NECROZMA_ULTRA},
        [8] = {SPECIES_GARCHOMP, SPECIES_HAXORUS, SPECIES_ALTARIA, SPECIES_KYUREM_BLACK, SPECIES_KINGDRA, SPECIES_DRAGAPULT, SPECIES_LATIAS, SPECIES_ZEKROM, SPECIES_RESHIRAM, SPECIES_FLAPPLE},
        [9] = {SPECIES_GIRATINA, SPECIES_GUZZLORD, SPECIES_GARCHOMP_MEGA, SPECIES_ZYGARDE_10, SPECIES_KINGDRA, SPECIES_RAYQUAZA, SPECIES_SCEPTILE_MEGA, SPECIES_DRAGALGE, SPECIES_AMPHAROS_MEGA, SPECIES_DRACOZOLT},
    },
    [CLASS_BIRD_KEEPER_RS] = {
        [0] = {SPECIES_TERAPAGOS_STELLAR, SPECIES_RATTATA, SPECIES_DUDUNSPARCE, SPECIES_DITTO, SPECIES_IGGLYBUFF, SPECIES_DOLLIV, SPECIES_WHISMUR, SPECIES_TAILLOW, SPECIES_ARBOLIVA, SPECIES_FLETCHLING},
        [1] = {SPECIES_SQUAWKABILLY_YELLOW, SPECIES_DEERLING_AUTUMN, SPECIES_SPEAROW, SPECIES_MINCCINO, SPECIES_PIKIPEK, SPECIES_SKWOVET, SPECIES_GRAFAIAI, SPECIES_LECHONK, SPECIES_ARBOLIVA, SPECIES_BIDOOF},
        [2] = {SPECIES_HOOTHOOT, SPECIES_TERAPAGOS_STELLAR, SPECIES_MAUSHOLD_FOUR, SPECIES_DITTO, SPECIES_DUDUNSPARCE_THREE, SPECIES_SQUAWKABILLY_YELLOW, SPECIES_SQUAWKABILLY_WHITE, SPECIES_HELIOPTILE, SPECIES_DOLLIV, SPECIES_URSALUNA_BLOODMOON},
        [3] = {SPECIES_WATCHOG, SPECIES_STARAVIA, SPECIES_PERSIAN, SPECIES_SPINDA, SPECIES_LITLEO, SPECIES_AIPOM, SPECIES_LICKITUNG, SPECIES_DIGGERSBY, SPECIES_GUMSHOOS, SPECIES_JIGGLYPUFF},
        [4] = {SPECIES_LOUDRED, SPECIES_BIBAREL, SPECIES_TYPE_NULL, SPECIES_GIRAFARIG, SPECIES_SMEARGLE, SPECIES_RATICATE, SPECIES_DODUO, SPECIES_CASTFORM, SPECIES_TRUMBEAK, SPECIES_PORYGON},
        [5] = {SPECIES_WIGGLYTUFF, SPECIES_DUNSPARCE, SPECIES_TYPE_NULL, SPECIES_SWABLU, SPECIES_SMEARGLE, SPECIES_LINOONE, SPECIES_PIDGEOTTO, SPECIES_WATCHOG, SPECIES_EEVEE_GIGA, SPECIES_LICKITUNG},
        [6] = {SPECIES_HAPPINY, SPECIES_SAWSBUCK, SPECIES_STOUTLAND, SPECIES_KANGASKHAN, SPECIES_MILTANK, SPECIES_UNFEZANT_F, SPECIES_BEWEAR, SPECIES_DODRIO, SPECIES_SLAKING, SPECIES_PYROAR_FEMALE},
        [7] = {SPECIES_TAUROS, SPECIES_SAWSBUCK_SUMMER, SPECIES_PYROAR, SPECIES_PURUGLY, SPECIES_FURFROU_LA_REINE, SPECIES_LOPUNNY_MEGA, SPECIES_INDEEDEE, SPECIES_FEAROW, SPECIES_ZANGOOSE, SPECIES_SAWSBUCK},
        [8] = {SPECIES_FEAROW, SPECIES_AUDINO, SPECIES_EXPLOUD, SPECIES_KANGASKHAN, SPECIES_DODRIO, SPECIES_TOUCANNON, SPECIES_ZANGOOSE, SPECIES_INDEEDEE_FEMALE, SPECIES_SAWSBUCK_AUTUMN, SPECIES_FURFROU_DEBUTANTE},
        [9] = {SPECIES_STARAPTOR, SPECIES_PYROAR_FEMALE, SPECIES_LOPUNNY, SPECIES_AUDINO_MEGA, SPECIES_FURFROU_KABUKI, SPECIES_AMBIPOM, SPECIES_LICKILICKY, SPECIES_STANTLER, SPECIES_DODRIO, SPECIES_WYRDEER},
    },
    [CLASS_NINJA_BOY] = {
        [0] = {SPECIES_PURRLOIN, SPECIES_ZIGZAGOON_G, SPECIES_TRUBBISH, SPECIES_KINGAMBIT, SPECIES_POOCHYENA, SPECIES_SANDILE, SPECIES_IRON_JUGULIS, SPECIES_GRAFAIAI, SPECIES_CHI_YU, SPECIES_OKIDOGI},
        [1] = {SPECIES_FEZANDIPITI, SPECIES_INKAY, SPECIES_BRUTE_BONNET, SPECIES_MASCHIFF, SPECIES_NICKIT, SPECIES_GRAFAIAI, SPECIES_WOOPER_P, SPECIES_GLIMMORA, SPECIES_TRUBBISH, SPECIES_ROARING_MOON},
        [2] = {SPECIES_MASCHIFF, SPECIES_WO_CHIEN, SPECIES_BULBASAUR, SPECIES_EKANS, SPECIES_POOCHYENA, SPECIES_TRUBBISH, SPECIES_RATTATA_A, SPECIES_CLODSIRE, SPECIES_WOOPER_P, SPECIES_ZORUA},
        [3] = {SPECIES_SABLEYE, SPECIES_GLOOM, SPECIES_GULPIN, SPECIES_MIGHTYENA, SPECIES_RATICATE_A, SPECIES_WHIRLIPEDE, SPECIES_MORGREM, SPECIES_SABLEYE_MEGA, SPECIES_STUNKY, SPECIES_GASTLY},
        [4] = {SPECIES_TENTACOOL, SPECIES_GASTLY, SPECIES_VENONAT, SPECIES_SILVALLY_POISON, SPECIES_NIDORINO, SPECIES_ZWEILOUS, SPECIES_MURKROW, SPECIES_SNEASEL, SPECIES_VENOMOTH, SPECIES_NIDORINA},
        [5] = {SPECIES_PERSIAN_A, SPECIES_OVERQWIL, SPECIES_GRIMER, SPECIES_ODDISH, SPECIES_MORGREM, SPECIES_HAUNTER, SPECIES_KAKUNA, SPECIES_KOFFING, SPECIES_GULPIN, SPECIES_WHIRLIPEDE},
        [6] = {SPECIES_HYDREIGON, SPECIES_ETERNATUS_ETERNAMAX, SPECIES_CACTURNE, SPECIES_GRIMMSNARL, SPECIES_SKUNTANK, SPECIES_WEEZING_G, SPECIES_GENGAR, SPECIES_ARCEUS_DARK, SPECIES_TOXICROAK, SPECIES_HOUNDOOM},
        [7] = {SPECIES_CACTURNE, SPECIES_SKUNTANK, SPECIES_MALAMAR, SPECIES_DRAGALGE, SPECIES_ARCEUS_POISON, SPECIES_VENUSAUR_MEGA, SPECIES_DARKRAI, SPECIES_GOLBAT, SPECIES_WEAVILE, SPECIES_DRAPION},
        [8] = {SPECIES_MANDIBUZZ, SPECIES_ETERNATUS, SPECIES_MUK, SPECIES_URSHIFU_SINGLE, SPECIES_GRENINJA, SPECIES_NIHILEGO, SPECIES_DARKRAI, SPECIES_SHARPEDO, SPECIES_TOXTRICITY_GIGA, SPECIES_TYRANITAR},
        [9] = {SPECIES_LUXRAY, SPECIES_TENTACRUEL, SPECIES_SLOWKING_G, SPECIES_TYRANITAR, SPECIES_VILEPLUME, SPECIES_CROBAT, SPECIES_ABSOL, SPECIES_VICTREEBEL, SPECIES_SKUNTANK, SPECIES_SHARPEDO},
    },
    [CLASS_BATTLE_GIRL] = {
        [0] = {SPECIES_PAWMOT, SPECIES_KORAIDON, SPECIES_TAUROS_BLAZE_P, SPECIES_IRON_HANDS, SPECIES_ANNIHILAPE, SPECIES_PAWMO, SPECIES_GREAT_TUSK, SPECIES_IRON_VALIANT, SPECIES_TAUROS_AQUA_P, SPECIES_TAUROS_P},
        [1] = {SPECIES_CLOBBOPUS, SPECIES_PAWMOT, SPECIES_IRON_HANDS, SPECIES_KUBFU, SPECIES_TAUROS_AQUA_P, SPECIES_ANNIHILAPE, SPECIES_SLITHER_WING, SPECIES_KORAIDON, SPECIES_FLAMIGO, SPECIES_IRON_VALIANT},
        [2] = {SPECIES_TAUROS_AQUA_P, SPECIES_PAWMOT, SPECIES_GREAT_TUSK, SPECIES_TAUROS_BLAZE_P, SPECIES_FLAMIGO, SPECIES_ANNIHILAPE, SPECIES_CLOBBOPUS, SPECIES_SLITHER_WING, SPECIES_QUAQUAVAL, SPECIES_KUBFU},
        [3] = {SPECIES_MONFERNO, SPECIES_FARFETCHD_G, SPECIES_MAKUHITA, SPECIES_HAKAMO_O, SPECIES_RIOLU, SPECIES_MEDITITE, SPECIES_CRABRAWLER, SPECIES_CROAGUNK, SPECIES_PIGNITE, SPECIES_SCRAGGY},
        [4] = {SPECIES_RIOLU, SPECIES_TYROGUE, SPECIES_MAKUHITA, SPECIES_PANCHAM, SPECIES_SILVALLY_FIGHT, SPECIES_TIMBURR, SPECIES_SNEASLER, SPECIES_SNEASEL_H, SPECIES_CROAGUNK, SPECIES_MACHOP},
        [5] = {SPECIES_LILLIGANT_H, SPECIES_HITMONCHAN, SPECIES_PIKACHU_LIBRE, SPECIES_MIENFOO, SPECIES_MEDITITE, SPECIES_STUFFUL, SPECIES_TYROGUE, SPECIES_COMBUSKEN, SPECIES_PIGNITE, SPECIES_MONFERNO},
        [6] = {SPECIES_SAWK, SPECIES_EMBOAR, SPECIES_SIRFETCHD, SPECIES_LUCARIO, SPECIES_SAMUROTT, SPECIES_MARSHADOW, SPECIES_PHEROMOSA, SPECIES_URSHIFU_SINGLE, SPECIES_GALLADE, SPECIES_KELDEO},
        [7] = {SPECIES_BEARTIC, SPECIES_SAWK, SPECIES_BLAZIKEN_MEGA, SPECIES_HAWLUCHA, SPECIES_MARSHADOW, SPECIES_TOXICROAK, SPECIES_PANGORO, SPECIES_EMBOAR, SPECIES_URSHIFU_SINGLE_GIGA, SPECIES_URSHIFU_RAPID},
        [8] = {SPECIES_KELDEO_RESOLUTE, SPECIES_ARCEUS_FIGHT, SPECIES_POLIWRATH, SPECIES_VIRIZION, SPECIES_MACHAMP, SPECIES_CHESNAUGHT, SPECIES_MEDICHAM, SPECIES_URSHIFU_RAPID_GIGA, SPECIES_INFERNAPE, SPECIES_DECIDUEYE_H},
        [9] = {SPECIES_PANGORO, SPECIES_HERACROSS, SPECIES_VIRIZION, SPECIES_BEWEAR, SPECIES_KELDEO_RESOLUTE, SPECIES_CONKELDURR, SPECIES_BRELOOM, SPECIES_BLAZIKEN_MEGA, SPECIES_CRABOMINABLE, SPECIES_MEDICHAM_MEGA},
    },
    [CLASS_PARASOL_LADY] = {
        [0] = {SPECIES_DONDOZO, SPECIES_MUDKIP, SPECIES_QUAQUAVAL, SPECIES_TOTODILE, SPECIES_FEZANDIPITI, SPECIES_TATSUGIRI_RED, SPECIES_TAUROS_AQUA_P, SPECIES_PIPLUP, SPECIES_TINKATON, SPECIES_QUAXLY},
        [1] = {SPECIES_FIDOUGH, SPECIES_SOBBLE, SPECIES_QUAQUAVAL, SPECIES_TINKATUFF, SPECIES_FLABEBE_WHITE, SPECIES_FROAKIE, SPECIES_QUAXLY, SPECIES_FINIZEN, SPECIES_MAGIKARP, SPECIES_FEZANDIPITI},
        [2] = {SPECIES_TATSUGIRI_RED, SPECIES_FEEBAS, SPECIES_SWIRLIX, SPECIES_POPPLIO, SPECIES_FLABEBE_YELLOW, SPECIES_FLABEBE_BLUE, SPECIES_FEZANDIPITI, SPECIES_FLABEBE, SPECIES_MAGIKARP, SPECIES_FINIZEN},
        [3] = {SPECIES_SHELLDER, SPECIES_MORGREM, SPECIES_STARYU, SPECIES_JIGGLYPUFF, SPECIES_PIKACHU_POP_STAR, SPECIES_CORSOLA, SPECIES_OMANYTE, SPECIES_FLOETTE_WHITE, SPECIES_PSYDUCK, SPECIES_GOLDEEN},
        [4] = {SPECIES_MIME_JR_G, SPECIES_CHINCHOU, SPECIES_SEALEO, SPECIES_TOGEPI, SPECIES_LOTAD, SPECIES_QUAGSIRE, SPECIES_FLOETTE, SPECIES_POLIWHIRL, SPECIES_MANTYKE, SPECIES_BASCULIN_H},
        [5] = {SPECIES_BASCULIN_RED, SPECIES_ARAQUANID, SPECIES_MAREANIE, SPECIES_BRIONNE, SPECIES_RALTS, SPECIES_KIRLIA, SPECIES_FLOETTE_ORANGE, SPECIES_MORELULL, SPECIES_MIME_JR, SPECIES_SHIINOTIC},
        [6] = {SPECIES_SHARPEDO_MEGA, SPECIES_KINGDRA, SPECIES_SWAMPERT, SPECIES_RELICANTH, SPECIES_SEAKING, SPECIES_SUICUNE, SPECIES_PALKIA, SPECIES_WEEZING_G, SPECIES_TENTACRUEL, SPECIES_ARCEUS_FAIRY},
        [7] = {SPECIES_XERNEAS, SPECIES_DREDNAW_GIGA, SPECIES_FERALIGATR, SPECIES_URSHIFU_RAPID, SPECIES_LAPRAS, SPECIES_ENAMORUS, SPECIES_DREDNAW, SPECIES_AUDINO_MEGA, SPECIES_SHARPEDO, SPECIES_POLIWRATH},
        [8] = {SPECIES_FLORGES_BLUE, SPECIES_GRIMMSNARL, SPECIES_POLIWRATH, SPECIES_STARMIE, SPECIES_FLORGES_WHITE, SPECIES_HATTERENE, SPECIES_PRIMARINA, SPECIES_SLOWKING, SPECIES_LUMINEON, SPECIES_KELDEO},
        [9] = {SPECIES_PHIONE, SPECIES_INTELEON_GIGA, SPECIES_URSHIFU_RAPID_GIGA, SPECIES_GASTRODON_EAST, SPECIES_FLOATZEL, SPECIES_ARCEUS_FAIRY, SPECIES_MANAPHY, SPECIES_PALKIA_ORIGIN, SPECIES_BRUXISH, SPECIES_ENAMORUS},
    },
    [CLASS_SWIMMER_F_RS] = {
        [0] = {SPECIES_SQUAWKABILLY_BLUE, SPECIES_TANDEMAUS, SPECIES_PIDGEY, SPECIES_SHROODLE, SPECIES_SMOLIV, SPECIES_LECHONK, SPECIES_ZORUA_H, SPECIES_WHISMUR, SPECIES_MAUSHOLD_FOUR, SPECIES_DUDUNSPARCE},
        [1] = {SPECIES_ARBOLIVA, SPECIES_SQUAWKABILLY, SPECIES_PATRAT, SPECIES_MEOWTH_GIGA, SPECIES_PIDOVE, SPECIES_HOOTHOOT, SPECIES_PIKIPEK, SPECIES_DITTO, SPECIES_EGG, SPECIES_TERAPAGOS},
        [2] = {SPECIES_DEERLING_AUTUMN, SPECIES_SQUAWKABILLY_BLUE, SPECIES_TERAPAGOS, SPECIES_DOLLIV, SPECIES_HELIOPTILE, SPECIES_EGG, SPECIES_DEERLING, SPECIES_DEERLING_WINTER, SPECIES_AZURILL, SPECIES_SPEAROW},
        [3] = {SPECIES_BIBAREL, SPECIES_MUNCHLAX, SPECIES_LICKITUNG, SPECIES_JIGGLYPUFF, SPECIES_FARFETCHD, SPECIES_GLAMEOW, SPECIES_CHATOT, SPECIES_CASTFORM, SPECIES_RATICATE, SPECIES_STUFFUL},
        [4] = {SPECIES_RUFFLET, SPECIES_TEDDIURSA, SPECIES_BUNEARY, SPECIES_DODUO, SPECIES_LOUDRED, SPECIES_LINOONE, SPECIES_DIGGERSBY, SPECIES_WATCHOG, SPECIES_FURRET, SPECIES_TRUMBEAK},
        [5] = {SPECIES_PERSIAN, SPECIES_EEVEE, SPECIES_DUNSPARCE, SPECIES_RATICATE, SPECIES_BIBAREL, SPECIES_STUFFUL, SPECIES_DELCATTY, SPECIES_PIDGEOTTO, SPECIES_RATICATE_A, SPECIES_WIGGLYTUFF},
        [6] = {SPECIES_FURFROU_MATRON, SPECIES_TAUROS, SPECIES_PYROAR, SPECIES_HAPPINY, SPECIES_SNORLAX, SPECIES_FURFROU_DEBUTANTE, SPECIES_MILTANK, SPECIES_ZANGOOSE, SPECIES_ZOROARK_H, SPECIES_SNORLAX_GIGA},
        [7] = {SPECIES_PYROAR, SPECIES_HAPPINY, SPECIES_SNORLAX, SPECIES_BEWEAR, SPECIES_UNFEZANT_F, SPECIES_PURUGLY, SPECIES_LOPUNNY, SPECIES_FURFROU_HEART, SPECIES_FURFROU_STAR, SPECIES_URSALUNA},
        [8] = {SPECIES_SWELLOW, SPECIES_FURFROU_DANDY, SPECIES_AMBIPOM, SPECIES_KOMALA, SPECIES_SAWSBUCK, SPECIES_PURUGLY, SPECIES_FURFROU_DEBUTANTE, SPECIES_PYROAR_FEMALE, SPECIES_FURFROU_MATRON, SPECIES_ZOROARK_H},
        [9] = {SPECIES_FURFROU_PHAROAH, SPECIES_UNFEZANT, SPECIES_AMBIPOM, SPECIES_TAUROS, SPECIES_LOPUNNY_MEGA, SPECIES_PIDGEOT, SPECIES_SNORLAX, SPECIES_BOUFFALANT, SPECIES_HELIOLISK, SPECIES_PORYGON2},
    },
    [CLASS_PICNICKER_RS] = {
        [0] = {SPECIES_BUNNELBY, SPECIES_DUDUNSPARCE_THREE, SPECIES_TAILLOW, SPECIES_DEERLING, SPECIES_CYCLIZAR, SPECIES_LECHONK, SPECIES_URSALUNA_BLOODMOON, SPECIES_STARLY, SPECIES_OINKOLOGNE, SPECIES_FLETCHLING},
        [1] = {SPECIES_BUNNELBY, SPECIES_DEERLING_WINTER, SPECIES_RATTATA_A, SPECIES_SQUAWKABILLY_WHITE, SPECIES_SPEAROW, SPECIES_DUDUNSPARCE_THREE, SPECIES_AZURILL, SPECIES_SQUAWKABILLY_YELLOW, SPECIES_FLETCHLING, SPECIES_MEOWTH},
        [2] = {SPECIES_DEERLING_AUTUMN, SPECIES_TERAPAGOS_TERASTAL, SPECIES_URSALUNA_BLOODMOON, SPECIES_ARBOLIVA, SPECIES_IGGLYBUFF, SPECIES_MEOWTH, SPECIES_SQUAWKABILLY_BLUE, SPECIES_SKITTY, SPECIES_OINKOLOGNE, SPECIES_DEERLING},
        [3] = {SPECIES_FARFETCHD, SPECIES_TRUMBEAK, SPECIES_GUMSHOOS, SPECIES_EEVEE, SPECIES_MUNCHLAX, SPECIES_JIGGLYPUFF, SPECIES_GIRAFARIG, SPECIES_SMEARGLE, SPECIES_SWABLU, SPECIES_FURRET},
        [4] = {SPECIES_DUNSPARCE, SPECIES_SWABLU, SPECIES_TYPE_NULL, SPECIES_WOOLOO, SPECIES_FARFETCHD, SPECIES_JIGGLYPUFF, SPECIES_PORYGON, SPECIES_SPINDA, SPECIES_GLAMEOW, SPECIES_LINOONE},
        [5] = {SPECIES_GIRAFARIG, SPECIES_SPINDA, SPECIES_BIBAREL, SPECIES_RATICATE_A, SPECIES_BUNEARY, SPECIES_WATCHOG, SPECIES_KECLEON, SPECIES_TEDDIURSA, SPECIES_GLAMEOW, SPECIES_JIGGLYPUFF},
        [6] = {SPECIES_ZANGOOSE, SPECIES_PORYGON_Z, SPECIES_FURFROU_LA_REINE, SPECIES_MELOETTA, SPECIES_FURFROU_MATRON, SPECIES_BEWEAR, SPECIES_FURFROU_HEART, SPECIES_DRAMPA, SPECIES_UNFEZANT, SPECIES_REGIGIGAS},
        [7] = {SPECIES_FURFROU_MATRON, SPECIES_EXPLOUD, SPECIES_PYROAR, SPECIES_MILTANK, SPECIES_CHANSEY, SPECIES_DUBWOOL, SPECIES_FURFROU, SPECIES_STARAPTOR, SPECIES_DRAMPA, SPECIES_ORANGURU},
        [8] = {SPECIES_PORYGON2, SPECIES_DODRIO, SPECIES_URSARING, SPECIES_PIDGEOT, SPECIES_UNFEZANT, SPECIES_MELOETTA_PIROUETTE, SPECIES_LICKILICKY, SPECIES_PIDGEOT_MEGA, SPECIES_FURFROU_PHAROAH, SPECIES_STANTLER},
        [9] = {SPECIES_DODRIO, SPECIES_STOUTLAND, SPECIES_INDEEDEE_FEMALE, SPECIES_PYROAR_FEMALE, SPECIES_ARCEUS, SPECIES_AUDINO, SPECIES_BEWEAR, SPECIES_URSARING, SPECIES_REGIGIGAS, SPECIES_FURFROU_PHAROAH},
    },
    [CLASS_TWINS_RS] = {
        [0] = {SPECIES_DEERLING_AUTUMN, SPECIES_DOLLIV, SPECIES_SPEAROW, SPECIES_HELIOPTILE, SPECIES_DEERLING_SUMMER, SPECIES_FLETCHLING, SPECIES_ZIGZAGOON_G, SPECIES_TANDEMAUS, SPECIES_LILLIPUP, SPECIES_TERAPAGOS_TERASTAL},
        [1] = {SPECIES_IGGLYBUFF, SPECIES_WHISMUR, SPECIES_FARIGIRAF, SPECIES_SQUAWKABILLY_BLUE, SPECIES_GRAFAIAI, SPECIES_SMOLIV, SPECIES_SHROODLE, SPECIES_BIDOOF, SPECIES_DUDUNSPARCE_THREE, SPECIES_HELIOPTILE},
        [2] = {SPECIES_TERAPAGOS_TERASTAL, SPECIES_MEOWTH, SPECIES_ZIGZAGOON_G, SPECIES_SHROODLE, SPECIES_TAILLOW, SPECIES_DUDUNSPARCE, SPECIES_SQUAWKABILLY_YELLOW, SPECIES_SQUAWKABILLY, SPECIES_BUNNELBY, SPECIES_AZURILL},
        [3] = {SPECIES_CHATOT, SPECIES_LICKITUNG, SPECIES_WIGGLYTUFF, SPECIES_FURRET, SPECIES_TRANQUILL, SPECIES_DUNSPARCE, SPECIES_EEVEE_GIGA, SPECIES_RATICATE_A, SPECIES_MUNCHLAX, SPECIES_LINOONE},
        [4] = {SPECIES_SPINDA, SPECIES_SLAKOTH, SPECIES_DELCATTY, SPECIES_GIRAFARIG, SPECIES_HERDIER, SPECIES_TRANQUILL, SPECIES_DIGGERSBY, SPECIES_LINOONE, SPECIES_LICKITUNG, SPECIES_SMEARGLE},
        [5] = {SPECIES_STUFFUL, SPECIES_LINOONE_G, SPECIES_DIGGERSBY, SPECIES_PORYGON, SPECIES_CHATOT, SPECIES_CASTFORM, SPECIES_AIPOM, SPECIES_VIGOROTH, SPECIES_SMEARGLE, SPECIES_TEDDIURSA},
        [6] = {SPECIES_MILTANK, SPECIES_SNORLAX_GIGA, SPECIES_UNFEZANT, SPECIES_MELOETTA_PIROUETTE, SPECIES_HAPPINY, SPECIES_ARCEUS, SPECIES_TOUCANNON, SPECIES_SLAKING, SPECIES_REGIGIGAS, SPECIES_FURFROU_LA_REINE},
        [7] = {SPECIES_LOPUNNY_MEGA, SPECIES_INDEEDEE_FEMALE, SPECIES_STANTLER, SPECIES_PURUGLY, SPECIES_FURFROU_DIAMOND, SPECIES_ZANGOOSE, SPECIES_UNFEZANT, SPECIES_FURFROU_KABUKI, SPECIES_OBSTAGOON, SPECIES_CINCCINO},
        [8] = {SPECIES_CINCCINO, SPECIES_ZANGOOSE, SPECIES_TAUROS, SPECIES_URSALUNA, SPECIES_BLISSEY, SPECIES_KANGASKHAN_MEGA, SPECIES_SAWSBUCK_SUMMER, SPECIES_DRAMPA, SPECIES_LICKILICKY, SPECIES_PIDGEOT_MEGA},
        [9] = {SPECIES_KANGASKHAN, SPECIES_TAUROS, SPECIES_TOUCANNON, SPECIES_SLAKING, SPECIES_BEWEAR, SPECIES_BRAVIARY, SPECIES_EXPLOUD, SPECIES_FURFROU_STAR, SPECIES_SNORLAX, SPECIES_LICKILICKY},
    },
    [CLASS_SAILOR_RS] = {
        [0] = {SPECIES_MAUSHOLD_FOUR, SPECIES_FLETCHLING, SPECIES_TANDEMAUS, SPECIES_WHISMUR, SPECIES_LILLIPUP, SPECIES_DOLLIV, SPECIES_STARLY, SPECIES_HOOTHOOT, SPECIES_CYCLIZAR, SPECIES_SKITTY},
        [1] = {SPECIES_SKITTY, SPECIES_ARBOLIVA, SPECIES_RATTATA, SPECIES_SQUAWKABILLY, SPECIES_EGG, SPECIES_TERAPAGOS, SPECIES_ZIGZAGOON, SPECIES_PATRAT, SPECIES_HELIOPTILE, SPECIES_MAUSHOLD_FOUR},
        [2] = {SPECIES_MINCCINO, SPECIES_CYCLIZAR, SPECIES_SKITTY, SPECIES_GRAFAIAI, SPECIES_HOOTHOOT, SPECIES_SQUAWKABILLY_WHITE, SPECIES_URSALUNA_BLOODMOON, SPECIES_DEERLING, SPECIES_DITTO, SPECIES_HELIOPTILE},
        [3] = {SPECIES_JIGGLYPUFF, SPECIES_RUFFLET, SPECIES_GUMSHOOS, SPECIES_VIGOROTH, SPECIES_PERSIAN, SPECIES_BIBAREL, SPECIES_BUNEARY, SPECIES_CHATOT, SPECIES_PORYGON, SPECIES_PIDGEOTTO},
        [4] = {SPECIES_RATICATE, SPECIES_PIDGEOTTO, SPECIES_SPINDA, SPECIES_GUMSHOOS, SPECIES_WOOLOO, SPECIES_GIRAFARIG, SPECIES_EEVEE, SPECIES_LICKITUNG, SPECIES_CASTFORM, SPECIES_EEVEE_GIGA},
        [5] = {SPECIES_HERDIER, SPECIES_SWABLU, SPECIES_JIGGLYPUFF, SPECIES_CHATOT, SPECIES_EEVEE_GIGA, SPECIES_PIDGEOTTO, SPECIES_RATICATE_A, SPECIES_DELCATTY, SPECIES_MUNCHLAX, SPECIES_LICKITUNG},
        [6] = {SPECIES_PYROAR_FEMALE, SPECIES_PIDGEOT, SPECIES_STARAPTOR, SPECIES_LOPUNNY, SPECIES_PYROAR, SPECIES_KANGASKHAN_MEGA, SPECIES_ARCEUS, SPECIES_SAWSBUCK_SUMMER, SPECIES_WYRDEER, SPECIES_FURFROU},
        [7] = {SPECIES_KOMALA, SPECIES_URSARING, SPECIES_DUBWOOL, SPECIES_FEAROW, SPECIES_FURFROU_DEBUTANTE, SPECIES_MILTANK, SPECIES_SAWSBUCK_WINTER, SPECIES_PORYGON2, SPECIES_UNFEZANT_F, SPECIES_MELOETTA_PIROUETTE},
        [8] = {SPECIES_DRAMPA, SPECIES_SNORLAX_GIGA, SPECIES_STANTLER, SPECIES_UNFEZANT_F, SPECIES_URSARING, SPECIES_ORANGURU, SPECIES_BRAVIARY, SPECIES_HAPPINY, SPECIES_FURFROU_STAR, SPECIES_PYROAR_FEMALE},
        [9] = {SPECIES_FURFROU_PHAROAH, SPECIES_PORYGON_Z, SPECIES_HELIOLISK, SPECIES_BOUFFALANT, SPECIES_PORYGON2, SPECIES_KANGASKHAN, SPECIES_BEWEAR, SPECIES_FURFROU_DANDY, SPECIES_REGIGIGAS, SPECIES_STOUTLAND},
    },
    [CLASS_BOARDER] = {
        [0] = {SPECIES_CETODDLE, SPECIES_ARCTIBAX, SPECIES_IRON_BUNDLE, SPECIES_VANILLITE, SPECIES_FRIGIBAX, SPECIES_CETITAN, SPECIES_CHIEN_PAO, SPECIES_DARUMAKA_G, SPECIES_VULPIX_A, SPECIES_BERGMITE},
        [1] = {SPECIES_CETITAN, SPECIES_SNOM, SPECIES_IRON_BUNDLE, SPECIES_ARCTIBAX, SPECIES_BERGMITE, SPECIES_DARUMAKA_G, SPECIES_VULPIX_A, SPECIES_CETODDLE, SPECIES_FRIGIBAX, SPECIES_VANILLITE},
        [2] = {SPECIES_FRIGIBAX, SPECIES_VULPIX_A, SPECIES_CUBCHOO, SPECIES_DARUMAKA_G, SPECIES_SNOM, SPECIES_CETITAN, SPECIES_ARCTIBAX, SPECIES_VANILLITE, SPECIES_CETODDLE, SPECIES_BAXCALIBUR},
        [3] = {SPECIES_SEALEO, SPECIES_SWINUB, SPECIES_VANILLISH, SPECIES_SNOVER, SPECIES_SPHEAL, SPECIES_SMOOCHUM, SPECIES_SANDSHREW_A, SPECIES_SNEASEL, SPECIES_PIKACHU_BELLE, SPECIES_JYNX},
        [4] = {SPECIES_SILVALLY_ICE, SPECIES_SPHEAL, SPECIES_AURORUS, SPECIES_SANDSHREW_A, SPECIES_ROTOM_FROST, SPECIES_SNORUNT, SPECIES_SNOVER, SPECIES_SNEASEL, SPECIES_JYNX, SPECIES_SWINUB},
        [5] = {SPECIES_AURORUS, SPECIES_SNEASEL, SPECIES_SPHEAL, SPECIES_JYNX, SPECIES_ROTOM_FROST, SPECIES_SILVALLY_ICE, SPECIES_SANDSHREW_A, SPECIES_MR_MIME_G, SPECIES_SWINUB, SPECIES_VANILLISH},
        [6] = {SPECIES_ABOMASNOW, SPECIES_ARCEUS_ICE, SPECIES_MAMOSWINE, SPECIES_ARTICUNO, SPECIES_ARCTOZOLT, SPECIES_DARMANITAN_G, SPECIES_DELIBIRD, SPECIES_CRABOMINABLE, SPECIES_AVALUGG_H, SPECIES_WALREIN},
        [7] = {SPECIES_ABOMASNOW, SPECIES_WALREIN, SPECIES_GLACEON, SPECIES_ARTICUNO, SPECIES_FROSLASS, SPECIES_CLOYSTER, SPECIES_EISCUE_NOICE, SPECIES_DEWGONG, SPECIES_ARCTOZOLT, SPECIES_NINETALES_A},
        [8] = {SPECIES_GLASTRIER, SPECIES_KYUREM_WHITE, SPECIES_MR_RIME, SPECIES_VANILLUXE, SPECIES_ARCTOZOLT, SPECIES_DARMANITAN_G, SPECIES_AVALUGG, SPECIES_DARMANITAN_G_ZEN, SPECIES_NINETALES_A, SPECIES_CRABOMINABLE},
        [9] = {SPECIES_FROSLASS, SPECIES_KYUREM_WHITE, SPECIES_ARCTOVISH, SPECIES_MR_RIME, SPECIES_WALREIN, SPECIES_LAPRAS, SPECIES_DARMANITAN_G_ZEN, SPECIES_DELIBIRD, SPECIES_SANDSLASH_A, SPECIES_CLOYSTER},
    },
    [CLASS_COLLECTOR] = {
        [0] = {SPECIES_RATTATA_A, SPECIES_MEOWTH, SPECIES_LILLIPUP, SPECIES_SKITTY, SPECIES_DEERLING_WINTER, SPECIES_YUNGOOS, SPECIES_HELIOPTILE, SPECIES_HYDRAPPLE, SPECIES_DIPPLIN, SPECIES_ARBOLIVA},
        [1] = {SPECIES_ROARING_MOON, SPECIES_SQUAWKABILLY_WHITE, SPECIES_TERAPAGOS_STELLAR, SPECIES_DEERLING, SPECIES_DRATINI, SPECIES_DUDUNSPARCE, SPECIES_FARIGIRAF, SPECIES_ZIGZAGOON_G, SPECIES_TERAPAGOS_TERASTAL, SPECIES_TERAPAGOS},
        [2] = {SPECIES_ROARING_MOON, SPECIES_ZORUA_H, SPECIES_AZURILL, SPECIES_DIPPLIN, SPECIES_MAUSHOLD, SPECIES_TANDEMAUS, SPECIES_AXEW, SPECIES_DEERLING_WINTER, SPECIES_MAUSHOLD_FOUR, SPECIES_TERAPAGOS_STELLAR},
        [3] = {SPECIES_DUNSPARCE, SPECIES_WIGGLYTUFF, SPECIES_JANGMO_O, SPECIES_SWABLU, SPECIES_SLAKOTH, SPECIES_AIPOM, SPECIES_WATCHOG, SPECIES_ZWEILOUS, SPECIES_HERDIER, SPECIES_EEVEE_GIGA},
        [4] = {SPECIES_LOUDRED, SPECIES_RUFFLET, SPECIES_SPINDA, SPECIES_DIGGERSBY, SPECIES_GIRAFARIG, SPECIES_TYPE_NULL, SPECIES_LICKITUNG, SPECIES_JIGGLYPUFF, SPECIES_HERDIER, SPECIES_MUNCHLAX},
        [5] = {SPECIES_DIGGERSBY, SPECIES_CASTFORM, SPECIES_WOOLOO, SPECIES_SMEARGLE, SPECIES_BUNEARY, SPECIES_EEVEE_GIGA, SPECIES_GIRAFARIG, SPECIES_LINOONE_G, SPECIES_BAGON, SPECIES_STUFFUL},
        [6] = {SPECIES_PORYGON2, SPECIES_APPLETUN_GIGA, SPECIES_KINGDRA, SPECIES_FLYGON, SPECIES_SLIGGOO, SPECIES_BRAVIARY, SPECIES_GOODRA_H, SPECIES_PALKIA_ORIGIN, SPECIES_KYUREM_WHITE, SPECIES_PYROAR_FEMALE},
        [7] = {SPECIES_GIRATINA_ORIGIN, SPECIES_DIALGA, SPECIES_LICKILICKY, SPECIES_ZOROARK_H, SPECIES_TURTONATOR, SPECIES_CHANSEY, SPECIES_BOUFFALANT, SPECIES_SILVALLY, SPECIES_ARCEUS_DRAGON, SPECIES_REGIDRAGO},
        [8] = {SPECIES_SALAMENCE_MEGA, SPECIES_SWELLOW, SPECIES_DIALGA, SPECIES_FLYGON, SPECIES_EXEGGUTOR_A, SPECIES_AMPHAROS_MEGA, SPECIES_SNORLAX, SPECIES_NOIVERN, SPECIES_DRAGAPULT, SPECIES_HAPPINY},
        [9] = {SPECIES_AUDINO, SPECIES_DURALUDON, SPECIES_TOUCANNON, SPECIES_FURFROU_LA_REINE, SPECIES_ZYGARDE_COMPLETE, SPECIES_OBSTAGOON, SPECIES_FLYGON, SPECIES_LATIOS_MEGA, SPECIES_SLIGGOO, SPECIES_BLISSEY},
    },
    [CLASS_PKMN_TRAINER_3] = {
        [0] = {SPECIES_ZIGZAGOON, SPECIES_ZORUA_H, SPECIES_SQUAWKABILLY, SPECIES_DEERLING_AUTUMN, SPECIES_CYCLIZAR, SPECIES_DUDUNSPARCE_THREE, SPECIES_HOOTHOOT, SPECIES_MEOWTH, SPECIES_DOLLIV, SPECIES_TAILLOW},
        [1] = {SPECIES_OINKOLOGNE_FEMALE, SPECIES_SPEAROW, SPECIES_TANDEMAUS, SPECIES_MAUSHOLD_FOUR, SPECIES_ZIGZAGOON, SPECIES_WHISMUR, SPECIES_SQUAWKABILLY_WHITE, SPECIES_RATTATA_A, SPECIES_SHROODLE, SPECIES_TERAPAGOS_STELLAR},
        [2] = {SPECIES_IGGLYBUFF, SPECIES_RATTATA_A, SPECIES_SKITTY, SPECIES_BIDOOF, SPECIES_MAUSHOLD, SPECIES_DUDUNSPARCE, SPECIES_SQUAWKABILLY_YELLOW, SPECIES_ZIGZAGOON, SPECIES_TERAPAGOS, SPECIES_MINCCINO},
        [3] = {SPECIES_SWABLU, SPECIES_TEDDIURSA, SPECIES_LITLEO, SPECIES_HERDIER, SPECIES_GLAMEOW, SPECIES_SMEARGLE, SPECIES_LICKITUNG, SPECIES_VIGOROTH, SPECIES_PORYGON, SPECIES_WIGGLYTUFF},
        [4] = {SPECIES_SWABLU, SPECIES_SPINDA, SPECIES_WIGGLYTUFF, SPECIES_SLAKOTH, SPECIES_VIGOROTH, SPECIES_CHATOT, SPECIES_RUFFLET, SPECIES_RATICATE, SPECIES_CASTFORM, SPECIES_STARAVIA},
        [5] = {SPECIES_BUNEARY, SPECIES_CHATOT, SPECIES_RATICATE, SPECIES_DUNSPARCE, SPECIES_RUFFLET, SPECIES_FARFETCHD, SPECIES_LITLEO, SPECIES_EEVEE_GIGA, SPECIES_RATICATE_A, SPECIES_SMEARGLE},
        [6] = {SPECIES_FURFROU, SPECIES_INDEEDEE, SPECIES_SAWSBUCK_SUMMER, SPECIES_KANGASKHAN_MEGA, SPECIES_SILVALLY, SPECIES_SNORLAX_GIGA, SPECIES_LICKILICKY, SPECIES_KANGASKHAN, SPECIES_KOMALA, SPECIES_MELOETTA},
        [7] = {SPECIES_SWELLOW, SPECIES_DODRIO, SPECIES_HELIOLISK, SPECIES_PIDGEOT, SPECIES_FURFROU_KABUKI, SPECIES_LICKILICKY, SPECIES_URSALUNA, SPECIES_UNFEZANT, SPECIES_STOUTLAND, SPECIES_STARAPTOR},
        [8] = {SPECIES_MILTANK, SPECIES_URSALUNA, SPECIES_FURFROU_STAR, SPECIES_FEAROW, SPECIES_PYROAR, SPECIES_SAWSBUCK_WINTER, SPECIES_BEWEAR, SPECIES_URSARING, SPECIES_TOUCANNON, SPECIES_SAWSBUCK_SUMMER},
        [9] = {SPECIES_INDEEDEE_FEMALE, SPECIES_CHANSEY, SPECIES_STOUTLAND, SPECIES_MELOETTA, SPECIES_FURFROU, SPECIES_REGIGIGAS, SPECIES_KANGASKHAN_MEGA, SPECIES_URSARING, SPECIES_HELIOLISK, SPECIES_OBSTAGOON},
    },
    [CLASS_PKMN_BREEDER_RS] = {
        [0] = {SPECIES_LILLIPUP, SPECIES_TERAPAGOS, SPECIES_ZIGZAGOON_G, SPECIES_ARBOLIVA, SPECIES_IGGLYBUFF, SPECIES_DEERLING_AUTUMN, SPECIES_PIDGEY, SPECIES_SKITTY, SPECIES_FLETCHLING, SPECIES_SQUAWKABILLY_YELLOW},
        [1] = {SPECIES_FARIGIRAF, SPECIES_DITTO, SPECIES_PATRAT, SPECIES_MEOWTH, SPECIES_MAUSHOLD_FOUR, SPECIES_BIDOOF, SPECIES_PIDGEY, SPECIES_SQUAWKABILLY_YELLOW, SPECIES_SMOLIV, SPECIES_DUDUNSPARCE},
        [2] = {SPECIES_TERAPAGOS_STELLAR, SPECIES_BUNNELBY, SPECIES_PIKIPEK, SPECIES_LILLIPUP, SPECIES_DEERLING_WINTER, SPECIES_TERAPAGOS, SPECIES_SQUAWKABILLY, SPECIES_TERAPAGOS_TERASTAL, SPECIES_ZORUA_H, SPECIES_EGG},
        [3] = {SPECIES_DODUO, SPECIES_EEVEE_GIGA, SPECIES_LOUDRED, SPECIES_RUFFLET, SPECIES_RATICATE, SPECIES_SWABLU, SPECIES_RATICATE_A, SPECIES_BIBAREL, SPECIES_TRUMBEAK, SPECIES_PORYGON},
        [4] = {SPECIES_SWABLU, SPECIES_SPINDA, SPECIES_STUFFUL, SPECIES_LICKITUNG, SPECIES_JIGGLYPUFF, SPECIES_BIBAREL, SPECIES_PORYGON, SPECIES_HERDIER, SPECIES_RATICATE, SPECIES_EEVEE_GIGA},
        [5] = {SPECIES_RATICATE_A, SPECIES_BUNEARY, SPECIES_GLAMEOW, SPECIES_FARFETCHD, SPECIES_LINOONE_G, SPECIES_PERSIAN, SPECIES_STUFFUL, SPECIES_PORYGON, SPECIES_DUNSPARCE, SPECIES_SLAKOTH},
        [6] = {SPECIES_TOUCANNON, SPECIES_FURFROU, SPECIES_MELOETTA, SPECIES_AMBIPOM, SPECIES_FEAROW, SPECIES_PIDGEOT_MEGA, SPECIES_FURFROU_LA_REINE, SPECIES_BRAVIARY, SPECIES_ZANGOOSE, SPECIES_SAWSBUCK_SUMMER},
        [7] = {SPECIES_URSARING, SPECIES_KOMALA, SPECIES_STANTLER, SPECIES_HAPPINY, SPECIES_SAWSBUCK_WINTER, SPECIES_ARCEUS, SPECIES_FURFROU_DIAMOND, SPECIES_BLISSEY, SPECIES_AUDINO_MEGA, SPECIES_SLAKING},
        [8] = {SPECIES_FURFROU_PHAROAH, SPECIES_MILTANK, SPECIES_AUDINO, SPECIES_FURFROU_LA_REINE, SPECIES_INDEEDEE, SPECIES_KANGASKHAN_MEGA, SPECIES_TOUCANNON, SPECIES_TAUROS, SPECIES_WYRDEER, SPECIES_BLISSEY},
        [9] = {SPECIES_CINCCINO, SPECIES_MILTANK, SPECIES_TAUROS, SPECIES_FURFROU_KABUKI, SPECIES_HELIOLISK, SPECIES_PORYGON2, SPECIES_INDEEDEE_FEMALE, SPECIES_FEAROW, SPECIES_ARCEUS, SPECIES_PYROAR},
    },
    [CLASS_PKMN_RANGER_RS] = {
        [0] = {SPECIES_WHISMUR, SPECIES_MINCCINO, SPECIES_BUNNELBY, SPECIES_SENTRET, SPECIES_DOLLIV, SPECIES_STARLY, SPECIES_LECHONK, SPECIES_MEOWTH_GIGA, SPECIES_PATRAT, SPECIES_EGG},
        [1] = {SPECIES_AZURILL, SPECIES_DUDUNSPARCE, SPECIES_DEERLING_WINTER, SPECIES_PIKIPEK, SPECIES_DEERLING, SPECIES_SQUAWKABILLY_BLUE, SPECIES_SKWOVET, SPECIES_FLETCHLING, SPECIES_TERAPAGOS_TERASTAL, SPECIES_MEOWTH},
        [2] = {SPECIES_HELIOPTILE, SPECIES_DEERLING_WINTER, SPECIES_MAUSHOLD_FOUR, SPECIES_YUNGOOS, SPECIES_DITTO, SPECIES_FARIGIRAF, SPECIES_STARLY, SPECIES_SPEAROW, SPECIES_TERAPAGOS_TERASTAL, SPECIES_SENTRET},
        [3] = {SPECIES_EEVEE, SPECIES_LOUDRED, SPECIES_SPINDA, SPECIES_TYPE_NULL, SPECIES_BUNEARY, SPECIES_FARFETCHD, SPECIES_DIGGERSBY, SPECIES_DUNSPARCE, SPECIES_GIRAFARIG, SPECIES_PERSIAN},
        [4] = {SPECIES_RATICATE, SPECIES_MUNCHLAX, SPECIES_LITLEO, SPECIES_WOOLOO, SPECIES_BIBAREL, SPECIES_LOUDRED, SPECIES_TRANQUILL, SPECIES_RATICATE_A, SPECIES_STARAVIA, SPECIES_WATCHOG},
        [5] = {SPECIES_DIGGERSBY, SPECIES_LINOONE_G, SPECIES_RATICATE, SPECIES_STUFFUL, SPECIES_GLAMEOW, SPECIES_RATICATE_A, SPECIES_DODUO, SPECIES_LINOONE, SPECIES_JIGGLYPUFF, SPECIES_WOOLOO},
        [6] = {SPECIES_WYRDEER, SPECIES_OBSTAGOON, SPECIES_LICKILICKY, SPECIES_KANGASKHAN, SPECIES_LOPUNNY_MEGA, SPECIES_FURFROU_DEBUTANTE, SPECIES_FURFROU_HEART, SPECIES_GREEDENT, SPECIES_INDEEDEE, SPECIES_UNFEZANT_F},
        [7] = {SPECIES_FURFROU_DANDY, SPECIES_AUDINO_MEGA, SPECIES_MELOETTA_PIROUETTE, SPECIES_SAWSBUCK, SPECIES_PIDGEOT, SPECIES_PYROAR_FEMALE, SPECIES_HAPPINY, SPECIES_TAUROS, SPECIES_CINCCINO, SPECIES_HELIOLISK},
        [8] = {SPECIES_EXPLOUD, SPECIES_ZOROARK_H, SPECIES_TAUROS, SPECIES_INDEEDEE_FEMALE, SPECIES_BEWEAR, SPECIES_DUBWOOL, SPECIES_SNORLAX_GIGA, SPECIES_FURFROU_HEART, SPECIES_FURFROU_KABUKI, SPECIES_GREEDENT},
        [9] = {SPECIES_MELOETTA, SPECIES_DUBWOOL, SPECIES_ARCEUS, SPECIES_STANTLER, SPECIES_FURFROU_LA_REINE, SPECIES_BLISSEY, SPECIES_LOPUNNY, SPECIES_PORYGON2, SPECIES_SNORLAX_GIGA, SPECIES_SWELLOW},
    },
    [CLASS_MAGMA_LEADER] = {
        [0] = {SPECIES_SKWOVET, SPECIES_SQUAWKABILLY, SPECIES_DEERLING, SPECIES_TANDEMAUS, SPECIES_SMOLIV, SPECIES_MEOWTH_GIGA, SPECIES_SPEAROW, SPECIES_MAUSHOLD_FOUR, SPECIES_SENTRET, SPECIES_MEOWTH},
        [1] = {SPECIES_DEERLING, SPECIES_URSALUNA_BLOODMOON, SPECIES_IGGLYBUFF, SPECIES_DEERLING_AUTUMN, SPECIES_BUNNELBY, SPECIES_SMOLIV, SPECIES_HELIOPTILE, SPECIES_AZURILL, SPECIES_PIKIPEK, SPECIES_LECHONK},
        [2] = {SPECIES_WHISMUR, SPECIES_MEOWTH, SPECIES_TERAPAGOS, SPECIES_IGGLYBUFF, SPECIES_MEOWTH_GIGA, SPECIES_EGG, SPECIES_ARBOLIVA, SPECIES_MAUSHOLD_FOUR, SPECIES_ZIGZAGOON, SPECIES_ZIGZAGOON_G},
        [3] = {SPECIES_SMEARGLE, SPECIES_CHATOT, SPECIES_PERSIAN, SPECIES_PORYGON, SPECIES_TRUMBEAK, SPECIES_RATICATE, SPECIES_FURRET, SPECIES_DELCATTY, SPECIES_LICKITUNG, SPECIES_SPINDA},
        [4] = {SPECIES_EEVEE, SPECIES_BIBAREL, SPECIES_SWABLU, SPECIES_TYPE_NULL, SPECIES_WOOLOO, SPECIES_PERSIAN, SPECIES_LICKITUNG, SPECIES_VIGOROTH, SPECIES_SPINDA, SPECIES_DUNSPARCE},
        [5] = {SPECIES_HERDIER, SPECIES_BIBAREL, SPECIES_LITLEO, SPECIES_PIDGEOTTO, SPECIES_KECLEON, SPECIES_STUFFUL, SPECIES_MUNCHLAX, SPECIES_TEDDIURSA, SPECIES_STARAVIA, SPECIES_FURRET},
        [6] = {SPECIES_DUBWOOL, SPECIES_REGIGIGAS, SPECIES_FURFROU_KABUKI, SPECIES_PIDGEOT, SPECIES_WYRDEER, SPECIES_PORYGON2, SPECIES_TAUROS, SPECIES_STOUTLAND, SPECIES_STANTLER, SPECIES_SAWSBUCK_WINTER},
        [7] = {SPECIES_PORYGON_Z, SPECIES_DUBWOOL, SPECIES_KANGASKHAN, SPECIES_FURFROU_PHAROAH, SPECIES_MELOETTA, SPECIES_WYRDEER, SPECIES_OBSTAGOON, SPECIES_PIDGEOT_MEGA, SPECIES_STANTLER, SPECIES_DRAMPA},
        [8] = {SPECIES_PORYGON_Z, SPECIES_SILVALLY, SPECIES_HELIOLISK, SPECIES_URSALUNA, SPECIES_FURFROU_LA_REINE, SPECIES_EXPLOUD, SPECIES_BOUFFALANT, SPECIES_SAWSBUCK, SPECIES_AUDINO, SPECIES_SNORLAX},
        [9] = {SPECIES_LOPUNNY_MEGA, SPECIES_FURFROU_LA_REINE, SPECIES_MELOETTA, SPECIES_MELOETTA_PIROUETTE, SPECIES_ORANGURU, SPECIES_BLISSEY, SPECIES_BEWEAR, SPECIES_AUDINO, SPECIES_PYROAR_FEMALE, SPECIES_UNFEZANT},
    },
    [CLASS_TEAM_MAGMA] = {
        [0] = {SPECIES_PIKIPEK, SPECIES_OINKOLOGNE, SPECIES_ZIGZAGOON_G, SPECIES_MAUSHOLD, SPECIES_SQUAWKABILLY, SPECIES_WHISMUR, SPECIES_DEERLING_AUTUMN, SPECIES_PATRAT, SPECIES_CYCLIZAR, SPECIES_SKWOVET},
        [1] = {SPECIES_SPEAROW, SPECIES_MAUSHOLD_FOUR, SPECIES_SQUAWKABILLY_WHITE, SPECIES_TERAPAGOS_STELLAR, SPECIES_DEERLING_WINTER, SPECIES_DEERLING_AUTUMN, SPECIES_DUDUNSPARCE, SPECIES_BIDOOF, SPECIES_BUNNELBY, SPECIES_STARLY},
        [2] = {SPECIES_STARLY, SPECIES_DITTO, SPECIES_AZURILL, SPECIES_BIDOOF, SPECIES_OINKOLOGNE_FEMALE, SPECIES_TERAPAGOS, SPECIES_TERAPAGOS_STELLAR, SPECIES_EGG, SPECIES_SENTRET, SPECIES_PIDGEY},
        [3] = {SPECIES_VIGOROTH, SPECIES_RATICATE, SPECIES_SMEARGLE, SPECIES_GUMSHOOS, SPECIES_HERDIER, SPECIES_PIDGEOTTO, SPECIES_DUNSPARCE, SPECIES_SWABLU, SPECIES_CASTFORM, SPECIES_LICKITUNG},
        [4] = {SPECIES_SLAKOTH, SPECIES_FURRET, SPECIES_CHATOT, SPECIES_CASTFORM, SPECIES_GIRAFARIG, SPECIES_DIGGERSBY, SPECIES_STUFFUL, SPECIES_RATICATE_A, SPECIES_DODUO, SPECIES_LOUDRED},
        [5] = {SPECIES_LITLEO, SPECIES_EEVEE, SPECIES_LICKITUNG, SPECIES_JIGGLYPUFF, SPECIES_GLAMEOW, SPECIES_WATCHOG, SPECIES_KECLEON, SPECIES_SLAKOTH, SPECIES_RUFFLET, SPECIES_GIRAFARIG},
        [6] = {SPECIES_LOPUNNY, SPECIES_SLAKING, SPECIES_FURFROU_LA_REINE, SPECIES_TAUROS, SPECIES_FURFROU_HEART, SPECIES_EXPLOUD, SPECIES_KANGASKHAN_MEGA, SPECIES_SWELLOW, SPECIES_DODRIO, SPECIES_HAPPINY},
        [7] = {SPECIES_INDEEDEE, SPECIES_PORYGON2, SPECIES_DUBWOOL, SPECIES_KANGASKHAN_MEGA, SPECIES_PYROAR_FEMALE, SPECIES_SAWSBUCK_SUMMER, SPECIES_ZOROARK_H, SPECIES_AMBIPOM, SPECIES_TAUROS, SPECIES_FURFROU_STAR},
        [8] = {SPECIES_DRAMPA, SPECIES_SNORLAX, SPECIES_HAPPINY, SPECIES_FURFROU_DIAMOND, SPECIES_STANTLER, SPECIES_UNFEZANT, SPECIES_AMBIPOM, SPECIES_PIDGEOT_MEGA, SPECIES_MILTANK, SPECIES_BRAVIARY},
        [9] = {SPECIES_SAWSBUCK_AUTUMN, SPECIES_AUDINO_MEGA, SPECIES_INDEEDEE, SPECIES_FURFROU_STAR, SPECIES_EXPLOUD, SPECIES_FURFROU_LA_REINE, SPECIES_PIDGEOT_MEGA, SPECIES_DODRIO, SPECIES_REGIGIGAS, SPECIES_KOMALA},
    },
    [CLASS_LASS_RS] = {
        [0] = {SPECIES_PATRAT, SPECIES_SPEAROW, SPECIES_SQUAWKABILLY_YELLOW, SPECIES_ZIGZAGOON, SPECIES_DEERLING_WINTER, SPECIES_SHROODLE, SPECIES_WHISMUR, SPECIES_FARIGIRAF, SPECIES_GRAFAIAI, SPECIES_BIDOOF},
        [1] = {SPECIES_OINKOLOGNE_FEMALE, SPECIES_DEERLING_WINTER, SPECIES_MAUSHOLD, SPECIES_MEOWTH_GIGA, SPECIES_CYCLIZAR, SPECIES_SQUAWKABILLY_BLUE, SPECIES_PIDOVE, SPECIES_OINKOLOGNE, SPECIES_DUDUNSPARCE, SPECIES_SQUAWKABILLY},
        [2] = {SPECIES_TERAPAGOS_TERASTAL, SPECIES_EGG, SPECIES_SENTRET, SPECIES_DOLLIV, SPECIES_OINKOLOGNE_FEMALE, SPECIES_AZURILL, SPECIES_LECHONK, SPECIES_RATTATA_A, SPECIES_SKWOVET, SPECIES_LILLIPUP},
        [3] = {SPECIES_EEVEE, SPECIES_DODUO, SPECIES_HERDIER, SPECIES_TYPE_NULL, SPECIES_EEVEE_GIGA, SPECIES_LITLEO, SPECIES_AIPOM, SPECIES_CASTFORM, SPECIES_TEDDIURSA, SPECIES_PERSIAN},
        [4] = {SPECIES_LINOONE_G, SPECIES_LINOONE, SPECIES_SLAKOTH, SPECIES_SMEARGLE, SPECIES_DELCATTY, SPECIES_JIGGLYPUFF, SPECIES_LITLEO, SPECIES_DUNSPARCE, SPECIES_PIDGEOTTO, SPECIES_DODUO},
        [5] = {SPECIES_DIGGERSBY, SPECIES_PORYGON, SPECIES_PIDGEOTTO, SPECIES_DODUO, SPECIES_HERDIER, SPECIES_JIGGLYPUFF, SPECIES_DELCATTY, SPECIES_GIRAFARIG, SPECIES_TRANQUILL, SPECIES_EEVEE},
        [6] = {SPECIES_FEAROW, SPECIES_ARCEUS, SPECIES_EXPLOUD, SPECIES_PIDGEOT, SPECIES_SAWSBUCK_SUMMER, SPECIES_AUDINO_MEGA, SPECIES_FURFROU_PHAROAH, SPECIES_SWELLOW, SPECIES_FURFROU_KABUKI, SPECIES_BRAVIARY},
        [7] = {SPECIES_FURFROU_MATRON, SPECIES_AMBIPOM, SPECIES_FEAROW, SPECIES_SAWSBUCK_AUTUMN, SPECIES_FURFROU_DANDY, SPECIES_CINCCINO, SPECIES_DODRIO, SPECIES_LICKILICKY, SPECIES_LOPUNNY, SPECIES_FURFROU_LA_REINE},
        [8] = {SPECIES_SAWSBUCK_SUMMER, SPECIES_UNFEZANT, SPECIES_LOPUNNY, SPECIES_MELOETTA, SPECIES_LICKILICKY, SPECIES_ARCEUS, SPECIES_REGIGIGAS, SPECIES_SLAKING, SPECIES_STOUTLAND, SPECIES_HAPPINY},
        [9] = {SPECIES_TOUCANNON, SPECIES_LOPUNNY, SPECIES_REGIGIGAS, SPECIES_PORYGON_Z, SPECIES_PYROAR_FEMALE, SPECIES_PORYGON2, SPECIES_DUBWOOL, SPECIES_INDEEDEE, SPECIES_MELOETTA, SPECIES_OBSTAGOON},
    },
    [CLASS_BUG_CATCHER_RS] = {
        [0] = {SPECIES_URSALUNA_BLOODMOON, SPECIES_SQUAWKABILLY_WHITE, SPECIES_DEERLING_WINTER, SPECIES_DUDUNSPARCE, SPECIES_SQUAWKABILLY_YELLOW, SPECIES_FARIGIRAF, SPECIES_EGG, SPECIES_RATTATA_A, SPECIES_ZIGZAGOON, SPECIES_YUNGOOS},
        [1] = {SPECIES_HELIOPTILE, SPECIES_EGG, SPECIES_URSALUNA_BLOODMOON, SPECIES_SHROODLE, SPECIES_DEERLING_AUTUMN, SPECIES_MAUSHOLD, SPECIES_DITTO, SPECIES_DEERLING, SPECIES_MINCCINO, SPECIES_STARLY},
        [2] = {SPECIES_WHISMUR, SPECIES_GRAFAIAI, SPECIES_ZORUA_H, SPECIES_STARLY, SPECIES_ARBOLIVA, SPECIES_LILLIPUP, SPECIES_DEERLING_SUMMER, SPECIES_LECHONK, SPECIES_BIDOOF, SPECIES_EGG},
        [3] = {SPECIES_SMEARGLE, SPECIES_TRANQUILL, SPECIES_EEVEE_GIGA, SPECIES_PORYGON, SPECIES_BUNEARY, SPECIES_TYPE_NULL, SPECIES_STUFFUL, SPECIES_PERSIAN, SPECIES_SLAKOTH, SPECIES_LINOONE_G},
        [4] = {SPECIES_VIGOROTH, SPECIES_LINOONE, SPECIES_SMEARGLE, SPECIES_LINOONE_G, SPECIES_SPINDA, SPECIES_LICKITUNG, SPECIES_TEDDIURSA, SPECIES_GUMSHOOS, SPECIES_GIRAFARIG, SPECIES_RATICATE},
        [5] = {SPECIES_RATICATE_A, SPECIES_TYPE_NULL, SPECIES_EEVEE_GIGA, SPECIES_WATCHOG, SPECIES_SMEARGLE, SPECIES_WOOLOO, SPECIES_VIGOROTH, SPECIES_DELCATTY, SPECIES_TRANQUILL, SPECIES_LITLEO},
        [6] = {SPECIES_FURFROU_MATRON, SPECIES_SWELLOW, SPECIES_FURFROU_HEART, SPECIES_FURFROU_STAR, SPECIES_ARCEUS, SPECIES_PIDGEOT_MEGA, SPECIES_HELIOLISK, SPECIES_DRAMPA, SPECIES_SAWSBUCK, SPECIES_UNFEZANT_F},
        [7] = {SPECIES_FURFROU_MATRON, SPECIES_SNORLAX, SPECIES_BLISSEY, SPECIES_REGIGIGAS, SPECIES_PIDGEOT, SPECIES_SAWSBUCK, SPECIES_WYRDEER, SPECIES_URSARING, SPECIES_STOUTLAND, SPECIES_INDEEDEE_FEMALE},
        [8] = {SPECIES_ARCEUS, SPECIES_LOPUNNY_MEGA, SPECIES_FURFROU_HEART, SPECIES_BOUFFALANT, SPECIES_GREEDENT, SPECIES_UNFEZANT, SPECIES_SILVALLY, SPECIES_STANTLER, SPECIES_FURFROU_KABUKI, SPECIES_PIDGEOT},
        [9] = {SPECIES_CHANSEY, SPECIES_OBSTAGOON, SPECIES_CINCCINO, SPECIES_KOMALA, SPECIES_SNORLAX, SPECIES_SILVALLY, SPECIES_SAWSBUCK_AUTUMN, SPECIES_LICKILICKY, SPECIES_INDEEDEE, SPECIES_UNFEZANT},
    },
    [CLASS_HIKER_RS] = {
        [0] = {SPECIES_SQUAWKABILLY, SPECIES_SQUAWKABILLY_YELLOW, SPECIES_TERAPAGOS, SPECIES_FARIGIRAF, SPECIES_WHISMUR, SPECIES_PIDGEY, SPECIES_RATTATA, SPECIES_OINKOLOGNE_FEMALE, SPECIES_DOLLIV, SPECIES_TERAPAGOS_TERASTAL},
        [1] = {SPECIES_TAILLOW, SPECIES_ZIGZAGOON_G, SPECIES_FARIGIRAF, SPECIES_OINKOLOGNE, SPECIES_ZIGZAGOON, SPECIES_FLETCHLING, SPECIES_OINKOLOGNE_FEMALE, SPECIES_DEERLING_WINTER, SPECIES_DEERLING, SPECIES_AZURILL},
        [2] = {SPECIES_HOOTHOOT, SPECIES_PIDGEY, SPECIES_ARBOLIVA, SPECIES_MAUSHOLD_FOUR, SPECIES_SQUAWKABILLY_YELLOW, SPECIES_GRAFAIAI, SPECIES_SENTRET, SPECIES_PIDOVE, SPECIES_ZIGZAGOON, SPECIES_EGG},
        [3] = {SPECIES_SLAKOTH, SPECIES_LINOONE_G, SPECIES_PERSIAN, SPECIES_WIGGLYTUFF, SPECIES_EEVEE, SPECIES_BUNEARY, SPECIES_SPINDA, SPECIES_EEVEE_GIGA, SPECIES_LINOONE, SPECIES_HERDIER},
        [4] = {SPECIES_BUNEARY, SPECIES_SWABLU, SPECIES_RUFFLET, SPECIES_VIGOROTH, SPECIES_BIBAREL, SPECIES_LINOONE, SPECIES_GIRAFARIG, SPECIES_GUMSHOOS, SPECIES_PIDGEOTTO, SPECIES_WIGGLYTUFF},
        [5] = {SPECIES_BUNEARY, SPECIES_RATICATE, SPECIES_LITLEO, SPECIES_EEVEE_GIGA, SPECIES_LOUDRED, SPECIES_EEVEE, SPECIES_SMEARGLE, SPECIES_CASTFORM, SPECIES_GIRAFARIG, SPECIES_WOOLOO},
        [6] = {SPECIES_DODRIO, SPECIES_UNFEZANT, SPECIES_REGIGIGAS, SPECIES_FURFROU, SPECIES_PORYGON2, SPECIES_PIDGEOT, SPECIES_KANGASKHAN, SPECIES_ARCEUS, SPECIES_TOUCANNON, SPECIES_DUBWOOL},
        [7] = {SPECIES_SILVALLY, SPECIES_FURFROU_KABUKI, SPECIES_EXPLOUD, SPECIES_PIDGEOT, SPECIES_DUBWOOL, SPECIES_FURFROU_DIAMOND, SPECIES_SNORLAX, SPECIES_SAWSBUCK_AUTUMN, SPECIES_STARAPTOR, SPECIES_PORYGON_Z},
        [8] = {SPECIES_MELOETTA_PIROUETTE, SPECIES_INDEEDEE, SPECIES_FURFROU_DIAMOND, SPECIES_FURFROU_DANDY, SPECIES_STANTLER, SPECIES_UNFEZANT_F, SPECIES_FURFROU_PHAROAH, SPECIES_CHANSEY, SPECIES_REGIGIGAS, SPECIES_CINCCINO},
        [9] = {SPECIES_STOUTLAND, SPECIES_URSALUNA, SPECIES_SNORLAX_GIGA, SPECIES_ORANGURU, SPECIES_STARAPTOR, SPECIES_OBSTAGOON, SPECIES_FURFROU_HEART, SPECIES_SAWSBUCK, SPECIES_BEWEAR, SPECIES_EXPLOUD},
    },
    [CLASS_YOUNG_COUPLE_RS] = {
        [0] = {SPECIES_IGGLYBUFF, SPECIES_BUNNELBY, SPECIES_DEERLING_AUTUMN, SPECIES_TANDEMAUS, SPECIES_SKITTY, SPECIES_TERAPAGOS, SPECIES_SKWOVET, SPECIES_PIDGEY, SPECIES_STARLY, SPECIES_SQUAWKABILLY_WHITE},
        [1] = {SPECIES_OINKOLOGNE_FEMALE, SPECIES_DEERLING_SUMMER, SPECIES_SKWOVET, SPECIES_HELIOPTILE, SPECIES_RATTATA_A, SPECIES_CYCLIZAR, SPECIES_SQUAWKABILLY, SPECIES_ZIGZAGOON, SPECIES_DEERLING_WINTER, SPECIES_MEOWTH_GIGA},
        [2] = {SPECIES_LECHONK, SPECIES_IGGLYBUFF, SPECIES_ZORUA_H, SPECIES_OINKOLOGNE, SPECIES_FLETCHLING, SPECIES_ARBOLIVA, SPECIES_ZIGZAGOON, SPECIES_RATTATA_A, SPECIES_GRAFAIAI, SPECIES_PIKIPEK},
        [3] = {SPECIES_KECLEON, SPECIES_GLAMEOW, SPECIES_WOOLOO, SPECIES_LOUDRED, SPECIES_SMEARGLE, SPECIES_GIRAFARIG, SPECIES_FARFETCHD, SPECIES_MUNCHLAX, SPECIES_BUNEARY, SPECIES_PERSIAN},
        [4] = {SPECIES_BUNEARY, SPECIES_LICKITUNG, SPECIES_GLAMEOW, SPECIES_RUFFLET, SPECIES_JIGGLYPUFF, SPECIES_RATICATE_A, SPECIES_LINOONE, SPECIES_LINOONE_G, SPECIES_STARAVIA, SPECIES_SWABLU},
        [5] = {SPECIES_WIGGLYTUFF, SPECIES_HERDIER, SPECIES_CHATOT, SPECIES_PIDGEOTTO, SPECIES_RATICATE_A, SPECIES_KECLEON, SPECIES_JIGGLYPUFF, SPECIES_GUMSHOOS, SPECIES_VIGOROTH, SPECIES_BIBAREL},
        [6] = {SPECIES_SILVALLY, SPECIES_LOPUNNY, SPECIES_SAWSBUCK_SUMMER, SPECIES_GREEDENT, SPECIES_FEAROW, SPECIES_CHANSEY, SPECIES_PURUGLY, SPECIES_SLAKING, SPECIES_CINCCINO, SPECIES_REGIGIGAS},
        [7] = {SPECIES_FURFROU_DANDY, SPECIES_SAWSBUCK, SPECIES_URSARING, SPECIES_LOPUNNY_MEGA, SPECIES_KANGASKHAN_MEGA, SPECIES_PIDGEOT, SPECIES_SNORLAX_GIGA, SPECIES_MELOETTA, SPECIES_FURFROU, SPECIES_LICKILICKY},
        [8] = {SPECIES_FEAROW, SPECIES_STARAPTOR, SPECIES_REGIGIGAS, SPECIES_BLISSEY, SPECIES_KANGASKHAN_MEGA, SPECIES_SILVALLY, SPECIES_MELOETTA_PIROUETTE, SPECIES_FURFROU, SPECIES_URSALUNA, SPECIES_TAUROS},
        [9] = {SPECIES_AUDINO_MEGA, SPECIES_FURFROU_DANDY, SPECIES_PORYGON_Z, SPECIES_BLISSEY, SPECIES_CHANSEY, SPECIES_ZANGOOSE, SPECIES_KOMALA, SPECIES_DODRIO, SPECIES_FURFROU_STAR, SPECIES_KANGASKHAN_MEGA},
    },
    [CLASS_OLD_COUPLE] = {
        [0] = {SPECIES_TERAPAGOS_STELLAR, SPECIES_DEERLING_SUMMER, SPECIES_SMOLIV, SPECIES_PIKIPEK, SPECIES_DUDUNSPARCE, SPECIES_PIDOVE, SPECIES_EGG, SPECIES_RATTATA, SPECIES_RATTATA_A, SPECIES_TERAPAGOS},
        [1] = {SPECIES_MEOWTH, SPECIES_LILLIPUP, SPECIES_URSALUNA_BLOODMOON, SPECIES_WHISMUR, SPECIES_PIDOVE, SPECIES_SPEAROW, SPECIES_TAILLOW, SPECIES_BIDOOF, SPECIES_MEOWTH_GIGA, SPECIES_SQUAWKABILLY_BLUE},
        [2] = {SPECIES_MEOWTH, SPECIES_MEOWTH_GIGA, SPECIES_DEERLING, SPECIES_DITTO, SPECIES_MINCCINO, SPECIES_HOOTHOOT, SPECIES_HELIOPTILE, SPECIES_ZIGZAGOON, SPECIES_BIDOOF, SPECIES_MAUSHOLD},
        [3] = {SPECIES_CASTFORM, SPECIES_WOOLOO, SPECIES_KECLEON, SPECIES_LOUDRED, SPECIES_AIPOM, SPECIES_PERSIAN, SPECIES_DODUO, SPECIES_SPINDA, SPECIES_VIGOROTH, SPECIES_PORYGON},
        [4] = {SPECIES_EEVEE, SPECIES_STARAVIA, SPECIES_LITLEO, SPECIES_LINOONE, SPECIES_BIBAREL, SPECIES_DODUO, SPECIES_FARFETCHD, SPECIES_LOUDRED, SPECIES_TRUMBEAK, SPECIES_RATICATE_A},
        [5] = {SPECIES_BIBAREL, SPECIES_TRANQUILL, SPECIES_TYPE_NULL, SPECIES_TEDDIURSA, SPECIES_RATICATE, SPECIES_FARFETCHD, SPECIES_LOUDRED, SPECIES_AIPOM, SPECIES_TRUMBEAK, SPECIES_LINOONE},
        [6] = {SPECIES_FURFROU_MATRON, SPECIES_TAUROS, SPECIES_KANGASKHAN_MEGA, SPECIES_STANTLER, SPECIES_INDEEDEE_FEMALE, SPECIES_SLAKING, SPECIES_UNFEZANT, SPECIES_FURFROU_KABUKI, SPECIES_PORYGON2, SPECIES_PORYGON_Z},
        [7] = {SPECIES_ORANGURU, SPECIES_FURFROU_DANDY, SPECIES_OBSTAGOON, SPECIES_SAWSBUCK, SPECIES_PYROAR, SPECIES_GREEDENT, SPECIES_KOMALA, SPECIES_EXPLOUD, SPECIES_FURFROU_KABUKI, SPECIES_UNFEZANT_F},
        [8] = {SPECIES_FURFROU_DIAMOND, SPECIES_MILTANK, SPECIES_WYRDEER, SPECIES_LICKILICKY, SPECIES_KANGASKHAN, SPECIES_KOMALA, SPECIES_BLISSEY, SPECIES_KANGASKHAN_MEGA, SPECIES_DRAMPA, SPECIES_FURFROU_DANDY},
        [9] = {SPECIES_SWELLOW, SPECIES_SNORLAX_GIGA, SPECIES_WYRDEER, SPECIES_PURUGLY, SPECIES_FURFROU_KABUKI, SPECIES_DRAMPA, SPECIES_REGIGIGAS, SPECIES_HELIOLISK, SPECIES_FEAROW, SPECIES_ORANGURU},
    },
    [CLASS_SIS_AND_BRO_RS] = {
        [0] = {SPECIES_CYCLIZAR, SPECIES_ZIGZAGOON_G, SPECIES_SQUAWKABILLY_YELLOW, SPECIES_ARBOLIVA, SPECIES_WHISMUR, SPECIES_BUNNELBY, SPECIES_DEERLING, SPECIES_OINKOLOGNE, SPECIES_TERAPAGOS_TERASTAL, SPECIES_BIDOOF},
        [1] = {SPECIES_WHISMUR, SPECIES_DUDUNSPARCE, SPECIES_SENTRET, SPECIES_ARBOLIVA, SPECIES_MEOWTH, SPECIES_LILLIPUP, SPECIES_RATTATA_A, SPECIES_PATRAT, SPECIES_FLETCHLING, SPECIES_ZORUA_H},
        [2] = {SPECIES_SPEAROW, SPECIES_SENTRET, SPECIES_SQUAWKABILLY, SPECIES_MEOWTH_GIGA, SPECIES_DEERLING, SPECIES_DOLLIV, SPECIES_MAUSHOLD_FOUR, SPECIES_SQUAWKABILLY_BLUE, SPECIES_TERAPAGOS, SPECIES_BUNNELBY},
        [3] = {SPECIES_DODUO, SPECIES_FURRET, SPECIES_RATICATE_A, SPECIES_LICKITUNG, SPECIES_BIBAREL, SPECIES_GIRAFARIG, SPECIES_LINOONE, SPECIES_TRUMBEAK, SPECIES_JIGGLYPUFF, SPECIES_DELCATTY},
        [4] = {SPECIES_RATICATE, SPECIES_PORYGON, SPECIES_GLAMEOW, SPECIES_GUMSHOOS, SPECIES_PERSIAN, SPECIES_LITLEO, SPECIES_WOOLOO, SPECIES_JIGGLYPUFF, SPECIES_DIGGERSBY, SPECIES_WATCHOG},
        [5] = {SPECIES_DELCATTY, SPECIES_STARAVIA, SPECIES_TRUMBEAK, SPECIES_RATICATE, SPECIES_LINOONE, SPECIES_FURRET, SPECIES_MUNCHLAX, SPECIES_JIGGLYPUFF, SPECIES_WOOLOO, SPECIES_GLAMEOW},
        [6] = {SPECIES_FURFROU_HEART, SPECIES_BRAVIARY, SPECIES_URSALUNA, SPECIES_SLAKING, SPECIES_MILTANK, SPECIES_FURFROU_MATRON, SPECIES_FURFROU_DEBUTANTE, SPECIES_PYROAR, SPECIES_CINCCINO, SPECIES_TOUCANNON},
        [7] = {SPECIES_FURFROU_KABUKI, SPECIES_HELIOLISK, SPECIES_AUDINO, SPECIES_BLISSEY, SPECIES_UNFEZANT, SPECIES_FURFROU_STAR, SPECIES_ZANGOOSE, SPECIES_INDEEDEE_FEMALE, SPECIES_SILVALLY, SPECIES_URSARING},
        [8] = {SPECIES_BLISSEY, SPECIES_DUBWOOL, SPECIES_TAUROS, SPECIES_STOUTLAND, SPECIES_KANGASKHAN, SPECIES_SAWSBUCK_SUMMER, SPECIES_AUDINO_MEGA, SPECIES_URSALUNA, SPECIES_PORYGON2, SPECIES_DODRIO},
        [9] = {SPECIES_PIDGEOT, SPECIES_WYRDEER, SPECIES_LOPUNNY_MEGA, SPECIES_PYROAR, SPECIES_GREEDENT, SPECIES_URSALUNA, SPECIES_INDEEDEE_FEMALE, SPECIES_MELOETTA_PIROUETTE, SPECIES_FURFROU_HEART, SPECIES_BEWEAR},
    },
    [CLASS_AQUA_ADMIN] = {
        [0] = {SPECIES_SQUAWKABILLY_WHITE, SPECIES_DOLLIV, SPECIES_DITTO, SPECIES_SHROODLE, SPECIES_MINCCINO, SPECIES_OINKOLOGNE, SPECIES_ZORUA_H, SPECIES_OINKOLOGNE_FEMALE, SPECIES_AZURILL, SPECIES_LILLIPUP},
        [1] = {SPECIES_ARBOLIVA, SPECIES_PIDOVE, SPECIES_MAUSHOLD_FOUR, SPECIES_STARLY, SPECIES_YUNGOOS, SPECIES_DEERLING, SPECIES_DITTO, SPECIES_DOLLIV, SPECIES_MINCCINO, SPECIES_TANDEMAUS},
        [2] = {SPECIES_OINKOLOGNE, SPECIES_BUNNELBY, SPECIES_DEERLING, SPECIES_SQUAWKABILLY, SPECIES_DEERLING_AUTUMN, SPECIES_YUNGOOS, SPECIES_HOOTHOOT, SPECIES_DITTO, SPECIES_CYCLIZAR, SPECIES_FARIGIRAF},
        [3] = {SPECIES_DIGGERSBY, SPECIES_FURRET, SPECIES_BIBAREL, SPECIES_TYPE_NULL, SPECIES_GIRAFARIG, SPECIES_GLAMEOW, SPECIES_TRUMBEAK, SPECIES_BUNEARY, SPECIES_LICKITUNG, SPECIES_LITLEO},
        [4] = {SPECIES_RATICATE_A, SPECIES_STUFFUL, SPECIES_HERDIER, SPECIES_EEVEE, SPECIES_GIRAFARIG, SPECIES_KECLEON, SPECIES_PERSIAN, SPECIES_RATICATE, SPECIES_DODUO, SPECIES_TRUMBEAK},
        [5] = {SPECIES_KECLEON, SPECIES_EEVEE, SPECIES_JIGGLYPUFF, SPECIES_DUNSPARCE, SPECIES_RATICATE_A, SPECIES_GUMSHOOS, SPECIES_CHATOT, SPECIES_CASTFORM, SPECIES_VIGOROTH, SPECIES_SPINDA},
        [6] = {SPECIES_SLAKING, SPECIES_LICKILICKY, SPECIES_EXPLOUD, SPECIES_FEAROW, SPECIES_BEWEAR, SPECIES_FURFROU_STAR, SPECIES_STARAPTOR, SPECIES_SNORLAX_GIGA, SPECIES_KOMALA, SPECIES_SAWSBUCK_WINTER},
        [7] = {SPECIES_BRAVIARY, SPECIES_MELOETTA_PIROUETTE, SPECIES_OBSTAGOON, SPECIES_BOUFFALANT, SPECIES_FURFROU_LA_REINE, SPECIES_PURUGLY, SPECIES_EXPLOUD, SPECIES_SNORLAX_GIGA, SPECIES_SAWSBUCK_SUMMER, SPECIES_MELOETTA},
        [8] = {SPECIES_KOMALA, SPECIES_DODRIO, SPECIES_FURFROU_PHAROAH, SPECIES_STARAPTOR, SPECIES_MELOETTA, SPECIES_SAWSBUCK_WINTER, SPECIES_FURFROU_LA_REINE, SPECIES_DUBWOOL, SPECIES_CINCCINO, SPECIES_STOUTLAND},
        [9] = {SPECIES_MELOETTA_PIROUETTE, SPECIES_BOUFFALANT, SPECIES_HAPPINY, SPECIES_TAUROS, SPECIES_PYROAR_FEMALE, SPECIES_FURFROU_DIAMOND, SPECIES_FEAROW, SPECIES_PURUGLY, SPECIES_LICKILICKY, SPECIES_AUDINO},
    },
    [CLASS_MAGMA_ADMIN] = {
        [0] = {SPECIES_MEOWTH, SPECIES_SKITTY, SPECIES_AZURILL, SPECIES_CYCLIZAR, SPECIES_SPEAROW, SPECIES_DUDUNSPARCE_THREE, SPECIES_PIDGEY, SPECIES_HOOTHOOT, SPECIES_MAUSHOLD, SPECIES_LILLIPUP},
        [1] = {SPECIES_FARIGIRAF, SPECIES_DEERLING_SUMMER, SPECIES_ZIGZAGOON_G, SPECIES_TERAPAGOS_STELLAR, SPECIES_DEERLING_AUTUMN, SPECIES_BIDOOF, SPECIES_URSALUNA_BLOODMOON, SPECIES_FLETCHLING, SPECIES_OINKOLOGNE_FEMALE, SPECIES_PIDOVE},
        [2] = {SPECIES_DEERLING, SPECIES_OINKOLOGNE, SPECIES_YUNGOOS, SPECIES_IGGLYBUFF, SPECIES_SKWOVET, SPECIES_MEOWTH, SPECIES_TERAPAGOS, SPECIES_SQUAWKABILLY_YELLOW, SPECIES_TAILLOW, SPECIES_PIDGEY},
        [3] = {SPECIES_GIRAFARIG, SPECIES_KECLEON, SPECIES_DIGGERSBY, SPECIES_FARFETCHD, SPECIES_CHATOT, SPECIES_LITLEO, SPECIES_FURRET, SPECIES_LOUDRED, SPECIES_TRANQUILL, SPECIES_CASTFORM},
        [4] = {SPECIES_RUFFLET, SPECIES_SMEARGLE, SPECIES_SPINDA, SPECIES_GLAMEOW, SPECIES_VIGOROTH, SPECIES_JIGGLYPUFF, SPECIES_MUNCHLAX, SPECIES_LITLEO, SPECIES_HERDIER, SPECIES_TYPE_NULL},
        [5] = {SPECIES_LITLEO, SPECIES_DELCATTY, SPECIES_DUNSPARCE, SPECIES_FURRET, SPECIES_SLAKOTH, SPECIES_HERDIER, SPECIES_LICKITUNG, SPECIES_RUFFLET, SPECIES_AIPOM, SPECIES_SMEARGLE},
        [6] = {SPECIES_CINCCINO, SPECIES_DRAMPA, SPECIES_FURFROU_DEBUTANTE, SPECIES_ORANGURU, SPECIES_SAWSBUCK_WINTER, SPECIES_ARCEUS, SPECIES_HAPPINY, SPECIES_AMBIPOM, SPECIES_FEAROW, SPECIES_URSARING},
        [7] = {SPECIES_PURUGLY, SPECIES_STOUTLAND, SPECIES_FURFROU_STAR, SPECIES_FURFROU_MATRON, SPECIES_CINCCINO, SPECIES_DRAMPA, SPECIES_FURFROU, SPECIES_PIDGEOT_MEGA, SPECIES_ZOROARK_H, SPECIES_DODRIO},
        [8] = {SPECIES_SNORLAX, SPECIES_DODRIO, SPECIES_PYROAR_FEMALE, SPECIES_GREEDENT, SPECIES_AMBIPOM, SPECIES_HELIOLISK, SPECIES_PYROAR, SPECIES_CINCCINO, SPECIES_FURFROU_STAR, SPECIES_FURFROU_HEART},
        [9] = {SPECIES_EXPLOUD, SPECIES_CINCCINO, SPECIES_PYROAR, SPECIES_PURUGLY, SPECIES_LOPUNNY_MEGA, SPECIES_REGIGIGAS, SPECIES_SAWSBUCK_SUMMER, SPECIES_SAWSBUCK_WINTER, SPECIES_FURFROU, SPECIES_KOMALA},
    },
    [CLASS_YOUNGSTER] = {
        [0] = {SPECIES_SKITTY, SPECIES_GRUBBIN, SPECIES_MINCCINO, SPECIES_WHISMUR, SPECIES_HOOTHOOT, SPECIES_BURMY, SPECIES_DEERLING_SUMMER, SPECIES_AZURILL, SPECIES_RABSCA, SPECIES_VENIPEDE},
        [1] = {SPECIES_TERAPAGOS_TERASTAL, SPECIES_DUDUNSPARCE_THREE, SPECIES_SPEAROW, SPECIES_RATTATA_A, SPECIES_BURMY, SPECIES_SQUAWKABILLY_BLUE, SPECIES_SENTRET, SPECIES_FLETCHLING, SPECIES_YUNGOOS, SPECIES_SKWOVET},
        [2] = {SPECIES_DEERLING_WINTER, SPECIES_SKWOVET, SPECIES_MEOWTH, SPECIES_SNOM, SPECIES_ZORUA_H, SPECIES_VENIPEDE, SPECIES_JOLTIK, SPECIES_SPEAROW, SPECIES_MAUSHOLD_FOUR, SPECIES_RELLOR},
        [3] = {SPECIES_RATICATE, SPECIES_PARAS, SPECIES_TRAPINCH, SPECIES_STARAVIA, SPECIES_VENOMOTH, SPECIES_PARASECT, SPECIES_WORMADAM_TRASH, SPECIES_ARAQUANID, SPECIES_SHEDINJA, SPECIES_MUNCHLAX},
        [4] = {SPECIES_KAKUNA, SPECIES_WOOLOO, SPECIES_AIPOM, SPECIES_CASCOON, SPECIES_SHUCKLE, SPECIES_SMEARGLE, SPECIES_DODUO, SPECIES_BIBAREL, SPECIES_FORRETRESS, SPECIES_PARAS},
        [5] = {SPECIES_KECLEON, SPECIES_ARIADOS, SPECIES_MUNCHLAX, SPECIES_VENONAT, SPECIES_SILCOON, SPECIES_LOUDRED, SPECIES_SILVALLY_BUG, SPECIES_TEDDIURSA, SPECIES_WOOLOO, SPECIES_SWADLOON},
        [6] = {SPECIES_SAWSBUCK_AUTUMN, SPECIES_VIKAVOLT, SPECIES_DURANT, SPECIES_FURFROU_STAR, SPECIES_FURFROU_HEART, SPECIES_AUDINO_MEGA, SPECIES_FROSMOTH, SPECIES_UNFEZANT, SPECIES_PHEROMOSA, SPECIES_PYROAR},
        [7] = {SPECIES_HERACROSS_MEGA, SPECIES_BOUFFALANT, SPECIES_GENESECT_DOUSE, SPECIES_VIVILLON_JUNGLE, SPECIES_VIVILLON_POKEBALL, SPECIES_DUSTOX, SPECIES_REGIGIGAS, SPECIES_FROSMOTH, SPECIES_FLYGON, SPECIES_UNFEZANT},
        [8] = {SPECIES_NINJASK, SPECIES_INDEEDEE_FEMALE, SPECIES_PYROAR, SPECIES_KRICKETUNE, SPECIES_VIVILLON_ARCHIPELAGO, SPECIES_ORANGURU, SPECIES_TAUROS, SPECIES_DUBWOOL, SPECIES_CENTISKORCH, SPECIES_ACCELGOR},
        [9] = {SPECIES_ARCEUS, SPECIES_FURFROU_KABUKI, SPECIES_TOUCANNON, SPECIES_PURUGLY, SPECIES_DRAMPA, SPECIES_BEAUTIFLY, SPECIES_VIVILLON_CONTINENTAL, SPECIES_BRAVIARY, SPECIES_HERACROSS, SPECIES_BEEDRILL},
    },
    [CLASS_BUG_CATCHER] = {
        [0] = {SPECIES_BURMY, SPECIES_RELLOR, SPECIES_SLITHER_WING, SPECIES_GRUBBIN, SPECIES_WEEDLE, SPECIES_JOLTIK, SPECIES_LOKIX, SPECIES_WIMPOD, SPECIES_VENIPEDE, SPECIES_NYMBLE},
        [1] = {SPECIES_GRUBBIN, SPECIES_RELLOR, SPECIES_JOLTIK, SPECIES_WEEDLE, SPECIES_LEDYBA, SPECIES_BURMY_TRASH, SPECIES_LOKIX, SPECIES_NYMBLE, SPECIES_COMBEE, SPECIES_BURMY},
        [2] = {SPECIES_SNOM, SPECIES_BURMY_SANDY, SPECIES_BLIPBUG, SPECIES_CATERPIE, SPECIES_COMBEE, SPECIES_SHELMET, SPECIES_DEWPIDER, SPECIES_TAROUNTULA, SPECIES_NINCADA, SPECIES_DWEBBLE},
        [3] = {SPECIES_DOTTLER, SPECIES_PARAS, SPECIES_SILCOON, SPECIES_WHIRLIPEDE, SPECIES_SKORUPI, SPECIES_SWADLOON, SPECIES_METAPOD, SPECIES_CASCOON, SPECIES_KAKUNA, SPECIES_ANORITH},
        [4] = {SPECIES_CHARJABUG, SPECIES_SWADLOON, SPECIES_DOTTLER, SPECIES_ILLUMISE, SPECIES_LARVESTA, SPECIES_SHEDINJA, SPECIES_LEDIAN, SPECIES_YANMA, SPECIES_WORMADAM_SANDY, SPECIES_WHIRLIPEDE},
        [5] = {SPECIES_VOLBEAT, SPECIES_SILVALLY_BUG, SPECIES_PARAS, SPECIES_DOTTLER, SPECIES_LEDIAN, SPECIES_LARVESTA, SPECIES_SPEWPA, SPECIES_METAPOD, SPECIES_CASCOON, SPECIES_SWADLOON},
        [6] = {SPECIES_HERACROSS, SPECIES_YANMEGA, SPECIES_VIVILLON_HIGH_PLAINS, SPECIES_BUTTERFREE, SPECIES_CENTISKORCH_GIGA, SPECIES_VIVILLON_POLAR, SPECIES_VIVILLON_MONSOON, SPECIES_GENESECT_CHILL, SPECIES_VIVILLON_MARINE, SPECIES_VIVILLON_RIVER},
        [7] = {SPECIES_ORBEETLE, SPECIES_VIVILLON, SPECIES_GENESECT_DOUSE, SPECIES_BEAUTIFLY, SPECIES_VIVILLON_POKEBALL, SPECIES_HERACROSS, SPECIES_VIVILLON_OCEAN, SPECIES_VIVILLON_ARCHIPELAGO, SPECIES_ARCEUS_BUG, SPECIES_BEEDRILL},
        [8] = {SPECIES_GALVANTULA, SPECIES_VIVILLON_SUN, SPECIES_CENTISKORCH_GIGA, SPECIES_GENESECT_BURN, SPECIES_BUTTERFREE, SPECIES_BEAUTIFLY, SPECIES_ARMALDO, SPECIES_DUSTOX, SPECIES_WORMADAM, SPECIES_ACCELGOR},
        [9] = {SPECIES_VIVILLON_RIVER, SPECIES_DURANT, SPECIES_GENESECT_BURN, SPECIES_SCYTHER, SPECIES_VIVILLON_SUN, SPECIES_GENESECT_SHOCK, SPECIES_ORBEETLE, SPECIES_GENESECT_CHILL, SPECIES_VIVILLON_SANDSTORM, SPECIES_VIVILLON_MODERN},
    },
    [CLASS_LASS] = {
        [0] = {SPECIES_BRAMBLEGHAST, SPECIES_FERROSEED, SPECIES_TURTWIG, SPECIES_APPLIN, SPECIES_IGGLYBUFF, SPECIES_PUMPKABOO, SPECIES_MEOWSCARADA, SPECIES_IMPIDIMP, SPECIES_SWIRLIX, SPECIES_CHERUBI},
        [1] = {SPECIES_DIPPLIN, SPECIES_FLABEBE_YELLOW, SPECIES_FIDOUGH, SPECIES_FEZANDIPITI, SPECIES_BOUNSWEET, SPECIES_IRON_VALIANT, SPECIES_ROWLET, SPECIES_DEERLING_AUTUMN, SPECIES_ARBOLIVA, SPECIES_PUMPKABOO},
        [2] = {SPECIES_MEOWSCARADA, SPECIES_DEERLING, SPECIES_PHANTUMP, SPECIES_SNUBBULL, SPECIES_GOSSIFLEUR, SPECIES_ROWLET, SPECIES_FIDOUGH, SPECIES_BRAMBLEGHAST, SPECIES_FLABEBE_YELLOW, SPECIES_FLABEBE_WHITE},
        [3] = {SPECIES_MIME_JR, SPECIES_PANSAGE, SPECIES_DARTRIX, SPECIES_ELECTRODE_H, SPECIES_JIGGLYPUFF, SPECIES_CLEFABLE, SPECIES_SUNFLORA, SPECIES_CHERRIM, SPECIES_BELLSPROUT, SPECIES_THWACKEY},
        [4] = {SPECIES_JIGGLYPUFF, SPECIES_SKIDDO, SPECIES_KIRLIA, SPECIES_SKIPLOOM, SPECIES_MAWILE_MEGA, SPECIES_SERVINE, SPECIES_ODDISH, SPECIES_MORGREM, SPECIES_PARASECT, SPECIES_LOMBRE},
        [5] = {SPECIES_MR_MIME, SPECIES_SEWADDLE, SPECIES_ODDISH, SPECIES_GLOOM, SPECIES_MAWILE, SPECIES_IVYSAUR, SPECIES_FLOETTE_ETERNAL, SPECIES_PIKACHU_POP_STAR, SPECIES_JIGGLYPUFF, SPECIES_PANSAGE},
        [6] = {SPECIES_SHAYMIN_SKY, SPECIES_SAWSBUCK_WINTER, SPECIES_TOGEKISS, SPECIES_GARDEVOIR_MEGA, SPECIES_AMOONGUSS, SPECIES_GRIMMSNARL_GIGA, SPECIES_TAPU_FINI, SPECIES_SHIFTRY, SPECIES_WEEPINBELL, SPECIES_ELDEGOSS},
        [7] = {SPECIES_VICTREEBEL, SPECIES_FLORGES_YELLOW, SPECIES_CARNIVINE, SPECIES_SAWSBUCK_SUMMER, SPECIES_VENUSAUR_MEGA, SPECIES_ALCREMIE_BERRY, SPECIES_AZUMARILL, SPECIES_HATTERENE, SPECIES_ARCEUS_GRASS, SPECIES_DHELMISE},
        [8] = {SPECIES_CALYREX, SPECIES_BRELOOM, SPECIES_SHIFTRY, SPECIES_VILEPLUME, SPECIES_SCEPTILE, SPECIES_FLORGES, SPECIES_ALCREMIE_RIBBON, SPECIES_FERROTHORN, SPECIES_ROSELIA, SPECIES_TAPU_FINI},
        [9] = {SPECIES_DIANCIE_MEGA, SPECIES_CHERRIM_SUN, SPECIES_COMFEY, SPECIES_SAWSBUCK_WINTER, SPECIES_GRIMMSNARL_GIGA, SPECIES_GARDEVOIR, SPECIES_TANGROWTH, SPECIES_ZARUDE, SPECIES_SAWSBUCK_AUTUMN, SPECIES_GARDEVOIR_MEGA},
    },
    [CLASS_SAILOR] = {
        [0] = {SPECIES_TATSUGIRI, SPECIES_WUGTRIO, SPECIES_IRON_BUNDLE, SPECIES_TATSUGIRI_RED, SPECIES_GREAT_TUSK, SPECIES_DEWPIDER, SPECIES_TOTODILE, SPECIES_FRILLISH_F, SPECIES_WIMPOD, SPECIES_SOBBLE},
        [1] = {SPECIES_TATSUGIRI_RED, SPECIES_CLOBBOPUS, SPECIES_DUCKLETT, SPECIES_QUAQUAVAL, SPECIES_FINIZEN, SPECIES_WALKING_WAKE, SPECIES_FEEBAS, SPECIES_QUAXLY, SPECIES_MARILL, SPECIES_SQUIRTLE},
        [2] = {SPECIES_QUAXLY, SPECIES_CHEWTLE, SPECIES_FRILLISH_F, SPECIES_FINIZEN, SPECIES_WIMPOD, SPECIES_DONDOZO, SPECIES_ANNIHILAPE, SPECIES_TATSUGIRI_YELLOW, SPECIES_QUAQUAVAL, SPECIES_SOBBLE},
        [3] = {SPECIES_DRIZZILE, SPECIES_MEDITITE, SPECIES_ARAQUANID, SPECIES_SILVALLY_FIGHT, SPECIES_BUIZEL, SPECIES_FINNEON, SPECIES_MACHOP, SPECIES_WISHIWASHI_S, SPECIES_MANTYKE, SPECIES_RIOLU},
        [4] = {SPECIES_FROGADIER, SPECIES_WISHIWASHI_S, SPECIES_TENTACOOL, SPECIES_BRIONNE, SPECIES_SLOWPOKE, SPECIES_GOLDEEN, SPECIES_KRABBY, SPECIES_MACHOKE, SPECIES_ARAQUANID, SPECIES_ROTOM_WASH},
        [5] = {SPECIES_HAKAMO_O, SPECIES_PANPOUR, SPECIES_PRINPLUP, SPECIES_KABUTO, SPECIES_TYMPOLE, SPECIES_POLIWHIRL, SPECIES_SLOWPOKE, SPECIES_PIGNITE, SPECIES_SNEASLER, SPECIES_MEDITITE},
        [6] = {SPECIES_SHARPEDO, SPECIES_JELLICENT_F, SPECIES_SWANNA, SPECIES_CRAWDAUNT, SPECIES_TAPU_FINI, SPECIES_SUICUNE, SPECIES_BLAZIKEN, SPECIES_DRACOVISH, SPECIES_URSHIFU_RAPID, SPECIES_DECIDUEYE_H},
        [7] = {SPECIES_LAPRAS, SPECIES_CARRACOSTA, SPECIES_BLASTOISE_MEGA, SPECIES_GASTRODON_EAST, SPECIES_ZAMAZENTA_CROWNED, SPECIES_TOXICROAK, SPECIES_ALOMOMOLA, SPECIES_EMBOAR, SPECIES_TAPU_FINI, SPECIES_BLASTOISE},
        [8] = {SPECIES_PRIMARINA, SPECIES_BLASTOISE_MEGA, SPECIES_GASTRODON, SPECIES_FERALIGATR, SPECIES_PALKIA_ORIGIN, SPECIES_POLIWRATH, SPECIES_BLAZIKEN_MEGA, SPECIES_GOLISOPOD, SPECIES_HERACROSS_MEGA, SPECIES_MANTINE},
        [9] = {SPECIES_ASHGRENINJA, SPECIES_BRELOOM, SPECIES_SHARPEDO_MEGA, SPECIES_KABUTOPS, SPECIES_POLIWRATH, SPECIES_POLITOED, SPECIES_MARSHADOW, SPECIES_GYARADOS_MEGA, SPECIES_GOLISOPOD, SPECIES_LUDICOLO},
    },
    [CLASS_CAMPER] = {
        [0] = {SPECIES_SQUAWKABILLY_WHITE, SPECIES_SILICOBRA, SPECIES_ZIGZAGOON, SPECIES_RATTATA, SPECIES_GREAT_TUSK, SPECIES_DITTO, SPECIES_MINCCINO, SPECIES_SHROODLE, SPECIES_SQUAWKABILLY, SPECIES_SQUAWKABILLY_BLUE},
        [1] = {SPECIES_MAUSHOLD_FOUR, SPECIES_SENTRET, SPECIES_SHROODLE, SPECIES_SQUAWKABILLY, SPECIES_OINKOLOGNE_FEMALE, SPECIES_GREAT_TUSK, SPECIES_TOEDSCRUEL, SPECIES_RATTATA_A, SPECIES_SQUAWKABILLY_WHITE, SPECIES_FARIGIRAF},
        [2] = {SPECIES_OINKOLOGNE, SPECIES_CLODSIRE, SPECIES_WOOPER, SPECIES_SKWOVET, SPECIES_BIDOOF, SPECIES_CYCLIZAR, SPECIES_IGGLYBUFF, SPECIES_GRAFAIAI, SPECIES_SQUAWKABILLY_YELLOW, SPECIES_WOOPER_P},
        [3] = {SPECIES_WATCHOG, SPECIES_CHATOT, SPECIES_DODUO, SPECIES_KROKOROK, SPECIES_JIGGLYPUFF, SPECIES_HIPPOPOTAS, SPECIES_DIGLETT, SPECIES_HIPPOPOTAS_F, SPECIES_LOUDRED, SPECIES_MAROWAK},
        [4] = {SPECIES_DRILBUR, SPECIES_KROKOROK, SPECIES_VIGOROTH, SPECIES_CASTFORM, SPECIES_EEVEE, SPECIES_QUAGSIRE, SPECIES_CUBONE, SPECIES_GABITE, SPECIES_MAROWAK, SPECIES_SANDYGAST},
        [5] = {SPECIES_WATCHOG, SPECIES_EEVEE_GIGA, SPECIES_SMEARGLE, SPECIES_LOUDRED, SPECIES_DELCATTY, SPECIES_TRUMBEAK, SPECIES_BARBOACH, SPECIES_BIBAREL, SPECIES_LICKITUNG, SPECIES_QUAGSIRE},
        [6] = {SPECIES_INDEEDEE, SPECIES_SANDACONDA, SPECIES_FURFROU_DEBUTANTE, SPECIES_CAMERUPT_MEGA, SPECIES_SAWSBUCK_WINTER, SPECIES_PIDGEOT_MEGA, SPECIES_RHYDON, SPECIES_LOPUNNY, SPECIES_FURFROU_DANDY, SPECIES_SAWSBUCK},
        [7] = {SPECIES_NIDOQUEEN, SPECIES_PORYGON_Z, SPECIES_SWELLOW, SPECIES_SAWSBUCK, SPECIES_FURFROU_DEBUTANTE, SPECIES_AUDINO_MEGA, SPECIES_OBSTAGOON, SPECIES_GLISCOR, SPECIES_FURFROU_DIAMOND, SPECIES_GOLEM},
        [8] = {SPECIES_SAWSBUCK_AUTUMN, SPECIES_SNORLAX, SPECIES_FURFROU_MATRON, SPECIES_MAMOSWINE, SPECIES_CAMERUPT, SPECIES_LANDORUS, SPECIES_OBSTAGOON, SPECIES_LOPUNNY, SPECIES_PORYGON_Z, SPECIES_STUNFISK},
        [9] = {SPECIES_URSARING, SPECIES_BEWEAR, SPECIES_LOPUNNY_MEGA, SPECIES_SANDACONDA, SPECIES_TOUCANNON, SPECIES_AUDINO_MEGA, SPECIES_RUNERIGUS, SPECIES_SLAKING, SPECIES_GOLURK, SPECIES_OBSTAGOON},
    },
    [CLASS_PICNICKER] = {
        [0] = {SPECIES_PHANTUMP, SPECIES_BUDEW, SPECIES_FLORAGATO, SPECIES_MILCERY, SPECIES_FOONGUS, SPECIES_TOEDSCRUEL, SPECIES_BRUTE_BONNET, SPECIES_SCOVILLAIN, SPECIES_IMPIDIMP, SPECIES_FERROSEED},
        [1] = {SPECIES_BRAMBLEGHAST, SPECIES_OGERPON_HEARTHFLAME_MASK, SPECIES_BOUNSWEET, SPECIES_TINKATUFF, SPECIES_OGERPON_CORNERSTONE_MASK, SPECIES_PETILIL, SPECIES_SCREAM_TAIL, SPECIES_BRAMBLIN, SPECIES_IRON_VALIANT, SPECIES_MEOWSCARADA},
        [2] = {SPECIES_SMOLIV, SPECIES_ROWLET, SPECIES_GOSSIFLEUR, SPECIES_MILCERY, SPECIES_IMPIDIMP, SPECIES_SCOVILLAIN, SPECIES_BRUTE_BONNET, SPECIES_AZURILL, SPECIES_DOLLIV, SPECIES_FOONGUS},
        [3] = {SPECIES_SKIPLOOM, SPECIES_PIKACHU_POP_STAR, SPECIES_MAWILE_MEGA, SPECIES_JIGGLYPUFF, SPECIES_RIBOMBEE, SPECIES_NUZLEAF, SPECIES_SILVALLY_FAIRY, SPECIES_PANSAGE, SPECIES_CARBINK, SPECIES_PARASECT},
        [4] = {SPECIES_FLOETTE_BLUE, SPECIES_SUNFLORA, SPECIES_PARASECT, SPECIES_STEENEE, SPECIES_ROTOM_MOW, SPECIES_CLEFABLE, SPECIES_MR_MIME, SPECIES_VOLTORB_H, SPECIES_LOMBRE, SPECIES_FOMANTIS},
        [5] = {SPECIES_MAWILE, SPECIES_GLOOM, SPECIES_MORELULL, SPECIES_SWADLOON, SPECIES_MORGREM, SPECIES_DARTRIX, SPECIES_LILLIGANT_H, SPECIES_VOLTORB_H, SPECIES_FOMANTIS, SPECIES_SKIPLOOM},
        [6] = {SPECIES_SAWSBUCK_SUMMER, SPECIES_AMOONGUSS, SPECIES_TAPU_FINI, SPECIES_SCEPTILE_MEGA, SPECIES_FLAPPLE, SPECIES_GOGOAT, SPECIES_VILEPLUME, SPECIES_SLURPUFF, SPECIES_ZACIAN, SPECIES_ALCREMIE_CLOVER},
        [7] = {SPECIES_SAWSBUCK_WINTER, SPECIES_ZARUDE, SPECIES_ALCREMIE_RIBBON, SPECIES_TSAREENA, SPECIES_GOURGEIST, SPECIES_DECIDUEYE_H, SPECIES_TAPU_LELE, SPECIES_TORTERRA, SPECIES_RILLABOOM_GIGA, SPECIES_TANGROWTH},
        [8] = {SPECIES_VIRIZION, SPECIES_TAPU_KOKO, SPECIES_LEAVANNY, SPECIES_MARACTUS, SPECIES_GARDEVOIR_MEGA, SPECIES_APPLETUN_GIGA, SPECIES_GOURGEIST_XL, SPECIES_CHESNAUGHT, SPECIES_SIMISAGE, SPECIES_MAGEARNA},
        [9] = {SPECIES_RILLABOOM_GIGA, SPECIES_SIMISAGE, SPECIES_ALCREMIE_STRAWBERRY, SPECIES_GRIMMSNARL, SPECIES_TAPU_BULU, SPECIES_ZACIAN_CROWNED, SPECIES_DHELMISE, SPECIES_JUMPLUFF, SPECIES_FLORGES_ORANGE, SPECIES_GRANBULL},
    },
    [CLASS_POKEMANIAC] = {
        [0] = {SPECIES_IRON_THORNS, SPECIES_GIBLE, SPECIES_IRON_BOULDER, SPECIES_TATSUGIRI_RED, SPECIES_BAXCALIBUR, SPECIES_TATSUGIRI, SPECIES_ROARING_MOON, SPECIES_DIPPLIN, SPECIES_NACLI, SPECIES_FRIGIBAX},
        [1] = {SPECIES_DWEBBLE, SPECIES_OGERPON_CORNERSTONE_MASK, SPECIES_DEINO, SPECIES_DIPPLIN, SPECIES_HYDRAPPLE, SPECIES_ARCTIBAX, SPECIES_ROLYCOLY, SPECIES_ROCKRUFF, SPECIES_BONSLY, SPECIES_APPLIN},
        [2] = {SPECIES_ROARING_MOON, SPECIES_NACLI, SPECIES_ARCHALUDON, SPECIES_RAGING_BOLT, SPECIES_OGERPON_CORNERSTONE_MASK, SPECIES_LARVITAR, SPECIES_FRIGIBAX, SPECIES_APPLIN, SPECIES_MIRAIDON, SPECIES_KLAWF},
        [3] = {SPECIES_GABITE, SPECIES_AMAURA, SPECIES_NOSEPASS, SPECIES_VIBRAVA, SPECIES_ZWEILOUS, SPECIES_GRAVELER_A, SPECIES_JANGMO_O, SPECIES_BAGON, SPECIES_KABUTO, SPECIES_CARKOL},
        [4] = {SPECIES_SILVALLY_DRAGON, SPECIES_GEODUDE, SPECIES_MINIOR_BLUE, SPECIES_AMAURA, SPECIES_LILEEP, SPECIES_CARKOL, SPECIES_GRAVELER, SPECIES_BAGON, SPECIES_ONIX, SPECIES_ARCHEN},
        [5] = {SPECIES_VIBRAVA, SPECIES_GRAVELER_A, SPECIES_FRAXURE, SPECIES_ARCHEN, SPECIES_RHYHORN, SPECIES_CARBINK, SPECIES_KABUTO, SPECIES_MINIOR_SHIELD, SPECIES_PUPITAR, SPECIES_CRANIDOS},
        [6] = {SPECIES_DRAMPA, SPECIES_KYUREM_WHITE, SPECIES_TURTONATOR, SPECIES_PALKIA_ORIGIN, SPECIES_ZYGARDE_COMPLETE, SPECIES_TYRANTRUM, SPECIES_AGGRON, SPECIES_ARCEUS_ROCK, SPECIES_DRAGONITE, SPECIES_APPLETUN},
        [7] = {SPECIES_AERODACTYL_MEGA, SPECIES_SCEPTILE_MEGA, SPECIES_KYUREM, SPECIES_ARMALDO, SPECIES_DIANCIE, SPECIES_AGGRON, SPECIES_DURALUDON, SPECIES_COALOSSAL_GIGA, SPECIES_DIALGA_ORIGIN, SPECIES_FLAPPLE},
        [8] = {SPECIES_PALKIA_ORIGIN, SPECIES_DRUDDIGON, SPECIES_LAIRON, SPECIES_SOLROCK, SPECIES_NAGANADEL, SPECIES_DRAMPA, SPECIES_MAGCARGO, SPECIES_BARBARACLE, SPECIES_GARCHOMP_MEGA, SPECIES_DREDNAW_GIGA},
        [9] = {SPECIES_STONJOURNER, SPECIES_CHARIZARD_MEGA_X, SPECIES_ARCANINE_H, SPECIES_ARCEUS_ROCK, SPECIES_ALTARIA_MEGA, SPECIES_DRAGAPULT, SPECIES_TURTONATOR, SPECIES_KYUREM, SPECIES_DREDNAW_GIGA, SPECIES_EXEGGUTOR_A},
    },
    [CLASS_SUPER_NERD] = {
        [0] = {SPECIES_WATTREL, SPECIES_TYNAMO, SPECIES_TINKATUFF, SPECIES_FERROSEED, SPECIES_PICHU, SPECIES_MEOWTH_G, SPECIES_REVAVROOM, SPECIES_MIRAIDON, SPECIES_IRON_THORNS, SPECIES_PAWMOT},
        [1] = {SPECIES_FERROSEED, SPECIES_TYNAMO, SPECIES_SANDY_SHOCKS, SPECIES_YAMPER, SPECIES_TOXEL, SPECIES_VAROOM, SPECIES_REVAVROOM, SPECIES_PICHU, SPECIES_PICHU_SPIKY, SPECIES_TINKATON},
        [2] = {SPECIES_HONEDGE, SPECIES_TINKATINK, SPECIES_IRON_CROWN, SPECIES_IRON_THORNS, SPECIES_BELLIBOLT, SPECIES_ORTHWORM, SPECIES_TOXEL, SPECIES_TYNAMO, SPECIES_PAWNIARD, SPECIES_TADBULB},
        [3] = {SPECIES_MAGEARNA_P, SPECIES_PIKACHU_FLYING, SPECIES_PIKACHU_CAP_ORIGINAL, SPECIES_ROTOM_FAN, SPECIES_PIKACHU_CAP_ALOLA, SPECIES_PIKACHU_POP_STAR, SPECIES_PLUSLE, SPECIES_ROTOM_HEAT, SPECIES_ELECTRODE_H, SPECIES_ELECTRIKE},
        [4] = {SPECIES_SILVALLY_ELECTRIC, SPECIES_MAWILE, SPECIES_BRONZOR, SPECIES_DIGLETT_A, SPECIES_MINUN, SPECIES_PIKACHU_SURFING, SPECIES_PIKACHU_FLYING, SPECIES_PIKACHU_GIGA, SPECIES_PIKACHU, SPECIES_PIKACHU_POP_STAR},
        [5] = {SPECIES_PIKACHU_POP_STAR, SPECIES_PIKACHU_PHD, SPECIES_FLAAFFY, SPECIES_PIKACHU_CAP_ALOLA, SPECIES_SILVALLY_STEEL, SPECIES_ROTOM, SPECIES_TOGEDEMARU, SPECIES_PIKACHU_LIBRE, SPECIES_PIKACHU, SPECIES_DIGLETT_A},
        [6] = {SPECIES_TOXTRICITY_GIGA, SPECIES_TOXTRICITY, SPECIES_TOXTRICITY_LOW_KEY, SPECIES_STUNFISK_G, SPECIES_EMOLGA, SPECIES_AEGISLASH_BLADE, SPECIES_MAGNETON, SPECIES_COPPERAJAH, SPECIES_COBALION, SPECIES_THUNDURUS},
        [7] = {SPECIES_TOXTRICITY_LOW_KEY_GIGA, SPECIES_DUGTRIO_A, SPECIES_MANECTRIC, SPECIES_AGGRON_MEGA, SPECIES_ZAMAZENTA_CROWNED, SPECIES_ARCTOZOLT, SPECIES_ZAPDOS, SPECIES_JOLTEON, SPECIES_ELECTRODE, SPECIES_LANTURN},
        [8] = {SPECIES_MELMETAL, SPECIES_GENESECT_BURN, SPECIES_TOXTRICITY_LOW_KEY, SPECIES_EXCADRILL, SPECIES_LANTURN, SPECIES_MORPEKO_HANGRY, SPECIES_ZEBSTRIKA, SPECIES_METAGROSS, SPECIES_DIALGA, SPECIES_ARCTOZOLT},
        [9] = {SPECIES_MORPEKO_HANGRY, SPECIES_DURANT, SPECIES_MAGNEZONE, SPECIES_GOODRA_H, SPECIES_HEATRAN, SPECIES_MELTAN, SPECIES_FALINKS, SPECIES_ELECTABUZZ, SPECIES_AGGRON_MEGA, SPECIES_EMPOLEON},
    },
    [CLASS_HIKER] = {
        [0] = {SPECIES_GARGANACL, SPECIES_GIBLE, SPECIES_GLIMMORA, SPECIES_BONSLY, SPECIES_KLAWF, SPECIES_IRON_BOULDER, SPECIES_SANDY_SHOCKS, SPECIES_GLIMMET, SPECIES_IRON_THORNS, SPECIES_LARVITAR},
        [1] = {SPECIES_DWEBBLE, SPECIES_TOEDSCOOL, SPECIES_GOLETT, SPECIES_BONSLY, SPECIES_ROLYCOLY, SPECIES_URSALUNA_BLOODMOON, SPECIES_OGERPON_CORNERSTONE_MASK, SPECIES_WOOPER, SPECIES_LARVITAR, SPECIES_KLAWF},
        [2] = {SPECIES_SANDILE, SPECIES_BONSLY, SPECIES_WOOPER_P, SPECIES_BINACLE, SPECIES_GOLETT, SPECIES_SANDY_SHOCKS, SPECIES_GREAT_TUSK, SPECIES_DWEBBLE, SPECIES_GLIMMORA, SPECIES_ROLYCOLY},
        [3] = {SPECIES_GEODUDE, SPECIES_CARKOL, SPECIES_KROKOROK, SPECIES_TRAPINCH, SPECIES_PHANPY, SPECIES_BOLDORE, SPECIES_ANORITH, SPECIES_DIGGERSBY, SPECIES_PUPITAR, SPECIES_CORSOLA},
        [4] = {SPECIES_CUBONE, SPECIES_GLIGAR, SPECIES_MINIOR_BLUE, SPECIES_MINIOR_INDIGO, SPECIES_GRAVELER, SPECIES_GEODUDE_A, SPECIES_ROGGENROLA, SPECIES_GROWLITHE_H, SPECIES_MINIOR_SHIELD, SPECIES_KROKOROK},
        [5] = {SPECIES_DIGLETT_A, SPECIES_LILEEP, SPECIES_SWINUB, SPECIES_LYCANROC_N, SPECIES_ARON, SPECIES_MINIOR_INDIGO, SPECIES_MINIOR_GREEN, SPECIES_CUBONE_A, SPECIES_MINIOR_VIOLET, SPECIES_WORMADAM_SANDY},
        [6] = {SPECIES_CRUSTLE, SPECIES_TORTERRA, SPECIES_LAIRON, SPECIES_KABUTOPS, SPECIES_DUGTRIO, SPECIES_MAGCARGO, SPECIES_AERODACTYL, SPECIES_CARRACOSTA, SPECIES_AERODACTYL_MEGA, SPECIES_STEELIX},
        [7] = {SPECIES_COALOSSAL, SPECIES_DUGTRIO, SPECIES_LUNATONE, SPECIES_DIANCIE, SPECIES_LAIRON, SPECIES_WHISCASH, SPECIES_MUDSDALE, SPECIES_TYRANITAR_MEGA, SPECIES_COALOSSAL_GIGA, SPECIES_AERODACTYL_MEGA},
        [8] = {SPECIES_STAKATAKA, SPECIES_PROBOPASS, SPECIES_TERRAKION, SPECIES_LAIRON, SPECIES_DUGTRIO_A, SPECIES_REGIROCK, SPECIES_TYRANTRUM, SPECIES_KLEAVOR, SPECIES_SANDACONDA, SPECIES_ZYGARDE_10},
        [9] = {SPECIES_BARBARACLE, SPECIES_CRUSTLE, SPECIES_ARMALDO, SPECIES_PALOSSAND, SPECIES_OMASTAR, SPECIES_MAMOSWINE, SPECIES_ARCEUS_GROUND, SPECIES_CAMERUPT, SPECIES_GROUDON_PRIMAL, SPECIES_KABUTOPS},
    },
    [CLASS_BIKER] = {
        [0] = {SPECIES_LOKIX, SPECIES_DEINO, SPECIES_WO_CHIEN, SPECIES_MASCHIFF, SPECIES_NIDORAN_M, SPECIES_SPINARAK, SPECIES_TING_LU, SPECIES_SHROODLE, SPECIES_IRON_JUGULIS, SPECIES_PURRLOIN},
        [1] = {SPECIES_TRUBBISH, SPECIES_VAROOM, SPECIES_PURRLOIN, SPECIES_EKANS, SPECIES_NIDORAN_F, SPECIES_LOKIX, SPECIES_MEOWTH_A, SPECIES_IRON_JUGULIS, SPECIES_BRUTE_BONNET, SPECIES_MABOSSTIFF},
        [2] = {SPECIES_GRAFAIAI, SPECIES_BUDEW, SPECIES_WO_CHIEN, SPECIES_SKRELP, SPECIES_NIDORAN_M, SPECIES_INKAY, SPECIES_TOXEL, SPECIES_BRUTE_BONNET, SPECIES_MABOSSTIFF, SPECIES_TING_LU},
        [3] = {SPECIES_GASTLY, SPECIES_VENONAT, SPECIES_LINOONE_G, SPECIES_WHIRLIPEDE, SPECIES_PERSIAN_A, SPECIES_NIDORINA, SPECIES_ZWEILOUS, SPECIES_HAUNTER, SPECIES_TENTACOOL, SPECIES_SILVALLY_DARK},
        [4] = {SPECIES_GLOOM, SPECIES_LIEPARD, SPECIES_VENOMOTH, SPECIES_ARBOK, SPECIES_NUZLEAF, SPECIES_SCRAGGY, SPECIES_PERSIAN_A, SPECIES_SABLEYE, SPECIES_SKORUPI, SPECIES_MIGHTYENA},
        [5] = {SPECIES_TENTACOOL, SPECIES_GLOOM, SPECIES_CARVANHA, SPECIES_RATICATE_A, SPECIES_SNEASEL_H, SPECIES_STUNKY, SPECIES_GULPIN, SPECIES_HAUNTER, SPECIES_HOUNDOUR, SPECIES_WHIRLIPEDE},
        [6] = {SPECIES_TOXTRICITY_LOW_KEY, SPECIES_GENGAR_GIGA, SPECIES_GRIMMSNARL_GIGA, SPECIES_INCINEROAR, SPECIES_TOXTRICITY_LOW_KEY_GIGA, SPECIES_ASHGRENINJA, SPECIES_UMBREON, SPECIES_TYRANITAR_MEGA, SPECIES_SWALOT, SPECIES_MUK},
        [7] = {SPECIES_MUK_A, SPECIES_HOOPA_UNBOUND, SPECIES_THIEVUL, SPECIES_CRAWDAUNT, SPECIES_MALAMAR, SPECIES_HYDREIGON, SPECIES_DRAGALGE, SPECIES_ZOROARK, SPECIES_SLOWKING_G, SPECIES_ZARUDE_DADA},
        [8] = {SPECIES_TOXTRICITY_GIGA, SPECIES_GARBODOR, SPECIES_MUK_A, SPECIES_THIEVUL, SPECIES_ARCEUS_DARK, SPECIES_INCINEROAR, SPECIES_VILEPLUME, SPECIES_SEVIPER, SPECIES_MORPEKO, SPECIES_BEEDRILL},
        [9] = {SPECIES_TYRANITAR, SPECIES_ZARUDE, SPECIES_WEEZING, SPECIES_HONCHKROW, SPECIES_TOXTRICITY, SPECIES_URSHIFU_SINGLE_GIGA, SPECIES_BEEDRILL, SPECIES_DUSTOX, SPECIES_MUK_A, SPECIES_ZOROARK},
    },
    [CLASS_BURGLAR] = {
        [0] = {SPECIES_FUECOCO, SPECIES_ARMAROUGE, SPECIES_CHIEN_PAO, SPECIES_TAUROS_BLAZE_P, SPECIES_MEOWTH_A, SPECIES_SCORBUNNY, SPECIES_TING_LU, SPECIES_CROCALOR, SPECIES_IMPIDIMP, SPECIES_BOMBIRDIER},
        [1] = {SPECIES_CYNDAQUIL, SPECIES_CHIMCHAR, SPECIES_RATTATA_A, SPECIES_CHARCADET, SPECIES_MABOSSTIFF, SPECIES_SKELEDIRGE, SPECIES_CROCALOR, SPECIES_WO_CHIEN, SPECIES_SIZZLIPEDE, SPECIES_MEOWTH_A},
        [2] = {SPECIES_MEOWTH_A, SPECIES_LOKIX, SPECIES_TEPIG, SPECIES_CYNDAQUIL, SPECIES_BOMBIRDIER, SPECIES_CHI_YU, SPECIES_MEOWSCARADA, SPECIES_LITTEN, SPECIES_TORCHIC, SPECIES_IMPIDIMP},
        [3] = {SPECIES_TORRACAT, SPECIES_SILVALLY_FIRE, SPECIES_PANSEAR, SPECIES_RATICATE_A, SPECIES_NUMEL, SPECIES_COMBUSKEN, SPECIES_NUZLEAF, SPECIES_HOUNDOUR, SPECIES_GROWLITHE, SPECIES_SNEASEL},
        [4] = {SPECIES_RATICATE_A, SPECIES_PERSIAN_A, SPECIES_GRIMER_A, SPECIES_GROWLITHE, SPECIES_PIGNITE, SPECIES_SABLEYE_MEGA, SPECIES_SABLEYE, SPECIES_SLUGMA, SPECIES_MIGHTYENA, SPECIES_QWILFISH_H},
        [5] = {SPECIES_PERSIAN_A, SPECIES_SABLEYE_MEGA, SPECIES_GROWLITHE, SPECIES_SCRAGGY, SPECIES_MURKROW, SPECIES_OVERQWIL, SPECIES_GROWLITHE_H, SPECIES_HOUNDOUR, SPECIES_COMBUSKEN, SPECIES_MAGBY},
        [6] = {SPECIES_KROOKODILE, SPECIES_HONCHKROW, SPECIES_GUZZLORD, SPECIES_LUXRAY, SPECIES_BLAZIKEN_MEGA, SPECIES_TURTONATOR, SPECIES_MAGMORTAR, SPECIES_GRENINJA, SPECIES_MUK_A, SPECIES_FLAREON},
        [7] = {SPECIES_ORICORIO, SPECIES_INFERNAPE, SPECIES_TYPHLOSION_H, SPECIES_DARMANITAN, SPECIES_CHARIZARD_GIGA, SPECIES_ZOROARK, SPECIES_DARMANITAN_G_ZEN, SPECIES_HO_OH, SPECIES_TALONFLAME, SPECIES_CHARIZARD_MEGA_X},
        [8] = {SPECIES_SHIFTRY, SPECIES_CAMERUPT, SPECIES_BLAZIKEN, SPECIES_RESHIRAM, SPECIES_HOUNDOOM, SPECIES_GRIMMSNARL, SPECIES_CRAWDAUNT, SPECIES_MOLTRES, SPECIES_HONCHKROW, SPECIES_INCINEROAR},
        [9] = {SPECIES_ENTEI, SPECIES_CAMERUPT, SPECIES_MAGMORTAR, SPECIES_MAGMAR, SPECIES_SHARPEDO_MEGA, SPECIES_ZARUDE_DADA, SPECIES_WEAVILE, SPECIES_GRENINJA, SPECIES_CHARIZARD_GIGA, SPECIES_SAMUROTT_H},
    },
    [CLASS_ENGINEER] = {
        [0] = {SPECIES_ORTHWORM, SPECIES_BELLIBOLT, SPECIES_KLINK, SPECIES_JOLTIK, SPECIES_VAROOM, SPECIES_RAGING_BOLT, SPECIES_PICHU_SPIKY, SPECIES_HELIOPTILE, SPECIES_TINKATUFF, SPECIES_IRON_CROWN},
        [1] = {SPECIES_TYNAMO, SPECIES_CUFANT, SPECIES_IRON_TREADS, SPECIES_PAWMO, SPECIES_PICHU, SPECIES_PAWNIARD, SPECIES_TINKATUFF, SPECIES_SHINX, SPECIES_ARCHALUDON, SPECIES_KILOWATTREL},
        [2] = {SPECIES_MIRAIDON, SPECIES_TINKATON, SPECIES_TOXEL, SPECIES_KLINK, SPECIES_BELLIBOLT, SPECIES_JOLTIK, SPECIES_TINKATINK, SPECIES_IRON_CROWN, SPECIES_ORTHWORM, SPECIES_SANDY_SHOCKS},
        [3] = {SPECIES_FLAAFFY, SPECIES_TOGEDEMARU, SPECIES_RAICHU_A, SPECIES_PIKACHU_COSPLAY, SPECIES_FORRETRESS, SPECIES_SILVALLY_STEEL, SPECIES_MAGEARNA_P, SPECIES_CHARJABUG, SPECIES_PLUSLE, SPECIES_SANDSHREW_A},
        [4] = {SPECIES_PIKACHU, SPECIES_ELECTRIKE, SPECIES_MAGNEMITE, SPECIES_DIGLETT_A, SPECIES_MAGEARNA_P, SPECIES_CHINCHOU, SPECIES_ROTOM, SPECIES_ROTOM_MOW, SPECIES_RAICHU, SPECIES_ELEKID},
        [5] = {SPECIES_PIKACHU_BELLE, SPECIES_CHARJABUG, SPECIES_MAGNEMITE, SPECIES_VOLTORB_H, SPECIES_PIKACHU_CAP_KALOS, SPECIES_PIKACHU_POP_STAR, SPECIES_RAICHU, SPECIES_MAWILE_MEGA, SPECIES_PIKACHU, SPECIES_PIKACHU_CAP_UNOVA},
        [6] = {SPECIES_MANECTRIC, SPECIES_STUNFISK, SPECIES_STUNFISK_G, SPECIES_HELIOLISK, SPECIES_DIALGA_ORIGIN, SPECIES_NECROZMA_DUSK_MANE, SPECIES_EMPOLEON, SPECIES_DURANT, SPECIES_TOXTRICITY, SPECIES_KLINKLANG},
        [7] = {SPECIES_SKARMORY, SPECIES_LANTURN, SPECIES_LUCARIO, SPECIES_DOUBLADE, SPECIES_KLEFKI, SPECIES_DURALUDON, SPECIES_GENESECT_BURN, SPECIES_COBALION, SPECIES_GENESECT_SHOCK, SPECIES_ORICORIO_Y},
        [8] = {SPECIES_PERRSERKER, SPECIES_BISHARP, SPECIES_STEELIX_MEGA, SPECIES_ZAMAZENTA_CROWNED, SPECIES_GENESECT_SHOCK, SPECIES_ELECTIVIRE, SPECIES_SCIZOR_MEGA, SPECIES_METAGROSS, SPECIES_THUNDURUS_THERIAN, SPECIES_GALVANTULA},
        [9] = {SPECIES_CORVIKNIGHT_GIGA, SPECIES_REGISTEEL, SPECIES_BASTIODON, SPECIES_ZEBSTRIKA, SPECIES_HEATRAN, SPECIES_BISHARP, SPECIES_METAGROSS_MEGA, SPECIES_DUGTRIO_A, SPECIES_SOLGALEO, SPECIES_MORPEKO},
    },
    [CLASS_FISHERMAN] = {
        [0] = {SPECIES_QUAXWELL, SPECIES_OGERPON_WELLSPRING_MASK, SPECIES_WINGULL, SPECIES_QUAXLY, SPECIES_FRILLISH_F, SPECIES_BINACLE, SPECIES_POPPLIO, SPECIES_SOBBLE, SPECIES_DONDOZO, SPECIES_WOOPER},
        [1] = {SPECIES_SHELLOS_EAST, SPECIES_SURSKIT, SPECIES_FRILLISH, SPECIES_QUAQUAVAL, SPECIES_WUGTRIO, SPECIES_DONDOZO, SPECIES_OGERPON_WELLSPRING_MASK, SPECIES_WIMPOD, SPECIES_PIPLUP, SPECIES_MAGIKARP},
        [2] = {SPECIES_QUAXLY, SPECIES_PALAFIN_HERO, SPECIES_WIGLETT, SPECIES_QUAXWELL, SPECIES_MARILL, SPECIES_WINGULL, SPECIES_DUCKLETT, SPECIES_QUAQUAVAL, SPECIES_CHEWTLE, SPECIES_SKRELP},
        [3] = {SPECIES_SILVALLY_WATER, SPECIES_BASCULEGION_F, SPECIES_BUIZEL, SPECIES_LUVDISC, SPECIES_WARTORTLE, SPECIES_FINNEON, SPECIES_BARBOACH, SPECIES_BRIONNE, SPECIES_SPHEAL, SPECIES_BASCULIN_BLUE},
        [4] = {SPECIES_SHELLOS, SPECIES_MAREANIE, SPECIES_PANPOUR, SPECIES_ARAQUANID, SPECIES_PALPITOAD, SPECIES_OMANYTE, SPECIES_DEWOTT, SPECIES_SHELLDER, SPECIES_BASCULEGION_M, SPECIES_POLIWAG},
        [5] = {SPECIES_OMANYTE, SPECIES_TYMPOLE, SPECIES_CLAWITZER, SPECIES_GOLDEEN, SPECIES_WAILMER, SPECIES_MAREANIE, SPECIES_MANTYKE, SPECIES_LUVDISC, SPECIES_DRIZZILE, SPECIES_BIBAREL},
        [6] = {SPECIES_BARRASKEWDA, SPECIES_SHARPEDO, SPECIES_DRACOVISH, SPECIES_TENTACRUEL, SPECIES_INTELEON, SPECIES_PALKIA_ORIGIN, SPECIES_GRENINJA, SPECIES_KABUTOPS, SPECIES_KINGLER, SPECIES_SEAKING},
        [7] = {SPECIES_CRAWDAUNT, SPECIES_ASHGRENINJA, SPECIES_GOLISOPOD, SPECIES_DEWGONG, SPECIES_KYOGRE, SPECIES_LAPRAS_GIGA, SPECIES_FERALIGATR, SPECIES_KELDEO_RESOLUTE, SPECIES_TENTACRUEL, SPECIES_TAPU_FINI},
        [8] = {SPECIES_GASTRODON, SPECIES_GOREBYSS, SPECIES_KYOGRE, SPECIES_DRACOVISH, SPECIES_KELDEO_RESOLUTE, SPECIES_ARCTOVISH, SPECIES_EMPOLEON, SPECIES_BRUXISH, SPECIES_GOLDUCK, SPECIES_TOXAPEX},
        [9] = {SPECIES_CRAWDAUNT, SPECIES_INTELEON_GIGA, SPECIES_WALREIN, SPECIES_SWAMPERT, SPECIES_GOLDUCK, SPECIES_LAPRAS_GIGA, SPECIES_DRACOVISH, SPECIES_MILOTIC, SPECIES_PALKIA_ORIGIN, SPECIES_RELICANTH},
    },
    [CLASS_SWIMMER_M] = {
        [0] = {SPECIES_IRON_BUNDLE, SPECIES_WINGULL, SPECIES_TATSUGIRI_YELLOW, SPECIES_TATSUGIRI, SPECIES_QUAXLY, SPECIES_BINACLE, SPECIES_PALAFIN, SPECIES_SHELLOS_EAST, SPECIES_WALKING_WAKE, SPECIES_FROAKIE},
        [1] = {SPECIES_TATSUGIRI_RED, SPECIES_ARROKUDA, SPECIES_WINGULL, SPECIES_BINACLE, SPECIES_MUDKIP, SPECIES_PALAFIN, SPECIES_WALKING_WAKE, SPECIES_SHELLOS_EAST, SPECIES_WIGLETT, SPECIES_QUAXLY},
        [2] = {SPECIES_DUCKLETT, SPECIES_BINACLE, SPECIES_FROAKIE, SPECIES_SKRELP, SPECIES_CLAUNCHER, SPECIES_WOOPER, SPECIES_PIPLUP, SPECIES_OSHAWOTT, SPECIES_ARROKUDA, SPECIES_WALKING_WAKE},
        [3] = {SPECIES_CLAWITZER, SPECIES_BIBAREL, SPECIES_DEWOTT, SPECIES_QUAGSIRE, SPECIES_OMANYTE, SPECIES_SILVALLY_WATER, SPECIES_SEEL, SPECIES_MARSHTOMP, SPECIES_PANPOUR, SPECIES_CHINCHOU},
        [4] = {SPECIES_CLAMPERL, SPECIES_ROTOM_WASH, SPECIES_TIRTOUGA, SPECIES_LOTAD, SPECIES_WAILMER, SPECIES_REMORAID, SPECIES_SILVALLY_WATER, SPECIES_BASCULIN_RED, SPECIES_LOMBRE, SPECIES_FROGADIER},
        [5] = {SPECIES_CLAWITZER, SPECIES_POLIWHIRL, SPECIES_CARVANHA, SPECIES_SEALEO, SPECIES_CROCONAW, SPECIES_BASCULIN_BLUE, SPECIES_SHELLOS, SPECIES_HORSEA, SPECIES_FROGADIER, SPECIES_PYUKUMUKU},
        [6] = {SPECIES_TAPU_FINI, SPECIES_SHARPEDO, SPECIES_TENTACRUEL, SPECIES_CLOYSTER, SPECIES_CRAWDAUNT, SPECIES_BARRASKEWDA, SPECIES_LAPRAS_GIGA, SPECIES_WHISCASH, SPECIES_BLASTOISE, SPECIES_SAMUROTT_H},
        [7] = {SPECIES_BLASTOISE_GIGA, SPECIES_SUICUNE, SPECIES_OMASTAR, SPECIES_GYARADOS_MEGA, SPECIES_SIMIPOUR, SPECIES_WAILORD, SPECIES_POLITOED, SPECIES_TOXAPEX, SPECIES_SWANNA, SPECIES_GOREBYSS},
        [8] = {SPECIES_KINGDRA, SPECIES_PALKIA, SPECIES_TENTACRUEL, SPECIES_ARCTOVISH, SPECIES_URSHIFU_RAPID_GIGA, SPECIES_INTELEON, SPECIES_BRUXISH, SPECIES_SWANNA, SPECIES_SUICUNE, SPECIES_EMPOLEON},
        [9] = {SPECIES_POLITOED, SPECIES_KINGLER, SPECIES_BLASTOISE, SPECIES_SLOWKING, SPECIES_JELLICENT, SPECIES_KYOGRE_PRIMAL, SPECIES_TENTACRUEL, SPECIES_FLOATZEL, SPECIES_DREDNAW, SPECIES_SHARPEDO_MEGA},
    },
    [CLASS_CUE_BALL] = {
        [0] = {SPECIES_PIDGEY, SPECIES_HELIOPTILE, SPECIES_SPEAROW, SPECIES_ZORUA_H, SPECIES_AZURILL, SPECIES_ZIGZAGOON_G, SPECIES_WHISMUR, SPECIES_ARBOLIVA, SPECIES_TANDEMAUS, SPECIES_MEOWTH},
        [1] = {SPECIES_HELIOPTILE, SPECIES_FARIGIRAF, SPECIES_SQUAWKABILLY, SPECIES_DEERLING_WINTER, SPECIES_DOLLIV, SPECIES_MAUSHOLD_FOUR, SPECIES_GRAFAIAI, SPECIES_SQUAWKABILLY_YELLOW, SPECIES_SENTRET, SPECIES_TERAPAGOS_TERASTAL},
        [2] = {SPECIES_HOOTHOOT, SPECIES_TAILLOW, SPECIES_PIDGEY, SPECIES_STARLY, SPECIES_WHISMUR, SPECIES_FLETCHLING, SPECIES_SQUAWKABILLY_YELLOW, SPECIES_LECHONK, SPECIES_TERAPAGOS_TERASTAL, SPECIES_BUNNELBY},
        [3] = {SPECIES_RUFFLET, SPECIES_GUMSHOOS, SPECIES_WIGGLYTUFF, SPECIES_RATICATE_A, SPECIES_SMEARGLE, SPECIES_TRANQUILL, SPECIES_TRUMBEAK, SPECIES_TEDDIURSA, SPECIES_LICKITUNG, SPECIES_SWABLU},
        [4] = {SPECIES_DIGGERSBY, SPECIES_BIBAREL, SPECIES_LICKITUNG, SPECIES_WIGGLYTUFF, SPECIES_RUFFLET, SPECIES_GLAMEOW, SPECIES_LINOONE, SPECIES_DODUO, SPECIES_EEVEE, SPECIES_GIRAFARIG},
        [5] = {SPECIES_LOUDRED, SPECIES_FARFETCHD, SPECIES_LINOONE_G, SPECIES_PORYGON, SPECIES_SWABLU, SPECIES_FURRET, SPECIES_TRANQUILL, SPECIES_CASTFORM, SPECIES_CHATOT, SPECIES_WATCHOG},
        [6] = {SPECIES_EXPLOUD, SPECIES_WYRDEER, SPECIES_INDEEDEE_FEMALE, SPECIES_OBSTAGOON, SPECIES_FURFROU_MATRON, SPECIES_LICKILICKY, SPECIES_FURFROU_PHAROAH, SPECIES_SWELLOW, SPECIES_LOPUNNY_MEGA, SPECIES_INDEEDEE},
        [7] = {SPECIES_MILTANK, SPECIES_UNFEZANT, SPECIES_ZOROARK_H, SPECIES_STOUTLAND, SPECIES_KOMALA, SPECIES_PURUGLY, SPECIES_DUBWOOL, SPECIES_BEWEAR, SPECIES_PYROAR, SPECIES_TAUROS},
        [8] = {SPECIES_SAWSBUCK_SUMMER, SPECIES_CHANSEY, SPECIES_AUDINO_MEGA, SPECIES_ZANGOOSE, SPECIES_FURFROU_HEART, SPECIES_UNFEZANT_F, SPECIES_LOPUNNY, SPECIES_REGIGIGAS, SPECIES_PYROAR, SPECIES_BOUFFALANT},
        [9] = {SPECIES_BEWEAR, SPECIES_TAUROS, SPECIES_ZANGOOSE, SPECIES_INDEEDEE_FEMALE, SPECIES_PIDGEOT, SPECIES_HAPPINY, SPECIES_MELOETTA, SPECIES_BLISSEY, SPECIES_CINCCINO, SPECIES_LOPUNNY},
    },
    [CLASS_GAMER] = {
        [0] = {SPECIES_BUNNELBY, SPECIES_SQUAWKABILLY, SPECIES_DOLLIV, SPECIES_TERAPAGOS_STELLAR, SPECIES_DEERLING, SPECIES_MEOWTH, SPECIES_STARLY, SPECIES_TAILLOW, SPECIES_WHISMUR, SPECIES_TANDEMAUS},
        [1] = {SPECIES_MINCCINO, SPECIES_OINKOLOGNE_FEMALE, SPECIES_SQUAWKABILLY_WHITE, SPECIES_BUNNELBY, SPECIES_HOOTHOOT, SPECIES_PIDGEY, SPECIES_TERAPAGOS, SPECIES_TAILLOW, SPECIES_SKITTY, SPECIES_SKWOVET},
        [2] = {SPECIES_PATRAT, SPECIES_TERAPAGOS, SPECIES_MAUSHOLD, SPECIES_IGGLYBUFF, SPECIES_LILLIPUP, SPECIES_SHROODLE, SPECIES_FLETCHLING, SPECIES_EGG, SPECIES_RATTATA_A, SPECIES_DUDUNSPARCE},
        [3] = {SPECIES_SPINDA, SPECIES_MUNCHLAX, SPECIES_DIGGERSBY, SPECIES_JIGGLYPUFF, SPECIES_TRUMBEAK, SPECIES_GIRAFARIG, SPECIES_WATCHOG, SPECIES_CHATOT, SPECIES_TRANQUILL, SPECIES_LINOONE},
        [4] = {SPECIES_STARAVIA, SPECIES_TEDDIURSA, SPECIES_GIRAFARIG, SPECIES_VIGOROTH, SPECIES_SPINDA, SPECIES_GLAMEOW, SPECIES_DIGGERSBY, SPECIES_HERDIER, SPECIES_DELCATTY, SPECIES_AIPOM},
        [5] = {SPECIES_WOOLOO, SPECIES_DODUO, SPECIES_CHATOT, SPECIES_SMEARGLE, SPECIES_LINOONE_G, SPECIES_TYPE_NULL, SPECIES_CASTFORM, SPECIES_STUFFUL, SPECIES_EEVEE_GIGA, SPECIES_MUNCHLAX},
        [6] = {SPECIES_INDEEDEE, SPECIES_KANGASKHAN, SPECIES_MILTANK, SPECIES_SILVALLY, SPECIES_LOPUNNY, SPECIES_DODRIO, SPECIES_UNFEZANT_F, SPECIES_SNORLAX_GIGA, SPECIES_URSARING, SPECIES_FURFROU_KABUKI},
        [7] = {SPECIES_FURFROU_STAR, SPECIES_FURFROU_DEBUTANTE, SPECIES_BOUFFALANT, SPECIES_DUBWOOL, SPECIES_PIDGEOT_MEGA, SPECIES_SLAKING, SPECIES_PYROAR, SPECIES_FURFROU_MATRON, SPECIES_URSARING, SPECIES_AMBIPOM},
        [8] = {SPECIES_SAWSBUCK, SPECIES_TAUROS, SPECIES_URSALUNA, SPECIES_DODRIO, SPECIES_URSARING, SPECIES_PIDGEOT_MEGA, SPECIES_MILTANK, SPECIES_MELOETTA, SPECIES_ZANGOOSE, SPECIES_UNFEZANT_F},
        [9] = {SPECIES_SLAKING, SPECIES_FEAROW, SPECIES_GREEDENT, SPECIES_KANGASKHAN, SPECIES_PURUGLY, SPECIES_FURFROU_LA_REINE, SPECIES_BRAVIARY, SPECIES_SAWSBUCK, SPECIES_BEWEAR, SPECIES_TAUROS},
    },
    [CLASS_BEAUTY] = {
        [0] = {SPECIES_SNUBBULL, SPECIES_FARIGIRAF, SPECIES_FLABEBE_ORANGE, SPECIES_TAILLOW, SPECIES_SQUAWKABILLY_YELLOW, SPECIES_TANDEMAUS, SPECIES_TERAPAGOS_STELLAR, SPECIES_MILCERY, SPECIES_LECHONK, SPECIES_CLEFFA},
        [1] = {SPECIES_SENTRET, SPECIES_LECHONK, SPECIES_FLABEBE_WHITE, SPECIES_URSALUNA_BLOODMOON, SPECIES_IGGLYBUFF, SPECIES_SPRITZEE, SPECIES_DUDUNSPARCE_THREE, SPECIES_MAUSHOLD_FOUR, SPECIES_PIDGEY, SPECIES_FLETCHLING},
        [2] = {SPECIES_TERAPAGOS_TERASTAL, SPECIES_OINKOLOGNE, SPECIES_BIDOOF, SPECIES_TINKATON, SPECIES_LILLIPUP, SPECIES_FLABEBE_BLUE, SPECIES_IMPIDIMP, SPECIES_MARILL, SPECIES_HELIOPTILE, SPECIES_FLABEBE_ORANGE},
        [3] = {SPECIES_MORELULL, SPECIES_SMEARGLE, SPECIES_CHATOT, SPECIES_TEDDIURSA, SPECIES_RATICATE_A, SPECIES_MAGEARNA_P, SPECIES_MAWILE, SPECIES_CARBINK, SPECIES_TOGETIC, SPECIES_KIRLIA},
        [4] = {SPECIES_CARBINK, SPECIES_GLAMEOW, SPECIES_LITLEO, SPECIES_SPINDA, SPECIES_MR_MIME, SPECIES_WIGGLYTUFF, SPECIES_FLOETTE, SPECIES_SLAKOTH, SPECIES_STUFFUL, SPECIES_RATICATE},
        [5] = {SPECIES_LITLEO, SPECIES_TEDDIURSA, SPECIES_JIGGLYPUFF, SPECIES_VIGOROTH, SPECIES_WATCHOG, SPECIES_STUFFUL, SPECIES_TOGETIC, SPECIES_TRANQUILL, SPECIES_TYPE_NULL, SPECIES_MORGREM},
        [6] = {SPECIES_LICKILICKY, SPECIES_ALCREMIE_STAR, SPECIES_CHANSEY, SPECIES_BELLOSSOM, SPECIES_GREEDENT, SPECIES_FURFROU_PHAROAH, SPECIES_ALTARIA_MEGA, SPECIES_FLORGES_YELLOW, SPECIES_PYROAR_FEMALE, SPECIES_STANTLER},
        [7] = {SPECIES_FLORGES_YELLOW, SPECIES_FURFROU_STAR, SPECIES_CHANSEY, SPECIES_PURUGLY, SPECIES_INDEEDEE_FEMALE, SPECIES_TAUROS, SPECIES_FURFROU_HEART, SPECIES_FLORGES, SPECIES_INDEEDEE, SPECIES_FURFROU_LA_REINE},
        [8] = {SPECIES_ALTARIA_MEGA, SPECIES_DEDENNE, SPECIES_PIDGEOT, SPECIES_WEEZING_G, SPECIES_SAWSBUCK, SPECIES_FURFROU_DANDY, SPECIES_SAWSBUCK_SUMMER, SPECIES_FLORGES_BLUE, SPECIES_URSALUNA, SPECIES_HATTERENE},
        [9] = {SPECIES_TOGEKISS, SPECIES_FURFROU_LA_REINE, SPECIES_FURFROU_HEART, SPECIES_PIDGEOT, SPECIES_MELOETTA_PIROUETTE, SPECIES_COMFEY, SPECIES_CINCCINO, SPECIES_SAWSBUCK_SUMMER, SPECIES_ALCREMIE_GIGA, SPECIES_BLISSEY},
    },
    [CLASS_SWIMMER_F] = {
        [0] = {SPECIES_PALAFIN_HERO, SPECIES_QUAQUAVAL, SPECIES_SURSKIT, SPECIES_TATSUGIRI_RED, SPECIES_PIPLUP, SPECIES_VELUZA, SPECIES_SQUIRTLE, SPECIES_TATSUGIRI_YELLOW, SPECIES_DONDOZO, SPECIES_WALKING_WAKE},
        [1] = {SPECIES_PIPLUP, SPECIES_SHELLOS_EAST, SPECIES_DUCKLETT, SPECIES_OSHAWOTT, SPECIES_MUDKIP, SPECIES_SKRELP, SPECIES_QUAXLY, SPECIES_VELUZA, SPECIES_WALKING_WAKE, SPECIES_TATSUGIRI_YELLOW},
        [2] = {SPECIES_MUDKIP, SPECIES_PALAFIN, SPECIES_POPPLIO, SPECIES_ARROKUDA, SPECIES_SOBBLE, SPECIES_TOTODILE, SPECIES_DEWPIDER, SPECIES_WALKING_WAKE, SPECIES_FEEBAS, SPECIES_QUAXWELL},
        [3] = {SPECIES_CORSOLA, SPECIES_POLIWAG, SPECIES_SEEL, SPECIES_SLOWPOKE, SPECIES_QWILFISH, SPECIES_PRINPLUP, SPECIES_POLIWHIRL, SPECIES_BASCULEGION_F, SPECIES_BARBOACH, SPECIES_QUAGSIRE},
        [4] = {SPECIES_BIBAREL, SPECIES_WISHIWASHI, SPECIES_BARBOACH, SPECIES_CLAWITZER, SPECIES_SPHEAL, SPECIES_QWILFISH, SPECIES_CORPHISH, SPECIES_MAREANIE, SPECIES_DRIZZILE, SPECIES_ARAQUANID},
        [5] = {SPECIES_MANTYKE, SPECIES_BASCULIN_RED, SPECIES_REMORAID, SPECIES_STARYU, SPECIES_CHINCHOU, SPECIES_SLOWPOKE, SPECIES_KRABBY, SPECIES_MARSHTOMP, SPECIES_WAILMER, SPECIES_WISHIWASHI},
        [6] = {SPECIES_JELLICENT, SPECIES_CRAMORANT_GULPING, SPECIES_SEISMITOAD, SPECIES_CRAMORANT, SPECIES_GRENINJA, SPECIES_URSHIFU_RAPID_GIGA, SPECIES_WALREIN, SPECIES_ARCEUS_WATER, SPECIES_VAPOREON, SPECIES_MANTINE},
        [7] = {SPECIES_KYOGRE_PRIMAL, SPECIES_SIMIPOUR, SPECIES_KINGDRA, SPECIES_TENTACRUEL, SPECIES_PRIMARINA, SPECIES_SHARPEDO_MEGA, SPECIES_TAPU_FINI, SPECIES_CRAMORANT_GULPING, SPECIES_VAPOREON, SPECIES_GYARADOS_MEGA},
        [8] = {SPECIES_TAPU_FINI, SPECIES_BARRASKEWDA, SPECIES_SIMIPOUR, SPECIES_ASHGRENINJA, SPECIES_SEISMITOAD, SPECIES_BLASTOISE, SPECIES_KINGLER, SPECIES_FERALIGATR, SPECIES_TOXAPEX, SPECIES_AZUMARILL},
        [9] = {SPECIES_RELICANTH, SPECIES_VOLCANION, SPECIES_PRIMARINA, SPECIES_MILOTIC, SPECIES_DRACOVISH, SPECIES_OMASTAR, SPECIES_MANTINE, SPECIES_PHIONE, SPECIES_CRAMORANT_GULPING, SPECIES_AZUMARILL},
    },
    [CLASS_PSYCHIC] = {
        [0] = {SPECIES_UNOWN_L, SPECIES_IRON_CROWN, SPECIES_COSMOG, SPECIES_UNOWN_X, SPECIES_HATENNA, SPECIES_RABSCA, SPECIES_UNOWN_G, SPECIES_IRON_BOULDER, SPECIES_UNOWN_R, SPECIES_ARMAROUGE},
        [1] = {SPECIES_UNOWN_W, SPECIES_WYNAUT, SPECIES_IRON_BOULDER, SPECIES_UNOWN_I, SPECIES_IRON_CROWN, SPECIES_UNOWN_M, SPECIES_UNOWN_Z, SPECIES_UNOWN_QUESTION, SPECIES_ESPATHRA, SPECIES_VELUZA},
        [2] = {SPECIES_UNOWN_QUESTION, SPECIES_IRON_BOULDER, SPECIES_UNOWN_G, SPECIES_UNOWN_B, SPECIES_FARIGIRAF, SPECIES_GOTHITA, SPECIES_UNOWN_D, SPECIES_COSMOG, SPECIES_UNOWN_N, SPECIES_UNOWN_P},
        [3] = {SPECIES_GOTHORITA, SPECIES_PIKACHU_PHD, SPECIES_MUNNA, SPECIES_SWOOBAT, SPECIES_MR_MIME, SPECIES_ABRA, SPECIES_DOTTLER, SPECIES_SPOINK, SPECIES_RAICHU_A, SPECIES_SLOWPOKE_G},
        [4] = {SPECIES_CHIMECHO, SPECIES_COSMOEM, SPECIES_SMOOCHUM, SPECIES_BRONZOR, SPECIES_PIKACHU_PHD, SPECIES_DUOSION, SPECIES_NATU, SPECIES_WOOBAT, SPECIES_HATTREM, SPECIES_PSYDUCK},
        [5] = {SPECIES_DUOSION, SPECIES_ESPURR, SPECIES_PIKACHU_PHD, SPECIES_MEDITITE, SPECIES_GIRAFARIG, SPECIES_EXEGGCUTE, SPECIES_ABRA, SPECIES_KADABRA, SPECIES_BELDUM, SPECIES_COSMOEM},
        [6] = {SPECIES_SLOWKING_G, SPECIES_NECROZMA_DUSK_MANE, SPECIES_GRUMPIG, SPECIES_NOCTOWL, SPECIES_LUNALA, SPECIES_GOLDUCK, SPECIES_XATU, SPECIES_LUGIA, SPECIES_SLOWKING, SPECIES_BEHEEYEM},
        [7] = {SPECIES_SLOWBRO, SPECIES_NECROZMA_DAWN_WINGS, SPECIES_MEWTWO, SPECIES_MUSHARNA, SPECIES_GARDEVOIR, SPECIES_MEDICHAM, SPECIES_WOBBUFFET, SPECIES_SLOWBRO_MEGA, SPECIES_NECROZMA_DUSK_MANE, SPECIES_LATIOS_MEGA},
        [8] = {SPECIES_DEOXYS_ATTACK, SPECIES_MEWTWO_MEGA_Y, SPECIES_MUSHARNA, SPECIES_HATTERENE_GIGA, SPECIES_RAPIDASH_G, SPECIES_CRESSELIA, SPECIES_MEWTWO_MEGA_X, SPECIES_SOLROCK, SPECIES_STARMIE, SPECIES_SOLGALEO},
        [9] = {SPECIES_VICTINI, SPECIES_LUNATONE, SPECIES_GALLADE, SPECIES_JIRACHI, SPECIES_CRESSELIA, SPECIES_MESPRIT, SPECIES_NECROZMA_ULTRA, SPECIES_MEWTWO_MEGA_X, SPECIES_DEOXYS_DEFENSE, SPECIES_HOOPA},
    },
    [CLASS_ROCKER] = {
        [0] = {SPECIES_PICHU_SPIKY, SPECIES_TOXEL, SPECIES_PAWMOT, SPECIES_HELIOPTILE, SPECIES_BELLIBOLT, SPECIES_PAWMI, SPECIES_SHINX, SPECIES_TYNAMO, SPECIES_TADBULB, SPECIES_IRON_HANDS},
        [1] = {SPECIES_PAWMI, SPECIES_BELLIBOLT, SPECIES_JOLTIK, SPECIES_HELIOPTILE, SPECIES_IRON_THORNS, SPECIES_TYNAMO, SPECIES_IRON_HANDS, SPECIES_PAWMOT, SPECIES_MAREEP, SPECIES_TOXEL},
        [2] = {SPECIES_PAWMOT, SPECIES_SHINX, SPECIES_PICHU, SPECIES_IRON_THORNS, SPECIES_PAWMO, SPECIES_BELLIBOLT, SPECIES_HELIOPTILE, SPECIES_YAMPER, SPECIES_KILOWATTREL, SPECIES_TADBULB},
        [3] = {SPECIES_LUXIO, SPECIES_PIKACHU_SURFING, SPECIES_ROTOM_FROST, SPECIES_PIKACHU_PHD, SPECIES_PIKACHU_ROCK_STAR, SPECIES_ROTOM_FAN, SPECIES_RAICHU_A, SPECIES_GEODUDE_A, SPECIES_BLITZLE, SPECIES_PLUSLE},
        [4] = {SPECIES_MAGNEMITE, SPECIES_ELECTRIKE, SPECIES_PIKACHU_CAP_SINNOH, SPECIES_GEODUDE_A, SPECIES_ROTOM_WASH, SPECIES_PIKACHU_CAP_ORIGINAL, SPECIES_PIKACHU_CAP_UNOVA, SPECIES_PIKACHU_LIBRE, SPECIES_VOLTORB_H, SPECIES_ELEKID},
        [5] = {SPECIES_GEODUDE_A, SPECIES_PIKACHU_CAP_KALOS, SPECIES_PIKACHU_BELLE, SPECIES_SILVALLY_ELECTRIC, SPECIES_PIKACHU, SPECIES_GRAVELER_A, SPECIES_PIKACHU_CAP_ORIGINAL, SPECIES_RAICHU_A, SPECIES_ROTOM_HEAT, SPECIES_PIKACHU_FLYING},
        [6] = {SPECIES_STUNFISK, SPECIES_MANECTRIC, SPECIES_PINCURCHIN, SPECIES_THUNDURUS, SPECIES_RAIKOU, SPECIES_ELECTABUZZ, SPECIES_EELEKTROSS, SPECIES_ZERAORA, SPECIES_MORPEKO_HANGRY, SPECIES_GALVANTULA},
        [7] = {SPECIES_MAGNETON, SPECIES_DRACOZOLT, SPECIES_MORPEKO_HANGRY, SPECIES_TOXTRICITY, SPECIES_LANTURN, SPECIES_EMOLGA, SPECIES_BOLTUND, SPECIES_JOLTEON, SPECIES_STUNFISK, SPECIES_PINCURCHIN},
        [8] = {SPECIES_AMPHAROS, SPECIES_EMOLGA, SPECIES_AMPHAROS_MEGA, SPECIES_TOXTRICITY, SPECIES_MORPEKO_HANGRY, SPECIES_ELECTIVIRE, SPECIES_ORICORIO_Y, SPECIES_TOXTRICITY_GIGA, SPECIES_ELECTABUZZ, SPECIES_REGIELEKI},
        [9] = {SPECIES_XURKITREE, SPECIES_MORPEKO_HANGRY, SPECIES_ZERAORA, SPECIES_TOXTRICITY, SPECIES_MAGNETON, SPECIES_ARCTOZOLT, SPECIES_VIKAVOLT, SPECIES_TAPU_KOKO, SPECIES_PINCURCHIN, SPECIES_ZAPDOS},
    },
    [CLASS_JUGGLER] = {
        [0] = {SPECIES_BALTOY, SPECIES_FLUTTER_MANE, SPECIES_ELGYEM, SPECIES_PUMPKABOO_XL, SPECIES_ARMAROUGE, SPECIES_WYNAUT, SPECIES_GIMMIGHOUL, SPECIES_UNOWN_Y, SPECIES_UNOWN_J, SPECIES_POLTCHAGEIST},
        [1] = {SPECIES_ANNIHILAPE, SPECIES_FRILLISH_F, SPECIES_UNOWN_Y, SPECIES_ZORUA_H, SPECIES_RABSCA, SPECIES_SINISTEA_CHIPPED, SPECIES_HATENNA, SPECIES_PUMPKABOO_M, SPECIES_SINISTCHA, SPECIES_UNOWN_X},
        [2] = {SPECIES_DREEPY, SPECIES_SKELEDIRGE, SPECIES_PUMPKABOO_M, SPECIES_BRAMBLIN, SPECIES_UNOWN_U, SPECIES_UNOWN_F, SPECIES_GOLETT, SPECIES_GHOLDENGO, SPECIES_INKAY, SPECIES_SINISTEA},
        [3] = {SPECIES_NATU, SPECIES_SANDYGAST, SPECIES_COSMOEM, SPECIES_GIRAFARIG, SPECIES_MISDREAVUS, SPECIES_LAMPENT, SPECIES_WOOBAT, SPECIES_MUNNA, SPECIES_SLOWPOKE, SPECIES_SMOOCHUM},
        [4] = {SPECIES_MR_MIME_G, SPECIES_ESPURR, SPECIES_PSYDUCK, SPECIES_SILVALLY_GHOST, SPECIES_SABLEYE, SPECIES_CHIMECHO, SPECIES_DRIFLOON, SPECIES_RALTS, SPECIES_HATTREM, SPECIES_SHEDINJA},
        [5] = {SPECIES_CHIMECHO, SPECIES_GASTLY, SPECIES_KIRLIA, SPECIES_MISDREAVUS, SPECIES_MIME_JR_G, SPECIES_EXEGGCUTE_A, SPECIES_GOTHORITA, SPECIES_DUOSION, SPECIES_SPOINK, SPECIES_SHEDINJA},
        [6] = {SPECIES_MELOETTA, SPECIES_NOCTOWL, SPECIES_CALYREX_SHADOW_RIDER, SPECIES_AEGISLASH, SPECIES_HYPNO, SPECIES_JELLICENT_F, SPECIES_GIRATINA, SPECIES_RUNERIGUS, SPECIES_INDEEDEE_FEMALE, SPECIES_BRONZONG},
        [7] = {SPECIES_NECROZMA_ULTRA, SPECIES_GENGAR_GIGA, SPECIES_DHELMISE, SPECIES_LUGIA, SPECIES_MIMIKYU, SPECIES_CLAYDOL, SPECIES_VICTINI, SPECIES_BRUXISH, SPECIES_AEGISLASH_BLADE, SPECIES_ORBEETLE_GIGA},
        [8] = {SPECIES_HOOPA, SPECIES_CRESSELIA, SPECIES_DRAGAPULT, SPECIES_MALAMAR, SPECIES_MUSHARNA, SPECIES_TREVENANT, SPECIES_BLACEPHALON, SPECIES_ORANGURU, SPECIES_GOLDUCK, SPECIES_GOURGEIST_L},
        [9] = {SPECIES_GALLADE_MEGA, SPECIES_ESPEON, SPECIES_MELOETTA, SPECIES_HOOPA_UNBOUND, SPECIES_GIRATINA, SPECIES_EXEGGUTOR, SPECIES_MISMAGIUS, SPECIES_SLOWBRO_MEGA, SPECIES_MUSHARNA, SPECIES_MEWTWO},
    },
    [CLASS_TAMER] = {
        [0] = {SPECIES_WHISMUR, SPECIES_GRAFAIAI, SPECIES_GOOMY, SPECIES_HELIOPTILE, SPECIES_URSALUNA_BLOODMOON, SPECIES_TANDEMAUS, SPECIES_TERAPAGOS_TERASTAL, SPECIES_DOLLIV, SPECIES_WALKING_WAKE, SPECIES_AZURILL},
        [1] = {SPECIES_TATSUGIRI, SPECIES_SMOLIV, SPECIES_APPLIN, SPECIES_TANDEMAUS, SPECIES_GRAFAIAI, SPECIES_ARCTIBAX, SPECIES_TERAPAGOS, SPECIES_BUNNELBY, SPECIES_FLETCHLING, SPECIES_DREEPY},
        [2] = {SPECIES_DRATINI, SPECIES_APPLIN, SPECIES_NOIBAT, SPECIES_YUNGOOS, SPECIES_KORAIDON, SPECIES_ARCHALUDON, SPECIES_PATRAT, SPECIES_FRIGIBAX, SPECIES_LILLIPUP, SPECIES_MAUSHOLD_FOUR},
        [3] = {SPECIES_DODUO, SPECIES_WATCHOG, SPECIES_TYPE_NULL, SPECIES_RATICATE, SPECIES_STUFFUL, SPECIES_HAKAMO_O, SPECIES_DELCATTY, SPECIES_RUFFLET, SPECIES_AIPOM, SPECIES_BIBAREL},
        [4] = {SPECIES_GLAMEOW, SPECIES_PERSIAN, SPECIES_CASTFORM, SPECIES_LINOONE, SPECIES_SILVALLY_DRAGON, SPECIES_SHELGON, SPECIES_KECLEON, SPECIES_SPINDA, SPECIES_DRAGONAIR, SPECIES_DRAKLOAK},
        [5] = {SPECIES_LOUDRED, SPECIES_BUNEARY, SPECIES_TYPE_NULL, SPECIES_SMEARGLE, SPECIES_BAGON, SPECIES_RATICATE, SPECIES_SPINDA, SPECIES_GUMSHOOS, SPECIES_JANGMO_O, SPECIES_VIBRAVA},
        [6] = {SPECIES_UNFEZANT_F, SPECIES_ETERNATUS, SPECIES_FURFROU_DEBUTANTE, SPECIES_PORYGON_Z, SPECIES_PURUGLY, SPECIES_OBSTAGOON, SPECIES_SLIGGOO_H, SPECIES_SCEPTILE_MEGA, SPECIES_FURFROU_DIAMOND, SPECIES_URSARING},
        [7] = {SPECIES_BRAVIARY, SPECIES_MELOETTA, SPECIES_NECROZMA_ULTRA, SPECIES_ZOROARK_H, SPECIES_ZANGOOSE, SPECIES_FURFROU_LA_REINE, SPECIES_SALAMENCE, SPECIES_ETERNATUS_ETERNAMAX, SPECIES_PORYGON_Z, SPECIES_GOODRA},
        [8] = {SPECIES_SLIGGOO, SPECIES_RESHIRAM, SPECIES_REGIGIGAS, SPECIES_PYROAR, SPECIES_REGIDRAGO, SPECIES_SILVALLY, SPECIES_LATIAS, SPECIES_PORYGON2, SPECIES_TAUROS, SPECIES_DRAGONITE},
        [9] = {SPECIES_RAYQUAZA, SPECIES_KYUREM, SPECIES_URSALUNA, SPECIES_LATIAS_MEGA, SPECIES_FURFROU_STAR, SPECIES_FURFROU, SPECIES_BOUFFALANT, SPECIES_SWELLOW, SPECIES_DRAGALGE, SPECIES_DURALUDON},
    },
    [CLASS_BIRD_KEEPER] = {
        [0] = {SPECIES_KILOWATTREL, SPECIES_WINGULL, SPECIES_FLAMIGO, SPECIES_NOIBAT, SPECIES_BOMBIRDIER, SPECIES_SQUAWKABILLY_WHITE, SPECIES_SPEAROW, SPECIES_PIDGEY, SPECIES_TAILLOW, SPECIES_FLETCHLING},
        [1] = {SPECIES_ZUBAT, SPECIES_FLETCHLING, SPECIES_WINGULL, SPECIES_KILOWATTREL, SPECIES_SQUAWKABILLY_YELLOW, SPECIES_SQUAWKABILLY, SPECIES_BOMBIRDIER, SPECIES_STARLY, SPECIES_TAILLOW, SPECIES_HOOTHOOT},
        [2] = {SPECIES_FLAMIGO, SPECIES_PIDGEY, SPECIES_HOOTHOOT, SPECIES_ZUBAT, SPECIES_ROWLET, SPECIES_DUCKLETT, SPECIES_PIKIPEK, SPECIES_WINGULL, SPECIES_NOIBAT, SPECIES_SPEAROW},
        [3] = {SPECIES_DRIFLOON, SPECIES_PIDGEOTTO, SPECIES_MASQUERAIN, SPECIES_MINIOR_YELLOW, SPECIES_TRANQUILL, SPECIES_PIKACHU_FLYING, SPECIES_MINIOR_SHIELD, SPECIES_LEDIAN, SPECIES_NATU, SPECIES_FLETCHINDER},
        [4] = {SPECIES_VULLABY, SPECIES_MINIOR_GREEN, SPECIES_GLIGAR, SPECIES_RUFFLET, SPECIES_LEDIAN, SPECIES_DARTRIX, SPECIES_SWABLU, SPECIES_CHATOT, SPECIES_CORVISQUIRE, SPECIES_YANMA},
        [5] = {SPECIES_CORVISQUIRE, SPECIES_DRIFLOON, SPECIES_GLIGAR, SPECIES_CHATOT, SPECIES_MINIOR_VIOLET, SPECIES_SWABLU, SPECIES_YANMA, SPECIES_SKIPLOOM, SPECIES_DODUO, SPECIES_TRUMBEAK},
        [6] = {SPECIES_ENAMORUS_THERIAN, SPECIES_MOLTRES_G, SPECIES_ENAMORUS, SPECIES_FEAROW, SPECIES_CELESTEELA, SPECIES_YANMEGA, SPECIES_ARCEUS_FLYING, SPECIES_MANTINE, SPECIES_VIVILLON_TUNDRA, SPECIES_VIVILLON_ARCHIPELAGO},
        [7] = {SPECIES_PIDGEOT, SPECIES_GLISCOR, SPECIES_VIVILLON_OCEAN, SPECIES_ALTARIA, SPECIES_LANDORUS, SPECIES_CRAMORANT_GULPING, SPECIES_SHAYMIN_SKY, SPECIES_MANDIBUZZ, SPECIES_ORICORIO_P, SPECIES_VIVILLON_SANDSTORM},
        [8] = {SPECIES_PELIPPER, SPECIES_UNFEZANT, SPECIES_ARTICUNO_G, SPECIES_TROPIUS, SPECIES_HO_OH, SPECIES_VIVILLON_MONSOON, SPECIES_TORNADUS_THERIAN, SPECIES_SALAMENCE, SPECIES_ALTARIA, SPECIES_NOIVERN},
        [9] = {SPECIES_DRAGONITE, SPECIES_ORICORIO_S, SPECIES_SIGILYPH, SPECIES_CRAMORANT_GORGING, SPECIES_AERODACTYL, SPECIES_DRIFBLIM, SPECIES_VIVILLON_FANCY, SPECIES_MANTINE, SPECIES_VIVILLON_HIGH_PLAINS, SPECIES_BUTTERFREE},
    },
    [CLASS_BLACK_BELT] = {
        [0] = {SPECIES_KORAIDON, SPECIES_OKIDOGI, SPECIES_FLAMIGO, SPECIES_IRON_HANDS, SPECIES_GREAT_TUSK, SPECIES_TAUROS_BLAZE_P, SPECIES_TAUROS_P, SPECIES_SLITHER_WING, SPECIES_PAWMOT, SPECIES_ANNIHILAPE},
        [1] = {SPECIES_PAWMOT, SPECIES_TAUROS_P, SPECIES_IRON_HANDS, SPECIES_TAUROS_BLAZE_P, SPECIES_GREAT_TUSK, SPECIES_QUAQUAVAL, SPECIES_ANNIHILAPE, SPECIES_PAWMO, SPECIES_IRON_VALIANT, SPECIES_SLITHER_WING},
        [2] = {SPECIES_OKIDOGI, SPECIES_PAWMOT, SPECIES_CLOBBOPUS, SPECIES_QUAQUAVAL, SPECIES_SLITHER_WING, SPECIES_KUBFU, SPECIES_TAUROS_AQUA_P, SPECIES_TAUROS_P, SPECIES_IRON_VALIANT, SPECIES_IRON_HANDS},
        [3] = {SPECIES_HITMONTOP, SPECIES_MANKEY, SPECIES_STUFFUL, SPECIES_PIKACHU_LIBRE, SPECIES_COMBUSKEN, SPECIES_MAKUHITA, SPECIES_FARFETCHD_G, SPECIES_HITMONCHAN, SPECIES_MONFERNO, SPECIES_HAKAMO_O},
        [4] = {SPECIES_PANCHAM, SPECIES_RIOLU, SPECIES_MONFERNO, SPECIES_LILLIGANT_H, SPECIES_MACHOP, SPECIES_SCRAGGY, SPECIES_HITMONTOP, SPECIES_PIGNITE, SPECIES_CRABRAWLER, SPECIES_COMBUSKEN},
        [5] = {SPECIES_MACHOKE, SPECIES_HITMONLEE, SPECIES_PIGNITE, SPECIES_FARFETCHD_G, SPECIES_MONFERNO, SPECIES_MEDITITE, SPECIES_SNEASEL_H, SPECIES_HAKAMO_O, SPECIES_GURDURR, SPECIES_HITMONCHAN},
        [6] = {SPECIES_ZAPDOS_G, SPECIES_MARSHADOW, SPECIES_BRELOOM, SPECIES_INFERNAPE, SPECIES_MEDICHAM, SPECIES_KELDEO_RESOLUTE, SPECIES_FALINKS, SPECIES_POLIWRATH, SPECIES_GRAPPLOCT, SPECIES_DECIDUEYE_H},
        [7] = {SPECIES_MIENSHAO, SPECIES_TOXICROAK, SPECIES_SCRAFTY, SPECIES_KELDEO, SPECIES_PASSIMIAN, SPECIES_BEARTIC, SPECIES_SAMUROTT, SPECIES_PANGORO, SPECIES_ZAMAZENTA_CROWNED, SPECIES_LOPUNNY_MEGA},
        [8] = {SPECIES_SCRAFTY, SPECIES_HERACROSS_MEGA, SPECIES_KOMMO_O, SPECIES_PASSIMIAN, SPECIES_URSHIFU_SINGLE_GIGA, SPECIES_MIENSHAO, SPECIES_LUCARIO_MEGA, SPECIES_HAWLUCHA, SPECIES_BEWEAR, SPECIES_SAWK},
        [9] = {SPECIES_SAMUROTT, SPECIES_MEWTWO_MEGA_X, SPECIES_BLAZIKEN_MEGA, SPECIES_VIRIZION, SPECIES_URSHIFU_RAPID, SPECIES_BUZZWOLE, SPECIES_GALLADE, SPECIES_CRABOMINABLE, SPECIES_GALLADE_MEGA, SPECIES_PASSIMIAN},
    },
    [CLASS_RIVAL] = {
        [0] = {SPECIES_ZORUA_H, SPECIES_IGGLYBUFF, SPECIES_DUDUNSPARCE, SPECIES_SQUAWKABILLY, SPECIES_DOLLIV, SPECIES_HELIOPTILE, SPECIES_MINCCINO, SPECIES_SQUAWKABILLY_BLUE, SPECIES_MAUSHOLD_FOUR, SPECIES_LILLIPUP},
        [1] = {SPECIES_DEERLING_AUTUMN, SPECIES_SQUAWKABILLY_YELLOW, SPECIES_SKWOVET, SPECIES_HELIOPTILE, SPECIES_BUNNELBY, SPECIES_STARLY, SPECIES_DEERLING_WINTER, SPECIES_SQUAWKABILLY_WHITE, SPECIES_TERAPAGOS, SPECIES_SENTRET},
        [2] = {SPECIES_DEERLING, SPECIES_TERAPAGOS_TERASTAL, SPECIES_DITTO, SPECIES_URSALUNA_BLOODMOON, SPECIES_TAILLOW, SPECIES_ARBOLIVA, SPECIES_PIDGEY, SPECIES_IGGLYBUFF, SPECIES_MINCCINO, SPECIES_EGG},
        [3] = {SPECIES_SPINDA, SPECIES_BIBAREL, SPECIES_GLAMEOW, SPECIES_PORYGON, SPECIES_RUFFLET, SPECIES_AIPOM, SPECIES_SMEARGLE, SPECIES_PIDGEOTTO, SPECIES_SLAKOTH, SPECIES_EEVEE},
        [4] = {SPECIES_WOOLOO, SPECIES_RATICATE_A, SPECIES_LICKITUNG, SPECIES_VIGOROTH, SPECIES_RUFFLET, SPECIES_KECLEON, SPECIES_HERDIER, SPECIES_GIRAFARIG, SPECIES_GUMSHOOS, SPECIES_PIDGEOTTO},
        [5] = {SPECIES_WATCHOG, SPECIES_STUFFUL, SPECIES_CHATOT, SPECIES_TRUMBEAK, SPECIES_TRANQUILL, SPECIES_CASTFORM, SPECIES_BIBAREL, SPECIES_WOOLOO, SPECIES_RUFFLET, SPECIES_TEDDIURSA},
        [6] = {SPECIES_LICKILICKY, SPECIES_ZANGOOSE, SPECIES_MELOETTA, SPECIES_PURUGLY, SPECIES_MELOETTA_PIROUETTE, SPECIES_FURFROU_MATRON, SPECIES_OBSTAGOON, SPECIES_FURFROU_HEART, SPECIES_FEAROW, SPECIES_PYROAR_FEMALE},
        [7] = {SPECIES_MILTANK, SPECIES_DUBWOOL, SPECIES_STARAPTOR, SPECIES_FURFROU_HEART, SPECIES_SILVALLY, SPECIES_FURFROU, SPECIES_KANGASKHAN_MEGA, SPECIES_SNORLAX_GIGA, SPECIES_SNORLAX, SPECIES_LICKILICKY},
        [8] = {SPECIES_ZANGOOSE, SPECIES_CHANSEY, SPECIES_MELOETTA_PIROUETTE, SPECIES_CINCCINO, SPECIES_OBSTAGOON, SPECIES_ORANGURU, SPECIES_SILVALLY, SPECIES_BEWEAR, SPECIES_SWELLOW, SPECIES_LOPUNNY},
        [9] = {SPECIES_BEWEAR, SPECIES_KANGASKHAN, SPECIES_SNORLAX, SPECIES_KOMALA, SPECIES_TAUROS, SPECIES_TOUCANNON, SPECIES_FURFROU_STAR, SPECIES_STANTLER, SPECIES_LICKILICKY, SPECIES_HAPPINY},
    },
    [CLASS_SCIENTIST] = {
        [0] = {SPECIES_SPEAROW, SPECIES_DITTO, SPECIES_EGG, SPECIES_MINCCINO, SPECIES_TERAPAGOS_TERASTAL, SPECIES_PIDOVE, SPECIES_PIDGEY, SPECIES_LECHONK, SPECIES_SENTRET, SPECIES_GRAFAIAI},
        [1] = {SPECIES_ZIGZAGOON_G, SPECIES_MEOWTH_GIGA, SPECIES_DEERLING_AUTUMN, SPECIES_DITTO, SPECIES_URSALUNA_BLOODMOON, SPECIES_SMOLIV, SPECIES_SPEAROW, SPECIES_TAILLOW, SPECIES_TANDEMAUS, SPECIES_PATRAT},
        [2] = {SPECIES_ZIGZAGOON, SPECIES_DEERLING_AUTUMN, SPECIES_AZURILL, SPECIES_TANDEMAUS, SPECIES_DEERLING_WINTER, SPECIES_LILLIPUP, SPECIES_MEOWTH, SPECIES_WHISMUR, SPECIES_MEOWTH_GIGA, SPECIES_RATTATA},
        [3] = {SPECIES_KECLEON, SPECIES_EEVEE, SPECIES_LITLEO, SPECIES_GUMSHOOS, SPECIES_GLAMEOW, SPECIES_SMEARGLE, SPECIES_JIGGLYPUFF, SPECIES_SLAKOTH, SPECIES_RATICATE_A, SPECIES_LINOONE},
        [4] = {SPECIES_SMEARGLE, SPECIES_LOUDRED, SPECIES_BIBAREL, SPECIES_BUNEARY, SPECIES_JIGGLYPUFF, SPECIES_SLAKOTH, SPECIES_WIGGLYTUFF, SPECIES_FARFETCHD, SPECIES_FURRET, SPECIES_SWABLU},
        [5] = {SPECIES_SLAKOTH, SPECIES_LINOONE, SPECIES_SWABLU, SPECIES_PIDGEOTTO, SPECIES_LINOONE_G, SPECIES_PERSIAN, SPECIES_EEVEE, SPECIES_JIGGLYPUFF, SPECIES_EEVEE_GIGA, SPECIES_GLAMEOW},
        [6] = {SPECIES_CHANSEY, SPECIES_BOUFFALANT, SPECIES_HELIOLISK, SPECIES_EXPLOUD, SPECIES_CINCCINO, SPECIES_SILVALLY, SPECIES_LOPUNNY_MEGA, SPECIES_ZANGOOSE, SPECIES_KOMALA, SPECIES_STANTLER},
        [7] = {SPECIES_LICKILICKY, SPECIES_URSARING, SPECIES_AUDINO_MEGA, SPECIES_DODRIO, SPECIES_PIDGEOT, SPECIES_SWELLOW, SPECIES_FURFROU, SPECIES_SAWSBUCK_AUTUMN, SPECIES_GREEDENT, SPECIES_INDEEDEE},
        [8] = {SPECIES_PIDGEOT, SPECIES_OBSTAGOON, SPECIES_FURFROU_DEBUTANTE, SPECIES_UNFEZANT_F, SPECIES_PIDGEOT_MEGA, SPECIES_CHANSEY, SPECIES_BOUFFALANT, SPECIES_EXPLOUD, SPECIES_AUDINO, SPECIES_ZANGOOSE},
        [9] = {SPECIES_MILTANK, SPECIES_DUBWOOL, SPECIES_KOMALA, SPECIES_STARAPTOR, SPECIES_SNORLAX, SPECIES_EXPLOUD, SPECIES_PYROAR, SPECIES_AUDINO_MEGA, SPECIES_PIDGEOT_MEGA, SPECIES_FURFROU_STAR},
    },
    [CLASS_BOSS] = {
        [0] = {SPECIES_MAUSHOLD_FOUR, SPECIES_ZORUA_H, SPECIES_BIDOOF, SPECIES_DEERLING_AUTUMN, SPECIES_TERAPAGOS_STELLAR, SPECIES_SQUAWKABILLY_YELLOW, SPECIES_FLETCHLING, SPECIES_URSALUNA_BLOODMOON, SPECIES_TANDEMAUS, SPECIES_SKITTY},
        [1] = {SPECIES_ARBOLIVA, SPECIES_MAUSHOLD, SPECIES_MEOWTH, SPECIES_DUDUNSPARCE_THREE, SPECIES_SKITTY, SPECIES_ZIGZAGOON_G, SPECIES_LILLIPUP, SPECIES_DOLLIV, SPECIES_TAILLOW, SPECIES_ZIGZAGOON},
        [2] = {SPECIES_ZIGZAGOON, SPECIES_SENTRET, SPECIES_GRAFAIAI, SPECIES_AZURILL, SPECIES_SQUAWKABILLY, SPECIES_SKITTY, SPECIES_PATRAT, SPECIES_FLETCHLING, SPECIES_DEERLING, SPECIES_TERAPAGOS_STELLAR},
        [3] = {SPECIES_TEDDIURSA, SPECIES_VIGOROTH, SPECIES_DELCATTY, SPECIES_RATICATE_A, SPECIES_STUFFUL, SPECIES_LOUDRED, SPECIES_BIBAREL, SPECIES_PERSIAN, SPECIES_PIDGEOTTO, SPECIES_DIGGERSBY},
        [4] = {SPECIES_PIDGEOTTO, SPECIES_KECLEON, SPECIES_LINOONE, SPECIES_WIGGLYTUFF, SPECIES_SLAKOTH, SPECIES_PORYGON, SPECIES_WATCHOG, SPECIES_EEVEE, SPECIES_DUNSPARCE, SPECIES_DODUO},
        [5] = {SPECIES_TEDDIURSA, SPECIES_JIGGLYPUFF, SPECIES_EEVEE, SPECIES_LINOONE_G, SPECIES_SLAKOTH, SPECIES_TRANQUILL, SPECIES_FARFETCHD, SPECIES_LICKITUNG, SPECIES_DIGGERSBY, SPECIES_PERSIAN},
        [6] = {SPECIES_INDEEDEE, SPECIES_MELOETTA_PIROUETTE, SPECIES_SAWSBUCK, SPECIES_PORYGON2, SPECIES_URSALUNA, SPECIES_ZOROARK_H, SPECIES_KOMALA, SPECIES_PORYGON_Z, SPECIES_LOPUNNY_MEGA, SPECIES_ZANGOOSE},
        [7] = {SPECIES_STARAPTOR, SPECIES_PIDGEOT, SPECIES_FURFROU_PHAROAH, SPECIES_FURFROU, SPECIES_WYRDEER, SPECIES_BRAVIARY, SPECIES_PURUGLY, SPECIES_SAWSBUCK_AUTUMN, SPECIES_LOPUNNY, SPECIES_UNFEZANT_F},
        [8] = {SPECIES_STARAPTOR, SPECIES_SNORLAX_GIGA, SPECIES_SWELLOW, SPECIES_SAWSBUCK_AUTUMN, SPECIES_ARCEUS, SPECIES_ZANGOOSE, SPECIES_KANGASKHAN_MEGA, SPECIES_GREEDENT, SPECIES_FURFROU_PHAROAH, SPECIES_OBSTAGOON},
        [9] = {SPECIES_PYROAR_FEMALE, SPECIES_WYRDEER, SPECIES_INDEEDEE_FEMALE, SPECIES_TOUCANNON, SPECIES_MILTANK, SPECIES_SWELLOW, SPECIES_FURFROU_DEBUTANTE, SPECIES_FURFROU_LA_REINE, SPECIES_BOUFFALANT, SPECIES_CINCCINO},
    },
    [CLASS_LEADER] = {
        [0] = {SPECIES_SQUAWKABILLY_WHITE, SPECIES_DEERLING_AUTUMN, SPECIES_ARBOLIVA, SPECIES_IGGLYBUFF, SPECIES_PIKIPEK, SPECIES_SKITTY, SPECIES_SQUAWKABILLY, SPECIES_PIDGEY, SPECIES_SPEAROW, SPECIES_TANDEMAUS},
        [1] = {SPECIES_GRAFAIAI, SPECIES_SQUAWKABILLY_WHITE, SPECIES_BIDOOF, SPECIES_TAILLOW, SPECIES_ZORUA_H, SPECIES_DEERLING_WINTER, SPECIES_TERAPAGOS_STELLAR, SPECIES_HELIOPTILE, SPECIES_FLETCHLING, SPECIES_CYCLIZAR},
        [2] = {SPECIES_MINCCINO, SPECIES_ZIGZAGOON_G, SPECIES_SQUAWKABILLY, SPECIES_FARIGIRAF, SPECIES_DEERLING_SUMMER, SPECIES_PIKIPEK, SPECIES_WHISMUR, SPECIES_YUNGOOS, SPECIES_MAUSHOLD_FOUR, SPECIES_PIDOVE},
        [3] = {SPECIES_RUFFLET, SPECIES_TYPE_NULL, SPECIES_SWABLU, SPECIES_CASTFORM, SPECIES_MUNCHLAX, SPECIES_STUFFUL, SPECIES_KECLEON, SPECIES_CHATOT, SPECIES_SLAKOTH, SPECIES_SMEARGLE},
        [4] = {SPECIES_WIGGLYTUFF, SPECIES_PORYGON, SPECIES_KECLEON, SPECIES_STUFFUL, SPECIES_CHATOT, SPECIES_PIDGEOTTO, SPECIES_SMEARGLE, SPECIES_LITLEO, SPECIES_GLAMEOW, SPECIES_EEVEE},
        [5] = {SPECIES_WATCHOG, SPECIES_CHATOT, SPECIES_FURRET, SPECIES_GLAMEOW, SPECIES_TYPE_NULL, SPECIES_RATICATE, SPECIES_TEDDIURSA, SPECIES_LINOONE, SPECIES_AIPOM, SPECIES_LINOONE_G},
        [6] = {SPECIES_UNFEZANT_F, SPECIES_ZOROARK_H, SPECIES_GREEDENT, SPECIES_AUDINO, SPECIES_URSALUNA, SPECIES_MELOETTA_PIROUETTE, SPECIES_FURFROU_DIAMOND, SPECIES_TOUCANNON, SPECIES_FURFROU_KABUKI, SPECIES_PORYGON2},
        [7] = {SPECIES_SAWSBUCK_AUTUMN, SPECIES_PIDGEOT_MEGA, SPECIES_MELOETTA_PIROUETTE, SPECIES_UNFEZANT, SPECIES_FURFROU_LA_REINE, SPECIES_SNORLAX_GIGA, SPECIES_ORANGURU, SPECIES_DODRIO, SPECIES_OBSTAGOON, SPECIES_FURFROU_KABUKI},
        [8] = {SPECIES_SAWSBUCK_AUTUMN, SPECIES_BRAVIARY, SPECIES_WYRDEER, SPECIES_URSARING, SPECIES_TAUROS, SPECIES_ARCEUS, SPECIES_DODRIO, SPECIES_AMBIPOM, SPECIES_FURFROU_MATRON, SPECIES_SILVALLY},
        [9] = {SPECIES_FURFROU_MATRON, SPECIES_LOPUNNY_MEGA, SPECIES_SLAKING, SPECIES_BOUFFALANT, SPECIES_CHANSEY, SPECIES_INDEEDEE_FEMALE, SPECIES_KANGASKHAN, SPECIES_INDEEDEE, SPECIES_PURUGLY, SPECIES_URSALUNA},
    },
    [CLASS_TEAM_ROCKET] = {
        [0] = {SPECIES_FLETCHLING, SPECIES_MEOWTH, SPECIES_MAUSHOLD_FOUR, SPECIES_IGGLYBUFF, SPECIES_ZORUA_H, SPECIES_BIDOOF, SPECIES_MEOWTH_GIGA, SPECIES_TERAPAGOS, SPECIES_TAILLOW, SPECIES_TANDEMAUS},
        [1] = {SPECIES_PIDOVE, SPECIES_OINKOLOGNE_FEMALE, SPECIES_DEERLING_SUMMER, SPECIES_SMOLIV, SPECIES_STARLY, SPECIES_HOOTHOOT, SPECIES_YUNGOOS, SPECIES_CYCLIZAR, SPECIES_SQUAWKABILLY_WHITE, SPECIES_MINCCINO},
        [2] = {SPECIES_MEOWTH_GIGA, SPECIES_IGGLYBUFF, SPECIES_PIKIPEK, SPECIES_WHISMUR, SPECIES_TERAPAGOS_STELLAR, SPECIES_TERAPAGOS, SPECIES_SPEAROW, SPECIES_LECHONK, SPECIES_GRAFAIAI, SPECIES_DOLLIV},
        [3] = {SPECIES_CASTFORM, SPECIES_TRANQUILL, SPECIES_HERDIER, SPECIES_TRUMBEAK, SPECIES_RATICATE_A, SPECIES_RUFFLET, SPECIES_PIDGEOTTO, SPECIES_JIGGLYPUFF, SPECIES_GLAMEOW, SPECIES_KECLEON},
        [4] = {SPECIES_RUFFLET, SPECIES_LICKITUNG, SPECIES_PORYGON, SPECIES_EEVEE_GIGA, SPECIES_MUNCHLAX, SPECIES_GLAMEOW, SPECIES_CASTFORM, SPECIES_FARFETCHD, SPECIES_EEVEE, SPECIES_DELCATTY},
        [5] = {SPECIES_LINOONE_G, SPECIES_JIGGLYPUFF, SPECIES_GLAMEOW, SPECIES_PORYGON, SPECIES_PERSIAN, SPECIES_DUNSPARCE, SPECIES_DODUO, SPECIES_FURRET, SPECIES_RATICATE_A, SPECIES_GIRAFARIG},
        [6] = {SPECIES_SLAKING, SPECIES_AUDINO, SPECIES_ORANGURU, SPECIES_BEWEAR, SPECIES_TOUCANNON, SPECIES_INDEEDEE, SPECIES_UNFEZANT_F, SPECIES_SNORLAX_GIGA, SPECIES_MILTANK, SPECIES_KANGASKHAN},
        [7] = {SPECIES_SNORLAX, SPECIES_STANTLER, SPECIES_FURFROU_DIAMOND, SPECIES_GREEDENT, SPECIES_PYROAR_FEMALE, SPECIES_PURUGLY, SPECIES_EXPLOUD, SPECIES_DUBWOOL, SPECIES_KOMALA, SPECIES_LICKILICKY},
        [8] = {SPECIES_ARCEUS, SPECIES_DUBWOOL, SPECIES_BRAVIARY, SPECIES_LOPUNNY, SPECIES_BEWEAR, SPECIES_FURFROU_MATRON, SPECIES_SWELLOW, SPECIES_MELOETTA_PIROUETTE, SPECIES_INDEEDEE, SPECIES_PYROAR},
        [9] = {SPECIES_PIDGEOT_MEGA, SPECIES_AMBIPOM, SPECIES_SILVALLY, SPECIES_KANGASKHAN_MEGA, SPECIES_SWELLOW, SPECIES_LOPUNNY_MEGA, SPECIES_PORYGON2, SPECIES_KANGASKHAN, SPECIES_STANTLER, SPECIES_STOUTLAND},
    },
    [CLASS_COOLTRAINER] = {
        [0] = {SPECIES_IRON_THORNS, SPECIES_BALTOY, SPECIES_UNOWN_S, SPECIES_UNOWN_X, SPECIES_UNOWN_G, SPECIES_GOTHITA, SPECIES_MARILL, SPECIES_AXEW, SPECIES_UNOWN_M, SPECIES_UNOWN_F},
        [1] = {SPECIES_UNOWN_X, SPECIES_QUAXLY, SPECIES_PAWMI, SPECIES_PAWMO, SPECIES_DEWPIDER, SPECIES_HYDRAPPLE, SPECIES_QUAXWELL, SPECIES_APPLIN, SPECIES_PICHU, SPECIES_BINACLE},
        [2] = {SPECIES_IRON_BUNDLE, SPECIES_DIPPLIN, SPECIES_MIRAIDON, SPECIES_FRILLISH, SPECIES_IRON_THORNS, SPECIES_DREEPY, SPECIES_TOXEL, SPECIES_UNOWN_I, SPECIES_SOBBLE, SPECIES_VELUZA},
        [3] = {SPECIES_SEALEO, SPECIES_CLAMPERL, SPECIES_BLITZLE, SPECIES_VIBRAVA, SPECIES_MR_MIME_G, SPECIES_CHARJABUG, SPECIES_GEODUDE_A, SPECIES_SPOINK, SPECIES_COMBUSKEN, SPECIES_MR_MIME},
        [4] = {SPECIES_RAICHU_A, SPECIES_PIKACHU_CAP_UNOVA, SPECIES_DRIZZILE, SPECIES_GROWLITHE, SPECIES_STARYU, SPECIES_PIKACHU_FLYING, SPECIES_MINUN, SPECIES_MAROWAK_A, SPECIES_KADABRA, SPECIES_SLOWPOKE_G},
        [5] = {SPECIES_VIBRAVA, SPECIES_COMBUSKEN, SPECIES_PIKACHU_CAP_ALOLA, SPECIES_TENTACOOL, SPECIES_SILVALLY_DRAGON, SPECIES_BELDUM, SPECIES_CROCONAW, SPECIES_SPHEAL, SPECIES_FLAAFFY, SPECIES_PIKACHU_POP_STAR},
        [6] = {SPECIES_LUDICOLO, SPECIES_MELOETTA, SPECIES_DRAMPA, SPECIES_TOXAPEX, SPECIES_WYRDEER, SPECIES_HOOPA, SPECIES_PALKIA_ORIGIN, SPECIES_HOUNDOOM_MEGA, SPECIES_JIRACHI, SPECIES_EMBOAR},
        [7] = {SPECIES_MOLTRES, SPECIES_DRACOVISH, SPECIES_RESHIRAM, SPECIES_CENTISKORCH, SPECIES_ORBEETLE_GIGA, SPECIES_DARMANITAN_G_ZEN, SPECIES_ALAKAZAM, SPECIES_PHIONE, SPECIES_BRONZONG, SPECIES_CALYREX},
        [8] = {SPECIES_INDEEDEE, SPECIES_INTELEON_GIGA, SPECIES_GALLADE_MEGA, SPECIES_KYOGRE_PRIMAL, SPECIES_LUNALA, SPECIES_STUNFISK, SPECIES_TAPU_FINI, SPECIES_DRAMPA, SPECIES_SLOWKING_G, SPECIES_GRENINJA},
        [9] = {SPECIES_GOODRA, SPECIES_ALAKAZAM, SPECIES_APPLETUN, SPECIES_DIALGA_ORIGIN, SPECIES_LUMINEON, SPECIES_CRAMORANT, SPECIES_DEOXYS_ATTACK, SPECIES_GYARADOS_MEGA, SPECIES_ZYGARDE_COMPLETE, SPECIES_ZYGARDE_10},
    },
    [CLASS_ELITE_FOUR] = {
        [0] = {SPECIES_BRUTE_BONNET, SPECIES_APPLIN, SPECIES_MIRAIDON, SPECIES_IMPIDIMP, SPECIES_ROARING_MOON, SPECIES_WALKING_WAKE, SPECIES_PUMPKABOO_M, SPECIES_ELGYEM, SPECIES_ANNIHILAPE, SPECIES_UNOWN_N},
        [1] = {SPECIES_WYNAUT, SPECIES_TATSUGIRI, SPECIES_IRON_TREADS, SPECIES_CHI_YU, SPECIES_PUMPKABOO_L, SPECIES_KLINK, SPECIES_BAXCALIBUR, SPECIES_GOOMY, SPECIES_MEOWTH_G, SPECIES_VELUZA},
        [2] = {SPECIES_CHIEN_PAO, SPECIES_FRILLISH, SPECIES_SOLOSIS, SPECIES_KINGAMBIT, SPECIES_BAXCALIBUR, SPECIES_UNOWN_X, SPECIES_TING_LU, SPECIES_IRON_TREADS, SPECIES_TATSUGIRI, SPECIES_UNOWN_S},
        [3] = {SPECIES_SHIELDON, SPECIES_PIKACHU_PHD, SPECIES_MIME_JR_G, SPECIES_SILVALLY_PSYCHIC, SPECIES_EXEGGCUTE_A, SPECIES_MIGHTYENA, SPECIES_SLOWPOKE_G, SPECIES_STUNKY, SPECIES_MAGEARNA_P, SPECIES_BAGON},
        [4] = {SPECIES_MAGNEMITE, SPECIES_CHIMECHO, SPECIES_ARBOK, SPECIES_DRAGONAIR, SPECIES_SWOOBAT, SPECIES_SHIELDON, SPECIES_SILVALLY_STEEL, SPECIES_EXEGGCUTE_A, SPECIES_DRAKLOAK, SPECIES_MUNNA},
        [5] = {SPECIES_SLOWPOKE_G, SPECIES_SANDYGAST, SPECIES_SABLEYE, SPECIES_RALTS, SPECIES_JANGMO_O, SPECIES_RATICATE_A, SPECIES_MAWILE_MEGA, SPECIES_SMOOCHUM, SPECIES_SHEDINJA, SPECIES_MORGREM},
        [6] = {SPECIES_BANETTE_MEGA, SPECIES_MARSHADOW, SPECIES_DARKRAI, SPECIES_NOIVERN, SPECIES_SAMUROTT_H, SPECIES_LATIAS, SPECIES_DRAGAPULT, SPECIES_EXEGGUTOR_A, SPECIES_ZARUDE, SPECIES_TURTONATOR},
        [7] = {SPECIES_GENGAR_MEGA, SPECIES_MORPEKO_HANGRY, SPECIES_APPLETUN, SPECIES_WYRDEER, SPECIES_GOURGEIST_M, SPECIES_KLANG, SPECIES_SCRAFTY, SPECIES_POLTEAGEIST_CHIPPED, SPECIES_CALYREX_SHADOW_RIDER, SPECIES_COBALION},
        [8] = {SPECIES_BISHARP, SPECIES_URSHIFU_SINGLE, SPECIES_GENESECT_DOUSE, SPECIES_MANDIBUZZ, SPECIES_KYUREM_BLACK, SPECIES_UXIE, SPECIES_LUXRAY, SPECIES_NECROZMA_DAWN_WINGS, SPECIES_ZOROARK_H, SPECIES_SANDSLASH_A},
        [9] = {SPECIES_ETERNATUS, SPECIES_TYRANTRUM, SPECIES_MEDICHAM_MEGA, SPECIES_NECROZMA_DUSK_MANE, SPECIES_COBALION, SPECIES_SKARMORY, SPECIES_RUNERIGUS, SPECIES_KARTANA, SPECIES_DARKRAI, SPECIES_ORBEETLE_GIGA},
    },
    [CLASS_GENTLEMAN] = {
        [0] = {SPECIES_TERAPAGOS_STELLAR, SPECIES_STARLY, SPECIES_TINKATUFF, SPECIES_MINCCINO, SPECIES_VAROOM, SPECIES_FERROSEED, SPECIES_HELIOPTILE, SPECIES_BIDOOF, SPECIES_FLETCHLING, SPECIES_YUNGOOS},
        [1] = {SPECIES_SQUAWKABILLY_WHITE, SPECIES_PIDGEY, SPECIES_ORTHWORM, SPECIES_IGGLYBUFF, SPECIES_SKITTY, SPECIES_HELIOPTILE, SPECIES_TAILLOW, SPECIES_ZORUA_H, SPECIES_OINKOLOGNE, SPECIES_GRAFAIAI},
        [2] = {SPECIES_IRON_TREADS, SPECIES_TINKATUFF, SPECIES_DEERLING_WINTER, SPECIES_TINKATON, SPECIES_GHOLDENGO, SPECIES_SPEAROW, SPECIES_PIDOVE, SPECIES_FLETCHLING, SPECIES_URSALUNA_BLOODMOON, SPECIES_CUFANT},
        [3] = {SPECIES_FORRETRESS, SPECIES_LITLEO, SPECIES_BRONZOR, SPECIES_VIGOROTH, SPECIES_TYPE_NULL, SPECIES_DODUO, SPECIES_TEDDIURSA, SPECIES_DELCATTY, SPECIES_GLAMEOW, SPECIES_RATICATE_A},
        [4] = {SPECIES_EEVEE, SPECIES_PIDGEOTTO, SPECIES_BRONZOR, SPECIES_RATICATE, SPECIES_LICKITUNG, SPECIES_EEVEE_GIGA, SPECIES_LITLEO, SPECIES_MAGEARNA_P, SPECIES_STARAVIA, SPECIES_LINOONE_G},
        [5] = {SPECIES_WORMADAM_TRASH, SPECIES_SMEARGLE, SPECIES_WIGGLYTUFF, SPECIES_TYPE_NULL, SPECIES_STARAVIA, SPECIES_JIGGLYPUFF, SPECIES_DIGLETT_A, SPECIES_GIRAFARIG, SPECIES_HERDIER, SPECIES_MAGNEMITE},
        [6] = {SPECIES_STOUTLAND, SPECIES_AUDINO_MEGA, SPECIES_KOMALA, SPECIES_BRONZONG, SPECIES_DOUBLADE, SPECIES_AGGRON, SPECIES_NECROZMA_DUSK_MANE, SPECIES_HAPPINY, SPECIES_KLEFKI, SPECIES_KARTANA},
        [7] = {SPECIES_AUDINO, SPECIES_REGISTEEL, SPECIES_FURFROU_HEART, SPECIES_FURFROU_DANDY, SPECIES_AEGISLASH, SPECIES_SAWSBUCK_AUTUMN, SPECIES_DURALUDON, SPECIES_ORANGURU, SPECIES_LICKILICKY, SPECIES_METAGROSS_MEGA},
        [8] = {SPECIES_DURALUDON, SPECIES_FURFROU_KABUKI, SPECIES_GENESECT_CHILL, SPECIES_FURFROU_DANDY, SPECIES_SOLGALEO, SPECIES_PIDGEOT_MEGA, SPECIES_STOUTLAND, SPECIES_GENESECT_BURN, SPECIES_BRAVIARY, SPECIES_SLIGGOO_H},
        [9] = {SPECIES_UNFEZANT, SPECIES_BOUFFALANT, SPECIES_INDEEDEE, SPECIES_STOUTLAND, SPECIES_EMPOLEON, SPECIES_LOPUNNY_MEGA, SPECIES_DUGTRIO_A, SPECIES_SWELLOW, SPECIES_STEELIX, SPECIES_SCIZOR},
    },
    [CLASS_RIVAL_2] = {
        [0] = {SPECIES_DEERLING_AUTUMN, SPECIES_DEERLING_WINTER, SPECIES_MEOWTH, SPECIES_RATTATA, SPECIES_STARLY, SPECIES_EGG, SPECIES_DUDUNSPARCE_THREE, SPECIES_DOLLIV, SPECIES_SKITTY, SPECIES_MAUSHOLD_FOUR},
        [1] = {SPECIES_YUNGOOS, SPECIES_SENTRET, SPECIES_MINCCINO, SPECIES_ARBOLIVA, SPECIES_AZURILL, SPECIES_EGG, SPECIES_SMOLIV, SPECIES_ZORUA_H, SPECIES_SQUAWKABILLY, SPECIES_DUDUNSPARCE_THREE},
        [2] = {SPECIES_SHROODLE, SPECIES_RATTATA_A, SPECIES_TERAPAGOS, SPECIES_AZURILL, SPECIES_DOLLIV, SPECIES_LECHONK, SPECIES_TAILLOW, SPECIES_IGGLYBUFF, SPECIES_TERAPAGOS_STELLAR, SPECIES_BUNNELBY},
        [3] = {SPECIES_RATICATE, SPECIES_JIGGLYPUFF, SPECIES_DIGGERSBY, SPECIES_SLAKOTH, SPECIES_WIGGLYTUFF, SPECIES_LINOONE_G, SPECIES_HERDIER, SPECIES_SWABLU, SPECIES_VIGOROTH, SPECIES_LICKITUNG},
        [4] = {SPECIES_LICKITUNG, SPECIES_LITLEO, SPECIES_BUNEARY, SPECIES_SPINDA, SPECIES_DODUO, SPECIES_WIGGLYTUFF, SPECIES_RATICATE, SPECIES_FURRET, SPECIES_SMEARGLE, SPECIES_RATICATE_A},
        [5] = {SPECIES_TRANQUILL, SPECIES_PERSIAN, SPECIES_STARAVIA, SPECIES_CASTFORM, SPECIES_DELCATTY, SPECIES_WATCHOG, SPECIES_DODUO, SPECIES_MUNCHLAX, SPECIES_TYPE_NULL, SPECIES_BUNEARY},
        [6] = {SPECIES_PORYGON_Z, SPECIES_PURUGLY, SPECIES_PYROAR, SPECIES_ARCEUS, SPECIES_WYRDEER, SPECIES_SAWSBUCK_SUMMER, SPECIES_MELOETTA, SPECIES_STARAPTOR, SPECIES_LOPUNNY, SPECIES_LOPUNNY_MEGA},
        [7] = {SPECIES_HELIOLISK, SPECIES_STARAPTOR, SPECIES_FURFROU_DANDY, SPECIES_SNORLAX_GIGA, SPECIES_DRAMPA, SPECIES_PIDGEOT, SPECIES_URSALUNA, SPECIES_ZOROARK_H, SPECIES_PYROAR, SPECIES_SILVALLY},
        [8] = {SPECIES_INDEEDEE, SPECIES_FURFROU, SPECIES_PYROAR_FEMALE, SPECIES_LOPUNNY, SPECIES_SNORLAX, SPECIES_PYROAR, SPECIES_AUDINO_MEGA, SPECIES_DODRIO, SPECIES_FURFROU_DEBUTANTE, SPECIES_FEAROW},
        [9] = {SPECIES_SWELLOW, SPECIES_SLAKING, SPECIES_LOPUNNY_MEGA, SPECIES_STARAPTOR, SPECIES_LICKILICKY, SPECIES_SAWSBUCK_AUTUMN, SPECIES_MELOETTA, SPECIES_PYROAR_FEMALE, SPECIES_CINCCINO, SPECIES_SAWSBUCK_WINTER},
    },
    [CLASS_CHAMPION] = {
        [0] = {SPECIES_BIBAREL, SPECIES_SQUAWKABILLY_BLUE, SPECIES_FURFROU_HEART, SPECIES_WHISMUR, SPECIES_BEWEAR, SPECIES_MELOETTA_PIROUETTE, SPECIES_SAWSBUCK_AUTUMN, SPECIES_EEVEE, SPECIES_DODRIO, SPECIES_TYPE_NULL},
        [1] = {SPECIES_FURFROU_LA_REINE, SPECIES_WIGGLYTUFF, SPECIES_BOUFFALANT, SPECIES_SLAKING, SPECIES_STANTLER, SPECIES_BIDOOF, SPECIES_STUFFUL, SPECIES_PIDGEOTTO, SPECIES_SQUAWKABILLY_YELLOW, SPECIES_HELIOLISK},
        [2] = {SPECIES_BUNEARY, SPECIES_URSALUNA_BLOODMOON, SPECIES_LINOONE_G, SPECIES_TAILLOW, SPECIES_DRAMPA, SPECIES_SHROODLE, SPECIES_SLAKOTH, SPECIES_STOUTLAND, SPECIES_WIGGLYTUFF, SPECIES_SNORLAX},
        [3] = {SPECIES_CHATOT, SPECIES_CASTFORM, SPECIES_PIKIPEK, SPECIES_FURFROU_KABUKI, SPECIES_LINOONE, SPECIES_MAUSHOLD, SPECIES_SAWSBUCK_AUTUMN, SPECIES_UNFEZANT, SPECIES_OINKOLOGNE_FEMALE, SPECIES_EEVEE},
        [4] = {SPECIES_SILVALLY, SPECIES_BOUFFALANT, SPECIES_MEOWTH, SPECIES_DRAMPA, SPECIES_DOLLIV, SPECIES_LINOONE, SPECIES_BRAVIARY, SPECIES_TAUROS, SPECIES_AZURILL, SPECIES_SQUAWKABILLY_BLUE},
        [5] = {SPECIES_AUDINO_MEGA, SPECIES_PORYGON, SPECIES_DOLLIV, SPECIES_FURFROU_DIAMOND, SPECIES_DRAMPA, SPECIES_MEOWTH_GIGA, SPECIES_TERAPAGOS_TERASTAL, SPECIES_FARFETCHD, SPECIES_MAUSHOLD_FOUR, SPECIES_SLAKING},
        [6] = {SPECIES_ZORUA_H, SPECIES_FURFROU_DEBUTANTE, SPECIES_MILTANK, SPECIES_LINOONE_G, SPECIES_PIKIPEK, SPECIES_AMBIPOM, SPECIES_BRAVIARY, SPECIES_FURFROU_STAR, SPECIES_MEOWTH_GIGA, SPECIES_AIPOM},
        [7] = {SPECIES_MUNCHLAX, SPECIES_SENTRET, SPECIES_ORANGURU, SPECIES_SQUAWKABILLY, SPECIES_EEVEE_GIGA, SPECIES_FURFROU_LA_REINE, SPECIES_SWABLU, SPECIES_SLAKOTH, SPECIES_MAUSHOLD, SPECIES_TOUCANNON},
        [8] = {SPECIES_FURFROU_KABUKI, SPECIES_LOPUNNY, SPECIES_SMOLIV, SPECIES_PATRAT, SPECIES_EEVEE_GIGA, SPECIES_LITLEO, SPECIES_PIDOVE, SPECIES_GUMSHOOS, SPECIES_PYROAR, SPECIES_TAUROS},
        [9] = {SPECIES_BRAVIARY, SPECIES_MINCCINO, SPECIES_SQUAWKABILLY_BLUE, SPECIES_SNORLAX_GIGA, SPECIES_VIGOROTH, SPECIES_TOUCANNON, SPECIES_STARLY, SPECIES_WHISMUR, SPECIES_MEOWTH_GIGA, SPECIES_GUMSHOOS},
    },
    [CLASS_CHANNELER] = {
        [0] = {SPECIES_RATTATA_A, SPECIES_MINCCINO, SPECIES_LILLIPUP, SPECIES_TANDEMAUS, SPECIES_BUNNELBY, SPECIES_FLETCHLING, SPECIES_FARIGIRAF, SPECIES_DEERLING_WINTER, SPECIES_DEERLING_AUTUMN, SPECIES_MAUSHOLD},
        [1] = {SPECIES_TERAPAGOS_TERASTAL, SPECIES_HELIOPTILE, SPECIES_SPEAROW, SPECIES_DUDUNSPARCE_THREE, SPECIES_ZIGZAGOON_G, SPECIES_GRAFAIAI, SPECIES_URSALUNA_BLOODMOON, SPECIES_OINKOLOGNE, SPECIES_DEERLING_SUMMER, SPECIES_MEOWTH_GIGA},
        [2] = {SPECIES_TERAPAGOS, SPECIES_DUDUNSPARCE_THREE, SPECIES_SQUAWKABILLY_WHITE, SPECIES_WHISMUR, SPECIES_PATRAT, SPECIES_EGG, SPECIES_ZIGZAGOON, SPECIES_ZIGZAGOON_G, SPECIES_DEERLING_SUMMER, SPECIES_MAUSHOLD},
        [3] = {SPECIES_LOUDRED, SPECIES_AIPOM, SPECIES_SPINDA, SPECIES_DELCATTY, SPECIES_DUNSPARCE, SPECIES_DIGGERSBY, SPECIES_TRANQUILL, SPECIES_SLAKOTH, SPECIES_FURRET, SPECIES_KECLEON},
        [4] = {SPECIES_PORYGON, SPECIES_HERDIER, SPECIES_LICKITUNG, SPECIES_TRANQUILL, SPECIES_BUNEARY, SPECIES_TRUMBEAK, SPECIES_WATCHOG, SPECIES_DUNSPARCE, SPECIES_LOUDRED, SPECIES_LITLEO},
        [5] = {SPECIES_MUNCHLAX, SPECIES_DODUO, SPECIES_TRUMBEAK, SPECIES_KECLEON, SPECIES_GLAMEOW, SPECIES_LOUDRED, SPECIES_CHATOT, SPECIES_PERSIAN, SPECIES_DIGGERSBY, SPECIES_PORYGON},
        [6] = {SPECIES_LOPUNNY_MEGA, SPECIES_BEWEAR, SPECIES_STARAPTOR, SPECIES_REGIGIGAS, SPECIES_CHANSEY, SPECIES_INDEEDEE_FEMALE, SPECIES_PYROAR, SPECIES_LICKILICKY, SPECIES_AUDINO_MEGA, SPECIES_MELOETTA},
        [7] = {SPECIES_LOPUNNY, SPECIES_DUBWOOL, SPECIES_HELIOLISK, SPECIES_PIDGEOT_MEGA, SPECIES_SNORLAX_GIGA, SPECIES_HAPPINY, SPECIES_MILTANK, SPECIES_FURFROU_DIAMOND, SPECIES_PORYGON_Z, SPECIES_TOUCANNON},
        [8] = {SPECIES_FURFROU_KABUKI, SPECIES_URSALUNA, SPECIES_OBSTAGOON, SPECIES_SNORLAX_GIGA, SPECIES_KANGASKHAN_MEGA, SPECIES_DODRIO, SPECIES_ZOROARK_H, SPECIES_BEWEAR, SPECIES_CINCCINO, SPECIES_FURFROU_DEBUTANTE},
        [9] = {SPECIES_DRAMPA, SPECIES_FURFROU_MATRON, SPECIES_SAWSBUCK, SPECIES_MELOETTA, SPECIES_BRAVIARY, SPECIES_KANGASKHAN, SPECIES_FURFROU, SPECIES_SWELLOW, SPECIES_ZOROARK_H, SPECIES_FURFROU_DIAMOND},
    },
    [CLASS_TWINS] = {
        [0] = {SPECIES_GRAFAIAI, SPECIES_RATTATA, SPECIES_SENTRET, SPECIES_STARLY, SPECIES_SKITTY, SPECIES_TAILLOW, SPECIES_MINCCINO, SPECIES_SQUAWKABILLY_WHITE, SPECIES_CYCLIZAR, SPECIES_LECHONK},
        [1] = {SPECIES_LECHONK, SPECIES_MEOWTH, SPECIES_PIDGEY, SPECIES_CYCLIZAR, SPECIES_DITTO, SPECIES_PIKIPEK, SPECIES_MINCCINO, SPECIES_SQUAWKABILLY_WHITE, SPECIES_GRAFAIAI, SPECIES_OINKOLOGNE},
        [2] = {SPECIES_DEERLING_WINTER, SPECIES_PIDOVE, SPECIES_DEERLING_SUMMER, SPECIES_SHROODLE, SPECIES_SQUAWKABILLY_YELLOW, SPECIES_DITTO, SPECIES_TERAPAGOS_TERASTAL, SPECIES_MAUSHOLD, SPECIES_SKWOVET, SPECIES_URSALUNA_BLOODMOON},
        [3] = {SPECIES_LICKITUNG, SPECIES_WATCHOG, SPECIES_DODUO, SPECIES_RATICATE, SPECIES_TRANQUILL, SPECIES_FARFETCHD, SPECIES_CASTFORM, SPECIES_RATICATE_A, SPECIES_DIGGERSBY, SPECIES_DELCATTY},
        [4] = {SPECIES_MUNCHLAX, SPECIES_PIDGEOTTO, SPECIES_WOOLOO, SPECIES_TRUMBEAK, SPECIES_FURRET, SPECIES_RUFFLET, SPECIES_PERSIAN, SPECIES_BIBAREL, SPECIES_DUNSPARCE, SPECIES_TYPE_NULL},
        [5] = {SPECIES_WATCHOG, SPECIES_WIGGLYTUFF, SPECIES_MUNCHLAX, SPECIES_SPINDA, SPECIES_SMEARGLE, SPECIES_BIBAREL, SPECIES_DUNSPARCE, SPECIES_PERSIAN, SPECIES_DIGGERSBY, SPECIES_STARAVIA},
        [6] = {SPECIES_STANTLER, SPECIES_DRAMPA, SPECIES_MELOETTA_PIROUETTE, SPECIES_TOUCANNON, SPECIES_FURFROU, SPECIES_BOUFFALANT, SPECIES_PYROAR, SPECIES_ARCEUS, SPECIES_FURFROU_LA_REINE, SPECIES_UNFEZANT_F},
        [7] = {SPECIES_FURFROU_KABUKI, SPECIES_DRAMPA, SPECIES_FURFROU_HEART, SPECIES_GREEDENT, SPECIES_SAWSBUCK_AUTUMN, SPECIES_AUDINO, SPECIES_PORYGON2, SPECIES_URSALUNA, SPECIES_AMBIPOM, SPECIES_PIDGEOT},
        [8] = {SPECIES_ZOROARK_H, SPECIES_SNORLAX_GIGA, SPECIES_DUBWOOL, SPECIES_FEAROW, SPECIES_SILVALLY, SPECIES_HELIOLISK, SPECIES_FURFROU_HEART, SPECIES_FURFROU_PHAROAH, SPECIES_FURFROU, SPECIES_CINCCINO},
        [9] = {SPECIES_FEAROW, SPECIES_DRAMPA, SPECIES_OBSTAGOON, SPECIES_FURFROU_PHAROAH, SPECIES_INDEEDEE, SPECIES_FURFROU_DIAMOND, SPECIES_DUBWOOL, SPECIES_CINCCINO, SPECIES_INDEEDEE_FEMALE, SPECIES_LOPUNNY_MEGA},
    },
    [CLASS_COOL_COUPLE] = {
        [0] = {SPECIES_ZIGZAGOON_G, SPECIES_OINKOLOGNE_FEMALE, SPECIES_MEOWTH_GIGA, SPECIES_PIDGEY, SPECIES_PIKIPEK, SPECIES_DEERLING_WINTER, SPECIES_TANDEMAUS, SPECIES_PATRAT, SPECIES_SKWOVET, SPECIES_YUNGOOS},
        [1] = {SPECIES_MEOWTH, SPECIES_SPEAROW, SPECIES_SKWOVET, SPECIES_DEERLING_AUTUMN, SPECIES_DEERLING, SPECIES_RATTATA, SPECIES_DEERLING_SUMMER, SPECIES_TERAPAGOS, SPECIES_DUDUNSPARCE, SPECIES_WHISMUR},
        [2] = {SPECIES_DEERLING_SUMMER, SPECIES_LECHONK, SPECIES_SQUAWKABILLY_YELLOW, SPECIES_GRAFAIAI, SPECIES_PATRAT, SPECIES_OINKOLOGNE_FEMALE, SPECIES_OINKOLOGNE, SPECIES_DUDUNSPARCE_THREE, SPECIES_AZURILL, SPECIES_RATTATA_A},
        [3] = {SPECIES_JIGGLYPUFF, SPECIES_LINOONE, SPECIES_TRANQUILL, SPECIES_MUNCHLAX, SPECIES_SWABLU, SPECIES_RATICATE_A, SPECIES_RUFFLET, SPECIES_LINOONE_G, SPECIES_FARFETCHD, SPECIES_PERSIAN},
        [4] = {SPECIES_WATCHOG, SPECIES_KECLEON, SPECIES_LOUDRED, SPECIES_BUNEARY, SPECIES_RATICATE_A, SPECIES_DODUO, SPECIES_SWABLU, SPECIES_LINOONE_G, SPECIES_DUNSPARCE, SPECIES_CHATOT},
        [5] = {SPECIES_STUFFUL, SPECIES_RATICATE, SPECIES_VIGOROTH, SPECIES_DELCATTY, SPECIES_RUFFLET, SPECIES_PIDGEOTTO, SPECIES_LOUDRED, SPECIES_SLAKOTH, SPECIES_DODUO, SPECIES_SWABLU},
        [6] = {SPECIES_FURFROU_KABUKI, SPECIES_OBSTAGOON, SPECIES_DRAMPA, SPECIES_CINCCINO, SPECIES_FURFROU_DIAMOND, SPECIES_UNFEZANT, SPECIES_UNFEZANT_F, SPECIES_FURFROU_DEBUTANTE, SPECIES_URSARING, SPECIES_ZOROARK_H},
        [7] = {SPECIES_FEAROW, SPECIES_MELOETTA, SPECIES_INDEEDEE, SPECIES_KANGASKHAN_MEGA, SPECIES_SAWSBUCK_SUMMER, SPECIES_PIDGEOT, SPECIES_STARAPTOR, SPECIES_FURFROU_PHAROAH, SPECIES_EXPLOUD, SPECIES_FURFROU_DANDY},
        [8] = {SPECIES_URSARING, SPECIES_MELOETTA, SPECIES_TOUCANNON, SPECIES_ZANGOOSE, SPECIES_ZOROARK_H, SPECIES_FURFROU_STAR, SPECIES_OBSTAGOON, SPECIES_GREEDENT, SPECIES_INDEEDEE_FEMALE, SPECIES_CINCCINO},
        [9] = {SPECIES_GREEDENT, SPECIES_PURUGLY, SPECIES_FURFROU_KABUKI, SPECIES_FEAROW, SPECIES_OBSTAGOON, SPECIES_CHANSEY, SPECIES_TOUCANNON, SPECIES_KANGASKHAN_MEGA, SPECIES_LOPUNNY, SPECIES_SNORLAX_GIGA},
    },
    [CLASS_YOUNG_COUPLE] = {
        [0] = {SPECIES_MEOWTH, SPECIES_YUNGOOS, SPECIES_DEERLING, SPECIES_ARBOLIVA, SPECIES_TERAPAGOS_STELLAR, SPECIES_DOLLIV, SPECIES_CYCLIZAR, SPECIES_HOOTHOOT, SPECIES_DUDUNSPARCE, SPECIES_DUDUNSPARCE_THREE},
        [1] = {SPECIES_IGGLYBUFF, SPECIES_TERAPAGOS_STELLAR, SPECIES_MAUSHOLD_FOUR, SPECIES_RATTATA_A, SPECIES_SQUAWKABILLY, SPECIES_SPEAROW, SPECIES_LILLIPUP, SPECIES_WHISMUR, SPECIES_TERAPAGOS_TERASTAL, SPECIES_TAILLOW},
        [2] = {SPECIES_BUNNELBY, SPECIES_MEOWTH, SPECIES_ARBOLIVA, SPECIES_FARIGIRAF, SPECIES_URSALUNA_BLOODMOON, SPECIES_DEERLING_AUTUMN, SPECIES_MINCCINO, SPECIES_PIDOVE, SPECIES_HOOTHOOT, SPECIES_EGG},
        [3] = {SPECIES_HERDIER, SPECIES_FURRET, SPECIES_RUFFLET, SPECIES_RATICATE_A, SPECIES_AIPOM, SPECIES_DUNSPARCE, SPECIES_JIGGLYPUFF, SPECIES_PORYGON, SPECIES_GUMSHOOS, SPECIES_LOUDRED},
        [4] = {SPECIES_PIDGEOTTO, SPECIES_EEVEE, SPECIES_TEDDIURSA, SPECIES_RUFFLET, SPECIES_TYPE_NULL, SPECIES_AIPOM, SPECIES_MUNCHLAX, SPECIES_PERSIAN, SPECIES_BIBAREL, SPECIES_GUMSHOOS},
        [5] = {SPECIES_CHATOT, SPECIES_SWABLU, SPECIES_RATICATE, SPECIES_DODUO, SPECIES_TRUMBEAK, SPECIES_JIGGLYPUFF, SPECIES_RATICATE_A, SPECIES_SPINDA, SPECIES_WOOLOO, SPECIES_MUNCHLAX},
        [6] = {SPECIES_DODRIO, SPECIES_PYROAR_FEMALE, SPECIES_ARCEUS, SPECIES_LOPUNNY_MEGA, SPECIES_MILTANK, SPECIES_FURFROU_LA_REINE, SPECIES_SILVALLY, SPECIES_ZOROARK_H, SPECIES_PORYGON_Z, SPECIES_WYRDEER},
        [7] = {SPECIES_URSALUNA, SPECIES_STANTLER, SPECIES_FURFROU_DEBUTANTE, SPECIES_AUDINO_MEGA, SPECIES_PYROAR_FEMALE, SPECIES_CINCCINO, SPECIES_KOMALA, SPECIES_DRAMPA, SPECIES_KANGASKHAN_MEGA, SPECIES_MILTANK},
        [8] = {SPECIES_SAWSBUCK, SPECIES_FEAROW, SPECIES_SNORLAX, SPECIES_OBSTAGOON, SPECIES_SAWSBUCK_AUTUMN, SPECIES_GREEDENT, SPECIES_AUDINO, SPECIES_BRAVIARY, SPECIES_FURFROU_DANDY, SPECIES_SAWSBUCK_WINTER},
        [9] = {SPECIES_OBSTAGOON, SPECIES_LOPUNNY_MEGA, SPECIES_PORYGON_Z, SPECIES_FURFROU_DANDY, SPECIES_SLAKING, SPECIES_AUDINO, SPECIES_UNFEZANT, SPECIES_SAWSBUCK_WINTER, SPECIES_PORYGON2, SPECIES_BLISSEY},
    },
    [CLASS_CRUSH_KIN] = {
        [0] = {SPECIES_FLETCHLING, SPECIES_MAUSHOLD_FOUR, SPECIES_DITTO, SPECIES_TAILLOW, SPECIES_MAUSHOLD, SPECIES_SKWOVET, SPECIES_SHROODLE, SPECIES_PIDOVE, SPECIES_SQUAWKABILLY, SPECIES_PIKIPEK},
        [1] = {SPECIES_DEERLING_WINTER, SPECIES_SPEAROW, SPECIES_DEERLING, SPECIES_IGGLYBUFF, SPECIES_LILLIPUP, SPECIES_OINKOLOGNE_FEMALE, SPECIES_BIDOOF, SPECIES_PIKIPEK, SPECIES_YUNGOOS, SPECIES_MINCCINO},
        [2] = {SPECIES_TERAPAGOS_TERASTAL, SPECIES_MEOWTH, SPECIES_ZIGZAGOON, SPECIES_WHISMUR, SPECIES_TERAPAGOS_STELLAR, SPECIES_TANDEMAUS, SPECIES_FARIGIRAF, SPECIES_FLETCHLING, SPECIES_RATTATA, SPECIES_BIDOOF},
        [3] = {SPECIES_PERSIAN, SPECIES_GLAMEOW, SPECIES_SWABLU, SPECIES_TEDDIURSA, SPECIES_LINOONE_G, SPECIES_WOOLOO, SPECIES_SLAKOTH, SPECIES_JIGGLYPUFF, SPECIES_SPINDA, SPECIES_RUFFLET},
        [4] = {SPECIES_VIGOROTH, SPECIES_FURRET, SPECIES_CASTFORM, SPECIES_DUNSPARCE, SPECIES_STARAVIA, SPECIES_BIBAREL, SPECIES_LICKITUNG, SPECIES_STUFFUL, SPECIES_DIGGERSBY, SPECIES_PIDGEOTTO},
        [5] = {SPECIES_DELCATTY, SPECIES_DODUO, SPECIES_SMEARGLE, SPECIES_KECLEON, SPECIES_SLAKOTH, SPECIES_WATCHOG, SPECIES_TYPE_NULL, SPECIES_TRANQUILL, SPECIES_TRUMBEAK, SPECIES_GLAMEOW},
        [6] = {SPECIES_ARCEUS, SPECIES_AMBIPOM, SPECIES_FEAROW, SPECIES_TAUROS, SPECIES_TOUCANNON, SPECIES_ORANGURU, SPECIES_DODRIO, SPECIES_PYROAR_FEMALE, SPECIES_URSARING, SPECIES_STARAPTOR},
        [7] = {SPECIES_URSALUNA, SPECIES_URSARING, SPECIES_SLAKING, SPECIES_AUDINO, SPECIES_STOUTLAND, SPECIES_MELOETTA, SPECIES_INDEEDEE, SPECIES_ARCEUS, SPECIES_PURUGLY, SPECIES_INDEEDEE_FEMALE},
        [8] = {SPECIES_SNORLAX, SPECIES_FURFROU_HEART, SPECIES_SWELLOW, SPECIES_REGIGIGAS, SPECIES_SAWSBUCK_AUTUMN, SPECIES_AUDINO, SPECIES_FURFROU_PHAROAH, SPECIES_CINCCINO, SPECIES_ZANGOOSE, SPECIES_KANGASKHAN},
        [9] = {SPECIES_DRAMPA, SPECIES_FURFROU_DANDY, SPECIES_KANGASKHAN_MEGA, SPECIES_AMBIPOM, SPECIES_SILVALLY, SPECIES_SNORLAX_GIGA, SPECIES_PORYGON_Z, SPECIES_FURFROU_KABUKI, SPECIES_HELIOLISK, SPECIES_ORANGURU},
    },
    [CLASS_SIS_AND_BRO] = {
        [0] = {SPECIES_OINKOLOGNE, SPECIES_TERAPAGOS, SPECIES_MAUSHOLD, SPECIES_CYCLIZAR, SPECIES_WHISMUR, SPECIES_MAUSHOLD_FOUR, SPECIES_RATTATA_A, SPECIES_ZORUA_H, SPECIES_IGGLYBUFF, SPECIES_OINKOLOGNE_FEMALE},
        [1] = {SPECIES_MINCCINO, SPECIES_ARBOLIVA, SPECIES_SQUAWKABILLY_YELLOW, SPECIES_DOLLIV, SPECIES_STARLY, SPECIES_SQUAWKABILLY, SPECIES_SQUAWKABILLY_BLUE, SPECIES_DEERLING_SUMMER, SPECIES_DUDUNSPARCE_THREE, SPECIES_IGGLYBUFF},
        [2] = {SPECIES_LECHONK, SPECIES_TANDEMAUS, SPECIES_SHROODLE, SPECIES_ZORUA_H, SPECIES_SPEAROW, SPECIES_CYCLIZAR, SPECIES_DEERLING, SPECIES_TAILLOW, SPECIES_OINKOLOGNE, SPECIES_ZIGZAGOON_G},
        [3] = {SPECIES_SWABLU, SPECIES_LINOONE, SPECIES_PIDGEOTTO, SPECIES_LINOONE_G, SPECIES_TRUMBEAK, SPECIES_CASTFORM, SPECIES_FARFETCHD, SPECIES_EEVEE_GIGA, SPECIES_TEDDIURSA, SPECIES_JIGGLYPUFF},
        [4] = {SPECIES_LITLEO, SPECIES_CHATOT, SPECIES_HERDIER, SPECIES_SLAKOTH, SPECIES_BUNEARY, SPECIES_GLAMEOW, SPECIES_LOUDRED, SPECIES_PERSIAN, SPECIES_BIBAREL, SPECIES_KECLEON},
        [5] = {SPECIES_BUNEARY, SPECIES_GLAMEOW, SPECIES_CASTFORM, SPECIES_GIRAFARIG, SPECIES_SLAKOTH, SPECIES_CHATOT, SPECIES_TRUMBEAK, SPECIES_VIGOROTH, SPECIES_KECLEON, SPECIES_DUNSPARCE},
        [6] = {SPECIES_INDEEDEE, SPECIES_PORYGON2, SPECIES_BOUFFALANT, SPECIES_KANGASKHAN_MEGA, SPECIES_FURFROU_LA_REINE, SPECIES_LOPUNNY, SPECIES_CHANSEY, SPECIES_SLAKING, SPECIES_PIDGEOT_MEGA, SPECIES_ORANGURU},
        [7] = {SPECIES_PIDGEOT_MEGA, SPECIES_SAWSBUCK_SUMMER, SPECIES_KANGASKHAN, SPECIES_GREEDENT, SPECIES_FEAROW, SPECIES_BRAVIARY, SPECIES_KANGASKHAN_MEGA, SPECIES_URSARING, SPECIES_SILVALLY, SPECIES_INDEEDEE},
        [8] = {SPECIES_LOPUNNY, SPECIES_PORYGON2, SPECIES_FURFROU, SPECIES_PYROAR, SPECIES_ZOROARK_H, SPECIES_BLISSEY, SPECIES_URSARING, SPECIES_STANTLER, SPECIES_SLAKING, SPECIES_SNORLAX},
        [9] = {SPECIES_PIDGEOT_MEGA, SPECIES_SAWSBUCK_AUTUMN, SPECIES_INDEEDEE_FEMALE, SPECIES_FURFROU_HEART, SPECIES_SAWSBUCK, SPECIES_FURFROU_MATRON, SPECIES_PURUGLY, SPECIES_BEWEAR, SPECIES_SAWSBUCK_SUMMER, SPECIES_ZANGOOSE},
    },
    [CLASS_PKMN_PROF] = {
        [0] = {SPECIES_MAUSHOLD, SPECIES_SKITTY, SPECIES_DUDUNSPARCE_THREE, SPECIES_TAILLOW, SPECIES_FLETCHLING, SPECIES_DUDUNSPARCE, SPECIES_OINKOLOGNE_FEMALE, SPECIES_SQUAWKABILLY_WHITE, SPECIES_HELIOPTILE, SPECIES_GRAFAIAI},
        [1] = {SPECIES_IGGLYBUFF, SPECIES_DEERLING, SPECIES_MAUSHOLD_FOUR, SPECIES_ZORUA_H, SPECIES_MINCCINO, SPECIES_DEERLING_SUMMER, SPECIES_SHROODLE, SPECIES_OINKOLOGNE, SPECIES_HELIOPTILE, SPECIES_TERAPAGOS_STELLAR},
        [2] = {SPECIES_STARLY, SPECIES_BUNNELBY, SPECIES_RATTATA, SPECIES_TERAPAGOS_TERASTAL, SPECIES_PATRAT, SPECIES_HELIOPTILE, SPECIES_LECHONK, SPECIES_SQUAWKABILLY_WHITE, SPECIES_PIDGEY, SPECIES_MAUSHOLD},
        [3] = {SPECIES_STUFFUL, SPECIES_TEDDIURSA, SPECIES_SWABLU, SPECIES_FARFETCHD, SPECIES_LINOONE_G, SPECIES_MUNCHLAX, SPECIES_TYPE_NULL, SPECIES_DODUO, SPECIES_RATICATE, SPECIES_RUFFLET},
        [4] = {SPECIES_TYPE_NULL, SPECIES_LINOONE_G, SPECIES_DODUO, SPECIES_EEVEE_GIGA, SPECIES_TRUMBEAK, SPECIES_GLAMEOW, SPECIES_BUNEARY, SPECIES_TEDDIURSA, SPECIES_MUNCHLAX, SPECIES_RATICATE_A},
        [5] = {SPECIES_EEVEE, SPECIES_EEVEE_GIGA, SPECIES_LINOONE_G, SPECIES_DELCATTY, SPECIES_CHATOT, SPECIES_PERSIAN, SPECIES_VIGOROTH, SPECIES_LINOONE, SPECIES_SLAKOTH, SPECIES_PORYGON},
        [6] = {SPECIES_WYRDEER, SPECIES_UNFEZANT, SPECIES_SILVALLY, SPECIES_MELOETTA, SPECIES_AUDINO, SPECIES_SAWSBUCK, SPECIES_STARAPTOR, SPECIES_BEWEAR, SPECIES_ORANGURU, SPECIES_LICKILICKY},
        [7] = {SPECIES_FURFROU_DEBUTANTE, SPECIES_SAWSBUCK_SUMMER, SPECIES_ZOROARK_H, SPECIES_LOPUNNY_MEGA, SPECIES_STANTLER, SPECIES_AUDINO, SPECIES_PORYGON2, SPECIES_TAUROS, SPECIES_PYROAR, SPECIES_DRAMPA},
        [8] = {SPECIES_PIDGEOT_MEGA, SPECIES_UNFEZANT, SPECIES_AMBIPOM, SPECIES_PORYGON_Z, SPECIES_KOMALA, SPECIES_SAWSBUCK_AUTUMN, SPECIES_SAWSBUCK, SPECIES_STOUTLAND, SPECIES_DODRIO, SPECIES_SNORLAX_GIGA},
        [9] = {SPECIES_MILTANK, SPECIES_SAWSBUCK_SUMMER, SPECIES_SAWSBUCK_WINTER, SPECIES_SWELLOW, SPECIES_URSARING, SPECIES_LICKILICKY, SPECIES_KOMALA, SPECIES_ARCEUS, SPECIES_AUDINO, SPECIES_PYROAR_FEMALE},
    },
    [CLASS_PLAYER] = {
        [0] = {SPECIES_MEOWTH, SPECIES_SKITTY, SPECIES_MAUSHOLD, SPECIES_BIDOOF, SPECIES_ZIGZAGOON_G, SPECIES_ARBOLIVA, SPECIES_DUDUNSPARCE_THREE, SPECIES_DEERLING_SUMMER, SPECIES_ZORUA_H, SPECIES_DEERLING_WINTER},
        [1] = {SPECIES_DEERLING_WINTER, SPECIES_LILLIPUP, SPECIES_BIDOOF, SPECIES_FLETCHLING, SPECIES_SMOLIV, SPECIES_BUNNELBY, SPECIES_PIDGEY, SPECIES_LECHONK, SPECIES_YUNGOOS, SPECIES_ZIGZAGOON_G},
        [2] = {SPECIES_PIDGEY, SPECIES_DEERLING_SUMMER, SPECIES_MEOWTH, SPECIES_SQUAWKABILLY_WHITE, SPECIES_OINKOLOGNE_FEMALE, SPECIES_MINCCINO, SPECIES_TERAPAGOS, SPECIES_OINKOLOGNE, SPECIES_PIDOVE, SPECIES_LILLIPUP},
        [3] = {SPECIES_CHATOT, SPECIES_TRUMBEAK, SPECIES_CASTFORM, SPECIES_DELCATTY, SPECIES_DODUO, SPECIES_LINOONE, SPECIES_LITLEO, SPECIES_GLAMEOW, SPECIES_PIDGEOTTO, SPECIES_STUFFUL},
        [4] = {SPECIES_DODUO, SPECIES_MUNCHLAX, SPECIES_TEDDIURSA, SPECIES_JIGGLYPUFF, SPECIES_VIGOROTH, SPECIES_KECLEON, SPECIES_TYPE_NULL, SPECIES_TRUMBEAK, SPECIES_PORYGON, SPECIES_EEVEE},
        [5] = {SPECIES_SPINDA, SPECIES_STUFFUL, SPECIES_LINOONE, SPECIES_PIDGEOTTO, SPECIES_DODUO, SPECIES_KECLEON, SPECIES_SWABLU, SPECIES_DELCATTY, SPECIES_CHATOT, SPECIES_PORYGON},
        [6] = {SPECIES_SWELLOW, SPECIES_PURUGLY, SPECIES_MELOETTA_PIROUETTE, SPECIES_AMBIPOM, SPECIES_LICKILICKY, SPECIES_FURFROU_DANDY, SPECIES_PYROAR, SPECIES_PYROAR_FEMALE, SPECIES_SAWSBUCK_WINTER, SPECIES_UNFEZANT_F},
        [7] = {SPECIES_PYROAR, SPECIES_INDEEDEE_FEMALE, SPECIES_FURFROU_DEBUTANTE, SPECIES_PURUGLY, SPECIES_FURFROU_LA_REINE, SPECIES_SAWSBUCK_WINTER, SPECIES_CINCCINO, SPECIES_REGIGIGAS, SPECIES_FURFROU_DIAMOND, SPECIES_CHANSEY},
        [8] = {SPECIES_HAPPINY, SPECIES_TOUCANNON, SPECIES_KANGASKHAN_MEGA, SPECIES_MELOETTA_PIROUETTE, SPECIES_PYROAR, SPECIES_GREEDENT, SPECIES_FURFROU_DIAMOND, SPECIES_KANGASKHAN, SPECIES_AUDINO_MEGA, SPECIES_FURFROU_PHAROAH},
        [9] = {SPECIES_KOMALA, SPECIES_PYROAR_FEMALE, SPECIES_GREEDENT, SPECIES_LOPUNNY, SPECIES_INDEEDEE_FEMALE, SPECIES_LICKILICKY, SPECIES_SLAKING, SPECIES_PORYGON2, SPECIES_FURFROU_DEBUTANTE, SPECIES_PURUGLY},
    },
    [CLASS_CRUSH_GIRL] = {
        [0] = {SPECIES_MEOWTH_GIGA, SPECIES_CYCLIZAR, SPECIES_SHROODLE, SPECIES_SQUAWKABILLY_BLUE, SPECIES_PIDGEY, SPECIES_DOLLIV, SPECIES_TERAPAGOS, SPECIES_MAUSHOLD, SPECIES_BIDOOF, SPECIES_HELIOPTILE},
        [1] = {SPECIES_YUNGOOS, SPECIES_DEERLING_AUTUMN, SPECIES_HOOTHOOT, SPECIES_SENTRET, SPECIES_WHISMUR, SPECIES_BIDOOF, SPECIES_SHROODLE, SPECIES_SKITTY, SPECIES_HELIOPTILE, SPECIES_RATTATA_A},
        [2] = {SPECIES_DEERLING, SPECIES_PIDOVE, SPECIES_EGG, SPECIES_DOLLIV, SPECIES_SQUAWKABILLY_YELLOW, SPECIES_PIKIPEK, SPECIES_TERAPAGOS, SPECIES_LECHONK, SPECIES_MAUSHOLD, SPECIES_SPEAROW},
        [3] = {SPECIES_TYPE_NULL, SPECIES_SWABLU, SPECIES_PIDGEOTTO, SPECIES_GIRAFARIG, SPECIES_DODUO, SPECIES_LINOONE, SPECIES_EEVEE_GIGA, SPECIES_MUNCHLAX, SPECIES_STUFFUL, SPECIES_EEVEE},
        [4] = {SPECIES_AIPOM, SPECIES_LOUDRED, SPECIES_RATICATE, SPECIES_DIGGERSBY, SPECIES_HERDIER, SPECIES_DODUO, SPECIES_RUFFLET, SPECIES_SMEARGLE, SPECIES_EEVEE_GIGA, SPECIES_STARAVIA},
        [5] = {SPECIES_LICKITUNG, SPECIES_WATCHOG, SPECIES_TYPE_NULL, SPECIES_DUNSPARCE, SPECIES_BUNEARY, SPECIES_WIGGLYTUFF, SPECIES_EEVEE_GIGA, SPECIES_PERSIAN, SPECIES_LINOONE_G, SPECIES_HERDIER},
        [6] = {SPECIES_DODRIO, SPECIES_AUDINO, SPECIES_BLISSEY, SPECIES_FURFROU, SPECIES_INDEEDEE, SPECIES_EXPLOUD, SPECIES_PURUGLY, SPECIES_FURFROU_HEART, SPECIES_STOUTLAND, SPECIES_WYRDEER},
        [7] = {SPECIES_AMBIPOM, SPECIES_URSALUNA, SPECIES_ZOROARK_H, SPECIES_STOUTLAND, SPECIES_FURFROU_HEART, SPECIES_PIDGEOT_MEGA, SPECIES_REGIGIGAS, SPECIES_UNFEZANT, SPECIES_TOUCANNON, SPECIES_CHANSEY},
        [8] = {SPECIES_FURFROU_KABUKI, SPECIES_SAWSBUCK, SPECIES_LICKILICKY, SPECIES_PURUGLY, SPECIES_ORANGURU, SPECIES_LOPUNNY, SPECIES_TAUROS, SPECIES_PORYGON_Z, SPECIES_ZANGOOSE, SPECIES_FURFROU_DANDY},
        [9] = {SPECIES_SAWSBUCK_WINTER, SPECIES_SAWSBUCK_SUMMER, SPECIES_WYRDEER, SPECIES_TAUROS, SPECIES_PORYGON_Z, SPECIES_PURUGLY, SPECIES_DUBWOOL, SPECIES_SLAKING, SPECIES_SNORLAX, SPECIES_INDEEDEE},
    },
    [CLASS_TUBER] = {
        [0] = {SPECIES_YUNGOOS, SPECIES_AZURILL, SPECIES_DUDUNSPARCE_THREE, SPECIES_TAILLOW, SPECIES_BUNNELBY, SPECIES_TERAPAGOS_TERASTAL, SPECIES_LILLIPUP, SPECIES_DEERLING, SPECIES_IGGLYBUFF, SPECIES_MAUSHOLD},
        [1] = {SPECIES_SQUAWKABILLY, SPECIES_OINKOLOGNE_FEMALE, SPECIES_WHISMUR, SPECIES_ZIGZAGOON_G, SPECIES_DOLLIV, SPECIES_BUNNELBY, SPECIES_DEERLING_SUMMER, SPECIES_MAUSHOLD_FOUR, SPECIES_TERAPAGOS, SPECIES_SENTRET},
        [2] = {SPECIES_MEOWTH, SPECIES_HOOTHOOT, SPECIES_LECHONK, SPECIES_SMOLIV, SPECIES_MINCCINO, SPECIES_GRAFAIAI, SPECIES_FARIGIRAF, SPECIES_SPEAROW, SPECIES_ARBOLIVA, SPECIES_DEERLING_WINTER},
        [3] = {SPECIES_LITLEO, SPECIES_PORYGON, SPECIES_BUNEARY, SPECIES_SLAKOTH, SPECIES_HERDIER, SPECIES_EEVEE, SPECIES_KECLEON, SPECIES_MUNCHLAX, SPECIES_CHATOT, SPECIES_AIPOM},
        [4] = {SPECIES_RATICATE_A, SPECIES_STUFFUL, SPECIES_LOUDRED, SPECIES_DELCATTY, SPECIES_PORYGON, SPECIES_BUNEARY, SPECIES_CASTFORM, SPECIES_BIBAREL, SPECIES_SLAKOTH, SPECIES_AIPOM},
        [5] = {SPECIES_SLAKOTH, SPECIES_DUNSPARCE, SPECIES_RUFFLET, SPECIES_TEDDIURSA, SPECIES_PERSIAN, SPECIES_LICKITUNG, SPECIES_GLAMEOW, SPECIES_STUFFUL, SPECIES_LINOONE_G, SPECIES_JIGGLYPUFF},
        [6] = {SPECIES_KANGASKHAN, SPECIES_OBSTAGOON, SPECIES_FEAROW, SPECIES_FURFROU_KABUKI, SPECIES_REGIGIGAS, SPECIES_DRAMPA, SPECIES_HAPPINY, SPECIES_FURFROU_STAR, SPECIES_AMBIPOM, SPECIES_DUBWOOL},
        [7] = {SPECIES_PORYGON_Z, SPECIES_SAWSBUCK, SPECIES_SNORLAX_GIGA, SPECIES_DRAMPA, SPECIES_FEAROW, SPECIES_ZOROARK_H, SPECIES_FURFROU_PHAROAH, SPECIES_SILVALLY, SPECIES_KOMALA, SPECIES_PIDGEOT_MEGA},
        [8] = {SPECIES_DUBWOOL, SPECIES_LOPUNNY_MEGA, SPECIES_SAWSBUCK_SUMMER, SPECIES_BLISSEY, SPECIES_ARCEUS, SPECIES_KOMALA, SPECIES_AUDINO_MEGA, SPECIES_INDEEDEE_FEMALE, SPECIES_ORANGURU, SPECIES_FURFROU_DEBUTANTE},
        [9] = {SPECIES_PORYGON_Z, SPECIES_KANGASKHAN, SPECIES_FURFROU_DEBUTANTE, SPECIES_HAPPINY, SPECIES_LICKILICKY, SPECIES_AMBIPOM, SPECIES_REGIGIGAS, SPECIES_SAWSBUCK_WINTER, SPECIES_KOMALA, SPECIES_SAWSBUCK_SUMMER},
    },
    [CLASS_PKMN_BREEDER] = {
        [0] = {SPECIES_SCREAM_TAIL, SPECIES_SQUAWKABILLY, SPECIES_ZIGZAGOON_G, SPECIES_TINKATINK, SPECIES_RATTATA_A, SPECIES_FIDOUGH, SPECIES_MARILL, SPECIES_DUDUNSPARCE_THREE, SPECIES_HOOTHOOT, SPECIES_MILCERY},
        [1] = {SPECIES_SWIRLIX, SPECIES_MEOWTH_GIGA, SPECIES_SKITTY, SPECIES_SKWOVET, SPECIES_MAUSHOLD, SPECIES_DACHSBUN, SPECIES_SHROODLE, SPECIES_SQUAWKABILLY_YELLOW, SPECIES_BUNNELBY, SPECIES_SENTRET},
        [2] = {SPECIES_DUDUNSPARCE, SPECIES_COTTONEE, SPECIES_MAUSHOLD, SPECIES_SHROODLE, SPECIES_RATTATA_A, SPECIES_DACHSBUN, SPECIES_SCREAM_TAIL, SPECIES_FLABEBE_ORANGE, SPECIES_DEERLING_WINTER, SPECIES_DUDUNSPARCE_THREE},
        [3] = {SPECIES_LINOONE, SPECIES_TEDDIURSA, SPECIES_RATICATE, SPECIES_LICKITUNG, SPECIES_CUTIEFLY, SPECIES_MAGEARNA_P, SPECIES_KIRLIA, SPECIES_CLEFABLE, SPECIES_FARFETCHD, SPECIES_GUMSHOOS},
        [4] = {SPECIES_CASTFORM, SPECIES_GLAMEOW, SPECIES_WATCHOG, SPECIES_WOOLOO, SPECIES_CLEFABLE, SPECIES_BUNEARY, SPECIES_CHATOT, SPECIES_MIME_JR_G, SPECIES_SILVALLY_FAIRY, SPECIES_BIBAREL},
        [5] = {SPECIES_KECLEON, SPECIES_SILVALLY_FAIRY, SPECIES_EEVEE, SPECIES_RIBOMBEE, SPECIES_AIPOM, SPECIES_FLOETTE_ORANGE, SPECIES_EEVEE_GIGA, SPECIES_TEDDIURSA, SPECIES_GLAMEOW, SPECIES_JIGGLYPUFF},
        [6] = {SPECIES_FLORGES, SPECIES_AROMATISSE, SPECIES_KLEFKI, SPECIES_FURFROU_DEBUTANTE, SPECIES_HATTERENE_GIGA, SPECIES_FLORGES_WHITE, SPECIES_STANTLER, SPECIES_GRIMMSNARL, SPECIES_BLISSEY, SPECIES_FLORGES_BLUE},
        [7] = {SPECIES_PORYGON_Z, SPECIES_FURFROU_DIAMOND, SPECIES_ZACIAN, SPECIES_ZANGOOSE, SPECIES_FURFROU_HEART, SPECIES_TAPU_LELE, SPECIES_ORANGURU, SPECIES_HATTERENE, SPECIES_DIANCIE, SPECIES_FLORGES_BLUE},
        [8] = {SPECIES_MEGANIUM, SPECIES_HAPPINY, SPECIES_PRIMARINA, SPECIES_STANTLER, SPECIES_BELLOSSOM, SPECIES_URSALUNA, SPECIES_FLORGES_YELLOW, SPECIES_MILTANK, SPECIES_FLORGES_BLUE, SPECIES_HELIOLISK},
        [9] = {SPECIES_CHANSEY, SPECIES_KOMALA, SPECIES_KLEFKI, SPECIES_MELOETTA_PIROUETTE, SPECIES_FEAROW, SPECIES_ENAMORUS_THERIAN, SPECIES_MIMIKYU_BUSTED, SPECIES_BRAVIARY, SPECIES_FURFROU_DEBUTANTE, SPECIES_HATTERENE},
    },
    [CLASS_PKMN_RANGER] = {
        [0] = {SPECIES_OGERPON_WELLSPRING_MASK, SPECIES_GLIMMET, SPECIES_DEERLING_WINTER, SPECIES_DEERLING_AUTUMN, SPECIES_BULBASAUR, SPECIES_TOEDSCRUEL, SPECIES_POLTCHAGEIST, SPECIES_PUMPKABOO, SPECIES_DIPPLIN, SPECIES_SCOVILLAIN},
        [1] = {SPECIES_OGERPON_HEARTHFLAME_MASK, SPECIES_BULBASAUR, SPECIES_TREECKO, SPECIES_TURTWIG, SPECIES_DEERLING_SUMMER, SPECIES_CHESPIN, SPECIES_DEERLING_AUTUMN, SPECIES_ARBOLIVA, SPECIES_OGERPON_WELLSPRING_MASK, SPECIES_GARGANACL},
        [2] = {SPECIES_CAPSAKID, SPECIES_PUMPKABOO_M, SPECIES_SINISTCHA, SPECIES_PETILIL, SPECIES_APPLIN, SPECIES_FERROSEED, SPECIES_PUMPKABOO, SPECIES_TREECKO, SPECIES_PUMPKABOO_L, SPECIES_SUNKERN},
        [3] = {SPECIES_SNOVER, SPECIES_ARON, SPECIES_CARBINK, SPECIES_MINIOR_VIOLET, SPECIES_BELLSPROUT, SPECIES_TIRTOUGA, SPECIES_FOMANTIS, SPECIES_NUZLEAF, SPECIES_SILVALLY_GRASS, SPECIES_GEODUDE},
        [4] = {SPECIES_SKIPLOOM, SPECIES_MINIOR_VIOLET, SPECIES_EXEGGCUTE, SPECIES_SERVINE, SPECIES_ELECTRODE_H, SPECIES_LOMBRE, SPECIES_SWADLOON, SPECIES_MINIOR_GREEN, SPECIES_ANORITH, SPECIES_ROGGENROLA},
        [5] = {SPECIES_ROTOM_MOW, SPECIES_LYCANROC_N, SPECIES_GLOOM, SPECIES_QUILLADIN, SPECIES_LILEEP, SPECIES_CHERRIM, SPECIES_ARON, SPECIES_MINIOR_YELLOW, SPECIES_GROVYLE, SPECIES_CORSOLA},
        [6] = {SPECIES_EXEGGUTOR, SPECIES_GOLEM, SPECIES_TERRAKION, SPECIES_ARMALDO, SPECIES_RILLABOOM, SPECIES_EXEGGUTOR_A, SPECIES_LUNATONE, SPECIES_APPLETUN_GIGA, SPECIES_RHYPERIOR, SPECIES_SHAYMIN_SKY},
        [7] = {SPECIES_ABOMASNOW, SPECIES_DHELMISE, SPECIES_DECIDUEYE_H, SPECIES_AGGRON, SPECIES_OMASTAR, SPECIES_GOURGEIST, SPECIES_FLAPPLE, SPECIES_GIGALITH, SPECIES_APPLETUN, SPECIES_ROSERADE},
        [8] = {SPECIES_DREDNAW, SPECIES_CELEBI, SPECIES_NIHILEGO, SPECIES_SHIFTRY, SPECIES_WORMADAM, SPECIES_GOLEM_A, SPECIES_LEAFEON, SPECIES_FERROTHORN, SPECIES_TSAREENA, SPECIES_STAKATAKA},
        [9] = {SPECIES_ARCEUS_ROCK, SPECIES_RHYDON, SPECIES_ABOMASNOW, SPECIES_DREDNAW_GIGA, SPECIES_SHAYMIN_SKY, SPECIES_SOLROCK, SPECIES_NIHILEGO, SPECIES_REGIROCK, SPECIES_CARRACOSTA, SPECIES_DREDNAW},
    },
    [CLASS_AROMA_LADY] = {
        [0] = {SPECIES_OINKOLOGNE, SPECIES_URSALUNA_BLOODMOON, SPECIES_SHROODLE, SPECIES_YUNGOOS, SPECIES_SMOLIV, SPECIES_ZORUA_H, SPECIES_PIDGEY, SPECIES_TANDEMAUS, SPECIES_DUDUNSPARCE_THREE, SPECIES_SKITTY},
        [1] = {SPECIES_LILLIPUP, SPECIES_DEERLING_WINTER, SPECIES_PIDGEY, SPECIES_SKITTY, SPECIES_RATTATA_A, SPECIES_URSALUNA_BLOODMOON, SPECIES_STARLY, SPECIES_SMOLIV, SPECIES_FLETCHLING, SPECIES_MINCCINO},
        [2] = {SPECIES_HELIOPTILE, SPECIES_OINKOLOGNE_FEMALE, SPECIES_ZIGZAGOON_G, SPECIES_DUDUNSPARCE_THREE, SPECIES_STARLY, SPECIES_TANDEMAUS, SPECIES_DEERLING_WINTER, SPECIES_OINKOLOGNE, SPECIES_SKWOVET, SPECIES_PIKIPEK},
        [3] = {SPECIES_BIBAREL, SPECIES_RATICATE_A, SPECIES_WATCHOG, SPECIES_GUMSHOOS, SPECIES_PERSIAN, SPECIES_SLAKOTH, SPECIES_EEVEE_GIGA, SPECIES_JIGGLYPUFF, SPECIES_VIGOROTH, SPECIES_WOOLOO},
        [4] = {SPECIES_KECLEON, SPECIES_FARFETCHD, SPECIES_WOOLOO, SPECIES_WATCHOG, SPECIES_BUNEARY, SPECIES_VIGOROTH, SPECIES_TYPE_NULL, SPECIES_LOUDRED, SPECIES_FURRET, SPECIES_GUMSHOOS},
        [5] = {SPECIES_DODUO, SPECIES_SMEARGLE, SPECIES_GIRAFARIG, SPECIES_RUFFLET, SPECIES_SLAKOTH, SPECIES_SWABLU, SPECIES_PIDGEOTTO, SPECIES_EEVEE, SPECIES_WATCHOG, SPECIES_TRANQUILL},
        [6] = {SPECIES_SNORLAX, SPECIES_KANGASKHAN_MEGA, SPECIES_GREEDENT, SPECIES_UNFEZANT, SPECIES_FEAROW, SPECIES_EXPLOUD, SPECIES_MELOETTA, SPECIES_FURFROU_LA_REINE, SPECIES_URSARING, SPECIES_FURFROU_PHAROAH},
        [7] = {SPECIES_STANTLER, SPECIES_HAPPINY, SPECIES_KANGASKHAN, SPECIES_MELOETTA_PIROUETTE, SPECIES_FURFROU_DIAMOND, SPECIES_AMBIPOM, SPECIES_FURFROU_KABUKI, SPECIES_SAWSBUCK_SUMMER, SPECIES_PIDGEOT, SPECIES_URSARING},
        [8] = {SPECIES_FURFROU_DEBUTANTE, SPECIES_KOMALA, SPECIES_HELIOLISK, SPECIES_SAWSBUCK_SUMMER, SPECIES_PURUGLY, SPECIES_FURFROU_HEART, SPECIES_CHANSEY, SPECIES_DRAMPA, SPECIES_FURFROU_DANDY, SPECIES_TAUROS},
        [9] = {SPECIES_FEAROW, SPECIES_LOPUNNY, SPECIES_HELIOLISK, SPECIES_LOPUNNY_MEGA, SPECIES_HAPPINY, SPECIES_SAWSBUCK_WINTER, SPECIES_PORYGON2, SPECIES_SAWSBUCK_SUMMER, SPECIES_TAUROS, SPECIES_EXPLOUD},
    },
    [CLASS_RUIN_MANIAC] = {
        [0] = {SPECIES_ARBOLIVA, SPECIES_SMOLIV, SPECIES_ZIGZAGOON_G, SPECIES_DEERLING_AUTUMN, SPECIES_BUNNELBY, SPECIES_ZIGZAGOON, SPECIES_DEERLING, SPECIES_TERAPAGOS_TERASTAL, SPECIES_PATRAT, SPECIES_AZURILL},
        [1] = {SPECIES_LILLIPUP, SPECIES_SPEAROW, SPECIES_OINKOLOGNE_FEMALE, SPECIES_URSALUNA_BLOODMOON, SPECIES_BIDOOF, SPECIES_PIKIPEK, SPECIES_AZURILL, SPECIES_RATTATA, SPECIES_SHROODLE, SPECIES_PIDGEY},
        [2] = {SPECIES_MAUSHOLD_FOUR, SPECIES_HELIOPTILE, SPECIES_DUDUNSPARCE, SPECIES_EGG, SPECIES_CYCLIZAR, SPECIES_PIDOVE, SPECIES_SQUAWKABILLY, SPECIES_TERAPAGOS, SPECIES_DEERLING_SUMMER, SPECIES_DOLLIV},
        [3] = {SPECIES_GUMSHOOS, SPECIES_HERDIER, SPECIES_AIPOM, SPECIES_FURRET, SPECIES_DUNSPARCE, SPECIES_GLAMEOW, SPECIES_LINOONE, SPECIES_RUFFLET, SPECIES_SWABLU, SPECIES_BUNEARY},
        [4] = {SPECIES_VIGOROTH, SPECIES_HERDIER, SPECIES_SPINDA, SPECIES_CHATOT, SPECIES_SMEARGLE, SPECIES_CASTFORM, SPECIES_LINOONE_G, SPECIES_EEVEE_GIGA, SPECIES_LITLEO, SPECIES_FURRET},
        [5] = {SPECIES_GUMSHOOS, SPECIES_VIGOROTH, SPECIES_DODUO, SPECIES_TRANQUILL, SPECIES_AIPOM, SPECIES_LOUDRED, SPECIES_RATICATE_A, SPECIES_LINOONE, SPECIES_WOOLOO, SPECIES_JIGGLYPUFF},
        [6] = {SPECIES_FEAROW, SPECIES_PYROAR, SPECIES_PURUGLY, SPECIES_MELOETTA_PIROUETTE, SPECIES_SWELLOW, SPECIES_DODRIO, SPECIES_LOPUNNY_MEGA, SPECIES_KANGASKHAN_MEGA, SPECIES_KOMALA, SPECIES_SAWSBUCK_AUTUMN},
        [7] = {SPECIES_KANGASKHAN, SPECIES_UNFEZANT, SPECIES_URSALUNA, SPECIES_BRAVIARY, SPECIES_ORANGURU, SPECIES_TAUROS, SPECIES_GREEDENT, SPECIES_MELOETTA_PIROUETTE, SPECIES_PIDGEOT, SPECIES_FURFROU_HEART},
        [8] = {SPECIES_MELOETTA_PIROUETTE, SPECIES_LOPUNNY_MEGA, SPECIES_AUDINO, SPECIES_MILTANK, SPECIES_TAUROS, SPECIES_MELOETTA, SPECIES_DODRIO, SPECIES_URSALUNA, SPECIES_INDEEDEE, SPECIES_DRAMPA},
        [9] = {SPECIES_FURFROU_MATRON, SPECIES_AUDINO, SPECIES_CHANSEY, SPECIES_FURFROU_PHAROAH, SPECIES_KOMALA, SPECIES_DODRIO, SPECIES_REGIGIGAS, SPECIES_FURFROU_DEBUTANTE, SPECIES_CINCCINO, SPECIES_ZANGOOSE},
    },
    [CLASS_LADY] = {
        [0] = {SPECIES_UNOWN_G, SPECIES_BIDOOF, SPECIES_SMOLIV, SPECIES_UNOWN_R, SPECIES_ZIGZAGOON_G, SPECIES_GRAFAIAI, SPECIES_MAUSHOLD_FOUR, SPECIES_DUDUNSPARCE_THREE, SPECIES_UNOWN_U, SPECIES_SENTRET},
        [1] = {SPECIES_PIDGEY, SPECIES_INKAY, SPECIES_DITTO, SPECIES_UNOWN_I, SPECIES_DEERLING, SPECIES_PIDOVE, SPECIES_UNOWN_D, SPECIES_DOLLIV, SPECIES_UNOWN_T, SPECIES_UNOWN_J},
        [2] = {SPECIES_UNOWN_I, SPECIES_ESPATHRA, SPECIES_UNOWN_P, SPECIES_COSMOG, SPECIES_UNOWN_F, SPECIES_ZIGZAGOON, SPECIES_ELGYEM, SPECIES_WHISMUR, SPECIES_MUNKIDORI, SPECIES_TERAPAGOS_STELLAR},
        [3] = {SPECIES_GOTHORITA, SPECIES_SILVALLY_PSYCHIC, SPECIES_JIGGLYPUFF, SPECIES_WOOLOO, SPECIES_LICKITUNG, SPECIES_RALTS, SPECIES_BUNEARY, SPECIES_DELCATTY, SPECIES_TRUMBEAK, SPECIES_STUFFUL},
        [4] = {SPECIES_LINOONE, SPECIES_FURRET, SPECIES_SPOINK, SPECIES_VIGOROTH, SPECIES_SLAKOTH, SPECIES_RATICATE_A, SPECIES_HATTREM, SPECIES_SMOOCHUM, SPECIES_PERSIAN, SPECIES_EEVEE},
        [5] = {SPECIES_GOTHORITA, SPECIES_GUMSHOOS, SPECIES_BUNEARY, SPECIES_DUOSION, SPECIES_MUNCHLAX, SPECIES_DOTTLER, SPECIES_NATU, SPECIES_MEDITITE, SPECIES_DIGGERSBY, SPECIES_RAICHU_A},
        [6] = {SPECIES_HOOPA_UNBOUND, SPECIES_UNFEZANT, SPECIES_PONYTA_G, SPECIES_SLOWBRO_G, SPECIES_MR_RIME, SPECIES_MEWTWO, SPECIES_GOLDUCK, SPECIES_EXPLOUD, SPECIES_SIGILYPH, SPECIES_TAPU_LELE},
        [7] = {SPECIES_LUNALA, SPECIES_SILVALLY, SPECIES_SOLGALEO, SPECIES_HAPPINY, SPECIES_FURFROU, SPECIES_LUNATONE, SPECIES_URSALUNA, SPECIES_MEOWSTIC, SPECIES_MEWTWO, SPECIES_HATTERENE},
        [8] = {SPECIES_ARCEUS_PSYCHIC, SPECIES_PURUGLY, SPECIES_AZELF, SPECIES_FURFROU_DEBUTANTE, SPECIES_ORICORIO_P, SPECIES_SLOWKING_G, SPECIES_FURFROU_DANDY, SPECIES_SAWSBUCK, SPECIES_WOBBUFFET, SPECIES_HOOPA_UNBOUND},
        [9] = {SPECIES_CALYREX_SHADOW_RIDER, SPECIES_AUDINO_MEGA, SPECIES_LATIOS_MEGA, SPECIES_GALLADE, SPECIES_METANG, SPECIES_CALYREX_ICE_RIDER, SPECIES_LOPUNNY_MEGA, SPECIES_XATU, SPECIES_SLAKING, SPECIES_UNFEZANT},
    },
    [CLASS_PAINTER] = {
        [0] = {SPECIES_SQUAWKABILLY_WHITE, SPECIES_DEERLING_SUMMER, SPECIES_SCREAM_TAIL, SPECIES_ZIGZAGOON, SPECIES_ESPATHRA, SPECIES_MAUSHOLD_FOUR, SPECIES_RABSCA, SPECIES_UNOWN_U, SPECIES_VELUZA, SPECIES_GRAFAIAI},
        [1] = {SPECIES_FLITTLE, SPECIES_ZIGZAGOON, SPECIES_VELUZA, SPECIES_INKAY, SPECIES_TERAPAGOS_STELLAR, SPECIES_DUDUNSPARCE_THREE, SPECIES_ESPATHRA, SPECIES_CYCLIZAR, SPECIES_UNOWN_V, SPECIES_MEOWTH},
        [2] = {SPECIES_SQUAWKABILLY_YELLOW, SPECIES_LILLIPUP, SPECIES_OINKOLOGNE_FEMALE, SPECIES_SQUAWKABILLY_BLUE, SPECIES_DEERLING_SUMMER, SPECIES_DITTO, SPECIES_DEERLING, SPECIES_BIDOOF, SPECIES_STARLY, SPECIES_UNOWN_P},
        [3] = {SPECIES_AIPOM, SPECIES_CHINGLING, SPECIES_HATTREM, SPECIES_GLAMEOW, SPECIES_TRUMBEAK, SPECIES_TEDDIURSA, SPECIES_ESPURR, SPECIES_SPOINK, SPECIES_TYPE_NULL, SPECIES_HERDIER},
        [4] = {SPECIES_PIKACHU_PHD, SPECIES_HERDIER, SPECIES_HATTREM, SPECIES_GLAMEOW, SPECIES_BRONZOR, SPECIES_DROWZEE, SPECIES_TRUMBEAK, SPECIES_DODUO, SPECIES_NATU, SPECIES_VIGOROTH},
        [5] = {SPECIES_MR_MIME_G, SPECIES_LINOONE_G, SPECIES_PORYGON, SPECIES_CHIMECHO, SPECIES_PIKACHU_PHD, SPECIES_FARFETCHD, SPECIES_DELCATTY, SPECIES_TRUMBEAK, SPECIES_KECLEON, SPECIES_WOOLOO},
        [6] = {SPECIES_MR_RIME, SPECIES_MELOETTA_PIROUETTE, SPECIES_MEOWSTIC, SPECIES_ARCEUS, SPECIES_SILVALLY, SPECIES_SLOWBRO_MEGA, SPECIES_LOPUNNY_MEGA, SPECIES_FURFROU_STAR, SPECIES_FURFROU_MATRON, SPECIES_SWELLOW},
        [7] = {SPECIES_PORYGON2, SPECIES_GOLDUCK, SPECIES_HOOPA, SPECIES_VICTINI, SPECIES_SAWSBUCK_WINTER, SPECIES_TAUROS, SPECIES_BOUFFALANT, SPECIES_AMBIPOM, SPECIES_NECROZMA_DAWN_WINGS, SPECIES_SNORLAX_GIGA},
        [8] = {SPECIES_GALLADE_MEGA, SPECIES_PORYGON2, SPECIES_ARCEUS_PSYCHIC, SPECIES_FEAROW, SPECIES_METANG, SPECIES_HAPPINY, SPECIES_FURFROU_MATRON, SPECIES_SAWSBUCK_AUTUMN, SPECIES_SAWSBUCK_SUMMER, SPECIES_STOUTLAND},
        [9] = {SPECIES_SOLGALEO, SPECIES_FURFROU_KABUKI, SPECIES_PYROAR, SPECIES_LUNATONE, SPECIES_SLOWBRO_MEGA, SPECIES_STARMIE, SPECIES_STANTLER, SPECIES_BRAVIARY_H, SPECIES_VICTINI, SPECIES_DUBWOOL},
    },
};

#endif
static void SetEVSpread(struct Pokemon* mon, u8 hp, u8 atk, u8 def, u8 spa, u8 spdef, u8 spd){
	struct Pokemon* party = mon;
	party[0].hpEv = hp;	
	party[0].atkEv = atk;	
	party[0].defEv = def;
	party[0].spAtkEv = spa;
	party[0].spDefEv = spdef;
	party[0].spdEv = spd;
}

static void SetIVSpread(struct Pokemon* mon, u8 hp, u8 atk, u8 def, u8 spa, u8 spdef, u8 spd){
	struct Pokemon* party = mon;
	party[0].hpIV = hp;	
	party[0].attackIV = atk;	
	party[0].defenseIV = def;
	party[0].spAttackIV = spa;
	party[0].spDefenseIV = spdef;
	party[0].speedIV = spd;
}

static void SetAbilityFromEnum(struct Pokemon* mon, u8 abilityNum, u8 natureNum) {
	
	switch(abilityNum) {
		case Ability_Hidden:
		GIVE_HIDDEN_ABILITY:
			GiveMonNatureAndAbility(mon, natureNum, 0xFF, FALSE, TRUE, FALSE); //Give Hidden Ability
			break;
		case Ability_1:
		case Ability_2:
			GiveMonNatureAndAbility(mon, natureNum, MathMin(1, abilityNum - 1), FALSE, TRUE, FALSE);
			break;
		case Ability_Random_1_2:
		GIVE_RANDOM_ABILITY:
			GiveMonNatureAndAbility(mon, natureNum, Random() % 2, FALSE, TRUE, FALSE);
			break;
		case Ability_RandomAll: ;
			u8 random = Random() % 3;

			if (random == 2)
				goto GIVE_HIDDEN_ABILITY;

			goto GIVE_RANDOM_ABILITY;
	}
}
