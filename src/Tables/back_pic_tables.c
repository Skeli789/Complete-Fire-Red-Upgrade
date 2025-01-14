#include "../defines.h"
#include "../../include/data2.h"
#include "../../include/sprite.h"
#include "../../include/constants/trainers.h"

/*
back_pic_tables.c
	Set up tables to switch the player's back sprite

tables to edit:
	gTrainerBackPicPaletteTable
	gTrainerBackAnimsPtrTable
	gTrainerBackPicCoords
	sTrainerBackPicTable_[backsprite]
	gSpriteTemplateTable_TrainerBackSprites
*/

#define gTrainerBackPic_RedPal (const u8*) 0x8E76EBC
#define gTrainerBackPic_LeafPal (const u8*) 0x8E76EE4
//#define gTrainerBackPic_BrendanPal (const u8*) 0x8E550A4
//#define gTrainerBackPic_MayPal (const u8*) 0x8E553CC
#define gTrainerBackPic_PokeDudePal (const u8*) 0x8E76F0C
#define gTrainerBackPic_OldManPal (const u8*) 0x8E76F34

#define PAL_DEFINE(name) extern const u8 gTrainerBackPic_##name##Pal[];
#define PAL_ENTRY(NAME, name) [TRAINER_BACK_PIC_##NAME] = {gTrainerBackPic_##name##Pal, TRAINER_BACK_PIC_##NAME},

PAL_DEFINE(PlayerMale)
PAL_DEFINE(PlayerFemale)
PAL_DEFINE(Brendan)
PAL_DEFINE(May)
PAL_DEFINE(Marlon)
PAL_DEFINE(MarlonArm)
PAL_DEFINE(MarlonPlayerM)
PAL_DEFINE(MarlonPlayerF)
PAL_DEFINE(PokeKid)
PAL_DEFINE(Rival)
PAL_DEFINE(Jax)
PAL_DEFINE(RedNew)
PAL_DEFINE(RedPlayer)
PAL_DEFINE(LeafPlayer)
PAL_DEFINE(EthanPlayer)
PAL_DEFINE(LyraPlayer)
PAL_DEFINE(LucasPlayer)
PAL_DEFINE(DawnPlayer)
PAL_DEFINE(PlayerChampionM)
PAL_DEFINE(PlayerChampionF)
PAL_DEFINE(Catherine)
PAL_DEFINE(Ginger)
PAL_DEFINE(Abimbola)
PAL_DEFINE(Mahina)
PAL_DEFINE(Alford)
PAL_DEFINE(Vega)
PAL_DEFINE(Alice)
PAL_DEFINE(Mel)
PAL_DEFINE(Galavan)
PAL_DEFINE(BigMo)
PAL_DEFINE(Tessy)

const struct CompressedSpritePalette gTrainerBackPicPaletteTable[] =
{
	PAL_ENTRY(BRENDAN, Brendan)
	PAL_ENTRY(MAY, May)
	PAL_ENTRY(POKE_DUDE, PokeDude)
	PAL_ENTRY(OLD_MAN, OldMan)

#ifndef UNBOUND
	PAL_ENTRY(RED, Red)
	PAL_ENTRY(LEAF, Leaf)
#else //For Pokemon Unbound
	PAL_ENTRY(RED, PlayerMale)
	PAL_ENTRY(LEAF, PlayerFemale)
	PAL_ENTRY(MARLON, Marlon)
	PAL_ENTRY(MARLON_ARM, MarlonArm)
	PAL_ENTRY(MARLON_PLAYER_M,MarlonPlayerM)
	PAL_ENTRY(MARLON_PLAYER_F, MarlonPlayerF)
	PAL_ENTRY(POKE_KID, PokeKid)
	PAL_ENTRY(RIVAL, Rival)
	PAL_ENTRY(JAX, Jax)
	PAL_ENTRY(RED_NEW, RedNew)
	PAL_ENTRY(RED_PLAYER, RedPlayer)
	PAL_ENTRY(LEAF_PLAYER, LeafPlayer)
	PAL_ENTRY(ETHAN_PLAYER, EthanPlayer)
	PAL_ENTRY(LYRA_PLAYER, LyraPlayer)
	PAL_ENTRY(LUCAS_PLAYER, LucasPlayer)
	PAL_ENTRY(DAWN_PLAYER, DawnPlayer)
	PAL_ENTRY(PLAYER_CHAMPION_M, PlayerChampionM)
	PAL_ENTRY(PLAYER_CHAMPION_F, PlayerChampionF)
	PAL_ENTRY(CATHERINE, Catherine)
	PAL_ENTRY(GINGER, Ginger)
	PAL_ENTRY(ABIMBOLA, Abimbola)
	PAL_ENTRY(MAHINA, Mahina)
	PAL_ENTRY(ALFORD, Alford)
	PAL_ENTRY(VEGA, Vega)
	PAL_ENTRY(ALICE, Alice)
	PAL_ENTRY(MEL, Mel)
	PAL_ENTRY(GALAVAN, Galavan)
	PAL_ENTRY(BIG_MO, BigMo)
	PAL_ENTRY(TESSY, Tessy)
#endif
};

