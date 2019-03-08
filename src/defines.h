#pragma once

#include "config.h"

#include "../include/global.h"
#include "../include/battle.h"
#include "../include/battle_message.h"
#include "../include/battle_anim.h"
#include "../include/pokedex.h"
#include "../include/party_menu.h"
#include "../include/constants/abilities.h"
#include "../include/constants/moves.h"
#include "../include/constants/items.h"
#include "../include/constants/hold_effects.h"
#include "../include/constants/battle_move_effects.h"
#include "../include/constants/flags.h"
#include "../include/constants/map_types.h"
#include "../include/constants/flags.h"
#include "../include/constants/pokemon.h"
#include "../include/constants/trainers.h"
#include "../include/constants/metatile_behaviors.h"
#include "../include/pokemon.h"
#include "../include/battle_controllers.h"
#include "../include/item.h"
#include "../include/battle_string_ids.h"
#include "../include/string_util.h"
#include "../include/link.h"
#include "../include/text.h"
#include "../include/task.h"
#include "../include/sprite.h"
#include "../include/battle_interface.h"
#include "../include/data2.h"
#include "../include/global.fieldmap.h"
#include "../include/battle_setup.h"
#include "../include/songs.h"
#include "../include/gba/m4a_internal.h"
#include "../include/window.h"
#include "../include/field_weather.h"

#include "pokemon_data_request.h"
#include "pokemon.h"
#include "ram_locs.h"
#include "rom_locs.h"
#include "Vanilla_functions.h"

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

#define ABILITY_PRESENT(ability) AbilityBattleEffects(ABILITYEFFECT_CHECK_ON_FIELD, 0, ability, 0, 0)
#define NO_MOLD_BREAKERS(ability) (ability != ABILITY_MOLDBREAKER && ability != ABILITY_TERAVOLT && ability != ABILITY_TURBOBLAZE)
#define IS_BLANK_TYPE(type) (type == TYPE_MYSTERY || type == TYPE_ROOSTLESS || type == TYPE_BLANK)
#define IS_TRANSFORMED(bank) (gBattleMons[bank].status2 & STATUS2_TRANSFORMED)
#define TOOK_DAMAGE(bank) (gSpecialStatuses[bank].moveturnLostHP_physical || gSpecialStatuses[bank].moveturnLostHP_special)
#define MOVE_HAD_EFFECT (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT))
#define PINCH_BERRY_CHECK(bank) (gBattleMons[bank].hp <= gBattleMons[bank].maxHP / 4 || (ABILITY(bank) == ABILITY_GLUTTONY && gBattleMons[bank].hp <= gBattleMons[bank].maxHP / 2))
#define STAT_CAN_RISE(bank, stat) ((gBattleMons[bank].statStages[stat-1] < 12 && ABILITY(bank) != ABILITY_CONTRARY) || (ABILITY(bank) == ABILITY_CONTRARY && gBattleMons[bank].statStages[stat-1] > 0))
#define STAT_CAN_FALL(bank, stat) ((gBattleMons[bank].statStages[stat-1] > 0 && ABILITY(bank) != ABILITY_CONTRARY) || (ABILITY(bank) == ABILITY_CONTRARY && gBattleMons[bank].statStages[stat-1] < 12))
#define BATTLER_MAX_HP(bank) (gBattleMons[bank].hp == gBattleMons[bank].maxHP)
#define IS_Z_MOVE(move) (move >= FIRST_Z_MOVE && move <= LAST_Z_MOVE)
#define IS_EVEN(val) (umodsi(val, 2) == 0)
#define IS_ODD(val) (umodsi(val, 2) != 0)

#define AI_THINKING_STRUCT ((struct AI_ThinkingStruct*)(gBattleResources->ai))
#define BATTLE_HISTORY ((struct BattleHistory*)(gBattleResources->battleHistory))

#define uBYTE0_16(value)(( (u8) (((u16)(value) & (0x000000FF)) >> 0x00)))
#define uBYTE1_16(value)(( (u8) (((u16)(value) & (0x0000FF00)) >> 0x08)))

#define EOS 0xFF

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
