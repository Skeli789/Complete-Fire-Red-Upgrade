#include "defines.h"
#include "defines_battle.h"
#include "../include/battle_anim.h"
#include "../include/m4a.h"
#include "../include/string_util.h"
#include "../include/new/battle_indicators.h"
#include "../include/new/battle_util.h"
#include "../include/new/dynamax.h"
#include "../include/new/item.h"
#include "../include/new/mega.h"
#include "../include/new/move_menu.h"
#include "../include/new/set_z_effect.h"

extern const u8 Mega_IndicatorTiles[];
extern const u8 Alpha_IndicatorTiles[];
extern const u8 Omega_IndicatorTiles[];
extern const u8 Ultra_IndicatorTiles[];
extern const u16 Mega_IndicatorPal[];
extern const u8 Dynamax_IndicatorTiles[];
extern const u8 Mega_TriggerTiles[];
extern const u8 Ultra_TriggerTiles[];
extern const u16 Mega_TriggerPal[];
extern const u8 Z_Move_TriggerTiles[];
extern const u16 Z_Move_TriggerPal[];
extern const u8 Dynamax_TriggerTiles[]; //For some reason this doesn't work
extern const u8 Dynamax_Trigger_WorkingTiles[]; //This is used as the image until the bug is fixed
extern const u16 Dynamax_TriggerPal[];
extern const u8 Raid_ShieldTiles[];

static bool8 IsIgnoredTriggerColour(u16 colour);
static struct Sprite* GetHealthboxSprite(u8 bank);
static u16 ConvertColorToGrayscale(u16 colour);
static u16 LightUpTriggerSymbol(u16 clra);
static void SpriteCB_MegaTrigger(struct Sprite* self);
static void SpriteCB_MegaIndicator(struct Sprite* self);
static void SpriteCB_ZTrigger(struct Sprite* self);
static void SpriteCB_DynamaxTrigger(struct Sprite* self);
static void SpriteCB_RaidShield(struct Sprite* sprite);
static void DestroyMegaTriggers(void);
static void DestroyZTrigger(void);
static void DestroyDynamaxTrigger(void);

enum MegaGraphicsTags
{
	GFX_TAG_MEGA_INDICATOR = 0xFDF0,
	GFX_TAG_ALPHA_INDICATOR,
	GFX_TAG_OMEGA_INDICATOR,
	GFX_TAG_ULTRA_INDICATOR,
	GFX_TAG_MEGA_TRIGGER,
	GFX_TAG_ULTRA_TRIGGER,
	GFX_TAG_Z_TRIGGER,
	GFX_TAG_DYNAMAX_INDICATOR,
	GFX_TAG_DYNAMAX_TRIGGER,
	GFX_TAG_RAID_SHIELD,
};

enum
{
	MegaTriggerNothing,
	MegaTriggerLightUp,
	MegaTriggerNormalColour,
	MegaTriggerGrayscale,
};

static const struct CompressedSpriteSheet sMegaIndicatorSpriteSheet = {Mega_IndicatorTiles, (8 * 8) / 2, GFX_TAG_MEGA_INDICATOR};
static const struct CompressedSpriteSheet sAlphaIndicatorSpriteSheet = {Alpha_IndicatorTiles, (8 * 8) / 2, GFX_TAG_ALPHA_INDICATOR};
static const struct CompressedSpriteSheet sOmegaIndicatorSpriteSheet = {Omega_IndicatorTiles, (8 * 8) / 2, GFX_TAG_OMEGA_INDICATOR};
static const struct CompressedSpriteSheet sUltraIndicatorSpriteSheet = {Ultra_IndicatorTiles, (8 * 8) / 2, GFX_TAG_ULTRA_INDICATOR};
static const struct SpritePalette sMegaIndicatorPalette = {Mega_IndicatorPal, GFX_TAG_MEGA_INDICATOR};

static const struct CompressedSpriteSheet sMegaTriggerSpriteSheet = {Mega_TriggerTiles, (32 * 32) / 2, GFX_TAG_MEGA_TRIGGER};
static const struct CompressedSpriteSheet sUltraTriggerSpriteSheet = {Ultra_TriggerTiles, (32 * 32) / 2, GFX_TAG_ULTRA_TRIGGER};
static const struct SpritePalette sMegaTriggerPalette = {Mega_TriggerPal, GFX_TAG_MEGA_TRIGGER};

static const struct CompressedSpriteSheet sZTriggerSpriteSheet = {Z_Move_TriggerTiles, (32 * 32) / 2, GFX_TAG_Z_TRIGGER};
static const struct SpritePalette sZTriggerPalette = {Z_Move_TriggerPal, GFX_TAG_Z_TRIGGER};

