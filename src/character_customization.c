#include "defines.h"
#include "defines_battle.h"
#include "../include/event_object_movement.h"
#include "../include/field_effect_helpers.h"
#include "../include/field_player_avatar.h"
#include "../include/link.h"
#include "../include/random.h"
#include "../include/sprite.h"
#include "../include/constants/event_object_movement_constants.h"
#include "../include/constants/event_objects.h"
#include "../include/constants/trainers.h"

#include "../include/new/character_customization.h"
#include "../include/new/follow_me.h"
#include "../include/new/multi.h"
#include "../include/new/util.h"
/*
character_customization.c
	functions for altering the player's sprite based on the current sprite/palette selections

tables to edit:
	gOverworldTableSwitcher
	sPlayerAvatarGfxIds

*/

#ifdef UNBOUND
extern const u8 TS_Male_Player_White_Brunette_BlackPal[];
extern const u8 TS_Male_Player_White_Silver_RedPal[];
extern const u8 TS_Male_Player_White_Silver_BlackPal[];
extern const u8 TS_Male_Player_Brown_Dark_RedPal[];
extern const u8 TS_Male_Player_Brown_Dark_BlackPal[];
extern const u8 TS_Male_Player_Brown_Silver_RedPal[];
extern const u8 TS_Male_Player_Brown_Silver_BlackPal[];
extern const u8 BS_Male_Player_White_Brunette_BlackPal[];
extern const u8 BS_Male_Player_White_Silver_RedPal[];
extern const u8 BS_Male_Player_White_Silver_BlackPal[];
extern const u8 BS_Male_Player_Brown_Dark_RedPal[];
extern const u8 BS_Male_Player_Brown_Dark_BlackPal[];
extern const u8 BS_Male_Player_Brown_Silver_RedPal[];
extern const u8 BS_Male_Player_Brown_Silver_BlackPal[];
extern const u8 TS_Female_Player_White_Brunette_PurplePal[];
extern const u8 TS_Female_Player_White_Silver_RedPal[];
extern const u8 TS_Female_Player_White_Silver_PurplePal[];
extern const u8 TS_Female_Player_Brown_Dark_RedPal[];
extern const u8 TS_Female_Player_Brown_Dark_PurplePal[];
extern const u8 TS_Female_Player_Brown_Silver_RedPal[];
extern const u8 TS_Female_Player_Brown_Silver_PurplePal[];
extern const u8 BS_Female_Player_White_Brunette_PurplePal[];
extern const u8 BS_Female_Player_White_Silver_RedPal[];
extern const u8 BS_Female_Player_White_Silver_PurplePal[];
extern const u8 BS_Female_Player_Brown_Dark_RedPal[];
extern const u8 BS_Female_Player_Brown_Dark_PurplePal[];
extern const u8 BS_Female_Player_Brown_Silver_RedPal[];
extern const u8 BS_Female_Player_Brown_Silver_PurplePal[];

struct CharacterCustomizationPaletteSwitch
{
	u16 owNum;
	const u8* frontSpritePal;
	const u8* backSpritePal;
};

