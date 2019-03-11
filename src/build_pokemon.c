#include "defines.h"
#include "helper_functions.h"
#include "catching.h"
#include "frontier.h"
#include "build_pokemon.h"
#include "multi.h"

extern u8 ClassPokeBalls[NUM_TRAINER_CLASSES];

extern void GetFrontierTrainerName(u8* dst, u16 trainerId, u8 battlerNum);
extern void MultiInitPokemonOrder(void);

u8 CreateNPCTrainerParty(pokemon_t* party, u16 trainerNum, bool8 firstTrainer, u8 side);
u8 BuildFrontierParty(pokemon_t* party, u16 trainerNum, bool8 firstTrainer, bool8 ForPlayer, u8 side);
void BuildRandomPlayerTeam(void);
void SetWildMonHeldItem(void);
void GiveMonNatureAndAbility(pokemon_t* mon, u8 nature, u8 abilityNum);
bool8 SpeciesAlreadyOnTeam(u16 species, u8 partySize, species_t* speciesArray);
bool8 ItemAlreadyOnTeam(u16 item, u8 partySize, item_t* itemArray);
bool8 MegastoneAlreadyOnTeam(u16 item, u8 partySize, item_t* itemArray);
bool8 ZCrystalAlreadyOnTeam(u16 item, u8 partySize, item_t* itemArray);
bool8 PokemonTierBan(u16 species, u16 item, struct BattleTowerSpreads* spread, pokemon_t* mon, u8 checkFromLocationType);
u8 GetHighestMonLevel(pokemon_t* party);
u8 GetMonPokeBall(struct PokemonSubstruct0* data);
void SetMonPokeBall(struct PokemonSubstruct0* data, u8 ballId);

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
	
	if (gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER) {
		ExtensionState.partyBackup = Calloc(sizeof(struct Pokemon) * 5); //Space of 5 because you can choose to only use 1 mon
		if (!ExtensionState.partyBackup)
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
    
    if ((gBattleTypeFlags & (BATTLE_TYPE_TRAINER | BATTLE_TYPE_EREADER_TRAINER | BATTLE_TYPE_TRAINER_TOWER)) == BATTLE_TYPE_TRAINER) 
	{
		if (firstTrainer)
			ZeroEnemyPartyMons(); //party_opponent_purge();
			
		if (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS && side == B_SIDE_OPPONENT)
        {
            if (trainer->partySize > 3)
                monsCount = 3;
            else
                monsCount = trainer->partySize;
        }
        else if (gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER && side == B_SIDE_PLAYER)
        {
            if (trainer->partySize > 3)
                monsCount = 3;
            else
                monsCount = trainer->partySize;
        }
		else
            monsCount = trainer->partySize;
		
        
        for (i = 0; i < monsCount; ++i) {
		
			if (trainer->doubleBattle)
				personalityValue = 0x80;
			else if (trainer->gender) //Female
				personalityValue = 0x78;
			else
				personalityValue = 0x88;
				
			for (j = 0; trainer->trainerName[j] != EOS; ++j)
				nameHash += trainer->trainerName[j];
			
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
			
			u8 otGender = trainer->gender;
			SetMonData(&party[i], REQ_OTGENDER, &otGender);
			SetMonData(&party[i], REQ_OTNAME, &trainer->trainerName);
			party[i].metLocation = gCurrentMapName;
			party[i].obedient = 1;
		
			#ifdef TRAINER_CLASS_POKE_BALLS
				#ifdef CLASS_BASED_BALLS_MULTI_FIX
				//In multi battles, the second opponent and partner will share poke ball first the first opponent due to graphical issues
					if (side == B_SIDE_OPPONENT)
						SetMonData(&party[i], REQ_POKEBALL, &ClassPokeBalls[gTrainers[gTrainerBattleOpponent_A].trainerClass]);
					else
						SetMonData(&party[i], REQ_POKEBALL, &ClassPokeBalls[trainer->trainerClass]);
				#else
					SetMonData(&party[i], REQ_POKEBALL, &ClassPokeBalls[trainer->trainerClass]);
				#endif
			#endif
			
			#ifdef TRAINERS_WITH_EVS
				u8 spreadNum = trainer->party.NoItemCustomMoves[i].iv;
				if (trainer->aiFlags > 1 
				&& spreadNum != 0
				&& spreadNum < TRAINERS_WITH_EVS_TABLE_SIZE)
				{
					struct TrainersWithEvs spread = TrainersWithEvsTable[spreadNum];
					
					SET_EVS(spread);
					SET_IVS_SINGLE_VALUE(MathMin(31, spread.ivs));
					
					u8 ballType;
					if (spread.ball != 0xFF)
						ballType = MathMin(NUM_BALLS, spread.ball);
					else
						ballType = umodsi(Random(), NUM_BALLS + 1); //Set Random Ball
					SetMonData(&party[i], REQ_POKEBALL, &ballType);
					
					if (spread.ability == 0)
						GiveMonNatureAndAbility(&party[i], spread.nature, 0xFF); //Give Hidden Ability
					else
						GiveMonNatureAndAbility(&party[i], spread.nature, MathMin(1, spread.ability - 1));
				}
			#endif
			
			CalculateMonStats(&party[i]);
			HealMon(&party[i]);
        }
		if (trainer->partySize > 1 && ViableMonCount(gPlayerParty) >= 2) //Double battles will not happen if the player only has 1 mon that can fight or if the foe only has 1 poke
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
        monsCount = VarGet(BATTLE_TOWER_POKE_NUM);
    }
	
	species_t* speciesArray = Calloc(sizeof(species_t) * monsCount);
	item_t* itemArray = Calloc(sizeof(item_t) * monsCount);
	
    for (i = 0; i < monsCount; ++i) {
		
        u8 loop = 1;
		struct BattleTowerSpreads spread;
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
			level = VarGet(BATTLE_TOWER_POKE_LEVEL);
		
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

bool8 PokemonTierBan(u16 species, u16 item, struct BattleTowerSpreads* spread, pokemon_t* mon, u8 checkFromLocationType) {
	u32 i;
	u8 ability;
	u16* moveLoc;
	
	if (species == PKMN_EGG)
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
				if (species == PKMN_RAYQUAZA && moveLoc[i] == MOVE_DRAGONASCENT)
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