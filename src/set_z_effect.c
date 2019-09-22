#include "defines.h"
#include "defines_battle.h"
#include "../include/constants/items.h"

#include "../include/new/battle_util.h"
#include "../include/new/item.h"
#include "../include/new/mega.h"
#include "../include/new/move_menu.h"
#include "../include/new/set_z_effect.h"
#include "../include/new/stat_buffs.h"
#include "../include/new/z_move_battle_scripts.h"
#include "../include/new/z_move_effects.h"

extern const u8* const gZMoveNames[];

struct SpecialZMove
{
	u16 species;
	u16 item;
	u16 move;
	u16 zmove;
};

static const struct SpecialZMove sSpecialZMoveTable[] = 
{
	{SPECIES_RAICHU_A,				ITEM_ALORAICHIUM_Z, 		MOVE_THUNDERBOLT, 		MOVE_STOKED_SPARKSURFER},
	{SPECIES_DECIDUEYE,				ITEM_DECIDIUM_Z, 			MOVE_SPIRITSHACKLE,		MOVE_SINISTER_ARROW_RAID},
	{SPECIES_EEVEE,					ITEM_EEVIUM_Z, 				MOVE_LASTRESORT,		MOVE_EXTREME_EVOBOOST},
	{SPECIES_INCINEROAR,			ITEM_INCINIUM_Z, 			MOVE_DARKESTLARIAT,		MOVE_MALICIOUS_MOONSAULT},
	{SPECIES_KOMMO_O,				ITEM_KOMMONIUM_Z,			MOVE_CLANGINGSCALES,	MOVE_CLANGOROUS_SOULBLAZE},
	{SPECIES_LUNALA,				ITEM_LUNALIUM_Z,			MOVE_MOONGEISTBEAM,		MOVE_MENACING_MOONRAZE_MAELSTROM},
	{SPECIES_NECROZMA_DAWN_WINGS,	ITEM_LUNALIUM_Z,			MOVE_MOONGEISTBEAM,		MOVE_MENACING_MOONRAZE_MAELSTROM},
	{SPECIES_LYCANROC,				ITEM_LYCANIUM_Z,			MOVE_STONEEDGE,			MOVE_SPLINTERED_STORMSHARDS},
	{SPECIES_LYCANROC_N,			ITEM_LYCANIUM_Z,			MOVE_STONEEDGE,			MOVE_SPLINTERED_STORMSHARDS},
	{SPECIES_LYCANROC_DUSK,			ITEM_LYCANIUM_Z,			MOVE_STONEEDGE,			MOVE_SPLINTERED_STORMSHARDS},
	{SPECIES_MARSHADOW,				ITEM_MARSHADIUM_Z, 			MOVE_SPECTRALTHIEF,		MOVE_SOUL_STEALING_7_STAR_STRIKE},
	{SPECIES_MEW,					ITEM_MEWNIUM_Z, 			MOVE_PSYCHIC,			MOVE_GENESIS_SUPERNOVA},
	{SPECIES_MIMIKYU,				ITEM_MIMIKIUM_Z, 			MOVE_PLAYROUGH,			MOVE_LETS_SNUGGLE_FOREVER},
	{SPECIES_MIMIKYU_BUSTED,		ITEM_MIMIKIUM_Z, 			MOVE_PLAYROUGH,			MOVE_LETS_SNUGGLE_FOREVER},
	{SPECIES_PIKACHU,				ITEM_PIKANIUM_Z, 			MOVE_VOLTTACKLE,		MOVE_CATASTROPIKA},
	{SPECIES_PIKACHU_CAP_ORIGINAL,	ITEM_PIKASHUNIUM_Z, 		MOVE_THUNDERBOLT,		MOVE_10000000_VOLT_THUNDERBOLT},
	{SPECIES_PIKACHU_CAP_HOENN,		ITEM_PIKASHUNIUM_Z, 		MOVE_THUNDERBOLT,		MOVE_10000000_VOLT_THUNDERBOLT},
	{SPECIES_PIKACHU_CAP_SINNOH,	ITEM_PIKASHUNIUM_Z, 		MOVE_THUNDERBOLT,		MOVE_10000000_VOLT_THUNDERBOLT},
	{SPECIES_PIKACHU_CAP_UNOVA,		ITEM_PIKASHUNIUM_Z, 		MOVE_THUNDERBOLT,		MOVE_10000000_VOLT_THUNDERBOLT},
	{SPECIES_PIKACHU_CAP_KALOS,		ITEM_PIKASHUNIUM_Z, 		MOVE_THUNDERBOLT,		MOVE_10000000_VOLT_THUNDERBOLT},
	{SPECIES_PIKACHU_CAP_ALOLA,		ITEM_PIKASHUNIUM_Z, 		MOVE_THUNDERBOLT,		MOVE_10000000_VOLT_THUNDERBOLT},
	{SPECIES_PIKACHU_CAP_PARTNER,	ITEM_PIKASHUNIUM_Z, 		MOVE_THUNDERBOLT,		MOVE_10000000_VOLT_THUNDERBOLT},
	{SPECIES_PRIMARINA,				ITEM_PRIMARIUM_Z, 			MOVE_SPARKLINGARIA,		MOVE_OCEANIC_OPERETTA},
	{SPECIES_SNORLAX,				ITEM_SNORLIUM_Z, 			MOVE_GIGAIMPACT,		MOVE_PULVERIZING_PANCAKE},
	{SPECIES_SOLGALEO,				ITEM_SOLGANIUM_Z, 			MOVE_SUNSTEELSTRIKE,	MOVE_SEARING_SUNRAZE_SMASH},
	{SPECIES_NECROZMA_DUSK_MANE,	ITEM_SOLGANIUM_Z, 			MOVE_SUNSTEELSTRIKE,	MOVE_SEARING_SUNRAZE_SMASH},
	{SPECIES_TAPU_KOKO,				ITEM_TAPUNIUM_Z, 			MOVE_NATURESMADNESS,	MOVE_GUARDIAN_OF_ALOLA},
	{SPECIES_TAPU_BULU,				ITEM_TAPUNIUM_Z, 			MOVE_NATURESMADNESS,	MOVE_GUARDIAN_OF_ALOLA},
	{SPECIES_TAPU_LELE,				ITEM_TAPUNIUM_Z, 			MOVE_NATURESMADNESS,	MOVE_GUARDIAN_OF_ALOLA},
	{SPECIES_TAPU_FINI,				ITEM_TAPUNIUM_Z, 			MOVE_NATURESMADNESS,	MOVE_GUARDIAN_OF_ALOLA},
	{SPECIES_NECROZMA_ULTRA,		ITEM_ULTRANECROZIUM_Z, 		MOVE_PHOTONGEYSER, 		MOVE_LIGHT_THAT_BURNS_THE_SKY},
};

