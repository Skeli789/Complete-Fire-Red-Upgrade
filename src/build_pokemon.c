#include "defines.h"
#include "defines_battle.h"
#include "../include/event_data.h"
#include "../include/pokemon.h"
#include "../include/pokemon_storage_system.h"
#include "../include/random.h"
#include "../include/wild_encounter.h"
#include "../include/constants/items.h"
#include "../include/constants/maps.h"
#include "../include/constants/trainers.h"

#include "../include/new/build_pokemon.h"
#include "../include/new/build_pokemon_2.h"
#include "../include/new/catching.h"
#include "../include/new/dexnav.h"
#include "../include/new/form_change.h"
#include "../include/new/frontier.h"
#include "../include/new/helper_functions.h"
#include "../include/new/item.h"
#include "../include/new/learn_move.h"
#include "../include/new/multi.h"

#include "Tables/Trainers_With_EVs_Table.h"
#include "Tables/battle_tower_spreads.h"

#define TOTAL_SPREADS ARRAY_COUNT(gFrontierSpreads)

extern const u8 gClassPokeBalls[NUM_TRAINER_CLASSES];
extern const species_t gRandomizerBanList[];
extern const species_t gSetPerfectXIvList[];

extern bool8 sp051_CanTeamParticipateInSkyBattle(void);
extern bool8 CanMonParticipateInASkyBattle(struct Pokemon* mon);

