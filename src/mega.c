#include "defines.h"
#include "defines_battle.h"
#include "../include/link.h"
#include "../include/constants/items.h"

#include "../include/new/helper_functions.h"
#include "../include/new/mega.h"

#define TRAINER_ITEM_COUNT 4

extern u8 BattleScript_Primal[];
extern u8 BattleScript_PrimalSub[];
extern u8 BattleScript_MegaEvolution[];
extern u8 BattleScript_MegaWish[];
extern u8 BattleScript_UltraBurst[];

const struct Evolution* CanMegaEvolve(u8 bank, bool8 CheckUBInstead);
u8* DoMegaEvolution(u8 bank);
u8* DoPrimalReversion(u8 bank, u8 caseId);
void MegaRevert(pokemon_t* mon);
void TryRevertMega(pokemon_t* mon);
bool8 IsItemKeystone(u16 item);
item_t FindTrainerKeystone(u16 trainerId);
item_t FindPlayerKeystone(void);
item_t FindBankKeystone(u8 bank);
bool8 MegaEvolutionEnabled(u8 bank);
bool8 BankMegaEvolved(u8 bank, bool8 checkUB);
bool8 IsMega(u8 bank);
bool8 IsBluePrimal(u8 bank);
bool8 IsRedPrimal(u8 bank);
bool8 IsUltraNecrozma(u8 bank);
bool8 HasMegaSymbol(u8 bank);
u8* GetTrainerName(u8 bank);

item_t KeystoneTable[] = 
{
    ITEM_MEGA_RING,
};

const struct Evolution* CanMegaEvolve(u8 bank, bool8 CheckUBInstead) {
	#ifndef MEGA_EVOLUTION_FEATURE
		return NULL;
	#else
	
	if (!CheckUBInstead && !MegaEvolutionEnabled(bank)) //Ultra Burst doesn't need Mega Ring
		return NULL;
	
	pokemon_t* mon = GetBankPartyData(bank);
	const struct Evolution* evolutions = gEvolutionTable[mon->species];
	int i, j;
	
	for (i = 0; i < EVOS_PER_MON; ++i) {
		if (evolutions[i].method == EVO_MEGA) {
			// Ignore reversion information
			if (evolutions[i].param == 0) continue;
			
			// Check for held item
			if ((!CheckUBInstead && evolutions[i].unknown == MEGA_VARIANT_STANDARD) || (CheckUBInstead && evolutions[i].unknown == MEGA_VARIANT_ULTRA_BURST)) {
				if (evolutions[i].param == mon->item)
					return &evolutions[i];	
			} 
			else if (evolutions[i].unknown == MEGA_VARIANT_WISH) {
			// Check learned moves
				for (j = 0; j < 4; ++j) {
					if (evolutions[i].param == mon->moves[j])
						return &evolutions[i];
				}
			}
		}
	}
	
	return NULL;
	#endif
}

u8* DoMegaEvolution(u8 bank) {
	const struct Evolution* evolutions = CanMegaEvolve(bank, FALSE);
	
	if (evolutions == NULL) //Check Ultra Burst if no Mega
		evolutions = CanMegaEvolve(bank, TRUE);
	
	if (evolutions != NULL) {
		u16 species = gBattleMons[bank].species;
		DoFormChange(bank, evolutions->targetSpecies, TRUE, TRUE);
		gBattleMons[bank].ability = GetPartyAbility(GetBankPartyData(bank));
		gBattleScripting->bank = bank;
		gLastUsedItem = gBattleMons[bank].item;
		
		//FD 00's FD 16 FE is reacting to FD 04's FD 01!
		PREPARE_SPECIES_BUFFER(gBattleTextBuff1, species);
		PREPARE_ITEM_BUFFER(gBattleTextBuff2, FindBankKeystone(bank));
		
		if (evolutions->unknown == MEGA_VARIANT_WISH)
			return BattleScript_MegaWish;
		else if (evolutions->unknown == MEGA_VARIANT_ULTRA_BURST)
			return BattleScript_UltraBurst;
		else
			return BattleScript_MegaEvolution;
	}
	else
		return NULL;
}