void SetZEffect(void)
{
	int i;
	
	gNewBS->ZMoveData->runningZEffect = TRUE;

	if (gNewBS->ZMoveData->effect == Z_EFFECT_CURSE) {
		if (IsOfType(gBankAttacker, TYPE_GHOST))
			gNewBS->ZMoveData->effect = Z_EFFECT_RECOVER_HP;
		else
			gNewBS->ZMoveData->effect = Z_EFFECT_ATK_UP_1;
	}
	
	gBattleScripting->bank = gBankAttacker;
	
	switch (gNewBS->ZMoveData->effect) {
		case Z_EFFECT_RESET_STATS:
			for (i = 0; i < BATTLE_STATS_NO-1; ++i) {
				if (gBattleMons[gBankAttacker].statStages[i] < 6) {
					gBattleMons[gBankAttacker].statStages[i] = 6;
				}
			}
			BattleScriptPush(gBattlescriptCurrInstr + 5);
			gBattlescriptCurrInstr = BattleScript_StatsResetZMove - 5;
			break;
		
		case Z_EFFECT_ALL_STATS_UP_1:
			if (!MainStatsMaxed(gBankAttacker)) {
				for (i = 0; i < STAT_ACC-1; ++i) { //Doesn't increase Acc or Evsn
					if (gBattleMons[gBankAttacker].statStages[i] < 12)
					++gBattleMons[gBankAttacker].statStages[i];
				}
				BattleScriptPush(gBattlescriptCurrInstr + 5);
				gBattlescriptCurrInstr = BattleScript_AllStatsUpZMove - 5;
			}
			break;
			
		case Z_EFFECT_BOOST_CRITS:
			if (!(gBattleMons[gBankAttacker].status2 & STATUS2_FOCUS_ENERGY)) {
				gBattleMons[gBankAttacker].status2 |= STATUS2_FOCUS_ENERGY;
				BattleScriptPush(gBattlescriptCurrInstr + 5);
				gBattlescriptCurrInstr = BattleScript_BoostCritsZMove - 5;
			}
			break;
			
		case Z_EFFECT_FOLLOW_ME:
			gSideTimers[SIDE(gBankAttacker)].followmeTimer = 1;
			gSideTimers[SIDE(gBankAttacker)].followmeTarget = gBankAttacker;
			BattleScriptPush(gBattlescriptCurrInstr + 5);
            gBattlescriptCurrInstr = BattleScript_FollowMeZMove - 5;
			break;
			
		case Z_EFFECT_RECOVER_HP:
			if (gBattleMons[gBankAttacker].hp != gBattleMons[gBankAttacker].maxHP) {
				gBattleMoveDamage = (-1) * gBattleMons[gBankAttacker].maxHP;
				BattleScriptPush(gBattlescriptCurrInstr + 5);
				gBattlescriptCurrInstr = BattleScript_RecoverHPZMove - 5;
			}
			break;
			
		case Z_EFFECT_RESTORE_REPLACEMENT_HP:
			gNewBS->ZMoveData->healReplacement = TRUE;
			BattleScriptPush(gBattlescriptCurrInstr + 5);
            gBattlescriptCurrInstr = BattleScript_SetUpHealReplacementZMove - 5;
			break;
		
		case Z_EFFECT_ATK_UP_1:
		case Z_EFFECT_DEF_UP_1:
		case Z_EFFECT_SPD_UP_1:
		case Z_EFFECT_SPATK_UP_1:
		case Z_EFFECT_SPDEF_UP_1:
		case Z_EFFECT_ACC_UP_1:
		case Z_EFFECT_EVSN_UP_1:
			if (!ChangeStatBuffs(SET_STAT_BUFF_VALUE(1), gNewBS->ZMoveData->effect - Z_EFFECT_ATK_UP_1 + 1, MOVE_EFFECT_AFFECTS_USER, 0)) {
				gBattleScripting->animArg1 = 0xE + (gNewBS->ZMoveData->effect - Z_EFFECT_ATK_UP_1 + 1);
				gBattleScripting->animArg2 = 0;
				BattleScriptPush(gBattlescriptCurrInstr + 5);
				gBattlescriptCurrInstr = BattleScript_StatUpZMove - 5;
			}
			break;
			
		case Z_EFFECT_ATK_UP_2:
		case Z_EFFECT_DEF_UP_2:
		case Z_EFFECT_SPD_UP_2:
		case Z_EFFECT_SPATK_UP_2:
		case Z_EFFECT_SPDEF_UP_2:
		case Z_EFFECT_ACC_UP_2:
		case Z_EFFECT_EVSN_UP_2:
			if (!ChangeStatBuffs(SET_STAT_BUFF_VALUE(2), gNewBS->ZMoveData->effect - Z_EFFECT_ATK_UP_2 + 1, MOVE_EFFECT_AFFECTS_USER, 0)) {
				gBattleScripting->animArg1 = 0xE + (gNewBS->ZMoveData->effect - Z_EFFECT_ATK_UP_2 + 1);
				gBattleScripting->animArg2 = 0;
				BattleScriptPush(gBattlescriptCurrInstr + 5);
				gBattlescriptCurrInstr = BattleScript_StatUpZMove - 5;
			}
			break;
			
		case Z_EFFECT_ATK_UP_3:
		case Z_EFFECT_DEF_UP_3:
		case Z_EFFECT_SPD_UP_3:
		case Z_EFFECT_SPATK_UP_3:
		case Z_EFFECT_SPDEF_UP_3:
		case Z_EFFECT_ACC_UP_3:
		case Z_EFFECT_EVSN_UP_3:
			if (!ChangeStatBuffs(SET_STAT_BUFF_VALUE(3), gNewBS->ZMoveData->effect - Z_EFFECT_ATK_UP_3 + 1, MOVE_EFFECT_AFFECTS_USER, 0)) {
				gBattleScripting->animArg1 = 0xE + (gNewBS->ZMoveData->effect - Z_EFFECT_ATK_UP_3 + 1);
				gBattleScripting->animArg2 = 0;
				BattleScriptPush(gBattlescriptCurrInstr + 5);
				gBattlescriptCurrInstr = BattleScript_StatUpZMove - 5;
			}
			break;
	}
	gNewBS->ZMoveData->runningZEffect = FALSE;
}