#define gTrainerBackAnims_Red (const union AnimCmd* const*) 0x8239F44
#define gTrainerBackAnims_Leaf (const union AnimCmd* const*) 0x8239F4C
#define gTrainerBackAnims_Brendan (const union AnimCmd* const*) 0x8239F54
#define gTrainerBackAnims_May (const union AnimCmd* const*) 0x8239F54
#define gTrainerBackAnims_PokeDude (const union AnimCmd* const*) 0x8239F54
#define gTrainerBackAnims_OldMan (const union AnimCmd* const*) 0x8239F5C
#define gTrainerBackAnims_PokeKid (const union AnimCmd* const*) 0x8239F54
#define gTrainerBackAnims_Rival (const union AnimCmd* const*) 0x8239F44
#define gTrainerBackAnims_Jax (const union AnimCmd* const*) 0x8239F44
#define gTrainerBackAnims_RedNew (const union AnimCmd* const*) 0x8239F54
#define gTrainerBackAnims_EthanLyra (const union AnimCmd* const*) 0x8239F44
#define gTrainerBackAnims_LucasDawn (const union AnimCmd* const*) 0x8239F44
#define gTrainerBackAnims_PlayerChampion (const union AnimCmd* const*) 0x8239F44
#define gTrainerBackAnims_Catherine (const union AnimCmd* const*) 0x8239F44
#define gTrainerBackAnims_Ginger (const union AnimCmd* const*) 0x8239F44
#define gTrainerBackAnims_Abimbola (const union AnimCmd* const*) 0x8239F44
#define gTrainerBackAnims_Mahina (const union AnimCmd* const*) 0x8239F44
#define gTrainerBackAnims_Alford (const union AnimCmd* const*) 0x8239F44
#define gTrainerBackAnims_Vega (const union AnimCmd* const*) 0x8239F44
#define gTrainerBackAnims_Alice (const union AnimCmd* const*) 0x8239F44
#define gTrainerBackAnims_Mel (const union AnimCmd* const*) 0x8239F44
#define gTrainerBackAnims_Galavan (const union AnimCmd* const*) 0x8239F44
#define gTrainerBackAnims_BigMo (const union AnimCmd* const*) 0x8239F44
#define gTrainerBackAnims_Tessy (const union AnimCmd* const*) 0x8239F44

static const union AnimCmd sAnimCmd_Marlon[] =
{
    ANIMCMD_FRAME(1, 32),
    ANIMCMD_FRAME(2, 6),
    ANIMCMD_FRAME(3, 6),
    ANIMCMD_FRAME(4, 4),
    ANIMCMD_END
};

const union AnimCmd *const gTrainerBackAnims_Marlon[] =
{
	(void*) 0x82347E8, //sAnim_GeneralFrame0
	sAnimCmd_Marlon,
};

