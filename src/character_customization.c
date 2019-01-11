#include "defines.h"
#include "helper_functions.h"

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
            trainerPicId = gLinkPlayers[GetMultiplayerId()].gender + BACK_PIC_RED;
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