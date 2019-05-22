#include "defines.h"
#include "defines_battle.h"
#include "../include/event_data.h"
#include "../include/random.h"
#include "../include/constants/songs.h"
#include "../include/constants/trainer_classes.h"

#include "../include/new/end_battle.h"
#include "../include/new/end_battle_battle_scripts.h"
#include "../include/new/form_change.h"
#include "../include/new/helper_functions.h"
#include "../include/new/mega.h"
#include "../include/new/multi.h"

const u16 gEndBattleFlagClearTable[] =
{
#ifdef CATCH_TRAINERS_POKEMON_FLAG
	CATCH_TRAINERS_POKEMON_FLAG,
#endif
#ifdef ACTIVATE_TUTORIAL_FLAG
	ACTIVATE_TUTORIAL_FLAG,
#endif
	INVERSE_FLAG,
	SKY_BATTLE_FLAG,
	NO_CATCHING_FLAG,
	NO_RUNNING_FLAG,
	NO_CATCHING_AND_RUNNING_FLAG,
	TAG_BATTLE_FLAG,
	TWO_OPPONENT_FLAG,
	WILD_CUSTOM_MOVES_FLAG,
	SMART_WILD_FLAG,
	HIDDEN_ABILITY_FLAG,
	DOUBLE_WILD_BATTLE_FLAG,
	SHINY_CREATION_FLAG,
};

//This file's functions:
static void NaturalCureHeal(void);
static void RestoreNonConsumableItems(void);
static void RecalcAllStats(void);
static void BringBackTheDead(void);
static void EndPartnerBattlePartyRestore(void);
static void EndSkyBattlePartyRestore(void);
static void EndBattleFlagClear(void);

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
			case CLASS_LEADER:
			case CLASS_ELITE_4:
			case CLASS_CHAMPION:
				PlayBGM(BGM_VICTORY_SPECIAL);
				specialMus = TRUE;
				break;
			default:
				PlayBGM(BGM_VICTORY_TRAINER_BATTLE);
				break;

		#else //For Pokemon Unbound
			case CLASS_CHAMPION:
				PlayBGM(BGM_VICTORY_CHAMPION);
				specialMus = TRUE;
				break;
			case CLASS_ELITE_4:
				PlayBGM(BGM_VICTORY_ELITE_4);
				specialMus = TRUE;
				break;
			case CLASS_LEADER:
			case CLASS_FRONTIER_BRAIN:
				PlayBGM(BGM_VICTORY_GYM);
				specialMus = TRUE;
				break;
			case CLASS_SHADOW_ADMIN: //0x30
			case CLASS_BOSS: //0x53
			case CLASS_SHADOW: //0x55
			#ifndef DEBUG_UNBOUND_MUSIC
				PlayBGM(BGM_VICTORY_PLASMA);
				specialMus = TRUE;
				break;
			#endif
			case CLASS_LOR_LEADER: //0x2
			case CLASS_LOR_ADMIN: //0x2E
			case CLASS_LOR: //0x2F
				PlayBGM(BGM_VICTORY_GALACTIC);
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

	gSpecialVar_LastResult = 0;
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

	gSpecialVar_LastResult = 1;
    gBattleMainFunc = (u32) HandleEndTurn_FinishBattle;
}

void HandleEndTurn_RanFromBattle(void)
{
    gCurrentActionFuncId = 0;

    if (gBattleTypeFlags & BATTLE_TYPE_FRONTIER && gBattleTypeFlags & BATTLE_TYPE_TRAINER)
    {
        gBattlescriptCurrInstr = BattleScript_PrintPlayerForfeited;
		gBattleOutcome = B_OUTCOME_LOST;
    }
    else
    {
        switch (gProtectStructs[gBankAttacker].fleeFlag) {
			case 1:
				gBattlescriptCurrInstr = BattleScript_SmokeBallEscape; //0x81D8901
				break;
			case 2:
				gBattleScripting->bank = gBankAttacker;
				gBattlescriptCurrInstr = BattleScript_RanAwayUsingMonAbility; //0x81D890F
				break;
			default:
				gBattlescriptCurrInstr = BattleScript_GotAwaySafely; //0x81D8916
				break;
        }
    }

    gBattleMainFunc = (u32) HandleEndTurn_FinishBattle;
}