static const struct CompressedSpriteSheet sDynamaxIndicatorSpriteSheet = {Dynamax_IndicatorTiles, (8 * 8) / 2, GFX_TAG_DYNAMAX_INDICATOR};
static const struct CompressedSpriteSheet sDynamaxTriggerSpriteSheet = {Dynamax_Trigger_WorkingTiles, (32 * 32) / 2, GFX_TAG_DYNAMAX_TRIGGER};
static const struct SpritePalette sDynamaxTriggerPalette = {Dynamax_TriggerPal, GFX_TAG_DYNAMAX_TRIGGER};
static const struct CompressedSpriteSheet sRaidShieldSpriteSheet = {Raid_ShieldTiles, (16 * 8) / 2, GFX_TAG_RAID_SHIELD};

static const struct OamData sIndicatorOam =
{
	.affineMode = ST_OAM_AFFINE_OFF,
	.objMode = ST_OAM_OBJ_NORMAL,
	.shape = SPRITE_SHAPE(8x8),
	.size = SPRITE_SIZE(8x8),
	.priority = 0, //Above sprites
};

static const union AffineAnimCmd sSpriteAffineAnim_RaidShieldCreate[] =
{
	AFFINEANIMCMD_FRAME(32, 32, 0, 4), //Double in size
	AFFINEANIMCMD_FRAME(-8, -8, 0, 16), //Shrink into place
	AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sSpriteAffineAnim_RaidShieldDestroy[] =
{
	AFFINEANIMCMD_FRAME(32, 32, 0, 4), //Double in size
	AFFINEANIMCMD_END,
};

static const union AffineAnimCmd* const sSpriteAffineAnimTable_RaidShield[] =
{
	sSpriteAffineAnim_RaidShieldCreate,
	sSpriteAffineAnim_RaidShieldDestroy,
};

static const struct OamData sTriggerOam =
{
	.affineMode = ST_OAM_AFFINE_OFF,
	.objMode = ST_OAM_OBJ_NORMAL,
	.shape = SPRITE_SHAPE(32x32),
	.size = SPRITE_SIZE(32x32),
	.priority = 1, //Above Pokemon sprites
};

static const struct OamData sRaidShieldOam =
{
	.affineMode = ST_OAM_AFFINE_DOUBLE,
	.objMode = ST_OAM_OBJ_NORMAL,
	.shape = SPRITE_SHAPE(16x8),
	.size = SPRITE_SIZE(16x8),
	.priority = 0, //Above health bar
};

static const struct SpriteTemplate sMegaIndicatorSpriteTemplate =
{
	.tileTag = GFX_TAG_MEGA_INDICATOR,
	.paletteTag = GFX_TAG_MEGA_INDICATOR,
	.oam = &sIndicatorOam,
	.anims = gDummySpriteAnimTable,
	.images = NULL,
	.affineAnims = gDummySpriteAffineAnimTable,
	.callback = SpriteCB_MegaIndicator,
};

static const struct SpriteTemplate sAlphaIndicatorSpriteTemplate =
{
	.tileTag = GFX_TAG_ALPHA_INDICATOR,
	.paletteTag = GFX_TAG_MEGA_INDICATOR,
	.oam = &sIndicatorOam,
	.anims = gDummySpriteAnimTable,
	.images = NULL,
	.affineAnims = gDummySpriteAffineAnimTable,
	.callback = SpriteCB_MegaIndicator,
};

static const struct SpriteTemplate sOmegaIndicatorSpriteTemplate =
{
	.tileTag = GFX_TAG_OMEGA_INDICATOR,
	.paletteTag = GFX_TAG_MEGA_INDICATOR,
	.oam = &sIndicatorOam,
	.anims = gDummySpriteAnimTable,
	.images = NULL,
	.affineAnims = gDummySpriteAffineAnimTable,
	.callback = SpriteCB_MegaIndicator,
};

static const struct SpriteTemplate sUltraIndicatorSpriteTemplate =
{
	.tileTag = GFX_TAG_ULTRA_INDICATOR,
	.paletteTag = GFX_TAG_MEGA_INDICATOR,
	.oam = &sIndicatorOam,
	.anims = gDummySpriteAnimTable,
	.images = NULL,
	.affineAnims = gDummySpriteAffineAnimTable,
	.callback = SpriteCB_MegaIndicator,
};

static const struct SpriteTemplate sMegaTriggerSpriteTemplate =
{
	.tileTag = GFX_TAG_MEGA_TRIGGER,
	.paletteTag = GFX_TAG_MEGA_TRIGGER,
	.oam = &sTriggerOam,
	.anims = gDummySpriteAnimTable,
	.images = NULL,
	.affineAnims = gDummySpriteAffineAnimTable,
	.callback = SpriteCB_MegaTrigger,
};

static const struct SpriteTemplate sUltraTriggerSpriteTemplate =
{
	.tileTag = GFX_TAG_ULTRA_TRIGGER,
	.paletteTag = GFX_TAG_MEGA_TRIGGER,
	.oam = &sTriggerOam,
	.anims = gDummySpriteAnimTable,
	.images = NULL,
	.affineAnims = gDummySpriteAffineAnimTable,
	.callback = SpriteCB_MegaTrigger,
};

static const struct SpriteTemplate sZTriggerSpriteTemplate =
{
	.tileTag = GFX_TAG_Z_TRIGGER,
	.paletteTag = GFX_TAG_Z_TRIGGER,
	.oam = &sTriggerOam,
	.anims = gDummySpriteAnimTable,
	.images = NULL,
	.affineAnims = gDummySpriteAffineAnimTable,
	.callback = SpriteCB_ZTrigger,
};

static const struct SpriteTemplate sDynamaxIndicatorSpriteTemplate =
{
	.tileTag = GFX_TAG_DYNAMAX_INDICATOR,
	.paletteTag = GFX_TAG_MEGA_INDICATOR,
	.oam = &sIndicatorOam,
	.anims = gDummySpriteAnimTable,
	.images = NULL,
	.affineAnims = gDummySpriteAffineAnimTable,
	.callback = SpriteCB_MegaIndicator,
};

static const struct SpriteTemplate sDynamaxTriggerSpriteTemplate =
{
	.tileTag = GFX_TAG_DYNAMAX_TRIGGER,
	.paletteTag = GFX_TAG_DYNAMAX_TRIGGER,
	.oam = &sTriggerOam,
	.anims = gDummySpriteAnimTable,
	.images = NULL,
	.affineAnims = gDummySpriteAffineAnimTable,
	.callback = SpriteCB_DynamaxTrigger,
};

const struct SpriteTemplate gRaidShieldSpriteTemplate =
{
	.tileTag = GFX_TAG_RAID_SHIELD,
	.paletteTag = TAG_HEALTHBAR_PLAYER1_TILE,
	.oam = &sRaidShieldOam,
	.anims = gDummySpriteAnimTable,
	.images = NULL,
	.affineAnims = sSpriteAffineAnimTable_RaidShield,
	.callback = SpriteCB_RaidShield,
};

/* Declare the colours the trigger button doesn't light up */
static const u16 sIgnoredTriggerColours[] =
{
	RGB(7, 10, 8),
	RGB(7, 6, 6),
	RGB(15, 18, 16),
	RGB(10, 13, 12),
	RGB(13, 13, 11),
	RGB(31, 31, 31),
	RGB(4, 7, 0),
	RGB(4, 4, 0),
	RGB(0, 0, 0),
};

/* Easy match function */
static bool8 IsIgnoredTriggerColour(u16 colour)
{
	for (u32 i = 0; i < ARRAY_COUNT(sIgnoredTriggerColours); ++i)
	{
		if (sIgnoredTriggerColours[i] == colour)
			return TRUE;
	}

	return FALSE;
}

static struct Sprite* GetHealthboxSprite(u8 bank)
{
	return &gSprites[gHealthboxSpriteIds[bank]];
}

static u16 ConvertColorToGrayscale(u16 colour)
{
	s32 r = colour & 31;
	s32 g = (colour >> 5) & 31;
	s32 b = (colour >> 10) & 31;
	s32 gray = (r * Q_8_8(0.3) + g * Q_8_8(0.59) + b * Q_8_8(0.1133)) >> 8;
	return RGB2(gray, gray, gray);
}

static u16 LightUpTriggerSymbol(u16 clra)
{
	u16 clrb = 0x7FFF;

	u32 currentAlpha  = 20;

	const u32 rbmask= ((0x1F)|(0x1F << 10)), gmask= 0x1F << 5;
	const u32 rbhalf= 0x4010, ghalf= 0x0200;

	// Red and blue
	u32 parta = clra & rbmask;
	u32 partb = clrb & rbmask;
	u32 part = (partb-parta) * (32 - ((currentAlpha < 0x100) ? currentAlpha : currentAlpha >> 12)) + parta*32 + rbhalf;
	u16 clr = (part >> 5) & rbmask;

	// Green
	parta = clra & gmask;
	partb = clrb & gmask;
	part = (partb-parta) * (32 - ((currentAlpha < 0x100) ? currentAlpha : currentAlpha >> 12)) + parta*32 + ghalf;
	clr |= (part >> 5) & gmask;

	return clr;
}

#define TRIGGER_BANK self->data[4]
#define PALETTE_STATE self->data[1]
#define TAG self->template->tileTag
#define PAL_TAG self->template->paletteTag

static void SpriteCB_MegaTrigger(struct Sprite* self)
{
	if (TAG == GFX_TAG_MEGA_TRIGGER)
	{
		if (!CanMegaEvolve(TRIGGER_BANK, FALSE) || gBattleSpritesDataPtr->bankData[TRIGGER_BANK].transformSpecies)
			self->invisible = TRUE;
		else
			self->invisible = FALSE;
	}
	else //Ultra Burst
	{
		if (!CanMegaEvolve(TRIGGER_BANK, TRUE) || gBattleSpritesDataPtr->bankData[TRIGGER_BANK].transformSpecies)
			self->invisible = TRUE;
		else
			self->invisible = FALSE;
	}

	s16 xShift = 0;
	if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
	{
		if (GetBattlerPosition(TRIGGER_BANK) == B_POSITION_PLAYER_LEFT)
			xShift = -48; //X Pos = 16
		else
			xShift = 10; //X Pos = 74
	}

	self->pos1.x = 64 + (32 / 2);
	self->pos1.y = 80 + (32 / 2);
	self->pos2.x = xShift;
	self->pos2.y = self->data[3];

	if (gBattlerControllerFuncs[TRIGGER_BANK] == (void*) (0x0802EA10 | 1) //Old HandleInputChooseMove
	||  gBattlerControllerFuncs[TRIGGER_BANK] == HandleInputChooseMove
	|| gBattlerControllerFuncs[TRIGGER_BANK] == HandleMoveSwitching)
	{
		if (self->data[3] > 0)
			self->data[3] -= 2;
		else
			self->data[3] = 0;
	}

	//Mega Trigger should recede and destroy itself as long as the game isn't
	//running one of the two mentioned functions.
	else if (gBattlerControllerFuncs[TRIGGER_BANK] != (void*) (0x08032C90 | 1)  //PlayerHandleChooseMove
		  && gBattlerControllerFuncs[TRIGGER_BANK] != (void*) (0x08032C4C | 1)) //HandleChooseMoveAfterDma3
	{
		if (self->data[3] < 32)
			self->data[3] += 2;
		else
		{
			DestroyMegaTriggers();
			return;
		}
	}

	const struct Evolution* evo;
	struct ChooseMoveStruct* moveInfo = (struct ChooseMoveStruct*) (&gBattleBufferA[TRIGGER_BANK][4]);
	if (TAG == GFX_TAG_MEGA_TRIGGER)
	{
		evo = CanMegaEvolve(TRIGGER_BANK, FALSE);
		if (evo->unknown != MEGA_VARIANT_ULTRA_BURST)
		{
			if (!moveInfo->canMegaEvolve)
				PALETTE_STATE = MegaTriggerGrayscale;
			else
			{
				if (gNewBS->megaData.chosen[TRIGGER_BANK])
					PALETTE_STATE = MegaTriggerLightUp;
				else
					PALETTE_STATE = MegaTriggerNormalColour;
			}
		}
	}
	else //TAG == GFX_TAG_ULTRA_TRIGGER
	{
		evo = CanMegaEvolve(TRIGGER_BANK, TRUE);
		if (evo->unknown == MEGA_VARIANT_ULTRA_BURST)
		{
			if (!moveInfo->canMegaEvolve)
				PALETTE_STATE = MegaTriggerGrayscale;
			else
			{
				if (gNewBS->ultraData.chosen[TRIGGER_BANK])
					PALETTE_STATE = MegaTriggerLightUp;
				else
					PALETTE_STATE = MegaTriggerNormalColour;
			}
		}
	}

	// Only change the palette if the state has changed
	if (PALETTE_STATE != self->data[2])
	{
		u16* pal = &gPlttBufferFaded2[IndexOfSpritePaletteTag(PAL_TAG) * 16];
		u8 i;

		for(i = 1; i < 16; i++)
		{
			if (IsIgnoredTriggerColour(Mega_TriggerPal[i])) continue;

			switch(PALETTE_STATE) {
				case MegaTriggerLightUp:
					pal[i] = LightUpTriggerSymbol(Mega_TriggerPal[i]);
					break;
				case MegaTriggerNormalColour:
					pal[i] = Mega_TriggerPal[i];
					break;
				case MegaTriggerGrayscale:
					pal[i] = ConvertColorToGrayscale(Mega_TriggerPal[i]);
					break;
			}
		}

		self->data[2] = PALETTE_STATE;
	}
}

#define INDICATOR_BANK self->data[0]

static void SpriteCB_MegaIndicator(struct Sprite* self)
{
	switch(TAG) {
		case GFX_TAG_ALPHA_INDICATOR:
			if (!IsBluePrimal(INDICATOR_BANK))
			{
				self->invisible = TRUE;
				return;
			}
			break;

		case GFX_TAG_OMEGA_INDICATOR:
			if (!IsRedPrimal(INDICATOR_BANK))
			{
				self->invisible = TRUE;
				return;
			}
			break;

		case GFX_TAG_ULTRA_INDICATOR:
			if (!IsUltraNecrozma(INDICATOR_BANK))
			{
				self->invisible = TRUE;
				return;
			}
			break;

		case GFX_TAG_DYNAMAX_INDICATOR:
			if (gNewBS == NULL || !IsDynamaxed(INDICATOR_BANK))
			{
				self->invisible = TRUE;
				return;
			}
			break;

		default: //GFX_TAG_MEGA_INDICATOR
			if (!IsMega(INDICATOR_BANK))
			{
				self->invisible = TRUE;
				return;
			}
	}

	struct Sprite* healthbox = GetHealthboxSprite(INDICATOR_BANK);

	if (healthbox->invisible)
	{
		self->invisible = TRUE;
		return;
	}
	else
	{
		self->invisible = FALSE;
	}

	u8 y = healthbox->oam.y;
	s8 x = healthbox->oam.x;

	// Mirror healthbox priority
	self->oam.priority = healthbox->oam.priority;

	if (y)
	{
		self->invisible = FALSE;

		// Figure out the X position for the indicator - it differs depending on
		// the battle type and the side the healthbox represents.
		s16 shift = 64; // Halfway point for OAM

		if (SIDE(INDICATOR_BANK) == B_SIDE_OPPONENT)
			shift += 18;
		else
			shift += 26;

		// Convert the level to a string to get how long it is
		u8 buf[10];
		u8 stringlen = ConvertIntToDecimalStringN(buf, GetBankPartyData(INDICATOR_BANK)->level, 0, 3) - buf;

		// The x position depends on the X origin of the healthbox as well as
		// the string length
		if (SIDE(INDICATOR_BANK) == B_SIDE_OPPONENT)
			self->pos1.x = x + shift - 5 * stringlen;
		else
			self->pos1.x = (u8) x + shift - 5 * stringlen;

		u8 pingid = gBattleSpritesDataPtr->healthBoxesData[INDICATOR_BANK].healthboxBounceSpriteId;
		struct Sprite* ping = &gSprites[pingid];

		bool8 sineActive = (gBattleSpritesDataPtr->healthBoxesData[INDICATOR_BANK].healthboxIsBouncing);

		struct Sprite* shaker = &gSprites[gShakerData[1]];
		u8 hbid = gHealthboxSpriteIds[INDICATOR_BANK];

		if (sineActive)
		{
			self->pos1.y = healthbox->pos1.y - 4;
			self->pos2.y = Sine(ping->data[0], ping->data[2]);
			return;
		}
		else if (shaker->data[1] == hbid && gShakerData[0])
		{
			self->pos2.y = (shaker->data[2] & 1) ? -1 : 1;
			return;
		}

		// Fix indicator position
		self->pos1.y = healthbox->pos1.y - 5;
		self->pos2.x = 0;
		self->pos2.y = 0;
	}
	else
		self->invisible = TRUE;
}

static void SpriteCB_ZTrigger(struct Sprite* self)
{
	s16 xShift = 0;
	if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
	{
		if (GetBattlerPosition(TRIGGER_BANK) == B_POSITION_PLAYER_LEFT)
			xShift = -48; //X Pos = 16
		else
			xShift = 10; //X Pos = 74
	}

	self->pos1.x = 64 + (32 / 2);
	self->pos1.y = 80 + (32 / 2);
	self->pos2.x = xShift;
	self->pos2.y = self->data[3];

	if (gBattlerControllerFuncs[TRIGGER_BANK] == (void*) (0x0802EA10 | 1) //Old HandleInputChooseMove
	||  gBattlerControllerFuncs[TRIGGER_BANK] == HandleInputChooseMove
	|| gBattlerControllerFuncs[TRIGGER_BANK] == HandleMoveSwitching)
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
			if (self->data[3] < 32)
				self->data[3] += 2;
			else
				self->data[3] = 32;
		}
	}

	//Z-Move Trigger should recede and destroy itself as long as the game isn't
	//running one of the two mentioned functions.
	else if (gBattlerControllerFuncs[TRIGGER_BANK] != (void*) (0x08032C90 | 1)  //PlayerHandleChooseMove
		  && gBattlerControllerFuncs[TRIGGER_BANK] != (void*) (0x08032C4C | 1)) //HandleChooseMoveAfterDma3
	{
		if (self->data[3] < 32)
			self->data[3] += 2;
		else
		{
			DestroyZTrigger();
			return;
		}
	}

	if (gNewBS->zMoveData.viewing)
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
					pal[i] = LightUpTriggerSymbol(Z_Move_TriggerPal[i]);
					break;
				case MegaTriggerNormalColour:
					pal[i] = Z_Move_TriggerPal[i];
					break;
			}
		}

		self->data[2] = PALETTE_STATE;
	}
}

