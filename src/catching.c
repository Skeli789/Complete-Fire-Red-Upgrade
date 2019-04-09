#include "defines.h"
#include "defines_battle.h"
#include "../include/constants/items.h"
#include "../include/new/helper_functions.h"
#include "../include/new/catching.h"

#define gOpenPokeballGfx (u8*) 0x8D022E8

extern const struct BallIdItemIdRelation BallIdItemIdRelations[];
extern const struct CompressedSpriteSheet gBallSpriteSheets[];
extern const struct CompressedSpritePalette gBallSpritePalettes[];

extern u8 gText_CantAimAtTwoTargets[];
extern u8 gText_CantAimAtSemiInvulnerableTarget[];

void atkEF_handleballthrow(void);
u8 GetCatchingBattler(void);
bool8 CriticalCapture(u32 odds);
u8 GiveMonToPlayer(pokemon_t* mon);
u8 ItemIdToBallId(u16 ballItem);
item_t BallIdToItemId(u8 ballId);
u16 GetBattlerPokeballItemId(u8 bank);
bool8 DoubleWildPokeBallItemUseFix(u8 taskId);
pokemon_t* LoadTargetPartyData(void);

extern void TryFormRevert(pokemon_t* mon);
extern void TryRevertMega(pokemon_t* mon);