#define STATE_BEFORE_ACTION_CHOSEN 0

bool8 HandleRunActionFrontier(void)
{
    if (gBattleTypeFlags & BATTLE_TYPE_TRAINER
    && gBattleTypeFlags & BATTLE_TYPE_FRONTIER
    && gBattleBufferB[gActiveBattler][1] == ACTION_RUN)
    {
		BattleScriptExecute(BattleScript_AskIfWantsToForfeitMatch);
		gBattleCommunication[gActiveBattler] = STATE_BEFORE_ACTION_CHOSEN;
        return TRUE;
    }

	return FALSE;
}

#define ABILITY_PREVENTING_ESCAPE 2

u8 IsRunningFromBattleImpossible(void)
{
    u8 itemEffect;
    u8 side;
    int i;

    itemEffect = ITEM_EFFECT(gActiveBattler);
    gStringBank = gActiveBattler;

    if (gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_FRONTIER))
        return FALSE;

	else if (FlagGet(NO_RUNNING_FLAG) || FlagGet(NO_CATCHING_AND_RUNNING_FLAG))
		return TRUE;

    else if (itemEffect == ITEM_EFFECT_CAN_ALWAYS_RUN)
        return FALSE;
    else if (gBattleMons[gActiveBattler].ability == ABILITY_RUNAWAY)
        return FALSE;
	else if (IsOfType(gActiveBattler, TYPE_GHOST))
		return FALSE;

    side = SIDE(gActiveBattler);

    for (i = 0; i < gBattlersCount; i++)
    {
        if (ABILITY(gActiveBattler) != ABILITY_SHADOWTAG //Shadow Tag's not affected by Shadow Tag
		&& side != SIDE(i)
        && ABILITY(i) == ABILITY_SHADOWTAG)
        {
            gBattleScripting->bank = i;
            gLastUsedAbility = ABILITY(i);
            gBattleCommunication[MULTISTRING_CHOOSER] = 2;
            return ABILITY_PREVENTING_ESCAPE;
        }
        if (side != SIDE(i)
        && ABILITY(i) == ABILITY_ARENATRAP
		&& !CheckGrounding(gActiveBattler))
        {
            gBattleScripting->bank = i;
            gLastUsedAbility = ABILITY(i);
            gBattleCommunication[MULTISTRING_CHOOSER] = 2;
            return ABILITY_PREVENTING_ESCAPE;
        }

		if (i != gActiveBattler
		&& ABILITY(i) == ABILITY_MAGNETPULL
		&& IsOfType(gActiveBattler, TYPE_STEEL))
		{
			gBattleScripting->bank = i;
			gLastUsedAbility = ABILITY(i);
			gBattleCommunication[MULTISTRING_CHOOSER] = 2;
			return ABILITY_PREVENTING_ESCAPE;
		}
    }

    if ((gBattleMons[gActiveBattler].status2 & (STATUS2_ESCAPE_PREVENTION | STATUS2_WRAPPED))
    || (gStatuses3[gActiveBattler] & STATUS3_SKY_DROP_TARGET))
    {
        gBattleCommunication[MULTISTRING_CHOOSER] = 0;
        return TRUE;
    }

	if (!gNewBS->TeleportBit)
	{
		if (gStatuses3[gActiveBattler] & STATUS3_ROOTED)
		{
			gBattleCommunication[MULTISTRING_CHOOSER] = 0;
			return 1;
		}
	}
	else
		gNewBS->TeleportBit = FALSE;

	if (gNewBS->FairyLockTimer)
	{
		gBattleCommunication[MULTISTRING_CHOOSER] = 0;
		return TRUE;
	}

    if (gBattleTypeFlags & BATTLE_TYPE_OAK_TUTORIAL)
    {
        gBattleCommunication[MULTISTRING_CHOOSER] = 1;
        return TRUE;
    }

    return 0;
}