static const struct CharacterCustomizationPaletteSwitch sCharacterPalSwitchTable[] =
{
	{262, 		TS_Male_Player_White_Brunette_BlackPal, BS_Male_Player_White_Brunette_BlackPal},
	{271, 		TS_Male_Player_White_Silver_RedPal, BS_Male_Player_White_Silver_RedPal},
	{280, 		TS_Male_Player_White_Silver_BlackPal, BS_Male_Player_White_Silver_BlackPal},
	{289, 		TS_Male_Player_Brown_Dark_RedPal, BS_Male_Player_Brown_Dark_RedPal},
	{298, 		TS_Male_Player_Brown_Dark_BlackPal, BS_Male_Player_Brown_Dark_BlackPal},
	{307, 		TS_Male_Player_Brown_Silver_RedPal, BS_Male_Player_Brown_Silver_RedPal},
	{316, 		TS_Male_Player_Brown_Silver_BlackPal, BS_Male_Player_Brown_Silver_BlackPal},
	{325, 		TS_Female_Player_White_Brunette_PurplePal, BS_Female_Player_White_Brunette_PurplePal},
	{334, 		TS_Female_Player_White_Silver_RedPal, BS_Female_Player_White_Silver_RedPal},
	{343, 		TS_Female_Player_White_Silver_PurplePal, BS_Female_Player_White_Silver_PurplePal},
	{352, 		TS_Female_Player_Brown_Dark_RedPal, BS_Female_Player_Brown_Dark_RedPal},
	{361, 		TS_Female_Player_Brown_Dark_PurplePal, BS_Female_Player_Brown_Dark_PurplePal},
	{370, 		TS_Female_Player_Brown_Silver_RedPal, BS_Female_Player_Brown_Silver_RedPal},
	{379, 		TS_Female_Player_Brown_Silver_PurplePal, BS_Female_Player_Brown_Silver_PurplePal},
	{0xFFFF, 	NULL, NULL},
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
		(NPCPtr*) 0x839FDB0,
		(NPCPtr*) 0x0,
		// etc...
		// please note that this method makes compatability with OW Manager challenging
	};
#endif

struct PlayerGraphics
{
	u16 graphicsId;
	u8 stateFlag;
};

static const struct PlayerGraphics sPlayerAvatarGfxIds[][2] =
{
	[PLAYER_AVATAR_STATE_NORMAL] =			{{EVENT_OBJ_GFX_RED_NORMAL, PLAYER_AVATAR_FLAG_ON_FOOT},			{EVENT_OBJ_GFX_LEAF_NORMAL, PLAYER_AVATAR_FLAG_ON_FOOT}},
	[PLAYER_AVATAR_STATE_BIKE] =			{{EVENT_OBJ_GFX_RED_BIKE, PLAYER_AVATAR_FLAG_BIKE},					{EVENT_OBJ_GFX_LEAF_BIKE, PLAYER_AVATAR_FLAG_BIKE}},
	[PLAYER_AVATAR_STATE_SURFING] =			{{EVENT_OBJ_GFX_RED_SURFING, PLAYER_AVATAR_FLAG_SURFING},			{EVENT_OBJ_GFX_LEAF_SURFING, PLAYER_AVATAR_FLAG_SURFING}},
	[PLAYER_AVATAR_STATE_FIELD_MOVE] =		{{EVENT_OBJ_GFX_RED_FIELD_MOVE, 0},									{EVENT_OBJ_GFX_LEAF_FIELD_MOVE, 0}},
	[PLAYER_AVATAR_STATE_FISHING] =			{{EVENT_OBJ_GFX_RED_FISHING, 0},									{EVENT_OBJ_GFX_LEAF_FISHING, 0}},
	[PLAYER_AVATAR_STATE_FIELD_MOVE_2] =	{{EVENT_OBJ_GFX_RED_FIELD_MOVE_2, 0},								{EVENT_OBJ_GFX_LEAF_FIELD_MOVE_2, 0}},
	[PLAYER_AVATAR_STATE_UNDERWATER] =		{{EVENT_OBJ_GFX_RED_UNDERWATER, PLAYER_AVATAR_FLAG_UNDERWATER},		{EVENT_OBJ_GFX_LEAF_UNDERWATER, PLAYER_AVATAR_FLAG_UNDERWATER}},
};

//This file's functions:
#ifdef UNBOUND
static const u8* GetAlternateTrainerSpritePal(void);
#endif

