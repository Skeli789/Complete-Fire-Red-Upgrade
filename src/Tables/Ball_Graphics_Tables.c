#include "..\\defines.h"
#include "..\\catching.h"

#define POKEBALL_COUNT NUM_BALLS

#define gInterfaceGfx_PokeBall (const u8*) 0x8D01724
#define gInterfaceGfx_GreatBall (const u8*) 0x8D01800
#define gInterfaceGfx_SafariBall (const u8*) 0x8D018E0
#define gInterfaceGfx_UltraBall (const u8*) 0x8D019C8
#define gInterfaceGfx_MasterBall (const u8*) 0x8D01A9C
#define gInterfaceGfx_NetBall (const u8*) 0x8D01B80
#define gInterfaceGfx_DiveBall (const u8*) 0x8D01C60
#define gInterfaceGfx_NestBall (const u8*) 0x8D01D9C
#define gInterfaceGfx_RepeatBall (const u8*) 0x8D01E84
#define gInterfaceGfx_TimerBall (const u8*) 0x8D01F68
#define gInterfaceGfx_LuxuryBall (const u8*) 0x8D02084
#define gInterfaceGfx_PremierBall (const u8*) 0x8D021D8
extern const u8 gInterfaceGfx_DuskBallTiles[];
extern const u8 gInterfaceGfx_HealBallTiles[];
extern const u8 gInterfaceGfx_QuickBallTiles[];
extern const u8 gInterfaceGfx_CherishBallTiles[];
extern const u8 gInterfaceGfx_ParkBallTiles[];
extern const u8 gInterfaceGfx_FastBallTiles[];
extern const u8 gInterfaceGfx_LevelBallTiles[];
extern const u8 gInterfaceGfx_LureBallTiles[];
extern const u8 gInterfaceGfx_HeavyBallTiles[];
extern const u8 gInterfaceGfx_LoveBallTiles[];
extern const u8 gInterfaceGfx_FriendBallTiles[];
extern const u8 gInterfaceGfx_MoonBallTiles[];
extern const u8 gInterfaceGfx_SportBallTiles[];
extern const u8 gInterfaceGfx_BeastBallTiles[];
extern const u8 gInterfaceGfx_DreamBallTiles[];

#define gInterfacePal_PokeBall (const u8*) 0x8D017E0
#define gInterfacePal_GreatBall (const u8*) 0x8D018BC
#define gInterfacePal_SafariBall (const u8*) 0x8D019A4
#define gInterfacePal_UltraBall (const u8*) 0x8D01A7C
#define gInterfacePal_MasterBall (const u8*) 0x8D01B5C
#define gInterfacePal_NetBall (const u8*) 0x8D01C40
#define gInterfacePal_DiveBall (const u8*) 0x8D01D74
#define gInterfacePal_NestBall (const u8*) 0x8D01E60
#define gInterfacePal_RepeatBall (const u8*) 0x8D01F44
#define gInterfacePal_TimerBall (const u8*) 0x8D02060
#define gInterfacePal_LuxuryBall (const u8*) 0x8D021B0
#define gInterfacePal_PremierBall (const u8*) 0x8D022C8
extern const u8 gInterfaceGfx_DuskBallPal[];
extern const u8 gInterfaceGfx_HealBallPal[];
extern const u8 gInterfaceGfx_QuickBallPal[];
extern const u8 gInterfaceGfx_CherishBallPal[];
extern const u8 gInterfaceGfx_ParkBallPal[];
extern const u8 gInterfaceGfx_FastBallPal[];
extern const u8 gInterfaceGfx_LevelBallPal[];
extern const u8 gInterfaceGfx_LureBallPal[];
extern const u8 gInterfaceGfx_HeavyBallPal[];
extern const u8 gInterfaceGfx_LoveBallPal[];
extern const u8 gInterfaceGfx_FriendBallPal[];
extern const u8 gInterfaceGfx_MoonBallPal[];
extern const u8 gInterfaceGfx_SportBallPal[];
extern const u8 gInterfaceGfx_BeastBallPal[];
extern const u8 gInterfaceGfx_DreamBallPal[];

#define sBallOamData (const struct OamData*) 0x826062C
#define sBallAnimSequences (const union AnimCmd* const*) 0x8260674
#define sBallAffineAnimSequences (const union AffineAnimCmd* const*) 0x82606E0

#define gBattleAnimSpriteSheet_Particles (const u8*) 0x8D1A608
extern const u8 gBattleAnimSpriteSheet_ParticlesDuskBallTiles[];
extern const u8 gBattleAnimSpriteSheet_ParticlesHealBallTiles[];
extern const u8 gBattleAnimSpriteSheet_ParticlesQuickCherishBallTiles[];
extern const u8 gBattleAnimSpriteSheet_ParticlesQuickCherishBallTiles[];
	
#define gBattleAnimSpritePalette_136 (const u8*) 0x8D1A6DC
extern const u8 gBattleAnimSpriteSheet_ParticlesDuskBallPal[];
extern const u8 gBattleAnimSpriteSheet_ParticlesHealBallPal[];
extern const u8 gBattleAnimSpriteSheet_ParticlesQuickCherishBallPal[];
extern const u8 gBattleAnimSpriteSheet_ParticlesQuickCherishBallPal[];

#define gUnknown_083AC9C8 (const struct OamData*) 0x83AC9C8
#define gUnknown_0840C050 (const union AnimCmd* const*) 0x840C050

#define BALL_OPEN_WHITE_STARS 0 //Custom for Heal Ball
#define BALL_OPEN_RAINBOW_CIRCLES 0 //Custom for Cherish Ball
#define BALL_OPEN_YELLOW_CIRCLES 2 //Custom for Quick Ball
#define BALL_OPEN_PURPLE_CIRCLES 2 //Custom for Dusk Ball