//Graphics//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern u8 Z_Move_TriggerTiles[];
extern u16 Z_Move_TriggerPal[];

static const struct CompressedSpriteSheet ZTriggerSpriteSheet = {Z_Move_TriggerTiles, (32 * 32) / 2, GFX_TAG_Z_TRIGGER};
static const struct SpritePalette ZTriggerPalette = {Z_Move_TriggerPal, GFX_TAG_Z_TRIGGER};
static const u16 ZTriggerOAM[4] = {0, 0x8000, 0x800, 0};

static void ZTriggerCallback(struct Sprite* self);

static const struct SpriteTemplate ZTriggerTemplate =
{
    .tileTag = GFX_TAG_Z_TRIGGER,
    .paletteTag = GFX_TAG_Z_TRIGGER,
    .oam = (const struct OamData*) &ZTriggerOAM,
    .anims = (const union AnimCmd* const*) 0x08231CF0,
    .images = NULL,
    .affineAnims = (const union AffineAnimCmd* const*) 0x08231CFC,
    .callback = ZTriggerCallback,
};

#define TRIGGER_BANK self->data[4]
#define PALETTE_STATE self->data[1]
static void ZTriggerCallback(struct Sprite* self) 
{		
	s16 xshift, yshift;
	if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE) 
	{
		xshift = -6;
		yshift = 0;
	} 
	else 
	{
		xshift = -5;
		yshift = 2;
	}

	// Find the health box object that this trigger is supposed to be attached to
	u8 id = gHealthboxIDs[TRIGGER_BANK];
	struct Sprite* healthbox = &gSprites[id];
	
	u8 y = healthbox->oam.y;

	u8 pingid = gBattleSpritesDataPtr->healthBoxesData[TRIGGER_BANK].healthboxBounceSpriteId;
	struct Sprite* ping = &gSprites[pingid];
 
	if (y) 
	{
		// Copy the healthbox's position (it has various animations)
		//self->y = healthbox->y + 20;
		self->pos1.x = (healthbox->oam.x) + xshift + self->data[3];
		self->pos2.y = Sine(ping->data[0], ping->data[2]);
		self->pos1.y = healthbox->pos1.y + yshift;
	} 
	else 
	{
		// The box is offscreen, so hide this one as well
		self->pos1.x = -32;
	}
	
	if (gBattleBankFunc[TRIGGER_BANK] == (0x0802EA10 | 1) //Old HandleInputChooseMove
	||  gBattleBankFunc[TRIGGER_BANK] == (u32) HandleInputChooseMove
	|| gBattleBankFunc[TRIGGER_BANK] == (u32) HandleMoveSwitching)
	{
		struct ChooseMoveStruct* moveInfo = (struct ChooseMoveStruct*) (&gBattleBufferA[TRIGGER_BANK][4]);

		if (moveInfo->possibleZMoves[gMoveSelectionCursor[TRIGGER_BANK]]) //Viewing move that can become Z-Move
		{
			if (self->data[3] > 0)
				self->data[3] -= 2;
			else
				self->data[3] = 0;
		}
		else
		{
			if (self->data[3] < 24)
				self->data[3] += 2;
			else 
				self->data[3] = 24;
		}
	}
	
	//Mega Trigger should recede and destroy itself as long as the game isn't
	//running one of the two mentioned functions.
	else if (gBattleBankFunc[TRIGGER_BANK] != (0x08032C90 | 1)  //PlayerHandleChooseMove
	      && gBattleBankFunc[TRIGGER_BANK] != (0x08032C4C | 1)) //HandleChooseMoveAfterDma3
	{
		if (self->data[3] < 24)
			self->data[3] += 2;
		else 
		{
			self->pos1.x = -32;
			DestroyZTrigger();
			return;
		}
	}

  	if (gNewBS->ZMoveData->viewing)
		PALETTE_STATE = MegaTriggerLightUp;
	else
		PALETTE_STATE = MegaTriggerNormalColour;
	
	// Only change the palette if the state has changed
	if (PALETTE_STATE != self->data[2]) 
	{
		u16* pal = &gPlttBufferFaded2[IndexOfSpritePaletteTag(GFX_TAG_Z_TRIGGER) * 16];
		u8 i;
		
		for(i = 1; i < 16; i++) 
		{
			if (IsIgnoredTriggerColour(Z_Move_TriggerPal[i])) continue;
	
			switch(PALETTE_STATE) {
				case MegaTriggerLightUp:
					pal[i] = LightUpMegaSymbol(Z_Move_TriggerPal[i]);
					break;
				case MegaTriggerNormalColour:
					pal[i] = Z_Move_TriggerPal[i];
					break;
			}
		}
		
		self->data[2] = PALETTE_STATE;
	}
}