void atkEF_handleballthrow(void) {
	if (gBattleExecBuffer) return;
	
    u8 ball_multiplier = 0;
	
    gActiveBattler = gBankAttacker;
    gBankTarget = GetCatchingBattler();
	
	u16 atkSpecies = gBattleMons[gBankAttacker].species;
	u16 defSpecies = gBattleMons[gBankTarget].species;
	u8 atkLevel = gBattleMons[gBankAttacker].level;
	u8 defLevel = gBattleMons[gBankTarget].level;
	
	u8 ItemType = ItemId_GetType(gLastUsedItem);
	
    if (gBattleTypeFlags & BATTLE_TYPE_TRAINER) {
		#ifdef CATCH_TRAINERS_POKEMON
			if (FlagGet(CATCH_TRAINERS_POKEMON_FLAG)) {
				EmitBallThrowAnim(0, 4);
				MarkBufferBankForExecution(gActiveBattler);
				gBattlescriptCurrInstr = BattleScript_SuccessBallThrow;
				if (ItemType != BALL_TYPE_PARK_BALL)
					SetMonData(&gEnemyParty[gBattlerPartyIndexes[gBankTarget]], MON_DATA_POKEBALL, (const void*) &ItemType);
				if (CalculatePlayerPartyCount() == 6)
					gBattleCommunication[MULTISTRING_CHOOSER] = 0;
				else
					gBattleCommunication[MULTISTRING_CHOOSER] = 1;
			}
			else {
				EmitBallThrowAnim(0, 5);
				MarkBufferBankForExecution(gActiveBattler);
				gBattlescriptCurrInstr = BattleScript_TrainerBallBlock;
			}
		
		#else
			EmitBallThrowAnim(0, 5);
			MarkBufferBankForExecution(gActiveBattler);
			gBattlescriptCurrInstr = BattleScript_TrainerBallBlock;
		#endif
    }
	
	else if ((gBattleTypeFlags & BATTLE_TYPE_GHOST) || FlagGet(NO_CATCHING_FLAG) || FlagGet(NO_CATCHING_AND_RUNNING_FLAG)) {
		EmitBallThrowAnim(0, 6);
		MarkBufferBankForExecution(gActiveBattler);
		gBattlescriptCurrInstr = BattleScript_DodgedBall;
	}
	
    else if (gBattleTypeFlags & (BATTLE_TYPE_OLD_MAN | BATTLE_TYPE_POKE_DUDE)) {
        EmitBallThrowAnim(0, 4);
        MarkBufferBankForExecution(gActiveBattler);
        gBattlescriptCurrInstr = BattleScript_TutorialThrow;
    }
	
    else {
        u32 odds;
        u8 catch_rate;
		
        if (ItemType == BALL_TYPE_SAFARI_BALL)
            catch_rate = udivsi(gBattleStruct->safariCatchFactor * 1275, 100);
        else
            catch_rate = gBaseStats[GetBankPartyData(gBankTarget)->species].catchRate; //Uses party data b/c Transform update Gen 5+
		
        if (ItemType > 5) {
            switch (ItemType) {
				case BALL_TYPE_NET_BALL:
					if (IsOfType(gBankTarget, TYPE_WATER) || IsOfType(gBankTarget, TYPE_BUG))
						ball_multiplier = 35;
					else
						ball_multiplier = 10;
					break;
					
				case BALL_TYPE_DIVE_BALL:
					if (Overworld_GetMapTypeOfSaveblockLocation() == MAP_TYPE_UNDERWATER
					|| FishingByte || TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_SURFING))
						ball_multiplier = 35;
					else
						ball_multiplier = 10;
					break;
					
				case BALL_TYPE_NEST_BALL:
					if (gBattleMons[gBankTarget].level <= 29)
						ball_multiplier = MathMax(10, 41 - defLevel);
					else
						ball_multiplier = 10;
					break;
					
				case BALL_TYPE_REPEAT_BALL:
					if (GetSetPokedexFlag(SpeciesToNationalPokedexNum(defSpecies), FLAG_GET_CAUGHT))
						ball_multiplier = 35;
					else
						ball_multiplier = 10;
					break;
					
				case BALL_TYPE_TIMER_BALL:
					ball_multiplier = MathMax(40, 10 + gBattleResults->battleTurnCounter * 3);
					break;
					
				case BALL_TYPE_LUXURY_BALL:
				case BALL_TYPE_PREMIER_BALL:
				case BALL_TYPE_CHERISH_BALL:
				case BALL_TYPE_FRIEND_BALL:
				case BALL_TYPE_HEAL_BALL:
					ball_multiplier = 10;
					break;
				
				case BALL_TYPE_LEVEL_BALL:
					if (atkLevel <= defLevel)
						ball_multiplier = 10;
					else if (atkLevel > defLevel && atkLevel < defLevel * 2)
						ball_multiplier = 20;
					else if (atkLevel > defLevel * 2 && atkLevel < defLevel * 4)
						ball_multiplier = 40;
					else
						ball_multiplier = 80;
					break;
					
				case BALL_TYPE_LURE_BALL:
					if (FishingByte)
						ball_multiplier = 50;
					else
						ball_multiplier = 10;
					break;
				
				case BALL_TYPE_MOON_BALL:
					switch (SpeciesToNationalPokedexNum(defSpecies)) {
						case PKDX_NIDORAN_F:
						case PKDX_NIDORINA:
						case PKDX_NIDOQUEEN:
						case PKDX_NIDORAN_M:
						case PKDX_NIDORINO:
						case PKDX_NIDOKING:
						case PKDX_JIGGLYPUFF:
						case PKDX_WIGGLYTUFF:
						case PKDX_CLEFAIRY:
						case PKDX_CLEFABLE:
						case PKDX_IGGLYBUFF:
						case PKDX_CLEFFA:
						case PKDX_SKITTY:
						case PKDX_DELCATTY:
						case PKDX_MUNNA:
						case PKDX_MUSHARNA:
							ball_multiplier = 40;
							break;
						
						default:
							ball_multiplier = 10;
					}
					break;
				
				case BALL_TYPE_LOVE_BALL:
					if (SpeciesToNationalPokedexNum(atkSpecies) == SpeciesToNationalPokedexNum(defSpecies)) {
						u8 atkGender = GetGenderFromSpeciesAndPersonality(atkSpecies, gBattleMons[gBankAttacker].personality);
						u8 defGender = GetGenderFromSpeciesAndPersonality(defSpecies, gBattleMons[gBankTarget].personality);
						
						if (atkGender != 0xFF && defGender != 0xFF && atkGender != defGender)
							ball_multiplier = 80;
						else
							ball_multiplier = 10;
					}
					else
						ball_multiplier = 10;
					break;
				
				//Heavy Ball modifies the catch rate itself, not the multiplier
				case BALL_TYPE_HEAVY_BALL:	;
					ball_multiplier = 10;
					//Apparently abilities or Float Stone have no affect here
					u16 defWeight = GetPokedexHeightWeight(SpeciesToNationalPokedexNum(defSpecies), PKDX_GET_WEIGHT);
					u8 old_catch_rate;
					
					if (defWeight < 1000) {
						old_catch_rate = catch_rate;
						catch_rate -= 20;
						if (catch_rate > old_catch_rate)
							catch_rate = 1; //New in USUM
					}
					else if (defWeight >= 2000 && defWeight < 3000)
						catch_rate += 20;
					else if (defWeight > 3000)
						catch_rate += 30;
					break;
				
				case BALL_TYPE_FAST_BALL:
					if (gBaseStats[gBankTarget].baseSpeed >= 100)
						ball_multiplier = 40;
					break;
				
				case BALL_TYPE_SPORT_BALL:
					ball_multiplier = 15;
					break;
				
				case BALL_TYPE_DUSK_BALL:
					if (Overworld_GetMapTypeOfSaveblockLocation() == MAP_TYPE_UNDERGROUND)
						ball_multiplier = DUSK_BALL_MULTIPLIER;
						
					#ifdef TIME_ENABLED
					else if (Clock->hour >= TIME_NIGHT_START || Clock->hour < TIME_MORNING_START)
						ball_multiplier = DUSK_BALL_MULTIPLIER;
					#endif
					break;
				
				case BALL_TYPE_QUICK_BALL:
					if (gBattleResults->battleTurnCounter == 0)
						ball_multiplier = 50;
					else
						ball_multiplier = 10;
					break;
				
				case BALL_TYPE_BEAST_BALL:
					switch (SpeciesToNationalPokedexNum(defSpecies)) {
						case PKDX_NIHILEGO:
						case PKDX_BUZZWOLE:
						case PKDX_PHEROMOSA:
						case PKDX_XURKITREE:
						case PKDX_CELESTEELA:
						case PKDX_KARTANA:
						case PKDX_GUZZLORD:
						case PKDX_POIPOLE:
						case PKDX_NAGANADEL:
						case PKDX_STAKATAKA:
						case PKDX_BLACEPHALON:
							ball_multiplier = 50;
							break;
						
						default:
							ball_multiplier = 1;
					}
					break;
			}
        }
        else
            ball_multiplier = sBallCatchBonuses[ItemType - BALL_TYPE_ULTRA_BALL];
		
        odds = udivsi(((catch_rate * udivsi(ball_multiplier, 10)) * (gBattleMons[gBankTarget].maxHP * 3 - gBattleMons[gBankTarget].hp * 2)), (3 * gBattleMons[gBankTarget].maxHP));
		
		#ifndef NO_HARDER_WILD_DOUBLES
		if (!(gBattleTypeFlags & BATTLE_TYPE_TRAINER) && (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)) {
			u16 PokesCaught = GetNationalPokedexCount(FLAG_GET_CAUGHT);
			if (PokesCaught < 30)
				odds = udivsi(odds * 30, 100);
			else if (PokesCaught <= 150)
				odds = udivsi(odds * 50, 100);
			else if (PokesCaught <= 300)
				odds = udivsi(odds * 70, 100);
			else if (PokesCaught <= 450)
				odds = udivsi(odds * 80, 100);
			else if (PokesCaught <= 600)
				odds = udivsi(odds * 80, 100);
		}
		#endif
		
        if (gBattleMons[gBankTarget].status1 & (STATUS_SLEEP | STATUS_FREEZE))
            odds = udivsi(odds * 250, 100);
        if (gBattleMons[gBankTarget].status1 & (STATUS_PSN_ANY | STATUS_BURN | STATUS_PARALYSIS))
            odds = udivsi((odds * 150), 100);

        if (ItemType != ITEM_SAFARI_BALL) {
            if (ItemType == BALL_TYPE_MASTER_BALL)
                gBattleResults->usedMasterBall = 1;
			
			//This was used for the TV shows in Ruby, but seems kind of pointless in FR.
			//Commenting it out also prevents errors from using poke balls with large indices.
            //else if (gBattleResults.usedBalls[ItemType - BALL_TYPE_ULTRA_BALL] < 0xFF)
            //        gBattleResults.usedBalls[ItemType - BALL_TYPE_ULTRA_BALL]++;
        }
		
        if (odds >= 0xFF) { //Poke Caught
            EmitBallThrowAnim(0, 4);
            MarkBufferBankForExecution(gActiveBattler);
            gBattlescriptCurrInstr = BattleScript_SuccessBallThrow;
			
			if (ItemType != BALL_TYPE_PARK_BALL) //Might crash the game if using Park Ball on regular Pokemon. Or will load garbage data for ball
				SetMonData(&gEnemyParty[gBattlerPartyIndexes[gBankTarget]], MON_DATA_POKEBALL, (const void*) &ItemType);
			
            if (CalculatePlayerPartyCount() == 6)
                gBattleCommunication[MULTISTRING_CHOOSER] = 0;
            else
                gBattleCommunication[MULTISTRING_CHOOSER] = 1;
        }
		
        else { //Poke may be caught, calculate shakes
            u8 shakes, maxShakes;
			if (CriticalCapture(odds))
				maxShakes = 2;
			else
				maxShakes = 4;
			
			if (ItemType == BALL_TYPE_MASTER_BALL
			||	ItemType == BALL_TYPE_DREAM_BALL
			||	ItemType == BALL_TYPE_PARK_BALL)
                shakes = maxShakes;
				
			else {
				odds = udivsi(0xFFFF0, Sqrt(Sqrt(udivsi(0xFF0000, odds))));
				for (shakes = 0; shakes < maxShakes && Random() < odds; ++shakes) ;
			}
        
            EmitBallThrowAnim(0, shakes);
            MarkBufferBankForExecution(gActiveBattler);
			
            if (shakes == maxShakes) {
                gBattlescriptCurrInstr = BattleScript_SuccessBallThrow;
				
				if (ItemType != BALL_TYPE_PARK_BALL) //May crash the game if using Park Ball on regular Pokemon
					SetMonData(&gEnemyParty[gBattlerPartyIndexes[gBankTarget]], MON_DATA_POKEBALL, (const void*) &ItemType);
				
                if (CalculatePlayerPartyCount() == 6)
                    gBattleCommunication[MULTISTRING_CHOOSER] = 0;
                else
                    gBattleCommunication[MULTISTRING_CHOOSER] = 1;
            }
			
            else { //rip
                gBattleCommunication[MULTISTRING_CHOOSER] = shakes;
                gBattlescriptCurrInstr = BattleScript_ShakeBallThrow;
            }
        }
    }
}

