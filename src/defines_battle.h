#pragma once

#include "config.h"

#include "../include/battle.h"
#include "../include/battle_controllers.h"
#include "../include/battle_interface.h"
#include "../include/battle_message.h"
#include "../include/battle_setup.h"
#include "../include/data2.h"
#include "../include/pokemon.h"
#include "../include/pokemon_data_request.h"

#include "../include/constants/pokemon.h"
#include "../include/constants/species.h"
#include "../include/constants/abilities.h"
#include "../include/constants/moves.h"
#include "../include/constants/hold_effects.h"
#include "../include/constants/battle_move_effects.h"

#include "../include/new/ram_locs_battle.h"
#include "../include/new/Vanilla_functions_battle.h"

#define SIDE(bank) GetBattlerSide(bank)
#define PARTNER(bank) (bank ^ BIT_FLANK)
#define FOE(bank) ((bank ^ BIT_SIDE) & BIT_SIDE)
#define ABILITY(bank) BanksAbility(bank)
#define SPECIES(bank) gBattleMons[bank].species
#define ITEM(bank) gBattleMons[bank].item
#define ITEM_EFFECT(bank) GetBankItemEffect(bank)
#define ITEM_QUALITY(bank) ItemId_GetHoldEffectParam(gBattleMons[bank].item)
#define ITEM_POCKET(bank) GetPocketByItemId(gBattleMons[bank].item)
#define SPLIT(move) gBattleMoves[move].split
#define CONSUMED_ITEMS(bank) gBattleStruct->usedHeldItems[bank]
#define SAVED_CONSUMED_ITEMS(bank) gNewBS->SavedConsumedItems[gBattlerPartyIndexes[bank]]
#define CHOICED_MOVE(bank) gBattleStruct->choicedMove[bank]
#define SECOND_OPPONENT (VarGet(SECOND_OPPONENT_VAR))
#define SPECIES_TABLES_TERMIN 0xFEFE
#define MOVE_TABLES_TERMIN 0xFEFE
#define ITEM_TABLES_TERMIN 0xFEFE
#define ABILITY_TABLES_TERMIN 0xFF
#define MOVE_EFFECT_TABLES_TERMIN 0xFF

#define ABILITY_PRESENT(ability) AbilityBattleEffects(ABILITYEFFECT_CHECK_ON_FIELD, 0, ability, 0, 0)
#define NO_MOLD_BREAKERS(ability) (ability != ABILITY_MOLDBREAKER && ability != ABILITY_TERAVOLT && ability != ABILITY_TURBOBLAZE)
#define IS_BLANK_TYPE(type) (type == TYPE_MYSTERY || type == TYPE_ROOSTLESS || type == TYPE_BLANK)
#define IS_TRANSFORMED(bank) (gBattleMons[bank].status2 & STATUS2_TRANSFORMED)
#define TOOK_DAMAGE(bank) (gSpecialStatuses[bank].moveturnLostHP_physical || gSpecialStatuses[bank].moveturnLostHP_special)
#define MOVE_HAD_EFFECT (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT))
#define PINCH_BERRY_CHECK(bank) (gBattleMons[bank].hp <= gBattleMons[bank].maxHP / 4 || (ABILITY(bank) == ABILITY_GLUTTONY && gBattleMons[bank].hp <= gBattleMons[bank].maxHP / 2))
#define STAT_CAN_RISE(bank, stat) ((gBattleMons[bank].statStages[stat-1] < 12 && ABILITY(bank) != ABILITY_CONTRARY) || (ABILITY(bank) == ABILITY_CONTRARY && gBattleMons[bank].statStages[stat-1] > 0))
#define STAT_CAN_FALL(bank, stat) ((gBattleMons[bank].statStages[stat-1] > 0 && ABILITY(bank) != ABILITY_CONTRARY) || (ABILITY(bank) == ABILITY_CONTRARY && gBattleMons[bank].statStages[stat-1] < 12))
#define BATTLER_ALIVE(bank) (gBattleMons[bank].hp)
#define BATTLER_MAX_HP(bank) (gBattleMons[bank].hp == gBattleMons[bank].maxHP)
#define BATTLER_SEMI_INVULERABLE(bank) (gStatuses3[bank] & STATUS3_SEMI_INVULNERABLE)
#define IS_Z_MOVE(move) (move >= FIRST_Z_MOVE && move <= LAST_Z_MOVE)