static void SpriteCB_DynamaxTrigger(struct Sprite* self)
{
	s16 xShift = 0;
	if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
	{
		if (GetBattlerPosition(TRIGGER_BANK) == B_POSITION_PLAYER_LEFT)
			xShift = -48; //X Pos = 16
		else
			xShift = 10; //X Pos = 74
	}

	self->pos1.x = 64 + (32 / 2);
	self->pos1.y = 80 + (32 / 2);
	self->pos2.x = xShift;
	self->pos2.y = self->data[3];

	if (gBattlerControllerFuncs[TRIGGER_BANK] == (void*) (0x0802EA10 | 1) //Old HandleInputChooseMove
	||  gBattlerControllerFuncs[TRIGGER_BANK] == HandleInputChooseMove
	|| gBattlerControllerFuncs[TRIGGER_BANK] == HandleMoveSwitching)
	{
		struct ChooseMoveStruct* moveInfo = (struct ChooseMoveStruct*) (&gBattleBufferA[TRIGGER_BANK][4]);
		if (!moveInfo->dynamaxed && !moveInfo->canMegaEvolve && moveInfo->possibleMaxMoves[gMoveSelectionCursor[TRIGGER_BANK]]) //Viewing move that can become Max Move
		{
			if (self->data[3] > 0)
				self->data[3] -= 2;
			else
				self->data[3] = 0;
		}
		else
		{
			if (self->data[3] < 32)
				self->data[3] += 2;
			else
				self->data[3] = 32;
		}
	}

	//Dynamax Trigger should recede and destroy itself as long as the game isn't
	//running one of the two mentioned functions.
	else if (gBattlerControllerFuncs[TRIGGER_BANK] != (void*) (0x08032C90 | 1)  //PlayerHandleChooseMove
		  && gBattlerControllerFuncs[TRIGGER_BANK] != (void*) (0x08032C4C | 1)) //HandleChooseMoveAfterDma3
	{
		if (self->data[3] < 32)
			self->data[3] += 2;
		else
		{
			DestroyDynamaxTrigger();
			return;
		}
	}

	if (gNewBS->dynamaxData.viewing)
		PALETTE_STATE = MegaTriggerLightUp;
	else
		PALETTE_STATE = MegaTriggerNormalColour;

	// Only change the palette if the state has changed
	if (PALETTE_STATE != self->data[2])
	{
		u16* pal = &gPlttBufferFaded2[IndexOfSpritePaletteTag(GFX_TAG_DYNAMAX_TRIGGER) * 16];
		u8 i;

		for(i = 1; i < 16; i++)
		{
			if (IsIgnoredTriggerColour(Dynamax_TriggerPal[i])) continue;

			switch(PALETTE_STATE) {
				case MegaTriggerLightUp:
					pal[i] = LightUpTriggerSymbol(Dynamax_TriggerPal[i]);
					break;
				case MegaTriggerNormalColour:
					pal[i] = Dynamax_TriggerPal[i];
					break;
			}
		}

		self->data[2] = PALETTE_STATE;
	}
}