u8 GetCatchingBattler(void) {
    if (IsBattlerAlive(GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT)))
        return GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);
    else
        return GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT);
}

bool8 CriticalCapture(u32 odds) {
	#ifndef CRITICAL_CAPTURE
		odds += 1; //So the compiler doesn't complain
		return FALSE;
	#else
	u16 PokesCaught = GetNationalPokedexCount(FLAG_GET_CAUGHT);
	if (PokesCaught <= 30)
		odds = 0;
	else if (PokesCaught <= 150)
		odds /= 2;
	else if (PokesCaught <= 300)
		;
	else if (PokesCaught <= 450)
		odds = udivsi(odds * 150, 100);
	else if (PokesCaught <= 600)
		odds *= 2;
	else
		odds = udivsi(odds * 250, 100);
	
	odds = udivsi(odds, 6);
	if (umodsi(Random(), 0xFF) < odds)
	{
		gNewBS->criticalCapture = TRUE;
		return TRUE;
	}
	else
	{
		gNewBS->criticalCapture = FALSE;
		return FALSE;
	}
	#endif
}

u8 GiveMonToPlayer(pokemon_t* mon) { //Hook in
    int i;

	TryFormRevert(mon);
	TryRevertMega(mon);

    SetMonData(mon, MON_DATA_OT_NAME, gSaveBlock2->playerName);
    SetMonData(mon, MON_DATA_OT_GENDER, &gSaveBlock2->playerGender);
    SetMonData(mon, MON_DATA_OT_ID, gSaveBlock2->playerTrainerId);
	
	if (ItemId_GetType(gLastUsedItem) == BALL_TYPE_HEAL_BALL)
		HealMon(mon);
	else if (ItemId_GetType(gLastUsedItem) == BALL_TYPE_FRIEND_BALL)
		mon->friendship = 200;
	
    i = 0;

    while (i < 6 && gPlayerParty[i].species != PKMN_NONE)
        ++i;

    if (i >= 6)
        return SendMonToPC(mon);

    CopyMon(&gPlayerParty[i], mon, sizeof(*mon));
    gPlayerPartyCount = i + 1;
    return 0;
}

