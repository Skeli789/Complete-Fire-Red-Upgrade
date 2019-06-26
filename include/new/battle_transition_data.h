#pragma once

// Don't Modify These Values!
// VAR_PRE_BATTLE_MUGSHOT_STYLE
enum {
    MUGSHOT_BIG = 0,
    MUGSHOT_DP,
    MUGSHOT_TWO_BARS,
};

// VAR_PRE_BATTLE_MUGSHOT_SPRITE
enum {
    MUGSHOT_PLAYER = 0,
    MUGSHOT_VS_SYMBOL,
};

//MUGSHOT_BIG tiles
extern const u16 Big_MugshotTiles[];
extern const u16 Big_MugshotPal[];
extern const u16 Big_MugshotMap[];

//MUGSHOT_DP Tiles
extern const u16 DP_MugshotTiles[];
extern const u16 DP_MugshotPal[];
extern const u16 DP_MugshotMap[];

//MUGSHOT_TWO_BARS Tiles
#define TwoBars_MugshotTiles ((const u16*) 0x83F8F60)
#define TwoBars_MugshotPal ((const u16*) 0x83FA660)
#define TwoBars_MugshotMap ((const u16*) 0x83FAC34)


#define sOpponentMugshotsPals ((const u16**) 0x83FA740)
#define sPlayerMugshotsPals ((const u16**) 0x83FA754)
#define sMugshotsTrainerPicIDsTable ((const u8*) 0x3FA494)
extern const s16 sMugshotsOpponentCoords[][2];
extern const s16 sMugshotsOpponentRotationScales[][2];

extern const u8 VS_SpriteTiles[];
extern const u8 VS_SpritePal[];

extern const u8 Silver_MugshotTiles[];
extern const u8 Silver_MugshotPal[];

extern const u16 Big_Mugshot_Yellow_GrayPal[];


struct MugshotTable
{
	const u8* sprite;
	const u8* pal;
	u16 size;
	s16 x;
	s16 y;
};