enum BallOpenParticles 
{
	BALL_OPEN_STICKS,
	BALL_OPEN_YELLOW_STARS,
	BALL_OPEN_BLUE_BUBBLES,
	BALL_OPEN_HEARTS,
	BALL_OPEN_GREEN_STARS,
	BALL_OPEN_GREEN_BUBBLES,
};

enum BallTags
{
	GFX_TAG_POKEBALL = 55000, //0xD6D8
	GFX_TAG_GREATBALL,
	GFX_TAG_SAFARIBALL,
	GFX_TAG_ULTRABALL,
	GFX_TAG_MASTERBALL,
	GFX_TAG_NETBALL,
	GFX_TAG_DIVEBALL,
	GFX_TAG_NESTBALL,
	GFX_TAG_REPEATBALL,
	GFX_TAG_TIMERBALL,
	GFX_TAG_LUXURYBALL,
	GFX_TAG_PREMIERBALL,
	GFX_TAG_DUSKBALL = 0xFF00, //65280
	GFX_TAG_HEALBALL,
	GFX_TAG_QUICKBALL,
	GFX_TAG_CHERISHBALL,
	GFX_TAG_PARKBALL,
	GFX_TAG_FASTBALL,
	GFX_TAG_LEVELBALL,
	GFX_TAG_LUREBALL,
	GFX_TAG_HEAVYBALL,
	GFX_TAG_LOVEBALL,
	GFX_TAG_FRIENDBALL,
	GFX_TAG_MOONBALL,
	GFX_TAG_SPORTBALL,
	GFX_TAG_BEASTBALL,
	GFX_TAG_DREAMBALL,	
};

enum BallOpenParticleTags
{
	TAG_BALL_OPEN_DUSK = 0xFE00,
	TAG_BALL_OPEN_HEAL,
	TAG_BALL_OPEN_QUICK,
	TAG_BALL_OPEN_CHERISH,
	TAG_BALL_OPEN_PARK,
	TAG_BALL_OPEN_FAST,
	TAG_BALL_OPEN_LEVEL,
	TAG_BALL_OPEN_LURE,
	TAG_BALL_OPEN_HEAVY,
	TAG_BALL_OPEN_LOVE,
	TAG_BALL_OPEN_FRIEND,
	TAG_BALL_OPEN_MOON,
	TAG_BALL_OPEN_SPORT,
	TAG_BALL_OPEN_BEAST,
	TAG_BALL_OPEN_DREAM,
};

const struct BallIdItemIdRelation BallIdItemIdRelations[] =
{
	{ITEM_MASTER_BALL, 		BALL_TYPE_MASTER_BALL},
	{ITEM_ULTRA_BALL, 		BALL_TYPE_ULTRA_BALL},
	{ITEM_GREAT_BALL, 		BALL_TYPE_GREAT_BALL},
	{ITEM_POKE_BALL, 		BALL_TYPE_POKE_BALL},
	{ITEM_SAFARI_BALL, 		BALL_TYPE_SAFARI_BALL},
	{ITEM_NET_BALL, 		BALL_TYPE_NET_BALL},
	{ITEM_DIVE_BALL, 		BALL_TYPE_DIVE_BALL},
	{ITEM_NEST_BALL, 		BALL_TYPE_NEST_BALL},
	{ITEM_REPEAT_BALL, 		BALL_TYPE_REPEAT_BALL},
	{ITEM_TIMER_BALL, 		BALL_TYPE_TIMER_BALL},
	{ITEM_LUXURY_BALL, 		BALL_TYPE_LUXURY_BALL},
	{ITEM_PREMIER_BALL, 	BALL_TYPE_PREMIER_BALL},
	{ITEM_DUSK_BALL, 		BALL_TYPE_DUSK_BALL},
	{ITEM_HEAL_BALL, 		BALL_TYPE_HEAL_BALL},
	{ITEM_QUICK_BALL, 		BALL_TYPE_QUICK_BALL},
	{ITEM_CHERISH_BALL, 	BALL_TYPE_CHERISH_BALL},
	{ITEM_PARK_BALL, 		BALL_TYPE_PARK_BALL},
	{ITEM_FAST_BALL, 		BALL_TYPE_FAST_BALL},
	{ITEM_LEVEL_BALL, 		BALL_TYPE_LEVEL_BALL},
	{ITEM_LURE_BALL, 		BALL_TYPE_LURE_BALL},
	{ITEM_HEAVY_BALL, 		BALL_TYPE_HEAVY_BALL},
	{ITEM_LOVE_BALL, 		BALL_TYPE_LOVE_BALL},
	{ITEM_FRIEND_BALL, 		BALL_TYPE_FRIEND_BALL},
	{ITEM_MOON_BALL, 		BALL_TYPE_MOON_BALL},
	{ITEM_SPORT_BALL, 		BALL_TYPE_SPORT_BALL},
	{ITEM_BEAST_BALL, 		BALL_TYPE_BEAST_BALL},
	{ITEM_DREAM_BALL, 		BALL_TYPE_DREAM_BALL},
};