u8 ItemIdToBallId(u16 ballItem)
{
	switch (ballItem) {
		case ITEM_POKE_BALL:
			return 0;
		case ITEM_GREAT_BALL:
			return 1;
		case ITEM_SAFARI_BALL:
			return 2;
		case ITEM_ULTRA_BALL:
			return 3;
		case ITEM_MASTER_BALL:
			return 4;
		default:
			if (ItemId_GetType(ballItem) > 0)
				return ItemId_GetType(ballItem) - 1;
			else
				return 0;
	}
}

void LoadBallGfx(u8 ballId)
{
    u16 var;
    if (GetSpriteTileStartByTag(gBallSpriteSheets[ballId].tag) == 0xFFFF)
    {
        LoadCompressedSpriteSheetUsingHeap(&gBallSpriteSheets[ballId]);
        LoadCompressedSpritePaletteUsingHeap(&gBallSpritePalettes[ballId]);
    }
    switch (ballId + 1) {
		case BALL_TYPE_MASTER_BALL:
		case BALL_TYPE_ULTRA_BALL:
		case BALL_TYPE_GREAT_BALL:
		case BALL_TYPE_POKE_BALL:
		case BALL_TYPE_SAFARI_BALL:
		case BALL_TYPE_NET_BALL:
		case BALL_TYPE_NEST_BALL:
		case BALL_TYPE_REPEAT_BALL:
		case BALL_TYPE_TIMER_BALL:
			var = GetSpriteTileStartByTag(gBallSpriteSheets[ballId].tag);
			LZDecompressVram(gOpenPokeballGfx, (void*)(VRAM + 0x10100 + var * 32));
			break;
    }
}