void TryLoadZTrigger(void)
{
	u8 objid;
	
	if (gBattleTypeFlags & (BATTLE_TYPE_SAFARI | BATTLE_TYPE_POKE_DUDE | BATTLE_TYPE_OLD_MAN))
		return;
	
	LoadSpritePalette(&ZTriggerPalette);
	LoadCompressedSpriteSheetUsingHeap(&ZTriggerSpriteSheet);
		
	objid = CreateSprite(&ZTriggerTemplate, 130, 90, 1);
	gSprites[objid].data[3] = 24;
	gSprites[objid].pos1.x = -32;
	gSprites[objid].data[4] = gActiveBattler;
}

void DestroyZTrigger(void)
{
	FreeSpritePaletteByTag(GFX_TAG_Z_TRIGGER);
    FreeSpriteTilesByTag(GFX_TAG_Z_TRIGGER);

	for (int i = 0; i < MAX_SPRITES; ++i)
	{
		if (gSprites[i].template->tileTag == GFX_TAG_Z_TRIGGER)
			DestroySprite(&gSprites[i]);
	}
}

move_t GetTypeBasedZMove(u16 move, u8 bank)
{
	u8 moveType = gBattleMoves[move].type;
	
	if (moveType < TYPE_FIRE)
		return MOVE_BREAKNECK_BLITZ_P + (moveType * 2) + CalcMoveSplit(bank, move);
	else if (moveType >= TYPE_FAIRY)
		return MOVE_TWINKLE_TACKLE_P + ((moveType - TYPE_FAIRY) * 2) + CalcMoveSplit(bank, move);
	else
		return MOVE_BREAKNECK_BLITZ_P + ((moveType - 1) * 2) + CalcMoveSplit(bank, move);
}