const struct CompressedSpriteSheet gBallSpriteSheets[POKEBALL_COUNT] =
{
    {gInterfaceGfx_PokeBall,    		(16 * 48) / 2, GFX_TAG_POKEBALL},
    {gInterfaceGfx_GreatBall,   		(16 * 48) / 2, GFX_TAG_GREATBALL},
    {gInterfaceGfx_SafariBall,  		(16 * 48) / 2, GFX_TAG_SAFARIBALL},
    {gInterfaceGfx_UltraBall,   		(16 * 48) / 2, GFX_TAG_ULTRABALL},
    {gInterfaceGfx_MasterBall,  		(16 * 48) / 2, GFX_TAG_MASTERBALL},
    {gInterfaceGfx_NetBall,     		(16 * 48) / 2, GFX_TAG_NETBALL},
    {gInterfaceGfx_DiveBall,    		(16 * 48) / 2, GFX_TAG_DIVEBALL},
    {gInterfaceGfx_NestBall,    		(16 * 48) / 2, GFX_TAG_NESTBALL},
    {gInterfaceGfx_RepeatBall,  		(16 * 48) / 2, GFX_TAG_REPEATBALL},
    {gInterfaceGfx_TimerBall,   		(16 * 48) / 2, GFX_TAG_TIMERBALL},
    {gInterfaceGfx_LuxuryBall,  		(16 * 48) / 2, GFX_TAG_LUXURYBALL},
    {gInterfaceGfx_PremierBall, 		(16 * 48) / 2, GFX_TAG_PREMIERBALL},
	{gInterfaceGfx_DuskBallTiles, 		(16 * 48) / 2, GFX_TAG_DUSKBALL},
	{gInterfaceGfx_HealBallTiles, 		(16 * 48) / 2, GFX_TAG_HEALBALL},
	{gInterfaceGfx_QuickBallTiles, 		(16 * 48) / 2, GFX_TAG_QUICKBALL},
	{gInterfaceGfx_CherishBallTiles, 	(16 * 48) / 2, GFX_TAG_CHERISHBALL},
	{gInterfaceGfx_ParkBallTiles, 		(16 * 48) / 2, GFX_TAG_PARKBALL},
	{gInterfaceGfx_FastBallTiles, 		(16 * 48) / 2, GFX_TAG_FASTBALL},
	{gInterfaceGfx_LevelBallTiles, 		(16 * 48) / 2, GFX_TAG_LEVELBALL},
	{gInterfaceGfx_LureBallTiles, 		(16 * 48) / 2, GFX_TAG_LUREBALL},
	{gInterfaceGfx_HeavyBallTiles, 		(16 * 48) / 2, GFX_TAG_HEAVYBALL},
	{gInterfaceGfx_LoveBallTiles, 		(16 * 48) / 2, GFX_TAG_LOVEBALL},
	{gInterfaceGfx_FriendBallTiles, 	(16 * 48) / 2, GFX_TAG_FRIENDBALL},
	{gInterfaceGfx_MoonBallTiles, 		(16 * 48) / 2, GFX_TAG_MOONBALL},
	{gInterfaceGfx_SportBallTiles, 		(16 * 48) / 2, GFX_TAG_SPORTBALL},
	{gInterfaceGfx_BeastBallTiles, 		(16 * 48) / 2, GFX_TAG_BEASTBALL},
	{gInterfaceGfx_DreamBallTiles, 		(16 * 48) / 2, GFX_TAG_DREAMBALL},
};

const struct CompressedSpritePalette gBallSpritePalettes[POKEBALL_COUNT] =
{
    {gInterfacePal_PokeBall,    	GFX_TAG_POKEBALL},
    {gInterfacePal_GreatBall,   	GFX_TAG_GREATBALL},
    {gInterfacePal_SafariBall,  	GFX_TAG_SAFARIBALL},
    {gInterfacePal_UltraBall,   	GFX_TAG_ULTRABALL},
    {gInterfacePal_MasterBall,  	GFX_TAG_MASTERBALL},
    {gInterfacePal_NetBall,     	GFX_TAG_NETBALL},
    {gInterfacePal_DiveBall,    	GFX_TAG_DIVEBALL},
    {gInterfacePal_NestBall,    	GFX_TAG_NESTBALL},
    {gInterfacePal_RepeatBall,  	GFX_TAG_REPEATBALL},
    {gInterfacePal_TimerBall,   	GFX_TAG_TIMERBALL},
    {gInterfacePal_LuxuryBall,  	GFX_TAG_LUXURYBALL},
    {gInterfacePal_PremierBall, 	GFX_TAG_PREMIERBALL},
	{gInterfaceGfx_DuskBallPal, 	GFX_TAG_DUSKBALL},
	{gInterfaceGfx_HealBallPal, 	GFX_TAG_HEALBALL},
	{gInterfaceGfx_QuickBallPal, 	GFX_TAG_QUICKBALL},
	{gInterfaceGfx_CherishBallPal, 	GFX_TAG_CHERISHBALL},
	{gInterfaceGfx_ParkBallPal, 	GFX_TAG_PARKBALL},
	{gInterfaceGfx_FastBallPal, 	GFX_TAG_FASTBALL},
	{gInterfaceGfx_LevelBallPal, 	GFX_TAG_LEVELBALL},
	{gInterfaceGfx_LureBallPal, 	GFX_TAG_LUREBALL},
	{gInterfaceGfx_HeavyBallPal, 	GFX_TAG_HEAVYBALL},
	{gInterfaceGfx_LoveBallPal, 	GFX_TAG_LOVEBALL},
	{gInterfaceGfx_FriendBallPal, 	GFX_TAG_FRIENDBALL},
	{gInterfaceGfx_MoonBallPal, 	GFX_TAG_MOONBALL},
	{gInterfaceGfx_SportBallPal, 	GFX_TAG_SPORTBALL},
	{gInterfaceGfx_BeastBallPal, 	GFX_TAG_BEASTBALL},
	{gInterfaceGfx_DreamBallPal, 	GFX_TAG_DREAMBALL},
};

