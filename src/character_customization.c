#include "defines.h"
#include "helper_functions.h"
/*

npc_palletes:
	0805F4B0
	0805F570
	0805F5C8
	
	npc_change_type_maybe?
	

*/


u16 GetBackspriteId(void);
void LoadTrainerBackPal(u16 trainerPicId, u8 bank);

#ifdef UNBOUND
struct CharacterCustomizationPaletteSwitch
{
	u16 owNum;
	u8* frontSpritePal;
	u8* backSpritePal;
};

struct CharacterCustomizationPaletteSwitch CharacterPalSwitchTable[] = 
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

/*

typedef const struct EventObjectGraphicsInfo* NPCPtr;
#ifdef EXISTING_OW_TABLE_ADDRESS
	#define gOverworldTableSwitcher ((struct EventObjectGraphicsInfo***) EXISTING_OW_TABLE_ADDRESS)
#else
	// create 255 OW tables
	const struct EventObjectGraphicsInfo** gOverworldTableSwitcher[255] = {
		(NPCPtr*) 0x839fdb0,
		(NPCPtr*) 0,
};
#endif

//npc_get_type hack for character customization
//	hook at 0805F2C8 via r1
NPCPtr GetEventObjectGraphicsInfo(u32 gfxId) {
	u8 tableId = ((gfxId << 16) >> 24) & 0xFF;	// upper byte
	u8 spriteId = gfxId & 0xFF;		// lower byte
	u32 newId;

	// check runtime changeable OWs
	if (tableId == 0xFF && spriteId <= 0xF)
	{
		//runtime changeable
		newId = VarGet(VAR_RUNTIME_CHANGEABLE+spriteId);
		tableId = ((gfxId << 16) >> 24) & 0xFF;	// upper byte
		spriteId = (newId & 0xFF);		// lower byte
	}
	else
	{
		if (tableId == 0)
		{
			if (spriteId > 239)
			{
				newId = VarGetX4010(spriteId + 16);
				tableId = ((gfxId << 16) >> 24) & 0xFF;	// upper byte
				spriteId = (newId & 0xFF);		// lower byte
			}
			else	// load sprite/table IDs from vars
			{
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
					tableId = ((gfxId << 16) >> 24) & 0xFF;	// upper byte
					spriteId = (gfxId & 0xFF);		// lower byte
				}	// else, table and sprite ID stay the same
			}	// runtime changeable
		}	// non-zero table ID
	}
	NPCPtr spriteAddr = gOverworldTableSwitcher[tableId][spriteId];
	if (spriteAddr == 0)
		spriteAddr = gOverworldTableSwitcher[0][16];	// first non-player sprite in first table default
	return spriteAddr;
};


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

u16 GetBackspriteId(void) {
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

void LoadTrainerBackPal(u16 trainerPicId, u8 paletteNum) {
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