const union AnimCmd* const* const gTrainerBackAnimsPtrTable[] =
{
	[TRAINER_BACK_PIC_RED] = gTrainerBackAnims_Red,
	[TRAINER_BACK_PIC_LEAF] = gTrainerBackAnims_Leaf,
	[TRAINER_BACK_PIC_BRENDAN] = gTrainerBackAnims_Brendan,
	[TRAINER_BACK_PIC_MAY] = gTrainerBackAnims_May,
	[TRAINER_BACK_PIC_POKE_DUDE] = gTrainerBackAnims_PokeDude,
	[TRAINER_BACK_PIC_OLD_MAN] = gTrainerBackAnims_OldMan,

#ifdef UNBOUND
	[TRAINER_BACK_PIC_MARLON] = gTrainerBackAnims_Marlon,
	[TRAINER_BACK_PIC_MARLON_ARM] = gTrainerBackAnims_Marlon,
	[TRAINER_BACK_PIC_MARLON_PLAYER_M] = gTrainerBackAnims_Marlon,
	[TRAINER_BACK_PIC_MARLON_PLAYER_F] = gTrainerBackAnims_Marlon,
	[TRAINER_BACK_PIC_POKE_KID] = gTrainerBackAnims_PokeKid,
	[TRAINER_BACK_PIC_RIVAL] = gTrainerBackAnims_Rival,
	[TRAINER_BACK_PIC_JAX] = gTrainerBackAnims_Jax,
	[TRAINER_BACK_PIC_RED_NEW] = gTrainerBackAnims_RedNew,
	[TRAINER_BACK_PIC_RED_PLAYER] = gTrainerBackAnims_RedNew,
	[TRAINER_BACK_PIC_LEAF_PLAYER] = gTrainerBackAnims_RedNew,
	[TRAINER_BACK_PIC_ETHAN_PLAYER] = gTrainerBackAnims_EthanLyra,
	[TRAINER_BACK_PIC_LYRA_PLAYER] = gTrainerBackAnims_EthanLyra,
	[TRAINER_BACK_PIC_LUCAS_PLAYER] = gTrainerBackAnims_LucasDawn,
	[TRAINER_BACK_PIC_DAWN_PLAYER] = gTrainerBackAnims_LucasDawn,
	[TRAINER_BACK_PIC_PLAYER_CHAMPION_M] = gTrainerBackAnims_PlayerChampion,
	[TRAINER_BACK_PIC_PLAYER_CHAMPION_F] = gTrainerBackAnims_PlayerChampion,
	[TRAINER_BACK_PIC_CATHERINE] = gTrainerBackAnims_Catherine,
	[TRAINER_BACK_PIC_GINGER] = gTrainerBackAnims_Ginger,
	[TRAINER_BACK_PIC_ABIMBOLA] = gTrainerBackAnims_Abimbola,
	[TRAINER_BACK_PIC_MAHINA] = gTrainerBackAnims_Mahina,
	[TRAINER_BACK_PIC_ALFORD] = gTrainerBackAnims_Alford,
	[TRAINER_BACK_PIC_VEGA] = gTrainerBackAnims_Vega,
	[TRAINER_BACK_PIC_ALICE] = gTrainerBackAnims_Alice,
	[TRAINER_BACK_PIC_MEL] = gTrainerBackAnims_Mel,
	[TRAINER_BACK_PIC_GALAVAN] = gTrainerBackAnims_Galavan,
	[TRAINER_BACK_PIC_BIG_MO] = gTrainerBackAnims_BigMo,
	[TRAINER_BACK_PIC_TESSY] =	gTrainerBackAnims_Tessy,
#endif
};