const struct SpriteTemplate gBallSpriteTemplates[POKEBALL_COUNT] =
{
    {
        .tileTag = GFX_TAG_POKEBALL,
        .paletteTag = GFX_TAG_POKEBALL,
        .oam = sBallOamData,
        .anims = sBallAnimSequences,
        .images = NULL,
        .affineAnims = sBallAffineAnimSequences,
        .callback = SpriteCB_TestBallThrow,
    },
    {
        .tileTag = GFX_TAG_GREATBALL,
        .paletteTag = GFX_TAG_GREATBALL,
        .oam = sBallOamData,
        .anims = sBallAnimSequences,
        .images = NULL,
        .affineAnims = sBallAffineAnimSequences,
        .callback = SpriteCB_TestBallThrow,
    },
    {
        .tileTag = GFX_TAG_SAFARIBALL,
        .paletteTag = GFX_TAG_SAFARIBALL,
        .oam = sBallOamData,
        .anims = sBallAnimSequences,
        .images = NULL,
        .affineAnims = sBallAffineAnimSequences,
        .callback = SpriteCB_TestBallThrow,
    },
    {
        .tileTag = GFX_TAG_ULTRABALL,
        .paletteTag = GFX_TAG_ULTRABALL,
        .oam = sBallOamData,
        .anims = sBallAnimSequences,
        .images = NULL,
        .affineAnims = sBallAffineAnimSequences,
        .callback = SpriteCB_TestBallThrow,
    },
    {
        .tileTag = GFX_TAG_MASTERBALL,
        .paletteTag = GFX_TAG_MASTERBALL,
        .oam = sBallOamData,
        .anims = sBallAnimSequences,
        .images = NULL,
        .affineAnims = sBallAffineAnimSequences,
        .callback = SpriteCB_TestBallThrow,
    },
    {
        .tileTag = GFX_TAG_NETBALL,
        .paletteTag = GFX_TAG_NETBALL,
        .oam = sBallOamData,
        .anims = sBallAnimSequences,
        .images = NULL,
        .affineAnims = sBallAffineAnimSequences,
        .callback = SpriteCB_TestBallThrow,
    },
    {
        .tileTag = GFX_TAG_DIVEBALL,
        .paletteTag = GFX_TAG_DIVEBALL,
        .oam = sBallOamData,
        .anims = sBallAnimSequences,
        .images = NULL,
        .affineAnims = sBallAffineAnimSequences,
        .callback = SpriteCB_TestBallThrow,
    },
    {
        .tileTag = GFX_TAG_NESTBALL,
        .paletteTag = GFX_TAG_NESTBALL,
        .oam = sBallOamData,
        .anims = sBallAnimSequences,
        .images = NULL,
        .affineAnims = sBallAffineAnimSequences,
        .callback = SpriteCB_TestBallThrow,
    },
    {
        .tileTag = GFX_TAG_REPEATBALL,
        .paletteTag = GFX_TAG_REPEATBALL,
        .oam = sBallOamData,
        .anims = sBallAnimSequences,
        .images = NULL,
        .affineAnims = sBallAffineAnimSequences,
        .callback = SpriteCB_TestBallThrow,
    },
    {
        .tileTag = GFX_TAG_TIMERBALL,
        .paletteTag = GFX_TAG_TIMERBALL,
        .oam = sBallOamData,
        .anims = sBallAnimSequences,
        .images = NULL,
        .affineAnims = sBallAffineAnimSequences,
        .callback = SpriteCB_TestBallThrow,
    },
    {
        .tileTag = GFX_TAG_LUXURYBALL,
        .paletteTag = GFX_TAG_LUXURYBALL,
        .oam = sBallOamData,
        .anims = sBallAnimSequences,
        .images = NULL,
        .affineAnims = sBallAffineAnimSequences,
        .callback = SpriteCB_TestBallThrow,
    },
    {
        .tileTag = GFX_TAG_PREMIERBALL,
        .paletteTag = GFX_TAG_PREMIERBALL,
        .oam = sBallOamData,
        .anims = sBallAnimSequences,
        .images = NULL,
        .affineAnims = sBallAffineAnimSequences,
        .callback = SpriteCB_TestBallThrow,
    },
    {
        .tileTag = GFX_TAG_DUSKBALL,
        .paletteTag = GFX_TAG_DUSKBALL,
        .oam = sBallOamData,
        .anims = sBallAnimSequences,
        .images = NULL,
        .affineAnims = sBallAffineAnimSequences,
        .callback = SpriteCB_TestBallThrow,
    },
	{
        .tileTag = GFX_TAG_HEALBALL,
        .paletteTag = GFX_TAG_HEALBALL,
        .oam = sBallOamData,
        .anims = sBallAnimSequences,
        .images = NULL,
        .affineAnims = sBallAffineAnimSequences,
        .callback = SpriteCB_TestBallThrow,
    },
	{
        .tileTag = GFX_TAG_QUICKBALL,
        .paletteTag = GFX_TAG_QUICKBALL,
        .oam = sBallOamData,
        .anims = sBallAnimSequences,
        .images = NULL,
        .affineAnims = sBallAffineAnimSequences,
        .callback = SpriteCB_TestBallThrow,
    },
	{
        .tileTag = GFX_TAG_CHERISHBALL,
        .paletteTag = GFX_TAG_CHERISHBALL,
        .oam = sBallOamData,
        .anims = sBallAnimSequences,
        .images = NULL,
        .affineAnims = sBallAffineAnimSequences,
        .callback = SpriteCB_TestBallThrow,
    },
	{
        .tileTag = GFX_TAG_PARKBALL,
        .paletteTag = GFX_TAG_PARKBALL,
        .oam = sBallOamData,
        .anims = sBallAnimSequences,
        .images = NULL,
        .affineAnims = sBallAffineAnimSequences,
        .callback = SpriteCB_TestBallThrow,
    },
	{
        .tileTag = GFX_TAG_FASTBALL,
        .paletteTag = GFX_TAG_FASTBALL,
        .oam = sBallOamData,
        .anims = sBallAnimSequences,
        .images = NULL,
        .affineAnims = sBallAffineAnimSequences,
        .callback = SpriteCB_TestBallThrow,
    },
	{
        .tileTag = GFX_TAG_LEVELBALL,
        .paletteTag = GFX_TAG_LEVELBALL,
        .oam = sBallOamData,
        .anims = sBallAnimSequences,
        .images = NULL,
        .affineAnims = sBallAffineAnimSequences,
        .callback = SpriteCB_TestBallThrow,
    },
	{
        .tileTag = GFX_TAG_LUREBALL,
        .paletteTag = GFX_TAG_LUREBALL,
        .oam = sBallOamData,
        .anims = sBallAnimSequences,
        .images = NULL,
        .affineAnims = sBallAffineAnimSequences,
        .callback = SpriteCB_TestBallThrow,
    },
	{
        .tileTag = GFX_TAG_HEAVYBALL,
        .paletteTag = GFX_TAG_HEAVYBALL,
        .oam = sBallOamData,
        .anims = sBallAnimSequences,
        .images = NULL,
        .affineAnims = sBallAffineAnimSequences,
        .callback = SpriteCB_TestBallThrow,
    },
	{
        .tileTag = GFX_TAG_LOVEBALL,
        .paletteTag = GFX_TAG_LOVEBALL,
        .oam = sBallOamData,
        .anims = sBallAnimSequences,
        .images = NULL,
        .affineAnims = sBallAffineAnimSequences,
        .callback = SpriteCB_TestBallThrow,
    },
	{
        .tileTag = GFX_TAG_FRIENDBALL,
        .paletteTag = GFX_TAG_FRIENDBALL,
        .oam = sBallOamData,
        .anims = sBallAnimSequences,
        .images = NULL,
        .affineAnims = sBallAffineAnimSequences,
        .callback = SpriteCB_TestBallThrow,
    },
	{
        .tileTag = GFX_TAG_MOONBALL,
        .paletteTag = GFX_TAG_MOONBALL,
        .oam = sBallOamData,
        .anims = sBallAnimSequences,
        .images = NULL,
        .affineAnims = sBallAffineAnimSequences,
        .callback = SpriteCB_TestBallThrow,
    },
	{
        .tileTag = GFX_TAG_SPORTBALL,
        .paletteTag = GFX_TAG_SPORTBALL,
        .oam = sBallOamData,
        .anims = sBallAnimSequences,
        .images = NULL,
        .affineAnims = sBallAffineAnimSequences,
        .callback = SpriteCB_TestBallThrow,
    },
	{
        .tileTag = GFX_TAG_BEASTBALL,
        .paletteTag = GFX_TAG_BEASTBALL,
        .oam = sBallOamData,
        .anims = sBallAnimSequences,
        .images = NULL,
        .affineAnims = sBallAffineAnimSequences,
        .callback = SpriteCB_TestBallThrow,
    },
	{
        .tileTag = GFX_TAG_DREAMBALL,
        .paletteTag = GFX_TAG_DREAMBALL,
        .oam = sBallOamData,
        .anims = sBallAnimSequences,
        .images = NULL,
        .affineAnims = sBallAffineAnimSequences,
        .callback = SpriteCB_TestBallThrow,
    },
};

