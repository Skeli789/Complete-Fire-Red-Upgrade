#include "defines.h"
#include "defines_battle.h"
#include "../include/event_data.h"

#include "../include/new/helper_functions.h"
#include "../include/new/catching.h"
#include "../include/new/frontier.h"
#include "../include/new/build_pokemon.h"
#include "../include/new/multi.h"
#include "../include/random.h"
#include "../include/constants/items.h"

#include "Tables/Trainers_With_EVs_Table.h"

extern u8 gClassPokeBalls[NUM_TRAINER_CLASSES];

extern void GetFrontierTrainerName(u8* dst, u16 trainerId, u8 battlerNum);
extern void MultiInitPokemonOrder(void);
extern u16 RandRange(u16 min, u16 max);

u8 CreateNPCTrainerParty(pokemon_t* party, u16 trainerNum, bool8 firstTrainer, u8 side);
u8 BuildFrontierParty(pokemon_t* party, u16 trainerNum, bool8 firstTrainer, bool8 ForPlayer, u8 side);
void BuildRandomPlayerTeam(void);
void SetWildMonHeldItem(void);
void GiveMonNatureAndAbility(pokemon_t* mon, u8 nature, u8 abilityNum);
bool8 SpeciesAlreadyOnTeam(u16 species, u8 partySize, species_t* speciesArray);
bool8 ItemAlreadyOnTeam(u16 item, u8 partySize, item_t* itemArray);
bool8 MegastoneAlreadyOnTeam(u16 item, u8 partySize, item_t* itemArray);
bool8 ZCrystalAlreadyOnTeam(u16 item, u8 partySize, item_t* itemArray);
bool8 PokemonTierBan(u16 species, u16 item, struct BattleTowerSpread* spread, pokemon_t* mon, u8 checkFromLocationType);
u8 GetHighestMonLevel(pokemon_t* party);
u8 GetMonPokeBall(struct PokemonSubstruct0* data);
void SetMonPokeBall(struct PokemonSubstruct0* data, u8 ballId);

#ifdef OPEN_WORLD_TRAINERS

extern const u8 openWorldLevelRanges[NUM_BADGE_OPTIONS][2];
extern const species_t gGeneralTrainerSpreads[NUM_TRAINER_CLASSES][NUM_BADGE_OPTIONS][NUM_MONS_PER_BADGE];

static u8 GetOpenWorldTrainerMonAmount(void);
static u8 GetOpenWorldSpeciesIndex(u32 nameHash, u8 i);
static u8 GetOpenWorldSpeciesLevel(u32 nameHash, u8 i);
static u8 GetOpenWorldBadgeCount(void);

#endif

void BuildTrainerPartySetup(void) {
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
	
	if (gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER && ViableMonCount(gEnemyParty) > 1) {
		ExtensionState.partyBackup = Calloc(sizeof(struct Pokemon) * 6);
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
					Memcpy(&((pokemon_t*) ExtensionState.partyBackup)[counter++], &gPlayerParty[i], sizeof(struct Pokemon));				
			}
			ReducePartyToThree(); //Well...sometimes can be less than 3
		}
		Memset(&gPlayerParty[3], 0x0, sizeof(struct Pokemon) * 3);
		CreateNPCTrainerParty(&gPlayerParty[3], VarGet(PARTNER_VAR), FALSE, B_SIDE_PLAYER);
	}
	
	if (ViableMonCount(gEnemyParty) <= 1) //Error prevention
		gBattleTypeFlags &= ~(BATTLE_TYPE_INGAME_PARTNER | BATTLE_TYPE_TWO_OPPONENTS | BATTLE_TYPE_DOUBLE);
}