bool8 TryRunFromBattle(u8 bank)
{
    bool8 effect = FALSE;
    u8 itemEffect;
    u8 speedVar;

    itemEffect = ITEM_EFFECT(bank);
    gStringBank = bank;

	if (IsOfType(bank, TYPE_GHOST))
	{
		++effect;
	}
    else if (itemEffect == ITEM_EFFECT_CAN_ALWAYS_RUN)
    {
        gLastUsedItem = ITEM(bank);
        gProtectStructs[bank].fleeFlag = 1;
        ++effect;
    }
    else if (ABILITY(bank) == ABILITY_RUNAWAY)
    {
        gLastUsedAbility = ABILITY_RUNAWAY;
        gProtectStructs[bank].fleeFlag = 2;
        ++effect;
    }
	#ifndef NO_GHOST_BATTLES
	else if ((gBattleTypeFlags & (BATTLE_TYPE_SCRIPTED_WILD_1 | BATTLE_TYPE_GHOST)) == BATTLE_TYPE_GHOST)
	{
		if (SIDE(bank) == B_SIDE_PLAYER)
			++effect;
	}
	#endif
    else if (gBattleTypeFlags & (BATTLE_TYPE_FRONTIER | BATTLE_TYPE_TRAINER_TOWER) && gBattleTypeFlags & BATTLE_TYPE_TRAINER)
    {
        ++effect;
    }
    else
    {
        if (!(gBattleTypeFlags & BATTLE_TYPE_DOUBLE))
        {
		SINGLE_FLEE_CALC:
            if (gBattleMons[bank].speed < gBattleMons[BATTLE_OPPOSITE(bank)].speed)
            {
                speedVar = udivsi((gBattleMons[bank].speed * 128), (gBattleMons[BATTLE_OPPOSITE(bank)].speed)) + (gBattleStruct->runTries * 30);
                if (speedVar > (Random() & 0xFF))
                    ++effect;
            }
            else // same speed or faster
            {
                ++effect;
            }
        }
		else //Wild Double
		{
			if (gBattleMons[BATTLE_OPPOSITE(bank)].hp)
				goto SINGLE_FLEE_CALC;

            if (gBattleMons[bank].speed < gBattleMons[PARTNER(BATTLE_OPPOSITE(bank))].speed)
            {
                speedVar = udivsi((gBattleMons[bank].speed * 128), (gBattleMons[PARTNER(BATTLE_OPPOSITE(bank))].speed)) + (gBattleStruct->runTries * 30);
                if (speedVar > (Random() & 0xFF))
                    ++effect;
            }
            else // same speed or faster
            {
                ++effect;
            }
		}

        gBattleStruct->runTries++;
    }

    if (effect)
    {
        gCurrentTurnActionNumber = gBattlersCount;
        gBattleOutcome = B_OUTCOME_RAN;
    }

    return effect;
}

void EndOfBattleThings(void)
{
	NaturalCureHeal();
	RestoreNonConsumableItems();
	FormsRevert(gPlayerParty);
	MegaRevert(gPlayerParty);
	UpdateBurmy();
	EndPartnerBattlePartyRestore();
	EndSkyBattlePartyRestore();
	RecalcAllStats();
	BringBackTheDead();
	EndBattleFlagClear();
	TerrainType = 0; //Reset now b/c normal reset is after BG is loaded
}

static void NaturalCureHeal(void)
{
	for (int bank = 0; bank < gBattlersCount; ++bank)
	{
		if (ABILITY(bank) == ABILITY_NATURALCURE && gBattleMons[bank].status1)
			GetBankPartyData(bank)->condition = 0;
	}
}