//npc_get_type hack for character customization
//hook at 0805F2C8 via r1
NPCPtr GetEventObjectGraphicsInfo(u16 graphicsId)
{
	u16 newId;
	u8 tableId = (graphicsId >> 8) & 0xFF;	// upper byte
	u8 spriteId = graphicsId & 0xFF;		// lower byte

	//Check runtime changeable OWs
	if (tableId == 0xFF && spriteId <= 0xF)
	{
		//Runtime changeable
		newId = VarGet(VAR_RUNTIME_CHANGEABLE + spriteId);
		tableId = (newId >> 8) & 0xFF;	// upper byte
		spriteId = (newId & 0xFF);		// lower byte
	}
	else
	{
		switch (spriteId) {
			case EVENT_OBJ_GFX_RED_BIKE_VS_SEEKER:
			case EVENT_OBJ_GFX_LEAF_BIKE_VS_SEEKER:
				newId = VarGet(VAR_PLAYER_VS_SEEKER_ON_BIKE);
				break;
		}

		if (spriteId > 239 && tableId == 0)
		{
			newId = VarGetEventObjectGraphicsId(spriteId + 16);
			tableId = (newId >> 8) & 0xFF;	// upper byte
			spriteId = (newId & 0xFF);		// lower byte
		}
	}

	NPCPtr spriteAddr;
	#ifndef EXISTING_OW_TABLE_ADDRESS
	if (tableId >= ARRAY_COUNT(gOverworldTableSwitcher)
	|| gOverworldTableSwitcher[tableId] == 0)
		spriteAddr = gOverworldTableSwitcher[0][spriteId];
	else
	#endif
		spriteAddr = gOverworldTableSwitcher[tableId][spriteId];

	if (spriteAddr == NULL)
		spriteAddr = gOverworldTableSwitcher[0][EVENT_OBJ_GFX_LITTLE_BOY];	// first non-player sprite in first table default

	return spriteAddr;
};

NPCPtr GetEventObjectGraphicsInfoByEventObj(struct EventObject* eventObj)
{
	return GetEventObjectGraphicsInfo(GetEventObjectGraphicsId(eventObj));
}

static u16 GetCustomGraphicsIdByState(u8 state)
{
	u16 spriteId = 0;

	switch (state) {
		case PLAYER_AVATAR_STATE_NORMAL:
			spriteId = VarGet(VAR_PLAYER_WALKRUN);
			break;
		case PLAYER_AVATAR_STATE_BIKE:
			spriteId = VarGet(VAR_PLAYER_BIKING);
			break;
		case PLAYER_AVATAR_STATE_SURFING:
			spriteId = VarGet(VAR_PLAYER_SURFING);
			break;
		case PLAYER_AVATAR_STATE_FIELD_MOVE:
		case PLAYER_AVATAR_STATE_FIELD_MOVE_2:
			spriteId = VarGet(VAR_PLAYER_VS_SEEKER);
			break;
		case PLAYER_AVATAR_STATE_FISHING:
			spriteId = VarGet(VAR_PLAYER_FISHING);
			break;
		case PLAYER_AVATAR_STATE_UNDERWATER:
			spriteId = VarGet(VAR_PLAYER_UNDERWATER);
			break;
	}

	return spriteId;
}

u16 GetPlayerAvatarGraphicsIdByStateIdAndGender(u8 state, u8 gender)
{
	u16 graphicsId = GetCustomGraphicsIdByState(state);
	if (graphicsId != 0)
		return graphicsId;

	return sPlayerAvatarGfxIds[state][gender].graphicsId;
}

u16 GetPlayerAvatarGraphicsIdByStateId(u8 state)
{
	return GetPlayerAvatarGraphicsIdByStateIdAndGender(state, gPlayerAvatar->gender);
}

u8 GetPlayerAvatarStateTransitionByGraphicsId(u16 graphicsId, u8 gender)
{
    for (u8 state = 0; state < ARRAY_COUNT(sPlayerAvatarGfxIds); ++state)
    {
		u16 customGraphicsId = GetCustomGraphicsIdByState(state);
		if (customGraphicsId == graphicsId)
			graphicsId = sPlayerAvatarGfxIds[state][gender].graphicsId;

        if (sPlayerAvatarGfxIds[state][gender].graphicsId == graphicsId)
            return sPlayerAvatarGfxIds[state][gender].stateFlag;
    }

    return PLAYER_AVATAR_FLAG_ON_FOOT;
}