//Returns the number of Pokemon
u8 CreateNPCTrainerParty(pokemon_t* party, u16 trainerNum, bool8 firstTrainer, bool8 side) {
    u32 nameHash = 0;
    u32 personalityValue;
    int i, j;
    u8 monsCount = 1;
	u32 otid = (Random() << 8) | Random(); 
	
    if (trainerNum == 0x400) return 0;
    
    struct Trainer* trainer = &gTrainers[trainerNum];
    
    if (((gBattleTypeFlags & (BATTLE_TYPE_TRAINER | BATTLE_TYPE_EREADER_TRAINER | BATTLE_TYPE_TRAINER_TOWER)) == BATTLE_TYPE_TRAINER)
	||   (gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER))
	{
		if (firstTrainer)
			ZeroEnemyPartyMons(); //party_opponent_purge();
		
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
        for (i = 0; i < monsCount; ++i) {
		
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
				switch (gTrainers[trainerNum].partyFlags) {
					case 0:
						MAKE_POKEMON(trainer->party.NoItemDefaultMoves);
						break;

					case PARTY_FLAG_CUSTOM_MOVES:
						MAKE_POKEMON(trainer->party.NoItemCustomMoves);
						SET_MOVES(trainer->party.NoItemCustomMoves);
						break;
					
					case PARTY_FLAG_HAS_ITEM:				
						MAKE_POKEMON(trainer->party.ItemDefaultMoves);
						party[i].item = trainer->party.ItemDefaultMoves[i].heldItem;
						break;

					case PARTY_FLAG_CUSTOM_MOVES | PARTY_FLAG_HAS_ITEM:
						MAKE_POKEMON(trainer->party.ItemCustomMoves);
						SET_MOVES(trainer->party.ItemCustomMoves);
						party[i].item = trainer->party.ItemCustomMoves[i].heldItem;
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
				if (gTrainers[trainerNum].partyFlags == (PARTY_FLAG_CUSTOM_MOVES | PARTY_FLAG_HAS_ITEM)
				&& trainer->aiFlags > 1 
				&& spreadNum != 0
				&& spreadNum < ARRAY_COUNT(gTrainersWithEvsSpreads))
				{
					struct TrainersWithEvs spread = gTrainersWithEvsSpreads[spreadNum];
					
					SET_EVS(spread);
					SET_IVS_SINGLE_VALUE(MathMin(31, spread.ivs));
					
					u8 ballType;
					switch(spread.ball) {
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
							ballType = MathMin(NUM_BALLS, spread.ball);
					}
						
					SetMonData(&party[i], REQ_POKEBALL, &ballType);
					
					switch(spread.ability) {
						case Ability_Hidden:
						TRAINER_WITH_EV_GIVE_HIDDEN_ABILITY:
							GiveMonNatureAndAbility(&party[i], spread.nature, 0xFF); //Give Hidden Ability
							break;
						case Ability_1:
						case Ability_2:
							GiveMonNatureAndAbility(&party[i], spread.nature, MathMin(1, spread.ability - 1));
							break;
						case Ability_Random_1_2:
						TRAINER_WITH_EV_GIVE_RANDOM_ABILITY:
							GiveMonNatureAndAbility(&party[i], spread.nature, Random() % 2);
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
        }
		#ifdef OPEN_WORLD_TRAINERS
		if ((GetOpenWorldTrainerMonAmount() > 1 || trainer->doubleBattle)
		#else
		if (trainer->partySize > 1
		#endif
		&& ViableMonCount(gPlayerParty) >= 2) //Double battles will not happen if the player only has 1 mon that can fight or if the foe only has 1 poke
		{
			gBattleTypeFlags |= trainer->doubleBattle;
		
			#ifdef ACTIVATE_DOUBLE_BATTLE_FROM_FLAG
				if (FlagGet(DOUBLE_BATTLE_FLAG))
					gBattleTypeFlags |= BATTLE_TYPE_DOUBLE;
			#endif
		}

    }
    return monsCount;
}

//Returns the number of Pokemon
u8 BuildFrontierParty(pokemon_t* party, u16 trainerNum, bool8 firstTrainer, bool8 ForPlayer, u8 side) {
    int i, j;
    u8 monsCount;
	u32 otid = (Random() << 8) | Random(); 
	
	if (!ForPlayer) {
		if (trainerNum == 0x400) 
			return 0;
		else if (trainerNum != BATTLE_TOWER_TID) 
			return (CreateNPCTrainerParty(party, trainerNum, firstTrainer, side));
	}
	
	struct BattleTowerTrainer trainer = gTowerTrainers[VarGet(TOWER_TRAINER_ID_VAR + (firstTrainer ^ 1))];
    
	if (ForPlayer)
		ZeroPlayerPartyMons();
	else if (firstTrainer)
		ZeroEnemyPartyMons();
	
	if (ForPlayer)
		monsCount = PARTY_SIZE;
	else if (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS && VarGet(BATTLE_TOWER_POKE_NUM) > 3 && side == B_SIDE_OPPONENT)
        monsCount = 3;
	else if (gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER && VarGet(BATTLE_TOWER_POKE_NUM) > 3 && side == B_SIDE_PLAYER)
        monsCount = 3;
    else {
        monsCount = MathMax(1, MathMin(PARTY_SIZE, VarGet(BATTLE_TOWER_POKE_NUM)));
    }
	
	species_t* speciesArray = Calloc(sizeof(species_t) * monsCount);
	item_t* itemArray = Calloc(sizeof(item_t) * monsCount);
	
    for (i = 0; i < monsCount; ++i) {
		
        u8 loop = 1;
		struct BattleTowerSpread spread;
		u16 species;
		u16 item;
			
		do {
        spread = gFrontierSpreads[umodsi(Random(), TOTAL_SPREADS)];
		
		species = spread.species;
		item = spread.item;
				
		//Prevent duplicate species and items
		//Only allow one Mega Stone & Z-Crystal per team
		if (!SpeciesAlreadyOnTeam(species, monsCount, speciesArray) && 
			!ItemAlreadyOnTeam(item, monsCount, itemArray) && 
			!MegastoneAlreadyOnTeam(item, monsCount, itemArray) &&
			!ZCrystalAlreadyOnTeam(item, monsCount, itemArray) &&
			!PokemonTierBan(species, item, &spread, 0, CHECK_BATTLE_TOWER_SPREADS)) {
				speciesArray[i] = species;
				itemArray[i] = item;
				loop = 0;
		}
				
		} while (loop == 1);
		
		u8 level;
		if (VarGet(BATTLE_TOWER_TIER) == BATTLE_TOWER_LITTLE_CUP)
			level = 5;
		else
			level = MathMax(1, MathMin(MAX_LEVEL, VarGet(BATTLE_TOWER_POKE_LEVEL)));
		
		if (ForPlayer)
			CreateMon(&party[i], spread.species, level, 0, TRUE, 0, OT_ID_PLAYER_ID, otid);
		
		else {
			CreateMon(&party[i], spread.species, level, 0, TRUE, 0, OT_ID_PRESET, otid);
			
			GetFrontierTrainerName((u8*) &party[i].otname, trainerNum, firstTrainer ^ 1);
			party[i].otGender = trainer.gender;
		}
		
		party[i].metLocation = 0x3A; //Battle Tower RS
		party[i].metLevel = 50;
		party[i].obedient = 1;
		
		SET_IVS(spread);
		SET_EVS(spread);
		
		if (spread.ability > 0)
			GiveMonNatureAndAbility(&party[i], spread.nature, spread.ability - 1);
		
		else { //Hidden Ability
			GiveMonNatureAndAbility(&party[i], spread.nature, 0xFF);
			party[i].hiddenAbility = 1; //Hidden Ability Bit
		}
			
        for (j = 0; j < MAX_MON_MOVES; j++) {
			party[i].moves[j] = spread.moves[j];
			party[i].pp[j] = gBattleMoves[spread.moves[j]].pp;
		}
             
        party[i].item = item;
		
		u8 ballType;
		if (spread.ball)
			ballType = MathMin(NUM_BALLS, spread.ball);
		else
			ballType = umodsi(Random(), NUM_BALLS + 1);
		SetMonData(&party[i], REQ_POKEBALL, &ballType);
		
		CalculateMonStats(&party[i]);
		HealMon(&party[i]);
    }
	
	Free(speciesArray);
	Free(itemArray);
	
	if (!ForPlayer) { //Probably best to put these checks somewhere else
		if (VarGet(BATTLE_TOWER_BATTLE_TYPE) == BATTLE_TOWER_DOUBLE || VarGet(BATTLE_TOWER_BATTLE_TYPE) == BATTLE_TOWER_MULTI) 
			gBattleTypeFlags |= BATTLE_TYPE_DOUBLE;
	}
	
    return monsCount;
}

void BuildRandomPlayerTeam(void) {
	BuildFrontierParty(gPlayerParty, 0, TRUE, TRUE, B_SIDE_PLAYER);
}

void SetWildMonHeldItem(void)
{
	u16 rnd = umodsi(Random(), 100);
	u16 species;
    u16 var1 = 45;
    u16 var2 = 95;

    if (!GetMonData(&gPlayerParty[0], MON_DATA_IS_EGG, 0)
    && GetPartyAbility(&gPlayerParty[0]) == ABILITY_COMPOUNDEYES) {
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
				gEnemyParty[i].item = gBaseStats[species].item1;
				return;
			}

			if (rnd < var1)
				return;
		   
			if (rnd < var2)
				gEnemyParty[i].item = gBaseStats[species].item1;
			else
				gEnemyParty[i].item = gBaseStats[species].item2;
		}
    }
}

void GiveMonNatureAndAbility(pokemon_t* mon, u8 nature, u8 abilityNum) {
	u32 personality;
	
	while(1) {
		personality = (Random() << 8) | Random(); //Sets the personality value to an random number
		personality &= ~(1);
		
		if (abilityNum != 0xFF)
			personality |= MathMin(1, abilityNum); //Either 0 or 1
			
		if (umodsi(personality, NUM_NATURES) == nature) { //Compare nature from personality
			mon->personality = personality;
			break;
		}
	}
}

bool8 SpeciesAlreadyOnTeam(u16 species, u8 partySize, species_t* speciesArray) {
	for (int i = 0; i < partySize; ++i) {
		if (SpeciesToNationalPokedexNum(speciesArray[i]) == SpeciesToNationalPokedexNum(species))
			return TRUE;
	}
	
	return FALSE;
}

bool8 ItemAlreadyOnTeam(u16 item, u8 partySize, item_t* itemArray) {
	
	if (item == 0) return FALSE;
	
	for (int i = 0; i < partySize; ++i) {
		if (itemArray[i] == item)
			return TRUE;
	}

	return FALSE;
}

bool8 MegastoneAlreadyOnTeam(u16 item, u8 partySize, item_t* itemArray) {

	if (ItemId_GetHoldEffect(item) != ITEM_EFFECT_MEGA_STONE) 
		return FALSE;
	
	for (int i = 0; i < partySize; ++i) {
		if (ItemId_GetHoldEffect(itemArray[i]) == ITEM_EFFECT_MEGA_STONE)
			return TRUE;
	}

	return FALSE;
}


bool8 ZCrystalAlreadyOnTeam(u16 item, u8 partySize, item_t* itemArray) {

	if (ItemId_GetHoldEffect(item) != ITEM_EFFECT_Z_CRYSTAL) 
		return FALSE;
	
	for (int i = 0; i < partySize; ++i) {
		if (ItemId_GetHoldEffect(itemArray[i]) == ITEM_EFFECT_Z_CRYSTAL)
			return TRUE;
	}

	return FALSE;
}

bool8 PokemonTierBan(u16 species, u16 item, struct BattleTowerSpread* spread, pokemon_t* mon, u8 checkFromLocationType) {
	u32 i;
	u8 ability;
	u16* moveLoc;
	
	if (species == SPECIES_EGG)
		return 1;
		
	u16 tier = VarGet(BATTLE_TOWER_TIER);
	
	switch (tier) {
		case BATTLE_TOWER_STANDARD:
			for (i = 0; StandardSpeciesBanList[i] != SPECIES_TABLES_TERMIN; ++i) {
				if (species == StandardSpeciesBanList[i])
					return TRUE;
			}
			for (i = 0; StandardItemBanList[i] != ITEM_TABLES_TERMIN; ++i) {
				if (item == StandardItemBanList[i])
					return TRUE;
			}
			break;
		
		case BATTLE_TOWER_OU:
		//For OU, there's a species, item, ability, and move ban list
			for (i = 0; OU_SpeciesBanList[i] != SPECIES_TABLES_TERMIN; ++i) {
				if (species == OU_SpeciesBanList[i])
					return TRUE;
			}
			for (i = 0; OU_ItemBanList[i] != ITEM_TABLES_TERMIN; ++i) {
				if (item == OU_ItemBanList[i])
					return TRUE;
			}
			
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
			
		case BATTLE_TOWER_LITTLE_CUP:	;
			bool8 inSpeciesList = FALSE;
			
			for (i = 0; LittleCup_SpeciesList[i] != SPECIES_TABLES_TERMIN; ++i) {
				if (species == LittleCup_SpeciesList[i]) {
					inSpeciesList = TRUE;
					break;
				}
			}
			if (!inSpeciesList)
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

u8 GetHighestMonLevel(pokemon_t* party)
{
	u8 max = party[0].level;
	
	for (int i = 0; i < 6; ++i)
	{
		if (max == MAX_LEVEL)
			return max;
	
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

void BattlePokemonScriptCommand_GiveHiddenAbility(pokemon_t* mon)
{
	if (FlagGet(HIDDEN_ABILITY_FLAG))
	{
		FlagClear(HIDDEN_ABILITY_FLAG);
		mon->hiddenAbility = TRUE;
	}
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


u32 CheckShinyMon(bool8 hasFixedPersonality, u32 personality) {
	u16 numerator = 1;	//default 1/4096 rate
	u32 pid;
	
	if (hasFixedPersonality)
		pid = personality;
	else
		pid = Random32();
	
	if (CheckBagHasItem(ITEM_SHINY_CHARM, 1) > 0)
		numerator = 3;
	
	if (RandRange(0,4097) < numerator)
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

