#include "../defines.h"

#include "../../include/new/battle_transition_data.h"
/*
mugshot_tables.c
	set up image/palette tables for trainer battle transition mugshots

tables to edit:
	sPreBattleMugshotSprites
	sMugshotsBigPals
	sMugshotsDpPals
	sMugshotsTwoBarsPals
	sMugshotPlayerPals

*/

///////////////////////// Custom Mugshot Table ///////////////////////////
/////////// Replaces Trainer Front Sprite with custom Image //////////////

#ifdef FLAG_LOAD_MUGSHOT_SPRITE_FROM_TABLE
const struct MugshotTable sPreBattleMugshotSprites[147] =
{
	//[106] = {Silver_MugshotTiles, Silver_MugshotPal, 64 * 64 / 2, 0, -32}, //Example: Replaces Blue's sprite with Silver's mugshot
	//[124] = {Silver_MugshotTiles, Silver_MugshotPal, 64 * 64 / 2, 0, -32}, //Example: Replaces Player's sprite with Silver's mugshot
};
#endif

//////////////////// Mugshot Tilemap Palettes ////////////////////////
///// trainerbattle 0xW 0xX 0xY00 @z, 0xY is the palette number //////
const u16* const sMugshotsBigPals[] =
{
	[0x0] = Big_MugshotPal, //This is called as an error pal
	[0x1] = Big_MugshotPal, //0x100
	[0x2] = Big_Mugshot_BlackPal, //0x200
	[0x3] = Big_Mugshot_Dark_GrayPal, //0x300
	[0x4] = Big_Mugshot_Dark_PurplePal, //0x400
	[0x5] = Big_Mugshot_GreenPal, //0x500
	[0x6] = Big_Mugshot_Icy_BluePal, //0x600
	[0x7] = Big_Mugshot_Light_BrownPal, //0x700
	[0x8] = Big_Mugshot_Light_GreenPal, //0x800
	[0x9] = Big_Mugshot_Pearly_WhitePal, //0x900
	[0xA] = Big_Mugshot_PinkPal, //0xA00
	[0xB] = Big_Mugshot_PurplePal, //0xB00
	[0xC] = Big_Mugshot_Royal_BluePal, //0xC00
	[0xD] = Big_Mugshot_Sky_BluePal, //0xD00
	[0xE] = Big_Mugshot_Steel_GrayPal, //0xE00
	[0xF] = Big_Mugshot_Yellow_GrayPal, //0xF00
};

const u16* const sMugshotsDpPals[] =
{
	[0x0] = DP_MugshotPal, //This is called as an error pal
	[0x1] = DP_MugshotPal, //0x100
	[0x2] = DP_Mugshot_BlackPal, //0x200
	[0x3] = DP_Mugshot_Dark_GrayPal, //0x300
	[0x4] = DP_Mugshot_Dark_PurplePal, //0x400
	[0x5] = DP_Mugshot_Dark_GreenPal, //0x500
	[0x6] = DP_Mugshot_Icy_BluePal, //0x600
	[0x7] = DP_Mugshot_Light_BrownPal, //0x700
	[0x8] = DP_Mugshot_Light_GreenPal, //0x800
	[0x9] = DP_Mugshot_WhitePal, //0x900
	[0xA] = DP_Mugshot_PinkPal, //0xA00
	[0xB] = DP_Mugshot_Dark_RedPal, //0xB00
	[0xC] = DP_Mugshot_Royal_BluePal, //0xC00
	[0xD] = DP_Mugshot_Sky_BluePal, //0xD00
	[0xE] = DP_Mugshot_Steel_GrayPal, //0xE00
	[0xF] = DP_Mugshot_YellowPal, //0xF00
};

// Separate from DP Table so user can customize palettes easier
const u16* const sMugshotsTwoBarsPals[] =
{
	[0x0] = TwoBars_MugshotPal, //This is called as an error pal
	[0x1] = TwoBars_MugshotPal, //0x100
	[0x2] = DP_Mugshot_BlackPal, //0x200
	[0x3] = DP_Mugshot_Dark_GrayPal, //0x300
	[0x4] = DP_Mugshot_Dark_GreenPal, //0x400
	[0x5] = DP_Mugshot_Dark_PurplePal, //0x500
	[0x6] = DP_Mugshot_Dark_RedPal, //0x600
	[0x7] = DP_Mugshot_Icy_BluePal, //0x700
	[0x8] = DP_Mugshot_Light_GreenPal, //0x800
	[0x9] = DP_Mugshot_Royal_BluePal, //0x900
	[0xA] = DP_Mugshot_Steel_GrayPal, //0xA00
	[0xB] = DP_Mugshot_Light_BrownPal, //0xB00
	[0xC] = DP_Mugshot_PinkPal, //0xC00
	[0xD] = DP_Mugshot_WhitePal, //0xD00
	[0xE] = DP_Mugshot_Sky_BluePal, //0xE00
	[0xF] = DP_Mugshot_YellowPal, //0xF00
};


const u16* const sMugshotPlayerPals[] =
{
	[0x0] = TwoBars_MugshotPal, //This is called as an error pal
	[0x1] = TwoBars_MugshotPal,
	[0x2] = DP_Mugshot_BlackPal,
	[0x3] = DP_Mugshot_Dark_GrayPal,
	[0x4] = DP_Mugshot_Dark_GreenPal,
	[0x5] = DP_Mugshot_Dark_PurplePal,
	[0x6] = DP_Mugshot_Dark_RedPal,
	[0x7] = DP_Mugshot_Icy_BluePal,
	[0x8] = DP_Mugshot_Light_GreenPal,
	[0x9] = DP_Mugshot_Royal_BluePal,
	[0xA] = DP_Mugshot_Steel_GrayPal,
	[0xB] = DP_Mugshot_Light_BrownPal,
	[0xC] = DP_Mugshot_PinkPal,
	[0xD] = DP_Mugshot_WhitePal,
	[0xE] = DP_Mugshot_Sky_BluePal,
	[0xF] = DP_Mugshot_YellowPal,
};