const struct CompressedSpriteSheet gBallOpenParticleSpritesheets[] =
{
    {gBattleAnimSpriteSheet_Particles, 							(8 * 64) / 2, 55020},
    {gBattleAnimSpriteSheet_Particles, 							(8 * 64) / 2, 55021},
    {gBattleAnimSpriteSheet_Particles, 							(8 * 64) / 2, 55022},
    {gBattleAnimSpriteSheet_Particles, 							(8 * 64) / 2, 55023},
    {gBattleAnimSpriteSheet_Particles, 							(8 * 64) / 2, 55024},
    {gBattleAnimSpriteSheet_Particles, 							(8 * 64) / 2, 55025},
    {gBattleAnimSpriteSheet_Particles, 							(8 * 64) / 2, 55026},
    {gBattleAnimSpriteSheet_Particles, 							(8 * 64) / 2, 55027},
    {gBattleAnimSpriteSheet_Particles, 							(8 * 64) / 2, 55028},
    {gBattleAnimSpriteSheet_Particles, 							(8 * 64) / 2, 55029},
    {gBattleAnimSpriteSheet_Particles, 							(8 * 64) / 2, 55030},
    {gBattleAnimSpriteSheet_Particles, 							(8 * 64) / 2, 55031},
    {gBattleAnimSpriteSheet_ParticlesDuskBallTiles, 			(8 * 64) / 2, TAG_BALL_OPEN_DUSK},
	{gBattleAnimSpriteSheet_ParticlesHealBallTiles, 			(8 * 64) / 2, TAG_BALL_OPEN_HEAL},
	{gBattleAnimSpriteSheet_ParticlesQuickCherishBallTiles, 	(8 * 64) / 2, TAG_BALL_OPEN_QUICK},
	{gBattleAnimSpriteSheet_ParticlesQuickCherishBallTiles, 	(8 * 64) / 2, TAG_BALL_OPEN_CHERISH},
	{gBattleAnimSpriteSheet_Particles, 							(8 * 64) / 2, TAG_BALL_OPEN_PARK},
	{gBattleAnimSpriteSheet_ParticlesQuickCherishBallTiles, 	(8 * 64) / 2, TAG_BALL_OPEN_FAST},
	{gBattleAnimSpriteSheet_Particles, 							(8 * 64) / 2, TAG_BALL_OPEN_LEVEL},
	{gBattleAnimSpriteSheet_Particles, 							(8 * 64) / 2, TAG_BALL_OPEN_LURE},
	{gBattleAnimSpriteSheet_Particles, 							(8 * 64) / 2, TAG_BALL_OPEN_HEAVY},
	{gBattleAnimSpriteSheet_Particles, 							(8 * 64) / 2, TAG_BALL_OPEN_LOVE},
	{gBattleAnimSpriteSheet_Particles, 							(8 * 64) / 2, TAG_BALL_OPEN_FRIEND},
	{gBattleAnimSpriteSheet_ParticlesHealBallTiles, 			(8 * 64) / 2, TAG_BALL_OPEN_MOON},
	{gBattleAnimSpriteSheet_Particles, 							(8 * 64) / 2, TAG_BALL_OPEN_SPORT},
	{gBattleAnimSpriteSheet_Particles, 							(8 * 64) / 2, TAG_BALL_OPEN_BEAST},
	{gBattleAnimSpriteSheet_Particles, 							(8 * 64) / 2, TAG_BALL_OPEN_DREAM},
};

