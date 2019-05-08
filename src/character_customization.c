#include "defines.h"
#include "defines_battle.h"
#include "../include/link.h"
#include "../include/random.h"
#include "../include/constants/event_objects.h"

#include "../include/new/character_customization.h"
#include "../include/new/helper_functions.h"

#ifdef UNBOUND
struct CharacterCustomizationPaletteSwitch
{
	u16 owNum;
	u8* frontSpritePal;
	u8* backSpritePal;
};

static const struct CharacterCustomizationPaletteSwitch CharacterPalSwitchTable[] = 
{
	{262, (u8*) 0, (u8*) 0x8F08000},
	{271, (u8*) 0, (u8*) 0x8F08030},
	{280, (u8*) 0, (u8*) 0x8F08060},
	{289, (u8*) 0, (u8*) 0x8F08090},
	{298, (u8*) 0, (u8*) 0x8F080C0},
	{307, (u8*) 0, (u8*) 0x8F080F0},
	{316, (u8*) 0, (u8*) 0x8F08120},
	{325, (u8*) 0, (u8*) 0x8F08150},
	{334, (u8*) 0, (u8*) 0x8F08180},
	{343, (u8*) 0, (u8*) 0x8F081B0},
	{352, (u8*) 0, (u8*) 0x8F081E0},
	{361, (u8*) 0, (u8*) 0x8F08210},
	{370, (u8*) 0, (u8*) 0x8F08240},
	{379, (u8*) 0, (u8*) 0x8F08270},
	{0xFFFF, (u8*) 0, (u8*) 0},
};
#endif

#ifdef EXISTING_OW_TABLE_ADDRESS
	#define gOverworldTableSwitcher ((struct EventObjectGraphicsInfo***) EXISTING_OW_TABLE_ADDRESS)
#elif defined UNBOUND //For Pokemon Unbound
	const struct EventObjectGraphicsInfo** const gOverworldTableSwitcher[] = 
	{
		(NPCPtr*) 0x88110E0,
		(NPCPtr*) 0x88B2720,
		(NPCPtr*) 0x88B2B20,
	};

#else //Modify this
	// create 255 OW tables
	const struct EventObjectGraphicsInfo** const gOverworldTableSwitcher[255] = 
	{
		(NPCPtr*) 0x839fdb0,
		(NPCPtr*) 0x0,
		// etc...
		// please note that this method makes compatability with OW Manager challenging
	};
#endif

//npc_get_type hack for character customization
//hook at 0805F2C8 via r1
NPCPtr GetEventObjectGraphicsInfo(u16 graphicsId)
{
	u16 newId;
	u8 tableId = (graphicsId >> 8) & 0xFF;	// upper byte
	u8 spriteId = graphicsId & 0xFF;		// lower byte
	
	// check runtime changeable OWs
	if (tableId == 0xFF && spriteId <= 0xF)
	{
		//runtime changeable
		newId = VarGet(VAR_RUNTIME_CHANGEABLE+spriteId);
		tableId = (newId >> 8) & 0xFF;	// upper byte
		spriteId = (newId & 0xFF);		// lower byte
	}
	else
	{
		if (tableId == 0 && spriteId > 239)
		{
			newId = VarGetEventObjectGraphicsId(spriteId + 16);
			tableId = (newId >> 8) & 0xFF;	// upper byte
			spriteId = (newId & 0xFF);		// lower byte
		}
		else	// load sprite/table IDs from vars
		{
			u16 newId = 0;
			if ((spriteId == 0) || (spriteId == 7))
				newId = VarGet(VAR_PLAYER_WALKRUN);
			else if ((spriteId == 1) || (spriteId == 8))
				newId = VarGet(VAR_PLAYER_BIKING);
			else if ((spriteId == 2) || (spriteId == 9))
				newId = VarGet(VAR_PLAYER_SURFING);
			else if ((spriteId == 3) || (spriteId == 5) || (spriteId == 0xA) || (spriteId == 0xC))
				newId = VarGet(VAR_PLAYER_VS_SEEKER);
			else if ((spriteId == 4) || (spriteId == 0xB))
				newId = VarGet(VAR_PLAYER_FISHING);
			else if ((spriteId == 6) || (spriteId == 0xD))
				newId = VarGet(VAR_PLAYER_VS_SEEKER_ON_BIKE);
			
			// get updated table and sprite IDs
			if (newId != 0)
			{
				tableId = (newId >> 8) & 0xFF;	// upper byte
				spriteId = (newId & 0xFF);		// lower byte
			}	// else, table and sprite ID stay the same
		}	// runtime changeable
	}
	
	NPCPtr spriteAddr;
	if (tableId >= ARRAY_COUNT(gOverworldTableSwitcher)
	|| gOverworldTableSwitcher[tableId] == 0)
		spriteAddr = gOverworldTableSwitcher[0][spriteId];
	else
		spriteAddr = gOverworldTableSwitcher[tableId][spriteId];
	
	if (spriteAddr == 0)
		spriteAddr = gOverworldTableSwitcher[0][EVENT_OBJ_GFX_NINJA_BOY];	// first non-player sprite in first table default
	
	return spriteAddr;
};