static void RestoreNonConsumableItems(void)
{
	u16* items = ExtensionState.itemBackup;

	if (ExtensionState.itemBackup != NULL)
	{
		if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)
		{
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

static void RecalcAllStats(void)
{
	for (int i = 0; i < PARTY_SIZE; ++i)
		CalculateMonStats(&gPlayerParty[i]);
}

static void BringBackTheDead(void)
{ //Used after Multi Battles that you lost, but your partner won
	if (ViableMonCount(gPlayerParty) == 0)
	{
		for (int i = 0; i < PARTY_SIZE; ++i)
		{
			if (gPlayerParty[i].species != 0 && !GetMonData(&gPlayerParty[i], MON_DATA_IS_EGG, 0))
			{
				gPlayerParty[i].hp = 1;
				break;
			}
		}
	}
}

static void EndPartnerBattlePartyRestore(void)
{
	int i;
	u8 counter = 0;
	pokemon_t* backup = ExtensionState.partyBackup;

	if (ExtensionState.partyBackup != NULL)
	{
		if (gSelectedOrderFromParty[0] == 0)
		{ 	//Special 0x2F was not used
			Memcpy(&gPlayerParty[3], ExtensionState.partyBackup, sizeof(struct Pokemon) * 3);
		}
		else
		{ 	//Special 0x2F was used
			pokemon_t* foughtMons = Calloc(sizeof(struct Pokemon) * 3);
			if (foughtMons != NULL)
			{
				Memcpy(foughtMons, gPlayerParty, sizeof(struct Pokemon) * 3);
				Memset(gPlayerParty, 0x0, sizeof(struct Pokemon) * 6);
				for (i = 0; i < 3; ++i)
				{
					if (gSelectedOrderFromParty[i] != 0)
						Memcpy(&gPlayerParty[gSelectedOrderFromParty[i] - 1], &foughtMons[i], sizeof(struct Pokemon));
				}
			}

			for (i = 0; i < PARTY_SIZE; ++i)
			{
				if (gPlayerParty[i].species == 0)
					Memcpy(&gPlayerParty[i], &backup[counter++], sizeof(struct Pokemon));
			}
		}
		Free(ExtensionState.partyBackup);
	}
}


//TO DO, restore party order like above
static void EndSkyBattlePartyRestore(void)
{
	int i;
	u8 counter = 0;
	pokemon_t* backup = ExtensionState.skyBattlePartyBackup;

	if (ExtensionState.skyBattlePartyBackup != NULL)
	{
		for (i = 0; i < PARTY_SIZE; ++i) {
			if (gPlayerParty[i].species == 0)
				Memcpy(&gPlayerParty[i], &backup[counter++], sizeof(struct Pokemon));
		}
		Free(backup);
	}
}

static void EndBattleFlagClear(void)
{
	for (u32 i = 0; i < ARRAY_COUNT(gEndBattleFlagClearTable); ++i)
		FlagClear(gEndBattleFlagClearTable[i]);

	u16 inducer = VarGet(STATUS_INDUCER_VAR);
	if (inducer & 0xFF00) //Temporary status inducer
	{
		u8 status = inducer & 0xFF;
		u8 amount = ((inducer & 0xFF00) >> 8) - 1; //Subtract num battles by 1
		
		if (amount == 0) //Time's up
			VarSet(STATUS_INDUCER_VAR, 0);
		else
			VarSet(STATUS_INDUCER_VAR, status | (amount << 8));
	}

	VarSet(TERRAIN_VAR, 0);
	VarSet(BATTLE_TOWER_TRAINER_NAME, 0xFFFF);
	Free(gNewBS->MegaData);
	Free(gNewBS->UltraData);
	Free(gNewBS->ZMoveData);
	Free(gNewBS);
	Memset(&ExtensionState, 0x0, sizeof(struct BattleExtensionState));
}

bool8 IsConsumable(u16 item)
{
	u8 effect = gItems[SanitizeItemId(item)].holdEffect;

	for (u32 i = 0; ConsumableItemEffectTable[i] != 0xFF; ++i) {
		if (effect == ConsumableItemEffectTable[i])
			return TRUE;
	}

	return FALSE;
}