item_t BallIdToItemId(u8 ballId)
{
	for (int i = 0; i < NUM_BALLS; ++i)
	{
		if (BallIdItemIdRelations[i].ballId == ballId)
			return BallIdItemIdRelations[i].itemId;
	}
	
	return ITEM_NONE;
}

u16 GetBattlerPokeballItemId(u8 bank)
{
	u8 ballId;

    if (SIDE(bank) == B_SIDE_PLAYER)
	{
        ballId = GetMonData(&gPlayerParty[gBattlerPartyIndexes[bank]], MON_DATA_POKEBALL, 0);
	}
    else
	{
        ballId = GetMonData(&gEnemyParty[gBattlerPartyIndexes[bank]], MON_DATA_POKEBALL, 0);
	}
		
	return BallIdToItemId(ballId);
}

bool8 CriticalCapturAnimUpdate(void)
{
	return gNewBS->criticalCapture;
}

bool8 DoubleWildPokeBallItemUseFix(u8 taskId)
{
	bool8 effect = FALSE;

	if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
	{
		if (BATTLER_ALIVE(GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT))
		&& BATTLER_ALIVE(GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT)))
		{
			DisplayItemMessage(taskId, 2, gText_CantAimAtTwoTargets, bag_menu_inits_lists_menu);
			effect = TRUE;
		}
		else if ((BATTLER_ALIVE(GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT)) && BATTLER_SEMI_INVULERABLE(GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT)))
		||       (BATTLER_ALIVE(GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT)) && BATTLER_SEMI_INVULERABLE(GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT))))
		{
			DisplayItemMessage(taskId, 2, gText_CantAimAtSemiInvulnerableTarget, bag_menu_inits_lists_menu);
			effect = TRUE;
		}
	}
	else if (BATTLER_SEMI_INVULERABLE(GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT)))
	{
		DisplayItemMessage(taskId, 2, gText_CantAimAtSemiInvulnerableTarget, bag_menu_inits_lists_menu);
		effect = TRUE;
	}

	return effect;
}

pokemon_t* LoadTargetPartyData(void)
{
	return GetBankPartyData(gBankTarget);
}