static void SpriteCB_RaidShield(struct Sprite* sprite)
{
	u8 bank = sprite->data[0];
	s16 offset = sprite->data[1];

	//Deal with bouncing player healthbox
	struct Sprite* healthbox = &gSprites[gHealthboxSpriteIds[bank]];
	sprite->pos1.x = healthbox->pos1.x + 50;
	sprite->pos1.y = healthbox->pos1.y + 12;

	sprite->pos2.x = offset;
	sprite->pos2.y = healthbox->pos2.y;
	sprite->subpriority = healthbox->subpriority;

	if (!healthbox->invisible)
		sprite->invisible = FALSE;
	else
		sprite->invisible = TRUE;
	
	if (sprite->affineAnimEnded && sprite->oam.affineMode == ST_OAM_AFFINE_DOUBLE)
	{
		//Remove the OAM matrix to save space in memory
		FreeSpriteOamMatrix(sprite);
		CalcCenterToCornerVec(sprite, sprite->oam.shape, sprite->oam.size, sprite->oam.affineMode);
	}
}


void LoadRaidShieldGfx(void)
{
	if (IndexOfSpriteTileTag(GFX_TAG_RAID_SHIELD) == 0xFF)
		LoadCompressedSpriteSheetUsingHeap(&sRaidShieldSpriteSheet);
}