const struct CompressedSpritePalette gBallOpenParticlePalettes[] =
{
    {gBattleAnimSpritePalette_136, 							55020},
    {gBattleAnimSpritePalette_136, 							55021},
    {gBattleAnimSpritePalette_136, 							55022},
    {gBattleAnimSpritePalette_136, 							55023},
    {gBattleAnimSpritePalette_136, 							55024},
    {gBattleAnimSpritePalette_136, 							55025},
    {gBattleAnimSpritePalette_136, 							55026},
    {gBattleAnimSpritePalette_136, 							55027},
    {gBattleAnimSpritePalette_136, 							55028},
    {gBattleAnimSpritePalette_136, 							55029},
    {gBattleAnimSpritePalette_136, 							55030},
    {gBattleAnimSpritePalette_136, 							55031},
    {gBattleAnimSpriteSheet_ParticlesDuskBallPal, 			TAG_BALL_OPEN_DUSK},
	{gBattleAnimSpriteSheet_ParticlesHealBallPal, 			TAG_BALL_OPEN_HEAL},
	{gBattleAnimSpriteSheet_ParticlesQuickCherishBallPal, 	TAG_BALL_OPEN_QUICK},
	{gBattleAnimSpriteSheet_ParticlesQuickCherishBallPal, 	TAG_BALL_OPEN_CHERISH},
	{gBattleAnimSpritePalette_136, 							TAG_BALL_OPEN_PARK},
	{gBattleAnimSpriteSheet_ParticlesQuickCherishBallPal, 	TAG_BALL_OPEN_FAST},
	{gBattleAnimSpritePalette_136, 							TAG_BALL_OPEN_LEVEL},
	{gBattleAnimSpritePalette_136, 							TAG_BALL_OPEN_LURE},
	{gBattleAnimSpritePalette_136, 							TAG_BALL_OPEN_HEAVY},
	{gBattleAnimSpritePalette_136, 							TAG_BALL_OPEN_LOVE},
	{gBattleAnimSpritePalette_136, 							TAG_BALL_OPEN_FRIEND},
	{gBattleAnimSpriteSheet_ParticlesHealBallPal, 			TAG_BALL_OPEN_MOON},
	{gBattleAnimSpritePalette_136, 							TAG_BALL_OPEN_SPORT},
	{gBattleAnimSpritePalette_136, 							TAG_BALL_OPEN_BEAST},
	{gBattleAnimSpritePalette_136, 							TAG_BALL_OPEN_DREAM},
};

const u8 gBallOpenParticleAnimNums[] =
{
    BALL_OPEN_STICKS, 			//Poke Ball
    BALL_OPEN_STICKS, 			//Great Ball
    BALL_OPEN_STICKS, 			//Safari Ball
    BALL_OPEN_GREEN_BUBBLES, 	//Ultra Ball
    BALL_OPEN_YELLOW_STARS, 	//Master Ball
    BALL_OPEN_BLUE_BUBBLES, 	//Net Ball
    BALL_OPEN_BLUE_BUBBLES, 	//Dive Ball
    BALL_OPEN_HEARTS, 			//Nest Ball
    BALL_OPEN_GREEN_BUBBLES, 	//Repeat Ball
    BALL_OPEN_GREEN_BUBBLES, 	//Timer Ball
    BALL_OPEN_GREEN_STARS, 		//Luxury Ball
    BALL_OPEN_GREEN_STARS, 		//Premier Ball
	BALL_OPEN_PURPLE_CIRCLES, 	//Dusk Ball
	BALL_OPEN_WHITE_STARS, 		//Heal Ball
	BALL_OPEN_YELLOW_CIRCLES, 	//Quick Ball
	BALL_OPEN_RAINBOW_CIRCLES, 	//Cherish Ball
	BALL_OPEN_YELLOW_STARS, 	//Park Ball
	BALL_OPEN_YELLOW_CIRCLES, 	//Fast Ball
	BALL_OPEN_STICKS, 			//Level Ball
	BALL_OPEN_BLUE_BUBBLES, 	//Lure Ball
	BALL_OPEN_BLUE_BUBBLES, 	//Heavy Ball
	BALL_OPEN_HEARTS, 			//Love Ball
	BALL_OPEN_HEARTS, 			//Friend Ball
	BALL_OPEN_WHITE_STARS, 	    //Moon Ball
	BALL_OPEN_STICKS, 			//Sport Ball
	BALL_OPEN_BLUE_BUBBLES, 	//Beast Ball
	BALL_OPEN_HEARTS, 			//Dream Ball
};

