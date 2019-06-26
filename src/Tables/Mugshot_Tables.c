#include "../defines.h"

#include "../../include/new/battle_transition_data.h"

///////////////////////// Custom Mugshot Table ///////////////////////////
/////////// Replaces Trainer Front Sprite with custom Image //////////////

const struct MugshotTable sPreBattleMugshotSprites[147] =
{
	//[106] = {Silver_MugshotTiles, Silver_MugshotPal, 64 * 64 / 2, 0, -32}, //Example: Replaces Blue's sprite with Silver's mugshot
	//[124] = {Silver_MugshotTiles, Silver_MugshotPal, 64 * 64 / 2, 0, -32}, //Example: Replaces Player's sprite with Silver's mugshot
};



//////////////////// Mugshot Tilemap Palettes ////////////////////////
///// trainerbattle 0xW 0xX 0xY00 @z, 0xY is the palette number //////
const u16* const sMugshotsBigPals[] =
{
	[0x0] = Big_MugshotPal, //This is called as an error pal
	[0x1] = Big_MugshotPal, //0x100
	[0x2] = Big_MugshotPal, //0x200
	[0x3] = Big_Mugshot_Yellow_GrayPal, //0x300
	[0x4] = Big_MugshotPal, //0x400
	[0x5] = Big_MugshotPal, //0x500
	[0x6] = Big_MugshotPal, //0x600
	[0x7] = Big_MugshotPal, //0x700
	[0x8] = Big_MugshotPal, //0x800
	[0x9] = Big_MugshotPal, //0x900
	[0xA] = Big_MugshotPal, //0xA00
	[0xB] = Big_MugshotPal, //0xB00
	[0xC] = Big_MugshotPal, //0xC00
	[0xD] = Big_MugshotPal, //0xD00
	[0xE] = Big_MugshotPal, //0xE00
	[0xF] = Big_MugshotPal, //0xF00
};

const u16* const sMugshotsDpPals[] =
{
	[0x0] = DP_MugshotPal, //This is called as an error pal
	[0x1] = DP_MugshotPal, //0x100
	[0x2] = DP_MugshotPal, //0x200
	[0x3] = DP_MugshotPal, //0x300
	[0x4] = DP_MugshotPal, //0x400
	[0x5] = DP_MugshotPal, //0x500
	[0x6] = DP_MugshotPal, //0x600
	[0x7] = DP_MugshotPal, //0x700
	[0x8] = DP_MugshotPal, //0x800
	[0x9] = DP_MugshotPal, //0x900
	[0xA] = DP_MugshotPal, //0xA00
	[0xB] = DP_MugshotPal, //0xB00
	[0xC] = DP_MugshotPal, //0xC00
	[0xD] = DP_MugshotPal, //0xD00
	[0xE] = DP_MugshotPal, //0xE00
	[0xF] = DP_MugshotPal, //0xF00
};

const u16* const sMugshotsTwoBarsPals[] =
{
	[0x0] = TwoBars_MugshotPal, //This is called as an error pal
	[0x1] = TwoBars_MugshotPal, //0x100
	[0x2] = TwoBars_MugshotPal, //0x200
	[0x3] = TwoBars_MugshotPal, //0x300
	[0x4] = TwoBars_MugshotPal, //0x400
	[0x5] = TwoBars_MugshotPal, //0x500
	[0x6] = TwoBars_MugshotPal, //0x600
	[0x7] = TwoBars_MugshotPal, //0x700
	[0x8] = TwoBars_MugshotPal, //0x800
	[0x9] = TwoBars_MugshotPal, //0x900
	[0xA] = TwoBars_MugshotPal, //0xA00
	[0xB] = TwoBars_MugshotPal, //0xB00
	[0xC] = TwoBars_MugshotPal, //0xC00
	[0xD] = TwoBars_MugshotPal, //0xD00
	[0xE] = TwoBars_MugshotPal, //0xE00
	[0xF] = TwoBars_MugshotPal, //0xF00
};
