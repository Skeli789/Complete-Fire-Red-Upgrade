#include "defines.h"
#include "helper_functions.h"
#include "multi.h"

#define BattleScript_LinkBattleWonOrLost (u8*) 0x81D88CC
#define BattleScript_PayDayMoneyAndPickUpItems (u8*) 0x81D8803
#define BattleScript_LocalBattleLost (u8*) 0x81D8806
#define BattleScript_GotAwaySafely (u8*) 0x81D8916
#define BattleScript_RanAwayUsingMonAbility (u8*) 0x81D890F
#define BattleScript_SmokeBallEscape (u8*) 0x81D8901

void HandleEndTurn_BattleWon(void);
void HandleEndTurn_BattleLost(void);
void HandleEndTurn_RanFromBattle(void);
void EndOfBattleThings(void);
void RestoreNonConsumableItems(void);
void RecalcAllStats(void);
void BringBackTheDead(void);
void EndPartnerBattlePartyRestore(void);
void EndSkyBattlePartyRestore(void);
void EndBattleFlagClear(void);
bool8 IsConsumable(u16 item);

extern void FormsRevert(pokemon_t* party);
extern void MegaRevert(void);
extern void UpdateBurmy(void);

extern u8 BattleScript_Victory[];
extern u8 BattleScript_PrintPlayerForfeited[];
extern u8 BattleScript_PrintPlayerForfeitedLinkBattle[];
extern u8 BattleScript_LostMultiBattleTower[];
extern u8 BattleScript_LostBattleTower[];

extern u8 ConsumableItemEffectTable[];

void HandleEndTurn_BattleWon(void)
{
    gCurrentActionFuncId = 0;

    if (gBattleTypeFlags & BATTLE_TYPE_LINK)
    {
        //gSpecialVar_Result = gBattleOutcome;
        gBattleTextBuff1[0] = gBattleOutcome;
        gBankAttacker = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);
        gBattlescriptCurrInstr = BattleScript_LinkBattleWonOrLost;
        gBattleOutcome &= ~(B_OUTCOME_LINK_BATTLE_RAN);
    }
    else if (gBattleTypeFlags & BATTLE_TYPE_TRAINER
          && gBattleTypeFlags & (BATTLE_TYPE_FRONTIER | BATTLE_TYPE_EREADER_TRAINER | BATTLE_TYPE_TRAINER_TOWER))
    {
        BattleStopLowHpSound();
        gBattlescriptCurrInstr = BattleScript_Victory;

        PlayBGM(BGM_VICTORY_TRAINER_BATTLE);
    }
    else if (gBattleTypeFlags & BATTLE_TYPE_TRAINER && !(gBattleTypeFlags & BATTLE_TYPE_LINK))
    {
        BattleStopLowHpSound();
        gBattlescriptCurrInstr = BattleScript_Victory;
		
		u16 id = gTrainerBattleOpponent_A;
		u8 specialMus = FALSE;
		u8 loop = FALSE;
	
	VICTORY_MUSIC_SELECTION:
        switch (gTrainers[id].trainerClass) {
		#ifndef UNBOUND //Change this part
			case TRAINER_CLASS_LEADER:
			case TRAINER_CLASS_ELITE_4:
			case TRAINER_CLASS_CHAMPION:
				PlayBGM(BGM_VICTORY_SPECIAL);
				specialMus = TRUE;
				break;
			default:
				PlayBGM(BGM_VICTORY_TRAINER_BATTLE);
				break;
		
		#else //For Pokemon Unbound
			case TRAINER_CLASS_CHAMPION:
				PlayBGM(BGM_VICTORY_CHAMPION);
				specialMus = TRUE;
				break;
			case TRAINER_CLASS_ELITE_4:
				PlayBGM(BGM_VICYORY_ELITE_4);
				specialMus = TRUE;
				break;
			case TRAINER_CLASS_LEADER:
				PlayBGM(BGM_VICYORY_GYM);
				specialMus = TRUE;
				break;
			case TRAINER_CLASS_SHADOW_ADMIN: //0x30
			case TRAINER_CLASS_BOSS: //0x53
			case TRAINER_CLASS_SHADOW: //0x55
				PlayBGM(BGM_VICYORY_PLASMA);
				specialMus = TRUE;
				break;
			case TRAINER_CLASS_LOR_LEADER: //0x2
			case TRAINER_CLASS_LOR_ADMIN: //0x2E
			case TRAINER_CLASS_LOR: //0x2F
				PlayBGM(BGM_VICYORY_GALACTIC);
				specialMus = TRUE;
				break;
			default:
				PlayBGM(BGM_VICTORY_TRAINER_BATTLE);
				break;
		#endif
        }
		
		if (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS && !specialMus && !loop) {
			id = VarGet(SECOND_OPPONENT_VAR);
			loop = TRUE;
			goto VICTORY_MUSIC_SELECTION;
		}
    }
    else
        gBattlescriptCurrInstr = BattleScript_PayDayMoneyAndPickUpItems;

    gBattleMainFunc = (u32) HandleEndTurn_FinishBattle;
}