const TaskFunc gBallOpenParticleAnimationFuncs[] =
{
    PokeBallOpenParticleAnimation, //Poke Ball
    GreatBallOpenParticleAnimation, //Great Ball
    SafariBallOpenParticleAnimation, //Safari Ball
    UltraBallOpenParticleAnimation, //Ultra Ball
    MasterBallOpenParticleAnimation, //Master Ball
    SafariBallOpenParticleAnimation, //Net Ball
    DiveBallOpenParticleAnimation, //Dive Ball
    UltraBallOpenParticleAnimation, //Nest Ball
    RepeatBallOpenParticleAnimation, //Repeat Ball
    TimerBallOpenParticleAnimation, //Timer Ball
    GreatBallOpenParticleAnimation, //Luxury Ball
    PremierBallOpenParticleAnimation, //Premier Ball
	GreatBallOpenParticleAnimation, //Dusk Ball
	SafariBallOpenParticleAnimation, //Heal Ball
	UltraBallOpenParticleAnimation, //Quick Ball
	DiveBallOpenParticleAnimation, //Cherish Ball
	PokeBallOpenParticleAnimation, //Park Ball
	TimerBallOpenParticleAnimation, //Fast Ball
	SafariBallOpenParticleAnimation, //Level Ball
	SafariBallOpenParticleAnimation, //Lure Ball
	RepeatBallOpenParticleAnimation, //Heavy Ball
	GreatBallOpenParticleAnimation, //Love Ball
	PokeBallOpenParticleAnimation, //Friend Ball
	DiveBallOpenParticleAnimation, //Moon Ball
	PokeBallOpenParticleAnimation, //Sport Ball
	MasterBallOpenParticleAnimation, //Beast Ball
	RepeatBallOpenParticleAnimation, //Dream Ball
};

const u16 gBallOpenMonFadePal[] = //gUnknown_085E5310 in Emerald
{
    RGB(31, 22, 30), //Poke Ball - Pink
    RGB(16, 23, 30), //Great Ball - Dull Light Blue
    RGB(23, 30, 20), //Safari Ball - Light Green
    RGB(31, 31, 15), //Ultra Ball - Yellow
    RGB(23, 20, 28), //Master Ball - Violet
    RGB(21, 31, 25), //Net Ball - Turquoise
    RGB(12, 25, 30), //Dive Ball - Deep Sky Blue
    RGB(30, 27, 10), //Nest Ball - Gold
    RGB(31, 24, 16), //Repeat Ball - Light Orange
    RGB(29, 30, 30), //Timer Ball - Light Gray
    RGB(31, 17, 10), //Luxury Ball - Orange
    RGB(31, 9, 10), //Premier Ball - Red
	RGB(6, 0, 14), //Dusk Ball - Deep Purple
	RGB(31, 25, 31), //Heal Ball - Pink
	RGB(14, 20, 27), //Quick Ball - Dull Blue
	RGB(31, 9, 10), //Cherish Ball - Red
	RGB(27, 25, 5), //Park Ball - Gold
	RGB(31, 25, 4), //Fast Ball - Light Orange
	RGB(29, 23, 16), //Level Ball - Light Brown
	RGB(12, 25, 7), //Lure Ball - Dull Green
	RGB(12, 14, 15), //Heavy Ball - Darkish Gray
	RGB(26, 16, 20), //Love Ball - Pink
	RGB(15, 22, 11), //Friend Ball - Dull Green
	RGB(9, 9, 11), //Moon Ball - Dark Gray
	RGB(30, 17, 17), //Sport Ball - Pinkish Red
	RGB(7, 12, 31), //Beast Ball - Royal Blue
	RGB(31, 12, 20), //Dream Ball - Deep Pink
	
    RGB(0, 0, 0), //No idea what these lower values are for
    RGB(1, 16, 0),
    RGB(3, 0, 1),
    RGB(1, 8, 0),
    RGB(0, 8, 0),
    RGB(3, 8, 1),
    RGB(6, 8, 1),
    RGB(4, 0, 0),
};