void LoadMegaGraphics(u8 state)
{
	u8 spriteId;

	if (state == 2 || state == 0xFF)
	{
		unusedArg bool8 loadedMegaGfx = IndexOfSpriteTileTag(GFX_TAG_MEGA_INDICATOR) != 0xFF;
		unusedArg bool8 loadedAlphaGfx = IndexOfSpriteTileTag(GFX_TAG_ALPHA_INDICATOR) != 0xFF;
		unusedArg bool8 loadedOmegaGfx = IndexOfSpriteTileTag(GFX_TAG_OMEGA_INDICATOR) != 0xFF;
		unusedArg bool8 loadedUltraGfx = IndexOfSpriteTileTag(GFX_TAG_ULTRA_INDICATOR) != 0xFF;;
		unusedArg bool8 loadedDynamaxGfx = IndexOfSpriteTileTag(GFX_TAG_DYNAMAX_INDICATOR) != 0xFF;

		#if (defined MEGA_EVOLUTION_FEATURE || defined DYNAMAX_FEATURE)
		LoadSpritePalette(&sMegaIndicatorPalette);
		#endif

		// Create a Mega Indicator for every bank
		for (u8 bank = 0; bank < gBattlersCount; ++bank)
		{
			if (state == 0xFF)
				bank = gActiveBattler;

			#ifdef MEGA_EVOLUTION_FEATURE
			if (IsMega(bank))
			{
				if (!loadedMegaGfx)
				{
					LoadCompressedSpriteSheetUsingHeap(&sMegaIndicatorSpriteSheet);
					loadedMegaGfx = TRUE;
				}

				spriteId = CreateSprite(&sMegaIndicatorSpriteTemplate, -10, 0, 1);
				gSprites[spriteId].data[0] = bank;
				gNewBS->megaIndicatorObjIds[bank] = spriteId + 1;
			}
			else if (IsBluePrimal(bank))
			{
				if (!loadedAlphaGfx)
				{
					LoadCompressedSpriteSheetUsingHeap(&sAlphaIndicatorSpriteSheet);
					loadedAlphaGfx = TRUE;
				}

				spriteId = CreateSprite(&sAlphaIndicatorSpriteTemplate, -10, 0, 1);
				gSprites[spriteId].data[0] = bank;
				gNewBS->megaIndicatorObjIds[bank] = spriteId + 1;
			}
			else if (IsRedPrimal(bank))
			{
				if (!loadedOmegaGfx)
				{
					LoadCompressedSpriteSheetUsingHeap(&sOmegaIndicatorSpriteSheet);
					loadedOmegaGfx = TRUE;
				}

				spriteId = CreateSprite(&sOmegaIndicatorSpriteTemplate, -10, 0, 1);
				gSprites[spriteId].data[0] = bank;
				gNewBS->megaIndicatorObjIds[bank] = spriteId + 1;
			}
			else if (IsUltraNecrozma(bank))
			{
				if (!loadedUltraGfx)
				{
					LoadCompressedSpriteSheetUsingHeap(&sUltraIndicatorSpriteSheet);
					loadedUltraGfx = TRUE;
				}

				spriteId = CreateSprite(&sUltraIndicatorSpriteTemplate, -10, 0, 1);
				gSprites[spriteId].data[0] = bank;
				gNewBS->megaIndicatorObjIds[bank] = spriteId + 1;
			}
			else
			#endif
				if (IsDynamaxed(bank))
			{
				if (!loadedDynamaxGfx)
				{
					LoadCompressedSpriteSheetUsingHeap(&sDynamaxIndicatorSpriteSheet);
					loadedDynamaxGfx = TRUE;
				}

				spriteId = CreateSprite(&sDynamaxIndicatorSpriteTemplate, -10, 0, 1);
				gSprites[spriteId].data[0] = bank;
				gNewBS->megaIndicatorObjIds[bank] = spriteId + 1;
			}
			else
				gNewBS->megaIndicatorObjIds[bank] = 0;

			if (HasRaidShields(bank))
				CreateRaidShieldSprites();

			if (state == 0xFF)
				break; //Only need one iteration
		}
	}
}