void HandleEndTurn_BattleLost(void)
{
    gCurrentActionFuncId = 0;

    if (gBattleTypeFlags & BATTLE_TYPE_LINK)
    {
        if (gBattleTypeFlags & BATTLE_TYPE_FRONTIER)
        {
            if (gBattleOutcome & B_OUTCOME_LINK_BATTLE_RAN)
            {
                gBattlescriptCurrInstr = BattleScript_PrintPlayerForfeitedLinkBattle;
                gBattleOutcome &= ~(B_OUTCOME_LINK_BATTLE_RAN);
            }
            else
            {
                gBattlescriptCurrInstr = BattleScript_LostMultiBattleTower;
                gBattleOutcome &= ~(B_OUTCOME_LINK_BATTLE_RAN);
            }
        }
        else
        {
            gBattleTextBuff1[0] = gBattleOutcome;
            gBankAttacker = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);
            gBattlescriptCurrInstr = BattleScript_LinkBattleWonOrLost;
            gBattleOutcome &= ~(B_OUTCOME_LINK_BATTLE_RAN);
        }
    }
	else if (gBattleTypeFlags & BATTLE_TYPE_FRONTIER)
	{
		if (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS)
			gBattlescriptCurrInstr = BattleScript_LostMultiBattleTower;
		else
			gBattlescriptCurrInstr = BattleScript_LostBattleTower;
			
        gBattleOutcome &= ~(B_OUTCOME_RAN);
	}
    else if (gBattleTypeFlags & BATTLE_TYPE_TRAINER && sTrainerBattleMode == TRAINER_BATTLE_OAK_TUTORIAL)
	{
		if (sTrainerBattleOakTutorialHelper & 1)
			gBattleCommunication[MULTISTRING_CHOOSER] = 1;
		else
			gBattleCommunication[MULTISTRING_CHOOSER] = 2;
		
		gBankAttacker = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);
		gBattlescriptCurrInstr = BattleScript_LocalBattleLost;
	}	
    else 
	{
		gBattleCommunication[MULTISTRING_CHOOSER] = 0;
        gBattlescriptCurrInstr = BattleScript_LocalBattleLost;
    }

    gBattleMainFunc = (u32) HandleEndTurn_FinishBattle;
}

void HandleEndTurn_RanFromBattle(void)
{
    gCurrentActionFuncId = 0;

    if (gBattleTypeFlags & BATTLE_TYPE_FRONTIER && gBattleTypeFlags & BATTLE_TYPE_TRAINER)
    {
        gBattlescriptCurrInstr = BattleScript_PrintPlayerForfeited;
        gBattleOutcome = B_OUTCOME_FORFEITED;
    }
    else
    {
        switch (gProtectStructs[gBankAttacker].fleeFlag) {
			case 1:
				gBattlescriptCurrInstr = BattleScript_SmokeBallEscape; //0x81D8901
				break;
			case 2:
				gBattlescriptCurrInstr = BattleScript_RanAwayUsingMonAbility; //0x81D890F
				break;
			default:
				gBattlescriptCurrInstr = BattleScript_GotAwaySafely; //0x81D8916
				break;
        }
    }

    gBattleMainFunc = (u32) HandleEndTurn_FinishBattle;
}

void EndOfBattleThings(void) {
	RestoreNonConsumableItems();
	FormsRevert(gPlayerParty);
	MegaRevert();
	UpdateBurmy();
	EndPartnerBattlePartyRestore();
	EndSkyBattlePartyRestore();
	RecalcAllStats();
	BringBackTheDead();
	EndBattleFlagClear();
	TerrainType = 0; //Reset now b/c normal reset is after BG is loaded
}