const struct SpriteTemplate gBallParticleSpriteTemplates[POKEBALL_COUNT] = //gUnknown_085E51F0 in Emerald
{
    { //GFX_TAG_POKEBALL
        .tileTag = 55020,
        .paletteTag = 55020,
        .oam = gUnknown_083AC9C8, //gUnknown_08524904 in Emerald
        .anims = gUnknown_0840C050, //gUnknown_085E519C in Emerald
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCallbackDummy,
    },
    { //GFX_TAG_GREATBALL
        .tileTag = 55021,
        .paletteTag = 55021,
        .oam = gUnknown_083AC9C8,
        .anims = gUnknown_0840C050,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCallbackDummy,
    },
    { //GFX_TAG_SAFARIBALL
        .tileTag = 55022,
        .paletteTag = 55022,
        .oam = gUnknown_083AC9C8,
        .anims = gUnknown_0840C050,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCallbackDummy,
    },
    { //GFX_TAG_ULTRABALL
        .tileTag = 55023,
        .paletteTag = 55023,
        .oam = gUnknown_083AC9C8,
        .anims = gUnknown_0840C050,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCallbackDummy,
    },
    { //GFX_TAG_MASTERBALL
        .tileTag = 55024,
        .paletteTag = 55024,
        .oam = gUnknown_083AC9C8,
        .anims = gUnknown_0840C050,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCallbackDummy,
    },
    { //GFX_TAG_NETBALL
        .tileTag = 55025,
        .paletteTag = 55025,
        .oam = gUnknown_083AC9C8,
        .anims = gUnknown_0840C050,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCallbackDummy,
    },
    { //GFX_TAG_DIVEBALL
        .tileTag = 55026,
        .paletteTag = 55026,
        .oam = gUnknown_083AC9C8,
        .anims = gUnknown_0840C050,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCallbackDummy,
    },
    { //GFX_TAG_NESTBALL
        .tileTag = 55027,
        .paletteTag = 55027,
        .oam = gUnknown_083AC9C8,
        .anims = gUnknown_0840C050,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCallbackDummy,
    },
    { //GFX_TAG_REPEATBALL
        .tileTag = 55028,
        .paletteTag = 55028,
        .oam = gUnknown_083AC9C8,
        .anims = gUnknown_0840C050,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCallbackDummy,
    },
    { //GFX_TAG_TIMERBALL
        .tileTag = 55029,
        .paletteTag = 55029,
        .oam = gUnknown_083AC9C8,
        .anims = gUnknown_0840C050,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCallbackDummy,
    },
    { //GFX_TAG_LUXURYBALL
        .tileTag = 55030,
        .paletteTag = 55030,
        .oam = gUnknown_083AC9C8,
        .anims = gUnknown_0840C050,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCallbackDummy,
    },
    { //GFX_TAG_PREMIERBALL
        .tileTag = 55031,
        .paletteTag = 55031,
        .oam = gUnknown_083AC9C8,
        .anims = gUnknown_0840C050,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCallbackDummy,
    },
	{ //GFX_TAG_DUSKBALL
        .tileTag = TAG_BALL_OPEN_DUSK,
        .paletteTag = TAG_BALL_OPEN_DUSK,
        .oam = gUnknown_083AC9C8,
        .anims = gUnknown_0840C050,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCallbackDummy,
    },
	{ //GFX_TAG_HEALBALL
        .tileTag = TAG_BALL_OPEN_HEAL,
        .paletteTag = TAG_BALL_OPEN_HEAL,
        .oam = gUnknown_083AC9C8,
        .anims = gUnknown_0840C050,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCallbackDummy,
    },
	{ //GFX_TAG_QUICKBALL
        .tileTag = TAG_BALL_OPEN_QUICK,
        .paletteTag = TAG_BALL_OPEN_QUICK,
        .oam = gUnknown_083AC9C8,
        .anims = gUnknown_0840C050,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCallbackDummy,
    },
	{ //GFX_TAG_CHERISHBALL
        .tileTag = TAG_BALL_OPEN_CHERISH,
        .paletteTag = TAG_BALL_OPEN_CHERISH,
        .oam = gUnknown_083AC9C8,
        .anims = gUnknown_0840C050,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCallbackDummy,
    },
	{ //GFX_TAG_PARKBALL
        .tileTag = TAG_BALL_OPEN_PARK,
        .paletteTag = TAG_BALL_OPEN_PARK,
        .oam = gUnknown_083AC9C8,
        .anims = gUnknown_0840C050,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCallbackDummy,
    },
	{ //GFX_TAG_FASTBALL
        .tileTag = TAG_BALL_OPEN_FAST,
        .paletteTag = TAG_BALL_OPEN_FAST,
        .oam = gUnknown_083AC9C8,
        .anims = gUnknown_0840C050,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCallbackDummy,
    },
	{ //GFX_TAG_LEVELBALL
        .tileTag = TAG_BALL_OPEN_LEVEL,
        .paletteTag = TAG_BALL_OPEN_LEVEL,
        .oam = gUnknown_083AC9C8,
        .anims = gUnknown_0840C050,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCallbackDummy,
    },
	{ //GFX_TAG_LUREBALL
        .tileTag = TAG_BALL_OPEN_LURE,
        .paletteTag = TAG_BALL_OPEN_LURE,
        .oam = gUnknown_083AC9C8,
        .anims = gUnknown_0840C050,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCallbackDummy,
    },
	{ //GFX_TAG_HEAVYBALL
        .tileTag = TAG_BALL_OPEN_HEAVY,
        .paletteTag = TAG_BALL_OPEN_HEAVY,
        .oam = gUnknown_083AC9C8,
        .anims = gUnknown_0840C050,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCallbackDummy,
    },
	{ //GFX_TAG_LOVEBALL
        .tileTag = TAG_BALL_OPEN_LOVE,
        .paletteTag = TAG_BALL_OPEN_LOVE,
        .oam = gUnknown_083AC9C8,
        .anims = gUnknown_0840C050,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCallbackDummy,
    },
	{ //GFX_TAG_FRIENDBALL
        .tileTag = TAG_BALL_OPEN_FRIEND,
        .paletteTag = TAG_BALL_OPEN_FRIEND,
        .oam = gUnknown_083AC9C8,
        .anims = gUnknown_0840C050,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCallbackDummy,
    },
	{ //GFX_TAG_MOONBALL
        .tileTag = TAG_BALL_OPEN_MOON,
        .paletteTag = TAG_BALL_OPEN_MOON,
        .oam = gUnknown_083AC9C8,
        .anims = gUnknown_0840C050,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCallbackDummy,
    },
	{ //GFX_TAG_SPORTBALL
        .tileTag = TAG_BALL_OPEN_SPORT,
        .paletteTag = TAG_BALL_OPEN_SPORT,
        .oam = gUnknown_083AC9C8,
        .anims = gUnknown_0840C050,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCallbackDummy,
    },
	{ //GFX_TAG_BEASTBALL
        .tileTag = TAG_BALL_OPEN_BEAST,
        .paletteTag = TAG_BALL_OPEN_BEAST,
        .oam = gUnknown_083AC9C8,
        .anims = gUnknown_0840C050,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCallbackDummy,
    },
	{ //GFX_TAG_DREAMBALL
        .tileTag = TAG_BALL_OPEN_DREAM,
        .paletteTag = TAG_BALL_OPEN_DREAM,
        .oam = gUnknown_083AC9C8,
        .anims = gUnknown_0840C050,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCallbackDummy,
    },
};