u8* DoPrimalReversion(u8 bank, u8 caseId) {
	pokemon_t* mon = GetBankPartyData(bank);
	const struct Evolution* evolutions = gEvolutionTable[mon->species];
	u16 item = mon->item;

	for (u8 i = 0; i < EVOS_PER_MON; ++i) {
		if (evolutions[i].method == EVO_MEGA && evolutions[i].unknown == MEGA_VARIANT_PRIMAL && evolutions[i].param == item) {
			DoFormChange(bank, evolutions[i].targetSpecies, TRUE, TRUE);
			gBattleMons[bank].ability = GetPartyAbility(mon);
			
			switch (caseId) {
				case 0:
					return BattleScript_Primal;
				default:
					return BattleScript_PrimalSub;
			}
		}
	}
	return NULL;
}

//In theory, this function will do nothing as the regular forms revert should
//should take care of the reversion. This is to prevent bugs if the player
//gives themselves a Mega or Primal to start the battle.
void MegaRevert(pokemon_t* party) 
{					  
	int i;			  
	
	for (i = 0; i < PARTY_SIZE; ++i) {
		TryRevertMega(&party[i]);
	}
}

void TryRevertMega(pokemon_t* mon)
{
	const struct Evolution* evolutions = gEvolutionTable[mon->species];

	for (u8 i = 0; i < EVOS_PER_MON; ++i) 
	{
		if (evolutions[i].method == EVO_MEGA && evolutions[i].param == 0)
		{
			mon->species = evolutions[i].targetSpecies;
			CalculateMonStats(mon);
		}
	}
}

bool8 IsItemKeystone(u16 item)
{
    for (u8 i = 0; i < sizeof(KeystoneTable) / sizeof(item_t); ++i) {
        if (item == KeystoneTable[i]) {
            return TRUE;
        }
    }
    return FALSE;
}

item_t FindTrainerKeystone(u16 trainerId)
{
	if (gBattleTypeFlags & (BATTLE_TYPE_FRONTIER | BATTLE_TYPE_LINK))
		return ITEM_MEGA_RING;
		
    for (u8 i = 0; i < TRAINER_ITEM_COUNT; ++i) {
        if (IsItemKeystone(gTrainers[trainerId].items[i])) {
            return gTrainers[trainerId].items[i];
        }
    }

    return ITEM_NONE;
}


item_t FindPlayerKeystone(void)
{
    for (u8 i = 0; i < sizeof(KeystoneTable) / sizeof(item_t); ++i) {
        if (CheckBagHasItem(KeystoneTable[i], 1)) {
            return KeystoneTable[i];
        }
    }

    return ITEM_NONE;
}

item_t FindBankKeystone(u8 bank)
{
	if (gBattleTypeFlags & BATTLE_TYPE_LINK)
		return ITEM_MEGA_RING; //You can always Mega Evolve in a link battle
		
	#ifdef DEBUG_MEGA
		if (bank + 1)
			return ITEM_MEGA_RING;
	#endif
		
    if (SIDE(bank) == SIDE_OPPONENT) {
		if (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS) {
			if (GetBattlerPosition(bank) == B_POSITION_OPPONENT_LEFT)
				return FindTrainerKeystone(gTrainerBattleOpponent_A);
			else
				return FindTrainerKeystone(SECOND_OPPONENT);
		}
		else
			return FindTrainerKeystone(gTrainerBattleOpponent_A);
	}
	else {
		if (gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER) {
			if (GetBattlerPosition(bank) == B_POSITION_PLAYER_RIGHT)
				return FindTrainerKeystone(VarGet(PARTNER_VAR));
			else
				return FindPlayerKeystone();
		}
		else
			return FindPlayerKeystone();
	}
}