//This file's functions:
static u8 CreateNPCTrainerParty(pokemon_t* const party, const u16 trainerNum, const bool8 firstTrainer, const bool8 side);
static u8 BuildFrontierParty(pokemon_t* const party, const u16 trainerNum, const bool8 firstTrainer, const bool8 forPlayer, const u8 side);
static void CreateFrontierMon(struct Pokemon* mon, const u8 level, const struct BattleTowerSpread* spread, const u16 trainerId, const u8 trainerNum, const u8 trainerGender, const bool8 forPlayer);
static void SetWildMonHeldItem(void);
static u32 GetBaseStatsTotal(const u16 species);
static bool8 BaseStatsTotalGEAlreadyOnTeam(const u16 toCheck, const u8 partySize, u16* speciesArray);
static bool8 SpeciesAlreadyOnTeam(const u16 species, const u8 partySize, const species_t* const speciesArray);
static bool8 ItemAlreadyOnTeam(const u16 item, const u8 partySize, const item_t* const itemArray);
static bool8 MegastoneAlreadyOnTeam(const u16 item, const u8 partySize, const item_t* const itemArray);
static bool8 ZCrystalAlreadyOnTeam(const u16 item, const u8 partySize, const item_t* const itemArray);
static bool8 PokemonTierBan(const u16 species, const u16 item, const struct BattleTowerSpread* const spread, const pokemon_t* const mon, const u8 checkFromLocationType);
static bool8 IsPokemonBannedBasedOnStreak(u16 species, u16 item, u16* speciesArray, u8 monsCount, u16 trainerId, u8 tier, bool8 forPlayer);
static u16 GivePlayerFrontierMonGivenSpecies(const u16 species, const struct BattleTowerSpread* const spreadTable, const u16 numSpreads);
const struct BattleTowerSpread* GetSpreadBySpecies(const u16 species, const struct BattleTowerSpread* const spreads, const u16 numSpreads);
static u8 GetPartyIdFromPartyData(struct Pokemon* mon);
static u8 GetHighestMonLevel(const pokemon_t* const party);

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
	gDontFadeWhite = FALSE;
	
	if (gBattleTypeFlags & (BATTLE_TYPE_TOWER_LINK_MULTI))
	{
		BuildFrontierParty(&gEnemyParty[0], gTrainerBattleOpponent_A, TRUE, FALSE, B_SIDE_OPPONENT);
		BuildFrontierParty(&gEnemyParty[3], VarGet(SECOND_OPPONENT_VAR), FALSE, FALSE, B_SIDE_OPPONENT);
	}
	else if (gBattleTypeFlags & BATTLE_TYPE_FRONTIER)
	{
		if (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS)
		{
			BuildFrontierParty(&gEnemyParty[0], gTrainerBattleOpponent_A, TRUE, FALSE, B_SIDE_OPPONENT);
			BuildFrontierParty(&gEnemyParty[3], VarGet(SECOND_OPPONENT_VAR), FALSE, FALSE, B_SIDE_OPPONENT);
		}
		else
			BuildFrontierParty(&gEnemyParty[0], gTrainerBattleOpponent_A, TRUE, FALSE, B_SIDE_OPPONENT);
	}
	else if (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS)
	{
		CreateNPCTrainerParty(&gEnemyParty[0], gTrainerBattleOpponent_A, TRUE, B_SIDE_OPPONENT);
		CreateNPCTrainerParty(&gEnemyParty[3], VarGet(SECOND_OPPONENT_VAR), FALSE, B_SIDE_OPPONENT);
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

	if (gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER && ViableMonCount(gEnemyParty) > 1) {
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

	if (ViableMonCount(gEnemyParty) <= 1) //Error prevention
		gBattleTypeFlags &= ~(BATTLE_TYPE_INGAME_PARTNER | BATTLE_TYPE_TWO_OPPONENTS | BATTLE_TYPE_DOUBLE);
}

extern void SortItemsInBag(u8 pocket, u8 type);
void sp067_GenerateRandomBattleTowerTeam(void)
{
	BuildFrontierParty(gPlayerParty, 0, TRUE, TRUE, B_SIDE_PLAYER);
	
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
	const struct BattleTowerSpread* spreads;

	switch (Var8001) {
		case 0:
		default:
			numSpreads = TOTAL_SPREADS;
			spreads = gFrontierSpreads;
	}

	return GivePlayerFrontierMonGivenSpecies(Var8000, spreads, numSpreads);
}

//@Details: Add a random Pokemon battleable in the given tier.
//@Inputs:
//		Var8000: Tier
//		Var8001: Spread Type
//@Returns: If the Pokemon was added or not.
u16 sp069_GivePlayerRandomFrontierMonByTier(void)
{
	u16 numSpreads;
	struct Pokemon mon;
	const struct BattleTowerSpread* spread;
	const struct BattleTowerSpread* spreads;

	switch (Var8001) {
		case 0:
		default:
			numSpreads = TOTAL_SPREADS;
			spreads = gFrontierSpreads;
	}
	
	do
	{
		spread = &spreads[Random() % numSpreads];
	} while (IsPokemonBannedBasedOnStreak(spread->species, spread->item, NULL, 0, 0, Var8000, TRUE));
	
	CreateFrontierMon(&mon, 50, spread, 0, 0, 0, TRUE);
	return GiveMonToPlayer(&mon);
}

//Returns the number of Pokemon
static u8 CreateNPCTrainerParty(pokemon_t* const party, const u16 trainerId, const bool8 firstTrainer, const bool8 side)
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

				if (FlagGet(SCALE_TRAINER_LEVELS_FLAG) || (gBattleTypeFlags & BATTLE_TYPE_TRAINER_TOWER))																					\
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
							ballType = Random() % (NUM_BALLS + 1); //Set Random Ball
							break;
						default:
							ballType = MathMin(NUM_BALLS, spread->ball);
					}

					SetMonData(&party[i], REQ_POKEBALL, &ballType);

					switch(spread->ability) {
						case Ability_Hidden:
						TRAINER_WITH_EV_GIVE_HIDDEN_ABILITY:
							GiveMonNatureAndAbility(&party[i], spread->nature, 0xFF); //Give Hidden Ability
							break;
						case Ability_1:
						case Ability_2:
							GiveMonNatureAndAbility(&party[i], spread->nature, MathMin(1, spread->ability - 1));
							break;
						case Ability_Random_1_2:
						TRAINER_WITH_EV_GIVE_RANDOM_ABILITY:
							GiveMonNatureAndAbility(&party[i], spread->nature, Random() % 2);
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

			CalculateMonStats(&party[i]);
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
static u8 BuildFrontierParty(pokemon_t* const party, const u16 trainerId, const bool8 firstTrainer, const bool8 forPlayer, const u8 side) 
{
    int i;
	u8 monsCount;
	
	u8 trainerGender = 0;
	u8 tier = VarGet(BATTLE_TOWER_TIER);
	u8 battleTowerPokeNum = VarGet(BATTLE_TOWER_POKE_NUM);
	u16 tableId = VarGet(TOWER_TRAINER_ID_VAR + (firstTrainer ^ 1));

	if (!forPlayer) 
	{
		if (trainerId == 0x400)
			return 0;
		else if (trainerId != BATTLE_TOWER_TID 
			  && trainerId != BATTLE_TOWER_SPECIAL_TID
			  && trainerId != FRONTIER_BRAIN_TID)
		{
			return (CreateNPCTrainerParty(party, trainerId, firstTrainer, side));
		}
	}

	//Two of the three variables here hold garbage data which is never called.
	const struct BattleTowerTrainer* trainer = &gTowerTrainers[tableId];
	const struct SpecialBattleTowerTrainer* specialTrainer = &gSpecialTowerTrainers[tableId];
	const struct FrontierBrain* frontierBrain = &gFrontierBrains[tableId];
	
	switch (trainerId) {
		case BATTLE_TOWER_TID:
			trainerGender = trainer->gender;
			break;
		case BATTLE_TOWER_SPECIAL_TID:
			trainerGender = specialTrainer->gender;
			break;
		case FRONTIER_BRAIN_TID:
			trainerGender = frontierBrain->gender;
			break;
	}
	
	if (forPlayer)
		ZeroPlayerPartyMons();
	else if (firstTrainer)
		ZeroEnemyPartyMons();

	if (forPlayer)
		monsCount = PARTY_SIZE;
	else if (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS && battleTowerPokeNum > 3 && side == B_SIDE_OPPONENT)
		monsCount = 3;
	else if (gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER && battleTowerPokeNum > 3 && side == B_SIDE_PLAYER)
		monsCount = 3;
	else
		monsCount = MathMax(1, MathMin(PARTY_SIZE, battleTowerPokeNum));

	species_t speciesArray[monsCount];
	item_t itemArray[monsCount];
	
	for (i = 0; i < monsCount; ++i)
	{
		speciesArray[i] = 0;
		itemArray[i] = 0;
	}

    for (i = 0; i < monsCount; ++i) 
	{
        u8 loop = 1;
		u16 species;
		u16 item;
		const struct BattleTowerSpread* spread;

		do 
		{
			switch (trainerId) {
				case BATTLE_TOWER_SPECIAL_TID:
					spread = &specialTrainer->spreads[Random() % specialTrainer->spreadSize]; //Special trainers have preset spreads.
					break;
				case FRONTIER_BRAIN_TID:
					spread = &frontierBrain->spreads[Random() % frontierBrain->spreadSize]; //Frontier Brains have preset spreads.
					break;
				default: //BATTLE_TOWER_TID and forPlayer
					spread = &gFrontierSpreads[Random() % TOTAL_SPREADS];
			}

			species = spread->species;
			item = spread->item;

			//Prevent duplicate species and items
			//Only allow one Mega Stone & Z-Crystal per team
			if (!IsPokemonBannedBasedOnStreak(species, item, speciesArray, monsCount, trainerId, tier, forPlayer)
			&& !SpeciesAlreadyOnTeam(species, monsCount, speciesArray)
			&& !ItemAlreadyOnTeam(item, monsCount, itemArray)
			&& !MegastoneAlreadyOnTeam(item, monsCount, itemArray)
			&& !ZCrystalAlreadyOnTeam(item, monsCount, itemArray)
			&& !PokemonTierBan(species, item, spread, 0, CHECK_BATTLE_TOWER_SPREADS)) 
			{
				speciesArray[i] = species;
				itemArray[i] = item;
				loop = 0;
			}
		} while (loop == 1);
		
		u8 level;
		if (tier == BATTLE_TOWER_LITTLE_CUP)
			level = 5;
		else
			level = MathMax(1, MathMin(MAX_LEVEL, VarGet(BATTLE_TOWER_POKE_LEVEL)));


		CreateFrontierMon(&party[i], level, spread, trainerId, firstTrainer ^ 1, trainerGender, forPlayer);
    }

	if (!forPlayer) //Probably best to put these checks somewhere else
	{ 
		u8 battleType = VarGet(BATTLE_TOWER_BATTLE_TYPE);
		if (battleType == BATTLE_TOWER_DOUBLE || battleType == BATTLE_TOWER_MULTI)
			gBattleTypeFlags |= BATTLE_TYPE_DOUBLE;
	}

    return monsCount;
}

static void CreateFrontierMon(struct Pokemon* mon, const u8 level, const struct BattleTowerSpread* spread, const u16 trainerId, const u8 trainerNum, const u8 trainerGender, const bool8 forPlayer)
{
	int i, j;

	if (forPlayer)
		CreateMon(mon, spread->species, level, 0, TRUE, 0, OT_ID_PLAYER_ID, 0);

	else 
	{
		CreateMon(mon, spread->species, level, 0, TRUE, 0, OT_ID_PRESET, Random32());

		CopyFrontierTrainerName((u8*) mon->otname, trainerId, trainerNum);
		mon->otGender = trainerGender;
	}

	mon->metLocation = 0x3A; //Battle Tower RS
	mon->metLevel = level;
	mon->obedient = TRUE;
	mon->friendship = 255;

	i = 0;
	struct Pokemon* party = mon;

	SET_IVS(spread);
	SET_EVS(spread);

	if (spread->ability > 0)
	{
		GiveMonNatureAndAbility(mon, spread->nature, spread->ability - 1);
	}
	else //Hidden Ability
	{ 
		GiveMonNatureAndAbility(mon, spread->nature, 0xFF);
		mon->hiddenAbility = TRUE;
	}

    for (j = 0; j < MAX_MON_MOVES; j++) 
	{
		mon->moves[j] = spread->moves[j];
		mon->pp[j] = gBattleMoves[spread->moves[j]].pp;
	}

	SetMonData(mon, MON_DATA_HELD_ITEM, &spread->item);

	u8 ballType;
	if (spread->ball)
		ballType = MathMin(NUM_BALLS, spread->ball);
	else
		ballType = umodsi(Random(), NUM_BALLS + 1);
	SetMonData(mon, REQ_POKEBALL, &ballType);

	CalculateMonStats(mon);
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
			if (i > 0 && !(gBattleTypeFlags & BATTLE_TYPE_DOUBLE))
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

void GiveMonNatureAndAbility(pokemon_t* mon, u8 nature, u8 abilityNum)
{
	u32 personality;

	while(TRUE) 
	{
		personality = Random32();
		personality &= ~(1);

		if (abilityNum != 0xFF)
			personality |= MathMin(1, abilityNum); //Either 0 or 1

		if (GetNatureFromPersonality(personality) == nature) 
		{
			mon->personality = personality;
			break;
		}
	}
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

static bool8 MegastoneAlreadyOnTeam(const u16 item, const u8 partySize, const item_t* const itemArray)
{
	if (!IsMegaStone(item))
		return FALSE;

	for (int i = 0; i < partySize; ++i) 
	{
		if (IsMegaStone(itemArray[i]))
			return TRUE;
	}

	return FALSE;
}


static bool8 ZCrystalAlreadyOnTeam(const u16 item, const u8 partySize, const item_t* const itemArray)
{
	if (!IsZCrystal(item))
		return FALSE;

	for (int i = 0; i < partySize; ++i) 
	{
		if (IsZCrystal(itemArray[i]))
			return TRUE;
	}

	return FALSE;
}

static bool8 PokemonTierBan(const u16 species, const u16 item, const struct BattleTowerSpread* const spread, const pokemon_t* const mon, const u8 checkFromLocationType)
{
	u32 i;
	u8 ability;
	const u16* moveLoc;

	if (species == SPECIES_EGG)
		return 1;

	u16 tier = VarGet(BATTLE_TOWER_TIER);

	switch (tier) {
		case BATTLE_TOWER_STANDARD:
			if (CheckTableForSpecies(species, StandardSpeciesBanList)
			||  CheckTableForItem(item, StandardItemBanList))
				return TRUE;
			break;

		case BATTLE_TOWER_OU:
		//For OU, there's a species, item, ability, and move ban list
			if (CheckTableForSpecies(species, OU_SpeciesBanList)
			||  CheckTableForItem(item, OU_ItemBanList))
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
			if (CheckTableForAbility(ability, OU_AbilityBanList))
				return TRUE;

			//Check Banned Moves
			for (i = 0; i < MAX_MON_MOVES; ++i) {
				if (CheckTableForMove(moveLoc[i], SmogonMoveBanList)
				|| moveLoc[i] == MOVE_BATONPASS)
					return TRUE;
			}
			break;

		case BATTLE_TOWER_UBER:
		//For Ubers ban Rayquaza with Dragon Ascent, Moody, and some moves
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
				if (CheckTableForMove(moveLoc[i], SmogonMoveBanList))
					return TRUE;
				if (species == SPECIES_RAYQUAZA && moveLoc[i] == MOVE_DRAGONASCENT)
					return TRUE;
			}
			break;

		case BATTLE_TOWER_LITTLE_CUP:
			if (!CheckTableForSpecies(species, LittleCup_SpeciesList))
				return TRUE; //Banned

			if (checkFromLocationType == CHECK_BATTLE_TOWER_SPREADS)
				moveLoc = spread->moves;
			else
				moveLoc = mon->moves;

			for (i = 0; i < MAX_MON_MOVES; ++i) {
				if (CheckTableForMove(moveLoc[i], LittleCup_MoveBanList))
					return TRUE;
			}
			break;

		case BATTLE_TOWER_MIDDLE_CUP:
			//To-Do
			break;
	}

	return FALSE; //Not banned
}

static bool8 IsPokemonBannedBasedOnStreak(u16 species, u16 item, u16* speciesArray, u8 monsCount, u16 trainerId, u8 tier, bool8 forPlayer)
{
	u16 streak;
	
	if (!forPlayer && trainerId == BATTLE_TOWER_TID && tier == BATTLE_TOWER_STANDARD)
	{
		streak = GetCurrentBattleTowerStreak();
				
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
			if (BaseStatsTotalGEAlreadyOnTeam(570, monsCount, speciesArray))
				return TRUE;
		}
	}
	else if (forPlayer)
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
	
	return FALSE;
}

static u16 GivePlayerFrontierMonGivenSpecies(const u16 species, const struct BattleTowerSpread* const spreadTable, const u16 numSpreads)
{
	struct Pokemon mon;
	const struct BattleTowerSpread* spread = GetSpreadBySpecies(species, spreadTable, numSpreads);
	
	if (spread == NULL)
		return 0xFFFF;

	CreateFrontierMon(&mon, 50, spread, 0, 0, 0, TRUE);
	return GiveMonToPlayer(&mon);
}

const struct BattleTowerSpread* GetSpreadBySpecies(const u16 species, const struct BattleTowerSpread* const spreads, const u16 numSpreads)
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

bool8 IsMonAllowedInBattleTower(struct Pokemon* mon)
{
	if (FlagGet(BATTLE_TOWER_FLAG))
	{
		u16 species = mon->species;
		u16 item = mon->item;

		if (GetMonData(mon, MON_DATA_IS_EGG, NULL))
			return FALSE;

		if (PokemonTierBan(species, item, NULL, mon, CHECK_PARTY_OFFSET))
			return FALSE;
		
		u8 tier = VarGet(BATTLE_TOWER_TIER);
		if (tier != BATTLE_TOWER_FREE_FOR_ALL) //Free for all has no duplicate species or item restriction
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
			|| (tier == BATTLE_TOWER_STANDARD && ItemAlreadyOnTeam(mon->item, partySize, itemArray)))
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
    ((struct Pokemon*) boxMon)->language = gGameLanguage;
    SetBoxMonData(boxMon, MON_DATA_OT_NAME, gSaveBlock2->playerName);
    ((struct Pokemon*) boxMon)->species  = species;
    ((struct Pokemon*) boxMon)->experience = gExperienceTables[gBaseStats[species].growthRate][level];
    SetBoxMonData(boxMon, MON_DATA_FRIENDSHIP, &gBaseStats[species].friendship);
    ((struct Pokemon*) boxMon)->metLocation = GetCurrentRegionMapSectionId();
    ((struct Pokemon*) boxMon)->metLevel = level;
    ((struct Pokemon*) boxMon)->metGame = gGameVersion;
    value = BALL_TYPE_POKE_BALL;
    SetBoxMonData(boxMon, MON_DATA_POKEBALL, &value);
    ((struct Pokemon*) boxMon)->otGender = gSaveBlock2->playerGender;

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