void CreateMegaIndicatorAfterAnim(void)
{
	if (!gNewBS->megaIndicatorObjIds[gActiveBattler] && gActiveBattler < gBattlersCount)
	{
		LoadMegaGraphics(0xFF);
	}
}

//The Mega Graphics are usually loaded before, but this is placed here just in
//case the battle started against a Primal/Dynamax and their symbol wasn't loaded.
void TryLoadIndicatorForEachBank(void)
{
	for (gActiveBattler = 0; gActiveBattler < gBattlersCount; ++gActiveBattler)
		CreateMegaIndicatorAfterAnim();
}

void DestroyMegaIndicator(u8 bank)
{
	if (gNewBS->megaIndicatorObjIds[bank] > 0)
	{
		DestroySprite(&gSprites[gNewBS->megaIndicatorObjIds[bank] - 1]);
		gNewBS->megaIndicatorObjIds[bank] = 0;
	}
}

void TryLoadMegaTriggers(void)
{
	u8 spriteId;

	if (gBattleTypeFlags & (BATTLE_TYPE_SAFARI | BATTLE_TYPE_POKE_DUDE | BATTLE_TYPE_OLD_MAN))
		return;

	LoadSpritePalette(&sMegaTriggerPalette);
	LoadCompressedSpriteSheetUsingHeap(&sMegaTriggerSpriteSheet);
	LoadCompressedSpriteSheetUsingHeap(&sUltraTriggerSpriteSheet);

	spriteId = CreateSprite(&sMegaTriggerSpriteTemplate, 130, 90, 1);
	gSprites[spriteId].data[3] = 32;
	gSprites[spriteId].pos1.y = -32;
	gSprites[spriteId].data[4] = gActiveBattler;

	spriteId = CreateSprite(&sUltraTriggerSpriteTemplate, 130, 90, 1);
	gSprites[spriteId].data[3] = 32;
	gSprites[spriteId].pos1.y = -32;
	gSprites[spriteId].data[4] = gActiveBattler;
}