move_t GetSpecialZMove(u16 move, u16 species, u16 item)
{
	bool8 isSpecialZCrystal = FALSE;

	for (u32 i = 0; i < ARRAY_COUNT(sSpecialZMoveTable); ++i)
	{
		if (sSpecialZMoveTable[i].item == item)
		{
			isSpecialZCrystal = TRUE;

			if (sSpecialZMoveTable[i].species == species
			&&  sSpecialZMoveTable[i].move == move)
			{
				return sSpecialZMoveTable[i].zmove;
			}
		}
	}
	
	if (isSpecialZCrystal)
		return 0xFFFF;
	else
		return MOVE_NONE;
}

bool8 IsSpecialZCrystal(u16 item)
{
	for (u32 i = 0; i < ARRAY_COUNT(sSpecialZMoveTable); ++i)
	{
		if (sSpecialZMoveTable[i].item == item)
			return TRUE;
	}

	return FALSE;
}

const u8* GetZMoveName(u16 move)
{
	if (!IsZMove(move))
		return gZMoveNames[MOVE_BREAKNECK_BLITZ_P];
	else
		return gZMoveNames[move - MOVE_BREAKNECK_BLITZ_P];
}

bool8 DoesZMoveUsageStopMegaEvolution(u8 bank)
{
	if (gNewBS->ZMoveData->partyIndex[SIDE(bank)] & gBitTable[gBattlerPartyIndexes[bank]])
		return TRUE;
		
	return FALSE;
}

move_t CanUseZMove(u8 bank, u8 moveIndex, u16 move)
{
	struct Pokemon* mon = GetBankPartyData(bank);
	u16 item = GetMonData(mon, MON_DATA_HELD_ITEM, NULL);

	if (move == MOVE_NONE)
		move = gBattleMons[bank].moves[moveIndex];

	if (IsMega(bank)
	|| IsRedPrimal(bank)
	|| IsBluePrimal(bank))
		return MOVE_NONE;

	if (IsZCrystal(item) || item == ITEM_ULTRANECROZIUM_Z) //The only "Mega Stone" that let's you use a Z-Move
	{
		u16 zMove = GetSpecialZMove(move, SPECIES(bank), item);
		if (zMove != MOVE_NONE && zMove != 0xFFFF) //There's a special Z-Move
			return zMove;

		if (move != MOVE_NONE && zMove != 0xFFFF //Special Z-Crystal
		&& gBattleMoves[move].type == ItemId_GetHoldEffectParam(item))
		{
			if (SPLIT(move) == SPLIT_STATUS)
				return 0xFFFF;
			else
				return GetTypeBasedZMove(move, bank);
		}
	}

	return MOVE_NONE;
}


u16 ReplaceWithZMoveRuntime(u8 bankAtk, u16 move)
{
	if (gNewBS->ZMoveData->toBeUsed[bankAtk]
	&& !gNewBS->ZMoveData->used[bankAtk]
	&& SPLIT(move) != SPLIT_STATUS)
	{
		u16 zMove = CanUseZMove(bankAtk, 0xFF, move);
		if (zMove != MOVE_NONE)	
			move = zMove;
	}

	return move;
}