bool8 MegaEvolutionEnabled(u8 bank) {
	if (gBattleTypeFlags & (BATTLE_TYPE_LINK 
						  | BATTLE_TYPE_TRAINER_TOWER 
						  | BATTLE_TYPE_FRONTIER
						  | BATTLE_TYPE_EREADER_TRAINER))
	{
		return TRUE;
	}

	if (FindBankKeystone(bank) == ITEM_NONE) {
	#ifdef DEBUG_MEGA
		return TRUE;
	#else
		return FALSE;
	#endif
	}
	else
		return TRUE;
}

bool8 BankMegaEvolved(u8 bank, bool8 checkUB) {
	if ((SIDE(bank) == B_SIDE_PLAYER && gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER)
	||  (SIDE(bank) == B_SIDE_OPPONENT && gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS))
	{
		if (checkUB) {
			if (gNewBS->UltraData->done[bank])
				return TRUE;
			else
				return FALSE;
		}
		else {	
			if (gNewBS->MegaData->done[bank])
				return TRUE;
			else
				return FALSE;
		}
	}
	
	if (checkUB) {
		if (gNewBS->UltraData->done[bank] || gNewBS->UltraData->done[PARTNER(bank)])
			return TRUE;
	}
	else {
		if (gNewBS->MegaData->done[bank] || (gNewBS->MegaData->done[PARTNER(bank)]))
			return TRUE;
	}
	
	return FALSE;
}

bool8 IsMega(u8 bank)
{
	pokemon_t* mon = GetBankPartyData(bank);
	const struct Evolution* evolutions = gEvolutionTable[mon->species];

	for (u8 i = 0; i < EVOS_PER_MON; ++i) 
	{
		if (evolutions[i].method == EVO_MEGA && evolutions[i].unknown == MEGA_VARIANT_STANDARD && evolutions[i].param == 0)
			return TRUE;
	}
	
	return FALSE;
}

//No better way to check for these next two
bool8 IsBluePrimal(u8 bank)
{
	u16 species = GetBankPartyData(bank)->species;
	return species == PKMN_KYOGRE_PRIMAL;
}

bool8 IsRedPrimal(u8 bank)
{
	u16 species = GetBankPartyData(bank)->species;
	return species == PKMN_GROUDON_PRIMAL;
}

bool8 IsUltraNecrozma(u8 bank)
{
	pokemon_t* mon = GetBankPartyData(bank);
	const struct Evolution* evolutions = gEvolutionTable[mon->species];

	for (u8 i = 0; i < EVOS_PER_MON; ++i) 
	{
		if (evolutions[i].method == EVO_MEGA && evolutions[i].unknown == MEGA_VARIANT_ULTRA_BURST && evolutions[i].param == 0)
			return TRUE;
	}
	
	return FALSE;
}

bool8 HasMegaSymbol(u8 bank)
{
	return IsMega(bank) || IsBluePrimal(bank) || IsRedPrimal(bank) || IsUltraNecrozma(bank);
}