u16 GetPlayerAvatarGraphicsIdByCurrentState(void)
{
	u8 state = 0;
	u8 gender = gPlayerAvatar->gender;
    u8 flags = gPlayerAvatar->flags;

    for (; state < ARRAY_COUNT(sPlayerAvatarGfxIds); ++state)
    {
        if (sPlayerAvatarGfxIds[state][gender].stateFlag & flags)
		{
			u16 graphicsId = sPlayerAvatarGfxIds[state][gender].graphicsId;
			u16 customGraphicsId = GetCustomGraphicsIdByState(state);
			if (customGraphicsId != 0)
				graphicsId = customGraphicsId;

			return graphicsId;
		}
    }

    return sPlayerAvatarGfxIds[0][0].graphicsId;
}

u8 GetPlayerAvatarGenderByGraphicsId(u8 gfxId)
{
    for (u8 state = 0; state < ARRAY_COUNT(sPlayerAvatarGfxIds); ++state)
    {
		for (u8 gender = 0; gender < ARRAY_COUNT(sPlayerAvatarGfxIds[0]); ++gender)
		{
			if (sPlayerAvatarGfxIds[state][gender].graphicsId == gfxId)
				return gender;

			u16 customGraphicsId = GetCustomGraphicsIdByState(state);
			if (customGraphicsId != 0 && customGraphicsId == gfxId)
				return gSaveBlock2->playerGender;
		}
	}

	return MALE;
}

static void SetPlayerAvatarExtraStateTransition(u16 graphicsId, u8 b)
{
    u8 unk = GetPlayerAvatarStateTransitionByGraphicsId(graphicsId, gPlayerAvatar->gender);
	SetPlayerAvatarTransitionFlags(unk | b);
}

u16 GetEventObjectGraphicsId(struct EventObject* eventObj)
{
	u8 lowerByte = eventObj->graphicsIdLowerByte;
	u8 upperByte = eventObj->graphicsIdUpperByte;

	#ifndef EXISTING_OW_TABLE_ADDRESS
	if (upperByte >= ARRAY_COUNT(gOverworldTableSwitcher)
	&& upperByte != 0xFF) //Dynamic OW table
		return lowerByte;
	#endif

	return lowerByte | (upperByte << 8);
}

void SetPlayerAvatarEventObjectIdAndObjectId(u8 eventObjectId, u8 spriteId)
{
    gPlayerAvatar->eventObjectId = eventObjectId;
    gPlayerAvatar->spriteId = spriteId;
    gPlayerAvatar->gender = GetPlayerAvatarGenderByGraphicsId(GetEventObjectGraphicsId(&gEventObjects[eventObjectId]));
    SetPlayerAvatarExtraStateTransition(GetEventObjectGraphicsId(&gEventObjects[eventObjectId]), 0x20);
}

// load trainer card sprite based on variables
// 	hook at 810c374 via r2
u8 PlayerGenderToFrontTrainerPicId(u8 gender, bool8 modify)
{
	if (modify != TRUE)
		return gender;

	u16 trainerId = VarGet(VAR_TRAINERCARD_MALE + gender);
	if (trainerId == 0)
		trainerId = 0x87 + gender;

	return trainerId;
};