void RestoreNonConsumableItems(void) {
	u16* items = ExtensionState.itemBackup;
	
	if (ExtensionState.itemBackup != NULL) {
		if (gBattleTypeFlags & BATTLE_TYPE_TRAINER) {
			for (int i = 0; i < PARTY_SIZE; ++i) 
			{
				if (gBattleTypeFlags & BATTLE_TYPE_FRONTIER
				||  items[i] == 0
				||  !IsConsumable(items[i]))
					gPlayerParty[i].item = items[i];
				
				else if (gPlayerParty[i].item != items[i] //The player consumed their item, and then picked up another one
				&& IsConsumable(items[i]))
					gPlayerParty[i].item = 0; //They don't get to keep the new item
			}
		}
		Free(items);
	}
}

void RecalcAllStats(void) {
	for (int i = 0; i < PARTY_SIZE; ++i) 
		CalculateMonStats(&gPlayerParty[i]);
}

void BringBackTheDead(void) { //Used after Multi Battles that you lost, but your partner won
	if (ViableMonCount(gPlayerParty) == 0) {
		for (int i = 0; i < PARTY_SIZE; ++i) {
			if (gPlayerParty[i].species != 0 && !GetMonData(&gPlayerParty[i], MON_DATA_IS_EGG, 0)) {
				gPlayerParty[i].hp = 1;
				break;
			}
		}
	}
}

void EndPartnerBattlePartyRestore(void) {
	int i;
	u8 counter = 0;
	pokemon_t* backup = ExtensionState.partyBackup;
	
	if (ExtensionState.partyBackup != NULL) {
		if (gSelectedOrderFromParty[0] == 0) { //Special 0x2F was not used
			for (i = 0; i < PARTY_SIZE; ++i) {
				if (gPlayerParty[i].species == 0)
					Memcpy(&gPlayerParty[i], &backup[counter++], sizeof(struct Pokemon));
			}
		}
		
		else { //Special 0x2F was used
			pokemon_t* foughtMons = Calloc(sizeof(struct Pokemon) * 3);
			if (foughtMons != NULL) {
				Memcpy(foughtMons, gPlayerParty, sizeof(struct Pokemon) * 3);
				Memset(gPlayerParty, 0x0, sizeof(struct Pokemon) * 6);
				for (i = 0; i < 3; ++i) {
					if (gSelectedOrderFromParty[i] != 0)
						Memcpy(&gPlayerParty[gSelectedOrderFromParty[i] - 1], &foughtMons[i], sizeof(struct Pokemon));
				}
			}
			for (i = 0; i < PARTY_SIZE; ++i) {
				if (gPlayerParty[i].species == 0)
					Memcpy(&gPlayerParty[i], &backup[counter++], sizeof(struct Pokemon));
			}
		}	
		Free(backup);
	}
}


//TO DO, restore party order like above
void EndSkyBattlePartyRestore(void) {
	int i;
	u8 counter = 0;
	pokemon_t* backup = ExtensionState.skyBattlePartyBackup;
	
	if (ExtensionState.skyBattlePartyBackup != NULL) {
		for (i = 0; i < PARTY_SIZE; ++i) {
			if (gPlayerParty[i].species == 0)
				Memcpy(&gPlayerParty[i], &backup[counter++], sizeof(struct Pokemon));
		}
		Free(backup);
	}
}

void EndBattleFlagClear(void) {
	FlagClear(NO_CATCHING_FLAG);
	FlagClear(NO_RUNNING_FLAG);
	FlagClear(NO_CATCHING_AND_RUNNING_FLAG);
	FlagClear(WILD_CUSTOM_MOVES_FLAG);
	FlagClear(TAG_BATTLE_FLAG);
	FlagClear(TWO_OPPONENT_FLAG);
	FlagClear(SMART_WILD_FLAG);
	VarSet(BATTLE_TOWER_TRAINER_NAME, 0xFFFF);
	Memset(&ExtensionState, 0x0, sizeof(struct BattleExtensionState));
	Free(gNewBS->MegaData);
	Free(gNewBS->UltraData);
	Free(gNewBS->ZMoveData);
	Free(gNewBS);
}

bool8 IsConsumable(u16 item) {
	u8 effect = gItems[SanitizeItemId(item)].holdEffect;
	
	for (u32 i = 0; ConsumableItemEffectTable[i] != 0xFF; ++i) {
		if (effect == ConsumableItemEffectTable[i])
			return TRUE;
	}
	
	return FALSE;
}