#include "defines.h"
#include "defines_battle.h"
#include "../include/event_data.h"
#include "../include/pokemon.h"
#include "../include/pokemon_storage_system.h"
#include "../include/random.h"
#include "../include/wild_encounter.h"
#include "../include/constants/items.h"
#include "../include/constants/maps.h"
#include "../include/constants/pokedex.h"
#include "../include/constants/region_map_sections.h"
#include "../include/constants/trainers.h"

#include "../include/new/ability_tables.h"
#include "../include/new/AI_advanced.h"
#include "../include/new/build_pokemon.h"
#include "../include/new/build_pokemon_2.h"
#include "../include/new/catching.h"
#include "../include/new/dexnav.h"
#include "../include/new/form_change.h"
#include "../include/new/frontier.h"
#include "../include/new/Helper_Functions.h"
#include "../include/new/item.h"
#include "../include/new/learn_move.h"
#include "../include/new/mega.h"
#include "../include/new/multi.h"
#include "../include/new/pokemon_storage_system.h"

#include "Tables/Trainers_With_EVs_Table.h"
#include "Tables/Battle_Tower_Spreads.h"

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
	NUM_INDEX_CHECKS
};

struct TeamBuilder
{
	u16 speciesArray[PARTY_SIZE];
	u16 itemArray[PARTY_SIZE];
	bool8 speciesOnTeam[NATIONAL_DEX_COUNT];
	bool8 moveOnTeam[MOVES_COUNT];
	bool8 abilityOnTeam[ABILITIES_COUNT];
	bool8 itemEffectOnTeam[ITEM_EFFECT_COUNT];
	const struct BattleTowerSpread* spreads[PARTY_SIZE];
	u8 spreadClasses[PARTY_SIZE];
	u8 partyIndex[NUM_INDEX_CHECKS];
	u8 tier;
	u8 battleType;
	u8 monsCount;
	u8 numStalls;
	u8 numChoiceItems;
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
extern const species_t gRandomizerBanList[];
extern const species_t gSetPerfectXIvList[];
extern const species_t gDeerlingForms[];
extern const species_t gSawsbuckForms[];
extern const species_t gVivillonForms[];
extern const species_t gFurfrouForms[];
extern const species_t gFlabebeForms[];
extern const species_t gFloetteForms[];
extern const species_t gFlorgesForms[];
extern const species_t gPikachuCapForms[];
extern const u8 gNumDeerlingForms;
extern const u8 gNumSawsbuckForms;
extern const u8 gNumVivillonForms;
extern const u8 gNumFlabebeForms;
extern const u8 gNumFloetteForms;
extern const u8 gNumFlorgesForms;
extern const u8 gNumFurfrouForms;
extern const u8 gNumPikachuCapForms;

extern bool8 sp051_CanTeamParticipateInSkyBattle(void);
extern bool8 CanMonParticipateInASkyBattle(struct Pokemon* mon);

//This file's functions:
static u8 CreateNPCTrainerParty(struct Pokemon* const party, const u16 trainerNum, const bool8 firstTrainer, const bool8 side);
static u8 BuildFrontierParty(struct Pokemon* const party, const u16 trainerNum, const u8 tier, const bool8 firstTrainer, const bool8 forPlayer, const u8 side);
static void BuildFrontierMultiParty(u8 multiId);
static void CreateFrontierMon(struct Pokemon* mon, const u8 level, const struct BattleTowerSpread* spread, const u16 trainerId, const u8 trainerNum, const u8 trainerGender, const bool8 forPlayer);
static void SetWildMonHeldItem(void);
static u8 ConvertFrontierAbilityNumToAbility(const u8 abilityNum, const u16 species);
static u32 GetBaseStatsTotal(const u16 species);
static bool8 BaseStatsTotalGEAlreadyOnTeam(const u16 toCheck, const u8 partySize, u16* speciesArray);
static bool8 SpeciesAlreadyOnTeam(const u16 species, const u8 partySize, const species_t* const speciesArray);
static bool8 ItemAlreadyOnTeam(const u16 item, const u8 partySize, const item_t* const itemArray);
static void AdjustTypesForMegas(const u16 species, const u16 item, u8* const type1, u8* const type2);
static bool8 TeamNotAllSameType(const u16 species, const u16 item, const u8 partySize, const species_t* const speciesArray, const item_t* const itemArray);
static bool8 TooManyLegendariesOnGSCupTeam(const u16 species, const u8 partySize, const species_t* const speciesArray);
static bool8 PokemonTierBan(const u16 species, const u16 item, const struct BattleTowerSpread* const spread, const struct Pokemon* const mon, const u8 tier, const u8 checkFromLocationType);
static bool8 IsPokemonBannedBasedOnStreak(u16 species, u16 item, u16* speciesArray, u8 monsCount, u16 trainerId, u8 tier, bool8 forPlayer);
static bool8 TeamDoesntHaveSynergy(const struct BattleTowerSpread* const spread, const struct TeamBuilder* const builder);
static u16 GivePlayerFrontierMonGivenSpecies(const u16 species, const struct BattleTowerSpread* const spreadTable, const u16 numSpreads);
static const struct BattleTowerSpread* GetSpreadBySpecies(const u16 species, const struct BattleTowerSpread* const spreads, const u16 numSpreads);
static const struct BattleTowerSpread* TryAdjustSpreadForSpecies(const struct BattleTowerSpread* originalSpread);
static u16 TryAdjustAestheticSpecies(u16 species);
static void SwapMons(struct Pokemon* party, u8 i, u8 j);
static void PostProcessTeam(struct Pokemon* party, struct TeamBuilder* builder);
static u8 GetPartyIdFromPartyData(struct Pokemon* mon);
static u8 GetHighestMonLevel(const struct Pokemon* const party);

#ifdef OPEN_WORLD_TRAINERS

extern const u8 openWorldLevelRanges[NUM_BADGE_OPTIONS][2];
extern const species_t gGeneralTrainerSpreads[NUM_TRAINER_CLASSES][NUM_BADGE_OPTIONS][NUM_MONS_PER_BADGE];

static u8 GetOpenWorldTrainerMonAmount(void);
static u8 GetOpenWorldSpeciesIndex(u32 nameHash, u8 i);
static u8 GetOpenWorldSpeciesLevel(u32 nameHash, u8 i);
static u8 GetOpenWorldBadgeCount(void);

#endif

void BuildTrainerPartySetup(void) 
{
	u8 towerTier = VarGet(BATTLE_TOWER_TIER);
	gDontFadeWhite = FALSE;
	
	if (gBattleTypeFlags & (BATTLE_TYPE_TOWER_LINK_MULTI))
	{
		BuildFrontierParty(&gEnemyParty[0], gTrainerBattleOpponent_A, towerTier, TRUE, FALSE, B_SIDE_OPPONENT);
		BuildFrontierParty(&gEnemyParty[3], VarGet(SECOND_OPPONENT_VAR), towerTier, FALSE, FALSE, B_SIDE_OPPONENT);
	}
	else if (gBattleTypeFlags & BATTLE_TYPE_FRONTIER)
	{
		if (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS)
		{
			BuildFrontierParty(&gEnemyParty[0], gTrainerBattleOpponent_A, towerTier, TRUE, FALSE, B_SIDE_OPPONENT);
			BuildFrontierParty(&gEnemyParty[3], SECOND_OPPONENT, towerTier, FALSE, FALSE, B_SIDE_OPPONENT);
		}
		else
			BuildFrontierParty(&gEnemyParty[0], gTrainerBattleOpponent_A, towerTier, TRUE, FALSE, B_SIDE_OPPONENT);
			
		if (IsRandomBattleTowerBattle())
			BuildFrontierParty(gPlayerParty, 0, towerTier, TRUE, TRUE + 1, B_SIDE_PLAYER);
	}
	else if (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS)
	{
		CreateNPCTrainerParty(&gEnemyParty[0], gTrainerBattleOpponent_A, TRUE, B_SIDE_OPPONENT);
		CreateNPCTrainerParty(&gEnemyParty[3], SECOND_OPPONENT, FALSE, B_SIDE_OPPONENT);
	}
	else if (!(gBattleTypeFlags & BATTLE_TYPE_LINK))
	{
		if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)
			CreateNPCTrainerParty(&gEnemyParty[0], gTrainerBattleOpponent_A, TRUE, B_SIDE_OPPONENT);

		else if (!(gBattleTypeFlags & (BATTLE_TYPE_POKE_DUDE | BATTLE_TYPE_SCRIPTED_WILD_1)))
			SetWildMonHeldItem();
	}
	
	if (FlagGet(SKY_BATTLE_FLAG))
	{
		if (sp051_CanTeamParticipateInSkyBattle())
		{
			ExtensionState.skyBattlePartyBackup = Calloc(sizeof(struct Pokemon) * PARTY_SIZE);
			if (ExtensionState.skyBattlePartyBackup != NULL)
			{
				u8 counter = 0;
				for (int i = 0; i < PARTY_SIZE; ++i)
				{
					if (!CanMonParticipateInASkyBattle(&gPlayerParty[i]))
					{
						(ExtensionState.skyBattlePartyBackup)[counter++] = gPlayerParty[i];
						Memset(&gPlayerParty[i], 0x0, sizeof(struct Pokemon));
					}
				}
				CompactPartySlots();
			}
		}
	}