u8* GetTrainerName(u8 bank) {
	u16 trainerId = 0xFFFF;
	u8 multiplayerId = GetMultiplayerId();
	
	u16 linkPartner = GetBattlerMultiplayerId(BATTLE_PARTNER(gLinkPlayers[multiplayerId].id));
	u16 linkOpponent1 = GetBattlerMultiplayerId(BATTLE_OPPOSITE(gLinkPlayers[multiplayerId].id));
	u16 linkOpponent2 = GetBattlerMultiplayerId(BATTLE_PARTNER(BATTLE_OPPOSITE(gLinkPlayers[multiplayerId].id)));
	
	switch (GetBattlerPosition(bank)) {
		case B_POSITION_OPPONENT_LEFT:
			if (gBattleTypeFlags & BATTLE_TYPE_LINK)
				trainerId = linkOpponent1;
			else
				trainerId = gTrainerBattleOpponent_A;
			break;
		
		case B_POSITION_PLAYER_RIGHT:
			if (gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER)
				trainerId = VarGet(PARTNER_VAR);
			else if (gBattleTypeFlags & BATTLE_TYPE_LINK && gBattleTypeFlags & BATTLE_TYPE_MULTI)
				trainerId = linkPartner;
			break;
		
		case B_POSITION_OPPONENT_RIGHT:
			if (gBattleTypeFlags & BATTLE_TYPE_LINK && gBattleTypeFlags & BATTLE_TYPE_MULTI)
				trainerId = linkOpponent2;
			else if (gBattleTypeFlags & BATTLE_TYPE_LINK)
				trainerId = linkOpponent1;
			else if (!(gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS))
				trainerId = gTrainerBattleOpponent_A;
			else
				trainerId = SECOND_OPPONENT;
			break;
	}
	
	if (trainerId == 0xFFFF) {
		if (gBattleTypeFlags & BATTLE_TYPE_LINK)
			return gLinkPlayers[multiplayerId].name;
		else
			return gSaveBlock2->playerName;
	}
	
	else if (gBattleTypeFlags & BATTLE_TYPE_LINK)
		return gLinkPlayers[trainerId].name;
		
	else {
	#ifdef UNBOUND
		u8 class = gTrainers[trainerId].trainerClass;
		if (class == 0x51 || class == 0x59)
			return GetExpandedPlaceholder(ExpandPlaceholder_RivalName);
		else
			return gTrainers[trainerId].trainerName;
	#elif defined OVERWRITE_RIVAL
		u8 class = gTrainers[trainerId].trainerClass;
		if (class == 0x51 || class == 0x59 || class == 0x5A)
			return GetExpandedPlaceholder(ExpandPlaceholder_RivalName);
		else
			return gTrainers[trainerId].trainerName;
	#else
		return gTrainers[trainerId].trainerName;
	#endif
	}
}

void MegaRetrieveData(void) {
	gNewBS->MegaData->chosen[gActiveBattler] |= gBattleBufferB[gActiveBattler][4];
	gNewBS->UltraData->chosen[gActiveBattler] |= gBattleBufferB[gActiveBattler][5];
	gNewBS->ZMoveData->toBeUsed[gActiveBattler] |= gBattleBufferB[gActiveBattler][6];
}

//Graphics//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern u8 Mega_IndicatorTiles[];
extern u8 Alpha_IndicatorTiles[];
extern u8 Omega_IndicatorTiles[];
extern u8 Ultra_IndicatorTiles[];
extern u16 Mega_IndicatorPal[];
extern u8 Mega_TriggerTiles[];
extern u8 Ultra_TriggerTiles[];
extern u16 Mega_TriggerPal[];
extern u16 Ultra_TriggerPal[];

bool8 IsIgnoredTriggerColour(u16 colour);
u16 ConvertColorToGrayscale(u16 colour);
u16 LightUpMegaSymbol(u16 clra);
void MegaTriggerCallback(struct Sprite* self);
void MegaIndicatorCallback(struct Sprite* self);
void LoadMegaGraphics(u8 state);
void CreateMegaIndicatorAfterAnim(void);
void TryLoadIndicatorForEachBank(void);
void TryLoadMegaTriggers(void);
void DestroyMegaTriggers(void);

const struct CompressedSpriteSheet MegaIndicatorSpriteSheet = {Mega_IndicatorTiles, (8 * 8) / 2, GFX_TAG_MEGA_INDICATOR};
const struct CompressedSpriteSheet AlphaIndicatorSpriteSheet = {Alpha_IndicatorTiles, (8 * 8) / 2, GFX_TAG_ALPHA_INDICATOR};
const struct CompressedSpriteSheet OmegaIndicatorSpriteSheet = {Omega_IndicatorTiles, (8 * 8) / 2, GFX_TAG_OMEGA_INDICATOR};
const struct CompressedSpriteSheet UltraIndicatorSpriteSheet = {Ultra_IndicatorTiles, (8 * 8) / 2, GFX_TAG_ULTRA_INDICATOR};
const struct SpritePalette MegaIndicatorPalette = {Mega_IndicatorPal, GFX_TAG_MEGA_INDICATOR};