const struct MonCoords gTrainerBackPicCoords[] =
{
	[TRAINER_BACK_PIC_RED] = 		{.coords = 8, .y_offset = 5},
	[TRAINER_BACK_PIC_LEAF] = 		{.coords = 8, .y_offset = 5},
	[TRAINER_BACK_PIC_BRENDAN] = 	{.coords = 8, .y_offset = 4},
	[TRAINER_BACK_PIC_MAY] = 		{.coords = 8, .y_offset = 4},
	[TRAINER_BACK_PIC_POKE_DUDE] = 	{.coords = 8, .y_offset = 4},
	[TRAINER_BACK_PIC_OLD_MAN] = 	{.coords = 8, .y_offset = 4},

#ifdef UNBOUND
	[TRAINER_BACK_PIC_MARLON] =            {.coords = 8, .y_offset = 4},
	[TRAINER_BACK_PIC_MARLON_ARM] =        {.coords = 8, .y_offset = 4},
	[TRAINER_BACK_PIC_MARLON_PLAYER_M] =   {.coords = 8, .y_offset = 4},
	[TRAINER_BACK_PIC_MARLON_PLAYER_F] =   {.coords = 8, .y_offset = 4},
	[TRAINER_BACK_PIC_POKE_KID] =          {.coords = 8, .y_offset = 4},
	[TRAINER_BACK_PIC_RIVAL] =             {.coords = 8, .y_offset = 4},
	[TRAINER_BACK_PIC_JAX] =               {.coords = 8, .y_offset = 4},
	[TRAINER_BACK_PIC_RED_NEW] =           {.coords = 8, .y_offset = 4},
	[TRAINER_BACK_PIC_RED_PLAYER] =        {.coords = 8, .y_offset = 4},
	[TRAINER_BACK_PIC_LEAF_PLAYER] =       {.coords = 8, .y_offset = 4},
	[TRAINER_BACK_PIC_ETHAN_PLAYER] =      {.coords = 8, .y_offset = 4},
	[TRAINER_BACK_PIC_LYRA_PLAYER] =       {.coords = 8, .y_offset = 4},
	[TRAINER_BACK_PIC_LUCAS_PLAYER] =      {.coords = 8, .y_offset = 4},
	[TRAINER_BACK_PIC_DAWN_PLAYER] =       {.coords = 8, .y_offset = 4},
	[TRAINER_BACK_PIC_PLAYER_CHAMPION_M] = {.coords = 8, .y_offset = 5},
	[TRAINER_BACK_PIC_PLAYER_CHAMPION_F] = {.coords = 8, .y_offset = 5},
	[TRAINER_BACK_PIC_CATHERINE] =         {.coords = 8, .y_offset = 4},
	[TRAINER_BACK_PIC_GINGER] =            {.coords = 8, .y_offset = 4},
	[TRAINER_BACK_PIC_ABIMBOLA] =          {.coords = 8, .y_offset = 4},
	[TRAINER_BACK_PIC_MAHINA] =            {.coords = 8, .y_offset = 4},
	[TRAINER_BACK_PIC_ALFORD] =            {.coords = 8, .y_offset = 4},
	[TRAINER_BACK_PIC_VEGA] =              {.coords = 8, .y_offset = 4},
	[TRAINER_BACK_PIC_ALICE] =             {.coords = 8, .y_offset = 4},
	[TRAINER_BACK_PIC_MEL] =               {.coords = 8, .y_offset = 4},
	[TRAINER_BACK_PIC_GALAVAN] =           {.coords = 8, .y_offset = 4},
	[TRAINER_BACK_PIC_BIG_MO] =            {.coords = 8, .y_offset = 4},
	[TRAINER_BACK_PIC_TESSY] =             {.coords = 8, .y_offset = 4},
#endif
};

#define gOamData_TrainerBacksprite (void*) 0x824F018
#define gAffineAnims_TrainerBacksprite (void*) 0x82348C8
#define gSpriteCB_TrainerBacksprite (void*) (0x80120C4 | 1)

#define sTrainerBackPicTable_Red (void*) 0x8234718
#define sTrainerBackPicTable_Leaf (void*) 0x8234740
//#define sTrainerBackPicTable_Brendan (void*) 0x82347A8
//#define sTrainerBackPicTable_May (void*) 0x82347C8
#define sTrainerBackPicTable_PokeDude (void*) 0x8234768
#define sTrainerBackPicTable_OldMan (void*) 0x8234788