#define STAT_STAGE(bank, stat) (gBattleMons[bank].statStages[stat - 1])

#define AI_THINKING_STRUCT ((struct AI_ThinkingStruct*)(gBattleResources->ai))
#define BATTLE_HISTORY ((struct BattleHistory*)(gBattleResources->battleHistory))

#define RELOAD_BATTLE_STATS(bank, partydata)				\
{															\
	if (gStatuses3[bank] & STATUS3_POWER_TRICK) {			\
		gBattleMons[bank].defense = partydata->attack;		\
		gBattleMons[bank].attack = partydata->defense;		\
	}														\
	else {													\
		gBattleMons[bank].attack = partydata->attack;		\
		gBattleMons[bank].defense = partydata->defense;		\
	}														\
	gBattleMons[bank].speed = partydata->speed;				\
	gBattleMons[bank].spAttack = partydata->spAttack;		\
	gBattleMons[bank].spDefense = partydata->spDefense;		\
	gBattleMons[bank].hp = partydata->hp;					\
	gBattleMons[bank].maxHP = partydata->maxHP;				\
}

#define SET_BATTLER_TYPE(battlerId, type)           \
{                                                   \
    gBattleMons[battlerId].type1 = type;            \
    gBattleMons[battlerId].type2 = type;            \
    gBattleMons[battlerId].type3 = TYPE_BLANK;    	\
}

#define APPLY_STAT_MOD(var, mon, stat, statIndex)                                   	\
{                                                                                   	\
    (var) = (stat) * (gStatStageRatios)[(mon)->statStages[(statIndex)-1]][0];         	\
    (var) = udivsi((var), (gStatStageRatios)[(mon)->statStages[(statIndex)-1]][1]);     \
}

enum
{
	TRAINER_SLIDE_LAST_SWITCHIN,
	TRAINER_SLIDE_LAST_LOW_HP,
	TRAINER_SLIDE_FIRST_DOWN,
};

extern struct BattleMove gBattleMoves[];
extern const u8 gTypeEffectiveness[124 * 3];
#define BattleScript_MoveEnd (u8*) 0x81D694E
#define BattleScript_Atk49 (u8*) 0x81D6954
extern u8 BattleScript_ButItFailed[];
#define gBattleTerrainTable ((struct BattleBackground*) *((u32*) 0x800F320))

typedef u8 TrainerClassNames_t[13];
#define gTrainerClassNames ((TrainerClassNames_t*) *((u32*) 0x811B4B4)) //0x823E558
#define gTrainers ((struct Trainer*) *((u32*) 0x800FC00)) //0x823EAC8
#define gTrainerFrontPicCoords ((struct TrainerPicCoords*) 0x823932C)
#define gTrainerFrontPicTable ((struct CompressedSpriteSheet*) 0x823957C)
#define gTrainerFrontPicPaletteTable ((struct CompressedSpritePalette*) 0x8239A1C)
extern struct TrainerPicCoords gTrainerBackPicCoords[];
#define gTrainerMoneyTable ((struct TrainerMoney*) 0x824F220)

#define sATypeMove_Table ((u8**) 0x83FEA28) //Table of things like "A Normal Move!"
#define gBattleStringsTable ((u8**) 0x83FDF3C)

#define gStatusConditionString_Poison ((u8*) 0x8250094)
#define gStatusConditionString_Sleep ((u8*) 0x825009C)
#define gStatusConditionString_Paralysis ((u8*) 0x82500A4)
#define gStatusConditionString_Burn ((u8*) 0x82500AC)
#define gStatusConditionString_Ice ((u8*) 0x82500B4)
#define gStatusConditionString_Confusion ((u8*) 0x82500BC)
#define gStatusConditionString_Love ((u8*) 0x82500C4)