const struct CompressedSpriteSheet MegaTriggerSpriteSheet = {Mega_TriggerTiles, (32 * 32) / 2, GFX_TAG_MEGA_TRIGGER};
const struct CompressedSpriteSheet UltraTriggerSpriteSheet = {Ultra_TriggerTiles, (32 * 32) / 2, GFX_TAG_ULTRA_TRIGGER};
const struct SpritePalette MegaTriggerPalette = {Mega_TriggerPal, GFX_TAG_MEGA_TRIGGER};
const struct SpritePalette UltraTriggerPalette = {Ultra_TriggerPal, GFX_TAG_ULTRA_TRIGGER};

const struct OamData MegaIndicatorOAM = {0};
const u16 MegaTriggerOAM[4] = {0, 0x8000, 0x800, 0};

const struct SpriteTemplate MegaIndicatorTemplate =
{
    .tileTag = GFX_TAG_MEGA_INDICATOR,
    .paletteTag = GFX_TAG_MEGA_INDICATOR,
    .oam = &MegaIndicatorOAM,
    .anims = (const union AnimCmd* const*) 0x08231CF0,
    .images = NULL,
    .affineAnims = (const union AffineAnimCmd* const*) 0x08231CFC,
    .callback = MegaIndicatorCallback,
};

const struct SpriteTemplate AlphaIndicatorTemplate =
{
    .tileTag = GFX_TAG_ALPHA_INDICATOR,
    .paletteTag = GFX_TAG_MEGA_INDICATOR,
    .oam = &MegaIndicatorOAM,
    .anims = (const union AnimCmd* const*) 0x08231CF0,
    .images = NULL,
    .affineAnims = (const union AffineAnimCmd* const*) 0x08231CFC,
    .callback = MegaIndicatorCallback,
};

const struct SpriteTemplate OmegaIndicatorTemplate =
{
    .tileTag = GFX_TAG_OMEGA_INDICATOR,
    .paletteTag = GFX_TAG_MEGA_INDICATOR,
    .oam = &MegaIndicatorOAM,
    .anims = (const union AnimCmd* const*) 0x08231CF0,
    .images = NULL,
    .affineAnims = (const union AffineAnimCmd* const*) 0x08231CFC,
    .callback = MegaIndicatorCallback,
};

const struct SpriteTemplate UltraIndicatorTemplate =
{
    .tileTag = GFX_TAG_ULTRA_INDICATOR,
    .paletteTag = GFX_TAG_MEGA_INDICATOR,
    .oam = &MegaIndicatorOAM,
    .anims = (const union AnimCmd* const*) 0x08231CF0,
    .images = NULL,
    .affineAnims = (const union AffineAnimCmd* const*) 0x08231CFC,
    .callback = MegaIndicatorCallback,
};

const struct SpriteTemplate MegaTriggerTemplate =
{
    .tileTag = GFX_TAG_MEGA_TRIGGER,
    .paletteTag = GFX_TAG_MEGA_TRIGGER,
    .oam = (const struct OamData*) &MegaTriggerOAM,
    .anims = (const union AnimCmd* const*) 0x08231CF0,
    .images = NULL,
    .affineAnims = (const union AffineAnimCmd* const*) 0x08231CFC,
    .callback = MegaTriggerCallback,
};

const struct SpriteTemplate UltraTriggerTemplate =
{
    .tileTag = GFX_TAG_ULTRA_TRIGGER,
    .paletteTag = GFX_TAG_ULTRA_TRIGGER,
    .oam = (const struct OamData*) &MegaTriggerOAM,
    .anims = (const union AnimCmd* const*) 0x08231CF0,
    .images = NULL,
    .affineAnims = (const union AffineAnimCmd* const*) 0x08231CFC,
    .callback = MegaTriggerCallback,
};