#define FOUR_FRAME_TABLE(name)                                           \
	extern const u8 gTrainerBackPic_##name##Tiles[];                     \
	static const struct SpriteFrameImage sTrainerBackPicTable_##name[] = \
	{                                                                    \
		{gTrainerBackPic_##name##Tiles + 0x0000, 0x800, 0},              \
		{gTrainerBackPic_##name##Tiles + 0x0800, 0x800, 0},              \
		{gTrainerBackPic_##name##Tiles + 0x1000, 0x800, 0},              \
		{gTrainerBackPic_##name##Tiles + 0x1800, 0x800, 0},              \
	};

#define FIVE_FRAME_TABLE(name)                                           \
	extern const u8 gTrainerBackPic_##name##Tiles[];                     \
	static const struct SpriteFrameImage sTrainerBackPicTable_##name[] = \
	{                                                                    \
		{gTrainerBackPic_##name##Tiles + 0x0000, 0x800, 0},              \
		{gTrainerBackPic_##name##Tiles + 0x0800, 0x800, 0},              \
		{gTrainerBackPic_##name##Tiles + 0x1000, 0x800, 0},              \
		{gTrainerBackPic_##name##Tiles + 0x1800, 0x800, 0},              \
		{gTrainerBackPic_##name##Tiles + 0x2000, 0x800, 0},              \
	};

FOUR_FRAME_TABLE(Brendan)
FOUR_FRAME_TABLE(May)

#ifdef UNBOUND

#undef sTrainerBackPicTable_Red
#undef sTrainerBackPicTable_Leaf

extern const u8 gTrainerBackPic_PlayerMaleTiles[];
extern const u8 gTrainerBackPic_PlayerFemaleTiles[];
extern const u8 gTrainerBackPic_BigMoTiles[];
static const struct SpriteFrameImage sTrainerBackPicTable_Red[] =
{
	{gTrainerBackPic_PlayerMaleTiles, 			0x800, 0},
	{gTrainerBackPic_PlayerMaleTiles + 0x0800, 	0x800, 0},
	{gTrainerBackPic_PlayerMaleTiles + 0x1000, 	0x800, 0},
	{gTrainerBackPic_PlayerMaleTiles + 0x1800, 	0x800, 0},
	{gTrainerBackPic_PlayerMaleTiles + 0x2000, 	0x800, 0},
};

static const struct SpriteFrameImage sTrainerBackPicTable_Leaf[] =
{
	{gTrainerBackPic_PlayerFemaleTiles, 			0x800, 0},
	{gTrainerBackPic_PlayerFemaleTiles + 0x0800, 	0x800, 0},
	{gTrainerBackPic_PlayerFemaleTiles + 0x1000, 	0x800, 0},
	{gTrainerBackPic_PlayerFemaleTiles + 0x1800, 	0x800, 0},
	{gTrainerBackPic_PlayerFemaleTiles + 0x2000, 	0x800, 0},
};

static const struct SpriteFrameImage sTrainerBackPicTable_BigMo[] =
{
	{gTrainerBackPic_BigMoTiles, 			0x800, 0},
	{gTrainerBackPic_BigMoTiles ,			0x800, 0},
	{gTrainerBackPic_BigMoTiles + 0x0800, 	0x800, 0},
	{gTrainerBackPic_BigMoTiles + 0x1000, 	0x800, 0},
	{gTrainerBackPic_BigMoTiles + 0x1800, 	0x800, 0},
};

FIVE_FRAME_TABLE(Marlon)
FIVE_FRAME_TABLE(MarlonArm)
FIVE_FRAME_TABLE(MarlonPlayerM)
FIVE_FRAME_TABLE(MarlonPlayerF)
FOUR_FRAME_TABLE(PokeKid)
FIVE_FRAME_TABLE(Rival)
FIVE_FRAME_TABLE(Jax)
FOUR_FRAME_TABLE(RedNew)
FOUR_FRAME_TABLE(RedPlayer)
FOUR_FRAME_TABLE(LeafPlayer)
FIVE_FRAME_TABLE(EthanPlayer)
FIVE_FRAME_TABLE(LyraPlayer)
FIVE_FRAME_TABLE(LucasPlayer)
FIVE_FRAME_TABLE(DawnPlayer)
FIVE_FRAME_TABLE(PlayerChampionM)
FIVE_FRAME_TABLE(PlayerChampionF)
FIVE_FRAME_TABLE(Catherine)
FIVE_FRAME_TABLE(Ginger)
FIVE_FRAME_TABLE(Abimbola)
FIVE_FRAME_TABLE(Mahina)
FIVE_FRAME_TABLE(Alford)
FIVE_FRAME_TABLE(Vega)
FIVE_FRAME_TABLE(Alice)
FIVE_FRAME_TABLE(Mel)
FIVE_FRAME_TABLE(Galavan)
FIVE_FRAME_TABLE(Tessy)

#endif

#define BACK_TEMPLATE(name)                            \
	{                                                  \
		.tileTag = 0xFFFF,                             \
		.paletteTag = 0,                               \
		.oam = gOamData_TrainerBacksprite,             \
		.anims = NULL,                                 \
		.images = sTrainerBackPicTable_##name,         \
		.affineAnims = gAffineAnims_TrainerBacksprite, \
		.callback = gSpriteCB_TrainerBacksprite,       \
	},

const struct SpriteTemplate gSpriteTemplateTable_TrainerBackSprites[] =
{
	[TRAINER_BACK_PIC_RED] = BACK_TEMPLATE(Red)
	[TRAINER_BACK_PIC_LEAF] = BACK_TEMPLATE(Leaf)
	[TRAINER_BACK_PIC_BRENDAN] = BACK_TEMPLATE(Brendan)
	[TRAINER_BACK_PIC_MAY] = BACK_TEMPLATE(May)
	[TRAINER_BACK_PIC_POKE_DUDE] = BACK_TEMPLATE(PokeDude)
	[TRAINER_BACK_PIC_OLD_MAN] = BACK_TEMPLATE(OldMan)

#ifdef UNBOUND
	[TRAINER_BACK_PIC_MARLON] = BACK_TEMPLATE(Marlon)
	[TRAINER_BACK_PIC_MARLON_ARM] = BACK_TEMPLATE(MarlonArm)
	[TRAINER_BACK_PIC_MARLON_PLAYER_M] = BACK_TEMPLATE(MarlonPlayerM)
	[TRAINER_BACK_PIC_MARLON_PLAYER_F] = BACK_TEMPLATE(MarlonPlayerF)
	[TRAINER_BACK_PIC_POKE_KID] = BACK_TEMPLATE(PokeKid)
	[TRAINER_BACK_PIC_RIVAL] = BACK_TEMPLATE(Rival)
	[TRAINER_BACK_PIC_JAX] = BACK_TEMPLATE(Jax)
	[TRAINER_BACK_PIC_RED_NEW] = BACK_TEMPLATE(RedNew)
	[TRAINER_BACK_PIC_RED_PLAYER] = BACK_TEMPLATE(RedPlayer)
	[TRAINER_BACK_PIC_LEAF_PLAYER] = BACK_TEMPLATE(LeafPlayer)
	[TRAINER_BACK_PIC_ETHAN_PLAYER] = BACK_TEMPLATE(EthanPlayer)
	[TRAINER_BACK_PIC_LYRA_PLAYER] = BACK_TEMPLATE(LyraPlayer)
	[TRAINER_BACK_PIC_LUCAS_PLAYER] = BACK_TEMPLATE(LucasPlayer)
	[TRAINER_BACK_PIC_DAWN_PLAYER] = BACK_TEMPLATE(DawnPlayer)
	[TRAINER_BACK_PIC_PLAYER_CHAMPION_M] = BACK_TEMPLATE(PlayerChampionM)
	[TRAINER_BACK_PIC_PLAYER_CHAMPION_F] = BACK_TEMPLATE(PlayerChampionF)
	[TRAINER_BACK_PIC_CATHERINE] = BACK_TEMPLATE(Catherine)
	[TRAINER_BACK_PIC_GINGER] = BACK_TEMPLATE(Ginger)
	[TRAINER_BACK_PIC_ABIMBOLA] = BACK_TEMPLATE(Abimbola)
	[TRAINER_BACK_PIC_MAHINA] = BACK_TEMPLATE(Mahina)
	[TRAINER_BACK_PIC_ALFORD] = BACK_TEMPLATE(Alford)
	[TRAINER_BACK_PIC_VEGA] = BACK_TEMPLATE(Vega)
	[TRAINER_BACK_PIC_ALICE] = BACK_TEMPLATE(Alice)
	[TRAINER_BACK_PIC_MEL] = BACK_TEMPLATE(Mel)
	[TRAINER_BACK_PIC_GALAVAN] = BACK_TEMPLATE(Galavan)
	[TRAINER_BACK_PIC_BIG_MO] = BACK_TEMPLATE(BigMo)
	[TRAINER_BACK_PIC_TESSY] = BACK_TEMPLATE(Tessy)
#endif
};