static void DestroyMegaTriggers(void)
{
	FreeSpritePaletteByTag(GFX_TAG_MEGA_TRIGGER);
	FreeSpriteTilesByTag(GFX_TAG_MEGA_TRIGGER);
	FreeSpritePaletteByTag(GFX_TAG_ULTRA_TRIGGER);
	FreeSpriteTilesByTag(GFX_TAG_ULTRA_TRIGGER);

	for (int i = 0; i < MAX_SPRITES; ++i)
	{
		if (gSprites[i].template->tileTag == GFX_TAG_MEGA_TRIGGER
		||  gSprites[i].template->tileTag == GFX_TAG_ULTRA_TRIGGER)
		{
			DestroySprite(&gSprites[i]);
		}
	}
}

void TryLoadZTrigger(void)
{
	u8 spriteId;

	if (gBattleTypeFlags & (BATTLE_TYPE_SAFARI | BATTLE_TYPE_POKE_DUDE | BATTLE_TYPE_OLD_MAN))
		return;

	LoadSpritePalette(&sZTriggerPalette);
	LoadCompressedSpriteSheetUsingHeap(&sZTriggerSpriteSheet);

	spriteId = CreateSprite(&sZTriggerSpriteTemplate, 130, 90, 1);
	gSprites[spriteId].data[3] = 32;
	gSprites[spriteId].pos1.y = -32;
	gSprites[spriteId].data[4] = gActiveBattler;
}