/* Declare the colours the trigger button doesn't light up */
const u16 IgnoredColours[] = 
{
  RGB(7, 10, 8), 
  RGB(15, 18, 16), 
  RGB(10, 13, 12), 
  RGB(4, 7, 0),
  RGB(4, 4, 0),
  RGB(0, 0, 0),
};

/* Easy match function */
bool8 IsIgnoredTriggerColour(u16 colour) 
{
	for (u32 i = 0; i < ARRAY_COUNT(IgnoredColours); ++i) 
	{
		if (IgnoredColours[i] == colour) 
			return TRUE;
	}
  
	return FALSE;
}

struct Sprite* GetHealthboxObjId(u8 bank) 
{
	return &gSprites[gHealthboxIDs[bank]];
}

u16 ConvertColorToGrayscale(u16 colour) 
{
	s32 r = colour & 31;
	s32 g = (colour >> 5) & 31;
	s32 b = (colour >> 10) & 31;
	s32 gray = (r * Q_8_8(0.3) + g * Q_8_8(0.59) + b * Q_8_8(0.1133)) >> 8;
	return RGB2(gray, gray, gray);
}

u16 LightUpMegaSymbol(u16 clra) 
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

void MegaTriggerCallback(struct Sprite* self) 
{		
	if (TAG == GFX_TAG_MEGA_TRIGGER) 
	{
		if (!CanMegaEvolve(TRIGGER_BANK, FALSE))
		{
			self->invisible = TRUE;
		}
		else
			self->invisible = FALSE;
	}
	else //Ultra Burst
	{
		if (!CanMegaEvolve(TRIGGER_BANK, TRUE))
			self->invisible = TRUE;
		else
			self->invisible = FALSE;
	}
		
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
	
	if (gBattleBankFunc[TRIGGER_BANK] == (0x0802EA10 | 1)) //HandleInputChooseMove
	{
		if (self->data[3] > 0)
			self->data[3] -= 2;
		else
			self->data[3] = 0;  
	}
	
	//Mega Trigger should recede and destroy itself as long as the game isn't
	//running one of the two mentioned functions.
	else if (gBattleBankFunc[TRIGGER_BANK] != (0x08032C90 | 1)  //PlayerHandleChooseMove
	      && gBattleBankFunc[TRIGGER_BANK] != (0x08032C4C | 1)) //HandleChooseMoveAfterDma3
	{
		if (self->data[3] < 16)
			self->data[3] += 2;
		else 
		{
			self->pos1.x = -32;
			DestroyMegaTriggers();
			return;
		}
	}
	
  	const struct Evolution* evo;
	if (TAG == GFX_TAG_MEGA_TRIGGER)
	{
		evo = CanMegaEvolve(TRIGGER_BANK, FALSE);
		if (evo->unknown != MEGA_VARIANT_ULTRA_BURST) 
		{
			if (gNewBS->MegaData->done[TRIGGER_BANK])
				PALETTE_STATE = MegaTriggerGrayscale;
			else
			{
				if (gNewBS->MegaData->chosen[TRIGGER_BANK])
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
			if (gNewBS->UltraData->done[TRIGGER_BANK])
				PALETTE_STATE = MegaTriggerGrayscale;
			else 
			{
				if (gNewBS->UltraData->chosen[TRIGGER_BANK])
					PALETTE_STATE = MegaTriggerLightUp;
				else
					PALETTE_STATE = MegaTriggerNormalColour;
			}
		}
	}
	
	// Only change the palette if the state has changed
	if (PALETTE_STATE != self->data[2]) 
	{
		u16* pal = &gPlttBufferFaded2[IndexOfSpritePaletteTag(TAG) * 16];
		u8 i;
		
		for(i = 1; i < 16; i++) 
		{
			if (IsIgnoredTriggerColour(Mega_TriggerPal[i])) continue;
	
			switch(PALETTE_STATE) {
				case MegaTriggerLightUp:
					pal[i] = LightUpMegaSymbol(Mega_TriggerPal[i]);
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

#define OBJ_BANK self->data[0]

void MegaIndicatorCallback(struct Sprite* self) {
	// Visibility
	
	switch(TAG) {
		case GFX_TAG_ALPHA_INDICATOR:
			if (!IsBluePrimal(OBJ_BANK))
			{
				self->pos1.x = -8;
				return;
			}
			break;
		
		case GFX_TAG_OMEGA_INDICATOR:
			if (!IsRedPrimal(OBJ_BANK))
			{
				self->pos1.x = -8;
				return;
			}
			break;
		
		case GFX_TAG_ULTRA_INDICATOR:
			if (!IsUltraNecrozma(OBJ_BANK))
			{
				self->pos1.x = -8;
				return;
			}
			break;
		
		default: //GFX_TAG_MEGA_TRIGGER
			if (!IsMega(OBJ_BANK)) 
			{
				self->pos1.x = -8;
				return;
			}
	}
	
	struct Sprite* healthbox = GetHealthboxObjId(OBJ_BANK);
	
	if (healthbox->invisible) 
	{
		self->pos1.x = -8;
		return;
	}
	
	u8 y = healthbox->oam.y;
	u8 x = healthbox->oam.x;
		
	// Mirror healthbox priority
	self->oam.priority = healthbox->oam.priority;
		
	if (y) 
	{
		// Figure out the X position for the indicator - it differs depending on
		// the battle type and the side the healthbox represents.
		s16 shift = 64; // Halfway point for OAM
	
		if (SIDE(OBJ_BANK) == B_SIDE_OPPONENT)
			shift += 18;
		else
			shift += 26;
		
		// Convert the level to a string to get how long it is
		u8 buf[10];
		u8 stringlen = ConvertIntToDecimalStringN(buf, GetBankPartyData(OBJ_BANK)->level, 0, 3) - buf;
			
		// The x position depends on the X origin of the healthbox as well as
		// the string length
		self->pos1.x = x + shift - 5 * stringlen;
			
		u8 pingid = gBattleSpritesDataPtr->healthBoxesData[OBJ_BANK].healthboxBounceSpriteId;
		struct Sprite* ping = &gSprites[pingid]; 
			
		bool8 sineActive = (gBattleSpritesDataPtr->healthBoxesData[OBJ_BANK].healthboxIsBouncing 
						 || gBattleSpritesDataPtr->healthBoxesData[OBJ_BANK].battlerIsBouncing);
		
		struct Sprite* shaker = &gSprites[ShakerData[1]];
		u8 hbid = gHealthboxIDs[OBJ_BANK];
			
		if (sineActive) 
		{
			self->pos1.y = healthbox->pos1.y - 4;
			self->pos2.y = Sine(ping->data[0], ping->data[2]);
			return;
		} 
		else if (shaker->data[1] == hbid && ShakerData[0]) 
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
		self->pos1.x = -8;
}

void LoadMegaGraphics(u8 state) 
{
#ifdef MEGA_EVOLUTION_FEATURE
	u8 objid;

	if (state == 2) 
	{	
		LoadSpritePalette(&MegaIndicatorPalette);
		
		LoadCompressedSpriteSheetUsingHeap(&MegaIndicatorSpriteSheet);
		LoadCompressedSpriteSheetUsingHeap(&AlphaIndicatorSpriteSheet);
		LoadCompressedSpriteSheetUsingHeap(&OmegaIndicatorSpriteSheet);
		LoadCompressedSpriteSheetUsingHeap(&UltraIndicatorSpriteSheet);
		
		// Create a Mega Indicator for every bank
		for (u8 bank = 0; bank < gBattlersCount; ++bank) 
		{
			if (IsMega(bank))
			{
				objid = CreateSprite(&MegaIndicatorTemplate, 0, 0, 1);
				gSprites[objid].data[0] = bank;
				gNewBS->megaIndicatorObjIds[bank] = objid + 1;
			}
			else if (IsBluePrimal(bank))
			{
				objid = CreateSprite(&AlphaIndicatorTemplate, 0, 0, 1);
				gSprites[objid].data[0] = bank;
				gNewBS->megaIndicatorObjIds[bank] = objid + 1;
			}
			else if (IsRedPrimal(bank))
			{
				objid = CreateSprite(&OmegaIndicatorTemplate, 0, 0, 1);
				gSprites[objid].data[0] = bank;
				gNewBS->megaIndicatorObjIds[bank] = objid + 1;
			}
			else if (IsUltraNecrozma(bank))
			{
				objid = CreateSprite(&UltraIndicatorTemplate, 0, 0, 1);
				gSprites[objid].data[0] = bank;
				gNewBS->megaIndicatorObjIds[bank] = objid + 1;
			}
			else
				gNewBS->megaIndicatorObjIds[bank] = 0;
		}
	}
#endif
}

void CreateMegaIndicatorAfterAnim(void)
{
	u8 objid;
	
	if (!gNewBS->megaIndicatorObjIds[gActiveBattler] && gActiveBattler < gBattlersCount)
	{	
		if (IsMega(gActiveBattler))
		{
			objid = CreateSprite(&MegaIndicatorTemplate, 0, 0, 1);
			gSprites[objid].data[0] = gActiveBattler;
			gNewBS->megaIndicatorObjIds[gActiveBattler] = objid + 1;
		}
		else if (IsBluePrimal(gActiveBattler))
		{
			objid = CreateSprite(&AlphaIndicatorTemplate, 0, 0, 1);
			gSprites[objid].data[0] = gActiveBattler;
			gNewBS->megaIndicatorObjIds[gActiveBattler] = objid + 1;
		}
		else if (IsRedPrimal(gActiveBattler))
		{
			objid = CreateSprite(&OmegaIndicatorTemplate, 0, 0, 1);
			gSprites[objid].data[0] = gActiveBattler;
			gNewBS->megaIndicatorObjIds[gActiveBattler] = objid + 1;
		}
		else if (IsUltraNecrozma(gActiveBattler))
		{
			objid = CreateSprite(&UltraIndicatorTemplate, 0, 0, 1);
			gSprites[objid].data[0] = gActiveBattler;
			gNewBS->megaIndicatorObjIds[gActiveBattler] = objid + 1;
		}
	}
}

//The Mega Graphics are usually loaded before, but this is placed here just in
//case the battle started against a Primal and their symbol wasn't loaded.
void TryLoadIndicatorForEachBank(void)
{
	for (gActiveBattler = 0; gActiveBattler < gBattlersCount; ++gActiveBattler)
		CreateMegaIndicatorAfterAnim();
}

void TryLoadMegaTriggers(void)
{
	u8 objid;
	
	if (gBattleTypeFlags & BATTLE_TYPE_SAFARI)
		return;
	
	LoadSpritePalette(&MegaTriggerPalette);
	LoadSpritePalette(&UltraTriggerPalette);
	LoadCompressedSpriteSheetUsingHeap(&MegaTriggerSpriteSheet);
	LoadCompressedSpriteSheetUsingHeap(&UltraTriggerSpriteSheet);
		
	objid = CreateSprite(&MegaTriggerTemplate, 130, 90, 1);
	gSprites[objid].data[3] = 16;
	gSprites[objid].pos1.x = -32;
	gSprites[objid].data[4] = gActiveBattler;
		
	objid = CreateSprite(&UltraTriggerTemplate, 130, 90, 1);
	gSprites[objid].data[3] = 16;
	gSprites[objid].pos1.x = -32;
	gSprites[objid].data[4] = gActiveBattler;

}

void DestroyMegaTriggers(void)
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