/*
// load trainer card sprite based on variables
// 	hook at 810c374 via r2
void TrainerCardSprite(u8 gender, bool8 modify) {
	if (modify != TRUE)
		return;
	u16 trainerId = VarGet(VAR_TRAINERCARD_MALE + gender);
	if (trainerId == 0)
		trainerId = 0x87 + gender;	
};
*/

void PlayerHandleDrawTrainerPic(void)
{
    s16 xPos, yPos;
    u32 trainerPicId = GetBackspriteId();
	
    if (gBattleTypeFlags & (BATTLE_TYPE_MULTI | BATTLE_TYPE_INGAME_PARTNER))
    {
        if ((GetBattlerPosition(gActiveBattler) & BIT_FLANK) != B_FLANK_LEFT) // Second mon, on the right.
            xPos = 90;
        else // First mon, on the left.
            xPos = 32;
		
        yPos = (8 - gTrainerBackPicCoords[trainerPicId].coords) * 4 + 80;
    }
	
    else
    {
        xPos = 80;
        yPos = (8 - gTrainerBackPicCoords[trainerPicId].coords) * 4 + 80;
    }

	LoadTrainerBackPal(trainerPicId, gActiveBattler);
    SetMultiuseSpriteTemplateToTrainerBack(trainerPicId, GetBattlerPosition(gActiveBattler));
    gBattlerSpriteIds[gActiveBattler] = CreateSprite(&gMultiuseSpriteTemplate[0], xPos, yPos, GetBattlerSpriteSubpriority(gActiveBattler));

    gSprites[gBattlerSpriteIds[gActiveBattler]].oam.paletteNum = gActiveBattler;
    gSprites[gBattlerSpriteIds[gActiveBattler]].pos2.x = 240;
    gSprites[gBattlerSpriteIds[gActiveBattler]].data[0] = -2;
    gSprites[gBattlerSpriteIds[gActiveBattler]].callback = sub_8033EEC; //sub_805D7AC in Emerald

    gBattleBankFunc[gActiveBattler] = (u32) sub_802F730;
}

void PlayerHandleTrainerSlide(void)
{
    u32 trainerPicId = GetBackspriteId();
	
	LoadTrainerBackPal(trainerPicId, gActiveBattler);
    SetMultiuseSpriteTemplateToTrainerBack(trainerPicId, GetBattlerPosition(gActiveBattler));
    gBattlerSpriteIds[gActiveBattler] = CreateSprite(&gMultiuseSpriteTemplate[0], 80, (8 - gTrainerBackPicCoords[trainerPicId].coords) * 4 + 80, 30);

    gSprites[gBattlerSpriteIds[gActiveBattler]].oam.paletteNum = gActiveBattler;
    gSprites[gBattlerSpriteIds[gActiveBattler]].pos2.x = -96;
    gSprites[gBattlerSpriteIds[gActiveBattler]].data[0] = 2;
    gSprites[gBattlerSpriteIds[gActiveBattler]].callback = sub_8033EEC;

    gBattleBankFunc[gActiveBattler] = (u32) sub_802F768;
}

u16 GetBackspriteId(void)
{
	u16 trainerPicId;
	
	if (gBattleTypeFlags & BATTLE_TYPE_LINK)
    {
        if ((gLinkPlayers[GetMultiplayerId()].version & 0xFF) == VERSION_FIRE_RED
        || (gLinkPlayers[GetMultiplayerId()].version & 0xFF) == VERSION_LEAF_GREEN)
            trainerPicId = gLinkPlayers[GetMultiplayerId()].gender;
        else
            trainerPicId = gLinkPlayers[GetMultiplayerId()].gender + BACK_PIC_BRENDAN;
    }
	
	else if (gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER && gActiveBattler == 2)
		trainerPicId = VarGet(PARTNER_BACKSPRITE_VAR);
		
    else 
	{
		if (VarGet(BACKSPRITE_SWITCH_VAR))
			trainerPicId = VarGet(BACKSPRITE_SWITCH_VAR);
		else
			trainerPicId = gSaveBlock2->playerGender;
	}
	return trainerPicId;
}

void LoadTrainerBackPal(u16 trainerPicId, u8 paletteNum)
{
	#ifdef UNBOUND
	//Changes the skin tones of the player character in Unbound
		if (VarGet(OW_SPRITE_SWITCH_VAR) && gActiveBattler == 0) {
			for (int i = 0; CharacterPalSwitchTable[i].owNum != 0xFFFF; ++i) {
				if (CharacterPalSwitchTable[i].owNum == VarGet(OW_SPRITE_SWITCH_VAR)) {
					LoadCompressedPalette(CharacterPalSwitchTable[i].backSpritePal, 0x100 + paletteNum * 16, 32);
					break;
				}
				else if (CharacterPalSwitchTable[i].owNum == 0xFFFF)
					DecompressTrainerBackPic(trainerPicId, paletteNum);
			}			
		}
		else {
			DecompressTrainerBackPic(trainerPicId, paletteNum);
		}
	#else
		DecompressTrainerBackPic(trainerPicId, paletteNum);
	#endif
}

