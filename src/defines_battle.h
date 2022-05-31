#pragma once
/*
defines_battle.h
	general header files for files associated with the battle engine
*/
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
#define ABILITY(bank) gBattleMons[bank].ability
#define SPECIES(bank) gBattleMons[bank].species
#define ITEM(bank) gBattleMons[bank].item
#define ITEM_EFFECT(bank) GetBankItemEffect(bank)
#define ITEM_QUALITY(bank) ItemId_GetHoldEffectParam(gBattleMons[bank].item)
#define ITEM_POCKET(bank) GetPocketByItemId(gBattleMons[bank].item)
#define SPLIT(move) gBattleMoves[move].split
#define CONSUMED_ITEMS(bank) gBattleStruct->usedHeldItems[bank]
#define SAVED_CONSUMED_ITEMS(bank) gNewBS->SavedConsumedItems[gBattlerPartyIndexes[bank]]
#define CHOICED_MOVE(bank) gBattleStruct->choicedMove[bank]
#define SECOND_OPPONENT (VarGet(VAR_SECOND_OPPONENT))

#define REALLY_SMART_AI //The vanilla FR AI memory system sucks so this should always be defined

#define IS_MOLD_BREAKER(ability, move) (IsMoldBreakerAbility(ability) || gSpecialMoveFlags[move].gMoldBreakerMoves)
#define NO_MOLD_BREAKERS(ability, move) (!IsMoldBreakerAbility(ability) && !gSpecialMoveFlags[move].gMoldBreakerMoves)
#define IS_BLANK_TYPE(type) (type == TYPE_MYSTERY || type == TYPE_ROOSTLESS || type == TYPE_BLANK)
#define IS_TRANSFORMED(bank) (gBattleMons[bank].status2 & STATUS2_TRANSFORMED)
#define IS_BEHIND_SUBSTITUTE(bank) (gBattleMons[bank].status2 & STATUS2_SUBSTITUTE)
#define TOOK_DAMAGE(bank) (gSpecialStatuses[bank].moveturnLostHP_physical || gSpecialStatuses[bank].moveturnLostHP_special || (gNewBS->enduredDamage & gBitTable[bank]))
#define MOVE_HAD_EFFECT (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT))
#define PINCH_BERRY_CHECK(bank) (gBattleMons[bank].hp <= gBattleMons[bank].maxHP / 4 || (ABILITY(bank) == ABILITY_GLUTTONY && gBattleMons[bank].hp <= gBattleMons[bank].maxHP / 2))
#define STAT_CAN_RISE(bank, stat) ((STAT_STAGE(bank, stat) < STAT_STAGE_MAX && ABILITY(bank) != ABILITY_CONTRARY) || (ABILITY(bank) == ABILITY_CONTRARY && STAT_STAGE(bank, stat) > 0))
#define STAT_CAN_FALL(bank, stat) ((STAT_STAGE(bank, stat) > 0 && ABILITY(bank) != ABILITY_CONTRARY) || (ABILITY(bank) == ABILITY_CONTRARY && STAT_STAGE(bank, stat) < STAT_STAGE_MAX))
#define AI_STAT_CAN_RISE(bank, stat) (STAT_STAGE(bank, stat) < STAT_STAGE_MAX)
#define AI_STAT_CAN_FALL(bank, stat) (STAT_STAGE(bank, stat) > STAT_STAGE_MIN)
#define BATTLER_ALIVE(bank) (gBattleMons[bank].hp > 0)
#define BATTLER_MAX_HP(bank) (gBattleMons[bank].hp == gBattleMons[bank].maxHP)
#define BATTLER_SEMI_INVULNERABLE(bank) (gStatuses3[bank] & STATUS3_SEMI_INVULNERABLE)
#define IS_Z_MOVE(move) (move >= FIRST_Z_MOVE && move <= LAST_Z_MOVE)
#define IS_DOUBLE_BATTLE (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
#define IS_SINGLE_BATTLE !IS_DOUBLE_BATTLE
#define IS_GHOST_BATTLE ((gBattleTypeFlags & (BATTLE_TYPE_SCRIPTED_WILD_1 | BATTLE_TYPE_GHOST)) == BATTLE_TYPE_GHOST)

#define STAT_STAGE(bank, stat) (gBattleMons[bank].statStages[stat - 1])

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

#define APPLY_QUICK_STAT_MOD(var, buff)                 \
{                                                       \
    (var) = (var) * gStatStageRatios[buff][0];          \
    (var) = udivsi((var), gStatStageRatios[buff][1]);   \
}

enum
{
	TRAINER_SLIDE_LAST_SWITCHIN,
	TRAINER_SLIDE_LAST_LOW_HP,
	TRAINER_SLIDE_FIRST_DOWN,
};

extern const struct BattleMove gBattleMoves[];
extern const u8 gTypeEffectiveness[NUMBER_OF_MON_TYPES][NUMBER_OF_MON_TYPES];
#define BattleScript_MoveEnd (const u8*) 0x81D694E
#define BattleScript_Atk49 (const u8*) 0x81D6954
extern u8 BattleScript_ButItFailed[];
#define BattleScript_NotAffected (const u8*) 0x81D7E04

typedef u8 TrainerClassNames_t[13];
#define gTrainerClassNames ((TrainerClassNames_t*) *((u32*) 0x811B4B4)) //0x823E558
#define gTrainers ((struct Trainer*) *((u32*) 0x800FC00)) //0x823EAC8
#define gTrainerFrontPicCoords ((struct TrainerPicCoords*) 0x823932C)
#define gTrainerFrontPicTable ((struct CompressedSpriteSheet*) 0x823957C)
#define gTrainerFrontPicPaletteTable ((struct CompressedSpritePalette*) 0x8239A1C)
extern struct TrainerPicCoords gTrainerBackPicCoords[];

#define sATypeMove_Table ((u8**) 0x83FEA28) //Table of things like "A Normal Move!"
#define gBattleStringsTable ((u8**) 0x83FDF3C)

#define gStatusConditionString_Poison ((u8*) 0x8250094)
#define gStatusConditionString_Sleep ((u8*) 0x825009C)
#define gStatusConditionString_Paralysis ((u8*) 0x82500A4)
#define gStatusConditionString_Burn ((u8*) 0x82500AC)
#ifdef FROSTBITE
extern const u8 gStatusConditionString_Frostbite[];
#define gStatusConditionString_Ice gStatusConditionString_Frostbite
#else
#define gStatusConditionString_Ice ((u8*) 0x82500B4)
#endif
#define gStatusConditionString_Confusion ((u8*) 0x82500BC)
#define gStatusConditionString_Love ((u8*) 0x82500C4)