	if (gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER && ViableMonCount(gEnemyParty) > 1)
	{
		if (gBattleTypeFlags & BATTLE_TYPE_FRONTIER)
		{
			if (IsRandomBattleTowerBattle()
			|| GetMonData(&gPlayerParty[3], MON_DATA_SPECIES, NULL) == SPECIES_NONE) //Ideally the partner's Pokemon should be prepared beforehand. This is a backup measure
				BuildFrontierParty(&gPlayerParty[3], VarGet(PARTNER_VAR), towerTier, 3, FALSE, B_SIDE_PLAYER);
		}
		else
		{
			ExtensionState.partyBackup = Calloc(sizeof(struct Pokemon) * PARTY_SIZE);
			if (ExtensionState.partyBackup == NULL)
				return;

			if (gSelectedOrderFromParty[0] == 0)
				Memcpy(ExtensionState.partyBackup, &gPlayerParty[3], sizeof(struct Pokemon) * 3); //Special 0x2F was not used
			else //Special 0x2F was used
			{
				u8 counter = 0;
				u8 mon1 = gSelectedOrderFromParty[0];
				u8 mon2 = gSelectedOrderFromParty[1];
				u8 mon3 = gSelectedOrderFromParty[2];
				for (int i = 0; i < PARTY_SIZE; ++i) {
					if (i + 1 != mon1 && i + 1 != mon2 && i + 1 != mon3) //Don't backup selected mons
						Memcpy(&((struct Pokemon*) ExtensionState.partyBackup)[counter++], &gPlayerParty[i], sizeof(struct Pokemon));
				}
				ReducePartyToThree(); //Well...sometimes can be less than 3
			}
			Memset(&gPlayerParty[3], 0x0, sizeof(struct Pokemon) * 3);
			CreateNPCTrainerParty(&gPlayerParty[3], VarGet(PARTNER_VAR), FALSE, B_SIDE_PLAYER);
		}
	}

	if (ViableMonCount(gEnemyParty) <= 1) //Error prevention
		gBattleTypeFlags &= ~(BATTLE_TYPE_INGAME_PARTNER | BATTLE_TYPE_TWO_OPPONENTS | BATTLE_TYPE_DOUBLE);
}

extern void SortItemsInBag(u8 pocket, u8 type);
void sp067_GenerateRandomBattleTowerTeam(void)
{
	u8 tier;

	switch (Var8000) {
		case 0:
		default:
			tier = BATTLE_TOWER_STANDARD;
			break;

		case 1: //Legendary Pokemon
			tier = BATTLE_TOWER_NO_RESTRICTIONS;
			break;
			
		case 2: //Little Cup
			tier = BATTLE_TOWER_LITTLE_CUP;
			break;

		case 3: //Middle Cup
			tier = BATTLE_TOWER_MIDDLE_CUP;
			break;
			
		case 4: 
			tier = VarGet(BATTLE_TOWER_TIER);
			break;
	}

	VarSet(BATTLE_TOWER_TIER, tier);
	BuildFrontierParty(gPlayerParty, 0, tier, TRUE, TRUE, B_SIDE_PLAYER);


//////
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
		u8* name = ItemId_GetName(i);
		if (name[0] != 0xAC && name[0] != 0xFF) //'?', ' '
			AddBagItem(i, 1);
	}
	
	SortItemsInBag(0, 0);*/
}

//@Details: Adds a Pokemon with the given species from the requested spreads to
//			the player's party. If no space, then adds it to the PC.
//@Inputs:
//		Var8000: Species
//		Var8001: Spread Type
//@Returns: If the Pokemon was added or not.
u16 sp068_GivePlayerFrontierMonGivenSpecies(void)
{
	u16 numSpreads;
	u16 val;
	const struct BattleTowerSpread* spreads;
	
	u16 species = Var8000;

	switch (Var8001) {
		case 0:
		default:
			numSpreads = TOTAL_SPREADS;
			spreads = gFrontierSpreads;
			break;
		case 1:
			numSpreads = TOTAL_LITTLE_CUP_SPREADS;
			spreads = gLittleCupSpreads;
			break;
		case 2:
			numSpreads = TOTAL_MIDDLE_CUP_SPREADS;
			spreads = gMiddleCupSpreads;
			break;
		case 3:
			numSpreads = TOTAL_LEGENDARY_SPREADS;
			spreads = gFrontierLegendarySpreads;
			break;
		case 4: //Any Spread
			numSpreads = TOTAL_SPREADS;
			spreads = gFrontierSpreads;
			val = GivePlayerFrontierMonGivenSpecies(species, spreads, numSpreads);
			
			if (val != 0xFFFF)
				return val;
				
			numSpreads = TOTAL_LEGENDARY_SPREADS;
			spreads = gFrontierLegendarySpreads;
			val = GivePlayerFrontierMonGivenSpecies(species, spreads, numSpreads);
			
			if (val != 0xFFFF)
				return val;
				
			numSpreads = TOTAL_MIDDLE_CUP_SPREADS;
			spreads = gMiddleCupSpreads;
			val = GivePlayerFrontierMonGivenSpecies(species, spreads, numSpreads);
			
			if (val != 0xFFFF)
				return val;
				
			numSpreads = TOTAL_LITTLE_CUP_SPREADS;
			spreads = gLittleCupSpreads;
			break;
	}

	return GivePlayerFrontierMonGivenSpecies(species, spreads, numSpreads);
}

//@Details: Add a random Pokemon battleable in the given tier.
//@Inputs:
//		Var8000: Tier
//		Var8001: Spread Type
//@Returns: If the Pokemon was added or not.
u16 sp069_GivePlayerRandomFrontierMonByTier(void)
{
	u8 level, tier;
	u16 numSpreads;
	struct Pokemon mon;
	const struct BattleTowerSpread* spread;
	const struct BattleTowerSpread* spreads;

	switch (Var8001) {
		case 0:
		default:
			level = 50;
			numSpreads = TOTAL_SPREADS;
			spreads = gFrontierSpreads;
			tier = BATTLE_TOWER_STANDARD;
			break;

		case 1: //Legendary Pokemon
			level = 50;
			numSpreads = TOTAL_LEGENDARY_SPREADS;
			spreads = gFrontierLegendarySpreads;
			tier = BATTLE_TOWER_NO_RESTRICTIONS;
			break;
			
		case 2: //Little Cup
			level = 5;
			numSpreads = TOTAL_LITTLE_CUP_SPREADS;
			spreads = gLittleCupSpreads;
			tier = BATTLE_TOWER_LITTLE_CUP;
			break;

		case 3: //Middle Cup
			level = 50;
			numSpreads = TOTAL_MIDDLE_CUP_SPREADS;
			spreads = gMiddleCupSpreads;
			tier = BATTLE_TOWER_MIDDLE_CUP;
			break;
	}
	
	do
	{
		spread = TryAdjustSpreadForSpecies(&spreads[Random() % numSpreads]);
	} while (IsPokemonBannedBasedOnStreak(spread->species, spread->item, NULL, 0, 0, tier, TRUE)
		  || PokemonTierBan(spread->species, spread->item, spread, NULL, tier, CHECK_BATTLE_TOWER_SPREADS));
	
	CreateFrontierMon(&mon, level, spread, 0, 0, 0, TRUE);
	return GiveMonToPlayer(&mon);
}

u16 sp06A_GivePlayerFrontierMonByLoadedSpread(void)
{
	struct Pokemon mon;
	struct BattleTowerSpread* spread = (struct BattleTowerSpread*) gLoadPointer;

	CreateFrontierMon(&mon, Var8000, spread, 0, 0, 0, TRUE);
	return GiveMonToPlayer(&mon);
}

//@Details: Temporily replaces the player's team with the the team of the given multi
//			battle trainer to allow the player to choose which Pokemon they want the
//			partner to use.
//@Inputs:
//		Var8000: Given Id.
void sp06B_ReplacePlayerTeamWithMultiTrainerTeam(void)
{
	BackupPartyToTempTeam(0, 0, 3);
	BuildFrontierMultiParty(Var8000);
}