static void DestroyZTrigger(void)
{
	FreeSpritePaletteByTag(GFX_TAG_Z_TRIGGER);
	FreeSpriteTilesByTag(GFX_TAG_Z_TRIGGER);

	for (int i = 0; i < MAX_SPRITES; ++i)
	{
		if (gSprites[i].template->tileTag == GFX_TAG_Z_TRIGGER)
			DestroySprite(&gSprites[i]);
	}
}

void TryLoadDynamaxTrigger(void)
{
	u8 spriteId;

	if (gBattleTypeFlags & (BATTLE_TYPE_SAFARI | BATTLE_TYPE_POKE_DUDE | BATTLE_TYPE_OLD_MAN))
		return;

	if (!(gBattleTypeFlags & BATTLE_TYPE_DYNAMAX))
		return;

	LoadSpritePalette(&sDynamaxTriggerPalette);
	LoadCompressedSpriteSheetUsingHeap(&sDynamaxTriggerSpriteSheet);

	spriteId = CreateSprite(&sDynamaxTriggerSpriteTemplate, 130, 90, 1);
	gSprites[spriteId].data[3] = 32;
	gSprites[spriteId].pos1.y = -32;
	gSprites[spriteId].data[4] = gActiveBattler;
}

static void DestroyDynamaxTrigger(void)
{
	FreeSpritePaletteByTag(GFX_TAG_DYNAMAX_TRIGGER);
	FreeSpriteTilesByTag(GFX_TAG_DYNAMAX_TRIGGER);

	for (int i = 0; i < MAX_SPRITES; ++i)
	{
		if (gSprites[i].template->tileTag == GFX_TAG_DYNAMAX_TRIGGER)
			DestroySprite(&gSprites[i]);
	}
}

static void DestroyRaidShieldSpriteAndMatrix(struct Sprite *sprite)
{
	m4aMPlayStop(&gMPlayInfo_SE1);
	m4aMPlayStop(&gMPlayInfo_SE2);
	PlaySE(0xBF); //Screen break

	FreeSpriteOamMatrix(sprite);
	DestroySprite(sprite);
}

void DestroyRaidShieldSprite(void)
{
	u8 i;
	++gNewBS->dynamaxData.shieldsDestroyed;

	for (i = 0; i < MAX_NUM_RAID_SHIELDS; ++i)
	{
		if (gNewBS->dynamaxData.shieldSpriteIds[i] == 0)
			break; //No more sprites

		if (gNewBS->dynamaxData.shieldCount - i - gNewBS->dynamaxData.shieldsDestroyed <= 0)
		{
			struct Sprite* sprite = &gSprites[gNewBS->dynamaxData.shieldSpriteIds[i] - 1];
			gNewBS->dynamaxData.shieldSpriteIds[i] = 0;

			//Turn on affine anims again since they were turned off earlier
			sprite->oam.affineMode = ST_OAM_AFFINE_DOUBLE;
			CalcCenterToCornerVec(sprite, sprite->oam.shape, sprite->oam.size, sprite->oam.affineMode);
			InitSpriteAffineAnim(sprite);
			StartSpriteAffineAnim(sprite, 1);
			StoreSpriteCallbackInData6(sprite, DestroyRaidShieldSpriteAndMatrix);
			sprite->callback = RunStoredCallbackWhenAffineAnimEnds;
		}
	}
}