void InitPlayerAvatar(s16 x, s16 y, u8 direction, u8 gender)
{
	u8 eventObjectId;
	struct EventObject* eventObject;
	struct EventObjectTemplate playerEventObjTemplate = {0};
	u16 graphicsId = GetPlayerAvatarGraphicsIdByStateIdAndGender(PLAYER_AVATAR_STATE_NORMAL, gender);

	playerEventObjTemplate.localId = EVENT_OBJ_ID_PLAYER;
	playerEventObjTemplate.graphicsIdLowerByte = graphicsId & 0xFF;
	playerEventObjTemplate.graphicsIdUpperByte = graphicsId >> 8;
	playerEventObjTemplate.x = x - 7;
	playerEventObjTemplate.y = y - 7;
	playerEventObjTemplate.movementType = MOVEMENT_TYPE_PLAYER;

	eventObjectId = SpawnSpecialEventObject(&playerEventObjTemplate);
	eventObject = &gEventObjects[eventObjectId];
	eventObject->isPlayer = 1;
	eventObject->warpArrowSpriteId = CreateWarpArrowSprite();
	EventObjectTurn(eventObject, direction);
	ClearPlayerAvatarInfo();

	gPlayerAvatar->runningState = NOT_MOVING;
	gPlayerAvatar->tileTransitionState = T_NOT_MOVING;
	gPlayerAvatar->eventObjectId = eventObjectId;
	gPlayerAvatar->spriteId = eventObject->spriteId;
	gPlayerAvatar->gender = gender;
	SetPlayerAvatarStateMask(PLAYER_AVATAR_FLAG_FIELD_MOVE | PLAYER_AVATAR_FLAG_ON_FOOT);
	CreateFollowerAvatar();
}

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
	gSprites[gBattlerSpriteIds[gActiveBattler]].data[0] = -3; //-2; //Speed scrolling in
	gSprites[gBattlerSpriteIds[gActiveBattler]].callback = sub_8033EEC; //sub_805D7AC in Emerald

	gBattlerControllerFuncs[gActiveBattler] = sub_802F730;
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

	gBattlerControllerFuncs[gActiveBattler] = sub_802F768;
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
	{
		trainerPicId = LoadPartnerBackspriteIndex();
	}
	else if (gBattleTypeFlags & BATTLE_TYPE_BATTLE_SANDS)
	{
		trainerPicId = LoadPartnerBackspriteIndex(); //The trainer's backsprite for the Battle Sands is stored in the multi partner var
	}
	else
	{
		if (VarGet(VAR_BACKSPRITE_SWITCH))
			trainerPicId = VarGet(VAR_BACKSPRITE_SWITCH);
		else
			trainerPicId = gSaveBlock2->playerGender;
	}

	return trainerPicId;
}

void LoadTrainerBackPal(u16 trainerPicId, u8 paletteNum)
{
	#ifdef UNBOUND
	//Changes the skin tones of the player character in Unbound
		u16 owNum = VarGet(VAR_PLAYER_WALKRUN);

		if (VarGet(VAR_PLAYER_WALKRUN) && gActiveBattler == 0 && (trainerPicId == TRAINER_BACK_PIC_RED || trainerPicId == TRAINER_BACK_PIC_LEAF))
		{
			for (int i = 0; sCharacterPalSwitchTable[i].owNum != 0xFFFF; ++i)
			{
				if (sCharacterPalSwitchTable[i].owNum == owNum)
				{
					LoadCompressedPalette(sCharacterPalSwitchTable[i].backSpritePal, 0x100 + paletteNum * 16, 32);
					break;
				}
			}
		}
		else
			DecompressTrainerBackPic(trainerPicId, paletteNum);
	#else
		DecompressTrainerBackPic(trainerPicId, paletteNum);
	#endif
}

void TryUpdateTrainerPicPalTrainerCard(u16 trainerPicId, u16 palOffset)
{
	LoadCompressedPalette(GetTrainerSpritePal(trainerPicId), palOffset * 0x10, 0x20);
}

const u8* GetTrainerSpritePal(u16 trainerPicId)
{
	#ifdef UNBOUND
	const u8* palette;
	if (VarGet(VAR_PLAYER_WALKRUN) != 0)
	{
		if ((gSaveBlock2->playerGender == FEMALE && VarGet(VAR_TRAINERCARD_FEMALE) == 0)
		||  (gSaveBlock2->playerGender != FEMALE && VarGet(VAR_TRAINERCARD_MALE) == 0))
		{
			if (trainerPicId == TRAINER_PIC_PLAYER_M || trainerPicId == TRAINER_PIC_PLAYER_F)
			{
				if ((palette = GetAlternateTrainerSpritePal()) != NULL)
					return palette;
			}
		}
	}
	#endif

	return gTrainerFrontPicPaletteTable[trainerPicId].data;
}

#ifdef UNBOUND
static const u8* GetAlternateTrainerSpritePal(void)
{
	u16 owNum = VarGet(VAR_PLAYER_WALKRUN);

	for (int i = 0; sCharacterPalSwitchTable[i].owNum != 0xFFFF; ++i)
	{
		if (sCharacterPalSwitchTable[i].owNum == owNum)
			return sCharacterPalSwitchTable[i].frontSpritePal;
	}

	return NULL;
}
#endif