//Returns the number of Pokemon
static u8 CreateNPCTrainerParty(struct Pokemon* const party, const u16 trainerId, const bool8 firstTrainer, const bool8 side)
{
	u32 nameHash = 0;
	u32 personalityValue;
	int i, j;
	u8 monsCount = 1;
	u32 otid = Random32();

	if (trainerId == TRAINER_SECRET_BASE) return 0;

	struct Trainer* trainer = &gTrainers[trainerId];

	if (((gBattleTypeFlags & (BATTLE_TYPE_TRAINER | BATTLE_TYPE_EREADER_TRAINER | BATTLE_TYPE_TRAINER_TOWER)) == BATTLE_TYPE_TRAINER)
	||   (gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER))
	{
		if (firstTrainer)
			ZeroEnemyPartyMons();

		if (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS && side == B_SIDE_OPPONENT)
		{
			#ifdef OPEN_WORLD_TRAINERS
			if ((firstTrainer && gTrainerBattleOpponent_A < DYNAMIC_TRAINER_LIMIT)
			||  (!firstTrainer && VarGet(SECOND_OPPONENT_VAR) < DYNAMIC_TRAINER_LIMIT))
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

		u8 trainerNameLengthOddness = StringLength(trainer->trainerName) & 1;
		for (i = 0; i < monsCount; ++i)
		{
			gBankAttacker = i + 1;
			u8 genderOffset = 0x80;

			if ((i + 1) % 3 == 0) //Every third Pokemon
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

			for (j = 0; trainer->trainerName[j] != EOS; ++j)
				nameHash += trainer->trainerName[j];

			#ifdef OPEN_WORLD_TRAINERS
			u8 openWorldSpeciesIndex = GetOpenWorldSpeciesIndex(nameHash, i);
			u8 openWorldLevel = GetOpenWorldSpeciesLevel(nameHash, i);

			if (gTrainerBattleOpponent_A < DYNAMIC_TRAINER_LIMIT)
			{
				u16 speciesToCreate = gGeneralTrainerSpreads[trainer->trainerClass][GetOpenWorldBadgeCount()][openWorldSpeciesIndex];

				if (FlagGet(SCALE_TRAINER_LEVELS_FLAG) || (gBattleTypeFlags & BATTLE_TYPE_TRAINER_TOWER))																					//
					openWorldLevel = GetHighestMonLevel(gPlayerParty);

				CreateMon(&party[i], speciesToCreate, openWorldLevel, STANDARD_IV, TRUE, personalityValue, OT_ID_PRESET, otid);
			}
			else
			#endif
			{
				switch (gTrainers[trainerId].partyFlags) {
					case 0:
						MAKE_POKEMON(trainer->party.NoItemDefaultMoves);
						break;

					case PARTY_FLAG_CUSTOM_MOVES:
						MAKE_POKEMON(trainer->party.NoItemCustomMoves);
						SET_MOVES(trainer->party.NoItemCustomMoves);
						break;

					case PARTY_FLAG_HAS_ITEM:
						MAKE_POKEMON(trainer->party.ItemDefaultMoves);
						SetMonData(&party[i], MON_DATA_HELD_ITEM, &trainer->party.ItemDefaultMoves[i].heldItem);
						break;

					case PARTY_FLAG_CUSTOM_MOVES | PARTY_FLAG_HAS_ITEM:
						MAKE_POKEMON(trainer->party.ItemCustomMoves);
						SET_MOVES(trainer->party.ItemCustomMoves);
						SetMonData(&party[i], MON_DATA_HELD_ITEM, &trainer->party.ItemCustomMoves[i].heldItem);
						break;
				}
			}

			u8 otGender = trainer->gender;
			SetMonData(&party[i], REQ_OTGENDER, &otGender);
			SetMonData(&party[i], REQ_OTNAME, &trainer->trainerName);
			party[i].metLocation = gCurrentMapName;
			party[i].obedient = 1;

			#ifdef TRAINER_CLASS_POKE_BALLS
				SetMonData(&party[i], REQ_POKEBALL, &gClassPokeBalls[trainer->trainerClass]);
			#endif

			#ifdef TRAINERS_WITH_EVS
				u8 spreadNum = trainer->party.NoItemCustomMoves[i].iv;
				if (gTrainers[trainerId].partyFlags == (PARTY_FLAG_CUSTOM_MOVES | PARTY_FLAG_HAS_ITEM)
				&& trainer->aiFlags > 1
				&& spreadNum != 0
				&& spreadNum < ARRAY_COUNT(gTrainersWithEvsSpreads))
				{
					const struct TrainersWithEvs* spread = &gTrainersWithEvsSpreads[spreadNum];

					SET_EVS(spread);
					SET_IVS_SINGLE_VALUE(MathMin(31, spread->ivs));

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
							ballType = MathMin(LAST_BALL_INDEX, spread->ball);
					}

					SetMonData(&party[i], REQ_POKEBALL, &ballType);

					switch(spread->ability) {
						case Ability_Hidden:
						TRAINER_WITH_EV_GIVE_HIDDEN_ABILITY:
							GiveMonNatureAndAbility(&party[i], spread->nature, 0xFF, FALSE); //Give Hidden Ability
							break;
						case Ability_1:
						case Ability_2:
							GiveMonNatureAndAbility(&party[i], spread->nature, MathMin(1, spread->ability - 1), FALSE);
							break;
						case Ability_Random_1_2:
						TRAINER_WITH_EV_GIVE_RANDOM_ABILITY:
							GiveMonNatureAndAbility(&party[i], spread->nature, Random() % 2, FALSE);
							break;
						case Ability_RandomAll: ;
							u8 random = Random() % 3;

							if (random == 2)
								goto TRAINER_WITH_EV_GIVE_HIDDEN_ABILITY;
							else
								goto TRAINER_WITH_EV_GIVE_RANDOM_ABILITY;
							break;
					}
				}
			#endif

			CalculateMonStatsNew(&party[i]);
			HealMon(&party[i]);
			
			//Status Inducers
			TryStatusInducer(&party[i]);
			gBankTarget = i + 1;
		}
		#ifdef OPEN_WORLD_TRAINERS
		if ((GetOpenWorldTrainerMonAmount() > 1 || trainer->doubleBattle)
		#else
		if (trainer->partySize > 1
		#endif
		&& ViableMonCount(gPlayerParty) >= 2) //Double battles will not happen if the player only has 1 mon that can fight or if the foe only has 1 poke
		{
			gBattleTypeFlags |= trainer->doubleBattle;

			#ifdef DOUBLE_BATTLE_FLAG
				if (FlagGet(DOUBLE_BATTLE_FLAG))
					gBattleTypeFlags |= BATTLE_TYPE_DOUBLE;
			#endif
		}

	}
	return monsCount;
}

//Returns the number of Pokemon
static u8 BuildFrontierParty(struct Pokemon* const party, const u16 trainerId, const u8 tier, const bool8 firstTrainer, const bool8 forPlayer, const u8 side) 
{
	u32 i, j;
	u8 monsCount;
	u8 rand;
	
	u8 trainerGender = 0;
	u8 battleTowerPokeNum = VarGet(BATTLE_TOWER_POKE_NUM);
	u8 battleType = VarGet(BATTLE_TOWER_BATTLE_TYPE);
	u8 level = GetBattleTowerLevel(tier);
	u16 tableId = VarGet(TOWER_TRAINER_ID_VAR + (firstTrainer ^ 1));

	if (!forPlayer) 
	{
		if (trainerId == 0x400)
			return 0;
		else if (trainerId != BATTLE_TOWER_TID 
			  && trainerId != BATTLE_TOWER_SPECIAL_TID
			  && trainerId != FRONTIER_BRAIN_TID
			  && trainerId != BATTLE_TOWER_MULTI_TRAINER_TID)
		{
			return (CreateNPCTrainerParty(party, trainerId, firstTrainer, side));
		}
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
	
	for (i = 0; i < NUM_INDEX_CHECKS; ++i)
		builder->partyIndex[i] = 0xFF;

	for (i = 0; i < monsCount; ++i) 
	{
		u8 loop = 1;
		u16 species, dexNum, item;
		u8 ability, itemEffect, class;
		const struct BattleTowerSpread* spread = NULL;

		do 
		{
			switch (trainerId) {
				case BATTLE_TOWER_SPECIAL_TID:
				case FRONTIER_BRAIN_TID:
					switch (tier) {
						case BATTLE_TOWER_UBER:
						case BATTLE_TOWER_NO_RESTRICTIONS:
						case BATTLE_TOWER_UBER_CAMOMONS:
						SPECIAL_TRAINER_LEGENDARY_SPREADS:
							if (specialTrainer->legendarySpreads != NULL)
								spread = &specialTrainer->legendarySpreads[Random() % specialTrainer->legSpreadSize];
							else
								goto REGULAR_LEGENDARY_SPREADS;
							break;
						case BATTLE_TOWER_LITTLE_CUP:
						case BATTLE_TOWER_LC_CAMOMONS:
						SPECIAL_TRAINER_LITTLE_SPREADS:
							if (specialTrainer->littleCupSpreads != NULL)
								spread = &specialTrainer->littleCupSpreads[Random() % specialTrainer->lcSpreadSize];
							else
								goto REGULAR_LC_SPREADS;
							break;
						case BATTLE_TOWER_MIDDLE_CUP:
						case BATTLE_TOWER_MC_CAMOMONS:
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
						case BATTLE_TOWER_SCALEMONS: ;
							if (trainerId == FRONTIER_BRAIN_TID && IN_BATTLE_MINE)
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
								case 6:
									goto SPECIAL_TRAINER_LEGENDARY_SPREADS;
							}
							goto SPECIAL_TRAINER_REGULAR_SPREADS;
						case BATTLE_TOWER_350_CUP: ;
							if (trainerId == FRONTIER_BRAIN_TID && IN_BATTLE_MINE)
								goto SPECIAL_TRAINER_LITTLE_SPREADS; 
						
							rand = Random() & 3;
							switch (rand) {
								case 0:
								case 1:
									goto SPECIAL_TRAINER_LITTLE_SPREADS;
								case 2:
									goto SPECIAL_TRAINER_LEGENDARY_SPREADS;
							}
							goto SPECIAL_TRAINER_REGULAR_SPREADS;
						case BATTLE_TOWER_AVERAGE_MONS: ;
							if (trainerId == FRONTIER_BRAIN_TID && IN_BATTLE_MINE)
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
				case BATTLE_TOWER_MULTI_TRAINER_TID: //This will only get called if the Player's party was not set up properly beforehand
					if (IsRandomBattleTowerBattle())
						goto GENERIC_RANDOM_SPREADS; //Generate random Pokemon for the partner in random battles

					switch (tier) {
						case BATTLE_TOWER_UBER:
						case BATTLE_TOWER_NO_RESTRICTIONS:
						case BATTLE_TOWER_UBER_CAMOMONS:
						MULTI_PARTNER_LEGENDARY_SPREADS:
							if (multiPartner->legendarySpreads != NULL)
								spread = &multiPartner->legendarySpreads[Random() % multiPartner->legSpreadSize];
							else
								goto REGULAR_LEGENDARY_SPREADS;
							break;
						case BATTLE_TOWER_LITTLE_CUP:
						case BATTLE_TOWER_LC_CAMOMONS:
						MULTI_PARTNER_LITTLE_SPREADS:
							if (multiPartner->littleCupSpreads != NULL)
								spread = &multiPartner->littleCupSpreads[Random() % multiPartner->lcSpreadSize];
							else
								goto REGULAR_LC_SPREADS;
							break;
						case BATTLE_TOWER_MIDDLE_CUP:
						case BATTLE_TOWER_MC_CAMOMONS:
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
						case BATTLE_TOWER_SCALEMONS: ;
							rand = Random() & 7;
							switch (rand) {
								case 0: //High prevalence of baby spreads b/c they
								case 1: //get insane stats in Scalemons
								case 2:
								case 3:
								case 4:
								case 5:
									goto MULTI_PARTNER_LITTLE_SPREADS;
								case 6:
									goto MULTI_PARTNER_LEGENDARY_SPREADS;
							}
							goto MULTI_PARTNER_REGULAR_SPREADS;
						case BATTLE_TOWER_350_CUP: ;
							rand = Random() & 3;
							switch (rand) {
								case 0:
								case 1:
									goto MULTI_PARTNER_LITTLE_SPREADS;
								case 2:
									goto MULTI_PARTNER_LEGENDARY_SPREADS;
							}
							goto MULTI_PARTNER_REGULAR_SPREADS;
						case BATTLE_TOWER_AVERAGE_MONS: ;
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
						case BATTLE_TOWER_UBER:
						case BATTLE_TOWER_NO_RESTRICTIONS:
						case BATTLE_TOWER_UBER_CAMOMONS:
							if (Random() % 100 < 5) //5% chance per mon of not being legendary
								spread = &gFrontierSpreads[Random() % TOTAL_SPREADS];
							else
							REGULAR_LEGENDARY_SPREADS:
								spread = &gFrontierLegendarySpreads[Random() % TOTAL_LEGENDARY_SPREADS];
							break;
						case BATTLE_TOWER_LITTLE_CUP:
						case BATTLE_TOWER_LC_CAMOMONS:
						REGULAR_LC_SPREADS:
							spread = &gLittleCupSpreads[Random() % TOTAL_LITTLE_CUP_SPREADS];
							break;
						case BATTLE_TOWER_MIDDLE_CUP:
						case BATTLE_TOWER_MC_CAMOMONS:
							if (!IsFrontierSingles(battleType)) //Doubles - GS Cup
							{
								if ((Random() & 1) == 0)
									goto REGULAR_LEGENDARY_SPREADS;
							
								goto REGULAR_SPREADS;
							}

						REGULAR_MC_SPREADS:
							spread = &gMiddleCupSpreads[Random() % TOTAL_MIDDLE_CUP_SPREADS];
							break;
						case BATTLE_TOWER_OU:
						case BATTLE_TOWER_MONOTYPE:
						case BATTLE_TOWER_CAMOMONS:
							//25% chance of trying to use a legend allowed in these tiers
							if ((Random() & 3) == 0)
								goto REGULAR_LEGENDARY_SPREADS;
								
							goto REGULAR_SPREADS;
						case BATTLE_TOWER_SCALEMONS: ;
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
								case 6:
									goto REGULAR_LEGENDARY_SPREADS;
							}
							goto REGULAR_SPREADS;
						case BATTLE_TOWER_350_CUP: ;
							rand = Random() & 3;
							switch (rand) {
								case 0:
								case 1:
									goto REGULAR_LC_SPREADS;
								case 2:
									goto REGULAR_LEGENDARY_SPREADS;
							}
							goto REGULAR_SPREADS;
						case BATTLE_TOWER_AVERAGE_MONS: ;
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
						case BATTLE_TOWER_STANDARD:
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
						case BATTLE_TOWER_BENJAMIN_BUTTERFREE: //Don't use legends even though you can
						default:
						REGULAR_SPREADS:
							spread = &gFrontierSpreads[Random() % TOTAL_SPREADS];
							break;
					}
					
					spread = TryAdjustSpreadForSpecies(spread); //Update Arceus
					break;
					
				default: //forPlayer
					switch (tier) {
						case BATTLE_TOWER_UBER:
						case BATTLE_TOWER_NO_RESTRICTIONS:
						case BATTLE_TOWER_UBER_CAMOMONS:
							goto REGULAR_LEGENDARY_SPREADS;
						case BATTLE_TOWER_LITTLE_CUP:
						case BATTLE_TOWER_LC_CAMOMONS:
							goto REGULAR_LC_SPREADS;
						case BATTLE_TOWER_MIDDLE_CUP:
						case BATTLE_TOWER_MC_CAMOMONS:
							if (!IsFrontierSingles(battleType)) //Doubles - GS Cup
							{
								if ((Random() & 1) == 0)
									goto REGULAR_LEGENDARY_SPREADS;
							
								goto REGULAR_SPREADS;
							}
							goto REGULAR_MC_SPREADS;
						case BATTLE_TOWER_OU:
						case BATTLE_TOWER_MONOTYPE:
						case BATTLE_TOWER_CAMOMONS:
							//25% chance of trying to use a legend allowed in these tiers
							if ((Random() & 3) == 0)
								goto REGULAR_LEGENDARY_SPREADS;

							goto REGULAR_SPREADS;
						case BATTLE_TOWER_SCALEMONS: ;
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
								case 6:
									goto REGULAR_LEGENDARY_SPREADS;
							}
							goto REGULAR_SPREADS;
						case BATTLE_TOWER_350_CUP: ;
							rand = Random() & 3;
							switch (rand) {
								case 0:
								case 1:
									goto REGULAR_LC_SPREADS;
								case 2:
									goto REGULAR_LEGENDARY_SPREADS;
							}
							goto REGULAR_SPREADS;
						case BATTLE_TOWER_AVERAGE_MONS: ;
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
			ability = ConvertFrontierAbilityNumToAbility(spread->ability, species);
			itemEffect = (ability != ABILITY_KLUTZ) ? ItemId_GetHoldEffect(spread->item) : 0;
			
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
			
			//Prevent duplicate species and items
			//Only allow one Mega Stone & Z-Crystal per team
			if (!IsPokemonBannedBasedOnStreak(species, item, builder->speciesArray, monsCount, trainerId, tier, forPlayer)
			&& (!builder->speciesOnTeam[dexNum] || tier == BATTLE_TOWER_NO_RESTRICTIONS)
			&& (!ItemAlreadyOnTeam(item, monsCount, builder->itemArray) || tier == BATTLE_TOWER_NO_RESTRICTIONS)
			&& (itemEffect != ITEM_EFFECT_MEGA_STONE || item == ITEM_ULTRANECROZIUM_Z || !builder->itemEffectOnTeam[ITEM_EFFECT_MEGA_STONE])
			&& ((itemEffect != ITEM_EFFECT_Z_CRYSTAL && item != ITEM_ULTRANECROZIUM_Z) || !builder->itemEffectOnTeam[ITEM_EFFECT_Z_CRYSTAL])
			&& !PokemonTierBan(species, item, spread, NULL, tier, CHECK_BATTLE_TOWER_SPREADS)
			&& !(tier == BATTLE_TOWER_MONOTYPE && TeamNotAllSameType(species, item, monsCount, builder->speciesArray, builder->itemArray))
			&& !(tier == BATTLE_TOWER_GS_CUP && !IsFrontierSingles(battleType) && TooManyLegendariesOnGSCupTeam(species, monsCount, builder->speciesArray))
			&& !((trainerId == BATTLE_TOWER_TID || forPlayer || (trainerId == BATTLE_TOWER_MULTI_TRAINER_TID && IsRandomBattleTowerBattle())) && TeamDoesntHaveSynergy(spread, builder)))
			{
				class = PredictFightingStyle(spread->moves, ability, itemEffect, 0xFF);

				builder->spreads[i] = spread;
				builder->speciesArray[i] = species;
				builder->itemArray[i] = item;
				builder->abilityOnTeam[ability] = TRUE;
				
				if (item == ITEM_ULTRANECROZIUM_Z)
					builder->itemEffectOnTeam[ITEM_EFFECT_Z_CRYSTAL] = TRUE; //Treat like a Z-Crystal
				else
					builder->itemEffectOnTeam[itemEffect] = TRUE;
					
				if (itemEffect == ITEM_EFFECT_CHOICE_BAND)
					++builder->numChoiceItems;
				
				builder->speciesOnTeam[dexNum] = TRUE;
				for (j = 0; j < MAX_MON_MOVES; ++j)
					builder->moveOnTeam[spread->moves[j]] = TRUE;

				if (spread->spdEv >= 20)
					builder->partyIndex[FAST_MON] = i;
					
				if (gAbilityRatings[ability] < 0)
					builder->partyIndex[BAD_ABILITY] = i;
					
				if (IsClassStall(class) || IsClassDoublesUtility(class) || IsClassDoublesTeamSupport(class))
					++builder->numStalls;
				else if (IsClassBatonPass(class))
					builder->partyIndex[BATON_PASSER] = i;
				else if (IsClassScreener(class))
					builder->partyIndex[SCREENER] = i;
				else if (IsClassCleric(class))
					builder->partyIndex[CLERIC] = i;
				else if (IsClassEntryHazards(class))
					builder->partyIndex[HAZARDS_SETUP] = i;

				if (!IsFrontierSingles(battleType)) //Doubles or Multi
				{
					switch (ability) {
						case ABILITY_VOLTABSORB:
						case ABILITY_MOTORDRIVE:
						case ABILITY_LIGHTNINGROD:
							builder->partyIndex[ELECTRIC_IMMUNITY] = i;
							break;
						
						case ABILITY_WATERABSORB:
						case ABILITY_DRYSKIN:
						case ABILITY_STORMDRAIN:
							builder->partyIndex[WATER_IMMUNITY] = i;
							break;

						case ABILITY_FLASHFIRE:
							builder->partyIndex[FIRE_IMMUNITY] = i;
							break;

						case ABILITY_SAPSIPPER:
							builder->partyIndex[GRASS_IMMUNITY] = i;
							break;

						case ABILITY_LEVITATE:
							if (itemEffect != ITEM_EFFECT_IRON_BALL)
								builder->partyIndex[GROUND_IMMUNITY] = i;
							break;

						case ABILITY_SOUNDPROOF:
							builder->partyIndex[SOUND_IMMUNITY] = i;
							break;

						case ABILITY_JUSTIFIED:
							builder->partyIndex[JUSTIFIED_BOOSTED] = i;
							break;
					}
					
					u8 typeDmg;
					u8 defType1 = (gBattleTypeFlags & BATTLE_TYPE_CAMOMONS) ? GetCamomonsTypeBySpread(spread, 0) : gBaseStats[species].type1;
					u8 defType2 = (gBattleTypeFlags & BATTLE_TYPE_CAMOMONS) ? GetCamomonsTypeBySpread(spread, 1) : gBaseStats[species].type2;

					for (j = 0; j < ARRAY_COUNT(sImmunities); ++j)
					{
						typeDmg = 10;
						ModulateByTypeEffectiveness(sImmunities[j].type, defType1, defType2, &typeDmg);
						if (typeDmg == 0 && builder->partyIndex[sImmunities[j].index] == 0xFF)
							builder->partyIndex[sImmunities[j].index] = i;
					}
				}

				loop = 0;
			}
		} while (loop == 1);

		CreateFrontierMon(&party[i], level, spread, trainerId, firstTrainer ^ 1, trainerGender, forPlayer);
	}

	if (forPlayer)
		PostProcessTeam(gPlayerParty, builder);
	else
		PostProcessTeam(gEnemyParty, builder);

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
	u8 tier = VarGet(BATTLE_TOWER_TIER);
	const struct BattleTowerSpread* spread = NULL;
	const struct MultiBattleTowerTrainer* multiPartner = &gFrontierMultiBattleTrainers[multiId];
	u8 idOnTeam[multiPartner->regSpreadSize];
	
	//Clear Values
	for (i = 0; i < multiPartner->regSpreadSize; ++i)
		idOnTeam[i] = FALSE;

	ZeroPlayerPartyMons();

	//Build Team
	for (int i = 0; i < PARTY_SIZE; ++i)
	{
		switch (tier) {
			case BATTLE_TOWER_UBER:
			case BATTLE_TOWER_NO_RESTRICTIONS:
			case BATTLE_TOWER_UBER_CAMOMONS:
				if (multiPartner->legendarySpreads != NULL && i < multiPartner->legSpreadSize)
					spread = &multiPartner->legendarySpreads[i];
				else
					return; //No Pokemon data to load
				break;
			case BATTLE_TOWER_LITTLE_CUP:
			case BATTLE_TOWER_LC_CAMOMONS:
				if (multiPartner->littleCupSpreads != NULL && i < multiPartner->lcSpreadSize)
					spread = &multiPartner->littleCupSpreads[i];
				else
					return; //No Pokemon data to load
				break;
			case BATTLE_TOWER_MIDDLE_CUP:
			case BATTLE_TOWER_MC_CAMOMONS:
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
					spread = &multiPartner->regularSpreads[i];
				else
					return; //No Pokemon data to load
		}

		CreateFrontierMon(&gPlayerParty[i], GetBattleTowerLevel(tier), spread, BATTLE_TOWER_MULTI_TRAINER_TID, 2, multiPartner->gender, FALSE);
	}
}

static void CreateFrontierMon(struct Pokemon* mon, const u8 level, const struct BattleTowerSpread* spread, const u16 trainerId, const u8 trainerNum, const u8 trainerGender, const bool8 forPlayer)
{
	int i, j;
	
	u16 species = TryAdjustAestheticSpecies(spread->species);
	u32 otId;
	
	if (trainerId == BATTLE_TOWER_MULTI_TRAINER_TID)
		otId = gFrontierMultiBattleTrainers[VarGet(TOWER_TRAINER_ID_PARTNER_VAR)].otId;
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
		mon->otGender = trainerGender;
	}

	#ifdef UNBOUND
		mon->metLocation = MAPSEC_BATTLE_FRONTIER;
	#else
		mon->metLocation = MAPSEC_TRAINER_TOWER;
	#endif

	mon->metLevel = level;
	mon->obedient = TRUE;
	mon->friendship = 255;

	i = 0;
	struct Pokemon* party = mon;

	SET_IVS(spread);
	SET_EVS(spread);

	if (spread->ability > FRONTIER_ABILITY_HIDDEN)
	{
		GiveMonNatureAndAbility(mon, spread->nature, spread->ability - 1, spread->shiny);
	}
	else //Hidden Ability
	{ 
		GiveMonNatureAndAbility(mon, spread->nature, 0xFF, spread->shiny);
		mon->hiddenAbility = TRUE;
	}

	for (j = 0; j < MAX_MON_MOVES; j++) 
	{
		mon->moves[j] = spread->moves[j];
		mon->pp[j] = gBattleMoves[spread->moves[j]].pp;
	}

	SetMonData(mon, MON_DATA_HELD_ITEM, &spread->item);

	u8 ballType;
	if (spread->ball != BALL_TYPE_RANDOM)
		ballType = MathMin(LAST_BALL_INDEX, spread->ball);
	else
		ballType = umodsi(Random(), NUM_BALLS);
	SetMonData(mon, REQ_POKEBALL, &ballType);

	TryFormRevert(mon); //To fix Minior forms
	CalculateMonStatsNew(mon);
	HealMon(mon);
}

static void SetWildMonHeldItem(void)
{
	u16 rnd = umodsi(Random(), 100);
	u16 species;
	u16 var1 = 45;
	u16 var2 = 95;

	if (!GetMonData(&gPlayerParty[0], MON_DATA_IS_EGG, 0)
	&& GetPartyAbility(&gPlayerParty[0]) == ABILITY_COMPOUNDEYES) 
	{
		var1 = 20;
		var2 = 80;
	}

	if (!(gBattleTypeFlags & (BATTLE_TYPE_POKE_DUDE | BATTLE_TYPE_SCRIPTED_WILD_1 | BATTLE_TYPE_TRAINER)))
	{
		for (int i = 0; i < 2; ++i) //Two possible wild opponents
		{
			if (i > 0 && !IS_DOUBLE_BATTLE)
				break;

			species = gEnemyParty[i].species;

			if (gBaseStats[species].item1 == gBaseStats[species].item2 && gBaseStats[species].item1 != 0)
			{
				SetMonData(&gEnemyParty[i], MON_DATA_HELD_ITEM, &gBaseStats[species].item1);
				continue;
			}

			if (rnd < var1)
				continue;

			if (rnd < var2)
				SetMonData(&gEnemyParty[i], MON_DATA_HELD_ITEM, &gBaseStats[species].item1);
			else
				SetMonData(&gEnemyParty[i], MON_DATA_HELD_ITEM, &gBaseStats[species].item2);
		}
	}
}

void GiveMonNatureAndAbility(struct Pokemon* mon, u8 nature, u8 abilityNum, bool8 forceShiny)
{
	u32 personality;
	u32 trainerId = GetMonData(mon, MON_DATA_OT_ID, NULL);
	u16 sid = HIHALF(trainerId);
	u16 tid = LOHALF(trainerId);

	do
	{
		personality = Random32();
		if (forceShiny)
		{
			u8 shinyRange = RandRange(0,8);
			personality = (((shinyRange ^ (sid ^ tid)) ^ LOHALF(personality)) << 16) | LOHALF(personality);
		}

		if (abilityNum != 0xFF)
		{
			personality &= ~(1);
			personality |= MathMin(1, abilityNum); //Either 0 or 1
		}

	} while (GetNatureFromPersonality(personality) != nature);
	
	mon->personality = personality;
}

static u8 ConvertFrontierAbilityNumToAbility(const u8 abilityNum, const u16 species)
{
	u8 ability = ABILITY_NONE;

	switch (abilityNum) {
		case FRONTIER_ABILITY_1:
			ability = gBaseStats[species].ability1;
			break;
		case FRONTIER_ABILITY_2:
			ability = gBaseStats[species].ability2;
			break;
		case FRONTIER_ABILITY_HIDDEN:
			ability = gBaseStats[species].hiddenAbility;
			break;
	}
			
	if (ability == ABILITY_NONE)
		ability = gBaseStats[species].ability1;
		
	return ability;
}

static u32 GetBaseStatsTotal(const u16 species)
{
	u32 sum = 0;
	u8* ptr = &gBaseStats[species].baseHP;
	
	for (int i = 0; i < NUM_STATS; ++i)
		sum += ptr[i];

	return sum;
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
	u16 megaSpecies = GetMegaSpecies(species, item);
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
	bool8 isMulti = IsFrontierMulti(VarGet(BATTLE_TOWER_BATTLE_TYPE));

	if (!CheckTableForSpecies(species, gGSCup_LegendarySpeciesList))
		return FALSE; //Allowed normally so we don't care

	for (int i = 0; i < partySize; ++i) 
	{
		if (speciesArray[i] == SPECIES_NONE)
			continue;
			
		if (CheckTableForSpecies(speciesArray[i], gGSCup_LegendarySpeciesList))
			++legendCount;

		if (isMulti && legendCount >= 1) //1 legendary per multi trainer
			return TRUE;

		if (legendCount >= 2) //2 Legendaries allowed at most
			return TRUE;
	}
	
	return FALSE;
}

static bool8 PokemonTierBan(const u16 species, const u16 item, const struct BattleTowerSpread* const spread, const struct Pokemon* const mon, const u8 tier, const u8 checkFromLocationType)
{
	u32 i;
	u8 ability;
	const u16* moveLoc;

	if (species == SPECIES_EGG)
		return 1;

	u16 battleFormat = VarGet(BATTLE_TOWER_BATTLE_TYPE);

	switch (tier) {
		case BATTLE_TOWER_STANDARD:
			if (CheckTableForSpecies(species, gBattleTowerStandardSpeciesBanList)
			||  CheckTableForItem(item, gBattleTowerStandard_ItemBanList))
				return TRUE;
			break;

		case BATTLE_TOWER_OU:
		STANDARD_OU_CHECK:
		//For OU, there's a species, item, ability, and move ban list
			//Load correct ability and moves
			switch (checkFromLocationType) {
				case CHECK_BATTLE_TOWER_SPREADS:
					moveLoc = spread->moves;
					LOAD_TIER_CHECKING_ABILITY;
					break;

				default:
					moveLoc = mon->moves;
					ability = GetPartyAbility(mon);
			}
				
			if (IsFrontierSingles(battleFormat))
			{
				if (CheckTableForSpecies(species, gSmogonOU_SpeciesBanList)
				||  CheckTableForItem(item, gSmogonOU_ItemBanList))
					return TRUE;

				//Check Banned Abilities
				if (CheckTableForAbility(ability, gSmogonOU_AbilityBanList))
					return TRUE;

				//Check Banned Moves
				for (i = 0; i < MAX_MON_MOVES; ++i)
				{
					if (CheckTableForMove(moveLoc[i], gSmogon_MoveBanList)
					|| moveLoc[i] == MOVE_BATONPASS)
						return TRUE;
				}
			}
			else //Double Battles
			{
				bool8 knowsRecycle = FALSE;
				bool8 knowsPainSplit = FALSE;
				bool8 knowsFling = FALSE;
				bool8 knowsHealingMove = FALSE;

				if (CheckTableForSpecies(species, gSmogonOUDoubles_SpeciesBanList)
				||  CheckTableForItem(item, gSmogonOUDoubles_ItemBanList))
					return TRUE;
			
				//Check Banned Abilities
				if (CheckTableForAbility(ability, gSmogonOUDoubles_AbilityBanList))
					return TRUE;

				//Check Banned Moves
				for (i = 0; i < MAX_MON_MOVES; ++i)
				{
					if (CheckTableForMove(moveLoc[i], gSmogon_MoveBanList)
					||  CheckTableForMove(moveLoc[i], gSmogonOUDoubles_MoveBanList))
						return TRUE;
					
					switch (moveLoc[i]) {
						case MOVE_HEALPULSE:
						case MOVE_MILKDRINK:
						case MOVE_MOONLIGHT:
						case MOVE_MORNINGSUN:
						case MOVE_RECOVER:
						case MOVE_ROOST:
						case MOVE_SLACKOFF:
						case MOVE_SOFTBOILED:
						case MOVE_WISH:
							knowsHealingMove = TRUE;
							break;
						case MOVE_RECYCLE:
							knowsRecycle = TRUE;
							break;
						case MOVE_PAINSPLIT:
							knowsPainSplit = TRUE;
							break;
						case MOVE_FLING:
							knowsFling = TRUE;
							break;
					}
					
					if (knowsRecycle && ItemId_GetHoldEffect(item) == ITEM_EFFECT_RESTORE_PP) //Leppa Berry
					{
						if (knowsHealingMove
						||  knowsPainSplit
						||  knowsFling)
							return TRUE;
					}
				}
			}
			break;

		case BATTLE_TOWER_UBER:
		case BATTLE_TOWER_UBER_CAMOMONS:
		STANDARD_UBER_CHECK:
		//For Ubers ban Rayquaza with Dragon Ascent (handled in mega.c), Moody, and some moves
			switch (checkFromLocationType) {
				case CHECK_BATTLE_TOWER_SPREADS:
					moveLoc = spread->moves;
					LOAD_TIER_CHECKING_ABILITY;
					break;

				default:
					moveLoc = mon->moves;
					ability = GetPartyAbility(mon);
			}

			//Check Banned Abilities
			if (ability == ABILITY_MOODY)
				return 1;

			//Check Banned Moves
			for (i = 0; i < MAX_MON_MOVES; ++i) {
				if (CheckTableForMove(moveLoc[i], gSmogon_MoveBanList))
					return TRUE;
			}
			break;

		case BATTLE_TOWER_LITTLE_CUP:
		case BATTLE_TOWER_LC_CAMOMONS:
			if (!CheckTableForSpecies(species, gSmogonLittleCup_SpeciesList)
			||  CheckTableForItem(item, gSmogonLittleCup_ItemBanList))
				return TRUE; //Banned

			if (checkFromLocationType == CHECK_BATTLE_TOWER_SPREADS)
				moveLoc = spread->moves;
			else
				moveLoc = mon->moves;

			for (i = 0; i < MAX_MON_MOVES; ++i) {
				if (CheckTableForMove(moveLoc[i], gSmogonLittleCup_MoveBanList))
					return TRUE;
			}
			break;

		case BATTLE_TOWER_MIDDLE_CUP:
		case BATTLE_TOWER_MC_CAMOMONS:
			if (IsFrontierSingles(battleFormat)) //Middle Cup in Singles
			{
				if (!CheckTableForSpecies(species, gMiddleCup_SpeciesList)
				||   CheckTableForItem(item, gMiddleCup_ItemBanList))
					return TRUE; //Banned
					
				//Load correct ability and moves
				switch (checkFromLocationType) {
					case CHECK_BATTLE_TOWER_SPREADS:
						moveLoc = spread->moves;
						LOAD_TIER_CHECKING_ABILITY;
						break;

					default:
						moveLoc = mon->moves;
						ability = GetPartyAbility(mon);
				}

				//Check Banned Abilities
				if (CheckTableForAbility(ability, gMiddleCup_AbilityBanList))
					return TRUE;
			}
			else //GS Cup
			{
				if (CheckTableForItem(item, gBattleTowerStandard_ItemBanList))
					return TRUE;

				if (CheckTableForSpecies(species, gBattleTowerStandardSpeciesBanList)
				&& !CheckTableForSpecies(species, gGSCup_LegendarySpeciesList))
					return TRUE;
			}
			break;
			
		case BATTLE_TOWER_MONOTYPE:
		//For Monotype, there's a species, item, ability, and move ban list
			if (CheckTableForSpecies(species, gSmogonMonotype_SpeciesBanList)
			||  CheckTableForItem(item, gSmogonMonotype_ItemBanList))
				return TRUE;

			//Load correct ability and moves
			switch (checkFromLocationType) {
				case CHECK_BATTLE_TOWER_SPREADS:
					moveLoc = spread->moves;
					LOAD_TIER_CHECKING_ABILITY;
					break;

				default:
					moveLoc = mon->moves;
					ability = GetPartyAbility(mon);
			}

			//Check Banned Abilities
			if (CheckTableForAbility(ability, gSmogonMonotype_AbilityBanList))
				return TRUE;

			//Check Banned Moves
			for (i = 0; i < MAX_MON_MOVES; ++i) {
				if (CheckTableForMove(moveLoc[i], gSmogon_MoveBanList)
				|| moveLoc[i] == MOVE_BATONPASS
				|| moveLoc[i] == MOVE_SWAGGER)
					return TRUE;
			}
			break;
			
		case BATTLE_TOWER_CAMOMONS:
			if (CheckTableForSpecies(species, gSmogonCamomons_SpeciesBanList))
				return TRUE;
				
			goto STANDARD_OU_CHECK;
			
		case BATTLE_TOWER_SCALEMONS:
			if (CheckTableForSpecies(species, gSmogonScalemons_SpeciesBanList)
			||  CheckTableForItem(item, gSmogonScalemons_ItemBanList))
				return TRUE;

			//Load correct ability and moves
			switch (checkFromLocationType) {
				case CHECK_BATTLE_TOWER_SPREADS:
					moveLoc = spread->moves;
					LOAD_TIER_CHECKING_ABILITY;
					break;

				default:
					moveLoc = mon->moves;
					ability = GetPartyAbility(mon);
			}

			//Check Banned Abilities
			if (CheckTableForAbility(ability, gSmogonScalemons_AbilityBanList))
				return TRUE;
			
			goto STANDARD_UBER_CHECK;
			
		case BATTLE_TOWER_350_CUP:
			if (CheckTableForSpecies(species, gSmogon350Cup_SpeciesBanList)
			||  CheckTableForItem(item, gSmogon350Cup_ItemBanList))
				return TRUE;

			//Load correct ability and moves
			switch (checkFromLocationType) {
				case CHECK_BATTLE_TOWER_SPREADS:
					moveLoc = spread->moves;
					LOAD_TIER_CHECKING_ABILITY;
					break;

				default:
					moveLoc = mon->moves;
					ability = GetPartyAbility(mon);
			}

			//Check Banned Abilities
			if (CheckTableForAbility(ability, gSmogon350Cup_AbilityBanList))
				return TRUE;
				
			goto STANDARD_UBER_CHECK;
			
		case BATTLE_TOWER_AVERAGE_MONS:
			if (CheckTableForSpecies(species, gSmogonAverageMons_SpeciesBanList)
			||  CheckTableForItem(item, gSmogonAverageMons_ItemBanList))
				return TRUE;

			//Load correct ability and moves
			switch (checkFromLocationType) {
				case CHECK_BATTLE_TOWER_SPREADS:
					moveLoc = spread->moves;
					LOAD_TIER_CHECKING_ABILITY;
					break;

				default:
					moveLoc = mon->moves;
					ability = GetPartyAbility(mon);
			}

			//Check Banned Abilities
			if (CheckTableForAbility(ability, gSmogonAverageMons_AbilityBanList))
				return TRUE;
			break;
			
		case BATTLE_TOWER_BENJAMIN_BUTTERFREE:
			if (CheckTableForSpecies(species, gSmogonBenjaminButterfree_SpeciesBanList))
				return TRUE;
				
			goto STANDARD_OU_CHECK;
	}

	return FALSE; //Not banned
}

bool8 IsMonBannedInTier(struct Pokemon* mon, u8 tier)
{
	u16 species = GetMonData(mon, MON_DATA_SPECIES, NULL);
	u16 item = GetMonData(mon, MON_DATA_HELD_ITEM, NULL);

	return PokemonTierBan(species, item, NULL, mon, tier, CHECK_PARTY_OFFSET);
}

static bool8 IsPokemonBannedBasedOnStreak(u16 species, u16 item, u16* speciesArray, u8 monsCount, u16 trainerId, u8 tier, bool8 forPlayer)
{
	u16 streak = GetCurrentBattleTowerStreak();
	
	if (!forPlayer && trainerId == BATTLE_TOWER_TID && tier == BATTLE_TOWER_STANDARD)
	{
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
	else if ((forPlayer || trainerId == BATTLE_TOWER_MULTI_TRAINER_TID) && tier == BATTLE_TOWER_STANDARD)
	{
		streak = GetMaxBattleTowerStreakForTier(tier);
		
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
	else if (trainerId == BATTLE_TOWER_SPECIAL_TID && streak < 20 && tier == BATTLE_TOWER_STANDARD)
	{
		if (IsMegaStone(item)) //Special trainers aren't allowed to Mega Evolve
			return TRUE;	   //before the player has beaten Palmer in the 20th battle.
	}
	
	return FALSE;
}

static bool8 TeamDoesntHaveSynergy(const struct BattleTowerSpread* const spread, const struct TeamBuilder* const builder)
{
	int i;

	u8 ability = ConvertFrontierAbilityNumToAbility(spread->ability, spread->species);
	u8 itemEffect = (ability != ABILITY_KLUTZ) ? ItemId_GetHoldEffect(spread->item) : 0;

	bool8 hasTailwinder = builder->moveOnTeam[MOVE_TAILWIND];
	bool8 hasTrickRoomer = builder->moveOnTeam[MOVE_TRICKROOM];
	bool8 hasRainSetter = builder->abilityOnTeam[ABILITY_DRIZZLE] || builder->moveOnTeam[MOVE_RAINDANCE];
	bool8 hasSunSetter = builder->abilityOnTeam[ABILITY_DROUGHT] || builder->moveOnTeam[MOVE_SUNNYDAY];
	bool8 hasSandSetter = builder->abilityOnTeam[ABILITY_SANDSTREAM] || builder->moveOnTeam[MOVE_SANDSTORM];
	bool8 hasHailSetter = builder->abilityOnTeam[ABILITY_SNOWWARNING] || builder->moveOnTeam[MOVE_HAIL];
	bool8 hasElectricTerrainSetter = builder->abilityOnTeam[ABILITY_ELECTRICSURGE] || builder->moveOnTeam[MOVE_ELECTRICTERRAIN];
	bool8 hasWonderGuard = builder->abilityOnTeam[ABILITY_WONDERGUARD];
	bool8 hasJustified = builder->abilityOnTeam[ABILITY_JUSTIFIED];
	
	if (builder->partyIndex[BAD_ABILITY] != 0xFF && gAbilityRatings[ability] < 0)
		return TRUE; //Only stick at most 1 Pokemon with a sucky ability on a team

	if (builder->tier != BATTLE_TOWER_MONOTYPE && hasTrickRoomer && spread->spdEv >= 20)
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
				
			if (builder->tier != BATTLE_TOWER_MONOTYPE && builder->partyIndex[FAST_MON] != 0xFF)
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

	u8 class = PredictFightingStyle(spread->moves, ability, itemEffect, 0xFF);
	if (builder->monsCount <= 4)
	{
		if (IS_SINGLE_BATTLE)
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
		else //Double Battle
		{
			if (itemEffect == ITEM_EFFECT_CHOICE_BAND && builder->numChoiceItems >= 1)
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
		if (IS_SINGLE_BATTLE)
		{
			if (IsClassStall(class))
			{
				if (builder->tier == BATTLE_TOWER_MONOTYPE
				|| (IsMiddleCupTier(builder->tier) && IsFrontierSingles(builder->battleType)))
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
			if (itemEffect == ITEM_EFFECT_CHOICE_BAND && builder->numChoiceItems >= 2)
				return TRUE; //Max two choiced Pokemon per 6v6 doubles team
		
			if (IsClassDoublesUtility(class) || IsClassDoublesTeamSupport(class))
			{
				if (builder->numStalls >= 3) //Stalls here refers to utility/team support
					return TRUE; //Max three utility/team support Pokemon per 6v6 Team
			}
		}
	}
	
	return FALSE;
}

static u16 GivePlayerFrontierMonGivenSpecies(const u16 species, const struct BattleTowerSpread* const spreadTable, const u16 numSpreads)
{
	struct Pokemon mon;
	const struct BattleTowerSpread* spread = GetSpreadBySpecies(species, spreadTable, numSpreads);
	
	if (spread == NULL)
		return 0xFFFF;

	spread = TryAdjustSpreadForSpecies(spread); //Update Arceus
	CreateFrontierMon(&mon, 50, spread, 0, 0, 0, TRUE);
	return GiveMonToPlayer(&mon);
}

static const struct BattleTowerSpread* GetSpreadBySpecies(const u16 species, const struct BattleTowerSpread* const spreads, const u16 numSpreads)
{
	u32 i;

	for (i = 0; i < numSpreads; ++i)
	{
		if (spreads[i].species == species)
			break;
	}

	if (i == numSpreads)
		return NULL; //Species not found
		
	u8 offset = Random() % 5; //Max number of possible spreads for a given Pokemon

	while (spreads[i + offset].species != species) //Overshot
		offset = Random() % 5;
		
	return &spreads[i + offset];
}

static const struct BattleTowerSpread* TryAdjustSpreadForSpecies(const struct BattleTowerSpread* originalSpread)
{
	u16 species = originalSpread->species;

	switch (species) {
		case SPECIES_PIKACHU:
			return &gPikachuSpreads[Random() % TOTAL_PIKACHU_SPREADS]; //Sooo many different forms of Pikachu

		case SPECIES_WORMADAM:
			return &gWormadamSpreads[Random() % TOTAL_WORMADAM_SPREADS];

		case SPECIES_ROTOM:
			return &gRotomSpreads[Random() % TOTAL_ROTOM_SPREADS]; //All the Rotom forms

		case SPECIES_ARCEUS:
			return &gArceusSpreads[Random() % TOTAL_ARCEUS_SPREADS]; //There are more Arceus spreads than any other Pokemon,
																	 //so they're held seperately to keep things fresh.
		
		case SPECIES_ORICORIO:
			return &gOricorioSpreads[Random() % TOTAL_ORICORIO_SPREADS];
	}

	return originalSpread;
}

static u16 TryAdjustAestheticSpecies(u16 species)
{
	u16 nationalDexNum = SpeciesToNationalPokedexNum(species);
	
	switch (nationalDexNum) {
		case NATIONAL_DEX_DEERLING:
			species = gDeerlingForms[Random() % gNumDeerlingForms];
			break;
		case NATIONAL_DEX_SAWSBUCK:
			species = gSawsbuckForms[Random() % gNumSawsbuckForms];
			break;
		case NATIONAL_DEX_VIVILLON:
			species = gVivillonForms[Random() % gNumVivillonForms];
			break;
		case NATIONAL_DEX_FLABEBE:
			species = gFlabebeForms[Random() % gNumFlabebeForms];
			break;
		case NATIONAL_DEX_FLOETTE:
			if (species != SPECIES_FLOETTE_ETERNAL) //Floette Eternal gets its own spreads
				species = gFloetteForms[Random() % gNumFloetteForms];
			break;
		case NATIONAL_DEX_FLORGES:
			species = gFlorgesForms[Random() % gNumFlorgesForms];
			break;
		case NATIONAL_DEX_FURFROU:
			species = gFurfrouForms[Random() % gNumFurfrouForms];
			break;
		default:
			if (species == SPECIES_PIKACHU_CAP_ORIGINAL)
				species = gPikachuCapForms[Random() % gNumPikachuCapForms];
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
	u8 noIfImmunity;
	u8 yesIfImmunity;
};

enum
{
	LEARN_TYPE_LEVEL_UP,
	LEARN_TYPE_TM,
	LEARN_TYPE_TUTOR,
};

#ifdef UNBOUND

#define TUTOR51_HEAT_WAVE 51
#define TUTOR52_HYPER_VOICE 52

static struct DoubleReplacementMoves sDoubleSpreadReplacementMoves[] =
{
	{MOVE_OUTRAGE, MOVE_DRAGONCLAW, LEARN_TYPE_TM, ITEM_TM02_DRAGON_CLAW, 0, 0},
	{MOVE_SUBSTITUTE, MOVE_PROTECT, LEARN_TYPE_TM, ITEM_TM17_PROTECT, 0, 0},
	{MOVE_THRASH, MOVE_RETURN, LEARN_TYPE_TM, ITEM_TM27_RETURN, 0, 0},
	{MOVE_STONEEDGE, MOVE_ROCKSLIDE, LEARN_TYPE_TM, ITEM_TM80_ROCK_SLIDE, 0, 0},
	{MOVE_MOONBLAST, MOVE_DAZZLINGGLEAM, LEARN_TYPE_TM, ITEM_TM102_DAZZLING_GLEAM, 0, 0},
	{MOVE_SLUDGEWAVE, MOVE_SLUDGEBOMB, LEARN_TYPE_TM, ITEM_TM36_SLUDGE_BOMB, POISON_IMMUNITY, 0},
	{MOVE_FLAMETHROWER, MOVE_HEATWAVE, LEARN_TYPE_TUTOR, TUTOR51_HEAT_WAVE, 0, 0},
	{MOVE_FIREBLAST, MOVE_HEATWAVE, LEARN_TYPE_TUTOR, TUTOR51_HEAT_WAVE, 0, 0},
	{MOVE_BOOMBURST, MOVE_HYPERVOICE, LEARN_TYPE_TUTOR, TUTOR52_HYPER_VOICE, SOUND_IMMUNITY, 0},
	{MOVE_SURF, MOVE_MUDDYWATER, LEARN_TYPE_LEVEL_UP, 0, WATER_IMMUNITY, 0},
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
	}
	
	//Shuffle moves in camomons
	if (IsCamomonsTier(builder->tier) && builder->trainerId != FRONTIER_BRAIN_TID)
	{
		for (i = 0; i < PARTY_SIZE; ++i)
		{
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
			else if (defiantIndex != 0xFF && party == gEnemyParty && GetPartyAbility(&gPlayerParty[0]) == ABILITY_INTIMIDATE)
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
		for (i = 0; i < PARTY_SIZE; ++i)
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
						u8 newPP = gBattleMoves[newMove].pp;

						if (pos < MAX_MON_MOVES && !MoveInMonMoveset(newMove, &party[i]))
						{
							if (sDoubleSpreadReplacementMoves[j].noIfImmunity != 0
							&&  builder->partyIndex[sDoubleSpreadReplacementMoves[j].noIfImmunity] != i)
								continue;
								
							if (sDoubleSpreadReplacementMoves[j].yesIfImmunity == 0
							||  builder->partyIndex[sDoubleSpreadReplacementMoves[j].yesIfImmunity] != i) //There's an immunity on some other Pokemon
							{		
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
			&& (GetPartyAbility(&gPlayerParty[0]) == ABILITY_INTIMIDATE || GetPartyAbility(&gPlayerParty[1]) == ABILITY_INTIMIDATE))
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
}

bool8 IsMonAllowedInBattleTower(struct Pokemon* mon)
{
	if (FlagGet(BATTLE_TOWER_FLAG))
	{
		u16 species = mon->species;
		u16 item = mon->item;
		u16 tier = VarGet(BATTLE_TOWER_TIER);

		if (GetMonData(mon, MON_DATA_IS_EGG, NULL))
			return FALSE;

		if (PokemonTierBan(species, item, NULL, mon, tier, CHECK_PARTY_OFFSET))
			return FALSE;

		if (tier != BATTLE_TOWER_NO_RESTRICTIONS) //Free for all has no duplicate species or item restriction
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
			|| (DuplicateItemsAreBannedInTier(tier, VarGet(BATTLE_TOWER_BATTLE_TYPE)) && ItemAlreadyOnTeam(mon->item, partySize, itemArray))
			|| (tier == BATTLE_TOWER_MONOTYPE && TeamNotAllSameType(mon->species, mon->item, partySize, speciesArray, itemArray))
			|| (tier == BATTLE_TOWER_GS_CUP && !IsFrontierSingles(VarGet(BATTLE_TOWER_BATTLE_TYPE)) && TooManyLegendariesOnGSCupTeam(mon->species, partySize, speciesArray)))
				return FALSE;
		}
	}
	else if (mon->hp == 0) //Regular multi battle probably
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
	u8 max = party[0].level;

	for (int i = 1; i < PARTY_SIZE; ++i)
	{
		if (max == MAX_LEVEL
		||  party[i].species == SPECIES_NONE)
			return max;

		if (GetMonData(&party[i], MON_DATA_IS_EGG, NULL))
			continue;

		if (party[i].level > max)
			max = party[i].level;
	}

	return max;
}

u8 GetMonPokeBall(struct PokemonSubstruct0* data)
{
	u8 ball = data->pokeball;

	if (ball == 0) //Bug prevention
		return BALL_TYPE_POKE_BALL;

	return ball;
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

	u8 max = MathMax(openWorldLevelRanges[badgeCount][0], openWorldLevelRanges[badgeCount][1]); //Prevent incorrect order errors
	u8 min = MathMin(openWorldLevelRanges[badgeCount][0], openWorldLevelRanges[badgeCount][1]);
	u8 range = (max - min) + 1;

	return min + ((nameHash + 7 * i) ^ T1_READ_32(gSaveBlock2->playerTrainerId)) % range;
}

static u8 GetOpenWorldBadgeCount(void)
{
	u8 badgeCount = 0;

	if (FlagGet(FLAG_ELITE4_BEAT)) //0x82C
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

#endif

//unused1 is used to hook in so don't use it for anything
u8 ScriptGiveMon(u16 species, u8 level, u16 item, unusedArg u32 unused1, unusedArg u32 unused2, u8 ballType)
{
	u16 nationalDexNum;
	int sentToPc;
	struct Pokemon mon;

	CreateMon(&mon, species, level, 32, 0, 0, 0, 0);
	SetMonData(&mon, MON_DATA_HELD_ITEM, &item);

	#ifdef GIVEPOKEMON_BALL_HACK
	if (ballType)
		mon.pokeball = ballType;
	#endif

	if (FlagGet(HIDDEN_ABILITY_FLAG))
		mon.hiddenAbility = TRUE;

	sentToPc = GiveMonToPlayer(&mon);
	nationalDexNum = SpeciesToNationalPokedexNum(species);

	switch(sentToPc) {
		case 0:
		case 1:
			GetSetPokedexFlag(nationalDexNum, FLAG_SET_SEEN);
			GetSetPokedexFlag(nationalDexNum, FLAG_SET_CAUGHT);
			break;
	}

	unused1 += unused2 +=  ballType; //So the compiler doesn't complain

	return sentToPc;
}

static u32 CheckShinyMon(u32 pid) 
{
	u16 chance = 1;	//Default 1/4096 rate

	if (CheckBagHasItem(ITEM_SHINY_CHARM, 1) > 0)
		chance = 3;
		
	if (FlagGet(SHINY_CREATION_FLAG))
		chance = 4097;

	if (RandRange(0, 4097) < chance)		//Nominal 1/4096
	{
		// make shiny
		u8 shinyRange = RandRange(0,8);

		u32 playerId = T1_READ_32(gSaveBlock2->playerTrainerId);
		u16 sid = HIHALF(playerId);
		u16 tid = LOHALF(playerId);
		pid = (((shinyRange ^ (sid ^ tid)) ^ LOHALF(pid)) << 16) | LOHALF(pid);
	}
	
	return pid;
};


void CreateBoxMon(struct BoxPokemon* boxMon, u16 species, u8 level, u8 fixedIV, bool8 hasFixedPersonality, u32 fixedPersonality, u8 otIdType, u32 fixedOtId) 
{
	int i;
	u8 speciesName[POKEMON_NAME_LENGTH + 1];
	u32 personality;
	u32 value;

#ifdef UNBOUND
	if (FlagGet(POKEMON_RANDOMIZER_FLAG))
	{
		u32 id = MathMax(1, T1_READ_32(gSaveBlock2->playerTrainerId)); //0 id would mean every Pokemon would crash the game
		u32 newSpecies = species * id;
		species = MathMax(SPECIES_BULBASAUR, newSpecies % NUM_SPECIES);
		
		while (CheckTableForSpecies(species, gRandomizerBanList))
			species *= id;
	}
#endif

	ZeroBoxMonData(boxMon);
	if (hasFixedPersonality)
		personality = fixedPersonality;
	else
		personality = Random32();

	personality = CheckShinyMon(personality);	//Shiny charm
	boxMon->personality = personality;
	
	//Determine original trainer ID
	if (otIdType == OT_ID_RANDOM_NO_SHINY) //Pokemon cannot be shiny
	{
		u32 shinyValue;
		do
		{
			value = Random32();
			shinyValue = HIHALF(value) ^ LOHALF(value) ^ HIHALF(personality) ^ LOHALF(personality);
		} while (shinyValue < 8);
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

	if (fixedIV < 32)
	{
		for (i = 0; i < NUM_STATS; ++i)
			SetBoxMonData(boxMon, MON_DATA_HP_IV + i, &fixedIV);
	}
	else
	{
		u32 iv;
		value = Random();

		iv = value & 0x1F;
		SetBoxMonData(boxMon, MON_DATA_HP_IV, &iv);
		iv = (value & 0x3E0) >> 5;
		SetBoxMonData(boxMon, MON_DATA_ATK_IV, &iv);
		iv = (value & 0x7C00) >> 10;
		SetBoxMonData(boxMon, MON_DATA_DEF_IV, &iv);

		value = Random();

		iv = value & 0x1F;
		SetBoxMonData(boxMon, MON_DATA_SPEED_IV, &iv);
		iv = (value & 0x3E0) >> 5;
		SetBoxMonData(boxMon, MON_DATA_SPATK_IV, &iv);
		iv = (value & 0x7C00) >> 10;
		SetBoxMonData(boxMon, MON_DATA_SPDEF_IV, &iv);
		
		#ifdef CREATE_WITH_X_PERFECT_IVS
		{
			if (CheckTableForSpecies(species, gSetPerfectXIvList))
			{
				u8 numPerfectStats = 0;
				u8 perfect = 31;
				bool8 perfectStats[NUM_STATS] = {0};
				
				while (numPerfectStats < MathMin(CREATE_WITH_X_PERFECT_IVS, NUM_STATS)) //Error prevention
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

	GiveBoxMonInitialMoveset(boxMon);
}

void CreateMonWithNatureLetter(struct Pokemon* mon, u16 species, u8 level, u8 fixedIV, u8 nature, u8 letter)
{
	u32 personality;
	letter -= 1;

	if ((u8)(letter) < 28)
	{
		u16 actualLetter;

		do
		{
			personality = Random32();
			actualLetter = GetUnownLetterFromPersonality(personality);
		}
		while (nature != GetNatureFromPersonality(personality) || actualLetter != letter);
	}
	else
	{
		CreateMonWithNature(mon, species, level, 32, nature);
		return;
	}

	CreateMon(mon, species, level, fixedIV, TRUE, personality, OT_ID_PLAYER_ID, 0);
}

void TryStatusInducer(struct Pokemon* mon)
{
	if (VarGet(STATUS_INDUCER_VAR))
	{
		u8 status = VarGet(STATUS_INDUCER_VAR) & 0xFF; //Lowest byte is status
		mon->condition = status;
	}
}

bool8 GetAlternateHasSpecies(struct BoxPokemon* mon)
{
	return GetBoxMonData(mon, MON_DATA_SPECIES, NULL) != SPECIES_NONE && !GetBoxMonData(mon, MON_DATA_SANITY_IS_BAD_EGG, NULL);
}

#define CALC_STAT(base, iv, ev, statIndex, field)               \
{                                                               \
    s32 n = (((2 * base + iv + ev / 4) * level) / 100) + 5; 	\
    u8 nature = GetNature(mon);                                 \
    n = ModifyStatByNature(nature, n, statIndex);               \
    SetMonData(mon, field, &n);                                 \
}

void CalculateMonStatsNew(struct Pokemon *mon)
{
	int i;
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

	if (species == SPECIES_SHEDINJA)
	{
		newMaxHP = 1;
	}
	else
	{
		u32 n = 2 * baseHP + ivs[STAT_HP];
		newMaxHP = MathMin((((n + evs[STAT_HP] / 4) * level) / 100) + level + 10, 0xFFFF);
	}

	gBattleScripting->field_23 = newMaxHP - oldMaxHP;
	if (gBattleScripting->field_23 == 0)
		gBattleScripting->field_23 = 1;

	SetMonData(mon, MON_DATA_MAX_HP, &newMaxHP);

	if (IsScaleMonsBattle())
	{
		for (i = STAT_ATK; i < NUM_STATS; ++i) //HP doesn't change in Scalemons
		{
			u16 base = ((u8*) (&gBaseStats[species].baseHP))[i];
			base = (base * (600 - baseHP)) / (baseStatTotal - baseHP);
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

	if (species == SPECIES_SHEDINJA)
	{
		if (currentHP != 0 || oldMaxHP == 0)
			currentHP = 1;
		else
			return;
	}
	else
	{
		if (currentHP == 0 && oldMaxHP == 0)
			currentHP = newMaxHP;
		else if (currentHP != 0)
			currentHP += newMaxHP - oldMaxHP;
		else
			return;
	}

	SetMonData(mon, MON_DATA_HP, &currentHP);
}
